
/home/zhongnuo/sambashare/7.1/sdk/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


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
300032c4:	3000927d 	.word	0x3000927d
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	30009105 	.word	0x30009105
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
300033fc:	3000927d 	.word	0x3000927d
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	30009105 	.word	0x30009105
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
3000349c:	f008 f88a 	bl	3000b5b4 <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000bc24 	.word	0x3000bc24
300034bc:	3000bc30 	.word	0x3000bc30

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
300034f0:	f008 f860 	bl	3000b5b4 <rtk_log_write>
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
30003524:	f008 f846 	bl	3000b5b4 <rtk_log_write>
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
30003540:	3000bc24 	.word	0x3000bc24
30003544:	3000bc74 	.word	0x3000bc74
30003548:	3000927d 	.word	0x3000927d
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000bc54 	.word	0x3000bc54
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
3000365a:	f007 ffab 	bl	3000b5b4 <rtk_log_write>
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
3000367a:	f007 ff9b 	bl	3000b5b4 <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000cf58 	.word	0x3000cf58
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000bc24 	.word	0x3000bc24
300036a8:	3000bcac 	.word	0x3000bcac
300036ac:	3000bc94 	.word	0x3000bc94
300036b0:	3000927d 	.word	0x3000927d
300036b4:	30009105 	.word	0x30009105
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
3000373e:	f007 ff39 	bl	3000b5b4 <rtk_log_write>
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
30003790:	f007 ff10 	bl	3000b5b4 <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 f9e9 	bl	3000bb70 <__io_assert_failed_veneer>
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
300037ca:	f007 fef3 	bl	3000b5b4 <rtk_log_write>
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
30003816:	f007 fecd 	bl	3000b5b4 <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f008 f9a6 	bl	3000bb70 <__io_assert_failed_veneer>
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
30003958:	3000cec8 	.word	0x3000cec8
3000395c:	3000ceca 	.word	0x3000ceca
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000bc24 	.word	0x3000bc24
3000396c:	3000bcd0 	.word	0x3000bcd0
30003970:	3000cdc4 	.word	0x3000cdc4
30003974:	3000cecc 	.word	0x3000cecc
30003978:	3000bcec 	.word	0x3000bcec
3000397c:	3000cf7c 	.word	0x3000cf7c
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000bd18 	.word	0x3000bd18
30003988:	3000cf6c 	.word	0x3000cf6c
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000f391 	.word	0x3000f391
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	3000927d 	.word	0x3000927d
300039b8:	3000f394 	.word	0x3000f394
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
30003a36:	f008 f89b 	bl	3000bb70 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f008 f892 	bl	3000bb70 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000f391 	.word	0x3000f391
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000cf6c 	.word	0x3000cf6c
30003a78:	3000cf7c 	.word	0x3000cf7c
30003a7c:	0000a4c1 	.word	0x0000a4c1

30003a80 <BOOT_INT_SecureFault>:
30003a80:	f3ef 8008 	mrs	r0, MSP
30003a84:	f3ef 8109 	mrs	r1, PSP
30003a88:	4672      	mov	r2, lr
30003a8a:	f04f 0304 	mov.w	r3, #4
30003a8e:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30003a92:	f000 b801 	b.w	30003a98 <BOOT_FaultHandler>
30003a96:	4770      	bx	lr

30003a98 <BOOT_FaultHandler>:
30003a98:	b5f0      	push	{r4, r5, r6, r7, lr}
30003a9a:	461f      	mov	r7, r3
30003a9c:	4b41      	ldr	r3, [pc, #260]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003a9e:	b095      	sub	sp, #84	; 0x54
30003aa0:	4616      	mov	r6, r2
30003aa2:	4604      	mov	r4, r0
30003aa4:	4a40      	ldr	r2, [pc, #256]	; (30003ba8 <BOOT_FaultHandler+0x110>)
30003aa6:	2001      	movs	r0, #1
30003aa8:	460d      	mov	r5, r1
30003aaa:	4619      	mov	r1, r3
30003aac:	f007 fd82 	bl	3000b5b4 <rtk_log_write>
30003ab0:	2244      	movs	r2, #68	; 0x44
30003ab2:	2100      	movs	r1, #0
30003ab4:	a803      	add	r0, sp, #12
30003ab6:	f008 f883 	bl	3000bbc0 <____wrap_memset_veneer>
30003aba:	0672      	lsls	r2, r6, #25
30003abc:	4b39      	ldr	r3, [pc, #228]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003abe:	d55f      	bpl.n	30003b80 <BOOT_FaultHandler+0xe8>
30003ac0:	4619      	mov	r1, r3
30003ac2:	4a3a      	ldr	r2, [pc, #232]	; (30003bac <BOOT_FaultHandler+0x114>)
30003ac4:	2001      	movs	r0, #1
30003ac6:	f007 fd75 	bl	3000b5b4 <rtk_log_write>
30003aca:	f006 010c 	and.w	r1, r6, #12
30003ace:	46a6      	mov	lr, r4
30003ad0:	f1a1 010c 	sub.w	r1, r1, #12
30003ad4:	fab1 f181 	clz	r1, r1
30003ad8:	0949      	lsrs	r1, r1, #5
30003ada:	f1a4 0c24 	sub.w	ip, r4, #36	; 0x24
30003ade:	1f22      	subs	r2, r4, #4
30003ae0:	a804      	add	r0, sp, #16
30003ae2:	9603      	str	r6, [sp, #12]
30003ae4:	f85c 3f04 	ldr.w	r3, [ip, #4]!
30003ae8:	4594      	cmp	ip, r2
30003aea:	f840 3b04 	str.w	r3, [r0], #4
30003aee:	d1f9      	bne.n	30003ae4 <BOOT_FaultHandler+0x4c>
30003af0:	2900      	cmp	r1, #0
30003af2:	bf08      	it	eq
30003af4:	4675      	moveq	r5, lr
30003af6:	f10d 0c30 	add.w	ip, sp, #48	; 0x30
30003afa:	1f28      	subs	r0, r5, #4
30003afc:	f105 031c 	add.w	r3, r5, #28
30003b00:	f850 4f04 	ldr.w	r4, [r0, #4]!
30003b04:	4283      	cmp	r3, r0
30003b06:	f84c 4b04 	str.w	r4, [ip], #4
30003b0a:	d1f9      	bne.n	30003b00 <BOOT_FaultHandler+0x68>
30003b0c:	4c28      	ldr	r4, [pc, #160]	; (30003bb0 <BOOT_FaultHandler+0x118>)
30003b0e:	aa03      	add	r2, sp, #12
30003b10:	4629      	mov	r1, r5
30003b12:	6be8      	ldr	r0, [r5, #60]	; 0x3c
30003b14:	f007 fa78 	bl	3000b008 <crash_dump>
30003b18:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30003b1c:	4b21      	ldr	r3, [pc, #132]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b1e:	2001      	movs	r0, #1
30003b20:	9100      	str	r1, [sp, #0]
30003b22:	4a24      	ldr	r2, [pc, #144]	; (30003bb4 <BOOT_FaultHandler+0x11c>)
30003b24:	4619      	mov	r1, r3
30003b26:	f007 fd45 	bl	3000b5b4 <rtk_log_write>
30003b2a:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30003b2e:	4b1d      	ldr	r3, [pc, #116]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b30:	2001      	movs	r0, #1
30003b32:	9200      	str	r2, [sp, #0]
30003b34:	4619      	mov	r1, r3
30003b36:	4a20      	ldr	r2, [pc, #128]	; (30003bb8 <BOOT_FaultHandler+0x120>)
30003b38:	f007 fd3c 	bl	3000b5b4 <rtk_log_write>
30003b3c:	6a61      	ldr	r1, [r4, #36]	; 0x24
30003b3e:	4b19      	ldr	r3, [pc, #100]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b40:	2001      	movs	r0, #1
30003b42:	9100      	str	r1, [sp, #0]
30003b44:	4a1d      	ldr	r2, [pc, #116]	; (30003bbc <BOOT_FaultHandler+0x124>)
30003b46:	4619      	mov	r1, r3
30003b48:	f007 fd34 	bl	3000b5b4 <rtk_log_write>
30003b4c:	f3ef 8488 	mrs	r4, MSP_NS
30003b50:	f3ef 8589 	mrs	r5, PSP_NS
30003b54:	2101      	movs	r1, #1
30003b56:	4b1a      	ldr	r3, [pc, #104]	; (30003bc0 <BOOT_FaultHandler+0x128>)
30003b58:	2002      	movs	r0, #2
30003b5a:	4798      	blx	r3
30003b5c:	4b19      	ldr	r3, [pc, #100]	; (30003bc4 <BOOT_FaultHandler+0x12c>)
30003b5e:	2002      	movs	r0, #2
30003b60:	4798      	blx	r3
30003b62:	4b10      	ldr	r3, [pc, #64]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b64:	9000      	str	r0, [sp, #0]
30003b66:	2001      	movs	r0, #1
30003b68:	4619      	mov	r1, r3
30003b6a:	4a17      	ldr	r2, [pc, #92]	; (30003bc8 <BOOT_FaultHandler+0x130>)
30003b6c:	f007 fd22 	bl	3000b5b4 <rtk_log_write>
30003b70:	463b      	mov	r3, r7
30003b72:	4632      	mov	r2, r6
30003b74:	4629      	mov	r1, r5
30003b76:	4620      	mov	r0, r4
30003b78:	f008 f802 	bl	3000bb80 <__INT_HardFault_C_veneer>
30003b7c:	b015      	add	sp, #84	; 0x54
30003b7e:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003b80:	4a12      	ldr	r2, [pc, #72]	; (30003bcc <BOOT_FaultHandler+0x134>)
30003b82:	2001      	movs	r0, #1
30003b84:	4619      	mov	r1, r3
30003b86:	f007 fd15 	bl	3000b5b4 <rtk_log_write>
30003b8a:	f3ef 8394 	mrs	r3, CONTROL_NS
30003b8e:	f3ef 8e88 	mrs	lr, MSP_NS
30003b92:	f3ef 8589 	mrs	r5, PSP_NS
30003b96:	f016 0108 	ands.w	r1, r6, #8
30003b9a:	bf18      	it	ne
30003b9c:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30003ba0:	e79b      	b.n	30003ada <BOOT_FaultHandler+0x42>
30003ba2:	bf00      	nop
30003ba4:	3000bd58 	.word	0x3000bd58
30003ba8:	3000bd60 	.word	0x3000bd60
30003bac:	3000bd84 	.word	0x3000bd84
30003bb0:	e000ed00 	.word	0xe000ed00
30003bb4:	3000bdd8 	.word	0x3000bdd8
30003bb8:	3000bdf0 	.word	0x3000bdf0
30003bbc:	3000be08 	.word	0x3000be08
30003bc0:	0000994d 	.word	0x0000994d
30003bc4:	00009979 	.word	0x00009979
30003bc8:	3000be20 	.word	0x3000be20
30003bcc:	3000bdac 	.word	0x3000bdac

30003bd0 <FLASH_Erase_With_Lock>:
30003bd0:	4b11      	ldr	r3, [pc, #68]	; (30003c18 <FLASH_Erase_With_Lock+0x48>)
30003bd2:	b510      	push	{r4, lr}
30003bd4:	460c      	mov	r4, r1
30003bd6:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30003bda:	4798      	blx	r3
30003bdc:	4b0f      	ldr	r3, [pc, #60]	; (30003c1c <FLASH_Erase_With_Lock+0x4c>)
30003bde:	695b      	ldr	r3, [r3, #20]
30003be0:	03db      	lsls	r3, r3, #15
30003be2:	d512      	bpl.n	30003c0a <FLASH_Erase_With_Lock+0x3a>
30003be4:	f014 021f 	ands.w	r2, r4, #31
30003be8:	d110      	bne.n	30003c0c <FLASH_Erase_With_Lock+0x3c>
30003bea:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003bee:	f3bf 8f4f 	dsb	sy
30003bf2:	480a      	ldr	r0, [pc, #40]	; (30003c1c <FLASH_Erase_With_Lock+0x4c>)
30003bf4:	4422      	add	r2, r4
30003bf6:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
30003bfa:	3420      	adds	r4, #32
30003bfc:	1b13      	subs	r3, r2, r4
30003bfe:	2b00      	cmp	r3, #0
30003c00:	dcf9      	bgt.n	30003bf6 <FLASH_Erase_With_Lock+0x26>
30003c02:	f3bf 8f4f 	dsb	sy
30003c06:	f3bf 8f6f 	isb	sy
30003c0a:	bd10      	pop	{r4, pc}
30003c0c:	f024 041f 	bic.w	r4, r4, #31
30003c10:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30003c14:	e7eb      	b.n	30003bee <FLASH_Erase_With_Lock+0x1e>
30003c16:	bf00      	nop
30003c18:	0000a16d 	.word	0x0000a16d
30003c1c:	e000ed00 	.word	0xe000ed00

30003c20 <FLASH_TxData_With_Lock>:
30003c20:	b538      	push	{r3, r4, r5, lr}
30003c22:	4b13      	ldr	r3, [pc, #76]	; (30003c70 <FLASH_TxData_With_Lock+0x50>)
30003c24:	4604      	mov	r4, r0
30003c26:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30003c2a:	460d      	mov	r5, r1
30003c2c:	4798      	blx	r3
30003c2e:	4b11      	ldr	r3, [pc, #68]	; (30003c74 <FLASH_TxData_With_Lock+0x54>)
30003c30:	695b      	ldr	r3, [r3, #20]
30003c32:	03db      	lsls	r3, r3, #15
30003c34:	d517      	bpl.n	30003c66 <FLASH_TxData_With_Lock+0x46>
30003c36:	f014 031f 	ands.w	r3, r4, #31
30003c3a:	d115      	bne.n	30003c68 <FLASH_TxData_With_Lock+0x48>
30003c3c:	f3bf 8f4f 	dsb	sy
30003c40:	b16d      	cbz	r5, 30003c5e <FLASH_TxData_With_Lock+0x3e>
30003c42:	1e6a      	subs	r2, r5, #1
30003c44:	f104 0320 	add.w	r3, r4, #32
30003c48:	490a      	ldr	r1, [pc, #40]	; (30003c74 <FLASH_TxData_With_Lock+0x54>)
30003c4a:	f022 021f 	bic.w	r2, r2, #31
30003c4e:	441a      	add	r2, r3
30003c50:	4293      	cmp	r3, r2
30003c52:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30003c56:	461c      	mov	r4, r3
30003c58:	f103 0320 	add.w	r3, r3, #32
30003c5c:	d1f8      	bne.n	30003c50 <FLASH_TxData_With_Lock+0x30>
30003c5e:	f3bf 8f4f 	dsb	sy
30003c62:	f3bf 8f6f 	isb	sy
30003c66:	bd38      	pop	{r3, r4, r5, pc}
30003c68:	f024 041f 	bic.w	r4, r4, #31
30003c6c:	441d      	add	r5, r3
30003c6e:	e7e5      	b.n	30003c3c <FLASH_TxData_With_Lock+0x1c>
30003c70:	0000a04d 	.word	0x0000a04d
30003c74:	e000ed00 	.word	0xe000ed00

30003c78 <WDG_reset>:
30003c78:	b500      	push	{lr}
30003c7a:	b083      	sub	sp, #12
30003c7c:	4b08      	ldr	r3, [pc, #32]	; (30003ca0 <WDG_reset+0x28>)
30003c7e:	4668      	mov	r0, sp
30003c80:	4798      	blx	r3
30003c82:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30003c86:	4669      	mov	r1, sp
30003c88:	4806      	ldr	r0, [pc, #24]	; (30003ca4 <WDG_reset+0x2c>)
30003c8a:	4b07      	ldr	r3, [pc, #28]	; (30003ca8 <WDG_reset+0x30>)
30003c8c:	f8ad 2002 	strh.w	r2, [sp, #2]
30003c90:	4798      	blx	r3
30003c92:	4804      	ldr	r0, [pc, #16]	; (30003ca4 <WDG_reset+0x2c>)
30003c94:	4b05      	ldr	r3, [pc, #20]	; (30003cac <WDG_reset+0x34>)
30003c96:	4798      	blx	r3
30003c98:	b003      	add	sp, #12
30003c9a:	f85d fb04 	ldr.w	pc, [sp], #4
30003c9e:	bf00      	nop
30003ca0:	0000d3f9 	.word	0x0000d3f9
30003ca4:	41000440 	.word	0x41000440
30003ca8:	0000d409 	.word	0x0000d409
30003cac:	0000d489 	.word	0x0000d489

30003cb0 <flash_read_id>:
30003cb0:	b538      	push	{r3, r4, r5, lr}
30003cb2:	4d04      	ldr	r5, [pc, #16]	; (30003cc4 <flash_read_id+0x14>)
30003cb4:	460c      	mov	r4, r1
30003cb6:	4602      	mov	r2, r0
30003cb8:	4b03      	ldr	r3, [pc, #12]	; (30003cc8 <flash_read_id+0x18>)
30003cba:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30003cbe:	4798      	blx	r3
30003cc0:	4620      	mov	r0, r4
30003cc2:	bd38      	pop	{r3, r4, r5, pc}
30003cc4:	2001c01c 	.word	0x2001c01c
30003cc8:	00009db5 	.word	0x00009db5

30003ccc <mcc_ReverseBit>:
30003ccc:	0843      	lsrs	r3, r0, #1
30003cce:	08c1      	lsrs	r1, r0, #3
30003cd0:	f003 0308 	and.w	r3, r3, #8
30003cd4:	0942      	lsrs	r2, r0, #5
30003cd6:	f001 0104 	and.w	r1, r1, #4
30003cda:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30003cde:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30003ce2:	f002 0202 	and.w	r2, r2, #2
30003ce6:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
30003cea:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30003cee:	430b      	orrs	r3, r1
30003cf0:	00c1      	lsls	r1, r0, #3
30003cf2:	4313      	orrs	r3, r2
30003cf4:	f001 0120 	and.w	r1, r1, #32
30003cf8:	0042      	lsls	r2, r0, #1
30003cfa:	ea43 000c 	orr.w	r0, r3, ip
30003cfe:	f002 0310 	and.w	r3, r2, #16
30003d02:	4308      	orrs	r0, r1
30003d04:	4318      	orrs	r0, r3
30003d06:	b2c0      	uxtb	r0, r0
30003d08:	4770      	bx	lr
30003d0a:	bf00      	nop

30003d0c <mcc_init_crc32>:
30003d0c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30003d0e:	2600      	movs	r6, #0
30003d10:	4f16      	ldr	r7, [pc, #88]	; (30003d6c <mcc_init_crc32+0x60>)
30003d12:	4605      	mov	r5, r0
30003d14:	b2f0      	uxtb	r0, r6
30003d16:	f7ff ffd9 	bl	30003ccc <mcc_ReverseBit>
30003d1a:	2208      	movs	r2, #8
30003d1c:	0604      	lsls	r4, r0, #24
30003d1e:	2c00      	cmp	r4, #0
30003d20:	ea4f 0444 	mov.w	r4, r4, lsl #1
30003d24:	bfb8      	it	lt
30003d26:	407c      	eorlt	r4, r7
30003d28:	3a01      	subs	r2, #1
30003d2a:	d1f8      	bne.n	30003d1e <mcc_init_crc32+0x12>
30003d2c:	0e20      	lsrs	r0, r4, #24
30003d2e:	3504      	adds	r5, #4
30003d30:	f7ff ffcc 	bl	30003ccc <mcc_ReverseBit>
30003d34:	4603      	mov	r3, r0
30003d36:	f3c4 4007 	ubfx	r0, r4, #16, #8
30003d3a:	3601      	adds	r6, #1
30003d3c:	f805 3c04 	strb.w	r3, [r5, #-4]
30003d40:	f7ff ffc4 	bl	30003ccc <mcc_ReverseBit>
30003d44:	4603      	mov	r3, r0
30003d46:	f3c4 2007 	ubfx	r0, r4, #8, #8
30003d4a:	f805 3c03 	strb.w	r3, [r5, #-3]
30003d4e:	f7ff ffbd 	bl	30003ccc <mcc_ReverseBit>
30003d52:	4603      	mov	r3, r0
30003d54:	b2e0      	uxtb	r0, r4
30003d56:	f805 3c02 	strb.w	r3, [r5, #-2]
30003d5a:	f7ff ffb7 	bl	30003ccc <mcc_ReverseBit>
30003d5e:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30003d62:	f805 0c01 	strb.w	r0, [r5, #-1]
30003d66:	d1d5      	bne.n	30003d14 <mcc_init_crc32+0x8>
30003d68:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003d6a:	bf00      	nop
30003d6c:	04c11db7 	.word	0x04c11db7

30003d70 <mcc_crc32>:
30003d70:	b179      	cbz	r1, 30003d92 <mcc_crc32+0x22>
30003d72:	eb00 0c01 	add.w	ip, r0, r1
30003d76:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30003d7a:	f810 3b01 	ldrb.w	r3, [r0], #1
30003d7e:	404b      	eors	r3, r1
30003d80:	4560      	cmp	r0, ip
30003d82:	b2db      	uxtb	r3, r3
30003d84:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30003d88:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30003d8c:	d1f5      	bne.n	30003d7a <mcc_crc32+0xa>
30003d8e:	43c8      	mvns	r0, r1
30003d90:	4770      	bx	lr
30003d92:	4608      	mov	r0, r1
30003d94:	4770      	bx	lr
30003d96:	bf00      	nop

30003d98 <recovery_check>:
30003d98:	2803      	cmp	r0, #3
30003d9a:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003d9e:	460e      	mov	r6, r1
30003da0:	b083      	sub	sp, #12
30003da2:	4698      	mov	r8, r3
30003da4:	d83e      	bhi.n	30003e24 <recovery_check+0x8c>
30003da6:	4b24      	ldr	r3, [pc, #144]	; (30003e38 <recovery_check+0xa0>)
30003da8:	f853 1020 	ldr.w	r1, [r3, r0, lsl #2]
30003dac:	f102 6300 	add.w	r3, r2, #134217728	; 0x8000000
30003db0:	42b3      	cmp	r3, r6
30003db2:	d333      	bcc.n	30003e1c <recovery_check+0x84>
30003db4:	f1b6 6f00 	cmp.w	r6, #134217728	; 0x8000000
30003db8:	d930      	bls.n	30003e1c <recovery_check+0x84>
30003dba:	f8d6 300a 	ldr.w	r3, [r6, #10]
30003dbe:	f103 090c 	add.w	r9, r3, #12
30003dc2:	454a      	cmp	r2, r9
30003dc4:	d32a      	bcc.n	30003e1c <recovery_check+0x84>
30003dc6:	46b3      	mov	fp, r6
30003dc8:	f106 0410 	add.w	r4, r6, #16
30003dcc:	2700      	movs	r7, #0
30003dce:	4d1b      	ldr	r5, [pc, #108]	; (30003e3c <recovery_check+0xa4>)
30003dd0:	441c      	add	r4, r3
30003dd2:	f85b ab04 	ldr.w	sl, [fp], #4
30003dd6:	3701      	adds	r7, #1
30003dd8:	f1b9 0f00 	cmp.w	r9, #0
30003ddc:	d024      	beq.n	30003e28 <recovery_check+0x90>
30003dde:	465a      	mov	r2, fp
30003de0:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30003de4:	f812 0b01 	ldrb.w	r0, [r2], #1
30003de8:	4058      	eors	r0, r3
30003dea:	42a2      	cmp	r2, r4
30003dec:	b2c0      	uxtb	r0, r0
30003dee:	f855 0020 	ldr.w	r0, [r5, r0, lsl #2]
30003df2:	ea80 2313 	eor.w	r3, r0, r3, lsr #8
30003df6:	d1f5      	bne.n	30003de4 <recovery_check+0x4c>
30003df8:	43db      	mvns	r3, r3
30003dfa:	459a      	cmp	sl, r3
30003dfc:	d016      	beq.n	30003e2c <recovery_check+0x94>
30003dfe:	463a      	mov	r2, r7
30003e00:	480f      	ldr	r0, [pc, #60]	; (30003e40 <recovery_check+0xa8>)
30003e02:	9101      	str	r1, [sp, #4]
30003e04:	f007 fe74 	bl	3000baf0 <__DiagPrintf_veneer>
30003e08:	2f03      	cmp	r7, #3
30003e0a:	9901      	ldr	r1, [sp, #4]
30003e0c:	d1e3      	bne.n	30003dd6 <recovery_check+0x3e>
30003e0e:	480d      	ldr	r0, [pc, #52]	; (30003e44 <recovery_check+0xac>)
30003e10:	f007 fe6e 	bl	3000baf0 <__DiagPrintf_veneer>
30003e14:	2000      	movs	r0, #0
30003e16:	b003      	add	sp, #12
30003e18:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003e1c:	2000      	movs	r0, #0
30003e1e:	b003      	add	sp, #12
30003e20:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003e24:	4908      	ldr	r1, [pc, #32]	; (30003e48 <recovery_check+0xb0>)
30003e26:	e7c1      	b.n	30003dac <recovery_check+0x14>
30003e28:	464b      	mov	r3, r9
30003e2a:	e7e6      	b.n	30003dfa <recovery_check+0x62>
30003e2c:	f8d6 3006 	ldr.w	r3, [r6, #6]
30003e30:	2001      	movs	r0, #1
30003e32:	f8c8 3000 	str.w	r3, [r8]
30003e36:	e7f2      	b.n	30003e1e <recovery_check+0x86>
30003e38:	3000dc54 	.word	0x3000dc54
30003e3c:	3000e164 	.word	0x3000e164
30003e40:	3000be44 	.word	0x3000be44
30003e44:	3000be78 	.word	0x3000be78
30003e48:	3000be3c 	.word	0x3000be3c

30003e4c <rewrite_bp>:
30003e4c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003e50:	2401      	movs	r4, #1
30003e52:	b0a3      	sub	sp, #140	; 0x8c
30003e54:	4689      	mov	r9, r1
30003e56:	4615      	mov	r5, r2
30003e58:	f8dd a0b0 	ldr.w	sl, [sp, #176]	; 0xb0
30003e5c:	4698      	mov	r8, r3
30003e5e:	f89d b0b4 	ldrb.w	fp, [sp, #180]	; 0xb4
30003e62:	4607      	mov	r7, r0
30003e64:	f88d 4007 	strb.w	r4, [sp, #7]
30003e68:	2800      	cmp	r0, #0
30003e6a:	d14f      	bne.n	30003f0c <rewrite_bp+0xc0>
30003e6c:	f8d1 1006 	ldr.w	r1, [r1, #6]
30003e70:	2965      	cmp	r1, #101	; 0x65
30003e72:	d07c      	beq.n	30003f6e <rewrite_bp+0x122>
30003e74:	4e47      	ldr	r6, [pc, #284]	; (30003f94 <rewrite_bp+0x148>)
30003e76:	4848      	ldr	r0, [pc, #288]	; (30003f98 <rewrite_bp+0x14c>)
30003e78:	f007 fe3a 	bl	3000baf0 <__DiagPrintf_veneer>
30003e7c:	f8c6 5404 	str.w	r5, [r6, #1028]	; 0x404
30003e80:	f8c6 9400 	str.w	r9, [r6, #1024]	; 0x400
30003e84:	ad02      	add	r5, sp, #8
30003e86:	f1bb 0f00 	cmp.w	fp, #0
30003e8a:	d050      	beq.n	30003f2e <rewrite_bp+0xe2>
30003e8c:	2280      	movs	r2, #128	; 0x80
30003e8e:	4641      	mov	r1, r8
30003e90:	4628      	mov	r0, r5
30003e92:	f8df 9124 	ldr.w	r9, [pc, #292]	; 30003fb8 <rewrite_bp+0x16c>
30003e96:	f007 fe1b 	bl	3000bad0 <____wrap_memcpy_veneer>
30003e9a:	4b40      	ldr	r3, [pc, #256]	; (30003f9c <rewrite_bp+0x150>)
30003e9c:	f8c9 a000 	str.w	sl, [r9]
30003ea0:	f8c3 8000 	str.w	r8, [r3]
30003ea4:	2002      	movs	r0, #2
30003ea6:	f88d 7014 	strb.w	r7, [sp, #20]
30003eaa:	2701      	movs	r7, #1
30003eac:	4651      	mov	r1, sl
30003eae:	e9cd 7003 	strd	r7, r0, [sp, #12]
30003eb2:	f7ff fe8d 	bl	30003bd0 <FLASH_Erase_With_Lock>
30003eb6:	f8d9 0000 	ldr.w	r0, [r9]
30003eba:	aa03      	add	r2, sp, #12
30003ebc:	217c      	movs	r1, #124	; 0x7c
30003ebe:	3004      	adds	r0, #4
30003ec0:	f7ff feae 	bl	30003c20 <FLASH_TxData_With_Lock>
30003ec4:	f8d9 0000 	ldr.w	r0, [r9]
30003ec8:	4639      	mov	r1, r7
30003eca:	f10d 0207 	add.w	r2, sp, #7
30003ece:	f600 70ff 	addw	r0, r0, #4095	; 0xfff
30003ed2:	f7ff fea5 	bl	30003c20 <FLASH_TxData_With_Lock>
30003ed6:	f8d9 0000 	ldr.w	r0, [r9]
30003eda:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30003ede:	1d01      	adds	r1, r0, #4
30003ee0:	f500 5c80 	add.w	ip, r0, #4096	; 0x1000
30003ee4:	f811 3b01 	ldrb.w	r3, [r1], #1
30003ee8:	4053      	eors	r3, r2
30003eea:	4561      	cmp	r1, ip
30003eec:	b2db      	uxtb	r3, r3
30003eee:	f856 3023 	ldr.w	r3, [r6, r3, lsl #2]
30003ef2:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30003ef6:	d1f5      	bne.n	30003ee4 <rewrite_bp+0x98>
30003ef8:	43d3      	mvns	r3, r2
30003efa:	2104      	movs	r1, #4
30003efc:	462a      	mov	r2, r5
30003efe:	9302      	str	r3, [sp, #8]
30003f00:	f7ff fe8e 	bl	30003c20 <FLASH_TxData_With_Lock>
30003f04:	4620      	mov	r0, r4
30003f06:	b023      	add	sp, #140	; 0x8c
30003f08:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003f0c:	f8d2 1006 	ldr.w	r1, [r2, #6]
30003f10:	2965      	cmp	r1, #101	; 0x65
30003f12:	d01a      	beq.n	30003f4a <rewrite_bp+0xfe>
30003f14:	4e1f      	ldr	r6, [pc, #124]	; (30003f94 <rewrite_bp+0x148>)
30003f16:	2402      	movs	r4, #2
30003f18:	4821      	ldr	r0, [pc, #132]	; (30003fa0 <rewrite_bp+0x154>)
30003f1a:	f007 fde9 	bl	3000baf0 <__DiagPrintf_veneer>
30003f1e:	f8c6 5404 	str.w	r5, [r6, #1028]	; 0x404
30003f22:	f8c6 9400 	str.w	r9, [r6, #1024]	; 0x400
30003f26:	ad02      	add	r5, sp, #8
30003f28:	f1bb 0f00 	cmp.w	fp, #0
30003f2c:	d1ae      	bne.n	30003e8c <rewrite_bp+0x40>
30003f2e:	4651      	mov	r1, sl
30003f30:	2280      	movs	r2, #128	; 0x80
30003f32:	4628      	mov	r0, r5
30003f34:	f8df 9080 	ldr.w	r9, [pc, #128]	; 30003fb8 <rewrite_bp+0x16c>
30003f38:	f007 fdca 	bl	3000bad0 <____wrap_memcpy_veneer>
30003f3c:	4b17      	ldr	r3, [pc, #92]	; (30003f9c <rewrite_bp+0x150>)
30003f3e:	f8c9 8000 	str.w	r8, [r9]
30003f42:	f8c3 a000 	str.w	sl, [r3]
30003f46:	46c2      	mov	sl, r8
30003f48:	e7ac      	b.n	30003ea4 <rewrite_bp+0x58>
30003f4a:	4816      	ldr	r0, [pc, #88]	; (30003fa4 <rewrite_bp+0x158>)
30003f4c:	f007 fdd0 	bl	3000baf0 <__DiagPrintf_veneer>
30003f50:	4b15      	ldr	r3, [pc, #84]	; (30003fa8 <rewrite_bp+0x15c>)
30003f52:	a802      	add	r0, sp, #8
30003f54:	4798      	blx	r3
30003f56:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30003f5a:	4814      	ldr	r0, [pc, #80]	; (30003fac <rewrite_bp+0x160>)
30003f5c:	a902      	add	r1, sp, #8
30003f5e:	4b14      	ldr	r3, [pc, #80]	; (30003fb0 <rewrite_bp+0x164>)
30003f60:	f8ad 200a 	strh.w	r2, [sp, #10]
30003f64:	4798      	blx	r3
30003f66:	4811      	ldr	r0, [pc, #68]	; (30003fac <rewrite_bp+0x160>)
30003f68:	4b12      	ldr	r3, [pc, #72]	; (30003fb4 <rewrite_bp+0x168>)
30003f6a:	4798      	blx	r3
30003f6c:	e7fe      	b.n	30003f6c <rewrite_bp+0x120>
30003f6e:	480d      	ldr	r0, [pc, #52]	; (30003fa4 <rewrite_bp+0x158>)
30003f70:	f007 fdbe 	bl	3000baf0 <__DiagPrintf_veneer>
30003f74:	4b0c      	ldr	r3, [pc, #48]	; (30003fa8 <rewrite_bp+0x15c>)
30003f76:	a802      	add	r0, sp, #8
30003f78:	4798      	blx	r3
30003f7a:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30003f7e:	480b      	ldr	r0, [pc, #44]	; (30003fac <rewrite_bp+0x160>)
30003f80:	a902      	add	r1, sp, #8
30003f82:	4b0b      	ldr	r3, [pc, #44]	; (30003fb0 <rewrite_bp+0x164>)
30003f84:	f8ad 200a 	strh.w	r2, [sp, #10]
30003f88:	4798      	blx	r3
30003f8a:	4808      	ldr	r0, [pc, #32]	; (30003fac <rewrite_bp+0x160>)
30003f8c:	4b09      	ldr	r3, [pc, #36]	; (30003fb4 <rewrite_bp+0x168>)
30003f8e:	4798      	blx	r3
30003f90:	e7fe      	b.n	30003f90 <rewrite_bp+0x144>
30003f92:	bf00      	nop
30003f94:	3000e164 	.word	0x3000e164
30003f98:	3000bf04 	.word	0x3000bf04
30003f9c:	3000f4ac 	.word	0x3000f4ac
30003fa0:	3000bf1c 	.word	0x3000bf1c
30003fa4:	3000beb0 	.word	0x3000beb0
30003fa8:	0000d3f9 	.word	0x0000d3f9
30003fac:	41000440 	.word	0x41000440
30003fb0:	0000d409 	.word	0x0000d409
30003fb4:	0000d489 	.word	0x0000d489
30003fb8:	3000f4b0 	.word	0x3000f4b0

30003fbc <BOOT_SectionInit>:
30003fbc:	4800      	ldr	r0, [pc, #0]	; (30003fc0 <BOOT_SectionInit+0x4>)
30003fbe:	4770      	bx	lr
30003fc0:	60000020 	.word	0x60000020

30003fc4 <BOOT_NsStart>:
30003fc4:	b570      	push	{r4, r5, r6, lr}
30003fc6:	4c23      	ldr	r4, [pc, #140]	; (30004054 <BOOT_NsStart+0x90>)
30003fc8:	4605      	mov	r5, r0
30003fca:	f384 8808 	msr	MSP, r4
30003fce:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003fd2:	2100      	movs	r1, #0
30003fd4:	4620      	mov	r0, r4
30003fd6:	4e20      	ldr	r6, [pc, #128]	; (30004058 <BOOT_NsStart+0x94>)
30003fd8:	47b0      	blx	r6
30003fda:	4a20      	ldr	r2, [pc, #128]	; (3000405c <BOOT_NsStart+0x98>)
30003fdc:	6953      	ldr	r3, [r2, #20]
30003fde:	03d9      	lsls	r1, r3, #15
30003fe0:	d50b      	bpl.n	30003ffa <BOOT_NsStart+0x36>
30003fe2:	f3bf 8f4f 	dsb	sy
30003fe6:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
30003fea:	3420      	adds	r4, #32
30003fec:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
30003ff0:	d1f9      	bne.n	30003fe6 <BOOT_NsStart+0x22>
30003ff2:	f3bf 8f4f 	dsb	sy
30003ff6:	f3bf 8f6f 	isb	sy
30003ffa:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
30003ffe:	f383 8808 	msr	MSP, r3
30004002:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30004006:	2100      	movs	r1, #0
30004008:	4815      	ldr	r0, [pc, #84]	; (30004060 <BOOT_NsStart+0x9c>)
3000400a:	47b0      	blx	r6
3000400c:	4a13      	ldr	r2, [pc, #76]	; (3000405c <BOOT_NsStart+0x98>)
3000400e:	6953      	ldr	r3, [r2, #20]
30004010:	03db      	lsls	r3, r3, #15
30004012:	d50c      	bpl.n	3000402e <BOOT_NsStart+0x6a>
30004014:	f3bf 8f4f 	dsb	sy
30004018:	4b11      	ldr	r3, [pc, #68]	; (30004060 <BOOT_NsStart+0x9c>)
3000401a:	490e      	ldr	r1, [pc, #56]	; (30004054 <BOOT_NsStart+0x90>)
3000401c:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30004020:	3320      	adds	r3, #32
30004022:	428b      	cmp	r3, r1
30004024:	d1fa      	bne.n	3000401c <BOOT_NsStart+0x58>
30004026:	f3bf 8f4f 	dsb	sy
3000402a:	f3bf 8f6f 	isb	sy
3000402e:	f025 0501 	bic.w	r5, r5, #1
30004032:	086d      	lsrs	r5, r5, #1
30004034:	006d      	lsls	r5, r5, #1
30004036:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
3000403a:	ed2d 8b10 	vpush	{d8-d15}
3000403e:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
30004042:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
30004046:	47ac      	blxns	r5
30004048:	ecbd 8b10 	vpop	{d8-d15}
3000404c:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004050:	bd70      	pop	{r4, r5, r6, pc}
30004052:	bf00      	nop
30004054:	30002000 	.word	0x30002000
30004058:	00012be5 	.word	0x00012be5
3000405c:	e000ed00 	.word	0xe000ed00
30004060:	30001000 	.word	0x30001000

30004064 <BOOT_RccConfig>:
30004064:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004068:	4c2f      	ldr	r4, [pc, #188]	; (30004128 <BOOT_RccConfig+0xc4>)
3000406a:	b090      	sub	sp, #64	; 0x40
3000406c:	4f2f      	ldr	r7, [pc, #188]	; (3000412c <BOOT_RccConfig+0xc8>)
3000406e:	466d      	mov	r5, sp
30004070:	f10d 0c20 	add.w	ip, sp, #32
30004074:	683e      	ldr	r6, [r7, #0]
30004076:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
3000407a:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
3000407e:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30004082:	f104 0310 	add.w	r3, r4, #16
30004086:	2400      	movs	r4, #0
30004088:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000408a:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
3000408e:	1c72      	adds	r2, r6, #1
30004090:	e9cd 4404 	strd	r4, r4, [sp, #16]
30004094:	e9cd 4406 	strd	r4, r4, [sp, #24]
30004098:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000409c:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
300040a0:	d03f      	beq.n	30004122 <BOOT_RccConfig+0xbe>
300040a2:	463a      	mov	r2, r7
300040a4:	6891      	ldr	r1, [r2, #8]
300040a6:	0fb3      	lsrs	r3, r6, #30
300040a8:	a810      	add	r0, sp, #64	; 0x40
300040aa:	2901      	cmp	r1, #1
300040ac:	eb00 0383 	add.w	r3, r0, r3, lsl #2
300040b0:	d10e      	bne.n	300040d0 <BOOT_RccConfig+0x6c>
300040b2:	f853 0c30 	ldr.w	r0, [r3, #-48]
300040b6:	6854      	ldr	r4, [r2, #4]
300040b8:	4330      	orrs	r0, r6
300040ba:	ae10      	add	r6, sp, #64	; 0x40
300040bc:	0fa1      	lsrs	r1, r4, #30
300040be:	f843 0c30 	str.w	r0, [r3, #-48]
300040c2:	eb06 0181 	add.w	r1, r6, r1, lsl #2
300040c6:	f851 3c10 	ldr.w	r3, [r1, #-16]
300040ca:	4323      	orrs	r3, r4
300040cc:	f841 3c10 	str.w	r3, [r1, #-16]
300040d0:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300040d4:	1c73      	adds	r3, r6, #1
300040d6:	d1e5      	bne.n	300040a4 <BOOT_RccConfig+0x40>
300040d8:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300040da:	9c04      	ldr	r4, [sp, #16]
300040dc:	af0d      	add	r7, sp, #52	; 0x34
300040de:	ae05      	add	r6, sp, #20
300040e0:	4a13      	ldr	r2, [pc, #76]	; (30004130 <BOOT_RccConfig+0xcc>)
300040e2:	f85c 0b04 	ldr.w	r0, [ip], #4
300040e6:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300040ea:	f855 1b04 	ldr.w	r1, [r5], #4
300040ee:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300040f2:	f850 8002 	ldr.w	r8, [r0, r2]
300040f6:	45e6      	cmp	lr, ip
300040f8:	ea43 0308 	orr.w	r3, r3, r8
300040fc:	5083      	str	r3, [r0, r2]
300040fe:	588b      	ldr	r3, [r1, r2]
30004100:	ea44 0403 	orr.w	r4, r4, r3
30004104:	508c      	str	r4, [r1, r2]
30004106:	d004      	beq.n	30004112 <BOOT_RccConfig+0xae>
30004108:	f857 3b04 	ldr.w	r3, [r7], #4
3000410c:	f856 4b04 	ldr.w	r4, [r6], #4
30004110:	e7e7      	b.n	300040e2 <BOOT_RccConfig+0x7e>
30004112:	200a      	movs	r0, #10
30004114:	4b07      	ldr	r3, [pc, #28]	; (30004134 <BOOT_RccConfig+0xd0>)
30004116:	4798      	blx	r3
30004118:	4b07      	ldr	r3, [pc, #28]	; (30004138 <BOOT_RccConfig+0xd4>)
3000411a:	b010      	add	sp, #64	; 0x40
3000411c:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30004120:	4718      	bx	r3
30004122:	4623      	mov	r3, r4
30004124:	e7da      	b.n	300040dc <BOOT_RccConfig+0x78>
30004126:	bf00      	nop
30004128:	3000dc34 	.word	0x3000dc34
3000412c:	3000dfa8 	.word	0x3000dfa8
30004130:	42008000 	.word	0x42008000
30004134:	00009b2d 	.word	0x00009b2d
30004138:	30007591 	.word	0x30007591

3000413c <BOOT_CACHEWRR_Set>:
3000413c:	4b14      	ldr	r3, [pc, #80]	; (30004190 <BOOT_CACHEWRR_Set+0x54>)
3000413e:	b430      	push	{r4, r5}
30004140:	681a      	ldr	r2, [r3, #0]
30004142:	2a01      	cmp	r2, #1
30004144:	d10f      	bne.n	30004166 <BOOT_CACHEWRR_Set+0x2a>
30004146:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
3000414a:	685d      	ldr	r5, [r3, #4]
3000414c:	689c      	ldr	r4, [r3, #8]
3000414e:	6882      	ldr	r2, [r0, #8]
30004150:	4910      	ldr	r1, [pc, #64]	; (30004194 <BOOT_CACHEWRR_Set+0x58>)
30004152:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004156:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
3000415a:	6082      	str	r2, [r0, #8]
3000415c:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30004160:	68da      	ldr	r2, [r3, #12]
30004162:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
30004166:	691a      	ldr	r2, [r3, #16]
30004168:	2a01      	cmp	r2, #1
3000416a:	d10f      	bne.n	3000418c <BOOT_CACHEWRR_Set+0x50>
3000416c:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30004170:	695d      	ldr	r5, [r3, #20]
30004172:	4a08      	ldr	r2, [pc, #32]	; (30004194 <BOOT_CACHEWRR_Set+0x58>)
30004174:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30004178:	688b      	ldr	r3, [r1, #8]
3000417a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000417e:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
30004182:	608b      	str	r3, [r1, #8]
30004184:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30004188:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
3000418c:	bc30      	pop	{r4, r5}
3000418e:	4770      	bx	lr
30004190:	3000e654 	.word	0x3000e654
30004194:	e0042000 	.word	0xe0042000

30004198 <BOOT_TCMSet>:
30004198:	2300      	movs	r3, #0
3000419a:	b5f0      	push	{r4, r5, r6, r7, lr}
3000419c:	4db3      	ldr	r5, [pc, #716]	; (3000446c <BOOT_TCMSet+0x2d4>)
3000419e:	461c      	mov	r4, r3
300041a0:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
300041a4:	3307      	adds	r3, #7
300041a6:	4282      	cmp	r2, r0
300041a8:	d03c      	beq.n	30004224 <BOOT_TCMSet+0x8c>
300041aa:	3401      	adds	r4, #1
300041ac:	2c07      	cmp	r4, #7
300041ae:	d1f7      	bne.n	300041a0 <BOOT_TCMSet+0x8>
300041b0:	f3bf 8f4f 	dsb	sy
300041b4:	f3bf 8f6f 	isb	sy
300041b8:	48ad      	ldr	r0, [pc, #692]	; (30004470 <BOOT_TCMSet+0x2d8>)
300041ba:	2200      	movs	r2, #0
300041bc:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
300041c0:	f3bf 8f4f 	dsb	sy
300041c4:	f3bf 8f6f 	isb	sy
300041c8:	6943      	ldr	r3, [r0, #20]
300041ca:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
300041ce:	6143      	str	r3, [r0, #20]
300041d0:	f3bf 8f4f 	dsb	sy
300041d4:	f3bf 8f6f 	isb	sy
300041d8:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300041dc:	f3bf 8f4f 	dsb	sy
300041e0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300041e4:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300041e8:	f3c5 344e 	ubfx	r4, r5, #13, #15
300041ec:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300041f0:	0164      	lsls	r4, r4, #5
300041f2:	ea04 0106 	and.w	r1, r4, r6
300041f6:	462b      	mov	r3, r5
300041f8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300041fc:	3b01      	subs	r3, #1
300041fe:	1c5f      	adds	r7, r3, #1
30004200:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30004204:	d1f8      	bne.n	300041f8 <BOOT_TCMSet+0x60>
30004206:	3c20      	subs	r4, #32
30004208:	f114 0f20 	cmn.w	r4, #32
3000420c:	d1f1      	bne.n	300041f2 <BOOT_TCMSet+0x5a>
3000420e:	f3bf 8f4f 	dsb	sy
30004212:	6943      	ldr	r3, [r0, #20]
30004214:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30004218:	6143      	str	r3, [r0, #20]
3000421a:	f3bf 8f4f 	dsb	sy
3000421e:	f3bf 8f6f 	isb	sy
30004222:	bdf0      	pop	{r4, r5, r6, r7, pc}
30004224:	2c00      	cmp	r4, #0
30004226:	d0c3      	beq.n	300041b0 <BOOT_TCMSet+0x18>
30004228:	f3bf 8f4f 	dsb	sy
3000422c:	f3bf 8f6f 	isb	sy
30004230:	4890      	ldr	r0, [pc, #576]	; (30004474 <BOOT_TCMSet+0x2dc>)
30004232:	2300      	movs	r3, #0
30004234:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30004238:	f3bf 8f4f 	dsb	sy
3000423c:	f3bf 8f6f 	isb	sy
30004240:	6942      	ldr	r2, [r0, #20]
30004242:	03d2      	lsls	r2, r2, #15
30004244:	d520      	bpl.n	30004288 <BOOT_TCMSet+0xf0>
30004246:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000424a:	f3bf 8f4f 	dsb	sy
3000424e:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30004252:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004256:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000425a:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000425e:	ea4f 1c43 	mov.w	ip, r3, lsl #5
30004262:	ea0c 0106 	and.w	r1, ip, r6
30004266:	4673      	mov	r3, lr
30004268:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000426c:	3b01      	subs	r3, #1
3000426e:	1c5f      	adds	r7, r3, #1
30004270:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004274:	d1f8      	bne.n	30004268 <BOOT_TCMSet+0xd0>
30004276:	f1ac 0c20 	sub.w	ip, ip, #32
3000427a:	f11c 0f20 	cmn.w	ip, #32
3000427e:	d1f0      	bne.n	30004262 <BOOT_TCMSet+0xca>
30004280:	f3bf 8f4f 	dsb	sy
30004284:	f3bf 8f6f 	isb	sy
30004288:	f3bf 8f4f 	dsb	sy
3000428c:	f3bf 8f6f 	isb	sy
30004290:	4878      	ldr	r0, [pc, #480]	; (30004474 <BOOT_TCMSet+0x2dc>)
30004292:	2200      	movs	r2, #0
30004294:	6943      	ldr	r3, [r0, #20]
30004296:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
3000429a:	6143      	str	r3, [r0, #20]
3000429c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
300042a0:	f3bf 8f4f 	dsb	sy
300042a4:	f3bf 8f6f 	isb	sy
300042a8:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300042ac:	f3bf 8f4f 	dsb	sy
300042b0:	6943      	ldr	r3, [r0, #20]
300042b2:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300042b6:	6143      	str	r3, [r0, #20]
300042b8:	f3bf 8f4f 	dsb	sy
300042bc:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
300042c0:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300042c4:	f3c3 06c9 	ubfx	r6, r3, #3, #10
300042c8:	f3c3 334e 	ubfx	r3, r3, #13, #15
300042cc:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300042d0:	ea0c 010e 	and.w	r1, ip, lr
300042d4:	4633      	mov	r3, r6
300042d6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300042da:	3b01      	subs	r3, #1
300042dc:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300042e0:	1c5a      	adds	r2, r3, #1
300042e2:	d1f8      	bne.n	300042d6 <BOOT_TCMSet+0x13e>
300042e4:	f1ac 0c20 	sub.w	ip, ip, #32
300042e8:	f11c 0f20 	cmn.w	ip, #32
300042ec:	d1f0      	bne.n	300042d0 <BOOT_TCMSet+0x138>
300042ee:	f3bf 8f4f 	dsb	sy
300042f2:	f3bf 8f6f 	isb	sy
300042f6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300042fa:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300042fe:	00e3      	lsls	r3, r4, #3
30004300:	6b41      	ldr	r1, [r0, #52]	; 0x34
30004302:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004306:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
3000430a:	6341      	str	r1, [r0, #52]	; 0x34
3000430c:	6911      	ldr	r1, [r2, #16]
3000430e:	2903      	cmp	r1, #3
30004310:	d00f      	beq.n	30004332 <BOOT_TCMSet+0x19a>
30004312:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
30004316:	4858      	ldr	r0, [pc, #352]	; (30004478 <BOOT_TCMSet+0x2e0>)
30004318:	f04f 0c00 	mov.w	ip, #0
3000431c:	4f57      	ldr	r7, [pc, #348]	; (3000447c <BOOT_TCMSet+0x2e4>)
3000431e:	68b2      	ldr	r2, [r6, #8]
30004320:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
30004324:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
30004328:	60b2      	str	r2, [r6, #8]
3000432a:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
3000432e:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
30004332:	1b1a      	subs	r2, r3, r4
30004334:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004338:	68d1      	ldr	r1, [r2, #12]
3000433a:	2903      	cmp	r1, #3
3000433c:	d00f      	beq.n	3000435e <BOOT_TCMSet+0x1c6>
3000433e:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
30004342:	484d      	ldr	r0, [pc, #308]	; (30004478 <BOOT_TCMSet+0x2e0>)
30004344:	f04f 0c00 	mov.w	ip, #0
30004348:	4f4c      	ldr	r7, [pc, #304]	; (3000447c <BOOT_TCMSet+0x2e4>)
3000434a:	68b2      	ldr	r2, [r6, #8]
3000434c:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004350:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
30004354:	60b2      	str	r2, [r6, #8]
30004356:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
3000435a:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
3000435e:	1b1a      	subs	r2, r3, r4
30004360:	4945      	ldr	r1, [pc, #276]	; (30004478 <BOOT_TCMSet+0x2e0>)
30004362:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004366:	6950      	ldr	r0, [r2, #20]
30004368:	6608      	str	r0, [r1, #96]	; 0x60
3000436a:	6990      	ldr	r0, [r2, #24]
3000436c:	6648      	str	r0, [r1, #100]	; 0x64
3000436e:	6852      	ldr	r2, [r2, #4]
30004370:	2a01      	cmp	r2, #1
30004372:	d04b      	beq.n	3000440c <BOOT_TCMSet+0x274>
30004374:	1b1b      	subs	r3, r3, r4
30004376:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000437a:	68ab      	ldr	r3, [r5, #8]
3000437c:	2b01      	cmp	r3, #1
3000437e:	f47f af50 	bne.w	30004222 <BOOT_TCMSet+0x8a>
30004382:	483b      	ldr	r0, [pc, #236]	; (30004470 <BOOT_TCMSet+0x2d8>)
30004384:	2300      	movs	r3, #0
30004386:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000438a:	f3bf 8f4f 	dsb	sy
3000438e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004392:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004396:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000439a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000439e:	0164      	lsls	r4, r4, #5
300043a0:	ea04 0106 	and.w	r1, r4, r6
300043a4:	462b      	mov	r3, r5
300043a6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300043aa:	3b01      	subs	r3, #1
300043ac:	1c5f      	adds	r7, r3, #1
300043ae:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300043b2:	d1f8      	bne.n	300043a6 <BOOT_TCMSet+0x20e>
300043b4:	3c20      	subs	r4, #32
300043b6:	f114 0f20 	cmn.w	r4, #32
300043ba:	d1f1      	bne.n	300043a0 <BOOT_TCMSet+0x208>
300043bc:	f3bf 8f4f 	dsb	sy
300043c0:	6943      	ldr	r3, [r0, #20]
300043c2:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300043c6:	6143      	str	r3, [r0, #20]
300043c8:	f3bf 8f4f 	dsb	sy
300043cc:	f3bf 8f6f 	isb	sy
300043d0:	4828      	ldr	r0, [pc, #160]	; (30004474 <BOOT_TCMSet+0x2dc>)
300043d2:	2300      	movs	r3, #0
300043d4:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300043d8:	f3bf 8f4f 	dsb	sy
300043dc:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300043e0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300043e4:	f3c5 344e 	ubfx	r4, r5, #13, #15
300043e8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300043ec:	0164      	lsls	r4, r4, #5
300043ee:	ea04 0106 	and.w	r1, r4, r6
300043f2:	462b      	mov	r3, r5
300043f4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300043f8:	3b01      	subs	r3, #1
300043fa:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300043fe:	1c5a      	adds	r2, r3, #1
30004400:	d1f8      	bne.n	300043f4 <BOOT_TCMSet+0x25c>
30004402:	3c20      	subs	r4, #32
30004404:	f114 0f20 	cmn.w	r4, #32
30004408:	d1f1      	bne.n	300043ee <BOOT_TCMSet+0x256>
3000440a:	e700      	b.n	3000420e <BOOT_TCMSet+0x76>
3000440c:	f3bf 8f4f 	dsb	sy
30004410:	f3bf 8f6f 	isb	sy
30004414:	4a16      	ldr	r2, [pc, #88]	; (30004470 <BOOT_TCMSet+0x2d8>)
30004416:	2000      	movs	r0, #0
30004418:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
3000441c:	f3bf 8f4f 	dsb	sy
30004420:	f3bf 8f6f 	isb	sy
30004424:	6951      	ldr	r1, [r2, #20]
30004426:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
3000442a:	6151      	str	r1, [r2, #20]
3000442c:	f3bf 8f4f 	dsb	sy
30004430:	f3bf 8f6f 	isb	sy
30004434:	f3bf 8f4f 	dsb	sy
30004438:	f3bf 8f6f 	isb	sy
3000443c:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30004440:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30004444:	f3bf 8f4f 	dsb	sy
30004448:	f3bf 8f6f 	isb	sy
3000444c:	6951      	ldr	r1, [r2, #20]
3000444e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30004452:	6151      	str	r1, [r2, #20]
30004454:	f3bf 8f4f 	dsb	sy
30004458:	f3bf 8f6f 	isb	sy
3000445c:	1b1b      	subs	r3, r3, r4
3000445e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30004462:	68ab      	ldr	r3, [r5, #8]
30004464:	2b01      	cmp	r3, #1
30004466:	f47f aedc 	bne.w	30004222 <BOOT_TCMSet+0x8a>
3000446a:	e78a      	b.n	30004382 <BOOT_TCMSet+0x1ea>
3000446c:	3000e590 	.word	0x3000e590
30004470:	e002ed00 	.word	0xe002ed00
30004474:	e000ed00 	.word	0xe000ed00
30004478:	e0042000 	.word	0xe0042000
3000447c:	ffff0000 	.word	0xffff0000

30004480 <BOOT_GRstConfig>:
30004480:	b538      	push	{r3, r4, r5, lr}
30004482:	4c09      	ldr	r4, [pc, #36]	; (300044a8 <BOOT_GRstConfig+0x28>)
30004484:	f241 050a 	movw	r5, #4106	; 0x100a
30004488:	2028      	movs	r0, #40	; 0x28
3000448a:	4a08      	ldr	r2, [pc, #32]	; (300044ac <BOOT_GRstConfig+0x2c>)
3000448c:	8823      	ldrh	r3, [r4, #0]
3000448e:	b29b      	uxth	r3, r3
30004490:	431d      	orrs	r5, r3
30004492:	8025      	strh	r5, [r4, #0]
30004494:	4790      	blx	r2
30004496:	88a3      	ldrh	r3, [r4, #4]
30004498:	b29b      	uxth	r3, r3
3000449a:	ea23 0305 	bic.w	r3, r3, r5
3000449e:	80a3      	strh	r3, [r4, #4]
300044a0:	8863      	ldrh	r3, [r4, #2]
300044a2:	8065      	strh	r5, [r4, #2]
300044a4:	bd38      	pop	{r3, r4, r5, pc}
300044a6:	bf00      	nop
300044a8:	42008200 	.word	0x42008200
300044ac:	00009b2d 	.word	0x00009b2d

300044b0 <BOOT_DDR_LCDC_HPR>:
300044b0:	4a04      	ldr	r2, [pc, #16]	; (300044c4 <BOOT_DDR_LCDC_HPR+0x14>)
300044b2:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300044b6:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300044ba:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300044be:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300044c2:	4770      	bx	lr
300044c4:	42008000 	.word	0x42008000

300044c8 <BOOT_DDR_Init>:
300044c8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300044cc:	b530      	push	{r4, r5, lr}
300044ce:	4d15      	ldr	r5, [pc, #84]	; (30004524 <BOOT_DDR_Init+0x5c>)
300044d0:	b099      	sub	sp, #100	; 0x64
300044d2:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300044d4:	47a8      	blx	r5
300044d6:	2803      	cmp	r0, #3
300044d8:	d11e      	bne.n	30004518 <BOOT_DDR_Init+0x50>
300044da:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300044de:	4a12      	ldr	r2, [pc, #72]	; (30004528 <BOOT_DDR_Init+0x60>)
300044e0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300044e4:	4911      	ldr	r1, [pc, #68]	; (3000452c <BOOT_DDR_Init+0x64>)
300044e6:	a801      	add	r0, sp, #4
300044e8:	631c      	str	r4, [r3, #48]	; 0x30
300044ea:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300044ee:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300044f2:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300044f6:	4788      	blx	r1
300044f8:	a801      	add	r0, sp, #4
300044fa:	4b0d      	ldr	r3, [pc, #52]	; (30004530 <BOOT_DDR_Init+0x68>)
300044fc:	4798      	blx	r3
300044fe:	4b0d      	ldr	r3, [pc, #52]	; (30004534 <BOOT_DDR_Init+0x6c>)
30004500:	4798      	blx	r3
30004502:	4b0d      	ldr	r3, [pc, #52]	; (30004538 <BOOT_DDR_Init+0x70>)
30004504:	4798      	blx	r3
30004506:	4b0d      	ldr	r3, [pc, #52]	; (3000453c <BOOT_DDR_Init+0x74>)
30004508:	4798      	blx	r3
3000450a:	2101      	movs	r1, #1
3000450c:	f44f 60e0 	mov.w	r0, #1792	; 0x700
30004510:	4b0b      	ldr	r3, [pc, #44]	; (30004540 <BOOT_DDR_Init+0x78>)
30004512:	4798      	blx	r3
30004514:	b019      	add	sp, #100	; 0x64
30004516:	bd30      	pop	{r4, r5, pc}
30004518:	47a8      	blx	r5
3000451a:	2802      	cmp	r0, #2
3000451c:	d0dd      	beq.n	300044da <BOOT_DDR_Init+0x12>
3000451e:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
30004522:	e7dc      	b.n	300044de <BOOT_DDR_Init+0x16>
30004524:	0000c0f9 	.word	0x0000c0f9
30004528:	42008000 	.word	0x42008000
3000452c:	300076ad 	.word	0x300076ad
30004530:	30007c25 	.word	0x30007c25
30004534:	3000a9e5 	.word	0x3000a9e5
30004538:	30007cf5 	.word	0x30007cf5
3000453c:	30007cf9 	.word	0x30007cf9
30004540:	3000a945 	.word	0x3000a945

30004544 <BOOT_PSRAM_Init>:
30004544:	b500      	push	{lr}
30004546:	b089      	sub	sp, #36	; 0x24
30004548:	4b1b      	ldr	r3, [pc, #108]	; (300045b8 <BOOT_PSRAM_Init+0x74>)
3000454a:	a801      	add	r0, sp, #4
3000454c:	4798      	blx	r3
3000454e:	a801      	add	r0, sp, #4
30004550:	4b1a      	ldr	r3, [pc, #104]	; (300045bc <BOOT_PSRAM_Init+0x78>)
30004552:	4798      	blx	r3
30004554:	4b1a      	ldr	r3, [pc, #104]	; (300045c0 <BOOT_PSRAM_Init+0x7c>)
30004556:	4798      	blx	r3
30004558:	4b1a      	ldr	r3, [pc, #104]	; (300045c4 <BOOT_PSRAM_Init+0x80>)
3000455a:	4798      	blx	r3
3000455c:	2801      	cmp	r0, #1
3000455e:	d01d      	beq.n	3000459c <BOOT_PSRAM_Init+0x58>
30004560:	4b19      	ldr	r3, [pc, #100]	; (300045c8 <BOOT_PSRAM_Init+0x84>)
30004562:	2004      	movs	r0, #4
30004564:	4a19      	ldr	r2, [pc, #100]	; (300045cc <BOOT_PSRAM_Init+0x88>)
30004566:	4619      	mov	r1, r3
30004568:	f007 f824 	bl	3000b5b4 <rtk_log_write>
3000456c:	4b18      	ldr	r3, [pc, #96]	; (300045d0 <BOOT_PSRAM_Init+0x8c>)
3000456e:	4798      	blx	r3
30004570:	4b18      	ldr	r3, [pc, #96]	; (300045d4 <BOOT_PSRAM_Init+0x90>)
30004572:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004576:	009b      	lsls	r3, r3, #2
30004578:	d517      	bpl.n	300045aa <BOOT_PSRAM_Init+0x66>
3000457a:	4a17      	ldr	r2, [pc, #92]	; (300045d8 <BOOT_PSRAM_Init+0x94>)
3000457c:	4b17      	ldr	r3, [pc, #92]	; (300045dc <BOOT_PSRAM_Init+0x98>)
3000457e:	6812      	ldr	r2, [r2, #0]
30004580:	605a      	str	r2, [r3, #4]
30004582:	4a17      	ldr	r2, [pc, #92]	; (300045e0 <BOOT_PSRAM_Init+0x9c>)
30004584:	f04f 0c20 	mov.w	ip, #32
30004588:	2101      	movs	r1, #1
3000458a:	4b16      	ldr	r3, [pc, #88]	; (300045e4 <BOOT_PSRAM_Init+0xa0>)
3000458c:	6952      	ldr	r2, [r2, #20]
3000458e:	4608      	mov	r0, r1
30004590:	fbbc f2f2 	udiv	r2, ip, r2
30004594:	4798      	blx	r3
30004596:	b009      	add	sp, #36	; 0x24
30004598:	f85d fb04 	ldr.w	pc, [sp], #4
3000459c:	4b12      	ldr	r3, [pc, #72]	; (300045e8 <BOOT_PSRAM_Init+0xa4>)
3000459e:	4798      	blx	r3
300045a0:	4b0c      	ldr	r3, [pc, #48]	; (300045d4 <BOOT_PSRAM_Init+0x90>)
300045a2:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
300045a6:	009b      	lsls	r3, r3, #2
300045a8:	d4e7      	bmi.n	3000457a <BOOT_PSRAM_Init+0x36>
300045aa:	4b10      	ldr	r3, [pc, #64]	; (300045ec <BOOT_PSRAM_Init+0xa8>)
300045ac:	4798      	blx	r3
300045ae:	4a0b      	ldr	r2, [pc, #44]	; (300045dc <BOOT_PSRAM_Init+0x98>)
300045b0:	4b09      	ldr	r3, [pc, #36]	; (300045d8 <BOOT_PSRAM_Init+0x94>)
300045b2:	6852      	ldr	r2, [r2, #4]
300045b4:	601a      	str	r2, [r3, #0]
300045b6:	e7e4      	b.n	30004582 <BOOT_PSRAM_Init+0x3e>
300045b8:	30008451 	.word	0x30008451
300045bc:	30008471 	.word	0x30008471
300045c0:	300084d9 	.word	0x300084d9
300045c4:	30007545 	.word	0x30007545
300045c8:	3000bd58 	.word	0x3000bd58
300045cc:	3000bf34 	.word	0x3000bf34
300045d0:	3000877d 	.word	0x3000877d
300045d4:	42008000 	.word	0x42008000
300045d8:	3000f4a8 	.word	0x3000f4a8
300045dc:	41002000 	.word	0x41002000
300045e0:	3000f3a0 	.word	0x3000f3a0
300045e4:	30008a5d 	.word	0x30008a5d
300045e8:	300086a5 	.word	0x300086a5
300045ec:	300087b9 	.word	0x300087b9

300045f0 <BOOT_WakeFromPG>:
300045f0:	b538      	push	{r3, r4, r5, lr}
300045f2:	4c46      	ldr	r4, [pc, #280]	; (3000470c <BOOT_WakeFromPG+0x11c>)
300045f4:	f002 fe2a 	bl	3000724c <BOOT_RAM_TZCfg>
300045f8:	69a3      	ldr	r3, [r4, #24]
300045fa:	b10b      	cbz	r3, 30004600 <BOOT_WakeFromPG+0x10>
300045fc:	4a44      	ldr	r2, [pc, #272]	; (30004710 <BOOT_WakeFromPG+0x120>)
300045fe:	6093      	str	r3, [r2, #8]
30004600:	4b44      	ldr	r3, [pc, #272]	; (30004714 <BOOT_WakeFromPG+0x124>)
30004602:	f06f 0c1a 	mvn.w	ip, #26
30004606:	4844      	ldr	r0, [pc, #272]	; (30004718 <BOOT_WakeFromPG+0x128>)
30004608:	f103 0e50 	add.w	lr, r3, #80	; 0x50
3000460c:	ebac 0c04 	sub.w	ip, ip, r4
30004610:	eb0c 0203 	add.w	r2, ip, r3
30004614:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30004618:	4402      	add	r2, r0
3000461a:	4573      	cmp	r3, lr
3000461c:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30004620:	d1f6      	bne.n	30004610 <BOOT_WakeFromPG+0x20>
30004622:	6822      	ldr	r2, [r4, #0]
30004624:	4b3a      	ldr	r3, [pc, #232]	; (30004710 <BOOT_WakeFromPG+0x120>)
30004626:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000462a:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
3000462e:	6002      	str	r2, [r0, #0]
30004630:	6045      	str	r5, [r0, #4]
30004632:	6081      	str	r1, [r0, #8]
30004634:	6a59      	ldr	r1, [r3, #36]	; 0x24
30004636:	4a39      	ldr	r2, [pc, #228]	; (3000471c <BOOT_WakeFromPG+0x12c>)
30004638:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000463c:	4d38      	ldr	r5, [pc, #224]	; (30004720 <BOOT_WakeFromPG+0x130>)
3000463e:	6259      	str	r1, [r3, #36]	; 0x24
30004640:	6a51      	ldr	r1, [r2, #36]	; 0x24
30004642:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004646:	6251      	str	r1, [r2, #36]	; 0x24
30004648:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000464c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004650:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004654:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004658:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000465c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004660:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004664:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004668:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000466c:	f7ff fd66 	bl	3000413c <BOOT_CACHEWRR_Set>
30004670:	4b2c      	ldr	r3, [pc, #176]	; (30004724 <BOOT_WakeFromPG+0x134>)
30004672:	6818      	ldr	r0, [r3, #0]
30004674:	f7ff fd90 	bl	30004198 <BOOT_TCMSet>
30004678:	2201      	movs	r2, #1
3000467a:	492b      	ldr	r1, [pc, #172]	; (30004728 <BOOT_WakeFromPG+0x138>)
3000467c:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30004680:	47a8      	blx	r5
30004682:	4b2a      	ldr	r3, [pc, #168]	; (3000472c <BOOT_WakeFromPG+0x13c>)
30004684:	4798      	blx	r3
30004686:	4b2a      	ldr	r3, [pc, #168]	; (30004730 <BOOT_WakeFromPG+0x140>)
30004688:	4798      	blx	r3
3000468a:	2801      	cmp	r0, #1
3000468c:	4604      	mov	r4, r0
3000468e:	d026      	beq.n	300046de <BOOT_WakeFromPG+0xee>
30004690:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30004694:	2201      	movs	r2, #1
30004696:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
3000469a:	47a8      	blx	r5
3000469c:	4a25      	ldr	r2, [pc, #148]	; (30004734 <BOOT_WakeFromPG+0x144>)
3000469e:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
300046a2:	f043 0302 	orr.w	r3, r3, #2
300046a6:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
300046aa:	f7ff ff4b 	bl	30004544 <BOOT_PSRAM_Init>
300046ae:	2096      	movs	r0, #150	; 0x96
300046b0:	4b21      	ldr	r3, [pc, #132]	; (30004738 <BOOT_WakeFromPG+0x148>)
300046b2:	4798      	blx	r3
300046b4:	4a21      	ldr	r2, [pc, #132]	; (3000473c <BOOT_WakeFromPG+0x14c>)
300046b6:	4c19      	ldr	r4, [pc, #100]	; (3000471c <BOOT_WakeFromPG+0x12c>)
300046b8:	6893      	ldr	r3, [r2, #8]
300046ba:	4921      	ldr	r1, [pc, #132]	; (30004740 <BOOT_WakeFromPG+0x150>)
300046bc:	6852      	ldr	r2, [r2, #4]
300046be:	4821      	ldr	r0, [pc, #132]	; (30004744 <BOOT_WakeFromPG+0x154>)
300046c0:	605a      	str	r2, [r3, #4]
300046c2:	60a3      	str	r3, [r4, #8]
300046c4:	4a20      	ldr	r2, [pc, #128]	; (30004748 <BOOT_WakeFromPG+0x158>)
300046c6:	61c8      	str	r0, [r1, #28]
300046c8:	f382 8888 	msr	MSP_NS, r2
300046cc:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
300046d0:	f382 8809 	msr	PSP, r2
300046d4:	6858      	ldr	r0, [r3, #4]
300046d6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300046da:	f7ff bc73 	b.w	30003fc4 <BOOT_NsStart>
300046de:	4602      	mov	r2, r0
300046e0:	491a      	ldr	r1, [pc, #104]	; (3000474c <BOOT_WakeFromPG+0x15c>)
300046e2:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300046e6:	47a8      	blx	r5
300046e8:	4622      	mov	r2, r4
300046ea:	4919      	ldr	r1, [pc, #100]	; (30004750 <BOOT_WakeFromPG+0x160>)
300046ec:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300046f0:	47a8      	blx	r5
300046f2:	f7ff fee9 	bl	300044c8 <BOOT_DDR_Init>
300046f6:	4a0f      	ldr	r2, [pc, #60]	; (30004734 <BOOT_WakeFromPG+0x144>)
300046f8:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300046fc:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30004700:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30004704:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30004708:	e7d4      	b.n	300046b4 <BOOT_WakeFromPG+0xc4>
3000470a:	bf00      	nop
3000470c:	3000f43c 	.word	0x3000f43c
30004710:	e000ed00 	.word	0xe000ed00
30004714:	3000f457 	.word	0x3000f457
30004718:	e000e100 	.word	0xe000e100
3000471c:	e002ed00 	.word	0xe002ed00
30004720:	0000b479 	.word	0x0000b479
30004724:	3000dfa0 	.word	0x3000dfa0
30004728:	40001000 	.word	0x40001000
3000472c:	0000d835 	.word	0x0000d835
30004730:	3000746d 	.word	0x3000746d
30004734:	42008000 	.word	0x42008000
30004738:	00009b2d 	.word	0x00009b2d
3000473c:	60000020 	.word	0x60000020
30004740:	30000000 	.word	0x30000000
30004744:	30003a81 	.word	0x30003a81
30004748:	2001bffc 	.word	0x2001bffc
3000474c:	40080000 	.word	0x40080000
30004750:	40040000 	.word	0x40040000

30004754 <BOOT_SCBConfig_HP>:
30004754:	4b0e      	ldr	r3, [pc, #56]	; (30004790 <BOOT_SCBConfig_HP+0x3c>)
30004756:	4a0f      	ldr	r2, [pc, #60]	; (30004794 <BOOT_SCBConfig_HP+0x40>)
30004758:	6a59      	ldr	r1, [r3, #36]	; 0x24
3000475a:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000475e:	6259      	str	r1, [r3, #36]	; 0x24
30004760:	6a51      	ldr	r1, [r2, #36]	; 0x24
30004762:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004766:	6251      	str	r1, [r2, #36]	; 0x24
30004768:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000476c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004770:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004774:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004778:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000477c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004780:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004784:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004788:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000478c:	4770      	bx	lr
3000478e:	bf00      	nop
30004790:	e000ed00 	.word	0xe000ed00
30004794:	e002ed00 	.word	0xe002ed00

30004798 <BOOT_SCBVTORBackup_HP>:
30004798:	4a02      	ldr	r2, [pc, #8]	; (300047a4 <BOOT_SCBVTORBackup_HP+0xc>)
3000479a:	4b03      	ldr	r3, [pc, #12]	; (300047a8 <BOOT_SCBVTORBackup_HP+0x10>)
3000479c:	6892      	ldr	r2, [r2, #8]
3000479e:	619a      	str	r2, [r3, #24]
300047a0:	4770      	bx	lr
300047a2:	bf00      	nop
300047a4:	e000ed00 	.word	0xe000ed00
300047a8:	3000f43c 	.word	0x3000f43c

300047ac <BOOT_SCBVTORReFill_HP>:
300047ac:	4a02      	ldr	r2, [pc, #8]	; (300047b8 <BOOT_SCBVTORReFill_HP+0xc>)
300047ae:	4b03      	ldr	r3, [pc, #12]	; (300047bc <BOOT_SCBVTORReFill_HP+0x10>)
300047b0:	6992      	ldr	r2, [r2, #24]
300047b2:	609a      	str	r2, [r3, #8]
300047b4:	4770      	bx	lr
300047b6:	bf00      	nop
300047b8:	3000f43c 	.word	0x3000f43c
300047bc:	e000ed00 	.word	0xe000ed00

300047c0 <BOOT_NVICBackup_HP>:
300047c0:	491a      	ldr	r1, [pc, #104]	; (3000482c <BOOT_NVICBackup_HP+0x6c>)
300047c2:	f06f 001a 	mvn.w	r0, #26
300047c6:	b410      	push	{r4}
300047c8:	4c19      	ldr	r4, [pc, #100]	; (30004830 <BOOT_NVICBackup_HP+0x70>)
300047ca:	680a      	ldr	r2, [r1, #0]
300047cc:	f104 031b 	add.w	r3, r4, #27
300047d0:	1b00      	subs	r0, r0, r4
300047d2:	6022      	str	r2, [r4, #0]
300047d4:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
300047d8:	684a      	ldr	r2, [r1, #4]
300047da:	6062      	str	r2, [r4, #4]
300047dc:	688a      	ldr	r2, [r1, #8]
300047de:	60a2      	str	r2, [r4, #8]
300047e0:	18c2      	adds	r2, r0, r3
300047e2:	440a      	add	r2, r1
300047e4:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
300047e8:	f803 2f01 	strb.w	r2, [r3, #1]!
300047ec:	4563      	cmp	r3, ip
300047ee:	d1f7      	bne.n	300047e0 <BOOT_NVICBackup_HP+0x20>
300047f0:	4b10      	ldr	r3, [pc, #64]	; (30004834 <BOOT_NVICBackup_HP+0x74>)
300047f2:	695b      	ldr	r3, [r3, #20]
300047f4:	03db      	lsls	r3, r3, #15
300047f6:	d512      	bpl.n	3000481e <BOOT_NVICBackup_HP+0x5e>
300047f8:	f014 011f 	ands.w	r1, r4, #31
300047fc:	4b0c      	ldr	r3, [pc, #48]	; (30004830 <BOOT_NVICBackup_HP+0x70>)
300047fe:	d111      	bne.n	30004824 <BOOT_NVICBackup_HP+0x64>
30004800:	216c      	movs	r1, #108	; 0x6c
30004802:	f3bf 8f4f 	dsb	sy
30004806:	480b      	ldr	r0, [pc, #44]	; (30004834 <BOOT_NVICBackup_HP+0x74>)
30004808:	4419      	add	r1, r3
3000480a:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
3000480e:	3320      	adds	r3, #32
30004810:	1aca      	subs	r2, r1, r3
30004812:	2a00      	cmp	r2, #0
30004814:	dcf9      	bgt.n	3000480a <BOOT_NVICBackup_HP+0x4a>
30004816:	f3bf 8f4f 	dsb	sy
3000481a:	f3bf 8f6f 	isb	sy
3000481e:	f85d 4b04 	ldr.w	r4, [sp], #4
30004822:	4770      	bx	lr
30004824:	f024 031f 	bic.w	r3, r4, #31
30004828:	316c      	adds	r1, #108	; 0x6c
3000482a:	e7ea      	b.n	30004802 <BOOT_NVICBackup_HP+0x42>
3000482c:	e000e100 	.word	0xe000e100
30004830:	3000f43c 	.word	0x3000f43c
30004834:	e000ed00 	.word	0xe000ed00

30004838 <BOOT_NVICReFill_HP>:
30004838:	b510      	push	{r4, lr}
3000483a:	f06f 0c1a 	mvn.w	ip, #26
3000483e:	4c0d      	ldr	r4, [pc, #52]	; (30004874 <BOOT_NVICReFill_HP+0x3c>)
30004840:	480d      	ldr	r0, [pc, #52]	; (30004878 <BOOT_NVICReFill_HP+0x40>)
30004842:	f104 031b 	add.w	r3, r4, #27
30004846:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
3000484a:	ebac 0c04 	sub.w	ip, ip, r4
3000484e:	eb0c 0203 	add.w	r2, ip, r3
30004852:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30004856:	4402      	add	r2, r0
30004858:	4573      	cmp	r3, lr
3000485a:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
3000485e:	d1f6      	bne.n	3000484e <BOOT_NVICReFill_HP+0x16>
30004860:	e9d4 3100 	ldrd	r3, r1, [r4]
30004864:	68a2      	ldr	r2, [r4, #8]
30004866:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000486a:	6003      	str	r3, [r0, #0]
3000486c:	6041      	str	r1, [r0, #4]
3000486e:	6082      	str	r2, [r0, #8]
30004870:	bd10      	pop	{r4, pc}
30004872:	bf00      	nop
30004874:	3000f43c 	.word	0x3000f43c
30004878:	e000e100 	.word	0xe000e100

3000487c <BOOT_ImgCopy>:
3000487c:	b510      	push	{r4, lr}
3000487e:	b082      	sub	sp, #8
30004880:	4b09      	ldr	r3, [pc, #36]	; (300048a8 <BOOT_ImgCopy+0x2c>)
30004882:	4604      	mov	r4, r0
30004884:	e9cd 2100 	strd	r2, r1, [sp]
30004888:	4798      	blx	r3
3000488a:	e9dd 2100 	ldrd	r2, r1, [sp]
3000488e:	b128      	cbz	r0, 3000489c <BOOT_ImgCopy+0x20>
30004890:	4620      	mov	r0, r4
30004892:	4b06      	ldr	r3, [pc, #24]	; (300048ac <BOOT_ImgCopy+0x30>)
30004894:	b002      	add	sp, #8
30004896:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000489a:	4718      	bx	r3
3000489c:	4620      	mov	r0, r4
3000489e:	b002      	add	sp, #8
300048a0:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300048a4:	f003 bb8e 	b.w	30007fc4 <NandImgCopy>
300048a8:	3000927d 	.word	0x3000927d
300048ac:	00012a1d 	.word	0x00012a1d

300048b0 <BOOT_LoadImages>:
300048b0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300048b4:	4b61      	ldr	r3, [pc, #388]	; (30004a3c <BOOT_LoadImages+0x18c>)
300048b6:	b0b7      	sub	sp, #220	; 0xdc
300048b8:	2400      	movs	r4, #0
300048ba:	2104      	movs	r1, #4
300048bc:	aa0f      	add	r2, sp, #60	; 0x3c
300048be:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
300048c2:	4b5f      	ldr	r3, [pc, #380]	; (30004a40 <BOOT_LoadImages+0x190>)
300048c4:	46a3      	mov	fp, r4
300048c6:	9404      	str	r4, [sp, #16]
300048c8:	9406      	str	r4, [sp, #24]
300048ca:	4798      	blx	r3
300048cc:	f89d 303e 	ldrb.w	r3, [sp, #62]	; 0x3e
300048d0:	2201      	movs	r2, #1
300048d2:	fa02 f303 	lsl.w	r3, r2, r3
300048d6:	f5b3 0f00 	cmp.w	r3, #8388608	; 0x800000
300048da:	9302      	str	r3, [sp, #8]
300048dc:	f000 81e2 	beq.w	30004ca4 <BOOT_LoadImages+0x3f4>
300048e0:	9b02      	ldr	r3, [sp, #8]
300048e2:	f1b3 7f80 	cmp.w	r3, #16777216	; 0x1000000
300048e6:	f000 83b1 	beq.w	3000504c <BOOT_LoadImages+0x79c>
300048ea:	9b02      	ldr	r3, [sp, #8]
300048ec:	f1b3 7f00 	cmp.w	r3, #33554432	; 0x2000000
300048f0:	f000 83b2 	beq.w	30005058 <BOOT_LoadImages+0x7a8>
300048f4:	9b02      	ldr	r3, [sp, #8]
300048f6:	2700      	movs	r7, #0
300048f8:	4a52      	ldr	r2, [pc, #328]	; (30004a44 <BOOT_LoadImages+0x194>)
300048fa:	4d53      	ldr	r5, [pc, #332]	; (30004a48 <BOOT_LoadImages+0x198>)
300048fc:	4853      	ldr	r0, [pc, #332]	; (30004a4c <BOOT_LoadImages+0x19c>)
300048fe:	441d      	add	r5, r3
30004900:	4413      	add	r3, r2
30004902:	461c      	mov	r4, r3
30004904:	9303      	str	r3, [sp, #12]
30004906:	2310      	movs	r3, #16
30004908:	e9cd 7710 	strd	r7, r7, [sp, #64]	; 0x40
3000490c:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
30004910:	e9cd 7712 	strd	r7, r7, [sp, #72]	; 0x48
30004914:	f7ff f9fa 	bl	30003d0c <mcc_init_crc32>
30004918:	f8d4 9004 	ldr.w	r9, [r4, #4]
3000491c:	7b23      	ldrb	r3, [r4, #12]
3000491e:	7b2e      	ldrb	r6, [r5, #12]
30004920:	9307      	str	r3, [sp, #28]
30004922:	f109 33ff 	add.w	r3, r9, #4294967295	; 0xffffffff
30004926:	686a      	ldr	r2, [r5, #4]
30004928:	3303      	adds	r3, #3
3000492a:	68ac      	ldr	r4, [r5, #8]
3000492c:	9205      	str	r2, [sp, #20]
3000492e:	f200 8154 	bhi.w	30004bda <BOOT_LoadImages+0x32a>
30004932:	9b03      	ldr	r3, [sp, #12]
30004934:	9a05      	ldr	r2, [sp, #20]
30004936:	689b      	ldr	r3, [r3, #8]
30004938:	f102 38ff 	add.w	r8, r2, #4294967295	; 0xffffffff
3000493c:	3b01      	subs	r3, #1
3000493e:	3303      	adds	r3, #3
30004940:	d873      	bhi.n	30004a2a <BOOT_LoadImages+0x17a>
30004942:	9b07      	ldr	r3, [sp, #28]
30004944:	2b01      	cmp	r3, #1
30004946:	f240 833e 	bls.w	30004fc6 <BOOT_LoadImages+0x716>
3000494a:	4841      	ldr	r0, [pc, #260]	; (30004a50 <BOOT_LoadImages+0x1a0>)
3000494c:	f007 f8d0 	bl	3000baf0 <__DiagPrintf_veneer>
30004950:	f118 0f03 	cmn.w	r8, #3
30004954:	d86f      	bhi.n	30004a36 <BOOT_LoadImages+0x186>
30004956:	3c01      	subs	r4, #1
30004958:	3403      	adds	r4, #3
3000495a:	f200 8145 	bhi.w	30004be8 <BOOT_LoadImages+0x338>
3000495e:	2e01      	cmp	r6, #1
30004960:	f200 8378 	bhi.w	30005054 <BOOT_LoadImages+0x7a4>
30004964:	9905      	ldr	r1, [sp, #20]
30004966:	483b      	ldr	r0, [pc, #236]	; (30004a54 <BOOT_LoadImages+0x1a4>)
30004968:	f007 f8c2 	bl	3000baf0 <__DiagPrintf_veneer>
3000496c:	9b03      	ldr	r3, [sp, #12]
3000496e:	4c37      	ldr	r4, [pc, #220]	; (30004a4c <BOOT_LoadImages+0x19c>)
30004970:	f8d3 200d 	ldr.w	r2, [r3, #13]
30004974:	9209      	str	r2, [sp, #36]	; 0x24
30004976:	f8d3 3011 	ldr.w	r3, [r3, #17]
3000497a:	930c      	str	r3, [sp, #48]	; 0x30
3000497c:	f8d5 300d 	ldr.w	r3, [r5, #13]
30004980:	9308      	str	r3, [sp, #32]
30004982:	f8d5 3011 	ldr.w	r3, [r5, #17]
30004986:	930b      	str	r3, [sp, #44]	; 0x2c
30004988:	2300      	movs	r3, #0
3000498a:	930a      	str	r3, [sp, #40]	; 0x28
3000498c:	9b02      	ldr	r3, [sp, #8]
3000498e:	f04f 0801 	mov.w	r8, #1
30004992:	f8df a0d4 	ldr.w	sl, [pc, #212]	; 30004a68 <BOOT_LoadImages+0x1b8>
30004996:	f103 6600 	add.w	r6, r3, #134217728	; 0x8000000
3000499a:	4f2f      	ldr	r7, [pc, #188]	; (30004a58 <BOOT_LoadImages+0x1a8>)
3000499c:	449a      	add	sl, r3
3000499e:	6829      	ldr	r1, [r5, #0]
300049a0:	4650      	mov	r0, sl
300049a2:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
300049a6:	f810 3b01 	ldrb.w	r3, [r0], #1
300049aa:	4053      	eors	r3, r2
300049ac:	42b0      	cmp	r0, r6
300049ae:	b2db      	uxtb	r3, r3
300049b0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300049b4:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300049b8:	d1f5      	bne.n	300049a6 <BOOT_LoadImages+0xf6>
300049ba:	43d2      	mvns	r2, r2
300049bc:	4291      	cmp	r1, r2
300049be:	f000 83e3 	beq.w	30005188 <BOOT_LoadImages+0x8d8>
300049c2:	4643      	mov	r3, r8
300049c4:	f108 0801 	add.w	r8, r8, #1
300049c8:	4638      	mov	r0, r7
300049ca:	f007 f891 	bl	3000baf0 <__DiagPrintf_veneer>
300049ce:	f1b8 0f04 	cmp.w	r8, #4
300049d2:	d1e4      	bne.n	3000499e <BOOT_LoadImages+0xee>
300049d4:	f04f 0800 	mov.w	r8, #0
300049d8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300049da:	2b00      	cmp	r3, #0
300049dc:	d049      	beq.n	30004a72 <BOOT_LoadImages+0x1c2>
300049de:	9b03      	ldr	r3, [sp, #12]
300049e0:	9a09      	ldr	r2, [sp, #36]	; 0x24
300049e2:	7b1b      	ldrb	r3, [r3, #12]
300049e4:	4296      	cmp	r6, r2
300049e6:	9307      	str	r3, [sp, #28]
300049e8:	d340      	bcc.n	30004a6c <BOOT_LoadImages+0x1bc>
300049ea:	f06f 4378 	mvn.w	r3, #4160749568	; 0xf8000000
300049ee:	429a      	cmp	r2, r3
300049f0:	d93c      	bls.n	30004a6c <BOOT_LoadImages+0x1bc>
300049f2:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300049f4:	4296      	cmp	r6, r2
300049f6:	f0c0 8159 	bcc.w	30004cac <BOOT_LoadImages+0x3fc>
300049fa:	429a      	cmp	r2, r3
300049fc:	f240 8156 	bls.w	30004cac <BOOT_LoadImages+0x3fc>
30004a00:	f1b8 0f00 	cmp.w	r8, #0
30004a04:	f040 84c5 	bne.w	30005392 <BOOT_LoadImages+0xae2>
30004a08:	9b07      	ldr	r3, [sp, #28]
30004a0a:	9304      	str	r3, [sp, #16]
30004a0c:	4813      	ldr	r0, [pc, #76]	; (30004a5c <BOOT_LoadImages+0x1ac>)
30004a0e:	f007 f86f 	bl	3000baf0 <__DiagPrintf_veneer>
30004a12:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004a14:	f8cd 9014 	str.w	r9, [sp, #20]
30004a18:	f8c4 3400 	str.w	r3, [r4, #1024]	; 0x400
30004a1c:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004a1e:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004a22:	462b      	mov	r3, r5
30004a24:	9d03      	ldr	r5, [sp, #12]
30004a26:	9303      	str	r3, [sp, #12]
30004a28:	e04b      	b.n	30004ac2 <BOOT_LoadImages+0x212>
30004a2a:	480d      	ldr	r0, [pc, #52]	; (30004a60 <BOOT_LoadImages+0x1b0>)
30004a2c:	f007 f860 	bl	3000baf0 <__DiagPrintf_veneer>
30004a30:	f118 0f03 	cmn.w	r8, #3
30004a34:	d98f      	bls.n	30004956 <BOOT_LoadImages+0xa6>
30004a36:	480b      	ldr	r0, [pc, #44]	; (30004a64 <BOOT_LoadImages+0x1b4>)
30004a38:	e0d7      	b.n	30004bea <BOOT_LoadImages+0x33a>
30004a3a:	bf00      	nop
30004a3c:	2001c01c 	.word	0x2001c01c
30004a40:	00009db5 	.word	0x00009db5
30004a44:	07ffe000 	.word	0x07ffe000
30004a48:	07fff000 	.word	0x07fff000
30004a4c:	3000e164 	.word	0x3000e164
30004a50:	3000bfac 	.word	0x3000bfac
30004a54:	3000c584 	.word	0x3000c584
30004a58:	3000c0bc 	.word	0x3000c0bc
30004a5c:	3000c154 	.word	0x3000c154
30004a60:	3000bf98 	.word	0x3000bf98
30004a64:	3000c534 	.word	0x3000c534
30004a68:	07fff004 	.word	0x07fff004
30004a6c:	48b3      	ldr	r0, [pc, #716]	; (30004d3c <BOOT_LoadImages+0x48c>)
30004a6e:	f007 f83f 	bl	3000baf0 <__DiagPrintf_veneer>
30004a72:	f1b8 0f00 	cmp.w	r8, #0
30004a76:	f000 80c7 	beq.w	30004c08 <BOOT_LoadImages+0x358>
30004a7a:	9b08      	ldr	r3, [sp, #32]
30004a7c:	429e      	cmp	r6, r3
30004a7e:	f0c0 837f 	bcc.w	30005180 <BOOT_LoadImages+0x8d0>
30004a82:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004a86:	bf2c      	ite	cs
30004a88:	2300      	movcs	r3, #0
30004a8a:	2301      	movcc	r3, #1
30004a8c:	2b00      	cmp	r3, #0
30004a8e:	f040 8377 	bne.w	30005180 <BOOT_LoadImages+0x8d0>
30004a92:	4698      	mov	r8, r3
30004a94:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30004a96:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004a9a:	f0c0 810b 	bcc.w	30004cb4 <BOOT_LoadImages+0x404>
30004a9e:	42b3      	cmp	r3, r6
30004aa0:	f200 8108 	bhi.w	30004cb4 <BOOT_LoadImages+0x404>
30004aa4:	7b2b      	ldrb	r3, [r5, #12]
30004aa6:	9304      	str	r3, [sp, #16]
30004aa8:	f1b8 0f00 	cmp.w	r8, #0
30004aac:	f040 8207 	bne.w	30004ebe <BOOT_LoadImages+0x60e>
30004ab0:	48a3      	ldr	r0, [pc, #652]	; (30004d40 <BOOT_LoadImages+0x490>)
30004ab2:	f007 f81d 	bl	3000baf0 <__DiagPrintf_veneer>
30004ab6:	9b08      	ldr	r3, [sp, #32]
30004ab8:	f8c4 3400 	str.w	r3, [r4, #1024]	; 0x400
30004abc:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30004abe:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004ac2:	9904      	ldr	r1, [sp, #16]
30004ac4:	489f      	ldr	r0, [pc, #636]	; (30004d44 <BOOT_LoadImages+0x494>)
30004ac6:	f007 f813 	bl	3000baf0 <__DiagPrintf_veneer>
30004aca:	4b9f      	ldr	r3, [pc, #636]	; (30004d48 <BOOT_LoadImages+0x498>)
30004acc:	4798      	blx	r3
30004ace:	1e03      	subs	r3, r0, #0
30004ad0:	bf18      	it	ne
30004ad2:	2301      	movne	r3, #1
30004ad4:	9306      	str	r3, [sp, #24]
30004ad6:	2300      	movs	r3, #0
30004ad8:	9307      	str	r3, [sp, #28]
30004ada:	4b9c      	ldr	r3, [pc, #624]	; (30004d4c <BOOT_LoadImages+0x49c>)
30004adc:	9309      	str	r3, [sp, #36]	; 0x24
30004ade:	4b9c      	ldr	r3, [pc, #624]	; (30004d50 <BOOT_LoadImages+0x4a0>)
30004ae0:	9308      	str	r3, [sp, #32]
30004ae2:	4b9c      	ldr	r3, [pc, #624]	; (30004d54 <BOOT_LoadImages+0x4a4>)
30004ae4:	930a      	str	r3, [sp, #40]	; 0x28
30004ae6:	f8d4 0400 	ldr.w	r0, [r4, #1024]	; 0x400
30004aea:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30004aee:	f8d0 200a 	ldr.w	r2, [r0, #10]
30004af2:	9902      	ldr	r1, [sp, #8]
30004af4:	f102 070c 	add.w	r7, r2, #12
30004af8:	f8dc 300a 	ldr.w	r3, [ip, #10]
30004afc:	f8d0 9000 	ldr.w	r9, [r0]
30004b00:	428f      	cmp	r7, r1
30004b02:	f103 060c 	add.w	r6, r3, #12
30004b06:	f8dc 8000 	ldr.w	r8, [ip]
30004b0a:	bf28      	it	cs
30004b0c:	460f      	movcs	r7, r1
30004b0e:	428e      	cmp	r6, r1
30004b10:	bf28      	it	cs
30004b12:	460e      	movcs	r6, r1
30004b14:	9906      	ldr	r1, [sp, #24]
30004b16:	2900      	cmp	r1, #0
30004b18:	f000 8144 	beq.w	30004da4 <BOOT_LoadImages+0x4f4>
30004b1c:	9904      	ldr	r1, [sp, #16]
30004b1e:	2900      	cmp	r1, #0
30004b20:	f040 8241 	bne.w	30004fa6 <BOOT_LoadImages+0x6f6>
30004b24:	3201      	adds	r2, #1
30004b26:	f000 84ca 	beq.w	300054be <BOOT_LoadImages+0xc0e>
30004b2a:	9b07      	ldr	r3, [sp, #28]
30004b2c:	f8d0 1006 	ldr.w	r1, [r0, #6]
30004b30:	b113      	cbz	r3, 30004b38 <BOOT_LoadImages+0x288>
30004b32:	2965      	cmp	r1, #101	; 0x65
30004b34:	f000 850b 	beq.w	3000554e <BOOT_LoadImages+0xc9e>
30004b38:	4887      	ldr	r0, [pc, #540]	; (30004d58 <BOOT_LoadImages+0x4a8>)
30004b3a:	f006 ffd9 	bl	3000baf0 <__DiagPrintf_veneer>
30004b3e:	f8dd b018 	ldr.w	fp, [sp, #24]
30004b42:	9b07      	ldr	r3, [sp, #28]
30004b44:	2601      	movs	r6, #1
30004b46:	2b00      	cmp	r3, #0
30004b48:	f040 82d2 	bne.w	300050f0 <BOOT_LoadImages+0x840>
30004b4c:	f1bb 0f01 	cmp.w	fp, #1
30004b50:	f000 80de 	beq.w	30004d10 <BOOT_LoadImages+0x460>
30004b54:	f1bb 0f02 	cmp.w	fp, #2
30004b58:	f000 8282 	beq.w	30005060 <BOOT_LoadImages+0x7b0>
30004b5c:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004b5e:	4658      	mov	r0, fp
30004b60:	461a      	mov	r2, r3
30004b62:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30004b64:	6013      	str	r3, [r2, #0]
30004b66:	9b08      	ldr	r3, [sp, #32]
30004b68:	4798      	blx	r3
30004b6a:	1e03      	subs	r3, r0, #0
30004b6c:	bf18      	it	ne
30004b6e:	2301      	movne	r3, #1
30004b70:	2800      	cmp	r0, #0
30004b72:	f000 819c 	beq.w	30004eae <BOOT_LoadImages+0x5fe>
30004b76:	2e00      	cmp	r6, #0
30004b78:	f000 8199 	beq.w	30004eae <BOOT_LoadImages+0x5fe>
30004b7c:	9b04      	ldr	r3, [sp, #16]
30004b7e:	4c77      	ldr	r4, [pc, #476]	; (30004d5c <BOOT_LoadImages+0x4ac>)
30004b80:	fab3 f383 	clz	r3, r3
30004b84:	095b      	lsrs	r3, r3, #5
30004b86:	9304      	str	r3, [sp, #16]
30004b88:	4875      	ldr	r0, [pc, #468]	; (30004d60 <BOOT_LoadImages+0x4b0>)
30004b8a:	6943      	ldr	r3, [r0, #20]
30004b8c:	03da      	lsls	r2, r3, #15
30004b8e:	d51e      	bpl.n	30004bce <BOOT_LoadImages+0x31e>
30004b90:	2300      	movs	r3, #0
30004b92:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004b96:	f3bf 8f4f 	dsb	sy
30004b9a:	f8d0 7080 	ldr.w	r7, [r0, #128]	; 0x80
30004b9e:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30004ba2:	f3c7 364e 	ubfx	r6, r7, #13, #15
30004ba6:	f3c7 07c9 	ubfx	r7, r7, #3, #10
30004baa:	0176      	lsls	r6, r6, #5
30004bac:	ea06 010c 	and.w	r1, r6, ip
30004bb0:	463b      	mov	r3, r7
30004bb2:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004bb6:	3b01      	subs	r3, #1
30004bb8:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004bbc:	d2f9      	bcs.n	30004bb2 <BOOT_LoadImages+0x302>
30004bbe:	3e20      	subs	r6, #32
30004bc0:	f116 0f20 	cmn.w	r6, #32
30004bc4:	d1f2      	bne.n	30004bac <BOOT_LoadImages+0x2fc>
30004bc6:	f3bf 8f4f 	dsb	sy
30004bca:	f3bf 8f6f 	isb	sy
30004bce:	2304      	movs	r3, #4
30004bd0:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
30004bd4:	2301      	movs	r3, #1
30004bd6:	9307      	str	r3, [sp, #28]
30004bd8:	e785      	b.n	30004ae6 <BOOT_LoadImages+0x236>
30004bda:	4862      	ldr	r0, [pc, #392]	; (30004d64 <BOOT_LoadImages+0x4b4>)
30004bdc:	f006 ff88 	bl	3000baf0 <__DiagPrintf_veneer>
30004be0:	9b05      	ldr	r3, [sp, #20]
30004be2:	f103 38ff 	add.w	r8, r3, #4294967295	; 0xffffffff
30004be6:	e6b3      	b.n	30004950 <BOOT_LoadImages+0xa0>
30004be8:	485f      	ldr	r0, [pc, #380]	; (30004d68 <BOOT_LoadImages+0x4b8>)
30004bea:	f006 ff81 	bl	3000baf0 <__DiagPrintf_veneer>
30004bee:	9b03      	ldr	r3, [sp, #12]
30004bf0:	f8d3 200d 	ldr.w	r2, [r3, #13]
30004bf4:	9209      	str	r2, [sp, #36]	; 0x24
30004bf6:	f8d3 3011 	ldr.w	r3, [r3, #17]
30004bfa:	930c      	str	r3, [sp, #48]	; 0x30
30004bfc:	f8d5 300d 	ldr.w	r3, [r5, #13]
30004c00:	9308      	str	r3, [sp, #32]
30004c02:	f8d5 3011 	ldr.w	r3, [r5, #17]
30004c06:	930b      	str	r3, [sp, #44]	; 0x2c
30004c08:	4858      	ldr	r0, [pc, #352]	; (30004d6c <BOOT_LoadImages+0x4bc>)
30004c0a:	f006 ff71 	bl	3000baf0 <__DiagPrintf_veneer>
30004c0e:	4858      	ldr	r0, [pc, #352]	; (30004d70 <BOOT_LoadImages+0x4c0>)
30004c10:	f006 ff6e 	bl	3000baf0 <__DiagPrintf_veneer>
30004c14:	f8dd 8008 	ldr.w	r8, [sp, #8]
30004c18:	9909      	ldr	r1, [sp, #36]	; 0x24
30004c1a:	ab10      	add	r3, sp, #64	; 0x40
30004c1c:	4642      	mov	r2, r8
30004c1e:	2000      	movs	r0, #0
30004c20:	f7ff f8ba 	bl	30003d98 <recovery_check>
30004c24:	4642      	mov	r2, r8
30004c26:	4606      	mov	r6, r0
30004c28:	990c      	ldr	r1, [sp, #48]	; 0x30
30004c2a:	ab11      	add	r3, sp, #68	; 0x44
30004c2c:	2001      	movs	r0, #1
30004c2e:	f7ff f8b3 	bl	30003d98 <recovery_check>
30004c32:	4642      	mov	r2, r8
30004c34:	4604      	mov	r4, r0
30004c36:	9908      	ldr	r1, [sp, #32]
30004c38:	ab12      	add	r3, sp, #72	; 0x48
30004c3a:	2002      	movs	r0, #2
30004c3c:	f7ff f8ac 	bl	30003d98 <recovery_check>
30004c40:	ab13      	add	r3, sp, #76	; 0x4c
30004c42:	4607      	mov	r7, r0
30004c44:	4642      	mov	r2, r8
30004c46:	990b      	ldr	r1, [sp, #44]	; 0x2c
30004c48:	2003      	movs	r0, #3
30004c4a:	f7ff f8a5 	bl	30003d98 <recovery_check>
30004c4e:	4334      	orrs	r4, r6
30004c50:	d037      	beq.n	30004cc2 <BOOT_LoadImages+0x412>
30004c52:	4338      	orrs	r0, r7
30004c54:	e9dd 3210 	ldrd	r3, r2, [sp, #64]	; 0x40
30004c58:	f000 8096 	beq.w	30004d88 <BOOT_LoadImages+0x4d8>
30004c5c:	4293      	cmp	r3, r2
30004c5e:	e9dd 1012 	ldrd	r1, r0, [sp, #72]	; 0x48
30004c62:	f0c0 8492 	bcc.w	3000558a <BOOT_LoadImages+0xcda>
30004c66:	428b      	cmp	r3, r1
30004c68:	d302      	bcc.n	30004c70 <BOOT_LoadImages+0x3c0>
30004c6a:	4283      	cmp	r3, r0
30004c6c:	f080 850b 	bcs.w	30005686 <BOOT_LoadImages+0xdd6>
30004c70:	4293      	cmp	r3, r2
30004c72:	f000 848a 	beq.w	3000558a <BOOT_LoadImages+0xcda>
30004c76:	4293      	cmp	r3, r2
30004c78:	bf38      	it	cc
30004c7a:	4613      	movcc	r3, r2
30004c7c:	428b      	cmp	r3, r1
30004c7e:	f240 84d7 	bls.w	30005630 <BOOT_LoadImages+0xd80>
30004c82:	428b      	cmp	r3, r1
30004c84:	bf38      	it	cc
30004c86:	460b      	movcc	r3, r1
30004c88:	4283      	cmp	r3, r0
30004c8a:	d82d      	bhi.n	30004ce8 <BOOT_LoadImages+0x438>
30004c8c:	2000      	movs	r0, #0
30004c8e:	9b03      	ldr	r3, [sp, #12]
30004c90:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30004c92:	9001      	str	r0, [sp, #4]
30004c94:	2001      	movs	r0, #1
30004c96:	9908      	ldr	r1, [sp, #32]
30004c98:	9500      	str	r5, [sp, #0]
30004c9a:	f7ff f8d7 	bl	30003e4c <rewrite_bp>
30004c9e:	fa5f fb80 	uxtb.w	fp, r0
30004ca2:	e021      	b.n	30004ce8 <BOOT_LoadImages+0x438>
30004ca4:	4833      	ldr	r0, [pc, #204]	; (30004d74 <BOOT_LoadImages+0x4c4>)
30004ca6:	f006 ff23 	bl	3000baf0 <__DiagPrintf_veneer>
30004caa:	e623      	b.n	300048f4 <BOOT_LoadImages+0x44>
30004cac:	4832      	ldr	r0, [pc, #200]	; (30004d78 <BOOT_LoadImages+0x4c8>)
30004cae:	f006 ff1f 	bl	3000baf0 <__DiagPrintf_veneer>
30004cb2:	e6de      	b.n	30004a72 <BOOT_LoadImages+0x1c2>
30004cb4:	4831      	ldr	r0, [pc, #196]	; (30004d7c <BOOT_LoadImages+0x4cc>)
30004cb6:	f006 ff1b 	bl	3000baf0 <__DiagPrintf_veneer>
30004cba:	f1b8 0f00 	cmp.w	r8, #0
30004cbe:	d0a3      	beq.n	30004c08 <BOOT_LoadImages+0x358>
30004cc0:	e6a2      	b.n	30004a08 <BOOT_LoadImages+0x158>
30004cc2:	ea57 0300 	orrs.w	r3, r7, r0
30004cc6:	f000 83da 	beq.w	3000547e <BOOT_LoadImages+0xbce>
30004cca:	e9dd 2312 	ldrd	r2, r3, [sp, #72]	; 0x48
30004cce:	429a      	cmp	r2, r3
30004cd0:	f0c0 844f 	bcc.w	30005572 <BOOT_LoadImages+0xcc2>
30004cd4:	9b03      	ldr	r3, [sp, #12]
30004cd6:	4620      	mov	r0, r4
30004cd8:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30004cda:	9908      	ldr	r1, [sp, #32]
30004cdc:	9500      	str	r5, [sp, #0]
30004cde:	9401      	str	r4, [sp, #4]
30004ce0:	f7ff f8b4 	bl	30003e4c <rewrite_bp>
30004ce4:	fa5f fb80 	uxtb.w	fp, r0
30004ce8:	4b25      	ldr	r3, [pc, #148]	; (30004d80 <BOOT_LoadImages+0x4d0>)
30004cea:	2601      	movs	r6, #1
30004cec:	f1bb 0f01 	cmp.w	fp, #1
30004cf0:	681d      	ldr	r5, [r3, #0]
30004cf2:	4b24      	ldr	r3, [pc, #144]	; (30004d84 <BOOT_LoadImages+0x4d4>)
30004cf4:	9605      	str	r6, [sp, #20]
30004cf6:	681b      	ldr	r3, [r3, #0]
30004cf8:	9303      	str	r3, [sp, #12]
30004cfa:	f04f 0300 	mov.w	r3, #0
30004cfe:	9307      	str	r3, [sp, #28]
30004d00:	4b12      	ldr	r3, [pc, #72]	; (30004d4c <BOOT_LoadImages+0x49c>)
30004d02:	9309      	str	r3, [sp, #36]	; 0x24
30004d04:	4b12      	ldr	r3, [pc, #72]	; (30004d50 <BOOT_LoadImages+0x4a0>)
30004d06:	9308      	str	r3, [sp, #32]
30004d08:	4b12      	ldr	r3, [pc, #72]	; (30004d54 <BOOT_LoadImages+0x4a4>)
30004d0a:	930a      	str	r3, [sp, #40]	; 0x28
30004d0c:	f47f af22 	bne.w	30004b54 <BOOT_LoadImages+0x2a4>
30004d10:	4c12      	ldr	r4, [pc, #72]	; (30004d5c <BOOT_LoadImages+0x4ac>)
30004d12:	4658      	mov	r0, fp
30004d14:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004d16:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004d1a:	3310      	adds	r3, #16
30004d1c:	930d      	str	r3, [sp, #52]	; 0x34
30004d1e:	6013      	str	r3, [r2, #0]
30004d20:	9b08      	ldr	r3, [sp, #32]
30004d22:	4798      	blx	r3
30004d24:	1e03      	subs	r3, r0, #0
30004d26:	bf18      	it	ne
30004d28:	2301      	movne	r3, #1
30004d2a:	2800      	cmp	r0, #0
30004d2c:	f000 80bd 	beq.w	30004eaa <BOOT_LoadImages+0x5fa>
30004d30:	2e00      	cmp	r6, #0
30004d32:	f000 80ba 	beq.w	30004eaa <BOOT_LoadImages+0x5fa>
30004d36:	f8cd b010 	str.w	fp, [sp, #16]
30004d3a:	e725      	b.n	30004b88 <BOOT_LoadImages+0x2d8>
30004d3c:	3000c108 	.word	0x3000c108
30004d40:	3000c164 	.word	0x3000c164
30004d44:	3000c198 	.word	0x3000c198
30004d48:	0000c149 	.word	0x0000c149
30004d4c:	3000f438 	.word	0x3000f438
30004d50:	3000695d 	.word	0x3000695d
30004d54:	3000df9f 	.word	0x3000df9f
30004d58:	3000bf04 	.word	0x3000bf04
30004d5c:	3000e164 	.word	0x3000e164
30004d60:	e000ed00 	.word	0xe000ed00
30004d64:	3000bf80 	.word	0x3000bf80
30004d68:	3000bfe4 	.word	0x3000bfe4
30004d6c:	3000c174 	.word	0x3000c174
30004d70:	3000c188 	.word	0x3000c188
30004d74:	3000bf48 	.word	0x3000bf48
30004d78:	3000c124 	.word	0x3000c124
30004d7c:	3000c518 	.word	0x3000c518
30004d80:	3000f4b0 	.word	0x3000f4b0
30004d84:	3000f4ac 	.word	0x3000f4ac
30004d88:	4293      	cmp	r3, r2
30004d8a:	9909      	ldr	r1, [sp, #36]	; 0x24
30004d8c:	9b03      	ldr	r3, [sp, #12]
30004d8e:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30004d90:	9500      	str	r5, [sp, #0]
30004d92:	f0c0 8313 	bcc.w	300053bc <BOOT_LoadImages+0xb0c>
30004d96:	2401      	movs	r4, #1
30004d98:	9401      	str	r4, [sp, #4]
30004d9a:	f7ff f857 	bl	30003e4c <rewrite_bp>
30004d9e:	fa5f fb80 	uxtb.w	fp, r0
30004da2:	e7a1      	b.n	30004ce8 <BOOT_LoadImages+0x438>
30004da4:	9b04      	ldr	r3, [sp, #16]
30004da6:	2b00      	cmp	r3, #0
30004da8:	f000 80aa 	beq.w	30004f00 <BOOT_LoadImages+0x650>
30004dac:	970b      	str	r7, [sp, #44]	; 0x2c
30004dae:	4667      	mov	r7, ip
30004db0:	f8dd a018 	ldr.w	sl, [sp, #24]
30004db4:	1d3b      	adds	r3, r7, #4
30004db6:	b346      	cbz	r6, 30004e0a <BOOT_LoadImages+0x55a>
30004db8:	1d30      	adds	r0, r6, #4
30004dba:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004dbe:	4438      	add	r0, r7
30004dc0:	f813 1b01 	ldrb.w	r1, [r3], #1
30004dc4:	4051      	eors	r1, r2
30004dc6:	4283      	cmp	r3, r0
30004dc8:	b2c9      	uxtb	r1, r1
30004dca:	f854 1021 	ldr.w	r1, [r4, r1, lsl #2]
30004dce:	ea81 2212 	eor.w	r2, r1, r2, lsr #8
30004dd2:	d1f5      	bne.n	30004dc0 <BOOT_LoadImages+0x510>
30004dd4:	ea6f 0b02 	mvn.w	fp, r2
30004dd8:	f10a 0a01 	add.w	sl, sl, #1
30004ddc:	45d8      	cmp	r8, fp
30004dde:	fa5f fa8a 	uxtb.w	sl, sl
30004de2:	d019      	beq.n	30004e18 <BOOT_LoadImages+0x568>
30004de4:	f8d7 300a 	ldr.w	r3, [r7, #10]
30004de8:	3301      	adds	r3, #1
30004dea:	d037      	beq.n	30004e5c <BOOT_LoadImages+0x5ac>
30004dec:	4653      	mov	r3, sl
30004dee:	465a      	mov	r2, fp
30004df0:	4641      	mov	r1, r8
30004df2:	48a6      	ldr	r0, [pc, #664]	; (3000508c <BOOT_LoadImages+0x7dc>)
30004df4:	f006 fe7c 	bl	3000baf0 <__DiagPrintf_veneer>
30004df8:	f1ba 0f02 	cmp.w	sl, #2
30004dfc:	f200 824a 	bhi.w	30005294 <BOOT_LoadImages+0x9e4>
30004e00:	f8d4 7404 	ldr.w	r7, [r4, #1028]	; 0x404
30004e04:	1d3b      	adds	r3, r7, #4
30004e06:	2e00      	cmp	r6, #0
30004e08:	d1d6      	bne.n	30004db8 <BOOT_LoadImages+0x508>
30004e0a:	46b3      	mov	fp, r6
30004e0c:	f10a 0a01 	add.w	sl, sl, #1
30004e10:	45d8      	cmp	r8, fp
30004e12:	fa5f fa8a 	uxtb.w	sl, sl
30004e16:	d1e5      	bne.n	30004de4 <BOOT_LoadImages+0x534>
30004e18:	4642      	mov	r2, r8
30004e1a:	4641      	mov	r1, r8
30004e1c:	489c      	ldr	r0, [pc, #624]	; (30005090 <BOOT_LoadImages+0x7e0>)
30004e1e:	f006 fe67 	bl	3000baf0 <__DiagPrintf_veneer>
30004e22:	9b07      	ldr	r3, [sp, #28]
30004e24:	2b00      	cmp	r3, #0
30004e26:	f000 8358 	beq.w	300054da <BOOT_LoadImages+0xc2a>
30004e2a:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004e2e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004e32:	2965      	cmp	r1, #101	; 0x65
30004e34:	f040 80c1 	bne.w	30004fba <BOOT_LoadImages+0x70a>
30004e38:	4896      	ldr	r0, [pc, #600]	; (30005094 <BOOT_LoadImages+0x7e4>)
30004e3a:	f006 fe59 	bl	3000baf0 <__DiagPrintf_veneer>
30004e3e:	4b96      	ldr	r3, [pc, #600]	; (30005098 <BOOT_LoadImages+0x7e8>)
30004e40:	a814      	add	r0, sp, #80	; 0x50
30004e42:	4798      	blx	r3
30004e44:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30004e48:	4894      	ldr	r0, [pc, #592]	; (3000509c <BOOT_LoadImages+0x7ec>)
30004e4a:	a914      	add	r1, sp, #80	; 0x50
30004e4c:	4b94      	ldr	r3, [pc, #592]	; (300050a0 <BOOT_LoadImages+0x7f0>)
30004e4e:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
30004e52:	4798      	blx	r3
30004e54:	4891      	ldr	r0, [pc, #580]	; (3000509c <BOOT_LoadImages+0x7ec>)
30004e56:	4b93      	ldr	r3, [pc, #588]	; (300050a4 <BOOT_LoadImages+0x7f4>)
30004e58:	4798      	blx	r3
30004e5a:	e7fe      	b.n	30004e5a <BOOT_LoadImages+0x5aa>
30004e5c:	4651      	mov	r1, sl
30004e5e:	4892      	ldr	r0, [pc, #584]	; (300050a8 <BOOT_LoadImages+0x7f8>)
30004e60:	f006 fe46 	bl	3000baf0 <__DiagPrintf_veneer>
30004e64:	f1ba 0f02 	cmp.w	sl, #2
30004e68:	d9ca      	bls.n	30004e00 <BOOT_LoadImages+0x550>
30004e6a:	9f0b      	ldr	r7, [sp, #44]	; 0x2c
30004e6c:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30004e70:	2303      	movs	r3, #3
30004e72:	2280      	movs	r2, #128	; 0x80
30004e74:	21ff      	movs	r1, #255	; 0xff
30004e76:	4650      	mov	r0, sl
30004e78:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
30004e7c:	f006 fea0 	bl	3000bbc0 <____wrap_memset_veneer>
30004e80:	2280      	movs	r2, #128	; 0x80
30004e82:	4629      	mov	r1, r5
30004e84:	4650      	mov	r0, sl
30004e86:	f006 fe23 	bl	3000bad0 <____wrap_memcpy_veneer>
30004e8a:	9b07      	ldr	r3, [sp, #28]
30004e8c:	2b00      	cmp	r3, #0
30004e8e:	f000 828d 	beq.w	300053ac <BOOT_LoadImages+0xafc>
30004e92:	9b04      	ldr	r3, [sp, #16]
30004e94:	f1a3 0301 	sub.w	r3, r3, #1
30004e98:	fab3 f383 	clz	r3, r3
30004e9c:	095b      	lsrs	r3, r3, #5
30004e9e:	f88d 3064 	strb.w	r3, [sp, #100]	; 0x64
30004ea2:	4882      	ldr	r0, [pc, #520]	; (300050ac <BOOT_LoadImages+0x7fc>)
30004ea4:	f006 fe24 	bl	3000baf0 <__DiagPrintf_veneer>
30004ea8:	e7fe      	b.n	30004ea8 <BOOT_LoadImages+0x5f8>
30004eaa:	2200      	movs	r2, #0
30004eac:	9204      	str	r2, [sp, #16]
30004eae:	9c07      	ldr	r4, [sp, #28]
30004eb0:	b1ec      	cbz	r4, 30004eee <BOOT_LoadImages+0x63e>
30004eb2:	b1e3      	cbz	r3, 30004eee <BOOT_LoadImages+0x63e>
30004eb4:	4c7e      	ldr	r4, [pc, #504]	; (300050b0 <BOOT_LoadImages+0x800>)
30004eb6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004eba:	47a0      	blx	r4
30004ebc:	e7fb      	b.n	30004eb6 <BOOT_LoadImages+0x606>
30004ebe:	487d      	ldr	r0, [pc, #500]	; (300050b4 <BOOT_LoadImages+0x804>)
30004ec0:	f006 fe16 	bl	3000baf0 <__DiagPrintf_veneer>
30004ec4:	9b05      	ldr	r3, [sp, #20]
30004ec6:	4599      	cmp	r9, r3
30004ec8:	f4ff adf2 	bcc.w	30004ab0 <BOOT_LoadImages+0x200>
30004ecc:	487a      	ldr	r0, [pc, #488]	; (300050b8 <BOOT_LoadImages+0x808>)
30004ece:	f006 fe0f 	bl	3000baf0 <__DiagPrintf_veneer>
30004ed2:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004ed4:	9a07      	ldr	r2, [sp, #28]
30004ed6:	f8c4 3400 	str.w	r3, [r4, #1024]	; 0x400
30004eda:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004edc:	f8cd 9014 	str.w	r9, [sp, #20]
30004ee0:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004ee4:	462b      	mov	r3, r5
30004ee6:	9d03      	ldr	r5, [sp, #12]
30004ee8:	e9cd 3203 	strd	r3, r2, [sp, #12]
30004eec:	e5e9      	b.n	30004ac2 <BOOT_LoadImages+0x212>
30004eee:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004ef0:	781b      	ldrb	r3, [r3, #0]
30004ef2:	2b00      	cmp	r3, #0
30004ef4:	f040 815c 	bne.w	300051b0 <BOOT_LoadImages+0x900>
30004ef8:	2001      	movs	r0, #1
30004efa:	b037      	add	sp, #220	; 0xdc
30004efc:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f00:	f8dd a010 	ldr.w	sl, [sp, #16]
30004f04:	1d03      	adds	r3, r0, #4
30004f06:	b34f      	cbz	r7, 30004f5c <BOOT_LoadImages+0x6ac>
30004f08:	f107 0c04 	add.w	ip, r7, #4
30004f0c:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004f10:	4484      	add	ip, r0
30004f12:	f813 1b01 	ldrb.w	r1, [r3], #1
30004f16:	4051      	eors	r1, r2
30004f18:	4563      	cmp	r3, ip
30004f1a:	b2c9      	uxtb	r1, r1
30004f1c:	f854 1021 	ldr.w	r1, [r4, r1, lsl #2]
30004f20:	ea81 2212 	eor.w	r2, r1, r2, lsr #8
30004f24:	d1f5      	bne.n	30004f12 <BOOT_LoadImages+0x662>
30004f26:	ea6f 0b02 	mvn.w	fp, r2
30004f2a:	f10a 0a01 	add.w	sl, sl, #1
30004f2e:	45d9      	cmp	r9, fp
30004f30:	fa5f fa8a 	uxtb.w	sl, sl
30004f34:	d019      	beq.n	30004f6a <BOOT_LoadImages+0x6ba>
30004f36:	f8d0 300a 	ldr.w	r3, [r0, #10]
30004f3a:	3301      	adds	r3, #1
30004f3c:	d02b      	beq.n	30004f96 <BOOT_LoadImages+0x6e6>
30004f3e:	4653      	mov	r3, sl
30004f40:	465a      	mov	r2, fp
30004f42:	4649      	mov	r1, r9
30004f44:	485d      	ldr	r0, [pc, #372]	; (300050bc <BOOT_LoadImages+0x80c>)
30004f46:	f006 fdd3 	bl	3000baf0 <__DiagPrintf_veneer>
30004f4a:	f1ba 0f02 	cmp.w	sl, #2
30004f4e:	f200 816a 	bhi.w	30005226 <BOOT_LoadImages+0x976>
30004f52:	f8d4 0400 	ldr.w	r0, [r4, #1024]	; 0x400
30004f56:	1d03      	adds	r3, r0, #4
30004f58:	2f00      	cmp	r7, #0
30004f5a:	d1d5      	bne.n	30004f08 <BOOT_LoadImages+0x658>
30004f5c:	46bb      	mov	fp, r7
30004f5e:	f10a 0a01 	add.w	sl, sl, #1
30004f62:	45d9      	cmp	r9, fp
30004f64:	fa5f fa8a 	uxtb.w	sl, sl
30004f68:	d1e5      	bne.n	30004f36 <BOOT_LoadImages+0x686>
30004f6a:	464a      	mov	r2, r9
30004f6c:	4649      	mov	r1, r9
30004f6e:	4854      	ldr	r0, [pc, #336]	; (300050c0 <BOOT_LoadImages+0x810>)
30004f70:	f006 fdbe 	bl	3000baf0 <__DiagPrintf_veneer>
30004f74:	9b07      	ldr	r3, [sp, #28]
30004f76:	2b00      	cmp	r3, #0
30004f78:	f000 82b4 	beq.w	300054e4 <BOOT_LoadImages+0xc34>
30004f7c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004f80:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004f84:	2965      	cmp	r1, #101	; 0x65
30004f86:	f000 8341 	beq.w	3000560c <BOOT_LoadImages+0xd5c>
30004f8a:	484e      	ldr	r0, [pc, #312]	; (300050c4 <BOOT_LoadImages+0x814>)
30004f8c:	f04f 0b01 	mov.w	fp, #1
30004f90:	f006 fdae 	bl	3000baf0 <__DiagPrintf_veneer>
30004f94:	e5d5      	b.n	30004b42 <BOOT_LoadImages+0x292>
30004f96:	4651      	mov	r1, sl
30004f98:	484b      	ldr	r0, [pc, #300]	; (300050c8 <BOOT_LoadImages+0x818>)
30004f9a:	f006 fda9 	bl	3000baf0 <__DiagPrintf_veneer>
30004f9e:	f1ba 0f02 	cmp.w	sl, #2
30004fa2:	d9d6      	bls.n	30004f52 <BOOT_LoadImages+0x6a2>
30004fa4:	e762      	b.n	30004e6c <BOOT_LoadImages+0x5bc>
30004fa6:	3301      	adds	r3, #1
30004fa8:	f000 8290 	beq.w	300054cc <BOOT_LoadImages+0xc1c>
30004fac:	9b07      	ldr	r3, [sp, #28]
30004fae:	f8dc 1006 	ldr.w	r1, [ip, #6]
30004fb2:	b113      	cbz	r3, 30004fba <BOOT_LoadImages+0x70a>
30004fb4:	2965      	cmp	r1, #101	; 0x65
30004fb6:	f000 82b8 	beq.w	3000552a <BOOT_LoadImages+0xc7a>
30004fba:	4844      	ldr	r0, [pc, #272]	; (300050cc <BOOT_LoadImages+0x81c>)
30004fbc:	f04f 0b02 	mov.w	fp, #2
30004fc0:	f006 fd96 	bl	3000baf0 <__DiagPrintf_veneer>
30004fc4:	e5bd      	b.n	30004b42 <BOOT_LoadImages+0x292>
30004fc6:	4649      	mov	r1, r9
30004fc8:	4841      	ldr	r0, [pc, #260]	; (300050d0 <BOOT_LoadImages+0x820>)
30004fca:	f006 fd91 	bl	3000baf0 <__DiagPrintf_veneer>
30004fce:	f118 0f03 	cmn.w	r8, #3
30004fd2:	f240 830a 	bls.w	300055ea <BOOT_LoadImages+0xd3a>
30004fd6:	483f      	ldr	r0, [pc, #252]	; (300050d4 <BOOT_LoadImages+0x824>)
30004fd8:	f006 fd8a 	bl	3000baf0 <__DiagPrintf_veneer>
30004fdc:	9b03      	ldr	r3, [sp, #12]
30004fde:	970a      	str	r7, [sp, #40]	; 0x28
30004fe0:	f8d3 200d 	ldr.w	r2, [r3, #13]
30004fe4:	9209      	str	r2, [sp, #36]	; 0x24
30004fe6:	f8d3 3011 	ldr.w	r3, [r3, #17]
30004fea:	930c      	str	r3, [sp, #48]	; 0x30
30004fec:	f8d5 300d 	ldr.w	r3, [r5, #13]
30004ff0:	9308      	str	r3, [sp, #32]
30004ff2:	f8d5 3011 	ldr.w	r3, [r5, #17]
30004ff6:	930b      	str	r3, [sp, #44]	; 0x2c
30004ff8:	f8df 80f0 	ldr.w	r8, [pc, #240]	; 300050ec <BOOT_LoadImages+0x83c>
30004ffc:	2601      	movs	r6, #1
30004ffe:	9b02      	ldr	r3, [sp, #8]
30005000:	4c35      	ldr	r4, [pc, #212]	; (300050d8 <BOOT_LoadImages+0x828>)
30005002:	4498      	add	r8, r3
30005004:	4f35      	ldr	r7, [pc, #212]	; (300050dc <BOOT_LoadImages+0x82c>)
30005006:	f8dd a00c 	ldr.w	sl, [sp, #12]
3000500a:	f8da 1000 	ldr.w	r1, [sl]
3000500e:	4640      	mov	r0, r8
30005010:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30005014:	f810 3b01 	ldrb.w	r3, [r0], #1
30005018:	4053      	eors	r3, r2
3000501a:	4285      	cmp	r5, r0
3000501c:	b2db      	uxtb	r3, r3
3000501e:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005022:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005026:	d1f5      	bne.n	30005014 <BOOT_LoadImages+0x764>
30005028:	43d2      	mvns	r2, r2
3000502a:	4291      	cmp	r1, r2
3000502c:	f000 80b3 	beq.w	30005196 <BOOT_LoadImages+0x8e6>
30005030:	4633      	mov	r3, r6
30005032:	3601      	adds	r6, #1
30005034:	4638      	mov	r0, r7
30005036:	f006 fd5b 	bl	3000baf0 <__DiagPrintf_veneer>
3000503a:	2e04      	cmp	r6, #4
3000503c:	d1e5      	bne.n	3000500a <BOOT_LoadImages+0x75a>
3000503e:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30005040:	2b00      	cmp	r3, #0
30005042:	f43f ade1 	beq.w	30004c08 <BOOT_LoadImages+0x358>
30005046:	2300      	movs	r3, #0
30005048:	930a      	str	r3, [sp, #40]	; 0x28
3000504a:	e49f      	b.n	3000498c <BOOT_LoadImages+0xdc>
3000504c:	4824      	ldr	r0, [pc, #144]	; (300050e0 <BOOT_LoadImages+0x830>)
3000504e:	f006 fd4f 	bl	3000baf0 <__DiagPrintf_veneer>
30005052:	e44f      	b.n	300048f4 <BOOT_LoadImages+0x44>
30005054:	4823      	ldr	r0, [pc, #140]	; (300050e4 <BOOT_LoadImages+0x834>)
30005056:	e5c8      	b.n	30004bea <BOOT_LoadImages+0x33a>
30005058:	4823      	ldr	r0, [pc, #140]	; (300050e8 <BOOT_LoadImages+0x838>)
3000505a:	f006 fd49 	bl	3000baf0 <__DiagPrintf_veneer>
3000505e:	e449      	b.n	300048f4 <BOOT_LoadImages+0x44>
30005060:	4c1d      	ldr	r4, [pc, #116]	; (300050d8 <BOOT_LoadImages+0x828>)
30005062:	4658      	mov	r0, fp
30005064:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005066:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000506a:	3310      	adds	r3, #16
3000506c:	930d      	str	r3, [sp, #52]	; 0x34
3000506e:	6013      	str	r3, [r2, #0]
30005070:	9b08      	ldr	r3, [sp, #32]
30005072:	4798      	blx	r3
30005074:	1e03      	subs	r3, r0, #0
30005076:	bf18      	it	ne
30005078:	2301      	movne	r3, #1
3000507a:	b118      	cbz	r0, 30005084 <BOOT_LoadImages+0x7d4>
3000507c:	b116      	cbz	r6, 30005084 <BOOT_LoadImages+0x7d4>
3000507e:	2300      	movs	r3, #0
30005080:	9304      	str	r3, [sp, #16]
30005082:	e581      	b.n	30004b88 <BOOT_LoadImages+0x2d8>
30005084:	2201      	movs	r2, #1
30005086:	9204      	str	r2, [sp, #16]
30005088:	e711      	b.n	30004eae <BOOT_LoadImages+0x5fe>
3000508a:	bf00      	nop
3000508c:	3000c3bc 	.word	0x3000c3bc
30005090:	3000c364 	.word	0x3000c364
30005094:	3000c214 	.word	0x3000c214
30005098:	0000d3f9 	.word	0x0000d3f9
3000509c:	41000440 	.word	0x41000440
300050a0:	0000d409 	.word	0x0000d409
300050a4:	0000d489 	.word	0x0000d489
300050a8:	3000c3a0 	.word	0x3000c3a0
300050ac:	3000c494 	.word	0x3000c494
300050b0:	00009be5 	.word	0x00009be5
300050b4:	3000c140 	.word	0x3000c140
300050b8:	3000c154 	.word	0x3000c154
300050bc:	3000c2b4 	.word	0x3000c2b4
300050c0:	3000c25c 	.word	0x3000c25c
300050c4:	3000bf04 	.word	0x3000bf04
300050c8:	3000c298 	.word	0x3000c298
300050cc:	3000bf1c 	.word	0x3000bf1c
300050d0:	3000bfc8 	.word	0x3000bfc8
300050d4:	3000c534 	.word	0x3000c534
300050d8:	3000e164 	.word	0x3000e164
300050dc:	3000c034 	.word	0x3000c034
300050e0:	3000bf58 	.word	0x3000bf58
300050e4:	3000c54c 	.word	0x3000c54c
300050e8:	3000bf6c 	.word	0x3000bf6c
300050ec:	07ffe004 	.word	0x07ffe004
300050f0:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
300050f4:	2280      	movs	r2, #128	; 0x80
300050f6:	21ff      	movs	r1, #255	; 0xff
300050f8:	4650      	mov	r0, sl
300050fa:	f006 fd61 	bl	3000bbc0 <____wrap_memset_veneer>
300050fe:	4629      	mov	r1, r5
30005100:	2280      	movs	r2, #128	; 0x80
30005102:	4650      	mov	r0, sl
30005104:	f006 fce4 	bl	3000bad0 <____wrap_memcpy_veneer>
30005108:	9b04      	ldr	r3, [sp, #16]
3000510a:	f1a3 0101 	sub.w	r1, r3, #1
3000510e:	fab1 f181 	clz	r1, r1
30005112:	0949      	lsrs	r1, r1, #5
30005114:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30005118:	9f05      	ldr	r7, [sp, #20]
3000511a:	9b03      	ldr	r3, [sp, #12]
3000511c:	3701      	adds	r7, #1
3000511e:	48ab      	ldr	r0, [pc, #684]	; (300053cc <BOOT_LoadImages+0xb1c>)
30005120:	1d1e      	adds	r6, r3, #4
30005122:	9705      	str	r7, [sp, #20]
30005124:	f006 fce4 	bl	3000baf0 <__DiagPrintf_veneer>
30005128:	9717      	str	r7, [sp, #92]	; 0x5c
3000512a:	2002      	movs	r0, #2
3000512c:	9f03      	ldr	r7, [sp, #12]
3000512e:	4639      	mov	r1, r7
30005130:	f7fe fd4e 	bl	30003bd0 <FLASH_Erase_With_Lock>
30005134:	4630      	mov	r0, r6
30005136:	aa17      	add	r2, sp, #92	; 0x5c
30005138:	217c      	movs	r1, #124	; 0x7c
3000513a:	f7fe fd71 	bl	30003c20 <FLASH_TxData_With_Lock>
3000513e:	f10d 023b 	add.w	r2, sp, #59	; 0x3b
30005142:	2101      	movs	r1, #1
30005144:	f607 70ff 	addw	r0, r7, #4095	; 0xfff
30005148:	f7fe fd6a 	bl	30003c20 <FLASH_TxData_With_Lock>
3000514c:	4632      	mov	r2, r6
3000514e:	f507 5080 	add.w	r0, r7, #4096	; 0x1000
30005152:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30005156:	f812 3b01 	ldrb.w	r3, [r2], #1
3000515a:	404b      	eors	r3, r1
3000515c:	4282      	cmp	r2, r0
3000515e:	b2db      	uxtb	r3, r3
30005160:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005164:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30005168:	d1f5      	bne.n	30005156 <BOOT_LoadImages+0x8a6>
3000516a:	43c9      	mvns	r1, r1
3000516c:	4652      	mov	r2, sl
3000516e:	9803      	ldr	r0, [sp, #12]
30005170:	9116      	str	r1, [sp, #88]	; 0x58
30005172:	2104      	movs	r1, #4
30005174:	f7fe fd54 	bl	30003c20 <FLASH_TxData_With_Lock>
30005178:	9b07      	ldr	r3, [sp, #28]
3000517a:	f083 0601 	eor.w	r6, r3, #1
3000517e:	e4e5      	b.n	30004b4c <BOOT_LoadImages+0x29c>
30005180:	4893      	ldr	r0, [pc, #588]	; (300053d0 <BOOT_LoadImages+0xb20>)
30005182:	f006 fcb5 	bl	3000baf0 <__DiagPrintf_veneer>
30005186:	e53f      	b.n	30004c08 <BOOT_LoadImages+0x358>
30005188:	460a      	mov	r2, r1
3000518a:	4892      	ldr	r0, [pc, #584]	; (300053d4 <BOOT_LoadImages+0xb24>)
3000518c:	f04f 0801 	mov.w	r8, #1
30005190:	f006 fcae 	bl	3000baf0 <__DiagPrintf_veneer>
30005194:	e420      	b.n	300049d8 <BOOT_LoadImages+0x128>
30005196:	460a      	mov	r2, r1
30005198:	488f      	ldr	r0, [pc, #572]	; (300053d8 <BOOT_LoadImages+0xb28>)
3000519a:	f006 fca9 	bl	3000baf0 <__DiagPrintf_veneer>
3000519e:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300051a0:	2b00      	cmp	r3, #0
300051a2:	f47f abf3 	bne.w	3000498c <BOOT_LoadImages+0xdc>
300051a6:	4698      	mov	r8, r3
300051a8:	9b02      	ldr	r3, [sp, #8]
300051aa:	f103 6600 	add.w	r6, r3, #134217728	; 0x8000000
300051ae:	e416      	b.n	300049de <BOOT_LoadImages+0x12e>
300051b0:	4b8a      	ldr	r3, [pc, #552]	; (300053dc <BOOT_LoadImages+0xb2c>)
300051b2:	4658      	mov	r0, fp
300051b4:	4798      	blx	r3
300051b6:	1e03      	subs	r3, r0, #0
300051b8:	bf18      	it	ne
300051ba:	2301      	movne	r3, #1
300051bc:	2800      	cmp	r0, #0
300051be:	f000 8157 	beq.w	30005470 <BOOT_LoadImages+0xbc0>
300051c2:	2e00      	cmp	r6, #0
300051c4:	f000 8154 	beq.w	30005470 <BOOT_LoadImages+0xbc0>
300051c8:	9b04      	ldr	r3, [sp, #16]
300051ca:	4985      	ldr	r1, [pc, #532]	; (300053e0 <BOOT_LoadImages+0xb30>)
300051cc:	fab3 f383 	clz	r3, r3
300051d0:	095b      	lsrs	r3, r3, #5
300051d2:	9304      	str	r3, [sp, #16]
300051d4:	694b      	ldr	r3, [r1, #20]
300051d6:	03db      	lsls	r3, r3, #15
300051d8:	d51e      	bpl.n	30005218 <BOOT_LoadImages+0x968>
300051da:	2300      	movs	r3, #0
300051dc:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300051e0:	f3bf 8f4f 	dsb	sy
300051e4:	f8d1 6080 	ldr.w	r6, [r1, #128]	; 0x80
300051e8:	f643 77e0 	movw	r7, #16352	; 0x3fe0
300051ec:	f3c6 344e 	ubfx	r4, r6, #13, #15
300051f0:	f3c6 06c9 	ubfx	r6, r6, #3, #10
300051f4:	0164      	lsls	r4, r4, #5
300051f6:	ea04 0007 	and.w	r0, r4, r7
300051fa:	4633      	mov	r3, r6
300051fc:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30005200:	3b01      	subs	r3, #1
30005202:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30005206:	d2f9      	bcs.n	300051fc <BOOT_LoadImages+0x94c>
30005208:	3c20      	subs	r4, #32
3000520a:	f114 0f20 	cmn.w	r4, #32
3000520e:	d1f2      	bne.n	300051f6 <BOOT_LoadImages+0x946>
30005210:	f3bf 8f4f 	dsb	sy
30005214:	f3bf 8f6f 	isb	sy
30005218:	2304      	movs	r3, #4
3000521a:	4c72      	ldr	r4, [pc, #456]	; (300053e4 <BOOT_LoadImages+0xb34>)
3000521c:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
30005220:	2301      	movs	r3, #1
30005222:	9307      	str	r3, [sp, #28]
30005224:	e45f      	b.n	30004ae6 <BOOT_LoadImages+0x236>
30005226:	4649      	mov	r1, r9
30005228:	465a      	mov	r2, fp
3000522a:	486f      	ldr	r0, [pc, #444]	; (300053e8 <BOOT_LoadImages+0xb38>)
3000522c:	f006 fc60 	bl	3000baf0 <__DiagPrintf_veneer>
30005230:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005234:	2002      	movs	r0, #2
30005236:	f7fe fccb 	bl	30003bd0 <FLASH_Erase_With_Lock>
3000523a:	2302      	movs	r3, #2
3000523c:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
30005240:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30005244:	2280      	movs	r2, #128	; 0x80
30005246:	21ff      	movs	r1, #255	; 0xff
30005248:	4650      	mov	r0, sl
3000524a:	f006 fcb9 	bl	3000bbc0 <____wrap_memset_veneer>
3000524e:	2280      	movs	r2, #128	; 0x80
30005250:	4629      	mov	r1, r5
30005252:	4650      	mov	r0, sl
30005254:	f006 fc3c 	bl	3000bad0 <____wrap_memcpy_veneer>
30005258:	9b07      	ldr	r3, [sp, #28]
3000525a:	2b00      	cmp	r3, #0
3000525c:	f47f ae19 	bne.w	30004e92 <BOOT_LoadImages+0x5e2>
30005260:	2301      	movs	r3, #1
30005262:	f88d 3064 	strb.w	r3, [sp, #100]	; 0x64
30005266:	9b06      	ldr	r3, [sp, #24]
30005268:	2b00      	cmp	r3, #0
3000526a:	d03b      	beq.n	300052e4 <BOOT_LoadImages+0xa34>
3000526c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005270:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005274:	3201      	adds	r2, #1
30005276:	f000 81c5 	beq.w	30005604 <BOOT_LoadImages+0xd54>
3000527a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000527e:	2965      	cmp	r1, #101	; 0x65
30005280:	f000 8101 	beq.w	30005486 <BOOT_LoadImages+0xbd6>
30005284:	f04f 0b02 	mov.w	fp, #2
30005288:	4858      	ldr	r0, [pc, #352]	; (300053ec <BOOT_LoadImages+0xb3c>)
3000528a:	f006 fc31 	bl	3000baf0 <__DiagPrintf_veneer>
3000528e:	f89d 1064 	ldrb.w	r1, [sp, #100]	; 0x64
30005292:	e741      	b.n	30005118 <BOOT_LoadImages+0x868>
30005294:	465a      	mov	r2, fp
30005296:	4641      	mov	r1, r8
30005298:	4855      	ldr	r0, [pc, #340]	; (300053f0 <BOOT_LoadImages+0xb40>)
3000529a:	9f0b      	ldr	r7, [sp, #44]	; 0x2c
3000529c:	f006 fc28 	bl	3000baf0 <__DiagPrintf_veneer>
300052a0:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
300052a4:	2002      	movs	r0, #2
300052a6:	f7fe fc93 	bl	30003bd0 <FLASH_Erase_With_Lock>
300052aa:	2302      	movs	r3, #2
300052ac:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
300052b0:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
300052b4:	2280      	movs	r2, #128	; 0x80
300052b6:	21ff      	movs	r1, #255	; 0xff
300052b8:	4650      	mov	r0, sl
300052ba:	f006 fc81 	bl	3000bbc0 <____wrap_memset_veneer>
300052be:	2280      	movs	r2, #128	; 0x80
300052c0:	4629      	mov	r1, r5
300052c2:	4650      	mov	r0, sl
300052c4:	f006 fc04 	bl	3000bad0 <____wrap_memcpy_veneer>
300052c8:	9b07      	ldr	r3, [sp, #28]
300052ca:	2b00      	cmp	r3, #0
300052cc:	f47f ade1 	bne.w	30004e92 <BOOT_LoadImages+0x5e2>
300052d0:	9b07      	ldr	r3, [sp, #28]
300052d2:	f88d 3064 	strb.w	r3, [sp, #100]	; 0x64
300052d6:	9b06      	ldr	r3, [sp, #24]
300052d8:	2b00      	cmp	r3, #0
300052da:	d144      	bne.n	30005366 <BOOT_LoadImages+0xab6>
300052dc:	9b04      	ldr	r3, [sp, #16]
300052de:	2b01      	cmp	r3, #1
300052e0:	f000 808e 	beq.w	30005400 <BOOT_LoadImages+0xb50>
300052e4:	f04f 0b00 	mov.w	fp, #0
300052e8:	4f42      	ldr	r7, [pc, #264]	; (300053f4 <BOOT_LoadImages+0xb44>)
300052ea:	f10b 0b01 	add.w	fp, fp, #1
300052ee:	f8d4 0404 	ldr.w	r0, [r4, #1028]	; 0x404
300052f2:	fa5f fb8b 	uxtb.w	fp, fp
300052f6:	1d01      	adds	r1, r0, #4
300052f8:	b39e      	cbz	r6, 30005362 <BOOT_LoadImages+0xab2>
300052fa:	f106 0c04 	add.w	ip, r6, #4
300052fe:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30005302:	4484      	add	ip, r0
30005304:	f811 2b01 	ldrb.w	r2, [r1], #1
30005308:	405a      	eors	r2, r3
3000530a:	4561      	cmp	r1, ip
3000530c:	b2d2      	uxtb	r2, r2
3000530e:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005312:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30005316:	d1f5      	bne.n	30005304 <BOOT_LoadImages+0xa54>
30005318:	ea6f 0903 	mvn.w	r9, r3
3000531c:	45c8      	cmp	r8, r9
3000531e:	f000 80e6 	beq.w	300054ee <BOOT_LoadImages+0xc3e>
30005322:	f8d0 300a 	ldr.w	r3, [r0, #10]
30005326:	3301      	adds	r3, #1
30005328:	d013      	beq.n	30005352 <BOOT_LoadImages+0xaa2>
3000532a:	465b      	mov	r3, fp
3000532c:	464a      	mov	r2, r9
3000532e:	4641      	mov	r1, r8
30005330:	4638      	mov	r0, r7
30005332:	f006 fbdd 	bl	3000baf0 <__DiagPrintf_veneer>
30005336:	f1bb 0f02 	cmp.w	fp, #2
3000533a:	d9d6      	bls.n	300052ea <BOOT_LoadImages+0xa3a>
3000533c:	4641      	mov	r1, r8
3000533e:	464a      	mov	r2, r9
30005340:	482b      	ldr	r0, [pc, #172]	; (300053f0 <BOOT_LoadImages+0xb40>)
30005342:	f006 fbd5 	bl	3000baf0 <__DiagPrintf_veneer>
30005346:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
3000534a:	2002      	movs	r0, #2
3000534c:	f7fe fc40 	bl	30003bd0 <FLASH_Erase_With_Lock>
30005350:	e5a7      	b.n	30004ea2 <BOOT_LoadImages+0x5f2>
30005352:	4659      	mov	r1, fp
30005354:	4828      	ldr	r0, [pc, #160]	; (300053f8 <BOOT_LoadImages+0xb48>)
30005356:	f006 fbcb 	bl	3000baf0 <__DiagPrintf_veneer>
3000535a:	f1bb 0f02 	cmp.w	fp, #2
3000535e:	d9c4      	bls.n	300052ea <BOOT_LoadImages+0xa3a>
30005360:	e59f      	b.n	30004ea2 <BOOT_LoadImages+0x5f2>
30005362:	46b1      	mov	r9, r6
30005364:	e7da      	b.n	3000531c <BOOT_LoadImages+0xa6c>
30005366:	9b04      	ldr	r3, [sp, #16]
30005368:	2b01      	cmp	r3, #1
3000536a:	f47f af7f 	bne.w	3000526c <BOOT_LoadImages+0x9bc>
3000536e:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005372:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005376:	3201      	adds	r2, #1
30005378:	f000 817b 	beq.w	30005672 <BOOT_LoadImages+0xdc2>
3000537c:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005380:	2965      	cmp	r1, #101	; 0x65
30005382:	f000 8164 	beq.w	3000564e <BOOT_LoadImages+0xd9e>
30005386:	481d      	ldr	r0, [pc, #116]	; (300053fc <BOOT_LoadImages+0xb4c>)
30005388:	f04f 0b01 	mov.w	fp, #1
3000538c:	f006 fbb0 	bl	3000baf0 <__DiagPrintf_veneer>
30005390:	e77d      	b.n	3000528e <BOOT_LoadImages+0x9de>
30005392:	9a08      	ldr	r2, [sp, #32]
30005394:	42b2      	cmp	r2, r6
30005396:	d802      	bhi.n	3000539e <BOOT_LoadImages+0xaee>
30005398:	429a      	cmp	r2, r3
3000539a:	f63f ab7b 	bhi.w	30004a94 <BOOT_LoadImages+0x1e4>
3000539e:	480c      	ldr	r0, [pc, #48]	; (300053d0 <BOOT_LoadImages+0xb20>)
300053a0:	f006 fba6 	bl	3000baf0 <__DiagPrintf_veneer>
300053a4:	9b07      	ldr	r3, [sp, #28]
300053a6:	9304      	str	r3, [sp, #16]
300053a8:	f7ff bb30 	b.w	30004a0c <BOOT_LoadImages+0x15c>
300053ac:	9b04      	ldr	r3, [sp, #16]
300053ae:	2b00      	cmp	r3, #0
300053b0:	f040 8163 	bne.w	3000567a <BOOT_LoadImages+0xdca>
300053b4:	2301      	movs	r3, #1
300053b6:	f88d 3064 	strb.w	r3, [sp, #100]	; 0x64
300053ba:	e793      	b.n	300052e4 <BOOT_LoadImages+0xa34>
300053bc:	2001      	movs	r0, #1
300053be:	9001      	str	r0, [sp, #4]
300053c0:	f7fe fd44 	bl	30003e4c <rewrite_bp>
300053c4:	fa5f fb80 	uxtb.w	fp, r0
300053c8:	e48e      	b.n	30004ce8 <BOOT_LoadImages+0x438>
300053ca:	bf00      	nop
300053cc:	3000c4b4 	.word	0x3000c4b4
300053d0:	3000c568 	.word	0x3000c568
300053d4:	3000c080 	.word	0x3000c080
300053d8:	3000bff8 	.word	0x3000bff8
300053dc:	30006961 	.word	0x30006961
300053e0:	e000ed00 	.word	0xe000ed00
300053e4:	3000e164 	.word	0x3000e164
300053e8:	3000c300 	.word	0x3000c300
300053ec:	3000bf1c 	.word	0x3000bf1c
300053f0:	3000c408 	.word	0x3000c408
300053f4:	3000c3bc 	.word	0x3000c3bc
300053f8:	3000c480 	.word	0x3000c480
300053fc:	3000bf04 	.word	0x3000bf04
30005400:	f04f 0b00 	mov.w	fp, #0
30005404:	4ea6      	ldr	r6, [pc, #664]	; (300056a0 <BOOT_LoadImages+0xdf0>)
30005406:	f10b 0b01 	add.w	fp, fp, #1
3000540a:	f8d4 0400 	ldr.w	r0, [r4, #1024]	; 0x400
3000540e:	fa5f fb8b 	uxtb.w	fp, fp
30005412:	1d01      	adds	r1, r0, #4
30005414:	2f00      	cmp	r7, #0
30005416:	d050      	beq.n	300054ba <BOOT_LoadImages+0xc0a>
30005418:	f107 0c04 	add.w	ip, r7, #4
3000541c:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30005420:	4484      	add	ip, r0
30005422:	f811 2b01 	ldrb.w	r2, [r1], #1
30005426:	405a      	eors	r2, r3
30005428:	4561      	cmp	r1, ip
3000542a:	b2d2      	uxtb	r2, r2
3000542c:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005430:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30005434:	d1f5      	bne.n	30005422 <BOOT_LoadImages+0xb72>
30005436:	ea6f 0803 	mvn.w	r8, r3
3000543a:	45c1      	cmp	r9, r8
3000543c:	f000 80b7 	beq.w	300055ae <BOOT_LoadImages+0xcfe>
30005440:	f8d0 300a 	ldr.w	r3, [r0, #10]
30005444:	3301      	adds	r3, #1
30005446:	d030      	beq.n	300054aa <BOOT_LoadImages+0xbfa>
30005448:	465b      	mov	r3, fp
3000544a:	4642      	mov	r2, r8
3000544c:	4649      	mov	r1, r9
3000544e:	4630      	mov	r0, r6
30005450:	f006 fb4e 	bl	3000baf0 <__DiagPrintf_veneer>
30005454:	f1bb 0f02 	cmp.w	fp, #2
30005458:	d9d5      	bls.n	30005406 <BOOT_LoadImages+0xb56>
3000545a:	4649      	mov	r1, r9
3000545c:	4642      	mov	r2, r8
3000545e:	4891      	ldr	r0, [pc, #580]	; (300056a4 <BOOT_LoadImages+0xdf4>)
30005460:	f006 fb46 	bl	3000baf0 <__DiagPrintf_veneer>
30005464:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005468:	2002      	movs	r0, #2
3000546a:	f7fe fbb1 	bl	30003bd0 <FLASH_Erase_With_Lock>
3000546e:	e518      	b.n	30004ea2 <BOOT_LoadImages+0x5f2>
30005470:	2c00      	cmp	r4, #0
30005472:	f43f ad41 	beq.w	30004ef8 <BOOT_LoadImages+0x648>
30005476:	2b00      	cmp	r3, #0
30005478:	f47f ad1c 	bne.w	30004eb4 <BOOT_LoadImages+0x604>
3000547c:	e53c      	b.n	30004ef8 <BOOT_LoadImages+0x648>
3000547e:	488a      	ldr	r0, [pc, #552]	; (300056a8 <BOOT_LoadImages+0xdf8>)
30005480:	f006 fb36 	bl	3000baf0 <__DiagPrintf_veneer>
30005484:	e7fe      	b.n	30005484 <BOOT_LoadImages+0xbd4>
30005486:	4889      	ldr	r0, [pc, #548]	; (300056ac <BOOT_LoadImages+0xdfc>)
30005488:	f006 fb32 	bl	3000baf0 <__DiagPrintf_veneer>
3000548c:	4b88      	ldr	r3, [pc, #544]	; (300056b0 <BOOT_LoadImages+0xe00>)
3000548e:	a814      	add	r0, sp, #80	; 0x50
30005490:	4798      	blx	r3
30005492:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30005496:	4887      	ldr	r0, [pc, #540]	; (300056b4 <BOOT_LoadImages+0xe04>)
30005498:	a914      	add	r1, sp, #80	; 0x50
3000549a:	4b87      	ldr	r3, [pc, #540]	; (300056b8 <BOOT_LoadImages+0xe08>)
3000549c:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
300054a0:	4798      	blx	r3
300054a2:	4884      	ldr	r0, [pc, #528]	; (300056b4 <BOOT_LoadImages+0xe04>)
300054a4:	4b85      	ldr	r3, [pc, #532]	; (300056bc <BOOT_LoadImages+0xe0c>)
300054a6:	4798      	blx	r3
300054a8:	e7fe      	b.n	300054a8 <BOOT_LoadImages+0xbf8>
300054aa:	4659      	mov	r1, fp
300054ac:	4884      	ldr	r0, [pc, #528]	; (300056c0 <BOOT_LoadImages+0xe10>)
300054ae:	f006 fb1f 	bl	3000baf0 <__DiagPrintf_veneer>
300054b2:	f1bb 0f02 	cmp.w	fp, #2
300054b6:	d9a6      	bls.n	30005406 <BOOT_LoadImages+0xb56>
300054b8:	e4f3      	b.n	30004ea2 <BOOT_LoadImages+0x5f2>
300054ba:	46b8      	mov	r8, r7
300054bc:	e7bd      	b.n	3000543a <BOOT_LoadImages+0xb8a>
300054be:	2303      	movs	r3, #3
300054c0:	4880      	ldr	r0, [pc, #512]	; (300056c4 <BOOT_LoadImages+0xe14>)
300054c2:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
300054c6:	f006 fb13 	bl	3000baf0 <__DiagPrintf_veneer>
300054ca:	e6b9      	b.n	30005240 <BOOT_LoadImages+0x990>
300054cc:	2303      	movs	r3, #3
300054ce:	487e      	ldr	r0, [pc, #504]	; (300056c8 <BOOT_LoadImages+0xe18>)
300054d0:	f88d 303b 	strb.w	r3, [sp, #59]	; 0x3b
300054d4:	f006 fb0c 	bl	3000baf0 <__DiagPrintf_veneer>
300054d8:	e6ea      	b.n	300052b0 <BOOT_LoadImages+0xa00>
300054da:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300054de:	f8d3 1006 	ldr.w	r1, [r3, #6]
300054e2:	e56a      	b.n	30004fba <BOOT_LoadImages+0x70a>
300054e4:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300054e8:	f8d3 1006 	ldr.w	r1, [r3, #6]
300054ec:	e54d      	b.n	30004f8a <BOOT_LoadImages+0x6da>
300054ee:	4641      	mov	r1, r8
300054f0:	4642      	mov	r2, r8
300054f2:	4876      	ldr	r0, [pc, #472]	; (300056cc <BOOT_LoadImages+0xe1c>)
300054f4:	f006 fafc 	bl	3000baf0 <__DiagPrintf_veneer>
300054f8:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300054fc:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005500:	2965      	cmp	r1, #101	; 0x65
30005502:	f47f aebf 	bne.w	30005284 <BOOT_LoadImages+0x9d4>
30005506:	4869      	ldr	r0, [pc, #420]	; (300056ac <BOOT_LoadImages+0xdfc>)
30005508:	f006 faf2 	bl	3000baf0 <__DiagPrintf_veneer>
3000550c:	4b68      	ldr	r3, [pc, #416]	; (300056b0 <BOOT_LoadImages+0xe00>)
3000550e:	a814      	add	r0, sp, #80	; 0x50
30005510:	4798      	blx	r3
30005512:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30005516:	4867      	ldr	r0, [pc, #412]	; (300056b4 <BOOT_LoadImages+0xe04>)
30005518:	a914      	add	r1, sp, #80	; 0x50
3000551a:	4b67      	ldr	r3, [pc, #412]	; (300056b8 <BOOT_LoadImages+0xe08>)
3000551c:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
30005520:	4798      	blx	r3
30005522:	4864      	ldr	r0, [pc, #400]	; (300056b4 <BOOT_LoadImages+0xe04>)
30005524:	4b65      	ldr	r3, [pc, #404]	; (300056bc <BOOT_LoadImages+0xe0c>)
30005526:	4798      	blx	r3
30005528:	e7fe      	b.n	30005528 <BOOT_LoadImages+0xc78>
3000552a:	4860      	ldr	r0, [pc, #384]	; (300056ac <BOOT_LoadImages+0xdfc>)
3000552c:	f006 fae0 	bl	3000baf0 <__DiagPrintf_veneer>
30005530:	4b5f      	ldr	r3, [pc, #380]	; (300056b0 <BOOT_LoadImages+0xe00>)
30005532:	a814      	add	r0, sp, #80	; 0x50
30005534:	4798      	blx	r3
30005536:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
3000553a:	485e      	ldr	r0, [pc, #376]	; (300056b4 <BOOT_LoadImages+0xe04>)
3000553c:	a914      	add	r1, sp, #80	; 0x50
3000553e:	4b5e      	ldr	r3, [pc, #376]	; (300056b8 <BOOT_LoadImages+0xe08>)
30005540:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
30005544:	4798      	blx	r3
30005546:	485b      	ldr	r0, [pc, #364]	; (300056b4 <BOOT_LoadImages+0xe04>)
30005548:	4b5c      	ldr	r3, [pc, #368]	; (300056bc <BOOT_LoadImages+0xe0c>)
3000554a:	4798      	blx	r3
3000554c:	e7fe      	b.n	3000554c <BOOT_LoadImages+0xc9c>
3000554e:	4860      	ldr	r0, [pc, #384]	; (300056d0 <BOOT_LoadImages+0xe20>)
30005550:	f006 face 	bl	3000baf0 <__DiagPrintf_veneer>
30005554:	4b56      	ldr	r3, [pc, #344]	; (300056b0 <BOOT_LoadImages+0xe00>)
30005556:	a814      	add	r0, sp, #80	; 0x50
30005558:	4798      	blx	r3
3000555a:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
3000555e:	4855      	ldr	r0, [pc, #340]	; (300056b4 <BOOT_LoadImages+0xe04>)
30005560:	a914      	add	r1, sp, #80	; 0x50
30005562:	4b55      	ldr	r3, [pc, #340]	; (300056b8 <BOOT_LoadImages+0xe08>)
30005564:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
30005568:	4798      	blx	r3
3000556a:	4852      	ldr	r0, [pc, #328]	; (300056b4 <BOOT_LoadImages+0xe04>)
3000556c:	4b53      	ldr	r3, [pc, #332]	; (300056bc <BOOT_LoadImages+0xe0c>)
3000556e:	4798      	blx	r3
30005570:	e7fe      	b.n	30005570 <BOOT_LoadImages+0xcc0>
30005572:	9b03      	ldr	r3, [sp, #12]
30005574:	2001      	movs	r0, #1
30005576:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30005578:	9908      	ldr	r1, [sp, #32]
3000557a:	e9cd 5400 	strd	r5, r4, [sp]
3000557e:	f7fe fc65 	bl	30003e4c <rewrite_bp>
30005582:	fa5f fb80 	uxtb.w	fp, r0
30005586:	f7ff bbaf 	b.w	30004ce8 <BOOT_LoadImages+0x438>
3000558a:	428a      	cmp	r2, r1
3000558c:	f4ff ab73 	bcc.w	30004c76 <BOOT_LoadImages+0x3c6>
30005590:	4282      	cmp	r2, r0
30005592:	f4ff ab70 	bcc.w	30004c76 <BOOT_LoadImages+0x3c6>
30005596:	2001      	movs	r0, #1
30005598:	9b03      	ldr	r3, [sp, #12]
3000559a:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000559c:	9909      	ldr	r1, [sp, #36]	; 0x24
3000559e:	9500      	str	r5, [sp, #0]
300055a0:	9001      	str	r0, [sp, #4]
300055a2:	f7fe fc53 	bl	30003e4c <rewrite_bp>
300055a6:	fa5f fb80 	uxtb.w	fp, r0
300055aa:	f7ff bb9d 	b.w	30004ce8 <BOOT_LoadImages+0x438>
300055ae:	4649      	mov	r1, r9
300055b0:	464a      	mov	r2, r9
300055b2:	4848      	ldr	r0, [pc, #288]	; (300056d4 <BOOT_LoadImages+0xe24>)
300055b4:	f006 fa9c 	bl	3000baf0 <__DiagPrintf_veneer>
300055b8:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300055bc:	f8d3 1006 	ldr.w	r1, [r3, #6]
300055c0:	2965      	cmp	r1, #101	; 0x65
300055c2:	f47f aee0 	bne.w	30005386 <BOOT_LoadImages+0xad6>
300055c6:	4842      	ldr	r0, [pc, #264]	; (300056d0 <BOOT_LoadImages+0xe20>)
300055c8:	f006 fa92 	bl	3000baf0 <__DiagPrintf_veneer>
300055cc:	4b38      	ldr	r3, [pc, #224]	; (300056b0 <BOOT_LoadImages+0xe00>)
300055ce:	a814      	add	r0, sp, #80	; 0x50
300055d0:	4798      	blx	r3
300055d2:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
300055d6:	4837      	ldr	r0, [pc, #220]	; (300056b4 <BOOT_LoadImages+0xe04>)
300055d8:	a914      	add	r1, sp, #80	; 0x50
300055da:	4b37      	ldr	r3, [pc, #220]	; (300056b8 <BOOT_LoadImages+0xe08>)
300055dc:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
300055e0:	4798      	blx	r3
300055e2:	4834      	ldr	r0, [pc, #208]	; (300056b4 <BOOT_LoadImages+0xe04>)
300055e4:	4b35      	ldr	r3, [pc, #212]	; (300056bc <BOOT_LoadImages+0xe0c>)
300055e6:	4798      	blx	r3
300055e8:	e7fe      	b.n	300055e8 <BOOT_LoadImages+0xd38>
300055ea:	3c01      	subs	r4, #1
300055ec:	3403      	adds	r4, #3
300055ee:	d848      	bhi.n	30005682 <BOOT_LoadImages+0xdd2>
300055f0:	2e01      	cmp	r6, #1
300055f2:	d975      	bls.n	300056e0 <BOOT_LoadImages+0xe30>
300055f4:	4838      	ldr	r0, [pc, #224]	; (300056d8 <BOOT_LoadImages+0xe28>)
300055f6:	f006 fa7b 	bl	3000baf0 <__DiagPrintf_veneer>
300055fa:	9b03      	ldr	r3, [sp, #12]
300055fc:	970a      	str	r7, [sp, #40]	; 0x28
300055fe:	f8d3 200d 	ldr.w	r2, [r3, #13]
30005602:	e4ef      	b.n	30004fe4 <BOOT_LoadImages+0x734>
30005604:	4830      	ldr	r0, [pc, #192]	; (300056c8 <BOOT_LoadImages+0xe18>)
30005606:	f006 fa73 	bl	3000baf0 <__DiagPrintf_veneer>
3000560a:	e44a      	b.n	30004ea2 <BOOT_LoadImages+0x5f2>
3000560c:	4830      	ldr	r0, [pc, #192]	; (300056d0 <BOOT_LoadImages+0xe20>)
3000560e:	f006 fa6f 	bl	3000baf0 <__DiagPrintf_veneer>
30005612:	4b27      	ldr	r3, [pc, #156]	; (300056b0 <BOOT_LoadImages+0xe00>)
30005614:	a814      	add	r0, sp, #80	; 0x50
30005616:	4798      	blx	r3
30005618:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
3000561c:	4825      	ldr	r0, [pc, #148]	; (300056b4 <BOOT_LoadImages+0xe04>)
3000561e:	a914      	add	r1, sp, #80	; 0x50
30005620:	4b25      	ldr	r3, [pc, #148]	; (300056b8 <BOOT_LoadImages+0xe08>)
30005622:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
30005626:	4798      	blx	r3
30005628:	4822      	ldr	r0, [pc, #136]	; (300056b4 <BOOT_LoadImages+0xe04>)
3000562a:	4b24      	ldr	r3, [pc, #144]	; (300056bc <BOOT_LoadImages+0xe0c>)
3000562c:	4798      	blx	r3
3000562e:	e7fe      	b.n	3000562e <BOOT_LoadImages+0xd7e>
30005630:	4281      	cmp	r1, r0
30005632:	f4ff ab26 	bcc.w	30004c82 <BOOT_LoadImages+0x3d2>
30005636:	2000      	movs	r0, #0
30005638:	9b03      	ldr	r3, [sp, #12]
3000563a:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
3000563c:	9908      	ldr	r1, [sp, #32]
3000563e:	9500      	str	r5, [sp, #0]
30005640:	9001      	str	r0, [sp, #4]
30005642:	f7fe fc03 	bl	30003e4c <rewrite_bp>
30005646:	fa5f fb80 	uxtb.w	fp, r0
3000564a:	f7ff bb4d 	b.w	30004ce8 <BOOT_LoadImages+0x438>
3000564e:	4820      	ldr	r0, [pc, #128]	; (300056d0 <BOOT_LoadImages+0xe20>)
30005650:	f006 fa4e 	bl	3000baf0 <__DiagPrintf_veneer>
30005654:	4b16      	ldr	r3, [pc, #88]	; (300056b0 <BOOT_LoadImages+0xe00>)
30005656:	a814      	add	r0, sp, #80	; 0x50
30005658:	4798      	blx	r3
3000565a:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
3000565e:	4815      	ldr	r0, [pc, #84]	; (300056b4 <BOOT_LoadImages+0xe04>)
30005660:	a914      	add	r1, sp, #80	; 0x50
30005662:	4b15      	ldr	r3, [pc, #84]	; (300056b8 <BOOT_LoadImages+0xe08>)
30005664:	f8ad 2052 	strh.w	r2, [sp, #82]	; 0x52
30005668:	4798      	blx	r3
3000566a:	4812      	ldr	r0, [pc, #72]	; (300056b4 <BOOT_LoadImages+0xe04>)
3000566c:	4b13      	ldr	r3, [pc, #76]	; (300056bc <BOOT_LoadImages+0xe0c>)
3000566e:	4798      	blx	r3
30005670:	e7fe      	b.n	30005670 <BOOT_LoadImages+0xdc0>
30005672:	4814      	ldr	r0, [pc, #80]	; (300056c4 <BOOT_LoadImages+0xe14>)
30005674:	f006 fa3c 	bl	3000baf0 <__DiagPrintf_veneer>
30005678:	e413      	b.n	30004ea2 <BOOT_LoadImages+0x5f2>
3000567a:	9b07      	ldr	r3, [sp, #28]
3000567c:	f88d 3064 	strb.w	r3, [sp, #100]	; 0x64
30005680:	e62c      	b.n	300052dc <BOOT_LoadImages+0xa2c>
30005682:	4816      	ldr	r0, [pc, #88]	; (300056dc <BOOT_LoadImages+0xe2c>)
30005684:	e7b7      	b.n	300055f6 <BOOT_LoadImages+0xd46>
30005686:	2001      	movs	r0, #1
30005688:	9b03      	ldr	r3, [sp, #12]
3000568a:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000568c:	9001      	str	r0, [sp, #4]
3000568e:	2000      	movs	r0, #0
30005690:	9909      	ldr	r1, [sp, #36]	; 0x24
30005692:	9500      	str	r5, [sp, #0]
30005694:	f7fe fbda 	bl	30003e4c <rewrite_bp>
30005698:	fa5f fb80 	uxtb.w	fp, r0
3000569c:	f7ff bb24 	b.w	30004ce8 <BOOT_LoadImages+0x438>
300056a0:	3000c2b4 	.word	0x3000c2b4
300056a4:	3000c300 	.word	0x3000c300
300056a8:	3000c4ec 	.word	0x3000c4ec
300056ac:	3000c214 	.word	0x3000c214
300056b0:	0000d3f9 	.word	0x0000d3f9
300056b4:	41000440 	.word	0x41000440
300056b8:	0000d409 	.word	0x0000d409
300056bc:	0000d489 	.word	0x0000d489
300056c0:	3000c46c 	.word	0x3000c46c
300056c4:	3000c1ac 	.word	0x3000c1ac
300056c8:	3000c204 	.word	0x3000c204
300056cc:	3000c364 	.word	0x3000c364
300056d0:	3000c1bc 	.word	0x3000c1bc
300056d4:	3000c25c 	.word	0x3000c25c
300056d8:	3000c54c 	.word	0x3000c54c
300056dc:	3000bfe4 	.word	0x3000bfe4
300056e0:	9905      	ldr	r1, [sp, #20]
300056e2:	4809      	ldr	r0, [pc, #36]	; (30005708 <BOOT_LoadImages+0xe58>)
300056e4:	f006 fa04 	bl	3000baf0 <__DiagPrintf_veneer>
300056e8:	9b03      	ldr	r3, [sp, #12]
300056ea:	f8d3 200d 	ldr.w	r2, [r3, #13]
300056ee:	9209      	str	r2, [sp, #36]	; 0x24
300056f0:	f8d3 3011 	ldr.w	r3, [r3, #17]
300056f4:	930c      	str	r3, [sp, #48]	; 0x30
300056f6:	f8d5 300d 	ldr.w	r3, [r5, #13]
300056fa:	9308      	str	r3, [sp, #32]
300056fc:	f8d5 3011 	ldr.w	r3, [r5, #17]
30005700:	930b      	str	r3, [sp, #44]	; 0x2c
30005702:	2301      	movs	r3, #1
30005704:	930a      	str	r3, [sp, #40]	; 0x28
30005706:	e477      	b.n	30004ff8 <BOOT_LoadImages+0x748>
30005708:	3000c584 	.word	0x3000c584

3000570c <BOOT_ReasonSet>:
3000570c:	4a03      	ldr	r2, [pc, #12]	; (3000571c <BOOT_ReasonSet+0x10>)
3000570e:	4904      	ldr	r1, [pc, #16]	; (30005720 <BOOT_ReasonSet+0x14>)
30005710:	8c13      	ldrh	r3, [r2, #32]
30005712:	b29b      	uxth	r3, r3
30005714:	8413      	strh	r3, [r2, #32]
30005716:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
3000571a:	4770      	bx	lr
3000571c:	42008000 	.word	0x42008000
30005720:	42008200 	.word	0x42008200

30005724 <BOOT_Enable_KM0>:
30005724:	b510      	push	{r4, lr}
30005726:	4c12      	ldr	r4, [pc, #72]	; (30005770 <BOOT_Enable_KM0+0x4c>)
30005728:	47a0      	blx	r4
3000572a:	b110      	cbz	r0, 30005732 <BOOT_Enable_KM0+0xe>
3000572c:	4b11      	ldr	r3, [pc, #68]	; (30005774 <BOOT_Enable_KM0+0x50>)
3000572e:	4798      	blx	r3
30005730:	b1a8      	cbz	r0, 3000575e <BOOT_Enable_KM0+0x3a>
30005732:	47a0      	blx	r4
30005734:	b940      	cbnz	r0, 30005748 <BOOT_Enable_KM0+0x24>
30005736:	4a10      	ldr	r2, [pc, #64]	; (30005778 <BOOT_Enable_KM0+0x54>)
30005738:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
3000573c:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
30005740:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
30005744:	bf40      	sev
30005746:	bd10      	pop	{r4, pc}
30005748:	4b0a      	ldr	r3, [pc, #40]	; (30005774 <BOOT_Enable_KM0+0x50>)
3000574a:	4798      	blx	r3
3000574c:	2800      	cmp	r0, #0
3000574e:	d0f2      	beq.n	30005736 <BOOT_Enable_KM0+0x12>
30005750:	4b0a      	ldr	r3, [pc, #40]	; (3000577c <BOOT_Enable_KM0+0x58>)
30005752:	2004      	movs	r0, #4
30005754:	4a0a      	ldr	r2, [pc, #40]	; (30005780 <BOOT_Enable_KM0+0x5c>)
30005756:	4619      	mov	r1, r3
30005758:	f005 ff2c 	bl	3000b5b4 <rtk_log_write>
3000575c:	e7eb      	b.n	30005736 <BOOT_Enable_KM0+0x12>
3000575e:	4b07      	ldr	r3, [pc, #28]	; (3000577c <BOOT_Enable_KM0+0x58>)
30005760:	2004      	movs	r0, #4
30005762:	4a08      	ldr	r2, [pc, #32]	; (30005784 <BOOT_Enable_KM0+0x60>)
30005764:	4619      	mov	r1, r3
30005766:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000576a:	f005 bf23 	b.w	3000b5b4 <rtk_log_write>
3000576e:	bf00      	nop
30005770:	0000c09d 	.word	0x0000c09d
30005774:	0000c0ad 	.word	0x0000c0ad
30005778:	42008000 	.word	0x42008000
3000577c:	3000bd58 	.word	0x3000bd58
30005780:	3000c5c8 	.word	0x3000c5c8
30005784:	3000c5a0 	.word	0x3000c5a0

30005788 <BOOT_AP_Clk_Get>:
30005788:	4a06      	ldr	r2, [pc, #24]	; (300057a4 <BOOT_AP_Clk_Get+0x1c>)
3000578a:	6913      	ldr	r3, [r2, #16]
3000578c:	2b00      	cmp	r3, #0
3000578e:	db03      	blt.n	30005798 <BOOT_AP_Clk_Get+0x10>
30005790:	6810      	ldr	r0, [r2, #0]
30005792:	fbb0 f0f3 	udiv	r0, r0, r3
30005796:	4770      	bx	lr
30005798:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000579c:	6850      	ldr	r0, [r2, #4]
3000579e:	fbb0 f0f3 	udiv	r0, r0, r3
300057a2:	4770      	bx	lr
300057a4:	3000e674 	.word	0x3000e674

300057a8 <BOOT_SOC_ClkSet>:
300057a8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300057ac:	4c5d      	ldr	r4, [pc, #372]	; (30005924 <BOOT_SOC_ClkSet+0x17c>)
300057ae:	b083      	sub	sp, #12
300057b0:	6926      	ldr	r6, [r4, #16]
300057b2:	6825      	ldr	r5, [r4, #0]
300057b4:	2e00      	cmp	r6, #0
300057b6:	db7a      	blt.n	300058ae <BOOT_SOC_ClkSet+0x106>
300057b8:	fbb5 f6f6 	udiv	r6, r5, r6
300057bc:	4b5a      	ldr	r3, [pc, #360]	; (30005928 <BOOT_SOC_ClkSet+0x180>)
300057be:	68a7      	ldr	r7, [r4, #8]
300057c0:	fba3 1305 	umull	r1, r3, r3, r5
300057c4:	68e2      	ldr	r2, [r4, #12]
300057c6:	2f01      	cmp	r7, #1
300057c8:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
300057cc:	ea4f 1b53 	mov.w	fp, r3, lsr #5
300057d0:	ea4f 1393 	mov.w	r3, r3, lsr #6
300057d4:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
300057d8:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
300057dc:	f103 39ff 	add.w	r9, r3, #4294967295	; 0xffffffff
300057e0:	fbb5 f5f2 	udiv	r5, r5, r2
300057e4:	d06f      	beq.n	300058c6 <BOOT_SOC_ClkSet+0x11e>
300057e6:	4b51      	ldr	r3, [pc, #324]	; (3000592c <BOOT_SOC_ClkSet+0x184>)
300057e8:	2200      	movs	r2, #0
300057ea:	2dfa      	cmp	r5, #250	; 0xfa
300057ec:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
300057f0:	d863      	bhi.n	300058ba <BOOT_SOC_ClkSet+0x112>
300057f2:	4f4f      	ldr	r7, [pc, #316]	; (30005930 <BOOT_SOC_ClkSet+0x188>)
300057f4:	783b      	ldrb	r3, [r7, #0]
300057f6:	2b01      	cmp	r3, #1
300057f8:	d103      	bne.n	30005802 <BOOT_SOC_ClkSet+0x5a>
300057fa:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
300057fe:	f200 8085 	bhi.w	3000590c <BOOT_SOC_ClkSet+0x164>
30005802:	2001      	movs	r0, #1
30005804:	f8df 8154 	ldr.w	r8, [pc, #340]	; 3000595c <BOOT_SOC_ClkSet+0x1b4>
30005808:	f001 fecc 	bl	300075a4 <CLK_SWITCH_XTAL>
3000580c:	6820      	ldr	r0, [r4, #0]
3000580e:	4b49      	ldr	r3, [pc, #292]	; (30005934 <BOOT_SOC_ClkSet+0x18c>)
30005810:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
30005814:	fb08 f000 	mul.w	r0, r8, r0
30005818:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
3000581c:	4798      	blx	r3
3000581e:	4a46      	ldr	r2, [pc, #280]	; (30005938 <BOOT_SOC_ClkSet+0x190>)
30005820:	68e1      	ldr	r1, [r4, #12]
30005822:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
30005826:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
3000582a:	ea4f 5909 	mov.w	r9, r9, lsl #20
3000582e:	1e4b      	subs	r3, r1, #1
30005830:	4942      	ldr	r1, [pc, #264]	; (3000593c <BOOT_SOC_ClkSet+0x194>)
30005832:	fa1f fb8b 	uxth.w	fp, fp
30005836:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
3000583a:	4001      	ands	r1, r0
3000583c:	f003 0307 	and.w	r3, r3, #7
30005840:	2000      	movs	r0, #0
30005842:	430b      	orrs	r3, r1
30005844:	ea4a 0303 	orr.w	r3, sl, r3
30005848:	ea4b 0303 	orr.w	r3, fp, r3
3000584c:	ea49 0303 	orr.w	r3, r9, r3
30005850:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
30005854:	f001 fea6 	bl	300075a4 <CLK_SWITCH_XTAL>
30005858:	4b39      	ldr	r3, [pc, #228]	; (30005940 <BOOT_SOC_ClkSet+0x198>)
3000585a:	4a3a      	ldr	r2, [pc, #232]	; (30005944 <BOOT_SOC_ClkSet+0x19c>)
3000585c:	2004      	movs	r0, #4
3000585e:	4619      	mov	r1, r3
30005860:	9500      	str	r5, [sp, #0]
30005862:	f005 fea7 	bl	3000b5b4 <rtk_log_write>
30005866:	783b      	ldrb	r3, [r7, #0]
30005868:	b1f3      	cbz	r3, 300058a8 <BOOT_SOC_ClkSet+0x100>
3000586a:	6923      	ldr	r3, [r4, #16]
3000586c:	2b00      	cmp	r3, #0
3000586e:	db40      	blt.n	300058f2 <BOOT_SOC_ClkSet+0x14a>
30005870:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005874:	492d      	ldr	r1, [pc, #180]	; (3000592c <BOOT_SOC_ClkSet+0x184>)
30005876:	2000      	movs	r0, #0
30005878:	6913      	ldr	r3, [r2, #16]
3000587a:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
3000587e:	f043 030c 	orr.w	r3, r3, #12
30005882:	6113      	str	r3, [r2, #16]
30005884:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
30005888:	6921      	ldr	r1, [r4, #16]
3000588a:	4b2d      	ldr	r3, [pc, #180]	; (30005940 <BOOT_SOC_ClkSet+0x198>)
3000588c:	6928      	ldr	r0, [r5, #16]
3000588e:	3901      	subs	r1, #1
30005890:	4a2d      	ldr	r2, [pc, #180]	; (30005948 <BOOT_SOC_ClkSet+0x1a0>)
30005892:	f020 0003 	bic.w	r0, r0, #3
30005896:	f001 0103 	and.w	r1, r1, #3
3000589a:	4301      	orrs	r1, r0
3000589c:	2004      	movs	r0, #4
3000589e:	6129      	str	r1, [r5, #16]
300058a0:	4619      	mov	r1, r3
300058a2:	9600      	str	r6, [sp, #0]
300058a4:	f005 fe86 	bl	3000b5b4 <rtk_log_write>
300058a8:	b003      	add	sp, #12
300058aa:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300058ae:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
300058b2:	6863      	ldr	r3, [r4, #4]
300058b4:	fbb3 f6f6 	udiv	r6, r3, r6
300058b8:	e780      	b.n	300057bc <BOOT_SOC_ClkSet+0x14>
300058ba:	f240 7197 	movw	r1, #1943	; 0x797
300058be:	4823      	ldr	r0, [pc, #140]	; (3000594c <BOOT_SOC_ClkSet+0x1a4>)
300058c0:	f006 f956 	bl	3000bb70 <__io_assert_failed_veneer>
300058c4:	e795      	b.n	300057f2 <BOOT_SOC_ClkSet+0x4a>
300058c6:	4b22      	ldr	r3, [pc, #136]	; (30005950 <BOOT_SOC_ClkSet+0x1a8>)
300058c8:	4638      	mov	r0, r7
300058ca:	4798      	blx	r3
300058cc:	4b17      	ldr	r3, [pc, #92]	; (3000592c <BOOT_SOC_ClkSet+0x184>)
300058ce:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
300058d2:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
300058d6:	d21f      	bcs.n	30005918 <BOOT_SOC_ClkSet+0x170>
300058d8:	4f15      	ldr	r7, [pc, #84]	; (30005930 <BOOT_SOC_ClkSet+0x188>)
300058da:	783b      	ldrb	r3, [r7, #0]
300058dc:	2b01      	cmp	r3, #1
300058de:	d190      	bne.n	30005802 <BOOT_SOC_ClkSet+0x5a>
300058e0:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
300058e4:	d98d      	bls.n	30005802 <BOOT_SOC_ClkSet+0x5a>
300058e6:	f240 7193 	movw	r1, #1939	; 0x793
300058ea:	4818      	ldr	r0, [pc, #96]	; (3000594c <BOOT_SOC_ClkSet+0x1a4>)
300058ec:	f006 f940 	bl	3000bb70 <__io_assert_failed_veneer>
300058f0:	e787      	b.n	30005802 <BOOT_SOC_ClkSet+0x5a>
300058f2:	6860      	ldr	r0, [r4, #4]
300058f4:	4b17      	ldr	r3, [pc, #92]	; (30005954 <BOOT_SOC_ClkSet+0x1ac>)
300058f6:	fb08 f000 	mul.w	r0, r8, r0
300058fa:	4798      	blx	r3
300058fc:	4b16      	ldr	r3, [pc, #88]	; (30005958 <BOOT_SOC_ClkSet+0x1b0>)
300058fe:	2001      	movs	r0, #1
30005900:	4798      	blx	r3
30005902:	4b0a      	ldr	r3, [pc, #40]	; (3000592c <BOOT_SOC_ClkSet+0x184>)
30005904:	2201      	movs	r2, #1
30005906:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
3000590a:	e7bb      	b.n	30005884 <BOOT_SOC_ClkSet+0xdc>
3000590c:	f240 7199 	movw	r1, #1945	; 0x799
30005910:	480e      	ldr	r0, [pc, #56]	; (3000594c <BOOT_SOC_ClkSet+0x1a4>)
30005912:	f006 f92d 	bl	3000bb70 <__io_assert_failed_veneer>
30005916:	e774      	b.n	30005802 <BOOT_SOC_ClkSet+0x5a>
30005918:	f240 7191 	movw	r1, #1937	; 0x791
3000591c:	480b      	ldr	r0, [pc, #44]	; (3000594c <BOOT_SOC_ClkSet+0x1a4>)
3000591e:	f006 f927 	bl	3000bb70 <__io_assert_failed_veneer>
30005922:	e7d9      	b.n	300058d8 <BOOT_SOC_ClkSet+0x130>
30005924:	3000e674 	.word	0x3000e674
30005928:	51eb851f 	.word	0x51eb851f
3000592c:	23020000 	.word	0x23020000
30005930:	3000df9f 	.word	0x3000df9f
30005934:	300081cd 	.word	0x300081cd
30005938:	42008000 	.word	0x42008000
3000593c:	ff8f08f8 	.word	0xff8f08f8
30005940:	3000bd58 	.word	0x3000bd58
30005944:	3000c5e8 	.word	0x3000c5e8
30005948:	3000c600 	.word	0x3000c600
3000594c:	3000dc64 	.word	0x3000dc64
30005950:	3000af35 	.word	0x3000af35
30005954:	3000824d 	.word	0x3000824d
30005958:	3000828d 	.word	0x3000828d
3000595c:	000f4240 	.word	0x000f4240

30005960 <BOOT_Disable_AP>:
30005960:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005964:	4906      	ldr	r1, [pc, #24]	; (30005980 <BOOT_Disable_AP+0x20>)
30005966:	685a      	ldr	r2, [r3, #4]
30005968:	f042 0202 	orr.w	r2, r2, #2
3000596c:	605a      	str	r2, [r3, #4]
3000596e:	685a      	ldr	r2, [r3, #4]
30005970:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30005974:	605a      	str	r2, [r3, #4]
30005976:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30005978:	f043 0303 	orr.w	r3, r3, #3
3000597c:	678b      	str	r3, [r1, #120]	; 0x78
3000597e:	4770      	bx	lr
30005980:	41000200 	.word	0x41000200

30005984 <BOOT_Enable_AP>:
30005984:	b538      	push	{r3, r4, r5, lr}
30005986:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
3000598a:	2201      	movs	r2, #1
3000598c:	2104      	movs	r1, #4
3000598e:	4b2b      	ldr	r3, [pc, #172]	; (30005a3c <BOOT_Enable_AP+0xb8>)
30005990:	482b      	ldr	r0, [pc, #172]	; (30005a40 <BOOT_Enable_AP+0xbc>)
30005992:	4798      	blx	r3
30005994:	6863      	ldr	r3, [r4, #4]
30005996:	4d2b      	ldr	r5, [pc, #172]	; (30005a44 <BOOT_Enable_AP+0xc0>)
30005998:	2032      	movs	r0, #50	; 0x32
3000599a:	f043 0302 	orr.w	r3, r3, #2
3000599e:	6063      	str	r3, [r4, #4]
300059a0:	6863      	ldr	r3, [r4, #4]
300059a2:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300059a6:	6063      	str	r3, [r4, #4]
300059a8:	6823      	ldr	r3, [r4, #0]
300059aa:	f043 0302 	orr.w	r3, r3, #2
300059ae:	6023      	str	r3, [r4, #0]
300059b0:	47a8      	blx	r5
300059b2:	6823      	ldr	r3, [r4, #0]
300059b4:	2032      	movs	r0, #50	; 0x32
300059b6:	f043 0304 	orr.w	r3, r3, #4
300059ba:	6023      	str	r3, [r4, #0]
300059bc:	6823      	ldr	r3, [r4, #0]
300059be:	f043 0301 	orr.w	r3, r3, #1
300059c2:	6023      	str	r3, [r4, #0]
300059c4:	6823      	ldr	r3, [r4, #0]
300059c6:	f043 0310 	orr.w	r3, r3, #16
300059ca:	6023      	str	r3, [r4, #0]
300059cc:	47a8      	blx	r5
300059ce:	6823      	ldr	r3, [r4, #0]
300059d0:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
300059d4:	f043 0340 	orr.w	r3, r3, #64	; 0x40
300059d8:	6023      	str	r3, [r4, #0]
300059da:	47a8      	blx	r5
300059dc:	6863      	ldr	r3, [r4, #4]
300059de:	4a1a      	ldr	r2, [pc, #104]	; (30005a48 <BOOT_Enable_AP+0xc4>)
300059e0:	f023 0310 	bic.w	r3, r3, #16
300059e4:	6063      	str	r3, [r4, #4]
300059e6:	6863      	ldr	r3, [r4, #4]
300059e8:	f023 0302 	bic.w	r3, r3, #2
300059ec:	6063      	str	r3, [r4, #4]
300059ee:	4790      	blx	r2
300059f0:	b178      	cbz	r0, 30005a12 <BOOT_Enable_AP+0x8e>
300059f2:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300059f6:	4815      	ldr	r0, [pc, #84]	; (30005a4c <BOOT_Enable_AP+0xc8>)
300059f8:	4a15      	ldr	r2, [pc, #84]	; (30005a50 <BOOT_Enable_AP+0xcc>)
300059fa:	68d9      	ldr	r1, [r3, #12]
300059fc:	f041 0101 	orr.w	r1, r1, #1
30005a00:	60d9      	str	r1, [r3, #12]
30005a02:	6899      	ldr	r1, [r3, #8]
30005a04:	f041 0101 	orr.w	r1, r1, #1
30005a08:	6099      	str	r1, [r3, #8]
30005a0a:	6843      	ldr	r3, [r0, #4]
30005a0c:	431a      	orrs	r2, r3
30005a0e:	6042      	str	r2, [r0, #4]
30005a10:	bd38      	pop	{r3, r4, r5, pc}
30005a12:	4604      	mov	r4, r0
30005a14:	4d0f      	ldr	r5, [pc, #60]	; (30005a54 <BOOT_Enable_AP+0xd0>)
30005a16:	4601      	mov	r1, r0
30005a18:	2041      	movs	r0, #65	; 0x41
30005a1a:	47a8      	blx	r5
30005a1c:	4621      	mov	r1, r4
30005a1e:	2042      	movs	r0, #66	; 0x42
30005a20:	47a8      	blx	r5
30005a22:	4621      	mov	r1, r4
30005a24:	2043      	movs	r0, #67	; 0x43
30005a26:	47a8      	blx	r5
30005a28:	4621      	mov	r1, r4
30005a2a:	2044      	movs	r0, #68	; 0x44
30005a2c:	47a8      	blx	r5
30005a2e:	4621      	mov	r1, r4
30005a30:	2045      	movs	r0, #69	; 0x45
30005a32:	47a8      	blx	r5
30005a34:	4621      	mov	r1, r4
30005a36:	2046      	movs	r0, #70	; 0x46
30005a38:	47a8      	blx	r5
30005a3a:	e7da      	b.n	300059f2 <BOOT_Enable_AP+0x6e>
30005a3c:	0000aab9 	.word	0x0000aab9
30005a40:	4200c000 	.word	0x4200c000
30005a44:	00009b2d 	.word	0x00009b2d
30005a48:	300092c5 	.word	0x300092c5
30005a4c:	41000200 	.word	0x41000200
30005a50:	01001111 	.word	0x01001111
30005a54:	0000b20d 	.word	0x0000b20d

30005a58 <BOOT_RAM_KeyDeriveFunc>:
30005a58:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005a5c:	ed2d 8b04 	vpush	{d8-d9}
30005a60:	b0cb      	sub	sp, #300	; 0x12c
30005a62:	4c6a      	ldr	r4, [pc, #424]	; (30005c0c <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30005a64:	4680      	mov	r8, r0
30005a66:	4689      	mov	r9, r1
30005a68:	4869      	ldr	r0, [pc, #420]	; (30005c10 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30005a6a:	ee08 2a90 	vmov	s17, r2
30005a6e:	4969      	ldr	r1, [pc, #420]	; (30005c14 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005a70:	2201      	movs	r2, #1
30005a72:	ee09 3a10 	vmov	s18, r3
30005a76:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30005a7a:	47a0      	blx	r4
30005a7c:	2201      	movs	r2, #1
30005a7e:	4966      	ldr	r1, [pc, #408]	; (30005c18 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30005a80:	ae02      	add	r6, sp, #8
30005a82:	4864      	ldr	r0, [pc, #400]	; (30005c14 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005a84:	47a0      	blx	r4
30005a86:	4b65      	ldr	r3, [pc, #404]	; (30005c1c <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30005a88:	2000      	movs	r0, #0
30005a8a:	4798      	blx	r3
30005a8c:	22e0      	movs	r2, #224	; 0xe0
30005a8e:	2100      	movs	r1, #0
30005a90:	a812      	add	r0, sp, #72	; 0x48
30005a92:	f006 f895 	bl	3000bbc0 <____wrap_memset_veneer>
30005a96:	2300      	movs	r3, #0
30005a98:	f8ad 3004 	strh.w	r3, [sp, #4]
30005a9c:	f88d 3006 	strb.w	r3, [sp, #6]
30005aa0:	2301      	movs	r3, #1
30005aa2:	f88d 3007 	strb.w	r3, [sp, #7]
30005aa6:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005aa8:	2b00      	cmp	r3, #0
30005aaa:	f000 80ab 	beq.w	30005c04 <BOOT_RAM_KeyDeriveFunc+0x1ac>
30005aae:	ab0a      	add	r3, sp, #40	; 0x28
30005ab0:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30005c30 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30005ab4:	4f5a      	ldr	r7, [pc, #360]	; (30005c20 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30005ab6:	ee08 3a10 	vmov	s16, r3
30005aba:	ab12      	add	r3, sp, #72	; 0x48
30005abc:	464a      	mov	r2, r9
30005abe:	4641      	mov	r1, r8
30005ac0:	2020      	movs	r0, #32
30005ac2:	47d8      	blx	fp
30005ac4:	aa12      	add	r2, sp, #72	; 0x48
30005ac6:	ee19 1a10 	vmov	r1, s18
30005aca:	ee18 0a90 	vmov	r0, s17
30005ace:	47b8      	blx	r7
30005ad0:	ee18 5a10 	vmov	r5, s16
30005ad4:	aa12      	add	r2, sp, #72	; 0x48
30005ad6:	2104      	movs	r1, #4
30005ad8:	a801      	add	r0, sp, #4
30005ada:	4634      	mov	r4, r6
30005adc:	47b8      	blx	r7
30005ade:	a912      	add	r1, sp, #72	; 0x48
30005ae0:	4b50      	ldr	r3, [pc, #320]	; (30005c24 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30005ae2:	ee18 0a10 	vmov	r0, s16
30005ae6:	4798      	blx	r3
30005ae8:	f1ba 0f01 	cmp.w	sl, #1
30005aec:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30005aee:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30005af0:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30005af4:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30005af8:	d91f      	bls.n	30005b3a <BOOT_RAM_KeyDeriveFunc+0xe2>
30005afa:	2501      	movs	r5, #1
30005afc:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30005b00:	ab12      	add	r3, sp, #72	; 0x48
30005b02:	4641      	mov	r1, r8
30005b04:	464a      	mov	r2, r9
30005b06:	2020      	movs	r0, #32
30005b08:	47d8      	blx	fp
30005b0a:	aa12      	add	r2, sp, #72	; 0x48
30005b0c:	2120      	movs	r1, #32
30005b0e:	4630      	mov	r0, r6
30005b10:	47b8      	blx	r7
30005b12:	a912      	add	r1, sp, #72	; 0x48
30005b14:	4b43      	ldr	r3, [pc, #268]	; (30005c24 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30005b16:	4630      	mov	r0, r6
30005b18:	4798      	blx	r3
30005b1a:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30005b1e:	f10d 0107 	add.w	r1, sp, #7
30005b22:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30005b26:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30005b2a:	42a3      	cmp	r3, r4
30005b2c:	ea82 0200 	eor.w	r2, r2, r0
30005b30:	701a      	strb	r2, [r3, #0]
30005b32:	d1f6      	bne.n	30005b22 <BOOT_RAM_KeyDeriveFunc+0xca>
30005b34:	3501      	adds	r5, #1
30005b36:	45aa      	cmp	sl, r5
30005b38:	d1e2      	bne.n	30005b00 <BOOT_RAM_KeyDeriveFunc+0xa8>
30005b3a:	9a59      	ldr	r2, [sp, #356]	; 0x164
30005b3c:	ee18 1a10 	vmov	r1, s16
30005b40:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005b42:	2a20      	cmp	r2, #32
30005b44:	985a      	ldr	r0, [sp, #360]	; 0x168
30005b46:	bf28      	it	cs
30005b48:	2220      	movcs	r2, #32
30005b4a:	1a9b      	subs	r3, r3, r2
30005b4c:	9359      	str	r3, [sp, #356]	; 0x164
30005b4e:	4603      	mov	r3, r0
30005b50:	4413      	add	r3, r2
30005b52:	935a      	str	r3, [sp, #360]	; 0x168
30005b54:	f005 ffbc 	bl	3000bad0 <____wrap_memcpy_veneer>
30005b58:	f10d 0207 	add.w	r2, sp, #7
30005b5c:	2104      	movs	r1, #4
30005b5e:	7813      	ldrb	r3, [r2, #0]
30005b60:	3901      	subs	r1, #1
30005b62:	3301      	adds	r3, #1
30005b64:	b2db      	uxtb	r3, r3
30005b66:	f802 3901 	strb.w	r3, [r2], #-1
30005b6a:	b90b      	cbnz	r3, 30005b70 <BOOT_RAM_KeyDeriveFunc+0x118>
30005b6c:	2900      	cmp	r1, #0
30005b6e:	d1f6      	bne.n	30005b5e <BOOT_RAM_KeyDeriveFunc+0x106>
30005b70:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005b72:	2b00      	cmp	r3, #0
30005b74:	d1a1      	bne.n	30005aba <BOOT_RAM_KeyDeriveFunc+0x62>
30005b76:	2220      	movs	r2, #32
30005b78:	2100      	movs	r1, #0
30005b7a:	4c2b      	ldr	r4, [pc, #172]	; (30005c28 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30005b7c:	4630      	mov	r0, r6
30005b7e:	47a0      	blx	r4
30005b80:	2220      	movs	r2, #32
30005b82:	2100      	movs	r1, #0
30005b84:	ee18 0a10 	vmov	r0, s16
30005b88:	47a0      	blx	r4
30005b8a:	4b28      	ldr	r3, [pc, #160]	; (30005c2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005b8c:	695b      	ldr	r3, [r3, #20]
30005b8e:	03da      	lsls	r2, r3, #15
30005b90:	d512      	bpl.n	30005bb8 <BOOT_RAM_KeyDeriveFunc+0x160>
30005b92:	f016 021f 	ands.w	r2, r6, #31
30005b96:	4633      	mov	r3, r6
30005b98:	d130      	bne.n	30005bfc <BOOT_RAM_KeyDeriveFunc+0x1a4>
30005b9a:	2220      	movs	r2, #32
30005b9c:	f3bf 8f4f 	dsb	sy
30005ba0:	4822      	ldr	r0, [pc, #136]	; (30005c2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005ba2:	441a      	add	r2, r3
30005ba4:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005ba8:	3320      	adds	r3, #32
30005baa:	1ad1      	subs	r1, r2, r3
30005bac:	2900      	cmp	r1, #0
30005bae:	dcf9      	bgt.n	30005ba4 <BOOT_RAM_KeyDeriveFunc+0x14c>
30005bb0:	f3bf 8f4f 	dsb	sy
30005bb4:	f3bf 8f6f 	isb	sy
30005bb8:	4b1c      	ldr	r3, [pc, #112]	; (30005c2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005bba:	695b      	ldr	r3, [r3, #20]
30005bbc:	03db      	lsls	r3, r3, #15
30005bbe:	d513      	bpl.n	30005be8 <BOOT_RAM_KeyDeriveFunc+0x190>
30005bc0:	ee18 3a10 	vmov	r3, s16
30005bc4:	f013 021f 	ands.w	r2, r3, #31
30005bc8:	d114      	bne.n	30005bf4 <BOOT_RAM_KeyDeriveFunc+0x19c>
30005bca:	2220      	movs	r2, #32
30005bcc:	f3bf 8f4f 	dsb	sy
30005bd0:	4816      	ldr	r0, [pc, #88]	; (30005c2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005bd2:	441a      	add	r2, r3
30005bd4:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005bd8:	3320      	adds	r3, #32
30005bda:	1ad1      	subs	r1, r2, r3
30005bdc:	2900      	cmp	r1, #0
30005bde:	dcf9      	bgt.n	30005bd4 <BOOT_RAM_KeyDeriveFunc+0x17c>
30005be0:	f3bf 8f4f 	dsb	sy
30005be4:	f3bf 8f6f 	isb	sy
30005be8:	2000      	movs	r0, #0
30005bea:	b04b      	add	sp, #300	; 0x12c
30005bec:	ecbd 8b04 	vpop	{d8-d9}
30005bf0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005bf4:	f023 031f 	bic.w	r3, r3, #31
30005bf8:	3220      	adds	r2, #32
30005bfa:	e7e7      	b.n	30005bcc <BOOT_RAM_KeyDeriveFunc+0x174>
30005bfc:	f026 031f 	bic.w	r3, r6, #31
30005c00:	3220      	adds	r2, #32
30005c02:	e7cb      	b.n	30005b9c <BOOT_RAM_KeyDeriveFunc+0x144>
30005c04:	ab0a      	add	r3, sp, #40	; 0x28
30005c06:	ee08 3a10 	vmov	s16, r3
30005c0a:	e7b4      	b.n	30005b76 <BOOT_RAM_KeyDeriveFunc+0x11e>
30005c0c:	0000b479 	.word	0x0000b479
30005c10:	40000008 	.word	0x40000008
30005c14:	40000004 	.word	0x40000004
30005c18:	40000002 	.word	0x40000002
30005c1c:	30009561 	.word	0x30009561
30005c20:	30009365 	.word	0x30009365
30005c24:	300093a1 	.word	0x300093a1
30005c28:	00012be5 	.word	0x00012be5
30005c2c:	e000ed00 	.word	0xe000ed00
30005c30:	30008c65 	.word	0x30008c65

30005c34 <BOOT_Share_Memory_Patch>:
30005c34:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005c38:	4817      	ldr	r0, [pc, #92]	; (30005c98 <BOOT_Share_Memory_Patch+0x64>)
30005c3a:	4918      	ldr	r1, [pc, #96]	; (30005c9c <BOOT_Share_Memory_Patch+0x68>)
30005c3c:	b430      	push	{r4, r5}
30005c3e:	6b13      	ldr	r3, [r2, #48]	; 0x30
30005c40:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30005c44:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30005c48:	6313      	str	r3, [r2, #48]	; 0x30
30005c4a:	6823      	ldr	r3, [r4, #0]
30005c4c:	6804      	ldr	r4, [r0, #0]
30005c4e:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005c52:	6809      	ldr	r1, [r1, #0]
30005c54:	4423      	add	r3, r4
30005c56:	6805      	ldr	r5, [r0, #0]
30005c58:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30005c5c:	440b      	add	r3, r1
30005c5e:	4910      	ldr	r1, [pc, #64]	; (30005ca0 <BOOT_Share_Memory_Patch+0x6c>)
30005c60:	6808      	ldr	r0, [r1, #0]
30005c62:	442b      	add	r3, r5
30005c64:	6b11      	ldr	r1, [r2, #48]	; 0x30
30005c66:	4403      	add	r3, r0
30005c68:	480e      	ldr	r0, [pc, #56]	; (30005ca4 <BOOT_Share_Memory_Patch+0x70>)
30005c6a:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30005c6e:	6311      	str	r1, [r2, #48]	; 0x30
30005c70:	6821      	ldr	r1, [r4, #0]
30005c72:	4a0d      	ldr	r2, [pc, #52]	; (30005ca8 <BOOT_Share_Memory_Patch+0x74>)
30005c74:	6804      	ldr	r4, [r0, #0]
30005c76:	440b      	add	r3, r1
30005c78:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005c7c:	6811      	ldr	r1, [r2, #0]
30005c7e:	4423      	add	r3, r4
30005c80:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30005c84:	6800      	ldr	r0, [r0, #0]
30005c86:	440b      	add	r3, r1
30005c88:	6812      	ldr	r2, [r2, #0]
30005c8a:	4908      	ldr	r1, [pc, #32]	; (30005cac <BOOT_Share_Memory_Patch+0x78>)
30005c8c:	4403      	add	r3, r0
30005c8e:	6808      	ldr	r0, [r1, #0]
30005c90:	4413      	add	r3, r2
30005c92:	bc30      	pop	{r4, r5}
30005c94:	4418      	add	r0, r3
30005c96:	4770      	bx	lr
30005c98:	2200c000 	.word	0x2200c000
30005c9c:	22018000 	.word	0x22018000
30005ca0:	22100000 	.word	0x22100000
30005ca4:	20010000 	.word	0x20010000
30005ca8:	20020000 	.word	0x20020000
30005cac:	23010000 	.word	0x23010000

30005cb0 <BOOT_Image1>:
30005cb0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30005cb4:	4bac      	ldr	r3, [pc, #688]	; (30005f68 <BOOT_Image1+0x2b8>)
30005cb6:	b082      	sub	sp, #8
30005cb8:	4798      	blx	r3
30005cba:	2800      	cmp	r0, #0
30005cbc:	f040 80c1 	bne.w	30005e42 <BOOT_Image1+0x192>
30005cc0:	48aa      	ldr	r0, [pc, #680]	; (30005f6c <BOOT_Image1+0x2bc>)
30005cc2:	f005 ff15 	bl	3000baf0 <__DiagPrintf_veneer>
30005cc6:	49aa      	ldr	r1, [pc, #680]	; (30005f70 <BOOT_Image1+0x2c0>)
30005cc8:	48aa      	ldr	r0, [pc, #680]	; (30005f74 <BOOT_Image1+0x2c4>)
30005cca:	f005 ff11 	bl	3000baf0 <__DiagPrintf_veneer>
30005cce:	48aa      	ldr	r0, [pc, #680]	; (30005f78 <BOOT_Image1+0x2c8>)
30005cd0:	4aaa      	ldr	r2, [pc, #680]	; (30005f7c <BOOT_Image1+0x2cc>)
30005cd2:	2100      	movs	r1, #0
30005cd4:	4baa      	ldr	r3, [pc, #680]	; (30005f80 <BOOT_Image1+0x2d0>)
30005cd6:	1a12      	subs	r2, r2, r0
30005cd8:	4798      	blx	r3
30005cda:	4aaa      	ldr	r2, [pc, #680]	; (30005f84 <BOOT_Image1+0x2d4>)
30005cdc:	49aa      	ldr	r1, [pc, #680]	; (30005f88 <BOOT_Image1+0x2d8>)
30005cde:	8c13      	ldrh	r3, [r2, #32]
30005ce0:	b29b      	uxth	r3, r3
30005ce2:	8413      	strh	r3, [r2, #32]
30005ce4:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005ce8:	f002 fede 	bl	30008aa8 <BOOT_Reason>
30005cec:	2800      	cmp	r0, #0
30005cee:	f000 80e9 	beq.w	30005ec4 <BOOT_Image1+0x214>
30005cf2:	4ea6      	ldr	r6, [pc, #664]	; (30005f8c <BOOT_Image1+0x2dc>)
30005cf4:	4ba6      	ldr	r3, [pc, #664]	; (30005f90 <BOOT_Image1+0x2e0>)
30005cf6:	4798      	blx	r3
30005cf8:	7834      	ldrb	r4, [r6, #0]
30005cfa:	2c00      	cmp	r4, #0
30005cfc:	f000 8097 	beq.w	30005e2e <BOOT_Image1+0x17e>
30005d00:	2000      	movs	r0, #0
30005d02:	4ba4      	ldr	r3, [pc, #656]	; (30005f94 <BOOT_Image1+0x2e4>)
30005d04:	4798      	blx	r3
30005d06:	4ba4      	ldr	r3, [pc, #656]	; (30005f98 <BOOT_Image1+0x2e8>)
30005d08:	2001      	movs	r0, #1
30005d0a:	4798      	blx	r3
30005d0c:	4c9e      	ldr	r4, [pc, #632]	; (30005f88 <BOOT_Image1+0x2d8>)
30005d0e:	f7ff fd4b 	bl	300057a8 <BOOT_SOC_ClkSet>
30005d12:	f241 070a 	movw	r7, #4106	; 0x100a
30005d16:	2028      	movs	r0, #40	; 0x28
30005d18:	8823      	ldrh	r3, [r4, #0]
30005d1a:	4da0      	ldr	r5, [pc, #640]	; (30005f9c <BOOT_Image1+0x2ec>)
30005d1c:	b29b      	uxth	r3, r3
30005d1e:	431f      	orrs	r7, r3
30005d20:	4b9f      	ldr	r3, [pc, #636]	; (30005fa0 <BOOT_Image1+0x2f0>)
30005d22:	8027      	strh	r7, [r4, #0]
30005d24:	4798      	blx	r3
30005d26:	88a3      	ldrh	r3, [r4, #4]
30005d28:	b29b      	uxth	r3, r3
30005d2a:	ea23 0307 	bic.w	r3, r3, r7
30005d2e:	80a3      	strh	r3, [r4, #4]
30005d30:	8863      	ldrh	r3, [r4, #2]
30005d32:	8067      	strh	r7, [r4, #2]
30005d34:	47a8      	blx	r5
30005d36:	2802      	cmp	r0, #2
30005d38:	f000 808b 	beq.w	30005e52 <BOOT_Image1+0x1a2>
30005d3c:	4b99      	ldr	r3, [pc, #612]	; (30005fa4 <BOOT_Image1+0x2f4>)
30005d3e:	2401      	movs	r4, #1
30005d40:	4999      	ldr	r1, [pc, #612]	; (30005fa8 <BOOT_Image1+0x2f8>)
30005d42:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005d46:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
30005d4a:	4622      	mov	r2, r4
30005d4c:	4f97      	ldr	r7, [pc, #604]	; (30005fac <BOOT_Image1+0x2fc>)
30005d4e:	47b8      	blx	r7
30005d50:	4622      	mov	r2, r4
30005d52:	4997      	ldr	r1, [pc, #604]	; (30005fb0 <BOOT_Image1+0x300>)
30005d54:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005d58:	47b8      	blx	r7
30005d5a:	4a8a      	ldr	r2, [pc, #552]	; (30005f84 <BOOT_Image1+0x2d4>)
30005d5c:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005d60:	f043 0303 	orr.w	r3, r3, #3
30005d64:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005d68:	4b92      	ldr	r3, [pc, #584]	; (30005fb4 <BOOT_Image1+0x304>)
30005d6a:	4798      	blx	r3
30005d6c:	4a85      	ldr	r2, [pc, #532]	; (30005f84 <BOOT_Image1+0x2d4>)
30005d6e:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30005d72:	f023 0303 	bic.w	r3, r3, #3
30005d76:	b108      	cbz	r0, 30005d7c <BOOT_Image1+0xcc>
30005d78:	f043 0302 	orr.w	r3, r3, #2
30005d7c:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30005d80:	4b8d      	ldr	r3, [pc, #564]	; (30005fb8 <BOOT_Image1+0x308>)
30005d82:	781b      	ldrb	r3, [r3, #0]
30005d84:	2b00      	cmp	r3, #0
30005d86:	d17f      	bne.n	30005e88 <BOOT_Image1+0x1d8>
30005d88:	4c8c      	ldr	r4, [pc, #560]	; (30005fbc <BOOT_Image1+0x30c>)
30005d8a:	2102      	movs	r1, #2
30005d8c:	2201      	movs	r2, #1
30005d8e:	488c      	ldr	r0, [pc, #560]	; (30005fc0 <BOOT_Image1+0x310>)
30005d90:	47a0      	blx	r4
30005d92:	4c8c      	ldr	r4, [pc, #560]	; (30005fc4 <BOOT_Image1+0x314>)
30005d94:	68a3      	ldr	r3, [r4, #8]
30005d96:	0799      	lsls	r1, r3, #30
30005d98:	d470      	bmi.n	30005e7c <BOOT_Image1+0x1cc>
30005d9a:	498b      	ldr	r1, [pc, #556]	; (30005fc8 <BOOT_Image1+0x318>)
30005d9c:	e841 f100 	tt	r1, r1
30005da0:	68a3      	ldr	r3, [r4, #8]
30005da2:	f3c1 5180 	ubfx	r1, r1, #22, #1
30005da6:	079a      	lsls	r2, r3, #30
30005da8:	f100 8088 	bmi.w	30005ebc <BOOT_Image1+0x20c>
30005dac:	4b87      	ldr	r3, [pc, #540]	; (30005fcc <BOOT_Image1+0x31c>)
30005dae:	4f88      	ldr	r7, [pc, #544]	; (30005fd0 <BOOT_Image1+0x320>)
30005db0:	681b      	ldr	r3, [r3, #0]
30005db2:	4798      	blx	r3
30005db4:	f7fe f956 	bl	30004064 <BOOT_RccConfig>
30005db8:	f7fe f9c0 	bl	3000413c <BOOT_CACHEWRR_Set>
30005dbc:	4b85      	ldr	r3, [pc, #532]	; (30005fd4 <BOOT_Image1+0x324>)
30005dbe:	6818      	ldr	r0, [r3, #0]
30005dc0:	f7fe f9ea 	bl	30004198 <BOOT_TCMSet>
30005dc4:	4b84      	ldr	r3, [pc, #528]	; (30005fd8 <BOOT_Image1+0x328>)
30005dc6:	695b      	ldr	r3, [r3, #20]
30005dc8:	03db      	lsls	r3, r3, #15
30005dca:	d512      	bpl.n	30005df2 <BOOT_Image1+0x142>
30005dcc:	f017 011f 	ands.w	r1, r7, #31
30005dd0:	d13b      	bne.n	30005e4a <BOOT_Image1+0x19a>
30005dd2:	463b      	mov	r3, r7
30005dd4:	2174      	movs	r1, #116	; 0x74
30005dd6:	f3bf 8f4f 	dsb	sy
30005dda:	487f      	ldr	r0, [pc, #508]	; (30005fd8 <BOOT_Image1+0x328>)
30005ddc:	4419      	add	r1, r3
30005dde:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30005de2:	3320      	adds	r3, #32
30005de4:	1aca      	subs	r2, r1, r3
30005de6:	2a00      	cmp	r2, #0
30005de8:	dcf9      	bgt.n	30005dde <BOOT_Image1+0x12e>
30005dea:	f3bf 8f4f 	dsb	sy
30005dee:	f3bf 8f6f 	isb	sy
30005df2:	4b6c      	ldr	r3, [pc, #432]	; (30005fa4 <BOOT_Image1+0x2f4>)
30005df4:	2200      	movs	r2, #0
30005df6:	f8df 818c 	ldr.w	r8, [pc, #396]	; 30005f84 <BOOT_Image1+0x2d4>
30005dfa:	f8c8 7270 	str.w	r7, [r8, #624]	; 0x270
30005dfe:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30005e02:	f7fd fc5d 	bl	300036c0 <flash_highspeed_setup>
30005e06:	47a8      	blx	r5
30005e08:	2802      	cmp	r0, #2
30005e0a:	f000 8170 	beq.w	300060ee <BOOT_Image1+0x43e>
30005e0e:	7833      	ldrb	r3, [r6, #0]
30005e10:	2b00      	cmp	r3, #0
30005e12:	d05e      	beq.n	30005ed2 <BOOT_Image1+0x222>
30005e14:	4c71      	ldr	r4, [pc, #452]	; (30005fdc <BOOT_Image1+0x32c>)
30005e16:	4e72      	ldr	r6, [pc, #456]	; (30005fe0 <BOOT_Image1+0x330>)
30005e18:	4d72      	ldr	r5, [pc, #456]	; (30005fe4 <BOOT_Image1+0x334>)
30005e1a:	4633      	mov	r3, r6
30005e1c:	462a      	mov	r2, r5
30005e1e:	4970      	ldr	r1, [pc, #448]	; (30005fe0 <BOOT_Image1+0x330>)
30005e20:	2002      	movs	r0, #2
30005e22:	f005 fbc7 	bl	3000b5b4 <rtk_log_write>
30005e26:	f241 3088 	movw	r0, #5000	; 0x1388
30005e2a:	47a0      	blx	r4
30005e2c:	e7f5      	b.n	30005e1a <BOOT_Image1+0x16a>
30005e2e:	4b5a      	ldr	r3, [pc, #360]	; (30005f98 <BOOT_Image1+0x2e8>)
30005e30:	2001      	movs	r0, #1
30005e32:	4798      	blx	r3
30005e34:	4620      	mov	r0, r4
30005e36:	4b6c      	ldr	r3, [pc, #432]	; (30005fe8 <BOOT_Image1+0x338>)
30005e38:	4798      	blx	r3
30005e3a:	4b6c      	ldr	r3, [pc, #432]	; (30005fec <BOOT_Image1+0x33c>)
30005e3c:	2001      	movs	r0, #1
30005e3e:	4798      	blx	r3
30005e40:	e764      	b.n	30005d0c <BOOT_Image1+0x5c>
30005e42:	486b      	ldr	r0, [pc, #428]	; (30005ff0 <BOOT_Image1+0x340>)
30005e44:	f005 fe54 	bl	3000baf0 <__DiagPrintf_veneer>
30005e48:	e73d      	b.n	30005cc6 <BOOT_Image1+0x16>
30005e4a:	3174      	adds	r1, #116	; 0x74
30005e4c:	f027 031f 	bic.w	r3, r7, #31
30005e50:	e7c1      	b.n	30005dd6 <BOOT_Image1+0x126>
30005e52:	6de3      	ldr	r3, [r4, #92]	; 0x5c
30005e54:	2201      	movs	r2, #1
30005e56:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005e5a:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30005e5e:	65e3      	str	r3, [r4, #92]	; 0x5c
30005e60:	4b50      	ldr	r3, [pc, #320]	; (30005fa4 <BOOT_Image1+0x2f4>)
30005e62:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30005e66:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005e6a:	4b50      	ldr	r3, [pc, #320]	; (30005fac <BOOT_Image1+0x2fc>)
30005e6c:	4798      	blx	r3
30005e6e:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
30005e72:	f043 0302 	orr.w	r3, r3, #2
30005e76:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
30005e7a:	e775      	b.n	30005d68 <BOOT_Image1+0xb8>
30005e7c:	f3ef 8108 	mrs	r1, MSP
30005e80:	485c      	ldr	r0, [pc, #368]	; (30005ff4 <BOOT_Image1+0x344>)
30005e82:	f005 fe35 	bl	3000baf0 <__DiagPrintf_veneer>
30005e86:	e788      	b.n	30005d9a <BOOT_Image1+0xea>
30005e88:	4f5b      	ldr	r7, [pc, #364]	; (30005ff8 <BOOT_Image1+0x348>)
30005e8a:	47b8      	blx	r7
30005e8c:	4604      	mov	r4, r0
30005e8e:	47b8      	blx	r7
30005e90:	2c02      	cmp	r4, #2
30005e92:	f43f af79 	beq.w	30005d88 <BOOT_Image1+0xd8>
30005e96:	2803      	cmp	r0, #3
30005e98:	f43f af76 	beq.w	30005d88 <BOOT_Image1+0xd8>
30005e9c:	4b57      	ldr	r3, [pc, #348]	; (30005ffc <BOOT_Image1+0x34c>)
30005e9e:	4798      	blx	r3
30005ea0:	4c46      	ldr	r4, [pc, #280]	; (30005fbc <BOOT_Image1+0x30c>)
30005ea2:	2200      	movs	r2, #0
30005ea4:	2101      	movs	r1, #1
30005ea6:	4846      	ldr	r0, [pc, #280]	; (30005fc0 <BOOT_Image1+0x310>)
30005ea8:	47a0      	blx	r4
30005eaa:	2101      	movs	r1, #1
30005eac:	4844      	ldr	r0, [pc, #272]	; (30005fc0 <BOOT_Image1+0x310>)
30005eae:	4b54      	ldr	r3, [pc, #336]	; (30006000 <BOOT_Image1+0x350>)
30005eb0:	4798      	blx	r3
30005eb2:	2201      	movs	r2, #1
30005eb4:	4842      	ldr	r0, [pc, #264]	; (30005fc0 <BOOT_Image1+0x310>)
30005eb6:	4611      	mov	r1, r2
30005eb8:	47a0      	blx	r4
30005eba:	e766      	b.n	30005d8a <BOOT_Image1+0xda>
30005ebc:	4851      	ldr	r0, [pc, #324]	; (30006004 <BOOT_Image1+0x354>)
30005ebe:	f005 fe17 	bl	3000baf0 <__DiagPrintf_veneer>
30005ec2:	e773      	b.n	30005dac <BOOT_Image1+0xfc>
30005ec4:	4601      	mov	r1, r0
30005ec6:	f44f 7201 	mov.w	r2, #516	; 0x204
30005eca:	4836      	ldr	r0, [pc, #216]	; (30005fa4 <BOOT_Image1+0x2f4>)
30005ecc:	f005 fe78 	bl	3000bbc0 <____wrap_memset_veneer>
30005ed0:	e70f      	b.n	30005cf2 <BOOT_Image1+0x42>
30005ed2:	4b43      	ldr	r3, [pc, #268]	; (30005fe0 <BOOT_Image1+0x330>)
30005ed4:	2004      	movs	r0, #4
30005ed6:	4a4c      	ldr	r2, [pc, #304]	; (30006008 <BOOT_Image1+0x358>)
30005ed8:	4619      	mov	r1, r3
30005eda:	f005 fb6b 	bl	3000b5b4 <rtk_log_write>
30005ede:	f7fe faf3 	bl	300044c8 <BOOT_DDR_Init>
30005ee2:	f8d8 3290 	ldr.w	r3, [r8, #656]	; 0x290
30005ee6:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005eea:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005eee:	f8c8 3290 	str.w	r3, [r8, #656]	; 0x290
30005ef2:	2200      	movs	r2, #0
30005ef4:	4b23      	ldr	r3, [pc, #140]	; (30005f84 <BOOT_Image1+0x2d4>)
30005ef6:	4d45      	ldr	r5, [pc, #276]	; (3000600c <BOOT_Image1+0x35c>)
30005ef8:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
30005efc:	f8c3 22d4 	str.w	r2, [r3, #724]	; 0x2d4
30005f00:	f8c3 22d8 	str.w	r2, [r3, #728]	; 0x2d8
30005f04:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
30005f08:	f7ff fe94 	bl	30005c34 <BOOT_Share_Memory_Patch>
30005f0c:	4b40      	ldr	r3, [pc, #256]	; (30006010 <BOOT_Image1+0x360>)
30005f0e:	2002      	movs	r0, #2
30005f10:	4798      	blx	r3
30005f12:	4b33      	ldr	r3, [pc, #204]	; (30005fe0 <BOOT_Image1+0x330>)
30005f14:	9000      	str	r0, [sp, #0]
30005f16:	2004      	movs	r0, #4
30005f18:	4619      	mov	r1, r3
30005f1a:	4a3e      	ldr	r2, [pc, #248]	; (30006014 <BOOT_Image1+0x364>)
30005f1c:	f005 fb4a 	bl	3000b5b4 <rtk_log_write>
30005f20:	782b      	ldrb	r3, [r5, #0]
30005f22:	2b00      	cmp	r3, #0
30005f24:	f040 8107 	bne.w	30006136 <BOOT_Image1+0x486>
30005f28:	6823      	ldr	r3, [r4, #0]
30005f2a:	4e3b      	ldr	r6, [pc, #236]	; (30006018 <BOOT_Image1+0x368>)
30005f2c:	f043 0302 	orr.w	r3, r3, #2
30005f30:	6023      	str	r3, [r4, #0]
30005f32:	47b0      	blx	r6
30005f34:	b190      	cbz	r0, 30005f5c <BOOT_Image1+0x2ac>
30005f36:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30005f3a:	4838      	ldr	r0, [pc, #224]	; (3000601c <BOOT_Image1+0x36c>)
30005f3c:	f005 fdd8 	bl	3000baf0 <__DiagPrintf_veneer>
30005f40:	4837      	ldr	r0, [pc, #220]	; (30006020 <BOOT_Image1+0x370>)
30005f42:	f005 fdd5 	bl	3000baf0 <__DiagPrintf_veneer>
30005f46:	f005 fb71 	bl	3000b62c <BOOT_VerCheck>
30005f4a:	f7fe fcb1 	bl	300048b0 <BOOT_LoadImages>
30005f4e:	2800      	cmp	r0, #0
30005f50:	d16a      	bne.n	30006028 <BOOT_Image1+0x378>
30005f52:	4c22      	ldr	r4, [pc, #136]	; (30005fdc <BOOT_Image1+0x32c>)
30005f54:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005f58:	47a0      	blx	r4
30005f5a:	e7fb      	b.n	30005f54 <BOOT_Image1+0x2a4>
30005f5c:	4931      	ldr	r1, [pc, #196]	; (30006024 <BOOT_Image1+0x374>)
30005f5e:	482f      	ldr	r0, [pc, #188]	; (3000601c <BOOT_Image1+0x36c>)
30005f60:	f005 fdc6 	bl	3000baf0 <__DiagPrintf_veneer>
30005f64:	e7ec      	b.n	30005f40 <BOOT_Image1+0x290>
30005f66:	bf00      	nop
30005f68:	0000c149 	.word	0x0000c149
30005f6c:	3000c618 	.word	0x3000c618
30005f70:	3000dcb4 	.word	0x3000dcb4
30005f74:	3000c63c 	.word	0x3000c63c
30005f78:	3000f390 	.word	0x3000f390
30005f7c:	3000f83c 	.word	0x3000f83c
30005f80:	00012be5 	.word	0x00012be5
30005f84:	42008000 	.word	0x42008000
30005f88:	42008200 	.word	0x42008200
30005f8c:	3000df9e 	.word	0x3000df9e
30005f90:	3000aa0d 	.word	0x3000aa0d
30005f94:	3000adc5 	.word	0x3000adc5
30005f98:	3000ace5 	.word	0x3000ace5
30005f9c:	3000746d 	.word	0x3000746d
30005fa0:	00009b2d 	.word	0x00009b2d
30005fa4:	23020000 	.word	0x23020000
30005fa8:	40080000 	.word	0x40080000
30005fac:	0000b479 	.word	0x0000b479
30005fb0:	40040000 	.word	0x40040000
30005fb4:	300092c5 	.word	0x300092c5
30005fb8:	3000df9d 	.word	0x3000df9d
30005fbc:	0000aab9 	.word	0x0000aab9
30005fc0:	4200c000 	.word	0x4200c000
30005fc4:	2001c00c 	.word	0x2001c00c
30005fc8:	30005cb1 	.word	0x30005cb1
30005fcc:	00033000 	.word	0x00033000
30005fd0:	2001c01c 	.word	0x2001c01c
30005fd4:	3000dfa0 	.word	0x3000dfa0
30005fd8:	e000ed00 	.word	0xe000ed00
30005fdc:	00009be5 	.word	0x00009be5
30005fe0:	3000bd58 	.word	0x3000bd58
30005fe4:	3000c6e0 	.word	0x3000c6e0
30005fe8:	3000af15 	.word	0x3000af15
30005fec:	3000ae85 	.word	0x3000ae85
30005ff0:	3000c628 	.word	0x3000c628
30005ff4:	3000c654 	.word	0x3000c654
30005ff8:	0000c0f9 	.word	0x0000c0f9
30005ffc:	0000aea1 	.word	0x0000aea1
30006000:	0000aaa5 	.word	0x0000aaa5
30006004:	3000c684 	.word	0x3000c684
30006008:	3000c720 	.word	0x3000c720
3000600c:	3000df9c 	.word	0x3000df9c
30006010:	00009979 	.word	0x00009979
30006014:	3000c734 	.word	0x3000c734
30006018:	0000c169 	.word	0x0000c169
3000601c:	3000c750 	.word	0x3000c750
30006020:	3000c768 	.word	0x3000c768
30006024:	0016e360 	.word	0x0016e360
30006028:	f7ff fb7c 	bl	30005724 <BOOT_Enable_KM0>
3000602c:	2200      	movs	r2, #0
3000602e:	2101      	movs	r1, #1
30006030:	484e      	ldr	r0, [pc, #312]	; (3000616c <BOOT_Image1+0x4bc>)
30006032:	4b4f      	ldr	r3, [pc, #316]	; (30006170 <BOOT_Image1+0x4c0>)
30006034:	4798      	blx	r3
30006036:	4b4f      	ldr	r3, [pc, #316]	; (30006174 <BOOT_Image1+0x4c4>)
30006038:	2008      	movs	r0, #8
3000603a:	4798      	blx	r3
3000603c:	f001 f906 	bl	3000724c <BOOT_RAM_TZCfg>
30006040:	782b      	ldrb	r3, [r5, #0]
30006042:	2b01      	cmp	r3, #1
30006044:	f000 8081 	beq.w	3000614a <BOOT_Image1+0x49a>
30006048:	4b4b      	ldr	r3, [pc, #300]	; (30006178 <BOOT_Image1+0x4c8>)
3000604a:	781b      	ldrb	r3, [r3, #0]
3000604c:	2b00      	cmp	r3, #0
3000604e:	d168      	bne.n	30006122 <BOOT_Image1+0x472>
30006050:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006054:	4949      	ldr	r1, [pc, #292]	; (3000617c <BOOT_Image1+0x4cc>)
30006056:	685a      	ldr	r2, [r3, #4]
30006058:	f042 0202 	orr.w	r2, r2, #2
3000605c:	605a      	str	r2, [r3, #4]
3000605e:	685a      	ldr	r2, [r3, #4]
30006060:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006064:	605a      	str	r2, [r3, #4]
30006066:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006068:	f043 0303 	orr.w	r3, r3, #3
3000606c:	678b      	str	r3, [r1, #120]	; 0x78
3000606e:	782b      	ldrb	r3, [r5, #0]
30006070:	2b01      	cmp	r3, #1
30006072:	d072      	beq.n	3000615a <BOOT_Image1+0x4aa>
30006074:	4842      	ldr	r0, [pc, #264]	; (30006180 <BOOT_Image1+0x4d0>)
30006076:	4943      	ldr	r1, [pc, #268]	; (30006184 <BOOT_Image1+0x4d4>)
30006078:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
3000607c:	4a42      	ldr	r2, [pc, #264]	; (30006188 <BOOT_Image1+0x4d8>)
3000607e:	f043 0301 	orr.w	r3, r3, #1
30006082:	4d42      	ldr	r5, [pc, #264]	; (3000618c <BOOT_Image1+0x4dc>)
30006084:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
30006088:	688c      	ldr	r4, [r1, #8]
3000608a:	680b      	ldr	r3, [r1, #0]
3000608c:	4940      	ldr	r1, [pc, #256]	; (30006190 <BOOT_Image1+0x4e0>)
3000608e:	6063      	str	r3, [r4, #4]
30006090:	4b40      	ldr	r3, [pc, #256]	; (30006194 <BOOT_Image1+0x4e4>)
30006092:	6094      	str	r4, [r2, #8]
30006094:	61e9      	str	r1, [r5, #28]
30006096:	6a59      	ldr	r1, [r3, #36]	; 0x24
30006098:	483f      	ldr	r0, [pc, #252]	; (30006198 <BOOT_Image1+0x4e8>)
3000609a:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000609e:	6259      	str	r1, [r3, #36]	; 0x24
300060a0:	6a51      	ldr	r1, [r2, #36]	; 0x24
300060a2:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
300060a6:	6251      	str	r1, [r2, #36]	; 0x24
300060a8:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
300060ac:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300060b0:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
300060b4:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
300060b8:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300060bc:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300060c0:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
300060c4:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
300060c8:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
300060cc:	689b      	ldr	r3, [r3, #8]
300060ce:	6183      	str	r3, [r0, #24]
300060d0:	f7fe fb76 	bl	300047c0 <BOOT_NVICBackup_HP>
300060d4:	4b31      	ldr	r3, [pc, #196]	; (3000619c <BOOT_Image1+0x4ec>)
300060d6:	f383 8888 	msr	MSP_NS, r3
300060da:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
300060de:	f383 8809 	msr	PSP, r3
300060e2:	6860      	ldr	r0, [r4, #4]
300060e4:	b002      	add	sp, #8
300060e6:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300060ea:	f7fd bf6b 	b.w	30003fc4 <BOOT_NsStart>
300060ee:	4b2c      	ldr	r3, [pc, #176]	; (300061a0 <BOOT_Image1+0x4f0>)
300060f0:	2004      	movs	r0, #4
300060f2:	4a2c      	ldr	r2, [pc, #176]	; (300061a4 <BOOT_Image1+0x4f4>)
300060f4:	4619      	mov	r1, r3
300060f6:	f005 fa5d 	bl	3000b5b4 <rtk_log_write>
300060fa:	4b2b      	ldr	r3, [pc, #172]	; (300061a8 <BOOT_Image1+0x4f8>)
300060fc:	4798      	blx	r3
300060fe:	f7fe fa21 	bl	30004544 <BOOT_PSRAM_Init>
30006102:	7830      	ldrb	r0, [r6, #0]
30006104:	2800      	cmp	r0, #0
30006106:	f47f aef4 	bne.w	30005ef2 <BOOT_Image1+0x242>
3000610a:	4b28      	ldr	r3, [pc, #160]	; (300061ac <BOOT_Image1+0x4fc>)
3000610c:	4798      	blx	r3
3000610e:	2800      	cmp	r0, #0
30006110:	f43f aeef 	beq.w	30005ef2 <BOOT_Image1+0x242>
30006114:	4b22      	ldr	r3, [pc, #136]	; (300061a0 <BOOT_Image1+0x4f0>)
30006116:	2003      	movs	r0, #3
30006118:	4a25      	ldr	r2, [pc, #148]	; (300061b0 <BOOT_Image1+0x500>)
3000611a:	4619      	mov	r1, r3
3000611c:	f005 fa4a 	bl	3000b5b4 <rtk_log_write>
30006120:	e6e7      	b.n	30005ef2 <BOOT_Image1+0x242>
30006122:	f7ff fc2f 	bl	30005984 <BOOT_Enable_AP>
30006126:	4a16      	ldr	r2, [pc, #88]	; (30006180 <BOOT_Image1+0x4d0>)
30006128:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
3000612c:	f043 0306 	orr.w	r3, r3, #6
30006130:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
30006134:	e79b      	b.n	3000606e <BOOT_Image1+0x3be>
30006136:	4b1f      	ldr	r3, [pc, #124]	; (300061b4 <BOOT_Image1+0x504>)
30006138:	4798      	blx	r3
3000613a:	2800      	cmp	r0, #0
3000613c:	f47f aef4 	bne.w	30005f28 <BOOT_Image1+0x278>
30006140:	68a3      	ldr	r3, [r4, #8]
30006142:	f043 0302 	orr.w	r3, r3, #2
30006146:	60a3      	str	r3, [r4, #8]
30006148:	e6ee      	b.n	30005f28 <BOOT_Image1+0x278>
3000614a:	47b0      	blx	r6
3000614c:	2801      	cmp	r0, #1
3000614e:	4b1a      	ldr	r3, [pc, #104]	; (300061b8 <BOOT_Image1+0x508>)
30006150:	bf0c      	ite	eq
30006152:	2019      	moveq	r0, #25
30006154:	2004      	movne	r0, #4
30006156:	4798      	blx	r3
30006158:	e776      	b.n	30006048 <BOOT_Image1+0x398>
3000615a:	47b0      	blx	r6
3000615c:	2801      	cmp	r0, #1
3000615e:	4b16      	ldr	r3, [pc, #88]	; (300061b8 <BOOT_Image1+0x508>)
30006160:	bf0c      	ite	eq
30006162:	20e6      	moveq	r0, #230	; 0xe6
30006164:	2019      	movne	r0, #25
30006166:	4798      	blx	r3
30006168:	e784      	b.n	30006074 <BOOT_Image1+0x3c4>
3000616a:	bf00      	nop
3000616c:	4200c000 	.word	0x4200c000
30006170:	0000ac6d 	.word	0x0000ac6d
30006174:	0000e1d9 	.word	0x0000e1d9
30006178:	3000df9f 	.word	0x3000df9f
3000617c:	41000200 	.word	0x41000200
30006180:	42008200 	.word	0x42008200
30006184:	60000020 	.word	0x60000020
30006188:	e002ed00 	.word	0xe002ed00
3000618c:	30000000 	.word	0x30000000
30006190:	30003a81 	.word	0x30003a81
30006194:	e000ed00 	.word	0xe000ed00
30006198:	3000f43c 	.word	0x3000f43c
3000619c:	2001bffc 	.word	0x2001bffc
300061a0:	3000bd58 	.word	0x3000bd58
300061a4:	3000c6b4 	.word	0x3000c6b4
300061a8:	300082ed 	.word	0x300082ed
300061ac:	3000ade5 	.word	0x3000ade5
300061b0:	3000c6c8 	.word	0x3000c6c8
300061b4:	0000c179 	.word	0x0000c179
300061b8:	00009be5 	.word	0x00009be5

300061bc <BOOT_LoadSubImage>:
300061bc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300061c0:	b08f      	sub	sp, #60	; 0x3c
300061c2:	469a      	mov	sl, r3
300061c4:	460f      	mov	r7, r1
300061c6:	f89d 3060 	ldrb.w	r3, [sp, #96]	; 0x60
300061ca:	9305      	str	r3, [sp, #20]
300061cc:	b39a      	cbz	r2, 30006236 <BOOT_LoadSubImage+0x7a>
300061ce:	4691      	mov	r9, r2
300061d0:	2600      	movs	r6, #0
300061d2:	4605      	mov	r5, r0
300061d4:	2220      	movs	r2, #32
300061d6:	4639      	mov	r1, r7
300061d8:	a806      	add	r0, sp, #24
300061da:	f7fe fb4f 	bl	3000487c <BOOT_ImgCopy>
300061de:	9b06      	ldr	r3, [sp, #24]
300061e0:	4a49      	ldr	r2, [pc, #292]	; (30006308 <BOOT_LoadSubImage+0x14c>)
300061e2:	4293      	cmp	r3, r2
300061e4:	d157      	bne.n	30006296 <BOOT_LoadSubImage+0xda>
300061e6:	f102 427b 	add.w	r2, r2, #4211081216	; 0xfb000000
300061ea:	9b07      	ldr	r3, [sp, #28]
300061ec:	f502 0278 	add.w	r2, r2, #16252928	; 0xf80000
300061f0:	f502 62c0 	add.w	r2, r2, #1536	; 0x600
300061f4:	4293      	cmp	r3, r2
300061f6:	d14e      	bne.n	30006296 <BOOT_LoadSubImage+0xda>
300061f8:	4b44      	ldr	r3, [pc, #272]	; (3000630c <BOOT_LoadSubImage+0x150>)
300061fa:	e9dd 4008 	ldrd	r4, r0, [sp, #32]
300061fe:	18c3      	adds	r3, r0, r3
30006200:	f1a0 0820 	sub.w	r8, r0, #32
30006204:	3420      	adds	r4, #32
30006206:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000620a:	d21a      	bcs.n	30006242 <BOOT_LoadSubImage+0x86>
3000620c:	b17d      	cbz	r5, 3000622e <BOOT_LoadSubImage+0x72>
3000620e:	1d2b      	adds	r3, r5, #4
30006210:	f845 8036 	str.w	r8, [r5, r6, lsl #3]
30006214:	4a3e      	ldr	r2, [pc, #248]	; (30006310 <BOOT_LoadSubImage+0x154>)
30006216:	2004      	movs	r0, #4
30006218:	e9cd 8401 	strd	r8, r4, [sp, #4]
3000621c:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
30006220:	f85a 3026 	ldr.w	r3, [sl, r6, lsl #2]
30006224:	9300      	str	r3, [sp, #0]
30006226:	4b3b      	ldr	r3, [pc, #236]	; (30006314 <BOOT_LoadSubImage+0x158>)
30006228:	4619      	mov	r1, r3
3000622a:	f005 f9c3 	bl	3000b5b4 <rtk_log_write>
3000622e:	3601      	adds	r6, #1
30006230:	4427      	add	r7, r4
30006232:	454e      	cmp	r6, r9
30006234:	d1ce      	bne.n	300061d4 <BOOT_LoadSubImage+0x18>
30006236:	4b38      	ldr	r3, [pc, #224]	; (30006318 <BOOT_LoadSubImage+0x15c>)
30006238:	2001      	movs	r0, #1
3000623a:	601f      	str	r7, [r3, #0]
3000623c:	b00f      	add	sp, #60	; 0x3c
3000623e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006242:	4639      	mov	r1, r7
30006244:	4622      	mov	r2, r4
30006246:	4640      	mov	r0, r8
30006248:	f7fe fb18 	bl	3000487c <BOOT_ImgCopy>
3000624c:	4933      	ldr	r1, [pc, #204]	; (3000631c <BOOT_LoadSubImage+0x160>)
3000624e:	694b      	ldr	r3, [r1, #20]
30006250:	03db      	lsls	r3, r3, #15
30006252:	d5db      	bpl.n	3000620c <BOOT_LoadSubImage+0x50>
30006254:	ea08 0304 	and.w	r3, r8, r4
30006258:	3301      	adds	r3, #1
3000625a:	d023      	beq.n	300062a4 <BOOT_LoadSubImage+0xe8>
3000625c:	f018 031f 	ands.w	r3, r8, #31
30006260:	d042      	beq.n	300062e8 <BOOT_LoadSubImage+0x12c>
30006262:	4423      	add	r3, r4
30006264:	f028 021f 	bic.w	r2, r8, #31
30006268:	f3bf 8f4f 	dsb	sy
3000626c:	2b00      	cmp	r3, #0
3000626e:	dd0d      	ble.n	3000628c <BOOT_LoadSubImage+0xd0>
30006270:	1e59      	subs	r1, r3, #1
30006272:	f102 0320 	add.w	r3, r2, #32
30006276:	4829      	ldr	r0, [pc, #164]	; (3000631c <BOOT_LoadSubImage+0x160>)
30006278:	f021 011f 	bic.w	r1, r1, #31
3000627c:	4419      	add	r1, r3
3000627e:	428b      	cmp	r3, r1
30006280:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30006284:	461a      	mov	r2, r3
30006286:	f103 0320 	add.w	r3, r3, #32
3000628a:	d1f8      	bne.n	3000627e <BOOT_LoadSubImage+0xc2>
3000628c:	f3bf 8f4f 	dsb	sy
30006290:	f3bf 8f6f 	isb	sy
30006294:	e7ba      	b.n	3000620c <BOOT_LoadSubImage+0x50>
30006296:	9b05      	ldr	r3, [sp, #20]
30006298:	2b01      	cmp	r3, #1
3000629a:	d028      	beq.n	300062ee <BOOT_LoadSubImage+0x132>
3000629c:	2000      	movs	r0, #0
3000629e:	b00f      	add	sp, #60	; 0x3c
300062a0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300062a4:	2300      	movs	r3, #0
300062a6:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300062aa:	f3bf 8f4f 	dsb	sy
300062ae:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
300062b2:	f643 7be0 	movw	fp, #16352	; 0x3fe0
300062b6:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
300062ba:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
300062be:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
300062c2:	ea0c 000b 	and.w	r0, ip, fp
300062c6:	4673      	mov	r3, lr
300062c8:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
300062cc:	3b01      	subs	r3, #1
300062ce:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300062d2:	d2f9      	bcs.n	300062c8 <BOOT_LoadSubImage+0x10c>
300062d4:	f1ac 0c20 	sub.w	ip, ip, #32
300062d8:	f11c 0f20 	cmn.w	ip, #32
300062dc:	d1f1      	bne.n	300062c2 <BOOT_LoadSubImage+0x106>
300062de:	f3bf 8f4f 	dsb	sy
300062e2:	f3bf 8f6f 	isb	sy
300062e6:	e791      	b.n	3000620c <BOOT_LoadSubImage+0x50>
300062e8:	4623      	mov	r3, r4
300062ea:	4642      	mov	r2, r8
300062ec:	e7bc      	b.n	30006268 <BOOT_LoadSubImage+0xac>
300062ee:	f85a 4026 	ldr.w	r4, [sl, r6, lsl #2]
300062f2:	2004      	movs	r0, #4
300062f4:	4b07      	ldr	r3, [pc, #28]	; (30006314 <BOOT_LoadSubImage+0x158>)
300062f6:	4a0a      	ldr	r2, [pc, #40]	; (30006320 <BOOT_LoadSubImage+0x164>)
300062f8:	4619      	mov	r1, r3
300062fa:	9400      	str	r4, [sp, #0]
300062fc:	f005 f95a 	bl	3000b5b4 <rtk_log_write>
30006300:	2000      	movs	r0, #0
30006302:	b00f      	add	sp, #60	; 0x3c
30006304:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006308:	35393138 	.word	0x35393138
3000630c:	f7ffffe0 	.word	0xf7ffffe0
30006310:	3000c830 	.word	0x3000c830
30006314:	3000bd58 	.word	0x3000bd58
30006318:	3000e56c 	.word	0x3000e56c
3000631c:	e000ed00 	.word	0xe000ed00
30006320:	3000c81c 	.word	0x3000c81c

30006324 <BOOT_LoadAPSubImage>:
30006324:	4bb5      	ldr	r3, [pc, #724]	; (300065fc <BOOT_LoadAPSubImage+0x2d8>)
30006326:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000632a:	460c      	mov	r4, r1
3000632c:	4606      	mov	r6, r0
3000632e:	b08f      	sub	sp, #60	; 0x3c
30006330:	4615      	mov	r5, r2
30006332:	2220      	movs	r2, #32
30006334:	e893 0003 	ldmia.w	r3, {r0, r1}
30006338:	0c0b      	lsrs	r3, r1, #16
3000633a:	9004      	str	r0, [sp, #16]
3000633c:	f8ad 1014 	strh.w	r1, [sp, #20]
30006340:	a806      	add	r0, sp, #24
30006342:	4621      	mov	r1, r4
30006344:	f88d 3016 	strb.w	r3, [sp, #22]
30006348:	f7fe fa98 	bl	3000487c <BOOT_ImgCopy>
3000634c:	4bac      	ldr	r3, [pc, #688]	; (30006600 <BOOT_LoadAPSubImage+0x2dc>)
3000634e:	9a06      	ldr	r2, [sp, #24]
30006350:	429a      	cmp	r2, r3
30006352:	d108      	bne.n	30006366 <BOOT_LoadAPSubImage+0x42>
30006354:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30006358:	9a07      	ldr	r2, [sp, #28]
3000635a:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
3000635e:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30006362:	429a      	cmp	r2, r3
30006364:	d006      	beq.n	30006374 <BOOT_LoadAPSubImage+0x50>
30006366:	2d01      	cmp	r5, #1
30006368:	f000 80b3 	beq.w	300064d2 <BOOT_LoadAPSubImage+0x1ae>
3000636c:	2000      	movs	r0, #0
3000636e:	b00f      	add	sp, #60	; 0x3c
30006370:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006374:	9a09      	ldr	r2, [sp, #36]	; 0x24
30006376:	4ba3      	ldr	r3, [pc, #652]	; (30006604 <BOOT_LoadAPSubImage+0x2e0>)
30006378:	6ca7      	ldr	r7, [r4, #72]	; 0x48
3000637a:	f1a2 0820 	sub.w	r8, r2, #32
3000637e:	4413      	add	r3, r2
30006380:	37d0      	adds	r7, #208	; 0xd0
30006382:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006386:	d247      	bcs.n	30006418 <BOOT_LoadAPSubImage+0xf4>
30006388:	4b9f      	ldr	r3, [pc, #636]	; (30006608 <BOOT_LoadAPSubImage+0x2e4>)
3000638a:	eb07 0508 	add.w	r5, r7, r8
3000638e:	f8d4 8070 	ldr.w	r8, [r4, #112]	; 0x70
30006392:	4798      	blx	r3
30006394:	2801      	cmp	r0, #1
30006396:	4427      	add	r7, r4
30006398:	4b9c      	ldr	r3, [pc, #624]	; (3000660c <BOOT_LoadAPSubImage+0x2e8>)
3000639a:	d029      	beq.n	300063f0 <BOOT_LoadAPSubImage+0xcc>
3000639c:	4619      	mov	r1, r3
3000639e:	4a9c      	ldr	r2, [pc, #624]	; (30006610 <BOOT_LoadAPSubImage+0x2ec>)
300063a0:	2004      	movs	r0, #4
300063a2:	f005 f907 	bl	3000b5b4 <rtk_log_write>
300063a6:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
300063aa:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300063ae:	f080 809c 	bcs.w	300064ea <BOOT_LoadAPSubImage+0x1c6>
300063b2:	4445      	add	r5, r8
300063b4:	eb08 0107 	add.w	r1, r8, r7
300063b8:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
300063bc:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300063c0:	d256      	bcs.n	30006470 <BOOT_LoadAPSubImage+0x14c>
300063c2:	2e00      	cmp	r6, #0
300063c4:	f000 8081 	beq.w	300064ca <BOOT_LoadAPSubImage+0x1a6>
300063c8:	4b90      	ldr	r3, [pc, #576]	; (3000660c <BOOT_LoadAPSubImage+0x2e8>)
300063ca:	aa04      	add	r2, sp, #16
300063cc:	2004      	movs	r0, #4
300063ce:	9200      	str	r2, [sp, #0]
300063d0:	4a90      	ldr	r2, [pc, #576]	; (30006614 <BOOT_LoadAPSubImage+0x2f0>)
300063d2:	e9dd 1408 	ldrd	r1, r4, [sp, #32]
300063d6:	3c20      	subs	r4, #32
300063d8:	3120      	adds	r1, #32
300063da:	e9c6 4100 	strd	r4, r1, [r6]
300063de:	9102      	str	r1, [sp, #8]
300063e0:	4619      	mov	r1, r3
300063e2:	9401      	str	r4, [sp, #4]
300063e4:	f005 f8e6 	bl	3000b5b4 <rtk_log_write>
300063e8:	2001      	movs	r0, #1
300063ea:	b00f      	add	sp, #60	; 0x3c
300063ec:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300063f0:	4a89      	ldr	r2, [pc, #548]	; (30006618 <BOOT_LoadAPSubImage+0x2f4>)
300063f2:	2004      	movs	r0, #4
300063f4:	4619      	mov	r1, r3
300063f6:	f005 f8dd 	bl	3000b5b4 <rtk_log_write>
300063fa:	4642      	mov	r2, r8
300063fc:	4629      	mov	r1, r5
300063fe:	4638      	mov	r0, r7
30006400:	f000 fbe4 	bl	30006bcc <BOOT_LoadBL32RDPImg>
30006404:	2800      	cmp	r0, #0
30006406:	d1d4      	bne.n	300063b2 <BOOT_LoadAPSubImage+0x8e>
30006408:	4b80      	ldr	r3, [pc, #512]	; (3000660c <BOOT_LoadAPSubImage+0x2e8>)
3000640a:	2004      	movs	r0, #4
3000640c:	4a83      	ldr	r2, [pc, #524]	; (3000661c <BOOT_LoadAPSubImage+0x2f8>)
3000640e:	4619      	mov	r1, r3
30006410:	f005 f8d0 	bl	3000b5b4 <rtk_log_write>
30006414:	20ff      	movs	r0, #255	; 0xff
30006416:	e7aa      	b.n	3000636e <BOOT_LoadAPSubImage+0x4a>
30006418:	463a      	mov	r2, r7
3000641a:	4621      	mov	r1, r4
3000641c:	4640      	mov	r0, r8
3000641e:	f7fe fa2d 	bl	3000487c <BOOT_ImgCopy>
30006422:	497f      	ldr	r1, [pc, #508]	; (30006620 <BOOT_LoadAPSubImage+0x2fc>)
30006424:	694b      	ldr	r3, [r1, #20]
30006426:	03da      	lsls	r2, r3, #15
30006428:	d5ae      	bpl.n	30006388 <BOOT_LoadAPSubImage+0x64>
3000642a:	ea07 0308 	and.w	r3, r7, r8
3000642e:	3301      	adds	r3, #1
30006430:	f000 8086 	beq.w	30006540 <BOOT_LoadAPSubImage+0x21c>
30006434:	f018 031f 	ands.w	r3, r8, #31
30006438:	f000 80bb 	beq.w	300065b2 <BOOT_LoadAPSubImage+0x28e>
3000643c:	443b      	add	r3, r7
3000643e:	f028 021f 	bic.w	r2, r8, #31
30006442:	f3bf 8f4f 	dsb	sy
30006446:	2b00      	cmp	r3, #0
30006448:	dd0d      	ble.n	30006466 <BOOT_LoadAPSubImage+0x142>
3000644a:	1e59      	subs	r1, r3, #1
3000644c:	f102 0320 	add.w	r3, r2, #32
30006450:	4873      	ldr	r0, [pc, #460]	; (30006620 <BOOT_LoadAPSubImage+0x2fc>)
30006452:	f021 011f 	bic.w	r1, r1, #31
30006456:	4419      	add	r1, r3
30006458:	428b      	cmp	r3, r1
3000645a:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
3000645e:	461a      	mov	r2, r3
30006460:	f103 0320 	add.w	r3, r3, #32
30006464:	d1f8      	bne.n	30006458 <BOOT_LoadAPSubImage+0x134>
30006466:	f3bf 8f4f 	dsb	sy
3000646a:	f3bf 8f6f 	isb	sy
3000646e:	e78b      	b.n	30006388 <BOOT_LoadAPSubImage+0x64>
30006470:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30006474:	4628      	mov	r0, r5
30006476:	4622      	mov	r2, r4
30006478:	f7fe fa00 	bl	3000487c <BOOT_ImgCopy>
3000647c:	4968      	ldr	r1, [pc, #416]	; (30006620 <BOOT_LoadAPSubImage+0x2fc>)
3000647e:	694b      	ldr	r3, [r1, #20]
30006480:	03d8      	lsls	r0, r3, #15
30006482:	d59e      	bpl.n	300063c2 <BOOT_LoadAPSubImage+0x9e>
30006484:	ea05 0304 	and.w	r3, r5, r4
30006488:	3301      	adds	r3, #1
3000648a:	d075      	beq.n	30006578 <BOOT_LoadAPSubImage+0x254>
3000648c:	f015 031f 	ands.w	r3, r5, #31
30006490:	d002      	beq.n	30006498 <BOOT_LoadAPSubImage+0x174>
30006492:	f025 051f 	bic.w	r5, r5, #31
30006496:	441c      	add	r4, r3
30006498:	f3bf 8f4f 	dsb	sy
3000649c:	2c00      	cmp	r4, #0
3000649e:	dd0d      	ble.n	300064bc <BOOT_LoadAPSubImage+0x198>
300064a0:	1e62      	subs	r2, r4, #1
300064a2:	f105 0320 	add.w	r3, r5, #32
300064a6:	495e      	ldr	r1, [pc, #376]	; (30006620 <BOOT_LoadAPSubImage+0x2fc>)
300064a8:	f022 021f 	bic.w	r2, r2, #31
300064ac:	441a      	add	r2, r3
300064ae:	4293      	cmp	r3, r2
300064b0:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
300064b4:	461d      	mov	r5, r3
300064b6:	f103 0320 	add.w	r3, r3, #32
300064ba:	d1f8      	bne.n	300064ae <BOOT_LoadAPSubImage+0x18a>
300064bc:	f3bf 8f4f 	dsb	sy
300064c0:	f3bf 8f6f 	isb	sy
300064c4:	2e00      	cmp	r6, #0
300064c6:	f47f af7f 	bne.w	300063c8 <BOOT_LoadAPSubImage+0xa4>
300064ca:	2001      	movs	r0, #1
300064cc:	b00f      	add	sp, #60	; 0x3c
300064ce:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300064d2:	4b4e      	ldr	r3, [pc, #312]	; (3000660c <BOOT_LoadAPSubImage+0x2e8>)
300064d4:	ac04      	add	r4, sp, #16
300064d6:	4a53      	ldr	r2, [pc, #332]	; (30006624 <BOOT_LoadAPSubImage+0x300>)
300064d8:	2004      	movs	r0, #4
300064da:	4619      	mov	r1, r3
300064dc:	9400      	str	r4, [sp, #0]
300064de:	f005 f869 	bl	3000b5b4 <rtk_log_write>
300064e2:	2000      	movs	r0, #0
300064e4:	b00f      	add	sp, #60	; 0x3c
300064e6:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300064ea:	4639      	mov	r1, r7
300064ec:	4642      	mov	r2, r8
300064ee:	4628      	mov	r0, r5
300064f0:	f7fe f9c4 	bl	3000487c <BOOT_ImgCopy>
300064f4:	494a      	ldr	r1, [pc, #296]	; (30006620 <BOOT_LoadAPSubImage+0x2fc>)
300064f6:	694b      	ldr	r3, [r1, #20]
300064f8:	03db      	lsls	r3, r3, #15
300064fa:	f57f af5a 	bpl.w	300063b2 <BOOT_LoadAPSubImage+0x8e>
300064fe:	ea08 0305 	and.w	r3, r8, r5
30006502:	3301      	adds	r3, #1
30006504:	d058      	beq.n	300065b8 <BOOT_LoadAPSubImage+0x294>
30006506:	f015 031f 	ands.w	r3, r5, #31
3000650a:	d073      	beq.n	300065f4 <BOOT_LoadAPSubImage+0x2d0>
3000650c:	4443      	add	r3, r8
3000650e:	f025 021f 	bic.w	r2, r5, #31
30006512:	f3bf 8f4f 	dsb	sy
30006516:	2b00      	cmp	r3, #0
30006518:	dd0d      	ble.n	30006536 <BOOT_LoadAPSubImage+0x212>
3000651a:	1e59      	subs	r1, r3, #1
3000651c:	f102 0320 	add.w	r3, r2, #32
30006520:	483f      	ldr	r0, [pc, #252]	; (30006620 <BOOT_LoadAPSubImage+0x2fc>)
30006522:	f021 011f 	bic.w	r1, r1, #31
30006526:	4419      	add	r1, r3
30006528:	428b      	cmp	r3, r1
3000652a:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
3000652e:	461a      	mov	r2, r3
30006530:	f103 0320 	add.w	r3, r3, #32
30006534:	d1f8      	bne.n	30006528 <BOOT_LoadAPSubImage+0x204>
30006536:	f3bf 8f4f 	dsb	sy
3000653a:	f3bf 8f6f 	isb	sy
3000653e:	e738      	b.n	300063b2 <BOOT_LoadAPSubImage+0x8e>
30006540:	2300      	movs	r3, #0
30006542:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30006546:	f3bf 8f4f 	dsb	sy
3000654a:	f8d1 0080 	ldr.w	r0, [r1, #128]	; 0x80
3000654e:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30006552:	f3c0 324e 	ubfx	r2, r0, #13, #15
30006556:	f3c0 00c9 	ubfx	r0, r0, #3, #10
3000655a:	0152      	lsls	r2, r2, #5
3000655c:	ea02 0c0e 	and.w	ip, r2, lr
30006560:	4603      	mov	r3, r0
30006562:	ea4c 7583 	orr.w	r5, ip, r3, lsl #30
30006566:	3b01      	subs	r3, #1
30006568:	f8c1 5274 	str.w	r5, [r1, #628]	; 0x274
3000656c:	d2f9      	bcs.n	30006562 <BOOT_LoadAPSubImage+0x23e>
3000656e:	3a20      	subs	r2, #32
30006570:	f112 0f20 	cmn.w	r2, #32
30006574:	d1f2      	bne.n	3000655c <BOOT_LoadAPSubImage+0x238>
30006576:	e776      	b.n	30006466 <BOOT_LoadAPSubImage+0x142>
30006578:	2300      	movs	r3, #0
3000657a:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000657e:	f3bf 8f4f 	dsb	sy
30006582:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
30006586:	f643 77e0 	movw	r7, #16352	; 0x3fe0
3000658a:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000658e:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30006592:	0164      	lsls	r4, r4, #5
30006594:	ea04 0007 	and.w	r0, r4, r7
30006598:	462b      	mov	r3, r5
3000659a:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
3000659e:	3b01      	subs	r3, #1
300065a0:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300065a4:	1c5a      	adds	r2, r3, #1
300065a6:	d1f8      	bne.n	3000659a <BOOT_LoadAPSubImage+0x276>
300065a8:	3c20      	subs	r4, #32
300065aa:	f114 0f20 	cmn.w	r4, #32
300065ae:	d1f1      	bne.n	30006594 <BOOT_LoadAPSubImage+0x270>
300065b0:	e784      	b.n	300064bc <BOOT_LoadAPSubImage+0x198>
300065b2:	463b      	mov	r3, r7
300065b4:	4642      	mov	r2, r8
300065b6:	e744      	b.n	30006442 <BOOT_LoadAPSubImage+0x11e>
300065b8:	2300      	movs	r3, #0
300065ba:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300065be:	f3bf 8f4f 	dsb	sy
300065c2:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
300065c6:	f643 79e0 	movw	r9, #16352	; 0x3fe0
300065ca:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
300065ce:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
300065d2:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
300065d6:	ea0c 0009 	and.w	r0, ip, r9
300065da:	4673      	mov	r3, lr
300065dc:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
300065e0:	3b01      	subs	r3, #1
300065e2:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300065e6:	d2f9      	bcs.n	300065dc <BOOT_LoadAPSubImage+0x2b8>
300065e8:	f1ac 0c20 	sub.w	ip, ip, #32
300065ec:	f11c 0f20 	cmn.w	ip, #32
300065f0:	d1f1      	bne.n	300065d6 <BOOT_LoadAPSubImage+0x2b2>
300065f2:	e7a0      	b.n	30006536 <BOOT_LoadAPSubImage+0x212>
300065f4:	4643      	mov	r3, r8
300065f6:	462a      	mov	r2, r5
300065f8:	e78b      	b.n	30006512 <BOOT_LoadAPSubImage+0x1ee>
300065fa:	bf00      	nop
300065fc:	3000c814 	.word	0x3000c814
30006600:	35393138 	.word	0x35393138
30006604:	f7ffffe0 	.word	0xf7ffffe0
30006608:	0000c109 	.word	0x0000c109
3000660c:	3000bd58 	.word	0x3000bd58
30006610:	3000c874 	.word	0x3000c874
30006614:	3000c830 	.word	0x3000c830
30006618:	3000c844 	.word	0x3000c844
3000661c:	3000c854 	.word	0x3000c854
30006620:	e000ed00 	.word	0xe000ed00
30006624:	3000c81c 	.word	0x3000c81c

30006628 <BOOT_RSIPIvSet>:
30006628:	b530      	push	{r4, r5, lr}
3000662a:	2300      	movs	r3, #0
3000662c:	b083      	sub	sp, #12
3000662e:	4a09      	ldr	r2, [pc, #36]	; (30006654 <BOOT_RSIPIvSet+0x2c>)
30006630:	4605      	mov	r5, r0
30006632:	460c      	mov	r4, r1
30006634:	9300      	str	r3, [sp, #0]
30006636:	9301      	str	r3, [sp, #4]
30006638:	4790      	blx	r2
3000663a:	b148      	cbz	r0, 30006650 <BOOT_RSIPIvSet+0x28>
3000663c:	f105 0120 	add.w	r1, r5, #32
30006640:	4b05      	ldr	r3, [pc, #20]	; (30006658 <BOOT_RSIPIvSet+0x30>)
30006642:	2208      	movs	r2, #8
30006644:	4668      	mov	r0, sp
30006646:	4798      	blx	r3
30006648:	4669      	mov	r1, sp
3000664a:	b2e0      	uxtb	r0, r4
3000664c:	4b03      	ldr	r3, [pc, #12]	; (3000665c <BOOT_RSIPIvSet+0x34>)
3000664e:	4798      	blx	r3
30006650:	b003      	add	sp, #12
30006652:	bd30      	pop	{r4, r5, pc}
30006654:	0000c129 	.word	0x0000c129
30006658:	00012a1d 	.word	0x00012a1d
3000665c:	30008fed 	.word	0x30008fed

30006660 <BOOT_OTFCheck>:
30006660:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30006664:	4d1c      	ldr	r5, [pc, #112]	; (300066d8 <BOOT_OTFCheck+0x78>)
30006666:	b082      	sub	sp, #8
30006668:	4604      	mov	r4, r0
3000666a:	4688      	mov	r8, r1
3000666c:	4617      	mov	r7, r2
3000666e:	461e      	mov	r6, r3
30006670:	47a8      	blx	r5
30006672:	b148      	cbz	r0, 30006688 <BOOT_OTFCheck+0x28>
30006674:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30006678:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000667c:	d204      	bcs.n	30006688 <BOOT_OTFCheck+0x28>
3000667e:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30006682:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006686:	d302      	bcc.n	3000668e <BOOT_OTFCheck+0x2e>
30006688:	b002      	add	sp, #8
3000668a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000668e:	4b13      	ldr	r3, [pc, #76]	; (300066dc <BOOT_OTFCheck+0x7c>)
30006690:	2004      	movs	r0, #4
30006692:	4a13      	ldr	r2, [pc, #76]	; (300066e0 <BOOT_OTFCheck+0x80>)
30006694:	4619      	mov	r1, r3
30006696:	f004 ff8d 	bl	3000b5b4 <rtk_log_write>
3000669a:	4b12      	ldr	r3, [pc, #72]	; (300066e4 <BOOT_OTFCheck+0x84>)
3000669c:	4798      	blx	r3
3000669e:	4603      	mov	r3, r0
300066a0:	b150      	cbz	r0, 300066b8 <BOOT_OTFCheck+0x58>
300066a2:	2801      	cmp	r0, #1
300066a4:	d009      	beq.n	300066ba <BOOT_OTFCheck+0x5a>
300066a6:	4b0d      	ldr	r3, [pc, #52]	; (300066dc <BOOT_OTFCheck+0x7c>)
300066a8:	2002      	movs	r0, #2
300066aa:	4a0f      	ldr	r2, [pc, #60]	; (300066e8 <BOOT_OTFCheck+0x88>)
300066ac:	4619      	mov	r1, r3
300066ae:	b002      	add	sp, #8
300066b0:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300066b4:	f004 bf7e 	b.w	3000b5b4 <rtk_log_write>
300066b8:	2302      	movs	r3, #2
300066ba:	4630      	mov	r0, r6
300066bc:	4621      	mov	r1, r4
300066be:	9301      	str	r3, [sp, #4]
300066c0:	4642      	mov	r2, r8
300066c2:	2301      	movs	r3, #1
300066c4:	4c09      	ldr	r4, [pc, #36]	; (300066ec <BOOT_OTFCheck+0x8c>)
300066c6:	9700      	str	r7, [sp, #0]
300066c8:	47a0      	blx	r4
300066ca:	2001      	movs	r0, #1
300066cc:	4b08      	ldr	r3, [pc, #32]	; (300066f0 <BOOT_OTFCheck+0x90>)
300066ce:	b002      	add	sp, #8
300066d0:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300066d4:	4718      	bx	r3
300066d6:	bf00      	nop
300066d8:	0000c129 	.word	0x0000c129
300066dc:	3000bd58 	.word	0x3000bd58
300066e0:	3000c884 	.word	0x3000c884
300066e4:	0000c245 	.word	0x0000c245
300066e8:	3000c898 	.word	0x3000c898
300066ec:	30009021 	.word	0x30009021
300066f0:	30009009 	.word	0x30009009

300066f4 <BOOT_OTA_LoadIMG2>:
300066f4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300066f8:	4b6e      	ldr	r3, [pc, #440]	; (300068b4 <BOOT_OTA_LoadIMG2+0x1c0>)
300066fa:	b08d      	sub	sp, #52	; 0x34
300066fc:	4a6e      	ldr	r2, [pc, #440]	; (300068b8 <BOOT_OTA_LoadIMG2+0x1c4>)
300066fe:	f103 040c 	add.w	r4, r3, #12
30006702:	9003      	str	r0, [sp, #12]
30006704:	f8d2 8000 	ldr.w	r8, [r2]
30006708:	ae06      	add	r6, sp, #24
3000670a:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
3000670e:	ab09      	add	r3, sp, #36	; 0x24
30006710:	e886 0007 	stmia.w	r6, {r0, r1, r2}
30006714:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30006718:	e883 0007 	stmia.w	r3, {r0, r1, r2}
3000671c:	4641      	mov	r1, r8
3000671e:	4867      	ldr	r0, [pc, #412]	; (300068bc <BOOT_OTA_LoadIMG2+0x1c8>)
30006720:	f44f 72b2 	mov.w	r2, #356	; 0x164
30006724:	f7fe f8aa 	bl	3000487c <BOOT_ImgCopy>
30006728:	4641      	mov	r1, r8
3000672a:	4864      	ldr	r0, [pc, #400]	; (300068bc <BOOT_OTA_LoadIMG2+0x1c8>)
3000672c:	f000 fd2a 	bl	30007184 <BOOT_CertificateCheck>
30006730:	2800      	cmp	r0, #0
30006732:	f040 80b6 	bne.w	300068a2 <BOOT_OTA_LoadIMG2+0x1ae>
30006736:	4604      	mov	r4, r0
30006738:	f44f 72c8 	mov.w	r2, #400	; 0x190
3000673c:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30006740:	485f      	ldr	r0, [pc, #380]	; (300068c0 <BOOT_OTA_LoadIMG2+0x1cc>)
30006742:	f7fe f89b 	bl	3000487c <BOOT_ImgCopy>
30006746:	4b5f      	ldr	r3, [pc, #380]	; (300068c4 <BOOT_OTA_LoadIMG2+0x1d0>)
30006748:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000674c:	4798      	blx	r3
3000674e:	2800      	cmp	r0, #0
30006750:	f040 809d 	bne.w	3000688e <BOOT_OTA_LoadIMG2+0x19a>
30006754:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30006758:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30006908 <BOOT_OTA_LoadIMG2+0x214>
3000675c:	4a5a      	ldr	r2, [pc, #360]	; (300068c8 <BOOT_OTA_LoadIMG2+0x1d4>)
3000675e:	2000      	movs	r0, #0
30006760:	4445      	add	r5, r8
30006762:	4659      	mov	r1, fp
30006764:	4f59      	ldr	r7, [pc, #356]	; (300068cc <BOOT_OTA_LoadIMG2+0x1d8>)
30006766:	f04f 0801 	mov.w	r8, #1
3000676a:	462b      	mov	r3, r5
3000676c:	f8df 919c 	ldr.w	r9, [pc, #412]	; 3000690c <BOOT_OTA_LoadIMG2+0x218>
30006770:	47b8      	blx	r7
30006772:	2101      	movs	r1, #1
30006774:	2000      	movs	r0, #0
30006776:	4b56      	ldr	r3, [pc, #344]	; (300068d0 <BOOT_OTA_LoadIMG2+0x1dc>)
30006778:	4798      	blx	r3
3000677a:	2301      	movs	r3, #1
3000677c:	4952      	ldr	r1, [pc, #328]	; (300068c8 <BOOT_OTA_LoadIMG2+0x1d4>)
3000677e:	4658      	mov	r0, fp
30006780:	461a      	mov	r2, r3
30006782:	f7ff ff6d 	bl	30006660 <BOOT_OTFCheck>
30006786:	47c8      	blx	r9
30006788:	2800      	cmp	r0, #0
3000678a:	bf0c      	ite	eq
3000678c:	46aa      	moveq	sl, r5
3000678e:	46da      	movne	sl, fp
30006790:	4633      	mov	r3, r6
30006792:	2203      	movs	r2, #3
30006794:	4651      	mov	r1, sl
30006796:	484f      	ldr	r0, [pc, #316]	; (300068d4 <BOOT_OTA_LoadIMG2+0x1e0>)
30006798:	f8cd 8000 	str.w	r8, [sp]
3000679c:	f7ff fd0e 	bl	300061bc <BOOT_LoadSubImage>
300067a0:	2800      	cmp	r0, #0
300067a2:	d06f      	beq.n	30006884 <BOOT_OTA_LoadIMG2+0x190>
300067a4:	4e4b      	ldr	r6, [pc, #300]	; (300068d4 <BOOT_OTA_LoadIMG2+0x1e0>)
300067a6:	47c8      	blx	r9
300067a8:	2800      	cmp	r0, #0
300067aa:	bf08      	it	eq
300067ac:	46ab      	moveq	fp, r5
300067ae:	4f47      	ldr	r7, [pc, #284]	; (300068cc <BOOT_OTA_LoadIMG2+0x1d8>)
300067b0:	68f2      	ldr	r2, [r6, #12]
300067b2:	4640      	mov	r0, r8
300067b4:	6873      	ldr	r3, [r6, #4]
300067b6:	f8c6 b000 	str.w	fp, [r6]
300067ba:	4413      	add	r3, r2
300067bc:	6972      	ldr	r2, [r6, #20]
300067be:	f8df b150 	ldr.w	fp, [pc, #336]	; 30006910 <BOOT_OTA_LoadIMG2+0x21c>
300067c2:	441a      	add	r2, r3
300067c4:	4b44      	ldr	r3, [pc, #272]	; (300068d8 <BOOT_OTA_LoadIMG2+0x1e4>)
300067c6:	eb0a 0102 	add.w	r1, sl, r2
300067ca:	61b1      	str	r1, [r6, #24]
300067cc:	1e51      	subs	r1, r2, #1
300067ce:	400b      	ands	r3, r1
300067d0:	4659      	mov	r1, fp
300067d2:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
300067d6:	441d      	add	r5, r3
300067d8:	1a9b      	subs	r3, r3, r2
300067da:	4a40      	ldr	r2, [pc, #256]	; (300068dc <BOOT_OTA_LoadIMG2+0x1e8>)
300067dc:	61f3      	str	r3, [r6, #28]
300067de:	462b      	mov	r3, r5
300067e0:	47b8      	blx	r7
300067e2:	4641      	mov	r1, r8
300067e4:	4640      	mov	r0, r8
300067e6:	4b3a      	ldr	r3, [pc, #232]	; (300068d0 <BOOT_OTA_LoadIMG2+0x1dc>)
300067e8:	4798      	blx	r3
300067ea:	4642      	mov	r2, r8
300067ec:	2302      	movs	r3, #2
300067ee:	493b      	ldr	r1, [pc, #236]	; (300068dc <BOOT_OTA_LoadIMG2+0x1e8>)
300067f0:	4658      	mov	r0, fp
300067f2:	f7ff ff35 	bl	30006660 <BOOT_OTFCheck>
300067f6:	47c8      	blx	r9
300067f8:	2800      	cmp	r0, #0
300067fa:	bf0c      	ite	eq
300067fc:	462f      	moveq	r7, r5
300067fe:	465f      	movne	r7, fp
30006800:	ab09      	add	r3, sp, #36	; 0x24
30006802:	2203      	movs	r2, #3
30006804:	f106 0020 	add.w	r0, r6, #32
30006808:	4639      	mov	r1, r7
3000680a:	f8cd 8000 	str.w	r8, [sp]
3000680e:	f7ff fcd5 	bl	300061bc <BOOT_LoadSubImage>
30006812:	2800      	cmp	r0, #0
30006814:	d036      	beq.n	30006884 <BOOT_OTA_LoadIMG2+0x190>
30006816:	47c8      	blx	r9
30006818:	6af2      	ldr	r2, [r6, #44]	; 0x2c
3000681a:	6a73      	ldr	r3, [r6, #36]	; 0x24
3000681c:	2800      	cmp	r0, #0
3000681e:	bf08      	it	eq
30006820:	46ab      	moveq	fp, r5
30006822:	4827      	ldr	r0, [pc, #156]	; (300068c0 <BOOT_OTA_LoadIMG2+0x1cc>)
30006824:	4413      	add	r3, r2
30006826:	6b72      	ldr	r2, [r6, #52]	; 0x34
30006828:	f8c6 b020 	str.w	fp, [r6, #32]
3000682c:	4413      	add	r3, r2
3000682e:	4a2a      	ldr	r2, [pc, #168]	; (300068d8 <BOOT_OTA_LoadIMG2+0x1e4>)
30006830:	1e59      	subs	r1, r3, #1
30006832:	441f      	add	r7, r3
30006834:	400a      	ands	r2, r1
30006836:	2100      	movs	r1, #0
30006838:	63b7      	str	r7, [r6, #56]	; 0x38
3000683a:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
3000683e:	9100      	str	r1, [sp, #0]
30006840:	1ad1      	subs	r1, r2, r3
30006842:	2900      	cmp	r1, #0
30006844:	63f1      	str	r1, [r6, #60]	; 0x3c
30006846:	4631      	mov	r1, r6
30006848:	bf18      	it	ne
3000684a:	4613      	movne	r3, r2
3000684c:	4a24      	ldr	r2, [pc, #144]	; (300068e0 <BOOT_OTA_LoadIMG2+0x1ec>)
3000684e:	441d      	add	r5, r3
30006850:	4b1a      	ldr	r3, [pc, #104]	; (300068bc <BOOT_OTA_LoadIMG2+0x1c8>)
30006852:	6015      	str	r5, [r2, #0]
30006854:	bf0c      	ite	eq
30006856:	2207      	moveq	r2, #7
30006858:	2208      	movne	r2, #8
3000685a:	f000 fb89 	bl	30006f70 <BOOT_SignatureCheck>
3000685e:	4b21      	ldr	r3, [pc, #132]	; (300068e4 <BOOT_OTA_LoadIMG2+0x1f0>)
30006860:	4605      	mov	r5, r0
30006862:	689b      	ldr	r3, [r3, #8]
30006864:	079b      	lsls	r3, r3, #30
30006866:	d509      	bpl.n	3000687c <BOOT_OTA_LoadIMG2+0x188>
30006868:	4b1f      	ldr	r3, [pc, #124]	; (300068e8 <BOOT_OTA_LoadIMG2+0x1f4>)
3000686a:	4a20      	ldr	r2, [pc, #128]	; (300068ec <BOOT_OTA_LoadIMG2+0x1f8>)
3000686c:	9903      	ldr	r1, [sp, #12]
3000686e:	2800      	cmp	r0, #0
30006870:	bf18      	it	ne
30006872:	461a      	movne	r2, r3
30006874:	481e      	ldr	r0, [pc, #120]	; (300068f0 <BOOT_OTA_LoadIMG2+0x1fc>)
30006876:	462b      	mov	r3, r5
30006878:	f005 f93a 	bl	3000baf0 <__DiagPrintf_veneer>
3000687c:	b11d      	cbz	r5, 30006886 <BOOT_OTA_LoadIMG2+0x192>
3000687e:	481d      	ldr	r0, [pc, #116]	; (300068f4 <BOOT_OTA_LoadIMG2+0x200>)
30006880:	f005 f936 	bl	3000baf0 <__DiagPrintf_veneer>
30006884:	24ff      	movs	r4, #255	; 0xff
30006886:	4620      	mov	r0, r4
30006888:	b00d      	add	sp, #52	; 0x34
3000688a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000688e:	2208      	movs	r2, #8
30006890:	4919      	ldr	r1, [pc, #100]	; (300068f8 <BOOT_OTA_LoadIMG2+0x204>)
30006892:	4b1a      	ldr	r3, [pc, #104]	; (300068fc <BOOT_OTA_LoadIMG2+0x208>)
30006894:	a804      	add	r0, sp, #16
30006896:	4798      	blx	r3
30006898:	a904      	add	r1, sp, #16
3000689a:	2001      	movs	r0, #1
3000689c:	4b18      	ldr	r3, [pc, #96]	; (30006900 <BOOT_OTA_LoadIMG2+0x20c>)
3000689e:	4798      	blx	r3
300068a0:	e758      	b.n	30006754 <BOOT_OTA_LoadIMG2+0x60>
300068a2:	24ff      	movs	r4, #255	; 0xff
300068a4:	4817      	ldr	r0, [pc, #92]	; (30006904 <BOOT_OTA_LoadIMG2+0x210>)
300068a6:	f005 f923 	bl	3000baf0 <__DiagPrintf_veneer>
300068aa:	4620      	mov	r0, r4
300068ac:	b00d      	add	sp, #52	; 0x34
300068ae:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300068b2:	bf00      	nop
300068b4:	3000dc74 	.word	0x3000dc74
300068b8:	3000f438 	.word	0x3000f438
300068bc:	3000f4b4 	.word	0x3000f4b4
300068c0:	3000f618 	.word	0x3000f618
300068c4:	0000c129 	.word	0x0000c129
300068c8:	0cffffe0 	.word	0x0cffffe0
300068cc:	3000909d 	.word	0x3000909d
300068d0:	300090dd 	.word	0x300090dd
300068d4:	3000f7c0 	.word	0x3000f7c0
300068d8:	fffff000 	.word	0xfffff000
300068dc:	0dffffe0 	.word	0x0dffffe0
300068e0:	3000e56c 	.word	0x3000e56c
300068e4:	2001c00c 	.word	0x2001c00c
300068e8:	3000c8b8 	.word	0x3000c8b8
300068ec:	3000c8b0 	.word	0x3000c8b0
300068f0:	3000c8e8 	.word	0x3000c8e8
300068f4:	3000c91c 	.word	0x3000c91c
300068f8:	3000f638 	.word	0x3000f638
300068fc:	00012a1d 	.word	0x00012a1d
30006900:	30008fed 	.word	0x30008fed
30006904:	3000c8c0 	.word	0x3000c8c0
30006908:	0c000000 	.word	0x0c000000
3000690c:	3000927d 	.word	0x3000927d
30006910:	0d000000 	.word	0x0d000000

30006914 <BOOT_OTA_Region_Init>:
30006914:	b510      	push	{r4, lr}
30006916:	4c10      	ldr	r4, [pc, #64]	; (30006958 <BOOT_OTA_Region_Init+0x44>)
30006918:	2200      	movs	r2, #0
3000691a:	2002      	movs	r0, #2
3000691c:	4621      	mov	r1, r4
3000691e:	f001 fa73 	bl	30007e08 <flash_get_layout_info>
30006922:	2200      	movs	r2, #0
30006924:	1d21      	adds	r1, r4, #4
30006926:	2003      	movs	r0, #3
30006928:	f001 fa6e 	bl	30007e08 <flash_get_layout_info>
3000692c:	2200      	movs	r2, #0
3000692e:	f104 0110 	add.w	r1, r4, #16
30006932:	2004      	movs	r0, #4
30006934:	f001 fa68 	bl	30007e08 <flash_get_layout_info>
30006938:	2200      	movs	r2, #0
3000693a:	f104 0114 	add.w	r1, r4, #20
3000693e:	2005      	movs	r0, #5
30006940:	f001 fa62 	bl	30007e08 <flash_get_layout_info>
30006944:	e9d4 2300 	ldrd	r2, r3, [r4]
30006948:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
3000694c:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006950:	e9c4 2302 	strd	r2, r3, [r4, #8]
30006954:	bd10      	pop	{r4, pc}
30006956:	bf00      	nop
30006958:	3000f7a8 	.word	0x3000f7a8

3000695c <BOOT_OTA_IMG2>:
3000695c:	f7ff beca 	b.w	300066f4 <BOOT_OTA_LoadIMG2>

30006960 <BOOT_OTA_AP>:
30006960:	b5f0      	push	{r4, r5, r6, r7, lr}
30006962:	4a32      	ldr	r2, [pc, #200]	; (30006a2c <BOOT_OTA_AP+0xcc>)
30006964:	b0eb      	sub	sp, #428	; 0x1ac
30006966:	4b32      	ldr	r3, [pc, #200]	; (30006a30 <BOOT_OTA_AP+0xd0>)
30006968:	6815      	ldr	r5, [r2, #0]
3000696a:	ac02      	add	r4, sp, #8
3000696c:	4e31      	ldr	r6, [pc, #196]	; (30006a34 <BOOT_OTA_AP+0xd4>)
3000696e:	f505 5780 	add.w	r7, r5, #4096	; 0x1000
30006972:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006974:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006978:	4629      	mov	r1, r5
3000697a:	4d2f      	ldr	r5, [pc, #188]	; (30006a38 <BOOT_OTA_AP+0xd8>)
3000697c:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006980:	a806      	add	r0, sp, #24
30006982:	f7fd ff7b 	bl	3000487c <BOOT_ImgCopy>
30006986:	4a2d      	ldr	r2, [pc, #180]	; (30006a3c <BOOT_OTA_AP+0xdc>)
30006988:	463b      	mov	r3, r7
3000698a:	4629      	mov	r1, r5
3000698c:	2003      	movs	r0, #3
3000698e:	47b0      	blx	r6
30006990:	2101      	movs	r1, #1
30006992:	4b2b      	ldr	r3, [pc, #172]	; (30006a40 <BOOT_OTA_AP+0xe0>)
30006994:	2003      	movs	r0, #3
30006996:	4798      	blx	r3
30006998:	2201      	movs	r2, #1
3000699a:	4928      	ldr	r1, [pc, #160]	; (30006a3c <BOOT_OTA_AP+0xdc>)
3000699c:	2303      	movs	r3, #3
3000699e:	4628      	mov	r0, r5
300069a0:	f7ff fe5e 	bl	30006660 <BOOT_OTFCheck>
300069a4:	4b27      	ldr	r3, [pc, #156]	; (30006a44 <BOOT_OTA_AP+0xe4>)
300069a6:	4798      	blx	r3
300069a8:	4623      	mov	r3, r4
300069aa:	2201      	movs	r2, #1
300069ac:	4c26      	ldr	r4, [pc, #152]	; (30006a48 <BOOT_OTA_AP+0xe8>)
300069ae:	2800      	cmp	r0, #0
300069b0:	bf14      	ite	ne
300069b2:	4629      	movne	r1, r5
300069b4:	4639      	moveq	r1, r7
300069b6:	9200      	str	r2, [sp, #0]
300069b8:	4620      	mov	r0, r4
300069ba:	2204      	movs	r2, #4
300069bc:	f7ff fbfe 	bl	300061bc <BOOT_LoadSubImage>
300069c0:	2801      	cmp	r0, #1
300069c2:	d122      	bne.n	30006a0a <BOOT_OTA_AP+0xaa>
300069c4:	2302      	movs	r3, #2
300069c6:	4605      	mov	r5, r0
300069c8:	2204      	movs	r2, #4
300069ca:	a806      	add	r0, sp, #24
300069cc:	9300      	str	r3, [sp, #0]
300069ce:	4621      	mov	r1, r4
300069d0:	4b1e      	ldr	r3, [pc, #120]	; (30006a4c <BOOT_OTA_AP+0xec>)
300069d2:	f000 facd 	bl	30006f70 <BOOT_SignatureCheck>
300069d6:	4606      	mov	r6, r0
300069d8:	bb18      	cbnz	r0, 30006a22 <BOOT_OTA_AP+0xc2>
300069da:	68e3      	ldr	r3, [r4, #12]
300069dc:	462a      	mov	r2, r5
300069de:	6861      	ldr	r1, [r4, #4]
300069e0:	f104 0018 	add.w	r0, r4, #24
300069e4:	4419      	add	r1, r3
300069e6:	6963      	ldr	r3, [r4, #20]
300069e8:	4419      	add	r1, r3
300069ea:	4439      	add	r1, r7
300069ec:	f7ff fc9a 	bl	30006324 <BOOT_LoadAPSubImage>
300069f0:	2801      	cmp	r0, #1
300069f2:	d102      	bne.n	300069fa <BOOT_OTA_AP+0x9a>
300069f4:	4630      	mov	r0, r6
300069f6:	b06b      	add	sp, #428	; 0x1ac
300069f8:	bdf0      	pop	{r4, r5, r6, r7, pc}
300069fa:	4b15      	ldr	r3, [pc, #84]	; (30006a50 <BOOT_OTA_AP+0xf0>)
300069fc:	681b      	ldr	r3, [r3, #0]
300069fe:	079b      	lsls	r3, r3, #30
30006a00:	d40a      	bmi.n	30006a18 <BOOT_OTA_AP+0xb8>
30006a02:	26ff      	movs	r6, #255	; 0xff
30006a04:	4630      	mov	r0, r6
30006a06:	b06b      	add	sp, #428	; 0x1ac
30006a08:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006a0a:	26ff      	movs	r6, #255	; 0xff
30006a0c:	4811      	ldr	r0, [pc, #68]	; (30006a54 <BOOT_OTA_AP+0xf4>)
30006a0e:	f005 f86f 	bl	3000baf0 <__DiagPrintf_veneer>
30006a12:	4630      	mov	r0, r6
30006a14:	b06b      	add	sp, #428	; 0x1ac
30006a16:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006a18:	480f      	ldr	r0, [pc, #60]	; (30006a58 <BOOT_OTA_AP+0xf8>)
30006a1a:	26ff      	movs	r6, #255	; 0xff
30006a1c:	f005 f868 	bl	3000baf0 <__DiagPrintf_veneer>
30006a20:	e7e8      	b.n	300069f4 <BOOT_OTA_AP+0x94>
30006a22:	480e      	ldr	r0, [pc, #56]	; (30006a5c <BOOT_OTA_AP+0xfc>)
30006a24:	26ff      	movs	r6, #255	; 0xff
30006a26:	f005 f863 	bl	3000baf0 <__DiagPrintf_veneer>
30006a2a:	e7e3      	b.n	300069f4 <BOOT_OTA_AP+0x94>
30006a2c:	3000e56c 	.word	0x3000e56c
30006a30:	3000dc8c 	.word	0x3000dc8c
30006a34:	3000909d 	.word	0x3000909d
30006a38:	0e000000 	.word	0x0e000000
30006a3c:	0fffffe0 	.word	0x0fffffe0
30006a40:	300090dd 	.word	0x300090dd
30006a44:	3000927d 	.word	0x3000927d
30006a48:	3000f7c0 	.word	0x3000f7c0
30006a4c:	3000f4b4 	.word	0x3000f4b4
30006a50:	2001c00c 	.word	0x2001c00c
30006a54:	3000c964 	.word	0x3000c964
30006a58:	3000c988 	.word	0x3000c988
30006a5c:	3000c940 	.word	0x3000c940

30006a60 <BOOT_LoadRDPImg>:
30006a60:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006a64:	4d4e      	ldr	r5, [pc, #312]	; (30006ba0 <BOOT_LoadRDPImg+0x140>)
30006a66:	b0d9      	sub	sp, #356	; 0x164
30006a68:	4613      	mov	r3, r2
30006a6a:	468b      	mov	fp, r1
30006a6c:	4604      	mov	r4, r0
30006a6e:	aa06      	add	r2, sp, #24
30006a70:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30006a74:	e895 0003 	ldmia.w	r5, {r0, r1}
30006a78:	2500      	movs	r5, #0
30006a7a:	e882 0003 	stmia.w	r2, {r0, r1}
30006a7e:	4619      	mov	r1, r3
30006a80:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006a84:	4620      	mov	r0, r4
30006a86:	f7fd fef9 	bl	3000487c <BOOT_ImgCopy>
30006a8a:	ab06      	add	r3, sp, #24
30006a8c:	2202      	movs	r2, #2
30006a8e:	4641      	mov	r1, r8
30006a90:	4658      	mov	r0, fp
30006a92:	9500      	str	r5, [sp, #0]
30006a94:	f7ff fb92 	bl	300061bc <BOOT_LoadSubImage>
30006a98:	2801      	cmp	r0, #1
30006a9a:	d06f      	beq.n	30006b7c <BOOT_LoadRDPImg+0x11c>
30006a9c:	4b41      	ldr	r3, [pc, #260]	; (30006ba4 <BOOT_LoadRDPImg+0x144>)
30006a9e:	4628      	mov	r0, r5
30006aa0:	4798      	blx	r3
30006aa2:	2101      	movs	r1, #1
30006aa4:	4b40      	ldr	r3, [pc, #256]	; (30006ba8 <BOOT_LoadRDPImg+0x148>)
30006aa6:	4628      	mov	r0, r5
30006aa8:	4798      	blx	r3
30006aaa:	4b40      	ldr	r3, [pc, #256]	; (30006bac <BOOT_LoadRDPImg+0x14c>)
30006aac:	2120      	movs	r1, #32
30006aae:	a810      	add	r0, sp, #64	; 0x40
30006ab0:	4798      	blx	r3
30006ab2:	f8df a110 	ldr.w	sl, [pc, #272]	; 30006bc4 <BOOT_LoadRDPImg+0x164>
30006ab6:	f104 0120 	add.w	r1, r4, #32
30006aba:	2210      	movs	r2, #16
30006abc:	a80c      	add	r0, sp, #48	; 0x30
30006abe:	af08      	add	r7, sp, #32
30006ac0:	47d0      	blx	sl
30006ac2:	f8df 9104 	ldr.w	r9, [pc, #260]	; 30006bc8 <BOOT_LoadRDPImg+0x168>
30006ac6:	ab06      	add	r3, sp, #24
30006ac8:	9504      	str	r5, [sp, #16]
30006aca:	9305      	str	r3, [sp, #20]
30006acc:	4641      	mov	r1, r8
30006ace:	a818      	add	r0, sp, #96	; 0x60
30006ad0:	2210      	movs	r2, #16
30006ad2:	f7fd fed3 	bl	3000487c <BOOT_ImgCopy>
30006ad6:	2310      	movs	r3, #16
30006ad8:	aa0c      	add	r2, sp, #48	; 0x30
30006ada:	a818      	add	r0, sp, #96	; 0x60
30006adc:	4619      	mov	r1, r3
30006ade:	9700      	str	r7, [sp, #0]
30006ae0:	47c8      	blx	r9
30006ae2:	683b      	ldr	r3, [r7, #0]
30006ae4:	4a32      	ldr	r2, [pc, #200]	; (30006bb0 <BOOT_LoadRDPImg+0x150>)
30006ae6:	4293      	cmp	r3, r2
30006ae8:	d14c      	bne.n	30006b84 <BOOT_LoadRDPImg+0x124>
30006aea:	9a05      	ldr	r2, [sp, #20]
30006aec:	687b      	ldr	r3, [r7, #4]
30006aee:	f852 1b04 	ldr.w	r1, [r2], #4
30006af2:	9205      	str	r2, [sp, #20]
30006af4:	4a2f      	ldr	r2, [pc, #188]	; (30006bb4 <BOOT_LoadRDPImg+0x154>)
30006af6:	4293      	cmp	r3, r2
30006af8:	d148      	bne.n	30006b8c <BOOT_LoadRDPImg+0x12c>
30006afa:	9100      	str	r1, [sp, #0]
30006afc:	2004      	movs	r0, #4
30006afe:	68fe      	ldr	r6, [r7, #12]
30006b00:	4b2d      	ldr	r3, [pc, #180]	; (30006bb8 <BOOT_LoadRDPImg+0x158>)
30006b02:	3e20      	subs	r6, #32
30006b04:	4a2d      	ldr	r2, [pc, #180]	; (30006bbc <BOOT_LoadRDPImg+0x15c>)
30006b06:	4619      	mov	r1, r3
30006b08:	9601      	str	r6, [sp, #4]
30006b0a:	68bd      	ldr	r5, [r7, #8]
30006b0c:	3520      	adds	r5, #32
30006b0e:	9502      	str	r5, [sp, #8]
30006b10:	f004 fd50 	bl	3000b5b4 <rtk_log_write>
30006b14:	f1bb 0f00 	cmp.w	fp, #0
30006b18:	d006      	beq.n	30006b28 <BOOT_LoadRDPImg+0xc8>
30006b1a:	9a04      	ldr	r2, [sp, #16]
30006b1c:	f10b 0304 	add.w	r3, fp, #4
30006b20:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
30006b24:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
30006b28:	b1dd      	cbz	r5, 30006b62 <BOOT_LoadRDPImg+0x102>
30006b2a:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006b2e:	462c      	mov	r4, r5
30006b30:	4641      	mov	r1, r8
30006b32:	a818      	add	r0, sp, #96	; 0x60
30006b34:	bf28      	it	cs
30006b36:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006b3a:	4622      	mov	r2, r4
30006b3c:	44a0      	add	r8, r4
30006b3e:	f7fd fe9d 	bl	3000487c <BOOT_ImgCopy>
30006b42:	aa0c      	add	r2, sp, #48	; 0x30
30006b44:	4621      	mov	r1, r4
30006b46:	2310      	movs	r3, #16
30006b48:	9600      	str	r6, [sp, #0]
30006b4a:	a818      	add	r0, sp, #96	; 0x60
30006b4c:	47c8      	blx	r9
30006b4e:	ab18      	add	r3, sp, #96	; 0x60
30006b50:	f1a4 0110 	sub.w	r1, r4, #16
30006b54:	2210      	movs	r2, #16
30006b56:	a80c      	add	r0, sp, #48	; 0x30
30006b58:	4426      	add	r6, r4
30006b5a:	4419      	add	r1, r3
30006b5c:	47d0      	blx	sl
30006b5e:	1b2d      	subs	r5, r5, r4
30006b60:	d1e3      	bne.n	30006b2a <BOOT_LoadRDPImg+0xca>
30006b62:	9b04      	ldr	r3, [sp, #16]
30006b64:	b13b      	cbz	r3, 30006b76 <BOOT_LoadRDPImg+0x116>
30006b66:	2100      	movs	r1, #0
30006b68:	4b0f      	ldr	r3, [pc, #60]	; (30006ba8 <BOOT_LoadRDPImg+0x148>)
30006b6a:	4608      	mov	r0, r1
30006b6c:	4798      	blx	r3
30006b6e:	2002      	movs	r0, #2
30006b70:	b059      	add	sp, #356	; 0x164
30006b72:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006b76:	2301      	movs	r3, #1
30006b78:	9304      	str	r3, [sp, #16]
30006b7a:	e7a7      	b.n	30006acc <BOOT_LoadRDPImg+0x6c>
30006b7c:	2002      	movs	r0, #2
30006b7e:	b059      	add	sp, #356	; 0x164
30006b80:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006b84:	9b04      	ldr	r3, [sp, #16]
30006b86:	aa06      	add	r2, sp, #24
30006b88:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30006b8c:	4b0a      	ldr	r3, [pc, #40]	; (30006bb8 <BOOT_LoadRDPImg+0x158>)
30006b8e:	2004      	movs	r0, #4
30006b90:	9100      	str	r1, [sp, #0]
30006b92:	4a0b      	ldr	r2, [pc, #44]	; (30006bc0 <BOOT_LoadRDPImg+0x160>)
30006b94:	4619      	mov	r1, r3
30006b96:	f004 fd0d 	bl	3000b5b4 <rtk_log_write>
30006b9a:	2000      	movs	r0, #0
30006b9c:	e7e8      	b.n	30006b70 <BOOT_LoadRDPImg+0x110>
30006b9e:	bf00      	nop
30006ba0:	3000dc9c 	.word	0x3000dc9c
30006ba4:	00002429 	.word	0x00002429
30006ba8:	00002479 	.word	0x00002479
30006bac:	00004641 	.word	0x00004641
30006bb0:	35393138 	.word	0x35393138
30006bb4:	31313738 	.word	0x31313738
30006bb8:	3000bd58 	.word	0x3000bd58
30006bbc:	3000c830 	.word	0x3000c830
30006bc0:	3000c81c 	.word	0x3000c81c
30006bc4:	00012a1d 	.word	0x00012a1d
30006bc8:	000046f5 	.word	0x000046f5

30006bcc <BOOT_LoadBL32RDPImg>:
30006bcc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006bd0:	4b35      	ldr	r3, [pc, #212]	; (30006ca8 <BOOT_LoadBL32RDPImg+0xdc>)
30006bd2:	b0d1      	sub	sp, #324	; 0x144
30006bd4:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 30006cc4 <BOOT_LoadBL32RDPImg+0xf8>
30006bd8:	4607      	mov	r7, r0
30006bda:	f10d 0810 	add.w	r8, sp, #16
30006bde:	460e      	mov	r6, r1
30006be0:	4615      	mov	r5, r2
30006be2:	ac02      	add	r4, sp, #8
30006be4:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30006cc8 <BOOT_LoadBL32RDPImg+0xfc>
30006be8:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 30006ccc <BOOT_LoadBL32RDPImg+0x100>
30006bec:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006bee:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
30006bf2:	e89c 0003 	ldmia.w	ip, {r0, r1}
30006bf6:	e884 0003 	stmia.w	r4, {r0, r1}
30006bfa:	2000      	movs	r0, #0
30006bfc:	47c8      	blx	r9
30006bfe:	2101      	movs	r1, #1
30006c00:	2000      	movs	r0, #0
30006c02:	47d0      	blx	sl
30006c04:	2120      	movs	r1, #32
30006c06:	4b29      	ldr	r3, [pc, #164]	; (30006cac <BOOT_LoadBL32RDPImg+0xe0>)
30006c08:	eb0d 0001 	add.w	r0, sp, r1
30006c0c:	4798      	blx	r3
30006c0e:	4b28      	ldr	r3, [pc, #160]	; (30006cb0 <BOOT_LoadBL32RDPImg+0xe4>)
30006c10:	689b      	ldr	r3, [r3, #8]
30006c12:	079b      	lsls	r3, r3, #30
30006c14:	d436      	bmi.n	30006c84 <BOOT_LoadBL32RDPImg+0xb8>
30006c16:	b1fd      	cbz	r5, 30006c58 <BOOT_LoadBL32RDPImg+0x8c>
30006c18:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 30006cd0 <BOOT_LoadBL32RDPImg+0x104>
30006c1c:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 30006cd4 <BOOT_LoadBL32RDPImg+0x108>
30006c20:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006c24:	462c      	mov	r4, r5
30006c26:	4639      	mov	r1, r7
30006c28:	a810      	add	r0, sp, #64	; 0x40
30006c2a:	bf28      	it	cs
30006c2c:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006c30:	4622      	mov	r2, r4
30006c32:	4427      	add	r7, r4
30006c34:	f7fd fe22 	bl	3000487c <BOOT_ImgCopy>
30006c38:	4642      	mov	r2, r8
30006c3a:	4621      	mov	r1, r4
30006c3c:	2310      	movs	r3, #16
30006c3e:	9600      	str	r6, [sp, #0]
30006c40:	a810      	add	r0, sp, #64	; 0x40
30006c42:	47d8      	blx	fp
30006c44:	ab10      	add	r3, sp, #64	; 0x40
30006c46:	f1a4 0110 	sub.w	r1, r4, #16
30006c4a:	2210      	movs	r2, #16
30006c4c:	4640      	mov	r0, r8
30006c4e:	4426      	add	r6, r4
30006c50:	4419      	add	r1, r3
30006c52:	47c8      	blx	r9
30006c54:	1b2d      	subs	r5, r5, r4
30006c56:	d1e3      	bne.n	30006c20 <BOOT_LoadBL32RDPImg+0x54>
30006c58:	f856 2c20 	ldr.w	r2, [r6, #-32]
30006c5c:	3e20      	subs	r6, #32
30006c5e:	4b15      	ldr	r3, [pc, #84]	; (30006cb4 <BOOT_LoadBL32RDPImg+0xe8>)
30006c60:	429a      	cmp	r2, r3
30006c62:	d108      	bne.n	30006c76 <BOOT_LoadBL32RDPImg+0xaa>
30006c64:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30006c68:	6872      	ldr	r2, [r6, #4]
30006c6a:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30006c6e:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30006c72:	429a      	cmp	r2, r3
30006c74:	d00d      	beq.n	30006c92 <BOOT_LoadBL32RDPImg+0xc6>
30006c76:	4810      	ldr	r0, [pc, #64]	; (30006cb8 <BOOT_LoadBL32RDPImg+0xec>)
30006c78:	f004 ff3a 	bl	3000baf0 <__DiagPrintf_veneer>
30006c7c:	2000      	movs	r0, #0
30006c7e:	b051      	add	sp, #324	; 0x144
30006c80:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006c84:	4621      	mov	r1, r4
30006c86:	462b      	mov	r3, r5
30006c88:	4632      	mov	r2, r6
30006c8a:	480c      	ldr	r0, [pc, #48]	; (30006cbc <BOOT_LoadBL32RDPImg+0xf0>)
30006c8c:	f004 ff30 	bl	3000baf0 <__DiagPrintf_veneer>
30006c90:	e7c1      	b.n	30006c16 <BOOT_LoadBL32RDPImg+0x4a>
30006c92:	480b      	ldr	r0, [pc, #44]	; (30006cc0 <BOOT_LoadBL32RDPImg+0xf4>)
30006c94:	f004 ff2c 	bl	3000baf0 <__DiagPrintf_veneer>
30006c98:	2100      	movs	r1, #0
30006c9a:	4608      	mov	r0, r1
30006c9c:	47d0      	blx	sl
30006c9e:	2001      	movs	r0, #1
30006ca0:	b051      	add	sp, #324	; 0x144
30006ca2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ca6:	bf00      	nop
30006ca8:	3000ca30 	.word	0x3000ca30
30006cac:	00004641 	.word	0x00004641
30006cb0:	2001c00c 	.word	0x2001c00c
30006cb4:	35393138 	.word	0x35393138
30006cb8:	3000ca14 	.word	0x3000ca14
30006cbc:	3000c9ec 	.word	0x3000c9ec
30006cc0:	3000ca20 	.word	0x3000ca20
30006cc4:	3000ca44 	.word	0x3000ca44
30006cc8:	00002429 	.word	0x00002429
30006ccc:	00002479 	.word	0x00002479
30006cd0:	000046f5 	.word	0x000046f5
30006cd4:	00012a1d 	.word	0x00012a1d

30006cd8 <_BOOT_Validate_ImgHash>:
30006cd8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006cdc:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
30006ce0:	4605      	mov	r5, r0
30006ce2:	4690      	mov	r8, r2
30006ce4:	22e0      	movs	r2, #224	; 0xe0
30006ce6:	9103      	str	r1, [sp, #12]
30006ce8:	2100      	movs	r1, #0
30006cea:	a818      	add	r0, sp, #96	; 0x60
30006cec:	461c      	mov	r4, r3
30006cee:	f88d 101f 	strb.w	r1, [sp, #31]
30006cf2:	f004 ff65 	bl	3000bbc0 <____wrap_memset_veneer>
30006cf6:	4628      	mov	r0, r5
30006cf8:	f10d 021f 	add.w	r2, sp, #31
30006cfc:	f10d 011e 	add.w	r1, sp, #30
30006d00:	f002 faf2 	bl	300092e8 <SBOOT_GetMdType>
30006d04:	9001      	str	r0, [sp, #4]
30006d06:	2800      	cmp	r0, #0
30006d08:	d166      	bne.n	30006dd8 <_BOOT_Validate_ImgHash+0x100>
30006d0a:	f89d 301f 	ldrb.w	r3, [sp, #31]
30006d0e:	2b00      	cmp	r3, #0
30006d10:	d067      	beq.n	30006de2 <_BOOT_Validate_ImgHash+0x10a>
30006d12:	2101      	movs	r1, #1
30006d14:	4605      	mov	r5, r0
30006d16:	4b80      	ldr	r3, [pc, #512]	; (30006f18 <_BOOT_Validate_ImgHash+0x240>)
30006d18:	4608      	mov	r0, r1
30006d1a:	4798      	blx	r3
30006d1c:	4629      	mov	r1, r5
30006d1e:	ab18      	add	r3, sp, #96	; 0x60
30006d20:	2220      	movs	r2, #32
30006d22:	f89d 001e 	ldrb.w	r0, [sp, #30]
30006d26:	4d7d      	ldr	r5, [pc, #500]	; (30006f1c <_BOOT_Validate_ImgHash+0x244>)
30006d28:	47a8      	blx	r5
30006d2a:	2800      	cmp	r0, #0
30006d2c:	f040 808b 	bne.w	30006e46 <_BOOT_Validate_ImgHash+0x16e>
30006d30:	b32c      	cbz	r4, 30006d7e <_BOOT_Validate_ImgHash+0xa6>
30006d32:	aad0      	add	r2, sp, #832	; 0x340
30006d34:	1e63      	subs	r3, r4, #1
30006d36:	ae50      	add	r6, sp, #320	; 0x140
30006d38:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 30006f34 <_BOOT_Validate_ImgHash+0x25c>
30006d3c:	f012 0218 	ands.w	r2, r2, #24
30006d40:	b2db      	uxtb	r3, r3
30006d42:	f026 091f 	bic.w	r9, r6, #31
30006d46:	4d76      	ldr	r5, [pc, #472]	; (30006f20 <_BOOT_Validate_ImgHash+0x248>)
30006d48:	9204      	str	r2, [sp, #16]
30006d4a:	f108 0208 	add.w	r2, r8, #8
30006d4e:	bf08      	it	eq
30006d50:	46b1      	moveq	r9, r6
30006d52:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30006d56:	9302      	str	r3, [sp, #8]
30006d58:	e9d8 b700 	ldrd	fp, r7, [r8]
30006d5c:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30006d60:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30006d64:	f0c0 80a7 	bcc.w	30006eb6 <_BOOT_Validate_ImgHash+0x1de>
30006d68:	4639      	mov	r1, r7
30006d6a:	4658      	mov	r0, fp
30006d6c:	aa18      	add	r2, sp, #96	; 0x60
30006d6e:	47d0      	blx	sl
30006d70:	2800      	cmp	r0, #0
30006d72:	d168      	bne.n	30006e46 <_BOOT_Validate_ImgHash+0x16e>
30006d74:	f108 0808 	add.w	r8, r8, #8
30006d78:	9b02      	ldr	r3, [sp, #8]
30006d7a:	4543      	cmp	r3, r8
30006d7c:	d1ec      	bne.n	30006d58 <_BOOT_Validate_ImgHash+0x80>
30006d7e:	ac08      	add	r4, sp, #32
30006d80:	a918      	add	r1, sp, #96	; 0x60
30006d82:	4b68      	ldr	r3, [pc, #416]	; (30006f24 <_BOOT_Validate_ImgHash+0x24c>)
30006d84:	4620      	mov	r0, r4
30006d86:	4798      	blx	r3
30006d88:	4601      	mov	r1, r0
30006d8a:	2800      	cmp	r0, #0
30006d8c:	d15b      	bne.n	30006e46 <_BOOT_Validate_ImgHash+0x16e>
30006d8e:	2001      	movs	r0, #1
30006d90:	4b61      	ldr	r3, [pc, #388]	; (30006f18 <_BOOT_Validate_ImgHash+0x240>)
30006d92:	4798      	blx	r3
30006d94:	f89d 501e 	ldrb.w	r5, [sp, #30]
30006d98:	4b63      	ldr	r3, [pc, #396]	; (30006f28 <_BOOT_Validate_ImgHash+0x250>)
30006d9a:	4798      	blx	r3
30006d9c:	fbb0 f3f5 	udiv	r3, r0, r5
30006da0:	fb03 0315 	mls	r3, r3, r5, r0
30006da4:	b2db      	uxtb	r3, r3
30006da6:	b1bd      	cbz	r5, 30006dd8 <_BOOT_Validate_ImgHash+0x100>
30006da8:	2000      	movs	r0, #0
30006daa:	9f03      	ldr	r7, [sp, #12]
30006dac:	4606      	mov	r6, r0
30006dae:	1c5a      	adds	r2, r3, #1
30006db0:	5cf9      	ldrb	r1, [r7, r3]
30006db2:	5ce3      	ldrb	r3, [r4, r3]
30006db4:	3001      	adds	r0, #1
30006db6:	b2d2      	uxtb	r2, r2
30006db8:	404b      	eors	r3, r1
30006dba:	431e      	orrs	r6, r3
30006dbc:	fbb2 f3f5 	udiv	r3, r2, r5
30006dc0:	fb05 2213 	mls	r2, r5, r3, r2
30006dc4:	b2d3      	uxtb	r3, r2
30006dc6:	b2c2      	uxtb	r2, r0
30006dc8:	4295      	cmp	r5, r2
30006dca:	d8f0      	bhi.n	30006dae <_BOOT_Validate_ImgHash+0xd6>
30006dcc:	2e00      	cmp	r6, #0
30006dce:	9b01      	ldr	r3, [sp, #4]
30006dd0:	bf18      	it	ne
30006dd2:	f06f 0306 	mvnne.w	r3, #6
30006dd6:	9301      	str	r3, [sp, #4]
30006dd8:	9801      	ldr	r0, [sp, #4]
30006dda:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30006dde:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006de2:	a918      	add	r1, sp, #96	; 0x60
30006de4:	f89d 001e 	ldrb.w	r0, [sp, #30]
30006de8:	4b50      	ldr	r3, [pc, #320]	; (30006f2c <_BOOT_Validate_ImgHash+0x254>)
30006dea:	4798      	blx	r3
30006dec:	bb58      	cbnz	r0, 30006e46 <_BOOT_Validate_ImgHash+0x16e>
30006dee:	b31c      	cbz	r4, 30006e38 <_BOOT_Validate_ImgHash+0x160>
30006df0:	aad0      	add	r2, sp, #832	; 0x340
30006df2:	1e63      	subs	r3, r4, #1
30006df4:	ae50      	add	r6, sp, #320	; 0x140
30006df6:	f8df a140 	ldr.w	sl, [pc, #320]	; 30006f38 <_BOOT_Validate_ImgHash+0x260>
30006dfa:	f012 0218 	ands.w	r2, r2, #24
30006dfe:	b2db      	uxtb	r3, r3
30006e00:	f026 091f 	bic.w	r9, r6, #31
30006e04:	4d46      	ldr	r5, [pc, #280]	; (30006f20 <_BOOT_Validate_ImgHash+0x248>)
30006e06:	9204      	str	r2, [sp, #16]
30006e08:	f108 0208 	add.w	r2, r8, #8
30006e0c:	bf08      	it	eq
30006e0e:	46b1      	moveq	r9, r6
30006e10:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30006e14:	9302      	str	r3, [sp, #8]
30006e16:	e9d8 b700 	ldrd	fp, r7, [r8]
30006e1a:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30006e1e:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30006e22:	d318      	bcc.n	30006e56 <_BOOT_Validate_ImgHash+0x17e>
30006e24:	4639      	mov	r1, r7
30006e26:	4658      	mov	r0, fp
30006e28:	aa18      	add	r2, sp, #96	; 0x60
30006e2a:	47d0      	blx	sl
30006e2c:	b958      	cbnz	r0, 30006e46 <_BOOT_Validate_ImgHash+0x16e>
30006e2e:	f108 0808 	add.w	r8, r8, #8
30006e32:	9b02      	ldr	r3, [sp, #8]
30006e34:	4543      	cmp	r3, r8
30006e36:	d1ee      	bne.n	30006e16 <_BOOT_Validate_ImgHash+0x13e>
30006e38:	ac08      	add	r4, sp, #32
30006e3a:	a918      	add	r1, sp, #96	; 0x60
30006e3c:	4b3c      	ldr	r3, [pc, #240]	; (30006f30 <_BOOT_Validate_ImgHash+0x258>)
30006e3e:	4620      	mov	r0, r4
30006e40:	4798      	blx	r3
30006e42:	2800      	cmp	r0, #0
30006e44:	d0a6      	beq.n	30006d94 <_BOOT_Validate_ImgHash+0xbc>
30006e46:	f06f 030a 	mvn.w	r3, #10
30006e4a:	9301      	str	r3, [sp, #4]
30006e4c:	9801      	ldr	r0, [sp, #4]
30006e4e:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30006e52:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e56:	2f00      	cmp	r7, #0
30006e58:	d0e9      	beq.n	30006e2e <_BOOT_Validate_ImgHash+0x156>
30006e5a:	9c04      	ldr	r4, [sp, #16]
30006e5c:	f8cd 8014 	str.w	r8, [sp, #20]
30006e60:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30006e64:	46b8      	mov	r8, r7
30006e66:	4659      	mov	r1, fp
30006e68:	4630      	mov	r0, r6
30006e6a:	bf28      	it	cs
30006e6c:	f44f 7800 	movcs.w	r8, #512	; 0x200
30006e70:	4642      	mov	r2, r8
30006e72:	f7fd fd03 	bl	3000487c <BOOT_ImgCopy>
30006e76:	696a      	ldr	r2, [r5, #20]
30006e78:	03d3      	lsls	r3, r2, #15
30006e7a:	d50f      	bpl.n	30006e9c <_BOOT_Validate_ImgHash+0x1c4>
30006e7c:	eb08 0004 	add.w	r0, r8, r4
30006e80:	f3bf 8f4f 	dsb	sy
30006e84:	4448      	add	r0, r9
30006e86:	464a      	mov	r2, r9
30006e88:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30006e8c:	3220      	adds	r2, #32
30006e8e:	1a83      	subs	r3, r0, r2
30006e90:	2b00      	cmp	r3, #0
30006e92:	dcf9      	bgt.n	30006e88 <_BOOT_Validate_ImgHash+0x1b0>
30006e94:	f3bf 8f4f 	dsb	sy
30006e98:	f3bf 8f6f 	isb	sy
30006e9c:	aa18      	add	r2, sp, #96	; 0x60
30006e9e:	4641      	mov	r1, r8
30006ea0:	4630      	mov	r0, r6
30006ea2:	47d0      	blx	sl
30006ea4:	2800      	cmp	r0, #0
30006ea6:	d1ce      	bne.n	30006e46 <_BOOT_Validate_ImgHash+0x16e>
30006ea8:	ebb7 0708 	subs.w	r7, r7, r8
30006eac:	44c3      	add	fp, r8
30006eae:	d1d7      	bne.n	30006e60 <_BOOT_Validate_ImgHash+0x188>
30006eb0:	f8dd 8014 	ldr.w	r8, [sp, #20]
30006eb4:	e7bb      	b.n	30006e2e <_BOOT_Validate_ImgHash+0x156>
30006eb6:	2f00      	cmp	r7, #0
30006eb8:	f43f af5c 	beq.w	30006d74 <_BOOT_Validate_ImgHash+0x9c>
30006ebc:	9c04      	ldr	r4, [sp, #16]
30006ebe:	f8cd 8014 	str.w	r8, [sp, #20]
30006ec2:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30006ec6:	46b8      	mov	r8, r7
30006ec8:	4659      	mov	r1, fp
30006eca:	4630      	mov	r0, r6
30006ecc:	bf28      	it	cs
30006ece:	f44f 7800 	movcs.w	r8, #512	; 0x200
30006ed2:	4642      	mov	r2, r8
30006ed4:	f7fd fcd2 	bl	3000487c <BOOT_ImgCopy>
30006ed8:	696a      	ldr	r2, [r5, #20]
30006eda:	03d2      	lsls	r2, r2, #15
30006edc:	d50f      	bpl.n	30006efe <_BOOT_Validate_ImgHash+0x226>
30006ede:	eb08 0004 	add.w	r0, r8, r4
30006ee2:	f3bf 8f4f 	dsb	sy
30006ee6:	4448      	add	r0, r9
30006ee8:	464a      	mov	r2, r9
30006eea:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30006eee:	3220      	adds	r2, #32
30006ef0:	1a83      	subs	r3, r0, r2
30006ef2:	2b00      	cmp	r3, #0
30006ef4:	dcf9      	bgt.n	30006eea <_BOOT_Validate_ImgHash+0x212>
30006ef6:	f3bf 8f4f 	dsb	sy
30006efa:	f3bf 8f6f 	isb	sy
30006efe:	aa18      	add	r2, sp, #96	; 0x60
30006f00:	4641      	mov	r1, r8
30006f02:	4630      	mov	r0, r6
30006f04:	47d0      	blx	sl
30006f06:	2800      	cmp	r0, #0
30006f08:	d19d      	bne.n	30006e46 <_BOOT_Validate_ImgHash+0x16e>
30006f0a:	ebb7 0708 	subs.w	r7, r7, r8
30006f0e:	44c3      	add	fp, r8
30006f10:	d1d7      	bne.n	30006ec2 <_BOOT_Validate_ImgHash+0x1ea>
30006f12:	f8dd 8014 	ldr.w	r8, [sp, #20]
30006f16:	e72d      	b.n	30006d74 <_BOOT_Validate_ImgHash+0x9c>
30006f18:	30009325 	.word	0x30009325
30006f1c:	30008c65 	.word	0x30008c65
30006f20:	e000ed00 	.word	0xe000ed00
30006f24:	300093a1 	.word	0x300093a1
30006f28:	0000ea4d 	.word	0x0000ea4d
30006f2c:	300093e1 	.word	0x300093e1
30006f30:	3000945d 	.word	0x3000945d
30006f34:	30009365 	.word	0x30009365
30006f38:	30009421 	.word	0x30009421

30006f3c <BOOT_Validate_ImgHash>:
30006f3c:	b530      	push	{r4, r5, lr}
30006f3e:	b085      	sub	sp, #20
30006f40:	4d0a      	ldr	r5, [pc, #40]	; (30006f6c <BOOT_Validate_ImgHash+0x30>)
30006f42:	4604      	mov	r4, r0
30006f44:	9301      	str	r3, [sp, #4]
30006f46:	e9cd 2102 	strd	r2, r1, [sp, #8]
30006f4a:	47a8      	blx	r5
30006f4c:	9903      	ldr	r1, [sp, #12]
30006f4e:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30006f52:	b128      	cbz	r0, 30006f60 <BOOT_Validate_ImgHash+0x24>
30006f54:	4620      	mov	r0, r4
30006f56:	b005      	add	sp, #20
30006f58:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30006f5c:	f002 bae2 	b.w	30009524 <SBOOT_Validate_ImgHash>
30006f60:	4620      	mov	r0, r4
30006f62:	b005      	add	sp, #20
30006f64:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30006f68:	f7ff beb6 	b.w	30006cd8 <_BOOT_Validate_ImgHash>
30006f6c:	3000927d 	.word	0x3000927d

30006f70 <BOOT_SignatureCheck>:
30006f70:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30006f74:	461e      	mov	r6, r3
30006f76:	b089      	sub	sp, #36	; 0x24
30006f78:	4b79      	ldr	r3, [pc, #484]	; (30007160 <BOOT_SignatureCheck+0x1f0>)
30006f7a:	4680      	mov	r8, r0
30006f7c:	4689      	mov	r9, r1
30006f7e:	4617      	mov	r7, r2
30006f80:	f10d 0c20 	add.w	ip, sp, #32
30006f84:	4c77      	ldr	r4, [pc, #476]	; (30007164 <BOOT_SignatureCheck+0x1f4>)
30006f86:	9d10      	ldr	r5, [sp, #64]	; 0x40
30006f88:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006f8a:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30006f8e:	47a0      	blx	r4
30006f90:	b910      	cbnz	r0, 30006f98 <BOOT_SignatureCheck+0x28>
30006f92:	b009      	add	sp, #36	; 0x24
30006f94:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006f98:	4b73      	ldr	r3, [pc, #460]	; (30007168 <BOOT_SignatureCheck+0x1f8>)
30006f9a:	2000      	movs	r0, #0
30006f9c:	4798      	blx	r3
30006f9e:	f898 3013 	ldrb.w	r3, [r8, #19]
30006fa2:	f898 2012 	ldrb.w	r2, [r8, #18]
30006fa6:	f10d 010f 	add.w	r1, sp, #15
30006faa:	f10d 000e 	add.w	r0, sp, #14
30006fae:	f002 faf3 	bl	30009598 <SBOOT_Validate_Algorithm>
30006fb2:	4604      	mov	r4, r0
30006fb4:	b9e8      	cbnz	r0, 30006ff2 <BOOT_SignatureCheck+0x82>
30006fb6:	69f1      	ldr	r1, [r6, #28]
30006fb8:	b919      	cbnz	r1, 30006fc2 <BOOT_SignatureCheck+0x52>
30006fba:	e0cf      	b.n	3000715c <BOOT_SignatureCheck+0x1ec>
30006fbc:	b2d0      	uxtb	r0, r2
30006fbe:	4288      	cmp	r0, r1
30006fc0:	d208      	bcs.n	30006fd4 <BOOT_SignatureCheck+0x64>
30006fc2:	eb00 04c0 	add.w	r4, r0, r0, lsl #3
30006fc6:	1c42      	adds	r2, r0, #1
30006fc8:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30006fcc:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30006fd0:	42ab      	cmp	r3, r5
30006fd2:	d1f3      	bne.n	30006fbc <BOOT_SignatureCheck+0x4c>
30006fd4:	2224      	movs	r2, #36	; 0x24
30006fd6:	23b4      	movs	r3, #180	; 0xb4
30006fd8:	fb12 3200 	smlabb	r2, r2, r0, r3
30006fdc:	f108 0570 	add.w	r5, r8, #112	; 0x70
30006fe0:	4432      	add	r2, r6
30006fe2:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006fe6:	4629      	mov	r1, r5
30006fe8:	f002 fa58 	bl	3000949c <SBOOT_Validate_PubKey>
30006fec:	4604      	mov	r4, r0
30006fee:	2800      	cmp	r0, #0
30006ff0:	d07a      	beq.n	300070e8 <BOOT_SignatureCheck+0x178>
30006ff2:	f898 3011 	ldrb.w	r3, [r8, #17]
30006ff6:	aa08      	add	r2, sp, #32
30006ff8:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30006ffc:	f853 1c10 	ldr.w	r1, [r3, #-16]
30007000:	4b5a      	ldr	r3, [pc, #360]	; (3000716c <BOOT_SignatureCheck+0x1fc>)
30007002:	2002      	movs	r0, #2
30007004:	4a5a      	ldr	r2, [pc, #360]	; (30007170 <BOOT_SignatureCheck+0x200>)
30007006:	e9cd 1400 	strd	r1, r4, [sp]
3000700a:	4619      	mov	r1, r3
3000700c:	f004 fad2 	bl	3000b5b4 <rtk_log_write>
30007010:	2f00      	cmp	r7, #0
30007012:	d045      	beq.n	300070a0 <BOOT_SignatureCheck+0x130>
30007014:	464d      	mov	r5, r9
30007016:	f107 39ff 	add.w	r9, r7, #4294967295	; 0xffffffff
3000701a:	f04f 0800 	mov.w	r8, #0
3000701e:	4f55      	ldr	r7, [pc, #340]	; (30007174 <BOOT_SignatureCheck+0x204>)
30007020:	fa5f f389 	uxtb.w	r3, r9
30007024:	f105 0908 	add.w	r9, r5, #8
30007028:	1d2e      	adds	r6, r5, #4
3000702a:	4c53      	ldr	r4, [pc, #332]	; (30007178 <BOOT_SignatureCheck+0x208>)
3000702c:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30007030:	e003      	b.n	3000703a <BOOT_SignatureCheck+0xca>
30007032:	3508      	adds	r5, #8
30007034:	3608      	adds	r6, #8
30007036:	45a9      	cmp	r9, r5
30007038:	d032      	beq.n	300070a0 <BOOT_SignatureCheck+0x130>
3000703a:	f856 0c04 	ldr.w	r0, [r6, #-4]
3000703e:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30007042:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30007046:	d3f4      	bcc.n	30007032 <BOOT_SignatureCheck+0xc2>
30007048:	2100      	movs	r1, #0
3000704a:	686a      	ldr	r2, [r5, #4]
3000704c:	47b8      	blx	r7
3000704e:	6963      	ldr	r3, [r4, #20]
30007050:	03d9      	lsls	r1, r3, #15
30007052:	d5ee      	bpl.n	30007032 <BOOT_SignatureCheck+0xc2>
30007054:	f856 2c04 	ldr.w	r2, [r6, #-4]
30007058:	686b      	ldr	r3, [r5, #4]
3000705a:	ea02 0103 	and.w	r1, r2, r3
3000705e:	3101      	adds	r1, #1
30007060:	d022      	beq.n	300070a8 <BOOT_SignatureCheck+0x138>
30007062:	f012 011f 	ands.w	r1, r2, #31
30007066:	d002      	beq.n	3000706e <BOOT_SignatureCheck+0xfe>
30007068:	440b      	add	r3, r1
3000706a:	f022 021f 	bic.w	r2, r2, #31
3000706e:	f3bf 8f4f 	dsb	sy
30007072:	2b00      	cmp	r3, #0
30007074:	dd0c      	ble.n	30007090 <BOOT_SignatureCheck+0x120>
30007076:	1e59      	subs	r1, r3, #1
30007078:	f102 0320 	add.w	r3, r2, #32
3000707c:	f021 011f 	bic.w	r1, r1, #31
30007080:	4419      	add	r1, r3
30007082:	428b      	cmp	r3, r1
30007084:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30007088:	461a      	mov	r2, r3
3000708a:	f103 0320 	add.w	r3, r3, #32
3000708e:	d1f8      	bne.n	30007082 <BOOT_SignatureCheck+0x112>
30007090:	f3bf 8f4f 	dsb	sy
30007094:	f3bf 8f6f 	isb	sy
30007098:	3508      	adds	r5, #8
3000709a:	3608      	adds	r6, #8
3000709c:	45a9      	cmp	r9, r5
3000709e:	d1cc      	bne.n	3000703a <BOOT_SignatureCheck+0xca>
300070a0:	20ff      	movs	r0, #255	; 0xff
300070a2:	b009      	add	sp, #36	; 0x24
300070a4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300070a8:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
300070ac:	f3bf 8f4f 	dsb	sy
300070b0:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
300070b4:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300070b8:	f3c3 304e 	ubfx	r0, r3, #13, #15
300070bc:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
300070c0:	0140      	lsls	r0, r0, #5
300070c2:	ea00 010e 	and.w	r1, r0, lr
300070c6:	4663      	mov	r3, ip
300070c8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300070cc:	3b01      	subs	r3, #1
300070ce:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
300070d2:	1c5a      	adds	r2, r3, #1
300070d4:	d1f8      	bne.n	300070c8 <BOOT_SignatureCheck+0x158>
300070d6:	3820      	subs	r0, #32
300070d8:	f110 0f20 	cmn.w	r0, #32
300070dc:	d1f1      	bne.n	300070c2 <BOOT_SignatureCheck+0x152>
300070de:	f3bf 8f4f 	dsb	sy
300070e2:	f3bf 8f6f 	isb	sy
300070e6:	e7d7      	b.n	30007098 <BOOT_SignatureCheck+0x128>
300070e8:	f508 7380 	add.w	r3, r8, #256	; 0x100
300070ec:	462a      	mov	r2, r5
300070ee:	f89d 100f 	ldrb.w	r1, [sp, #15]
300070f2:	9301      	str	r3, [sp, #4]
300070f4:	f44f 7380 	mov.w	r3, #256	; 0x100
300070f8:	f89d 000e 	ldrb.w	r0, [sp, #14]
300070fc:	9300      	str	r3, [sp, #0]
300070fe:	4643      	mov	r3, r8
30007100:	f002 f9ea 	bl	300094d8 <SBOOT_Validate_Signature>
30007104:	4604      	mov	r4, r0
30007106:	2800      	cmp	r0, #0
30007108:	f47f af73 	bne.w	30006ff2 <BOOT_SignatureCheck+0x82>
3000710c:	f108 0530 	add.w	r5, r8, #48	; 0x30
30007110:	4b1a      	ldr	r3, [pc, #104]	; (3000717c <BOOT_SignatureCheck+0x20c>)
30007112:	f89d 400f 	ldrb.w	r4, [sp, #15]
30007116:	4798      	blx	r3
30007118:	4629      	mov	r1, r5
3000711a:	b9c0      	cbnz	r0, 3000714e <BOOT_SignatureCheck+0x1de>
3000711c:	4620      	mov	r0, r4
3000711e:	463b      	mov	r3, r7
30007120:	464a      	mov	r2, r9
30007122:	f7ff fdd9 	bl	30006cd8 <_BOOT_Validate_ImgHash>
30007126:	4604      	mov	r4, r0
30007128:	f898 3011 	ldrb.w	r3, [r8, #17]
3000712c:	aa08      	add	r2, sp, #32
3000712e:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30007132:	f853 1c10 	ldr.w	r1, [r3, #-16]
30007136:	2c00      	cmp	r4, #0
30007138:	f47f af62 	bne.w	30007000 <BOOT_SignatureCheck+0x90>
3000713c:	4b0b      	ldr	r3, [pc, #44]	; (3000716c <BOOT_SignatureCheck+0x1fc>)
3000713e:	2004      	movs	r0, #4
30007140:	9100      	str	r1, [sp, #0]
30007142:	4a0f      	ldr	r2, [pc, #60]	; (30007180 <BOOT_SignatureCheck+0x210>)
30007144:	4619      	mov	r1, r3
30007146:	f004 fa35 	bl	3000b5b4 <rtk_log_write>
3000714a:	4620      	mov	r0, r4
3000714c:	e721      	b.n	30006f92 <BOOT_SignatureCheck+0x22>
3000714e:	4620      	mov	r0, r4
30007150:	463b      	mov	r3, r7
30007152:	464a      	mov	r2, r9
30007154:	f002 f9e6 	bl	30009524 <SBOOT_Validate_ImgHash>
30007158:	4604      	mov	r4, r0
3000715a:	e7e5      	b.n	30007128 <BOOT_SignatureCheck+0x1b8>
3000715c:	22b4      	movs	r2, #180	; 0xb4
3000715e:	e73d      	b.n	30006fdc <BOOT_SignatureCheck+0x6c>
30007160:	3000dca4 	.word	0x3000dca4
30007164:	0000c149 	.word	0x0000c149
30007168:	30009561 	.word	0x30009561
3000716c:	3000bd58 	.word	0x3000bd58
30007170:	3000ca64 	.word	0x3000ca64
30007174:	00012be5 	.word	0x00012be5
30007178:	e000ed00 	.word	0xe000ed00
3000717c:	3000927d 	.word	0x3000927d
30007180:	3000ca4c 	.word	0x3000ca4c

30007184 <BOOT_CertificateCheck>:
30007184:	b570      	push	{r4, r5, r6, lr}
30007186:	4b2b      	ldr	r3, [pc, #172]	; (30007234 <BOOT_CertificateCheck+0xb0>)
30007188:	b0b0      	sub	sp, #192	; 0xc0
3000718a:	4604      	mov	r4, r0
3000718c:	460d      	mov	r5, r1
3000718e:	4798      	blx	r3
30007190:	b910      	cbnz	r0, 30007198 <BOOT_CertificateCheck+0x14>
30007192:	2000      	movs	r0, #0
30007194:	b030      	add	sp, #192	; 0xc0
30007196:	bd70      	pop	{r4, r5, r6, pc}
30007198:	69a1      	ldr	r1, [r4, #24]
3000719a:	2290      	movs	r2, #144	; 0x90
3000719c:	a80c      	add	r0, sp, #48	; 0x30
3000719e:	4429      	add	r1, r5
300071a0:	f7fd fb6c 	bl	3000487c <BOOT_ImgCopy>
300071a4:	4b24      	ldr	r3, [pc, #144]	; (30007238 <BOOT_CertificateCheck+0xb4>)
300071a6:	4798      	blx	r3
300071a8:	4924      	ldr	r1, [pc, #144]	; (3000723c <BOOT_CertificateCheck+0xb8>)
300071aa:	f000 031f 	and.w	r3, r0, #31
300071ae:	2220      	movs	r2, #32
300071b0:	6808      	ldr	r0, [r1, #0]
300071b2:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
300071b6:	a930      	add	r1, sp, #192	; 0xc0
300071b8:	3a01      	subs	r2, #1
300071ba:	f813 c000 	ldrb.w	ip, [r3, r0]
300071be:	4419      	add	r1, r3
300071c0:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
300071c4:	f103 0301 	add.w	r3, r3, #1
300071c8:	f801 ccb0 	strb.w	ip, [r1, #-176]
300071cc:	f003 031f 	and.w	r3, r3, #31
300071d0:	d1f1      	bne.n	300071b6 <BOOT_CertificateCheck+0x32>
300071d2:	4610      	mov	r0, r2
300071d4:	4b1a      	ldr	r3, [pc, #104]	; (30007240 <BOOT_CertificateCheck+0xbc>)
300071d6:	4798      	blx	r3
300071d8:	7ce3      	ldrb	r3, [r4, #19]
300071da:	7ca2      	ldrb	r2, [r4, #18]
300071dc:	f10d 010f 	add.w	r1, sp, #15
300071e0:	f10d 000e 	add.w	r0, sp, #14
300071e4:	f002 f9d8 	bl	30009598 <SBOOT_Validate_Algorithm>
300071e8:	4605      	mov	r5, r0
300071ea:	b9c0      	cbnz	r0, 3000721e <BOOT_CertificateCheck+0x9a>
300071ec:	f104 0620 	add.w	r6, r4, #32
300071f0:	aa04      	add	r2, sp, #16
300071f2:	f89d 000e 	ldrb.w	r0, [sp, #14]
300071f6:	4631      	mov	r1, r6
300071f8:	f002 f950 	bl	3000949c <SBOOT_Validate_PubKey>
300071fc:	4605      	mov	r5, r0
300071fe:	b970      	cbnz	r0, 3000721e <BOOT_CertificateCheck+0x9a>
30007200:	69a0      	ldr	r0, [r4, #24]
30007202:	4623      	mov	r3, r4
30007204:	ac0c      	add	r4, sp, #48	; 0x30
30007206:	4632      	mov	r2, r6
30007208:	f89d 100f 	ldrb.w	r1, [sp, #15]
3000720c:	e9cd 0400 	strd	r0, r4, [sp]
30007210:	f89d 000e 	ldrb.w	r0, [sp, #14]
30007214:	f002 f960 	bl	300094d8 <SBOOT_Validate_Signature>
30007218:	4605      	mov	r5, r0
3000721a:	2800      	cmp	r0, #0
3000721c:	d0b9      	beq.n	30007192 <BOOT_CertificateCheck+0xe>
3000721e:	4b09      	ldr	r3, [pc, #36]	; (30007244 <BOOT_CertificateCheck+0xc0>)
30007220:	2002      	movs	r0, #2
30007222:	4a09      	ldr	r2, [pc, #36]	; (30007248 <BOOT_CertificateCheck+0xc4>)
30007224:	4619      	mov	r1, r3
30007226:	9500      	str	r5, [sp, #0]
30007228:	f004 f9c4 	bl	3000b5b4 <rtk_log_write>
3000722c:	20ff      	movs	r0, #255	; 0xff
3000722e:	b030      	add	sp, #192	; 0xc0
30007230:	bd70      	pop	{r4, r5, r6, pc}
30007232:	bf00      	nop
30007234:	0000c149 	.word	0x0000c149
30007238:	0000ea4d 	.word	0x0000ea4d
3000723c:	3000dfa4 	.word	0x3000dfa4
30007240:	30009561 	.word	0x30009561
30007244:	3000bd58 	.word	0x3000bd58
30007248:	3000ca88 	.word	0x3000ca88

3000724c <BOOT_RAM_TZCfg>:
3000724c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007250:	b085      	sub	sp, #20
30007252:	4e58      	ldr	r6, [pc, #352]	; (300073b4 <BOOT_RAM_TZCfg+0x168>)
30007254:	4958      	ldr	r1, [pc, #352]	; (300073b8 <BOOT_RAM_TZCfg+0x16c>)
30007256:	2501      	movs	r5, #1
30007258:	f10d 0e08 	add.w	lr, sp, #8
3000725c:	4a57      	ldr	r2, [pc, #348]	; (300073bc <BOOT_RAM_TZCfg+0x170>)
3000725e:	4b58      	ldr	r3, [pc, #352]	; (300073c0 <BOOT_RAM_TZCfg+0x174>)
30007260:	46ec      	mov	ip, sp
30007262:	4677      	mov	r7, lr
30007264:	f8df 8178 	ldr.w	r8, [pc, #376]	; 300073e0 <BOOT_RAM_TZCfg+0x194>
30007268:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
3000726c:	f8df 9174 	ldr.w	r9, [pc, #372]	; 300073e4 <BOOT_RAM_TZCfg+0x198>
30007270:	9202      	str	r2, [sp, #8]
30007272:	9303      	str	r3, [sp, #12]
30007274:	e9cd 6100 	strd	r6, r1, [sp]
30007278:	f85c 2b04 	ldr.w	r2, [ip], #4
3000727c:	2300      	movs	r3, #0
3000727e:	f85e 1b04 	ldr.w	r1, [lr], #4
30007282:	680c      	ldr	r4, [r1, #0]
30007284:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30007288:	fa05 f003 	lsl.w	r0, r5, r3
3000728c:	f1b4 3fff 	cmp.w	r4, #4294967295	; 0xffffffff
30007290:	d00c      	beq.n	300072ac <BOOT_RAM_TZCfg+0x60>
30007292:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30007296:	3301      	adds	r3, #1
30007298:	684c      	ldr	r4, [r1, #4]
3000729a:	310c      	adds	r1, #12
3000729c:	2b08      	cmp	r3, #8
3000729e:	f8cb 4004 	str.w	r4, [fp, #4]
300072a2:	6c14      	ldr	r4, [r2, #64]	; 0x40
300072a4:	ea40 0004 	orr.w	r0, r0, r4
300072a8:	6410      	str	r0, [r2, #64]	; 0x40
300072aa:	d1ea      	bne.n	30007282 <BOOT_RAM_TZCfg+0x36>
300072ac:	42b2      	cmp	r2, r6
300072ae:	d072      	beq.n	30007396 <BOOT_RAM_TZCfg+0x14a>
300072b0:	6455      	str	r5, [r2, #68]	; 0x44
300072b2:	45bc      	cmp	ip, r7
300072b4:	d1e0      	bne.n	30007278 <BOOT_RAM_TZCfg+0x2c>
300072b6:	4b43      	ldr	r3, [pc, #268]	; (300073c4 <BOOT_RAM_TZCfg+0x178>)
300072b8:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
300072bc:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
300072c0:	4841      	ldr	r0, [pc, #260]	; (300073c8 <BOOT_RAM_TZCfg+0x17c>)
300072c2:	6019      	str	r1, [r3, #0]
300072c4:	2501      	movs	r5, #1
300072c6:	605a      	str	r2, [r3, #4]
300072c8:	2100      	movs	r1, #0
300072ca:	4a40      	ldr	r2, [pc, #256]	; (300073cc <BOOT_RAM_TZCfg+0x180>)
300072cc:	4c40      	ldr	r4, [pc, #256]	; (300073d0 <BOOT_RAM_TZCfg+0x184>)
300072ce:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
300072d2:	6813      	ldr	r3, [r2, #0]
300072d4:	3301      	adds	r3, #1
300072d6:	d019      	beq.n	3000730c <BOOT_RAM_TZCfg+0xc0>
300072d8:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
300072dc:	320c      	adds	r2, #12
300072de:	f852 3c0c 	ldr.w	r3, [r2, #-12]
300072e2:	3101      	adds	r1, #1
300072e4:	f023 031f 	bic.w	r3, r3, #31
300072e8:	2908      	cmp	r1, #8
300072ea:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
300072ee:	e952 0302 	ldrd	r0, r3, [r2, #-8]
300072f2:	ea4f 0343 	mov.w	r3, r3, lsl #1
300072f6:	f020 001f 	bic.w	r0, r0, #31
300072fa:	f003 0302 	and.w	r3, r3, #2
300072fe:	ea43 0300 	orr.w	r3, r3, r0
30007302:	f043 0301 	orr.w	r3, r3, #1
30007306:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
3000730a:	d1e2      	bne.n	300072d2 <BOOT_RAM_TZCfg+0x86>
3000730c:	4830      	ldr	r0, [pc, #192]	; (300073d0 <BOOT_RAM_TZCfg+0x184>)
3000730e:	2201      	movs	r2, #1
30007310:	4b30      	ldr	r3, [pc, #192]	; (300073d4 <BOOT_RAM_TZCfg+0x188>)
30007312:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30007316:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000731a:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
3000731e:	d11e      	bne.n	3000735e <BOOT_RAM_TZCfg+0x112>
30007320:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30007324:	f3bf 8f4f 	dsb	sy
30007328:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000732c:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30007330:	f3c5 344e 	ubfx	r4, r5, #13, #15
30007334:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30007338:	0164      	lsls	r4, r4, #5
3000733a:	ea04 0106 	and.w	r1, r4, r6
3000733e:	462b      	mov	r3, r5
30007340:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30007344:	3b01      	subs	r3, #1
30007346:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000734a:	1c5a      	adds	r2, r3, #1
3000734c:	d1f8      	bne.n	30007340 <BOOT_RAM_TZCfg+0xf4>
3000734e:	3c20      	subs	r4, #32
30007350:	f114 0f20 	cmn.w	r4, #32
30007354:	d1f1      	bne.n	3000733a <BOOT_RAM_TZCfg+0xee>
30007356:	f3bf 8f4f 	dsb	sy
3000735a:	f3bf 8f6f 	isb	sy
3000735e:	4b1c      	ldr	r3, [pc, #112]	; (300073d0 <BOOT_RAM_TZCfg+0x184>)
30007360:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30007364:	491c      	ldr	r1, [pc, #112]	; (300073d8 <BOOT_RAM_TZCfg+0x18c>)
30007366:	691c      	ldr	r4, [r3, #16]
30007368:	4a1c      	ldr	r2, [pc, #112]	; (300073dc <BOOT_RAM_TZCfg+0x190>)
3000736a:	f044 0408 	orr.w	r4, r4, #8
3000736e:	611c      	str	r4, [r3, #16]
30007370:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
30007374:	68d8      	ldr	r0, [r3, #12]
30007376:	4028      	ands	r0, r5
30007378:	4301      	orrs	r1, r0
3000737a:	60d9      	str	r1, [r3, #12]
3000737c:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30007380:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30007384:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30007388:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
3000738c:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30007390:	b005      	add	sp, #20
30007392:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007396:	f898 3000 	ldrb.w	r3, [r8]
3000739a:	2b00      	cmp	r3, #0
3000739c:	d189      	bne.n	300072b2 <BOOT_RAM_TZCfg+0x66>
3000739e:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
300073a2:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
300073a6:	6c33      	ldr	r3, [r6, #64]	; 0x40
300073a8:	f043 0380 	orr.w	r3, r3, #128	; 0x80
300073ac:	6433      	str	r3, [r6, #64]	; 0x40
300073ae:	6475      	str	r5, [r6, #68]	; 0x44
300073b0:	e77f      	b.n	300072b2 <BOOT_RAM_TZCfg+0x66>
300073b2:	bf00      	nop
300073b4:	51001a00 	.word	0x51001a00
300073b8:	51001200 	.word	0x51001200
300073bc:	3000e0a4 	.word	0x3000e0a4
300073c0:	3000e044 	.word	0x3000e044
300073c4:	51001b00 	.word	0x51001b00
300073c8:	500e0000 	.word	0x500e0000
300073cc:	3000e104 	.word	0x3000e104
300073d0:	e000ed00 	.word	0xe000ed00
300073d4:	42008000 	.word	0x42008000
300073d8:	05fa6000 	.word	0x05fa6000
300073dc:	e000e100 	.word	0xe000e100
300073e0:	3000df9f 	.word	0x3000df9f
300073e4:	005fffff 	.word	0x005fffff

300073e8 <ChipInfo_Invalid>:
300073e8:	4e06      	ldr	r6, [pc, #24]	; (30007404 <ChipInfo_Invalid+0x1c>)
300073ea:	4d07      	ldr	r5, [pc, #28]	; (30007408 <ChipInfo_Invalid+0x20>)
300073ec:	4c07      	ldr	r4, [pc, #28]	; (3000740c <ChipInfo_Invalid+0x24>)
300073ee:	b508      	push	{r3, lr}
300073f0:	462b      	mov	r3, r5
300073f2:	4622      	mov	r2, r4
300073f4:	4904      	ldr	r1, [pc, #16]	; (30007408 <ChipInfo_Invalid+0x20>)
300073f6:	2002      	movs	r0, #2
300073f8:	f004 f8dc 	bl	3000b5b4 <rtk_log_write>
300073fc:	f242 7010 	movw	r0, #10000	; 0x2710
30007400:	47b0      	blx	r6
30007402:	e7f5      	b.n	300073f0 <ChipInfo_Invalid+0x8>
30007404:	00009be5 	.word	0x00009be5
30007408:	3000cab4 	.word	0x3000cab4
3000740c:	3000cac0 	.word	0x3000cac0

30007410 <ChipInfo_Get>:
30007410:	b570      	push	{r4, r5, r6, lr}
30007412:	4c11      	ldr	r4, [pc, #68]	; (30007458 <ChipInfo_Get+0x48>)
30007414:	7820      	ldrb	r0, [r4, #0]
30007416:	28ff      	cmp	r0, #255	; 0xff
30007418:	d104      	bne.n	30007424 <ChipInfo_Get+0x14>
3000741a:	4d10      	ldr	r5, [pc, #64]	; (3000745c <ChipInfo_Get+0x4c>)
3000741c:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007420:	b108      	cbz	r0, 30007426 <ChipInfo_Get+0x16>
30007422:	7020      	strb	r0, [r4, #0]
30007424:	bd70      	pop	{r4, r5, r6, pc}
30007426:	4621      	mov	r1, r4
30007428:	f240 70ff 	movw	r0, #2047	; 0x7ff
3000742c:	f000 fec4 	bl	300081b8 <OTP_Read8>
30007430:	7820      	ldrb	r0, [r4, #0]
30007432:	28ff      	cmp	r0, #255	; 0xff
30007434:	d002      	beq.n	3000743c <ChipInfo_Get+0x2c>
30007436:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
3000743a:	bd70      	pop	{r4, r5, r6, pc}
3000743c:	4e08      	ldr	r6, [pc, #32]	; (30007460 <ChipInfo_Get+0x50>)
3000743e:	4d09      	ldr	r5, [pc, #36]	; (30007464 <ChipInfo_Get+0x54>)
30007440:	4c09      	ldr	r4, [pc, #36]	; (30007468 <ChipInfo_Get+0x58>)
30007442:	462b      	mov	r3, r5
30007444:	4622      	mov	r2, r4
30007446:	4907      	ldr	r1, [pc, #28]	; (30007464 <ChipInfo_Get+0x54>)
30007448:	2003      	movs	r0, #3
3000744a:	f004 f8b3 	bl	3000b5b4 <rtk_log_write>
3000744e:	f242 7010 	movw	r0, #10000	; 0x2710
30007452:	47b0      	blx	r6
30007454:	e7f5      	b.n	30007442 <ChipInfo_Get+0x32>
30007456:	bf00      	nop
30007458:	3000e688 	.word	0x3000e688
3000745c:	23020000 	.word	0x23020000
30007460:	00009be5 	.word	0x00009be5
30007464:	3000cab4 	.word	0x3000cab4
30007468:	3000cae4 	.word	0x3000cae4

3000746c <ChipInfo_MemoryType>:
3000746c:	b510      	push	{r4, lr}
3000746e:	4c11      	ldr	r4, [pc, #68]	; (300074b4 <ChipInfo_MemoryType+0x48>)
30007470:	47a0      	blx	r4
30007472:	2802      	cmp	r0, #2
30007474:	d019      	beq.n	300074aa <ChipInfo_MemoryType+0x3e>
30007476:	47a0      	blx	r4
30007478:	2803      	cmp	r0, #3
3000747a:	d016      	beq.n	300074aa <ChipInfo_MemoryType+0x3e>
3000747c:	4c0e      	ldr	r4, [pc, #56]	; (300074b8 <ChipInfo_MemoryType+0x4c>)
3000747e:	4b0f      	ldr	r3, [pc, #60]	; (300074bc <ChipInfo_MemoryType+0x50>)
30007480:	4798      	blx	r3
30007482:	2100      	movs	r1, #0
30007484:	23e0      	movs	r3, #224	; 0xe0
30007486:	4622      	mov	r2, r4
30007488:	e004      	b.n	30007494 <ChipInfo_MemoryType+0x28>
3000748a:	7a13      	ldrb	r3, [r2, #8]
3000748c:	3101      	adds	r1, #1
3000748e:	3206      	adds	r2, #6
30007490:	2bff      	cmp	r3, #255	; 0xff
30007492:	d00c      	beq.n	300074ae <ChipInfo_MemoryType+0x42>
30007494:	4298      	cmp	r0, r3
30007496:	d1f8      	bne.n	3000748a <ChipInfo_MemoryType+0x1e>
30007498:	eb01 0141 	add.w	r1, r1, r1, lsl #1
3000749c:	eb04 0141 	add.w	r1, r4, r1, lsl #1
300074a0:	78cb      	ldrb	r3, [r1, #3]
300074a2:	2b02      	cmp	r3, #2
300074a4:	d001      	beq.n	300074aa <ChipInfo_MemoryType+0x3e>
300074a6:	2001      	movs	r0, #1
300074a8:	bd10      	pop	{r4, pc}
300074aa:	2002      	movs	r0, #2
300074ac:	bd10      	pop	{r4, pc}
300074ae:	f7ff ff9b 	bl	300073e8 <ChipInfo_Invalid>
300074b2:	bf00      	nop
300074b4:	0000c0f9 	.word	0x0000c0f9
300074b8:	3000cb10 	.word	0x3000cb10
300074bc:	30007411 	.word	0x30007411

300074c0 <ChipInfo_DDRType>:
300074c0:	b510      	push	{r4, lr}
300074c2:	4b0f      	ldr	r3, [pc, #60]	; (30007500 <ChipInfo_DDRType+0x40>)
300074c4:	4798      	blx	r3
300074c6:	4b0f      	ldr	r3, [pc, #60]	; (30007504 <ChipInfo_DDRType+0x44>)
300074c8:	4604      	mov	r4, r0
300074ca:	4798      	blx	r3
300074cc:	2801      	cmp	r0, #1
300074ce:	d112      	bne.n	300074f6 <ChipInfo_DDRType+0x36>
300074d0:	480d      	ldr	r0, [pc, #52]	; (30007508 <ChipInfo_DDRType+0x48>)
300074d2:	2100      	movs	r1, #0
300074d4:	22e0      	movs	r2, #224	; 0xe0
300074d6:	4603      	mov	r3, r0
300074d8:	e003      	b.n	300074e2 <ChipInfo_DDRType+0x22>
300074da:	789a      	ldrb	r2, [r3, #2]
300074dc:	3101      	adds	r1, #1
300074de:	2aff      	cmp	r2, #255	; 0xff
300074e0:	d00b      	beq.n	300074fa <ChipInfo_DDRType+0x3a>
300074e2:	4294      	cmp	r4, r2
300074e4:	f103 0306 	add.w	r3, r3, #6
300074e8:	d1f7      	bne.n	300074da <ChipInfo_DDRType+0x1a>
300074ea:	eb01 0141 	add.w	r1, r1, r1, lsl #1
300074ee:	eb00 0141 	add.w	r1, r0, r1, lsl #1
300074f2:	78c8      	ldrb	r0, [r1, #3]
300074f4:	bd10      	pop	{r4, pc}
300074f6:	20ff      	movs	r0, #255	; 0xff
300074f8:	bd10      	pop	{r4, pc}
300074fa:	2000      	movs	r0, #0
300074fc:	bd10      	pop	{r4, pc}
300074fe:	bf00      	nop
30007500:	30007411 	.word	0x30007411
30007504:	3000746d 	.word	0x3000746d
30007508:	3000cb10 	.word	0x3000cb10

3000750c <ChipInfo_MemorySize>:
3000750c:	b510      	push	{r4, lr}
3000750e:	4c0b      	ldr	r4, [pc, #44]	; (3000753c <ChipInfo_MemorySize+0x30>)
30007510:	4b0b      	ldr	r3, [pc, #44]	; (30007540 <ChipInfo_MemorySize+0x34>)
30007512:	4798      	blx	r3
30007514:	2100      	movs	r1, #0
30007516:	23e0      	movs	r3, #224	; 0xe0
30007518:	4622      	mov	r2, r4
3000751a:	e004      	b.n	30007526 <ChipInfo_MemorySize+0x1a>
3000751c:	7a13      	ldrb	r3, [r2, #8]
3000751e:	3101      	adds	r1, #1
30007520:	3206      	adds	r2, #6
30007522:	2bff      	cmp	r3, #255	; 0xff
30007524:	d007      	beq.n	30007536 <ChipInfo_MemorySize+0x2a>
30007526:	4298      	cmp	r0, r3
30007528:	d1f8      	bne.n	3000751c <ChipInfo_MemorySize+0x10>
3000752a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
3000752e:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007532:	7908      	ldrb	r0, [r1, #4]
30007534:	bd10      	pop	{r4, pc}
30007536:	f7ff ff57 	bl	300073e8 <ChipInfo_Invalid>
3000753a:	bf00      	nop
3000753c:	3000cb10 	.word	0x3000cb10
30007540:	30007411 	.word	0x30007411

30007544 <ChipInfo_MemoryVendor>:
30007544:	b510      	push	{r4, lr}
30007546:	4c0f      	ldr	r4, [pc, #60]	; (30007584 <ChipInfo_MemoryVendor+0x40>)
30007548:	47a0      	blx	r4
3000754a:	2803      	cmp	r0, #3
3000754c:	d016      	beq.n	3000757c <ChipInfo_MemoryVendor+0x38>
3000754e:	47a0      	blx	r4
30007550:	2802      	cmp	r0, #2
30007552:	d012      	beq.n	3000757a <ChipInfo_MemoryVendor+0x36>
30007554:	4c0c      	ldr	r4, [pc, #48]	; (30007588 <ChipInfo_MemoryVendor+0x44>)
30007556:	4b0d      	ldr	r3, [pc, #52]	; (3000758c <ChipInfo_MemoryVendor+0x48>)
30007558:	4798      	blx	r3
3000755a:	2100      	movs	r1, #0
3000755c:	23e0      	movs	r3, #224	; 0xe0
3000755e:	4622      	mov	r2, r4
30007560:	e004      	b.n	3000756c <ChipInfo_MemoryVendor+0x28>
30007562:	7a13      	ldrb	r3, [r2, #8]
30007564:	3101      	adds	r1, #1
30007566:	3206      	adds	r2, #6
30007568:	2bff      	cmp	r3, #255	; 0xff
3000756a:	d009      	beq.n	30007580 <ChipInfo_MemoryVendor+0x3c>
3000756c:	4298      	cmp	r0, r3
3000756e:	d1f8      	bne.n	30007562 <ChipInfo_MemoryVendor+0x1e>
30007570:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007574:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007578:	7948      	ldrb	r0, [r1, #5]
3000757a:	bd10      	pop	{r4, pc}
3000757c:	2001      	movs	r0, #1
3000757e:	bd10      	pop	{r4, pc}
30007580:	f7ff ff32 	bl	300073e8 <ChipInfo_Invalid>
30007584:	0000c0f9 	.word	0x0000c0f9
30007588:	3000cb10 	.word	0x3000cb10
3000758c:	30007411 	.word	0x30007411

30007590 <SDM32K_Enable>:
30007590:	4a02      	ldr	r2, [pc, #8]	; (3000759c <SDM32K_Enable+0xc>)
30007592:	4b03      	ldr	r3, [pc, #12]	; (300075a0 <SDM32K_Enable+0x10>)
30007594:	6811      	ldr	r1, [r2, #0]
30007596:	430b      	orrs	r3, r1
30007598:	6013      	str	r3, [r2, #0]
3000759a:	4770      	bx	lr
3000759c:	42008e00 	.word	0x42008e00
300075a0:	c0060000 	.word	0xc0060000

300075a4 <CLK_SWITCH_XTAL>:
300075a4:	b508      	push	{r3, lr}
300075a6:	2801      	cmp	r0, #1
300075a8:	4b0a      	ldr	r3, [pc, #40]	; (300075d4 <CLK_SWITCH_XTAL+0x30>)
300075aa:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
300075ae:	d00d      	beq.n	300075cc <CLK_SWITCH_XTAL+0x28>
300075b0:	4b09      	ldr	r3, [pc, #36]	; (300075d8 <CLK_SWITCH_XTAL+0x34>)
300075b2:	4313      	orrs	r3, r2
300075b4:	4907      	ldr	r1, [pc, #28]	; (300075d4 <CLK_SWITCH_XTAL+0x30>)
300075b6:	4a09      	ldr	r2, [pc, #36]	; (300075dc <CLK_SWITCH_XTAL+0x38>)
300075b8:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
300075bc:	4790      	blx	r2
300075be:	4a08      	ldr	r2, [pc, #32]	; (300075e0 <CLK_SWITCH_XTAL+0x3c>)
300075c0:	4b08      	ldr	r3, [pc, #32]	; (300075e4 <CLK_SWITCH_XTAL+0x40>)
300075c2:	fba2 2000 	umull	r2, r0, r2, r0
300075c6:	0c80      	lsrs	r0, r0, #18
300075c8:	6018      	str	r0, [r3, #0]
300075ca:	bd08      	pop	{r3, pc}
300075cc:	4b06      	ldr	r3, [pc, #24]	; (300075e8 <CLK_SWITCH_XTAL+0x44>)
300075ce:	4013      	ands	r3, r2
300075d0:	e7f0      	b.n	300075b4 <CLK_SWITCH_XTAL+0x10>
300075d2:	bf00      	nop
300075d4:	42008000 	.word	0x42008000
300075d8:	80000500 	.word	0x80000500
300075dc:	30008f4d 	.word	0x30008f4d
300075e0:	431bde83 	.word	0x431bde83
300075e4:	2001c700 	.word	0x2001c700
300075e8:	7ffffaff 	.word	0x7ffffaff

300075ec <DDR_PHY_ChipInfo.part.0>:
300075ec:	b508      	push	{r3, lr}
300075ee:	4b24      	ldr	r3, [pc, #144]	; (30007680 <DDR_PHY_ChipInfo.part.0+0x94>)
300075f0:	4798      	blx	r3
300075f2:	280e      	cmp	r0, #14
300075f4:	d80d      	bhi.n	30007612 <DDR_PHY_ChipInfo.part.0+0x26>
300075f6:	2801      	cmp	r0, #1
300075f8:	d912      	bls.n	30007620 <DDR_PHY_ChipInfo.part.0+0x34>
300075fa:	1e83      	subs	r3, r0, #2
300075fc:	2b0c      	cmp	r3, #12
300075fe:	d80f      	bhi.n	30007620 <DDR_PHY_ChipInfo.part.0+0x34>
30007600:	e8df f003 	tbb	[pc, r3]
30007604:	210e1d19 	.word	0x210e1d19
30007608:	2a0e260e 	.word	0x2a0e260e
3000760c:	340e0e2f 	.word	0x340e0e2f
30007610:	39          	.byte	0x39
30007611:	00          	.byte	0x00
30007612:	28e0      	cmp	r0, #224	; 0xe0
30007614:	d104      	bne.n	30007620 <DDR_PHY_ChipInfo.part.0+0x34>
30007616:	2301      	movs	r3, #1
30007618:	4a1a      	ldr	r2, [pc, #104]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
3000761a:	4618      	mov	r0, r3
3000761c:	7013      	strb	r3, [r2, #0]
3000761e:	bd08      	pop	{r3, pc}
30007620:	4b19      	ldr	r3, [pc, #100]	; (30007688 <DDR_PHY_ChipInfo.part.0+0x9c>)
30007622:	2002      	movs	r0, #2
30007624:	4a19      	ldr	r2, [pc, #100]	; (3000768c <DDR_PHY_ChipInfo.part.0+0xa0>)
30007626:	4619      	mov	r1, r3
30007628:	f003 ffc4 	bl	3000b5b4 <rtk_log_write>
3000762c:	2300      	movs	r3, #0
3000762e:	4a15      	ldr	r2, [pc, #84]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
30007630:	4618      	mov	r0, r3
30007632:	7013      	strb	r3, [r2, #0]
30007634:	bd08      	pop	{r3, pc}
30007636:	4b13      	ldr	r3, [pc, #76]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
30007638:	2202      	movs	r2, #2
3000763a:	701a      	strb	r2, [r3, #0]
3000763c:	bd08      	pop	{r3, pc}
3000763e:	4b11      	ldr	r3, [pc, #68]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
30007640:	2203      	movs	r2, #3
30007642:	701a      	strb	r2, [r3, #0]
30007644:	bd08      	pop	{r3, pc}
30007646:	2304      	movs	r3, #4
30007648:	4a0e      	ldr	r2, [pc, #56]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
3000764a:	4618      	mov	r0, r3
3000764c:	7013      	strb	r3, [r2, #0]
3000764e:	bd08      	pop	{r3, pc}
30007650:	4b0c      	ldr	r3, [pc, #48]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
30007652:	2207      	movs	r2, #7
30007654:	701a      	strb	r2, [r3, #0]
30007656:	bd08      	pop	{r3, pc}
30007658:	2305      	movs	r3, #5
3000765a:	4a0a      	ldr	r2, [pc, #40]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
3000765c:	4618      	mov	r0, r3
3000765e:	7013      	strb	r3, [r2, #0]
30007660:	bd08      	pop	{r3, pc}
30007662:	2306      	movs	r3, #6
30007664:	4a07      	ldr	r2, [pc, #28]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
30007666:	4618      	mov	r0, r3
30007668:	7013      	strb	r3, [r2, #0]
3000766a:	bd08      	pop	{r3, pc}
3000766c:	2308      	movs	r3, #8
3000766e:	4a05      	ldr	r2, [pc, #20]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
30007670:	4618      	mov	r0, r3
30007672:	7013      	strb	r3, [r2, #0]
30007674:	bd08      	pop	{r3, pc}
30007676:	2309      	movs	r3, #9
30007678:	4a02      	ldr	r2, [pc, #8]	; (30007684 <DDR_PHY_ChipInfo.part.0+0x98>)
3000767a:	4618      	mov	r0, r3
3000767c:	7013      	strb	r3, [r2, #0]
3000767e:	bd08      	pop	{r3, pc}
30007680:	30007411 	.word	0x30007411
30007684:	3000e689 	.word	0x3000e689
30007688:	3000cb94 	.word	0x3000cb94
3000768c:	3000cb9c 	.word	0x3000cb9c

30007690 <DDR_PHY_ChipInfo_ddrtype>:
30007690:	b510      	push	{r4, lr}
30007692:	4c04      	ldr	r4, [pc, #16]	; (300076a4 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007694:	7820      	ldrb	r0, [r4, #0]
30007696:	b100      	cbz	r0, 3000769a <DDR_PHY_ChipInfo_ddrtype+0xa>
30007698:	bd10      	pop	{r4, pc}
3000769a:	4b03      	ldr	r3, [pc, #12]	; (300076a8 <DDR_PHY_ChipInfo_ddrtype+0x18>)
3000769c:	4798      	blx	r3
3000769e:	7020      	strb	r0, [r4, #0]
300076a0:	bd10      	pop	{r4, pc}
300076a2:	bf00      	nop
300076a4:	3000f390 	.word	0x3000f390
300076a8:	300074c1 	.word	0x300074c1

300076ac <DDR_PHY_StructInit>:
300076ac:	f240 2315 	movw	r3, #533	; 0x215
300076b0:	6583      	str	r3, [r0, #88]	; 0x58
300076b2:	4770      	bx	lr

300076b4 <DDR_PHY_DLL_CLK_DIV>:
300076b4:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
300076b6:	4d70      	ldr	r5, [pc, #448]	; (30007878 <DDR_PHY_DLL_CLK_DIV+0x1c4>)
300076b8:	4c70      	ldr	r4, [pc, #448]	; (3000787c <DDR_PHY_DLL_CLK_DIV+0x1c8>)
300076ba:	fba5 3200 	umull	r3, r2, r5, r0
300076be:	2365      	movs	r3, #101	; 0x65
300076c0:	eb00 0e04 	add.w	lr, r0, r4
300076c4:	0952      	lsrs	r2, r2, #5
300076c6:	fb00 f303 	mul.w	r3, r0, r3
300076ca:	486d      	ldr	r0, [pc, #436]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300076cc:	f1a2 0c03 	sub.w	ip, r2, #3
300076d0:	6a47      	ldr	r7, [r0, #36]	; 0x24
300076d2:	eb0c 018c 	add.w	r1, ip, ip, lsl #2
300076d6:	ea4f 2c0c 	mov.w	ip, ip, lsl #8
300076da:	ebae 0ec1 	sub.w	lr, lr, r1, lsl #3
300076de:	fa1f fc8c 	uxth.w	ip, ip
300076e2:	ea4f 2ece 	mov.w	lr, lr, lsl #11
300076e6:	fba5 1e0e 	umull	r1, lr, r5, lr
300076ea:	4966      	ldr	r1, [pc, #408]	; (30007884 <DDR_PHY_DLL_CLK_DIV+0x1d0>)
300076ec:	fba1 6103 	umull	r6, r1, r1, r3
300076f0:	4e65      	ldr	r6, [pc, #404]	; (30007888 <DDR_PHY_DLL_CLK_DIV+0x1d4>)
300076f2:	0a09      	lsrs	r1, r1, #8
300076f4:	1a52      	subs	r2, r2, r1
300076f6:	3903      	subs	r1, #3
300076f8:	02d2      	lsls	r2, r2, #11
300076fa:	eb02 125e 	add.w	r2, r2, lr, lsr #5
300076fe:	ea06 2ece 	and.w	lr, r6, lr, lsl #11
30007702:	4e62      	ldr	r6, [pc, #392]	; (3000788c <DDR_PHY_DLL_CLK_DIV+0x1d8>)
30007704:	403e      	ands	r6, r7
30007706:	ea4e 0606 	orr.w	r6, lr, r6
3000770a:	6246      	str	r6, [r0, #36]	; 0x24
3000770c:	4e60      	ldr	r6, [pc, #384]	; (30007890 <DDR_PHY_DLL_CLK_DIV+0x1dc>)
3000770e:	fba6 6303 	umull	r6, r3, r6, r3
30007712:	eb04 1353 	add.w	r3, r4, r3, lsr #5
30007716:	6a84      	ldr	r4, [r0, #40]	; 0x28
30007718:	f424 447f 	bic.w	r4, r4, #65280	; 0xff00
3000771c:	ea4c 0404 	orr.w	r4, ip, r4
30007720:	6284      	str	r4, [r0, #40]	; 0x28
30007722:	eb01 0481 	add.w	r4, r1, r1, lsl #2
30007726:	b2c9      	uxtb	r1, r1
30007728:	eba3 03c4 	sub.w	r3, r3, r4, lsl #3
3000772c:	02db      	lsls	r3, r3, #11
3000772e:	fba5 4303 	umull	r4, r3, r5, r3
30007732:	6a45      	ldr	r5, [r0, #36]	; 0x24
30007734:	4c57      	ldr	r4, [pc, #348]	; (30007894 <DDR_PHY_DLL_CLK_DIV+0x1e0>)
30007736:	eba2 1253 	sub.w	r2, r2, r3, lsr #5
3000773a:	f3c3 134a 	ubfx	r3, r3, #5, #11
3000773e:	402c      	ands	r4, r5
30007740:	4d55      	ldr	r5, [pc, #340]	; (30007898 <DDR_PHY_DLL_CLK_DIV+0x1e4>)
30007742:	eb02 1242 	add.w	r2, r2, r2, lsl #5
30007746:	4323      	orrs	r3, r4
30007748:	02d2      	lsls	r2, r2, #11
3000774a:	6243      	str	r3, [r0, #36]	; 0x24
3000774c:	6a84      	ldr	r4, [r0, #40]	; 0x28
3000774e:	fba5 3202 	umull	r3, r2, r5, r2
30007752:	4b52      	ldr	r3, [pc, #328]	; (3000789c <DDR_PHY_DLL_CLK_DIV+0x1e8>)
30007754:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30007758:	4321      	orrs	r1, r4
3000775a:	2401      	movs	r4, #1
3000775c:	6281      	str	r1, [r0, #40]	; 0x28
3000775e:	6a01      	ldr	r1, [r0, #32]
30007760:	400b      	ands	r3, r1
30007762:	494f      	ldr	r1, [pc, #316]	; (300078a0 <DDR_PHY_DLL_CLK_DIV+0x1ec>)
30007764:	ea43 3292 	orr.w	r2, r3, r2, lsr #14
30007768:	6202      	str	r2, [r0, #32]
3000776a:	6784      	str	r4, [r0, #120]	; 0x78
3000776c:	7808      	ldrb	r0, [r1, #0]
3000776e:	28ff      	cmp	r0, #255	; 0xff
30007770:	d075      	beq.n	3000785e <DDR_PHY_DLL_CLK_DIV+0x1aa>
30007772:	4c4c      	ldr	r4, [pc, #304]	; (300078a4 <DDR_PHY_DLL_CLK_DIV+0x1f0>)
30007774:	0145      	lsls	r5, r0, #5
30007776:	4b42      	ldr	r3, [pc, #264]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007778:	eb04 1040 	add.w	r0, r4, r0, lsl #5
3000777c:	5962      	ldr	r2, [r4, r5]
3000777e:	4e4a      	ldr	r6, [pc, #296]	; (300078a8 <DDR_PHY_DLL_CLK_DIV+0x1f4>)
30007780:	611a      	str	r2, [r3, #16]
30007782:	6842      	ldr	r2, [r0, #4]
30007784:	615a      	str	r2, [r3, #20]
30007786:	6882      	ldr	r2, [r0, #8]
30007788:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
3000778c:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
30007790:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
30007794:	609a      	str	r2, [r3, #8]
30007796:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
3000779a:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
3000779e:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300077a2:	7830      	ldrb	r0, [r6, #0]
300077a4:	2800      	cmp	r0, #0
300077a6:	d056      	beq.n	30007856 <DDR_PHY_DLL_CLK_DIV+0x1a2>
300077a8:	2801      	cmp	r0, #1
300077aa:	442c      	add	r4, r5
300077ac:	4b34      	ldr	r3, [pc, #208]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300077ae:	d037      	beq.n	30007820 <DDR_PHY_DLL_CLK_DIV+0x16c>
300077b0:	2222      	movs	r2, #34	; 0x22
300077b2:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
300077b6:	6962      	ldr	r2, [r4, #20]
300077b8:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
300077bc:	e9d4 1206 	ldrd	r1, r2, [r4, #24]
300077c0:	f8c3 1084 	str.w	r1, [r3, #132]	; 0x84
300077c4:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
300077c8:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300077cc:	4937      	ldr	r1, [pc, #220]	; (300078ac <DDR_PHY_DLL_CLK_DIV+0x1f8>)
300077ce:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
300077d2:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300077d6:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300077da:	009b      	lsls	r3, r3, #2
300077dc:	d516      	bpl.n	3000780c <DDR_PHY_DLL_CLK_DIV+0x158>
300077de:	2800      	cmp	r0, #0
300077e0:	d044      	beq.n	3000786c <DDR_PHY_DLL_CLK_DIV+0x1b8>
300077e2:	4b27      	ldr	r3, [pc, #156]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300077e4:	4a32      	ldr	r2, [pc, #200]	; (300078b0 <DDR_PHY_DLL_CLK_DIV+0x1fc>)
300077e6:	601a      	str	r2, [r3, #0]
300077e8:	4b25      	ldr	r3, [pc, #148]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300077ea:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300077ee:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
300077f2:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
300077f6:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300077fa:	699a      	ldr	r2, [r3, #24]
300077fc:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
30007800:	619a      	str	r2, [r3, #24]
30007802:	699a      	ldr	r2, [r3, #24]
30007804:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30007808:	619a      	str	r2, [r3, #24]
3000780a:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
3000780c:	b920      	cbnz	r0, 30007818 <DDR_PHY_DLL_CLK_DIV+0x164>
3000780e:	4b29      	ldr	r3, [pc, #164]	; (300078b4 <DDR_PHY_DLL_CLK_DIV+0x200>)
30007810:	4798      	blx	r3
30007812:	2801      	cmp	r0, #1
30007814:	7030      	strb	r0, [r6, #0]
30007816:	d025      	beq.n	30007864 <DDR_PHY_DLL_CLK_DIV+0x1b0>
30007818:	4b19      	ldr	r3, [pc, #100]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000781a:	4a27      	ldr	r2, [pc, #156]	; (300078b8 <DDR_PHY_DLL_CLK_DIV+0x204>)
3000781c:	601a      	str	r2, [r3, #0]
3000781e:	e7e3      	b.n	300077e8 <DDR_PHY_DLL_CLK_DIV+0x134>
30007820:	2233      	movs	r2, #51	; 0x33
30007822:	6965      	ldr	r5, [r4, #20]
30007824:	4921      	ldr	r1, [pc, #132]	; (300078ac <DDR_PHY_DLL_CLK_DIV+0x1f8>)
30007826:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
3000782a:	f8c3 5080 	str.w	r5, [r3, #128]	; 0x80
3000782e:	e9d4 0206 	ldrd	r0, r2, [r4, #24]
30007832:	f8c3 0084 	str.w	r0, [r3, #132]	; 0x84
30007836:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000783a:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000783e:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
30007842:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30007846:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
3000784a:	009a      	lsls	r2, r3, #2
3000784c:	d50a      	bpl.n	30007864 <DDR_PHY_DLL_CLK_DIV+0x1b0>
3000784e:	4b0c      	ldr	r3, [pc, #48]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007850:	4a1a      	ldr	r2, [pc, #104]	; (300078bc <DDR_PHY_DLL_CLK_DIV+0x208>)
30007852:	601a      	str	r2, [r3, #0]
30007854:	e7c8      	b.n	300077e8 <DDR_PHY_DLL_CLK_DIV+0x134>
30007856:	4b17      	ldr	r3, [pc, #92]	; (300078b4 <DDR_PHY_DLL_CLK_DIV+0x200>)
30007858:	4798      	blx	r3
3000785a:	7030      	strb	r0, [r6, #0]
3000785c:	e7a4      	b.n	300077a8 <DDR_PHY_DLL_CLK_DIV+0xf4>
3000785e:	f7ff fec5 	bl	300075ec <DDR_PHY_ChipInfo.part.0>
30007862:	e786      	b.n	30007772 <DDR_PHY_DLL_CLK_DIV+0xbe>
30007864:	4b06      	ldr	r3, [pc, #24]	; (30007880 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007866:	4a16      	ldr	r2, [pc, #88]	; (300078c0 <DDR_PHY_DLL_CLK_DIV+0x20c>)
30007868:	601a      	str	r2, [r3, #0]
3000786a:	e7bd      	b.n	300077e8 <DDR_PHY_DLL_CLK_DIV+0x134>
3000786c:	4b11      	ldr	r3, [pc, #68]	; (300078b4 <DDR_PHY_DLL_CLK_DIV+0x200>)
3000786e:	4798      	blx	r3
30007870:	2801      	cmp	r0, #1
30007872:	7030      	strb	r0, [r6, #0]
30007874:	d1b5      	bne.n	300077e2 <DDR_PHY_DLL_CLK_DIV+0x12e>
30007876:	e7ea      	b.n	3000784e <DDR_PHY_DLL_CLK_DIV+0x19a>
30007878:	cccccccd 	.word	0xcccccccd
3000787c:	001fff88 	.word	0x001fff88
30007880:	41011000 	.word	0x41011000
30007884:	10624dd3 	.word	0x10624dd3
30007888:	07ff0000 	.word	0x07ff0000
3000788c:	f800ffff 	.word	0xf800ffff
30007890:	51eb851f 	.word	0x51eb851f
30007894:	fffff800 	.word	0xfffff800
30007898:	d1b71759 	.word	0xd1b71759
3000789c:	fff80000 	.word	0xfff80000
300078a0:	3000e689 	.word	0x3000e689
300078a4:	3000cbbc 	.word	0x3000cbbc
300078a8:	3000f390 	.word	0x3000f390
300078ac:	42008000 	.word	0x42008000
300078b0:	4c000821 	.word	0x4c000821
300078b4:	300074c1 	.word	0x300074c1
300078b8:	40000021 	.word	0x40000021
300078bc:	4c000903 	.word	0x4c000903
300078c0:	40000103 	.word	0x40000103

300078c4 <DDR_PHY_CRT_Init>:
300078c4:	4a32      	ldr	r2, [pc, #200]	; (30007990 <DDR_PHY_CRT_Init+0xcc>)
300078c6:	2005      	movs	r0, #5
300078c8:	b538      	push	{r3, r4, r5, lr}
300078ca:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
300078ce:	4c31      	ldr	r4, [pc, #196]	; (30007994 <DDR_PHY_CRT_Init+0xd0>)
300078d0:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300078d4:	4d30      	ldr	r5, [pc, #192]	; (30007998 <DDR_PHY_CRT_Init+0xd4>)
300078d6:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
300078da:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300078de:	f043 0301 	orr.w	r3, r3, #1
300078e2:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300078e6:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300078ea:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300078ee:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300078f2:	68e3      	ldr	r3, [r4, #12]
300078f4:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
300078f8:	60e3      	str	r3, [r4, #12]
300078fa:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300078fe:	f043 0302 	orr.w	r3, r3, #2
30007902:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007906:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000790a:	f023 0308 	bic.w	r3, r3, #8
3000790e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007912:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007916:	f043 0304 	orr.w	r3, r3, #4
3000791a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000791e:	47a8      	blx	r5
30007920:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007924:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30007928:	f043 0308 	orr.w	r3, r3, #8
3000792c:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007930:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30007932:	f043 6380 	orr.w	r3, r3, #67108864	; 0x4000000
30007936:	62a3      	str	r3, [r4, #40]	; 0x28
30007938:	47a8      	blx	r5
3000793a:	6aa2      	ldr	r2, [r4, #40]	; 0x28
3000793c:	f641 73ff 	movw	r3, #8191	; 0x1fff
30007940:	4916      	ldr	r1, [pc, #88]	; (3000799c <DDR_PHY_CRT_Init+0xd8>)
30007942:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
30007946:	62a2      	str	r2, [r4, #40]	; 0x28
30007948:	69e2      	ldr	r2, [r4, #28]
3000794a:	f042 0202 	orr.w	r2, r2, #2
3000794e:	61e2      	str	r2, [r4, #28]
30007950:	69e2      	ldr	r2, [r4, #28]
30007952:	f042 0208 	orr.w	r2, r2, #8
30007956:	61e2      	str	r2, [r4, #28]
30007958:	68a2      	ldr	r2, [r4, #8]
3000795a:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
3000795e:	60a2      	str	r2, [r4, #8]
30007960:	6862      	ldr	r2, [r4, #4]
30007962:	4313      	orrs	r3, r2
30007964:	6063      	str	r3, [r4, #4]
30007966:	68a3      	ldr	r3, [r4, #8]
30007968:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
3000796c:	60a3      	str	r3, [r4, #8]
3000796e:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
30007972:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007976:	f023 0303 	bic.w	r3, r3, #3
3000797a:	f043 0302 	orr.w	r3, r3, #2
3000797e:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007982:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007986:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000798a:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000798e:	bd38      	pop	{r3, r4, r5, pc}
30007990:	42008000 	.word	0x42008000
30007994:	41011000 	.word	0x41011000
30007998:	00009b2d 	.word	0x00009b2d
3000799c:	90161f00 	.word	0x90161f00

300079a0 <DDR_PHY_R240_ZQ_CAL>:
300079a0:	b538      	push	{r3, r4, r5, lr}
300079a2:	4b1e      	ldr	r3, [pc, #120]	; (30007a1c <DDR_PHY_R240_ZQ_CAL+0x7c>)
300079a4:	7818      	ldrb	r0, [r3, #0]
300079a6:	28ff      	cmp	r0, #255	; 0xff
300079a8:	d034      	beq.n	30007a14 <DDR_PHY_R240_ZQ_CAL+0x74>
300079aa:	4a1d      	ldr	r2, [pc, #116]	; (30007a20 <DDR_PHY_R240_ZQ_CAL+0x80>)
300079ac:	eb00 0080 	add.w	r0, r0, r0, lsl #2
300079b0:	4b1c      	ldr	r3, [pc, #112]	; (30007a24 <DDR_PHY_R240_ZQ_CAL+0x84>)
300079b2:	2400      	movs	r4, #0
300079b4:	eb02 0180 	add.w	r1, r2, r0, lsl #2
300079b8:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
300079bc:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
300079c0:	7c0a      	ldrb	r2, [r1, #16]
300079c2:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
300079c6:	0052      	lsls	r2, r2, #1
300079c8:	f002 023e 	and.w	r2, r2, #62	; 0x3e
300079cc:	4302      	orrs	r2, r0
300079ce:	6848      	ldr	r0, [r1, #4]
300079d0:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
300079d4:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
300079d8:	4a13      	ldr	r2, [pc, #76]	; (30007a28 <DDR_PHY_R240_ZQ_CAL+0x88>)
300079da:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
300079de:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300079e2:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
300079e6:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
300079ea:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
300079ee:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300079f2:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
300079f6:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300079fa:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300079fe:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
30007a02:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30007a06:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30007a0a:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
30007a0e:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30007a12:	bd38      	pop	{r3, r4, r5, pc}
30007a14:	f7ff fdea 	bl	300075ec <DDR_PHY_ChipInfo.part.0>
30007a18:	e7c7      	b.n	300079aa <DDR_PHY_R240_ZQ_CAL+0xa>
30007a1a:	bf00      	nop
30007a1c:	3000e689 	.word	0x3000e689
30007a20:	3000ccfc 	.word	0x3000ccfc
30007a24:	41011000 	.word	0x41011000
30007a28:	00262626 	.word	0x00262626

30007a2c <DDR_PHY_ZQ_SET_SEL>:
30007a2c:	b410      	push	{r4}
30007a2e:	4b25      	ldr	r3, [pc, #148]	; (30007ac4 <DDR_PHY_ZQ_SET_SEL+0x98>)
30007a30:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30007a34:	4a24      	ldr	r2, [pc, #144]	; (30007ac8 <DDR_PHY_ZQ_SET_SEL+0x9c>)
30007a36:	4c25      	ldr	r4, [pc, #148]	; (30007acc <DDR_PHY_ZQ_SET_SEL+0xa0>)
30007a38:	4925      	ldr	r1, [pc, #148]	; (30007ad0 <DDR_PHY_ZQ_SET_SEL+0xa4>)
30007a3a:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
30007a3e:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
30007a42:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
30007a46:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
30007a4a:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
30007a4e:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
30007a52:	f64f 74ff 	movw	r4, #65535	; 0xffff
30007a56:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
30007a5a:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
30007a5e:	2200      	movs	r2, #0
30007a60:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30007a64:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
30007a68:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30007a6c:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
30007a70:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30007a74:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30007a78:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30007a7c:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
30007a80:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30007a84:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30007a88:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30007a8c:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
30007a90:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30007a94:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30007a98:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30007a9c:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
30007aa0:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
30007aa4:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
30007aa8:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30007aac:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
30007ab0:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
30007ab4:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
30007ab8:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30007abc:	f85d 4b04 	ldr.w	r4, [sp], #4
30007ac0:	4770      	bx	lr
30007ac2:	bf00      	nop
30007ac4:	41011000 	.word	0x41011000
30007ac8:	ffff1111 	.word	0xffff1111
30007acc:	ffff3333 	.word	0xffff3333
30007ad0:	ffff0000 	.word	0xffff0000

30007ad4 <DDR_PHY_DELAY_TAP_SET>:
30007ad4:	b510      	push	{r4, lr}
30007ad6:	4b15      	ldr	r3, [pc, #84]	; (30007b2c <DDR_PHY_DELAY_TAP_SET+0x58>)
30007ad8:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
30007adc:	4914      	ldr	r1, [pc, #80]	; (30007b30 <DDR_PHY_DELAY_TAP_SET+0x5c>)
30007ade:	4c15      	ldr	r4, [pc, #84]	; (30007b34 <DDR_PHY_DELAY_TAP_SET+0x60>)
30007ae0:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
30007ae4:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
30007ae8:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30007aec:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
30007af0:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30007af4:	7820      	ldrb	r0, [r4, #0]
30007af6:	b1a0      	cbz	r0, 30007b22 <DDR_PHY_DELAY_TAP_SET+0x4e>
30007af8:	2801      	cmp	r0, #1
30007afa:	4b0c      	ldr	r3, [pc, #48]	; (30007b2c <DDR_PHY_DELAY_TAP_SET+0x58>)
30007afc:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
30007b00:	bf0c      	ite	eq
30007b02:	4a0d      	ldreq	r2, [pc, #52]	; (30007b38 <DDR_PHY_DELAY_TAP_SET+0x64>)
30007b04:	4a0d      	ldrne	r2, [pc, #52]	; (30007b3c <DDR_PHY_DELAY_TAP_SET+0x68>)
30007b06:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
30007b0a:	f640 0288 	movw	r2, #2184	; 0x888
30007b0e:	4b07      	ldr	r3, [pc, #28]	; (30007b2c <DDR_PHY_DELAY_TAP_SET+0x58>)
30007b10:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30007b14:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30007b18:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30007b1c:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30007b20:	bd10      	pop	{r4, pc}
30007b22:	4b07      	ldr	r3, [pc, #28]	; (30007b40 <DDR_PHY_DELAY_TAP_SET+0x6c>)
30007b24:	4798      	blx	r3
30007b26:	7020      	strb	r0, [r4, #0]
30007b28:	e7e6      	b.n	30007af8 <DDR_PHY_DELAY_TAP_SET+0x24>
30007b2a:	bf00      	nop
30007b2c:	41011000 	.word	0x41011000
30007b30:	20888888 	.word	0x20888888
30007b34:	3000f390 	.word	0x3000f390
30007b38:	7c033333 	.word	0x7c033333
30007b3c:	7c011111 	.word	0x7c011111
30007b40:	300074c1 	.word	0x300074c1

30007b44 <DDR_PHY_READ_CTRL>:
30007b44:	4b10      	ldr	r3, [pc, #64]	; (30007b88 <DDR_PHY_READ_CTRL+0x44>)
30007b46:	b510      	push	{r4, lr}
30007b48:	7818      	ldrb	r0, [r3, #0]
30007b4a:	28ff      	cmp	r0, #255	; 0xff
30007b4c:	d019      	beq.n	30007b82 <DDR_PHY_READ_CTRL+0x3e>
30007b4e:	4a0f      	ldr	r2, [pc, #60]	; (30007b8c <DDR_PHY_READ_CTRL+0x48>)
30007b50:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30007b54:	4b0e      	ldr	r3, [pc, #56]	; (30007b90 <DDR_PHY_READ_CTRL+0x4c>)
30007b56:	2108      	movs	r1, #8
30007b58:	eb02 0080 	add.w	r0, r2, r0, lsl #2
30007b5c:	22fe      	movs	r2, #254	; 0xfe
30007b5e:	7c44      	ldrb	r4, [r0, #17]
30007b60:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
30007b64:	7c84      	ldrb	r4, [r0, #18]
30007b66:	7cc0      	ldrb	r0, [r0, #19]
30007b68:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
30007b6c:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
30007b70:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30007b74:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
30007b78:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30007b7c:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
30007b80:	bd10      	pop	{r4, pc}
30007b82:	f7ff fd33 	bl	300075ec <DDR_PHY_ChipInfo.part.0>
30007b86:	e7e2      	b.n	30007b4e <DDR_PHY_READ_CTRL+0xa>
30007b88:	3000e689 	.word	0x3000e689
30007b8c:	3000ccfc 	.word	0x3000ccfc
30007b90:	41011000 	.word	0x41011000

30007b94 <DDR_PHY_READ_LEVELING>:
30007b94:	b508      	push	{r3, lr}
30007b96:	4b21      	ldr	r3, [pc, #132]	; (30007c1c <DDR_PHY_READ_LEVELING+0x88>)
30007b98:	7818      	ldrb	r0, [r3, #0]
30007b9a:	28ff      	cmp	r0, #255	; 0xff
30007b9c:	d03b      	beq.n	30007c16 <DDR_PHY_READ_LEVELING+0x82>
30007b9e:	4b20      	ldr	r3, [pc, #128]	; (30007c20 <DDR_PHY_READ_LEVELING+0x8c>)
30007ba0:	b1e8      	cbz	r0, 30007bde <DDR_PHY_READ_LEVELING+0x4a>
30007ba2:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
30007ba6:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30007baa:	220f      	movs	r2, #15
30007bac:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
30007bb0:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
30007bb4:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
30007bb8:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
30007bbc:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
30007bc0:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
30007bc4:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
30007bc8:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
30007bcc:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
30007bd0:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30007bd4:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30007bd8:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
30007bdc:	bd08      	pop	{r3, pc}
30007bde:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
30007be2:	2110      	movs	r1, #16
30007be4:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30007be8:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30007bec:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30007bf0:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30007bf4:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30007bf8:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30007bfc:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30007c00:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30007c04:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30007c08:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30007c0c:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30007c10:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30007c14:	bd08      	pop	{r3, pc}
30007c16:	f7ff fce9 	bl	300075ec <DDR_PHY_ChipInfo.part.0>
30007c1a:	e7c0      	b.n	30007b9e <DDR_PHY_READ_LEVELING+0xa>
30007c1c:	3000e689 	.word	0x3000e689
30007c20:	41011000 	.word	0x41011000

30007c24 <DDR_PHY_Init>:
30007c24:	b510      	push	{r4, lr}
30007c26:	6d80      	ldr	r0, [r0, #88]	; 0x58
30007c28:	4c2f      	ldr	r4, [pc, #188]	; (30007ce8 <DDR_PHY_Init+0xc4>)
30007c2a:	f7ff fd43 	bl	300076b4 <DDR_PHY_DLL_CLK_DIV>
30007c2e:	f7ff fe49 	bl	300078c4 <DDR_PHY_CRT_Init>
30007c32:	f7ff feb5 	bl	300079a0 <DDR_PHY_R240_ZQ_CAL>
30007c36:	f7ff fef9 	bl	30007a2c <DDR_PHY_ZQ_SET_SEL>
30007c3a:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30007c3e:	2300      	movs	r3, #0
30007c40:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30007c44:	4829      	ldr	r0, [pc, #164]	; (30007cec <DDR_PHY_Init+0xc8>)
30007c46:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30007c4a:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
30007c4e:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
30007c52:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
30007c56:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30007c5a:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
30007c5e:	f7ff ff39 	bl	30007ad4 <DDR_PHY_DELAY_TAP_SET>
30007c62:	f7ff ff6f 	bl	30007b44 <DDR_PHY_READ_CTRL>
30007c66:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30007c6a:	4a21      	ldr	r2, [pc, #132]	; (30007cf0 <DDR_PHY_Init+0xcc>)
30007c6c:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
30007c70:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30007c74:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30007c78:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30007c7c:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30007c80:	f7ff ff88 	bl	30007b94 <DDR_PHY_READ_LEVELING>
30007c84:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30007c88:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30007c8c:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30007c90:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30007c94:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30007c98:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30007c9c:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30007ca0:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30007ca4:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30007ca8:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30007cac:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30007cb0:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30007cb4:	f043 0303 	orr.w	r3, r3, #3
30007cb8:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30007cbc:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30007cc0:	f043 030c 	orr.w	r3, r3, #12
30007cc4:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30007cc8:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007ccc:	f023 0303 	bic.w	r3, r3, #3
30007cd0:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007cd4:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007cd8:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
30007cdc:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30007ce0:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007ce4:	bd10      	pop	{r4, pc}
30007ce6:	bf00      	nop
30007ce8:	41011000 	.word	0x41011000
30007cec:	00045500 	.word	0x00045500
30007cf0:	11117777 	.word	0x11117777

30007cf4 <DDR_PHY_CAL>:
30007cf4:	4770      	bx	lr
30007cf6:	bf00      	nop

30007cf8 <DDR_PHY_AutoGating>:
30007cf8:	4a0c      	ldr	r2, [pc, #48]	; (30007d2c <DDR_PHY_AutoGating+0x34>)
30007cfa:	2032      	movs	r0, #50	; 0x32
30007cfc:	490c      	ldr	r1, [pc, #48]	; (30007d30 <DDR_PHY_AutoGating+0x38>)
30007cfe:	b508      	push	{r3, lr}
30007d00:	6893      	ldr	r3, [r2, #8]
30007d02:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30007d06:	6093      	str	r3, [r2, #8]
30007d08:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30007d0c:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30007d10:	f043 0314 	orr.w	r3, r3, #20
30007d14:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30007d18:	4788      	blx	r1
30007d1a:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30007d1e:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30007d22:	f043 0303 	orr.w	r3, r3, #3
30007d26:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30007d2a:	bd08      	pop	{r3, pc}
30007d2c:	41011000 	.word	0x41011000
30007d30:	00009b2d 	.word	0x00009b2d

30007d34 <flash_init_userdef>:
30007d34:	4b14      	ldr	r3, [pc, #80]	; (30007d88 <flash_init_userdef+0x54>)
30007d36:	2201      	movs	r2, #1
30007d38:	2140      	movs	r1, #64	; 0x40
30007d3a:	f44f 6000 	mov.w	r0, #2048	; 0x800
30007d3e:	b430      	push	{r4, r5}
30007d40:	e9c3 1203 	strd	r1, r2, [r3, #12]
30007d44:	2502      	movs	r5, #2
30007d46:	2400      	movs	r4, #0
30007d48:	f640 0104 	movw	r1, #2052	; 0x804
30007d4c:	601a      	str	r2, [r3, #0]
30007d4e:	22bb      	movs	r2, #187	; 0xbb
30007d50:	83d8      	strh	r0, [r3, #30]
30007d52:	8419      	strh	r1, [r3, #32]
30007d54:	203b      	movs	r0, #59	; 0x3b
30007d56:	21eb      	movs	r1, #235	; 0xeb
30007d58:	629a      	str	r2, [r3, #40]	; 0x28
30007d5a:	4a0c      	ldr	r2, [pc, #48]	; (30007d8c <flash_init_userdef+0x58>)
30007d5c:	6258      	str	r0, [r3, #36]	; 0x24
30007d5e:	6319      	str	r1, [r3, #48]	; 0x30
30007d60:	f64a 3120 	movw	r1, #43808	; 0xab20
30007d64:	655a      	str	r2, [r3, #84]	; 0x54
30007d66:	22b9      	movs	r2, #185	; 0xb9
30007d68:	4809      	ldr	r0, [pc, #36]	; (30007d90 <flash_init_userdef+0x5c>)
30007d6a:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30007d6e:	6598      	str	r0, [r3, #88]	; 0x58
30007d70:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30007d74:	e9c3 5405 	strd	r5, r4, [r3, #20]
30007d78:	2406      	movs	r4, #6
30007d7a:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30007d7e:	246b      	movs	r4, #107	; 0x6b
30007d80:	62dc      	str	r4, [r3, #44]	; 0x2c
30007d82:	bc30      	pop	{r4, r5}
30007d84:	4770      	bx	lr
30007d86:	bf00      	nop
30007d88:	2001c01c 	.word	0x2001c01c
30007d8c:	00059f06 	.word	0x00059f06
30007d90:	d8600001 	.word	0xd8600001

30007d94 <nand_init_userdef>:
30007d94:	4b18      	ldr	r3, [pc, #96]	; (30007df8 <nand_init_userdef+0x64>)
30007d96:	2001      	movs	r0, #1
30007d98:	2200      	movs	r2, #0
30007d9a:	2102      	movs	r1, #2
30007d9c:	b410      	push	{r4}
30007d9e:	6018      	str	r0, [r3, #0]
30007da0:	f640 0408 	movw	r4, #2056	; 0x808
30007da4:	6159      	str	r1, [r3, #20]
30007da6:	83dc      	strh	r4, [r3, #30]
30007da8:	f44f 6400 	mov.w	r4, #2048	; 0x800
30007dac:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30007db0:	841c      	strh	r4, [r3, #32]
30007db2:	246b      	movs	r4, #107	; 0x6b
30007db4:	619a      	str	r2, [r3, #24]
30007db6:	62dc      	str	r4, [r3, #44]	; 0x2c
30007db8:	4c10      	ldr	r4, [pc, #64]	; (30007dfc <nand_init_userdef+0x68>)
30007dba:	775a      	strb	r2, [r3, #29]
30007dbc:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30007dc0:	629a      	str	r2, [r3, #40]	; 0x28
30007dc2:	631a      	str	r2, [r3, #48]	; 0x30
30007dc4:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30007dc8:	65da      	str	r2, [r3, #92]	; 0x5c
30007dca:	e9c3 0003 	strd	r0, r0, [r3, #12]
30007dce:	203b      	movs	r0, #59	; 0x3b
30007dd0:	6258      	str	r0, [r3, #36]	; 0x24
30007dd2:	200b      	movs	r0, #11
30007dd4:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30007dd8:	6698      	str	r0, [r3, #104]	; 0x68
30007dda:	2008      	movs	r0, #8
30007ddc:	4908      	ldr	r1, [pc, #32]	; (30007e00 <nand_init_userdef+0x6c>)
30007dde:	64d8      	str	r0, [r3, #76]	; 0x4c
30007de0:	f241 0013 	movw	r0, #4115	; 0x1013
30007de4:	6599      	str	r1, [r3, #88]	; 0x58
30007de6:	4907      	ldr	r1, [pc, #28]	; (30007e04 <nand_init_userdef+0x70>)
30007de8:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30007dec:	6719      	str	r1, [r3, #112]	; 0x70
30007dee:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30007df2:	f85d 4b04 	ldr.w	r4, [sp], #4
30007df6:	4770      	bx	lr
30007df8:	2001c01c 	.word	0x2001c01c
30007dfc:	000f9f06 	.word	0x000f9f06
30007e00:	d800001f 	.word	0xd800001f
30007e04:	10300804 	.word	0x10300804

30007e08 <flash_get_layout_info>:
30007e08:	b570      	push	{r4, r5, r6, lr}
30007e0a:	4b0f      	ldr	r3, [pc, #60]	; (30007e48 <flash_get_layout_info+0x40>)
30007e0c:	4615      	mov	r5, r2
30007e0e:	4604      	mov	r4, r0
30007e10:	460e      	mov	r6, r1
30007e12:	4798      	blx	r3
30007e14:	4b0d      	ldr	r3, [pc, #52]	; (30007e4c <flash_get_layout_info+0x44>)
30007e16:	4a0e      	ldr	r2, [pc, #56]	; (30007e50 <flash_get_layout_info+0x48>)
30007e18:	2800      	cmp	r0, #0
30007e1a:	bf14      	ite	ne
30007e1c:	469c      	movne	ip, r3
30007e1e:	4694      	moveq	ip, r2
30007e20:	f8dc 3000 	ldr.w	r3, [ip]
30007e24:	2bff      	cmp	r3, #255	; 0xff
30007e26:	d104      	bne.n	30007e32 <flash_get_layout_info+0x2a>
30007e28:	e00d      	b.n	30007e46 <flash_get_layout_info+0x3e>
30007e2a:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
30007e2e:	2bff      	cmp	r3, #255	; 0xff
30007e30:	d009      	beq.n	30007e46 <flash_get_layout_info+0x3e>
30007e32:	429c      	cmp	r4, r3
30007e34:	d1f9      	bne.n	30007e2a <flash_get_layout_info+0x22>
30007e36:	b116      	cbz	r6, 30007e3e <flash_get_layout_info+0x36>
30007e38:	f8dc 3004 	ldr.w	r3, [ip, #4]
30007e3c:	6033      	str	r3, [r6, #0]
30007e3e:	b115      	cbz	r5, 30007e46 <flash_get_layout_info+0x3e>
30007e40:	f8dc 3008 	ldr.w	r3, [ip, #8]
30007e44:	602b      	str	r3, [r5, #0]
30007e46:	bd70      	pop	{r4, r5, r6, pc}
30007e48:	3000927d 	.word	0x3000927d
30007e4c:	3000e764 	.word	0x3000e764
30007e50:	3000e68c 	.word	0x3000e68c

30007e54 <NAND_CHECK_IS_BAD_BLOCK>:
30007e54:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007e58:	4e24      	ldr	r6, [pc, #144]	; (30007eec <NAND_CHECK_IS_BAD_BLOCK+0x98>)
30007e5a:	0907      	lsrs	r7, r0, #4
30007e5c:	0045      	lsls	r5, r0, #1
30007e5e:	b085      	sub	sp, #20
30007e60:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30007e64:	f005 051e 	and.w	r5, r5, #30
30007e68:	40eb      	lsrs	r3, r5
30007e6a:	f003 0303 	and.w	r3, r3, #3
30007e6e:	2b02      	cmp	r3, #2
30007e70:	d02c      	beq.n	30007ecc <NAND_CHECK_IS_BAD_BLOCK+0x78>
30007e72:	2b01      	cmp	r3, #1
30007e74:	d026      	beq.n	30007ec4 <NAND_CHECK_IS_BAD_BLOCK+0x70>
30007e76:	4b1e      	ldr	r3, [pc, #120]	; (30007ef0 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30007e78:	f04f 0801 	mov.w	r8, #1
30007e7c:	2204      	movs	r2, #4
30007e7e:	4604      	mov	r4, r0
30007e80:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30007e84:	0180      	lsls	r0, r0, #6
30007e86:	ab03      	add	r3, sp, #12
30007e88:	f8df 9070 	ldr.w	r9, [pc, #112]	; 30007efc <NAND_CHECK_IS_BAD_BLOCK+0xa8>
30007e8c:	fa08 f101 	lsl.w	r1, r8, r1
30007e90:	47c8      	blx	r9
30007e92:	f89d 300c 	ldrb.w	r3, [sp, #12]
30007e96:	f89d 200d 	ldrb.w	r2, [sp, #13]
30007e9a:	4013      	ands	r3, r2
30007e9c:	2bff      	cmp	r3, #255	; 0xff
30007e9e:	d019      	beq.n	30007ed4 <NAND_CHECK_IS_BAD_BLOCK+0x80>
30007ea0:	2002      	movs	r0, #2
30007ea2:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
30007ea6:	4b13      	ldr	r3, [pc, #76]	; (30007ef4 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30007ea8:	fa00 f505 	lsl.w	r5, r0, r5
30007eac:	9400      	str	r4, [sp, #0]
30007eae:	4619      	mov	r1, r3
30007eb0:	432a      	orrs	r2, r5
30007eb2:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
30007eb6:	4a10      	ldr	r2, [pc, #64]	; (30007ef8 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30007eb8:	f003 fb7c 	bl	3000b5b4 <rtk_log_write>
30007ebc:	4640      	mov	r0, r8
30007ebe:	b005      	add	sp, #20
30007ec0:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007ec4:	2000      	movs	r0, #0
30007ec6:	b005      	add	sp, #20
30007ec8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007ecc:	2001      	movs	r0, #1
30007ece:	b005      	add	sp, #20
30007ed0:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007ed4:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30007ed8:	fa08 f005 	lsl.w	r0, r8, r5
30007edc:	4303      	orrs	r3, r0
30007ede:	2000      	movs	r0, #0
30007ee0:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30007ee4:	b005      	add	sp, #20
30007ee6:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007eea:	bf00      	nop
30007eec:	3000f398 	.word	0x3000f398
30007ef0:	2001c01c 	.word	0x2001c01c
30007ef4:	3000cf90 	.word	0x3000cf90
30007ef8:	3000cf9c 	.word	0x3000cf9c
30007efc:	3000924d 	.word	0x3000924d

30007f00 <Nand_Get_NandAddr>:
30007f00:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007f04:	4f10      	ldr	r7, [pc, #64]	; (30007f48 <Nand_Get_NandAddr+0x48>)
30007f06:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30007f0a:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30007f0e:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30007f12:	40e8      	lsrs	r0, r5
30007f14:	fa26 f505 	lsr.w	r5, r6, r5
30007f18:	09ad      	lsrs	r5, r5, #6
30007f1a:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30007f1e:	d90f      	bls.n	30007f40 <Nand_Get_NandAddr+0x40>
30007f20:	0984      	lsrs	r4, r0, #6
30007f22:	f04f 0840 	mov.w	r8, #64	; 0x40
30007f26:	4620      	mov	r0, r4
30007f28:	3401      	adds	r4, #1
30007f2a:	f7ff ff93 	bl	30007e54 <NAND_CHECK_IS_BAD_BLOCK>
30007f2e:	b128      	cbz	r0, 30007f3c <Nand_Get_NandAddr+0x3c>
30007f30:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007f34:	3501      	adds	r5, #1
30007f36:	fa08 f303 	lsl.w	r3, r8, r3
30007f3a:	441e      	add	r6, r3
30007f3c:	42a5      	cmp	r5, r4
30007f3e:	d8f2      	bhi.n	30007f26 <Nand_Get_NandAddr+0x26>
30007f40:	4630      	mov	r0, r6
30007f42:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007f46:	bf00      	nop
30007f48:	2001c01c 	.word	0x2001c01c

30007f4c <Nand_L2P_Table>:
30007f4c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007f50:	b082      	sub	sp, #8
30007f52:	4604      	mov	r4, r0
30007f54:	2002      	movs	r0, #2
30007f56:	aa01      	add	r2, sp, #4
30007f58:	4669      	mov	r1, sp
30007f5a:	f7ff ff55 	bl	30007e08 <flash_get_layout_info>
30007f5e:	9800      	ldr	r0, [sp, #0]
30007f60:	42a0      	cmp	r0, r4
30007f62:	d810      	bhi.n	30007f86 <Nand_L2P_Table+0x3a>
30007f64:	9b01      	ldr	r3, [sp, #4]
30007f66:	42a3      	cmp	r3, r4
30007f68:	d805      	bhi.n	30007f76 <Nand_L2P_Table+0x2a>
30007f6a:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30007f6e:	4620      	mov	r0, r4
30007f70:	b002      	add	sp, #8
30007f72:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007f76:	4621      	mov	r1, r4
30007f78:	f7ff ffc2 	bl	30007f00 <Nand_Get_NandAddr>
30007f7c:	4604      	mov	r4, r0
30007f7e:	4620      	mov	r0, r4
30007f80:	b002      	add	sp, #8
30007f82:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007f86:	4f0e      	ldr	r7, [pc, #56]	; (30007fc0 <Nand_L2P_Table+0x74>)
30007f88:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30007f8c:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
30007f90:	fa24 f606 	lsr.w	r6, r4, r6
30007f94:	09b6      	lsrs	r6, r6, #6
30007f96:	d0ea      	beq.n	30007f6e <Nand_L2P_Table+0x22>
30007f98:	2500      	movs	r5, #0
30007f9a:	f04f 0840 	mov.w	r8, #64	; 0x40
30007f9e:	4628      	mov	r0, r5
30007fa0:	3501      	adds	r5, #1
30007fa2:	f7ff ff57 	bl	30007e54 <NAND_CHECK_IS_BAD_BLOCK>
30007fa6:	b128      	cbz	r0, 30007fb4 <Nand_L2P_Table+0x68>
30007fa8:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007fac:	3601      	adds	r6, #1
30007fae:	fa08 f303 	lsl.w	r3, r8, r3
30007fb2:	441c      	add	r4, r3
30007fb4:	42b5      	cmp	r5, r6
30007fb6:	d3f2      	bcc.n	30007f9e <Nand_L2P_Table+0x52>
30007fb8:	4620      	mov	r0, r4
30007fba:	b002      	add	sp, #8
30007fbc:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007fc0:	2001c01c 	.word	0x2001c01c

30007fc4 <NandImgCopy>:
30007fc4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007fc8:	4617      	mov	r7, r2
30007fca:	4603      	mov	r3, r0
30007fcc:	b089      	sub	sp, #36	; 0x24
30007fce:	f8df 9100 	ldr.w	r9, [pc, #256]	; 300080d0 <NandImgCopy+0x10c>
30007fd2:	f107 38ff 	add.w	r8, r7, #4294967295	; 0xffffffff
30007fd6:	4608      	mov	r0, r1
30007fd8:	9307      	str	r3, [sp, #28]
30007fda:	f7ff ffb7 	bl	30007f4c <Nand_L2P_Table>
30007fde:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30007fe2:	4480      	add	r8, r0
30007fe4:	fa20 f406 	lsr.w	r4, r0, r6
30007fe8:	fa28 f806 	lsr.w	r8, r8, r6
30007fec:	eba8 0804 	sub.w	r8, r8, r4
30007ff0:	f1b8 3fff 	cmp.w	r8, #4294967295	; 0xffffffff
30007ff4:	d054      	beq.n	300080a0 <NandImgCopy+0xdc>
30007ff6:	2300      	movs	r3, #0
30007ff8:	4605      	mov	r5, r0
30007ffa:	f04f 0b40 	mov.w	fp, #64	; 0x40
30007ffe:	469a      	mov	sl, r3
30008000:	9306      	str	r3, [sp, #24]
30008002:	463b      	mov	r3, r7
30008004:	4647      	mov	r7, r8
30008006:	4698      	mov	r8, r3
30008008:	e020      	b.n	3000804c <NandImgCopy+0x88>
3000800a:	f1c2 0320 	rsb	r3, r2, #32
3000800e:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008012:	40d9      	lsrs	r1, r3
30008014:	9b07      	ldr	r3, [sp, #28]
30008016:	4029      	ands	r1, r5
30008018:	4453      	add	r3, sl
3000801a:	b377      	cbz	r7, 3000807a <NandImgCopy+0xb6>
3000801c:	2601      	movs	r6, #1
3000801e:	4620      	mov	r0, r4
30008020:	9105      	str	r1, [sp, #20]
30008022:	fa06 f202 	lsl.w	r2, r6, r2
30008026:	4e27      	ldr	r6, [pc, #156]	; (300080c4 <NandImgCopy+0x100>)
30008028:	9204      	str	r2, [sp, #16]
3000802a:	1a52      	subs	r2, r2, r1
3000802c:	4492      	add	sl, r2
3000802e:	4415      	add	r5, r2
30008030:	47b0      	blx	r6
30008032:	f010 0f70 	tst.w	r0, #112	; 0x70
30008036:	9b04      	ldr	r3, [sp, #16]
30008038:	9905      	ldr	r1, [sp, #20]
3000803a:	d134      	bne.n	300080a6 <NandImgCopy+0xe2>
3000803c:	eba8 0803 	sub.w	r8, r8, r3
30008040:	4488      	add	r8, r1
30008042:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008046:	3f01      	subs	r7, #1
30008048:	fa25 f406 	lsr.w	r4, r5, r6
3000804c:	9b06      	ldr	r3, [sp, #24]
3000804e:	4632      	mov	r2, r6
30008050:	09a6      	lsrs	r6, r4, #6
30008052:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008056:	d8d8      	bhi.n	3000800a <NandImgCopy+0x46>
30008058:	e005      	b.n	30008066 <NandImgCopy+0xa2>
3000805a:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
3000805e:	3440      	adds	r4, #64	; 0x40
30008060:	fa0b f303 	lsl.w	r3, fp, r3
30008064:	441d      	add	r5, r3
30008066:	4630      	mov	r0, r6
30008068:	3601      	adds	r6, #1
3000806a:	f7ff fef3 	bl	30007e54 <NAND_CHECK_IS_BAD_BLOCK>
3000806e:	2800      	cmp	r0, #0
30008070:	d1f3      	bne.n	3000805a <NandImgCopy+0x96>
30008072:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008076:	9606      	str	r6, [sp, #24]
30008078:	e7c7      	b.n	3000800a <NandImgCopy+0x46>
3000807a:	4642      	mov	r2, r8
3000807c:	4620      	mov	r0, r4
3000807e:	4d11      	ldr	r5, [pc, #68]	; (300080c4 <NandImgCopy+0x100>)
30008080:	47a8      	blx	r5
30008082:	f010 0f70 	tst.w	r0, #112	; 0x70
30008086:	d00b      	beq.n	300080a0 <NandImgCopy+0xdc>
30008088:	09a1      	lsrs	r1, r4, #6
3000808a:	4b0f      	ldr	r3, [pc, #60]	; (300080c8 <NandImgCopy+0x104>)
3000808c:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008090:	4a0e      	ldr	r2, [pc, #56]	; (300080cc <NandImgCopy+0x108>)
30008092:	e9cd 1001 	strd	r1, r0, [sp, #4]
30008096:	9400      	str	r4, [sp, #0]
30008098:	4619      	mov	r1, r3
3000809a:	2004      	movs	r0, #4
3000809c:	f003 fa8a 	bl	3000b5b4 <rtk_log_write>
300080a0:	b009      	add	sp, #36	; 0x24
300080a2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300080a6:	1ac9      	subs	r1, r1, r3
300080a8:	09a3      	lsrs	r3, r4, #6
300080aa:	f004 043f 	and.w	r4, r4, #63	; 0x3f
300080ae:	9002      	str	r0, [sp, #8]
300080b0:	9301      	str	r3, [sp, #4]
300080b2:	4488      	add	r8, r1
300080b4:	4b04      	ldr	r3, [pc, #16]	; (300080c8 <NandImgCopy+0x104>)
300080b6:	2004      	movs	r0, #4
300080b8:	4a04      	ldr	r2, [pc, #16]	; (300080cc <NandImgCopy+0x108>)
300080ba:	4619      	mov	r1, r3
300080bc:	9400      	str	r4, [sp, #0]
300080be:	f003 fa79 	bl	3000b5b4 <rtk_log_write>
300080c2:	e7be      	b.n	30008042 <NandImgCopy+0x7e>
300080c4:	3000924d 	.word	0x3000924d
300080c8:	3000cf90 	.word	0x3000cf90
300080cc:	3000cfb4 	.word	0x3000cfb4
300080d0:	2001c01c 	.word	0x2001c01c

300080d4 <OTP_Read8.part.0>:
300080d4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300080d8:	4b30      	ldr	r3, [pc, #192]	; (3000819c <OTP_Read8.part.0+0xc8>)
300080da:	4680      	mov	r8, r0
300080dc:	e843 f300 	tt	r3, r3
300080e0:	f3c3 5380 	ubfx	r3, r3, #22, #1
300080e4:	4a2e      	ldr	r2, [pc, #184]	; (300081a0 <OTP_Read8.part.0+0xcc>)
300080e6:	4e2f      	ldr	r6, [pc, #188]	; (300081a4 <OTP_Read8.part.0+0xd0>)
300080e8:	b082      	sub	sp, #8
300080ea:	2b00      	cmp	r3, #0
300080ec:	4689      	mov	r9, r1
300080ee:	bf0e      	itee	eq
300080f0:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300080f4:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300080f8:	4616      	movne	r6, r2
300080fa:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300080fc:	05d8      	lsls	r0, r3, #23
300080fe:	d505      	bpl.n	3000810c <OTP_Read8.part.0+0x38>
30008100:	4d29      	ldr	r5, [pc, #164]	; (300081a8 <OTP_Read8.part.0+0xd4>)
30008102:	2064      	movs	r0, #100	; 0x64
30008104:	47a8      	blx	r5
30008106:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008108:	05d9      	lsls	r1, r3, #23
3000810a:	d4fa      	bmi.n	30008102 <OTP_Read8.part.0+0x2e>
3000810c:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000810e:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008112:	643b      	str	r3, [r7, #64]	; 0x40
30008114:	4b25      	ldr	r3, [pc, #148]	; (300081ac <OTP_Read8.part.0+0xd8>)
30008116:	681a      	ldr	r2, [r3, #0]
30008118:	07d2      	lsls	r2, r2, #31
3000811a:	d403      	bmi.n	30008124 <OTP_Read8.part.0+0x50>
3000811c:	681a      	ldr	r2, [r3, #0]
3000811e:	f042 0201 	orr.w	r2, r2, #1
30008122:	601a      	str	r2, [r3, #0]
30008124:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008128:	ea4f 2308 	mov.w	r3, r8, lsl #8
3000812c:	694a      	ldr	r2, [r1, #20]
3000812e:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008132:	614a      	str	r2, [r1, #20]
30008134:	6033      	str	r3, [r6, #0]
30008136:	6833      	ldr	r3, [r6, #0]
30008138:	2b00      	cmp	r3, #0
3000813a:	db10      	blt.n	3000815e <OTP_Read8.part.0+0x8a>
3000813c:	2400      	movs	r4, #0
3000813e:	4d1a      	ldr	r5, [pc, #104]	; (300081a8 <OTP_Read8.part.0+0xd4>)
30008140:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008144:	e001      	b.n	3000814a <OTP_Read8.part.0+0x76>
30008146:	4554      	cmp	r4, sl
30008148:	d01a      	beq.n	30008180 <OTP_Read8.part.0+0xac>
3000814a:	2005      	movs	r0, #5
3000814c:	3401      	adds	r4, #1
3000814e:	47a8      	blx	r5
30008150:	6833      	ldr	r3, [r6, #0]
30008152:	2b00      	cmp	r3, #0
30008154:	daf7      	bge.n	30008146 <OTP_Read8.part.0+0x72>
30008156:	f644 6320 	movw	r3, #20000	; 0x4e20
3000815a:	429c      	cmp	r4, r3
3000815c:	d010      	beq.n	30008180 <OTP_Read8.part.0+0xac>
3000815e:	6833      	ldr	r3, [r6, #0]
30008160:	2001      	movs	r0, #1
30008162:	f889 3000 	strb.w	r3, [r9]
30008166:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
3000816a:	6953      	ldr	r3, [r2, #20]
3000816c:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008170:	6153      	str	r3, [r2, #20]
30008172:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008174:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008178:	643b      	str	r3, [r7, #64]	; 0x40
3000817a:	b002      	add	sp, #8
3000817c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008180:	22ff      	movs	r2, #255	; 0xff
30008182:	4b0b      	ldr	r3, [pc, #44]	; (300081b0 <OTP_Read8.part.0+0xdc>)
30008184:	2002      	movs	r0, #2
30008186:	f8cd 8000 	str.w	r8, [sp]
3000818a:	4619      	mov	r1, r3
3000818c:	f889 2000 	strb.w	r2, [r9]
30008190:	4a08      	ldr	r2, [pc, #32]	; (300081b4 <OTP_Read8.part.0+0xe0>)
30008192:	f003 fa0f 	bl	3000b5b4 <rtk_log_write>
30008196:	2000      	movs	r0, #0
30008198:	e7e5      	b.n	30008166 <OTP_Read8.part.0+0x92>
3000819a:	bf00      	nop
3000819c:	0000e5f9 	.word	0x0000e5f9
300081a0:	5200000c 	.word	0x5200000c
300081a4:	42000008 	.word	0x42000008
300081a8:	00009b2d 	.word	0x00009b2d
300081ac:	42008000 	.word	0x42008000
300081b0:	3000cff0 	.word	0x3000cff0
300081b4:	3000cff4 	.word	0x3000cff4

300081b8 <OTP_Read8>:
300081b8:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
300081bc:	d201      	bcs.n	300081c2 <OTP_Read8+0xa>
300081be:	f7ff bf89 	b.w	300080d4 <OTP_Read8.part.0>
300081c2:	22ff      	movs	r2, #255	; 0xff
300081c4:	2000      	movs	r0, #0
300081c6:	700a      	strb	r2, [r1, #0]
300081c8:	4770      	bx	lr
300081ca:	bf00      	nop

300081cc <PLL_NP_ClkSet>:
300081cc:	b570      	push	{r4, r5, r6, lr}
300081ce:	4604      	mov	r4, r0
300081d0:	4b1b      	ldr	r3, [pc, #108]	; (30008240 <PLL_NP_ClkSet+0x74>)
300081d2:	4798      	blx	r3
300081d4:	4d1b      	ldr	r5, [pc, #108]	; (30008244 <PLL_NP_ClkSet+0x78>)
300081d6:	08c2      	lsrs	r2, r0, #3
300081d8:	fbb4 f3f0 	udiv	r3, r4, r0
300081dc:	fb00 4c13 	mls	ip, r0, r3, r4
300081e0:	1e9c      	subs	r4, r3, #2
300081e2:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300081e4:	0164      	lsls	r4, r4, #5
300081e6:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
300081ea:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
300081ee:	431c      	orrs	r4, r3
300081f0:	fbbc f3f2 	udiv	r3, ip, r2
300081f4:	646c      	str	r4, [r5, #68]	; 0x44
300081f6:	041c      	lsls	r4, r3, #16
300081f8:	fb02 c013 	mls	r0, r2, r3, ip
300081fc:	2300      	movs	r3, #0
300081fe:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30008200:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30008204:	4619      	mov	r1, r3
30008206:	fa1f fc86 	uxth.w	ip, r6
3000820a:	4e0f      	ldr	r6, [pc, #60]	; (30008248 <PLL_NP_ClkSet+0x7c>)
3000820c:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30008210:	ea44 040c 	orr.w	r4, r4, ip
30008214:	f003 fa58 	bl	3000b6c8 <__aeabi_uldivmod>
30008218:	4603      	mov	r3, r0
3000821a:	2001      	movs	r0, #1
3000821c:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30008220:	64ec      	str	r4, [r5, #76]	; 0x4c
30008222:	47b0      	blx	r6
30008224:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008226:	203c      	movs	r0, #60	; 0x3c
30008228:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
3000822c:	646b      	str	r3, [r5, #68]	; 0x44
3000822e:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008230:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30008234:	646b      	str	r3, [r5, #68]	; 0x44
30008236:	4633      	mov	r3, r6
30008238:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000823c:	4718      	bx	r3
3000823e:	bf00      	nop
30008240:	000099f5 	.word	0x000099f5
30008244:	42008800 	.word	0x42008800
30008248:	00009b2d 	.word	0x00009b2d

3000824c <PLL_AP_ClkSet>:
3000824c:	b510      	push	{r4, lr}
3000824e:	4604      	mov	r4, r0
30008250:	4b0b      	ldr	r3, [pc, #44]	; (30008280 <PLL_AP_ClkSet+0x34>)
30008252:	4798      	blx	r3
30008254:	fbb4 f0f0 	udiv	r0, r4, r0
30008258:	f1a0 0319 	sub.w	r3, r0, #25
3000825c:	1e84      	subs	r4, r0, #2
3000825e:	2b0f      	cmp	r3, #15
30008260:	d807      	bhi.n	30008272 <PLL_AP_ClkSet+0x26>
30008262:	4b08      	ldr	r3, [pc, #32]	; (30008284 <PLL_AP_ClkSet+0x38>)
30008264:	b2e0      	uxtb	r0, r4
30008266:	695c      	ldr	r4, [r3, #20]
30008268:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
3000826c:	4320      	orrs	r0, r4
3000826e:	6158      	str	r0, [r3, #20]
30008270:	bd10      	pop	{r4, pc}
30008272:	f240 1181 	movw	r1, #385	; 0x181
30008276:	4804      	ldr	r0, [pc, #16]	; (30008288 <PLL_AP_ClkSet+0x3c>)
30008278:	f003 fc7a 	bl	3000bb70 <__io_assert_failed_veneer>
3000827c:	e7f1      	b.n	30008262 <PLL_AP_ClkSet+0x16>
3000827e:	bf00      	nop
30008280:	000099f5 	.word	0x000099f5
30008284:	41000300 	.word	0x41000300
30008288:	3000d010 	.word	0x3000d010

3000828c <PLL_AP>:
3000828c:	2801      	cmp	r0, #1
3000828e:	b538      	push	{r3, r4, r5, lr}
30008290:	d009      	beq.n	300082a6 <PLL_AP+0x1a>
30008292:	4b13      	ldr	r3, [pc, #76]	; (300082e0 <PLL_AP+0x54>)
30008294:	681a      	ldr	r2, [r3, #0]
30008296:	f022 0209 	bic.w	r2, r2, #9
3000829a:	601a      	str	r2, [r3, #0]
3000829c:	681a      	ldr	r2, [r3, #0]
3000829e:	f022 0206 	bic.w	r2, r2, #6
300082a2:	601a      	str	r2, [r3, #0]
300082a4:	bd38      	pop	{r3, r4, r5, pc}
300082a6:	4b0f      	ldr	r3, [pc, #60]	; (300082e4 <PLL_AP+0x58>)
300082a8:	6f9a      	ldr	r2, [r3, #120]	; 0x78
300082aa:	f012 0f05 	tst.w	r2, #5
300082ae:	d00f      	beq.n	300082d0 <PLL_AP+0x44>
300082b0:	4d0d      	ldr	r5, [pc, #52]	; (300082e8 <PLL_AP+0x5c>)
300082b2:	4c0b      	ldr	r4, [pc, #44]	; (300082e0 <PLL_AP+0x54>)
300082b4:	2001      	movs	r0, #1
300082b6:	6823      	ldr	r3, [r4, #0]
300082b8:	f043 0306 	orr.w	r3, r3, #6
300082bc:	6023      	str	r3, [r4, #0]
300082be:	47a8      	blx	r5
300082c0:	6823      	ldr	r3, [r4, #0]
300082c2:	f043 0309 	orr.w	r3, r3, #9
300082c6:	6023      	str	r3, [r4, #0]
300082c8:	6863      	ldr	r3, [r4, #4]
300082ca:	2b00      	cmp	r3, #0
300082cc:	dafc      	bge.n	300082c8 <PLL_AP+0x3c>
300082ce:	bd38      	pop	{r3, r4, r5, pc}
300082d0:	6f9a      	ldr	r2, [r3, #120]	; 0x78
300082d2:	20a0      	movs	r0, #160	; 0xa0
300082d4:	4d04      	ldr	r5, [pc, #16]	; (300082e8 <PLL_AP+0x5c>)
300082d6:	f042 0207 	orr.w	r2, r2, #7
300082da:	679a      	str	r2, [r3, #120]	; 0x78
300082dc:	47a8      	blx	r5
300082de:	e7e8      	b.n	300082b2 <PLL_AP+0x26>
300082e0:	41000300 	.word	0x41000300
300082e4:	42008800 	.word	0x42008800
300082e8:	00009b2d 	.word	0x00009b2d

300082ec <PSRAM_INFO_Update>:
300082ec:	4b49      	ldr	r3, [pc, #292]	; (30008414 <PSRAM_INFO_Update+0x128>)
300082ee:	494a      	ldr	r1, [pc, #296]	; (30008418 <PSRAM_INFO_Update+0x12c>)
300082f0:	4a4a      	ldr	r2, [pc, #296]	; (3000841c <PSRAM_INFO_Update+0x130>)
300082f2:	b530      	push	{r4, r5, lr}
300082f4:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
300082f8:	b083      	sub	sp, #12
300082fa:	680d      	ldr	r5, [r1, #0]
300082fc:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008300:	4c47      	ldr	r4, [pc, #284]	; (30008420 <PSRAM_INFO_Update+0x134>)
30008302:	fb02 f505 	mul.w	r5, r2, r5
30008306:	3301      	adds	r3, #1
30008308:	fbb5 f5f3 	udiv	r5, r5, r3
3000830c:	086a      	lsrs	r2, r5, #1
3000830e:	4b45      	ldr	r3, [pc, #276]	; (30008424 <PSRAM_INFO_Update+0x138>)
30008310:	60e2      	str	r2, [r4, #12]
30008312:	4798      	blx	r3
30008314:	4b44      	ldr	r3, [pc, #272]	; (30008428 <PSRAM_INFO_Update+0x13c>)
30008316:	6020      	str	r0, [r4, #0]
30008318:	4798      	blx	r3
3000831a:	4b44      	ldr	r3, [pc, #272]	; (3000842c <PSRAM_INFO_Update+0x140>)
3000831c:	220a      	movs	r2, #10
3000831e:	60a0      	str	r0, [r4, #8]
30008320:	6122      	str	r2, [r4, #16]
30008322:	2220      	movs	r2, #32
30008324:	fbb3 f3f5 	udiv	r3, r3, r5
30008328:	6163      	str	r3, [r4, #20]
3000832a:	fbb2 f3f3 	udiv	r3, r2, r3
3000832e:	6822      	ldr	r2, [r4, #0]
30008330:	6223      	str	r3, [r4, #32]
30008332:	2a01      	cmp	r2, #1
30008334:	68e3      	ldr	r3, [r4, #12]
30008336:	4a3e      	ldr	r2, [pc, #248]	; (30008430 <PSRAM_INFO_Update+0x144>)
30008338:	d01d      	beq.n	30008376 <PSRAM_INFO_Update+0x8a>
3000833a:	4293      	cmp	r3, r2
3000833c:	d810      	bhi.n	30008360 <PSRAM_INFO_Update+0x74>
3000833e:	2304      	movs	r3, #4
30008340:	2208      	movs	r2, #8
30008342:	4618      	mov	r0, r3
30008344:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008348:	4b3a      	ldr	r3, [pc, #232]	; (30008434 <PSRAM_INFO_Update+0x148>)
3000834a:	493b      	ldr	r1, [pc, #236]	; (30008438 <PSRAM_INFO_Update+0x14c>)
3000834c:	4a3b      	ldr	r2, [pc, #236]	; (3000843c <PSRAM_INFO_Update+0x150>)
3000834e:	f881 0110 	strb.w	r0, [r1, #272]	; 0x110
30008352:	4619      	mov	r1, r3
30008354:	2004      	movs	r0, #4
30008356:	9500      	str	r5, [sp, #0]
30008358:	f003 f92c 	bl	3000b5b4 <rtk_log_write>
3000835c:	b003      	add	sp, #12
3000835e:	bd30      	pop	{r4, r5, pc}
30008360:	4a37      	ldr	r2, [pc, #220]	; (30008440 <PSRAM_INFO_Update+0x154>)
30008362:	4293      	cmp	r3, r2
30008364:	d918      	bls.n	30008398 <PSRAM_INFO_Update+0xac>
30008366:	4a37      	ldr	r2, [pc, #220]	; (30008444 <PSRAM_INFO_Update+0x158>)
30008368:	4293      	cmp	r3, r2
3000836a:	d827      	bhi.n	300083bc <PSRAM_INFO_Update+0xd0>
3000836c:	2306      	movs	r3, #6
3000836e:	4618      	mov	r0, r3
30008370:	e9c4 3306 	strd	r3, r3, [r4, #24]
30008374:	e7e8      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
30008376:	4293      	cmp	r3, r2
30008378:	d908      	bls.n	3000838c <PSRAM_INFO_Update+0xa0>
3000837a:	4a31      	ldr	r2, [pc, #196]	; (30008440 <PSRAM_INFO_Update+0x154>)
3000837c:	4293      	cmp	r3, r2
3000837e:	d811      	bhi.n	300083a4 <PSRAM_INFO_Update+0xb8>
30008380:	2305      	movs	r3, #5
30008382:	220f      	movs	r2, #15
30008384:	4618      	mov	r0, r3
30008386:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000838a:	e7dd      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
3000838c:	2304      	movs	r3, #4
3000838e:	220f      	movs	r2, #15
30008390:	4618      	mov	r0, r3
30008392:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008396:	e7d7      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
30008398:	2305      	movs	r3, #5
3000839a:	2208      	movs	r2, #8
3000839c:	4618      	mov	r0, r3
3000839e:	e9c4 3206 	strd	r3, r2, [r4, #24]
300083a2:	e7d1      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
300083a4:	4a27      	ldr	r2, [pc, #156]	; (30008444 <PSRAM_INFO_Update+0x158>)
300083a6:	4293      	cmp	r3, r2
300083a8:	d918      	bls.n	300083dc <PSRAM_INFO_Update+0xf0>
300083aa:	4a27      	ldr	r2, [pc, #156]	; (30008448 <PSRAM_INFO_Update+0x15c>)
300083ac:	4293      	cmp	r3, r2
300083ae:	d821      	bhi.n	300083f4 <PSRAM_INFO_Update+0x108>
300083b0:	2307      	movs	r3, #7
300083b2:	2214      	movs	r2, #20
300083b4:	4618      	mov	r0, r3
300083b6:	e9c4 3206 	strd	r3, r2, [r4, #24]
300083ba:	e7c5      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
300083bc:	4a22      	ldr	r2, [pc, #136]	; (30008448 <PSRAM_INFO_Update+0x15c>)
300083be:	4293      	cmp	r3, r2
300083c0:	d912      	bls.n	300083e8 <PSRAM_INFO_Update+0xfc>
300083c2:	4a22      	ldr	r2, [pc, #136]	; (3000844c <PSRAM_INFO_Update+0x160>)
300083c4:	4293      	cmp	r3, r2
300083c6:	bf8c      	ite	hi
300083c8:	2300      	movhi	r3, #0
300083ca:	2301      	movls	r3, #1
300083cc:	2804      	cmp	r0, #4
300083ce:	d100      	bne.n	300083d2 <PSRAM_INFO_Update+0xe6>
300083d0:	b953      	cbnz	r3, 300083e8 <PSRAM_INFO_Update+0xfc>
300083d2:	2803      	cmp	r0, #3
300083d4:	d100      	bne.n	300083d8 <PSRAM_INFO_Update+0xec>
300083d6:	b9b3      	cbnz	r3, 30008406 <PSRAM_INFO_Update+0x11a>
300083d8:	7e20      	ldrb	r0, [r4, #24]
300083da:	e7b5      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
300083dc:	2306      	movs	r3, #6
300083de:	2212      	movs	r2, #18
300083e0:	4618      	mov	r0, r3
300083e2:	e9c4 3206 	strd	r3, r2, [r4, #24]
300083e6:	e7af      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
300083e8:	2307      	movs	r3, #7
300083ea:	2206      	movs	r2, #6
300083ec:	4618      	mov	r0, r3
300083ee:	e9c4 3206 	strd	r3, r2, [r4, #24]
300083f2:	e7a9      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
300083f4:	4a15      	ldr	r2, [pc, #84]	; (3000844c <PSRAM_INFO_Update+0x160>)
300083f6:	4293      	cmp	r3, r2
300083f8:	d8ee      	bhi.n	300083d8 <PSRAM_INFO_Update+0xec>
300083fa:	2308      	movs	r3, #8
300083fc:	221c      	movs	r2, #28
300083fe:	4618      	mov	r0, r3
30008400:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008404:	e7a0      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
30008406:	230a      	movs	r3, #10
30008408:	2206      	movs	r2, #6
3000840a:	4618      	mov	r0, r3
3000840c:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008410:	e79a      	b.n	30008348 <PSRAM_INFO_Update+0x5c>
30008412:	bf00      	nop
30008414:	42008000 	.word	0x42008000
30008418:	3000e674 	.word	0x3000e674
3000841c:	000f4240 	.word	0x000f4240
30008420:	3000f3a0 	.word	0x3000f3a0
30008424:	30007545 	.word	0x30007545
30008428:	3000750d 	.word	0x3000750d
3000842c:	3b9aca00 	.word	0x3b9aca00
30008430:	0632ea00 	.word	0x0632ea00
30008434:	3000d020 	.word	0x3000d020
30008438:	23020000 	.word	0x23020000
3000843c:	3000d028 	.word	0x3000d028
30008440:	07ed6b40 	.word	0x07ed6b40
30008444:	09e4f580 	.word	0x09e4f580
30008448:	0bebc200 	.word	0x0bebc200
3000844c:	0ee6b280 	.word	0x0ee6b280

30008450 <PSRAM_PHY_StructInit>:
30008450:	2203      	movs	r2, #3
30008452:	2301      	movs	r3, #1
30008454:	210a      	movs	r1, #10
30008456:	b410      	push	{r4}
30008458:	e9c0 2202 	strd	r2, r2, [r0, #8]
3000845c:	2402      	movs	r4, #2
3000845e:	2207      	movs	r2, #7
30008460:	e9c0 3400 	strd	r3, r4, [r0]
30008464:	e9c0 1304 	strd	r1, r3, [r0, #16]
30008468:	f85d 4b04 	ldr.w	r4, [sp], #4
3000846c:	6182      	str	r2, [r0, #24]
3000846e:	4770      	bx	lr

30008470 <PSRAM_PHY_Init>:
30008470:	b538      	push	{r3, r4, r5, lr}
30008472:	4604      	mov	r4, r0
30008474:	6903      	ldr	r3, [r0, #16]
30008476:	4d14      	ldr	r5, [pc, #80]	; (300084c8 <PSRAM_PHY_Init+0x58>)
30008478:	f003 031f 	and.w	r3, r3, #31
3000847c:	4913      	ldr	r1, [pc, #76]	; (300084cc <PSRAM_PHY_Init+0x5c>)
3000847e:	e9d0 0200 	ldrd	r0, r2, [r0]
30008482:	0612      	lsls	r2, r2, #24
30008484:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30008488:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
3000848c:	4313      	orrs	r3, r2
3000848e:	68a2      	ldr	r2, [r4, #8]
30008490:	0412      	lsls	r2, r2, #16
30008492:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008496:	4313      	orrs	r3, r2
30008498:	68e2      	ldr	r2, [r4, #12]
3000849a:	0212      	lsls	r2, r2, #8
3000849c:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
300084a0:	4313      	orrs	r3, r2
300084a2:	606b      	str	r3, [r5, #4]
300084a4:	4788      	blx	r1
300084a6:	2801      	cmp	r0, #1
300084a8:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
300084ac:	bf08      	it	eq
300084ae:	4a08      	ldreq	r2, [pc, #32]	; (300084d0 <PSRAM_PHY_Init+0x60>)
300084b0:	ea4f 1303 	mov.w	r3, r3, lsl #4
300084b4:	f001 010f 	and.w	r1, r1, #15
300084b8:	bf18      	it	ne
300084ba:	4a06      	ldrne	r2, [pc, #24]	; (300084d4 <PSRAM_PHY_Init+0x64>)
300084bc:	b2db      	uxtb	r3, r3
300084be:	430b      	orrs	r3, r1
300084c0:	431a      	orrs	r2, r3
300084c2:	61aa      	str	r2, [r5, #24]
300084c4:	bd38      	pop	{r3, r4, r5, pc}
300084c6:	bf00      	nop
300084c8:	41002000 	.word	0x41002000
300084cc:	30007545 	.word	0x30007545
300084d0:	80000e00 	.word	0x80000e00
300084d4:	80000f00 	.word	0x80000f00

300084d8 <PSRAM_CTRL_Init>:
300084d8:	b470      	push	{r4, r5, r6}
300084da:	4b4c      	ldr	r3, [pc, #304]	; (3000860c <PSRAM_CTRL_Init+0x134>)
300084dc:	2400      	movs	r4, #0
300084de:	4a4c      	ldr	r2, [pc, #304]	; (30008610 <PSRAM_CTRL_Init+0x138>)
300084e0:	f24d 0101 	movw	r1, #53249	; 0xd001
300084e4:	609c      	str	r4, [r3, #8]
300084e6:	2001      	movs	r0, #1
300084e8:	601a      	str	r2, [r3, #0]
300084ea:	4a4a      	ldr	r2, [pc, #296]	; (30008614 <PSRAM_CTRL_Init+0x13c>)
300084ec:	6158      	str	r0, [r3, #20]
300084ee:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
300084f2:	6811      	ldr	r1, [r2, #0]
300084f4:	2902      	cmp	r1, #2
300084f6:	d04a      	beq.n	3000858e <PSRAM_CTRL_Init+0xb6>
300084f8:	6954      	ldr	r4, [r2, #20]
300084fa:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
300084fe:	69d0      	ldr	r0, [r2, #28]
30008500:	6915      	ldr	r5, [r2, #16]
30008502:	fbb0 f0f4 	udiv	r0, r0, r4
30008506:	fbb1 f1f4 	udiv	r1, r1, r4
3000850a:	6994      	ldr	r4, [r2, #24]
3000850c:	f005 020f 	and.w	r2, r5, #15
30008510:	f241 5551 	movw	r5, #5457	; 0x1551
30008514:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30008518:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
3000851c:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008520:	094a      	lsrs	r2, r1, #5
30008522:	0181      	lsls	r1, r0, #6
30008524:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30008528:	4d3b      	ldr	r5, [pc, #236]	; (30008618 <PSRAM_CTRL_Init+0x140>)
3000852a:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
3000852e:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
30008532:	00a2      	lsls	r2, r4, #2
30008534:	4301      	orrs	r1, r0
30008536:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
3000853a:	3a01      	subs	r2, #1
3000853c:	430d      	orrs	r5, r1
3000853e:	f242 0120 	movw	r1, #8224	; 0x2020
30008542:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008546:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
3000854a:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
3000854e:	f44f 2180 	mov.w	r1, #262144	; 0x40000
30008552:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
30008556:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
3000855a:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000855e:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
30008562:	4420      	add	r0, r4
30008564:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
30008568:	f640 71fe 	movw	r1, #4094	; 0xffe
3000856c:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
30008570:	ea01 0140 	and.w	r1, r1, r0, lsl #1
30008574:	432a      	orrs	r2, r5
30008576:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
3000857a:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
3000857e:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
30008582:	4a22      	ldr	r2, [pc, #136]	; (3000860c <PSRAM_CTRL_Init+0x134>)
30008584:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008586:	061b      	lsls	r3, r3, #24
30008588:	d5fc      	bpl.n	30008584 <PSRAM_CTRL_Init+0xac>
3000858a:	bc70      	pop	{r4, r5, r6}
3000858c:	4770      	bx	lr
3000858e:	6955      	ldr	r5, [r2, #20]
30008590:	207d      	movs	r0, #125	; 0x7d
30008592:	69d1      	ldr	r1, [r2, #28]
30008594:	26a0      	movs	r6, #160	; 0xa0
30008596:	6914      	ldr	r4, [r2, #16]
30008598:	6992      	ldr	r2, [r2, #24]
3000859a:	f004 040f 	and.w	r4, r4, #15
3000859e:	0092      	lsls	r2, r2, #2
300085a0:	fbb1 f1f5 	udiv	r1, r1, r5
300085a4:	fbb0 f5f5 	udiv	r5, r0, r5
300085a8:	f245 5051 	movw	r0, #21841	; 0x5551
300085ac:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
300085b0:	f001 003f 	and.w	r0, r1, #63	; 0x3f
300085b4:	0189      	lsls	r1, r1, #6
300085b6:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
300085ba:	f641 4570 	movw	r5, #7280	; 0x1c70
300085be:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
300085c2:	4325      	orrs	r5, r4
300085c4:	2420      	movs	r4, #32
300085c6:	4308      	orrs	r0, r1
300085c8:	1e51      	subs	r1, r2, #1
300085ca:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
300085ce:	3a02      	subs	r2, #2
300085d0:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
300085d4:	f3c1 010b 	ubfx	r1, r1, #0, #12
300085d8:	f3c2 020b 	ubfx	r2, r2, #0, #12
300085dc:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
300085e0:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
300085e4:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
300085e8:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
300085ec:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
300085f0:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
300085f4:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
300085f8:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
300085fc:	4301      	orrs	r1, r0
300085fe:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
30008602:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30008606:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
3000860a:	e7ba      	b.n	30008582 <PSRAM_CTRL_Init+0xaa>
3000860c:	41012000 	.word	0x41012000
30008610:	00bfe000 	.word	0x00bfe000
30008614:	3000f3a0 	.word	0x3000f3a0
30008618:	00107000 	.word	0x00107000

3000861c <PSRAM_REG_Write>:
3000861c:	b430      	push	{r4, r5}
3000861e:	4c20      	ldr	r4, [pc, #128]	; (300086a0 <PSRAM_REG_Write+0x84>)
30008620:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008622:	f013 0301 	ands.w	r3, r3, #1
30008626:	d1fb      	bne.n	30008620 <PSRAM_REG_Write+0x4>
30008628:	6825      	ldr	r5, [r4, #0]
3000862a:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
3000862e:	6025      	str	r5, [r4, #0]
30008630:	60a3      	str	r3, [r4, #8]
30008632:	6825      	ldr	r5, [r4, #0]
30008634:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008638:	6025      	str	r5, [r4, #0]
3000863a:	6063      	str	r3, [r4, #4]
3000863c:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
30008640:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
30008644:	23c0      	movs	r3, #192	; 0xc0
30008646:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000864a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000864e:	0e03      	lsrs	r3, r0, #24
30008650:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008654:	f3c0 4307 	ubfx	r3, r0, #16, #8
30008658:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000865c:	f3c0 2307 	ubfx	r3, r0, #8, #8
30008660:	b2c0      	uxtb	r0, r0
30008662:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008666:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
3000866a:	b141      	cbz	r1, 3000867e <PSRAM_REG_Write+0x62>
3000866c:	3a01      	subs	r2, #1
3000866e:	480c      	ldr	r0, [pc, #48]	; (300086a0 <PSRAM_REG_Write+0x84>)
30008670:	4411      	add	r1, r2
30008672:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30008676:	428a      	cmp	r2, r1
30008678:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000867c:	d1f9      	bne.n	30008672 <PSRAM_REG_Write+0x56>
3000867e:	4a08      	ldr	r2, [pc, #32]	; (300086a0 <PSRAM_REG_Write+0x84>)
30008680:	2301      	movs	r3, #1
30008682:	6093      	str	r3, [r2, #8]
30008684:	6893      	ldr	r3, [r2, #8]
30008686:	07d9      	lsls	r1, r3, #31
30008688:	d4fc      	bmi.n	30008684 <PSRAM_REG_Write+0x68>
3000868a:	4a05      	ldr	r2, [pc, #20]	; (300086a0 <PSRAM_REG_Write+0x84>)
3000868c:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000868e:	07db      	lsls	r3, r3, #31
30008690:	d4fc      	bmi.n	3000868c <PSRAM_REG_Write+0x70>
30008692:	6813      	ldr	r3, [r2, #0]
30008694:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008698:	6013      	str	r3, [r2, #0]
3000869a:	bc30      	pop	{r4, r5}
3000869c:	4770      	bx	lr
3000869e:	bf00      	nop
300086a0:	41012000 	.word	0x41012000

300086a4 <PSRAM_APM_DEVIC_Init>:
300086a4:	4b12      	ldr	r3, [pc, #72]	; (300086f0 <PSRAM_APM_DEVIC_Init+0x4c>)
300086a6:	2102      	movs	r1, #2
300086a8:	2000      	movs	r0, #0
300086aa:	b530      	push	{r4, r5, lr}
300086ac:	4c11      	ldr	r4, [pc, #68]	; (300086f4 <PSRAM_APM_DEVIC_Init+0x50>)
300086ae:	b083      	sub	sp, #12
300086b0:	699d      	ldr	r5, [r3, #24]
300086b2:	466a      	mov	r2, sp
300086b4:	eb04 0c05 	add.w	ip, r4, r5
300086b8:	1eeb      	subs	r3, r5, #3
300086ba:	4c0f      	ldr	r4, [pc, #60]	; (300086f8 <PSRAM_APM_DEVIC_Init+0x54>)
300086bc:	408b      	lsls	r3, r1
300086be:	f81c cc03 	ldrb.w	ip, [ip, #-3]
300086c2:	f003 031c 	and.w	r3, r3, #28
300086c6:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
300086ca:	f043 0320 	orr.w	r3, r3, #32
300086ce:	fa5f fc8c 	uxtb.w	ip, ip
300086d2:	f88d 3000 	strb.w	r3, [sp]
300086d6:	f88d c004 	strb.w	ip, [sp, #4]
300086da:	f88d 3001 	strb.w	r3, [sp, #1]
300086de:	f88d c005 	strb.w	ip, [sp, #5]
300086e2:	47a0      	blx	r4
300086e4:	aa01      	add	r2, sp, #4
300086e6:	2102      	movs	r1, #2
300086e8:	2004      	movs	r0, #4
300086ea:	47a0      	blx	r4
300086ec:	b003      	add	sp, #12
300086ee:	bd30      	pop	{r4, r5, pc}
300086f0:	3000f3a0 	.word	0x3000f3a0
300086f4:	3000e83c 	.word	0x3000e83c
300086f8:	3000861d 	.word	0x3000861d

300086fc <PSRAM_WB_REG_Write>:
300086fc:	b430      	push	{r4, r5}
300086fe:	4c1e      	ldr	r4, [pc, #120]	; (30008778 <PSRAM_WB_REG_Write+0x7c>)
30008700:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008702:	f013 0301 	ands.w	r3, r3, #1
30008706:	d1fb      	bne.n	30008700 <PSRAM_WB_REG_Write+0x4>
30008708:	6825      	ldr	r5, [r4, #0]
3000870a:	b2c0      	uxtb	r0, r0
3000870c:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008710:	6025      	str	r5, [r4, #0]
30008712:	60a3      	str	r3, [r4, #8]
30008714:	6825      	ldr	r5, [r4, #0]
30008716:	f425 7540 	bic.w	r5, r5, #768	; 0x300
3000871a:	6025      	str	r5, [r4, #0]
3000871c:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30008720:	6063      	str	r3, [r4, #4]
30008722:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
30008726:	2560      	movs	r5, #96	; 0x60
30008728:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
3000872c:	2501      	movs	r5, #1
3000872e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008732:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30008736:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000873a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000873e:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30008742:	b141      	cbz	r1, 30008756 <PSRAM_WB_REG_Write+0x5a>
30008744:	3a01      	subs	r2, #1
30008746:	480c      	ldr	r0, [pc, #48]	; (30008778 <PSRAM_WB_REG_Write+0x7c>)
30008748:	4411      	add	r1, r2
3000874a:	f812 3f01 	ldrb.w	r3, [r2, #1]!
3000874e:	428a      	cmp	r2, r1
30008750:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30008754:	d1f9      	bne.n	3000874a <PSRAM_WB_REG_Write+0x4e>
30008756:	4a08      	ldr	r2, [pc, #32]	; (30008778 <PSRAM_WB_REG_Write+0x7c>)
30008758:	2301      	movs	r3, #1
3000875a:	6093      	str	r3, [r2, #8]
3000875c:	6893      	ldr	r3, [r2, #8]
3000875e:	07d9      	lsls	r1, r3, #31
30008760:	d4fc      	bmi.n	3000875c <PSRAM_WB_REG_Write+0x60>
30008762:	4a05      	ldr	r2, [pc, #20]	; (30008778 <PSRAM_WB_REG_Write+0x7c>)
30008764:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008766:	07db      	lsls	r3, r3, #31
30008768:	d4fc      	bmi.n	30008764 <PSRAM_WB_REG_Write+0x68>
3000876a:	6813      	ldr	r3, [r2, #0]
3000876c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008770:	6013      	str	r3, [r2, #0]
30008772:	bc30      	pop	{r4, r5}
30008774:	4770      	bx	lr
30008776:	bf00      	nop
30008778:	41012000 	.word	0x41012000

3000877c <PSRAM_WB_DEVIC_Init>:
3000877c:	490b      	ldr	r1, [pc, #44]	; (300087ac <PSRAM_WB_DEVIC_Init+0x30>)
3000877e:	4b0c      	ldr	r3, [pc, #48]	; (300087b0 <PSRAM_WB_DEVIC_Init+0x34>)
30008780:	b510      	push	{r4, lr}
30008782:	6988      	ldr	r0, [r1, #24]
30008784:	b082      	sub	sp, #8
30008786:	248f      	movs	r4, #143	; 0x8f
30008788:	2102      	movs	r1, #2
3000878a:	4403      	add	r3, r0
3000878c:	aa01      	add	r2, sp, #4
3000878e:	f88d 4004 	strb.w	r4, [sp, #4]
30008792:	2000      	movs	r0, #0
30008794:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30008798:	4c06      	ldr	r4, [pc, #24]	; (300087b4 <PSRAM_WB_DEVIC_Init+0x38>)
3000879a:	011b      	lsls	r3, r3, #4
3000879c:	f043 030c 	orr.w	r3, r3, #12
300087a0:	f88d 3005 	strb.w	r3, [sp, #5]
300087a4:	47a0      	blx	r4
300087a6:	b002      	add	sp, #8
300087a8:	bd10      	pop	{r4, pc}
300087aa:	bf00      	nop
300087ac:	3000f3a0 	.word	0x3000f3a0
300087b0:	3000e85c 	.word	0x3000e85c
300087b4:	300086fd 	.word	0x300086fd

300087b8 <PSRAM_calibration>:
300087b8:	4a9c      	ldr	r2, [pc, #624]	; (30008a2c <PSRAM_calibration+0x274>)
300087ba:	2300      	movs	r3, #0
300087bc:	499c      	ldr	r1, [pc, #624]	; (30008a30 <PSRAM_calibration+0x278>)
300087be:	489d      	ldr	r0, [pc, #628]	; (30008a34 <PSRAM_calibration+0x27c>)
300087c0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300087c4:	6852      	ldr	r2, [r2, #4]
300087c6:	b093      	sub	sp, #76	; 0x4c
300087c8:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
300087cc:	400a      	ands	r2, r1
300087ce:	9311      	str	r3, [sp, #68]	; 0x44
300087d0:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
300087d4:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
300087d8:	920a      	str	r2, [sp, #40]	; 0x28
300087da:	6942      	ldr	r2, [r0, #20]
300087dc:	03d1      	lsls	r1, r2, #15
300087de:	d51e      	bpl.n	3000881e <PSRAM_calibration+0x66>
300087e0:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300087e4:	f3bf 8f4f 	dsb	sy
300087e8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300087ec:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300087f0:	f3c5 344e 	ubfx	r4, r5, #13, #15
300087f4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300087f8:	0164      	lsls	r4, r4, #5
300087fa:	ea04 0106 	and.w	r1, r4, r6
300087fe:	462b      	mov	r3, r5
30008800:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30008804:	3b01      	subs	r3, #1
30008806:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000880a:	1c5a      	adds	r2, r3, #1
3000880c:	d1f8      	bne.n	30008800 <PSRAM_calibration+0x48>
3000880e:	3c20      	subs	r4, #32
30008810:	f114 0f20 	cmn.w	r4, #32
30008814:	d1f1      	bne.n	300087fa <PSRAM_calibration+0x42>
30008816:	f3bf 8f4f 	dsb	sy
3000881a:	f3bf 8f6f 	isb	sy
3000881e:	4885      	ldr	r0, [pc, #532]	; (30008a34 <PSRAM_calibration+0x27c>)
30008820:	2300      	movs	r3, #0
30008822:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008826:	f3bf 8f4f 	dsb	sy
3000882a:	6943      	ldr	r3, [r0, #20]
3000882c:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30008830:	6143      	str	r3, [r0, #20]
30008832:	f3bf 8f4f 	dsb	sy
30008836:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000883a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000883e:	f3c5 344e 	ubfx	r4, r5, #13, #15
30008842:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30008846:	0164      	lsls	r4, r4, #5
30008848:	ea04 0106 	and.w	r1, r4, r6
3000884c:	462b      	mov	r3, r5
3000884e:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30008852:	3b01      	subs	r3, #1
30008854:	1c5f      	adds	r7, r3, #1
30008856:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000885a:	d1f8      	bne.n	3000884e <PSRAM_calibration+0x96>
3000885c:	3c20      	subs	r4, #32
3000885e:	f114 0f20 	cmn.w	r4, #32
30008862:	d1f1      	bne.n	30008848 <PSRAM_calibration+0x90>
30008864:	f3bf 8f4f 	dsb	sy
30008868:	f3bf 8f6f 	isb	sy
3000886c:	2204      	movs	r2, #4
3000886e:	461e      	mov	r6, r3
30008870:	9306      	str	r3, [sp, #24]
30008872:	2700      	movs	r7, #0
30008874:	9209      	str	r2, [sp, #36]	; 0x24
30008876:	46b3      	mov	fp, r6
30008878:	4a6c      	ldr	r2, [pc, #432]	; (30008a2c <PSRAM_calibration+0x274>)
3000887a:	4c6f      	ldr	r4, [pc, #444]	; (30008a38 <PSRAM_calibration+0x280>)
3000887c:	6813      	ldr	r3, [r2, #0]
3000887e:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 30008a54 <PSRAM_calibration+0x29c>
30008882:	f023 0301 	bic.w	r3, r3, #1
30008886:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 30008a58 <PSRAM_calibration+0x2a0>
3000888a:	6013      	str	r3, [r2, #0]
3000888c:	2301      	movs	r3, #1
3000888e:	9305      	str	r3, [sp, #20]
30008890:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008892:	9a05      	ldr	r2, [sp, #20]
30008894:	9704      	str	r7, [sp, #16]
30008896:	2700      	movs	r7, #0
30008898:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
3000889c:	463d      	mov	r5, r7
3000889e:	e9cd b607 	strd	fp, r6, [sp, #28]
300088a2:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300088a6:	465e      	mov	r6, fp
300088a8:	e010      	b.n	300088cc <PSRAM_calibration+0x114>
300088aa:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
300088ae:	bf08      	it	eq
300088b0:	462e      	moveq	r6, r5
300088b2:	2d1f      	cmp	r5, #31
300088b4:	eb06 0b07 	add.w	fp, r6, r7
300088b8:	f107 0701 	add.w	r7, r7, #1
300088bc:	d102      	bne.n	300088c4 <PSRAM_calibration+0x10c>
300088be:	9b04      	ldr	r3, [sp, #16]
300088c0:	429f      	cmp	r7, r3
300088c2:	dc46      	bgt.n	30008952 <PSRAM_calibration+0x19a>
300088c4:	3501      	adds	r5, #1
300088c6:	2d20      	cmp	r5, #32
300088c8:	f000 80ac 	beq.w	30008a24 <PSRAM_calibration+0x26c>
300088cc:	ea48 0205 	orr.w	r2, r8, r5
300088d0:	4b56      	ldr	r3, [pc, #344]	; (30008a2c <PSRAM_calibration+0x274>)
300088d2:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
300088d6:	4959      	ldr	r1, [pc, #356]	; (30008a3c <PSRAM_calibration+0x284>)
300088d8:	605a      	str	r2, [r3, #4]
300088da:	6822      	ldr	r2, [r4, #0]
300088dc:	4b58      	ldr	r3, [pc, #352]	; (30008a40 <PSRAM_calibration+0x288>)
300088de:	f8ce 2000 	str.w	r2, [lr]
300088e2:	6860      	ldr	r0, [r4, #4]
300088e4:	4a57      	ldr	r2, [pc, #348]	; (30008a44 <PSRAM_calibration+0x28c>)
300088e6:	f8c9 0000 	str.w	r0, [r9]
300088ea:	68a0      	ldr	r0, [r4, #8]
300088ec:	f8ca 0000 	str.w	r0, [sl]
300088f0:	68e0      	ldr	r0, [r4, #12]
300088f2:	6018      	str	r0, [r3, #0]
300088f4:	6920      	ldr	r0, [r4, #16]
300088f6:	6008      	str	r0, [r1, #0]
300088f8:	6960      	ldr	r0, [r4, #20]
300088fa:	6010      	str	r0, [r2, #0]
300088fc:	f8de 0000 	ldr.w	r0, [lr]
30008900:	900c      	str	r0, [sp, #48]	; 0x30
30008902:	f8d9 0000 	ldr.w	r0, [r9]
30008906:	900d      	str	r0, [sp, #52]	; 0x34
30008908:	f8da 0000 	ldr.w	r0, [sl]
3000890c:	900e      	str	r0, [sp, #56]	; 0x38
3000890e:	6818      	ldr	r0, [r3, #0]
30008910:	4b4d      	ldr	r3, [pc, #308]	; (30008a48 <PSRAM_calibration+0x290>)
30008912:	900f      	str	r0, [sp, #60]	; 0x3c
30008914:	a80c      	add	r0, sp, #48	; 0x30
30008916:	6809      	ldr	r1, [r1, #0]
30008918:	9110      	str	r1, [sp, #64]	; 0x40
3000891a:	6812      	ldr	r2, [r2, #0]
3000891c:	4946      	ldr	r1, [pc, #280]	; (30008a38 <PSRAM_calibration+0x280>)
3000891e:	9211      	str	r2, [sp, #68]	; 0x44
30008920:	2218      	movs	r2, #24
30008922:	4798      	blx	r3
30008924:	2800      	cmp	r0, #0
30008926:	d0c0      	beq.n	300088aa <PSRAM_calibration+0xf2>
30008928:	1c70      	adds	r0, r6, #1
3000892a:	d0cb      	beq.n	300088c4 <PSRAM_calibration+0x10c>
3000892c:	9b04      	ldr	r3, [sp, #16]
3000892e:	42bb      	cmp	r3, r7
30008930:	da0a      	bge.n	30008948 <PSRAM_calibration+0x190>
30008932:	9b05      	ldr	r3, [sp, #20]
30008934:	f8cd b01c 	str.w	fp, [sp, #28]
30008938:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
3000893c:	9704      	str	r7, [sp, #16]
3000893e:	2700      	movs	r7, #0
30008940:	9608      	str	r6, [sp, #32]
30008942:	465e      	mov	r6, fp
30008944:	9306      	str	r3, [sp, #24]
30008946:	e7bd      	b.n	300088c4 <PSRAM_calibration+0x10c>
30008948:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
3000894c:	2700      	movs	r7, #0
3000894e:	465e      	mov	r6, fp
30008950:	e7b8      	b.n	300088c4 <PSRAM_calibration+0x10c>
30008952:	9b05      	ldr	r3, [sp, #20]
30008954:	9306      	str	r3, [sp, #24]
30008956:	9b05      	ldr	r3, [sp, #20]
30008958:	005b      	lsls	r3, r3, #1
3000895a:	9305      	str	r3, [sp, #20]
3000895c:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000895e:	3b01      	subs	r3, #1
30008960:	9309      	str	r3, [sp, #36]	; 0x24
30008962:	d195      	bne.n	30008890 <PSRAM_calibration+0xd8>
30008964:	9b06      	ldr	r3, [sp, #24]
30008966:	2004      	movs	r0, #4
30008968:	4a38      	ldr	r2, [pc, #224]	; (30008a4c <PSRAM_calibration+0x294>)
3000896a:	e9cd 7302 	strd	r7, r3, [sp, #8]
3000896e:	4b38      	ldr	r3, [pc, #224]	; (30008a50 <PSRAM_calibration+0x298>)
30008970:	e9cd 6b00 	strd	r6, fp, [sp]
30008974:	4619      	mov	r1, r3
30008976:	f002 fe1d 	bl	3000b5b4 <rtk_log_write>
3000897a:	482e      	ldr	r0, [pc, #184]	; (30008a34 <PSRAM_calibration+0x27c>)
3000897c:	6943      	ldr	r3, [r0, #20]
3000897e:	03d9      	lsls	r1, r3, #15
30008980:	d425      	bmi.n	300089ce <PSRAM_calibration+0x216>
30008982:	9b09      	ldr	r3, [sp, #36]	; 0x24
30008984:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008988:	f3bf 8f4f 	dsb	sy
3000898c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30008990:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30008994:	f3c5 344e 	ubfx	r4, r5, #13, #15
30008998:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000899c:	0164      	lsls	r4, r4, #5
3000899e:	ea04 010c 	and.w	r1, r4, ip
300089a2:	462b      	mov	r3, r5
300089a4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300089a8:	3b01      	subs	r3, #1
300089aa:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300089ae:	1c5a      	adds	r2, r3, #1
300089b0:	d1f8      	bne.n	300089a4 <PSRAM_calibration+0x1ec>
300089b2:	3c20      	subs	r4, #32
300089b4:	f114 0f20 	cmn.w	r4, #32
300089b8:	d1f1      	bne.n	3000899e <PSRAM_calibration+0x1e6>
300089ba:	f3bf 8f4f 	dsb	sy
300089be:	6943      	ldr	r3, [r0, #20]
300089c0:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300089c4:	6143      	str	r3, [r0, #20]
300089c6:	f3bf 8f4f 	dsb	sy
300089ca:	f3bf 8f6f 	isb	sy
300089ce:	2f08      	cmp	r7, #8
300089d0:	dd24      	ble.n	30008a1c <PSRAM_calibration+0x264>
300089d2:	ebbb 0306 	subs.w	r3, fp, r6
300089d6:	9806      	ldr	r0, [sp, #24]
300089d8:	bf48      	it	mi
300089da:	3301      	addmi	r3, #1
300089dc:	eb16 060b 	adds.w	r6, r6, fp
300089e0:	ea4f 0363 	mov.w	r3, r3, asr #1
300089e4:	bf48      	it	mi
300089e6:	3601      	addmi	r6, #1
300089e8:	1e9a      	subs	r2, r3, #2
300089ea:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300089ec:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
300089f0:	0414      	lsls	r4, r2, #16
300089f2:	f3c6 0344 	ubfx	r3, r6, #1, #5
300089f6:	0212      	lsls	r2, r2, #8
300089f8:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
300089fc:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30008a00:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30008a04:	2001      	movs	r0, #1
30008a06:	430b      	orrs	r3, r1
30008a08:	4908      	ldr	r1, [pc, #32]	; (30008a2c <PSRAM_calibration+0x274>)
30008a0a:	4323      	orrs	r3, r4
30008a0c:	4313      	orrs	r3, r2
30008a0e:	604b      	str	r3, [r1, #4]
30008a10:	680b      	ldr	r3, [r1, #0]
30008a12:	4303      	orrs	r3, r0
30008a14:	600b      	str	r3, [r1, #0]
30008a16:	b013      	add	sp, #76	; 0x4c
30008a18:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008a1c:	2000      	movs	r0, #0
30008a1e:	b013      	add	sp, #76	; 0x4c
30008a20:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008a24:	9f04      	ldr	r7, [sp, #16]
30008a26:	e9dd b607 	ldrd	fp, r6, [sp, #28]
30008a2a:	e794      	b.n	30008956 <PSRAM_calibration+0x19e>
30008a2c:	41002000 	.word	0x41002000
30008a30:	0fffffe0 	.word	0x0fffffe0
30008a34:	e000ed00 	.word	0xe000ed00
30008a38:	3000e844 	.word	0x3000e844
30008a3c:	60200000 	.word	0x60200000
30008a40:	60150000 	.word	0x60150000
30008a44:	60250000 	.word	0x60250000
30008a48:	000129bd 	.word	0x000129bd
30008a4c:	3000d048 	.word	0x3000d048
30008a50:	3000d020 	.word	0x3000d020
30008a54:	60050000 	.word	0x60050000
30008a58:	60100000 	.word	0x60100000

30008a5c <PSRAM_AutoGating>:
30008a5c:	b1c0      	cbz	r0, 30008a90 <PSRAM_AutoGating+0x34>
30008a5e:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30008a62:	4810      	ldr	r0, [pc, #64]	; (30008aa4 <PSRAM_AutoGating+0x48>)
30008a64:	f3c1 0108 	ubfx	r1, r1, #0, #9
30008a68:	0252      	lsls	r2, r2, #9
30008a6a:	b410      	push	{r4}
30008a6c:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
30008a70:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
30008a74:	4020      	ands	r0, r4
30008a76:	4301      	orrs	r1, r0
30008a78:	430a      	orrs	r2, r1
30008a7a:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30008a7e:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
30008a82:	f042 0201 	orr.w	r2, r2, #1
30008a86:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30008a8a:	f85d 4b04 	ldr.w	r4, [sp], #4
30008a8e:	4770      	bx	lr
30008a90:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30008a94:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30008a98:	f023 0301 	bic.w	r3, r3, #1
30008a9c:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30008aa0:	4770      	bx	lr
30008aa2:	bf00      	nop
30008aa4:	ffffc000 	.word	0xffffc000

30008aa8 <BOOT_Reason>:
30008aa8:	4b02      	ldr	r3, [pc, #8]	; (30008ab4 <BOOT_Reason+0xc>)
30008aaa:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30008aae:	b280      	uxth	r0, r0
30008ab0:	4770      	bx	lr
30008ab2:	bf00      	nop
30008ab4:	42008200 	.word	0x42008200

30008ab8 <CRYPTO_SetSecurityModeAD>:
30008ab8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008abc:	b083      	sub	sp, #12
30008abe:	460d      	mov	r5, r1
30008ac0:	4616      	mov	r6, r2
30008ac2:	4699      	mov	r9, r3
30008ac4:	4604      	mov	r4, r0
30008ac6:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
30008aca:	2800      	cmp	r0, #0
30008acc:	f000 80a9 	beq.w	30008c22 <CRYPTO_SetSecurityModeAD+0x16a>
30008ad0:	7823      	ldrb	r3, [r4, #0]
30008ad2:	2b01      	cmp	r3, #1
30008ad4:	d003      	beq.n	30008ade <CRYPTO_SetSecurityModeAD+0x26>
30008ad6:	2125      	movs	r1, #37	; 0x25
30008ad8:	485d      	ldr	r0, [pc, #372]	; (30008c50 <CRYPTO_SetSecurityModeAD+0x198>)
30008ada:	f003 f849 	bl	3000bb70 <__io_assert_failed_veneer>
30008ade:	2300      	movs	r3, #0
30008ae0:	1c6a      	adds	r2, r5, #1
30008ae2:	6065      	str	r5, [r4, #4]
30008ae4:	8123      	strh	r3, [r4, #8]
30008ae6:	d008      	beq.n	30008afa <CRYPTO_SetSecurityModeAD+0x42>
30008ae8:	f005 0330 	and.w	r3, r5, #48	; 0x30
30008aec:	2b20      	cmp	r3, #32
30008aee:	d07b      	beq.n	30008be8 <CRYPTO_SetSecurityModeAD+0x130>
30008af0:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30008af4:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30008af8:	7265      	strb	r5, [r4, #9]
30008afa:	1c73      	adds	r3, r6, #1
30008afc:	60e6      	str	r6, [r4, #12]
30008afe:	f000 8082 	beq.w	30008c06 <CRYPTO_SetSecurityModeAD+0x14e>
30008b02:	f006 0303 	and.w	r3, r6, #3
30008b06:	f3c6 0280 	ubfx	r2, r6, #2, #1
30008b0a:	7422      	strb	r2, [r4, #16]
30008b0c:	f1a3 0202 	sub.w	r2, r3, #2
30008b10:	2b01      	cmp	r3, #1
30008b12:	fab2 f282 	clz	r2, r2
30008b16:	ea4f 1252 	mov.w	r2, r2, lsr #5
30008b1a:	7462      	strb	r2, [r4, #17]
30008b1c:	fab3 f283 	clz	r2, r3
30008b20:	ea4f 1252 	mov.w	r2, r2, lsr #5
30008b24:	74a2      	strb	r2, [r4, #18]
30008b26:	bf0c      	ite	eq
30008b28:	2201      	moveq	r2, #1
30008b2a:	2200      	movne	r2, #0
30008b2c:	74e2      	strb	r2, [r4, #19]
30008b2e:	d13f      	bne.n	30008bb0 <CRYPTO_SetSecurityModeAD+0xf8>
30008b30:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30008b34:	2b40      	cmp	r3, #64	; 0x40
30008b36:	f000 8083 	beq.w	30008c40 <CRYPTO_SetSecurityModeAD+0x188>
30008b3a:	d86a      	bhi.n	30008c12 <CRYPTO_SetSecurityModeAD+0x15a>
30008b3c:	2b10      	cmp	r3, #16
30008b3e:	d06d      	beq.n	30008c1c <CRYPTO_SetSecurityModeAD+0x164>
30008b40:	2b20      	cmp	r3, #32
30008b42:	d173      	bne.n	30008c2c <CRYPTO_SetSecurityModeAD+0x174>
30008b44:	7523      	strb	r3, [r4, #20]
30008b46:	62a3      	str	r3, [r4, #40]	; 0x28
30008b48:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008b4a:	6227      	str	r7, [r4, #32]
30008b4c:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30008b50:	2f00      	cmp	r7, #0
30008b52:	d038      	beq.n	30008bc6 <CRYPTO_SetSecurityModeAD+0x10e>
30008b54:	7d23      	ldrb	r3, [r4, #20]
30008b56:	f104 0064 	add.w	r0, r4, #100	; 0x64
30008b5a:	2b30      	cmp	r3, #48	; 0x30
30008b5c:	65e0      	str	r0, [r4, #92]	; 0x5c
30008b5e:	d046      	beq.n	30008bee <CRYPTO_SetSecurityModeAD+0x136>
30008b60:	2b40      	cmp	r3, #64	; 0x40
30008b62:	d044      	beq.n	30008bee <CRYPTO_SetSecurityModeAD+0x136>
30008b64:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30008b68:	2240      	movs	r2, #64	; 0x40
30008b6a:	2136      	movs	r1, #54	; 0x36
30008b6c:	4d39      	ldr	r5, [pc, #228]	; (30008c54 <CRYPTO_SetSecurityModeAD+0x19c>)
30008b6e:	6623      	str	r3, [r4, #96]	; 0x60
30008b70:	47a8      	blx	r5
30008b72:	2240      	movs	r2, #64	; 0x40
30008b74:	215c      	movs	r1, #92	; 0x5c
30008b76:	6e20      	ldr	r0, [r4, #96]	; 0x60
30008b78:	47a8      	blx	r5
30008b7a:	4b37      	ldr	r3, [pc, #220]	; (30008c58 <CRYPTO_SetSecurityModeAD+0x1a0>)
30008b7c:	681d      	ldr	r5, [r3, #0]
30008b7e:	bb25      	cbnz	r5, 30008bca <CRYPTO_SetSecurityModeAD+0x112>
30008b80:	f1b8 0f00 	cmp.w	r8, #0
30008b84:	d05f      	beq.n	30008c46 <CRYPTO_SetSecurityModeAD+0x18e>
30008b86:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30008b8a:	e001      	b.n	30008b90 <CRYPTO_SetSecurityModeAD+0xd8>
30008b8c:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30008b90:	6de2      	ldr	r2, [r4, #92]	; 0x5c
30008b92:	f818 3005 	ldrb.w	r3, [r8, r5]
30008b96:	5d51      	ldrb	r1, [r2, r5]
30008b98:	404b      	eors	r3, r1
30008b9a:	5553      	strb	r3, [r2, r5]
30008b9c:	6e22      	ldr	r2, [r4, #96]	; 0x60
30008b9e:	6a63      	ldr	r3, [r4, #36]	; 0x24
30008ba0:	5d51      	ldrb	r1, [r2, r5]
30008ba2:	5d5b      	ldrb	r3, [r3, r5]
30008ba4:	404b      	eors	r3, r1
30008ba6:	5553      	strb	r3, [r2, r5]
30008ba8:	3501      	adds	r5, #1
30008baa:	42af      	cmp	r7, r5
30008bac:	d1ee      	bne.n	30008b8c <CRYPTO_SetSecurityModeAD+0xd4>
30008bae:	e00c      	b.n	30008bca <CRYPTO_SetSecurityModeAD+0x112>
30008bb0:	2200      	movs	r2, #0
30008bb2:	7522      	strb	r2, [r4, #20]
30008bb4:	b99b      	cbnz	r3, 30008bde <CRYPTO_SetSecurityModeAD+0x126>
30008bb6:	2314      	movs	r3, #20
30008bb8:	6227      	str	r7, [r4, #32]
30008bba:	62a3      	str	r3, [r4, #40]	; 0x28
30008bbc:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008bbe:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30008bc2:	2f00      	cmp	r7, #0
30008bc4:	d1c6      	bne.n	30008b54 <CRYPTO_SetSecurityModeAD+0x9c>
30008bc6:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30008bca:	3601      	adds	r6, #1
30008bcc:	d003      	beq.n	30008bd6 <CRYPTO_SetSecurityModeAD+0x11e>
30008bce:	2201      	movs	r2, #1
30008bd0:	2300      	movs	r3, #0
30008bd2:	85a2      	strh	r2, [r4, #44]	; 0x2c
30008bd4:	6323      	str	r3, [r4, #48]	; 0x30
30008bd6:	2000      	movs	r0, #0
30008bd8:	b003      	add	sp, #12
30008bda:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008bde:	2b02      	cmp	r3, #2
30008be0:	d114      	bne.n	30008c0c <CRYPTO_SetSecurityModeAD+0x154>
30008be2:	2310      	movs	r3, #16
30008be4:	62a3      	str	r3, [r4, #40]	; 0x28
30008be6:	e7af      	b.n	30008b48 <CRYPTO_SetSecurityModeAD+0x90>
30008be8:	2301      	movs	r3, #1
30008bea:	7223      	strb	r3, [r4, #8]
30008bec:	e780      	b.n	30008af0 <CRYPTO_SetSecurityModeAD+0x38>
30008bee:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30008bf2:	2280      	movs	r2, #128	; 0x80
30008bf4:	2136      	movs	r1, #54	; 0x36
30008bf6:	4d17      	ldr	r5, [pc, #92]	; (30008c54 <CRYPTO_SetSecurityModeAD+0x19c>)
30008bf8:	6623      	str	r3, [r4, #96]	; 0x60
30008bfa:	47a8      	blx	r5
30008bfc:	2280      	movs	r2, #128	; 0x80
30008bfe:	215c      	movs	r1, #92	; 0x5c
30008c00:	6e20      	ldr	r0, [r4, #96]	; 0x60
30008c02:	47a8      	blx	r5
30008c04:	e7b9      	b.n	30008b7a <CRYPTO_SetSecurityModeAD+0xc2>
30008c06:	2300      	movs	r3, #0
30008c08:	6123      	str	r3, [r4, #16]
30008c0a:	7523      	strb	r3, [r4, #20]
30008c0c:	2300      	movs	r3, #0
30008c0e:	62a3      	str	r3, [r4, #40]	; 0x28
30008c10:	e79a      	b.n	30008b48 <CRYPTO_SetSecurityModeAD+0x90>
30008c12:	2b80      	cmp	r3, #128	; 0x80
30008c14:	d10a      	bne.n	30008c2c <CRYPTO_SetSecurityModeAD+0x174>
30008c16:	2340      	movs	r3, #64	; 0x40
30008c18:	7523      	strb	r3, [r4, #20]
30008c1a:	e794      	b.n	30008b46 <CRYPTO_SetSecurityModeAD+0x8e>
30008c1c:	231c      	movs	r3, #28
30008c1e:	7523      	strb	r3, [r4, #20]
30008c20:	e7f5      	b.n	30008c0e <CRYPTO_SetSecurityModeAD+0x156>
30008c22:	2124      	movs	r1, #36	; 0x24
30008c24:	480a      	ldr	r0, [pc, #40]	; (30008c50 <CRYPTO_SetSecurityModeAD+0x198>)
30008c26:	f002 ffa3 	bl	3000bb70 <__io_assert_failed_veneer>
30008c2a:	e751      	b.n	30008ad0 <CRYPTO_SetSecurityModeAD+0x18>
30008c2c:	4b0b      	ldr	r3, [pc, #44]	; (30008c5c <CRYPTO_SetSecurityModeAD+0x1a4>)
30008c2e:	2002      	movs	r0, #2
30008c30:	4a0b      	ldr	r2, [pc, #44]	; (30008c60 <CRYPTO_SetSecurityModeAD+0x1a8>)
30008c32:	4619      	mov	r1, r3
30008c34:	9600      	str	r6, [sp, #0]
30008c36:	f002 fcbd 	bl	3000b5b4 <rtk_log_write>
30008c3a:	2300      	movs	r3, #0
30008c3c:	7523      	strb	r3, [r4, #20]
30008c3e:	e7e6      	b.n	30008c0e <CRYPTO_SetSecurityModeAD+0x156>
30008c40:	2330      	movs	r3, #48	; 0x30
30008c42:	7523      	strb	r3, [r4, #20]
30008c44:	e7e3      	b.n	30008c0e <CRYPTO_SetSecurityModeAD+0x156>
30008c46:	2180      	movs	r1, #128	; 0x80
30008c48:	4801      	ldr	r0, [pc, #4]	; (30008c50 <CRYPTO_SetSecurityModeAD+0x198>)
30008c4a:	f002 ff91 	bl	3000bb70 <__io_assert_failed_veneer>
30008c4e:	e79a      	b.n	30008b86 <CRYPTO_SetSecurityModeAD+0xce>
30008c50:	3000d0e8 	.word	0x3000d0e8
30008c54:	00012be5 	.word	0x00012be5
30008c58:	2001c200 	.word	0x2001c200
30008c5c:	3000d088 	.word	0x3000d088
30008c60:	3000d094 	.word	0x3000d094

30008c64 <rtl_crypto_hmac_sha2_init>:
30008c64:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008c68:	4c97      	ldr	r4, [pc, #604]	; (30008ec8 <rtl_crypto_hmac_sha2_init+0x264>)
30008c6a:	4617      	mov	r7, r2
30008c6c:	e844 f400 	tt	r4, r4
30008c70:	4a96      	ldr	r2, [pc, #600]	; (30008ecc <rtl_crypto_hmac_sha2_init+0x268>)
30008c72:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
30008c76:	461c      	mov	r4, r3
30008c78:	4b95      	ldr	r3, [pc, #596]	; (30008ed0 <rtl_crypto_hmac_sha2_init+0x26c>)
30008c7a:	bf18      	it	ne
30008c7c:	4613      	movne	r3, r2
30008c7e:	b085      	sub	sp, #20
30008c80:	4605      	mov	r5, r0
30008c82:	460e      	mov	r6, r1
30008c84:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30008c86:	2b00      	cmp	r3, #0
30008c88:	db05      	blt.n	30008c96 <rtl_crypto_hmac_sha2_init+0x32>
30008c8a:	2900      	cmp	r1, #0
30008c8c:	f000 8118 	beq.w	30008ec0 <rtl_crypto_hmac_sha2_init+0x25c>
30008c90:	078b      	lsls	r3, r1, #30
30008c92:	f040 8112 	bne.w	30008eba <rtl_crypto_hmac_sha2_init+0x256>
30008c96:	2d20      	cmp	r5, #32
30008c98:	bf18      	it	ne
30008c9a:	2d1c      	cmpne	r5, #28
30008c9c:	bf0c      	ite	eq
30008c9e:	f04f 0901 	moveq.w	r9, #1
30008ca2:	f04f 0900 	movne.w	r9, #0
30008ca6:	d04d      	beq.n	30008d44 <rtl_crypto_hmac_sha2_init+0xe0>
30008ca8:	2d40      	cmp	r5, #64	; 0x40
30008caa:	bf18      	it	ne
30008cac:	2d30      	cmpne	r5, #48	; 0x30
30008cae:	d102      	bne.n	30008cb6 <rtl_crypto_hmac_sha2_init+0x52>
30008cb0:	2f80      	cmp	r7, #128	; 0x80
30008cb2:	f200 80f9 	bhi.w	30008ea8 <rtl_crypto_hmac_sha2_init+0x244>
30008cb6:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30008ef0 <rtl_crypto_hmac_sha2_init+0x28c>
30008cba:	f898 3000 	ldrb.w	r3, [r8]
30008cbe:	2b01      	cmp	r3, #1
30008cc0:	d003      	beq.n	30008cca <rtl_crypto_hmac_sha2_init+0x66>
30008cc2:	21b4      	movs	r1, #180	; 0xb4
30008cc4:	4883      	ldr	r0, [pc, #524]	; (30008ed4 <rtl_crypto_hmac_sha2_init+0x270>)
30008cc6:	f002 ff53 	bl	3000bb70 <__io_assert_failed_veneer>
30008cca:	2d30      	cmp	r5, #48	; 0x30
30008ccc:	f000 809b 	beq.w	30008e06 <rtl_crypto_hmac_sha2_init+0x1a2>
30008cd0:	d83f      	bhi.n	30008d52 <rtl_crypto_hmac_sha2_init+0xee>
30008cd2:	2d1c      	cmp	r5, #28
30008cd4:	f000 80cb 	beq.w	30008e6e <rtl_crypto_hmac_sha2_init+0x20a>
30008cd8:	f1b9 0f00 	cmp.w	r9, #0
30008cdc:	d078      	beq.n	30008dd0 <rtl_crypto_hmac_sha2_init+0x16c>
30008cde:	2c00      	cmp	r4, #0
30008ce0:	f000 808f 	beq.w	30008e02 <rtl_crypto_hmac_sha2_init+0x19e>
30008ce4:	4b7c      	ldr	r3, [pc, #496]	; (30008ed8 <rtl_crypto_hmac_sha2_init+0x274>)
30008ce6:	f8df e210 	ldr.w	lr, [pc, #528]	; 30008ef8 <rtl_crypto_hmac_sha2_init+0x294>
30008cea:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30008f08 <rtl_crypto_hmac_sha2_init+0x2a4>
30008cee:	4a7b      	ldr	r2, [pc, #492]	; (30008edc <rtl_crypto_hmac_sha2_init+0x278>)
30008cf0:	e9c4 e302 	strd	lr, r3, [r4, #8]
30008cf4:	4b7a      	ldr	r3, [pc, #488]	; (30008ee0 <rtl_crypto_hmac_sha2_init+0x27c>)
30008cf6:	e9c4 9304 	strd	r9, r3, [r4, #16]
30008cfa:	4b7a      	ldr	r3, [pc, #488]	; (30008ee4 <rtl_crypto_hmac_sha2_init+0x280>)
30008cfc:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008d00:	4b79      	ldr	r3, [pc, #484]	; (30008ee8 <rtl_crypto_hmac_sha2_init+0x284>)
30008d02:	2225      	movs	r2, #37	; 0x25
30008d04:	6223      	str	r3, [r4, #32]
30008d06:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30008d0a:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30008d0e:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30008d12:	6263      	str	r3, [r4, #36]	; 0x24
30008d14:	2300      	movs	r3, #0
30008d16:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008d1a:	4d74      	ldr	r5, [pc, #464]	; (30008eec <rtl_crypto_hmac_sha2_init+0x288>)
30008d1c:	9300      	str	r3, [sp, #0]
30008d1e:	9702      	str	r7, [sp, #8]
30008d20:	9601      	str	r6, [sp, #4]
30008d22:	4873      	ldr	r0, [pc, #460]	; (30008ef0 <rtl_crypto_hmac_sha2_init+0x28c>)
30008d24:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30008d28:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30008d2c:	47a8      	blx	r5
30008d2e:	4605      	mov	r5, r0
30008d30:	4b70      	ldr	r3, [pc, #448]	; (30008ef4 <rtl_crypto_hmac_sha2_init+0x290>)
30008d32:	4620      	mov	r0, r4
30008d34:	4798      	blx	r3
30008d36:	2301      	movs	r3, #1
30008d38:	4628      	mov	r0, r5
30008d3a:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30008d3e:	b005      	add	sp, #20
30008d40:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008d44:	2f40      	cmp	r7, #64	; 0x40
30008d46:	d9b6      	bls.n	30008cb6 <rtl_crypto_hmac_sha2_init+0x52>
30008d48:	21ae      	movs	r1, #174	; 0xae
30008d4a:	4862      	ldr	r0, [pc, #392]	; (30008ed4 <rtl_crypto_hmac_sha2_init+0x270>)
30008d4c:	f002 ff10 	bl	3000bb70 <__io_assert_failed_veneer>
30008d50:	e7b1      	b.n	30008cb6 <rtl_crypto_hmac_sha2_init+0x52>
30008d52:	2d40      	cmp	r5, #64	; 0x40
30008d54:	d13c      	bne.n	30008dd0 <rtl_crypto_hmac_sha2_init+0x16c>
30008d56:	2c00      	cmp	r4, #0
30008d58:	d040      	beq.n	30008ddc <rtl_crypto_hmac_sha2_init+0x178>
30008d5a:	4b67      	ldr	r3, [pc, #412]	; (30008ef8 <rtl_crypto_hmac_sha2_init+0x294>)
30008d5c:	4a67      	ldr	r2, [pc, #412]	; (30008efc <rtl_crypto_hmac_sha2_init+0x298>)
30008d5e:	4968      	ldr	r1, [pc, #416]	; (30008f00 <rtl_crypto_hmac_sha2_init+0x29c>)
30008d60:	4d68      	ldr	r5, [pc, #416]	; (30008f04 <rtl_crypto_hmac_sha2_init+0x2a0>)
30008d62:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30008f48 <rtl_crypto_hmac_sha2_init+0x2e4>
30008d66:	e9c4 3202 	strd	r3, r2, [r4, #8]
30008d6a:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30008d6e:	2285      	movs	r2, #133	; 0x85
30008d70:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30008d74:	33d4      	adds	r3, #212	; 0xd4
30008d76:	6163      	str	r3, [r4, #20]
30008d78:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30008d7c:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30008d80:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30008d84:	6123      	str	r3, [r4, #16]
30008d86:	4b60      	ldr	r3, [pc, #384]	; (30008f08 <rtl_crypto_hmac_sha2_init+0x2a4>)
30008d88:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008d8c:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30008d90:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30008d94:	f203 337f 	addw	r3, r3, #895	; 0x37f
30008d98:	6263      	str	r3, [r4, #36]	; 0x24
30008d9a:	4b51      	ldr	r3, [pc, #324]	; (30008ee0 <rtl_crypto_hmac_sha2_init+0x27c>)
30008d9c:	6223      	str	r3, [r4, #32]
30008d9e:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30008da2:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30008da6:	f203 5397 	addw	r3, r3, #1431	; 0x597
30008daa:	62e3      	str	r3, [r4, #44]	; 0x2c
30008dac:	4b4b      	ldr	r3, [pc, #300]	; (30008edc <rtl_crypto_hmac_sha2_init+0x278>)
30008dae:	62a3      	str	r3, [r4, #40]	; 0x28
30008db0:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30008db4:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30008db8:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30008dbc:	6363      	str	r3, [r4, #52]	; 0x34
30008dbe:	4b49      	ldr	r3, [pc, #292]	; (30008ee4 <rtl_crypto_hmac_sha2_init+0x280>)
30008dc0:	6323      	str	r3, [r4, #48]	; 0x30
30008dc2:	4b49      	ldr	r3, [pc, #292]	; (30008ee8 <rtl_crypto_hmac_sha2_init+0x284>)
30008dc4:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008dc8:	4b50      	ldr	r3, [pc, #320]	; (30008f0c <rtl_crypto_hmac_sha2_init+0x2a8>)
30008dca:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008dce:	e7a1      	b.n	30008d14 <rtl_crypto_hmac_sha2_init+0xb0>
30008dd0:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
30008dd4:	4628      	mov	r0, r5
30008dd6:	b005      	add	sp, #20
30008dd8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008ddc:	2285      	movs	r2, #133	; 0x85
30008dde:	2300      	movs	r3, #0
30008de0:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008de4:	4c41      	ldr	r4, [pc, #260]	; (30008eec <rtl_crypto_hmac_sha2_init+0x288>)
30008de6:	9702      	str	r7, [sp, #8]
30008de8:	4841      	ldr	r0, [pc, #260]	; (30008ef0 <rtl_crypto_hmac_sha2_init+0x28c>)
30008dea:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30008dee:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30008df2:	e9cd 3600 	strd	r3, r6, [sp]
30008df6:	47a0      	blx	r4
30008df8:	4605      	mov	r5, r0
30008dfa:	4628      	mov	r0, r5
30008dfc:	b005      	add	sp, #20
30008dfe:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008e02:	2225      	movs	r2, #37	; 0x25
30008e04:	e7eb      	b.n	30008dde <rtl_crypto_hmac_sha2_init+0x17a>
30008e06:	2c00      	cmp	r4, #0
30008e08:	d053      	beq.n	30008eb2 <rtl_crypto_hmac_sha2_init+0x24e>
30008e0a:	4b41      	ldr	r3, [pc, #260]	; (30008f10 <rtl_crypto_hmac_sha2_init+0x2ac>)
30008e0c:	f8df e130 	ldr.w	lr, [pc, #304]	; 30008f40 <rtl_crypto_hmac_sha2_init+0x2dc>
30008e10:	4a40      	ldr	r2, [pc, #256]	; (30008f14 <rtl_crypto_hmac_sha2_init+0x2b0>)
30008e12:	4941      	ldr	r1, [pc, #260]	; (30008f18 <rtl_crypto_hmac_sha2_init+0x2b4>)
30008e14:	4d41      	ldr	r5, [pc, #260]	; (30008f1c <rtl_crypto_hmac_sha2_init+0x2b8>)
30008e16:	f8df c12c 	ldr.w	ip, [pc, #300]	; 30008f44 <rtl_crypto_hmac_sha2_init+0x2e0>
30008e1a:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30008e1e:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30008e22:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30008e26:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30008e2a:	6163      	str	r3, [r4, #20]
30008e2c:	4b3c      	ldr	r3, [pc, #240]	; (30008f20 <rtl_crypto_hmac_sha2_init+0x2bc>)
30008e2e:	6123      	str	r3, [r4, #16]
30008e30:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30008e34:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30008e38:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30008e3c:	61e3      	str	r3, [r4, #28]
30008e3e:	4b39      	ldr	r3, [pc, #228]	; (30008f24 <rtl_crypto_hmac_sha2_init+0x2c0>)
30008e40:	61a3      	str	r3, [r4, #24]
30008e42:	4b39      	ldr	r3, [pc, #228]	; (30008f28 <rtl_crypto_hmac_sha2_init+0x2c4>)
30008e44:	e9c4 3208 	strd	r3, r2, [r4, #32]
30008e48:	4b38      	ldr	r3, [pc, #224]	; (30008f2c <rtl_crypto_hmac_sha2_init+0x2c8>)
30008e4a:	2245      	movs	r2, #69	; 0x45
30008e4c:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30008e50:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30008e54:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30008e58:	33aa      	adds	r3, #170	; 0xaa
30008e5a:	6363      	str	r3, [r4, #52]	; 0x34
30008e5c:	4b34      	ldr	r3, [pc, #208]	; (30008f30 <rtl_crypto_hmac_sha2_init+0x2cc>)
30008e5e:	6323      	str	r3, [r4, #48]	; 0x30
30008e60:	4b34      	ldr	r3, [pc, #208]	; (30008f34 <rtl_crypto_hmac_sha2_init+0x2d0>)
30008e62:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008e66:	4b34      	ldr	r3, [pc, #208]	; (30008f38 <rtl_crypto_hmac_sha2_init+0x2d4>)
30008e68:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008e6c:	e752      	b.n	30008d14 <rtl_crypto_hmac_sha2_init+0xb0>
30008e6e:	b314      	cbz	r4, 30008eb6 <rtl_crypto_hmac_sha2_init+0x252>
30008e70:	4b32      	ldr	r3, [pc, #200]	; (30008f3c <rtl_crypto_hmac_sha2_init+0x2d8>)
30008e72:	2215      	movs	r2, #21
30008e74:	4932      	ldr	r1, [pc, #200]	; (30008f40 <rtl_crypto_hmac_sha2_init+0x2dc>)
30008e76:	4827      	ldr	r0, [pc, #156]	; (30008f14 <rtl_crypto_hmac_sha2_init+0x2b0>)
30008e78:	4d28      	ldr	r5, [pc, #160]	; (30008f1c <rtl_crypto_hmac_sha2_init+0x2b8>)
30008e7a:	e9c4 1302 	strd	r1, r3, [r4, #8]
30008e7e:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
30008e82:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
30008e86:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30008e8a:	6123      	str	r3, [r4, #16]
30008e8c:	4b22      	ldr	r3, [pc, #136]	; (30008f18 <rtl_crypto_hmac_sha2_init+0x2b4>)
30008e8e:	e9c4 0305 	strd	r0, r3, [r4, #20]
30008e92:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30008e96:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30008e9a:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30008e9e:	61e3      	str	r3, [r4, #28]
30008ea0:	4b28      	ldr	r3, [pc, #160]	; (30008f44 <rtl_crypto_hmac_sha2_init+0x2e0>)
30008ea2:	e9c4 5308 	strd	r5, r3, [r4, #32]
30008ea6:	e735      	b.n	30008d14 <rtl_crypto_hmac_sha2_init+0xb0>
30008ea8:	21b1      	movs	r1, #177	; 0xb1
30008eaa:	480a      	ldr	r0, [pc, #40]	; (30008ed4 <rtl_crypto_hmac_sha2_init+0x270>)
30008eac:	f002 fe60 	bl	3000bb70 <__io_assert_failed_veneer>
30008eb0:	e701      	b.n	30008cb6 <rtl_crypto_hmac_sha2_init+0x52>
30008eb2:	2245      	movs	r2, #69	; 0x45
30008eb4:	e793      	b.n	30008dde <rtl_crypto_hmac_sha2_init+0x17a>
30008eb6:	2215      	movs	r2, #21
30008eb8:	e791      	b.n	30008dde <rtl_crypto_hmac_sha2_init+0x17a>
30008eba:	f06f 0505 	mvn.w	r5, #5
30008ebe:	e789      	b.n	30008dd4 <rtl_crypto_hmac_sha2_init+0x170>
30008ec0:	f06f 0503 	mvn.w	r5, #3
30008ec4:	e786      	b.n	30008dd4 <rtl_crypto_hmac_sha2_init+0x170>
30008ec6:	bf00      	nop
30008ec8:	0000e5f9 	.word	0x0000e5f9
30008ecc:	500c8000 	.word	0x500c8000
30008ed0:	400c8000 	.word	0x400c8000
30008ed4:	3000d0cc 	.word	0x3000d0cc
30008ed8:	bb67ae85 	.word	0xbb67ae85
30008edc:	510e527f 	.word	0x510e527f
30008ee0:	a54ff53a 	.word	0xa54ff53a
30008ee4:	9b05688c 	.word	0x9b05688c
30008ee8:	1f83d9ab 	.word	0x1f83d9ab
30008eec:	30008ab9 	.word	0x30008ab9
30008ef0:	2001c460 	.word	0x2001c460
30008ef4:	00004c99 	.word	0x00004c99
30008ef8:	6a09e667 	.word	0x6a09e667
30008efc:	f3bcc908 	.word	0xf3bcc908
30008f00:	fe94f82b 	.word	0xfe94f82b
30008f04:	fb41bd6b 	.word	0xfb41bd6b
30008f08:	3c6ef372 	.word	0x3c6ef372
30008f0c:	5be0cd19 	.word	0x5be0cd19
30008f10:	cbbb9d5d 	.word	0xcbbb9d5d
30008f14:	f70e5939 	.word	0xf70e5939
30008f18:	ffc00b31 	.word	0xffc00b31
30008f1c:	64f98fa7 	.word	0x64f98fa7
30008f20:	629a292a 	.word	0x629a292a
30008f24:	9159015a 	.word	0x9159015a
30008f28:	152fecd8 	.word	0x152fecd8
30008f2c:	67332667 	.word	0x67332667
30008f30:	8eb44a87 	.word	0x8eb44a87
30008f34:	db0c2e0d 	.word	0xdb0c2e0d
30008f38:	47b5481d 	.word	0x47b5481d
30008f3c:	367cd507 	.word	0x367cd507
30008f40:	c1059ed8 	.word	0xc1059ed8
30008f44:	befa4fa4 	.word	0xbefa4fa4
30008f48:	137e2179 	.word	0x137e2179

30008f4c <CPU_ClkGet>:
30008f4c:	b570      	push	{r4, r5, r6, lr}
30008f4e:	4d1e      	ldr	r5, [pc, #120]	; (30008fc8 <CPU_ClkGet+0x7c>)
30008f50:	4b1e      	ldr	r3, [pc, #120]	; (30008fcc <CPU_ClkGet+0x80>)
30008f52:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
30008f56:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30008f5a:	f3c4 1481 	ubfx	r4, r4, #6, #2
30008f5e:	4798      	blx	r3
30008f60:	b92c      	cbnz	r4, 30008f6e <CPU_ClkGet+0x22>
30008f62:	2801      	cmp	r0, #1
30008f64:	d007      	beq.n	30008f76 <CPU_ClkGet+0x2a>
30008f66:	07b2      	lsls	r2, r6, #30
30008f68:	d407      	bmi.n	30008f7a <CPU_ClkGet+0x2e>
30008f6a:	4819      	ldr	r0, [pc, #100]	; (30008fd0 <CPU_ClkGet+0x84>)
30008f6c:	bd70      	pop	{r4, r5, r6, pc}
30008f6e:	2c01      	cmp	r4, #1
30008f70:	d007      	beq.n	30008f82 <CPU_ClkGet+0x36>
30008f72:	2000      	movs	r0, #0
30008f74:	bd70      	pop	{r4, r5, r6, pc}
30008f76:	4817      	ldr	r0, [pc, #92]	; (30008fd4 <CPU_ClkGet+0x88>)
30008f78:	bd70      	pop	{r4, r5, r6, pc}
30008f7a:	4b17      	ldr	r3, [pc, #92]	; (30008fd8 <CPU_ClkGet+0x8c>)
30008f7c:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008f80:	4718      	bx	r3
30008f82:	2801      	cmp	r0, #1
30008f84:	d011      	beq.n	30008faa <CPU_ClkGet+0x5e>
30008f86:	05f3      	lsls	r3, r6, #23
30008f88:	d51c      	bpl.n	30008fc4 <CPU_ClkGet+0x78>
30008f8a:	4914      	ldr	r1, [pc, #80]	; (30008fdc <CPU_ClkGet+0x90>)
30008f8c:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30008f90:	6c48      	ldr	r0, [r1, #68]	; 0x44
30008f92:	4a13      	ldr	r2, [pc, #76]	; (30008fe0 <CPU_ClkGet+0x94>)
30008f94:	f003 0307 	and.w	r3, r3, #7
30008f98:	f3c0 1045 	ubfx	r0, r0, #5, #6
30008f9c:	3301      	adds	r3, #1
30008f9e:	3002      	adds	r0, #2
30008fa0:	fb02 f000 	mul.w	r0, r2, r0
30008fa4:	fbb0 f0f3 	udiv	r0, r0, r3
30008fa8:	bd70      	pop	{r4, r5, r6, pc}
30008faa:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30008fae:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30008fb2:	f3c3 4007 	ubfx	r0, r3, #16, #8
30008fb6:	d003      	beq.n	30008fc0 <CPU_ClkGet+0x74>
30008fb8:	4b0a      	ldr	r3, [pc, #40]	; (30008fe4 <CPU_ClkGet+0x98>)
30008fba:	fb03 f000 	mul.w	r0, r3, r0
30008fbe:	bd70      	pop	{r4, r5, r6, pc}
30008fc0:	4809      	ldr	r0, [pc, #36]	; (30008fe8 <CPU_ClkGet+0x9c>)
30008fc2:	bd70      	pop	{r4, r5, r6, pc}
30008fc4:	4806      	ldr	r0, [pc, #24]	; (30008fe0 <CPU_ClkGet+0x94>)
30008fc6:	bd70      	pop	{r4, r5, r6, pc}
30008fc8:	42008000 	.word	0x42008000
30008fcc:	0000c0f9 	.word	0x0000c0f9
30008fd0:	003d0900 	.word	0x003d0900
30008fd4:	01312d00 	.word	0x01312d00
30008fd8:	000099f5 	.word	0x000099f5
30008fdc:	42008800 	.word	0x42008800
30008fe0:	02625a00 	.word	0x02625a00
30008fe4:	000f4240 	.word	0x000f4240
30008fe8:	02faf080 	.word	0x02faf080

30008fec <RSIP_IV_Set>:
30008fec:	4b05      	ldr	r3, [pc, #20]	; (30009004 <RSIP_IV_Set+0x18>)
30008fee:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30008ff2:	680b      	ldr	r3, [r1, #0]
30008ff4:	ba1b      	rev	r3, r3
30008ff6:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008ffa:	684b      	ldr	r3, [r1, #4]
30008ffc:	ba1b      	rev	r3, r3
30008ffe:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30009002:	4770      	bx	lr
30009004:	42008c00 	.word	0x42008c00

30009008 <RSIP_OTF_Cmd>:
30009008:	4a04      	ldr	r2, [pc, #16]	; (3000901c <RSIP_OTF_Cmd+0x14>)
3000900a:	2801      	cmp	r0, #1
3000900c:	6953      	ldr	r3, [r2, #20]
3000900e:	bf0c      	ite	eq
30009010:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30009014:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30009018:	6153      	str	r3, [r2, #20]
3000901a:	4770      	bx	lr
3000901c:	42008c00 	.word	0x42008c00

30009020 <RSIP_OTF_Enable>:
30009020:	b570      	push	{r4, r5, r6, lr}
30009022:	4604      	mov	r4, r0
30009024:	b1f3      	cbz	r3, 30009064 <RSIP_OTF_Enable+0x44>
30009026:	9b04      	ldr	r3, [sp, #16]
30009028:	460d      	mov	r5, r1
3000902a:	4616      	mov	r6, r2
3000902c:	2b07      	cmp	r3, #7
3000902e:	d827      	bhi.n	30009080 <RSIP_OTF_Enable+0x60>
30009030:	9b05      	ldr	r3, [sp, #20]
30009032:	3b01      	subs	r3, #1
30009034:	2b01      	cmp	r3, #1
30009036:	d81d      	bhi.n	30009074 <RSIP_OTF_Enable+0x54>
30009038:	9a04      	ldr	r2, [sp, #16]
3000903a:	9b05      	ldr	r3, [sp, #20]
3000903c:	f002 0007 	and.w	r0, r2, #7
30009040:	005b      	lsls	r3, r3, #1
30009042:	4305      	orrs	r5, r0
30009044:	4813      	ldr	r0, [pc, #76]	; (30009094 <RSIP_OTF_Enable+0x74>)
30009046:	f003 0306 	and.w	r3, r3, #6
3000904a:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000904e:	ea43 0206 	orr.w	r2, r3, r6
30009052:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009054:	f042 0201 	orr.w	r2, r2, #1
30009058:	f023 0301 	bic.w	r3, r3, #1
3000905c:	6463      	str	r3, [r4, #68]	; 0x44
3000905e:	6425      	str	r5, [r4, #64]	; 0x40
30009060:	6462      	str	r2, [r4, #68]	; 0x44
30009062:	bd70      	pop	{r4, r5, r6, pc}
30009064:	480b      	ldr	r0, [pc, #44]	; (30009094 <RSIP_OTF_Enable+0x74>)
30009066:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000906a:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000906c:	f023 0301 	bic.w	r3, r3, #1
30009070:	6463      	str	r3, [r4, #68]	; 0x44
30009072:	bd70      	pop	{r4, r5, r6, pc}
30009074:	f240 2131 	movw	r1, #561	; 0x231
30009078:	4807      	ldr	r0, [pc, #28]	; (30009098 <RSIP_OTF_Enable+0x78>)
3000907a:	f002 fd79 	bl	3000bb70 <__io_assert_failed_veneer>
3000907e:	e7db      	b.n	30009038 <RSIP_OTF_Enable+0x18>
30009080:	f44f 710c 	mov.w	r1, #560	; 0x230
30009084:	4804      	ldr	r0, [pc, #16]	; (30009098 <RSIP_OTF_Enable+0x78>)
30009086:	f002 fd73 	bl	3000bb70 <__io_assert_failed_veneer>
3000908a:	9b05      	ldr	r3, [sp, #20]
3000908c:	3b01      	subs	r3, #1
3000908e:	2b01      	cmp	r3, #1
30009090:	d9d2      	bls.n	30009038 <RSIP_OTF_Enable+0x18>
30009092:	e7ef      	b.n	30009074 <RSIP_OTF_Enable+0x54>
30009094:	42008c00 	.word	0x42008c00
30009098:	3000d0bc 	.word	0x3000d0bc

3000909c <RSIP_MMU_Config>:
3000909c:	eb00 0040 	add.w	r0, r0, r0, lsl #1
300090a0:	311f      	adds	r1, #31
300090a2:	321f      	adds	r2, #31
300090a4:	331f      	adds	r3, #31
300090a6:	0080      	lsls	r0, r0, #2
300090a8:	0949      	lsrs	r1, r1, #5
300090aa:	0952      	lsrs	r2, r2, #5
300090ac:	095b      	lsrs	r3, r3, #5
300090ae:	0249      	lsls	r1, r1, #9
300090b0:	0252      	lsls	r2, r2, #9
300090b2:	025b      	lsls	r3, r3, #9
300090b4:	b410      	push	{r4}
300090b6:	4c08      	ldr	r4, [pc, #32]	; (300090d8 <RSIP_MMU_Config+0x3c>)
300090b8:	4404      	add	r4, r0
300090ba:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
300090be:	f020 0001 	bic.w	r0, r0, #1
300090c2:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
300090c6:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
300090ca:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
300090ce:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
300090d2:	f85d 4b04 	ldr.w	r4, [sp], #4
300090d6:	4770      	bx	lr
300090d8:	42008c00 	.word	0x42008c00

300090dc <RSIP_MMU_Cmd>:
300090dc:	eb00 0040 	add.w	r0, r0, r0, lsl #1
300090e0:	4b07      	ldr	r3, [pc, #28]	; (30009100 <RSIP_MMU_Cmd+0x24>)
300090e2:	0080      	lsls	r0, r0, #2
300090e4:	4403      	add	r3, r0
300090e6:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
300090ea:	b121      	cbz	r1, 300090f6 <RSIP_MMU_Cmd+0x1a>
300090ec:	f042 0201 	orr.w	r2, r2, #1
300090f0:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300090f4:	4770      	bx	lr
300090f6:	f022 0201 	bic.w	r2, r2, #1
300090fa:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300090fe:	4770      	bx	lr
30009100:	42008c00 	.word	0x42008c00

30009104 <RSIP_MMU_Cache_Clean>:
30009104:	4a02      	ldr	r2, [pc, #8]	; (30009110 <RSIP_MMU_Cache_Clean+0xc>)
30009106:	6993      	ldr	r3, [r2, #24]
30009108:	f043 0304 	orr.w	r3, r3, #4
3000910c:	6193      	str	r3, [r2, #24]
3000910e:	4770      	bx	lr
30009110:	42008c00 	.word	0x42008c00

30009114 <NAND_RxData>:
30009114:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009118:	460c      	mov	r4, r1
3000911a:	4616      	mov	r6, r2
3000911c:	461f      	mov	r7, r3
3000911e:	2800      	cmp	r0, #0
30009120:	f000 808a 	beq.w	30009238 <NAND_RxData+0x124>
30009124:	4605      	mov	r5, r0
30009126:	f8df 911c 	ldr.w	r9, [pc, #284]	; 30009244 <NAND_RxData+0x130>
3000912a:	2000      	movs	r0, #0
3000912c:	f8df 8118 	ldr.w	r8, [pc, #280]	; 30009248 <NAND_RxData+0x134>
30009130:	47c0      	blx	r8
30009132:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30009136:	2000      	movs	r0, #0
30009138:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
3000913c:	6819      	ldr	r1, [r3, #0]
3000913e:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
30009142:	6019      	str	r1, [r3, #0]
30009144:	6819      	ldr	r1, [r3, #0]
30009146:	f441 7140 	orr.w	r1, r1, #768	; 0x300
3000914a:	6019      	str	r1, [r3, #0]
3000914c:	605a      	str	r2, [r3, #4]
3000914e:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
30009152:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
30009156:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
3000915a:	ea4f 420c 	mov.w	r2, ip, lsl #16
3000915e:	f1bc 0f03 	cmp.w	ip, #3
30009162:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30009166:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000916a:	ea42 0201 	orr.w	r2, r2, r1
3000916e:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30009172:	f04f 0201 	mov.w	r2, #1
30009176:	611a      	str	r2, [r3, #16]
30009178:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
3000917c:	bf08      	it	eq
3000917e:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30009182:	f3c4 2307 	ubfx	r3, r4, #8, #8
30009186:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000918a:	b2e4      	uxtb	r4, r4
3000918c:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009190:	2301      	movs	r3, #1
30009192:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30009196:	f017 0403 	ands.w	r4, r7, #3
3000919a:	6083      	str	r3, [r0, #8]
3000919c:	d137      	bne.n	3000920e <NAND_RxData+0xfa>
3000919e:	463b      	mov	r3, r7
300091a0:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
300091a4:	42b4      	cmp	r4, r6
300091a6:	d216      	bcs.n	300091d6 <NAND_RxData+0xc2>
300091a8:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
300091ac:	f012 0f3c 	tst.w	r2, #60	; 0x3c
300091b0:	f3c2 0583 	ubfx	r5, r2, #2, #4
300091b4:	d00c      	beq.n	300091d0 <NAND_RxData+0xbc>
300091b6:	00ad      	lsls	r5, r5, #2
300091b8:	1f19      	subs	r1, r3, #4
300091ba:	f1a5 0c04 	sub.w	ip, r5, #4
300091be:	449c      	add	ip, r3
300091c0:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
300091c4:	f841 0f04 	str.w	r0, [r1, #4]!
300091c8:	458c      	cmp	ip, r1
300091ca:	d1f9      	bne.n	300091c0 <NAND_RxData+0xac>
300091cc:	442b      	add	r3, r5
300091ce:	442c      	add	r4, r5
300091d0:	1b32      	subs	r2, r6, r4
300091d2:	2a03      	cmp	r2, #3
300091d4:	d8e6      	bhi.n	300091a4 <NAND_RxData+0x90>
300091d6:	1e63      	subs	r3, r4, #1
300091d8:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
300091dc:	441f      	add	r7, r3
300091de:	e002      	b.n	300091e6 <NAND_RxData+0xd2>
300091e0:	6a93      	ldr	r3, [r2, #40]	; 0x28
300091e2:	071b      	lsls	r3, r3, #28
300091e4:	d40d      	bmi.n	30009202 <NAND_RxData+0xee>
300091e6:	42b4      	cmp	r4, r6
300091e8:	d3fa      	bcc.n	300091e0 <NAND_RxData+0xcc>
300091ea:	2004      	movs	r0, #4
300091ec:	47c0      	blx	r8
300091ee:	2000      	movs	r0, #0
300091f0:	47c0      	blx	r8
300091f2:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
300091f6:	6813      	ldr	r3, [r2, #0]
300091f8:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300091fc:	6013      	str	r3, [r2, #0]
300091fe:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009202:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
30009206:	3401      	adds	r4, #1
30009208:	f807 3f01 	strb.w	r3, [r7, #1]!
3000920c:	e7eb      	b.n	300091e6 <NAND_RxData+0xd2>
3000920e:	f1c4 0404 	rsb	r4, r4, #4
30009212:	2200      	movs	r2, #0
30009214:	4639      	mov	r1, r7
30009216:	e00a      	b.n	3000922e <NAND_RxData+0x11a>
30009218:	6a85      	ldr	r5, [r0, #40]	; 0x28
3000921a:	072d      	lsls	r5, r5, #28
3000921c:	d505      	bpl.n	3000922a <NAND_RxData+0x116>
3000921e:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
30009222:	3201      	adds	r2, #1
30009224:	700b      	strb	r3, [r1, #0]
30009226:	18b9      	adds	r1, r7, r2
30009228:	460b      	mov	r3, r1
3000922a:	4294      	cmp	r4, r2
3000922c:	d0b8      	beq.n	300091a0 <NAND_RxData+0x8c>
3000922e:	42b2      	cmp	r2, r6
30009230:	460b      	mov	r3, r1
30009232:	d3f1      	bcc.n	30009218 <NAND_RxData+0x104>
30009234:	4614      	mov	r4, r2
30009236:	e7ce      	b.n	300091d6 <NAND_RxData+0xc2>
30009238:	f8df 9008 	ldr.w	r9, [pc, #8]	; 30009244 <NAND_RxData+0x130>
3000923c:	f899 5008 	ldrb.w	r5, [r9, #8]
30009240:	e773      	b.n	3000912a <NAND_RxData+0x16>
30009242:	bf00      	nop
30009244:	2001c01c 	.word	0x2001c01c
30009248:	0000b9c1 	.word	0x0000b9c1

3000924c <NAND_Page_Read>:
3000924c:	b530      	push	{r4, r5, lr}
3000924e:	b085      	sub	sp, #20
30009250:	4c07      	ldr	r4, [pc, #28]	; (30009270 <NAND_Page_Read+0x24>)
30009252:	4d08      	ldr	r5, [pc, #32]	; (30009274 <NAND_Page_Read+0x28>)
30009254:	9301      	str	r3, [sp, #4]
30009256:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000925a:	47a0      	blx	r4
3000925c:	4604      	mov	r4, r0
3000925e:	9903      	ldr	r1, [sp, #12]
30009260:	7a28      	ldrb	r0, [r5, #8]
30009262:	4d05      	ldr	r5, [pc, #20]	; (30009278 <NAND_Page_Read+0x2c>)
30009264:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30009268:	47a8      	blx	r5
3000926a:	4620      	mov	r0, r4
3000926c:	b005      	add	sp, #20
3000926e:	bd30      	pop	{r4, r5, pc}
30009270:	0000bd85 	.word	0x0000bd85
30009274:	2001c01c 	.word	0x2001c01c
30009278:	30009115 	.word	0x30009115

3000927c <SYSCFG_BootFromNor>:
3000927c:	4b0e      	ldr	r3, [pc, #56]	; (300092b8 <SYSCFG_BootFromNor+0x3c>)
3000927e:	b510      	push	{r4, lr}
30009280:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30009284:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009288:	0392      	lsls	r2, r2, #14
3000928a:	d50a      	bpl.n	300092a2 <SYSCFG_BootFromNor+0x26>
3000928c:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009290:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30009294:	d105      	bne.n	300092a2 <SYSCFG_BootFromNor+0x26>
30009296:	4b09      	ldr	r3, [pc, #36]	; (300092bc <SYSCFG_BootFromNor+0x40>)
30009298:	4798      	blx	r3
3000929a:	3800      	subs	r0, #0
3000929c:	bf18      	it	ne
3000929e:	2001      	movne	r0, #1
300092a0:	bd10      	pop	{r4, pc}
300092a2:	4c07      	ldr	r4, [pc, #28]	; (300092c0 <SYSCFG_BootFromNor+0x44>)
300092a4:	47a0      	blx	r4
300092a6:	2802      	cmp	r0, #2
300092a8:	d004      	beq.n	300092b4 <SYSCFG_BootFromNor+0x38>
300092aa:	47a0      	blx	r4
300092ac:	2801      	cmp	r0, #1
300092ae:	d1f2      	bne.n	30009296 <SYSCFG_BootFromNor+0x1a>
300092b0:	2000      	movs	r0, #0
300092b2:	bd10      	pop	{r4, pc}
300092b4:	2001      	movs	r0, #1
300092b6:	bd10      	pop	{r4, pc}
300092b8:	42008000 	.word	0x42008000
300092bc:	0000c0bd 	.word	0x0000c0bd
300092c0:	0000c1a9 	.word	0x0000c1a9

300092c4 <SYSCFG_RLVersion>:
300092c4:	4b07      	ldr	r3, [pc, #28]	; (300092e4 <SYSCFG_RLVersion+0x20>)
300092c6:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
300092ca:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
300092ce:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
300092d2:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
300092d6:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
300092da:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
300092de:	f3c0 4003 	ubfx	r0, r0, #16, #4
300092e2:	4770      	bx	lr
300092e4:	42008000 	.word	0x42008000

300092e8 <SBOOT_GetMdType>:
300092e8:	b470      	push	{r4, r5, r6}
300092ea:	4c0c      	ldr	r4, [pc, #48]	; (3000931c <SBOOT_GetMdType+0x34>)
300092ec:	4e0c      	ldr	r6, [pc, #48]	; (30009320 <SBOOT_GetMdType+0x38>)
300092ee:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
300092f2:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
300092f6:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
300092fa:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
300092fe:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009302:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009306:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000930a:	6033      	str	r3, [r6, #0]
3000930c:	b913      	cbnz	r3, 30009314 <SBOOT_GetMdType+0x2c>
3000930e:	bc70      	pop	{r4, r5, r6}
30009310:	f002 bbd6 	b.w	3000bac0 <__SBOOT_GetMdType_A_veneer>
30009314:	bc70      	pop	{r4, r5, r6}
30009316:	f002 bbe3 	b.w	3000bae0 <__SBOOT_GetMdType_B_veneer>
3000931a:	bf00      	nop
3000931c:	42008000 	.word	0x42008000
30009320:	3000f3c4 	.word	0x3000f3c4

30009324 <CRYPTO_OTPKey_SHA_Init>:
30009324:	4a0d      	ldr	r2, [pc, #52]	; (3000935c <CRYPTO_OTPKey_SHA_Init+0x38>)
30009326:	b410      	push	{r4}
30009328:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
3000932c:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009330:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009334:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009338:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
3000933c:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009340:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009344:	4a06      	ldr	r2, [pc, #24]	; (30009360 <CRYPTO_OTPKey_SHA_Init+0x3c>)
30009346:	6013      	str	r3, [r2, #0]
30009348:	b91b      	cbnz	r3, 30009352 <CRYPTO_OTPKey_SHA_Init+0x2e>
3000934a:	f85d 4b04 	ldr.w	r4, [sp], #4
3000934e:	f002 bc27 	b.w	3000bba0 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30009352:	f85d 4b04 	ldr.w	r4, [sp], #4
30009356:	f002 bc53 	b.w	3000bc00 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
3000935a:	bf00      	nop
3000935c:	42008000 	.word	0x42008000
30009360:	3000f3c4 	.word	0x3000f3c4

30009364 <rtl_crypto_hmac_sha2_update>:
30009364:	b470      	push	{r4, r5, r6}
30009366:	4c0c      	ldr	r4, [pc, #48]	; (30009398 <rtl_crypto_hmac_sha2_update+0x34>)
30009368:	4e0c      	ldr	r6, [pc, #48]	; (3000939c <rtl_crypto_hmac_sha2_update+0x38>)
3000936a:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000936e:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009372:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009376:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000937a:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000937e:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009382:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009386:	6033      	str	r3, [r6, #0]
30009388:	b913      	cbnz	r3, 30009390 <rtl_crypto_hmac_sha2_update+0x2c>
3000938a:	bc70      	pop	{r4, r5, r6}
3000938c:	f002 bbd8 	b.w	3000bb40 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009390:	bc70      	pop	{r4, r5, r6}
30009392:	f002 bbc5 	b.w	3000bb20 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009396:	bf00      	nop
30009398:	42008000 	.word	0x42008000
3000939c:	3000f3c4 	.word	0x3000f3c4

300093a0 <rtl_crypto_hmac_sha2_final>:
300093a0:	4a0d      	ldr	r2, [pc, #52]	; (300093d8 <rtl_crypto_hmac_sha2_final+0x38>)
300093a2:	b410      	push	{r4}
300093a4:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
300093a8:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
300093ac:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
300093b0:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
300093b4:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
300093b8:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
300093bc:	f3c3 4303 	ubfx	r3, r3, #16, #4
300093c0:	4a06      	ldr	r2, [pc, #24]	; (300093dc <rtl_crypto_hmac_sha2_final+0x3c>)
300093c2:	6013      	str	r3, [r2, #0]
300093c4:	b91b      	cbnz	r3, 300093ce <rtl_crypto_hmac_sha2_final+0x2e>
300093c6:	f85d 4b04 	ldr.w	r4, [sp], #4
300093ca:	f002 bb71 	b.w	3000bab0 <__rtl_crypto_hmac_sha2_final_A_veneer>
300093ce:	f85d 4b04 	ldr.w	r4, [sp], #4
300093d2:	f002 bb5d 	b.w	3000ba90 <__rtl_crypto_hmac_sha2_final_B_veneer>
300093d6:	bf00      	nop
300093d8:	42008000 	.word	0x42008000
300093dc:	3000f3c4 	.word	0x3000f3c4

300093e0 <rtl_crypto_sha2_init>:
300093e0:	4a0d      	ldr	r2, [pc, #52]	; (30009418 <rtl_crypto_sha2_init+0x38>)
300093e2:	b410      	push	{r4}
300093e4:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
300093e8:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
300093ec:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
300093f0:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
300093f4:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
300093f8:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
300093fc:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009400:	4a06      	ldr	r2, [pc, #24]	; (3000941c <rtl_crypto_sha2_init+0x3c>)
30009402:	6013      	str	r3, [r2, #0]
30009404:	b91b      	cbnz	r3, 3000940e <rtl_crypto_sha2_init+0x2e>
30009406:	f85d 4b04 	ldr.w	r4, [sp], #4
3000940a:	f002 bb19 	b.w	3000ba40 <__rtl_crypto_sha2_init_A_veneer>
3000940e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009412:	f002 bbed 	b.w	3000bbf0 <__rtl_crypto_sha2_init_B_veneer>
30009416:	bf00      	nop
30009418:	42008000 	.word	0x42008000
3000941c:	3000f3c4 	.word	0x3000f3c4

30009420 <rtl_crypto_sha2_update>:
30009420:	b470      	push	{r4, r5, r6}
30009422:	4c0c      	ldr	r4, [pc, #48]	; (30009454 <rtl_crypto_sha2_update+0x34>)
30009424:	4e0c      	ldr	r6, [pc, #48]	; (30009458 <rtl_crypto_sha2_update+0x38>)
30009426:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000942a:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
3000942e:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009432:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009436:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000943a:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
3000943e:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009442:	6033      	str	r3, [r6, #0]
30009444:	b913      	cbnz	r3, 3000944c <rtl_crypto_sha2_update+0x2c>
30009446:	bc70      	pop	{r4, r5, r6}
30009448:	f002 bb8a 	b.w	3000bb60 <__rtl_crypto_sha2_update_A_veneer>
3000944c:	bc70      	pop	{r4, r5, r6}
3000944e:	f002 bb57 	b.w	3000bb00 <__rtl_crypto_sha2_update_B_veneer>
30009452:	bf00      	nop
30009454:	42008000 	.word	0x42008000
30009458:	3000f3c4 	.word	0x3000f3c4

3000945c <rtl_crypto_sha2_final>:
3000945c:	4a0d      	ldr	r2, [pc, #52]	; (30009494 <rtl_crypto_sha2_final+0x38>)
3000945e:	b410      	push	{r4}
30009460:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009464:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009468:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
3000946c:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009470:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009474:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009478:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000947c:	4a06      	ldr	r2, [pc, #24]	; (30009498 <rtl_crypto_sha2_final+0x3c>)
3000947e:	6013      	str	r3, [r2, #0]
30009480:	b91b      	cbnz	r3, 3000948a <rtl_crypto_sha2_final+0x2e>
30009482:	f85d 4b04 	ldr.w	r4, [sp], #4
30009486:	f002 bbab 	b.w	3000bbe0 <__rtl_crypto_sha2_final_A_veneer>
3000948a:	f85d 4b04 	ldr.w	r4, [sp], #4
3000948e:	f002 bb8f 	b.w	3000bbb0 <__rtl_crypto_sha2_final_B_veneer>
30009492:	bf00      	nop
30009494:	42008000 	.word	0x42008000
30009498:	3000f3c4 	.word	0x3000f3c4

3000949c <SBOOT_Validate_PubKey>:
3000949c:	b470      	push	{r4, r5, r6}
3000949e:	4c0c      	ldr	r4, [pc, #48]	; (300094d0 <SBOOT_Validate_PubKey+0x34>)
300094a0:	4e0c      	ldr	r6, [pc, #48]	; (300094d4 <SBOOT_Validate_PubKey+0x38>)
300094a2:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
300094a6:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
300094aa:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
300094ae:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
300094b2:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
300094b6:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
300094ba:	f3c3 4303 	ubfx	r3, r3, #16, #4
300094be:	6033      	str	r3, [r6, #0]
300094c0:	b913      	cbnz	r3, 300094c8 <SBOOT_Validate_PubKey+0x2c>
300094c2:	bc70      	pop	{r4, r5, r6}
300094c4:	f002 badc 	b.w	3000ba80 <__SBOOT_Validate_PubKey_A_veneer>
300094c8:	bc70      	pop	{r4, r5, r6}
300094ca:	f002 bb61 	b.w	3000bb90 <__SBOOT_Validate_PubKey_B_veneer>
300094ce:	bf00      	nop
300094d0:	42008000 	.word	0x42008000
300094d4:	3000f3c4 	.word	0x3000f3c4

300094d8 <SBOOT_Validate_Signature>:
300094d8:	b5f0      	push	{r4, r5, r6, r7, lr}
300094da:	4d10      	ldr	r5, [pc, #64]	; (3000951c <SBOOT_Validate_Signature+0x44>)
300094dc:	9f05      	ldr	r7, [sp, #20]
300094de:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
300094e2:	f8dd c018 	ldr.w	ip, [sp, #24]
300094e6:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
300094ea:	f8df e034 	ldr.w	lr, [pc, #52]	; 30009520 <SBOOT_Validate_Signature+0x48>
300094ee:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
300094f2:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
300094f6:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
300094fa:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
300094fe:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009502:	e9cd 7c05 	strd	r7, ip, [sp, #20]
30009506:	f8ce 4000 	str.w	r4, [lr]
3000950a:	b91c      	cbnz	r4, 30009514 <SBOOT_Validate_Signature+0x3c>
3000950c:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009510:	f002 ba8e 	b.w	3000ba30 <__SBOOT_Validate_Signature_A_veneer>
30009514:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009518:	f002 bafa 	b.w	3000bb10 <__SBOOT_Validate_Signature_B_veneer>
3000951c:	42008000 	.word	0x42008000
30009520:	3000f3c4 	.word	0x3000f3c4

30009524 <SBOOT_Validate_ImgHash>:
30009524:	b4f0      	push	{r4, r5, r6, r7}
30009526:	4d0c      	ldr	r5, [pc, #48]	; (30009558 <SBOOT_Validate_ImgHash+0x34>)
30009528:	4f0c      	ldr	r7, [pc, #48]	; (3000955c <SBOOT_Validate_ImgHash+0x38>)
3000952a:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000952e:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009532:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009536:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000953a:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000953e:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009542:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009546:	603c      	str	r4, [r7, #0]
30009548:	b914      	cbnz	r4, 30009550 <SBOOT_Validate_ImgHash+0x2c>
3000954a:	bcf0      	pop	{r4, r5, r6, r7}
3000954c:	f002 bb00 	b.w	3000bb50 <__SBOOT_Validate_ImgHash_A_veneer>
30009550:	bcf0      	pop	{r4, r5, r6, r7}
30009552:	f002 baed 	b.w	3000bb30 <__SBOOT_Validate_ImgHash_B_veneer>
30009556:	bf00      	nop
30009558:	42008000 	.word	0x42008000
3000955c:	3000f3c4 	.word	0x3000f3c4

30009560 <CRYPTO_SHA_Init>:
30009560:	4b0b      	ldr	r3, [pc, #44]	; (30009590 <CRYPTO_SHA_Init+0x30>)
30009562:	490c      	ldr	r1, [pc, #48]	; (30009594 <CRYPTO_SHA_Init+0x34>)
30009564:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009568:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000956c:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
30009570:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
30009574:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009578:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000957c:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009580:	6008      	str	r0, [r1, #0]
30009582:	b908      	cbnz	r0, 30009588 <CRYPTO_SHA_Init+0x28>
30009584:	f002 bb24 	b.w	3000bbd0 <__CRYPTO_SHA_Init_A_veneer>
30009588:	2000      	movs	r0, #0
3000958a:	f002 ba71 	b.w	3000ba70 <__CRYPTO_SHA_Init_B_veneer>
3000958e:	bf00      	nop
30009590:	42008000 	.word	0x42008000
30009594:	3000f3c4 	.word	0x3000f3c4

30009598 <SBOOT_Validate_Algorithm>:
30009598:	b4f0      	push	{r4, r5, r6, r7}
3000959a:	4d0c      	ldr	r5, [pc, #48]	; (300095cc <SBOOT_Validate_Algorithm+0x34>)
3000959c:	4f0c      	ldr	r7, [pc, #48]	; (300095d0 <SBOOT_Validate_Algorithm+0x38>)
3000959e:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
300095a2:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
300095a6:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
300095aa:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
300095ae:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
300095b2:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
300095b6:	f3c4 4403 	ubfx	r4, r4, #16, #4
300095ba:	603c      	str	r4, [r7, #0]
300095bc:	b914      	cbnz	r4, 300095c4 <SBOOT_Validate_Algorithm+0x2c>
300095be:	bcf0      	pop	{r4, r5, r6, r7}
300095c0:	f002 ba6e 	b.w	3000baa0 <__SBOOT_Validate_Algorithm_A_veneer>
300095c4:	bcf0      	pop	{r4, r5, r6, r7}
300095c6:	f002 ba4b 	b.w	3000ba60 <__SBOOT_Validate_Algorithm_B_veneer>
300095ca:	bf00      	nop
300095cc:	42008000 	.word	0x42008000
300095d0:	3000f3c4 	.word	0x3000f3c4

300095d4 <ddr_init_index>:
300095d4:	b570      	push	{r4, r5, r6, lr}
300095d6:	4b0f      	ldr	r3, [pc, #60]	; (30009614 <ddr_init_index+0x40>)
300095d8:	4798      	blx	r3
300095da:	4604      	mov	r4, r0
300095dc:	4b0e      	ldr	r3, [pc, #56]	; (30009618 <ddr_init_index+0x44>)
300095de:	4798      	blx	r3
300095e0:	2c03      	cmp	r4, #3
300095e2:	d014      	beq.n	3000960e <ddr_init_index+0x3a>
300095e4:	2c01      	cmp	r4, #1
300095e6:	d00c      	beq.n	30009602 <ddr_init_index+0x2e>
300095e8:	4e0c      	ldr	r6, [pc, #48]	; (3000961c <ddr_init_index+0x48>)
300095ea:	4d0d      	ldr	r5, [pc, #52]	; (30009620 <ddr_init_index+0x4c>)
300095ec:	4c0d      	ldr	r4, [pc, #52]	; (30009624 <ddr_init_index+0x50>)
300095ee:	462b      	mov	r3, r5
300095f0:	4622      	mov	r2, r4
300095f2:	490b      	ldr	r1, [pc, #44]	; (30009620 <ddr_init_index+0x4c>)
300095f4:	2002      	movs	r0, #2
300095f6:	f001 ffdd 	bl	3000b5b4 <rtk_log_write>
300095fa:	f242 7010 	movw	r0, #10000	; 0x2710
300095fe:	47b0      	blx	r6
30009600:	e7f5      	b.n	300095ee <ddr_init_index+0x1a>
30009602:	2802      	cmp	r0, #2
30009604:	d003      	beq.n	3000960e <ddr_init_index+0x3a>
30009606:	2803      	cmp	r0, #3
30009608:	d1ee      	bne.n	300095e8 <ddr_init_index+0x14>
3000960a:	2000      	movs	r0, #0
3000960c:	bd70      	pop	{r4, r5, r6, pc}
3000960e:	4620      	mov	r0, r4
30009610:	bd70      	pop	{r4, r5, r6, pc}
30009612:	bf00      	nop
30009614:	30007691 	.word	0x30007691
30009618:	3000750d 	.word	0x3000750d
3000961c:	00009be5 	.word	0x00009be5
30009620:	3000d104 	.word	0x3000d104
30009624:	3000d108 	.word	0x3000d108

30009628 <rxi316_perf_tune>:
30009628:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000962c:	4606      	mov	r6, r0
3000962e:	6800      	ldr	r0, [r0, #0]
30009630:	b0c3      	sub	sp, #268	; 0x10c
30009632:	7985      	ldrb	r5, [r0, #6]
30009634:	2d00      	cmp	r5, #0
30009636:	f000 8334 	beq.w	30009ca2 <rxi316_perf_tune+0x67a>
3000963a:	2d01      	cmp	r5, #1
3000963c:	d003      	beq.n	30009646 <rxi316_perf_tune+0x1e>
3000963e:	2d03      	cmp	r5, #3
30009640:	bf14      	ite	ne
30009642:	2502      	movne	r5, #2
30009644:	2500      	moveq	r5, #0
30009646:	68f3      	ldr	r3, [r6, #12]
30009648:	2420      	movs	r4, #32
3000964a:	4684      	mov	ip, r0
3000964c:	6859      	ldr	r1, [r3, #4]
3000964e:	46a3      	mov	fp, r4
30009650:	46a2      	mov	sl, r4
30009652:	46a0      	mov	r8, r4
30009654:	784b      	ldrb	r3, [r1, #1]
30009656:	46a1      	mov	r9, r4
30009658:	7bcf      	ldrb	r7, [r1, #15]
3000965a:	9323      	str	r3, [sp, #140]	; 0x8c
3000965c:	788b      	ldrb	r3, [r1, #2]
3000965e:	780a      	ldrb	r2, [r1, #0]
30009660:	9324      	str	r3, [sp, #144]	; 0x90
30009662:	78cb      	ldrb	r3, [r1, #3]
30009664:	9325      	str	r3, [sp, #148]	; 0x94
30009666:	790b      	ldrb	r3, [r1, #4]
30009668:	9326      	str	r3, [sp, #152]	; 0x98
3000966a:	794b      	ldrb	r3, [r1, #5]
3000966c:	9327      	str	r3, [sp, #156]	; 0x9c
3000966e:	798b      	ldrb	r3, [r1, #6]
30009670:	9328      	str	r3, [sp, #160]	; 0xa0
30009672:	79cb      	ldrb	r3, [r1, #7]
30009674:	9329      	str	r3, [sp, #164]	; 0xa4
30009676:	7a0b      	ldrb	r3, [r1, #8]
30009678:	932a      	str	r3, [sp, #168]	; 0xa8
3000967a:	7a4b      	ldrb	r3, [r1, #9]
3000967c:	932b      	str	r3, [sp, #172]	; 0xac
3000967e:	7a8b      	ldrb	r3, [r1, #10]
30009680:	932c      	str	r3, [sp, #176]	; 0xb0
30009682:	7acb      	ldrb	r3, [r1, #11]
30009684:	932d      	str	r3, [sp, #180]	; 0xb4
30009686:	7b0b      	ldrb	r3, [r1, #12]
30009688:	932e      	str	r3, [sp, #184]	; 0xb8
3000968a:	7b4b      	ldrb	r3, [r1, #13]
3000968c:	932f      	str	r3, [sp, #188]	; 0xbc
3000968e:	7b8b      	ldrb	r3, [r1, #14]
30009690:	9330      	str	r3, [sp, #192]	; 0xc0
30009692:	2300      	movs	r3, #0
30009694:	9731      	str	r7, [sp, #196]	; 0xc4
30009696:	7c0f      	ldrb	r7, [r1, #16]
30009698:	941d      	str	r4, [sp, #116]	; 0x74
3000969a:	9732      	str	r7, [sp, #200]	; 0xc8
3000969c:	7c4f      	ldrb	r7, [r1, #17]
3000969e:	9733      	str	r7, [sp, #204]	; 0xcc
300096a0:	7c8f      	ldrb	r7, [r1, #18]
300096a2:	9734      	str	r7, [sp, #208]	; 0xd0
300096a4:	7ccf      	ldrb	r7, [r1, #19]
300096a6:	9735      	str	r7, [sp, #212]	; 0xd4
300096a8:	7d0f      	ldrb	r7, [r1, #20]
300096aa:	9736      	str	r7, [sp, #216]	; 0xd8
300096ac:	7d4f      	ldrb	r7, [r1, #21]
300096ae:	9737      	str	r7, [sp, #220]	; 0xdc
300096b0:	7d8f      	ldrb	r7, [r1, #22]
300096b2:	9738      	str	r7, [sp, #224]	; 0xe0
300096b4:	7dcf      	ldrb	r7, [r1, #23]
300096b6:	9739      	str	r7, [sp, #228]	; 0xe4
300096b8:	7e0f      	ldrb	r7, [r1, #24]
300096ba:	973a      	str	r7, [sp, #232]	; 0xe8
300096bc:	7e4f      	ldrb	r7, [r1, #25]
300096be:	973b      	str	r7, [sp, #236]	; 0xec
300096c0:	7e8f      	ldrb	r7, [r1, #26]
300096c2:	973c      	str	r7, [sp, #240]	; 0xf0
300096c4:	7ecf      	ldrb	r7, [r1, #27]
300096c6:	973d      	str	r7, [sp, #244]	; 0xf4
300096c8:	7f0f      	ldrb	r7, [r1, #28]
300096ca:	973e      	str	r7, [sp, #248]	; 0xf8
300096cc:	7f4f      	ldrb	r7, [r1, #29]
300096ce:	973f      	str	r7, [sp, #252]	; 0xfc
300096d0:	7f8f      	ldrb	r7, [r1, #30]
300096d2:	7fc9      	ldrb	r1, [r1, #31]
300096d4:	9740      	str	r7, [sp, #256]	; 0x100
300096d6:	4627      	mov	r7, r4
300096d8:	9141      	str	r1, [sp, #260]	; 0x104
300096da:	4619      	mov	r1, r3
300096dc:	9409      	str	r4, [sp, #36]	; 0x24
300096de:	941c      	str	r4, [sp, #112]	; 0x70
300096e0:	9404      	str	r4, [sp, #16]
300096e2:	941b      	str	r4, [sp, #108]	; 0x6c
300096e4:	940a      	str	r4, [sp, #40]	; 0x28
300096e6:	9303      	str	r3, [sp, #12]
300096e8:	e9cd 4405 	strd	r4, r4, [sp, #20]
300096ec:	e9cd 4401 	strd	r4, r4, [sp, #4]
300096f0:	e9cd 4407 	strd	r4, r4, [sp, #28]
300096f4:	e9cd 4419 	strd	r4, r4, [sp, #100]	; 0x64
300096f8:	e9cd 4417 	strd	r4, r4, [sp, #92]	; 0x5c
300096fc:	e9cd 4415 	strd	r4, r4, [sp, #84]	; 0x54
30009700:	e9cd 4413 	strd	r4, r4, [sp, #76]	; 0x4c
30009704:	e9cd 4411 	strd	r4, r4, [sp, #68]	; 0x44
30009708:	e9cd 440f 	strd	r4, r4, [sp, #60]	; 0x3c
3000970c:	e9cd 440d 	strd	r4, r4, [sp, #52]	; 0x34
30009710:	e9cd 440b 	strd	r4, r4, [sp, #44]	; 0x2c
30009714:	b94a      	cbnz	r2, 3000972a <rxi316_perf_tune+0x102>
30009716:	3101      	adds	r1, #1
30009718:	4699      	mov	r9, r3
3000971a:	3301      	adds	r3, #1
3000971c:	2b20      	cmp	r3, #32
3000971e:	d069      	beq.n	300097f4 <rxi316_perf_tune+0x1cc>
30009720:	aa22      	add	r2, sp, #136	; 0x88
30009722:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
30009726:	2a00      	cmp	r2, #0
30009728:	d0f5      	beq.n	30009716 <rxi316_perf_tune+0xee>
3000972a:	2a01      	cmp	r2, #1
3000972c:	d05d      	beq.n	300097ea <rxi316_perf_tune+0x1c2>
3000972e:	2a02      	cmp	r2, #2
30009730:	d06e      	beq.n	30009810 <rxi316_perf_tune+0x1e8>
30009732:	2a03      	cmp	r2, #3
30009734:	d06f      	beq.n	30009816 <rxi316_perf_tune+0x1ee>
30009736:	2a04      	cmp	r2, #4
30009738:	d070      	beq.n	3000981c <rxi316_perf_tune+0x1f4>
3000973a:	2a05      	cmp	r2, #5
3000973c:	d071      	beq.n	30009822 <rxi316_perf_tune+0x1fa>
3000973e:	2a06      	cmp	r2, #6
30009740:	d072      	beq.n	30009828 <rxi316_perf_tune+0x200>
30009742:	2a07      	cmp	r2, #7
30009744:	f000 82af 	beq.w	30009ca6 <rxi316_perf_tune+0x67e>
30009748:	2a08      	cmp	r2, #8
3000974a:	f000 8381 	beq.w	30009e50 <rxi316_perf_tune+0x828>
3000974e:	2a09      	cmp	r2, #9
30009750:	f000 8386 	beq.w	30009e60 <rxi316_perf_tune+0x838>
30009754:	2a0a      	cmp	r2, #10
30009756:	f000 838e 	beq.w	30009e76 <rxi316_perf_tune+0x84e>
3000975a:	2a0b      	cmp	r2, #11
3000975c:	f000 8386 	beq.w	30009e6c <rxi316_perf_tune+0x844>
30009760:	2a0c      	cmp	r2, #12
30009762:	f000 838d 	beq.w	30009e80 <rxi316_perf_tune+0x858>
30009766:	2a0d      	cmp	r2, #13
30009768:	f000 8394 	beq.w	30009e94 <rxi316_perf_tune+0x86c>
3000976c:	2a0e      	cmp	r2, #14
3000976e:	f000 838c 	beq.w	30009e8a <rxi316_perf_tune+0x862>
30009772:	2a0f      	cmp	r2, #15
30009774:	f000 8393 	beq.w	30009e9e <rxi316_perf_tune+0x876>
30009778:	2a10      	cmp	r2, #16
3000977a:	f000 8395 	beq.w	30009ea8 <rxi316_perf_tune+0x880>
3000977e:	2a11      	cmp	r2, #17
30009780:	f000 8394 	beq.w	30009eac <rxi316_perf_tune+0x884>
30009784:	2a12      	cmp	r2, #18
30009786:	f000 8393 	beq.w	30009eb0 <rxi316_perf_tune+0x888>
3000978a:	2a13      	cmp	r2, #19
3000978c:	f000 8394 	beq.w	30009eb8 <rxi316_perf_tune+0x890>
30009790:	2a14      	cmp	r2, #20
30009792:	f000 838f 	beq.w	30009eb4 <rxi316_perf_tune+0x88c>
30009796:	2a15      	cmp	r2, #21
30009798:	f000 8396 	beq.w	30009ec8 <rxi316_perf_tune+0x8a0>
3000979c:	2a16      	cmp	r2, #22
3000979e:	f000 8391 	beq.w	30009ec4 <rxi316_perf_tune+0x89c>
300097a2:	2a17      	cmp	r2, #23
300097a4:	f000 83a0 	beq.w	30009ee8 <rxi316_perf_tune+0x8c0>
300097a8:	2a18      	cmp	r2, #24
300097aa:	f000 839b 	beq.w	30009ee4 <rxi316_perf_tune+0x8bc>
300097ae:	2a19      	cmp	r2, #25
300097b0:	f000 8396 	beq.w	30009ee0 <rxi316_perf_tune+0x8b8>
300097b4:	2a1a      	cmp	r2, #26
300097b6:	f000 8391 	beq.w	30009edc <rxi316_perf_tune+0x8b4>
300097ba:	2a1b      	cmp	r2, #27
300097bc:	f000 838c 	beq.w	30009ed8 <rxi316_perf_tune+0x8b0>
300097c0:	2a1c      	cmp	r2, #28
300097c2:	f000 8387 	beq.w	30009ed4 <rxi316_perf_tune+0x8ac>
300097c6:	2a1d      	cmp	r2, #29
300097c8:	f000 8382 	beq.w	30009ed0 <rxi316_perf_tune+0x8a8>
300097cc:	2a1e      	cmp	r2, #30
300097ce:	f000 837d 	beq.w	30009ecc <rxi316_perf_tune+0x8a4>
300097d2:	2a1f      	cmp	r2, #31
300097d4:	f000 8374 	beq.w	30009ec0 <rxi316_perf_tune+0x898>
300097d8:	2a20      	cmp	r2, #32
300097da:	f000 836f 	beq.w	30009ebc <rxi316_perf_tune+0x894>
300097de:	9809      	ldr	r0, [sp, #36]	; 0x24
300097e0:	2a21      	cmp	r2, #33	; 0x21
300097e2:	bf08      	it	eq
300097e4:	4618      	moveq	r0, r3
300097e6:	9009      	str	r0, [sp, #36]	; 0x24
300097e8:	e797      	b.n	3000971a <rxi316_perf_tune+0xf2>
300097ea:	4698      	mov	r8, r3
300097ec:	3301      	adds	r3, #1
300097ee:	3101      	adds	r1, #1
300097f0:	2b20      	cmp	r3, #32
300097f2:	d195      	bne.n	30009720 <rxi316_perf_tune+0xf8>
300097f4:	f89c 2002 	ldrb.w	r2, [ip, #2]
300097f8:	f8dc 3008 	ldr.w	r3, [ip, #8]
300097fc:	b1ba      	cbz	r2, 3000982e <rxi316_perf_tune+0x206>
300097fe:	2a01      	cmp	r2, #1
30009800:	f000 8331 	beq.w	30009e66 <rxi316_perf_tune+0x83e>
30009804:	2a03      	cmp	r2, #3
30009806:	bf0c      	ite	eq
30009808:	2204      	moveq	r2, #4
3000980a:	2203      	movne	r2, #3
3000980c:	921e      	str	r2, [sp, #120]	; 0x78
3000980e:	e010      	b.n	30009832 <rxi316_perf_tune+0x20a>
30009810:	3101      	adds	r1, #1
30009812:	461f      	mov	r7, r3
30009814:	e781      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009816:	3101      	adds	r1, #1
30009818:	469a      	mov	sl, r3
3000981a:	e77e      	b.n	3000971a <rxi316_perf_tune+0xf2>
3000981c:	3101      	adds	r1, #1
3000981e:	469b      	mov	fp, r3
30009820:	e77b      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009822:	3101      	adds	r1, #1
30009824:	9301      	str	r3, [sp, #4]
30009826:	e778      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009828:	3101      	adds	r1, #1
3000982a:	9302      	str	r3, [sp, #8]
3000982c:	e775      	b.n	3000971a <rxi316_perf_tune+0xf2>
3000982e:	2201      	movs	r2, #1
30009830:	921e      	str	r2, [sp, #120]	; 0x78
30009832:	3b02      	subs	r3, #2
30009834:	428b      	cmp	r3, r1
30009836:	d005      	beq.n	30009844 <rxi316_perf_tune+0x21c>
30009838:	4bac      	ldr	r3, [pc, #688]	; (30009aec <rxi316_perf_tune+0x4c4>)
3000983a:	2002      	movs	r0, #2
3000983c:	4aac      	ldr	r2, [pc, #688]	; (30009af0 <rxi316_perf_tune+0x4c8>)
3000983e:	4619      	mov	r1, r3
30009840:	f001 feb8 	bl	3000b5b4 <rtk_log_write>
30009844:	9b1e      	ldr	r3, [sp, #120]	; 0x78
30009846:	9a03      	ldr	r2, [sp, #12]
30009848:	4293      	cmp	r3, r2
3000984a:	d005      	beq.n	30009858 <rxi316_perf_tune+0x230>
3000984c:	4ba7      	ldr	r3, [pc, #668]	; (30009aec <rxi316_perf_tune+0x4c4>)
3000984e:	2002      	movs	r0, #2
30009850:	4aa8      	ldr	r2, [pc, #672]	; (30009af4 <rxi316_perf_tune+0x4cc>)
30009852:	4619      	mov	r1, r3
30009854:	f001 feae 	bl	3000b5b4 <rtk_log_write>
30009858:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000985a:	2b20      	cmp	r3, #32
3000985c:	f000 82f1 	beq.w	30009e42 <rxi316_perf_tune+0x81a>
30009860:	1b5a      	subs	r2, r3, r5
30009862:	3a05      	subs	r2, #5
30009864:	f002 030f 	and.w	r3, r2, #15
30009868:	9303      	str	r3, [sp, #12]
3000986a:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000986c:	2b20      	cmp	r3, #32
3000986e:	f000 82e6 	beq.w	30009e3e <rxi316_perf_tune+0x816>
30009872:	1b5a      	subs	r2, r3, r5
30009874:	3a06      	subs	r2, #6
30009876:	0112      	lsls	r2, r2, #4
30009878:	b2d2      	uxtb	r2, r2
3000987a:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000987c:	2b20      	cmp	r3, #32
3000987e:	f000 82da 	beq.w	30009e36 <rxi316_perf_tune+0x80e>
30009882:	1b5b      	subs	r3, r3, r5
30009884:	3b07      	subs	r3, #7
30009886:	021b      	lsls	r3, r3, #8
30009888:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000988c:	930c      	str	r3, [sp, #48]	; 0x30
3000988e:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009890:	2b20      	cmp	r3, #32
30009892:	f000 820b 	beq.w	30009cac <rxi316_perf_tune+0x684>
30009896:	1b5b      	subs	r3, r3, r5
30009898:	3b08      	subs	r3, #8
3000989a:	031b      	lsls	r3, r3, #12
3000989c:	b29b      	uxth	r3, r3
3000989e:	930b      	str	r3, [sp, #44]	; 0x2c
300098a0:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300098a2:	2b20      	cmp	r3, #32
300098a4:	f000 8209 	beq.w	30009cba <rxi316_perf_tune+0x692>
300098a8:	1b5b      	subs	r3, r3, r5
300098aa:	3b09      	subs	r3, #9
300098ac:	041b      	lsls	r3, r3, #16
300098ae:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
300098b2:	930a      	str	r3, [sp, #40]	; 0x28
300098b4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300098b6:	2b20      	cmp	r3, #32
300098b8:	f000 8206 	beq.w	30009cc8 <rxi316_perf_tune+0x6a0>
300098bc:	1b5b      	subs	r3, r3, r5
300098be:	3b0a      	subs	r3, #10
300098c0:	051b      	lsls	r3, r3, #20
300098c2:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
300098c6:	930d      	str	r3, [sp, #52]	; 0x34
300098c8:	9b10      	ldr	r3, [sp, #64]	; 0x40
300098ca:	2b20      	cmp	r3, #32
300098cc:	f000 8203 	beq.w	30009cd6 <rxi316_perf_tune+0x6ae>
300098d0:	1b5b      	subs	r3, r3, r5
300098d2:	3b0b      	subs	r3, #11
300098d4:	061b      	lsls	r3, r3, #24
300098d6:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
300098da:	930e      	str	r3, [sp, #56]	; 0x38
300098dc:	9b11      	ldr	r3, [sp, #68]	; 0x44
300098de:	2b20      	cmp	r3, #32
300098e0:	f000 8200 	beq.w	30009ce4 <rxi316_perf_tune+0x6bc>
300098e4:	1b5b      	subs	r3, r3, r5
300098e6:	3b0c      	subs	r3, #12
300098e8:	071b      	lsls	r3, r3, #28
300098ea:	930f      	str	r3, [sp, #60]	; 0x3c
300098ec:	9b12      	ldr	r3, [sp, #72]	; 0x48
300098ee:	2b20      	cmp	r3, #32
300098f0:	f000 81ff 	beq.w	30009cf2 <rxi316_perf_tune+0x6ca>
300098f4:	1b5b      	subs	r3, r3, r5
300098f6:	3b0d      	subs	r3, #13
300098f8:	f003 030f 	and.w	r3, r3, #15
300098fc:	9310      	str	r3, [sp, #64]	; 0x40
300098fe:	9b13      	ldr	r3, [sp, #76]	; 0x4c
30009900:	2b20      	cmp	r3, #32
30009902:	f000 81fc 	beq.w	30009cfe <rxi316_perf_tune+0x6d6>
30009906:	1b5b      	subs	r3, r3, r5
30009908:	3b0e      	subs	r3, #14
3000990a:	011b      	lsls	r3, r3, #4
3000990c:	b2db      	uxtb	r3, r3
3000990e:	9311      	str	r3, [sp, #68]	; 0x44
30009910:	9b14      	ldr	r3, [sp, #80]	; 0x50
30009912:	2b20      	cmp	r3, #32
30009914:	f000 81f9 	beq.w	30009d0a <rxi316_perf_tune+0x6e2>
30009918:	1b5b      	subs	r3, r3, r5
3000991a:	3b0f      	subs	r3, #15
3000991c:	021b      	lsls	r3, r3, #8
3000991e:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
30009922:	9312      	str	r3, [sp, #72]	; 0x48
30009924:	9b15      	ldr	r3, [sp, #84]	; 0x54
30009926:	2b20      	cmp	r3, #32
30009928:	f000 81f6 	beq.w	30009d18 <rxi316_perf_tune+0x6f0>
3000992c:	1b5b      	subs	r3, r3, r5
3000992e:	031b      	lsls	r3, r3, #12
30009930:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
30009934:	9321      	str	r3, [sp, #132]	; 0x84
30009936:	9b16      	ldr	r3, [sp, #88]	; 0x58
30009938:	2b20      	cmp	r3, #32
3000993a:	f000 81f4 	beq.w	30009d26 <rxi316_perf_tune+0x6fe>
3000993e:	1b5b      	subs	r3, r3, r5
30009940:	3b11      	subs	r3, #17
30009942:	041b      	lsls	r3, r3, #16
30009944:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
30009948:	9320      	str	r3, [sp, #128]	; 0x80
3000994a:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000994c:	2b20      	cmp	r3, #32
3000994e:	f000 81f1 	beq.w	30009d34 <rxi316_perf_tune+0x70c>
30009952:	1b5b      	subs	r3, r3, r5
30009954:	3b12      	subs	r3, #18
30009956:	051b      	lsls	r3, r3, #20
30009958:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000995c:	931f      	str	r3, [sp, #124]	; 0x7c
3000995e:	9b18      	ldr	r3, [sp, #96]	; 0x60
30009960:	2b20      	cmp	r3, #32
30009962:	f000 81ee 	beq.w	30009d42 <rxi316_perf_tune+0x71a>
30009966:	1b5b      	subs	r3, r3, r5
30009968:	3b13      	subs	r3, #19
3000996a:	061b      	lsls	r3, r3, #24
3000996c:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
30009970:	931e      	str	r3, [sp, #120]	; 0x78
30009972:	9b19      	ldr	r3, [sp, #100]	; 0x64
30009974:	2b20      	cmp	r3, #32
30009976:	f000 81eb 	beq.w	30009d50 <rxi316_perf_tune+0x728>
3000997a:	1b5b      	subs	r3, r3, r5
3000997c:	3b14      	subs	r3, #20
3000997e:	071b      	lsls	r3, r3, #28
30009980:	9318      	str	r3, [sp, #96]	; 0x60
30009982:	9b1a      	ldr	r3, [sp, #104]	; 0x68
30009984:	2b20      	cmp	r3, #32
30009986:	f000 81ea 	beq.w	30009d5e <rxi316_perf_tune+0x736>
3000998a:	1b5b      	subs	r3, r3, r5
3000998c:	3b15      	subs	r3, #21
3000998e:	f003 030f 	and.w	r3, r3, #15
30009992:	9317      	str	r3, [sp, #92]	; 0x5c
30009994:	9b07      	ldr	r3, [sp, #28]
30009996:	2b20      	cmp	r3, #32
30009998:	f000 81e7 	beq.w	30009d6a <rxi316_perf_tune+0x742>
3000999c:	1b5b      	subs	r3, r3, r5
3000999e:	3b02      	subs	r3, #2
300099a0:	f003 031f 	and.w	r3, r3, #31
300099a4:	9316      	str	r3, [sp, #88]	; 0x58
300099a6:	9b08      	ldr	r3, [sp, #32]
300099a8:	2b20      	cmp	r3, #32
300099aa:	f000 81e4 	beq.w	30009d76 <rxi316_perf_tune+0x74e>
300099ae:	1b5b      	subs	r3, r3, r5
300099b0:	3b02      	subs	r3, #2
300099b2:	015b      	lsls	r3, r3, #5
300099b4:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
300099b8:	9315      	str	r3, [sp, #84]	; 0x54
300099ba:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
300099bc:	2b20      	cmp	r3, #32
300099be:	f000 81e1 	beq.w	30009d84 <rxi316_perf_tune+0x75c>
300099c2:	1b5b      	subs	r3, r3, r5
300099c4:	f1b9 0f20 	cmp.w	r9, #32
300099c8:	f1a3 0302 	sub.w	r3, r3, #2
300099cc:	ea4f 2383 	mov.w	r3, r3, lsl #10
300099d0:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
300099d4:	9314      	str	r3, [sp, #80]	; 0x50
300099d6:	f000 81dc 	beq.w	30009d92 <rxi316_perf_tune+0x76a>
300099da:	eba9 0105 	sub.w	r1, r9, r5
300099de:	f1b8 0f20 	cmp.w	r8, #32
300099e2:	f1a1 0102 	sub.w	r1, r1, #2
300099e6:	f001 030f 	and.w	r3, r1, #15
300099ea:	9313      	str	r3, [sp, #76]	; 0x4c
300099ec:	f000 81d7 	beq.w	30009d9e <rxi316_perf_tune+0x776>
300099f0:	eba8 0105 	sub.w	r1, r8, r5
300099f4:	2f20      	cmp	r7, #32
300099f6:	f1a1 0103 	sub.w	r1, r1, #3
300099fa:	ea4f 1101 	mov.w	r1, r1, lsl #4
300099fe:	b2c9      	uxtb	r1, r1
30009a00:	f000 81d2 	beq.w	30009da8 <rxi316_perf_tune+0x780>
30009a04:	1b7b      	subs	r3, r7, r5
30009a06:	f1ba 0f20 	cmp.w	sl, #32
30009a0a:	f1a3 0304 	sub.w	r3, r3, #4
30009a0e:	ea4f 2303 	mov.w	r3, r3, lsl #8
30009a12:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
30009a16:	9308      	str	r3, [sp, #32]
30009a18:	f000 81cd 	beq.w	30009db6 <rxi316_perf_tune+0x78e>
30009a1c:	ebaa 0305 	sub.w	r3, sl, r5
30009a20:	f1bb 0f20 	cmp.w	fp, #32
30009a24:	f1a3 0305 	sub.w	r3, r3, #5
30009a28:	ea4f 3303 	mov.w	r3, r3, lsl #12
30009a2c:	b29b      	uxth	r3, r3
30009a2e:	9307      	str	r3, [sp, #28]
30009a30:	f000 81c8 	beq.w	30009dc4 <rxi316_perf_tune+0x79c>
30009a34:	ebab 0c05 	sub.w	ip, fp, r5
30009a38:	9b01      	ldr	r3, [sp, #4]
30009a3a:	f1ac 0c06 	sub.w	ip, ip, #6
30009a3e:	2b20      	cmp	r3, #32
30009a40:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
30009a44:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
30009a48:	f000 81c2 	beq.w	30009dd0 <rxi316_perf_tune+0x7a8>
30009a4c:	eba3 0e05 	sub.w	lr, r3, r5
30009a50:	9b02      	ldr	r3, [sp, #8]
30009a52:	f1ae 0e07 	sub.w	lr, lr, #7
30009a56:	2b20      	cmp	r3, #32
30009a58:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
30009a5c:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
30009a60:	f000 81bc 	beq.w	30009ddc <rxi316_perf_tune+0x7b4>
30009a64:	eba3 0805 	sub.w	r8, r3, r5
30009a68:	9b04      	ldr	r3, [sp, #16]
30009a6a:	f1a8 0808 	sub.w	r8, r8, #8
30009a6e:	2b20      	cmp	r3, #32
30009a70:	ea4f 6808 	mov.w	r8, r8, lsl #24
30009a74:	f008 6870 	and.w	r8, r8, #251658240	; 0xf000000
30009a78:	f000 81b6 	beq.w	30009de8 <rxi316_perf_tune+0x7c0>
30009a7c:	1b5b      	subs	r3, r3, r5
30009a7e:	3b09      	subs	r3, #9
30009a80:	071b      	lsls	r3, r3, #28
30009a82:	9302      	str	r3, [sp, #8]
30009a84:	9b05      	ldr	r3, [sp, #20]
30009a86:	2b20      	cmp	r3, #32
30009a88:	f000 81b5 	beq.w	30009df6 <rxi316_perf_tune+0x7ce>
30009a8c:	eba3 0905 	sub.w	r9, r3, r5
30009a90:	9b06      	ldr	r3, [sp, #24]
30009a92:	f1a9 090a 	sub.w	r9, r9, #10
30009a96:	2b20      	cmp	r3, #32
30009a98:	f009 090f 	and.w	r9, r9, #15
30009a9c:	f000 81b1 	beq.w	30009e02 <rxi316_perf_tune+0x7da>
30009aa0:	1b58      	subs	r0, r3, r5
30009aa2:	9b1c      	ldr	r3, [sp, #112]	; 0x70
30009aa4:	380b      	subs	r0, #11
30009aa6:	2b20      	cmp	r3, #32
30009aa8:	ea4f 1000 	mov.w	r0, r0, lsl #4
30009aac:	b2c0      	uxtb	r0, r0
30009aae:	f000 81ad 	beq.w	30009e0c <rxi316_perf_tune+0x7e4>
30009ab2:	eba3 0b05 	sub.w	fp, r3, r5
30009ab6:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009ab8:	f1ab 0b02 	sub.w	fp, fp, #2
30009abc:	2b20      	cmp	r3, #32
30009abe:	f00b 0b1f 	and.w	fp, fp, #31
30009ac2:	f000 81a9 	beq.w	30009e18 <rxi316_perf_tune+0x7f0>
30009ac6:	eba3 0a05 	sub.w	sl, r3, r5
30009aca:	9b1d      	ldr	r3, [sp, #116]	; 0x74
30009acc:	f1aa 0a02 	sub.w	sl, sl, #2
30009ad0:	2b20      	cmp	r3, #32
30009ad2:	f00a 0a1f 	and.w	sl, sl, #31
30009ad6:	f000 81a5 	beq.w	30009e24 <rxi316_perf_tune+0x7fc>
30009ada:	1b5f      	subs	r7, r3, r5
30009adc:	2c20      	cmp	r4, #32
30009ade:	f1a7 0702 	sub.w	r7, r7, #2
30009ae2:	f007 071f 	and.w	r7, r7, #31
30009ae6:	f000 81a2 	beq.w	30009e2e <rxi316_perf_tune+0x806>
30009aea:	e005      	b.n	30009af8 <rxi316_perf_tune+0x4d0>
30009aec:	3000d104 	.word	0x3000d104
30009af0:	3000d12c 	.word	0x3000d12c
30009af4:	3000d170 	.word	0x3000d170
30009af8:	1b63      	subs	r3, r4, r5
30009afa:	3b02      	subs	r3, #2
30009afc:	015b      	lsls	r3, r3, #5
30009afe:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
30009b02:	9301      	str	r3, [sp, #4]
30009b04:	9c03      	ldr	r4, [sp, #12]
30009b06:	ea40 0009 	orr.w	r0, r0, r9
30009b0a:	4314      	orrs	r4, r2
30009b0c:	4622      	mov	r2, r4
30009b0e:	e9dd 4510 	ldrd	r4, r5, [sp, #64]	; 0x40
30009b12:	432c      	orrs	r4, r5
30009b14:	9d13      	ldr	r5, [sp, #76]	; 0x4c
30009b16:	4329      	orrs	r1, r5
30009b18:	e9dd 5315 	ldrd	r5, r3, [sp, #84]	; 0x54
30009b1c:	431d      	orrs	r5, r3
30009b1e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30009b20:	431a      	orrs	r2, r3
30009b22:	9b12      	ldr	r3, [sp, #72]	; 0x48
30009b24:	431c      	orrs	r4, r3
30009b26:	9b08      	ldr	r3, [sp, #32]
30009b28:	4319      	orrs	r1, r3
30009b2a:	9b14      	ldr	r3, [sp, #80]	; 0x50
30009b2c:	431d      	orrs	r5, r3
30009b2e:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009b30:	431a      	orrs	r2, r3
30009b32:	9b21      	ldr	r3, [sp, #132]	; 0x84
30009b34:	431c      	orrs	r4, r3
30009b36:	9b07      	ldr	r3, [sp, #28]
30009b38:	4319      	orrs	r1, r3
30009b3a:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009b3c:	431a      	orrs	r2, r3
30009b3e:	9b20      	ldr	r3, [sp, #128]	; 0x80
30009b40:	ea41 010c 	orr.w	r1, r1, ip
30009b44:	431c      	orrs	r4, r3
30009b46:	9b01      	ldr	r3, [sp, #4]
30009b48:	ea41 010e 	orr.w	r1, r1, lr
30009b4c:	431f      	orrs	r7, r3
30009b4e:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30009b50:	ea41 0108 	orr.w	r1, r1, r8
30009b54:	431a      	orrs	r2, r3
30009b56:	9b1f      	ldr	r3, [sp, #124]	; 0x7c
30009b58:	431c      	orrs	r4, r3
30009b5a:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30009b5c:	431a      	orrs	r2, r3
30009b5e:	9b1e      	ldr	r3, [sp, #120]	; 0x78
30009b60:	4323      	orrs	r3, r4
30009b62:	9c0f      	ldr	r4, [sp, #60]	; 0x3c
30009b64:	4322      	orrs	r2, r4
30009b66:	9c18      	ldr	r4, [sp, #96]	; 0x60
30009b68:	4323      	orrs	r3, r4
30009b6a:	9c02      	ldr	r4, [sp, #8]
30009b6c:	430c      	orrs	r4, r1
30009b6e:	49ba      	ldr	r1, [pc, #744]	; (30009e58 <rxi316_perf_tune+0x830>)
30009b70:	f8c1 2400 	str.w	r2, [r1, #1024]	; 0x400
30009b74:	f8c1 3404 	str.w	r3, [r1, #1028]	; 0x404
30009b78:	9b17      	ldr	r3, [sp, #92]	; 0x5c
30009b7a:	f8c1 3408 	str.w	r3, [r1, #1032]	; 0x408
30009b7e:	f8c1 540c 	str.w	r5, [r1, #1036]	; 0x40c
30009b82:	f8c1 4410 	str.w	r4, [r1, #1040]	; 0x410
30009b86:	f8c1 0414 	str.w	r0, [r1, #1044]	; 0x414
30009b8a:	f8c1 b418 	str.w	fp, [r1, #1048]	; 0x418
30009b8e:	f8c1 a41c 	str.w	sl, [r1, #1052]	; 0x41c
30009b92:	f8c1 7420 	str.w	r7, [r1, #1056]	; 0x420
30009b96:	68f3      	ldr	r3, [r6, #12]
30009b98:	689a      	ldr	r2, [r3, #8]
30009b9a:	e9d2 3200 	ldrd	r3, r2, [r2]
30009b9e:	f003 0301 	and.w	r3, r3, #1
30009ba2:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
30009ba6:	f8c1 3460 	str.w	r3, [r1, #1120]	; 0x460
30009baa:	68f3      	ldr	r3, [r6, #12]
30009bac:	689a      	ldr	r2, [r3, #8]
30009bae:	7a13      	ldrb	r3, [r2, #8]
30009bb0:	6950      	ldr	r0, [r2, #20]
30009bb2:	ea43 6300 	orr.w	r3, r3, r0, lsl #24
30009bb6:	e9d2 0203 	ldrd	r0, r2, [r2, #12]
30009bba:	0200      	lsls	r0, r0, #8
30009bbc:	0412      	lsls	r2, r2, #16
30009bbe:	b280      	uxth	r0, r0
30009bc0:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
30009bc4:	4303      	orrs	r3, r0
30009bc6:	4313      	orrs	r3, r2
30009bc8:	f8c1 3464 	str.w	r3, [r1, #1124]	; 0x464
30009bcc:	68f3      	ldr	r3, [r6, #12]
30009bce:	6898      	ldr	r0, [r3, #8]
30009bd0:	e9d0 3207 	ldrd	r3, r2, [r0, #28]
30009bd4:	7e00      	ldrb	r0, [r0, #24]
30009bd6:	021b      	lsls	r3, r3, #8
30009bd8:	0412      	lsls	r2, r2, #16
30009bda:	b29b      	uxth	r3, r3
30009bdc:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
30009be0:	4313      	orrs	r3, r2
30009be2:	4303      	orrs	r3, r0
30009be4:	f8c1 3468 	str.w	r3, [r1, #1128]	; 0x468
30009be8:	68f3      	ldr	r3, [r6, #12]
30009bea:	689a      	ldr	r2, [r3, #8]
30009bec:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
30009bf0:	041b      	lsls	r3, r3, #16
30009bf2:	f002 023f 	and.w	r2, r2, #63	; 0x3f
30009bf6:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
30009bfa:	4313      	orrs	r3, r2
30009bfc:	f8c1 3480 	str.w	r3, [r1, #1152]	; 0x480
30009c00:	6833      	ldr	r3, [r6, #0]
30009c02:	781b      	ldrb	r3, [r3, #0]
30009c04:	2b03      	cmp	r3, #3
30009c06:	f000 811f 	beq.w	30009e48 <rxi316_perf_tune+0x820>
30009c0a:	68f3      	ldr	r3, [r6, #12]
30009c0c:	6899      	ldr	r1, [r3, #8]
30009c0e:	6b0b      	ldr	r3, [r1, #48]	; 0x30
30009c10:	005b      	lsls	r3, r3, #1
30009c12:	f003 0302 	and.w	r3, r3, #2
30009c16:	6b8a      	ldr	r2, [r1, #56]	; 0x38
30009c18:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
30009c1a:	0412      	lsls	r2, r2, #16
30009c1c:	6b49      	ldr	r1, [r1, #52]	; 0x34
30009c1e:	f000 0001 	and.w	r0, r0, #1
30009c22:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009c26:	0209      	lsls	r1, r1, #8
30009c28:	431a      	orrs	r2, r3
30009c2a:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
30009c2e:	4b8a      	ldr	r3, [pc, #552]	; (30009e58 <rxi316_perf_tune+0x830>)
30009c30:	4302      	orrs	r2, r0
30009c32:	488a      	ldr	r0, [pc, #552]	; (30009e5c <rxi316_perf_tune+0x834>)
30009c34:	430a      	orrs	r2, r1
30009c36:	4310      	orrs	r0, r2
30009c38:	f8c3 0484 	str.w	r0, [r3, #1156]	; 0x484
30009c3c:	68f2      	ldr	r2, [r6, #12]
30009c3e:	6892      	ldr	r2, [r2, #8]
30009c40:	6bd2      	ldr	r2, [r2, #60]	; 0x3c
30009c42:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30009c46:	68f2      	ldr	r2, [r6, #12]
30009c48:	6892      	ldr	r2, [r2, #8]
30009c4a:	6c12      	ldr	r2, [r2, #64]	; 0x40
30009c4c:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30009c50:	68f2      	ldr	r2, [r6, #12]
30009c52:	6892      	ldr	r2, [r2, #8]
30009c54:	6c52      	ldr	r2, [r2, #68]	; 0x44
30009c56:	f8c3 2490 	str.w	r2, [r3, #1168]	; 0x490
30009c5a:	68f2      	ldr	r2, [r6, #12]
30009c5c:	6892      	ldr	r2, [r2, #8]
30009c5e:	6c92      	ldr	r2, [r2, #72]	; 0x48
30009c60:	f8c3 2494 	str.w	r2, [r3, #1172]	; 0x494
30009c64:	68f2      	ldr	r2, [r6, #12]
30009c66:	6892      	ldr	r2, [r2, #8]
30009c68:	6cd2      	ldr	r2, [r2, #76]	; 0x4c
30009c6a:	f8c3 2498 	str.w	r2, [r3, #1176]	; 0x498
30009c6e:	68f2      	ldr	r2, [r6, #12]
30009c70:	6892      	ldr	r2, [r2, #8]
30009c72:	6d12      	ldr	r2, [r2, #80]	; 0x50
30009c74:	f002 020f 	and.w	r2, r2, #15
30009c78:	f8c3 24a0 	str.w	r2, [r3, #1184]	; 0x4a0
30009c7c:	f8d3 2470 	ldr.w	r2, [r3, #1136]	; 0x470
30009c80:	f022 023f 	bic.w	r2, r2, #63	; 0x3f
30009c84:	f042 0204 	orr.w	r2, r2, #4
30009c88:	f8c3 2470 	str.w	r2, [r3, #1136]	; 0x470
30009c8c:	f8d3 2474 	ldr.w	r2, [r3, #1140]	; 0x474
30009c90:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
30009c94:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
30009c98:	f8c3 2474 	str.w	r2, [r3, #1140]	; 0x474
30009c9c:	b043      	add	sp, #268	; 0x10c
30009c9e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009ca2:	2502      	movs	r5, #2
30009ca4:	e4cf      	b.n	30009646 <rxi316_perf_tune+0x1e>
30009ca6:	3101      	adds	r1, #1
30009ca8:	9304      	str	r3, [sp, #16]
30009caa:	e536      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009cac:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009cb0:	930b      	str	r3, [sp, #44]	; 0x2c
30009cb2:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009cb4:	2b20      	cmp	r3, #32
30009cb6:	f47f adf7 	bne.w	300098a8 <rxi316_perf_tune+0x280>
30009cba:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
30009cbe:	930a      	str	r3, [sp, #40]	; 0x28
30009cc0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30009cc2:	2b20      	cmp	r3, #32
30009cc4:	f47f adfa 	bne.w	300098bc <rxi316_perf_tune+0x294>
30009cc8:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
30009ccc:	930d      	str	r3, [sp, #52]	; 0x34
30009cce:	9b10      	ldr	r3, [sp, #64]	; 0x40
30009cd0:	2b20      	cmp	r3, #32
30009cd2:	f47f adfd 	bne.w	300098d0 <rxi316_perf_tune+0x2a8>
30009cd6:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
30009cda:	930e      	str	r3, [sp, #56]	; 0x38
30009cdc:	9b11      	ldr	r3, [sp, #68]	; 0x44
30009cde:	2b20      	cmp	r3, #32
30009ce0:	f47f ae00 	bne.w	300098e4 <rxi316_perf_tune+0x2bc>
30009ce4:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009ce8:	930f      	str	r3, [sp, #60]	; 0x3c
30009cea:	9b12      	ldr	r3, [sp, #72]	; 0x48
30009cec:	2b20      	cmp	r3, #32
30009cee:	f47f ae01 	bne.w	300098f4 <rxi316_perf_tune+0x2cc>
30009cf2:	230f      	movs	r3, #15
30009cf4:	9310      	str	r3, [sp, #64]	; 0x40
30009cf6:	9b13      	ldr	r3, [sp, #76]	; 0x4c
30009cf8:	2b20      	cmp	r3, #32
30009cfa:	f47f ae04 	bne.w	30009906 <rxi316_perf_tune+0x2de>
30009cfe:	23f0      	movs	r3, #240	; 0xf0
30009d00:	9311      	str	r3, [sp, #68]	; 0x44
30009d02:	9b14      	ldr	r3, [sp, #80]	; 0x50
30009d04:	2b20      	cmp	r3, #32
30009d06:	f47f ae07 	bne.w	30009918 <rxi316_perf_tune+0x2f0>
30009d0a:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009d0e:	9312      	str	r3, [sp, #72]	; 0x48
30009d10:	9b15      	ldr	r3, [sp, #84]	; 0x54
30009d12:	2b20      	cmp	r3, #32
30009d14:	f47f ae0a 	bne.w	3000992c <rxi316_perf_tune+0x304>
30009d18:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009d1c:	9321      	str	r3, [sp, #132]	; 0x84
30009d1e:	9b16      	ldr	r3, [sp, #88]	; 0x58
30009d20:	2b20      	cmp	r3, #32
30009d22:	f47f ae0c 	bne.w	3000993e <rxi316_perf_tune+0x316>
30009d26:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
30009d2a:	9320      	str	r3, [sp, #128]	; 0x80
30009d2c:	9b17      	ldr	r3, [sp, #92]	; 0x5c
30009d2e:	2b20      	cmp	r3, #32
30009d30:	f47f ae0f 	bne.w	30009952 <rxi316_perf_tune+0x32a>
30009d34:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
30009d38:	931f      	str	r3, [sp, #124]	; 0x7c
30009d3a:	9b18      	ldr	r3, [sp, #96]	; 0x60
30009d3c:	2b20      	cmp	r3, #32
30009d3e:	f47f ae12 	bne.w	30009966 <rxi316_perf_tune+0x33e>
30009d42:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
30009d46:	931e      	str	r3, [sp, #120]	; 0x78
30009d48:	9b19      	ldr	r3, [sp, #100]	; 0x64
30009d4a:	2b20      	cmp	r3, #32
30009d4c:	f47f ae15 	bne.w	3000997a <rxi316_perf_tune+0x352>
30009d50:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009d54:	9318      	str	r3, [sp, #96]	; 0x60
30009d56:	9b1a      	ldr	r3, [sp, #104]	; 0x68
30009d58:	2b20      	cmp	r3, #32
30009d5a:	f47f ae16 	bne.w	3000998a <rxi316_perf_tune+0x362>
30009d5e:	230f      	movs	r3, #15
30009d60:	9317      	str	r3, [sp, #92]	; 0x5c
30009d62:	9b07      	ldr	r3, [sp, #28]
30009d64:	2b20      	cmp	r3, #32
30009d66:	f47f ae19 	bne.w	3000999c <rxi316_perf_tune+0x374>
30009d6a:	231f      	movs	r3, #31
30009d6c:	9316      	str	r3, [sp, #88]	; 0x58
30009d6e:	9b08      	ldr	r3, [sp, #32]
30009d70:	2b20      	cmp	r3, #32
30009d72:	f47f ae1c 	bne.w	300099ae <rxi316_perf_tune+0x386>
30009d76:	f44f 7378 	mov.w	r3, #992	; 0x3e0
30009d7a:	9315      	str	r3, [sp, #84]	; 0x54
30009d7c:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
30009d7e:	2b20      	cmp	r3, #32
30009d80:	f47f ae1f 	bne.w	300099c2 <rxi316_perf_tune+0x39a>
30009d84:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
30009d88:	f1b9 0f20 	cmp.w	r9, #32
30009d8c:	9314      	str	r3, [sp, #80]	; 0x50
30009d8e:	f47f ae24 	bne.w	300099da <rxi316_perf_tune+0x3b2>
30009d92:	230f      	movs	r3, #15
30009d94:	f1b8 0f20 	cmp.w	r8, #32
30009d98:	9313      	str	r3, [sp, #76]	; 0x4c
30009d9a:	f47f ae29 	bne.w	300099f0 <rxi316_perf_tune+0x3c8>
30009d9e:	2f20      	cmp	r7, #32
30009da0:	f04f 01f0 	mov.w	r1, #240	; 0xf0
30009da4:	f47f ae2e 	bne.w	30009a04 <rxi316_perf_tune+0x3dc>
30009da8:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009dac:	f1ba 0f20 	cmp.w	sl, #32
30009db0:	9308      	str	r3, [sp, #32]
30009db2:	f47f ae33 	bne.w	30009a1c <rxi316_perf_tune+0x3f4>
30009db6:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009dba:	f1bb 0f20 	cmp.w	fp, #32
30009dbe:	9307      	str	r3, [sp, #28]
30009dc0:	f47f ae38 	bne.w	30009a34 <rxi316_perf_tune+0x40c>
30009dc4:	9b01      	ldr	r3, [sp, #4]
30009dc6:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
30009dca:	2b20      	cmp	r3, #32
30009dcc:	f47f ae3e 	bne.w	30009a4c <rxi316_perf_tune+0x424>
30009dd0:	9b02      	ldr	r3, [sp, #8]
30009dd2:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
30009dd6:	2b20      	cmp	r3, #32
30009dd8:	f47f ae44 	bne.w	30009a64 <rxi316_perf_tune+0x43c>
30009ddc:	9b04      	ldr	r3, [sp, #16]
30009dde:	f04f 6870 	mov.w	r8, #251658240	; 0xf000000
30009de2:	2b20      	cmp	r3, #32
30009de4:	f47f ae4a 	bne.w	30009a7c <rxi316_perf_tune+0x454>
30009de8:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009dec:	9302      	str	r3, [sp, #8]
30009dee:	9b05      	ldr	r3, [sp, #20]
30009df0:	2b20      	cmp	r3, #32
30009df2:	f47f ae4b 	bne.w	30009a8c <rxi316_perf_tune+0x464>
30009df6:	9b06      	ldr	r3, [sp, #24]
30009df8:	f04f 090f 	mov.w	r9, #15
30009dfc:	2b20      	cmp	r3, #32
30009dfe:	f47f ae4f 	bne.w	30009aa0 <rxi316_perf_tune+0x478>
30009e02:	9b1c      	ldr	r3, [sp, #112]	; 0x70
30009e04:	20f0      	movs	r0, #240	; 0xf0
30009e06:	2b20      	cmp	r3, #32
30009e08:	f47f ae53 	bne.w	30009ab2 <rxi316_perf_tune+0x48a>
30009e0c:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009e0e:	f04f 0b1f 	mov.w	fp, #31
30009e12:	2b20      	cmp	r3, #32
30009e14:	f47f ae57 	bne.w	30009ac6 <rxi316_perf_tune+0x49e>
30009e18:	9b1d      	ldr	r3, [sp, #116]	; 0x74
30009e1a:	f04f 0a1f 	mov.w	sl, #31
30009e1e:	2b20      	cmp	r3, #32
30009e20:	f47f ae5b 	bne.w	30009ada <rxi316_perf_tune+0x4b2>
30009e24:	2c20      	cmp	r4, #32
30009e26:	f04f 071f 	mov.w	r7, #31
30009e2a:	f47f ae65 	bne.w	30009af8 <rxi316_perf_tune+0x4d0>
30009e2e:	f44f 7378 	mov.w	r3, #992	; 0x3e0
30009e32:	9301      	str	r3, [sp, #4]
30009e34:	e666      	b.n	30009b04 <rxi316_perf_tune+0x4dc>
30009e36:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009e3a:	930c      	str	r3, [sp, #48]	; 0x30
30009e3c:	e527      	b.n	3000988e <rxi316_perf_tune+0x266>
30009e3e:	22f0      	movs	r2, #240	; 0xf0
30009e40:	e51b      	b.n	3000987a <rxi316_perf_tune+0x252>
30009e42:	230f      	movs	r3, #15
30009e44:	9303      	str	r3, [sp, #12]
30009e46:	e510      	b.n	3000986a <rxi316_perf_tune+0x242>
30009e48:	68f2      	ldr	r2, [r6, #12]
30009e4a:	2302      	movs	r3, #2
30009e4c:	6891      	ldr	r1, [r2, #8]
30009e4e:	e6e2      	b.n	30009c16 <rxi316_perf_tune+0x5ee>
30009e50:	3101      	adds	r1, #1
30009e52:	9305      	str	r3, [sp, #20]
30009e54:	e461      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009e56:	bf00      	nop
30009e58:	41100000 	.word	0x41100000
30009e5c:	40000008 	.word	0x40000008
30009e60:	3101      	adds	r1, #1
30009e62:	9306      	str	r3, [sp, #24]
30009e64:	e459      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009e66:	2202      	movs	r2, #2
30009e68:	921e      	str	r2, [sp, #120]	; 0x78
30009e6a:	e4e2      	b.n	30009832 <rxi316_perf_tune+0x20a>
30009e6c:	9a03      	ldr	r2, [sp, #12]
30009e6e:	9308      	str	r3, [sp, #32]
30009e70:	3201      	adds	r2, #1
30009e72:	9203      	str	r2, [sp, #12]
30009e74:	e451      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009e76:	9a03      	ldr	r2, [sp, #12]
30009e78:	9307      	str	r3, [sp, #28]
30009e7a:	3201      	adds	r2, #1
30009e7c:	9203      	str	r2, [sp, #12]
30009e7e:	e44c      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009e80:	9a03      	ldr	r2, [sp, #12]
30009e82:	931b      	str	r3, [sp, #108]	; 0x6c
30009e84:	3201      	adds	r2, #1
30009e86:	9203      	str	r2, [sp, #12]
30009e88:	e447      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009e8a:	9a03      	ldr	r2, [sp, #12]
30009e8c:	461c      	mov	r4, r3
30009e8e:	3201      	adds	r2, #1
30009e90:	9203      	str	r2, [sp, #12]
30009e92:	e442      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009e94:	9a03      	ldr	r2, [sp, #12]
30009e96:	931d      	str	r3, [sp, #116]	; 0x74
30009e98:	3201      	adds	r2, #1
30009e9a:	9203      	str	r2, [sp, #12]
30009e9c:	e43d      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009e9e:	9a03      	ldr	r2, [sp, #12]
30009ea0:	931c      	str	r3, [sp, #112]	; 0x70
30009ea2:	3201      	adds	r2, #1
30009ea4:	9203      	str	r2, [sp, #12]
30009ea6:	e438      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ea8:	930e      	str	r3, [sp, #56]	; 0x38
30009eaa:	e436      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009eac:	930d      	str	r3, [sp, #52]	; 0x34
30009eae:	e434      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009eb0:	930c      	str	r3, [sp, #48]	; 0x30
30009eb2:	e432      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009eb4:	930a      	str	r3, [sp, #40]	; 0x28
30009eb6:	e430      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009eb8:	930b      	str	r3, [sp, #44]	; 0x2c
30009eba:	e42e      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ebc:	931a      	str	r3, [sp, #104]	; 0x68
30009ebe:	e42c      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ec0:	9319      	str	r3, [sp, #100]	; 0x64
30009ec2:	e42a      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ec4:	9310      	str	r3, [sp, #64]	; 0x40
30009ec6:	e428      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ec8:	930f      	str	r3, [sp, #60]	; 0x3c
30009eca:	e426      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ecc:	9318      	str	r3, [sp, #96]	; 0x60
30009ece:	e424      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ed0:	9317      	str	r3, [sp, #92]	; 0x5c
30009ed2:	e422      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ed4:	9316      	str	r3, [sp, #88]	; 0x58
30009ed6:	e420      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ed8:	9315      	str	r3, [sp, #84]	; 0x54
30009eda:	e41e      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009edc:	9314      	str	r3, [sp, #80]	; 0x50
30009ede:	e41c      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ee0:	9313      	str	r3, [sp, #76]	; 0x4c
30009ee2:	e41a      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ee4:	9312      	str	r3, [sp, #72]	; 0x48
30009ee6:	e418      	b.n	3000971a <rxi316_perf_tune+0xf2>
30009ee8:	9311      	str	r3, [sp, #68]	; 0x44
30009eea:	e416      	b.n	3000971a <rxi316_perf_tune+0xf2>

30009eec <rxi316_dram_init>:
30009eec:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009ef0:	4604      	mov	r4, r0
30009ef2:	b08f      	sub	sp, #60	; 0x3c
30009ef4:	4b10      	ldr	r3, [pc, #64]	; (30009f38 <rxi316_dram_init+0x4c>)
30009ef6:	4798      	blx	r3
30009ef8:	f894 b014 	ldrb.w	fp, [r4, #20]
30009efc:	2101      	movs	r1, #1
30009efe:	900c      	str	r0, [sp, #48]	; 0x30
30009f00:	fa01 f30b 	lsl.w	r3, r1, fp
30009f04:	9302      	str	r3, [sp, #8]
30009f06:	6823      	ldr	r3, [r4, #0]
30009f08:	78da      	ldrb	r2, [r3, #3]
30009f0a:	6923      	ldr	r3, [r4, #16]
30009f0c:	fa03 f30b 	lsl.w	r3, r3, fp
30009f10:	b1a2      	cbz	r2, 30009f3c <rxi316_dram_init+0x50>
30009f12:	428a      	cmp	r2, r1
30009f14:	f000 8469 	beq.w	3000a7ea <rxi316_dram_init+0x8fe>
30009f18:	2a02      	cmp	r2, #2
30009f1a:	bf0b      	itete	eq
30009f1c:	2206      	moveq	r2, #6
30009f1e:	220a      	movne	r2, #10
30009f20:	2540      	moveq	r5, #64	; 0x40
30009f22:	2580      	movne	r5, #128	; 0x80
30009f24:	9207      	str	r2, [sp, #28]
30009f26:	bf0c      	ite	eq
30009f28:	2204      	moveq	r2, #4
30009f2a:	2208      	movne	r2, #8
30009f2c:	9208      	str	r2, [sp, #32]
30009f2e:	bf0c      	ite	eq
30009f30:	2208      	moveq	r2, #8
30009f32:	2210      	movne	r2, #16
30009f34:	9201      	str	r2, [sp, #4]
30009f36:	e007      	b.n	30009f48 <rxi316_dram_init+0x5c>
30009f38:	30007691 	.word	0x30007691
30009f3c:	2203      	movs	r2, #3
30009f3e:	25c0      	movs	r5, #192	; 0xc0
30009f40:	9108      	str	r1, [sp, #32]
30009f42:	9207      	str	r2, [sp, #28]
30009f44:	2202      	movs	r2, #2
30009f46:	9201      	str	r2, [sp, #4]
30009f48:	6860      	ldr	r0, [r4, #4]
30009f4a:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30009f4e:	4abf      	ldr	r2, [pc, #764]	; (3000a24c <rxi316_dram_init+0x360>)
30009f50:	6806      	ldr	r6, [r0, #0]
30009f52:	960d      	str	r6, [sp, #52]	; 0x34
30009f54:	6846      	ldr	r6, [r0, #4]
30009f56:	6880      	ldr	r0, [r0, #8]
30009f58:	9609      	str	r6, [sp, #36]	; 0x24
30009f5a:	9003      	str	r0, [sp, #12]
30009f5c:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
30009f60:	6820      	ldr	r0, [r4, #0]
30009f62:	7981      	ldrb	r1, [r0, #6]
30009f64:	6882      	ldr	r2, [r0, #8]
30009f66:	b129      	cbz	r1, 30009f74 <rxi316_dram_init+0x88>
30009f68:	2901      	cmp	r1, #1
30009f6a:	f000 8436 	beq.w	3000a7da <rxi316_dram_init+0x8ee>
30009f6e:	2903      	cmp	r1, #3
30009f70:	f000 8443 	beq.w	3000a7fa <rxi316_dram_init+0x90e>
30009f74:	3a06      	subs	r2, #6
30009f76:	7881      	ldrb	r1, [r0, #2]
30009f78:	f002 020f 	and.w	r2, r2, #15
30009f7c:	7940      	ldrb	r0, [r0, #5]
30009f7e:	0109      	lsls	r1, r1, #4
30009f80:	0280      	lsls	r0, r0, #10
30009f82:	f001 0130 	and.w	r1, r1, #48	; 0x30
30009f86:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
30009f8a:	4301      	orrs	r1, r0
30009f8c:	430d      	orrs	r5, r1
30009f8e:	432a      	orrs	r2, r5
30009f90:	4dae      	ldr	r5, [pc, #696]	; (3000a24c <rxi316_dram_init+0x360>)
30009f92:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
30009f96:	68e2      	ldr	r2, [r4, #12]
30009f98:	6812      	ldr	r2, [r2, #0]
30009f9a:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
30009f9e:	f002 0207 	and.w	r2, r2, #7
30009fa2:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
30009fa6:	68e2      	ldr	r2, [r4, #12]
30009fa8:	6811      	ldr	r1, [r2, #0]
30009faa:	6822      	ldr	r2, [r4, #0]
30009fac:	6848      	ldr	r0, [r1, #4]
30009fae:	f892 9000 	ldrb.w	r9, [r2]
30009fb2:	688a      	ldr	r2, [r1, #8]
30009fb4:	9004      	str	r0, [sp, #16]
30009fb6:	0300      	lsls	r0, r0, #12
30009fb8:	0412      	lsls	r2, r2, #16
30009fba:	f009 070f 	and.w	r7, r9, #15
30009fbe:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
30009fc2:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
30009fc6:	4302      	orrs	r2, r0
30009fc8:	68c8      	ldr	r0, [r1, #12]
30009fca:	0440      	lsls	r0, r0, #17
30009fcc:	433a      	orrs	r2, r7
30009fce:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
30009fd2:	6908      	ldr	r0, [r1, #16]
30009fd4:	0480      	lsls	r0, r0, #18
30009fd6:	4332      	orrs	r2, r6
30009fd8:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
30009fdc:	4310      	orrs	r0, r2
30009fde:	694a      	ldr	r2, [r1, #20]
30009fe0:	04d2      	lsls	r2, r2, #19
30009fe2:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
30009fe6:	4310      	orrs	r0, r2
30009fe8:	698a      	ldr	r2, [r1, #24]
30009fea:	0552      	lsls	r2, r2, #21
30009fec:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
30009ff0:	4310      	orrs	r0, r2
30009ff2:	69ca      	ldr	r2, [r1, #28]
30009ff4:	0592      	lsls	r2, r2, #22
30009ff6:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
30009ffa:	4310      	orrs	r0, r2
30009ffc:	6a8a      	ldr	r2, [r1, #40]	; 0x28
30009ffe:	0612      	lsls	r2, r2, #24
3000a000:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000a004:	4302      	orrs	r2, r0
3000a006:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000a00a:	06c0      	lsls	r0, r0, #27
3000a00c:	0789      	lsls	r1, r1, #30
3000a00e:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000a012:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000a016:	4310      	orrs	r0, r2
3000a018:	9a02      	ldr	r2, [sp, #8]
3000a01a:	0212      	lsls	r2, r2, #8
3000a01c:	4308      	orrs	r0, r1
3000a01e:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000a022:	4310      	orrs	r0, r2
3000a024:	6068      	str	r0, [r5, #4]
3000a026:	6822      	ldr	r2, [r4, #0]
3000a028:	7992      	ldrb	r2, [r2, #6]
3000a02a:	2a00      	cmp	r2, #0
3000a02c:	f040 8321 	bne.w	3000a672 <rxi316_dram_init+0x786>
3000a030:	686a      	ldr	r2, [r5, #4]
3000a032:	f042 0210 	orr.w	r2, r2, #16
3000a036:	606a      	str	r2, [r5, #4]
3000a038:	68e2      	ldr	r2, [r4, #12]
3000a03a:	9802      	ldr	r0, [sp, #8]
3000a03c:	6812      	ldr	r2, [r2, #0]
3000a03e:	f100 3aff 	add.w	sl, r0, #4294967295	; 0xffffffff
3000a042:	6861      	ldr	r1, [r4, #4]
3000a044:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000a046:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000a04a:	9005      	str	r0, [sp, #20]
3000a04c:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000a04e:	fa2e f80b 	lsr.w	r8, lr, fp
3000a052:	6949      	ldr	r1, [r1, #20]
3000a054:	ea1e 0f0a 	tst.w	lr, sl
3000a058:	9006      	str	r0, [sp, #24]
3000a05a:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000a05c:	fa21 fe0b 	lsr.w	lr, r1, fp
3000a060:	bf18      	it	ne
3000a062:	f108 0801 	addne.w	r8, r8, #1
3000a066:	ea11 0f0a 	tst.w	r1, sl
3000a06a:	9906      	ldr	r1, [sp, #24]
3000a06c:	950a      	str	r5, [sp, #40]	; 0x28
3000a06e:	bf18      	it	ne
3000a070:	f10e 0e01 	addne.w	lr, lr, #1
3000a074:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000a076:	0049      	lsls	r1, r1, #1
3000a078:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000a07c:	950b      	str	r5, [sp, #44]	; 0x2c
3000a07e:	f001 0102 	and.w	r1, r1, #2
3000a082:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a086:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000a08a:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000a08e:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000a092:	0080      	lsls	r0, r0, #2
3000a094:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000a098:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000a09c:	9006      	str	r0, [sp, #24]
3000a09e:	013f      	lsls	r7, r7, #4
3000a0a0:	9805      	ldr	r0, [sp, #20]
3000a0a2:	f00c 0c08 	and.w	ip, ip, #8
3000a0a6:	0176      	lsls	r6, r6, #5
3000a0a8:	f007 0710 	and.w	r7, r7, #16
3000a0ac:	f000 0001 	and.w	r0, r0, #1
3000a0b0:	f006 0620 	and.w	r6, r6, #32
3000a0b4:	4605      	mov	r5, r0
3000a0b6:	9806      	ldr	r0, [sp, #24]
3000a0b8:	f000 0004 	and.w	r0, r0, #4
3000a0bc:	4301      	orrs	r1, r0
3000a0be:	4329      	orrs	r1, r5
3000a0c0:	ea41 0c0c 	orr.w	ip, r1, ip
3000a0c4:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a0c6:	ea4c 0707 	orr.w	r7, ip, r7
3000a0ca:	018d      	lsls	r5, r1, #6
3000a0cc:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a0ce:	4337      	orrs	r7, r6
3000a0d0:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000a0d4:	0208      	lsls	r0, r1, #8
3000a0d6:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000a0d8:	432f      	orrs	r7, r5
3000a0da:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000a0de:	06c9      	lsls	r1, r1, #27
3000a0e0:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000a0e4:	4307      	orrs	r7, r0
3000a0e6:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000a0ea:	0736      	lsls	r6, r6, #28
3000a0ec:	430f      	orrs	r7, r1
3000a0ee:	0752      	lsls	r2, r2, #29
3000a0f0:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000a0f4:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000a0f8:	433e      	orrs	r6, r7
3000a0fa:	4316      	orrs	r6, r2
3000a0fc:	4a53      	ldr	r2, [pc, #332]	; (3000a24c <rxi316_dram_init+0x360>)
3000a0fe:	ea46 0608 	orr.w	r6, r6, r8
3000a102:	ea46 060e 	orr.w	r6, r6, lr
3000a106:	6096      	str	r6, [r2, #8]
3000a108:	6862      	ldr	r2, [r4, #4]
3000a10a:	6990      	ldr	r0, [r2, #24]
3000a10c:	9a01      	ldr	r2, [sp, #4]
3000a10e:	2a10      	cmp	r2, #16
3000a110:	6822      	ldr	r2, [r4, #0]
3000a112:	7811      	ldrb	r1, [r2, #0]
3000a114:	7916      	ldrb	r6, [r2, #4]
3000a116:	f000 8321 	beq.w	3000a75c <rxi316_dram_init+0x870>
3000a11a:	f1a1 0502 	sub.w	r5, r1, #2
3000a11e:	f1a1 0e09 	sub.w	lr, r1, #9
3000a122:	9f01      	ldr	r7, [sp, #4]
3000a124:	fab5 f585 	clz	r5, r5
3000a128:	fabe fe8e 	clz	lr, lr
3000a12c:	2f08      	cmp	r7, #8
3000a12e:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000a132:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000a136:	ea45 020e 	orr.w	r2, r5, lr
3000a13a:	f000 82a8 	beq.w	3000a68e <rxi316_dram_init+0x7a2>
3000a13e:	9f01      	ldr	r7, [sp, #4]
3000a140:	2f04      	cmp	r7, #4
3000a142:	f000 837c 	beq.w	3000a83e <rxi316_dram_init+0x952>
3000a146:	9f01      	ldr	r7, [sp, #4]
3000a148:	2f02      	cmp	r7, #2
3000a14a:	bf08      	it	eq
3000a14c:	4310      	orreq	r0, r2
3000a14e:	2a00      	cmp	r2, #0
3000a150:	f040 82a2 	bne.w	3000a698 <rxi316_dram_init+0x7ac>
3000a154:	2903      	cmp	r1, #3
3000a156:	f000 8355 	beq.w	3000a804 <rxi316_dram_init+0x918>
3000a15a:	2500      	movs	r5, #0
3000a15c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a160:	462f      	mov	r7, r5
3000a162:	433d      	orrs	r5, r7
3000a164:	4f39      	ldr	r7, [pc, #228]	; (3000a24c <rxi316_dram_init+0x360>)
3000a166:	430d      	orrs	r5, r1
3000a168:	4305      	orrs	r5, r0
3000a16a:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000a16e:	637e      	str	r6, [r7, #52]	; 0x34
3000a170:	6862      	ldr	r2, [r4, #4]
3000a172:	9903      	ldr	r1, [sp, #12]
3000a174:	69d2      	ldr	r2, [r2, #28]
3000a176:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000a17a:	63ba      	str	r2, [r7, #56]	; 0x38
3000a17c:	6822      	ldr	r2, [r4, #0]
3000a17e:	7810      	ldrb	r0, [r2, #0]
3000a180:	6862      	ldr	r2, [r4, #4]
3000a182:	2803      	cmp	r0, #3
3000a184:	6a11      	ldr	r1, [r2, #32]
3000a186:	f000 82dc 	beq.w	3000a742 <rxi316_dram_init+0x856>
3000a18a:	2809      	cmp	r0, #9
3000a18c:	f000 82fa 	beq.w	3000a784 <rxi316_dram_init+0x898>
3000a190:	482e      	ldr	r0, [pc, #184]	; (3000a24c <rxi316_dram_init+0x360>)
3000a192:	63c1      	str	r1, [r0, #60]	; 0x3c
3000a194:	6862      	ldr	r2, [r4, #4]
3000a196:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000a198:	6402      	str	r2, [r0, #64]	; 0x40
3000a19a:	6860      	ldr	r0, [r4, #4]
3000a19c:	68c5      	ldr	r5, [r0, #12]
3000a19e:	6802      	ldr	r2, [r0, #0]
3000a1a0:	6841      	ldr	r1, [r0, #4]
3000a1a2:	442a      	add	r2, r5
3000a1a4:	6880      	ldr	r0, [r0, #8]
3000a1a6:	4429      	add	r1, r5
3000a1a8:	ea1a 0f02 	tst.w	sl, r2
3000a1ac:	fa22 f20b 	lsr.w	r2, r2, fp
3000a1b0:	fa21 f60b 	lsr.w	r6, r1, fp
3000a1b4:	fa20 f50b 	lsr.w	r5, r0, fp
3000a1b8:	bf18      	it	ne
3000a1ba:	3201      	addne	r2, #1
3000a1bc:	ea1a 0f01 	tst.w	sl, r1
3000a1c0:	6821      	ldr	r1, [r4, #0]
3000a1c2:	bf18      	it	ne
3000a1c4:	3601      	addne	r6, #1
3000a1c6:	ea1a 0f00 	tst.w	sl, r0
3000a1ca:	7808      	ldrb	r0, [r1, #0]
3000a1cc:	bf18      	it	ne
3000a1ce:	3501      	addne	r5, #1
3000a1d0:	68a1      	ldr	r1, [r4, #8]
3000a1d2:	3802      	subs	r0, #2
3000a1d4:	6dcf      	ldr	r7, [r1, #92]	; 0x5c
3000a1d6:	2801      	cmp	r0, #1
3000a1d8:	f240 82ac 	bls.w	3000a734 <rxi316_dram_init+0x848>
3000a1dc:	698f      	ldr	r7, [r1, #24]
3000a1de:	fbb7 f0f3 	udiv	r0, r7, r3
3000a1e2:	fb03 7710 	mls	r7, r3, r0, r7
3000a1e6:	b107      	cbz	r7, 3000a1ea <rxi316_dram_init+0x2fe>
3000a1e8:	3001      	adds	r0, #1
3000a1ea:	6a4f      	ldr	r7, [r1, #36]	; 0x24
3000a1ec:	fbb7 f1f3 	udiv	r1, r7, r3
3000a1f0:	fb03 7711 	mls	r7, r3, r1, r7
3000a1f4:	b107      	cbz	r7, 3000a1f8 <rxi316_dram_init+0x30c>
3000a1f6:	3101      	adds	r1, #1
3000a1f8:	0176      	lsls	r6, r6, #5
3000a1fa:	f002 021f 	and.w	r2, r2, #31
3000a1fe:	02ad      	lsls	r5, r5, #10
3000a200:	f406 7678 	and.w	r6, r6, #992	; 0x3e0
3000a204:	0500      	lsls	r0, r0, #20
3000a206:	f405 45f8 	and.w	r5, r5, #31744	; 0x7c00
3000a20a:	4316      	orrs	r6, r2
3000a20c:	f000 70f8 	and.w	r0, r0, #32505856	; 0x1f00000
3000a210:	4a0e      	ldr	r2, [pc, #56]	; (3000a24c <rxi316_dram_init+0x360>)
3000a212:	432e      	orrs	r6, r5
3000a214:	4306      	orrs	r6, r0
3000a216:	0648      	lsls	r0, r1, #25
3000a218:	f000 5078 	and.w	r0, r0, #1040187392	; 0x3e000000
3000a21c:	4330      	orrs	r0, r6
3000a21e:	6310      	str	r0, [r2, #48]	; 0x30
3000a220:	68a0      	ldr	r0, [r4, #8]
3000a222:	6802      	ldr	r2, [r0, #0]
3000a224:	fbb2 f1f3 	udiv	r1, r2, r3
3000a228:	fb03 2211 	mls	r2, r3, r1, r2
3000a22c:	2a00      	cmp	r2, #0
3000a22e:	f000 827f 	beq.w	3000a730 <rxi316_dram_init+0x844>
3000a232:	1c4a      	adds	r2, r1, #1
3000a234:	9205      	str	r2, [sp, #20]
3000a236:	6842      	ldr	r2, [r0, #4]
3000a238:	fbb2 f0f3 	udiv	r0, r2, r3
3000a23c:	fb03 2210 	mls	r2, r3, r0, r2
3000a240:	2a00      	cmp	r2, #0
3000a242:	f000 8273 	beq.w	3000a72c <rxi316_dram_init+0x840>
3000a246:	38c7      	subs	r0, #199	; 0xc7
3000a248:	e002      	b.n	3000a250 <rxi316_dram_init+0x364>
3000a24a:	bf00      	nop
3000a24c:	41100000 	.word	0x41100000
3000a250:	68e2      	ldr	r2, [r4, #12]
3000a252:	6811      	ldr	r1, [r2, #0]
3000a254:	4ac3      	ldr	r2, [pc, #780]	; (3000a564 <rxi316_dram_init+0x678>)
3000a256:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000a25a:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000a25e:	900a      	str	r0, [sp, #40]	; 0x28
3000a260:	0712      	lsls	r2, r2, #28
3000a262:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000a266:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000a26a:	430a      	orrs	r2, r1
3000a26c:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000a270:	430a      	orrs	r2, r1
3000a272:	49bd      	ldr	r1, [pc, #756]	; (3000a568 <rxi316_dram_init+0x67c>)
3000a274:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000a278:	610a      	str	r2, [r1, #16]
3000a27a:	68a5      	ldr	r5, [r4, #8]
3000a27c:	692a      	ldr	r2, [r5, #16]
3000a27e:	fbb2 fcf3 	udiv	ip, r2, r3
3000a282:	fb03 221c 	mls	r2, r3, ip, r2
3000a286:	2a00      	cmp	r2, #0
3000a288:	f000 824d 	beq.w	3000a726 <rxi316_dram_init+0x83a>
3000a28c:	f10c 0201 	add.w	r2, ip, #1
3000a290:	9206      	str	r2, [sp, #24]
3000a292:	69aa      	ldr	r2, [r5, #24]
3000a294:	fbb2 f0f3 	udiv	r0, r2, r3
3000a298:	fb03 2210 	mls	r2, r3, r0, r2
3000a29c:	b102      	cbz	r2, 3000a2a0 <rxi316_dram_init+0x3b4>
3000a29e:	3001      	adds	r0, #1
3000a2a0:	68ea      	ldr	r2, [r5, #12]
3000a2a2:	fbb2 f1f3 	udiv	r1, r2, r3
3000a2a6:	fb03 2211 	mls	r2, r3, r1, r2
3000a2aa:	9103      	str	r1, [sp, #12]
3000a2ac:	b10a      	cbz	r2, 3000a2b2 <rxi316_dram_init+0x3c6>
3000a2ae:	3101      	adds	r1, #1
3000a2b0:	9103      	str	r1, [sp, #12]
3000a2b2:	6a6a      	ldr	r2, [r5, #36]	; 0x24
3000a2b4:	fbb2 f1f3 	udiv	r1, r2, r3
3000a2b8:	fb03 2211 	mls	r2, r3, r1, r2
3000a2bc:	b102      	cbz	r2, 3000a2c0 <rxi316_dram_init+0x3d4>
3000a2be:	3101      	adds	r1, #1
3000a2c0:	6822      	ldr	r2, [r4, #0]
3000a2c2:	7812      	ldrb	r2, [r2, #0]
3000a2c4:	2a09      	cmp	r2, #9
3000a2c6:	f000 8259 	beq.w	3000a77c <rxi316_dram_init+0x890>
3000a2ca:	2a02      	cmp	r2, #2
3000a2cc:	d104      	bne.n	3000a2d8 <rxi316_dram_init+0x3ec>
3000a2ce:	9a08      	ldr	r2, [sp, #32]
3000a2d0:	3a02      	subs	r2, #2
3000a2d2:	fa22 f20b 	lsr.w	r2, r2, fp
3000a2d6:	4411      	add	r1, r2
3000a2d8:	68e2      	ldr	r2, [r4, #12]
3000a2da:	0349      	lsls	r1, r1, #13
3000a2dc:	6d2d      	ldr	r5, [r5, #80]	; 0x50
3000a2de:	6812      	ldr	r2, [r2, #0]
3000a2e0:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000a2e4:	fa25 f60b 	lsr.w	r6, r5, fp
3000a2e8:	ea15 0f0a 	tst.w	r5, sl
3000a2ec:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000a2ee:	bf18      	it	ne
3000a2f0:	3601      	addne	r6, #1
3000a2f2:	05d2      	lsls	r2, r2, #23
3000a2f4:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000a2f8:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000a2fc:	9a06      	ldr	r2, [sp, #24]
3000a2fe:	0112      	lsls	r2, r2, #4
3000a300:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000a304:	4332      	orrs	r2, r6
3000a306:	0246      	lsls	r6, r0, #9
3000a308:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000a30c:	4316      	orrs	r6, r2
3000a30e:	9a03      	ldr	r2, [sp, #12]
3000a310:	f002 020f 	and.w	r2, r2, #15
3000a314:	4316      	orrs	r6, r2
3000a316:	4a94      	ldr	r2, [pc, #592]	; (3000a568 <rxi316_dram_init+0x67c>)
3000a318:	4331      	orrs	r1, r6
3000a31a:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000a31e:	6151      	str	r1, [r2, #20]
3000a320:	68a7      	ldr	r7, [r4, #8]
3000a322:	697a      	ldr	r2, [r7, #20]
3000a324:	fbb2 fef3 	udiv	lr, r2, r3
3000a328:	fb03 221e 	mls	r2, r3, lr, r2
3000a32c:	b10a      	cbz	r2, 3000a332 <rxi316_dram_init+0x446>
3000a32e:	f10e 0e01 	add.w	lr, lr, #1
3000a332:	6939      	ldr	r1, [r7, #16]
3000a334:	fbb1 f2f3 	udiv	r2, r1, r3
3000a338:	fb03 1112 	mls	r1, r3, r2, r1
3000a33c:	b101      	cbz	r1, 3000a340 <rxi316_dram_init+0x454>
3000a33e:	3201      	adds	r2, #1
3000a340:	68fd      	ldr	r5, [r7, #12]
3000a342:	fbb5 f1f3 	udiv	r1, r5, r3
3000a346:	fb03 5511 	mls	r5, r3, r1, r5
3000a34a:	b105      	cbz	r5, 3000a34e <rxi316_dram_init+0x462>
3000a34c:	3101      	adds	r1, #1
3000a34e:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000a350:	fbb5 f6f3 	udiv	r6, r5, r3
3000a354:	fb03 5516 	mls	r5, r3, r6, r5
3000a358:	b105      	cbz	r5, 3000a35c <rxi316_dram_init+0x470>
3000a35a:	3601      	adds	r6, #1
3000a35c:	68bd      	ldr	r5, [r7, #8]
3000a35e:	4411      	add	r1, r2
3000a360:	fbb5 f2f3 	udiv	r2, r5, r3
3000a364:	42b1      	cmp	r1, r6
3000a366:	fb03 5512 	mls	r5, r3, r2, r5
3000a36a:	bf38      	it	cc
3000a36c:	4631      	movcc	r1, r6
3000a36e:	b905      	cbnz	r5, 3000a372 <rxi316_dram_init+0x486>
3000a370:	3a01      	subs	r2, #1
3000a372:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000a374:	ea15 0f0a 	tst.w	r5, sl
3000a378:	fa25 fc0b 	lsr.w	ip, r5, fp
3000a37c:	69fd      	ldr	r5, [r7, #28]
3000a37e:	bf18      	it	ne
3000a380:	f10c 0c01 	addne.w	ip, ip, #1
3000a384:	fa25 f60b 	lsr.w	r6, r5, fp
3000a388:	ea15 0f0a 	tst.w	r5, sl
3000a38c:	bf18      	it	ne
3000a38e:	3601      	addne	r6, #1
3000a390:	f1b9 0f09 	cmp.w	r9, #9
3000a394:	f000 81ed 	beq.w	3000a772 <rxi316_dram_init+0x886>
3000a398:	f1b9 0f02 	cmp.w	r9, #2
3000a39c:	f000 81f4 	beq.w	3000a788 <rxi316_dram_init+0x89c>
3000a3a0:	f1b9 0f03 	cmp.w	r9, #3
3000a3a4:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000a3a6:	f000 827d 	beq.w	3000a8a4 <rxi316_dram_init+0x9b8>
3000a3aa:	46a8      	mov	r8, r5
3000a3ac:	9d01      	ldr	r5, [sp, #4]
3000a3ae:	44a8      	add	r8, r5
3000a3b0:	fa28 f50b 	lsr.w	r5, r8, fp
3000a3b4:	ea18 0f0a 	tst.w	r8, sl
3000a3b8:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000a3bc:	bf18      	it	ne
3000a3be:	3501      	addne	r5, #1
3000a3c0:	fbb8 f7f3 	udiv	r7, r8, r3
3000a3c4:	fb03 8817 	mls	r8, r3, r7, r8
3000a3c8:	f1b8 0f00 	cmp.w	r8, #0
3000a3cc:	d000      	beq.n	3000a3d0 <rxi316_dram_init+0x4e4>
3000a3ce:	3701      	adds	r7, #1
3000a3d0:	0109      	lsls	r1, r1, #4
3000a3d2:	f00e 0e0f 	and.w	lr, lr, #15
3000a3d6:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000a3da:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000a3de:	063f      	lsls	r7, r7, #24
3000a3e0:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000a3e4:	ea41 0e0e 	orr.w	lr, r1, lr
3000a3e8:	0291      	lsls	r1, r2, #10
3000a3ea:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000a3ee:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000a3f2:	ea4e 0e01 	orr.w	lr, lr, r1
3000a3f6:	0471      	lsls	r1, r6, #17
3000a3f8:	ea4e 0c0c 	orr.w	ip, lr, ip
3000a3fc:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000a400:	ea4c 0c01 	orr.w	ip, ip, r1
3000a404:	0529      	lsls	r1, r5, #20
3000a406:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a40a:	ea4c 0c01 	orr.w	ip, ip, r1
3000a40e:	4956      	ldr	r1, [pc, #344]	; (3000a568 <rxi316_dram_init+0x67c>)
3000a410:	ea4c 0707 	orr.w	r7, ip, r7
3000a414:	618f      	str	r7, [r1, #24]
3000a416:	6821      	ldr	r1, [r4, #0]
3000a418:	780f      	ldrb	r7, [r1, #0]
3000a41a:	2f02      	cmp	r7, #2
3000a41c:	f000 8153 	beq.w	3000a6c6 <rxi316_dram_init+0x7da>
3000a420:	2f09      	cmp	r7, #9
3000a422:	f000 8150 	beq.w	3000a6c6 <rxi316_dram_init+0x7da>
3000a426:	68e1      	ldr	r1, [r4, #12]
3000a428:	2f03      	cmp	r7, #3
3000a42a:	6809      	ldr	r1, [r1, #0]
3000a42c:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000a42e:	f001 080f 	and.w	r8, r1, #15
3000a432:	68a1      	ldr	r1, [r4, #8]
3000a434:	6a09      	ldr	r1, [r1, #32]
3000a436:	d005      	beq.n	3000a444 <rxi316_dram_init+0x558>
3000a438:	ea0a 0701 	and.w	r7, sl, r1
3000a43c:	fa21 f10b 	lsr.w	r1, r1, fp
3000a440:	b107      	cbz	r7, 3000a444 <rxi316_dram_init+0x558>
3000a442:	3101      	adds	r1, #1
3000a444:	f1b9 0f02 	cmp.w	r9, #2
3000a448:	f000 8155 	beq.w	3000a6f6 <rxi316_dram_init+0x80a>
3000a44c:	f1b9 0f03 	cmp.w	r9, #3
3000a450:	f000 819d 	beq.w	3000a78e <rxi316_dram_init+0x8a2>
3000a454:	f1b9 0f09 	cmp.w	r9, #9
3000a458:	f000 8214 	beq.w	3000a884 <rxi316_dram_init+0x998>
3000a45c:	f04f 0e00 	mov.w	lr, #0
3000a460:	46f1      	mov	r9, lr
3000a462:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000a466:	0109      	lsls	r1, r1, #4
3000a468:	9f06      	ldr	r7, [sp, #24]
3000a46a:	b2c9      	uxtb	r1, r1
3000a46c:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000a470:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a472:	ea41 0108 	orr.w	r1, r1, r8
3000a476:	4339      	orrs	r1, r7
3000a478:	4f3b      	ldr	r7, [pc, #236]	; (3000a568 <rxi316_dram_init+0x67c>)
3000a47a:	ea41 0109 	orr.w	r1, r1, r9
3000a47e:	ea41 010e 	orr.w	r1, r1, lr
3000a482:	61f9      	str	r1, [r7, #28]
3000a484:	68a7      	ldr	r7, [r4, #8]
3000a486:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000a488:	fbb4 f1f3 	udiv	r1, r4, r3
3000a48c:	fb03 4311 	mls	r3, r3, r1, r4
3000a490:	b103      	cbz	r3, 3000a494 <rxi316_dram_init+0x5a8>
3000a492:	3101      	adds	r1, #1
3000a494:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000a496:	094b      	lsrs	r3, r1, #5
3000a498:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000a49c:	0912      	lsrs	r2, r2, #4
3000a49e:	ea14 0f0a 	tst.w	r4, sl
3000a4a2:	fa24 f80b 	lsr.w	r8, r4, fp
3000a4a6:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000a4a8:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000a4ac:	bf18      	it	ne
3000a4ae:	f108 0801 	addne.w	r8, r8, #1
3000a4b2:	9306      	str	r3, [sp, #24]
3000a4b4:	ea14 0f0a 	tst.w	r4, sl
3000a4b8:	fa24 fe0b 	lsr.w	lr, r4, fp
3000a4bc:	9b01      	ldr	r3, [sp, #4]
3000a4be:	fa29 f40b 	lsr.w	r4, r9, fp
3000a4c2:	bf18      	it	ne
3000a4c4:	f10e 0e01 	addne.w	lr, lr, #1
3000a4c8:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000a4ca:	ea19 0f0a 	tst.w	r9, sl
3000a4ce:	fa23 f90b 	lsr.w	r9, r3, fp
3000a4d2:	9905      	ldr	r1, [sp, #20]
3000a4d4:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000a4d8:	bf18      	it	ne
3000a4da:	3401      	addne	r4, #1
3000a4dc:	45b9      	cmp	r9, r7
3000a4de:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000a4e2:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000a4e6:	f240 80ec 	bls.w	3000a6c2 <rxi316_dram_init+0x7d6>
3000a4ea:	eba9 0707 	sub.w	r7, r9, r7
3000a4ee:	fa27 f40b 	lsr.w	r4, r7, fp
3000a4f2:	ea17 070a 	ands.w	r7, r7, sl
3000a4f6:	d001      	beq.n	3000a4fc <rxi316_dram_init+0x610>
3000a4f8:	3401      	adds	r4, #1
3000a4fa:	2700      	movs	r7, #0
3000a4fc:	9b04      	ldr	r3, [sp, #16]
3000a4fe:	b10b      	cbz	r3, 3000a504 <rxi316_dram_init+0x618>
3000a500:	073b      	lsls	r3, r7, #28
3000a502:	9304      	str	r3, [sp, #16]
3000a504:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000a508:	9b06      	ldr	r3, [sp, #24]
3000a50a:	0289      	lsls	r1, r1, #10
3000a50c:	02c0      	lsls	r0, r0, #11
3000a50e:	f00c 0c04 	and.w	ip, ip, #4
3000a512:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000a516:	01d2      	lsls	r2, r2, #7
3000a518:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000a51c:	0236      	lsls	r6, r6, #8
3000a51e:	ea4c 0c01 	orr.w	ip, ip, r1
3000a522:	b2d2      	uxtb	r2, r2
3000a524:	026d      	lsls	r5, r5, #9
3000a526:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000a52a:	ea4c 0c00 	orr.w	ip, ip, r0
3000a52e:	00df      	lsls	r7, r3, #3
3000a530:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000a534:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000a538:	ea4c 0202 	orr.w	r2, ip, r2
3000a53c:	f007 0708 	and.w	r7, r7, #8
3000a540:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000a544:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000a548:	4332      	orrs	r2, r6
3000a54a:	0524      	lsls	r4, r4, #20
3000a54c:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000a550:	4905      	ldr	r1, [pc, #20]	; (3000a568 <rxi316_dram_init+0x67c>)
3000a552:	ea42 0305 	orr.w	r3, r2, r5
3000a556:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000a55a:	9a04      	ldr	r2, [sp, #16]
3000a55c:	433b      	orrs	r3, r7
3000a55e:	ea43 0308 	orr.w	r3, r3, r8
3000a562:	e003      	b.n	3000a56c <rxi316_dram_init+0x680>
3000a564:	00ffff00 	.word	0x00ffff00
3000a568:	41100000 	.word	0x41100000
3000a56c:	ea43 030e 	orr.w	r3, r3, lr
3000a570:	4323      	orrs	r3, r4
3000a572:	4313      	orrs	r3, r2
3000a574:	9a03      	ldr	r2, [sp, #12]
3000a576:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000a57a:	620b      	str	r3, [r1, #32]
3000a57c:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000a580:	9b02      	ldr	r3, [sp, #8]
3000a582:	2b01      	cmp	r3, #1
3000a584:	d002      	beq.n	3000a58c <rxi316_dram_init+0x6a0>
3000a586:	9b08      	ldr	r3, [sp, #32]
3000a588:	3303      	adds	r3, #3
3000a58a:	9307      	str	r3, [sp, #28]
3000a58c:	4bb2      	ldr	r3, [pc, #712]	; (3000a858 <rxi316_dram_init+0x96c>)
3000a58e:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000a592:	9a07      	ldr	r2, [sp, #28]
3000a594:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000a598:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000a59c:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000a5a0:	60da      	str	r2, [r3, #12]
3000a5a2:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a5a6:	430a      	orrs	r2, r1
3000a5a8:	49ac      	ldr	r1, [pc, #688]	; (3000a85c <rxi316_dram_init+0x970>)
3000a5aa:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a5ae:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000a5b2:	0096      	lsls	r6, r2, #2
3000a5b4:	d57f      	bpl.n	3000a6b6 <rxi316_dram_init+0x7ca>
3000a5b6:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000a5ba:	4ca7      	ldr	r4, [pc, #668]	; (3000a858 <rxi316_dram_init+0x96c>)
3000a5bc:	601a      	str	r2, [r3, #0]
3000a5be:	691a      	ldr	r2, [r3, #16]
3000a5c0:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000a5c4:	611a      	str	r2, [r3, #16]
3000a5c6:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000a5ca:	f043 0303 	orr.w	r3, r3, #3
3000a5ce:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000a5d2:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000a5d6:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a5d8:	2b01      	cmp	r3, #1
3000a5da:	4ba1      	ldr	r3, [pc, #644]	; (3000a860 <rxi316_dram_init+0x974>)
3000a5dc:	bf0c      	ite	eq
3000a5de:	4aa1      	ldreq	r2, [pc, #644]	; (3000a864 <rxi316_dram_init+0x978>)
3000a5e0:	4aa1      	ldrne	r2, [pc, #644]	; (3000a868 <rxi316_dram_init+0x97c>)
3000a5e2:	601a      	str	r2, [r3, #0]
3000a5e4:	2203      	movs	r2, #3
3000a5e6:	68e0      	ldr	r0, [r4, #12]
3000a5e8:	4ba0      	ldr	r3, [pc, #640]	; (3000a86c <rxi316_dram_init+0x980>)
3000a5ea:	4003      	ands	r3, r0
3000a5ec:	60e3      	str	r3, [r4, #12]
3000a5ee:	2308      	movs	r3, #8
3000a5f0:	62a1      	str	r1, [r4, #40]	; 0x28
3000a5f2:	62e2      	str	r2, [r4, #44]	; 0x2c
3000a5f4:	6023      	str	r3, [r4, #0]
3000a5f6:	6823      	ldr	r3, [r4, #0]
3000a5f8:	071d      	lsls	r5, r3, #28
3000a5fa:	d5fc      	bpl.n	3000a5f6 <rxi316_dram_init+0x70a>
3000a5fc:	4b9c      	ldr	r3, [pc, #624]	; (3000a870 <rxi316_dram_init+0x984>)
3000a5fe:	2001      	movs	r0, #1
3000a600:	4798      	blx	r3
3000a602:	68e2      	ldr	r2, [r4, #12]
3000a604:	4b99      	ldr	r3, [pc, #612]	; (3000a86c <rxi316_dram_init+0x980>)
3000a606:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000a60a:	4013      	ands	r3, r2
3000a60c:	2203      	movs	r2, #3
3000a60e:	60e3      	str	r3, [r4, #12]
3000a610:	2308      	movs	r3, #8
3000a612:	62a1      	str	r1, [r4, #40]	; 0x28
3000a614:	62e2      	str	r2, [r4, #44]	; 0x2c
3000a616:	4a90      	ldr	r2, [pc, #576]	; (3000a858 <rxi316_dram_init+0x96c>)
3000a618:	6023      	str	r3, [r4, #0]
3000a61a:	6813      	ldr	r3, [r2, #0]
3000a61c:	071c      	lsls	r4, r3, #28
3000a61e:	d5fc      	bpl.n	3000a61a <rxi316_dram_init+0x72e>
3000a620:	68d0      	ldr	r0, [r2, #12]
3000a622:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000a626:	4b91      	ldr	r3, [pc, #580]	; (3000a86c <rxi316_dram_init+0x980>)
3000a628:	4003      	ands	r3, r0
3000a62a:	2008      	movs	r0, #8
3000a62c:	60d3      	str	r3, [r2, #12]
3000a62e:	6291      	str	r1, [r2, #40]	; 0x28
3000a630:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000a632:	4989      	ldr	r1, [pc, #548]	; (3000a858 <rxi316_dram_init+0x96c>)
3000a634:	f023 0302 	bic.w	r3, r3, #2
3000a638:	62d3      	str	r3, [r2, #44]	; 0x2c
3000a63a:	6010      	str	r0, [r2, #0]
3000a63c:	680b      	ldr	r3, [r1, #0]
3000a63e:	0718      	lsls	r0, r3, #28
3000a640:	d5fc      	bpl.n	3000a63c <rxi316_dram_init+0x750>
3000a642:	4a85      	ldr	r2, [pc, #532]	; (3000a858 <rxi316_dram_init+0x96c>)
3000a644:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000a648:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a64c:	60d3      	str	r3, [r2, #12]
3000a64e:	980a      	ldr	r0, [sp, #40]	; 0x28
3000a650:	6913      	ldr	r3, [r2, #16]
3000a652:	4318      	orrs	r0, r3
3000a654:	6110      	str	r0, [r2, #16]
3000a656:	6913      	ldr	r3, [r2, #16]
3000a658:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000a65c:	6113      	str	r3, [r2, #16]
3000a65e:	6011      	str	r1, [r2, #0]
3000a660:	68d3      	ldr	r3, [r2, #12]
3000a662:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000a666:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000a66a:	d1f9      	bne.n	3000a660 <rxi316_dram_init+0x774>
3000a66c:	b00f      	add	sp, #60	; 0x3c
3000a66e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a672:	2a01      	cmp	r2, #1
3000a674:	f000 80b4 	beq.w	3000a7e0 <rxi316_dram_init+0x8f4>
3000a678:	2a02      	cmp	r2, #2
3000a67a:	f000 811d 	beq.w	3000a8b8 <rxi316_dram_init+0x9cc>
3000a67e:	2a03      	cmp	r2, #3
3000a680:	f47f acda 	bne.w	3000a038 <rxi316_dram_init+0x14c>
3000a684:	686a      	ldr	r2, [r5, #4]
3000a686:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a68a:	606a      	str	r2, [r5, #4]
3000a68c:	e4d4      	b.n	3000a038 <rxi316_dram_init+0x14c>
3000a68e:	2a00      	cmp	r2, #0
3000a690:	f43f ad60 	beq.w	3000a154 <rxi316_dram_init+0x268>
3000a694:	2203      	movs	r2, #3
3000a696:	4310      	orrs	r0, r2
3000a698:	68a2      	ldr	r2, [r4, #8]
3000a69a:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a69c:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000a69e:	013f      	lsls	r7, r7, #4
3000a6a0:	2d00      	cmp	r5, #0
3000a6a2:	f000 80ba 	beq.w	3000a81a <rxi316_dram_init+0x92e>
3000a6a6:	1e91      	subs	r1, r2, #2
3000a6a8:	2906      	cmp	r1, #6
3000a6aa:	f200 8093 	bhi.w	3000a7d4 <rxi316_dram_init+0x8e8>
3000a6ae:	1e55      	subs	r5, r2, #1
3000a6b0:	2100      	movs	r1, #0
3000a6b2:	026d      	lsls	r5, r5, #9
3000a6b4:	e555      	b.n	3000a162 <rxi316_dram_init+0x276>
3000a6b6:	2201      	movs	r2, #1
3000a6b8:	601a      	str	r2, [r3, #0]
3000a6ba:	681a      	ldr	r2, [r3, #0]
3000a6bc:	07d2      	lsls	r2, r2, #31
3000a6be:	d5fc      	bpl.n	3000a6ba <rxi316_dram_init+0x7ce>
3000a6c0:	e7bf      	b.n	3000a642 <rxi316_dram_init+0x756>
3000a6c2:	2701      	movs	r7, #1
3000a6c4:	e71a      	b.n	3000a4fc <rxi316_dram_init+0x610>
3000a6c6:	68a1      	ldr	r1, [r4, #8]
3000a6c8:	2f03      	cmp	r7, #3
3000a6ca:	6a09      	ldr	r1, [r1, #32]
3000a6cc:	f000 80a2 	beq.w	3000a814 <rxi316_dram_init+0x928>
3000a6d0:	2f02      	cmp	r7, #2
3000a6d2:	f040 80fc 	bne.w	3000a8ce <rxi316_dram_init+0x9e2>
3000a6d6:	fbb1 fcf3 	udiv	ip, r1, r3
3000a6da:	fb03 1e1c 	mls	lr, r3, ip, r1
3000a6de:	4661      	mov	r1, ip
3000a6e0:	f1be 0f00 	cmp.w	lr, #0
3000a6e4:	f000 808c 	beq.w	3000a800 <rxi316_dram_init+0x914>
3000a6e8:	f1b9 0f02 	cmp.w	r9, #2
3000a6ec:	f101 0101 	add.w	r1, r1, #1
3000a6f0:	46b8      	mov	r8, r7
3000a6f2:	f47f aeab 	bne.w	3000a44c <rxi316_dram_init+0x560>
3000a6f6:	4f5f      	ldr	r7, [pc, #380]	; (3000a874 <rxi316_dram_init+0x988>)
3000a6f8:	f04f 0e00 	mov.w	lr, #0
3000a6fc:	f8df 9178 	ldr.w	r9, [pc, #376]	; 3000a878 <rxi316_dram_init+0x98c>
3000a700:	fbb7 f7f3 	udiv	r7, r7, r3
3000a704:	fbb9 f9f3 	udiv	r9, r9, r3
3000a708:	09ff      	lsrs	r7, r7, #7
3000a70a:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a70e:	3701      	adds	r7, #1
3000a710:	f109 0c01 	add.w	ip, r9, #1
3000a714:	f8df 9164 	ldr.w	r9, [pc, #356]	; 3000a87c <rxi316_dram_init+0x990>
3000a718:	023f      	lsls	r7, r7, #8
3000a71a:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000a71e:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000a722:	9709      	str	r7, [sp, #36]	; 0x24
3000a724:	e69f      	b.n	3000a466 <rxi316_dram_init+0x57a>
3000a726:	f8cd c018 	str.w	ip, [sp, #24]
3000a72a:	e5b2      	b.n	3000a292 <rxi316_dram_init+0x3a6>
3000a72c:	38c8      	subs	r0, #200	; 0xc8
3000a72e:	e58f      	b.n	3000a250 <rxi316_dram_init+0x364>
3000a730:	9105      	str	r1, [sp, #20]
3000a732:	e580      	b.n	3000a236 <rxi316_dram_init+0x34a>
3000a734:	ea1a 0f07 	tst.w	sl, r7
3000a738:	fa27 f00b 	lsr.w	r0, r7, fp
3000a73c:	f43f ad55 	beq.w	3000a1ea <rxi316_dram_init+0x2fe>
3000a740:	e552      	b.n	3000a1e8 <rxi316_dram_init+0x2fc>
3000a742:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000a744:	1f50      	subs	r0, r2, #5
3000a746:	00c0      	lsls	r0, r0, #3
3000a748:	68e2      	ldr	r2, [r4, #12]
3000a74a:	6812      	ldr	r2, [r2, #0]
3000a74c:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000a74e:	fab2 f282 	clz	r2, r2
3000a752:	0952      	lsrs	r2, r2, #5
3000a754:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000a758:	4301      	orrs	r1, r0
3000a75a:	e519      	b.n	3000a190 <rxi316_dram_init+0x2a4>
3000a75c:	f1a1 0502 	sub.w	r5, r1, #2
3000a760:	2909      	cmp	r1, #9
3000a762:	fab5 f585 	clz	r5, r5
3000a766:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000a76a:	f000 80a3 	beq.w	3000a8b4 <rxi316_dram_init+0x9c8>
3000a76e:	462a      	mov	r2, r5
3000a770:	e4ed      	b.n	3000a14e <rxi316_dram_init+0x262>
3000a772:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000a774:	46a8      	mov	r8, r5
3000a776:	9d08      	ldr	r5, [sp, #32]
3000a778:	44a8      	add	r8, r5
3000a77a:	e619      	b.n	3000a3b0 <rxi316_dram_init+0x4c4>
3000a77c:	9a08      	ldr	r2, [sp, #32]
3000a77e:	fa22 f10b 	lsr.w	r1, r2, fp
3000a782:	e5a9      	b.n	3000a2d8 <rxi316_dram_init+0x3ec>
3000a784:	2000      	movs	r0, #0
3000a786:	e7df      	b.n	3000a748 <rxi316_dram_init+0x85c>
3000a788:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000a78c:	e610      	b.n	3000a3b0 <rxi316_dram_init+0x4c4>
3000a78e:	f242 7710 	movw	r7, #10000	; 0x2710
3000a792:	f8df 90ec 	ldr.w	r9, [pc, #236]	; 3000a880 <rxi316_dram_init+0x994>
3000a796:	f8df e0e0 	ldr.w	lr, [pc, #224]	; 3000a878 <rxi316_dram_init+0x98c>
3000a79a:	fbb7 fcf3 	udiv	ip, r7, r3
3000a79e:	9f05      	ldr	r7, [sp, #20]
3000a7a0:	4467      	add	r7, ip
3000a7a2:	fbb9 f9f3 	udiv	r9, r9, r3
3000a7a6:	fbbe fef3 	udiv	lr, lr, r3
3000a7aa:	09ff      	lsrs	r7, r7, #7
3000a7ac:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a7b0:	3701      	adds	r7, #1
3000a7b2:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000a7b6:	f109 0c01 	add.w	ip, r9, #1
3000a7ba:	f8df 90c0 	ldr.w	r9, [pc, #192]	; 3000a87c <rxi316_dram_init+0x990>
3000a7be:	023f      	lsls	r7, r7, #8
3000a7c0:	f10e 0e01 	add.w	lr, lr, #1
3000a7c4:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000a7c8:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000a7cc:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000a7d0:	9709      	str	r7, [sp, #36]	; 0x24
3000a7d2:	e648      	b.n	3000a466 <rxi316_dram_init+0x57a>
3000a7d4:	2100      	movs	r1, #0
3000a7d6:	460d      	mov	r5, r1
3000a7d8:	e4c3      	b.n	3000a162 <rxi316_dram_init+0x276>
3000a7da:	3a07      	subs	r2, #7
3000a7dc:	f7ff bbcb 	b.w	30009f76 <rxi316_dram_init+0x8a>
3000a7e0:	686a      	ldr	r2, [r5, #4]
3000a7e2:	f042 0220 	orr.w	r2, r2, #32
3000a7e6:	606a      	str	r2, [r5, #4]
3000a7e8:	e426      	b.n	3000a038 <rxi316_dram_init+0x14c>
3000a7ea:	2204      	movs	r2, #4
3000a7ec:	2102      	movs	r1, #2
3000a7ee:	2500      	movs	r5, #0
3000a7f0:	9201      	str	r2, [sp, #4]
3000a7f2:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000a7f6:	f7ff bba7 	b.w	30009f48 <rxi316_dram_init+0x5c>
3000a7fa:	3a08      	subs	r2, #8
3000a7fc:	f7ff bbbb 	b.w	30009f76 <rxi316_dram_init+0x8a>
3000a800:	46b8      	mov	r8, r7
3000a802:	e61f      	b.n	3000a444 <rxi316_dram_init+0x558>
3000a804:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000a806:	2a04      	cmp	r2, #4
3000a808:	d90c      	bls.n	3000a824 <rxi316_dram_init+0x938>
3000a80a:	2a0b      	cmp	r2, #11
3000a80c:	d86b      	bhi.n	3000a8e6 <rxi316_dram_init+0x9fa>
3000a80e:	1f17      	subs	r7, r2, #4
3000a810:	013f      	lsls	r7, r7, #4
3000a812:	e008      	b.n	3000a826 <rxi316_dram_init+0x93a>
3000a814:	f04f 0802 	mov.w	r8, #2
3000a818:	e614      	b.n	3000a444 <rxi316_dram_init+0x558>
3000a81a:	2903      	cmp	r1, #3
3000a81c:	d005      	beq.n	3000a82a <rxi316_dram_init+0x93e>
3000a81e:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a822:	e49e      	b.n	3000a162 <rxi316_dram_init+0x276>
3000a824:	2710      	movs	r7, #16
3000a826:	68a2      	ldr	r2, [r4, #8]
3000a828:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000a82a:	2a04      	cmp	r2, #4
3000a82c:	d94a      	bls.n	3000a8c4 <rxi316_dram_init+0x9d8>
3000a82e:	1f51      	subs	r1, r2, #5
3000a830:	2903      	cmp	r1, #3
3000a832:	d84f      	bhi.n	3000a8d4 <rxi316_dram_init+0x9e8>
3000a834:	1f15      	subs	r5, r2, #4
3000a836:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a83a:	026d      	lsls	r5, r5, #9
3000a83c:	e491      	b.n	3000a162 <rxi316_dram_init+0x276>
3000a83e:	f1a1 0c02 	sub.w	ip, r1, #2
3000a842:	f1bc 0f01 	cmp.w	ip, #1
3000a846:	d903      	bls.n	3000a850 <rxi316_dram_init+0x964>
3000a848:	f1be 0f00 	cmp.w	lr, #0
3000a84c:	f43f ac7f 	beq.w	3000a14e <rxi316_dram_init+0x262>
3000a850:	f040 0002 	orr.w	r0, r0, #2
3000a854:	e47b      	b.n	3000a14e <rxi316_dram_init+0x262>
3000a856:	bf00      	nop
3000a858:	41100000 	.word	0x41100000
3000a85c:	42008000 	.word	0x42008000
3000a860:	41011000 	.word	0x41011000
3000a864:	44000103 	.word	0x44000103
3000a868:	44000021 	.word	0x44000021
3000a86c:	fff8fffc 	.word	0xfff8fffc
3000a870:	00009b2d 	.word	0x00009b2d
3000a874:	00061a80 	.word	0x00061a80
3000a878:	0bebc200 	.word	0x0bebc200
3000a87c:	003ff000 	.word	0x003ff000
3000a880:	1dcd6500 	.word	0x1dcd6500
3000a884:	4f1d      	ldr	r7, [pc, #116]	; (3000a8fc <rxi316_dram_init+0xa10>)
3000a886:	f04f 0e00 	mov.w	lr, #0
3000a88a:	fbb7 f9f3 	udiv	r9, r7, r3
3000a88e:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a892:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000a896:	f109 0701 	add.w	r7, r9, #1
3000a89a:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000a900 <rxi316_dram_init+0xa14>
3000a89e:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000a8a2:	e5e0      	b.n	3000a466 <rxi316_dram_init+0x57a>
3000a8a4:	f105 0802 	add.w	r8, r5, #2
3000a8a8:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000a8aa:	eba8 0805 	sub.w	r8, r8, r5
3000a8ae:	9d08      	ldr	r5, [sp, #32]
3000a8b0:	44a8      	add	r8, r5
3000a8b2:	e57d      	b.n	3000a3b0 <rxi316_dram_init+0x4c4>
3000a8b4:	2204      	movs	r2, #4
3000a8b6:	e6ee      	b.n	3000a696 <rxi316_dram_init+0x7aa>
3000a8b8:	686a      	ldr	r2, [r5, #4]
3000a8ba:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000a8be:	606a      	str	r2, [r5, #4]
3000a8c0:	f7ff bbba 	b.w	3000a038 <rxi316_dram_init+0x14c>
3000a8c4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a8c8:	f44f 7500 	mov.w	r5, #512	; 0x200
3000a8cc:	e449      	b.n	3000a162 <rxi316_dram_init+0x276>
3000a8ce:	f04f 0802 	mov.w	r8, #2
3000a8d2:	e5b1      	b.n	3000a438 <rxi316_dram_init+0x54c>
3000a8d4:	f1a2 0109 	sub.w	r1, r2, #9
3000a8d8:	2905      	cmp	r1, #5
3000a8da:	d80b      	bhi.n	3000a8f4 <rxi316_dram_init+0xa08>
3000a8dc:	0855      	lsrs	r5, r2, #1
3000a8de:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a8e2:	026d      	lsls	r5, r5, #9
3000a8e4:	e43d      	b.n	3000a162 <rxi316_dram_init+0x276>
3000a8e6:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000a8e8:	f040 0004 	orr.w	r0, r0, #4
3000a8ec:	f1a2 070c 	sub.w	r7, r2, #12
3000a8f0:	013f      	lsls	r7, r7, #4
3000a8f2:	e798      	b.n	3000a826 <rxi316_dram_init+0x93a>
3000a8f4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a8f8:	2500      	movs	r5, #0
3000a8fa:	e432      	b.n	3000a162 <rxi316_dram_init+0x276>
3000a8fc:	0bebc200 	.word	0x0bebc200
3000a900:	003ff000 	.word	0x003ff000

3000a904 <rxi316_ftend_init>:
3000a904:	2300      	movs	r3, #0
3000a906:	b510      	push	{r4, lr}
3000a908:	4c0c      	ldr	r4, [pc, #48]	; (3000a93c <rxi316_ftend_init+0x38>)
3000a90a:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000a90e:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000a912:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000a916:	f7fe fe5d 	bl	300095d4 <ddr_init_index>
3000a91a:	4a09      	ldr	r2, [pc, #36]	; (3000a940 <rxi316_ftend_init+0x3c>)
3000a91c:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000a920:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000a924:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000a928:	68cb      	ldr	r3, [r1, #12]
3000a92a:	689b      	ldr	r3, [r3, #8]
3000a92c:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000a92e:	3301      	adds	r3, #1
3000a930:	b2db      	uxtb	r3, r3
3000a932:	4313      	orrs	r3, r2
3000a934:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000a938:	bd10      	pop	{r4, pc}
3000a93a:	bf00      	nop
3000a93c:	41101000 	.word	0x41101000
3000a940:	3000ec50 	.word	0x3000ec50

3000a944 <rxi316_DynSre_init>:
3000a944:	b538      	push	{r3, r4, r5, lr}
3000a946:	4b22      	ldr	r3, [pc, #136]	; (3000a9d0 <rxi316_DynSre_init+0x8c>)
3000a948:	4604      	mov	r4, r0
3000a94a:	460d      	mov	r5, r1
3000a94c:	4798      	blx	r3
3000a94e:	2802      	cmp	r0, #2
3000a950:	d937      	bls.n	3000a9c2 <rxi316_DynSre_init+0x7e>
3000a952:	4b20      	ldr	r3, [pc, #128]	; (3000a9d4 <rxi316_DynSre_init+0x90>)
3000a954:	691b      	ldr	r3, [r3, #16]
3000a956:	f3c3 230f 	ubfx	r3, r3, #8, #16
3000a95a:	3b46      	subs	r3, #70	; 0x46
3000a95c:	42a3      	cmp	r3, r4
3000a95e:	d915      	bls.n	3000a98c <rxi316_DynSre_init+0x48>
3000a960:	2d01      	cmp	r5, #1
3000a962:	d01a      	beq.n	3000a99a <rxi316_DynSre_init+0x56>
3000a964:	4a1b      	ldr	r2, [pc, #108]	; (3000a9d4 <rxi316_DynSre_init+0x90>)
3000a966:	6893      	ldr	r3, [r2, #8]
3000a968:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000a96c:	6093      	str	r3, [r2, #8]
3000a96e:	4b19      	ldr	r3, [pc, #100]	; (3000a9d4 <rxi316_DynSre_init+0x90>)
3000a970:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000a974:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a978:	625a      	str	r2, [r3, #36]	; 0x24
3000a97a:	629a      	str	r2, [r3, #40]	; 0x28
3000a97c:	69da      	ldr	r2, [r3, #28]
3000a97e:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000a982:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000a986:	61da      	str	r2, [r3, #28]
3000a988:	6019      	str	r1, [r3, #0]
3000a98a:	bd38      	pop	{r3, r4, r5, pc}
3000a98c:	f240 411e 	movw	r1, #1054	; 0x41e
3000a990:	4811      	ldr	r0, [pc, #68]	; (3000a9d8 <rxi316_DynSre_init+0x94>)
3000a992:	f001 f8ed 	bl	3000bb70 <__io_assert_failed_veneer>
3000a996:	2d01      	cmp	r5, #1
3000a998:	d1e4      	bne.n	3000a964 <rxi316_DynSre_init+0x20>
3000a99a:	4b0e      	ldr	r3, [pc, #56]	; (3000a9d4 <rxi316_DynSre_init+0x90>)
3000a99c:	f64f 70fe 	movw	r0, #65534	; 0xfffe
3000a9a0:	4a0e      	ldr	r2, [pc, #56]	; (3000a9dc <rxi316_DynSre_init+0x98>)
3000a9a2:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000a9a6:	4004      	ands	r4, r0
3000a9a8:	400a      	ands	r2, r1
3000a9aa:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a9ae:	f8d3 0100 	ldr.w	r0, [r3, #256]	; 0x100
3000a9b2:	4304      	orrs	r4, r0
3000a9b4:	f8c3 4100 	str.w	r4, [r3, #256]	; 0x100
3000a9b8:	689a      	ldr	r2, [r3, #8]
3000a9ba:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a9be:	609a      	str	r2, [r3, #8]
3000a9c0:	e7d5      	b.n	3000a96e <rxi316_DynSre_init+0x2a>
3000a9c2:	4b07      	ldr	r3, [pc, #28]	; (3000a9e0 <rxi316_DynSre_init+0x9c>)
3000a9c4:	4798      	blx	r3
3000a9c6:	2801      	cmp	r0, #1
3000a9c8:	bf08      	it	eq
3000a9ca:	2400      	moveq	r4, #0
3000a9cc:	e7c8      	b.n	3000a960 <rxi316_DynSre_init+0x1c>
3000a9ce:	bf00      	nop
3000a9d0:	300092c5 	.word	0x300092c5
3000a9d4:	41100000 	.word	0x41100000
3000a9d8:	3000d1b4 	.word	0x3000d1b4
3000a9dc:	ffff0000 	.word	0xffff0000
3000a9e0:	30007691 	.word	0x30007691

3000a9e4 <ddr_init>:
3000a9e4:	b500      	push	{lr}
3000a9e6:	b083      	sub	sp, #12
3000a9e8:	f7fe fdf4 	bl	300095d4 <ddr_init_index>
3000a9ec:	4b06      	ldr	r3, [pc, #24]	; (3000aa08 <ddr_init+0x24>)
3000a9ee:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000a9f2:	9001      	str	r0, [sp, #4]
3000a9f4:	f7fe fe18 	bl	30009628 <rxi316_perf_tune>
3000a9f8:	9801      	ldr	r0, [sp, #4]
3000a9fa:	f7ff fa77 	bl	30009eec <rxi316_dram_init>
3000a9fe:	b003      	add	sp, #12
3000aa00:	f85d eb04 	ldr.w	lr, [sp], #4
3000aa04:	f7ff bf7e 	b.w	3000a904 <rxi316_ftend_init>
3000aa08:	3000ec50 	.word	0x3000ec50

3000aa0c <SWR_Calib_DCore>:
3000aa0c:	4b0f      	ldr	r3, [pc, #60]	; (3000aa4c <SWR_Calib_DCore+0x40>)
3000aa0e:	f240 3231 	movw	r2, #817	; 0x331
3000aa12:	490f      	ldr	r1, [pc, #60]	; (3000aa50 <SWR_Calib_DCore+0x44>)
3000aa14:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000aa16:	4001      	ands	r1, r0
3000aa18:	430a      	orrs	r2, r1
3000aa1a:	651a      	str	r2, [r3, #80]	; 0x50
3000aa1c:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000aa1e:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000aa22:	641a      	str	r2, [r3, #64]	; 0x40
3000aa24:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000aa28:	f042 0201 	orr.w	r2, r2, #1
3000aa2c:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000aa30:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000aa34:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000aa38:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000aa3c:	685a      	ldr	r2, [r3, #4]
3000aa3e:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000aa42:	f042 0204 	orr.w	r2, r2, #4
3000aa46:	605a      	str	r2, [r3, #4]
3000aa48:	4770      	bx	lr
3000aa4a:	bf00      	nop
3000aa4c:	42008100 	.word	0x42008100
3000aa50:	fffffccc 	.word	0xfffffccc

3000aa54 <SWR_Calib_MEM>:
3000aa54:	4b51      	ldr	r3, [pc, #324]	; (3000ab9c <SWR_Calib_MEM+0x148>)
3000aa56:	f240 3231 	movw	r2, #817	; 0x331
3000aa5a:	4951      	ldr	r1, [pc, #324]	; (3000aba0 <SWR_Calib_MEM+0x14c>)
3000aa5c:	f04f 0c00 	mov.w	ip, #0
3000aa60:	b510      	push	{r4, lr}
3000aa62:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000aa66:	b082      	sub	sp, #8
3000aa68:	4001      	ands	r1, r0
3000aa6a:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000aa6e:	430a      	orrs	r2, r1
3000aa70:	f10d 0107 	add.w	r1, sp, #7
3000aa74:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000aa78:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000aa7c:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000aa80:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000aa84:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000aa88:	f042 0201 	orr.w	r2, r2, #1
3000aa8c:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000aa90:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000aa94:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000aa98:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000aa9c:	f88d c007 	strb.w	ip, [sp, #7]
3000aaa0:	f7fd fb8a 	bl	300081b8 <OTP_Read8>
3000aaa4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000aaa8:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000aaac:	2af0      	cmp	r2, #240	; 0xf0
3000aaae:	d156      	bne.n	3000ab5e <SWR_Calib_MEM+0x10a>
3000aab0:	f003 030f 	and.w	r3, r3, #15
3000aab4:	2b0f      	cmp	r3, #15
3000aab6:	d04e      	beq.n	3000ab56 <SWR_Calib_MEM+0x102>
3000aab8:	2302      	movs	r3, #2
3000aaba:	4c3a      	ldr	r4, [pc, #232]	; (3000aba4 <SWR_Calib_MEM+0x150>)
3000aabc:	f10d 0106 	add.w	r1, sp, #6
3000aac0:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000aac4:	7023      	strb	r3, [r4, #0]
3000aac6:	f7fd fb77 	bl	300081b8 <OTP_Read8>
3000aaca:	7822      	ldrb	r2, [r4, #0]
3000aacc:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000aad0:	2a03      	cmp	r2, #3
3000aad2:	d05a      	beq.n	3000ab8a <SWR_Calib_MEM+0x136>
3000aad4:	2a02      	cmp	r2, #2
3000aad6:	bf0c      	ite	eq
3000aad8:	f003 030f 	andeq.w	r3, r3, #15
3000aadc:	2300      	movne	r3, #0
3000aade:	492f      	ldr	r1, [pc, #188]	; (3000ab9c <SWR_Calib_MEM+0x148>)
3000aae0:	4831      	ldr	r0, [pc, #196]	; (3000aba8 <SWR_Calib_MEM+0x154>)
3000aae2:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000aae6:	f022 020f 	bic.w	r2, r2, #15
3000aaea:	4313      	orrs	r3, r2
3000aaec:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000aaf0:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000aaf2:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000aaf6:	2b07      	cmp	r3, #7
3000aaf8:	d03a      	beq.n	3000ab70 <SWR_Calib_MEM+0x11c>
3000aafa:	2300      	movs	r3, #0
3000aafc:	f10d 0107 	add.w	r1, sp, #7
3000ab00:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000ab04:	f88d 3007 	strb.w	r3, [sp, #7]
3000ab08:	f7fd fb56 	bl	300081b8 <OTP_Read8>
3000ab0c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ab10:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000ab14:	2af0      	cmp	r2, #240	; 0xf0
3000ab16:	d124      	bne.n	3000ab62 <SWR_Calib_MEM+0x10e>
3000ab18:	f003 030f 	and.w	r3, r3, #15
3000ab1c:	2b0f      	cmp	r3, #15
3000ab1e:	d022      	beq.n	3000ab66 <SWR_Calib_MEM+0x112>
3000ab20:	2302      	movs	r3, #2
3000ab22:	4c22      	ldr	r4, [pc, #136]	; (3000abac <SWR_Calib_MEM+0x158>)
3000ab24:	f10d 0107 	add.w	r1, sp, #7
3000ab28:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000ab2c:	7023      	strb	r3, [r4, #0]
3000ab2e:	f7fd fb43 	bl	300081b8 <OTP_Read8>
3000ab32:	7822      	ldrb	r2, [r4, #0]
3000ab34:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ab38:	2a03      	cmp	r2, #3
3000ab3a:	d028      	beq.n	3000ab8e <SWR_Calib_MEM+0x13a>
3000ab3c:	2a02      	cmp	r2, #2
3000ab3e:	d029      	beq.n	3000ab94 <SWR_Calib_MEM+0x140>
3000ab40:	2200      	movs	r2, #0
3000ab42:	4916      	ldr	r1, [pc, #88]	; (3000ab9c <SWR_Calib_MEM+0x148>)
3000ab44:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000ab48:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ab4c:	4313      	orrs	r3, r2
3000ab4e:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000ab52:	b002      	add	sp, #8
3000ab54:	bd10      	pop	{r4, pc}
3000ab56:	4b13      	ldr	r3, [pc, #76]	; (3000aba4 <SWR_Calib_MEM+0x150>)
3000ab58:	2201      	movs	r2, #1
3000ab5a:	701a      	strb	r2, [r3, #0]
3000ab5c:	e7cd      	b.n	3000aafa <SWR_Calib_MEM+0xa6>
3000ab5e:	2303      	movs	r3, #3
3000ab60:	e7ab      	b.n	3000aaba <SWR_Calib_MEM+0x66>
3000ab62:	2303      	movs	r3, #3
3000ab64:	e7dd      	b.n	3000ab22 <SWR_Calib_MEM+0xce>
3000ab66:	4b11      	ldr	r3, [pc, #68]	; (3000abac <SWR_Calib_MEM+0x158>)
3000ab68:	2201      	movs	r2, #1
3000ab6a:	701a      	strb	r2, [r3, #0]
3000ab6c:	b002      	add	sp, #8
3000ab6e:	bd10      	pop	{r4, pc}
3000ab70:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000ab72:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000ab76:	d1c0      	bne.n	3000aafa <SWR_Calib_MEM+0xa6>
3000ab78:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000ab7c:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000ab80:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000ab84:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000ab88:	e7b7      	b.n	3000aafa <SWR_Calib_MEM+0xa6>
3000ab8a:	091b      	lsrs	r3, r3, #4
3000ab8c:	e7a7      	b.n	3000aade <SWR_Calib_MEM+0x8a>
3000ab8e:	091b      	lsrs	r3, r3, #4
3000ab90:	059a      	lsls	r2, r3, #22
3000ab92:	e7d6      	b.n	3000ab42 <SWR_Calib_MEM+0xee>
3000ab94:	f003 030f 	and.w	r3, r3, #15
3000ab98:	059a      	lsls	r2, r3, #22
3000ab9a:	e7d2      	b.n	3000ab42 <SWR_Calib_MEM+0xee>
3000ab9c:	42008800 	.word	0x42008800
3000aba0:	fffffccc 	.word	0xfffffccc
3000aba4:	3000edeb 	.word	0x3000edeb
3000aba8:	42008100 	.word	0x42008100
3000abac:	3000edea 	.word	0x3000edea

3000abb0 <SWR_Calib_AUD>:
3000abb0:	4b48      	ldr	r3, [pc, #288]	; (3000acd4 <SWR_Calib_AUD+0x124>)
3000abb2:	f240 3231 	movw	r2, #817	; 0x331
3000abb6:	4948      	ldr	r1, [pc, #288]	; (3000acd8 <SWR_Calib_AUD+0x128>)
3000abb8:	f04f 0c00 	mov.w	ip, #0
3000abbc:	b510      	push	{r4, lr}
3000abbe:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000abc2:	b082      	sub	sp, #8
3000abc4:	4001      	ands	r1, r0
3000abc6:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000abca:	430a      	orrs	r2, r1
3000abcc:	f10d 0107 	add.w	r1, sp, #7
3000abd0:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000abd4:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000abd8:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000abdc:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000abe0:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000abe4:	f042 0201 	orr.w	r2, r2, #1
3000abe8:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000abec:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000abf0:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000abf4:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000abf8:	f88d c007 	strb.w	ip, [sp, #7]
3000abfc:	f7fd fadc 	bl	300081b8 <OTP_Read8>
3000ac00:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ac04:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000ac08:	2af0      	cmp	r2, #240	; 0xf0
3000ac0a:	d150      	bne.n	3000acae <SWR_Calib_AUD+0xfe>
3000ac0c:	f003 030f 	and.w	r3, r3, #15
3000ac10:	2b0f      	cmp	r3, #15
3000ac12:	d048      	beq.n	3000aca6 <SWR_Calib_AUD+0xf6>
3000ac14:	2302      	movs	r3, #2
3000ac16:	4c31      	ldr	r4, [pc, #196]	; (3000acdc <SWR_Calib_AUD+0x12c>)
3000ac18:	f10d 0106 	add.w	r1, sp, #6
3000ac1c:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000ac20:	7023      	strb	r3, [r4, #0]
3000ac22:	f7fd fac9 	bl	300081b8 <OTP_Read8>
3000ac26:	7822      	ldrb	r2, [r4, #0]
3000ac28:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000ac2c:	2a03      	cmp	r2, #3
3000ac2e:	d047      	beq.n	3000acc0 <SWR_Calib_AUD+0x110>
3000ac30:	2a02      	cmp	r2, #2
3000ac32:	bf0c      	ite	eq
3000ac34:	f003 030f 	andeq.w	r3, r3, #15
3000ac38:	2300      	movne	r3, #0
3000ac3a:	4926      	ldr	r1, [pc, #152]	; (3000acd4 <SWR_Calib_AUD+0x124>)
3000ac3c:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000ac40:	f022 020f 	bic.w	r2, r2, #15
3000ac44:	4313      	orrs	r3, r2
3000ac46:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000ac4a:	2300      	movs	r3, #0
3000ac4c:	f10d 0107 	add.w	r1, sp, #7
3000ac50:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000ac54:	f88d 3007 	strb.w	r3, [sp, #7]
3000ac58:	f7fd faae 	bl	300081b8 <OTP_Read8>
3000ac5c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ac60:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000ac64:	2af0      	cmp	r2, #240	; 0xf0
3000ac66:	d124      	bne.n	3000acb2 <SWR_Calib_AUD+0x102>
3000ac68:	f003 030f 	and.w	r3, r3, #15
3000ac6c:	2b0f      	cmp	r3, #15
3000ac6e:	d022      	beq.n	3000acb6 <SWR_Calib_AUD+0x106>
3000ac70:	2302      	movs	r3, #2
3000ac72:	4c1b      	ldr	r4, [pc, #108]	; (3000ace0 <SWR_Calib_AUD+0x130>)
3000ac74:	f10d 0107 	add.w	r1, sp, #7
3000ac78:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000ac7c:	7023      	strb	r3, [r4, #0]
3000ac7e:	f7fd fa9b 	bl	300081b8 <OTP_Read8>
3000ac82:	7822      	ldrb	r2, [r4, #0]
3000ac84:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ac88:	2a03      	cmp	r2, #3
3000ac8a:	d01b      	beq.n	3000acc4 <SWR_Calib_AUD+0x114>
3000ac8c:	2a02      	cmp	r2, #2
3000ac8e:	d01c      	beq.n	3000acca <SWR_Calib_AUD+0x11a>
3000ac90:	2200      	movs	r2, #0
3000ac92:	4910      	ldr	r1, [pc, #64]	; (3000acd4 <SWR_Calib_AUD+0x124>)
3000ac94:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000ac98:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ac9c:	4313      	orrs	r3, r2
3000ac9e:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000aca2:	b002      	add	sp, #8
3000aca4:	bd10      	pop	{r4, pc}
3000aca6:	4b0d      	ldr	r3, [pc, #52]	; (3000acdc <SWR_Calib_AUD+0x12c>)
3000aca8:	2201      	movs	r2, #1
3000acaa:	701a      	strb	r2, [r3, #0]
3000acac:	e7cd      	b.n	3000ac4a <SWR_Calib_AUD+0x9a>
3000acae:	2303      	movs	r3, #3
3000acb0:	e7b1      	b.n	3000ac16 <SWR_Calib_AUD+0x66>
3000acb2:	2303      	movs	r3, #3
3000acb4:	e7dd      	b.n	3000ac72 <SWR_Calib_AUD+0xc2>
3000acb6:	4b0a      	ldr	r3, [pc, #40]	; (3000ace0 <SWR_Calib_AUD+0x130>)
3000acb8:	2201      	movs	r2, #1
3000acba:	701a      	strb	r2, [r3, #0]
3000acbc:	b002      	add	sp, #8
3000acbe:	bd10      	pop	{r4, pc}
3000acc0:	091b      	lsrs	r3, r3, #4
3000acc2:	e7ba      	b.n	3000ac3a <SWR_Calib_AUD+0x8a>
3000acc4:	091b      	lsrs	r3, r3, #4
3000acc6:	059a      	lsls	r2, r3, #22
3000acc8:	e7e3      	b.n	3000ac92 <SWR_Calib_AUD+0xe2>
3000acca:	f003 030f 	and.w	r3, r3, #15
3000acce:	059a      	lsls	r2, r3, #22
3000acd0:	e7df      	b.n	3000ac92 <SWR_Calib_AUD+0xe2>
3000acd2:	bf00      	nop
3000acd4:	42008800 	.word	0x42008800
3000acd8:	fffffccc 	.word	0xfffffccc
3000acdc:	3000ede9 	.word	0x3000ede9
3000ace0:	3000ede8 	.word	0x3000ede8

3000ace4 <SWR_MEM>:
3000ace4:	2801      	cmp	r0, #1
3000ace6:	d00e      	beq.n	3000ad06 <SWR_MEM+0x22>
3000ace8:	4b31      	ldr	r3, [pc, #196]	; (3000adb0 <SWR_MEM+0xcc>)
3000acea:	2000      	movs	r0, #0
3000acec:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000acf0:	f022 0201 	bic.w	r2, r2, #1
3000acf4:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000acf8:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000acfc:	f022 0202 	bic.w	r2, r2, #2
3000ad00:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000ad04:	4770      	bx	lr
3000ad06:	b510      	push	{r4, lr}
3000ad08:	4c29      	ldr	r4, [pc, #164]	; (3000adb0 <SWR_MEM+0xcc>)
3000ad0a:	4a2a      	ldr	r2, [pc, #168]	; (3000adb4 <SWR_MEM+0xd0>)
3000ad0c:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000ad10:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000ad14:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000ad18:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000ad1c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ad20:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000ad24:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ad28:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000ad2c:	f023 0303 	bic.w	r3, r3, #3
3000ad30:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ad34:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ad38:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ad3c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ad40:	4790      	blx	r2
3000ad42:	2803      	cmp	r0, #3
3000ad44:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000ad48:	d026      	beq.n	3000ad98 <SWR_MEM+0xb4>
3000ad4a:	f043 0303 	orr.w	r3, r3, #3
3000ad4e:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ad52:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ad56:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000ad5a:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ad5e:	4c14      	ldr	r4, [pc, #80]	; (3000adb0 <SWR_MEM+0xcc>)
3000ad60:	4b15      	ldr	r3, [pc, #84]	; (3000adb8 <SWR_MEM+0xd4>)
3000ad62:	4798      	blx	r3
3000ad64:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000ad68:	4a14      	ldr	r2, [pc, #80]	; (3000adbc <SWR_MEM+0xd8>)
3000ad6a:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000ad6e:	f043 0302 	orr.w	r3, r3, #2
3000ad72:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000ad76:	4790      	blx	r2
3000ad78:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ad7c:	4b10      	ldr	r3, [pc, #64]	; (3000adc0 <SWR_MEM+0xdc>)
3000ad7e:	f042 0201 	orr.w	r2, r2, #1
3000ad82:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000ad86:	e001      	b.n	3000ad8c <SWR_MEM+0xa8>
3000ad88:	3b01      	subs	r3, #1
3000ad8a:	d00e      	beq.n	3000adaa <SWR_MEM+0xc6>
3000ad8c:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ad90:	03d2      	lsls	r2, r2, #15
3000ad92:	d5f9      	bpl.n	3000ad88 <SWR_MEM+0xa4>
3000ad94:	2000      	movs	r0, #0
3000ad96:	bd10      	pop	{r4, pc}
3000ad98:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ad9c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ada0:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000ada4:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ada8:	e7d9      	b.n	3000ad5e <SWR_MEM+0x7a>
3000adaa:	2001      	movs	r0, #1
3000adac:	bd10      	pop	{r4, pc}
3000adae:	bf00      	nop
3000adb0:	42008800 	.word	0x42008800
3000adb4:	300074c1 	.word	0x300074c1
3000adb8:	3000aa55 	.word	0x3000aa55
3000adbc:	00009b2d 	.word	0x00009b2d
3000adc0:	000186a0 	.word	0x000186a0

3000adc4 <SWR_MEM_Manual>:
3000adc4:	4a06      	ldr	r2, [pc, #24]	; (3000ade0 <SWR_MEM_Manual+0x1c>)
3000adc6:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000adca:	b120      	cbz	r0, 3000add6 <SWR_MEM_Manual+0x12>
3000adcc:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000add0:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000add4:	4770      	bx	lr
3000add6:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000adda:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000adde:	4770      	bx	lr
3000ade0:	42008800 	.word	0x42008800

3000ade4 <SWR_MEM_Mode_Set>:
3000ade4:	2801      	cmp	r0, #1
3000ade6:	b538      	push	{r3, r4, r5, lr}
3000ade8:	d807      	bhi.n	3000adfa <SWR_MEM_Mode_Set+0x16>
3000adea:	d10a      	bne.n	3000ae02 <SWR_MEM_Mode_Set+0x1e>
3000adec:	4c22      	ldr	r4, [pc, #136]	; (3000ae78 <SWR_MEM_Mode_Set+0x94>)
3000adee:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000adf2:	035b      	lsls	r3, r3, #13
3000adf4:	d51b      	bpl.n	3000ae2e <SWR_MEM_Mode_Set+0x4a>
3000adf6:	2000      	movs	r0, #0
3000adf8:	bd38      	pop	{r3, r4, r5, pc}
3000adfa:	217e      	movs	r1, #126	; 0x7e
3000adfc:	481f      	ldr	r0, [pc, #124]	; (3000ae7c <SWR_MEM_Mode_Set+0x98>)
3000adfe:	f000 feb7 	bl	3000bb70 <__io_assert_failed_veneer>
3000ae02:	491d      	ldr	r1, [pc, #116]	; (3000ae78 <SWR_MEM_Mode_Set+0x94>)
3000ae04:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000ae08:	0358      	lsls	r0, r3, #13
3000ae0a:	d5f4      	bpl.n	3000adf6 <SWR_MEM_Mode_Set+0x12>
3000ae0c:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000ae10:	f242 7310 	movw	r3, #10000	; 0x2710
3000ae14:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000ae18:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000ae1c:	e001      	b.n	3000ae22 <SWR_MEM_Mode_Set+0x3e>
3000ae1e:	3b01      	subs	r3, #1
3000ae20:	d027      	beq.n	3000ae72 <SWR_MEM_Mode_Set+0x8e>
3000ae22:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000ae26:	0352      	lsls	r2, r2, #13
3000ae28:	d4f9      	bmi.n	3000ae1e <SWR_MEM_Mode_Set+0x3a>
3000ae2a:	2000      	movs	r0, #0
3000ae2c:	e7e4      	b.n	3000adf8 <SWR_MEM_Mode_Set+0x14>
3000ae2e:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000ae32:	2064      	movs	r0, #100	; 0x64
3000ae34:	4d12      	ldr	r5, [pc, #72]	; (3000ae80 <SWR_MEM_Mode_Set+0x9c>)
3000ae36:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000ae3a:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000ae3e:	47a8      	blx	r5
3000ae40:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ae44:	2064      	movs	r0, #100	; 0x64
3000ae46:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000ae4a:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ae4e:	47a8      	blx	r5
3000ae50:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000ae54:	f242 7310 	movw	r3, #10000	; 0x2710
3000ae58:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000ae5c:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000ae60:	e001      	b.n	3000ae66 <SWR_MEM_Mode_Set+0x82>
3000ae62:	3b01      	subs	r3, #1
3000ae64:	d005      	beq.n	3000ae72 <SWR_MEM_Mode_Set+0x8e>
3000ae66:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ae6a:	0355      	lsls	r5, r2, #13
3000ae6c:	d5f9      	bpl.n	3000ae62 <SWR_MEM_Mode_Set+0x7e>
3000ae6e:	2000      	movs	r0, #0
3000ae70:	e7c2      	b.n	3000adf8 <SWR_MEM_Mode_Set+0x14>
3000ae72:	2001      	movs	r0, #1
3000ae74:	bd38      	pop	{r3, r4, r5, pc}
3000ae76:	bf00      	nop
3000ae78:	42008800 	.word	0x42008800
3000ae7c:	3000d1c8 	.word	0x3000d1c8
3000ae80:	00009b2d 	.word	0x00009b2d

3000ae84 <SWR_AUDIO>:
3000ae84:	2801      	cmp	r0, #1
3000ae86:	d00e      	beq.n	3000aea6 <SWR_AUDIO+0x22>
3000ae88:	4b1e      	ldr	r3, [pc, #120]	; (3000af04 <SWR_AUDIO+0x80>)
3000ae8a:	2000      	movs	r0, #0
3000ae8c:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ae90:	f022 0201 	bic.w	r2, r2, #1
3000ae94:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000ae98:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ae9c:	f022 0202 	bic.w	r2, r2, #2
3000aea0:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000aea4:	4770      	bx	lr
3000aea6:	b510      	push	{r4, lr}
3000aea8:	4c16      	ldr	r4, [pc, #88]	; (3000af04 <SWR_AUDIO+0x80>)
3000aeaa:	4b17      	ldr	r3, [pc, #92]	; (3000af08 <SWR_AUDIO+0x84>)
3000aeac:	4798      	blx	r3
3000aeae:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000aeb2:	4a16      	ldr	r2, [pc, #88]	; (3000af0c <SWR_AUDIO+0x88>)
3000aeb4:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000aeb8:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000aebc:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000aec0:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000aec4:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000aec8:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000aecc:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000aed0:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000aed4:	f043 0302 	orr.w	r3, r3, #2
3000aed8:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000aedc:	4790      	blx	r2
3000aede:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000aee2:	4b0b      	ldr	r3, [pc, #44]	; (3000af10 <SWR_AUDIO+0x8c>)
3000aee4:	f042 0201 	orr.w	r2, r2, #1
3000aee8:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000aeec:	e001      	b.n	3000aef2 <SWR_AUDIO+0x6e>
3000aeee:	3b01      	subs	r3, #1
3000aef0:	d005      	beq.n	3000aefe <SWR_AUDIO+0x7a>
3000aef2:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000aef6:	03d2      	lsls	r2, r2, #15
3000aef8:	d5f9      	bpl.n	3000aeee <SWR_AUDIO+0x6a>
3000aefa:	2000      	movs	r0, #0
3000aefc:	bd10      	pop	{r4, pc}
3000aefe:	2001      	movs	r0, #1
3000af00:	bd10      	pop	{r4, pc}
3000af02:	bf00      	nop
3000af04:	42008800 	.word	0x42008800
3000af08:	3000abb1 	.word	0x3000abb1
3000af0c:	00009b2d 	.word	0x00009b2d
3000af10:	000186a0 	.word	0x000186a0

3000af14 <SWR_AUDIO_Manual>:
3000af14:	4a06      	ldr	r2, [pc, #24]	; (3000af30 <SWR_AUDIO_Manual+0x1c>)
3000af16:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000af1a:	b120      	cbz	r0, 3000af26 <SWR_AUDIO_Manual+0x12>
3000af1c:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000af20:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000af24:	4770      	bx	lr
3000af26:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000af2a:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000af2e:	4770      	bx	lr
3000af30:	42008800 	.word	0x42008800

3000af34 <SWR_BST_MODE_Set>:
3000af34:	4a06      	ldr	r2, [pc, #24]	; (3000af50 <SWR_BST_MODE_Set+0x1c>)
3000af36:	6813      	ldr	r3, [r2, #0]
3000af38:	b118      	cbz	r0, 3000af42 <SWR_BST_MODE_Set+0xe>
3000af3a:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000af3e:	6013      	str	r3, [r2, #0]
3000af40:	4770      	bx	lr
3000af42:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000af46:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000af4a:	6013      	str	r3, [r2, #0]
3000af4c:	4770      	bx	lr
3000af4e:	bf00      	nop
3000af50:	42008100 	.word	0x42008100

3000af54 <crash_dump_memory>:
3000af54:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000af58:	460e      	mov	r6, r1
3000af5a:	4680      	mov	r8, r0
3000af5c:	4b21      	ldr	r3, [pc, #132]	; (3000afe4 <crash_dump_memory+0x90>)
3000af5e:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000af62:	4a21      	ldr	r2, [pc, #132]	; (3000afe8 <crash_dump_memory+0x94>)
3000af64:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000af68:	4644      	mov	r4, r8
3000af6a:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000af6e:	4b1f      	ldr	r3, [pc, #124]	; (3000afec <crash_dump_memory+0x98>)
3000af70:	481f      	ldr	r0, [pc, #124]	; (3000aff0 <crash_dump_memory+0x9c>)
3000af72:	bf88      	it	hi
3000af74:	2680      	movhi	r6, #128	; 0x80
3000af76:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000af7a:	4f1e      	ldr	r7, [pc, #120]	; (3000aff4 <crash_dump_memory+0xa0>)
3000af7c:	bf14      	ite	ne
3000af7e:	4611      	movne	r1, r2
3000af80:	4619      	moveq	r1, r3
3000af82:	f000 fdb5 	bl	3000baf0 <__DiagPrintf_veneer>
3000af86:	4632      	mov	r2, r6
3000af88:	4641      	mov	r1, r8
3000af8a:	481b      	ldr	r0, [pc, #108]	; (3000aff8 <crash_dump_memory+0xa4>)
3000af8c:	f000 fdb0 	bl	3000baf0 <__DiagPrintf_veneer>
3000af90:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000b000 <crash_dump_memory+0xac>
3000af94:	2300      	movs	r3, #0
3000af96:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000b004 <crash_dump_memory+0xb0>
3000af9a:	e007      	b.n	3000afac <crash_dump_memory+0x58>
3000af9c:	6821      	ldr	r1, [r4, #0]
3000af9e:	4638      	mov	r0, r7
3000afa0:	f000 fda6 	bl	3000baf0 <__DiagPrintf_veneer>
3000afa4:	42ae      	cmp	r6, r5
3000afa6:	d00f      	beq.n	3000afc8 <crash_dump_memory+0x74>
3000afa8:	3404      	adds	r4, #4
3000afaa:	462b      	mov	r3, r5
3000afac:	1c5d      	adds	r5, r3, #1
3000afae:	b183      	cbz	r3, 3000afd2 <crash_dump_memory+0x7e>
3000afb0:	079b      	lsls	r3, r3, #30
3000afb2:	d1f3      	bne.n	3000af9c <crash_dump_memory+0x48>
3000afb4:	4621      	mov	r1, r4
3000afb6:	4648      	mov	r0, r9
3000afb8:	f000 fd9a 	bl	3000baf0 <__DiagPrintf_veneer>
3000afbc:	6821      	ldr	r1, [r4, #0]
3000afbe:	4638      	mov	r0, r7
3000afc0:	f000 fd96 	bl	3000baf0 <__DiagPrintf_veneer>
3000afc4:	42ae      	cmp	r6, r5
3000afc6:	d1ef      	bne.n	3000afa8 <crash_dump_memory+0x54>
3000afc8:	480c      	ldr	r0, [pc, #48]	; (3000affc <crash_dump_memory+0xa8>)
3000afca:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000afce:	f000 bd8f 	b.w	3000baf0 <__DiagPrintf_veneer>
3000afd2:	4641      	mov	r1, r8
3000afd4:	4650      	mov	r0, sl
3000afd6:	f000 fd8b 	bl	3000baf0 <__DiagPrintf_veneer>
3000afda:	6821      	ldr	r1, [r4, #0]
3000afdc:	4638      	mov	r0, r7
3000afde:	f000 fd87 	bl	3000baf0 <__DiagPrintf_veneer>
3000afe2:	e7e1      	b.n	3000afa8 <crash_dump_memory+0x54>
3000afe4:	42008000 	.word	0x42008000
3000afe8:	3000d1dc 	.word	0x3000d1dc
3000afec:	3000d1e0 	.word	0x3000d1e0
3000aff0:	3000d1e4 	.word	0x3000d1e4
3000aff4:	3000d24c 	.word	0x3000d24c
3000aff8:	3000d208 	.word	0x3000d208
3000affc:	3000c71c 	.word	0x3000c71c
3000b000:	3000d254 	.word	0x3000d254
3000b004:	3000d244 	.word	0x3000d244

3000b008 <crash_dump>:
3000b008:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b00c:	4c4f      	ldr	r4, [pc, #316]	; (3000b14c <crash_dump+0x144>)
3000b00e:	4615      	mov	r5, r2
3000b010:	4688      	mov	r8, r1
3000b012:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000b016:	484e      	ldr	r0, [pc, #312]	; (3000b150 <crash_dump+0x148>)
3000b018:	462f      	mov	r7, r5
3000b01a:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000b17c <crash_dump+0x174>
3000b01e:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000b022:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000b188 <crash_dump+0x180>
3000b026:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000b02a:	e9c4 2100 	strd	r2, r1, [r4]
3000b02e:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000b030:	60a2      	str	r2, [r4, #8]
3000b032:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000b034:	60e2      	str	r2, [r4, #12]
3000b036:	686a      	ldr	r2, [r5, #4]
3000b038:	6122      	str	r2, [r4, #16]
3000b03a:	68aa      	ldr	r2, [r5, #8]
3000b03c:	6162      	str	r2, [r4, #20]
3000b03e:	68ea      	ldr	r2, [r5, #12]
3000b040:	61a2      	str	r2, [r4, #24]
3000b042:	692a      	ldr	r2, [r5, #16]
3000b044:	61e2      	str	r2, [r4, #28]
3000b046:	696a      	ldr	r2, [r5, #20]
3000b048:	6222      	str	r2, [r4, #32]
3000b04a:	69aa      	ldr	r2, [r5, #24]
3000b04c:	6262      	str	r2, [r4, #36]	; 0x24
3000b04e:	69ea      	ldr	r2, [r5, #28]
3000b050:	62a2      	str	r2, [r4, #40]	; 0x28
3000b052:	6a2a      	ldr	r2, [r5, #32]
3000b054:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b056:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000b058:	6322      	str	r2, [r4, #48]	; 0x30
3000b05a:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000b05c:	6362      	str	r2, [r4, #52]	; 0x34
3000b05e:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000b060:	63a2      	str	r2, [r4, #56]	; 0x38
3000b062:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000b064:	63e2      	str	r2, [r4, #60]	; 0x3c
3000b066:	682a      	ldr	r2, [r5, #0]
3000b068:	6422      	str	r2, [r4, #64]	; 0x40
3000b06a:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000b06e:	6462      	str	r2, [r4, #68]	; 0x44
3000b070:	4a38      	ldr	r2, [pc, #224]	; (3000b154 <crash_dump+0x14c>)
3000b072:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000b076:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000b07a:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000b07e:	64e1      	str	r1, [r4, #76]	; 0x4c
3000b080:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000b084:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000b088:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000b08c:	6561      	str	r1, [r4, #84]	; 0x54
3000b08e:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000b090:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000b094:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000b098:	65e1      	str	r1, [r4, #92]	; 0x5c
3000b09a:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000b09c:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000b09e:	b289      	uxth	r1, r1
3000b0a0:	b292      	uxth	r2, r2
3000b0a2:	6621      	str	r1, [r4, #96]	; 0x60
3000b0a4:	6662      	str	r2, [r4, #100]	; 0x64
3000b0a6:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000b0aa:	66a2      	str	r2, [r4, #104]	; 0x68
3000b0ac:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000b0b0:	66e3      	str	r3, [r4, #108]	; 0x6c
3000b0b2:	f000 fd1d 	bl	3000baf0 <__DiagPrintf_veneer>
3000b0b6:	4620      	mov	r0, r4
3000b0b8:	462c      	mov	r4, r5
3000b0ba:	f000 f867 	bl	3000b18c <fault_diagnosis>
3000b0be:	4826      	ldr	r0, [pc, #152]	; (3000b158 <crash_dump+0x150>)
3000b0c0:	f000 fd16 	bl	3000baf0 <__DiagPrintf_veneer>
3000b0c4:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000b0c6:	4825      	ldr	r0, [pc, #148]	; (3000b15c <crash_dump+0x154>)
3000b0c8:	f000 fd12 	bl	3000baf0 <__DiagPrintf_veneer>
3000b0cc:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000b0ce:	4824      	ldr	r0, [pc, #144]	; (3000b160 <crash_dump+0x158>)
3000b0d0:	f000 fd0e 	bl	3000baf0 <__DiagPrintf_veneer>
3000b0d4:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000b0d6:	4823      	ldr	r0, [pc, #140]	; (3000b164 <crash_dump+0x15c>)
3000b0d8:	f000 fd0a 	bl	3000baf0 <__DiagPrintf_veneer>
3000b0dc:	f857 1b20 	ldr.w	r1, [r7], #32
3000b0e0:	4821      	ldr	r0, [pc, #132]	; (3000b168 <crash_dump+0x160>)
3000b0e2:	463e      	mov	r6, r7
3000b0e4:	f000 fd04 	bl	3000baf0 <__DiagPrintf_veneer>
3000b0e8:	4920      	ldr	r1, [pc, #128]	; (3000b16c <crash_dump+0x164>)
3000b0ea:	4658      	mov	r0, fp
3000b0ec:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000b0f0:	f000 fcfe 	bl	3000baf0 <__DiagPrintf_veneer>
3000b0f4:	454e      	cmp	r6, r9
3000b0f6:	d008      	beq.n	3000b10a <crash_dump+0x102>
3000b0f8:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000b0fc:	4658      	mov	r0, fp
3000b0fe:	f85a 1b04 	ldr.w	r1, [sl], #4
3000b102:	f000 fcf5 	bl	3000baf0 <__DiagPrintf_veneer>
3000b106:	454e      	cmp	r6, r9
3000b108:	d1f6      	bne.n	3000b0f8 <crash_dump+0xf0>
3000b10a:	4e19      	ldr	r6, [pc, #100]	; (3000b170 <crash_dump+0x168>)
3000b10c:	4919      	ldr	r1, [pc, #100]	; (3000b174 <crash_dump+0x16c>)
3000b10e:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000b17c <crash_dump+0x174>
3000b112:	e001      	b.n	3000b118 <crash_dump+0x110>
3000b114:	f856 1b04 	ldr.w	r1, [r6], #4
3000b118:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000b11c:	4648      	mov	r0, r9
3000b11e:	f000 fce7 	bl	3000baf0 <__DiagPrintf_veneer>
3000b122:	42bc      	cmp	r4, r7
3000b124:	d1f6      	bne.n	3000b114 <crash_dump+0x10c>
3000b126:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000b128:	4913      	ldr	r1, [pc, #76]	; (3000b178 <crash_dump+0x170>)
3000b12a:	4814      	ldr	r0, [pc, #80]	; (3000b17c <crash_dump+0x174>)
3000b12c:	f000 fce0 	bl	3000baf0 <__DiagPrintf_veneer>
3000b130:	2180      	movs	r1, #128	; 0x80
3000b132:	4640      	mov	r0, r8
3000b134:	f7ff ff0e 	bl	3000af54 <crash_dump_memory>
3000b138:	4811      	ldr	r0, [pc, #68]	; (3000b180 <crash_dump+0x178>)
3000b13a:	f000 fcd9 	bl	3000baf0 <__DiagPrintf_veneer>
3000b13e:	4811      	ldr	r0, [pc, #68]	; (3000b184 <crash_dump+0x17c>)
3000b140:	f000 fcd6 	bl	3000baf0 <__DiagPrintf_veneer>
3000b144:	2000      	movs	r0, #0
3000b146:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b14a:	bf00      	nop
3000b14c:	3000f3c8 	.word	0x3000f3c8
3000b150:	3000d268 	.word	0x3000d268
3000b154:	e000ed00 	.word	0xe000ed00
3000b158:	3000d28c 	.word	0x3000d28c
3000b15c:	3000d2b4 	.word	0x3000d2b4
3000b160:	3000d2c4 	.word	0x3000d2c4
3000b164:	3000d2d4 	.word	0x3000d2d4
3000b168:	3000d2e4 	.word	0x3000d2e4
3000b16c:	3000d260 	.word	0x3000d260
3000b170:	3000d344 	.word	0x3000d344
3000b174:	3000d264 	.word	0x3000d264
3000b178:	3000d304 	.word	0x3000d304
3000b17c:	3000d2f4 	.word	0x3000d2f4
3000b180:	3000d308 	.word	0x3000d308
3000b184:	3000d334 	.word	0x3000d334
3000b188:	3000d364 	.word	0x3000d364

3000b18c <fault_diagnosis>:
3000b18c:	b510      	push	{r4, lr}
3000b18e:	4604      	mov	r4, r0
3000b190:	4888      	ldr	r0, [pc, #544]	; (3000b3b4 <fault_diagnosis+0x228>)
3000b192:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000b194:	f000 fcac 	bl	3000baf0 <__DiagPrintf_veneer>
3000b198:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000b19c:	079b      	lsls	r3, r3, #30
3000b19e:	f100 808a 	bmi.w	3000b2b6 <fault_diagnosis+0x12a>
3000b1a2:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b1a4:	b1e1      	cbz	r1, 3000b1e0 <fault_diagnosis+0x54>
3000b1a6:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b1aa:	07d8      	lsls	r0, r3, #31
3000b1ac:	f100 8094 	bmi.w	3000b2d8 <fault_diagnosis+0x14c>
3000b1b0:	079a      	lsls	r2, r3, #30
3000b1b2:	f100 809a 	bmi.w	3000b2ea <fault_diagnosis+0x15e>
3000b1b6:	0758      	lsls	r0, r3, #29
3000b1b8:	f100 80a1 	bmi.w	3000b2fe <fault_diagnosis+0x172>
3000b1bc:	0719      	lsls	r1, r3, #28
3000b1be:	f100 80a8 	bmi.w	3000b312 <fault_diagnosis+0x186>
3000b1c2:	06da      	lsls	r2, r3, #27
3000b1c4:	f100 80af 	bmi.w	3000b326 <fault_diagnosis+0x19a>
3000b1c8:	0698      	lsls	r0, r3, #26
3000b1ca:	f100 80b6 	bmi.w	3000b33a <fault_diagnosis+0x1ae>
3000b1ce:	0619      	lsls	r1, r3, #24
3000b1d0:	f100 80bd 	bmi.w	3000b34e <fault_diagnosis+0x1c2>
3000b1d4:	065a      	lsls	r2, r3, #25
3000b1d6:	d503      	bpl.n	3000b1e0 <fault_diagnosis+0x54>
3000b1d8:	f013 0f28 	tst.w	r3, #40	; 0x28
3000b1dc:	f040 8186 	bne.w	3000b4ec <fault_diagnosis+0x360>
3000b1e0:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b1e4:	b199      	cbz	r1, 3000b20e <fault_diagnosis+0x82>
3000b1e6:	07cb      	lsls	r3, r1, #31
3000b1e8:	f100 80b9 	bmi.w	3000b35e <fault_diagnosis+0x1d2>
3000b1ec:	0788      	lsls	r0, r1, #30
3000b1ee:	f100 80bf 	bmi.w	3000b370 <fault_diagnosis+0x1e4>
3000b1f2:	070a      	lsls	r2, r1, #28
3000b1f4:	f100 80c5 	bmi.w	3000b382 <fault_diagnosis+0x1f6>
3000b1f8:	06cb      	lsls	r3, r1, #27
3000b1fa:	f100 80cb 	bmi.w	3000b394 <fault_diagnosis+0x208>
3000b1fe:	0688      	lsls	r0, r1, #26
3000b200:	f100 80d1 	bmi.w	3000b3a6 <fault_diagnosis+0x21a>
3000b204:	060a      	lsls	r2, r1, #24
3000b206:	d502      	bpl.n	3000b20e <fault_diagnosis+0x82>
3000b208:	078b      	lsls	r3, r1, #30
3000b20a:	f040 8194 	bne.w	3000b536 <fault_diagnosis+0x3aa>
3000b20e:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b212:	b1b1      	cbz	r1, 3000b242 <fault_diagnosis+0xb6>
3000b214:	07c8      	lsls	r0, r1, #31
3000b216:	f100 80f7 	bmi.w	3000b408 <fault_diagnosis+0x27c>
3000b21a:	078a      	lsls	r2, r1, #30
3000b21c:	f100 80fd 	bmi.w	3000b41a <fault_diagnosis+0x28e>
3000b220:	074b      	lsls	r3, r1, #29
3000b222:	f100 8103 	bmi.w	3000b42c <fault_diagnosis+0x2a0>
3000b226:	0708      	lsls	r0, r1, #28
3000b228:	f100 8109 	bmi.w	3000b43e <fault_diagnosis+0x2b2>
3000b22c:	06ca      	lsls	r2, r1, #27
3000b22e:	f100 810f 	bmi.w	3000b450 <fault_diagnosis+0x2c4>
3000b232:	068b      	lsls	r3, r1, #26
3000b234:	f100 8115 	bmi.w	3000b462 <fault_diagnosis+0x2d6>
3000b238:	0608      	lsls	r0, r1, #24
3000b23a:	d502      	bpl.n	3000b242 <fault_diagnosis+0xb6>
3000b23c:	0789      	lsls	r1, r1, #30
3000b23e:	f100 8180 	bmi.w	3000b542 <fault_diagnosis+0x3b6>
3000b242:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b246:	b1b9      	cbz	r1, 3000b278 <fault_diagnosis+0xec>
3000b248:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b24c:	07da      	lsls	r2, r3, #31
3000b24e:	f100 810f 	bmi.w	3000b470 <fault_diagnosis+0x2e4>
3000b252:	0798      	lsls	r0, r3, #30
3000b254:	f100 8115 	bmi.w	3000b482 <fault_diagnosis+0x2f6>
3000b258:	0759      	lsls	r1, r3, #29
3000b25a:	f100 811d 	bmi.w	3000b498 <fault_diagnosis+0x30c>
3000b25e:	071a      	lsls	r2, r3, #28
3000b260:	f100 8125 	bmi.w	3000b4ae <fault_diagnosis+0x322>
3000b264:	06db      	lsls	r3, r3, #27
3000b266:	f100 812d 	bmi.w	3000b4c4 <fault_diagnosis+0x338>
3000b26a:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b26e:	07d8      	lsls	r0, r3, #31
3000b270:	f100 8133 	bmi.w	3000b4da <fault_diagnosis+0x34e>
3000b274:	0799      	lsls	r1, r3, #30
3000b276:	d424      	bmi.n	3000b2c2 <fault_diagnosis+0x136>
3000b278:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000b27c:	2b00      	cmp	r3, #0
3000b27e:	db00      	blt.n	3000b282 <fault_diagnosis+0xf6>
3000b280:	bd10      	pop	{r4, pc}
3000b282:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b284:	2900      	cmp	r1, #0
3000b286:	d0fb      	beq.n	3000b280 <fault_diagnosis+0xf4>
3000b288:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b28c:	07da      	lsls	r2, r3, #31
3000b28e:	f100 8143 	bmi.w	3000b518 <fault_diagnosis+0x38c>
3000b292:	0798      	lsls	r0, r3, #30
3000b294:	f100 8138 	bmi.w	3000b508 <fault_diagnosis+0x37c>
3000b298:	0759      	lsls	r1, r3, #29
3000b29a:	f100 812d 	bmi.w	3000b4f8 <fault_diagnosis+0x36c>
3000b29e:	071a      	lsls	r2, r3, #28
3000b2a0:	f100 8141 	bmi.w	3000b526 <fault_diagnosis+0x39a>
3000b2a4:	06db      	lsls	r3, r3, #27
3000b2a6:	d5eb      	bpl.n	3000b280 <fault_diagnosis+0xf4>
3000b2a8:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b2aa:	4a43      	ldr	r2, [pc, #268]	; (3000b3b8 <fault_diagnosis+0x22c>)
3000b2ac:	4843      	ldr	r0, [pc, #268]	; (3000b3bc <fault_diagnosis+0x230>)
3000b2ae:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000b2b2:	f000 bc1d 	b.w	3000baf0 <__DiagPrintf_veneer>
3000b2b6:	4a42      	ldr	r2, [pc, #264]	; (3000b3c0 <fault_diagnosis+0x234>)
3000b2b8:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000b2ba:	4842      	ldr	r0, [pc, #264]	; (3000b3c4 <fault_diagnosis+0x238>)
3000b2bc:	f000 fc18 	bl	3000baf0 <__DiagPrintf_veneer>
3000b2c0:	e76f      	b.n	3000b1a2 <fault_diagnosis+0x16>
3000b2c2:	4a41      	ldr	r2, [pc, #260]	; (3000b3c8 <fault_diagnosis+0x23c>)
3000b2c4:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b2c8:	4840      	ldr	r0, [pc, #256]	; (3000b3cc <fault_diagnosis+0x240>)
3000b2ca:	f000 fc11 	bl	3000baf0 <__DiagPrintf_veneer>
3000b2ce:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000b2d2:	2b00      	cmp	r3, #0
3000b2d4:	dad4      	bge.n	3000b280 <fault_diagnosis+0xf4>
3000b2d6:	e7d4      	b.n	3000b282 <fault_diagnosis+0xf6>
3000b2d8:	4a3d      	ldr	r2, [pc, #244]	; (3000b3d0 <fault_diagnosis+0x244>)
3000b2da:	483e      	ldr	r0, [pc, #248]	; (3000b3d4 <fault_diagnosis+0x248>)
3000b2dc:	f000 fc08 	bl	3000baf0 <__DiagPrintf_veneer>
3000b2e0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b2e4:	079a      	lsls	r2, r3, #30
3000b2e6:	f57f af66 	bpl.w	3000b1b6 <fault_diagnosis+0x2a>
3000b2ea:	4a3b      	ldr	r2, [pc, #236]	; (3000b3d8 <fault_diagnosis+0x24c>)
3000b2ec:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b2ee:	4839      	ldr	r0, [pc, #228]	; (3000b3d4 <fault_diagnosis+0x248>)
3000b2f0:	f000 fbfe 	bl	3000baf0 <__DiagPrintf_veneer>
3000b2f4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b2f8:	0758      	lsls	r0, r3, #29
3000b2fa:	f57f af5f 	bpl.w	3000b1bc <fault_diagnosis+0x30>
3000b2fe:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b300:	4a36      	ldr	r2, [pc, #216]	; (3000b3dc <fault_diagnosis+0x250>)
3000b302:	4834      	ldr	r0, [pc, #208]	; (3000b3d4 <fault_diagnosis+0x248>)
3000b304:	f000 fbf4 	bl	3000baf0 <__DiagPrintf_veneer>
3000b308:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b30c:	0719      	lsls	r1, r3, #28
3000b30e:	f57f af58 	bpl.w	3000b1c2 <fault_diagnosis+0x36>
3000b312:	4a33      	ldr	r2, [pc, #204]	; (3000b3e0 <fault_diagnosis+0x254>)
3000b314:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b316:	482f      	ldr	r0, [pc, #188]	; (3000b3d4 <fault_diagnosis+0x248>)
3000b318:	f000 fbea 	bl	3000baf0 <__DiagPrintf_veneer>
3000b31c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b320:	06da      	lsls	r2, r3, #27
3000b322:	f57f af51 	bpl.w	3000b1c8 <fault_diagnosis+0x3c>
3000b326:	4a2f      	ldr	r2, [pc, #188]	; (3000b3e4 <fault_diagnosis+0x258>)
3000b328:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b32a:	482a      	ldr	r0, [pc, #168]	; (3000b3d4 <fault_diagnosis+0x248>)
3000b32c:	f000 fbe0 	bl	3000baf0 <__DiagPrintf_veneer>
3000b330:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b334:	0698      	lsls	r0, r3, #26
3000b336:	f57f af4a 	bpl.w	3000b1ce <fault_diagnosis+0x42>
3000b33a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b33c:	4a2a      	ldr	r2, [pc, #168]	; (3000b3e8 <fault_diagnosis+0x25c>)
3000b33e:	4825      	ldr	r0, [pc, #148]	; (3000b3d4 <fault_diagnosis+0x248>)
3000b340:	f000 fbd6 	bl	3000baf0 <__DiagPrintf_veneer>
3000b344:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b348:	0619      	lsls	r1, r3, #24
3000b34a:	f57f af43 	bpl.w	3000b1d4 <fault_diagnosis+0x48>
3000b34e:	4a27      	ldr	r2, [pc, #156]	; (3000b3ec <fault_diagnosis+0x260>)
3000b350:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b352:	4820      	ldr	r0, [pc, #128]	; (3000b3d4 <fault_diagnosis+0x248>)
3000b354:	f000 fbcc 	bl	3000baf0 <__DiagPrintf_veneer>
3000b358:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b35c:	e73a      	b.n	3000b1d4 <fault_diagnosis+0x48>
3000b35e:	4a24      	ldr	r2, [pc, #144]	; (3000b3f0 <fault_diagnosis+0x264>)
3000b360:	4824      	ldr	r0, [pc, #144]	; (3000b3f4 <fault_diagnosis+0x268>)
3000b362:	f000 fbc5 	bl	3000baf0 <__DiagPrintf_veneer>
3000b366:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b36a:	0788      	lsls	r0, r1, #30
3000b36c:	f57f af41 	bpl.w	3000b1f2 <fault_diagnosis+0x66>
3000b370:	4a21      	ldr	r2, [pc, #132]	; (3000b3f8 <fault_diagnosis+0x26c>)
3000b372:	4820      	ldr	r0, [pc, #128]	; (3000b3f4 <fault_diagnosis+0x268>)
3000b374:	f000 fbbc 	bl	3000baf0 <__DiagPrintf_veneer>
3000b378:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b37c:	070a      	lsls	r2, r1, #28
3000b37e:	f57f af3b 	bpl.w	3000b1f8 <fault_diagnosis+0x6c>
3000b382:	4a1e      	ldr	r2, [pc, #120]	; (3000b3fc <fault_diagnosis+0x270>)
3000b384:	481b      	ldr	r0, [pc, #108]	; (3000b3f4 <fault_diagnosis+0x268>)
3000b386:	f000 fbb3 	bl	3000baf0 <__DiagPrintf_veneer>
3000b38a:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b38e:	06cb      	lsls	r3, r1, #27
3000b390:	f57f af35 	bpl.w	3000b1fe <fault_diagnosis+0x72>
3000b394:	4a1a      	ldr	r2, [pc, #104]	; (3000b400 <fault_diagnosis+0x274>)
3000b396:	4817      	ldr	r0, [pc, #92]	; (3000b3f4 <fault_diagnosis+0x268>)
3000b398:	f000 fbaa 	bl	3000baf0 <__DiagPrintf_veneer>
3000b39c:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b3a0:	0688      	lsls	r0, r1, #26
3000b3a2:	f57f af2f 	bpl.w	3000b204 <fault_diagnosis+0x78>
3000b3a6:	4a17      	ldr	r2, [pc, #92]	; (3000b404 <fault_diagnosis+0x278>)
3000b3a8:	4812      	ldr	r0, [pc, #72]	; (3000b3f4 <fault_diagnosis+0x268>)
3000b3aa:	f000 fba1 	bl	3000baf0 <__DiagPrintf_veneer>
3000b3ae:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b3b2:	e727      	b.n	3000b204 <fault_diagnosis+0x78>
3000b3b4:	3000d374 	.word	0x3000d374
3000b3b8:	3000dc04 	.word	0x3000dc04
3000b3bc:	3000db5c 	.word	0x3000db5c
3000b3c0:	3000d38c 	.word	0x3000d38c
3000b3c4:	3000d3b8 	.word	0x3000d3b8
3000b3c8:	3000dac0 	.word	0x3000dac0
3000b3cc:	3000d904 	.word	0x3000d904
3000b3d0:	3000d3d0 	.word	0x3000d3d0
3000b3d4:	3000d400 	.word	0x3000d400
3000b3d8:	3000d418 	.word	0x3000d418
3000b3dc:	3000d450 	.word	0x3000d450
3000b3e0:	3000d488 	.word	0x3000d488
3000b3e4:	3000d4c0 	.word	0x3000d4c0
3000b3e8:	3000d4f0 	.word	0x3000d4f0
3000b3ec:	3000d52c 	.word	0x3000d52c
3000b3f0:	3000d59c 	.word	0x3000d59c
3000b3f4:	3000d5e0 	.word	0x3000d5e0
3000b3f8:	3000d5f8 	.word	0x3000d5f8
3000b3fc:	3000d634 	.word	0x3000d634
3000b400:	3000d66c 	.word	0x3000d66c
3000b404:	3000d6a0 	.word	0x3000d6a0
3000b408:	4a51      	ldr	r2, [pc, #324]	; (3000b550 <fault_diagnosis+0x3c4>)
3000b40a:	4852      	ldr	r0, [pc, #328]	; (3000b554 <fault_diagnosis+0x3c8>)
3000b40c:	f000 fb70 	bl	3000baf0 <__DiagPrintf_veneer>
3000b410:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b414:	078a      	lsls	r2, r1, #30
3000b416:	f57f af03 	bpl.w	3000b220 <fault_diagnosis+0x94>
3000b41a:	4a4f      	ldr	r2, [pc, #316]	; (3000b558 <fault_diagnosis+0x3cc>)
3000b41c:	484d      	ldr	r0, [pc, #308]	; (3000b554 <fault_diagnosis+0x3c8>)
3000b41e:	f000 fb67 	bl	3000baf0 <__DiagPrintf_veneer>
3000b422:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b426:	074b      	lsls	r3, r1, #29
3000b428:	f57f aefd 	bpl.w	3000b226 <fault_diagnosis+0x9a>
3000b42c:	4a4b      	ldr	r2, [pc, #300]	; (3000b55c <fault_diagnosis+0x3d0>)
3000b42e:	4849      	ldr	r0, [pc, #292]	; (3000b554 <fault_diagnosis+0x3c8>)
3000b430:	f000 fb5e 	bl	3000baf0 <__DiagPrintf_veneer>
3000b434:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b438:	0708      	lsls	r0, r1, #28
3000b43a:	f57f aef7 	bpl.w	3000b22c <fault_diagnosis+0xa0>
3000b43e:	4a48      	ldr	r2, [pc, #288]	; (3000b560 <fault_diagnosis+0x3d4>)
3000b440:	4844      	ldr	r0, [pc, #272]	; (3000b554 <fault_diagnosis+0x3c8>)
3000b442:	f000 fb55 	bl	3000baf0 <__DiagPrintf_veneer>
3000b446:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b44a:	06ca      	lsls	r2, r1, #27
3000b44c:	f57f aef1 	bpl.w	3000b232 <fault_diagnosis+0xa6>
3000b450:	4a44      	ldr	r2, [pc, #272]	; (3000b564 <fault_diagnosis+0x3d8>)
3000b452:	4840      	ldr	r0, [pc, #256]	; (3000b554 <fault_diagnosis+0x3c8>)
3000b454:	f000 fb4c 	bl	3000baf0 <__DiagPrintf_veneer>
3000b458:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b45c:	068b      	lsls	r3, r1, #26
3000b45e:	f57f aeeb 	bpl.w	3000b238 <fault_diagnosis+0xac>
3000b462:	4a41      	ldr	r2, [pc, #260]	; (3000b568 <fault_diagnosis+0x3dc>)
3000b464:	483b      	ldr	r0, [pc, #236]	; (3000b554 <fault_diagnosis+0x3c8>)
3000b466:	f000 fb43 	bl	3000baf0 <__DiagPrintf_veneer>
3000b46a:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b46e:	e6e3      	b.n	3000b238 <fault_diagnosis+0xac>
3000b470:	4a3e      	ldr	r2, [pc, #248]	; (3000b56c <fault_diagnosis+0x3e0>)
3000b472:	483f      	ldr	r0, [pc, #252]	; (3000b570 <fault_diagnosis+0x3e4>)
3000b474:	f000 fb3c 	bl	3000baf0 <__DiagPrintf_veneer>
3000b478:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b47c:	0798      	lsls	r0, r3, #30
3000b47e:	f57f aeeb 	bpl.w	3000b258 <fault_diagnosis+0xcc>
3000b482:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b486:	4a3b      	ldr	r2, [pc, #236]	; (3000b574 <fault_diagnosis+0x3e8>)
3000b488:	4839      	ldr	r0, [pc, #228]	; (3000b570 <fault_diagnosis+0x3e4>)
3000b48a:	f000 fb31 	bl	3000baf0 <__DiagPrintf_veneer>
3000b48e:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b492:	0759      	lsls	r1, r3, #29
3000b494:	f57f aee3 	bpl.w	3000b25e <fault_diagnosis+0xd2>
3000b498:	4a37      	ldr	r2, [pc, #220]	; (3000b578 <fault_diagnosis+0x3ec>)
3000b49a:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b49e:	4834      	ldr	r0, [pc, #208]	; (3000b570 <fault_diagnosis+0x3e4>)
3000b4a0:	f000 fb26 	bl	3000baf0 <__DiagPrintf_veneer>
3000b4a4:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b4a8:	071a      	lsls	r2, r3, #28
3000b4aa:	f57f aedb 	bpl.w	3000b264 <fault_diagnosis+0xd8>
3000b4ae:	4a33      	ldr	r2, [pc, #204]	; (3000b57c <fault_diagnosis+0x3f0>)
3000b4b0:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b4b4:	482e      	ldr	r0, [pc, #184]	; (3000b570 <fault_diagnosis+0x3e4>)
3000b4b6:	f000 fb1b 	bl	3000baf0 <__DiagPrintf_veneer>
3000b4ba:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b4be:	06db      	lsls	r3, r3, #27
3000b4c0:	f57f aed3 	bpl.w	3000b26a <fault_diagnosis+0xde>
3000b4c4:	4a2e      	ldr	r2, [pc, #184]	; (3000b580 <fault_diagnosis+0x3f4>)
3000b4c6:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b4ca:	4829      	ldr	r0, [pc, #164]	; (3000b570 <fault_diagnosis+0x3e4>)
3000b4cc:	f000 fb10 	bl	3000baf0 <__DiagPrintf_veneer>
3000b4d0:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b4d4:	07d8      	lsls	r0, r3, #31
3000b4d6:	f57f aecd 	bpl.w	3000b274 <fault_diagnosis+0xe8>
3000b4da:	4a2a      	ldr	r2, [pc, #168]	; (3000b584 <fault_diagnosis+0x3f8>)
3000b4dc:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b4e0:	4823      	ldr	r0, [pc, #140]	; (3000b570 <fault_diagnosis+0x3e4>)
3000b4e2:	f000 fb05 	bl	3000baf0 <__DiagPrintf_veneer>
3000b4e6:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b4ea:	e6c3      	b.n	3000b274 <fault_diagnosis+0xe8>
3000b4ec:	4a26      	ldr	r2, [pc, #152]	; (3000b588 <fault_diagnosis+0x3fc>)
3000b4ee:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000b4f0:	4826      	ldr	r0, [pc, #152]	; (3000b58c <fault_diagnosis+0x400>)
3000b4f2:	f000 fafd 	bl	3000baf0 <__DiagPrintf_veneer>
3000b4f6:	e673      	b.n	3000b1e0 <fault_diagnosis+0x54>
3000b4f8:	4a25      	ldr	r2, [pc, #148]	; (3000b590 <fault_diagnosis+0x404>)
3000b4fa:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b4fc:	4825      	ldr	r0, [pc, #148]	; (3000b594 <fault_diagnosis+0x408>)
3000b4fe:	f000 faf7 	bl	3000baf0 <__DiagPrintf_veneer>
3000b502:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b506:	e6ca      	b.n	3000b29e <fault_diagnosis+0x112>
3000b508:	4a23      	ldr	r2, [pc, #140]	; (3000b598 <fault_diagnosis+0x40c>)
3000b50a:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b50c:	4821      	ldr	r0, [pc, #132]	; (3000b594 <fault_diagnosis+0x408>)
3000b50e:	f000 faef 	bl	3000baf0 <__DiagPrintf_veneer>
3000b512:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b516:	e6bf      	b.n	3000b298 <fault_diagnosis+0x10c>
3000b518:	4a20      	ldr	r2, [pc, #128]	; (3000b59c <fault_diagnosis+0x410>)
3000b51a:	481e      	ldr	r0, [pc, #120]	; (3000b594 <fault_diagnosis+0x408>)
3000b51c:	f000 fae8 	bl	3000baf0 <__DiagPrintf_veneer>
3000b520:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b524:	e6b5      	b.n	3000b292 <fault_diagnosis+0x106>
3000b526:	4a1e      	ldr	r2, [pc, #120]	; (3000b5a0 <fault_diagnosis+0x414>)
3000b528:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b52a:	481a      	ldr	r0, [pc, #104]	; (3000b594 <fault_diagnosis+0x408>)
3000b52c:	f000 fae0 	bl	3000baf0 <__DiagPrintf_veneer>
3000b530:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b534:	e6b6      	b.n	3000b2a4 <fault_diagnosis+0x118>
3000b536:	4a1b      	ldr	r2, [pc, #108]	; (3000b5a4 <fault_diagnosis+0x418>)
3000b538:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000b53a:	481b      	ldr	r0, [pc, #108]	; (3000b5a8 <fault_diagnosis+0x41c>)
3000b53c:	f000 fad8 	bl	3000baf0 <__DiagPrintf_veneer>
3000b540:	e665      	b.n	3000b20e <fault_diagnosis+0x82>
3000b542:	4a1a      	ldr	r2, [pc, #104]	; (3000b5ac <fault_diagnosis+0x420>)
3000b544:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000b546:	481a      	ldr	r0, [pc, #104]	; (3000b5b0 <fault_diagnosis+0x424>)
3000b548:	f000 fad2 	bl	3000baf0 <__DiagPrintf_veneer>
3000b54c:	e679      	b.n	3000b242 <fault_diagnosis+0xb6>
3000b54e:	bf00      	nop
3000b550:	3000d734 	.word	0x3000d734
3000b554:	3000d768 	.word	0x3000d768
3000b558:	3000d780 	.word	0x3000d780
3000b55c:	3000d7b8 	.word	0x3000d7b8
3000b560:	3000d7f0 	.word	0x3000d7f0
3000b564:	3000d818 	.word	0x3000d818
3000b568:	3000d840 	.word	0x3000d840
3000b56c:	3000d8bc 	.word	0x3000d8bc
3000b570:	3000d904 	.word	0x3000d904
3000b574:	3000d91c 	.word	0x3000d91c
3000b578:	3000d968 	.word	0x3000d968
3000b57c:	3000d9c8 	.word	0x3000d9c8
3000b580:	3000da10 	.word	0x3000da10
3000b584:	3000da6c 	.word	0x3000da6c
3000b588:	3000d55c 	.word	0x3000d55c
3000b58c:	3000d584 	.word	0x3000d584
3000b590:	3000dba8 	.word	0x3000dba8
3000b594:	3000db5c 	.word	0x3000db5c
3000b598:	3000db74 	.word	0x3000db74
3000b59c:	3000db2c 	.word	0x3000db2c
3000b5a0:	3000dbd4 	.word	0x3000dbd4
3000b5a4:	3000d6ec 	.word	0x3000d6ec
3000b5a8:	3000d71c 	.word	0x3000d71c
3000b5ac:	3000d880 	.word	0x3000d880
3000b5b0:	3000d8a4 	.word	0x3000d8a4

3000b5b4 <rtk_log_write>:
3000b5b4:	b40c      	push	{r2, r3}
3000b5b6:	4b18      	ldr	r3, [pc, #96]	; (3000b618 <rtk_log_write+0x64>)
3000b5b8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000b5bc:	681a      	ldr	r2, [r3, #0]
3000b5be:	b083      	sub	sp, #12
3000b5c0:	4680      	mov	r8, r0
3000b5c2:	460e      	mov	r6, r1
3000b5c4:	2a02      	cmp	r2, #2
3000b5c6:	d825      	bhi.n	3000b614 <rtk_log_write+0x60>
3000b5c8:	681f      	ldr	r7, [r3, #0]
3000b5ca:	b1a7      	cbz	r7, 3000b5f6 <rtk_log_write+0x42>
3000b5cc:	4d13      	ldr	r5, [pc, #76]	; (3000b61c <rtk_log_write+0x68>)
3000b5ce:	2400      	movs	r4, #0
3000b5d0:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000b628 <rtk_log_write+0x74>
3000b5d4:	e002      	b.n	3000b5dc <rtk_log_write+0x28>
3000b5d6:	3401      	adds	r4, #1
3000b5d8:	42a7      	cmp	r7, r4
3000b5da:	d90c      	bls.n	3000b5f6 <rtk_log_write+0x42>
3000b5dc:	4628      	mov	r0, r5
3000b5de:	4631      	mov	r1, r6
3000b5e0:	350b      	adds	r5, #11
3000b5e2:	47c8      	blx	r9
3000b5e4:	2800      	cmp	r0, #0
3000b5e6:	d1f6      	bne.n	3000b5d6 <rtk_log_write+0x22>
3000b5e8:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000b5ec:	4b0c      	ldr	r3, [pc, #48]	; (3000b620 <rtk_log_write+0x6c>)
3000b5ee:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000b5f2:	5d1b      	ldrb	r3, [r3, r4]
3000b5f4:	e001      	b.n	3000b5fa <rtk_log_write+0x46>
3000b5f6:	4b0b      	ldr	r3, [pc, #44]	; (3000b624 <rtk_log_write+0x70>)
3000b5f8:	781b      	ldrb	r3, [r3, #0]
3000b5fa:	4598      	cmp	r8, r3
3000b5fc:	d805      	bhi.n	3000b60a <rtk_log_write+0x56>
3000b5fe:	aa0b      	add	r2, sp, #44	; 0x2c
3000b600:	990a      	ldr	r1, [sp, #40]	; 0x28
3000b602:	2000      	movs	r0, #0
3000b604:	9201      	str	r2, [sp, #4]
3000b606:	f000 fa23 	bl	3000ba50 <__DiagVSprintf_veneer>
3000b60a:	b003      	add	sp, #12
3000b60c:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000b610:	b002      	add	sp, #8
3000b612:	4770      	bx	lr
3000b614:	2703      	movs	r7, #3
3000b616:	e7d9      	b.n	3000b5cc <rtk_log_write+0x18>
3000b618:	3000f810 	.word	0x3000f810
3000b61c:	3000f815 	.word	0x3000f815
3000b620:	3000f814 	.word	0x3000f814
3000b624:	3000edec 	.word	0x3000edec
3000b628:	00012c89 	.word	0x00012c89

3000b62c <BOOT_VerCheck>:
3000b62c:	b570      	push	{r4, r5, r6, lr}
3000b62e:	4b1d      	ldr	r3, [pc, #116]	; (3000b6a4 <BOOT_VerCheck+0x78>)
3000b630:	4798      	blx	r3
3000b632:	2800      	cmp	r0, #0
3000b634:	d135      	bne.n	3000b6a2 <BOOT_VerCheck+0x76>
3000b636:	4b1c      	ldr	r3, [pc, #112]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b638:	2403      	movs	r4, #3
3000b63a:	4d1c      	ldr	r5, [pc, #112]	; (3000b6ac <BOOT_VerCheck+0x80>)
3000b63c:	2003      	movs	r0, #3
3000b63e:	4a1c      	ldr	r2, [pc, #112]	; (3000b6b0 <BOOT_VerCheck+0x84>)
3000b640:	4619      	mov	r1, r3
3000b642:	f7ff ffb7 	bl	3000b5b4 <rtk_log_write>
3000b646:	4b18      	ldr	r3, [pc, #96]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b648:	462a      	mov	r2, r5
3000b64a:	2003      	movs	r0, #3
3000b64c:	4619      	mov	r1, r3
3000b64e:	f7ff ffb1 	bl	3000b5b4 <rtk_log_write>
3000b652:	4b15      	ldr	r3, [pc, #84]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b654:	4a17      	ldr	r2, [pc, #92]	; (3000b6b4 <BOOT_VerCheck+0x88>)
3000b656:	2003      	movs	r0, #3
3000b658:	4619      	mov	r1, r3
3000b65a:	f7ff ffab 	bl	3000b5b4 <rtk_log_write>
3000b65e:	4b12      	ldr	r3, [pc, #72]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b660:	4a15      	ldr	r2, [pc, #84]	; (3000b6b8 <BOOT_VerCheck+0x8c>)
3000b662:	2003      	movs	r0, #3
3000b664:	4619      	mov	r1, r3
3000b666:	f7ff ffa5 	bl	3000b5b4 <rtk_log_write>
3000b66a:	4b0f      	ldr	r3, [pc, #60]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b66c:	4a13      	ldr	r2, [pc, #76]	; (3000b6bc <BOOT_VerCheck+0x90>)
3000b66e:	2003      	movs	r0, #3
3000b670:	4619      	mov	r1, r3
3000b672:	f7ff ff9f 	bl	3000b5b4 <rtk_log_write>
3000b676:	4b0c      	ldr	r3, [pc, #48]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b678:	462a      	mov	r2, r5
3000b67a:	2003      	movs	r0, #3
3000b67c:	4619      	mov	r1, r3
3000b67e:	f7ff ff99 	bl	3000b5b4 <rtk_log_write>
3000b682:	4b09      	ldr	r3, [pc, #36]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b684:	4a0e      	ldr	r2, [pc, #56]	; (3000b6c0 <BOOT_VerCheck+0x94>)
3000b686:	2003      	movs	r0, #3
3000b688:	4619      	mov	r1, r3
3000b68a:	f7ff ff93 	bl	3000b5b4 <rtk_log_write>
3000b68e:	3c01      	subs	r4, #1
3000b690:	d1d9      	bne.n	3000b646 <BOOT_VerCheck+0x1a>
3000b692:	4b05      	ldr	r3, [pc, #20]	; (3000b6a8 <BOOT_VerCheck+0x7c>)
3000b694:	2003      	movs	r0, #3
3000b696:	4a0b      	ldr	r2, [pc, #44]	; (3000b6c4 <BOOT_VerCheck+0x98>)
3000b698:	4619      	mov	r1, r3
3000b69a:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000b69e:	f7ff bf89 	b.w	3000b5b4 <rtk_log_write>
3000b6a2:	bd70      	pop	{r4, r5, r6, pc}
3000b6a4:	300092c5 	.word	0x300092c5
3000b6a8:	3000dce6 	.word	0x3000dce6
3000b6ac:	3000dd2e 	.word	0x3000dd2e
3000b6b0:	3000dceb 	.word	0x3000dceb
3000b6b4:	3000dd71 	.word	0x3000dd71
3000b6b8:	3000ddb4 	.word	0x3000ddb4
3000b6bc:	3000ddf7 	.word	0x3000ddf7
3000b6c0:	3000de3a 	.word	0x3000de3a
3000b6c4:	3000de7d 	.word	0x3000de7d

3000b6c8 <__aeabi_uldivmod>:
3000b6c8:	b953      	cbnz	r3, 3000b6e0 <__aeabi_uldivmod+0x18>
3000b6ca:	b94a      	cbnz	r2, 3000b6e0 <__aeabi_uldivmod+0x18>
3000b6cc:	2900      	cmp	r1, #0
3000b6ce:	bf08      	it	eq
3000b6d0:	2800      	cmpeq	r0, #0
3000b6d2:	bf1c      	itt	ne
3000b6d4:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000b6d8:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000b6dc:	f000 b960 	b.w	3000b9a0 <__aeabi_idiv0>
3000b6e0:	f1ad 0c08 	sub.w	ip, sp, #8
3000b6e4:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000b6e8:	f000 f806 	bl	3000b6f8 <__udivmoddi4>
3000b6ec:	f8dd e004 	ldr.w	lr, [sp, #4]
3000b6f0:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000b6f4:	b004      	add	sp, #16
3000b6f6:	4770      	bx	lr

3000b6f8 <__udivmoddi4>:
3000b6f8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b6fc:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000b6fe:	4688      	mov	r8, r1
3000b700:	468e      	mov	lr, r1
3000b702:	4615      	mov	r5, r2
3000b704:	4604      	mov	r4, r0
3000b706:	4619      	mov	r1, r3
3000b708:	2b00      	cmp	r3, #0
3000b70a:	f040 80c6 	bne.w	3000b89a <__udivmoddi4+0x1a2>
3000b70e:	4542      	cmp	r2, r8
3000b710:	fab2 f782 	clz	r7, r2
3000b714:	d946      	bls.n	3000b7a4 <__udivmoddi4+0xac>
3000b716:	b14f      	cbz	r7, 3000b72c <__udivmoddi4+0x34>
3000b718:	f1c7 0c20 	rsb	ip, r7, #32
3000b71c:	fa08 f307 	lsl.w	r3, r8, r7
3000b720:	40bd      	lsls	r5, r7
3000b722:	40bc      	lsls	r4, r7
3000b724:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b728:	ea4c 0e03 	orr.w	lr, ip, r3
3000b72c:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000b730:	fa1f fc85 	uxth.w	ip, r5
3000b734:	0c22      	lsrs	r2, r4, #16
3000b736:	fbbe f9f8 	udiv	r9, lr, r8
3000b73a:	fb08 e319 	mls	r3, r8, r9, lr
3000b73e:	fb09 fa0c 	mul.w	sl, r9, ip
3000b742:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000b746:	459a      	cmp	sl, r3
3000b748:	d928      	bls.n	3000b79c <__udivmoddi4+0xa4>
3000b74a:	18eb      	adds	r3, r5, r3
3000b74c:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000b750:	d204      	bcs.n	3000b75c <__udivmoddi4+0x64>
3000b752:	459a      	cmp	sl, r3
3000b754:	d902      	bls.n	3000b75c <__udivmoddi4+0x64>
3000b756:	f1a9 0002 	sub.w	r0, r9, #2
3000b75a:	442b      	add	r3, r5
3000b75c:	eba3 030a 	sub.w	r3, r3, sl
3000b760:	b2a4      	uxth	r4, r4
3000b762:	fbb3 f2f8 	udiv	r2, r3, r8
3000b766:	fb08 3312 	mls	r3, r8, r2, r3
3000b76a:	fb02 fc0c 	mul.w	ip, r2, ip
3000b76e:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b772:	45a4      	cmp	ip, r4
3000b774:	d914      	bls.n	3000b7a0 <__udivmoddi4+0xa8>
3000b776:	192c      	adds	r4, r5, r4
3000b778:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000b77c:	d203      	bcs.n	3000b786 <__udivmoddi4+0x8e>
3000b77e:	45a4      	cmp	ip, r4
3000b780:	d901      	bls.n	3000b786 <__udivmoddi4+0x8e>
3000b782:	1e93      	subs	r3, r2, #2
3000b784:	442c      	add	r4, r5
3000b786:	eba4 040c 	sub.w	r4, r4, ip
3000b78a:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000b78e:	b11e      	cbz	r6, 3000b798 <__udivmoddi4+0xa0>
3000b790:	40fc      	lsrs	r4, r7
3000b792:	2300      	movs	r3, #0
3000b794:	6034      	str	r4, [r6, #0]
3000b796:	6073      	str	r3, [r6, #4]
3000b798:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b79c:	4648      	mov	r0, r9
3000b79e:	e7dd      	b.n	3000b75c <__udivmoddi4+0x64>
3000b7a0:	4613      	mov	r3, r2
3000b7a2:	e7f0      	b.n	3000b786 <__udivmoddi4+0x8e>
3000b7a4:	b902      	cbnz	r2, 3000b7a8 <__udivmoddi4+0xb0>
3000b7a6:	deff      	udf	#255	; 0xff
3000b7a8:	bb8f      	cbnz	r7, 3000b80e <__udivmoddi4+0x116>
3000b7aa:	eba8 0302 	sub.w	r3, r8, r2
3000b7ae:	2101      	movs	r1, #1
3000b7b0:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b7b4:	b2aa      	uxth	r2, r5
3000b7b6:	0c20      	lsrs	r0, r4, #16
3000b7b8:	fbb3 fcfe 	udiv	ip, r3, lr
3000b7bc:	fb0e 331c 	mls	r3, lr, ip, r3
3000b7c0:	fb0c f802 	mul.w	r8, ip, r2
3000b7c4:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000b7c8:	4598      	cmp	r8, r3
3000b7ca:	d962      	bls.n	3000b892 <__udivmoddi4+0x19a>
3000b7cc:	18eb      	adds	r3, r5, r3
3000b7ce:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000b7d2:	d204      	bcs.n	3000b7de <__udivmoddi4+0xe6>
3000b7d4:	4598      	cmp	r8, r3
3000b7d6:	d902      	bls.n	3000b7de <__udivmoddi4+0xe6>
3000b7d8:	f1ac 0002 	sub.w	r0, ip, #2
3000b7dc:	442b      	add	r3, r5
3000b7de:	eba3 0308 	sub.w	r3, r3, r8
3000b7e2:	b2a4      	uxth	r4, r4
3000b7e4:	fbb3 fcfe 	udiv	ip, r3, lr
3000b7e8:	fb0e 331c 	mls	r3, lr, ip, r3
3000b7ec:	fb0c f202 	mul.w	r2, ip, r2
3000b7f0:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b7f4:	42a2      	cmp	r2, r4
3000b7f6:	d94e      	bls.n	3000b896 <__udivmoddi4+0x19e>
3000b7f8:	192c      	adds	r4, r5, r4
3000b7fa:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000b7fe:	d204      	bcs.n	3000b80a <__udivmoddi4+0x112>
3000b800:	42a2      	cmp	r2, r4
3000b802:	d902      	bls.n	3000b80a <__udivmoddi4+0x112>
3000b804:	f1ac 0302 	sub.w	r3, ip, #2
3000b808:	442c      	add	r4, r5
3000b80a:	1aa4      	subs	r4, r4, r2
3000b80c:	e7bd      	b.n	3000b78a <__udivmoddi4+0x92>
3000b80e:	f1c7 0c20 	rsb	ip, r7, #32
3000b812:	40bd      	lsls	r5, r7
3000b814:	fa08 f307 	lsl.w	r3, r8, r7
3000b818:	40bc      	lsls	r4, r7
3000b81a:	fa28 f90c 	lsr.w	r9, r8, ip
3000b81e:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b822:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b826:	ea4c 0203 	orr.w	r2, ip, r3
3000b82a:	b2ab      	uxth	r3, r5
3000b82c:	fbb9 fcfe 	udiv	ip, r9, lr
3000b830:	0c11      	lsrs	r1, r2, #16
3000b832:	fb0e 901c 	mls	r0, lr, ip, r9
3000b836:	fb0c f803 	mul.w	r8, ip, r3
3000b83a:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b83e:	4580      	cmp	r8, r0
3000b840:	d923      	bls.n	3000b88a <__udivmoddi4+0x192>
3000b842:	1828      	adds	r0, r5, r0
3000b844:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000b848:	d204      	bcs.n	3000b854 <__udivmoddi4+0x15c>
3000b84a:	4580      	cmp	r8, r0
3000b84c:	d902      	bls.n	3000b854 <__udivmoddi4+0x15c>
3000b84e:	f1ac 0102 	sub.w	r1, ip, #2
3000b852:	4428      	add	r0, r5
3000b854:	eba0 0008 	sub.w	r0, r0, r8
3000b858:	b292      	uxth	r2, r2
3000b85a:	fbb0 fcfe 	udiv	ip, r0, lr
3000b85e:	fb0e 001c 	mls	r0, lr, ip, r0
3000b862:	fb0c f803 	mul.w	r8, ip, r3
3000b866:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000b86a:	4598      	cmp	r8, r3
3000b86c:	d90f      	bls.n	3000b88e <__udivmoddi4+0x196>
3000b86e:	18eb      	adds	r3, r5, r3
3000b870:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000b874:	d204      	bcs.n	3000b880 <__udivmoddi4+0x188>
3000b876:	4598      	cmp	r8, r3
3000b878:	d902      	bls.n	3000b880 <__udivmoddi4+0x188>
3000b87a:	f1ac 0202 	sub.w	r2, ip, #2
3000b87e:	442b      	add	r3, r5
3000b880:	eba3 0308 	sub.w	r3, r3, r8
3000b884:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000b888:	e792      	b.n	3000b7b0 <__udivmoddi4+0xb8>
3000b88a:	4661      	mov	r1, ip
3000b88c:	e7e2      	b.n	3000b854 <__udivmoddi4+0x15c>
3000b88e:	4662      	mov	r2, ip
3000b890:	e7f6      	b.n	3000b880 <__udivmoddi4+0x188>
3000b892:	4660      	mov	r0, ip
3000b894:	e7a3      	b.n	3000b7de <__udivmoddi4+0xe6>
3000b896:	4663      	mov	r3, ip
3000b898:	e7b7      	b.n	3000b80a <__udivmoddi4+0x112>
3000b89a:	4543      	cmp	r3, r8
3000b89c:	d905      	bls.n	3000b8aa <__udivmoddi4+0x1b2>
3000b89e:	b10e      	cbz	r6, 3000b8a4 <__udivmoddi4+0x1ac>
3000b8a0:	e9c6 0800 	strd	r0, r8, [r6]
3000b8a4:	2100      	movs	r1, #0
3000b8a6:	4608      	mov	r0, r1
3000b8a8:	e776      	b.n	3000b798 <__udivmoddi4+0xa0>
3000b8aa:	fab3 fc83 	clz	ip, r3
3000b8ae:	f1bc 0f00 	cmp.w	ip, #0
3000b8b2:	d110      	bne.n	3000b8d6 <__udivmoddi4+0x1de>
3000b8b4:	4543      	cmp	r3, r8
3000b8b6:	d301      	bcc.n	3000b8bc <__udivmoddi4+0x1c4>
3000b8b8:	4282      	cmp	r2, r0
3000b8ba:	d80a      	bhi.n	3000b8d2 <__udivmoddi4+0x1da>
3000b8bc:	1a84      	subs	r4, r0, r2
3000b8be:	eb68 0303 	sbc.w	r3, r8, r3
3000b8c2:	2001      	movs	r0, #1
3000b8c4:	469e      	mov	lr, r3
3000b8c6:	2e00      	cmp	r6, #0
3000b8c8:	d068      	beq.n	3000b99c <__udivmoddi4+0x2a4>
3000b8ca:	e9c6 4e00 	strd	r4, lr, [r6]
3000b8ce:	2100      	movs	r1, #0
3000b8d0:	e762      	b.n	3000b798 <__udivmoddi4+0xa0>
3000b8d2:	4660      	mov	r0, ip
3000b8d4:	e7f7      	b.n	3000b8c6 <__udivmoddi4+0x1ce>
3000b8d6:	f1cc 0e20 	rsb	lr, ip, #32
3000b8da:	fa03 f30c 	lsl.w	r3, r3, ip
3000b8de:	fa02 f50c 	lsl.w	r5, r2, ip
3000b8e2:	fa00 f70c 	lsl.w	r7, r0, ip
3000b8e6:	fa22 f40e 	lsr.w	r4, r2, lr
3000b8ea:	fa28 fb0e 	lsr.w	fp, r8, lr
3000b8ee:	fa20 f10e 	lsr.w	r1, r0, lr
3000b8f2:	431c      	orrs	r4, r3
3000b8f4:	fa08 f30c 	lsl.w	r3, r8, ip
3000b8f8:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000b8fc:	4319      	orrs	r1, r3
3000b8fe:	b2a3      	uxth	r3, r4
3000b900:	0c0a      	lsrs	r2, r1, #16
3000b902:	fbbb f8f9 	udiv	r8, fp, r9
3000b906:	fb09 bb18 	mls	fp, r9, r8, fp
3000b90a:	fb08 fa03 	mul.w	sl, r8, r3
3000b90e:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000b912:	4592      	cmp	sl, r2
3000b914:	d93e      	bls.n	3000b994 <__udivmoddi4+0x29c>
3000b916:	18a2      	adds	r2, r4, r2
3000b918:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000b91c:	d204      	bcs.n	3000b928 <__udivmoddi4+0x230>
3000b91e:	4592      	cmp	sl, r2
3000b920:	d902      	bls.n	3000b928 <__udivmoddi4+0x230>
3000b922:	f1a8 0002 	sub.w	r0, r8, #2
3000b926:	4422      	add	r2, r4
3000b928:	eba2 020a 	sub.w	r2, r2, sl
3000b92c:	b289      	uxth	r1, r1
3000b92e:	fbb2 f8f9 	udiv	r8, r2, r9
3000b932:	fb09 2218 	mls	r2, r9, r8, r2
3000b936:	fb08 f303 	mul.w	r3, r8, r3
3000b93a:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000b93e:	4293      	cmp	r3, r2
3000b940:	d92a      	bls.n	3000b998 <__udivmoddi4+0x2a0>
3000b942:	18a2      	adds	r2, r4, r2
3000b944:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000b948:	d204      	bcs.n	3000b954 <__udivmoddi4+0x25c>
3000b94a:	4293      	cmp	r3, r2
3000b94c:	d902      	bls.n	3000b954 <__udivmoddi4+0x25c>
3000b94e:	f1a8 0102 	sub.w	r1, r8, #2
3000b952:	4422      	add	r2, r4
3000b954:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b958:	1ad2      	subs	r2, r2, r3
3000b95a:	fba0 9805 	umull	r9, r8, r0, r5
3000b95e:	4542      	cmp	r2, r8
3000b960:	464b      	mov	r3, r9
3000b962:	4641      	mov	r1, r8
3000b964:	d302      	bcc.n	3000b96c <__udivmoddi4+0x274>
3000b966:	d106      	bne.n	3000b976 <__udivmoddi4+0x27e>
3000b968:	454f      	cmp	r7, r9
3000b96a:	d204      	bcs.n	3000b976 <__udivmoddi4+0x27e>
3000b96c:	3801      	subs	r0, #1
3000b96e:	ebb9 0305 	subs.w	r3, r9, r5
3000b972:	eb68 0104 	sbc.w	r1, r8, r4
3000b976:	b18e      	cbz	r6, 3000b99c <__udivmoddi4+0x2a4>
3000b978:	1afc      	subs	r4, r7, r3
3000b97a:	eb62 0701 	sbc.w	r7, r2, r1
3000b97e:	fa07 fe0e 	lsl.w	lr, r7, lr
3000b982:	fa24 f30c 	lsr.w	r3, r4, ip
3000b986:	fa27 f70c 	lsr.w	r7, r7, ip
3000b98a:	ea4e 0303 	orr.w	r3, lr, r3
3000b98e:	e9c6 3700 	strd	r3, r7, [r6]
3000b992:	e79c      	b.n	3000b8ce <__udivmoddi4+0x1d6>
3000b994:	4640      	mov	r0, r8
3000b996:	e7c7      	b.n	3000b928 <__udivmoddi4+0x230>
3000b998:	4641      	mov	r1, r8
3000b99a:	e7db      	b.n	3000b954 <__udivmoddi4+0x25c>
3000b99c:	4631      	mov	r1, r6
3000b99e:	e6fb      	b.n	3000b798 <__udivmoddi4+0xa0>

3000b9a0 <__aeabi_idiv0>:
3000b9a0:	4770      	bx	lr
3000b9a2:	bf00      	nop

3000b9a4 <_mbtowc_r>:
3000b9a4:	b430      	push	{r4, r5}
3000b9a6:	4c06      	ldr	r4, [pc, #24]	; (3000b9c0 <_mbtowc_r+0x1c>)
3000b9a8:	4d06      	ldr	r5, [pc, #24]	; (3000b9c4 <_mbtowc_r+0x20>)
3000b9aa:	6824      	ldr	r4, [r4, #0]
3000b9ac:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b9ae:	2c00      	cmp	r4, #0
3000b9b0:	bf08      	it	eq
3000b9b2:	462c      	moveq	r4, r5
3000b9b4:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000b9b8:	46a4      	mov	ip, r4
3000b9ba:	bc30      	pop	{r4, r5}
3000b9bc:	4760      	bx	ip
3000b9be:	bf00      	nop
3000b9c0:	3000ef60 	.word	0x3000ef60
3000b9c4:	3000edf0 	.word	0x3000edf0

3000b9c8 <__ascii_mbtowc>:
3000b9c8:	b082      	sub	sp, #8
3000b9ca:	b901      	cbnz	r1, 3000b9ce <__ascii_mbtowc+0x6>
3000b9cc:	a901      	add	r1, sp, #4
3000b9ce:	b142      	cbz	r2, 3000b9e2 <__ascii_mbtowc+0x1a>
3000b9d0:	b14b      	cbz	r3, 3000b9e6 <__ascii_mbtowc+0x1e>
3000b9d2:	7813      	ldrb	r3, [r2, #0]
3000b9d4:	600b      	str	r3, [r1, #0]
3000b9d6:	7812      	ldrb	r2, [r2, #0]
3000b9d8:	1e10      	subs	r0, r2, #0
3000b9da:	bf18      	it	ne
3000b9dc:	2001      	movne	r0, #1
3000b9de:	b002      	add	sp, #8
3000b9e0:	4770      	bx	lr
3000b9e2:	4610      	mov	r0, r2
3000b9e4:	e7fb      	b.n	3000b9de <__ascii_mbtowc+0x16>
3000b9e6:	f06f 0001 	mvn.w	r0, #1
3000b9ea:	e7f8      	b.n	3000b9de <__ascii_mbtowc+0x16>

3000b9ec <_wctomb_r>:
3000b9ec:	b430      	push	{r4, r5}
3000b9ee:	4c06      	ldr	r4, [pc, #24]	; (3000ba08 <_wctomb_r+0x1c>)
3000b9f0:	4d06      	ldr	r5, [pc, #24]	; (3000ba0c <_wctomb_r+0x20>)
3000b9f2:	6824      	ldr	r4, [r4, #0]
3000b9f4:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b9f6:	2c00      	cmp	r4, #0
3000b9f8:	bf08      	it	eq
3000b9fa:	462c      	moveq	r4, r5
3000b9fc:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000ba00:	46a4      	mov	ip, r4
3000ba02:	bc30      	pop	{r4, r5}
3000ba04:	4760      	bx	ip
3000ba06:	bf00      	nop
3000ba08:	3000ef60 	.word	0x3000ef60
3000ba0c:	3000edf0 	.word	0x3000edf0

3000ba10 <__ascii_wctomb>:
3000ba10:	b149      	cbz	r1, 3000ba26 <__ascii_wctomb+0x16>
3000ba12:	2aff      	cmp	r2, #255	; 0xff
3000ba14:	bf8d      	iteet	hi
3000ba16:	238a      	movhi	r3, #138	; 0x8a
3000ba18:	2001      	movls	r0, #1
3000ba1a:	700a      	strbls	r2, [r1, #0]
3000ba1c:	6003      	strhi	r3, [r0, #0]
3000ba1e:	bf88      	it	hi
3000ba20:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000ba24:	4770      	bx	lr
3000ba26:	4608      	mov	r0, r1
3000ba28:	4770      	bx	lr
3000ba2a:	bf00      	nop
3000ba2c:	0000      	movs	r0, r0
	...

3000ba30 <__SBOOT_Validate_Signature_A_veneer>:
3000ba30:	b401      	push	{r0}
3000ba32:	4802      	ldr	r0, [pc, #8]	; (3000ba3c <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000ba34:	4684      	mov	ip, r0
3000ba36:	bc01      	pop	{r0}
3000ba38:	4760      	bx	ip
3000ba3a:	bf00      	nop
3000ba3c:	000032bd 	.word	0x000032bd

3000ba40 <__rtl_crypto_sha2_init_A_veneer>:
3000ba40:	b401      	push	{r0}
3000ba42:	4802      	ldr	r0, [pc, #8]	; (3000ba4c <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000ba44:	4684      	mov	ip, r0
3000ba46:	bc01      	pop	{r0}
3000ba48:	4760      	bx	ip
3000ba4a:	bf00      	nop
3000ba4c:	00004e89 	.word	0x00004e89

3000ba50 <__DiagVSprintf_veneer>:
3000ba50:	b401      	push	{r0}
3000ba52:	4802      	ldr	r0, [pc, #8]	; (3000ba5c <__DiagVSprintf_veneer+0xc>)
3000ba54:	4684      	mov	ip, r0
3000ba56:	bc01      	pop	{r0}
3000ba58:	4760      	bx	ip
3000ba5a:	bf00      	nop
3000ba5c:	0000e331 	.word	0x0000e331

3000ba60 <__SBOOT_Validate_Algorithm_B_veneer>:
3000ba60:	b401      	push	{r0}
3000ba62:	4802      	ldr	r0, [pc, #8]	; (3000ba6c <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000ba64:	4684      	mov	ip, r0
3000ba66:	bc01      	pop	{r0}
3000ba68:	4760      	bx	ip
3000ba6a:	bf00      	nop
3000ba6c:	000033ed 	.word	0x000033ed

3000ba70 <__CRYPTO_SHA_Init_B_veneer>:
3000ba70:	b401      	push	{r0}
3000ba72:	4802      	ldr	r0, [pc, #8]	; (3000ba7c <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000ba74:	4684      	mov	ip, r0
3000ba76:	bc01      	pop	{r0}
3000ba78:	4760      	bx	ip
3000ba7a:	bf00      	nop
3000ba7c:	00002451 	.word	0x00002451

3000ba80 <__SBOOT_Validate_PubKey_A_veneer>:
3000ba80:	b401      	push	{r0}
3000ba82:	4802      	ldr	r0, [pc, #8]	; (3000ba8c <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000ba84:	4684      	mov	ip, r0
3000ba86:	bc01      	pop	{r0}
3000ba88:	4760      	bx	ip
3000ba8a:	bf00      	nop
3000ba8c:	00003321 	.word	0x00003321

3000ba90 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000ba90:	b401      	push	{r0}
3000ba92:	4802      	ldr	r0, [pc, #8]	; (3000ba9c <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000ba94:	4684      	mov	ip, r0
3000ba96:	bc01      	pop	{r0}
3000ba98:	4760      	bx	ip
3000ba9a:	bf00      	nop
3000ba9c:	00005bb9 	.word	0x00005bb9

3000baa0 <__SBOOT_Validate_Algorithm_A_veneer>:
3000baa0:	b401      	push	{r0}
3000baa2:	4802      	ldr	r0, [pc, #8]	; (3000baac <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000baa4:	4684      	mov	ip, r0
3000baa6:	bc01      	pop	{r0}
3000baa8:	4760      	bx	ip
3000baaa:	bf00      	nop
3000baac:	00003279 	.word	0x00003279

3000bab0 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000bab0:	b401      	push	{r0}
3000bab2:	4802      	ldr	r0, [pc, #8]	; (3000babc <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000bab4:	4684      	mov	ip, r0
3000bab6:	bc01      	pop	{r0}
3000bab8:	4760      	bx	ip
3000baba:	bf00      	nop
3000babc:	00005a45 	.word	0x00005a45

3000bac0 <__SBOOT_GetMdType_A_veneer>:
3000bac0:	b401      	push	{r0}
3000bac2:	4802      	ldr	r0, [pc, #8]	; (3000bacc <__SBOOT_GetMdType_A_veneer+0xc>)
3000bac4:	4684      	mov	ip, r0
3000bac6:	bc01      	pop	{r0}
3000bac8:	4760      	bx	ip
3000baca:	bf00      	nop
3000bacc:	00003029 	.word	0x00003029

3000bad0 <____wrap_memcpy_veneer>:
3000bad0:	b401      	push	{r0}
3000bad2:	4802      	ldr	r0, [pc, #8]	; (3000badc <____wrap_memcpy_veneer+0xc>)
3000bad4:	4684      	mov	ip, r0
3000bad6:	bc01      	pop	{r0}
3000bad8:	4760      	bx	ip
3000bada:	bf00      	nop
3000badc:	00012f05 	.word	0x00012f05

3000bae0 <__SBOOT_GetMdType_B_veneer>:
3000bae0:	b401      	push	{r0}
3000bae2:	4802      	ldr	r0, [pc, #8]	; (3000baec <__SBOOT_GetMdType_B_veneer+0xc>)
3000bae4:	4684      	mov	ip, r0
3000bae6:	bc01      	pop	{r0}
3000bae8:	4760      	bx	ip
3000baea:	bf00      	nop
3000baec:	0000319d 	.word	0x0000319d

3000baf0 <__DiagPrintf_veneer>:
3000baf0:	b401      	push	{r0}
3000baf2:	4802      	ldr	r0, [pc, #8]	; (3000bafc <__DiagPrintf_veneer+0xc>)
3000baf4:	4684      	mov	ip, r0
3000baf6:	bc01      	pop	{r0}
3000baf8:	4760      	bx	ip
3000bafa:	bf00      	nop
3000bafc:	0000e5f9 	.word	0x0000e5f9

3000bb00 <__rtl_crypto_sha2_update_B_veneer>:
3000bb00:	b401      	push	{r0}
3000bb02:	4802      	ldr	r0, [pc, #8]	; (3000bb0c <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000bb04:	4684      	mov	ip, r0
3000bb06:	bc01      	pop	{r0}
3000bb08:	4760      	bx	ip
3000bb0a:	bf00      	nop
3000bb0c:	00005385 	.word	0x00005385

3000bb10 <__SBOOT_Validate_Signature_B_veneer>:
3000bb10:	b401      	push	{r0}
3000bb12:	4802      	ldr	r0, [pc, #8]	; (3000bb1c <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000bb14:	4684      	mov	ip, r0
3000bb16:	bc01      	pop	{r0}
3000bb18:	4760      	bx	ip
3000bb1a:	bf00      	nop
3000bb1c:	00003431 	.word	0x00003431

3000bb20 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000bb20:	b401      	push	{r0}
3000bb22:	4802      	ldr	r0, [pc, #8]	; (3000bb2c <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000bb24:	4684      	mov	ip, r0
3000bb26:	bc01      	pop	{r0}
3000bb28:	4760      	bx	ip
3000bb2a:	bf00      	nop
3000bb2c:	00005b4d 	.word	0x00005b4d

3000bb30 <__SBOOT_Validate_ImgHash_B_veneer>:
3000bb30:	b401      	push	{r0}
3000bb32:	4802      	ldr	r0, [pc, #8]	; (3000bb3c <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000bb34:	4684      	mov	ip, r0
3000bb36:	bc01      	pop	{r0}
3000bb38:	4760      	bx	ip
3000bb3a:	bf00      	nop
3000bb3c:	00003541 	.word	0x00003541

3000bb40 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000bb40:	b401      	push	{r0}
3000bb42:	4802      	ldr	r0, [pc, #8]	; (3000bb4c <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000bb44:	4684      	mov	ip, r0
3000bb46:	bc01      	pop	{r0}
3000bb48:	4760      	bx	ip
3000bb4a:	bf00      	nop
3000bb4c:	000059d9 	.word	0x000059d9

3000bb50 <__SBOOT_Validate_ImgHash_A_veneer>:
3000bb50:	b401      	push	{r0}
3000bb52:	4802      	ldr	r0, [pc, #8]	; (3000bb5c <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000bb54:	4684      	mov	ip, r0
3000bb56:	bc01      	pop	{r0}
3000bb58:	4760      	bx	ip
3000bb5a:	bf00      	nop
3000bb5c:	000033cd 	.word	0x000033cd

3000bb60 <__rtl_crypto_sha2_update_A_veneer>:
3000bb60:	b401      	push	{r0}
3000bb62:	4802      	ldr	r0, [pc, #8]	; (3000bb6c <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000bb64:	4684      	mov	ip, r0
3000bb66:	bc01      	pop	{r0}
3000bb68:	4760      	bx	ip
3000bb6a:	bf00      	nop
3000bb6c:	00005211 	.word	0x00005211

3000bb70 <__io_assert_failed_veneer>:
3000bb70:	b401      	push	{r0}
3000bb72:	4802      	ldr	r0, [pc, #8]	; (3000bb7c <__io_assert_failed_veneer+0xc>)
3000bb74:	4684      	mov	ip, r0
3000bb76:	bc01      	pop	{r0}
3000bb78:	4760      	bx	ip
3000bb7a:	bf00      	nop
3000bb7c:	0000992d 	.word	0x0000992d

3000bb80 <__INT_HardFault_C_veneer>:
3000bb80:	b401      	push	{r0}
3000bb82:	4802      	ldr	r0, [pc, #8]	; (3000bb8c <__INT_HardFault_C_veneer+0xc>)
3000bb84:	4684      	mov	ip, r0
3000bb86:	bc01      	pop	{r0}
3000bb88:	4760      	bx	ip
3000bb8a:	bf00      	nop
3000bb8c:	0000d8ed 	.word	0x0000d8ed

3000bb90 <__SBOOT_Validate_PubKey_B_veneer>:
3000bb90:	b401      	push	{r0}
3000bb92:	4802      	ldr	r0, [pc, #8]	; (3000bb9c <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000bb94:	4684      	mov	ip, r0
3000bb96:	bc01      	pop	{r0}
3000bb98:	4760      	bx	ip
3000bb9a:	bf00      	nop
3000bb9c:	00003495 	.word	0x00003495

3000bba0 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000bba0:	b401      	push	{r0}
3000bba2:	4802      	ldr	r0, [pc, #8]	; (3000bbac <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000bba4:	4684      	mov	ip, r0
3000bba6:	bc01      	pop	{r0}
3000bba8:	4760      	bx	ip
3000bbaa:	bf00      	nop
3000bbac:	00002369 	.word	0x00002369

3000bbb0 <__rtl_crypto_sha2_final_B_veneer>:
3000bbb0:	b401      	push	{r0}
3000bbb2:	4802      	ldr	r0, [pc, #8]	; (3000bbbc <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000bbb4:	4684      	mov	ip, r0
3000bbb6:	bc01      	pop	{r0}
3000bbb8:	4760      	bx	ip
3000bbba:	bf00      	nop
3000bbbc:	000053f1 	.word	0x000053f1

3000bbc0 <____wrap_memset_veneer>:
3000bbc0:	b401      	push	{r0}
3000bbc2:	4802      	ldr	r0, [pc, #8]	; (3000bbcc <____wrap_memset_veneer+0xc>)
3000bbc4:	4684      	mov	ip, r0
3000bbc6:	bc01      	pop	{r0}
3000bbc8:	4760      	bx	ip
3000bbca:	bf00      	nop
3000bbcc:	00012efd 	.word	0x00012efd

3000bbd0 <__CRYPTO_SHA_Init_A_veneer>:
3000bbd0:	b401      	push	{r0}
3000bbd2:	4802      	ldr	r0, [pc, #8]	; (3000bbdc <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000bbd4:	4684      	mov	ip, r0
3000bbd6:	bc01      	pop	{r0}
3000bbd8:	4760      	bx	ip
3000bbda:	bf00      	nop
3000bbdc:	000022dd 	.word	0x000022dd

3000bbe0 <__rtl_crypto_sha2_final_A_veneer>:
3000bbe0:	b401      	push	{r0}
3000bbe2:	4802      	ldr	r0, [pc, #8]	; (3000bbec <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000bbe4:	4684      	mov	ip, r0
3000bbe6:	bc01      	pop	{r0}
3000bbe8:	4760      	bx	ip
3000bbea:	bf00      	nop
3000bbec:	0000527d 	.word	0x0000527d

3000bbf0 <__rtl_crypto_sha2_init_B_veneer>:
3000bbf0:	b401      	push	{r0}
3000bbf2:	4802      	ldr	r0, [pc, #8]	; (3000bbfc <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000bbf4:	4684      	mov	ip, r0
3000bbf6:	bc01      	pop	{r0}
3000bbf8:	4760      	bx	ip
3000bbfa:	bf00      	nop
3000bbfc:	00004ffd 	.word	0x00004ffd

3000bc00 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000bc00:	b401      	push	{r0}
3000bc02:	4802      	ldr	r0, [pc, #8]	; (3000bc0c <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000bc04:	4684      	mov	ip, r0
3000bc06:	bc01      	pop	{r0}
3000bc08:	4760      	bx	ip
3000bc0a:	bf00      	nop
3000bc0c:	000024dd 	.word	0x000024dd
3000bc10:	004f4931 	.word	0x004f4931
3000bc14:	00004f32 	.word	0x00004f32
3000bc18:	004f4932 	.word	0x004f4932
3000bc1c:	00004f34 	.word	0x00004f34
3000bc20:	004f4934 	.word	0x004f4934
3000bc24:	53414c46 	.word	0x53414c46
3000bc28:	4b4c4348 	.word	0x4b4c4348
3000bc2c:	00000000 	.word	0x00000000
3000bc30:	2d73255b 	.word	0x2d73255b
3000bc34:	63205d49 	.word	0x63205d49
3000bc38:	62696c61 	.word	0x62696c61
3000bc3c:	69746172 	.word	0x69746172
3000bc40:	6f5f6e6f 	.word	0x6f5f6e6f
3000bc44:	255b3a6b 	.word	0x255b3a6b
3000bc48:	64253a64 	.word	0x64253a64
3000bc4c:	5d64253a 	.word	0x5d64253a
3000bc50:	00000a20 	.word	0x00000a20
3000bc54:	2d73255b 	.word	0x2d73255b
3000bc58:	46205d49 	.word	0x46205d49
3000bc5c:	4853414c 	.word	0x4853414c
3000bc60:	4c414320 	.word	0x4c414320
3000bc64:	305b4249 	.word	0x305b4249
3000bc68:	20782578 	.word	0x20782578
3000bc6c:	0a5d4b4f 	.word	0x0a5d4b4f
3000bc70:	00000000 	.word	0x00000000
3000bc74:	2d73255b 	.word	0x2d73255b
3000bc78:	46205d45 	.word	0x46205d45
3000bc7c:	4853414c 	.word	0x4853414c
3000bc80:	4c414320 	.word	0x4c414320
3000bc84:	305b4249 	.word	0x305b4249
3000bc88:	20782578 	.word	0x20782578
3000bc8c:	4c494146 	.word	0x4c494146
3000bc90:	00000a5d 	.word	0x00000a5d
3000bc94:	2d73255b 	.word	0x2d73255b
3000bc98:	46205d49 	.word	0x46205d49
3000bc9c:	6873616c 	.word	0x6873616c
3000bca0:	61655220 	.word	0x61655220
3000bca4:	73252064 	.word	0x73252064
3000bca8:	0000000a 	.word	0x0000000a
3000bcac:	2d73255b 	.word	0x2d73255b
3000bcb0:	46205d45 	.word	0x46205d45
3000bcb4:	6873616c 	.word	0x6873616c
3000bcb8:	69775320 	.word	0x69775320
3000bcbc:	20686374 	.word	0x20686374
3000bcc0:	64616552 	.word	0x64616552
3000bcc4:	646f4d20 	.word	0x646f4d20
3000bcc8:	41462065 	.word	0x41462065
3000bccc:	000a4c49 	.word	0x000a4c49
3000bcd0:	2d73255b 	.word	0x2d73255b
3000bcd4:	46205d49 	.word	0x46205d49
3000bcd8:	6873616c 	.word	0x6873616c
3000bcdc:	3a444920 	.word	0x3a444920
3000bce0:	2d782520 	.word	0x2d782520
3000bce4:	252d7825 	.word	0x252d7825
3000bce8:	00000a78 	.word	0x00000a78
3000bcec:	2d73255b 	.word	0x2d73255b
3000bcf0:	54205d57 	.word	0x54205d57
3000bcf4:	20736968 	.word	0x20736968
3000bcf8:	73616c66 	.word	0x73616c66
3000bcfc:	79742068 	.word	0x79742068
3000bd00:	69206570 	.word	0x69206570
3000bd04:	6f6e2073 	.word	0x6f6e2073
3000bd08:	75732074 	.word	0x75732074
3000bd0c:	726f7070 	.word	0x726f7070
3000bd10:	21646574 	.word	0x21646574
3000bd14:	0000000a 	.word	0x0000000a
3000bd18:	2d73255b 	.word	0x2d73255b
3000bd1c:	4e205d49 	.word	0x4e205d49
3000bd20:	20444e41 	.word	0x20444e41
3000bd24:	203a4449 	.word	0x203a4449
3000bd28:	252d7825 	.word	0x252d7825
3000bd2c:	00000a78 	.word	0x00000a78
3000bd30:	00003552 	.word	0x00003552
3000bd34:	00003652 	.word	0x00003652
3000bd38:	00003752 	.word	0x00003752
3000bd3c:	00003852 	.word	0x00003852
3000bd40:	00003952 	.word	0x00003952
3000bd44:	00303152 	.word	0x00303152
3000bd48:	00313152 	.word	0x00313152
3000bd4c:	00003152 	.word	0x00003152
3000bd50:	00003252 	.word	0x00003252
3000bd54:	00003352 	.word	0x00003352
3000bd58:	544f4f42 	.word	0x544f4f42
3000bd5c:	00000000 	.word	0x00000000
3000bd60:	2d73255b 	.word	0x2d73255b
3000bd64:	48205d41 	.word	0x48205d41
3000bd68:	20647261 	.word	0x20647261
3000bd6c:	6c756146 	.word	0x6c756146
3000bd70:	61502074 	.word	0x61502074
3000bd74:	20686374 	.word	0x20686374
3000bd78:	63655328 	.word	0x63655328
3000bd7c:	29657275 	.word	0x29657275
3000bd80:	00000a0d 	.word	0x00000a0d
3000bd84:	2d73255b 	.word	0x2d73255b
3000bd88:	45205d41 	.word	0x45205d41
3000bd8c:	70656378 	.word	0x70656378
3000bd90:	6e6f6974 	.word	0x6e6f6974
3000bd94:	6f726620 	.word	0x6f726620
3000bd98:	6573206d 	.word	0x6573206d
3000bd9c:	65727563 	.word	0x65727563
3000bda0:	726f7720 	.word	0x726f7720
3000bda4:	0d21646c 	.word	0x0d21646c
3000bda8:	0000000a 	.word	0x0000000a
3000bdac:	2d73255b 	.word	0x2d73255b
3000bdb0:	45205d41 	.word	0x45205d41
3000bdb4:	70656378 	.word	0x70656378
3000bdb8:	6e6f6974 	.word	0x6e6f6974
3000bdbc:	6f726620 	.word	0x6f726620
3000bdc0:	6f6e206d 	.word	0x6f6e206d
3000bdc4:	65732d6e 	.word	0x65732d6e
3000bdc8:	65727563 	.word	0x65727563
3000bdcc:	726f7720 	.word	0x726f7720
3000bdd0:	0d21646c 	.word	0x0d21646c
3000bdd4:	0000000a 	.word	0x0000000a
3000bdd8:	2d73255b 	.word	0x2d73255b
3000bddc:	53205d41 	.word	0x53205d41
3000bde0:	20525346 	.word	0x20525346
3000bde4:	30203d20 	.word	0x30203d20
3000bde8:	38302578 	.word	0x38302578
3000bdec:	000a0d78 	.word	0x000a0d78
3000bdf0:	2d73255b 	.word	0x2d73255b
3000bdf4:	53205d41 	.word	0x53205d41
3000bdf8:	20524146 	.word	0x20524146
3000bdfc:	30203d20 	.word	0x30203d20
3000be00:	38302578 	.word	0x38302578
3000be04:	000a0d78 	.word	0x000a0d78
3000be08:	2d73255b 	.word	0x2d73255b
3000be0c:	53205d41 	.word	0x53205d41
3000be10:	52534348 	.word	0x52534348
3000be14:	30203d20 	.word	0x30203d20
3000be18:	38302578 	.word	0x38302578
3000be1c:	000a0d78 	.word	0x000a0d78
3000be20:	2d73255b 	.word	0x2d73255b
3000be24:	42205d41 	.word	0x42205d41
3000be28:	5f50554b 	.word	0x5f50554b
3000be2c:	32474552 	.word	0x32474552
3000be30:	25783020 	.word	0x25783020
3000be34:	0a0d2078 	.word	0x0a0d2078
3000be38:	00000000 	.word	0x00000000
3000be3c:	4f4b4e55 	.word	0x4f4b4e55
3000be40:	00004e57 	.word	0x00004e57
3000be44:	203a7325 	.word	0x203a7325
3000be48:	6f636552 	.word	0x6f636552
3000be4c:	79726576 	.word	0x79726576
3000be50:	616d6920 	.word	0x616d6920
3000be54:	43206567 	.word	0x43206567
3000be58:	63204352 	.word	0x63204352
3000be5c:	6b636568 	.word	0x6b636568
3000be60:	69616620 	.word	0x69616620
3000be64:	72202c6c 	.word	0x72202c6c
3000be68:	79727465 	.word	0x79727465
3000be6c:	6d697420 	.word	0x6d697420
3000be70:	203a7365 	.word	0x203a7365
3000be74:	000a6425 	.word	0x000a6425
3000be78:	203a7325 	.word	0x203a7325
3000be7c:	6f636552 	.word	0x6f636552
3000be80:	79726576 	.word	0x79726576
3000be84:	616d6920 	.word	0x616d6920
3000be88:	43206567 	.word	0x43206567
3000be8c:	63204352 	.word	0x63204352
3000be90:	6b636568 	.word	0x6b636568
3000be94:	69616620 	.word	0x69616620
3000be98:	2064656c 	.word	0x2064656c
3000be9c:	65746661 	.word	0x65746661
3000bea0:	20332072 	.word	0x20332072
3000bea4:	72746572 	.word	0x72746572
3000bea8:	0a736569 	.word	0x0a736569
3000beac:	00000000 	.word	0x00000000
3000beb0:	6f636552 	.word	0x6f636552
3000beb4:	79726576 	.word	0x79726576
3000beb8:	616d6920 	.word	0x616d6920
3000bebc:	73206567 	.word	0x73206567
3000bec0:	63656c65 	.word	0x63656c65
3000bec4:	20646574 	.word	0x20646574
3000bec8:	66207369 	.word	0x66207369
3000becc:	6f746361 	.word	0x6f746361
3000bed0:	62207972 	.word	0x62207972
3000bed4:	72616e69 	.word	0x72616e69
3000bed8:	72202c79 	.word	0x72202c79
3000bedc:	766f6365 	.word	0x766f6365
3000bee0:	20797265 	.word	0x20797265
3000bee4:	6c696166 	.word	0x6c696166
3000bee8:	202e6465 	.word	0x202e6465
3000beec:	6f626552 	.word	0x6f626552
3000bef0:	6220746f 	.word	0x6220746f
3000bef4:	61772079 	.word	0x61772079
3000bef8:	64686374 	.word	0x64686374
3000befc:	0a0d676f 	.word	0x0a0d676f
3000bf00:	00000000 	.word	0x00000000
3000bf04:	3141544f 	.word	0x3141544f
3000bf08:	45535520 	.word	0x45535520
3000bf0c:	6576202c 	.word	0x6576202c
3000bf10:	6f697372 	.word	0x6f697372
3000bf14:	25203a6e 	.word	0x25203a6e
3000bf18:	00000a64 	.word	0x00000a64
3000bf1c:	3241544f 	.word	0x3241544f
3000bf20:	45535520 	.word	0x45535520
3000bf24:	6576202c 	.word	0x6576202c
3000bf28:	6f697372 	.word	0x6f697372
3000bf2c:	25203a6e 	.word	0x25203a6e
3000bf30:	00000a64 	.word	0x00000a64
3000bf34:	2d73255b 	.word	0x2d73255b
3000bf38:	49205d49 	.word	0x49205d49
3000bf3c:	2074696e 	.word	0x2074696e
3000bf40:	0a0d4257 	.word	0x0a0d4257
3000bf44:	00000000 	.word	0x00000000
3000bf48:	73616c66 	.word	0x73616c66
3000bf4c:	69735f68 	.word	0x69735f68
3000bf50:	203a657a 	.word	0x203a657a
3000bf54:	000a4d38 	.word	0x000a4d38
3000bf58:	73616c66 	.word	0x73616c66
3000bf5c:	69735f68 	.word	0x69735f68
3000bf60:	203a657a 	.word	0x203a657a
3000bf64:	0a4d3631 	.word	0x0a4d3631
3000bf68:	00000000 	.word	0x00000000
3000bf6c:	73616c66 	.word	0x73616c66
3000bf70:	69735f68 	.word	0x69735f68
3000bf74:	203a657a 	.word	0x203a657a
3000bf78:	0a4d3233 	.word	0x0a4d3233
3000bf7c:	00000000 	.word	0x00000000
3000bf80:	20315042 	.word	0x20315042
3000bf84:	73726576 	.word	0x73726576
3000bf88:	206e6f69 	.word	0x206e6f69
3000bf8c:	61766e69 	.word	0x61766e69
3000bf90:	0a64696c 	.word	0x0a64696c
3000bf94:	00000000 	.word	0x00000000
3000bf98:	20315042 	.word	0x20315042
3000bf9c:	6d726f66 	.word	0x6d726f66
3000bfa0:	69207461 	.word	0x69207461
3000bfa4:	6c61766e 	.word	0x6c61766e
3000bfa8:	000a6469 	.word	0x000a6469
3000bfac:	20315042 	.word	0x20315042
3000bfb0:	69746361 	.word	0x69746361
3000bfb4:	69206576 	.word	0x69206576
3000bfb8:	7865646e 	.word	0x7865646e
3000bfbc:	766e6920 	.word	0x766e6920
3000bfc0:	64696c61 	.word	0x64696c61
3000bfc4:	0000000a 	.word	0x0000000a
3000bfc8:	20315042 	.word	0x20315042
3000bfcc:	61746164 	.word	0x61746164
3000bfd0:	6c617620 	.word	0x6c617620
3000bfd4:	202c6469 	.word	0x202c6469
3000bfd8:	73726576 	.word	0x73726576
3000bfdc:	3a6e6f69 	.word	0x3a6e6f69
3000bfe0:	000a6425 	.word	0x000a6425
3000bfe4:	20325042 	.word	0x20325042
3000bfe8:	6d726f66 	.word	0x6d726f66
3000bfec:	69207461 	.word	0x69207461
3000bff0:	6c61766e 	.word	0x6c61766e
3000bff4:	000a6469 	.word	0x000a6469
3000bff8:	20315042 	.word	0x20315042
3000bffc:	33435243 	.word	0x33435243
3000c000:	616d2032 	.word	0x616d2032
3000c004:	2c686374 	.word	0x2c686374
3000c008:	74746120 	.word	0x74746120
3000c00c:	65686361 	.word	0x65686361
3000c010:	52432064 	.word	0x52432064
3000c014:	30203a43 	.word	0x30203a43
3000c018:	2c782578 	.word	0x2c782578
3000c01c:	6c616320 	.word	0x6c616320
3000c020:	616c7563 	.word	0x616c7563
3000c024:	20646574 	.word	0x20646574
3000c028:	3a435243 	.word	0x3a435243
3000c02c:	25783020 	.word	0x25783020
3000c030:	00000a78 	.word	0x00000a78
3000c034:	20315042 	.word	0x20315042
3000c038:	33435243 	.word	0x33435243
3000c03c:	6f6e2032 	.word	0x6f6e2032
3000c040:	616d2074 	.word	0x616d2074
3000c044:	2c686374 	.word	0x2c686374
3000c048:	74746120 	.word	0x74746120
3000c04c:	65686361 	.word	0x65686361
3000c050:	52432064 	.word	0x52432064
3000c054:	30203a43 	.word	0x30203a43
3000c058:	2c782578 	.word	0x2c782578
3000c05c:	6c616320 	.word	0x6c616320
3000c060:	616c7563 	.word	0x616c7563
3000c064:	20646574 	.word	0x20646574
3000c068:	3a435243 	.word	0x3a435243
3000c06c:	25783020 	.word	0x25783020
3000c070:	61662078 	.word	0x61662078
3000c074:	64656c69 	.word	0x64656c69
3000c078:	6425203a 	.word	0x6425203a
3000c07c:	0000000a 	.word	0x0000000a
3000c080:	20325042 	.word	0x20325042
3000c084:	33435243 	.word	0x33435243
3000c088:	616d2032 	.word	0x616d2032
3000c08c:	2c686374 	.word	0x2c686374
3000c090:	74746120 	.word	0x74746120
3000c094:	65686361 	.word	0x65686361
3000c098:	52432064 	.word	0x52432064
3000c09c:	30203a43 	.word	0x30203a43
3000c0a0:	2c782578 	.word	0x2c782578
3000c0a4:	6c616320 	.word	0x6c616320
3000c0a8:	616c7563 	.word	0x616c7563
3000c0ac:	20646574 	.word	0x20646574
3000c0b0:	3a435243 	.word	0x3a435243
3000c0b4:	25783020 	.word	0x25783020
3000c0b8:	00000a78 	.word	0x00000a78
3000c0bc:	20325042 	.word	0x20325042
3000c0c0:	33435243 	.word	0x33435243
3000c0c4:	6f6e2032 	.word	0x6f6e2032
3000c0c8:	616d2074 	.word	0x616d2074
3000c0cc:	2c686374 	.word	0x2c686374
3000c0d0:	74746120 	.word	0x74746120
3000c0d4:	65686361 	.word	0x65686361
3000c0d8:	52432064 	.word	0x52432064
3000c0dc:	30203a43 	.word	0x30203a43
3000c0e0:	2c782578 	.word	0x2c782578
3000c0e4:	6c616320 	.word	0x6c616320
3000c0e8:	616c7563 	.word	0x616c7563
3000c0ec:	20646574 	.word	0x20646574
3000c0f0:	3a435243 	.word	0x3a435243
3000c0f4:	25783020 	.word	0x25783020
3000c0f8:	61662078 	.word	0x61662078
3000c0fc:	64656c69 	.word	0x64656c69
3000c100:	6425203a 	.word	0x6425203a
3000c104:	0000000a 	.word	0x0000000a
3000c108:	20315042 	.word	0x20315042
3000c10c:	3161746f 	.word	0x3161746f
3000c110:	64646120 	.word	0x64646120
3000c114:	73736572 	.word	0x73736572
3000c118:	766e6920 	.word	0x766e6920
3000c11c:	64696c61 	.word	0x64696c61
3000c120:	0000000a 	.word	0x0000000a
3000c124:	20315042 	.word	0x20315042
3000c128:	3261746f 	.word	0x3261746f
3000c12c:	64646120 	.word	0x64646120
3000c130:	73736572 	.word	0x73736572
3000c134:	766e6920 	.word	0x766e6920
3000c138:	64696c61 	.word	0x64696c61
3000c13c:	0000000a 	.word	0x0000000a
3000c140:	68746f42 	.word	0x68746f42
3000c144:	20504220 	.word	0x20504220
3000c148:	20435243 	.word	0x20435243
3000c14c:	696c6176 	.word	0x696c6176
3000c150:	00000a64 	.word	0x00000a64
3000c154:	20315042 	.word	0x20315042
3000c158:	656c6573 	.word	0x656c6573
3000c15c:	64657463 	.word	0x64657463
3000c160:	0000000a 	.word	0x0000000a
3000c164:	20325042 	.word	0x20325042
3000c168:	656c6573 	.word	0x656c6573
3000c16c:	64657463 	.word	0x64657463
3000c170:	0000000a 	.word	0x0000000a
3000c174:	68746f42 	.word	0x68746f42
3000c178:	20504220 	.word	0x20504220
3000c17c:	61766e69 	.word	0x61766e69
3000c180:	0a64696c 	.word	0x0a64696c
3000c184:	00000000 	.word	0x00000000
3000c188:	6f636552 	.word	0x6f636552
3000c18c:	79726576 	.word	0x79726576
3000c190:	61747320 	.word	0x61747320
3000c194:	000a7472 	.word	0x000a7472
3000c198:	69746341 	.word	0x69746341
3000c19c:	49206576 	.word	0x49206576
3000c1a0:	7865646e 	.word	0x7865646e
3000c1a4:	6425203a 	.word	0x6425203a
3000c1a8:	0000000a 	.word	0x0000000a
3000c1ac:	3141544f 	.word	0x3141544f
3000c1b0:	766e6920 	.word	0x766e6920
3000c1b4:	64696c61 	.word	0x64696c61
3000c1b8:	0000000a 	.word	0x0000000a
3000c1bc:	7261500a 	.word	0x7261500a
3000c1c0:	69746974 	.word	0x69746974
3000c1c4:	41206e6f 	.word	0x41206e6f
3000c1c8:	20736920 	.word	0x20736920
3000c1cc:	74636166 	.word	0x74636166
3000c1d0:	2079726f 	.word	0x2079726f
3000c1d4:	616e6962 	.word	0x616e6962
3000c1d8:	202c7972 	.word	0x202c7972
3000c1dc:	6f636572 	.word	0x6f636572
3000c1e0:	65726576 	.word	0x65726576
3000c1e4:	61662064 	.word	0x61662064
3000c1e8:	64656c69 	.word	0x64656c69
3000c1ec:	6552202e 	.word	0x6552202e
3000c1f0:	746f6f62 	.word	0x746f6f62
3000c1f4:	20796220 	.word	0x20796220
3000c1f8:	63746177 	.word	0x63746177
3000c1fc:	676f6468 	.word	0x676f6468
3000c200:	0000000a 	.word	0x0000000a
3000c204:	3241544f 	.word	0x3241544f
3000c208:	766e6920 	.word	0x766e6920
3000c20c:	64696c61 	.word	0x64696c61
3000c210:	0000000a 	.word	0x0000000a
3000c214:	7261500a 	.word	0x7261500a
3000c218:	69746974 	.word	0x69746974
3000c21c:	42206e6f 	.word	0x42206e6f
3000c220:	20736920 	.word	0x20736920
3000c224:	74636166 	.word	0x74636166
3000c228:	2079726f 	.word	0x2079726f
3000c22c:	616e6962 	.word	0x616e6962
3000c230:	202c7972 	.word	0x202c7972
3000c234:	6f636572 	.word	0x6f636572
3000c238:	65726576 	.word	0x65726576
3000c23c:	61662064 	.word	0x61662064
3000c240:	64656c69 	.word	0x64656c69
3000c244:	6552202e 	.word	0x6552202e
3000c248:	746f6f62 	.word	0x746f6f62
3000c24c:	20796220 	.word	0x20796220
3000c250:	63746177 	.word	0x63746177
3000c254:	676f6468 	.word	0x676f6468
3000c258:	0000000a 	.word	0x0000000a
3000c25c:	3141544f 	.word	0x3141544f
3000c260:	43524320 	.word	0x43524320
3000c264:	6d203233 	.word	0x6d203233
3000c268:	68637461 	.word	0x68637461
3000c26c:	7461202c 	.word	0x7461202c
3000c270:	68636174 	.word	0x68636174
3000c274:	43206465 	.word	0x43206465
3000c278:	203a4352 	.word	0x203a4352
3000c27c:	78257830 	.word	0x78257830
3000c280:	6163202c 	.word	0x6163202c
3000c284:	6c75636c 	.word	0x6c75636c
3000c288:	64657461 	.word	0x64657461
3000c28c:	43524320 	.word	0x43524320
3000c290:	7830203a 	.word	0x7830203a
3000c294:	000a7825 	.word	0x000a7825
3000c298:	3141544f 	.word	0x3141544f
3000c29c:	766e6920 	.word	0x766e6920
3000c2a0:	64696c61 	.word	0x64696c61
3000c2a4:	69616620 	.word	0x69616620
3000c2a8:	3a64656c 	.word	0x3a64656c
3000c2ac:	0a642520 	.word	0x0a642520
3000c2b0:	00000000 	.word	0x00000000
3000c2b4:	3141544f 	.word	0x3141544f
3000c2b8:	43524320 	.word	0x43524320
3000c2bc:	6e203233 	.word	0x6e203233
3000c2c0:	6d20746f 	.word	0x6d20746f
3000c2c4:	68637461 	.word	0x68637461
3000c2c8:	7461202c 	.word	0x7461202c
3000c2cc:	68636174 	.word	0x68636174
3000c2d0:	43206465 	.word	0x43206465
3000c2d4:	203a4352 	.word	0x203a4352
3000c2d8:	78257830 	.word	0x78257830
3000c2dc:	6163202c 	.word	0x6163202c
3000c2e0:	6c75636c 	.word	0x6c75636c
3000c2e4:	64657461 	.word	0x64657461
3000c2e8:	43524320 	.word	0x43524320
3000c2ec:	7830203a 	.word	0x7830203a
3000c2f0:	66207825 	.word	0x66207825
3000c2f4:	656c6961 	.word	0x656c6961
3000c2f8:	25203a64 	.word	0x25203a64
3000c2fc:	00000a64 	.word	0x00000a64
3000c300:	3141544f 	.word	0x3141544f
3000c304:	43524320 	.word	0x43524320
3000c308:	6e203233 	.word	0x6e203233
3000c30c:	6d20746f 	.word	0x6d20746f
3000c310:	68637461 	.word	0x68637461
3000c314:	7461202c 	.word	0x7461202c
3000c318:	68636174 	.word	0x68636174
3000c31c:	43206465 	.word	0x43206465
3000c320:	203a4352 	.word	0x203a4352
3000c324:	78257830 	.word	0x78257830
3000c328:	6163202c 	.word	0x6163202c
3000c32c:	6c75636c 	.word	0x6c75636c
3000c330:	64657461 	.word	0x64657461
3000c334:	43524320 	.word	0x43524320
3000c338:	7830203a 	.word	0x7830203a
3000c33c:	202c7825 	.word	0x202c7825
3000c340:	73617265 	.word	0x73617265
3000c344:	544f2065 	.word	0x544f2065
3000c348:	61203141 	.word	0x61203141
3000c34c:	72657466 	.word	0x72657466
3000c350:	69616620 	.word	0x69616620
3000c354:	2064656c 	.word	0x2064656c
3000c358:	65747461 	.word	0x65747461
3000c35c:	7374706d 	.word	0x7374706d
3000c360:	0000000a 	.word	0x0000000a
3000c364:	3241544f 	.word	0x3241544f
3000c368:	43524320 	.word	0x43524320
3000c36c:	6d203233 	.word	0x6d203233
3000c370:	68637461 	.word	0x68637461
3000c374:	7461202c 	.word	0x7461202c
3000c378:	68636174 	.word	0x68636174
3000c37c:	43206465 	.word	0x43206465
3000c380:	203a4352 	.word	0x203a4352
3000c384:	78257830 	.word	0x78257830
3000c388:	6163202c 	.word	0x6163202c
3000c38c:	6c75636c 	.word	0x6c75636c
3000c390:	64657461 	.word	0x64657461
3000c394:	43524320 	.word	0x43524320
3000c398:	7830203a 	.word	0x7830203a
3000c39c:	000a7825 	.word	0x000a7825
3000c3a0:	3241544f 	.word	0x3241544f
3000c3a4:	766e6920 	.word	0x766e6920
3000c3a8:	64696c61 	.word	0x64696c61
3000c3ac:	69616620 	.word	0x69616620
3000c3b0:	3a64656c 	.word	0x3a64656c
3000c3b4:	0a642520 	.word	0x0a642520
3000c3b8:	00000000 	.word	0x00000000
3000c3bc:	3241544f 	.word	0x3241544f
3000c3c0:	43524320 	.word	0x43524320
3000c3c4:	6e203233 	.word	0x6e203233
3000c3c8:	6d20746f 	.word	0x6d20746f
3000c3cc:	68637461 	.word	0x68637461
3000c3d0:	7461202c 	.word	0x7461202c
3000c3d4:	68636174 	.word	0x68636174
3000c3d8:	43206465 	.word	0x43206465
3000c3dc:	203a4352 	.word	0x203a4352
3000c3e0:	78257830 	.word	0x78257830
3000c3e4:	6163202c 	.word	0x6163202c
3000c3e8:	6c75636c 	.word	0x6c75636c
3000c3ec:	64657461 	.word	0x64657461
3000c3f0:	43524320 	.word	0x43524320
3000c3f4:	7830203a 	.word	0x7830203a
3000c3f8:	66207825 	.word	0x66207825
3000c3fc:	656c6961 	.word	0x656c6961
3000c400:	25203a64 	.word	0x25203a64
3000c404:	00000a64 	.word	0x00000a64
3000c408:	3241544f 	.word	0x3241544f
3000c40c:	43524320 	.word	0x43524320
3000c410:	6e203233 	.word	0x6e203233
3000c414:	6d20746f 	.word	0x6d20746f
3000c418:	68637461 	.word	0x68637461
3000c41c:	7461202c 	.word	0x7461202c
3000c420:	68636174 	.word	0x68636174
3000c424:	43206465 	.word	0x43206465
3000c428:	203a4352 	.word	0x203a4352
3000c42c:	78257830 	.word	0x78257830
3000c430:	6163202c 	.word	0x6163202c
3000c434:	6c75636c 	.word	0x6c75636c
3000c438:	64657461 	.word	0x64657461
3000c43c:	43524320 	.word	0x43524320
3000c440:	7830203a 	.word	0x7830203a
3000c444:	202c7825 	.word	0x202c7825
3000c448:	73617265 	.word	0x73617265
3000c44c:	544f2065 	.word	0x544f2065
3000c450:	61203241 	.word	0x61203241
3000c454:	72657466 	.word	0x72657466
3000c458:	69616620 	.word	0x69616620
3000c45c:	2064656c 	.word	0x2064656c
3000c460:	65747461 	.word	0x65747461
3000c464:	7374706d 	.word	0x7374706d
3000c468:	0000000a 	.word	0x0000000a
3000c46c:	3141544f 	.word	0x3141544f
3000c470:	766e6920 	.word	0x766e6920
3000c474:	64696c61 	.word	0x64696c61
3000c478:	0a642520 	.word	0x0a642520
3000c47c:	00000000 	.word	0x00000000
3000c480:	3241544f 	.word	0x3241544f
3000c484:	766e6920 	.word	0x766e6920
3000c488:	64696c61 	.word	0x64696c61
3000c48c:	0a642520 	.word	0x0a642520
3000c490:	00000000 	.word	0x00000000
3000c494:	68746f42 	.word	0x68746f42
3000c498:	41544f20 	.word	0x41544f20
3000c49c:	616d6920 	.word	0x616d6920
3000c4a0:	76206567 	.word	0x76206567
3000c4a4:	66697265 	.word	0x66697265
3000c4a8:	61662079 	.word	0x61662079
3000c4ac:	64656c69 	.word	0x64656c69
3000c4b0:	0000000a 	.word	0x0000000a
3000c4b4:	69746341 	.word	0x69746341
3000c4b8:	69206576 	.word	0x69206576
3000c4bc:	7865646e 	.word	0x7865646e
3000c4c0:	41544f20 	.word	0x41544f20
3000c4c4:	766e6920 	.word	0x766e6920
3000c4c8:	64696c61 	.word	0x64696c61
3000c4cc:	6572202c 	.word	0x6572202c
3000c4d0:	74697277 	.word	0x74697277
3000c4d4:	63612065 	.word	0x63612065
3000c4d8:	65766974 	.word	0x65766974
3000c4dc:	646e6920 	.word	0x646e6920
3000c4e0:	74207865 	.word	0x74207865
3000c4e4:	6425206f 	.word	0x6425206f
3000c4e8:	0000000a 	.word	0x0000000a
3000c4ec:	206c6c41 	.word	0x206c6c41
3000c4f0:	2041544f 	.word	0x2041544f
3000c4f4:	72646461 	.word	0x72646461
3000c4f8:	65737365 	.word	0x65737365
3000c4fc:	6f6e2073 	.word	0x6f6e2073
3000c500:	61762074 	.word	0x61762074
3000c504:	2c64696c 	.word	0x2c64696c
3000c508:	63657220 	.word	0x63657220
3000c50c:	7265766f 	.word	0x7265766f
3000c510:	61662079 	.word	0x61662079
3000c514:	000a6c69 	.word	0x000a6c69
3000c518:	20325042 	.word	0x20325042
3000c51c:	3261746f 	.word	0x3261746f
3000c520:	64646120 	.word	0x64646120
3000c524:	73736572 	.word	0x73736572
3000c528:	766e6920 	.word	0x766e6920
3000c52c:	64696c61 	.word	0x64696c61
3000c530:	0000000a 	.word	0x0000000a
3000c534:	20325042 	.word	0x20325042
3000c538:	73726576 	.word	0x73726576
3000c53c:	206e6f69 	.word	0x206e6f69
3000c540:	61766e69 	.word	0x61766e69
3000c544:	0a64696c 	.word	0x0a64696c
3000c548:	00000000 	.word	0x00000000
3000c54c:	20325042 	.word	0x20325042
3000c550:	69746361 	.word	0x69746361
3000c554:	69206576 	.word	0x69206576
3000c558:	7865646e 	.word	0x7865646e
3000c55c:	766e6920 	.word	0x766e6920
3000c560:	64696c61 	.word	0x64696c61
3000c564:	0000000a 	.word	0x0000000a
3000c568:	20325042 	.word	0x20325042
3000c56c:	3161746f 	.word	0x3161746f
3000c570:	64646120 	.word	0x64646120
3000c574:	73736572 	.word	0x73736572
3000c578:	766e6920 	.word	0x766e6920
3000c57c:	64696c61 	.word	0x64696c61
3000c580:	0000000a 	.word	0x0000000a
3000c584:	20325042 	.word	0x20325042
3000c588:	61746164 	.word	0x61746164
3000c58c:	6c617620 	.word	0x6c617620
3000c590:	202c6469 	.word	0x202c6469
3000c594:	73726576 	.word	0x73726576
3000c598:	3a6e6f69 	.word	0x3a6e6f69
3000c59c:	000a6425 	.word	0x000a6425
3000c5a0:	2d73255b 	.word	0x2d73255b
3000c5a4:	44205d49 	.word	0x44205d49
3000c5a8:	62617369 	.word	0x62617369
3000c5ac:	4b20656c 	.word	0x4b20656c
3000c5b0:	4220304d 	.word	0x4220304d
3000c5b4:	20746f6f 	.word	0x20746f6f
3000c5b8:	206e6920 	.word	0x206e6920
3000c5bc:	74736554 	.word	0x74736554
3000c5c0:	646f6d20 	.word	0x646f6d20
3000c5c4:	000a0d65 	.word	0x000a0d65
3000c5c8:	2d73255b 	.word	0x2d73255b
3000c5cc:	42205d49 	.word	0x42205d49
3000c5d0:	73617079 	.word	0x73617079
3000c5d4:	544f2073 	.word	0x544f2073
3000c5d8:	75612050 	.word	0x75612050
3000c5dc:	6f6c6f74 	.word	0x6f6c6f74
3000c5e0:	0a0d6461 	.word	0x0a0d6461
3000c5e4:	00000000 	.word	0x00000000
3000c5e8:	2d73255b 	.word	0x2d73255b
3000c5ec:	4e205d49 	.word	0x4e205d49
3000c5f0:	72462050 	.word	0x72462050
3000c5f4:	25207165 	.word	0x25207165
3000c5f8:	484d2064 	.word	0x484d2064
3000c5fc:	00000a7a 	.word	0x00000a7a
3000c600:	2d73255b 	.word	0x2d73255b
3000c604:	41205d49 	.word	0x41205d49
3000c608:	72462050 	.word	0x72462050
3000c60c:	25207165 	.word	0x25207165
3000c610:	484d2064 	.word	0x484d2064
3000c614:	00000a7a 	.word	0x00000a7a
3000c618:	31474d49 	.word	0x31474d49
3000c61c:	4f425320 	.word	0x4f425320
3000c620:	4f20544f 	.word	0x4f20544f
3000c624:	000a4646 	.word	0x000a4646
3000c628:	31474d49 	.word	0x31474d49
3000c62c:	4f425320 	.word	0x4f425320
3000c630:	5320544f 	.word	0x5320544f
3000c634:	45434355 	.word	0x45434355
3000c638:	000a5353 	.word	0x000a5353
3000c63c:	746f6f62 	.word	0x746f6f62
3000c640:	64616f6c 	.word	0x64616f6c
3000c644:	765f7265 	.word	0x765f7265
3000c648:	69737265 	.word	0x69737265
3000c64c:	25206e6f 	.word	0x25206e6f
3000c650:	00000a73 	.word	0x00000a73
3000c654:	444f4d5b 	.word	0x444f4d5b
3000c658:	5f454c55 	.word	0x5f454c55
3000c65c:	544f4f42 	.word	0x544f4f42
3000c660:	56454c2d 	.word	0x56454c2d
3000c664:	495f4c45 	.word	0x495f4c45
3000c668:	5d4f464e 	.word	0x5d4f464e
3000c66c:	474d493a 	.word	0x474d493a
3000c670:	4e452031 	.word	0x4e452031
3000c674:	20524554 	.word	0x20524554
3000c678:	3a50534d 	.word	0x3a50534d
3000c67c:	3830255b 	.word	0x3830255b
3000c680:	000a5d78 	.word	0x000a5d78
3000c684:	444f4d5b 	.word	0x444f4d5b
3000c688:	5f454c55 	.word	0x5f454c55
3000c68c:	544f4f42 	.word	0x544f4f42
3000c690:	56454c2d 	.word	0x56454c2d
3000c694:	495f4c45 	.word	0x495f4c45
3000c698:	5d4f464e 	.word	0x5d4f464e
3000c69c:	474d493a 	.word	0x474d493a
3000c6a0:	45532031 	.word	0x45532031
3000c6a4:	45525543 	.word	0x45525543
3000c6a8:	41545320 	.word	0x41545320
3000c6ac:	203a4554 	.word	0x203a4554
3000c6b0:	000a6425 	.word	0x000a6425
3000c6b4:	2d73255b 	.word	0x2d73255b
3000c6b8:	49205d49 	.word	0x49205d49
3000c6bc:	2074696e 	.word	0x2074696e
3000c6c0:	41525350 	.word	0x41525350
3000c6c4:	000a0d4d 	.word	0x000a0d4d
3000c6c8:	2d73255b 	.word	0x2d73255b
3000c6cc:	73205d57 	.word	0x73205d57
3000c6d0:	70207465 	.word	0x70207465
3000c6d4:	66206d66 	.word	0x66206d66
3000c6d8:	0d6c6961 	.word	0x0d6c6961
3000c6dc:	0000000a 	.word	0x0000000a
3000c6e0:	2d73255b 	.word	0x2d73255b
3000c6e4:	45205d45 	.word	0x45205d45
3000c6e8:	524f5252 	.word	0x524f5252
3000c6ec:	53202121 	.word	0x53202121
3000c6f0:	6c756f68 	.word	0x6c756f68
3000c6f4:	6f4e2064 	.word	0x6f4e2064
3000c6f8:	6e652074 	.word	0x6e652074
3000c6fc:	656c6261 	.word	0x656c6261
3000c700:	6d654d20 	.word	0x6d654d20
3000c704:	5f727753 	.word	0x5f727753
3000c708:	796c6e4f 	.word	0x796c6e4f
3000c70c:	206e6920 	.word	0x206e6920
3000c710:	20524444 	.word	0x20524444
3000c714:	70696843 	.word	0x70696843
3000c718:	0d212121 	.word	0x0d212121
3000c71c:	0000000a 	.word	0x0000000a
3000c720:	2d73255b 	.word	0x2d73255b
3000c724:	49205d49 	.word	0x49205d49
3000c728:	2074696e 	.word	0x2074696e
3000c72c:	0d524444 	.word	0x0d524444
3000c730:	0000000a 	.word	0x0000000a
3000c734:	2d73255b 	.word	0x2d73255b
3000c738:	42205d49 	.word	0x42205d49
3000c73c:	5f50554b 	.word	0x5f50554b
3000c740:	32474552 	.word	0x32474552
3000c744:	25783020 	.word	0x25783020
3000c748:	0a0d2078 	.word	0x0a0d2078
3000c74c:	00000000 	.word	0x00000000
3000c750:	55676f4c 	.word	0x55676f4c
3000c754:	20747261 	.word	0x20747261
3000c758:	64756142 	.word	0x64756142
3000c75c:	65746172 	.word	0x65746172
3000c760:	6425203a 	.word	0x6425203a
3000c764:	0000000a 	.word	0x0000000a
3000c768:	726f4e0a 	.word	0x726f4e0a
3000c76c:	206c616d 	.word	0x206c616d
3000c770:	746f6f62 	.word	0x746f6f62
3000c774:	0000000a 	.word	0x0000000a
3000c778:	5f315042 	.word	0x5f315042
3000c77c:	3141544f 	.word	0x3141544f
3000c780:	00000000 	.word	0x00000000
3000c784:	5f315042 	.word	0x5f315042
3000c788:	3241544f 	.word	0x3241544f
3000c78c:	00000000 	.word	0x00000000
3000c790:	5f325042 	.word	0x5f325042
3000c794:	3141544f 	.word	0x3141544f
3000c798:	00000000 	.word	0x00000000
3000c79c:	5f325042 	.word	0x5f325042
3000c7a0:	3241544f 	.word	0x3241544f
3000c7a4:	00000000 	.word	0x00000000
3000c7a8:	20304d4b 	.word	0x20304d4b
3000c7ac:	20504958 	.word	0x20504958
3000c7b0:	00474d49 	.word	0x00474d49
3000c7b4:	20304d4b 	.word	0x20304d4b
3000c7b8:	4d415253 	.word	0x4d415253
3000c7bc:	00000000 	.word	0x00000000
3000c7c0:	20304d4b 	.word	0x20304d4b
3000c7c4:	4d415244 	.word	0x4d415244
3000c7c8:	00000000 	.word	0x00000000
3000c7cc:	20344d4b 	.word	0x20344d4b
3000c7d0:	20504958 	.word	0x20504958
3000c7d4:	00474d49 	.word	0x00474d49
3000c7d8:	20344d4b 	.word	0x20344d4b
3000c7dc:	4d415253 	.word	0x4d415253
3000c7e0:	00000000 	.word	0x00000000
3000c7e4:	20344d4b 	.word	0x20344d4b
3000c7e8:	4d415244 	.word	0x4d415244
3000c7ec:	00000000 	.word	0x00000000
3000c7f0:	58205041 	.word	0x58205041
3000c7f4:	49205049 	.word	0x49205049
3000c7f8:	0000474d 	.word	0x0000474d
3000c7fc:	42205041 	.word	0x42205041
3000c800:	5320314c 	.word	0x5320314c
3000c804:	004d4152 	.word	0x004d4152
3000c808:	42205041 	.word	0x42205041
3000c80c:	4420314c 	.word	0x4420314c
3000c810:	004d4152 	.word	0x004d4152
3000c814:	46205041 	.word	0x46205041
3000c818:	00005049 	.word	0x00005049
3000c81c:	2d73255b 	.word	0x2d73255b
3000c820:	25205d49 	.word	0x25205d49
3000c824:	6e492073 	.word	0x6e492073
3000c828:	696c6176 	.word	0x696c6176
3000c82c:	00000a64 	.word	0x00000a64
3000c830:	2d73255b 	.word	0x2d73255b
3000c834:	25205d49 	.word	0x25205d49
3000c838:	30255b73 	.word	0x30255b73
3000c83c:	253a7838 	.word	0x253a7838
3000c840:	000a5d78 	.word	0x000a5d78
3000c844:	2d73255b 	.word	0x2d73255b
3000c848:	52205d49 	.word	0x52205d49
3000c84c:	45205044 	.word	0x45205044
3000c850:	00000a4e 	.word	0x00000a4e
3000c854:	2d73255b 	.word	0x2d73255b
3000c858:	46205d49 	.word	0x46205d49
3000c85c:	206c6961 	.word	0x206c6961
3000c860:	6c206f74 	.word	0x6c206f74
3000c864:	2064616f 	.word	0x2064616f
3000c868:	20504452 	.word	0x20504452
3000c86c:	67616d69 	.word	0x67616d69
3000c870:	000a2165 	.word	0x000a2165
3000c874:	2d73255b 	.word	0x2d73255b
3000c878:	52205d49 	.word	0x52205d49
3000c87c:	4f205044 	.word	0x4f205044
3000c880:	000a4646 	.word	0x000a4646
3000c884:	2d73255b 	.word	0x2d73255b
3000c888:	49205d49 	.word	0x49205d49
3000c88c:	2032474d 	.word	0x2032474d
3000c890:	2046544f 	.word	0x2046544f
3000c894:	000a4e45 	.word	0x000a4e45
3000c898:	2d73255b 	.word	0x2d73255b
3000c89c:	4f205d45 	.word	0x4f205d45
3000c8a0:	4d204654 	.word	0x4d204654
3000c8a4:	2065646f 	.word	0x2065646f
3000c8a8:	6f727265 	.word	0x6f727265
3000c8ac:	000a0d72 	.word	0x000a0d72
3000c8b0:	494c4156 	.word	0x494c4156
3000c8b4:	00000044 	.word	0x00000044
3000c8b8:	41564e49 	.word	0x41564e49
3000c8bc:	0044494c 	.word	0x0044494c
3000c8c0:	74726563 	.word	0x74726563
3000c8c4:	63696669 	.word	0x63696669
3000c8c8:	20657461 	.word	0x20657461
3000c8cc:	20434345 	.word	0x20434345
3000c8d0:	69726576 	.word	0x69726576
3000c8d4:	66207966 	.word	0x66207966
3000c8d8:	2c6c6961 	.word	0x2c6c6961
3000c8dc:	6c6f7220 	.word	0x6c6f7220
3000c8e0:	6361626c 	.word	0x6361626c
3000c8e4:	000a0d6b 	.word	0x000a0d6b
3000c8e8:	444f4d5b 	.word	0x444f4d5b
3000c8ec:	5f454c55 	.word	0x5f454c55
3000c8f0:	544f4f42 	.word	0x544f4f42
3000c8f4:	56454c2d 	.word	0x56454c2d
3000c8f8:	495f4c45 	.word	0x495f4c45
3000c8fc:	5d4f464e 	.word	0x5d4f464e
3000c900:	474d493a 	.word	0x474d493a
3000c904:	544f2832 	.word	0x544f2832
3000c908:	29642541 	.word	0x29642541
3000c90c:	2c732520 	.word	0x2c732520
3000c910:	74657220 	.word	0x74657220
3000c914:	6425203a 	.word	0x6425203a
3000c918:	0000000a 	.word	0x0000000a
3000c91c:	32474d49 	.word	0x32474d49
3000c920:	43434520 	.word	0x43434520
3000c924:	72657620 	.word	0x72657620
3000c928:	20796669 	.word	0x20796669
3000c92c:	6c696166 	.word	0x6c696166
3000c930:	6f72202c 	.word	0x6f72202c
3000c934:	61626c6c 	.word	0x61626c6c
3000c938:	0a0d6b63 	.word	0x0a0d6b63
3000c93c:	00000000 	.word	0x00000000
3000c940:	5f41544f 	.word	0x5f41544f
3000c944:	45205041 	.word	0x45205041
3000c948:	76204343 	.word	0x76204343
3000c94c:	66697265 	.word	0x66697265
3000c950:	61662079 	.word	0x61662079
3000c954:	202c6c69 	.word	0x202c6c69
3000c958:	6c6c6f72 	.word	0x6c6c6f72
3000c95c:	6b636162 	.word	0x6b636162
3000c960:	00000a0d 	.word	0x00000a0d
3000c964:	64616f4c 	.word	0x64616f4c
3000c968:	41544f20 	.word	0x41544f20
3000c96c:	2050415f 	.word	0x2050415f
3000c970:	67616d69 	.word	0x67616d69
3000c974:	61662065 	.word	0x61662065
3000c978:	202c6c69 	.word	0x202c6c69
3000c97c:	6c6c6f72 	.word	0x6c6c6f72
3000c980:	6b636162 	.word	0x6b636162
3000c984:	00000a0d 	.word	0x00000a0d
3000c988:	444f4d5b 	.word	0x444f4d5b
3000c98c:	5f454c55 	.word	0x5f454c55
3000c990:	544f4f42 	.word	0x544f4f42
3000c994:	56454c2d 	.word	0x56454c2d
3000c998:	455f4c45 	.word	0x455f4c45
3000c99c:	524f5252 	.word	0x524f5252
3000c9a0:	61463a5d 	.word	0x61463a5d
3000c9a4:	74206c69 	.word	0x74206c69
3000c9a8:	6f6c206f 	.word	0x6f6c206f
3000c9ac:	41206461 	.word	0x41206461
3000c9b0:	6d692050 	.word	0x6d692050
3000c9b4:	21656761 	.word	0x21656761
3000c9b8:	0000000a 	.word	0x0000000a
3000c9bc:	20344d4b 	.word	0x20344d4b
3000c9c0:	33474d49 	.word	0x33474d49
3000c9c4:	00000000 	.word	0x00000000
3000c9c8:	20344d4b 	.word	0x20344d4b
3000c9cc:	0043534e 	.word	0x0043534e
3000c9d0:	31474d49 	.word	0x31474d49
3000c9d4:	00000000 	.word	0x00000000
3000c9d8:	32474d49 	.word	0x32474d49
3000c9dc:	00000000 	.word	0x00000000
3000c9e0:	42205041 	.word	0x42205041
3000c9e4:	4626314c 	.word	0x4626314c
3000c9e8:	00005049 	.word	0x00005049
3000c9ec:	444f4d5b 	.word	0x444f4d5b
3000c9f0:	5f454c55 	.word	0x5f454c55
3000c9f4:	544f4f42 	.word	0x544f4f42
3000c9f8:	56454c2d 	.word	0x56454c2d
3000c9fc:	495f4c45 	.word	0x495f4c45
3000ca00:	5d4f464e 	.word	0x5d4f464e
3000ca04:	5b73253a 	.word	0x5b73253a
3000ca08:	78383025 	.word	0x78383025
3000ca0c:	5d78253a 	.word	0x5d78253a
3000ca10:	0000000a 	.word	0x0000000a
3000ca14:	20504452 	.word	0x20504452
3000ca18:	4c494146 	.word	0x4c494146
3000ca1c:	0000000a 	.word	0x0000000a
3000ca20:	20504452 	.word	0x20504452
3000ca24:	43435553 	.word	0x43435553
3000ca28:	0a535345 	.word	0x0a535345
3000ca2c:	00000000 	.word	0x00000000
3000ca30:	059e4eb3 	.word	0x059e4eb3
3000ca34:	4b11ea39 	.word	0x4b11ea39
3000ca38:	2a1b920a 	.word	0x2a1b920a
3000ca3c:	29939cee 	.word	0x29939cee
3000ca40:	00000000 	.word	0x00000000
3000ca44:	42205041 	.word	0x42205041
3000ca48:	0032334c 	.word	0x0032334c
3000ca4c:	2d73255b 	.word	0x2d73255b
3000ca50:	25205d49 	.word	0x25205d49
3000ca54:	45562073 	.word	0x45562073
3000ca58:	59464952 	.word	0x59464952
3000ca5c:	53415020 	.word	0x53415020
3000ca60:	00000a53 	.word	0x00000a53
3000ca64:	2d73255b 	.word	0x2d73255b
3000ca68:	25205d45 	.word	0x25205d45
3000ca6c:	45562073 	.word	0x45562073
3000ca70:	59464952 	.word	0x59464952
3000ca74:	49414620 	.word	0x49414620
3000ca78:	72202c4c 	.word	0x72202c4c
3000ca7c:	3d207465 	.word	0x3d207465
3000ca80:	0a642520 	.word	0x0a642520
3000ca84:	00000000 	.word	0x00000000
3000ca88:	2d73255b 	.word	0x2d73255b
3000ca8c:	43205d45 	.word	0x43205d45
3000ca90:	69747265 	.word	0x69747265
3000ca94:	61636966 	.word	0x61636966
3000ca98:	56206574 	.word	0x56206574
3000ca9c:	46495245 	.word	0x46495245
3000caa0:	41462059 	.word	0x41462059
3000caa4:	202c4c49 	.word	0x202c4c49
3000caa8:	20746572 	.word	0x20746572
3000caac:	6425203d 	.word	0x6425203d
3000cab0:	0000000a 	.word	0x0000000a
3000cab4:	50494843 	.word	0x50494843
3000cab8:	4f464e49 	.word	0x4f464e49
3000cabc:	00000000 	.word	0x00000000
3000cac0:	2d73255b 	.word	0x2d73255b
3000cac4:	49205d45 	.word	0x49205d45
3000cac8:	6c61766e 	.word	0x6c61766e
3000cacc:	43206469 	.word	0x43206469
3000cad0:	696e6968 	.word	0x696e6968
3000cad4:	216f666e 	.word	0x216f666e
3000cad8:	65684320 	.word	0x65684320
3000cadc:	4f206b63 	.word	0x4f206b63
3000cae0:	000a5054 	.word	0x000a5054
3000cae4:	2d73255b 	.word	0x2d73255b
3000cae8:	50205d57 	.word	0x50205d57
3000caec:	7361656c 	.word	0x7361656c
3000caf0:	72702065 	.word	0x72702065
3000caf4:	6172676f 	.word	0x6172676f
3000caf8:	6863206d 	.word	0x6863206d
3000cafc:	6e697069 	.word	0x6e697069
3000cb00:	69206f66 	.word	0x69206f66
3000cb04:	544f206e 	.word	0x544f206e
3000cb08:	0a212050 	.word	0x0a212050
3000cb0c:	00000000 	.word	0x00000000

3000cb10 <Memory_Info>:
3000cb10:	03e00007 00060004 010202c0 02010100     ................
3000cb20:	02000102 00040302 02040400 01010102     ................
3000cb30:	01020221 02240401 03000102 00030103     !.....$.........
3000cb40:	01050500 06000002 01020206 01070700     ................
3000cb50:	08000002 01020208 02280801 09000102     ..........(.....
3000cb60:	00040309 03290901 0a000004 0003010a     ......).........
3000cb70:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000cb80:	0d000102 0004030d 010e0e00 ffff0003     ................
3000cb90:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000cba0:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000cbb0:	62696c61 6f696172 000a0d6e              alibraion...

3000cbbc <ddrphy_tx_scan>:
3000cbbc:	00000000 00080000 00080808 00000000     ................
3000cbcc:	20000000 33333333 000c0022 00000000     ... 3333".......
3000cbdc:	07060012 000f0707 080f0f0f 00000008     ................
3000cbec:	20000001 22222222 000c0012 34000000     ... """".......4
3000cbfc:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000cc0c:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cc1c:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000cc2c:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cc3c:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000cc4c:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cc5c:	0e0e001a 00170e0e 12171717 00000012     ................
3000cc6c:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000cc7c:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000cc8c:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cc9c:	0f0f001b 00180f0f 13181818 00000013     ................
3000ccac:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000ccbc:	1110001b 00191111 13191919 00000013     ................
3000cccc:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000ccdc:	1313001c 001c1313 121c1c1c 00000012     ................
3000ccec:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000ccfc <ddrphy_zq_rx_scan>:
	...
3000cd04:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000cd14:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000cd24:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000cd34:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000cd44:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000cd54:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000cd64:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000cd74:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000cd84:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000cd94:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000cda4:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000cdb4:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000cdc4 <Flash_AVL>:
3000cdc4:	000000ef 000000ff 00000000 000043fc     .............C..
3000cdd4:	00000000 000000a1 000000ff 00000000     ................
3000cde4:	0000fffc 00000000 0000000b 000000ff     ................
3000cdf4:	00000000 000043fc 00000000 0000000e     .....C..........
3000ce04:	000000ff 00000000 000043fc 00000000     .........C......
3000ce14:	000000c8 000000ff 00000001 000043fc     .............C..
3000ce24:	00000000 000028c2 0000ffff 00000005     .....(..........
3000ce34:	000200fc 00000000 000000c2 000000ff     ................
3000ce44:	00000002 000000fc 00000000 00000068     ............h...
3000ce54:	000000ff 00000002 000000fc 00000000     ................
3000ce64:	00000051 000000ff 00000002 000000fc     Q...............
3000ce74:	00000000 0000001c 000000ff 00000003     ................
3000ce84:	000000fc 00000000 00000020 000000ff     ........ .......
3000ce94:	00000004 000000fc 00000000 00000000     ................
3000cea4:	000000ff 000000fe ffffffff 30007d35     ............5}.0
3000ceb4:	000000ff ffffffff 000000ff ffffffff     ................
3000cec4:	00000000                                ....

3000cec8 <Flash_ReadMode>:
3000cec8:	                                         ..

3000ceca <Flash_Speed>:
3000ceca:	                                         ..

3000cecc <NAND_AVL>:
3000cecc:	000000ef 000000ff 00000000 ffffffff     ................
3000cedc:	00000000 000000c8 000000ff 00000001     ................
3000ceec:	ffffffff 00000000 000000e5 000000ff     ................
3000cefc:	00000001 ffffffff 00000000 000000c2     ................
3000cf0c:	000000ff 00000002 ffffffff 00000000     ................
3000cf1c:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000cf34:	000000ff 000000fe ffffffff 30007d95     .............}.0
3000cf44:	000000ff ffffffff 000000ff ffffffff     ................
3000cf54:	00000000 3000bc10 3000bc14 3000bc18     .......0...0...0
3000cf64:	3000bc1c 3000bc20                       ...0 ..0

3000cf6c <__FUNCTION__.0>:
3000cf6c:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000cf7c <__FUNCTION__.1>:
3000cf7c:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000cf8c:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000cf9c:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000cfac:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000cfbc:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000cfcc:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000cfdc:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000cfec:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000cffc:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000d00c:	00000a20                                 ...

3000d010 <__FUNCTION__.1>:
3000d010:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d020:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000d030:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000d040:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000d050:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000d060:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000d070:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000d080:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000d090:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000d0a0:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000d0b0:	726f6620 41485320 00000a32               for SHA2...

3000d0bc <__FUNCTION__.1>:
3000d0bc:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d0cc <__FUNCTION__.3>:
3000d0cc:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d0dc:	32616873 696e695f 00000074              sha2_init...

3000d0e8 <__FUNCTION__.4>:
3000d0e8:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d0f8:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000d108:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000d118:	6e207369 6320746f 69666e6f 65727567     is not configure
3000d128:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000d138:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000d148:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000d158:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000d168:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000d178:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000d188:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000d198:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000d1a8:	69625f6b 21217374 00000a21              k_bits!!!...

3000d1b4 <__FUNCTION__.0>:
3000d1b4:	33697872 445f3631 72536e79 6e695f65     rxi316_DynSre_in
3000d1c4:	00007469                                it..

3000d1c8 <__FUNCTION__.1>:
3000d1c8:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d1d8:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000d1e8:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000d1f8:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000d208:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000d218:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000d228:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000d238:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000d248:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000d258:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000d268:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000d278:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000d288:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000d298:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000d2a8:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000d2b8:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000d2c8:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000d2d8:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000d2e8:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000d2f8:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000d308:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000d318:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000d328:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000d338:	00000000                                ....

3000d33c <register_string>:
3000d33c:	00000000 3000d264 3000bd30 3000bd34     ....d..00..04..0
3000d34c:	3000bd38 3000bd3c 3000bd40 3000bd44     8..0<..0@..0D..0
3000d35c:	3000bd48 3000d260 3000bd4c 3000bd50     H..0`..0L..0P..0
3000d36c:	3000bd54 3000d304 65637845 6f697470     T..0...0Exceptio
3000d37c:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000d38c:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000d39c:	64657375 20796220 6c696166 76206465     used by failed v
3000d3ac:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000d3bc:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000d3cc:	00000000 75636553 79746972 75616620     ....Security fau
3000d3dc:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d3ec:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000d3fc:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000d40c:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000d41c:	79746972 75616620 6920746c 61632073     rity fault is ca
3000d42c:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000d43c:	65746e69 74697267 69732079 74616e67     integrity signat
3000d44c:	00657275 75636553 79746972 75616620     ure.Security fau
3000d45c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d46c:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000d47c:	6572206e 6e727574 00000000 75636553     n return....Secu
3000d48c:	79746972 75616620 6920746c 61632073     rity fault is ca
3000d49c:	64657375 20796220 72747441 74756269     used by Attribut
3000d4ac:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000d4bc:	00006e6f 75636553 79746972 75616620     on..Security fau
3000d4cc:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d4dc:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000d4ec:	00006e6f 75636553 79746972 75616620     on..Security fau
3000d4fc:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d50c:	797a614c 61747320 70206574 65736572     Lazy state prese
3000d51c:	74617672 206e6f69 6f727265 00000072     rvation error...
3000d52c:	75636553 79746972 75616620 6920746c     Security fault i
3000d53c:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000d54c:	61747320 65206574 726f7272 00000000      state error....
3000d55c:	20656854 75636573 79746972 75616620     The security fau
3000d56c:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000d57c:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000d58c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d59c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000d5ac:	61662074 20746c75 63207369 65737561     t fault is cause
3000d5bc:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000d5cc:	63636120 20737365 6c6f6976 6f697461      access violatio
3000d5dc:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000d5ec:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000d5fc:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000d60c:	20746c75 63207369 65737561 79622064     ult is caused by
3000d61c:	74616420 63612061 73736563 6f697620      data access vio
3000d62c:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000d63c:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000d64c:	63207369 65737561 79622064 736e7520     is caused by uns
3000d65c:	6b636174 20676e69 6f727265 00000072     tacking error...
3000d66c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000d67c:	61662074 20746c75 63207369 65737561     t fault is cause
3000d68c:	79622064 61747320 6e696b63 72652067     d by stacking er
3000d69c:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000d6ac:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000d6bc:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000d6cc:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000d6dc:	70206574 65736572 74617672 006e6f69     te preservation.
3000d6ec:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000d6fc:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000d70c:	64657272 64646120 73736572 00736920     rred address is.
3000d71c:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000d72c:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000d73c:	73692074 75616320 20646573 69207962     t is caused by i
3000d74c:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000d75c:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000d76c:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000d77c:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000d78c:	75616320 20646573 70207962 69636572      caused by preci
3000d79c:	64206573 20617461 65636361 76207373     se data access v
3000d7ac:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000d7bc:	6c756166 73692074 75616320 20646573     fault is caused 
3000d7cc:	69207962 6572706d 65736963 74616420     by imprecise dat
3000d7dc:	63612061 73736563 6f697620 6974616c     a access violati
3000d7ec:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000d7fc:	75616320 20646573 75207962 6174736e      caused by unsta
3000d80c:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000d81c:	6c756166 73692074 75616320 20646573     fault is caused 
3000d82c:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000d83c:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000d84c:	75616320 20646573 66207962 74616f6c      caused by float
3000d85c:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000d86c:	65746174 65727020 76726573 6f697461     tate preservatio
3000d87c:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000d88c:	636f2074 72727563 61206465 65726464     t occurred addre
3000d89c:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000d8ac:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d8bc:	67617355 61662065 20746c75 63207369     Usage fault is c
3000d8cc:	65737561 79622064 74746120 74706d65     aused by attempt
3000d8dc:	6f742073 65786520 65747563 206e6120     s to execute an 
3000d8ec:	65646e75 656e6966 6e692064 75727473     undefined instru
3000d8fc:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000d90c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d91c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000d92c:	65737561 79622064 74746120 74706d65     aused by attempt
3000d93c:	6f742073 69777320 20686374 61206f74     s to switch to a
3000d94c:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000d95c:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000d96c:	61662065 20746c75 63207369 65737561     e fault is cause
3000d97c:	79622064 74746120 74706d65 6f742073     d by attempts to
3000d98c:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000d99c:	74697720 20612068 20646162 756c6176      with a bad valu
3000d9ac:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000d9bc:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000d9cc:	61662065 20746c75 63207369 65737561     e fault is cause
3000d9dc:	79622064 74746120 74706d65 6f742073     d by attempts to
3000d9ec:	65786520 65747563 63206120 6f72706f      execute a copro
3000d9fc:	73736563 6920726f 7274736e 69746375     cessor instructi
3000da0c:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000da1c:	63207369 65737561 79622064 646e6920     is caused by ind
3000da2c:	74616369 74207365 20746168 74732061     icates that a st
3000da3c:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000da4c:	61776472 63206572 6b636568 61682029     rdware check) ha
3000da5c:	61742073 206e656b 63616c70 00000065     s taken place...
3000da6c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000da7c:	65737561 79622064 646e6920 74616369     aused by indicat
3000da8c:	74207365 20746168 75206e61 696c616e     es that an unali
3000da9c:	64656e67 63636120 20737365 6c756166     gned access faul
3000daac:	61682074 61742073 206e656b 63616c70     t has taken plac
3000dabc:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000dacc:	63207369 65737561 79622064 646e4920     is caused by Ind
3000dadc:	74616369 61207365 76696420 20656469     icates a divide 
3000daec:	7a207962 206f7265 20736168 656b6174     by zero has take
3000dafc:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000db0c:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000db1c:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000db2c:	75626544 61662067 20746c75 63207369     Debug fault is c
3000db3c:	65737561 79622064 6c616820 65722074     aused by halt re
3000db4c:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000db5c:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000db6c:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000db7c:	20746c75 63207369 65737561 79622064     ult is caused by
3000db8c:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000db9c:	7865206e 74756365 00006465 75626544     n executed..Debu
3000dbac:	61662067 20746c75 63207369 65737561     g fault is cause
3000dbbc:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000dbcc:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000dbdc:	20746c75 63207369 65737561 79622064     ult is caused by
3000dbec:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000dbfc:	72727563 00006465 75626544 61662067     curred..Debug fa
3000dc0c:	20746c75 63207369 65737561 79622064     ult is caused by
3000dc1c:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000dc2c:	72657373 00646574 00000208 0000020c     sserted.........
3000dc3c:	00000210 00000008 00000214 00000218     ................
3000dc4c:	0000021c 0000000c                       ........

3000dc54 <CSWTCH.380>:
3000dc54:	3000c778 3000c784 3000c790 3000c79c     x..0...0...0...0

3000dc64 <__FUNCTION__.0>:
3000dc64:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.
3000dc74:	3000c7a8 3000c7b4 3000c7c0 3000c7cc     ...0...0...0...0
3000dc84:	3000c7d8 3000c7e4 3000c7f0 3000c7fc     ...0...0...0...0
3000dc94:	3000c808 3000c814 3000c9bc 3000c9c8     ...0...0...0...0
3000dca4:	3000c9d0 3000c9d8 3000c9c0 3000c9e0     ...0...0...0...0

3000dcb4 <km4_bootloader_rev>:
3000dcb4:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000dcc4:	375f7265 36373130 39336635 30325f62     er_701765f39b_20
3000dcd4:	302f3632 36302f35 3a35312d 333a3732     26/05/06-15:27:3
3000dce4:	4f420032 5b00544f 572d7325 2a2a205d     2.BOOT.[%s-W] **
3000dcf4:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000dd04:	2a2a2a2a 2a2a2a2a 52415720 474e494e     ******** WARNING
3000dd14:	2a2a2a20 2a2a2a2a 2a2a2a2a 2a2a2a2a      ***************
3000dd24:	2a2a2a2a 2a2a2a2a 255b000a 5d572d73     ********..[%s-W]
3000dd34:	20202a20 20202020 20202020 20202020      *              
3000dd44:	20202020 20202020 20202020 20202020                     
3000dd54:	20202020 20202020 20202020 20202020                     
3000dd64:	20202020 20202020 0a2a2020 73255b00               *..[%s
3000dd74:	205d572d 2020202a 20202020 20202020     -W] *           
3000dd84:	75432020 6e657272 68632074 76207069       Current chip v
3000dd94:	69737265 69206e6f 43412073 21217475     ersion is ACut!!
3000dda4:	20202021 20202020 20202020 000a2a20     !            *..
3000ddb4:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000ddc4:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000ddd4:	2074726f 72727563 20746e65 73726576     ort current vers
3000dde4:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000ddf4:	5b000a2a 572d7325 202a205d 20202020     *..[%s-W] *     
3000de04:	656c5020 20657361 746e6f63 20746361      Please contact 
3000de14:	204b5452 66204d50 6d20726f 2065726f     RTK PM for more 
3000de24:	646f7270 20746375 6f666e69 20202021     product info!   
3000de34:	2a202020 255b000a 5d572d73 2a2a2a20        *..[%s-W] ***
3000de44:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000de54:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000de64:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000de74:	2a2a2a2a 0a2a2a2a 73255b00 205d572d     *******..[%s-W] 
3000de84:	0043000a 49534f50 002e0058              ..C.POSIX...

3000de90 <_ctype_>:
3000de90:	20202000 20202020 28282020 20282828     .         ((((( 
3000dea0:	20202020 20202020 20202020 20202020                     
3000deb0:	10108820 10101010 10101010 10101010      ...............
3000dec0:	04040410 04040404 10040404 10101010     ................
3000ded0:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000dee0:	01010101 01010101 01010101 10101010     ................
3000def0:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000df00:	02020202 02020202 02020202 10101010     ................
3000df10:	00000020 00000000 00000000 00000000      ...............
	...
