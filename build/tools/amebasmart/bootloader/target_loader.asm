
/media/jw/E/AIDual_KM4/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


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
300032c4:	30009e75 	.word	0x30009e75
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	30009c7d 	.word	0x30009c7d
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
300033fc:	30009e75 	.word	0x30009e75
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	30009c7d 	.word	0x30009c7d
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
3000349c:	f008 fe58 	bl	3000c150 <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000c6dc 	.word	0x3000c6dc
300034bc:	3000c6e8 	.word	0x3000c6e8

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
300034f0:	f008 fe2e 	bl	3000c150 <rtk_log_write>
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
30003524:	f008 fe14 	bl	3000c150 <rtk_log_write>
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
30003540:	3000c6dc 	.word	0x3000c6dc
30003544:	3000c72c 	.word	0x3000c72c
30003548:	30009e75 	.word	0x30009e75
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000c70c 	.word	0x3000c70c
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
3000365a:	f008 fd79 	bl	3000c150 <rtk_log_write>
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
3000367a:	f008 fd69 	bl	3000c150 <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000d830 	.word	0x3000d830
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000c6dc 	.word	0x3000c6dc
300036a8:	3000c764 	.word	0x3000c764
300036ac:	3000c74c 	.word	0x3000c74c
300036b0:	30009e75 	.word	0x30009e75
300036b4:	30009c7d 	.word	0x30009c7d
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
3000373e:	f008 fd07 	bl	3000c150 <rtk_log_write>
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
30003790:	f008 fcde 	bl	3000c150 <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 ff21 	bl	3000c5e0 <__io_assert_failed_veneer>
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
300037ca:	f008 fcc1 	bl	3000c150 <rtk_log_write>
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
30003816:	f008 fc9b 	bl	3000c150 <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f008 fede 	bl	3000c5e0 <__io_assert_failed_veneer>
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
30003958:	3000d7a0 	.word	0x3000d7a0
3000395c:	3000d7a2 	.word	0x3000d7a2
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000c6dc 	.word	0x3000c6dc
3000396c:	3000c788 	.word	0x3000c788
30003970:	3000d69c 	.word	0x3000d69c
30003974:	3000d7a4 	.word	0x3000d7a4
30003978:	3000c7a4 	.word	0x3000c7a4
3000397c:	3000d854 	.word	0x3000d854
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000c7d0 	.word	0x3000c7d0
30003988:	3000d844 	.word	0x3000d844
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000fe71 	.word	0x3000fe71
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	30009e75 	.word	0x30009e75
300039b8:	3000fe74 	.word	0x3000fe74
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
30003a36:	f008 fdd3 	bl	3000c5e0 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f008 fdca 	bl	3000c5e0 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000fe71 	.word	0x3000fe71
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000d844 	.word	0x3000d844
30003a78:	3000d854 	.word	0x3000d854
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
30003afc:	f004 f886 	bl	30007c0c <ymodem_uart_getdata>
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
30003bd4:	f008 fabc 	bl	3000c150 <rtk_log_write>
30003bd8:	2244      	movs	r2, #68	; 0x44
30003bda:	2100      	movs	r1, #0
30003bdc:	a803      	add	r0, sp, #12
30003bde:	f008 fd03 	bl	3000c5e8 <____wrap_memset_veneer>
30003be2:	0672      	lsls	r2, r6, #25
30003be4:	4b34      	ldr	r3, [pc, #208]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003be6:	d555      	bpl.n	30003c94 <BOOT_FaultHandler+0xd4>
30003be8:	4619      	mov	r1, r3
30003bea:	4a35      	ldr	r2, [pc, #212]	; (30003cc0 <BOOT_FaultHandler+0x100>)
30003bec:	2001      	movs	r0, #1
30003bee:	f008 faaf 	bl	3000c150 <rtk_log_write>
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
30003c3c:	f007 ffb2 	bl	3000bba4 <crash_dump>
30003c40:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30003c44:	4b1c      	ldr	r3, [pc, #112]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003c46:	2001      	movs	r0, #1
30003c48:	9100      	str	r1, [sp, #0]
30003c4a:	4a1f      	ldr	r2, [pc, #124]	; (30003cc8 <BOOT_FaultHandler+0x108>)
30003c4c:	4619      	mov	r1, r3
30003c4e:	f008 fa7f 	bl	3000c150 <rtk_log_write>
30003c52:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30003c56:	4b18      	ldr	r3, [pc, #96]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003c58:	2001      	movs	r0, #1
30003c5a:	9200      	str	r2, [sp, #0]
30003c5c:	4619      	mov	r1, r3
30003c5e:	4a1b      	ldr	r2, [pc, #108]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003c60:	f008 fa76 	bl	3000c150 <rtk_log_write>
30003c64:	6a61      	ldr	r1, [r4, #36]	; 0x24
30003c66:	4b14      	ldr	r3, [pc, #80]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003c68:	2001      	movs	r0, #1
30003c6a:	9100      	str	r1, [sp, #0]
30003c6c:	4a18      	ldr	r2, [pc, #96]	; (30003cd0 <BOOT_FaultHandler+0x110>)
30003c6e:	4619      	mov	r1, r3
30003c70:	f008 fa6e 	bl	3000c150 <rtk_log_write>
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
30003c8c:	f008 fce0 	bl	3000c650 <__INT_HardFault_C_veneer>
30003c90:	b015      	add	sp, #84	; 0x54
30003c92:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003c94:	4a10      	ldr	r2, [pc, #64]	; (30003cd8 <BOOT_FaultHandler+0x118>)
30003c96:	2001      	movs	r0, #1
30003c98:	4619      	mov	r1, r3
30003c9a:	f008 fa59 	bl	3000c150 <rtk_log_write>
30003c9e:	f3ef 8394 	mrs	r3, CONTROL_NS
30003ca2:	f3ef 8e88 	mrs	lr, MSP_NS
30003ca6:	f3ef 8589 	mrs	r5, PSP_NS
30003caa:	f016 0108 	ands.w	r1, r6, #8
30003cae:	bf18      	it	ne
30003cb0:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30003cb4:	e7a5      	b.n	30003c02 <BOOT_FaultHandler+0x42>
30003cb6:	bf00      	nop
30003cb8:	3000c810 	.word	0x3000c810
30003cbc:	3000c818 	.word	0x3000c818
30003cc0:	3000c83c 	.word	0x3000c83c
30003cc4:	e000ed00 	.word	0xe000ed00
30003cc8:	3000c890 	.word	0x3000c890
30003ccc:	3000c8a8 	.word	0x3000c8a8
30003cd0:	3000c8c0 	.word	0x3000c8c0
30003cd4:	0000994d 	.word	0x0000994d
30003cd8:	3000c864 	.word	0x3000c864

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
30003ec8:	3000eac0 	.word	0x3000eac0

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
30003eee:	f008 fb8b 	bl	3000c608 <____wrap_memcpy_veneer>
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
30003fde:	f008 fb5f 	bl	3000c6a0 <__DiagPrintf_veneer>
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
30004042:	f008 fb2d 	bl	3000c6a0 <__DiagPrintf_veneer>
30004046:	2001      	movs	r0, #1
30004048:	b023      	add	sp, #140	; 0x8c
3000404a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000404e:	992a      	ldr	r1, [sp, #168]	; 0xa8
30004050:	4640      	mov	r0, r8
30004052:	f008 fad9 	bl	3000c608 <____wrap_memcpy_veneer>
30004056:	e74c      	b.n	30003ef2 <rewrite_bp+0x26>
30004058:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
3000405c:	4809      	ldr	r0, [pc, #36]	; (30004084 <rewrite_bp+0x1b8>)
3000405e:	f8d6 1006 	ldr.w	r1, [r6, #6]
30004062:	f008 fb1d 	bl	3000c6a0 <__DiagPrintf_veneer>
30004066:	2002      	movs	r0, #2
30004068:	b023      	add	sp, #140	; 0x8c
3000406a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000406e:	bf00      	nop
30004070:	3000eac0 	.word	0x3000eac0
30004074:	2001c01c 	.word	0x2001c01c
30004078:	00009db5 	.word	0x00009db5
3000407c:	3000c908 	.word	0x3000c908
30004080:	3000c8d8 	.word	0x3000c8d8
30004084:	3000c8f0 	.word	0x3000c8f0

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
300041f4:	3000e5a0 	.word	0x3000e5a0
300041f8:	3000e904 	.word	0x3000e904
300041fc:	42008000 	.word	0x42008000
30004200:	00009b2d 	.word	0x00009b2d
30004204:	30007f11 	.word	0x30007f11

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
3000425c:	3000f128 	.word	0x3000f128
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
30004538:	3000f064 	.word	0x3000f064
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
300045d8:	f240 10ff 	movw	r0, #511	; 0x1ff
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
300045f8:	3000802d 	.word	0x3000802d
300045fc:	30008531 	.word	0x30008531
30004600:	3000b581 	.word	0x3000b581
30004604:	30008601 	.word	0x30008601
30004608:	30008605 	.word	0x30008605
3000460c:	3000b529 	.word	0x3000b529

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
30004634:	f007 fd8c 	bl	3000c150 <rtk_log_write>
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
30004684:	30008fc9 	.word	0x30008fc9
30004688:	30008fe9 	.word	0x30008fe9
3000468c:	30009051 	.word	0x30009051
30004690:	30007ec5 	.word	0x30007ec5
30004694:	3000c810 	.word	0x3000c810
30004698:	3000c928 	.word	0x3000c928
3000469c:	300092f5 	.word	0x300092f5
300046a0:	42008000 	.word	0x42008000
300046a4:	3000ff88 	.word	0x3000ff88
300046a8:	41002000 	.word	0x41002000
300046ac:	3000fe80 	.word	0x3000fe80
300046b0:	300095d5 	.word	0x300095d5
300046b4:	3000921d 	.word	0x3000921d
300046b8:	30009331 	.word	0x30009331

300046bc <BOOT_WakeFromPG>:
300046bc:	b538      	push	{r3, r4, r5, lr}
300046be:	4c49      	ldr	r4, [pc, #292]	; (300047e4 <BOOT_WakeFromPG+0x128>)
300046c0:	f002 ffb2 	bl	30007628 <BOOT_RAM_TZCfg>
300046c4:	69a3      	ldr	r3, [r4, #24]
300046c6:	b10b      	cbz	r3, 300046cc <BOOT_WakeFromPG+0x10>
300046c8:	4a47      	ldr	r2, [pc, #284]	; (300047e8 <BOOT_WakeFromPG+0x12c>)
300046ca:	6093      	str	r3, [r2, #8]
300046cc:	4b47      	ldr	r3, [pc, #284]	; (300047ec <BOOT_WakeFromPG+0x130>)
300046ce:	f06f 0c1a 	mvn.w	ip, #26
300046d2:	4847      	ldr	r0, [pc, #284]	; (300047f0 <BOOT_WakeFromPG+0x134>)
300046d4:	f103 0e50 	add.w	lr, r3, #80	; 0x50
300046d8:	ebac 0c04 	sub.w	ip, ip, r4
300046dc:	eb0c 0203 	add.w	r2, ip, r3
300046e0:	f813 1f01 	ldrb.w	r1, [r3, #1]!
300046e4:	4402      	add	r2, r0
300046e6:	4573      	cmp	r3, lr
300046e8:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
300046ec:	d1f6      	bne.n	300046dc <BOOT_WakeFromPG+0x20>
300046ee:	6822      	ldr	r2, [r4, #0]
300046f0:	4b3d      	ldr	r3, [pc, #244]	; (300047e8 <BOOT_WakeFromPG+0x12c>)
300046f2:	f422 7280 	bic.w	r2, r2, #256	; 0x100
300046f6:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
300046fa:	6002      	str	r2, [r0, #0]
300046fc:	6045      	str	r5, [r0, #4]
300046fe:	6081      	str	r1, [r0, #8]
30004700:	6a59      	ldr	r1, [r3, #36]	; 0x24
30004702:	4a3c      	ldr	r2, [pc, #240]	; (300047f4 <BOOT_WakeFromPG+0x138>)
30004704:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30004708:	4d3b      	ldr	r5, [pc, #236]	; (300047f8 <BOOT_WakeFromPG+0x13c>)
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
3000473c:	4b2f      	ldr	r3, [pc, #188]	; (300047fc <BOOT_WakeFromPG+0x140>)
3000473e:	6818      	ldr	r0, [r3, #0]
30004740:	f7ff fd90 	bl	30004264 <BOOT_TCMSet>
30004744:	2201      	movs	r2, #1
30004746:	492e      	ldr	r1, [pc, #184]	; (30004800 <BOOT_WakeFromPG+0x144>)
30004748:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
3000474c:	47a8      	blx	r5
3000474e:	4b2d      	ldr	r3, [pc, #180]	; (30004804 <BOOT_WakeFromPG+0x148>)
30004750:	4798      	blx	r3
30004752:	4b2d      	ldr	r3, [pc, #180]	; (30004808 <BOOT_WakeFromPG+0x14c>)
30004754:	4798      	blx	r3
30004756:	2801      	cmp	r0, #1
30004758:	4604      	mov	r4, r0
3000475a:	d026      	beq.n	300047aa <BOOT_WakeFromPG+0xee>
3000475c:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30004760:	2201      	movs	r2, #1
30004762:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30004766:	47a8      	blx	r5
30004768:	4a28      	ldr	r2, [pc, #160]	; (3000480c <BOOT_WakeFromPG+0x150>)
3000476a:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000476e:	f043 0302 	orr.w	r3, r3, #2
30004772:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30004776:	f7ff ff4b 	bl	30004610 <BOOT_PSRAM_Init>
3000477a:	2096      	movs	r0, #150	; 0x96
3000477c:	4b24      	ldr	r3, [pc, #144]	; (30004810 <BOOT_WakeFromPG+0x154>)
3000477e:	4798      	blx	r3
30004780:	4a24      	ldr	r2, [pc, #144]	; (30004814 <BOOT_WakeFromPG+0x158>)
30004782:	4c1c      	ldr	r4, [pc, #112]	; (300047f4 <BOOT_WakeFromPG+0x138>)
30004784:	6893      	ldr	r3, [r2, #8]
30004786:	4924      	ldr	r1, [pc, #144]	; (30004818 <BOOT_WakeFromPG+0x15c>)
30004788:	6852      	ldr	r2, [r2, #4]
3000478a:	4824      	ldr	r0, [pc, #144]	; (3000481c <BOOT_WakeFromPG+0x160>)
3000478c:	605a      	str	r2, [r3, #4]
3000478e:	60a3      	str	r3, [r4, #8]
30004790:	4a23      	ldr	r2, [pc, #140]	; (30004820 <BOOT_WakeFromPG+0x164>)
30004792:	61c8      	str	r0, [r1, #28]
30004794:	f382 8888 	msr	MSP_NS, r2
30004798:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
3000479c:	f382 8809 	msr	PSP, r2
300047a0:	6858      	ldr	r0, [r3, #4]
300047a2:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300047a6:	f7ff bc73 	b.w	30004090 <BOOT_NsStart>
300047aa:	4b1e      	ldr	r3, [pc, #120]	; (30004824 <BOOT_WakeFromPG+0x168>)
300047ac:	2004      	movs	r0, #4
300047ae:	4a1e      	ldr	r2, [pc, #120]	; (30004828 <BOOT_WakeFromPG+0x16c>)
300047b0:	4619      	mov	r1, r3
300047b2:	f007 fccd 	bl	3000c150 <rtk_log_write>
300047b6:	4622      	mov	r2, r4
300047b8:	491c      	ldr	r1, [pc, #112]	; (3000482c <BOOT_WakeFromPG+0x170>)
300047ba:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300047be:	47a8      	blx	r5
300047c0:	4622      	mov	r2, r4
300047c2:	491b      	ldr	r1, [pc, #108]	; (30004830 <BOOT_WakeFromPG+0x174>)
300047c4:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300047c8:	47a8      	blx	r5
300047ca:	f7ff fee3 	bl	30004594 <BOOT_DDR_Init>
300047ce:	4a0f      	ldr	r2, [pc, #60]	; (3000480c <BOOT_WakeFromPG+0x150>)
300047d0:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300047d4:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300047d8:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300047dc:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300047e0:	e7ce      	b.n	30004780 <BOOT_WakeFromPG+0xc4>
300047e2:	bf00      	nop
300047e4:	3000ff1c 	.word	0x3000ff1c
300047e8:	e000ed00 	.word	0xe000ed00
300047ec:	3000ff37 	.word	0x3000ff37
300047f0:	e000e100 	.word	0xe000e100
300047f4:	e002ed00 	.word	0xe002ed00
300047f8:	0000b479 	.word	0x0000b479
300047fc:	3000e8fc 	.word	0x3000e8fc
30004800:	40001000 	.word	0x40001000
30004804:	0000d835 	.word	0x0000d835
30004808:	30007ded 	.word	0x30007ded
3000480c:	42008000 	.word	0x42008000
30004810:	00009b2d 	.word	0x00009b2d
30004814:	60000020 	.word	0x60000020
30004818:	30000000 	.word	0x30000000
3000481c:	30003a81 	.word	0x30003a81
30004820:	2001bffc 	.word	0x2001bffc
30004824:	3000c810 	.word	0x3000c810
30004828:	3000c93c 	.word	0x3000c93c
3000482c:	40080000 	.word	0x40080000
30004830:	40040000 	.word	0x40040000

30004834 <BOOT_SCBConfig_HP>:
30004834:	4b0e      	ldr	r3, [pc, #56]	; (30004870 <BOOT_SCBConfig_HP+0x3c>)
30004836:	4a0f      	ldr	r2, [pc, #60]	; (30004874 <BOOT_SCBConfig_HP+0x40>)
30004838:	6a59      	ldr	r1, [r3, #36]	; 0x24
3000483a:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000483e:	6259      	str	r1, [r3, #36]	; 0x24
30004840:	6a51      	ldr	r1, [r2, #36]	; 0x24
30004842:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004846:	6251      	str	r1, [r2, #36]	; 0x24
30004848:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000484c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004850:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004854:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004858:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000485c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004860:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004864:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004868:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000486c:	4770      	bx	lr
3000486e:	bf00      	nop
30004870:	e000ed00 	.word	0xe000ed00
30004874:	e002ed00 	.word	0xe002ed00

30004878 <BOOT_SCBVTORBackup_HP>:
30004878:	4a02      	ldr	r2, [pc, #8]	; (30004884 <BOOT_SCBVTORBackup_HP+0xc>)
3000487a:	4b03      	ldr	r3, [pc, #12]	; (30004888 <BOOT_SCBVTORBackup_HP+0x10>)
3000487c:	6892      	ldr	r2, [r2, #8]
3000487e:	619a      	str	r2, [r3, #24]
30004880:	4770      	bx	lr
30004882:	bf00      	nop
30004884:	e000ed00 	.word	0xe000ed00
30004888:	3000ff1c 	.word	0x3000ff1c

3000488c <BOOT_SCBVTORReFill_HP>:
3000488c:	4a02      	ldr	r2, [pc, #8]	; (30004898 <BOOT_SCBVTORReFill_HP+0xc>)
3000488e:	4b03      	ldr	r3, [pc, #12]	; (3000489c <BOOT_SCBVTORReFill_HP+0x10>)
30004890:	6992      	ldr	r2, [r2, #24]
30004892:	609a      	str	r2, [r3, #8]
30004894:	4770      	bx	lr
30004896:	bf00      	nop
30004898:	3000ff1c 	.word	0x3000ff1c
3000489c:	e000ed00 	.word	0xe000ed00

300048a0 <BOOT_NVICBackup_HP>:
300048a0:	491a      	ldr	r1, [pc, #104]	; (3000490c <BOOT_NVICBackup_HP+0x6c>)
300048a2:	f06f 001a 	mvn.w	r0, #26
300048a6:	b410      	push	{r4}
300048a8:	4c19      	ldr	r4, [pc, #100]	; (30004910 <BOOT_NVICBackup_HP+0x70>)
300048aa:	680a      	ldr	r2, [r1, #0]
300048ac:	f104 031b 	add.w	r3, r4, #27
300048b0:	1b00      	subs	r0, r0, r4
300048b2:	6022      	str	r2, [r4, #0]
300048b4:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
300048b8:	684a      	ldr	r2, [r1, #4]
300048ba:	6062      	str	r2, [r4, #4]
300048bc:	688a      	ldr	r2, [r1, #8]
300048be:	60a2      	str	r2, [r4, #8]
300048c0:	18c2      	adds	r2, r0, r3
300048c2:	440a      	add	r2, r1
300048c4:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
300048c8:	f803 2f01 	strb.w	r2, [r3, #1]!
300048cc:	4563      	cmp	r3, ip
300048ce:	d1f7      	bne.n	300048c0 <BOOT_NVICBackup_HP+0x20>
300048d0:	4b10      	ldr	r3, [pc, #64]	; (30004914 <BOOT_NVICBackup_HP+0x74>)
300048d2:	695b      	ldr	r3, [r3, #20]
300048d4:	03db      	lsls	r3, r3, #15
300048d6:	d512      	bpl.n	300048fe <BOOT_NVICBackup_HP+0x5e>
300048d8:	f014 011f 	ands.w	r1, r4, #31
300048dc:	4b0c      	ldr	r3, [pc, #48]	; (30004910 <BOOT_NVICBackup_HP+0x70>)
300048de:	d111      	bne.n	30004904 <BOOT_NVICBackup_HP+0x64>
300048e0:	216c      	movs	r1, #108	; 0x6c
300048e2:	f3bf 8f4f 	dsb	sy
300048e6:	480b      	ldr	r0, [pc, #44]	; (30004914 <BOOT_NVICBackup_HP+0x74>)
300048e8:	4419      	add	r1, r3
300048ea:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
300048ee:	3320      	adds	r3, #32
300048f0:	1aca      	subs	r2, r1, r3
300048f2:	2a00      	cmp	r2, #0
300048f4:	dcf9      	bgt.n	300048ea <BOOT_NVICBackup_HP+0x4a>
300048f6:	f3bf 8f4f 	dsb	sy
300048fa:	f3bf 8f6f 	isb	sy
300048fe:	f85d 4b04 	ldr.w	r4, [sp], #4
30004902:	4770      	bx	lr
30004904:	f024 031f 	bic.w	r3, r4, #31
30004908:	316c      	adds	r1, #108	; 0x6c
3000490a:	e7ea      	b.n	300048e2 <BOOT_NVICBackup_HP+0x42>
3000490c:	e000e100 	.word	0xe000e100
30004910:	3000ff1c 	.word	0x3000ff1c
30004914:	e000ed00 	.word	0xe000ed00

30004918 <BOOT_NVICReFill_HP>:
30004918:	b510      	push	{r4, lr}
3000491a:	f06f 0c1a 	mvn.w	ip, #26
3000491e:	4c0d      	ldr	r4, [pc, #52]	; (30004954 <BOOT_NVICReFill_HP+0x3c>)
30004920:	480d      	ldr	r0, [pc, #52]	; (30004958 <BOOT_NVICReFill_HP+0x40>)
30004922:	f104 031b 	add.w	r3, r4, #27
30004926:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
3000492a:	ebac 0c04 	sub.w	ip, ip, r4
3000492e:	eb0c 0203 	add.w	r2, ip, r3
30004932:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30004936:	4402      	add	r2, r0
30004938:	4573      	cmp	r3, lr
3000493a:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
3000493e:	d1f6      	bne.n	3000492e <BOOT_NVICReFill_HP+0x16>
30004940:	e9d4 3100 	ldrd	r3, r1, [r4]
30004944:	68a2      	ldr	r2, [r4, #8]
30004946:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000494a:	6003      	str	r3, [r0, #0]
3000494c:	6041      	str	r1, [r0, #4]
3000494e:	6082      	str	r2, [r0, #8]
30004950:	bd10      	pop	{r4, pc}
30004952:	bf00      	nop
30004954:	3000ff1c 	.word	0x3000ff1c
30004958:	e000e100 	.word	0xe000e100

3000495c <BOOT_ImgCopy>:
3000495c:	b510      	push	{r4, lr}
3000495e:	b082      	sub	sp, #8
30004960:	4b09      	ldr	r3, [pc, #36]	; (30004988 <BOOT_ImgCopy+0x2c>)
30004962:	4604      	mov	r4, r0
30004964:	e9cd 2100 	strd	r2, r1, [sp]
30004968:	4798      	blx	r3
3000496a:	e9dd 2100 	ldrd	r2, r1, [sp]
3000496e:	b128      	cbz	r0, 3000497c <BOOT_ImgCopy+0x20>
30004970:	4620      	mov	r0, r4
30004972:	4b06      	ldr	r3, [pc, #24]	; (3000498c <BOOT_ImgCopy+0x30>)
30004974:	b002      	add	sp, #8
30004976:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000497a:	4718      	bx	r3
3000497c:	4620      	mov	r0, r4
3000497e:	b002      	add	sp, #8
30004980:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004984:	f003 bfa4 	b.w	300088d0 <NandImgCopy>
30004988:	30009e75 	.word	0x30009e75
3000498c:	00012a1d 	.word	0x00012a1d

30004990 <BOOT_LoadImages>:
30004990:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004994:	4b57      	ldr	r3, [pc, #348]	; (30004af4 <BOOT_LoadImages+0x164>)
30004996:	2700      	movs	r7, #0
30004998:	2104      	movs	r1, #4
3000499a:	2601      	movs	r6, #1
3000499c:	ed2d 8b02 	vpush	{d8}
300049a0:	b0b7      	sub	sp, #220	; 0xdc
300049a2:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
300049a6:	4b54      	ldr	r3, [pc, #336]	; (30004af8 <BOOT_LoadImages+0x168>)
300049a8:	aa11      	add	r2, sp, #68	; 0x44
300049aa:	9703      	str	r7, [sp, #12]
300049ac:	9705      	str	r7, [sp, #20]
300049ae:	4798      	blx	r3
300049b0:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
300049b4:	409e      	lsls	r6, r3
300049b6:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
300049ba:	f000 823a 	beq.w	30004e32 <BOOT_LoadImages+0x4a2>
300049be:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
300049c2:	f000 8375 	beq.w	300050b0 <BOOT_LoadImages+0x720>
300049c6:	4c4d      	ldr	r4, [pc, #308]	; (30004afc <BOOT_LoadImages+0x16c>)
300049c8:	f04f 0800 	mov.w	r8, #0
300049cc:	4d4c      	ldr	r5, [pc, #304]	; (30004b00 <BOOT_LoadImages+0x170>)
300049ce:	eb06 0904 	add.w	r9, r6, r4
300049d2:	484c      	ldr	r0, [pc, #304]	; (30004b04 <BOOT_LoadImages+0x174>)
300049d4:	4435      	add	r5, r6
300049d6:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
300049da:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
300049de:	f7ff f9ff 	bl	30003de0 <mcc_init_crc32>
300049e2:	f8d9 3004 	ldr.w	r3, [r9, #4]
300049e6:	f8d5 a008 	ldr.w	sl, [r5, #8]
300049ea:	f899 200c 	ldrb.w	r2, [r9, #12]
300049ee:	9308      	str	r3, [sp, #32]
300049f0:	3b01      	subs	r3, #1
300049f2:	9206      	str	r2, [sp, #24]
300049f4:	3303      	adds	r3, #3
300049f6:	686a      	ldr	r2, [r5, #4]
300049f8:	f895 b00c 	ldrb.w	fp, [r5, #12]
300049fc:	9204      	str	r2, [sp, #16]
300049fe:	f200 81bc 	bhi.w	30004d7a <BOOT_LoadImages+0x3ea>
30004a02:	f8d9 2008 	ldr.w	r2, [r9, #8]
30004a06:	9b04      	ldr	r3, [sp, #16]
30004a08:	3a01      	subs	r2, #1
30004a0a:	3b01      	subs	r3, #1
30004a0c:	3203      	adds	r2, #3
30004a0e:	d868      	bhi.n	30004ae2 <BOOT_LoadImages+0x152>
30004a10:	9a06      	ldr	r2, [sp, #24]
30004a12:	9307      	str	r3, [sp, #28]
30004a14:	2a01      	cmp	r2, #1
30004a16:	f240 8305 	bls.w	30005024 <BOOT_LoadImages+0x694>
30004a1a:	483b      	ldr	r0, [pc, #236]	; (30004b08 <BOOT_LoadImages+0x178>)
30004a1c:	f007 fe40 	bl	3000c6a0 <__DiagPrintf_veneer>
30004a20:	9b07      	ldr	r3, [sp, #28]
30004a22:	3303      	adds	r3, #3
30004a24:	d864      	bhi.n	30004af0 <BOOT_LoadImages+0x160>
30004a26:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30004a2a:	f11a 0f03 	cmn.w	sl, #3
30004a2e:	f200 81aa 	bhi.w	30004d86 <BOOT_LoadImages+0x3f6>
30004a32:	f1bb 0f01 	cmp.w	fp, #1
30004a36:	f200 8347 	bhi.w	300050c8 <BOOT_LoadImages+0x738>
30004a3a:	9904      	ldr	r1, [sp, #16]
30004a3c:	4833      	ldr	r0, [pc, #204]	; (30004b0c <BOOT_LoadImages+0x17c>)
30004a3e:	f007 fe2f 	bl	3000c6a0 <__DiagPrintf_veneer>
30004a42:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004a46:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30004a4a:	4c2e      	ldr	r4, [pc, #184]	; (30004b04 <BOOT_LoadImages+0x174>)
30004a4c:	9307      	str	r3, [sp, #28]
30004a4e:	2300      	movs	r3, #0
30004a50:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30004a54:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004a58:	9309      	str	r3, [sp, #36]	; 0x24
30004a5a:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30004b24 <BOOT_LoadImages+0x194>
30004a5e:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30004a62:	6829      	ldr	r1, [r5, #0]
30004a64:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
30004a68:	44b4      	add	ip, r6
30004a6a:	f81c 2b01 	ldrb.w	r2, [ip], #1
30004a6e:	4042      	eors	r2, r0
30004a70:	459c      	cmp	ip, r3
30004a72:	b2d2      	uxtb	r2, r2
30004a74:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30004a78:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30004a7c:	d1f5      	bne.n	30004a6a <BOOT_LoadImages+0xda>
30004a7e:	43c2      	mvns	r2, r0
30004a80:	930f      	str	r3, [sp, #60]	; 0x3c
30004a82:	4291      	cmp	r1, r2
30004a84:	f000 8301 	beq.w	3000508a <BOOT_LoadImages+0x6fa>
30004a88:	4821      	ldr	r0, [pc, #132]	; (30004b10 <BOOT_LoadImages+0x180>)
30004a8a:	f007 fe09 	bl	3000c6a0 <__DiagPrintf_veneer>
30004a8e:	2200      	movs	r2, #0
30004a90:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004a92:	9909      	ldr	r1, [sp, #36]	; 0x24
30004a94:	2900      	cmp	r1, #0
30004a96:	d04e      	beq.n	30004b36 <BOOT_LoadImages+0x1a6>
30004a98:	f899 100c 	ldrb.w	r1, [r9, #12]
30004a9c:	4553      	cmp	r3, sl
30004a9e:	9106      	str	r1, [sp, #24]
30004aa0:	d342      	bcc.n	30004b28 <BOOT_LoadImages+0x198>
30004aa2:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30004aa6:	45e2      	cmp	sl, ip
30004aa8:	d93e      	bls.n	30004b28 <BOOT_LoadImages+0x198>
30004aaa:	9807      	ldr	r0, [sp, #28]
30004aac:	4560      	cmp	r0, ip
30004aae:	f240 8203 	bls.w	30004eb8 <BOOT_LoadImages+0x528>
30004ab2:	4283      	cmp	r3, r0
30004ab4:	f0c0 8200 	bcc.w	30004eb8 <BOOT_LoadImages+0x528>
30004ab8:	2a00      	cmp	r2, #0
30004aba:	f000 820d 	beq.w	30004ed8 <BOOT_LoadImages+0x548>
30004abe:	455b      	cmp	r3, fp
30004ac0:	d301      	bcc.n	30004ac6 <BOOT_LoadImages+0x136>
30004ac2:	45e3      	cmp	fp, ip
30004ac4:	d845      	bhi.n	30004b52 <BOOT_LoadImages+0x1c2>
30004ac6:	4813      	ldr	r0, [pc, #76]	; (30004b14 <BOOT_LoadImages+0x184>)
30004ac8:	f007 fdea 	bl	3000c6a0 <__DiagPrintf_veneer>
30004acc:	9f06      	ldr	r7, [sp, #24]
30004ace:	4812      	ldr	r0, [pc, #72]	; (30004b18 <BOOT_LoadImages+0x188>)
30004ad0:	464d      	mov	r5, r9
30004ad2:	f007 fde5 	bl	3000c6a0 <__DiagPrintf_veneer>
30004ad6:	9b07      	ldr	r3, [sp, #28]
30004ad8:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30004adc:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004ae0:	e049      	b.n	30004b76 <BOOT_LoadImages+0x1e6>
30004ae2:	480e      	ldr	r0, [pc, #56]	; (30004b1c <BOOT_LoadImages+0x18c>)
30004ae4:	9307      	str	r3, [sp, #28]
30004ae6:	f007 fddb 	bl	3000c6a0 <__DiagPrintf_veneer>
30004aea:	9b07      	ldr	r3, [sp, #28]
30004aec:	3303      	adds	r3, #3
30004aee:	d99a      	bls.n	30004a26 <BOOT_LoadImages+0x96>
30004af0:	480b      	ldr	r0, [pc, #44]	; (30004b20 <BOOT_LoadImages+0x190>)
30004af2:	e149      	b.n	30004d88 <BOOT_LoadImages+0x3f8>
30004af4:	2001c01c 	.word	0x2001c01c
30004af8:	00009db5 	.word	0x00009db5
30004afc:	07ffe000 	.word	0x07ffe000
30004b00:	07fff000 	.word	0x07fff000
30004b04:	3000eac0 	.word	0x3000eac0
30004b08:	3000c9a0 	.word	0x3000c9a0
30004b0c:	3000cd88 	.word	0x3000cd88
30004b10:	3000caa4 	.word	0x3000caa4
30004b14:	3000cd6c 	.word	0x3000cd6c
30004b18:	3000cb30 	.word	0x3000cb30
30004b1c:	3000c98c 	.word	0x3000c98c
30004b20:	3000cd38 	.word	0x3000cd38
30004b24:	07fff004 	.word	0x07fff004
30004b28:	48c4      	ldr	r0, [pc, #784]	; (30004e3c <BOOT_LoadImages+0x4ac>)
30004b2a:	930f      	str	r3, [sp, #60]	; 0x3c
30004b2c:	9209      	str	r2, [sp, #36]	; 0x24
30004b2e:	f007 fdb7 	bl	3000c6a0 <__DiagPrintf_veneer>
30004b32:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004b34:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004b36:	2a00      	cmp	r2, #0
30004b38:	f000 8131 	beq.w	30004d9e <BOOT_LoadImages+0x40e>
30004b3c:	455b      	cmp	r3, fp
30004b3e:	f0c0 82c5 	bcc.w	300050cc <BOOT_LoadImages+0x73c>
30004b42:	f1bb 6f00 	cmp.w	fp, #134217728	; 0x8000000
30004b46:	bf2c      	ite	cs
30004b48:	2200      	movcs	r2, #0
30004b4a:	2201      	movcc	r2, #1
30004b4c:	2a00      	cmp	r2, #0
30004b4e:	f040 82bd 	bne.w	300050cc <BOOT_LoadImages+0x73c>
30004b52:	4598      	cmp	r8, r3
30004b54:	f200 81b8 	bhi.w	30004ec8 <BOOT_LoadImages+0x538>
30004b58:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30004b5c:	f0c0 81b4 	bcc.w	30004ec8 <BOOT_LoadImages+0x538>
30004b60:	7b2f      	ldrb	r7, [r5, #12]
30004b62:	2a00      	cmp	r2, #0
30004b64:	f040 81de 	bne.w	30004f24 <BOOT_LoadImages+0x594>
30004b68:	48b5      	ldr	r0, [pc, #724]	; (30004e40 <BOOT_LoadImages+0x4b0>)
30004b6a:	f007 fd99 	bl	3000c6a0 <__DiagPrintf_veneer>
30004b6e:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30004b72:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30004b76:	4639      	mov	r1, r7
30004b78:	48b2      	ldr	r0, [pc, #712]	; (30004e44 <BOOT_LoadImages+0x4b4>)
30004b7a:	f007 fd91 	bl	3000c6a0 <__DiagPrintf_veneer>
30004b7e:	4bb2      	ldr	r3, [pc, #712]	; (30004e48 <BOOT_LoadImages+0x4b8>)
30004b80:	4798      	blx	r3
30004b82:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004b86:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30004b8a:	f8d1 300a 	ldr.w	r3, [r1, #10]
30004b8e:	6809      	ldr	r1, [r1, #0]
30004b90:	330c      	adds	r3, #12
30004b92:	910b      	str	r1, [sp, #44]	; 0x2c
30004b94:	42b3      	cmp	r3, r6
30004b96:	bf28      	it	cs
30004b98:	4633      	movcs	r3, r6
30004b9a:	930d      	str	r3, [sp, #52]	; 0x34
30004b9c:	f8d2 300a 	ldr.w	r3, [r2, #10]
30004ba0:	6812      	ldr	r2, [r2, #0]
30004ba2:	330c      	adds	r3, #12
30004ba4:	920a      	str	r2, [sp, #40]	; 0x28
30004ba6:	429e      	cmp	r6, r3
30004ba8:	bf28      	it	cs
30004baa:	461e      	movcs	r6, r3
30004bac:	960c      	str	r6, [sp, #48]	; 0x30
30004bae:	2800      	cmp	r0, #0
30004bb0:	f040 8194 	bne.w	30004edc <BOOT_LoadImages+0x54c>
30004bb4:	4ba5      	ldr	r3, [pc, #660]	; (30004e4c <BOOT_LoadImages+0x4bc>)
30004bb6:	4606      	mov	r6, r0
30004bb8:	f8df 92bc 	ldr.w	r9, [pc, #700]	; 30004e78 <BOOT_LoadImages+0x4e8>
30004bbc:	4680      	mov	r8, r0
30004bbe:	f8df b2bc 	ldr.w	fp, [pc, #700]	; 30004e7c <BOOT_LoadImages+0x4ec>
30004bc2:	4682      	mov	sl, r0
30004bc4:	e9cd 0303 	strd	r0, r3, [sp, #12]
30004bc8:	2f00      	cmp	r7, #0
30004bca:	f040 81c9 	bne.w	30004f60 <BOOT_LoadImages+0x5d0>
30004bce:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30004bd2:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30004bd4:	f10c 0104 	add.w	r1, ip, #4
30004bd8:	2b00      	cmp	r3, #0
30004bda:	f000 83d4 	beq.w	30005386 <BOOT_LoadImages+0x9f6>
30004bde:	1d18      	adds	r0, r3, #4
30004be0:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004be4:	4460      	add	r0, ip
30004be6:	f811 3b01 	ldrb.w	r3, [r1], #1
30004bea:	4053      	eors	r3, r2
30004bec:	4281      	cmp	r1, r0
30004bee:	b2db      	uxtb	r3, r3
30004bf0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004bf4:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004bf8:	d1f5      	bne.n	30004be6 <BOOT_LoadImages+0x256>
30004bfa:	43d2      	mvns	r2, r2
30004bfc:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30004bfe:	4293      	cmp	r3, r2
30004c00:	f000 838d 	beq.w	3000531e <BOOT_LoadImages+0x98e>
30004c04:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004c08:	f088 0301 	eor.w	r3, r8, #1
30004c0c:	3101      	adds	r1, #1
30004c0e:	f003 0301 	and.w	r3, r3, #1
30004c12:	ea46 0603 	orr.w	r6, r6, r3
30004c16:	f000 8345 	beq.w	300052a4 <BOOT_LoadImages+0x914>
30004c1a:	990b      	ldr	r1, [sp, #44]	; 0x2c
30004c1c:	488c      	ldr	r0, [pc, #560]	; (30004e50 <BOOT_LoadImages+0x4c0>)
30004c1e:	f007 fd3f 	bl	3000c6a0 <__DiagPrintf_veneer>
30004c22:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004c26:	2002      	movs	r0, #2
30004c28:	f7ff f858 	bl	30003cdc <FLASH_Erase_With_Lock>
30004c2c:	2e00      	cmp	r6, #0
30004c2e:	f040 831f 	bne.w	30005270 <BOOT_LoadImages+0x8e0>
30004c32:	9605      	str	r6, [sp, #20]
30004c34:	f08a 0601 	eor.w	r6, sl, #1
30004c38:	f04f 0801 	mov.w	r8, #1
30004c3c:	f006 0601 	and.w	r6, r6, #1
30004c40:	9b03      	ldr	r3, [sp, #12]
30004c42:	2b01      	cmp	r3, #1
30004c44:	f000 8238 	beq.w	300050b8 <BOOT_LoadImages+0x728>
30004c48:	9b03      	ldr	r3, [sp, #12]
30004c4a:	2b02      	cmp	r3, #2
30004c4c:	f000 83a9 	beq.w	300053a2 <BOOT_LoadImages+0xa12>
30004c50:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30004c52:	4650      	mov	r0, sl
30004c54:	f8c9 3000 	str.w	r3, [r9]
30004c58:	47d8      	blx	fp
30004c5a:	1e03      	subs	r3, r0, #0
30004c5c:	bf18      	it	ne
30004c5e:	2301      	movne	r3, #1
30004c60:	2800      	cmp	r0, #0
30004c62:	f000 8156 	beq.w	30004f12 <BOOT_LoadImages+0x582>
30004c66:	2e00      	cmp	r6, #0
30004c68:	f000 8153 	beq.w	30004f12 <BOOT_LoadImages+0x582>
30004c6c:	fab7 f787 	clz	r7, r7
30004c70:	9b05      	ldr	r3, [sp, #20]
30004c72:	097f      	lsrs	r7, r7, #5
30004c74:	2b00      	cmp	r3, #0
30004c76:	f000 83e1 	beq.w	3000543c <BOOT_LoadImages+0xaac>
30004c7a:	469a      	mov	sl, r3
30004c7c:	4c75      	ldr	r4, [pc, #468]	; (30004e54 <BOOT_LoadImages+0x4c4>)
30004c7e:	2f00      	cmp	r7, #0
30004c80:	f040 8138 	bne.w	30004ef4 <BOOT_LoadImages+0x564>
30004c84:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004c88:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004c8c:	3201      	adds	r2, #1
30004c8e:	f000 82c5 	beq.w	3000521c <BOOT_LoadImages+0x88c>
30004c92:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004c96:	4870      	ldr	r0, [pc, #448]	; (30004e58 <BOOT_LoadImages+0x4c8>)
30004c98:	f007 fd02 	bl	3000c6a0 <__DiagPrintf_veneer>
30004c9c:	2301      	movs	r3, #1
30004c9e:	9303      	str	r3, [sp, #12]
30004ca0:	f04f 0801 	mov.w	r8, #1
30004ca4:	4646      	mov	r6, r8
30004ca6:	f8cd 8014 	str.w	r8, [sp, #20]
30004caa:	f1ba 0f00 	cmp.w	sl, #0
30004cae:	d0c7      	beq.n	30004c40 <BOOT_LoadImages+0x2b0>
30004cb0:	ab16      	add	r3, sp, #88	; 0x58
30004cb2:	2280      	movs	r2, #128	; 0x80
30004cb4:	21ff      	movs	r1, #255	; 0xff
30004cb6:	46d0      	mov	r8, sl
30004cb8:	461e      	mov	r6, r3
30004cba:	4618      	mov	r0, r3
30004cbc:	ee08 3a10 	vmov	s16, r3
30004cc0:	f007 fc92 	bl	3000c5e8 <____wrap_memset_veneer>
30004cc4:	2280      	movs	r2, #128	; 0x80
30004cc6:	4629      	mov	r1, r5
30004cc8:	4630      	mov	r0, r6
30004cca:	f007 fc9d 	bl	3000c608 <____wrap_memcpy_veneer>
30004cce:	2f01      	cmp	r7, #1
30004cd0:	f000 831e 	beq.w	30005310 <BOOT_LoadImages+0x980>
30004cd4:	2100      	movs	r1, #0
30004cd6:	f8cd a014 	str.w	sl, [sp, #20]
30004cda:	f04f 0a01 	mov.w	sl, #1
30004cde:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30004ce2:	1d2e      	adds	r6, r5, #4
30004ce4:	485d      	ldr	r0, [pc, #372]	; (30004e5c <BOOT_LoadImages+0x4cc>)
30004ce6:	f007 fcdb 	bl	3000c6a0 <__DiagPrintf_veneer>
30004cea:	4629      	mov	r1, r5
30004cec:	2002      	movs	r0, #2
30004cee:	f7fe fff5 	bl	30003cdc <FLASH_Erase_With_Lock>
30004cf2:	aa17      	add	r2, sp, #92	; 0x5c
30004cf4:	217c      	movs	r1, #124	; 0x7c
30004cf6:	4630      	mov	r0, r6
30004cf8:	f7ff f818 	bl	30003d2c <FLASH_TxData_With_Lock>
30004cfc:	4630      	mov	r0, r6
30004cfe:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30004d02:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004d06:	f810 3b01 	ldrb.w	r3, [r0], #1
30004d0a:	4053      	eors	r3, r2
30004d0c:	4288      	cmp	r0, r1
30004d0e:	b2db      	uxtb	r3, r3
30004d10:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004d14:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004d18:	d1f5      	bne.n	30004d06 <BOOT_LoadImages+0x376>
30004d1a:	43d2      	mvns	r2, r2
30004d1c:	4629      	mov	r1, r5
30004d1e:	2002      	movs	r0, #2
30004d20:	9216      	str	r2, [sp, #88]	; 0x58
30004d22:	f7fe ffdb 	bl	30003cdc <FLASH_Erase_With_Lock>
30004d26:	ee18 2a10 	vmov	r2, s16
30004d2a:	2180      	movs	r1, #128	; 0x80
30004d2c:	4628      	mov	r0, r5
30004d2e:	f7fe fffd 	bl	30003d2c <FLASH_TxData_With_Lock>
30004d32:	9b05      	ldr	r3, [sp, #20]
30004d34:	2b00      	cmp	r3, #0
30004d36:	f000 8143 	beq.w	30004fc0 <BOOT_LoadImages+0x630>
30004d3a:	f1b8 0f00 	cmp.w	r8, #0
30004d3e:	f040 8376 	bne.w	3000542e <BOOT_LoadImages+0xa9e>
30004d42:	2f01      	cmp	r7, #1
30004d44:	f000 8280 	beq.w	30005248 <BOOT_LoadImages+0x8b8>
30004d48:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004d4c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004d50:	3201      	adds	r2, #1
30004d52:	f000 81d0 	beq.w	300050f6 <BOOT_LoadImages+0x766>
30004d56:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004d5a:	4841      	ldr	r0, [pc, #260]	; (30004e60 <BOOT_LoadImages+0x4d0>)
30004d5c:	f007 fca0 	bl	3000c6a0 <__DiagPrintf_veneer>
30004d60:	f08a 0601 	eor.w	r6, sl, #1
30004d64:	f04f 0801 	mov.w	r8, #1
30004d68:	f006 0601 	and.w	r6, r6, #1
30004d6c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004d70:	3310      	adds	r3, #16
30004d72:	930e      	str	r3, [sp, #56]	; 0x38
30004d74:	2302      	movs	r3, #2
30004d76:	9303      	str	r3, [sp, #12]
30004d78:	e76a      	b.n	30004c50 <BOOT_LoadImages+0x2c0>
30004d7a:	483a      	ldr	r0, [pc, #232]	; (30004e64 <BOOT_LoadImages+0x4d4>)
30004d7c:	f007 fc90 	bl	3000c6a0 <__DiagPrintf_veneer>
30004d80:	9b04      	ldr	r3, [sp, #16]
30004d82:	3b01      	subs	r3, #1
30004d84:	e64d      	b.n	30004a22 <BOOT_LoadImages+0x92>
30004d86:	4838      	ldr	r0, [pc, #224]	; (30004e68 <BOOT_LoadImages+0x4d8>)
30004d88:	f007 fc8a 	bl	3000c6a0 <__DiagPrintf_veneer>
30004d8c:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004d90:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30004d94:	9307      	str	r3, [sp, #28]
30004d96:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30004d9a:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004d9e:	4833      	ldr	r0, [pc, #204]	; (30004e6c <BOOT_LoadImages+0x4dc>)
30004da0:	f007 fc7e 	bl	3000c6a0 <__DiagPrintf_veneer>
30004da4:	4832      	ldr	r0, [pc, #200]	; (30004e70 <BOOT_LoadImages+0x4e0>)
30004da6:	f007 fc7b 	bl	3000c6a0 <__DiagPrintf_veneer>
30004daa:	4631      	mov	r1, r6
30004dac:	aa12      	add	r2, sp, #72	; 0x48
30004dae:	4650      	mov	r0, sl
30004db0:	f7ff f85c 	bl	30003e6c <recovery_check>
30004db4:	4631      	mov	r1, r6
30004db6:	4604      	mov	r4, r0
30004db8:	aa13      	add	r2, sp, #76	; 0x4c
30004dba:	9807      	ldr	r0, [sp, #28]
30004dbc:	f7ff f856 	bl	30003e6c <recovery_check>
30004dc0:	4631      	mov	r1, r6
30004dc2:	aa14      	add	r2, sp, #80	; 0x50
30004dc4:	9004      	str	r0, [sp, #16]
30004dc6:	4658      	mov	r0, fp
30004dc8:	f7ff f850 	bl	30003e6c <recovery_check>
30004dcc:	4631      	mov	r1, r6
30004dce:	aa15      	add	r2, sp, #84	; 0x54
30004dd0:	4606      	mov	r6, r0
30004dd2:	4640      	mov	r0, r8
30004dd4:	f7ff f84a 	bl	30003e6c <recovery_check>
30004dd8:	9b04      	ldr	r3, [sp, #16]
30004dda:	4306      	orrs	r6, r0
30004ddc:	431c      	orrs	r4, r3
30004dde:	d04f      	beq.n	30004e80 <BOOT_LoadImages+0x4f0>
30004de0:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30004de4:	2e00      	cmp	r6, #0
30004de6:	f000 818a 	beq.w	300050fe <BOOT_LoadImages+0x76e>
30004dea:	4293      	cmp	r3, r2
30004dec:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
30004df0:	f0c0 8273 	bcc.w	300052da <BOOT_LoadImages+0x94a>
30004df4:	428b      	cmp	r3, r1
30004df6:	d302      	bcc.n	30004dfe <BOOT_LoadImages+0x46e>
30004df8:	4283      	cmp	r3, r0
30004dfa:	f080 82fb 	bcs.w	300053f4 <BOOT_LoadImages+0xa64>
30004dfe:	4293      	cmp	r3, r2
30004e00:	f000 826b 	beq.w	300052da <BOOT_LoadImages+0x94a>
30004e04:	4293      	cmp	r3, r2
30004e06:	bf38      	it	cc
30004e08:	4613      	movcc	r3, r2
30004e0a:	428b      	cmp	r3, r1
30004e0c:	f240 82dc 	bls.w	300053c8 <BOOT_LoadImages+0xa38>
30004e10:	428b      	cmp	r3, r1
30004e12:	bf38      	it	cc
30004e14:	460b      	movcc	r3, r1
30004e16:	4283      	cmp	r3, r0
30004e18:	f240 82e5 	bls.w	300053e6 <BOOT_LoadImages+0xa56>
30004e1c:	f04f 0800 	mov.w	r8, #0
30004e20:	4b0a      	ldr	r3, [pc, #40]	; (30004e4c <BOOT_LoadImages+0x4bc>)
30004e22:	2601      	movs	r6, #1
30004e24:	f8df 9050 	ldr.w	r9, [pc, #80]	; 30004e78 <BOOT_LoadImages+0x4e8>
30004e28:	f8df b050 	ldr.w	fp, [pc, #80]	; 30004e7c <BOOT_LoadImages+0x4ec>
30004e2c:	46c2      	mov	sl, r8
30004e2e:	9304      	str	r3, [sp, #16]
30004e30:	e70e      	b.n	30004c50 <BOOT_LoadImages+0x2c0>
30004e32:	4810      	ldr	r0, [pc, #64]	; (30004e74 <BOOT_LoadImages+0x4e4>)
30004e34:	f007 fc34 	bl	3000c6a0 <__DiagPrintf_veneer>
30004e38:	e5c5      	b.n	300049c6 <BOOT_LoadImages+0x36>
30004e3a:	bf00      	nop
30004e3c:	3000cae4 	.word	0x3000cae4
30004e40:	3000cb40 	.word	0x3000cb40
30004e44:	3000cb74 	.word	0x3000cb74
30004e48:	0000c149 	.word	0x0000c149
30004e4c:	3000e8fb 	.word	0x3000e8fb
30004e50:	3000cbe4 	.word	0x3000cbe4
30004e54:	3000eac0 	.word	0x3000eac0
30004e58:	3000c8d8 	.word	0x3000c8d8
30004e5c:	3000ccb8 	.word	0x3000ccb8
30004e60:	3000c8f0 	.word	0x3000c8f0
30004e64:	3000c974 	.word	0x3000c974
30004e68:	3000c9d8 	.word	0x3000c9d8
30004e6c:	3000cb50 	.word	0x3000cb50
30004e70:	3000cb64 	.word	0x3000cb64
30004e74:	3000c950 	.word	0x3000c950
30004e78:	3000ff18 	.word	0x3000ff18
30004e7c:	30006d39 	.word	0x30006d39
30004e80:	2e00      	cmp	r6, #0
30004e82:	f000 827c 	beq.w	3000537e <BOOT_LoadImages+0x9ee>
30004e86:	4659      	mov	r1, fp
30004e88:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
30004e8c:	e9cd 5400 	strd	r5, r4, [sp]
30004e90:	429a      	cmp	r2, r3
30004e92:	464b      	mov	r3, r9
30004e94:	4642      	mov	r2, r8
30004e96:	f080 821e 	bcs.w	300052d6 <BOOT_LoadImages+0x946>
30004e9a:	2001      	movs	r0, #1
30004e9c:	f7ff f816 	bl	30003ecc <rewrite_bp>
30004ea0:	46a0      	mov	r8, r4
30004ea2:	b2c3      	uxtb	r3, r0
30004ea4:	2601      	movs	r6, #1
30004ea6:	46a2      	mov	sl, r4
30004ea8:	f8df 9318 	ldr.w	r9, [pc, #792]	; 300051c4 <BOOT_LoadImages+0x834>
30004eac:	9303      	str	r3, [sp, #12]
30004eae:	4bb0      	ldr	r3, [pc, #704]	; (30005170 <BOOT_LoadImages+0x7e0>)
30004eb0:	f8df b314 	ldr.w	fp, [pc, #788]	; 300051c8 <BOOT_LoadImages+0x838>
30004eb4:	9304      	str	r3, [sp, #16]
30004eb6:	e6c3      	b.n	30004c40 <BOOT_LoadImages+0x2b0>
30004eb8:	48ae      	ldr	r0, [pc, #696]	; (30005174 <BOOT_LoadImages+0x7e4>)
30004eba:	930f      	str	r3, [sp, #60]	; 0x3c
30004ebc:	9209      	str	r2, [sp, #36]	; 0x24
30004ebe:	f007 fbef 	bl	3000c6a0 <__DiagPrintf_veneer>
30004ec2:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004ec4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004ec6:	e636      	b.n	30004b36 <BOOT_LoadImages+0x1a6>
30004ec8:	48ab      	ldr	r0, [pc, #684]	; (30005178 <BOOT_LoadImages+0x7e8>)
30004eca:	9204      	str	r2, [sp, #16]
30004ecc:	f007 fbe8 	bl	3000c6a0 <__DiagPrintf_veneer>
30004ed0:	9a04      	ldr	r2, [sp, #16]
30004ed2:	2a00      	cmp	r2, #0
30004ed4:	f43f af63 	beq.w	30004d9e <BOOT_LoadImages+0x40e>
30004ed8:	9f06      	ldr	r7, [sp, #24]
30004eda:	e5f8      	b.n	30004ace <BOOT_LoadImages+0x13e>
30004edc:	2300      	movs	r3, #0
30004ede:	4aa4      	ldr	r2, [pc, #656]	; (30005170 <BOOT_LoadImages+0x7e0>)
30004ee0:	f8df 92e0 	ldr.w	r9, [pc, #736]	; 300051c4 <BOOT_LoadImages+0x834>
30004ee4:	f8df b2e0 	ldr.w	fp, [pc, #736]	; 300051c8 <BOOT_LoadImages+0x838>
30004ee8:	469a      	mov	sl, r3
30004eea:	9303      	str	r3, [sp, #12]
30004eec:	9204      	str	r2, [sp, #16]
30004eee:	2f00      	cmp	r7, #0
30004ef0:	f43f aec8 	beq.w	30004c84 <BOOT_LoadImages+0x2f4>
30004ef4:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004ef8:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004efc:	3201      	adds	r2, #1
30004efe:	f000 8118 	beq.w	30005132 <BOOT_LoadImages+0x7a2>
30004f02:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004f06:	489d      	ldr	r0, [pc, #628]	; (3000517c <BOOT_LoadImages+0x7ec>)
30004f08:	f007 fbca 	bl	3000c6a0 <__DiagPrintf_veneer>
30004f0c:	2302      	movs	r3, #2
30004f0e:	9303      	str	r3, [sp, #12]
30004f10:	e6c6      	b.n	30004ca0 <BOOT_LoadImages+0x310>
30004f12:	f00a 0401 	and.w	r4, sl, #1
30004f16:	b1c3      	cbz	r3, 30004f4a <BOOT_LoadImages+0x5ba>
30004f18:	b1bc      	cbz	r4, 30004f4a <BOOT_LoadImages+0x5ba>
30004f1a:	4c99      	ldr	r4, [pc, #612]	; (30005180 <BOOT_LoadImages+0x7f0>)
30004f1c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004f20:	47a0      	blx	r4
30004f22:	e7fb      	b.n	30004f1c <BOOT_LoadImages+0x58c>
30004f24:	4897      	ldr	r0, [pc, #604]	; (30005184 <BOOT_LoadImages+0x7f4>)
30004f26:	f007 fbbb 	bl	3000c6a0 <__DiagPrintf_veneer>
30004f2a:	9b08      	ldr	r3, [sp, #32]
30004f2c:	9a04      	ldr	r2, [sp, #16]
30004f2e:	4293      	cmp	r3, r2
30004f30:	f4ff ae1a 	bcc.w	30004b68 <BOOT_LoadImages+0x1d8>
30004f34:	4894      	ldr	r0, [pc, #592]	; (30005188 <BOOT_LoadImages+0x7f8>)
30004f36:	464d      	mov	r5, r9
30004f38:	f007 fbb2 	bl	3000c6a0 <__DiagPrintf_veneer>
30004f3c:	9b07      	ldr	r3, [sp, #28]
30004f3e:	9f06      	ldr	r7, [sp, #24]
30004f40:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30004f44:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004f48:	e615      	b.n	30004b76 <BOOT_LoadImages+0x1e6>
30004f4a:	9b04      	ldr	r3, [sp, #16]
30004f4c:	781b      	ldrb	r3, [r3, #0]
30004f4e:	2b00      	cmp	r3, #0
30004f50:	f040 80c0 	bne.w	300050d4 <BOOT_LoadImages+0x744>
30004f54:	2001      	movs	r0, #1
30004f56:	b037      	add	sp, #220	; 0xdc
30004f58:	ecbd 8b02 	vpop	{d8}
30004f5c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f60:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30004f64:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004f66:	f10c 0104 	add.w	r1, ip, #4
30004f6a:	2b00      	cmp	r3, #0
30004f6c:	f000 820d 	beq.w	3000538a <BOOT_LoadImages+0x9fa>
30004f70:	1d18      	adds	r0, r3, #4
30004f72:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004f76:	4460      	add	r0, ip
30004f78:	f811 3b01 	ldrb.w	r3, [r1], #1
30004f7c:	4053      	eors	r3, r2
30004f7e:	4281      	cmp	r1, r0
30004f80:	b2db      	uxtb	r3, r3
30004f82:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004f86:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004f8a:	d1f5      	bne.n	30004f78 <BOOT_LoadImages+0x5e8>
30004f8c:	43d2      	mvns	r2, r2
30004f8e:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004f90:	4293      	cmp	r3, r2
30004f92:	f000 81d4 	beq.w	3000533e <BOOT_LoadImages+0x9ae>
30004f96:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004f9a:	f088 0301 	eor.w	r3, r8, #1
30004f9e:	3101      	adds	r1, #1
30004fa0:	f003 0301 	and.w	r3, r3, #1
30004fa4:	ea46 0603 	orr.w	r6, r6, r3
30004fa8:	f000 8178 	beq.w	3000529c <BOOT_LoadImages+0x90c>
30004fac:	990a      	ldr	r1, [sp, #40]	; 0x28
30004fae:	4877      	ldr	r0, [pc, #476]	; (3000518c <BOOT_LoadImages+0x7fc>)
30004fb0:	f007 fb76 	bl	3000c6a0 <__DiagPrintf_veneer>
30004fb4:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30004fb8:	2002      	movs	r0, #2
30004fba:	f7fe fe8f 	bl	30003cdc <FLASH_Erase_With_Lock>
30004fbe:	e635      	b.n	30004c2c <BOOT_LoadImages+0x29c>
30004fc0:	f1b8 0f00 	cmp.w	r8, #0
30004fc4:	f040 822e 	bne.w	30005424 <BOOT_LoadImages+0xa94>
30004fc8:	2f01      	cmp	r7, #1
30004fca:	f000 80ff 	beq.w	300051cc <BOOT_LoadImages+0x83c>
30004fce:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
30004fd2:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004fd4:	1d31      	adds	r1, r6, #4
30004fd6:	2b00      	cmp	r3, #0
30004fd8:	f000 81e5 	beq.w	300053a6 <BOOT_LoadImages+0xa16>
30004fdc:	1d18      	adds	r0, r3, #4
30004fde:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004fe2:	4430      	add	r0, r6
30004fe4:	f811 3b01 	ldrb.w	r3, [r1], #1
30004fe8:	4053      	eors	r3, r2
30004fea:	4281      	cmp	r1, r0
30004fec:	b2db      	uxtb	r3, r3
30004fee:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004ff2:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004ff6:	d1f5      	bne.n	30004fe4 <BOOT_LoadImages+0x654>
30004ff8:	43d2      	mvns	r2, r2
30004ffa:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004ffc:	4293      	cmp	r3, r2
30004ffe:	f000 81af 	beq.w	30005360 <BOOT_LoadImages+0x9d0>
30005002:	f8d6 300a 	ldr.w	r3, [r6, #10]
30005006:	3301      	adds	r3, #1
30005008:	d075      	beq.n	300050f6 <BOOT_LoadImages+0x766>
3000500a:	990a      	ldr	r1, [sp, #40]	; 0x28
3000500c:	485f      	ldr	r0, [pc, #380]	; (3000518c <BOOT_LoadImages+0x7fc>)
3000500e:	f007 fb47 	bl	3000c6a0 <__DiagPrintf_veneer>
30005012:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30005016:	2002      	movs	r0, #2
30005018:	f7fe fe60 	bl	30003cdc <FLASH_Erase_With_Lock>
3000501c:	485c      	ldr	r0, [pc, #368]	; (30005190 <BOOT_LoadImages+0x800>)
3000501e:	f007 fb3f 	bl	3000c6a0 <__DiagPrintf_veneer>
30005022:	e7fe      	b.n	30005022 <BOOT_LoadImages+0x692>
30005024:	9908      	ldr	r1, [sp, #32]
30005026:	485b      	ldr	r0, [pc, #364]	; (30005194 <BOOT_LoadImages+0x804>)
30005028:	f007 fb3a 	bl	3000c6a0 <__DiagPrintf_veneer>
3000502c:	9b07      	ldr	r3, [sp, #28]
3000502e:	3303      	adds	r3, #3
30005030:	f240 81ad 	bls.w	3000538e <BOOT_LoadImages+0x9fe>
30005034:	4858      	ldr	r0, [pc, #352]	; (30005198 <BOOT_LoadImages+0x808>)
30005036:	f007 fb33 	bl	3000c6a0 <__DiagPrintf_veneer>
3000503a:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
3000503e:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005042:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005046:	9307      	str	r3, [sp, #28]
30005048:	f8d5 b00d 	ldr.w	fp, [r5, #13]
3000504c:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005050:	4852      	ldr	r0, [pc, #328]	; (3000519c <BOOT_LoadImages+0x80c>)
30005052:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30005056:	5931      	ldr	r1, [r6, r4]
30005058:	4430      	add	r0, r6
3000505a:	4c51      	ldr	r4, [pc, #324]	; (300051a0 <BOOT_LoadImages+0x810>)
3000505c:	f810 3b01 	ldrb.w	r3, [r0], #1
30005060:	4053      	eors	r3, r2
30005062:	4285      	cmp	r5, r0
30005064:	b2db      	uxtb	r3, r3
30005066:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000506a:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000506e:	d1f5      	bne.n	3000505c <BOOT_LoadImages+0x6cc>
30005070:	43d2      	mvns	r2, r2
30005072:	4291      	cmp	r1, r2
30005074:	d010      	beq.n	30005098 <BOOT_LoadImages+0x708>
30005076:	484b      	ldr	r0, [pc, #300]	; (300051a4 <BOOT_LoadImages+0x814>)
30005078:	f007 fb12 	bl	3000c6a0 <__DiagPrintf_veneer>
3000507c:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000507e:	2b00      	cmp	r3, #0
30005080:	f43f ae8d 	beq.w	30004d9e <BOOT_LoadImages+0x40e>
30005084:	2300      	movs	r3, #0
30005086:	9309      	str	r3, [sp, #36]	; 0x24
30005088:	e4e7      	b.n	30004a5a <BOOT_LoadImages+0xca>
3000508a:	460a      	mov	r2, r1
3000508c:	4846      	ldr	r0, [pc, #280]	; (300051a8 <BOOT_LoadImages+0x818>)
3000508e:	f007 fb07 	bl	3000c6a0 <__DiagPrintf_veneer>
30005092:	2201      	movs	r2, #1
30005094:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005096:	e4fc      	b.n	30004a92 <BOOT_LoadImages+0x102>
30005098:	460a      	mov	r2, r1
3000509a:	4844      	ldr	r0, [pc, #272]	; (300051ac <BOOT_LoadImages+0x81c>)
3000509c:	f007 fb00 	bl	3000c6a0 <__DiagPrintf_veneer>
300050a0:	9b09      	ldr	r3, [sp, #36]	; 0x24
300050a2:	2b00      	cmp	r3, #0
300050a4:	f47f acd9 	bne.w	30004a5a <BOOT_LoadImages+0xca>
300050a8:	461a      	mov	r2, r3
300050aa:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
300050ae:	e4f3      	b.n	30004a98 <BOOT_LoadImages+0x108>
300050b0:	483f      	ldr	r0, [pc, #252]	; (300051b0 <BOOT_LoadImages+0x820>)
300050b2:	f007 faf5 	bl	3000c6a0 <__DiagPrintf_veneer>
300050b6:	e486      	b.n	300049c6 <BOOT_LoadImages+0x36>
300050b8:	4c39      	ldr	r4, [pc, #228]	; (300051a0 <BOOT_LoadImages+0x810>)
300050ba:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300050be:	3310      	adds	r3, #16
300050c0:	930e      	str	r3, [sp, #56]	; 0x38
300050c2:	2301      	movs	r3, #1
300050c4:	9303      	str	r3, [sp, #12]
300050c6:	e5c3      	b.n	30004c50 <BOOT_LoadImages+0x2c0>
300050c8:	483a      	ldr	r0, [pc, #232]	; (300051b4 <BOOT_LoadImages+0x824>)
300050ca:	e65d      	b.n	30004d88 <BOOT_LoadImages+0x3f8>
300050cc:	483a      	ldr	r0, [pc, #232]	; (300051b8 <BOOT_LoadImages+0x828>)
300050ce:	f007 fae7 	bl	3000c6a0 <__DiagPrintf_veneer>
300050d2:	e664      	b.n	30004d9e <BOOT_LoadImages+0x40e>
300050d4:	4b39      	ldr	r3, [pc, #228]	; (300051bc <BOOT_LoadImages+0x82c>)
300050d6:	4650      	mov	r0, sl
300050d8:	4798      	blx	r3
300050da:	1e03      	subs	r3, r0, #0
300050dc:	bf18      	it	ne
300050de:	2301      	movne	r3, #1
300050e0:	b110      	cbz	r0, 300050e8 <BOOT_LoadImages+0x758>
300050e2:	2e00      	cmp	r6, #0
300050e4:	f47f adc2 	bne.w	30004c6c <BOOT_LoadImages+0x2dc>
300050e8:	2c00      	cmp	r4, #0
300050ea:	f43f af33 	beq.w	30004f54 <BOOT_LoadImages+0x5c4>
300050ee:	2b00      	cmp	r3, #0
300050f0:	f47f af13 	bne.w	30004f1a <BOOT_LoadImages+0x58a>
300050f4:	e72e      	b.n	30004f54 <BOOT_LoadImages+0x5c4>
300050f6:	4832      	ldr	r0, [pc, #200]	; (300051c0 <BOOT_LoadImages+0x830>)
300050f8:	f007 fad2 	bl	3000c6a0 <__DiagPrintf_veneer>
300050fc:	e78e      	b.n	3000501c <BOOT_LoadImages+0x68c>
300050fe:	4293      	cmp	r3, r2
30005100:	f04f 0401 	mov.w	r4, #1
30005104:	464b      	mov	r3, r9
30005106:	4651      	mov	r1, sl
30005108:	bf28      	it	cs
3000510a:	4630      	movcs	r0, r6
3000510c:	9a07      	ldr	r2, [sp, #28]
3000510e:	bf38      	it	cc
30005110:	4620      	movcc	r0, r4
30005112:	9500      	str	r5, [sp, #0]
30005114:	9401      	str	r4, [sp, #4]
30005116:	46b0      	mov	r8, r6
30005118:	f7fe fed8 	bl	30003ecc <rewrite_bp>
3000511c:	4626      	mov	r6, r4
3000511e:	b2c3      	uxtb	r3, r0
30005120:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 300051c4 <BOOT_LoadImages+0x834>
30005124:	46c2      	mov	sl, r8
30005126:	f8df b0a0 	ldr.w	fp, [pc, #160]	; 300051c8 <BOOT_LoadImages+0x838>
3000512a:	9303      	str	r3, [sp, #12]
3000512c:	4b10      	ldr	r3, [pc, #64]	; (30005170 <BOOT_LoadImages+0x7e0>)
3000512e:	9304      	str	r3, [sp, #16]
30005130:	e586      	b.n	30004c40 <BOOT_LoadImages+0x2b0>
30005132:	ab16      	add	r3, sp, #88	; 0x58
30005134:	4822      	ldr	r0, [pc, #136]	; (300051c0 <BOOT_LoadImages+0x830>)
30005136:	461e      	mov	r6, r3
30005138:	ee08 3a10 	vmov	s16, r3
3000513c:	f007 fab0 	bl	3000c6a0 <__DiagPrintf_veneer>
30005140:	2280      	movs	r2, #128	; 0x80
30005142:	21ff      	movs	r1, #255	; 0xff
30005144:	4630      	mov	r0, r6
30005146:	f007 fa4f 	bl	3000c5e8 <____wrap_memset_veneer>
3000514a:	2280      	movs	r2, #128	; 0x80
3000514c:	4629      	mov	r1, r5
3000514e:	4630      	mov	r0, r6
30005150:	f007 fa5a 	bl	3000c608 <____wrap_memcpy_veneer>
30005154:	f1ba 0f00 	cmp.w	sl, #0
30005158:	f040 80a8 	bne.w	300052ac <BOOT_LoadImages+0x91c>
3000515c:	2301      	movs	r3, #1
3000515e:	9305      	str	r3, [sp, #20]
30005160:	46d0      	mov	r8, sl
30005162:	f04f 0a00 	mov.w	sl, #0
30005166:	4651      	mov	r1, sl
30005168:	f88d a064 	strb.w	sl, [sp, #100]	; 0x64
3000516c:	e5b9      	b.n	30004ce2 <BOOT_LoadImages+0x352>
3000516e:	bf00      	nop
30005170:	3000e8fb 	.word	0x3000e8fb
30005174:	3000cb00 	.word	0x3000cb00
30005178:	3000cd1c 	.word	0x3000cd1c
3000517c:	3000c8f0 	.word	0x3000c8f0
30005180:	00009be5 	.word	0x00009be5
30005184:	3000cb1c 	.word	0x3000cb1c
30005188:	3000cb30 	.word	0x3000cb30
3000518c:	3000cc6c 	.word	0x3000cc6c
30005190:	3000c908 	.word	0x3000c908
30005194:	3000c9bc 	.word	0x3000c9bc
30005198:	3000cd38 	.word	0x3000cd38
3000519c:	07ffe004 	.word	0x07ffe004
300051a0:	3000eac0 	.word	0x3000eac0
300051a4:	3000ca28 	.word	0x3000ca28
300051a8:	3000ca68 	.word	0x3000ca68
300051ac:	3000c9ec 	.word	0x3000c9ec
300051b0:	3000c960 	.word	0x3000c960
300051b4:	3000cd50 	.word	0x3000cd50
300051b8:	3000cd6c 	.word	0x3000cd6c
300051bc:	30006d3d 	.word	0x30006d3d
300051c0:	3000cb98 	.word	0x3000cb98
300051c4:	3000ff18 	.word	0x3000ff18
300051c8:	30006d39 	.word	0x30006d39
300051cc:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
300051d0:	9b0d      	ldr	r3, [sp, #52]	; 0x34
300051d2:	1d31      	adds	r1, r6, #4
300051d4:	2b00      	cmp	r3, #0
300051d6:	f000 8102 	beq.w	300053de <BOOT_LoadImages+0xa4e>
300051da:	1d18      	adds	r0, r3, #4
300051dc:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
300051e0:	4430      	add	r0, r6
300051e2:	f811 3b01 	ldrb.w	r3, [r1], #1
300051e6:	4053      	eors	r3, r2
300051e8:	4281      	cmp	r1, r0
300051ea:	b2db      	uxtb	r3, r3
300051ec:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051f0:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300051f4:	d1f5      	bne.n	300051e2 <BOOT_LoadImages+0x852>
300051f6:	43d2      	mvns	r2, r2
300051f8:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300051fa:	4293      	cmp	r3, r2
300051fc:	f000 80d5 	beq.w	300053aa <BOOT_LoadImages+0xa1a>
30005200:	f8d6 300a 	ldr.w	r3, [r6, #10]
30005204:	3301      	adds	r3, #1
30005206:	d062      	beq.n	300052ce <BOOT_LoadImages+0x93e>
30005208:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000520a:	4890      	ldr	r0, [pc, #576]	; (3000544c <BOOT_LoadImages+0xabc>)
3000520c:	f007 fa48 	bl	3000c6a0 <__DiagPrintf_veneer>
30005210:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005214:	2002      	movs	r0, #2
30005216:	f7fe fd61 	bl	30003cdc <FLASH_Erase_With_Lock>
3000521a:	e6ff      	b.n	3000501c <BOOT_LoadImages+0x68c>
3000521c:	ab16      	add	r3, sp, #88	; 0x58
3000521e:	488c      	ldr	r0, [pc, #560]	; (30005450 <BOOT_LoadImages+0xac0>)
30005220:	461e      	mov	r6, r3
30005222:	ee08 3a10 	vmov	s16, r3
30005226:	f007 fa3b 	bl	3000c6a0 <__DiagPrintf_veneer>
3000522a:	2280      	movs	r2, #128	; 0x80
3000522c:	21ff      	movs	r1, #255	; 0xff
3000522e:	4630      	mov	r0, r6
30005230:	f007 f9da 	bl	3000c5e8 <____wrap_memset_veneer>
30005234:	2280      	movs	r2, #128	; 0x80
30005236:	4629      	mov	r1, r5
30005238:	4630      	mov	r0, r6
3000523a:	f007 f9e5 	bl	3000c608 <____wrap_memcpy_veneer>
3000523e:	f1ba 0f00 	cmp.w	sl, #0
30005242:	d036      	beq.n	300052b2 <BOOT_LoadImages+0x922>
30005244:	46b8      	mov	r8, r7
30005246:	e545      	b.n	30004cd4 <BOOT_LoadImages+0x344>
30005248:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000524c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005250:	3201      	adds	r2, #1
30005252:	d03c      	beq.n	300052ce <BOOT_LoadImages+0x93e>
30005254:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005258:	487e      	ldr	r0, [pc, #504]	; (30005454 <BOOT_LoadImages+0xac4>)
3000525a:	f007 fa21 	bl	3000c6a0 <__DiagPrintf_veneer>
3000525e:	9705      	str	r7, [sp, #20]
30005260:	f08a 0601 	eor.w	r6, sl, #1
30005264:	f04f 0801 	mov.w	r8, #1
30005268:	f006 0601 	and.w	r6, r6, #1
3000526c:	4647      	mov	r7, r8
3000526e:	e724      	b.n	300050ba <BOOT_LoadImages+0x72a>
30005270:	ab16      	add	r3, sp, #88	; 0x58
30005272:	2280      	movs	r2, #128	; 0x80
30005274:	21ff      	movs	r1, #255	; 0xff
30005276:	461e      	mov	r6, r3
30005278:	4618      	mov	r0, r3
3000527a:	ee08 3a10 	vmov	s16, r3
3000527e:	f007 f9b3 	bl	3000c5e8 <____wrap_memset_veneer>
30005282:	2280      	movs	r2, #128	; 0x80
30005284:	4629      	mov	r1, r5
30005286:	4630      	mov	r0, r6
30005288:	f007 f9be 	bl	3000c608 <____wrap_memcpy_veneer>
3000528c:	f1ba 0f00 	cmp.w	sl, #0
30005290:	f040 80d9 	bne.w	30005446 <BOOT_LoadImages+0xab6>
30005294:	b9bf      	cbnz	r7, 300052c6 <BOOT_LoadImages+0x936>
30005296:	46c2      	mov	sl, r8
30005298:	9705      	str	r7, [sp, #20]
3000529a:	e00c      	b.n	300052b6 <BOOT_LoadImages+0x926>
3000529c:	486e      	ldr	r0, [pc, #440]	; (30005458 <BOOT_LoadImages+0xac8>)
3000529e:	f007 f9ff 	bl	3000c6a0 <__DiagPrintf_veneer>
300052a2:	e4c3      	b.n	30004c2c <BOOT_LoadImages+0x29c>
300052a4:	486a      	ldr	r0, [pc, #424]	; (30005450 <BOOT_LoadImages+0xac0>)
300052a6:	f007 f9fb 	bl	3000c6a0 <__DiagPrintf_veneer>
300052aa:	e4bf      	b.n	30004c2c <BOOT_LoadImages+0x29c>
300052ac:	f04f 0800 	mov.w	r8, #0
300052b0:	e50d      	b.n	30004cce <BOOT_LoadImages+0x33e>
300052b2:	2301      	movs	r3, #1
300052b4:	9305      	str	r3, [sp, #20]
300052b6:	46d0      	mov	r8, sl
300052b8:	2101      	movs	r1, #1
300052ba:	f04f 0a00 	mov.w	sl, #0
300052be:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300052c2:	4657      	mov	r7, sl
300052c4:	e50d      	b.n	30004ce2 <BOOT_LoadImages+0x352>
300052c6:	f8cd a014 	str.w	sl, [sp, #20]
300052ca:	46c2      	mov	sl, r8
300052cc:	e748      	b.n	30005160 <BOOT_LoadImages+0x7d0>
300052ce:	4860      	ldr	r0, [pc, #384]	; (30005450 <BOOT_LoadImages+0xac0>)
300052d0:	f007 f9e6 	bl	3000c6a0 <__DiagPrintf_veneer>
300052d4:	e6a2      	b.n	3000501c <BOOT_LoadImages+0x68c>
300052d6:	4620      	mov	r0, r4
300052d8:	e5e0      	b.n	30004e9c <BOOT_LoadImages+0x50c>
300052da:	428a      	cmp	r2, r1
300052dc:	f4ff ad92 	bcc.w	30004e04 <BOOT_LoadImages+0x474>
300052e0:	4282      	cmp	r2, r0
300052e2:	f4ff ad8f 	bcc.w	30004e04 <BOOT_LoadImages+0x474>
300052e6:	2601      	movs	r6, #1
300052e8:	464b      	mov	r3, r9
300052ea:	9a07      	ldr	r2, [sp, #28]
300052ec:	4651      	mov	r1, sl
300052ee:	4630      	mov	r0, r6
300052f0:	9500      	str	r5, [sp, #0]
300052f2:	9601      	str	r6, [sp, #4]
300052f4:	f7fe fdea 	bl	30003ecc <rewrite_bp>
300052f8:	f04f 0800 	mov.w	r8, #0
300052fc:	b2c3      	uxtb	r3, r0
300052fe:	f8df 9180 	ldr.w	r9, [pc, #384]	; 30005480 <BOOT_LoadImages+0xaf0>
30005302:	f8df b180 	ldr.w	fp, [pc, #384]	; 30005484 <BOOT_LoadImages+0xaf4>
30005306:	46c2      	mov	sl, r8
30005308:	9303      	str	r3, [sp, #12]
3000530a:	4b54      	ldr	r3, [pc, #336]	; (3000545c <BOOT_LoadImages+0xacc>)
3000530c:	9304      	str	r3, [sp, #16]
3000530e:	e497      	b.n	30004c40 <BOOT_LoadImages+0x2b0>
30005310:	f8cd a014 	str.w	sl, [sp, #20]
30005314:	4639      	mov	r1, r7
30005316:	46ba      	mov	sl, r7
30005318:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
3000531c:	e4e1      	b.n	30004ce2 <BOOT_LoadImages+0x352>
3000531e:	461a      	mov	r2, r3
30005320:	4619      	mov	r1, r3
30005322:	484f      	ldr	r0, [pc, #316]	; (30005460 <BOOT_LoadImages+0xad0>)
30005324:	f007 f9bc 	bl	3000c6a0 <__DiagPrintf_veneer>
30005328:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000532c:	4849      	ldr	r0, [pc, #292]	; (30005454 <BOOT_LoadImages+0xac4>)
3000532e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005332:	f007 f9b5 	bl	3000c6a0 <__DiagPrintf_veneer>
30005336:	2301      	movs	r3, #1
30005338:	4698      	mov	r8, r3
3000533a:	9303      	str	r3, [sp, #12]
3000533c:	e476      	b.n	30004c2c <BOOT_LoadImages+0x29c>
3000533e:	461a      	mov	r2, r3
30005340:	4619      	mov	r1, r3
30005342:	4848      	ldr	r0, [pc, #288]	; (30005464 <BOOT_LoadImages+0xad4>)
30005344:	f04f 0801 	mov.w	r8, #1
30005348:	f007 f9aa 	bl	3000c6a0 <__DiagPrintf_veneer>
3000534c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005350:	4845      	ldr	r0, [pc, #276]	; (30005468 <BOOT_LoadImages+0xad8>)
30005352:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005356:	f007 f9a3 	bl	3000c6a0 <__DiagPrintf_veneer>
3000535a:	2302      	movs	r3, #2
3000535c:	9303      	str	r3, [sp, #12]
3000535e:	e465      	b.n	30004c2c <BOOT_LoadImages+0x29c>
30005360:	461a      	mov	r2, r3
30005362:	4619      	mov	r1, r3
30005364:	483f      	ldr	r0, [pc, #252]	; (30005464 <BOOT_LoadImages+0xad4>)
30005366:	f007 f99b 	bl	3000c6a0 <__DiagPrintf_veneer>
3000536a:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000536e:	483e      	ldr	r0, [pc, #248]	; (30005468 <BOOT_LoadImages+0xad8>)
30005370:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005374:	f007 f994 	bl	3000c6a0 <__DiagPrintf_veneer>
30005378:	2300      	movs	r3, #0
3000537a:	9305      	str	r3, [sp, #20]
3000537c:	e4f0      	b.n	30004d60 <BOOT_LoadImages+0x3d0>
3000537e:	483b      	ldr	r0, [pc, #236]	; (3000546c <BOOT_LoadImages+0xadc>)
30005380:	f007 f98e 	bl	3000c6a0 <__DiagPrintf_veneer>
30005384:	e7fe      	b.n	30005384 <BOOT_LoadImages+0x9f4>
30005386:	9a0d      	ldr	r2, [sp, #52]	; 0x34
30005388:	e438      	b.n	30004bfc <BOOT_LoadImages+0x26c>
3000538a:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000538c:	e5ff      	b.n	30004f8e <BOOT_LoadImages+0x5fe>
3000538e:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30005392:	f11a 0f03 	cmn.w	sl, #3
30005396:	d824      	bhi.n	300053e2 <BOOT_LoadImages+0xa52>
30005398:	f1bb 0f01 	cmp.w	fp, #1
3000539c:	d932      	bls.n	30005404 <BOOT_LoadImages+0xa74>
3000539e:	4834      	ldr	r0, [pc, #208]	; (30005470 <BOOT_LoadImages+0xae0>)
300053a0:	e649      	b.n	30005036 <BOOT_LoadImages+0x6a6>
300053a2:	4c34      	ldr	r4, [pc, #208]	; (30005474 <BOOT_LoadImages+0xae4>)
300053a4:	e4e2      	b.n	30004d6c <BOOT_LoadImages+0x3dc>
300053a6:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300053a8:	e627      	b.n	30004ffa <BOOT_LoadImages+0x66a>
300053aa:	461a      	mov	r2, r3
300053ac:	4619      	mov	r1, r3
300053ae:	482c      	ldr	r0, [pc, #176]	; (30005460 <BOOT_LoadImages+0xad0>)
300053b0:	f007 f976 	bl	3000c6a0 <__DiagPrintf_veneer>
300053b4:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300053b8:	4826      	ldr	r0, [pc, #152]	; (30005454 <BOOT_LoadImages+0xac4>)
300053ba:	f8d3 1006 	ldr.w	r1, [r3, #6]
300053be:	f007 f96f 	bl	3000c6a0 <__DiagPrintf_veneer>
300053c2:	2300      	movs	r3, #0
300053c4:	9305      	str	r3, [sp, #20]
300053c6:	e74b      	b.n	30005260 <BOOT_LoadImages+0x8d0>
300053c8:	4281      	cmp	r1, r0
300053ca:	f4ff ad21 	bcc.w	30004e10 <BOOT_LoadImages+0x480>
300053ce:	2400      	movs	r4, #0
300053d0:	464b      	mov	r3, r9
300053d2:	4642      	mov	r2, r8
300053d4:	4659      	mov	r1, fp
300053d6:	4620      	mov	r0, r4
300053d8:	9500      	str	r5, [sp, #0]
300053da:	9401      	str	r4, [sp, #4]
300053dc:	e55e      	b.n	30004e9c <BOOT_LoadImages+0x50c>
300053de:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300053e0:	e70a      	b.n	300051f8 <BOOT_LoadImages+0x868>
300053e2:	4825      	ldr	r0, [pc, #148]	; (30005478 <BOOT_LoadImages+0xae8>)
300053e4:	e627      	b.n	30005036 <BOOT_LoadImages+0x6a6>
300053e6:	2400      	movs	r4, #0
300053e8:	464b      	mov	r3, r9
300053ea:	4642      	mov	r2, r8
300053ec:	4659      	mov	r1, fp
300053ee:	9500      	str	r5, [sp, #0]
300053f0:	9401      	str	r4, [sp, #4]
300053f2:	e552      	b.n	30004e9a <BOOT_LoadImages+0x50a>
300053f4:	2601      	movs	r6, #1
300053f6:	464b      	mov	r3, r9
300053f8:	9a07      	ldr	r2, [sp, #28]
300053fa:	4651      	mov	r1, sl
300053fc:	2000      	movs	r0, #0
300053fe:	9500      	str	r5, [sp, #0]
30005400:	9601      	str	r6, [sp, #4]
30005402:	e777      	b.n	300052f4 <BOOT_LoadImages+0x964>
30005404:	9904      	ldr	r1, [sp, #16]
30005406:	481d      	ldr	r0, [pc, #116]	; (3000547c <BOOT_LoadImages+0xaec>)
30005408:	f007 f94a 	bl	3000c6a0 <__DiagPrintf_veneer>
3000540c:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005410:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005414:	9307      	str	r3, [sp, #28]
30005416:	2301      	movs	r3, #1
30005418:	f8d5 b00d 	ldr.w	fp, [r5, #13]
3000541c:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005420:	9309      	str	r3, [sp, #36]	; 0x24
30005422:	e615      	b.n	30005050 <BOOT_LoadImages+0x6c0>
30005424:	f08a 0601 	eor.w	r6, sl, #1
30005428:	f006 0601 	and.w	r6, r6, #1
3000542c:	e408      	b.n	30004c40 <BOOT_LoadImages+0x2b0>
3000542e:	f08a 0601 	eor.w	r6, sl, #1
30005432:	f8cd 8014 	str.w	r8, [sp, #20]
30005436:	f006 0601 	and.w	r6, r6, #1
3000543a:	e401      	b.n	30004c40 <BOOT_LoadImages+0x2b0>
3000543c:	2601      	movs	r6, #1
3000543e:	4c0d      	ldr	r4, [pc, #52]	; (30005474 <BOOT_LoadImages+0xae4>)
30005440:	46b2      	mov	sl, r6
30005442:	f7ff bbc1 	b.w	30004bc8 <BOOT_LoadImages+0x238>
30005446:	f04f 0a00 	mov.w	sl, #0
3000544a:	e440      	b.n	30004cce <BOOT_LoadImages+0x33e>
3000544c:	3000cbe4 	.word	0x3000cbe4
30005450:	3000cb88 	.word	0x3000cb88
30005454:	3000c8d8 	.word	0x3000c8d8
30005458:	3000cb98 	.word	0x3000cb98
3000545c:	3000e8fb 	.word	0x3000e8fb
30005460:	3000cba8 	.word	0x3000cba8
30005464:	3000cc30 	.word	0x3000cc30
30005468:	3000c8f0 	.word	0x3000c8f0
3000546c:	3000ccf0 	.word	0x3000ccf0
30005470:	3000cd50 	.word	0x3000cd50
30005474:	3000eac0 	.word	0x3000eac0
30005478:	3000c9d8 	.word	0x3000c9d8
3000547c:	3000cd88 	.word	0x3000cd88
30005480:	3000ff18 	.word	0x3000ff18
30005484:	30006d39 	.word	0x30006d39

30005488 <BOOT_ReasonSet>:
30005488:	4a03      	ldr	r2, [pc, #12]	; (30005498 <BOOT_ReasonSet+0x10>)
3000548a:	4904      	ldr	r1, [pc, #16]	; (3000549c <BOOT_ReasonSet+0x14>)
3000548c:	8c13      	ldrh	r3, [r2, #32]
3000548e:	b29b      	uxth	r3, r3
30005490:	8413      	strh	r3, [r2, #32]
30005492:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005496:	4770      	bx	lr
30005498:	42008000 	.word	0x42008000
3000549c:	42008200 	.word	0x42008200

300054a0 <BOOT_Enable_KM0>:
300054a0:	b510      	push	{r4, lr}
300054a2:	4c12      	ldr	r4, [pc, #72]	; (300054ec <BOOT_Enable_KM0+0x4c>)
300054a4:	47a0      	blx	r4
300054a6:	b110      	cbz	r0, 300054ae <BOOT_Enable_KM0+0xe>
300054a8:	4b11      	ldr	r3, [pc, #68]	; (300054f0 <BOOT_Enable_KM0+0x50>)
300054aa:	4798      	blx	r3
300054ac:	b1a8      	cbz	r0, 300054da <BOOT_Enable_KM0+0x3a>
300054ae:	47a0      	blx	r4
300054b0:	b940      	cbnz	r0, 300054c4 <BOOT_Enable_KM0+0x24>
300054b2:	4a10      	ldr	r2, [pc, #64]	; (300054f4 <BOOT_Enable_KM0+0x54>)
300054b4:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300054b8:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300054bc:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300054c0:	bf40      	sev
300054c2:	bd10      	pop	{r4, pc}
300054c4:	4b0a      	ldr	r3, [pc, #40]	; (300054f0 <BOOT_Enable_KM0+0x50>)
300054c6:	4798      	blx	r3
300054c8:	2800      	cmp	r0, #0
300054ca:	d0f2      	beq.n	300054b2 <BOOT_Enable_KM0+0x12>
300054cc:	4b0a      	ldr	r3, [pc, #40]	; (300054f8 <BOOT_Enable_KM0+0x58>)
300054ce:	2004      	movs	r0, #4
300054d0:	4a0a      	ldr	r2, [pc, #40]	; (300054fc <BOOT_Enable_KM0+0x5c>)
300054d2:	4619      	mov	r1, r3
300054d4:	f006 fe3c 	bl	3000c150 <rtk_log_write>
300054d8:	e7eb      	b.n	300054b2 <BOOT_Enable_KM0+0x12>
300054da:	4b07      	ldr	r3, [pc, #28]	; (300054f8 <BOOT_Enable_KM0+0x58>)
300054dc:	2004      	movs	r0, #4
300054de:	4a08      	ldr	r2, [pc, #32]	; (30005500 <BOOT_Enable_KM0+0x60>)
300054e0:	4619      	mov	r1, r3
300054e2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300054e6:	f006 be33 	b.w	3000c150 <rtk_log_write>
300054ea:	bf00      	nop
300054ec:	0000c09d 	.word	0x0000c09d
300054f0:	0000c0ad 	.word	0x0000c0ad
300054f4:	42008000 	.word	0x42008000
300054f8:	3000c810 	.word	0x3000c810
300054fc:	3000cdcc 	.word	0x3000cdcc
30005500:	3000cda4 	.word	0x3000cda4

30005504 <BOOT_AP_Clk_Get>:
30005504:	4a06      	ldr	r2, [pc, #24]	; (30005520 <BOOT_AP_Clk_Get+0x1c>)
30005506:	6913      	ldr	r3, [r2, #16]
30005508:	2b00      	cmp	r3, #0
3000550a:	db03      	blt.n	30005514 <BOOT_AP_Clk_Get+0x10>
3000550c:	6810      	ldr	r0, [r2, #0]
3000550e:	fbb0 f0f3 	udiv	r0, r0, r3
30005512:	4770      	bx	lr
30005514:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30005518:	6850      	ldr	r0, [r2, #4]
3000551a:	fbb0 f0f3 	udiv	r0, r0, r3
3000551e:	4770      	bx	lr
30005520:	3000f148 	.word	0x3000f148

30005524 <BOOT_SOC_ClkSet>:
30005524:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005528:	4c5d      	ldr	r4, [pc, #372]	; (300056a0 <BOOT_SOC_ClkSet+0x17c>)
3000552a:	b083      	sub	sp, #12
3000552c:	6926      	ldr	r6, [r4, #16]
3000552e:	6825      	ldr	r5, [r4, #0]
30005530:	2e00      	cmp	r6, #0
30005532:	db7a      	blt.n	3000562a <BOOT_SOC_ClkSet+0x106>
30005534:	fbb5 f6f6 	udiv	r6, r5, r6
30005538:	4b5a      	ldr	r3, [pc, #360]	; (300056a4 <BOOT_SOC_ClkSet+0x180>)
3000553a:	68a7      	ldr	r7, [r4, #8]
3000553c:	fba3 1305 	umull	r1, r3, r3, r5
30005540:	68e2      	ldr	r2, [r4, #12]
30005542:	2f01      	cmp	r7, #1
30005544:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
30005548:	ea4f 1b53 	mov.w	fp, r3, lsr #5
3000554c:	ea4f 1393 	mov.w	r3, r3, lsr #6
30005550:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30005554:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30005558:	f103 39ff 	add.w	r9, r3, #4294967295	; 0xffffffff
3000555c:	fbb5 f5f2 	udiv	r5, r5, r2
30005560:	d06f      	beq.n	30005642 <BOOT_SOC_ClkSet+0x11e>
30005562:	4b51      	ldr	r3, [pc, #324]	; (300056a8 <BOOT_SOC_ClkSet+0x184>)
30005564:	2200      	movs	r2, #0
30005566:	2dfa      	cmp	r5, #250	; 0xfa
30005568:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
3000556c:	d863      	bhi.n	30005636 <BOOT_SOC_ClkSet+0x112>
3000556e:	4f4f      	ldr	r7, [pc, #316]	; (300056ac <BOOT_SOC_ClkSet+0x188>)
30005570:	783b      	ldrb	r3, [r7, #0]
30005572:	2b01      	cmp	r3, #1
30005574:	d103      	bne.n	3000557e <BOOT_SOC_ClkSet+0x5a>
30005576:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
3000557a:	f200 8085 	bhi.w	30005688 <BOOT_SOC_ClkSet+0x164>
3000557e:	2001      	movs	r0, #1
30005580:	f8df 8154 	ldr.w	r8, [pc, #340]	; 300056d8 <BOOT_SOC_ClkSet+0x1b4>
30005584:	f002 fcce 	bl	30007f24 <CLK_SWITCH_XTAL>
30005588:	6820      	ldr	r0, [r4, #0]
3000558a:	4b49      	ldr	r3, [pc, #292]	; (300056b0 <BOOT_SOC_ClkSet+0x18c>)
3000558c:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
30005590:	fb08 f000 	mul.w	r0, r8, r0
30005594:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
30005598:	4798      	blx	r3
3000559a:	4a46      	ldr	r2, [pc, #280]	; (300056b4 <BOOT_SOC_ClkSet+0x190>)
3000559c:	68e1      	ldr	r1, [r4, #12]
3000559e:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
300055a2:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
300055a6:	ea4f 5909 	mov.w	r9, r9, lsl #20
300055aa:	1e4b      	subs	r3, r1, #1
300055ac:	4942      	ldr	r1, [pc, #264]	; (300056b8 <BOOT_SOC_ClkSet+0x194>)
300055ae:	fa1f fb8b 	uxth.w	fp, fp
300055b2:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
300055b6:	4001      	ands	r1, r0
300055b8:	f003 0307 	and.w	r3, r3, #7
300055bc:	2000      	movs	r0, #0
300055be:	430b      	orrs	r3, r1
300055c0:	ea4a 0303 	orr.w	r3, sl, r3
300055c4:	ea4b 0303 	orr.w	r3, fp, r3
300055c8:	ea49 0303 	orr.w	r3, r9, r3
300055cc:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
300055d0:	f002 fca8 	bl	30007f24 <CLK_SWITCH_XTAL>
300055d4:	4b39      	ldr	r3, [pc, #228]	; (300056bc <BOOT_SOC_ClkSet+0x198>)
300055d6:	4a3a      	ldr	r2, [pc, #232]	; (300056c0 <BOOT_SOC_ClkSet+0x19c>)
300055d8:	2004      	movs	r0, #4
300055da:	4619      	mov	r1, r3
300055dc:	9500      	str	r5, [sp, #0]
300055de:	f006 fdb7 	bl	3000c150 <rtk_log_write>
300055e2:	783b      	ldrb	r3, [r7, #0]
300055e4:	b1f3      	cbz	r3, 30005624 <BOOT_SOC_ClkSet+0x100>
300055e6:	6923      	ldr	r3, [r4, #16]
300055e8:	2b00      	cmp	r3, #0
300055ea:	db40      	blt.n	3000566e <BOOT_SOC_ClkSet+0x14a>
300055ec:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300055f0:	492d      	ldr	r1, [pc, #180]	; (300056a8 <BOOT_SOC_ClkSet+0x184>)
300055f2:	2000      	movs	r0, #0
300055f4:	6913      	ldr	r3, [r2, #16]
300055f6:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
300055fa:	f043 030c 	orr.w	r3, r3, #12
300055fe:	6113      	str	r3, [r2, #16]
30005600:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
30005604:	6921      	ldr	r1, [r4, #16]
30005606:	4b2d      	ldr	r3, [pc, #180]	; (300056bc <BOOT_SOC_ClkSet+0x198>)
30005608:	6928      	ldr	r0, [r5, #16]
3000560a:	3901      	subs	r1, #1
3000560c:	4a2d      	ldr	r2, [pc, #180]	; (300056c4 <BOOT_SOC_ClkSet+0x1a0>)
3000560e:	f020 0003 	bic.w	r0, r0, #3
30005612:	f001 0103 	and.w	r1, r1, #3
30005616:	4301      	orrs	r1, r0
30005618:	2004      	movs	r0, #4
3000561a:	6129      	str	r1, [r5, #16]
3000561c:	4619      	mov	r1, r3
3000561e:	9600      	str	r6, [sp, #0]
30005620:	f006 fd96 	bl	3000c150 <rtk_log_write>
30005624:	b003      	add	sp, #12
30005626:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000562a:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
3000562e:	6863      	ldr	r3, [r4, #4]
30005630:	fbb3 f6f6 	udiv	r6, r3, r6
30005634:	e780      	b.n	30005538 <BOOT_SOC_ClkSet+0x14>
30005636:	f240 61b9 	movw	r1, #1721	; 0x6b9
3000563a:	4823      	ldr	r0, [pc, #140]	; (300056c8 <BOOT_SOC_ClkSet+0x1a4>)
3000563c:	f006 ffd0 	bl	3000c5e0 <__io_assert_failed_veneer>
30005640:	e795      	b.n	3000556e <BOOT_SOC_ClkSet+0x4a>
30005642:	4b22      	ldr	r3, [pc, #136]	; (300056cc <BOOT_SOC_ClkSet+0x1a8>)
30005644:	4638      	mov	r0, r7
30005646:	4798      	blx	r3
30005648:	4b17      	ldr	r3, [pc, #92]	; (300056a8 <BOOT_SOC_ClkSet+0x184>)
3000564a:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
3000564e:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
30005652:	d21f      	bcs.n	30005694 <BOOT_SOC_ClkSet+0x170>
30005654:	4f15      	ldr	r7, [pc, #84]	; (300056ac <BOOT_SOC_ClkSet+0x188>)
30005656:	783b      	ldrb	r3, [r7, #0]
30005658:	2b01      	cmp	r3, #1
3000565a:	d190      	bne.n	3000557e <BOOT_SOC_ClkSet+0x5a>
3000565c:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
30005660:	d98d      	bls.n	3000557e <BOOT_SOC_ClkSet+0x5a>
30005662:	f240 61b5 	movw	r1, #1717	; 0x6b5
30005666:	4818      	ldr	r0, [pc, #96]	; (300056c8 <BOOT_SOC_ClkSet+0x1a4>)
30005668:	f006 ffba 	bl	3000c5e0 <__io_assert_failed_veneer>
3000566c:	e787      	b.n	3000557e <BOOT_SOC_ClkSet+0x5a>
3000566e:	6860      	ldr	r0, [r4, #4]
30005670:	4b17      	ldr	r3, [pc, #92]	; (300056d0 <BOOT_SOC_ClkSet+0x1ac>)
30005672:	fb08 f000 	mul.w	r0, r8, r0
30005676:	4798      	blx	r3
30005678:	4b16      	ldr	r3, [pc, #88]	; (300056d4 <BOOT_SOC_ClkSet+0x1b0>)
3000567a:	2001      	movs	r0, #1
3000567c:	4798      	blx	r3
3000567e:	4b0a      	ldr	r3, [pc, #40]	; (300056a8 <BOOT_SOC_ClkSet+0x184>)
30005680:	2201      	movs	r2, #1
30005682:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30005686:	e7bb      	b.n	30005600 <BOOT_SOC_ClkSet+0xdc>
30005688:	f240 61bb 	movw	r1, #1723	; 0x6bb
3000568c:	480e      	ldr	r0, [pc, #56]	; (300056c8 <BOOT_SOC_ClkSet+0x1a4>)
3000568e:	f006 ffa7 	bl	3000c5e0 <__io_assert_failed_veneer>
30005692:	e774      	b.n	3000557e <BOOT_SOC_ClkSet+0x5a>
30005694:	f240 61b3 	movw	r1, #1715	; 0x6b3
30005698:	480b      	ldr	r0, [pc, #44]	; (300056c8 <BOOT_SOC_ClkSet+0x1a4>)
3000569a:	f006 ffa1 	bl	3000c5e0 <__io_assert_failed_veneer>
3000569e:	e7d9      	b.n	30005654 <BOOT_SOC_ClkSet+0x130>
300056a0:	3000f148 	.word	0x3000f148
300056a4:	51eb851f 	.word	0x51eb851f
300056a8:	23020000 	.word	0x23020000
300056ac:	3000e8fb 	.word	0x3000e8fb
300056b0:	30008d51 	.word	0x30008d51
300056b4:	42008000 	.word	0x42008000
300056b8:	ff8f08f8 	.word	0xff8f08f8
300056bc:	3000c810 	.word	0x3000c810
300056c0:	3000cdec 	.word	0x3000cdec
300056c4:	3000ce04 	.word	0x3000ce04
300056c8:	3000e5c0 	.word	0x3000e5c0
300056cc:	3000bad1 	.word	0x3000bad1
300056d0:	30008dd1 	.word	0x30008dd1
300056d4:	30008e11 	.word	0x30008e11
300056d8:	000f4240 	.word	0x000f4240

300056dc <BOOT_Disable_AP>:
300056dc:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300056e0:	4906      	ldr	r1, [pc, #24]	; (300056fc <BOOT_Disable_AP+0x20>)
300056e2:	685a      	ldr	r2, [r3, #4]
300056e4:	f042 0202 	orr.w	r2, r2, #2
300056e8:	605a      	str	r2, [r3, #4]
300056ea:	685a      	ldr	r2, [r3, #4]
300056ec:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300056f0:	605a      	str	r2, [r3, #4]
300056f2:	6f8b      	ldr	r3, [r1, #120]	; 0x78
300056f4:	f043 0303 	orr.w	r3, r3, #3
300056f8:	678b      	str	r3, [r1, #120]	; 0x78
300056fa:	4770      	bx	lr
300056fc:	41000200 	.word	0x41000200

30005700 <BOOT_Enable_AP>:
30005700:	b538      	push	{r3, r4, r5, lr}
30005702:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30005706:	2201      	movs	r2, #1
30005708:	2104      	movs	r1, #4
3000570a:	4b2b      	ldr	r3, [pc, #172]	; (300057b8 <BOOT_Enable_AP+0xb8>)
3000570c:	482b      	ldr	r0, [pc, #172]	; (300057bc <BOOT_Enable_AP+0xbc>)
3000570e:	4798      	blx	r3
30005710:	6863      	ldr	r3, [r4, #4]
30005712:	4d2b      	ldr	r5, [pc, #172]	; (300057c0 <BOOT_Enable_AP+0xc0>)
30005714:	2032      	movs	r0, #50	; 0x32
30005716:	f043 0302 	orr.w	r3, r3, #2
3000571a:	6063      	str	r3, [r4, #4]
3000571c:	6863      	ldr	r3, [r4, #4]
3000571e:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30005722:	6063      	str	r3, [r4, #4]
30005724:	6823      	ldr	r3, [r4, #0]
30005726:	f043 0302 	orr.w	r3, r3, #2
3000572a:	6023      	str	r3, [r4, #0]
3000572c:	47a8      	blx	r5
3000572e:	6823      	ldr	r3, [r4, #0]
30005730:	2032      	movs	r0, #50	; 0x32
30005732:	f043 0304 	orr.w	r3, r3, #4
30005736:	6023      	str	r3, [r4, #0]
30005738:	6823      	ldr	r3, [r4, #0]
3000573a:	f043 0301 	orr.w	r3, r3, #1
3000573e:	6023      	str	r3, [r4, #0]
30005740:	6823      	ldr	r3, [r4, #0]
30005742:	f043 0310 	orr.w	r3, r3, #16
30005746:	6023      	str	r3, [r4, #0]
30005748:	47a8      	blx	r5
3000574a:	6823      	ldr	r3, [r4, #0]
3000574c:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30005750:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30005754:	6023      	str	r3, [r4, #0]
30005756:	47a8      	blx	r5
30005758:	6863      	ldr	r3, [r4, #4]
3000575a:	4a1a      	ldr	r2, [pc, #104]	; (300057c4 <BOOT_Enable_AP+0xc4>)
3000575c:	f023 0310 	bic.w	r3, r3, #16
30005760:	6063      	str	r3, [r4, #4]
30005762:	6863      	ldr	r3, [r4, #4]
30005764:	f023 0302 	bic.w	r3, r3, #2
30005768:	6063      	str	r3, [r4, #4]
3000576a:	4790      	blx	r2
3000576c:	b178      	cbz	r0, 3000578e <BOOT_Enable_AP+0x8e>
3000576e:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005772:	4815      	ldr	r0, [pc, #84]	; (300057c8 <BOOT_Enable_AP+0xc8>)
30005774:	4a15      	ldr	r2, [pc, #84]	; (300057cc <BOOT_Enable_AP+0xcc>)
30005776:	68d9      	ldr	r1, [r3, #12]
30005778:	f041 0101 	orr.w	r1, r1, #1
3000577c:	60d9      	str	r1, [r3, #12]
3000577e:	6899      	ldr	r1, [r3, #8]
30005780:	f041 0101 	orr.w	r1, r1, #1
30005784:	6099      	str	r1, [r3, #8]
30005786:	6843      	ldr	r3, [r0, #4]
30005788:	431a      	orrs	r2, r3
3000578a:	6042      	str	r2, [r0, #4]
3000578c:	bd38      	pop	{r3, r4, r5, pc}
3000578e:	4604      	mov	r4, r0
30005790:	4d0f      	ldr	r5, [pc, #60]	; (300057d0 <BOOT_Enable_AP+0xd0>)
30005792:	4601      	mov	r1, r0
30005794:	2041      	movs	r0, #65	; 0x41
30005796:	47a8      	blx	r5
30005798:	4621      	mov	r1, r4
3000579a:	2042      	movs	r0, #66	; 0x42
3000579c:	47a8      	blx	r5
3000579e:	4621      	mov	r1, r4
300057a0:	2043      	movs	r0, #67	; 0x43
300057a2:	47a8      	blx	r5
300057a4:	4621      	mov	r1, r4
300057a6:	2044      	movs	r0, #68	; 0x44
300057a8:	47a8      	blx	r5
300057aa:	4621      	mov	r1, r4
300057ac:	2045      	movs	r0, #69	; 0x45
300057ae:	47a8      	blx	r5
300057b0:	4621      	mov	r1, r4
300057b2:	2046      	movs	r0, #70	; 0x46
300057b4:	47a8      	blx	r5
300057b6:	e7da      	b.n	3000576e <BOOT_Enable_AP+0x6e>
300057b8:	0000aab9 	.word	0x0000aab9
300057bc:	4200c000 	.word	0x4200c000
300057c0:	00009b2d 	.word	0x00009b2d
300057c4:	30009ebd 	.word	0x30009ebd
300057c8:	41000200 	.word	0x41000200
300057cc:	01001111 	.word	0x01001111
300057d0:	0000b20d 	.word	0x0000b20d

300057d4 <BOOT_RAM_KeyDeriveFunc>:
300057d4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300057d8:	ed2d 8b04 	vpush	{d8-d9}
300057dc:	b0cb      	sub	sp, #300	; 0x12c
300057de:	4c6a      	ldr	r4, [pc, #424]	; (30005988 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
300057e0:	4680      	mov	r8, r0
300057e2:	4689      	mov	r9, r1
300057e4:	4869      	ldr	r0, [pc, #420]	; (3000598c <BOOT_RAM_KeyDeriveFunc+0x1b8>)
300057e6:	ee08 2a90 	vmov	s17, r2
300057ea:	4969      	ldr	r1, [pc, #420]	; (30005990 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
300057ec:	2201      	movs	r2, #1
300057ee:	ee09 3a10 	vmov	s18, r3
300057f2:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
300057f6:	47a0      	blx	r4
300057f8:	2201      	movs	r2, #1
300057fa:	4966      	ldr	r1, [pc, #408]	; (30005994 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
300057fc:	ae02      	add	r6, sp, #8
300057fe:	4864      	ldr	r0, [pc, #400]	; (30005990 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005800:	47a0      	blx	r4
30005802:	4b65      	ldr	r3, [pc, #404]	; (30005998 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30005804:	2000      	movs	r0, #0
30005806:	4798      	blx	r3
30005808:	22e0      	movs	r2, #224	; 0xe0
3000580a:	2100      	movs	r1, #0
3000580c:	a812      	add	r0, sp, #72	; 0x48
3000580e:	f006 feeb 	bl	3000c5e8 <____wrap_memset_veneer>
30005812:	2300      	movs	r3, #0
30005814:	f8ad 3004 	strh.w	r3, [sp, #4]
30005818:	f88d 3006 	strb.w	r3, [sp, #6]
3000581c:	2301      	movs	r3, #1
3000581e:	f88d 3007 	strb.w	r3, [sp, #7]
30005822:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005824:	2b00      	cmp	r3, #0
30005826:	f000 80ab 	beq.w	30005980 <BOOT_RAM_KeyDeriveFunc+0x1ac>
3000582a:	ab0a      	add	r3, sp, #40	; 0x28
3000582c:	f8df b17c 	ldr.w	fp, [pc, #380]	; 300059ac <BOOT_RAM_KeyDeriveFunc+0x1d8>
30005830:	4f5a      	ldr	r7, [pc, #360]	; (3000599c <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30005832:	ee08 3a10 	vmov	s16, r3
30005836:	ab12      	add	r3, sp, #72	; 0x48
30005838:	464a      	mov	r2, r9
3000583a:	4641      	mov	r1, r8
3000583c:	2020      	movs	r0, #32
3000583e:	47d8      	blx	fp
30005840:	aa12      	add	r2, sp, #72	; 0x48
30005842:	ee19 1a10 	vmov	r1, s18
30005846:	ee18 0a90 	vmov	r0, s17
3000584a:	47b8      	blx	r7
3000584c:	ee18 5a10 	vmov	r5, s16
30005850:	aa12      	add	r2, sp, #72	; 0x48
30005852:	2104      	movs	r1, #4
30005854:	a801      	add	r0, sp, #4
30005856:	4634      	mov	r4, r6
30005858:	47b8      	blx	r7
3000585a:	a912      	add	r1, sp, #72	; 0x48
3000585c:	4b50      	ldr	r3, [pc, #320]	; (300059a0 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
3000585e:	ee18 0a10 	vmov	r0, s16
30005862:	4798      	blx	r3
30005864:	f1ba 0f01 	cmp.w	sl, #1
30005868:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
3000586a:	c40f      	stmia	r4!, {r0, r1, r2, r3}
3000586c:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30005870:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30005874:	d91f      	bls.n	300058b6 <BOOT_RAM_KeyDeriveFunc+0xe2>
30005876:	2501      	movs	r5, #1
30005878:	f10d 0447 	add.w	r4, sp, #71	; 0x47
3000587c:	ab12      	add	r3, sp, #72	; 0x48
3000587e:	4641      	mov	r1, r8
30005880:	464a      	mov	r2, r9
30005882:	2020      	movs	r0, #32
30005884:	47d8      	blx	fp
30005886:	aa12      	add	r2, sp, #72	; 0x48
30005888:	2120      	movs	r1, #32
3000588a:	4630      	mov	r0, r6
3000588c:	47b8      	blx	r7
3000588e:	a912      	add	r1, sp, #72	; 0x48
30005890:	4b43      	ldr	r3, [pc, #268]	; (300059a0 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30005892:	4630      	mov	r0, r6
30005894:	4798      	blx	r3
30005896:	f10d 0327 	add.w	r3, sp, #39	; 0x27
3000589a:	f10d 0107 	add.w	r1, sp, #7
3000589e:	f813 2f01 	ldrb.w	r2, [r3, #1]!
300058a2:	f811 0f01 	ldrb.w	r0, [r1, #1]!
300058a6:	42a3      	cmp	r3, r4
300058a8:	ea82 0200 	eor.w	r2, r2, r0
300058ac:	701a      	strb	r2, [r3, #0]
300058ae:	d1f6      	bne.n	3000589e <BOOT_RAM_KeyDeriveFunc+0xca>
300058b0:	3501      	adds	r5, #1
300058b2:	45aa      	cmp	sl, r5
300058b4:	d1e2      	bne.n	3000587c <BOOT_RAM_KeyDeriveFunc+0xa8>
300058b6:	9a59      	ldr	r2, [sp, #356]	; 0x164
300058b8:	ee18 1a10 	vmov	r1, s16
300058bc:	9b59      	ldr	r3, [sp, #356]	; 0x164
300058be:	2a20      	cmp	r2, #32
300058c0:	985a      	ldr	r0, [sp, #360]	; 0x168
300058c2:	bf28      	it	cs
300058c4:	2220      	movcs	r2, #32
300058c6:	1a9b      	subs	r3, r3, r2
300058c8:	9359      	str	r3, [sp, #356]	; 0x164
300058ca:	4603      	mov	r3, r0
300058cc:	4413      	add	r3, r2
300058ce:	935a      	str	r3, [sp, #360]	; 0x168
300058d0:	f006 fe9a 	bl	3000c608 <____wrap_memcpy_veneer>
300058d4:	f10d 0207 	add.w	r2, sp, #7
300058d8:	2104      	movs	r1, #4
300058da:	7813      	ldrb	r3, [r2, #0]
300058dc:	3901      	subs	r1, #1
300058de:	3301      	adds	r3, #1
300058e0:	b2db      	uxtb	r3, r3
300058e2:	f802 3901 	strb.w	r3, [r2], #-1
300058e6:	b90b      	cbnz	r3, 300058ec <BOOT_RAM_KeyDeriveFunc+0x118>
300058e8:	2900      	cmp	r1, #0
300058ea:	d1f6      	bne.n	300058da <BOOT_RAM_KeyDeriveFunc+0x106>
300058ec:	9b59      	ldr	r3, [sp, #356]	; 0x164
300058ee:	2b00      	cmp	r3, #0
300058f0:	d1a1      	bne.n	30005836 <BOOT_RAM_KeyDeriveFunc+0x62>
300058f2:	2220      	movs	r2, #32
300058f4:	2100      	movs	r1, #0
300058f6:	4c2b      	ldr	r4, [pc, #172]	; (300059a4 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
300058f8:	4630      	mov	r0, r6
300058fa:	47a0      	blx	r4
300058fc:	2220      	movs	r2, #32
300058fe:	2100      	movs	r1, #0
30005900:	ee18 0a10 	vmov	r0, s16
30005904:	47a0      	blx	r4
30005906:	4b28      	ldr	r3, [pc, #160]	; (300059a8 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005908:	695b      	ldr	r3, [r3, #20]
3000590a:	03da      	lsls	r2, r3, #15
3000590c:	d512      	bpl.n	30005934 <BOOT_RAM_KeyDeriveFunc+0x160>
3000590e:	f016 021f 	ands.w	r2, r6, #31
30005912:	4633      	mov	r3, r6
30005914:	d130      	bne.n	30005978 <BOOT_RAM_KeyDeriveFunc+0x1a4>
30005916:	2220      	movs	r2, #32
30005918:	f3bf 8f4f 	dsb	sy
3000591c:	4822      	ldr	r0, [pc, #136]	; (300059a8 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
3000591e:	441a      	add	r2, r3
30005920:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005924:	3320      	adds	r3, #32
30005926:	1ad1      	subs	r1, r2, r3
30005928:	2900      	cmp	r1, #0
3000592a:	dcf9      	bgt.n	30005920 <BOOT_RAM_KeyDeriveFunc+0x14c>
3000592c:	f3bf 8f4f 	dsb	sy
30005930:	f3bf 8f6f 	isb	sy
30005934:	4b1c      	ldr	r3, [pc, #112]	; (300059a8 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005936:	695b      	ldr	r3, [r3, #20]
30005938:	03db      	lsls	r3, r3, #15
3000593a:	d513      	bpl.n	30005964 <BOOT_RAM_KeyDeriveFunc+0x190>
3000593c:	ee18 3a10 	vmov	r3, s16
30005940:	f013 021f 	ands.w	r2, r3, #31
30005944:	d114      	bne.n	30005970 <BOOT_RAM_KeyDeriveFunc+0x19c>
30005946:	2220      	movs	r2, #32
30005948:	f3bf 8f4f 	dsb	sy
3000594c:	4816      	ldr	r0, [pc, #88]	; (300059a8 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
3000594e:	441a      	add	r2, r3
30005950:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005954:	3320      	adds	r3, #32
30005956:	1ad1      	subs	r1, r2, r3
30005958:	2900      	cmp	r1, #0
3000595a:	dcf9      	bgt.n	30005950 <BOOT_RAM_KeyDeriveFunc+0x17c>
3000595c:	f3bf 8f4f 	dsb	sy
30005960:	f3bf 8f6f 	isb	sy
30005964:	2000      	movs	r0, #0
30005966:	b04b      	add	sp, #300	; 0x12c
30005968:	ecbd 8b04 	vpop	{d8-d9}
3000596c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005970:	f023 031f 	bic.w	r3, r3, #31
30005974:	3220      	adds	r2, #32
30005976:	e7e7      	b.n	30005948 <BOOT_RAM_KeyDeriveFunc+0x174>
30005978:	f026 031f 	bic.w	r3, r6, #31
3000597c:	3220      	adds	r2, #32
3000597e:	e7cb      	b.n	30005918 <BOOT_RAM_KeyDeriveFunc+0x144>
30005980:	ab0a      	add	r3, sp, #40	; 0x28
30005982:	ee08 3a10 	vmov	s16, r3
30005986:	e7b4      	b.n	300058f2 <BOOT_RAM_KeyDeriveFunc+0x11e>
30005988:	0000b479 	.word	0x0000b479
3000598c:	40000008 	.word	0x40000008
30005990:	40000004 	.word	0x40000004
30005994:	40000002 	.word	0x40000002
30005998:	3000a159 	.word	0x3000a159
3000599c:	30009f5d 	.word	0x30009f5d
300059a0:	30009f99 	.word	0x30009f99
300059a4:	00012be5 	.word	0x00012be5
300059a8:	e000ed00 	.word	0xe000ed00
300059ac:	300097dd 	.word	0x300097dd

300059b0 <password_hash_check>:
300059b0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300059b4:	b08f      	sub	sp, #60	; 0x3c
300059b6:	4690      	mov	r8, r2
300059b8:	461d      	mov	r5, r3
300059ba:	4606      	mov	r6, r0
300059bc:	af04      	add	r7, sp, #16
300059be:	3201      	adds	r2, #1
300059c0:	6d3b      	ldr	r3, [r7, #80]	; 0x50
300059c2:	6079      	str	r1, [r7, #4]
300059c4:	2100      	movs	r1, #0
300059c6:	f103 0008 	add.w	r0, r3, #8
300059ca:	f108 0308 	add.w	r3, r8, #8
300059ce:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
300059d2:	f023 0307 	bic.w	r3, r3, #7
300059d6:	f020 0007 	bic.w	r0, r0, #7
300059da:	ebad 0d03 	sub.w	sp, sp, r3
300059de:	6d3b      	ldr	r3, [r7, #80]	; 0x50
300059e0:	f10d 0910 	add.w	r9, sp, #16
300059e4:	3301      	adds	r3, #1
300059e6:	ebad 0d00 	sub.w	sp, sp, r0
300059ea:	603b      	str	r3, [r7, #0]
300059ec:	4648      	mov	r0, r9
300059ee:	4b3f      	ldr	r3, [pc, #252]	; (30005aec <password_hash_check+0x13c>)
300059f0:	f10d 0a10 	add.w	sl, sp, #16
300059f4:	4798      	blx	r3
300059f6:	683b      	ldr	r3, [r7, #0]
300059f8:	2100      	movs	r1, #0
300059fa:	4650      	mov	r0, sl
300059fc:	461a      	mov	r2, r3
300059fe:	4b3b      	ldr	r3, [pc, #236]	; (30005aec <password_hash_check+0x13c>)
30005a00:	4798      	blx	r3
30005a02:	f1b8 0001 	subs.w	r0, r8, #1
30005a06:	d01d      	beq.n	30005a44 <password_hash_check+0x94>
30005a08:	2200      	movs	r2, #0
30005a0a:	f109 33ff 	add.w	r3, r9, #4294967295	; 0xffffffff
30005a0e:	4611      	mov	r1, r2
30005a10:	e00b      	b.n	30005a2a <password_hash_check+0x7a>
30005a12:	f108 38ff 	add.w	r8, r8, #4294967295	; 0xffffffff
30005a16:	2100      	movs	r1, #0
30005a18:	1c9c      	adds	r4, r3, #2
30005a1a:	4282      	cmp	r2, r0
30005a1c:	7059      	strb	r1, [r3, #1]
30005a1e:	f103 0301 	add.w	r3, r3, #1
30005a22:	4611      	mov	r1, r2
30005a24:	eba4 0409 	sub.w	r4, r4, r9
30005a28:	d20b      	bcs.n	30005a42 <password_hash_check+0x92>
30005a2a:	1874      	adds	r4, r6, r1
30005a2c:	f102 0c01 	add.w	ip, r2, #1
30005a30:	5c71      	ldrb	r1, [r6, r1]
30005a32:	3202      	adds	r2, #2
30005a34:	7864      	ldrb	r4, [r4, #1]
30005a36:	2c30      	cmp	r4, #48	; 0x30
30005a38:	d101      	bne.n	30005a3e <password_hash_check+0x8e>
30005a3a:	295c      	cmp	r1, #92	; 0x5c
30005a3c:	d0e9      	beq.n	30005a12 <password_hash_check+0x62>
30005a3e:	4662      	mov	r2, ip
30005a40:	e7ea      	b.n	30005a18 <password_hash_check+0x68>
30005a42:	4406      	add	r6, r0
30005a44:	7833      	ldrb	r3, [r6, #0]
30005a46:	f809 3004 	strb.w	r3, [r9, r4]
30005a4a:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30005a4c:	1e5c      	subs	r4, r3, #1
30005a4e:	d04a      	beq.n	30005ae6 <password_hash_check+0x136>
30005a50:	2200      	movs	r2, #0
30005a52:	f10a 33ff 	add.w	r3, sl, #4294967295	; 0xffffffff
30005a56:	4611      	mov	r1, r2
30005a58:	e00b      	b.n	30005a72 <password_hash_check+0xc2>
30005a5a:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30005a5e:	2100      	movs	r1, #0
30005a60:	1c98      	adds	r0, r3, #2
30005a62:	42a2      	cmp	r2, r4
30005a64:	7059      	strb	r1, [r3, #1]
30005a66:	f103 0301 	add.w	r3, r3, #1
30005a6a:	4611      	mov	r1, r2
30005a6c:	eba0 000a 	sub.w	r0, r0, sl
30005a70:	d20a      	bcs.n	30005a88 <password_hash_check+0xd8>
30005a72:	1868      	adds	r0, r5, r1
30005a74:	1c56      	adds	r6, r2, #1
30005a76:	5c69      	ldrb	r1, [r5, r1]
30005a78:	3202      	adds	r2, #2
30005a7a:	7840      	ldrb	r0, [r0, #1]
30005a7c:	2830      	cmp	r0, #48	; 0x30
30005a7e:	d101      	bne.n	30005a84 <password_hash_check+0xd4>
30005a80:	295c      	cmp	r1, #92	; 0x5c
30005a82:	d0ea      	beq.n	30005a5a <password_hash_check+0xaa>
30005a84:	4632      	mov	r2, r6
30005a86:	e7eb      	b.n	30005a60 <password_hash_check+0xb0>
30005a88:	4425      	add	r5, r4
30005a8a:	782b      	ldrb	r3, [r5, #0]
30005a8c:	f107 0408 	add.w	r4, r7, #8
30005a90:	2520      	movs	r5, #32
30005a92:	4641      	mov	r1, r8
30005a94:	f80a 3000 	strb.w	r3, [sl, r0]
30005a98:	4652      	mov	r2, sl
30005a9a:	687b      	ldr	r3, [r7, #4]
30005a9c:	4648      	mov	r0, r9
30005a9e:	9402      	str	r4, [sp, #8]
30005aa0:	9300      	str	r3, [sp, #0]
30005aa2:	465b      	mov	r3, fp
30005aa4:	9501      	str	r5, [sp, #4]
30005aa6:	f7ff fe95 	bl	300057d4 <BOOT_RAM_KeyDeriveFunc>
30005aaa:	4620      	mov	r0, r4
30005aac:	462a      	mov	r2, r5
30005aae:	4910      	ldr	r1, [pc, #64]	; (30005af0 <password_hash_check+0x140>)
30005ab0:	f006 fdae 	bl	3000c610 <____wrap_memcmp_veneer>
30005ab4:	4604      	mov	r4, r0
30005ab6:	b968      	cbnz	r0, 30005ad4 <password_hash_check+0x124>
30005ab8:	480e      	ldr	r0, [pc, #56]	; (30005af4 <password_hash_check+0x144>)
30005aba:	f006 fdf1 	bl	3000c6a0 <__DiagPrintf_veneer>
30005abe:	462a      	mov	r2, r5
30005ac0:	4621      	mov	r1, r4
30005ac2:	f107 0008 	add.w	r0, r7, #8
30005ac6:	4b09      	ldr	r3, [pc, #36]	; (30005aec <password_hash_check+0x13c>)
30005ac8:	4798      	blx	r3
30005aca:	4620      	mov	r0, r4
30005acc:	372c      	adds	r7, #44	; 0x2c
30005ace:	46bd      	mov	sp, r7
30005ad0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005ad4:	2401      	movs	r4, #1
30005ad6:	4808      	ldr	r0, [pc, #32]	; (30005af8 <password_hash_check+0x148>)
30005ad8:	f006 fde2 	bl	3000c6a0 <__DiagPrintf_veneer>
30005adc:	372c      	adds	r7, #44	; 0x2c
30005ade:	4620      	mov	r0, r4
30005ae0:	46bd      	mov	sp, r7
30005ae2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005ae6:	4620      	mov	r0, r4
30005ae8:	e7cf      	b.n	30005a8a <password_hash_check+0xda>
30005aea:	bf00      	nop
30005aec:	00012be5 	.word	0x00012be5
30005af0:	3000eec8 	.word	0x3000eec8
30005af4:	3000ce34 	.word	0x3000ce34
30005af8:	3000ce1c 	.word	0x3000ce1c

30005afc <BOOT_Share_Memory_Patch>:
30005afc:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005b00:	4817      	ldr	r0, [pc, #92]	; (30005b60 <BOOT_Share_Memory_Patch+0x64>)
30005b02:	4918      	ldr	r1, [pc, #96]	; (30005b64 <BOOT_Share_Memory_Patch+0x68>)
30005b04:	b430      	push	{r4, r5}
30005b06:	6b13      	ldr	r3, [r2, #48]	; 0x30
30005b08:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30005b0c:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30005b10:	6313      	str	r3, [r2, #48]	; 0x30
30005b12:	6823      	ldr	r3, [r4, #0]
30005b14:	6804      	ldr	r4, [r0, #0]
30005b16:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005b1a:	6809      	ldr	r1, [r1, #0]
30005b1c:	4423      	add	r3, r4
30005b1e:	6805      	ldr	r5, [r0, #0]
30005b20:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30005b24:	440b      	add	r3, r1
30005b26:	4910      	ldr	r1, [pc, #64]	; (30005b68 <BOOT_Share_Memory_Patch+0x6c>)
30005b28:	6808      	ldr	r0, [r1, #0]
30005b2a:	442b      	add	r3, r5
30005b2c:	6b11      	ldr	r1, [r2, #48]	; 0x30
30005b2e:	4403      	add	r3, r0
30005b30:	480e      	ldr	r0, [pc, #56]	; (30005b6c <BOOT_Share_Memory_Patch+0x70>)
30005b32:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30005b36:	6311      	str	r1, [r2, #48]	; 0x30
30005b38:	6821      	ldr	r1, [r4, #0]
30005b3a:	4a0d      	ldr	r2, [pc, #52]	; (30005b70 <BOOT_Share_Memory_Patch+0x74>)
30005b3c:	6804      	ldr	r4, [r0, #0]
30005b3e:	440b      	add	r3, r1
30005b40:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005b44:	6811      	ldr	r1, [r2, #0]
30005b46:	4423      	add	r3, r4
30005b48:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30005b4c:	6800      	ldr	r0, [r0, #0]
30005b4e:	440b      	add	r3, r1
30005b50:	6812      	ldr	r2, [r2, #0]
30005b52:	4908      	ldr	r1, [pc, #32]	; (30005b74 <BOOT_Share_Memory_Patch+0x78>)
30005b54:	4403      	add	r3, r0
30005b56:	6808      	ldr	r0, [r1, #0]
30005b58:	4413      	add	r3, r2
30005b5a:	bc30      	pop	{r4, r5}
30005b5c:	4418      	add	r0, r3
30005b5e:	4770      	bx	lr
30005b60:	2200c000 	.word	0x2200c000
30005b64:	22018000 	.word	0x22018000
30005b68:	22100000 	.word	0x22100000
30005b6c:	20010000 	.word	0x20010000
30005b70:	20020000 	.word	0x20020000
30005b74:	23010000 	.word	0x23010000

30005b78 <BOOT_Image1>:
30005b78:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005b7c:	4b8e      	ldr	r3, [pc, #568]	; (30005db8 <BOOT_Image1+0x240>)
30005b7e:	b089      	sub	sp, #36	; 0x24
30005b80:	4798      	blx	r3
30005b82:	2800      	cmp	r0, #0
30005b84:	f040 82dc 	bne.w	30006140 <BOOT_Image1+0x5c8>
30005b88:	488c      	ldr	r0, [pc, #560]	; (30005dbc <BOOT_Image1+0x244>)
30005b8a:	f006 fd89 	bl	3000c6a0 <__DiagPrintf_veneer>
30005b8e:	498c      	ldr	r1, [pc, #560]	; (30005dc0 <BOOT_Image1+0x248>)
30005b90:	488c      	ldr	r0, [pc, #560]	; (30005dc4 <BOOT_Image1+0x24c>)
30005b92:	f006 fd85 	bl	3000c6a0 <__DiagPrintf_veneer>
30005b96:	488c      	ldr	r0, [pc, #560]	; (30005dc8 <BOOT_Image1+0x250>)
30005b98:	4a8c      	ldr	r2, [pc, #560]	; (30005dcc <BOOT_Image1+0x254>)
30005b9a:	2100      	movs	r1, #0
30005b9c:	4b8c      	ldr	r3, [pc, #560]	; (30005dd0 <BOOT_Image1+0x258>)
30005b9e:	1a12      	subs	r2, r2, r0
30005ba0:	4798      	blx	r3
30005ba2:	4a8c      	ldr	r2, [pc, #560]	; (30005dd4 <BOOT_Image1+0x25c>)
30005ba4:	498c      	ldr	r1, [pc, #560]	; (30005dd8 <BOOT_Image1+0x260>)
30005ba6:	8c13      	ldrh	r3, [r2, #32]
30005ba8:	b29b      	uxth	r3, r3
30005baa:	8413      	strh	r3, [r2, #32]
30005bac:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005bb0:	f003 fd36 	bl	30009620 <BOOT_Reason>
30005bb4:	2800      	cmp	r0, #0
30005bb6:	f000 8328 	beq.w	3000620a <BOOT_Image1+0x692>
30005bba:	4d88      	ldr	r5, [pc, #544]	; (30005ddc <BOOT_Image1+0x264>)
30005bbc:	4b88      	ldr	r3, [pc, #544]	; (30005de0 <BOOT_Image1+0x268>)
30005bbe:	4798      	blx	r3
30005bc0:	782c      	ldrb	r4, [r5, #0]
30005bc2:	2c00      	cmp	r4, #0
30005bc4:	f000 82b2 	beq.w	3000612c <BOOT_Image1+0x5b4>
30005bc8:	2000      	movs	r0, #0
30005bca:	4b86      	ldr	r3, [pc, #536]	; (30005de4 <BOOT_Image1+0x26c>)
30005bcc:	4798      	blx	r3
30005bce:	4b86      	ldr	r3, [pc, #536]	; (30005de8 <BOOT_Image1+0x270>)
30005bd0:	2001      	movs	r0, #1
30005bd2:	4798      	blx	r3
30005bd4:	4c80      	ldr	r4, [pc, #512]	; (30005dd8 <BOOT_Image1+0x260>)
30005bd6:	f7ff fca5 	bl	30005524 <BOOT_SOC_ClkSet>
30005bda:	f241 070a 	movw	r7, #4106	; 0x100a
30005bde:	2028      	movs	r0, #40	; 0x28
30005be0:	8823      	ldrh	r3, [r4, #0]
30005be2:	4e82      	ldr	r6, [pc, #520]	; (30005dec <BOOT_Image1+0x274>)
30005be4:	b29b      	uxth	r3, r3
30005be6:	431f      	orrs	r7, r3
30005be8:	4b81      	ldr	r3, [pc, #516]	; (30005df0 <BOOT_Image1+0x278>)
30005bea:	8027      	strh	r7, [r4, #0]
30005bec:	4798      	blx	r3
30005bee:	88a3      	ldrh	r3, [r4, #4]
30005bf0:	b29b      	uxth	r3, r3
30005bf2:	ea23 0307 	bic.w	r3, r3, r7
30005bf6:	80a3      	strh	r3, [r4, #4]
30005bf8:	8863      	ldrh	r3, [r4, #2]
30005bfa:	8067      	strh	r7, [r4, #2]
30005bfc:	47b0      	blx	r6
30005bfe:	2802      	cmp	r0, #2
30005c00:	f000 82ea 	beq.w	300061d8 <BOOT_Image1+0x660>
30005c04:	4b7b      	ldr	r3, [pc, #492]	; (30005df4 <BOOT_Image1+0x27c>)
30005c06:	2401      	movs	r4, #1
30005c08:	497b      	ldr	r1, [pc, #492]	; (30005df8 <BOOT_Image1+0x280>)
30005c0a:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005c0e:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
30005c12:	4622      	mov	r2, r4
30005c14:	4f79      	ldr	r7, [pc, #484]	; (30005dfc <BOOT_Image1+0x284>)
30005c16:	47b8      	blx	r7
30005c18:	4622      	mov	r2, r4
30005c1a:	4979      	ldr	r1, [pc, #484]	; (30005e00 <BOOT_Image1+0x288>)
30005c1c:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005c20:	47b8      	blx	r7
30005c22:	4a6c      	ldr	r2, [pc, #432]	; (30005dd4 <BOOT_Image1+0x25c>)
30005c24:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005c28:	f043 0303 	orr.w	r3, r3, #3
30005c2c:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005c30:	4b74      	ldr	r3, [pc, #464]	; (30005e04 <BOOT_Image1+0x28c>)
30005c32:	4798      	blx	r3
30005c34:	4a67      	ldr	r2, [pc, #412]	; (30005dd4 <BOOT_Image1+0x25c>)
30005c36:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30005c3a:	f023 0303 	bic.w	r3, r3, #3
30005c3e:	b108      	cbz	r0, 30005c44 <BOOT_Image1+0xcc>
30005c40:	f043 0302 	orr.w	r3, r3, #2
30005c44:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30005c48:	4b6f      	ldr	r3, [pc, #444]	; (30005e08 <BOOT_Image1+0x290>)
30005c4a:	781b      	ldrb	r3, [r3, #0]
30005c4c:	2b00      	cmp	r3, #0
30005c4e:	f040 82e9 	bne.w	30006224 <BOOT_Image1+0x6ac>
30005c52:	4c6e      	ldr	r4, [pc, #440]	; (30005e0c <BOOT_Image1+0x294>)
30005c54:	2201      	movs	r2, #1
30005c56:	2102      	movs	r1, #2
30005c58:	486d      	ldr	r0, [pc, #436]	; (30005e10 <BOOT_Image1+0x298>)
30005c5a:	47a0      	blx	r4
30005c5c:	4c6d      	ldr	r4, [pc, #436]	; (30005e14 <BOOT_Image1+0x29c>)
30005c5e:	68a3      	ldr	r3, [r4, #8]
30005c60:	079f      	lsls	r7, r3, #30
30005c62:	f100 82d9 	bmi.w	30006218 <BOOT_Image1+0x6a0>
30005c66:	496c      	ldr	r1, [pc, #432]	; (30005e18 <BOOT_Image1+0x2a0>)
30005c68:	e841 f100 	tt	r1, r1
30005c6c:	68a3      	ldr	r3, [r4, #8]
30005c6e:	f3c1 5180 	ubfx	r1, r1, #22, #1
30005c72:	0798      	lsls	r0, r3, #30
30005c74:	f100 82c5 	bmi.w	30006202 <BOOT_Image1+0x68a>
30005c78:	4b68      	ldr	r3, [pc, #416]	; (30005e1c <BOOT_Image1+0x2a4>)
30005c7a:	4f69      	ldr	r7, [pc, #420]	; (30005e20 <BOOT_Image1+0x2a8>)
30005c7c:	681b      	ldr	r3, [r3, #0]
30005c7e:	4798      	blx	r3
30005c80:	f7fe fa56 	bl	30004130 <BOOT_RccConfig>
30005c84:	f7fe fac0 	bl	30004208 <BOOT_CACHEWRR_Set>
30005c88:	4b66      	ldr	r3, [pc, #408]	; (30005e24 <BOOT_Image1+0x2ac>)
30005c8a:	6818      	ldr	r0, [r3, #0]
30005c8c:	f7fe faea 	bl	30004264 <BOOT_TCMSet>
30005c90:	4b65      	ldr	r3, [pc, #404]	; (30005e28 <BOOT_Image1+0x2b0>)
30005c92:	695b      	ldr	r3, [r3, #20]
30005c94:	03da      	lsls	r2, r3, #15
30005c96:	d513      	bpl.n	30005cc0 <BOOT_Image1+0x148>
30005c98:	f017 011f 	ands.w	r1, r7, #31
30005c9c:	f040 8261 	bne.w	30006162 <BOOT_Image1+0x5ea>
30005ca0:	463b      	mov	r3, r7
30005ca2:	2174      	movs	r1, #116	; 0x74
30005ca4:	f3bf 8f4f 	dsb	sy
30005ca8:	485f      	ldr	r0, [pc, #380]	; (30005e28 <BOOT_Image1+0x2b0>)
30005caa:	4419      	add	r1, r3
30005cac:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30005cb0:	3320      	adds	r3, #32
30005cb2:	1aca      	subs	r2, r1, r3
30005cb4:	2a00      	cmp	r2, #0
30005cb6:	dcf9      	bgt.n	30005cac <BOOT_Image1+0x134>
30005cb8:	f3bf 8f4f 	dsb	sy
30005cbc:	f3bf 8f6f 	isb	sy
30005cc0:	4b44      	ldr	r3, [pc, #272]	; (30005dd4 <BOOT_Image1+0x25c>)
30005cc2:	2200      	movs	r2, #0
30005cc4:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30005cc8:	4b4a      	ldr	r3, [pc, #296]	; (30005df4 <BOOT_Image1+0x27c>)
30005cca:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30005cce:	f7fd fcf7 	bl	300036c0 <flash_highspeed_setup>
30005cd2:	47b0      	blx	r6
30005cd4:	2802      	cmp	r0, #2
30005cd6:	f000 82c4 	beq.w	30006262 <BOOT_Image1+0x6ea>
30005cda:	782b      	ldrb	r3, [r5, #0]
30005cdc:	2b00      	cmp	r3, #0
30005cde:	f040 8233 	bne.w	30006148 <BOOT_Image1+0x5d0>
30005ce2:	68a3      	ldr	r3, [r4, #8]
30005ce4:	079b      	lsls	r3, r3, #30
30005ce6:	f100 836a 	bmi.w	300063be <BOOT_Image1+0x846>
30005cea:	f7fe fc53 	bl	30004594 <BOOT_DDR_Init>
30005cee:	4a39      	ldr	r2, [pc, #228]	; (30005dd4 <BOOT_Image1+0x25c>)
30005cf0:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30005cf4:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005cf8:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005cfc:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30005d00:	4b34      	ldr	r3, [pc, #208]	; (30005dd4 <BOOT_Image1+0x25c>)
30005d02:	2500      	movs	r5, #0
30005d04:	f8c3 52d0 	str.w	r5, [r3, #720]	; 0x2d0
30005d08:	f8c3 52d4 	str.w	r5, [r3, #724]	; 0x2d4
30005d0c:	f8c3 52d8 	str.w	r5, [r3, #728]	; 0x2d8
30005d10:	f8c3 50d0 	str.w	r5, [r3, #208]	; 0xd0
30005d14:	f7ff fef2 	bl	30005afc <BOOT_Share_Memory_Patch>
30005d18:	f10d 0115 	add.w	r1, sp, #21
30005d1c:	f44f 7059 	mov.w	r0, #868	; 0x364
30005d20:	f002 ff44 	bl	30008bac <OTP_Read8>
30005d24:	f10d 0117 	add.w	r1, sp, #23
30005d28:	f44f 705a 	mov.w	r0, #872	; 0x368
30005d2c:	f88d 5016 	strb.w	r5, [sp, #22]
30005d30:	f88d 5017 	strb.w	r5, [sp, #23]
30005d34:	f002 ff3a 	bl	30008bac <OTP_Read8>
30005d38:	f10d 0116 	add.w	r1, sp, #22
30005d3c:	f240 3069 	movw	r0, #873	; 0x369
30005d40:	f002 ff34 	bl	30008bac <OTP_Read8>
30005d44:	f89d 3015 	ldrb.w	r3, [sp, #21]
30005d48:	f013 0f1e 	tst.w	r3, #30
30005d4c:	d10a      	bne.n	30005d64 <BOOT_Image1+0x1ec>
30005d4e:	f89d 2016 	ldrb.w	r2, [sp, #22]
30005d52:	f89d 3017 	ldrb.w	r3, [sp, #23]
30005d56:	f002 0208 	and.w	r2, r2, #8
30005d5a:	f003 0301 	and.w	r3, r3, #1
30005d5e:	4313      	orrs	r3, r2
30005d60:	f040 8299 	bne.w	30006296 <BOOT_Image1+0x71e>
30005d64:	4b31      	ldr	r3, [pc, #196]	; (30005e2c <BOOT_Image1+0x2b4>)
30005d66:	781b      	ldrb	r3, [r3, #0]
30005d68:	2b00      	cmp	r3, #0
30005d6a:	f040 82b5 	bne.w	300062d8 <BOOT_Image1+0x760>
30005d6e:	6823      	ldr	r3, [r4, #0]
30005d70:	f043 0302 	orr.w	r3, r3, #2
30005d74:	6023      	str	r3, [r4, #0]
30005d76:	4b2e      	ldr	r3, [pc, #184]	; (30005e30 <BOOT_Image1+0x2b8>)
30005d78:	4798      	blx	r3
30005d7a:	2801      	cmp	r0, #1
30005d7c:	f04f 0100 	mov.w	r1, #0
30005d80:	f04f 0002 	mov.w	r0, #2
30005d84:	bf0c      	ite	eq
30005d86:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
30005d8a:	4a2a      	ldrne	r2, [pc, #168]	; (30005e34 <BOOT_Image1+0x2bc>)
30005d8c:	f001 fed6 	bl	30007b3c <ymodem_uart_port_init>
30005d90:	4b27      	ldr	r3, [pc, #156]	; (30005e30 <BOOT_Image1+0x2b8>)
30005d92:	4798      	blx	r3
30005d94:	2800      	cmp	r0, #0
30005d96:	f000 825f 	beq.w	30006258 <BOOT_Image1+0x6e0>
30005d9a:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30005d9e:	4826      	ldr	r0, [pc, #152]	; (30005e38 <BOOT_Image1+0x2c0>)
30005da0:	f006 fc7e 	bl	3000c6a0 <__DiagPrintf_veneer>
30005da4:	4825      	ldr	r0, [pc, #148]	; (30005e3c <BOOT_Image1+0x2c4>)
30005da6:	2564      	movs	r5, #100	; 0x64
30005da8:	f006 fc22 	bl	3000c5f0 <____wrap_strlen_veneer>
30005dac:	4601      	mov	r1, r0
30005dae:	4823      	ldr	r0, [pc, #140]	; (30005e3c <BOOT_Image1+0x2c4>)
30005db0:	4c23      	ldr	r4, [pc, #140]	; (30005e40 <BOOT_Image1+0x2c8>)
30005db2:	f001 ff1d 	bl	30007bf0 <ymodem_uart_putdata>
30005db6:	e048      	b.n	30005e4a <BOOT_Image1+0x2d2>
30005db8:	0000c149 	.word	0x0000c149
30005dbc:	3000ce50 	.word	0x3000ce50
30005dc0:	3000e610 	.word	0x3000e610
30005dc4:	3000ce74 	.word	0x3000ce74
30005dc8:	3000fe70 	.word	0x3000fe70
30005dcc:	30010314 	.word	0x30010314
30005dd0:	00012be5 	.word	0x00012be5
30005dd4:	42008000 	.word	0x42008000
30005dd8:	42008200 	.word	0x42008200
30005ddc:	3000e8fa 	.word	0x3000e8fa
30005de0:	3000b5a9 	.word	0x3000b5a9
30005de4:	3000b961 	.word	0x3000b961
30005de8:	3000b881 	.word	0x3000b881
30005dec:	30007ded 	.word	0x30007ded
30005df0:	00009b2d 	.word	0x00009b2d
30005df4:	23020000 	.word	0x23020000
30005df8:	40080000 	.word	0x40080000
30005dfc:	0000b479 	.word	0x0000b479
30005e00:	40040000 	.word	0x40040000
30005e04:	30009ebd 	.word	0x30009ebd
30005e08:	3000e8f9 	.word	0x3000e8f9
30005e0c:	0000aab9 	.word	0x0000aab9
30005e10:	4200c000 	.word	0x4200c000
30005e14:	2001c00c 	.word	0x2001c00c
30005e18:	30005b79 	.word	0x30005b79
30005e1c:	00033000 	.word	0x00033000
30005e20:	2001c01c 	.word	0x2001c01c
30005e24:	3000e8fc 	.word	0x3000e8fc
30005e28:	e000ed00 	.word	0xe000ed00
30005e2c:	3000e8f8 	.word	0x3000e8f8
30005e30:	0000c169 	.word	0x0000c169
30005e34:	0016e360 	.word	0x0016e360
30005e38:	3000cfd8 	.word	0x3000cfd8
30005e3c:	3000eee8 	.word	0x3000eee8
30005e40:	00009be5 	.word	0x00009be5
30005e44:	3d01      	subs	r5, #1
30005e46:	f000 82be 	beq.w	300063c6 <BOOT_Image1+0x84e>
30005e4a:	2001      	movs	r0, #1
30005e4c:	47a0      	blx	r4
30005e4e:	f001 feb5 	bl	30007bbc <ymodem_uart_readable>
30005e52:	2800      	cmp	r0, #0
30005e54:	d0f6      	beq.n	30005e44 <BOOT_Image1+0x2cc>
30005e56:	f001 feb1 	bl	30007bbc <ymodem_uart_readable>
30005e5a:	2800      	cmp	r0, #0
30005e5c:	f000 82b9 	beq.w	300063d2 <BOOT_Image1+0x85a>
30005e60:	2301      	movs	r3, #1
30005e62:	9303      	str	r3, [sp, #12]
30005e64:	2001      	movs	r0, #1
30005e66:	47a0      	blx	r4
30005e68:	2108      	movs	r1, #8
30005e6a:	48c0      	ldr	r0, [pc, #768]	; (3000616c <BOOT_Image1+0x5f4>)
30005e6c:	f001 fece 	bl	30007c0c <ymodem_uart_getdata>
30005e70:	49be      	ldr	r1, [pc, #760]	; (3000616c <BOOT_Image1+0x5f4>)
30005e72:	2208      	movs	r2, #8
30005e74:	4bbe      	ldr	r3, [pc, #760]	; (30006170 <BOOT_Image1+0x5f8>)
30005e76:	f101 0080 	add.w	r0, r1, #128	; 0x80
30005e7a:	4798      	blx	r3
30005e7c:	4605      	mov	r5, r0
30005e7e:	2800      	cmp	r0, #0
30005e80:	f040 8368 	bne.w	30006554 <BOOT_Image1+0x9dc>
30005e84:	f001 fe9a 	bl	30007bbc <ymodem_uart_readable>
30005e88:	2800      	cmp	r0, #0
30005e8a:	d0fb      	beq.n	30005e84 <BOOT_Image1+0x30c>
30005e8c:	2101      	movs	r1, #1
30005e8e:	48b9      	ldr	r0, [pc, #740]	; (30006174 <BOOT_Image1+0x5fc>)
30005e90:	f001 febc 	bl	30007c0c <ymodem_uart_getdata>
30005e94:	f001 fe92 	bl	30007bbc <ymodem_uart_readable>
30005e98:	2800      	cmp	r0, #0
30005e9a:	d0fb      	beq.n	30005e94 <BOOT_Image1+0x31c>
30005e9c:	4fb6      	ldr	r7, [pc, #728]	; (30006178 <BOOT_Image1+0x600>)
30005e9e:	2101      	movs	r1, #1
30005ea0:	f207 4051 	addw	r0, r7, #1105	; 0x451
30005ea4:	f001 feb2 	bl	30007c0c <ymodem_uart_getdata>
30005ea8:	f897 2451 	ldrb.w	r2, [r7, #1105]	; 0x451
30005eac:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005eb0:	429a      	cmp	r2, r3
30005eb2:	f000 8360 	beq.w	30006576 <BOOT_Image1+0x9fe>
30005eb6:	f207 4952 	addw	r9, r7, #1106	; 0x452
30005eba:	2600      	movs	r6, #0
30005ebc:	46b0      	mov	r8, r6
30005ebe:	3601      	adds	r6, #1
30005ec0:	f001 fe7c 	bl	30007bbc <ymodem_uart_readable>
30005ec4:	2800      	cmp	r0, #0
30005ec6:	d0fb      	beq.n	30005ec0 <BOOT_Image1+0x348>
30005ec8:	4648      	mov	r0, r9
30005eca:	2101      	movs	r1, #1
30005ecc:	f001 fe9e 	bl	30007c0c <ymodem_uart_getdata>
30005ed0:	f899 2000 	ldrb.w	r2, [r9]
30005ed4:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005ed8:	f108 080b 	add.w	r8, r8, #11
30005edc:	f109 0901 	add.w	r9, r9, #1
30005ee0:	46b3      	mov	fp, r6
30005ee2:	429a      	cmp	r2, r3
30005ee4:	d1ea      	bne.n	30005ebc <BOOT_Image1+0x344>
30005ee6:	2e00      	cmp	r6, #0
30005ee8:	dd13      	ble.n	30005f12 <BOOT_Image1+0x39a>
30005eea:	f507 618a 	add.w	r1, r7, #1104	; 0x450
30005eee:	2300      	movs	r3, #0
30005ef0:	1988      	adds	r0, r1, r6
30005ef2:	e001      	b.n	30005ef8 <BOOT_Image1+0x380>
30005ef4:	4288      	cmp	r0, r1
30005ef6:	d00d      	beq.n	30005f14 <BOOT_Image1+0x39c>
30005ef8:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30005efc:	eb03 0383 	add.w	r3, r3, r3, lsl #2
30005f00:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
30005f04:	eb02 0343 	add.w	r3, r2, r3, lsl #1
30005f08:	f1bc 0f09 	cmp.w	ip, #9
30005f0c:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
30005f10:	d9f0      	bls.n	30005ef4 <BOOT_Image1+0x37c>
30005f12:	2300      	movs	r3, #0
30005f14:	f8c7 34d4 	str.w	r3, [r7, #1236]	; 0x4d4
30005f18:	f001 fe50 	bl	30007bbc <ymodem_uart_readable>
30005f1c:	2800      	cmp	r0, #0
30005f1e:	d0fb      	beq.n	30005f18 <BOOT_Image1+0x3a0>
30005f20:	4992      	ldr	r1, [pc, #584]	; (3000616c <BOOT_Image1+0x5f4>)
30005f22:	460b      	mov	r3, r1
30005f24:	2101      	movs	r1, #1
30005f26:	eb03 0008 	add.w	r0, r3, r8
30005f2a:	9302      	str	r3, [sp, #8]
30005f2c:	f001 fe6e 	bl	30007c0c <ymodem_uart_getdata>
30005f30:	9902      	ldr	r1, [sp, #8]
30005f32:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005f36:	f811 2008 	ldrb.w	r2, [r1, r8]
30005f3a:	429a      	cmp	r2, r3
30005f3c:	f000 8317 	beq.w	3000656e <BOOT_Image1+0x9f6>
30005f40:	f10b 080b 	add.w	r8, fp, #11
30005f44:	f04f 0900 	mov.w	r9, #0
30005f48:	4488      	add	r8, r1
30005f4a:	f109 0901 	add.w	r9, r9, #1
30005f4e:	f001 fe35 	bl	30007bbc <ymodem_uart_readable>
30005f52:	2800      	cmp	r0, #0
30005f54:	d0fb      	beq.n	30005f4e <BOOT_Image1+0x3d6>
30005f56:	4640      	mov	r0, r8
30005f58:	2101      	movs	r1, #1
30005f5a:	f001 fe57 	bl	30007c0c <ymodem_uart_getdata>
30005f5e:	f898 2000 	ldrb.w	r2, [r8]
30005f62:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005f66:	f108 0801 	add.w	r8, r8, #1
30005f6a:	eb09 0a0b 	add.w	sl, r9, fp
30005f6e:	429a      	cmp	r2, r3
30005f70:	d1eb      	bne.n	30005f4a <BOOT_Image1+0x3d2>
30005f72:	f001 fe23 	bl	30007bbc <ymodem_uart_readable>
30005f76:	2800      	cmp	r0, #0
30005f78:	d0fb      	beq.n	30005f72 <BOOT_Image1+0x3fa>
30005f7a:	9b02      	ldr	r3, [sp, #8]
30005f7c:	f10a 080b 	add.w	r8, sl, #11
30005f80:	2101      	movs	r1, #1
30005f82:	eb03 0008 	add.w	r0, r3, r8
30005f86:	f001 fe41 	bl	30007c0c <ymodem_uart_getdata>
30005f8a:	9902      	ldr	r1, [sp, #8]
30005f8c:	f897 24d1 	ldrb.w	r2, [r7, #1233]	; 0x4d1
30005f90:	f811 3008 	ldrb.w	r3, [r1, r8]
30005f94:	429a      	cmp	r2, r3
30005f96:	f000 82e8 	beq.w	3000656a <BOOT_Image1+0x9f2>
30005f9a:	f10a 080c 	add.w	r8, sl, #12
30005f9e:	4488      	add	r8, r1
30005fa0:	3501      	adds	r5, #1
30005fa2:	f001 fe0b 	bl	30007bbc <ymodem_uart_readable>
30005fa6:	2800      	cmp	r0, #0
30005fa8:	d0fb      	beq.n	30005fa2 <BOOT_Image1+0x42a>
30005faa:	4640      	mov	r0, r8
30005fac:	2101      	movs	r1, #1
30005fae:	f001 fe2d 	bl	30007c0c <ymodem_uart_getdata>
30005fb2:	f898 2000 	ldrb.w	r2, [r8]
30005fb6:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005fba:	f108 0801 	add.w	r8, r8, #1
30005fbe:	eb05 0b0a 	add.w	fp, r5, sl
30005fc2:	429a      	cmp	r2, r3
30005fc4:	d1ec      	bne.n	30005fa0 <BOOT_Image1+0x428>
30005fc6:	9b02      	ldr	r3, [sp, #8]
30005fc8:	f10b 080c 	add.w	r8, fp, #12
30005fcc:	2101      	movs	r1, #1
30005fce:	eb03 0008 	add.w	r0, r3, r8
30005fd2:	f001 fe1b 	bl	30007c0c <ymodem_uart_getdata>
30005fd6:	9b02      	ldr	r3, [sp, #8]
30005fd8:	f813 1008 	ldrb.w	r1, [r3, r8]
30005fdc:	290a      	cmp	r1, #10
30005fde:	f8c7 14d8 	str.w	r1, [r7, #1240]	; 0x4d8
30005fe2:	f200 8266 	bhi.w	300064b2 <BOOT_Image1+0x93a>
30005fe6:	9b02      	ldr	r3, [sp, #8]
30005fe8:	f10b 000d 	add.w	r0, fp, #13
30005fec:	2101      	movs	r1, #1
30005fee:	f10b 080e 	add.w	r8, fp, #14
30005ff2:	4418      	add	r0, r3
30005ff4:	f001 fe0a 	bl	30007c0c <ymodem_uart_getdata>
30005ff8:	9b02      	ldr	r3, [sp, #8]
30005ffa:	2102      	movs	r1, #2
30005ffc:	4498      	add	r8, r3
30005ffe:	4640      	mov	r0, r8
30006000:	f001 fe04 	bl	30007c0c <ymodem_uart_getdata>
30006004:	4641      	mov	r1, r8
30006006:	2202      	movs	r2, #2
30006008:	485c      	ldr	r0, [pc, #368]	; (3000617c <BOOT_Image1+0x604>)
3000600a:	4b59      	ldr	r3, [pc, #356]	; (30006170 <BOOT_Image1+0x5f8>)
3000600c:	4798      	blx	r3
3000600e:	2800      	cmp	r0, #0
30006010:	f040 82a7 	bne.w	30006562 <BOOT_Image1+0x9ea>
30006014:	9b02      	ldr	r3, [sp, #8]
30006016:	f10b 0010 	add.w	r0, fp, #16
3000601a:	2101      	movs	r1, #1
3000601c:	f10b 0811 	add.w	r8, fp, #17
30006020:	4418      	add	r0, r3
30006022:	f001 fdf3 	bl	30007c0c <ymodem_uart_getdata>
30006026:	4b56      	ldr	r3, [pc, #344]	; (30006180 <BOOT_Image1+0x608>)
30006028:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
3000602c:	f103 0228 	add.w	r2, r3, #40	; 0x28
30006030:	f103 0130 	add.w	r1, r3, #48	; 0x30
30006034:	f7fd fd44 	bl	30003ac0 <hotfix_get_addr_size>
30006038:	9b02      	ldr	r3, [sp, #8]
3000603a:	2102      	movs	r1, #2
3000603c:	4498      	add	r8, r3
3000603e:	4640      	mov	r0, r8
30006040:	f001 fde4 	bl	30007c0c <ymodem_uart_getdata>
30006044:	4641      	mov	r1, r8
30006046:	2202      	movs	r2, #2
30006048:	484e      	ldr	r0, [pc, #312]	; (30006184 <BOOT_Image1+0x60c>)
3000604a:	4b49      	ldr	r3, [pc, #292]	; (30006170 <BOOT_Image1+0x5f8>)
3000604c:	4798      	blx	r3
3000604e:	2800      	cmp	r0, #0
30006050:	f000 8153 	beq.w	300062fa <BOOT_Image1+0x782>
30006054:	9b03      	ldr	r3, [sp, #12]
30006056:	2b00      	cmp	r3, #0
30006058:	f000 819a 	beq.w	30006390 <BOOT_Image1+0x818>
3000605c:	f006 f8b4 	bl	3000c1c8 <BOOT_VerCheck>
30006060:	f7fe fc96 	bl	30004990 <BOOT_LoadImages>
30006064:	2800      	cmp	r0, #0
30006066:	f000 8141 	beq.w	300062ec <BOOT_Image1+0x774>
3000606a:	f7ff fa19 	bl	300054a0 <BOOT_Enable_KM0>
3000606e:	2200      	movs	r2, #0
30006070:	2101      	movs	r1, #1
30006072:	4b45      	ldr	r3, [pc, #276]	; (30006188 <BOOT_Image1+0x610>)
30006074:	4845      	ldr	r0, [pc, #276]	; (3000618c <BOOT_Image1+0x614>)
30006076:	4798      	blx	r3
30006078:	4b45      	ldr	r3, [pc, #276]	; (30006190 <BOOT_Image1+0x618>)
3000607a:	2008      	movs	r0, #8
3000607c:	4798      	blx	r3
3000607e:	f001 fad3 	bl	30007628 <BOOT_RAM_TZCfg>
30006082:	4b44      	ldr	r3, [pc, #272]	; (30006194 <BOOT_Image1+0x61c>)
30006084:	781b      	ldrb	r3, [r3, #0]
30006086:	2b01      	cmp	r3, #1
30006088:	f000 8203 	beq.w	30006492 <BOOT_Image1+0x91a>
3000608c:	4b42      	ldr	r3, [pc, #264]	; (30006198 <BOOT_Image1+0x620>)
3000608e:	781b      	ldrb	r3, [r3, #0]
30006090:	2b00      	cmp	r3, #0
30006092:	f000 8184 	beq.w	3000639e <BOOT_Image1+0x826>
30006096:	f7ff fb33 	bl	30005700 <BOOT_Enable_AP>
3000609a:	4a40      	ldr	r2, [pc, #256]	; (3000619c <BOOT_Image1+0x624>)
3000609c:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
300060a0:	f043 0306 	orr.w	r3, r3, #6
300060a4:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
300060a8:	4b3a      	ldr	r3, [pc, #232]	; (30006194 <BOOT_Image1+0x61c>)
300060aa:	781b      	ldrb	r3, [r3, #0]
300060ac:	2b01      	cmp	r3, #1
300060ae:	f000 81f8 	beq.w	300064a2 <BOOT_Image1+0x92a>
300060b2:	483a      	ldr	r0, [pc, #232]	; (3000619c <BOOT_Image1+0x624>)
300060b4:	493a      	ldr	r1, [pc, #232]	; (300061a0 <BOOT_Image1+0x628>)
300060b6:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
300060ba:	4a3a      	ldr	r2, [pc, #232]	; (300061a4 <BOOT_Image1+0x62c>)
300060bc:	f043 0301 	orr.w	r3, r3, #1
300060c0:	4d39      	ldr	r5, [pc, #228]	; (300061a8 <BOOT_Image1+0x630>)
300060c2:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
300060c6:	688c      	ldr	r4, [r1, #8]
300060c8:	680b      	ldr	r3, [r1, #0]
300060ca:	4938      	ldr	r1, [pc, #224]	; (300061ac <BOOT_Image1+0x634>)
300060cc:	6063      	str	r3, [r4, #4]
300060ce:	4b38      	ldr	r3, [pc, #224]	; (300061b0 <BOOT_Image1+0x638>)
300060d0:	6094      	str	r4, [r2, #8]
300060d2:	61e9      	str	r1, [r5, #28]
300060d4:	6a59      	ldr	r1, [r3, #36]	; 0x24
300060d6:	4837      	ldr	r0, [pc, #220]	; (300061b4 <BOOT_Image1+0x63c>)
300060d8:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300060dc:	6259      	str	r1, [r3, #36]	; 0x24
300060de:	6a51      	ldr	r1, [r2, #36]	; 0x24
300060e0:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
300060e4:	6251      	str	r1, [r2, #36]	; 0x24
300060e6:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
300060ea:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300060ee:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
300060f2:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
300060f6:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300060fa:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300060fe:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30006102:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30006106:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000610a:	689b      	ldr	r3, [r3, #8]
3000610c:	6183      	str	r3, [r0, #24]
3000610e:	f7fe fbc7 	bl	300048a0 <BOOT_NVICBackup_HP>
30006112:	4b29      	ldr	r3, [pc, #164]	; (300061b8 <BOOT_Image1+0x640>)
30006114:	f383 8888 	msr	MSP_NS, r3
30006118:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
3000611c:	f383 8809 	msr	PSP, r3
30006120:	6860      	ldr	r0, [r4, #4]
30006122:	f7fd ffb5 	bl	30004090 <BOOT_NsStart>
30006126:	b009      	add	sp, #36	; 0x24
30006128:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000612c:	4b23      	ldr	r3, [pc, #140]	; (300061bc <BOOT_Image1+0x644>)
3000612e:	2001      	movs	r0, #1
30006130:	4798      	blx	r3
30006132:	4620      	mov	r0, r4
30006134:	4b22      	ldr	r3, [pc, #136]	; (300061c0 <BOOT_Image1+0x648>)
30006136:	4798      	blx	r3
30006138:	4b22      	ldr	r3, [pc, #136]	; (300061c4 <BOOT_Image1+0x64c>)
3000613a:	2001      	movs	r0, #1
3000613c:	4798      	blx	r3
3000613e:	e549      	b.n	30005bd4 <BOOT_Image1+0x5c>
30006140:	4821      	ldr	r0, [pc, #132]	; (300061c8 <BOOT_Image1+0x650>)
30006142:	f006 faad 	bl	3000c6a0 <__DiagPrintf_veneer>
30006146:	e522      	b.n	30005b8e <BOOT_Image1+0x16>
30006148:	4c20      	ldr	r4, [pc, #128]	; (300061cc <BOOT_Image1+0x654>)
3000614a:	4e21      	ldr	r6, [pc, #132]	; (300061d0 <BOOT_Image1+0x658>)
3000614c:	4d21      	ldr	r5, [pc, #132]	; (300061d4 <BOOT_Image1+0x65c>)
3000614e:	4633      	mov	r3, r6
30006150:	462a      	mov	r2, r5
30006152:	491f      	ldr	r1, [pc, #124]	; (300061d0 <BOOT_Image1+0x658>)
30006154:	2002      	movs	r0, #2
30006156:	f005 fffb 	bl	3000c150 <rtk_log_write>
3000615a:	f241 3088 	movw	r0, #5000	; 0x1388
3000615e:	47a0      	blx	r4
30006160:	e7f5      	b.n	3000614e <BOOT_Image1+0x5d6>
30006162:	3174      	adds	r1, #116	; 0x74
30006164:	f027 031f 	bic.w	r3, r7, #31
30006168:	e59c      	b.n	30005ca4 <BOOT_Image1+0x12c>
3000616a:	bf00      	nop
3000616c:	3000ef08 	.word	0x3000ef08
30006170:	000129bd 	.word	0x000129bd
30006174:	3000ef10 	.word	0x3000ef10
30006178:	3000eac0 	.word	0x3000eac0
3000617c:	3000ef9c 	.word	0x3000ef9c
30006180:	3000efa0 	.word	0x3000efa0
30006184:	3000efdc 	.word	0x3000efdc
30006188:	0000ac6d 	.word	0x0000ac6d
3000618c:	4200c000 	.word	0x4200c000
30006190:	0000e1d9 	.word	0x0000e1d9
30006194:	3000e8f8 	.word	0x3000e8f8
30006198:	3000e8fb 	.word	0x3000e8fb
3000619c:	42008200 	.word	0x42008200
300061a0:	60000020 	.word	0x60000020
300061a4:	e002ed00 	.word	0xe002ed00
300061a8:	30000000 	.word	0x30000000
300061ac:	30003a81 	.word	0x30003a81
300061b0:	e000ed00 	.word	0xe000ed00
300061b4:	3000ff1c 	.word	0x3000ff1c
300061b8:	2001bffc 	.word	0x2001bffc
300061bc:	3000b881 	.word	0x3000b881
300061c0:	3000bab1 	.word	0x3000bab1
300061c4:	3000ba21 	.word	0x3000ba21
300061c8:	3000ce60 	.word	0x3000ce60
300061cc:	00009be5 	.word	0x00009be5
300061d0:	3000c810 	.word	0x3000c810
300061d4:	3000cf18 	.word	0x3000cf18
300061d8:	6de3      	ldr	r3, [r4, #92]	; 0x5c
300061da:	2201      	movs	r2, #1
300061dc:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
300061e0:	f423 7398 	bic.w	r3, r3, #304	; 0x130
300061e4:	65e3      	str	r3, [r4, #92]	; 0x5c
300061e6:	4bb5      	ldr	r3, [pc, #724]	; (300064bc <BOOT_Image1+0x944>)
300061e8:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
300061ec:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
300061f0:	4bb3      	ldr	r3, [pc, #716]	; (300064c0 <BOOT_Image1+0x948>)
300061f2:	4798      	blx	r3
300061f4:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
300061f8:	f043 0302 	orr.w	r3, r3, #2
300061fc:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
30006200:	e516      	b.n	30005c30 <BOOT_Image1+0xb8>
30006202:	48b0      	ldr	r0, [pc, #704]	; (300064c4 <BOOT_Image1+0x94c>)
30006204:	f006 fa4c 	bl	3000c6a0 <__DiagPrintf_veneer>
30006208:	e536      	b.n	30005c78 <BOOT_Image1+0x100>
3000620a:	4601      	mov	r1, r0
3000620c:	f44f 7201 	mov.w	r2, #516	; 0x204
30006210:	48aa      	ldr	r0, [pc, #680]	; (300064bc <BOOT_Image1+0x944>)
30006212:	f006 f9e9 	bl	3000c5e8 <____wrap_memset_veneer>
30006216:	e4d0      	b.n	30005bba <BOOT_Image1+0x42>
30006218:	f3ef 8108 	mrs	r1, MSP
3000621c:	48aa      	ldr	r0, [pc, #680]	; (300064c8 <BOOT_Image1+0x950>)
3000621e:	f006 fa3f 	bl	3000c6a0 <__DiagPrintf_veneer>
30006222:	e520      	b.n	30005c66 <BOOT_Image1+0xee>
30006224:	4fa9      	ldr	r7, [pc, #676]	; (300064cc <BOOT_Image1+0x954>)
30006226:	47b8      	blx	r7
30006228:	4604      	mov	r4, r0
3000622a:	47b8      	blx	r7
3000622c:	2c02      	cmp	r4, #2
3000622e:	f43f ad10 	beq.w	30005c52 <BOOT_Image1+0xda>
30006232:	2803      	cmp	r0, #3
30006234:	f43f ad0d 	beq.w	30005c52 <BOOT_Image1+0xda>
30006238:	4ba5      	ldr	r3, [pc, #660]	; (300064d0 <BOOT_Image1+0x958>)
3000623a:	4798      	blx	r3
3000623c:	4ca5      	ldr	r4, [pc, #660]	; (300064d4 <BOOT_Image1+0x95c>)
3000623e:	2200      	movs	r2, #0
30006240:	2101      	movs	r1, #1
30006242:	48a5      	ldr	r0, [pc, #660]	; (300064d8 <BOOT_Image1+0x960>)
30006244:	47a0      	blx	r4
30006246:	2101      	movs	r1, #1
30006248:	48a3      	ldr	r0, [pc, #652]	; (300064d8 <BOOT_Image1+0x960>)
3000624a:	4ba4      	ldr	r3, [pc, #656]	; (300064dc <BOOT_Image1+0x964>)
3000624c:	4798      	blx	r3
3000624e:	2201      	movs	r2, #1
30006250:	48a1      	ldr	r0, [pc, #644]	; (300064d8 <BOOT_Image1+0x960>)
30006252:	4611      	mov	r1, r2
30006254:	47a0      	blx	r4
30006256:	e4fd      	b.n	30005c54 <BOOT_Image1+0xdc>
30006258:	49a1      	ldr	r1, [pc, #644]	; (300064e0 <BOOT_Image1+0x968>)
3000625a:	48a2      	ldr	r0, [pc, #648]	; (300064e4 <BOOT_Image1+0x96c>)
3000625c:	f006 fa20 	bl	3000c6a0 <__DiagPrintf_veneer>
30006260:	e5a0      	b.n	30005da4 <BOOT_Image1+0x22c>
30006262:	4ba1      	ldr	r3, [pc, #644]	; (300064e8 <BOOT_Image1+0x970>)
30006264:	2004      	movs	r0, #4
30006266:	4aa1      	ldr	r2, [pc, #644]	; (300064ec <BOOT_Image1+0x974>)
30006268:	4619      	mov	r1, r3
3000626a:	f005 ff71 	bl	3000c150 <rtk_log_write>
3000626e:	4ba0      	ldr	r3, [pc, #640]	; (300064f0 <BOOT_Image1+0x978>)
30006270:	4798      	blx	r3
30006272:	f7fe f9cd 	bl	30004610 <BOOT_PSRAM_Init>
30006276:	7828      	ldrb	r0, [r5, #0]
30006278:	2800      	cmp	r0, #0
3000627a:	f47f ad41 	bne.w	30005d00 <BOOT_Image1+0x188>
3000627e:	4b9d      	ldr	r3, [pc, #628]	; (300064f4 <BOOT_Image1+0x97c>)
30006280:	4798      	blx	r3
30006282:	2800      	cmp	r0, #0
30006284:	f43f ad3c 	beq.w	30005d00 <BOOT_Image1+0x188>
30006288:	4b97      	ldr	r3, [pc, #604]	; (300064e8 <BOOT_Image1+0x970>)
3000628a:	2003      	movs	r0, #3
3000628c:	4a9a      	ldr	r2, [pc, #616]	; (300064f8 <BOOT_Image1+0x980>)
3000628e:	4619      	mov	r1, r3
30006290:	f005 ff5e 	bl	3000c150 <rtk_log_write>
30006294:	e534      	b.n	30005d00 <BOOT_Image1+0x188>
30006296:	4899      	ldr	r0, [pc, #612]	; (300064fc <BOOT_Image1+0x984>)
30006298:	f006 fa02 	bl	3000c6a0 <__DiagPrintf_veneer>
3000629c:	f89d 1017 	ldrb.w	r1, [sp, #23]
300062a0:	f89d 3016 	ldrb.w	r3, [sp, #22]
300062a4:	f44f 705a 	mov.w	r0, #872	; 0x368
300062a8:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
300062ac:	f023 0308 	bic.w	r3, r3, #8
300062b0:	f88d 1017 	strb.w	r1, [sp, #23]
300062b4:	f88d 3016 	strb.w	r3, [sp, #22]
300062b8:	f002 fc82 	bl	30008bc0 <OTP_Write8>
300062bc:	f89d 1016 	ldrb.w	r1, [sp, #22]
300062c0:	f240 3069 	movw	r0, #873	; 0x369
300062c4:	f002 fc7c 	bl	30008bc0 <OTP_Write8>
300062c8:	488d      	ldr	r0, [pc, #564]	; (30006500 <BOOT_Image1+0x988>)
300062ca:	f006 f9e9 	bl	3000c6a0 <__DiagPrintf_veneer>
300062ce:	4b8d      	ldr	r3, [pc, #564]	; (30006504 <BOOT_Image1+0x98c>)
300062d0:	781b      	ldrb	r3, [r3, #0]
300062d2:	2b00      	cmp	r3, #0
300062d4:	f43f ad4b 	beq.w	30005d6e <BOOT_Image1+0x1f6>
300062d8:	4b8b      	ldr	r3, [pc, #556]	; (30006508 <BOOT_Image1+0x990>)
300062da:	4798      	blx	r3
300062dc:	2800      	cmp	r0, #0
300062de:	f47f ad46 	bne.w	30005d6e <BOOT_Image1+0x1f6>
300062e2:	68a3      	ldr	r3, [r4, #8]
300062e4:	f043 0302 	orr.w	r3, r3, #2
300062e8:	60a3      	str	r3, [r4, #8]
300062ea:	e540      	b.n	30005d6e <BOOT_Image1+0x1f6>
300062ec:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300062f0:	47a0      	blx	r4
300062f2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300062f6:	47a0      	blx	r4
300062f8:	e7f8      	b.n	300062ec <BOOT_Image1+0x774>
300062fa:	9b02      	ldr	r3, [sp, #8]
300062fc:	f10b 0013 	add.w	r0, fp, #19
30006300:	2101      	movs	r1, #1
30006302:	f10b 0814 	add.w	r8, fp, #20
30006306:	4418      	add	r0, r3
30006308:	f001 fc80 	bl	30007c0c <ymodem_uart_getdata>
3000630c:	4b7f      	ldr	r3, [pc, #508]	; (3000650c <BOOT_Image1+0x994>)
3000630e:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
30006312:	f1a3 0218 	sub.w	r2, r3, #24
30006316:	f1a3 0110 	sub.w	r1, r3, #16
3000631a:	f7fd fbd1 	bl	30003ac0 <hotfix_get_addr_size>
3000631e:	9b02      	ldr	r3, [sp, #8]
30006320:	2102      	movs	r1, #2
30006322:	4498      	add	r8, r3
30006324:	4640      	mov	r0, r8
30006326:	f001 fc71 	bl	30007c0c <ymodem_uart_getdata>
3000632a:	4641      	mov	r1, r8
3000632c:	2202      	movs	r2, #2
3000632e:	4878      	ldr	r0, [pc, #480]	; (30006510 <BOOT_Image1+0x998>)
30006330:	4b78      	ldr	r3, [pc, #480]	; (30006514 <BOOT_Image1+0x99c>)
30006332:	4798      	blx	r3
30006334:	2800      	cmp	r0, #0
30006336:	f47f ae8d 	bne.w	30006054 <BOOT_Image1+0x4dc>
3000633a:	9b02      	ldr	r3, [sp, #8]
3000633c:	f10b 0016 	add.w	r0, fp, #22
30006340:	2101      	movs	r1, #1
30006342:	f10b 0817 	add.w	r8, fp, #23
30006346:	4418      	add	r0, r3
30006348:	f001 fc60 	bl	30007c0c <ymodem_uart_getdata>
3000634c:	4b72      	ldr	r3, [pc, #456]	; (30006518 <BOOT_Image1+0x9a0>)
3000634e:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
30006352:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
30006356:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
3000635a:	f7fd fbb1 	bl	30003ac0 <hotfix_get_addr_size>
3000635e:	9a02      	ldr	r2, [sp, #8]
30006360:	2102      	movs	r1, #2
30006362:	4490      	add	r8, r2
30006364:	4640      	mov	r0, r8
30006366:	f001 fc51 	bl	30007c0c <ymodem_uart_getdata>
3000636a:	4641      	mov	r1, r8
3000636c:	2202      	movs	r2, #2
3000636e:	486b      	ldr	r0, [pc, #428]	; (3000651c <BOOT_Image1+0x9a4>)
30006370:	4b68      	ldr	r3, [pc, #416]	; (30006514 <BOOT_Image1+0x99c>)
30006372:	4798      	blx	r3
30006374:	2800      	cmp	r0, #0
30006376:	f47f ae6d 	bne.w	30006054 <BOOT_Image1+0x4dc>
3000637a:	9b02      	ldr	r3, [sp, #8]
3000637c:	f10b 0019 	add.w	r0, fp, #25
30006380:	2101      	movs	r1, #1
30006382:	4418      	add	r0, r3
30006384:	f001 fc42 	bl	30007c0c <ymodem_uart_getdata>
30006388:	9b03      	ldr	r3, [sp, #12]
3000638a:	2b00      	cmp	r3, #0
3000638c:	f040 80f9 	bne.w	30006582 <BOOT_Image1+0xa0a>
30006390:	2002      	movs	r0, #2
30006392:	f001 fbeb 	bl	30007b6c <ymodem_uart_port_deinit>
30006396:	4862      	ldr	r0, [pc, #392]	; (30006520 <BOOT_Image1+0x9a8>)
30006398:	f006 f982 	bl	3000c6a0 <__DiagPrintf_veneer>
3000639c:	e65e      	b.n	3000605c <BOOT_Image1+0x4e4>
3000639e:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300063a2:	685a      	ldr	r2, [r3, #4]
300063a4:	f042 0202 	orr.w	r2, r2, #2
300063a8:	605a      	str	r2, [r3, #4]
300063aa:	685a      	ldr	r2, [r3, #4]
300063ac:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300063b0:	605a      	str	r2, [r3, #4]
300063b2:	4a5c      	ldr	r2, [pc, #368]	; (30006524 <BOOT_Image1+0x9ac>)
300063b4:	6f93      	ldr	r3, [r2, #120]	; 0x78
300063b6:	f043 0303 	orr.w	r3, r3, #3
300063ba:	6793      	str	r3, [r2, #120]	; 0x78
300063bc:	e674      	b.n	300060a8 <BOOT_Image1+0x530>
300063be:	485a      	ldr	r0, [pc, #360]	; (30006528 <BOOT_Image1+0x9b0>)
300063c0:	f006 f96e 	bl	3000c6a0 <__DiagPrintf_veneer>
300063c4:	e491      	b.n	30005cea <BOOT_Image1+0x172>
300063c6:	f001 fbf9 	bl	30007bbc <ymodem_uart_readable>
300063ca:	2800      	cmp	r0, #0
300063cc:	d0e0      	beq.n	30006390 <BOOT_Image1+0x818>
300063ce:	9503      	str	r5, [sp, #12]
300063d0:	e548      	b.n	30005e64 <BOOT_Image1+0x2ec>
300063d2:	4f56      	ldr	r7, [pc, #344]	; (3000652c <BOOT_Image1+0x9b4>)
300063d4:	4605      	mov	r5, r0
300063d6:	f04f 080b 	mov.w	r8, #11
300063da:	260a      	movs	r6, #10
300063dc:	f507 6389 	add.w	r3, r7, #1096	; 0x448
300063e0:	4681      	mov	r9, r0
300063e2:	9302      	str	r3, [sp, #8]
300063e4:	2004      	movs	r0, #4
300063e6:	f001 fbc1 	bl	30007b6c <ymodem_uart_port_deinit>
300063ea:	9a02      	ldr	r2, [sp, #8]
300063ec:	9500      	str	r5, [sp, #0]
300063ee:	4613      	mov	r3, r2
300063f0:	1990      	adds	r0, r2, r6
300063f2:	f8d7 14d4 	ldr.w	r1, [r7, #1236]	; 0x4d4
300063f6:	464a      	mov	r2, r9
300063f8:	4443      	add	r3, r8
300063fa:	f7ff fad9 	bl	300059b0 <password_hash_check>
300063fe:	4680      	mov	r8, r0
30006400:	2800      	cmp	r0, #0
30006402:	d1c5      	bne.n	30006390 <BOOT_Image1+0x818>
30006404:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30006408:	47a0      	blx	r4
3000640a:	4849      	ldr	r0, [pc, #292]	; (30006530 <BOOT_Image1+0x9b8>)
3000640c:	f006 f8f0 	bl	3000c5f0 <____wrap_strlen_veneer>
30006410:	4601      	mov	r1, r0
30006412:	4847      	ldr	r0, [pc, #284]	; (30006530 <BOOT_Image1+0x9b8>)
30006414:	f001 fbec 	bl	30007bf0 <ymodem_uart_putdata>
30006418:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000641c:	47a0      	blx	r4
3000641e:	f8d7 24d8 	ldr.w	r2, [r7, #1240]	; 0x4d8
30006422:	4944      	ldr	r1, [pc, #272]	; (30006534 <BOOT_Image1+0x9bc>)
30006424:	2002      	movs	r0, #2
30006426:	f001 fc83 	bl	30007d30 <ymodem_img_download>
3000642a:	2032      	movs	r0, #50	; 0x32
3000642c:	47a0      	blx	r4
3000642e:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
30006432:	b1e3      	cbz	r3, 3000646e <BOOT_Image1+0x8f6>
30006434:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30006518 <BOOT_Image1+0x9a0>
30006438:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
3000643c:	f859 6b04 	ldr.w	r6, [r9], #4
30006440:	0b36      	lsrs	r6, r6, #12
30006442:	d00c      	beq.n	3000645e <BOOT_Image1+0x8e6>
30006444:	2500      	movs	r5, #0
30006446:	f8da 1000 	ldr.w	r1, [sl]
3000644a:	2002      	movs	r0, #2
3000644c:	eb01 3105 	add.w	r1, r1, r5, lsl #12
30006450:	3501      	adds	r5, #1
30006452:	f7fd fc43 	bl	30003cdc <FLASH_Erase_With_Lock>
30006456:	2032      	movs	r0, #50	; 0x32
30006458:	47a0      	blx	r4
3000645a:	42ae      	cmp	r6, r5
3000645c:	d1f3      	bne.n	30006446 <BOOT_Image1+0x8ce>
3000645e:	f108 0801 	add.w	r8, r8, #1
30006462:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
30006466:	f10a 0a04 	add.w	sl, sl, #4
3000646a:	4543      	cmp	r3, r8
3000646c:	d8e6      	bhi.n	3000643c <BOOT_Image1+0x8c4>
3000646e:	4832      	ldr	r0, [pc, #200]	; (30006538 <BOOT_Image1+0x9c0>)
30006470:	f006 f916 	bl	3000c6a0 <__DiagPrintf_veneer>
30006474:	4b31      	ldr	r3, [pc, #196]	; (3000653c <BOOT_Image1+0x9c4>)
30006476:	a806      	add	r0, sp, #24
30006478:	4798      	blx	r3
3000647a:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
3000647e:	4830      	ldr	r0, [pc, #192]	; (30006540 <BOOT_Image1+0x9c8>)
30006480:	a906      	add	r1, sp, #24
30006482:	4b30      	ldr	r3, [pc, #192]	; (30006544 <BOOT_Image1+0x9cc>)
30006484:	f8ad 201a 	strh.w	r2, [sp, #26]
30006488:	4798      	blx	r3
3000648a:	482d      	ldr	r0, [pc, #180]	; (30006540 <BOOT_Image1+0x9c8>)
3000648c:	4b2e      	ldr	r3, [pc, #184]	; (30006548 <BOOT_Image1+0x9d0>)
3000648e:	4798      	blx	r3
30006490:	e7fe      	b.n	30006490 <BOOT_Image1+0x918>
30006492:	4b2e      	ldr	r3, [pc, #184]	; (3000654c <BOOT_Image1+0x9d4>)
30006494:	4798      	blx	r3
30006496:	2801      	cmp	r0, #1
30006498:	bf0c      	ite	eq
3000649a:	2019      	moveq	r0, #25
3000649c:	2004      	movne	r0, #4
3000649e:	47a0      	blx	r4
300064a0:	e5f4      	b.n	3000608c <BOOT_Image1+0x514>
300064a2:	4b2a      	ldr	r3, [pc, #168]	; (3000654c <BOOT_Image1+0x9d4>)
300064a4:	4798      	blx	r3
300064a6:	2801      	cmp	r0, #1
300064a8:	bf0c      	ite	eq
300064aa:	20e6      	moveq	r0, #230	; 0xe6
300064ac:	2019      	movne	r0, #25
300064ae:	47a0      	blx	r4
300064b0:	e5ff      	b.n	300060b2 <BOOT_Image1+0x53a>
300064b2:	4827      	ldr	r0, [pc, #156]	; (30006550 <BOOT_Image1+0x9d8>)
300064b4:	f006 f8f4 	bl	3000c6a0 <__DiagPrintf_veneer>
300064b8:	e5cc      	b.n	30006054 <BOOT_Image1+0x4dc>
300064ba:	bf00      	nop
300064bc:	23020000 	.word	0x23020000
300064c0:	0000b479 	.word	0x0000b479
300064c4:	3000cebc 	.word	0x3000cebc
300064c8:	3000ce8c 	.word	0x3000ce8c
300064cc:	0000c0f9 	.word	0x0000c0f9
300064d0:	0000aea1 	.word	0x0000aea1
300064d4:	0000aab9 	.word	0x0000aab9
300064d8:	4200c000 	.word	0x4200c000
300064dc:	0000aaa5 	.word	0x0000aaa5
300064e0:	0016e360 	.word	0x0016e360
300064e4:	3000cfd8 	.word	0x3000cfd8
300064e8:	3000c810 	.word	0x3000c810
300064ec:	3000ceec 	.word	0x3000ceec
300064f0:	30008e71 	.word	0x30008e71
300064f4:	3000b981 	.word	0x3000b981
300064f8:	3000cf00 	.word	0x3000cf00
300064fc:	3000cf7c 	.word	0x3000cf7c
30006500:	3000cfb4 	.word	0x3000cfb4
30006504:	3000e8f8 	.word	0x3000e8f8
30006508:	0000c179 	.word	0x0000c179
3000650c:	3000efe0 	.word	0x3000efe0
30006510:	3000f008 	.word	0x3000f008
30006514:	000129bd 	.word	0x000129bd
30006518:	3000f00c 	.word	0x3000f00c
3000651c:	3000f034 	.word	0x3000f034
30006520:	3000d008 	.word	0x3000d008
30006524:	41000200 	.word	0x41000200
30006528:	3000cf58 	.word	0x3000cf58
3000652c:	3000eac0 	.word	0x3000eac0
30006530:	3000f8c0 	.word	0x3000f8c0
30006534:	3000efa0 	.word	0x3000efa0
30006538:	3000d040 	.word	0x3000d040
3000653c:	0000d3f9 	.word	0x0000d3f9
30006540:	41000440 	.word	0x41000440
30006544:	0000d409 	.word	0x0000d409
30006548:	0000d489 	.word	0x0000d489
3000654c:	0000c169 	.word	0x0000c169
30006550:	3000d018 	.word	0x3000d018
30006554:	480d      	ldr	r0, [pc, #52]	; (3000658c <BOOT_Image1+0xa14>)
30006556:	f006 f8a3 	bl	3000c6a0 <__DiagPrintf_veneer>
3000655a:	480d      	ldr	r0, [pc, #52]	; (30006590 <BOOT_Image1+0xa18>)
3000655c:	f006 f8a0 	bl	3000c6a0 <__DiagPrintf_veneer>
30006560:	e578      	b.n	30006054 <BOOT_Image1+0x4dc>
30006562:	480c      	ldr	r0, [pc, #48]	; (30006594 <BOOT_Image1+0xa1c>)
30006564:	f006 f89c 	bl	3000c6a0 <__DiagPrintf_veneer>
30006568:	e574      	b.n	30006054 <BOOT_Image1+0x4dc>
3000656a:	46d3      	mov	fp, sl
3000656c:	e52b      	b.n	30005fc6 <BOOT_Image1+0x44e>
3000656e:	46da      	mov	sl, fp
30006570:	f04f 0900 	mov.w	r9, #0
30006574:	e4fd      	b.n	30005f72 <BOOT_Image1+0x3fa>
30006576:	2600      	movs	r6, #0
30006578:	f04f 080a 	mov.w	r8, #10
3000657c:	4633      	mov	r3, r6
3000657e:	46b3      	mov	fp, r6
30006580:	e4c8      	b.n	30005f14 <BOOT_Image1+0x39c>
30006582:	f106 080b 	add.w	r8, r6, #11
30006586:	360a      	adds	r6, #10
30006588:	44c8      	add	r8, r9
3000658a:	e72b      	b.n	300063e4 <BOOT_Image1+0x86c>
3000658c:	3000cff0 	.word	0x3000cff0
30006590:	3000d008 	.word	0x3000d008
30006594:	3000d02c 	.word	0x3000d02c

30006598 <BOOT_LoadSubImage>:
30006598:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000659c:	b08f      	sub	sp, #60	; 0x3c
3000659e:	469a      	mov	sl, r3
300065a0:	460f      	mov	r7, r1
300065a2:	f89d 3060 	ldrb.w	r3, [sp, #96]	; 0x60
300065a6:	9305      	str	r3, [sp, #20]
300065a8:	b39a      	cbz	r2, 30006612 <BOOT_LoadSubImage+0x7a>
300065aa:	4691      	mov	r9, r2
300065ac:	2600      	movs	r6, #0
300065ae:	4605      	mov	r5, r0
300065b0:	2220      	movs	r2, #32
300065b2:	4639      	mov	r1, r7
300065b4:	a806      	add	r0, sp, #24
300065b6:	f7fe f9d1 	bl	3000495c <BOOT_ImgCopy>
300065ba:	9b06      	ldr	r3, [sp, #24]
300065bc:	4a49      	ldr	r2, [pc, #292]	; (300066e4 <BOOT_LoadSubImage+0x14c>)
300065be:	4293      	cmp	r3, r2
300065c0:	d157      	bne.n	30006672 <BOOT_LoadSubImage+0xda>
300065c2:	f102 427b 	add.w	r2, r2, #4211081216	; 0xfb000000
300065c6:	9b07      	ldr	r3, [sp, #28]
300065c8:	f502 0278 	add.w	r2, r2, #16252928	; 0xf80000
300065cc:	f502 62c0 	add.w	r2, r2, #1536	; 0x600
300065d0:	4293      	cmp	r3, r2
300065d2:	d14e      	bne.n	30006672 <BOOT_LoadSubImage+0xda>
300065d4:	4b44      	ldr	r3, [pc, #272]	; (300066e8 <BOOT_LoadSubImage+0x150>)
300065d6:	e9dd 4008 	ldrd	r4, r0, [sp, #32]
300065da:	18c3      	adds	r3, r0, r3
300065dc:	f1a0 0820 	sub.w	r8, r0, #32
300065e0:	3420      	adds	r4, #32
300065e2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300065e6:	d21a      	bcs.n	3000661e <BOOT_LoadSubImage+0x86>
300065e8:	b17d      	cbz	r5, 3000660a <BOOT_LoadSubImage+0x72>
300065ea:	1d2b      	adds	r3, r5, #4
300065ec:	f845 8036 	str.w	r8, [r5, r6, lsl #3]
300065f0:	4a3e      	ldr	r2, [pc, #248]	; (300066ec <BOOT_LoadSubImage+0x154>)
300065f2:	2004      	movs	r0, #4
300065f4:	e9cd 8401 	strd	r8, r4, [sp, #4]
300065f8:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
300065fc:	f85a 3026 	ldr.w	r3, [sl, r6, lsl #2]
30006600:	9300      	str	r3, [sp, #0]
30006602:	4b3b      	ldr	r3, [pc, #236]	; (300066f0 <BOOT_LoadSubImage+0x158>)
30006604:	4619      	mov	r1, r3
30006606:	f005 fda3 	bl	3000c150 <rtk_log_write>
3000660a:	3601      	adds	r6, #1
3000660c:	4427      	add	r7, r4
3000660e:	454e      	cmp	r6, r9
30006610:	d1ce      	bne.n	300065b0 <BOOT_LoadSubImage+0x18>
30006612:	4b38      	ldr	r3, [pc, #224]	; (300066f4 <BOOT_LoadSubImage+0x15c>)
30006614:	2001      	movs	r0, #1
30006616:	601f      	str	r7, [r3, #0]
30006618:	b00f      	add	sp, #60	; 0x3c
3000661a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000661e:	4639      	mov	r1, r7
30006620:	4622      	mov	r2, r4
30006622:	4640      	mov	r0, r8
30006624:	f7fe f99a 	bl	3000495c <BOOT_ImgCopy>
30006628:	4933      	ldr	r1, [pc, #204]	; (300066f8 <BOOT_LoadSubImage+0x160>)
3000662a:	694b      	ldr	r3, [r1, #20]
3000662c:	03db      	lsls	r3, r3, #15
3000662e:	d5db      	bpl.n	300065e8 <BOOT_LoadSubImage+0x50>
30006630:	ea08 0304 	and.w	r3, r8, r4
30006634:	3301      	adds	r3, #1
30006636:	d023      	beq.n	30006680 <BOOT_LoadSubImage+0xe8>
30006638:	f018 031f 	ands.w	r3, r8, #31
3000663c:	d042      	beq.n	300066c4 <BOOT_LoadSubImage+0x12c>
3000663e:	4423      	add	r3, r4
30006640:	f028 021f 	bic.w	r2, r8, #31
30006644:	f3bf 8f4f 	dsb	sy
30006648:	2b00      	cmp	r3, #0
3000664a:	dd0d      	ble.n	30006668 <BOOT_LoadSubImage+0xd0>
3000664c:	1e59      	subs	r1, r3, #1
3000664e:	f102 0320 	add.w	r3, r2, #32
30006652:	4829      	ldr	r0, [pc, #164]	; (300066f8 <BOOT_LoadSubImage+0x160>)
30006654:	f021 011f 	bic.w	r1, r1, #31
30006658:	4419      	add	r1, r3
3000665a:	428b      	cmp	r3, r1
3000665c:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30006660:	461a      	mov	r2, r3
30006662:	f103 0320 	add.w	r3, r3, #32
30006666:	d1f8      	bne.n	3000665a <BOOT_LoadSubImage+0xc2>
30006668:	f3bf 8f4f 	dsb	sy
3000666c:	f3bf 8f6f 	isb	sy
30006670:	e7ba      	b.n	300065e8 <BOOT_LoadSubImage+0x50>
30006672:	9b05      	ldr	r3, [sp, #20]
30006674:	2b01      	cmp	r3, #1
30006676:	d028      	beq.n	300066ca <BOOT_LoadSubImage+0x132>
30006678:	2000      	movs	r0, #0
3000667a:	b00f      	add	sp, #60	; 0x3c
3000667c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006680:	2300      	movs	r3, #0
30006682:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30006686:	f3bf 8f4f 	dsb	sy
3000668a:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
3000668e:	f643 7be0 	movw	fp, #16352	; 0x3fe0
30006692:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
30006696:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000669a:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
3000669e:	ea0c 000b 	and.w	r0, ip, fp
300066a2:	4673      	mov	r3, lr
300066a4:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
300066a8:	3b01      	subs	r3, #1
300066aa:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300066ae:	d2f9      	bcs.n	300066a4 <BOOT_LoadSubImage+0x10c>
300066b0:	f1ac 0c20 	sub.w	ip, ip, #32
300066b4:	f11c 0f20 	cmn.w	ip, #32
300066b8:	d1f1      	bne.n	3000669e <BOOT_LoadSubImage+0x106>
300066ba:	f3bf 8f4f 	dsb	sy
300066be:	f3bf 8f6f 	isb	sy
300066c2:	e791      	b.n	300065e8 <BOOT_LoadSubImage+0x50>
300066c4:	4623      	mov	r3, r4
300066c6:	4642      	mov	r2, r8
300066c8:	e7bc      	b.n	30006644 <BOOT_LoadSubImage+0xac>
300066ca:	f85a 4026 	ldr.w	r4, [sl, r6, lsl #2]
300066ce:	2004      	movs	r0, #4
300066d0:	4b07      	ldr	r3, [pc, #28]	; (300066f0 <BOOT_LoadSubImage+0x158>)
300066d2:	4a0a      	ldr	r2, [pc, #40]	; (300066fc <BOOT_LoadSubImage+0x164>)
300066d4:	4619      	mov	r1, r3
300066d6:	9400      	str	r4, [sp, #0]
300066d8:	f005 fd3a 	bl	3000c150 <rtk_log_write>
300066dc:	2000      	movs	r0, #0
300066de:	b00f      	add	sp, #60	; 0x3c
300066e0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300066e4:	35393138 	.word	0x35393138
300066e8:	f7ffffe0 	.word	0xf7ffffe0
300066ec:	3000d0e8 	.word	0x3000d0e8
300066f0:	3000c810 	.word	0x3000c810
300066f4:	3000f038 	.word	0x3000f038
300066f8:	e000ed00 	.word	0xe000ed00
300066fc:	3000d0d4 	.word	0x3000d0d4

30006700 <BOOT_LoadAPSubImage>:
30006700:	4bb5      	ldr	r3, [pc, #724]	; (300069d8 <BOOT_LoadAPSubImage+0x2d8>)
30006702:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30006706:	460c      	mov	r4, r1
30006708:	4606      	mov	r6, r0
3000670a:	b08f      	sub	sp, #60	; 0x3c
3000670c:	4615      	mov	r5, r2
3000670e:	2220      	movs	r2, #32
30006710:	e893 0003 	ldmia.w	r3, {r0, r1}
30006714:	0c0b      	lsrs	r3, r1, #16
30006716:	9004      	str	r0, [sp, #16]
30006718:	f8ad 1014 	strh.w	r1, [sp, #20]
3000671c:	a806      	add	r0, sp, #24
3000671e:	4621      	mov	r1, r4
30006720:	f88d 3016 	strb.w	r3, [sp, #22]
30006724:	f7fe f91a 	bl	3000495c <BOOT_ImgCopy>
30006728:	4bac      	ldr	r3, [pc, #688]	; (300069dc <BOOT_LoadAPSubImage+0x2dc>)
3000672a:	9a06      	ldr	r2, [sp, #24]
3000672c:	429a      	cmp	r2, r3
3000672e:	d108      	bne.n	30006742 <BOOT_LoadAPSubImage+0x42>
30006730:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30006734:	9a07      	ldr	r2, [sp, #28]
30006736:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
3000673a:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
3000673e:	429a      	cmp	r2, r3
30006740:	d006      	beq.n	30006750 <BOOT_LoadAPSubImage+0x50>
30006742:	2d01      	cmp	r5, #1
30006744:	f000 80b3 	beq.w	300068ae <BOOT_LoadAPSubImage+0x1ae>
30006748:	2000      	movs	r0, #0
3000674a:	b00f      	add	sp, #60	; 0x3c
3000674c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006750:	9a09      	ldr	r2, [sp, #36]	; 0x24
30006752:	4ba3      	ldr	r3, [pc, #652]	; (300069e0 <BOOT_LoadAPSubImage+0x2e0>)
30006754:	6ca7      	ldr	r7, [r4, #72]	; 0x48
30006756:	f1a2 0820 	sub.w	r8, r2, #32
3000675a:	4413      	add	r3, r2
3000675c:	37d0      	adds	r7, #208	; 0xd0
3000675e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006762:	d247      	bcs.n	300067f4 <BOOT_LoadAPSubImage+0xf4>
30006764:	4b9f      	ldr	r3, [pc, #636]	; (300069e4 <BOOT_LoadAPSubImage+0x2e4>)
30006766:	eb07 0508 	add.w	r5, r7, r8
3000676a:	f8d4 8070 	ldr.w	r8, [r4, #112]	; 0x70
3000676e:	4798      	blx	r3
30006770:	2801      	cmp	r0, #1
30006772:	4427      	add	r7, r4
30006774:	4b9c      	ldr	r3, [pc, #624]	; (300069e8 <BOOT_LoadAPSubImage+0x2e8>)
30006776:	d029      	beq.n	300067cc <BOOT_LoadAPSubImage+0xcc>
30006778:	4619      	mov	r1, r3
3000677a:	4a9c      	ldr	r2, [pc, #624]	; (300069ec <BOOT_LoadAPSubImage+0x2ec>)
3000677c:	2004      	movs	r0, #4
3000677e:	f005 fce7 	bl	3000c150 <rtk_log_write>
30006782:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30006786:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000678a:	f080 809c 	bcs.w	300068c6 <BOOT_LoadAPSubImage+0x1c6>
3000678e:	4445      	add	r5, r8
30006790:	eb08 0107 	add.w	r1, r8, r7
30006794:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30006798:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000679c:	d256      	bcs.n	3000684c <BOOT_LoadAPSubImage+0x14c>
3000679e:	2e00      	cmp	r6, #0
300067a0:	f000 8081 	beq.w	300068a6 <BOOT_LoadAPSubImage+0x1a6>
300067a4:	4b90      	ldr	r3, [pc, #576]	; (300069e8 <BOOT_LoadAPSubImage+0x2e8>)
300067a6:	aa04      	add	r2, sp, #16
300067a8:	2004      	movs	r0, #4
300067aa:	9200      	str	r2, [sp, #0]
300067ac:	4a90      	ldr	r2, [pc, #576]	; (300069f0 <BOOT_LoadAPSubImage+0x2f0>)
300067ae:	e9dd 1408 	ldrd	r1, r4, [sp, #32]
300067b2:	3c20      	subs	r4, #32
300067b4:	3120      	adds	r1, #32
300067b6:	e9c6 4100 	strd	r4, r1, [r6]
300067ba:	9102      	str	r1, [sp, #8]
300067bc:	4619      	mov	r1, r3
300067be:	9401      	str	r4, [sp, #4]
300067c0:	f005 fcc6 	bl	3000c150 <rtk_log_write>
300067c4:	2001      	movs	r0, #1
300067c6:	b00f      	add	sp, #60	; 0x3c
300067c8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300067cc:	4a89      	ldr	r2, [pc, #548]	; (300069f4 <BOOT_LoadAPSubImage+0x2f4>)
300067ce:	2004      	movs	r0, #4
300067d0:	4619      	mov	r1, r3
300067d2:	f005 fcbd 	bl	3000c150 <rtk_log_write>
300067d6:	4642      	mov	r2, r8
300067d8:	4629      	mov	r1, r5
300067da:	4638      	mov	r0, r7
300067dc:	f000 fbe4 	bl	30006fa8 <BOOT_LoadBL32RDPImg>
300067e0:	2800      	cmp	r0, #0
300067e2:	d1d4      	bne.n	3000678e <BOOT_LoadAPSubImage+0x8e>
300067e4:	4b80      	ldr	r3, [pc, #512]	; (300069e8 <BOOT_LoadAPSubImage+0x2e8>)
300067e6:	2004      	movs	r0, #4
300067e8:	4a83      	ldr	r2, [pc, #524]	; (300069f8 <BOOT_LoadAPSubImage+0x2f8>)
300067ea:	4619      	mov	r1, r3
300067ec:	f005 fcb0 	bl	3000c150 <rtk_log_write>
300067f0:	20ff      	movs	r0, #255	; 0xff
300067f2:	e7aa      	b.n	3000674a <BOOT_LoadAPSubImage+0x4a>
300067f4:	463a      	mov	r2, r7
300067f6:	4621      	mov	r1, r4
300067f8:	4640      	mov	r0, r8
300067fa:	f7fe f8af 	bl	3000495c <BOOT_ImgCopy>
300067fe:	497f      	ldr	r1, [pc, #508]	; (300069fc <BOOT_LoadAPSubImage+0x2fc>)
30006800:	694b      	ldr	r3, [r1, #20]
30006802:	03da      	lsls	r2, r3, #15
30006804:	d5ae      	bpl.n	30006764 <BOOT_LoadAPSubImage+0x64>
30006806:	ea07 0308 	and.w	r3, r7, r8
3000680a:	3301      	adds	r3, #1
3000680c:	f000 8086 	beq.w	3000691c <BOOT_LoadAPSubImage+0x21c>
30006810:	f018 031f 	ands.w	r3, r8, #31
30006814:	f000 80bb 	beq.w	3000698e <BOOT_LoadAPSubImage+0x28e>
30006818:	443b      	add	r3, r7
3000681a:	f028 021f 	bic.w	r2, r8, #31
3000681e:	f3bf 8f4f 	dsb	sy
30006822:	2b00      	cmp	r3, #0
30006824:	dd0d      	ble.n	30006842 <BOOT_LoadAPSubImage+0x142>
30006826:	1e59      	subs	r1, r3, #1
30006828:	f102 0320 	add.w	r3, r2, #32
3000682c:	4873      	ldr	r0, [pc, #460]	; (300069fc <BOOT_LoadAPSubImage+0x2fc>)
3000682e:	f021 011f 	bic.w	r1, r1, #31
30006832:	4419      	add	r1, r3
30006834:	428b      	cmp	r3, r1
30006836:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
3000683a:	461a      	mov	r2, r3
3000683c:	f103 0320 	add.w	r3, r3, #32
30006840:	d1f8      	bne.n	30006834 <BOOT_LoadAPSubImage+0x134>
30006842:	f3bf 8f4f 	dsb	sy
30006846:	f3bf 8f6f 	isb	sy
3000684a:	e78b      	b.n	30006764 <BOOT_LoadAPSubImage+0x64>
3000684c:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30006850:	4628      	mov	r0, r5
30006852:	4622      	mov	r2, r4
30006854:	f7fe f882 	bl	3000495c <BOOT_ImgCopy>
30006858:	4968      	ldr	r1, [pc, #416]	; (300069fc <BOOT_LoadAPSubImage+0x2fc>)
3000685a:	694b      	ldr	r3, [r1, #20]
3000685c:	03d8      	lsls	r0, r3, #15
3000685e:	d59e      	bpl.n	3000679e <BOOT_LoadAPSubImage+0x9e>
30006860:	ea05 0304 	and.w	r3, r5, r4
30006864:	3301      	adds	r3, #1
30006866:	d075      	beq.n	30006954 <BOOT_LoadAPSubImage+0x254>
30006868:	f015 031f 	ands.w	r3, r5, #31
3000686c:	d002      	beq.n	30006874 <BOOT_LoadAPSubImage+0x174>
3000686e:	f025 051f 	bic.w	r5, r5, #31
30006872:	441c      	add	r4, r3
30006874:	f3bf 8f4f 	dsb	sy
30006878:	2c00      	cmp	r4, #0
3000687a:	dd0d      	ble.n	30006898 <BOOT_LoadAPSubImage+0x198>
3000687c:	1e62      	subs	r2, r4, #1
3000687e:	f105 0320 	add.w	r3, r5, #32
30006882:	495e      	ldr	r1, [pc, #376]	; (300069fc <BOOT_LoadAPSubImage+0x2fc>)
30006884:	f022 021f 	bic.w	r2, r2, #31
30006888:	441a      	add	r2, r3
3000688a:	4293      	cmp	r3, r2
3000688c:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30006890:	461d      	mov	r5, r3
30006892:	f103 0320 	add.w	r3, r3, #32
30006896:	d1f8      	bne.n	3000688a <BOOT_LoadAPSubImage+0x18a>
30006898:	f3bf 8f4f 	dsb	sy
3000689c:	f3bf 8f6f 	isb	sy
300068a0:	2e00      	cmp	r6, #0
300068a2:	f47f af7f 	bne.w	300067a4 <BOOT_LoadAPSubImage+0xa4>
300068a6:	2001      	movs	r0, #1
300068a8:	b00f      	add	sp, #60	; 0x3c
300068aa:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300068ae:	4b4e      	ldr	r3, [pc, #312]	; (300069e8 <BOOT_LoadAPSubImage+0x2e8>)
300068b0:	ac04      	add	r4, sp, #16
300068b2:	4a53      	ldr	r2, [pc, #332]	; (30006a00 <BOOT_LoadAPSubImage+0x300>)
300068b4:	2004      	movs	r0, #4
300068b6:	4619      	mov	r1, r3
300068b8:	9400      	str	r4, [sp, #0]
300068ba:	f005 fc49 	bl	3000c150 <rtk_log_write>
300068be:	2000      	movs	r0, #0
300068c0:	b00f      	add	sp, #60	; 0x3c
300068c2:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300068c6:	4639      	mov	r1, r7
300068c8:	4642      	mov	r2, r8
300068ca:	4628      	mov	r0, r5
300068cc:	f7fe f846 	bl	3000495c <BOOT_ImgCopy>
300068d0:	494a      	ldr	r1, [pc, #296]	; (300069fc <BOOT_LoadAPSubImage+0x2fc>)
300068d2:	694b      	ldr	r3, [r1, #20]
300068d4:	03db      	lsls	r3, r3, #15
300068d6:	f57f af5a 	bpl.w	3000678e <BOOT_LoadAPSubImage+0x8e>
300068da:	ea08 0305 	and.w	r3, r8, r5
300068de:	3301      	adds	r3, #1
300068e0:	d058      	beq.n	30006994 <BOOT_LoadAPSubImage+0x294>
300068e2:	f015 031f 	ands.w	r3, r5, #31
300068e6:	d073      	beq.n	300069d0 <BOOT_LoadAPSubImage+0x2d0>
300068e8:	4443      	add	r3, r8
300068ea:	f025 021f 	bic.w	r2, r5, #31
300068ee:	f3bf 8f4f 	dsb	sy
300068f2:	2b00      	cmp	r3, #0
300068f4:	dd0d      	ble.n	30006912 <BOOT_LoadAPSubImage+0x212>
300068f6:	1e59      	subs	r1, r3, #1
300068f8:	f102 0320 	add.w	r3, r2, #32
300068fc:	483f      	ldr	r0, [pc, #252]	; (300069fc <BOOT_LoadAPSubImage+0x2fc>)
300068fe:	f021 011f 	bic.w	r1, r1, #31
30006902:	4419      	add	r1, r3
30006904:	428b      	cmp	r3, r1
30006906:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
3000690a:	461a      	mov	r2, r3
3000690c:	f103 0320 	add.w	r3, r3, #32
30006910:	d1f8      	bne.n	30006904 <BOOT_LoadAPSubImage+0x204>
30006912:	f3bf 8f4f 	dsb	sy
30006916:	f3bf 8f6f 	isb	sy
3000691a:	e738      	b.n	3000678e <BOOT_LoadAPSubImage+0x8e>
3000691c:	2300      	movs	r3, #0
3000691e:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30006922:	f3bf 8f4f 	dsb	sy
30006926:	f8d1 0080 	ldr.w	r0, [r1, #128]	; 0x80
3000692a:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
3000692e:	f3c0 324e 	ubfx	r2, r0, #13, #15
30006932:	f3c0 00c9 	ubfx	r0, r0, #3, #10
30006936:	0152      	lsls	r2, r2, #5
30006938:	ea02 0c0e 	and.w	ip, r2, lr
3000693c:	4603      	mov	r3, r0
3000693e:	ea4c 7583 	orr.w	r5, ip, r3, lsl #30
30006942:	3b01      	subs	r3, #1
30006944:	f8c1 5274 	str.w	r5, [r1, #628]	; 0x274
30006948:	d2f9      	bcs.n	3000693e <BOOT_LoadAPSubImage+0x23e>
3000694a:	3a20      	subs	r2, #32
3000694c:	f112 0f20 	cmn.w	r2, #32
30006950:	d1f2      	bne.n	30006938 <BOOT_LoadAPSubImage+0x238>
30006952:	e776      	b.n	30006842 <BOOT_LoadAPSubImage+0x142>
30006954:	2300      	movs	r3, #0
30006956:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000695a:	f3bf 8f4f 	dsb	sy
3000695e:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
30006962:	f643 77e0 	movw	r7, #16352	; 0x3fe0
30006966:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000696a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000696e:	0164      	lsls	r4, r4, #5
30006970:	ea04 0007 	and.w	r0, r4, r7
30006974:	462b      	mov	r3, r5
30006976:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
3000697a:	3b01      	subs	r3, #1
3000697c:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30006980:	1c5a      	adds	r2, r3, #1
30006982:	d1f8      	bne.n	30006976 <BOOT_LoadAPSubImage+0x276>
30006984:	3c20      	subs	r4, #32
30006986:	f114 0f20 	cmn.w	r4, #32
3000698a:	d1f1      	bne.n	30006970 <BOOT_LoadAPSubImage+0x270>
3000698c:	e784      	b.n	30006898 <BOOT_LoadAPSubImage+0x198>
3000698e:	463b      	mov	r3, r7
30006990:	4642      	mov	r2, r8
30006992:	e744      	b.n	3000681e <BOOT_LoadAPSubImage+0x11e>
30006994:	2300      	movs	r3, #0
30006996:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000699a:	f3bf 8f4f 	dsb	sy
3000699e:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
300069a2:	f643 79e0 	movw	r9, #16352	; 0x3fe0
300069a6:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
300069aa:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
300069ae:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
300069b2:	ea0c 0009 	and.w	r0, ip, r9
300069b6:	4673      	mov	r3, lr
300069b8:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
300069bc:	3b01      	subs	r3, #1
300069be:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300069c2:	d2f9      	bcs.n	300069b8 <BOOT_LoadAPSubImage+0x2b8>
300069c4:	f1ac 0c20 	sub.w	ip, ip, #32
300069c8:	f11c 0f20 	cmn.w	ip, #32
300069cc:	d1f1      	bne.n	300069b2 <BOOT_LoadAPSubImage+0x2b2>
300069ce:	e7a0      	b.n	30006912 <BOOT_LoadAPSubImage+0x212>
300069d0:	4643      	mov	r3, r8
300069d2:	462a      	mov	r2, r5
300069d4:	e78b      	b.n	300068ee <BOOT_LoadAPSubImage+0x1ee>
300069d6:	bf00      	nop
300069d8:	3000d0cc 	.word	0x3000d0cc
300069dc:	35393138 	.word	0x35393138
300069e0:	f7ffffe0 	.word	0xf7ffffe0
300069e4:	0000c109 	.word	0x0000c109
300069e8:	3000c810 	.word	0x3000c810
300069ec:	3000d12c 	.word	0x3000d12c
300069f0:	3000d0e8 	.word	0x3000d0e8
300069f4:	3000d0fc 	.word	0x3000d0fc
300069f8:	3000d10c 	.word	0x3000d10c
300069fc:	e000ed00 	.word	0xe000ed00
30006a00:	3000d0d4 	.word	0x3000d0d4

30006a04 <BOOT_RSIPIvSet>:
30006a04:	b530      	push	{r4, r5, lr}
30006a06:	2300      	movs	r3, #0
30006a08:	b083      	sub	sp, #12
30006a0a:	4a09      	ldr	r2, [pc, #36]	; (30006a30 <BOOT_RSIPIvSet+0x2c>)
30006a0c:	4605      	mov	r5, r0
30006a0e:	460c      	mov	r4, r1
30006a10:	9300      	str	r3, [sp, #0]
30006a12:	9301      	str	r3, [sp, #4]
30006a14:	4790      	blx	r2
30006a16:	b148      	cbz	r0, 30006a2c <BOOT_RSIPIvSet+0x28>
30006a18:	f105 0120 	add.w	r1, r5, #32
30006a1c:	4b05      	ldr	r3, [pc, #20]	; (30006a34 <BOOT_RSIPIvSet+0x30>)
30006a1e:	2208      	movs	r2, #8
30006a20:	4668      	mov	r0, sp
30006a22:	4798      	blx	r3
30006a24:	4669      	mov	r1, sp
30006a26:	b2e0      	uxtb	r0, r4
30006a28:	4b03      	ldr	r3, [pc, #12]	; (30006a38 <BOOT_RSIPIvSet+0x34>)
30006a2a:	4798      	blx	r3
30006a2c:	b003      	add	sp, #12
30006a2e:	bd30      	pop	{r4, r5, pc}
30006a30:	0000c129 	.word	0x0000c129
30006a34:	00012a1d 	.word	0x00012a1d
30006a38:	30009b65 	.word	0x30009b65

30006a3c <BOOT_OTFCheck>:
30006a3c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30006a40:	4d1c      	ldr	r5, [pc, #112]	; (30006ab4 <BOOT_OTFCheck+0x78>)
30006a42:	b082      	sub	sp, #8
30006a44:	4604      	mov	r4, r0
30006a46:	4688      	mov	r8, r1
30006a48:	4617      	mov	r7, r2
30006a4a:	461e      	mov	r6, r3
30006a4c:	47a8      	blx	r5
30006a4e:	b148      	cbz	r0, 30006a64 <BOOT_OTFCheck+0x28>
30006a50:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30006a54:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006a58:	d204      	bcs.n	30006a64 <BOOT_OTFCheck+0x28>
30006a5a:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30006a5e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006a62:	d302      	bcc.n	30006a6a <BOOT_OTFCheck+0x2e>
30006a64:	b002      	add	sp, #8
30006a66:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30006a6a:	4b13      	ldr	r3, [pc, #76]	; (30006ab8 <BOOT_OTFCheck+0x7c>)
30006a6c:	2004      	movs	r0, #4
30006a6e:	4a13      	ldr	r2, [pc, #76]	; (30006abc <BOOT_OTFCheck+0x80>)
30006a70:	4619      	mov	r1, r3
30006a72:	f005 fb6d 	bl	3000c150 <rtk_log_write>
30006a76:	4b12      	ldr	r3, [pc, #72]	; (30006ac0 <BOOT_OTFCheck+0x84>)
30006a78:	4798      	blx	r3
30006a7a:	4603      	mov	r3, r0
30006a7c:	b150      	cbz	r0, 30006a94 <BOOT_OTFCheck+0x58>
30006a7e:	2801      	cmp	r0, #1
30006a80:	d009      	beq.n	30006a96 <BOOT_OTFCheck+0x5a>
30006a82:	4b0d      	ldr	r3, [pc, #52]	; (30006ab8 <BOOT_OTFCheck+0x7c>)
30006a84:	2002      	movs	r0, #2
30006a86:	4a0f      	ldr	r2, [pc, #60]	; (30006ac4 <BOOT_OTFCheck+0x88>)
30006a88:	4619      	mov	r1, r3
30006a8a:	b002      	add	sp, #8
30006a8c:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30006a90:	f005 bb5e 	b.w	3000c150 <rtk_log_write>
30006a94:	2302      	movs	r3, #2
30006a96:	4630      	mov	r0, r6
30006a98:	4621      	mov	r1, r4
30006a9a:	9301      	str	r3, [sp, #4]
30006a9c:	4642      	mov	r2, r8
30006a9e:	2301      	movs	r3, #1
30006aa0:	4c09      	ldr	r4, [pc, #36]	; (30006ac8 <BOOT_OTFCheck+0x8c>)
30006aa2:	9700      	str	r7, [sp, #0]
30006aa4:	47a0      	blx	r4
30006aa6:	2001      	movs	r0, #1
30006aa8:	4b08      	ldr	r3, [pc, #32]	; (30006acc <BOOT_OTFCheck+0x90>)
30006aaa:	b002      	add	sp, #8
30006aac:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30006ab0:	4718      	bx	r3
30006ab2:	bf00      	nop
30006ab4:	0000c129 	.word	0x0000c129
30006ab8:	3000c810 	.word	0x3000c810
30006abc:	3000d13c 	.word	0x3000d13c
30006ac0:	0000c245 	.word	0x0000c245
30006ac4:	3000d150 	.word	0x3000d150
30006ac8:	30009b99 	.word	0x30009b99
30006acc:	30009b81 	.word	0x30009b81

30006ad0 <BOOT_OTA_LoadIMG2>:
30006ad0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006ad4:	4b6e      	ldr	r3, [pc, #440]	; (30006c90 <BOOT_OTA_LoadIMG2+0x1c0>)
30006ad6:	b08d      	sub	sp, #52	; 0x34
30006ad8:	4a6e      	ldr	r2, [pc, #440]	; (30006c94 <BOOT_OTA_LoadIMG2+0x1c4>)
30006ada:	f103 040c 	add.w	r4, r3, #12
30006ade:	9003      	str	r0, [sp, #12]
30006ae0:	f8d2 8000 	ldr.w	r8, [r2]
30006ae4:	ae06      	add	r6, sp, #24
30006ae6:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30006aea:	ab09      	add	r3, sp, #36	; 0x24
30006aec:	e886 0007 	stmia.w	r6, {r0, r1, r2}
30006af0:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30006af4:	e883 0007 	stmia.w	r3, {r0, r1, r2}
30006af8:	4641      	mov	r1, r8
30006afa:	4867      	ldr	r0, [pc, #412]	; (30006c98 <BOOT_OTA_LoadIMG2+0x1c8>)
30006afc:	f44f 72b2 	mov.w	r2, #356	; 0x164
30006b00:	f7fd ff2c 	bl	3000495c <BOOT_ImgCopy>
30006b04:	4641      	mov	r1, r8
30006b06:	4864      	ldr	r0, [pc, #400]	; (30006c98 <BOOT_OTA_LoadIMG2+0x1c8>)
30006b08:	f000 fd2a 	bl	30007560 <BOOT_CertificateCheck>
30006b0c:	2800      	cmp	r0, #0
30006b0e:	f040 80b7 	bne.w	30006c80 <BOOT_OTA_LoadIMG2+0x1b0>
30006b12:	4604      	mov	r4, r0
30006b14:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006b18:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30006b1c:	485f      	ldr	r0, [pc, #380]	; (30006c9c <BOOT_OTA_LoadIMG2+0x1cc>)
30006b1e:	f7fd ff1d 	bl	3000495c <BOOT_ImgCopy>
30006b22:	4b5f      	ldr	r3, [pc, #380]	; (30006ca0 <BOOT_OTA_LoadIMG2+0x1d0>)
30006b24:	e9cd 4404 	strd	r4, r4, [sp, #16]
30006b28:	4798      	blx	r3
30006b2a:	2800      	cmp	r0, #0
30006b2c:	f040 809e 	bne.w	30006c6c <BOOT_OTA_LoadIMG2+0x19c>
30006b30:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30006b34:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30006ce4 <BOOT_OTA_LoadIMG2+0x214>
30006b38:	4a5a      	ldr	r2, [pc, #360]	; (30006ca4 <BOOT_OTA_LoadIMG2+0x1d4>)
30006b3a:	2000      	movs	r0, #0
30006b3c:	4445      	add	r5, r8
30006b3e:	4659      	mov	r1, fp
30006b40:	4f59      	ldr	r7, [pc, #356]	; (30006ca8 <BOOT_OTA_LoadIMG2+0x1d8>)
30006b42:	f04f 0801 	mov.w	r8, #1
30006b46:	462b      	mov	r3, r5
30006b48:	f8df 919c 	ldr.w	r9, [pc, #412]	; 30006ce8 <BOOT_OTA_LoadIMG2+0x218>
30006b4c:	47b8      	blx	r7
30006b4e:	2101      	movs	r1, #1
30006b50:	2000      	movs	r0, #0
30006b52:	4b56      	ldr	r3, [pc, #344]	; (30006cac <BOOT_OTA_LoadIMG2+0x1dc>)
30006b54:	4798      	blx	r3
30006b56:	2301      	movs	r3, #1
30006b58:	4952      	ldr	r1, [pc, #328]	; (30006ca4 <BOOT_OTA_LoadIMG2+0x1d4>)
30006b5a:	4658      	mov	r0, fp
30006b5c:	461a      	mov	r2, r3
30006b5e:	f7ff ff6d 	bl	30006a3c <BOOT_OTFCheck>
30006b62:	47c8      	blx	r9
30006b64:	2800      	cmp	r0, #0
30006b66:	bf0c      	ite	eq
30006b68:	46aa      	moveq	sl, r5
30006b6a:	46da      	movne	sl, fp
30006b6c:	4633      	mov	r3, r6
30006b6e:	2203      	movs	r2, #3
30006b70:	4651      	mov	r1, sl
30006b72:	484f      	ldr	r0, [pc, #316]	; (30006cb0 <BOOT_OTA_LoadIMG2+0x1e0>)
30006b74:	f8cd 8000 	str.w	r8, [sp]
30006b78:	f7ff fd0e 	bl	30006598 <BOOT_LoadSubImage>
30006b7c:	2800      	cmp	r0, #0
30006b7e:	d070      	beq.n	30006c62 <BOOT_OTA_LoadIMG2+0x192>
30006b80:	4e4b      	ldr	r6, [pc, #300]	; (30006cb0 <BOOT_OTA_LoadIMG2+0x1e0>)
30006b82:	47c8      	blx	r9
30006b84:	2800      	cmp	r0, #0
30006b86:	bf08      	it	eq
30006b88:	46ab      	moveq	fp, r5
30006b8a:	4f47      	ldr	r7, [pc, #284]	; (30006ca8 <BOOT_OTA_LoadIMG2+0x1d8>)
30006b8c:	68f2      	ldr	r2, [r6, #12]
30006b8e:	4640      	mov	r0, r8
30006b90:	6873      	ldr	r3, [r6, #4]
30006b92:	f8c6 b000 	str.w	fp, [r6]
30006b96:	4413      	add	r3, r2
30006b98:	6972      	ldr	r2, [r6, #20]
30006b9a:	f8df b150 	ldr.w	fp, [pc, #336]	; 30006cec <BOOT_OTA_LoadIMG2+0x21c>
30006b9e:	441a      	add	r2, r3
30006ba0:	4b44      	ldr	r3, [pc, #272]	; (30006cb4 <BOOT_OTA_LoadIMG2+0x1e4>)
30006ba2:	eb0a 0102 	add.w	r1, sl, r2
30006ba6:	61b1      	str	r1, [r6, #24]
30006ba8:	1e51      	subs	r1, r2, #1
30006baa:	400b      	ands	r3, r1
30006bac:	4659      	mov	r1, fp
30006bae:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006bb2:	441d      	add	r5, r3
30006bb4:	1a9b      	subs	r3, r3, r2
30006bb6:	4a40      	ldr	r2, [pc, #256]	; (30006cb8 <BOOT_OTA_LoadIMG2+0x1e8>)
30006bb8:	61f3      	str	r3, [r6, #28]
30006bba:	462b      	mov	r3, r5
30006bbc:	47b8      	blx	r7
30006bbe:	4641      	mov	r1, r8
30006bc0:	4640      	mov	r0, r8
30006bc2:	4b3a      	ldr	r3, [pc, #232]	; (30006cac <BOOT_OTA_LoadIMG2+0x1dc>)
30006bc4:	4798      	blx	r3
30006bc6:	4642      	mov	r2, r8
30006bc8:	2302      	movs	r3, #2
30006bca:	493b      	ldr	r1, [pc, #236]	; (30006cb8 <BOOT_OTA_LoadIMG2+0x1e8>)
30006bcc:	4658      	mov	r0, fp
30006bce:	f7ff ff35 	bl	30006a3c <BOOT_OTFCheck>
30006bd2:	47c8      	blx	r9
30006bd4:	2800      	cmp	r0, #0
30006bd6:	bf0c      	ite	eq
30006bd8:	462f      	moveq	r7, r5
30006bda:	465f      	movne	r7, fp
30006bdc:	ab09      	add	r3, sp, #36	; 0x24
30006bde:	2203      	movs	r2, #3
30006be0:	f106 0020 	add.w	r0, r6, #32
30006be4:	4639      	mov	r1, r7
30006be6:	f8cd 8000 	str.w	r8, [sp]
30006bea:	f7ff fcd5 	bl	30006598 <BOOT_LoadSubImage>
30006bee:	2800      	cmp	r0, #0
30006bf0:	d037      	beq.n	30006c62 <BOOT_OTA_LoadIMG2+0x192>
30006bf2:	47c8      	blx	r9
30006bf4:	6af2      	ldr	r2, [r6, #44]	; 0x2c
30006bf6:	6a73      	ldr	r3, [r6, #36]	; 0x24
30006bf8:	2800      	cmp	r0, #0
30006bfa:	bf08      	it	eq
30006bfc:	46ab      	moveq	fp, r5
30006bfe:	4827      	ldr	r0, [pc, #156]	; (30006c9c <BOOT_OTA_LoadIMG2+0x1cc>)
30006c00:	4413      	add	r3, r2
30006c02:	6b72      	ldr	r2, [r6, #52]	; 0x34
30006c04:	f8c6 b020 	str.w	fp, [r6, #32]
30006c08:	4413      	add	r3, r2
30006c0a:	4a2a      	ldr	r2, [pc, #168]	; (30006cb4 <BOOT_OTA_LoadIMG2+0x1e4>)
30006c0c:	1e59      	subs	r1, r3, #1
30006c0e:	441f      	add	r7, r3
30006c10:	400a      	ands	r2, r1
30006c12:	2100      	movs	r1, #0
30006c14:	63b7      	str	r7, [r6, #56]	; 0x38
30006c16:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30006c1a:	9100      	str	r1, [sp, #0]
30006c1c:	1ad1      	subs	r1, r2, r3
30006c1e:	2900      	cmp	r1, #0
30006c20:	63f1      	str	r1, [r6, #60]	; 0x3c
30006c22:	4631      	mov	r1, r6
30006c24:	bf18      	it	ne
30006c26:	4613      	movne	r3, r2
30006c28:	4a24      	ldr	r2, [pc, #144]	; (30006cbc <BOOT_OTA_LoadIMG2+0x1ec>)
30006c2a:	441d      	add	r5, r3
30006c2c:	4b1a      	ldr	r3, [pc, #104]	; (30006c98 <BOOT_OTA_LoadIMG2+0x1c8>)
30006c2e:	6015      	str	r5, [r2, #0]
30006c30:	bf0c      	ite	eq
30006c32:	2207      	moveq	r2, #7
30006c34:	2208      	movne	r2, #8
30006c36:	f000 fb89 	bl	3000734c <BOOT_SignatureCheck>
30006c3a:	4b21      	ldr	r3, [pc, #132]	; (30006cc0 <BOOT_OTA_LoadIMG2+0x1f0>)
30006c3c:	4605      	mov	r5, r0
30006c3e:	689b      	ldr	r3, [r3, #8]
30006c40:	079b      	lsls	r3, r3, #30
30006c42:	d50a      	bpl.n	30006c5a <BOOT_OTA_LoadIMG2+0x18a>
30006c44:	4b1f      	ldr	r3, [pc, #124]	; (30006cc4 <BOOT_OTA_LoadIMG2+0x1f4>)
30006c46:	4a20      	ldr	r2, [pc, #128]	; (30006cc8 <BOOT_OTA_LoadIMG2+0x1f8>)
30006c48:	9903      	ldr	r1, [sp, #12]
30006c4a:	2800      	cmp	r0, #0
30006c4c:	bf18      	it	ne
30006c4e:	461a      	movne	r2, r3
30006c50:	481e      	ldr	r0, [pc, #120]	; (30006ccc <BOOT_OTA_LoadIMG2+0x1fc>)
30006c52:	3101      	adds	r1, #1
30006c54:	462b      	mov	r3, r5
30006c56:	f005 fd23 	bl	3000c6a0 <__DiagPrintf_veneer>
30006c5a:	b11d      	cbz	r5, 30006c64 <BOOT_OTA_LoadIMG2+0x194>
30006c5c:	481c      	ldr	r0, [pc, #112]	; (30006cd0 <BOOT_OTA_LoadIMG2+0x200>)
30006c5e:	f005 fd1f 	bl	3000c6a0 <__DiagPrintf_veneer>
30006c62:	24ff      	movs	r4, #255	; 0xff
30006c64:	4620      	mov	r0, r4
30006c66:	b00d      	add	sp, #52	; 0x34
30006c68:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006c6c:	2208      	movs	r2, #8
30006c6e:	4919      	ldr	r1, [pc, #100]	; (30006cd4 <BOOT_OTA_LoadIMG2+0x204>)
30006c70:	4b19      	ldr	r3, [pc, #100]	; (30006cd8 <BOOT_OTA_LoadIMG2+0x208>)
30006c72:	a804      	add	r0, sp, #16
30006c74:	4798      	blx	r3
30006c76:	a904      	add	r1, sp, #16
30006c78:	2001      	movs	r0, #1
30006c7a:	4b18      	ldr	r3, [pc, #96]	; (30006cdc <BOOT_OTA_LoadIMG2+0x20c>)
30006c7c:	4798      	blx	r3
30006c7e:	e757      	b.n	30006b30 <BOOT_OTA_LoadIMG2+0x60>
30006c80:	24ff      	movs	r4, #255	; 0xff
30006c82:	4817      	ldr	r0, [pc, #92]	; (30006ce0 <BOOT_OTA_LoadIMG2+0x210>)
30006c84:	f005 fd0c 	bl	3000c6a0 <__DiagPrintf_veneer>
30006c88:	4620      	mov	r0, r4
30006c8a:	b00d      	add	sp, #52	; 0x34
30006c8c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006c90:	3000e5d0 	.word	0x3000e5d0
30006c94:	3000ff18 	.word	0x3000ff18
30006c98:	3000ff8c 	.word	0x3000ff8c
30006c9c:	300100f0 	.word	0x300100f0
30006ca0:	0000c129 	.word	0x0000c129
30006ca4:	0cffffe0 	.word	0x0cffffe0
30006ca8:	30009c15 	.word	0x30009c15
30006cac:	30009c55 	.word	0x30009c55
30006cb0:	30010298 	.word	0x30010298
30006cb4:	fffff000 	.word	0xfffff000
30006cb8:	0dffffe0 	.word	0x0dffffe0
30006cbc:	3000f038 	.word	0x3000f038
30006cc0:	2001c00c 	.word	0x2001c00c
30006cc4:	3000d170 	.word	0x3000d170
30006cc8:	3000d168 	.word	0x3000d168
30006ccc:	3000d1a0 	.word	0x3000d1a0
30006cd0:	3000d1d4 	.word	0x3000d1d4
30006cd4:	30010110 	.word	0x30010110
30006cd8:	00012a1d 	.word	0x00012a1d
30006cdc:	30009b65 	.word	0x30009b65
30006ce0:	3000d178 	.word	0x3000d178
30006ce4:	0c000000 	.word	0x0c000000
30006ce8:	30009e75 	.word	0x30009e75
30006cec:	0d000000 	.word	0x0d000000

30006cf0 <BOOT_OTA_Region_Init>:
30006cf0:	b510      	push	{r4, lr}
30006cf2:	4c10      	ldr	r4, [pc, #64]	; (30006d34 <BOOT_OTA_Region_Init+0x44>)
30006cf4:	2200      	movs	r2, #0
30006cf6:	2002      	movs	r0, #2
30006cf8:	4621      	mov	r1, r4
30006cfa:	f001 fd0b 	bl	30008714 <flash_get_layout_info>
30006cfe:	2200      	movs	r2, #0
30006d00:	1d21      	adds	r1, r4, #4
30006d02:	2003      	movs	r0, #3
30006d04:	f001 fd06 	bl	30008714 <flash_get_layout_info>
30006d08:	2200      	movs	r2, #0
30006d0a:	f104 0110 	add.w	r1, r4, #16
30006d0e:	2004      	movs	r0, #4
30006d10:	f001 fd00 	bl	30008714 <flash_get_layout_info>
30006d14:	2200      	movs	r2, #0
30006d16:	f104 0114 	add.w	r1, r4, #20
30006d1a:	2005      	movs	r0, #5
30006d1c:	f001 fcfa 	bl	30008714 <flash_get_layout_info>
30006d20:	e9d4 2300 	ldrd	r2, r3, [r4]
30006d24:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30006d28:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006d2c:	e9c4 2302 	strd	r2, r3, [r4, #8]
30006d30:	bd10      	pop	{r4, pc}
30006d32:	bf00      	nop
30006d34:	30010280 	.word	0x30010280

30006d38 <BOOT_OTA_IMG2>:
30006d38:	f7ff beca 	b.w	30006ad0 <BOOT_OTA_LoadIMG2>

30006d3c <BOOT_OTA_AP>:
30006d3c:	b5f0      	push	{r4, r5, r6, r7, lr}
30006d3e:	4a32      	ldr	r2, [pc, #200]	; (30006e08 <BOOT_OTA_AP+0xcc>)
30006d40:	b0eb      	sub	sp, #428	; 0x1ac
30006d42:	4b32      	ldr	r3, [pc, #200]	; (30006e0c <BOOT_OTA_AP+0xd0>)
30006d44:	6815      	ldr	r5, [r2, #0]
30006d46:	ac02      	add	r4, sp, #8
30006d48:	4e31      	ldr	r6, [pc, #196]	; (30006e10 <BOOT_OTA_AP+0xd4>)
30006d4a:	f505 5780 	add.w	r7, r5, #4096	; 0x1000
30006d4e:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006d50:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006d54:	4629      	mov	r1, r5
30006d56:	4d2f      	ldr	r5, [pc, #188]	; (30006e14 <BOOT_OTA_AP+0xd8>)
30006d58:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006d5c:	a806      	add	r0, sp, #24
30006d5e:	f7fd fdfd 	bl	3000495c <BOOT_ImgCopy>
30006d62:	4a2d      	ldr	r2, [pc, #180]	; (30006e18 <BOOT_OTA_AP+0xdc>)
30006d64:	463b      	mov	r3, r7
30006d66:	4629      	mov	r1, r5
30006d68:	2003      	movs	r0, #3
30006d6a:	47b0      	blx	r6
30006d6c:	2101      	movs	r1, #1
30006d6e:	4b2b      	ldr	r3, [pc, #172]	; (30006e1c <BOOT_OTA_AP+0xe0>)
30006d70:	2003      	movs	r0, #3
30006d72:	4798      	blx	r3
30006d74:	2201      	movs	r2, #1
30006d76:	4928      	ldr	r1, [pc, #160]	; (30006e18 <BOOT_OTA_AP+0xdc>)
30006d78:	2303      	movs	r3, #3
30006d7a:	4628      	mov	r0, r5
30006d7c:	f7ff fe5e 	bl	30006a3c <BOOT_OTFCheck>
30006d80:	4b27      	ldr	r3, [pc, #156]	; (30006e20 <BOOT_OTA_AP+0xe4>)
30006d82:	4798      	blx	r3
30006d84:	4623      	mov	r3, r4
30006d86:	2201      	movs	r2, #1
30006d88:	4c26      	ldr	r4, [pc, #152]	; (30006e24 <BOOT_OTA_AP+0xe8>)
30006d8a:	2800      	cmp	r0, #0
30006d8c:	bf14      	ite	ne
30006d8e:	4629      	movne	r1, r5
30006d90:	4639      	moveq	r1, r7
30006d92:	9200      	str	r2, [sp, #0]
30006d94:	4620      	mov	r0, r4
30006d96:	2204      	movs	r2, #4
30006d98:	f7ff fbfe 	bl	30006598 <BOOT_LoadSubImage>
30006d9c:	2801      	cmp	r0, #1
30006d9e:	d122      	bne.n	30006de6 <BOOT_OTA_AP+0xaa>
30006da0:	2302      	movs	r3, #2
30006da2:	4605      	mov	r5, r0
30006da4:	2204      	movs	r2, #4
30006da6:	a806      	add	r0, sp, #24
30006da8:	9300      	str	r3, [sp, #0]
30006daa:	4621      	mov	r1, r4
30006dac:	4b1e      	ldr	r3, [pc, #120]	; (30006e28 <BOOT_OTA_AP+0xec>)
30006dae:	f000 facd 	bl	3000734c <BOOT_SignatureCheck>
30006db2:	4606      	mov	r6, r0
30006db4:	bb18      	cbnz	r0, 30006dfe <BOOT_OTA_AP+0xc2>
30006db6:	68e3      	ldr	r3, [r4, #12]
30006db8:	462a      	mov	r2, r5
30006dba:	6861      	ldr	r1, [r4, #4]
30006dbc:	f104 0018 	add.w	r0, r4, #24
30006dc0:	4419      	add	r1, r3
30006dc2:	6963      	ldr	r3, [r4, #20]
30006dc4:	4419      	add	r1, r3
30006dc6:	4439      	add	r1, r7
30006dc8:	f7ff fc9a 	bl	30006700 <BOOT_LoadAPSubImage>
30006dcc:	2801      	cmp	r0, #1
30006dce:	d102      	bne.n	30006dd6 <BOOT_OTA_AP+0x9a>
30006dd0:	4630      	mov	r0, r6
30006dd2:	b06b      	add	sp, #428	; 0x1ac
30006dd4:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006dd6:	4b15      	ldr	r3, [pc, #84]	; (30006e2c <BOOT_OTA_AP+0xf0>)
30006dd8:	681b      	ldr	r3, [r3, #0]
30006dda:	079b      	lsls	r3, r3, #30
30006ddc:	d40a      	bmi.n	30006df4 <BOOT_OTA_AP+0xb8>
30006dde:	26ff      	movs	r6, #255	; 0xff
30006de0:	4630      	mov	r0, r6
30006de2:	b06b      	add	sp, #428	; 0x1ac
30006de4:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006de6:	26ff      	movs	r6, #255	; 0xff
30006de8:	4811      	ldr	r0, [pc, #68]	; (30006e30 <BOOT_OTA_AP+0xf4>)
30006dea:	f005 fc59 	bl	3000c6a0 <__DiagPrintf_veneer>
30006dee:	4630      	mov	r0, r6
30006df0:	b06b      	add	sp, #428	; 0x1ac
30006df2:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006df4:	480f      	ldr	r0, [pc, #60]	; (30006e34 <BOOT_OTA_AP+0xf8>)
30006df6:	26ff      	movs	r6, #255	; 0xff
30006df8:	f005 fc52 	bl	3000c6a0 <__DiagPrintf_veneer>
30006dfc:	e7e8      	b.n	30006dd0 <BOOT_OTA_AP+0x94>
30006dfe:	480e      	ldr	r0, [pc, #56]	; (30006e38 <BOOT_OTA_AP+0xfc>)
30006e00:	26ff      	movs	r6, #255	; 0xff
30006e02:	f005 fc4d 	bl	3000c6a0 <__DiagPrintf_veneer>
30006e06:	e7e3      	b.n	30006dd0 <BOOT_OTA_AP+0x94>
30006e08:	3000f038 	.word	0x3000f038
30006e0c:	3000e5e8 	.word	0x3000e5e8
30006e10:	30009c15 	.word	0x30009c15
30006e14:	0e000000 	.word	0x0e000000
30006e18:	0fffffe0 	.word	0x0fffffe0
30006e1c:	30009c55 	.word	0x30009c55
30006e20:	30009e75 	.word	0x30009e75
30006e24:	30010298 	.word	0x30010298
30006e28:	3000ff8c 	.word	0x3000ff8c
30006e2c:	2001c00c 	.word	0x2001c00c
30006e30:	3000d21c 	.word	0x3000d21c
30006e34:	3000d240 	.word	0x3000d240
30006e38:	3000d1f8 	.word	0x3000d1f8

30006e3c <BOOT_LoadRDPImg>:
30006e3c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006e40:	4d4e      	ldr	r5, [pc, #312]	; (30006f7c <BOOT_LoadRDPImg+0x140>)
30006e42:	b0d9      	sub	sp, #356	; 0x164
30006e44:	4613      	mov	r3, r2
30006e46:	468b      	mov	fp, r1
30006e48:	4604      	mov	r4, r0
30006e4a:	aa06      	add	r2, sp, #24
30006e4c:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30006e50:	e895 0003 	ldmia.w	r5, {r0, r1}
30006e54:	2500      	movs	r5, #0
30006e56:	e882 0003 	stmia.w	r2, {r0, r1}
30006e5a:	4619      	mov	r1, r3
30006e5c:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006e60:	4620      	mov	r0, r4
30006e62:	f7fd fd7b 	bl	3000495c <BOOT_ImgCopy>
30006e66:	ab06      	add	r3, sp, #24
30006e68:	2202      	movs	r2, #2
30006e6a:	4641      	mov	r1, r8
30006e6c:	4658      	mov	r0, fp
30006e6e:	9500      	str	r5, [sp, #0]
30006e70:	f7ff fb92 	bl	30006598 <BOOT_LoadSubImage>
30006e74:	2801      	cmp	r0, #1
30006e76:	d06f      	beq.n	30006f58 <BOOT_LoadRDPImg+0x11c>
30006e78:	4b41      	ldr	r3, [pc, #260]	; (30006f80 <BOOT_LoadRDPImg+0x144>)
30006e7a:	4628      	mov	r0, r5
30006e7c:	4798      	blx	r3
30006e7e:	2101      	movs	r1, #1
30006e80:	4b40      	ldr	r3, [pc, #256]	; (30006f84 <BOOT_LoadRDPImg+0x148>)
30006e82:	4628      	mov	r0, r5
30006e84:	4798      	blx	r3
30006e86:	4b40      	ldr	r3, [pc, #256]	; (30006f88 <BOOT_LoadRDPImg+0x14c>)
30006e88:	2120      	movs	r1, #32
30006e8a:	a810      	add	r0, sp, #64	; 0x40
30006e8c:	4798      	blx	r3
30006e8e:	f8df a110 	ldr.w	sl, [pc, #272]	; 30006fa0 <BOOT_LoadRDPImg+0x164>
30006e92:	f104 0120 	add.w	r1, r4, #32
30006e96:	2210      	movs	r2, #16
30006e98:	a80c      	add	r0, sp, #48	; 0x30
30006e9a:	af08      	add	r7, sp, #32
30006e9c:	47d0      	blx	sl
30006e9e:	f8df 9104 	ldr.w	r9, [pc, #260]	; 30006fa4 <BOOT_LoadRDPImg+0x168>
30006ea2:	ab06      	add	r3, sp, #24
30006ea4:	9504      	str	r5, [sp, #16]
30006ea6:	9305      	str	r3, [sp, #20]
30006ea8:	4641      	mov	r1, r8
30006eaa:	a818      	add	r0, sp, #96	; 0x60
30006eac:	2210      	movs	r2, #16
30006eae:	f7fd fd55 	bl	3000495c <BOOT_ImgCopy>
30006eb2:	2310      	movs	r3, #16
30006eb4:	aa0c      	add	r2, sp, #48	; 0x30
30006eb6:	a818      	add	r0, sp, #96	; 0x60
30006eb8:	4619      	mov	r1, r3
30006eba:	9700      	str	r7, [sp, #0]
30006ebc:	47c8      	blx	r9
30006ebe:	683b      	ldr	r3, [r7, #0]
30006ec0:	4a32      	ldr	r2, [pc, #200]	; (30006f8c <BOOT_LoadRDPImg+0x150>)
30006ec2:	4293      	cmp	r3, r2
30006ec4:	d14c      	bne.n	30006f60 <BOOT_LoadRDPImg+0x124>
30006ec6:	9a05      	ldr	r2, [sp, #20]
30006ec8:	687b      	ldr	r3, [r7, #4]
30006eca:	f852 1b04 	ldr.w	r1, [r2], #4
30006ece:	9205      	str	r2, [sp, #20]
30006ed0:	4a2f      	ldr	r2, [pc, #188]	; (30006f90 <BOOT_LoadRDPImg+0x154>)
30006ed2:	4293      	cmp	r3, r2
30006ed4:	d148      	bne.n	30006f68 <BOOT_LoadRDPImg+0x12c>
30006ed6:	9100      	str	r1, [sp, #0]
30006ed8:	2004      	movs	r0, #4
30006eda:	68fe      	ldr	r6, [r7, #12]
30006edc:	4b2d      	ldr	r3, [pc, #180]	; (30006f94 <BOOT_LoadRDPImg+0x158>)
30006ede:	3e20      	subs	r6, #32
30006ee0:	4a2d      	ldr	r2, [pc, #180]	; (30006f98 <BOOT_LoadRDPImg+0x15c>)
30006ee2:	4619      	mov	r1, r3
30006ee4:	9601      	str	r6, [sp, #4]
30006ee6:	68bd      	ldr	r5, [r7, #8]
30006ee8:	3520      	adds	r5, #32
30006eea:	9502      	str	r5, [sp, #8]
30006eec:	f005 f930 	bl	3000c150 <rtk_log_write>
30006ef0:	f1bb 0f00 	cmp.w	fp, #0
30006ef4:	d006      	beq.n	30006f04 <BOOT_LoadRDPImg+0xc8>
30006ef6:	9a04      	ldr	r2, [sp, #16]
30006ef8:	f10b 0304 	add.w	r3, fp, #4
30006efc:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
30006f00:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
30006f04:	b1dd      	cbz	r5, 30006f3e <BOOT_LoadRDPImg+0x102>
30006f06:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006f0a:	462c      	mov	r4, r5
30006f0c:	4641      	mov	r1, r8
30006f0e:	a818      	add	r0, sp, #96	; 0x60
30006f10:	bf28      	it	cs
30006f12:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006f16:	4622      	mov	r2, r4
30006f18:	44a0      	add	r8, r4
30006f1a:	f7fd fd1f 	bl	3000495c <BOOT_ImgCopy>
30006f1e:	aa0c      	add	r2, sp, #48	; 0x30
30006f20:	4621      	mov	r1, r4
30006f22:	2310      	movs	r3, #16
30006f24:	9600      	str	r6, [sp, #0]
30006f26:	a818      	add	r0, sp, #96	; 0x60
30006f28:	47c8      	blx	r9
30006f2a:	ab18      	add	r3, sp, #96	; 0x60
30006f2c:	f1a4 0110 	sub.w	r1, r4, #16
30006f30:	2210      	movs	r2, #16
30006f32:	a80c      	add	r0, sp, #48	; 0x30
30006f34:	4426      	add	r6, r4
30006f36:	4419      	add	r1, r3
30006f38:	47d0      	blx	sl
30006f3a:	1b2d      	subs	r5, r5, r4
30006f3c:	d1e3      	bne.n	30006f06 <BOOT_LoadRDPImg+0xca>
30006f3e:	9b04      	ldr	r3, [sp, #16]
30006f40:	b13b      	cbz	r3, 30006f52 <BOOT_LoadRDPImg+0x116>
30006f42:	2100      	movs	r1, #0
30006f44:	4b0f      	ldr	r3, [pc, #60]	; (30006f84 <BOOT_LoadRDPImg+0x148>)
30006f46:	4608      	mov	r0, r1
30006f48:	4798      	blx	r3
30006f4a:	2002      	movs	r0, #2
30006f4c:	b059      	add	sp, #356	; 0x164
30006f4e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006f52:	2301      	movs	r3, #1
30006f54:	9304      	str	r3, [sp, #16]
30006f56:	e7a7      	b.n	30006ea8 <BOOT_LoadRDPImg+0x6c>
30006f58:	2002      	movs	r0, #2
30006f5a:	b059      	add	sp, #356	; 0x164
30006f5c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006f60:	9b04      	ldr	r3, [sp, #16]
30006f62:	aa06      	add	r2, sp, #24
30006f64:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30006f68:	4b0a      	ldr	r3, [pc, #40]	; (30006f94 <BOOT_LoadRDPImg+0x158>)
30006f6a:	2004      	movs	r0, #4
30006f6c:	9100      	str	r1, [sp, #0]
30006f6e:	4a0b      	ldr	r2, [pc, #44]	; (30006f9c <BOOT_LoadRDPImg+0x160>)
30006f70:	4619      	mov	r1, r3
30006f72:	f005 f8ed 	bl	3000c150 <rtk_log_write>
30006f76:	2000      	movs	r0, #0
30006f78:	e7e8      	b.n	30006f4c <BOOT_LoadRDPImg+0x110>
30006f7a:	bf00      	nop
30006f7c:	3000e5f8 	.word	0x3000e5f8
30006f80:	00002429 	.word	0x00002429
30006f84:	00002479 	.word	0x00002479
30006f88:	00004641 	.word	0x00004641
30006f8c:	35393138 	.word	0x35393138
30006f90:	31313738 	.word	0x31313738
30006f94:	3000c810 	.word	0x3000c810
30006f98:	3000d0e8 	.word	0x3000d0e8
30006f9c:	3000d0d4 	.word	0x3000d0d4
30006fa0:	00012a1d 	.word	0x00012a1d
30006fa4:	000046f5 	.word	0x000046f5

30006fa8 <BOOT_LoadBL32RDPImg>:
30006fa8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006fac:	4b35      	ldr	r3, [pc, #212]	; (30007084 <BOOT_LoadBL32RDPImg+0xdc>)
30006fae:	b0d1      	sub	sp, #324	; 0x144
30006fb0:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 300070a0 <BOOT_LoadBL32RDPImg+0xf8>
30006fb4:	4607      	mov	r7, r0
30006fb6:	f10d 0810 	add.w	r8, sp, #16
30006fba:	460e      	mov	r6, r1
30006fbc:	4615      	mov	r5, r2
30006fbe:	ac02      	add	r4, sp, #8
30006fc0:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 300070a4 <BOOT_LoadBL32RDPImg+0xfc>
30006fc4:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 300070a8 <BOOT_LoadBL32RDPImg+0x100>
30006fc8:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006fca:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
30006fce:	e89c 0003 	ldmia.w	ip, {r0, r1}
30006fd2:	e884 0003 	stmia.w	r4, {r0, r1}
30006fd6:	2000      	movs	r0, #0
30006fd8:	47c8      	blx	r9
30006fda:	2101      	movs	r1, #1
30006fdc:	2000      	movs	r0, #0
30006fde:	47d0      	blx	sl
30006fe0:	2120      	movs	r1, #32
30006fe2:	4b29      	ldr	r3, [pc, #164]	; (30007088 <BOOT_LoadBL32RDPImg+0xe0>)
30006fe4:	eb0d 0001 	add.w	r0, sp, r1
30006fe8:	4798      	blx	r3
30006fea:	4b28      	ldr	r3, [pc, #160]	; (3000708c <BOOT_LoadBL32RDPImg+0xe4>)
30006fec:	689b      	ldr	r3, [r3, #8]
30006fee:	079b      	lsls	r3, r3, #30
30006ff0:	d436      	bmi.n	30007060 <BOOT_LoadBL32RDPImg+0xb8>
30006ff2:	b1fd      	cbz	r5, 30007034 <BOOT_LoadBL32RDPImg+0x8c>
30006ff4:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 300070ac <BOOT_LoadBL32RDPImg+0x104>
30006ff8:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 300070b0 <BOOT_LoadBL32RDPImg+0x108>
30006ffc:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30007000:	462c      	mov	r4, r5
30007002:	4639      	mov	r1, r7
30007004:	a810      	add	r0, sp, #64	; 0x40
30007006:	bf28      	it	cs
30007008:	f44f 7480 	movcs.w	r4, #256	; 0x100
3000700c:	4622      	mov	r2, r4
3000700e:	4427      	add	r7, r4
30007010:	f7fd fca4 	bl	3000495c <BOOT_ImgCopy>
30007014:	4642      	mov	r2, r8
30007016:	4621      	mov	r1, r4
30007018:	2310      	movs	r3, #16
3000701a:	9600      	str	r6, [sp, #0]
3000701c:	a810      	add	r0, sp, #64	; 0x40
3000701e:	47d8      	blx	fp
30007020:	ab10      	add	r3, sp, #64	; 0x40
30007022:	f1a4 0110 	sub.w	r1, r4, #16
30007026:	2210      	movs	r2, #16
30007028:	4640      	mov	r0, r8
3000702a:	4426      	add	r6, r4
3000702c:	4419      	add	r1, r3
3000702e:	47c8      	blx	r9
30007030:	1b2d      	subs	r5, r5, r4
30007032:	d1e3      	bne.n	30006ffc <BOOT_LoadBL32RDPImg+0x54>
30007034:	f856 2c20 	ldr.w	r2, [r6, #-32]
30007038:	3e20      	subs	r6, #32
3000703a:	4b15      	ldr	r3, [pc, #84]	; (30007090 <BOOT_LoadBL32RDPImg+0xe8>)
3000703c:	429a      	cmp	r2, r3
3000703e:	d108      	bne.n	30007052 <BOOT_LoadBL32RDPImg+0xaa>
30007040:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30007044:	6872      	ldr	r2, [r6, #4]
30007046:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
3000704a:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
3000704e:	429a      	cmp	r2, r3
30007050:	d00d      	beq.n	3000706e <BOOT_LoadBL32RDPImg+0xc6>
30007052:	4810      	ldr	r0, [pc, #64]	; (30007094 <BOOT_LoadBL32RDPImg+0xec>)
30007054:	f005 fb24 	bl	3000c6a0 <__DiagPrintf_veneer>
30007058:	2000      	movs	r0, #0
3000705a:	b051      	add	sp, #324	; 0x144
3000705c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007060:	4621      	mov	r1, r4
30007062:	462b      	mov	r3, r5
30007064:	4632      	mov	r2, r6
30007066:	480c      	ldr	r0, [pc, #48]	; (30007098 <BOOT_LoadBL32RDPImg+0xf0>)
30007068:	f005 fb1a 	bl	3000c6a0 <__DiagPrintf_veneer>
3000706c:	e7c1      	b.n	30006ff2 <BOOT_LoadBL32RDPImg+0x4a>
3000706e:	480b      	ldr	r0, [pc, #44]	; (3000709c <BOOT_LoadBL32RDPImg+0xf4>)
30007070:	f005 fb16 	bl	3000c6a0 <__DiagPrintf_veneer>
30007074:	2100      	movs	r1, #0
30007076:	4608      	mov	r0, r1
30007078:	47d0      	blx	sl
3000707a:	2001      	movs	r0, #1
3000707c:	b051      	add	sp, #324	; 0x144
3000707e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007082:	bf00      	nop
30007084:	3000d2e8 	.word	0x3000d2e8
30007088:	00004641 	.word	0x00004641
3000708c:	2001c00c 	.word	0x2001c00c
30007090:	35393138 	.word	0x35393138
30007094:	3000d2cc 	.word	0x3000d2cc
30007098:	3000d2a4 	.word	0x3000d2a4
3000709c:	3000d2d8 	.word	0x3000d2d8
300070a0:	3000d2fc 	.word	0x3000d2fc
300070a4:	00002429 	.word	0x00002429
300070a8:	00002479 	.word	0x00002479
300070ac:	000046f5 	.word	0x000046f5
300070b0:	00012a1d 	.word	0x00012a1d

300070b4 <_BOOT_Validate_ImgHash>:
300070b4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300070b8:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
300070bc:	4605      	mov	r5, r0
300070be:	4690      	mov	r8, r2
300070c0:	22e0      	movs	r2, #224	; 0xe0
300070c2:	9103      	str	r1, [sp, #12]
300070c4:	2100      	movs	r1, #0
300070c6:	a818      	add	r0, sp, #96	; 0x60
300070c8:	461c      	mov	r4, r3
300070ca:	f88d 101f 	strb.w	r1, [sp, #31]
300070ce:	f005 fa8b 	bl	3000c5e8 <____wrap_memset_veneer>
300070d2:	4628      	mov	r0, r5
300070d4:	f10d 021f 	add.w	r2, sp, #31
300070d8:	f10d 011e 	add.w	r1, sp, #30
300070dc:	f002 ff00 	bl	30009ee0 <SBOOT_GetMdType>
300070e0:	9001      	str	r0, [sp, #4]
300070e2:	2800      	cmp	r0, #0
300070e4:	d166      	bne.n	300071b4 <_BOOT_Validate_ImgHash+0x100>
300070e6:	f89d 301f 	ldrb.w	r3, [sp, #31]
300070ea:	2b00      	cmp	r3, #0
300070ec:	d067      	beq.n	300071be <_BOOT_Validate_ImgHash+0x10a>
300070ee:	2101      	movs	r1, #1
300070f0:	4605      	mov	r5, r0
300070f2:	4b80      	ldr	r3, [pc, #512]	; (300072f4 <_BOOT_Validate_ImgHash+0x240>)
300070f4:	4608      	mov	r0, r1
300070f6:	4798      	blx	r3
300070f8:	4629      	mov	r1, r5
300070fa:	ab18      	add	r3, sp, #96	; 0x60
300070fc:	2220      	movs	r2, #32
300070fe:	f89d 001e 	ldrb.w	r0, [sp, #30]
30007102:	4d7d      	ldr	r5, [pc, #500]	; (300072f8 <_BOOT_Validate_ImgHash+0x244>)
30007104:	47a8      	blx	r5
30007106:	2800      	cmp	r0, #0
30007108:	f040 808b 	bne.w	30007222 <_BOOT_Validate_ImgHash+0x16e>
3000710c:	b32c      	cbz	r4, 3000715a <_BOOT_Validate_ImgHash+0xa6>
3000710e:	aad0      	add	r2, sp, #832	; 0x340
30007110:	1e63      	subs	r3, r4, #1
30007112:	ae50      	add	r6, sp, #320	; 0x140
30007114:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 30007310 <_BOOT_Validate_ImgHash+0x25c>
30007118:	f012 0218 	ands.w	r2, r2, #24
3000711c:	b2db      	uxtb	r3, r3
3000711e:	f026 091f 	bic.w	r9, r6, #31
30007122:	4d76      	ldr	r5, [pc, #472]	; (300072fc <_BOOT_Validate_ImgHash+0x248>)
30007124:	9204      	str	r2, [sp, #16]
30007126:	f108 0208 	add.w	r2, r8, #8
3000712a:	bf08      	it	eq
3000712c:	46b1      	moveq	r9, r6
3000712e:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30007132:	9302      	str	r3, [sp, #8]
30007134:	e9d8 b700 	ldrd	fp, r7, [r8]
30007138:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
3000713c:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30007140:	f0c0 80a7 	bcc.w	30007292 <_BOOT_Validate_ImgHash+0x1de>
30007144:	4639      	mov	r1, r7
30007146:	4658      	mov	r0, fp
30007148:	aa18      	add	r2, sp, #96	; 0x60
3000714a:	47d0      	blx	sl
3000714c:	2800      	cmp	r0, #0
3000714e:	d168      	bne.n	30007222 <_BOOT_Validate_ImgHash+0x16e>
30007150:	f108 0808 	add.w	r8, r8, #8
30007154:	9b02      	ldr	r3, [sp, #8]
30007156:	4543      	cmp	r3, r8
30007158:	d1ec      	bne.n	30007134 <_BOOT_Validate_ImgHash+0x80>
3000715a:	ac08      	add	r4, sp, #32
3000715c:	a918      	add	r1, sp, #96	; 0x60
3000715e:	4b68      	ldr	r3, [pc, #416]	; (30007300 <_BOOT_Validate_ImgHash+0x24c>)
30007160:	4620      	mov	r0, r4
30007162:	4798      	blx	r3
30007164:	4601      	mov	r1, r0
30007166:	2800      	cmp	r0, #0
30007168:	d15b      	bne.n	30007222 <_BOOT_Validate_ImgHash+0x16e>
3000716a:	2001      	movs	r0, #1
3000716c:	4b61      	ldr	r3, [pc, #388]	; (300072f4 <_BOOT_Validate_ImgHash+0x240>)
3000716e:	4798      	blx	r3
30007170:	f89d 501e 	ldrb.w	r5, [sp, #30]
30007174:	4b63      	ldr	r3, [pc, #396]	; (30007304 <_BOOT_Validate_ImgHash+0x250>)
30007176:	4798      	blx	r3
30007178:	fbb0 f3f5 	udiv	r3, r0, r5
3000717c:	fb03 0315 	mls	r3, r3, r5, r0
30007180:	b2db      	uxtb	r3, r3
30007182:	b1bd      	cbz	r5, 300071b4 <_BOOT_Validate_ImgHash+0x100>
30007184:	2000      	movs	r0, #0
30007186:	9f03      	ldr	r7, [sp, #12]
30007188:	4606      	mov	r6, r0
3000718a:	1c5a      	adds	r2, r3, #1
3000718c:	5cf9      	ldrb	r1, [r7, r3]
3000718e:	5ce3      	ldrb	r3, [r4, r3]
30007190:	3001      	adds	r0, #1
30007192:	b2d2      	uxtb	r2, r2
30007194:	404b      	eors	r3, r1
30007196:	431e      	orrs	r6, r3
30007198:	fbb2 f3f5 	udiv	r3, r2, r5
3000719c:	fb05 2213 	mls	r2, r5, r3, r2
300071a0:	b2d3      	uxtb	r3, r2
300071a2:	b2c2      	uxtb	r2, r0
300071a4:	4295      	cmp	r5, r2
300071a6:	d8f0      	bhi.n	3000718a <_BOOT_Validate_ImgHash+0xd6>
300071a8:	2e00      	cmp	r6, #0
300071aa:	9b01      	ldr	r3, [sp, #4]
300071ac:	bf18      	it	ne
300071ae:	f06f 0306 	mvnne.w	r3, #6
300071b2:	9301      	str	r3, [sp, #4]
300071b4:	9801      	ldr	r0, [sp, #4]
300071b6:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300071ba:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300071be:	a918      	add	r1, sp, #96	; 0x60
300071c0:	f89d 001e 	ldrb.w	r0, [sp, #30]
300071c4:	4b50      	ldr	r3, [pc, #320]	; (30007308 <_BOOT_Validate_ImgHash+0x254>)
300071c6:	4798      	blx	r3
300071c8:	bb58      	cbnz	r0, 30007222 <_BOOT_Validate_ImgHash+0x16e>
300071ca:	b31c      	cbz	r4, 30007214 <_BOOT_Validate_ImgHash+0x160>
300071cc:	aad0      	add	r2, sp, #832	; 0x340
300071ce:	1e63      	subs	r3, r4, #1
300071d0:	ae50      	add	r6, sp, #320	; 0x140
300071d2:	f8df a140 	ldr.w	sl, [pc, #320]	; 30007314 <_BOOT_Validate_ImgHash+0x260>
300071d6:	f012 0218 	ands.w	r2, r2, #24
300071da:	b2db      	uxtb	r3, r3
300071dc:	f026 091f 	bic.w	r9, r6, #31
300071e0:	4d46      	ldr	r5, [pc, #280]	; (300072fc <_BOOT_Validate_ImgHash+0x248>)
300071e2:	9204      	str	r2, [sp, #16]
300071e4:	f108 0208 	add.w	r2, r8, #8
300071e8:	bf08      	it	eq
300071ea:	46b1      	moveq	r9, r6
300071ec:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300071f0:	9302      	str	r3, [sp, #8]
300071f2:	e9d8 b700 	ldrd	fp, r7, [r8]
300071f6:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300071fa:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300071fe:	d318      	bcc.n	30007232 <_BOOT_Validate_ImgHash+0x17e>
30007200:	4639      	mov	r1, r7
30007202:	4658      	mov	r0, fp
30007204:	aa18      	add	r2, sp, #96	; 0x60
30007206:	47d0      	blx	sl
30007208:	b958      	cbnz	r0, 30007222 <_BOOT_Validate_ImgHash+0x16e>
3000720a:	f108 0808 	add.w	r8, r8, #8
3000720e:	9b02      	ldr	r3, [sp, #8]
30007210:	4543      	cmp	r3, r8
30007212:	d1ee      	bne.n	300071f2 <_BOOT_Validate_ImgHash+0x13e>
30007214:	ac08      	add	r4, sp, #32
30007216:	a918      	add	r1, sp, #96	; 0x60
30007218:	4b3c      	ldr	r3, [pc, #240]	; (3000730c <_BOOT_Validate_ImgHash+0x258>)
3000721a:	4620      	mov	r0, r4
3000721c:	4798      	blx	r3
3000721e:	2800      	cmp	r0, #0
30007220:	d0a6      	beq.n	30007170 <_BOOT_Validate_ImgHash+0xbc>
30007222:	f06f 030a 	mvn.w	r3, #10
30007226:	9301      	str	r3, [sp, #4]
30007228:	9801      	ldr	r0, [sp, #4]
3000722a:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
3000722e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007232:	2f00      	cmp	r7, #0
30007234:	d0e9      	beq.n	3000720a <_BOOT_Validate_ImgHash+0x156>
30007236:	9c04      	ldr	r4, [sp, #16]
30007238:	f8cd 8014 	str.w	r8, [sp, #20]
3000723c:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30007240:	46b8      	mov	r8, r7
30007242:	4659      	mov	r1, fp
30007244:	4630      	mov	r0, r6
30007246:	bf28      	it	cs
30007248:	f44f 7800 	movcs.w	r8, #512	; 0x200
3000724c:	4642      	mov	r2, r8
3000724e:	f7fd fb85 	bl	3000495c <BOOT_ImgCopy>
30007252:	696a      	ldr	r2, [r5, #20]
30007254:	03d3      	lsls	r3, r2, #15
30007256:	d50f      	bpl.n	30007278 <_BOOT_Validate_ImgHash+0x1c4>
30007258:	eb08 0004 	add.w	r0, r8, r4
3000725c:	f3bf 8f4f 	dsb	sy
30007260:	4448      	add	r0, r9
30007262:	464a      	mov	r2, r9
30007264:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30007268:	3220      	adds	r2, #32
3000726a:	1a83      	subs	r3, r0, r2
3000726c:	2b00      	cmp	r3, #0
3000726e:	dcf9      	bgt.n	30007264 <_BOOT_Validate_ImgHash+0x1b0>
30007270:	f3bf 8f4f 	dsb	sy
30007274:	f3bf 8f6f 	isb	sy
30007278:	aa18      	add	r2, sp, #96	; 0x60
3000727a:	4641      	mov	r1, r8
3000727c:	4630      	mov	r0, r6
3000727e:	47d0      	blx	sl
30007280:	2800      	cmp	r0, #0
30007282:	d1ce      	bne.n	30007222 <_BOOT_Validate_ImgHash+0x16e>
30007284:	ebb7 0708 	subs.w	r7, r7, r8
30007288:	44c3      	add	fp, r8
3000728a:	d1d7      	bne.n	3000723c <_BOOT_Validate_ImgHash+0x188>
3000728c:	f8dd 8014 	ldr.w	r8, [sp, #20]
30007290:	e7bb      	b.n	3000720a <_BOOT_Validate_ImgHash+0x156>
30007292:	2f00      	cmp	r7, #0
30007294:	f43f af5c 	beq.w	30007150 <_BOOT_Validate_ImgHash+0x9c>
30007298:	9c04      	ldr	r4, [sp, #16]
3000729a:	f8cd 8014 	str.w	r8, [sp, #20]
3000729e:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
300072a2:	46b8      	mov	r8, r7
300072a4:	4659      	mov	r1, fp
300072a6:	4630      	mov	r0, r6
300072a8:	bf28      	it	cs
300072aa:	f44f 7800 	movcs.w	r8, #512	; 0x200
300072ae:	4642      	mov	r2, r8
300072b0:	f7fd fb54 	bl	3000495c <BOOT_ImgCopy>
300072b4:	696a      	ldr	r2, [r5, #20]
300072b6:	03d2      	lsls	r2, r2, #15
300072b8:	d50f      	bpl.n	300072da <_BOOT_Validate_ImgHash+0x226>
300072ba:	eb08 0004 	add.w	r0, r8, r4
300072be:	f3bf 8f4f 	dsb	sy
300072c2:	4448      	add	r0, r9
300072c4:	464a      	mov	r2, r9
300072c6:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
300072ca:	3220      	adds	r2, #32
300072cc:	1a83      	subs	r3, r0, r2
300072ce:	2b00      	cmp	r3, #0
300072d0:	dcf9      	bgt.n	300072c6 <_BOOT_Validate_ImgHash+0x212>
300072d2:	f3bf 8f4f 	dsb	sy
300072d6:	f3bf 8f6f 	isb	sy
300072da:	aa18      	add	r2, sp, #96	; 0x60
300072dc:	4641      	mov	r1, r8
300072de:	4630      	mov	r0, r6
300072e0:	47d0      	blx	sl
300072e2:	2800      	cmp	r0, #0
300072e4:	d19d      	bne.n	30007222 <_BOOT_Validate_ImgHash+0x16e>
300072e6:	ebb7 0708 	subs.w	r7, r7, r8
300072ea:	44c3      	add	fp, r8
300072ec:	d1d7      	bne.n	3000729e <_BOOT_Validate_ImgHash+0x1ea>
300072ee:	f8dd 8014 	ldr.w	r8, [sp, #20]
300072f2:	e72d      	b.n	30007150 <_BOOT_Validate_ImgHash+0x9c>
300072f4:	30009f1d 	.word	0x30009f1d
300072f8:	300097dd 	.word	0x300097dd
300072fc:	e000ed00 	.word	0xe000ed00
30007300:	30009f99 	.word	0x30009f99
30007304:	0000ea4d 	.word	0x0000ea4d
30007308:	30009fd9 	.word	0x30009fd9
3000730c:	3000a055 	.word	0x3000a055
30007310:	30009f5d 	.word	0x30009f5d
30007314:	3000a019 	.word	0x3000a019

30007318 <BOOT_Validate_ImgHash>:
30007318:	b530      	push	{r4, r5, lr}
3000731a:	b085      	sub	sp, #20
3000731c:	4d0a      	ldr	r5, [pc, #40]	; (30007348 <BOOT_Validate_ImgHash+0x30>)
3000731e:	4604      	mov	r4, r0
30007320:	9301      	str	r3, [sp, #4]
30007322:	e9cd 2102 	strd	r2, r1, [sp, #8]
30007326:	47a8      	blx	r5
30007328:	9903      	ldr	r1, [sp, #12]
3000732a:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000732e:	b128      	cbz	r0, 3000733c <BOOT_Validate_ImgHash+0x24>
30007330:	4620      	mov	r0, r4
30007332:	b005      	add	sp, #20
30007334:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30007338:	f002 bef0 	b.w	3000a11c <SBOOT_Validate_ImgHash>
3000733c:	4620      	mov	r0, r4
3000733e:	b005      	add	sp, #20
30007340:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30007344:	f7ff beb6 	b.w	300070b4 <_BOOT_Validate_ImgHash>
30007348:	30009e75 	.word	0x30009e75

3000734c <BOOT_SignatureCheck>:
3000734c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007350:	461e      	mov	r6, r3
30007352:	b089      	sub	sp, #36	; 0x24
30007354:	4b79      	ldr	r3, [pc, #484]	; (3000753c <BOOT_SignatureCheck+0x1f0>)
30007356:	4680      	mov	r8, r0
30007358:	4689      	mov	r9, r1
3000735a:	4617      	mov	r7, r2
3000735c:	f10d 0c20 	add.w	ip, sp, #32
30007360:	4c77      	ldr	r4, [pc, #476]	; (30007540 <BOOT_SignatureCheck+0x1f4>)
30007362:	9d10      	ldr	r5, [sp, #64]	; 0x40
30007364:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30007366:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
3000736a:	47a0      	blx	r4
3000736c:	b910      	cbnz	r0, 30007374 <BOOT_SignatureCheck+0x28>
3000736e:	b009      	add	sp, #36	; 0x24
30007370:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007374:	4b73      	ldr	r3, [pc, #460]	; (30007544 <BOOT_SignatureCheck+0x1f8>)
30007376:	2000      	movs	r0, #0
30007378:	4798      	blx	r3
3000737a:	f898 3013 	ldrb.w	r3, [r8, #19]
3000737e:	f898 2012 	ldrb.w	r2, [r8, #18]
30007382:	f10d 010f 	add.w	r1, sp, #15
30007386:	f10d 000e 	add.w	r0, sp, #14
3000738a:	f002 ff01 	bl	3000a190 <SBOOT_Validate_Algorithm>
3000738e:	4604      	mov	r4, r0
30007390:	b9e8      	cbnz	r0, 300073ce <BOOT_SignatureCheck+0x82>
30007392:	69f1      	ldr	r1, [r6, #28]
30007394:	b919      	cbnz	r1, 3000739e <BOOT_SignatureCheck+0x52>
30007396:	e0cf      	b.n	30007538 <BOOT_SignatureCheck+0x1ec>
30007398:	b2d0      	uxtb	r0, r2
3000739a:	4288      	cmp	r0, r1
3000739c:	d208      	bcs.n	300073b0 <BOOT_SignatureCheck+0x64>
3000739e:	eb00 04c0 	add.w	r4, r0, r0, lsl #3
300073a2:	1c42      	adds	r2, r0, #1
300073a4:	eb06 0484 	add.w	r4, r6, r4, lsl #2
300073a8:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
300073ac:	42ab      	cmp	r3, r5
300073ae:	d1f3      	bne.n	30007398 <BOOT_SignatureCheck+0x4c>
300073b0:	2224      	movs	r2, #36	; 0x24
300073b2:	23b4      	movs	r3, #180	; 0xb4
300073b4:	fb12 3200 	smlabb	r2, r2, r0, r3
300073b8:	f108 0570 	add.w	r5, r8, #112	; 0x70
300073bc:	4432      	add	r2, r6
300073be:	f89d 000e 	ldrb.w	r0, [sp, #14]
300073c2:	4629      	mov	r1, r5
300073c4:	f002 fe66 	bl	3000a094 <SBOOT_Validate_PubKey>
300073c8:	4604      	mov	r4, r0
300073ca:	2800      	cmp	r0, #0
300073cc:	d07a      	beq.n	300074c4 <BOOT_SignatureCheck+0x178>
300073ce:	f898 3011 	ldrb.w	r3, [r8, #17]
300073d2:	aa08      	add	r2, sp, #32
300073d4:	eb02 0383 	add.w	r3, r2, r3, lsl #2
300073d8:	f853 1c10 	ldr.w	r1, [r3, #-16]
300073dc:	4b5a      	ldr	r3, [pc, #360]	; (30007548 <BOOT_SignatureCheck+0x1fc>)
300073de:	2002      	movs	r0, #2
300073e0:	4a5a      	ldr	r2, [pc, #360]	; (3000754c <BOOT_SignatureCheck+0x200>)
300073e2:	e9cd 1400 	strd	r1, r4, [sp]
300073e6:	4619      	mov	r1, r3
300073e8:	f004 feb2 	bl	3000c150 <rtk_log_write>
300073ec:	2f00      	cmp	r7, #0
300073ee:	d045      	beq.n	3000747c <BOOT_SignatureCheck+0x130>
300073f0:	464d      	mov	r5, r9
300073f2:	f107 39ff 	add.w	r9, r7, #4294967295	; 0xffffffff
300073f6:	f04f 0800 	mov.w	r8, #0
300073fa:	4f55      	ldr	r7, [pc, #340]	; (30007550 <BOOT_SignatureCheck+0x204>)
300073fc:	fa5f f389 	uxtb.w	r3, r9
30007400:	f105 0908 	add.w	r9, r5, #8
30007404:	1d2e      	adds	r6, r5, #4
30007406:	4c53      	ldr	r4, [pc, #332]	; (30007554 <BOOT_SignatureCheck+0x208>)
30007408:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
3000740c:	e003      	b.n	30007416 <BOOT_SignatureCheck+0xca>
3000740e:	3508      	adds	r5, #8
30007410:	3608      	adds	r6, #8
30007412:	45a9      	cmp	r9, r5
30007414:	d032      	beq.n	3000747c <BOOT_SignatureCheck+0x130>
30007416:	f856 0c04 	ldr.w	r0, [r6, #-4]
3000741a:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
3000741e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30007422:	d3f4      	bcc.n	3000740e <BOOT_SignatureCheck+0xc2>
30007424:	2100      	movs	r1, #0
30007426:	686a      	ldr	r2, [r5, #4]
30007428:	47b8      	blx	r7
3000742a:	6963      	ldr	r3, [r4, #20]
3000742c:	03d9      	lsls	r1, r3, #15
3000742e:	d5ee      	bpl.n	3000740e <BOOT_SignatureCheck+0xc2>
30007430:	f856 2c04 	ldr.w	r2, [r6, #-4]
30007434:	686b      	ldr	r3, [r5, #4]
30007436:	ea02 0103 	and.w	r1, r2, r3
3000743a:	3101      	adds	r1, #1
3000743c:	d022      	beq.n	30007484 <BOOT_SignatureCheck+0x138>
3000743e:	f012 011f 	ands.w	r1, r2, #31
30007442:	d002      	beq.n	3000744a <BOOT_SignatureCheck+0xfe>
30007444:	440b      	add	r3, r1
30007446:	f022 021f 	bic.w	r2, r2, #31
3000744a:	f3bf 8f4f 	dsb	sy
3000744e:	2b00      	cmp	r3, #0
30007450:	dd0c      	ble.n	3000746c <BOOT_SignatureCheck+0x120>
30007452:	1e59      	subs	r1, r3, #1
30007454:	f102 0320 	add.w	r3, r2, #32
30007458:	f021 011f 	bic.w	r1, r1, #31
3000745c:	4419      	add	r1, r3
3000745e:	428b      	cmp	r3, r1
30007460:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30007464:	461a      	mov	r2, r3
30007466:	f103 0320 	add.w	r3, r3, #32
3000746a:	d1f8      	bne.n	3000745e <BOOT_SignatureCheck+0x112>
3000746c:	f3bf 8f4f 	dsb	sy
30007470:	f3bf 8f6f 	isb	sy
30007474:	3508      	adds	r5, #8
30007476:	3608      	adds	r6, #8
30007478:	45a9      	cmp	r9, r5
3000747a:	d1cc      	bne.n	30007416 <BOOT_SignatureCheck+0xca>
3000747c:	20ff      	movs	r0, #255	; 0xff
3000747e:	b009      	add	sp, #36	; 0x24
30007480:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007484:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30007488:	f3bf 8f4f 	dsb	sy
3000748c:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30007490:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30007494:	f3c3 304e 	ubfx	r0, r3, #13, #15
30007498:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
3000749c:	0140      	lsls	r0, r0, #5
3000749e:	ea00 010e 	and.w	r1, r0, lr
300074a2:	4663      	mov	r3, ip
300074a4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300074a8:	3b01      	subs	r3, #1
300074aa:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
300074ae:	1c5a      	adds	r2, r3, #1
300074b0:	d1f8      	bne.n	300074a4 <BOOT_SignatureCheck+0x158>
300074b2:	3820      	subs	r0, #32
300074b4:	f110 0f20 	cmn.w	r0, #32
300074b8:	d1f1      	bne.n	3000749e <BOOT_SignatureCheck+0x152>
300074ba:	f3bf 8f4f 	dsb	sy
300074be:	f3bf 8f6f 	isb	sy
300074c2:	e7d7      	b.n	30007474 <BOOT_SignatureCheck+0x128>
300074c4:	f508 7380 	add.w	r3, r8, #256	; 0x100
300074c8:	462a      	mov	r2, r5
300074ca:	f89d 100f 	ldrb.w	r1, [sp, #15]
300074ce:	9301      	str	r3, [sp, #4]
300074d0:	f44f 7380 	mov.w	r3, #256	; 0x100
300074d4:	f89d 000e 	ldrb.w	r0, [sp, #14]
300074d8:	9300      	str	r3, [sp, #0]
300074da:	4643      	mov	r3, r8
300074dc:	f002 fdf8 	bl	3000a0d0 <SBOOT_Validate_Signature>
300074e0:	4604      	mov	r4, r0
300074e2:	2800      	cmp	r0, #0
300074e4:	f47f af73 	bne.w	300073ce <BOOT_SignatureCheck+0x82>
300074e8:	f108 0530 	add.w	r5, r8, #48	; 0x30
300074ec:	4b1a      	ldr	r3, [pc, #104]	; (30007558 <BOOT_SignatureCheck+0x20c>)
300074ee:	f89d 400f 	ldrb.w	r4, [sp, #15]
300074f2:	4798      	blx	r3
300074f4:	4629      	mov	r1, r5
300074f6:	b9c0      	cbnz	r0, 3000752a <BOOT_SignatureCheck+0x1de>
300074f8:	4620      	mov	r0, r4
300074fa:	463b      	mov	r3, r7
300074fc:	464a      	mov	r2, r9
300074fe:	f7ff fdd9 	bl	300070b4 <_BOOT_Validate_ImgHash>
30007502:	4604      	mov	r4, r0
30007504:	f898 3011 	ldrb.w	r3, [r8, #17]
30007508:	aa08      	add	r2, sp, #32
3000750a:	eb02 0383 	add.w	r3, r2, r3, lsl #2
3000750e:	f853 1c10 	ldr.w	r1, [r3, #-16]
30007512:	2c00      	cmp	r4, #0
30007514:	f47f af62 	bne.w	300073dc <BOOT_SignatureCheck+0x90>
30007518:	4b0b      	ldr	r3, [pc, #44]	; (30007548 <BOOT_SignatureCheck+0x1fc>)
3000751a:	2004      	movs	r0, #4
3000751c:	9100      	str	r1, [sp, #0]
3000751e:	4a0f      	ldr	r2, [pc, #60]	; (3000755c <BOOT_SignatureCheck+0x210>)
30007520:	4619      	mov	r1, r3
30007522:	f004 fe15 	bl	3000c150 <rtk_log_write>
30007526:	4620      	mov	r0, r4
30007528:	e721      	b.n	3000736e <BOOT_SignatureCheck+0x22>
3000752a:	4620      	mov	r0, r4
3000752c:	463b      	mov	r3, r7
3000752e:	464a      	mov	r2, r9
30007530:	f002 fdf4 	bl	3000a11c <SBOOT_Validate_ImgHash>
30007534:	4604      	mov	r4, r0
30007536:	e7e5      	b.n	30007504 <BOOT_SignatureCheck+0x1b8>
30007538:	22b4      	movs	r2, #180	; 0xb4
3000753a:	e73d      	b.n	300073b8 <BOOT_SignatureCheck+0x6c>
3000753c:	3000e600 	.word	0x3000e600
30007540:	0000c149 	.word	0x0000c149
30007544:	3000a159 	.word	0x3000a159
30007548:	3000c810 	.word	0x3000c810
3000754c:	3000d31c 	.word	0x3000d31c
30007550:	00012be5 	.word	0x00012be5
30007554:	e000ed00 	.word	0xe000ed00
30007558:	30009e75 	.word	0x30009e75
3000755c:	3000d304 	.word	0x3000d304

30007560 <BOOT_CertificateCheck>:
30007560:	b570      	push	{r4, r5, r6, lr}
30007562:	4b2b      	ldr	r3, [pc, #172]	; (30007610 <BOOT_CertificateCheck+0xb0>)
30007564:	b0b0      	sub	sp, #192	; 0xc0
30007566:	4604      	mov	r4, r0
30007568:	460d      	mov	r5, r1
3000756a:	4798      	blx	r3
3000756c:	b910      	cbnz	r0, 30007574 <BOOT_CertificateCheck+0x14>
3000756e:	2000      	movs	r0, #0
30007570:	b030      	add	sp, #192	; 0xc0
30007572:	bd70      	pop	{r4, r5, r6, pc}
30007574:	69a1      	ldr	r1, [r4, #24]
30007576:	2290      	movs	r2, #144	; 0x90
30007578:	a80c      	add	r0, sp, #48	; 0x30
3000757a:	4429      	add	r1, r5
3000757c:	f7fd f9ee 	bl	3000495c <BOOT_ImgCopy>
30007580:	4b24      	ldr	r3, [pc, #144]	; (30007614 <BOOT_CertificateCheck+0xb4>)
30007582:	4798      	blx	r3
30007584:	4924      	ldr	r1, [pc, #144]	; (30007618 <BOOT_CertificateCheck+0xb8>)
30007586:	f000 031f 	and.w	r3, r0, #31
3000758a:	2220      	movs	r2, #32
3000758c:	6808      	ldr	r0, [r1, #0]
3000758e:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30007592:	a930      	add	r1, sp, #192	; 0xc0
30007594:	3a01      	subs	r2, #1
30007596:	f813 c000 	ldrb.w	ip, [r3, r0]
3000759a:	4419      	add	r1, r3
3000759c:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
300075a0:	f103 0301 	add.w	r3, r3, #1
300075a4:	f801 ccb0 	strb.w	ip, [r1, #-176]
300075a8:	f003 031f 	and.w	r3, r3, #31
300075ac:	d1f1      	bne.n	30007592 <BOOT_CertificateCheck+0x32>
300075ae:	4610      	mov	r0, r2
300075b0:	4b1a      	ldr	r3, [pc, #104]	; (3000761c <BOOT_CertificateCheck+0xbc>)
300075b2:	4798      	blx	r3
300075b4:	7ce3      	ldrb	r3, [r4, #19]
300075b6:	7ca2      	ldrb	r2, [r4, #18]
300075b8:	f10d 010f 	add.w	r1, sp, #15
300075bc:	f10d 000e 	add.w	r0, sp, #14
300075c0:	f002 fde6 	bl	3000a190 <SBOOT_Validate_Algorithm>
300075c4:	4605      	mov	r5, r0
300075c6:	b9c0      	cbnz	r0, 300075fa <BOOT_CertificateCheck+0x9a>
300075c8:	f104 0620 	add.w	r6, r4, #32
300075cc:	aa04      	add	r2, sp, #16
300075ce:	f89d 000e 	ldrb.w	r0, [sp, #14]
300075d2:	4631      	mov	r1, r6
300075d4:	f002 fd5e 	bl	3000a094 <SBOOT_Validate_PubKey>
300075d8:	4605      	mov	r5, r0
300075da:	b970      	cbnz	r0, 300075fa <BOOT_CertificateCheck+0x9a>
300075dc:	69a0      	ldr	r0, [r4, #24]
300075de:	4623      	mov	r3, r4
300075e0:	ac0c      	add	r4, sp, #48	; 0x30
300075e2:	4632      	mov	r2, r6
300075e4:	f89d 100f 	ldrb.w	r1, [sp, #15]
300075e8:	e9cd 0400 	strd	r0, r4, [sp]
300075ec:	f89d 000e 	ldrb.w	r0, [sp, #14]
300075f0:	f002 fd6e 	bl	3000a0d0 <SBOOT_Validate_Signature>
300075f4:	4605      	mov	r5, r0
300075f6:	2800      	cmp	r0, #0
300075f8:	d0b9      	beq.n	3000756e <BOOT_CertificateCheck+0xe>
300075fa:	4b09      	ldr	r3, [pc, #36]	; (30007620 <BOOT_CertificateCheck+0xc0>)
300075fc:	2002      	movs	r0, #2
300075fe:	4a09      	ldr	r2, [pc, #36]	; (30007624 <BOOT_CertificateCheck+0xc4>)
30007600:	4619      	mov	r1, r3
30007602:	9500      	str	r5, [sp, #0]
30007604:	f004 fda4 	bl	3000c150 <rtk_log_write>
30007608:	20ff      	movs	r0, #255	; 0xff
3000760a:	b030      	add	sp, #192	; 0xc0
3000760c:	bd70      	pop	{r4, r5, r6, pc}
3000760e:	bf00      	nop
30007610:	0000c149 	.word	0x0000c149
30007614:	0000ea4d 	.word	0x0000ea4d
30007618:	3000e900 	.word	0x3000e900
3000761c:	3000a159 	.word	0x3000a159
30007620:	3000c810 	.word	0x3000c810
30007624:	3000d340 	.word	0x3000d340

30007628 <BOOT_RAM_TZCfg>:
30007628:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000762c:	b085      	sub	sp, #20
3000762e:	4e58      	ldr	r6, [pc, #352]	; (30007790 <BOOT_RAM_TZCfg+0x168>)
30007630:	4958      	ldr	r1, [pc, #352]	; (30007794 <BOOT_RAM_TZCfg+0x16c>)
30007632:	2501      	movs	r5, #1
30007634:	f10d 0e08 	add.w	lr, sp, #8
30007638:	4a57      	ldr	r2, [pc, #348]	; (30007798 <BOOT_RAM_TZCfg+0x170>)
3000763a:	4b58      	ldr	r3, [pc, #352]	; (3000779c <BOOT_RAM_TZCfg+0x174>)
3000763c:	46ec      	mov	ip, sp
3000763e:	4677      	mov	r7, lr
30007640:	f8df 8178 	ldr.w	r8, [pc, #376]	; 300077bc <BOOT_RAM_TZCfg+0x194>
30007644:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
30007648:	f8df 9174 	ldr.w	r9, [pc, #372]	; 300077c0 <BOOT_RAM_TZCfg+0x198>
3000764c:	9202      	str	r2, [sp, #8]
3000764e:	9303      	str	r3, [sp, #12]
30007650:	e9cd 6100 	strd	r6, r1, [sp]
30007654:	f85c 2b04 	ldr.w	r2, [ip], #4
30007658:	2300      	movs	r3, #0
3000765a:	f85e 1b04 	ldr.w	r1, [lr], #4
3000765e:	680c      	ldr	r4, [r1, #0]
30007660:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30007664:	fa05 f003 	lsl.w	r0, r5, r3
30007668:	f1b4 3fff 	cmp.w	r4, #4294967295	; 0xffffffff
3000766c:	d00c      	beq.n	30007688 <BOOT_RAM_TZCfg+0x60>
3000766e:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30007672:	3301      	adds	r3, #1
30007674:	684c      	ldr	r4, [r1, #4]
30007676:	310c      	adds	r1, #12
30007678:	2b08      	cmp	r3, #8
3000767a:	f8cb 4004 	str.w	r4, [fp, #4]
3000767e:	6c14      	ldr	r4, [r2, #64]	; 0x40
30007680:	ea40 0004 	orr.w	r0, r0, r4
30007684:	6410      	str	r0, [r2, #64]	; 0x40
30007686:	d1ea      	bne.n	3000765e <BOOT_RAM_TZCfg+0x36>
30007688:	42b2      	cmp	r2, r6
3000768a:	d072      	beq.n	30007772 <BOOT_RAM_TZCfg+0x14a>
3000768c:	6455      	str	r5, [r2, #68]	; 0x44
3000768e:	45bc      	cmp	ip, r7
30007690:	d1e0      	bne.n	30007654 <BOOT_RAM_TZCfg+0x2c>
30007692:	4b43      	ldr	r3, [pc, #268]	; (300077a0 <BOOT_RAM_TZCfg+0x178>)
30007694:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
30007698:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
3000769c:	4841      	ldr	r0, [pc, #260]	; (300077a4 <BOOT_RAM_TZCfg+0x17c>)
3000769e:	6019      	str	r1, [r3, #0]
300076a0:	2501      	movs	r5, #1
300076a2:	605a      	str	r2, [r3, #4]
300076a4:	2100      	movs	r1, #0
300076a6:	4a40      	ldr	r2, [pc, #256]	; (300077a8 <BOOT_RAM_TZCfg+0x180>)
300076a8:	4c40      	ldr	r4, [pc, #256]	; (300077ac <BOOT_RAM_TZCfg+0x184>)
300076aa:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
300076ae:	6813      	ldr	r3, [r2, #0]
300076b0:	3301      	adds	r3, #1
300076b2:	d019      	beq.n	300076e8 <BOOT_RAM_TZCfg+0xc0>
300076b4:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
300076b8:	320c      	adds	r2, #12
300076ba:	f852 3c0c 	ldr.w	r3, [r2, #-12]
300076be:	3101      	adds	r1, #1
300076c0:	f023 031f 	bic.w	r3, r3, #31
300076c4:	2908      	cmp	r1, #8
300076c6:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
300076ca:	e952 0302 	ldrd	r0, r3, [r2, #-8]
300076ce:	ea4f 0343 	mov.w	r3, r3, lsl #1
300076d2:	f020 001f 	bic.w	r0, r0, #31
300076d6:	f003 0302 	and.w	r3, r3, #2
300076da:	ea43 0300 	orr.w	r3, r3, r0
300076de:	f043 0301 	orr.w	r3, r3, #1
300076e2:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
300076e6:	d1e2      	bne.n	300076ae <BOOT_RAM_TZCfg+0x86>
300076e8:	4830      	ldr	r0, [pc, #192]	; (300077ac <BOOT_RAM_TZCfg+0x184>)
300076ea:	2201      	movs	r2, #1
300076ec:	4b30      	ldr	r3, [pc, #192]	; (300077b0 <BOOT_RAM_TZCfg+0x188>)
300076ee:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
300076f2:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
300076f6:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
300076fa:	d11e      	bne.n	3000773a <BOOT_RAM_TZCfg+0x112>
300076fc:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30007700:	f3bf 8f4f 	dsb	sy
30007704:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30007708:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000770c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30007710:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30007714:	0164      	lsls	r4, r4, #5
30007716:	ea04 0106 	and.w	r1, r4, r6
3000771a:	462b      	mov	r3, r5
3000771c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30007720:	3b01      	subs	r3, #1
30007722:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30007726:	1c5a      	adds	r2, r3, #1
30007728:	d1f8      	bne.n	3000771c <BOOT_RAM_TZCfg+0xf4>
3000772a:	3c20      	subs	r4, #32
3000772c:	f114 0f20 	cmn.w	r4, #32
30007730:	d1f1      	bne.n	30007716 <BOOT_RAM_TZCfg+0xee>
30007732:	f3bf 8f4f 	dsb	sy
30007736:	f3bf 8f6f 	isb	sy
3000773a:	4b1c      	ldr	r3, [pc, #112]	; (300077ac <BOOT_RAM_TZCfg+0x184>)
3000773c:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30007740:	491c      	ldr	r1, [pc, #112]	; (300077b4 <BOOT_RAM_TZCfg+0x18c>)
30007742:	691c      	ldr	r4, [r3, #16]
30007744:	4a1c      	ldr	r2, [pc, #112]	; (300077b8 <BOOT_RAM_TZCfg+0x190>)
30007746:	f044 0408 	orr.w	r4, r4, #8
3000774a:	611c      	str	r4, [r3, #16]
3000774c:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
30007750:	68d8      	ldr	r0, [r3, #12]
30007752:	4028      	ands	r0, r5
30007754:	4301      	orrs	r1, r0
30007756:	60d9      	str	r1, [r3, #12]
30007758:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
3000775c:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30007760:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30007764:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30007768:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
3000776c:	b005      	add	sp, #20
3000776e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007772:	f898 3000 	ldrb.w	r3, [r8]
30007776:	2b00      	cmp	r3, #0
30007778:	d189      	bne.n	3000768e <BOOT_RAM_TZCfg+0x66>
3000777a:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
3000777e:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
30007782:	6c33      	ldr	r3, [r6, #64]	; 0x40
30007784:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30007788:	6433      	str	r3, [r6, #64]	; 0x40
3000778a:	6475      	str	r5, [r6, #68]	; 0x44
3000778c:	e77f      	b.n	3000768e <BOOT_RAM_TZCfg+0x66>
3000778e:	bf00      	nop
30007790:	51001a00 	.word	0x51001a00
30007794:	51001200 	.word	0x51001200
30007798:	3000ea00 	.word	0x3000ea00
3000779c:	3000e9a0 	.word	0x3000e9a0
300077a0:	51001b00 	.word	0x51001b00
300077a4:	500e0000 	.word	0x500e0000
300077a8:	3000ea60 	.word	0x3000ea60
300077ac:	e000ed00 	.word	0xe000ed00
300077b0:	42008000 	.word	0x42008000
300077b4:	05fa6000 	.word	0x05fa6000
300077b8:	e000e100 	.word	0xe000e100
300077bc:	3000e8fb 	.word	0x3000e8fb
300077c0:	005fffff 	.word	0x005fffff

300077c4 <ymodem_inquiry>:
300077c4:	2806      	cmp	r0, #6
300077c6:	b510      	push	{r4, lr}
300077c8:	4604      	mov	r4, r0
300077ca:	d003      	beq.n	300077d4 <ymodem_inquiry+0x10>
300077cc:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077d0:	4b03      	ldr	r3, [pc, #12]	; (300077e0 <ymodem_inquiry+0x1c>)
300077d2:	4798      	blx	r3
300077d4:	4620      	mov	r0, r4
300077d6:	f000 fa07 	bl	30007be8 <ymodem_uart_putc>
300077da:	2001      	movs	r0, #1
300077dc:	bd10      	pop	{r4, pc}
300077de:	bf00      	nop
300077e0:	00009b2d 	.word	0x00009b2d

300077e4 <ymodem_get_first>:
300077e4:	b530      	push	{r4, r5, lr}
300077e6:	b083      	sub	sp, #12
300077e8:	2300      	movs	r3, #0
300077ea:	4604      	mov	r4, r0
300077ec:	4936      	ldr	r1, [pc, #216]	; (300078c8 <ymodem_get_first+0xe4>)
300077ee:	f10d 0006 	add.w	r0, sp, #6
300077f2:	f88d 3006 	strb.w	r3, [sp, #6]
300077f6:	f000 fa19 	bl	30007c2c <ymodem_uart_getc_to>
300077fa:	b9d0      	cbnz	r0, 30007832 <ymodem_get_first+0x4e>
300077fc:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007800:	3b01      	subs	r3, #1
30007802:	2b17      	cmp	r3, #23
30007804:	d80d      	bhi.n	30007822 <ymodem_get_first+0x3e>
30007806:	e8df f003 	tbb	[pc, r3]
3000780a:	231e      	.short	0x231e
3000780c:	0c0c290c 	.word	0x0c0c290c
30007810:	0c0c0c0c 	.word	0x0c0c0c0c
30007814:	0c0c0c0c 	.word	0x0c0c0c0c
30007818:	0c0c0c0c 	.word	0x0c0c0c0c
3000781c:	0c0c0c0c 	.word	0x0c0c0c0c
30007820:	190c      	.short	0x190c
30007822:	4d29      	ldr	r5, [pc, #164]	; (300078c8 <ymodem_get_first+0xe4>)
30007824:	4629      	mov	r1, r5
30007826:	f10d 0007 	add.w	r0, sp, #7
3000782a:	f000 f9ff 	bl	30007c2c <ymodem_uart_getc_to>
3000782e:	2800      	cmp	r0, #0
30007830:	d0f8      	beq.n	30007824 <ymodem_get_first+0x40>
30007832:	2301      	movs	r3, #1
30007834:	2008      	movs	r0, #8
30007836:	60a3      	str	r3, [r4, #8]
30007838:	b003      	add	sp, #12
3000783a:	bd30      	pop	{r4, r5, pc}
3000783c:	2301      	movs	r3, #1
3000783e:	2002      	movs	r0, #2
30007840:	60a3      	str	r3, [r4, #8]
30007842:	b003      	add	sp, #12
30007844:	bd30      	pop	{r4, r5, pc}
30007846:	2380      	movs	r3, #128	; 0x80
30007848:	2001      	movs	r0, #1
3000784a:	80a3      	strh	r3, [r4, #4]
3000784c:	b003      	add	sp, #12
3000784e:	bd30      	pop	{r4, r5, pc}
30007850:	f44f 6380 	mov.w	r3, #1024	; 0x400
30007854:	2001      	movs	r0, #1
30007856:	80a3      	strh	r3, [r4, #4]
30007858:	b003      	add	sp, #12
3000785a:	bd30      	pop	{r4, r5, pc}
3000785c:	2006      	movs	r0, #6
3000785e:	4d1b      	ldr	r5, [pc, #108]	; (300078cc <ymodem_get_first+0xe8>)
30007860:	f000 f9c2 	bl	30007be8 <ymodem_uart_putc>
30007864:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007868:	47a8      	blx	r5
3000786a:	2043      	movs	r0, #67	; 0x43
3000786c:	f000 f9bc 	bl	30007be8 <ymodem_uart_putc>
30007870:	2006      	movs	r0, #6
30007872:	f000 f9b9 	bl	30007be8 <ymodem_uart_putc>
30007876:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000787a:	47a8      	blx	r5
3000787c:	2043      	movs	r0, #67	; 0x43
3000787e:	f000 f9b3 	bl	30007be8 <ymodem_uart_putc>
30007882:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007886:	47a8      	blx	r5
30007888:	2018      	movs	r0, #24
3000788a:	f000 f9ad 	bl	30007be8 <ymodem_uart_putc>
3000788e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007892:	47a8      	blx	r5
30007894:	2018      	movs	r0, #24
30007896:	f000 f9a7 	bl	30007be8 <ymodem_uart_putc>
3000789a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000789e:	47a8      	blx	r5
300078a0:	2018      	movs	r0, #24
300078a2:	f000 f9a1 	bl	30007be8 <ymodem_uart_putc>
300078a6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078aa:	47a8      	blx	r5
300078ac:	2018      	movs	r0, #24
300078ae:	f000 f99b 	bl	30007be8 <ymodem_uart_putc>
300078b2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078b6:	47a8      	blx	r5
300078b8:	2018      	movs	r0, #24
300078ba:	f000 f995 	bl	30007be8 <ymodem_uart_putc>
300078be:	2301      	movs	r3, #1
300078c0:	2005      	movs	r0, #5
300078c2:	60a3      	str	r3, [r4, #8]
300078c4:	b003      	add	sp, #12
300078c6:	bd30      	pop	{r4, r5, pc}
300078c8:	000caa30 	.word	0x000caa30
300078cc:	00009b2d 	.word	0x00009b2d

300078d0 <ymodem_get_others>:
300078d0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300078d4:	b083      	sub	sp, #12
300078d6:	2300      	movs	r3, #0
300078d8:	4605      	mov	r5, r0
300078da:	4952      	ldr	r1, [pc, #328]	; (30007a24 <ymodem_get_others+0x154>)
300078dc:	a801      	add	r0, sp, #4
300078de:	f88d 3006 	strb.w	r3, [sp, #6]
300078e2:	f88d 3007 	strb.w	r3, [sp, #7]
300078e6:	f000 f9a1 	bl	30007c2c <ymodem_uart_getc_to>
300078ea:	bb38      	cbnz	r0, 3000793c <ymodem_get_others+0x6c>
300078ec:	494d      	ldr	r1, [pc, #308]	; (30007a24 <ymodem_get_others+0x154>)
300078ee:	f10d 0005 	add.w	r0, sp, #5
300078f2:	f000 f99b 	bl	30007c2c <ymodem_uart_getc_to>
300078f6:	4607      	mov	r7, r0
300078f8:	bb00      	cbnz	r0, 3000793c <ymodem_get_others+0x6c>
300078fa:	f89d 2004 	ldrb.w	r2, [sp, #4]
300078fe:	f89d 6005 	ldrb.w	r6, [sp, #5]
30007902:	43d3      	mvns	r3, r2
30007904:	b2db      	uxtb	r3, r3
30007906:	429e      	cmp	r6, r3
30007908:	bf0c      	ite	eq
3000790a:	2601      	moveq	r6, #1
3000790c:	2604      	movne	r6, #4
3000790e:	682b      	ldr	r3, [r5, #0]
30007910:	429a      	cmp	r2, r3
30007912:	d21a      	bcs.n	3000794a <ymodem_get_others+0x7a>
30007914:	2701      	movs	r7, #1
30007916:	88ab      	ldrh	r3, [r5, #4]
30007918:	b1d3      	cbz	r3, 30007950 <ymodem_get_others+0x80>
3000791a:	2400      	movs	r4, #0
3000791c:	f8df 9108 	ldr.w	r9, [pc, #264]	; 30007a28 <ymodem_get_others+0x158>
30007920:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007a24 <ymodem_get_others+0x154>
30007924:	e002      	b.n	3000792c <ymodem_get_others+0x5c>
30007926:	88ab      	ldrh	r3, [r5, #4]
30007928:	42a3      	cmp	r3, r4
3000792a:	d911      	bls.n	30007950 <ymodem_get_others+0x80>
3000792c:	eb04 0009 	add.w	r0, r4, r9
30007930:	4641      	mov	r1, r8
30007932:	3401      	adds	r4, #1
30007934:	f000 f97a 	bl	30007c2c <ymodem_uart_getc_to>
30007938:	2800      	cmp	r0, #0
3000793a:	d0f4      	beq.n	30007926 <ymodem_get_others+0x56>
3000793c:	2301      	movs	r3, #1
3000793e:	2608      	movs	r6, #8
30007940:	60ab      	str	r3, [r5, #8]
30007942:	4630      	mov	r0, r6
30007944:	b003      	add	sp, #12
30007946:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000794a:	d0e4      	beq.n	30007916 <ymodem_get_others+0x46>
3000794c:	2604      	movs	r6, #4
3000794e:	e7e2      	b.n	30007916 <ymodem_get_others+0x46>
30007950:	4934      	ldr	r1, [pc, #208]	; (30007a24 <ymodem_get_others+0x154>)
30007952:	f10d 0006 	add.w	r0, sp, #6
30007956:	f000 f969 	bl	30007c2c <ymodem_uart_getc_to>
3000795a:	2800      	cmp	r0, #0
3000795c:	d1ee      	bne.n	3000793c <ymodem_get_others+0x6c>
3000795e:	4931      	ldr	r1, [pc, #196]	; (30007a24 <ymodem_get_others+0x154>)
30007960:	f10d 0007 	add.w	r0, sp, #7
30007964:	f000 f962 	bl	30007c2c <ymodem_uart_getc_to>
30007968:	2800      	cmp	r0, #0
3000796a:	d1e7      	bne.n	3000793c <ymodem_get_others+0x6c>
3000796c:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007970:	f89d e007 	ldrb.w	lr, [sp, #7]
30007974:	88aa      	ldrh	r2, [r5, #4]
30007976:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
3000797a:	b36a      	cbz	r2, 300079d8 <ymodem_get_others+0x108>
3000797c:	4603      	mov	r3, r0
3000797e:	482a      	ldr	r0, [pc, #168]	; (30007a28 <ymodem_get_others+0x158>)
30007980:	f241 0c21 	movw	ip, #4129	; 0x1021
30007984:	eb02 0800 	add.w	r8, r2, r0
30007988:	7801      	ldrb	r1, [r0, #0]
3000798a:	3001      	adds	r0, #1
3000798c:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
30007990:	2108      	movs	r1, #8
30007992:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
30007996:	005b      	lsls	r3, r3, #1
30007998:	b10c      	cbz	r4, 3000799e <ymodem_get_others+0xce>
3000799a:	ea83 030c 	eor.w	r3, r3, ip
3000799e:	3901      	subs	r1, #1
300079a0:	d1f7      	bne.n	30007992 <ymodem_get_others+0xc2>
300079a2:	4580      	cmp	r8, r0
300079a4:	d1f0      	bne.n	30007988 <ymodem_get_others+0xb8>
300079a6:	b29b      	uxth	r3, r3
300079a8:	459e      	cmp	lr, r3
300079aa:	d00a      	beq.n	300079c2 <ymodem_get_others+0xf2>
300079ac:	4b1f      	ldr	r3, [pc, #124]	; (30007a2c <ymodem_get_others+0x15c>)
300079ae:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079b2:	4798      	blx	r3
300079b4:	2015      	movs	r0, #21
300079b6:	f000 f917 	bl	30007be8 <ymodem_uart_putc>
300079ba:	2301      	movs	r3, #1
300079bc:	2604      	movs	r6, #4
300079be:	60ab      	str	r3, [r5, #8]
300079c0:	e7bf      	b.n	30007942 <ymodem_get_others+0x72>
300079c2:	b917      	cbnz	r7, 300079ca <ymodem_get_others+0xfa>
300079c4:	4c1a      	ldr	r4, [pc, #104]	; (30007a30 <ymodem_get_others+0x160>)
300079c6:	6823      	ldr	r3, [r4, #0]
300079c8:	b9b3      	cbnz	r3, 300079f8 <ymodem_get_others+0x128>
300079ca:	f000 f947 	bl	30007c5c <ymodem_uart_clean_rx>
300079ce:	2e01      	cmp	r6, #1
300079d0:	d004      	beq.n	300079dc <ymodem_get_others+0x10c>
300079d2:	2e04      	cmp	r6, #4
300079d4:	d1b5      	bne.n	30007942 <ymodem_get_others+0x72>
300079d6:	e7e9      	b.n	300079ac <ymodem_get_others+0xdc>
300079d8:	4613      	mov	r3, r2
300079da:	e7e5      	b.n	300079a8 <ymodem_get_others+0xd8>
300079dc:	4c14      	ldr	r4, [pc, #80]	; (30007a30 <ymodem_get_others+0x160>)
300079de:	2006      	movs	r0, #6
300079e0:	60ae      	str	r6, [r5, #8]
300079e2:	f000 f901 	bl	30007be8 <ymodem_uart_putc>
300079e6:	6823      	ldr	r3, [r4, #0]
300079e8:	b193      	cbz	r3, 30007a10 <ymodem_get_others+0x140>
300079ea:	2f00      	cmp	r7, #0
300079ec:	f103 0301 	add.w	r3, r3, #1
300079f0:	bf18      	it	ne
300079f2:	2604      	movne	r6, #4
300079f4:	6023      	str	r3, [r4, #0]
300079f6:	e7a4      	b.n	30007942 <ymodem_get_others+0x72>
300079f8:	68eb      	ldr	r3, [r5, #12]
300079fa:	6861      	ldr	r1, [r4, #4]
300079fc:	3301      	adds	r3, #1
300079fe:	480a      	ldr	r0, [pc, #40]	; (30007a28 <ymodem_get_others+0x158>)
30007a00:	60eb      	str	r3, [r5, #12]
30007a02:	f000 f94f 	bl	30007ca4 <UARTIMG_Write>
30007a06:	6862      	ldr	r2, [r4, #4]
30007a08:	88ab      	ldrh	r3, [r5, #4]
30007a0a:	4413      	add	r3, r2
30007a0c:	6063      	str	r3, [r4, #4]
30007a0e:	e7dc      	b.n	300079ca <ymodem_get_others+0xfa>
30007a10:	4b06      	ldr	r3, [pc, #24]	; (30007a2c <ymodem_get_others+0x15c>)
30007a12:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a16:	4798      	blx	r3
30007a18:	2043      	movs	r0, #67	; 0x43
30007a1a:	f000 f8e5 	bl	30007be8 <ymodem_uart_putc>
30007a1e:	6823      	ldr	r3, [r4, #0]
30007a20:	e7e3      	b.n	300079ea <ymodem_get_others+0x11a>
30007a22:	bf00      	nop
30007a24:	000caa30 	.word	0x000caa30
30007a28:	20016ffc 	.word	0x20016ffc
30007a2c:	00009b2d 	.word	0x00009b2d
30007a30:	3000f044 	.word	0x3000f044

30007a34 <ymodem_rx_frame>:
30007a34:	2300      	movs	r3, #0
30007a36:	2201      	movs	r2, #1
30007a38:	b510      	push	{r4, lr}
30007a3a:	4604      	mov	r4, r0
30007a3c:	6083      	str	r3, [r0, #8]
30007a3e:	8082      	strh	r2, [r0, #4]
30007a40:	f7ff fed0 	bl	300077e4 <ymodem_get_first>
30007a44:	68a3      	ldr	r3, [r4, #8]
30007a46:	b92b      	cbnz	r3, 30007a54 <ymodem_rx_frame+0x20>
30007a48:	4620      	mov	r0, r4
30007a4a:	f7ff ff41 	bl	300078d0 <ymodem_get_others>
30007a4e:	68a3      	ldr	r3, [r4, #8]
30007a50:	2b00      	cmp	r3, #0
30007a52:	d0f9      	beq.n	30007a48 <ymodem_rx_frame+0x14>
30007a54:	2801      	cmp	r0, #1
30007a56:	d000      	beq.n	30007a5a <ymodem_rx_frame+0x26>
30007a58:	bd10      	pop	{r4, pc}
30007a5a:	6823      	ldr	r3, [r4, #0]
30007a5c:	3301      	adds	r3, #1
30007a5e:	b2db      	uxtb	r3, r3
30007a60:	6023      	str	r3, [r4, #0]
30007a62:	bd10      	pop	{r4, pc}

30007a64 <ymodem_img_rxbuffer>:
30007a64:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007a68:	2300      	movs	r3, #0
30007a6a:	4c28      	ldr	r4, [pc, #160]	; (30007b0c <ymodem_img_rxbuffer+0xa8>)
30007a6c:	4e28      	ldr	r6, [pc, #160]	; (30007b10 <ymodem_img_rxbuffer+0xac>)
30007a6e:	460d      	mov	r5, r1
30007a70:	60a3      	str	r3, [r4, #8]
30007a72:	6163      	str	r3, [r4, #20]
30007a74:	f000 f8f2 	bl	30007c5c <ymodem_uart_clean_rx>
30007a78:	2043      	movs	r0, #67	; 0x43
30007a7a:	f000 f8b5 	bl	30007be8 <ymodem_uart_putc>
30007a7e:	f24c 3050 	movw	r0, #50000	; 0xc350
30007a82:	47b0      	blx	r6
30007a84:	f000 f89a 	bl	30007bbc <ymodem_uart_readable>
30007a88:	2800      	cmp	r0, #0
30007a8a:	d0f5      	beq.n	30007a78 <ymodem_img_rxbuffer+0x14>
30007a8c:	2700      	movs	r7, #0
30007a8e:	f04f 0801 	mov.w	r8, #1
30007a92:	e9c4 7500 	strd	r7, r5, [r4]
30007a96:	4d1f      	ldr	r5, [pc, #124]	; (30007b14 <ymodem_img_rxbuffer+0xb0>)
30007a98:	4628      	mov	r0, r5
30007a9a:	f8a4 800c 	strh.w	r8, [r4, #12]
30007a9e:	6127      	str	r7, [r4, #16]
30007aa0:	f7ff fea0 	bl	300077e4 <ymodem_get_first>
30007aa4:	6923      	ldr	r3, [r4, #16]
30007aa6:	b92b      	cbnz	r3, 30007ab4 <ymodem_img_rxbuffer+0x50>
30007aa8:	4628      	mov	r0, r5
30007aaa:	f7ff ff11 	bl	300078d0 <ymodem_get_others>
30007aae:	6923      	ldr	r3, [r4, #16]
30007ab0:	2b00      	cmp	r3, #0
30007ab2:	d0f9      	beq.n	30007aa8 <ymodem_img_rxbuffer+0x44>
30007ab4:	2801      	cmp	r0, #1
30007ab6:	d024      	beq.n	30007b02 <ymodem_img_rxbuffer+0x9e>
30007ab8:	3804      	subs	r0, #4
30007aba:	2804      	cmp	r0, #4
30007abc:	d80c      	bhi.n	30007ad8 <ymodem_img_rxbuffer+0x74>
30007abe:	a301      	add	r3, pc, #4	; (adr r3, 30007ac4 <ymodem_img_rxbuffer+0x60>)
30007ac0:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007ac4:	30007a99 	.word	0x30007a99
30007ac8:	30007ae5 	.word	0x30007ae5
30007acc:	30007ad9 	.word	0x30007ad9
30007ad0:	30007ad9 	.word	0x30007ad9
30007ad4:	30007af1 	.word	0x30007af1
30007ad8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007adc:	47b0      	blx	r6
30007ade:	2018      	movs	r0, #24
30007ae0:	f000 f882 	bl	30007be8 <ymodem_uart_putc>
30007ae4:	200a      	movs	r0, #10
30007ae6:	f000 f8c9 	bl	30007c7c <ymodem_uart_waitbusy>
30007aea:	2001      	movs	r0, #1
30007aec:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007af0:	f000 f8b4 	bl	30007c5c <ymodem_uart_clean_rx>
30007af4:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007af8:	47b0      	blx	r6
30007afa:	2015      	movs	r0, #21
30007afc:	f000 f874 	bl	30007be8 <ymodem_uart_putc>
30007b00:	e7ca      	b.n	30007a98 <ymodem_img_rxbuffer+0x34>
30007b02:	68a3      	ldr	r3, [r4, #8]
30007b04:	3301      	adds	r3, #1
30007b06:	b2db      	uxtb	r3, r3
30007b08:	60a3      	str	r3, [r4, #8]
30007b0a:	e7c5      	b.n	30007a98 <ymodem_img_rxbuffer+0x34>
30007b0c:	3000f044 	.word	0x3000f044
30007b10:	00009b2d 	.word	0x00009b2d
30007b14:	3000f04c 	.word	0x3000f04c

30007b18 <ymodem_log_open>:
30007b18:	b510      	push	{r4, lr}
30007b1a:	2303      	movs	r3, #3
30007b1c:	4c04      	ldr	r4, [pc, #16]	; (30007b30 <ymodem_log_open+0x18>)
30007b1e:	2200      	movs	r2, #0
30007b20:	2108      	movs	r1, #8
30007b22:	4804      	ldr	r0, [pc, #16]	; (30007b34 <ymodem_log_open+0x1c>)
30007b24:	47a0      	blx	r4
30007b26:	2008      	movs	r0, #8
30007b28:	4b03      	ldr	r3, [pc, #12]	; (30007b38 <ymodem_log_open+0x20>)
30007b2a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007b2e:	4718      	bx	r3
30007b30:	30009df5 	.word	0x30009df5
30007b34:	0000ed39 	.word	0x0000ed39
30007b38:	0000e1bd 	.word	0x0000e1bd

30007b3c <ymodem_uart_port_init>:
30007b3c:	b500      	push	{lr}
30007b3e:	b083      	sub	sp, #12
30007b40:	2008      	movs	r0, #8
30007b42:	4b06      	ldr	r3, [pc, #24]	; (30007b5c <ymodem_uart_port_init+0x20>)
30007b44:	9201      	str	r2, [sp, #4]
30007b46:	4798      	blx	r3
30007b48:	4b05      	ldr	r3, [pc, #20]	; (30007b60 <ymodem_uart_port_init+0x24>)
30007b4a:	2008      	movs	r0, #8
30007b4c:	4798      	blx	r3
30007b4e:	9901      	ldr	r1, [sp, #4]
30007b50:	4804      	ldr	r0, [pc, #16]	; (30007b64 <ymodem_uart_port_init+0x28>)
30007b52:	4b05      	ldr	r3, [pc, #20]	; (30007b68 <ymodem_uart_port_init+0x2c>)
30007b54:	b003      	add	sp, #12
30007b56:	f85d eb04 	ldr.w	lr, [sp], #4
30007b5a:	4718      	bx	r3
30007b5c:	0000e1d9 	.word	0x0000e1d9
30007b60:	0000e2fd 	.word	0x0000e2fd
30007b64:	4200c000 	.word	0x4200c000
30007b68:	0000a9e5 	.word	0x0000a9e5

30007b6c <ymodem_uart_port_deinit>:
30007b6c:	2804      	cmp	r0, #4
30007b6e:	d004      	beq.n	30007b7a <ymodem_uart_port_deinit+0xe>
30007b70:	4b0d      	ldr	r3, [pc, #52]	; (30007ba8 <ymodem_uart_port_deinit+0x3c>)
30007b72:	681b      	ldr	r3, [r3, #0]
30007b74:	039b      	lsls	r3, r3, #14
30007b76:	d40f      	bmi.n	30007b98 <ymodem_uart_port_deinit+0x2c>
30007b78:	4770      	bx	lr
30007b7a:	2200      	movs	r2, #0
30007b7c:	2303      	movs	r3, #3
30007b7e:	2108      	movs	r1, #8
30007b80:	480a      	ldr	r0, [pc, #40]	; (30007bac <ymodem_uart_port_deinit+0x40>)
30007b82:	b510      	push	{r4, lr}
30007b84:	4c0a      	ldr	r4, [pc, #40]	; (30007bb0 <ymodem_uart_port_deinit+0x44>)
30007b86:	47a0      	blx	r4
30007b88:	4b0a      	ldr	r3, [pc, #40]	; (30007bb4 <ymodem_uart_port_deinit+0x48>)
30007b8a:	2008      	movs	r0, #8
30007b8c:	4798      	blx	r3
30007b8e:	4b06      	ldr	r3, [pc, #24]	; (30007ba8 <ymodem_uart_port_deinit+0x3c>)
30007b90:	681b      	ldr	r3, [r3, #0]
30007b92:	039a      	lsls	r2, r3, #14
30007b94:	d403      	bmi.n	30007b9e <ymodem_uart_port_deinit+0x32>
30007b96:	bd10      	pop	{r4, pc}
30007b98:	4807      	ldr	r0, [pc, #28]	; (30007bb8 <ymodem_uart_port_deinit+0x4c>)
30007b9a:	f004 bd81 	b.w	3000c6a0 <__DiagPrintf_veneer>
30007b9e:	4806      	ldr	r0, [pc, #24]	; (30007bb8 <ymodem_uart_port_deinit+0x4c>)
30007ba0:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007ba4:	f004 bd7c 	b.w	3000c6a0 <__DiagPrintf_veneer>
30007ba8:	2001c00c 	.word	0x2001c00c
30007bac:	0000ed39 	.word	0x0000ed39
30007bb0:	30009df5 	.word	0x30009df5
30007bb4:	0000e1bd 	.word	0x0000e1bd
30007bb8:	3000d36c 	.word	0x3000d36c

30007bbc <ymodem_uart_readable>:
30007bbc:	4b00      	ldr	r3, [pc, #0]	; (30007bc0 <ymodem_uart_readable+0x4>)
30007bbe:	4718      	bx	r3
30007bc0:	0000ac39 	.word	0x0000ac39

30007bc4 <ymodem_uart_writable>:
30007bc4:	4a03      	ldr	r2, [pc, #12]	; (30007bd4 <ymodem_uart_writable+0x10>)
30007bc6:	b508      	push	{r3, lr}
30007bc8:	4b03      	ldr	r3, [pc, #12]	; (30007bd8 <ymodem_uart_writable+0x14>)
30007bca:	6810      	ldr	r0, [r2, #0]
30007bcc:	4798      	blx	r3
30007bce:	b2c0      	uxtb	r0, r0
30007bd0:	bd08      	pop	{r3, pc}
30007bd2:	bf00      	nop
30007bd4:	3000f05c 	.word	0x3000f05c
30007bd8:	0000d1a9 	.word	0x0000d1a9

30007bdc <ymodem_uart_getc>:
30007bdc:	2000      	movs	r0, #0
30007bde:	4b01      	ldr	r3, [pc, #4]	; (30007be4 <ymodem_uart_getc+0x8>)
30007be0:	4718      	bx	r3
30007be2:	bf00      	nop
30007be4:	0000ac1d 	.word	0x0000ac1d

30007be8 <ymodem_uart_putc>:
30007be8:	4b00      	ldr	r3, [pc, #0]	; (30007bec <ymodem_uart_putc+0x4>)
30007bea:	4718      	bx	r3
30007bec:	0000abb5 	.word	0x0000abb5

30007bf0 <ymodem_uart_putdata>:
30007bf0:	b149      	cbz	r1, 30007c06 <ymodem_uart_putdata+0x16>
30007bf2:	b570      	push	{r4, r5, r6, lr}
30007bf4:	4604      	mov	r4, r0
30007bf6:	1845      	adds	r5, r0, r1
30007bf8:	4e03      	ldr	r6, [pc, #12]	; (30007c08 <ymodem_uart_putdata+0x18>)
30007bfa:	f814 0b01 	ldrb.w	r0, [r4], #1
30007bfe:	47b0      	blx	r6
30007c00:	42ac      	cmp	r4, r5
30007c02:	d1fa      	bne.n	30007bfa <ymodem_uart_putdata+0xa>
30007c04:	bd70      	pop	{r4, r5, r6, pc}
30007c06:	4770      	bx	lr
30007c08:	0000abb5 	.word	0x0000abb5

30007c0c <ymodem_uart_getdata>:
30007c0c:	b151      	cbz	r1, 30007c24 <ymodem_uart_getdata+0x18>
30007c0e:	b570      	push	{r4, r5, r6, lr}
30007c10:	4604      	mov	r4, r0
30007c12:	1845      	adds	r5, r0, r1
30007c14:	4e04      	ldr	r6, [pc, #16]	; (30007c28 <ymodem_uart_getdata+0x1c>)
30007c16:	2001      	movs	r0, #1
30007c18:	47b0      	blx	r6
30007c1a:	f804 0b01 	strb.w	r0, [r4], #1
30007c1e:	42ac      	cmp	r4, r5
30007c20:	d1f9      	bne.n	30007c16 <ymodem_uart_getdata+0xa>
30007c22:	bd70      	pop	{r4, r5, r6, pc}
30007c24:	4770      	bx	lr
30007c26:	bf00      	nop
30007c28:	0000ac1d 	.word	0x0000ac1d

30007c2c <ymodem_uart_getc_to>:
30007c2c:	b570      	push	{r4, r5, r6, lr}
30007c2e:	4d09      	ldr	r5, [pc, #36]	; (30007c54 <ymodem_uart_getc_to+0x28>)
30007c30:	4606      	mov	r6, r0
30007c32:	460c      	mov	r4, r1
30007c34:	e001      	b.n	30007c3a <ymodem_uart_getc_to+0xe>
30007c36:	3c01      	subs	r4, #1
30007c38:	d009      	beq.n	30007c4e <ymodem_uart_getc_to+0x22>
30007c3a:	47a8      	blx	r5
30007c3c:	2800      	cmp	r0, #0
30007c3e:	d0fa      	beq.n	30007c36 <ymodem_uart_getc_to+0xa>
30007c40:	4b05      	ldr	r3, [pc, #20]	; (30007c58 <ymodem_uart_getc_to+0x2c>)
30007c42:	2000      	movs	r0, #0
30007c44:	4798      	blx	r3
30007c46:	4603      	mov	r3, r0
30007c48:	2000      	movs	r0, #0
30007c4a:	7033      	strb	r3, [r6, #0]
30007c4c:	bd70      	pop	{r4, r5, r6, pc}
30007c4e:	2001      	movs	r0, #1
30007c50:	bd70      	pop	{r4, r5, r6, pc}
30007c52:	bf00      	nop
30007c54:	0000ac39 	.word	0x0000ac39
30007c58:	0000ac1d 	.word	0x0000ac1d

30007c5c <ymodem_uart_clean_rx>:
30007c5c:	b538      	push	{r3, r4, r5, lr}
30007c5e:	4c05      	ldr	r4, [pc, #20]	; (30007c74 <ymodem_uart_clean_rx+0x18>)
30007c60:	4d05      	ldr	r5, [pc, #20]	; (30007c78 <ymodem_uart_clean_rx+0x1c>)
30007c62:	e000      	b.n	30007c66 <ymodem_uart_clean_rx+0xa>
30007c64:	47a8      	blx	r5
30007c66:	47a0      	blx	r4
30007c68:	4603      	mov	r3, r0
30007c6a:	2000      	movs	r0, #0
30007c6c:	2b00      	cmp	r3, #0
30007c6e:	d1f9      	bne.n	30007c64 <ymodem_uart_clean_rx+0x8>
30007c70:	bd38      	pop	{r3, r4, r5, pc}
30007c72:	bf00      	nop
30007c74:	0000ac39 	.word	0x0000ac39
30007c78:	0000ac1d 	.word	0x0000ac1d

30007c7c <ymodem_uart_waitbusy>:
30007c7c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007c7e:	2400      	movs	r4, #0
30007c80:	4e06      	ldr	r6, [pc, #24]	; (30007c9c <ymodem_uart_waitbusy+0x20>)
30007c82:	4f07      	ldr	r7, [pc, #28]	; (30007ca0 <ymodem_uart_waitbusy+0x24>)
30007c84:	4605      	mov	r5, r0
30007c86:	e002      	b.n	30007c8e <ymodem_uart_waitbusy+0x12>
30007c88:	47b8      	blx	r7
30007c8a:	42ac      	cmp	r4, r5
30007c8c:	d804      	bhi.n	30007c98 <ymodem_uart_waitbusy+0x1c>
30007c8e:	6973      	ldr	r3, [r6, #20]
30007c90:	3401      	adds	r4, #1
30007c92:	2064      	movs	r0, #100	; 0x64
30007c94:	03db      	lsls	r3, r3, #15
30007c96:	d5f7      	bpl.n	30007c88 <ymodem_uart_waitbusy+0xc>
30007c98:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007c9a:	bf00      	nop
30007c9c:	4200c000 	.word	0x4200c000
30007ca0:	00009b2d 	.word	0x00009b2d

30007ca4 <UARTIMG_Write>:
30007ca4:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007ca8:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007cac:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007cb0:	d21d      	bcs.n	30007cee <UARTIMG_Write+0x4a>
30007cb2:	3a01      	subs	r2, #1
30007cb4:	4f19      	ldr	r7, [pc, #100]	; (30007d1c <UARTIMG_Write+0x78>)
30007cb6:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007d2c <UARTIMG_Write+0x88>
30007cba:	4688      	mov	r8, r1
30007cbc:	1916      	adds	r6, r2, r4
30007cbe:	4605      	mov	r5, r0
30007cc0:	f8d9 3000 	ldr.w	r3, [r9]
30007cc4:	403e      	ands	r6, r7
30007cc6:	4027      	ands	r7, r4
30007cc8:	42bb      	cmp	r3, r7
30007cca:	d91b      	bls.n	30007d04 <UARTIMG_Write+0x60>
30007ccc:	42b3      	cmp	r3, r6
30007cce:	d910      	bls.n	30007cf2 <UARTIMG_Write+0x4e>
30007cd0:	4e13      	ldr	r6, [pc, #76]	; (30007d20 <UARTIMG_Write+0x7c>)
30007cd2:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007cd6:	4f13      	ldr	r7, [pc, #76]	; (30007d24 <UARTIMG_Write+0x80>)
30007cd8:	4446      	add	r6, r8
30007cda:	eba3 0808 	sub.w	r8, r3, r8
30007cde:	eb08 0204 	add.w	r2, r8, r4
30007ce2:	4620      	mov	r0, r4
30007ce4:	3408      	adds	r4, #8
30007ce6:	2108      	movs	r1, #8
30007ce8:	47b8      	blx	r7
30007cea:	42b4      	cmp	r4, r6
30007cec:	d1f7      	bne.n	30007cde <UARTIMG_Write+0x3a>
30007cee:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007cf2:	4631      	mov	r1, r6
30007cf4:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007cf8:	2002      	movs	r0, #2
30007cfa:	4b0b      	ldr	r3, [pc, #44]	; (30007d28 <UARTIMG_Write+0x84>)
30007cfc:	4798      	blx	r3
30007cfe:	f8c9 6000 	str.w	r6, [r9]
30007d02:	e7e5      	b.n	30007cd0 <UARTIMG_Write+0x2c>
30007d04:	4b08      	ldr	r3, [pc, #32]	; (30007d28 <UARTIMG_Write+0x84>)
30007d06:	4639      	mov	r1, r7
30007d08:	2002      	movs	r0, #2
30007d0a:	4798      	blx	r3
30007d0c:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007d10:	42b3      	cmp	r3, r6
30007d12:	f8c9 3000 	str.w	r3, [r9]
30007d16:	d8db      	bhi.n	30007cd0 <UARTIMG_Write+0x2c>
30007d18:	e7eb      	b.n	30007cf2 <UARTIMG_Write+0x4e>
30007d1a:	bf00      	nop
30007d1c:	fffff000 	.word	0xfffff000
30007d20:	f8000400 	.word	0xf8000400
30007d24:	0000a04d 	.word	0x0000a04d
30007d28:	0000a16d 	.word	0x0000a16d
30007d2c:	3000f060 	.word	0x3000f060

30007d30 <ymodem_img_download>:
30007d30:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007d34:	4616      	mov	r6, r2
30007d36:	460d      	mov	r5, r1
30007d38:	4a0a      	ldr	r2, [pc, #40]	; (30007d64 <ymodem_img_download+0x34>)
30007d3a:	2100      	movs	r1, #0
30007d3c:	4680      	mov	r8, r0
30007d3e:	f7ff fefd 	bl	30007b3c <ymodem_uart_port_init>
30007d42:	b156      	cbz	r6, 30007d5a <ymodem_img_download+0x2a>
30007d44:	3d04      	subs	r5, #4
30007d46:	2400      	movs	r4, #0
30007d48:	4f06      	ldr	r7, [pc, #24]	; (30007d64 <ymodem_img_download+0x34>)
30007d4a:	3401      	adds	r4, #1
30007d4c:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007d50:	4638      	mov	r0, r7
30007d52:	f7ff fe87 	bl	30007a64 <ymodem_img_rxbuffer>
30007d56:	42b4      	cmp	r4, r6
30007d58:	d1f7      	bne.n	30007d4a <ymodem_img_download+0x1a>
30007d5a:	4640      	mov	r0, r8
30007d5c:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007d60:	f7ff bf04 	b.w	30007b6c <ymodem_uart_port_deinit>
30007d64:	0016e360 	.word	0x0016e360

30007d68 <ChipInfo_Invalid>:
30007d68:	4e06      	ldr	r6, [pc, #24]	; (30007d84 <ChipInfo_Invalid+0x1c>)
30007d6a:	4d07      	ldr	r5, [pc, #28]	; (30007d88 <ChipInfo_Invalid+0x20>)
30007d6c:	4c07      	ldr	r4, [pc, #28]	; (30007d8c <ChipInfo_Invalid+0x24>)
30007d6e:	b508      	push	{r3, lr}
30007d70:	462b      	mov	r3, r5
30007d72:	4622      	mov	r2, r4
30007d74:	4904      	ldr	r1, [pc, #16]	; (30007d88 <ChipInfo_Invalid+0x20>)
30007d76:	2002      	movs	r0, #2
30007d78:	f004 f9ea 	bl	3000c150 <rtk_log_write>
30007d7c:	f242 7010 	movw	r0, #10000	; 0x2710
30007d80:	47b0      	blx	r6
30007d82:	e7f5      	b.n	30007d70 <ChipInfo_Invalid+0x8>
30007d84:	00009be5 	.word	0x00009be5
30007d88:	3000d38c 	.word	0x3000d38c
30007d8c:	3000d398 	.word	0x3000d398

30007d90 <ChipInfo_Get>:
30007d90:	b570      	push	{r4, r5, r6, lr}
30007d92:	4c11      	ldr	r4, [pc, #68]	; (30007dd8 <ChipInfo_Get+0x48>)
30007d94:	7820      	ldrb	r0, [r4, #0]
30007d96:	28ff      	cmp	r0, #255	; 0xff
30007d98:	d104      	bne.n	30007da4 <ChipInfo_Get+0x14>
30007d9a:	4d10      	ldr	r5, [pc, #64]	; (30007ddc <ChipInfo_Get+0x4c>)
30007d9c:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007da0:	b108      	cbz	r0, 30007da6 <ChipInfo_Get+0x16>
30007da2:	7020      	strb	r0, [r4, #0]
30007da4:	bd70      	pop	{r4, r5, r6, pc}
30007da6:	4621      	mov	r1, r4
30007da8:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007dac:	f000 fefe 	bl	30008bac <OTP_Read8>
30007db0:	7820      	ldrb	r0, [r4, #0]
30007db2:	28ff      	cmp	r0, #255	; 0xff
30007db4:	d002      	beq.n	30007dbc <ChipInfo_Get+0x2c>
30007db6:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007dba:	bd70      	pop	{r4, r5, r6, pc}
30007dbc:	4e08      	ldr	r6, [pc, #32]	; (30007de0 <ChipInfo_Get+0x50>)
30007dbe:	4d09      	ldr	r5, [pc, #36]	; (30007de4 <ChipInfo_Get+0x54>)
30007dc0:	4c09      	ldr	r4, [pc, #36]	; (30007de8 <ChipInfo_Get+0x58>)
30007dc2:	462b      	mov	r3, r5
30007dc4:	4622      	mov	r2, r4
30007dc6:	4907      	ldr	r1, [pc, #28]	; (30007de4 <ChipInfo_Get+0x54>)
30007dc8:	2003      	movs	r0, #3
30007dca:	f004 f9c1 	bl	3000c150 <rtk_log_write>
30007dce:	f242 7010 	movw	r0, #10000	; 0x2710
30007dd2:	47b0      	blx	r6
30007dd4:	e7f5      	b.n	30007dc2 <ChipInfo_Get+0x32>
30007dd6:	bf00      	nop
30007dd8:	3000f15c 	.word	0x3000f15c
30007ddc:	23020000 	.word	0x23020000
30007de0:	00009be5 	.word	0x00009be5
30007de4:	3000d38c 	.word	0x3000d38c
30007de8:	3000d3bc 	.word	0x3000d3bc

30007dec <ChipInfo_MemoryType>:
30007dec:	b510      	push	{r4, lr}
30007dee:	4c11      	ldr	r4, [pc, #68]	; (30007e34 <ChipInfo_MemoryType+0x48>)
30007df0:	47a0      	blx	r4
30007df2:	2802      	cmp	r0, #2
30007df4:	d019      	beq.n	30007e2a <ChipInfo_MemoryType+0x3e>
30007df6:	47a0      	blx	r4
30007df8:	2803      	cmp	r0, #3
30007dfa:	d016      	beq.n	30007e2a <ChipInfo_MemoryType+0x3e>
30007dfc:	4c0e      	ldr	r4, [pc, #56]	; (30007e38 <ChipInfo_MemoryType+0x4c>)
30007dfe:	4b0f      	ldr	r3, [pc, #60]	; (30007e3c <ChipInfo_MemoryType+0x50>)
30007e00:	4798      	blx	r3
30007e02:	2100      	movs	r1, #0
30007e04:	23e0      	movs	r3, #224	; 0xe0
30007e06:	4622      	mov	r2, r4
30007e08:	e004      	b.n	30007e14 <ChipInfo_MemoryType+0x28>
30007e0a:	7a13      	ldrb	r3, [r2, #8]
30007e0c:	3101      	adds	r1, #1
30007e0e:	3206      	adds	r2, #6
30007e10:	2bff      	cmp	r3, #255	; 0xff
30007e12:	d00c      	beq.n	30007e2e <ChipInfo_MemoryType+0x42>
30007e14:	4298      	cmp	r0, r3
30007e16:	d1f8      	bne.n	30007e0a <ChipInfo_MemoryType+0x1e>
30007e18:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e1c:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e20:	78cb      	ldrb	r3, [r1, #3]
30007e22:	2b02      	cmp	r3, #2
30007e24:	d001      	beq.n	30007e2a <ChipInfo_MemoryType+0x3e>
30007e26:	2001      	movs	r0, #1
30007e28:	bd10      	pop	{r4, pc}
30007e2a:	2002      	movs	r0, #2
30007e2c:	bd10      	pop	{r4, pc}
30007e2e:	f7ff ff9b 	bl	30007d68 <ChipInfo_Invalid>
30007e32:	bf00      	nop
30007e34:	0000c0f9 	.word	0x0000c0f9
30007e38:	3000d3e8 	.word	0x3000d3e8
30007e3c:	30007d91 	.word	0x30007d91

30007e40 <ChipInfo_DDRType>:
30007e40:	b510      	push	{r4, lr}
30007e42:	4b0f      	ldr	r3, [pc, #60]	; (30007e80 <ChipInfo_DDRType+0x40>)
30007e44:	4798      	blx	r3
30007e46:	4b0f      	ldr	r3, [pc, #60]	; (30007e84 <ChipInfo_DDRType+0x44>)
30007e48:	4604      	mov	r4, r0
30007e4a:	4798      	blx	r3
30007e4c:	2801      	cmp	r0, #1
30007e4e:	d112      	bne.n	30007e76 <ChipInfo_DDRType+0x36>
30007e50:	480d      	ldr	r0, [pc, #52]	; (30007e88 <ChipInfo_DDRType+0x48>)
30007e52:	2100      	movs	r1, #0
30007e54:	22e0      	movs	r2, #224	; 0xe0
30007e56:	4603      	mov	r3, r0
30007e58:	e003      	b.n	30007e62 <ChipInfo_DDRType+0x22>
30007e5a:	789a      	ldrb	r2, [r3, #2]
30007e5c:	3101      	adds	r1, #1
30007e5e:	2aff      	cmp	r2, #255	; 0xff
30007e60:	d00b      	beq.n	30007e7a <ChipInfo_DDRType+0x3a>
30007e62:	4294      	cmp	r4, r2
30007e64:	f103 0306 	add.w	r3, r3, #6
30007e68:	d1f7      	bne.n	30007e5a <ChipInfo_DDRType+0x1a>
30007e6a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e6e:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007e72:	78c8      	ldrb	r0, [r1, #3]
30007e74:	bd10      	pop	{r4, pc}
30007e76:	20ff      	movs	r0, #255	; 0xff
30007e78:	bd10      	pop	{r4, pc}
30007e7a:	2000      	movs	r0, #0
30007e7c:	bd10      	pop	{r4, pc}
30007e7e:	bf00      	nop
30007e80:	30007d91 	.word	0x30007d91
30007e84:	30007ded 	.word	0x30007ded
30007e88:	3000d3e8 	.word	0x3000d3e8

30007e8c <ChipInfo_MemorySize>:
30007e8c:	b510      	push	{r4, lr}
30007e8e:	4c0b      	ldr	r4, [pc, #44]	; (30007ebc <ChipInfo_MemorySize+0x30>)
30007e90:	4b0b      	ldr	r3, [pc, #44]	; (30007ec0 <ChipInfo_MemorySize+0x34>)
30007e92:	4798      	blx	r3
30007e94:	2100      	movs	r1, #0
30007e96:	23e0      	movs	r3, #224	; 0xe0
30007e98:	4622      	mov	r2, r4
30007e9a:	e004      	b.n	30007ea6 <ChipInfo_MemorySize+0x1a>
30007e9c:	7a13      	ldrb	r3, [r2, #8]
30007e9e:	3101      	adds	r1, #1
30007ea0:	3206      	adds	r2, #6
30007ea2:	2bff      	cmp	r3, #255	; 0xff
30007ea4:	d007      	beq.n	30007eb6 <ChipInfo_MemorySize+0x2a>
30007ea6:	4298      	cmp	r0, r3
30007ea8:	d1f8      	bne.n	30007e9c <ChipInfo_MemorySize+0x10>
30007eaa:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007eae:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007eb2:	7908      	ldrb	r0, [r1, #4]
30007eb4:	bd10      	pop	{r4, pc}
30007eb6:	f7ff ff57 	bl	30007d68 <ChipInfo_Invalid>
30007eba:	bf00      	nop
30007ebc:	3000d3e8 	.word	0x3000d3e8
30007ec0:	30007d91 	.word	0x30007d91

30007ec4 <ChipInfo_MemoryVendor>:
30007ec4:	b510      	push	{r4, lr}
30007ec6:	4c0f      	ldr	r4, [pc, #60]	; (30007f04 <ChipInfo_MemoryVendor+0x40>)
30007ec8:	47a0      	blx	r4
30007eca:	2803      	cmp	r0, #3
30007ecc:	d016      	beq.n	30007efc <ChipInfo_MemoryVendor+0x38>
30007ece:	47a0      	blx	r4
30007ed0:	2802      	cmp	r0, #2
30007ed2:	d012      	beq.n	30007efa <ChipInfo_MemoryVendor+0x36>
30007ed4:	4c0c      	ldr	r4, [pc, #48]	; (30007f08 <ChipInfo_MemoryVendor+0x44>)
30007ed6:	4b0d      	ldr	r3, [pc, #52]	; (30007f0c <ChipInfo_MemoryVendor+0x48>)
30007ed8:	4798      	blx	r3
30007eda:	2100      	movs	r1, #0
30007edc:	23e0      	movs	r3, #224	; 0xe0
30007ede:	4622      	mov	r2, r4
30007ee0:	e004      	b.n	30007eec <ChipInfo_MemoryVendor+0x28>
30007ee2:	7a13      	ldrb	r3, [r2, #8]
30007ee4:	3101      	adds	r1, #1
30007ee6:	3206      	adds	r2, #6
30007ee8:	2bff      	cmp	r3, #255	; 0xff
30007eea:	d009      	beq.n	30007f00 <ChipInfo_MemoryVendor+0x3c>
30007eec:	4298      	cmp	r0, r3
30007eee:	d1f8      	bne.n	30007ee2 <ChipInfo_MemoryVendor+0x1e>
30007ef0:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007ef4:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007ef8:	7948      	ldrb	r0, [r1, #5]
30007efa:	bd10      	pop	{r4, pc}
30007efc:	2001      	movs	r0, #1
30007efe:	bd10      	pop	{r4, pc}
30007f00:	f7ff ff32 	bl	30007d68 <ChipInfo_Invalid>
30007f04:	0000c0f9 	.word	0x0000c0f9
30007f08:	3000d3e8 	.word	0x3000d3e8
30007f0c:	30007d91 	.word	0x30007d91

30007f10 <SDM32K_Enable>:
30007f10:	4a02      	ldr	r2, [pc, #8]	; (30007f1c <SDM32K_Enable+0xc>)
30007f12:	4b03      	ldr	r3, [pc, #12]	; (30007f20 <SDM32K_Enable+0x10>)
30007f14:	6811      	ldr	r1, [r2, #0]
30007f16:	430b      	orrs	r3, r1
30007f18:	6013      	str	r3, [r2, #0]
30007f1a:	4770      	bx	lr
30007f1c:	42008e00 	.word	0x42008e00
30007f20:	c0060000 	.word	0xc0060000

30007f24 <CLK_SWITCH_XTAL>:
30007f24:	b508      	push	{r3, lr}
30007f26:	2801      	cmp	r0, #1
30007f28:	4b0a      	ldr	r3, [pc, #40]	; (30007f54 <CLK_SWITCH_XTAL+0x30>)
30007f2a:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007f2e:	d00d      	beq.n	30007f4c <CLK_SWITCH_XTAL+0x28>
30007f30:	4b09      	ldr	r3, [pc, #36]	; (30007f58 <CLK_SWITCH_XTAL+0x34>)
30007f32:	4313      	orrs	r3, r2
30007f34:	4907      	ldr	r1, [pc, #28]	; (30007f54 <CLK_SWITCH_XTAL+0x30>)
30007f36:	4a09      	ldr	r2, [pc, #36]	; (30007f5c <CLK_SWITCH_XTAL+0x38>)
30007f38:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007f3c:	4790      	blx	r2
30007f3e:	4a08      	ldr	r2, [pc, #32]	; (30007f60 <CLK_SWITCH_XTAL+0x3c>)
30007f40:	4b08      	ldr	r3, [pc, #32]	; (30007f64 <CLK_SWITCH_XTAL+0x40>)
30007f42:	fba2 2000 	umull	r2, r0, r2, r0
30007f46:	0c80      	lsrs	r0, r0, #18
30007f48:	6018      	str	r0, [r3, #0]
30007f4a:	bd08      	pop	{r3, pc}
30007f4c:	4b06      	ldr	r3, [pc, #24]	; (30007f68 <CLK_SWITCH_XTAL+0x44>)
30007f4e:	4013      	ands	r3, r2
30007f50:	e7f0      	b.n	30007f34 <CLK_SWITCH_XTAL+0x10>
30007f52:	bf00      	nop
30007f54:	42008000 	.word	0x42008000
30007f58:	80000500 	.word	0x80000500
30007f5c:	30009ac5 	.word	0x30009ac5
30007f60:	431bde83 	.word	0x431bde83
30007f64:	2001c700 	.word	0x2001c700
30007f68:	7ffffaff 	.word	0x7ffffaff

30007f6c <DDR_PHY_ChipInfo.part.0>:
30007f6c:	b508      	push	{r3, lr}
30007f6e:	4b24      	ldr	r3, [pc, #144]	; (30008000 <DDR_PHY_ChipInfo.part.0+0x94>)
30007f70:	4798      	blx	r3
30007f72:	280e      	cmp	r0, #14
30007f74:	d80d      	bhi.n	30007f92 <DDR_PHY_ChipInfo.part.0+0x26>
30007f76:	2801      	cmp	r0, #1
30007f78:	d912      	bls.n	30007fa0 <DDR_PHY_ChipInfo.part.0+0x34>
30007f7a:	1e83      	subs	r3, r0, #2
30007f7c:	2b0c      	cmp	r3, #12
30007f7e:	d80f      	bhi.n	30007fa0 <DDR_PHY_ChipInfo.part.0+0x34>
30007f80:	e8df f003 	tbb	[pc, r3]
30007f84:	210e1d19 	.word	0x210e1d19
30007f88:	2a0e260e 	.word	0x2a0e260e
30007f8c:	340e0e2f 	.word	0x340e0e2f
30007f90:	39          	.byte	0x39
30007f91:	00          	.byte	0x00
30007f92:	28e0      	cmp	r0, #224	; 0xe0
30007f94:	d104      	bne.n	30007fa0 <DDR_PHY_ChipInfo.part.0+0x34>
30007f96:	2301      	movs	r3, #1
30007f98:	4a1a      	ldr	r2, [pc, #104]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f9a:	4618      	mov	r0, r3
30007f9c:	7013      	strb	r3, [r2, #0]
30007f9e:	bd08      	pop	{r3, pc}
30007fa0:	4b19      	ldr	r3, [pc, #100]	; (30008008 <DDR_PHY_ChipInfo.part.0+0x9c>)
30007fa2:	2002      	movs	r0, #2
30007fa4:	4a19      	ldr	r2, [pc, #100]	; (3000800c <DDR_PHY_ChipInfo.part.0+0xa0>)
30007fa6:	4619      	mov	r1, r3
30007fa8:	f004 f8d2 	bl	3000c150 <rtk_log_write>
30007fac:	2300      	movs	r3, #0
30007fae:	4a15      	ldr	r2, [pc, #84]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fb0:	4618      	mov	r0, r3
30007fb2:	7013      	strb	r3, [r2, #0]
30007fb4:	bd08      	pop	{r3, pc}
30007fb6:	4b13      	ldr	r3, [pc, #76]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fb8:	2202      	movs	r2, #2
30007fba:	701a      	strb	r2, [r3, #0]
30007fbc:	bd08      	pop	{r3, pc}
30007fbe:	4b11      	ldr	r3, [pc, #68]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fc0:	2203      	movs	r2, #3
30007fc2:	701a      	strb	r2, [r3, #0]
30007fc4:	bd08      	pop	{r3, pc}
30007fc6:	2304      	movs	r3, #4
30007fc8:	4a0e      	ldr	r2, [pc, #56]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fca:	4618      	mov	r0, r3
30007fcc:	7013      	strb	r3, [r2, #0]
30007fce:	bd08      	pop	{r3, pc}
30007fd0:	4b0c      	ldr	r3, [pc, #48]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fd2:	2207      	movs	r2, #7
30007fd4:	701a      	strb	r2, [r3, #0]
30007fd6:	bd08      	pop	{r3, pc}
30007fd8:	2305      	movs	r3, #5
30007fda:	4a0a      	ldr	r2, [pc, #40]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fdc:	4618      	mov	r0, r3
30007fde:	7013      	strb	r3, [r2, #0]
30007fe0:	bd08      	pop	{r3, pc}
30007fe2:	2306      	movs	r3, #6
30007fe4:	4a07      	ldr	r2, [pc, #28]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fe6:	4618      	mov	r0, r3
30007fe8:	7013      	strb	r3, [r2, #0]
30007fea:	bd08      	pop	{r3, pc}
30007fec:	2308      	movs	r3, #8
30007fee:	4a05      	ldr	r2, [pc, #20]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007ff0:	4618      	mov	r0, r3
30007ff2:	7013      	strb	r3, [r2, #0]
30007ff4:	bd08      	pop	{r3, pc}
30007ff6:	2309      	movs	r3, #9
30007ff8:	4a02      	ldr	r2, [pc, #8]	; (30008004 <DDR_PHY_ChipInfo.part.0+0x98>)
30007ffa:	4618      	mov	r0, r3
30007ffc:	7013      	strb	r3, [r2, #0]
30007ffe:	bd08      	pop	{r3, pc}
30008000:	30007d91 	.word	0x30007d91
30008004:	3000f15d 	.word	0x3000f15d
30008008:	3000d46c 	.word	0x3000d46c
3000800c:	3000d474 	.word	0x3000d474

30008010 <DDR_PHY_ChipInfo_ddrtype>:
30008010:	b510      	push	{r4, lr}
30008012:	4c04      	ldr	r4, [pc, #16]	; (30008024 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30008014:	7820      	ldrb	r0, [r4, #0]
30008016:	b100      	cbz	r0, 3000801a <DDR_PHY_ChipInfo_ddrtype+0xa>
30008018:	bd10      	pop	{r4, pc}
3000801a:	4b03      	ldr	r3, [pc, #12]	; (30008028 <DDR_PHY_ChipInfo_ddrtype+0x18>)
3000801c:	4798      	blx	r3
3000801e:	7020      	strb	r0, [r4, #0]
30008020:	bd10      	pop	{r4, pc}
30008022:	bf00      	nop
30008024:	3000fe70 	.word	0x3000fe70
30008028:	30007e41 	.word	0x30007e41

3000802c <DDR_PHY_StructInit>:
3000802c:	f240 2315 	movw	r3, #533	; 0x215
30008030:	6583      	str	r3, [r0, #88]	; 0x58
30008032:	4770      	bx	lr

30008034 <DDR_PHY_DLL_CLK_DIV>:
30008034:	b570      	push	{r4, r5, r6, lr}
30008036:	4d5a      	ldr	r5, [pc, #360]	; (300081a0 <DDR_PHY_DLL_CLK_DIV+0x16c>)
30008038:	495a      	ldr	r1, [pc, #360]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000803a:	fba5 4200 	umull	r4, r2, r5, r0
3000803e:	4b5a      	ldr	r3, [pc, #360]	; (300081a8 <DDR_PHY_DLL_CLK_DIV+0x174>)
30008040:	6a4e      	ldr	r6, [r1, #36]	; 0x24
30008042:	0952      	lsrs	r2, r2, #5
30008044:	4c59      	ldr	r4, [pc, #356]	; (300081ac <DDR_PHY_DLL_CLK_DIV+0x178>)
30008046:	4403      	add	r3, r0
30008048:	4859      	ldr	r0, [pc, #356]	; (300081b0 <DDR_PHY_DLL_CLK_DIV+0x17c>)
3000804a:	3a03      	subs	r2, #3
3000804c:	4034      	ands	r4, r6
3000804e:	eb02 0682 	add.w	r6, r2, r2, lsl #2
30008052:	eba3 03c6 	sub.w	r3, r3, r6, lsl #3
30008056:	02db      	lsls	r3, r3, #11
30008058:	fba5 5303 	umull	r5, r3, r5, r3
3000805c:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
30008060:	f3c3 134a 	ubfx	r3, r3, #5, #11
30008064:	4320      	orrs	r0, r4
30008066:	0214      	lsls	r4, r2, #8
30008068:	b2d2      	uxtb	r2, r2
3000806a:	6248      	str	r0, [r1, #36]	; 0x24
3000806c:	b2a4      	uxth	r4, r4
3000806e:	6a88      	ldr	r0, [r1, #40]	; 0x28
30008070:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
30008074:	4304      	orrs	r4, r0
30008076:	484f      	ldr	r0, [pc, #316]	; (300081b4 <DDR_PHY_DLL_CLK_DIV+0x180>)
30008078:	628c      	str	r4, [r1, #40]	; 0x28
3000807a:	6a4c      	ldr	r4, [r1, #36]	; 0x24
3000807c:	4020      	ands	r0, r4
3000807e:	2401      	movs	r4, #1
30008080:	4303      	orrs	r3, r0
30008082:	624b      	str	r3, [r1, #36]	; 0x24
30008084:	6a8b      	ldr	r3, [r1, #40]	; 0x28
30008086:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
3000808a:	431a      	orrs	r2, r3
3000808c:	4b4a      	ldr	r3, [pc, #296]	; (300081b8 <DDR_PHY_DLL_CLK_DIV+0x184>)
3000808e:	628a      	str	r2, [r1, #40]	; 0x28
30008090:	678c      	str	r4, [r1, #120]	; 0x78
30008092:	7818      	ldrb	r0, [r3, #0]
30008094:	28ff      	cmp	r0, #255	; 0xff
30008096:	d076      	beq.n	30008186 <DDR_PHY_DLL_CLK_DIV+0x152>
30008098:	4c48      	ldr	r4, [pc, #288]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x188>)
3000809a:	0145      	lsls	r5, r0, #5
3000809c:	4b41      	ldr	r3, [pc, #260]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000809e:	eb04 1040 	add.w	r0, r4, r0, lsl #5
300080a2:	5962      	ldr	r2, [r4, r5]
300080a4:	4e46      	ldr	r6, [pc, #280]	; (300081c0 <DDR_PHY_DLL_CLK_DIV+0x18c>)
300080a6:	611a      	str	r2, [r3, #16]
300080a8:	6842      	ldr	r2, [r0, #4]
300080aa:	615a      	str	r2, [r3, #20]
300080ac:	6882      	ldr	r2, [r0, #8]
300080ae:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
300080b2:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
300080b6:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
300080ba:	609a      	str	r2, [r3, #8]
300080bc:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300080c0:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300080c4:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300080c8:	7830      	ldrb	r0, [r6, #0]
300080ca:	2800      	cmp	r0, #0
300080cc:	d057      	beq.n	3000817e <DDR_PHY_DLL_CLK_DIV+0x14a>
300080ce:	2801      	cmp	r0, #1
300080d0:	eb04 0305 	add.w	r3, r4, r5
300080d4:	4a33      	ldr	r2, [pc, #204]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
300080d6:	d037      	beq.n	30008148 <DDR_PHY_DLL_CLK_DIV+0x114>
300080d8:	2122      	movs	r1, #34	; 0x22
300080da:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300080de:	6959      	ldr	r1, [r3, #20]
300080e0:	f8c2 1080 	str.w	r1, [r2, #128]	; 0x80
300080e4:	e9d3 1306 	ldrd	r1, r3, [r3, #24]
300080e8:	f8c2 1084 	str.w	r1, [r2, #132]	; 0x84
300080ec:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
300080f0:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300080f4:	4933      	ldr	r1, [pc, #204]	; (300081c4 <DDR_PHY_DLL_CLK_DIV+0x190>)
300080f6:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
300080fa:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300080fe:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
30008102:	009b      	lsls	r3, r3, #2
30008104:	d516      	bpl.n	30008134 <DDR_PHY_DLL_CLK_DIV+0x100>
30008106:	2800      	cmp	r0, #0
30008108:	d044      	beq.n	30008194 <DDR_PHY_DLL_CLK_DIV+0x160>
3000810a:	4b26      	ldr	r3, [pc, #152]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000810c:	4a2e      	ldr	r2, [pc, #184]	; (300081c8 <DDR_PHY_DLL_CLK_DIV+0x194>)
3000810e:	601a      	str	r2, [r3, #0]
30008110:	4b24      	ldr	r3, [pc, #144]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008112:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008116:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
3000811a:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
3000811e:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008122:	699a      	ldr	r2, [r3, #24]
30008124:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
30008128:	619a      	str	r2, [r3, #24]
3000812a:	699a      	ldr	r2, [r3, #24]
3000812c:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30008130:	619a      	str	r2, [r3, #24]
30008132:	bd70      	pop	{r4, r5, r6, pc}
30008134:	b920      	cbnz	r0, 30008140 <DDR_PHY_DLL_CLK_DIV+0x10c>
30008136:	4b25      	ldr	r3, [pc, #148]	; (300081cc <DDR_PHY_DLL_CLK_DIV+0x198>)
30008138:	4798      	blx	r3
3000813a:	2801      	cmp	r0, #1
3000813c:	7030      	strb	r0, [r6, #0]
3000813e:	d025      	beq.n	3000818c <DDR_PHY_DLL_CLK_DIV+0x158>
30008140:	4b18      	ldr	r3, [pc, #96]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008142:	4a23      	ldr	r2, [pc, #140]	; (300081d0 <DDR_PHY_DLL_CLK_DIV+0x19c>)
30008144:	601a      	str	r2, [r3, #0]
30008146:	e7e3      	b.n	30008110 <DDR_PHY_DLL_CLK_DIV+0xdc>
30008148:	2033      	movs	r0, #51	; 0x33
3000814a:	695c      	ldr	r4, [r3, #20]
3000814c:	491d      	ldr	r1, [pc, #116]	; (300081c4 <DDR_PHY_DLL_CLK_DIV+0x190>)
3000814e:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
30008152:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
30008156:	e9d3 0306 	ldrd	r0, r3, [r3, #24]
3000815a:	f8c2 0084 	str.w	r0, [r2, #132]	; 0x84
3000815e:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
30008162:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008166:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
3000816a:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000816e:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
30008172:	009a      	lsls	r2, r3, #2
30008174:	d50a      	bpl.n	3000818c <DDR_PHY_DLL_CLK_DIV+0x158>
30008176:	4b0b      	ldr	r3, [pc, #44]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008178:	4a16      	ldr	r2, [pc, #88]	; (300081d4 <DDR_PHY_DLL_CLK_DIV+0x1a0>)
3000817a:	601a      	str	r2, [r3, #0]
3000817c:	e7c8      	b.n	30008110 <DDR_PHY_DLL_CLK_DIV+0xdc>
3000817e:	4b13      	ldr	r3, [pc, #76]	; (300081cc <DDR_PHY_DLL_CLK_DIV+0x198>)
30008180:	4798      	blx	r3
30008182:	7030      	strb	r0, [r6, #0]
30008184:	e7a3      	b.n	300080ce <DDR_PHY_DLL_CLK_DIV+0x9a>
30008186:	f7ff fef1 	bl	30007f6c <DDR_PHY_ChipInfo.part.0>
3000818a:	e785      	b.n	30008098 <DDR_PHY_DLL_CLK_DIV+0x64>
3000818c:	4b05      	ldr	r3, [pc, #20]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000818e:	4a12      	ldr	r2, [pc, #72]	; (300081d8 <DDR_PHY_DLL_CLK_DIV+0x1a4>)
30008190:	601a      	str	r2, [r3, #0]
30008192:	e7bd      	b.n	30008110 <DDR_PHY_DLL_CLK_DIV+0xdc>
30008194:	4b0d      	ldr	r3, [pc, #52]	; (300081cc <DDR_PHY_DLL_CLK_DIV+0x198>)
30008196:	4798      	blx	r3
30008198:	2801      	cmp	r0, #1
3000819a:	7030      	strb	r0, [r6, #0]
3000819c:	d1b5      	bne.n	3000810a <DDR_PHY_DLL_CLK_DIV+0xd6>
3000819e:	e7ea      	b.n	30008176 <DDR_PHY_DLL_CLK_DIV+0x142>
300081a0:	cccccccd 	.word	0xcccccccd
300081a4:	41011000 	.word	0x41011000
300081a8:	001fff88 	.word	0x001fff88
300081ac:	f800ffff 	.word	0xf800ffff
300081b0:	07ff0000 	.word	0x07ff0000
300081b4:	fffff800 	.word	0xfffff800
300081b8:	3000f15d 	.word	0x3000f15d
300081bc:	3000d494 	.word	0x3000d494
300081c0:	3000fe70 	.word	0x3000fe70
300081c4:	42008000 	.word	0x42008000
300081c8:	4c000821 	.word	0x4c000821
300081cc:	30007e41 	.word	0x30007e41
300081d0:	40000021 	.word	0x40000021
300081d4:	4c000903 	.word	0x4c000903
300081d8:	40000103 	.word	0x40000103

300081dc <DDR_PHY_CRT_Init>:
300081dc:	492f      	ldr	r1, [pc, #188]	; (3000829c <DDR_PHY_CRT_Init+0xc0>)
300081de:	2005      	movs	r0, #5
300081e0:	4a2f      	ldr	r2, [pc, #188]	; (300082a0 <DDR_PHY_CRT_Init+0xc4>)
300081e2:	b510      	push	{r4, lr}
300081e4:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
300081e8:	4c2e      	ldr	r4, [pc, #184]	; (300082a4 <DDR_PHY_CRT_Init+0xc8>)
300081ea:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300081ee:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
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
3000822a:	4790      	blx	r2
3000822c:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008230:	f641 73ff 	movw	r3, #8191	; 0x1fff
30008234:	491c      	ldr	r1, [pc, #112]	; (300082a8 <DDR_PHY_CRT_Init+0xcc>)
30008236:	f042 0204 	orr.w	r2, r2, #4
3000823a:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000823e:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008242:	f042 0208 	orr.w	r2, r2, #8
30008246:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000824a:	6aa2      	ldr	r2, [r4, #40]	; 0x28
3000824c:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
30008250:	62a2      	str	r2, [r4, #40]	; 0x28
30008252:	6aa2      	ldr	r2, [r4, #40]	; 0x28
30008254:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
30008258:	62a2      	str	r2, [r4, #40]	; 0x28
3000825a:	69e2      	ldr	r2, [r4, #28]
3000825c:	f042 0208 	orr.w	r2, r2, #8
30008260:	61e2      	str	r2, [r4, #28]
30008262:	68a2      	ldr	r2, [r4, #8]
30008264:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008268:	60a2      	str	r2, [r4, #8]
3000826a:	6862      	ldr	r2, [r4, #4]
3000826c:	4313      	orrs	r3, r2
3000826e:	6063      	str	r3, [r4, #4]
30008270:	68a3      	ldr	r3, [r4, #8]
30008272:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
30008276:	60a3      	str	r3, [r4, #8]
30008278:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
3000827c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008280:	f023 0303 	bic.w	r3, r3, #3
30008284:	f043 0302 	orr.w	r3, r3, #2
30008288:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000828c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008290:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30008294:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008298:	bd10      	pop	{r4, pc}
3000829a:	bf00      	nop
3000829c:	42008000 	.word	0x42008000
300082a0:	00009b2d 	.word	0x00009b2d
300082a4:	41011000 	.word	0x41011000
300082a8:	90161f00 	.word	0x90161f00

300082ac <DDR_PHY_R240_ZQ_CAL>:
300082ac:	b538      	push	{r3, r4, r5, lr}
300082ae:	4b1e      	ldr	r3, [pc, #120]	; (30008328 <DDR_PHY_R240_ZQ_CAL+0x7c>)
300082b0:	7818      	ldrb	r0, [r3, #0]
300082b2:	28ff      	cmp	r0, #255	; 0xff
300082b4:	d034      	beq.n	30008320 <DDR_PHY_R240_ZQ_CAL+0x74>
300082b6:	4a1d      	ldr	r2, [pc, #116]	; (3000832c <DDR_PHY_R240_ZQ_CAL+0x80>)
300082b8:	eb00 0080 	add.w	r0, r0, r0, lsl #2
300082bc:	4b1c      	ldr	r3, [pc, #112]	; (30008330 <DDR_PHY_R240_ZQ_CAL+0x84>)
300082be:	2400      	movs	r4, #0
300082c0:	eb02 0180 	add.w	r1, r2, r0, lsl #2
300082c4:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
300082c8:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
300082cc:	7c0a      	ldrb	r2, [r1, #16]
300082ce:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
300082d2:	0052      	lsls	r2, r2, #1
300082d4:	f002 023e 	and.w	r2, r2, #62	; 0x3e
300082d8:	4302      	orrs	r2, r0
300082da:	6848      	ldr	r0, [r1, #4]
300082dc:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
300082e0:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
300082e4:	4a13      	ldr	r2, [pc, #76]	; (30008334 <DDR_PHY_R240_ZQ_CAL+0x88>)
300082e6:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
300082ea:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300082ee:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
300082f2:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
300082f6:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
300082fa:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300082fe:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
30008302:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30008306:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000830a:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
3000830e:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008312:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30008316:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
3000831a:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000831e:	bd38      	pop	{r3, r4, r5, pc}
30008320:	f7ff fe24 	bl	30007f6c <DDR_PHY_ChipInfo.part.0>
30008324:	e7c7      	b.n	300082b6 <DDR_PHY_R240_ZQ_CAL+0xa>
30008326:	bf00      	nop
30008328:	3000f15d 	.word	0x3000f15d
3000832c:	3000d5d4 	.word	0x3000d5d4
30008330:	41011000 	.word	0x41011000
30008334:	00262626 	.word	0x00262626

30008338 <DDR_PHY_ZQ_SET_SEL>:
30008338:	b410      	push	{r4}
3000833a:	4b25      	ldr	r3, [pc, #148]	; (300083d0 <DDR_PHY_ZQ_SET_SEL+0x98>)
3000833c:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008340:	4a24      	ldr	r2, [pc, #144]	; (300083d4 <DDR_PHY_ZQ_SET_SEL+0x9c>)
30008342:	4c25      	ldr	r4, [pc, #148]	; (300083d8 <DDR_PHY_ZQ_SET_SEL+0xa0>)
30008344:	4925      	ldr	r1, [pc, #148]	; (300083dc <DDR_PHY_ZQ_SET_SEL+0xa4>)
30008346:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
3000834a:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
3000834e:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
30008352:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
30008356:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
3000835a:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
3000835e:	f64f 74ff 	movw	r4, #65535	; 0xffff
30008362:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
30008366:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
3000836a:	2200      	movs	r2, #0
3000836c:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30008370:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
30008374:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30008378:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
3000837c:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008380:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30008384:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30008388:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
3000838c:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30008390:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30008394:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30008398:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
3000839c:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
300083a0:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
300083a4:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
300083a8:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
300083ac:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
300083b0:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
300083b4:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
300083b8:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
300083bc:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
300083c0:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
300083c4:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
300083c8:	f85d 4b04 	ldr.w	r4, [sp], #4
300083cc:	4770      	bx	lr
300083ce:	bf00      	nop
300083d0:	41011000 	.word	0x41011000
300083d4:	ffff1111 	.word	0xffff1111
300083d8:	ffff3333 	.word	0xffff3333
300083dc:	ffff0000 	.word	0xffff0000

300083e0 <DDR_PHY_DELAY_TAP_SET>:
300083e0:	b510      	push	{r4, lr}
300083e2:	4b15      	ldr	r3, [pc, #84]	; (30008438 <DDR_PHY_DELAY_TAP_SET+0x58>)
300083e4:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
300083e8:	4914      	ldr	r1, [pc, #80]	; (3000843c <DDR_PHY_DELAY_TAP_SET+0x5c>)
300083ea:	4c15      	ldr	r4, [pc, #84]	; (30008440 <DDR_PHY_DELAY_TAP_SET+0x60>)
300083ec:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
300083f0:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
300083f4:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300083f8:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
300083fc:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30008400:	7820      	ldrb	r0, [r4, #0]
30008402:	b1a0      	cbz	r0, 3000842e <DDR_PHY_DELAY_TAP_SET+0x4e>
30008404:	2801      	cmp	r0, #1
30008406:	4b0c      	ldr	r3, [pc, #48]	; (30008438 <DDR_PHY_DELAY_TAP_SET+0x58>)
30008408:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
3000840c:	bf0c      	ite	eq
3000840e:	4a0d      	ldreq	r2, [pc, #52]	; (30008444 <DDR_PHY_DELAY_TAP_SET+0x64>)
30008410:	4a0d      	ldrne	r2, [pc, #52]	; (30008448 <DDR_PHY_DELAY_TAP_SET+0x68>)
30008412:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
30008416:	f640 0288 	movw	r2, #2184	; 0x888
3000841a:	4b07      	ldr	r3, [pc, #28]	; (30008438 <DDR_PHY_DELAY_TAP_SET+0x58>)
3000841c:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30008420:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30008424:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30008428:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
3000842c:	bd10      	pop	{r4, pc}
3000842e:	4b07      	ldr	r3, [pc, #28]	; (3000844c <DDR_PHY_DELAY_TAP_SET+0x6c>)
30008430:	4798      	blx	r3
30008432:	7020      	strb	r0, [r4, #0]
30008434:	e7e6      	b.n	30008404 <DDR_PHY_DELAY_TAP_SET+0x24>
30008436:	bf00      	nop
30008438:	41011000 	.word	0x41011000
3000843c:	20888888 	.word	0x20888888
30008440:	3000fe70 	.word	0x3000fe70
30008444:	7c033333 	.word	0x7c033333
30008448:	7c011111 	.word	0x7c011111
3000844c:	30007e41 	.word	0x30007e41

30008450 <DDR_PHY_READ_CTRL>:
30008450:	4b10      	ldr	r3, [pc, #64]	; (30008494 <DDR_PHY_READ_CTRL+0x44>)
30008452:	b510      	push	{r4, lr}
30008454:	7818      	ldrb	r0, [r3, #0]
30008456:	28ff      	cmp	r0, #255	; 0xff
30008458:	d019      	beq.n	3000848e <DDR_PHY_READ_CTRL+0x3e>
3000845a:	4a0f      	ldr	r2, [pc, #60]	; (30008498 <DDR_PHY_READ_CTRL+0x48>)
3000845c:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30008460:	4b0e      	ldr	r3, [pc, #56]	; (3000849c <DDR_PHY_READ_CTRL+0x4c>)
30008462:	2108      	movs	r1, #8
30008464:	eb02 0080 	add.w	r0, r2, r0, lsl #2
30008468:	22fe      	movs	r2, #254	; 0xfe
3000846a:	7c44      	ldrb	r4, [r0, #17]
3000846c:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
30008470:	7c84      	ldrb	r4, [r0, #18]
30008472:	7cc0      	ldrb	r0, [r0, #19]
30008474:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
30008478:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
3000847c:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30008480:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
30008484:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30008488:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
3000848c:	bd10      	pop	{r4, pc}
3000848e:	f7ff fd6d 	bl	30007f6c <DDR_PHY_ChipInfo.part.0>
30008492:	e7e2      	b.n	3000845a <DDR_PHY_READ_CTRL+0xa>
30008494:	3000f15d 	.word	0x3000f15d
30008498:	3000d5d4 	.word	0x3000d5d4
3000849c:	41011000 	.word	0x41011000

300084a0 <DDR_PHY_READ_LEVELING>:
300084a0:	b508      	push	{r3, lr}
300084a2:	4b21      	ldr	r3, [pc, #132]	; (30008528 <DDR_PHY_READ_LEVELING+0x88>)
300084a4:	7818      	ldrb	r0, [r3, #0]
300084a6:	28ff      	cmp	r0, #255	; 0xff
300084a8:	d03b      	beq.n	30008522 <DDR_PHY_READ_LEVELING+0x82>
300084aa:	4b20      	ldr	r3, [pc, #128]	; (3000852c <DDR_PHY_READ_LEVELING+0x8c>)
300084ac:	b1e8      	cbz	r0, 300084ea <DDR_PHY_READ_LEVELING+0x4a>
300084ae:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
300084b2:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300084b6:	220f      	movs	r2, #15
300084b8:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
300084bc:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
300084c0:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
300084c4:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
300084c8:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
300084cc:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
300084d0:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
300084d4:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
300084d8:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
300084dc:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
300084e0:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
300084e4:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
300084e8:	bd08      	pop	{r3, pc}
300084ea:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300084ee:	2110      	movs	r1, #16
300084f0:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300084f4:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
300084f8:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
300084fc:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30008500:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30008504:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30008508:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
3000850c:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30008510:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30008514:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30008518:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
3000851c:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30008520:	bd08      	pop	{r3, pc}
30008522:	f7ff fd23 	bl	30007f6c <DDR_PHY_ChipInfo.part.0>
30008526:	e7c0      	b.n	300084aa <DDR_PHY_READ_LEVELING+0xa>
30008528:	3000f15d 	.word	0x3000f15d
3000852c:	41011000 	.word	0x41011000

30008530 <DDR_PHY_Init>:
30008530:	b510      	push	{r4, lr}
30008532:	6d80      	ldr	r0, [r0, #88]	; 0x58
30008534:	4c2f      	ldr	r4, [pc, #188]	; (300085f4 <DDR_PHY_Init+0xc4>)
30008536:	f7ff fd7d 	bl	30008034 <DDR_PHY_DLL_CLK_DIV>
3000853a:	f7ff fe4f 	bl	300081dc <DDR_PHY_CRT_Init>
3000853e:	f7ff feb5 	bl	300082ac <DDR_PHY_R240_ZQ_CAL>
30008542:	f7ff fef9 	bl	30008338 <DDR_PHY_ZQ_SET_SEL>
30008546:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
3000854a:	2300      	movs	r3, #0
3000854c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30008550:	4829      	ldr	r0, [pc, #164]	; (300085f8 <DDR_PHY_Init+0xc8>)
30008552:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30008556:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
3000855a:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
3000855e:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
30008562:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30008566:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
3000856a:	f7ff ff39 	bl	300083e0 <DDR_PHY_DELAY_TAP_SET>
3000856e:	f7ff ff6f 	bl	30008450 <DDR_PHY_READ_CTRL>
30008572:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30008576:	4a21      	ldr	r2, [pc, #132]	; (300085fc <DDR_PHY_Init+0xcc>)
30008578:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
3000857c:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008580:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30008584:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008588:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
3000858c:	f7ff ff88 	bl	300084a0 <DDR_PHY_READ_LEVELING>
30008590:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30008594:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008598:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
3000859c:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
300085a0:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
300085a4:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
300085a8:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
300085ac:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
300085b0:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
300085b4:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
300085b8:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
300085bc:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300085c0:	f043 0303 	orr.w	r3, r3, #3
300085c4:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300085c8:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300085cc:	f043 030c 	orr.w	r3, r3, #12
300085d0:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300085d4:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300085d8:	f023 0303 	bic.w	r3, r3, #3
300085dc:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300085e0:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300085e4:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
300085e8:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
300085ec:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300085f0:	bd10      	pop	{r4, pc}
300085f2:	bf00      	nop
300085f4:	41011000 	.word	0x41011000
300085f8:	00045500 	.word	0x00045500
300085fc:	11117777 	.word	0x11117777

30008600 <DDR_PHY_CAL>:
30008600:	4770      	bx	lr
30008602:	bf00      	nop

30008604 <DDR_PHY_AutoGating>:
30008604:	4a0c      	ldr	r2, [pc, #48]	; (30008638 <DDR_PHY_AutoGating+0x34>)
30008606:	2032      	movs	r0, #50	; 0x32
30008608:	490c      	ldr	r1, [pc, #48]	; (3000863c <DDR_PHY_AutoGating+0x38>)
3000860a:	b508      	push	{r3, lr}
3000860c:	6893      	ldr	r3, [r2, #8]
3000860e:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30008612:	6093      	str	r3, [r2, #8]
30008614:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30008618:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
3000861c:	f043 0314 	orr.w	r3, r3, #20
30008620:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008624:	4788      	blx	r1
30008626:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000862a:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
3000862e:	f043 0303 	orr.w	r3, r3, #3
30008632:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30008636:	bd08      	pop	{r3, pc}
30008638:	41011000 	.word	0x41011000
3000863c:	00009b2d 	.word	0x00009b2d

30008640 <flash_init_userdef>:
30008640:	4b14      	ldr	r3, [pc, #80]	; (30008694 <flash_init_userdef+0x54>)
30008642:	2201      	movs	r2, #1
30008644:	2140      	movs	r1, #64	; 0x40
30008646:	f44f 6000 	mov.w	r0, #2048	; 0x800
3000864a:	b430      	push	{r4, r5}
3000864c:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008650:	2502      	movs	r5, #2
30008652:	2400      	movs	r4, #0
30008654:	f640 0104 	movw	r1, #2052	; 0x804
30008658:	601a      	str	r2, [r3, #0]
3000865a:	22bb      	movs	r2, #187	; 0xbb
3000865c:	83d8      	strh	r0, [r3, #30]
3000865e:	8419      	strh	r1, [r3, #32]
30008660:	203b      	movs	r0, #59	; 0x3b
30008662:	21eb      	movs	r1, #235	; 0xeb
30008664:	629a      	str	r2, [r3, #40]	; 0x28
30008666:	4a0c      	ldr	r2, [pc, #48]	; (30008698 <flash_init_userdef+0x58>)
30008668:	6258      	str	r0, [r3, #36]	; 0x24
3000866a:	6319      	str	r1, [r3, #48]	; 0x30
3000866c:	f64a 3120 	movw	r1, #43808	; 0xab20
30008670:	655a      	str	r2, [r3, #84]	; 0x54
30008672:	22b9      	movs	r2, #185	; 0xb9
30008674:	4809      	ldr	r0, [pc, #36]	; (3000869c <flash_init_userdef+0x5c>)
30008676:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
3000867a:	6598      	str	r0, [r3, #88]	; 0x58
3000867c:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008680:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008684:	2406      	movs	r4, #6
30008686:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
3000868a:	246b      	movs	r4, #107	; 0x6b
3000868c:	62dc      	str	r4, [r3, #44]	; 0x2c
3000868e:	bc30      	pop	{r4, r5}
30008690:	4770      	bx	lr
30008692:	bf00      	nop
30008694:	2001c01c 	.word	0x2001c01c
30008698:	00059f06 	.word	0x00059f06
3000869c:	d8600001 	.word	0xd8600001

300086a0 <nand_init_userdef>:
300086a0:	4b18      	ldr	r3, [pc, #96]	; (30008704 <nand_init_userdef+0x64>)
300086a2:	2001      	movs	r0, #1
300086a4:	2200      	movs	r2, #0
300086a6:	2102      	movs	r1, #2
300086a8:	b410      	push	{r4}
300086aa:	6018      	str	r0, [r3, #0]
300086ac:	f640 0408 	movw	r4, #2056	; 0x808
300086b0:	6159      	str	r1, [r3, #20]
300086b2:	83dc      	strh	r4, [r3, #30]
300086b4:	f44f 6400 	mov.w	r4, #2048	; 0x800
300086b8:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
300086bc:	841c      	strh	r4, [r3, #32]
300086be:	246b      	movs	r4, #107	; 0x6b
300086c0:	619a      	str	r2, [r3, #24]
300086c2:	62dc      	str	r4, [r3, #44]	; 0x2c
300086c4:	4c10      	ldr	r4, [pc, #64]	; (30008708 <nand_init_userdef+0x68>)
300086c6:	775a      	strb	r2, [r3, #29]
300086c8:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
300086cc:	629a      	str	r2, [r3, #40]	; 0x28
300086ce:	631a      	str	r2, [r3, #48]	; 0x30
300086d0:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
300086d4:	65da      	str	r2, [r3, #92]	; 0x5c
300086d6:	e9c3 0003 	strd	r0, r0, [r3, #12]
300086da:	203b      	movs	r0, #59	; 0x3b
300086dc:	6258      	str	r0, [r3, #36]	; 0x24
300086de:	200b      	movs	r0, #11
300086e0:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
300086e4:	6698      	str	r0, [r3, #104]	; 0x68
300086e6:	2008      	movs	r0, #8
300086e8:	4908      	ldr	r1, [pc, #32]	; (3000870c <nand_init_userdef+0x6c>)
300086ea:	64d8      	str	r0, [r3, #76]	; 0x4c
300086ec:	f241 0013 	movw	r0, #4115	; 0x1013
300086f0:	6599      	str	r1, [r3, #88]	; 0x58
300086f2:	4907      	ldr	r1, [pc, #28]	; (30008710 <nand_init_userdef+0x70>)
300086f4:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
300086f8:	6719      	str	r1, [r3, #112]	; 0x70
300086fa:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
300086fe:	f85d 4b04 	ldr.w	r4, [sp], #4
30008702:	4770      	bx	lr
30008704:	2001c01c 	.word	0x2001c01c
30008708:	000f9f06 	.word	0x000f9f06
3000870c:	d800001f 	.word	0xd800001f
30008710:	10300804 	.word	0x10300804

30008714 <flash_get_layout_info>:
30008714:	b570      	push	{r4, r5, r6, lr}
30008716:	4b0f      	ldr	r3, [pc, #60]	; (30008754 <flash_get_layout_info+0x40>)
30008718:	4615      	mov	r5, r2
3000871a:	4604      	mov	r4, r0
3000871c:	460e      	mov	r6, r1
3000871e:	4798      	blx	r3
30008720:	4b0d      	ldr	r3, [pc, #52]	; (30008758 <flash_get_layout_info+0x44>)
30008722:	4a0e      	ldr	r2, [pc, #56]	; (3000875c <flash_get_layout_info+0x48>)
30008724:	2800      	cmp	r0, #0
30008726:	bf14      	ite	ne
30008728:	469c      	movne	ip, r3
3000872a:	4694      	moveq	ip, r2
3000872c:	f8dc 3000 	ldr.w	r3, [ip]
30008730:	2bff      	cmp	r3, #255	; 0xff
30008732:	d104      	bne.n	3000873e <flash_get_layout_info+0x2a>
30008734:	e00d      	b.n	30008752 <flash_get_layout_info+0x3e>
30008736:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
3000873a:	2bff      	cmp	r3, #255	; 0xff
3000873c:	d009      	beq.n	30008752 <flash_get_layout_info+0x3e>
3000873e:	429c      	cmp	r4, r3
30008740:	d1f9      	bne.n	30008736 <flash_get_layout_info+0x22>
30008742:	b116      	cbz	r6, 3000874a <flash_get_layout_info+0x36>
30008744:	f8dc 3004 	ldr.w	r3, [ip, #4]
30008748:	6033      	str	r3, [r6, #0]
3000874a:	b115      	cbz	r5, 30008752 <flash_get_layout_info+0x3e>
3000874c:	f8dc 3008 	ldr.w	r3, [ip, #8]
30008750:	602b      	str	r3, [r5, #0]
30008752:	bd70      	pop	{r4, r5, r6, pc}
30008754:	30009e75 	.word	0x30009e75
30008758:	3000f238 	.word	0x3000f238
3000875c:	3000f160 	.word	0x3000f160

30008760 <NAND_CHECK_IS_BAD_BLOCK>:
30008760:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008764:	4e24      	ldr	r6, [pc, #144]	; (300087f8 <NAND_CHECK_IS_BAD_BLOCK+0x98>)
30008766:	0907      	lsrs	r7, r0, #4
30008768:	0045      	lsls	r5, r0, #1
3000876a:	b085      	sub	sp, #20
3000876c:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008770:	f005 051e 	and.w	r5, r5, #30
30008774:	40eb      	lsrs	r3, r5
30008776:	f003 0303 	and.w	r3, r3, #3
3000877a:	2b02      	cmp	r3, #2
3000877c:	d02c      	beq.n	300087d8 <NAND_CHECK_IS_BAD_BLOCK+0x78>
3000877e:	2b01      	cmp	r3, #1
30008780:	d026      	beq.n	300087d0 <NAND_CHECK_IS_BAD_BLOCK+0x70>
30008782:	4b1e      	ldr	r3, [pc, #120]	; (300087fc <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008784:	f04f 0801 	mov.w	r8, #1
30008788:	2204      	movs	r2, #4
3000878a:	4604      	mov	r4, r0
3000878c:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008790:	0180      	lsls	r0, r0, #6
30008792:	ab03      	add	r3, sp, #12
30008794:	f8df 9070 	ldr.w	r9, [pc, #112]	; 30008808 <NAND_CHECK_IS_BAD_BLOCK+0xa8>
30008798:	fa08 f101 	lsl.w	r1, r8, r1
3000879c:	47c8      	blx	r9
3000879e:	f89d 300c 	ldrb.w	r3, [sp, #12]
300087a2:	f89d 200d 	ldrb.w	r2, [sp, #13]
300087a6:	4013      	ands	r3, r2
300087a8:	2bff      	cmp	r3, #255	; 0xff
300087aa:	d019      	beq.n	300087e0 <NAND_CHECK_IS_BAD_BLOCK+0x80>
300087ac:	2002      	movs	r0, #2
300087ae:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
300087b2:	4b13      	ldr	r3, [pc, #76]	; (30008800 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
300087b4:	fa00 f505 	lsl.w	r5, r0, r5
300087b8:	9400      	str	r4, [sp, #0]
300087ba:	4619      	mov	r1, r3
300087bc:	432a      	orrs	r2, r5
300087be:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
300087c2:	4a10      	ldr	r2, [pc, #64]	; (30008804 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
300087c4:	f003 fcc4 	bl	3000c150 <rtk_log_write>
300087c8:	4640      	mov	r0, r8
300087ca:	b005      	add	sp, #20
300087cc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087d0:	2000      	movs	r0, #0
300087d2:	b005      	add	sp, #20
300087d4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087d8:	2001      	movs	r0, #1
300087da:	b005      	add	sp, #20
300087dc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087e0:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
300087e4:	fa08 f005 	lsl.w	r0, r8, r5
300087e8:	4303      	orrs	r3, r0
300087ea:	2000      	movs	r0, #0
300087ec:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
300087f0:	b005      	add	sp, #20
300087f2:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087f6:	bf00      	nop
300087f8:	3000fe78 	.word	0x3000fe78
300087fc:	2001c01c 	.word	0x2001c01c
30008800:	3000d868 	.word	0x3000d868
30008804:	3000d874 	.word	0x3000d874
30008808:	30009dc5 	.word	0x30009dc5

3000880c <Nand_Get_NandAddr>:
3000880c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008810:	4f10      	ldr	r7, [pc, #64]	; (30008854 <Nand_Get_NandAddr+0x48>)
30008812:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008816:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
3000881a:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
3000881e:	40e8      	lsrs	r0, r5
30008820:	fa26 f505 	lsr.w	r5, r6, r5
30008824:	09ad      	lsrs	r5, r5, #6
30008826:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
3000882a:	d90f      	bls.n	3000884c <Nand_Get_NandAddr+0x40>
3000882c:	0984      	lsrs	r4, r0, #6
3000882e:	f04f 0840 	mov.w	r8, #64	; 0x40
30008832:	4620      	mov	r0, r4
30008834:	3401      	adds	r4, #1
30008836:	f7ff ff93 	bl	30008760 <NAND_CHECK_IS_BAD_BLOCK>
3000883a:	b128      	cbz	r0, 30008848 <Nand_Get_NandAddr+0x3c>
3000883c:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008840:	3501      	adds	r5, #1
30008842:	fa08 f303 	lsl.w	r3, r8, r3
30008846:	441e      	add	r6, r3
30008848:	42a5      	cmp	r5, r4
3000884a:	d8f2      	bhi.n	30008832 <Nand_Get_NandAddr+0x26>
3000884c:	4630      	mov	r0, r6
3000884e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008852:	bf00      	nop
30008854:	2001c01c 	.word	0x2001c01c

30008858 <Nand_L2P_Table>:
30008858:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000885c:	b082      	sub	sp, #8
3000885e:	4604      	mov	r4, r0
30008860:	2002      	movs	r0, #2
30008862:	aa01      	add	r2, sp, #4
30008864:	4669      	mov	r1, sp
30008866:	f7ff ff55 	bl	30008714 <flash_get_layout_info>
3000886a:	9800      	ldr	r0, [sp, #0]
3000886c:	42a0      	cmp	r0, r4
3000886e:	d810      	bhi.n	30008892 <Nand_L2P_Table+0x3a>
30008870:	9b01      	ldr	r3, [sp, #4]
30008872:	42a3      	cmp	r3, r4
30008874:	d805      	bhi.n	30008882 <Nand_L2P_Table+0x2a>
30008876:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
3000887a:	4620      	mov	r0, r4
3000887c:	b002      	add	sp, #8
3000887e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008882:	4621      	mov	r1, r4
30008884:	f7ff ffc2 	bl	3000880c <Nand_Get_NandAddr>
30008888:	4604      	mov	r4, r0
3000888a:	4620      	mov	r0, r4
3000888c:	b002      	add	sp, #8
3000888e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008892:	4f0e      	ldr	r7, [pc, #56]	; (300088cc <Nand_L2P_Table+0x74>)
30008894:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30008898:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
3000889c:	fa24 f606 	lsr.w	r6, r4, r6
300088a0:	09b6      	lsrs	r6, r6, #6
300088a2:	d0ea      	beq.n	3000887a <Nand_L2P_Table+0x22>
300088a4:	2500      	movs	r5, #0
300088a6:	f04f 0840 	mov.w	r8, #64	; 0x40
300088aa:	4628      	mov	r0, r5
300088ac:	3501      	adds	r5, #1
300088ae:	f7ff ff57 	bl	30008760 <NAND_CHECK_IS_BAD_BLOCK>
300088b2:	b128      	cbz	r0, 300088c0 <Nand_L2P_Table+0x68>
300088b4:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
300088b8:	3601      	adds	r6, #1
300088ba:	fa08 f303 	lsl.w	r3, r8, r3
300088be:	441c      	add	r4, r3
300088c0:	42b5      	cmp	r5, r6
300088c2:	d3f2      	bcc.n	300088aa <Nand_L2P_Table+0x52>
300088c4:	4620      	mov	r0, r4
300088c6:	b002      	add	sp, #8
300088c8:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300088cc:	2001c01c 	.word	0x2001c01c

300088d0 <NandImgCopy>:
300088d0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300088d4:	4617      	mov	r7, r2
300088d6:	4603      	mov	r3, r0
300088d8:	b089      	sub	sp, #36	; 0x24
300088da:	f8df 9100 	ldr.w	r9, [pc, #256]	; 300089dc <NandImgCopy+0x10c>
300088de:	f107 38ff 	add.w	r8, r7, #4294967295	; 0xffffffff
300088e2:	4608      	mov	r0, r1
300088e4:	9307      	str	r3, [sp, #28]
300088e6:	f7ff ffb7 	bl	30008858 <Nand_L2P_Table>
300088ea:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
300088ee:	4480      	add	r8, r0
300088f0:	fa20 f406 	lsr.w	r4, r0, r6
300088f4:	fa28 f806 	lsr.w	r8, r8, r6
300088f8:	eba8 0804 	sub.w	r8, r8, r4
300088fc:	f1b8 3fff 	cmp.w	r8, #4294967295	; 0xffffffff
30008900:	d054      	beq.n	300089ac <NandImgCopy+0xdc>
30008902:	2300      	movs	r3, #0
30008904:	4605      	mov	r5, r0
30008906:	f04f 0b40 	mov.w	fp, #64	; 0x40
3000890a:	469a      	mov	sl, r3
3000890c:	9306      	str	r3, [sp, #24]
3000890e:	463b      	mov	r3, r7
30008910:	4647      	mov	r7, r8
30008912:	4698      	mov	r8, r3
30008914:	e020      	b.n	30008958 <NandImgCopy+0x88>
30008916:	f1c2 0320 	rsb	r3, r2, #32
3000891a:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000891e:	40d9      	lsrs	r1, r3
30008920:	9b07      	ldr	r3, [sp, #28]
30008922:	4029      	ands	r1, r5
30008924:	4453      	add	r3, sl
30008926:	b377      	cbz	r7, 30008986 <NandImgCopy+0xb6>
30008928:	2601      	movs	r6, #1
3000892a:	4620      	mov	r0, r4
3000892c:	9105      	str	r1, [sp, #20]
3000892e:	fa06 f202 	lsl.w	r2, r6, r2
30008932:	4e27      	ldr	r6, [pc, #156]	; (300089d0 <NandImgCopy+0x100>)
30008934:	9204      	str	r2, [sp, #16]
30008936:	1a52      	subs	r2, r2, r1
30008938:	4492      	add	sl, r2
3000893a:	4415      	add	r5, r2
3000893c:	47b0      	blx	r6
3000893e:	f010 0f70 	tst.w	r0, #112	; 0x70
30008942:	9b04      	ldr	r3, [sp, #16]
30008944:	9905      	ldr	r1, [sp, #20]
30008946:	d134      	bne.n	300089b2 <NandImgCopy+0xe2>
30008948:	eba8 0803 	sub.w	r8, r8, r3
3000894c:	4488      	add	r8, r1
3000894e:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008952:	3f01      	subs	r7, #1
30008954:	fa25 f406 	lsr.w	r4, r5, r6
30008958:	9b06      	ldr	r3, [sp, #24]
3000895a:	4632      	mov	r2, r6
3000895c:	09a6      	lsrs	r6, r4, #6
3000895e:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008962:	d8d8      	bhi.n	30008916 <NandImgCopy+0x46>
30008964:	e005      	b.n	30008972 <NandImgCopy+0xa2>
30008966:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
3000896a:	3440      	adds	r4, #64	; 0x40
3000896c:	fa0b f303 	lsl.w	r3, fp, r3
30008970:	441d      	add	r5, r3
30008972:	4630      	mov	r0, r6
30008974:	3601      	adds	r6, #1
30008976:	f7ff fef3 	bl	30008760 <NAND_CHECK_IS_BAD_BLOCK>
3000897a:	2800      	cmp	r0, #0
3000897c:	d1f3      	bne.n	30008966 <NandImgCopy+0x96>
3000897e:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008982:	9606      	str	r6, [sp, #24]
30008984:	e7c7      	b.n	30008916 <NandImgCopy+0x46>
30008986:	4642      	mov	r2, r8
30008988:	4620      	mov	r0, r4
3000898a:	4d11      	ldr	r5, [pc, #68]	; (300089d0 <NandImgCopy+0x100>)
3000898c:	47a8      	blx	r5
3000898e:	f010 0f70 	tst.w	r0, #112	; 0x70
30008992:	d00b      	beq.n	300089ac <NandImgCopy+0xdc>
30008994:	09a1      	lsrs	r1, r4, #6
30008996:	4b0f      	ldr	r3, [pc, #60]	; (300089d4 <NandImgCopy+0x104>)
30008998:	f004 043f 	and.w	r4, r4, #63	; 0x3f
3000899c:	4a0e      	ldr	r2, [pc, #56]	; (300089d8 <NandImgCopy+0x108>)
3000899e:	e9cd 1001 	strd	r1, r0, [sp, #4]
300089a2:	9400      	str	r4, [sp, #0]
300089a4:	4619      	mov	r1, r3
300089a6:	2004      	movs	r0, #4
300089a8:	f003 fbd2 	bl	3000c150 <rtk_log_write>
300089ac:	b009      	add	sp, #36	; 0x24
300089ae:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300089b2:	1ac9      	subs	r1, r1, r3
300089b4:	09a3      	lsrs	r3, r4, #6
300089b6:	f004 043f 	and.w	r4, r4, #63	; 0x3f
300089ba:	9002      	str	r0, [sp, #8]
300089bc:	9301      	str	r3, [sp, #4]
300089be:	4488      	add	r8, r1
300089c0:	4b04      	ldr	r3, [pc, #16]	; (300089d4 <NandImgCopy+0x104>)
300089c2:	2004      	movs	r0, #4
300089c4:	4a04      	ldr	r2, [pc, #16]	; (300089d8 <NandImgCopy+0x108>)
300089c6:	4619      	mov	r1, r3
300089c8:	9400      	str	r4, [sp, #0]
300089ca:	f003 fbc1 	bl	3000c150 <rtk_log_write>
300089ce:	e7be      	b.n	3000894e <NandImgCopy+0x7e>
300089d0:	30009dc5 	.word	0x30009dc5
300089d4:	3000d868 	.word	0x3000d868
300089d8:	3000d88c 	.word	0x3000d88c
300089dc:	2001c01c 	.word	0x2001c01c

300089e0 <OTP_Read8.part.0>:
300089e0:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300089e4:	4b30      	ldr	r3, [pc, #192]	; (30008aa8 <OTP_Read8.part.0+0xc8>)
300089e6:	4680      	mov	r8, r0
300089e8:	e843 f300 	tt	r3, r3
300089ec:	f3c3 5380 	ubfx	r3, r3, #22, #1
300089f0:	4a2e      	ldr	r2, [pc, #184]	; (30008aac <OTP_Read8.part.0+0xcc>)
300089f2:	4e2f      	ldr	r6, [pc, #188]	; (30008ab0 <OTP_Read8.part.0+0xd0>)
300089f4:	b082      	sub	sp, #8
300089f6:	2b00      	cmp	r3, #0
300089f8:	4689      	mov	r9, r1
300089fa:	bf0e      	itee	eq
300089fc:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008a00:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008a04:	4616      	movne	r6, r2
30008a06:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a08:	05d8      	lsls	r0, r3, #23
30008a0a:	d505      	bpl.n	30008a18 <OTP_Read8.part.0+0x38>
30008a0c:	4d29      	ldr	r5, [pc, #164]	; (30008ab4 <OTP_Read8.part.0+0xd4>)
30008a0e:	2064      	movs	r0, #100	; 0x64
30008a10:	47a8      	blx	r5
30008a12:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a14:	05d9      	lsls	r1, r3, #23
30008a16:	d4fa      	bmi.n	30008a0e <OTP_Read8.part.0+0x2e>
30008a18:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a1a:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008a1e:	643b      	str	r3, [r7, #64]	; 0x40
30008a20:	4b25      	ldr	r3, [pc, #148]	; (30008ab8 <OTP_Read8.part.0+0xd8>)
30008a22:	681a      	ldr	r2, [r3, #0]
30008a24:	07d2      	lsls	r2, r2, #31
30008a26:	d403      	bmi.n	30008a30 <OTP_Read8.part.0+0x50>
30008a28:	681a      	ldr	r2, [r3, #0]
30008a2a:	f042 0201 	orr.w	r2, r2, #1
30008a2e:	601a      	str	r2, [r3, #0]
30008a30:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008a34:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008a38:	694a      	ldr	r2, [r1, #20]
30008a3a:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008a3e:	614a      	str	r2, [r1, #20]
30008a40:	6033      	str	r3, [r6, #0]
30008a42:	6833      	ldr	r3, [r6, #0]
30008a44:	2b00      	cmp	r3, #0
30008a46:	db10      	blt.n	30008a6a <OTP_Read8.part.0+0x8a>
30008a48:	2400      	movs	r4, #0
30008a4a:	4d1a      	ldr	r5, [pc, #104]	; (30008ab4 <OTP_Read8.part.0+0xd4>)
30008a4c:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008a50:	e001      	b.n	30008a56 <OTP_Read8.part.0+0x76>
30008a52:	4554      	cmp	r4, sl
30008a54:	d01a      	beq.n	30008a8c <OTP_Read8.part.0+0xac>
30008a56:	2005      	movs	r0, #5
30008a58:	3401      	adds	r4, #1
30008a5a:	47a8      	blx	r5
30008a5c:	6833      	ldr	r3, [r6, #0]
30008a5e:	2b00      	cmp	r3, #0
30008a60:	daf7      	bge.n	30008a52 <OTP_Read8.part.0+0x72>
30008a62:	f644 6320 	movw	r3, #20000	; 0x4e20
30008a66:	429c      	cmp	r4, r3
30008a68:	d010      	beq.n	30008a8c <OTP_Read8.part.0+0xac>
30008a6a:	6833      	ldr	r3, [r6, #0]
30008a6c:	2001      	movs	r0, #1
30008a6e:	f889 3000 	strb.w	r3, [r9]
30008a72:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008a76:	6953      	ldr	r3, [r2, #20]
30008a78:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008a7c:	6153      	str	r3, [r2, #20]
30008a7e:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a80:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008a84:	643b      	str	r3, [r7, #64]	; 0x40
30008a86:	b002      	add	sp, #8
30008a88:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008a8c:	22ff      	movs	r2, #255	; 0xff
30008a8e:	4b0b      	ldr	r3, [pc, #44]	; (30008abc <OTP_Read8.part.0+0xdc>)
30008a90:	2002      	movs	r0, #2
30008a92:	f8cd 8000 	str.w	r8, [sp]
30008a96:	4619      	mov	r1, r3
30008a98:	f889 2000 	strb.w	r2, [r9]
30008a9c:	4a08      	ldr	r2, [pc, #32]	; (30008ac0 <OTP_Read8.part.0+0xe0>)
30008a9e:	f003 fb57 	bl	3000c150 <rtk_log_write>
30008aa2:	2000      	movs	r0, #0
30008aa4:	e7e5      	b.n	30008a72 <OTP_Read8.part.0+0x92>
30008aa6:	bf00      	nop
30008aa8:	0000e5f9 	.word	0x0000e5f9
30008aac:	5200000c 	.word	0x5200000c
30008ab0:	42000008 	.word	0x42000008
30008ab4:	00009b2d 	.word	0x00009b2d
30008ab8:	42008000 	.word	0x42008000
30008abc:	3000d8c8 	.word	0x3000d8c8
30008ac0:	3000d8cc 	.word	0x3000d8cc

30008ac4 <OTP_ProgramMarginRead8.part.0>:
30008ac4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008ac8:	4b31      	ldr	r3, [pc, #196]	; (30008b90 <OTP_ProgramMarginRead8.part.0+0xcc>)
30008aca:	4680      	mov	r8, r0
30008acc:	e843 f300 	tt	r3, r3
30008ad0:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008ad4:	4a2f      	ldr	r2, [pc, #188]	; (30008b94 <OTP_ProgramMarginRead8.part.0+0xd0>)
30008ad6:	4e30      	ldr	r6, [pc, #192]	; (30008b98 <OTP_ProgramMarginRead8.part.0+0xd4>)
30008ad8:	b082      	sub	sp, #8
30008ada:	2b00      	cmp	r3, #0
30008adc:	4689      	mov	r9, r1
30008ade:	bf0e      	itee	eq
30008ae0:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008ae4:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008ae8:	4616      	movne	r6, r2
30008aea:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008aec:	05d8      	lsls	r0, r3, #23
30008aee:	d505      	bpl.n	30008afc <OTP_ProgramMarginRead8.part.0+0x38>
30008af0:	4d2a      	ldr	r5, [pc, #168]	; (30008b9c <OTP_ProgramMarginRead8.part.0+0xd8>)
30008af2:	2064      	movs	r0, #100	; 0x64
30008af4:	47a8      	blx	r5
30008af6:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008af8:	05d9      	lsls	r1, r3, #23
30008afa:	d4fa      	bmi.n	30008af2 <OTP_ProgramMarginRead8.part.0+0x2e>
30008afc:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008afe:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008b02:	643b      	str	r3, [r7, #64]	; 0x40
30008b04:	4b26      	ldr	r3, [pc, #152]	; (30008ba0 <OTP_ProgramMarginRead8.part.0+0xdc>)
30008b06:	681a      	ldr	r2, [r3, #0]
30008b08:	07d2      	lsls	r2, r2, #31
30008b0a:	d403      	bmi.n	30008b14 <OTP_ProgramMarginRead8.part.0+0x50>
30008b0c:	681a      	ldr	r2, [r3, #0]
30008b0e:	f042 0201 	orr.w	r2, r2, #1
30008b12:	601a      	str	r2, [r3, #0]
30008b14:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008b18:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008b1c:	694a      	ldr	r2, [r1, #20]
30008b1e:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
30008b22:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008b26:	614a      	str	r2, [r1, #20]
30008b28:	6033      	str	r3, [r6, #0]
30008b2a:	6833      	ldr	r3, [r6, #0]
30008b2c:	2b00      	cmp	r3, #0
30008b2e:	db10      	blt.n	30008b52 <OTP_ProgramMarginRead8.part.0+0x8e>
30008b30:	2400      	movs	r4, #0
30008b32:	4d1a      	ldr	r5, [pc, #104]	; (30008b9c <OTP_ProgramMarginRead8.part.0+0xd8>)
30008b34:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008b38:	e001      	b.n	30008b3e <OTP_ProgramMarginRead8.part.0+0x7a>
30008b3a:	4554      	cmp	r4, sl
30008b3c:	d01a      	beq.n	30008b74 <OTP_ProgramMarginRead8.part.0+0xb0>
30008b3e:	2005      	movs	r0, #5
30008b40:	3401      	adds	r4, #1
30008b42:	47a8      	blx	r5
30008b44:	6833      	ldr	r3, [r6, #0]
30008b46:	2b00      	cmp	r3, #0
30008b48:	daf7      	bge.n	30008b3a <OTP_ProgramMarginRead8.part.0+0x76>
30008b4a:	f644 6320 	movw	r3, #20000	; 0x4e20
30008b4e:	429c      	cmp	r4, r3
30008b50:	d010      	beq.n	30008b74 <OTP_ProgramMarginRead8.part.0+0xb0>
30008b52:	6833      	ldr	r3, [r6, #0]
30008b54:	2001      	movs	r0, #1
30008b56:	f889 3000 	strb.w	r3, [r9]
30008b5a:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008b5e:	6953      	ldr	r3, [r2, #20]
30008b60:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008b64:	6153      	str	r3, [r2, #20]
30008b66:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008b68:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008b6c:	643b      	str	r3, [r7, #64]	; 0x40
30008b6e:	b002      	add	sp, #8
30008b70:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008b74:	22ff      	movs	r2, #255	; 0xff
30008b76:	4b0b      	ldr	r3, [pc, #44]	; (30008ba4 <OTP_ProgramMarginRead8.part.0+0xe0>)
30008b78:	2002      	movs	r0, #2
30008b7a:	f8cd 8000 	str.w	r8, [sp]
30008b7e:	4619      	mov	r1, r3
30008b80:	f889 2000 	strb.w	r2, [r9]
30008b84:	4a08      	ldr	r2, [pc, #32]	; (30008ba8 <OTP_ProgramMarginRead8.part.0+0xe4>)
30008b86:	f003 fae3 	bl	3000c150 <rtk_log_write>
30008b8a:	2000      	movs	r0, #0
30008b8c:	e7e5      	b.n	30008b5a <OTP_ProgramMarginRead8.part.0+0x96>
30008b8e:	bf00      	nop
30008b90:	0000e5f9 	.word	0x0000e5f9
30008b94:	5200000c 	.word	0x5200000c
30008b98:	42000008 	.word	0x42000008
30008b9c:	00009b2d 	.word	0x00009b2d
30008ba0:	42008000 	.word	0x42008000
30008ba4:	3000d8c8 	.word	0x3000d8c8
30008ba8:	3000d8cc 	.word	0x3000d8cc

30008bac <OTP_Read8>:
30008bac:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008bb0:	d201      	bcs.n	30008bb6 <OTP_Read8+0xa>
30008bb2:	f7ff bf15 	b.w	300089e0 <OTP_Read8.part.0>
30008bb6:	22ff      	movs	r2, #255	; 0xff
30008bb8:	2000      	movs	r0, #0
30008bba:	700a      	strb	r2, [r1, #0]
30008bbc:	4770      	bx	lr
30008bbe:	bf00      	nop

30008bc0 <OTP_Write8>:
30008bc0:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008bc4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008bc8:	b087      	sub	sp, #28
30008bca:	d278      	bcs.n	30008cbe <OTP_Write8+0xfe>
30008bcc:	4689      	mov	r9, r1
30008bce:	f10d 0117 	add.w	r1, sp, #23
30008bd2:	4680      	mov	r8, r0
30008bd4:	f7ff ff76 	bl	30008ac4 <OTP_ProgramMarginRead8.part.0>
30008bd8:	2800      	cmp	r0, #0
30008bda:	d073      	beq.n	30008cc4 <OTP_Write8+0x104>
30008bdc:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008be0:	2202      	movs	r2, #2
30008be2:	46ca      	mov	sl, r9
30008be4:	9303      	str	r3, [sp, #12]
30008be6:	f89d 3017 	ldrb.w	r3, [sp, #23]
30008bea:	9202      	str	r2, [sp, #8]
30008bec:	ea6a 0a03 	orn	sl, sl, r3
30008bf0:	fa5f fa8a 	uxtb.w	sl, sl
30008bf4:	f1ba 0fff 	cmp.w	sl, #255	; 0xff
30008bf8:	d04f      	beq.n	30008c9a <OTP_Write8+0xda>
30008bfa:	4b4b      	ldr	r3, [pc, #300]	; (30008d28 <OTP_Write8+0x168>)
30008bfc:	e843 f300 	tt	r3, r3
30008c00:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008c04:	4a49      	ldr	r2, [pc, #292]	; (30008d2c <OTP_Write8+0x16c>)
30008c06:	2b00      	cmp	r3, #0
30008c08:	4b49      	ldr	r3, [pc, #292]	; (30008d30 <OTP_Write8+0x170>)
30008c0a:	bf0b      	itete	eq
30008c0c:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008c10:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008c14:	461d      	moveq	r5, r3
30008c16:	4615      	movne	r5, r2
30008c18:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c1a:	05d8      	lsls	r0, r3, #23
30008c1c:	d505      	bpl.n	30008c2a <OTP_Write8+0x6a>
30008c1e:	4c45      	ldr	r4, [pc, #276]	; (30008d34 <OTP_Write8+0x174>)
30008c20:	2064      	movs	r0, #100	; 0x64
30008c22:	47a0      	blx	r4
30008c24:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c26:	05d9      	lsls	r1, r3, #23
30008c28:	d4fa      	bmi.n	30008c20 <OTP_Write8+0x60>
30008c2a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c2c:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008c30:	643b      	str	r3, [r7, #64]	; 0x40
30008c32:	4b41      	ldr	r3, [pc, #260]	; (30008d38 <OTP_Write8+0x178>)
30008c34:	681a      	ldr	r2, [r3, #0]
30008c36:	07d2      	lsls	r2, r2, #31
30008c38:	d403      	bmi.n	30008c42 <OTP_Write8+0x82>
30008c3a:	681a      	ldr	r2, [r3, #0]
30008c3c:	f042 0201 	orr.w	r2, r2, #1
30008c40:	601a      	str	r2, [r3, #0]
30008c42:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008c46:	9b03      	ldr	r3, [sp, #12]
30008c48:	694a      	ldr	r2, [r1, #20]
30008c4a:	ea4a 0303 	orr.w	r3, sl, r3
30008c4e:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
30008c52:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
30008c56:	614a      	str	r2, [r1, #20]
30008c58:	602b      	str	r3, [r5, #0]
30008c5a:	682b      	ldr	r3, [r5, #0]
30008c5c:	2b00      	cmp	r3, #0
30008c5e:	da12      	bge.n	30008c86 <OTP_Write8+0xc6>
30008c60:	f04f 0b00 	mov.w	fp, #0
30008c64:	4c33      	ldr	r4, [pc, #204]	; (30008d34 <OTP_Write8+0x174>)
30008c66:	f644 6620 	movw	r6, #20000	; 0x4e20
30008c6a:	e001      	b.n	30008c70 <OTP_Write8+0xb0>
30008c6c:	45b3      	cmp	fp, r6
30008c6e:	d036      	beq.n	30008cde <OTP_Write8+0x11e>
30008c70:	2005      	movs	r0, #5
30008c72:	f10b 0b01 	add.w	fp, fp, #1
30008c76:	47a0      	blx	r4
30008c78:	682b      	ldr	r3, [r5, #0]
30008c7a:	2b00      	cmp	r3, #0
30008c7c:	dbf6      	blt.n	30008c6c <OTP_Write8+0xac>
30008c7e:	f644 6220 	movw	r2, #20000	; 0x4e20
30008c82:	4593      	cmp	fp, r2
30008c84:	d02b      	beq.n	30008cde <OTP_Write8+0x11e>
30008c86:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008c8a:	6953      	ldr	r3, [r2, #20]
30008c8c:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008c90:	6153      	str	r3, [r2, #20]
30008c92:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c94:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008c98:	643b      	str	r3, [r7, #64]	; 0x40
30008c9a:	f10d 0117 	add.w	r1, sp, #23
30008c9e:	4640      	mov	r0, r8
30008ca0:	f7ff ff10 	bl	30008ac4 <OTP_ProgramMarginRead8.part.0>
30008ca4:	2800      	cmp	r0, #0
30008ca6:	d034      	beq.n	30008d12 <OTP_Write8+0x152>
30008ca8:	f89d 3017 	ldrb.w	r3, [sp, #23]
30008cac:	4599      	cmp	r9, r3
30008cae:	d039      	beq.n	30008d24 <OTP_Write8+0x164>
30008cb0:	9a02      	ldr	r2, [sp, #8]
30008cb2:	2a01      	cmp	r2, #1
30008cb4:	d110      	bne.n	30008cd8 <OTP_Write8+0x118>
30008cb6:	2000      	movs	r0, #0
30008cb8:	b007      	add	sp, #28
30008cba:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008cbe:	23ff      	movs	r3, #255	; 0xff
30008cc0:	f88d 3017 	strb.w	r3, [sp, #23]
30008cc4:	4b1d      	ldr	r3, [pc, #116]	; (30008d3c <OTP_Write8+0x17c>)
30008cc6:	2002      	movs	r0, #2
30008cc8:	4a1d      	ldr	r2, [pc, #116]	; (30008d40 <OTP_Write8+0x180>)
30008cca:	4619      	mov	r1, r3
30008ccc:	f003 fa40 	bl	3000c150 <rtk_log_write>
30008cd0:	2000      	movs	r0, #0
30008cd2:	b007      	add	sp, #28
30008cd4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008cd8:	2201      	movs	r2, #1
30008cda:	9202      	str	r2, [sp, #8]
30008cdc:	e786      	b.n	30008bec <OTP_Write8+0x2c>
30008cde:	4b17      	ldr	r3, [pc, #92]	; (30008d3c <OTP_Write8+0x17c>)
30008ce0:	2003      	movs	r0, #3
30008ce2:	4a18      	ldr	r2, [pc, #96]	; (30008d44 <OTP_Write8+0x184>)
30008ce4:	4619      	mov	r1, r3
30008ce6:	f8cd 8000 	str.w	r8, [sp]
30008cea:	f003 fa31 	bl	3000c150 <rtk_log_write>
30008cee:	f04f 4084 	mov.w	r0, #1107296256	; 0x42000000
30008cf2:	4b12      	ldr	r3, [pc, #72]	; (30008d3c <OTP_Write8+0x17c>)
30008cf4:	6941      	ldr	r1, [r0, #20]
30008cf6:	4a14      	ldr	r2, [pc, #80]	; (30008d48 <OTP_Write8+0x188>)
30008cf8:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
30008cfc:	6141      	str	r1, [r0, #20]
30008cfe:	4619      	mov	r1, r3
30008d00:	6c38      	ldr	r0, [r7, #64]	; 0x40
30008d02:	f420 7080 	bic.w	r0, r0, #256	; 0x100
30008d06:	6438      	str	r0, [r7, #64]	; 0x40
30008d08:	2002      	movs	r0, #2
30008d0a:	f003 fa21 	bl	3000c150 <rtk_log_write>
30008d0e:	2000      	movs	r0, #0
30008d10:	e7df      	b.n	30008cd2 <OTP_Write8+0x112>
30008d12:	4b0a      	ldr	r3, [pc, #40]	; (30008d3c <OTP_Write8+0x17c>)
30008d14:	4604      	mov	r4, r0
30008d16:	4a0d      	ldr	r2, [pc, #52]	; (30008d4c <OTP_Write8+0x18c>)
30008d18:	2002      	movs	r0, #2
30008d1a:	4619      	mov	r1, r3
30008d1c:	f003 fa18 	bl	3000c150 <rtk_log_write>
30008d20:	4620      	mov	r0, r4
30008d22:	e7d6      	b.n	30008cd2 <OTP_Write8+0x112>
30008d24:	2001      	movs	r0, #1
30008d26:	e7d4      	b.n	30008cd2 <OTP_Write8+0x112>
30008d28:	0000e5f9 	.word	0x0000e5f9
30008d2c:	5200000c 	.word	0x5200000c
30008d30:	42000008 	.word	0x42000008
30008d34:	00009b2d 	.word	0x00009b2d
30008d38:	42008000 	.word	0x42008000
30008d3c:	3000d8c8 	.word	0x3000d8c8
30008d40:	3000d8e8 	.word	0x3000d8e8
30008d44:	3000d900 	.word	0x3000d900
30008d48:	3000d91c 	.word	0x3000d91c
30008d4c:	3000d938 	.word	0x3000d938

30008d50 <PLL_NP_ClkSet>:
30008d50:	b570      	push	{r4, r5, r6, lr}
30008d52:	4604      	mov	r4, r0
30008d54:	4b1b      	ldr	r3, [pc, #108]	; (30008dc4 <PLL_NP_ClkSet+0x74>)
30008d56:	4798      	blx	r3
30008d58:	4d1b      	ldr	r5, [pc, #108]	; (30008dc8 <PLL_NP_ClkSet+0x78>)
30008d5a:	08c2      	lsrs	r2, r0, #3
30008d5c:	fbb4 f3f0 	udiv	r3, r4, r0
30008d60:	fb00 4c13 	mls	ip, r0, r3, r4
30008d64:	1e9c      	subs	r4, r3, #2
30008d66:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008d68:	0164      	lsls	r4, r4, #5
30008d6a:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30008d6e:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30008d72:	431c      	orrs	r4, r3
30008d74:	fbbc f3f2 	udiv	r3, ip, r2
30008d78:	646c      	str	r4, [r5, #68]	; 0x44
30008d7a:	041c      	lsls	r4, r3, #16
30008d7c:	fb02 c013 	mls	r0, r2, r3, ip
30008d80:	2300      	movs	r3, #0
30008d82:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30008d84:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30008d88:	4619      	mov	r1, r3
30008d8a:	fa1f fc86 	uxth.w	ip, r6
30008d8e:	4e0f      	ldr	r6, [pc, #60]	; (30008dcc <PLL_NP_ClkSet+0x7c>)
30008d90:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30008d94:	ea44 040c 	orr.w	r4, r4, ip
30008d98:	f003 fa64 	bl	3000c264 <__aeabi_uldivmod>
30008d9c:	4603      	mov	r3, r0
30008d9e:	2001      	movs	r0, #1
30008da0:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30008da4:	64ec      	str	r4, [r5, #76]	; 0x4c
30008da6:	47b0      	blx	r6
30008da8:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008daa:	203c      	movs	r0, #60	; 0x3c
30008dac:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30008db0:	646b      	str	r3, [r5, #68]	; 0x44
30008db2:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008db4:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30008db8:	646b      	str	r3, [r5, #68]	; 0x44
30008dba:	4633      	mov	r3, r6
30008dbc:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008dc0:	4718      	bx	r3
30008dc2:	bf00      	nop
30008dc4:	000099f5 	.word	0x000099f5
30008dc8:	42008800 	.word	0x42008800
30008dcc:	00009b2d 	.word	0x00009b2d

30008dd0 <PLL_AP_ClkSet>:
30008dd0:	b510      	push	{r4, lr}
30008dd2:	4604      	mov	r4, r0
30008dd4:	4b0b      	ldr	r3, [pc, #44]	; (30008e04 <PLL_AP_ClkSet+0x34>)
30008dd6:	4798      	blx	r3
30008dd8:	fbb4 f0f0 	udiv	r0, r4, r0
30008ddc:	f1a0 0319 	sub.w	r3, r0, #25
30008de0:	1e84      	subs	r4, r0, #2
30008de2:	2b0f      	cmp	r3, #15
30008de4:	d807      	bhi.n	30008df6 <PLL_AP_ClkSet+0x26>
30008de6:	4b08      	ldr	r3, [pc, #32]	; (30008e08 <PLL_AP_ClkSet+0x38>)
30008de8:	b2e0      	uxtb	r0, r4
30008dea:	695c      	ldr	r4, [r3, #20]
30008dec:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30008df0:	4320      	orrs	r0, r4
30008df2:	6158      	str	r0, [r3, #20]
30008df4:	bd10      	pop	{r4, pc}
30008df6:	f240 1181 	movw	r1, #385	; 0x181
30008dfa:	4804      	ldr	r0, [pc, #16]	; (30008e0c <PLL_AP_ClkSet+0x3c>)
30008dfc:	f003 fbf0 	bl	3000c5e0 <__io_assert_failed_veneer>
30008e00:	e7f1      	b.n	30008de6 <PLL_AP_ClkSet+0x16>
30008e02:	bf00      	nop
30008e04:	000099f5 	.word	0x000099f5
30008e08:	41000300 	.word	0x41000300
30008e0c:	3000d954 	.word	0x3000d954

30008e10 <PLL_AP>:
30008e10:	2801      	cmp	r0, #1
30008e12:	b538      	push	{r3, r4, r5, lr}
30008e14:	d009      	beq.n	30008e2a <PLL_AP+0x1a>
30008e16:	4b13      	ldr	r3, [pc, #76]	; (30008e64 <PLL_AP+0x54>)
30008e18:	681a      	ldr	r2, [r3, #0]
30008e1a:	f022 0209 	bic.w	r2, r2, #9
30008e1e:	601a      	str	r2, [r3, #0]
30008e20:	681a      	ldr	r2, [r3, #0]
30008e22:	f022 0206 	bic.w	r2, r2, #6
30008e26:	601a      	str	r2, [r3, #0]
30008e28:	bd38      	pop	{r3, r4, r5, pc}
30008e2a:	4b0f      	ldr	r3, [pc, #60]	; (30008e68 <PLL_AP+0x58>)
30008e2c:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008e2e:	f012 0f05 	tst.w	r2, #5
30008e32:	d00f      	beq.n	30008e54 <PLL_AP+0x44>
30008e34:	4d0d      	ldr	r5, [pc, #52]	; (30008e6c <PLL_AP+0x5c>)
30008e36:	4c0b      	ldr	r4, [pc, #44]	; (30008e64 <PLL_AP+0x54>)
30008e38:	2001      	movs	r0, #1
30008e3a:	6823      	ldr	r3, [r4, #0]
30008e3c:	f043 0306 	orr.w	r3, r3, #6
30008e40:	6023      	str	r3, [r4, #0]
30008e42:	47a8      	blx	r5
30008e44:	6823      	ldr	r3, [r4, #0]
30008e46:	f043 0309 	orr.w	r3, r3, #9
30008e4a:	6023      	str	r3, [r4, #0]
30008e4c:	6863      	ldr	r3, [r4, #4]
30008e4e:	2b00      	cmp	r3, #0
30008e50:	dafc      	bge.n	30008e4c <PLL_AP+0x3c>
30008e52:	bd38      	pop	{r3, r4, r5, pc}
30008e54:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008e56:	20a0      	movs	r0, #160	; 0xa0
30008e58:	4d04      	ldr	r5, [pc, #16]	; (30008e6c <PLL_AP+0x5c>)
30008e5a:	f042 0207 	orr.w	r2, r2, #7
30008e5e:	679a      	str	r2, [r3, #120]	; 0x78
30008e60:	47a8      	blx	r5
30008e62:	e7e8      	b.n	30008e36 <PLL_AP+0x26>
30008e64:	41000300 	.word	0x41000300
30008e68:	42008800 	.word	0x42008800
30008e6c:	00009b2d 	.word	0x00009b2d

30008e70 <PSRAM_INFO_Update>:
30008e70:	4b46      	ldr	r3, [pc, #280]	; (30008f8c <PSRAM_INFO_Update+0x11c>)
30008e72:	4947      	ldr	r1, [pc, #284]	; (30008f90 <PSRAM_INFO_Update+0x120>)
30008e74:	4a47      	ldr	r2, [pc, #284]	; (30008f94 <PSRAM_INFO_Update+0x124>)
30008e76:	b530      	push	{r4, r5, lr}
30008e78:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
30008e7c:	b083      	sub	sp, #12
30008e7e:	680d      	ldr	r5, [r1, #0]
30008e80:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008e84:	4c44      	ldr	r4, [pc, #272]	; (30008f98 <PSRAM_INFO_Update+0x128>)
30008e86:	fb02 f505 	mul.w	r5, r2, r5
30008e8a:	3301      	adds	r3, #1
30008e8c:	fbb5 f5f3 	udiv	r5, r5, r3
30008e90:	086a      	lsrs	r2, r5, #1
30008e92:	4b42      	ldr	r3, [pc, #264]	; (30008f9c <PSRAM_INFO_Update+0x12c>)
30008e94:	60e2      	str	r2, [r4, #12]
30008e96:	4798      	blx	r3
30008e98:	4b41      	ldr	r3, [pc, #260]	; (30008fa0 <PSRAM_INFO_Update+0x130>)
30008e9a:	6020      	str	r0, [r4, #0]
30008e9c:	4798      	blx	r3
30008e9e:	4b41      	ldr	r3, [pc, #260]	; (30008fa4 <PSRAM_INFO_Update+0x134>)
30008ea0:	220a      	movs	r2, #10
30008ea2:	60a0      	str	r0, [r4, #8]
30008ea4:	6122      	str	r2, [r4, #16]
30008ea6:	2220      	movs	r2, #32
30008ea8:	fbb3 f3f5 	udiv	r3, r3, r5
30008eac:	6163      	str	r3, [r4, #20]
30008eae:	fbb2 f3f3 	udiv	r3, r2, r3
30008eb2:	6822      	ldr	r2, [r4, #0]
30008eb4:	6223      	str	r3, [r4, #32]
30008eb6:	2a01      	cmp	r2, #1
30008eb8:	68e3      	ldr	r3, [r4, #12]
30008eba:	4a3b      	ldr	r2, [pc, #236]	; (30008fa8 <PSRAM_INFO_Update+0x138>)
30008ebc:	d01d      	beq.n	30008efa <PSRAM_INFO_Update+0x8a>
30008ebe:	4293      	cmp	r3, r2
30008ec0:	d810      	bhi.n	30008ee4 <PSRAM_INFO_Update+0x74>
30008ec2:	2304      	movs	r3, #4
30008ec4:	2108      	movs	r1, #8
30008ec6:	461a      	mov	r2, r3
30008ec8:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008ecc:	4b37      	ldr	r3, [pc, #220]	; (30008fac <PSRAM_INFO_Update+0x13c>)
30008ece:	f883 2110 	strb.w	r2, [r3, #272]	; 0x110
30008ed2:	4b37      	ldr	r3, [pc, #220]	; (30008fb0 <PSRAM_INFO_Update+0x140>)
30008ed4:	2004      	movs	r0, #4
30008ed6:	4a37      	ldr	r2, [pc, #220]	; (30008fb4 <PSRAM_INFO_Update+0x144>)
30008ed8:	4619      	mov	r1, r3
30008eda:	9500      	str	r5, [sp, #0]
30008edc:	f003 f938 	bl	3000c150 <rtk_log_write>
30008ee0:	b003      	add	sp, #12
30008ee2:	bd30      	pop	{r4, r5, pc}
30008ee4:	4a34      	ldr	r2, [pc, #208]	; (30008fb8 <PSRAM_INFO_Update+0x148>)
30008ee6:	4293      	cmp	r3, r2
30008ee8:	d916      	bls.n	30008f18 <PSRAM_INFO_Update+0xa8>
30008eea:	4a34      	ldr	r2, [pc, #208]	; (30008fbc <PSRAM_INFO_Update+0x14c>)
30008eec:	4293      	cmp	r3, r2
30008eee:	d824      	bhi.n	30008f3a <PSRAM_INFO_Update+0xca>
30008ef0:	2306      	movs	r3, #6
30008ef2:	461a      	mov	r2, r3
30008ef4:	e9c4 3306 	strd	r3, r3, [r4, #24]
30008ef8:	e7e8      	b.n	30008ecc <PSRAM_INFO_Update+0x5c>
30008efa:	4293      	cmp	r3, r2
30008efc:	d907      	bls.n	30008f0e <PSRAM_INFO_Update+0x9e>
30008efe:	4a2e      	ldr	r2, [pc, #184]	; (30008fb8 <PSRAM_INFO_Update+0x148>)
30008f00:	4293      	cmp	r3, r2
30008f02:	d80f      	bhi.n	30008f24 <PSRAM_INFO_Update+0xb4>
30008f04:	2205      	movs	r2, #5
30008f06:	230f      	movs	r3, #15
30008f08:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f0c:	e7e1      	b.n	30008ed2 <PSRAM_INFO_Update+0x62>
30008f0e:	2204      	movs	r2, #4
30008f10:	230f      	movs	r3, #15
30008f12:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f16:	e7dc      	b.n	30008ed2 <PSRAM_INFO_Update+0x62>
30008f18:	2305      	movs	r3, #5
30008f1a:	2108      	movs	r1, #8
30008f1c:	461a      	mov	r2, r3
30008f1e:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008f22:	e7d3      	b.n	30008ecc <PSRAM_INFO_Update+0x5c>
30008f24:	4a25      	ldr	r2, [pc, #148]	; (30008fbc <PSRAM_INFO_Update+0x14c>)
30008f26:	4293      	cmp	r3, r2
30008f28:	d917      	bls.n	30008f5a <PSRAM_INFO_Update+0xea>
30008f2a:	4a25      	ldr	r2, [pc, #148]	; (30008fc0 <PSRAM_INFO_Update+0x150>)
30008f2c:	4293      	cmp	r3, r2
30008f2e:	d81f      	bhi.n	30008f70 <PSRAM_INFO_Update+0x100>
30008f30:	2207      	movs	r2, #7
30008f32:	2314      	movs	r3, #20
30008f34:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f38:	e7cb      	b.n	30008ed2 <PSRAM_INFO_Update+0x62>
30008f3a:	4a21      	ldr	r2, [pc, #132]	; (30008fc0 <PSRAM_INFO_Update+0x150>)
30008f3c:	4293      	cmp	r3, r2
30008f3e:	d911      	bls.n	30008f64 <PSRAM_INFO_Update+0xf4>
30008f40:	4a20      	ldr	r2, [pc, #128]	; (30008fc4 <PSRAM_INFO_Update+0x154>)
30008f42:	4293      	cmp	r3, r2
30008f44:	bf8c      	ite	hi
30008f46:	2300      	movhi	r3, #0
30008f48:	2301      	movls	r3, #1
30008f4a:	2804      	cmp	r0, #4
30008f4c:	d100      	bne.n	30008f50 <PSRAM_INFO_Update+0xe0>
30008f4e:	b94b      	cbnz	r3, 30008f64 <PSRAM_INFO_Update+0xf4>
30008f50:	2803      	cmp	r0, #3
30008f52:	d100      	bne.n	30008f56 <PSRAM_INFO_Update+0xe6>
30008f54:	b9a3      	cbnz	r3, 30008f80 <PSRAM_INFO_Update+0x110>
30008f56:	7e22      	ldrb	r2, [r4, #24]
30008f58:	e7b8      	b.n	30008ecc <PSRAM_INFO_Update+0x5c>
30008f5a:	2206      	movs	r2, #6
30008f5c:	2312      	movs	r3, #18
30008f5e:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f62:	e7b6      	b.n	30008ed2 <PSRAM_INFO_Update+0x62>
30008f64:	2307      	movs	r3, #7
30008f66:	2106      	movs	r1, #6
30008f68:	461a      	mov	r2, r3
30008f6a:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008f6e:	e7ad      	b.n	30008ecc <PSRAM_INFO_Update+0x5c>
30008f70:	4a14      	ldr	r2, [pc, #80]	; (30008fc4 <PSRAM_INFO_Update+0x154>)
30008f72:	4293      	cmp	r3, r2
30008f74:	d8ad      	bhi.n	30008ed2 <PSRAM_INFO_Update+0x62>
30008f76:	2208      	movs	r2, #8
30008f78:	231c      	movs	r3, #28
30008f7a:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f7e:	e7a8      	b.n	30008ed2 <PSRAM_INFO_Update+0x62>
30008f80:	230a      	movs	r3, #10
30008f82:	2106      	movs	r1, #6
30008f84:	461a      	mov	r2, r3
30008f86:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008f8a:	e79f      	b.n	30008ecc <PSRAM_INFO_Update+0x5c>
30008f8c:	42008000 	.word	0x42008000
30008f90:	3000f148 	.word	0x3000f148
30008f94:	000f4240 	.word	0x000f4240
30008f98:	3000fe80 	.word	0x3000fe80
30008f9c:	30007ec5 	.word	0x30007ec5
30008fa0:	30007e8d 	.word	0x30007e8d
30008fa4:	3b9aca00 	.word	0x3b9aca00
30008fa8:	0632ea00 	.word	0x0632ea00
30008fac:	23020000 	.word	0x23020000
30008fb0:	3000d964 	.word	0x3000d964
30008fb4:	3000d96c 	.word	0x3000d96c
30008fb8:	07ed6b40 	.word	0x07ed6b40
30008fbc:	09e4f580 	.word	0x09e4f580
30008fc0:	0bebc200 	.word	0x0bebc200
30008fc4:	0ee6b280 	.word	0x0ee6b280

30008fc8 <PSRAM_PHY_StructInit>:
30008fc8:	2203      	movs	r2, #3
30008fca:	2301      	movs	r3, #1
30008fcc:	210a      	movs	r1, #10
30008fce:	b410      	push	{r4}
30008fd0:	e9c0 2202 	strd	r2, r2, [r0, #8]
30008fd4:	2402      	movs	r4, #2
30008fd6:	2207      	movs	r2, #7
30008fd8:	e9c0 3400 	strd	r3, r4, [r0]
30008fdc:	e9c0 1304 	strd	r1, r3, [r0, #16]
30008fe0:	f85d 4b04 	ldr.w	r4, [sp], #4
30008fe4:	6182      	str	r2, [r0, #24]
30008fe6:	4770      	bx	lr

30008fe8 <PSRAM_PHY_Init>:
30008fe8:	b538      	push	{r3, r4, r5, lr}
30008fea:	4604      	mov	r4, r0
30008fec:	6903      	ldr	r3, [r0, #16]
30008fee:	4d14      	ldr	r5, [pc, #80]	; (30009040 <PSRAM_PHY_Init+0x58>)
30008ff0:	f003 031f 	and.w	r3, r3, #31
30008ff4:	4913      	ldr	r1, [pc, #76]	; (30009044 <PSRAM_PHY_Init+0x5c>)
30008ff6:	e9d0 0200 	ldrd	r0, r2, [r0]
30008ffa:	0612      	lsls	r2, r2, #24
30008ffc:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30009000:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30009004:	4313      	orrs	r3, r2
30009006:	68a2      	ldr	r2, [r4, #8]
30009008:	0412      	lsls	r2, r2, #16
3000900a:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000900e:	4313      	orrs	r3, r2
30009010:	68e2      	ldr	r2, [r4, #12]
30009012:	0212      	lsls	r2, r2, #8
30009014:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009018:	4313      	orrs	r3, r2
3000901a:	606b      	str	r3, [r5, #4]
3000901c:	4788      	blx	r1
3000901e:	2801      	cmp	r0, #1
30009020:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30009024:	bf08      	it	eq
30009026:	4a08      	ldreq	r2, [pc, #32]	; (30009048 <PSRAM_PHY_Init+0x60>)
30009028:	ea4f 1303 	mov.w	r3, r3, lsl #4
3000902c:	f001 010f 	and.w	r1, r1, #15
30009030:	bf18      	it	ne
30009032:	4a06      	ldrne	r2, [pc, #24]	; (3000904c <PSRAM_PHY_Init+0x64>)
30009034:	b2db      	uxtb	r3, r3
30009036:	430b      	orrs	r3, r1
30009038:	431a      	orrs	r2, r3
3000903a:	61aa      	str	r2, [r5, #24]
3000903c:	bd38      	pop	{r3, r4, r5, pc}
3000903e:	bf00      	nop
30009040:	41002000 	.word	0x41002000
30009044:	30007ec5 	.word	0x30007ec5
30009048:	80000e00 	.word	0x80000e00
3000904c:	80000f00 	.word	0x80000f00

30009050 <PSRAM_CTRL_Init>:
30009050:	b470      	push	{r4, r5, r6}
30009052:	4b4c      	ldr	r3, [pc, #304]	; (30009184 <PSRAM_CTRL_Init+0x134>)
30009054:	2400      	movs	r4, #0
30009056:	4a4c      	ldr	r2, [pc, #304]	; (30009188 <PSRAM_CTRL_Init+0x138>)
30009058:	f24d 0101 	movw	r1, #53249	; 0xd001
3000905c:	609c      	str	r4, [r3, #8]
3000905e:	2001      	movs	r0, #1
30009060:	601a      	str	r2, [r3, #0]
30009062:	4a4a      	ldr	r2, [pc, #296]	; (3000918c <PSRAM_CTRL_Init+0x13c>)
30009064:	6158      	str	r0, [r3, #20]
30009066:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
3000906a:	6811      	ldr	r1, [r2, #0]
3000906c:	2902      	cmp	r1, #2
3000906e:	d04a      	beq.n	30009106 <PSRAM_CTRL_Init+0xb6>
30009070:	6954      	ldr	r4, [r2, #20]
30009072:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30009076:	69d0      	ldr	r0, [r2, #28]
30009078:	6915      	ldr	r5, [r2, #16]
3000907a:	fbb0 f0f4 	udiv	r0, r0, r4
3000907e:	fbb1 f1f4 	udiv	r1, r1, r4
30009082:	6994      	ldr	r4, [r2, #24]
30009084:	f005 020f 	and.w	r2, r5, #15
30009088:	f241 5551 	movw	r5, #5457	; 0x1551
3000908c:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30009090:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30009094:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30009098:	094a      	lsrs	r2, r1, #5
3000909a:	0181      	lsls	r1, r0, #6
3000909c:	f000 003f 	and.w	r0, r0, #63	; 0x3f
300090a0:	4d3b      	ldr	r5, [pc, #236]	; (30009190 <PSRAM_CTRL_Init+0x140>)
300090a2:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
300090a6:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
300090aa:	00a2      	lsls	r2, r4, #2
300090ac:	4301      	orrs	r1, r0
300090ae:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
300090b2:	3a01      	subs	r2, #1
300090b4:	430d      	orrs	r5, r1
300090b6:	f242 0120 	movw	r1, #8224	; 0x2020
300090ba:	f3c2 020b 	ubfx	r2, r2, #0, #12
300090be:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
300090c2:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
300090c6:	f44f 2180 	mov.w	r1, #262144	; 0x40000
300090ca:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
300090ce:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
300090d2:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300090d6:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
300090da:	4420      	add	r0, r4
300090dc:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
300090e0:	f640 71fe 	movw	r1, #4094	; 0xffe
300090e4:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
300090e8:	ea01 0140 	and.w	r1, r1, r0, lsl #1
300090ec:	432a      	orrs	r2, r5
300090ee:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300090f2:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300090f6:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
300090fa:	4a22      	ldr	r2, [pc, #136]	; (30009184 <PSRAM_CTRL_Init+0x134>)
300090fc:	6a93      	ldr	r3, [r2, #40]	; 0x28
300090fe:	061b      	lsls	r3, r3, #24
30009100:	d5fc      	bpl.n	300090fc <PSRAM_CTRL_Init+0xac>
30009102:	bc70      	pop	{r4, r5, r6}
30009104:	4770      	bx	lr
30009106:	6955      	ldr	r5, [r2, #20]
30009108:	207d      	movs	r0, #125	; 0x7d
3000910a:	69d1      	ldr	r1, [r2, #28]
3000910c:	26a0      	movs	r6, #160	; 0xa0
3000910e:	6914      	ldr	r4, [r2, #16]
30009110:	6992      	ldr	r2, [r2, #24]
30009112:	f004 040f 	and.w	r4, r4, #15
30009116:	0092      	lsls	r2, r2, #2
30009118:	fbb1 f1f5 	udiv	r1, r1, r5
3000911c:	fbb0 f5f5 	udiv	r5, r0, r5
30009120:	f245 5051 	movw	r0, #21841	; 0x5551
30009124:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30009128:	f001 003f 	and.w	r0, r1, #63	; 0x3f
3000912c:	0189      	lsls	r1, r1, #6
3000912e:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
30009132:	f641 4570 	movw	r5, #7280	; 0x1c70
30009136:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
3000913a:	4325      	orrs	r5, r4
3000913c:	2420      	movs	r4, #32
3000913e:	4308      	orrs	r0, r1
30009140:	1e51      	subs	r1, r2, #1
30009142:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30009146:	3a02      	subs	r2, #2
30009148:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
3000914c:	f3c1 010b 	ubfx	r1, r1, #0, #12
30009150:	f3c2 020b 	ubfx	r2, r2, #0, #12
30009154:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30009158:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
3000915c:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30009160:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30009164:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30009168:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
3000916c:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30009170:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30009174:	4301      	orrs	r1, r0
30009176:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
3000917a:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000917e:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30009182:	e7ba      	b.n	300090fa <PSRAM_CTRL_Init+0xaa>
30009184:	41012000 	.word	0x41012000
30009188:	00bfe000 	.word	0x00bfe000
3000918c:	3000fe80 	.word	0x3000fe80
30009190:	00107000 	.word	0x00107000

30009194 <PSRAM_REG_Write>:
30009194:	b430      	push	{r4, r5}
30009196:	4c20      	ldr	r4, [pc, #128]	; (30009218 <PSRAM_REG_Write+0x84>)
30009198:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000919a:	f013 0301 	ands.w	r3, r3, #1
3000919e:	d1fb      	bne.n	30009198 <PSRAM_REG_Write+0x4>
300091a0:	6825      	ldr	r5, [r4, #0]
300091a2:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
300091a6:	6025      	str	r5, [r4, #0]
300091a8:	60a3      	str	r3, [r4, #8]
300091aa:	6825      	ldr	r5, [r4, #0]
300091ac:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300091b0:	6025      	str	r5, [r4, #0]
300091b2:	6063      	str	r3, [r4, #4]
300091b4:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
300091b8:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
300091bc:	23c0      	movs	r3, #192	; 0xc0
300091be:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091c2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091c6:	0e03      	lsrs	r3, r0, #24
300091c8:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091cc:	f3c0 4307 	ubfx	r3, r0, #16, #8
300091d0:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091d4:	f3c0 2307 	ubfx	r3, r0, #8, #8
300091d8:	b2c0      	uxtb	r0, r0
300091da:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091de:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300091e2:	b141      	cbz	r1, 300091f6 <PSRAM_REG_Write+0x62>
300091e4:	3a01      	subs	r2, #1
300091e6:	480c      	ldr	r0, [pc, #48]	; (30009218 <PSRAM_REG_Write+0x84>)
300091e8:	4411      	add	r1, r2
300091ea:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300091ee:	428a      	cmp	r2, r1
300091f0:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300091f4:	d1f9      	bne.n	300091ea <PSRAM_REG_Write+0x56>
300091f6:	4a08      	ldr	r2, [pc, #32]	; (30009218 <PSRAM_REG_Write+0x84>)
300091f8:	2301      	movs	r3, #1
300091fa:	6093      	str	r3, [r2, #8]
300091fc:	6893      	ldr	r3, [r2, #8]
300091fe:	07d9      	lsls	r1, r3, #31
30009200:	d4fc      	bmi.n	300091fc <PSRAM_REG_Write+0x68>
30009202:	4a05      	ldr	r2, [pc, #20]	; (30009218 <PSRAM_REG_Write+0x84>)
30009204:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009206:	07db      	lsls	r3, r3, #31
30009208:	d4fc      	bmi.n	30009204 <PSRAM_REG_Write+0x70>
3000920a:	6813      	ldr	r3, [r2, #0]
3000920c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009210:	6013      	str	r3, [r2, #0]
30009212:	bc30      	pop	{r4, r5}
30009214:	4770      	bx	lr
30009216:	bf00      	nop
30009218:	41012000 	.word	0x41012000

3000921c <PSRAM_APM_DEVIC_Init>:
3000921c:	4b12      	ldr	r3, [pc, #72]	; (30009268 <PSRAM_APM_DEVIC_Init+0x4c>)
3000921e:	2102      	movs	r1, #2
30009220:	2000      	movs	r0, #0
30009222:	b530      	push	{r4, r5, lr}
30009224:	4c11      	ldr	r4, [pc, #68]	; (3000926c <PSRAM_APM_DEVIC_Init+0x50>)
30009226:	b083      	sub	sp, #12
30009228:	699d      	ldr	r5, [r3, #24]
3000922a:	466a      	mov	r2, sp
3000922c:	eb04 0c05 	add.w	ip, r4, r5
30009230:	1eeb      	subs	r3, r5, #3
30009232:	4c0f      	ldr	r4, [pc, #60]	; (30009270 <PSRAM_APM_DEVIC_Init+0x54>)
30009234:	408b      	lsls	r3, r1
30009236:	f81c cc03 	ldrb.w	ip, [ip, #-3]
3000923a:	f003 031c 	and.w	r3, r3, #28
3000923e:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30009242:	f043 0320 	orr.w	r3, r3, #32
30009246:	fa5f fc8c 	uxtb.w	ip, ip
3000924a:	f88d 3000 	strb.w	r3, [sp]
3000924e:	f88d c004 	strb.w	ip, [sp, #4]
30009252:	f88d 3001 	strb.w	r3, [sp, #1]
30009256:	f88d c005 	strb.w	ip, [sp, #5]
3000925a:	47a0      	blx	r4
3000925c:	aa01      	add	r2, sp, #4
3000925e:	2102      	movs	r1, #2
30009260:	2004      	movs	r0, #4
30009262:	47a0      	blx	r4
30009264:	b003      	add	sp, #12
30009266:	bd30      	pop	{r4, r5, pc}
30009268:	3000fe80 	.word	0x3000fe80
3000926c:	3000f310 	.word	0x3000f310
30009270:	30009195 	.word	0x30009195

30009274 <PSRAM_WB_REG_Write>:
30009274:	b430      	push	{r4, r5}
30009276:	4c1e      	ldr	r4, [pc, #120]	; (300092f0 <PSRAM_WB_REG_Write+0x7c>)
30009278:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000927a:	f013 0301 	ands.w	r3, r3, #1
3000927e:	d1fb      	bne.n	30009278 <PSRAM_WB_REG_Write+0x4>
30009280:	6825      	ldr	r5, [r4, #0]
30009282:	b2c0      	uxtb	r0, r0
30009284:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009288:	6025      	str	r5, [r4, #0]
3000928a:	60a3      	str	r3, [r4, #8]
3000928c:	6825      	ldr	r5, [r4, #0]
3000928e:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009292:	6025      	str	r5, [r4, #0]
30009294:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30009298:	6063      	str	r3, [r4, #4]
3000929a:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
3000929e:	2560      	movs	r5, #96	; 0x60
300092a0:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300092a4:	2501      	movs	r5, #1
300092a6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300092aa:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300092ae:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300092b2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300092b6:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300092ba:	b141      	cbz	r1, 300092ce <PSRAM_WB_REG_Write+0x5a>
300092bc:	3a01      	subs	r2, #1
300092be:	480c      	ldr	r0, [pc, #48]	; (300092f0 <PSRAM_WB_REG_Write+0x7c>)
300092c0:	4411      	add	r1, r2
300092c2:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300092c6:	428a      	cmp	r2, r1
300092c8:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300092cc:	d1f9      	bne.n	300092c2 <PSRAM_WB_REG_Write+0x4e>
300092ce:	4a08      	ldr	r2, [pc, #32]	; (300092f0 <PSRAM_WB_REG_Write+0x7c>)
300092d0:	2301      	movs	r3, #1
300092d2:	6093      	str	r3, [r2, #8]
300092d4:	6893      	ldr	r3, [r2, #8]
300092d6:	07d9      	lsls	r1, r3, #31
300092d8:	d4fc      	bmi.n	300092d4 <PSRAM_WB_REG_Write+0x60>
300092da:	4a05      	ldr	r2, [pc, #20]	; (300092f0 <PSRAM_WB_REG_Write+0x7c>)
300092dc:	6a93      	ldr	r3, [r2, #40]	; 0x28
300092de:	07db      	lsls	r3, r3, #31
300092e0:	d4fc      	bmi.n	300092dc <PSRAM_WB_REG_Write+0x68>
300092e2:	6813      	ldr	r3, [r2, #0]
300092e4:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300092e8:	6013      	str	r3, [r2, #0]
300092ea:	bc30      	pop	{r4, r5}
300092ec:	4770      	bx	lr
300092ee:	bf00      	nop
300092f0:	41012000 	.word	0x41012000

300092f4 <PSRAM_WB_DEVIC_Init>:
300092f4:	490b      	ldr	r1, [pc, #44]	; (30009324 <PSRAM_WB_DEVIC_Init+0x30>)
300092f6:	4b0c      	ldr	r3, [pc, #48]	; (30009328 <PSRAM_WB_DEVIC_Init+0x34>)
300092f8:	b510      	push	{r4, lr}
300092fa:	6988      	ldr	r0, [r1, #24]
300092fc:	b082      	sub	sp, #8
300092fe:	248f      	movs	r4, #143	; 0x8f
30009300:	2102      	movs	r1, #2
30009302:	4403      	add	r3, r0
30009304:	aa01      	add	r2, sp, #4
30009306:	f88d 4004 	strb.w	r4, [sp, #4]
3000930a:	2000      	movs	r0, #0
3000930c:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30009310:	4c06      	ldr	r4, [pc, #24]	; (3000932c <PSRAM_WB_DEVIC_Init+0x38>)
30009312:	011b      	lsls	r3, r3, #4
30009314:	f043 030c 	orr.w	r3, r3, #12
30009318:	f88d 3005 	strb.w	r3, [sp, #5]
3000931c:	47a0      	blx	r4
3000931e:	b002      	add	sp, #8
30009320:	bd10      	pop	{r4, pc}
30009322:	bf00      	nop
30009324:	3000fe80 	.word	0x3000fe80
30009328:	3000f330 	.word	0x3000f330
3000932c:	30009275 	.word	0x30009275

30009330 <PSRAM_calibration>:
30009330:	4a9c      	ldr	r2, [pc, #624]	; (300095a4 <PSRAM_calibration+0x274>)
30009332:	2300      	movs	r3, #0
30009334:	499c      	ldr	r1, [pc, #624]	; (300095a8 <PSRAM_calibration+0x278>)
30009336:	489d      	ldr	r0, [pc, #628]	; (300095ac <PSRAM_calibration+0x27c>)
30009338:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000933c:	6852      	ldr	r2, [r2, #4]
3000933e:	b093      	sub	sp, #76	; 0x4c
30009340:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
30009344:	400a      	ands	r2, r1
30009346:	9311      	str	r3, [sp, #68]	; 0x44
30009348:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
3000934c:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
30009350:	920a      	str	r2, [sp, #40]	; 0x28
30009352:	6942      	ldr	r2, [r0, #20]
30009354:	03d1      	lsls	r1, r2, #15
30009356:	d51e      	bpl.n	30009396 <PSRAM_calibration+0x66>
30009358:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000935c:	f3bf 8f4f 	dsb	sy
30009360:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009364:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009368:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000936c:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009370:	0164      	lsls	r4, r4, #5
30009372:	ea04 0106 	and.w	r1, r4, r6
30009376:	462b      	mov	r3, r5
30009378:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000937c:	3b01      	subs	r3, #1
3000937e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009382:	1c5a      	adds	r2, r3, #1
30009384:	d1f8      	bne.n	30009378 <PSRAM_calibration+0x48>
30009386:	3c20      	subs	r4, #32
30009388:	f114 0f20 	cmn.w	r4, #32
3000938c:	d1f1      	bne.n	30009372 <PSRAM_calibration+0x42>
3000938e:	f3bf 8f4f 	dsb	sy
30009392:	f3bf 8f6f 	isb	sy
30009396:	4885      	ldr	r0, [pc, #532]	; (300095ac <PSRAM_calibration+0x27c>)
30009398:	2300      	movs	r3, #0
3000939a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000939e:	f3bf 8f4f 	dsb	sy
300093a2:	6943      	ldr	r3, [r0, #20]
300093a4:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300093a8:	6143      	str	r3, [r0, #20]
300093aa:	f3bf 8f4f 	dsb	sy
300093ae:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300093b2:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300093b6:	f3c5 344e 	ubfx	r4, r5, #13, #15
300093ba:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300093be:	0164      	lsls	r4, r4, #5
300093c0:	ea04 0106 	and.w	r1, r4, r6
300093c4:	462b      	mov	r3, r5
300093c6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300093ca:	3b01      	subs	r3, #1
300093cc:	1c5f      	adds	r7, r3, #1
300093ce:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300093d2:	d1f8      	bne.n	300093c6 <PSRAM_calibration+0x96>
300093d4:	3c20      	subs	r4, #32
300093d6:	f114 0f20 	cmn.w	r4, #32
300093da:	d1f1      	bne.n	300093c0 <PSRAM_calibration+0x90>
300093dc:	f3bf 8f4f 	dsb	sy
300093e0:	f3bf 8f6f 	isb	sy
300093e4:	2204      	movs	r2, #4
300093e6:	461e      	mov	r6, r3
300093e8:	9306      	str	r3, [sp, #24]
300093ea:	2700      	movs	r7, #0
300093ec:	9209      	str	r2, [sp, #36]	; 0x24
300093ee:	46b3      	mov	fp, r6
300093f0:	4a6c      	ldr	r2, [pc, #432]	; (300095a4 <PSRAM_calibration+0x274>)
300093f2:	4c6f      	ldr	r4, [pc, #444]	; (300095b0 <PSRAM_calibration+0x280>)
300093f4:	6813      	ldr	r3, [r2, #0]
300093f6:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 300095cc <PSRAM_calibration+0x29c>
300093fa:	f023 0301 	bic.w	r3, r3, #1
300093fe:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 300095d0 <PSRAM_calibration+0x2a0>
30009402:	6013      	str	r3, [r2, #0]
30009404:	2301      	movs	r3, #1
30009406:	9305      	str	r3, [sp, #20]
30009408:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000940a:	9a05      	ldr	r2, [sp, #20]
3000940c:	9704      	str	r7, [sp, #16]
3000940e:	2700      	movs	r7, #0
30009410:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30009414:	463d      	mov	r5, r7
30009416:	e9cd b607 	strd	fp, r6, [sp, #28]
3000941a:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
3000941e:	465e      	mov	r6, fp
30009420:	e010      	b.n	30009444 <PSRAM_calibration+0x114>
30009422:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
30009426:	bf08      	it	eq
30009428:	462e      	moveq	r6, r5
3000942a:	2d1f      	cmp	r5, #31
3000942c:	eb06 0b07 	add.w	fp, r6, r7
30009430:	f107 0701 	add.w	r7, r7, #1
30009434:	d102      	bne.n	3000943c <PSRAM_calibration+0x10c>
30009436:	9b04      	ldr	r3, [sp, #16]
30009438:	429f      	cmp	r7, r3
3000943a:	dc46      	bgt.n	300094ca <PSRAM_calibration+0x19a>
3000943c:	3501      	adds	r5, #1
3000943e:	2d20      	cmp	r5, #32
30009440:	f000 80ac 	beq.w	3000959c <PSRAM_calibration+0x26c>
30009444:	ea48 0205 	orr.w	r2, r8, r5
30009448:	4b56      	ldr	r3, [pc, #344]	; (300095a4 <PSRAM_calibration+0x274>)
3000944a:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
3000944e:	4959      	ldr	r1, [pc, #356]	; (300095b4 <PSRAM_calibration+0x284>)
30009450:	605a      	str	r2, [r3, #4]
30009452:	6822      	ldr	r2, [r4, #0]
30009454:	4b58      	ldr	r3, [pc, #352]	; (300095b8 <PSRAM_calibration+0x288>)
30009456:	f8ce 2000 	str.w	r2, [lr]
3000945a:	6860      	ldr	r0, [r4, #4]
3000945c:	4a57      	ldr	r2, [pc, #348]	; (300095bc <PSRAM_calibration+0x28c>)
3000945e:	f8c9 0000 	str.w	r0, [r9]
30009462:	68a0      	ldr	r0, [r4, #8]
30009464:	f8ca 0000 	str.w	r0, [sl]
30009468:	68e0      	ldr	r0, [r4, #12]
3000946a:	6018      	str	r0, [r3, #0]
3000946c:	6920      	ldr	r0, [r4, #16]
3000946e:	6008      	str	r0, [r1, #0]
30009470:	6960      	ldr	r0, [r4, #20]
30009472:	6010      	str	r0, [r2, #0]
30009474:	f8de 0000 	ldr.w	r0, [lr]
30009478:	900c      	str	r0, [sp, #48]	; 0x30
3000947a:	f8d9 0000 	ldr.w	r0, [r9]
3000947e:	900d      	str	r0, [sp, #52]	; 0x34
30009480:	f8da 0000 	ldr.w	r0, [sl]
30009484:	900e      	str	r0, [sp, #56]	; 0x38
30009486:	6818      	ldr	r0, [r3, #0]
30009488:	4b4d      	ldr	r3, [pc, #308]	; (300095c0 <PSRAM_calibration+0x290>)
3000948a:	900f      	str	r0, [sp, #60]	; 0x3c
3000948c:	a80c      	add	r0, sp, #48	; 0x30
3000948e:	6809      	ldr	r1, [r1, #0]
30009490:	9110      	str	r1, [sp, #64]	; 0x40
30009492:	6812      	ldr	r2, [r2, #0]
30009494:	4946      	ldr	r1, [pc, #280]	; (300095b0 <PSRAM_calibration+0x280>)
30009496:	9211      	str	r2, [sp, #68]	; 0x44
30009498:	2218      	movs	r2, #24
3000949a:	4798      	blx	r3
3000949c:	2800      	cmp	r0, #0
3000949e:	d0c0      	beq.n	30009422 <PSRAM_calibration+0xf2>
300094a0:	1c70      	adds	r0, r6, #1
300094a2:	d0cb      	beq.n	3000943c <PSRAM_calibration+0x10c>
300094a4:	9b04      	ldr	r3, [sp, #16]
300094a6:	42bb      	cmp	r3, r7
300094a8:	da0a      	bge.n	300094c0 <PSRAM_calibration+0x190>
300094aa:	9b05      	ldr	r3, [sp, #20]
300094ac:	f8cd b01c 	str.w	fp, [sp, #28]
300094b0:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300094b4:	9704      	str	r7, [sp, #16]
300094b6:	2700      	movs	r7, #0
300094b8:	9608      	str	r6, [sp, #32]
300094ba:	465e      	mov	r6, fp
300094bc:	9306      	str	r3, [sp, #24]
300094be:	e7bd      	b.n	3000943c <PSRAM_calibration+0x10c>
300094c0:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300094c4:	2700      	movs	r7, #0
300094c6:	465e      	mov	r6, fp
300094c8:	e7b8      	b.n	3000943c <PSRAM_calibration+0x10c>
300094ca:	9b05      	ldr	r3, [sp, #20]
300094cc:	9306      	str	r3, [sp, #24]
300094ce:	9b05      	ldr	r3, [sp, #20]
300094d0:	005b      	lsls	r3, r3, #1
300094d2:	9305      	str	r3, [sp, #20]
300094d4:	9b09      	ldr	r3, [sp, #36]	; 0x24
300094d6:	3b01      	subs	r3, #1
300094d8:	9309      	str	r3, [sp, #36]	; 0x24
300094da:	d195      	bne.n	30009408 <PSRAM_calibration+0xd8>
300094dc:	9b06      	ldr	r3, [sp, #24]
300094de:	2004      	movs	r0, #4
300094e0:	4a38      	ldr	r2, [pc, #224]	; (300095c4 <PSRAM_calibration+0x294>)
300094e2:	e9cd 7302 	strd	r7, r3, [sp, #8]
300094e6:	4b38      	ldr	r3, [pc, #224]	; (300095c8 <PSRAM_calibration+0x298>)
300094e8:	e9cd 6b00 	strd	r6, fp, [sp]
300094ec:	4619      	mov	r1, r3
300094ee:	f002 fe2f 	bl	3000c150 <rtk_log_write>
300094f2:	482e      	ldr	r0, [pc, #184]	; (300095ac <PSRAM_calibration+0x27c>)
300094f4:	6943      	ldr	r3, [r0, #20]
300094f6:	03d9      	lsls	r1, r3, #15
300094f8:	d425      	bmi.n	30009546 <PSRAM_calibration+0x216>
300094fa:	9b09      	ldr	r3, [sp, #36]	; 0x24
300094fc:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009500:	f3bf 8f4f 	dsb	sy
30009504:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009508:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
3000950c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009510:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009514:	0164      	lsls	r4, r4, #5
30009516:	ea04 010c 	and.w	r1, r4, ip
3000951a:	462b      	mov	r3, r5
3000951c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009520:	3b01      	subs	r3, #1
30009522:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30009526:	1c5a      	adds	r2, r3, #1
30009528:	d1f8      	bne.n	3000951c <PSRAM_calibration+0x1ec>
3000952a:	3c20      	subs	r4, #32
3000952c:	f114 0f20 	cmn.w	r4, #32
30009530:	d1f1      	bne.n	30009516 <PSRAM_calibration+0x1e6>
30009532:	f3bf 8f4f 	dsb	sy
30009536:	6943      	ldr	r3, [r0, #20]
30009538:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
3000953c:	6143      	str	r3, [r0, #20]
3000953e:	f3bf 8f4f 	dsb	sy
30009542:	f3bf 8f6f 	isb	sy
30009546:	2f08      	cmp	r7, #8
30009548:	dd24      	ble.n	30009594 <PSRAM_calibration+0x264>
3000954a:	ebbb 0306 	subs.w	r3, fp, r6
3000954e:	9806      	ldr	r0, [sp, #24]
30009550:	bf48      	it	mi
30009552:	3301      	addmi	r3, #1
30009554:	eb16 060b 	adds.w	r6, r6, fp
30009558:	ea4f 0363 	mov.w	r3, r3, asr #1
3000955c:	bf48      	it	mi
3000955e:	3601      	addmi	r6, #1
30009560:	1e9a      	subs	r2, r3, #2
30009562:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009564:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009568:	0414      	lsls	r4, r2, #16
3000956a:	f3c6 0344 	ubfx	r3, r6, #1, #5
3000956e:	0212      	lsls	r2, r2, #8
30009570:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009574:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009578:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000957c:	2001      	movs	r0, #1
3000957e:	430b      	orrs	r3, r1
30009580:	4908      	ldr	r1, [pc, #32]	; (300095a4 <PSRAM_calibration+0x274>)
30009582:	4323      	orrs	r3, r4
30009584:	4313      	orrs	r3, r2
30009586:	604b      	str	r3, [r1, #4]
30009588:	680b      	ldr	r3, [r1, #0]
3000958a:	4303      	orrs	r3, r0
3000958c:	600b      	str	r3, [r1, #0]
3000958e:	b013      	add	sp, #76	; 0x4c
30009590:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009594:	2000      	movs	r0, #0
30009596:	b013      	add	sp, #76	; 0x4c
30009598:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000959c:	9f04      	ldr	r7, [sp, #16]
3000959e:	e9dd b607 	ldrd	fp, r6, [sp, #28]
300095a2:	e794      	b.n	300094ce <PSRAM_calibration+0x19e>
300095a4:	41002000 	.word	0x41002000
300095a8:	0fffffe0 	.word	0x0fffffe0
300095ac:	e000ed00 	.word	0xe000ed00
300095b0:	3000f318 	.word	0x3000f318
300095b4:	60200000 	.word	0x60200000
300095b8:	60150000 	.word	0x60150000
300095bc:	60250000 	.word	0x60250000
300095c0:	000129bd 	.word	0x000129bd
300095c4:	3000d98c 	.word	0x3000d98c
300095c8:	3000d964 	.word	0x3000d964
300095cc:	60050000 	.word	0x60050000
300095d0:	60100000 	.word	0x60100000

300095d4 <PSRAM_AutoGating>:
300095d4:	b1c0      	cbz	r0, 30009608 <PSRAM_AutoGating+0x34>
300095d6:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300095da:	4810      	ldr	r0, [pc, #64]	; (3000961c <PSRAM_AutoGating+0x48>)
300095dc:	f3c1 0108 	ubfx	r1, r1, #0, #9
300095e0:	0252      	lsls	r2, r2, #9
300095e2:	b410      	push	{r4}
300095e4:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
300095e8:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
300095ec:	4020      	ands	r0, r4
300095ee:	4301      	orrs	r1, r0
300095f0:	430a      	orrs	r2, r1
300095f2:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
300095f6:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
300095fa:	f042 0201 	orr.w	r2, r2, #1
300095fe:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30009602:	f85d 4b04 	ldr.w	r4, [sp], #4
30009606:	4770      	bx	lr
30009608:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000960c:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30009610:	f023 0301 	bic.w	r3, r3, #1
30009614:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30009618:	4770      	bx	lr
3000961a:	bf00      	nop
3000961c:	ffffc000 	.word	0xffffc000

30009620 <BOOT_Reason>:
30009620:	4b02      	ldr	r3, [pc, #8]	; (3000962c <BOOT_Reason+0xc>)
30009622:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009626:	b280      	uxth	r0, r0
30009628:	4770      	bx	lr
3000962a:	bf00      	nop
3000962c:	42008200 	.word	0x42008200

30009630 <CRYPTO_SetSecurityModeAD>:
30009630:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009634:	b083      	sub	sp, #12
30009636:	460d      	mov	r5, r1
30009638:	4616      	mov	r6, r2
3000963a:	4699      	mov	r9, r3
3000963c:	4604      	mov	r4, r0
3000963e:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
30009642:	2800      	cmp	r0, #0
30009644:	f000 80a9 	beq.w	3000979a <CRYPTO_SetSecurityModeAD+0x16a>
30009648:	7823      	ldrb	r3, [r4, #0]
3000964a:	2b01      	cmp	r3, #1
3000964c:	d003      	beq.n	30009656 <CRYPTO_SetSecurityModeAD+0x26>
3000964e:	2125      	movs	r1, #37	; 0x25
30009650:	485d      	ldr	r0, [pc, #372]	; (300097c8 <CRYPTO_SetSecurityModeAD+0x198>)
30009652:	f002 ffc5 	bl	3000c5e0 <__io_assert_failed_veneer>
30009656:	2300      	movs	r3, #0
30009658:	1c6a      	adds	r2, r5, #1
3000965a:	6065      	str	r5, [r4, #4]
3000965c:	8123      	strh	r3, [r4, #8]
3000965e:	d008      	beq.n	30009672 <CRYPTO_SetSecurityModeAD+0x42>
30009660:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009664:	2b20      	cmp	r3, #32
30009666:	d07b      	beq.n	30009760 <CRYPTO_SetSecurityModeAD+0x130>
30009668:	f085 0580 	eor.w	r5, r5, #128	; 0x80
3000966c:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30009670:	7265      	strb	r5, [r4, #9]
30009672:	1c73      	adds	r3, r6, #1
30009674:	60e6      	str	r6, [r4, #12]
30009676:	f000 8082 	beq.w	3000977e <CRYPTO_SetSecurityModeAD+0x14e>
3000967a:	f006 0303 	and.w	r3, r6, #3
3000967e:	f3c6 0280 	ubfx	r2, r6, #2, #1
30009682:	7422      	strb	r2, [r4, #16]
30009684:	f1a3 0202 	sub.w	r2, r3, #2
30009688:	2b01      	cmp	r3, #1
3000968a:	fab2 f282 	clz	r2, r2
3000968e:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009692:	7462      	strb	r2, [r4, #17]
30009694:	fab3 f283 	clz	r2, r3
30009698:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000969c:	74a2      	strb	r2, [r4, #18]
3000969e:	bf0c      	ite	eq
300096a0:	2201      	moveq	r2, #1
300096a2:	2200      	movne	r2, #0
300096a4:	74e2      	strb	r2, [r4, #19]
300096a6:	d13f      	bne.n	30009728 <CRYPTO_SetSecurityModeAD+0xf8>
300096a8:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
300096ac:	2b40      	cmp	r3, #64	; 0x40
300096ae:	f000 8083 	beq.w	300097b8 <CRYPTO_SetSecurityModeAD+0x188>
300096b2:	d86a      	bhi.n	3000978a <CRYPTO_SetSecurityModeAD+0x15a>
300096b4:	2b10      	cmp	r3, #16
300096b6:	d06d      	beq.n	30009794 <CRYPTO_SetSecurityModeAD+0x164>
300096b8:	2b20      	cmp	r3, #32
300096ba:	d173      	bne.n	300097a4 <CRYPTO_SetSecurityModeAD+0x174>
300096bc:	7523      	strb	r3, [r4, #20]
300096be:	62a3      	str	r3, [r4, #40]	; 0x28
300096c0:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300096c2:	6227      	str	r7, [r4, #32]
300096c4:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
300096c8:	2f00      	cmp	r7, #0
300096ca:	d038      	beq.n	3000973e <CRYPTO_SetSecurityModeAD+0x10e>
300096cc:	7d23      	ldrb	r3, [r4, #20]
300096ce:	f104 0064 	add.w	r0, r4, #100	; 0x64
300096d2:	2b30      	cmp	r3, #48	; 0x30
300096d4:	65e0      	str	r0, [r4, #92]	; 0x5c
300096d6:	d046      	beq.n	30009766 <CRYPTO_SetSecurityModeAD+0x136>
300096d8:	2b40      	cmp	r3, #64	; 0x40
300096da:	d044      	beq.n	30009766 <CRYPTO_SetSecurityModeAD+0x136>
300096dc:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
300096e0:	2240      	movs	r2, #64	; 0x40
300096e2:	2136      	movs	r1, #54	; 0x36
300096e4:	4d39      	ldr	r5, [pc, #228]	; (300097cc <CRYPTO_SetSecurityModeAD+0x19c>)
300096e6:	6623      	str	r3, [r4, #96]	; 0x60
300096e8:	47a8      	blx	r5
300096ea:	2240      	movs	r2, #64	; 0x40
300096ec:	215c      	movs	r1, #92	; 0x5c
300096ee:	6e20      	ldr	r0, [r4, #96]	; 0x60
300096f0:	47a8      	blx	r5
300096f2:	4b37      	ldr	r3, [pc, #220]	; (300097d0 <CRYPTO_SetSecurityModeAD+0x1a0>)
300096f4:	681d      	ldr	r5, [r3, #0]
300096f6:	bb25      	cbnz	r5, 30009742 <CRYPTO_SetSecurityModeAD+0x112>
300096f8:	f1b8 0f00 	cmp.w	r8, #0
300096fc:	d05f      	beq.n	300097be <CRYPTO_SetSecurityModeAD+0x18e>
300096fe:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30009702:	e001      	b.n	30009708 <CRYPTO_SetSecurityModeAD+0xd8>
30009704:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30009708:	6de2      	ldr	r2, [r4, #92]	; 0x5c
3000970a:	f818 3005 	ldrb.w	r3, [r8, r5]
3000970e:	5d51      	ldrb	r1, [r2, r5]
30009710:	404b      	eors	r3, r1
30009712:	5553      	strb	r3, [r2, r5]
30009714:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009716:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009718:	5d51      	ldrb	r1, [r2, r5]
3000971a:	5d5b      	ldrb	r3, [r3, r5]
3000971c:	404b      	eors	r3, r1
3000971e:	5553      	strb	r3, [r2, r5]
30009720:	3501      	adds	r5, #1
30009722:	42af      	cmp	r7, r5
30009724:	d1ee      	bne.n	30009704 <CRYPTO_SetSecurityModeAD+0xd4>
30009726:	e00c      	b.n	30009742 <CRYPTO_SetSecurityModeAD+0x112>
30009728:	2200      	movs	r2, #0
3000972a:	7522      	strb	r2, [r4, #20]
3000972c:	b99b      	cbnz	r3, 30009756 <CRYPTO_SetSecurityModeAD+0x126>
3000972e:	2314      	movs	r3, #20
30009730:	6227      	str	r7, [r4, #32]
30009732:	62a3      	str	r3, [r4, #40]	; 0x28
30009734:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009736:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
3000973a:	2f00      	cmp	r7, #0
3000973c:	d1c6      	bne.n	300096cc <CRYPTO_SetSecurityModeAD+0x9c>
3000973e:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009742:	3601      	adds	r6, #1
30009744:	d003      	beq.n	3000974e <CRYPTO_SetSecurityModeAD+0x11e>
30009746:	2201      	movs	r2, #1
30009748:	2300      	movs	r3, #0
3000974a:	85a2      	strh	r2, [r4, #44]	; 0x2c
3000974c:	6323      	str	r3, [r4, #48]	; 0x30
3000974e:	2000      	movs	r0, #0
30009750:	b003      	add	sp, #12
30009752:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009756:	2b02      	cmp	r3, #2
30009758:	d114      	bne.n	30009784 <CRYPTO_SetSecurityModeAD+0x154>
3000975a:	2310      	movs	r3, #16
3000975c:	62a3      	str	r3, [r4, #40]	; 0x28
3000975e:	e7af      	b.n	300096c0 <CRYPTO_SetSecurityModeAD+0x90>
30009760:	2301      	movs	r3, #1
30009762:	7223      	strb	r3, [r4, #8]
30009764:	e780      	b.n	30009668 <CRYPTO_SetSecurityModeAD+0x38>
30009766:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
3000976a:	2280      	movs	r2, #128	; 0x80
3000976c:	2136      	movs	r1, #54	; 0x36
3000976e:	4d17      	ldr	r5, [pc, #92]	; (300097cc <CRYPTO_SetSecurityModeAD+0x19c>)
30009770:	6623      	str	r3, [r4, #96]	; 0x60
30009772:	47a8      	blx	r5
30009774:	2280      	movs	r2, #128	; 0x80
30009776:	215c      	movs	r1, #92	; 0x5c
30009778:	6e20      	ldr	r0, [r4, #96]	; 0x60
3000977a:	47a8      	blx	r5
3000977c:	e7b9      	b.n	300096f2 <CRYPTO_SetSecurityModeAD+0xc2>
3000977e:	2300      	movs	r3, #0
30009780:	6123      	str	r3, [r4, #16]
30009782:	7523      	strb	r3, [r4, #20]
30009784:	2300      	movs	r3, #0
30009786:	62a3      	str	r3, [r4, #40]	; 0x28
30009788:	e79a      	b.n	300096c0 <CRYPTO_SetSecurityModeAD+0x90>
3000978a:	2b80      	cmp	r3, #128	; 0x80
3000978c:	d10a      	bne.n	300097a4 <CRYPTO_SetSecurityModeAD+0x174>
3000978e:	2340      	movs	r3, #64	; 0x40
30009790:	7523      	strb	r3, [r4, #20]
30009792:	e794      	b.n	300096be <CRYPTO_SetSecurityModeAD+0x8e>
30009794:	231c      	movs	r3, #28
30009796:	7523      	strb	r3, [r4, #20]
30009798:	e7f5      	b.n	30009786 <CRYPTO_SetSecurityModeAD+0x156>
3000979a:	2124      	movs	r1, #36	; 0x24
3000979c:	480a      	ldr	r0, [pc, #40]	; (300097c8 <CRYPTO_SetSecurityModeAD+0x198>)
3000979e:	f002 ff1f 	bl	3000c5e0 <__io_assert_failed_veneer>
300097a2:	e751      	b.n	30009648 <CRYPTO_SetSecurityModeAD+0x18>
300097a4:	4b0b      	ldr	r3, [pc, #44]	; (300097d4 <CRYPTO_SetSecurityModeAD+0x1a4>)
300097a6:	2002      	movs	r0, #2
300097a8:	4a0b      	ldr	r2, [pc, #44]	; (300097d8 <CRYPTO_SetSecurityModeAD+0x1a8>)
300097aa:	4619      	mov	r1, r3
300097ac:	9600      	str	r6, [sp, #0]
300097ae:	f002 fccf 	bl	3000c150 <rtk_log_write>
300097b2:	2300      	movs	r3, #0
300097b4:	7523      	strb	r3, [r4, #20]
300097b6:	e7e6      	b.n	30009786 <CRYPTO_SetSecurityModeAD+0x156>
300097b8:	2330      	movs	r3, #48	; 0x30
300097ba:	7523      	strb	r3, [r4, #20]
300097bc:	e7e3      	b.n	30009786 <CRYPTO_SetSecurityModeAD+0x156>
300097be:	2180      	movs	r1, #128	; 0x80
300097c0:	4801      	ldr	r0, [pc, #4]	; (300097c8 <CRYPTO_SetSecurityModeAD+0x198>)
300097c2:	f002 ff0d 	bl	3000c5e0 <__io_assert_failed_veneer>
300097c6:	e79a      	b.n	300096fe <CRYPTO_SetSecurityModeAD+0xce>
300097c8:	3000da68 	.word	0x3000da68
300097cc:	00012be5 	.word	0x00012be5
300097d0:	2001c200 	.word	0x2001c200
300097d4:	3000d9cc 	.word	0x3000d9cc
300097d8:	3000d9d8 	.word	0x3000d9d8

300097dc <rtl_crypto_hmac_sha2_init>:
300097dc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300097e0:	4c97      	ldr	r4, [pc, #604]	; (30009a40 <rtl_crypto_hmac_sha2_init+0x264>)
300097e2:	4617      	mov	r7, r2
300097e4:	e844 f400 	tt	r4, r4
300097e8:	4a96      	ldr	r2, [pc, #600]	; (30009a44 <rtl_crypto_hmac_sha2_init+0x268>)
300097ea:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
300097ee:	461c      	mov	r4, r3
300097f0:	4b95      	ldr	r3, [pc, #596]	; (30009a48 <rtl_crypto_hmac_sha2_init+0x26c>)
300097f2:	bf18      	it	ne
300097f4:	4613      	movne	r3, r2
300097f6:	b085      	sub	sp, #20
300097f8:	4605      	mov	r5, r0
300097fa:	460e      	mov	r6, r1
300097fc:	6b5b      	ldr	r3, [r3, #52]	; 0x34
300097fe:	2b00      	cmp	r3, #0
30009800:	db05      	blt.n	3000980e <rtl_crypto_hmac_sha2_init+0x32>
30009802:	2900      	cmp	r1, #0
30009804:	f000 8118 	beq.w	30009a38 <rtl_crypto_hmac_sha2_init+0x25c>
30009808:	078b      	lsls	r3, r1, #30
3000980a:	f040 8112 	bne.w	30009a32 <rtl_crypto_hmac_sha2_init+0x256>
3000980e:	2d20      	cmp	r5, #32
30009810:	bf18      	it	ne
30009812:	2d1c      	cmpne	r5, #28
30009814:	bf0c      	ite	eq
30009816:	f04f 0901 	moveq.w	r9, #1
3000981a:	f04f 0900 	movne.w	r9, #0
3000981e:	d04d      	beq.n	300098bc <rtl_crypto_hmac_sha2_init+0xe0>
30009820:	2d40      	cmp	r5, #64	; 0x40
30009822:	bf18      	it	ne
30009824:	2d30      	cmpne	r5, #48	; 0x30
30009826:	d102      	bne.n	3000982e <rtl_crypto_hmac_sha2_init+0x52>
30009828:	2f80      	cmp	r7, #128	; 0x80
3000982a:	f200 80f9 	bhi.w	30009a20 <rtl_crypto_hmac_sha2_init+0x244>
3000982e:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30009a68 <rtl_crypto_hmac_sha2_init+0x28c>
30009832:	f898 3000 	ldrb.w	r3, [r8]
30009836:	2b01      	cmp	r3, #1
30009838:	d003      	beq.n	30009842 <rtl_crypto_hmac_sha2_init+0x66>
3000983a:	21b4      	movs	r1, #180	; 0xb4
3000983c:	4883      	ldr	r0, [pc, #524]	; (30009a4c <rtl_crypto_hmac_sha2_init+0x270>)
3000983e:	f002 fecf 	bl	3000c5e0 <__io_assert_failed_veneer>
30009842:	2d30      	cmp	r5, #48	; 0x30
30009844:	f000 809b 	beq.w	3000997e <rtl_crypto_hmac_sha2_init+0x1a2>
30009848:	d83f      	bhi.n	300098ca <rtl_crypto_hmac_sha2_init+0xee>
3000984a:	2d1c      	cmp	r5, #28
3000984c:	f000 80cb 	beq.w	300099e6 <rtl_crypto_hmac_sha2_init+0x20a>
30009850:	f1b9 0f00 	cmp.w	r9, #0
30009854:	d078      	beq.n	30009948 <rtl_crypto_hmac_sha2_init+0x16c>
30009856:	2c00      	cmp	r4, #0
30009858:	f000 808f 	beq.w	3000997a <rtl_crypto_hmac_sha2_init+0x19e>
3000985c:	4b7c      	ldr	r3, [pc, #496]	; (30009a50 <rtl_crypto_hmac_sha2_init+0x274>)
3000985e:	f8df e210 	ldr.w	lr, [pc, #528]	; 30009a70 <rtl_crypto_hmac_sha2_init+0x294>
30009862:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30009a80 <rtl_crypto_hmac_sha2_init+0x2a4>
30009866:	4a7b      	ldr	r2, [pc, #492]	; (30009a54 <rtl_crypto_hmac_sha2_init+0x278>)
30009868:	e9c4 e302 	strd	lr, r3, [r4, #8]
3000986c:	4b7a      	ldr	r3, [pc, #488]	; (30009a58 <rtl_crypto_hmac_sha2_init+0x27c>)
3000986e:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009872:	4b7a      	ldr	r3, [pc, #488]	; (30009a5c <rtl_crypto_hmac_sha2_init+0x280>)
30009874:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009878:	4b79      	ldr	r3, [pc, #484]	; (30009a60 <rtl_crypto_hmac_sha2_init+0x284>)
3000987a:	2225      	movs	r2, #37	; 0x25
3000987c:	6223      	str	r3, [r4, #32]
3000987e:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009882:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009886:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
3000988a:	6263      	str	r3, [r4, #36]	; 0x24
3000988c:	2300      	movs	r3, #0
3000988e:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30009892:	4d74      	ldr	r5, [pc, #464]	; (30009a64 <rtl_crypto_hmac_sha2_init+0x288>)
30009894:	9300      	str	r3, [sp, #0]
30009896:	9702      	str	r7, [sp, #8]
30009898:	9601      	str	r6, [sp, #4]
3000989a:	4873      	ldr	r0, [pc, #460]	; (30009a68 <rtl_crypto_hmac_sha2_init+0x28c>)
3000989c:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
300098a0:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
300098a4:	47a8      	blx	r5
300098a6:	4605      	mov	r5, r0
300098a8:	4b70      	ldr	r3, [pc, #448]	; (30009a6c <rtl_crypto_hmac_sha2_init+0x290>)
300098aa:	4620      	mov	r0, r4
300098ac:	4798      	blx	r3
300098ae:	2301      	movs	r3, #1
300098b0:	4628      	mov	r0, r5
300098b2:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
300098b6:	b005      	add	sp, #20
300098b8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300098bc:	2f40      	cmp	r7, #64	; 0x40
300098be:	d9b6      	bls.n	3000982e <rtl_crypto_hmac_sha2_init+0x52>
300098c0:	21ae      	movs	r1, #174	; 0xae
300098c2:	4862      	ldr	r0, [pc, #392]	; (30009a4c <rtl_crypto_hmac_sha2_init+0x270>)
300098c4:	f002 fe8c 	bl	3000c5e0 <__io_assert_failed_veneer>
300098c8:	e7b1      	b.n	3000982e <rtl_crypto_hmac_sha2_init+0x52>
300098ca:	2d40      	cmp	r5, #64	; 0x40
300098cc:	d13c      	bne.n	30009948 <rtl_crypto_hmac_sha2_init+0x16c>
300098ce:	2c00      	cmp	r4, #0
300098d0:	d040      	beq.n	30009954 <rtl_crypto_hmac_sha2_init+0x178>
300098d2:	4b67      	ldr	r3, [pc, #412]	; (30009a70 <rtl_crypto_hmac_sha2_init+0x294>)
300098d4:	4a67      	ldr	r2, [pc, #412]	; (30009a74 <rtl_crypto_hmac_sha2_init+0x298>)
300098d6:	4968      	ldr	r1, [pc, #416]	; (30009a78 <rtl_crypto_hmac_sha2_init+0x29c>)
300098d8:	4d68      	ldr	r5, [pc, #416]	; (30009a7c <rtl_crypto_hmac_sha2_init+0x2a0>)
300098da:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30009ac0 <rtl_crypto_hmac_sha2_init+0x2e4>
300098de:	e9c4 3202 	strd	r3, r2, [r4, #8]
300098e2:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
300098e6:	2285      	movs	r2, #133	; 0x85
300098e8:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
300098ec:	33d4      	adds	r3, #212	; 0xd4
300098ee:	6163      	str	r3, [r4, #20]
300098f0:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
300098f4:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
300098f8:	f203 734a 	addw	r3, r3, #1866	; 0x74a
300098fc:	6123      	str	r3, [r4, #16]
300098fe:	4b60      	ldr	r3, [pc, #384]	; (30009a80 <rtl_crypto_hmac_sha2_init+0x2a4>)
30009900:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009904:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30009908:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
3000990c:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009910:	6263      	str	r3, [r4, #36]	; 0x24
30009912:	4b51      	ldr	r3, [pc, #324]	; (30009a58 <rtl_crypto_hmac_sha2_init+0x27c>)
30009914:	6223      	str	r3, [r4, #32]
30009916:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
3000991a:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
3000991e:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009922:	62e3      	str	r3, [r4, #44]	; 0x2c
30009924:	4b4b      	ldr	r3, [pc, #300]	; (30009a54 <rtl_crypto_hmac_sha2_init+0x278>)
30009926:	62a3      	str	r3, [r4, #40]	; 0x28
30009928:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
3000992c:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30009930:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009934:	6363      	str	r3, [r4, #52]	; 0x34
30009936:	4b49      	ldr	r3, [pc, #292]	; (30009a5c <rtl_crypto_hmac_sha2_init+0x280>)
30009938:	6323      	str	r3, [r4, #48]	; 0x30
3000993a:	4b49      	ldr	r3, [pc, #292]	; (30009a60 <rtl_crypto_hmac_sha2_init+0x284>)
3000993c:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009940:	4b50      	ldr	r3, [pc, #320]	; (30009a84 <rtl_crypto_hmac_sha2_init+0x2a8>)
30009942:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009946:	e7a1      	b.n	3000988c <rtl_crypto_hmac_sha2_init+0xb0>
30009948:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
3000994c:	4628      	mov	r0, r5
3000994e:	b005      	add	sp, #20
30009950:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009954:	2285      	movs	r2, #133	; 0x85
30009956:	2300      	movs	r3, #0
30009958:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000995c:	4c41      	ldr	r4, [pc, #260]	; (30009a64 <rtl_crypto_hmac_sha2_init+0x288>)
3000995e:	9702      	str	r7, [sp, #8]
30009960:	4841      	ldr	r0, [pc, #260]	; (30009a68 <rtl_crypto_hmac_sha2_init+0x28c>)
30009962:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009966:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
3000996a:	e9cd 3600 	strd	r3, r6, [sp]
3000996e:	47a0      	blx	r4
30009970:	4605      	mov	r5, r0
30009972:	4628      	mov	r0, r5
30009974:	b005      	add	sp, #20
30009976:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000997a:	2225      	movs	r2, #37	; 0x25
3000997c:	e7eb      	b.n	30009956 <rtl_crypto_hmac_sha2_init+0x17a>
3000997e:	2c00      	cmp	r4, #0
30009980:	d053      	beq.n	30009a2a <rtl_crypto_hmac_sha2_init+0x24e>
30009982:	4b41      	ldr	r3, [pc, #260]	; (30009a88 <rtl_crypto_hmac_sha2_init+0x2ac>)
30009984:	f8df e130 	ldr.w	lr, [pc, #304]	; 30009ab8 <rtl_crypto_hmac_sha2_init+0x2dc>
30009988:	4a40      	ldr	r2, [pc, #256]	; (30009a8c <rtl_crypto_hmac_sha2_init+0x2b0>)
3000998a:	4941      	ldr	r1, [pc, #260]	; (30009a90 <rtl_crypto_hmac_sha2_init+0x2b4>)
3000998c:	4d41      	ldr	r5, [pc, #260]	; (30009a94 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000998e:	f8df c12c 	ldr.w	ip, [pc, #300]	; 30009abc <rtl_crypto_hmac_sha2_init+0x2e0>
30009992:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30009996:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
3000999a:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
3000999e:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
300099a2:	6163      	str	r3, [r4, #20]
300099a4:	4b3c      	ldr	r3, [pc, #240]	; (30009a98 <rtl_crypto_hmac_sha2_init+0x2bc>)
300099a6:	6123      	str	r3, [r4, #16]
300099a8:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
300099ac:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
300099b0:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
300099b4:	61e3      	str	r3, [r4, #28]
300099b6:	4b39      	ldr	r3, [pc, #228]	; (30009a9c <rtl_crypto_hmac_sha2_init+0x2c0>)
300099b8:	61a3      	str	r3, [r4, #24]
300099ba:	4b39      	ldr	r3, [pc, #228]	; (30009aa0 <rtl_crypto_hmac_sha2_init+0x2c4>)
300099bc:	e9c4 3208 	strd	r3, r2, [r4, #32]
300099c0:	4b38      	ldr	r3, [pc, #224]	; (30009aa4 <rtl_crypto_hmac_sha2_init+0x2c8>)
300099c2:	2245      	movs	r2, #69	; 0x45
300099c4:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
300099c8:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
300099cc:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
300099d0:	33aa      	adds	r3, #170	; 0xaa
300099d2:	6363      	str	r3, [r4, #52]	; 0x34
300099d4:	4b34      	ldr	r3, [pc, #208]	; (30009aa8 <rtl_crypto_hmac_sha2_init+0x2cc>)
300099d6:	6323      	str	r3, [r4, #48]	; 0x30
300099d8:	4b34      	ldr	r3, [pc, #208]	; (30009aac <rtl_crypto_hmac_sha2_init+0x2d0>)
300099da:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
300099de:	4b34      	ldr	r3, [pc, #208]	; (30009ab0 <rtl_crypto_hmac_sha2_init+0x2d4>)
300099e0:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
300099e4:	e752      	b.n	3000988c <rtl_crypto_hmac_sha2_init+0xb0>
300099e6:	b314      	cbz	r4, 30009a2e <rtl_crypto_hmac_sha2_init+0x252>
300099e8:	4b32      	ldr	r3, [pc, #200]	; (30009ab4 <rtl_crypto_hmac_sha2_init+0x2d8>)
300099ea:	2215      	movs	r2, #21
300099ec:	4932      	ldr	r1, [pc, #200]	; (30009ab8 <rtl_crypto_hmac_sha2_init+0x2dc>)
300099ee:	4827      	ldr	r0, [pc, #156]	; (30009a8c <rtl_crypto_hmac_sha2_init+0x2b0>)
300099f0:	4d28      	ldr	r5, [pc, #160]	; (30009a94 <rtl_crypto_hmac_sha2_init+0x2b8>)
300099f2:	e9c4 1302 	strd	r1, r3, [r4, #8]
300099f6:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
300099fa:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
300099fe:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30009a02:	6123      	str	r3, [r4, #16]
30009a04:	4b22      	ldr	r3, [pc, #136]	; (30009a90 <rtl_crypto_hmac_sha2_init+0x2b4>)
30009a06:	e9c4 0305 	strd	r0, r3, [r4, #20]
30009a0a:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30009a0e:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30009a12:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30009a16:	61e3      	str	r3, [r4, #28]
30009a18:	4b28      	ldr	r3, [pc, #160]	; (30009abc <rtl_crypto_hmac_sha2_init+0x2e0>)
30009a1a:	e9c4 5308 	strd	r5, r3, [r4, #32]
30009a1e:	e735      	b.n	3000988c <rtl_crypto_hmac_sha2_init+0xb0>
30009a20:	21b1      	movs	r1, #177	; 0xb1
30009a22:	480a      	ldr	r0, [pc, #40]	; (30009a4c <rtl_crypto_hmac_sha2_init+0x270>)
30009a24:	f002 fddc 	bl	3000c5e0 <__io_assert_failed_veneer>
30009a28:	e701      	b.n	3000982e <rtl_crypto_hmac_sha2_init+0x52>
30009a2a:	2245      	movs	r2, #69	; 0x45
30009a2c:	e793      	b.n	30009956 <rtl_crypto_hmac_sha2_init+0x17a>
30009a2e:	2215      	movs	r2, #21
30009a30:	e791      	b.n	30009956 <rtl_crypto_hmac_sha2_init+0x17a>
30009a32:	f06f 0505 	mvn.w	r5, #5
30009a36:	e789      	b.n	3000994c <rtl_crypto_hmac_sha2_init+0x170>
30009a38:	f06f 0503 	mvn.w	r5, #3
30009a3c:	e786      	b.n	3000994c <rtl_crypto_hmac_sha2_init+0x170>
30009a3e:	bf00      	nop
30009a40:	0000e5f9 	.word	0x0000e5f9
30009a44:	500c8000 	.word	0x500c8000
30009a48:	400c8000 	.word	0x400c8000
30009a4c:	3000da4c 	.word	0x3000da4c
30009a50:	bb67ae85 	.word	0xbb67ae85
30009a54:	510e527f 	.word	0x510e527f
30009a58:	a54ff53a 	.word	0xa54ff53a
30009a5c:	9b05688c 	.word	0x9b05688c
30009a60:	1f83d9ab 	.word	0x1f83d9ab
30009a64:	30009631 	.word	0x30009631
30009a68:	2001c460 	.word	0x2001c460
30009a6c:	00004c99 	.word	0x00004c99
30009a70:	6a09e667 	.word	0x6a09e667
30009a74:	f3bcc908 	.word	0xf3bcc908
30009a78:	fe94f82b 	.word	0xfe94f82b
30009a7c:	fb41bd6b 	.word	0xfb41bd6b
30009a80:	3c6ef372 	.word	0x3c6ef372
30009a84:	5be0cd19 	.word	0x5be0cd19
30009a88:	cbbb9d5d 	.word	0xcbbb9d5d
30009a8c:	f70e5939 	.word	0xf70e5939
30009a90:	ffc00b31 	.word	0xffc00b31
30009a94:	64f98fa7 	.word	0x64f98fa7
30009a98:	629a292a 	.word	0x629a292a
30009a9c:	9159015a 	.word	0x9159015a
30009aa0:	152fecd8 	.word	0x152fecd8
30009aa4:	67332667 	.word	0x67332667
30009aa8:	8eb44a87 	.word	0x8eb44a87
30009aac:	db0c2e0d 	.word	0xdb0c2e0d
30009ab0:	47b5481d 	.word	0x47b5481d
30009ab4:	367cd507 	.word	0x367cd507
30009ab8:	c1059ed8 	.word	0xc1059ed8
30009abc:	befa4fa4 	.word	0xbefa4fa4
30009ac0:	137e2179 	.word	0x137e2179

30009ac4 <CPU_ClkGet>:
30009ac4:	b570      	push	{r4, r5, r6, lr}
30009ac6:	4d1e      	ldr	r5, [pc, #120]	; (30009b40 <CPU_ClkGet+0x7c>)
30009ac8:	4b1e      	ldr	r3, [pc, #120]	; (30009b44 <CPU_ClkGet+0x80>)
30009aca:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
30009ace:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30009ad2:	f3c4 1481 	ubfx	r4, r4, #6, #2
30009ad6:	4798      	blx	r3
30009ad8:	b92c      	cbnz	r4, 30009ae6 <CPU_ClkGet+0x22>
30009ada:	2801      	cmp	r0, #1
30009adc:	d007      	beq.n	30009aee <CPU_ClkGet+0x2a>
30009ade:	07b2      	lsls	r2, r6, #30
30009ae0:	d407      	bmi.n	30009af2 <CPU_ClkGet+0x2e>
30009ae2:	4819      	ldr	r0, [pc, #100]	; (30009b48 <CPU_ClkGet+0x84>)
30009ae4:	bd70      	pop	{r4, r5, r6, pc}
30009ae6:	2c01      	cmp	r4, #1
30009ae8:	d007      	beq.n	30009afa <CPU_ClkGet+0x36>
30009aea:	2000      	movs	r0, #0
30009aec:	bd70      	pop	{r4, r5, r6, pc}
30009aee:	4817      	ldr	r0, [pc, #92]	; (30009b4c <CPU_ClkGet+0x88>)
30009af0:	bd70      	pop	{r4, r5, r6, pc}
30009af2:	4b17      	ldr	r3, [pc, #92]	; (30009b50 <CPU_ClkGet+0x8c>)
30009af4:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30009af8:	4718      	bx	r3
30009afa:	2801      	cmp	r0, #1
30009afc:	d011      	beq.n	30009b22 <CPU_ClkGet+0x5e>
30009afe:	05f3      	lsls	r3, r6, #23
30009b00:	d51c      	bpl.n	30009b3c <CPU_ClkGet+0x78>
30009b02:	4914      	ldr	r1, [pc, #80]	; (30009b54 <CPU_ClkGet+0x90>)
30009b04:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30009b08:	6c48      	ldr	r0, [r1, #68]	; 0x44
30009b0a:	4a13      	ldr	r2, [pc, #76]	; (30009b58 <CPU_ClkGet+0x94>)
30009b0c:	f003 0307 	and.w	r3, r3, #7
30009b10:	f3c0 1045 	ubfx	r0, r0, #5, #6
30009b14:	3301      	adds	r3, #1
30009b16:	3002      	adds	r0, #2
30009b18:	fb02 f000 	mul.w	r0, r2, r0
30009b1c:	fbb0 f0f3 	udiv	r0, r0, r3
30009b20:	bd70      	pop	{r4, r5, r6, pc}
30009b22:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30009b26:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30009b2a:	f3c3 4007 	ubfx	r0, r3, #16, #8
30009b2e:	d003      	beq.n	30009b38 <CPU_ClkGet+0x74>
30009b30:	4b0a      	ldr	r3, [pc, #40]	; (30009b5c <CPU_ClkGet+0x98>)
30009b32:	fb03 f000 	mul.w	r0, r3, r0
30009b36:	bd70      	pop	{r4, r5, r6, pc}
30009b38:	4809      	ldr	r0, [pc, #36]	; (30009b60 <CPU_ClkGet+0x9c>)
30009b3a:	bd70      	pop	{r4, r5, r6, pc}
30009b3c:	4806      	ldr	r0, [pc, #24]	; (30009b58 <CPU_ClkGet+0x94>)
30009b3e:	bd70      	pop	{r4, r5, r6, pc}
30009b40:	42008000 	.word	0x42008000
30009b44:	0000c0f9 	.word	0x0000c0f9
30009b48:	003d0900 	.word	0x003d0900
30009b4c:	01312d00 	.word	0x01312d00
30009b50:	000099f5 	.word	0x000099f5
30009b54:	42008800 	.word	0x42008800
30009b58:	02625a00 	.word	0x02625a00
30009b5c:	000f4240 	.word	0x000f4240
30009b60:	02faf080 	.word	0x02faf080

30009b64 <RSIP_IV_Set>:
30009b64:	4b05      	ldr	r3, [pc, #20]	; (30009b7c <RSIP_IV_Set+0x18>)
30009b66:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30009b6a:	680b      	ldr	r3, [r1, #0]
30009b6c:	ba1b      	rev	r3, r3
30009b6e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009b72:	684b      	ldr	r3, [r1, #4]
30009b74:	ba1b      	rev	r3, r3
30009b76:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30009b7a:	4770      	bx	lr
30009b7c:	42008c00 	.word	0x42008c00

30009b80 <RSIP_OTF_Cmd>:
30009b80:	4a04      	ldr	r2, [pc, #16]	; (30009b94 <RSIP_OTF_Cmd+0x14>)
30009b82:	2801      	cmp	r0, #1
30009b84:	6953      	ldr	r3, [r2, #20]
30009b86:	bf0c      	ite	eq
30009b88:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30009b8c:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30009b90:	6153      	str	r3, [r2, #20]
30009b92:	4770      	bx	lr
30009b94:	42008c00 	.word	0x42008c00

30009b98 <RSIP_OTF_Enable>:
30009b98:	b570      	push	{r4, r5, r6, lr}
30009b9a:	4604      	mov	r4, r0
30009b9c:	b1f3      	cbz	r3, 30009bdc <RSIP_OTF_Enable+0x44>
30009b9e:	9b04      	ldr	r3, [sp, #16]
30009ba0:	460d      	mov	r5, r1
30009ba2:	4616      	mov	r6, r2
30009ba4:	2b07      	cmp	r3, #7
30009ba6:	d827      	bhi.n	30009bf8 <RSIP_OTF_Enable+0x60>
30009ba8:	9b05      	ldr	r3, [sp, #20]
30009baa:	3b01      	subs	r3, #1
30009bac:	2b01      	cmp	r3, #1
30009bae:	d81d      	bhi.n	30009bec <RSIP_OTF_Enable+0x54>
30009bb0:	9a04      	ldr	r2, [sp, #16]
30009bb2:	9b05      	ldr	r3, [sp, #20]
30009bb4:	f002 0007 	and.w	r0, r2, #7
30009bb8:	005b      	lsls	r3, r3, #1
30009bba:	4305      	orrs	r5, r0
30009bbc:	4813      	ldr	r0, [pc, #76]	; (30009c0c <RSIP_OTF_Enable+0x74>)
30009bbe:	f003 0306 	and.w	r3, r3, #6
30009bc2:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009bc6:	ea43 0206 	orr.w	r2, r3, r6
30009bca:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009bcc:	f042 0201 	orr.w	r2, r2, #1
30009bd0:	f023 0301 	bic.w	r3, r3, #1
30009bd4:	6463      	str	r3, [r4, #68]	; 0x44
30009bd6:	6425      	str	r5, [r4, #64]	; 0x40
30009bd8:	6462      	str	r2, [r4, #68]	; 0x44
30009bda:	bd70      	pop	{r4, r5, r6, pc}
30009bdc:	480b      	ldr	r0, [pc, #44]	; (30009c0c <RSIP_OTF_Enable+0x74>)
30009bde:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009be2:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009be4:	f023 0301 	bic.w	r3, r3, #1
30009be8:	6463      	str	r3, [r4, #68]	; 0x44
30009bea:	bd70      	pop	{r4, r5, r6, pc}
30009bec:	f240 2131 	movw	r1, #561	; 0x231
30009bf0:	4807      	ldr	r0, [pc, #28]	; (30009c10 <RSIP_OTF_Enable+0x78>)
30009bf2:	f002 fcf5 	bl	3000c5e0 <__io_assert_failed_veneer>
30009bf6:	e7db      	b.n	30009bb0 <RSIP_OTF_Enable+0x18>
30009bf8:	f44f 710c 	mov.w	r1, #560	; 0x230
30009bfc:	4804      	ldr	r0, [pc, #16]	; (30009c10 <RSIP_OTF_Enable+0x78>)
30009bfe:	f002 fcef 	bl	3000c5e0 <__io_assert_failed_veneer>
30009c02:	9b05      	ldr	r3, [sp, #20]
30009c04:	3b01      	subs	r3, #1
30009c06:	2b01      	cmp	r3, #1
30009c08:	d9d2      	bls.n	30009bb0 <RSIP_OTF_Enable+0x18>
30009c0a:	e7ef      	b.n	30009bec <RSIP_OTF_Enable+0x54>
30009c0c:	42008c00 	.word	0x42008c00
30009c10:	3000da3c 	.word	0x3000da3c

30009c14 <RSIP_MMU_Config>:
30009c14:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009c18:	311f      	adds	r1, #31
30009c1a:	321f      	adds	r2, #31
30009c1c:	331f      	adds	r3, #31
30009c1e:	0080      	lsls	r0, r0, #2
30009c20:	0949      	lsrs	r1, r1, #5
30009c22:	0952      	lsrs	r2, r2, #5
30009c24:	095b      	lsrs	r3, r3, #5
30009c26:	0249      	lsls	r1, r1, #9
30009c28:	0252      	lsls	r2, r2, #9
30009c2a:	025b      	lsls	r3, r3, #9
30009c2c:	b410      	push	{r4}
30009c2e:	4c08      	ldr	r4, [pc, #32]	; (30009c50 <RSIP_MMU_Config+0x3c>)
30009c30:	4404      	add	r4, r0
30009c32:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
30009c36:	f020 0001 	bic.w	r0, r0, #1
30009c3a:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
30009c3e:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
30009c42:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
30009c46:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
30009c4a:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c4e:	4770      	bx	lr
30009c50:	42008c00 	.word	0x42008c00

30009c54 <RSIP_MMU_Cmd>:
30009c54:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009c58:	4b07      	ldr	r3, [pc, #28]	; (30009c78 <RSIP_MMU_Cmd+0x24>)
30009c5a:	0080      	lsls	r0, r0, #2
30009c5c:	4403      	add	r3, r0
30009c5e:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
30009c62:	b121      	cbz	r1, 30009c6e <RSIP_MMU_Cmd+0x1a>
30009c64:	f042 0201 	orr.w	r2, r2, #1
30009c68:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009c6c:	4770      	bx	lr
30009c6e:	f022 0201 	bic.w	r2, r2, #1
30009c72:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009c76:	4770      	bx	lr
30009c78:	42008c00 	.word	0x42008c00

30009c7c <RSIP_MMU_Cache_Clean>:
30009c7c:	4a02      	ldr	r2, [pc, #8]	; (30009c88 <RSIP_MMU_Cache_Clean+0xc>)
30009c7e:	6993      	ldr	r3, [r2, #24]
30009c80:	f043 0304 	orr.w	r3, r3, #4
30009c84:	6193      	str	r3, [r2, #24]
30009c86:	4770      	bx	lr
30009c88:	42008c00 	.word	0x42008c00

30009c8c <NAND_RxData>:
30009c8c:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009c90:	460c      	mov	r4, r1
30009c92:	4616      	mov	r6, r2
30009c94:	461f      	mov	r7, r3
30009c96:	2800      	cmp	r0, #0
30009c98:	f000 808a 	beq.w	30009db0 <NAND_RxData+0x124>
30009c9c:	4605      	mov	r5, r0
30009c9e:	f8df 911c 	ldr.w	r9, [pc, #284]	; 30009dbc <NAND_RxData+0x130>
30009ca2:	2000      	movs	r0, #0
30009ca4:	f8df 8118 	ldr.w	r8, [pc, #280]	; 30009dc0 <NAND_RxData+0x134>
30009ca8:	47c0      	blx	r8
30009caa:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30009cae:	2000      	movs	r0, #0
30009cb0:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
30009cb4:	6819      	ldr	r1, [r3, #0]
30009cb6:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
30009cba:	6019      	str	r1, [r3, #0]
30009cbc:	6819      	ldr	r1, [r3, #0]
30009cbe:	f441 7140 	orr.w	r1, r1, #768	; 0x300
30009cc2:	6019      	str	r1, [r3, #0]
30009cc4:	605a      	str	r2, [r3, #4]
30009cc6:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
30009cca:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
30009cce:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
30009cd2:	ea4f 420c 	mov.w	r2, ip, lsl #16
30009cd6:	f1bc 0f03 	cmp.w	ip, #3
30009cda:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30009cde:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009ce2:	ea42 0201 	orr.w	r2, r2, r1
30009ce6:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30009cea:	f04f 0201 	mov.w	r2, #1
30009cee:	611a      	str	r2, [r3, #16]
30009cf0:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30009cf4:	bf08      	it	eq
30009cf6:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30009cfa:	f3c4 2307 	ubfx	r3, r4, #8, #8
30009cfe:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
30009d02:	b2e4      	uxtb	r4, r4
30009d04:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009d08:	2301      	movs	r3, #1
30009d0a:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30009d0e:	f017 0403 	ands.w	r4, r7, #3
30009d12:	6083      	str	r3, [r0, #8]
30009d14:	d137      	bne.n	30009d86 <NAND_RxData+0xfa>
30009d16:	463b      	mov	r3, r7
30009d18:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30009d1c:	42b4      	cmp	r4, r6
30009d1e:	d216      	bcs.n	30009d4e <NAND_RxData+0xc2>
30009d20:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
30009d24:	f012 0f3c 	tst.w	r2, #60	; 0x3c
30009d28:	f3c2 0583 	ubfx	r5, r2, #2, #4
30009d2c:	d00c      	beq.n	30009d48 <NAND_RxData+0xbc>
30009d2e:	00ad      	lsls	r5, r5, #2
30009d30:	1f19      	subs	r1, r3, #4
30009d32:	f1a5 0c04 	sub.w	ip, r5, #4
30009d36:	449c      	add	ip, r3
30009d38:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
30009d3c:	f841 0f04 	str.w	r0, [r1, #4]!
30009d40:	458c      	cmp	ip, r1
30009d42:	d1f9      	bne.n	30009d38 <NAND_RxData+0xac>
30009d44:	442b      	add	r3, r5
30009d46:	442c      	add	r4, r5
30009d48:	1b32      	subs	r2, r6, r4
30009d4a:	2a03      	cmp	r2, #3
30009d4c:	d8e6      	bhi.n	30009d1c <NAND_RxData+0x90>
30009d4e:	1e63      	subs	r3, r4, #1
30009d50:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009d54:	441f      	add	r7, r3
30009d56:	e002      	b.n	30009d5e <NAND_RxData+0xd2>
30009d58:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009d5a:	071b      	lsls	r3, r3, #28
30009d5c:	d40d      	bmi.n	30009d7a <NAND_RxData+0xee>
30009d5e:	42b4      	cmp	r4, r6
30009d60:	d3fa      	bcc.n	30009d58 <NAND_RxData+0xcc>
30009d62:	2004      	movs	r0, #4
30009d64:	47c0      	blx	r8
30009d66:	2000      	movs	r0, #0
30009d68:	47c0      	blx	r8
30009d6a:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009d6e:	6813      	ldr	r3, [r2, #0]
30009d70:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009d74:	6013      	str	r3, [r2, #0]
30009d76:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009d7a:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
30009d7e:	3401      	adds	r4, #1
30009d80:	f807 3f01 	strb.w	r3, [r7, #1]!
30009d84:	e7eb      	b.n	30009d5e <NAND_RxData+0xd2>
30009d86:	f1c4 0404 	rsb	r4, r4, #4
30009d8a:	2200      	movs	r2, #0
30009d8c:	4639      	mov	r1, r7
30009d8e:	e00a      	b.n	30009da6 <NAND_RxData+0x11a>
30009d90:	6a85      	ldr	r5, [r0, #40]	; 0x28
30009d92:	072d      	lsls	r5, r5, #28
30009d94:	d505      	bpl.n	30009da2 <NAND_RxData+0x116>
30009d96:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
30009d9a:	3201      	adds	r2, #1
30009d9c:	700b      	strb	r3, [r1, #0]
30009d9e:	18b9      	adds	r1, r7, r2
30009da0:	460b      	mov	r3, r1
30009da2:	4294      	cmp	r4, r2
30009da4:	d0b8      	beq.n	30009d18 <NAND_RxData+0x8c>
30009da6:	42b2      	cmp	r2, r6
30009da8:	460b      	mov	r3, r1
30009daa:	d3f1      	bcc.n	30009d90 <NAND_RxData+0x104>
30009dac:	4614      	mov	r4, r2
30009dae:	e7ce      	b.n	30009d4e <NAND_RxData+0xc2>
30009db0:	f8df 9008 	ldr.w	r9, [pc, #8]	; 30009dbc <NAND_RxData+0x130>
30009db4:	f899 5008 	ldrb.w	r5, [r9, #8]
30009db8:	e773      	b.n	30009ca2 <NAND_RxData+0x16>
30009dba:	bf00      	nop
30009dbc:	2001c01c 	.word	0x2001c01c
30009dc0:	0000b9c1 	.word	0x0000b9c1

30009dc4 <NAND_Page_Read>:
30009dc4:	b530      	push	{r4, r5, lr}
30009dc6:	b085      	sub	sp, #20
30009dc8:	4c07      	ldr	r4, [pc, #28]	; (30009de8 <NAND_Page_Read+0x24>)
30009dca:	4d08      	ldr	r5, [pc, #32]	; (30009dec <NAND_Page_Read+0x28>)
30009dcc:	9301      	str	r3, [sp, #4]
30009dce:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009dd2:	47a0      	blx	r4
30009dd4:	4604      	mov	r4, r0
30009dd6:	9903      	ldr	r1, [sp, #12]
30009dd8:	7a28      	ldrb	r0, [r5, #8]
30009dda:	4d05      	ldr	r5, [pc, #20]	; (30009df0 <NAND_Page_Read+0x2c>)
30009ddc:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30009de0:	47a8      	blx	r5
30009de2:	4620      	mov	r0, r4
30009de4:	b005      	add	sp, #20
30009de6:	bd30      	pop	{r4, r5, pc}
30009de8:	0000bd85 	.word	0x0000bd85
30009dec:	2001c01c 	.word	0x2001c01c
30009df0:	30009c8d 	.word	0x30009c8d

30009df4 <irq_register>:
30009df4:	b530      	push	{r4, r5, lr}
30009df6:	461c      	mov	r4, r3
30009df8:	460d      	mov	r5, r1
30009dfa:	f040 0001 	orr.w	r0, r0, #1
30009dfe:	4916      	ldr	r1, [pc, #88]	; (30009e58 <irq_register+0x64>)
30009e00:	4b16      	ldr	r3, [pc, #88]	; (30009e5c <irq_register+0x68>)
30009e02:	2c07      	cmp	r4, #7
30009e04:	f841 0025 	str.w	r0, [r1, r5, lsl #2]
30009e08:	b085      	sub	sp, #20
30009e0a:	f843 2025 	str.w	r2, [r3, r5, lsl #2]
30009e0e:	d813      	bhi.n	30009e38 <irq_register+0x44>
30009e10:	2c07      	cmp	r4, #7
30009e12:	bf28      	it	cs
30009e14:	2407      	movcs	r4, #7
30009e16:	2d00      	cmp	r5, #0
30009e18:	ea4f 1444 	mov.w	r4, r4, lsl #5
30009e1c:	b2e4      	uxtb	r4, r4
30009e1e:	db04      	blt.n	30009e2a <irq_register+0x36>
30009e20:	4b0f      	ldr	r3, [pc, #60]	; (30009e60 <irq_register+0x6c>)
30009e22:	2001      	movs	r0, #1
30009e24:	555c      	strb	r4, [r3, r5]
30009e26:	b005      	add	sp, #20
30009e28:	bd30      	pop	{r4, r5, pc}
30009e2a:	f005 050f 	and.w	r5, r5, #15
30009e2e:	4b0d      	ldr	r3, [pc, #52]	; (30009e64 <irq_register+0x70>)
30009e30:	2001      	movs	r0, #1
30009e32:	555c      	strb	r4, [r3, r5]
30009e34:	b005      	add	sp, #20
30009e36:	bd30      	pop	{r4, r5, pc}
30009e38:	4b0b      	ldr	r3, [pc, #44]	; (30009e68 <irq_register+0x74>)
30009e3a:	2107      	movs	r1, #7
30009e3c:	4a0b      	ldr	r2, [pc, #44]	; (30009e6c <irq_register+0x78>)
30009e3e:	2003      	movs	r0, #3
30009e40:	9500      	str	r5, [sp, #0]
30009e42:	e9cd 4101 	strd	r4, r1, [sp, #4]
30009e46:	4619      	mov	r1, r3
30009e48:	f002 f982 	bl	3000c150 <rtk_log_write>
30009e4c:	f44f 7168 	mov.w	r1, #928	; 0x3a0
30009e50:	4807      	ldr	r0, [pc, #28]	; (30009e70 <irq_register+0x7c>)
30009e52:	f002 fbc5 	bl	3000c5e0 <__io_assert_failed_veneer>
30009e56:	e7db      	b.n	30009e10 <irq_register+0x1c>
30009e58:	30000184 	.word	0x30000184
30009e5c:	300002c4 	.word	0x300002c4
30009e60:	e000e400 	.word	0xe000e400
30009e64:	e000ed14 	.word	0xe000ed14
30009e68:	3000d9cc 	.word	0x3000d9cc
30009e6c:	3000da00 	.word	0x3000da00
30009e70:	3000da28 	.word	0x3000da28

30009e74 <SYSCFG_BootFromNor>:
30009e74:	4b0e      	ldr	r3, [pc, #56]	; (30009eb0 <SYSCFG_BootFromNor+0x3c>)
30009e76:	b510      	push	{r4, lr}
30009e78:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30009e7c:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009e80:	0392      	lsls	r2, r2, #14
30009e82:	d50a      	bpl.n	30009e9a <SYSCFG_BootFromNor+0x26>
30009e84:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009e88:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30009e8c:	d105      	bne.n	30009e9a <SYSCFG_BootFromNor+0x26>
30009e8e:	4b09      	ldr	r3, [pc, #36]	; (30009eb4 <SYSCFG_BootFromNor+0x40>)
30009e90:	4798      	blx	r3
30009e92:	3800      	subs	r0, #0
30009e94:	bf18      	it	ne
30009e96:	2001      	movne	r0, #1
30009e98:	bd10      	pop	{r4, pc}
30009e9a:	4c07      	ldr	r4, [pc, #28]	; (30009eb8 <SYSCFG_BootFromNor+0x44>)
30009e9c:	47a0      	blx	r4
30009e9e:	2802      	cmp	r0, #2
30009ea0:	d004      	beq.n	30009eac <SYSCFG_BootFromNor+0x38>
30009ea2:	47a0      	blx	r4
30009ea4:	2801      	cmp	r0, #1
30009ea6:	d1f2      	bne.n	30009e8e <SYSCFG_BootFromNor+0x1a>
30009ea8:	2000      	movs	r0, #0
30009eaa:	bd10      	pop	{r4, pc}
30009eac:	2001      	movs	r0, #1
30009eae:	bd10      	pop	{r4, pc}
30009eb0:	42008000 	.word	0x42008000
30009eb4:	0000c0bd 	.word	0x0000c0bd
30009eb8:	0000c1a9 	.word	0x0000c1a9

30009ebc <SYSCFG_RLVersion>:
30009ebc:	4b07      	ldr	r3, [pc, #28]	; (30009edc <SYSCFG_RLVersion+0x20>)
30009ebe:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009ec2:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009ec6:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30009eca:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30009ece:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009ed2:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009ed6:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009eda:	4770      	bx	lr
30009edc:	42008000 	.word	0x42008000

30009ee0 <SBOOT_GetMdType>:
30009ee0:	b470      	push	{r4, r5, r6}
30009ee2:	4c0c      	ldr	r4, [pc, #48]	; (30009f14 <SBOOT_GetMdType+0x34>)
30009ee4:	4e0c      	ldr	r6, [pc, #48]	; (30009f18 <SBOOT_GetMdType+0x38>)
30009ee6:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009eea:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009eee:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009ef2:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009ef6:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009efa:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009efe:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009f02:	6033      	str	r3, [r6, #0]
30009f04:	b913      	cbnz	r3, 30009f0c <SBOOT_GetMdType+0x2c>
30009f06:	bc70      	pop	{r4, r5, r6}
30009f08:	f002 bbd2 	b.w	3000c6b0 <__SBOOT_GetMdType_A_veneer>
30009f0c:	bc70      	pop	{r4, r5, r6}
30009f0e:	f002 bbcb 	b.w	3000c6a8 <__SBOOT_GetMdType_B_veneer>
30009f12:	bf00      	nop
30009f14:	42008000 	.word	0x42008000
30009f18:	3000fea4 	.word	0x3000fea4

30009f1c <CRYPTO_OTPKey_SHA_Init>:
30009f1c:	4a0d      	ldr	r2, [pc, #52]	; (30009f54 <CRYPTO_OTPKey_SHA_Init+0x38>)
30009f1e:	b410      	push	{r4}
30009f20:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009f24:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009f28:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009f2c:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009f30:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009f34:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009f38:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009f3c:	4a06      	ldr	r2, [pc, #24]	; (30009f58 <CRYPTO_OTPKey_SHA_Init+0x3c>)
30009f3e:	6013      	str	r3, [r2, #0]
30009f40:	b91b      	cbnz	r3, 30009f4a <CRYPTO_OTPKey_SHA_Init+0x2e>
30009f42:	f85d 4b04 	ldr.w	r4, [sp], #4
30009f46:	f002 bb8f 	b.w	3000c668 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30009f4a:	f85d 4b04 	ldr.w	r4, [sp], #4
30009f4e:	f002 bb97 	b.w	3000c680 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
30009f52:	bf00      	nop
30009f54:	42008000 	.word	0x42008000
30009f58:	3000fea4 	.word	0x3000fea4

30009f5c <rtl_crypto_hmac_sha2_update>:
30009f5c:	b470      	push	{r4, r5, r6}
30009f5e:	4c0c      	ldr	r4, [pc, #48]	; (30009f90 <rtl_crypto_hmac_sha2_update+0x34>)
30009f60:	4e0c      	ldr	r6, [pc, #48]	; (30009f94 <rtl_crypto_hmac_sha2_update+0x38>)
30009f62:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009f66:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009f6a:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009f6e:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009f72:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009f76:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009f7a:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009f7e:	6033      	str	r3, [r6, #0]
30009f80:	b913      	cbnz	r3, 30009f88 <rtl_crypto_hmac_sha2_update+0x2c>
30009f82:	bc70      	pop	{r4, r5, r6}
30009f84:	f002 bb68 	b.w	3000c658 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009f88:	bc70      	pop	{r4, r5, r6}
30009f8a:	f002 bb5d 	b.w	3000c648 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009f8e:	bf00      	nop
30009f90:	42008000 	.word	0x42008000
30009f94:	3000fea4 	.word	0x3000fea4

30009f98 <rtl_crypto_hmac_sha2_final>:
30009f98:	4a0d      	ldr	r2, [pc, #52]	; (30009fd0 <rtl_crypto_hmac_sha2_final+0x38>)
30009f9a:	b410      	push	{r4}
30009f9c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009fa0:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009fa4:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009fa8:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009fac:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009fb0:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009fb4:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009fb8:	4a06      	ldr	r2, [pc, #24]	; (30009fd4 <rtl_crypto_hmac_sha2_final+0x3c>)
30009fba:	6013      	str	r3, [r2, #0]
30009fbc:	b91b      	cbnz	r3, 30009fc6 <rtl_crypto_hmac_sha2_final+0x2e>
30009fbe:	f85d 4b04 	ldr.w	r4, [sp], #4
30009fc2:	f002 bb61 	b.w	3000c688 <__rtl_crypto_hmac_sha2_final_A_veneer>
30009fc6:	f85d 4b04 	ldr.w	r4, [sp], #4
30009fca:	f002 bb19 	b.w	3000c600 <__rtl_crypto_hmac_sha2_final_B_veneer>
30009fce:	bf00      	nop
30009fd0:	42008000 	.word	0x42008000
30009fd4:	3000fea4 	.word	0x3000fea4

30009fd8 <rtl_crypto_sha2_init>:
30009fd8:	4a0d      	ldr	r2, [pc, #52]	; (3000a010 <rtl_crypto_sha2_init+0x38>)
30009fda:	b410      	push	{r4}
30009fdc:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009fe0:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009fe4:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009fe8:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009fec:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009ff0:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009ff4:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009ff8:	4a06      	ldr	r2, [pc, #24]	; (3000a014 <rtl_crypto_sha2_init+0x3c>)
30009ffa:	6013      	str	r3, [r2, #0]
30009ffc:	b91b      	cbnz	r3, 3000a006 <rtl_crypto_sha2_init+0x2e>
30009ffe:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a002:	f002 bae9 	b.w	3000c5d8 <__rtl_crypto_sha2_init_A_veneer>
3000a006:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a00a:	f002 bb05 	b.w	3000c618 <__rtl_crypto_sha2_init_B_veneer>
3000a00e:	bf00      	nop
3000a010:	42008000 	.word	0x42008000
3000a014:	3000fea4 	.word	0x3000fea4

3000a018 <rtl_crypto_sha2_update>:
3000a018:	b470      	push	{r4, r5, r6}
3000a01a:	4c0c      	ldr	r4, [pc, #48]	; (3000a04c <rtl_crypto_sha2_update+0x34>)
3000a01c:	4e0c      	ldr	r6, [pc, #48]	; (3000a050 <rtl_crypto_sha2_update+0x38>)
3000a01e:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000a022:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
3000a026:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
3000a02a:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000a02e:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000a032:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
3000a036:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000a03a:	6033      	str	r3, [r6, #0]
3000a03c:	b913      	cbnz	r3, 3000a044 <rtl_crypto_sha2_update+0x2c>
3000a03e:	bc70      	pop	{r4, r5, r6}
3000a040:	f002 bb1a 	b.w	3000c678 <__rtl_crypto_sha2_update_A_veneer>
3000a044:	bc70      	pop	{r4, r5, r6}
3000a046:	f002 bac3 	b.w	3000c5d0 <__rtl_crypto_sha2_update_B_veneer>
3000a04a:	bf00      	nop
3000a04c:	42008000 	.word	0x42008000
3000a050:	3000fea4 	.word	0x3000fea4

3000a054 <rtl_crypto_sha2_final>:
3000a054:	4a0d      	ldr	r2, [pc, #52]	; (3000a08c <rtl_crypto_sha2_final+0x38>)
3000a056:	b410      	push	{r4}
3000a058:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
3000a05c:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
3000a060:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
3000a064:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
3000a068:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
3000a06c:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
3000a070:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000a074:	4a06      	ldr	r2, [pc, #24]	; (3000a090 <rtl_crypto_sha2_final+0x3c>)
3000a076:	6013      	str	r3, [r2, #0]
3000a078:	b91b      	cbnz	r3, 3000a082 <rtl_crypto_sha2_final+0x2e>
3000a07a:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a07e:	f002 baa3 	b.w	3000c5c8 <__rtl_crypto_sha2_final_A_veneer>
3000a082:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a086:	f002 bad3 	b.w	3000c630 <__rtl_crypto_sha2_final_B_veneer>
3000a08a:	bf00      	nop
3000a08c:	42008000 	.word	0x42008000
3000a090:	3000fea4 	.word	0x3000fea4

3000a094 <SBOOT_Validate_PubKey>:
3000a094:	b470      	push	{r4, r5, r6}
3000a096:	4c0c      	ldr	r4, [pc, #48]	; (3000a0c8 <SBOOT_Validate_PubKey+0x34>)
3000a098:	4e0c      	ldr	r6, [pc, #48]	; (3000a0cc <SBOOT_Validate_PubKey+0x38>)
3000a09a:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000a09e:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
3000a0a2:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
3000a0a6:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000a0aa:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000a0ae:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
3000a0b2:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000a0b6:	6033      	str	r3, [r6, #0]
3000a0b8:	b913      	cbnz	r3, 3000a0c0 <SBOOT_Validate_PubKey+0x2c>
3000a0ba:	bc70      	pop	{r4, r5, r6}
3000a0bc:	f002 bae8 	b.w	3000c690 <__SBOOT_Validate_PubKey_A_veneer>
3000a0c0:	bc70      	pop	{r4, r5, r6}
3000a0c2:	f002 bafd 	b.w	3000c6c0 <__SBOOT_Validate_PubKey_B_veneer>
3000a0c6:	bf00      	nop
3000a0c8:	42008000 	.word	0x42008000
3000a0cc:	3000fea4 	.word	0x3000fea4

3000a0d0 <SBOOT_Validate_Signature>:
3000a0d0:	b5f0      	push	{r4, r5, r6, r7, lr}
3000a0d2:	4d10      	ldr	r5, [pc, #64]	; (3000a114 <SBOOT_Validate_Signature+0x44>)
3000a0d4:	9f05      	ldr	r7, [sp, #20]
3000a0d6:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000a0da:	f8dd c018 	ldr.w	ip, [sp, #24]
3000a0de:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000a0e2:	f8df e034 	ldr.w	lr, [pc, #52]	; 3000a118 <SBOOT_Validate_Signature+0x48>
3000a0e6:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000a0ea:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000a0ee:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000a0f2:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000a0f6:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000a0fa:	e9cd 7c05 	strd	r7, ip, [sp, #20]
3000a0fe:	f8ce 4000 	str.w	r4, [lr]
3000a102:	b91c      	cbnz	r4, 3000a10c <SBOOT_Validate_Signature+0x3c>
3000a104:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
3000a108:	f002 ba8e 	b.w	3000c628 <__SBOOT_Validate_Signature_A_veneer>
3000a10c:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
3000a110:	f002 ba86 	b.w	3000c620 <__SBOOT_Validate_Signature_B_veneer>
3000a114:	42008000 	.word	0x42008000
3000a118:	3000fea4 	.word	0x3000fea4

3000a11c <SBOOT_Validate_ImgHash>:
3000a11c:	b4f0      	push	{r4, r5, r6, r7}
3000a11e:	4d0c      	ldr	r5, [pc, #48]	; (3000a150 <SBOOT_Validate_ImgHash+0x34>)
3000a120:	4f0c      	ldr	r7, [pc, #48]	; (3000a154 <SBOOT_Validate_ImgHash+0x38>)
3000a122:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000a126:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000a12a:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000a12e:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000a132:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000a136:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000a13a:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000a13e:	603c      	str	r4, [r7, #0]
3000a140:	b914      	cbnz	r4, 3000a148 <SBOOT_Validate_ImgHash+0x2c>
3000a142:	bcf0      	pop	{r4, r5, r6, r7}
3000a144:	f002 ba78 	b.w	3000c638 <__SBOOT_Validate_ImgHash_A_veneer>
3000a148:	bcf0      	pop	{r4, r5, r6, r7}
3000a14a:	f002 ba79 	b.w	3000c640 <__SBOOT_Validate_ImgHash_B_veneer>
3000a14e:	bf00      	nop
3000a150:	42008000 	.word	0x42008000
3000a154:	3000fea4 	.word	0x3000fea4

3000a158 <CRYPTO_SHA_Init>:
3000a158:	4b0b      	ldr	r3, [pc, #44]	; (3000a188 <CRYPTO_SHA_Init+0x30>)
3000a15a:	490c      	ldr	r1, [pc, #48]	; (3000a18c <CRYPTO_SHA_Init+0x34>)
3000a15c:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000a160:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000a164:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
3000a168:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
3000a16c:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000a170:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000a174:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000a178:	6008      	str	r0, [r1, #0]
3000a17a:	b908      	cbnz	r0, 3000a180 <CRYPTO_SHA_Init+0x28>
3000a17c:	f002 ba78 	b.w	3000c670 <__CRYPTO_SHA_Init_A_veneer>
3000a180:	2000      	movs	r0, #0
3000a182:	f002 ba99 	b.w	3000c6b8 <__CRYPTO_SHA_Init_B_veneer>
3000a186:	bf00      	nop
3000a188:	42008000 	.word	0x42008000
3000a18c:	3000fea4 	.word	0x3000fea4

3000a190 <SBOOT_Validate_Algorithm>:
3000a190:	b4f0      	push	{r4, r5, r6, r7}
3000a192:	4d0c      	ldr	r5, [pc, #48]	; (3000a1c4 <SBOOT_Validate_Algorithm+0x34>)
3000a194:	4f0c      	ldr	r7, [pc, #48]	; (3000a1c8 <SBOOT_Validate_Algorithm+0x38>)
3000a196:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000a19a:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000a19e:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000a1a2:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000a1a6:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000a1aa:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000a1ae:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000a1b2:	603c      	str	r4, [r7, #0]
3000a1b4:	b914      	cbnz	r4, 3000a1bc <SBOOT_Validate_Algorithm+0x2c>
3000a1b6:	bcf0      	pop	{r4, r5, r6, r7}
3000a1b8:	f002 ba1e 	b.w	3000c5f8 <__SBOOT_Validate_Algorithm_A_veneer>
3000a1bc:	bcf0      	pop	{r4, r5, r6, r7}
3000a1be:	f002 ba4f 	b.w	3000c660 <__SBOOT_Validate_Algorithm_B_veneer>
3000a1c2:	bf00      	nop
3000a1c4:	42008000 	.word	0x42008000
3000a1c8:	3000fea4 	.word	0x3000fea4

3000a1cc <ddr_init_index>:
3000a1cc:	b570      	push	{r4, r5, r6, lr}
3000a1ce:	4b0f      	ldr	r3, [pc, #60]	; (3000a20c <ddr_init_index+0x40>)
3000a1d0:	4798      	blx	r3
3000a1d2:	4604      	mov	r4, r0
3000a1d4:	4b0e      	ldr	r3, [pc, #56]	; (3000a210 <ddr_init_index+0x44>)
3000a1d6:	4798      	blx	r3
3000a1d8:	2c03      	cmp	r4, #3
3000a1da:	d014      	beq.n	3000a206 <ddr_init_index+0x3a>
3000a1dc:	2c01      	cmp	r4, #1
3000a1de:	d00c      	beq.n	3000a1fa <ddr_init_index+0x2e>
3000a1e0:	4e0c      	ldr	r6, [pc, #48]	; (3000a214 <ddr_init_index+0x48>)
3000a1e2:	4d0d      	ldr	r5, [pc, #52]	; (3000a218 <ddr_init_index+0x4c>)
3000a1e4:	4c0d      	ldr	r4, [pc, #52]	; (3000a21c <ddr_init_index+0x50>)
3000a1e6:	462b      	mov	r3, r5
3000a1e8:	4622      	mov	r2, r4
3000a1ea:	490b      	ldr	r1, [pc, #44]	; (3000a218 <ddr_init_index+0x4c>)
3000a1ec:	2002      	movs	r0, #2
3000a1ee:	f001 ffaf 	bl	3000c150 <rtk_log_write>
3000a1f2:	f242 7010 	movw	r0, #10000	; 0x2710
3000a1f6:	47b0      	blx	r6
3000a1f8:	e7f5      	b.n	3000a1e6 <ddr_init_index+0x1a>
3000a1fa:	2802      	cmp	r0, #2
3000a1fc:	d003      	beq.n	3000a206 <ddr_init_index+0x3a>
3000a1fe:	2803      	cmp	r0, #3
3000a200:	d1ee      	bne.n	3000a1e0 <ddr_init_index+0x14>
3000a202:	2000      	movs	r0, #0
3000a204:	bd70      	pop	{r4, r5, r6, pc}
3000a206:	4620      	mov	r0, r4
3000a208:	bd70      	pop	{r4, r5, r6, pc}
3000a20a:	bf00      	nop
3000a20c:	30008011 	.word	0x30008011
3000a210:	30007e8d 	.word	0x30007e8d
3000a214:	00009be5 	.word	0x00009be5
3000a218:	3000da84 	.word	0x3000da84
3000a21c:	3000da88 	.word	0x3000da88

3000a220 <rxi316_perf_tune>:
3000a220:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a224:	4606      	mov	r6, r0
3000a226:	6800      	ldr	r0, [r0, #0]
3000a228:	b0c1      	sub	sp, #260	; 0x104
3000a22a:	7985      	ldrb	r5, [r0, #6]
3000a22c:	2d00      	cmp	r5, #0
3000a22e:	f000 832c 	beq.w	3000a88a <rxi316_perf_tune+0x66a>
3000a232:	2d01      	cmp	r5, #1
3000a234:	d003      	beq.n	3000a23e <rxi316_perf_tune+0x1e>
3000a236:	2d03      	cmp	r5, #3
3000a238:	bf14      	ite	ne
3000a23a:	2502      	movne	r5, #2
3000a23c:	2500      	moveq	r5, #0
3000a23e:	68f3      	ldr	r3, [r6, #12]
3000a240:	2420      	movs	r4, #32
3000a242:	4684      	mov	ip, r0
3000a244:	6859      	ldr	r1, [r3, #4]
3000a246:	46a3      	mov	fp, r4
3000a248:	46a2      	mov	sl, r4
3000a24a:	46a0      	mov	r8, r4
3000a24c:	784b      	ldrb	r3, [r1, #1]
3000a24e:	46a1      	mov	r9, r4
3000a250:	7bcf      	ldrb	r7, [r1, #15]
3000a252:	9321      	str	r3, [sp, #132]	; 0x84
3000a254:	788b      	ldrb	r3, [r1, #2]
3000a256:	780a      	ldrb	r2, [r1, #0]
3000a258:	9322      	str	r3, [sp, #136]	; 0x88
3000a25a:	78cb      	ldrb	r3, [r1, #3]
3000a25c:	9323      	str	r3, [sp, #140]	; 0x8c
3000a25e:	790b      	ldrb	r3, [r1, #4]
3000a260:	9324      	str	r3, [sp, #144]	; 0x90
3000a262:	794b      	ldrb	r3, [r1, #5]
3000a264:	9325      	str	r3, [sp, #148]	; 0x94
3000a266:	798b      	ldrb	r3, [r1, #6]
3000a268:	9326      	str	r3, [sp, #152]	; 0x98
3000a26a:	79cb      	ldrb	r3, [r1, #7]
3000a26c:	9327      	str	r3, [sp, #156]	; 0x9c
3000a26e:	7a0b      	ldrb	r3, [r1, #8]
3000a270:	9328      	str	r3, [sp, #160]	; 0xa0
3000a272:	7a4b      	ldrb	r3, [r1, #9]
3000a274:	9329      	str	r3, [sp, #164]	; 0xa4
3000a276:	7a8b      	ldrb	r3, [r1, #10]
3000a278:	932a      	str	r3, [sp, #168]	; 0xa8
3000a27a:	7acb      	ldrb	r3, [r1, #11]
3000a27c:	932b      	str	r3, [sp, #172]	; 0xac
3000a27e:	7b0b      	ldrb	r3, [r1, #12]
3000a280:	932c      	str	r3, [sp, #176]	; 0xb0
3000a282:	7b4b      	ldrb	r3, [r1, #13]
3000a284:	932d      	str	r3, [sp, #180]	; 0xb4
3000a286:	7b8b      	ldrb	r3, [r1, #14]
3000a288:	932e      	str	r3, [sp, #184]	; 0xb8
3000a28a:	2300      	movs	r3, #0
3000a28c:	972f      	str	r7, [sp, #188]	; 0xbc
3000a28e:	7c0f      	ldrb	r7, [r1, #16]
3000a290:	941c      	str	r4, [sp, #112]	; 0x70
3000a292:	9730      	str	r7, [sp, #192]	; 0xc0
3000a294:	7c4f      	ldrb	r7, [r1, #17]
3000a296:	9731      	str	r7, [sp, #196]	; 0xc4
3000a298:	7c8f      	ldrb	r7, [r1, #18]
3000a29a:	9732      	str	r7, [sp, #200]	; 0xc8
3000a29c:	7ccf      	ldrb	r7, [r1, #19]
3000a29e:	9733      	str	r7, [sp, #204]	; 0xcc
3000a2a0:	7d0f      	ldrb	r7, [r1, #20]
3000a2a2:	9734      	str	r7, [sp, #208]	; 0xd0
3000a2a4:	7d4f      	ldrb	r7, [r1, #21]
3000a2a6:	9735      	str	r7, [sp, #212]	; 0xd4
3000a2a8:	7d8f      	ldrb	r7, [r1, #22]
3000a2aa:	9736      	str	r7, [sp, #216]	; 0xd8
3000a2ac:	7dcf      	ldrb	r7, [r1, #23]
3000a2ae:	9737      	str	r7, [sp, #220]	; 0xdc
3000a2b0:	7e0f      	ldrb	r7, [r1, #24]
3000a2b2:	9738      	str	r7, [sp, #224]	; 0xe0
3000a2b4:	7e4f      	ldrb	r7, [r1, #25]
3000a2b6:	9739      	str	r7, [sp, #228]	; 0xe4
3000a2b8:	7e8f      	ldrb	r7, [r1, #26]
3000a2ba:	973a      	str	r7, [sp, #232]	; 0xe8
3000a2bc:	7ecf      	ldrb	r7, [r1, #27]
3000a2be:	973b      	str	r7, [sp, #236]	; 0xec
3000a2c0:	7f0f      	ldrb	r7, [r1, #28]
3000a2c2:	973c      	str	r7, [sp, #240]	; 0xf0
3000a2c4:	7f4f      	ldrb	r7, [r1, #29]
3000a2c6:	973d      	str	r7, [sp, #244]	; 0xf4
3000a2c8:	7f8f      	ldrb	r7, [r1, #30]
3000a2ca:	7fc9      	ldrb	r1, [r1, #31]
3000a2cc:	973e      	str	r7, [sp, #248]	; 0xf8
3000a2ce:	4627      	mov	r7, r4
3000a2d0:	913f      	str	r1, [sp, #252]	; 0xfc
3000a2d2:	4619      	mov	r1, r3
3000a2d4:	9408      	str	r4, [sp, #32]
3000a2d6:	941b      	str	r4, [sp, #108]	; 0x6c
3000a2d8:	9403      	str	r4, [sp, #12]
3000a2da:	941a      	str	r4, [sp, #104]	; 0x68
3000a2dc:	9409      	str	r4, [sp, #36]	; 0x24
3000a2de:	9302      	str	r3, [sp, #8]
3000a2e0:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000a2e4:	e9cd 4400 	strd	r4, r4, [sp]
3000a2e8:	e9cd 4406 	strd	r4, r4, [sp, #24]
3000a2ec:	e9cd 4418 	strd	r4, r4, [sp, #96]	; 0x60
3000a2f0:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
3000a2f4:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
3000a2f8:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
3000a2fc:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
3000a300:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000a304:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000a308:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
3000a30c:	b94a      	cbnz	r2, 3000a322 <rxi316_perf_tune+0x102>
3000a30e:	3101      	adds	r1, #1
3000a310:	4699      	mov	r9, r3
3000a312:	3301      	adds	r3, #1
3000a314:	2b20      	cmp	r3, #32
3000a316:	d069      	beq.n	3000a3ec <rxi316_perf_tune+0x1cc>
3000a318:	aa20      	add	r2, sp, #128	; 0x80
3000a31a:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000a31e:	2a00      	cmp	r2, #0
3000a320:	d0f5      	beq.n	3000a30e <rxi316_perf_tune+0xee>
3000a322:	2a01      	cmp	r2, #1
3000a324:	d05d      	beq.n	3000a3e2 <rxi316_perf_tune+0x1c2>
3000a326:	2a02      	cmp	r2, #2
3000a328:	d06e      	beq.n	3000a408 <rxi316_perf_tune+0x1e8>
3000a32a:	2a03      	cmp	r2, #3
3000a32c:	d06f      	beq.n	3000a40e <rxi316_perf_tune+0x1ee>
3000a32e:	2a04      	cmp	r2, #4
3000a330:	d070      	beq.n	3000a414 <rxi316_perf_tune+0x1f4>
3000a332:	2a05      	cmp	r2, #5
3000a334:	d071      	beq.n	3000a41a <rxi316_perf_tune+0x1fa>
3000a336:	2a06      	cmp	r2, #6
3000a338:	d072      	beq.n	3000a420 <rxi316_perf_tune+0x200>
3000a33a:	2a07      	cmp	r2, #7
3000a33c:	f000 82a7 	beq.w	3000a88e <rxi316_perf_tune+0x66e>
3000a340:	2a08      	cmp	r2, #8
3000a342:	f000 8364 	beq.w	3000aa0e <rxi316_perf_tune+0x7ee>
3000a346:	2a09      	cmp	r2, #9
3000a348:	f000 8364 	beq.w	3000aa14 <rxi316_perf_tune+0x7f4>
3000a34c:	2a0a      	cmp	r2, #10
3000a34e:	f000 836c 	beq.w	3000aa2a <rxi316_perf_tune+0x80a>
3000a352:	2a0b      	cmp	r2, #11
3000a354:	f000 8364 	beq.w	3000aa20 <rxi316_perf_tune+0x800>
3000a358:	2a0c      	cmp	r2, #12
3000a35a:	f000 836b 	beq.w	3000aa34 <rxi316_perf_tune+0x814>
3000a35e:	2a0d      	cmp	r2, #13
3000a360:	f000 8377 	beq.w	3000aa52 <rxi316_perf_tune+0x832>
3000a364:	2a0e      	cmp	r2, #14
3000a366:	f000 836f 	beq.w	3000aa48 <rxi316_perf_tune+0x828>
3000a36a:	2a0f      	cmp	r2, #15
3000a36c:	f000 8376 	beq.w	3000aa5c <rxi316_perf_tune+0x83c>
3000a370:	2a10      	cmp	r2, #16
3000a372:	f000 8378 	beq.w	3000aa66 <rxi316_perf_tune+0x846>
3000a376:	2a11      	cmp	r2, #17
3000a378:	f000 8377 	beq.w	3000aa6a <rxi316_perf_tune+0x84a>
3000a37c:	2a12      	cmp	r2, #18
3000a37e:	f000 8376 	beq.w	3000aa6e <rxi316_perf_tune+0x84e>
3000a382:	2a13      	cmp	r2, #19
3000a384:	f000 8377 	beq.w	3000aa76 <rxi316_perf_tune+0x856>
3000a388:	2a14      	cmp	r2, #20
3000a38a:	f000 8372 	beq.w	3000aa72 <rxi316_perf_tune+0x852>
3000a38e:	2a15      	cmp	r2, #21
3000a390:	f000 8379 	beq.w	3000aa86 <rxi316_perf_tune+0x866>
3000a394:	2a16      	cmp	r2, #22
3000a396:	f000 8374 	beq.w	3000aa82 <rxi316_perf_tune+0x862>
3000a39a:	2a17      	cmp	r2, #23
3000a39c:	f000 8383 	beq.w	3000aaa6 <rxi316_perf_tune+0x886>
3000a3a0:	2a18      	cmp	r2, #24
3000a3a2:	f000 837e 	beq.w	3000aaa2 <rxi316_perf_tune+0x882>
3000a3a6:	2a19      	cmp	r2, #25
3000a3a8:	f000 8379 	beq.w	3000aa9e <rxi316_perf_tune+0x87e>
3000a3ac:	2a1a      	cmp	r2, #26
3000a3ae:	f000 8374 	beq.w	3000aa9a <rxi316_perf_tune+0x87a>
3000a3b2:	2a1b      	cmp	r2, #27
3000a3b4:	f000 836f 	beq.w	3000aa96 <rxi316_perf_tune+0x876>
3000a3b8:	2a1c      	cmp	r2, #28
3000a3ba:	f000 836a 	beq.w	3000aa92 <rxi316_perf_tune+0x872>
3000a3be:	2a1d      	cmp	r2, #29
3000a3c0:	f000 8365 	beq.w	3000aa8e <rxi316_perf_tune+0x86e>
3000a3c4:	2a1e      	cmp	r2, #30
3000a3c6:	f000 8360 	beq.w	3000aa8a <rxi316_perf_tune+0x86a>
3000a3ca:	2a1f      	cmp	r2, #31
3000a3cc:	f000 8357 	beq.w	3000aa7e <rxi316_perf_tune+0x85e>
3000a3d0:	2a20      	cmp	r2, #32
3000a3d2:	f000 8352 	beq.w	3000aa7a <rxi316_perf_tune+0x85a>
3000a3d6:	9808      	ldr	r0, [sp, #32]
3000a3d8:	2a21      	cmp	r2, #33	; 0x21
3000a3da:	bf08      	it	eq
3000a3dc:	4618      	moveq	r0, r3
3000a3de:	9008      	str	r0, [sp, #32]
3000a3e0:	e797      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000a3e2:	4698      	mov	r8, r3
3000a3e4:	3301      	adds	r3, #1
3000a3e6:	3101      	adds	r1, #1
3000a3e8:	2b20      	cmp	r3, #32
3000a3ea:	d195      	bne.n	3000a318 <rxi316_perf_tune+0xf8>
3000a3ec:	f89c 2002 	ldrb.w	r2, [ip, #2]
3000a3f0:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000a3f4:	b1ba      	cbz	r2, 3000a426 <rxi316_perf_tune+0x206>
3000a3f6:	2a01      	cmp	r2, #1
3000a3f8:	f000 830f 	beq.w	3000aa1a <rxi316_perf_tune+0x7fa>
3000a3fc:	2a03      	cmp	r2, #3
3000a3fe:	bf0c      	ite	eq
3000a400:	2204      	moveq	r2, #4
3000a402:	2203      	movne	r2, #3
3000a404:	921d      	str	r2, [sp, #116]	; 0x74
3000a406:	e010      	b.n	3000a42a <rxi316_perf_tune+0x20a>
3000a408:	3101      	adds	r1, #1
3000a40a:	461f      	mov	r7, r3
3000a40c:	e781      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000a40e:	3101      	adds	r1, #1
3000a410:	469a      	mov	sl, r3
3000a412:	e77e      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000a414:	3101      	adds	r1, #1
3000a416:	469b      	mov	fp, r3
3000a418:	e77b      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000a41a:	3101      	adds	r1, #1
3000a41c:	9300      	str	r3, [sp, #0]
3000a41e:	e778      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000a420:	3101      	adds	r1, #1
3000a422:	9301      	str	r3, [sp, #4]
3000a424:	e775      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000a426:	2201      	movs	r2, #1
3000a428:	921d      	str	r2, [sp, #116]	; 0x74
3000a42a:	3b02      	subs	r3, #2
3000a42c:	428b      	cmp	r3, r1
3000a42e:	d005      	beq.n	3000a43c <rxi316_perf_tune+0x21c>
3000a430:	4b90      	ldr	r3, [pc, #576]	; (3000a674 <rxi316_perf_tune+0x454>)
3000a432:	2002      	movs	r0, #2
3000a434:	4a90      	ldr	r2, [pc, #576]	; (3000a678 <rxi316_perf_tune+0x458>)
3000a436:	4619      	mov	r1, r3
3000a438:	f001 fe8a 	bl	3000c150 <rtk_log_write>
3000a43c:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a43e:	9a02      	ldr	r2, [sp, #8]
3000a440:	4293      	cmp	r3, r2
3000a442:	d005      	beq.n	3000a450 <rxi316_perf_tune+0x230>
3000a444:	4b8b      	ldr	r3, [pc, #556]	; (3000a674 <rxi316_perf_tune+0x454>)
3000a446:	2002      	movs	r0, #2
3000a448:	4a8c      	ldr	r2, [pc, #560]	; (3000a67c <rxi316_perf_tune+0x45c>)
3000a44a:	4619      	mov	r1, r3
3000a44c:	f001 fe80 	bl	3000c150 <rtk_log_write>
3000a450:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a452:	2b20      	cmp	r3, #32
3000a454:	f000 82d8 	beq.w	3000aa08 <rxi316_perf_tune+0x7e8>
3000a458:	1b5a      	subs	r2, r3, r5
3000a45a:	3a05      	subs	r2, #5
3000a45c:	f002 030f 	and.w	r3, r2, #15
3000a460:	9302      	str	r3, [sp, #8]
3000a462:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a464:	2b20      	cmp	r3, #32
3000a466:	f000 82cd 	beq.w	3000aa04 <rxi316_perf_tune+0x7e4>
3000a46a:	1b5a      	subs	r2, r3, r5
3000a46c:	3a06      	subs	r2, #6
3000a46e:	0112      	lsls	r2, r2, #4
3000a470:	b2d2      	uxtb	r2, r2
3000a472:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a474:	2b20      	cmp	r3, #32
3000a476:	f000 82c1 	beq.w	3000a9fc <rxi316_perf_tune+0x7dc>
3000a47a:	1b5b      	subs	r3, r3, r5
3000a47c:	3b07      	subs	r3, #7
3000a47e:	021b      	lsls	r3, r3, #8
3000a480:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a484:	930b      	str	r3, [sp, #44]	; 0x2c
3000a486:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a488:	2b20      	cmp	r3, #32
3000a48a:	f000 8214 	beq.w	3000a8b6 <rxi316_perf_tune+0x696>
3000a48e:	1b5b      	subs	r3, r3, r5
3000a490:	3b08      	subs	r3, #8
3000a492:	031b      	lsls	r3, r3, #12
3000a494:	b29b      	uxth	r3, r3
3000a496:	930a      	str	r3, [sp, #40]	; 0x28
3000a498:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a49a:	2b20      	cmp	r3, #32
3000a49c:	f000 8212 	beq.w	3000a8c4 <rxi316_perf_tune+0x6a4>
3000a4a0:	1b5b      	subs	r3, r3, r5
3000a4a2:	3b09      	subs	r3, #9
3000a4a4:	041b      	lsls	r3, r3, #16
3000a4a6:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a4aa:	9309      	str	r3, [sp, #36]	; 0x24
3000a4ac:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a4ae:	2b20      	cmp	r3, #32
3000a4b0:	f000 820f 	beq.w	3000a8d2 <rxi316_perf_tune+0x6b2>
3000a4b4:	1b5b      	subs	r3, r3, r5
3000a4b6:	3b0a      	subs	r3, #10
3000a4b8:	051b      	lsls	r3, r3, #20
3000a4ba:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a4be:	930c      	str	r3, [sp, #48]	; 0x30
3000a4c0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a4c2:	2b20      	cmp	r3, #32
3000a4c4:	f000 820c 	beq.w	3000a8e0 <rxi316_perf_tune+0x6c0>
3000a4c8:	1b5b      	subs	r3, r3, r5
3000a4ca:	3b0b      	subs	r3, #11
3000a4cc:	061b      	lsls	r3, r3, #24
3000a4ce:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a4d2:	930d      	str	r3, [sp, #52]	; 0x34
3000a4d4:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a4d6:	2b20      	cmp	r3, #32
3000a4d8:	f000 8209 	beq.w	3000a8ee <rxi316_perf_tune+0x6ce>
3000a4dc:	1b5b      	subs	r3, r3, r5
3000a4de:	3b0c      	subs	r3, #12
3000a4e0:	071b      	lsls	r3, r3, #28
3000a4e2:	930e      	str	r3, [sp, #56]	; 0x38
3000a4e4:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a4e6:	2b20      	cmp	r3, #32
3000a4e8:	f000 8208 	beq.w	3000a8fc <rxi316_perf_tune+0x6dc>
3000a4ec:	1b5b      	subs	r3, r3, r5
3000a4ee:	3b0d      	subs	r3, #13
3000a4f0:	f003 030f 	and.w	r3, r3, #15
3000a4f4:	930f      	str	r3, [sp, #60]	; 0x3c
3000a4f6:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a4f8:	2b20      	cmp	r3, #32
3000a4fa:	f000 8205 	beq.w	3000a908 <rxi316_perf_tune+0x6e8>
3000a4fe:	1b5b      	subs	r3, r3, r5
3000a500:	3b0e      	subs	r3, #14
3000a502:	011b      	lsls	r3, r3, #4
3000a504:	b2db      	uxtb	r3, r3
3000a506:	9310      	str	r3, [sp, #64]	; 0x40
3000a508:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a50a:	2b20      	cmp	r3, #32
3000a50c:	f000 8202 	beq.w	3000a914 <rxi316_perf_tune+0x6f4>
3000a510:	1b5b      	subs	r3, r3, r5
3000a512:	3b0f      	subs	r3, #15
3000a514:	021b      	lsls	r3, r3, #8
3000a516:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a51a:	9311      	str	r3, [sp, #68]	; 0x44
3000a51c:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a51e:	2b20      	cmp	r3, #32
3000a520:	f000 81ff 	beq.w	3000a922 <rxi316_perf_tune+0x702>
3000a524:	1b5b      	subs	r3, r3, r5
3000a526:	031b      	lsls	r3, r3, #12
3000a528:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
3000a52c:	9312      	str	r3, [sp, #72]	; 0x48
3000a52e:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a530:	2b20      	cmp	r3, #32
3000a532:	f000 81fd 	beq.w	3000a930 <rxi316_perf_tune+0x710>
3000a536:	1b5b      	subs	r3, r3, r5
3000a538:	3b11      	subs	r3, #17
3000a53a:	041b      	lsls	r3, r3, #16
3000a53c:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a540:	9313      	str	r3, [sp, #76]	; 0x4c
3000a542:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a544:	2b20      	cmp	r3, #32
3000a546:	f000 81fa 	beq.w	3000a93e <rxi316_perf_tune+0x71e>
3000a54a:	1b5b      	subs	r3, r3, r5
3000a54c:	3b12      	subs	r3, #18
3000a54e:	051b      	lsls	r3, r3, #20
3000a550:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a554:	931f      	str	r3, [sp, #124]	; 0x7c
3000a556:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a558:	2b20      	cmp	r3, #32
3000a55a:	f000 81f7 	beq.w	3000a94c <rxi316_perf_tune+0x72c>
3000a55e:	1b5b      	subs	r3, r3, r5
3000a560:	3b13      	subs	r3, #19
3000a562:	061b      	lsls	r3, r3, #24
3000a564:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a568:	931e      	str	r3, [sp, #120]	; 0x78
3000a56a:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a56c:	2b20      	cmp	r3, #32
3000a56e:	f000 81f4 	beq.w	3000a95a <rxi316_perf_tune+0x73a>
3000a572:	1b5b      	subs	r3, r3, r5
3000a574:	3b14      	subs	r3, #20
3000a576:	071b      	lsls	r3, r3, #28
3000a578:	931d      	str	r3, [sp, #116]	; 0x74
3000a57a:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a57c:	2b20      	cmp	r3, #32
3000a57e:	f000 81f3 	beq.w	3000a968 <rxi316_perf_tune+0x748>
3000a582:	1b5b      	subs	r3, r3, r5
3000a584:	3b15      	subs	r3, #21
3000a586:	f003 030f 	and.w	r3, r3, #15
3000a58a:	9318      	str	r3, [sp, #96]	; 0x60
3000a58c:	9b06      	ldr	r3, [sp, #24]
3000a58e:	2b20      	cmp	r3, #32
3000a590:	f000 81f0 	beq.w	3000a974 <rxi316_perf_tune+0x754>
3000a594:	1b5b      	subs	r3, r3, r5
3000a596:	3b02      	subs	r3, #2
3000a598:	f003 031f 	and.w	r3, r3, #31
3000a59c:	9317      	str	r3, [sp, #92]	; 0x5c
3000a59e:	9b07      	ldr	r3, [sp, #28]
3000a5a0:	2b20      	cmp	r3, #32
3000a5a2:	f000 81ed 	beq.w	3000a980 <rxi316_perf_tune+0x760>
3000a5a6:	1b5b      	subs	r3, r3, r5
3000a5a8:	3b02      	subs	r3, #2
3000a5aa:	015b      	lsls	r3, r3, #5
3000a5ac:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a5b0:	9316      	str	r3, [sp, #88]	; 0x58
3000a5b2:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a5b4:	2b20      	cmp	r3, #32
3000a5b6:	f000 81ea 	beq.w	3000a98e <rxi316_perf_tune+0x76e>
3000a5ba:	1b5b      	subs	r3, r3, r5
3000a5bc:	f1b9 0f20 	cmp.w	r9, #32
3000a5c0:	f1a3 0302 	sub.w	r3, r3, #2
3000a5c4:	ea4f 2383 	mov.w	r3, r3, lsl #10
3000a5c8:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
3000a5cc:	9315      	str	r3, [sp, #84]	; 0x54
3000a5ce:	f000 81e5 	beq.w	3000a99c <rxi316_perf_tune+0x77c>
3000a5d2:	eba9 0105 	sub.w	r1, r9, r5
3000a5d6:	f1b8 0f20 	cmp.w	r8, #32
3000a5da:	f1a1 0102 	sub.w	r1, r1, #2
3000a5de:	f001 030f 	and.w	r3, r1, #15
3000a5e2:	9314      	str	r3, [sp, #80]	; 0x50
3000a5e4:	f000 81e0 	beq.w	3000a9a8 <rxi316_perf_tune+0x788>
3000a5e8:	eba8 0105 	sub.w	r1, r8, r5
3000a5ec:	2f20      	cmp	r7, #32
3000a5ee:	f1a1 0103 	sub.w	r1, r1, #3
3000a5f2:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a5f6:	b2c9      	uxtb	r1, r1
3000a5f8:	f000 81db 	beq.w	3000a9b2 <rxi316_perf_tune+0x792>
3000a5fc:	1b7b      	subs	r3, r7, r5
3000a5fe:	f1ba 0f20 	cmp.w	sl, #32
3000a602:	f1a3 0304 	sub.w	r3, r3, #4
3000a606:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a60a:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a60e:	9307      	str	r3, [sp, #28]
3000a610:	f000 81d6 	beq.w	3000a9c0 <rxi316_perf_tune+0x7a0>
3000a614:	ebaa 0305 	sub.w	r3, sl, r5
3000a618:	f1bb 0f20 	cmp.w	fp, #32
3000a61c:	f1a3 0305 	sub.w	r3, r3, #5
3000a620:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a624:	b29b      	uxth	r3, r3
3000a626:	9306      	str	r3, [sp, #24]
3000a628:	f000 81d1 	beq.w	3000a9ce <rxi316_perf_tune+0x7ae>
3000a62c:	ebab 0c05 	sub.w	ip, fp, r5
3000a630:	9b00      	ldr	r3, [sp, #0]
3000a632:	f1ac 0c06 	sub.w	ip, ip, #6
3000a636:	2b20      	cmp	r3, #32
3000a638:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a63c:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a640:	f000 81cb 	beq.w	3000a9da <rxi316_perf_tune+0x7ba>
3000a644:	eba3 0e05 	sub.w	lr, r3, r5
3000a648:	9b01      	ldr	r3, [sp, #4]
3000a64a:	f1ae 0e07 	sub.w	lr, lr, #7
3000a64e:	2b20      	cmp	r3, #32
3000a650:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a654:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a658:	f000 81c5 	beq.w	3000a9e6 <rxi316_perf_tune+0x7c6>
3000a65c:	1b5f      	subs	r7, r3, r5
3000a65e:	3f08      	subs	r7, #8
3000a660:	063f      	lsls	r7, r7, #24
3000a662:	f007 6370 	and.w	r3, r7, #251658240	; 0xf000000
3000a666:	9301      	str	r3, [sp, #4]
3000a668:	9b03      	ldr	r3, [sp, #12]
3000a66a:	2b20      	cmp	r3, #32
3000a66c:	f000 81c2 	beq.w	3000a9f4 <rxi316_perf_tune+0x7d4>
3000a670:	e006      	b.n	3000a680 <rxi316_perf_tune+0x460>
3000a672:	bf00      	nop
3000a674:	3000da84 	.word	0x3000da84
3000a678:	3000daac 	.word	0x3000daac
3000a67c:	3000daf0 	.word	0x3000daf0
3000a680:	1b5b      	subs	r3, r3, r5
3000a682:	3b09      	subs	r3, #9
3000a684:	071b      	lsls	r3, r3, #28
3000a686:	9300      	str	r3, [sp, #0]
3000a688:	9b04      	ldr	r3, [sp, #16]
3000a68a:	2b20      	cmp	r3, #32
3000a68c:	f000 8110 	beq.w	3000a8b0 <rxi316_perf_tune+0x690>
3000a690:	eba3 0805 	sub.w	r8, r3, r5
3000a694:	f1a8 080a 	sub.w	r8, r8, #10
3000a698:	f008 080f 	and.w	r8, r8, #15
3000a69c:	9b05      	ldr	r3, [sp, #20]
3000a69e:	2b20      	cmp	r3, #32
3000a6a0:	f000 8104 	beq.w	3000a8ac <rxi316_perf_tune+0x68c>
3000a6a4:	1b58      	subs	r0, r3, r5
3000a6a6:	380b      	subs	r0, #11
3000a6a8:	0100      	lsls	r0, r0, #4
3000a6aa:	b2c0      	uxtb	r0, r0
3000a6ac:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a6ae:	2b20      	cmp	r3, #32
3000a6b0:	f000 80f9 	beq.w	3000a8a6 <rxi316_perf_tune+0x686>
3000a6b4:	eba3 0a05 	sub.w	sl, r3, r5
3000a6b8:	f1aa 0a02 	sub.w	sl, sl, #2
3000a6bc:	f00a 0a1f 	and.w	sl, sl, #31
3000a6c0:	9b08      	ldr	r3, [sp, #32]
3000a6c2:	2b20      	cmp	r3, #32
3000a6c4:	f000 80ec 	beq.w	3000a8a0 <rxi316_perf_tune+0x680>
3000a6c8:	eba3 0905 	sub.w	r9, r3, r5
3000a6cc:	f1a9 0902 	sub.w	r9, r9, #2
3000a6d0:	f009 091f 	and.w	r9, r9, #31
3000a6d4:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a6d6:	2b20      	cmp	r3, #32
3000a6d8:	f000 80df 	beq.w	3000a89a <rxi316_perf_tune+0x67a>
3000a6dc:	eba3 0b05 	sub.w	fp, r3, r5
3000a6e0:	f1ab 0b02 	sub.w	fp, fp, #2
3000a6e4:	f00b 0b1f 	and.w	fp, fp, #31
3000a6e8:	2c20      	cmp	r4, #32
3000a6ea:	f000 80d3 	beq.w	3000a894 <rxi316_perf_tune+0x674>
3000a6ee:	1b63      	subs	r3, r4, r5
3000a6f0:	3b02      	subs	r3, #2
3000a6f2:	015b      	lsls	r3, r3, #5
3000a6f4:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a6f8:	9c02      	ldr	r4, [sp, #8]
3000a6fa:	ea40 0008 	orr.w	r0, r0, r8
3000a6fe:	ea43 030b 	orr.w	r3, r3, fp
3000a702:	4314      	orrs	r4, r2
3000a704:	4622      	mov	r2, r4
3000a706:	e9dd 450f 	ldrd	r4, r5, [sp, #60]	; 0x3c
3000a70a:	432c      	orrs	r4, r5
3000a70c:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000a70e:	4329      	orrs	r1, r5
3000a710:	e9dd 5716 	ldrd	r5, r7, [sp, #88]	; 0x58
3000a714:	433d      	orrs	r5, r7
3000a716:	9f0b      	ldr	r7, [sp, #44]	; 0x2c
3000a718:	433a      	orrs	r2, r7
3000a71a:	9f11      	ldr	r7, [sp, #68]	; 0x44
3000a71c:	433c      	orrs	r4, r7
3000a71e:	9f07      	ldr	r7, [sp, #28]
3000a720:	4339      	orrs	r1, r7
3000a722:	9f15      	ldr	r7, [sp, #84]	; 0x54
3000a724:	433d      	orrs	r5, r7
3000a726:	9f0a      	ldr	r7, [sp, #40]	; 0x28
3000a728:	433a      	orrs	r2, r7
3000a72a:	9f12      	ldr	r7, [sp, #72]	; 0x48
3000a72c:	433c      	orrs	r4, r7
3000a72e:	9f06      	ldr	r7, [sp, #24]
3000a730:	4339      	orrs	r1, r7
3000a732:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a734:	433a      	orrs	r2, r7
3000a736:	9f13      	ldr	r7, [sp, #76]	; 0x4c
3000a738:	ea41 010c 	orr.w	r1, r1, ip
3000a73c:	433c      	orrs	r4, r7
3000a73e:	9f0c      	ldr	r7, [sp, #48]	; 0x30
3000a740:	ea41 010e 	orr.w	r1, r1, lr
3000a744:	433a      	orrs	r2, r7
3000a746:	9f1f      	ldr	r7, [sp, #124]	; 0x7c
3000a748:	433c      	orrs	r4, r7
3000a74a:	9f0d      	ldr	r7, [sp, #52]	; 0x34
3000a74c:	433a      	orrs	r2, r7
3000a74e:	9f1e      	ldr	r7, [sp, #120]	; 0x78
3000a750:	433c      	orrs	r4, r7
3000a752:	9f01      	ldr	r7, [sp, #4]
3000a754:	4339      	orrs	r1, r7
3000a756:	9f0e      	ldr	r7, [sp, #56]	; 0x38
3000a758:	433a      	orrs	r2, r7
3000a75a:	9f1d      	ldr	r7, [sp, #116]	; 0x74
3000a75c:	433c      	orrs	r4, r7
3000a75e:	9f00      	ldr	r7, [sp, #0]
3000a760:	4339      	orrs	r1, r7
3000a762:	4fb7      	ldr	r7, [pc, #732]	; (3000aa40 <rxi316_perf_tune+0x820>)
3000a764:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000a768:	f8c7 4404 	str.w	r4, [r7, #1028]	; 0x404
3000a76c:	9a18      	ldr	r2, [sp, #96]	; 0x60
3000a76e:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000a772:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000a776:	f8c7 1410 	str.w	r1, [r7, #1040]	; 0x410
3000a77a:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000a77e:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000a782:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000a786:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000a78a:	68f3      	ldr	r3, [r6, #12]
3000a78c:	689a      	ldr	r2, [r3, #8]
3000a78e:	e9d2 3200 	ldrd	r3, r2, [r2]
3000a792:	f003 0301 	and.w	r3, r3, #1
3000a796:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000a79a:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000a79e:	68f3      	ldr	r3, [r6, #12]
3000a7a0:	689a      	ldr	r2, [r3, #8]
3000a7a2:	7a13      	ldrb	r3, [r2, #8]
3000a7a4:	6951      	ldr	r1, [r2, #20]
3000a7a6:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000a7aa:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000a7ae:	0209      	lsls	r1, r1, #8
3000a7b0:	0412      	lsls	r2, r2, #16
3000a7b2:	b289      	uxth	r1, r1
3000a7b4:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a7b8:	430b      	orrs	r3, r1
3000a7ba:	4313      	orrs	r3, r2
3000a7bc:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000a7c0:	68f3      	ldr	r3, [r6, #12]
3000a7c2:	6899      	ldr	r1, [r3, #8]
3000a7c4:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000a7c8:	7e09      	ldrb	r1, [r1, #24]
3000a7ca:	021b      	lsls	r3, r3, #8
3000a7cc:	0412      	lsls	r2, r2, #16
3000a7ce:	b29b      	uxth	r3, r3
3000a7d0:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a7d4:	4313      	orrs	r3, r2
3000a7d6:	430b      	orrs	r3, r1
3000a7d8:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000a7dc:	68f3      	ldr	r3, [r6, #12]
3000a7de:	689a      	ldr	r2, [r3, #8]
3000a7e0:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000a7e4:	041b      	lsls	r3, r3, #16
3000a7e6:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000a7ea:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a7ee:	4313      	orrs	r3, r2
3000a7f0:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000a7f4:	68f3      	ldr	r3, [r6, #12]
3000a7f6:	6899      	ldr	r1, [r3, #8]
3000a7f8:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000a7fa:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000a7fc:	005b      	lsls	r3, r3, #1
3000a7fe:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000a800:	0412      	lsls	r2, r2, #16
3000a802:	f003 0302 	and.w	r3, r3, #2
3000a806:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a80a:	4313      	orrs	r3, r2
3000a80c:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000a80e:	f000 0101 	and.w	r1, r0, #1
3000a812:	0212      	lsls	r2, r2, #8
3000a814:	430b      	orrs	r3, r1
3000a816:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000a81a:	4313      	orrs	r3, r2
3000a81c:	4a89      	ldr	r2, [pc, #548]	; (3000aa44 <rxi316_perf_tune+0x824>)
3000a81e:	431a      	orrs	r2, r3
3000a820:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000a824:	68f3      	ldr	r3, [r6, #12]
3000a826:	689b      	ldr	r3, [r3, #8]
3000a828:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000a82a:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000a82e:	68f3      	ldr	r3, [r6, #12]
3000a830:	689b      	ldr	r3, [r3, #8]
3000a832:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000a834:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000a838:	68f3      	ldr	r3, [r6, #12]
3000a83a:	689b      	ldr	r3, [r3, #8]
3000a83c:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000a83e:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000a842:	68f3      	ldr	r3, [r6, #12]
3000a844:	689b      	ldr	r3, [r3, #8]
3000a846:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000a848:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000a84c:	68f3      	ldr	r3, [r6, #12]
3000a84e:	689b      	ldr	r3, [r3, #8]
3000a850:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000a852:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000a856:	68f3      	ldr	r3, [r6, #12]
3000a858:	689b      	ldr	r3, [r3, #8]
3000a85a:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000a85c:	f003 030f 	and.w	r3, r3, #15
3000a860:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000a864:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000a868:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000a86c:	f043 0304 	orr.w	r3, r3, #4
3000a870:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000a874:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000a878:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000a87c:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000a880:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000a884:	b041      	add	sp, #260	; 0x104
3000a886:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a88a:	2502      	movs	r5, #2
3000a88c:	e4d7      	b.n	3000a23e <rxi316_perf_tune+0x1e>
3000a88e:	3101      	adds	r1, #1
3000a890:	9303      	str	r3, [sp, #12]
3000a892:	e53e      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000a894:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a898:	e72e      	b.n	3000a6f8 <rxi316_perf_tune+0x4d8>
3000a89a:	f04f 0b1f 	mov.w	fp, #31
3000a89e:	e723      	b.n	3000a6e8 <rxi316_perf_tune+0x4c8>
3000a8a0:	f04f 091f 	mov.w	r9, #31
3000a8a4:	e716      	b.n	3000a6d4 <rxi316_perf_tune+0x4b4>
3000a8a6:	f04f 0a1f 	mov.w	sl, #31
3000a8aa:	e709      	b.n	3000a6c0 <rxi316_perf_tune+0x4a0>
3000a8ac:	20f0      	movs	r0, #240	; 0xf0
3000a8ae:	e6fd      	b.n	3000a6ac <rxi316_perf_tune+0x48c>
3000a8b0:	f04f 080f 	mov.w	r8, #15
3000a8b4:	e6f2      	b.n	3000a69c <rxi316_perf_tune+0x47c>
3000a8b6:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a8ba:	930a      	str	r3, [sp, #40]	; 0x28
3000a8bc:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a8be:	2b20      	cmp	r3, #32
3000a8c0:	f47f adee 	bne.w	3000a4a0 <rxi316_perf_tune+0x280>
3000a8c4:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a8c8:	9309      	str	r3, [sp, #36]	; 0x24
3000a8ca:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a8cc:	2b20      	cmp	r3, #32
3000a8ce:	f47f adf1 	bne.w	3000a4b4 <rxi316_perf_tune+0x294>
3000a8d2:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a8d6:	930c      	str	r3, [sp, #48]	; 0x30
3000a8d8:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a8da:	2b20      	cmp	r3, #32
3000a8dc:	f47f adf4 	bne.w	3000a4c8 <rxi316_perf_tune+0x2a8>
3000a8e0:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a8e4:	930d      	str	r3, [sp, #52]	; 0x34
3000a8e6:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a8e8:	2b20      	cmp	r3, #32
3000a8ea:	f47f adf7 	bne.w	3000a4dc <rxi316_perf_tune+0x2bc>
3000a8ee:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a8f2:	930e      	str	r3, [sp, #56]	; 0x38
3000a8f4:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a8f6:	2b20      	cmp	r3, #32
3000a8f8:	f47f adf8 	bne.w	3000a4ec <rxi316_perf_tune+0x2cc>
3000a8fc:	230f      	movs	r3, #15
3000a8fe:	930f      	str	r3, [sp, #60]	; 0x3c
3000a900:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a902:	2b20      	cmp	r3, #32
3000a904:	f47f adfb 	bne.w	3000a4fe <rxi316_perf_tune+0x2de>
3000a908:	23f0      	movs	r3, #240	; 0xf0
3000a90a:	9310      	str	r3, [sp, #64]	; 0x40
3000a90c:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a90e:	2b20      	cmp	r3, #32
3000a910:	f47f adfe 	bne.w	3000a510 <rxi316_perf_tune+0x2f0>
3000a914:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a918:	9311      	str	r3, [sp, #68]	; 0x44
3000a91a:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a91c:	2b20      	cmp	r3, #32
3000a91e:	f47f ae01 	bne.w	3000a524 <rxi316_perf_tune+0x304>
3000a922:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a926:	9312      	str	r3, [sp, #72]	; 0x48
3000a928:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a92a:	2b20      	cmp	r3, #32
3000a92c:	f47f ae03 	bne.w	3000a536 <rxi316_perf_tune+0x316>
3000a930:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a934:	9313      	str	r3, [sp, #76]	; 0x4c
3000a936:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a938:	2b20      	cmp	r3, #32
3000a93a:	f47f ae06 	bne.w	3000a54a <rxi316_perf_tune+0x32a>
3000a93e:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a942:	931f      	str	r3, [sp, #124]	; 0x7c
3000a944:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a946:	2b20      	cmp	r3, #32
3000a948:	f47f ae09 	bne.w	3000a55e <rxi316_perf_tune+0x33e>
3000a94c:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a950:	931e      	str	r3, [sp, #120]	; 0x78
3000a952:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a954:	2b20      	cmp	r3, #32
3000a956:	f47f ae0c 	bne.w	3000a572 <rxi316_perf_tune+0x352>
3000a95a:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a95e:	931d      	str	r3, [sp, #116]	; 0x74
3000a960:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a962:	2b20      	cmp	r3, #32
3000a964:	f47f ae0d 	bne.w	3000a582 <rxi316_perf_tune+0x362>
3000a968:	230f      	movs	r3, #15
3000a96a:	9318      	str	r3, [sp, #96]	; 0x60
3000a96c:	9b06      	ldr	r3, [sp, #24]
3000a96e:	2b20      	cmp	r3, #32
3000a970:	f47f ae10 	bne.w	3000a594 <rxi316_perf_tune+0x374>
3000a974:	231f      	movs	r3, #31
3000a976:	9317      	str	r3, [sp, #92]	; 0x5c
3000a978:	9b07      	ldr	r3, [sp, #28]
3000a97a:	2b20      	cmp	r3, #32
3000a97c:	f47f ae13 	bne.w	3000a5a6 <rxi316_perf_tune+0x386>
3000a980:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a984:	9316      	str	r3, [sp, #88]	; 0x58
3000a986:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a988:	2b20      	cmp	r3, #32
3000a98a:	f47f ae16 	bne.w	3000a5ba <rxi316_perf_tune+0x39a>
3000a98e:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
3000a992:	f1b9 0f20 	cmp.w	r9, #32
3000a996:	9315      	str	r3, [sp, #84]	; 0x54
3000a998:	f47f ae1b 	bne.w	3000a5d2 <rxi316_perf_tune+0x3b2>
3000a99c:	230f      	movs	r3, #15
3000a99e:	f1b8 0f20 	cmp.w	r8, #32
3000a9a2:	9314      	str	r3, [sp, #80]	; 0x50
3000a9a4:	f47f ae20 	bne.w	3000a5e8 <rxi316_perf_tune+0x3c8>
3000a9a8:	2f20      	cmp	r7, #32
3000a9aa:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000a9ae:	f47f ae25 	bne.w	3000a5fc <rxi316_perf_tune+0x3dc>
3000a9b2:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a9b6:	f1ba 0f20 	cmp.w	sl, #32
3000a9ba:	9307      	str	r3, [sp, #28]
3000a9bc:	f47f ae2a 	bne.w	3000a614 <rxi316_perf_tune+0x3f4>
3000a9c0:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a9c4:	f1bb 0f20 	cmp.w	fp, #32
3000a9c8:	9306      	str	r3, [sp, #24]
3000a9ca:	f47f ae2f 	bne.w	3000a62c <rxi316_perf_tune+0x40c>
3000a9ce:	9b00      	ldr	r3, [sp, #0]
3000a9d0:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000a9d4:	2b20      	cmp	r3, #32
3000a9d6:	f47f ae35 	bne.w	3000a644 <rxi316_perf_tune+0x424>
3000a9da:	9b01      	ldr	r3, [sp, #4]
3000a9dc:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000a9e0:	2b20      	cmp	r3, #32
3000a9e2:	f47f ae3b 	bne.w	3000a65c <rxi316_perf_tune+0x43c>
3000a9e6:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a9ea:	9301      	str	r3, [sp, #4]
3000a9ec:	9b03      	ldr	r3, [sp, #12]
3000a9ee:	2b20      	cmp	r3, #32
3000a9f0:	f47f ae46 	bne.w	3000a680 <rxi316_perf_tune+0x460>
3000a9f4:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a9f8:	9300      	str	r3, [sp, #0]
3000a9fa:	e645      	b.n	3000a688 <rxi316_perf_tune+0x468>
3000a9fc:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000aa00:	930b      	str	r3, [sp, #44]	; 0x2c
3000aa02:	e540      	b.n	3000a486 <rxi316_perf_tune+0x266>
3000aa04:	22f0      	movs	r2, #240	; 0xf0
3000aa06:	e534      	b.n	3000a472 <rxi316_perf_tune+0x252>
3000aa08:	230f      	movs	r3, #15
3000aa0a:	9302      	str	r3, [sp, #8]
3000aa0c:	e529      	b.n	3000a462 <rxi316_perf_tune+0x242>
3000aa0e:	3101      	adds	r1, #1
3000aa10:	9304      	str	r3, [sp, #16]
3000aa12:	e47e      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa14:	3101      	adds	r1, #1
3000aa16:	9305      	str	r3, [sp, #20]
3000aa18:	e47b      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa1a:	2202      	movs	r2, #2
3000aa1c:	921d      	str	r2, [sp, #116]	; 0x74
3000aa1e:	e504      	b.n	3000a42a <rxi316_perf_tune+0x20a>
3000aa20:	9a02      	ldr	r2, [sp, #8]
3000aa22:	9307      	str	r3, [sp, #28]
3000aa24:	3201      	adds	r2, #1
3000aa26:	9202      	str	r2, [sp, #8]
3000aa28:	e473      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa2a:	9a02      	ldr	r2, [sp, #8]
3000aa2c:	9306      	str	r3, [sp, #24]
3000aa2e:	3201      	adds	r2, #1
3000aa30:	9202      	str	r2, [sp, #8]
3000aa32:	e46e      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa34:	9a02      	ldr	r2, [sp, #8]
3000aa36:	931a      	str	r3, [sp, #104]	; 0x68
3000aa38:	3201      	adds	r2, #1
3000aa3a:	9202      	str	r2, [sp, #8]
3000aa3c:	e469      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa3e:	bf00      	nop
3000aa40:	41100000 	.word	0x41100000
3000aa44:	40000008 	.word	0x40000008
3000aa48:	9a02      	ldr	r2, [sp, #8]
3000aa4a:	461c      	mov	r4, r3
3000aa4c:	3201      	adds	r2, #1
3000aa4e:	9202      	str	r2, [sp, #8]
3000aa50:	e45f      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa52:	9a02      	ldr	r2, [sp, #8]
3000aa54:	931c      	str	r3, [sp, #112]	; 0x70
3000aa56:	3201      	adds	r2, #1
3000aa58:	9202      	str	r2, [sp, #8]
3000aa5a:	e45a      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa5c:	9a02      	ldr	r2, [sp, #8]
3000aa5e:	931b      	str	r3, [sp, #108]	; 0x6c
3000aa60:	3201      	adds	r2, #1
3000aa62:	9202      	str	r2, [sp, #8]
3000aa64:	e455      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa66:	930d      	str	r3, [sp, #52]	; 0x34
3000aa68:	e453      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa6a:	930c      	str	r3, [sp, #48]	; 0x30
3000aa6c:	e451      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa6e:	930b      	str	r3, [sp, #44]	; 0x2c
3000aa70:	e44f      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa72:	9309      	str	r3, [sp, #36]	; 0x24
3000aa74:	e44d      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa76:	930a      	str	r3, [sp, #40]	; 0x28
3000aa78:	e44b      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa7a:	9319      	str	r3, [sp, #100]	; 0x64
3000aa7c:	e449      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa7e:	9318      	str	r3, [sp, #96]	; 0x60
3000aa80:	e447      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa82:	930f      	str	r3, [sp, #60]	; 0x3c
3000aa84:	e445      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa86:	930e      	str	r3, [sp, #56]	; 0x38
3000aa88:	e443      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa8a:	9317      	str	r3, [sp, #92]	; 0x5c
3000aa8c:	e441      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa8e:	9316      	str	r3, [sp, #88]	; 0x58
3000aa90:	e43f      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa92:	9315      	str	r3, [sp, #84]	; 0x54
3000aa94:	e43d      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa96:	9314      	str	r3, [sp, #80]	; 0x50
3000aa98:	e43b      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa9a:	9313      	str	r3, [sp, #76]	; 0x4c
3000aa9c:	e439      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aa9e:	9312      	str	r3, [sp, #72]	; 0x48
3000aaa0:	e437      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aaa2:	9311      	str	r3, [sp, #68]	; 0x44
3000aaa4:	e435      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aaa6:	9310      	str	r3, [sp, #64]	; 0x40
3000aaa8:	e433      	b.n	3000a312 <rxi316_perf_tune+0xf2>
3000aaaa:	bf00      	nop

3000aaac <rxi316_dram_init>:
3000aaac:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000aab0:	4604      	mov	r4, r0
3000aab2:	b08f      	sub	sp, #60	; 0x3c
3000aab4:	4b10      	ldr	r3, [pc, #64]	; (3000aaf8 <rxi316_dram_init+0x4c>)
3000aab6:	4798      	blx	r3
3000aab8:	f894 b014 	ldrb.w	fp, [r4, #20]
3000aabc:	2101      	movs	r1, #1
3000aabe:	900c      	str	r0, [sp, #48]	; 0x30
3000aac0:	fa01 f30b 	lsl.w	r3, r1, fp
3000aac4:	9302      	str	r3, [sp, #8]
3000aac6:	6823      	ldr	r3, [r4, #0]
3000aac8:	78da      	ldrb	r2, [r3, #3]
3000aaca:	6923      	ldr	r3, [r4, #16]
3000aacc:	fa03 f30b 	lsl.w	r3, r3, fp
3000aad0:	b1a2      	cbz	r2, 3000aafc <rxi316_dram_init+0x50>
3000aad2:	428a      	cmp	r2, r1
3000aad4:	f000 847c 	beq.w	3000b3d0 <rxi316_dram_init+0x924>
3000aad8:	2a02      	cmp	r2, #2
3000aada:	bf0b      	itete	eq
3000aadc:	2206      	moveq	r2, #6
3000aade:	220a      	movne	r2, #10
3000aae0:	2540      	moveq	r5, #64	; 0x40
3000aae2:	2580      	movne	r5, #128	; 0x80
3000aae4:	9207      	str	r2, [sp, #28]
3000aae6:	bf0c      	ite	eq
3000aae8:	2204      	moveq	r2, #4
3000aaea:	2208      	movne	r2, #8
3000aaec:	9208      	str	r2, [sp, #32]
3000aaee:	bf0c      	ite	eq
3000aaf0:	2208      	moveq	r2, #8
3000aaf2:	2210      	movne	r2, #16
3000aaf4:	9201      	str	r2, [sp, #4]
3000aaf6:	e007      	b.n	3000ab08 <rxi316_dram_init+0x5c>
3000aaf8:	30008011 	.word	0x30008011
3000aafc:	2203      	movs	r2, #3
3000aafe:	25c0      	movs	r5, #192	; 0xc0
3000ab00:	9108      	str	r1, [sp, #32]
3000ab02:	9207      	str	r2, [sp, #28]
3000ab04:	2202      	movs	r2, #2
3000ab06:	9201      	str	r2, [sp, #4]
3000ab08:	6860      	ldr	r0, [r4, #4]
3000ab0a:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000ab0e:	4ac2      	ldr	r2, [pc, #776]	; (3000ae18 <rxi316_dram_init+0x36c>)
3000ab10:	6806      	ldr	r6, [r0, #0]
3000ab12:	960d      	str	r6, [sp, #52]	; 0x34
3000ab14:	6846      	ldr	r6, [r0, #4]
3000ab16:	6880      	ldr	r0, [r0, #8]
3000ab18:	9609      	str	r6, [sp, #36]	; 0x24
3000ab1a:	9003      	str	r0, [sp, #12]
3000ab1c:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000ab20:	6820      	ldr	r0, [r4, #0]
3000ab22:	7981      	ldrb	r1, [r0, #6]
3000ab24:	6882      	ldr	r2, [r0, #8]
3000ab26:	b129      	cbz	r1, 3000ab34 <rxi316_dram_init+0x88>
3000ab28:	2901      	cmp	r1, #1
3000ab2a:	f000 8449 	beq.w	3000b3c0 <rxi316_dram_init+0x914>
3000ab2e:	2903      	cmp	r1, #3
3000ab30:	f000 8456 	beq.w	3000b3e0 <rxi316_dram_init+0x934>
3000ab34:	3a06      	subs	r2, #6
3000ab36:	7881      	ldrb	r1, [r0, #2]
3000ab38:	f002 020f 	and.w	r2, r2, #15
3000ab3c:	7940      	ldrb	r0, [r0, #5]
3000ab3e:	0109      	lsls	r1, r1, #4
3000ab40:	0280      	lsls	r0, r0, #10
3000ab42:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000ab46:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000ab4a:	4301      	orrs	r1, r0
3000ab4c:	430d      	orrs	r5, r1
3000ab4e:	432a      	orrs	r2, r5
3000ab50:	4db1      	ldr	r5, [pc, #708]	; (3000ae18 <rxi316_dram_init+0x36c>)
3000ab52:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000ab56:	68e2      	ldr	r2, [r4, #12]
3000ab58:	6812      	ldr	r2, [r2, #0]
3000ab5a:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000ab5e:	f002 0207 	and.w	r2, r2, #7
3000ab62:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000ab66:	68e2      	ldr	r2, [r4, #12]
3000ab68:	6811      	ldr	r1, [r2, #0]
3000ab6a:	6822      	ldr	r2, [r4, #0]
3000ab6c:	6848      	ldr	r0, [r1, #4]
3000ab6e:	f892 9000 	ldrb.w	r9, [r2]
3000ab72:	688a      	ldr	r2, [r1, #8]
3000ab74:	9004      	str	r0, [sp, #16]
3000ab76:	0300      	lsls	r0, r0, #12
3000ab78:	0412      	lsls	r2, r2, #16
3000ab7a:	f009 070f 	and.w	r7, r9, #15
3000ab7e:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000ab82:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000ab86:	4302      	orrs	r2, r0
3000ab88:	68c8      	ldr	r0, [r1, #12]
3000ab8a:	0440      	lsls	r0, r0, #17
3000ab8c:	433a      	orrs	r2, r7
3000ab8e:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000ab92:	6908      	ldr	r0, [r1, #16]
3000ab94:	0480      	lsls	r0, r0, #18
3000ab96:	4332      	orrs	r2, r6
3000ab98:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000ab9c:	4310      	orrs	r0, r2
3000ab9e:	694a      	ldr	r2, [r1, #20]
3000aba0:	04d2      	lsls	r2, r2, #19
3000aba2:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000aba6:	4310      	orrs	r0, r2
3000aba8:	698a      	ldr	r2, [r1, #24]
3000abaa:	0552      	lsls	r2, r2, #21
3000abac:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000abb0:	4310      	orrs	r0, r2
3000abb2:	69ca      	ldr	r2, [r1, #28]
3000abb4:	0592      	lsls	r2, r2, #22
3000abb6:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000abba:	4310      	orrs	r0, r2
3000abbc:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000abbe:	0612      	lsls	r2, r2, #24
3000abc0:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000abc4:	4302      	orrs	r2, r0
3000abc6:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000abca:	06c0      	lsls	r0, r0, #27
3000abcc:	0789      	lsls	r1, r1, #30
3000abce:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000abd2:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000abd6:	4310      	orrs	r0, r2
3000abd8:	9a02      	ldr	r2, [sp, #8]
3000abda:	0212      	lsls	r2, r2, #8
3000abdc:	4308      	orrs	r0, r1
3000abde:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000abe2:	4310      	orrs	r0, r2
3000abe4:	6068      	str	r0, [r5, #4]
3000abe6:	6822      	ldr	r2, [r4, #0]
3000abe8:	7992      	ldrb	r2, [r2, #6]
3000abea:	2a00      	cmp	r2, #0
3000abec:	f040 8317 	bne.w	3000b21e <rxi316_dram_init+0x772>
3000abf0:	686a      	ldr	r2, [r5, #4]
3000abf2:	f042 0210 	orr.w	r2, r2, #16
3000abf6:	606a      	str	r2, [r5, #4]
3000abf8:	68e2      	ldr	r2, [r4, #12]
3000abfa:	9802      	ldr	r0, [sp, #8]
3000abfc:	6812      	ldr	r2, [r2, #0]
3000abfe:	f100 3aff 	add.w	sl, r0, #4294967295	; 0xffffffff
3000ac02:	6861      	ldr	r1, [r4, #4]
3000ac04:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000ac06:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000ac0a:	9005      	str	r0, [sp, #20]
3000ac0c:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000ac0e:	fa2e f80b 	lsr.w	r8, lr, fp
3000ac12:	6949      	ldr	r1, [r1, #20]
3000ac14:	ea1e 0f0a 	tst.w	lr, sl
3000ac18:	9006      	str	r0, [sp, #24]
3000ac1a:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000ac1c:	fa21 fe0b 	lsr.w	lr, r1, fp
3000ac20:	bf18      	it	ne
3000ac22:	f108 0801 	addne.w	r8, r8, #1
3000ac26:	ea11 0f0a 	tst.w	r1, sl
3000ac2a:	9906      	ldr	r1, [sp, #24]
3000ac2c:	950a      	str	r5, [sp, #40]	; 0x28
3000ac2e:	bf18      	it	ne
3000ac30:	f10e 0e01 	addne.w	lr, lr, #1
3000ac34:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000ac36:	0049      	lsls	r1, r1, #1
3000ac38:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000ac3c:	950b      	str	r5, [sp, #44]	; 0x2c
3000ac3e:	f001 0102 	and.w	r1, r1, #2
3000ac42:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000ac46:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000ac4a:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000ac4e:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000ac52:	0080      	lsls	r0, r0, #2
3000ac54:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000ac58:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000ac5c:	9006      	str	r0, [sp, #24]
3000ac5e:	013f      	lsls	r7, r7, #4
3000ac60:	9805      	ldr	r0, [sp, #20]
3000ac62:	f00c 0c08 	and.w	ip, ip, #8
3000ac66:	0176      	lsls	r6, r6, #5
3000ac68:	f007 0710 	and.w	r7, r7, #16
3000ac6c:	f000 0001 	and.w	r0, r0, #1
3000ac70:	f006 0620 	and.w	r6, r6, #32
3000ac74:	4605      	mov	r5, r0
3000ac76:	9806      	ldr	r0, [sp, #24]
3000ac78:	f000 0004 	and.w	r0, r0, #4
3000ac7c:	4301      	orrs	r1, r0
3000ac7e:	4329      	orrs	r1, r5
3000ac80:	ea41 0c0c 	orr.w	ip, r1, ip
3000ac84:	990a      	ldr	r1, [sp, #40]	; 0x28
3000ac86:	ea4c 0707 	orr.w	r7, ip, r7
3000ac8a:	018d      	lsls	r5, r1, #6
3000ac8c:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000ac8e:	4337      	orrs	r7, r6
3000ac90:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000ac94:	0208      	lsls	r0, r1, #8
3000ac96:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000ac98:	432f      	orrs	r7, r5
3000ac9a:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000ac9e:	06c9      	lsls	r1, r1, #27
3000aca0:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000aca4:	4307      	orrs	r7, r0
3000aca6:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000acaa:	0736      	lsls	r6, r6, #28
3000acac:	430f      	orrs	r7, r1
3000acae:	0752      	lsls	r2, r2, #29
3000acb0:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000acb4:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000acb8:	433e      	orrs	r6, r7
3000acba:	4316      	orrs	r6, r2
3000acbc:	4a56      	ldr	r2, [pc, #344]	; (3000ae18 <rxi316_dram_init+0x36c>)
3000acbe:	ea46 0608 	orr.w	r6, r6, r8
3000acc2:	ea46 060e 	orr.w	r6, r6, lr
3000acc6:	6096      	str	r6, [r2, #8]
3000acc8:	6862      	ldr	r2, [r4, #4]
3000acca:	6990      	ldr	r0, [r2, #24]
3000accc:	9a01      	ldr	r2, [sp, #4]
3000acce:	2a10      	cmp	r2, #16
3000acd0:	6822      	ldr	r2, [r4, #0]
3000acd2:	7811      	ldrb	r1, [r2, #0]
3000acd4:	7916      	ldrb	r6, [r2, #4]
3000acd6:	f000 833d 	beq.w	3000b354 <rxi316_dram_init+0x8a8>
3000acda:	f1a1 0502 	sub.w	r5, r1, #2
3000acde:	f1a1 0e09 	sub.w	lr, r1, #9
3000ace2:	9f01      	ldr	r7, [sp, #4]
3000ace4:	fab5 f585 	clz	r5, r5
3000ace8:	fabe fe8e 	clz	lr, lr
3000acec:	2f08      	cmp	r7, #8
3000acee:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000acf2:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000acf6:	ea45 020e 	orr.w	r2, r5, lr
3000acfa:	f000 829e 	beq.w	3000b23a <rxi316_dram_init+0x78e>
3000acfe:	9f01      	ldr	r7, [sp, #4]
3000ad00:	2f04      	cmp	r7, #4
3000ad02:	f000 83a5 	beq.w	3000b450 <rxi316_dram_init+0x9a4>
3000ad06:	9f01      	ldr	r7, [sp, #4]
3000ad08:	2f02      	cmp	r7, #2
3000ad0a:	bf08      	it	eq
3000ad0c:	4310      	orreq	r0, r2
3000ad0e:	2a00      	cmp	r2, #0
3000ad10:	f040 8298 	bne.w	3000b244 <rxi316_dram_init+0x798>
3000ad14:	2903      	cmp	r1, #3
3000ad16:	f000 8368 	beq.w	3000b3ea <rxi316_dram_init+0x93e>
3000ad1a:	2500      	movs	r5, #0
3000ad1c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000ad20:	462f      	mov	r7, r5
3000ad22:	433d      	orrs	r5, r7
3000ad24:	4f3c      	ldr	r7, [pc, #240]	; (3000ae18 <rxi316_dram_init+0x36c>)
3000ad26:	430d      	orrs	r5, r1
3000ad28:	4305      	orrs	r5, r0
3000ad2a:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000ad2e:	637e      	str	r6, [r7, #52]	; 0x34
3000ad30:	6862      	ldr	r2, [r4, #4]
3000ad32:	9903      	ldr	r1, [sp, #12]
3000ad34:	69d2      	ldr	r2, [r2, #28]
3000ad36:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000ad3a:	63ba      	str	r2, [r7, #56]	; 0x38
3000ad3c:	6822      	ldr	r2, [r4, #0]
3000ad3e:	7810      	ldrb	r0, [r2, #0]
3000ad40:	6862      	ldr	r2, [r4, #4]
3000ad42:	2803      	cmp	r0, #3
3000ad44:	6a11      	ldr	r1, [r2, #32]
3000ad46:	f000 82f8 	beq.w	3000b33a <rxi316_dram_init+0x88e>
3000ad4a:	2809      	cmp	r0, #9
3000ad4c:	f000 830d 	beq.w	3000b36a <rxi316_dram_init+0x8be>
3000ad50:	4831      	ldr	r0, [pc, #196]	; (3000ae18 <rxi316_dram_init+0x36c>)
3000ad52:	63c1      	str	r1, [r0, #60]	; 0x3c
3000ad54:	6862      	ldr	r2, [r4, #4]
3000ad56:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000ad58:	6402      	str	r2, [r0, #64]	; 0x40
3000ad5a:	6865      	ldr	r5, [r4, #4]
3000ad5c:	68e8      	ldr	r0, [r5, #12]
3000ad5e:	682a      	ldr	r2, [r5, #0]
3000ad60:	6869      	ldr	r1, [r5, #4]
3000ad62:	4402      	add	r2, r0
3000ad64:	68ae      	ldr	r6, [r5, #8]
3000ad66:	4401      	add	r1, r0
3000ad68:	ea1a 0f02 	tst.w	sl, r2
3000ad6c:	f000 82b8 	beq.w	3000b2e0 <rxi316_dram_init+0x834>
3000ad70:	fa22 f20b 	lsr.w	r2, r2, fp
3000ad74:	1c50      	adds	r0, r2, #1
3000ad76:	ea1a 0f01 	tst.w	sl, r1
3000ad7a:	fa21 f50b 	lsr.w	r5, r1, fp
3000ad7e:	6822      	ldr	r2, [r4, #0]
3000ad80:	fa26 f10b 	lsr.w	r1, r6, fp
3000ad84:	bf18      	it	ne
3000ad86:	3501      	addne	r5, #1
3000ad88:	ea1a 0f06 	tst.w	sl, r6
3000ad8c:	7816      	ldrb	r6, [r2, #0]
3000ad8e:	bf18      	it	ne
3000ad90:	3101      	addne	r1, #1
3000ad92:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000ad96:	1eb2      	subs	r2, r6, #2
3000ad98:	2a01      	cmp	r2, #1
3000ad9a:	f240 82a4 	bls.w	3000b2e6 <rxi316_dram_init+0x83a>
3000ad9e:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000ada2:	fbb7 f2f3 	udiv	r2, r7, r3
3000ada6:	fb03 7712 	mls	r7, r3, r2, r7
3000adaa:	b107      	cbz	r7, 3000adae <rxi316_dram_init+0x302>
3000adac:	3201      	adds	r2, #1
3000adae:	2e09      	cmp	r6, #9
3000adb0:	f040 82b3 	bne.w	3000b31a <rxi316_dram_init+0x86e>
3000adb4:	9e08      	ldr	r6, [sp, #32]
3000adb6:	fa26 f60b 	lsr.w	r6, r6, fp
3000adba:	016d      	lsls	r5, r5, #5
3000adbc:	f000 001f 	and.w	r0, r0, #31
3000adc0:	0289      	lsls	r1, r1, #10
3000adc2:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000adc6:	0512      	lsls	r2, r2, #20
3000adc8:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000adcc:	4305      	orrs	r5, r0
3000adce:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000add2:	430d      	orrs	r5, r1
3000add4:	4910      	ldr	r1, [pc, #64]	; (3000ae18 <rxi316_dram_init+0x36c>)
3000add6:	4315      	orrs	r5, r2
3000add8:	0672      	lsls	r2, r6, #25
3000adda:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000adde:	432a      	orrs	r2, r5
3000ade0:	630a      	str	r2, [r1, #48]	; 0x30
3000ade2:	68a0      	ldr	r0, [r4, #8]
3000ade4:	6802      	ldr	r2, [r0, #0]
3000ade6:	fbb2 f1f3 	udiv	r1, r2, r3
3000adea:	fb03 2211 	mls	r2, r3, r1, r2
3000adee:	2a00      	cmp	r2, #0
3000adf0:	f000 8274 	beq.w	3000b2dc <rxi316_dram_init+0x830>
3000adf4:	1c4a      	adds	r2, r1, #1
3000adf6:	9205      	str	r2, [sp, #20]
3000adf8:	6842      	ldr	r2, [r0, #4]
3000adfa:	fbb2 f0f3 	udiv	r0, r2, r3
3000adfe:	fb03 2210 	mls	r2, r3, r0, r2
3000ae02:	2a00      	cmp	r2, #0
3000ae04:	f000 8268 	beq.w	3000b2d8 <rxi316_dram_init+0x82c>
3000ae08:	38c7      	subs	r0, #199	; 0xc7
3000ae0a:	68e2      	ldr	r2, [r4, #12]
3000ae0c:	6811      	ldr	r1, [r2, #0]
3000ae0e:	4a03      	ldr	r2, [pc, #12]	; (3000ae1c <rxi316_dram_init+0x370>)
3000ae10:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000ae14:	e004      	b.n	3000ae20 <rxi316_dram_init+0x374>
3000ae16:	bf00      	nop
3000ae18:	41100000 	.word	0x41100000
3000ae1c:	00ffff00 	.word	0x00ffff00
3000ae20:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000ae24:	900a      	str	r0, [sp, #40]	; 0x28
3000ae26:	0712      	lsls	r2, r2, #28
3000ae28:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000ae2c:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000ae30:	430a      	orrs	r2, r1
3000ae32:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000ae36:	430a      	orrs	r2, r1
3000ae38:	49c3      	ldr	r1, [pc, #780]	; (3000b148 <rxi316_dram_init+0x69c>)
3000ae3a:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000ae3e:	610a      	str	r2, [r1, #16]
3000ae40:	68a2      	ldr	r2, [r4, #8]
3000ae42:	6911      	ldr	r1, [r2, #16]
3000ae44:	fbb1 fcf3 	udiv	ip, r1, r3
3000ae48:	fb03 111c 	mls	r1, r3, ip, r1
3000ae4c:	2900      	cmp	r1, #0
3000ae4e:	f000 8240 	beq.w	3000b2d2 <rxi316_dram_init+0x826>
3000ae52:	f10c 0101 	add.w	r1, ip, #1
3000ae56:	9106      	str	r1, [sp, #24]
3000ae58:	6991      	ldr	r1, [r2, #24]
3000ae5a:	fbb1 f0f3 	udiv	r0, r1, r3
3000ae5e:	fb03 1110 	mls	r1, r3, r0, r1
3000ae62:	b101      	cbz	r1, 3000ae66 <rxi316_dram_init+0x3ba>
3000ae64:	3001      	adds	r0, #1
3000ae66:	68d1      	ldr	r1, [r2, #12]
3000ae68:	fbb1 f5f3 	udiv	r5, r1, r3
3000ae6c:	fb03 1115 	mls	r1, r3, r5, r1
3000ae70:	9503      	str	r5, [sp, #12]
3000ae72:	b109      	cbz	r1, 3000ae78 <rxi316_dram_init+0x3cc>
3000ae74:	3501      	adds	r5, #1
3000ae76:	9503      	str	r5, [sp, #12]
3000ae78:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000ae7a:	fbb5 f1f3 	udiv	r1, r5, r3
3000ae7e:	fb03 5511 	mls	r5, r3, r1, r5
3000ae82:	b105      	cbz	r5, 3000ae86 <rxi316_dram_init+0x3da>
3000ae84:	3101      	adds	r1, #1
3000ae86:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000ae88:	0349      	lsls	r1, r1, #13
3000ae8a:	68e2      	ldr	r2, [r4, #12]
3000ae8c:	fa25 f60b 	lsr.w	r6, r5, fp
3000ae90:	ea15 0f0a 	tst.w	r5, sl
3000ae94:	6812      	ldr	r2, [r2, #0]
3000ae96:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000ae9a:	bf18      	it	ne
3000ae9c:	3601      	addne	r6, #1
3000ae9e:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000aea0:	05d2      	lsls	r2, r2, #23
3000aea2:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000aea6:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000aeaa:	9a06      	ldr	r2, [sp, #24]
3000aeac:	0112      	lsls	r2, r2, #4
3000aeae:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000aeb2:	4332      	orrs	r2, r6
3000aeb4:	0246      	lsls	r6, r0, #9
3000aeb6:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000aeba:	4316      	orrs	r6, r2
3000aebc:	9a03      	ldr	r2, [sp, #12]
3000aebe:	f002 020f 	and.w	r2, r2, #15
3000aec2:	4316      	orrs	r6, r2
3000aec4:	4aa0      	ldr	r2, [pc, #640]	; (3000b148 <rxi316_dram_init+0x69c>)
3000aec6:	4331      	orrs	r1, r6
3000aec8:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000aecc:	6151      	str	r1, [r2, #20]
3000aece:	68a7      	ldr	r7, [r4, #8]
3000aed0:	697a      	ldr	r2, [r7, #20]
3000aed2:	fbb2 fef3 	udiv	lr, r2, r3
3000aed6:	fb03 221e 	mls	r2, r3, lr, r2
3000aeda:	b10a      	cbz	r2, 3000aee0 <rxi316_dram_init+0x434>
3000aedc:	f10e 0e01 	add.w	lr, lr, #1
3000aee0:	6939      	ldr	r1, [r7, #16]
3000aee2:	fbb1 f2f3 	udiv	r2, r1, r3
3000aee6:	fb03 1112 	mls	r1, r3, r2, r1
3000aeea:	b101      	cbz	r1, 3000aeee <rxi316_dram_init+0x442>
3000aeec:	3201      	adds	r2, #1
3000aeee:	68fd      	ldr	r5, [r7, #12]
3000aef0:	fbb5 f1f3 	udiv	r1, r5, r3
3000aef4:	fb03 5511 	mls	r5, r3, r1, r5
3000aef8:	b105      	cbz	r5, 3000aefc <rxi316_dram_init+0x450>
3000aefa:	3101      	adds	r1, #1
3000aefc:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000aefe:	fbb5 f6f3 	udiv	r6, r5, r3
3000af02:	fb03 5516 	mls	r5, r3, r6, r5
3000af06:	b105      	cbz	r5, 3000af0a <rxi316_dram_init+0x45e>
3000af08:	3601      	adds	r6, #1
3000af0a:	68bd      	ldr	r5, [r7, #8]
3000af0c:	4411      	add	r1, r2
3000af0e:	fbb5 f2f3 	udiv	r2, r5, r3
3000af12:	42b1      	cmp	r1, r6
3000af14:	fb03 5512 	mls	r5, r3, r2, r5
3000af18:	bf38      	it	cc
3000af1a:	4631      	movcc	r1, r6
3000af1c:	b905      	cbnz	r5, 3000af20 <rxi316_dram_init+0x474>
3000af1e:	3a01      	subs	r2, #1
3000af20:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000af22:	ea15 0f0a 	tst.w	r5, sl
3000af26:	fa25 fc0b 	lsr.w	ip, r5, fp
3000af2a:	69fd      	ldr	r5, [r7, #28]
3000af2c:	bf18      	it	ne
3000af2e:	f10c 0c01 	addne.w	ip, ip, #1
3000af32:	fa25 f60b 	lsr.w	r6, r5, fp
3000af36:	ea15 0f0a 	tst.w	r5, sl
3000af3a:	bf18      	it	ne
3000af3c:	3601      	addne	r6, #1
3000af3e:	f1b9 0f09 	cmp.w	r9, #9
3000af42:	f000 81f5 	beq.w	3000b330 <rxi316_dram_init+0x884>
3000af46:	f1b9 0f02 	cmp.w	r9, #2
3000af4a:	f000 8233 	beq.w	3000b3b4 <rxi316_dram_init+0x908>
3000af4e:	f1b9 0f03 	cmp.w	r9, #3
3000af52:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000af54:	f000 8298 	beq.w	3000b488 <rxi316_dram_init+0x9dc>
3000af58:	46a8      	mov	r8, r5
3000af5a:	9d01      	ldr	r5, [sp, #4]
3000af5c:	44a8      	add	r8, r5
3000af5e:	fa28 f50b 	lsr.w	r5, r8, fp
3000af62:	ea18 0f0a 	tst.w	r8, sl
3000af66:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000af6a:	bf18      	it	ne
3000af6c:	3501      	addne	r5, #1
3000af6e:	fbb8 f7f3 	udiv	r7, r8, r3
3000af72:	fb03 8817 	mls	r8, r3, r7, r8
3000af76:	f1b8 0f00 	cmp.w	r8, #0
3000af7a:	d000      	beq.n	3000af7e <rxi316_dram_init+0x4d2>
3000af7c:	3701      	adds	r7, #1
3000af7e:	0109      	lsls	r1, r1, #4
3000af80:	f00e 0e0f 	and.w	lr, lr, #15
3000af84:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000af88:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000af8c:	063f      	lsls	r7, r7, #24
3000af8e:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000af92:	ea41 0e0e 	orr.w	lr, r1, lr
3000af96:	0291      	lsls	r1, r2, #10
3000af98:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000af9c:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000afa0:	ea4e 0e01 	orr.w	lr, lr, r1
3000afa4:	0471      	lsls	r1, r6, #17
3000afa6:	ea4e 0c0c 	orr.w	ip, lr, ip
3000afaa:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000afae:	ea4c 0c01 	orr.w	ip, ip, r1
3000afb2:	0529      	lsls	r1, r5, #20
3000afb4:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000afb8:	ea4c 0c01 	orr.w	ip, ip, r1
3000afbc:	4962      	ldr	r1, [pc, #392]	; (3000b148 <rxi316_dram_init+0x69c>)
3000afbe:	ea4c 0707 	orr.w	r7, ip, r7
3000afc2:	618f      	str	r7, [r1, #24]
3000afc4:	6821      	ldr	r1, [r4, #0]
3000afc6:	780f      	ldrb	r7, [r1, #0]
3000afc8:	2f02      	cmp	r7, #2
3000afca:	f000 8152 	beq.w	3000b272 <rxi316_dram_init+0x7c6>
3000afce:	2f09      	cmp	r7, #9
3000afd0:	f000 814f 	beq.w	3000b272 <rxi316_dram_init+0x7c6>
3000afd4:	68e1      	ldr	r1, [r4, #12]
3000afd6:	2f03      	cmp	r7, #3
3000afd8:	6809      	ldr	r1, [r1, #0]
3000afda:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000afdc:	f001 080f 	and.w	r8, r1, #15
3000afe0:	68a1      	ldr	r1, [r4, #8]
3000afe2:	6a09      	ldr	r1, [r1, #32]
3000afe4:	d005      	beq.n	3000aff2 <rxi316_dram_init+0x546>
3000afe6:	ea0a 0701 	and.w	r7, sl, r1
3000afea:	fa21 f10b 	lsr.w	r1, r1, fp
3000afee:	b107      	cbz	r7, 3000aff2 <rxi316_dram_init+0x546>
3000aff0:	3101      	adds	r1, #1
3000aff2:	f1b9 0f02 	cmp.w	r9, #2
3000aff6:	f000 8154 	beq.w	3000b2a2 <rxi316_dram_init+0x7f6>
3000affa:	f1b9 0f03 	cmp.w	r9, #3
3000affe:	f000 81b6 	beq.w	3000b36e <rxi316_dram_init+0x8c2>
3000b002:	f1b9 0f09 	cmp.w	r9, #9
3000b006:	f000 822f 	beq.w	3000b468 <rxi316_dram_init+0x9bc>
3000b00a:	f04f 0e00 	mov.w	lr, #0
3000b00e:	46f1      	mov	r9, lr
3000b010:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b014:	0109      	lsls	r1, r1, #4
3000b016:	9f06      	ldr	r7, [sp, #24]
3000b018:	b2c9      	uxtb	r1, r1
3000b01a:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000b01e:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b020:	ea41 0108 	orr.w	r1, r1, r8
3000b024:	4339      	orrs	r1, r7
3000b026:	4f48      	ldr	r7, [pc, #288]	; (3000b148 <rxi316_dram_init+0x69c>)
3000b028:	ea41 0109 	orr.w	r1, r1, r9
3000b02c:	ea41 010e 	orr.w	r1, r1, lr
3000b030:	61f9      	str	r1, [r7, #28]
3000b032:	68a7      	ldr	r7, [r4, #8]
3000b034:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000b036:	fbb4 f1f3 	udiv	r1, r4, r3
3000b03a:	fb03 4311 	mls	r3, r3, r1, r4
3000b03e:	b103      	cbz	r3, 3000b042 <rxi316_dram_init+0x596>
3000b040:	3101      	adds	r1, #1
3000b042:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000b044:	094b      	lsrs	r3, r1, #5
3000b046:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000b04a:	0912      	lsrs	r2, r2, #4
3000b04c:	ea14 0f0a 	tst.w	r4, sl
3000b050:	fa24 f80b 	lsr.w	r8, r4, fp
3000b054:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000b056:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000b05a:	bf18      	it	ne
3000b05c:	f108 0801 	addne.w	r8, r8, #1
3000b060:	9306      	str	r3, [sp, #24]
3000b062:	ea14 0f0a 	tst.w	r4, sl
3000b066:	fa24 fe0b 	lsr.w	lr, r4, fp
3000b06a:	9b01      	ldr	r3, [sp, #4]
3000b06c:	fa29 f40b 	lsr.w	r4, r9, fp
3000b070:	bf18      	it	ne
3000b072:	f10e 0e01 	addne.w	lr, lr, #1
3000b076:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000b078:	ea19 0f0a 	tst.w	r9, sl
3000b07c:	fa23 f90b 	lsr.w	r9, r3, fp
3000b080:	9905      	ldr	r1, [sp, #20]
3000b082:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000b086:	bf18      	it	ne
3000b088:	3401      	addne	r4, #1
3000b08a:	45b9      	cmp	r9, r7
3000b08c:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000b090:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000b094:	f240 80eb 	bls.w	3000b26e <rxi316_dram_init+0x7c2>
3000b098:	eba9 0707 	sub.w	r7, r9, r7
3000b09c:	fa27 f40b 	lsr.w	r4, r7, fp
3000b0a0:	ea17 070a 	ands.w	r7, r7, sl
3000b0a4:	d001      	beq.n	3000b0aa <rxi316_dram_init+0x5fe>
3000b0a6:	3401      	adds	r4, #1
3000b0a8:	2700      	movs	r7, #0
3000b0aa:	9b04      	ldr	r3, [sp, #16]
3000b0ac:	b10b      	cbz	r3, 3000b0b2 <rxi316_dram_init+0x606>
3000b0ae:	073b      	lsls	r3, r7, #28
3000b0b0:	9304      	str	r3, [sp, #16]
3000b0b2:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000b0b6:	9b06      	ldr	r3, [sp, #24]
3000b0b8:	0289      	lsls	r1, r1, #10
3000b0ba:	02c0      	lsls	r0, r0, #11
3000b0bc:	f00c 0c04 	and.w	ip, ip, #4
3000b0c0:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000b0c4:	01d2      	lsls	r2, r2, #7
3000b0c6:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000b0ca:	0236      	lsls	r6, r6, #8
3000b0cc:	ea4c 0c01 	orr.w	ip, ip, r1
3000b0d0:	b2d2      	uxtb	r2, r2
3000b0d2:	026d      	lsls	r5, r5, #9
3000b0d4:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000b0d8:	ea4c 0c00 	orr.w	ip, ip, r0
3000b0dc:	00df      	lsls	r7, r3, #3
3000b0de:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000b0e2:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000b0e6:	ea4c 0202 	orr.w	r2, ip, r2
3000b0ea:	f007 0708 	and.w	r7, r7, #8
3000b0ee:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000b0f2:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000b0f6:	4332      	orrs	r2, r6
3000b0f8:	0524      	lsls	r4, r4, #20
3000b0fa:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000b0fe:	4912      	ldr	r1, [pc, #72]	; (3000b148 <rxi316_dram_init+0x69c>)
3000b100:	ea42 0305 	orr.w	r3, r2, r5
3000b104:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000b108:	9a04      	ldr	r2, [sp, #16]
3000b10a:	433b      	orrs	r3, r7
3000b10c:	ea43 0308 	orr.w	r3, r3, r8
3000b110:	ea43 030e 	orr.w	r3, r3, lr
3000b114:	4323      	orrs	r3, r4
3000b116:	4313      	orrs	r3, r2
3000b118:	9a03      	ldr	r2, [sp, #12]
3000b11a:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000b11e:	620b      	str	r3, [r1, #32]
3000b120:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000b124:	9b02      	ldr	r3, [sp, #8]
3000b126:	2b01      	cmp	r3, #1
3000b128:	d002      	beq.n	3000b130 <rxi316_dram_init+0x684>
3000b12a:	9b08      	ldr	r3, [sp, #32]
3000b12c:	3303      	adds	r3, #3
3000b12e:	9307      	str	r3, [sp, #28]
3000b130:	4b05      	ldr	r3, [pc, #20]	; (3000b148 <rxi316_dram_init+0x69c>)
3000b132:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000b136:	9a07      	ldr	r2, [sp, #28]
3000b138:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000b13c:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000b140:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000b144:	e002      	b.n	3000b14c <rxi316_dram_init+0x6a0>
3000b146:	bf00      	nop
3000b148:	41100000 	.word	0x41100000
3000b14c:	60da      	str	r2, [r3, #12]
3000b14e:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b152:	430a      	orrs	r2, r1
3000b154:	49b3      	ldr	r1, [pc, #716]	; (3000b424 <rxi316_dram_init+0x978>)
3000b156:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b15a:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000b15e:	0096      	lsls	r6, r2, #2
3000b160:	d57f      	bpl.n	3000b262 <rxi316_dram_init+0x7b6>
3000b162:	691a      	ldr	r2, [r3, #16]
3000b164:	4cb0      	ldr	r4, [pc, #704]	; (3000b428 <rxi316_dram_init+0x97c>)
3000b166:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000b16a:	611a      	str	r2, [r3, #16]
3000b16c:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000b170:	601a      	str	r2, [r3, #0]
3000b172:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000b176:	f043 0303 	orr.w	r3, r3, #3
3000b17a:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000b17e:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000b182:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000b184:	2b01      	cmp	r3, #1
3000b186:	4ba9      	ldr	r3, [pc, #676]	; (3000b42c <rxi316_dram_init+0x980>)
3000b188:	bf0c      	ite	eq
3000b18a:	4aa9      	ldreq	r2, [pc, #676]	; (3000b430 <rxi316_dram_init+0x984>)
3000b18c:	4aa9      	ldrne	r2, [pc, #676]	; (3000b434 <rxi316_dram_init+0x988>)
3000b18e:	601a      	str	r2, [r3, #0]
3000b190:	2203      	movs	r2, #3
3000b192:	68e0      	ldr	r0, [r4, #12]
3000b194:	4ba8      	ldr	r3, [pc, #672]	; (3000b438 <rxi316_dram_init+0x98c>)
3000b196:	4003      	ands	r3, r0
3000b198:	60e3      	str	r3, [r4, #12]
3000b19a:	2308      	movs	r3, #8
3000b19c:	62a1      	str	r1, [r4, #40]	; 0x28
3000b19e:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b1a0:	6023      	str	r3, [r4, #0]
3000b1a2:	6823      	ldr	r3, [r4, #0]
3000b1a4:	071d      	lsls	r5, r3, #28
3000b1a6:	d5fc      	bpl.n	3000b1a2 <rxi316_dram_init+0x6f6>
3000b1a8:	4ba4      	ldr	r3, [pc, #656]	; (3000b43c <rxi316_dram_init+0x990>)
3000b1aa:	2001      	movs	r0, #1
3000b1ac:	4798      	blx	r3
3000b1ae:	68e2      	ldr	r2, [r4, #12]
3000b1b0:	4ba1      	ldr	r3, [pc, #644]	; (3000b438 <rxi316_dram_init+0x98c>)
3000b1b2:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000b1b6:	4013      	ands	r3, r2
3000b1b8:	2203      	movs	r2, #3
3000b1ba:	60e3      	str	r3, [r4, #12]
3000b1bc:	2308      	movs	r3, #8
3000b1be:	62a1      	str	r1, [r4, #40]	; 0x28
3000b1c0:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b1c2:	4a99      	ldr	r2, [pc, #612]	; (3000b428 <rxi316_dram_init+0x97c>)
3000b1c4:	6023      	str	r3, [r4, #0]
3000b1c6:	6813      	ldr	r3, [r2, #0]
3000b1c8:	071c      	lsls	r4, r3, #28
3000b1ca:	d5fc      	bpl.n	3000b1c6 <rxi316_dram_init+0x71a>
3000b1cc:	68d0      	ldr	r0, [r2, #12]
3000b1ce:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000b1d2:	4b99      	ldr	r3, [pc, #612]	; (3000b438 <rxi316_dram_init+0x98c>)
3000b1d4:	4003      	ands	r3, r0
3000b1d6:	2008      	movs	r0, #8
3000b1d8:	60d3      	str	r3, [r2, #12]
3000b1da:	6291      	str	r1, [r2, #40]	; 0x28
3000b1dc:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000b1de:	4992      	ldr	r1, [pc, #584]	; (3000b428 <rxi316_dram_init+0x97c>)
3000b1e0:	f023 0302 	bic.w	r3, r3, #2
3000b1e4:	62d3      	str	r3, [r2, #44]	; 0x2c
3000b1e6:	6010      	str	r0, [r2, #0]
3000b1e8:	680b      	ldr	r3, [r1, #0]
3000b1ea:	0718      	lsls	r0, r3, #28
3000b1ec:	d5fc      	bpl.n	3000b1e8 <rxi316_dram_init+0x73c>
3000b1ee:	4a8e      	ldr	r2, [pc, #568]	; (3000b428 <rxi316_dram_init+0x97c>)
3000b1f0:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000b1f4:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b1f8:	60d3      	str	r3, [r2, #12]
3000b1fa:	980a      	ldr	r0, [sp, #40]	; 0x28
3000b1fc:	6913      	ldr	r3, [r2, #16]
3000b1fe:	4318      	orrs	r0, r3
3000b200:	6110      	str	r0, [r2, #16]
3000b202:	6913      	ldr	r3, [r2, #16]
3000b204:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000b208:	6113      	str	r3, [r2, #16]
3000b20a:	6011      	str	r1, [r2, #0]
3000b20c:	68d3      	ldr	r3, [r2, #12]
3000b20e:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000b212:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b216:	d1f9      	bne.n	3000b20c <rxi316_dram_init+0x760>
3000b218:	b00f      	add	sp, #60	; 0x3c
3000b21a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b21e:	2a01      	cmp	r2, #1
3000b220:	f000 80d1 	beq.w	3000b3c6 <rxi316_dram_init+0x91a>
3000b224:	2a02      	cmp	r2, #2
3000b226:	f000 8139 	beq.w	3000b49c <rxi316_dram_init+0x9f0>
3000b22a:	2a03      	cmp	r2, #3
3000b22c:	f47f ace4 	bne.w	3000abf8 <rxi316_dram_init+0x14c>
3000b230:	686a      	ldr	r2, [r5, #4]
3000b232:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b236:	606a      	str	r2, [r5, #4]
3000b238:	e4de      	b.n	3000abf8 <rxi316_dram_init+0x14c>
3000b23a:	2a00      	cmp	r2, #0
3000b23c:	f43f ad6a 	beq.w	3000ad14 <rxi316_dram_init+0x268>
3000b240:	2203      	movs	r2, #3
3000b242:	4310      	orrs	r0, r2
3000b244:	68a2      	ldr	r2, [r4, #8]
3000b246:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b248:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b24a:	013f      	lsls	r7, r7, #4
3000b24c:	2d00      	cmp	r5, #0
3000b24e:	f000 80d7 	beq.w	3000b400 <rxi316_dram_init+0x954>
3000b252:	1e91      	subs	r1, r2, #2
3000b254:	2906      	cmp	r1, #6
3000b256:	f200 80b0 	bhi.w	3000b3ba <rxi316_dram_init+0x90e>
3000b25a:	1e55      	subs	r5, r2, #1
3000b25c:	2100      	movs	r1, #0
3000b25e:	026d      	lsls	r5, r5, #9
3000b260:	e55f      	b.n	3000ad22 <rxi316_dram_init+0x276>
3000b262:	2201      	movs	r2, #1
3000b264:	601a      	str	r2, [r3, #0]
3000b266:	681a      	ldr	r2, [r3, #0]
3000b268:	07d2      	lsls	r2, r2, #31
3000b26a:	d5fc      	bpl.n	3000b266 <rxi316_dram_init+0x7ba>
3000b26c:	e7bf      	b.n	3000b1ee <rxi316_dram_init+0x742>
3000b26e:	2701      	movs	r7, #1
3000b270:	e71b      	b.n	3000b0aa <rxi316_dram_init+0x5fe>
3000b272:	68a1      	ldr	r1, [r4, #8]
3000b274:	2f03      	cmp	r7, #3
3000b276:	6a09      	ldr	r1, [r1, #32]
3000b278:	f000 80bf 	beq.w	3000b3fa <rxi316_dram_init+0x94e>
3000b27c:	2f02      	cmp	r7, #2
3000b27e:	f040 8118 	bne.w	3000b4b2 <rxi316_dram_init+0xa06>
3000b282:	fbb1 fcf3 	udiv	ip, r1, r3
3000b286:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b28a:	4661      	mov	r1, ip
3000b28c:	f1be 0f00 	cmp.w	lr, #0
3000b290:	f000 80a9 	beq.w	3000b3e6 <rxi316_dram_init+0x93a>
3000b294:	f1b9 0f02 	cmp.w	r9, #2
3000b298:	f101 0101 	add.w	r1, r1, #1
3000b29c:	46b8      	mov	r8, r7
3000b29e:	f47f aeac 	bne.w	3000affa <rxi316_dram_init+0x54e>
3000b2a2:	4f67      	ldr	r7, [pc, #412]	; (3000b440 <rxi316_dram_init+0x994>)
3000b2a4:	f04f 0e00 	mov.w	lr, #0
3000b2a8:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b444 <rxi316_dram_init+0x998>
3000b2ac:	fbb7 f7f3 	udiv	r7, r7, r3
3000b2b0:	fbb9 f9f3 	udiv	r9, r9, r3
3000b2b4:	09ff      	lsrs	r7, r7, #7
3000b2b6:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b2ba:	3701      	adds	r7, #1
3000b2bc:	f109 0c01 	add.w	ip, r9, #1
3000b2c0:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b448 <rxi316_dram_init+0x99c>
3000b2c4:	023f      	lsls	r7, r7, #8
3000b2c6:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b2ca:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b2ce:	9709      	str	r7, [sp, #36]	; 0x24
3000b2d0:	e6a0      	b.n	3000b014 <rxi316_dram_init+0x568>
3000b2d2:	f8cd c018 	str.w	ip, [sp, #24]
3000b2d6:	e5bf      	b.n	3000ae58 <rxi316_dram_init+0x3ac>
3000b2d8:	38c8      	subs	r0, #200	; 0xc8
3000b2da:	e596      	b.n	3000ae0a <rxi316_dram_init+0x35e>
3000b2dc:	9105      	str	r1, [sp, #20]
3000b2de:	e58b      	b.n	3000adf8 <rxi316_dram_init+0x34c>
3000b2e0:	fa22 f00b 	lsr.w	r0, r2, fp
3000b2e4:	e547      	b.n	3000ad76 <rxi316_dram_init+0x2ca>
3000b2e6:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000b2ea:	ea1a 0f07 	tst.w	sl, r7
3000b2ee:	fa27 f20b 	lsr.w	r2, r7, fp
3000b2f2:	d000      	beq.n	3000b2f6 <rxi316_dram_init+0x84a>
3000b2f4:	3201      	adds	r2, #1
3000b2f6:	2e02      	cmp	r6, #2
3000b2f8:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b2fc:	d10f      	bne.n	3000b31e <rxi316_dram_init+0x872>
3000b2fe:	9e08      	ldr	r6, [sp, #32]
3000b300:	3e02      	subs	r6, #2
3000b302:	fa26 f60b 	lsr.w	r6, r6, fp
3000b306:	4437      	add	r7, r6
3000b308:	fbb7 f6f3 	udiv	r6, r7, r3
3000b30c:	fb03 7716 	mls	r7, r3, r6, r7
3000b310:	2f00      	cmp	r7, #0
3000b312:	f43f ad52 	beq.w	3000adba <rxi316_dram_init+0x30e>
3000b316:	3601      	adds	r6, #1
3000b318:	e54f      	b.n	3000adba <rxi316_dram_init+0x30e>
3000b31a:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b31e:	fbb7 f6f3 	udiv	r6, r7, r3
3000b322:	fb03 7716 	mls	r7, r3, r6, r7
3000b326:	2f00      	cmp	r7, #0
3000b328:	f43f ad47 	beq.w	3000adba <rxi316_dram_init+0x30e>
3000b32c:	3601      	adds	r6, #1
3000b32e:	e544      	b.n	3000adba <rxi316_dram_init+0x30e>
3000b330:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b332:	46a8      	mov	r8, r5
3000b334:	9d08      	ldr	r5, [sp, #32]
3000b336:	44a8      	add	r8, r5
3000b338:	e611      	b.n	3000af5e <rxi316_dram_init+0x4b2>
3000b33a:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b33c:	1f50      	subs	r0, r2, #5
3000b33e:	00c0      	lsls	r0, r0, #3
3000b340:	68e2      	ldr	r2, [r4, #12]
3000b342:	6812      	ldr	r2, [r2, #0]
3000b344:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b346:	fab2 f282 	clz	r2, r2
3000b34a:	0952      	lsrs	r2, r2, #5
3000b34c:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b350:	4301      	orrs	r1, r0
3000b352:	e4fd      	b.n	3000ad50 <rxi316_dram_init+0x2a4>
3000b354:	f1a1 0502 	sub.w	r5, r1, #2
3000b358:	2909      	cmp	r1, #9
3000b35a:	fab5 f585 	clz	r5, r5
3000b35e:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b362:	f000 8099 	beq.w	3000b498 <rxi316_dram_init+0x9ec>
3000b366:	462a      	mov	r2, r5
3000b368:	e4d1      	b.n	3000ad0e <rxi316_dram_init+0x262>
3000b36a:	2000      	movs	r0, #0
3000b36c:	e7e8      	b.n	3000b340 <rxi316_dram_init+0x894>
3000b36e:	f242 7710 	movw	r7, #10000	; 0x2710
3000b372:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b44c <rxi316_dram_init+0x9a0>
3000b376:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b444 <rxi316_dram_init+0x998>
3000b37a:	fbb7 fcf3 	udiv	ip, r7, r3
3000b37e:	9f05      	ldr	r7, [sp, #20]
3000b380:	4467      	add	r7, ip
3000b382:	fbb9 f9f3 	udiv	r9, r9, r3
3000b386:	fbbe fef3 	udiv	lr, lr, r3
3000b38a:	09ff      	lsrs	r7, r7, #7
3000b38c:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b390:	3701      	adds	r7, #1
3000b392:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b396:	f109 0c01 	add.w	ip, r9, #1
3000b39a:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b448 <rxi316_dram_init+0x99c>
3000b39e:	023f      	lsls	r7, r7, #8
3000b3a0:	f10e 0e01 	add.w	lr, lr, #1
3000b3a4:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b3a8:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b3ac:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b3b0:	9709      	str	r7, [sp, #36]	; 0x24
3000b3b2:	e62f      	b.n	3000b014 <rxi316_dram_init+0x568>
3000b3b4:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b3b8:	e5d1      	b.n	3000af5e <rxi316_dram_init+0x4b2>
3000b3ba:	2100      	movs	r1, #0
3000b3bc:	460d      	mov	r5, r1
3000b3be:	e4b0      	b.n	3000ad22 <rxi316_dram_init+0x276>
3000b3c0:	3a07      	subs	r2, #7
3000b3c2:	f7ff bbb8 	b.w	3000ab36 <rxi316_dram_init+0x8a>
3000b3c6:	686a      	ldr	r2, [r5, #4]
3000b3c8:	f042 0220 	orr.w	r2, r2, #32
3000b3cc:	606a      	str	r2, [r5, #4]
3000b3ce:	e413      	b.n	3000abf8 <rxi316_dram_init+0x14c>
3000b3d0:	2204      	movs	r2, #4
3000b3d2:	2102      	movs	r1, #2
3000b3d4:	2500      	movs	r5, #0
3000b3d6:	9201      	str	r2, [sp, #4]
3000b3d8:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b3dc:	f7ff bb94 	b.w	3000ab08 <rxi316_dram_init+0x5c>
3000b3e0:	3a08      	subs	r2, #8
3000b3e2:	f7ff bba8 	b.w	3000ab36 <rxi316_dram_init+0x8a>
3000b3e6:	46b8      	mov	r8, r7
3000b3e8:	e603      	b.n	3000aff2 <rxi316_dram_init+0x546>
3000b3ea:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b3ec:	2a04      	cmp	r2, #4
3000b3ee:	d90c      	bls.n	3000b40a <rxi316_dram_init+0x95e>
3000b3f0:	2a0b      	cmp	r2, #11
3000b3f2:	d86a      	bhi.n	3000b4ca <rxi316_dram_init+0xa1e>
3000b3f4:	1f17      	subs	r7, r2, #4
3000b3f6:	013f      	lsls	r7, r7, #4
3000b3f8:	e008      	b.n	3000b40c <rxi316_dram_init+0x960>
3000b3fa:	f04f 0802 	mov.w	r8, #2
3000b3fe:	e5f8      	b.n	3000aff2 <rxi316_dram_init+0x546>
3000b400:	2903      	cmp	r1, #3
3000b402:	d005      	beq.n	3000b410 <rxi316_dram_init+0x964>
3000b404:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b408:	e48b      	b.n	3000ad22 <rxi316_dram_init+0x276>
3000b40a:	2710      	movs	r7, #16
3000b40c:	68a2      	ldr	r2, [r4, #8]
3000b40e:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b410:	2a04      	cmp	r2, #4
3000b412:	d949      	bls.n	3000b4a8 <rxi316_dram_init+0x9fc>
3000b414:	1f51      	subs	r1, r2, #5
3000b416:	2903      	cmp	r1, #3
3000b418:	d84e      	bhi.n	3000b4b8 <rxi316_dram_init+0xa0c>
3000b41a:	1f15      	subs	r5, r2, #4
3000b41c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b420:	026d      	lsls	r5, r5, #9
3000b422:	e47e      	b.n	3000ad22 <rxi316_dram_init+0x276>
3000b424:	42008000 	.word	0x42008000
3000b428:	41100000 	.word	0x41100000
3000b42c:	41011000 	.word	0x41011000
3000b430:	44000103 	.word	0x44000103
3000b434:	44000021 	.word	0x44000021
3000b438:	fff8fffc 	.word	0xfff8fffc
3000b43c:	00009b2d 	.word	0x00009b2d
3000b440:	00061a80 	.word	0x00061a80
3000b444:	0bebc200 	.word	0x0bebc200
3000b448:	003ff000 	.word	0x003ff000
3000b44c:	1dcd6500 	.word	0x1dcd6500
3000b450:	f1a1 0c02 	sub.w	ip, r1, #2
3000b454:	f1bc 0f01 	cmp.w	ip, #1
3000b458:	d903      	bls.n	3000b462 <rxi316_dram_init+0x9b6>
3000b45a:	f1be 0f00 	cmp.w	lr, #0
3000b45e:	f43f ac56 	beq.w	3000ad0e <rxi316_dram_init+0x262>
3000b462:	f040 0002 	orr.w	r0, r0, #2
3000b466:	e452      	b.n	3000ad0e <rxi316_dram_init+0x262>
3000b468:	4f1d      	ldr	r7, [pc, #116]	; (3000b4e0 <rxi316_dram_init+0xa34>)
3000b46a:	f04f 0e00 	mov.w	lr, #0
3000b46e:	fbb7 f9f3 	udiv	r9, r7, r3
3000b472:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b476:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b47a:	f109 0701 	add.w	r7, r9, #1
3000b47e:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b4e4 <rxi316_dram_init+0xa38>
3000b482:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b486:	e5c5      	b.n	3000b014 <rxi316_dram_init+0x568>
3000b488:	f105 0802 	add.w	r8, r5, #2
3000b48c:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b48e:	eba8 0805 	sub.w	r8, r8, r5
3000b492:	9d08      	ldr	r5, [sp, #32]
3000b494:	44a8      	add	r8, r5
3000b496:	e562      	b.n	3000af5e <rxi316_dram_init+0x4b2>
3000b498:	2204      	movs	r2, #4
3000b49a:	e6d2      	b.n	3000b242 <rxi316_dram_init+0x796>
3000b49c:	686a      	ldr	r2, [r5, #4]
3000b49e:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b4a2:	606a      	str	r2, [r5, #4]
3000b4a4:	f7ff bba8 	b.w	3000abf8 <rxi316_dram_init+0x14c>
3000b4a8:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4ac:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b4b0:	e437      	b.n	3000ad22 <rxi316_dram_init+0x276>
3000b4b2:	f04f 0802 	mov.w	r8, #2
3000b4b6:	e596      	b.n	3000afe6 <rxi316_dram_init+0x53a>
3000b4b8:	f1a2 0109 	sub.w	r1, r2, #9
3000b4bc:	2905      	cmp	r1, #5
3000b4be:	d80b      	bhi.n	3000b4d8 <rxi316_dram_init+0xa2c>
3000b4c0:	0855      	lsrs	r5, r2, #1
3000b4c2:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4c6:	026d      	lsls	r5, r5, #9
3000b4c8:	e42b      	b.n	3000ad22 <rxi316_dram_init+0x276>
3000b4ca:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b4cc:	f040 0004 	orr.w	r0, r0, #4
3000b4d0:	f1a2 070c 	sub.w	r7, r2, #12
3000b4d4:	013f      	lsls	r7, r7, #4
3000b4d6:	e799      	b.n	3000b40c <rxi316_dram_init+0x960>
3000b4d8:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4dc:	2500      	movs	r5, #0
3000b4de:	e420      	b.n	3000ad22 <rxi316_dram_init+0x276>
3000b4e0:	0bebc200 	.word	0x0bebc200
3000b4e4:	003ff000 	.word	0x003ff000

3000b4e8 <rxi316_ftend_init>:
3000b4e8:	2300      	movs	r3, #0
3000b4ea:	b510      	push	{r4, lr}
3000b4ec:	4c0c      	ldr	r4, [pc, #48]	; (3000b520 <rxi316_ftend_init+0x38>)
3000b4ee:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b4f2:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b4f6:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b4fa:	f7fe fe67 	bl	3000a1cc <ddr_init_index>
3000b4fe:	4a09      	ldr	r2, [pc, #36]	; (3000b524 <rxi316_ftend_init+0x3c>)
3000b500:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b504:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b508:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b50c:	68cb      	ldr	r3, [r1, #12]
3000b50e:	689b      	ldr	r3, [r3, #8]
3000b510:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b512:	3301      	adds	r3, #1
3000b514:	b2db      	uxtb	r3, r3
3000b516:	4313      	orrs	r3, r2
3000b518:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b51c:	bd10      	pop	{r4, pc}
3000b51e:	bf00      	nop
3000b520:	41101000 	.word	0x41101000
3000b524:	3000f724 	.word	0x3000f724

3000b528 <rxi316_DynSre_init>:
3000b528:	2901      	cmp	r1, #1
3000b52a:	d013      	beq.n	3000b554 <rxi316_DynSre_init+0x2c>
3000b52c:	4a12      	ldr	r2, [pc, #72]	; (3000b578 <rxi316_DynSre_init+0x50>)
3000b52e:	6893      	ldr	r3, [r2, #8]
3000b530:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b534:	6093      	str	r3, [r2, #8]
3000b536:	4b10      	ldr	r3, [pc, #64]	; (3000b578 <rxi316_DynSre_init+0x50>)
3000b538:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b53c:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b540:	625a      	str	r2, [r3, #36]	; 0x24
3000b542:	629a      	str	r2, [r3, #40]	; 0x28
3000b544:	69da      	ldr	r2, [r3, #28]
3000b546:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b54a:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b54e:	61da      	str	r2, [r3, #28]
3000b550:	6019      	str	r1, [r3, #0]
3000b552:	4770      	bx	lr
3000b554:	4b08      	ldr	r3, [pc, #32]	; (3000b578 <rxi316_DynSre_init+0x50>)
3000b556:	b280      	uxth	r0, r0
3000b558:	4a08      	ldr	r2, [pc, #32]	; (3000b57c <rxi316_DynSre_init+0x54>)
3000b55a:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b55e:	400a      	ands	r2, r1
3000b560:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b564:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b568:	4310      	orrs	r0, r2
3000b56a:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b56e:	689a      	ldr	r2, [r3, #8]
3000b570:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b574:	609a      	str	r2, [r3, #8]
3000b576:	e7de      	b.n	3000b536 <rxi316_DynSre_init+0xe>
3000b578:	41100000 	.word	0x41100000
3000b57c:	ffff0000 	.word	0xffff0000

3000b580 <ddr_init>:
3000b580:	b500      	push	{lr}
3000b582:	b083      	sub	sp, #12
3000b584:	f7fe fe22 	bl	3000a1cc <ddr_init_index>
3000b588:	4b06      	ldr	r3, [pc, #24]	; (3000b5a4 <ddr_init+0x24>)
3000b58a:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b58e:	9001      	str	r0, [sp, #4]
3000b590:	f7fe fe46 	bl	3000a220 <rxi316_perf_tune>
3000b594:	9801      	ldr	r0, [sp, #4]
3000b596:	f7ff fa89 	bl	3000aaac <rxi316_dram_init>
3000b59a:	b003      	add	sp, #12
3000b59c:	f85d eb04 	ldr.w	lr, [sp], #4
3000b5a0:	f7ff bfa2 	b.w	3000b4e8 <rxi316_ftend_init>
3000b5a4:	3000f724 	.word	0x3000f724

3000b5a8 <SWR_Calib_DCore>:
3000b5a8:	4b0f      	ldr	r3, [pc, #60]	; (3000b5e8 <SWR_Calib_DCore+0x40>)
3000b5aa:	f240 3231 	movw	r2, #817	; 0x331
3000b5ae:	490f      	ldr	r1, [pc, #60]	; (3000b5ec <SWR_Calib_DCore+0x44>)
3000b5b0:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000b5b2:	4001      	ands	r1, r0
3000b5b4:	430a      	orrs	r2, r1
3000b5b6:	651a      	str	r2, [r3, #80]	; 0x50
3000b5b8:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000b5ba:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b5be:	641a      	str	r2, [r3, #64]	; 0x40
3000b5c0:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000b5c4:	f042 0201 	orr.w	r2, r2, #1
3000b5c8:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000b5cc:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b5d0:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b5d4:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b5d8:	685a      	ldr	r2, [r3, #4]
3000b5da:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000b5de:	f042 0204 	orr.w	r2, r2, #4
3000b5e2:	605a      	str	r2, [r3, #4]
3000b5e4:	4770      	bx	lr
3000b5e6:	bf00      	nop
3000b5e8:	42008100 	.word	0x42008100
3000b5ec:	fffffccc 	.word	0xfffffccc

3000b5f0 <SWR_Calib_MEM>:
3000b5f0:	4b51      	ldr	r3, [pc, #324]	; (3000b738 <SWR_Calib_MEM+0x148>)
3000b5f2:	f240 3231 	movw	r2, #817	; 0x331
3000b5f6:	4951      	ldr	r1, [pc, #324]	; (3000b73c <SWR_Calib_MEM+0x14c>)
3000b5f8:	f04f 0c00 	mov.w	ip, #0
3000b5fc:	b510      	push	{r4, lr}
3000b5fe:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000b602:	b082      	sub	sp, #8
3000b604:	4001      	ands	r1, r0
3000b606:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b60a:	430a      	orrs	r2, r1
3000b60c:	f10d 0107 	add.w	r1, sp, #7
3000b610:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000b614:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000b618:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b61c:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000b620:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000b624:	f042 0201 	orr.w	r2, r2, #1
3000b628:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000b62c:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000b630:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b634:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000b638:	f88d c007 	strb.w	ip, [sp, #7]
3000b63c:	f7fd fab6 	bl	30008bac <OTP_Read8>
3000b640:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b644:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b648:	2af0      	cmp	r2, #240	; 0xf0
3000b64a:	d156      	bne.n	3000b6fa <SWR_Calib_MEM+0x10a>
3000b64c:	f003 030f 	and.w	r3, r3, #15
3000b650:	2b0f      	cmp	r3, #15
3000b652:	d04e      	beq.n	3000b6f2 <SWR_Calib_MEM+0x102>
3000b654:	2302      	movs	r3, #2
3000b656:	4c3a      	ldr	r4, [pc, #232]	; (3000b740 <SWR_Calib_MEM+0x150>)
3000b658:	f10d 0106 	add.w	r1, sp, #6
3000b65c:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b660:	7023      	strb	r3, [r4, #0]
3000b662:	f7fd faa3 	bl	30008bac <OTP_Read8>
3000b666:	7822      	ldrb	r2, [r4, #0]
3000b668:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b66c:	2a03      	cmp	r2, #3
3000b66e:	d05a      	beq.n	3000b726 <SWR_Calib_MEM+0x136>
3000b670:	2a02      	cmp	r2, #2
3000b672:	bf0c      	ite	eq
3000b674:	f003 030f 	andeq.w	r3, r3, #15
3000b678:	2300      	movne	r3, #0
3000b67a:	492f      	ldr	r1, [pc, #188]	; (3000b738 <SWR_Calib_MEM+0x148>)
3000b67c:	4831      	ldr	r0, [pc, #196]	; (3000b744 <SWR_Calib_MEM+0x154>)
3000b67e:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b682:	f022 020f 	bic.w	r2, r2, #15
3000b686:	4313      	orrs	r3, r2
3000b688:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b68c:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b68e:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000b692:	2b07      	cmp	r3, #7
3000b694:	d03a      	beq.n	3000b70c <SWR_Calib_MEM+0x11c>
3000b696:	2300      	movs	r3, #0
3000b698:	f10d 0107 	add.w	r1, sp, #7
3000b69c:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b6a0:	f88d 3007 	strb.w	r3, [sp, #7]
3000b6a4:	f7fd fa82 	bl	30008bac <OTP_Read8>
3000b6a8:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b6ac:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b6b0:	2af0      	cmp	r2, #240	; 0xf0
3000b6b2:	d124      	bne.n	3000b6fe <SWR_Calib_MEM+0x10e>
3000b6b4:	f003 030f 	and.w	r3, r3, #15
3000b6b8:	2b0f      	cmp	r3, #15
3000b6ba:	d022      	beq.n	3000b702 <SWR_Calib_MEM+0x112>
3000b6bc:	2302      	movs	r3, #2
3000b6be:	4c22      	ldr	r4, [pc, #136]	; (3000b748 <SWR_Calib_MEM+0x158>)
3000b6c0:	f10d 0107 	add.w	r1, sp, #7
3000b6c4:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b6c8:	7023      	strb	r3, [r4, #0]
3000b6ca:	f7fd fa6f 	bl	30008bac <OTP_Read8>
3000b6ce:	7822      	ldrb	r2, [r4, #0]
3000b6d0:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b6d4:	2a03      	cmp	r2, #3
3000b6d6:	d028      	beq.n	3000b72a <SWR_Calib_MEM+0x13a>
3000b6d8:	2a02      	cmp	r2, #2
3000b6da:	d029      	beq.n	3000b730 <SWR_Calib_MEM+0x140>
3000b6dc:	2200      	movs	r2, #0
3000b6de:	4916      	ldr	r1, [pc, #88]	; (3000b738 <SWR_Calib_MEM+0x148>)
3000b6e0:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000b6e4:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b6e8:	4313      	orrs	r3, r2
3000b6ea:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b6ee:	b002      	add	sp, #8
3000b6f0:	bd10      	pop	{r4, pc}
3000b6f2:	4b13      	ldr	r3, [pc, #76]	; (3000b740 <SWR_Calib_MEM+0x150>)
3000b6f4:	2201      	movs	r2, #1
3000b6f6:	701a      	strb	r2, [r3, #0]
3000b6f8:	e7cd      	b.n	3000b696 <SWR_Calib_MEM+0xa6>
3000b6fa:	2303      	movs	r3, #3
3000b6fc:	e7ab      	b.n	3000b656 <SWR_Calib_MEM+0x66>
3000b6fe:	2303      	movs	r3, #3
3000b700:	e7dd      	b.n	3000b6be <SWR_Calib_MEM+0xce>
3000b702:	4b11      	ldr	r3, [pc, #68]	; (3000b748 <SWR_Calib_MEM+0x158>)
3000b704:	2201      	movs	r2, #1
3000b706:	701a      	strb	r2, [r3, #0]
3000b708:	b002      	add	sp, #8
3000b70a:	bd10      	pop	{r4, pc}
3000b70c:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b70e:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000b712:	d1c0      	bne.n	3000b696 <SWR_Calib_MEM+0xa6>
3000b714:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000b718:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000b71c:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000b720:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000b724:	e7b7      	b.n	3000b696 <SWR_Calib_MEM+0xa6>
3000b726:	091b      	lsrs	r3, r3, #4
3000b728:	e7a7      	b.n	3000b67a <SWR_Calib_MEM+0x8a>
3000b72a:	091b      	lsrs	r3, r3, #4
3000b72c:	059a      	lsls	r2, r3, #22
3000b72e:	e7d6      	b.n	3000b6de <SWR_Calib_MEM+0xee>
3000b730:	f003 030f 	and.w	r3, r3, #15
3000b734:	059a      	lsls	r2, r3, #22
3000b736:	e7d2      	b.n	3000b6de <SWR_Calib_MEM+0xee>
3000b738:	42008800 	.word	0x42008800
3000b73c:	fffffccc 	.word	0xfffffccc
3000b740:	3000f8bf 	.word	0x3000f8bf
3000b744:	42008100 	.word	0x42008100
3000b748:	3000f8be 	.word	0x3000f8be

3000b74c <SWR_Calib_AUD>:
3000b74c:	4b48      	ldr	r3, [pc, #288]	; (3000b870 <SWR_Calib_AUD+0x124>)
3000b74e:	f240 3231 	movw	r2, #817	; 0x331
3000b752:	4948      	ldr	r1, [pc, #288]	; (3000b874 <SWR_Calib_AUD+0x128>)
3000b754:	f04f 0c00 	mov.w	ip, #0
3000b758:	b510      	push	{r4, lr}
3000b75a:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000b75e:	b082      	sub	sp, #8
3000b760:	4001      	ands	r1, r0
3000b762:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b766:	430a      	orrs	r2, r1
3000b768:	f10d 0107 	add.w	r1, sp, #7
3000b76c:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000b770:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b774:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b778:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b77c:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000b780:	f042 0201 	orr.w	r2, r2, #1
3000b784:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000b788:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000b78c:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b790:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000b794:	f88d c007 	strb.w	ip, [sp, #7]
3000b798:	f7fd fa08 	bl	30008bac <OTP_Read8>
3000b79c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b7a0:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b7a4:	2af0      	cmp	r2, #240	; 0xf0
3000b7a6:	d150      	bne.n	3000b84a <SWR_Calib_AUD+0xfe>
3000b7a8:	f003 030f 	and.w	r3, r3, #15
3000b7ac:	2b0f      	cmp	r3, #15
3000b7ae:	d048      	beq.n	3000b842 <SWR_Calib_AUD+0xf6>
3000b7b0:	2302      	movs	r3, #2
3000b7b2:	4c31      	ldr	r4, [pc, #196]	; (3000b878 <SWR_Calib_AUD+0x12c>)
3000b7b4:	f10d 0106 	add.w	r1, sp, #6
3000b7b8:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b7bc:	7023      	strb	r3, [r4, #0]
3000b7be:	f7fd f9f5 	bl	30008bac <OTP_Read8>
3000b7c2:	7822      	ldrb	r2, [r4, #0]
3000b7c4:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b7c8:	2a03      	cmp	r2, #3
3000b7ca:	d047      	beq.n	3000b85c <SWR_Calib_AUD+0x110>
3000b7cc:	2a02      	cmp	r2, #2
3000b7ce:	bf0c      	ite	eq
3000b7d0:	f003 030f 	andeq.w	r3, r3, #15
3000b7d4:	2300      	movne	r3, #0
3000b7d6:	4926      	ldr	r1, [pc, #152]	; (3000b870 <SWR_Calib_AUD+0x124>)
3000b7d8:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000b7dc:	f022 020f 	bic.w	r2, r2, #15
3000b7e0:	4313      	orrs	r3, r2
3000b7e2:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b7e6:	2300      	movs	r3, #0
3000b7e8:	f10d 0107 	add.w	r1, sp, #7
3000b7ec:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b7f0:	f88d 3007 	strb.w	r3, [sp, #7]
3000b7f4:	f7fd f9da 	bl	30008bac <OTP_Read8>
3000b7f8:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b7fc:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b800:	2af0      	cmp	r2, #240	; 0xf0
3000b802:	d124      	bne.n	3000b84e <SWR_Calib_AUD+0x102>
3000b804:	f003 030f 	and.w	r3, r3, #15
3000b808:	2b0f      	cmp	r3, #15
3000b80a:	d022      	beq.n	3000b852 <SWR_Calib_AUD+0x106>
3000b80c:	2302      	movs	r3, #2
3000b80e:	4c1b      	ldr	r4, [pc, #108]	; (3000b87c <SWR_Calib_AUD+0x130>)
3000b810:	f10d 0107 	add.w	r1, sp, #7
3000b814:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b818:	7023      	strb	r3, [r4, #0]
3000b81a:	f7fd f9c7 	bl	30008bac <OTP_Read8>
3000b81e:	7822      	ldrb	r2, [r4, #0]
3000b820:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b824:	2a03      	cmp	r2, #3
3000b826:	d01b      	beq.n	3000b860 <SWR_Calib_AUD+0x114>
3000b828:	2a02      	cmp	r2, #2
3000b82a:	d01c      	beq.n	3000b866 <SWR_Calib_AUD+0x11a>
3000b82c:	2200      	movs	r2, #0
3000b82e:	4910      	ldr	r1, [pc, #64]	; (3000b870 <SWR_Calib_AUD+0x124>)
3000b830:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000b834:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b838:	4313      	orrs	r3, r2
3000b83a:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b83e:	b002      	add	sp, #8
3000b840:	bd10      	pop	{r4, pc}
3000b842:	4b0d      	ldr	r3, [pc, #52]	; (3000b878 <SWR_Calib_AUD+0x12c>)
3000b844:	2201      	movs	r2, #1
3000b846:	701a      	strb	r2, [r3, #0]
3000b848:	e7cd      	b.n	3000b7e6 <SWR_Calib_AUD+0x9a>
3000b84a:	2303      	movs	r3, #3
3000b84c:	e7b1      	b.n	3000b7b2 <SWR_Calib_AUD+0x66>
3000b84e:	2303      	movs	r3, #3
3000b850:	e7dd      	b.n	3000b80e <SWR_Calib_AUD+0xc2>
3000b852:	4b0a      	ldr	r3, [pc, #40]	; (3000b87c <SWR_Calib_AUD+0x130>)
3000b854:	2201      	movs	r2, #1
3000b856:	701a      	strb	r2, [r3, #0]
3000b858:	b002      	add	sp, #8
3000b85a:	bd10      	pop	{r4, pc}
3000b85c:	091b      	lsrs	r3, r3, #4
3000b85e:	e7ba      	b.n	3000b7d6 <SWR_Calib_AUD+0x8a>
3000b860:	091b      	lsrs	r3, r3, #4
3000b862:	059a      	lsls	r2, r3, #22
3000b864:	e7e3      	b.n	3000b82e <SWR_Calib_AUD+0xe2>
3000b866:	f003 030f 	and.w	r3, r3, #15
3000b86a:	059a      	lsls	r2, r3, #22
3000b86c:	e7df      	b.n	3000b82e <SWR_Calib_AUD+0xe2>
3000b86e:	bf00      	nop
3000b870:	42008800 	.word	0x42008800
3000b874:	fffffccc 	.word	0xfffffccc
3000b878:	3000f8bd 	.word	0x3000f8bd
3000b87c:	3000f8bc 	.word	0x3000f8bc

3000b880 <SWR_MEM>:
3000b880:	2801      	cmp	r0, #1
3000b882:	d00e      	beq.n	3000b8a2 <SWR_MEM+0x22>
3000b884:	4b31      	ldr	r3, [pc, #196]	; (3000b94c <SWR_MEM+0xcc>)
3000b886:	2000      	movs	r0, #0
3000b888:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b88c:	f022 0201 	bic.w	r2, r2, #1
3000b890:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b894:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b898:	f022 0202 	bic.w	r2, r2, #2
3000b89c:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b8a0:	4770      	bx	lr
3000b8a2:	b510      	push	{r4, lr}
3000b8a4:	4c29      	ldr	r4, [pc, #164]	; (3000b94c <SWR_MEM+0xcc>)
3000b8a6:	4a2a      	ldr	r2, [pc, #168]	; (3000b950 <SWR_MEM+0xd0>)
3000b8a8:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b8ac:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b8b0:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b8b4:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b8b8:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b8bc:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b8c0:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b8c4:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b8c8:	f023 0303 	bic.w	r3, r3, #3
3000b8cc:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b8d0:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b8d4:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b8d8:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b8dc:	4790      	blx	r2
3000b8de:	2803      	cmp	r0, #3
3000b8e0:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b8e4:	d026      	beq.n	3000b934 <SWR_MEM+0xb4>
3000b8e6:	f043 0303 	orr.w	r3, r3, #3
3000b8ea:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b8ee:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b8f2:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b8f6:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b8fa:	4c14      	ldr	r4, [pc, #80]	; (3000b94c <SWR_MEM+0xcc>)
3000b8fc:	4b15      	ldr	r3, [pc, #84]	; (3000b954 <SWR_MEM+0xd4>)
3000b8fe:	4798      	blx	r3
3000b900:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b904:	4a14      	ldr	r2, [pc, #80]	; (3000b958 <SWR_MEM+0xd8>)
3000b906:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b90a:	f043 0302 	orr.w	r3, r3, #2
3000b90e:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b912:	4790      	blx	r2
3000b914:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b918:	4b10      	ldr	r3, [pc, #64]	; (3000b95c <SWR_MEM+0xdc>)
3000b91a:	f042 0201 	orr.w	r2, r2, #1
3000b91e:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b922:	e001      	b.n	3000b928 <SWR_MEM+0xa8>
3000b924:	3b01      	subs	r3, #1
3000b926:	d00e      	beq.n	3000b946 <SWR_MEM+0xc6>
3000b928:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b92c:	03d2      	lsls	r2, r2, #15
3000b92e:	d5f9      	bpl.n	3000b924 <SWR_MEM+0xa4>
3000b930:	2000      	movs	r0, #0
3000b932:	bd10      	pop	{r4, pc}
3000b934:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b938:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b93c:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b940:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b944:	e7d9      	b.n	3000b8fa <SWR_MEM+0x7a>
3000b946:	2001      	movs	r0, #1
3000b948:	bd10      	pop	{r4, pc}
3000b94a:	bf00      	nop
3000b94c:	42008800 	.word	0x42008800
3000b950:	30007e41 	.word	0x30007e41
3000b954:	3000b5f1 	.word	0x3000b5f1
3000b958:	00009b2d 	.word	0x00009b2d
3000b95c:	000186a0 	.word	0x000186a0

3000b960 <SWR_MEM_Manual>:
3000b960:	4a06      	ldr	r2, [pc, #24]	; (3000b97c <SWR_MEM_Manual+0x1c>)
3000b962:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b966:	b120      	cbz	r0, 3000b972 <SWR_MEM_Manual+0x12>
3000b968:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b96c:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b970:	4770      	bx	lr
3000b972:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b976:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b97a:	4770      	bx	lr
3000b97c:	42008800 	.word	0x42008800

3000b980 <SWR_MEM_Mode_Set>:
3000b980:	2801      	cmp	r0, #1
3000b982:	b538      	push	{r3, r4, r5, lr}
3000b984:	d807      	bhi.n	3000b996 <SWR_MEM_Mode_Set+0x16>
3000b986:	d10a      	bne.n	3000b99e <SWR_MEM_Mode_Set+0x1e>
3000b988:	4c22      	ldr	r4, [pc, #136]	; (3000ba14 <SWR_MEM_Mode_Set+0x94>)
3000b98a:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b98e:	035b      	lsls	r3, r3, #13
3000b990:	d51b      	bpl.n	3000b9ca <SWR_MEM_Mode_Set+0x4a>
3000b992:	2000      	movs	r0, #0
3000b994:	bd38      	pop	{r3, r4, r5, pc}
3000b996:	217e      	movs	r1, #126	; 0x7e
3000b998:	481f      	ldr	r0, [pc, #124]	; (3000ba18 <SWR_MEM_Mode_Set+0x98>)
3000b99a:	f000 fe21 	bl	3000c5e0 <__io_assert_failed_veneer>
3000b99e:	491d      	ldr	r1, [pc, #116]	; (3000ba14 <SWR_MEM_Mode_Set+0x94>)
3000b9a0:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000b9a4:	0358      	lsls	r0, r3, #13
3000b9a6:	d5f4      	bpl.n	3000b992 <SWR_MEM_Mode_Set+0x12>
3000b9a8:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b9ac:	f242 7310 	movw	r3, #10000	; 0x2710
3000b9b0:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000b9b4:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000b9b8:	e001      	b.n	3000b9be <SWR_MEM_Mode_Set+0x3e>
3000b9ba:	3b01      	subs	r3, #1
3000b9bc:	d027      	beq.n	3000ba0e <SWR_MEM_Mode_Set+0x8e>
3000b9be:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000b9c2:	0352      	lsls	r2, r2, #13
3000b9c4:	d4f9      	bmi.n	3000b9ba <SWR_MEM_Mode_Set+0x3a>
3000b9c6:	2000      	movs	r0, #0
3000b9c8:	e7e4      	b.n	3000b994 <SWR_MEM_Mode_Set+0x14>
3000b9ca:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000b9ce:	2064      	movs	r0, #100	; 0x64
3000b9d0:	4d12      	ldr	r5, [pc, #72]	; (3000ba1c <SWR_MEM_Mode_Set+0x9c>)
3000b9d2:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000b9d6:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000b9da:	47a8      	blx	r5
3000b9dc:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b9e0:	2064      	movs	r0, #100	; 0x64
3000b9e2:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000b9e6:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b9ea:	47a8      	blx	r5
3000b9ec:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000b9f0:	f242 7310 	movw	r3, #10000	; 0x2710
3000b9f4:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000b9f8:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000b9fc:	e001      	b.n	3000ba02 <SWR_MEM_Mode_Set+0x82>
3000b9fe:	3b01      	subs	r3, #1
3000ba00:	d005      	beq.n	3000ba0e <SWR_MEM_Mode_Set+0x8e>
3000ba02:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ba06:	0355      	lsls	r5, r2, #13
3000ba08:	d5f9      	bpl.n	3000b9fe <SWR_MEM_Mode_Set+0x7e>
3000ba0a:	2000      	movs	r0, #0
3000ba0c:	e7c2      	b.n	3000b994 <SWR_MEM_Mode_Set+0x14>
3000ba0e:	2001      	movs	r0, #1
3000ba10:	bd38      	pop	{r3, r4, r5, pc}
3000ba12:	bf00      	nop
3000ba14:	42008800 	.word	0x42008800
3000ba18:	3000db34 	.word	0x3000db34
3000ba1c:	00009b2d 	.word	0x00009b2d

3000ba20 <SWR_AUDIO>:
3000ba20:	2801      	cmp	r0, #1
3000ba22:	d00e      	beq.n	3000ba42 <SWR_AUDIO+0x22>
3000ba24:	4b1e      	ldr	r3, [pc, #120]	; (3000baa0 <SWR_AUDIO+0x80>)
3000ba26:	2000      	movs	r0, #0
3000ba28:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ba2c:	f022 0201 	bic.w	r2, r2, #1
3000ba30:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000ba34:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ba38:	f022 0202 	bic.w	r2, r2, #2
3000ba3c:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000ba40:	4770      	bx	lr
3000ba42:	b510      	push	{r4, lr}
3000ba44:	4c16      	ldr	r4, [pc, #88]	; (3000baa0 <SWR_AUDIO+0x80>)
3000ba46:	4b17      	ldr	r3, [pc, #92]	; (3000baa4 <SWR_AUDIO+0x84>)
3000ba48:	4798      	blx	r3
3000ba4a:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000ba4e:	4a16      	ldr	r2, [pc, #88]	; (3000baa8 <SWR_AUDIO+0x88>)
3000ba50:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000ba54:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000ba58:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000ba5c:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000ba60:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000ba64:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000ba68:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000ba6c:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000ba70:	f043 0302 	orr.w	r3, r3, #2
3000ba74:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000ba78:	4790      	blx	r2
3000ba7a:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000ba7e:	4b0b      	ldr	r3, [pc, #44]	; (3000baac <SWR_AUDIO+0x8c>)
3000ba80:	f042 0201 	orr.w	r2, r2, #1
3000ba84:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000ba88:	e001      	b.n	3000ba8e <SWR_AUDIO+0x6e>
3000ba8a:	3b01      	subs	r3, #1
3000ba8c:	d005      	beq.n	3000ba9a <SWR_AUDIO+0x7a>
3000ba8e:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000ba92:	03d2      	lsls	r2, r2, #15
3000ba94:	d5f9      	bpl.n	3000ba8a <SWR_AUDIO+0x6a>
3000ba96:	2000      	movs	r0, #0
3000ba98:	bd10      	pop	{r4, pc}
3000ba9a:	2001      	movs	r0, #1
3000ba9c:	bd10      	pop	{r4, pc}
3000ba9e:	bf00      	nop
3000baa0:	42008800 	.word	0x42008800
3000baa4:	3000b74d 	.word	0x3000b74d
3000baa8:	00009b2d 	.word	0x00009b2d
3000baac:	000186a0 	.word	0x000186a0

3000bab0 <SWR_AUDIO_Manual>:
3000bab0:	4a06      	ldr	r2, [pc, #24]	; (3000bacc <SWR_AUDIO_Manual+0x1c>)
3000bab2:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000bab6:	b120      	cbz	r0, 3000bac2 <SWR_AUDIO_Manual+0x12>
3000bab8:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000babc:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000bac0:	4770      	bx	lr
3000bac2:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000bac6:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000baca:	4770      	bx	lr
3000bacc:	42008800 	.word	0x42008800

3000bad0 <SWR_BST_MODE_Set>:
3000bad0:	4a06      	ldr	r2, [pc, #24]	; (3000baec <SWR_BST_MODE_Set+0x1c>)
3000bad2:	6813      	ldr	r3, [r2, #0]
3000bad4:	b118      	cbz	r0, 3000bade <SWR_BST_MODE_Set+0xe>
3000bad6:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000bada:	6013      	str	r3, [r2, #0]
3000badc:	4770      	bx	lr
3000bade:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000bae2:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000bae6:	6013      	str	r3, [r2, #0]
3000bae8:	4770      	bx	lr
3000baea:	bf00      	nop
3000baec:	42008100 	.word	0x42008100

3000baf0 <crash_dump_memory>:
3000baf0:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000baf4:	460e      	mov	r6, r1
3000baf6:	4680      	mov	r8, r0
3000baf8:	4b21      	ldr	r3, [pc, #132]	; (3000bb80 <crash_dump_memory+0x90>)
3000bafa:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000bafe:	4a21      	ldr	r2, [pc, #132]	; (3000bb84 <crash_dump_memory+0x94>)
3000bb00:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000bb04:	4644      	mov	r4, r8
3000bb06:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000bb0a:	4b1f      	ldr	r3, [pc, #124]	; (3000bb88 <crash_dump_memory+0x98>)
3000bb0c:	481f      	ldr	r0, [pc, #124]	; (3000bb8c <crash_dump_memory+0x9c>)
3000bb0e:	bf88      	it	hi
3000bb10:	2680      	movhi	r6, #128	; 0x80
3000bb12:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000bb16:	4f1e      	ldr	r7, [pc, #120]	; (3000bb90 <crash_dump_memory+0xa0>)
3000bb18:	bf14      	ite	ne
3000bb1a:	4611      	movne	r1, r2
3000bb1c:	4619      	moveq	r1, r3
3000bb1e:	f000 fdbf 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bb22:	4632      	mov	r2, r6
3000bb24:	4641      	mov	r1, r8
3000bb26:	481b      	ldr	r0, [pc, #108]	; (3000bb94 <crash_dump_memory+0xa4>)
3000bb28:	f000 fdba 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bb2c:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bb9c <crash_dump_memory+0xac>
3000bb30:	2300      	movs	r3, #0
3000bb32:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000bba0 <crash_dump_memory+0xb0>
3000bb36:	e007      	b.n	3000bb48 <crash_dump_memory+0x58>
3000bb38:	6821      	ldr	r1, [r4, #0]
3000bb3a:	4638      	mov	r0, r7
3000bb3c:	f000 fdb0 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bb40:	42ae      	cmp	r6, r5
3000bb42:	d00f      	beq.n	3000bb64 <crash_dump_memory+0x74>
3000bb44:	3404      	adds	r4, #4
3000bb46:	462b      	mov	r3, r5
3000bb48:	1c5d      	adds	r5, r3, #1
3000bb4a:	b183      	cbz	r3, 3000bb6e <crash_dump_memory+0x7e>
3000bb4c:	079b      	lsls	r3, r3, #30
3000bb4e:	d1f3      	bne.n	3000bb38 <crash_dump_memory+0x48>
3000bb50:	4621      	mov	r1, r4
3000bb52:	4648      	mov	r0, r9
3000bb54:	f000 fda4 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bb58:	6821      	ldr	r1, [r4, #0]
3000bb5a:	4638      	mov	r0, r7
3000bb5c:	f000 fda0 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bb60:	42ae      	cmp	r6, r5
3000bb62:	d1ef      	bne.n	3000bb44 <crash_dump_memory+0x54>
3000bb64:	480c      	ldr	r0, [pc, #48]	; (3000bb98 <crash_dump_memory+0xa8>)
3000bb66:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000bb6a:	f000 bd99 	b.w	3000c6a0 <__DiagPrintf_veneer>
3000bb6e:	4641      	mov	r1, r8
3000bb70:	4650      	mov	r0, sl
3000bb72:	f000 fd95 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bb76:	6821      	ldr	r1, [r4, #0]
3000bb78:	4638      	mov	r0, r7
3000bb7a:	f000 fd91 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bb7e:	e7e1      	b.n	3000bb44 <crash_dump_memory+0x54>
3000bb80:	42008000 	.word	0x42008000
3000bb84:	3000db48 	.word	0x3000db48
3000bb88:	3000db4c 	.word	0x3000db4c
3000bb8c:	3000db50 	.word	0x3000db50
3000bb90:	3000dbb8 	.word	0x3000dbb8
3000bb94:	3000db74 	.word	0x3000db74
3000bb98:	3000cf54 	.word	0x3000cf54
3000bb9c:	3000dbc0 	.word	0x3000dbc0
3000bba0:	3000dbb0 	.word	0x3000dbb0

3000bba4 <crash_dump>:
3000bba4:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000bba8:	4c4f      	ldr	r4, [pc, #316]	; (3000bce8 <crash_dump+0x144>)
3000bbaa:	4615      	mov	r5, r2
3000bbac:	4688      	mov	r8, r1
3000bbae:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000bbb2:	484e      	ldr	r0, [pc, #312]	; (3000bcec <crash_dump+0x148>)
3000bbb4:	462f      	mov	r7, r5
3000bbb6:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000bd18 <crash_dump+0x174>
3000bbba:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000bbbe:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000bd24 <crash_dump+0x180>
3000bbc2:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000bbc6:	e9c4 2100 	strd	r2, r1, [r4]
3000bbca:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000bbcc:	60a2      	str	r2, [r4, #8]
3000bbce:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000bbd0:	60e2      	str	r2, [r4, #12]
3000bbd2:	686a      	ldr	r2, [r5, #4]
3000bbd4:	6122      	str	r2, [r4, #16]
3000bbd6:	68aa      	ldr	r2, [r5, #8]
3000bbd8:	6162      	str	r2, [r4, #20]
3000bbda:	68ea      	ldr	r2, [r5, #12]
3000bbdc:	61a2      	str	r2, [r4, #24]
3000bbde:	692a      	ldr	r2, [r5, #16]
3000bbe0:	61e2      	str	r2, [r4, #28]
3000bbe2:	696a      	ldr	r2, [r5, #20]
3000bbe4:	6222      	str	r2, [r4, #32]
3000bbe6:	69aa      	ldr	r2, [r5, #24]
3000bbe8:	6262      	str	r2, [r4, #36]	; 0x24
3000bbea:	69ea      	ldr	r2, [r5, #28]
3000bbec:	62a2      	str	r2, [r4, #40]	; 0x28
3000bbee:	6a2a      	ldr	r2, [r5, #32]
3000bbf0:	62e2      	str	r2, [r4, #44]	; 0x2c
3000bbf2:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000bbf4:	6322      	str	r2, [r4, #48]	; 0x30
3000bbf6:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000bbf8:	6362      	str	r2, [r4, #52]	; 0x34
3000bbfa:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000bbfc:	63a2      	str	r2, [r4, #56]	; 0x38
3000bbfe:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000bc00:	63e2      	str	r2, [r4, #60]	; 0x3c
3000bc02:	682a      	ldr	r2, [r5, #0]
3000bc04:	6422      	str	r2, [r4, #64]	; 0x40
3000bc06:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000bc0a:	6462      	str	r2, [r4, #68]	; 0x44
3000bc0c:	4a38      	ldr	r2, [pc, #224]	; (3000bcf0 <crash_dump+0x14c>)
3000bc0e:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000bc12:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000bc16:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000bc1a:	64e1      	str	r1, [r4, #76]	; 0x4c
3000bc1c:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000bc20:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000bc24:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000bc28:	6561      	str	r1, [r4, #84]	; 0x54
3000bc2a:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000bc2c:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000bc30:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000bc34:	65e1      	str	r1, [r4, #92]	; 0x5c
3000bc36:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000bc38:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000bc3a:	b289      	uxth	r1, r1
3000bc3c:	b292      	uxth	r2, r2
3000bc3e:	6621      	str	r1, [r4, #96]	; 0x60
3000bc40:	6662      	str	r2, [r4, #100]	; 0x64
3000bc42:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000bc46:	66a2      	str	r2, [r4, #104]	; 0x68
3000bc48:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000bc4c:	66e3      	str	r3, [r4, #108]	; 0x6c
3000bc4e:	f000 fd27 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bc52:	4620      	mov	r0, r4
3000bc54:	462c      	mov	r4, r5
3000bc56:	f000 f867 	bl	3000bd28 <fault_diagnosis>
3000bc5a:	4826      	ldr	r0, [pc, #152]	; (3000bcf4 <crash_dump+0x150>)
3000bc5c:	f000 fd20 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bc60:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000bc62:	4825      	ldr	r0, [pc, #148]	; (3000bcf8 <crash_dump+0x154>)
3000bc64:	f000 fd1c 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bc68:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000bc6a:	4824      	ldr	r0, [pc, #144]	; (3000bcfc <crash_dump+0x158>)
3000bc6c:	f000 fd18 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bc70:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000bc72:	4823      	ldr	r0, [pc, #140]	; (3000bd00 <crash_dump+0x15c>)
3000bc74:	f000 fd14 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bc78:	f857 1b20 	ldr.w	r1, [r7], #32
3000bc7c:	4821      	ldr	r0, [pc, #132]	; (3000bd04 <crash_dump+0x160>)
3000bc7e:	463e      	mov	r6, r7
3000bc80:	f000 fd0e 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bc84:	4920      	ldr	r1, [pc, #128]	; (3000bd08 <crash_dump+0x164>)
3000bc86:	4658      	mov	r0, fp
3000bc88:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000bc8c:	f000 fd08 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bc90:	454e      	cmp	r6, r9
3000bc92:	d008      	beq.n	3000bca6 <crash_dump+0x102>
3000bc94:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000bc98:	4658      	mov	r0, fp
3000bc9a:	f85a 1b04 	ldr.w	r1, [sl], #4
3000bc9e:	f000 fcff 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bca2:	454e      	cmp	r6, r9
3000bca4:	d1f6      	bne.n	3000bc94 <crash_dump+0xf0>
3000bca6:	4e19      	ldr	r6, [pc, #100]	; (3000bd0c <crash_dump+0x168>)
3000bca8:	4919      	ldr	r1, [pc, #100]	; (3000bd10 <crash_dump+0x16c>)
3000bcaa:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bd18 <crash_dump+0x174>
3000bcae:	e001      	b.n	3000bcb4 <crash_dump+0x110>
3000bcb0:	f856 1b04 	ldr.w	r1, [r6], #4
3000bcb4:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000bcb8:	4648      	mov	r0, r9
3000bcba:	f000 fcf1 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bcbe:	42bc      	cmp	r4, r7
3000bcc0:	d1f6      	bne.n	3000bcb0 <crash_dump+0x10c>
3000bcc2:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000bcc4:	4913      	ldr	r1, [pc, #76]	; (3000bd14 <crash_dump+0x170>)
3000bcc6:	4814      	ldr	r0, [pc, #80]	; (3000bd18 <crash_dump+0x174>)
3000bcc8:	f000 fcea 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bccc:	2180      	movs	r1, #128	; 0x80
3000bcce:	4640      	mov	r0, r8
3000bcd0:	f7ff ff0e 	bl	3000baf0 <crash_dump_memory>
3000bcd4:	4811      	ldr	r0, [pc, #68]	; (3000bd1c <crash_dump+0x178>)
3000bcd6:	f000 fce3 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bcda:	4811      	ldr	r0, [pc, #68]	; (3000bd20 <crash_dump+0x17c>)
3000bcdc:	f000 fce0 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bce0:	2000      	movs	r0, #0
3000bce2:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bce6:	bf00      	nop
3000bce8:	3000fea8 	.word	0x3000fea8
3000bcec:	3000dbd4 	.word	0x3000dbd4
3000bcf0:	e000ed00 	.word	0xe000ed00
3000bcf4:	3000dbf8 	.word	0x3000dbf8
3000bcf8:	3000dc20 	.word	0x3000dc20
3000bcfc:	3000dc30 	.word	0x3000dc30
3000bd00:	3000dc40 	.word	0x3000dc40
3000bd04:	3000dc50 	.word	0x3000dc50
3000bd08:	3000dbcc 	.word	0x3000dbcc
3000bd0c:	3000dcb0 	.word	0x3000dcb0
3000bd10:	3000dbd0 	.word	0x3000dbd0
3000bd14:	3000dc70 	.word	0x3000dc70
3000bd18:	3000dc60 	.word	0x3000dc60
3000bd1c:	3000dc74 	.word	0x3000dc74
3000bd20:	3000dca0 	.word	0x3000dca0
3000bd24:	3000dcd0 	.word	0x3000dcd0

3000bd28 <fault_diagnosis>:
3000bd28:	b510      	push	{r4, lr}
3000bd2a:	4604      	mov	r4, r0
3000bd2c:	4888      	ldr	r0, [pc, #544]	; (3000bf50 <fault_diagnosis+0x228>)
3000bd2e:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000bd30:	f000 fcb6 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bd34:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000bd38:	079b      	lsls	r3, r3, #30
3000bd3a:	f100 808a 	bmi.w	3000be52 <fault_diagnosis+0x12a>
3000bd3e:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bd40:	b1e1      	cbz	r1, 3000bd7c <fault_diagnosis+0x54>
3000bd42:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bd46:	07d8      	lsls	r0, r3, #31
3000bd48:	f100 8094 	bmi.w	3000be74 <fault_diagnosis+0x14c>
3000bd4c:	079a      	lsls	r2, r3, #30
3000bd4e:	f100 809a 	bmi.w	3000be86 <fault_diagnosis+0x15e>
3000bd52:	0758      	lsls	r0, r3, #29
3000bd54:	f100 80a1 	bmi.w	3000be9a <fault_diagnosis+0x172>
3000bd58:	0719      	lsls	r1, r3, #28
3000bd5a:	f100 80a8 	bmi.w	3000beae <fault_diagnosis+0x186>
3000bd5e:	06da      	lsls	r2, r3, #27
3000bd60:	f100 80af 	bmi.w	3000bec2 <fault_diagnosis+0x19a>
3000bd64:	0698      	lsls	r0, r3, #26
3000bd66:	f100 80b6 	bmi.w	3000bed6 <fault_diagnosis+0x1ae>
3000bd6a:	0619      	lsls	r1, r3, #24
3000bd6c:	f100 80bd 	bmi.w	3000beea <fault_diagnosis+0x1c2>
3000bd70:	065a      	lsls	r2, r3, #25
3000bd72:	d503      	bpl.n	3000bd7c <fault_diagnosis+0x54>
3000bd74:	f013 0f28 	tst.w	r3, #40	; 0x28
3000bd78:	f040 8186 	bne.w	3000c088 <fault_diagnosis+0x360>
3000bd7c:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bd80:	b199      	cbz	r1, 3000bdaa <fault_diagnosis+0x82>
3000bd82:	07cb      	lsls	r3, r1, #31
3000bd84:	f100 80b9 	bmi.w	3000befa <fault_diagnosis+0x1d2>
3000bd88:	0788      	lsls	r0, r1, #30
3000bd8a:	f100 80bf 	bmi.w	3000bf0c <fault_diagnosis+0x1e4>
3000bd8e:	070a      	lsls	r2, r1, #28
3000bd90:	f100 80c5 	bmi.w	3000bf1e <fault_diagnosis+0x1f6>
3000bd94:	06cb      	lsls	r3, r1, #27
3000bd96:	f100 80cb 	bmi.w	3000bf30 <fault_diagnosis+0x208>
3000bd9a:	0688      	lsls	r0, r1, #26
3000bd9c:	f100 80d1 	bmi.w	3000bf42 <fault_diagnosis+0x21a>
3000bda0:	060a      	lsls	r2, r1, #24
3000bda2:	d502      	bpl.n	3000bdaa <fault_diagnosis+0x82>
3000bda4:	078b      	lsls	r3, r1, #30
3000bda6:	f040 8194 	bne.w	3000c0d2 <fault_diagnosis+0x3aa>
3000bdaa:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bdae:	b1b1      	cbz	r1, 3000bdde <fault_diagnosis+0xb6>
3000bdb0:	07c8      	lsls	r0, r1, #31
3000bdb2:	f100 80f7 	bmi.w	3000bfa4 <fault_diagnosis+0x27c>
3000bdb6:	078a      	lsls	r2, r1, #30
3000bdb8:	f100 80fd 	bmi.w	3000bfb6 <fault_diagnosis+0x28e>
3000bdbc:	074b      	lsls	r3, r1, #29
3000bdbe:	f100 8103 	bmi.w	3000bfc8 <fault_diagnosis+0x2a0>
3000bdc2:	0708      	lsls	r0, r1, #28
3000bdc4:	f100 8109 	bmi.w	3000bfda <fault_diagnosis+0x2b2>
3000bdc8:	06ca      	lsls	r2, r1, #27
3000bdca:	f100 810f 	bmi.w	3000bfec <fault_diagnosis+0x2c4>
3000bdce:	068b      	lsls	r3, r1, #26
3000bdd0:	f100 8115 	bmi.w	3000bffe <fault_diagnosis+0x2d6>
3000bdd4:	0608      	lsls	r0, r1, #24
3000bdd6:	d502      	bpl.n	3000bdde <fault_diagnosis+0xb6>
3000bdd8:	0789      	lsls	r1, r1, #30
3000bdda:	f100 8180 	bmi.w	3000c0de <fault_diagnosis+0x3b6>
3000bdde:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bde2:	b1b9      	cbz	r1, 3000be14 <fault_diagnosis+0xec>
3000bde4:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bde8:	07da      	lsls	r2, r3, #31
3000bdea:	f100 810f 	bmi.w	3000c00c <fault_diagnosis+0x2e4>
3000bdee:	0798      	lsls	r0, r3, #30
3000bdf0:	f100 8115 	bmi.w	3000c01e <fault_diagnosis+0x2f6>
3000bdf4:	0759      	lsls	r1, r3, #29
3000bdf6:	f100 811d 	bmi.w	3000c034 <fault_diagnosis+0x30c>
3000bdfa:	071a      	lsls	r2, r3, #28
3000bdfc:	f100 8125 	bmi.w	3000c04a <fault_diagnosis+0x322>
3000be00:	06db      	lsls	r3, r3, #27
3000be02:	f100 812d 	bmi.w	3000c060 <fault_diagnosis+0x338>
3000be06:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000be0a:	07d8      	lsls	r0, r3, #31
3000be0c:	f100 8133 	bmi.w	3000c076 <fault_diagnosis+0x34e>
3000be10:	0799      	lsls	r1, r3, #30
3000be12:	d424      	bmi.n	3000be5e <fault_diagnosis+0x136>
3000be14:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000be18:	2b00      	cmp	r3, #0
3000be1a:	db00      	blt.n	3000be1e <fault_diagnosis+0xf6>
3000be1c:	bd10      	pop	{r4, pc}
3000be1e:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be20:	2900      	cmp	r1, #0
3000be22:	d0fb      	beq.n	3000be1c <fault_diagnosis+0xf4>
3000be24:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000be28:	07da      	lsls	r2, r3, #31
3000be2a:	f100 8143 	bmi.w	3000c0b4 <fault_diagnosis+0x38c>
3000be2e:	0798      	lsls	r0, r3, #30
3000be30:	f100 8138 	bmi.w	3000c0a4 <fault_diagnosis+0x37c>
3000be34:	0759      	lsls	r1, r3, #29
3000be36:	f100 812d 	bmi.w	3000c094 <fault_diagnosis+0x36c>
3000be3a:	071a      	lsls	r2, r3, #28
3000be3c:	f100 8141 	bmi.w	3000c0c2 <fault_diagnosis+0x39a>
3000be40:	06db      	lsls	r3, r3, #27
3000be42:	d5eb      	bpl.n	3000be1c <fault_diagnosis+0xf4>
3000be44:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be46:	4a43      	ldr	r2, [pc, #268]	; (3000bf54 <fault_diagnosis+0x22c>)
3000be48:	4843      	ldr	r0, [pc, #268]	; (3000bf58 <fault_diagnosis+0x230>)
3000be4a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000be4e:	f000 bc27 	b.w	3000c6a0 <__DiagPrintf_veneer>
3000be52:	4a42      	ldr	r2, [pc, #264]	; (3000bf5c <fault_diagnosis+0x234>)
3000be54:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000be56:	4842      	ldr	r0, [pc, #264]	; (3000bf60 <fault_diagnosis+0x238>)
3000be58:	f000 fc22 	bl	3000c6a0 <__DiagPrintf_veneer>
3000be5c:	e76f      	b.n	3000bd3e <fault_diagnosis+0x16>
3000be5e:	4a41      	ldr	r2, [pc, #260]	; (3000bf64 <fault_diagnosis+0x23c>)
3000be60:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be64:	4840      	ldr	r0, [pc, #256]	; (3000bf68 <fault_diagnosis+0x240>)
3000be66:	f000 fc1b 	bl	3000c6a0 <__DiagPrintf_veneer>
3000be6a:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000be6e:	2b00      	cmp	r3, #0
3000be70:	dad4      	bge.n	3000be1c <fault_diagnosis+0xf4>
3000be72:	e7d4      	b.n	3000be1e <fault_diagnosis+0xf6>
3000be74:	4a3d      	ldr	r2, [pc, #244]	; (3000bf6c <fault_diagnosis+0x244>)
3000be76:	483e      	ldr	r0, [pc, #248]	; (3000bf70 <fault_diagnosis+0x248>)
3000be78:	f000 fc12 	bl	3000c6a0 <__DiagPrintf_veneer>
3000be7c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000be80:	079a      	lsls	r2, r3, #30
3000be82:	f57f af66 	bpl.w	3000bd52 <fault_diagnosis+0x2a>
3000be86:	4a3b      	ldr	r2, [pc, #236]	; (3000bf74 <fault_diagnosis+0x24c>)
3000be88:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000be8a:	4839      	ldr	r0, [pc, #228]	; (3000bf70 <fault_diagnosis+0x248>)
3000be8c:	f000 fc08 	bl	3000c6a0 <__DiagPrintf_veneer>
3000be90:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000be94:	0758      	lsls	r0, r3, #29
3000be96:	f57f af5f 	bpl.w	3000bd58 <fault_diagnosis+0x30>
3000be9a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000be9c:	4a36      	ldr	r2, [pc, #216]	; (3000bf78 <fault_diagnosis+0x250>)
3000be9e:	4834      	ldr	r0, [pc, #208]	; (3000bf70 <fault_diagnosis+0x248>)
3000bea0:	f000 fbfe 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bea4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bea8:	0719      	lsls	r1, r3, #28
3000beaa:	f57f af58 	bpl.w	3000bd5e <fault_diagnosis+0x36>
3000beae:	4a33      	ldr	r2, [pc, #204]	; (3000bf7c <fault_diagnosis+0x254>)
3000beb0:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000beb2:	482f      	ldr	r0, [pc, #188]	; (3000bf70 <fault_diagnosis+0x248>)
3000beb4:	f000 fbf4 	bl	3000c6a0 <__DiagPrintf_veneer>
3000beb8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bebc:	06da      	lsls	r2, r3, #27
3000bebe:	f57f af51 	bpl.w	3000bd64 <fault_diagnosis+0x3c>
3000bec2:	4a2f      	ldr	r2, [pc, #188]	; (3000bf80 <fault_diagnosis+0x258>)
3000bec4:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bec6:	482a      	ldr	r0, [pc, #168]	; (3000bf70 <fault_diagnosis+0x248>)
3000bec8:	f000 fbea 	bl	3000c6a0 <__DiagPrintf_veneer>
3000becc:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bed0:	0698      	lsls	r0, r3, #26
3000bed2:	f57f af4a 	bpl.w	3000bd6a <fault_diagnosis+0x42>
3000bed6:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bed8:	4a2a      	ldr	r2, [pc, #168]	; (3000bf84 <fault_diagnosis+0x25c>)
3000beda:	4825      	ldr	r0, [pc, #148]	; (3000bf70 <fault_diagnosis+0x248>)
3000bedc:	f000 fbe0 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bee0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bee4:	0619      	lsls	r1, r3, #24
3000bee6:	f57f af43 	bpl.w	3000bd70 <fault_diagnosis+0x48>
3000beea:	4a27      	ldr	r2, [pc, #156]	; (3000bf88 <fault_diagnosis+0x260>)
3000beec:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000beee:	4820      	ldr	r0, [pc, #128]	; (3000bf70 <fault_diagnosis+0x248>)
3000bef0:	f000 fbd6 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bef4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bef8:	e73a      	b.n	3000bd70 <fault_diagnosis+0x48>
3000befa:	4a24      	ldr	r2, [pc, #144]	; (3000bf8c <fault_diagnosis+0x264>)
3000befc:	4824      	ldr	r0, [pc, #144]	; (3000bf90 <fault_diagnosis+0x268>)
3000befe:	f000 fbcf 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bf02:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf06:	0788      	lsls	r0, r1, #30
3000bf08:	f57f af41 	bpl.w	3000bd8e <fault_diagnosis+0x66>
3000bf0c:	4a21      	ldr	r2, [pc, #132]	; (3000bf94 <fault_diagnosis+0x26c>)
3000bf0e:	4820      	ldr	r0, [pc, #128]	; (3000bf90 <fault_diagnosis+0x268>)
3000bf10:	f000 fbc6 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bf14:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf18:	070a      	lsls	r2, r1, #28
3000bf1a:	f57f af3b 	bpl.w	3000bd94 <fault_diagnosis+0x6c>
3000bf1e:	4a1e      	ldr	r2, [pc, #120]	; (3000bf98 <fault_diagnosis+0x270>)
3000bf20:	481b      	ldr	r0, [pc, #108]	; (3000bf90 <fault_diagnosis+0x268>)
3000bf22:	f000 fbbd 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bf26:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf2a:	06cb      	lsls	r3, r1, #27
3000bf2c:	f57f af35 	bpl.w	3000bd9a <fault_diagnosis+0x72>
3000bf30:	4a1a      	ldr	r2, [pc, #104]	; (3000bf9c <fault_diagnosis+0x274>)
3000bf32:	4817      	ldr	r0, [pc, #92]	; (3000bf90 <fault_diagnosis+0x268>)
3000bf34:	f000 fbb4 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bf38:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf3c:	0688      	lsls	r0, r1, #26
3000bf3e:	f57f af2f 	bpl.w	3000bda0 <fault_diagnosis+0x78>
3000bf42:	4a17      	ldr	r2, [pc, #92]	; (3000bfa0 <fault_diagnosis+0x278>)
3000bf44:	4812      	ldr	r0, [pc, #72]	; (3000bf90 <fault_diagnosis+0x268>)
3000bf46:	f000 fbab 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bf4a:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf4e:	e727      	b.n	3000bda0 <fault_diagnosis+0x78>
3000bf50:	3000dce0 	.word	0x3000dce0
3000bf54:	3000e570 	.word	0x3000e570
3000bf58:	3000e4c8 	.word	0x3000e4c8
3000bf5c:	3000dcf8 	.word	0x3000dcf8
3000bf60:	3000dd24 	.word	0x3000dd24
3000bf64:	3000e42c 	.word	0x3000e42c
3000bf68:	3000e270 	.word	0x3000e270
3000bf6c:	3000dd3c 	.word	0x3000dd3c
3000bf70:	3000dd6c 	.word	0x3000dd6c
3000bf74:	3000dd84 	.word	0x3000dd84
3000bf78:	3000ddbc 	.word	0x3000ddbc
3000bf7c:	3000ddf4 	.word	0x3000ddf4
3000bf80:	3000de2c 	.word	0x3000de2c
3000bf84:	3000de5c 	.word	0x3000de5c
3000bf88:	3000de98 	.word	0x3000de98
3000bf8c:	3000df08 	.word	0x3000df08
3000bf90:	3000df4c 	.word	0x3000df4c
3000bf94:	3000df64 	.word	0x3000df64
3000bf98:	3000dfa0 	.word	0x3000dfa0
3000bf9c:	3000dfd8 	.word	0x3000dfd8
3000bfa0:	3000e00c 	.word	0x3000e00c
3000bfa4:	4a51      	ldr	r2, [pc, #324]	; (3000c0ec <fault_diagnosis+0x3c4>)
3000bfa6:	4852      	ldr	r0, [pc, #328]	; (3000c0f0 <fault_diagnosis+0x3c8>)
3000bfa8:	f000 fb7a 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bfac:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfb0:	078a      	lsls	r2, r1, #30
3000bfb2:	f57f af03 	bpl.w	3000bdbc <fault_diagnosis+0x94>
3000bfb6:	4a4f      	ldr	r2, [pc, #316]	; (3000c0f4 <fault_diagnosis+0x3cc>)
3000bfb8:	484d      	ldr	r0, [pc, #308]	; (3000c0f0 <fault_diagnosis+0x3c8>)
3000bfba:	f000 fb71 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bfbe:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfc2:	074b      	lsls	r3, r1, #29
3000bfc4:	f57f aefd 	bpl.w	3000bdc2 <fault_diagnosis+0x9a>
3000bfc8:	4a4b      	ldr	r2, [pc, #300]	; (3000c0f8 <fault_diagnosis+0x3d0>)
3000bfca:	4849      	ldr	r0, [pc, #292]	; (3000c0f0 <fault_diagnosis+0x3c8>)
3000bfcc:	f000 fb68 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bfd0:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfd4:	0708      	lsls	r0, r1, #28
3000bfd6:	f57f aef7 	bpl.w	3000bdc8 <fault_diagnosis+0xa0>
3000bfda:	4a48      	ldr	r2, [pc, #288]	; (3000c0fc <fault_diagnosis+0x3d4>)
3000bfdc:	4844      	ldr	r0, [pc, #272]	; (3000c0f0 <fault_diagnosis+0x3c8>)
3000bfde:	f000 fb5f 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bfe2:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfe6:	06ca      	lsls	r2, r1, #27
3000bfe8:	f57f aef1 	bpl.w	3000bdce <fault_diagnosis+0xa6>
3000bfec:	4a44      	ldr	r2, [pc, #272]	; (3000c100 <fault_diagnosis+0x3d8>)
3000bfee:	4840      	ldr	r0, [pc, #256]	; (3000c0f0 <fault_diagnosis+0x3c8>)
3000bff0:	f000 fb56 	bl	3000c6a0 <__DiagPrintf_veneer>
3000bff4:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bff8:	068b      	lsls	r3, r1, #26
3000bffa:	f57f aeeb 	bpl.w	3000bdd4 <fault_diagnosis+0xac>
3000bffe:	4a41      	ldr	r2, [pc, #260]	; (3000c104 <fault_diagnosis+0x3dc>)
3000c000:	483b      	ldr	r0, [pc, #236]	; (3000c0f0 <fault_diagnosis+0x3c8>)
3000c002:	f000 fb4d 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c006:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000c00a:	e6e3      	b.n	3000bdd4 <fault_diagnosis+0xac>
3000c00c:	4a3e      	ldr	r2, [pc, #248]	; (3000c108 <fault_diagnosis+0x3e0>)
3000c00e:	483f      	ldr	r0, [pc, #252]	; (3000c10c <fault_diagnosis+0x3e4>)
3000c010:	f000 fb46 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c014:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c018:	0798      	lsls	r0, r3, #30
3000c01a:	f57f aeeb 	bpl.w	3000bdf4 <fault_diagnosis+0xcc>
3000c01e:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c022:	4a3b      	ldr	r2, [pc, #236]	; (3000c110 <fault_diagnosis+0x3e8>)
3000c024:	4839      	ldr	r0, [pc, #228]	; (3000c10c <fault_diagnosis+0x3e4>)
3000c026:	f000 fb3b 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c02a:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c02e:	0759      	lsls	r1, r3, #29
3000c030:	f57f aee3 	bpl.w	3000bdfa <fault_diagnosis+0xd2>
3000c034:	4a37      	ldr	r2, [pc, #220]	; (3000c114 <fault_diagnosis+0x3ec>)
3000c036:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c03a:	4834      	ldr	r0, [pc, #208]	; (3000c10c <fault_diagnosis+0x3e4>)
3000c03c:	f000 fb30 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c040:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c044:	071a      	lsls	r2, r3, #28
3000c046:	f57f aedb 	bpl.w	3000be00 <fault_diagnosis+0xd8>
3000c04a:	4a33      	ldr	r2, [pc, #204]	; (3000c118 <fault_diagnosis+0x3f0>)
3000c04c:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c050:	482e      	ldr	r0, [pc, #184]	; (3000c10c <fault_diagnosis+0x3e4>)
3000c052:	f000 fb25 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c056:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c05a:	06db      	lsls	r3, r3, #27
3000c05c:	f57f aed3 	bpl.w	3000be06 <fault_diagnosis+0xde>
3000c060:	4a2e      	ldr	r2, [pc, #184]	; (3000c11c <fault_diagnosis+0x3f4>)
3000c062:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c066:	4829      	ldr	r0, [pc, #164]	; (3000c10c <fault_diagnosis+0x3e4>)
3000c068:	f000 fb1a 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c06c:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000c070:	07d8      	lsls	r0, r3, #31
3000c072:	f57f aecd 	bpl.w	3000be10 <fault_diagnosis+0xe8>
3000c076:	4a2a      	ldr	r2, [pc, #168]	; (3000c120 <fault_diagnosis+0x3f8>)
3000c078:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c07c:	4823      	ldr	r0, [pc, #140]	; (3000c10c <fault_diagnosis+0x3e4>)
3000c07e:	f000 fb0f 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c082:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000c086:	e6c3      	b.n	3000be10 <fault_diagnosis+0xe8>
3000c088:	4a26      	ldr	r2, [pc, #152]	; (3000c124 <fault_diagnosis+0x3fc>)
3000c08a:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000c08c:	4826      	ldr	r0, [pc, #152]	; (3000c128 <fault_diagnosis+0x400>)
3000c08e:	f000 fb07 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c092:	e673      	b.n	3000bd7c <fault_diagnosis+0x54>
3000c094:	4a25      	ldr	r2, [pc, #148]	; (3000c12c <fault_diagnosis+0x404>)
3000c096:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c098:	4825      	ldr	r0, [pc, #148]	; (3000c130 <fault_diagnosis+0x408>)
3000c09a:	f000 fb01 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c09e:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c0a2:	e6ca      	b.n	3000be3a <fault_diagnosis+0x112>
3000c0a4:	4a23      	ldr	r2, [pc, #140]	; (3000c134 <fault_diagnosis+0x40c>)
3000c0a6:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c0a8:	4821      	ldr	r0, [pc, #132]	; (3000c130 <fault_diagnosis+0x408>)
3000c0aa:	f000 faf9 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c0ae:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c0b2:	e6bf      	b.n	3000be34 <fault_diagnosis+0x10c>
3000c0b4:	4a20      	ldr	r2, [pc, #128]	; (3000c138 <fault_diagnosis+0x410>)
3000c0b6:	481e      	ldr	r0, [pc, #120]	; (3000c130 <fault_diagnosis+0x408>)
3000c0b8:	f000 faf2 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c0bc:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c0c0:	e6b5      	b.n	3000be2e <fault_diagnosis+0x106>
3000c0c2:	4a1e      	ldr	r2, [pc, #120]	; (3000c13c <fault_diagnosis+0x414>)
3000c0c4:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c0c6:	481a      	ldr	r0, [pc, #104]	; (3000c130 <fault_diagnosis+0x408>)
3000c0c8:	f000 faea 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c0cc:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c0d0:	e6b6      	b.n	3000be40 <fault_diagnosis+0x118>
3000c0d2:	4a1b      	ldr	r2, [pc, #108]	; (3000c140 <fault_diagnosis+0x418>)
3000c0d4:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000c0d6:	481b      	ldr	r0, [pc, #108]	; (3000c144 <fault_diagnosis+0x41c>)
3000c0d8:	f000 fae2 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c0dc:	e665      	b.n	3000bdaa <fault_diagnosis+0x82>
3000c0de:	4a1a      	ldr	r2, [pc, #104]	; (3000c148 <fault_diagnosis+0x420>)
3000c0e0:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000c0e2:	481a      	ldr	r0, [pc, #104]	; (3000c14c <fault_diagnosis+0x424>)
3000c0e4:	f000 fadc 	bl	3000c6a0 <__DiagPrintf_veneer>
3000c0e8:	e679      	b.n	3000bdde <fault_diagnosis+0xb6>
3000c0ea:	bf00      	nop
3000c0ec:	3000e0a0 	.word	0x3000e0a0
3000c0f0:	3000e0d4 	.word	0x3000e0d4
3000c0f4:	3000e0ec 	.word	0x3000e0ec
3000c0f8:	3000e124 	.word	0x3000e124
3000c0fc:	3000e15c 	.word	0x3000e15c
3000c100:	3000e184 	.word	0x3000e184
3000c104:	3000e1ac 	.word	0x3000e1ac
3000c108:	3000e228 	.word	0x3000e228
3000c10c:	3000e270 	.word	0x3000e270
3000c110:	3000e288 	.word	0x3000e288
3000c114:	3000e2d4 	.word	0x3000e2d4
3000c118:	3000e334 	.word	0x3000e334
3000c11c:	3000e37c 	.word	0x3000e37c
3000c120:	3000e3d8 	.word	0x3000e3d8
3000c124:	3000dec8 	.word	0x3000dec8
3000c128:	3000def0 	.word	0x3000def0
3000c12c:	3000e514 	.word	0x3000e514
3000c130:	3000e4c8 	.word	0x3000e4c8
3000c134:	3000e4e0 	.word	0x3000e4e0
3000c138:	3000e498 	.word	0x3000e498
3000c13c:	3000e540 	.word	0x3000e540
3000c140:	3000e058 	.word	0x3000e058
3000c144:	3000e088 	.word	0x3000e088
3000c148:	3000e1ec 	.word	0x3000e1ec
3000c14c:	3000e210 	.word	0x3000e210

3000c150 <rtk_log_write>:
3000c150:	b40c      	push	{r2, r3}
3000c152:	4b18      	ldr	r3, [pc, #96]	; (3000c1b4 <rtk_log_write+0x64>)
3000c154:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000c158:	681a      	ldr	r2, [r3, #0]
3000c15a:	b083      	sub	sp, #12
3000c15c:	4680      	mov	r8, r0
3000c15e:	460e      	mov	r6, r1
3000c160:	2a02      	cmp	r2, #2
3000c162:	d825      	bhi.n	3000c1b0 <rtk_log_write+0x60>
3000c164:	681f      	ldr	r7, [r3, #0]
3000c166:	b1a7      	cbz	r7, 3000c192 <rtk_log_write+0x42>
3000c168:	4d13      	ldr	r5, [pc, #76]	; (3000c1b8 <rtk_log_write+0x68>)
3000c16a:	2400      	movs	r4, #0
3000c16c:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000c1c4 <rtk_log_write+0x74>
3000c170:	e002      	b.n	3000c178 <rtk_log_write+0x28>
3000c172:	3401      	adds	r4, #1
3000c174:	42a7      	cmp	r7, r4
3000c176:	d90c      	bls.n	3000c192 <rtk_log_write+0x42>
3000c178:	4628      	mov	r0, r5
3000c17a:	4631      	mov	r1, r6
3000c17c:	350b      	adds	r5, #11
3000c17e:	47c8      	blx	r9
3000c180:	2800      	cmp	r0, #0
3000c182:	d1f6      	bne.n	3000c172 <rtk_log_write+0x22>
3000c184:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000c188:	4b0c      	ldr	r3, [pc, #48]	; (3000c1bc <rtk_log_write+0x6c>)
3000c18a:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000c18e:	5d1b      	ldrb	r3, [r3, r4]
3000c190:	e001      	b.n	3000c196 <rtk_log_write+0x46>
3000c192:	4b0b      	ldr	r3, [pc, #44]	; (3000c1c0 <rtk_log_write+0x70>)
3000c194:	781b      	ldrb	r3, [r3, #0]
3000c196:	4598      	cmp	r8, r3
3000c198:	d805      	bhi.n	3000c1a6 <rtk_log_write+0x56>
3000c19a:	aa0b      	add	r2, sp, #44	; 0x2c
3000c19c:	990a      	ldr	r1, [sp, #40]	; 0x28
3000c19e:	2000      	movs	r0, #0
3000c1a0:	9201      	str	r2, [sp, #4]
3000c1a2:	f000 fa79 	bl	3000c698 <__DiagVSprintf_veneer>
3000c1a6:	b003      	add	sp, #12
3000c1a8:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000c1ac:	b002      	add	sp, #8
3000c1ae:	4770      	bx	lr
3000c1b0:	2703      	movs	r7, #3
3000c1b2:	e7d9      	b.n	3000c168 <rtk_log_write+0x18>
3000c1b4:	300102e8 	.word	0x300102e8
3000c1b8:	300102ed 	.word	0x300102ed
3000c1bc:	300102ec 	.word	0x300102ec
3000c1c0:	3000f8d0 	.word	0x3000f8d0
3000c1c4:	00012c89 	.word	0x00012c89

3000c1c8 <BOOT_VerCheck>:
3000c1c8:	b570      	push	{r4, r5, r6, lr}
3000c1ca:	4b1d      	ldr	r3, [pc, #116]	; (3000c240 <BOOT_VerCheck+0x78>)
3000c1cc:	4798      	blx	r3
3000c1ce:	2800      	cmp	r0, #0
3000c1d0:	d135      	bne.n	3000c23e <BOOT_VerCheck+0x76>
3000c1d2:	4b1c      	ldr	r3, [pc, #112]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c1d4:	2403      	movs	r4, #3
3000c1d6:	4d1c      	ldr	r5, [pc, #112]	; (3000c248 <BOOT_VerCheck+0x80>)
3000c1d8:	2003      	movs	r0, #3
3000c1da:	4a1c      	ldr	r2, [pc, #112]	; (3000c24c <BOOT_VerCheck+0x84>)
3000c1dc:	4619      	mov	r1, r3
3000c1de:	f7ff ffb7 	bl	3000c150 <rtk_log_write>
3000c1e2:	4b18      	ldr	r3, [pc, #96]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c1e4:	462a      	mov	r2, r5
3000c1e6:	2003      	movs	r0, #3
3000c1e8:	4619      	mov	r1, r3
3000c1ea:	f7ff ffb1 	bl	3000c150 <rtk_log_write>
3000c1ee:	4b15      	ldr	r3, [pc, #84]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c1f0:	4a17      	ldr	r2, [pc, #92]	; (3000c250 <BOOT_VerCheck+0x88>)
3000c1f2:	2003      	movs	r0, #3
3000c1f4:	4619      	mov	r1, r3
3000c1f6:	f7ff ffab 	bl	3000c150 <rtk_log_write>
3000c1fa:	4b12      	ldr	r3, [pc, #72]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c1fc:	4a15      	ldr	r2, [pc, #84]	; (3000c254 <BOOT_VerCheck+0x8c>)
3000c1fe:	2003      	movs	r0, #3
3000c200:	4619      	mov	r1, r3
3000c202:	f7ff ffa5 	bl	3000c150 <rtk_log_write>
3000c206:	4b0f      	ldr	r3, [pc, #60]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c208:	4a13      	ldr	r2, [pc, #76]	; (3000c258 <BOOT_VerCheck+0x90>)
3000c20a:	2003      	movs	r0, #3
3000c20c:	4619      	mov	r1, r3
3000c20e:	f7ff ff9f 	bl	3000c150 <rtk_log_write>
3000c212:	4b0c      	ldr	r3, [pc, #48]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c214:	462a      	mov	r2, r5
3000c216:	2003      	movs	r0, #3
3000c218:	4619      	mov	r1, r3
3000c21a:	f7ff ff99 	bl	3000c150 <rtk_log_write>
3000c21e:	4b09      	ldr	r3, [pc, #36]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c220:	4a0e      	ldr	r2, [pc, #56]	; (3000c25c <BOOT_VerCheck+0x94>)
3000c222:	2003      	movs	r0, #3
3000c224:	4619      	mov	r1, r3
3000c226:	f7ff ff93 	bl	3000c150 <rtk_log_write>
3000c22a:	3c01      	subs	r4, #1
3000c22c:	d1d9      	bne.n	3000c1e2 <BOOT_VerCheck+0x1a>
3000c22e:	4b05      	ldr	r3, [pc, #20]	; (3000c244 <BOOT_VerCheck+0x7c>)
3000c230:	2003      	movs	r0, #3
3000c232:	4a0b      	ldr	r2, [pc, #44]	; (3000c260 <BOOT_VerCheck+0x98>)
3000c234:	4619      	mov	r1, r3
3000c236:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000c23a:	f7ff bf89 	b.w	3000c150 <rtk_log_write>
3000c23e:	bd70      	pop	{r4, r5, r6, pc}
3000c240:	30009ebd 	.word	0x30009ebd
3000c244:	3000e642 	.word	0x3000e642
3000c248:	3000e68a 	.word	0x3000e68a
3000c24c:	3000e647 	.word	0x3000e647
3000c250:	3000e6cd 	.word	0x3000e6cd
3000c254:	3000e710 	.word	0x3000e710
3000c258:	3000e753 	.word	0x3000e753
3000c25c:	3000e796 	.word	0x3000e796
3000c260:	3000e7d9 	.word	0x3000e7d9

3000c264 <__aeabi_uldivmod>:
3000c264:	b953      	cbnz	r3, 3000c27c <__aeabi_uldivmod+0x18>
3000c266:	b94a      	cbnz	r2, 3000c27c <__aeabi_uldivmod+0x18>
3000c268:	2900      	cmp	r1, #0
3000c26a:	bf08      	it	eq
3000c26c:	2800      	cmpeq	r0, #0
3000c26e:	bf1c      	itt	ne
3000c270:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000c274:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000c278:	f000 b960 	b.w	3000c53c <__aeabi_idiv0>
3000c27c:	f1ad 0c08 	sub.w	ip, sp, #8
3000c280:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000c284:	f000 f806 	bl	3000c294 <__udivmoddi4>
3000c288:	f8dd e004 	ldr.w	lr, [sp, #4]
3000c28c:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000c290:	b004      	add	sp, #16
3000c292:	4770      	bx	lr

3000c294 <__udivmoddi4>:
3000c294:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000c298:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000c29a:	4688      	mov	r8, r1
3000c29c:	468e      	mov	lr, r1
3000c29e:	4615      	mov	r5, r2
3000c2a0:	4604      	mov	r4, r0
3000c2a2:	4619      	mov	r1, r3
3000c2a4:	2b00      	cmp	r3, #0
3000c2a6:	f040 80c6 	bne.w	3000c436 <__udivmoddi4+0x1a2>
3000c2aa:	4542      	cmp	r2, r8
3000c2ac:	fab2 f782 	clz	r7, r2
3000c2b0:	d946      	bls.n	3000c340 <__udivmoddi4+0xac>
3000c2b2:	b14f      	cbz	r7, 3000c2c8 <__udivmoddi4+0x34>
3000c2b4:	f1c7 0c20 	rsb	ip, r7, #32
3000c2b8:	fa08 f307 	lsl.w	r3, r8, r7
3000c2bc:	40bd      	lsls	r5, r7
3000c2be:	40bc      	lsls	r4, r7
3000c2c0:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c2c4:	ea4c 0e03 	orr.w	lr, ip, r3
3000c2c8:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000c2cc:	fa1f fc85 	uxth.w	ip, r5
3000c2d0:	0c22      	lsrs	r2, r4, #16
3000c2d2:	fbbe f9f8 	udiv	r9, lr, r8
3000c2d6:	fb08 e319 	mls	r3, r8, r9, lr
3000c2da:	fb09 fa0c 	mul.w	sl, r9, ip
3000c2de:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000c2e2:	459a      	cmp	sl, r3
3000c2e4:	d928      	bls.n	3000c338 <__udivmoddi4+0xa4>
3000c2e6:	18eb      	adds	r3, r5, r3
3000c2e8:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000c2ec:	d204      	bcs.n	3000c2f8 <__udivmoddi4+0x64>
3000c2ee:	459a      	cmp	sl, r3
3000c2f0:	d902      	bls.n	3000c2f8 <__udivmoddi4+0x64>
3000c2f2:	f1a9 0002 	sub.w	r0, r9, #2
3000c2f6:	442b      	add	r3, r5
3000c2f8:	eba3 030a 	sub.w	r3, r3, sl
3000c2fc:	b2a4      	uxth	r4, r4
3000c2fe:	fbb3 f2f8 	udiv	r2, r3, r8
3000c302:	fb08 3312 	mls	r3, r8, r2, r3
3000c306:	fb02 fc0c 	mul.w	ip, r2, ip
3000c30a:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c30e:	45a4      	cmp	ip, r4
3000c310:	d914      	bls.n	3000c33c <__udivmoddi4+0xa8>
3000c312:	192c      	adds	r4, r5, r4
3000c314:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000c318:	d203      	bcs.n	3000c322 <__udivmoddi4+0x8e>
3000c31a:	45a4      	cmp	ip, r4
3000c31c:	d901      	bls.n	3000c322 <__udivmoddi4+0x8e>
3000c31e:	1e93      	subs	r3, r2, #2
3000c320:	442c      	add	r4, r5
3000c322:	eba4 040c 	sub.w	r4, r4, ip
3000c326:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000c32a:	b11e      	cbz	r6, 3000c334 <__udivmoddi4+0xa0>
3000c32c:	40fc      	lsrs	r4, r7
3000c32e:	2300      	movs	r3, #0
3000c330:	6034      	str	r4, [r6, #0]
3000c332:	6073      	str	r3, [r6, #4]
3000c334:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000c338:	4648      	mov	r0, r9
3000c33a:	e7dd      	b.n	3000c2f8 <__udivmoddi4+0x64>
3000c33c:	4613      	mov	r3, r2
3000c33e:	e7f0      	b.n	3000c322 <__udivmoddi4+0x8e>
3000c340:	b902      	cbnz	r2, 3000c344 <__udivmoddi4+0xb0>
3000c342:	deff      	udf	#255	; 0xff
3000c344:	bb8f      	cbnz	r7, 3000c3aa <__udivmoddi4+0x116>
3000c346:	eba8 0302 	sub.w	r3, r8, r2
3000c34a:	2101      	movs	r1, #1
3000c34c:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c350:	b2aa      	uxth	r2, r5
3000c352:	0c20      	lsrs	r0, r4, #16
3000c354:	fbb3 fcfe 	udiv	ip, r3, lr
3000c358:	fb0e 331c 	mls	r3, lr, ip, r3
3000c35c:	fb0c f802 	mul.w	r8, ip, r2
3000c360:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000c364:	4598      	cmp	r8, r3
3000c366:	d962      	bls.n	3000c42e <__udivmoddi4+0x19a>
3000c368:	18eb      	adds	r3, r5, r3
3000c36a:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000c36e:	d204      	bcs.n	3000c37a <__udivmoddi4+0xe6>
3000c370:	4598      	cmp	r8, r3
3000c372:	d902      	bls.n	3000c37a <__udivmoddi4+0xe6>
3000c374:	f1ac 0002 	sub.w	r0, ip, #2
3000c378:	442b      	add	r3, r5
3000c37a:	eba3 0308 	sub.w	r3, r3, r8
3000c37e:	b2a4      	uxth	r4, r4
3000c380:	fbb3 fcfe 	udiv	ip, r3, lr
3000c384:	fb0e 331c 	mls	r3, lr, ip, r3
3000c388:	fb0c f202 	mul.w	r2, ip, r2
3000c38c:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c390:	42a2      	cmp	r2, r4
3000c392:	d94e      	bls.n	3000c432 <__udivmoddi4+0x19e>
3000c394:	192c      	adds	r4, r5, r4
3000c396:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000c39a:	d204      	bcs.n	3000c3a6 <__udivmoddi4+0x112>
3000c39c:	42a2      	cmp	r2, r4
3000c39e:	d902      	bls.n	3000c3a6 <__udivmoddi4+0x112>
3000c3a0:	f1ac 0302 	sub.w	r3, ip, #2
3000c3a4:	442c      	add	r4, r5
3000c3a6:	1aa4      	subs	r4, r4, r2
3000c3a8:	e7bd      	b.n	3000c326 <__udivmoddi4+0x92>
3000c3aa:	f1c7 0c20 	rsb	ip, r7, #32
3000c3ae:	40bd      	lsls	r5, r7
3000c3b0:	fa08 f307 	lsl.w	r3, r8, r7
3000c3b4:	40bc      	lsls	r4, r7
3000c3b6:	fa28 f90c 	lsr.w	r9, r8, ip
3000c3ba:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c3be:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c3c2:	ea4c 0203 	orr.w	r2, ip, r3
3000c3c6:	b2ab      	uxth	r3, r5
3000c3c8:	fbb9 fcfe 	udiv	ip, r9, lr
3000c3cc:	0c11      	lsrs	r1, r2, #16
3000c3ce:	fb0e 901c 	mls	r0, lr, ip, r9
3000c3d2:	fb0c f803 	mul.w	r8, ip, r3
3000c3d6:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c3da:	4580      	cmp	r8, r0
3000c3dc:	d923      	bls.n	3000c426 <__udivmoddi4+0x192>
3000c3de:	1828      	adds	r0, r5, r0
3000c3e0:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000c3e4:	d204      	bcs.n	3000c3f0 <__udivmoddi4+0x15c>
3000c3e6:	4580      	cmp	r8, r0
3000c3e8:	d902      	bls.n	3000c3f0 <__udivmoddi4+0x15c>
3000c3ea:	f1ac 0102 	sub.w	r1, ip, #2
3000c3ee:	4428      	add	r0, r5
3000c3f0:	eba0 0008 	sub.w	r0, r0, r8
3000c3f4:	b292      	uxth	r2, r2
3000c3f6:	fbb0 fcfe 	udiv	ip, r0, lr
3000c3fa:	fb0e 001c 	mls	r0, lr, ip, r0
3000c3fe:	fb0c f803 	mul.w	r8, ip, r3
3000c402:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c406:	4598      	cmp	r8, r3
3000c408:	d90f      	bls.n	3000c42a <__udivmoddi4+0x196>
3000c40a:	18eb      	adds	r3, r5, r3
3000c40c:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000c410:	d204      	bcs.n	3000c41c <__udivmoddi4+0x188>
3000c412:	4598      	cmp	r8, r3
3000c414:	d902      	bls.n	3000c41c <__udivmoddi4+0x188>
3000c416:	f1ac 0202 	sub.w	r2, ip, #2
3000c41a:	442b      	add	r3, r5
3000c41c:	eba3 0308 	sub.w	r3, r3, r8
3000c420:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c424:	e792      	b.n	3000c34c <__udivmoddi4+0xb8>
3000c426:	4661      	mov	r1, ip
3000c428:	e7e2      	b.n	3000c3f0 <__udivmoddi4+0x15c>
3000c42a:	4662      	mov	r2, ip
3000c42c:	e7f6      	b.n	3000c41c <__udivmoddi4+0x188>
3000c42e:	4660      	mov	r0, ip
3000c430:	e7a3      	b.n	3000c37a <__udivmoddi4+0xe6>
3000c432:	4663      	mov	r3, ip
3000c434:	e7b7      	b.n	3000c3a6 <__udivmoddi4+0x112>
3000c436:	4543      	cmp	r3, r8
3000c438:	d905      	bls.n	3000c446 <__udivmoddi4+0x1b2>
3000c43a:	b10e      	cbz	r6, 3000c440 <__udivmoddi4+0x1ac>
3000c43c:	e9c6 0800 	strd	r0, r8, [r6]
3000c440:	2100      	movs	r1, #0
3000c442:	4608      	mov	r0, r1
3000c444:	e776      	b.n	3000c334 <__udivmoddi4+0xa0>
3000c446:	fab3 fc83 	clz	ip, r3
3000c44a:	f1bc 0f00 	cmp.w	ip, #0
3000c44e:	d110      	bne.n	3000c472 <__udivmoddi4+0x1de>
3000c450:	4543      	cmp	r3, r8
3000c452:	d301      	bcc.n	3000c458 <__udivmoddi4+0x1c4>
3000c454:	4282      	cmp	r2, r0
3000c456:	d80a      	bhi.n	3000c46e <__udivmoddi4+0x1da>
3000c458:	1a84      	subs	r4, r0, r2
3000c45a:	eb68 0303 	sbc.w	r3, r8, r3
3000c45e:	2001      	movs	r0, #1
3000c460:	469e      	mov	lr, r3
3000c462:	2e00      	cmp	r6, #0
3000c464:	d068      	beq.n	3000c538 <__udivmoddi4+0x2a4>
3000c466:	e9c6 4e00 	strd	r4, lr, [r6]
3000c46a:	2100      	movs	r1, #0
3000c46c:	e762      	b.n	3000c334 <__udivmoddi4+0xa0>
3000c46e:	4660      	mov	r0, ip
3000c470:	e7f7      	b.n	3000c462 <__udivmoddi4+0x1ce>
3000c472:	f1cc 0e20 	rsb	lr, ip, #32
3000c476:	fa03 f30c 	lsl.w	r3, r3, ip
3000c47a:	fa02 f50c 	lsl.w	r5, r2, ip
3000c47e:	fa00 f70c 	lsl.w	r7, r0, ip
3000c482:	fa22 f40e 	lsr.w	r4, r2, lr
3000c486:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c48a:	fa20 f10e 	lsr.w	r1, r0, lr
3000c48e:	431c      	orrs	r4, r3
3000c490:	fa08 f30c 	lsl.w	r3, r8, ip
3000c494:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c498:	4319      	orrs	r1, r3
3000c49a:	b2a3      	uxth	r3, r4
3000c49c:	0c0a      	lsrs	r2, r1, #16
3000c49e:	fbbb f8f9 	udiv	r8, fp, r9
3000c4a2:	fb09 bb18 	mls	fp, r9, r8, fp
3000c4a6:	fb08 fa03 	mul.w	sl, r8, r3
3000c4aa:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c4ae:	4592      	cmp	sl, r2
3000c4b0:	d93e      	bls.n	3000c530 <__udivmoddi4+0x29c>
3000c4b2:	18a2      	adds	r2, r4, r2
3000c4b4:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000c4b8:	d204      	bcs.n	3000c4c4 <__udivmoddi4+0x230>
3000c4ba:	4592      	cmp	sl, r2
3000c4bc:	d902      	bls.n	3000c4c4 <__udivmoddi4+0x230>
3000c4be:	f1a8 0002 	sub.w	r0, r8, #2
3000c4c2:	4422      	add	r2, r4
3000c4c4:	eba2 020a 	sub.w	r2, r2, sl
3000c4c8:	b289      	uxth	r1, r1
3000c4ca:	fbb2 f8f9 	udiv	r8, r2, r9
3000c4ce:	fb09 2218 	mls	r2, r9, r8, r2
3000c4d2:	fb08 f303 	mul.w	r3, r8, r3
3000c4d6:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c4da:	4293      	cmp	r3, r2
3000c4dc:	d92a      	bls.n	3000c534 <__udivmoddi4+0x2a0>
3000c4de:	18a2      	adds	r2, r4, r2
3000c4e0:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000c4e4:	d204      	bcs.n	3000c4f0 <__udivmoddi4+0x25c>
3000c4e6:	4293      	cmp	r3, r2
3000c4e8:	d902      	bls.n	3000c4f0 <__udivmoddi4+0x25c>
3000c4ea:	f1a8 0102 	sub.w	r1, r8, #2
3000c4ee:	4422      	add	r2, r4
3000c4f0:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c4f4:	1ad2      	subs	r2, r2, r3
3000c4f6:	fba0 9805 	umull	r9, r8, r0, r5
3000c4fa:	4542      	cmp	r2, r8
3000c4fc:	464b      	mov	r3, r9
3000c4fe:	4641      	mov	r1, r8
3000c500:	d302      	bcc.n	3000c508 <__udivmoddi4+0x274>
3000c502:	d106      	bne.n	3000c512 <__udivmoddi4+0x27e>
3000c504:	454f      	cmp	r7, r9
3000c506:	d204      	bcs.n	3000c512 <__udivmoddi4+0x27e>
3000c508:	3801      	subs	r0, #1
3000c50a:	ebb9 0305 	subs.w	r3, r9, r5
3000c50e:	eb68 0104 	sbc.w	r1, r8, r4
3000c512:	b18e      	cbz	r6, 3000c538 <__udivmoddi4+0x2a4>
3000c514:	1afc      	subs	r4, r7, r3
3000c516:	eb62 0701 	sbc.w	r7, r2, r1
3000c51a:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c51e:	fa24 f30c 	lsr.w	r3, r4, ip
3000c522:	fa27 f70c 	lsr.w	r7, r7, ip
3000c526:	ea4e 0303 	orr.w	r3, lr, r3
3000c52a:	e9c6 3700 	strd	r3, r7, [r6]
3000c52e:	e79c      	b.n	3000c46a <__udivmoddi4+0x1d6>
3000c530:	4640      	mov	r0, r8
3000c532:	e7c7      	b.n	3000c4c4 <__udivmoddi4+0x230>
3000c534:	4641      	mov	r1, r8
3000c536:	e7db      	b.n	3000c4f0 <__udivmoddi4+0x25c>
3000c538:	4631      	mov	r1, r6
3000c53a:	e6fb      	b.n	3000c334 <__udivmoddi4+0xa0>

3000c53c <__aeabi_idiv0>:
3000c53c:	4770      	bx	lr
3000c53e:	bf00      	nop

3000c540 <_mbtowc_r>:
3000c540:	b430      	push	{r4, r5}
3000c542:	4c06      	ldr	r4, [pc, #24]	; (3000c55c <_mbtowc_r+0x1c>)
3000c544:	4d06      	ldr	r5, [pc, #24]	; (3000c560 <_mbtowc_r+0x20>)
3000c546:	6824      	ldr	r4, [r4, #0]
3000c548:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c54a:	2c00      	cmp	r4, #0
3000c54c:	bf08      	it	eq
3000c54e:	462c      	moveq	r4, r5
3000c550:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000c554:	46a4      	mov	ip, r4
3000c556:	bc30      	pop	{r4, r5}
3000c558:	4760      	bx	ip
3000c55a:	bf00      	nop
3000c55c:	3000fa40 	.word	0x3000fa40
3000c560:	3000f8d4 	.word	0x3000f8d4

3000c564 <__ascii_mbtowc>:
3000c564:	b082      	sub	sp, #8
3000c566:	b901      	cbnz	r1, 3000c56a <__ascii_mbtowc+0x6>
3000c568:	a901      	add	r1, sp, #4
3000c56a:	b142      	cbz	r2, 3000c57e <__ascii_mbtowc+0x1a>
3000c56c:	b14b      	cbz	r3, 3000c582 <__ascii_mbtowc+0x1e>
3000c56e:	7813      	ldrb	r3, [r2, #0]
3000c570:	600b      	str	r3, [r1, #0]
3000c572:	7812      	ldrb	r2, [r2, #0]
3000c574:	1e10      	subs	r0, r2, #0
3000c576:	bf18      	it	ne
3000c578:	2001      	movne	r0, #1
3000c57a:	b002      	add	sp, #8
3000c57c:	4770      	bx	lr
3000c57e:	4610      	mov	r0, r2
3000c580:	e7fb      	b.n	3000c57a <__ascii_mbtowc+0x16>
3000c582:	f06f 0001 	mvn.w	r0, #1
3000c586:	e7f8      	b.n	3000c57a <__ascii_mbtowc+0x16>

3000c588 <_wctomb_r>:
3000c588:	b430      	push	{r4, r5}
3000c58a:	4c06      	ldr	r4, [pc, #24]	; (3000c5a4 <_wctomb_r+0x1c>)
3000c58c:	4d06      	ldr	r5, [pc, #24]	; (3000c5a8 <_wctomb_r+0x20>)
3000c58e:	6824      	ldr	r4, [r4, #0]
3000c590:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c592:	2c00      	cmp	r4, #0
3000c594:	bf08      	it	eq
3000c596:	462c      	moveq	r4, r5
3000c598:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000c59c:	46a4      	mov	ip, r4
3000c59e:	bc30      	pop	{r4, r5}
3000c5a0:	4760      	bx	ip
3000c5a2:	bf00      	nop
3000c5a4:	3000fa40 	.word	0x3000fa40
3000c5a8:	3000f8d4 	.word	0x3000f8d4

3000c5ac <__ascii_wctomb>:
3000c5ac:	b149      	cbz	r1, 3000c5c2 <__ascii_wctomb+0x16>
3000c5ae:	2aff      	cmp	r2, #255	; 0xff
3000c5b0:	bf8d      	iteet	hi
3000c5b2:	238a      	movhi	r3, #138	; 0x8a
3000c5b4:	2001      	movls	r0, #1
3000c5b6:	700a      	strbls	r2, [r1, #0]
3000c5b8:	6003      	strhi	r3, [r0, #0]
3000c5ba:	bf88      	it	hi
3000c5bc:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000c5c0:	4770      	bx	lr
3000c5c2:	4608      	mov	r0, r1
3000c5c4:	4770      	bx	lr
3000c5c6:	bf00      	nop

3000c5c8 <__rtl_crypto_sha2_final_A_veneer>:
3000c5c8:	f85f f000 	ldr.w	pc, [pc]	; 3000c5cc <__rtl_crypto_sha2_final_A_veneer+0x4>
3000c5cc:	0000527d 	.word	0x0000527d

3000c5d0 <__rtl_crypto_sha2_update_B_veneer>:
3000c5d0:	f85f f000 	ldr.w	pc, [pc]	; 3000c5d4 <__rtl_crypto_sha2_update_B_veneer+0x4>
3000c5d4:	00005385 	.word	0x00005385

3000c5d8 <__rtl_crypto_sha2_init_A_veneer>:
3000c5d8:	f85f f000 	ldr.w	pc, [pc]	; 3000c5dc <__rtl_crypto_sha2_init_A_veneer+0x4>
3000c5dc:	00004e89 	.word	0x00004e89

3000c5e0 <__io_assert_failed_veneer>:
3000c5e0:	f85f f000 	ldr.w	pc, [pc]	; 3000c5e4 <__io_assert_failed_veneer+0x4>
3000c5e4:	0000992d 	.word	0x0000992d

3000c5e8 <____wrap_memset_veneer>:
3000c5e8:	f85f f000 	ldr.w	pc, [pc]	; 3000c5ec <____wrap_memset_veneer+0x4>
3000c5ec:	00012efd 	.word	0x00012efd

3000c5f0 <____wrap_strlen_veneer>:
3000c5f0:	f85f f000 	ldr.w	pc, [pc]	; 3000c5f4 <____wrap_strlen_veneer+0x4>
3000c5f4:	00012f25 	.word	0x00012f25

3000c5f8 <__SBOOT_Validate_Algorithm_A_veneer>:
3000c5f8:	f85f f000 	ldr.w	pc, [pc]	; 3000c5fc <__SBOOT_Validate_Algorithm_A_veneer+0x4>
3000c5fc:	00003279 	.word	0x00003279

3000c600 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000c600:	f85f f000 	ldr.w	pc, [pc]	; 3000c604 <__rtl_crypto_hmac_sha2_final_B_veneer+0x4>
3000c604:	00005bb9 	.word	0x00005bb9

3000c608 <____wrap_memcpy_veneer>:
3000c608:	f85f f000 	ldr.w	pc, [pc]	; 3000c60c <____wrap_memcpy_veneer+0x4>
3000c60c:	00012f05 	.word	0x00012f05

3000c610 <____wrap_memcmp_veneer>:
3000c610:	f85f f000 	ldr.w	pc, [pc]	; 3000c614 <____wrap_memcmp_veneer+0x4>
3000c614:	00012f0d 	.word	0x00012f0d

3000c618 <__rtl_crypto_sha2_init_B_veneer>:
3000c618:	f85f f000 	ldr.w	pc, [pc]	; 3000c61c <__rtl_crypto_sha2_init_B_veneer+0x4>
3000c61c:	00004ffd 	.word	0x00004ffd

3000c620 <__SBOOT_Validate_Signature_B_veneer>:
3000c620:	f85f f000 	ldr.w	pc, [pc]	; 3000c624 <__SBOOT_Validate_Signature_B_veneer+0x4>
3000c624:	00003431 	.word	0x00003431

3000c628 <__SBOOT_Validate_Signature_A_veneer>:
3000c628:	f85f f000 	ldr.w	pc, [pc]	; 3000c62c <__SBOOT_Validate_Signature_A_veneer+0x4>
3000c62c:	000032bd 	.word	0x000032bd

3000c630 <__rtl_crypto_sha2_final_B_veneer>:
3000c630:	f85f f000 	ldr.w	pc, [pc]	; 3000c634 <__rtl_crypto_sha2_final_B_veneer+0x4>
3000c634:	000053f1 	.word	0x000053f1

3000c638 <__SBOOT_Validate_ImgHash_A_veneer>:
3000c638:	f85f f000 	ldr.w	pc, [pc]	; 3000c63c <__SBOOT_Validate_ImgHash_A_veneer+0x4>
3000c63c:	000033cd 	.word	0x000033cd

3000c640 <__SBOOT_Validate_ImgHash_B_veneer>:
3000c640:	f85f f000 	ldr.w	pc, [pc]	; 3000c644 <__SBOOT_Validate_ImgHash_B_veneer+0x4>
3000c644:	00003541 	.word	0x00003541

3000c648 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000c648:	f85f f000 	ldr.w	pc, [pc]	; 3000c64c <__rtl_crypto_hmac_sha2_update_B_veneer+0x4>
3000c64c:	00005b4d 	.word	0x00005b4d

3000c650 <__INT_HardFault_C_veneer>:
3000c650:	f85f f000 	ldr.w	pc, [pc]	; 3000c654 <__INT_HardFault_C_veneer+0x4>
3000c654:	0000d8ed 	.word	0x0000d8ed

3000c658 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000c658:	f85f f000 	ldr.w	pc, [pc]	; 3000c65c <__rtl_crypto_hmac_sha2_update_A_veneer+0x4>
3000c65c:	000059d9 	.word	0x000059d9

3000c660 <__SBOOT_Validate_Algorithm_B_veneer>:
3000c660:	f85f f000 	ldr.w	pc, [pc]	; 3000c664 <__SBOOT_Validate_Algorithm_B_veneer+0x4>
3000c664:	000033ed 	.word	0x000033ed

3000c668 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000c668:	f85f f000 	ldr.w	pc, [pc]	; 3000c66c <__CRYPTO_OTPKey_SHA_Init_A_veneer+0x4>
3000c66c:	00002369 	.word	0x00002369

3000c670 <__CRYPTO_SHA_Init_A_veneer>:
3000c670:	f85f f000 	ldr.w	pc, [pc]	; 3000c674 <__CRYPTO_SHA_Init_A_veneer+0x4>
3000c674:	000022dd 	.word	0x000022dd

3000c678 <__rtl_crypto_sha2_update_A_veneer>:
3000c678:	f85f f000 	ldr.w	pc, [pc]	; 3000c67c <__rtl_crypto_sha2_update_A_veneer+0x4>
3000c67c:	00005211 	.word	0x00005211

3000c680 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000c680:	f85f f000 	ldr.w	pc, [pc]	; 3000c684 <__CRYPTO_OTPKey_SHA_Init_B_veneer+0x4>
3000c684:	000024dd 	.word	0x000024dd

3000c688 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000c688:	f85f f000 	ldr.w	pc, [pc]	; 3000c68c <__rtl_crypto_hmac_sha2_final_A_veneer+0x4>
3000c68c:	00005a45 	.word	0x00005a45

3000c690 <__SBOOT_Validate_PubKey_A_veneer>:
3000c690:	f85f f000 	ldr.w	pc, [pc]	; 3000c694 <__SBOOT_Validate_PubKey_A_veneer+0x4>
3000c694:	00003321 	.word	0x00003321

3000c698 <__DiagVSprintf_veneer>:
3000c698:	f85f f000 	ldr.w	pc, [pc]	; 3000c69c <__DiagVSprintf_veneer+0x4>
3000c69c:	0000e331 	.word	0x0000e331

3000c6a0 <__DiagPrintf_veneer>:
3000c6a0:	f85f f000 	ldr.w	pc, [pc]	; 3000c6a4 <__DiagPrintf_veneer+0x4>
3000c6a4:	0000e5f9 	.word	0x0000e5f9

3000c6a8 <__SBOOT_GetMdType_B_veneer>:
3000c6a8:	f85f f000 	ldr.w	pc, [pc]	; 3000c6ac <__SBOOT_GetMdType_B_veneer+0x4>
3000c6ac:	0000319d 	.word	0x0000319d

3000c6b0 <__SBOOT_GetMdType_A_veneer>:
3000c6b0:	f85f f000 	ldr.w	pc, [pc]	; 3000c6b4 <__SBOOT_GetMdType_A_veneer+0x4>
3000c6b4:	00003029 	.word	0x00003029

3000c6b8 <__CRYPTO_SHA_Init_B_veneer>:
3000c6b8:	f85f f000 	ldr.w	pc, [pc]	; 3000c6bc <__CRYPTO_SHA_Init_B_veneer+0x4>
3000c6bc:	00002451 	.word	0x00002451

3000c6c0 <__SBOOT_Validate_PubKey_B_veneer>:
3000c6c0:	f85f f000 	ldr.w	pc, [pc]	; 3000c6c4 <__SBOOT_Validate_PubKey_B_veneer+0x4>
3000c6c4:	00003495 	.word	0x00003495
3000c6c8:	004f4931 	.word	0x004f4931
3000c6cc:	00004f32 	.word	0x00004f32
3000c6d0:	004f4932 	.word	0x004f4932
3000c6d4:	00004f34 	.word	0x00004f34
3000c6d8:	004f4934 	.word	0x004f4934
3000c6dc:	53414c46 	.word	0x53414c46
3000c6e0:	4b4c4348 	.word	0x4b4c4348
3000c6e4:	00000000 	.word	0x00000000
3000c6e8:	2d73255b 	.word	0x2d73255b
3000c6ec:	63205d49 	.word	0x63205d49
3000c6f0:	62696c61 	.word	0x62696c61
3000c6f4:	69746172 	.word	0x69746172
3000c6f8:	6f5f6e6f 	.word	0x6f5f6e6f
3000c6fc:	255b3a6b 	.word	0x255b3a6b
3000c700:	64253a64 	.word	0x64253a64
3000c704:	5d64253a 	.word	0x5d64253a
3000c708:	00000a20 	.word	0x00000a20
3000c70c:	2d73255b 	.word	0x2d73255b
3000c710:	46205d49 	.word	0x46205d49
3000c714:	4853414c 	.word	0x4853414c
3000c718:	4c414320 	.word	0x4c414320
3000c71c:	305b4249 	.word	0x305b4249
3000c720:	20782578 	.word	0x20782578
3000c724:	0a5d4b4f 	.word	0x0a5d4b4f
3000c728:	00000000 	.word	0x00000000
3000c72c:	2d73255b 	.word	0x2d73255b
3000c730:	46205d45 	.word	0x46205d45
3000c734:	4853414c 	.word	0x4853414c
3000c738:	4c414320 	.word	0x4c414320
3000c73c:	305b4249 	.word	0x305b4249
3000c740:	20782578 	.word	0x20782578
3000c744:	4c494146 	.word	0x4c494146
3000c748:	00000a5d 	.word	0x00000a5d
3000c74c:	2d73255b 	.word	0x2d73255b
3000c750:	46205d49 	.word	0x46205d49
3000c754:	6873616c 	.word	0x6873616c
3000c758:	61655220 	.word	0x61655220
3000c75c:	73252064 	.word	0x73252064
3000c760:	0000000a 	.word	0x0000000a
3000c764:	2d73255b 	.word	0x2d73255b
3000c768:	46205d45 	.word	0x46205d45
3000c76c:	6873616c 	.word	0x6873616c
3000c770:	69775320 	.word	0x69775320
3000c774:	20686374 	.word	0x20686374
3000c778:	64616552 	.word	0x64616552
3000c77c:	646f4d20 	.word	0x646f4d20
3000c780:	41462065 	.word	0x41462065
3000c784:	000a4c49 	.word	0x000a4c49
3000c788:	2d73255b 	.word	0x2d73255b
3000c78c:	46205d49 	.word	0x46205d49
3000c790:	6873616c 	.word	0x6873616c
3000c794:	3a444920 	.word	0x3a444920
3000c798:	2d782520 	.word	0x2d782520
3000c79c:	252d7825 	.word	0x252d7825
3000c7a0:	00000a78 	.word	0x00000a78
3000c7a4:	2d73255b 	.word	0x2d73255b
3000c7a8:	54205d57 	.word	0x54205d57
3000c7ac:	20736968 	.word	0x20736968
3000c7b0:	73616c66 	.word	0x73616c66
3000c7b4:	79742068 	.word	0x79742068
3000c7b8:	69206570 	.word	0x69206570
3000c7bc:	6f6e2073 	.word	0x6f6e2073
3000c7c0:	75732074 	.word	0x75732074
3000c7c4:	726f7070 	.word	0x726f7070
3000c7c8:	21646574 	.word	0x21646574
3000c7cc:	0000000a 	.word	0x0000000a
3000c7d0:	2d73255b 	.word	0x2d73255b
3000c7d4:	4e205d49 	.word	0x4e205d49
3000c7d8:	20444e41 	.word	0x20444e41
3000c7dc:	203a4449 	.word	0x203a4449
3000c7e0:	252d7825 	.word	0x252d7825
3000c7e4:	00000a78 	.word	0x00000a78
3000c7e8:	00003552 	.word	0x00003552
3000c7ec:	00003652 	.word	0x00003652
3000c7f0:	00003752 	.word	0x00003752
3000c7f4:	00003852 	.word	0x00003852
3000c7f8:	00003952 	.word	0x00003952
3000c7fc:	00303152 	.word	0x00303152
3000c800:	00313152 	.word	0x00313152
3000c804:	00003152 	.word	0x00003152
3000c808:	00003252 	.word	0x00003252
3000c80c:	00003352 	.word	0x00003352
3000c810:	544f4f42 	.word	0x544f4f42
3000c814:	00000000 	.word	0x00000000
3000c818:	2d73255b 	.word	0x2d73255b
3000c81c:	48205d41 	.word	0x48205d41
3000c820:	20647261 	.word	0x20647261
3000c824:	6c756146 	.word	0x6c756146
3000c828:	61502074 	.word	0x61502074
3000c82c:	20686374 	.word	0x20686374
3000c830:	63655328 	.word	0x63655328
3000c834:	29657275 	.word	0x29657275
3000c838:	00000a0d 	.word	0x00000a0d
3000c83c:	2d73255b 	.word	0x2d73255b
3000c840:	45205d41 	.word	0x45205d41
3000c844:	70656378 	.word	0x70656378
3000c848:	6e6f6974 	.word	0x6e6f6974
3000c84c:	6f726620 	.word	0x6f726620
3000c850:	6573206d 	.word	0x6573206d
3000c854:	65727563 	.word	0x65727563
3000c858:	726f7720 	.word	0x726f7720
3000c85c:	0d21646c 	.word	0x0d21646c
3000c860:	0000000a 	.word	0x0000000a
3000c864:	2d73255b 	.word	0x2d73255b
3000c868:	45205d41 	.word	0x45205d41
3000c86c:	70656378 	.word	0x70656378
3000c870:	6e6f6974 	.word	0x6e6f6974
3000c874:	6f726620 	.word	0x6f726620
3000c878:	6f6e206d 	.word	0x6f6e206d
3000c87c:	65732d6e 	.word	0x65732d6e
3000c880:	65727563 	.word	0x65727563
3000c884:	726f7720 	.word	0x726f7720
3000c888:	0d21646c 	.word	0x0d21646c
3000c88c:	0000000a 	.word	0x0000000a
3000c890:	2d73255b 	.word	0x2d73255b
3000c894:	53205d41 	.word	0x53205d41
3000c898:	20525346 	.word	0x20525346
3000c89c:	30203d20 	.word	0x30203d20
3000c8a0:	38302578 	.word	0x38302578
3000c8a4:	000a0d78 	.word	0x000a0d78
3000c8a8:	2d73255b 	.word	0x2d73255b
3000c8ac:	53205d41 	.word	0x53205d41
3000c8b0:	20524146 	.word	0x20524146
3000c8b4:	30203d20 	.word	0x30203d20
3000c8b8:	38302578 	.word	0x38302578
3000c8bc:	000a0d78 	.word	0x000a0d78
3000c8c0:	2d73255b 	.word	0x2d73255b
3000c8c4:	53205d41 	.word	0x53205d41
3000c8c8:	52534348 	.word	0x52534348
3000c8cc:	30203d20 	.word	0x30203d20
3000c8d0:	38302578 	.word	0x38302578
3000c8d4:	000a0d78 	.word	0x000a0d78
3000c8d8:	3141544f 	.word	0x3141544f
3000c8dc:	45535520 	.word	0x45535520
3000c8e0:	6576202c 	.word	0x6576202c
3000c8e4:	6f697372 	.word	0x6f697372
3000c8e8:	25203a6e 	.word	0x25203a6e
3000c8ec:	00000a64 	.word	0x00000a64
3000c8f0:	3241544f 	.word	0x3241544f
3000c8f4:	45535520 	.word	0x45535520
3000c8f8:	6576202c 	.word	0x6576202c
3000c8fc:	6f697372 	.word	0x6f697372
3000c900:	25203a6e 	.word	0x25203a6e
3000c904:	00000a64 	.word	0x00000a64
3000c908:	68746f42 	.word	0x68746f42
3000c90c:	41544f20 	.word	0x41544f20
3000c910:	616d6920 	.word	0x616d6920
3000c914:	76206567 	.word	0x76206567
3000c918:	66697265 	.word	0x66697265
3000c91c:	61662079 	.word	0x61662079
3000c920:	64656c69 	.word	0x64656c69
3000c924:	0000000a 	.word	0x0000000a
3000c928:	2d73255b 	.word	0x2d73255b
3000c92c:	49205d49 	.word	0x49205d49
3000c930:	2074696e 	.word	0x2074696e
3000c934:	0a0d4257 	.word	0x0a0d4257
3000c938:	00000000 	.word	0x00000000
3000c93c:	2d73255b 	.word	0x2d73255b
3000c940:	52205d49 	.word	0x52205d49
3000c944:	696e4965 	.word	0x696e4965
3000c948:	44442074 	.word	0x44442074
3000c94c:	000a0d52 	.word	0x000a0d52
3000c950:	73616c66 	.word	0x73616c66
3000c954:	69735f68 	.word	0x69735f68
3000c958:	203a657a 	.word	0x203a657a
3000c95c:	000a4d38 	.word	0x000a4d38
3000c960:	73616c66 	.word	0x73616c66
3000c964:	69735f68 	.word	0x69735f68
3000c968:	203a657a 	.word	0x203a657a
3000c96c:	0a4d3631 	.word	0x0a4d3631
3000c970:	00000000 	.word	0x00000000
3000c974:	20315042 	.word	0x20315042
3000c978:	73726576 	.word	0x73726576
3000c97c:	206e6f69 	.word	0x206e6f69
3000c980:	61766e69 	.word	0x61766e69
3000c984:	0a64696c 	.word	0x0a64696c
3000c988:	00000000 	.word	0x00000000
3000c98c:	20315042 	.word	0x20315042
3000c990:	6d726f66 	.word	0x6d726f66
3000c994:	69207461 	.word	0x69207461
3000c998:	6c61766e 	.word	0x6c61766e
3000c99c:	000a6469 	.word	0x000a6469
3000c9a0:	20315042 	.word	0x20315042
3000c9a4:	69746361 	.word	0x69746361
3000c9a8:	69206576 	.word	0x69206576
3000c9ac:	7865646e 	.word	0x7865646e
3000c9b0:	766e6920 	.word	0x766e6920
3000c9b4:	64696c61 	.word	0x64696c61
3000c9b8:	0000000a 	.word	0x0000000a
3000c9bc:	20315042 	.word	0x20315042
3000c9c0:	61746164 	.word	0x61746164
3000c9c4:	6c617620 	.word	0x6c617620
3000c9c8:	202c6469 	.word	0x202c6469
3000c9cc:	73726576 	.word	0x73726576
3000c9d0:	3a6e6f69 	.word	0x3a6e6f69
3000c9d4:	000a6425 	.word	0x000a6425
3000c9d8:	20325042 	.word	0x20325042
3000c9dc:	6d726f66 	.word	0x6d726f66
3000c9e0:	69207461 	.word	0x69207461
3000c9e4:	6c61766e 	.word	0x6c61766e
3000c9e8:	000a6469 	.word	0x000a6469
3000c9ec:	20315042 	.word	0x20315042
3000c9f0:	33435243 	.word	0x33435243
3000c9f4:	616d2032 	.word	0x616d2032
3000c9f8:	2c686374 	.word	0x2c686374
3000c9fc:	74746120 	.word	0x74746120
3000ca00:	65686361 	.word	0x65686361
3000ca04:	52432064 	.word	0x52432064
3000ca08:	30203a43 	.word	0x30203a43
3000ca0c:	2c782578 	.word	0x2c782578
3000ca10:	6c616320 	.word	0x6c616320
3000ca14:	616c7563 	.word	0x616c7563
3000ca18:	20646574 	.word	0x20646574
3000ca1c:	3a435243 	.word	0x3a435243
3000ca20:	25783020 	.word	0x25783020
3000ca24:	00000a78 	.word	0x00000a78
3000ca28:	20315042 	.word	0x20315042
3000ca2c:	33435243 	.word	0x33435243
3000ca30:	6f6e2032 	.word	0x6f6e2032
3000ca34:	616d2074 	.word	0x616d2074
3000ca38:	2c686374 	.word	0x2c686374
3000ca3c:	74746120 	.word	0x74746120
3000ca40:	65686361 	.word	0x65686361
3000ca44:	52432064 	.word	0x52432064
3000ca48:	30203a43 	.word	0x30203a43
3000ca4c:	2c782578 	.word	0x2c782578
3000ca50:	6c616320 	.word	0x6c616320
3000ca54:	616c7563 	.word	0x616c7563
3000ca58:	20646574 	.word	0x20646574
3000ca5c:	3a435243 	.word	0x3a435243
3000ca60:	25783020 	.word	0x25783020
3000ca64:	00000a78 	.word	0x00000a78
3000ca68:	20325042 	.word	0x20325042
3000ca6c:	33435243 	.word	0x33435243
3000ca70:	616d2032 	.word	0x616d2032
3000ca74:	2c686374 	.word	0x2c686374
3000ca78:	74746120 	.word	0x74746120
3000ca7c:	65686361 	.word	0x65686361
3000ca80:	52432064 	.word	0x52432064
3000ca84:	30203a43 	.word	0x30203a43
3000ca88:	2c782578 	.word	0x2c782578
3000ca8c:	6c616320 	.word	0x6c616320
3000ca90:	616c7563 	.word	0x616c7563
3000ca94:	20646574 	.word	0x20646574
3000ca98:	3a435243 	.word	0x3a435243
3000ca9c:	25783020 	.word	0x25783020
3000caa0:	00000a78 	.word	0x00000a78
3000caa4:	20325042 	.word	0x20325042
3000caa8:	33435243 	.word	0x33435243
3000caac:	6f6e2032 	.word	0x6f6e2032
3000cab0:	616d2074 	.word	0x616d2074
3000cab4:	2c686374 	.word	0x2c686374
3000cab8:	74746120 	.word	0x74746120
3000cabc:	65686361 	.word	0x65686361
3000cac0:	52432064 	.word	0x52432064
3000cac4:	30203a43 	.word	0x30203a43
3000cac8:	2c782578 	.word	0x2c782578
3000cacc:	6c616320 	.word	0x6c616320
3000cad0:	616c7563 	.word	0x616c7563
3000cad4:	20646574 	.word	0x20646574
3000cad8:	3a435243 	.word	0x3a435243
3000cadc:	25783020 	.word	0x25783020
3000cae0:	00000a78 	.word	0x00000a78
3000cae4:	20315042 	.word	0x20315042
3000cae8:	3161746f 	.word	0x3161746f
3000caec:	64646120 	.word	0x64646120
3000caf0:	73736572 	.word	0x73736572
3000caf4:	766e6920 	.word	0x766e6920
3000caf8:	64696c61 	.word	0x64696c61
3000cafc:	0000000a 	.word	0x0000000a
3000cb00:	20315042 	.word	0x20315042
3000cb04:	3261746f 	.word	0x3261746f
3000cb08:	64646120 	.word	0x64646120
3000cb0c:	73736572 	.word	0x73736572
3000cb10:	766e6920 	.word	0x766e6920
3000cb14:	64696c61 	.word	0x64696c61
3000cb18:	0000000a 	.word	0x0000000a
3000cb1c:	68746f42 	.word	0x68746f42
3000cb20:	20504220 	.word	0x20504220
3000cb24:	20435243 	.word	0x20435243
3000cb28:	696c6176 	.word	0x696c6176
3000cb2c:	00000a64 	.word	0x00000a64
3000cb30:	20315042 	.word	0x20315042
3000cb34:	656c6573 	.word	0x656c6573
3000cb38:	64657463 	.word	0x64657463
3000cb3c:	0000000a 	.word	0x0000000a
3000cb40:	20325042 	.word	0x20325042
3000cb44:	656c6573 	.word	0x656c6573
3000cb48:	64657463 	.word	0x64657463
3000cb4c:	0000000a 	.word	0x0000000a
3000cb50:	68746f42 	.word	0x68746f42
3000cb54:	20504220 	.word	0x20504220
3000cb58:	61766e69 	.word	0x61766e69
3000cb5c:	0a64696c 	.word	0x0a64696c
3000cb60:	00000000 	.word	0x00000000
3000cb64:	6f636552 	.word	0x6f636552
3000cb68:	79726576 	.word	0x79726576
3000cb6c:	61747320 	.word	0x61747320
3000cb70:	000a7472 	.word	0x000a7472
3000cb74:	69746341 	.word	0x69746341
3000cb78:	49206576 	.word	0x49206576
3000cb7c:	7865646e 	.word	0x7865646e
3000cb80:	6425203a 	.word	0x6425203a
3000cb84:	0000000a 	.word	0x0000000a
3000cb88:	3141544f 	.word	0x3141544f
3000cb8c:	766e6920 	.word	0x766e6920
3000cb90:	64696c61 	.word	0x64696c61
3000cb94:	0000000a 	.word	0x0000000a
3000cb98:	3241544f 	.word	0x3241544f
3000cb9c:	766e6920 	.word	0x766e6920
3000cba0:	64696c61 	.word	0x64696c61
3000cba4:	0000000a 	.word	0x0000000a
3000cba8:	3141544f 	.word	0x3141544f
3000cbac:	43524320 	.word	0x43524320
3000cbb0:	6d203233 	.word	0x6d203233
3000cbb4:	68637461 	.word	0x68637461
3000cbb8:	7461202c 	.word	0x7461202c
3000cbbc:	68636174 	.word	0x68636174
3000cbc0:	43206465 	.word	0x43206465
3000cbc4:	203a4352 	.word	0x203a4352
3000cbc8:	78257830 	.word	0x78257830
3000cbcc:	6163202c 	.word	0x6163202c
3000cbd0:	6c75636c 	.word	0x6c75636c
3000cbd4:	64657461 	.word	0x64657461
3000cbd8:	43524320 	.word	0x43524320
3000cbdc:	7830203a 	.word	0x7830203a
3000cbe0:	000a7825 	.word	0x000a7825
3000cbe4:	3141544f 	.word	0x3141544f
3000cbe8:	43524320 	.word	0x43524320
3000cbec:	6e203233 	.word	0x6e203233
3000cbf0:	6d20746f 	.word	0x6d20746f
3000cbf4:	68637461 	.word	0x68637461
3000cbf8:	7461202c 	.word	0x7461202c
3000cbfc:	68636174 	.word	0x68636174
3000cc00:	43206465 	.word	0x43206465
3000cc04:	203a4352 	.word	0x203a4352
3000cc08:	78257830 	.word	0x78257830
3000cc0c:	6163202c 	.word	0x6163202c
3000cc10:	6c75636c 	.word	0x6c75636c
3000cc14:	64657461 	.word	0x64657461
3000cc18:	43524320 	.word	0x43524320
3000cc1c:	7830203a 	.word	0x7830203a
3000cc20:	202c7825 	.word	0x202c7825
3000cc24:	73617265 	.word	0x73617265
3000cc28:	544f2065 	.word	0x544f2065
3000cc2c:	000a3141 	.word	0x000a3141
3000cc30:	3241544f 	.word	0x3241544f
3000cc34:	43524320 	.word	0x43524320
3000cc38:	6d203233 	.word	0x6d203233
3000cc3c:	68637461 	.word	0x68637461
3000cc40:	7461202c 	.word	0x7461202c
3000cc44:	68636174 	.word	0x68636174
3000cc48:	43206465 	.word	0x43206465
3000cc4c:	203a4352 	.word	0x203a4352
3000cc50:	78257830 	.word	0x78257830
3000cc54:	6163202c 	.word	0x6163202c
3000cc58:	6c75636c 	.word	0x6c75636c
3000cc5c:	64657461 	.word	0x64657461
3000cc60:	43524320 	.word	0x43524320
3000cc64:	7830203a 	.word	0x7830203a
3000cc68:	000a7825 	.word	0x000a7825
3000cc6c:	3241544f 	.word	0x3241544f
3000cc70:	43524320 	.word	0x43524320
3000cc74:	6e203233 	.word	0x6e203233
3000cc78:	6d20746f 	.word	0x6d20746f
3000cc7c:	68637461 	.word	0x68637461
3000cc80:	7461202c 	.word	0x7461202c
3000cc84:	68636174 	.word	0x68636174
3000cc88:	43206465 	.word	0x43206465
3000cc8c:	203a4352 	.word	0x203a4352
3000cc90:	78257830 	.word	0x78257830
3000cc94:	6163202c 	.word	0x6163202c
3000cc98:	6c75636c 	.word	0x6c75636c
3000cc9c:	64657461 	.word	0x64657461
3000cca0:	43524320 	.word	0x43524320
3000cca4:	7830203a 	.word	0x7830203a
3000cca8:	202c7825 	.word	0x202c7825
3000ccac:	73617265 	.word	0x73617265
3000ccb0:	544f2065 	.word	0x544f2065
3000ccb4:	000a3241 	.word	0x000a3241
3000ccb8:	69746341 	.word	0x69746341
3000ccbc:	69206576 	.word	0x69206576
3000ccc0:	7865646e 	.word	0x7865646e
3000ccc4:	41544f20 	.word	0x41544f20
3000ccc8:	766e6920 	.word	0x766e6920
3000cccc:	64696c61 	.word	0x64696c61
3000ccd0:	6572202c 	.word	0x6572202c
3000ccd4:	74697277 	.word	0x74697277
3000ccd8:	63612065 	.word	0x63612065
3000ccdc:	65766974 	.word	0x65766974
3000cce0:	646e6920 	.word	0x646e6920
3000cce4:	74207865 	.word	0x74207865
3000cce8:	6425206f 	.word	0x6425206f
3000ccec:	0000000a 	.word	0x0000000a
3000ccf0:	206c6c41 	.word	0x206c6c41
3000ccf4:	2041544f 	.word	0x2041544f
3000ccf8:	72646461 	.word	0x72646461
3000ccfc:	65737365 	.word	0x65737365
3000cd00:	6f6e2073 	.word	0x6f6e2073
3000cd04:	61762074 	.word	0x61762074
3000cd08:	2c64696c 	.word	0x2c64696c
3000cd0c:	63657220 	.word	0x63657220
3000cd10:	7265766f 	.word	0x7265766f
3000cd14:	61662079 	.word	0x61662079
3000cd18:	000a6c69 	.word	0x000a6c69
3000cd1c:	20325042 	.word	0x20325042
3000cd20:	3261746f 	.word	0x3261746f
3000cd24:	64646120 	.word	0x64646120
3000cd28:	73736572 	.word	0x73736572
3000cd2c:	766e6920 	.word	0x766e6920
3000cd30:	64696c61 	.word	0x64696c61
3000cd34:	0000000a 	.word	0x0000000a
3000cd38:	20325042 	.word	0x20325042
3000cd3c:	73726576 	.word	0x73726576
3000cd40:	206e6f69 	.word	0x206e6f69
3000cd44:	61766e69 	.word	0x61766e69
3000cd48:	0a64696c 	.word	0x0a64696c
3000cd4c:	00000000 	.word	0x00000000
3000cd50:	20325042 	.word	0x20325042
3000cd54:	69746361 	.word	0x69746361
3000cd58:	69206576 	.word	0x69206576
3000cd5c:	7865646e 	.word	0x7865646e
3000cd60:	766e6920 	.word	0x766e6920
3000cd64:	64696c61 	.word	0x64696c61
3000cd68:	0000000a 	.word	0x0000000a
3000cd6c:	20325042 	.word	0x20325042
3000cd70:	3161746f 	.word	0x3161746f
3000cd74:	64646120 	.word	0x64646120
3000cd78:	73736572 	.word	0x73736572
3000cd7c:	766e6920 	.word	0x766e6920
3000cd80:	64696c61 	.word	0x64696c61
3000cd84:	0000000a 	.word	0x0000000a
3000cd88:	20325042 	.word	0x20325042
3000cd8c:	61746164 	.word	0x61746164
3000cd90:	6c617620 	.word	0x6c617620
3000cd94:	202c6469 	.word	0x202c6469
3000cd98:	73726576 	.word	0x73726576
3000cd9c:	3a6e6f69 	.word	0x3a6e6f69
3000cda0:	000a6425 	.word	0x000a6425
3000cda4:	2d73255b 	.word	0x2d73255b
3000cda8:	44205d49 	.word	0x44205d49
3000cdac:	62617369 	.word	0x62617369
3000cdb0:	4b20656c 	.word	0x4b20656c
3000cdb4:	4220304d 	.word	0x4220304d
3000cdb8:	20746f6f 	.word	0x20746f6f
3000cdbc:	206e6920 	.word	0x206e6920
3000cdc0:	74736554 	.word	0x74736554
3000cdc4:	646f6d20 	.word	0x646f6d20
3000cdc8:	000a0d65 	.word	0x000a0d65
3000cdcc:	2d73255b 	.word	0x2d73255b
3000cdd0:	42205d49 	.word	0x42205d49
3000cdd4:	73617079 	.word	0x73617079
3000cdd8:	544f2073 	.word	0x544f2073
3000cddc:	75612050 	.word	0x75612050
3000cde0:	6f6c6f74 	.word	0x6f6c6f74
3000cde4:	0a0d6461 	.word	0x0a0d6461
3000cde8:	00000000 	.word	0x00000000
3000cdec:	2d73255b 	.word	0x2d73255b
3000cdf0:	4e205d49 	.word	0x4e205d49
3000cdf4:	72462050 	.word	0x72462050
3000cdf8:	25207165 	.word	0x25207165
3000cdfc:	484d2064 	.word	0x484d2064
3000ce00:	00000a7a 	.word	0x00000a7a
3000ce04:	2d73255b 	.word	0x2d73255b
3000ce08:	41205d49 	.word	0x41205d49
3000ce0c:	72462050 	.word	0x72462050
3000ce10:	25207165 	.word	0x25207165
3000ce14:	484d2064 	.word	0x484d2064
3000ce18:	00000a7a 	.word	0x00000a7a
3000ce1c:	7361700a 	.word	0x7361700a
3000ce20:	726f7773 	.word	0x726f7773
3000ce24:	65762064 	.word	0x65762064
3000ce28:	79666972 	.word	0x79666972
3000ce2c:	69616620 	.word	0x69616620
3000ce30:	00000a6c 	.word	0x00000a6c
3000ce34:	7361700a 	.word	0x7361700a
3000ce38:	726f7773 	.word	0x726f7773
3000ce3c:	65762064 	.word	0x65762064
3000ce40:	79666972 	.word	0x79666972
3000ce44:	63757320 	.word	0x63757320
3000ce48:	73736563 	.word	0x73736563
3000ce4c:	0000000a 	.word	0x0000000a
3000ce50:	31474d49 	.word	0x31474d49
3000ce54:	4f425320 	.word	0x4f425320
3000ce58:	4f20544f 	.word	0x4f20544f
3000ce5c:	000a4646 	.word	0x000a4646
3000ce60:	31474d49 	.word	0x31474d49
3000ce64:	4f425320 	.word	0x4f425320
3000ce68:	5320544f 	.word	0x5320544f
3000ce6c:	45434355 	.word	0x45434355
3000ce70:	000a5353 	.word	0x000a5353
3000ce74:	746f6f62 	.word	0x746f6f62
3000ce78:	64616f6c 	.word	0x64616f6c
3000ce7c:	765f7265 	.word	0x765f7265
3000ce80:	69737265 	.word	0x69737265
3000ce84:	25206e6f 	.word	0x25206e6f
3000ce88:	00000a73 	.word	0x00000a73
3000ce8c:	444f4d5b 	.word	0x444f4d5b
3000ce90:	5f454c55 	.word	0x5f454c55
3000ce94:	544f4f42 	.word	0x544f4f42
3000ce98:	56454c2d 	.word	0x56454c2d
3000ce9c:	495f4c45 	.word	0x495f4c45
3000cea0:	5d4f464e 	.word	0x5d4f464e
3000cea4:	474d493a 	.word	0x474d493a
3000cea8:	4e452031 	.word	0x4e452031
3000ceac:	20524554 	.word	0x20524554
3000ceb0:	3a50534d 	.word	0x3a50534d
3000ceb4:	3830255b 	.word	0x3830255b
3000ceb8:	000a5d78 	.word	0x000a5d78
3000cebc:	444f4d5b 	.word	0x444f4d5b
3000cec0:	5f454c55 	.word	0x5f454c55
3000cec4:	544f4f42 	.word	0x544f4f42
3000cec8:	56454c2d 	.word	0x56454c2d
3000cecc:	495f4c45 	.word	0x495f4c45
3000ced0:	5d4f464e 	.word	0x5d4f464e
3000ced4:	474d493a 	.word	0x474d493a
3000ced8:	45532031 	.word	0x45532031
3000cedc:	45525543 	.word	0x45525543
3000cee0:	41545320 	.word	0x41545320
3000cee4:	203a4554 	.word	0x203a4554
3000cee8:	000a6425 	.word	0x000a6425
3000ceec:	2d73255b 	.word	0x2d73255b
3000cef0:	49205d49 	.word	0x49205d49
3000cef4:	2074696e 	.word	0x2074696e
3000cef8:	41525350 	.word	0x41525350
3000cefc:	000a0d4d 	.word	0x000a0d4d
3000cf00:	2d73255b 	.word	0x2d73255b
3000cf04:	73205d57 	.word	0x73205d57
3000cf08:	70207465 	.word	0x70207465
3000cf0c:	66206d66 	.word	0x66206d66
3000cf10:	0d6c6961 	.word	0x0d6c6961
3000cf14:	0000000a 	.word	0x0000000a
3000cf18:	2d73255b 	.word	0x2d73255b
3000cf1c:	45205d45 	.word	0x45205d45
3000cf20:	524f5252 	.word	0x524f5252
3000cf24:	53202121 	.word	0x53202121
3000cf28:	6c756f68 	.word	0x6c756f68
3000cf2c:	6f4e2064 	.word	0x6f4e2064
3000cf30:	6e652074 	.word	0x6e652074
3000cf34:	656c6261 	.word	0x656c6261
3000cf38:	6d654d20 	.word	0x6d654d20
3000cf3c:	5f727753 	.word	0x5f727753
3000cf40:	796c6e4f 	.word	0x796c6e4f
3000cf44:	206e6920 	.word	0x206e6920
3000cf48:	20524444 	.word	0x20524444
3000cf4c:	70696843 	.word	0x70696843
3000cf50:	0d212121 	.word	0x0d212121
3000cf54:	0000000a 	.word	0x0000000a
3000cf58:	444f4d5b 	.word	0x444f4d5b
3000cf5c:	5f454c55 	.word	0x5f454c55
3000cf60:	544f4f42 	.word	0x544f4f42
3000cf64:	56454c2d 	.word	0x56454c2d
3000cf68:	495f4c45 	.word	0x495f4c45
3000cf6c:	5d4f464e 	.word	0x5d4f464e
3000cf70:	696e493a 	.word	0x696e493a
3000cf74:	44442074 	.word	0x44442074
3000cf78:	000a0d52 	.word	0x000a0d52
3000cf7c:	20445753 	.word	0x20445753
3000cf80:	64207369 	.word	0x64207369
3000cf84:	62617369 	.word	0x62617369
3000cf88:	2c64656c 	.word	0x2c64656c
3000cf8c:	6f727020 	.word	0x6f727020
3000cf90:	64656563 	.word	0x64656563
3000cf94:	206f7420 	.word	0x206f7420
3000cf98:	61736964 	.word	0x61736964
3000cf9c:	20656c62 	.word	0x20656c62
3000cfa0:	74726155 	.word	0x74726155
3000cfa4:	776f4420 	.word	0x776f4420
3000cfa8:	616f6c6e 	.word	0x616f6c6e
3000cfac:	2e2e2e64 	.word	0x2e2e2e64
3000cfb0:	0000000a 	.word	0x0000000a
3000cfb4:	74726155 	.word	0x74726155
3000cfb8:	776f4420 	.word	0x776f4420
3000cfbc:	616f6c6e 	.word	0x616f6c6e
3000cfc0:	61682064 	.word	0x61682064
3000cfc4:	65622073 	.word	0x65622073
3000cfc8:	64206e65 	.word	0x64206e65
3000cfcc:	62617369 	.word	0x62617369
3000cfd0:	0a64656c 	.word	0x0a64656c
3000cfd4:	00000000 	.word	0x00000000
3000cfd8:	55676f4c 	.word	0x55676f4c
3000cfdc:	20747261 	.word	0x20747261
3000cfe0:	64756142 	.word	0x64756142
3000cfe4:	65746172 	.word	0x65746172
3000cfe8:	6425203a 	.word	0x6425203a
3000cfec:	0000000a 	.word	0x0000000a
3000cff0:	4848530a 	.word	0x4848530a
3000cff4:	4946544f 	.word	0x4946544f
3000cff8:	6f6e2058 	.word	0x6f6e2058
3000cffc:	65722074 	.word	0x65722074
3000d000:	76696563 	.word	0x76696563
3000d004:	000a6465 	.word	0x000a6465
3000d008:	726f4e0a 	.word	0x726f4e0a
3000d00c:	206c616d 	.word	0x206c616d
3000d010:	746f6f62 	.word	0x746f6f62
3000d014:	0000000a 	.word	0x0000000a
3000d018:	5f6e6962 	.word	0x5f6e6962
3000d01c:	313e6f6e 	.word	0x313e6f6e
3000d020:	3d202c30 	.word	0x3d202c30
3000d024:	0a642520 	.word	0x0a642520
3000d028:	00000000 	.word	0x00000000
3000d02c:	6e206164 	.word	0x6e206164
3000d030:	7220746f 	.word	0x7220746f
3000d034:	69656365 	.word	0x69656365
3000d038:	0a646576 	.word	0x0a646576
3000d03c:	00000000 	.word	0x00000000
3000d040:	746f680a 	.word	0x746f680a
3000d044:	20786966 	.word	0x20786966
3000d048:	656e6f64 	.word	0x656e6f64
3000d04c:	202e2e2e 	.word	0x202e2e2e
3000d050:	6f626552 	.word	0x6f626552
3000d054:	6e69746f 	.word	0x6e69746f
3000d058:	0a2e2e67 	.word	0x0a2e2e67
3000d05c:	00000000 	.word	0x00000000
3000d060:	20304d4b 	.word	0x20304d4b
3000d064:	20504958 	.word	0x20504958
3000d068:	00474d49 	.word	0x00474d49
3000d06c:	20304d4b 	.word	0x20304d4b
3000d070:	4d415253 	.word	0x4d415253
3000d074:	00000000 	.word	0x00000000
3000d078:	20304d4b 	.word	0x20304d4b
3000d07c:	4d415244 	.word	0x4d415244
3000d080:	00000000 	.word	0x00000000
3000d084:	20344d4b 	.word	0x20344d4b
3000d088:	20504958 	.word	0x20504958
3000d08c:	00474d49 	.word	0x00474d49
3000d090:	20344d4b 	.word	0x20344d4b
3000d094:	4d415253 	.word	0x4d415253
3000d098:	00000000 	.word	0x00000000
3000d09c:	20344d4b 	.word	0x20344d4b
3000d0a0:	4d415244 	.word	0x4d415244
3000d0a4:	00000000 	.word	0x00000000
3000d0a8:	58205041 	.word	0x58205041
3000d0ac:	49205049 	.word	0x49205049
3000d0b0:	0000474d 	.word	0x0000474d
3000d0b4:	42205041 	.word	0x42205041
3000d0b8:	5320314c 	.word	0x5320314c
3000d0bc:	004d4152 	.word	0x004d4152
3000d0c0:	42205041 	.word	0x42205041
3000d0c4:	4420314c 	.word	0x4420314c
3000d0c8:	004d4152 	.word	0x004d4152
3000d0cc:	46205041 	.word	0x46205041
3000d0d0:	00005049 	.word	0x00005049
3000d0d4:	2d73255b 	.word	0x2d73255b
3000d0d8:	25205d49 	.word	0x25205d49
3000d0dc:	6e492073 	.word	0x6e492073
3000d0e0:	696c6176 	.word	0x696c6176
3000d0e4:	00000a64 	.word	0x00000a64
3000d0e8:	2d73255b 	.word	0x2d73255b
3000d0ec:	25205d49 	.word	0x25205d49
3000d0f0:	30255b73 	.word	0x30255b73
3000d0f4:	253a7838 	.word	0x253a7838
3000d0f8:	000a5d78 	.word	0x000a5d78
3000d0fc:	2d73255b 	.word	0x2d73255b
3000d100:	52205d49 	.word	0x52205d49
3000d104:	45205044 	.word	0x45205044
3000d108:	00000a4e 	.word	0x00000a4e
3000d10c:	2d73255b 	.word	0x2d73255b
3000d110:	46205d49 	.word	0x46205d49
3000d114:	206c6961 	.word	0x206c6961
3000d118:	6c206f74 	.word	0x6c206f74
3000d11c:	2064616f 	.word	0x2064616f
3000d120:	20504452 	.word	0x20504452
3000d124:	67616d69 	.word	0x67616d69
3000d128:	000a2165 	.word	0x000a2165
3000d12c:	2d73255b 	.word	0x2d73255b
3000d130:	52205d49 	.word	0x52205d49
3000d134:	4f205044 	.word	0x4f205044
3000d138:	000a4646 	.word	0x000a4646
3000d13c:	2d73255b 	.word	0x2d73255b
3000d140:	49205d49 	.word	0x49205d49
3000d144:	2032474d 	.word	0x2032474d
3000d148:	2046544f 	.word	0x2046544f
3000d14c:	000a4e45 	.word	0x000a4e45
3000d150:	2d73255b 	.word	0x2d73255b
3000d154:	4f205d45 	.word	0x4f205d45
3000d158:	4d204654 	.word	0x4d204654
3000d15c:	2065646f 	.word	0x2065646f
3000d160:	6f727265 	.word	0x6f727265
3000d164:	000a0d72 	.word	0x000a0d72
3000d168:	494c4156 	.word	0x494c4156
3000d16c:	00000044 	.word	0x00000044
3000d170:	41564e49 	.word	0x41564e49
3000d174:	0044494c 	.word	0x0044494c
3000d178:	74726563 	.word	0x74726563
3000d17c:	63696669 	.word	0x63696669
3000d180:	20657461 	.word	0x20657461
3000d184:	20434345 	.word	0x20434345
3000d188:	69726576 	.word	0x69726576
3000d18c:	66207966 	.word	0x66207966
3000d190:	2c6c6961 	.word	0x2c6c6961
3000d194:	6c6f7220 	.word	0x6c6f7220
3000d198:	6361626c 	.word	0x6361626c
3000d19c:	000a0d6b 	.word	0x000a0d6b
3000d1a0:	444f4d5b 	.word	0x444f4d5b
3000d1a4:	5f454c55 	.word	0x5f454c55
3000d1a8:	544f4f42 	.word	0x544f4f42
3000d1ac:	56454c2d 	.word	0x56454c2d
3000d1b0:	495f4c45 	.word	0x495f4c45
3000d1b4:	5d4f464e 	.word	0x5d4f464e
3000d1b8:	474d493a 	.word	0x474d493a
3000d1bc:	544f2832 	.word	0x544f2832
3000d1c0:	29642541 	.word	0x29642541
3000d1c4:	2c732520 	.word	0x2c732520
3000d1c8:	74657220 	.word	0x74657220
3000d1cc:	6425203a 	.word	0x6425203a
3000d1d0:	0000000a 	.word	0x0000000a
3000d1d4:	32474d49 	.word	0x32474d49
3000d1d8:	43434520 	.word	0x43434520
3000d1dc:	72657620 	.word	0x72657620
3000d1e0:	20796669 	.word	0x20796669
3000d1e4:	6c696166 	.word	0x6c696166
3000d1e8:	6f72202c 	.word	0x6f72202c
3000d1ec:	61626c6c 	.word	0x61626c6c
3000d1f0:	0a0d6b63 	.word	0x0a0d6b63
3000d1f4:	00000000 	.word	0x00000000
3000d1f8:	5f41544f 	.word	0x5f41544f
3000d1fc:	45205041 	.word	0x45205041
3000d200:	76204343 	.word	0x76204343
3000d204:	66697265 	.word	0x66697265
3000d208:	61662079 	.word	0x61662079
3000d20c:	202c6c69 	.word	0x202c6c69
3000d210:	6c6c6f72 	.word	0x6c6c6f72
3000d214:	6b636162 	.word	0x6b636162
3000d218:	00000a0d 	.word	0x00000a0d
3000d21c:	64616f4c 	.word	0x64616f4c
3000d220:	41544f20 	.word	0x41544f20
3000d224:	2050415f 	.word	0x2050415f
3000d228:	67616d69 	.word	0x67616d69
3000d22c:	61662065 	.word	0x61662065
3000d230:	202c6c69 	.word	0x202c6c69
3000d234:	6c6c6f72 	.word	0x6c6c6f72
3000d238:	6b636162 	.word	0x6b636162
3000d23c:	00000a0d 	.word	0x00000a0d
3000d240:	444f4d5b 	.word	0x444f4d5b
3000d244:	5f454c55 	.word	0x5f454c55
3000d248:	544f4f42 	.word	0x544f4f42
3000d24c:	56454c2d 	.word	0x56454c2d
3000d250:	455f4c45 	.word	0x455f4c45
3000d254:	524f5252 	.word	0x524f5252
3000d258:	61463a5d 	.word	0x61463a5d
3000d25c:	74206c69 	.word	0x74206c69
3000d260:	6f6c206f 	.word	0x6f6c206f
3000d264:	41206461 	.word	0x41206461
3000d268:	6d692050 	.word	0x6d692050
3000d26c:	21656761 	.word	0x21656761
3000d270:	0000000a 	.word	0x0000000a
3000d274:	20344d4b 	.word	0x20344d4b
3000d278:	33474d49 	.word	0x33474d49
3000d27c:	00000000 	.word	0x00000000
3000d280:	20344d4b 	.word	0x20344d4b
3000d284:	0043534e 	.word	0x0043534e
3000d288:	31474d49 	.word	0x31474d49
3000d28c:	00000000 	.word	0x00000000
3000d290:	32474d49 	.word	0x32474d49
3000d294:	00000000 	.word	0x00000000
3000d298:	42205041 	.word	0x42205041
3000d29c:	4626314c 	.word	0x4626314c
3000d2a0:	00005049 	.word	0x00005049
3000d2a4:	444f4d5b 	.word	0x444f4d5b
3000d2a8:	5f454c55 	.word	0x5f454c55
3000d2ac:	544f4f42 	.word	0x544f4f42
3000d2b0:	56454c2d 	.word	0x56454c2d
3000d2b4:	495f4c45 	.word	0x495f4c45
3000d2b8:	5d4f464e 	.word	0x5d4f464e
3000d2bc:	5b73253a 	.word	0x5b73253a
3000d2c0:	78383025 	.word	0x78383025
3000d2c4:	5d78253a 	.word	0x5d78253a
3000d2c8:	0000000a 	.word	0x0000000a
3000d2cc:	20504452 	.word	0x20504452
3000d2d0:	4c494146 	.word	0x4c494146
3000d2d4:	0000000a 	.word	0x0000000a
3000d2d8:	20504452 	.word	0x20504452
3000d2dc:	43435553 	.word	0x43435553
3000d2e0:	0a535345 	.word	0x0a535345
3000d2e4:	00000000 	.word	0x00000000
3000d2e8:	059e4eb3 	.word	0x059e4eb3
3000d2ec:	4b11ea39 	.word	0x4b11ea39
3000d2f0:	2a1b920a 	.word	0x2a1b920a
3000d2f4:	29939cee 	.word	0x29939cee
3000d2f8:	00000000 	.word	0x00000000
3000d2fc:	42205041 	.word	0x42205041
3000d300:	0032334c 	.word	0x0032334c
3000d304:	2d73255b 	.word	0x2d73255b
3000d308:	25205d49 	.word	0x25205d49
3000d30c:	45562073 	.word	0x45562073
3000d310:	59464952 	.word	0x59464952
3000d314:	53415020 	.word	0x53415020
3000d318:	00000a53 	.word	0x00000a53
3000d31c:	2d73255b 	.word	0x2d73255b
3000d320:	25205d45 	.word	0x25205d45
3000d324:	45562073 	.word	0x45562073
3000d328:	59464952 	.word	0x59464952
3000d32c:	49414620 	.word	0x49414620
3000d330:	72202c4c 	.word	0x72202c4c
3000d334:	3d207465 	.word	0x3d207465
3000d338:	0a642520 	.word	0x0a642520
3000d33c:	00000000 	.word	0x00000000
3000d340:	2d73255b 	.word	0x2d73255b
3000d344:	43205d45 	.word	0x43205d45
3000d348:	69747265 	.word	0x69747265
3000d34c:	61636966 	.word	0x61636966
3000d350:	56206574 	.word	0x56206574
3000d354:	46495245 	.word	0x46495245
3000d358:	41462059 	.word	0x41462059
3000d35c:	202c4c49 	.word	0x202c4c49
3000d360:	20746572 	.word	0x20746572
3000d364:	6425203d 	.word	0x6425203d
3000d368:	0000000a 	.word	0x0000000a
3000d36c:	6c630a0d 	.word	0x6c630a0d
3000d370:	2065736f 	.word	0x2065736f
3000d374:	646f4d79 	.word	0x646f4d79
3000d378:	54206d65 	.word	0x54206d65
3000d37c:	736e6172 	.word	0x736e6172
3000d380:	20726566 	.word	0x20726566
3000d384:	0d2e2e2e 	.word	0x0d2e2e2e
3000d388:	0000000a 	.word	0x0000000a
3000d38c:	50494843 	.word	0x50494843
3000d390:	4f464e49 	.word	0x4f464e49
3000d394:	00000000 	.word	0x00000000
3000d398:	2d73255b 	.word	0x2d73255b
3000d39c:	49205d45 	.word	0x49205d45
3000d3a0:	6c61766e 	.word	0x6c61766e
3000d3a4:	43206469 	.word	0x43206469
3000d3a8:	696e6968 	.word	0x696e6968
3000d3ac:	216f666e 	.word	0x216f666e
3000d3b0:	65684320 	.word	0x65684320
3000d3b4:	4f206b63 	.word	0x4f206b63
3000d3b8:	000a5054 	.word	0x000a5054
3000d3bc:	2d73255b 	.word	0x2d73255b
3000d3c0:	50205d57 	.word	0x50205d57
3000d3c4:	7361656c 	.word	0x7361656c
3000d3c8:	72702065 	.word	0x72702065
3000d3cc:	6172676f 	.word	0x6172676f
3000d3d0:	6863206d 	.word	0x6863206d
3000d3d4:	6e697069 	.word	0x6e697069
3000d3d8:	69206f66 	.word	0x69206f66
3000d3dc:	544f206e 	.word	0x544f206e
3000d3e0:	0a212050 	.word	0x0a212050
3000d3e4:	00000000 	.word	0x00000000

3000d3e8 <Memory_Info>:
3000d3e8:	03e00007 00060004 010202c0 02010100     ................
3000d3f8:	02000102 00040302 02040400 01010102     ................
3000d408:	01020221 02240401 03000102 00030103     !.....$.........
3000d418:	01050500 06000002 01020206 01070700     ................
3000d428:	08000002 01020208 02280801 09000102     ..........(.....
3000d438:	00040309 03290901 0a000004 0003010a     ......).........
3000d448:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000d458:	0d000102 0004030d 010e0e00 ffff0003     ................
3000d468:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000d478:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000d488:	62696c61 6f696172 000a0d6e              alibraion...

3000d494 <ddrphy_tx_scan>:
3000d494:	00000000 00080000 00080808 00000000     ................
3000d4a4:	20000000 33333333 000c0022 00000000     ... 3333".......
3000d4b4:	07060012 000f0707 080f0f0f 00000008     ................
3000d4c4:	20000001 22222222 000c0012 34000000     ... """".......4
3000d4d4:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000d4e4:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d4f4:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000d504:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d514:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000d524:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d534:	0e0e001a 00170e0e 12171717 00000012     ................
3000d544:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d554:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000d564:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d574:	0f0f001b 00180f0f 13181818 00000013     ................
3000d584:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d594:	1110001b 00191111 13191919 00000013     ................
3000d5a4:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000d5b4:	1313001c 001c1313 121c1c1c 00000012     ................
3000d5c4:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000d5d4 <ddrphy_zq_rx_scan>:
	...
3000d5dc:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000d5ec:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000d5fc:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000d60c:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000d61c:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000d62c:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000d63c:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000d64c:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000d65c:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000d66c:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000d67c:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000d68c:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000d69c <Flash_AVL>:
3000d69c:	000000ef 000000ff 00000000 000043fc     .............C..
3000d6ac:	00000000 000000a1 000000ff 00000000     ................
3000d6bc:	0000fffc 00000000 0000000b 000000ff     ................
3000d6cc:	00000000 000043fc 00000000 0000000e     .....C..........
3000d6dc:	000000ff 00000000 000043fc 00000000     .........C......
3000d6ec:	000000c8 000000ff 00000001 000043fc     .............C..
3000d6fc:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d70c:	000200fc 00000000 000000c2 000000ff     ................
3000d71c:	00000002 000000fc 00000000 00000068     ............h...
3000d72c:	000000ff 00000002 000000fc 00000000     ................
3000d73c:	00000051 000000ff 00000002 000000fc     Q...............
3000d74c:	00000000 0000001c 000000ff 00000003     ................
3000d75c:	000000fc 00000000 00000020 000000ff     ........ .......
3000d76c:	00000004 000000fc 00000000 00000000     ................
3000d77c:	000000ff 000000fe ffffffff 30008641     ............A..0
3000d78c:	000000ff ffffffff 000000ff ffffffff     ................
3000d79c:	00000000                                ....

3000d7a0 <Flash_ReadMode>:
3000d7a0:	                                         ..

3000d7a2 <Flash_Speed>:
3000d7a2:	                                         ..

3000d7a4 <NAND_AVL>:
3000d7a4:	000000ef 000000ff 00000000 ffffffff     ................
3000d7b4:	00000000 000000c8 000000ff 00000001     ................
3000d7c4:	ffffffff 00000000 000000e5 000000ff     ................
3000d7d4:	00000001 ffffffff 00000000 000000c2     ................
3000d7e4:	000000ff 00000002 ffffffff 00000000     ................
3000d7f4:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d80c:	000000ff 000000fe ffffffff 300086a1     ...............0
3000d81c:	000000ff ffffffff 000000ff ffffffff     ................
3000d82c:	00000000 3000c6c8 3000c6cc 3000c6d0     .......0...0...0
3000d83c:	3000c6d4 3000c6d8                       ...0...0

3000d844 <__FUNCTION__.0>:
3000d844:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d854 <__FUNCTION__.1>:
3000d854:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d864:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000d874:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000d884:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000d894:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000d8a4:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000d8b4:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000d8c4:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000d8d4:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000d8e4:	00000a20 2d73255b 50205d45 5220524d      ...[%s-E] PMR R
3000d8f4:	20646165 6f727265 000a2172 2d73255b     ead error!..[%s-
3000d904:	4f205d57 575f5054 65746972 61462038     W] OTP_Write8 Fa
3000d914:	25206c69 000a2078 2d73255b 4f205d45     il %x ..[%s-E] O
3000d924:	70205054 72676f72 65206d61 726f7272     TP program error
3000d934:	00000a21 2d73255b 50205d45 2032524d     !...[%s-E] PMR2 
3000d944:	64616552 72726520 0a21726f 00000000     Read error!.....

3000d954 <__FUNCTION__.1>:
3000d954:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d964:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000d974:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000d984:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000d994:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000d9a4:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000d9b4:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000d9c4:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000d9d4:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000d9e4:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000d9f4:	726f6620 41485320 00000a32 2d73255b      for SHA2...[%s-
3000da04:	69205d57 255b7172 70205d64 726f6972     W] irq[%d] prior
3000da14:	20797469 73206425 6c6c6168 203d3c20     ity %d shall <= 
3000da24:	000a6425                                %d..

3000da28 <__FUNCTION__.0>:
3000da28:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000da38:	00000000                                ....

3000da3c <__FUNCTION__.1>:
3000da3c:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000da4c <__FUNCTION__.3>:
3000da4c:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000da5c:	32616873 696e695f 00000074              sha2_init...

3000da68 <__FUNCTION__.4>:
3000da68:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000da78:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000da88:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000da98:	6e207369 6320746f 69666e6f 65727567     is not configure
3000daa8:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000dab8:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000dac8:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000dad8:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000dae8:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000daf8:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000db08:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000db18:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000db28:	69625f6b 21217374 00000a21              k_bits!!!...

3000db34 <__FUNCTION__.1>:
3000db34:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000db44:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000db54:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000db64:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000db74:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000db84:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000db94:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000dba4:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000dbb4:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000dbc4:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000dbd4:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000dbe4:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000dbf4:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000dc04:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000dc14:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000dc24:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000dc34:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000dc44:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000dc54:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000dc64:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000dc74:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000dc84:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000dc94:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000dca4:	00000000                                ....

3000dca8 <register_string>:
3000dca8:	00000000 3000dbd0 3000c7e8 3000c7ec     .......0...0...0
3000dcb8:	3000c7f0 3000c7f4 3000c7f8 3000c7fc     ...0...0...0...0
3000dcc8:	3000c800 3000dbcc 3000c804 3000c808     ...0...0...0...0
3000dcd8:	3000c80c 3000dc70 65637845 6f697470     ...0p..0Exceptio
3000dce8:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000dcf8:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000dd08:	64657375 20796220 6c696166 76206465     used by failed v
3000dd18:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000dd28:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000dd38:	00000000 75636553 79746972 75616620     ....Security fau
3000dd48:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dd58:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000dd68:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000dd78:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000dd88:	79746972 75616620 6920746c 61632073     rity fault is ca
3000dd98:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000dda8:	65746e69 74697267 69732079 74616e67     integrity signat
3000ddb8:	00657275 75636553 79746972 75616620     ure.Security fau
3000ddc8:	6920746c 61632073 64657375 20796220     lt is caused by 
3000ddd8:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000dde8:	6572206e 6e727574 00000000 75636553     n return....Secu
3000ddf8:	79746972 75616620 6920746c 61632073     rity fault is ca
3000de08:	64657375 20796220 72747441 74756269     used by Attribut
3000de18:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000de28:	00006e6f 75636553 79746972 75616620     on..Security fau
3000de38:	6920746c 61632073 64657375 20796220     lt is caused by 
3000de48:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000de58:	00006e6f 75636553 79746972 75616620     on..Security fau
3000de68:	6920746c 61632073 64657375 20796220     lt is caused by 
3000de78:	797a614c 61747320 70206574 65736572     Lazy state prese
3000de88:	74617672 206e6f69 6f727265 00000072     rvation error...
3000de98:	75636553 79746972 75616620 6920746c     Security fault i
3000dea8:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000deb8:	61747320 65206574 726f7272 00000000      state error....
3000dec8:	20656854 75636573 79746972 75616620     The security fau
3000ded8:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000dee8:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000def8:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000df08:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000df18:	61662074 20746c75 63207369 65737561     t fault is cause
3000df28:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000df38:	63636120 20737365 6c6f6976 6f697461      access violatio
3000df48:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000df58:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000df68:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000df78:	20746c75 63207369 65737561 79622064     ult is caused by
3000df88:	74616420 63612061 73736563 6f697620      data access vio
3000df98:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000dfa8:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000dfb8:	63207369 65737561 79622064 736e7520     is caused by uns
3000dfc8:	6b636174 20676e69 6f727265 00000072     tacking error...
3000dfd8:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000dfe8:	61662074 20746c75 63207369 65737561     t fault is cause
3000dff8:	79622064 61747320 6e696b63 72652067     d by stacking er
3000e008:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000e018:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000e028:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000e038:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000e048:	70206574 65736572 74617672 006e6f69     te preservation.
3000e058:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000e068:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000e078:	64657272 64646120 73736572 00736920     rred address is.
3000e088:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000e098:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000e0a8:	73692074 75616320 20646573 69207962     t is caused by i
3000e0b8:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000e0c8:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000e0d8:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000e0e8:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000e0f8:	75616320 20646573 70207962 69636572      caused by preci
3000e108:	64206573 20617461 65636361 76207373     se data access v
3000e118:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000e128:	6c756166 73692074 75616320 20646573     fault is caused 
3000e138:	69207962 6572706d 65736963 74616420     by imprecise dat
3000e148:	63612061 73736563 6f697620 6974616c     a access violati
3000e158:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000e168:	75616320 20646573 75207962 6174736e      caused by unsta
3000e178:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000e188:	6c756166 73692074 75616320 20646573     fault is caused 
3000e198:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000e1a8:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000e1b8:	75616320 20646573 66207962 74616f6c      caused by float
3000e1c8:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000e1d8:	65746174 65727020 76726573 6f697461     tate preservatio
3000e1e8:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000e1f8:	636f2074 72727563 61206465 65726464     t occurred addre
3000e208:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000e218:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e228:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e238:	65737561 79622064 74746120 74706d65     aused by attempt
3000e248:	6f742073 65786520 65747563 206e6120     s to execute an 
3000e258:	65646e75 656e6966 6e692064 75727473     undefined instru
3000e268:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000e278:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e288:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e298:	65737561 79622064 74746120 74706d65     aused by attempt
3000e2a8:	6f742073 69777320 20686374 61206f74     s to switch to a
3000e2b8:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000e2c8:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000e2d8:	61662065 20746c75 63207369 65737561     e fault is cause
3000e2e8:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e2f8:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000e308:	74697720 20612068 20646162 756c6176      with a bad valu
3000e318:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000e328:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000e338:	61662065 20746c75 63207369 65737561     e fault is cause
3000e348:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e358:	65786520 65747563 63206120 6f72706f      execute a copro
3000e368:	73736563 6920726f 7274736e 69746375     cessor instructi
3000e378:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000e388:	63207369 65737561 79622064 646e6920     is caused by ind
3000e398:	74616369 74207365 20746168 74732061     icates that a st
3000e3a8:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000e3b8:	61776472 63206572 6b636568 61682029     rdware check) ha
3000e3c8:	61742073 206e656b 63616c70 00000065     s taken place...
3000e3d8:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e3e8:	65737561 79622064 646e6920 74616369     aused by indicat
3000e3f8:	74207365 20746168 75206e61 696c616e     es that an unali
3000e408:	64656e67 63636120 20737365 6c756166     gned access faul
3000e418:	61682074 61742073 206e656b 63616c70     t has taken plac
3000e428:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000e438:	63207369 65737561 79622064 646e4920     is caused by Ind
3000e448:	74616369 61207365 76696420 20656469     icates a divide 
3000e458:	7a207962 206f7265 20736168 656b6174     by zero has take
3000e468:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000e478:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000e488:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000e498:	75626544 61662067 20746c75 63207369     Debug fault is c
3000e4a8:	65737561 79622064 6c616820 65722074     aused by halt re
3000e4b8:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000e4c8:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000e4d8:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000e4e8:	20746c75 63207369 65737561 79622064     ult is caused by
3000e4f8:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000e508:	7865206e 74756365 00006465 75626544     n executed..Debu
3000e518:	61662067 20746c75 63207369 65737561     g fault is cause
3000e528:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000e538:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000e548:	20746c75 63207369 65737561 79622064     ult is caused by
3000e558:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000e568:	72727563 00006465 75626544 61662067     curred..Debug fa
3000e578:	20746c75 63207369 65737561 79622064     ult is caused by
3000e588:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000e598:	72657373 00646574 00000208 0000020c     sserted.........
3000e5a8:	00000210 00000008 00000214 00000218     ................
3000e5b8:	0000021c 0000000c                       ........

3000e5c0 <__FUNCTION__.0>:
3000e5c0:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.
3000e5d0:	3000d060 3000d06c 3000d078 3000d084     `..0l..0x..0...0
3000e5e0:	3000d090 3000d09c 3000d0a8 3000d0b4     ...0...0...0...0
3000e5f0:	3000d0c0 3000d0cc 3000d274 3000d280     ...0...0t..0...0
3000e600:	3000d288 3000d290 3000d278 3000d298     ...0...0x..0...0

3000e610 <km4_bootloader_rev>:
3000e610:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000e620:	645f7265 64653830 65343335 30325f37     er_d08ed534e7_20
3000e630:	302f3432 31332f35 3a37312d 303a3830     24/05/31-17:08:0
3000e640:	4f420037 5b00544f 572d7325 2a2a205d     7.BOOT.[%s-W] **
3000e650:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e660:	2a2a2a2a 2a2a2a2a 52415720 474e494e     ******** WARNING
3000e670:	2a2a2a20 2a2a2a2a 2a2a2a2a 2a2a2a2a      ***************
3000e680:	2a2a2a2a 2a2a2a2a 255b000a 5d572d73     ********..[%s-W]
3000e690:	20202a20 20202020 20202020 20202020      *              
3000e6a0:	20202020 20202020 20202020 20202020                     
3000e6b0:	20202020 20202020 20202020 20202020                     
3000e6c0:	20202020 20202020 0a2a2020 73255b00               *..[%s
3000e6d0:	205d572d 2020202a 20202020 20202020     -W] *           
3000e6e0:	75432020 6e657272 68632074 76207069       Current chip v
3000e6f0:	69737265 69206e6f 43412073 21217475     ersion is ACut!!
3000e700:	20202021 20202020 20202020 000a2a20     !            *..
3000e710:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000e720:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000e730:	2074726f 72727563 20746e65 73726576     ort current vers
3000e740:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000e750:	5b000a2a 572d7325 202a205d 20202020     *..[%s-W] *     
3000e760:	656c5020 20657361 746e6f63 20746361      Please contact 
3000e770:	204b5452 66204d50 6d20726f 2065726f     RTK PM for more 
3000e780:	646f7270 20746375 6f666e69 20202021     product info!   
3000e790:	2a202020 255b000a 5d572d73 2a2a2a20        *..[%s-W] ***
3000e7a0:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e7b0:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e7c0:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e7d0:	2a2a2a2a 0a2a2a2a 73255b00 205d572d     *******..[%s-W] 
3000e7e0:	0043000a 49534f50 002e0058              ..C.POSIX...

3000e7ec <_ctype_>:
3000e7ec:	20202000 20202020 28282020 20282828     .         ((((( 
3000e7fc:	20202020 20202020 20202020 20202020                     
3000e80c:	10108820 10101010 10101010 10101010      ...............
3000e81c:	04040410 04040404 10040404 10101010     ................
3000e82c:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000e83c:	01010101 01010101 01010101 10101010     ................
3000e84c:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000e85c:	02020202 02020202 02020202 10101010     ................
3000e86c:	00000020 00000000 00000000 00000000      ...............
	...
