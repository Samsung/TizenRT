
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
300032c4:	3000921d 	.word	0x3000921d
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	300090a5 	.word	0x300090a5
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
300033fc:	3000921d 	.word	0x3000921d
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	300090a5 	.word	0x300090a5
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
3000349c:	f008 f85a 	bl	3000b554 <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000bbc4 	.word	0x3000bbc4
300034bc:	3000bbd0 	.word	0x3000bbd0

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
300034f0:	f008 f830 	bl	3000b554 <rtk_log_write>
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
30003524:	f008 f816 	bl	3000b554 <rtk_log_write>
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
30003540:	3000bbc4 	.word	0x3000bbc4
30003544:	3000bc14 	.word	0x3000bc14
30003548:	3000921d 	.word	0x3000921d
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000bbf4 	.word	0x3000bbf4
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
3000365a:	f007 ff7b 	bl	3000b554 <rtk_log_write>
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
3000367a:	f007 ff6b 	bl	3000b554 <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000cef8 	.word	0x3000cef8
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000bbc4 	.word	0x3000bbc4
300036a8:	3000bc4c 	.word	0x3000bc4c
300036ac:	3000bc34 	.word	0x3000bc34
300036b0:	3000921d 	.word	0x3000921d
300036b4:	300090a5 	.word	0x300090a5
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
3000373e:	f007 ff09 	bl	3000b554 <rtk_log_write>
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
30003790:	f007 fee0 	bl	3000b554 <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 f921 	bl	3000b9e0 <__io_assert_failed_veneer>
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
300037ca:	f007 fec3 	bl	3000b554 <rtk_log_write>
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
30003816:	f007 fe9d 	bl	3000b554 <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f008 f8de 	bl	3000b9e0 <__io_assert_failed_veneer>
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
30003958:	3000ce68 	.word	0x3000ce68
3000395c:	3000ce6a 	.word	0x3000ce6a
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000bbc4 	.word	0x3000bbc4
3000396c:	3000bc70 	.word	0x3000bc70
30003970:	3000cd64 	.word	0x3000cd64
30003974:	3000ce6c 	.word	0x3000ce6c
30003978:	3000bc8c 	.word	0x3000bc8c
3000397c:	3000cf1c 	.word	0x3000cf1c
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000bcb8 	.word	0x3000bcb8
30003988:	3000cf0c 	.word	0x3000cf0c
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000f331 	.word	0x3000f331
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	3000921d 	.word	0x3000921d
300039b8:	3000f334 	.word	0x3000f334
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
30003a36:	f007 ffd3 	bl	3000b9e0 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f007 ffca 	bl	3000b9e0 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000f331 	.word	0x3000f331
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000cf0c 	.word	0x3000cf0c
30003a78:	3000cf1c 	.word	0x3000cf1c
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
30003aac:	f007 fd52 	bl	3000b554 <rtk_log_write>
30003ab0:	2244      	movs	r2, #68	; 0x44
30003ab2:	2100      	movs	r1, #0
30003ab4:	a803      	add	r0, sp, #12
30003ab6:	f007 ffd3 	bl	3000ba60 <____wrap_memset_veneer>
30003aba:	0672      	lsls	r2, r6, #25
30003abc:	4b39      	ldr	r3, [pc, #228]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003abe:	d55f      	bpl.n	30003b80 <BOOT_FaultHandler+0xe8>
30003ac0:	4619      	mov	r1, r3
30003ac2:	4a3a      	ldr	r2, [pc, #232]	; (30003bac <BOOT_FaultHandler+0x114>)
30003ac4:	2001      	movs	r0, #1
30003ac6:	f007 fd45 	bl	3000b554 <rtk_log_write>
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
30003b14:	f007 fa48 	bl	3000afa8 <crash_dump>
30003b18:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30003b1c:	4b21      	ldr	r3, [pc, #132]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b1e:	2001      	movs	r0, #1
30003b20:	9100      	str	r1, [sp, #0]
30003b22:	4a24      	ldr	r2, [pc, #144]	; (30003bb4 <BOOT_FaultHandler+0x11c>)
30003b24:	4619      	mov	r1, r3
30003b26:	f007 fd15 	bl	3000b554 <rtk_log_write>
30003b2a:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30003b2e:	4b1d      	ldr	r3, [pc, #116]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b30:	2001      	movs	r0, #1
30003b32:	9200      	str	r2, [sp, #0]
30003b34:	4619      	mov	r1, r3
30003b36:	4a20      	ldr	r2, [pc, #128]	; (30003bb8 <BOOT_FaultHandler+0x120>)
30003b38:	f007 fd0c 	bl	3000b554 <rtk_log_write>
30003b3c:	6a61      	ldr	r1, [r4, #36]	; 0x24
30003b3e:	4b19      	ldr	r3, [pc, #100]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b40:	2001      	movs	r0, #1
30003b42:	9100      	str	r1, [sp, #0]
30003b44:	4a1d      	ldr	r2, [pc, #116]	; (30003bbc <BOOT_FaultHandler+0x124>)
30003b46:	4619      	mov	r1, r3
30003b48:	f007 fd04 	bl	3000b554 <rtk_log_write>
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
30003b6c:	f007 fcf2 	bl	3000b554 <rtk_log_write>
30003b70:	463b      	mov	r3, r7
30003b72:	4632      	mov	r2, r6
30003b74:	4629      	mov	r1, r5
30003b76:	4620      	mov	r0, r4
30003b78:	f007 ff8a 	bl	3000ba90 <__INT_HardFault_C_veneer>
30003b7c:	b015      	add	sp, #84	; 0x54
30003b7e:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003b80:	4a12      	ldr	r2, [pc, #72]	; (30003bcc <BOOT_FaultHandler+0x134>)
30003b82:	2001      	movs	r0, #1
30003b84:	4619      	mov	r1, r3
30003b86:	f007 fce5 	bl	3000b554 <rtk_log_write>
30003b8a:	f3ef 8394 	mrs	r3, CONTROL_NS
30003b8e:	f3ef 8e88 	mrs	lr, MSP_NS
30003b92:	f3ef 8589 	mrs	r5, PSP_NS
30003b96:	f016 0108 	ands.w	r1, r6, #8
30003b9a:	bf18      	it	ne
30003b9c:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30003ba0:	e79b      	b.n	30003ada <BOOT_FaultHandler+0x42>
30003ba2:	bf00      	nop
30003ba4:	3000bcf8 	.word	0x3000bcf8
30003ba8:	3000bd00 	.word	0x3000bd00
30003bac:	3000bd24 	.word	0x3000bd24
30003bb0:	e000ed00 	.word	0xe000ed00
30003bb4:	3000bd78 	.word	0x3000bd78
30003bb8:	3000bd90 	.word	0x3000bd90
30003bbc:	3000bda8 	.word	0x3000bda8
30003bc0:	0000994d 	.word	0x0000994d
30003bc4:	00009979 	.word	0x00009979
30003bc8:	3000bdc0 	.word	0x3000bdc0
30003bcc:	3000bd4c 	.word	0x3000bd4c

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
30003e04:	f007 fe8c 	bl	3000bb20 <__DiagPrintf_veneer>
30003e08:	2f03      	cmp	r7, #3
30003e0a:	9901      	ldr	r1, [sp, #4]
30003e0c:	d1e3      	bne.n	30003dd6 <recovery_check+0x3e>
30003e0e:	480d      	ldr	r0, [pc, #52]	; (30003e44 <recovery_check+0xac>)
30003e10:	f007 fe86 	bl	3000bb20 <__DiagPrintf_veneer>
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
30003e38:	3000dbf4 	.word	0x3000dbf4
30003e3c:	3000e104 	.word	0x3000e104
30003e40:	3000bde4 	.word	0x3000bde4
30003e44:	3000be18 	.word	0x3000be18
30003e48:	3000bddc 	.word	0x3000bddc

30003e4c <rewrite_bp>:
30003e4c:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30003e50:	2701      	movs	r7, #1
30003e52:	b0a2      	sub	sp, #136	; 0x88
30003e54:	461d      	mov	r5, r3
30003e56:	4604      	mov	r4, r0
30003e58:	f89d a0ac 	ldrb.w	sl, [sp, #172]	; 0xac
30003e5c:	f88d 7007 	strb.w	r7, [sp, #7]
30003e60:	2800      	cmp	r0, #0
30003e62:	d147      	bne.n	30003ef4 <rewrite_bp+0xa8>
30003e64:	4689      	mov	r9, r1
30003e66:	f8d1 1006 	ldr.w	r1, [r1, #6]
30003e6a:	2965      	cmp	r1, #101	; 0x65
30003e6c:	d06b      	beq.n	30003f46 <rewrite_bp+0xfa>
30003e6e:	4e3f      	ldr	r6, [pc, #252]	; (30003f6c <rewrite_bp+0x120>)
30003e70:	f10d 0808 	add.w	r8, sp, #8
30003e74:	483e      	ldr	r0, [pc, #248]	; (30003f70 <rewrite_bp+0x124>)
30003e76:	f007 fe53 	bl	3000bb20 <__DiagPrintf_veneer>
30003e7a:	2280      	movs	r2, #128	; 0x80
30003e7c:	f8c6 9400 	str.w	r9, [r6, #1024]	; 0x400
30003e80:	f1ba 0f00 	cmp.w	sl, #0
30003e84:	d048      	beq.n	30003f18 <rewrite_bp+0xcc>
30003e86:	4629      	mov	r1, r5
30003e88:	4640      	mov	r0, r8
30003e8a:	f007 fe31 	bl	3000baf0 <____wrap_memcpy_veneer>
30003e8e:	2002      	movs	r0, #2
30003e90:	f04f 0901 	mov.w	r9, #1
30003e94:	4629      	mov	r1, r5
30003e96:	f88d 4014 	strb.w	r4, [sp, #20]
30003e9a:	e9cd 9003 	strd	r9, r0, [sp, #12]
30003e9e:	f7ff fe97 	bl	30003bd0 <FLASH_Erase_With_Lock>
30003ea2:	f105 0c04 	add.w	ip, r5, #4
30003ea6:	aa03      	add	r2, sp, #12
30003ea8:	217c      	movs	r1, #124	; 0x7c
30003eaa:	4660      	mov	r0, ip
30003eac:	4664      	mov	r4, ip
30003eae:	f7ff feb7 	bl	30003c20 <FLASH_TxData_With_Lock>
30003eb2:	4649      	mov	r1, r9
30003eb4:	f10d 0207 	add.w	r2, sp, #7
30003eb8:	f605 70ff 	addw	r0, r5, #4095	; 0xfff
30003ebc:	f7ff feb0 	bl	30003c20 <FLASH_TxData_With_Lock>
30003ec0:	46a4      	mov	ip, r4
30003ec2:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30003ec6:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30003eca:	f81c 2b01 	ldrb.w	r2, [ip], #1
30003ece:	405a      	eors	r2, r3
30003ed0:	458c      	cmp	ip, r1
30003ed2:	b2d2      	uxtb	r2, r2
30003ed4:	f856 2022 	ldr.w	r2, [r6, r2, lsl #2]
30003ed8:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30003edc:	d1f5      	bne.n	30003eca <rewrite_bp+0x7e>
30003ede:	43db      	mvns	r3, r3
30003ee0:	4628      	mov	r0, r5
30003ee2:	4642      	mov	r2, r8
30003ee4:	2104      	movs	r1, #4
30003ee6:	9302      	str	r3, [sp, #8]
30003ee8:	f7ff fe9a 	bl	30003c20 <FLASH_TxData_With_Lock>
30003eec:	4638      	mov	r0, r7
30003eee:	b022      	add	sp, #136	; 0x88
30003ef0:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003ef4:	f8d2 1006 	ldr.w	r1, [r2, #6]
30003ef8:	4690      	mov	r8, r2
30003efa:	2965      	cmp	r1, #101	; 0x65
30003efc:	d011      	beq.n	30003f22 <rewrite_bp+0xd6>
30003efe:	4e1b      	ldr	r6, [pc, #108]	; (30003f6c <rewrite_bp+0x120>)
30003f00:	2702      	movs	r7, #2
30003f02:	481c      	ldr	r0, [pc, #112]	; (30003f74 <rewrite_bp+0x128>)
30003f04:	f007 fe0c 	bl	3000bb20 <__DiagPrintf_veneer>
30003f08:	f8c6 8404 	str.w	r8, [r6, #1028]	; 0x404
30003f0c:	2280      	movs	r2, #128	; 0x80
30003f0e:	f10d 0808 	add.w	r8, sp, #8
30003f12:	f1ba 0f00 	cmp.w	sl, #0
30003f16:	d1b6      	bne.n	30003e86 <rewrite_bp+0x3a>
30003f18:	992a      	ldr	r1, [sp, #168]	; 0xa8
30003f1a:	4640      	mov	r0, r8
30003f1c:	f007 fde8 	bl	3000baf0 <____wrap_memcpy_veneer>
30003f20:	e7b5      	b.n	30003e8e <rewrite_bp+0x42>
30003f22:	4815      	ldr	r0, [pc, #84]	; (30003f78 <rewrite_bp+0x12c>)
30003f24:	f007 fdfc 	bl	3000bb20 <__DiagPrintf_veneer>
30003f28:	4b14      	ldr	r3, [pc, #80]	; (30003f7c <rewrite_bp+0x130>)
30003f2a:	a802      	add	r0, sp, #8
30003f2c:	4798      	blx	r3
30003f2e:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30003f32:	4813      	ldr	r0, [pc, #76]	; (30003f80 <rewrite_bp+0x134>)
30003f34:	a902      	add	r1, sp, #8
30003f36:	4b13      	ldr	r3, [pc, #76]	; (30003f84 <rewrite_bp+0x138>)
30003f38:	f8ad 200a 	strh.w	r2, [sp, #10]
30003f3c:	4798      	blx	r3
30003f3e:	4810      	ldr	r0, [pc, #64]	; (30003f80 <rewrite_bp+0x134>)
30003f40:	4b11      	ldr	r3, [pc, #68]	; (30003f88 <rewrite_bp+0x13c>)
30003f42:	4798      	blx	r3
30003f44:	e7fe      	b.n	30003f44 <rewrite_bp+0xf8>
30003f46:	480c      	ldr	r0, [pc, #48]	; (30003f78 <rewrite_bp+0x12c>)
30003f48:	f007 fdea 	bl	3000bb20 <__DiagPrintf_veneer>
30003f4c:	4b0b      	ldr	r3, [pc, #44]	; (30003f7c <rewrite_bp+0x130>)
30003f4e:	a802      	add	r0, sp, #8
30003f50:	4798      	blx	r3
30003f52:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30003f56:	480a      	ldr	r0, [pc, #40]	; (30003f80 <rewrite_bp+0x134>)
30003f58:	a902      	add	r1, sp, #8
30003f5a:	4b0a      	ldr	r3, [pc, #40]	; (30003f84 <rewrite_bp+0x138>)
30003f5c:	f8ad 200a 	strh.w	r2, [sp, #10]
30003f60:	4798      	blx	r3
30003f62:	4807      	ldr	r0, [pc, #28]	; (30003f80 <rewrite_bp+0x134>)
30003f64:	4b08      	ldr	r3, [pc, #32]	; (30003f88 <rewrite_bp+0x13c>)
30003f66:	4798      	blx	r3
30003f68:	e7fe      	b.n	30003f68 <rewrite_bp+0x11c>
30003f6a:	bf00      	nop
30003f6c:	3000e104 	.word	0x3000e104
30003f70:	3000bea4 	.word	0x3000bea4
30003f74:	3000bebc 	.word	0x3000bebc
30003f78:	3000be50 	.word	0x3000be50
30003f7c:	0000d3f9 	.word	0x0000d3f9
30003f80:	41000440 	.word	0x41000440
30003f84:	0000d409 	.word	0x0000d409
30003f88:	0000d489 	.word	0x0000d489

30003f8c <BOOT_SectionInit>:
30003f8c:	4800      	ldr	r0, [pc, #0]	; (30003f90 <BOOT_SectionInit+0x4>)
30003f8e:	4770      	bx	lr
30003f90:	60000020 	.word	0x60000020

30003f94 <BOOT_NsStart>:
30003f94:	b570      	push	{r4, r5, r6, lr}
30003f96:	4c23      	ldr	r4, [pc, #140]	; (30004024 <BOOT_NsStart+0x90>)
30003f98:	4605      	mov	r5, r0
30003f9a:	f384 8808 	msr	MSP, r4
30003f9e:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003fa2:	2100      	movs	r1, #0
30003fa4:	4620      	mov	r0, r4
30003fa6:	4e20      	ldr	r6, [pc, #128]	; (30004028 <BOOT_NsStart+0x94>)
30003fa8:	47b0      	blx	r6
30003faa:	4a20      	ldr	r2, [pc, #128]	; (3000402c <BOOT_NsStart+0x98>)
30003fac:	6953      	ldr	r3, [r2, #20]
30003fae:	03d9      	lsls	r1, r3, #15
30003fb0:	d50b      	bpl.n	30003fca <BOOT_NsStart+0x36>
30003fb2:	f3bf 8f4f 	dsb	sy
30003fb6:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
30003fba:	3420      	adds	r4, #32
30003fbc:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
30003fc0:	d1f9      	bne.n	30003fb6 <BOOT_NsStart+0x22>
30003fc2:	f3bf 8f4f 	dsb	sy
30003fc6:	f3bf 8f6f 	isb	sy
30003fca:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
30003fce:	f383 8808 	msr	MSP, r3
30003fd2:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003fd6:	2100      	movs	r1, #0
30003fd8:	4815      	ldr	r0, [pc, #84]	; (30004030 <BOOT_NsStart+0x9c>)
30003fda:	47b0      	blx	r6
30003fdc:	4a13      	ldr	r2, [pc, #76]	; (3000402c <BOOT_NsStart+0x98>)
30003fde:	6953      	ldr	r3, [r2, #20]
30003fe0:	03db      	lsls	r3, r3, #15
30003fe2:	d50c      	bpl.n	30003ffe <BOOT_NsStart+0x6a>
30003fe4:	f3bf 8f4f 	dsb	sy
30003fe8:	4b11      	ldr	r3, [pc, #68]	; (30004030 <BOOT_NsStart+0x9c>)
30003fea:	490e      	ldr	r1, [pc, #56]	; (30004024 <BOOT_NsStart+0x90>)
30003fec:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30003ff0:	3320      	adds	r3, #32
30003ff2:	428b      	cmp	r3, r1
30003ff4:	d1fa      	bne.n	30003fec <BOOT_NsStart+0x58>
30003ff6:	f3bf 8f4f 	dsb	sy
30003ffa:	f3bf 8f6f 	isb	sy
30003ffe:	f025 0501 	bic.w	r5, r5, #1
30004002:	086d      	lsrs	r5, r5, #1
30004004:	006d      	lsls	r5, r5, #1
30004006:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
3000400a:	ed2d 8b10 	vpush	{d8-d15}
3000400e:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
30004012:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
30004016:	47ac      	blxns	r5
30004018:	ecbd 8b10 	vpop	{d8-d15}
3000401c:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004020:	bd70      	pop	{r4, r5, r6, pc}
30004022:	bf00      	nop
30004024:	30002000 	.word	0x30002000
30004028:	00012be5 	.word	0x00012be5
3000402c:	e000ed00 	.word	0xe000ed00
30004030:	30001000 	.word	0x30001000

30004034 <BOOT_RccConfig>:
30004034:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004038:	4c2f      	ldr	r4, [pc, #188]	; (300040f8 <BOOT_RccConfig+0xc4>)
3000403a:	b090      	sub	sp, #64	; 0x40
3000403c:	4f2f      	ldr	r7, [pc, #188]	; (300040fc <BOOT_RccConfig+0xc8>)
3000403e:	466d      	mov	r5, sp
30004040:	f10d 0c20 	add.w	ip, sp, #32
30004044:	683e      	ldr	r6, [r7, #0]
30004046:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
3000404a:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
3000404e:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30004052:	f104 0310 	add.w	r3, r4, #16
30004056:	2400      	movs	r4, #0
30004058:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000405a:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
3000405e:	1c72      	adds	r2, r6, #1
30004060:	e9cd 4404 	strd	r4, r4, [sp, #16]
30004064:	e9cd 4406 	strd	r4, r4, [sp, #24]
30004068:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000406c:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30004070:	d03f      	beq.n	300040f2 <BOOT_RccConfig+0xbe>
30004072:	463a      	mov	r2, r7
30004074:	6891      	ldr	r1, [r2, #8]
30004076:	0fb3      	lsrs	r3, r6, #30
30004078:	a810      	add	r0, sp, #64	; 0x40
3000407a:	2901      	cmp	r1, #1
3000407c:	eb00 0383 	add.w	r3, r0, r3, lsl #2
30004080:	d10e      	bne.n	300040a0 <BOOT_RccConfig+0x6c>
30004082:	f853 0c30 	ldr.w	r0, [r3, #-48]
30004086:	6854      	ldr	r4, [r2, #4]
30004088:	4330      	orrs	r0, r6
3000408a:	ae10      	add	r6, sp, #64	; 0x40
3000408c:	0fa1      	lsrs	r1, r4, #30
3000408e:	f843 0c30 	str.w	r0, [r3, #-48]
30004092:	eb06 0181 	add.w	r1, r6, r1, lsl #2
30004096:	f851 3c10 	ldr.w	r3, [r1, #-16]
3000409a:	4323      	orrs	r3, r4
3000409c:	f841 3c10 	str.w	r3, [r1, #-16]
300040a0:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300040a4:	1c73      	adds	r3, r6, #1
300040a6:	d1e5      	bne.n	30004074 <BOOT_RccConfig+0x40>
300040a8:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300040aa:	9c04      	ldr	r4, [sp, #16]
300040ac:	af0d      	add	r7, sp, #52	; 0x34
300040ae:	ae05      	add	r6, sp, #20
300040b0:	4a13      	ldr	r2, [pc, #76]	; (30004100 <BOOT_RccConfig+0xcc>)
300040b2:	f85c 0b04 	ldr.w	r0, [ip], #4
300040b6:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300040ba:	f855 1b04 	ldr.w	r1, [r5], #4
300040be:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300040c2:	f850 8002 	ldr.w	r8, [r0, r2]
300040c6:	45e6      	cmp	lr, ip
300040c8:	ea43 0308 	orr.w	r3, r3, r8
300040cc:	5083      	str	r3, [r0, r2]
300040ce:	588b      	ldr	r3, [r1, r2]
300040d0:	ea44 0403 	orr.w	r4, r4, r3
300040d4:	508c      	str	r4, [r1, r2]
300040d6:	d004      	beq.n	300040e2 <BOOT_RccConfig+0xae>
300040d8:	f857 3b04 	ldr.w	r3, [r7], #4
300040dc:	f856 4b04 	ldr.w	r4, [r6], #4
300040e0:	e7e7      	b.n	300040b2 <BOOT_RccConfig+0x7e>
300040e2:	200a      	movs	r0, #10
300040e4:	4b07      	ldr	r3, [pc, #28]	; (30004104 <BOOT_RccConfig+0xd0>)
300040e6:	4798      	blx	r3
300040e8:	4b07      	ldr	r3, [pc, #28]	; (30004108 <BOOT_RccConfig+0xd4>)
300040ea:	b010      	add	sp, #64	; 0x40
300040ec:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040f0:	4718      	bx	r3
300040f2:	4623      	mov	r3, r4
300040f4:	e7da      	b.n	300040ac <BOOT_RccConfig+0x78>
300040f6:	bf00      	nop
300040f8:	3000dbd4 	.word	0x3000dbd4
300040fc:	3000df48 	.word	0x3000df48
30004100:	42008000 	.word	0x42008000
30004104:	00009b2d 	.word	0x00009b2d
30004108:	30007531 	.word	0x30007531

3000410c <BOOT_CACHEWRR_Set>:
3000410c:	4b14      	ldr	r3, [pc, #80]	; (30004160 <BOOT_CACHEWRR_Set+0x54>)
3000410e:	b430      	push	{r4, r5}
30004110:	681a      	ldr	r2, [r3, #0]
30004112:	2a01      	cmp	r2, #1
30004114:	d10f      	bne.n	30004136 <BOOT_CACHEWRR_Set+0x2a>
30004116:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
3000411a:	685d      	ldr	r5, [r3, #4]
3000411c:	689c      	ldr	r4, [r3, #8]
3000411e:	6882      	ldr	r2, [r0, #8]
30004120:	4910      	ldr	r1, [pc, #64]	; (30004164 <BOOT_CACHEWRR_Set+0x58>)
30004122:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004126:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
3000412a:	6082      	str	r2, [r0, #8]
3000412c:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30004130:	68da      	ldr	r2, [r3, #12]
30004132:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
30004136:	691a      	ldr	r2, [r3, #16]
30004138:	2a01      	cmp	r2, #1
3000413a:	d10f      	bne.n	3000415c <BOOT_CACHEWRR_Set+0x50>
3000413c:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30004140:	695d      	ldr	r5, [r3, #20]
30004142:	4a08      	ldr	r2, [pc, #32]	; (30004164 <BOOT_CACHEWRR_Set+0x58>)
30004144:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30004148:	688b      	ldr	r3, [r1, #8]
3000414a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000414e:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
30004152:	608b      	str	r3, [r1, #8]
30004154:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30004158:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
3000415c:	bc30      	pop	{r4, r5}
3000415e:	4770      	bx	lr
30004160:	3000e5f4 	.word	0x3000e5f4
30004164:	e0042000 	.word	0xe0042000

30004168 <BOOT_TCMSet>:
30004168:	2300      	movs	r3, #0
3000416a:	b5f0      	push	{r4, r5, r6, r7, lr}
3000416c:	4db3      	ldr	r5, [pc, #716]	; (3000443c <BOOT_TCMSet+0x2d4>)
3000416e:	461c      	mov	r4, r3
30004170:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
30004174:	3307      	adds	r3, #7
30004176:	4282      	cmp	r2, r0
30004178:	d03c      	beq.n	300041f4 <BOOT_TCMSet+0x8c>
3000417a:	3401      	adds	r4, #1
3000417c:	2c07      	cmp	r4, #7
3000417e:	d1f7      	bne.n	30004170 <BOOT_TCMSet+0x8>
30004180:	f3bf 8f4f 	dsb	sy
30004184:	f3bf 8f6f 	isb	sy
30004188:	48ad      	ldr	r0, [pc, #692]	; (30004440 <BOOT_TCMSet+0x2d8>)
3000418a:	2200      	movs	r2, #0
3000418c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30004190:	f3bf 8f4f 	dsb	sy
30004194:	f3bf 8f6f 	isb	sy
30004198:	6943      	ldr	r3, [r0, #20]
3000419a:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
3000419e:	6143      	str	r3, [r0, #20]
300041a0:	f3bf 8f4f 	dsb	sy
300041a4:	f3bf 8f6f 	isb	sy
300041a8:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300041ac:	f3bf 8f4f 	dsb	sy
300041b0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300041b4:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300041b8:	f3c5 344e 	ubfx	r4, r5, #13, #15
300041bc:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300041c0:	0164      	lsls	r4, r4, #5
300041c2:	ea04 0106 	and.w	r1, r4, r6
300041c6:	462b      	mov	r3, r5
300041c8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300041cc:	3b01      	subs	r3, #1
300041ce:	1c5f      	adds	r7, r3, #1
300041d0:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300041d4:	d1f8      	bne.n	300041c8 <BOOT_TCMSet+0x60>
300041d6:	3c20      	subs	r4, #32
300041d8:	f114 0f20 	cmn.w	r4, #32
300041dc:	d1f1      	bne.n	300041c2 <BOOT_TCMSet+0x5a>
300041de:	f3bf 8f4f 	dsb	sy
300041e2:	6943      	ldr	r3, [r0, #20]
300041e4:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300041e8:	6143      	str	r3, [r0, #20]
300041ea:	f3bf 8f4f 	dsb	sy
300041ee:	f3bf 8f6f 	isb	sy
300041f2:	bdf0      	pop	{r4, r5, r6, r7, pc}
300041f4:	2c00      	cmp	r4, #0
300041f6:	d0c3      	beq.n	30004180 <BOOT_TCMSet+0x18>
300041f8:	f3bf 8f4f 	dsb	sy
300041fc:	f3bf 8f6f 	isb	sy
30004200:	4890      	ldr	r0, [pc, #576]	; (30004444 <BOOT_TCMSet+0x2dc>)
30004202:	2300      	movs	r3, #0
30004204:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30004208:	f3bf 8f4f 	dsb	sy
3000420c:	f3bf 8f6f 	isb	sy
30004210:	6942      	ldr	r2, [r0, #20]
30004212:	03d2      	lsls	r2, r2, #15
30004214:	d520      	bpl.n	30004258 <BOOT_TCMSet+0xf0>
30004216:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000421a:	f3bf 8f4f 	dsb	sy
3000421e:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30004222:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004226:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000422a:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000422e:	ea4f 1c43 	mov.w	ip, r3, lsl #5
30004232:	ea0c 0106 	and.w	r1, ip, r6
30004236:	4673      	mov	r3, lr
30004238:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000423c:	3b01      	subs	r3, #1
3000423e:	1c5f      	adds	r7, r3, #1
30004240:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004244:	d1f8      	bne.n	30004238 <BOOT_TCMSet+0xd0>
30004246:	f1ac 0c20 	sub.w	ip, ip, #32
3000424a:	f11c 0f20 	cmn.w	ip, #32
3000424e:	d1f0      	bne.n	30004232 <BOOT_TCMSet+0xca>
30004250:	f3bf 8f4f 	dsb	sy
30004254:	f3bf 8f6f 	isb	sy
30004258:	f3bf 8f4f 	dsb	sy
3000425c:	f3bf 8f6f 	isb	sy
30004260:	4878      	ldr	r0, [pc, #480]	; (30004444 <BOOT_TCMSet+0x2dc>)
30004262:	2200      	movs	r2, #0
30004264:	6943      	ldr	r3, [r0, #20]
30004266:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
3000426a:	6143      	str	r3, [r0, #20]
3000426c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30004270:	f3bf 8f4f 	dsb	sy
30004274:	f3bf 8f6f 	isb	sy
30004278:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
3000427c:	f3bf 8f4f 	dsb	sy
30004280:	6943      	ldr	r3, [r0, #20]
30004282:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30004286:	6143      	str	r3, [r0, #20]
30004288:	f3bf 8f4f 	dsb	sy
3000428c:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30004290:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30004294:	f3c3 06c9 	ubfx	r6, r3, #3, #10
30004298:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000429c:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300042a0:	ea0c 010e 	and.w	r1, ip, lr
300042a4:	4633      	mov	r3, r6
300042a6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300042aa:	3b01      	subs	r3, #1
300042ac:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300042b0:	1c5a      	adds	r2, r3, #1
300042b2:	d1f8      	bne.n	300042a6 <BOOT_TCMSet+0x13e>
300042b4:	f1ac 0c20 	sub.w	ip, ip, #32
300042b8:	f11c 0f20 	cmn.w	ip, #32
300042bc:	d1f0      	bne.n	300042a0 <BOOT_TCMSet+0x138>
300042be:	f3bf 8f4f 	dsb	sy
300042c2:	f3bf 8f6f 	isb	sy
300042c6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300042ca:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300042ce:	00e3      	lsls	r3, r4, #3
300042d0:	6b41      	ldr	r1, [r0, #52]	; 0x34
300042d2:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300042d6:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
300042da:	6341      	str	r1, [r0, #52]	; 0x34
300042dc:	6911      	ldr	r1, [r2, #16]
300042de:	2903      	cmp	r1, #3
300042e0:	d00f      	beq.n	30004302 <BOOT_TCMSet+0x19a>
300042e2:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300042e6:	4858      	ldr	r0, [pc, #352]	; (30004448 <BOOT_TCMSet+0x2e0>)
300042e8:	f04f 0c00 	mov.w	ip, #0
300042ec:	4f57      	ldr	r7, [pc, #348]	; (3000444c <BOOT_TCMSet+0x2e4>)
300042ee:	68b2      	ldr	r2, [r6, #8]
300042f0:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
300042f4:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
300042f8:	60b2      	str	r2, [r6, #8]
300042fa:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
300042fe:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
30004302:	1b1a      	subs	r2, r3, r4
30004304:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004308:	68d1      	ldr	r1, [r2, #12]
3000430a:	2903      	cmp	r1, #3
3000430c:	d00f      	beq.n	3000432e <BOOT_TCMSet+0x1c6>
3000430e:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
30004312:	484d      	ldr	r0, [pc, #308]	; (30004448 <BOOT_TCMSet+0x2e0>)
30004314:	f04f 0c00 	mov.w	ip, #0
30004318:	4f4c      	ldr	r7, [pc, #304]	; (3000444c <BOOT_TCMSet+0x2e4>)
3000431a:	68b2      	ldr	r2, [r6, #8]
3000431c:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004320:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
30004324:	60b2      	str	r2, [r6, #8]
30004326:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
3000432a:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
3000432e:	1b1a      	subs	r2, r3, r4
30004330:	4945      	ldr	r1, [pc, #276]	; (30004448 <BOOT_TCMSet+0x2e0>)
30004332:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004336:	6950      	ldr	r0, [r2, #20]
30004338:	6608      	str	r0, [r1, #96]	; 0x60
3000433a:	6990      	ldr	r0, [r2, #24]
3000433c:	6648      	str	r0, [r1, #100]	; 0x64
3000433e:	6852      	ldr	r2, [r2, #4]
30004340:	2a01      	cmp	r2, #1
30004342:	d04b      	beq.n	300043dc <BOOT_TCMSet+0x274>
30004344:	1b1b      	subs	r3, r3, r4
30004346:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000434a:	68ab      	ldr	r3, [r5, #8]
3000434c:	2b01      	cmp	r3, #1
3000434e:	f47f af50 	bne.w	300041f2 <BOOT_TCMSet+0x8a>
30004352:	483b      	ldr	r0, [pc, #236]	; (30004440 <BOOT_TCMSet+0x2d8>)
30004354:	2300      	movs	r3, #0
30004356:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000435a:	f3bf 8f4f 	dsb	sy
3000435e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004362:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004366:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000436a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000436e:	0164      	lsls	r4, r4, #5
30004370:	ea04 0106 	and.w	r1, r4, r6
30004374:	462b      	mov	r3, r5
30004376:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000437a:	3b01      	subs	r3, #1
3000437c:	1c5f      	adds	r7, r3, #1
3000437e:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30004382:	d1f8      	bne.n	30004376 <BOOT_TCMSet+0x20e>
30004384:	3c20      	subs	r4, #32
30004386:	f114 0f20 	cmn.w	r4, #32
3000438a:	d1f1      	bne.n	30004370 <BOOT_TCMSet+0x208>
3000438c:	f3bf 8f4f 	dsb	sy
30004390:	6943      	ldr	r3, [r0, #20]
30004392:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30004396:	6143      	str	r3, [r0, #20]
30004398:	f3bf 8f4f 	dsb	sy
3000439c:	f3bf 8f6f 	isb	sy
300043a0:	4828      	ldr	r0, [pc, #160]	; (30004444 <BOOT_TCMSet+0x2dc>)
300043a2:	2300      	movs	r3, #0
300043a4:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300043a8:	f3bf 8f4f 	dsb	sy
300043ac:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300043b0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300043b4:	f3c5 344e 	ubfx	r4, r5, #13, #15
300043b8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300043bc:	0164      	lsls	r4, r4, #5
300043be:	ea04 0106 	and.w	r1, r4, r6
300043c2:	462b      	mov	r3, r5
300043c4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300043c8:	3b01      	subs	r3, #1
300043ca:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300043ce:	1c5a      	adds	r2, r3, #1
300043d0:	d1f8      	bne.n	300043c4 <BOOT_TCMSet+0x25c>
300043d2:	3c20      	subs	r4, #32
300043d4:	f114 0f20 	cmn.w	r4, #32
300043d8:	d1f1      	bne.n	300043be <BOOT_TCMSet+0x256>
300043da:	e700      	b.n	300041de <BOOT_TCMSet+0x76>
300043dc:	f3bf 8f4f 	dsb	sy
300043e0:	f3bf 8f6f 	isb	sy
300043e4:	4a16      	ldr	r2, [pc, #88]	; (30004440 <BOOT_TCMSet+0x2d8>)
300043e6:	2000      	movs	r0, #0
300043e8:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300043ec:	f3bf 8f4f 	dsb	sy
300043f0:	f3bf 8f6f 	isb	sy
300043f4:	6951      	ldr	r1, [r2, #20]
300043f6:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300043fa:	6151      	str	r1, [r2, #20]
300043fc:	f3bf 8f4f 	dsb	sy
30004400:	f3bf 8f6f 	isb	sy
30004404:	f3bf 8f4f 	dsb	sy
30004408:	f3bf 8f6f 	isb	sy
3000440c:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30004410:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30004414:	f3bf 8f4f 	dsb	sy
30004418:	f3bf 8f6f 	isb	sy
3000441c:	6951      	ldr	r1, [r2, #20]
3000441e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30004422:	6151      	str	r1, [r2, #20]
30004424:	f3bf 8f4f 	dsb	sy
30004428:	f3bf 8f6f 	isb	sy
3000442c:	1b1b      	subs	r3, r3, r4
3000442e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30004432:	68ab      	ldr	r3, [r5, #8]
30004434:	2b01      	cmp	r3, #1
30004436:	f47f aedc 	bne.w	300041f2 <BOOT_TCMSet+0x8a>
3000443a:	e78a      	b.n	30004352 <BOOT_TCMSet+0x1ea>
3000443c:	3000e530 	.word	0x3000e530
30004440:	e002ed00 	.word	0xe002ed00
30004444:	e000ed00 	.word	0xe000ed00
30004448:	e0042000 	.word	0xe0042000
3000444c:	ffff0000 	.word	0xffff0000

30004450 <BOOT_GRstConfig>:
30004450:	b538      	push	{r3, r4, r5, lr}
30004452:	4c09      	ldr	r4, [pc, #36]	; (30004478 <BOOT_GRstConfig+0x28>)
30004454:	f241 050a 	movw	r5, #4106	; 0x100a
30004458:	2028      	movs	r0, #40	; 0x28
3000445a:	4a08      	ldr	r2, [pc, #32]	; (3000447c <BOOT_GRstConfig+0x2c>)
3000445c:	8823      	ldrh	r3, [r4, #0]
3000445e:	b29b      	uxth	r3, r3
30004460:	431d      	orrs	r5, r3
30004462:	8025      	strh	r5, [r4, #0]
30004464:	4790      	blx	r2
30004466:	88a3      	ldrh	r3, [r4, #4]
30004468:	b29b      	uxth	r3, r3
3000446a:	ea23 0305 	bic.w	r3, r3, r5
3000446e:	80a3      	strh	r3, [r4, #4]
30004470:	8863      	ldrh	r3, [r4, #2]
30004472:	8065      	strh	r5, [r4, #2]
30004474:	bd38      	pop	{r3, r4, r5, pc}
30004476:	bf00      	nop
30004478:	42008200 	.word	0x42008200
3000447c:	00009b2d 	.word	0x00009b2d

30004480 <BOOT_DDR_LCDC_HPR>:
30004480:	4a04      	ldr	r2, [pc, #16]	; (30004494 <BOOT_DDR_LCDC_HPR+0x14>)
30004482:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30004486:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
3000448a:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
3000448e:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30004492:	4770      	bx	lr
30004494:	42008000 	.word	0x42008000

30004498 <BOOT_DDR_Init>:
30004498:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000449c:	b530      	push	{r4, r5, lr}
3000449e:	4d15      	ldr	r5, [pc, #84]	; (300044f4 <BOOT_DDR_Init+0x5c>)
300044a0:	b099      	sub	sp, #100	; 0x64
300044a2:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300044a4:	47a8      	blx	r5
300044a6:	2803      	cmp	r0, #3
300044a8:	d11e      	bne.n	300044e8 <BOOT_DDR_Init+0x50>
300044aa:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300044ae:	4a12      	ldr	r2, [pc, #72]	; (300044f8 <BOOT_DDR_Init+0x60>)
300044b0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300044b4:	4911      	ldr	r1, [pc, #68]	; (300044fc <BOOT_DDR_Init+0x64>)
300044b6:	a801      	add	r0, sp, #4
300044b8:	631c      	str	r4, [r3, #48]	; 0x30
300044ba:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300044be:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300044c2:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300044c6:	4788      	blx	r1
300044c8:	a801      	add	r0, sp, #4
300044ca:	4b0d      	ldr	r3, [pc, #52]	; (30004500 <BOOT_DDR_Init+0x68>)
300044cc:	4798      	blx	r3
300044ce:	4b0d      	ldr	r3, [pc, #52]	; (30004504 <BOOT_DDR_Init+0x6c>)
300044d0:	4798      	blx	r3
300044d2:	4b0d      	ldr	r3, [pc, #52]	; (30004508 <BOOT_DDR_Init+0x70>)
300044d4:	4798      	blx	r3
300044d6:	4b0d      	ldr	r3, [pc, #52]	; (3000450c <BOOT_DDR_Init+0x74>)
300044d8:	4798      	blx	r3
300044da:	2101      	movs	r1, #1
300044dc:	f44f 60e0 	mov.w	r0, #1792	; 0x700
300044e0:	4b0b      	ldr	r3, [pc, #44]	; (30004510 <BOOT_DDR_Init+0x78>)
300044e2:	4798      	blx	r3
300044e4:	b019      	add	sp, #100	; 0x64
300044e6:	bd30      	pop	{r4, r5, pc}
300044e8:	47a8      	blx	r5
300044ea:	2802      	cmp	r0, #2
300044ec:	d0dd      	beq.n	300044aa <BOOT_DDR_Init+0x12>
300044ee:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
300044f2:	e7dc      	b.n	300044ae <BOOT_DDR_Init+0x16>
300044f4:	0000c0f9 	.word	0x0000c0f9
300044f8:	42008000 	.word	0x42008000
300044fc:	3000764d 	.word	0x3000764d
30004500:	30007bc5 	.word	0x30007bc5
30004504:	3000a985 	.word	0x3000a985
30004508:	30007c95 	.word	0x30007c95
3000450c:	30007c99 	.word	0x30007c99
30004510:	3000a8e5 	.word	0x3000a8e5

30004514 <BOOT_PSRAM_Init>:
30004514:	b500      	push	{lr}
30004516:	b089      	sub	sp, #36	; 0x24
30004518:	4b1b      	ldr	r3, [pc, #108]	; (30004588 <BOOT_PSRAM_Init+0x74>)
3000451a:	a801      	add	r0, sp, #4
3000451c:	4798      	blx	r3
3000451e:	a801      	add	r0, sp, #4
30004520:	4b1a      	ldr	r3, [pc, #104]	; (3000458c <BOOT_PSRAM_Init+0x78>)
30004522:	4798      	blx	r3
30004524:	4b1a      	ldr	r3, [pc, #104]	; (30004590 <BOOT_PSRAM_Init+0x7c>)
30004526:	4798      	blx	r3
30004528:	4b1a      	ldr	r3, [pc, #104]	; (30004594 <BOOT_PSRAM_Init+0x80>)
3000452a:	4798      	blx	r3
3000452c:	2801      	cmp	r0, #1
3000452e:	d01d      	beq.n	3000456c <BOOT_PSRAM_Init+0x58>
30004530:	4b19      	ldr	r3, [pc, #100]	; (30004598 <BOOT_PSRAM_Init+0x84>)
30004532:	2004      	movs	r0, #4
30004534:	4a19      	ldr	r2, [pc, #100]	; (3000459c <BOOT_PSRAM_Init+0x88>)
30004536:	4619      	mov	r1, r3
30004538:	f007 f80c 	bl	3000b554 <rtk_log_write>
3000453c:	4b18      	ldr	r3, [pc, #96]	; (300045a0 <BOOT_PSRAM_Init+0x8c>)
3000453e:	4798      	blx	r3
30004540:	4b18      	ldr	r3, [pc, #96]	; (300045a4 <BOOT_PSRAM_Init+0x90>)
30004542:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004546:	009b      	lsls	r3, r3, #2
30004548:	d517      	bpl.n	3000457a <BOOT_PSRAM_Init+0x66>
3000454a:	4a17      	ldr	r2, [pc, #92]	; (300045a8 <BOOT_PSRAM_Init+0x94>)
3000454c:	4b17      	ldr	r3, [pc, #92]	; (300045ac <BOOT_PSRAM_Init+0x98>)
3000454e:	6812      	ldr	r2, [r2, #0]
30004550:	605a      	str	r2, [r3, #4]
30004552:	4a17      	ldr	r2, [pc, #92]	; (300045b0 <BOOT_PSRAM_Init+0x9c>)
30004554:	f04f 0c20 	mov.w	ip, #32
30004558:	2101      	movs	r1, #1
3000455a:	4b16      	ldr	r3, [pc, #88]	; (300045b4 <BOOT_PSRAM_Init+0xa0>)
3000455c:	6952      	ldr	r2, [r2, #20]
3000455e:	4608      	mov	r0, r1
30004560:	fbbc f2f2 	udiv	r2, ip, r2
30004564:	4798      	blx	r3
30004566:	b009      	add	sp, #36	; 0x24
30004568:	f85d fb04 	ldr.w	pc, [sp], #4
3000456c:	4b12      	ldr	r3, [pc, #72]	; (300045b8 <BOOT_PSRAM_Init+0xa4>)
3000456e:	4798      	blx	r3
30004570:	4b0c      	ldr	r3, [pc, #48]	; (300045a4 <BOOT_PSRAM_Init+0x90>)
30004572:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004576:	009b      	lsls	r3, r3, #2
30004578:	d4e7      	bmi.n	3000454a <BOOT_PSRAM_Init+0x36>
3000457a:	4b10      	ldr	r3, [pc, #64]	; (300045bc <BOOT_PSRAM_Init+0xa8>)
3000457c:	4798      	blx	r3
3000457e:	4a0b      	ldr	r2, [pc, #44]	; (300045ac <BOOT_PSRAM_Init+0x98>)
30004580:	4b09      	ldr	r3, [pc, #36]	; (300045a8 <BOOT_PSRAM_Init+0x94>)
30004582:	6852      	ldr	r2, [r2, #4]
30004584:	601a      	str	r2, [r3, #0]
30004586:	e7e4      	b.n	30004552 <BOOT_PSRAM_Init+0x3e>
30004588:	300083f1 	.word	0x300083f1
3000458c:	30008411 	.word	0x30008411
30004590:	30008479 	.word	0x30008479
30004594:	300074e5 	.word	0x300074e5
30004598:	3000bcf8 	.word	0x3000bcf8
3000459c:	3000bed4 	.word	0x3000bed4
300045a0:	3000871d 	.word	0x3000871d
300045a4:	42008000 	.word	0x42008000
300045a8:	3000f448 	.word	0x3000f448
300045ac:	41002000 	.word	0x41002000
300045b0:	3000f340 	.word	0x3000f340
300045b4:	300089fd 	.word	0x300089fd
300045b8:	30008645 	.word	0x30008645
300045bc:	30008759 	.word	0x30008759

300045c0 <BOOT_WakeFromPG>:
300045c0:	b538      	push	{r3, r4, r5, lr}
300045c2:	4c46      	ldr	r4, [pc, #280]	; (300046dc <BOOT_WakeFromPG+0x11c>)
300045c4:	f002 fe12 	bl	300071ec <BOOT_RAM_TZCfg>
300045c8:	69a3      	ldr	r3, [r4, #24]
300045ca:	b10b      	cbz	r3, 300045d0 <BOOT_WakeFromPG+0x10>
300045cc:	4a44      	ldr	r2, [pc, #272]	; (300046e0 <BOOT_WakeFromPG+0x120>)
300045ce:	6093      	str	r3, [r2, #8]
300045d0:	4b44      	ldr	r3, [pc, #272]	; (300046e4 <BOOT_WakeFromPG+0x124>)
300045d2:	f06f 0c1a 	mvn.w	ip, #26
300045d6:	4844      	ldr	r0, [pc, #272]	; (300046e8 <BOOT_WakeFromPG+0x128>)
300045d8:	f103 0e50 	add.w	lr, r3, #80	; 0x50
300045dc:	ebac 0c04 	sub.w	ip, ip, r4
300045e0:	eb0c 0203 	add.w	r2, ip, r3
300045e4:	f813 1f01 	ldrb.w	r1, [r3, #1]!
300045e8:	4402      	add	r2, r0
300045ea:	4573      	cmp	r3, lr
300045ec:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
300045f0:	d1f6      	bne.n	300045e0 <BOOT_WakeFromPG+0x20>
300045f2:	6822      	ldr	r2, [r4, #0]
300045f4:	4b3a      	ldr	r3, [pc, #232]	; (300046e0 <BOOT_WakeFromPG+0x120>)
300045f6:	f422 7280 	bic.w	r2, r2, #256	; 0x100
300045fa:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
300045fe:	6002      	str	r2, [r0, #0]
30004600:	6045      	str	r5, [r0, #4]
30004602:	6081      	str	r1, [r0, #8]
30004604:	6a59      	ldr	r1, [r3, #36]	; 0x24
30004606:	4a39      	ldr	r2, [pc, #228]	; (300046ec <BOOT_WakeFromPG+0x12c>)
30004608:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000460c:	4d38      	ldr	r5, [pc, #224]	; (300046f0 <BOOT_WakeFromPG+0x130>)
3000460e:	6259      	str	r1, [r3, #36]	; 0x24
30004610:	6a51      	ldr	r1, [r2, #36]	; 0x24
30004612:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004616:	6251      	str	r1, [r2, #36]	; 0x24
30004618:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000461c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004620:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004624:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004628:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000462c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004630:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004634:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004638:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000463c:	f7ff fd66 	bl	3000410c <BOOT_CACHEWRR_Set>
30004640:	4b2c      	ldr	r3, [pc, #176]	; (300046f4 <BOOT_WakeFromPG+0x134>)
30004642:	6818      	ldr	r0, [r3, #0]
30004644:	f7ff fd90 	bl	30004168 <BOOT_TCMSet>
30004648:	2201      	movs	r2, #1
3000464a:	492b      	ldr	r1, [pc, #172]	; (300046f8 <BOOT_WakeFromPG+0x138>)
3000464c:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30004650:	47a8      	blx	r5
30004652:	4b2a      	ldr	r3, [pc, #168]	; (300046fc <BOOT_WakeFromPG+0x13c>)
30004654:	4798      	blx	r3
30004656:	4b2a      	ldr	r3, [pc, #168]	; (30004700 <BOOT_WakeFromPG+0x140>)
30004658:	4798      	blx	r3
3000465a:	2801      	cmp	r0, #1
3000465c:	4604      	mov	r4, r0
3000465e:	d026      	beq.n	300046ae <BOOT_WakeFromPG+0xee>
30004660:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30004664:	2201      	movs	r2, #1
30004666:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
3000466a:	47a8      	blx	r5
3000466c:	4a25      	ldr	r2, [pc, #148]	; (30004704 <BOOT_WakeFromPG+0x144>)
3000466e:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30004672:	f043 0302 	orr.w	r3, r3, #2
30004676:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
3000467a:	f7ff ff4b 	bl	30004514 <BOOT_PSRAM_Init>
3000467e:	2096      	movs	r0, #150	; 0x96
30004680:	4b21      	ldr	r3, [pc, #132]	; (30004708 <BOOT_WakeFromPG+0x148>)
30004682:	4798      	blx	r3
30004684:	4a21      	ldr	r2, [pc, #132]	; (3000470c <BOOT_WakeFromPG+0x14c>)
30004686:	4c19      	ldr	r4, [pc, #100]	; (300046ec <BOOT_WakeFromPG+0x12c>)
30004688:	6893      	ldr	r3, [r2, #8]
3000468a:	4921      	ldr	r1, [pc, #132]	; (30004710 <BOOT_WakeFromPG+0x150>)
3000468c:	6852      	ldr	r2, [r2, #4]
3000468e:	4821      	ldr	r0, [pc, #132]	; (30004714 <BOOT_WakeFromPG+0x154>)
30004690:	605a      	str	r2, [r3, #4]
30004692:	60a3      	str	r3, [r4, #8]
30004694:	4a20      	ldr	r2, [pc, #128]	; (30004718 <BOOT_WakeFromPG+0x158>)
30004696:	61c8      	str	r0, [r1, #28]
30004698:	f382 8888 	msr	MSP_NS, r2
3000469c:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
300046a0:	f382 8809 	msr	PSP, r2
300046a4:	6858      	ldr	r0, [r3, #4]
300046a6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300046aa:	f7ff bc73 	b.w	30003f94 <BOOT_NsStart>
300046ae:	4602      	mov	r2, r0
300046b0:	491a      	ldr	r1, [pc, #104]	; (3000471c <BOOT_WakeFromPG+0x15c>)
300046b2:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300046b6:	47a8      	blx	r5
300046b8:	4622      	mov	r2, r4
300046ba:	4919      	ldr	r1, [pc, #100]	; (30004720 <BOOT_WakeFromPG+0x160>)
300046bc:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300046c0:	47a8      	blx	r5
300046c2:	f7ff fee9 	bl	30004498 <BOOT_DDR_Init>
300046c6:	4a0f      	ldr	r2, [pc, #60]	; (30004704 <BOOT_WakeFromPG+0x144>)
300046c8:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300046cc:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300046d0:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300046d4:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300046d8:	e7d4      	b.n	30004684 <BOOT_WakeFromPG+0xc4>
300046da:	bf00      	nop
300046dc:	3000f3dc 	.word	0x3000f3dc
300046e0:	e000ed00 	.word	0xe000ed00
300046e4:	3000f3f7 	.word	0x3000f3f7
300046e8:	e000e100 	.word	0xe000e100
300046ec:	e002ed00 	.word	0xe002ed00
300046f0:	0000b479 	.word	0x0000b479
300046f4:	3000df40 	.word	0x3000df40
300046f8:	40001000 	.word	0x40001000
300046fc:	0000d835 	.word	0x0000d835
30004700:	3000740d 	.word	0x3000740d
30004704:	42008000 	.word	0x42008000
30004708:	00009b2d 	.word	0x00009b2d
3000470c:	60000020 	.word	0x60000020
30004710:	30000000 	.word	0x30000000
30004714:	30003a81 	.word	0x30003a81
30004718:	2001bffc 	.word	0x2001bffc
3000471c:	40080000 	.word	0x40080000
30004720:	40040000 	.word	0x40040000

30004724 <BOOT_SCBConfig_HP>:
30004724:	4b0e      	ldr	r3, [pc, #56]	; (30004760 <BOOT_SCBConfig_HP+0x3c>)
30004726:	4a0f      	ldr	r2, [pc, #60]	; (30004764 <BOOT_SCBConfig_HP+0x40>)
30004728:	6a59      	ldr	r1, [r3, #36]	; 0x24
3000472a:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000472e:	6259      	str	r1, [r3, #36]	; 0x24
30004730:	6a51      	ldr	r1, [r2, #36]	; 0x24
30004732:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004736:	6251      	str	r1, [r2, #36]	; 0x24
30004738:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000473c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004740:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004744:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004748:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000474c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004750:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004754:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004758:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000475c:	4770      	bx	lr
3000475e:	bf00      	nop
30004760:	e000ed00 	.word	0xe000ed00
30004764:	e002ed00 	.word	0xe002ed00

30004768 <BOOT_SCBVTORBackup_HP>:
30004768:	4a02      	ldr	r2, [pc, #8]	; (30004774 <BOOT_SCBVTORBackup_HP+0xc>)
3000476a:	4b03      	ldr	r3, [pc, #12]	; (30004778 <BOOT_SCBVTORBackup_HP+0x10>)
3000476c:	6892      	ldr	r2, [r2, #8]
3000476e:	619a      	str	r2, [r3, #24]
30004770:	4770      	bx	lr
30004772:	bf00      	nop
30004774:	e000ed00 	.word	0xe000ed00
30004778:	3000f3dc 	.word	0x3000f3dc

3000477c <BOOT_SCBVTORReFill_HP>:
3000477c:	4a02      	ldr	r2, [pc, #8]	; (30004788 <BOOT_SCBVTORReFill_HP+0xc>)
3000477e:	4b03      	ldr	r3, [pc, #12]	; (3000478c <BOOT_SCBVTORReFill_HP+0x10>)
30004780:	6992      	ldr	r2, [r2, #24]
30004782:	609a      	str	r2, [r3, #8]
30004784:	4770      	bx	lr
30004786:	bf00      	nop
30004788:	3000f3dc 	.word	0x3000f3dc
3000478c:	e000ed00 	.word	0xe000ed00

30004790 <BOOT_NVICBackup_HP>:
30004790:	491a      	ldr	r1, [pc, #104]	; (300047fc <BOOT_NVICBackup_HP+0x6c>)
30004792:	f06f 001a 	mvn.w	r0, #26
30004796:	b410      	push	{r4}
30004798:	4c19      	ldr	r4, [pc, #100]	; (30004800 <BOOT_NVICBackup_HP+0x70>)
3000479a:	680a      	ldr	r2, [r1, #0]
3000479c:	f104 031b 	add.w	r3, r4, #27
300047a0:	1b00      	subs	r0, r0, r4
300047a2:	6022      	str	r2, [r4, #0]
300047a4:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
300047a8:	684a      	ldr	r2, [r1, #4]
300047aa:	6062      	str	r2, [r4, #4]
300047ac:	688a      	ldr	r2, [r1, #8]
300047ae:	60a2      	str	r2, [r4, #8]
300047b0:	18c2      	adds	r2, r0, r3
300047b2:	440a      	add	r2, r1
300047b4:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
300047b8:	f803 2f01 	strb.w	r2, [r3, #1]!
300047bc:	4563      	cmp	r3, ip
300047be:	d1f7      	bne.n	300047b0 <BOOT_NVICBackup_HP+0x20>
300047c0:	4b10      	ldr	r3, [pc, #64]	; (30004804 <BOOT_NVICBackup_HP+0x74>)
300047c2:	695b      	ldr	r3, [r3, #20]
300047c4:	03db      	lsls	r3, r3, #15
300047c6:	d512      	bpl.n	300047ee <BOOT_NVICBackup_HP+0x5e>
300047c8:	f014 011f 	ands.w	r1, r4, #31
300047cc:	4b0c      	ldr	r3, [pc, #48]	; (30004800 <BOOT_NVICBackup_HP+0x70>)
300047ce:	d111      	bne.n	300047f4 <BOOT_NVICBackup_HP+0x64>
300047d0:	216c      	movs	r1, #108	; 0x6c
300047d2:	f3bf 8f4f 	dsb	sy
300047d6:	480b      	ldr	r0, [pc, #44]	; (30004804 <BOOT_NVICBackup_HP+0x74>)
300047d8:	4419      	add	r1, r3
300047da:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
300047de:	3320      	adds	r3, #32
300047e0:	1aca      	subs	r2, r1, r3
300047e2:	2a00      	cmp	r2, #0
300047e4:	dcf9      	bgt.n	300047da <BOOT_NVICBackup_HP+0x4a>
300047e6:	f3bf 8f4f 	dsb	sy
300047ea:	f3bf 8f6f 	isb	sy
300047ee:	f85d 4b04 	ldr.w	r4, [sp], #4
300047f2:	4770      	bx	lr
300047f4:	f024 031f 	bic.w	r3, r4, #31
300047f8:	316c      	adds	r1, #108	; 0x6c
300047fa:	e7ea      	b.n	300047d2 <BOOT_NVICBackup_HP+0x42>
300047fc:	e000e100 	.word	0xe000e100
30004800:	3000f3dc 	.word	0x3000f3dc
30004804:	e000ed00 	.word	0xe000ed00

30004808 <BOOT_NVICReFill_HP>:
30004808:	b510      	push	{r4, lr}
3000480a:	f06f 0c1a 	mvn.w	ip, #26
3000480e:	4c0d      	ldr	r4, [pc, #52]	; (30004844 <BOOT_NVICReFill_HP+0x3c>)
30004810:	480d      	ldr	r0, [pc, #52]	; (30004848 <BOOT_NVICReFill_HP+0x40>)
30004812:	f104 031b 	add.w	r3, r4, #27
30004816:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
3000481a:	ebac 0c04 	sub.w	ip, ip, r4
3000481e:	eb0c 0203 	add.w	r2, ip, r3
30004822:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30004826:	4402      	add	r2, r0
30004828:	4573      	cmp	r3, lr
3000482a:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
3000482e:	d1f6      	bne.n	3000481e <BOOT_NVICReFill_HP+0x16>
30004830:	e9d4 3100 	ldrd	r3, r1, [r4]
30004834:	68a2      	ldr	r2, [r4, #8]
30004836:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000483a:	6003      	str	r3, [r0, #0]
3000483c:	6041      	str	r1, [r0, #4]
3000483e:	6082      	str	r2, [r0, #8]
30004840:	bd10      	pop	{r4, pc}
30004842:	bf00      	nop
30004844:	3000f3dc 	.word	0x3000f3dc
30004848:	e000e100 	.word	0xe000e100

3000484c <BOOT_ImgCopy>:
3000484c:	b510      	push	{r4, lr}
3000484e:	b082      	sub	sp, #8
30004850:	4b09      	ldr	r3, [pc, #36]	; (30004878 <BOOT_ImgCopy+0x2c>)
30004852:	4604      	mov	r4, r0
30004854:	e9cd 2100 	strd	r2, r1, [sp]
30004858:	4798      	blx	r3
3000485a:	e9dd 2100 	ldrd	r2, r1, [sp]
3000485e:	b128      	cbz	r0, 3000486c <BOOT_ImgCopy+0x20>
30004860:	4620      	mov	r0, r4
30004862:	4b06      	ldr	r3, [pc, #24]	; (3000487c <BOOT_ImgCopy+0x30>)
30004864:	b002      	add	sp, #8
30004866:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000486a:	4718      	bx	r3
3000486c:	4620      	mov	r0, r4
3000486e:	b002      	add	sp, #8
30004870:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004874:	f003 bb76 	b.w	30007f64 <NandImgCopy>
30004878:	3000921d 	.word	0x3000921d
3000487c:	00012a1d 	.word	0x00012a1d

30004880 <BOOT_LoadImages>:
30004880:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004884:	4bb8      	ldr	r3, [pc, #736]	; (30004b68 <BOOT_LoadImages+0x2e8>)
30004886:	2400      	movs	r4, #0
30004888:	2104      	movs	r1, #4
3000488a:	2701      	movs	r7, #1
3000488c:	ed2d 8b02 	vpush	{d8}
30004890:	b0bb      	sub	sp, #236	; 0xec
30004892:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30004896:	4bb5      	ldr	r3, [pc, #724]	; (30004b6c <BOOT_LoadImages+0x2ec>)
30004898:	aa13      	add	r2, sp, #76	; 0x4c
3000489a:	9405      	str	r4, [sp, #20]
3000489c:	9407      	str	r4, [sp, #28]
3000489e:	940d      	str	r4, [sp, #52]	; 0x34
300048a0:	4798      	blx	r3
300048a2:	f89d 304e 	ldrb.w	r3, [sp, #78]	; 0x4e
300048a6:	409f      	lsls	r7, r3
300048a8:	f5b7 0f00 	cmp.w	r7, #8388608	; 0x800000
300048ac:	f000 81e7 	beq.w	30004c7e <BOOT_LoadImages+0x3fe>
300048b0:	f1b7 7f80 	cmp.w	r7, #16777216	; 0x1000000
300048b4:	f000 83d9 	beq.w	3000506a <BOOT_LoadImages+0x7ea>
300048b8:	f1b7 7f00 	cmp.w	r7, #33554432	; 0x2000000
300048bc:	f000 83db 	beq.w	30005076 <BOOT_LoadImages+0x7f6>
300048c0:	f8df b2f8 	ldr.w	fp, [pc, #760]	; 30004bbc <BOOT_LoadImages+0x33c>
300048c4:	2310      	movs	r3, #16
300048c6:	f04f 0800 	mov.w	r8, #0
300048ca:	48a9      	ldr	r0, [pc, #676]	; (30004b70 <BOOT_LoadImages+0x2f0>)
300048cc:	44bb      	add	fp, r7
300048ce:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
300048d2:	4da8      	ldr	r5, [pc, #672]	; (30004b74 <BOOT_LoadImages+0x2f4>)
300048d4:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
300048d8:	e9cd 8816 	strd	r8, r8, [sp, #88]	; 0x58
300048dc:	f7ff fa16 	bl	30003d0c <mcc_init_crc32>
300048e0:	f8db 3004 	ldr.w	r3, [fp, #4]
300048e4:	443d      	add	r5, r7
300048e6:	f89b 200c 	ldrb.w	r2, [fp, #12]
300048ea:	930e      	str	r3, [sp, #56]	; 0x38
300048ec:	3b01      	subs	r3, #1
300048ee:	9208      	str	r2, [sp, #32]
300048f0:	3303      	adds	r3, #3
300048f2:	686a      	ldr	r2, [r5, #4]
300048f4:	68ac      	ldr	r4, [r5, #8]
300048f6:	7b2e      	ldrb	r6, [r5, #12]
300048f8:	9206      	str	r2, [sp, #24]
300048fa:	f200 812e 	bhi.w	30004b5a <BOOT_LoadImages+0x2da>
300048fe:	f8db 3008 	ldr.w	r3, [fp, #8]
30004902:	9a06      	ldr	r2, [sp, #24]
30004904:	3b01      	subs	r3, #1
30004906:	f102 39ff 	add.w	r9, r2, #4294967295	; 0xffffffff
3000490a:	3303      	adds	r3, #3
3000490c:	d86f      	bhi.n	300049ee <BOOT_LoadImages+0x16e>
3000490e:	9b08      	ldr	r3, [sp, #32]
30004910:	2b01      	cmp	r3, #1
30004912:	f240 8369 	bls.w	30004fe8 <BOOT_LoadImages+0x768>
30004916:	4898      	ldr	r0, [pc, #608]	; (30004b78 <BOOT_LoadImages+0x2f8>)
30004918:	f007 f902 	bl	3000bb20 <__DiagPrintf_veneer>
3000491c:	f119 0f03 	cmn.w	r9, #3
30004920:	d86b      	bhi.n	300049fa <BOOT_LoadImages+0x17a>
30004922:	3c01      	subs	r4, #1
30004924:	3403      	adds	r4, #3
30004926:	f200 814d 	bhi.w	30004bc4 <BOOT_LoadImages+0x344>
3000492a:	2e01      	cmp	r6, #1
3000492c:	f200 83a1 	bhi.w	30005072 <BOOT_LoadImages+0x7f2>
30004930:	9906      	ldr	r1, [sp, #24]
30004932:	4892      	ldr	r0, [pc, #584]	; (30004b7c <BOOT_LoadImages+0x2fc>)
30004934:	f007 f8f4 	bl	3000bb20 <__DiagPrintf_veneer>
30004938:	f8db 300d 	ldr.w	r3, [fp, #13]
3000493c:	4c8c      	ldr	r4, [pc, #560]	; (30004b70 <BOOT_LoadImages+0x2f0>)
3000493e:	930a      	str	r3, [sp, #40]	; 0x28
30004940:	f8db 3011 	ldr.w	r3, [fp, #17]
30004944:	930f      	str	r3, [sp, #60]	; 0x3c
30004946:	f8d5 300d 	ldr.w	r3, [r5, #13]
3000494a:	9309      	str	r3, [sp, #36]	; 0x24
3000494c:	f8d5 3011 	ldr.w	r3, [r5, #17]
30004950:	930c      	str	r3, [sp, #48]	; 0x30
30004952:	2300      	movs	r3, #0
30004954:	930b      	str	r3, [sp, #44]	; 0x2c
30004956:	f8df 8268 	ldr.w	r8, [pc, #616]	; 30004bc0 <BOOT_LoadImages+0x340>
3000495a:	f04f 0901 	mov.w	r9, #1
3000495e:	f107 6600 	add.w	r6, r7, #134217728	; 0x8000000
30004962:	44b8      	add	r8, r7
30004964:	6829      	ldr	r1, [r5, #0]
30004966:	4640      	mov	r0, r8
30004968:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
3000496c:	f810 3b01 	ldrb.w	r3, [r0], #1
30004970:	4053      	eors	r3, r2
30004972:	42b0      	cmp	r0, r6
30004974:	b2db      	uxtb	r3, r3
30004976:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000497a:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000497e:	d1f5      	bne.n	3000496c <BOOT_LoadImages+0xec>
30004980:	43d2      	mvns	r2, r2
30004982:	4291      	cmp	r1, r2
30004984:	f000 84a9 	beq.w	300052da <BOOT_LoadImages+0xa5a>
30004988:	464b      	mov	r3, r9
3000498a:	f109 0901 	add.w	r9, r9, #1
3000498e:	487c      	ldr	r0, [pc, #496]	; (30004b80 <BOOT_LoadImages+0x300>)
30004990:	f007 f8c6 	bl	3000bb20 <__DiagPrintf_veneer>
30004994:	f1b9 0f04 	cmp.w	r9, #4
30004998:	d1e4      	bne.n	30004964 <BOOT_LoadImages+0xe4>
3000499a:	f04f 0900 	mov.w	r9, #0
3000499e:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300049a0:	b383      	cbz	r3, 30004a04 <BOOT_LoadImages+0x184>
300049a2:	9a0a      	ldr	r2, [sp, #40]	; 0x28
300049a4:	f89b 300c 	ldrb.w	r3, [fp, #12]
300049a8:	4296      	cmp	r6, r2
300049aa:	9308      	str	r3, [sp, #32]
300049ac:	d327      	bcc.n	300049fe <BOOT_LoadImages+0x17e>
300049ae:	f06f 4378 	mvn.w	r3, #4160749568	; 0xf8000000
300049b2:	429a      	cmp	r2, r3
300049b4:	d923      	bls.n	300049fe <BOOT_LoadImages+0x17e>
300049b6:	9a0f      	ldr	r2, [sp, #60]	; 0x3c
300049b8:	4296      	cmp	r6, r2
300049ba:	f0c0 8164 	bcc.w	30004c86 <BOOT_LoadImages+0x406>
300049be:	429a      	cmp	r2, r3
300049c0:	f240 8161 	bls.w	30004c86 <BOOT_LoadImages+0x406>
300049c4:	f1b9 0f00 	cmp.w	r9, #0
300049c8:	f040 8492 	bne.w	300052f0 <BOOT_LoadImages+0xa70>
300049cc:	9b08      	ldr	r3, [sp, #32]
300049ce:	9305      	str	r3, [sp, #20]
300049d0:	486c      	ldr	r0, [pc, #432]	; (30004b84 <BOOT_LoadImages+0x304>)
300049d2:	f007 f8a5 	bl	3000bb20 <__DiagPrintf_veneer>
300049d6:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300049d8:	9a0e      	ldr	r2, [sp, #56]	; 0x38
300049da:	f8c4 3400 	str.w	r3, [r4, #1024]	; 0x400
300049de:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300049e0:	9206      	str	r2, [sp, #24]
300049e2:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
300049e6:	462b      	mov	r3, r5
300049e8:	465d      	mov	r5, fp
300049ea:	469b      	mov	fp, r3
300049ec:	e032      	b.n	30004a54 <BOOT_LoadImages+0x1d4>
300049ee:	4866      	ldr	r0, [pc, #408]	; (30004b88 <BOOT_LoadImages+0x308>)
300049f0:	f007 f896 	bl	3000bb20 <__DiagPrintf_veneer>
300049f4:	f119 0f03 	cmn.w	r9, #3
300049f8:	d993      	bls.n	30004922 <BOOT_LoadImages+0xa2>
300049fa:	4864      	ldr	r0, [pc, #400]	; (30004b8c <BOOT_LoadImages+0x30c>)
300049fc:	e0e3      	b.n	30004bc6 <BOOT_LoadImages+0x346>
300049fe:	4864      	ldr	r0, [pc, #400]	; (30004b90 <BOOT_LoadImages+0x310>)
30004a00:	f007 f88e 	bl	3000bb20 <__DiagPrintf_veneer>
30004a04:	f1b9 0f00 	cmp.w	r9, #0
30004a08:	f000 80eb 	beq.w	30004be2 <BOOT_LoadImages+0x362>
30004a0c:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004a0e:	42b3      	cmp	r3, r6
30004a10:	f200 83fa 	bhi.w	30005208 <BOOT_LoadImages+0x988>
30004a14:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004a18:	bf2c      	ite	cs
30004a1a:	2300      	movcs	r3, #0
30004a1c:	2301      	movcc	r3, #1
30004a1e:	2b00      	cmp	r3, #0
30004a20:	f040 83f2 	bne.w	30005208 <BOOT_LoadImages+0x988>
30004a24:	4699      	mov	r9, r3
30004a26:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004a28:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004a2c:	f0c0 812f 	bcc.w	30004c8e <BOOT_LoadImages+0x40e>
30004a30:	42b3      	cmp	r3, r6
30004a32:	f200 812c 	bhi.w	30004c8e <BOOT_LoadImages+0x40e>
30004a36:	7b2b      	ldrb	r3, [r5, #12]
30004a38:	9305      	str	r3, [sp, #20]
30004a3a:	f1b9 0f00 	cmp.w	r9, #0
30004a3e:	f040 81f9 	bne.w	30004e34 <BOOT_LoadImages+0x5b4>
30004a42:	4854      	ldr	r0, [pc, #336]	; (30004b94 <BOOT_LoadImages+0x314>)
30004a44:	f007 f86c 	bl	3000bb20 <__DiagPrintf_veneer>
30004a48:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004a4a:	f8c4 3400 	str.w	r3, [r4, #1024]	; 0x400
30004a4e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004a50:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004a54:	9905      	ldr	r1, [sp, #20]
30004a56:	4850      	ldr	r0, [pc, #320]	; (30004b98 <BOOT_LoadImages+0x318>)
30004a58:	f007 f862 	bl	3000bb20 <__DiagPrintf_veneer>
30004a5c:	4b4f      	ldr	r3, [pc, #316]	; (30004b9c <BOOT_LoadImages+0x31c>)
30004a5e:	4798      	blx	r3
30004a60:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004a64:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30004a68:	f8d1 300a 	ldr.w	r3, [r1, #10]
30004a6c:	6809      	ldr	r1, [r1, #0]
30004a6e:	330c      	adds	r3, #12
30004a70:	f8d2 a000 	ldr.w	sl, [r2]
30004a74:	9110      	str	r1, [sp, #64]	; 0x40
30004a76:	42bb      	cmp	r3, r7
30004a78:	bf28      	it	cs
30004a7a:	463b      	movcs	r3, r7
30004a7c:	9304      	str	r3, [sp, #16]
30004a7e:	f8d2 300a 	ldr.w	r3, [r2, #10]
30004a82:	330c      	adds	r3, #12
30004a84:	429f      	cmp	r7, r3
30004a86:	bf28      	it	cs
30004a88:	461f      	movcs	r7, r3
30004a8a:	9703      	str	r7, [sp, #12]
30004a8c:	2800      	cmp	r0, #0
30004a8e:	f040 8162 	bne.w	30004d56 <BOOT_LoadImages+0x4d6>
30004a92:	4b43      	ldr	r3, [pc, #268]	; (30004ba0 <BOOT_LoadImages+0x320>)
30004a94:	4681      	mov	r9, r0
30004a96:	9308      	str	r3, [sp, #32]
30004a98:	4b42      	ldr	r3, [pc, #264]	; (30004ba4 <BOOT_LoadImages+0x324>)
30004a9a:	9309      	str	r3, [sp, #36]	; 0x24
30004a9c:	4b42      	ldr	r3, [pc, #264]	; (30004ba8 <BOOT_LoadImages+0x328>)
30004a9e:	930a      	str	r3, [sp, #40]	; 0x28
30004aa0:	9b05      	ldr	r3, [sp, #20]
30004aa2:	2b00      	cmp	r3, #0
30004aa4:	f000 8228 	beq.w	30004ef8 <BOOT_LoadImages+0x678>
30004aa8:	f04f 0800 	mov.w	r8, #0
30004aac:	4e3f      	ldr	r6, [pc, #252]	; (30004bac <BOOT_LoadImages+0x32c>)
30004aae:	f8d4 0404 	ldr.w	r0, [r4, #1028]	; 0x404
30004ab2:	9a03      	ldr	r2, [sp, #12]
30004ab4:	1d03      	adds	r3, r0, #4
30004ab6:	2a00      	cmp	r2, #0
30004ab8:	f000 81f4 	beq.w	30004ea4 <BOOT_LoadImages+0x624>
30004abc:	f102 0c04 	add.w	ip, r2, #4
30004ac0:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004ac4:	4484      	add	ip, r0
30004ac6:	f813 1b01 	ldrb.w	r1, [r3], #1
30004aca:	4051      	eors	r1, r2
30004acc:	4563      	cmp	r3, ip
30004ace:	b2c9      	uxtb	r1, r1
30004ad0:	f854 1021 	ldr.w	r1, [r4, r1, lsl #2]
30004ad4:	ea81 2212 	eor.w	r2, r1, r2, lsr #8
30004ad8:	d1f5      	bne.n	30004ac6 <BOOT_LoadImages+0x246>
30004ada:	43d7      	mvns	r7, r2
30004adc:	f108 0801 	add.w	r8, r8, #1
30004ae0:	45ba      	cmp	sl, r7
30004ae2:	fa5f f888 	uxtb.w	r8, r8
30004ae6:	f000 81e5 	beq.w	30004eb4 <BOOT_LoadImages+0x634>
30004aea:	f8d0 300a 	ldr.w	r3, [r0, #10]
30004aee:	3301      	adds	r3, #1
30004af0:	f000 8123 	beq.w	30004d3a <BOOT_LoadImages+0x4ba>
30004af4:	4643      	mov	r3, r8
30004af6:	463a      	mov	r2, r7
30004af8:	4651      	mov	r1, sl
30004afa:	4630      	mov	r0, r6
30004afc:	f007 f810 	bl	3000bb20 <__DiagPrintf_veneer>
30004b00:	f1b8 0f02 	cmp.w	r8, #2
30004b04:	d9d3      	bls.n	30004aae <BOOT_LoadImages+0x22e>
30004b06:	4651      	mov	r1, sl
30004b08:	463a      	mov	r2, r7
30004b0a:	4829      	ldr	r0, [pc, #164]	; (30004bb0 <BOOT_LoadImages+0x330>)
30004b0c:	f007 f808 	bl	3000bb20 <__DiagPrintf_veneer>
30004b10:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30004b14:	2002      	movs	r0, #2
30004b16:	f7ff f85b 	bl	30003bd0 <FLASH_Erase_With_Lock>
30004b1a:	2302      	movs	r3, #2
30004b1c:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
30004b20:	2300      	movs	r3, #0
30004b22:	930d      	str	r3, [sp, #52]	; 0x34
30004b24:	af1a      	add	r7, sp, #104	; 0x68
30004b26:	2280      	movs	r2, #128	; 0x80
30004b28:	21ff      	movs	r1, #255	; 0xff
30004b2a:	4638      	mov	r0, r7
30004b2c:	f006 ff98 	bl	3000ba60 <____wrap_memset_veneer>
30004b30:	2280      	movs	r2, #128	; 0x80
30004b32:	4629      	mov	r1, r5
30004b34:	4638      	mov	r0, r7
30004b36:	f006 ffdb 	bl	3000baf0 <____wrap_memcpy_veneer>
30004b3a:	f1b9 0f00 	cmp.w	r9, #0
30004b3e:	f000 829e 	beq.w	3000507e <BOOT_LoadImages+0x7fe>
30004b42:	9b05      	ldr	r3, [sp, #20]
30004b44:	f1a3 0301 	sub.w	r3, r3, #1
30004b48:	fab3 f383 	clz	r3, r3
30004b4c:	095b      	lsrs	r3, r3, #5
30004b4e:	f88d 3074 	strb.w	r3, [sp, #116]	; 0x74
30004b52:	4818      	ldr	r0, [pc, #96]	; (30004bb4 <BOOT_LoadImages+0x334>)
30004b54:	f006 ffe4 	bl	3000bb20 <__DiagPrintf_veneer>
30004b58:	e7fe      	b.n	30004b58 <BOOT_LoadImages+0x2d8>
30004b5a:	4817      	ldr	r0, [pc, #92]	; (30004bb8 <BOOT_LoadImages+0x338>)
30004b5c:	f006 ffe0 	bl	3000bb20 <__DiagPrintf_veneer>
30004b60:	9b06      	ldr	r3, [sp, #24]
30004b62:	f103 39ff 	add.w	r9, r3, #4294967295	; 0xffffffff
30004b66:	e6d9      	b.n	3000491c <BOOT_LoadImages+0x9c>
30004b68:	2001c01c 	.word	0x2001c01c
30004b6c:	00009db5 	.word	0x00009db5
30004b70:	3000e104 	.word	0x3000e104
30004b74:	07fff000 	.word	0x07fff000
30004b78:	3000bf4c 	.word	0x3000bf4c
30004b7c:	3000c524 	.word	0x3000c524
30004b80:	3000c05c 	.word	0x3000c05c
30004b84:	3000c0f4 	.word	0x3000c0f4
30004b88:	3000bf38 	.word	0x3000bf38
30004b8c:	3000c4d4 	.word	0x3000c4d4
30004b90:	3000c0a8 	.word	0x3000c0a8
30004b94:	3000c104 	.word	0x3000c104
30004b98:	3000c138 	.word	0x3000c138
30004b9c:	0000c149 	.word	0x0000c149
30004ba0:	3000f3d8 	.word	0x3000f3d8
30004ba4:	300068fd 	.word	0x300068fd
30004ba8:	3000df3f 	.word	0x3000df3f
30004bac:	3000c35c 	.word	0x3000c35c
30004bb0:	3000c3a8 	.word	0x3000c3a8
30004bb4:	3000c434 	.word	0x3000c434
30004bb8:	3000bf20 	.word	0x3000bf20
30004bbc:	07ffe000 	.word	0x07ffe000
30004bc0:	07fff004 	.word	0x07fff004
30004bc4:	48a7      	ldr	r0, [pc, #668]	; (30004e64 <BOOT_LoadImages+0x5e4>)
30004bc6:	f006 ffab 	bl	3000bb20 <__DiagPrintf_veneer>
30004bca:	f8db 300d 	ldr.w	r3, [fp, #13]
30004bce:	930a      	str	r3, [sp, #40]	; 0x28
30004bd0:	f8db 3011 	ldr.w	r3, [fp, #17]
30004bd4:	930f      	str	r3, [sp, #60]	; 0x3c
30004bd6:	f8d5 300d 	ldr.w	r3, [r5, #13]
30004bda:	9309      	str	r3, [sp, #36]	; 0x24
30004bdc:	f8d5 3011 	ldr.w	r3, [r5, #17]
30004be0:	930c      	str	r3, [sp, #48]	; 0x30
30004be2:	48a1      	ldr	r0, [pc, #644]	; (30004e68 <BOOT_LoadImages+0x5e8>)
30004be4:	f006 ff9c 	bl	3000bb20 <__DiagPrintf_veneer>
30004be8:	48a0      	ldr	r0, [pc, #640]	; (30004e6c <BOOT_LoadImages+0x5ec>)
30004bea:	f006 ff99 	bl	3000bb20 <__DiagPrintf_veneer>
30004bee:	463a      	mov	r2, r7
30004bf0:	990a      	ldr	r1, [sp, #40]	; 0x28
30004bf2:	ab14      	add	r3, sp, #80	; 0x50
30004bf4:	2000      	movs	r0, #0
30004bf6:	f7ff f8cf 	bl	30003d98 <recovery_check>
30004bfa:	463a      	mov	r2, r7
30004bfc:	4604      	mov	r4, r0
30004bfe:	990f      	ldr	r1, [sp, #60]	; 0x3c
30004c00:	ab15      	add	r3, sp, #84	; 0x54
30004c02:	2001      	movs	r0, #1
30004c04:	f7ff f8c8 	bl	30003d98 <recovery_check>
30004c08:	463a      	mov	r2, r7
30004c0a:	4606      	mov	r6, r0
30004c0c:	9909      	ldr	r1, [sp, #36]	; 0x24
30004c0e:	ab16      	add	r3, sp, #88	; 0x58
30004c10:	2002      	movs	r0, #2
30004c12:	f7ff f8c1 	bl	30003d98 <recovery_check>
30004c16:	463a      	mov	r2, r7
30004c18:	ab17      	add	r3, sp, #92	; 0x5c
30004c1a:	4607      	mov	r7, r0
30004c1c:	990c      	ldr	r1, [sp, #48]	; 0x30
30004c1e:	2003      	movs	r0, #3
30004c20:	f7ff f8ba 	bl	30003d98 <recovery_check>
30004c24:	4334      	orrs	r4, r6
30004c26:	d039      	beq.n	30004c9c <BOOT_LoadImages+0x41c>
30004c28:	ea57 0900 	orrs.w	r9, r7, r0
30004c2c:	e9dd 3214 	ldrd	r3, r2, [sp, #80]	; 0x50
30004c30:	d073      	beq.n	30004d1a <BOOT_LoadImages+0x49a>
30004c32:	4293      	cmp	r3, r2
30004c34:	e9dd 1016 	ldrd	r1, r0, [sp, #88]	; 0x58
30004c38:	f0c0 8469 	bcc.w	3000550e <BOOT_LoadImages+0xc8e>
30004c3c:	428b      	cmp	r3, r1
30004c3e:	d302      	bcc.n	30004c46 <BOOT_LoadImages+0x3c6>
30004c40:	4283      	cmp	r3, r0
30004c42:	f080 84f7 	bcs.w	30005634 <BOOT_LoadImages+0xdb4>
30004c46:	4293      	cmp	r3, r2
30004c48:	f000 8461 	beq.w	3000550e <BOOT_LoadImages+0xc8e>
30004c4c:	4293      	cmp	r3, r2
30004c4e:	bf38      	it	cc
30004c50:	4613      	movcc	r3, r2
30004c52:	428b      	cmp	r3, r1
30004c54:	f240 84c9 	bls.w	300055ea <BOOT_LoadImages+0xd6a>
30004c58:	428b      	cmp	r3, r1
30004c5a:	bf38      	it	cc
30004c5c:	460b      	movcc	r3, r1
30004c5e:	4283      	cmp	r3, r0
30004c60:	f240 843a 	bls.w	300054d8 <BOOT_LoadImages+0xc58>
30004c64:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30004c68:	2601      	movs	r6, #1
30004c6a:	f04f 0900 	mov.w	r9, #0
30004c6e:	9306      	str	r3, [sp, #24]
30004c70:	4b7f      	ldr	r3, [pc, #508]	; (30004e70 <BOOT_LoadImages+0x5f0>)
30004c72:	9308      	str	r3, [sp, #32]
30004c74:	4b7f      	ldr	r3, [pc, #508]	; (30004e74 <BOOT_LoadImages+0x5f4>)
30004c76:	9309      	str	r3, [sp, #36]	; 0x24
30004c78:	4b7f      	ldr	r3, [pc, #508]	; (30004e78 <BOOT_LoadImages+0x5f8>)
30004c7a:	930a      	str	r3, [sp, #40]	; 0x28
30004c7c:	e098      	b.n	30004db0 <BOOT_LoadImages+0x530>
30004c7e:	487f      	ldr	r0, [pc, #508]	; (30004e7c <BOOT_LoadImages+0x5fc>)
30004c80:	f006 ff4e 	bl	3000bb20 <__DiagPrintf_veneer>
30004c84:	e61c      	b.n	300048c0 <BOOT_LoadImages+0x40>
30004c86:	487e      	ldr	r0, [pc, #504]	; (30004e80 <BOOT_LoadImages+0x600>)
30004c88:	f006 ff4a 	bl	3000bb20 <__DiagPrintf_veneer>
30004c8c:	e6ba      	b.n	30004a04 <BOOT_LoadImages+0x184>
30004c8e:	487d      	ldr	r0, [pc, #500]	; (30004e84 <BOOT_LoadImages+0x604>)
30004c90:	f006 ff46 	bl	3000bb20 <__DiagPrintf_veneer>
30004c94:	f1b9 0f00 	cmp.w	r9, #0
30004c98:	d0a3      	beq.n	30004be2 <BOOT_LoadImages+0x362>
30004c9a:	e697      	b.n	300049cc <BOOT_LoadImages+0x14c>
30004c9c:	ea57 0300 	orrs.w	r3, r7, r0
30004ca0:	f000 833c 	beq.w	3000531c <BOOT_LoadImages+0xa9c>
30004ca4:	9909      	ldr	r1, [sp, #36]	; 0x24
30004ca6:	e9dd 2316 	ldrd	r2, r3, [sp, #88]	; 0x58
30004caa:	e9cd 5400 	strd	r5, r4, [sp]
30004cae:	429a      	cmp	r2, r3
30004cb0:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30004cb2:	465b      	mov	r3, fp
30004cb4:	f0c0 8416 	bcc.w	300054e4 <BOOT_LoadImages+0xc64>
30004cb8:	4620      	mov	r0, r4
30004cba:	f7ff f8c7 	bl	30003e4c <rewrite_bp>
30004cbe:	46a1      	mov	r9, r4
30004cc0:	b2c3      	uxtb	r3, r0
30004cc2:	2601      	movs	r6, #1
30004cc4:	9307      	str	r3, [sp, #28]
30004cc6:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30004cca:	9306      	str	r3, [sp, #24]
30004ccc:	4b68      	ldr	r3, [pc, #416]	; (30004e70 <BOOT_LoadImages+0x5f0>)
30004cce:	9308      	str	r3, [sp, #32]
30004cd0:	4b68      	ldr	r3, [pc, #416]	; (30004e74 <BOOT_LoadImages+0x5f4>)
30004cd2:	9309      	str	r3, [sp, #36]	; 0x24
30004cd4:	4b68      	ldr	r3, [pc, #416]	; (30004e78 <BOOT_LoadImages+0x5f8>)
30004cd6:	930a      	str	r3, [sp, #40]	; 0x28
30004cd8:	9b07      	ldr	r3, [sp, #28]
30004cda:	2b01      	cmp	r3, #1
30004cdc:	d164      	bne.n	30004da8 <BOOT_LoadImages+0x528>
30004cde:	4c6a      	ldr	r4, [pc, #424]	; (30004e88 <BOOT_LoadImages+0x608>)
30004ce0:	4618      	mov	r0, r3
30004ce2:	9a08      	ldr	r2, [sp, #32]
30004ce4:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004ce8:	3310      	adds	r3, #16
30004cea:	9311      	str	r3, [sp, #68]	; 0x44
30004cec:	6013      	str	r3, [r2, #0]
30004cee:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004cf0:	4798      	blx	r3
30004cf2:	1e03      	subs	r3, r0, #0
30004cf4:	bf18      	it	ne
30004cf6:	2301      	movne	r3, #1
30004cf8:	b108      	cbz	r0, 30004cfe <BOOT_LoadImages+0x47e>
30004cfa:	2e00      	cmp	r6, #0
30004cfc:	d16c      	bne.n	30004dd8 <BOOT_LoadImages+0x558>
30004cfe:	2200      	movs	r2, #0
30004d00:	9205      	str	r2, [sp, #20]
30004d02:	f019 0401 	ands.w	r4, r9, #1
30004d06:	f000 80ec 	beq.w	30004ee2 <BOOT_LoadImages+0x662>
30004d0a:	2b00      	cmp	r3, #0
30004d0c:	f000 80e9 	beq.w	30004ee2 <BOOT_LoadImages+0x662>
30004d10:	4c5e      	ldr	r4, [pc, #376]	; (30004e8c <BOOT_LoadImages+0x60c>)
30004d12:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004d16:	47a0      	blx	r4
30004d18:	e7fb      	b.n	30004d12 <BOOT_LoadImages+0x492>
30004d1a:	4293      	cmp	r3, r2
30004d1c:	f04f 0601 	mov.w	r6, #1
30004d20:	9a0f      	ldr	r2, [sp, #60]	; 0x3c
30004d22:	465b      	mov	r3, fp
30004d24:	990a      	ldr	r1, [sp, #40]	; 0x28
30004d26:	9500      	str	r5, [sp, #0]
30004d28:	9601      	str	r6, [sp, #4]
30004d2a:	f080 82ee 	bcs.w	3000530a <BOOT_LoadImages+0xa8a>
30004d2e:	4630      	mov	r0, r6
30004d30:	f7ff f88c 	bl	30003e4c <rewrite_bp>
30004d34:	b2c3      	uxtb	r3, r0
30004d36:	9307      	str	r3, [sp, #28]
30004d38:	e7c5      	b.n	30004cc6 <BOOT_LoadImages+0x446>
30004d3a:	4641      	mov	r1, r8
30004d3c:	4854      	ldr	r0, [pc, #336]	; (30004e90 <BOOT_LoadImages+0x610>)
30004d3e:	f006 feef 	bl	3000bb20 <__DiagPrintf_veneer>
30004d42:	f1b8 0f02 	cmp.w	r8, #2
30004d46:	f67f aeb2 	bls.w	30004aae <BOOT_LoadImages+0x22e>
30004d4a:	2303      	movs	r3, #3
30004d4c:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
30004d50:	2300      	movs	r3, #0
30004d52:	930d      	str	r3, [sp, #52]	; 0x34
30004d54:	e6e6      	b.n	30004b24 <BOOT_LoadImages+0x2a4>
30004d56:	4b46      	ldr	r3, [pc, #280]	; (30004e70 <BOOT_LoadImages+0x5f0>)
30004d58:	f04f 0900 	mov.w	r9, #0
30004d5c:	9308      	str	r3, [sp, #32]
30004d5e:	4b45      	ldr	r3, [pc, #276]	; (30004e74 <BOOT_LoadImages+0x5f4>)
30004d60:	9309      	str	r3, [sp, #36]	; 0x24
30004d62:	4b45      	ldr	r3, [pc, #276]	; (30004e78 <BOOT_LoadImages+0x5f8>)
30004d64:	930a      	str	r3, [sp, #40]	; 0x28
30004d66:	9b05      	ldr	r3, [sp, #20]
30004d68:	2b00      	cmp	r3, #0
30004d6a:	f040 8126 	bne.w	30004fba <BOOT_LoadImages+0x73a>
30004d6e:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004d72:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004d76:	3201      	adds	r2, #1
30004d78:	f000 82e6 	beq.w	30005348 <BOOT_LoadImages+0xac8>
30004d7c:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004d80:	f1b9 0f00 	cmp.w	r9, #0
30004d84:	d002      	beq.n	30004d8c <BOOT_LoadImages+0x50c>
30004d86:	2965      	cmp	r1, #101	; 0x65
30004d88:	f000 8365 	beq.w	30005456 <BOOT_LoadImages+0xbd6>
30004d8c:	4841      	ldr	r0, [pc, #260]	; (30004e94 <BOOT_LoadImages+0x614>)
30004d8e:	f006 fec7 	bl	3000bb20 <__DiagPrintf_veneer>
30004d92:	2301      	movs	r3, #1
30004d94:	930d      	str	r3, [sp, #52]	; 0x34
30004d96:	9307      	str	r3, [sp, #28]
30004d98:	2601      	movs	r6, #1
30004d9a:	f1b9 0f00 	cmp.w	r9, #0
30004d9e:	f040 81eb 	bne.w	30005178 <BOOT_LoadImages+0x8f8>
30004da2:	9b07      	ldr	r3, [sp, #28]
30004da4:	2b01      	cmp	r3, #1
30004da6:	d09a      	beq.n	30004cde <BOOT_LoadImages+0x45e>
30004da8:	9b07      	ldr	r3, [sp, #28]
30004daa:	2b02      	cmp	r3, #2
30004dac:	f000 81ac 	beq.w	30005108 <BOOT_LoadImages+0x888>
30004db0:	9b08      	ldr	r3, [sp, #32]
30004db2:	9807      	ldr	r0, [sp, #28]
30004db4:	461a      	mov	r2, r3
30004db6:	9b11      	ldr	r3, [sp, #68]	; 0x44
30004db8:	6013      	str	r3, [r2, #0]
30004dba:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004dbc:	4798      	blx	r3
30004dbe:	1e03      	subs	r3, r0, #0
30004dc0:	bf18      	it	ne
30004dc2:	2301      	movne	r3, #1
30004dc4:	2800      	cmp	r0, #0
30004dc6:	d09c      	beq.n	30004d02 <BOOT_LoadImages+0x482>
30004dc8:	2e00      	cmp	r6, #0
30004dca:	d09a      	beq.n	30004d02 <BOOT_LoadImages+0x482>
30004dcc:	9b05      	ldr	r3, [sp, #20]
30004dce:	4c2e      	ldr	r4, [pc, #184]	; (30004e88 <BOOT_LoadImages+0x608>)
30004dd0:	fab3 f383 	clz	r3, r3
30004dd4:	095b      	lsrs	r3, r3, #5
30004dd6:	9307      	str	r3, [sp, #28]
30004dd8:	482f      	ldr	r0, [pc, #188]	; (30004e98 <BOOT_LoadImages+0x618>)
30004dda:	6943      	ldr	r3, [r0, #20]
30004ddc:	03da      	lsls	r2, r3, #15
30004dde:	d51e      	bpl.n	30004e1e <BOOT_LoadImages+0x59e>
30004de0:	2300      	movs	r3, #0
30004de2:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004de6:	f3bf 8f4f 	dsb	sy
30004dea:	f8d0 7080 	ldr.w	r7, [r0, #128]	; 0x80
30004dee:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30004df2:	f3c7 364e 	ubfx	r6, r7, #13, #15
30004df6:	f3c7 07c9 	ubfx	r7, r7, #3, #10
30004dfa:	0176      	lsls	r6, r6, #5
30004dfc:	ea06 010c 	and.w	r1, r6, ip
30004e00:	463b      	mov	r3, r7
30004e02:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004e06:	3b01      	subs	r3, #1
30004e08:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004e0c:	d2f9      	bcs.n	30004e02 <BOOT_LoadImages+0x582>
30004e0e:	3e20      	subs	r6, #32
30004e10:	f116 0f20 	cmn.w	r6, #32
30004e14:	d1f2      	bne.n	30004dfc <BOOT_LoadImages+0x57c>
30004e16:	f3bf 8f4f 	dsb	sy
30004e1a:	f3bf 8f6f 	isb	sy
30004e1e:	2304      	movs	r3, #4
30004e20:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
30004e24:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30004e26:	2b00      	cmp	r3, #0
30004e28:	f000 840c 	beq.w	30005644 <BOOT_LoadImages+0xdc4>
30004e2c:	9a07      	ldr	r2, [sp, #28]
30004e2e:	4699      	mov	r9, r3
30004e30:	9205      	str	r2, [sp, #20]
30004e32:	e798      	b.n	30004d66 <BOOT_LoadImages+0x4e6>
30004e34:	4819      	ldr	r0, [pc, #100]	; (30004e9c <BOOT_LoadImages+0x61c>)
30004e36:	f006 fe73 	bl	3000bb20 <__DiagPrintf_veneer>
30004e3a:	9e0e      	ldr	r6, [sp, #56]	; 0x38
30004e3c:	9a06      	ldr	r2, [sp, #24]
30004e3e:	4296      	cmp	r6, r2
30004e40:	f4ff adff 	bcc.w	30004a42 <BOOT_LoadImages+0x1c2>
30004e44:	4816      	ldr	r0, [pc, #88]	; (30004ea0 <BOOT_LoadImages+0x620>)
30004e46:	f006 fe6b 	bl	3000bb20 <__DiagPrintf_veneer>
30004e4a:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004e4c:	9a08      	ldr	r2, [sp, #32]
30004e4e:	f8c4 3400 	str.w	r3, [r4, #1024]	; 0x400
30004e52:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004e54:	9606      	str	r6, [sp, #24]
30004e56:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004e5a:	462b      	mov	r3, r5
30004e5c:	9205      	str	r2, [sp, #20]
30004e5e:	465d      	mov	r5, fp
30004e60:	469b      	mov	fp, r3
30004e62:	e5f7      	b.n	30004a54 <BOOT_LoadImages+0x1d4>
30004e64:	3000bf84 	.word	0x3000bf84
30004e68:	3000c114 	.word	0x3000c114
30004e6c:	3000c128 	.word	0x3000c128
30004e70:	3000f3d8 	.word	0x3000f3d8
30004e74:	300068fd 	.word	0x300068fd
30004e78:	3000df3f 	.word	0x3000df3f
30004e7c:	3000bee8 	.word	0x3000bee8
30004e80:	3000c0c4 	.word	0x3000c0c4
30004e84:	3000c4b8 	.word	0x3000c4b8
30004e88:	3000e104 	.word	0x3000e104
30004e8c:	00009be5 	.word	0x00009be5
30004e90:	3000c340 	.word	0x3000c340
30004e94:	3000bea4 	.word	0x3000bea4
30004e98:	e000ed00 	.word	0xe000ed00
30004e9c:	3000c0e0 	.word	0x3000c0e0
30004ea0:	3000c0f4 	.word	0x3000c0f4
30004ea4:	9f03      	ldr	r7, [sp, #12]
30004ea6:	f108 0801 	add.w	r8, r8, #1
30004eaa:	45ba      	cmp	sl, r7
30004eac:	fa5f f888 	uxtb.w	r8, r8
30004eb0:	f47f ae1b 	bne.w	30004aea <BOOT_LoadImages+0x26a>
30004eb4:	4651      	mov	r1, sl
30004eb6:	4652      	mov	r2, sl
30004eb8:	489e      	ldr	r0, [pc, #632]	; (30005134 <BOOT_LoadImages+0x8b4>)
30004eba:	f006 fe31 	bl	3000bb20 <__DiagPrintf_veneer>
30004ebe:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004ec2:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004ec6:	f1b9 0f00 	cmp.w	r9, #0
30004eca:	d002      	beq.n	30004ed2 <BOOT_LoadImages+0x652>
30004ecc:	2965      	cmp	r1, #101	; 0x65
30004ece:	f000 8356 	beq.w	3000557e <BOOT_LoadImages+0xcfe>
30004ed2:	4899      	ldr	r0, [pc, #612]	; (30005138 <BOOT_LoadImages+0x8b8>)
30004ed4:	f006 fe24 	bl	3000bb20 <__DiagPrintf_veneer>
30004ed8:	2300      	movs	r3, #0
30004eda:	930d      	str	r3, [sp, #52]	; 0x34
30004edc:	2302      	movs	r3, #2
30004ede:	9307      	str	r3, [sp, #28]
30004ee0:	e75a      	b.n	30004d98 <BOOT_LoadImages+0x518>
30004ee2:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004ee4:	781b      	ldrb	r3, [r3, #0]
30004ee6:	2b00      	cmp	r3, #0
30004ee8:	f040 81c0 	bne.w	3000526c <BOOT_LoadImages+0x9ec>
30004eec:	2001      	movs	r0, #1
30004eee:	b03b      	add	sp, #236	; 0xec
30004ef0:	ecbd 8b02 	vpop	{d8}
30004ef4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004ef8:	f8dd 8014 	ldr.w	r8, [sp, #20]
30004efc:	4e8f      	ldr	r6, [pc, #572]	; (3000513c <BOOT_LoadImages+0x8bc>)
30004efe:	4647      	mov	r7, r8
30004f00:	f8dd 8040 	ldr.w	r8, [sp, #64]	; 0x40
30004f04:	9507      	str	r5, [sp, #28]
30004f06:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30004f0a:	9b04      	ldr	r3, [sp, #16]
30004f0c:	f10c 0004 	add.w	r0, ip, #4
30004f10:	b3b3      	cbz	r3, 30004f80 <BOOT_LoadImages+0x700>
30004f12:	f103 0e04 	add.w	lr, r3, #4
30004f16:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004f1a:	44e6      	add	lr, ip
30004f1c:	f810 3b01 	ldrb.w	r3, [r0], #1
30004f20:	4053      	eors	r3, r2
30004f22:	4570      	cmp	r0, lr
30004f24:	b2db      	uxtb	r3, r3
30004f26:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004f2a:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004f2e:	d1f5      	bne.n	30004f1c <BOOT_LoadImages+0x69c>
30004f30:	43d5      	mvns	r5, r2
30004f32:	1c7b      	adds	r3, r7, #1
30004f34:	45a8      	cmp	r8, r5
30004f36:	b2df      	uxtb	r7, r3
30004f38:	d027      	beq.n	30004f8a <BOOT_LoadImages+0x70a>
30004f3a:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004f3e:	3101      	adds	r1, #1
30004f40:	d011      	beq.n	30004f66 <BOOT_LoadImages+0x6e6>
30004f42:	463b      	mov	r3, r7
30004f44:	462a      	mov	r2, r5
30004f46:	4641      	mov	r1, r8
30004f48:	4630      	mov	r0, r6
30004f4a:	f006 fde9 	bl	3000bb20 <__DiagPrintf_veneer>
30004f4e:	2f02      	cmp	r7, #2
30004f50:	d9d9      	bls.n	30004f06 <BOOT_LoadImages+0x686>
30004f52:	462f      	mov	r7, r5
30004f54:	9910      	ldr	r1, [sp, #64]	; 0x40
30004f56:	487a      	ldr	r0, [pc, #488]	; (30005140 <BOOT_LoadImages+0x8c0>)
30004f58:	463a      	mov	r2, r7
30004f5a:	9d07      	ldr	r5, [sp, #28]
30004f5c:	f006 fde0 	bl	3000bb20 <__DiagPrintf_veneer>
30004f60:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004f64:	e5d6      	b.n	30004b14 <BOOT_LoadImages+0x294>
30004f66:	4639      	mov	r1, r7
30004f68:	4876      	ldr	r0, [pc, #472]	; (30005144 <BOOT_LoadImages+0x8c4>)
30004f6a:	f006 fdd9 	bl	3000bb20 <__DiagPrintf_veneer>
30004f6e:	2f02      	cmp	r7, #2
30004f70:	d9c9      	bls.n	30004f06 <BOOT_LoadImages+0x686>
30004f72:	2303      	movs	r3, #3
30004f74:	9d07      	ldr	r5, [sp, #28]
30004f76:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
30004f7a:	2300      	movs	r3, #0
30004f7c:	930d      	str	r3, [sp, #52]	; 0x34
30004f7e:	e5d1      	b.n	30004b24 <BOOT_LoadImages+0x2a4>
30004f80:	9d04      	ldr	r5, [sp, #16]
30004f82:	1c7b      	adds	r3, r7, #1
30004f84:	45a8      	cmp	r8, r5
30004f86:	b2df      	uxtb	r7, r3
30004f88:	d1d7      	bne.n	30004f3a <BOOT_LoadImages+0x6ba>
30004f8a:	9a10      	ldr	r2, [sp, #64]	; 0x40
30004f8c:	486e      	ldr	r0, [pc, #440]	; (30005148 <BOOT_LoadImages+0x8c8>)
30004f8e:	4611      	mov	r1, r2
30004f90:	9d07      	ldr	r5, [sp, #28]
30004f92:	f006 fdc5 	bl	3000bb20 <__DiagPrintf_veneer>
30004f96:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004f9a:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004f9e:	f1b9 0f00 	cmp.w	r9, #0
30004fa2:	d002      	beq.n	30004faa <BOOT_LoadImages+0x72a>
30004fa4:	2965      	cmp	r1, #101	; 0x65
30004fa6:	f000 82fc 	beq.w	300055a2 <BOOT_LoadImages+0xd22>
30004faa:	4868      	ldr	r0, [pc, #416]	; (3000514c <BOOT_LoadImages+0x8cc>)
30004fac:	f006 fdb8 	bl	3000bb20 <__DiagPrintf_veneer>
30004fb0:	2300      	movs	r3, #0
30004fb2:	930d      	str	r3, [sp, #52]	; 0x34
30004fb4:	2301      	movs	r3, #1
30004fb6:	9307      	str	r3, [sp, #28]
30004fb8:	e6ee      	b.n	30004d98 <BOOT_LoadImages+0x518>
30004fba:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004fbe:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004fc2:	3201      	adds	r2, #1
30004fc4:	f000 81ca 	beq.w	3000535c <BOOT_LoadImages+0xadc>
30004fc8:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004fcc:	f1b9 0f00 	cmp.w	r9, #0
30004fd0:	d002      	beq.n	30004fd8 <BOOT_LoadImages+0x758>
30004fd2:	2965      	cmp	r1, #101	; 0x65
30004fd4:	f000 8289 	beq.w	300054ea <BOOT_LoadImages+0xc6a>
30004fd8:	4857      	ldr	r0, [pc, #348]	; (30005138 <BOOT_LoadImages+0x8b8>)
30004fda:	f006 fda1 	bl	3000bb20 <__DiagPrintf_veneer>
30004fde:	2301      	movs	r3, #1
30004fe0:	930d      	str	r3, [sp, #52]	; 0x34
30004fe2:	2302      	movs	r3, #2
30004fe4:	9307      	str	r3, [sp, #28]
30004fe6:	e6d7      	b.n	30004d98 <BOOT_LoadImages+0x518>
30004fe8:	990e      	ldr	r1, [sp, #56]	; 0x38
30004fea:	4859      	ldr	r0, [pc, #356]	; (30005150 <BOOT_LoadImages+0x8d0>)
30004fec:	f006 fd98 	bl	3000bb20 <__DiagPrintf_veneer>
30004ff0:	f119 0f03 	cmn.w	r9, #3
30004ff4:	f240 82b7 	bls.w	30005566 <BOOT_LoadImages+0xce6>
30004ff8:	4856      	ldr	r0, [pc, #344]	; (30005154 <BOOT_LoadImages+0x8d4>)
30004ffa:	f006 fd91 	bl	3000bb20 <__DiagPrintf_veneer>
30004ffe:	f8cd 802c 	str.w	r8, [sp, #44]	; 0x2c
30005002:	f8db 300d 	ldr.w	r3, [fp, #13]
30005006:	930a      	str	r3, [sp, #40]	; 0x28
30005008:	f8db 3011 	ldr.w	r3, [fp, #17]
3000500c:	930f      	str	r3, [sp, #60]	; 0x3c
3000500e:	f8d5 300d 	ldr.w	r3, [r5, #13]
30005012:	9309      	str	r3, [sp, #36]	; 0x24
30005014:	f8d5 3011 	ldr.w	r3, [r5, #17]
30005018:	930c      	str	r3, [sp, #48]	; 0x30
3000501a:	f8df 9154 	ldr.w	r9, [pc, #340]	; 30005170 <BOOT_LoadImages+0x8f0>
3000501e:	2601      	movs	r6, #1
30005020:	4c4d      	ldr	r4, [pc, #308]	; (30005158 <BOOT_LoadImages+0x8d8>)
30005022:	44b9      	add	r9, r7
30005024:	f8df 814c 	ldr.w	r8, [pc, #332]	; 30005174 <BOOT_LoadImages+0x8f4>
30005028:	f8db 1000 	ldr.w	r1, [fp]
3000502c:	4648      	mov	r0, r9
3000502e:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30005032:	f810 3b01 	ldrb.w	r3, [r0], #1
30005036:	4053      	eors	r3, r2
30005038:	4285      	cmp	r5, r0
3000503a:	b2db      	uxtb	r3, r3
3000503c:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005040:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005044:	d1f5      	bne.n	30005032 <BOOT_LoadImages+0x7b2>
30005046:	43d2      	mvns	r2, r2
30005048:	4291      	cmp	r1, r2
3000504a:	f000 8102 	beq.w	30005252 <BOOT_LoadImages+0x9d2>
3000504e:	4633      	mov	r3, r6
30005050:	3601      	adds	r6, #1
30005052:	4640      	mov	r0, r8
30005054:	f006 fd64 	bl	3000bb20 <__DiagPrintf_veneer>
30005058:	2e04      	cmp	r6, #4
3000505a:	d1e5      	bne.n	30005028 <BOOT_LoadImages+0x7a8>
3000505c:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000505e:	2b00      	cmp	r3, #0
30005060:	f43f adbf 	beq.w	30004be2 <BOOT_LoadImages+0x362>
30005064:	2300      	movs	r3, #0
30005066:	930b      	str	r3, [sp, #44]	; 0x2c
30005068:	e475      	b.n	30004956 <BOOT_LoadImages+0xd6>
3000506a:	483c      	ldr	r0, [pc, #240]	; (3000515c <BOOT_LoadImages+0x8dc>)
3000506c:	f006 fd58 	bl	3000bb20 <__DiagPrintf_veneer>
30005070:	e426      	b.n	300048c0 <BOOT_LoadImages+0x40>
30005072:	483b      	ldr	r0, [pc, #236]	; (30005160 <BOOT_LoadImages+0x8e0>)
30005074:	e5a7      	b.n	30004bc6 <BOOT_LoadImages+0x346>
30005076:	483b      	ldr	r0, [pc, #236]	; (30005164 <BOOT_LoadImages+0x8e4>)
30005078:	f006 fd52 	bl	3000bb20 <__DiagPrintf_veneer>
3000507c:	e420      	b.n	300048c0 <BOOT_LoadImages+0x40>
3000507e:	9b05      	ldr	r3, [sp, #20]
30005080:	2b00      	cmp	r3, #0
30005082:	f040 8132 	bne.w	300052ea <BOOT_LoadImages+0xa6a>
30005086:	2301      	movs	r3, #1
30005088:	f88d 3074 	strb.w	r3, [sp, #116]	; 0x74
3000508c:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000508e:	2b00      	cmp	r3, #0
30005090:	f040 80c8 	bne.w	30005224 <BOOT_LoadImages+0x9a4>
30005094:	9b05      	ldr	r3, [sp, #20]
30005096:	9e0d      	ldr	r6, [sp, #52]	; 0x34
30005098:	2b01      	cmp	r3, #1
3000509a:	f000 819a 	beq.w	300053d2 <BOOT_LoadImages+0xb52>
3000509e:	46a8      	mov	r8, r5
300050a0:	3601      	adds	r6, #1
300050a2:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
300050a6:	9b03      	ldr	r3, [sp, #12]
300050a8:	b2f6      	uxtb	r6, r6
300050aa:	f10c 0104 	add.w	r1, ip, #4
300050ae:	2b00      	cmp	r3, #0
300050b0:	f000 80b6 	beq.w	30005220 <BOOT_LoadImages+0x9a0>
300050b4:	1d18      	adds	r0, r3, #4
300050b6:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
300050ba:	4460      	add	r0, ip
300050bc:	f811 2b01 	ldrb.w	r2, [r1], #1
300050c0:	405a      	eors	r2, r3
300050c2:	4281      	cmp	r1, r0
300050c4:	b2d2      	uxtb	r2, r2
300050c6:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
300050ca:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
300050ce:	d1f5      	bne.n	300050bc <BOOT_LoadImages+0x83c>
300050d0:	43dd      	mvns	r5, r3
300050d2:	45aa      	cmp	sl, r5
300050d4:	f000 815e 	beq.w	30005394 <BOOT_LoadImages+0xb14>
300050d8:	f8dc 300a 	ldr.w	r3, [ip, #10]
300050dc:	3301      	adds	r3, #1
300050de:	f000 8097 	beq.w	30005210 <BOOT_LoadImages+0x990>
300050e2:	4633      	mov	r3, r6
300050e4:	462a      	mov	r2, r5
300050e6:	4651      	mov	r1, sl
300050e8:	481f      	ldr	r0, [pc, #124]	; (30005168 <BOOT_LoadImages+0x8e8>)
300050ea:	f006 fd19 	bl	3000bb20 <__DiagPrintf_veneer>
300050ee:	2e02      	cmp	r6, #2
300050f0:	d9d6      	bls.n	300050a0 <BOOT_LoadImages+0x820>
300050f2:	4651      	mov	r1, sl
300050f4:	462a      	mov	r2, r5
300050f6:	481d      	ldr	r0, [pc, #116]	; (3000516c <BOOT_LoadImages+0x8ec>)
300050f8:	f006 fd12 	bl	3000bb20 <__DiagPrintf_veneer>
300050fc:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30005100:	2002      	movs	r0, #2
30005102:	f7fe fd65 	bl	30003bd0 <FLASH_Erase_With_Lock>
30005106:	e524      	b.n	30004b52 <BOOT_LoadImages+0x2d2>
30005108:	4c13      	ldr	r4, [pc, #76]	; (30005158 <BOOT_LoadImages+0x8d8>)
3000510a:	4618      	mov	r0, r3
3000510c:	9a08      	ldr	r2, [sp, #32]
3000510e:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005112:	3310      	adds	r3, #16
30005114:	9311      	str	r3, [sp, #68]	; 0x44
30005116:	6013      	str	r3, [r2, #0]
30005118:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000511a:	4798      	blx	r3
3000511c:	1e03      	subs	r3, r0, #0
3000511e:	bf18      	it	ne
30005120:	2301      	movne	r3, #1
30005122:	b118      	cbz	r0, 3000512c <BOOT_LoadImages+0x8ac>
30005124:	b116      	cbz	r6, 3000512c <BOOT_LoadImages+0x8ac>
30005126:	2300      	movs	r3, #0
30005128:	9307      	str	r3, [sp, #28]
3000512a:	e655      	b.n	30004dd8 <BOOT_LoadImages+0x558>
3000512c:	2201      	movs	r2, #1
3000512e:	9205      	str	r2, [sp, #20]
30005130:	e5e7      	b.n	30004d02 <BOOT_LoadImages+0x482>
30005132:	bf00      	nop
30005134:	3000c304 	.word	0x3000c304
30005138:	3000bebc 	.word	0x3000bebc
3000513c:	3000c254 	.word	0x3000c254
30005140:	3000c2a0 	.word	0x3000c2a0
30005144:	3000c238 	.word	0x3000c238
30005148:	3000c1fc 	.word	0x3000c1fc
3000514c:	3000bea4 	.word	0x3000bea4
30005150:	3000bf68 	.word	0x3000bf68
30005154:	3000c4d4 	.word	0x3000c4d4
30005158:	3000e104 	.word	0x3000e104
3000515c:	3000bef8 	.word	0x3000bef8
30005160:	3000c4ec 	.word	0x3000c4ec
30005164:	3000bf0c 	.word	0x3000bf0c
30005168:	3000c35c 	.word	0x3000c35c
3000516c:	3000c3a8 	.word	0x3000c3a8
30005170:	07ffe004 	.word	0x07ffe004
30005174:	3000bfd4 	.word	0x3000bfd4
30005178:	af1a      	add	r7, sp, #104	; 0x68
3000517a:	2280      	movs	r2, #128	; 0x80
3000517c:	21ff      	movs	r1, #255	; 0xff
3000517e:	4638      	mov	r0, r7
30005180:	f006 fc6e 	bl	3000ba60 <____wrap_memset_veneer>
30005184:	4629      	mov	r1, r5
30005186:	2280      	movs	r2, #128	; 0x80
30005188:	4638      	mov	r0, r7
3000518a:	f006 fcb1 	bl	3000baf0 <____wrap_memcpy_veneer>
3000518e:	9b05      	ldr	r3, [sp, #20]
30005190:	f1a3 0101 	sub.w	r1, r3, #1
30005194:	fab1 f181 	clz	r1, r1
30005198:	0949      	lsrs	r1, r1, #5
3000519a:	f88d 1074 	strb.w	r1, [sp, #116]	; 0x74
3000519e:	9b06      	ldr	r3, [sp, #24]
300051a0:	f10b 0604 	add.w	r6, fp, #4
300051a4:	48b5      	ldr	r0, [pc, #724]	; (3000547c <BOOT_LoadImages+0xbfc>)
300051a6:	3301      	adds	r3, #1
300051a8:	9306      	str	r3, [sp, #24]
300051aa:	f006 fcb9 	bl	3000bb20 <__DiagPrintf_veneer>
300051ae:	9b06      	ldr	r3, [sp, #24]
300051b0:	4659      	mov	r1, fp
300051b2:	2002      	movs	r0, #2
300051b4:	931b      	str	r3, [sp, #108]	; 0x6c
300051b6:	f7fe fd0b 	bl	30003bd0 <FLASH_Erase_With_Lock>
300051ba:	4630      	mov	r0, r6
300051bc:	aa1b      	add	r2, sp, #108	; 0x6c
300051be:	217c      	movs	r1, #124	; 0x7c
300051c0:	f7fe fd2e 	bl	30003c20 <FLASH_TxData_With_Lock>
300051c4:	f10d 024b 	add.w	r2, sp, #75	; 0x4b
300051c8:	2101      	movs	r1, #1
300051ca:	f60b 70ff 	addw	r0, fp, #4095	; 0xfff
300051ce:	f7fe fd27 	bl	30003c20 <FLASH_TxData_With_Lock>
300051d2:	4632      	mov	r2, r6
300051d4:	f50b 5080 	add.w	r0, fp, #4096	; 0x1000
300051d8:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
300051dc:	f812 3b01 	ldrb.w	r3, [r2], #1
300051e0:	404b      	eors	r3, r1
300051e2:	4282      	cmp	r2, r0
300051e4:	b2db      	uxtb	r3, r3
300051e6:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051ea:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
300051ee:	d1f5      	bne.n	300051dc <BOOT_LoadImages+0x95c>
300051f0:	43c9      	mvns	r1, r1
300051f2:	f089 0401 	eor.w	r4, r9, #1
300051f6:	463a      	mov	r2, r7
300051f8:	4658      	mov	r0, fp
300051fa:	911a      	str	r1, [sp, #104]	; 0x68
300051fc:	2104      	movs	r1, #4
300051fe:	f004 0601 	and.w	r6, r4, #1
30005202:	f7fe fd0d 	bl	30003c20 <FLASH_TxData_With_Lock>
30005206:	e5cc      	b.n	30004da2 <BOOT_LoadImages+0x522>
30005208:	489d      	ldr	r0, [pc, #628]	; (30005480 <BOOT_LoadImages+0xc00>)
3000520a:	f006 fc89 	bl	3000bb20 <__DiagPrintf_veneer>
3000520e:	e4e8      	b.n	30004be2 <BOOT_LoadImages+0x362>
30005210:	4631      	mov	r1, r6
30005212:	489c      	ldr	r0, [pc, #624]	; (30005484 <BOOT_LoadImages+0xc04>)
30005214:	f006 fc84 	bl	3000bb20 <__DiagPrintf_veneer>
30005218:	2e02      	cmp	r6, #2
3000521a:	f67f af41 	bls.w	300050a0 <BOOT_LoadImages+0x820>
3000521e:	e498      	b.n	30004b52 <BOOT_LoadImages+0x2d2>
30005220:	9d03      	ldr	r5, [sp, #12]
30005222:	e756      	b.n	300050d2 <BOOT_LoadImages+0x852>
30005224:	9b05      	ldr	r3, [sp, #20]
30005226:	2b01      	cmp	r3, #1
30005228:	f000 80a2 	beq.w	30005370 <BOOT_LoadImages+0xaf0>
3000522c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005230:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005234:	3201      	adds	r2, #1
30005236:	f000 819d 	beq.w	30005574 <BOOT_LoadImages+0xcf4>
3000523a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000523e:	2965      	cmp	r1, #101	; 0x65
30005240:	d070      	beq.n	30005324 <BOOT_LoadImages+0xaa4>
30005242:	4891      	ldr	r0, [pc, #580]	; (30005488 <BOOT_LoadImages+0xc08>)
30005244:	f006 fc6c 	bl	3000bb20 <__DiagPrintf_veneer>
30005248:	2302      	movs	r3, #2
3000524a:	9307      	str	r3, [sp, #28]
3000524c:	f89d 1074 	ldrb.w	r1, [sp, #116]	; 0x74
30005250:	e7a5      	b.n	3000519e <BOOT_LoadImages+0x91e>
30005252:	460a      	mov	r2, r1
30005254:	488d      	ldr	r0, [pc, #564]	; (3000548c <BOOT_LoadImages+0xc0c>)
30005256:	f006 fc63 	bl	3000bb20 <__DiagPrintf_veneer>
3000525a:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000525c:	2b00      	cmp	r3, #0
3000525e:	f47f ab7a 	bne.w	30004956 <BOOT_LoadImages+0xd6>
30005262:	4699      	mov	r9, r3
30005264:	f107 6600 	add.w	r6, r7, #134217728	; 0x8000000
30005268:	f7ff bb9b 	b.w	300049a2 <BOOT_LoadImages+0x122>
3000526c:	4b88      	ldr	r3, [pc, #544]	; (30005490 <BOOT_LoadImages+0xc10>)
3000526e:	9807      	ldr	r0, [sp, #28]
30005270:	4798      	blx	r3
30005272:	1e03      	subs	r3, r0, #0
30005274:	bf18      	it	ne
30005276:	2301      	movne	r3, #1
30005278:	2800      	cmp	r0, #0
3000527a:	d048      	beq.n	3000530e <BOOT_LoadImages+0xa8e>
3000527c:	2e00      	cmp	r6, #0
3000527e:	d046      	beq.n	3000530e <BOOT_LoadImages+0xa8e>
30005280:	9b05      	ldr	r3, [sp, #20]
30005282:	4984      	ldr	r1, [pc, #528]	; (30005494 <BOOT_LoadImages+0xc14>)
30005284:	fab3 f383 	clz	r3, r3
30005288:	095b      	lsrs	r3, r3, #5
3000528a:	9307      	str	r3, [sp, #28]
3000528c:	694b      	ldr	r3, [r1, #20]
3000528e:	03db      	lsls	r3, r3, #15
30005290:	d51e      	bpl.n	300052d0 <BOOT_LoadImages+0xa50>
30005292:	2300      	movs	r3, #0
30005294:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30005298:	f3bf 8f4f 	dsb	sy
3000529c:	f8d1 6080 	ldr.w	r6, [r1, #128]	; 0x80
300052a0:	f643 77e0 	movw	r7, #16352	; 0x3fe0
300052a4:	f3c6 344e 	ubfx	r4, r6, #13, #15
300052a8:	f3c6 06c9 	ubfx	r6, r6, #3, #10
300052ac:	0164      	lsls	r4, r4, #5
300052ae:	ea04 0007 	and.w	r0, r4, r7
300052b2:	4633      	mov	r3, r6
300052b4:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
300052b8:	3b01      	subs	r3, #1
300052ba:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300052be:	d2f9      	bcs.n	300052b4 <BOOT_LoadImages+0xa34>
300052c0:	3c20      	subs	r4, #32
300052c2:	f114 0f20 	cmn.w	r4, #32
300052c6:	d1f2      	bne.n	300052ae <BOOT_LoadImages+0xa2e>
300052c8:	f3bf 8f4f 	dsb	sy
300052cc:	f3bf 8f6f 	isb	sy
300052d0:	2304      	movs	r3, #4
300052d2:	4c71      	ldr	r4, [pc, #452]	; (30005498 <BOOT_LoadImages+0xc18>)
300052d4:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
300052d8:	e5a4      	b.n	30004e24 <BOOT_LoadImages+0x5a4>
300052da:	460a      	mov	r2, r1
300052dc:	486f      	ldr	r0, [pc, #444]	; (3000549c <BOOT_LoadImages+0xc1c>)
300052de:	f04f 0901 	mov.w	r9, #1
300052e2:	f006 fc1d 	bl	3000bb20 <__DiagPrintf_veneer>
300052e6:	f7ff bb5a 	b.w	3000499e <BOOT_LoadImages+0x11e>
300052ea:	f88d 9074 	strb.w	r9, [sp, #116]	; 0x74
300052ee:	e6cd      	b.n	3000508c <BOOT_LoadImages+0x80c>
300052f0:	9a09      	ldr	r2, [sp, #36]	; 0x24
300052f2:	42b2      	cmp	r2, r6
300052f4:	d802      	bhi.n	300052fc <BOOT_LoadImages+0xa7c>
300052f6:	429a      	cmp	r2, r3
300052f8:	f63f ab95 	bhi.w	30004a26 <BOOT_LoadImages+0x1a6>
300052fc:	4860      	ldr	r0, [pc, #384]	; (30005480 <BOOT_LoadImages+0xc00>)
300052fe:	f006 fc0f 	bl	3000bb20 <__DiagPrintf_veneer>
30005302:	9b08      	ldr	r3, [sp, #32]
30005304:	9305      	str	r3, [sp, #20]
30005306:	f7ff bb63 	b.w	300049d0 <BOOT_LoadImages+0x150>
3000530a:	4648      	mov	r0, r9
3000530c:	e510      	b.n	30004d30 <BOOT_LoadImages+0x4b0>
3000530e:	2c00      	cmp	r4, #0
30005310:	f43f adec 	beq.w	30004eec <BOOT_LoadImages+0x66c>
30005314:	2b00      	cmp	r3, #0
30005316:	f47f acfb 	bne.w	30004d10 <BOOT_LoadImages+0x490>
3000531a:	e5e7      	b.n	30004eec <BOOT_LoadImages+0x66c>
3000531c:	4860      	ldr	r0, [pc, #384]	; (300054a0 <BOOT_LoadImages+0xc20>)
3000531e:	f006 fbff 	bl	3000bb20 <__DiagPrintf_veneer>
30005322:	e7fe      	b.n	30005322 <BOOT_LoadImages+0xaa2>
30005324:	485f      	ldr	r0, [pc, #380]	; (300054a4 <BOOT_LoadImages+0xc24>)
30005326:	f006 fbfb 	bl	3000bb20 <__DiagPrintf_veneer>
3000532a:	4b5f      	ldr	r3, [pc, #380]	; (300054a8 <BOOT_LoadImages+0xc28>)
3000532c:	a818      	add	r0, sp, #96	; 0x60
3000532e:	4798      	blx	r3
30005330:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30005334:	485d      	ldr	r0, [pc, #372]	; (300054ac <BOOT_LoadImages+0xc2c>)
30005336:	a918      	add	r1, sp, #96	; 0x60
30005338:	4b5d      	ldr	r3, [pc, #372]	; (300054b0 <BOOT_LoadImages+0xc30>)
3000533a:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
3000533e:	4798      	blx	r3
30005340:	485a      	ldr	r0, [pc, #360]	; (300054ac <BOOT_LoadImages+0xc2c>)
30005342:	4b5c      	ldr	r3, [pc, #368]	; (300054b4 <BOOT_LoadImages+0xc34>)
30005344:	4798      	blx	r3
30005346:	e7fe      	b.n	30005346 <BOOT_LoadImages+0xac6>
30005348:	2303      	movs	r3, #3
3000534a:	485b      	ldr	r0, [pc, #364]	; (300054b8 <BOOT_LoadImages+0xc38>)
3000534c:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
30005350:	f006 fbe6 	bl	3000bb20 <__DiagPrintf_veneer>
30005354:	2301      	movs	r3, #1
30005356:	930d      	str	r3, [sp, #52]	; 0x34
30005358:	f7ff bbe4 	b.w	30004b24 <BOOT_LoadImages+0x2a4>
3000535c:	2303      	movs	r3, #3
3000535e:	4857      	ldr	r0, [pc, #348]	; (300054bc <BOOT_LoadImages+0xc3c>)
30005360:	f88d 304b 	strb.w	r3, [sp, #75]	; 0x4b
30005364:	f006 fbdc 	bl	3000bb20 <__DiagPrintf_veneer>
30005368:	2301      	movs	r3, #1
3000536a:	930d      	str	r3, [sp, #52]	; 0x34
3000536c:	f7ff bbda 	b.w	30004b24 <BOOT_LoadImages+0x2a4>
30005370:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005374:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005378:	3201      	adds	r2, #1
3000537a:	f000 8142 	beq.w	30005602 <BOOT_LoadImages+0xd82>
3000537e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005382:	2965      	cmp	r1, #101	; 0x65
30005384:	f000 811f 	beq.w	300055c6 <BOOT_LoadImages+0xd46>
30005388:	484d      	ldr	r0, [pc, #308]	; (300054c0 <BOOT_LoadImages+0xc40>)
3000538a:	f006 fbc9 	bl	3000bb20 <__DiagPrintf_veneer>
3000538e:	9b05      	ldr	r3, [sp, #20]
30005390:	9307      	str	r3, [sp, #28]
30005392:	e75b      	b.n	3000524c <BOOT_LoadImages+0x9cc>
30005394:	4651      	mov	r1, sl
30005396:	4652      	mov	r2, sl
30005398:	484a      	ldr	r0, [pc, #296]	; (300054c4 <BOOT_LoadImages+0xc44>)
3000539a:	4645      	mov	r5, r8
3000539c:	f006 fbc0 	bl	3000bb20 <__DiagPrintf_veneer>
300053a0:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300053a4:	f8d3 1006 	ldr.w	r1, [r3, #6]
300053a8:	2965      	cmp	r1, #101	; 0x65
300053aa:	f47f af4a 	bne.w	30005242 <BOOT_LoadImages+0x9c2>
300053ae:	483d      	ldr	r0, [pc, #244]	; (300054a4 <BOOT_LoadImages+0xc24>)
300053b0:	f006 fbb6 	bl	3000bb20 <__DiagPrintf_veneer>
300053b4:	4b3c      	ldr	r3, [pc, #240]	; (300054a8 <BOOT_LoadImages+0xc28>)
300053b6:	a818      	add	r0, sp, #96	; 0x60
300053b8:	4798      	blx	r3
300053ba:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
300053be:	483b      	ldr	r0, [pc, #236]	; (300054ac <BOOT_LoadImages+0xc2c>)
300053c0:	a918      	add	r1, sp, #96	; 0x60
300053c2:	4b3b      	ldr	r3, [pc, #236]	; (300054b0 <BOOT_LoadImages+0xc30>)
300053c4:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
300053c8:	4798      	blx	r3
300053ca:	4838      	ldr	r0, [pc, #224]	; (300054ac <BOOT_LoadImages+0xc2c>)
300053cc:	4b39      	ldr	r3, [pc, #228]	; (300054b4 <BOOT_LoadImages+0xc34>)
300053ce:	4798      	blx	r3
300053d0:	e7fe      	b.n	300053d0 <BOOT_LoadImages+0xb50>
300053d2:	ee08 7a10 	vmov	s16, r7
300053d6:	f8dd 8040 	ldr.w	r8, [sp, #64]	; 0x40
300053da:	4637      	mov	r7, r6
300053dc:	462e      	mov	r6, r5
300053de:	1c7b      	adds	r3, r7, #1
300053e0:	f8d4 0400 	ldr.w	r0, [r4, #1024]	; 0x400
300053e4:	b2df      	uxtb	r7, r3
300053e6:	9b04      	ldr	r3, [sp, #16]
300053e8:	1d01      	adds	r1, r0, #4
300053ea:	b393      	cbz	r3, 30005452 <BOOT_LoadImages+0xbd2>
300053ec:	f103 0c04 	add.w	ip, r3, #4
300053f0:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
300053f4:	4484      	add	ip, r0
300053f6:	f811 2b01 	ldrb.w	r2, [r1], #1
300053fa:	405a      	eors	r2, r3
300053fc:	4561      	cmp	r1, ip
300053fe:	b2d2      	uxtb	r2, r2
30005400:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005404:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30005408:	d1f5      	bne.n	300053f6 <BOOT_LoadImages+0xb76>
3000540a:	43dd      	mvns	r5, r3
3000540c:	45a8      	cmp	r8, r5
3000540e:	f000 8096 	beq.w	3000553e <BOOT_LoadImages+0xcbe>
30005412:	f8d0 300a 	ldr.w	r3, [r0, #10]
30005416:	3301      	adds	r3, #1
30005418:	d013      	beq.n	30005442 <BOOT_LoadImages+0xbc2>
3000541a:	463b      	mov	r3, r7
3000541c:	462a      	mov	r2, r5
3000541e:	4641      	mov	r1, r8
30005420:	4829      	ldr	r0, [pc, #164]	; (300054c8 <BOOT_LoadImages+0xc48>)
30005422:	f006 fb7d 	bl	3000bb20 <__DiagPrintf_veneer>
30005426:	2f02      	cmp	r7, #2
30005428:	d9d9      	bls.n	300053de <BOOT_LoadImages+0xb5e>
3000542a:	9910      	ldr	r1, [sp, #64]	; 0x40
3000542c:	462a      	mov	r2, r5
3000542e:	4827      	ldr	r0, [pc, #156]	; (300054cc <BOOT_LoadImages+0xc4c>)
30005430:	f006 fb76 	bl	3000bb20 <__DiagPrintf_veneer>
30005434:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005438:	2002      	movs	r0, #2
3000543a:	f7fe fbc9 	bl	30003bd0 <FLASH_Erase_With_Lock>
3000543e:	f7ff bb88 	b.w	30004b52 <BOOT_LoadImages+0x2d2>
30005442:	4639      	mov	r1, r7
30005444:	4822      	ldr	r0, [pc, #136]	; (300054d0 <BOOT_LoadImages+0xc50>)
30005446:	f006 fb6b 	bl	3000bb20 <__DiagPrintf_veneer>
3000544a:	2f02      	cmp	r7, #2
3000544c:	d9c7      	bls.n	300053de <BOOT_LoadImages+0xb5e>
3000544e:	f7ff bb80 	b.w	30004b52 <BOOT_LoadImages+0x2d2>
30005452:	9d04      	ldr	r5, [sp, #16]
30005454:	e7da      	b.n	3000540c <BOOT_LoadImages+0xb8c>
30005456:	481f      	ldr	r0, [pc, #124]	; (300054d4 <BOOT_LoadImages+0xc54>)
30005458:	f006 fb62 	bl	3000bb20 <__DiagPrintf_veneer>
3000545c:	4b12      	ldr	r3, [pc, #72]	; (300054a8 <BOOT_LoadImages+0xc28>)
3000545e:	a818      	add	r0, sp, #96	; 0x60
30005460:	4798      	blx	r3
30005462:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30005466:	4811      	ldr	r0, [pc, #68]	; (300054ac <BOOT_LoadImages+0xc2c>)
30005468:	a918      	add	r1, sp, #96	; 0x60
3000546a:	4b11      	ldr	r3, [pc, #68]	; (300054b0 <BOOT_LoadImages+0xc30>)
3000546c:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
30005470:	4798      	blx	r3
30005472:	480e      	ldr	r0, [pc, #56]	; (300054ac <BOOT_LoadImages+0xc2c>)
30005474:	4b0f      	ldr	r3, [pc, #60]	; (300054b4 <BOOT_LoadImages+0xc34>)
30005476:	4798      	blx	r3
30005478:	e7fe      	b.n	30005478 <BOOT_LoadImages+0xbf8>
3000547a:	bf00      	nop
3000547c:	3000c454 	.word	0x3000c454
30005480:	3000c508 	.word	0x3000c508
30005484:	3000c420 	.word	0x3000c420
30005488:	3000bebc 	.word	0x3000bebc
3000548c:	3000bf98 	.word	0x3000bf98
30005490:	30006901 	.word	0x30006901
30005494:	e000ed00 	.word	0xe000ed00
30005498:	3000e104 	.word	0x3000e104
3000549c:	3000c020 	.word	0x3000c020
300054a0:	3000c48c 	.word	0x3000c48c
300054a4:	3000c1b4 	.word	0x3000c1b4
300054a8:	0000d3f9 	.word	0x0000d3f9
300054ac:	41000440 	.word	0x41000440
300054b0:	0000d409 	.word	0x0000d409
300054b4:	0000d489 	.word	0x0000d489
300054b8:	3000c14c 	.word	0x3000c14c
300054bc:	3000c1a4 	.word	0x3000c1a4
300054c0:	3000bea4 	.word	0x3000bea4
300054c4:	3000c304 	.word	0x3000c304
300054c8:	3000c254 	.word	0x3000c254
300054cc:	3000c2a0 	.word	0x3000c2a0
300054d0:	3000c40c 	.word	0x3000c40c
300054d4:	3000c15c 	.word	0x3000c15c
300054d8:	2400      	movs	r4, #0
300054da:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300054dc:	9909      	ldr	r1, [sp, #36]	; 0x24
300054de:	465b      	mov	r3, fp
300054e0:	9500      	str	r5, [sp, #0]
300054e2:	9401      	str	r4, [sp, #4]
300054e4:	2001      	movs	r0, #1
300054e6:	f7ff bbe8 	b.w	30004cba <BOOT_LoadImages+0x43a>
300054ea:	4863      	ldr	r0, [pc, #396]	; (30005678 <BOOT_LoadImages+0xdf8>)
300054ec:	f006 fb18 	bl	3000bb20 <__DiagPrintf_veneer>
300054f0:	4b62      	ldr	r3, [pc, #392]	; (3000567c <BOOT_LoadImages+0xdfc>)
300054f2:	a818      	add	r0, sp, #96	; 0x60
300054f4:	4798      	blx	r3
300054f6:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
300054fa:	4861      	ldr	r0, [pc, #388]	; (30005680 <BOOT_LoadImages+0xe00>)
300054fc:	a918      	add	r1, sp, #96	; 0x60
300054fe:	4b61      	ldr	r3, [pc, #388]	; (30005684 <BOOT_LoadImages+0xe04>)
30005500:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
30005504:	4798      	blx	r3
30005506:	485e      	ldr	r0, [pc, #376]	; (30005680 <BOOT_LoadImages+0xe00>)
30005508:	4b5f      	ldr	r3, [pc, #380]	; (30005688 <BOOT_LoadImages+0xe08>)
3000550a:	4798      	blx	r3
3000550c:	e7fe      	b.n	3000550c <BOOT_LoadImages+0xc8c>
3000550e:	428a      	cmp	r2, r1
30005510:	f4ff ab9c 	bcc.w	30004c4c <BOOT_LoadImages+0x3cc>
30005514:	4282      	cmp	r2, r0
30005516:	f4ff ab99 	bcc.w	30004c4c <BOOT_LoadImages+0x3cc>
3000551a:	2601      	movs	r6, #1
3000551c:	9a0f      	ldr	r2, [sp, #60]	; 0x3c
3000551e:	990a      	ldr	r1, [sp, #40]	; 0x28
30005520:	465b      	mov	r3, fp
30005522:	4630      	mov	r0, r6
30005524:	9500      	str	r5, [sp, #0]
30005526:	9601      	str	r6, [sp, #4]
30005528:	f7fe fc90 	bl	30003e4c <rewrite_bp>
3000552c:	f04f 0900 	mov.w	r9, #0
30005530:	b2c3      	uxtb	r3, r0
30005532:	9307      	str	r3, [sp, #28]
30005534:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30005538:	9306      	str	r3, [sp, #24]
3000553a:	f7ff bbc7 	b.w	30004ccc <BOOT_LoadImages+0x44c>
3000553e:	9a10      	ldr	r2, [sp, #64]	; 0x40
30005540:	4635      	mov	r5, r6
30005542:	4852      	ldr	r0, [pc, #328]	; (3000568c <BOOT_LoadImages+0xe0c>)
30005544:	ee18 7a10 	vmov	r7, s16
30005548:	4611      	mov	r1, r2
3000554a:	f006 fae9 	bl	3000bb20 <__DiagPrintf_veneer>
3000554e:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005552:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005556:	2965      	cmp	r1, #101	; 0x65
30005558:	d05a      	beq.n	30005610 <BOOT_LoadImages+0xd90>
3000555a:	484d      	ldr	r0, [pc, #308]	; (30005690 <BOOT_LoadImages+0xe10>)
3000555c:	f006 fae0 	bl	3000bb20 <__DiagPrintf_veneer>
30005560:	2301      	movs	r3, #1
30005562:	9307      	str	r3, [sp, #28]
30005564:	e672      	b.n	3000524c <BOOT_LoadImages+0x9cc>
30005566:	3c01      	subs	r4, #1
30005568:	3403      	adds	r4, #3
3000556a:	d84f      	bhi.n	3000560c <BOOT_LoadImages+0xd8c>
3000556c:	2e01      	cmp	r6, #1
3000556e:	d96f      	bls.n	30005650 <BOOT_LoadImages+0xdd0>
30005570:	4848      	ldr	r0, [pc, #288]	; (30005694 <BOOT_LoadImages+0xe14>)
30005572:	e542      	b.n	30004ffa <BOOT_LoadImages+0x77a>
30005574:	4848      	ldr	r0, [pc, #288]	; (30005698 <BOOT_LoadImages+0xe18>)
30005576:	f006 fad3 	bl	3000bb20 <__DiagPrintf_veneer>
3000557a:	f7ff baea 	b.w	30004b52 <BOOT_LoadImages+0x2d2>
3000557e:	483e      	ldr	r0, [pc, #248]	; (30005678 <BOOT_LoadImages+0xdf8>)
30005580:	f006 face 	bl	3000bb20 <__DiagPrintf_veneer>
30005584:	4b3d      	ldr	r3, [pc, #244]	; (3000567c <BOOT_LoadImages+0xdfc>)
30005586:	a818      	add	r0, sp, #96	; 0x60
30005588:	4798      	blx	r3
3000558a:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
3000558e:	483c      	ldr	r0, [pc, #240]	; (30005680 <BOOT_LoadImages+0xe00>)
30005590:	a918      	add	r1, sp, #96	; 0x60
30005592:	4b3c      	ldr	r3, [pc, #240]	; (30005684 <BOOT_LoadImages+0xe04>)
30005594:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
30005598:	4798      	blx	r3
3000559a:	4839      	ldr	r0, [pc, #228]	; (30005680 <BOOT_LoadImages+0xe00>)
3000559c:	4b3a      	ldr	r3, [pc, #232]	; (30005688 <BOOT_LoadImages+0xe08>)
3000559e:	4798      	blx	r3
300055a0:	e7fe      	b.n	300055a0 <BOOT_LoadImages+0xd20>
300055a2:	483e      	ldr	r0, [pc, #248]	; (3000569c <BOOT_LoadImages+0xe1c>)
300055a4:	f006 fabc 	bl	3000bb20 <__DiagPrintf_veneer>
300055a8:	4b34      	ldr	r3, [pc, #208]	; (3000567c <BOOT_LoadImages+0xdfc>)
300055aa:	a818      	add	r0, sp, #96	; 0x60
300055ac:	4798      	blx	r3
300055ae:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
300055b2:	4833      	ldr	r0, [pc, #204]	; (30005680 <BOOT_LoadImages+0xe00>)
300055b4:	a918      	add	r1, sp, #96	; 0x60
300055b6:	4b33      	ldr	r3, [pc, #204]	; (30005684 <BOOT_LoadImages+0xe04>)
300055b8:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
300055bc:	4798      	blx	r3
300055be:	4830      	ldr	r0, [pc, #192]	; (30005680 <BOOT_LoadImages+0xe00>)
300055c0:	4b31      	ldr	r3, [pc, #196]	; (30005688 <BOOT_LoadImages+0xe08>)
300055c2:	4798      	blx	r3
300055c4:	e7fe      	b.n	300055c4 <BOOT_LoadImages+0xd44>
300055c6:	4835      	ldr	r0, [pc, #212]	; (3000569c <BOOT_LoadImages+0xe1c>)
300055c8:	f006 faaa 	bl	3000bb20 <__DiagPrintf_veneer>
300055cc:	4b2b      	ldr	r3, [pc, #172]	; (3000567c <BOOT_LoadImages+0xdfc>)
300055ce:	a818      	add	r0, sp, #96	; 0x60
300055d0:	4798      	blx	r3
300055d2:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
300055d6:	482a      	ldr	r0, [pc, #168]	; (30005680 <BOOT_LoadImages+0xe00>)
300055d8:	a918      	add	r1, sp, #96	; 0x60
300055da:	4b2a      	ldr	r3, [pc, #168]	; (30005684 <BOOT_LoadImages+0xe04>)
300055dc:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
300055e0:	4798      	blx	r3
300055e2:	4827      	ldr	r0, [pc, #156]	; (30005680 <BOOT_LoadImages+0xe00>)
300055e4:	4b28      	ldr	r3, [pc, #160]	; (30005688 <BOOT_LoadImages+0xe08>)
300055e6:	4798      	blx	r3
300055e8:	e7fe      	b.n	300055e8 <BOOT_LoadImages+0xd68>
300055ea:	4281      	cmp	r1, r0
300055ec:	f4ff ab34 	bcc.w	30004c58 <BOOT_LoadImages+0x3d8>
300055f0:	2400      	movs	r4, #0
300055f2:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300055f4:	9909      	ldr	r1, [sp, #36]	; 0x24
300055f6:	465b      	mov	r3, fp
300055f8:	4620      	mov	r0, r4
300055fa:	9500      	str	r5, [sp, #0]
300055fc:	9401      	str	r4, [sp, #4]
300055fe:	f7ff bb5c 	b.w	30004cba <BOOT_LoadImages+0x43a>
30005602:	4827      	ldr	r0, [pc, #156]	; (300056a0 <BOOT_LoadImages+0xe20>)
30005604:	f006 fa8c 	bl	3000bb20 <__DiagPrintf_veneer>
30005608:	f7ff baa3 	b.w	30004b52 <BOOT_LoadImages+0x2d2>
3000560c:	4825      	ldr	r0, [pc, #148]	; (300056a4 <BOOT_LoadImages+0xe24>)
3000560e:	e4f4      	b.n	30004ffa <BOOT_LoadImages+0x77a>
30005610:	4822      	ldr	r0, [pc, #136]	; (3000569c <BOOT_LoadImages+0xe1c>)
30005612:	f006 fa85 	bl	3000bb20 <__DiagPrintf_veneer>
30005616:	4b19      	ldr	r3, [pc, #100]	; (3000567c <BOOT_LoadImages+0xdfc>)
30005618:	a818      	add	r0, sp, #96	; 0x60
3000561a:	4798      	blx	r3
3000561c:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30005620:	4817      	ldr	r0, [pc, #92]	; (30005680 <BOOT_LoadImages+0xe00>)
30005622:	a918      	add	r1, sp, #96	; 0x60
30005624:	4b17      	ldr	r3, [pc, #92]	; (30005684 <BOOT_LoadImages+0xe04>)
30005626:	f8ad 2062 	strh.w	r2, [sp, #98]	; 0x62
3000562a:	4798      	blx	r3
3000562c:	4814      	ldr	r0, [pc, #80]	; (30005680 <BOOT_LoadImages+0xe00>)
3000562e:	4b16      	ldr	r3, [pc, #88]	; (30005688 <BOOT_LoadImages+0xe08>)
30005630:	4798      	blx	r3
30005632:	e7fe      	b.n	30005632 <BOOT_LoadImages+0xdb2>
30005634:	2601      	movs	r6, #1
30005636:	9a0f      	ldr	r2, [sp, #60]	; 0x3c
30005638:	990a      	ldr	r1, [sp, #40]	; 0x28
3000563a:	465b      	mov	r3, fp
3000563c:	2000      	movs	r0, #0
3000563e:	9500      	str	r5, [sp, #0]
30005640:	9601      	str	r6, [sp, #4]
30005642:	e771      	b.n	30005528 <BOOT_LoadImages+0xca8>
30005644:	9b07      	ldr	r3, [sp, #28]
30005646:	f04f 0901 	mov.w	r9, #1
3000564a:	9305      	str	r3, [sp, #20]
3000564c:	f7ff ba28 	b.w	30004aa0 <BOOT_LoadImages+0x220>
30005650:	9906      	ldr	r1, [sp, #24]
30005652:	4815      	ldr	r0, [pc, #84]	; (300056a8 <BOOT_LoadImages+0xe28>)
30005654:	f006 fa64 	bl	3000bb20 <__DiagPrintf_veneer>
30005658:	f8db 300d 	ldr.w	r3, [fp, #13]
3000565c:	930a      	str	r3, [sp, #40]	; 0x28
3000565e:	f8db 3011 	ldr.w	r3, [fp, #17]
30005662:	930f      	str	r3, [sp, #60]	; 0x3c
30005664:	f8d5 300d 	ldr.w	r3, [r5, #13]
30005668:	9309      	str	r3, [sp, #36]	; 0x24
3000566a:	f8d5 3011 	ldr.w	r3, [r5, #17]
3000566e:	930c      	str	r3, [sp, #48]	; 0x30
30005670:	2301      	movs	r3, #1
30005672:	930b      	str	r3, [sp, #44]	; 0x2c
30005674:	e4d1      	b.n	3000501a <BOOT_LoadImages+0x79a>
30005676:	bf00      	nop
30005678:	3000c1b4 	.word	0x3000c1b4
3000567c:	0000d3f9 	.word	0x0000d3f9
30005680:	41000440 	.word	0x41000440
30005684:	0000d409 	.word	0x0000d409
30005688:	0000d489 	.word	0x0000d489
3000568c:	3000c1fc 	.word	0x3000c1fc
30005690:	3000bea4 	.word	0x3000bea4
30005694:	3000c4ec 	.word	0x3000c4ec
30005698:	3000c1a4 	.word	0x3000c1a4
3000569c:	3000c15c 	.word	0x3000c15c
300056a0:	3000c14c 	.word	0x3000c14c
300056a4:	3000bf84 	.word	0x3000bf84
300056a8:	3000c524 	.word	0x3000c524

300056ac <BOOT_ReasonSet>:
300056ac:	4a03      	ldr	r2, [pc, #12]	; (300056bc <BOOT_ReasonSet+0x10>)
300056ae:	4904      	ldr	r1, [pc, #16]	; (300056c0 <BOOT_ReasonSet+0x14>)
300056b0:	8c13      	ldrh	r3, [r2, #32]
300056b2:	b29b      	uxth	r3, r3
300056b4:	8413      	strh	r3, [r2, #32]
300056b6:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
300056ba:	4770      	bx	lr
300056bc:	42008000 	.word	0x42008000
300056c0:	42008200 	.word	0x42008200

300056c4 <BOOT_Enable_KM0>:
300056c4:	b510      	push	{r4, lr}
300056c6:	4c12      	ldr	r4, [pc, #72]	; (30005710 <BOOT_Enable_KM0+0x4c>)
300056c8:	47a0      	blx	r4
300056ca:	b110      	cbz	r0, 300056d2 <BOOT_Enable_KM0+0xe>
300056cc:	4b11      	ldr	r3, [pc, #68]	; (30005714 <BOOT_Enable_KM0+0x50>)
300056ce:	4798      	blx	r3
300056d0:	b1a8      	cbz	r0, 300056fe <BOOT_Enable_KM0+0x3a>
300056d2:	47a0      	blx	r4
300056d4:	b940      	cbnz	r0, 300056e8 <BOOT_Enable_KM0+0x24>
300056d6:	4a10      	ldr	r2, [pc, #64]	; (30005718 <BOOT_Enable_KM0+0x54>)
300056d8:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300056dc:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300056e0:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300056e4:	bf40      	sev
300056e6:	bd10      	pop	{r4, pc}
300056e8:	4b0a      	ldr	r3, [pc, #40]	; (30005714 <BOOT_Enable_KM0+0x50>)
300056ea:	4798      	blx	r3
300056ec:	2800      	cmp	r0, #0
300056ee:	d0f2      	beq.n	300056d6 <BOOT_Enable_KM0+0x12>
300056f0:	4b0a      	ldr	r3, [pc, #40]	; (3000571c <BOOT_Enable_KM0+0x58>)
300056f2:	2004      	movs	r0, #4
300056f4:	4a0a      	ldr	r2, [pc, #40]	; (30005720 <BOOT_Enable_KM0+0x5c>)
300056f6:	4619      	mov	r1, r3
300056f8:	f005 ff2c 	bl	3000b554 <rtk_log_write>
300056fc:	e7eb      	b.n	300056d6 <BOOT_Enable_KM0+0x12>
300056fe:	4b07      	ldr	r3, [pc, #28]	; (3000571c <BOOT_Enable_KM0+0x58>)
30005700:	2004      	movs	r0, #4
30005702:	4a08      	ldr	r2, [pc, #32]	; (30005724 <BOOT_Enable_KM0+0x60>)
30005704:	4619      	mov	r1, r3
30005706:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000570a:	f005 bf23 	b.w	3000b554 <rtk_log_write>
3000570e:	bf00      	nop
30005710:	0000c09d 	.word	0x0000c09d
30005714:	0000c0ad 	.word	0x0000c0ad
30005718:	42008000 	.word	0x42008000
3000571c:	3000bcf8 	.word	0x3000bcf8
30005720:	3000c568 	.word	0x3000c568
30005724:	3000c540 	.word	0x3000c540

30005728 <BOOT_AP_Clk_Get>:
30005728:	4a06      	ldr	r2, [pc, #24]	; (30005744 <BOOT_AP_Clk_Get+0x1c>)
3000572a:	6913      	ldr	r3, [r2, #16]
3000572c:	2b00      	cmp	r3, #0
3000572e:	db03      	blt.n	30005738 <BOOT_AP_Clk_Get+0x10>
30005730:	6810      	ldr	r0, [r2, #0]
30005732:	fbb0 f0f3 	udiv	r0, r0, r3
30005736:	4770      	bx	lr
30005738:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000573c:	6850      	ldr	r0, [r2, #4]
3000573e:	fbb0 f0f3 	udiv	r0, r0, r3
30005742:	4770      	bx	lr
30005744:	3000e614 	.word	0x3000e614

30005748 <BOOT_SOC_ClkSet>:
30005748:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000574c:	4c5d      	ldr	r4, [pc, #372]	; (300058c4 <BOOT_SOC_ClkSet+0x17c>)
3000574e:	b083      	sub	sp, #12
30005750:	6926      	ldr	r6, [r4, #16]
30005752:	6825      	ldr	r5, [r4, #0]
30005754:	2e00      	cmp	r6, #0
30005756:	db7a      	blt.n	3000584e <BOOT_SOC_ClkSet+0x106>
30005758:	fbb5 f6f6 	udiv	r6, r5, r6
3000575c:	4b5a      	ldr	r3, [pc, #360]	; (300058c8 <BOOT_SOC_ClkSet+0x180>)
3000575e:	68a7      	ldr	r7, [r4, #8]
30005760:	fba3 1305 	umull	r1, r3, r3, r5
30005764:	68e2      	ldr	r2, [r4, #12]
30005766:	2f01      	cmp	r7, #1
30005768:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
3000576c:	ea4f 1b53 	mov.w	fp, r3, lsr #5
30005770:	ea4f 1393 	mov.w	r3, r3, lsr #6
30005774:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30005778:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
3000577c:	f103 39ff 	add.w	r9, r3, #4294967295	; 0xffffffff
30005780:	fbb5 f5f2 	udiv	r5, r5, r2
30005784:	d06f      	beq.n	30005866 <BOOT_SOC_ClkSet+0x11e>
30005786:	4b51      	ldr	r3, [pc, #324]	; (300058cc <BOOT_SOC_ClkSet+0x184>)
30005788:	2200      	movs	r2, #0
3000578a:	2dfa      	cmp	r5, #250	; 0xfa
3000578c:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30005790:	d863      	bhi.n	3000585a <BOOT_SOC_ClkSet+0x112>
30005792:	4f4f      	ldr	r7, [pc, #316]	; (300058d0 <BOOT_SOC_ClkSet+0x188>)
30005794:	783b      	ldrb	r3, [r7, #0]
30005796:	2b01      	cmp	r3, #1
30005798:	d103      	bne.n	300057a2 <BOOT_SOC_ClkSet+0x5a>
3000579a:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
3000579e:	f200 8085 	bhi.w	300058ac <BOOT_SOC_ClkSet+0x164>
300057a2:	2001      	movs	r0, #1
300057a4:	f8df 8154 	ldr.w	r8, [pc, #340]	; 300058fc <BOOT_SOC_ClkSet+0x1b4>
300057a8:	f001 fecc 	bl	30007544 <CLK_SWITCH_XTAL>
300057ac:	6820      	ldr	r0, [r4, #0]
300057ae:	4b49      	ldr	r3, [pc, #292]	; (300058d4 <BOOT_SOC_ClkSet+0x18c>)
300057b0:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
300057b4:	fb08 f000 	mul.w	r0, r8, r0
300057b8:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
300057bc:	4798      	blx	r3
300057be:	4a46      	ldr	r2, [pc, #280]	; (300058d8 <BOOT_SOC_ClkSet+0x190>)
300057c0:	68e1      	ldr	r1, [r4, #12]
300057c2:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
300057c6:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
300057ca:	ea4f 5909 	mov.w	r9, r9, lsl #20
300057ce:	1e4b      	subs	r3, r1, #1
300057d0:	4942      	ldr	r1, [pc, #264]	; (300058dc <BOOT_SOC_ClkSet+0x194>)
300057d2:	fa1f fb8b 	uxth.w	fp, fp
300057d6:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
300057da:	4001      	ands	r1, r0
300057dc:	f003 0307 	and.w	r3, r3, #7
300057e0:	2000      	movs	r0, #0
300057e2:	430b      	orrs	r3, r1
300057e4:	ea4a 0303 	orr.w	r3, sl, r3
300057e8:	ea4b 0303 	orr.w	r3, fp, r3
300057ec:	ea49 0303 	orr.w	r3, r9, r3
300057f0:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
300057f4:	f001 fea6 	bl	30007544 <CLK_SWITCH_XTAL>
300057f8:	4b39      	ldr	r3, [pc, #228]	; (300058e0 <BOOT_SOC_ClkSet+0x198>)
300057fa:	4a3a      	ldr	r2, [pc, #232]	; (300058e4 <BOOT_SOC_ClkSet+0x19c>)
300057fc:	2004      	movs	r0, #4
300057fe:	4619      	mov	r1, r3
30005800:	9500      	str	r5, [sp, #0]
30005802:	f005 fea7 	bl	3000b554 <rtk_log_write>
30005806:	783b      	ldrb	r3, [r7, #0]
30005808:	b1f3      	cbz	r3, 30005848 <BOOT_SOC_ClkSet+0x100>
3000580a:	6923      	ldr	r3, [r4, #16]
3000580c:	2b00      	cmp	r3, #0
3000580e:	db40      	blt.n	30005892 <BOOT_SOC_ClkSet+0x14a>
30005810:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005814:	492d      	ldr	r1, [pc, #180]	; (300058cc <BOOT_SOC_ClkSet+0x184>)
30005816:	2000      	movs	r0, #0
30005818:	6913      	ldr	r3, [r2, #16]
3000581a:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
3000581e:	f043 030c 	orr.w	r3, r3, #12
30005822:	6113      	str	r3, [r2, #16]
30005824:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
30005828:	6921      	ldr	r1, [r4, #16]
3000582a:	4b2d      	ldr	r3, [pc, #180]	; (300058e0 <BOOT_SOC_ClkSet+0x198>)
3000582c:	6928      	ldr	r0, [r5, #16]
3000582e:	3901      	subs	r1, #1
30005830:	4a2d      	ldr	r2, [pc, #180]	; (300058e8 <BOOT_SOC_ClkSet+0x1a0>)
30005832:	f020 0003 	bic.w	r0, r0, #3
30005836:	f001 0103 	and.w	r1, r1, #3
3000583a:	4301      	orrs	r1, r0
3000583c:	2004      	movs	r0, #4
3000583e:	6129      	str	r1, [r5, #16]
30005840:	4619      	mov	r1, r3
30005842:	9600      	str	r6, [sp, #0]
30005844:	f005 fe86 	bl	3000b554 <rtk_log_write>
30005848:	b003      	add	sp, #12
3000584a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000584e:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
30005852:	6863      	ldr	r3, [r4, #4]
30005854:	fbb3 f6f6 	udiv	r6, r3, r6
30005858:	e780      	b.n	3000575c <BOOT_SOC_ClkSet+0x14>
3000585a:	f240 718b 	movw	r1, #1931	; 0x78b
3000585e:	4823      	ldr	r0, [pc, #140]	; (300058ec <BOOT_SOC_ClkSet+0x1a4>)
30005860:	f006 f8be 	bl	3000b9e0 <__io_assert_failed_veneer>
30005864:	e795      	b.n	30005792 <BOOT_SOC_ClkSet+0x4a>
30005866:	4b22      	ldr	r3, [pc, #136]	; (300058f0 <BOOT_SOC_ClkSet+0x1a8>)
30005868:	4638      	mov	r0, r7
3000586a:	4798      	blx	r3
3000586c:	4b17      	ldr	r3, [pc, #92]	; (300058cc <BOOT_SOC_ClkSet+0x184>)
3000586e:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
30005872:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
30005876:	d21f      	bcs.n	300058b8 <BOOT_SOC_ClkSet+0x170>
30005878:	4f15      	ldr	r7, [pc, #84]	; (300058d0 <BOOT_SOC_ClkSet+0x188>)
3000587a:	783b      	ldrb	r3, [r7, #0]
3000587c:	2b01      	cmp	r3, #1
3000587e:	d190      	bne.n	300057a2 <BOOT_SOC_ClkSet+0x5a>
30005880:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
30005884:	d98d      	bls.n	300057a2 <BOOT_SOC_ClkSet+0x5a>
30005886:	f240 7187 	movw	r1, #1927	; 0x787
3000588a:	4818      	ldr	r0, [pc, #96]	; (300058ec <BOOT_SOC_ClkSet+0x1a4>)
3000588c:	f006 f8a8 	bl	3000b9e0 <__io_assert_failed_veneer>
30005890:	e787      	b.n	300057a2 <BOOT_SOC_ClkSet+0x5a>
30005892:	6860      	ldr	r0, [r4, #4]
30005894:	4b17      	ldr	r3, [pc, #92]	; (300058f4 <BOOT_SOC_ClkSet+0x1ac>)
30005896:	fb08 f000 	mul.w	r0, r8, r0
3000589a:	4798      	blx	r3
3000589c:	4b16      	ldr	r3, [pc, #88]	; (300058f8 <BOOT_SOC_ClkSet+0x1b0>)
3000589e:	2001      	movs	r0, #1
300058a0:	4798      	blx	r3
300058a2:	4b0a      	ldr	r3, [pc, #40]	; (300058cc <BOOT_SOC_ClkSet+0x184>)
300058a4:	2201      	movs	r2, #1
300058a6:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
300058aa:	e7bb      	b.n	30005824 <BOOT_SOC_ClkSet+0xdc>
300058ac:	f240 718d 	movw	r1, #1933	; 0x78d
300058b0:	480e      	ldr	r0, [pc, #56]	; (300058ec <BOOT_SOC_ClkSet+0x1a4>)
300058b2:	f006 f895 	bl	3000b9e0 <__io_assert_failed_veneer>
300058b6:	e774      	b.n	300057a2 <BOOT_SOC_ClkSet+0x5a>
300058b8:	f240 7185 	movw	r1, #1925	; 0x785
300058bc:	480b      	ldr	r0, [pc, #44]	; (300058ec <BOOT_SOC_ClkSet+0x1a4>)
300058be:	f006 f88f 	bl	3000b9e0 <__io_assert_failed_veneer>
300058c2:	e7d9      	b.n	30005878 <BOOT_SOC_ClkSet+0x130>
300058c4:	3000e614 	.word	0x3000e614
300058c8:	51eb851f 	.word	0x51eb851f
300058cc:	23020000 	.word	0x23020000
300058d0:	3000df3f 	.word	0x3000df3f
300058d4:	3000816d 	.word	0x3000816d
300058d8:	42008000 	.word	0x42008000
300058dc:	ff8f08f8 	.word	0xff8f08f8
300058e0:	3000bcf8 	.word	0x3000bcf8
300058e4:	3000c588 	.word	0x3000c588
300058e8:	3000c5a0 	.word	0x3000c5a0
300058ec:	3000dc04 	.word	0x3000dc04
300058f0:	3000aed5 	.word	0x3000aed5
300058f4:	300081ed 	.word	0x300081ed
300058f8:	3000822d 	.word	0x3000822d
300058fc:	000f4240 	.word	0x000f4240

30005900 <BOOT_Disable_AP>:
30005900:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005904:	4906      	ldr	r1, [pc, #24]	; (30005920 <BOOT_Disable_AP+0x20>)
30005906:	685a      	ldr	r2, [r3, #4]
30005908:	f042 0202 	orr.w	r2, r2, #2
3000590c:	605a      	str	r2, [r3, #4]
3000590e:	685a      	ldr	r2, [r3, #4]
30005910:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30005914:	605a      	str	r2, [r3, #4]
30005916:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30005918:	f043 0303 	orr.w	r3, r3, #3
3000591c:	678b      	str	r3, [r1, #120]	; 0x78
3000591e:	4770      	bx	lr
30005920:	41000200 	.word	0x41000200

30005924 <BOOT_Enable_AP>:
30005924:	b538      	push	{r3, r4, r5, lr}
30005926:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
3000592a:	2201      	movs	r2, #1
3000592c:	2104      	movs	r1, #4
3000592e:	4b2b      	ldr	r3, [pc, #172]	; (300059dc <BOOT_Enable_AP+0xb8>)
30005930:	482b      	ldr	r0, [pc, #172]	; (300059e0 <BOOT_Enable_AP+0xbc>)
30005932:	4798      	blx	r3
30005934:	6863      	ldr	r3, [r4, #4]
30005936:	4d2b      	ldr	r5, [pc, #172]	; (300059e4 <BOOT_Enable_AP+0xc0>)
30005938:	2032      	movs	r0, #50	; 0x32
3000593a:	f043 0302 	orr.w	r3, r3, #2
3000593e:	6063      	str	r3, [r4, #4]
30005940:	6863      	ldr	r3, [r4, #4]
30005942:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30005946:	6063      	str	r3, [r4, #4]
30005948:	6823      	ldr	r3, [r4, #0]
3000594a:	f043 0302 	orr.w	r3, r3, #2
3000594e:	6023      	str	r3, [r4, #0]
30005950:	47a8      	blx	r5
30005952:	6823      	ldr	r3, [r4, #0]
30005954:	2032      	movs	r0, #50	; 0x32
30005956:	f043 0304 	orr.w	r3, r3, #4
3000595a:	6023      	str	r3, [r4, #0]
3000595c:	6823      	ldr	r3, [r4, #0]
3000595e:	f043 0301 	orr.w	r3, r3, #1
30005962:	6023      	str	r3, [r4, #0]
30005964:	6823      	ldr	r3, [r4, #0]
30005966:	f043 0310 	orr.w	r3, r3, #16
3000596a:	6023      	str	r3, [r4, #0]
3000596c:	47a8      	blx	r5
3000596e:	6823      	ldr	r3, [r4, #0]
30005970:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30005974:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30005978:	6023      	str	r3, [r4, #0]
3000597a:	47a8      	blx	r5
3000597c:	6863      	ldr	r3, [r4, #4]
3000597e:	4a1a      	ldr	r2, [pc, #104]	; (300059e8 <BOOT_Enable_AP+0xc4>)
30005980:	f023 0310 	bic.w	r3, r3, #16
30005984:	6063      	str	r3, [r4, #4]
30005986:	6863      	ldr	r3, [r4, #4]
30005988:	f023 0302 	bic.w	r3, r3, #2
3000598c:	6063      	str	r3, [r4, #4]
3000598e:	4790      	blx	r2
30005990:	b178      	cbz	r0, 300059b2 <BOOT_Enable_AP+0x8e>
30005992:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005996:	4815      	ldr	r0, [pc, #84]	; (300059ec <BOOT_Enable_AP+0xc8>)
30005998:	4a15      	ldr	r2, [pc, #84]	; (300059f0 <BOOT_Enable_AP+0xcc>)
3000599a:	68d9      	ldr	r1, [r3, #12]
3000599c:	f041 0101 	orr.w	r1, r1, #1
300059a0:	60d9      	str	r1, [r3, #12]
300059a2:	6899      	ldr	r1, [r3, #8]
300059a4:	f041 0101 	orr.w	r1, r1, #1
300059a8:	6099      	str	r1, [r3, #8]
300059aa:	6843      	ldr	r3, [r0, #4]
300059ac:	431a      	orrs	r2, r3
300059ae:	6042      	str	r2, [r0, #4]
300059b0:	bd38      	pop	{r3, r4, r5, pc}
300059b2:	4604      	mov	r4, r0
300059b4:	4d0f      	ldr	r5, [pc, #60]	; (300059f4 <BOOT_Enable_AP+0xd0>)
300059b6:	4601      	mov	r1, r0
300059b8:	2041      	movs	r0, #65	; 0x41
300059ba:	47a8      	blx	r5
300059bc:	4621      	mov	r1, r4
300059be:	2042      	movs	r0, #66	; 0x42
300059c0:	47a8      	blx	r5
300059c2:	4621      	mov	r1, r4
300059c4:	2043      	movs	r0, #67	; 0x43
300059c6:	47a8      	blx	r5
300059c8:	4621      	mov	r1, r4
300059ca:	2044      	movs	r0, #68	; 0x44
300059cc:	47a8      	blx	r5
300059ce:	4621      	mov	r1, r4
300059d0:	2045      	movs	r0, #69	; 0x45
300059d2:	47a8      	blx	r5
300059d4:	4621      	mov	r1, r4
300059d6:	2046      	movs	r0, #70	; 0x46
300059d8:	47a8      	blx	r5
300059da:	e7da      	b.n	30005992 <BOOT_Enable_AP+0x6e>
300059dc:	0000aab9 	.word	0x0000aab9
300059e0:	4200c000 	.word	0x4200c000
300059e4:	00009b2d 	.word	0x00009b2d
300059e8:	30009265 	.word	0x30009265
300059ec:	41000200 	.word	0x41000200
300059f0:	01001111 	.word	0x01001111
300059f4:	0000b20d 	.word	0x0000b20d

300059f8 <BOOT_RAM_KeyDeriveFunc>:
300059f8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300059fc:	ed2d 8b04 	vpush	{d8-d9}
30005a00:	b0cb      	sub	sp, #300	; 0x12c
30005a02:	4c6a      	ldr	r4, [pc, #424]	; (30005bac <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30005a04:	4680      	mov	r8, r0
30005a06:	4689      	mov	r9, r1
30005a08:	4869      	ldr	r0, [pc, #420]	; (30005bb0 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30005a0a:	ee08 2a90 	vmov	s17, r2
30005a0e:	4969      	ldr	r1, [pc, #420]	; (30005bb4 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005a10:	2201      	movs	r2, #1
30005a12:	ee09 3a10 	vmov	s18, r3
30005a16:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30005a1a:	47a0      	blx	r4
30005a1c:	2201      	movs	r2, #1
30005a1e:	4966      	ldr	r1, [pc, #408]	; (30005bb8 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30005a20:	ae02      	add	r6, sp, #8
30005a22:	4864      	ldr	r0, [pc, #400]	; (30005bb4 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005a24:	47a0      	blx	r4
30005a26:	4b65      	ldr	r3, [pc, #404]	; (30005bbc <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30005a28:	2000      	movs	r0, #0
30005a2a:	4798      	blx	r3
30005a2c:	22e0      	movs	r2, #224	; 0xe0
30005a2e:	2100      	movs	r1, #0
30005a30:	a812      	add	r0, sp, #72	; 0x48
30005a32:	f006 f815 	bl	3000ba60 <____wrap_memset_veneer>
30005a36:	2300      	movs	r3, #0
30005a38:	f8ad 3004 	strh.w	r3, [sp, #4]
30005a3c:	f88d 3006 	strb.w	r3, [sp, #6]
30005a40:	2301      	movs	r3, #1
30005a42:	f88d 3007 	strb.w	r3, [sp, #7]
30005a46:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005a48:	2b00      	cmp	r3, #0
30005a4a:	f000 80ab 	beq.w	30005ba4 <BOOT_RAM_KeyDeriveFunc+0x1ac>
30005a4e:	ab0a      	add	r3, sp, #40	; 0x28
30005a50:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30005bd0 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30005a54:	4f5a      	ldr	r7, [pc, #360]	; (30005bc0 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30005a56:	ee08 3a10 	vmov	s16, r3
30005a5a:	ab12      	add	r3, sp, #72	; 0x48
30005a5c:	464a      	mov	r2, r9
30005a5e:	4641      	mov	r1, r8
30005a60:	2020      	movs	r0, #32
30005a62:	47d8      	blx	fp
30005a64:	aa12      	add	r2, sp, #72	; 0x48
30005a66:	ee19 1a10 	vmov	r1, s18
30005a6a:	ee18 0a90 	vmov	r0, s17
30005a6e:	47b8      	blx	r7
30005a70:	ee18 5a10 	vmov	r5, s16
30005a74:	aa12      	add	r2, sp, #72	; 0x48
30005a76:	2104      	movs	r1, #4
30005a78:	a801      	add	r0, sp, #4
30005a7a:	4634      	mov	r4, r6
30005a7c:	47b8      	blx	r7
30005a7e:	a912      	add	r1, sp, #72	; 0x48
30005a80:	4b50      	ldr	r3, [pc, #320]	; (30005bc4 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30005a82:	ee18 0a10 	vmov	r0, s16
30005a86:	4798      	blx	r3
30005a88:	f1ba 0f01 	cmp.w	sl, #1
30005a8c:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30005a8e:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30005a90:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30005a94:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30005a98:	d91f      	bls.n	30005ada <BOOT_RAM_KeyDeriveFunc+0xe2>
30005a9a:	2501      	movs	r5, #1
30005a9c:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30005aa0:	ab12      	add	r3, sp, #72	; 0x48
30005aa2:	4641      	mov	r1, r8
30005aa4:	464a      	mov	r2, r9
30005aa6:	2020      	movs	r0, #32
30005aa8:	47d8      	blx	fp
30005aaa:	aa12      	add	r2, sp, #72	; 0x48
30005aac:	2120      	movs	r1, #32
30005aae:	4630      	mov	r0, r6
30005ab0:	47b8      	blx	r7
30005ab2:	a912      	add	r1, sp, #72	; 0x48
30005ab4:	4b43      	ldr	r3, [pc, #268]	; (30005bc4 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30005ab6:	4630      	mov	r0, r6
30005ab8:	4798      	blx	r3
30005aba:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30005abe:	f10d 0107 	add.w	r1, sp, #7
30005ac2:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30005ac6:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30005aca:	42a3      	cmp	r3, r4
30005acc:	ea82 0200 	eor.w	r2, r2, r0
30005ad0:	701a      	strb	r2, [r3, #0]
30005ad2:	d1f6      	bne.n	30005ac2 <BOOT_RAM_KeyDeriveFunc+0xca>
30005ad4:	3501      	adds	r5, #1
30005ad6:	45aa      	cmp	sl, r5
30005ad8:	d1e2      	bne.n	30005aa0 <BOOT_RAM_KeyDeriveFunc+0xa8>
30005ada:	9a59      	ldr	r2, [sp, #356]	; 0x164
30005adc:	ee18 1a10 	vmov	r1, s16
30005ae0:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005ae2:	2a20      	cmp	r2, #32
30005ae4:	985a      	ldr	r0, [sp, #360]	; 0x168
30005ae6:	bf28      	it	cs
30005ae8:	2220      	movcs	r2, #32
30005aea:	1a9b      	subs	r3, r3, r2
30005aec:	9359      	str	r3, [sp, #356]	; 0x164
30005aee:	4603      	mov	r3, r0
30005af0:	4413      	add	r3, r2
30005af2:	935a      	str	r3, [sp, #360]	; 0x168
30005af4:	f005 fffc 	bl	3000baf0 <____wrap_memcpy_veneer>
30005af8:	f10d 0207 	add.w	r2, sp, #7
30005afc:	2104      	movs	r1, #4
30005afe:	7813      	ldrb	r3, [r2, #0]
30005b00:	3901      	subs	r1, #1
30005b02:	3301      	adds	r3, #1
30005b04:	b2db      	uxtb	r3, r3
30005b06:	f802 3901 	strb.w	r3, [r2], #-1
30005b0a:	b90b      	cbnz	r3, 30005b10 <BOOT_RAM_KeyDeriveFunc+0x118>
30005b0c:	2900      	cmp	r1, #0
30005b0e:	d1f6      	bne.n	30005afe <BOOT_RAM_KeyDeriveFunc+0x106>
30005b10:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005b12:	2b00      	cmp	r3, #0
30005b14:	d1a1      	bne.n	30005a5a <BOOT_RAM_KeyDeriveFunc+0x62>
30005b16:	2220      	movs	r2, #32
30005b18:	2100      	movs	r1, #0
30005b1a:	4c2b      	ldr	r4, [pc, #172]	; (30005bc8 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30005b1c:	4630      	mov	r0, r6
30005b1e:	47a0      	blx	r4
30005b20:	2220      	movs	r2, #32
30005b22:	2100      	movs	r1, #0
30005b24:	ee18 0a10 	vmov	r0, s16
30005b28:	47a0      	blx	r4
30005b2a:	4b28      	ldr	r3, [pc, #160]	; (30005bcc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005b2c:	695b      	ldr	r3, [r3, #20]
30005b2e:	03da      	lsls	r2, r3, #15
30005b30:	d512      	bpl.n	30005b58 <BOOT_RAM_KeyDeriveFunc+0x160>
30005b32:	f016 021f 	ands.w	r2, r6, #31
30005b36:	4633      	mov	r3, r6
30005b38:	d130      	bne.n	30005b9c <BOOT_RAM_KeyDeriveFunc+0x1a4>
30005b3a:	2220      	movs	r2, #32
30005b3c:	f3bf 8f4f 	dsb	sy
30005b40:	4822      	ldr	r0, [pc, #136]	; (30005bcc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005b42:	441a      	add	r2, r3
30005b44:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005b48:	3320      	adds	r3, #32
30005b4a:	1ad1      	subs	r1, r2, r3
30005b4c:	2900      	cmp	r1, #0
30005b4e:	dcf9      	bgt.n	30005b44 <BOOT_RAM_KeyDeriveFunc+0x14c>
30005b50:	f3bf 8f4f 	dsb	sy
30005b54:	f3bf 8f6f 	isb	sy
30005b58:	4b1c      	ldr	r3, [pc, #112]	; (30005bcc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005b5a:	695b      	ldr	r3, [r3, #20]
30005b5c:	03db      	lsls	r3, r3, #15
30005b5e:	d513      	bpl.n	30005b88 <BOOT_RAM_KeyDeriveFunc+0x190>
30005b60:	ee18 3a10 	vmov	r3, s16
30005b64:	f013 021f 	ands.w	r2, r3, #31
30005b68:	d114      	bne.n	30005b94 <BOOT_RAM_KeyDeriveFunc+0x19c>
30005b6a:	2220      	movs	r2, #32
30005b6c:	f3bf 8f4f 	dsb	sy
30005b70:	4816      	ldr	r0, [pc, #88]	; (30005bcc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005b72:	441a      	add	r2, r3
30005b74:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005b78:	3320      	adds	r3, #32
30005b7a:	1ad1      	subs	r1, r2, r3
30005b7c:	2900      	cmp	r1, #0
30005b7e:	dcf9      	bgt.n	30005b74 <BOOT_RAM_KeyDeriveFunc+0x17c>
30005b80:	f3bf 8f4f 	dsb	sy
30005b84:	f3bf 8f6f 	isb	sy
30005b88:	2000      	movs	r0, #0
30005b8a:	b04b      	add	sp, #300	; 0x12c
30005b8c:	ecbd 8b04 	vpop	{d8-d9}
30005b90:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005b94:	f023 031f 	bic.w	r3, r3, #31
30005b98:	3220      	adds	r2, #32
30005b9a:	e7e7      	b.n	30005b6c <BOOT_RAM_KeyDeriveFunc+0x174>
30005b9c:	f026 031f 	bic.w	r3, r6, #31
30005ba0:	3220      	adds	r2, #32
30005ba2:	e7cb      	b.n	30005b3c <BOOT_RAM_KeyDeriveFunc+0x144>
30005ba4:	ab0a      	add	r3, sp, #40	; 0x28
30005ba6:	ee08 3a10 	vmov	s16, r3
30005baa:	e7b4      	b.n	30005b16 <BOOT_RAM_KeyDeriveFunc+0x11e>
30005bac:	0000b479 	.word	0x0000b479
30005bb0:	40000008 	.word	0x40000008
30005bb4:	40000004 	.word	0x40000004
30005bb8:	40000002 	.word	0x40000002
30005bbc:	30009501 	.word	0x30009501
30005bc0:	30009305 	.word	0x30009305
30005bc4:	30009341 	.word	0x30009341
30005bc8:	00012be5 	.word	0x00012be5
30005bcc:	e000ed00 	.word	0xe000ed00
30005bd0:	30008c05 	.word	0x30008c05

30005bd4 <BOOT_Share_Memory_Patch>:
30005bd4:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005bd8:	4817      	ldr	r0, [pc, #92]	; (30005c38 <BOOT_Share_Memory_Patch+0x64>)
30005bda:	4918      	ldr	r1, [pc, #96]	; (30005c3c <BOOT_Share_Memory_Patch+0x68>)
30005bdc:	b430      	push	{r4, r5}
30005bde:	6b13      	ldr	r3, [r2, #48]	; 0x30
30005be0:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30005be4:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30005be8:	6313      	str	r3, [r2, #48]	; 0x30
30005bea:	6823      	ldr	r3, [r4, #0]
30005bec:	6804      	ldr	r4, [r0, #0]
30005bee:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005bf2:	6809      	ldr	r1, [r1, #0]
30005bf4:	4423      	add	r3, r4
30005bf6:	6805      	ldr	r5, [r0, #0]
30005bf8:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30005bfc:	440b      	add	r3, r1
30005bfe:	4910      	ldr	r1, [pc, #64]	; (30005c40 <BOOT_Share_Memory_Patch+0x6c>)
30005c00:	6808      	ldr	r0, [r1, #0]
30005c02:	442b      	add	r3, r5
30005c04:	6b11      	ldr	r1, [r2, #48]	; 0x30
30005c06:	4403      	add	r3, r0
30005c08:	480e      	ldr	r0, [pc, #56]	; (30005c44 <BOOT_Share_Memory_Patch+0x70>)
30005c0a:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30005c0e:	6311      	str	r1, [r2, #48]	; 0x30
30005c10:	6821      	ldr	r1, [r4, #0]
30005c12:	4a0d      	ldr	r2, [pc, #52]	; (30005c48 <BOOT_Share_Memory_Patch+0x74>)
30005c14:	6804      	ldr	r4, [r0, #0]
30005c16:	440b      	add	r3, r1
30005c18:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005c1c:	6811      	ldr	r1, [r2, #0]
30005c1e:	4423      	add	r3, r4
30005c20:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30005c24:	6800      	ldr	r0, [r0, #0]
30005c26:	440b      	add	r3, r1
30005c28:	6812      	ldr	r2, [r2, #0]
30005c2a:	4908      	ldr	r1, [pc, #32]	; (30005c4c <BOOT_Share_Memory_Patch+0x78>)
30005c2c:	4403      	add	r3, r0
30005c2e:	6808      	ldr	r0, [r1, #0]
30005c30:	4413      	add	r3, r2
30005c32:	bc30      	pop	{r4, r5}
30005c34:	4418      	add	r0, r3
30005c36:	4770      	bx	lr
30005c38:	2200c000 	.word	0x2200c000
30005c3c:	22018000 	.word	0x22018000
30005c40:	22100000 	.word	0x22100000
30005c44:	20010000 	.word	0x20010000
30005c48:	20020000 	.word	0x20020000
30005c4c:	23010000 	.word	0x23010000

30005c50 <BOOT_Image1>:
30005c50:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30005c54:	4bac      	ldr	r3, [pc, #688]	; (30005f08 <BOOT_Image1+0x2b8>)
30005c56:	b082      	sub	sp, #8
30005c58:	4798      	blx	r3
30005c5a:	2800      	cmp	r0, #0
30005c5c:	f040 80c1 	bne.w	30005de2 <BOOT_Image1+0x192>
30005c60:	48aa      	ldr	r0, [pc, #680]	; (30005f0c <BOOT_Image1+0x2bc>)
30005c62:	f005 ff5d 	bl	3000bb20 <__DiagPrintf_veneer>
30005c66:	49aa      	ldr	r1, [pc, #680]	; (30005f10 <BOOT_Image1+0x2c0>)
30005c68:	48aa      	ldr	r0, [pc, #680]	; (30005f14 <BOOT_Image1+0x2c4>)
30005c6a:	f005 ff59 	bl	3000bb20 <__DiagPrintf_veneer>
30005c6e:	48aa      	ldr	r0, [pc, #680]	; (30005f18 <BOOT_Image1+0x2c8>)
30005c70:	4aaa      	ldr	r2, [pc, #680]	; (30005f1c <BOOT_Image1+0x2cc>)
30005c72:	2100      	movs	r1, #0
30005c74:	4baa      	ldr	r3, [pc, #680]	; (30005f20 <BOOT_Image1+0x2d0>)
30005c76:	1a12      	subs	r2, r2, r0
30005c78:	4798      	blx	r3
30005c7a:	4aaa      	ldr	r2, [pc, #680]	; (30005f24 <BOOT_Image1+0x2d4>)
30005c7c:	49aa      	ldr	r1, [pc, #680]	; (30005f28 <BOOT_Image1+0x2d8>)
30005c7e:	8c13      	ldrh	r3, [r2, #32]
30005c80:	b29b      	uxth	r3, r3
30005c82:	8413      	strh	r3, [r2, #32]
30005c84:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005c88:	f002 fede 	bl	30008a48 <BOOT_Reason>
30005c8c:	2800      	cmp	r0, #0
30005c8e:	f000 80e9 	beq.w	30005e64 <BOOT_Image1+0x214>
30005c92:	4ea6      	ldr	r6, [pc, #664]	; (30005f2c <BOOT_Image1+0x2dc>)
30005c94:	4ba6      	ldr	r3, [pc, #664]	; (30005f30 <BOOT_Image1+0x2e0>)
30005c96:	4798      	blx	r3
30005c98:	7834      	ldrb	r4, [r6, #0]
30005c9a:	2c00      	cmp	r4, #0
30005c9c:	f000 8097 	beq.w	30005dce <BOOT_Image1+0x17e>
30005ca0:	2000      	movs	r0, #0
30005ca2:	4ba4      	ldr	r3, [pc, #656]	; (30005f34 <BOOT_Image1+0x2e4>)
30005ca4:	4798      	blx	r3
30005ca6:	4ba4      	ldr	r3, [pc, #656]	; (30005f38 <BOOT_Image1+0x2e8>)
30005ca8:	2001      	movs	r0, #1
30005caa:	4798      	blx	r3
30005cac:	4c9e      	ldr	r4, [pc, #632]	; (30005f28 <BOOT_Image1+0x2d8>)
30005cae:	f7ff fd4b 	bl	30005748 <BOOT_SOC_ClkSet>
30005cb2:	f241 070a 	movw	r7, #4106	; 0x100a
30005cb6:	2028      	movs	r0, #40	; 0x28
30005cb8:	8823      	ldrh	r3, [r4, #0]
30005cba:	4da0      	ldr	r5, [pc, #640]	; (30005f3c <BOOT_Image1+0x2ec>)
30005cbc:	b29b      	uxth	r3, r3
30005cbe:	431f      	orrs	r7, r3
30005cc0:	4b9f      	ldr	r3, [pc, #636]	; (30005f40 <BOOT_Image1+0x2f0>)
30005cc2:	8027      	strh	r7, [r4, #0]
30005cc4:	4798      	blx	r3
30005cc6:	88a3      	ldrh	r3, [r4, #4]
30005cc8:	b29b      	uxth	r3, r3
30005cca:	ea23 0307 	bic.w	r3, r3, r7
30005cce:	80a3      	strh	r3, [r4, #4]
30005cd0:	8863      	ldrh	r3, [r4, #2]
30005cd2:	8067      	strh	r7, [r4, #2]
30005cd4:	47a8      	blx	r5
30005cd6:	2802      	cmp	r0, #2
30005cd8:	f000 808b 	beq.w	30005df2 <BOOT_Image1+0x1a2>
30005cdc:	4b99      	ldr	r3, [pc, #612]	; (30005f44 <BOOT_Image1+0x2f4>)
30005cde:	2401      	movs	r4, #1
30005ce0:	4999      	ldr	r1, [pc, #612]	; (30005f48 <BOOT_Image1+0x2f8>)
30005ce2:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005ce6:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
30005cea:	4622      	mov	r2, r4
30005cec:	4f97      	ldr	r7, [pc, #604]	; (30005f4c <BOOT_Image1+0x2fc>)
30005cee:	47b8      	blx	r7
30005cf0:	4622      	mov	r2, r4
30005cf2:	4997      	ldr	r1, [pc, #604]	; (30005f50 <BOOT_Image1+0x300>)
30005cf4:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005cf8:	47b8      	blx	r7
30005cfa:	4a8a      	ldr	r2, [pc, #552]	; (30005f24 <BOOT_Image1+0x2d4>)
30005cfc:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005d00:	f043 0303 	orr.w	r3, r3, #3
30005d04:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005d08:	4b92      	ldr	r3, [pc, #584]	; (30005f54 <BOOT_Image1+0x304>)
30005d0a:	4798      	blx	r3
30005d0c:	4a85      	ldr	r2, [pc, #532]	; (30005f24 <BOOT_Image1+0x2d4>)
30005d0e:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30005d12:	f023 0303 	bic.w	r3, r3, #3
30005d16:	b108      	cbz	r0, 30005d1c <BOOT_Image1+0xcc>
30005d18:	f043 0302 	orr.w	r3, r3, #2
30005d1c:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30005d20:	4b8d      	ldr	r3, [pc, #564]	; (30005f58 <BOOT_Image1+0x308>)
30005d22:	781b      	ldrb	r3, [r3, #0]
30005d24:	2b00      	cmp	r3, #0
30005d26:	d17f      	bne.n	30005e28 <BOOT_Image1+0x1d8>
30005d28:	4c8c      	ldr	r4, [pc, #560]	; (30005f5c <BOOT_Image1+0x30c>)
30005d2a:	2102      	movs	r1, #2
30005d2c:	2201      	movs	r2, #1
30005d2e:	488c      	ldr	r0, [pc, #560]	; (30005f60 <BOOT_Image1+0x310>)
30005d30:	47a0      	blx	r4
30005d32:	4c8c      	ldr	r4, [pc, #560]	; (30005f64 <BOOT_Image1+0x314>)
30005d34:	68a3      	ldr	r3, [r4, #8]
30005d36:	0799      	lsls	r1, r3, #30
30005d38:	d470      	bmi.n	30005e1c <BOOT_Image1+0x1cc>
30005d3a:	498b      	ldr	r1, [pc, #556]	; (30005f68 <BOOT_Image1+0x318>)
30005d3c:	e841 f100 	tt	r1, r1
30005d40:	68a3      	ldr	r3, [r4, #8]
30005d42:	f3c1 5180 	ubfx	r1, r1, #22, #1
30005d46:	079a      	lsls	r2, r3, #30
30005d48:	f100 8088 	bmi.w	30005e5c <BOOT_Image1+0x20c>
30005d4c:	4b87      	ldr	r3, [pc, #540]	; (30005f6c <BOOT_Image1+0x31c>)
30005d4e:	4f88      	ldr	r7, [pc, #544]	; (30005f70 <BOOT_Image1+0x320>)
30005d50:	681b      	ldr	r3, [r3, #0]
30005d52:	4798      	blx	r3
30005d54:	f7fe f96e 	bl	30004034 <BOOT_RccConfig>
30005d58:	f7fe f9d8 	bl	3000410c <BOOT_CACHEWRR_Set>
30005d5c:	4b85      	ldr	r3, [pc, #532]	; (30005f74 <BOOT_Image1+0x324>)
30005d5e:	6818      	ldr	r0, [r3, #0]
30005d60:	f7fe fa02 	bl	30004168 <BOOT_TCMSet>
30005d64:	4b84      	ldr	r3, [pc, #528]	; (30005f78 <BOOT_Image1+0x328>)
30005d66:	695b      	ldr	r3, [r3, #20]
30005d68:	03db      	lsls	r3, r3, #15
30005d6a:	d512      	bpl.n	30005d92 <BOOT_Image1+0x142>
30005d6c:	f017 011f 	ands.w	r1, r7, #31
30005d70:	d13b      	bne.n	30005dea <BOOT_Image1+0x19a>
30005d72:	463b      	mov	r3, r7
30005d74:	2174      	movs	r1, #116	; 0x74
30005d76:	f3bf 8f4f 	dsb	sy
30005d7a:	487f      	ldr	r0, [pc, #508]	; (30005f78 <BOOT_Image1+0x328>)
30005d7c:	4419      	add	r1, r3
30005d7e:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30005d82:	3320      	adds	r3, #32
30005d84:	1aca      	subs	r2, r1, r3
30005d86:	2a00      	cmp	r2, #0
30005d88:	dcf9      	bgt.n	30005d7e <BOOT_Image1+0x12e>
30005d8a:	f3bf 8f4f 	dsb	sy
30005d8e:	f3bf 8f6f 	isb	sy
30005d92:	4b6c      	ldr	r3, [pc, #432]	; (30005f44 <BOOT_Image1+0x2f4>)
30005d94:	2200      	movs	r2, #0
30005d96:	f8df 818c 	ldr.w	r8, [pc, #396]	; 30005f24 <BOOT_Image1+0x2d4>
30005d9a:	f8c8 7270 	str.w	r7, [r8, #624]	; 0x270
30005d9e:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30005da2:	f7fd fc8d 	bl	300036c0 <flash_highspeed_setup>
30005da6:	47a8      	blx	r5
30005da8:	2802      	cmp	r0, #2
30005daa:	f000 8170 	beq.w	3000608e <BOOT_Image1+0x43e>
30005dae:	7833      	ldrb	r3, [r6, #0]
30005db0:	2b00      	cmp	r3, #0
30005db2:	d05e      	beq.n	30005e72 <BOOT_Image1+0x222>
30005db4:	4c71      	ldr	r4, [pc, #452]	; (30005f7c <BOOT_Image1+0x32c>)
30005db6:	4e72      	ldr	r6, [pc, #456]	; (30005f80 <BOOT_Image1+0x330>)
30005db8:	4d72      	ldr	r5, [pc, #456]	; (30005f84 <BOOT_Image1+0x334>)
30005dba:	4633      	mov	r3, r6
30005dbc:	462a      	mov	r2, r5
30005dbe:	4970      	ldr	r1, [pc, #448]	; (30005f80 <BOOT_Image1+0x330>)
30005dc0:	2002      	movs	r0, #2
30005dc2:	f005 fbc7 	bl	3000b554 <rtk_log_write>
30005dc6:	f241 3088 	movw	r0, #5000	; 0x1388
30005dca:	47a0      	blx	r4
30005dcc:	e7f5      	b.n	30005dba <BOOT_Image1+0x16a>
30005dce:	4b5a      	ldr	r3, [pc, #360]	; (30005f38 <BOOT_Image1+0x2e8>)
30005dd0:	2001      	movs	r0, #1
30005dd2:	4798      	blx	r3
30005dd4:	4620      	mov	r0, r4
30005dd6:	4b6c      	ldr	r3, [pc, #432]	; (30005f88 <BOOT_Image1+0x338>)
30005dd8:	4798      	blx	r3
30005dda:	4b6c      	ldr	r3, [pc, #432]	; (30005f8c <BOOT_Image1+0x33c>)
30005ddc:	2001      	movs	r0, #1
30005dde:	4798      	blx	r3
30005de0:	e764      	b.n	30005cac <BOOT_Image1+0x5c>
30005de2:	486b      	ldr	r0, [pc, #428]	; (30005f90 <BOOT_Image1+0x340>)
30005de4:	f005 fe9c 	bl	3000bb20 <__DiagPrintf_veneer>
30005de8:	e73d      	b.n	30005c66 <BOOT_Image1+0x16>
30005dea:	3174      	adds	r1, #116	; 0x74
30005dec:	f027 031f 	bic.w	r3, r7, #31
30005df0:	e7c1      	b.n	30005d76 <BOOT_Image1+0x126>
30005df2:	6de3      	ldr	r3, [r4, #92]	; 0x5c
30005df4:	2201      	movs	r2, #1
30005df6:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005dfa:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30005dfe:	65e3      	str	r3, [r4, #92]	; 0x5c
30005e00:	4b50      	ldr	r3, [pc, #320]	; (30005f44 <BOOT_Image1+0x2f4>)
30005e02:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30005e06:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005e0a:	4b50      	ldr	r3, [pc, #320]	; (30005f4c <BOOT_Image1+0x2fc>)
30005e0c:	4798      	blx	r3
30005e0e:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
30005e12:	f043 0302 	orr.w	r3, r3, #2
30005e16:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
30005e1a:	e775      	b.n	30005d08 <BOOT_Image1+0xb8>
30005e1c:	f3ef 8108 	mrs	r1, MSP
30005e20:	485c      	ldr	r0, [pc, #368]	; (30005f94 <BOOT_Image1+0x344>)
30005e22:	f005 fe7d 	bl	3000bb20 <__DiagPrintf_veneer>
30005e26:	e788      	b.n	30005d3a <BOOT_Image1+0xea>
30005e28:	4f5b      	ldr	r7, [pc, #364]	; (30005f98 <BOOT_Image1+0x348>)
30005e2a:	47b8      	blx	r7
30005e2c:	4604      	mov	r4, r0
30005e2e:	47b8      	blx	r7
30005e30:	2c02      	cmp	r4, #2
30005e32:	f43f af79 	beq.w	30005d28 <BOOT_Image1+0xd8>
30005e36:	2803      	cmp	r0, #3
30005e38:	f43f af76 	beq.w	30005d28 <BOOT_Image1+0xd8>
30005e3c:	4b57      	ldr	r3, [pc, #348]	; (30005f9c <BOOT_Image1+0x34c>)
30005e3e:	4798      	blx	r3
30005e40:	4c46      	ldr	r4, [pc, #280]	; (30005f5c <BOOT_Image1+0x30c>)
30005e42:	2200      	movs	r2, #0
30005e44:	2101      	movs	r1, #1
30005e46:	4846      	ldr	r0, [pc, #280]	; (30005f60 <BOOT_Image1+0x310>)
30005e48:	47a0      	blx	r4
30005e4a:	2101      	movs	r1, #1
30005e4c:	4844      	ldr	r0, [pc, #272]	; (30005f60 <BOOT_Image1+0x310>)
30005e4e:	4b54      	ldr	r3, [pc, #336]	; (30005fa0 <BOOT_Image1+0x350>)
30005e50:	4798      	blx	r3
30005e52:	2201      	movs	r2, #1
30005e54:	4842      	ldr	r0, [pc, #264]	; (30005f60 <BOOT_Image1+0x310>)
30005e56:	4611      	mov	r1, r2
30005e58:	47a0      	blx	r4
30005e5a:	e766      	b.n	30005d2a <BOOT_Image1+0xda>
30005e5c:	4851      	ldr	r0, [pc, #324]	; (30005fa4 <BOOT_Image1+0x354>)
30005e5e:	f005 fe5f 	bl	3000bb20 <__DiagPrintf_veneer>
30005e62:	e773      	b.n	30005d4c <BOOT_Image1+0xfc>
30005e64:	4601      	mov	r1, r0
30005e66:	f44f 7201 	mov.w	r2, #516	; 0x204
30005e6a:	4836      	ldr	r0, [pc, #216]	; (30005f44 <BOOT_Image1+0x2f4>)
30005e6c:	f005 fdf8 	bl	3000ba60 <____wrap_memset_veneer>
30005e70:	e70f      	b.n	30005c92 <BOOT_Image1+0x42>
30005e72:	4b43      	ldr	r3, [pc, #268]	; (30005f80 <BOOT_Image1+0x330>)
30005e74:	2004      	movs	r0, #4
30005e76:	4a4c      	ldr	r2, [pc, #304]	; (30005fa8 <BOOT_Image1+0x358>)
30005e78:	4619      	mov	r1, r3
30005e7a:	f005 fb6b 	bl	3000b554 <rtk_log_write>
30005e7e:	f7fe fb0b 	bl	30004498 <BOOT_DDR_Init>
30005e82:	f8d8 3290 	ldr.w	r3, [r8, #656]	; 0x290
30005e86:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005e8a:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005e8e:	f8c8 3290 	str.w	r3, [r8, #656]	; 0x290
30005e92:	2200      	movs	r2, #0
30005e94:	4b23      	ldr	r3, [pc, #140]	; (30005f24 <BOOT_Image1+0x2d4>)
30005e96:	4d45      	ldr	r5, [pc, #276]	; (30005fac <BOOT_Image1+0x35c>)
30005e98:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
30005e9c:	f8c3 22d4 	str.w	r2, [r3, #724]	; 0x2d4
30005ea0:	f8c3 22d8 	str.w	r2, [r3, #728]	; 0x2d8
30005ea4:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
30005ea8:	f7ff fe94 	bl	30005bd4 <BOOT_Share_Memory_Patch>
30005eac:	4b40      	ldr	r3, [pc, #256]	; (30005fb0 <BOOT_Image1+0x360>)
30005eae:	2002      	movs	r0, #2
30005eb0:	4798      	blx	r3
30005eb2:	4b33      	ldr	r3, [pc, #204]	; (30005f80 <BOOT_Image1+0x330>)
30005eb4:	9000      	str	r0, [sp, #0]
30005eb6:	2004      	movs	r0, #4
30005eb8:	4619      	mov	r1, r3
30005eba:	4a3e      	ldr	r2, [pc, #248]	; (30005fb4 <BOOT_Image1+0x364>)
30005ebc:	f005 fb4a 	bl	3000b554 <rtk_log_write>
30005ec0:	782b      	ldrb	r3, [r5, #0]
30005ec2:	2b00      	cmp	r3, #0
30005ec4:	f040 8107 	bne.w	300060d6 <BOOT_Image1+0x486>
30005ec8:	6823      	ldr	r3, [r4, #0]
30005eca:	4e3b      	ldr	r6, [pc, #236]	; (30005fb8 <BOOT_Image1+0x368>)
30005ecc:	f043 0302 	orr.w	r3, r3, #2
30005ed0:	6023      	str	r3, [r4, #0]
30005ed2:	47b0      	blx	r6
30005ed4:	b190      	cbz	r0, 30005efc <BOOT_Image1+0x2ac>
30005ed6:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30005eda:	4838      	ldr	r0, [pc, #224]	; (30005fbc <BOOT_Image1+0x36c>)
30005edc:	f005 fe20 	bl	3000bb20 <__DiagPrintf_veneer>
30005ee0:	4837      	ldr	r0, [pc, #220]	; (30005fc0 <BOOT_Image1+0x370>)
30005ee2:	f005 fe1d 	bl	3000bb20 <__DiagPrintf_veneer>
30005ee6:	f005 fb71 	bl	3000b5cc <BOOT_VerCheck>
30005eea:	f7fe fcc9 	bl	30004880 <BOOT_LoadImages>
30005eee:	2800      	cmp	r0, #0
30005ef0:	d16a      	bne.n	30005fc8 <BOOT_Image1+0x378>
30005ef2:	4c22      	ldr	r4, [pc, #136]	; (30005f7c <BOOT_Image1+0x32c>)
30005ef4:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005ef8:	47a0      	blx	r4
30005efa:	e7fb      	b.n	30005ef4 <BOOT_Image1+0x2a4>
30005efc:	4931      	ldr	r1, [pc, #196]	; (30005fc4 <BOOT_Image1+0x374>)
30005efe:	482f      	ldr	r0, [pc, #188]	; (30005fbc <BOOT_Image1+0x36c>)
30005f00:	f005 fe0e 	bl	3000bb20 <__DiagPrintf_veneer>
30005f04:	e7ec      	b.n	30005ee0 <BOOT_Image1+0x290>
30005f06:	bf00      	nop
30005f08:	0000c149 	.word	0x0000c149
30005f0c:	3000c5b8 	.word	0x3000c5b8
30005f10:	3000dc54 	.word	0x3000dc54
30005f14:	3000c5dc 	.word	0x3000c5dc
30005f18:	3000f330 	.word	0x3000f330
30005f1c:	3000f7d4 	.word	0x3000f7d4
30005f20:	00012be5 	.word	0x00012be5
30005f24:	42008000 	.word	0x42008000
30005f28:	42008200 	.word	0x42008200
30005f2c:	3000df3e 	.word	0x3000df3e
30005f30:	3000a9ad 	.word	0x3000a9ad
30005f34:	3000ad65 	.word	0x3000ad65
30005f38:	3000ac85 	.word	0x3000ac85
30005f3c:	3000740d 	.word	0x3000740d
30005f40:	00009b2d 	.word	0x00009b2d
30005f44:	23020000 	.word	0x23020000
30005f48:	40080000 	.word	0x40080000
30005f4c:	0000b479 	.word	0x0000b479
30005f50:	40040000 	.word	0x40040000
30005f54:	30009265 	.word	0x30009265
30005f58:	3000df3d 	.word	0x3000df3d
30005f5c:	0000aab9 	.word	0x0000aab9
30005f60:	4200c000 	.word	0x4200c000
30005f64:	2001c00c 	.word	0x2001c00c
30005f68:	30005c51 	.word	0x30005c51
30005f6c:	00033000 	.word	0x00033000
30005f70:	2001c01c 	.word	0x2001c01c
30005f74:	3000df40 	.word	0x3000df40
30005f78:	e000ed00 	.word	0xe000ed00
30005f7c:	00009be5 	.word	0x00009be5
30005f80:	3000bcf8 	.word	0x3000bcf8
30005f84:	3000c680 	.word	0x3000c680
30005f88:	3000aeb5 	.word	0x3000aeb5
30005f8c:	3000ae25 	.word	0x3000ae25
30005f90:	3000c5c8 	.word	0x3000c5c8
30005f94:	3000c5f4 	.word	0x3000c5f4
30005f98:	0000c0f9 	.word	0x0000c0f9
30005f9c:	0000aea1 	.word	0x0000aea1
30005fa0:	0000aaa5 	.word	0x0000aaa5
30005fa4:	3000c624 	.word	0x3000c624
30005fa8:	3000c6c0 	.word	0x3000c6c0
30005fac:	3000df3c 	.word	0x3000df3c
30005fb0:	00009979 	.word	0x00009979
30005fb4:	3000c6d4 	.word	0x3000c6d4
30005fb8:	0000c169 	.word	0x0000c169
30005fbc:	3000c6f0 	.word	0x3000c6f0
30005fc0:	3000c708 	.word	0x3000c708
30005fc4:	0016e360 	.word	0x0016e360
30005fc8:	f7ff fb7c 	bl	300056c4 <BOOT_Enable_KM0>
30005fcc:	2200      	movs	r2, #0
30005fce:	2101      	movs	r1, #1
30005fd0:	484e      	ldr	r0, [pc, #312]	; (3000610c <BOOT_Image1+0x4bc>)
30005fd2:	4b4f      	ldr	r3, [pc, #316]	; (30006110 <BOOT_Image1+0x4c0>)
30005fd4:	4798      	blx	r3
30005fd6:	4b4f      	ldr	r3, [pc, #316]	; (30006114 <BOOT_Image1+0x4c4>)
30005fd8:	2008      	movs	r0, #8
30005fda:	4798      	blx	r3
30005fdc:	f001 f906 	bl	300071ec <BOOT_RAM_TZCfg>
30005fe0:	782b      	ldrb	r3, [r5, #0]
30005fe2:	2b01      	cmp	r3, #1
30005fe4:	f000 8081 	beq.w	300060ea <BOOT_Image1+0x49a>
30005fe8:	4b4b      	ldr	r3, [pc, #300]	; (30006118 <BOOT_Image1+0x4c8>)
30005fea:	781b      	ldrb	r3, [r3, #0]
30005fec:	2b00      	cmp	r3, #0
30005fee:	d168      	bne.n	300060c2 <BOOT_Image1+0x472>
30005ff0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005ff4:	4949      	ldr	r1, [pc, #292]	; (3000611c <BOOT_Image1+0x4cc>)
30005ff6:	685a      	ldr	r2, [r3, #4]
30005ff8:	f042 0202 	orr.w	r2, r2, #2
30005ffc:	605a      	str	r2, [r3, #4]
30005ffe:	685a      	ldr	r2, [r3, #4]
30006000:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006004:	605a      	str	r2, [r3, #4]
30006006:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006008:	f043 0303 	orr.w	r3, r3, #3
3000600c:	678b      	str	r3, [r1, #120]	; 0x78
3000600e:	782b      	ldrb	r3, [r5, #0]
30006010:	2b01      	cmp	r3, #1
30006012:	d072      	beq.n	300060fa <BOOT_Image1+0x4aa>
30006014:	4842      	ldr	r0, [pc, #264]	; (30006120 <BOOT_Image1+0x4d0>)
30006016:	4943      	ldr	r1, [pc, #268]	; (30006124 <BOOT_Image1+0x4d4>)
30006018:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
3000601c:	4a42      	ldr	r2, [pc, #264]	; (30006128 <BOOT_Image1+0x4d8>)
3000601e:	f043 0301 	orr.w	r3, r3, #1
30006022:	4d42      	ldr	r5, [pc, #264]	; (3000612c <BOOT_Image1+0x4dc>)
30006024:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
30006028:	688c      	ldr	r4, [r1, #8]
3000602a:	680b      	ldr	r3, [r1, #0]
3000602c:	4940      	ldr	r1, [pc, #256]	; (30006130 <BOOT_Image1+0x4e0>)
3000602e:	6063      	str	r3, [r4, #4]
30006030:	4b40      	ldr	r3, [pc, #256]	; (30006134 <BOOT_Image1+0x4e4>)
30006032:	6094      	str	r4, [r2, #8]
30006034:	61e9      	str	r1, [r5, #28]
30006036:	6a59      	ldr	r1, [r3, #36]	; 0x24
30006038:	483f      	ldr	r0, [pc, #252]	; (30006138 <BOOT_Image1+0x4e8>)
3000603a:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000603e:	6259      	str	r1, [r3, #36]	; 0x24
30006040:	6a51      	ldr	r1, [r2, #36]	; 0x24
30006042:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30006046:	6251      	str	r1, [r2, #36]	; 0x24
30006048:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000604c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30006050:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30006054:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30006058:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000605c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30006060:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30006064:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30006068:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000606c:	689b      	ldr	r3, [r3, #8]
3000606e:	6183      	str	r3, [r0, #24]
30006070:	f7fe fb8e 	bl	30004790 <BOOT_NVICBackup_HP>
30006074:	4b31      	ldr	r3, [pc, #196]	; (3000613c <BOOT_Image1+0x4ec>)
30006076:	f383 8888 	msr	MSP_NS, r3
3000607a:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
3000607e:	f383 8809 	msr	PSP, r3
30006082:	6860      	ldr	r0, [r4, #4]
30006084:	b002      	add	sp, #8
30006086:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000608a:	f7fd bf83 	b.w	30003f94 <BOOT_NsStart>
3000608e:	4b2c      	ldr	r3, [pc, #176]	; (30006140 <BOOT_Image1+0x4f0>)
30006090:	2004      	movs	r0, #4
30006092:	4a2c      	ldr	r2, [pc, #176]	; (30006144 <BOOT_Image1+0x4f4>)
30006094:	4619      	mov	r1, r3
30006096:	f005 fa5d 	bl	3000b554 <rtk_log_write>
3000609a:	4b2b      	ldr	r3, [pc, #172]	; (30006148 <BOOT_Image1+0x4f8>)
3000609c:	4798      	blx	r3
3000609e:	f7fe fa39 	bl	30004514 <BOOT_PSRAM_Init>
300060a2:	7830      	ldrb	r0, [r6, #0]
300060a4:	2800      	cmp	r0, #0
300060a6:	f47f aef4 	bne.w	30005e92 <BOOT_Image1+0x242>
300060aa:	4b28      	ldr	r3, [pc, #160]	; (3000614c <BOOT_Image1+0x4fc>)
300060ac:	4798      	blx	r3
300060ae:	2800      	cmp	r0, #0
300060b0:	f43f aeef 	beq.w	30005e92 <BOOT_Image1+0x242>
300060b4:	4b22      	ldr	r3, [pc, #136]	; (30006140 <BOOT_Image1+0x4f0>)
300060b6:	2003      	movs	r0, #3
300060b8:	4a25      	ldr	r2, [pc, #148]	; (30006150 <BOOT_Image1+0x500>)
300060ba:	4619      	mov	r1, r3
300060bc:	f005 fa4a 	bl	3000b554 <rtk_log_write>
300060c0:	e6e7      	b.n	30005e92 <BOOT_Image1+0x242>
300060c2:	f7ff fc2f 	bl	30005924 <BOOT_Enable_AP>
300060c6:	4a16      	ldr	r2, [pc, #88]	; (30006120 <BOOT_Image1+0x4d0>)
300060c8:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
300060cc:	f043 0306 	orr.w	r3, r3, #6
300060d0:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
300060d4:	e79b      	b.n	3000600e <BOOT_Image1+0x3be>
300060d6:	4b1f      	ldr	r3, [pc, #124]	; (30006154 <BOOT_Image1+0x504>)
300060d8:	4798      	blx	r3
300060da:	2800      	cmp	r0, #0
300060dc:	f47f aef4 	bne.w	30005ec8 <BOOT_Image1+0x278>
300060e0:	68a3      	ldr	r3, [r4, #8]
300060e2:	f043 0302 	orr.w	r3, r3, #2
300060e6:	60a3      	str	r3, [r4, #8]
300060e8:	e6ee      	b.n	30005ec8 <BOOT_Image1+0x278>
300060ea:	47b0      	blx	r6
300060ec:	2801      	cmp	r0, #1
300060ee:	4b1a      	ldr	r3, [pc, #104]	; (30006158 <BOOT_Image1+0x508>)
300060f0:	bf0c      	ite	eq
300060f2:	2019      	moveq	r0, #25
300060f4:	2004      	movne	r0, #4
300060f6:	4798      	blx	r3
300060f8:	e776      	b.n	30005fe8 <BOOT_Image1+0x398>
300060fa:	47b0      	blx	r6
300060fc:	2801      	cmp	r0, #1
300060fe:	4b16      	ldr	r3, [pc, #88]	; (30006158 <BOOT_Image1+0x508>)
30006100:	bf0c      	ite	eq
30006102:	20e6      	moveq	r0, #230	; 0xe6
30006104:	2019      	movne	r0, #25
30006106:	4798      	blx	r3
30006108:	e784      	b.n	30006014 <BOOT_Image1+0x3c4>
3000610a:	bf00      	nop
3000610c:	4200c000 	.word	0x4200c000
30006110:	0000ac6d 	.word	0x0000ac6d
30006114:	0000e1d9 	.word	0x0000e1d9
30006118:	3000df3f 	.word	0x3000df3f
3000611c:	41000200 	.word	0x41000200
30006120:	42008200 	.word	0x42008200
30006124:	60000020 	.word	0x60000020
30006128:	e002ed00 	.word	0xe002ed00
3000612c:	30000000 	.word	0x30000000
30006130:	30003a81 	.word	0x30003a81
30006134:	e000ed00 	.word	0xe000ed00
30006138:	3000f3dc 	.word	0x3000f3dc
3000613c:	2001bffc 	.word	0x2001bffc
30006140:	3000bcf8 	.word	0x3000bcf8
30006144:	3000c654 	.word	0x3000c654
30006148:	3000828d 	.word	0x3000828d
3000614c:	3000ad85 	.word	0x3000ad85
30006150:	3000c668 	.word	0x3000c668
30006154:	0000c179 	.word	0x0000c179
30006158:	00009be5 	.word	0x00009be5

3000615c <BOOT_LoadSubImage>:
3000615c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006160:	b08f      	sub	sp, #60	; 0x3c
30006162:	469a      	mov	sl, r3
30006164:	460f      	mov	r7, r1
30006166:	f89d 3060 	ldrb.w	r3, [sp, #96]	; 0x60
3000616a:	9305      	str	r3, [sp, #20]
3000616c:	b39a      	cbz	r2, 300061d6 <BOOT_LoadSubImage+0x7a>
3000616e:	4691      	mov	r9, r2
30006170:	2600      	movs	r6, #0
30006172:	4605      	mov	r5, r0
30006174:	2220      	movs	r2, #32
30006176:	4639      	mov	r1, r7
30006178:	a806      	add	r0, sp, #24
3000617a:	f7fe fb67 	bl	3000484c <BOOT_ImgCopy>
3000617e:	9b06      	ldr	r3, [sp, #24]
30006180:	4a49      	ldr	r2, [pc, #292]	; (300062a8 <BOOT_LoadSubImage+0x14c>)
30006182:	4293      	cmp	r3, r2
30006184:	d157      	bne.n	30006236 <BOOT_LoadSubImage+0xda>
30006186:	f102 427b 	add.w	r2, r2, #4211081216	; 0xfb000000
3000618a:	9b07      	ldr	r3, [sp, #28]
3000618c:	f502 0278 	add.w	r2, r2, #16252928	; 0xf80000
30006190:	f502 62c0 	add.w	r2, r2, #1536	; 0x600
30006194:	4293      	cmp	r3, r2
30006196:	d14e      	bne.n	30006236 <BOOT_LoadSubImage+0xda>
30006198:	4b44      	ldr	r3, [pc, #272]	; (300062ac <BOOT_LoadSubImage+0x150>)
3000619a:	e9dd 4008 	ldrd	r4, r0, [sp, #32]
3000619e:	18c3      	adds	r3, r0, r3
300061a0:	f1a0 0820 	sub.w	r8, r0, #32
300061a4:	3420      	adds	r4, #32
300061a6:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300061aa:	d21a      	bcs.n	300061e2 <BOOT_LoadSubImage+0x86>
300061ac:	b17d      	cbz	r5, 300061ce <BOOT_LoadSubImage+0x72>
300061ae:	1d2b      	adds	r3, r5, #4
300061b0:	f845 8036 	str.w	r8, [r5, r6, lsl #3]
300061b4:	4a3e      	ldr	r2, [pc, #248]	; (300062b0 <BOOT_LoadSubImage+0x154>)
300061b6:	2004      	movs	r0, #4
300061b8:	e9cd 8401 	strd	r8, r4, [sp, #4]
300061bc:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
300061c0:	f85a 3026 	ldr.w	r3, [sl, r6, lsl #2]
300061c4:	9300      	str	r3, [sp, #0]
300061c6:	4b3b      	ldr	r3, [pc, #236]	; (300062b4 <BOOT_LoadSubImage+0x158>)
300061c8:	4619      	mov	r1, r3
300061ca:	f005 f9c3 	bl	3000b554 <rtk_log_write>
300061ce:	3601      	adds	r6, #1
300061d0:	4427      	add	r7, r4
300061d2:	454e      	cmp	r6, r9
300061d4:	d1ce      	bne.n	30006174 <BOOT_LoadSubImage+0x18>
300061d6:	4b38      	ldr	r3, [pc, #224]	; (300062b8 <BOOT_LoadSubImage+0x15c>)
300061d8:	2001      	movs	r0, #1
300061da:	601f      	str	r7, [r3, #0]
300061dc:	b00f      	add	sp, #60	; 0x3c
300061de:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300061e2:	4639      	mov	r1, r7
300061e4:	4622      	mov	r2, r4
300061e6:	4640      	mov	r0, r8
300061e8:	f7fe fb30 	bl	3000484c <BOOT_ImgCopy>
300061ec:	4933      	ldr	r1, [pc, #204]	; (300062bc <BOOT_LoadSubImage+0x160>)
300061ee:	694b      	ldr	r3, [r1, #20]
300061f0:	03db      	lsls	r3, r3, #15
300061f2:	d5db      	bpl.n	300061ac <BOOT_LoadSubImage+0x50>
300061f4:	ea08 0304 	and.w	r3, r8, r4
300061f8:	3301      	adds	r3, #1
300061fa:	d023      	beq.n	30006244 <BOOT_LoadSubImage+0xe8>
300061fc:	f018 031f 	ands.w	r3, r8, #31
30006200:	d042      	beq.n	30006288 <BOOT_LoadSubImage+0x12c>
30006202:	4423      	add	r3, r4
30006204:	f028 021f 	bic.w	r2, r8, #31
30006208:	f3bf 8f4f 	dsb	sy
3000620c:	2b00      	cmp	r3, #0
3000620e:	dd0d      	ble.n	3000622c <BOOT_LoadSubImage+0xd0>
30006210:	1e59      	subs	r1, r3, #1
30006212:	f102 0320 	add.w	r3, r2, #32
30006216:	4829      	ldr	r0, [pc, #164]	; (300062bc <BOOT_LoadSubImage+0x160>)
30006218:	f021 011f 	bic.w	r1, r1, #31
3000621c:	4419      	add	r1, r3
3000621e:	428b      	cmp	r3, r1
30006220:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30006224:	461a      	mov	r2, r3
30006226:	f103 0320 	add.w	r3, r3, #32
3000622a:	d1f8      	bne.n	3000621e <BOOT_LoadSubImage+0xc2>
3000622c:	f3bf 8f4f 	dsb	sy
30006230:	f3bf 8f6f 	isb	sy
30006234:	e7ba      	b.n	300061ac <BOOT_LoadSubImage+0x50>
30006236:	9b05      	ldr	r3, [sp, #20]
30006238:	2b01      	cmp	r3, #1
3000623a:	d028      	beq.n	3000628e <BOOT_LoadSubImage+0x132>
3000623c:	2000      	movs	r0, #0
3000623e:	b00f      	add	sp, #60	; 0x3c
30006240:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006244:	2300      	movs	r3, #0
30006246:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000624a:	f3bf 8f4f 	dsb	sy
3000624e:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30006252:	f643 7be0 	movw	fp, #16352	; 0x3fe0
30006256:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
3000625a:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000625e:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30006262:	ea0c 000b 	and.w	r0, ip, fp
30006266:	4673      	mov	r3, lr
30006268:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
3000626c:	3b01      	subs	r3, #1
3000626e:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30006272:	d2f9      	bcs.n	30006268 <BOOT_LoadSubImage+0x10c>
30006274:	f1ac 0c20 	sub.w	ip, ip, #32
30006278:	f11c 0f20 	cmn.w	ip, #32
3000627c:	d1f1      	bne.n	30006262 <BOOT_LoadSubImage+0x106>
3000627e:	f3bf 8f4f 	dsb	sy
30006282:	f3bf 8f6f 	isb	sy
30006286:	e791      	b.n	300061ac <BOOT_LoadSubImage+0x50>
30006288:	4623      	mov	r3, r4
3000628a:	4642      	mov	r2, r8
3000628c:	e7bc      	b.n	30006208 <BOOT_LoadSubImage+0xac>
3000628e:	f85a 4026 	ldr.w	r4, [sl, r6, lsl #2]
30006292:	2004      	movs	r0, #4
30006294:	4b07      	ldr	r3, [pc, #28]	; (300062b4 <BOOT_LoadSubImage+0x158>)
30006296:	4a0a      	ldr	r2, [pc, #40]	; (300062c0 <BOOT_LoadSubImage+0x164>)
30006298:	4619      	mov	r1, r3
3000629a:	9400      	str	r4, [sp, #0]
3000629c:	f005 f95a 	bl	3000b554 <rtk_log_write>
300062a0:	2000      	movs	r0, #0
300062a2:	b00f      	add	sp, #60	; 0x3c
300062a4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300062a8:	35393138 	.word	0x35393138
300062ac:	f7ffffe0 	.word	0xf7ffffe0
300062b0:	3000c7d0 	.word	0x3000c7d0
300062b4:	3000bcf8 	.word	0x3000bcf8
300062b8:	3000e50c 	.word	0x3000e50c
300062bc:	e000ed00 	.word	0xe000ed00
300062c0:	3000c7bc 	.word	0x3000c7bc

300062c4 <BOOT_LoadAPSubImage>:
300062c4:	4bb5      	ldr	r3, [pc, #724]	; (3000659c <BOOT_LoadAPSubImage+0x2d8>)
300062c6:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300062ca:	460c      	mov	r4, r1
300062cc:	4606      	mov	r6, r0
300062ce:	b08f      	sub	sp, #60	; 0x3c
300062d0:	4615      	mov	r5, r2
300062d2:	2220      	movs	r2, #32
300062d4:	e893 0003 	ldmia.w	r3, {r0, r1}
300062d8:	0c0b      	lsrs	r3, r1, #16
300062da:	9004      	str	r0, [sp, #16]
300062dc:	f8ad 1014 	strh.w	r1, [sp, #20]
300062e0:	a806      	add	r0, sp, #24
300062e2:	4621      	mov	r1, r4
300062e4:	f88d 3016 	strb.w	r3, [sp, #22]
300062e8:	f7fe fab0 	bl	3000484c <BOOT_ImgCopy>
300062ec:	4bac      	ldr	r3, [pc, #688]	; (300065a0 <BOOT_LoadAPSubImage+0x2dc>)
300062ee:	9a06      	ldr	r2, [sp, #24]
300062f0:	429a      	cmp	r2, r3
300062f2:	d108      	bne.n	30006306 <BOOT_LoadAPSubImage+0x42>
300062f4:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
300062f8:	9a07      	ldr	r2, [sp, #28]
300062fa:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
300062fe:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30006302:	429a      	cmp	r2, r3
30006304:	d006      	beq.n	30006314 <BOOT_LoadAPSubImage+0x50>
30006306:	2d01      	cmp	r5, #1
30006308:	f000 80b3 	beq.w	30006472 <BOOT_LoadAPSubImage+0x1ae>
3000630c:	2000      	movs	r0, #0
3000630e:	b00f      	add	sp, #60	; 0x3c
30006310:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006314:	9a09      	ldr	r2, [sp, #36]	; 0x24
30006316:	4ba3      	ldr	r3, [pc, #652]	; (300065a4 <BOOT_LoadAPSubImage+0x2e0>)
30006318:	6ca7      	ldr	r7, [r4, #72]	; 0x48
3000631a:	f1a2 0820 	sub.w	r8, r2, #32
3000631e:	4413      	add	r3, r2
30006320:	37d0      	adds	r7, #208	; 0xd0
30006322:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006326:	d247      	bcs.n	300063b8 <BOOT_LoadAPSubImage+0xf4>
30006328:	4b9f      	ldr	r3, [pc, #636]	; (300065a8 <BOOT_LoadAPSubImage+0x2e4>)
3000632a:	eb07 0508 	add.w	r5, r7, r8
3000632e:	f8d4 8070 	ldr.w	r8, [r4, #112]	; 0x70
30006332:	4798      	blx	r3
30006334:	2801      	cmp	r0, #1
30006336:	4427      	add	r7, r4
30006338:	4b9c      	ldr	r3, [pc, #624]	; (300065ac <BOOT_LoadAPSubImage+0x2e8>)
3000633a:	d029      	beq.n	30006390 <BOOT_LoadAPSubImage+0xcc>
3000633c:	4619      	mov	r1, r3
3000633e:	4a9c      	ldr	r2, [pc, #624]	; (300065b0 <BOOT_LoadAPSubImage+0x2ec>)
30006340:	2004      	movs	r0, #4
30006342:	f005 f907 	bl	3000b554 <rtk_log_write>
30006346:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
3000634a:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000634e:	f080 809c 	bcs.w	3000648a <BOOT_LoadAPSubImage+0x1c6>
30006352:	4445      	add	r5, r8
30006354:	eb08 0107 	add.w	r1, r8, r7
30006358:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
3000635c:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006360:	d256      	bcs.n	30006410 <BOOT_LoadAPSubImage+0x14c>
30006362:	2e00      	cmp	r6, #0
30006364:	f000 8081 	beq.w	3000646a <BOOT_LoadAPSubImage+0x1a6>
30006368:	4b90      	ldr	r3, [pc, #576]	; (300065ac <BOOT_LoadAPSubImage+0x2e8>)
3000636a:	aa04      	add	r2, sp, #16
3000636c:	2004      	movs	r0, #4
3000636e:	9200      	str	r2, [sp, #0]
30006370:	4a90      	ldr	r2, [pc, #576]	; (300065b4 <BOOT_LoadAPSubImage+0x2f0>)
30006372:	e9dd 1408 	ldrd	r1, r4, [sp, #32]
30006376:	3c20      	subs	r4, #32
30006378:	3120      	adds	r1, #32
3000637a:	e9c6 4100 	strd	r4, r1, [r6]
3000637e:	9102      	str	r1, [sp, #8]
30006380:	4619      	mov	r1, r3
30006382:	9401      	str	r4, [sp, #4]
30006384:	f005 f8e6 	bl	3000b554 <rtk_log_write>
30006388:	2001      	movs	r0, #1
3000638a:	b00f      	add	sp, #60	; 0x3c
3000638c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006390:	4a89      	ldr	r2, [pc, #548]	; (300065b8 <BOOT_LoadAPSubImage+0x2f4>)
30006392:	2004      	movs	r0, #4
30006394:	4619      	mov	r1, r3
30006396:	f005 f8dd 	bl	3000b554 <rtk_log_write>
3000639a:	4642      	mov	r2, r8
3000639c:	4629      	mov	r1, r5
3000639e:	4638      	mov	r0, r7
300063a0:	f000 fbe4 	bl	30006b6c <BOOT_LoadBL32RDPImg>
300063a4:	2800      	cmp	r0, #0
300063a6:	d1d4      	bne.n	30006352 <BOOT_LoadAPSubImage+0x8e>
300063a8:	4b80      	ldr	r3, [pc, #512]	; (300065ac <BOOT_LoadAPSubImage+0x2e8>)
300063aa:	2004      	movs	r0, #4
300063ac:	4a83      	ldr	r2, [pc, #524]	; (300065bc <BOOT_LoadAPSubImage+0x2f8>)
300063ae:	4619      	mov	r1, r3
300063b0:	f005 f8d0 	bl	3000b554 <rtk_log_write>
300063b4:	20ff      	movs	r0, #255	; 0xff
300063b6:	e7aa      	b.n	3000630e <BOOT_LoadAPSubImage+0x4a>
300063b8:	463a      	mov	r2, r7
300063ba:	4621      	mov	r1, r4
300063bc:	4640      	mov	r0, r8
300063be:	f7fe fa45 	bl	3000484c <BOOT_ImgCopy>
300063c2:	497f      	ldr	r1, [pc, #508]	; (300065c0 <BOOT_LoadAPSubImage+0x2fc>)
300063c4:	694b      	ldr	r3, [r1, #20]
300063c6:	03da      	lsls	r2, r3, #15
300063c8:	d5ae      	bpl.n	30006328 <BOOT_LoadAPSubImage+0x64>
300063ca:	ea07 0308 	and.w	r3, r7, r8
300063ce:	3301      	adds	r3, #1
300063d0:	f000 8086 	beq.w	300064e0 <BOOT_LoadAPSubImage+0x21c>
300063d4:	f018 031f 	ands.w	r3, r8, #31
300063d8:	f000 80bb 	beq.w	30006552 <BOOT_LoadAPSubImage+0x28e>
300063dc:	443b      	add	r3, r7
300063de:	f028 021f 	bic.w	r2, r8, #31
300063e2:	f3bf 8f4f 	dsb	sy
300063e6:	2b00      	cmp	r3, #0
300063e8:	dd0d      	ble.n	30006406 <BOOT_LoadAPSubImage+0x142>
300063ea:	1e59      	subs	r1, r3, #1
300063ec:	f102 0320 	add.w	r3, r2, #32
300063f0:	4873      	ldr	r0, [pc, #460]	; (300065c0 <BOOT_LoadAPSubImage+0x2fc>)
300063f2:	f021 011f 	bic.w	r1, r1, #31
300063f6:	4419      	add	r1, r3
300063f8:	428b      	cmp	r3, r1
300063fa:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300063fe:	461a      	mov	r2, r3
30006400:	f103 0320 	add.w	r3, r3, #32
30006404:	d1f8      	bne.n	300063f8 <BOOT_LoadAPSubImage+0x134>
30006406:	f3bf 8f4f 	dsb	sy
3000640a:	f3bf 8f6f 	isb	sy
3000640e:	e78b      	b.n	30006328 <BOOT_LoadAPSubImage+0x64>
30006410:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30006414:	4628      	mov	r0, r5
30006416:	4622      	mov	r2, r4
30006418:	f7fe fa18 	bl	3000484c <BOOT_ImgCopy>
3000641c:	4968      	ldr	r1, [pc, #416]	; (300065c0 <BOOT_LoadAPSubImage+0x2fc>)
3000641e:	694b      	ldr	r3, [r1, #20]
30006420:	03d8      	lsls	r0, r3, #15
30006422:	d59e      	bpl.n	30006362 <BOOT_LoadAPSubImage+0x9e>
30006424:	ea05 0304 	and.w	r3, r5, r4
30006428:	3301      	adds	r3, #1
3000642a:	d075      	beq.n	30006518 <BOOT_LoadAPSubImage+0x254>
3000642c:	f015 031f 	ands.w	r3, r5, #31
30006430:	d002      	beq.n	30006438 <BOOT_LoadAPSubImage+0x174>
30006432:	f025 051f 	bic.w	r5, r5, #31
30006436:	441c      	add	r4, r3
30006438:	f3bf 8f4f 	dsb	sy
3000643c:	2c00      	cmp	r4, #0
3000643e:	dd0d      	ble.n	3000645c <BOOT_LoadAPSubImage+0x198>
30006440:	1e62      	subs	r2, r4, #1
30006442:	f105 0320 	add.w	r3, r5, #32
30006446:	495e      	ldr	r1, [pc, #376]	; (300065c0 <BOOT_LoadAPSubImage+0x2fc>)
30006448:	f022 021f 	bic.w	r2, r2, #31
3000644c:	441a      	add	r2, r3
3000644e:	4293      	cmp	r3, r2
30006450:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30006454:	461d      	mov	r5, r3
30006456:	f103 0320 	add.w	r3, r3, #32
3000645a:	d1f8      	bne.n	3000644e <BOOT_LoadAPSubImage+0x18a>
3000645c:	f3bf 8f4f 	dsb	sy
30006460:	f3bf 8f6f 	isb	sy
30006464:	2e00      	cmp	r6, #0
30006466:	f47f af7f 	bne.w	30006368 <BOOT_LoadAPSubImage+0xa4>
3000646a:	2001      	movs	r0, #1
3000646c:	b00f      	add	sp, #60	; 0x3c
3000646e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006472:	4b4e      	ldr	r3, [pc, #312]	; (300065ac <BOOT_LoadAPSubImage+0x2e8>)
30006474:	ac04      	add	r4, sp, #16
30006476:	4a53      	ldr	r2, [pc, #332]	; (300065c4 <BOOT_LoadAPSubImage+0x300>)
30006478:	2004      	movs	r0, #4
3000647a:	4619      	mov	r1, r3
3000647c:	9400      	str	r4, [sp, #0]
3000647e:	f005 f869 	bl	3000b554 <rtk_log_write>
30006482:	2000      	movs	r0, #0
30006484:	b00f      	add	sp, #60	; 0x3c
30006486:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000648a:	4639      	mov	r1, r7
3000648c:	4642      	mov	r2, r8
3000648e:	4628      	mov	r0, r5
30006490:	f7fe f9dc 	bl	3000484c <BOOT_ImgCopy>
30006494:	494a      	ldr	r1, [pc, #296]	; (300065c0 <BOOT_LoadAPSubImage+0x2fc>)
30006496:	694b      	ldr	r3, [r1, #20]
30006498:	03db      	lsls	r3, r3, #15
3000649a:	f57f af5a 	bpl.w	30006352 <BOOT_LoadAPSubImage+0x8e>
3000649e:	ea08 0305 	and.w	r3, r8, r5
300064a2:	3301      	adds	r3, #1
300064a4:	d058      	beq.n	30006558 <BOOT_LoadAPSubImage+0x294>
300064a6:	f015 031f 	ands.w	r3, r5, #31
300064aa:	d073      	beq.n	30006594 <BOOT_LoadAPSubImage+0x2d0>
300064ac:	4443      	add	r3, r8
300064ae:	f025 021f 	bic.w	r2, r5, #31
300064b2:	f3bf 8f4f 	dsb	sy
300064b6:	2b00      	cmp	r3, #0
300064b8:	dd0d      	ble.n	300064d6 <BOOT_LoadAPSubImage+0x212>
300064ba:	1e59      	subs	r1, r3, #1
300064bc:	f102 0320 	add.w	r3, r2, #32
300064c0:	483f      	ldr	r0, [pc, #252]	; (300065c0 <BOOT_LoadAPSubImage+0x2fc>)
300064c2:	f021 011f 	bic.w	r1, r1, #31
300064c6:	4419      	add	r1, r3
300064c8:	428b      	cmp	r3, r1
300064ca:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300064ce:	461a      	mov	r2, r3
300064d0:	f103 0320 	add.w	r3, r3, #32
300064d4:	d1f8      	bne.n	300064c8 <BOOT_LoadAPSubImage+0x204>
300064d6:	f3bf 8f4f 	dsb	sy
300064da:	f3bf 8f6f 	isb	sy
300064de:	e738      	b.n	30006352 <BOOT_LoadAPSubImage+0x8e>
300064e0:	2300      	movs	r3, #0
300064e2:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300064e6:	f3bf 8f4f 	dsb	sy
300064ea:	f8d1 0080 	ldr.w	r0, [r1, #128]	; 0x80
300064ee:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300064f2:	f3c0 324e 	ubfx	r2, r0, #13, #15
300064f6:	f3c0 00c9 	ubfx	r0, r0, #3, #10
300064fa:	0152      	lsls	r2, r2, #5
300064fc:	ea02 0c0e 	and.w	ip, r2, lr
30006500:	4603      	mov	r3, r0
30006502:	ea4c 7583 	orr.w	r5, ip, r3, lsl #30
30006506:	3b01      	subs	r3, #1
30006508:	f8c1 5274 	str.w	r5, [r1, #628]	; 0x274
3000650c:	d2f9      	bcs.n	30006502 <BOOT_LoadAPSubImage+0x23e>
3000650e:	3a20      	subs	r2, #32
30006510:	f112 0f20 	cmn.w	r2, #32
30006514:	d1f2      	bne.n	300064fc <BOOT_LoadAPSubImage+0x238>
30006516:	e776      	b.n	30006406 <BOOT_LoadAPSubImage+0x142>
30006518:	2300      	movs	r3, #0
3000651a:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000651e:	f3bf 8f4f 	dsb	sy
30006522:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
30006526:	f643 77e0 	movw	r7, #16352	; 0x3fe0
3000652a:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000652e:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30006532:	0164      	lsls	r4, r4, #5
30006534:	ea04 0007 	and.w	r0, r4, r7
30006538:	462b      	mov	r3, r5
3000653a:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
3000653e:	3b01      	subs	r3, #1
30006540:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30006544:	1c5a      	adds	r2, r3, #1
30006546:	d1f8      	bne.n	3000653a <BOOT_LoadAPSubImage+0x276>
30006548:	3c20      	subs	r4, #32
3000654a:	f114 0f20 	cmn.w	r4, #32
3000654e:	d1f1      	bne.n	30006534 <BOOT_LoadAPSubImage+0x270>
30006550:	e784      	b.n	3000645c <BOOT_LoadAPSubImage+0x198>
30006552:	463b      	mov	r3, r7
30006554:	4642      	mov	r2, r8
30006556:	e744      	b.n	300063e2 <BOOT_LoadAPSubImage+0x11e>
30006558:	2300      	movs	r3, #0
3000655a:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000655e:	f3bf 8f4f 	dsb	sy
30006562:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30006566:	f643 79e0 	movw	r9, #16352	; 0x3fe0
3000656a:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
3000656e:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30006572:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30006576:	ea0c 0009 	and.w	r0, ip, r9
3000657a:	4673      	mov	r3, lr
3000657c:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30006580:	3b01      	subs	r3, #1
30006582:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30006586:	d2f9      	bcs.n	3000657c <BOOT_LoadAPSubImage+0x2b8>
30006588:	f1ac 0c20 	sub.w	ip, ip, #32
3000658c:	f11c 0f20 	cmn.w	ip, #32
30006590:	d1f1      	bne.n	30006576 <BOOT_LoadAPSubImage+0x2b2>
30006592:	e7a0      	b.n	300064d6 <BOOT_LoadAPSubImage+0x212>
30006594:	4643      	mov	r3, r8
30006596:	462a      	mov	r2, r5
30006598:	e78b      	b.n	300064b2 <BOOT_LoadAPSubImage+0x1ee>
3000659a:	bf00      	nop
3000659c:	3000c7b4 	.word	0x3000c7b4
300065a0:	35393138 	.word	0x35393138
300065a4:	f7ffffe0 	.word	0xf7ffffe0
300065a8:	0000c109 	.word	0x0000c109
300065ac:	3000bcf8 	.word	0x3000bcf8
300065b0:	3000c814 	.word	0x3000c814
300065b4:	3000c7d0 	.word	0x3000c7d0
300065b8:	3000c7e4 	.word	0x3000c7e4
300065bc:	3000c7f4 	.word	0x3000c7f4
300065c0:	e000ed00 	.word	0xe000ed00
300065c4:	3000c7bc 	.word	0x3000c7bc

300065c8 <BOOT_RSIPIvSet>:
300065c8:	b530      	push	{r4, r5, lr}
300065ca:	2300      	movs	r3, #0
300065cc:	b083      	sub	sp, #12
300065ce:	4a09      	ldr	r2, [pc, #36]	; (300065f4 <BOOT_RSIPIvSet+0x2c>)
300065d0:	4605      	mov	r5, r0
300065d2:	460c      	mov	r4, r1
300065d4:	9300      	str	r3, [sp, #0]
300065d6:	9301      	str	r3, [sp, #4]
300065d8:	4790      	blx	r2
300065da:	b148      	cbz	r0, 300065f0 <BOOT_RSIPIvSet+0x28>
300065dc:	f105 0120 	add.w	r1, r5, #32
300065e0:	4b05      	ldr	r3, [pc, #20]	; (300065f8 <BOOT_RSIPIvSet+0x30>)
300065e2:	2208      	movs	r2, #8
300065e4:	4668      	mov	r0, sp
300065e6:	4798      	blx	r3
300065e8:	4669      	mov	r1, sp
300065ea:	b2e0      	uxtb	r0, r4
300065ec:	4b03      	ldr	r3, [pc, #12]	; (300065fc <BOOT_RSIPIvSet+0x34>)
300065ee:	4798      	blx	r3
300065f0:	b003      	add	sp, #12
300065f2:	bd30      	pop	{r4, r5, pc}
300065f4:	0000c129 	.word	0x0000c129
300065f8:	00012a1d 	.word	0x00012a1d
300065fc:	30008f8d 	.word	0x30008f8d

30006600 <BOOT_OTFCheck>:
30006600:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30006604:	4d1c      	ldr	r5, [pc, #112]	; (30006678 <BOOT_OTFCheck+0x78>)
30006606:	b082      	sub	sp, #8
30006608:	4604      	mov	r4, r0
3000660a:	4688      	mov	r8, r1
3000660c:	4617      	mov	r7, r2
3000660e:	461e      	mov	r6, r3
30006610:	47a8      	blx	r5
30006612:	b148      	cbz	r0, 30006628 <BOOT_OTFCheck+0x28>
30006614:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30006618:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000661c:	d204      	bcs.n	30006628 <BOOT_OTFCheck+0x28>
3000661e:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30006622:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006626:	d302      	bcc.n	3000662e <BOOT_OTFCheck+0x2e>
30006628:	b002      	add	sp, #8
3000662a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000662e:	4b13      	ldr	r3, [pc, #76]	; (3000667c <BOOT_OTFCheck+0x7c>)
30006630:	2004      	movs	r0, #4
30006632:	4a13      	ldr	r2, [pc, #76]	; (30006680 <BOOT_OTFCheck+0x80>)
30006634:	4619      	mov	r1, r3
30006636:	f004 ff8d 	bl	3000b554 <rtk_log_write>
3000663a:	4b12      	ldr	r3, [pc, #72]	; (30006684 <BOOT_OTFCheck+0x84>)
3000663c:	4798      	blx	r3
3000663e:	4603      	mov	r3, r0
30006640:	b150      	cbz	r0, 30006658 <BOOT_OTFCheck+0x58>
30006642:	2801      	cmp	r0, #1
30006644:	d009      	beq.n	3000665a <BOOT_OTFCheck+0x5a>
30006646:	4b0d      	ldr	r3, [pc, #52]	; (3000667c <BOOT_OTFCheck+0x7c>)
30006648:	2002      	movs	r0, #2
3000664a:	4a0f      	ldr	r2, [pc, #60]	; (30006688 <BOOT_OTFCheck+0x88>)
3000664c:	4619      	mov	r1, r3
3000664e:	b002      	add	sp, #8
30006650:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30006654:	f004 bf7e 	b.w	3000b554 <rtk_log_write>
30006658:	2302      	movs	r3, #2
3000665a:	4630      	mov	r0, r6
3000665c:	4621      	mov	r1, r4
3000665e:	9301      	str	r3, [sp, #4]
30006660:	4642      	mov	r2, r8
30006662:	2301      	movs	r3, #1
30006664:	4c09      	ldr	r4, [pc, #36]	; (3000668c <BOOT_OTFCheck+0x8c>)
30006666:	9700      	str	r7, [sp, #0]
30006668:	47a0      	blx	r4
3000666a:	2001      	movs	r0, #1
3000666c:	4b08      	ldr	r3, [pc, #32]	; (30006690 <BOOT_OTFCheck+0x90>)
3000666e:	b002      	add	sp, #8
30006670:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30006674:	4718      	bx	r3
30006676:	bf00      	nop
30006678:	0000c129 	.word	0x0000c129
3000667c:	3000bcf8 	.word	0x3000bcf8
30006680:	3000c824 	.word	0x3000c824
30006684:	0000c245 	.word	0x0000c245
30006688:	3000c838 	.word	0x3000c838
3000668c:	30008fc1 	.word	0x30008fc1
30006690:	30008fa9 	.word	0x30008fa9

30006694 <BOOT_OTA_LoadIMG2>:
30006694:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006698:	4b6e      	ldr	r3, [pc, #440]	; (30006854 <BOOT_OTA_LoadIMG2+0x1c0>)
3000669a:	b08d      	sub	sp, #52	; 0x34
3000669c:	4a6e      	ldr	r2, [pc, #440]	; (30006858 <BOOT_OTA_LoadIMG2+0x1c4>)
3000669e:	f103 040c 	add.w	r4, r3, #12
300066a2:	9003      	str	r0, [sp, #12]
300066a4:	f8d2 8000 	ldr.w	r8, [r2]
300066a8:	ae06      	add	r6, sp, #24
300066aa:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
300066ae:	ab09      	add	r3, sp, #36	; 0x24
300066b0:	e886 0007 	stmia.w	r6, {r0, r1, r2}
300066b4:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
300066b8:	e883 0007 	stmia.w	r3, {r0, r1, r2}
300066bc:	4641      	mov	r1, r8
300066be:	4867      	ldr	r0, [pc, #412]	; (3000685c <BOOT_OTA_LoadIMG2+0x1c8>)
300066c0:	f44f 72b2 	mov.w	r2, #356	; 0x164
300066c4:	f7fe f8c2 	bl	3000484c <BOOT_ImgCopy>
300066c8:	4641      	mov	r1, r8
300066ca:	4864      	ldr	r0, [pc, #400]	; (3000685c <BOOT_OTA_LoadIMG2+0x1c8>)
300066cc:	f000 fd2a 	bl	30007124 <BOOT_CertificateCheck>
300066d0:	2800      	cmp	r0, #0
300066d2:	f040 80b6 	bne.w	30006842 <BOOT_OTA_LoadIMG2+0x1ae>
300066d6:	4604      	mov	r4, r0
300066d8:	f44f 72c8 	mov.w	r2, #400	; 0x190
300066dc:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
300066e0:	485f      	ldr	r0, [pc, #380]	; (30006860 <BOOT_OTA_LoadIMG2+0x1cc>)
300066e2:	f7fe f8b3 	bl	3000484c <BOOT_ImgCopy>
300066e6:	4b5f      	ldr	r3, [pc, #380]	; (30006864 <BOOT_OTA_LoadIMG2+0x1d0>)
300066e8:	e9cd 4404 	strd	r4, r4, [sp, #16]
300066ec:	4798      	blx	r3
300066ee:	2800      	cmp	r0, #0
300066f0:	f040 809d 	bne.w	3000682e <BOOT_OTA_LoadIMG2+0x19a>
300066f4:	f641 75f0 	movw	r5, #8176	; 0x1ff0
300066f8:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 300068a8 <BOOT_OTA_LoadIMG2+0x214>
300066fc:	4a5a      	ldr	r2, [pc, #360]	; (30006868 <BOOT_OTA_LoadIMG2+0x1d4>)
300066fe:	2000      	movs	r0, #0
30006700:	4445      	add	r5, r8
30006702:	4659      	mov	r1, fp
30006704:	4f59      	ldr	r7, [pc, #356]	; (3000686c <BOOT_OTA_LoadIMG2+0x1d8>)
30006706:	f04f 0801 	mov.w	r8, #1
3000670a:	462b      	mov	r3, r5
3000670c:	f8df 919c 	ldr.w	r9, [pc, #412]	; 300068ac <BOOT_OTA_LoadIMG2+0x218>
30006710:	47b8      	blx	r7
30006712:	2101      	movs	r1, #1
30006714:	2000      	movs	r0, #0
30006716:	4b56      	ldr	r3, [pc, #344]	; (30006870 <BOOT_OTA_LoadIMG2+0x1dc>)
30006718:	4798      	blx	r3
3000671a:	2301      	movs	r3, #1
3000671c:	4952      	ldr	r1, [pc, #328]	; (30006868 <BOOT_OTA_LoadIMG2+0x1d4>)
3000671e:	4658      	mov	r0, fp
30006720:	461a      	mov	r2, r3
30006722:	f7ff ff6d 	bl	30006600 <BOOT_OTFCheck>
30006726:	47c8      	blx	r9
30006728:	2800      	cmp	r0, #0
3000672a:	bf0c      	ite	eq
3000672c:	46aa      	moveq	sl, r5
3000672e:	46da      	movne	sl, fp
30006730:	4633      	mov	r3, r6
30006732:	2203      	movs	r2, #3
30006734:	4651      	mov	r1, sl
30006736:	484f      	ldr	r0, [pc, #316]	; (30006874 <BOOT_OTA_LoadIMG2+0x1e0>)
30006738:	f8cd 8000 	str.w	r8, [sp]
3000673c:	f7ff fd0e 	bl	3000615c <BOOT_LoadSubImage>
30006740:	2800      	cmp	r0, #0
30006742:	d06f      	beq.n	30006824 <BOOT_OTA_LoadIMG2+0x190>
30006744:	4e4b      	ldr	r6, [pc, #300]	; (30006874 <BOOT_OTA_LoadIMG2+0x1e0>)
30006746:	47c8      	blx	r9
30006748:	2800      	cmp	r0, #0
3000674a:	bf08      	it	eq
3000674c:	46ab      	moveq	fp, r5
3000674e:	4f47      	ldr	r7, [pc, #284]	; (3000686c <BOOT_OTA_LoadIMG2+0x1d8>)
30006750:	68f2      	ldr	r2, [r6, #12]
30006752:	4640      	mov	r0, r8
30006754:	6873      	ldr	r3, [r6, #4]
30006756:	f8c6 b000 	str.w	fp, [r6]
3000675a:	4413      	add	r3, r2
3000675c:	6972      	ldr	r2, [r6, #20]
3000675e:	f8df b150 	ldr.w	fp, [pc, #336]	; 300068b0 <BOOT_OTA_LoadIMG2+0x21c>
30006762:	441a      	add	r2, r3
30006764:	4b44      	ldr	r3, [pc, #272]	; (30006878 <BOOT_OTA_LoadIMG2+0x1e4>)
30006766:	eb0a 0102 	add.w	r1, sl, r2
3000676a:	61b1      	str	r1, [r6, #24]
3000676c:	1e51      	subs	r1, r2, #1
3000676e:	400b      	ands	r3, r1
30006770:	4659      	mov	r1, fp
30006772:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006776:	441d      	add	r5, r3
30006778:	1a9b      	subs	r3, r3, r2
3000677a:	4a40      	ldr	r2, [pc, #256]	; (3000687c <BOOT_OTA_LoadIMG2+0x1e8>)
3000677c:	61f3      	str	r3, [r6, #28]
3000677e:	462b      	mov	r3, r5
30006780:	47b8      	blx	r7
30006782:	4641      	mov	r1, r8
30006784:	4640      	mov	r0, r8
30006786:	4b3a      	ldr	r3, [pc, #232]	; (30006870 <BOOT_OTA_LoadIMG2+0x1dc>)
30006788:	4798      	blx	r3
3000678a:	4642      	mov	r2, r8
3000678c:	2302      	movs	r3, #2
3000678e:	493b      	ldr	r1, [pc, #236]	; (3000687c <BOOT_OTA_LoadIMG2+0x1e8>)
30006790:	4658      	mov	r0, fp
30006792:	f7ff ff35 	bl	30006600 <BOOT_OTFCheck>
30006796:	47c8      	blx	r9
30006798:	2800      	cmp	r0, #0
3000679a:	bf0c      	ite	eq
3000679c:	462f      	moveq	r7, r5
3000679e:	465f      	movne	r7, fp
300067a0:	ab09      	add	r3, sp, #36	; 0x24
300067a2:	2203      	movs	r2, #3
300067a4:	f106 0020 	add.w	r0, r6, #32
300067a8:	4639      	mov	r1, r7
300067aa:	f8cd 8000 	str.w	r8, [sp]
300067ae:	f7ff fcd5 	bl	3000615c <BOOT_LoadSubImage>
300067b2:	2800      	cmp	r0, #0
300067b4:	d036      	beq.n	30006824 <BOOT_OTA_LoadIMG2+0x190>
300067b6:	47c8      	blx	r9
300067b8:	6af2      	ldr	r2, [r6, #44]	; 0x2c
300067ba:	6a73      	ldr	r3, [r6, #36]	; 0x24
300067bc:	2800      	cmp	r0, #0
300067be:	bf08      	it	eq
300067c0:	46ab      	moveq	fp, r5
300067c2:	4827      	ldr	r0, [pc, #156]	; (30006860 <BOOT_OTA_LoadIMG2+0x1cc>)
300067c4:	4413      	add	r3, r2
300067c6:	6b72      	ldr	r2, [r6, #52]	; 0x34
300067c8:	f8c6 b020 	str.w	fp, [r6, #32]
300067cc:	4413      	add	r3, r2
300067ce:	4a2a      	ldr	r2, [pc, #168]	; (30006878 <BOOT_OTA_LoadIMG2+0x1e4>)
300067d0:	1e59      	subs	r1, r3, #1
300067d2:	441f      	add	r7, r3
300067d4:	400a      	ands	r2, r1
300067d6:	2100      	movs	r1, #0
300067d8:	63b7      	str	r7, [r6, #56]	; 0x38
300067da:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
300067de:	9100      	str	r1, [sp, #0]
300067e0:	1ad1      	subs	r1, r2, r3
300067e2:	2900      	cmp	r1, #0
300067e4:	63f1      	str	r1, [r6, #60]	; 0x3c
300067e6:	4631      	mov	r1, r6
300067e8:	bf18      	it	ne
300067ea:	4613      	movne	r3, r2
300067ec:	4a24      	ldr	r2, [pc, #144]	; (30006880 <BOOT_OTA_LoadIMG2+0x1ec>)
300067ee:	441d      	add	r5, r3
300067f0:	4b1a      	ldr	r3, [pc, #104]	; (3000685c <BOOT_OTA_LoadIMG2+0x1c8>)
300067f2:	6015      	str	r5, [r2, #0]
300067f4:	bf0c      	ite	eq
300067f6:	2207      	moveq	r2, #7
300067f8:	2208      	movne	r2, #8
300067fa:	f000 fb89 	bl	30006f10 <BOOT_SignatureCheck>
300067fe:	4b21      	ldr	r3, [pc, #132]	; (30006884 <BOOT_OTA_LoadIMG2+0x1f0>)
30006800:	4605      	mov	r5, r0
30006802:	689b      	ldr	r3, [r3, #8]
30006804:	079b      	lsls	r3, r3, #30
30006806:	d509      	bpl.n	3000681c <BOOT_OTA_LoadIMG2+0x188>
30006808:	4b1f      	ldr	r3, [pc, #124]	; (30006888 <BOOT_OTA_LoadIMG2+0x1f4>)
3000680a:	4a20      	ldr	r2, [pc, #128]	; (3000688c <BOOT_OTA_LoadIMG2+0x1f8>)
3000680c:	9903      	ldr	r1, [sp, #12]
3000680e:	2800      	cmp	r0, #0
30006810:	bf18      	it	ne
30006812:	461a      	movne	r2, r3
30006814:	481e      	ldr	r0, [pc, #120]	; (30006890 <BOOT_OTA_LoadIMG2+0x1fc>)
30006816:	462b      	mov	r3, r5
30006818:	f005 f982 	bl	3000bb20 <__DiagPrintf_veneer>
3000681c:	b11d      	cbz	r5, 30006826 <BOOT_OTA_LoadIMG2+0x192>
3000681e:	481d      	ldr	r0, [pc, #116]	; (30006894 <BOOT_OTA_LoadIMG2+0x200>)
30006820:	f005 f97e 	bl	3000bb20 <__DiagPrintf_veneer>
30006824:	24ff      	movs	r4, #255	; 0xff
30006826:	4620      	mov	r0, r4
30006828:	b00d      	add	sp, #52	; 0x34
3000682a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000682e:	2208      	movs	r2, #8
30006830:	4919      	ldr	r1, [pc, #100]	; (30006898 <BOOT_OTA_LoadIMG2+0x204>)
30006832:	4b1a      	ldr	r3, [pc, #104]	; (3000689c <BOOT_OTA_LoadIMG2+0x208>)
30006834:	a804      	add	r0, sp, #16
30006836:	4798      	blx	r3
30006838:	a904      	add	r1, sp, #16
3000683a:	2001      	movs	r0, #1
3000683c:	4b18      	ldr	r3, [pc, #96]	; (300068a0 <BOOT_OTA_LoadIMG2+0x20c>)
3000683e:	4798      	blx	r3
30006840:	e758      	b.n	300066f4 <BOOT_OTA_LoadIMG2+0x60>
30006842:	24ff      	movs	r4, #255	; 0xff
30006844:	4817      	ldr	r0, [pc, #92]	; (300068a4 <BOOT_OTA_LoadIMG2+0x210>)
30006846:	f005 f96b 	bl	3000bb20 <__DiagPrintf_veneer>
3000684a:	4620      	mov	r0, r4
3000684c:	b00d      	add	sp, #52	; 0x34
3000684e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006852:	bf00      	nop
30006854:	3000dc14 	.word	0x3000dc14
30006858:	3000f3d8 	.word	0x3000f3d8
3000685c:	3000f44c 	.word	0x3000f44c
30006860:	3000f5b0 	.word	0x3000f5b0
30006864:	0000c129 	.word	0x0000c129
30006868:	0cffffe0 	.word	0x0cffffe0
3000686c:	3000903d 	.word	0x3000903d
30006870:	3000907d 	.word	0x3000907d
30006874:	3000f758 	.word	0x3000f758
30006878:	fffff000 	.word	0xfffff000
3000687c:	0dffffe0 	.word	0x0dffffe0
30006880:	3000e50c 	.word	0x3000e50c
30006884:	2001c00c 	.word	0x2001c00c
30006888:	3000c858 	.word	0x3000c858
3000688c:	3000c850 	.word	0x3000c850
30006890:	3000c888 	.word	0x3000c888
30006894:	3000c8bc 	.word	0x3000c8bc
30006898:	3000f5d0 	.word	0x3000f5d0
3000689c:	00012a1d 	.word	0x00012a1d
300068a0:	30008f8d 	.word	0x30008f8d
300068a4:	3000c860 	.word	0x3000c860
300068a8:	0c000000 	.word	0x0c000000
300068ac:	3000921d 	.word	0x3000921d
300068b0:	0d000000 	.word	0x0d000000

300068b4 <BOOT_OTA_Region_Init>:
300068b4:	b510      	push	{r4, lr}
300068b6:	4c10      	ldr	r4, [pc, #64]	; (300068f8 <BOOT_OTA_Region_Init+0x44>)
300068b8:	2200      	movs	r2, #0
300068ba:	2002      	movs	r0, #2
300068bc:	4621      	mov	r1, r4
300068be:	f001 fa73 	bl	30007da8 <flash_get_layout_info>
300068c2:	2200      	movs	r2, #0
300068c4:	1d21      	adds	r1, r4, #4
300068c6:	2003      	movs	r0, #3
300068c8:	f001 fa6e 	bl	30007da8 <flash_get_layout_info>
300068cc:	2200      	movs	r2, #0
300068ce:	f104 0110 	add.w	r1, r4, #16
300068d2:	2004      	movs	r0, #4
300068d4:	f001 fa68 	bl	30007da8 <flash_get_layout_info>
300068d8:	2200      	movs	r2, #0
300068da:	f104 0114 	add.w	r1, r4, #20
300068de:	2005      	movs	r0, #5
300068e0:	f001 fa62 	bl	30007da8 <flash_get_layout_info>
300068e4:	e9d4 2300 	ldrd	r2, r3, [r4]
300068e8:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
300068ec:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
300068f0:	e9c4 2302 	strd	r2, r3, [r4, #8]
300068f4:	bd10      	pop	{r4, pc}
300068f6:	bf00      	nop
300068f8:	3000f740 	.word	0x3000f740

300068fc <BOOT_OTA_IMG2>:
300068fc:	f7ff beca 	b.w	30006694 <BOOT_OTA_LoadIMG2>

30006900 <BOOT_OTA_AP>:
30006900:	b5f0      	push	{r4, r5, r6, r7, lr}
30006902:	4a32      	ldr	r2, [pc, #200]	; (300069cc <BOOT_OTA_AP+0xcc>)
30006904:	b0eb      	sub	sp, #428	; 0x1ac
30006906:	4b32      	ldr	r3, [pc, #200]	; (300069d0 <BOOT_OTA_AP+0xd0>)
30006908:	6815      	ldr	r5, [r2, #0]
3000690a:	ac02      	add	r4, sp, #8
3000690c:	4e31      	ldr	r6, [pc, #196]	; (300069d4 <BOOT_OTA_AP+0xd4>)
3000690e:	f505 5780 	add.w	r7, r5, #4096	; 0x1000
30006912:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006914:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006918:	4629      	mov	r1, r5
3000691a:	4d2f      	ldr	r5, [pc, #188]	; (300069d8 <BOOT_OTA_AP+0xd8>)
3000691c:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006920:	a806      	add	r0, sp, #24
30006922:	f7fd ff93 	bl	3000484c <BOOT_ImgCopy>
30006926:	4a2d      	ldr	r2, [pc, #180]	; (300069dc <BOOT_OTA_AP+0xdc>)
30006928:	463b      	mov	r3, r7
3000692a:	4629      	mov	r1, r5
3000692c:	2003      	movs	r0, #3
3000692e:	47b0      	blx	r6
30006930:	2101      	movs	r1, #1
30006932:	4b2b      	ldr	r3, [pc, #172]	; (300069e0 <BOOT_OTA_AP+0xe0>)
30006934:	2003      	movs	r0, #3
30006936:	4798      	blx	r3
30006938:	2201      	movs	r2, #1
3000693a:	4928      	ldr	r1, [pc, #160]	; (300069dc <BOOT_OTA_AP+0xdc>)
3000693c:	2303      	movs	r3, #3
3000693e:	4628      	mov	r0, r5
30006940:	f7ff fe5e 	bl	30006600 <BOOT_OTFCheck>
30006944:	4b27      	ldr	r3, [pc, #156]	; (300069e4 <BOOT_OTA_AP+0xe4>)
30006946:	4798      	blx	r3
30006948:	4623      	mov	r3, r4
3000694a:	2201      	movs	r2, #1
3000694c:	4c26      	ldr	r4, [pc, #152]	; (300069e8 <BOOT_OTA_AP+0xe8>)
3000694e:	2800      	cmp	r0, #0
30006950:	bf14      	ite	ne
30006952:	4629      	movne	r1, r5
30006954:	4639      	moveq	r1, r7
30006956:	9200      	str	r2, [sp, #0]
30006958:	4620      	mov	r0, r4
3000695a:	2204      	movs	r2, #4
3000695c:	f7ff fbfe 	bl	3000615c <BOOT_LoadSubImage>
30006960:	2801      	cmp	r0, #1
30006962:	d122      	bne.n	300069aa <BOOT_OTA_AP+0xaa>
30006964:	2302      	movs	r3, #2
30006966:	4605      	mov	r5, r0
30006968:	2204      	movs	r2, #4
3000696a:	a806      	add	r0, sp, #24
3000696c:	9300      	str	r3, [sp, #0]
3000696e:	4621      	mov	r1, r4
30006970:	4b1e      	ldr	r3, [pc, #120]	; (300069ec <BOOT_OTA_AP+0xec>)
30006972:	f000 facd 	bl	30006f10 <BOOT_SignatureCheck>
30006976:	4606      	mov	r6, r0
30006978:	bb18      	cbnz	r0, 300069c2 <BOOT_OTA_AP+0xc2>
3000697a:	68e3      	ldr	r3, [r4, #12]
3000697c:	462a      	mov	r2, r5
3000697e:	6861      	ldr	r1, [r4, #4]
30006980:	f104 0018 	add.w	r0, r4, #24
30006984:	4419      	add	r1, r3
30006986:	6963      	ldr	r3, [r4, #20]
30006988:	4419      	add	r1, r3
3000698a:	4439      	add	r1, r7
3000698c:	f7ff fc9a 	bl	300062c4 <BOOT_LoadAPSubImage>
30006990:	2801      	cmp	r0, #1
30006992:	d102      	bne.n	3000699a <BOOT_OTA_AP+0x9a>
30006994:	4630      	mov	r0, r6
30006996:	b06b      	add	sp, #428	; 0x1ac
30006998:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000699a:	4b15      	ldr	r3, [pc, #84]	; (300069f0 <BOOT_OTA_AP+0xf0>)
3000699c:	681b      	ldr	r3, [r3, #0]
3000699e:	079b      	lsls	r3, r3, #30
300069a0:	d40a      	bmi.n	300069b8 <BOOT_OTA_AP+0xb8>
300069a2:	26ff      	movs	r6, #255	; 0xff
300069a4:	4630      	mov	r0, r6
300069a6:	b06b      	add	sp, #428	; 0x1ac
300069a8:	bdf0      	pop	{r4, r5, r6, r7, pc}
300069aa:	26ff      	movs	r6, #255	; 0xff
300069ac:	4811      	ldr	r0, [pc, #68]	; (300069f4 <BOOT_OTA_AP+0xf4>)
300069ae:	f005 f8b7 	bl	3000bb20 <__DiagPrintf_veneer>
300069b2:	4630      	mov	r0, r6
300069b4:	b06b      	add	sp, #428	; 0x1ac
300069b6:	bdf0      	pop	{r4, r5, r6, r7, pc}
300069b8:	480f      	ldr	r0, [pc, #60]	; (300069f8 <BOOT_OTA_AP+0xf8>)
300069ba:	26ff      	movs	r6, #255	; 0xff
300069bc:	f005 f8b0 	bl	3000bb20 <__DiagPrintf_veneer>
300069c0:	e7e8      	b.n	30006994 <BOOT_OTA_AP+0x94>
300069c2:	480e      	ldr	r0, [pc, #56]	; (300069fc <BOOT_OTA_AP+0xfc>)
300069c4:	26ff      	movs	r6, #255	; 0xff
300069c6:	f005 f8ab 	bl	3000bb20 <__DiagPrintf_veneer>
300069ca:	e7e3      	b.n	30006994 <BOOT_OTA_AP+0x94>
300069cc:	3000e50c 	.word	0x3000e50c
300069d0:	3000dc2c 	.word	0x3000dc2c
300069d4:	3000903d 	.word	0x3000903d
300069d8:	0e000000 	.word	0x0e000000
300069dc:	0fffffe0 	.word	0x0fffffe0
300069e0:	3000907d 	.word	0x3000907d
300069e4:	3000921d 	.word	0x3000921d
300069e8:	3000f758 	.word	0x3000f758
300069ec:	3000f44c 	.word	0x3000f44c
300069f0:	2001c00c 	.word	0x2001c00c
300069f4:	3000c904 	.word	0x3000c904
300069f8:	3000c928 	.word	0x3000c928
300069fc:	3000c8e0 	.word	0x3000c8e0

30006a00 <BOOT_LoadRDPImg>:
30006a00:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006a04:	4d4e      	ldr	r5, [pc, #312]	; (30006b40 <BOOT_LoadRDPImg+0x140>)
30006a06:	b0d9      	sub	sp, #356	; 0x164
30006a08:	4613      	mov	r3, r2
30006a0a:	468b      	mov	fp, r1
30006a0c:	4604      	mov	r4, r0
30006a0e:	aa06      	add	r2, sp, #24
30006a10:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30006a14:	e895 0003 	ldmia.w	r5, {r0, r1}
30006a18:	2500      	movs	r5, #0
30006a1a:	e882 0003 	stmia.w	r2, {r0, r1}
30006a1e:	4619      	mov	r1, r3
30006a20:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006a24:	4620      	mov	r0, r4
30006a26:	f7fd ff11 	bl	3000484c <BOOT_ImgCopy>
30006a2a:	ab06      	add	r3, sp, #24
30006a2c:	2202      	movs	r2, #2
30006a2e:	4641      	mov	r1, r8
30006a30:	4658      	mov	r0, fp
30006a32:	9500      	str	r5, [sp, #0]
30006a34:	f7ff fb92 	bl	3000615c <BOOT_LoadSubImage>
30006a38:	2801      	cmp	r0, #1
30006a3a:	d06f      	beq.n	30006b1c <BOOT_LoadRDPImg+0x11c>
30006a3c:	4b41      	ldr	r3, [pc, #260]	; (30006b44 <BOOT_LoadRDPImg+0x144>)
30006a3e:	4628      	mov	r0, r5
30006a40:	4798      	blx	r3
30006a42:	2101      	movs	r1, #1
30006a44:	4b40      	ldr	r3, [pc, #256]	; (30006b48 <BOOT_LoadRDPImg+0x148>)
30006a46:	4628      	mov	r0, r5
30006a48:	4798      	blx	r3
30006a4a:	4b40      	ldr	r3, [pc, #256]	; (30006b4c <BOOT_LoadRDPImg+0x14c>)
30006a4c:	2120      	movs	r1, #32
30006a4e:	a810      	add	r0, sp, #64	; 0x40
30006a50:	4798      	blx	r3
30006a52:	f8df a110 	ldr.w	sl, [pc, #272]	; 30006b64 <BOOT_LoadRDPImg+0x164>
30006a56:	f104 0120 	add.w	r1, r4, #32
30006a5a:	2210      	movs	r2, #16
30006a5c:	a80c      	add	r0, sp, #48	; 0x30
30006a5e:	af08      	add	r7, sp, #32
30006a60:	47d0      	blx	sl
30006a62:	f8df 9104 	ldr.w	r9, [pc, #260]	; 30006b68 <BOOT_LoadRDPImg+0x168>
30006a66:	ab06      	add	r3, sp, #24
30006a68:	9504      	str	r5, [sp, #16]
30006a6a:	9305      	str	r3, [sp, #20]
30006a6c:	4641      	mov	r1, r8
30006a6e:	a818      	add	r0, sp, #96	; 0x60
30006a70:	2210      	movs	r2, #16
30006a72:	f7fd feeb 	bl	3000484c <BOOT_ImgCopy>
30006a76:	2310      	movs	r3, #16
30006a78:	aa0c      	add	r2, sp, #48	; 0x30
30006a7a:	a818      	add	r0, sp, #96	; 0x60
30006a7c:	4619      	mov	r1, r3
30006a7e:	9700      	str	r7, [sp, #0]
30006a80:	47c8      	blx	r9
30006a82:	683b      	ldr	r3, [r7, #0]
30006a84:	4a32      	ldr	r2, [pc, #200]	; (30006b50 <BOOT_LoadRDPImg+0x150>)
30006a86:	4293      	cmp	r3, r2
30006a88:	d14c      	bne.n	30006b24 <BOOT_LoadRDPImg+0x124>
30006a8a:	9a05      	ldr	r2, [sp, #20]
30006a8c:	687b      	ldr	r3, [r7, #4]
30006a8e:	f852 1b04 	ldr.w	r1, [r2], #4
30006a92:	9205      	str	r2, [sp, #20]
30006a94:	4a2f      	ldr	r2, [pc, #188]	; (30006b54 <BOOT_LoadRDPImg+0x154>)
30006a96:	4293      	cmp	r3, r2
30006a98:	d148      	bne.n	30006b2c <BOOT_LoadRDPImg+0x12c>
30006a9a:	9100      	str	r1, [sp, #0]
30006a9c:	2004      	movs	r0, #4
30006a9e:	68fe      	ldr	r6, [r7, #12]
30006aa0:	4b2d      	ldr	r3, [pc, #180]	; (30006b58 <BOOT_LoadRDPImg+0x158>)
30006aa2:	3e20      	subs	r6, #32
30006aa4:	4a2d      	ldr	r2, [pc, #180]	; (30006b5c <BOOT_LoadRDPImg+0x15c>)
30006aa6:	4619      	mov	r1, r3
30006aa8:	9601      	str	r6, [sp, #4]
30006aaa:	68bd      	ldr	r5, [r7, #8]
30006aac:	3520      	adds	r5, #32
30006aae:	9502      	str	r5, [sp, #8]
30006ab0:	f004 fd50 	bl	3000b554 <rtk_log_write>
30006ab4:	f1bb 0f00 	cmp.w	fp, #0
30006ab8:	d006      	beq.n	30006ac8 <BOOT_LoadRDPImg+0xc8>
30006aba:	9a04      	ldr	r2, [sp, #16]
30006abc:	f10b 0304 	add.w	r3, fp, #4
30006ac0:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
30006ac4:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
30006ac8:	b1dd      	cbz	r5, 30006b02 <BOOT_LoadRDPImg+0x102>
30006aca:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006ace:	462c      	mov	r4, r5
30006ad0:	4641      	mov	r1, r8
30006ad2:	a818      	add	r0, sp, #96	; 0x60
30006ad4:	bf28      	it	cs
30006ad6:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006ada:	4622      	mov	r2, r4
30006adc:	44a0      	add	r8, r4
30006ade:	f7fd feb5 	bl	3000484c <BOOT_ImgCopy>
30006ae2:	aa0c      	add	r2, sp, #48	; 0x30
30006ae4:	4621      	mov	r1, r4
30006ae6:	2310      	movs	r3, #16
30006ae8:	9600      	str	r6, [sp, #0]
30006aea:	a818      	add	r0, sp, #96	; 0x60
30006aec:	47c8      	blx	r9
30006aee:	ab18      	add	r3, sp, #96	; 0x60
30006af0:	f1a4 0110 	sub.w	r1, r4, #16
30006af4:	2210      	movs	r2, #16
30006af6:	a80c      	add	r0, sp, #48	; 0x30
30006af8:	4426      	add	r6, r4
30006afa:	4419      	add	r1, r3
30006afc:	47d0      	blx	sl
30006afe:	1b2d      	subs	r5, r5, r4
30006b00:	d1e3      	bne.n	30006aca <BOOT_LoadRDPImg+0xca>
30006b02:	9b04      	ldr	r3, [sp, #16]
30006b04:	b13b      	cbz	r3, 30006b16 <BOOT_LoadRDPImg+0x116>
30006b06:	2100      	movs	r1, #0
30006b08:	4b0f      	ldr	r3, [pc, #60]	; (30006b48 <BOOT_LoadRDPImg+0x148>)
30006b0a:	4608      	mov	r0, r1
30006b0c:	4798      	blx	r3
30006b0e:	2002      	movs	r0, #2
30006b10:	b059      	add	sp, #356	; 0x164
30006b12:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006b16:	2301      	movs	r3, #1
30006b18:	9304      	str	r3, [sp, #16]
30006b1a:	e7a7      	b.n	30006a6c <BOOT_LoadRDPImg+0x6c>
30006b1c:	2002      	movs	r0, #2
30006b1e:	b059      	add	sp, #356	; 0x164
30006b20:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006b24:	9b04      	ldr	r3, [sp, #16]
30006b26:	aa06      	add	r2, sp, #24
30006b28:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30006b2c:	4b0a      	ldr	r3, [pc, #40]	; (30006b58 <BOOT_LoadRDPImg+0x158>)
30006b2e:	2004      	movs	r0, #4
30006b30:	9100      	str	r1, [sp, #0]
30006b32:	4a0b      	ldr	r2, [pc, #44]	; (30006b60 <BOOT_LoadRDPImg+0x160>)
30006b34:	4619      	mov	r1, r3
30006b36:	f004 fd0d 	bl	3000b554 <rtk_log_write>
30006b3a:	2000      	movs	r0, #0
30006b3c:	e7e8      	b.n	30006b10 <BOOT_LoadRDPImg+0x110>
30006b3e:	bf00      	nop
30006b40:	3000dc3c 	.word	0x3000dc3c
30006b44:	00002429 	.word	0x00002429
30006b48:	00002479 	.word	0x00002479
30006b4c:	00004641 	.word	0x00004641
30006b50:	35393138 	.word	0x35393138
30006b54:	31313738 	.word	0x31313738
30006b58:	3000bcf8 	.word	0x3000bcf8
30006b5c:	3000c7d0 	.word	0x3000c7d0
30006b60:	3000c7bc 	.word	0x3000c7bc
30006b64:	00012a1d 	.word	0x00012a1d
30006b68:	000046f5 	.word	0x000046f5

30006b6c <BOOT_LoadBL32RDPImg>:
30006b6c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006b70:	4b35      	ldr	r3, [pc, #212]	; (30006c48 <BOOT_LoadBL32RDPImg+0xdc>)
30006b72:	b0d1      	sub	sp, #324	; 0x144
30006b74:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 30006c64 <BOOT_LoadBL32RDPImg+0xf8>
30006b78:	4607      	mov	r7, r0
30006b7a:	f10d 0810 	add.w	r8, sp, #16
30006b7e:	460e      	mov	r6, r1
30006b80:	4615      	mov	r5, r2
30006b82:	ac02      	add	r4, sp, #8
30006b84:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30006c68 <BOOT_LoadBL32RDPImg+0xfc>
30006b88:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 30006c6c <BOOT_LoadBL32RDPImg+0x100>
30006b8c:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006b8e:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
30006b92:	e89c 0003 	ldmia.w	ip, {r0, r1}
30006b96:	e884 0003 	stmia.w	r4, {r0, r1}
30006b9a:	2000      	movs	r0, #0
30006b9c:	47c8      	blx	r9
30006b9e:	2101      	movs	r1, #1
30006ba0:	2000      	movs	r0, #0
30006ba2:	47d0      	blx	sl
30006ba4:	2120      	movs	r1, #32
30006ba6:	4b29      	ldr	r3, [pc, #164]	; (30006c4c <BOOT_LoadBL32RDPImg+0xe0>)
30006ba8:	eb0d 0001 	add.w	r0, sp, r1
30006bac:	4798      	blx	r3
30006bae:	4b28      	ldr	r3, [pc, #160]	; (30006c50 <BOOT_LoadBL32RDPImg+0xe4>)
30006bb0:	689b      	ldr	r3, [r3, #8]
30006bb2:	079b      	lsls	r3, r3, #30
30006bb4:	d436      	bmi.n	30006c24 <BOOT_LoadBL32RDPImg+0xb8>
30006bb6:	b1fd      	cbz	r5, 30006bf8 <BOOT_LoadBL32RDPImg+0x8c>
30006bb8:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 30006c70 <BOOT_LoadBL32RDPImg+0x104>
30006bbc:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 30006c74 <BOOT_LoadBL32RDPImg+0x108>
30006bc0:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006bc4:	462c      	mov	r4, r5
30006bc6:	4639      	mov	r1, r7
30006bc8:	a810      	add	r0, sp, #64	; 0x40
30006bca:	bf28      	it	cs
30006bcc:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006bd0:	4622      	mov	r2, r4
30006bd2:	4427      	add	r7, r4
30006bd4:	f7fd fe3a 	bl	3000484c <BOOT_ImgCopy>
30006bd8:	4642      	mov	r2, r8
30006bda:	4621      	mov	r1, r4
30006bdc:	2310      	movs	r3, #16
30006bde:	9600      	str	r6, [sp, #0]
30006be0:	a810      	add	r0, sp, #64	; 0x40
30006be2:	47d8      	blx	fp
30006be4:	ab10      	add	r3, sp, #64	; 0x40
30006be6:	f1a4 0110 	sub.w	r1, r4, #16
30006bea:	2210      	movs	r2, #16
30006bec:	4640      	mov	r0, r8
30006bee:	4426      	add	r6, r4
30006bf0:	4419      	add	r1, r3
30006bf2:	47c8      	blx	r9
30006bf4:	1b2d      	subs	r5, r5, r4
30006bf6:	d1e3      	bne.n	30006bc0 <BOOT_LoadBL32RDPImg+0x54>
30006bf8:	f856 2c20 	ldr.w	r2, [r6, #-32]
30006bfc:	3e20      	subs	r6, #32
30006bfe:	4b15      	ldr	r3, [pc, #84]	; (30006c54 <BOOT_LoadBL32RDPImg+0xe8>)
30006c00:	429a      	cmp	r2, r3
30006c02:	d108      	bne.n	30006c16 <BOOT_LoadBL32RDPImg+0xaa>
30006c04:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30006c08:	6872      	ldr	r2, [r6, #4]
30006c0a:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30006c0e:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30006c12:	429a      	cmp	r2, r3
30006c14:	d00d      	beq.n	30006c32 <BOOT_LoadBL32RDPImg+0xc6>
30006c16:	4810      	ldr	r0, [pc, #64]	; (30006c58 <BOOT_LoadBL32RDPImg+0xec>)
30006c18:	f004 ff82 	bl	3000bb20 <__DiagPrintf_veneer>
30006c1c:	2000      	movs	r0, #0
30006c1e:	b051      	add	sp, #324	; 0x144
30006c20:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006c24:	4621      	mov	r1, r4
30006c26:	462b      	mov	r3, r5
30006c28:	4632      	mov	r2, r6
30006c2a:	480c      	ldr	r0, [pc, #48]	; (30006c5c <BOOT_LoadBL32RDPImg+0xf0>)
30006c2c:	f004 ff78 	bl	3000bb20 <__DiagPrintf_veneer>
30006c30:	e7c1      	b.n	30006bb6 <BOOT_LoadBL32RDPImg+0x4a>
30006c32:	480b      	ldr	r0, [pc, #44]	; (30006c60 <BOOT_LoadBL32RDPImg+0xf4>)
30006c34:	f004 ff74 	bl	3000bb20 <__DiagPrintf_veneer>
30006c38:	2100      	movs	r1, #0
30006c3a:	4608      	mov	r0, r1
30006c3c:	47d0      	blx	sl
30006c3e:	2001      	movs	r0, #1
30006c40:	b051      	add	sp, #324	; 0x144
30006c42:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006c46:	bf00      	nop
30006c48:	3000c9d0 	.word	0x3000c9d0
30006c4c:	00004641 	.word	0x00004641
30006c50:	2001c00c 	.word	0x2001c00c
30006c54:	35393138 	.word	0x35393138
30006c58:	3000c9b4 	.word	0x3000c9b4
30006c5c:	3000c98c 	.word	0x3000c98c
30006c60:	3000c9c0 	.word	0x3000c9c0
30006c64:	3000c9e4 	.word	0x3000c9e4
30006c68:	00002429 	.word	0x00002429
30006c6c:	00002479 	.word	0x00002479
30006c70:	000046f5 	.word	0x000046f5
30006c74:	00012a1d 	.word	0x00012a1d

30006c78 <_BOOT_Validate_ImgHash>:
30006c78:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006c7c:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
30006c80:	4605      	mov	r5, r0
30006c82:	4690      	mov	r8, r2
30006c84:	22e0      	movs	r2, #224	; 0xe0
30006c86:	9103      	str	r1, [sp, #12]
30006c88:	2100      	movs	r1, #0
30006c8a:	a818      	add	r0, sp, #96	; 0x60
30006c8c:	461c      	mov	r4, r3
30006c8e:	f88d 101f 	strb.w	r1, [sp, #31]
30006c92:	f004 fee5 	bl	3000ba60 <____wrap_memset_veneer>
30006c96:	4628      	mov	r0, r5
30006c98:	f10d 021f 	add.w	r2, sp, #31
30006c9c:	f10d 011e 	add.w	r1, sp, #30
30006ca0:	f002 faf2 	bl	30009288 <SBOOT_GetMdType>
30006ca4:	9001      	str	r0, [sp, #4]
30006ca6:	2800      	cmp	r0, #0
30006ca8:	d166      	bne.n	30006d78 <_BOOT_Validate_ImgHash+0x100>
30006caa:	f89d 301f 	ldrb.w	r3, [sp, #31]
30006cae:	2b00      	cmp	r3, #0
30006cb0:	d067      	beq.n	30006d82 <_BOOT_Validate_ImgHash+0x10a>
30006cb2:	2101      	movs	r1, #1
30006cb4:	4605      	mov	r5, r0
30006cb6:	4b80      	ldr	r3, [pc, #512]	; (30006eb8 <_BOOT_Validate_ImgHash+0x240>)
30006cb8:	4608      	mov	r0, r1
30006cba:	4798      	blx	r3
30006cbc:	4629      	mov	r1, r5
30006cbe:	ab18      	add	r3, sp, #96	; 0x60
30006cc0:	2220      	movs	r2, #32
30006cc2:	f89d 001e 	ldrb.w	r0, [sp, #30]
30006cc6:	4d7d      	ldr	r5, [pc, #500]	; (30006ebc <_BOOT_Validate_ImgHash+0x244>)
30006cc8:	47a8      	blx	r5
30006cca:	2800      	cmp	r0, #0
30006ccc:	f040 808b 	bne.w	30006de6 <_BOOT_Validate_ImgHash+0x16e>
30006cd0:	b32c      	cbz	r4, 30006d1e <_BOOT_Validate_ImgHash+0xa6>
30006cd2:	aad0      	add	r2, sp, #832	; 0x340
30006cd4:	1e63      	subs	r3, r4, #1
30006cd6:	ae50      	add	r6, sp, #320	; 0x140
30006cd8:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 30006ed4 <_BOOT_Validate_ImgHash+0x25c>
30006cdc:	f012 0218 	ands.w	r2, r2, #24
30006ce0:	b2db      	uxtb	r3, r3
30006ce2:	f026 091f 	bic.w	r9, r6, #31
30006ce6:	4d76      	ldr	r5, [pc, #472]	; (30006ec0 <_BOOT_Validate_ImgHash+0x248>)
30006ce8:	9204      	str	r2, [sp, #16]
30006cea:	f108 0208 	add.w	r2, r8, #8
30006cee:	bf08      	it	eq
30006cf0:	46b1      	moveq	r9, r6
30006cf2:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30006cf6:	9302      	str	r3, [sp, #8]
30006cf8:	e9d8 b700 	ldrd	fp, r7, [r8]
30006cfc:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30006d00:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30006d04:	f0c0 80a7 	bcc.w	30006e56 <_BOOT_Validate_ImgHash+0x1de>
30006d08:	4639      	mov	r1, r7
30006d0a:	4658      	mov	r0, fp
30006d0c:	aa18      	add	r2, sp, #96	; 0x60
30006d0e:	47d0      	blx	sl
30006d10:	2800      	cmp	r0, #0
30006d12:	d168      	bne.n	30006de6 <_BOOT_Validate_ImgHash+0x16e>
30006d14:	f108 0808 	add.w	r8, r8, #8
30006d18:	9b02      	ldr	r3, [sp, #8]
30006d1a:	4543      	cmp	r3, r8
30006d1c:	d1ec      	bne.n	30006cf8 <_BOOT_Validate_ImgHash+0x80>
30006d1e:	ac08      	add	r4, sp, #32
30006d20:	a918      	add	r1, sp, #96	; 0x60
30006d22:	4b68      	ldr	r3, [pc, #416]	; (30006ec4 <_BOOT_Validate_ImgHash+0x24c>)
30006d24:	4620      	mov	r0, r4
30006d26:	4798      	blx	r3
30006d28:	4601      	mov	r1, r0
30006d2a:	2800      	cmp	r0, #0
30006d2c:	d15b      	bne.n	30006de6 <_BOOT_Validate_ImgHash+0x16e>
30006d2e:	2001      	movs	r0, #1
30006d30:	4b61      	ldr	r3, [pc, #388]	; (30006eb8 <_BOOT_Validate_ImgHash+0x240>)
30006d32:	4798      	blx	r3
30006d34:	f89d 501e 	ldrb.w	r5, [sp, #30]
30006d38:	4b63      	ldr	r3, [pc, #396]	; (30006ec8 <_BOOT_Validate_ImgHash+0x250>)
30006d3a:	4798      	blx	r3
30006d3c:	fbb0 f3f5 	udiv	r3, r0, r5
30006d40:	fb03 0315 	mls	r3, r3, r5, r0
30006d44:	b2db      	uxtb	r3, r3
30006d46:	b1bd      	cbz	r5, 30006d78 <_BOOT_Validate_ImgHash+0x100>
30006d48:	2000      	movs	r0, #0
30006d4a:	9f03      	ldr	r7, [sp, #12]
30006d4c:	4606      	mov	r6, r0
30006d4e:	1c5a      	adds	r2, r3, #1
30006d50:	5cf9      	ldrb	r1, [r7, r3]
30006d52:	5ce3      	ldrb	r3, [r4, r3]
30006d54:	3001      	adds	r0, #1
30006d56:	b2d2      	uxtb	r2, r2
30006d58:	404b      	eors	r3, r1
30006d5a:	431e      	orrs	r6, r3
30006d5c:	fbb2 f3f5 	udiv	r3, r2, r5
30006d60:	fb05 2213 	mls	r2, r5, r3, r2
30006d64:	b2d3      	uxtb	r3, r2
30006d66:	b2c2      	uxtb	r2, r0
30006d68:	4295      	cmp	r5, r2
30006d6a:	d8f0      	bhi.n	30006d4e <_BOOT_Validate_ImgHash+0xd6>
30006d6c:	2e00      	cmp	r6, #0
30006d6e:	9b01      	ldr	r3, [sp, #4]
30006d70:	bf18      	it	ne
30006d72:	f06f 0306 	mvnne.w	r3, #6
30006d76:	9301      	str	r3, [sp, #4]
30006d78:	9801      	ldr	r0, [sp, #4]
30006d7a:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30006d7e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006d82:	a918      	add	r1, sp, #96	; 0x60
30006d84:	f89d 001e 	ldrb.w	r0, [sp, #30]
30006d88:	4b50      	ldr	r3, [pc, #320]	; (30006ecc <_BOOT_Validate_ImgHash+0x254>)
30006d8a:	4798      	blx	r3
30006d8c:	bb58      	cbnz	r0, 30006de6 <_BOOT_Validate_ImgHash+0x16e>
30006d8e:	b31c      	cbz	r4, 30006dd8 <_BOOT_Validate_ImgHash+0x160>
30006d90:	aad0      	add	r2, sp, #832	; 0x340
30006d92:	1e63      	subs	r3, r4, #1
30006d94:	ae50      	add	r6, sp, #320	; 0x140
30006d96:	f8df a140 	ldr.w	sl, [pc, #320]	; 30006ed8 <_BOOT_Validate_ImgHash+0x260>
30006d9a:	f012 0218 	ands.w	r2, r2, #24
30006d9e:	b2db      	uxtb	r3, r3
30006da0:	f026 091f 	bic.w	r9, r6, #31
30006da4:	4d46      	ldr	r5, [pc, #280]	; (30006ec0 <_BOOT_Validate_ImgHash+0x248>)
30006da6:	9204      	str	r2, [sp, #16]
30006da8:	f108 0208 	add.w	r2, r8, #8
30006dac:	bf08      	it	eq
30006dae:	46b1      	moveq	r9, r6
30006db0:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30006db4:	9302      	str	r3, [sp, #8]
30006db6:	e9d8 b700 	ldrd	fp, r7, [r8]
30006dba:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30006dbe:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30006dc2:	d318      	bcc.n	30006df6 <_BOOT_Validate_ImgHash+0x17e>
30006dc4:	4639      	mov	r1, r7
30006dc6:	4658      	mov	r0, fp
30006dc8:	aa18      	add	r2, sp, #96	; 0x60
30006dca:	47d0      	blx	sl
30006dcc:	b958      	cbnz	r0, 30006de6 <_BOOT_Validate_ImgHash+0x16e>
30006dce:	f108 0808 	add.w	r8, r8, #8
30006dd2:	9b02      	ldr	r3, [sp, #8]
30006dd4:	4543      	cmp	r3, r8
30006dd6:	d1ee      	bne.n	30006db6 <_BOOT_Validate_ImgHash+0x13e>
30006dd8:	ac08      	add	r4, sp, #32
30006dda:	a918      	add	r1, sp, #96	; 0x60
30006ddc:	4b3c      	ldr	r3, [pc, #240]	; (30006ed0 <_BOOT_Validate_ImgHash+0x258>)
30006dde:	4620      	mov	r0, r4
30006de0:	4798      	blx	r3
30006de2:	2800      	cmp	r0, #0
30006de4:	d0a6      	beq.n	30006d34 <_BOOT_Validate_ImgHash+0xbc>
30006de6:	f06f 030a 	mvn.w	r3, #10
30006dea:	9301      	str	r3, [sp, #4]
30006dec:	9801      	ldr	r0, [sp, #4]
30006dee:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30006df2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006df6:	2f00      	cmp	r7, #0
30006df8:	d0e9      	beq.n	30006dce <_BOOT_Validate_ImgHash+0x156>
30006dfa:	9c04      	ldr	r4, [sp, #16]
30006dfc:	f8cd 8014 	str.w	r8, [sp, #20]
30006e00:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30006e04:	46b8      	mov	r8, r7
30006e06:	4659      	mov	r1, fp
30006e08:	4630      	mov	r0, r6
30006e0a:	bf28      	it	cs
30006e0c:	f44f 7800 	movcs.w	r8, #512	; 0x200
30006e10:	4642      	mov	r2, r8
30006e12:	f7fd fd1b 	bl	3000484c <BOOT_ImgCopy>
30006e16:	696a      	ldr	r2, [r5, #20]
30006e18:	03d3      	lsls	r3, r2, #15
30006e1a:	d50f      	bpl.n	30006e3c <_BOOT_Validate_ImgHash+0x1c4>
30006e1c:	eb08 0004 	add.w	r0, r8, r4
30006e20:	f3bf 8f4f 	dsb	sy
30006e24:	4448      	add	r0, r9
30006e26:	464a      	mov	r2, r9
30006e28:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30006e2c:	3220      	adds	r2, #32
30006e2e:	1a83      	subs	r3, r0, r2
30006e30:	2b00      	cmp	r3, #0
30006e32:	dcf9      	bgt.n	30006e28 <_BOOT_Validate_ImgHash+0x1b0>
30006e34:	f3bf 8f4f 	dsb	sy
30006e38:	f3bf 8f6f 	isb	sy
30006e3c:	aa18      	add	r2, sp, #96	; 0x60
30006e3e:	4641      	mov	r1, r8
30006e40:	4630      	mov	r0, r6
30006e42:	47d0      	blx	sl
30006e44:	2800      	cmp	r0, #0
30006e46:	d1ce      	bne.n	30006de6 <_BOOT_Validate_ImgHash+0x16e>
30006e48:	ebb7 0708 	subs.w	r7, r7, r8
30006e4c:	44c3      	add	fp, r8
30006e4e:	d1d7      	bne.n	30006e00 <_BOOT_Validate_ImgHash+0x188>
30006e50:	f8dd 8014 	ldr.w	r8, [sp, #20]
30006e54:	e7bb      	b.n	30006dce <_BOOT_Validate_ImgHash+0x156>
30006e56:	2f00      	cmp	r7, #0
30006e58:	f43f af5c 	beq.w	30006d14 <_BOOT_Validate_ImgHash+0x9c>
30006e5c:	9c04      	ldr	r4, [sp, #16]
30006e5e:	f8cd 8014 	str.w	r8, [sp, #20]
30006e62:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30006e66:	46b8      	mov	r8, r7
30006e68:	4659      	mov	r1, fp
30006e6a:	4630      	mov	r0, r6
30006e6c:	bf28      	it	cs
30006e6e:	f44f 7800 	movcs.w	r8, #512	; 0x200
30006e72:	4642      	mov	r2, r8
30006e74:	f7fd fcea 	bl	3000484c <BOOT_ImgCopy>
30006e78:	696a      	ldr	r2, [r5, #20]
30006e7a:	03d2      	lsls	r2, r2, #15
30006e7c:	d50f      	bpl.n	30006e9e <_BOOT_Validate_ImgHash+0x226>
30006e7e:	eb08 0004 	add.w	r0, r8, r4
30006e82:	f3bf 8f4f 	dsb	sy
30006e86:	4448      	add	r0, r9
30006e88:	464a      	mov	r2, r9
30006e8a:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30006e8e:	3220      	adds	r2, #32
30006e90:	1a83      	subs	r3, r0, r2
30006e92:	2b00      	cmp	r3, #0
30006e94:	dcf9      	bgt.n	30006e8a <_BOOT_Validate_ImgHash+0x212>
30006e96:	f3bf 8f4f 	dsb	sy
30006e9a:	f3bf 8f6f 	isb	sy
30006e9e:	aa18      	add	r2, sp, #96	; 0x60
30006ea0:	4641      	mov	r1, r8
30006ea2:	4630      	mov	r0, r6
30006ea4:	47d0      	blx	sl
30006ea6:	2800      	cmp	r0, #0
30006ea8:	d19d      	bne.n	30006de6 <_BOOT_Validate_ImgHash+0x16e>
30006eaa:	ebb7 0708 	subs.w	r7, r7, r8
30006eae:	44c3      	add	fp, r8
30006eb0:	d1d7      	bne.n	30006e62 <_BOOT_Validate_ImgHash+0x1ea>
30006eb2:	f8dd 8014 	ldr.w	r8, [sp, #20]
30006eb6:	e72d      	b.n	30006d14 <_BOOT_Validate_ImgHash+0x9c>
30006eb8:	300092c5 	.word	0x300092c5
30006ebc:	30008c05 	.word	0x30008c05
30006ec0:	e000ed00 	.word	0xe000ed00
30006ec4:	30009341 	.word	0x30009341
30006ec8:	0000ea4d 	.word	0x0000ea4d
30006ecc:	30009381 	.word	0x30009381
30006ed0:	300093fd 	.word	0x300093fd
30006ed4:	30009305 	.word	0x30009305
30006ed8:	300093c1 	.word	0x300093c1

30006edc <BOOT_Validate_ImgHash>:
30006edc:	b530      	push	{r4, r5, lr}
30006ede:	b085      	sub	sp, #20
30006ee0:	4d0a      	ldr	r5, [pc, #40]	; (30006f0c <BOOT_Validate_ImgHash+0x30>)
30006ee2:	4604      	mov	r4, r0
30006ee4:	9301      	str	r3, [sp, #4]
30006ee6:	e9cd 2102 	strd	r2, r1, [sp, #8]
30006eea:	47a8      	blx	r5
30006eec:	9903      	ldr	r1, [sp, #12]
30006eee:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30006ef2:	b128      	cbz	r0, 30006f00 <BOOT_Validate_ImgHash+0x24>
30006ef4:	4620      	mov	r0, r4
30006ef6:	b005      	add	sp, #20
30006ef8:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30006efc:	f002 bae2 	b.w	300094c4 <SBOOT_Validate_ImgHash>
30006f00:	4620      	mov	r0, r4
30006f02:	b005      	add	sp, #20
30006f04:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30006f08:	f7ff beb6 	b.w	30006c78 <_BOOT_Validate_ImgHash>
30006f0c:	3000921d 	.word	0x3000921d

30006f10 <BOOT_SignatureCheck>:
30006f10:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30006f14:	461e      	mov	r6, r3
30006f16:	b089      	sub	sp, #36	; 0x24
30006f18:	4b79      	ldr	r3, [pc, #484]	; (30007100 <BOOT_SignatureCheck+0x1f0>)
30006f1a:	4680      	mov	r8, r0
30006f1c:	4689      	mov	r9, r1
30006f1e:	4617      	mov	r7, r2
30006f20:	f10d 0c20 	add.w	ip, sp, #32
30006f24:	4c77      	ldr	r4, [pc, #476]	; (30007104 <BOOT_SignatureCheck+0x1f4>)
30006f26:	9d10      	ldr	r5, [sp, #64]	; 0x40
30006f28:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006f2a:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30006f2e:	47a0      	blx	r4
30006f30:	b910      	cbnz	r0, 30006f38 <BOOT_SignatureCheck+0x28>
30006f32:	b009      	add	sp, #36	; 0x24
30006f34:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006f38:	4b73      	ldr	r3, [pc, #460]	; (30007108 <BOOT_SignatureCheck+0x1f8>)
30006f3a:	2000      	movs	r0, #0
30006f3c:	4798      	blx	r3
30006f3e:	f898 3013 	ldrb.w	r3, [r8, #19]
30006f42:	f898 2012 	ldrb.w	r2, [r8, #18]
30006f46:	f10d 010f 	add.w	r1, sp, #15
30006f4a:	f10d 000e 	add.w	r0, sp, #14
30006f4e:	f002 faf3 	bl	30009538 <SBOOT_Validate_Algorithm>
30006f52:	4604      	mov	r4, r0
30006f54:	b9e8      	cbnz	r0, 30006f92 <BOOT_SignatureCheck+0x82>
30006f56:	69f1      	ldr	r1, [r6, #28]
30006f58:	b919      	cbnz	r1, 30006f62 <BOOT_SignatureCheck+0x52>
30006f5a:	e0cf      	b.n	300070fc <BOOT_SignatureCheck+0x1ec>
30006f5c:	b2d0      	uxtb	r0, r2
30006f5e:	4288      	cmp	r0, r1
30006f60:	d208      	bcs.n	30006f74 <BOOT_SignatureCheck+0x64>
30006f62:	eb00 04c0 	add.w	r4, r0, r0, lsl #3
30006f66:	1c42      	adds	r2, r0, #1
30006f68:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30006f6c:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30006f70:	42ab      	cmp	r3, r5
30006f72:	d1f3      	bne.n	30006f5c <BOOT_SignatureCheck+0x4c>
30006f74:	2224      	movs	r2, #36	; 0x24
30006f76:	23b4      	movs	r3, #180	; 0xb4
30006f78:	fb12 3200 	smlabb	r2, r2, r0, r3
30006f7c:	f108 0570 	add.w	r5, r8, #112	; 0x70
30006f80:	4432      	add	r2, r6
30006f82:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006f86:	4629      	mov	r1, r5
30006f88:	f002 fa58 	bl	3000943c <SBOOT_Validate_PubKey>
30006f8c:	4604      	mov	r4, r0
30006f8e:	2800      	cmp	r0, #0
30006f90:	d07a      	beq.n	30007088 <BOOT_SignatureCheck+0x178>
30006f92:	f898 3011 	ldrb.w	r3, [r8, #17]
30006f96:	aa08      	add	r2, sp, #32
30006f98:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30006f9c:	f853 1c10 	ldr.w	r1, [r3, #-16]
30006fa0:	4b5a      	ldr	r3, [pc, #360]	; (3000710c <BOOT_SignatureCheck+0x1fc>)
30006fa2:	2002      	movs	r0, #2
30006fa4:	4a5a      	ldr	r2, [pc, #360]	; (30007110 <BOOT_SignatureCheck+0x200>)
30006fa6:	e9cd 1400 	strd	r1, r4, [sp]
30006faa:	4619      	mov	r1, r3
30006fac:	f004 fad2 	bl	3000b554 <rtk_log_write>
30006fb0:	2f00      	cmp	r7, #0
30006fb2:	d045      	beq.n	30007040 <BOOT_SignatureCheck+0x130>
30006fb4:	464d      	mov	r5, r9
30006fb6:	f107 39ff 	add.w	r9, r7, #4294967295	; 0xffffffff
30006fba:	f04f 0800 	mov.w	r8, #0
30006fbe:	4f55      	ldr	r7, [pc, #340]	; (30007114 <BOOT_SignatureCheck+0x204>)
30006fc0:	fa5f f389 	uxtb.w	r3, r9
30006fc4:	f105 0908 	add.w	r9, r5, #8
30006fc8:	1d2e      	adds	r6, r5, #4
30006fca:	4c53      	ldr	r4, [pc, #332]	; (30007118 <BOOT_SignatureCheck+0x208>)
30006fcc:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30006fd0:	e003      	b.n	30006fda <BOOT_SignatureCheck+0xca>
30006fd2:	3508      	adds	r5, #8
30006fd4:	3608      	adds	r6, #8
30006fd6:	45a9      	cmp	r9, r5
30006fd8:	d032      	beq.n	30007040 <BOOT_SignatureCheck+0x130>
30006fda:	f856 0c04 	ldr.w	r0, [r6, #-4]
30006fde:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30006fe2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006fe6:	d3f4      	bcc.n	30006fd2 <BOOT_SignatureCheck+0xc2>
30006fe8:	2100      	movs	r1, #0
30006fea:	686a      	ldr	r2, [r5, #4]
30006fec:	47b8      	blx	r7
30006fee:	6963      	ldr	r3, [r4, #20]
30006ff0:	03d9      	lsls	r1, r3, #15
30006ff2:	d5ee      	bpl.n	30006fd2 <BOOT_SignatureCheck+0xc2>
30006ff4:	f856 2c04 	ldr.w	r2, [r6, #-4]
30006ff8:	686b      	ldr	r3, [r5, #4]
30006ffa:	ea02 0103 	and.w	r1, r2, r3
30006ffe:	3101      	adds	r1, #1
30007000:	d022      	beq.n	30007048 <BOOT_SignatureCheck+0x138>
30007002:	f012 011f 	ands.w	r1, r2, #31
30007006:	d002      	beq.n	3000700e <BOOT_SignatureCheck+0xfe>
30007008:	440b      	add	r3, r1
3000700a:	f022 021f 	bic.w	r2, r2, #31
3000700e:	f3bf 8f4f 	dsb	sy
30007012:	2b00      	cmp	r3, #0
30007014:	dd0c      	ble.n	30007030 <BOOT_SignatureCheck+0x120>
30007016:	1e59      	subs	r1, r3, #1
30007018:	f102 0320 	add.w	r3, r2, #32
3000701c:	f021 011f 	bic.w	r1, r1, #31
30007020:	4419      	add	r1, r3
30007022:	428b      	cmp	r3, r1
30007024:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30007028:	461a      	mov	r2, r3
3000702a:	f103 0320 	add.w	r3, r3, #32
3000702e:	d1f8      	bne.n	30007022 <BOOT_SignatureCheck+0x112>
30007030:	f3bf 8f4f 	dsb	sy
30007034:	f3bf 8f6f 	isb	sy
30007038:	3508      	adds	r5, #8
3000703a:	3608      	adds	r6, #8
3000703c:	45a9      	cmp	r9, r5
3000703e:	d1cc      	bne.n	30006fda <BOOT_SignatureCheck+0xca>
30007040:	20ff      	movs	r0, #255	; 0xff
30007042:	b009      	add	sp, #36	; 0x24
30007044:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007048:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
3000704c:	f3bf 8f4f 	dsb	sy
30007050:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30007054:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30007058:	f3c3 304e 	ubfx	r0, r3, #13, #15
3000705c:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30007060:	0140      	lsls	r0, r0, #5
30007062:	ea00 010e 	and.w	r1, r0, lr
30007066:	4663      	mov	r3, ip
30007068:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000706c:	3b01      	subs	r3, #1
3000706e:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30007072:	1c5a      	adds	r2, r3, #1
30007074:	d1f8      	bne.n	30007068 <BOOT_SignatureCheck+0x158>
30007076:	3820      	subs	r0, #32
30007078:	f110 0f20 	cmn.w	r0, #32
3000707c:	d1f1      	bne.n	30007062 <BOOT_SignatureCheck+0x152>
3000707e:	f3bf 8f4f 	dsb	sy
30007082:	f3bf 8f6f 	isb	sy
30007086:	e7d7      	b.n	30007038 <BOOT_SignatureCheck+0x128>
30007088:	f508 7380 	add.w	r3, r8, #256	; 0x100
3000708c:	462a      	mov	r2, r5
3000708e:	f89d 100f 	ldrb.w	r1, [sp, #15]
30007092:	9301      	str	r3, [sp, #4]
30007094:	f44f 7380 	mov.w	r3, #256	; 0x100
30007098:	f89d 000e 	ldrb.w	r0, [sp, #14]
3000709c:	9300      	str	r3, [sp, #0]
3000709e:	4643      	mov	r3, r8
300070a0:	f002 f9ea 	bl	30009478 <SBOOT_Validate_Signature>
300070a4:	4604      	mov	r4, r0
300070a6:	2800      	cmp	r0, #0
300070a8:	f47f af73 	bne.w	30006f92 <BOOT_SignatureCheck+0x82>
300070ac:	f108 0530 	add.w	r5, r8, #48	; 0x30
300070b0:	4b1a      	ldr	r3, [pc, #104]	; (3000711c <BOOT_SignatureCheck+0x20c>)
300070b2:	f89d 400f 	ldrb.w	r4, [sp, #15]
300070b6:	4798      	blx	r3
300070b8:	4629      	mov	r1, r5
300070ba:	b9c0      	cbnz	r0, 300070ee <BOOT_SignatureCheck+0x1de>
300070bc:	4620      	mov	r0, r4
300070be:	463b      	mov	r3, r7
300070c0:	464a      	mov	r2, r9
300070c2:	f7ff fdd9 	bl	30006c78 <_BOOT_Validate_ImgHash>
300070c6:	4604      	mov	r4, r0
300070c8:	f898 3011 	ldrb.w	r3, [r8, #17]
300070cc:	aa08      	add	r2, sp, #32
300070ce:	eb02 0383 	add.w	r3, r2, r3, lsl #2
300070d2:	f853 1c10 	ldr.w	r1, [r3, #-16]
300070d6:	2c00      	cmp	r4, #0
300070d8:	f47f af62 	bne.w	30006fa0 <BOOT_SignatureCheck+0x90>
300070dc:	4b0b      	ldr	r3, [pc, #44]	; (3000710c <BOOT_SignatureCheck+0x1fc>)
300070de:	2004      	movs	r0, #4
300070e0:	9100      	str	r1, [sp, #0]
300070e2:	4a0f      	ldr	r2, [pc, #60]	; (30007120 <BOOT_SignatureCheck+0x210>)
300070e4:	4619      	mov	r1, r3
300070e6:	f004 fa35 	bl	3000b554 <rtk_log_write>
300070ea:	4620      	mov	r0, r4
300070ec:	e721      	b.n	30006f32 <BOOT_SignatureCheck+0x22>
300070ee:	4620      	mov	r0, r4
300070f0:	463b      	mov	r3, r7
300070f2:	464a      	mov	r2, r9
300070f4:	f002 f9e6 	bl	300094c4 <SBOOT_Validate_ImgHash>
300070f8:	4604      	mov	r4, r0
300070fa:	e7e5      	b.n	300070c8 <BOOT_SignatureCheck+0x1b8>
300070fc:	22b4      	movs	r2, #180	; 0xb4
300070fe:	e73d      	b.n	30006f7c <BOOT_SignatureCheck+0x6c>
30007100:	3000dc44 	.word	0x3000dc44
30007104:	0000c149 	.word	0x0000c149
30007108:	30009501 	.word	0x30009501
3000710c:	3000bcf8 	.word	0x3000bcf8
30007110:	3000ca04 	.word	0x3000ca04
30007114:	00012be5 	.word	0x00012be5
30007118:	e000ed00 	.word	0xe000ed00
3000711c:	3000921d 	.word	0x3000921d
30007120:	3000c9ec 	.word	0x3000c9ec

30007124 <BOOT_CertificateCheck>:
30007124:	b570      	push	{r4, r5, r6, lr}
30007126:	4b2b      	ldr	r3, [pc, #172]	; (300071d4 <BOOT_CertificateCheck+0xb0>)
30007128:	b0b0      	sub	sp, #192	; 0xc0
3000712a:	4604      	mov	r4, r0
3000712c:	460d      	mov	r5, r1
3000712e:	4798      	blx	r3
30007130:	b910      	cbnz	r0, 30007138 <BOOT_CertificateCheck+0x14>
30007132:	2000      	movs	r0, #0
30007134:	b030      	add	sp, #192	; 0xc0
30007136:	bd70      	pop	{r4, r5, r6, pc}
30007138:	69a1      	ldr	r1, [r4, #24]
3000713a:	2290      	movs	r2, #144	; 0x90
3000713c:	a80c      	add	r0, sp, #48	; 0x30
3000713e:	4429      	add	r1, r5
30007140:	f7fd fb84 	bl	3000484c <BOOT_ImgCopy>
30007144:	4b24      	ldr	r3, [pc, #144]	; (300071d8 <BOOT_CertificateCheck+0xb4>)
30007146:	4798      	blx	r3
30007148:	4924      	ldr	r1, [pc, #144]	; (300071dc <BOOT_CertificateCheck+0xb8>)
3000714a:	f000 031f 	and.w	r3, r0, #31
3000714e:	2220      	movs	r2, #32
30007150:	6808      	ldr	r0, [r1, #0]
30007152:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30007156:	a930      	add	r1, sp, #192	; 0xc0
30007158:	3a01      	subs	r2, #1
3000715a:	f813 c000 	ldrb.w	ip, [r3, r0]
3000715e:	4419      	add	r1, r3
30007160:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30007164:	f103 0301 	add.w	r3, r3, #1
30007168:	f801 ccb0 	strb.w	ip, [r1, #-176]
3000716c:	f003 031f 	and.w	r3, r3, #31
30007170:	d1f1      	bne.n	30007156 <BOOT_CertificateCheck+0x32>
30007172:	4610      	mov	r0, r2
30007174:	4b1a      	ldr	r3, [pc, #104]	; (300071e0 <BOOT_CertificateCheck+0xbc>)
30007176:	4798      	blx	r3
30007178:	7ce3      	ldrb	r3, [r4, #19]
3000717a:	7ca2      	ldrb	r2, [r4, #18]
3000717c:	f10d 010f 	add.w	r1, sp, #15
30007180:	f10d 000e 	add.w	r0, sp, #14
30007184:	f002 f9d8 	bl	30009538 <SBOOT_Validate_Algorithm>
30007188:	4605      	mov	r5, r0
3000718a:	b9c0      	cbnz	r0, 300071be <BOOT_CertificateCheck+0x9a>
3000718c:	f104 0620 	add.w	r6, r4, #32
30007190:	aa04      	add	r2, sp, #16
30007192:	f89d 000e 	ldrb.w	r0, [sp, #14]
30007196:	4631      	mov	r1, r6
30007198:	f002 f950 	bl	3000943c <SBOOT_Validate_PubKey>
3000719c:	4605      	mov	r5, r0
3000719e:	b970      	cbnz	r0, 300071be <BOOT_CertificateCheck+0x9a>
300071a0:	69a0      	ldr	r0, [r4, #24]
300071a2:	4623      	mov	r3, r4
300071a4:	ac0c      	add	r4, sp, #48	; 0x30
300071a6:	4632      	mov	r2, r6
300071a8:	f89d 100f 	ldrb.w	r1, [sp, #15]
300071ac:	e9cd 0400 	strd	r0, r4, [sp]
300071b0:	f89d 000e 	ldrb.w	r0, [sp, #14]
300071b4:	f002 f960 	bl	30009478 <SBOOT_Validate_Signature>
300071b8:	4605      	mov	r5, r0
300071ba:	2800      	cmp	r0, #0
300071bc:	d0b9      	beq.n	30007132 <BOOT_CertificateCheck+0xe>
300071be:	4b09      	ldr	r3, [pc, #36]	; (300071e4 <BOOT_CertificateCheck+0xc0>)
300071c0:	2002      	movs	r0, #2
300071c2:	4a09      	ldr	r2, [pc, #36]	; (300071e8 <BOOT_CertificateCheck+0xc4>)
300071c4:	4619      	mov	r1, r3
300071c6:	9500      	str	r5, [sp, #0]
300071c8:	f004 f9c4 	bl	3000b554 <rtk_log_write>
300071cc:	20ff      	movs	r0, #255	; 0xff
300071ce:	b030      	add	sp, #192	; 0xc0
300071d0:	bd70      	pop	{r4, r5, r6, pc}
300071d2:	bf00      	nop
300071d4:	0000c149 	.word	0x0000c149
300071d8:	0000ea4d 	.word	0x0000ea4d
300071dc:	3000df44 	.word	0x3000df44
300071e0:	30009501 	.word	0x30009501
300071e4:	3000bcf8 	.word	0x3000bcf8
300071e8:	3000ca28 	.word	0x3000ca28

300071ec <BOOT_RAM_TZCfg>:
300071ec:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300071f0:	b085      	sub	sp, #20
300071f2:	4e58      	ldr	r6, [pc, #352]	; (30007354 <BOOT_RAM_TZCfg+0x168>)
300071f4:	4958      	ldr	r1, [pc, #352]	; (30007358 <BOOT_RAM_TZCfg+0x16c>)
300071f6:	2501      	movs	r5, #1
300071f8:	f10d 0e08 	add.w	lr, sp, #8
300071fc:	4a57      	ldr	r2, [pc, #348]	; (3000735c <BOOT_RAM_TZCfg+0x170>)
300071fe:	4b58      	ldr	r3, [pc, #352]	; (30007360 <BOOT_RAM_TZCfg+0x174>)
30007200:	46ec      	mov	ip, sp
30007202:	4677      	mov	r7, lr
30007204:	f8df 8178 	ldr.w	r8, [pc, #376]	; 30007380 <BOOT_RAM_TZCfg+0x194>
30007208:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
3000720c:	f8df 9174 	ldr.w	r9, [pc, #372]	; 30007384 <BOOT_RAM_TZCfg+0x198>
30007210:	9202      	str	r2, [sp, #8]
30007212:	9303      	str	r3, [sp, #12]
30007214:	e9cd 6100 	strd	r6, r1, [sp]
30007218:	f85c 2b04 	ldr.w	r2, [ip], #4
3000721c:	2300      	movs	r3, #0
3000721e:	f85e 1b04 	ldr.w	r1, [lr], #4
30007222:	680c      	ldr	r4, [r1, #0]
30007224:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30007228:	fa05 f003 	lsl.w	r0, r5, r3
3000722c:	f1b4 3fff 	cmp.w	r4, #4294967295	; 0xffffffff
30007230:	d00c      	beq.n	3000724c <BOOT_RAM_TZCfg+0x60>
30007232:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30007236:	3301      	adds	r3, #1
30007238:	684c      	ldr	r4, [r1, #4]
3000723a:	310c      	adds	r1, #12
3000723c:	2b08      	cmp	r3, #8
3000723e:	f8cb 4004 	str.w	r4, [fp, #4]
30007242:	6c14      	ldr	r4, [r2, #64]	; 0x40
30007244:	ea40 0004 	orr.w	r0, r0, r4
30007248:	6410      	str	r0, [r2, #64]	; 0x40
3000724a:	d1ea      	bne.n	30007222 <BOOT_RAM_TZCfg+0x36>
3000724c:	42b2      	cmp	r2, r6
3000724e:	d072      	beq.n	30007336 <BOOT_RAM_TZCfg+0x14a>
30007250:	6455      	str	r5, [r2, #68]	; 0x44
30007252:	45bc      	cmp	ip, r7
30007254:	d1e0      	bne.n	30007218 <BOOT_RAM_TZCfg+0x2c>
30007256:	4b43      	ldr	r3, [pc, #268]	; (30007364 <BOOT_RAM_TZCfg+0x178>)
30007258:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
3000725c:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30007260:	4841      	ldr	r0, [pc, #260]	; (30007368 <BOOT_RAM_TZCfg+0x17c>)
30007262:	6019      	str	r1, [r3, #0]
30007264:	2501      	movs	r5, #1
30007266:	605a      	str	r2, [r3, #4]
30007268:	2100      	movs	r1, #0
3000726a:	4a40      	ldr	r2, [pc, #256]	; (3000736c <BOOT_RAM_TZCfg+0x180>)
3000726c:	4c40      	ldr	r4, [pc, #256]	; (30007370 <BOOT_RAM_TZCfg+0x184>)
3000726e:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
30007272:	6813      	ldr	r3, [r2, #0]
30007274:	3301      	adds	r3, #1
30007276:	d019      	beq.n	300072ac <BOOT_RAM_TZCfg+0xc0>
30007278:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
3000727c:	320c      	adds	r2, #12
3000727e:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30007282:	3101      	adds	r1, #1
30007284:	f023 031f 	bic.w	r3, r3, #31
30007288:	2908      	cmp	r1, #8
3000728a:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
3000728e:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30007292:	ea4f 0343 	mov.w	r3, r3, lsl #1
30007296:	f020 001f 	bic.w	r0, r0, #31
3000729a:	f003 0302 	and.w	r3, r3, #2
3000729e:	ea43 0300 	orr.w	r3, r3, r0
300072a2:	f043 0301 	orr.w	r3, r3, #1
300072a6:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
300072aa:	d1e2      	bne.n	30007272 <BOOT_RAM_TZCfg+0x86>
300072ac:	4830      	ldr	r0, [pc, #192]	; (30007370 <BOOT_RAM_TZCfg+0x184>)
300072ae:	2201      	movs	r2, #1
300072b0:	4b30      	ldr	r3, [pc, #192]	; (30007374 <BOOT_RAM_TZCfg+0x188>)
300072b2:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
300072b6:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
300072ba:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
300072be:	d11e      	bne.n	300072fe <BOOT_RAM_TZCfg+0x112>
300072c0:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300072c4:	f3bf 8f4f 	dsb	sy
300072c8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300072cc:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300072d0:	f3c5 344e 	ubfx	r4, r5, #13, #15
300072d4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300072d8:	0164      	lsls	r4, r4, #5
300072da:	ea04 0106 	and.w	r1, r4, r6
300072de:	462b      	mov	r3, r5
300072e0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300072e4:	3b01      	subs	r3, #1
300072e6:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300072ea:	1c5a      	adds	r2, r3, #1
300072ec:	d1f8      	bne.n	300072e0 <BOOT_RAM_TZCfg+0xf4>
300072ee:	3c20      	subs	r4, #32
300072f0:	f114 0f20 	cmn.w	r4, #32
300072f4:	d1f1      	bne.n	300072da <BOOT_RAM_TZCfg+0xee>
300072f6:	f3bf 8f4f 	dsb	sy
300072fa:	f3bf 8f6f 	isb	sy
300072fe:	4b1c      	ldr	r3, [pc, #112]	; (30007370 <BOOT_RAM_TZCfg+0x184>)
30007300:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30007304:	491c      	ldr	r1, [pc, #112]	; (30007378 <BOOT_RAM_TZCfg+0x18c>)
30007306:	691c      	ldr	r4, [r3, #16]
30007308:	4a1c      	ldr	r2, [pc, #112]	; (3000737c <BOOT_RAM_TZCfg+0x190>)
3000730a:	f044 0408 	orr.w	r4, r4, #8
3000730e:	611c      	str	r4, [r3, #16]
30007310:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
30007314:	68d8      	ldr	r0, [r3, #12]
30007316:	4028      	ands	r0, r5
30007318:	4301      	orrs	r1, r0
3000731a:	60d9      	str	r1, [r3, #12]
3000731c:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30007320:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30007324:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30007328:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
3000732c:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30007330:	b005      	add	sp, #20
30007332:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007336:	f898 3000 	ldrb.w	r3, [r8]
3000733a:	2b00      	cmp	r3, #0
3000733c:	d189      	bne.n	30007252 <BOOT_RAM_TZCfg+0x66>
3000733e:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
30007342:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
30007346:	6c33      	ldr	r3, [r6, #64]	; 0x40
30007348:	f043 0380 	orr.w	r3, r3, #128	; 0x80
3000734c:	6433      	str	r3, [r6, #64]	; 0x40
3000734e:	6475      	str	r5, [r6, #68]	; 0x44
30007350:	e77f      	b.n	30007252 <BOOT_RAM_TZCfg+0x66>
30007352:	bf00      	nop
30007354:	51001a00 	.word	0x51001a00
30007358:	51001200 	.word	0x51001200
3000735c:	3000e044 	.word	0x3000e044
30007360:	3000dfe4 	.word	0x3000dfe4
30007364:	51001b00 	.word	0x51001b00
30007368:	500e0000 	.word	0x500e0000
3000736c:	3000e0a4 	.word	0x3000e0a4
30007370:	e000ed00 	.word	0xe000ed00
30007374:	42008000 	.word	0x42008000
30007378:	05fa6000 	.word	0x05fa6000
3000737c:	e000e100 	.word	0xe000e100
30007380:	3000df3f 	.word	0x3000df3f
30007384:	005fffff 	.word	0x005fffff

30007388 <ChipInfo_Invalid>:
30007388:	4e06      	ldr	r6, [pc, #24]	; (300073a4 <ChipInfo_Invalid+0x1c>)
3000738a:	4d07      	ldr	r5, [pc, #28]	; (300073a8 <ChipInfo_Invalid+0x20>)
3000738c:	4c07      	ldr	r4, [pc, #28]	; (300073ac <ChipInfo_Invalid+0x24>)
3000738e:	b508      	push	{r3, lr}
30007390:	462b      	mov	r3, r5
30007392:	4622      	mov	r2, r4
30007394:	4904      	ldr	r1, [pc, #16]	; (300073a8 <ChipInfo_Invalid+0x20>)
30007396:	2002      	movs	r0, #2
30007398:	f004 f8dc 	bl	3000b554 <rtk_log_write>
3000739c:	f242 7010 	movw	r0, #10000	; 0x2710
300073a0:	47b0      	blx	r6
300073a2:	e7f5      	b.n	30007390 <ChipInfo_Invalid+0x8>
300073a4:	00009be5 	.word	0x00009be5
300073a8:	3000ca54 	.word	0x3000ca54
300073ac:	3000ca60 	.word	0x3000ca60

300073b0 <ChipInfo_Get>:
300073b0:	b570      	push	{r4, r5, r6, lr}
300073b2:	4c11      	ldr	r4, [pc, #68]	; (300073f8 <ChipInfo_Get+0x48>)
300073b4:	7820      	ldrb	r0, [r4, #0]
300073b6:	28ff      	cmp	r0, #255	; 0xff
300073b8:	d104      	bne.n	300073c4 <ChipInfo_Get+0x14>
300073ba:	4d10      	ldr	r5, [pc, #64]	; (300073fc <ChipInfo_Get+0x4c>)
300073bc:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
300073c0:	b108      	cbz	r0, 300073c6 <ChipInfo_Get+0x16>
300073c2:	7020      	strb	r0, [r4, #0]
300073c4:	bd70      	pop	{r4, r5, r6, pc}
300073c6:	4621      	mov	r1, r4
300073c8:	f240 70ff 	movw	r0, #2047	; 0x7ff
300073cc:	f000 fec4 	bl	30008158 <OTP_Read8>
300073d0:	7820      	ldrb	r0, [r4, #0]
300073d2:	28ff      	cmp	r0, #255	; 0xff
300073d4:	d002      	beq.n	300073dc <ChipInfo_Get+0x2c>
300073d6:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
300073da:	bd70      	pop	{r4, r5, r6, pc}
300073dc:	4e08      	ldr	r6, [pc, #32]	; (30007400 <ChipInfo_Get+0x50>)
300073de:	4d09      	ldr	r5, [pc, #36]	; (30007404 <ChipInfo_Get+0x54>)
300073e0:	4c09      	ldr	r4, [pc, #36]	; (30007408 <ChipInfo_Get+0x58>)
300073e2:	462b      	mov	r3, r5
300073e4:	4622      	mov	r2, r4
300073e6:	4907      	ldr	r1, [pc, #28]	; (30007404 <ChipInfo_Get+0x54>)
300073e8:	2003      	movs	r0, #3
300073ea:	f004 f8b3 	bl	3000b554 <rtk_log_write>
300073ee:	f242 7010 	movw	r0, #10000	; 0x2710
300073f2:	47b0      	blx	r6
300073f4:	e7f5      	b.n	300073e2 <ChipInfo_Get+0x32>
300073f6:	bf00      	nop
300073f8:	3000e628 	.word	0x3000e628
300073fc:	23020000 	.word	0x23020000
30007400:	00009be5 	.word	0x00009be5
30007404:	3000ca54 	.word	0x3000ca54
30007408:	3000ca84 	.word	0x3000ca84

3000740c <ChipInfo_MemoryType>:
3000740c:	b510      	push	{r4, lr}
3000740e:	4c11      	ldr	r4, [pc, #68]	; (30007454 <ChipInfo_MemoryType+0x48>)
30007410:	47a0      	blx	r4
30007412:	2802      	cmp	r0, #2
30007414:	d019      	beq.n	3000744a <ChipInfo_MemoryType+0x3e>
30007416:	47a0      	blx	r4
30007418:	2803      	cmp	r0, #3
3000741a:	d016      	beq.n	3000744a <ChipInfo_MemoryType+0x3e>
3000741c:	4c0e      	ldr	r4, [pc, #56]	; (30007458 <ChipInfo_MemoryType+0x4c>)
3000741e:	4b0f      	ldr	r3, [pc, #60]	; (3000745c <ChipInfo_MemoryType+0x50>)
30007420:	4798      	blx	r3
30007422:	2100      	movs	r1, #0
30007424:	23e0      	movs	r3, #224	; 0xe0
30007426:	4622      	mov	r2, r4
30007428:	e004      	b.n	30007434 <ChipInfo_MemoryType+0x28>
3000742a:	7a13      	ldrb	r3, [r2, #8]
3000742c:	3101      	adds	r1, #1
3000742e:	3206      	adds	r2, #6
30007430:	2bff      	cmp	r3, #255	; 0xff
30007432:	d00c      	beq.n	3000744e <ChipInfo_MemoryType+0x42>
30007434:	4298      	cmp	r0, r3
30007436:	d1f8      	bne.n	3000742a <ChipInfo_MemoryType+0x1e>
30007438:	eb01 0141 	add.w	r1, r1, r1, lsl #1
3000743c:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007440:	78cb      	ldrb	r3, [r1, #3]
30007442:	2b02      	cmp	r3, #2
30007444:	d001      	beq.n	3000744a <ChipInfo_MemoryType+0x3e>
30007446:	2001      	movs	r0, #1
30007448:	bd10      	pop	{r4, pc}
3000744a:	2002      	movs	r0, #2
3000744c:	bd10      	pop	{r4, pc}
3000744e:	f7ff ff9b 	bl	30007388 <ChipInfo_Invalid>
30007452:	bf00      	nop
30007454:	0000c0f9 	.word	0x0000c0f9
30007458:	3000cab0 	.word	0x3000cab0
3000745c:	300073b1 	.word	0x300073b1

30007460 <ChipInfo_DDRType>:
30007460:	b510      	push	{r4, lr}
30007462:	4b0f      	ldr	r3, [pc, #60]	; (300074a0 <ChipInfo_DDRType+0x40>)
30007464:	4798      	blx	r3
30007466:	4b0f      	ldr	r3, [pc, #60]	; (300074a4 <ChipInfo_DDRType+0x44>)
30007468:	4604      	mov	r4, r0
3000746a:	4798      	blx	r3
3000746c:	2801      	cmp	r0, #1
3000746e:	d112      	bne.n	30007496 <ChipInfo_DDRType+0x36>
30007470:	480d      	ldr	r0, [pc, #52]	; (300074a8 <ChipInfo_DDRType+0x48>)
30007472:	2100      	movs	r1, #0
30007474:	22e0      	movs	r2, #224	; 0xe0
30007476:	4603      	mov	r3, r0
30007478:	e003      	b.n	30007482 <ChipInfo_DDRType+0x22>
3000747a:	789a      	ldrb	r2, [r3, #2]
3000747c:	3101      	adds	r1, #1
3000747e:	2aff      	cmp	r2, #255	; 0xff
30007480:	d00b      	beq.n	3000749a <ChipInfo_DDRType+0x3a>
30007482:	4294      	cmp	r4, r2
30007484:	f103 0306 	add.w	r3, r3, #6
30007488:	d1f7      	bne.n	3000747a <ChipInfo_DDRType+0x1a>
3000748a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
3000748e:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007492:	78c8      	ldrb	r0, [r1, #3]
30007494:	bd10      	pop	{r4, pc}
30007496:	20ff      	movs	r0, #255	; 0xff
30007498:	bd10      	pop	{r4, pc}
3000749a:	2000      	movs	r0, #0
3000749c:	bd10      	pop	{r4, pc}
3000749e:	bf00      	nop
300074a0:	300073b1 	.word	0x300073b1
300074a4:	3000740d 	.word	0x3000740d
300074a8:	3000cab0 	.word	0x3000cab0

300074ac <ChipInfo_MemorySize>:
300074ac:	b510      	push	{r4, lr}
300074ae:	4c0b      	ldr	r4, [pc, #44]	; (300074dc <ChipInfo_MemorySize+0x30>)
300074b0:	4b0b      	ldr	r3, [pc, #44]	; (300074e0 <ChipInfo_MemorySize+0x34>)
300074b2:	4798      	blx	r3
300074b4:	2100      	movs	r1, #0
300074b6:	23e0      	movs	r3, #224	; 0xe0
300074b8:	4622      	mov	r2, r4
300074ba:	e004      	b.n	300074c6 <ChipInfo_MemorySize+0x1a>
300074bc:	7a13      	ldrb	r3, [r2, #8]
300074be:	3101      	adds	r1, #1
300074c0:	3206      	adds	r2, #6
300074c2:	2bff      	cmp	r3, #255	; 0xff
300074c4:	d007      	beq.n	300074d6 <ChipInfo_MemorySize+0x2a>
300074c6:	4298      	cmp	r0, r3
300074c8:	d1f8      	bne.n	300074bc <ChipInfo_MemorySize+0x10>
300074ca:	eb01 0141 	add.w	r1, r1, r1, lsl #1
300074ce:	eb04 0141 	add.w	r1, r4, r1, lsl #1
300074d2:	7908      	ldrb	r0, [r1, #4]
300074d4:	bd10      	pop	{r4, pc}
300074d6:	f7ff ff57 	bl	30007388 <ChipInfo_Invalid>
300074da:	bf00      	nop
300074dc:	3000cab0 	.word	0x3000cab0
300074e0:	300073b1 	.word	0x300073b1

300074e4 <ChipInfo_MemoryVendor>:
300074e4:	b510      	push	{r4, lr}
300074e6:	4c0f      	ldr	r4, [pc, #60]	; (30007524 <ChipInfo_MemoryVendor+0x40>)
300074e8:	47a0      	blx	r4
300074ea:	2803      	cmp	r0, #3
300074ec:	d016      	beq.n	3000751c <ChipInfo_MemoryVendor+0x38>
300074ee:	47a0      	blx	r4
300074f0:	2802      	cmp	r0, #2
300074f2:	d012      	beq.n	3000751a <ChipInfo_MemoryVendor+0x36>
300074f4:	4c0c      	ldr	r4, [pc, #48]	; (30007528 <ChipInfo_MemoryVendor+0x44>)
300074f6:	4b0d      	ldr	r3, [pc, #52]	; (3000752c <ChipInfo_MemoryVendor+0x48>)
300074f8:	4798      	blx	r3
300074fa:	2100      	movs	r1, #0
300074fc:	23e0      	movs	r3, #224	; 0xe0
300074fe:	4622      	mov	r2, r4
30007500:	e004      	b.n	3000750c <ChipInfo_MemoryVendor+0x28>
30007502:	7a13      	ldrb	r3, [r2, #8]
30007504:	3101      	adds	r1, #1
30007506:	3206      	adds	r2, #6
30007508:	2bff      	cmp	r3, #255	; 0xff
3000750a:	d009      	beq.n	30007520 <ChipInfo_MemoryVendor+0x3c>
3000750c:	4298      	cmp	r0, r3
3000750e:	d1f8      	bne.n	30007502 <ChipInfo_MemoryVendor+0x1e>
30007510:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007514:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007518:	7948      	ldrb	r0, [r1, #5]
3000751a:	bd10      	pop	{r4, pc}
3000751c:	2001      	movs	r0, #1
3000751e:	bd10      	pop	{r4, pc}
30007520:	f7ff ff32 	bl	30007388 <ChipInfo_Invalid>
30007524:	0000c0f9 	.word	0x0000c0f9
30007528:	3000cab0 	.word	0x3000cab0
3000752c:	300073b1 	.word	0x300073b1

30007530 <SDM32K_Enable>:
30007530:	4a02      	ldr	r2, [pc, #8]	; (3000753c <SDM32K_Enable+0xc>)
30007532:	4b03      	ldr	r3, [pc, #12]	; (30007540 <SDM32K_Enable+0x10>)
30007534:	6811      	ldr	r1, [r2, #0]
30007536:	430b      	orrs	r3, r1
30007538:	6013      	str	r3, [r2, #0]
3000753a:	4770      	bx	lr
3000753c:	42008e00 	.word	0x42008e00
30007540:	c0060000 	.word	0xc0060000

30007544 <CLK_SWITCH_XTAL>:
30007544:	b508      	push	{r3, lr}
30007546:	2801      	cmp	r0, #1
30007548:	4b0a      	ldr	r3, [pc, #40]	; (30007574 <CLK_SWITCH_XTAL+0x30>)
3000754a:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
3000754e:	d00d      	beq.n	3000756c <CLK_SWITCH_XTAL+0x28>
30007550:	4b09      	ldr	r3, [pc, #36]	; (30007578 <CLK_SWITCH_XTAL+0x34>)
30007552:	4313      	orrs	r3, r2
30007554:	4907      	ldr	r1, [pc, #28]	; (30007574 <CLK_SWITCH_XTAL+0x30>)
30007556:	4a09      	ldr	r2, [pc, #36]	; (3000757c <CLK_SWITCH_XTAL+0x38>)
30007558:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
3000755c:	4790      	blx	r2
3000755e:	4a08      	ldr	r2, [pc, #32]	; (30007580 <CLK_SWITCH_XTAL+0x3c>)
30007560:	4b08      	ldr	r3, [pc, #32]	; (30007584 <CLK_SWITCH_XTAL+0x40>)
30007562:	fba2 2000 	umull	r2, r0, r2, r0
30007566:	0c80      	lsrs	r0, r0, #18
30007568:	6018      	str	r0, [r3, #0]
3000756a:	bd08      	pop	{r3, pc}
3000756c:	4b06      	ldr	r3, [pc, #24]	; (30007588 <CLK_SWITCH_XTAL+0x44>)
3000756e:	4013      	ands	r3, r2
30007570:	e7f0      	b.n	30007554 <CLK_SWITCH_XTAL+0x10>
30007572:	bf00      	nop
30007574:	42008000 	.word	0x42008000
30007578:	80000500 	.word	0x80000500
3000757c:	30008eed 	.word	0x30008eed
30007580:	431bde83 	.word	0x431bde83
30007584:	2001c700 	.word	0x2001c700
30007588:	7ffffaff 	.word	0x7ffffaff

3000758c <DDR_PHY_ChipInfo.part.0>:
3000758c:	b508      	push	{r3, lr}
3000758e:	4b24      	ldr	r3, [pc, #144]	; (30007620 <DDR_PHY_ChipInfo.part.0+0x94>)
30007590:	4798      	blx	r3
30007592:	280e      	cmp	r0, #14
30007594:	d80d      	bhi.n	300075b2 <DDR_PHY_ChipInfo.part.0+0x26>
30007596:	2801      	cmp	r0, #1
30007598:	d912      	bls.n	300075c0 <DDR_PHY_ChipInfo.part.0+0x34>
3000759a:	1e83      	subs	r3, r0, #2
3000759c:	2b0c      	cmp	r3, #12
3000759e:	d80f      	bhi.n	300075c0 <DDR_PHY_ChipInfo.part.0+0x34>
300075a0:	e8df f003 	tbb	[pc, r3]
300075a4:	210e1d19 	.word	0x210e1d19
300075a8:	2a0e260e 	.word	0x2a0e260e
300075ac:	340e0e2f 	.word	0x340e0e2f
300075b0:	39          	.byte	0x39
300075b1:	00          	.byte	0x00
300075b2:	28e0      	cmp	r0, #224	; 0xe0
300075b4:	d104      	bne.n	300075c0 <DDR_PHY_ChipInfo.part.0+0x34>
300075b6:	2301      	movs	r3, #1
300075b8:	4a1a      	ldr	r2, [pc, #104]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
300075ba:	4618      	mov	r0, r3
300075bc:	7013      	strb	r3, [r2, #0]
300075be:	bd08      	pop	{r3, pc}
300075c0:	4b19      	ldr	r3, [pc, #100]	; (30007628 <DDR_PHY_ChipInfo.part.0+0x9c>)
300075c2:	2002      	movs	r0, #2
300075c4:	4a19      	ldr	r2, [pc, #100]	; (3000762c <DDR_PHY_ChipInfo.part.0+0xa0>)
300075c6:	4619      	mov	r1, r3
300075c8:	f003 ffc4 	bl	3000b554 <rtk_log_write>
300075cc:	2300      	movs	r3, #0
300075ce:	4a15      	ldr	r2, [pc, #84]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
300075d0:	4618      	mov	r0, r3
300075d2:	7013      	strb	r3, [r2, #0]
300075d4:	bd08      	pop	{r3, pc}
300075d6:	4b13      	ldr	r3, [pc, #76]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
300075d8:	2202      	movs	r2, #2
300075da:	701a      	strb	r2, [r3, #0]
300075dc:	bd08      	pop	{r3, pc}
300075de:	4b11      	ldr	r3, [pc, #68]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
300075e0:	2203      	movs	r2, #3
300075e2:	701a      	strb	r2, [r3, #0]
300075e4:	bd08      	pop	{r3, pc}
300075e6:	2304      	movs	r3, #4
300075e8:	4a0e      	ldr	r2, [pc, #56]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
300075ea:	4618      	mov	r0, r3
300075ec:	7013      	strb	r3, [r2, #0]
300075ee:	bd08      	pop	{r3, pc}
300075f0:	4b0c      	ldr	r3, [pc, #48]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
300075f2:	2207      	movs	r2, #7
300075f4:	701a      	strb	r2, [r3, #0]
300075f6:	bd08      	pop	{r3, pc}
300075f8:	2305      	movs	r3, #5
300075fa:	4a0a      	ldr	r2, [pc, #40]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
300075fc:	4618      	mov	r0, r3
300075fe:	7013      	strb	r3, [r2, #0]
30007600:	bd08      	pop	{r3, pc}
30007602:	2306      	movs	r3, #6
30007604:	4a07      	ldr	r2, [pc, #28]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
30007606:	4618      	mov	r0, r3
30007608:	7013      	strb	r3, [r2, #0]
3000760a:	bd08      	pop	{r3, pc}
3000760c:	2308      	movs	r3, #8
3000760e:	4a05      	ldr	r2, [pc, #20]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
30007610:	4618      	mov	r0, r3
30007612:	7013      	strb	r3, [r2, #0]
30007614:	bd08      	pop	{r3, pc}
30007616:	2309      	movs	r3, #9
30007618:	4a02      	ldr	r2, [pc, #8]	; (30007624 <DDR_PHY_ChipInfo.part.0+0x98>)
3000761a:	4618      	mov	r0, r3
3000761c:	7013      	strb	r3, [r2, #0]
3000761e:	bd08      	pop	{r3, pc}
30007620:	300073b1 	.word	0x300073b1
30007624:	3000e629 	.word	0x3000e629
30007628:	3000cb34 	.word	0x3000cb34
3000762c:	3000cb3c 	.word	0x3000cb3c

30007630 <DDR_PHY_ChipInfo_ddrtype>:
30007630:	b510      	push	{r4, lr}
30007632:	4c04      	ldr	r4, [pc, #16]	; (30007644 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007634:	7820      	ldrb	r0, [r4, #0]
30007636:	b100      	cbz	r0, 3000763a <DDR_PHY_ChipInfo_ddrtype+0xa>
30007638:	bd10      	pop	{r4, pc}
3000763a:	4b03      	ldr	r3, [pc, #12]	; (30007648 <DDR_PHY_ChipInfo_ddrtype+0x18>)
3000763c:	4798      	blx	r3
3000763e:	7020      	strb	r0, [r4, #0]
30007640:	bd10      	pop	{r4, pc}
30007642:	bf00      	nop
30007644:	3000f330 	.word	0x3000f330
30007648:	30007461 	.word	0x30007461

3000764c <DDR_PHY_StructInit>:
3000764c:	f240 2315 	movw	r3, #533	; 0x215
30007650:	6583      	str	r3, [r0, #88]	; 0x58
30007652:	4770      	bx	lr

30007654 <DDR_PHY_DLL_CLK_DIV>:
30007654:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007656:	4d70      	ldr	r5, [pc, #448]	; (30007818 <DDR_PHY_DLL_CLK_DIV+0x1c4>)
30007658:	4c70      	ldr	r4, [pc, #448]	; (3000781c <DDR_PHY_DLL_CLK_DIV+0x1c8>)
3000765a:	fba5 3200 	umull	r3, r2, r5, r0
3000765e:	2365      	movs	r3, #101	; 0x65
30007660:	eb00 0e04 	add.w	lr, r0, r4
30007664:	0952      	lsrs	r2, r2, #5
30007666:	fb00 f303 	mul.w	r3, r0, r3
3000766a:	486d      	ldr	r0, [pc, #436]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000766c:	f1a2 0c03 	sub.w	ip, r2, #3
30007670:	6a47      	ldr	r7, [r0, #36]	; 0x24
30007672:	eb0c 018c 	add.w	r1, ip, ip, lsl #2
30007676:	ea4f 2c0c 	mov.w	ip, ip, lsl #8
3000767a:	ebae 0ec1 	sub.w	lr, lr, r1, lsl #3
3000767e:	fa1f fc8c 	uxth.w	ip, ip
30007682:	ea4f 2ece 	mov.w	lr, lr, lsl #11
30007686:	fba5 1e0e 	umull	r1, lr, r5, lr
3000768a:	4966      	ldr	r1, [pc, #408]	; (30007824 <DDR_PHY_DLL_CLK_DIV+0x1d0>)
3000768c:	fba1 6103 	umull	r6, r1, r1, r3
30007690:	4e65      	ldr	r6, [pc, #404]	; (30007828 <DDR_PHY_DLL_CLK_DIV+0x1d4>)
30007692:	0a09      	lsrs	r1, r1, #8
30007694:	1a52      	subs	r2, r2, r1
30007696:	3903      	subs	r1, #3
30007698:	02d2      	lsls	r2, r2, #11
3000769a:	eb02 125e 	add.w	r2, r2, lr, lsr #5
3000769e:	ea06 2ece 	and.w	lr, r6, lr, lsl #11
300076a2:	4e62      	ldr	r6, [pc, #392]	; (3000782c <DDR_PHY_DLL_CLK_DIV+0x1d8>)
300076a4:	403e      	ands	r6, r7
300076a6:	ea4e 0606 	orr.w	r6, lr, r6
300076aa:	6246      	str	r6, [r0, #36]	; 0x24
300076ac:	4e60      	ldr	r6, [pc, #384]	; (30007830 <DDR_PHY_DLL_CLK_DIV+0x1dc>)
300076ae:	fba6 6303 	umull	r6, r3, r6, r3
300076b2:	eb04 1353 	add.w	r3, r4, r3, lsr #5
300076b6:	6a84      	ldr	r4, [r0, #40]	; 0x28
300076b8:	f424 447f 	bic.w	r4, r4, #65280	; 0xff00
300076bc:	ea4c 0404 	orr.w	r4, ip, r4
300076c0:	6284      	str	r4, [r0, #40]	; 0x28
300076c2:	eb01 0481 	add.w	r4, r1, r1, lsl #2
300076c6:	b2c9      	uxtb	r1, r1
300076c8:	eba3 03c4 	sub.w	r3, r3, r4, lsl #3
300076cc:	02db      	lsls	r3, r3, #11
300076ce:	fba5 4303 	umull	r4, r3, r5, r3
300076d2:	6a45      	ldr	r5, [r0, #36]	; 0x24
300076d4:	4c57      	ldr	r4, [pc, #348]	; (30007834 <DDR_PHY_DLL_CLK_DIV+0x1e0>)
300076d6:	eba2 1253 	sub.w	r2, r2, r3, lsr #5
300076da:	f3c3 134a 	ubfx	r3, r3, #5, #11
300076de:	402c      	ands	r4, r5
300076e0:	4d55      	ldr	r5, [pc, #340]	; (30007838 <DDR_PHY_DLL_CLK_DIV+0x1e4>)
300076e2:	eb02 1242 	add.w	r2, r2, r2, lsl #5
300076e6:	4323      	orrs	r3, r4
300076e8:	02d2      	lsls	r2, r2, #11
300076ea:	6243      	str	r3, [r0, #36]	; 0x24
300076ec:	6a84      	ldr	r4, [r0, #40]	; 0x28
300076ee:	fba5 3202 	umull	r3, r2, r5, r2
300076f2:	4b52      	ldr	r3, [pc, #328]	; (3000783c <DDR_PHY_DLL_CLK_DIV+0x1e8>)
300076f4:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
300076f8:	4321      	orrs	r1, r4
300076fa:	2401      	movs	r4, #1
300076fc:	6281      	str	r1, [r0, #40]	; 0x28
300076fe:	6a01      	ldr	r1, [r0, #32]
30007700:	400b      	ands	r3, r1
30007702:	494f      	ldr	r1, [pc, #316]	; (30007840 <DDR_PHY_DLL_CLK_DIV+0x1ec>)
30007704:	ea43 3292 	orr.w	r2, r3, r2, lsr #14
30007708:	6202      	str	r2, [r0, #32]
3000770a:	6784      	str	r4, [r0, #120]	; 0x78
3000770c:	7808      	ldrb	r0, [r1, #0]
3000770e:	28ff      	cmp	r0, #255	; 0xff
30007710:	d075      	beq.n	300077fe <DDR_PHY_DLL_CLK_DIV+0x1aa>
30007712:	4c4c      	ldr	r4, [pc, #304]	; (30007844 <DDR_PHY_DLL_CLK_DIV+0x1f0>)
30007714:	0145      	lsls	r5, r0, #5
30007716:	4b42      	ldr	r3, [pc, #264]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007718:	eb04 1040 	add.w	r0, r4, r0, lsl #5
3000771c:	5962      	ldr	r2, [r4, r5]
3000771e:	4e4a      	ldr	r6, [pc, #296]	; (30007848 <DDR_PHY_DLL_CLK_DIV+0x1f4>)
30007720:	611a      	str	r2, [r3, #16]
30007722:	6842      	ldr	r2, [r0, #4]
30007724:	615a      	str	r2, [r3, #20]
30007726:	6882      	ldr	r2, [r0, #8]
30007728:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
3000772c:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
30007730:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
30007734:	609a      	str	r2, [r3, #8]
30007736:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
3000773a:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
3000773e:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
30007742:	7830      	ldrb	r0, [r6, #0]
30007744:	2800      	cmp	r0, #0
30007746:	d056      	beq.n	300077f6 <DDR_PHY_DLL_CLK_DIV+0x1a2>
30007748:	2801      	cmp	r0, #1
3000774a:	442c      	add	r4, r5
3000774c:	4b34      	ldr	r3, [pc, #208]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000774e:	d037      	beq.n	300077c0 <DDR_PHY_DLL_CLK_DIV+0x16c>
30007750:	2222      	movs	r2, #34	; 0x22
30007752:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
30007756:	6962      	ldr	r2, [r4, #20]
30007758:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000775c:	e9d4 1206 	ldrd	r1, r2, [r4, #24]
30007760:	f8c3 1084 	str.w	r1, [r3, #132]	; 0x84
30007764:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
30007768:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000776c:	4937      	ldr	r1, [pc, #220]	; (3000784c <DDR_PHY_DLL_CLK_DIV+0x1f8>)
3000776e:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
30007772:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30007776:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
3000777a:	009b      	lsls	r3, r3, #2
3000777c:	d516      	bpl.n	300077ac <DDR_PHY_DLL_CLK_DIV+0x158>
3000777e:	2800      	cmp	r0, #0
30007780:	d044      	beq.n	3000780c <DDR_PHY_DLL_CLK_DIV+0x1b8>
30007782:	4b27      	ldr	r3, [pc, #156]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007784:	4a32      	ldr	r2, [pc, #200]	; (30007850 <DDR_PHY_DLL_CLK_DIV+0x1fc>)
30007786:	601a      	str	r2, [r3, #0]
30007788:	4b25      	ldr	r3, [pc, #148]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000778a:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000778e:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
30007792:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
30007796:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000779a:	699a      	ldr	r2, [r3, #24]
3000779c:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
300077a0:	619a      	str	r2, [r3, #24]
300077a2:	699a      	ldr	r2, [r3, #24]
300077a4:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
300077a8:	619a      	str	r2, [r3, #24]
300077aa:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
300077ac:	b920      	cbnz	r0, 300077b8 <DDR_PHY_DLL_CLK_DIV+0x164>
300077ae:	4b29      	ldr	r3, [pc, #164]	; (30007854 <DDR_PHY_DLL_CLK_DIV+0x200>)
300077b0:	4798      	blx	r3
300077b2:	2801      	cmp	r0, #1
300077b4:	7030      	strb	r0, [r6, #0]
300077b6:	d025      	beq.n	30007804 <DDR_PHY_DLL_CLK_DIV+0x1b0>
300077b8:	4b19      	ldr	r3, [pc, #100]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300077ba:	4a27      	ldr	r2, [pc, #156]	; (30007858 <DDR_PHY_DLL_CLK_DIV+0x204>)
300077bc:	601a      	str	r2, [r3, #0]
300077be:	e7e3      	b.n	30007788 <DDR_PHY_DLL_CLK_DIV+0x134>
300077c0:	2233      	movs	r2, #51	; 0x33
300077c2:	6965      	ldr	r5, [r4, #20]
300077c4:	4921      	ldr	r1, [pc, #132]	; (3000784c <DDR_PHY_DLL_CLK_DIV+0x1f8>)
300077c6:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
300077ca:	f8c3 5080 	str.w	r5, [r3, #128]	; 0x80
300077ce:	e9d4 0206 	ldrd	r0, r2, [r4, #24]
300077d2:	f8c3 0084 	str.w	r0, [r3, #132]	; 0x84
300077d6:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
300077da:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300077de:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
300077e2:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300077e6:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300077ea:	009a      	lsls	r2, r3, #2
300077ec:	d50a      	bpl.n	30007804 <DDR_PHY_DLL_CLK_DIV+0x1b0>
300077ee:	4b0c      	ldr	r3, [pc, #48]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300077f0:	4a1a      	ldr	r2, [pc, #104]	; (3000785c <DDR_PHY_DLL_CLK_DIV+0x208>)
300077f2:	601a      	str	r2, [r3, #0]
300077f4:	e7c8      	b.n	30007788 <DDR_PHY_DLL_CLK_DIV+0x134>
300077f6:	4b17      	ldr	r3, [pc, #92]	; (30007854 <DDR_PHY_DLL_CLK_DIV+0x200>)
300077f8:	4798      	blx	r3
300077fa:	7030      	strb	r0, [r6, #0]
300077fc:	e7a4      	b.n	30007748 <DDR_PHY_DLL_CLK_DIV+0xf4>
300077fe:	f7ff fec5 	bl	3000758c <DDR_PHY_ChipInfo.part.0>
30007802:	e786      	b.n	30007712 <DDR_PHY_DLL_CLK_DIV+0xbe>
30007804:	4b06      	ldr	r3, [pc, #24]	; (30007820 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007806:	4a16      	ldr	r2, [pc, #88]	; (30007860 <DDR_PHY_DLL_CLK_DIV+0x20c>)
30007808:	601a      	str	r2, [r3, #0]
3000780a:	e7bd      	b.n	30007788 <DDR_PHY_DLL_CLK_DIV+0x134>
3000780c:	4b11      	ldr	r3, [pc, #68]	; (30007854 <DDR_PHY_DLL_CLK_DIV+0x200>)
3000780e:	4798      	blx	r3
30007810:	2801      	cmp	r0, #1
30007812:	7030      	strb	r0, [r6, #0]
30007814:	d1b5      	bne.n	30007782 <DDR_PHY_DLL_CLK_DIV+0x12e>
30007816:	e7ea      	b.n	300077ee <DDR_PHY_DLL_CLK_DIV+0x19a>
30007818:	cccccccd 	.word	0xcccccccd
3000781c:	001fff88 	.word	0x001fff88
30007820:	41011000 	.word	0x41011000
30007824:	10624dd3 	.word	0x10624dd3
30007828:	07ff0000 	.word	0x07ff0000
3000782c:	f800ffff 	.word	0xf800ffff
30007830:	51eb851f 	.word	0x51eb851f
30007834:	fffff800 	.word	0xfffff800
30007838:	d1b71759 	.word	0xd1b71759
3000783c:	fff80000 	.word	0xfff80000
30007840:	3000e629 	.word	0x3000e629
30007844:	3000cb5c 	.word	0x3000cb5c
30007848:	3000f330 	.word	0x3000f330
3000784c:	42008000 	.word	0x42008000
30007850:	4c000821 	.word	0x4c000821
30007854:	30007461 	.word	0x30007461
30007858:	40000021 	.word	0x40000021
3000785c:	4c000903 	.word	0x4c000903
30007860:	40000103 	.word	0x40000103

30007864 <DDR_PHY_CRT_Init>:
30007864:	4a32      	ldr	r2, [pc, #200]	; (30007930 <DDR_PHY_CRT_Init+0xcc>)
30007866:	2005      	movs	r0, #5
30007868:	b538      	push	{r3, r4, r5, lr}
3000786a:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
3000786e:	4c31      	ldr	r4, [pc, #196]	; (30007934 <DDR_PHY_CRT_Init+0xd0>)
30007870:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007874:	4d30      	ldr	r5, [pc, #192]	; (30007938 <DDR_PHY_CRT_Init+0xd4>)
30007876:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
3000787a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000787e:	f043 0301 	orr.w	r3, r3, #1
30007882:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007886:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000788a:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000788e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007892:	68e3      	ldr	r3, [r4, #12]
30007894:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30007898:	60e3      	str	r3, [r4, #12]
3000789a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000789e:	f043 0302 	orr.w	r3, r3, #2
300078a2:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300078a6:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300078aa:	f023 0308 	bic.w	r3, r3, #8
300078ae:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300078b2:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300078b6:	f043 0304 	orr.w	r3, r3, #4
300078ba:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300078be:	47a8      	blx	r5
300078c0:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300078c4:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
300078c8:	f043 0308 	orr.w	r3, r3, #8
300078cc:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300078d0:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300078d2:	f043 6380 	orr.w	r3, r3, #67108864	; 0x4000000
300078d6:	62a3      	str	r3, [r4, #40]	; 0x28
300078d8:	47a8      	blx	r5
300078da:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300078dc:	f641 73ff 	movw	r3, #8191	; 0x1fff
300078e0:	4916      	ldr	r1, [pc, #88]	; (3000793c <DDR_PHY_CRT_Init+0xd8>)
300078e2:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
300078e6:	62a2      	str	r2, [r4, #40]	; 0x28
300078e8:	69e2      	ldr	r2, [r4, #28]
300078ea:	f042 0202 	orr.w	r2, r2, #2
300078ee:	61e2      	str	r2, [r4, #28]
300078f0:	69e2      	ldr	r2, [r4, #28]
300078f2:	f042 0208 	orr.w	r2, r2, #8
300078f6:	61e2      	str	r2, [r4, #28]
300078f8:	68a2      	ldr	r2, [r4, #8]
300078fa:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
300078fe:	60a2      	str	r2, [r4, #8]
30007900:	6862      	ldr	r2, [r4, #4]
30007902:	4313      	orrs	r3, r2
30007904:	6063      	str	r3, [r4, #4]
30007906:	68a3      	ldr	r3, [r4, #8]
30007908:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
3000790c:	60a3      	str	r3, [r4, #8]
3000790e:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
30007912:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007916:	f023 0303 	bic.w	r3, r3, #3
3000791a:	f043 0302 	orr.w	r3, r3, #2
3000791e:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007922:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007926:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000792a:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000792e:	bd38      	pop	{r3, r4, r5, pc}
30007930:	42008000 	.word	0x42008000
30007934:	41011000 	.word	0x41011000
30007938:	00009b2d 	.word	0x00009b2d
3000793c:	90161f00 	.word	0x90161f00

30007940 <DDR_PHY_R240_ZQ_CAL>:
30007940:	b538      	push	{r3, r4, r5, lr}
30007942:	4b1e      	ldr	r3, [pc, #120]	; (300079bc <DDR_PHY_R240_ZQ_CAL+0x7c>)
30007944:	7818      	ldrb	r0, [r3, #0]
30007946:	28ff      	cmp	r0, #255	; 0xff
30007948:	d034      	beq.n	300079b4 <DDR_PHY_R240_ZQ_CAL+0x74>
3000794a:	4a1d      	ldr	r2, [pc, #116]	; (300079c0 <DDR_PHY_R240_ZQ_CAL+0x80>)
3000794c:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30007950:	4b1c      	ldr	r3, [pc, #112]	; (300079c4 <DDR_PHY_R240_ZQ_CAL+0x84>)
30007952:	2400      	movs	r4, #0
30007954:	eb02 0180 	add.w	r1, r2, r0, lsl #2
30007958:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
3000795c:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
30007960:	7c0a      	ldrb	r2, [r1, #16]
30007962:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
30007966:	0052      	lsls	r2, r2, #1
30007968:	f002 023e 	and.w	r2, r2, #62	; 0x3e
3000796c:	4302      	orrs	r2, r0
3000796e:	6848      	ldr	r0, [r1, #4]
30007970:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
30007974:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
30007978:	4a13      	ldr	r2, [pc, #76]	; (300079c8 <DDR_PHY_R240_ZQ_CAL+0x88>)
3000797a:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
3000797e:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30007982:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
30007986:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
3000798a:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
3000798e:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
30007992:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
30007996:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
3000799a:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000799e:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
300079a2:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300079a6:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300079aa:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
300079ae:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300079b2:	bd38      	pop	{r3, r4, r5, pc}
300079b4:	f7ff fdea 	bl	3000758c <DDR_PHY_ChipInfo.part.0>
300079b8:	e7c7      	b.n	3000794a <DDR_PHY_R240_ZQ_CAL+0xa>
300079ba:	bf00      	nop
300079bc:	3000e629 	.word	0x3000e629
300079c0:	3000cc9c 	.word	0x3000cc9c
300079c4:	41011000 	.word	0x41011000
300079c8:	00262626 	.word	0x00262626

300079cc <DDR_PHY_ZQ_SET_SEL>:
300079cc:	b410      	push	{r4}
300079ce:	4b25      	ldr	r3, [pc, #148]	; (30007a64 <DDR_PHY_ZQ_SET_SEL+0x98>)
300079d0:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300079d4:	4a24      	ldr	r2, [pc, #144]	; (30007a68 <DDR_PHY_ZQ_SET_SEL+0x9c>)
300079d6:	4c25      	ldr	r4, [pc, #148]	; (30007a6c <DDR_PHY_ZQ_SET_SEL+0xa0>)
300079d8:	4925      	ldr	r1, [pc, #148]	; (30007a70 <DDR_PHY_ZQ_SET_SEL+0xa4>)
300079da:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
300079de:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
300079e2:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
300079e6:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
300079ea:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
300079ee:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
300079f2:	f64f 74ff 	movw	r4, #65535	; 0xffff
300079f6:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
300079fa:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
300079fe:	2200      	movs	r2, #0
30007a00:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30007a04:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
30007a08:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30007a0c:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
30007a10:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30007a14:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30007a18:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30007a1c:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
30007a20:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30007a24:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30007a28:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30007a2c:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
30007a30:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30007a34:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30007a38:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30007a3c:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
30007a40:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
30007a44:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
30007a48:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30007a4c:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
30007a50:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
30007a54:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
30007a58:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30007a5c:	f85d 4b04 	ldr.w	r4, [sp], #4
30007a60:	4770      	bx	lr
30007a62:	bf00      	nop
30007a64:	41011000 	.word	0x41011000
30007a68:	ffff1111 	.word	0xffff1111
30007a6c:	ffff3333 	.word	0xffff3333
30007a70:	ffff0000 	.word	0xffff0000

30007a74 <DDR_PHY_DELAY_TAP_SET>:
30007a74:	b510      	push	{r4, lr}
30007a76:	4b15      	ldr	r3, [pc, #84]	; (30007acc <DDR_PHY_DELAY_TAP_SET+0x58>)
30007a78:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
30007a7c:	4914      	ldr	r1, [pc, #80]	; (30007ad0 <DDR_PHY_DELAY_TAP_SET+0x5c>)
30007a7e:	4c15      	ldr	r4, [pc, #84]	; (30007ad4 <DDR_PHY_DELAY_TAP_SET+0x60>)
30007a80:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
30007a84:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
30007a88:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30007a8c:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
30007a90:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30007a94:	7820      	ldrb	r0, [r4, #0]
30007a96:	b1a0      	cbz	r0, 30007ac2 <DDR_PHY_DELAY_TAP_SET+0x4e>
30007a98:	2801      	cmp	r0, #1
30007a9a:	4b0c      	ldr	r3, [pc, #48]	; (30007acc <DDR_PHY_DELAY_TAP_SET+0x58>)
30007a9c:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
30007aa0:	bf0c      	ite	eq
30007aa2:	4a0d      	ldreq	r2, [pc, #52]	; (30007ad8 <DDR_PHY_DELAY_TAP_SET+0x64>)
30007aa4:	4a0d      	ldrne	r2, [pc, #52]	; (30007adc <DDR_PHY_DELAY_TAP_SET+0x68>)
30007aa6:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
30007aaa:	f640 0288 	movw	r2, #2184	; 0x888
30007aae:	4b07      	ldr	r3, [pc, #28]	; (30007acc <DDR_PHY_DELAY_TAP_SET+0x58>)
30007ab0:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30007ab4:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30007ab8:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30007abc:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30007ac0:	bd10      	pop	{r4, pc}
30007ac2:	4b07      	ldr	r3, [pc, #28]	; (30007ae0 <DDR_PHY_DELAY_TAP_SET+0x6c>)
30007ac4:	4798      	blx	r3
30007ac6:	7020      	strb	r0, [r4, #0]
30007ac8:	e7e6      	b.n	30007a98 <DDR_PHY_DELAY_TAP_SET+0x24>
30007aca:	bf00      	nop
30007acc:	41011000 	.word	0x41011000
30007ad0:	20888888 	.word	0x20888888
30007ad4:	3000f330 	.word	0x3000f330
30007ad8:	7c033333 	.word	0x7c033333
30007adc:	7c011111 	.word	0x7c011111
30007ae0:	30007461 	.word	0x30007461

30007ae4 <DDR_PHY_READ_CTRL>:
30007ae4:	4b10      	ldr	r3, [pc, #64]	; (30007b28 <DDR_PHY_READ_CTRL+0x44>)
30007ae6:	b510      	push	{r4, lr}
30007ae8:	7818      	ldrb	r0, [r3, #0]
30007aea:	28ff      	cmp	r0, #255	; 0xff
30007aec:	d019      	beq.n	30007b22 <DDR_PHY_READ_CTRL+0x3e>
30007aee:	4a0f      	ldr	r2, [pc, #60]	; (30007b2c <DDR_PHY_READ_CTRL+0x48>)
30007af0:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30007af4:	4b0e      	ldr	r3, [pc, #56]	; (30007b30 <DDR_PHY_READ_CTRL+0x4c>)
30007af6:	2108      	movs	r1, #8
30007af8:	eb02 0080 	add.w	r0, r2, r0, lsl #2
30007afc:	22fe      	movs	r2, #254	; 0xfe
30007afe:	7c44      	ldrb	r4, [r0, #17]
30007b00:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
30007b04:	7c84      	ldrb	r4, [r0, #18]
30007b06:	7cc0      	ldrb	r0, [r0, #19]
30007b08:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
30007b0c:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
30007b10:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30007b14:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
30007b18:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30007b1c:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
30007b20:	bd10      	pop	{r4, pc}
30007b22:	f7ff fd33 	bl	3000758c <DDR_PHY_ChipInfo.part.0>
30007b26:	e7e2      	b.n	30007aee <DDR_PHY_READ_CTRL+0xa>
30007b28:	3000e629 	.word	0x3000e629
30007b2c:	3000cc9c 	.word	0x3000cc9c
30007b30:	41011000 	.word	0x41011000

30007b34 <DDR_PHY_READ_LEVELING>:
30007b34:	b508      	push	{r3, lr}
30007b36:	4b21      	ldr	r3, [pc, #132]	; (30007bbc <DDR_PHY_READ_LEVELING+0x88>)
30007b38:	7818      	ldrb	r0, [r3, #0]
30007b3a:	28ff      	cmp	r0, #255	; 0xff
30007b3c:	d03b      	beq.n	30007bb6 <DDR_PHY_READ_LEVELING+0x82>
30007b3e:	4b20      	ldr	r3, [pc, #128]	; (30007bc0 <DDR_PHY_READ_LEVELING+0x8c>)
30007b40:	b1e8      	cbz	r0, 30007b7e <DDR_PHY_READ_LEVELING+0x4a>
30007b42:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
30007b46:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30007b4a:	220f      	movs	r2, #15
30007b4c:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
30007b50:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
30007b54:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
30007b58:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
30007b5c:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
30007b60:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
30007b64:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
30007b68:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
30007b6c:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
30007b70:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30007b74:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30007b78:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
30007b7c:	bd08      	pop	{r3, pc}
30007b7e:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
30007b82:	2110      	movs	r1, #16
30007b84:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30007b88:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30007b8c:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30007b90:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30007b94:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30007b98:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30007b9c:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30007ba0:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30007ba4:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30007ba8:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30007bac:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30007bb0:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30007bb4:	bd08      	pop	{r3, pc}
30007bb6:	f7ff fce9 	bl	3000758c <DDR_PHY_ChipInfo.part.0>
30007bba:	e7c0      	b.n	30007b3e <DDR_PHY_READ_LEVELING+0xa>
30007bbc:	3000e629 	.word	0x3000e629
30007bc0:	41011000 	.word	0x41011000

30007bc4 <DDR_PHY_Init>:
30007bc4:	b510      	push	{r4, lr}
30007bc6:	6d80      	ldr	r0, [r0, #88]	; 0x58
30007bc8:	4c2f      	ldr	r4, [pc, #188]	; (30007c88 <DDR_PHY_Init+0xc4>)
30007bca:	f7ff fd43 	bl	30007654 <DDR_PHY_DLL_CLK_DIV>
30007bce:	f7ff fe49 	bl	30007864 <DDR_PHY_CRT_Init>
30007bd2:	f7ff feb5 	bl	30007940 <DDR_PHY_R240_ZQ_CAL>
30007bd6:	f7ff fef9 	bl	300079cc <DDR_PHY_ZQ_SET_SEL>
30007bda:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30007bde:	2300      	movs	r3, #0
30007be0:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30007be4:	4829      	ldr	r0, [pc, #164]	; (30007c8c <DDR_PHY_Init+0xc8>)
30007be6:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30007bea:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
30007bee:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
30007bf2:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
30007bf6:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30007bfa:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
30007bfe:	f7ff ff39 	bl	30007a74 <DDR_PHY_DELAY_TAP_SET>
30007c02:	f7ff ff6f 	bl	30007ae4 <DDR_PHY_READ_CTRL>
30007c06:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30007c0a:	4a21      	ldr	r2, [pc, #132]	; (30007c90 <DDR_PHY_Init+0xcc>)
30007c0c:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
30007c10:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30007c14:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30007c18:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30007c1c:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30007c20:	f7ff ff88 	bl	30007b34 <DDR_PHY_READ_LEVELING>
30007c24:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30007c28:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30007c2c:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30007c30:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30007c34:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30007c38:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30007c3c:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30007c40:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30007c44:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30007c48:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30007c4c:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30007c50:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30007c54:	f043 0303 	orr.w	r3, r3, #3
30007c58:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30007c5c:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30007c60:	f043 030c 	orr.w	r3, r3, #12
30007c64:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30007c68:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007c6c:	f023 0303 	bic.w	r3, r3, #3
30007c70:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007c74:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007c78:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
30007c7c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30007c80:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007c84:	bd10      	pop	{r4, pc}
30007c86:	bf00      	nop
30007c88:	41011000 	.word	0x41011000
30007c8c:	00045500 	.word	0x00045500
30007c90:	11117777 	.word	0x11117777

30007c94 <DDR_PHY_CAL>:
30007c94:	4770      	bx	lr
30007c96:	bf00      	nop

30007c98 <DDR_PHY_AutoGating>:
30007c98:	4a0c      	ldr	r2, [pc, #48]	; (30007ccc <DDR_PHY_AutoGating+0x34>)
30007c9a:	2032      	movs	r0, #50	; 0x32
30007c9c:	490c      	ldr	r1, [pc, #48]	; (30007cd0 <DDR_PHY_AutoGating+0x38>)
30007c9e:	b508      	push	{r3, lr}
30007ca0:	6893      	ldr	r3, [r2, #8]
30007ca2:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30007ca6:	6093      	str	r3, [r2, #8]
30007ca8:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30007cac:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30007cb0:	f043 0314 	orr.w	r3, r3, #20
30007cb4:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30007cb8:	4788      	blx	r1
30007cba:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30007cbe:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30007cc2:	f043 0303 	orr.w	r3, r3, #3
30007cc6:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30007cca:	bd08      	pop	{r3, pc}
30007ccc:	41011000 	.word	0x41011000
30007cd0:	00009b2d 	.word	0x00009b2d

30007cd4 <flash_init_userdef>:
30007cd4:	4b14      	ldr	r3, [pc, #80]	; (30007d28 <flash_init_userdef+0x54>)
30007cd6:	2201      	movs	r2, #1
30007cd8:	2140      	movs	r1, #64	; 0x40
30007cda:	f44f 6000 	mov.w	r0, #2048	; 0x800
30007cde:	b430      	push	{r4, r5}
30007ce0:	e9c3 1203 	strd	r1, r2, [r3, #12]
30007ce4:	2502      	movs	r5, #2
30007ce6:	2400      	movs	r4, #0
30007ce8:	f640 0104 	movw	r1, #2052	; 0x804
30007cec:	601a      	str	r2, [r3, #0]
30007cee:	22bb      	movs	r2, #187	; 0xbb
30007cf0:	83d8      	strh	r0, [r3, #30]
30007cf2:	8419      	strh	r1, [r3, #32]
30007cf4:	203b      	movs	r0, #59	; 0x3b
30007cf6:	21eb      	movs	r1, #235	; 0xeb
30007cf8:	629a      	str	r2, [r3, #40]	; 0x28
30007cfa:	4a0c      	ldr	r2, [pc, #48]	; (30007d2c <flash_init_userdef+0x58>)
30007cfc:	6258      	str	r0, [r3, #36]	; 0x24
30007cfe:	6319      	str	r1, [r3, #48]	; 0x30
30007d00:	f64a 3120 	movw	r1, #43808	; 0xab20
30007d04:	655a      	str	r2, [r3, #84]	; 0x54
30007d06:	22b9      	movs	r2, #185	; 0xb9
30007d08:	4809      	ldr	r0, [pc, #36]	; (30007d30 <flash_init_userdef+0x5c>)
30007d0a:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30007d0e:	6598      	str	r0, [r3, #88]	; 0x58
30007d10:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30007d14:	e9c3 5405 	strd	r5, r4, [r3, #20]
30007d18:	2406      	movs	r4, #6
30007d1a:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30007d1e:	246b      	movs	r4, #107	; 0x6b
30007d20:	62dc      	str	r4, [r3, #44]	; 0x2c
30007d22:	bc30      	pop	{r4, r5}
30007d24:	4770      	bx	lr
30007d26:	bf00      	nop
30007d28:	2001c01c 	.word	0x2001c01c
30007d2c:	00059f06 	.word	0x00059f06
30007d30:	d8600001 	.word	0xd8600001

30007d34 <nand_init_userdef>:
30007d34:	4b18      	ldr	r3, [pc, #96]	; (30007d98 <nand_init_userdef+0x64>)
30007d36:	2001      	movs	r0, #1
30007d38:	2200      	movs	r2, #0
30007d3a:	2102      	movs	r1, #2
30007d3c:	b410      	push	{r4}
30007d3e:	6018      	str	r0, [r3, #0]
30007d40:	f640 0408 	movw	r4, #2056	; 0x808
30007d44:	6159      	str	r1, [r3, #20]
30007d46:	83dc      	strh	r4, [r3, #30]
30007d48:	f44f 6400 	mov.w	r4, #2048	; 0x800
30007d4c:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30007d50:	841c      	strh	r4, [r3, #32]
30007d52:	246b      	movs	r4, #107	; 0x6b
30007d54:	619a      	str	r2, [r3, #24]
30007d56:	62dc      	str	r4, [r3, #44]	; 0x2c
30007d58:	4c10      	ldr	r4, [pc, #64]	; (30007d9c <nand_init_userdef+0x68>)
30007d5a:	775a      	strb	r2, [r3, #29]
30007d5c:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30007d60:	629a      	str	r2, [r3, #40]	; 0x28
30007d62:	631a      	str	r2, [r3, #48]	; 0x30
30007d64:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30007d68:	65da      	str	r2, [r3, #92]	; 0x5c
30007d6a:	e9c3 0003 	strd	r0, r0, [r3, #12]
30007d6e:	203b      	movs	r0, #59	; 0x3b
30007d70:	6258      	str	r0, [r3, #36]	; 0x24
30007d72:	200b      	movs	r0, #11
30007d74:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30007d78:	6698      	str	r0, [r3, #104]	; 0x68
30007d7a:	2008      	movs	r0, #8
30007d7c:	4908      	ldr	r1, [pc, #32]	; (30007da0 <nand_init_userdef+0x6c>)
30007d7e:	64d8      	str	r0, [r3, #76]	; 0x4c
30007d80:	f241 0013 	movw	r0, #4115	; 0x1013
30007d84:	6599      	str	r1, [r3, #88]	; 0x58
30007d86:	4907      	ldr	r1, [pc, #28]	; (30007da4 <nand_init_userdef+0x70>)
30007d88:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30007d8c:	6719      	str	r1, [r3, #112]	; 0x70
30007d8e:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30007d92:	f85d 4b04 	ldr.w	r4, [sp], #4
30007d96:	4770      	bx	lr
30007d98:	2001c01c 	.word	0x2001c01c
30007d9c:	000f9f06 	.word	0x000f9f06
30007da0:	d800001f 	.word	0xd800001f
30007da4:	10300804 	.word	0x10300804

30007da8 <flash_get_layout_info>:
30007da8:	b570      	push	{r4, r5, r6, lr}
30007daa:	4b0f      	ldr	r3, [pc, #60]	; (30007de8 <flash_get_layout_info+0x40>)
30007dac:	4615      	mov	r5, r2
30007dae:	4604      	mov	r4, r0
30007db0:	460e      	mov	r6, r1
30007db2:	4798      	blx	r3
30007db4:	4b0d      	ldr	r3, [pc, #52]	; (30007dec <flash_get_layout_info+0x44>)
30007db6:	4a0e      	ldr	r2, [pc, #56]	; (30007df0 <flash_get_layout_info+0x48>)
30007db8:	2800      	cmp	r0, #0
30007dba:	bf14      	ite	ne
30007dbc:	469c      	movne	ip, r3
30007dbe:	4694      	moveq	ip, r2
30007dc0:	f8dc 3000 	ldr.w	r3, [ip]
30007dc4:	2bff      	cmp	r3, #255	; 0xff
30007dc6:	d104      	bne.n	30007dd2 <flash_get_layout_info+0x2a>
30007dc8:	e00d      	b.n	30007de6 <flash_get_layout_info+0x3e>
30007dca:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
30007dce:	2bff      	cmp	r3, #255	; 0xff
30007dd0:	d009      	beq.n	30007de6 <flash_get_layout_info+0x3e>
30007dd2:	429c      	cmp	r4, r3
30007dd4:	d1f9      	bne.n	30007dca <flash_get_layout_info+0x22>
30007dd6:	b116      	cbz	r6, 30007dde <flash_get_layout_info+0x36>
30007dd8:	f8dc 3004 	ldr.w	r3, [ip, #4]
30007ddc:	6033      	str	r3, [r6, #0]
30007dde:	b115      	cbz	r5, 30007de6 <flash_get_layout_info+0x3e>
30007de0:	f8dc 3008 	ldr.w	r3, [ip, #8]
30007de4:	602b      	str	r3, [r5, #0]
30007de6:	bd70      	pop	{r4, r5, r6, pc}
30007de8:	3000921d 	.word	0x3000921d
30007dec:	3000e704 	.word	0x3000e704
30007df0:	3000e62c 	.word	0x3000e62c

30007df4 <NAND_CHECK_IS_BAD_BLOCK>:
30007df4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007df8:	4e24      	ldr	r6, [pc, #144]	; (30007e8c <NAND_CHECK_IS_BAD_BLOCK+0x98>)
30007dfa:	0907      	lsrs	r7, r0, #4
30007dfc:	0045      	lsls	r5, r0, #1
30007dfe:	b085      	sub	sp, #20
30007e00:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30007e04:	f005 051e 	and.w	r5, r5, #30
30007e08:	40eb      	lsrs	r3, r5
30007e0a:	f003 0303 	and.w	r3, r3, #3
30007e0e:	2b02      	cmp	r3, #2
30007e10:	d02c      	beq.n	30007e6c <NAND_CHECK_IS_BAD_BLOCK+0x78>
30007e12:	2b01      	cmp	r3, #1
30007e14:	d026      	beq.n	30007e64 <NAND_CHECK_IS_BAD_BLOCK+0x70>
30007e16:	4b1e      	ldr	r3, [pc, #120]	; (30007e90 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30007e18:	f04f 0801 	mov.w	r8, #1
30007e1c:	2204      	movs	r2, #4
30007e1e:	4604      	mov	r4, r0
30007e20:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30007e24:	0180      	lsls	r0, r0, #6
30007e26:	ab03      	add	r3, sp, #12
30007e28:	f8df 9070 	ldr.w	r9, [pc, #112]	; 30007e9c <NAND_CHECK_IS_BAD_BLOCK+0xa8>
30007e2c:	fa08 f101 	lsl.w	r1, r8, r1
30007e30:	47c8      	blx	r9
30007e32:	f89d 300c 	ldrb.w	r3, [sp, #12]
30007e36:	f89d 200d 	ldrb.w	r2, [sp, #13]
30007e3a:	4013      	ands	r3, r2
30007e3c:	2bff      	cmp	r3, #255	; 0xff
30007e3e:	d019      	beq.n	30007e74 <NAND_CHECK_IS_BAD_BLOCK+0x80>
30007e40:	2002      	movs	r0, #2
30007e42:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
30007e46:	4b13      	ldr	r3, [pc, #76]	; (30007e94 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30007e48:	fa00 f505 	lsl.w	r5, r0, r5
30007e4c:	9400      	str	r4, [sp, #0]
30007e4e:	4619      	mov	r1, r3
30007e50:	432a      	orrs	r2, r5
30007e52:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
30007e56:	4a10      	ldr	r2, [pc, #64]	; (30007e98 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30007e58:	f003 fb7c 	bl	3000b554 <rtk_log_write>
30007e5c:	4640      	mov	r0, r8
30007e5e:	b005      	add	sp, #20
30007e60:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007e64:	2000      	movs	r0, #0
30007e66:	b005      	add	sp, #20
30007e68:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007e6c:	2001      	movs	r0, #1
30007e6e:	b005      	add	sp, #20
30007e70:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007e74:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30007e78:	fa08 f005 	lsl.w	r0, r8, r5
30007e7c:	4303      	orrs	r3, r0
30007e7e:	2000      	movs	r0, #0
30007e80:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30007e84:	b005      	add	sp, #20
30007e86:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007e8a:	bf00      	nop
30007e8c:	3000f338 	.word	0x3000f338
30007e90:	2001c01c 	.word	0x2001c01c
30007e94:	3000cf30 	.word	0x3000cf30
30007e98:	3000cf3c 	.word	0x3000cf3c
30007e9c:	300091ed 	.word	0x300091ed

30007ea0 <Nand_Get_NandAddr>:
30007ea0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007ea4:	4f10      	ldr	r7, [pc, #64]	; (30007ee8 <Nand_Get_NandAddr+0x48>)
30007ea6:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30007eaa:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30007eae:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30007eb2:	40e8      	lsrs	r0, r5
30007eb4:	fa26 f505 	lsr.w	r5, r6, r5
30007eb8:	09ad      	lsrs	r5, r5, #6
30007eba:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30007ebe:	d90f      	bls.n	30007ee0 <Nand_Get_NandAddr+0x40>
30007ec0:	0984      	lsrs	r4, r0, #6
30007ec2:	f04f 0840 	mov.w	r8, #64	; 0x40
30007ec6:	4620      	mov	r0, r4
30007ec8:	3401      	adds	r4, #1
30007eca:	f7ff ff93 	bl	30007df4 <NAND_CHECK_IS_BAD_BLOCK>
30007ece:	b128      	cbz	r0, 30007edc <Nand_Get_NandAddr+0x3c>
30007ed0:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007ed4:	3501      	adds	r5, #1
30007ed6:	fa08 f303 	lsl.w	r3, r8, r3
30007eda:	441e      	add	r6, r3
30007edc:	42a5      	cmp	r5, r4
30007ede:	d8f2      	bhi.n	30007ec6 <Nand_Get_NandAddr+0x26>
30007ee0:	4630      	mov	r0, r6
30007ee2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007ee6:	bf00      	nop
30007ee8:	2001c01c 	.word	0x2001c01c

30007eec <Nand_L2P_Table>:
30007eec:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007ef0:	b082      	sub	sp, #8
30007ef2:	4604      	mov	r4, r0
30007ef4:	2002      	movs	r0, #2
30007ef6:	aa01      	add	r2, sp, #4
30007ef8:	4669      	mov	r1, sp
30007efa:	f7ff ff55 	bl	30007da8 <flash_get_layout_info>
30007efe:	9800      	ldr	r0, [sp, #0]
30007f00:	42a0      	cmp	r0, r4
30007f02:	d810      	bhi.n	30007f26 <Nand_L2P_Table+0x3a>
30007f04:	9b01      	ldr	r3, [sp, #4]
30007f06:	42a3      	cmp	r3, r4
30007f08:	d805      	bhi.n	30007f16 <Nand_L2P_Table+0x2a>
30007f0a:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30007f0e:	4620      	mov	r0, r4
30007f10:	b002      	add	sp, #8
30007f12:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007f16:	4621      	mov	r1, r4
30007f18:	f7ff ffc2 	bl	30007ea0 <Nand_Get_NandAddr>
30007f1c:	4604      	mov	r4, r0
30007f1e:	4620      	mov	r0, r4
30007f20:	b002      	add	sp, #8
30007f22:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007f26:	4f0e      	ldr	r7, [pc, #56]	; (30007f60 <Nand_L2P_Table+0x74>)
30007f28:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30007f2c:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
30007f30:	fa24 f606 	lsr.w	r6, r4, r6
30007f34:	09b6      	lsrs	r6, r6, #6
30007f36:	d0ea      	beq.n	30007f0e <Nand_L2P_Table+0x22>
30007f38:	2500      	movs	r5, #0
30007f3a:	f04f 0840 	mov.w	r8, #64	; 0x40
30007f3e:	4628      	mov	r0, r5
30007f40:	3501      	adds	r5, #1
30007f42:	f7ff ff57 	bl	30007df4 <NAND_CHECK_IS_BAD_BLOCK>
30007f46:	b128      	cbz	r0, 30007f54 <Nand_L2P_Table+0x68>
30007f48:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007f4c:	3601      	adds	r6, #1
30007f4e:	fa08 f303 	lsl.w	r3, r8, r3
30007f52:	441c      	add	r4, r3
30007f54:	42b5      	cmp	r5, r6
30007f56:	d3f2      	bcc.n	30007f3e <Nand_L2P_Table+0x52>
30007f58:	4620      	mov	r0, r4
30007f5a:	b002      	add	sp, #8
30007f5c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007f60:	2001c01c 	.word	0x2001c01c

30007f64 <NandImgCopy>:
30007f64:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007f68:	4617      	mov	r7, r2
30007f6a:	4603      	mov	r3, r0
30007f6c:	b089      	sub	sp, #36	; 0x24
30007f6e:	f8df 9100 	ldr.w	r9, [pc, #256]	; 30008070 <NandImgCopy+0x10c>
30007f72:	f107 38ff 	add.w	r8, r7, #4294967295	; 0xffffffff
30007f76:	4608      	mov	r0, r1
30007f78:	9307      	str	r3, [sp, #28]
30007f7a:	f7ff ffb7 	bl	30007eec <Nand_L2P_Table>
30007f7e:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30007f82:	4480      	add	r8, r0
30007f84:	fa20 f406 	lsr.w	r4, r0, r6
30007f88:	fa28 f806 	lsr.w	r8, r8, r6
30007f8c:	eba8 0804 	sub.w	r8, r8, r4
30007f90:	f1b8 3fff 	cmp.w	r8, #4294967295	; 0xffffffff
30007f94:	d054      	beq.n	30008040 <NandImgCopy+0xdc>
30007f96:	2300      	movs	r3, #0
30007f98:	4605      	mov	r5, r0
30007f9a:	f04f 0b40 	mov.w	fp, #64	; 0x40
30007f9e:	469a      	mov	sl, r3
30007fa0:	9306      	str	r3, [sp, #24]
30007fa2:	463b      	mov	r3, r7
30007fa4:	4647      	mov	r7, r8
30007fa6:	4698      	mov	r8, r3
30007fa8:	e020      	b.n	30007fec <NandImgCopy+0x88>
30007faa:	f1c2 0320 	rsb	r3, r2, #32
30007fae:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30007fb2:	40d9      	lsrs	r1, r3
30007fb4:	9b07      	ldr	r3, [sp, #28]
30007fb6:	4029      	ands	r1, r5
30007fb8:	4453      	add	r3, sl
30007fba:	b377      	cbz	r7, 3000801a <NandImgCopy+0xb6>
30007fbc:	2601      	movs	r6, #1
30007fbe:	4620      	mov	r0, r4
30007fc0:	9105      	str	r1, [sp, #20]
30007fc2:	fa06 f202 	lsl.w	r2, r6, r2
30007fc6:	4e27      	ldr	r6, [pc, #156]	; (30008064 <NandImgCopy+0x100>)
30007fc8:	9204      	str	r2, [sp, #16]
30007fca:	1a52      	subs	r2, r2, r1
30007fcc:	4492      	add	sl, r2
30007fce:	4415      	add	r5, r2
30007fd0:	47b0      	blx	r6
30007fd2:	f010 0f70 	tst.w	r0, #112	; 0x70
30007fd6:	9b04      	ldr	r3, [sp, #16]
30007fd8:	9905      	ldr	r1, [sp, #20]
30007fda:	d134      	bne.n	30008046 <NandImgCopy+0xe2>
30007fdc:	eba8 0803 	sub.w	r8, r8, r3
30007fe0:	4488      	add	r8, r1
30007fe2:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30007fe6:	3f01      	subs	r7, #1
30007fe8:	fa25 f406 	lsr.w	r4, r5, r6
30007fec:	9b06      	ldr	r3, [sp, #24]
30007fee:	4632      	mov	r2, r6
30007ff0:	09a6      	lsrs	r6, r4, #6
30007ff2:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30007ff6:	d8d8      	bhi.n	30007faa <NandImgCopy+0x46>
30007ff8:	e005      	b.n	30008006 <NandImgCopy+0xa2>
30007ffa:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30007ffe:	3440      	adds	r4, #64	; 0x40
30008000:	fa0b f303 	lsl.w	r3, fp, r3
30008004:	441d      	add	r5, r3
30008006:	4630      	mov	r0, r6
30008008:	3601      	adds	r6, #1
3000800a:	f7ff fef3 	bl	30007df4 <NAND_CHECK_IS_BAD_BLOCK>
3000800e:	2800      	cmp	r0, #0
30008010:	d1f3      	bne.n	30007ffa <NandImgCopy+0x96>
30008012:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008016:	9606      	str	r6, [sp, #24]
30008018:	e7c7      	b.n	30007faa <NandImgCopy+0x46>
3000801a:	4642      	mov	r2, r8
3000801c:	4620      	mov	r0, r4
3000801e:	4d11      	ldr	r5, [pc, #68]	; (30008064 <NandImgCopy+0x100>)
30008020:	47a8      	blx	r5
30008022:	f010 0f70 	tst.w	r0, #112	; 0x70
30008026:	d00b      	beq.n	30008040 <NandImgCopy+0xdc>
30008028:	09a1      	lsrs	r1, r4, #6
3000802a:	4b0f      	ldr	r3, [pc, #60]	; (30008068 <NandImgCopy+0x104>)
3000802c:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008030:	4a0e      	ldr	r2, [pc, #56]	; (3000806c <NandImgCopy+0x108>)
30008032:	e9cd 1001 	strd	r1, r0, [sp, #4]
30008036:	9400      	str	r4, [sp, #0]
30008038:	4619      	mov	r1, r3
3000803a:	2004      	movs	r0, #4
3000803c:	f003 fa8a 	bl	3000b554 <rtk_log_write>
30008040:	b009      	add	sp, #36	; 0x24
30008042:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008046:	1ac9      	subs	r1, r1, r3
30008048:	09a3      	lsrs	r3, r4, #6
3000804a:	f004 043f 	and.w	r4, r4, #63	; 0x3f
3000804e:	9002      	str	r0, [sp, #8]
30008050:	9301      	str	r3, [sp, #4]
30008052:	4488      	add	r8, r1
30008054:	4b04      	ldr	r3, [pc, #16]	; (30008068 <NandImgCopy+0x104>)
30008056:	2004      	movs	r0, #4
30008058:	4a04      	ldr	r2, [pc, #16]	; (3000806c <NandImgCopy+0x108>)
3000805a:	4619      	mov	r1, r3
3000805c:	9400      	str	r4, [sp, #0]
3000805e:	f003 fa79 	bl	3000b554 <rtk_log_write>
30008062:	e7be      	b.n	30007fe2 <NandImgCopy+0x7e>
30008064:	300091ed 	.word	0x300091ed
30008068:	3000cf30 	.word	0x3000cf30
3000806c:	3000cf54 	.word	0x3000cf54
30008070:	2001c01c 	.word	0x2001c01c

30008074 <OTP_Read8.part.0>:
30008074:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008078:	4b30      	ldr	r3, [pc, #192]	; (3000813c <OTP_Read8.part.0+0xc8>)
3000807a:	4680      	mov	r8, r0
3000807c:	e843 f300 	tt	r3, r3
30008080:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008084:	4a2e      	ldr	r2, [pc, #184]	; (30008140 <OTP_Read8.part.0+0xcc>)
30008086:	4e2f      	ldr	r6, [pc, #188]	; (30008144 <OTP_Read8.part.0+0xd0>)
30008088:	b082      	sub	sp, #8
3000808a:	2b00      	cmp	r3, #0
3000808c:	4689      	mov	r9, r1
3000808e:	bf0e      	itee	eq
30008090:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008094:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008098:	4616      	movne	r6, r2
3000809a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000809c:	05d8      	lsls	r0, r3, #23
3000809e:	d505      	bpl.n	300080ac <OTP_Read8.part.0+0x38>
300080a0:	4d29      	ldr	r5, [pc, #164]	; (30008148 <OTP_Read8.part.0+0xd4>)
300080a2:	2064      	movs	r0, #100	; 0x64
300080a4:	47a8      	blx	r5
300080a6:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300080a8:	05d9      	lsls	r1, r3, #23
300080aa:	d4fa      	bmi.n	300080a2 <OTP_Read8.part.0+0x2e>
300080ac:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300080ae:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300080b2:	643b      	str	r3, [r7, #64]	; 0x40
300080b4:	4b25      	ldr	r3, [pc, #148]	; (3000814c <OTP_Read8.part.0+0xd8>)
300080b6:	681a      	ldr	r2, [r3, #0]
300080b8:	07d2      	lsls	r2, r2, #31
300080ba:	d403      	bmi.n	300080c4 <OTP_Read8.part.0+0x50>
300080bc:	681a      	ldr	r2, [r3, #0]
300080be:	f042 0201 	orr.w	r2, r2, #1
300080c2:	601a      	str	r2, [r3, #0]
300080c4:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
300080c8:	ea4f 2308 	mov.w	r3, r8, lsl #8
300080cc:	694a      	ldr	r2, [r1, #20]
300080ce:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
300080d2:	614a      	str	r2, [r1, #20]
300080d4:	6033      	str	r3, [r6, #0]
300080d6:	6833      	ldr	r3, [r6, #0]
300080d8:	2b00      	cmp	r3, #0
300080da:	db10      	blt.n	300080fe <OTP_Read8.part.0+0x8a>
300080dc:	2400      	movs	r4, #0
300080de:	4d1a      	ldr	r5, [pc, #104]	; (30008148 <OTP_Read8.part.0+0xd4>)
300080e0:	f644 6a20 	movw	sl, #20000	; 0x4e20
300080e4:	e001      	b.n	300080ea <OTP_Read8.part.0+0x76>
300080e6:	4554      	cmp	r4, sl
300080e8:	d01a      	beq.n	30008120 <OTP_Read8.part.0+0xac>
300080ea:	2005      	movs	r0, #5
300080ec:	3401      	adds	r4, #1
300080ee:	47a8      	blx	r5
300080f0:	6833      	ldr	r3, [r6, #0]
300080f2:	2b00      	cmp	r3, #0
300080f4:	daf7      	bge.n	300080e6 <OTP_Read8.part.0+0x72>
300080f6:	f644 6320 	movw	r3, #20000	; 0x4e20
300080fa:	429c      	cmp	r4, r3
300080fc:	d010      	beq.n	30008120 <OTP_Read8.part.0+0xac>
300080fe:	6833      	ldr	r3, [r6, #0]
30008100:	2001      	movs	r0, #1
30008102:	f889 3000 	strb.w	r3, [r9]
30008106:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
3000810a:	6953      	ldr	r3, [r2, #20]
3000810c:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008110:	6153      	str	r3, [r2, #20]
30008112:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008114:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008118:	643b      	str	r3, [r7, #64]	; 0x40
3000811a:	b002      	add	sp, #8
3000811c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008120:	22ff      	movs	r2, #255	; 0xff
30008122:	4b0b      	ldr	r3, [pc, #44]	; (30008150 <OTP_Read8.part.0+0xdc>)
30008124:	2002      	movs	r0, #2
30008126:	f8cd 8000 	str.w	r8, [sp]
3000812a:	4619      	mov	r1, r3
3000812c:	f889 2000 	strb.w	r2, [r9]
30008130:	4a08      	ldr	r2, [pc, #32]	; (30008154 <OTP_Read8.part.0+0xe0>)
30008132:	f003 fa0f 	bl	3000b554 <rtk_log_write>
30008136:	2000      	movs	r0, #0
30008138:	e7e5      	b.n	30008106 <OTP_Read8.part.0+0x92>
3000813a:	bf00      	nop
3000813c:	0000e5f9 	.word	0x0000e5f9
30008140:	5200000c 	.word	0x5200000c
30008144:	42000008 	.word	0x42000008
30008148:	00009b2d 	.word	0x00009b2d
3000814c:	42008000 	.word	0x42008000
30008150:	3000cf90 	.word	0x3000cf90
30008154:	3000cf94 	.word	0x3000cf94

30008158 <OTP_Read8>:
30008158:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
3000815c:	d201      	bcs.n	30008162 <OTP_Read8+0xa>
3000815e:	f7ff bf89 	b.w	30008074 <OTP_Read8.part.0>
30008162:	22ff      	movs	r2, #255	; 0xff
30008164:	2000      	movs	r0, #0
30008166:	700a      	strb	r2, [r1, #0]
30008168:	4770      	bx	lr
3000816a:	bf00      	nop

3000816c <PLL_NP_ClkSet>:
3000816c:	b570      	push	{r4, r5, r6, lr}
3000816e:	4604      	mov	r4, r0
30008170:	4b1b      	ldr	r3, [pc, #108]	; (300081e0 <PLL_NP_ClkSet+0x74>)
30008172:	4798      	blx	r3
30008174:	4d1b      	ldr	r5, [pc, #108]	; (300081e4 <PLL_NP_ClkSet+0x78>)
30008176:	08c2      	lsrs	r2, r0, #3
30008178:	fbb4 f3f0 	udiv	r3, r4, r0
3000817c:	fb00 4c13 	mls	ip, r0, r3, r4
30008180:	1e9c      	subs	r4, r3, #2
30008182:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008184:	0164      	lsls	r4, r4, #5
30008186:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
3000818a:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
3000818e:	431c      	orrs	r4, r3
30008190:	fbbc f3f2 	udiv	r3, ip, r2
30008194:	646c      	str	r4, [r5, #68]	; 0x44
30008196:	041c      	lsls	r4, r3, #16
30008198:	fb02 c013 	mls	r0, r2, r3, ip
3000819c:	2300      	movs	r3, #0
3000819e:	6cee      	ldr	r6, [r5, #76]	; 0x4c
300081a0:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
300081a4:	4619      	mov	r1, r3
300081a6:	fa1f fc86 	uxth.w	ip, r6
300081aa:	4e0f      	ldr	r6, [pc, #60]	; (300081e8 <PLL_NP_ClkSet+0x7c>)
300081ac:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
300081b0:	ea44 040c 	orr.w	r4, r4, ip
300081b4:	f003 fa58 	bl	3000b668 <__aeabi_uldivmod>
300081b8:	4603      	mov	r3, r0
300081ba:	2001      	movs	r0, #1
300081bc:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
300081c0:	64ec      	str	r4, [r5, #76]	; 0x4c
300081c2:	47b0      	blx	r6
300081c4:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300081c6:	203c      	movs	r0, #60	; 0x3c
300081c8:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
300081cc:	646b      	str	r3, [r5, #68]	; 0x44
300081ce:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300081d0:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300081d4:	646b      	str	r3, [r5, #68]	; 0x44
300081d6:	4633      	mov	r3, r6
300081d8:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
300081dc:	4718      	bx	r3
300081de:	bf00      	nop
300081e0:	000099f5 	.word	0x000099f5
300081e4:	42008800 	.word	0x42008800
300081e8:	00009b2d 	.word	0x00009b2d

300081ec <PLL_AP_ClkSet>:
300081ec:	b510      	push	{r4, lr}
300081ee:	4604      	mov	r4, r0
300081f0:	4b0b      	ldr	r3, [pc, #44]	; (30008220 <PLL_AP_ClkSet+0x34>)
300081f2:	4798      	blx	r3
300081f4:	fbb4 f0f0 	udiv	r0, r4, r0
300081f8:	f1a0 0319 	sub.w	r3, r0, #25
300081fc:	1e84      	subs	r4, r0, #2
300081fe:	2b0f      	cmp	r3, #15
30008200:	d807      	bhi.n	30008212 <PLL_AP_ClkSet+0x26>
30008202:	4b08      	ldr	r3, [pc, #32]	; (30008224 <PLL_AP_ClkSet+0x38>)
30008204:	b2e0      	uxtb	r0, r4
30008206:	695c      	ldr	r4, [r3, #20]
30008208:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
3000820c:	4320      	orrs	r0, r4
3000820e:	6158      	str	r0, [r3, #20]
30008210:	bd10      	pop	{r4, pc}
30008212:	f240 1181 	movw	r1, #385	; 0x181
30008216:	4804      	ldr	r0, [pc, #16]	; (30008228 <PLL_AP_ClkSet+0x3c>)
30008218:	f003 fbe2 	bl	3000b9e0 <__io_assert_failed_veneer>
3000821c:	e7f1      	b.n	30008202 <PLL_AP_ClkSet+0x16>
3000821e:	bf00      	nop
30008220:	000099f5 	.word	0x000099f5
30008224:	41000300 	.word	0x41000300
30008228:	3000cfb0 	.word	0x3000cfb0

3000822c <PLL_AP>:
3000822c:	2801      	cmp	r0, #1
3000822e:	b538      	push	{r3, r4, r5, lr}
30008230:	d009      	beq.n	30008246 <PLL_AP+0x1a>
30008232:	4b13      	ldr	r3, [pc, #76]	; (30008280 <PLL_AP+0x54>)
30008234:	681a      	ldr	r2, [r3, #0]
30008236:	f022 0209 	bic.w	r2, r2, #9
3000823a:	601a      	str	r2, [r3, #0]
3000823c:	681a      	ldr	r2, [r3, #0]
3000823e:	f022 0206 	bic.w	r2, r2, #6
30008242:	601a      	str	r2, [r3, #0]
30008244:	bd38      	pop	{r3, r4, r5, pc}
30008246:	4b0f      	ldr	r3, [pc, #60]	; (30008284 <PLL_AP+0x58>)
30008248:	6f9a      	ldr	r2, [r3, #120]	; 0x78
3000824a:	f012 0f05 	tst.w	r2, #5
3000824e:	d00f      	beq.n	30008270 <PLL_AP+0x44>
30008250:	4d0d      	ldr	r5, [pc, #52]	; (30008288 <PLL_AP+0x5c>)
30008252:	4c0b      	ldr	r4, [pc, #44]	; (30008280 <PLL_AP+0x54>)
30008254:	2001      	movs	r0, #1
30008256:	6823      	ldr	r3, [r4, #0]
30008258:	f043 0306 	orr.w	r3, r3, #6
3000825c:	6023      	str	r3, [r4, #0]
3000825e:	47a8      	blx	r5
30008260:	6823      	ldr	r3, [r4, #0]
30008262:	f043 0309 	orr.w	r3, r3, #9
30008266:	6023      	str	r3, [r4, #0]
30008268:	6863      	ldr	r3, [r4, #4]
3000826a:	2b00      	cmp	r3, #0
3000826c:	dafc      	bge.n	30008268 <PLL_AP+0x3c>
3000826e:	bd38      	pop	{r3, r4, r5, pc}
30008270:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008272:	20a0      	movs	r0, #160	; 0xa0
30008274:	4d04      	ldr	r5, [pc, #16]	; (30008288 <PLL_AP+0x5c>)
30008276:	f042 0207 	orr.w	r2, r2, #7
3000827a:	679a      	str	r2, [r3, #120]	; 0x78
3000827c:	47a8      	blx	r5
3000827e:	e7e8      	b.n	30008252 <PLL_AP+0x26>
30008280:	41000300 	.word	0x41000300
30008284:	42008800 	.word	0x42008800
30008288:	00009b2d 	.word	0x00009b2d

3000828c <PSRAM_INFO_Update>:
3000828c:	4b49      	ldr	r3, [pc, #292]	; (300083b4 <PSRAM_INFO_Update+0x128>)
3000828e:	494a      	ldr	r1, [pc, #296]	; (300083b8 <PSRAM_INFO_Update+0x12c>)
30008290:	4a4a      	ldr	r2, [pc, #296]	; (300083bc <PSRAM_INFO_Update+0x130>)
30008292:	b530      	push	{r4, r5, lr}
30008294:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
30008298:	b083      	sub	sp, #12
3000829a:	680d      	ldr	r5, [r1, #0]
3000829c:	f3c3 2302 	ubfx	r3, r3, #8, #3
300082a0:	4c47      	ldr	r4, [pc, #284]	; (300083c0 <PSRAM_INFO_Update+0x134>)
300082a2:	fb02 f505 	mul.w	r5, r2, r5
300082a6:	3301      	adds	r3, #1
300082a8:	fbb5 f5f3 	udiv	r5, r5, r3
300082ac:	086a      	lsrs	r2, r5, #1
300082ae:	4b45      	ldr	r3, [pc, #276]	; (300083c4 <PSRAM_INFO_Update+0x138>)
300082b0:	60e2      	str	r2, [r4, #12]
300082b2:	4798      	blx	r3
300082b4:	4b44      	ldr	r3, [pc, #272]	; (300083c8 <PSRAM_INFO_Update+0x13c>)
300082b6:	6020      	str	r0, [r4, #0]
300082b8:	4798      	blx	r3
300082ba:	4b44      	ldr	r3, [pc, #272]	; (300083cc <PSRAM_INFO_Update+0x140>)
300082bc:	220a      	movs	r2, #10
300082be:	60a0      	str	r0, [r4, #8]
300082c0:	6122      	str	r2, [r4, #16]
300082c2:	2220      	movs	r2, #32
300082c4:	fbb3 f3f5 	udiv	r3, r3, r5
300082c8:	6163      	str	r3, [r4, #20]
300082ca:	fbb2 f3f3 	udiv	r3, r2, r3
300082ce:	6822      	ldr	r2, [r4, #0]
300082d0:	6223      	str	r3, [r4, #32]
300082d2:	2a01      	cmp	r2, #1
300082d4:	68e3      	ldr	r3, [r4, #12]
300082d6:	4a3e      	ldr	r2, [pc, #248]	; (300083d0 <PSRAM_INFO_Update+0x144>)
300082d8:	d01d      	beq.n	30008316 <PSRAM_INFO_Update+0x8a>
300082da:	4293      	cmp	r3, r2
300082dc:	d810      	bhi.n	30008300 <PSRAM_INFO_Update+0x74>
300082de:	2304      	movs	r3, #4
300082e0:	2208      	movs	r2, #8
300082e2:	4618      	mov	r0, r3
300082e4:	e9c4 3206 	strd	r3, r2, [r4, #24]
300082e8:	4b3a      	ldr	r3, [pc, #232]	; (300083d4 <PSRAM_INFO_Update+0x148>)
300082ea:	493b      	ldr	r1, [pc, #236]	; (300083d8 <PSRAM_INFO_Update+0x14c>)
300082ec:	4a3b      	ldr	r2, [pc, #236]	; (300083dc <PSRAM_INFO_Update+0x150>)
300082ee:	f881 0110 	strb.w	r0, [r1, #272]	; 0x110
300082f2:	4619      	mov	r1, r3
300082f4:	2004      	movs	r0, #4
300082f6:	9500      	str	r5, [sp, #0]
300082f8:	f003 f92c 	bl	3000b554 <rtk_log_write>
300082fc:	b003      	add	sp, #12
300082fe:	bd30      	pop	{r4, r5, pc}
30008300:	4a37      	ldr	r2, [pc, #220]	; (300083e0 <PSRAM_INFO_Update+0x154>)
30008302:	4293      	cmp	r3, r2
30008304:	d918      	bls.n	30008338 <PSRAM_INFO_Update+0xac>
30008306:	4a37      	ldr	r2, [pc, #220]	; (300083e4 <PSRAM_INFO_Update+0x158>)
30008308:	4293      	cmp	r3, r2
3000830a:	d827      	bhi.n	3000835c <PSRAM_INFO_Update+0xd0>
3000830c:	2306      	movs	r3, #6
3000830e:	4618      	mov	r0, r3
30008310:	e9c4 3306 	strd	r3, r3, [r4, #24]
30008314:	e7e8      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
30008316:	4293      	cmp	r3, r2
30008318:	d908      	bls.n	3000832c <PSRAM_INFO_Update+0xa0>
3000831a:	4a31      	ldr	r2, [pc, #196]	; (300083e0 <PSRAM_INFO_Update+0x154>)
3000831c:	4293      	cmp	r3, r2
3000831e:	d811      	bhi.n	30008344 <PSRAM_INFO_Update+0xb8>
30008320:	2305      	movs	r3, #5
30008322:	220f      	movs	r2, #15
30008324:	4618      	mov	r0, r3
30008326:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000832a:	e7dd      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
3000832c:	2304      	movs	r3, #4
3000832e:	220f      	movs	r2, #15
30008330:	4618      	mov	r0, r3
30008332:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008336:	e7d7      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
30008338:	2305      	movs	r3, #5
3000833a:	2208      	movs	r2, #8
3000833c:	4618      	mov	r0, r3
3000833e:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008342:	e7d1      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
30008344:	4a27      	ldr	r2, [pc, #156]	; (300083e4 <PSRAM_INFO_Update+0x158>)
30008346:	4293      	cmp	r3, r2
30008348:	d918      	bls.n	3000837c <PSRAM_INFO_Update+0xf0>
3000834a:	4a27      	ldr	r2, [pc, #156]	; (300083e8 <PSRAM_INFO_Update+0x15c>)
3000834c:	4293      	cmp	r3, r2
3000834e:	d821      	bhi.n	30008394 <PSRAM_INFO_Update+0x108>
30008350:	2307      	movs	r3, #7
30008352:	2214      	movs	r2, #20
30008354:	4618      	mov	r0, r3
30008356:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000835a:	e7c5      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
3000835c:	4a22      	ldr	r2, [pc, #136]	; (300083e8 <PSRAM_INFO_Update+0x15c>)
3000835e:	4293      	cmp	r3, r2
30008360:	d912      	bls.n	30008388 <PSRAM_INFO_Update+0xfc>
30008362:	4a22      	ldr	r2, [pc, #136]	; (300083ec <PSRAM_INFO_Update+0x160>)
30008364:	4293      	cmp	r3, r2
30008366:	bf8c      	ite	hi
30008368:	2300      	movhi	r3, #0
3000836a:	2301      	movls	r3, #1
3000836c:	2804      	cmp	r0, #4
3000836e:	d100      	bne.n	30008372 <PSRAM_INFO_Update+0xe6>
30008370:	b953      	cbnz	r3, 30008388 <PSRAM_INFO_Update+0xfc>
30008372:	2803      	cmp	r0, #3
30008374:	d100      	bne.n	30008378 <PSRAM_INFO_Update+0xec>
30008376:	b9b3      	cbnz	r3, 300083a6 <PSRAM_INFO_Update+0x11a>
30008378:	7e20      	ldrb	r0, [r4, #24]
3000837a:	e7b5      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
3000837c:	2306      	movs	r3, #6
3000837e:	2212      	movs	r2, #18
30008380:	4618      	mov	r0, r3
30008382:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008386:	e7af      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
30008388:	2307      	movs	r3, #7
3000838a:	2206      	movs	r2, #6
3000838c:	4618      	mov	r0, r3
3000838e:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008392:	e7a9      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
30008394:	4a15      	ldr	r2, [pc, #84]	; (300083ec <PSRAM_INFO_Update+0x160>)
30008396:	4293      	cmp	r3, r2
30008398:	d8ee      	bhi.n	30008378 <PSRAM_INFO_Update+0xec>
3000839a:	2308      	movs	r3, #8
3000839c:	221c      	movs	r2, #28
3000839e:	4618      	mov	r0, r3
300083a0:	e9c4 3206 	strd	r3, r2, [r4, #24]
300083a4:	e7a0      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
300083a6:	230a      	movs	r3, #10
300083a8:	2206      	movs	r2, #6
300083aa:	4618      	mov	r0, r3
300083ac:	e9c4 3206 	strd	r3, r2, [r4, #24]
300083b0:	e79a      	b.n	300082e8 <PSRAM_INFO_Update+0x5c>
300083b2:	bf00      	nop
300083b4:	42008000 	.word	0x42008000
300083b8:	3000e614 	.word	0x3000e614
300083bc:	000f4240 	.word	0x000f4240
300083c0:	3000f340 	.word	0x3000f340
300083c4:	300074e5 	.word	0x300074e5
300083c8:	300074ad 	.word	0x300074ad
300083cc:	3b9aca00 	.word	0x3b9aca00
300083d0:	0632ea00 	.word	0x0632ea00
300083d4:	3000cfc0 	.word	0x3000cfc0
300083d8:	23020000 	.word	0x23020000
300083dc:	3000cfc8 	.word	0x3000cfc8
300083e0:	07ed6b40 	.word	0x07ed6b40
300083e4:	09e4f580 	.word	0x09e4f580
300083e8:	0bebc200 	.word	0x0bebc200
300083ec:	0ee6b280 	.word	0x0ee6b280

300083f0 <PSRAM_PHY_StructInit>:
300083f0:	2203      	movs	r2, #3
300083f2:	2301      	movs	r3, #1
300083f4:	210a      	movs	r1, #10
300083f6:	b410      	push	{r4}
300083f8:	e9c0 2202 	strd	r2, r2, [r0, #8]
300083fc:	2402      	movs	r4, #2
300083fe:	2207      	movs	r2, #7
30008400:	e9c0 3400 	strd	r3, r4, [r0]
30008404:	e9c0 1304 	strd	r1, r3, [r0, #16]
30008408:	f85d 4b04 	ldr.w	r4, [sp], #4
3000840c:	6182      	str	r2, [r0, #24]
3000840e:	4770      	bx	lr

30008410 <PSRAM_PHY_Init>:
30008410:	b538      	push	{r3, r4, r5, lr}
30008412:	4604      	mov	r4, r0
30008414:	6903      	ldr	r3, [r0, #16]
30008416:	4d14      	ldr	r5, [pc, #80]	; (30008468 <PSRAM_PHY_Init+0x58>)
30008418:	f003 031f 	and.w	r3, r3, #31
3000841c:	4913      	ldr	r1, [pc, #76]	; (3000846c <PSRAM_PHY_Init+0x5c>)
3000841e:	e9d0 0200 	ldrd	r0, r2, [r0]
30008422:	0612      	lsls	r2, r2, #24
30008424:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30008428:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
3000842c:	4313      	orrs	r3, r2
3000842e:	68a2      	ldr	r2, [r4, #8]
30008430:	0412      	lsls	r2, r2, #16
30008432:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008436:	4313      	orrs	r3, r2
30008438:	68e2      	ldr	r2, [r4, #12]
3000843a:	0212      	lsls	r2, r2, #8
3000843c:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30008440:	4313      	orrs	r3, r2
30008442:	606b      	str	r3, [r5, #4]
30008444:	4788      	blx	r1
30008446:	2801      	cmp	r0, #1
30008448:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
3000844c:	bf08      	it	eq
3000844e:	4a08      	ldreq	r2, [pc, #32]	; (30008470 <PSRAM_PHY_Init+0x60>)
30008450:	ea4f 1303 	mov.w	r3, r3, lsl #4
30008454:	f001 010f 	and.w	r1, r1, #15
30008458:	bf18      	it	ne
3000845a:	4a06      	ldrne	r2, [pc, #24]	; (30008474 <PSRAM_PHY_Init+0x64>)
3000845c:	b2db      	uxtb	r3, r3
3000845e:	430b      	orrs	r3, r1
30008460:	431a      	orrs	r2, r3
30008462:	61aa      	str	r2, [r5, #24]
30008464:	bd38      	pop	{r3, r4, r5, pc}
30008466:	bf00      	nop
30008468:	41002000 	.word	0x41002000
3000846c:	300074e5 	.word	0x300074e5
30008470:	80000e00 	.word	0x80000e00
30008474:	80000f00 	.word	0x80000f00

30008478 <PSRAM_CTRL_Init>:
30008478:	b470      	push	{r4, r5, r6}
3000847a:	4b4c      	ldr	r3, [pc, #304]	; (300085ac <PSRAM_CTRL_Init+0x134>)
3000847c:	2400      	movs	r4, #0
3000847e:	4a4c      	ldr	r2, [pc, #304]	; (300085b0 <PSRAM_CTRL_Init+0x138>)
30008480:	f24d 0101 	movw	r1, #53249	; 0xd001
30008484:	609c      	str	r4, [r3, #8]
30008486:	2001      	movs	r0, #1
30008488:	601a      	str	r2, [r3, #0]
3000848a:	4a4a      	ldr	r2, [pc, #296]	; (300085b4 <PSRAM_CTRL_Init+0x13c>)
3000848c:	6158      	str	r0, [r3, #20]
3000848e:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
30008492:	6811      	ldr	r1, [r2, #0]
30008494:	2902      	cmp	r1, #2
30008496:	d04a      	beq.n	3000852e <PSRAM_CTRL_Init+0xb6>
30008498:	6954      	ldr	r4, [r2, #20]
3000849a:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
3000849e:	69d0      	ldr	r0, [r2, #28]
300084a0:	6915      	ldr	r5, [r2, #16]
300084a2:	fbb0 f0f4 	udiv	r0, r0, r4
300084a6:	fbb1 f1f4 	udiv	r1, r1, r4
300084aa:	6994      	ldr	r4, [r2, #24]
300084ac:	f005 020f 	and.w	r2, r5, #15
300084b0:	f241 5551 	movw	r5, #5457	; 0x1551
300084b4:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
300084b8:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
300084bc:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
300084c0:	094a      	lsrs	r2, r1, #5
300084c2:	0181      	lsls	r1, r0, #6
300084c4:	f000 003f 	and.w	r0, r0, #63	; 0x3f
300084c8:	4d3b      	ldr	r5, [pc, #236]	; (300085b8 <PSRAM_CTRL_Init+0x140>)
300084ca:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
300084ce:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
300084d2:	00a2      	lsls	r2, r4, #2
300084d4:	4301      	orrs	r1, r0
300084d6:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
300084da:	3a01      	subs	r2, #1
300084dc:	430d      	orrs	r5, r1
300084de:	f242 0120 	movw	r1, #8224	; 0x2020
300084e2:	f3c2 020b 	ubfx	r2, r2, #0, #12
300084e6:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
300084ea:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
300084ee:	f44f 2180 	mov.w	r1, #262144	; 0x40000
300084f2:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
300084f6:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
300084fa:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300084fe:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
30008502:	4420      	add	r0, r4
30008504:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
30008508:	f640 71fe 	movw	r1, #4094	; 0xffe
3000850c:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
30008510:	ea01 0140 	and.w	r1, r1, r0, lsl #1
30008514:	432a      	orrs	r2, r5
30008516:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
3000851a:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
3000851e:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
30008522:	4a22      	ldr	r2, [pc, #136]	; (300085ac <PSRAM_CTRL_Init+0x134>)
30008524:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008526:	061b      	lsls	r3, r3, #24
30008528:	d5fc      	bpl.n	30008524 <PSRAM_CTRL_Init+0xac>
3000852a:	bc70      	pop	{r4, r5, r6}
3000852c:	4770      	bx	lr
3000852e:	6955      	ldr	r5, [r2, #20]
30008530:	207d      	movs	r0, #125	; 0x7d
30008532:	69d1      	ldr	r1, [r2, #28]
30008534:	26a0      	movs	r6, #160	; 0xa0
30008536:	6914      	ldr	r4, [r2, #16]
30008538:	6992      	ldr	r2, [r2, #24]
3000853a:	f004 040f 	and.w	r4, r4, #15
3000853e:	0092      	lsls	r2, r2, #2
30008540:	fbb1 f1f5 	udiv	r1, r1, r5
30008544:	fbb0 f5f5 	udiv	r5, r0, r5
30008548:	f245 5051 	movw	r0, #21841	; 0x5551
3000854c:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30008550:	f001 003f 	and.w	r0, r1, #63	; 0x3f
30008554:	0189      	lsls	r1, r1, #6
30008556:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
3000855a:	f641 4570 	movw	r5, #7280	; 0x1c70
3000855e:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30008562:	4325      	orrs	r5, r4
30008564:	2420      	movs	r4, #32
30008566:	4308      	orrs	r0, r1
30008568:	1e51      	subs	r1, r2, #1
3000856a:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
3000856e:	3a02      	subs	r2, #2
30008570:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
30008574:	f3c1 010b 	ubfx	r1, r1, #0, #12
30008578:	f3c2 020b 	ubfx	r2, r2, #0, #12
3000857c:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30008580:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
30008584:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30008588:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
3000858c:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30008590:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30008594:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30008598:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
3000859c:	4301      	orrs	r1, r0
3000859e:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
300085a2:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300085a6:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
300085aa:	e7ba      	b.n	30008522 <PSRAM_CTRL_Init+0xaa>
300085ac:	41012000 	.word	0x41012000
300085b0:	00bfe000 	.word	0x00bfe000
300085b4:	3000f340 	.word	0x3000f340
300085b8:	00107000 	.word	0x00107000

300085bc <PSRAM_REG_Write>:
300085bc:	b430      	push	{r4, r5}
300085be:	4c20      	ldr	r4, [pc, #128]	; (30008640 <PSRAM_REG_Write+0x84>)
300085c0:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300085c2:	f013 0301 	ands.w	r3, r3, #1
300085c6:	d1fb      	bne.n	300085c0 <PSRAM_REG_Write+0x4>
300085c8:	6825      	ldr	r5, [r4, #0]
300085ca:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
300085ce:	6025      	str	r5, [r4, #0]
300085d0:	60a3      	str	r3, [r4, #8]
300085d2:	6825      	ldr	r5, [r4, #0]
300085d4:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300085d8:	6025      	str	r5, [r4, #0]
300085da:	6063      	str	r3, [r4, #4]
300085dc:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
300085e0:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
300085e4:	23c0      	movs	r3, #192	; 0xc0
300085e6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300085ea:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300085ee:	0e03      	lsrs	r3, r0, #24
300085f0:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300085f4:	f3c0 4307 	ubfx	r3, r0, #16, #8
300085f8:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300085fc:	f3c0 2307 	ubfx	r3, r0, #8, #8
30008600:	b2c0      	uxtb	r0, r0
30008602:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008606:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
3000860a:	b141      	cbz	r1, 3000861e <PSRAM_REG_Write+0x62>
3000860c:	3a01      	subs	r2, #1
3000860e:	480c      	ldr	r0, [pc, #48]	; (30008640 <PSRAM_REG_Write+0x84>)
30008610:	4411      	add	r1, r2
30008612:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30008616:	428a      	cmp	r2, r1
30008618:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000861c:	d1f9      	bne.n	30008612 <PSRAM_REG_Write+0x56>
3000861e:	4a08      	ldr	r2, [pc, #32]	; (30008640 <PSRAM_REG_Write+0x84>)
30008620:	2301      	movs	r3, #1
30008622:	6093      	str	r3, [r2, #8]
30008624:	6893      	ldr	r3, [r2, #8]
30008626:	07d9      	lsls	r1, r3, #31
30008628:	d4fc      	bmi.n	30008624 <PSRAM_REG_Write+0x68>
3000862a:	4a05      	ldr	r2, [pc, #20]	; (30008640 <PSRAM_REG_Write+0x84>)
3000862c:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000862e:	07db      	lsls	r3, r3, #31
30008630:	d4fc      	bmi.n	3000862c <PSRAM_REG_Write+0x70>
30008632:	6813      	ldr	r3, [r2, #0]
30008634:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008638:	6013      	str	r3, [r2, #0]
3000863a:	bc30      	pop	{r4, r5}
3000863c:	4770      	bx	lr
3000863e:	bf00      	nop
30008640:	41012000 	.word	0x41012000

30008644 <PSRAM_APM_DEVIC_Init>:
30008644:	4b12      	ldr	r3, [pc, #72]	; (30008690 <PSRAM_APM_DEVIC_Init+0x4c>)
30008646:	2102      	movs	r1, #2
30008648:	2000      	movs	r0, #0
3000864a:	b530      	push	{r4, r5, lr}
3000864c:	4c11      	ldr	r4, [pc, #68]	; (30008694 <PSRAM_APM_DEVIC_Init+0x50>)
3000864e:	b083      	sub	sp, #12
30008650:	699d      	ldr	r5, [r3, #24]
30008652:	466a      	mov	r2, sp
30008654:	eb04 0c05 	add.w	ip, r4, r5
30008658:	1eeb      	subs	r3, r5, #3
3000865a:	4c0f      	ldr	r4, [pc, #60]	; (30008698 <PSRAM_APM_DEVIC_Init+0x54>)
3000865c:	408b      	lsls	r3, r1
3000865e:	f81c cc03 	ldrb.w	ip, [ip, #-3]
30008662:	f003 031c 	and.w	r3, r3, #28
30008666:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
3000866a:	f043 0320 	orr.w	r3, r3, #32
3000866e:	fa5f fc8c 	uxtb.w	ip, ip
30008672:	f88d 3000 	strb.w	r3, [sp]
30008676:	f88d c004 	strb.w	ip, [sp, #4]
3000867a:	f88d 3001 	strb.w	r3, [sp, #1]
3000867e:	f88d c005 	strb.w	ip, [sp, #5]
30008682:	47a0      	blx	r4
30008684:	aa01      	add	r2, sp, #4
30008686:	2102      	movs	r1, #2
30008688:	2004      	movs	r0, #4
3000868a:	47a0      	blx	r4
3000868c:	b003      	add	sp, #12
3000868e:	bd30      	pop	{r4, r5, pc}
30008690:	3000f340 	.word	0x3000f340
30008694:	3000e7dc 	.word	0x3000e7dc
30008698:	300085bd 	.word	0x300085bd

3000869c <PSRAM_WB_REG_Write>:
3000869c:	b430      	push	{r4, r5}
3000869e:	4c1e      	ldr	r4, [pc, #120]	; (30008718 <PSRAM_WB_REG_Write+0x7c>)
300086a0:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300086a2:	f013 0301 	ands.w	r3, r3, #1
300086a6:	d1fb      	bne.n	300086a0 <PSRAM_WB_REG_Write+0x4>
300086a8:	6825      	ldr	r5, [r4, #0]
300086aa:	b2c0      	uxtb	r0, r0
300086ac:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
300086b0:	6025      	str	r5, [r4, #0]
300086b2:	60a3      	str	r3, [r4, #8]
300086b4:	6825      	ldr	r5, [r4, #0]
300086b6:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300086ba:	6025      	str	r5, [r4, #0]
300086bc:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
300086c0:	6063      	str	r3, [r4, #4]
300086c2:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
300086c6:	2560      	movs	r5, #96	; 0x60
300086c8:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300086cc:	2501      	movs	r5, #1
300086ce:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300086d2:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300086d6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300086da:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300086de:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300086e2:	b141      	cbz	r1, 300086f6 <PSRAM_WB_REG_Write+0x5a>
300086e4:	3a01      	subs	r2, #1
300086e6:	480c      	ldr	r0, [pc, #48]	; (30008718 <PSRAM_WB_REG_Write+0x7c>)
300086e8:	4411      	add	r1, r2
300086ea:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300086ee:	428a      	cmp	r2, r1
300086f0:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300086f4:	d1f9      	bne.n	300086ea <PSRAM_WB_REG_Write+0x4e>
300086f6:	4a08      	ldr	r2, [pc, #32]	; (30008718 <PSRAM_WB_REG_Write+0x7c>)
300086f8:	2301      	movs	r3, #1
300086fa:	6093      	str	r3, [r2, #8]
300086fc:	6893      	ldr	r3, [r2, #8]
300086fe:	07d9      	lsls	r1, r3, #31
30008700:	d4fc      	bmi.n	300086fc <PSRAM_WB_REG_Write+0x60>
30008702:	4a05      	ldr	r2, [pc, #20]	; (30008718 <PSRAM_WB_REG_Write+0x7c>)
30008704:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008706:	07db      	lsls	r3, r3, #31
30008708:	d4fc      	bmi.n	30008704 <PSRAM_WB_REG_Write+0x68>
3000870a:	6813      	ldr	r3, [r2, #0]
3000870c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008710:	6013      	str	r3, [r2, #0]
30008712:	bc30      	pop	{r4, r5}
30008714:	4770      	bx	lr
30008716:	bf00      	nop
30008718:	41012000 	.word	0x41012000

3000871c <PSRAM_WB_DEVIC_Init>:
3000871c:	490b      	ldr	r1, [pc, #44]	; (3000874c <PSRAM_WB_DEVIC_Init+0x30>)
3000871e:	4b0c      	ldr	r3, [pc, #48]	; (30008750 <PSRAM_WB_DEVIC_Init+0x34>)
30008720:	b510      	push	{r4, lr}
30008722:	6988      	ldr	r0, [r1, #24]
30008724:	b082      	sub	sp, #8
30008726:	248f      	movs	r4, #143	; 0x8f
30008728:	2102      	movs	r1, #2
3000872a:	4403      	add	r3, r0
3000872c:	aa01      	add	r2, sp, #4
3000872e:	f88d 4004 	strb.w	r4, [sp, #4]
30008732:	2000      	movs	r0, #0
30008734:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30008738:	4c06      	ldr	r4, [pc, #24]	; (30008754 <PSRAM_WB_DEVIC_Init+0x38>)
3000873a:	011b      	lsls	r3, r3, #4
3000873c:	f043 030c 	orr.w	r3, r3, #12
30008740:	f88d 3005 	strb.w	r3, [sp, #5]
30008744:	47a0      	blx	r4
30008746:	b002      	add	sp, #8
30008748:	bd10      	pop	{r4, pc}
3000874a:	bf00      	nop
3000874c:	3000f340 	.word	0x3000f340
30008750:	3000e7fc 	.word	0x3000e7fc
30008754:	3000869d 	.word	0x3000869d

30008758 <PSRAM_calibration>:
30008758:	4a9c      	ldr	r2, [pc, #624]	; (300089cc <PSRAM_calibration+0x274>)
3000875a:	2300      	movs	r3, #0
3000875c:	499c      	ldr	r1, [pc, #624]	; (300089d0 <PSRAM_calibration+0x278>)
3000875e:	489d      	ldr	r0, [pc, #628]	; (300089d4 <PSRAM_calibration+0x27c>)
30008760:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008764:	6852      	ldr	r2, [r2, #4]
30008766:	b093      	sub	sp, #76	; 0x4c
30008768:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
3000876c:	400a      	ands	r2, r1
3000876e:	9311      	str	r3, [sp, #68]	; 0x44
30008770:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
30008774:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
30008778:	920a      	str	r2, [sp, #40]	; 0x28
3000877a:	6942      	ldr	r2, [r0, #20]
3000877c:	03d1      	lsls	r1, r2, #15
3000877e:	d51e      	bpl.n	300087be <PSRAM_calibration+0x66>
30008780:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008784:	f3bf 8f4f 	dsb	sy
30008788:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000878c:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30008790:	f3c5 344e 	ubfx	r4, r5, #13, #15
30008794:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30008798:	0164      	lsls	r4, r4, #5
3000879a:	ea04 0106 	and.w	r1, r4, r6
3000879e:	462b      	mov	r3, r5
300087a0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300087a4:	3b01      	subs	r3, #1
300087a6:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300087aa:	1c5a      	adds	r2, r3, #1
300087ac:	d1f8      	bne.n	300087a0 <PSRAM_calibration+0x48>
300087ae:	3c20      	subs	r4, #32
300087b0:	f114 0f20 	cmn.w	r4, #32
300087b4:	d1f1      	bne.n	3000879a <PSRAM_calibration+0x42>
300087b6:	f3bf 8f4f 	dsb	sy
300087ba:	f3bf 8f6f 	isb	sy
300087be:	4885      	ldr	r0, [pc, #532]	; (300089d4 <PSRAM_calibration+0x27c>)
300087c0:	2300      	movs	r3, #0
300087c2:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300087c6:	f3bf 8f4f 	dsb	sy
300087ca:	6943      	ldr	r3, [r0, #20]
300087cc:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300087d0:	6143      	str	r3, [r0, #20]
300087d2:	f3bf 8f4f 	dsb	sy
300087d6:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300087da:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300087de:	f3c5 344e 	ubfx	r4, r5, #13, #15
300087e2:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300087e6:	0164      	lsls	r4, r4, #5
300087e8:	ea04 0106 	and.w	r1, r4, r6
300087ec:	462b      	mov	r3, r5
300087ee:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300087f2:	3b01      	subs	r3, #1
300087f4:	1c5f      	adds	r7, r3, #1
300087f6:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300087fa:	d1f8      	bne.n	300087ee <PSRAM_calibration+0x96>
300087fc:	3c20      	subs	r4, #32
300087fe:	f114 0f20 	cmn.w	r4, #32
30008802:	d1f1      	bne.n	300087e8 <PSRAM_calibration+0x90>
30008804:	f3bf 8f4f 	dsb	sy
30008808:	f3bf 8f6f 	isb	sy
3000880c:	2204      	movs	r2, #4
3000880e:	461e      	mov	r6, r3
30008810:	9306      	str	r3, [sp, #24]
30008812:	2700      	movs	r7, #0
30008814:	9209      	str	r2, [sp, #36]	; 0x24
30008816:	46b3      	mov	fp, r6
30008818:	4a6c      	ldr	r2, [pc, #432]	; (300089cc <PSRAM_calibration+0x274>)
3000881a:	4c6f      	ldr	r4, [pc, #444]	; (300089d8 <PSRAM_calibration+0x280>)
3000881c:	6813      	ldr	r3, [r2, #0]
3000881e:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 300089f4 <PSRAM_calibration+0x29c>
30008822:	f023 0301 	bic.w	r3, r3, #1
30008826:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 300089f8 <PSRAM_calibration+0x2a0>
3000882a:	6013      	str	r3, [r2, #0]
3000882c:	2301      	movs	r3, #1
3000882e:	9305      	str	r3, [sp, #20]
30008830:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008832:	9a05      	ldr	r2, [sp, #20]
30008834:	9704      	str	r7, [sp, #16]
30008836:	2700      	movs	r7, #0
30008838:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
3000883c:	463d      	mov	r5, r7
3000883e:	e9cd b607 	strd	fp, r6, [sp, #28]
30008842:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
30008846:	465e      	mov	r6, fp
30008848:	e010      	b.n	3000886c <PSRAM_calibration+0x114>
3000884a:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
3000884e:	bf08      	it	eq
30008850:	462e      	moveq	r6, r5
30008852:	2d1f      	cmp	r5, #31
30008854:	eb06 0b07 	add.w	fp, r6, r7
30008858:	f107 0701 	add.w	r7, r7, #1
3000885c:	d102      	bne.n	30008864 <PSRAM_calibration+0x10c>
3000885e:	9b04      	ldr	r3, [sp, #16]
30008860:	429f      	cmp	r7, r3
30008862:	dc46      	bgt.n	300088f2 <PSRAM_calibration+0x19a>
30008864:	3501      	adds	r5, #1
30008866:	2d20      	cmp	r5, #32
30008868:	f000 80ac 	beq.w	300089c4 <PSRAM_calibration+0x26c>
3000886c:	ea48 0205 	orr.w	r2, r8, r5
30008870:	4b56      	ldr	r3, [pc, #344]	; (300089cc <PSRAM_calibration+0x274>)
30008872:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
30008876:	4959      	ldr	r1, [pc, #356]	; (300089dc <PSRAM_calibration+0x284>)
30008878:	605a      	str	r2, [r3, #4]
3000887a:	6822      	ldr	r2, [r4, #0]
3000887c:	4b58      	ldr	r3, [pc, #352]	; (300089e0 <PSRAM_calibration+0x288>)
3000887e:	f8ce 2000 	str.w	r2, [lr]
30008882:	6860      	ldr	r0, [r4, #4]
30008884:	4a57      	ldr	r2, [pc, #348]	; (300089e4 <PSRAM_calibration+0x28c>)
30008886:	f8c9 0000 	str.w	r0, [r9]
3000888a:	68a0      	ldr	r0, [r4, #8]
3000888c:	f8ca 0000 	str.w	r0, [sl]
30008890:	68e0      	ldr	r0, [r4, #12]
30008892:	6018      	str	r0, [r3, #0]
30008894:	6920      	ldr	r0, [r4, #16]
30008896:	6008      	str	r0, [r1, #0]
30008898:	6960      	ldr	r0, [r4, #20]
3000889a:	6010      	str	r0, [r2, #0]
3000889c:	f8de 0000 	ldr.w	r0, [lr]
300088a0:	900c      	str	r0, [sp, #48]	; 0x30
300088a2:	f8d9 0000 	ldr.w	r0, [r9]
300088a6:	900d      	str	r0, [sp, #52]	; 0x34
300088a8:	f8da 0000 	ldr.w	r0, [sl]
300088ac:	900e      	str	r0, [sp, #56]	; 0x38
300088ae:	6818      	ldr	r0, [r3, #0]
300088b0:	4b4d      	ldr	r3, [pc, #308]	; (300089e8 <PSRAM_calibration+0x290>)
300088b2:	900f      	str	r0, [sp, #60]	; 0x3c
300088b4:	a80c      	add	r0, sp, #48	; 0x30
300088b6:	6809      	ldr	r1, [r1, #0]
300088b8:	9110      	str	r1, [sp, #64]	; 0x40
300088ba:	6812      	ldr	r2, [r2, #0]
300088bc:	4946      	ldr	r1, [pc, #280]	; (300089d8 <PSRAM_calibration+0x280>)
300088be:	9211      	str	r2, [sp, #68]	; 0x44
300088c0:	2218      	movs	r2, #24
300088c2:	4798      	blx	r3
300088c4:	2800      	cmp	r0, #0
300088c6:	d0c0      	beq.n	3000884a <PSRAM_calibration+0xf2>
300088c8:	1c70      	adds	r0, r6, #1
300088ca:	d0cb      	beq.n	30008864 <PSRAM_calibration+0x10c>
300088cc:	9b04      	ldr	r3, [sp, #16]
300088ce:	42bb      	cmp	r3, r7
300088d0:	da0a      	bge.n	300088e8 <PSRAM_calibration+0x190>
300088d2:	9b05      	ldr	r3, [sp, #20]
300088d4:	f8cd b01c 	str.w	fp, [sp, #28]
300088d8:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300088dc:	9704      	str	r7, [sp, #16]
300088de:	2700      	movs	r7, #0
300088e0:	9608      	str	r6, [sp, #32]
300088e2:	465e      	mov	r6, fp
300088e4:	9306      	str	r3, [sp, #24]
300088e6:	e7bd      	b.n	30008864 <PSRAM_calibration+0x10c>
300088e8:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300088ec:	2700      	movs	r7, #0
300088ee:	465e      	mov	r6, fp
300088f0:	e7b8      	b.n	30008864 <PSRAM_calibration+0x10c>
300088f2:	9b05      	ldr	r3, [sp, #20]
300088f4:	9306      	str	r3, [sp, #24]
300088f6:	9b05      	ldr	r3, [sp, #20]
300088f8:	005b      	lsls	r3, r3, #1
300088fa:	9305      	str	r3, [sp, #20]
300088fc:	9b09      	ldr	r3, [sp, #36]	; 0x24
300088fe:	3b01      	subs	r3, #1
30008900:	9309      	str	r3, [sp, #36]	; 0x24
30008902:	d195      	bne.n	30008830 <PSRAM_calibration+0xd8>
30008904:	9b06      	ldr	r3, [sp, #24]
30008906:	2004      	movs	r0, #4
30008908:	4a38      	ldr	r2, [pc, #224]	; (300089ec <PSRAM_calibration+0x294>)
3000890a:	e9cd 7302 	strd	r7, r3, [sp, #8]
3000890e:	4b38      	ldr	r3, [pc, #224]	; (300089f0 <PSRAM_calibration+0x298>)
30008910:	e9cd 6b00 	strd	r6, fp, [sp]
30008914:	4619      	mov	r1, r3
30008916:	f002 fe1d 	bl	3000b554 <rtk_log_write>
3000891a:	482e      	ldr	r0, [pc, #184]	; (300089d4 <PSRAM_calibration+0x27c>)
3000891c:	6943      	ldr	r3, [r0, #20]
3000891e:	03d9      	lsls	r1, r3, #15
30008920:	d425      	bmi.n	3000896e <PSRAM_calibration+0x216>
30008922:	9b09      	ldr	r3, [sp, #36]	; 0x24
30008924:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008928:	f3bf 8f4f 	dsb	sy
3000892c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30008930:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30008934:	f3c5 344e 	ubfx	r4, r5, #13, #15
30008938:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000893c:	0164      	lsls	r4, r4, #5
3000893e:	ea04 010c 	and.w	r1, r4, ip
30008942:	462b      	mov	r3, r5
30008944:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30008948:	3b01      	subs	r3, #1
3000894a:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
3000894e:	1c5a      	adds	r2, r3, #1
30008950:	d1f8      	bne.n	30008944 <PSRAM_calibration+0x1ec>
30008952:	3c20      	subs	r4, #32
30008954:	f114 0f20 	cmn.w	r4, #32
30008958:	d1f1      	bne.n	3000893e <PSRAM_calibration+0x1e6>
3000895a:	f3bf 8f4f 	dsb	sy
3000895e:	6943      	ldr	r3, [r0, #20]
30008960:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30008964:	6143      	str	r3, [r0, #20]
30008966:	f3bf 8f4f 	dsb	sy
3000896a:	f3bf 8f6f 	isb	sy
3000896e:	2f08      	cmp	r7, #8
30008970:	dd24      	ble.n	300089bc <PSRAM_calibration+0x264>
30008972:	ebbb 0306 	subs.w	r3, fp, r6
30008976:	9806      	ldr	r0, [sp, #24]
30008978:	bf48      	it	mi
3000897a:	3301      	addmi	r3, #1
3000897c:	eb16 060b 	adds.w	r6, r6, fp
30008980:	ea4f 0363 	mov.w	r3, r3, asr #1
30008984:	bf48      	it	mi
30008986:	3601      	addmi	r6, #1
30008988:	1e9a      	subs	r2, r3, #2
3000898a:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000898c:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30008990:	0414      	lsls	r4, r2, #16
30008992:	f3c6 0344 	ubfx	r3, r6, #1, #5
30008996:	0212      	lsls	r2, r2, #8
30008998:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
3000899c:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
300089a0:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
300089a4:	2001      	movs	r0, #1
300089a6:	430b      	orrs	r3, r1
300089a8:	4908      	ldr	r1, [pc, #32]	; (300089cc <PSRAM_calibration+0x274>)
300089aa:	4323      	orrs	r3, r4
300089ac:	4313      	orrs	r3, r2
300089ae:	604b      	str	r3, [r1, #4]
300089b0:	680b      	ldr	r3, [r1, #0]
300089b2:	4303      	orrs	r3, r0
300089b4:	600b      	str	r3, [r1, #0]
300089b6:	b013      	add	sp, #76	; 0x4c
300089b8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300089bc:	2000      	movs	r0, #0
300089be:	b013      	add	sp, #76	; 0x4c
300089c0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300089c4:	9f04      	ldr	r7, [sp, #16]
300089c6:	e9dd b607 	ldrd	fp, r6, [sp, #28]
300089ca:	e794      	b.n	300088f6 <PSRAM_calibration+0x19e>
300089cc:	41002000 	.word	0x41002000
300089d0:	0fffffe0 	.word	0x0fffffe0
300089d4:	e000ed00 	.word	0xe000ed00
300089d8:	3000e7e4 	.word	0x3000e7e4
300089dc:	60200000 	.word	0x60200000
300089e0:	60150000 	.word	0x60150000
300089e4:	60250000 	.word	0x60250000
300089e8:	000129bd 	.word	0x000129bd
300089ec:	3000cfe8 	.word	0x3000cfe8
300089f0:	3000cfc0 	.word	0x3000cfc0
300089f4:	60050000 	.word	0x60050000
300089f8:	60100000 	.word	0x60100000

300089fc <PSRAM_AutoGating>:
300089fc:	b1c0      	cbz	r0, 30008a30 <PSRAM_AutoGating+0x34>
300089fe:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30008a02:	4810      	ldr	r0, [pc, #64]	; (30008a44 <PSRAM_AutoGating+0x48>)
30008a04:	f3c1 0108 	ubfx	r1, r1, #0, #9
30008a08:	0252      	lsls	r2, r2, #9
30008a0a:	b410      	push	{r4}
30008a0c:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
30008a10:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
30008a14:	4020      	ands	r0, r4
30008a16:	4301      	orrs	r1, r0
30008a18:	430a      	orrs	r2, r1
30008a1a:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30008a1e:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
30008a22:	f042 0201 	orr.w	r2, r2, #1
30008a26:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30008a2a:	f85d 4b04 	ldr.w	r4, [sp], #4
30008a2e:	4770      	bx	lr
30008a30:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30008a34:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30008a38:	f023 0301 	bic.w	r3, r3, #1
30008a3c:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30008a40:	4770      	bx	lr
30008a42:	bf00      	nop
30008a44:	ffffc000 	.word	0xffffc000

30008a48 <BOOT_Reason>:
30008a48:	4b02      	ldr	r3, [pc, #8]	; (30008a54 <BOOT_Reason+0xc>)
30008a4a:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30008a4e:	b280      	uxth	r0, r0
30008a50:	4770      	bx	lr
30008a52:	bf00      	nop
30008a54:	42008200 	.word	0x42008200

30008a58 <CRYPTO_SetSecurityModeAD>:
30008a58:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008a5c:	b083      	sub	sp, #12
30008a5e:	460d      	mov	r5, r1
30008a60:	4616      	mov	r6, r2
30008a62:	4699      	mov	r9, r3
30008a64:	4604      	mov	r4, r0
30008a66:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
30008a6a:	2800      	cmp	r0, #0
30008a6c:	f000 80a9 	beq.w	30008bc2 <CRYPTO_SetSecurityModeAD+0x16a>
30008a70:	7823      	ldrb	r3, [r4, #0]
30008a72:	2b01      	cmp	r3, #1
30008a74:	d003      	beq.n	30008a7e <CRYPTO_SetSecurityModeAD+0x26>
30008a76:	2125      	movs	r1, #37	; 0x25
30008a78:	485d      	ldr	r0, [pc, #372]	; (30008bf0 <CRYPTO_SetSecurityModeAD+0x198>)
30008a7a:	f002 ffb1 	bl	3000b9e0 <__io_assert_failed_veneer>
30008a7e:	2300      	movs	r3, #0
30008a80:	1c6a      	adds	r2, r5, #1
30008a82:	6065      	str	r5, [r4, #4]
30008a84:	8123      	strh	r3, [r4, #8]
30008a86:	d008      	beq.n	30008a9a <CRYPTO_SetSecurityModeAD+0x42>
30008a88:	f005 0330 	and.w	r3, r5, #48	; 0x30
30008a8c:	2b20      	cmp	r3, #32
30008a8e:	d07b      	beq.n	30008b88 <CRYPTO_SetSecurityModeAD+0x130>
30008a90:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30008a94:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30008a98:	7265      	strb	r5, [r4, #9]
30008a9a:	1c73      	adds	r3, r6, #1
30008a9c:	60e6      	str	r6, [r4, #12]
30008a9e:	f000 8082 	beq.w	30008ba6 <CRYPTO_SetSecurityModeAD+0x14e>
30008aa2:	f006 0303 	and.w	r3, r6, #3
30008aa6:	f3c6 0280 	ubfx	r2, r6, #2, #1
30008aaa:	7422      	strb	r2, [r4, #16]
30008aac:	f1a3 0202 	sub.w	r2, r3, #2
30008ab0:	2b01      	cmp	r3, #1
30008ab2:	fab2 f282 	clz	r2, r2
30008ab6:	ea4f 1252 	mov.w	r2, r2, lsr #5
30008aba:	7462      	strb	r2, [r4, #17]
30008abc:	fab3 f283 	clz	r2, r3
30008ac0:	ea4f 1252 	mov.w	r2, r2, lsr #5
30008ac4:	74a2      	strb	r2, [r4, #18]
30008ac6:	bf0c      	ite	eq
30008ac8:	2201      	moveq	r2, #1
30008aca:	2200      	movne	r2, #0
30008acc:	74e2      	strb	r2, [r4, #19]
30008ace:	d13f      	bne.n	30008b50 <CRYPTO_SetSecurityModeAD+0xf8>
30008ad0:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30008ad4:	2b40      	cmp	r3, #64	; 0x40
30008ad6:	f000 8083 	beq.w	30008be0 <CRYPTO_SetSecurityModeAD+0x188>
30008ada:	d86a      	bhi.n	30008bb2 <CRYPTO_SetSecurityModeAD+0x15a>
30008adc:	2b10      	cmp	r3, #16
30008ade:	d06d      	beq.n	30008bbc <CRYPTO_SetSecurityModeAD+0x164>
30008ae0:	2b20      	cmp	r3, #32
30008ae2:	d173      	bne.n	30008bcc <CRYPTO_SetSecurityModeAD+0x174>
30008ae4:	7523      	strb	r3, [r4, #20]
30008ae6:	62a3      	str	r3, [r4, #40]	; 0x28
30008ae8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008aea:	6227      	str	r7, [r4, #32]
30008aec:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30008af0:	2f00      	cmp	r7, #0
30008af2:	d038      	beq.n	30008b66 <CRYPTO_SetSecurityModeAD+0x10e>
30008af4:	7d23      	ldrb	r3, [r4, #20]
30008af6:	f104 0064 	add.w	r0, r4, #100	; 0x64
30008afa:	2b30      	cmp	r3, #48	; 0x30
30008afc:	65e0      	str	r0, [r4, #92]	; 0x5c
30008afe:	d046      	beq.n	30008b8e <CRYPTO_SetSecurityModeAD+0x136>
30008b00:	2b40      	cmp	r3, #64	; 0x40
30008b02:	d044      	beq.n	30008b8e <CRYPTO_SetSecurityModeAD+0x136>
30008b04:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30008b08:	2240      	movs	r2, #64	; 0x40
30008b0a:	2136      	movs	r1, #54	; 0x36
30008b0c:	4d39      	ldr	r5, [pc, #228]	; (30008bf4 <CRYPTO_SetSecurityModeAD+0x19c>)
30008b0e:	6623      	str	r3, [r4, #96]	; 0x60
30008b10:	47a8      	blx	r5
30008b12:	2240      	movs	r2, #64	; 0x40
30008b14:	215c      	movs	r1, #92	; 0x5c
30008b16:	6e20      	ldr	r0, [r4, #96]	; 0x60
30008b18:	47a8      	blx	r5
30008b1a:	4b37      	ldr	r3, [pc, #220]	; (30008bf8 <CRYPTO_SetSecurityModeAD+0x1a0>)
30008b1c:	681d      	ldr	r5, [r3, #0]
30008b1e:	bb25      	cbnz	r5, 30008b6a <CRYPTO_SetSecurityModeAD+0x112>
30008b20:	f1b8 0f00 	cmp.w	r8, #0
30008b24:	d05f      	beq.n	30008be6 <CRYPTO_SetSecurityModeAD+0x18e>
30008b26:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30008b2a:	e001      	b.n	30008b30 <CRYPTO_SetSecurityModeAD+0xd8>
30008b2c:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30008b30:	6de2      	ldr	r2, [r4, #92]	; 0x5c
30008b32:	f818 3005 	ldrb.w	r3, [r8, r5]
30008b36:	5d51      	ldrb	r1, [r2, r5]
30008b38:	404b      	eors	r3, r1
30008b3a:	5553      	strb	r3, [r2, r5]
30008b3c:	6e22      	ldr	r2, [r4, #96]	; 0x60
30008b3e:	6a63      	ldr	r3, [r4, #36]	; 0x24
30008b40:	5d51      	ldrb	r1, [r2, r5]
30008b42:	5d5b      	ldrb	r3, [r3, r5]
30008b44:	404b      	eors	r3, r1
30008b46:	5553      	strb	r3, [r2, r5]
30008b48:	3501      	adds	r5, #1
30008b4a:	42af      	cmp	r7, r5
30008b4c:	d1ee      	bne.n	30008b2c <CRYPTO_SetSecurityModeAD+0xd4>
30008b4e:	e00c      	b.n	30008b6a <CRYPTO_SetSecurityModeAD+0x112>
30008b50:	2200      	movs	r2, #0
30008b52:	7522      	strb	r2, [r4, #20]
30008b54:	b99b      	cbnz	r3, 30008b7e <CRYPTO_SetSecurityModeAD+0x126>
30008b56:	2314      	movs	r3, #20
30008b58:	6227      	str	r7, [r4, #32]
30008b5a:	62a3      	str	r3, [r4, #40]	; 0x28
30008b5c:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008b5e:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30008b62:	2f00      	cmp	r7, #0
30008b64:	d1c6      	bne.n	30008af4 <CRYPTO_SetSecurityModeAD+0x9c>
30008b66:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30008b6a:	3601      	adds	r6, #1
30008b6c:	d003      	beq.n	30008b76 <CRYPTO_SetSecurityModeAD+0x11e>
30008b6e:	2201      	movs	r2, #1
30008b70:	2300      	movs	r3, #0
30008b72:	85a2      	strh	r2, [r4, #44]	; 0x2c
30008b74:	6323      	str	r3, [r4, #48]	; 0x30
30008b76:	2000      	movs	r0, #0
30008b78:	b003      	add	sp, #12
30008b7a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008b7e:	2b02      	cmp	r3, #2
30008b80:	d114      	bne.n	30008bac <CRYPTO_SetSecurityModeAD+0x154>
30008b82:	2310      	movs	r3, #16
30008b84:	62a3      	str	r3, [r4, #40]	; 0x28
30008b86:	e7af      	b.n	30008ae8 <CRYPTO_SetSecurityModeAD+0x90>
30008b88:	2301      	movs	r3, #1
30008b8a:	7223      	strb	r3, [r4, #8]
30008b8c:	e780      	b.n	30008a90 <CRYPTO_SetSecurityModeAD+0x38>
30008b8e:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30008b92:	2280      	movs	r2, #128	; 0x80
30008b94:	2136      	movs	r1, #54	; 0x36
30008b96:	4d17      	ldr	r5, [pc, #92]	; (30008bf4 <CRYPTO_SetSecurityModeAD+0x19c>)
30008b98:	6623      	str	r3, [r4, #96]	; 0x60
30008b9a:	47a8      	blx	r5
30008b9c:	2280      	movs	r2, #128	; 0x80
30008b9e:	215c      	movs	r1, #92	; 0x5c
30008ba0:	6e20      	ldr	r0, [r4, #96]	; 0x60
30008ba2:	47a8      	blx	r5
30008ba4:	e7b9      	b.n	30008b1a <CRYPTO_SetSecurityModeAD+0xc2>
30008ba6:	2300      	movs	r3, #0
30008ba8:	6123      	str	r3, [r4, #16]
30008baa:	7523      	strb	r3, [r4, #20]
30008bac:	2300      	movs	r3, #0
30008bae:	62a3      	str	r3, [r4, #40]	; 0x28
30008bb0:	e79a      	b.n	30008ae8 <CRYPTO_SetSecurityModeAD+0x90>
30008bb2:	2b80      	cmp	r3, #128	; 0x80
30008bb4:	d10a      	bne.n	30008bcc <CRYPTO_SetSecurityModeAD+0x174>
30008bb6:	2340      	movs	r3, #64	; 0x40
30008bb8:	7523      	strb	r3, [r4, #20]
30008bba:	e794      	b.n	30008ae6 <CRYPTO_SetSecurityModeAD+0x8e>
30008bbc:	231c      	movs	r3, #28
30008bbe:	7523      	strb	r3, [r4, #20]
30008bc0:	e7f5      	b.n	30008bae <CRYPTO_SetSecurityModeAD+0x156>
30008bc2:	2124      	movs	r1, #36	; 0x24
30008bc4:	480a      	ldr	r0, [pc, #40]	; (30008bf0 <CRYPTO_SetSecurityModeAD+0x198>)
30008bc6:	f002 ff0b 	bl	3000b9e0 <__io_assert_failed_veneer>
30008bca:	e751      	b.n	30008a70 <CRYPTO_SetSecurityModeAD+0x18>
30008bcc:	4b0b      	ldr	r3, [pc, #44]	; (30008bfc <CRYPTO_SetSecurityModeAD+0x1a4>)
30008bce:	2002      	movs	r0, #2
30008bd0:	4a0b      	ldr	r2, [pc, #44]	; (30008c00 <CRYPTO_SetSecurityModeAD+0x1a8>)
30008bd2:	4619      	mov	r1, r3
30008bd4:	9600      	str	r6, [sp, #0]
30008bd6:	f002 fcbd 	bl	3000b554 <rtk_log_write>
30008bda:	2300      	movs	r3, #0
30008bdc:	7523      	strb	r3, [r4, #20]
30008bde:	e7e6      	b.n	30008bae <CRYPTO_SetSecurityModeAD+0x156>
30008be0:	2330      	movs	r3, #48	; 0x30
30008be2:	7523      	strb	r3, [r4, #20]
30008be4:	e7e3      	b.n	30008bae <CRYPTO_SetSecurityModeAD+0x156>
30008be6:	2180      	movs	r1, #128	; 0x80
30008be8:	4801      	ldr	r0, [pc, #4]	; (30008bf0 <CRYPTO_SetSecurityModeAD+0x198>)
30008bea:	f002 fef9 	bl	3000b9e0 <__io_assert_failed_veneer>
30008bee:	e79a      	b.n	30008b26 <CRYPTO_SetSecurityModeAD+0xce>
30008bf0:	3000d088 	.word	0x3000d088
30008bf4:	00012be5 	.word	0x00012be5
30008bf8:	2001c200 	.word	0x2001c200
30008bfc:	3000d028 	.word	0x3000d028
30008c00:	3000d034 	.word	0x3000d034

30008c04 <rtl_crypto_hmac_sha2_init>:
30008c04:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008c08:	4c97      	ldr	r4, [pc, #604]	; (30008e68 <rtl_crypto_hmac_sha2_init+0x264>)
30008c0a:	4617      	mov	r7, r2
30008c0c:	e844 f400 	tt	r4, r4
30008c10:	4a96      	ldr	r2, [pc, #600]	; (30008e6c <rtl_crypto_hmac_sha2_init+0x268>)
30008c12:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
30008c16:	461c      	mov	r4, r3
30008c18:	4b95      	ldr	r3, [pc, #596]	; (30008e70 <rtl_crypto_hmac_sha2_init+0x26c>)
30008c1a:	bf18      	it	ne
30008c1c:	4613      	movne	r3, r2
30008c1e:	b085      	sub	sp, #20
30008c20:	4605      	mov	r5, r0
30008c22:	460e      	mov	r6, r1
30008c24:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30008c26:	2b00      	cmp	r3, #0
30008c28:	db05      	blt.n	30008c36 <rtl_crypto_hmac_sha2_init+0x32>
30008c2a:	2900      	cmp	r1, #0
30008c2c:	f000 8118 	beq.w	30008e60 <rtl_crypto_hmac_sha2_init+0x25c>
30008c30:	078b      	lsls	r3, r1, #30
30008c32:	f040 8112 	bne.w	30008e5a <rtl_crypto_hmac_sha2_init+0x256>
30008c36:	2d20      	cmp	r5, #32
30008c38:	bf18      	it	ne
30008c3a:	2d1c      	cmpne	r5, #28
30008c3c:	bf0c      	ite	eq
30008c3e:	f04f 0901 	moveq.w	r9, #1
30008c42:	f04f 0900 	movne.w	r9, #0
30008c46:	d04d      	beq.n	30008ce4 <rtl_crypto_hmac_sha2_init+0xe0>
30008c48:	2d40      	cmp	r5, #64	; 0x40
30008c4a:	bf18      	it	ne
30008c4c:	2d30      	cmpne	r5, #48	; 0x30
30008c4e:	d102      	bne.n	30008c56 <rtl_crypto_hmac_sha2_init+0x52>
30008c50:	2f80      	cmp	r7, #128	; 0x80
30008c52:	f200 80f9 	bhi.w	30008e48 <rtl_crypto_hmac_sha2_init+0x244>
30008c56:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30008e90 <rtl_crypto_hmac_sha2_init+0x28c>
30008c5a:	f898 3000 	ldrb.w	r3, [r8]
30008c5e:	2b01      	cmp	r3, #1
30008c60:	d003      	beq.n	30008c6a <rtl_crypto_hmac_sha2_init+0x66>
30008c62:	21b4      	movs	r1, #180	; 0xb4
30008c64:	4883      	ldr	r0, [pc, #524]	; (30008e74 <rtl_crypto_hmac_sha2_init+0x270>)
30008c66:	f002 febb 	bl	3000b9e0 <__io_assert_failed_veneer>
30008c6a:	2d30      	cmp	r5, #48	; 0x30
30008c6c:	f000 809b 	beq.w	30008da6 <rtl_crypto_hmac_sha2_init+0x1a2>
30008c70:	d83f      	bhi.n	30008cf2 <rtl_crypto_hmac_sha2_init+0xee>
30008c72:	2d1c      	cmp	r5, #28
30008c74:	f000 80cb 	beq.w	30008e0e <rtl_crypto_hmac_sha2_init+0x20a>
30008c78:	f1b9 0f00 	cmp.w	r9, #0
30008c7c:	d078      	beq.n	30008d70 <rtl_crypto_hmac_sha2_init+0x16c>
30008c7e:	2c00      	cmp	r4, #0
30008c80:	f000 808f 	beq.w	30008da2 <rtl_crypto_hmac_sha2_init+0x19e>
30008c84:	4b7c      	ldr	r3, [pc, #496]	; (30008e78 <rtl_crypto_hmac_sha2_init+0x274>)
30008c86:	f8df e210 	ldr.w	lr, [pc, #528]	; 30008e98 <rtl_crypto_hmac_sha2_init+0x294>
30008c8a:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30008ea8 <rtl_crypto_hmac_sha2_init+0x2a4>
30008c8e:	4a7b      	ldr	r2, [pc, #492]	; (30008e7c <rtl_crypto_hmac_sha2_init+0x278>)
30008c90:	e9c4 e302 	strd	lr, r3, [r4, #8]
30008c94:	4b7a      	ldr	r3, [pc, #488]	; (30008e80 <rtl_crypto_hmac_sha2_init+0x27c>)
30008c96:	e9c4 9304 	strd	r9, r3, [r4, #16]
30008c9a:	4b7a      	ldr	r3, [pc, #488]	; (30008e84 <rtl_crypto_hmac_sha2_init+0x280>)
30008c9c:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008ca0:	4b79      	ldr	r3, [pc, #484]	; (30008e88 <rtl_crypto_hmac_sha2_init+0x284>)
30008ca2:	2225      	movs	r2, #37	; 0x25
30008ca4:	6223      	str	r3, [r4, #32]
30008ca6:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30008caa:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30008cae:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30008cb2:	6263      	str	r3, [r4, #36]	; 0x24
30008cb4:	2300      	movs	r3, #0
30008cb6:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008cba:	4d74      	ldr	r5, [pc, #464]	; (30008e8c <rtl_crypto_hmac_sha2_init+0x288>)
30008cbc:	9300      	str	r3, [sp, #0]
30008cbe:	9702      	str	r7, [sp, #8]
30008cc0:	9601      	str	r6, [sp, #4]
30008cc2:	4873      	ldr	r0, [pc, #460]	; (30008e90 <rtl_crypto_hmac_sha2_init+0x28c>)
30008cc4:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30008cc8:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30008ccc:	47a8      	blx	r5
30008cce:	4605      	mov	r5, r0
30008cd0:	4b70      	ldr	r3, [pc, #448]	; (30008e94 <rtl_crypto_hmac_sha2_init+0x290>)
30008cd2:	4620      	mov	r0, r4
30008cd4:	4798      	blx	r3
30008cd6:	2301      	movs	r3, #1
30008cd8:	4628      	mov	r0, r5
30008cda:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30008cde:	b005      	add	sp, #20
30008ce0:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008ce4:	2f40      	cmp	r7, #64	; 0x40
30008ce6:	d9b6      	bls.n	30008c56 <rtl_crypto_hmac_sha2_init+0x52>
30008ce8:	21ae      	movs	r1, #174	; 0xae
30008cea:	4862      	ldr	r0, [pc, #392]	; (30008e74 <rtl_crypto_hmac_sha2_init+0x270>)
30008cec:	f002 fe78 	bl	3000b9e0 <__io_assert_failed_veneer>
30008cf0:	e7b1      	b.n	30008c56 <rtl_crypto_hmac_sha2_init+0x52>
30008cf2:	2d40      	cmp	r5, #64	; 0x40
30008cf4:	d13c      	bne.n	30008d70 <rtl_crypto_hmac_sha2_init+0x16c>
30008cf6:	2c00      	cmp	r4, #0
30008cf8:	d040      	beq.n	30008d7c <rtl_crypto_hmac_sha2_init+0x178>
30008cfa:	4b67      	ldr	r3, [pc, #412]	; (30008e98 <rtl_crypto_hmac_sha2_init+0x294>)
30008cfc:	4a67      	ldr	r2, [pc, #412]	; (30008e9c <rtl_crypto_hmac_sha2_init+0x298>)
30008cfe:	4968      	ldr	r1, [pc, #416]	; (30008ea0 <rtl_crypto_hmac_sha2_init+0x29c>)
30008d00:	4d68      	ldr	r5, [pc, #416]	; (30008ea4 <rtl_crypto_hmac_sha2_init+0x2a0>)
30008d02:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30008ee8 <rtl_crypto_hmac_sha2_init+0x2e4>
30008d06:	e9c4 3202 	strd	r3, r2, [r4, #8]
30008d0a:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30008d0e:	2285      	movs	r2, #133	; 0x85
30008d10:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30008d14:	33d4      	adds	r3, #212	; 0xd4
30008d16:	6163      	str	r3, [r4, #20]
30008d18:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30008d1c:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30008d20:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30008d24:	6123      	str	r3, [r4, #16]
30008d26:	4b60      	ldr	r3, [pc, #384]	; (30008ea8 <rtl_crypto_hmac_sha2_init+0x2a4>)
30008d28:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008d2c:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30008d30:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30008d34:	f203 337f 	addw	r3, r3, #895	; 0x37f
30008d38:	6263      	str	r3, [r4, #36]	; 0x24
30008d3a:	4b51      	ldr	r3, [pc, #324]	; (30008e80 <rtl_crypto_hmac_sha2_init+0x27c>)
30008d3c:	6223      	str	r3, [r4, #32]
30008d3e:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30008d42:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30008d46:	f203 5397 	addw	r3, r3, #1431	; 0x597
30008d4a:	62e3      	str	r3, [r4, #44]	; 0x2c
30008d4c:	4b4b      	ldr	r3, [pc, #300]	; (30008e7c <rtl_crypto_hmac_sha2_init+0x278>)
30008d4e:	62a3      	str	r3, [r4, #40]	; 0x28
30008d50:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30008d54:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30008d58:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30008d5c:	6363      	str	r3, [r4, #52]	; 0x34
30008d5e:	4b49      	ldr	r3, [pc, #292]	; (30008e84 <rtl_crypto_hmac_sha2_init+0x280>)
30008d60:	6323      	str	r3, [r4, #48]	; 0x30
30008d62:	4b49      	ldr	r3, [pc, #292]	; (30008e88 <rtl_crypto_hmac_sha2_init+0x284>)
30008d64:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008d68:	4b50      	ldr	r3, [pc, #320]	; (30008eac <rtl_crypto_hmac_sha2_init+0x2a8>)
30008d6a:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008d6e:	e7a1      	b.n	30008cb4 <rtl_crypto_hmac_sha2_init+0xb0>
30008d70:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
30008d74:	4628      	mov	r0, r5
30008d76:	b005      	add	sp, #20
30008d78:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008d7c:	2285      	movs	r2, #133	; 0x85
30008d7e:	2300      	movs	r3, #0
30008d80:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008d84:	4c41      	ldr	r4, [pc, #260]	; (30008e8c <rtl_crypto_hmac_sha2_init+0x288>)
30008d86:	9702      	str	r7, [sp, #8]
30008d88:	4841      	ldr	r0, [pc, #260]	; (30008e90 <rtl_crypto_hmac_sha2_init+0x28c>)
30008d8a:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30008d8e:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30008d92:	e9cd 3600 	strd	r3, r6, [sp]
30008d96:	47a0      	blx	r4
30008d98:	4605      	mov	r5, r0
30008d9a:	4628      	mov	r0, r5
30008d9c:	b005      	add	sp, #20
30008d9e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008da2:	2225      	movs	r2, #37	; 0x25
30008da4:	e7eb      	b.n	30008d7e <rtl_crypto_hmac_sha2_init+0x17a>
30008da6:	2c00      	cmp	r4, #0
30008da8:	d053      	beq.n	30008e52 <rtl_crypto_hmac_sha2_init+0x24e>
30008daa:	4b41      	ldr	r3, [pc, #260]	; (30008eb0 <rtl_crypto_hmac_sha2_init+0x2ac>)
30008dac:	f8df e130 	ldr.w	lr, [pc, #304]	; 30008ee0 <rtl_crypto_hmac_sha2_init+0x2dc>
30008db0:	4a40      	ldr	r2, [pc, #256]	; (30008eb4 <rtl_crypto_hmac_sha2_init+0x2b0>)
30008db2:	4941      	ldr	r1, [pc, #260]	; (30008eb8 <rtl_crypto_hmac_sha2_init+0x2b4>)
30008db4:	4d41      	ldr	r5, [pc, #260]	; (30008ebc <rtl_crypto_hmac_sha2_init+0x2b8>)
30008db6:	f8df c12c 	ldr.w	ip, [pc, #300]	; 30008ee4 <rtl_crypto_hmac_sha2_init+0x2e0>
30008dba:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30008dbe:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30008dc2:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30008dc6:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30008dca:	6163      	str	r3, [r4, #20]
30008dcc:	4b3c      	ldr	r3, [pc, #240]	; (30008ec0 <rtl_crypto_hmac_sha2_init+0x2bc>)
30008dce:	6123      	str	r3, [r4, #16]
30008dd0:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30008dd4:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30008dd8:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30008ddc:	61e3      	str	r3, [r4, #28]
30008dde:	4b39      	ldr	r3, [pc, #228]	; (30008ec4 <rtl_crypto_hmac_sha2_init+0x2c0>)
30008de0:	61a3      	str	r3, [r4, #24]
30008de2:	4b39      	ldr	r3, [pc, #228]	; (30008ec8 <rtl_crypto_hmac_sha2_init+0x2c4>)
30008de4:	e9c4 3208 	strd	r3, r2, [r4, #32]
30008de8:	4b38      	ldr	r3, [pc, #224]	; (30008ecc <rtl_crypto_hmac_sha2_init+0x2c8>)
30008dea:	2245      	movs	r2, #69	; 0x45
30008dec:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30008df0:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30008df4:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30008df8:	33aa      	adds	r3, #170	; 0xaa
30008dfa:	6363      	str	r3, [r4, #52]	; 0x34
30008dfc:	4b34      	ldr	r3, [pc, #208]	; (30008ed0 <rtl_crypto_hmac_sha2_init+0x2cc>)
30008dfe:	6323      	str	r3, [r4, #48]	; 0x30
30008e00:	4b34      	ldr	r3, [pc, #208]	; (30008ed4 <rtl_crypto_hmac_sha2_init+0x2d0>)
30008e02:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008e06:	4b34      	ldr	r3, [pc, #208]	; (30008ed8 <rtl_crypto_hmac_sha2_init+0x2d4>)
30008e08:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008e0c:	e752      	b.n	30008cb4 <rtl_crypto_hmac_sha2_init+0xb0>
30008e0e:	b314      	cbz	r4, 30008e56 <rtl_crypto_hmac_sha2_init+0x252>
30008e10:	4b32      	ldr	r3, [pc, #200]	; (30008edc <rtl_crypto_hmac_sha2_init+0x2d8>)
30008e12:	2215      	movs	r2, #21
30008e14:	4932      	ldr	r1, [pc, #200]	; (30008ee0 <rtl_crypto_hmac_sha2_init+0x2dc>)
30008e16:	4827      	ldr	r0, [pc, #156]	; (30008eb4 <rtl_crypto_hmac_sha2_init+0x2b0>)
30008e18:	4d28      	ldr	r5, [pc, #160]	; (30008ebc <rtl_crypto_hmac_sha2_init+0x2b8>)
30008e1a:	e9c4 1302 	strd	r1, r3, [r4, #8]
30008e1e:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
30008e22:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
30008e26:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30008e2a:	6123      	str	r3, [r4, #16]
30008e2c:	4b22      	ldr	r3, [pc, #136]	; (30008eb8 <rtl_crypto_hmac_sha2_init+0x2b4>)
30008e2e:	e9c4 0305 	strd	r0, r3, [r4, #20]
30008e32:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30008e36:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30008e3a:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30008e3e:	61e3      	str	r3, [r4, #28]
30008e40:	4b28      	ldr	r3, [pc, #160]	; (30008ee4 <rtl_crypto_hmac_sha2_init+0x2e0>)
30008e42:	e9c4 5308 	strd	r5, r3, [r4, #32]
30008e46:	e735      	b.n	30008cb4 <rtl_crypto_hmac_sha2_init+0xb0>
30008e48:	21b1      	movs	r1, #177	; 0xb1
30008e4a:	480a      	ldr	r0, [pc, #40]	; (30008e74 <rtl_crypto_hmac_sha2_init+0x270>)
30008e4c:	f002 fdc8 	bl	3000b9e0 <__io_assert_failed_veneer>
30008e50:	e701      	b.n	30008c56 <rtl_crypto_hmac_sha2_init+0x52>
30008e52:	2245      	movs	r2, #69	; 0x45
30008e54:	e793      	b.n	30008d7e <rtl_crypto_hmac_sha2_init+0x17a>
30008e56:	2215      	movs	r2, #21
30008e58:	e791      	b.n	30008d7e <rtl_crypto_hmac_sha2_init+0x17a>
30008e5a:	f06f 0505 	mvn.w	r5, #5
30008e5e:	e789      	b.n	30008d74 <rtl_crypto_hmac_sha2_init+0x170>
30008e60:	f06f 0503 	mvn.w	r5, #3
30008e64:	e786      	b.n	30008d74 <rtl_crypto_hmac_sha2_init+0x170>
30008e66:	bf00      	nop
30008e68:	0000e5f9 	.word	0x0000e5f9
30008e6c:	500c8000 	.word	0x500c8000
30008e70:	400c8000 	.word	0x400c8000
30008e74:	3000d06c 	.word	0x3000d06c
30008e78:	bb67ae85 	.word	0xbb67ae85
30008e7c:	510e527f 	.word	0x510e527f
30008e80:	a54ff53a 	.word	0xa54ff53a
30008e84:	9b05688c 	.word	0x9b05688c
30008e88:	1f83d9ab 	.word	0x1f83d9ab
30008e8c:	30008a59 	.word	0x30008a59
30008e90:	2001c460 	.word	0x2001c460
30008e94:	00004c99 	.word	0x00004c99
30008e98:	6a09e667 	.word	0x6a09e667
30008e9c:	f3bcc908 	.word	0xf3bcc908
30008ea0:	fe94f82b 	.word	0xfe94f82b
30008ea4:	fb41bd6b 	.word	0xfb41bd6b
30008ea8:	3c6ef372 	.word	0x3c6ef372
30008eac:	5be0cd19 	.word	0x5be0cd19
30008eb0:	cbbb9d5d 	.word	0xcbbb9d5d
30008eb4:	f70e5939 	.word	0xf70e5939
30008eb8:	ffc00b31 	.word	0xffc00b31
30008ebc:	64f98fa7 	.word	0x64f98fa7
30008ec0:	629a292a 	.word	0x629a292a
30008ec4:	9159015a 	.word	0x9159015a
30008ec8:	152fecd8 	.word	0x152fecd8
30008ecc:	67332667 	.word	0x67332667
30008ed0:	8eb44a87 	.word	0x8eb44a87
30008ed4:	db0c2e0d 	.word	0xdb0c2e0d
30008ed8:	47b5481d 	.word	0x47b5481d
30008edc:	367cd507 	.word	0x367cd507
30008ee0:	c1059ed8 	.word	0xc1059ed8
30008ee4:	befa4fa4 	.word	0xbefa4fa4
30008ee8:	137e2179 	.word	0x137e2179

30008eec <CPU_ClkGet>:
30008eec:	b570      	push	{r4, r5, r6, lr}
30008eee:	4d1e      	ldr	r5, [pc, #120]	; (30008f68 <CPU_ClkGet+0x7c>)
30008ef0:	4b1e      	ldr	r3, [pc, #120]	; (30008f6c <CPU_ClkGet+0x80>)
30008ef2:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
30008ef6:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30008efa:	f3c4 1481 	ubfx	r4, r4, #6, #2
30008efe:	4798      	blx	r3
30008f00:	b92c      	cbnz	r4, 30008f0e <CPU_ClkGet+0x22>
30008f02:	2801      	cmp	r0, #1
30008f04:	d007      	beq.n	30008f16 <CPU_ClkGet+0x2a>
30008f06:	07b2      	lsls	r2, r6, #30
30008f08:	d407      	bmi.n	30008f1a <CPU_ClkGet+0x2e>
30008f0a:	4819      	ldr	r0, [pc, #100]	; (30008f70 <CPU_ClkGet+0x84>)
30008f0c:	bd70      	pop	{r4, r5, r6, pc}
30008f0e:	2c01      	cmp	r4, #1
30008f10:	d007      	beq.n	30008f22 <CPU_ClkGet+0x36>
30008f12:	2000      	movs	r0, #0
30008f14:	bd70      	pop	{r4, r5, r6, pc}
30008f16:	4817      	ldr	r0, [pc, #92]	; (30008f74 <CPU_ClkGet+0x88>)
30008f18:	bd70      	pop	{r4, r5, r6, pc}
30008f1a:	4b17      	ldr	r3, [pc, #92]	; (30008f78 <CPU_ClkGet+0x8c>)
30008f1c:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008f20:	4718      	bx	r3
30008f22:	2801      	cmp	r0, #1
30008f24:	d011      	beq.n	30008f4a <CPU_ClkGet+0x5e>
30008f26:	05f3      	lsls	r3, r6, #23
30008f28:	d51c      	bpl.n	30008f64 <CPU_ClkGet+0x78>
30008f2a:	4914      	ldr	r1, [pc, #80]	; (30008f7c <CPU_ClkGet+0x90>)
30008f2c:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30008f30:	6c48      	ldr	r0, [r1, #68]	; 0x44
30008f32:	4a13      	ldr	r2, [pc, #76]	; (30008f80 <CPU_ClkGet+0x94>)
30008f34:	f003 0307 	and.w	r3, r3, #7
30008f38:	f3c0 1045 	ubfx	r0, r0, #5, #6
30008f3c:	3301      	adds	r3, #1
30008f3e:	3002      	adds	r0, #2
30008f40:	fb02 f000 	mul.w	r0, r2, r0
30008f44:	fbb0 f0f3 	udiv	r0, r0, r3
30008f48:	bd70      	pop	{r4, r5, r6, pc}
30008f4a:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30008f4e:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30008f52:	f3c3 4007 	ubfx	r0, r3, #16, #8
30008f56:	d003      	beq.n	30008f60 <CPU_ClkGet+0x74>
30008f58:	4b0a      	ldr	r3, [pc, #40]	; (30008f84 <CPU_ClkGet+0x98>)
30008f5a:	fb03 f000 	mul.w	r0, r3, r0
30008f5e:	bd70      	pop	{r4, r5, r6, pc}
30008f60:	4809      	ldr	r0, [pc, #36]	; (30008f88 <CPU_ClkGet+0x9c>)
30008f62:	bd70      	pop	{r4, r5, r6, pc}
30008f64:	4806      	ldr	r0, [pc, #24]	; (30008f80 <CPU_ClkGet+0x94>)
30008f66:	bd70      	pop	{r4, r5, r6, pc}
30008f68:	42008000 	.word	0x42008000
30008f6c:	0000c0f9 	.word	0x0000c0f9
30008f70:	003d0900 	.word	0x003d0900
30008f74:	01312d00 	.word	0x01312d00
30008f78:	000099f5 	.word	0x000099f5
30008f7c:	42008800 	.word	0x42008800
30008f80:	02625a00 	.word	0x02625a00
30008f84:	000f4240 	.word	0x000f4240
30008f88:	02faf080 	.word	0x02faf080

30008f8c <RSIP_IV_Set>:
30008f8c:	4b05      	ldr	r3, [pc, #20]	; (30008fa4 <RSIP_IV_Set+0x18>)
30008f8e:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30008f92:	680b      	ldr	r3, [r1, #0]
30008f94:	ba1b      	rev	r3, r3
30008f96:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008f9a:	684b      	ldr	r3, [r1, #4]
30008f9c:	ba1b      	rev	r3, r3
30008f9e:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30008fa2:	4770      	bx	lr
30008fa4:	42008c00 	.word	0x42008c00

30008fa8 <RSIP_OTF_Cmd>:
30008fa8:	4a04      	ldr	r2, [pc, #16]	; (30008fbc <RSIP_OTF_Cmd+0x14>)
30008faa:	2801      	cmp	r0, #1
30008fac:	6953      	ldr	r3, [r2, #20]
30008fae:	bf0c      	ite	eq
30008fb0:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30008fb4:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30008fb8:	6153      	str	r3, [r2, #20]
30008fba:	4770      	bx	lr
30008fbc:	42008c00 	.word	0x42008c00

30008fc0 <RSIP_OTF_Enable>:
30008fc0:	b570      	push	{r4, r5, r6, lr}
30008fc2:	4604      	mov	r4, r0
30008fc4:	b1f3      	cbz	r3, 30009004 <RSIP_OTF_Enable+0x44>
30008fc6:	9b04      	ldr	r3, [sp, #16]
30008fc8:	460d      	mov	r5, r1
30008fca:	4616      	mov	r6, r2
30008fcc:	2b07      	cmp	r3, #7
30008fce:	d827      	bhi.n	30009020 <RSIP_OTF_Enable+0x60>
30008fd0:	9b05      	ldr	r3, [sp, #20]
30008fd2:	3b01      	subs	r3, #1
30008fd4:	2b01      	cmp	r3, #1
30008fd6:	d81d      	bhi.n	30009014 <RSIP_OTF_Enable+0x54>
30008fd8:	9a04      	ldr	r2, [sp, #16]
30008fda:	9b05      	ldr	r3, [sp, #20]
30008fdc:	f002 0007 	and.w	r0, r2, #7
30008fe0:	005b      	lsls	r3, r3, #1
30008fe2:	4305      	orrs	r5, r0
30008fe4:	4813      	ldr	r0, [pc, #76]	; (30009034 <RSIP_OTF_Enable+0x74>)
30008fe6:	f003 0306 	and.w	r3, r3, #6
30008fea:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30008fee:	ea43 0206 	orr.w	r2, r3, r6
30008ff2:	6c63      	ldr	r3, [r4, #68]	; 0x44
30008ff4:	f042 0201 	orr.w	r2, r2, #1
30008ff8:	f023 0301 	bic.w	r3, r3, #1
30008ffc:	6463      	str	r3, [r4, #68]	; 0x44
30008ffe:	6425      	str	r5, [r4, #64]	; 0x40
30009000:	6462      	str	r2, [r4, #68]	; 0x44
30009002:	bd70      	pop	{r4, r5, r6, pc}
30009004:	480b      	ldr	r0, [pc, #44]	; (30009034 <RSIP_OTF_Enable+0x74>)
30009006:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000900a:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000900c:	f023 0301 	bic.w	r3, r3, #1
30009010:	6463      	str	r3, [r4, #68]	; 0x44
30009012:	bd70      	pop	{r4, r5, r6, pc}
30009014:	f240 2131 	movw	r1, #561	; 0x231
30009018:	4807      	ldr	r0, [pc, #28]	; (30009038 <RSIP_OTF_Enable+0x78>)
3000901a:	f002 fce1 	bl	3000b9e0 <__io_assert_failed_veneer>
3000901e:	e7db      	b.n	30008fd8 <RSIP_OTF_Enable+0x18>
30009020:	f44f 710c 	mov.w	r1, #560	; 0x230
30009024:	4804      	ldr	r0, [pc, #16]	; (30009038 <RSIP_OTF_Enable+0x78>)
30009026:	f002 fcdb 	bl	3000b9e0 <__io_assert_failed_veneer>
3000902a:	9b05      	ldr	r3, [sp, #20]
3000902c:	3b01      	subs	r3, #1
3000902e:	2b01      	cmp	r3, #1
30009030:	d9d2      	bls.n	30008fd8 <RSIP_OTF_Enable+0x18>
30009032:	e7ef      	b.n	30009014 <RSIP_OTF_Enable+0x54>
30009034:	42008c00 	.word	0x42008c00
30009038:	3000d05c 	.word	0x3000d05c

3000903c <RSIP_MMU_Config>:
3000903c:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009040:	311f      	adds	r1, #31
30009042:	321f      	adds	r2, #31
30009044:	331f      	adds	r3, #31
30009046:	0080      	lsls	r0, r0, #2
30009048:	0949      	lsrs	r1, r1, #5
3000904a:	0952      	lsrs	r2, r2, #5
3000904c:	095b      	lsrs	r3, r3, #5
3000904e:	0249      	lsls	r1, r1, #9
30009050:	0252      	lsls	r2, r2, #9
30009052:	025b      	lsls	r3, r3, #9
30009054:	b410      	push	{r4}
30009056:	4c08      	ldr	r4, [pc, #32]	; (30009078 <RSIP_MMU_Config+0x3c>)
30009058:	4404      	add	r4, r0
3000905a:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
3000905e:	f020 0001 	bic.w	r0, r0, #1
30009062:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
30009066:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
3000906a:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
3000906e:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
30009072:	f85d 4b04 	ldr.w	r4, [sp], #4
30009076:	4770      	bx	lr
30009078:	42008c00 	.word	0x42008c00

3000907c <RSIP_MMU_Cmd>:
3000907c:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009080:	4b07      	ldr	r3, [pc, #28]	; (300090a0 <RSIP_MMU_Cmd+0x24>)
30009082:	0080      	lsls	r0, r0, #2
30009084:	4403      	add	r3, r0
30009086:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
3000908a:	b121      	cbz	r1, 30009096 <RSIP_MMU_Cmd+0x1a>
3000908c:	f042 0201 	orr.w	r2, r2, #1
30009090:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009094:	4770      	bx	lr
30009096:	f022 0201 	bic.w	r2, r2, #1
3000909a:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000909e:	4770      	bx	lr
300090a0:	42008c00 	.word	0x42008c00

300090a4 <RSIP_MMU_Cache_Clean>:
300090a4:	4a02      	ldr	r2, [pc, #8]	; (300090b0 <RSIP_MMU_Cache_Clean+0xc>)
300090a6:	6993      	ldr	r3, [r2, #24]
300090a8:	f043 0304 	orr.w	r3, r3, #4
300090ac:	6193      	str	r3, [r2, #24]
300090ae:	4770      	bx	lr
300090b0:	42008c00 	.word	0x42008c00

300090b4 <NAND_RxData>:
300090b4:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
300090b8:	460c      	mov	r4, r1
300090ba:	4616      	mov	r6, r2
300090bc:	461f      	mov	r7, r3
300090be:	2800      	cmp	r0, #0
300090c0:	f000 808a 	beq.w	300091d8 <NAND_RxData+0x124>
300090c4:	4605      	mov	r5, r0
300090c6:	f8df 911c 	ldr.w	r9, [pc, #284]	; 300091e4 <NAND_RxData+0x130>
300090ca:	2000      	movs	r0, #0
300090cc:	f8df 8118 	ldr.w	r8, [pc, #280]	; 300091e8 <NAND_RxData+0x134>
300090d0:	47c0      	blx	r8
300090d2:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300090d6:	2000      	movs	r0, #0
300090d8:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
300090dc:	6819      	ldr	r1, [r3, #0]
300090de:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
300090e2:	6019      	str	r1, [r3, #0]
300090e4:	6819      	ldr	r1, [r3, #0]
300090e6:	f441 7140 	orr.w	r1, r1, #768	; 0x300
300090ea:	6019      	str	r1, [r3, #0]
300090ec:	605a      	str	r2, [r3, #4]
300090ee:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
300090f2:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
300090f6:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
300090fa:	ea4f 420c 	mov.w	r2, ip, lsl #16
300090fe:	f1bc 0f03 	cmp.w	ip, #3
30009102:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30009106:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000910a:	ea42 0201 	orr.w	r2, r2, r1
3000910e:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30009112:	f04f 0201 	mov.w	r2, #1
30009116:	611a      	str	r2, [r3, #16]
30009118:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
3000911c:	bf08      	it	eq
3000911e:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30009122:	f3c4 2307 	ubfx	r3, r4, #8, #8
30009126:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000912a:	b2e4      	uxtb	r4, r4
3000912c:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009130:	2301      	movs	r3, #1
30009132:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30009136:	f017 0403 	ands.w	r4, r7, #3
3000913a:	6083      	str	r3, [r0, #8]
3000913c:	d137      	bne.n	300091ae <NAND_RxData+0xfa>
3000913e:	463b      	mov	r3, r7
30009140:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30009144:	42b4      	cmp	r4, r6
30009146:	d216      	bcs.n	30009176 <NAND_RxData+0xc2>
30009148:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
3000914c:	f012 0f3c 	tst.w	r2, #60	; 0x3c
30009150:	f3c2 0583 	ubfx	r5, r2, #2, #4
30009154:	d00c      	beq.n	30009170 <NAND_RxData+0xbc>
30009156:	00ad      	lsls	r5, r5, #2
30009158:	1f19      	subs	r1, r3, #4
3000915a:	f1a5 0c04 	sub.w	ip, r5, #4
3000915e:	449c      	add	ip, r3
30009160:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
30009164:	f841 0f04 	str.w	r0, [r1, #4]!
30009168:	458c      	cmp	ip, r1
3000916a:	d1f9      	bne.n	30009160 <NAND_RxData+0xac>
3000916c:	442b      	add	r3, r5
3000916e:	442c      	add	r4, r5
30009170:	1b32      	subs	r2, r6, r4
30009172:	2a03      	cmp	r2, #3
30009174:	d8e6      	bhi.n	30009144 <NAND_RxData+0x90>
30009176:	1e63      	subs	r3, r4, #1
30009178:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000917c:	441f      	add	r7, r3
3000917e:	e002      	b.n	30009186 <NAND_RxData+0xd2>
30009180:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009182:	071b      	lsls	r3, r3, #28
30009184:	d40d      	bmi.n	300091a2 <NAND_RxData+0xee>
30009186:	42b4      	cmp	r4, r6
30009188:	d3fa      	bcc.n	30009180 <NAND_RxData+0xcc>
3000918a:	2004      	movs	r0, #4
3000918c:	47c0      	blx	r8
3000918e:	2000      	movs	r0, #0
30009190:	47c0      	blx	r8
30009192:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009196:	6813      	ldr	r3, [r2, #0]
30009198:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000919c:	6013      	str	r3, [r2, #0]
3000919e:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
300091a2:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
300091a6:	3401      	adds	r4, #1
300091a8:	f807 3f01 	strb.w	r3, [r7, #1]!
300091ac:	e7eb      	b.n	30009186 <NAND_RxData+0xd2>
300091ae:	f1c4 0404 	rsb	r4, r4, #4
300091b2:	2200      	movs	r2, #0
300091b4:	4639      	mov	r1, r7
300091b6:	e00a      	b.n	300091ce <NAND_RxData+0x11a>
300091b8:	6a85      	ldr	r5, [r0, #40]	; 0x28
300091ba:	072d      	lsls	r5, r5, #28
300091bc:	d505      	bpl.n	300091ca <NAND_RxData+0x116>
300091be:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
300091c2:	3201      	adds	r2, #1
300091c4:	700b      	strb	r3, [r1, #0]
300091c6:	18b9      	adds	r1, r7, r2
300091c8:	460b      	mov	r3, r1
300091ca:	4294      	cmp	r4, r2
300091cc:	d0b8      	beq.n	30009140 <NAND_RxData+0x8c>
300091ce:	42b2      	cmp	r2, r6
300091d0:	460b      	mov	r3, r1
300091d2:	d3f1      	bcc.n	300091b8 <NAND_RxData+0x104>
300091d4:	4614      	mov	r4, r2
300091d6:	e7ce      	b.n	30009176 <NAND_RxData+0xc2>
300091d8:	f8df 9008 	ldr.w	r9, [pc, #8]	; 300091e4 <NAND_RxData+0x130>
300091dc:	f899 5008 	ldrb.w	r5, [r9, #8]
300091e0:	e773      	b.n	300090ca <NAND_RxData+0x16>
300091e2:	bf00      	nop
300091e4:	2001c01c 	.word	0x2001c01c
300091e8:	0000b9c1 	.word	0x0000b9c1

300091ec <NAND_Page_Read>:
300091ec:	b530      	push	{r4, r5, lr}
300091ee:	b085      	sub	sp, #20
300091f0:	4c07      	ldr	r4, [pc, #28]	; (30009210 <NAND_Page_Read+0x24>)
300091f2:	4d08      	ldr	r5, [pc, #32]	; (30009214 <NAND_Page_Read+0x28>)
300091f4:	9301      	str	r3, [sp, #4]
300091f6:	e9cd 2102 	strd	r2, r1, [sp, #8]
300091fa:	47a0      	blx	r4
300091fc:	4604      	mov	r4, r0
300091fe:	9903      	ldr	r1, [sp, #12]
30009200:	7a28      	ldrb	r0, [r5, #8]
30009202:	4d05      	ldr	r5, [pc, #20]	; (30009218 <NAND_Page_Read+0x2c>)
30009204:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30009208:	47a8      	blx	r5
3000920a:	4620      	mov	r0, r4
3000920c:	b005      	add	sp, #20
3000920e:	bd30      	pop	{r4, r5, pc}
30009210:	0000bd85 	.word	0x0000bd85
30009214:	2001c01c 	.word	0x2001c01c
30009218:	300090b5 	.word	0x300090b5

3000921c <SYSCFG_BootFromNor>:
3000921c:	4b0e      	ldr	r3, [pc, #56]	; (30009258 <SYSCFG_BootFromNor+0x3c>)
3000921e:	b510      	push	{r4, lr}
30009220:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30009224:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009228:	0392      	lsls	r2, r2, #14
3000922a:	d50a      	bpl.n	30009242 <SYSCFG_BootFromNor+0x26>
3000922c:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009230:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30009234:	d105      	bne.n	30009242 <SYSCFG_BootFromNor+0x26>
30009236:	4b09      	ldr	r3, [pc, #36]	; (3000925c <SYSCFG_BootFromNor+0x40>)
30009238:	4798      	blx	r3
3000923a:	3800      	subs	r0, #0
3000923c:	bf18      	it	ne
3000923e:	2001      	movne	r0, #1
30009240:	bd10      	pop	{r4, pc}
30009242:	4c07      	ldr	r4, [pc, #28]	; (30009260 <SYSCFG_BootFromNor+0x44>)
30009244:	47a0      	blx	r4
30009246:	2802      	cmp	r0, #2
30009248:	d004      	beq.n	30009254 <SYSCFG_BootFromNor+0x38>
3000924a:	47a0      	blx	r4
3000924c:	2801      	cmp	r0, #1
3000924e:	d1f2      	bne.n	30009236 <SYSCFG_BootFromNor+0x1a>
30009250:	2000      	movs	r0, #0
30009252:	bd10      	pop	{r4, pc}
30009254:	2001      	movs	r0, #1
30009256:	bd10      	pop	{r4, pc}
30009258:	42008000 	.word	0x42008000
3000925c:	0000c0bd 	.word	0x0000c0bd
30009260:	0000c1a9 	.word	0x0000c1a9

30009264 <SYSCFG_RLVersion>:
30009264:	4b07      	ldr	r3, [pc, #28]	; (30009284 <SYSCFG_RLVersion+0x20>)
30009266:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000926a:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000926e:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30009272:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30009276:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000927a:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000927e:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009282:	4770      	bx	lr
30009284:	42008000 	.word	0x42008000

30009288 <SBOOT_GetMdType>:
30009288:	b470      	push	{r4, r5, r6}
3000928a:	4c0c      	ldr	r4, [pc, #48]	; (300092bc <SBOOT_GetMdType+0x34>)
3000928c:	4e0c      	ldr	r6, [pc, #48]	; (300092c0 <SBOOT_GetMdType+0x38>)
3000928e:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009292:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009296:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
3000929a:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000929e:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
300092a2:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
300092a6:	f3c3 4303 	ubfx	r3, r3, #16, #4
300092aa:	6033      	str	r3, [r6, #0]
300092ac:	b913      	cbnz	r3, 300092b4 <SBOOT_GetMdType+0x2c>
300092ae:	bc70      	pop	{r4, r5, r6}
300092b0:	f002 bc26 	b.w	3000bb00 <__SBOOT_GetMdType_A_veneer>
300092b4:	bc70      	pop	{r4, r5, r6}
300092b6:	f002 bc43 	b.w	3000bb40 <__SBOOT_GetMdType_B_veneer>
300092ba:	bf00      	nop
300092bc:	42008000 	.word	0x42008000
300092c0:	3000f364 	.word	0x3000f364

300092c4 <CRYPTO_OTPKey_SHA_Init>:
300092c4:	4a0d      	ldr	r2, [pc, #52]	; (300092fc <CRYPTO_OTPKey_SHA_Init+0x38>)
300092c6:	b410      	push	{r4}
300092c8:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
300092cc:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
300092d0:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
300092d4:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
300092d8:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
300092dc:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
300092e0:	f3c3 4303 	ubfx	r3, r3, #16, #4
300092e4:	4a06      	ldr	r2, [pc, #24]	; (30009300 <CRYPTO_OTPKey_SHA_Init+0x3c>)
300092e6:	6013      	str	r3, [r2, #0]
300092e8:	b91b      	cbnz	r3, 300092f2 <CRYPTO_OTPKey_SHA_Init+0x2e>
300092ea:	f85d 4b04 	ldr.w	r4, [sp], #4
300092ee:	f002 bbef 	b.w	3000bad0 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
300092f2:	f85d 4b04 	ldr.w	r4, [sp], #4
300092f6:	f002 bba3 	b.w	3000ba40 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
300092fa:	bf00      	nop
300092fc:	42008000 	.word	0x42008000
30009300:	3000f364 	.word	0x3000f364

30009304 <rtl_crypto_hmac_sha2_update>:
30009304:	b470      	push	{r4, r5, r6}
30009306:	4c0c      	ldr	r4, [pc, #48]	; (30009338 <rtl_crypto_hmac_sha2_update+0x34>)
30009308:	4e0c      	ldr	r6, [pc, #48]	; (3000933c <rtl_crypto_hmac_sha2_update+0x38>)
3000930a:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000930e:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009312:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009316:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000931a:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000931e:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009322:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009326:	6033      	str	r3, [r6, #0]
30009328:	b913      	cbnz	r3, 30009330 <rtl_crypto_hmac_sha2_update+0x2c>
3000932a:	bc70      	pop	{r4, r5, r6}
3000932c:	f002 bb78 	b.w	3000ba20 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009330:	bc70      	pop	{r4, r5, r6}
30009332:	f002 bc25 	b.w	3000bb80 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009336:	bf00      	nop
30009338:	42008000 	.word	0x42008000
3000933c:	3000f364 	.word	0x3000f364

30009340 <rtl_crypto_hmac_sha2_final>:
30009340:	4a0d      	ldr	r2, [pc, #52]	; (30009378 <rtl_crypto_hmac_sha2_final+0x38>)
30009342:	b410      	push	{r4}
30009344:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009348:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
3000934c:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009350:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009354:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009358:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
3000935c:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009360:	4a06      	ldr	r2, [pc, #24]	; (3000937c <rtl_crypto_hmac_sha2_final+0x3c>)
30009362:	6013      	str	r3, [r2, #0]
30009364:	b91b      	cbnz	r3, 3000936e <rtl_crypto_hmac_sha2_final+0x2e>
30009366:	f85d 4b04 	ldr.w	r4, [sp], #4
3000936a:	f002 bc11 	b.w	3000bb90 <__rtl_crypto_hmac_sha2_final_A_veneer>
3000936e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009372:	f002 bb7d 	b.w	3000ba70 <__rtl_crypto_hmac_sha2_final_B_veneer>
30009376:	bf00      	nop
30009378:	42008000 	.word	0x42008000
3000937c:	3000f364 	.word	0x3000f364

30009380 <rtl_crypto_sha2_init>:
30009380:	4a0d      	ldr	r2, [pc, #52]	; (300093b8 <rtl_crypto_sha2_init+0x38>)
30009382:	b410      	push	{r4}
30009384:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009388:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
3000938c:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009390:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009394:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009398:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
3000939c:	f3c3 4303 	ubfx	r3, r3, #16, #4
300093a0:	4a06      	ldr	r2, [pc, #24]	; (300093bc <rtl_crypto_sha2_init+0x3c>)
300093a2:	6013      	str	r3, [r2, #0]
300093a4:	b91b      	cbnz	r3, 300093ae <rtl_crypto_sha2_init+0x2e>
300093a6:	f85d 4b04 	ldr.w	r4, [sp], #4
300093aa:	f002 bbe1 	b.w	3000bb70 <__rtl_crypto_sha2_init_A_veneer>
300093ae:	f85d 4b04 	ldr.w	r4, [sp], #4
300093b2:	f002 bb95 	b.w	3000bae0 <__rtl_crypto_sha2_init_B_veneer>
300093b6:	bf00      	nop
300093b8:	42008000 	.word	0x42008000
300093bc:	3000f364 	.word	0x3000f364

300093c0 <rtl_crypto_sha2_update>:
300093c0:	b470      	push	{r4, r5, r6}
300093c2:	4c0c      	ldr	r4, [pc, #48]	; (300093f4 <rtl_crypto_sha2_update+0x34>)
300093c4:	4e0c      	ldr	r6, [pc, #48]	; (300093f8 <rtl_crypto_sha2_update+0x38>)
300093c6:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
300093ca:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
300093ce:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
300093d2:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
300093d6:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
300093da:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
300093de:	f3c3 4303 	ubfx	r3, r3, #16, #4
300093e2:	6033      	str	r3, [r6, #0]
300093e4:	b913      	cbnz	r3, 300093ec <rtl_crypto_sha2_update+0x2c>
300093e6:	bc70      	pop	{r4, r5, r6}
300093e8:	f002 bbda 	b.w	3000bba0 <__rtl_crypto_sha2_update_A_veneer>
300093ec:	bc70      	pop	{r4, r5, r6}
300093ee:	f002 bbaf 	b.w	3000bb50 <__rtl_crypto_sha2_update_B_veneer>
300093f2:	bf00      	nop
300093f4:	42008000 	.word	0x42008000
300093f8:	3000f364 	.word	0x3000f364

300093fc <rtl_crypto_sha2_final>:
300093fc:	4a0d      	ldr	r2, [pc, #52]	; (30009434 <rtl_crypto_sha2_final+0x38>)
300093fe:	b410      	push	{r4}
30009400:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009404:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009408:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
3000940c:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009410:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009414:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009418:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000941c:	4a06      	ldr	r2, [pc, #24]	; (30009438 <rtl_crypto_sha2_final+0x3c>)
3000941e:	6013      	str	r3, [r2, #0]
30009420:	b91b      	cbnz	r3, 3000942a <rtl_crypto_sha2_final+0x2e>
30009422:	f85d 4b04 	ldr.w	r4, [sp], #4
30009426:	f002 bb03 	b.w	3000ba30 <__rtl_crypto_sha2_final_A_veneer>
3000942a:	f85d 4b04 	ldr.w	r4, [sp], #4
3000942e:	f002 bb6f 	b.w	3000bb10 <__rtl_crypto_sha2_final_B_veneer>
30009432:	bf00      	nop
30009434:	42008000 	.word	0x42008000
30009438:	3000f364 	.word	0x3000f364

3000943c <SBOOT_Validate_PubKey>:
3000943c:	b470      	push	{r4, r5, r6}
3000943e:	4c0c      	ldr	r4, [pc, #48]	; (30009470 <SBOOT_Validate_PubKey+0x34>)
30009440:	4e0c      	ldr	r6, [pc, #48]	; (30009474 <SBOOT_Validate_PubKey+0x38>)
30009442:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009446:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
3000944a:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
3000944e:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009452:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009456:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
3000945a:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000945e:	6033      	str	r3, [r6, #0]
30009460:	b913      	cbnz	r3, 30009468 <SBOOT_Validate_PubKey+0x2c>
30009462:	bc70      	pop	{r4, r5, r6}
30009464:	f002 bb0c 	b.w	3000ba80 <__SBOOT_Validate_PubKey_A_veneer>
30009468:	bc70      	pop	{r4, r5, r6}
3000946a:	f002 baf1 	b.w	3000ba50 <__SBOOT_Validate_PubKey_B_veneer>
3000946e:	bf00      	nop
30009470:	42008000 	.word	0x42008000
30009474:	3000f364 	.word	0x3000f364

30009478 <SBOOT_Validate_Signature>:
30009478:	b5f0      	push	{r4, r5, r6, r7, lr}
3000947a:	4d10      	ldr	r5, [pc, #64]	; (300094bc <SBOOT_Validate_Signature+0x44>)
3000947c:	9f05      	ldr	r7, [sp, #20]
3000947e:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009482:	f8dd c018 	ldr.w	ip, [sp, #24]
30009486:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000948a:	f8df e034 	ldr.w	lr, [pc, #52]	; 300094c0 <SBOOT_Validate_Signature+0x48>
3000948e:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009492:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009496:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000949a:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000949e:	f3c4 4403 	ubfx	r4, r4, #16, #4
300094a2:	e9cd 7c05 	strd	r7, ip, [sp, #20]
300094a6:	f8ce 4000 	str.w	r4, [lr]
300094aa:	b91c      	cbnz	r4, 300094b4 <SBOOT_Validate_Signature+0x3c>
300094ac:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
300094b0:	f002 bb56 	b.w	3000bb60 <__SBOOT_Validate_Signature_A_veneer>
300094b4:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
300094b8:	f002 bb3a 	b.w	3000bb30 <__SBOOT_Validate_Signature_B_veneer>
300094bc:	42008000 	.word	0x42008000
300094c0:	3000f364 	.word	0x3000f364

300094c4 <SBOOT_Validate_ImgHash>:
300094c4:	b4f0      	push	{r4, r5, r6, r7}
300094c6:	4d0c      	ldr	r5, [pc, #48]	; (300094f8 <SBOOT_Validate_ImgHash+0x34>)
300094c8:	4f0c      	ldr	r7, [pc, #48]	; (300094fc <SBOOT_Validate_ImgHash+0x38>)
300094ca:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
300094ce:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
300094d2:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
300094d6:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
300094da:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
300094de:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
300094e2:	f3c4 4403 	ubfx	r4, r4, #16, #4
300094e6:	603c      	str	r4, [r7, #0]
300094e8:	b914      	cbnz	r4, 300094f0 <SBOOT_Validate_ImgHash+0x2c>
300094ea:	bcf0      	pop	{r4, r5, r6, r7}
300094ec:	f002 ba90 	b.w	3000ba10 <__SBOOT_Validate_ImgHash_A_veneer>
300094f0:	bcf0      	pop	{r4, r5, r6, r7}
300094f2:	f002 bad5 	b.w	3000baa0 <__SBOOT_Validate_ImgHash_B_veneer>
300094f6:	bf00      	nop
300094f8:	42008000 	.word	0x42008000
300094fc:	3000f364 	.word	0x3000f364

30009500 <CRYPTO_SHA_Init>:
30009500:	4b0b      	ldr	r3, [pc, #44]	; (30009530 <CRYPTO_SHA_Init+0x30>)
30009502:	490c      	ldr	r1, [pc, #48]	; (30009534 <CRYPTO_SHA_Init+0x34>)
30009504:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009508:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000950c:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
30009510:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
30009514:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009518:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000951c:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009520:	6008      	str	r0, [r1, #0]
30009522:	b908      	cbnz	r0, 30009528 <CRYPTO_SHA_Init+0x28>
30009524:	f002 ba54 	b.w	3000b9d0 <__CRYPTO_SHA_Init_A_veneer>
30009528:	2000      	movs	r0, #0
3000952a:	f002 ba61 	b.w	3000b9f0 <__CRYPTO_SHA_Init_B_veneer>
3000952e:	bf00      	nop
30009530:	42008000 	.word	0x42008000
30009534:	3000f364 	.word	0x3000f364

30009538 <SBOOT_Validate_Algorithm>:
30009538:	b4f0      	push	{r4, r5, r6, r7}
3000953a:	4d0c      	ldr	r5, [pc, #48]	; (3000956c <SBOOT_Validate_Algorithm+0x34>)
3000953c:	4f0c      	ldr	r7, [pc, #48]	; (30009570 <SBOOT_Validate_Algorithm+0x38>)
3000953e:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009542:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009546:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000954a:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000954e:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009552:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009556:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000955a:	603c      	str	r4, [r7, #0]
3000955c:	b914      	cbnz	r4, 30009564 <SBOOT_Validate_Algorithm+0x2c>
3000955e:	bcf0      	pop	{r4, r5, r6, r7}
30009560:	f002 baa6 	b.w	3000bab0 <__SBOOT_Validate_Algorithm_A_veneer>
30009564:	bcf0      	pop	{r4, r5, r6, r7}
30009566:	f002 baab 	b.w	3000bac0 <__SBOOT_Validate_Algorithm_B_veneer>
3000956a:	bf00      	nop
3000956c:	42008000 	.word	0x42008000
30009570:	3000f364 	.word	0x3000f364

30009574 <ddr_init_index>:
30009574:	b570      	push	{r4, r5, r6, lr}
30009576:	4b0f      	ldr	r3, [pc, #60]	; (300095b4 <ddr_init_index+0x40>)
30009578:	4798      	blx	r3
3000957a:	4604      	mov	r4, r0
3000957c:	4b0e      	ldr	r3, [pc, #56]	; (300095b8 <ddr_init_index+0x44>)
3000957e:	4798      	blx	r3
30009580:	2c03      	cmp	r4, #3
30009582:	d014      	beq.n	300095ae <ddr_init_index+0x3a>
30009584:	2c01      	cmp	r4, #1
30009586:	d00c      	beq.n	300095a2 <ddr_init_index+0x2e>
30009588:	4e0c      	ldr	r6, [pc, #48]	; (300095bc <ddr_init_index+0x48>)
3000958a:	4d0d      	ldr	r5, [pc, #52]	; (300095c0 <ddr_init_index+0x4c>)
3000958c:	4c0d      	ldr	r4, [pc, #52]	; (300095c4 <ddr_init_index+0x50>)
3000958e:	462b      	mov	r3, r5
30009590:	4622      	mov	r2, r4
30009592:	490b      	ldr	r1, [pc, #44]	; (300095c0 <ddr_init_index+0x4c>)
30009594:	2002      	movs	r0, #2
30009596:	f001 ffdd 	bl	3000b554 <rtk_log_write>
3000959a:	f242 7010 	movw	r0, #10000	; 0x2710
3000959e:	47b0      	blx	r6
300095a0:	e7f5      	b.n	3000958e <ddr_init_index+0x1a>
300095a2:	2802      	cmp	r0, #2
300095a4:	d003      	beq.n	300095ae <ddr_init_index+0x3a>
300095a6:	2803      	cmp	r0, #3
300095a8:	d1ee      	bne.n	30009588 <ddr_init_index+0x14>
300095aa:	2000      	movs	r0, #0
300095ac:	bd70      	pop	{r4, r5, r6, pc}
300095ae:	4620      	mov	r0, r4
300095b0:	bd70      	pop	{r4, r5, r6, pc}
300095b2:	bf00      	nop
300095b4:	30007631 	.word	0x30007631
300095b8:	300074ad 	.word	0x300074ad
300095bc:	00009be5 	.word	0x00009be5
300095c0:	3000d0a4 	.word	0x3000d0a4
300095c4:	3000d0a8 	.word	0x3000d0a8

300095c8 <rxi316_perf_tune>:
300095c8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300095cc:	4606      	mov	r6, r0
300095ce:	6800      	ldr	r0, [r0, #0]
300095d0:	b0c3      	sub	sp, #268	; 0x10c
300095d2:	7985      	ldrb	r5, [r0, #6]
300095d4:	2d00      	cmp	r5, #0
300095d6:	f000 8334 	beq.w	30009c42 <rxi316_perf_tune+0x67a>
300095da:	2d01      	cmp	r5, #1
300095dc:	d003      	beq.n	300095e6 <rxi316_perf_tune+0x1e>
300095de:	2d03      	cmp	r5, #3
300095e0:	bf14      	ite	ne
300095e2:	2502      	movne	r5, #2
300095e4:	2500      	moveq	r5, #0
300095e6:	68f3      	ldr	r3, [r6, #12]
300095e8:	2420      	movs	r4, #32
300095ea:	4684      	mov	ip, r0
300095ec:	6859      	ldr	r1, [r3, #4]
300095ee:	46a3      	mov	fp, r4
300095f0:	46a2      	mov	sl, r4
300095f2:	46a0      	mov	r8, r4
300095f4:	784b      	ldrb	r3, [r1, #1]
300095f6:	46a1      	mov	r9, r4
300095f8:	7bcf      	ldrb	r7, [r1, #15]
300095fa:	9323      	str	r3, [sp, #140]	; 0x8c
300095fc:	788b      	ldrb	r3, [r1, #2]
300095fe:	780a      	ldrb	r2, [r1, #0]
30009600:	9324      	str	r3, [sp, #144]	; 0x90
30009602:	78cb      	ldrb	r3, [r1, #3]
30009604:	9325      	str	r3, [sp, #148]	; 0x94
30009606:	790b      	ldrb	r3, [r1, #4]
30009608:	9326      	str	r3, [sp, #152]	; 0x98
3000960a:	794b      	ldrb	r3, [r1, #5]
3000960c:	9327      	str	r3, [sp, #156]	; 0x9c
3000960e:	798b      	ldrb	r3, [r1, #6]
30009610:	9328      	str	r3, [sp, #160]	; 0xa0
30009612:	79cb      	ldrb	r3, [r1, #7]
30009614:	9329      	str	r3, [sp, #164]	; 0xa4
30009616:	7a0b      	ldrb	r3, [r1, #8]
30009618:	932a      	str	r3, [sp, #168]	; 0xa8
3000961a:	7a4b      	ldrb	r3, [r1, #9]
3000961c:	932b      	str	r3, [sp, #172]	; 0xac
3000961e:	7a8b      	ldrb	r3, [r1, #10]
30009620:	932c      	str	r3, [sp, #176]	; 0xb0
30009622:	7acb      	ldrb	r3, [r1, #11]
30009624:	932d      	str	r3, [sp, #180]	; 0xb4
30009626:	7b0b      	ldrb	r3, [r1, #12]
30009628:	932e      	str	r3, [sp, #184]	; 0xb8
3000962a:	7b4b      	ldrb	r3, [r1, #13]
3000962c:	932f      	str	r3, [sp, #188]	; 0xbc
3000962e:	7b8b      	ldrb	r3, [r1, #14]
30009630:	9330      	str	r3, [sp, #192]	; 0xc0
30009632:	2300      	movs	r3, #0
30009634:	9731      	str	r7, [sp, #196]	; 0xc4
30009636:	7c0f      	ldrb	r7, [r1, #16]
30009638:	941d      	str	r4, [sp, #116]	; 0x74
3000963a:	9732      	str	r7, [sp, #200]	; 0xc8
3000963c:	7c4f      	ldrb	r7, [r1, #17]
3000963e:	9733      	str	r7, [sp, #204]	; 0xcc
30009640:	7c8f      	ldrb	r7, [r1, #18]
30009642:	9734      	str	r7, [sp, #208]	; 0xd0
30009644:	7ccf      	ldrb	r7, [r1, #19]
30009646:	9735      	str	r7, [sp, #212]	; 0xd4
30009648:	7d0f      	ldrb	r7, [r1, #20]
3000964a:	9736      	str	r7, [sp, #216]	; 0xd8
3000964c:	7d4f      	ldrb	r7, [r1, #21]
3000964e:	9737      	str	r7, [sp, #220]	; 0xdc
30009650:	7d8f      	ldrb	r7, [r1, #22]
30009652:	9738      	str	r7, [sp, #224]	; 0xe0
30009654:	7dcf      	ldrb	r7, [r1, #23]
30009656:	9739      	str	r7, [sp, #228]	; 0xe4
30009658:	7e0f      	ldrb	r7, [r1, #24]
3000965a:	973a      	str	r7, [sp, #232]	; 0xe8
3000965c:	7e4f      	ldrb	r7, [r1, #25]
3000965e:	973b      	str	r7, [sp, #236]	; 0xec
30009660:	7e8f      	ldrb	r7, [r1, #26]
30009662:	973c      	str	r7, [sp, #240]	; 0xf0
30009664:	7ecf      	ldrb	r7, [r1, #27]
30009666:	973d      	str	r7, [sp, #244]	; 0xf4
30009668:	7f0f      	ldrb	r7, [r1, #28]
3000966a:	973e      	str	r7, [sp, #248]	; 0xf8
3000966c:	7f4f      	ldrb	r7, [r1, #29]
3000966e:	973f      	str	r7, [sp, #252]	; 0xfc
30009670:	7f8f      	ldrb	r7, [r1, #30]
30009672:	7fc9      	ldrb	r1, [r1, #31]
30009674:	9740      	str	r7, [sp, #256]	; 0x100
30009676:	4627      	mov	r7, r4
30009678:	9141      	str	r1, [sp, #260]	; 0x104
3000967a:	4619      	mov	r1, r3
3000967c:	9409      	str	r4, [sp, #36]	; 0x24
3000967e:	941c      	str	r4, [sp, #112]	; 0x70
30009680:	9404      	str	r4, [sp, #16]
30009682:	941b      	str	r4, [sp, #108]	; 0x6c
30009684:	940a      	str	r4, [sp, #40]	; 0x28
30009686:	9303      	str	r3, [sp, #12]
30009688:	e9cd 4405 	strd	r4, r4, [sp, #20]
3000968c:	e9cd 4401 	strd	r4, r4, [sp, #4]
30009690:	e9cd 4407 	strd	r4, r4, [sp, #28]
30009694:	e9cd 4419 	strd	r4, r4, [sp, #100]	; 0x64
30009698:	e9cd 4417 	strd	r4, r4, [sp, #92]	; 0x5c
3000969c:	e9cd 4415 	strd	r4, r4, [sp, #84]	; 0x54
300096a0:	e9cd 4413 	strd	r4, r4, [sp, #76]	; 0x4c
300096a4:	e9cd 4411 	strd	r4, r4, [sp, #68]	; 0x44
300096a8:	e9cd 440f 	strd	r4, r4, [sp, #60]	; 0x3c
300096ac:	e9cd 440d 	strd	r4, r4, [sp, #52]	; 0x34
300096b0:	e9cd 440b 	strd	r4, r4, [sp, #44]	; 0x2c
300096b4:	b94a      	cbnz	r2, 300096ca <rxi316_perf_tune+0x102>
300096b6:	3101      	adds	r1, #1
300096b8:	4699      	mov	r9, r3
300096ba:	3301      	adds	r3, #1
300096bc:	2b20      	cmp	r3, #32
300096be:	d069      	beq.n	30009794 <rxi316_perf_tune+0x1cc>
300096c0:	aa22      	add	r2, sp, #136	; 0x88
300096c2:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
300096c6:	2a00      	cmp	r2, #0
300096c8:	d0f5      	beq.n	300096b6 <rxi316_perf_tune+0xee>
300096ca:	2a01      	cmp	r2, #1
300096cc:	d05d      	beq.n	3000978a <rxi316_perf_tune+0x1c2>
300096ce:	2a02      	cmp	r2, #2
300096d0:	d06e      	beq.n	300097b0 <rxi316_perf_tune+0x1e8>
300096d2:	2a03      	cmp	r2, #3
300096d4:	d06f      	beq.n	300097b6 <rxi316_perf_tune+0x1ee>
300096d6:	2a04      	cmp	r2, #4
300096d8:	d070      	beq.n	300097bc <rxi316_perf_tune+0x1f4>
300096da:	2a05      	cmp	r2, #5
300096dc:	d071      	beq.n	300097c2 <rxi316_perf_tune+0x1fa>
300096de:	2a06      	cmp	r2, #6
300096e0:	d072      	beq.n	300097c8 <rxi316_perf_tune+0x200>
300096e2:	2a07      	cmp	r2, #7
300096e4:	f000 82af 	beq.w	30009c46 <rxi316_perf_tune+0x67e>
300096e8:	2a08      	cmp	r2, #8
300096ea:	f000 8381 	beq.w	30009df0 <rxi316_perf_tune+0x828>
300096ee:	2a09      	cmp	r2, #9
300096f0:	f000 8386 	beq.w	30009e00 <rxi316_perf_tune+0x838>
300096f4:	2a0a      	cmp	r2, #10
300096f6:	f000 838e 	beq.w	30009e16 <rxi316_perf_tune+0x84e>
300096fa:	2a0b      	cmp	r2, #11
300096fc:	f000 8386 	beq.w	30009e0c <rxi316_perf_tune+0x844>
30009700:	2a0c      	cmp	r2, #12
30009702:	f000 838d 	beq.w	30009e20 <rxi316_perf_tune+0x858>
30009706:	2a0d      	cmp	r2, #13
30009708:	f000 8394 	beq.w	30009e34 <rxi316_perf_tune+0x86c>
3000970c:	2a0e      	cmp	r2, #14
3000970e:	f000 838c 	beq.w	30009e2a <rxi316_perf_tune+0x862>
30009712:	2a0f      	cmp	r2, #15
30009714:	f000 8393 	beq.w	30009e3e <rxi316_perf_tune+0x876>
30009718:	2a10      	cmp	r2, #16
3000971a:	f000 8395 	beq.w	30009e48 <rxi316_perf_tune+0x880>
3000971e:	2a11      	cmp	r2, #17
30009720:	f000 8394 	beq.w	30009e4c <rxi316_perf_tune+0x884>
30009724:	2a12      	cmp	r2, #18
30009726:	f000 8393 	beq.w	30009e50 <rxi316_perf_tune+0x888>
3000972a:	2a13      	cmp	r2, #19
3000972c:	f000 8394 	beq.w	30009e58 <rxi316_perf_tune+0x890>
30009730:	2a14      	cmp	r2, #20
30009732:	f000 838f 	beq.w	30009e54 <rxi316_perf_tune+0x88c>
30009736:	2a15      	cmp	r2, #21
30009738:	f000 8396 	beq.w	30009e68 <rxi316_perf_tune+0x8a0>
3000973c:	2a16      	cmp	r2, #22
3000973e:	f000 8391 	beq.w	30009e64 <rxi316_perf_tune+0x89c>
30009742:	2a17      	cmp	r2, #23
30009744:	f000 83a0 	beq.w	30009e88 <rxi316_perf_tune+0x8c0>
30009748:	2a18      	cmp	r2, #24
3000974a:	f000 839b 	beq.w	30009e84 <rxi316_perf_tune+0x8bc>
3000974e:	2a19      	cmp	r2, #25
30009750:	f000 8396 	beq.w	30009e80 <rxi316_perf_tune+0x8b8>
30009754:	2a1a      	cmp	r2, #26
30009756:	f000 8391 	beq.w	30009e7c <rxi316_perf_tune+0x8b4>
3000975a:	2a1b      	cmp	r2, #27
3000975c:	f000 838c 	beq.w	30009e78 <rxi316_perf_tune+0x8b0>
30009760:	2a1c      	cmp	r2, #28
30009762:	f000 8387 	beq.w	30009e74 <rxi316_perf_tune+0x8ac>
30009766:	2a1d      	cmp	r2, #29
30009768:	f000 8382 	beq.w	30009e70 <rxi316_perf_tune+0x8a8>
3000976c:	2a1e      	cmp	r2, #30
3000976e:	f000 837d 	beq.w	30009e6c <rxi316_perf_tune+0x8a4>
30009772:	2a1f      	cmp	r2, #31
30009774:	f000 8374 	beq.w	30009e60 <rxi316_perf_tune+0x898>
30009778:	2a20      	cmp	r2, #32
3000977a:	f000 836f 	beq.w	30009e5c <rxi316_perf_tune+0x894>
3000977e:	9809      	ldr	r0, [sp, #36]	; 0x24
30009780:	2a21      	cmp	r2, #33	; 0x21
30009782:	bf08      	it	eq
30009784:	4618      	moveq	r0, r3
30009786:	9009      	str	r0, [sp, #36]	; 0x24
30009788:	e797      	b.n	300096ba <rxi316_perf_tune+0xf2>
3000978a:	4698      	mov	r8, r3
3000978c:	3301      	adds	r3, #1
3000978e:	3101      	adds	r1, #1
30009790:	2b20      	cmp	r3, #32
30009792:	d195      	bne.n	300096c0 <rxi316_perf_tune+0xf8>
30009794:	f89c 2002 	ldrb.w	r2, [ip, #2]
30009798:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000979c:	b1ba      	cbz	r2, 300097ce <rxi316_perf_tune+0x206>
3000979e:	2a01      	cmp	r2, #1
300097a0:	f000 8331 	beq.w	30009e06 <rxi316_perf_tune+0x83e>
300097a4:	2a03      	cmp	r2, #3
300097a6:	bf0c      	ite	eq
300097a8:	2204      	moveq	r2, #4
300097aa:	2203      	movne	r2, #3
300097ac:	921e      	str	r2, [sp, #120]	; 0x78
300097ae:	e010      	b.n	300097d2 <rxi316_perf_tune+0x20a>
300097b0:	3101      	adds	r1, #1
300097b2:	461f      	mov	r7, r3
300097b4:	e781      	b.n	300096ba <rxi316_perf_tune+0xf2>
300097b6:	3101      	adds	r1, #1
300097b8:	469a      	mov	sl, r3
300097ba:	e77e      	b.n	300096ba <rxi316_perf_tune+0xf2>
300097bc:	3101      	adds	r1, #1
300097be:	469b      	mov	fp, r3
300097c0:	e77b      	b.n	300096ba <rxi316_perf_tune+0xf2>
300097c2:	3101      	adds	r1, #1
300097c4:	9301      	str	r3, [sp, #4]
300097c6:	e778      	b.n	300096ba <rxi316_perf_tune+0xf2>
300097c8:	3101      	adds	r1, #1
300097ca:	9302      	str	r3, [sp, #8]
300097cc:	e775      	b.n	300096ba <rxi316_perf_tune+0xf2>
300097ce:	2201      	movs	r2, #1
300097d0:	921e      	str	r2, [sp, #120]	; 0x78
300097d2:	3b02      	subs	r3, #2
300097d4:	428b      	cmp	r3, r1
300097d6:	d005      	beq.n	300097e4 <rxi316_perf_tune+0x21c>
300097d8:	4bac      	ldr	r3, [pc, #688]	; (30009a8c <rxi316_perf_tune+0x4c4>)
300097da:	2002      	movs	r0, #2
300097dc:	4aac      	ldr	r2, [pc, #688]	; (30009a90 <rxi316_perf_tune+0x4c8>)
300097de:	4619      	mov	r1, r3
300097e0:	f001 feb8 	bl	3000b554 <rtk_log_write>
300097e4:	9b1e      	ldr	r3, [sp, #120]	; 0x78
300097e6:	9a03      	ldr	r2, [sp, #12]
300097e8:	4293      	cmp	r3, r2
300097ea:	d005      	beq.n	300097f8 <rxi316_perf_tune+0x230>
300097ec:	4ba7      	ldr	r3, [pc, #668]	; (30009a8c <rxi316_perf_tune+0x4c4>)
300097ee:	2002      	movs	r0, #2
300097f0:	4aa8      	ldr	r2, [pc, #672]	; (30009a94 <rxi316_perf_tune+0x4cc>)
300097f2:	4619      	mov	r1, r3
300097f4:	f001 feae 	bl	3000b554 <rtk_log_write>
300097f8:	9b0e      	ldr	r3, [sp, #56]	; 0x38
300097fa:	2b20      	cmp	r3, #32
300097fc:	f000 82f1 	beq.w	30009de2 <rxi316_perf_tune+0x81a>
30009800:	1b5a      	subs	r2, r3, r5
30009802:	3a05      	subs	r2, #5
30009804:	f002 030f 	and.w	r3, r2, #15
30009808:	9303      	str	r3, [sp, #12]
3000980a:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000980c:	2b20      	cmp	r3, #32
3000980e:	f000 82e6 	beq.w	30009dde <rxi316_perf_tune+0x816>
30009812:	1b5a      	subs	r2, r3, r5
30009814:	3a06      	subs	r2, #6
30009816:	0112      	lsls	r2, r2, #4
30009818:	b2d2      	uxtb	r2, r2
3000981a:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000981c:	2b20      	cmp	r3, #32
3000981e:	f000 82da 	beq.w	30009dd6 <rxi316_perf_tune+0x80e>
30009822:	1b5b      	subs	r3, r3, r5
30009824:	3b07      	subs	r3, #7
30009826:	021b      	lsls	r3, r3, #8
30009828:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000982c:	930c      	str	r3, [sp, #48]	; 0x30
3000982e:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009830:	2b20      	cmp	r3, #32
30009832:	f000 820b 	beq.w	30009c4c <rxi316_perf_tune+0x684>
30009836:	1b5b      	subs	r3, r3, r5
30009838:	3b08      	subs	r3, #8
3000983a:	031b      	lsls	r3, r3, #12
3000983c:	b29b      	uxth	r3, r3
3000983e:	930b      	str	r3, [sp, #44]	; 0x2c
30009840:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009842:	2b20      	cmp	r3, #32
30009844:	f000 8209 	beq.w	30009c5a <rxi316_perf_tune+0x692>
30009848:	1b5b      	subs	r3, r3, r5
3000984a:	3b09      	subs	r3, #9
3000984c:	041b      	lsls	r3, r3, #16
3000984e:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
30009852:	930a      	str	r3, [sp, #40]	; 0x28
30009854:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30009856:	2b20      	cmp	r3, #32
30009858:	f000 8206 	beq.w	30009c68 <rxi316_perf_tune+0x6a0>
3000985c:	1b5b      	subs	r3, r3, r5
3000985e:	3b0a      	subs	r3, #10
30009860:	051b      	lsls	r3, r3, #20
30009862:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
30009866:	930d      	str	r3, [sp, #52]	; 0x34
30009868:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000986a:	2b20      	cmp	r3, #32
3000986c:	f000 8203 	beq.w	30009c76 <rxi316_perf_tune+0x6ae>
30009870:	1b5b      	subs	r3, r3, r5
30009872:	3b0b      	subs	r3, #11
30009874:	061b      	lsls	r3, r3, #24
30009876:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000987a:	930e      	str	r3, [sp, #56]	; 0x38
3000987c:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000987e:	2b20      	cmp	r3, #32
30009880:	f000 8200 	beq.w	30009c84 <rxi316_perf_tune+0x6bc>
30009884:	1b5b      	subs	r3, r3, r5
30009886:	3b0c      	subs	r3, #12
30009888:	071b      	lsls	r3, r3, #28
3000988a:	930f      	str	r3, [sp, #60]	; 0x3c
3000988c:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000988e:	2b20      	cmp	r3, #32
30009890:	f000 81ff 	beq.w	30009c92 <rxi316_perf_tune+0x6ca>
30009894:	1b5b      	subs	r3, r3, r5
30009896:	3b0d      	subs	r3, #13
30009898:	f003 030f 	and.w	r3, r3, #15
3000989c:	9310      	str	r3, [sp, #64]	; 0x40
3000989e:	9b13      	ldr	r3, [sp, #76]	; 0x4c
300098a0:	2b20      	cmp	r3, #32
300098a2:	f000 81fc 	beq.w	30009c9e <rxi316_perf_tune+0x6d6>
300098a6:	1b5b      	subs	r3, r3, r5
300098a8:	3b0e      	subs	r3, #14
300098aa:	011b      	lsls	r3, r3, #4
300098ac:	b2db      	uxtb	r3, r3
300098ae:	9311      	str	r3, [sp, #68]	; 0x44
300098b0:	9b14      	ldr	r3, [sp, #80]	; 0x50
300098b2:	2b20      	cmp	r3, #32
300098b4:	f000 81f9 	beq.w	30009caa <rxi316_perf_tune+0x6e2>
300098b8:	1b5b      	subs	r3, r3, r5
300098ba:	3b0f      	subs	r3, #15
300098bc:	021b      	lsls	r3, r3, #8
300098be:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
300098c2:	9312      	str	r3, [sp, #72]	; 0x48
300098c4:	9b15      	ldr	r3, [sp, #84]	; 0x54
300098c6:	2b20      	cmp	r3, #32
300098c8:	f000 81f6 	beq.w	30009cb8 <rxi316_perf_tune+0x6f0>
300098cc:	1b5b      	subs	r3, r3, r5
300098ce:	031b      	lsls	r3, r3, #12
300098d0:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
300098d4:	9321      	str	r3, [sp, #132]	; 0x84
300098d6:	9b16      	ldr	r3, [sp, #88]	; 0x58
300098d8:	2b20      	cmp	r3, #32
300098da:	f000 81f4 	beq.w	30009cc6 <rxi316_perf_tune+0x6fe>
300098de:	1b5b      	subs	r3, r3, r5
300098e0:	3b11      	subs	r3, #17
300098e2:	041b      	lsls	r3, r3, #16
300098e4:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
300098e8:	9320      	str	r3, [sp, #128]	; 0x80
300098ea:	9b17      	ldr	r3, [sp, #92]	; 0x5c
300098ec:	2b20      	cmp	r3, #32
300098ee:	f000 81f1 	beq.w	30009cd4 <rxi316_perf_tune+0x70c>
300098f2:	1b5b      	subs	r3, r3, r5
300098f4:	3b12      	subs	r3, #18
300098f6:	051b      	lsls	r3, r3, #20
300098f8:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
300098fc:	931f      	str	r3, [sp, #124]	; 0x7c
300098fe:	9b18      	ldr	r3, [sp, #96]	; 0x60
30009900:	2b20      	cmp	r3, #32
30009902:	f000 81ee 	beq.w	30009ce2 <rxi316_perf_tune+0x71a>
30009906:	1b5b      	subs	r3, r3, r5
30009908:	3b13      	subs	r3, #19
3000990a:	061b      	lsls	r3, r3, #24
3000990c:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
30009910:	931e      	str	r3, [sp, #120]	; 0x78
30009912:	9b19      	ldr	r3, [sp, #100]	; 0x64
30009914:	2b20      	cmp	r3, #32
30009916:	f000 81eb 	beq.w	30009cf0 <rxi316_perf_tune+0x728>
3000991a:	1b5b      	subs	r3, r3, r5
3000991c:	3b14      	subs	r3, #20
3000991e:	071b      	lsls	r3, r3, #28
30009920:	9318      	str	r3, [sp, #96]	; 0x60
30009922:	9b1a      	ldr	r3, [sp, #104]	; 0x68
30009924:	2b20      	cmp	r3, #32
30009926:	f000 81ea 	beq.w	30009cfe <rxi316_perf_tune+0x736>
3000992a:	1b5b      	subs	r3, r3, r5
3000992c:	3b15      	subs	r3, #21
3000992e:	f003 030f 	and.w	r3, r3, #15
30009932:	9317      	str	r3, [sp, #92]	; 0x5c
30009934:	9b07      	ldr	r3, [sp, #28]
30009936:	2b20      	cmp	r3, #32
30009938:	f000 81e7 	beq.w	30009d0a <rxi316_perf_tune+0x742>
3000993c:	1b5b      	subs	r3, r3, r5
3000993e:	3b02      	subs	r3, #2
30009940:	f003 031f 	and.w	r3, r3, #31
30009944:	9316      	str	r3, [sp, #88]	; 0x58
30009946:	9b08      	ldr	r3, [sp, #32]
30009948:	2b20      	cmp	r3, #32
3000994a:	f000 81e4 	beq.w	30009d16 <rxi316_perf_tune+0x74e>
3000994e:	1b5b      	subs	r3, r3, r5
30009950:	3b02      	subs	r3, #2
30009952:	015b      	lsls	r3, r3, #5
30009954:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
30009958:	9315      	str	r3, [sp, #84]	; 0x54
3000995a:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000995c:	2b20      	cmp	r3, #32
3000995e:	f000 81e1 	beq.w	30009d24 <rxi316_perf_tune+0x75c>
30009962:	1b5b      	subs	r3, r3, r5
30009964:	f1b9 0f20 	cmp.w	r9, #32
30009968:	f1a3 0302 	sub.w	r3, r3, #2
3000996c:	ea4f 2383 	mov.w	r3, r3, lsl #10
30009970:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
30009974:	9314      	str	r3, [sp, #80]	; 0x50
30009976:	f000 81dc 	beq.w	30009d32 <rxi316_perf_tune+0x76a>
3000997a:	eba9 0105 	sub.w	r1, r9, r5
3000997e:	f1b8 0f20 	cmp.w	r8, #32
30009982:	f1a1 0102 	sub.w	r1, r1, #2
30009986:	f001 030f 	and.w	r3, r1, #15
3000998a:	9313      	str	r3, [sp, #76]	; 0x4c
3000998c:	f000 81d7 	beq.w	30009d3e <rxi316_perf_tune+0x776>
30009990:	eba8 0105 	sub.w	r1, r8, r5
30009994:	2f20      	cmp	r7, #32
30009996:	f1a1 0103 	sub.w	r1, r1, #3
3000999a:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000999e:	b2c9      	uxtb	r1, r1
300099a0:	f000 81d2 	beq.w	30009d48 <rxi316_perf_tune+0x780>
300099a4:	1b7b      	subs	r3, r7, r5
300099a6:	f1ba 0f20 	cmp.w	sl, #32
300099aa:	f1a3 0304 	sub.w	r3, r3, #4
300099ae:	ea4f 2303 	mov.w	r3, r3, lsl #8
300099b2:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
300099b6:	9308      	str	r3, [sp, #32]
300099b8:	f000 81cd 	beq.w	30009d56 <rxi316_perf_tune+0x78e>
300099bc:	ebaa 0305 	sub.w	r3, sl, r5
300099c0:	f1bb 0f20 	cmp.w	fp, #32
300099c4:	f1a3 0305 	sub.w	r3, r3, #5
300099c8:	ea4f 3303 	mov.w	r3, r3, lsl #12
300099cc:	b29b      	uxth	r3, r3
300099ce:	9307      	str	r3, [sp, #28]
300099d0:	f000 81c8 	beq.w	30009d64 <rxi316_perf_tune+0x79c>
300099d4:	ebab 0c05 	sub.w	ip, fp, r5
300099d8:	9b01      	ldr	r3, [sp, #4]
300099da:	f1ac 0c06 	sub.w	ip, ip, #6
300099de:	2b20      	cmp	r3, #32
300099e0:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
300099e4:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
300099e8:	f000 81c2 	beq.w	30009d70 <rxi316_perf_tune+0x7a8>
300099ec:	eba3 0e05 	sub.w	lr, r3, r5
300099f0:	9b02      	ldr	r3, [sp, #8]
300099f2:	f1ae 0e07 	sub.w	lr, lr, #7
300099f6:	2b20      	cmp	r3, #32
300099f8:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
300099fc:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
30009a00:	f000 81bc 	beq.w	30009d7c <rxi316_perf_tune+0x7b4>
30009a04:	eba3 0805 	sub.w	r8, r3, r5
30009a08:	9b04      	ldr	r3, [sp, #16]
30009a0a:	f1a8 0808 	sub.w	r8, r8, #8
30009a0e:	2b20      	cmp	r3, #32
30009a10:	ea4f 6808 	mov.w	r8, r8, lsl #24
30009a14:	f008 6870 	and.w	r8, r8, #251658240	; 0xf000000
30009a18:	f000 81b6 	beq.w	30009d88 <rxi316_perf_tune+0x7c0>
30009a1c:	1b5b      	subs	r3, r3, r5
30009a1e:	3b09      	subs	r3, #9
30009a20:	071b      	lsls	r3, r3, #28
30009a22:	9302      	str	r3, [sp, #8]
30009a24:	9b05      	ldr	r3, [sp, #20]
30009a26:	2b20      	cmp	r3, #32
30009a28:	f000 81b5 	beq.w	30009d96 <rxi316_perf_tune+0x7ce>
30009a2c:	eba3 0905 	sub.w	r9, r3, r5
30009a30:	9b06      	ldr	r3, [sp, #24]
30009a32:	f1a9 090a 	sub.w	r9, r9, #10
30009a36:	2b20      	cmp	r3, #32
30009a38:	f009 090f 	and.w	r9, r9, #15
30009a3c:	f000 81b1 	beq.w	30009da2 <rxi316_perf_tune+0x7da>
30009a40:	1b58      	subs	r0, r3, r5
30009a42:	9b1c      	ldr	r3, [sp, #112]	; 0x70
30009a44:	380b      	subs	r0, #11
30009a46:	2b20      	cmp	r3, #32
30009a48:	ea4f 1000 	mov.w	r0, r0, lsl #4
30009a4c:	b2c0      	uxtb	r0, r0
30009a4e:	f000 81ad 	beq.w	30009dac <rxi316_perf_tune+0x7e4>
30009a52:	eba3 0b05 	sub.w	fp, r3, r5
30009a56:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009a58:	f1ab 0b02 	sub.w	fp, fp, #2
30009a5c:	2b20      	cmp	r3, #32
30009a5e:	f00b 0b1f 	and.w	fp, fp, #31
30009a62:	f000 81a9 	beq.w	30009db8 <rxi316_perf_tune+0x7f0>
30009a66:	eba3 0a05 	sub.w	sl, r3, r5
30009a6a:	9b1d      	ldr	r3, [sp, #116]	; 0x74
30009a6c:	f1aa 0a02 	sub.w	sl, sl, #2
30009a70:	2b20      	cmp	r3, #32
30009a72:	f00a 0a1f 	and.w	sl, sl, #31
30009a76:	f000 81a5 	beq.w	30009dc4 <rxi316_perf_tune+0x7fc>
30009a7a:	1b5f      	subs	r7, r3, r5
30009a7c:	2c20      	cmp	r4, #32
30009a7e:	f1a7 0702 	sub.w	r7, r7, #2
30009a82:	f007 071f 	and.w	r7, r7, #31
30009a86:	f000 81a2 	beq.w	30009dce <rxi316_perf_tune+0x806>
30009a8a:	e005      	b.n	30009a98 <rxi316_perf_tune+0x4d0>
30009a8c:	3000d0a4 	.word	0x3000d0a4
30009a90:	3000d0cc 	.word	0x3000d0cc
30009a94:	3000d110 	.word	0x3000d110
30009a98:	1b63      	subs	r3, r4, r5
30009a9a:	3b02      	subs	r3, #2
30009a9c:	015b      	lsls	r3, r3, #5
30009a9e:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
30009aa2:	9301      	str	r3, [sp, #4]
30009aa4:	9c03      	ldr	r4, [sp, #12]
30009aa6:	ea40 0009 	orr.w	r0, r0, r9
30009aaa:	4314      	orrs	r4, r2
30009aac:	4622      	mov	r2, r4
30009aae:	e9dd 4510 	ldrd	r4, r5, [sp, #64]	; 0x40
30009ab2:	432c      	orrs	r4, r5
30009ab4:	9d13      	ldr	r5, [sp, #76]	; 0x4c
30009ab6:	4329      	orrs	r1, r5
30009ab8:	e9dd 5315 	ldrd	r5, r3, [sp, #84]	; 0x54
30009abc:	431d      	orrs	r5, r3
30009abe:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30009ac0:	431a      	orrs	r2, r3
30009ac2:	9b12      	ldr	r3, [sp, #72]	; 0x48
30009ac4:	431c      	orrs	r4, r3
30009ac6:	9b08      	ldr	r3, [sp, #32]
30009ac8:	4319      	orrs	r1, r3
30009aca:	9b14      	ldr	r3, [sp, #80]	; 0x50
30009acc:	431d      	orrs	r5, r3
30009ace:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009ad0:	431a      	orrs	r2, r3
30009ad2:	9b21      	ldr	r3, [sp, #132]	; 0x84
30009ad4:	431c      	orrs	r4, r3
30009ad6:	9b07      	ldr	r3, [sp, #28]
30009ad8:	4319      	orrs	r1, r3
30009ada:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009adc:	431a      	orrs	r2, r3
30009ade:	9b20      	ldr	r3, [sp, #128]	; 0x80
30009ae0:	ea41 010c 	orr.w	r1, r1, ip
30009ae4:	431c      	orrs	r4, r3
30009ae6:	9b01      	ldr	r3, [sp, #4]
30009ae8:	ea41 010e 	orr.w	r1, r1, lr
30009aec:	431f      	orrs	r7, r3
30009aee:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30009af0:	ea41 0108 	orr.w	r1, r1, r8
30009af4:	431a      	orrs	r2, r3
30009af6:	9b1f      	ldr	r3, [sp, #124]	; 0x7c
30009af8:	431c      	orrs	r4, r3
30009afa:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30009afc:	431a      	orrs	r2, r3
30009afe:	9b1e      	ldr	r3, [sp, #120]	; 0x78
30009b00:	4323      	orrs	r3, r4
30009b02:	9c0f      	ldr	r4, [sp, #60]	; 0x3c
30009b04:	4322      	orrs	r2, r4
30009b06:	9c18      	ldr	r4, [sp, #96]	; 0x60
30009b08:	4323      	orrs	r3, r4
30009b0a:	9c02      	ldr	r4, [sp, #8]
30009b0c:	430c      	orrs	r4, r1
30009b0e:	49ba      	ldr	r1, [pc, #744]	; (30009df8 <rxi316_perf_tune+0x830>)
30009b10:	f8c1 2400 	str.w	r2, [r1, #1024]	; 0x400
30009b14:	f8c1 3404 	str.w	r3, [r1, #1028]	; 0x404
30009b18:	9b17      	ldr	r3, [sp, #92]	; 0x5c
30009b1a:	f8c1 3408 	str.w	r3, [r1, #1032]	; 0x408
30009b1e:	f8c1 540c 	str.w	r5, [r1, #1036]	; 0x40c
30009b22:	f8c1 4410 	str.w	r4, [r1, #1040]	; 0x410
30009b26:	f8c1 0414 	str.w	r0, [r1, #1044]	; 0x414
30009b2a:	f8c1 b418 	str.w	fp, [r1, #1048]	; 0x418
30009b2e:	f8c1 a41c 	str.w	sl, [r1, #1052]	; 0x41c
30009b32:	f8c1 7420 	str.w	r7, [r1, #1056]	; 0x420
30009b36:	68f3      	ldr	r3, [r6, #12]
30009b38:	689a      	ldr	r2, [r3, #8]
30009b3a:	e9d2 3200 	ldrd	r3, r2, [r2]
30009b3e:	f003 0301 	and.w	r3, r3, #1
30009b42:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
30009b46:	f8c1 3460 	str.w	r3, [r1, #1120]	; 0x460
30009b4a:	68f3      	ldr	r3, [r6, #12]
30009b4c:	689a      	ldr	r2, [r3, #8]
30009b4e:	7a13      	ldrb	r3, [r2, #8]
30009b50:	6950      	ldr	r0, [r2, #20]
30009b52:	ea43 6300 	orr.w	r3, r3, r0, lsl #24
30009b56:	e9d2 0203 	ldrd	r0, r2, [r2, #12]
30009b5a:	0200      	lsls	r0, r0, #8
30009b5c:	0412      	lsls	r2, r2, #16
30009b5e:	b280      	uxth	r0, r0
30009b60:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
30009b64:	4303      	orrs	r3, r0
30009b66:	4313      	orrs	r3, r2
30009b68:	f8c1 3464 	str.w	r3, [r1, #1124]	; 0x464
30009b6c:	68f3      	ldr	r3, [r6, #12]
30009b6e:	6898      	ldr	r0, [r3, #8]
30009b70:	e9d0 3207 	ldrd	r3, r2, [r0, #28]
30009b74:	7e00      	ldrb	r0, [r0, #24]
30009b76:	021b      	lsls	r3, r3, #8
30009b78:	0412      	lsls	r2, r2, #16
30009b7a:	b29b      	uxth	r3, r3
30009b7c:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
30009b80:	4313      	orrs	r3, r2
30009b82:	4303      	orrs	r3, r0
30009b84:	f8c1 3468 	str.w	r3, [r1, #1128]	; 0x468
30009b88:	68f3      	ldr	r3, [r6, #12]
30009b8a:	689a      	ldr	r2, [r3, #8]
30009b8c:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
30009b90:	041b      	lsls	r3, r3, #16
30009b92:	f002 023f 	and.w	r2, r2, #63	; 0x3f
30009b96:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
30009b9a:	4313      	orrs	r3, r2
30009b9c:	f8c1 3480 	str.w	r3, [r1, #1152]	; 0x480
30009ba0:	6833      	ldr	r3, [r6, #0]
30009ba2:	781b      	ldrb	r3, [r3, #0]
30009ba4:	2b03      	cmp	r3, #3
30009ba6:	f000 811f 	beq.w	30009de8 <rxi316_perf_tune+0x820>
30009baa:	68f3      	ldr	r3, [r6, #12]
30009bac:	6899      	ldr	r1, [r3, #8]
30009bae:	6b0b      	ldr	r3, [r1, #48]	; 0x30
30009bb0:	005b      	lsls	r3, r3, #1
30009bb2:	f003 0302 	and.w	r3, r3, #2
30009bb6:	6b8a      	ldr	r2, [r1, #56]	; 0x38
30009bb8:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
30009bba:	0412      	lsls	r2, r2, #16
30009bbc:	6b49      	ldr	r1, [r1, #52]	; 0x34
30009bbe:	f000 0001 	and.w	r0, r0, #1
30009bc2:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009bc6:	0209      	lsls	r1, r1, #8
30009bc8:	431a      	orrs	r2, r3
30009bca:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
30009bce:	4b8a      	ldr	r3, [pc, #552]	; (30009df8 <rxi316_perf_tune+0x830>)
30009bd0:	4302      	orrs	r2, r0
30009bd2:	488a      	ldr	r0, [pc, #552]	; (30009dfc <rxi316_perf_tune+0x834>)
30009bd4:	430a      	orrs	r2, r1
30009bd6:	4310      	orrs	r0, r2
30009bd8:	f8c3 0484 	str.w	r0, [r3, #1156]	; 0x484
30009bdc:	68f2      	ldr	r2, [r6, #12]
30009bde:	6892      	ldr	r2, [r2, #8]
30009be0:	6bd2      	ldr	r2, [r2, #60]	; 0x3c
30009be2:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30009be6:	68f2      	ldr	r2, [r6, #12]
30009be8:	6892      	ldr	r2, [r2, #8]
30009bea:	6c12      	ldr	r2, [r2, #64]	; 0x40
30009bec:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30009bf0:	68f2      	ldr	r2, [r6, #12]
30009bf2:	6892      	ldr	r2, [r2, #8]
30009bf4:	6c52      	ldr	r2, [r2, #68]	; 0x44
30009bf6:	f8c3 2490 	str.w	r2, [r3, #1168]	; 0x490
30009bfa:	68f2      	ldr	r2, [r6, #12]
30009bfc:	6892      	ldr	r2, [r2, #8]
30009bfe:	6c92      	ldr	r2, [r2, #72]	; 0x48
30009c00:	f8c3 2494 	str.w	r2, [r3, #1172]	; 0x494
30009c04:	68f2      	ldr	r2, [r6, #12]
30009c06:	6892      	ldr	r2, [r2, #8]
30009c08:	6cd2      	ldr	r2, [r2, #76]	; 0x4c
30009c0a:	f8c3 2498 	str.w	r2, [r3, #1176]	; 0x498
30009c0e:	68f2      	ldr	r2, [r6, #12]
30009c10:	6892      	ldr	r2, [r2, #8]
30009c12:	6d12      	ldr	r2, [r2, #80]	; 0x50
30009c14:	f002 020f 	and.w	r2, r2, #15
30009c18:	f8c3 24a0 	str.w	r2, [r3, #1184]	; 0x4a0
30009c1c:	f8d3 2470 	ldr.w	r2, [r3, #1136]	; 0x470
30009c20:	f022 023f 	bic.w	r2, r2, #63	; 0x3f
30009c24:	f042 0204 	orr.w	r2, r2, #4
30009c28:	f8c3 2470 	str.w	r2, [r3, #1136]	; 0x470
30009c2c:	f8d3 2474 	ldr.w	r2, [r3, #1140]	; 0x474
30009c30:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
30009c34:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
30009c38:	f8c3 2474 	str.w	r2, [r3, #1140]	; 0x474
30009c3c:	b043      	add	sp, #268	; 0x10c
30009c3e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009c42:	2502      	movs	r5, #2
30009c44:	e4cf      	b.n	300095e6 <rxi316_perf_tune+0x1e>
30009c46:	3101      	adds	r1, #1
30009c48:	9304      	str	r3, [sp, #16]
30009c4a:	e536      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009c4c:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009c50:	930b      	str	r3, [sp, #44]	; 0x2c
30009c52:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009c54:	2b20      	cmp	r3, #32
30009c56:	f47f adf7 	bne.w	30009848 <rxi316_perf_tune+0x280>
30009c5a:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
30009c5e:	930a      	str	r3, [sp, #40]	; 0x28
30009c60:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30009c62:	2b20      	cmp	r3, #32
30009c64:	f47f adfa 	bne.w	3000985c <rxi316_perf_tune+0x294>
30009c68:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
30009c6c:	930d      	str	r3, [sp, #52]	; 0x34
30009c6e:	9b10      	ldr	r3, [sp, #64]	; 0x40
30009c70:	2b20      	cmp	r3, #32
30009c72:	f47f adfd 	bne.w	30009870 <rxi316_perf_tune+0x2a8>
30009c76:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
30009c7a:	930e      	str	r3, [sp, #56]	; 0x38
30009c7c:	9b11      	ldr	r3, [sp, #68]	; 0x44
30009c7e:	2b20      	cmp	r3, #32
30009c80:	f47f ae00 	bne.w	30009884 <rxi316_perf_tune+0x2bc>
30009c84:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009c88:	930f      	str	r3, [sp, #60]	; 0x3c
30009c8a:	9b12      	ldr	r3, [sp, #72]	; 0x48
30009c8c:	2b20      	cmp	r3, #32
30009c8e:	f47f ae01 	bne.w	30009894 <rxi316_perf_tune+0x2cc>
30009c92:	230f      	movs	r3, #15
30009c94:	9310      	str	r3, [sp, #64]	; 0x40
30009c96:	9b13      	ldr	r3, [sp, #76]	; 0x4c
30009c98:	2b20      	cmp	r3, #32
30009c9a:	f47f ae04 	bne.w	300098a6 <rxi316_perf_tune+0x2de>
30009c9e:	23f0      	movs	r3, #240	; 0xf0
30009ca0:	9311      	str	r3, [sp, #68]	; 0x44
30009ca2:	9b14      	ldr	r3, [sp, #80]	; 0x50
30009ca4:	2b20      	cmp	r3, #32
30009ca6:	f47f ae07 	bne.w	300098b8 <rxi316_perf_tune+0x2f0>
30009caa:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009cae:	9312      	str	r3, [sp, #72]	; 0x48
30009cb0:	9b15      	ldr	r3, [sp, #84]	; 0x54
30009cb2:	2b20      	cmp	r3, #32
30009cb4:	f47f ae0a 	bne.w	300098cc <rxi316_perf_tune+0x304>
30009cb8:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009cbc:	9321      	str	r3, [sp, #132]	; 0x84
30009cbe:	9b16      	ldr	r3, [sp, #88]	; 0x58
30009cc0:	2b20      	cmp	r3, #32
30009cc2:	f47f ae0c 	bne.w	300098de <rxi316_perf_tune+0x316>
30009cc6:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
30009cca:	9320      	str	r3, [sp, #128]	; 0x80
30009ccc:	9b17      	ldr	r3, [sp, #92]	; 0x5c
30009cce:	2b20      	cmp	r3, #32
30009cd0:	f47f ae0f 	bne.w	300098f2 <rxi316_perf_tune+0x32a>
30009cd4:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
30009cd8:	931f      	str	r3, [sp, #124]	; 0x7c
30009cda:	9b18      	ldr	r3, [sp, #96]	; 0x60
30009cdc:	2b20      	cmp	r3, #32
30009cde:	f47f ae12 	bne.w	30009906 <rxi316_perf_tune+0x33e>
30009ce2:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
30009ce6:	931e      	str	r3, [sp, #120]	; 0x78
30009ce8:	9b19      	ldr	r3, [sp, #100]	; 0x64
30009cea:	2b20      	cmp	r3, #32
30009cec:	f47f ae15 	bne.w	3000991a <rxi316_perf_tune+0x352>
30009cf0:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009cf4:	9318      	str	r3, [sp, #96]	; 0x60
30009cf6:	9b1a      	ldr	r3, [sp, #104]	; 0x68
30009cf8:	2b20      	cmp	r3, #32
30009cfa:	f47f ae16 	bne.w	3000992a <rxi316_perf_tune+0x362>
30009cfe:	230f      	movs	r3, #15
30009d00:	9317      	str	r3, [sp, #92]	; 0x5c
30009d02:	9b07      	ldr	r3, [sp, #28]
30009d04:	2b20      	cmp	r3, #32
30009d06:	f47f ae19 	bne.w	3000993c <rxi316_perf_tune+0x374>
30009d0a:	231f      	movs	r3, #31
30009d0c:	9316      	str	r3, [sp, #88]	; 0x58
30009d0e:	9b08      	ldr	r3, [sp, #32]
30009d10:	2b20      	cmp	r3, #32
30009d12:	f47f ae1c 	bne.w	3000994e <rxi316_perf_tune+0x386>
30009d16:	f44f 7378 	mov.w	r3, #992	; 0x3e0
30009d1a:	9315      	str	r3, [sp, #84]	; 0x54
30009d1c:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
30009d1e:	2b20      	cmp	r3, #32
30009d20:	f47f ae1f 	bne.w	30009962 <rxi316_perf_tune+0x39a>
30009d24:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
30009d28:	f1b9 0f20 	cmp.w	r9, #32
30009d2c:	9314      	str	r3, [sp, #80]	; 0x50
30009d2e:	f47f ae24 	bne.w	3000997a <rxi316_perf_tune+0x3b2>
30009d32:	230f      	movs	r3, #15
30009d34:	f1b8 0f20 	cmp.w	r8, #32
30009d38:	9313      	str	r3, [sp, #76]	; 0x4c
30009d3a:	f47f ae29 	bne.w	30009990 <rxi316_perf_tune+0x3c8>
30009d3e:	2f20      	cmp	r7, #32
30009d40:	f04f 01f0 	mov.w	r1, #240	; 0xf0
30009d44:	f47f ae2e 	bne.w	300099a4 <rxi316_perf_tune+0x3dc>
30009d48:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009d4c:	f1ba 0f20 	cmp.w	sl, #32
30009d50:	9308      	str	r3, [sp, #32]
30009d52:	f47f ae33 	bne.w	300099bc <rxi316_perf_tune+0x3f4>
30009d56:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009d5a:	f1bb 0f20 	cmp.w	fp, #32
30009d5e:	9307      	str	r3, [sp, #28]
30009d60:	f47f ae38 	bne.w	300099d4 <rxi316_perf_tune+0x40c>
30009d64:	9b01      	ldr	r3, [sp, #4]
30009d66:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
30009d6a:	2b20      	cmp	r3, #32
30009d6c:	f47f ae3e 	bne.w	300099ec <rxi316_perf_tune+0x424>
30009d70:	9b02      	ldr	r3, [sp, #8]
30009d72:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
30009d76:	2b20      	cmp	r3, #32
30009d78:	f47f ae44 	bne.w	30009a04 <rxi316_perf_tune+0x43c>
30009d7c:	9b04      	ldr	r3, [sp, #16]
30009d7e:	f04f 6870 	mov.w	r8, #251658240	; 0xf000000
30009d82:	2b20      	cmp	r3, #32
30009d84:	f47f ae4a 	bne.w	30009a1c <rxi316_perf_tune+0x454>
30009d88:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009d8c:	9302      	str	r3, [sp, #8]
30009d8e:	9b05      	ldr	r3, [sp, #20]
30009d90:	2b20      	cmp	r3, #32
30009d92:	f47f ae4b 	bne.w	30009a2c <rxi316_perf_tune+0x464>
30009d96:	9b06      	ldr	r3, [sp, #24]
30009d98:	f04f 090f 	mov.w	r9, #15
30009d9c:	2b20      	cmp	r3, #32
30009d9e:	f47f ae4f 	bne.w	30009a40 <rxi316_perf_tune+0x478>
30009da2:	9b1c      	ldr	r3, [sp, #112]	; 0x70
30009da4:	20f0      	movs	r0, #240	; 0xf0
30009da6:	2b20      	cmp	r3, #32
30009da8:	f47f ae53 	bne.w	30009a52 <rxi316_perf_tune+0x48a>
30009dac:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009dae:	f04f 0b1f 	mov.w	fp, #31
30009db2:	2b20      	cmp	r3, #32
30009db4:	f47f ae57 	bne.w	30009a66 <rxi316_perf_tune+0x49e>
30009db8:	9b1d      	ldr	r3, [sp, #116]	; 0x74
30009dba:	f04f 0a1f 	mov.w	sl, #31
30009dbe:	2b20      	cmp	r3, #32
30009dc0:	f47f ae5b 	bne.w	30009a7a <rxi316_perf_tune+0x4b2>
30009dc4:	2c20      	cmp	r4, #32
30009dc6:	f04f 071f 	mov.w	r7, #31
30009dca:	f47f ae65 	bne.w	30009a98 <rxi316_perf_tune+0x4d0>
30009dce:	f44f 7378 	mov.w	r3, #992	; 0x3e0
30009dd2:	9301      	str	r3, [sp, #4]
30009dd4:	e666      	b.n	30009aa4 <rxi316_perf_tune+0x4dc>
30009dd6:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009dda:	930c      	str	r3, [sp, #48]	; 0x30
30009ddc:	e527      	b.n	3000982e <rxi316_perf_tune+0x266>
30009dde:	22f0      	movs	r2, #240	; 0xf0
30009de0:	e51b      	b.n	3000981a <rxi316_perf_tune+0x252>
30009de2:	230f      	movs	r3, #15
30009de4:	9303      	str	r3, [sp, #12]
30009de6:	e510      	b.n	3000980a <rxi316_perf_tune+0x242>
30009de8:	68f2      	ldr	r2, [r6, #12]
30009dea:	2302      	movs	r3, #2
30009dec:	6891      	ldr	r1, [r2, #8]
30009dee:	e6e2      	b.n	30009bb6 <rxi316_perf_tune+0x5ee>
30009df0:	3101      	adds	r1, #1
30009df2:	9305      	str	r3, [sp, #20]
30009df4:	e461      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009df6:	bf00      	nop
30009df8:	41100000 	.word	0x41100000
30009dfc:	40000008 	.word	0x40000008
30009e00:	3101      	adds	r1, #1
30009e02:	9306      	str	r3, [sp, #24]
30009e04:	e459      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e06:	2202      	movs	r2, #2
30009e08:	921e      	str	r2, [sp, #120]	; 0x78
30009e0a:	e4e2      	b.n	300097d2 <rxi316_perf_tune+0x20a>
30009e0c:	9a03      	ldr	r2, [sp, #12]
30009e0e:	9308      	str	r3, [sp, #32]
30009e10:	3201      	adds	r2, #1
30009e12:	9203      	str	r2, [sp, #12]
30009e14:	e451      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e16:	9a03      	ldr	r2, [sp, #12]
30009e18:	9307      	str	r3, [sp, #28]
30009e1a:	3201      	adds	r2, #1
30009e1c:	9203      	str	r2, [sp, #12]
30009e1e:	e44c      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e20:	9a03      	ldr	r2, [sp, #12]
30009e22:	931b      	str	r3, [sp, #108]	; 0x6c
30009e24:	3201      	adds	r2, #1
30009e26:	9203      	str	r2, [sp, #12]
30009e28:	e447      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e2a:	9a03      	ldr	r2, [sp, #12]
30009e2c:	461c      	mov	r4, r3
30009e2e:	3201      	adds	r2, #1
30009e30:	9203      	str	r2, [sp, #12]
30009e32:	e442      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e34:	9a03      	ldr	r2, [sp, #12]
30009e36:	931d      	str	r3, [sp, #116]	; 0x74
30009e38:	3201      	adds	r2, #1
30009e3a:	9203      	str	r2, [sp, #12]
30009e3c:	e43d      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e3e:	9a03      	ldr	r2, [sp, #12]
30009e40:	931c      	str	r3, [sp, #112]	; 0x70
30009e42:	3201      	adds	r2, #1
30009e44:	9203      	str	r2, [sp, #12]
30009e46:	e438      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e48:	930e      	str	r3, [sp, #56]	; 0x38
30009e4a:	e436      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e4c:	930d      	str	r3, [sp, #52]	; 0x34
30009e4e:	e434      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e50:	930c      	str	r3, [sp, #48]	; 0x30
30009e52:	e432      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e54:	930a      	str	r3, [sp, #40]	; 0x28
30009e56:	e430      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e58:	930b      	str	r3, [sp, #44]	; 0x2c
30009e5a:	e42e      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e5c:	931a      	str	r3, [sp, #104]	; 0x68
30009e5e:	e42c      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e60:	9319      	str	r3, [sp, #100]	; 0x64
30009e62:	e42a      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e64:	9310      	str	r3, [sp, #64]	; 0x40
30009e66:	e428      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e68:	930f      	str	r3, [sp, #60]	; 0x3c
30009e6a:	e426      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e6c:	9318      	str	r3, [sp, #96]	; 0x60
30009e6e:	e424      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e70:	9317      	str	r3, [sp, #92]	; 0x5c
30009e72:	e422      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e74:	9316      	str	r3, [sp, #88]	; 0x58
30009e76:	e420      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e78:	9315      	str	r3, [sp, #84]	; 0x54
30009e7a:	e41e      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e7c:	9314      	str	r3, [sp, #80]	; 0x50
30009e7e:	e41c      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e80:	9313      	str	r3, [sp, #76]	; 0x4c
30009e82:	e41a      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e84:	9312      	str	r3, [sp, #72]	; 0x48
30009e86:	e418      	b.n	300096ba <rxi316_perf_tune+0xf2>
30009e88:	9311      	str	r3, [sp, #68]	; 0x44
30009e8a:	e416      	b.n	300096ba <rxi316_perf_tune+0xf2>

30009e8c <rxi316_dram_init>:
30009e8c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009e90:	4604      	mov	r4, r0
30009e92:	b08f      	sub	sp, #60	; 0x3c
30009e94:	4b10      	ldr	r3, [pc, #64]	; (30009ed8 <rxi316_dram_init+0x4c>)
30009e96:	4798      	blx	r3
30009e98:	f894 b014 	ldrb.w	fp, [r4, #20]
30009e9c:	2101      	movs	r1, #1
30009e9e:	900c      	str	r0, [sp, #48]	; 0x30
30009ea0:	fa01 f30b 	lsl.w	r3, r1, fp
30009ea4:	9302      	str	r3, [sp, #8]
30009ea6:	6823      	ldr	r3, [r4, #0]
30009ea8:	78da      	ldrb	r2, [r3, #3]
30009eaa:	6923      	ldr	r3, [r4, #16]
30009eac:	fa03 f30b 	lsl.w	r3, r3, fp
30009eb0:	b1a2      	cbz	r2, 30009edc <rxi316_dram_init+0x50>
30009eb2:	428a      	cmp	r2, r1
30009eb4:	f000 8469 	beq.w	3000a78a <rxi316_dram_init+0x8fe>
30009eb8:	2a02      	cmp	r2, #2
30009eba:	bf0b      	itete	eq
30009ebc:	2206      	moveq	r2, #6
30009ebe:	220a      	movne	r2, #10
30009ec0:	2540      	moveq	r5, #64	; 0x40
30009ec2:	2580      	movne	r5, #128	; 0x80
30009ec4:	9207      	str	r2, [sp, #28]
30009ec6:	bf0c      	ite	eq
30009ec8:	2204      	moveq	r2, #4
30009eca:	2208      	movne	r2, #8
30009ecc:	9208      	str	r2, [sp, #32]
30009ece:	bf0c      	ite	eq
30009ed0:	2208      	moveq	r2, #8
30009ed2:	2210      	movne	r2, #16
30009ed4:	9201      	str	r2, [sp, #4]
30009ed6:	e007      	b.n	30009ee8 <rxi316_dram_init+0x5c>
30009ed8:	30007631 	.word	0x30007631
30009edc:	2203      	movs	r2, #3
30009ede:	25c0      	movs	r5, #192	; 0xc0
30009ee0:	9108      	str	r1, [sp, #32]
30009ee2:	9207      	str	r2, [sp, #28]
30009ee4:	2202      	movs	r2, #2
30009ee6:	9201      	str	r2, [sp, #4]
30009ee8:	6860      	ldr	r0, [r4, #4]
30009eea:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30009eee:	4abf      	ldr	r2, [pc, #764]	; (3000a1ec <rxi316_dram_init+0x360>)
30009ef0:	6806      	ldr	r6, [r0, #0]
30009ef2:	960d      	str	r6, [sp, #52]	; 0x34
30009ef4:	6846      	ldr	r6, [r0, #4]
30009ef6:	6880      	ldr	r0, [r0, #8]
30009ef8:	9609      	str	r6, [sp, #36]	; 0x24
30009efa:	9003      	str	r0, [sp, #12]
30009efc:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
30009f00:	6820      	ldr	r0, [r4, #0]
30009f02:	7981      	ldrb	r1, [r0, #6]
30009f04:	6882      	ldr	r2, [r0, #8]
30009f06:	b129      	cbz	r1, 30009f14 <rxi316_dram_init+0x88>
30009f08:	2901      	cmp	r1, #1
30009f0a:	f000 8436 	beq.w	3000a77a <rxi316_dram_init+0x8ee>
30009f0e:	2903      	cmp	r1, #3
30009f10:	f000 8443 	beq.w	3000a79a <rxi316_dram_init+0x90e>
30009f14:	3a06      	subs	r2, #6
30009f16:	7881      	ldrb	r1, [r0, #2]
30009f18:	f002 020f 	and.w	r2, r2, #15
30009f1c:	7940      	ldrb	r0, [r0, #5]
30009f1e:	0109      	lsls	r1, r1, #4
30009f20:	0280      	lsls	r0, r0, #10
30009f22:	f001 0130 	and.w	r1, r1, #48	; 0x30
30009f26:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
30009f2a:	4301      	orrs	r1, r0
30009f2c:	430d      	orrs	r5, r1
30009f2e:	432a      	orrs	r2, r5
30009f30:	4dae      	ldr	r5, [pc, #696]	; (3000a1ec <rxi316_dram_init+0x360>)
30009f32:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
30009f36:	68e2      	ldr	r2, [r4, #12]
30009f38:	6812      	ldr	r2, [r2, #0]
30009f3a:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
30009f3e:	f002 0207 	and.w	r2, r2, #7
30009f42:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
30009f46:	68e2      	ldr	r2, [r4, #12]
30009f48:	6811      	ldr	r1, [r2, #0]
30009f4a:	6822      	ldr	r2, [r4, #0]
30009f4c:	6848      	ldr	r0, [r1, #4]
30009f4e:	f892 9000 	ldrb.w	r9, [r2]
30009f52:	688a      	ldr	r2, [r1, #8]
30009f54:	9004      	str	r0, [sp, #16]
30009f56:	0300      	lsls	r0, r0, #12
30009f58:	0412      	lsls	r2, r2, #16
30009f5a:	f009 070f 	and.w	r7, r9, #15
30009f5e:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
30009f62:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
30009f66:	4302      	orrs	r2, r0
30009f68:	68c8      	ldr	r0, [r1, #12]
30009f6a:	0440      	lsls	r0, r0, #17
30009f6c:	433a      	orrs	r2, r7
30009f6e:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
30009f72:	6908      	ldr	r0, [r1, #16]
30009f74:	0480      	lsls	r0, r0, #18
30009f76:	4332      	orrs	r2, r6
30009f78:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
30009f7c:	4310      	orrs	r0, r2
30009f7e:	694a      	ldr	r2, [r1, #20]
30009f80:	04d2      	lsls	r2, r2, #19
30009f82:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
30009f86:	4310      	orrs	r0, r2
30009f88:	698a      	ldr	r2, [r1, #24]
30009f8a:	0552      	lsls	r2, r2, #21
30009f8c:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
30009f90:	4310      	orrs	r0, r2
30009f92:	69ca      	ldr	r2, [r1, #28]
30009f94:	0592      	lsls	r2, r2, #22
30009f96:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
30009f9a:	4310      	orrs	r0, r2
30009f9c:	6a8a      	ldr	r2, [r1, #40]	; 0x28
30009f9e:	0612      	lsls	r2, r2, #24
30009fa0:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
30009fa4:	4302      	orrs	r2, r0
30009fa6:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
30009faa:	06c0      	lsls	r0, r0, #27
30009fac:	0789      	lsls	r1, r1, #30
30009fae:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
30009fb2:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
30009fb6:	4310      	orrs	r0, r2
30009fb8:	9a02      	ldr	r2, [sp, #8]
30009fba:	0212      	lsls	r2, r2, #8
30009fbc:	4308      	orrs	r0, r1
30009fbe:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
30009fc2:	4310      	orrs	r0, r2
30009fc4:	6068      	str	r0, [r5, #4]
30009fc6:	6822      	ldr	r2, [r4, #0]
30009fc8:	7992      	ldrb	r2, [r2, #6]
30009fca:	2a00      	cmp	r2, #0
30009fcc:	f040 8321 	bne.w	3000a612 <rxi316_dram_init+0x786>
30009fd0:	686a      	ldr	r2, [r5, #4]
30009fd2:	f042 0210 	orr.w	r2, r2, #16
30009fd6:	606a      	str	r2, [r5, #4]
30009fd8:	68e2      	ldr	r2, [r4, #12]
30009fda:	9802      	ldr	r0, [sp, #8]
30009fdc:	6812      	ldr	r2, [r2, #0]
30009fde:	f100 3aff 	add.w	sl, r0, #4294967295	; 0xffffffff
30009fe2:	6861      	ldr	r1, [r4, #4]
30009fe4:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
30009fe6:	f8d1 e010 	ldr.w	lr, [r1, #16]
30009fea:	9005      	str	r0, [sp, #20]
30009fec:	6b10      	ldr	r0, [r2, #48]	; 0x30
30009fee:	fa2e f80b 	lsr.w	r8, lr, fp
30009ff2:	6949      	ldr	r1, [r1, #20]
30009ff4:	ea1e 0f0a 	tst.w	lr, sl
30009ff8:	9006      	str	r0, [sp, #24]
30009ffa:	6c55      	ldr	r5, [r2, #68]	; 0x44
30009ffc:	fa21 fe0b 	lsr.w	lr, r1, fp
3000a000:	bf18      	it	ne
3000a002:	f108 0801 	addne.w	r8, r8, #1
3000a006:	ea11 0f0a 	tst.w	r1, sl
3000a00a:	9906      	ldr	r1, [sp, #24]
3000a00c:	950a      	str	r5, [sp, #40]	; 0x28
3000a00e:	bf18      	it	ne
3000a010:	f10e 0e01 	addne.w	lr, lr, #1
3000a014:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000a016:	0049      	lsls	r1, r1, #1
3000a018:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000a01c:	950b      	str	r5, [sp, #44]	; 0x2c
3000a01e:	f001 0102 	and.w	r1, r1, #2
3000a022:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a026:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000a02a:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000a02e:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000a032:	0080      	lsls	r0, r0, #2
3000a034:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000a038:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000a03c:	9006      	str	r0, [sp, #24]
3000a03e:	013f      	lsls	r7, r7, #4
3000a040:	9805      	ldr	r0, [sp, #20]
3000a042:	f00c 0c08 	and.w	ip, ip, #8
3000a046:	0176      	lsls	r6, r6, #5
3000a048:	f007 0710 	and.w	r7, r7, #16
3000a04c:	f000 0001 	and.w	r0, r0, #1
3000a050:	f006 0620 	and.w	r6, r6, #32
3000a054:	4605      	mov	r5, r0
3000a056:	9806      	ldr	r0, [sp, #24]
3000a058:	f000 0004 	and.w	r0, r0, #4
3000a05c:	4301      	orrs	r1, r0
3000a05e:	4329      	orrs	r1, r5
3000a060:	ea41 0c0c 	orr.w	ip, r1, ip
3000a064:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a066:	ea4c 0707 	orr.w	r7, ip, r7
3000a06a:	018d      	lsls	r5, r1, #6
3000a06c:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a06e:	4337      	orrs	r7, r6
3000a070:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000a074:	0208      	lsls	r0, r1, #8
3000a076:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000a078:	432f      	orrs	r7, r5
3000a07a:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000a07e:	06c9      	lsls	r1, r1, #27
3000a080:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000a084:	4307      	orrs	r7, r0
3000a086:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000a08a:	0736      	lsls	r6, r6, #28
3000a08c:	430f      	orrs	r7, r1
3000a08e:	0752      	lsls	r2, r2, #29
3000a090:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000a094:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000a098:	433e      	orrs	r6, r7
3000a09a:	4316      	orrs	r6, r2
3000a09c:	4a53      	ldr	r2, [pc, #332]	; (3000a1ec <rxi316_dram_init+0x360>)
3000a09e:	ea46 0608 	orr.w	r6, r6, r8
3000a0a2:	ea46 060e 	orr.w	r6, r6, lr
3000a0a6:	6096      	str	r6, [r2, #8]
3000a0a8:	6862      	ldr	r2, [r4, #4]
3000a0aa:	6990      	ldr	r0, [r2, #24]
3000a0ac:	9a01      	ldr	r2, [sp, #4]
3000a0ae:	2a10      	cmp	r2, #16
3000a0b0:	6822      	ldr	r2, [r4, #0]
3000a0b2:	7811      	ldrb	r1, [r2, #0]
3000a0b4:	7916      	ldrb	r6, [r2, #4]
3000a0b6:	f000 8321 	beq.w	3000a6fc <rxi316_dram_init+0x870>
3000a0ba:	f1a1 0502 	sub.w	r5, r1, #2
3000a0be:	f1a1 0e09 	sub.w	lr, r1, #9
3000a0c2:	9f01      	ldr	r7, [sp, #4]
3000a0c4:	fab5 f585 	clz	r5, r5
3000a0c8:	fabe fe8e 	clz	lr, lr
3000a0cc:	2f08      	cmp	r7, #8
3000a0ce:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000a0d2:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000a0d6:	ea45 020e 	orr.w	r2, r5, lr
3000a0da:	f000 82a8 	beq.w	3000a62e <rxi316_dram_init+0x7a2>
3000a0de:	9f01      	ldr	r7, [sp, #4]
3000a0e0:	2f04      	cmp	r7, #4
3000a0e2:	f000 837c 	beq.w	3000a7de <rxi316_dram_init+0x952>
3000a0e6:	9f01      	ldr	r7, [sp, #4]
3000a0e8:	2f02      	cmp	r7, #2
3000a0ea:	bf08      	it	eq
3000a0ec:	4310      	orreq	r0, r2
3000a0ee:	2a00      	cmp	r2, #0
3000a0f0:	f040 82a2 	bne.w	3000a638 <rxi316_dram_init+0x7ac>
3000a0f4:	2903      	cmp	r1, #3
3000a0f6:	f000 8355 	beq.w	3000a7a4 <rxi316_dram_init+0x918>
3000a0fa:	2500      	movs	r5, #0
3000a0fc:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a100:	462f      	mov	r7, r5
3000a102:	433d      	orrs	r5, r7
3000a104:	4f39      	ldr	r7, [pc, #228]	; (3000a1ec <rxi316_dram_init+0x360>)
3000a106:	430d      	orrs	r5, r1
3000a108:	4305      	orrs	r5, r0
3000a10a:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000a10e:	637e      	str	r6, [r7, #52]	; 0x34
3000a110:	6862      	ldr	r2, [r4, #4]
3000a112:	9903      	ldr	r1, [sp, #12]
3000a114:	69d2      	ldr	r2, [r2, #28]
3000a116:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000a11a:	63ba      	str	r2, [r7, #56]	; 0x38
3000a11c:	6822      	ldr	r2, [r4, #0]
3000a11e:	7810      	ldrb	r0, [r2, #0]
3000a120:	6862      	ldr	r2, [r4, #4]
3000a122:	2803      	cmp	r0, #3
3000a124:	6a11      	ldr	r1, [r2, #32]
3000a126:	f000 82dc 	beq.w	3000a6e2 <rxi316_dram_init+0x856>
3000a12a:	2809      	cmp	r0, #9
3000a12c:	f000 82fa 	beq.w	3000a724 <rxi316_dram_init+0x898>
3000a130:	482e      	ldr	r0, [pc, #184]	; (3000a1ec <rxi316_dram_init+0x360>)
3000a132:	63c1      	str	r1, [r0, #60]	; 0x3c
3000a134:	6862      	ldr	r2, [r4, #4]
3000a136:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000a138:	6402      	str	r2, [r0, #64]	; 0x40
3000a13a:	6860      	ldr	r0, [r4, #4]
3000a13c:	68c5      	ldr	r5, [r0, #12]
3000a13e:	6802      	ldr	r2, [r0, #0]
3000a140:	6841      	ldr	r1, [r0, #4]
3000a142:	442a      	add	r2, r5
3000a144:	6880      	ldr	r0, [r0, #8]
3000a146:	4429      	add	r1, r5
3000a148:	ea1a 0f02 	tst.w	sl, r2
3000a14c:	fa22 f20b 	lsr.w	r2, r2, fp
3000a150:	fa21 f60b 	lsr.w	r6, r1, fp
3000a154:	fa20 f50b 	lsr.w	r5, r0, fp
3000a158:	bf18      	it	ne
3000a15a:	3201      	addne	r2, #1
3000a15c:	ea1a 0f01 	tst.w	sl, r1
3000a160:	6821      	ldr	r1, [r4, #0]
3000a162:	bf18      	it	ne
3000a164:	3601      	addne	r6, #1
3000a166:	ea1a 0f00 	tst.w	sl, r0
3000a16a:	7808      	ldrb	r0, [r1, #0]
3000a16c:	bf18      	it	ne
3000a16e:	3501      	addne	r5, #1
3000a170:	68a1      	ldr	r1, [r4, #8]
3000a172:	3802      	subs	r0, #2
3000a174:	6dcf      	ldr	r7, [r1, #92]	; 0x5c
3000a176:	2801      	cmp	r0, #1
3000a178:	f240 82ac 	bls.w	3000a6d4 <rxi316_dram_init+0x848>
3000a17c:	698f      	ldr	r7, [r1, #24]
3000a17e:	fbb7 f0f3 	udiv	r0, r7, r3
3000a182:	fb03 7710 	mls	r7, r3, r0, r7
3000a186:	b107      	cbz	r7, 3000a18a <rxi316_dram_init+0x2fe>
3000a188:	3001      	adds	r0, #1
3000a18a:	6a4f      	ldr	r7, [r1, #36]	; 0x24
3000a18c:	fbb7 f1f3 	udiv	r1, r7, r3
3000a190:	fb03 7711 	mls	r7, r3, r1, r7
3000a194:	b107      	cbz	r7, 3000a198 <rxi316_dram_init+0x30c>
3000a196:	3101      	adds	r1, #1
3000a198:	0176      	lsls	r6, r6, #5
3000a19a:	f002 021f 	and.w	r2, r2, #31
3000a19e:	02ad      	lsls	r5, r5, #10
3000a1a0:	f406 7678 	and.w	r6, r6, #992	; 0x3e0
3000a1a4:	0500      	lsls	r0, r0, #20
3000a1a6:	f405 45f8 	and.w	r5, r5, #31744	; 0x7c00
3000a1aa:	4316      	orrs	r6, r2
3000a1ac:	f000 70f8 	and.w	r0, r0, #32505856	; 0x1f00000
3000a1b0:	4a0e      	ldr	r2, [pc, #56]	; (3000a1ec <rxi316_dram_init+0x360>)
3000a1b2:	432e      	orrs	r6, r5
3000a1b4:	4306      	orrs	r6, r0
3000a1b6:	0648      	lsls	r0, r1, #25
3000a1b8:	f000 5078 	and.w	r0, r0, #1040187392	; 0x3e000000
3000a1bc:	4330      	orrs	r0, r6
3000a1be:	6310      	str	r0, [r2, #48]	; 0x30
3000a1c0:	68a0      	ldr	r0, [r4, #8]
3000a1c2:	6802      	ldr	r2, [r0, #0]
3000a1c4:	fbb2 f1f3 	udiv	r1, r2, r3
3000a1c8:	fb03 2211 	mls	r2, r3, r1, r2
3000a1cc:	2a00      	cmp	r2, #0
3000a1ce:	f000 827f 	beq.w	3000a6d0 <rxi316_dram_init+0x844>
3000a1d2:	1c4a      	adds	r2, r1, #1
3000a1d4:	9205      	str	r2, [sp, #20]
3000a1d6:	6842      	ldr	r2, [r0, #4]
3000a1d8:	fbb2 f0f3 	udiv	r0, r2, r3
3000a1dc:	fb03 2210 	mls	r2, r3, r0, r2
3000a1e0:	2a00      	cmp	r2, #0
3000a1e2:	f000 8273 	beq.w	3000a6cc <rxi316_dram_init+0x840>
3000a1e6:	38c7      	subs	r0, #199	; 0xc7
3000a1e8:	e002      	b.n	3000a1f0 <rxi316_dram_init+0x364>
3000a1ea:	bf00      	nop
3000a1ec:	41100000 	.word	0x41100000
3000a1f0:	68e2      	ldr	r2, [r4, #12]
3000a1f2:	6811      	ldr	r1, [r2, #0]
3000a1f4:	4ac3      	ldr	r2, [pc, #780]	; (3000a504 <rxi316_dram_init+0x678>)
3000a1f6:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000a1fa:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000a1fe:	900a      	str	r0, [sp, #40]	; 0x28
3000a200:	0712      	lsls	r2, r2, #28
3000a202:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000a206:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000a20a:	430a      	orrs	r2, r1
3000a20c:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000a210:	430a      	orrs	r2, r1
3000a212:	49bd      	ldr	r1, [pc, #756]	; (3000a508 <rxi316_dram_init+0x67c>)
3000a214:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000a218:	610a      	str	r2, [r1, #16]
3000a21a:	68a5      	ldr	r5, [r4, #8]
3000a21c:	692a      	ldr	r2, [r5, #16]
3000a21e:	fbb2 fcf3 	udiv	ip, r2, r3
3000a222:	fb03 221c 	mls	r2, r3, ip, r2
3000a226:	2a00      	cmp	r2, #0
3000a228:	f000 824d 	beq.w	3000a6c6 <rxi316_dram_init+0x83a>
3000a22c:	f10c 0201 	add.w	r2, ip, #1
3000a230:	9206      	str	r2, [sp, #24]
3000a232:	69aa      	ldr	r2, [r5, #24]
3000a234:	fbb2 f0f3 	udiv	r0, r2, r3
3000a238:	fb03 2210 	mls	r2, r3, r0, r2
3000a23c:	b102      	cbz	r2, 3000a240 <rxi316_dram_init+0x3b4>
3000a23e:	3001      	adds	r0, #1
3000a240:	68ea      	ldr	r2, [r5, #12]
3000a242:	fbb2 f1f3 	udiv	r1, r2, r3
3000a246:	fb03 2211 	mls	r2, r3, r1, r2
3000a24a:	9103      	str	r1, [sp, #12]
3000a24c:	b10a      	cbz	r2, 3000a252 <rxi316_dram_init+0x3c6>
3000a24e:	3101      	adds	r1, #1
3000a250:	9103      	str	r1, [sp, #12]
3000a252:	6a6a      	ldr	r2, [r5, #36]	; 0x24
3000a254:	fbb2 f1f3 	udiv	r1, r2, r3
3000a258:	fb03 2211 	mls	r2, r3, r1, r2
3000a25c:	b102      	cbz	r2, 3000a260 <rxi316_dram_init+0x3d4>
3000a25e:	3101      	adds	r1, #1
3000a260:	6822      	ldr	r2, [r4, #0]
3000a262:	7812      	ldrb	r2, [r2, #0]
3000a264:	2a09      	cmp	r2, #9
3000a266:	f000 8259 	beq.w	3000a71c <rxi316_dram_init+0x890>
3000a26a:	2a02      	cmp	r2, #2
3000a26c:	d104      	bne.n	3000a278 <rxi316_dram_init+0x3ec>
3000a26e:	9a08      	ldr	r2, [sp, #32]
3000a270:	3a02      	subs	r2, #2
3000a272:	fa22 f20b 	lsr.w	r2, r2, fp
3000a276:	4411      	add	r1, r2
3000a278:	68e2      	ldr	r2, [r4, #12]
3000a27a:	0349      	lsls	r1, r1, #13
3000a27c:	6d2d      	ldr	r5, [r5, #80]	; 0x50
3000a27e:	6812      	ldr	r2, [r2, #0]
3000a280:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000a284:	fa25 f60b 	lsr.w	r6, r5, fp
3000a288:	ea15 0f0a 	tst.w	r5, sl
3000a28c:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000a28e:	bf18      	it	ne
3000a290:	3601      	addne	r6, #1
3000a292:	05d2      	lsls	r2, r2, #23
3000a294:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000a298:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000a29c:	9a06      	ldr	r2, [sp, #24]
3000a29e:	0112      	lsls	r2, r2, #4
3000a2a0:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000a2a4:	4332      	orrs	r2, r6
3000a2a6:	0246      	lsls	r6, r0, #9
3000a2a8:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000a2ac:	4316      	orrs	r6, r2
3000a2ae:	9a03      	ldr	r2, [sp, #12]
3000a2b0:	f002 020f 	and.w	r2, r2, #15
3000a2b4:	4316      	orrs	r6, r2
3000a2b6:	4a94      	ldr	r2, [pc, #592]	; (3000a508 <rxi316_dram_init+0x67c>)
3000a2b8:	4331      	orrs	r1, r6
3000a2ba:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000a2be:	6151      	str	r1, [r2, #20]
3000a2c0:	68a7      	ldr	r7, [r4, #8]
3000a2c2:	697a      	ldr	r2, [r7, #20]
3000a2c4:	fbb2 fef3 	udiv	lr, r2, r3
3000a2c8:	fb03 221e 	mls	r2, r3, lr, r2
3000a2cc:	b10a      	cbz	r2, 3000a2d2 <rxi316_dram_init+0x446>
3000a2ce:	f10e 0e01 	add.w	lr, lr, #1
3000a2d2:	6939      	ldr	r1, [r7, #16]
3000a2d4:	fbb1 f2f3 	udiv	r2, r1, r3
3000a2d8:	fb03 1112 	mls	r1, r3, r2, r1
3000a2dc:	b101      	cbz	r1, 3000a2e0 <rxi316_dram_init+0x454>
3000a2de:	3201      	adds	r2, #1
3000a2e0:	68fd      	ldr	r5, [r7, #12]
3000a2e2:	fbb5 f1f3 	udiv	r1, r5, r3
3000a2e6:	fb03 5511 	mls	r5, r3, r1, r5
3000a2ea:	b105      	cbz	r5, 3000a2ee <rxi316_dram_init+0x462>
3000a2ec:	3101      	adds	r1, #1
3000a2ee:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000a2f0:	fbb5 f6f3 	udiv	r6, r5, r3
3000a2f4:	fb03 5516 	mls	r5, r3, r6, r5
3000a2f8:	b105      	cbz	r5, 3000a2fc <rxi316_dram_init+0x470>
3000a2fa:	3601      	adds	r6, #1
3000a2fc:	68bd      	ldr	r5, [r7, #8]
3000a2fe:	4411      	add	r1, r2
3000a300:	fbb5 f2f3 	udiv	r2, r5, r3
3000a304:	42b1      	cmp	r1, r6
3000a306:	fb03 5512 	mls	r5, r3, r2, r5
3000a30a:	bf38      	it	cc
3000a30c:	4631      	movcc	r1, r6
3000a30e:	b905      	cbnz	r5, 3000a312 <rxi316_dram_init+0x486>
3000a310:	3a01      	subs	r2, #1
3000a312:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000a314:	ea15 0f0a 	tst.w	r5, sl
3000a318:	fa25 fc0b 	lsr.w	ip, r5, fp
3000a31c:	69fd      	ldr	r5, [r7, #28]
3000a31e:	bf18      	it	ne
3000a320:	f10c 0c01 	addne.w	ip, ip, #1
3000a324:	fa25 f60b 	lsr.w	r6, r5, fp
3000a328:	ea15 0f0a 	tst.w	r5, sl
3000a32c:	bf18      	it	ne
3000a32e:	3601      	addne	r6, #1
3000a330:	f1b9 0f09 	cmp.w	r9, #9
3000a334:	f000 81ed 	beq.w	3000a712 <rxi316_dram_init+0x886>
3000a338:	f1b9 0f02 	cmp.w	r9, #2
3000a33c:	f000 81f4 	beq.w	3000a728 <rxi316_dram_init+0x89c>
3000a340:	f1b9 0f03 	cmp.w	r9, #3
3000a344:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000a346:	f000 827d 	beq.w	3000a844 <rxi316_dram_init+0x9b8>
3000a34a:	46a8      	mov	r8, r5
3000a34c:	9d01      	ldr	r5, [sp, #4]
3000a34e:	44a8      	add	r8, r5
3000a350:	fa28 f50b 	lsr.w	r5, r8, fp
3000a354:	ea18 0f0a 	tst.w	r8, sl
3000a358:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000a35c:	bf18      	it	ne
3000a35e:	3501      	addne	r5, #1
3000a360:	fbb8 f7f3 	udiv	r7, r8, r3
3000a364:	fb03 8817 	mls	r8, r3, r7, r8
3000a368:	f1b8 0f00 	cmp.w	r8, #0
3000a36c:	d000      	beq.n	3000a370 <rxi316_dram_init+0x4e4>
3000a36e:	3701      	adds	r7, #1
3000a370:	0109      	lsls	r1, r1, #4
3000a372:	f00e 0e0f 	and.w	lr, lr, #15
3000a376:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000a37a:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000a37e:	063f      	lsls	r7, r7, #24
3000a380:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000a384:	ea41 0e0e 	orr.w	lr, r1, lr
3000a388:	0291      	lsls	r1, r2, #10
3000a38a:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000a38e:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000a392:	ea4e 0e01 	orr.w	lr, lr, r1
3000a396:	0471      	lsls	r1, r6, #17
3000a398:	ea4e 0c0c 	orr.w	ip, lr, ip
3000a39c:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000a3a0:	ea4c 0c01 	orr.w	ip, ip, r1
3000a3a4:	0529      	lsls	r1, r5, #20
3000a3a6:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a3aa:	ea4c 0c01 	orr.w	ip, ip, r1
3000a3ae:	4956      	ldr	r1, [pc, #344]	; (3000a508 <rxi316_dram_init+0x67c>)
3000a3b0:	ea4c 0707 	orr.w	r7, ip, r7
3000a3b4:	618f      	str	r7, [r1, #24]
3000a3b6:	6821      	ldr	r1, [r4, #0]
3000a3b8:	780f      	ldrb	r7, [r1, #0]
3000a3ba:	2f02      	cmp	r7, #2
3000a3bc:	f000 8153 	beq.w	3000a666 <rxi316_dram_init+0x7da>
3000a3c0:	2f09      	cmp	r7, #9
3000a3c2:	f000 8150 	beq.w	3000a666 <rxi316_dram_init+0x7da>
3000a3c6:	68e1      	ldr	r1, [r4, #12]
3000a3c8:	2f03      	cmp	r7, #3
3000a3ca:	6809      	ldr	r1, [r1, #0]
3000a3cc:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000a3ce:	f001 080f 	and.w	r8, r1, #15
3000a3d2:	68a1      	ldr	r1, [r4, #8]
3000a3d4:	6a09      	ldr	r1, [r1, #32]
3000a3d6:	d005      	beq.n	3000a3e4 <rxi316_dram_init+0x558>
3000a3d8:	ea0a 0701 	and.w	r7, sl, r1
3000a3dc:	fa21 f10b 	lsr.w	r1, r1, fp
3000a3e0:	b107      	cbz	r7, 3000a3e4 <rxi316_dram_init+0x558>
3000a3e2:	3101      	adds	r1, #1
3000a3e4:	f1b9 0f02 	cmp.w	r9, #2
3000a3e8:	f000 8155 	beq.w	3000a696 <rxi316_dram_init+0x80a>
3000a3ec:	f1b9 0f03 	cmp.w	r9, #3
3000a3f0:	f000 819d 	beq.w	3000a72e <rxi316_dram_init+0x8a2>
3000a3f4:	f1b9 0f09 	cmp.w	r9, #9
3000a3f8:	f000 8214 	beq.w	3000a824 <rxi316_dram_init+0x998>
3000a3fc:	f04f 0e00 	mov.w	lr, #0
3000a400:	46f1      	mov	r9, lr
3000a402:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000a406:	0109      	lsls	r1, r1, #4
3000a408:	9f06      	ldr	r7, [sp, #24]
3000a40a:	b2c9      	uxtb	r1, r1
3000a40c:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000a410:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a412:	ea41 0108 	orr.w	r1, r1, r8
3000a416:	4339      	orrs	r1, r7
3000a418:	4f3b      	ldr	r7, [pc, #236]	; (3000a508 <rxi316_dram_init+0x67c>)
3000a41a:	ea41 0109 	orr.w	r1, r1, r9
3000a41e:	ea41 010e 	orr.w	r1, r1, lr
3000a422:	61f9      	str	r1, [r7, #28]
3000a424:	68a7      	ldr	r7, [r4, #8]
3000a426:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000a428:	fbb4 f1f3 	udiv	r1, r4, r3
3000a42c:	fb03 4311 	mls	r3, r3, r1, r4
3000a430:	b103      	cbz	r3, 3000a434 <rxi316_dram_init+0x5a8>
3000a432:	3101      	adds	r1, #1
3000a434:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000a436:	094b      	lsrs	r3, r1, #5
3000a438:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000a43c:	0912      	lsrs	r2, r2, #4
3000a43e:	ea14 0f0a 	tst.w	r4, sl
3000a442:	fa24 f80b 	lsr.w	r8, r4, fp
3000a446:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000a448:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000a44c:	bf18      	it	ne
3000a44e:	f108 0801 	addne.w	r8, r8, #1
3000a452:	9306      	str	r3, [sp, #24]
3000a454:	ea14 0f0a 	tst.w	r4, sl
3000a458:	fa24 fe0b 	lsr.w	lr, r4, fp
3000a45c:	9b01      	ldr	r3, [sp, #4]
3000a45e:	fa29 f40b 	lsr.w	r4, r9, fp
3000a462:	bf18      	it	ne
3000a464:	f10e 0e01 	addne.w	lr, lr, #1
3000a468:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000a46a:	ea19 0f0a 	tst.w	r9, sl
3000a46e:	fa23 f90b 	lsr.w	r9, r3, fp
3000a472:	9905      	ldr	r1, [sp, #20]
3000a474:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000a478:	bf18      	it	ne
3000a47a:	3401      	addne	r4, #1
3000a47c:	45b9      	cmp	r9, r7
3000a47e:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000a482:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000a486:	f240 80ec 	bls.w	3000a662 <rxi316_dram_init+0x7d6>
3000a48a:	eba9 0707 	sub.w	r7, r9, r7
3000a48e:	fa27 f40b 	lsr.w	r4, r7, fp
3000a492:	ea17 070a 	ands.w	r7, r7, sl
3000a496:	d001      	beq.n	3000a49c <rxi316_dram_init+0x610>
3000a498:	3401      	adds	r4, #1
3000a49a:	2700      	movs	r7, #0
3000a49c:	9b04      	ldr	r3, [sp, #16]
3000a49e:	b10b      	cbz	r3, 3000a4a4 <rxi316_dram_init+0x618>
3000a4a0:	073b      	lsls	r3, r7, #28
3000a4a2:	9304      	str	r3, [sp, #16]
3000a4a4:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000a4a8:	9b06      	ldr	r3, [sp, #24]
3000a4aa:	0289      	lsls	r1, r1, #10
3000a4ac:	02c0      	lsls	r0, r0, #11
3000a4ae:	f00c 0c04 	and.w	ip, ip, #4
3000a4b2:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000a4b6:	01d2      	lsls	r2, r2, #7
3000a4b8:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000a4bc:	0236      	lsls	r6, r6, #8
3000a4be:	ea4c 0c01 	orr.w	ip, ip, r1
3000a4c2:	b2d2      	uxtb	r2, r2
3000a4c4:	026d      	lsls	r5, r5, #9
3000a4c6:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000a4ca:	ea4c 0c00 	orr.w	ip, ip, r0
3000a4ce:	00df      	lsls	r7, r3, #3
3000a4d0:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000a4d4:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000a4d8:	ea4c 0202 	orr.w	r2, ip, r2
3000a4dc:	f007 0708 	and.w	r7, r7, #8
3000a4e0:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000a4e4:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000a4e8:	4332      	orrs	r2, r6
3000a4ea:	0524      	lsls	r4, r4, #20
3000a4ec:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000a4f0:	4905      	ldr	r1, [pc, #20]	; (3000a508 <rxi316_dram_init+0x67c>)
3000a4f2:	ea42 0305 	orr.w	r3, r2, r5
3000a4f6:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000a4fa:	9a04      	ldr	r2, [sp, #16]
3000a4fc:	433b      	orrs	r3, r7
3000a4fe:	ea43 0308 	orr.w	r3, r3, r8
3000a502:	e003      	b.n	3000a50c <rxi316_dram_init+0x680>
3000a504:	00ffff00 	.word	0x00ffff00
3000a508:	41100000 	.word	0x41100000
3000a50c:	ea43 030e 	orr.w	r3, r3, lr
3000a510:	4323      	orrs	r3, r4
3000a512:	4313      	orrs	r3, r2
3000a514:	9a03      	ldr	r2, [sp, #12]
3000a516:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000a51a:	620b      	str	r3, [r1, #32]
3000a51c:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000a520:	9b02      	ldr	r3, [sp, #8]
3000a522:	2b01      	cmp	r3, #1
3000a524:	d002      	beq.n	3000a52c <rxi316_dram_init+0x6a0>
3000a526:	9b08      	ldr	r3, [sp, #32]
3000a528:	3303      	adds	r3, #3
3000a52a:	9307      	str	r3, [sp, #28]
3000a52c:	4bb2      	ldr	r3, [pc, #712]	; (3000a7f8 <rxi316_dram_init+0x96c>)
3000a52e:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000a532:	9a07      	ldr	r2, [sp, #28]
3000a534:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000a538:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000a53c:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000a540:	60da      	str	r2, [r3, #12]
3000a542:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a546:	430a      	orrs	r2, r1
3000a548:	49ac      	ldr	r1, [pc, #688]	; (3000a7fc <rxi316_dram_init+0x970>)
3000a54a:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a54e:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000a552:	0096      	lsls	r6, r2, #2
3000a554:	d57f      	bpl.n	3000a656 <rxi316_dram_init+0x7ca>
3000a556:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000a55a:	4ca7      	ldr	r4, [pc, #668]	; (3000a7f8 <rxi316_dram_init+0x96c>)
3000a55c:	601a      	str	r2, [r3, #0]
3000a55e:	691a      	ldr	r2, [r3, #16]
3000a560:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000a564:	611a      	str	r2, [r3, #16]
3000a566:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000a56a:	f043 0303 	orr.w	r3, r3, #3
3000a56e:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000a572:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000a576:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a578:	2b01      	cmp	r3, #1
3000a57a:	4ba1      	ldr	r3, [pc, #644]	; (3000a800 <rxi316_dram_init+0x974>)
3000a57c:	bf0c      	ite	eq
3000a57e:	4aa1      	ldreq	r2, [pc, #644]	; (3000a804 <rxi316_dram_init+0x978>)
3000a580:	4aa1      	ldrne	r2, [pc, #644]	; (3000a808 <rxi316_dram_init+0x97c>)
3000a582:	601a      	str	r2, [r3, #0]
3000a584:	2203      	movs	r2, #3
3000a586:	68e0      	ldr	r0, [r4, #12]
3000a588:	4ba0      	ldr	r3, [pc, #640]	; (3000a80c <rxi316_dram_init+0x980>)
3000a58a:	4003      	ands	r3, r0
3000a58c:	60e3      	str	r3, [r4, #12]
3000a58e:	2308      	movs	r3, #8
3000a590:	62a1      	str	r1, [r4, #40]	; 0x28
3000a592:	62e2      	str	r2, [r4, #44]	; 0x2c
3000a594:	6023      	str	r3, [r4, #0]
3000a596:	6823      	ldr	r3, [r4, #0]
3000a598:	071d      	lsls	r5, r3, #28
3000a59a:	d5fc      	bpl.n	3000a596 <rxi316_dram_init+0x70a>
3000a59c:	4b9c      	ldr	r3, [pc, #624]	; (3000a810 <rxi316_dram_init+0x984>)
3000a59e:	2001      	movs	r0, #1
3000a5a0:	4798      	blx	r3
3000a5a2:	68e2      	ldr	r2, [r4, #12]
3000a5a4:	4b99      	ldr	r3, [pc, #612]	; (3000a80c <rxi316_dram_init+0x980>)
3000a5a6:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000a5aa:	4013      	ands	r3, r2
3000a5ac:	2203      	movs	r2, #3
3000a5ae:	60e3      	str	r3, [r4, #12]
3000a5b0:	2308      	movs	r3, #8
3000a5b2:	62a1      	str	r1, [r4, #40]	; 0x28
3000a5b4:	62e2      	str	r2, [r4, #44]	; 0x2c
3000a5b6:	4a90      	ldr	r2, [pc, #576]	; (3000a7f8 <rxi316_dram_init+0x96c>)
3000a5b8:	6023      	str	r3, [r4, #0]
3000a5ba:	6813      	ldr	r3, [r2, #0]
3000a5bc:	071c      	lsls	r4, r3, #28
3000a5be:	d5fc      	bpl.n	3000a5ba <rxi316_dram_init+0x72e>
3000a5c0:	68d0      	ldr	r0, [r2, #12]
3000a5c2:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000a5c6:	4b91      	ldr	r3, [pc, #580]	; (3000a80c <rxi316_dram_init+0x980>)
3000a5c8:	4003      	ands	r3, r0
3000a5ca:	2008      	movs	r0, #8
3000a5cc:	60d3      	str	r3, [r2, #12]
3000a5ce:	6291      	str	r1, [r2, #40]	; 0x28
3000a5d0:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000a5d2:	4989      	ldr	r1, [pc, #548]	; (3000a7f8 <rxi316_dram_init+0x96c>)
3000a5d4:	f023 0302 	bic.w	r3, r3, #2
3000a5d8:	62d3      	str	r3, [r2, #44]	; 0x2c
3000a5da:	6010      	str	r0, [r2, #0]
3000a5dc:	680b      	ldr	r3, [r1, #0]
3000a5de:	0718      	lsls	r0, r3, #28
3000a5e0:	d5fc      	bpl.n	3000a5dc <rxi316_dram_init+0x750>
3000a5e2:	4a85      	ldr	r2, [pc, #532]	; (3000a7f8 <rxi316_dram_init+0x96c>)
3000a5e4:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000a5e8:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a5ec:	60d3      	str	r3, [r2, #12]
3000a5ee:	980a      	ldr	r0, [sp, #40]	; 0x28
3000a5f0:	6913      	ldr	r3, [r2, #16]
3000a5f2:	4318      	orrs	r0, r3
3000a5f4:	6110      	str	r0, [r2, #16]
3000a5f6:	6913      	ldr	r3, [r2, #16]
3000a5f8:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000a5fc:	6113      	str	r3, [r2, #16]
3000a5fe:	6011      	str	r1, [r2, #0]
3000a600:	68d3      	ldr	r3, [r2, #12]
3000a602:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000a606:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000a60a:	d1f9      	bne.n	3000a600 <rxi316_dram_init+0x774>
3000a60c:	b00f      	add	sp, #60	; 0x3c
3000a60e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a612:	2a01      	cmp	r2, #1
3000a614:	f000 80b4 	beq.w	3000a780 <rxi316_dram_init+0x8f4>
3000a618:	2a02      	cmp	r2, #2
3000a61a:	f000 811d 	beq.w	3000a858 <rxi316_dram_init+0x9cc>
3000a61e:	2a03      	cmp	r2, #3
3000a620:	f47f acda 	bne.w	30009fd8 <rxi316_dram_init+0x14c>
3000a624:	686a      	ldr	r2, [r5, #4]
3000a626:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a62a:	606a      	str	r2, [r5, #4]
3000a62c:	e4d4      	b.n	30009fd8 <rxi316_dram_init+0x14c>
3000a62e:	2a00      	cmp	r2, #0
3000a630:	f43f ad60 	beq.w	3000a0f4 <rxi316_dram_init+0x268>
3000a634:	2203      	movs	r2, #3
3000a636:	4310      	orrs	r0, r2
3000a638:	68a2      	ldr	r2, [r4, #8]
3000a63a:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a63c:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000a63e:	013f      	lsls	r7, r7, #4
3000a640:	2d00      	cmp	r5, #0
3000a642:	f000 80ba 	beq.w	3000a7ba <rxi316_dram_init+0x92e>
3000a646:	1e91      	subs	r1, r2, #2
3000a648:	2906      	cmp	r1, #6
3000a64a:	f200 8093 	bhi.w	3000a774 <rxi316_dram_init+0x8e8>
3000a64e:	1e55      	subs	r5, r2, #1
3000a650:	2100      	movs	r1, #0
3000a652:	026d      	lsls	r5, r5, #9
3000a654:	e555      	b.n	3000a102 <rxi316_dram_init+0x276>
3000a656:	2201      	movs	r2, #1
3000a658:	601a      	str	r2, [r3, #0]
3000a65a:	681a      	ldr	r2, [r3, #0]
3000a65c:	07d2      	lsls	r2, r2, #31
3000a65e:	d5fc      	bpl.n	3000a65a <rxi316_dram_init+0x7ce>
3000a660:	e7bf      	b.n	3000a5e2 <rxi316_dram_init+0x756>
3000a662:	2701      	movs	r7, #1
3000a664:	e71a      	b.n	3000a49c <rxi316_dram_init+0x610>
3000a666:	68a1      	ldr	r1, [r4, #8]
3000a668:	2f03      	cmp	r7, #3
3000a66a:	6a09      	ldr	r1, [r1, #32]
3000a66c:	f000 80a2 	beq.w	3000a7b4 <rxi316_dram_init+0x928>
3000a670:	2f02      	cmp	r7, #2
3000a672:	f040 80fc 	bne.w	3000a86e <rxi316_dram_init+0x9e2>
3000a676:	fbb1 fcf3 	udiv	ip, r1, r3
3000a67a:	fb03 1e1c 	mls	lr, r3, ip, r1
3000a67e:	4661      	mov	r1, ip
3000a680:	f1be 0f00 	cmp.w	lr, #0
3000a684:	f000 808c 	beq.w	3000a7a0 <rxi316_dram_init+0x914>
3000a688:	f1b9 0f02 	cmp.w	r9, #2
3000a68c:	f101 0101 	add.w	r1, r1, #1
3000a690:	46b8      	mov	r8, r7
3000a692:	f47f aeab 	bne.w	3000a3ec <rxi316_dram_init+0x560>
3000a696:	4f5f      	ldr	r7, [pc, #380]	; (3000a814 <rxi316_dram_init+0x988>)
3000a698:	f04f 0e00 	mov.w	lr, #0
3000a69c:	f8df 9178 	ldr.w	r9, [pc, #376]	; 3000a818 <rxi316_dram_init+0x98c>
3000a6a0:	fbb7 f7f3 	udiv	r7, r7, r3
3000a6a4:	fbb9 f9f3 	udiv	r9, r9, r3
3000a6a8:	09ff      	lsrs	r7, r7, #7
3000a6aa:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a6ae:	3701      	adds	r7, #1
3000a6b0:	f109 0c01 	add.w	ip, r9, #1
3000a6b4:	f8df 9164 	ldr.w	r9, [pc, #356]	; 3000a81c <rxi316_dram_init+0x990>
3000a6b8:	023f      	lsls	r7, r7, #8
3000a6ba:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000a6be:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000a6c2:	9709      	str	r7, [sp, #36]	; 0x24
3000a6c4:	e69f      	b.n	3000a406 <rxi316_dram_init+0x57a>
3000a6c6:	f8cd c018 	str.w	ip, [sp, #24]
3000a6ca:	e5b2      	b.n	3000a232 <rxi316_dram_init+0x3a6>
3000a6cc:	38c8      	subs	r0, #200	; 0xc8
3000a6ce:	e58f      	b.n	3000a1f0 <rxi316_dram_init+0x364>
3000a6d0:	9105      	str	r1, [sp, #20]
3000a6d2:	e580      	b.n	3000a1d6 <rxi316_dram_init+0x34a>
3000a6d4:	ea1a 0f07 	tst.w	sl, r7
3000a6d8:	fa27 f00b 	lsr.w	r0, r7, fp
3000a6dc:	f43f ad55 	beq.w	3000a18a <rxi316_dram_init+0x2fe>
3000a6e0:	e552      	b.n	3000a188 <rxi316_dram_init+0x2fc>
3000a6e2:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000a6e4:	1f50      	subs	r0, r2, #5
3000a6e6:	00c0      	lsls	r0, r0, #3
3000a6e8:	68e2      	ldr	r2, [r4, #12]
3000a6ea:	6812      	ldr	r2, [r2, #0]
3000a6ec:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000a6ee:	fab2 f282 	clz	r2, r2
3000a6f2:	0952      	lsrs	r2, r2, #5
3000a6f4:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000a6f8:	4301      	orrs	r1, r0
3000a6fa:	e519      	b.n	3000a130 <rxi316_dram_init+0x2a4>
3000a6fc:	f1a1 0502 	sub.w	r5, r1, #2
3000a700:	2909      	cmp	r1, #9
3000a702:	fab5 f585 	clz	r5, r5
3000a706:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000a70a:	f000 80a3 	beq.w	3000a854 <rxi316_dram_init+0x9c8>
3000a70e:	462a      	mov	r2, r5
3000a710:	e4ed      	b.n	3000a0ee <rxi316_dram_init+0x262>
3000a712:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000a714:	46a8      	mov	r8, r5
3000a716:	9d08      	ldr	r5, [sp, #32]
3000a718:	44a8      	add	r8, r5
3000a71a:	e619      	b.n	3000a350 <rxi316_dram_init+0x4c4>
3000a71c:	9a08      	ldr	r2, [sp, #32]
3000a71e:	fa22 f10b 	lsr.w	r1, r2, fp
3000a722:	e5a9      	b.n	3000a278 <rxi316_dram_init+0x3ec>
3000a724:	2000      	movs	r0, #0
3000a726:	e7df      	b.n	3000a6e8 <rxi316_dram_init+0x85c>
3000a728:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000a72c:	e610      	b.n	3000a350 <rxi316_dram_init+0x4c4>
3000a72e:	f242 7710 	movw	r7, #10000	; 0x2710
3000a732:	f8df 90ec 	ldr.w	r9, [pc, #236]	; 3000a820 <rxi316_dram_init+0x994>
3000a736:	f8df e0e0 	ldr.w	lr, [pc, #224]	; 3000a818 <rxi316_dram_init+0x98c>
3000a73a:	fbb7 fcf3 	udiv	ip, r7, r3
3000a73e:	9f05      	ldr	r7, [sp, #20]
3000a740:	4467      	add	r7, ip
3000a742:	fbb9 f9f3 	udiv	r9, r9, r3
3000a746:	fbbe fef3 	udiv	lr, lr, r3
3000a74a:	09ff      	lsrs	r7, r7, #7
3000a74c:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a750:	3701      	adds	r7, #1
3000a752:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000a756:	f109 0c01 	add.w	ip, r9, #1
3000a75a:	f8df 90c0 	ldr.w	r9, [pc, #192]	; 3000a81c <rxi316_dram_init+0x990>
3000a75e:	023f      	lsls	r7, r7, #8
3000a760:	f10e 0e01 	add.w	lr, lr, #1
3000a764:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000a768:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000a76c:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000a770:	9709      	str	r7, [sp, #36]	; 0x24
3000a772:	e648      	b.n	3000a406 <rxi316_dram_init+0x57a>
3000a774:	2100      	movs	r1, #0
3000a776:	460d      	mov	r5, r1
3000a778:	e4c3      	b.n	3000a102 <rxi316_dram_init+0x276>
3000a77a:	3a07      	subs	r2, #7
3000a77c:	f7ff bbcb 	b.w	30009f16 <rxi316_dram_init+0x8a>
3000a780:	686a      	ldr	r2, [r5, #4]
3000a782:	f042 0220 	orr.w	r2, r2, #32
3000a786:	606a      	str	r2, [r5, #4]
3000a788:	e426      	b.n	30009fd8 <rxi316_dram_init+0x14c>
3000a78a:	2204      	movs	r2, #4
3000a78c:	2102      	movs	r1, #2
3000a78e:	2500      	movs	r5, #0
3000a790:	9201      	str	r2, [sp, #4]
3000a792:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000a796:	f7ff bba7 	b.w	30009ee8 <rxi316_dram_init+0x5c>
3000a79a:	3a08      	subs	r2, #8
3000a79c:	f7ff bbbb 	b.w	30009f16 <rxi316_dram_init+0x8a>
3000a7a0:	46b8      	mov	r8, r7
3000a7a2:	e61f      	b.n	3000a3e4 <rxi316_dram_init+0x558>
3000a7a4:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000a7a6:	2a04      	cmp	r2, #4
3000a7a8:	d90c      	bls.n	3000a7c4 <rxi316_dram_init+0x938>
3000a7aa:	2a0b      	cmp	r2, #11
3000a7ac:	d86b      	bhi.n	3000a886 <rxi316_dram_init+0x9fa>
3000a7ae:	1f17      	subs	r7, r2, #4
3000a7b0:	013f      	lsls	r7, r7, #4
3000a7b2:	e008      	b.n	3000a7c6 <rxi316_dram_init+0x93a>
3000a7b4:	f04f 0802 	mov.w	r8, #2
3000a7b8:	e614      	b.n	3000a3e4 <rxi316_dram_init+0x558>
3000a7ba:	2903      	cmp	r1, #3
3000a7bc:	d005      	beq.n	3000a7ca <rxi316_dram_init+0x93e>
3000a7be:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a7c2:	e49e      	b.n	3000a102 <rxi316_dram_init+0x276>
3000a7c4:	2710      	movs	r7, #16
3000a7c6:	68a2      	ldr	r2, [r4, #8]
3000a7c8:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000a7ca:	2a04      	cmp	r2, #4
3000a7cc:	d94a      	bls.n	3000a864 <rxi316_dram_init+0x9d8>
3000a7ce:	1f51      	subs	r1, r2, #5
3000a7d0:	2903      	cmp	r1, #3
3000a7d2:	d84f      	bhi.n	3000a874 <rxi316_dram_init+0x9e8>
3000a7d4:	1f15      	subs	r5, r2, #4
3000a7d6:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a7da:	026d      	lsls	r5, r5, #9
3000a7dc:	e491      	b.n	3000a102 <rxi316_dram_init+0x276>
3000a7de:	f1a1 0c02 	sub.w	ip, r1, #2
3000a7e2:	f1bc 0f01 	cmp.w	ip, #1
3000a7e6:	d903      	bls.n	3000a7f0 <rxi316_dram_init+0x964>
3000a7e8:	f1be 0f00 	cmp.w	lr, #0
3000a7ec:	f43f ac7f 	beq.w	3000a0ee <rxi316_dram_init+0x262>
3000a7f0:	f040 0002 	orr.w	r0, r0, #2
3000a7f4:	e47b      	b.n	3000a0ee <rxi316_dram_init+0x262>
3000a7f6:	bf00      	nop
3000a7f8:	41100000 	.word	0x41100000
3000a7fc:	42008000 	.word	0x42008000
3000a800:	41011000 	.word	0x41011000
3000a804:	44000103 	.word	0x44000103
3000a808:	44000021 	.word	0x44000021
3000a80c:	fff8fffc 	.word	0xfff8fffc
3000a810:	00009b2d 	.word	0x00009b2d
3000a814:	00061a80 	.word	0x00061a80
3000a818:	0bebc200 	.word	0x0bebc200
3000a81c:	003ff000 	.word	0x003ff000
3000a820:	1dcd6500 	.word	0x1dcd6500
3000a824:	4f1d      	ldr	r7, [pc, #116]	; (3000a89c <rxi316_dram_init+0xa10>)
3000a826:	f04f 0e00 	mov.w	lr, #0
3000a82a:	fbb7 f9f3 	udiv	r9, r7, r3
3000a82e:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a832:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000a836:	f109 0701 	add.w	r7, r9, #1
3000a83a:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000a8a0 <rxi316_dram_init+0xa14>
3000a83e:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000a842:	e5e0      	b.n	3000a406 <rxi316_dram_init+0x57a>
3000a844:	f105 0802 	add.w	r8, r5, #2
3000a848:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000a84a:	eba8 0805 	sub.w	r8, r8, r5
3000a84e:	9d08      	ldr	r5, [sp, #32]
3000a850:	44a8      	add	r8, r5
3000a852:	e57d      	b.n	3000a350 <rxi316_dram_init+0x4c4>
3000a854:	2204      	movs	r2, #4
3000a856:	e6ee      	b.n	3000a636 <rxi316_dram_init+0x7aa>
3000a858:	686a      	ldr	r2, [r5, #4]
3000a85a:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000a85e:	606a      	str	r2, [r5, #4]
3000a860:	f7ff bbba 	b.w	30009fd8 <rxi316_dram_init+0x14c>
3000a864:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a868:	f44f 7500 	mov.w	r5, #512	; 0x200
3000a86c:	e449      	b.n	3000a102 <rxi316_dram_init+0x276>
3000a86e:	f04f 0802 	mov.w	r8, #2
3000a872:	e5b1      	b.n	3000a3d8 <rxi316_dram_init+0x54c>
3000a874:	f1a2 0109 	sub.w	r1, r2, #9
3000a878:	2905      	cmp	r1, #5
3000a87a:	d80b      	bhi.n	3000a894 <rxi316_dram_init+0xa08>
3000a87c:	0855      	lsrs	r5, r2, #1
3000a87e:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a882:	026d      	lsls	r5, r5, #9
3000a884:	e43d      	b.n	3000a102 <rxi316_dram_init+0x276>
3000a886:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000a888:	f040 0004 	orr.w	r0, r0, #4
3000a88c:	f1a2 070c 	sub.w	r7, r2, #12
3000a890:	013f      	lsls	r7, r7, #4
3000a892:	e798      	b.n	3000a7c6 <rxi316_dram_init+0x93a>
3000a894:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a898:	2500      	movs	r5, #0
3000a89a:	e432      	b.n	3000a102 <rxi316_dram_init+0x276>
3000a89c:	0bebc200 	.word	0x0bebc200
3000a8a0:	003ff000 	.word	0x003ff000

3000a8a4 <rxi316_ftend_init>:
3000a8a4:	2300      	movs	r3, #0
3000a8a6:	b510      	push	{r4, lr}
3000a8a8:	4c0c      	ldr	r4, [pc, #48]	; (3000a8dc <rxi316_ftend_init+0x38>)
3000a8aa:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000a8ae:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000a8b2:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000a8b6:	f7fe fe5d 	bl	30009574 <ddr_init_index>
3000a8ba:	4a09      	ldr	r2, [pc, #36]	; (3000a8e0 <rxi316_ftend_init+0x3c>)
3000a8bc:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000a8c0:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000a8c4:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000a8c8:	68cb      	ldr	r3, [r1, #12]
3000a8ca:	689b      	ldr	r3, [r3, #8]
3000a8cc:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000a8ce:	3301      	adds	r3, #1
3000a8d0:	b2db      	uxtb	r3, r3
3000a8d2:	4313      	orrs	r3, r2
3000a8d4:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000a8d8:	bd10      	pop	{r4, pc}
3000a8da:	bf00      	nop
3000a8dc:	41101000 	.word	0x41101000
3000a8e0:	3000ebf0 	.word	0x3000ebf0

3000a8e4 <rxi316_DynSre_init>:
3000a8e4:	b538      	push	{r3, r4, r5, lr}
3000a8e6:	4b22      	ldr	r3, [pc, #136]	; (3000a970 <rxi316_DynSre_init+0x8c>)
3000a8e8:	4604      	mov	r4, r0
3000a8ea:	460d      	mov	r5, r1
3000a8ec:	4798      	blx	r3
3000a8ee:	2802      	cmp	r0, #2
3000a8f0:	d937      	bls.n	3000a962 <rxi316_DynSre_init+0x7e>
3000a8f2:	4b20      	ldr	r3, [pc, #128]	; (3000a974 <rxi316_DynSre_init+0x90>)
3000a8f4:	691b      	ldr	r3, [r3, #16]
3000a8f6:	f3c3 230f 	ubfx	r3, r3, #8, #16
3000a8fa:	3b46      	subs	r3, #70	; 0x46
3000a8fc:	42a3      	cmp	r3, r4
3000a8fe:	d915      	bls.n	3000a92c <rxi316_DynSre_init+0x48>
3000a900:	2d01      	cmp	r5, #1
3000a902:	d01a      	beq.n	3000a93a <rxi316_DynSre_init+0x56>
3000a904:	4a1b      	ldr	r2, [pc, #108]	; (3000a974 <rxi316_DynSre_init+0x90>)
3000a906:	6893      	ldr	r3, [r2, #8]
3000a908:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000a90c:	6093      	str	r3, [r2, #8]
3000a90e:	4b19      	ldr	r3, [pc, #100]	; (3000a974 <rxi316_DynSre_init+0x90>)
3000a910:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000a914:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a918:	625a      	str	r2, [r3, #36]	; 0x24
3000a91a:	629a      	str	r2, [r3, #40]	; 0x28
3000a91c:	69da      	ldr	r2, [r3, #28]
3000a91e:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000a922:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000a926:	61da      	str	r2, [r3, #28]
3000a928:	6019      	str	r1, [r3, #0]
3000a92a:	bd38      	pop	{r3, r4, r5, pc}
3000a92c:	f240 411e 	movw	r1, #1054	; 0x41e
3000a930:	4811      	ldr	r0, [pc, #68]	; (3000a978 <rxi316_DynSre_init+0x94>)
3000a932:	f001 f855 	bl	3000b9e0 <__io_assert_failed_veneer>
3000a936:	2d01      	cmp	r5, #1
3000a938:	d1e4      	bne.n	3000a904 <rxi316_DynSre_init+0x20>
3000a93a:	4b0e      	ldr	r3, [pc, #56]	; (3000a974 <rxi316_DynSre_init+0x90>)
3000a93c:	f64f 70fe 	movw	r0, #65534	; 0xfffe
3000a940:	4a0e      	ldr	r2, [pc, #56]	; (3000a97c <rxi316_DynSre_init+0x98>)
3000a942:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000a946:	4004      	ands	r4, r0
3000a948:	400a      	ands	r2, r1
3000a94a:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a94e:	f8d3 0100 	ldr.w	r0, [r3, #256]	; 0x100
3000a952:	4304      	orrs	r4, r0
3000a954:	f8c3 4100 	str.w	r4, [r3, #256]	; 0x100
3000a958:	689a      	ldr	r2, [r3, #8]
3000a95a:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a95e:	609a      	str	r2, [r3, #8]
3000a960:	e7d5      	b.n	3000a90e <rxi316_DynSre_init+0x2a>
3000a962:	4b07      	ldr	r3, [pc, #28]	; (3000a980 <rxi316_DynSre_init+0x9c>)
3000a964:	4798      	blx	r3
3000a966:	2801      	cmp	r0, #1
3000a968:	bf08      	it	eq
3000a96a:	2400      	moveq	r4, #0
3000a96c:	e7c8      	b.n	3000a900 <rxi316_DynSre_init+0x1c>
3000a96e:	bf00      	nop
3000a970:	30009265 	.word	0x30009265
3000a974:	41100000 	.word	0x41100000
3000a978:	3000d154 	.word	0x3000d154
3000a97c:	ffff0000 	.word	0xffff0000
3000a980:	30007631 	.word	0x30007631

3000a984 <ddr_init>:
3000a984:	b500      	push	{lr}
3000a986:	b083      	sub	sp, #12
3000a988:	f7fe fdf4 	bl	30009574 <ddr_init_index>
3000a98c:	4b06      	ldr	r3, [pc, #24]	; (3000a9a8 <ddr_init+0x24>)
3000a98e:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000a992:	9001      	str	r0, [sp, #4]
3000a994:	f7fe fe18 	bl	300095c8 <rxi316_perf_tune>
3000a998:	9801      	ldr	r0, [sp, #4]
3000a99a:	f7ff fa77 	bl	30009e8c <rxi316_dram_init>
3000a99e:	b003      	add	sp, #12
3000a9a0:	f85d eb04 	ldr.w	lr, [sp], #4
3000a9a4:	f7ff bf7e 	b.w	3000a8a4 <rxi316_ftend_init>
3000a9a8:	3000ebf0 	.word	0x3000ebf0

3000a9ac <SWR_Calib_DCore>:
3000a9ac:	4b0f      	ldr	r3, [pc, #60]	; (3000a9ec <SWR_Calib_DCore+0x40>)
3000a9ae:	f240 3231 	movw	r2, #817	; 0x331
3000a9b2:	490f      	ldr	r1, [pc, #60]	; (3000a9f0 <SWR_Calib_DCore+0x44>)
3000a9b4:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000a9b6:	4001      	ands	r1, r0
3000a9b8:	430a      	orrs	r2, r1
3000a9ba:	651a      	str	r2, [r3, #80]	; 0x50
3000a9bc:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000a9be:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000a9c2:	641a      	str	r2, [r3, #64]	; 0x40
3000a9c4:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000a9c8:	f042 0201 	orr.w	r2, r2, #1
3000a9cc:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000a9d0:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000a9d4:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000a9d8:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000a9dc:	685a      	ldr	r2, [r3, #4]
3000a9de:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000a9e2:	f042 0204 	orr.w	r2, r2, #4
3000a9e6:	605a      	str	r2, [r3, #4]
3000a9e8:	4770      	bx	lr
3000a9ea:	bf00      	nop
3000a9ec:	42008100 	.word	0x42008100
3000a9f0:	fffffccc 	.word	0xfffffccc

3000a9f4 <SWR_Calib_MEM>:
3000a9f4:	4b51      	ldr	r3, [pc, #324]	; (3000ab3c <SWR_Calib_MEM+0x148>)
3000a9f6:	f240 3231 	movw	r2, #817	; 0x331
3000a9fa:	4951      	ldr	r1, [pc, #324]	; (3000ab40 <SWR_Calib_MEM+0x14c>)
3000a9fc:	f04f 0c00 	mov.w	ip, #0
3000aa00:	b510      	push	{r4, lr}
3000aa02:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000aa06:	b082      	sub	sp, #8
3000aa08:	4001      	ands	r1, r0
3000aa0a:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000aa0e:	430a      	orrs	r2, r1
3000aa10:	f10d 0107 	add.w	r1, sp, #7
3000aa14:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000aa18:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000aa1c:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000aa20:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000aa24:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000aa28:	f042 0201 	orr.w	r2, r2, #1
3000aa2c:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000aa30:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000aa34:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000aa38:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000aa3c:	f88d c007 	strb.w	ip, [sp, #7]
3000aa40:	f7fd fb8a 	bl	30008158 <OTP_Read8>
3000aa44:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000aa48:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000aa4c:	2af0      	cmp	r2, #240	; 0xf0
3000aa4e:	d156      	bne.n	3000aafe <SWR_Calib_MEM+0x10a>
3000aa50:	f003 030f 	and.w	r3, r3, #15
3000aa54:	2b0f      	cmp	r3, #15
3000aa56:	d04e      	beq.n	3000aaf6 <SWR_Calib_MEM+0x102>
3000aa58:	2302      	movs	r3, #2
3000aa5a:	4c3a      	ldr	r4, [pc, #232]	; (3000ab44 <SWR_Calib_MEM+0x150>)
3000aa5c:	f10d 0106 	add.w	r1, sp, #6
3000aa60:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000aa64:	7023      	strb	r3, [r4, #0]
3000aa66:	f7fd fb77 	bl	30008158 <OTP_Read8>
3000aa6a:	7822      	ldrb	r2, [r4, #0]
3000aa6c:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000aa70:	2a03      	cmp	r2, #3
3000aa72:	d05a      	beq.n	3000ab2a <SWR_Calib_MEM+0x136>
3000aa74:	2a02      	cmp	r2, #2
3000aa76:	bf0c      	ite	eq
3000aa78:	f003 030f 	andeq.w	r3, r3, #15
3000aa7c:	2300      	movne	r3, #0
3000aa7e:	492f      	ldr	r1, [pc, #188]	; (3000ab3c <SWR_Calib_MEM+0x148>)
3000aa80:	4831      	ldr	r0, [pc, #196]	; (3000ab48 <SWR_Calib_MEM+0x154>)
3000aa82:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000aa86:	f022 020f 	bic.w	r2, r2, #15
3000aa8a:	4313      	orrs	r3, r2
3000aa8c:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000aa90:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000aa92:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000aa96:	2b07      	cmp	r3, #7
3000aa98:	d03a      	beq.n	3000ab10 <SWR_Calib_MEM+0x11c>
3000aa9a:	2300      	movs	r3, #0
3000aa9c:	f10d 0107 	add.w	r1, sp, #7
3000aaa0:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000aaa4:	f88d 3007 	strb.w	r3, [sp, #7]
3000aaa8:	f7fd fb56 	bl	30008158 <OTP_Read8>
3000aaac:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000aab0:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000aab4:	2af0      	cmp	r2, #240	; 0xf0
3000aab6:	d124      	bne.n	3000ab02 <SWR_Calib_MEM+0x10e>
3000aab8:	f003 030f 	and.w	r3, r3, #15
3000aabc:	2b0f      	cmp	r3, #15
3000aabe:	d022      	beq.n	3000ab06 <SWR_Calib_MEM+0x112>
3000aac0:	2302      	movs	r3, #2
3000aac2:	4c22      	ldr	r4, [pc, #136]	; (3000ab4c <SWR_Calib_MEM+0x158>)
3000aac4:	f10d 0107 	add.w	r1, sp, #7
3000aac8:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000aacc:	7023      	strb	r3, [r4, #0]
3000aace:	f7fd fb43 	bl	30008158 <OTP_Read8>
3000aad2:	7822      	ldrb	r2, [r4, #0]
3000aad4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000aad8:	2a03      	cmp	r2, #3
3000aada:	d028      	beq.n	3000ab2e <SWR_Calib_MEM+0x13a>
3000aadc:	2a02      	cmp	r2, #2
3000aade:	d029      	beq.n	3000ab34 <SWR_Calib_MEM+0x140>
3000aae0:	2200      	movs	r2, #0
3000aae2:	4916      	ldr	r1, [pc, #88]	; (3000ab3c <SWR_Calib_MEM+0x148>)
3000aae4:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000aae8:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000aaec:	4313      	orrs	r3, r2
3000aaee:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000aaf2:	b002      	add	sp, #8
3000aaf4:	bd10      	pop	{r4, pc}
3000aaf6:	4b13      	ldr	r3, [pc, #76]	; (3000ab44 <SWR_Calib_MEM+0x150>)
3000aaf8:	2201      	movs	r2, #1
3000aafa:	701a      	strb	r2, [r3, #0]
3000aafc:	e7cd      	b.n	3000aa9a <SWR_Calib_MEM+0xa6>
3000aafe:	2303      	movs	r3, #3
3000ab00:	e7ab      	b.n	3000aa5a <SWR_Calib_MEM+0x66>
3000ab02:	2303      	movs	r3, #3
3000ab04:	e7dd      	b.n	3000aac2 <SWR_Calib_MEM+0xce>
3000ab06:	4b11      	ldr	r3, [pc, #68]	; (3000ab4c <SWR_Calib_MEM+0x158>)
3000ab08:	2201      	movs	r2, #1
3000ab0a:	701a      	strb	r2, [r3, #0]
3000ab0c:	b002      	add	sp, #8
3000ab0e:	bd10      	pop	{r4, pc}
3000ab10:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000ab12:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000ab16:	d1c0      	bne.n	3000aa9a <SWR_Calib_MEM+0xa6>
3000ab18:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000ab1c:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000ab20:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000ab24:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000ab28:	e7b7      	b.n	3000aa9a <SWR_Calib_MEM+0xa6>
3000ab2a:	091b      	lsrs	r3, r3, #4
3000ab2c:	e7a7      	b.n	3000aa7e <SWR_Calib_MEM+0x8a>
3000ab2e:	091b      	lsrs	r3, r3, #4
3000ab30:	059a      	lsls	r2, r3, #22
3000ab32:	e7d6      	b.n	3000aae2 <SWR_Calib_MEM+0xee>
3000ab34:	f003 030f 	and.w	r3, r3, #15
3000ab38:	059a      	lsls	r2, r3, #22
3000ab3a:	e7d2      	b.n	3000aae2 <SWR_Calib_MEM+0xee>
3000ab3c:	42008800 	.word	0x42008800
3000ab40:	fffffccc 	.word	0xfffffccc
3000ab44:	3000ed8b 	.word	0x3000ed8b
3000ab48:	42008100 	.word	0x42008100
3000ab4c:	3000ed8a 	.word	0x3000ed8a

3000ab50 <SWR_Calib_AUD>:
3000ab50:	4b48      	ldr	r3, [pc, #288]	; (3000ac74 <SWR_Calib_AUD+0x124>)
3000ab52:	f240 3231 	movw	r2, #817	; 0x331
3000ab56:	4948      	ldr	r1, [pc, #288]	; (3000ac78 <SWR_Calib_AUD+0x128>)
3000ab58:	f04f 0c00 	mov.w	ip, #0
3000ab5c:	b510      	push	{r4, lr}
3000ab5e:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000ab62:	b082      	sub	sp, #8
3000ab64:	4001      	ands	r1, r0
3000ab66:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000ab6a:	430a      	orrs	r2, r1
3000ab6c:	f10d 0107 	add.w	r1, sp, #7
3000ab70:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000ab74:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000ab78:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000ab7c:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000ab80:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000ab84:	f042 0201 	orr.w	r2, r2, #1
3000ab88:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000ab8c:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000ab90:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000ab94:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000ab98:	f88d c007 	strb.w	ip, [sp, #7]
3000ab9c:	f7fd fadc 	bl	30008158 <OTP_Read8>
3000aba0:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000aba4:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000aba8:	2af0      	cmp	r2, #240	; 0xf0
3000abaa:	d150      	bne.n	3000ac4e <SWR_Calib_AUD+0xfe>
3000abac:	f003 030f 	and.w	r3, r3, #15
3000abb0:	2b0f      	cmp	r3, #15
3000abb2:	d048      	beq.n	3000ac46 <SWR_Calib_AUD+0xf6>
3000abb4:	2302      	movs	r3, #2
3000abb6:	4c31      	ldr	r4, [pc, #196]	; (3000ac7c <SWR_Calib_AUD+0x12c>)
3000abb8:	f10d 0106 	add.w	r1, sp, #6
3000abbc:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000abc0:	7023      	strb	r3, [r4, #0]
3000abc2:	f7fd fac9 	bl	30008158 <OTP_Read8>
3000abc6:	7822      	ldrb	r2, [r4, #0]
3000abc8:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000abcc:	2a03      	cmp	r2, #3
3000abce:	d047      	beq.n	3000ac60 <SWR_Calib_AUD+0x110>
3000abd0:	2a02      	cmp	r2, #2
3000abd2:	bf0c      	ite	eq
3000abd4:	f003 030f 	andeq.w	r3, r3, #15
3000abd8:	2300      	movne	r3, #0
3000abda:	4926      	ldr	r1, [pc, #152]	; (3000ac74 <SWR_Calib_AUD+0x124>)
3000abdc:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000abe0:	f022 020f 	bic.w	r2, r2, #15
3000abe4:	4313      	orrs	r3, r2
3000abe6:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000abea:	2300      	movs	r3, #0
3000abec:	f10d 0107 	add.w	r1, sp, #7
3000abf0:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000abf4:	f88d 3007 	strb.w	r3, [sp, #7]
3000abf8:	f7fd faae 	bl	30008158 <OTP_Read8>
3000abfc:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ac00:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000ac04:	2af0      	cmp	r2, #240	; 0xf0
3000ac06:	d124      	bne.n	3000ac52 <SWR_Calib_AUD+0x102>
3000ac08:	f003 030f 	and.w	r3, r3, #15
3000ac0c:	2b0f      	cmp	r3, #15
3000ac0e:	d022      	beq.n	3000ac56 <SWR_Calib_AUD+0x106>
3000ac10:	2302      	movs	r3, #2
3000ac12:	4c1b      	ldr	r4, [pc, #108]	; (3000ac80 <SWR_Calib_AUD+0x130>)
3000ac14:	f10d 0107 	add.w	r1, sp, #7
3000ac18:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000ac1c:	7023      	strb	r3, [r4, #0]
3000ac1e:	f7fd fa9b 	bl	30008158 <OTP_Read8>
3000ac22:	7822      	ldrb	r2, [r4, #0]
3000ac24:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ac28:	2a03      	cmp	r2, #3
3000ac2a:	d01b      	beq.n	3000ac64 <SWR_Calib_AUD+0x114>
3000ac2c:	2a02      	cmp	r2, #2
3000ac2e:	d01c      	beq.n	3000ac6a <SWR_Calib_AUD+0x11a>
3000ac30:	2200      	movs	r2, #0
3000ac32:	4910      	ldr	r1, [pc, #64]	; (3000ac74 <SWR_Calib_AUD+0x124>)
3000ac34:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000ac38:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ac3c:	4313      	orrs	r3, r2
3000ac3e:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000ac42:	b002      	add	sp, #8
3000ac44:	bd10      	pop	{r4, pc}
3000ac46:	4b0d      	ldr	r3, [pc, #52]	; (3000ac7c <SWR_Calib_AUD+0x12c>)
3000ac48:	2201      	movs	r2, #1
3000ac4a:	701a      	strb	r2, [r3, #0]
3000ac4c:	e7cd      	b.n	3000abea <SWR_Calib_AUD+0x9a>
3000ac4e:	2303      	movs	r3, #3
3000ac50:	e7b1      	b.n	3000abb6 <SWR_Calib_AUD+0x66>
3000ac52:	2303      	movs	r3, #3
3000ac54:	e7dd      	b.n	3000ac12 <SWR_Calib_AUD+0xc2>
3000ac56:	4b0a      	ldr	r3, [pc, #40]	; (3000ac80 <SWR_Calib_AUD+0x130>)
3000ac58:	2201      	movs	r2, #1
3000ac5a:	701a      	strb	r2, [r3, #0]
3000ac5c:	b002      	add	sp, #8
3000ac5e:	bd10      	pop	{r4, pc}
3000ac60:	091b      	lsrs	r3, r3, #4
3000ac62:	e7ba      	b.n	3000abda <SWR_Calib_AUD+0x8a>
3000ac64:	091b      	lsrs	r3, r3, #4
3000ac66:	059a      	lsls	r2, r3, #22
3000ac68:	e7e3      	b.n	3000ac32 <SWR_Calib_AUD+0xe2>
3000ac6a:	f003 030f 	and.w	r3, r3, #15
3000ac6e:	059a      	lsls	r2, r3, #22
3000ac70:	e7df      	b.n	3000ac32 <SWR_Calib_AUD+0xe2>
3000ac72:	bf00      	nop
3000ac74:	42008800 	.word	0x42008800
3000ac78:	fffffccc 	.word	0xfffffccc
3000ac7c:	3000ed89 	.word	0x3000ed89
3000ac80:	3000ed88 	.word	0x3000ed88

3000ac84 <SWR_MEM>:
3000ac84:	2801      	cmp	r0, #1
3000ac86:	d00e      	beq.n	3000aca6 <SWR_MEM+0x22>
3000ac88:	4b31      	ldr	r3, [pc, #196]	; (3000ad50 <SWR_MEM+0xcc>)
3000ac8a:	2000      	movs	r0, #0
3000ac8c:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000ac90:	f022 0201 	bic.w	r2, r2, #1
3000ac94:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000ac98:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000ac9c:	f022 0202 	bic.w	r2, r2, #2
3000aca0:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000aca4:	4770      	bx	lr
3000aca6:	b510      	push	{r4, lr}
3000aca8:	4c29      	ldr	r4, [pc, #164]	; (3000ad50 <SWR_MEM+0xcc>)
3000acaa:	4a2a      	ldr	r2, [pc, #168]	; (3000ad54 <SWR_MEM+0xd0>)
3000acac:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000acb0:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000acb4:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000acb8:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000acbc:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000acc0:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000acc4:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000acc8:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000accc:	f023 0303 	bic.w	r3, r3, #3
3000acd0:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000acd4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000acd8:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000acdc:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ace0:	4790      	blx	r2
3000ace2:	2803      	cmp	r0, #3
3000ace4:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000ace8:	d026      	beq.n	3000ad38 <SWR_MEM+0xb4>
3000acea:	f043 0303 	orr.w	r3, r3, #3
3000acee:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000acf2:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000acf6:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000acfa:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000acfe:	4c14      	ldr	r4, [pc, #80]	; (3000ad50 <SWR_MEM+0xcc>)
3000ad00:	4b15      	ldr	r3, [pc, #84]	; (3000ad58 <SWR_MEM+0xd4>)
3000ad02:	4798      	blx	r3
3000ad04:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000ad08:	4a14      	ldr	r2, [pc, #80]	; (3000ad5c <SWR_MEM+0xd8>)
3000ad0a:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000ad0e:	f043 0302 	orr.w	r3, r3, #2
3000ad12:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000ad16:	4790      	blx	r2
3000ad18:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ad1c:	4b10      	ldr	r3, [pc, #64]	; (3000ad60 <SWR_MEM+0xdc>)
3000ad1e:	f042 0201 	orr.w	r2, r2, #1
3000ad22:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000ad26:	e001      	b.n	3000ad2c <SWR_MEM+0xa8>
3000ad28:	3b01      	subs	r3, #1
3000ad2a:	d00e      	beq.n	3000ad4a <SWR_MEM+0xc6>
3000ad2c:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ad30:	03d2      	lsls	r2, r2, #15
3000ad32:	d5f9      	bpl.n	3000ad28 <SWR_MEM+0xa4>
3000ad34:	2000      	movs	r0, #0
3000ad36:	bd10      	pop	{r4, pc}
3000ad38:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ad3c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ad40:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000ad44:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ad48:	e7d9      	b.n	3000acfe <SWR_MEM+0x7a>
3000ad4a:	2001      	movs	r0, #1
3000ad4c:	bd10      	pop	{r4, pc}
3000ad4e:	bf00      	nop
3000ad50:	42008800 	.word	0x42008800
3000ad54:	30007461 	.word	0x30007461
3000ad58:	3000a9f5 	.word	0x3000a9f5
3000ad5c:	00009b2d 	.word	0x00009b2d
3000ad60:	000186a0 	.word	0x000186a0

3000ad64 <SWR_MEM_Manual>:
3000ad64:	4a06      	ldr	r2, [pc, #24]	; (3000ad80 <SWR_MEM_Manual+0x1c>)
3000ad66:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000ad6a:	b120      	cbz	r0, 3000ad76 <SWR_MEM_Manual+0x12>
3000ad6c:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000ad70:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000ad74:	4770      	bx	lr
3000ad76:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000ad7a:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000ad7e:	4770      	bx	lr
3000ad80:	42008800 	.word	0x42008800

3000ad84 <SWR_MEM_Mode_Set>:
3000ad84:	2801      	cmp	r0, #1
3000ad86:	b538      	push	{r3, r4, r5, lr}
3000ad88:	d807      	bhi.n	3000ad9a <SWR_MEM_Mode_Set+0x16>
3000ad8a:	d10a      	bne.n	3000ada2 <SWR_MEM_Mode_Set+0x1e>
3000ad8c:	4c22      	ldr	r4, [pc, #136]	; (3000ae18 <SWR_MEM_Mode_Set+0x94>)
3000ad8e:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000ad92:	035b      	lsls	r3, r3, #13
3000ad94:	d51b      	bpl.n	3000adce <SWR_MEM_Mode_Set+0x4a>
3000ad96:	2000      	movs	r0, #0
3000ad98:	bd38      	pop	{r3, r4, r5, pc}
3000ad9a:	217e      	movs	r1, #126	; 0x7e
3000ad9c:	481f      	ldr	r0, [pc, #124]	; (3000ae1c <SWR_MEM_Mode_Set+0x98>)
3000ad9e:	f000 fe1f 	bl	3000b9e0 <__io_assert_failed_veneer>
3000ada2:	491d      	ldr	r1, [pc, #116]	; (3000ae18 <SWR_MEM_Mode_Set+0x94>)
3000ada4:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000ada8:	0358      	lsls	r0, r3, #13
3000adaa:	d5f4      	bpl.n	3000ad96 <SWR_MEM_Mode_Set+0x12>
3000adac:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000adb0:	f242 7310 	movw	r3, #10000	; 0x2710
3000adb4:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000adb8:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000adbc:	e001      	b.n	3000adc2 <SWR_MEM_Mode_Set+0x3e>
3000adbe:	3b01      	subs	r3, #1
3000adc0:	d027      	beq.n	3000ae12 <SWR_MEM_Mode_Set+0x8e>
3000adc2:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000adc6:	0352      	lsls	r2, r2, #13
3000adc8:	d4f9      	bmi.n	3000adbe <SWR_MEM_Mode_Set+0x3a>
3000adca:	2000      	movs	r0, #0
3000adcc:	e7e4      	b.n	3000ad98 <SWR_MEM_Mode_Set+0x14>
3000adce:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000add2:	2064      	movs	r0, #100	; 0x64
3000add4:	4d12      	ldr	r5, [pc, #72]	; (3000ae20 <SWR_MEM_Mode_Set+0x9c>)
3000add6:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000adda:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000adde:	47a8      	blx	r5
3000ade0:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ade4:	2064      	movs	r0, #100	; 0x64
3000ade6:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000adea:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000adee:	47a8      	blx	r5
3000adf0:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000adf4:	f242 7310 	movw	r3, #10000	; 0x2710
3000adf8:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000adfc:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000ae00:	e001      	b.n	3000ae06 <SWR_MEM_Mode_Set+0x82>
3000ae02:	3b01      	subs	r3, #1
3000ae04:	d005      	beq.n	3000ae12 <SWR_MEM_Mode_Set+0x8e>
3000ae06:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ae0a:	0355      	lsls	r5, r2, #13
3000ae0c:	d5f9      	bpl.n	3000ae02 <SWR_MEM_Mode_Set+0x7e>
3000ae0e:	2000      	movs	r0, #0
3000ae10:	e7c2      	b.n	3000ad98 <SWR_MEM_Mode_Set+0x14>
3000ae12:	2001      	movs	r0, #1
3000ae14:	bd38      	pop	{r3, r4, r5, pc}
3000ae16:	bf00      	nop
3000ae18:	42008800 	.word	0x42008800
3000ae1c:	3000d168 	.word	0x3000d168
3000ae20:	00009b2d 	.word	0x00009b2d

3000ae24 <SWR_AUDIO>:
3000ae24:	2801      	cmp	r0, #1
3000ae26:	d00e      	beq.n	3000ae46 <SWR_AUDIO+0x22>
3000ae28:	4b1e      	ldr	r3, [pc, #120]	; (3000aea4 <SWR_AUDIO+0x80>)
3000ae2a:	2000      	movs	r0, #0
3000ae2c:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ae30:	f022 0201 	bic.w	r2, r2, #1
3000ae34:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000ae38:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ae3c:	f022 0202 	bic.w	r2, r2, #2
3000ae40:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000ae44:	4770      	bx	lr
3000ae46:	b510      	push	{r4, lr}
3000ae48:	4c16      	ldr	r4, [pc, #88]	; (3000aea4 <SWR_AUDIO+0x80>)
3000ae4a:	4b17      	ldr	r3, [pc, #92]	; (3000aea8 <SWR_AUDIO+0x84>)
3000ae4c:	4798      	blx	r3
3000ae4e:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000ae52:	4a16      	ldr	r2, [pc, #88]	; (3000aeac <SWR_AUDIO+0x88>)
3000ae54:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000ae58:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000ae5c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000ae60:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000ae64:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000ae68:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000ae6c:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000ae70:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000ae74:	f043 0302 	orr.w	r3, r3, #2
3000ae78:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000ae7c:	4790      	blx	r2
3000ae7e:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000ae82:	4b0b      	ldr	r3, [pc, #44]	; (3000aeb0 <SWR_AUDIO+0x8c>)
3000ae84:	f042 0201 	orr.w	r2, r2, #1
3000ae88:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000ae8c:	e001      	b.n	3000ae92 <SWR_AUDIO+0x6e>
3000ae8e:	3b01      	subs	r3, #1
3000ae90:	d005      	beq.n	3000ae9e <SWR_AUDIO+0x7a>
3000ae92:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000ae96:	03d2      	lsls	r2, r2, #15
3000ae98:	d5f9      	bpl.n	3000ae8e <SWR_AUDIO+0x6a>
3000ae9a:	2000      	movs	r0, #0
3000ae9c:	bd10      	pop	{r4, pc}
3000ae9e:	2001      	movs	r0, #1
3000aea0:	bd10      	pop	{r4, pc}
3000aea2:	bf00      	nop
3000aea4:	42008800 	.word	0x42008800
3000aea8:	3000ab51 	.word	0x3000ab51
3000aeac:	00009b2d 	.word	0x00009b2d
3000aeb0:	000186a0 	.word	0x000186a0

3000aeb4 <SWR_AUDIO_Manual>:
3000aeb4:	4a06      	ldr	r2, [pc, #24]	; (3000aed0 <SWR_AUDIO_Manual+0x1c>)
3000aeb6:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000aeba:	b120      	cbz	r0, 3000aec6 <SWR_AUDIO_Manual+0x12>
3000aebc:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000aec0:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000aec4:	4770      	bx	lr
3000aec6:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000aeca:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000aece:	4770      	bx	lr
3000aed0:	42008800 	.word	0x42008800

3000aed4 <SWR_BST_MODE_Set>:
3000aed4:	4a06      	ldr	r2, [pc, #24]	; (3000aef0 <SWR_BST_MODE_Set+0x1c>)
3000aed6:	6813      	ldr	r3, [r2, #0]
3000aed8:	b118      	cbz	r0, 3000aee2 <SWR_BST_MODE_Set+0xe>
3000aeda:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000aede:	6013      	str	r3, [r2, #0]
3000aee0:	4770      	bx	lr
3000aee2:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000aee6:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000aeea:	6013      	str	r3, [r2, #0]
3000aeec:	4770      	bx	lr
3000aeee:	bf00      	nop
3000aef0:	42008100 	.word	0x42008100

3000aef4 <crash_dump_memory>:
3000aef4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000aef8:	460e      	mov	r6, r1
3000aefa:	4680      	mov	r8, r0
3000aefc:	4b21      	ldr	r3, [pc, #132]	; (3000af84 <crash_dump_memory+0x90>)
3000aefe:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000af02:	4a21      	ldr	r2, [pc, #132]	; (3000af88 <crash_dump_memory+0x94>)
3000af04:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000af08:	4644      	mov	r4, r8
3000af0a:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000af0e:	4b1f      	ldr	r3, [pc, #124]	; (3000af8c <crash_dump_memory+0x98>)
3000af10:	481f      	ldr	r0, [pc, #124]	; (3000af90 <crash_dump_memory+0x9c>)
3000af12:	bf88      	it	hi
3000af14:	2680      	movhi	r6, #128	; 0x80
3000af16:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000af1a:	4f1e      	ldr	r7, [pc, #120]	; (3000af94 <crash_dump_memory+0xa0>)
3000af1c:	bf14      	ite	ne
3000af1e:	4611      	movne	r1, r2
3000af20:	4619      	moveq	r1, r3
3000af22:	f000 fdfd 	bl	3000bb20 <__DiagPrintf_veneer>
3000af26:	4632      	mov	r2, r6
3000af28:	4641      	mov	r1, r8
3000af2a:	481b      	ldr	r0, [pc, #108]	; (3000af98 <crash_dump_memory+0xa4>)
3000af2c:	f000 fdf8 	bl	3000bb20 <__DiagPrintf_veneer>
3000af30:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000afa0 <crash_dump_memory+0xac>
3000af34:	2300      	movs	r3, #0
3000af36:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000afa4 <crash_dump_memory+0xb0>
3000af3a:	e007      	b.n	3000af4c <crash_dump_memory+0x58>
3000af3c:	6821      	ldr	r1, [r4, #0]
3000af3e:	4638      	mov	r0, r7
3000af40:	f000 fdee 	bl	3000bb20 <__DiagPrintf_veneer>
3000af44:	42ae      	cmp	r6, r5
3000af46:	d00f      	beq.n	3000af68 <crash_dump_memory+0x74>
3000af48:	3404      	adds	r4, #4
3000af4a:	462b      	mov	r3, r5
3000af4c:	1c5d      	adds	r5, r3, #1
3000af4e:	b183      	cbz	r3, 3000af72 <crash_dump_memory+0x7e>
3000af50:	079b      	lsls	r3, r3, #30
3000af52:	d1f3      	bne.n	3000af3c <crash_dump_memory+0x48>
3000af54:	4621      	mov	r1, r4
3000af56:	4648      	mov	r0, r9
3000af58:	f000 fde2 	bl	3000bb20 <__DiagPrintf_veneer>
3000af5c:	6821      	ldr	r1, [r4, #0]
3000af5e:	4638      	mov	r0, r7
3000af60:	f000 fdde 	bl	3000bb20 <__DiagPrintf_veneer>
3000af64:	42ae      	cmp	r6, r5
3000af66:	d1ef      	bne.n	3000af48 <crash_dump_memory+0x54>
3000af68:	480c      	ldr	r0, [pc, #48]	; (3000af9c <crash_dump_memory+0xa8>)
3000af6a:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000af6e:	f000 bdd7 	b.w	3000bb20 <__DiagPrintf_veneer>
3000af72:	4641      	mov	r1, r8
3000af74:	4650      	mov	r0, sl
3000af76:	f000 fdd3 	bl	3000bb20 <__DiagPrintf_veneer>
3000af7a:	6821      	ldr	r1, [r4, #0]
3000af7c:	4638      	mov	r0, r7
3000af7e:	f000 fdcf 	bl	3000bb20 <__DiagPrintf_veneer>
3000af82:	e7e1      	b.n	3000af48 <crash_dump_memory+0x54>
3000af84:	42008000 	.word	0x42008000
3000af88:	3000d17c 	.word	0x3000d17c
3000af8c:	3000d180 	.word	0x3000d180
3000af90:	3000d184 	.word	0x3000d184
3000af94:	3000d1ec 	.word	0x3000d1ec
3000af98:	3000d1a8 	.word	0x3000d1a8
3000af9c:	3000c6bc 	.word	0x3000c6bc
3000afa0:	3000d1f4 	.word	0x3000d1f4
3000afa4:	3000d1e4 	.word	0x3000d1e4

3000afa8 <crash_dump>:
3000afa8:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000afac:	4c4f      	ldr	r4, [pc, #316]	; (3000b0ec <crash_dump+0x144>)
3000afae:	4615      	mov	r5, r2
3000afb0:	4688      	mov	r8, r1
3000afb2:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000afb6:	484e      	ldr	r0, [pc, #312]	; (3000b0f0 <crash_dump+0x148>)
3000afb8:	462f      	mov	r7, r5
3000afba:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000b11c <crash_dump+0x174>
3000afbe:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000afc2:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000b128 <crash_dump+0x180>
3000afc6:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000afca:	e9c4 2100 	strd	r2, r1, [r4]
3000afce:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000afd0:	60a2      	str	r2, [r4, #8]
3000afd2:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000afd4:	60e2      	str	r2, [r4, #12]
3000afd6:	686a      	ldr	r2, [r5, #4]
3000afd8:	6122      	str	r2, [r4, #16]
3000afda:	68aa      	ldr	r2, [r5, #8]
3000afdc:	6162      	str	r2, [r4, #20]
3000afde:	68ea      	ldr	r2, [r5, #12]
3000afe0:	61a2      	str	r2, [r4, #24]
3000afe2:	692a      	ldr	r2, [r5, #16]
3000afe4:	61e2      	str	r2, [r4, #28]
3000afe6:	696a      	ldr	r2, [r5, #20]
3000afe8:	6222      	str	r2, [r4, #32]
3000afea:	69aa      	ldr	r2, [r5, #24]
3000afec:	6262      	str	r2, [r4, #36]	; 0x24
3000afee:	69ea      	ldr	r2, [r5, #28]
3000aff0:	62a2      	str	r2, [r4, #40]	; 0x28
3000aff2:	6a2a      	ldr	r2, [r5, #32]
3000aff4:	62e2      	str	r2, [r4, #44]	; 0x2c
3000aff6:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000aff8:	6322      	str	r2, [r4, #48]	; 0x30
3000affa:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000affc:	6362      	str	r2, [r4, #52]	; 0x34
3000affe:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000b000:	63a2      	str	r2, [r4, #56]	; 0x38
3000b002:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000b004:	63e2      	str	r2, [r4, #60]	; 0x3c
3000b006:	682a      	ldr	r2, [r5, #0]
3000b008:	6422      	str	r2, [r4, #64]	; 0x40
3000b00a:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000b00e:	6462      	str	r2, [r4, #68]	; 0x44
3000b010:	4a38      	ldr	r2, [pc, #224]	; (3000b0f4 <crash_dump+0x14c>)
3000b012:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000b016:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000b01a:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000b01e:	64e1      	str	r1, [r4, #76]	; 0x4c
3000b020:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000b024:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000b028:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000b02c:	6561      	str	r1, [r4, #84]	; 0x54
3000b02e:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000b030:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000b034:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000b038:	65e1      	str	r1, [r4, #92]	; 0x5c
3000b03a:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000b03c:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000b03e:	b289      	uxth	r1, r1
3000b040:	b292      	uxth	r2, r2
3000b042:	6621      	str	r1, [r4, #96]	; 0x60
3000b044:	6662      	str	r2, [r4, #100]	; 0x64
3000b046:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000b04a:	66a2      	str	r2, [r4, #104]	; 0x68
3000b04c:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000b050:	66e3      	str	r3, [r4, #108]	; 0x6c
3000b052:	f000 fd65 	bl	3000bb20 <__DiagPrintf_veneer>
3000b056:	4620      	mov	r0, r4
3000b058:	462c      	mov	r4, r5
3000b05a:	f000 f867 	bl	3000b12c <fault_diagnosis>
3000b05e:	4826      	ldr	r0, [pc, #152]	; (3000b0f8 <crash_dump+0x150>)
3000b060:	f000 fd5e 	bl	3000bb20 <__DiagPrintf_veneer>
3000b064:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000b066:	4825      	ldr	r0, [pc, #148]	; (3000b0fc <crash_dump+0x154>)
3000b068:	f000 fd5a 	bl	3000bb20 <__DiagPrintf_veneer>
3000b06c:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000b06e:	4824      	ldr	r0, [pc, #144]	; (3000b100 <crash_dump+0x158>)
3000b070:	f000 fd56 	bl	3000bb20 <__DiagPrintf_veneer>
3000b074:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000b076:	4823      	ldr	r0, [pc, #140]	; (3000b104 <crash_dump+0x15c>)
3000b078:	f000 fd52 	bl	3000bb20 <__DiagPrintf_veneer>
3000b07c:	f857 1b20 	ldr.w	r1, [r7], #32
3000b080:	4821      	ldr	r0, [pc, #132]	; (3000b108 <crash_dump+0x160>)
3000b082:	463e      	mov	r6, r7
3000b084:	f000 fd4c 	bl	3000bb20 <__DiagPrintf_veneer>
3000b088:	4920      	ldr	r1, [pc, #128]	; (3000b10c <crash_dump+0x164>)
3000b08a:	4658      	mov	r0, fp
3000b08c:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000b090:	f000 fd46 	bl	3000bb20 <__DiagPrintf_veneer>
3000b094:	454e      	cmp	r6, r9
3000b096:	d008      	beq.n	3000b0aa <crash_dump+0x102>
3000b098:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000b09c:	4658      	mov	r0, fp
3000b09e:	f85a 1b04 	ldr.w	r1, [sl], #4
3000b0a2:	f000 fd3d 	bl	3000bb20 <__DiagPrintf_veneer>
3000b0a6:	454e      	cmp	r6, r9
3000b0a8:	d1f6      	bne.n	3000b098 <crash_dump+0xf0>
3000b0aa:	4e19      	ldr	r6, [pc, #100]	; (3000b110 <crash_dump+0x168>)
3000b0ac:	4919      	ldr	r1, [pc, #100]	; (3000b114 <crash_dump+0x16c>)
3000b0ae:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000b11c <crash_dump+0x174>
3000b0b2:	e001      	b.n	3000b0b8 <crash_dump+0x110>
3000b0b4:	f856 1b04 	ldr.w	r1, [r6], #4
3000b0b8:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000b0bc:	4648      	mov	r0, r9
3000b0be:	f000 fd2f 	bl	3000bb20 <__DiagPrintf_veneer>
3000b0c2:	42bc      	cmp	r4, r7
3000b0c4:	d1f6      	bne.n	3000b0b4 <crash_dump+0x10c>
3000b0c6:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000b0c8:	4913      	ldr	r1, [pc, #76]	; (3000b118 <crash_dump+0x170>)
3000b0ca:	4814      	ldr	r0, [pc, #80]	; (3000b11c <crash_dump+0x174>)
3000b0cc:	f000 fd28 	bl	3000bb20 <__DiagPrintf_veneer>
3000b0d0:	2180      	movs	r1, #128	; 0x80
3000b0d2:	4640      	mov	r0, r8
3000b0d4:	f7ff ff0e 	bl	3000aef4 <crash_dump_memory>
3000b0d8:	4811      	ldr	r0, [pc, #68]	; (3000b120 <crash_dump+0x178>)
3000b0da:	f000 fd21 	bl	3000bb20 <__DiagPrintf_veneer>
3000b0de:	4811      	ldr	r0, [pc, #68]	; (3000b124 <crash_dump+0x17c>)
3000b0e0:	f000 fd1e 	bl	3000bb20 <__DiagPrintf_veneer>
3000b0e4:	2000      	movs	r0, #0
3000b0e6:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b0ea:	bf00      	nop
3000b0ec:	3000f368 	.word	0x3000f368
3000b0f0:	3000d208 	.word	0x3000d208
3000b0f4:	e000ed00 	.word	0xe000ed00
3000b0f8:	3000d22c 	.word	0x3000d22c
3000b0fc:	3000d254 	.word	0x3000d254
3000b100:	3000d264 	.word	0x3000d264
3000b104:	3000d274 	.word	0x3000d274
3000b108:	3000d284 	.word	0x3000d284
3000b10c:	3000d200 	.word	0x3000d200
3000b110:	3000d2e4 	.word	0x3000d2e4
3000b114:	3000d204 	.word	0x3000d204
3000b118:	3000d2a4 	.word	0x3000d2a4
3000b11c:	3000d294 	.word	0x3000d294
3000b120:	3000d2a8 	.word	0x3000d2a8
3000b124:	3000d2d4 	.word	0x3000d2d4
3000b128:	3000d304 	.word	0x3000d304

3000b12c <fault_diagnosis>:
3000b12c:	b510      	push	{r4, lr}
3000b12e:	4604      	mov	r4, r0
3000b130:	4888      	ldr	r0, [pc, #544]	; (3000b354 <fault_diagnosis+0x228>)
3000b132:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000b134:	f000 fcf4 	bl	3000bb20 <__DiagPrintf_veneer>
3000b138:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000b13c:	079b      	lsls	r3, r3, #30
3000b13e:	f100 808a 	bmi.w	3000b256 <fault_diagnosis+0x12a>
3000b142:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b144:	b1e1      	cbz	r1, 3000b180 <fault_diagnosis+0x54>
3000b146:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b14a:	07d8      	lsls	r0, r3, #31
3000b14c:	f100 8094 	bmi.w	3000b278 <fault_diagnosis+0x14c>
3000b150:	079a      	lsls	r2, r3, #30
3000b152:	f100 809a 	bmi.w	3000b28a <fault_diagnosis+0x15e>
3000b156:	0758      	lsls	r0, r3, #29
3000b158:	f100 80a1 	bmi.w	3000b29e <fault_diagnosis+0x172>
3000b15c:	0719      	lsls	r1, r3, #28
3000b15e:	f100 80a8 	bmi.w	3000b2b2 <fault_diagnosis+0x186>
3000b162:	06da      	lsls	r2, r3, #27
3000b164:	f100 80af 	bmi.w	3000b2c6 <fault_diagnosis+0x19a>
3000b168:	0698      	lsls	r0, r3, #26
3000b16a:	f100 80b6 	bmi.w	3000b2da <fault_diagnosis+0x1ae>
3000b16e:	0619      	lsls	r1, r3, #24
3000b170:	f100 80bd 	bmi.w	3000b2ee <fault_diagnosis+0x1c2>
3000b174:	065a      	lsls	r2, r3, #25
3000b176:	d503      	bpl.n	3000b180 <fault_diagnosis+0x54>
3000b178:	f013 0f28 	tst.w	r3, #40	; 0x28
3000b17c:	f040 8186 	bne.w	3000b48c <fault_diagnosis+0x360>
3000b180:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b184:	b199      	cbz	r1, 3000b1ae <fault_diagnosis+0x82>
3000b186:	07cb      	lsls	r3, r1, #31
3000b188:	f100 80b9 	bmi.w	3000b2fe <fault_diagnosis+0x1d2>
3000b18c:	0788      	lsls	r0, r1, #30
3000b18e:	f100 80bf 	bmi.w	3000b310 <fault_diagnosis+0x1e4>
3000b192:	070a      	lsls	r2, r1, #28
3000b194:	f100 80c5 	bmi.w	3000b322 <fault_diagnosis+0x1f6>
3000b198:	06cb      	lsls	r3, r1, #27
3000b19a:	f100 80cb 	bmi.w	3000b334 <fault_diagnosis+0x208>
3000b19e:	0688      	lsls	r0, r1, #26
3000b1a0:	f100 80d1 	bmi.w	3000b346 <fault_diagnosis+0x21a>
3000b1a4:	060a      	lsls	r2, r1, #24
3000b1a6:	d502      	bpl.n	3000b1ae <fault_diagnosis+0x82>
3000b1a8:	078b      	lsls	r3, r1, #30
3000b1aa:	f040 8194 	bne.w	3000b4d6 <fault_diagnosis+0x3aa>
3000b1ae:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b1b2:	b1b1      	cbz	r1, 3000b1e2 <fault_diagnosis+0xb6>
3000b1b4:	07c8      	lsls	r0, r1, #31
3000b1b6:	f100 80f7 	bmi.w	3000b3a8 <fault_diagnosis+0x27c>
3000b1ba:	078a      	lsls	r2, r1, #30
3000b1bc:	f100 80fd 	bmi.w	3000b3ba <fault_diagnosis+0x28e>
3000b1c0:	074b      	lsls	r3, r1, #29
3000b1c2:	f100 8103 	bmi.w	3000b3cc <fault_diagnosis+0x2a0>
3000b1c6:	0708      	lsls	r0, r1, #28
3000b1c8:	f100 8109 	bmi.w	3000b3de <fault_diagnosis+0x2b2>
3000b1cc:	06ca      	lsls	r2, r1, #27
3000b1ce:	f100 810f 	bmi.w	3000b3f0 <fault_diagnosis+0x2c4>
3000b1d2:	068b      	lsls	r3, r1, #26
3000b1d4:	f100 8115 	bmi.w	3000b402 <fault_diagnosis+0x2d6>
3000b1d8:	0608      	lsls	r0, r1, #24
3000b1da:	d502      	bpl.n	3000b1e2 <fault_diagnosis+0xb6>
3000b1dc:	0789      	lsls	r1, r1, #30
3000b1de:	f100 8180 	bmi.w	3000b4e2 <fault_diagnosis+0x3b6>
3000b1e2:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b1e6:	b1b9      	cbz	r1, 3000b218 <fault_diagnosis+0xec>
3000b1e8:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b1ec:	07da      	lsls	r2, r3, #31
3000b1ee:	f100 810f 	bmi.w	3000b410 <fault_diagnosis+0x2e4>
3000b1f2:	0798      	lsls	r0, r3, #30
3000b1f4:	f100 8115 	bmi.w	3000b422 <fault_diagnosis+0x2f6>
3000b1f8:	0759      	lsls	r1, r3, #29
3000b1fa:	f100 811d 	bmi.w	3000b438 <fault_diagnosis+0x30c>
3000b1fe:	071a      	lsls	r2, r3, #28
3000b200:	f100 8125 	bmi.w	3000b44e <fault_diagnosis+0x322>
3000b204:	06db      	lsls	r3, r3, #27
3000b206:	f100 812d 	bmi.w	3000b464 <fault_diagnosis+0x338>
3000b20a:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b20e:	07d8      	lsls	r0, r3, #31
3000b210:	f100 8133 	bmi.w	3000b47a <fault_diagnosis+0x34e>
3000b214:	0799      	lsls	r1, r3, #30
3000b216:	d424      	bmi.n	3000b262 <fault_diagnosis+0x136>
3000b218:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000b21c:	2b00      	cmp	r3, #0
3000b21e:	db00      	blt.n	3000b222 <fault_diagnosis+0xf6>
3000b220:	bd10      	pop	{r4, pc}
3000b222:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b224:	2900      	cmp	r1, #0
3000b226:	d0fb      	beq.n	3000b220 <fault_diagnosis+0xf4>
3000b228:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b22c:	07da      	lsls	r2, r3, #31
3000b22e:	f100 8143 	bmi.w	3000b4b8 <fault_diagnosis+0x38c>
3000b232:	0798      	lsls	r0, r3, #30
3000b234:	f100 8138 	bmi.w	3000b4a8 <fault_diagnosis+0x37c>
3000b238:	0759      	lsls	r1, r3, #29
3000b23a:	f100 812d 	bmi.w	3000b498 <fault_diagnosis+0x36c>
3000b23e:	071a      	lsls	r2, r3, #28
3000b240:	f100 8141 	bmi.w	3000b4c6 <fault_diagnosis+0x39a>
3000b244:	06db      	lsls	r3, r3, #27
3000b246:	d5eb      	bpl.n	3000b220 <fault_diagnosis+0xf4>
3000b248:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b24a:	4a43      	ldr	r2, [pc, #268]	; (3000b358 <fault_diagnosis+0x22c>)
3000b24c:	4843      	ldr	r0, [pc, #268]	; (3000b35c <fault_diagnosis+0x230>)
3000b24e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000b252:	f000 bc65 	b.w	3000bb20 <__DiagPrintf_veneer>
3000b256:	4a42      	ldr	r2, [pc, #264]	; (3000b360 <fault_diagnosis+0x234>)
3000b258:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000b25a:	4842      	ldr	r0, [pc, #264]	; (3000b364 <fault_diagnosis+0x238>)
3000b25c:	f000 fc60 	bl	3000bb20 <__DiagPrintf_veneer>
3000b260:	e76f      	b.n	3000b142 <fault_diagnosis+0x16>
3000b262:	4a41      	ldr	r2, [pc, #260]	; (3000b368 <fault_diagnosis+0x23c>)
3000b264:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b268:	4840      	ldr	r0, [pc, #256]	; (3000b36c <fault_diagnosis+0x240>)
3000b26a:	f000 fc59 	bl	3000bb20 <__DiagPrintf_veneer>
3000b26e:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000b272:	2b00      	cmp	r3, #0
3000b274:	dad4      	bge.n	3000b220 <fault_diagnosis+0xf4>
3000b276:	e7d4      	b.n	3000b222 <fault_diagnosis+0xf6>
3000b278:	4a3d      	ldr	r2, [pc, #244]	; (3000b370 <fault_diagnosis+0x244>)
3000b27a:	483e      	ldr	r0, [pc, #248]	; (3000b374 <fault_diagnosis+0x248>)
3000b27c:	f000 fc50 	bl	3000bb20 <__DiagPrintf_veneer>
3000b280:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b284:	079a      	lsls	r2, r3, #30
3000b286:	f57f af66 	bpl.w	3000b156 <fault_diagnosis+0x2a>
3000b28a:	4a3b      	ldr	r2, [pc, #236]	; (3000b378 <fault_diagnosis+0x24c>)
3000b28c:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b28e:	4839      	ldr	r0, [pc, #228]	; (3000b374 <fault_diagnosis+0x248>)
3000b290:	f000 fc46 	bl	3000bb20 <__DiagPrintf_veneer>
3000b294:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b298:	0758      	lsls	r0, r3, #29
3000b29a:	f57f af5f 	bpl.w	3000b15c <fault_diagnosis+0x30>
3000b29e:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b2a0:	4a36      	ldr	r2, [pc, #216]	; (3000b37c <fault_diagnosis+0x250>)
3000b2a2:	4834      	ldr	r0, [pc, #208]	; (3000b374 <fault_diagnosis+0x248>)
3000b2a4:	f000 fc3c 	bl	3000bb20 <__DiagPrintf_veneer>
3000b2a8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b2ac:	0719      	lsls	r1, r3, #28
3000b2ae:	f57f af58 	bpl.w	3000b162 <fault_diagnosis+0x36>
3000b2b2:	4a33      	ldr	r2, [pc, #204]	; (3000b380 <fault_diagnosis+0x254>)
3000b2b4:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b2b6:	482f      	ldr	r0, [pc, #188]	; (3000b374 <fault_diagnosis+0x248>)
3000b2b8:	f000 fc32 	bl	3000bb20 <__DiagPrintf_veneer>
3000b2bc:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b2c0:	06da      	lsls	r2, r3, #27
3000b2c2:	f57f af51 	bpl.w	3000b168 <fault_diagnosis+0x3c>
3000b2c6:	4a2f      	ldr	r2, [pc, #188]	; (3000b384 <fault_diagnosis+0x258>)
3000b2c8:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b2ca:	482a      	ldr	r0, [pc, #168]	; (3000b374 <fault_diagnosis+0x248>)
3000b2cc:	f000 fc28 	bl	3000bb20 <__DiagPrintf_veneer>
3000b2d0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b2d4:	0698      	lsls	r0, r3, #26
3000b2d6:	f57f af4a 	bpl.w	3000b16e <fault_diagnosis+0x42>
3000b2da:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b2dc:	4a2a      	ldr	r2, [pc, #168]	; (3000b388 <fault_diagnosis+0x25c>)
3000b2de:	4825      	ldr	r0, [pc, #148]	; (3000b374 <fault_diagnosis+0x248>)
3000b2e0:	f000 fc1e 	bl	3000bb20 <__DiagPrintf_veneer>
3000b2e4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b2e8:	0619      	lsls	r1, r3, #24
3000b2ea:	f57f af43 	bpl.w	3000b174 <fault_diagnosis+0x48>
3000b2ee:	4a27      	ldr	r2, [pc, #156]	; (3000b38c <fault_diagnosis+0x260>)
3000b2f0:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000b2f2:	4820      	ldr	r0, [pc, #128]	; (3000b374 <fault_diagnosis+0x248>)
3000b2f4:	f000 fc14 	bl	3000bb20 <__DiagPrintf_veneer>
3000b2f8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000b2fc:	e73a      	b.n	3000b174 <fault_diagnosis+0x48>
3000b2fe:	4a24      	ldr	r2, [pc, #144]	; (3000b390 <fault_diagnosis+0x264>)
3000b300:	4824      	ldr	r0, [pc, #144]	; (3000b394 <fault_diagnosis+0x268>)
3000b302:	f000 fc0d 	bl	3000bb20 <__DiagPrintf_veneer>
3000b306:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b30a:	0788      	lsls	r0, r1, #30
3000b30c:	f57f af41 	bpl.w	3000b192 <fault_diagnosis+0x66>
3000b310:	4a21      	ldr	r2, [pc, #132]	; (3000b398 <fault_diagnosis+0x26c>)
3000b312:	4820      	ldr	r0, [pc, #128]	; (3000b394 <fault_diagnosis+0x268>)
3000b314:	f000 fc04 	bl	3000bb20 <__DiagPrintf_veneer>
3000b318:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b31c:	070a      	lsls	r2, r1, #28
3000b31e:	f57f af3b 	bpl.w	3000b198 <fault_diagnosis+0x6c>
3000b322:	4a1e      	ldr	r2, [pc, #120]	; (3000b39c <fault_diagnosis+0x270>)
3000b324:	481b      	ldr	r0, [pc, #108]	; (3000b394 <fault_diagnosis+0x268>)
3000b326:	f000 fbfb 	bl	3000bb20 <__DiagPrintf_veneer>
3000b32a:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b32e:	06cb      	lsls	r3, r1, #27
3000b330:	f57f af35 	bpl.w	3000b19e <fault_diagnosis+0x72>
3000b334:	4a1a      	ldr	r2, [pc, #104]	; (3000b3a0 <fault_diagnosis+0x274>)
3000b336:	4817      	ldr	r0, [pc, #92]	; (3000b394 <fault_diagnosis+0x268>)
3000b338:	f000 fbf2 	bl	3000bb20 <__DiagPrintf_veneer>
3000b33c:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b340:	0688      	lsls	r0, r1, #26
3000b342:	f57f af2f 	bpl.w	3000b1a4 <fault_diagnosis+0x78>
3000b346:	4a17      	ldr	r2, [pc, #92]	; (3000b3a4 <fault_diagnosis+0x278>)
3000b348:	4812      	ldr	r0, [pc, #72]	; (3000b394 <fault_diagnosis+0x268>)
3000b34a:	f000 fbe9 	bl	3000bb20 <__DiagPrintf_veneer>
3000b34e:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b352:	e727      	b.n	3000b1a4 <fault_diagnosis+0x78>
3000b354:	3000d314 	.word	0x3000d314
3000b358:	3000dba4 	.word	0x3000dba4
3000b35c:	3000dafc 	.word	0x3000dafc
3000b360:	3000d32c 	.word	0x3000d32c
3000b364:	3000d358 	.word	0x3000d358
3000b368:	3000da60 	.word	0x3000da60
3000b36c:	3000d8a4 	.word	0x3000d8a4
3000b370:	3000d370 	.word	0x3000d370
3000b374:	3000d3a0 	.word	0x3000d3a0
3000b378:	3000d3b8 	.word	0x3000d3b8
3000b37c:	3000d3f0 	.word	0x3000d3f0
3000b380:	3000d428 	.word	0x3000d428
3000b384:	3000d460 	.word	0x3000d460
3000b388:	3000d490 	.word	0x3000d490
3000b38c:	3000d4cc 	.word	0x3000d4cc
3000b390:	3000d53c 	.word	0x3000d53c
3000b394:	3000d580 	.word	0x3000d580
3000b398:	3000d598 	.word	0x3000d598
3000b39c:	3000d5d4 	.word	0x3000d5d4
3000b3a0:	3000d60c 	.word	0x3000d60c
3000b3a4:	3000d640 	.word	0x3000d640
3000b3a8:	4a51      	ldr	r2, [pc, #324]	; (3000b4f0 <fault_diagnosis+0x3c4>)
3000b3aa:	4852      	ldr	r0, [pc, #328]	; (3000b4f4 <fault_diagnosis+0x3c8>)
3000b3ac:	f000 fbb8 	bl	3000bb20 <__DiagPrintf_veneer>
3000b3b0:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b3b4:	078a      	lsls	r2, r1, #30
3000b3b6:	f57f af03 	bpl.w	3000b1c0 <fault_diagnosis+0x94>
3000b3ba:	4a4f      	ldr	r2, [pc, #316]	; (3000b4f8 <fault_diagnosis+0x3cc>)
3000b3bc:	484d      	ldr	r0, [pc, #308]	; (3000b4f4 <fault_diagnosis+0x3c8>)
3000b3be:	f000 fbaf 	bl	3000bb20 <__DiagPrintf_veneer>
3000b3c2:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b3c6:	074b      	lsls	r3, r1, #29
3000b3c8:	f57f aefd 	bpl.w	3000b1c6 <fault_diagnosis+0x9a>
3000b3cc:	4a4b      	ldr	r2, [pc, #300]	; (3000b4fc <fault_diagnosis+0x3d0>)
3000b3ce:	4849      	ldr	r0, [pc, #292]	; (3000b4f4 <fault_diagnosis+0x3c8>)
3000b3d0:	f000 fba6 	bl	3000bb20 <__DiagPrintf_veneer>
3000b3d4:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b3d8:	0708      	lsls	r0, r1, #28
3000b3da:	f57f aef7 	bpl.w	3000b1cc <fault_diagnosis+0xa0>
3000b3de:	4a48      	ldr	r2, [pc, #288]	; (3000b500 <fault_diagnosis+0x3d4>)
3000b3e0:	4844      	ldr	r0, [pc, #272]	; (3000b4f4 <fault_diagnosis+0x3c8>)
3000b3e2:	f000 fb9d 	bl	3000bb20 <__DiagPrintf_veneer>
3000b3e6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b3ea:	06ca      	lsls	r2, r1, #27
3000b3ec:	f57f aef1 	bpl.w	3000b1d2 <fault_diagnosis+0xa6>
3000b3f0:	4a44      	ldr	r2, [pc, #272]	; (3000b504 <fault_diagnosis+0x3d8>)
3000b3f2:	4840      	ldr	r0, [pc, #256]	; (3000b4f4 <fault_diagnosis+0x3c8>)
3000b3f4:	f000 fb94 	bl	3000bb20 <__DiagPrintf_veneer>
3000b3f8:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b3fc:	068b      	lsls	r3, r1, #26
3000b3fe:	f57f aeeb 	bpl.w	3000b1d8 <fault_diagnosis+0xac>
3000b402:	4a41      	ldr	r2, [pc, #260]	; (3000b508 <fault_diagnosis+0x3dc>)
3000b404:	483b      	ldr	r0, [pc, #236]	; (3000b4f4 <fault_diagnosis+0x3c8>)
3000b406:	f000 fb8b 	bl	3000bb20 <__DiagPrintf_veneer>
3000b40a:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b40e:	e6e3      	b.n	3000b1d8 <fault_diagnosis+0xac>
3000b410:	4a3e      	ldr	r2, [pc, #248]	; (3000b50c <fault_diagnosis+0x3e0>)
3000b412:	483f      	ldr	r0, [pc, #252]	; (3000b510 <fault_diagnosis+0x3e4>)
3000b414:	f000 fb84 	bl	3000bb20 <__DiagPrintf_veneer>
3000b418:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b41c:	0798      	lsls	r0, r3, #30
3000b41e:	f57f aeeb 	bpl.w	3000b1f8 <fault_diagnosis+0xcc>
3000b422:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b426:	4a3b      	ldr	r2, [pc, #236]	; (3000b514 <fault_diagnosis+0x3e8>)
3000b428:	4839      	ldr	r0, [pc, #228]	; (3000b510 <fault_diagnosis+0x3e4>)
3000b42a:	f000 fb79 	bl	3000bb20 <__DiagPrintf_veneer>
3000b42e:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b432:	0759      	lsls	r1, r3, #29
3000b434:	f57f aee3 	bpl.w	3000b1fe <fault_diagnosis+0xd2>
3000b438:	4a37      	ldr	r2, [pc, #220]	; (3000b518 <fault_diagnosis+0x3ec>)
3000b43a:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b43e:	4834      	ldr	r0, [pc, #208]	; (3000b510 <fault_diagnosis+0x3e4>)
3000b440:	f000 fb6e 	bl	3000bb20 <__DiagPrintf_veneer>
3000b444:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b448:	071a      	lsls	r2, r3, #28
3000b44a:	f57f aedb 	bpl.w	3000b204 <fault_diagnosis+0xd8>
3000b44e:	4a33      	ldr	r2, [pc, #204]	; (3000b51c <fault_diagnosis+0x3f0>)
3000b450:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b454:	482e      	ldr	r0, [pc, #184]	; (3000b510 <fault_diagnosis+0x3e4>)
3000b456:	f000 fb63 	bl	3000bb20 <__DiagPrintf_veneer>
3000b45a:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b45e:	06db      	lsls	r3, r3, #27
3000b460:	f57f aed3 	bpl.w	3000b20a <fault_diagnosis+0xde>
3000b464:	4a2e      	ldr	r2, [pc, #184]	; (3000b520 <fault_diagnosis+0x3f4>)
3000b466:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b46a:	4829      	ldr	r0, [pc, #164]	; (3000b510 <fault_diagnosis+0x3e4>)
3000b46c:	f000 fb58 	bl	3000bb20 <__DiagPrintf_veneer>
3000b470:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b474:	07d8      	lsls	r0, r3, #31
3000b476:	f57f aecd 	bpl.w	3000b214 <fault_diagnosis+0xe8>
3000b47a:	4a2a      	ldr	r2, [pc, #168]	; (3000b524 <fault_diagnosis+0x3f8>)
3000b47c:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b480:	4823      	ldr	r0, [pc, #140]	; (3000b510 <fault_diagnosis+0x3e4>)
3000b482:	f000 fb4d 	bl	3000bb20 <__DiagPrintf_veneer>
3000b486:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b48a:	e6c3      	b.n	3000b214 <fault_diagnosis+0xe8>
3000b48c:	4a26      	ldr	r2, [pc, #152]	; (3000b528 <fault_diagnosis+0x3fc>)
3000b48e:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000b490:	4826      	ldr	r0, [pc, #152]	; (3000b52c <fault_diagnosis+0x400>)
3000b492:	f000 fb45 	bl	3000bb20 <__DiagPrintf_veneer>
3000b496:	e673      	b.n	3000b180 <fault_diagnosis+0x54>
3000b498:	4a25      	ldr	r2, [pc, #148]	; (3000b530 <fault_diagnosis+0x404>)
3000b49a:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b49c:	4825      	ldr	r0, [pc, #148]	; (3000b534 <fault_diagnosis+0x408>)
3000b49e:	f000 fb3f 	bl	3000bb20 <__DiagPrintf_veneer>
3000b4a2:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b4a6:	e6ca      	b.n	3000b23e <fault_diagnosis+0x112>
3000b4a8:	4a23      	ldr	r2, [pc, #140]	; (3000b538 <fault_diagnosis+0x40c>)
3000b4aa:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b4ac:	4821      	ldr	r0, [pc, #132]	; (3000b534 <fault_diagnosis+0x408>)
3000b4ae:	f000 fb37 	bl	3000bb20 <__DiagPrintf_veneer>
3000b4b2:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b4b6:	e6bf      	b.n	3000b238 <fault_diagnosis+0x10c>
3000b4b8:	4a20      	ldr	r2, [pc, #128]	; (3000b53c <fault_diagnosis+0x410>)
3000b4ba:	481e      	ldr	r0, [pc, #120]	; (3000b534 <fault_diagnosis+0x408>)
3000b4bc:	f000 fb30 	bl	3000bb20 <__DiagPrintf_veneer>
3000b4c0:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b4c4:	e6b5      	b.n	3000b232 <fault_diagnosis+0x106>
3000b4c6:	4a1e      	ldr	r2, [pc, #120]	; (3000b540 <fault_diagnosis+0x414>)
3000b4c8:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b4ca:	481a      	ldr	r0, [pc, #104]	; (3000b534 <fault_diagnosis+0x408>)
3000b4cc:	f000 fb28 	bl	3000bb20 <__DiagPrintf_veneer>
3000b4d0:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b4d4:	e6b6      	b.n	3000b244 <fault_diagnosis+0x118>
3000b4d6:	4a1b      	ldr	r2, [pc, #108]	; (3000b544 <fault_diagnosis+0x418>)
3000b4d8:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000b4da:	481b      	ldr	r0, [pc, #108]	; (3000b548 <fault_diagnosis+0x41c>)
3000b4dc:	f000 fb20 	bl	3000bb20 <__DiagPrintf_veneer>
3000b4e0:	e665      	b.n	3000b1ae <fault_diagnosis+0x82>
3000b4e2:	4a1a      	ldr	r2, [pc, #104]	; (3000b54c <fault_diagnosis+0x420>)
3000b4e4:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000b4e6:	481a      	ldr	r0, [pc, #104]	; (3000b550 <fault_diagnosis+0x424>)
3000b4e8:	f000 fb1a 	bl	3000bb20 <__DiagPrintf_veneer>
3000b4ec:	e679      	b.n	3000b1e2 <fault_diagnosis+0xb6>
3000b4ee:	bf00      	nop
3000b4f0:	3000d6d4 	.word	0x3000d6d4
3000b4f4:	3000d708 	.word	0x3000d708
3000b4f8:	3000d720 	.word	0x3000d720
3000b4fc:	3000d758 	.word	0x3000d758
3000b500:	3000d790 	.word	0x3000d790
3000b504:	3000d7b8 	.word	0x3000d7b8
3000b508:	3000d7e0 	.word	0x3000d7e0
3000b50c:	3000d85c 	.word	0x3000d85c
3000b510:	3000d8a4 	.word	0x3000d8a4
3000b514:	3000d8bc 	.word	0x3000d8bc
3000b518:	3000d908 	.word	0x3000d908
3000b51c:	3000d968 	.word	0x3000d968
3000b520:	3000d9b0 	.word	0x3000d9b0
3000b524:	3000da0c 	.word	0x3000da0c
3000b528:	3000d4fc 	.word	0x3000d4fc
3000b52c:	3000d524 	.word	0x3000d524
3000b530:	3000db48 	.word	0x3000db48
3000b534:	3000dafc 	.word	0x3000dafc
3000b538:	3000db14 	.word	0x3000db14
3000b53c:	3000dacc 	.word	0x3000dacc
3000b540:	3000db74 	.word	0x3000db74
3000b544:	3000d68c 	.word	0x3000d68c
3000b548:	3000d6bc 	.word	0x3000d6bc
3000b54c:	3000d820 	.word	0x3000d820
3000b550:	3000d844 	.word	0x3000d844

3000b554 <rtk_log_write>:
3000b554:	b40c      	push	{r2, r3}
3000b556:	4b18      	ldr	r3, [pc, #96]	; (3000b5b8 <rtk_log_write+0x64>)
3000b558:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000b55c:	681a      	ldr	r2, [r3, #0]
3000b55e:	b083      	sub	sp, #12
3000b560:	4680      	mov	r8, r0
3000b562:	460e      	mov	r6, r1
3000b564:	2a02      	cmp	r2, #2
3000b566:	d825      	bhi.n	3000b5b4 <rtk_log_write+0x60>
3000b568:	681f      	ldr	r7, [r3, #0]
3000b56a:	b1a7      	cbz	r7, 3000b596 <rtk_log_write+0x42>
3000b56c:	4d13      	ldr	r5, [pc, #76]	; (3000b5bc <rtk_log_write+0x68>)
3000b56e:	2400      	movs	r4, #0
3000b570:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000b5c8 <rtk_log_write+0x74>
3000b574:	e002      	b.n	3000b57c <rtk_log_write+0x28>
3000b576:	3401      	adds	r4, #1
3000b578:	42a7      	cmp	r7, r4
3000b57a:	d90c      	bls.n	3000b596 <rtk_log_write+0x42>
3000b57c:	4628      	mov	r0, r5
3000b57e:	4631      	mov	r1, r6
3000b580:	350b      	adds	r5, #11
3000b582:	47c8      	blx	r9
3000b584:	2800      	cmp	r0, #0
3000b586:	d1f6      	bne.n	3000b576 <rtk_log_write+0x22>
3000b588:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000b58c:	4b0c      	ldr	r3, [pc, #48]	; (3000b5c0 <rtk_log_write+0x6c>)
3000b58e:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000b592:	5d1b      	ldrb	r3, [r3, r4]
3000b594:	e001      	b.n	3000b59a <rtk_log_write+0x46>
3000b596:	4b0b      	ldr	r3, [pc, #44]	; (3000b5c4 <rtk_log_write+0x70>)
3000b598:	781b      	ldrb	r3, [r3, #0]
3000b59a:	4598      	cmp	r8, r3
3000b59c:	d805      	bhi.n	3000b5aa <rtk_log_write+0x56>
3000b59e:	aa0b      	add	r2, sp, #44	; 0x2c
3000b5a0:	990a      	ldr	r1, [sp, #40]	; 0x28
3000b5a2:	2000      	movs	r0, #0
3000b5a4:	9201      	str	r2, [sp, #4]
3000b5a6:	f000 fa2b 	bl	3000ba00 <__DiagVSprintf_veneer>
3000b5aa:	b003      	add	sp, #12
3000b5ac:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000b5b0:	b002      	add	sp, #8
3000b5b2:	4770      	bx	lr
3000b5b4:	2703      	movs	r7, #3
3000b5b6:	e7d9      	b.n	3000b56c <rtk_log_write+0x18>
3000b5b8:	3000f7a8 	.word	0x3000f7a8
3000b5bc:	3000f7ad 	.word	0x3000f7ad
3000b5c0:	3000f7ac 	.word	0x3000f7ac
3000b5c4:	3000ed8c 	.word	0x3000ed8c
3000b5c8:	00012c89 	.word	0x00012c89

3000b5cc <BOOT_VerCheck>:
3000b5cc:	b570      	push	{r4, r5, r6, lr}
3000b5ce:	4b1d      	ldr	r3, [pc, #116]	; (3000b644 <BOOT_VerCheck+0x78>)
3000b5d0:	4798      	blx	r3
3000b5d2:	2800      	cmp	r0, #0
3000b5d4:	d135      	bne.n	3000b642 <BOOT_VerCheck+0x76>
3000b5d6:	4b1c      	ldr	r3, [pc, #112]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b5d8:	2403      	movs	r4, #3
3000b5da:	4d1c      	ldr	r5, [pc, #112]	; (3000b64c <BOOT_VerCheck+0x80>)
3000b5dc:	2003      	movs	r0, #3
3000b5de:	4a1c      	ldr	r2, [pc, #112]	; (3000b650 <BOOT_VerCheck+0x84>)
3000b5e0:	4619      	mov	r1, r3
3000b5e2:	f7ff ffb7 	bl	3000b554 <rtk_log_write>
3000b5e6:	4b18      	ldr	r3, [pc, #96]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b5e8:	462a      	mov	r2, r5
3000b5ea:	2003      	movs	r0, #3
3000b5ec:	4619      	mov	r1, r3
3000b5ee:	f7ff ffb1 	bl	3000b554 <rtk_log_write>
3000b5f2:	4b15      	ldr	r3, [pc, #84]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b5f4:	4a17      	ldr	r2, [pc, #92]	; (3000b654 <BOOT_VerCheck+0x88>)
3000b5f6:	2003      	movs	r0, #3
3000b5f8:	4619      	mov	r1, r3
3000b5fa:	f7ff ffab 	bl	3000b554 <rtk_log_write>
3000b5fe:	4b12      	ldr	r3, [pc, #72]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b600:	4a15      	ldr	r2, [pc, #84]	; (3000b658 <BOOT_VerCheck+0x8c>)
3000b602:	2003      	movs	r0, #3
3000b604:	4619      	mov	r1, r3
3000b606:	f7ff ffa5 	bl	3000b554 <rtk_log_write>
3000b60a:	4b0f      	ldr	r3, [pc, #60]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b60c:	4a13      	ldr	r2, [pc, #76]	; (3000b65c <BOOT_VerCheck+0x90>)
3000b60e:	2003      	movs	r0, #3
3000b610:	4619      	mov	r1, r3
3000b612:	f7ff ff9f 	bl	3000b554 <rtk_log_write>
3000b616:	4b0c      	ldr	r3, [pc, #48]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b618:	462a      	mov	r2, r5
3000b61a:	2003      	movs	r0, #3
3000b61c:	4619      	mov	r1, r3
3000b61e:	f7ff ff99 	bl	3000b554 <rtk_log_write>
3000b622:	4b09      	ldr	r3, [pc, #36]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b624:	4a0e      	ldr	r2, [pc, #56]	; (3000b660 <BOOT_VerCheck+0x94>)
3000b626:	2003      	movs	r0, #3
3000b628:	4619      	mov	r1, r3
3000b62a:	f7ff ff93 	bl	3000b554 <rtk_log_write>
3000b62e:	3c01      	subs	r4, #1
3000b630:	d1d9      	bne.n	3000b5e6 <BOOT_VerCheck+0x1a>
3000b632:	4b05      	ldr	r3, [pc, #20]	; (3000b648 <BOOT_VerCheck+0x7c>)
3000b634:	2003      	movs	r0, #3
3000b636:	4a0b      	ldr	r2, [pc, #44]	; (3000b664 <BOOT_VerCheck+0x98>)
3000b638:	4619      	mov	r1, r3
3000b63a:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000b63e:	f7ff bf89 	b.w	3000b554 <rtk_log_write>
3000b642:	bd70      	pop	{r4, r5, r6, pc}
3000b644:	30009265 	.word	0x30009265
3000b648:	3000dc86 	.word	0x3000dc86
3000b64c:	3000dcce 	.word	0x3000dcce
3000b650:	3000dc8b 	.word	0x3000dc8b
3000b654:	3000dd11 	.word	0x3000dd11
3000b658:	3000dd54 	.word	0x3000dd54
3000b65c:	3000dd97 	.word	0x3000dd97
3000b660:	3000ddda 	.word	0x3000ddda
3000b664:	3000de1d 	.word	0x3000de1d

3000b668 <__aeabi_uldivmod>:
3000b668:	b953      	cbnz	r3, 3000b680 <__aeabi_uldivmod+0x18>
3000b66a:	b94a      	cbnz	r2, 3000b680 <__aeabi_uldivmod+0x18>
3000b66c:	2900      	cmp	r1, #0
3000b66e:	bf08      	it	eq
3000b670:	2800      	cmpeq	r0, #0
3000b672:	bf1c      	itt	ne
3000b674:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000b678:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000b67c:	f000 b960 	b.w	3000b940 <__aeabi_idiv0>
3000b680:	f1ad 0c08 	sub.w	ip, sp, #8
3000b684:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000b688:	f000 f806 	bl	3000b698 <__udivmoddi4>
3000b68c:	f8dd e004 	ldr.w	lr, [sp, #4]
3000b690:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000b694:	b004      	add	sp, #16
3000b696:	4770      	bx	lr

3000b698 <__udivmoddi4>:
3000b698:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b69c:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000b69e:	4688      	mov	r8, r1
3000b6a0:	468e      	mov	lr, r1
3000b6a2:	4615      	mov	r5, r2
3000b6a4:	4604      	mov	r4, r0
3000b6a6:	4619      	mov	r1, r3
3000b6a8:	2b00      	cmp	r3, #0
3000b6aa:	f040 80c6 	bne.w	3000b83a <__udivmoddi4+0x1a2>
3000b6ae:	4542      	cmp	r2, r8
3000b6b0:	fab2 f782 	clz	r7, r2
3000b6b4:	d946      	bls.n	3000b744 <__udivmoddi4+0xac>
3000b6b6:	b14f      	cbz	r7, 3000b6cc <__udivmoddi4+0x34>
3000b6b8:	f1c7 0c20 	rsb	ip, r7, #32
3000b6bc:	fa08 f307 	lsl.w	r3, r8, r7
3000b6c0:	40bd      	lsls	r5, r7
3000b6c2:	40bc      	lsls	r4, r7
3000b6c4:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b6c8:	ea4c 0e03 	orr.w	lr, ip, r3
3000b6cc:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000b6d0:	fa1f fc85 	uxth.w	ip, r5
3000b6d4:	0c22      	lsrs	r2, r4, #16
3000b6d6:	fbbe f9f8 	udiv	r9, lr, r8
3000b6da:	fb08 e319 	mls	r3, r8, r9, lr
3000b6de:	fb09 fa0c 	mul.w	sl, r9, ip
3000b6e2:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000b6e6:	459a      	cmp	sl, r3
3000b6e8:	d928      	bls.n	3000b73c <__udivmoddi4+0xa4>
3000b6ea:	18eb      	adds	r3, r5, r3
3000b6ec:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000b6f0:	d204      	bcs.n	3000b6fc <__udivmoddi4+0x64>
3000b6f2:	459a      	cmp	sl, r3
3000b6f4:	d902      	bls.n	3000b6fc <__udivmoddi4+0x64>
3000b6f6:	f1a9 0002 	sub.w	r0, r9, #2
3000b6fa:	442b      	add	r3, r5
3000b6fc:	eba3 030a 	sub.w	r3, r3, sl
3000b700:	b2a4      	uxth	r4, r4
3000b702:	fbb3 f2f8 	udiv	r2, r3, r8
3000b706:	fb08 3312 	mls	r3, r8, r2, r3
3000b70a:	fb02 fc0c 	mul.w	ip, r2, ip
3000b70e:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b712:	45a4      	cmp	ip, r4
3000b714:	d914      	bls.n	3000b740 <__udivmoddi4+0xa8>
3000b716:	192c      	adds	r4, r5, r4
3000b718:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000b71c:	d203      	bcs.n	3000b726 <__udivmoddi4+0x8e>
3000b71e:	45a4      	cmp	ip, r4
3000b720:	d901      	bls.n	3000b726 <__udivmoddi4+0x8e>
3000b722:	1e93      	subs	r3, r2, #2
3000b724:	442c      	add	r4, r5
3000b726:	eba4 040c 	sub.w	r4, r4, ip
3000b72a:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000b72e:	b11e      	cbz	r6, 3000b738 <__udivmoddi4+0xa0>
3000b730:	40fc      	lsrs	r4, r7
3000b732:	2300      	movs	r3, #0
3000b734:	6034      	str	r4, [r6, #0]
3000b736:	6073      	str	r3, [r6, #4]
3000b738:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b73c:	4648      	mov	r0, r9
3000b73e:	e7dd      	b.n	3000b6fc <__udivmoddi4+0x64>
3000b740:	4613      	mov	r3, r2
3000b742:	e7f0      	b.n	3000b726 <__udivmoddi4+0x8e>
3000b744:	b902      	cbnz	r2, 3000b748 <__udivmoddi4+0xb0>
3000b746:	deff      	udf	#255	; 0xff
3000b748:	bb8f      	cbnz	r7, 3000b7ae <__udivmoddi4+0x116>
3000b74a:	eba8 0302 	sub.w	r3, r8, r2
3000b74e:	2101      	movs	r1, #1
3000b750:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b754:	b2aa      	uxth	r2, r5
3000b756:	0c20      	lsrs	r0, r4, #16
3000b758:	fbb3 fcfe 	udiv	ip, r3, lr
3000b75c:	fb0e 331c 	mls	r3, lr, ip, r3
3000b760:	fb0c f802 	mul.w	r8, ip, r2
3000b764:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000b768:	4598      	cmp	r8, r3
3000b76a:	d962      	bls.n	3000b832 <__udivmoddi4+0x19a>
3000b76c:	18eb      	adds	r3, r5, r3
3000b76e:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000b772:	d204      	bcs.n	3000b77e <__udivmoddi4+0xe6>
3000b774:	4598      	cmp	r8, r3
3000b776:	d902      	bls.n	3000b77e <__udivmoddi4+0xe6>
3000b778:	f1ac 0002 	sub.w	r0, ip, #2
3000b77c:	442b      	add	r3, r5
3000b77e:	eba3 0308 	sub.w	r3, r3, r8
3000b782:	b2a4      	uxth	r4, r4
3000b784:	fbb3 fcfe 	udiv	ip, r3, lr
3000b788:	fb0e 331c 	mls	r3, lr, ip, r3
3000b78c:	fb0c f202 	mul.w	r2, ip, r2
3000b790:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b794:	42a2      	cmp	r2, r4
3000b796:	d94e      	bls.n	3000b836 <__udivmoddi4+0x19e>
3000b798:	192c      	adds	r4, r5, r4
3000b79a:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000b79e:	d204      	bcs.n	3000b7aa <__udivmoddi4+0x112>
3000b7a0:	42a2      	cmp	r2, r4
3000b7a2:	d902      	bls.n	3000b7aa <__udivmoddi4+0x112>
3000b7a4:	f1ac 0302 	sub.w	r3, ip, #2
3000b7a8:	442c      	add	r4, r5
3000b7aa:	1aa4      	subs	r4, r4, r2
3000b7ac:	e7bd      	b.n	3000b72a <__udivmoddi4+0x92>
3000b7ae:	f1c7 0c20 	rsb	ip, r7, #32
3000b7b2:	40bd      	lsls	r5, r7
3000b7b4:	fa08 f307 	lsl.w	r3, r8, r7
3000b7b8:	40bc      	lsls	r4, r7
3000b7ba:	fa28 f90c 	lsr.w	r9, r8, ip
3000b7be:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b7c2:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b7c6:	ea4c 0203 	orr.w	r2, ip, r3
3000b7ca:	b2ab      	uxth	r3, r5
3000b7cc:	fbb9 fcfe 	udiv	ip, r9, lr
3000b7d0:	0c11      	lsrs	r1, r2, #16
3000b7d2:	fb0e 901c 	mls	r0, lr, ip, r9
3000b7d6:	fb0c f803 	mul.w	r8, ip, r3
3000b7da:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b7de:	4580      	cmp	r8, r0
3000b7e0:	d923      	bls.n	3000b82a <__udivmoddi4+0x192>
3000b7e2:	1828      	adds	r0, r5, r0
3000b7e4:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000b7e8:	d204      	bcs.n	3000b7f4 <__udivmoddi4+0x15c>
3000b7ea:	4580      	cmp	r8, r0
3000b7ec:	d902      	bls.n	3000b7f4 <__udivmoddi4+0x15c>
3000b7ee:	f1ac 0102 	sub.w	r1, ip, #2
3000b7f2:	4428      	add	r0, r5
3000b7f4:	eba0 0008 	sub.w	r0, r0, r8
3000b7f8:	b292      	uxth	r2, r2
3000b7fa:	fbb0 fcfe 	udiv	ip, r0, lr
3000b7fe:	fb0e 001c 	mls	r0, lr, ip, r0
3000b802:	fb0c f803 	mul.w	r8, ip, r3
3000b806:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000b80a:	4598      	cmp	r8, r3
3000b80c:	d90f      	bls.n	3000b82e <__udivmoddi4+0x196>
3000b80e:	18eb      	adds	r3, r5, r3
3000b810:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000b814:	d204      	bcs.n	3000b820 <__udivmoddi4+0x188>
3000b816:	4598      	cmp	r8, r3
3000b818:	d902      	bls.n	3000b820 <__udivmoddi4+0x188>
3000b81a:	f1ac 0202 	sub.w	r2, ip, #2
3000b81e:	442b      	add	r3, r5
3000b820:	eba3 0308 	sub.w	r3, r3, r8
3000b824:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000b828:	e792      	b.n	3000b750 <__udivmoddi4+0xb8>
3000b82a:	4661      	mov	r1, ip
3000b82c:	e7e2      	b.n	3000b7f4 <__udivmoddi4+0x15c>
3000b82e:	4662      	mov	r2, ip
3000b830:	e7f6      	b.n	3000b820 <__udivmoddi4+0x188>
3000b832:	4660      	mov	r0, ip
3000b834:	e7a3      	b.n	3000b77e <__udivmoddi4+0xe6>
3000b836:	4663      	mov	r3, ip
3000b838:	e7b7      	b.n	3000b7aa <__udivmoddi4+0x112>
3000b83a:	4543      	cmp	r3, r8
3000b83c:	d905      	bls.n	3000b84a <__udivmoddi4+0x1b2>
3000b83e:	b10e      	cbz	r6, 3000b844 <__udivmoddi4+0x1ac>
3000b840:	e9c6 0800 	strd	r0, r8, [r6]
3000b844:	2100      	movs	r1, #0
3000b846:	4608      	mov	r0, r1
3000b848:	e776      	b.n	3000b738 <__udivmoddi4+0xa0>
3000b84a:	fab3 fc83 	clz	ip, r3
3000b84e:	f1bc 0f00 	cmp.w	ip, #0
3000b852:	d110      	bne.n	3000b876 <__udivmoddi4+0x1de>
3000b854:	4543      	cmp	r3, r8
3000b856:	d301      	bcc.n	3000b85c <__udivmoddi4+0x1c4>
3000b858:	4282      	cmp	r2, r0
3000b85a:	d80a      	bhi.n	3000b872 <__udivmoddi4+0x1da>
3000b85c:	1a84      	subs	r4, r0, r2
3000b85e:	eb68 0303 	sbc.w	r3, r8, r3
3000b862:	2001      	movs	r0, #1
3000b864:	469e      	mov	lr, r3
3000b866:	2e00      	cmp	r6, #0
3000b868:	d068      	beq.n	3000b93c <__udivmoddi4+0x2a4>
3000b86a:	e9c6 4e00 	strd	r4, lr, [r6]
3000b86e:	2100      	movs	r1, #0
3000b870:	e762      	b.n	3000b738 <__udivmoddi4+0xa0>
3000b872:	4660      	mov	r0, ip
3000b874:	e7f7      	b.n	3000b866 <__udivmoddi4+0x1ce>
3000b876:	f1cc 0e20 	rsb	lr, ip, #32
3000b87a:	fa03 f30c 	lsl.w	r3, r3, ip
3000b87e:	fa02 f50c 	lsl.w	r5, r2, ip
3000b882:	fa00 f70c 	lsl.w	r7, r0, ip
3000b886:	fa22 f40e 	lsr.w	r4, r2, lr
3000b88a:	fa28 fb0e 	lsr.w	fp, r8, lr
3000b88e:	fa20 f10e 	lsr.w	r1, r0, lr
3000b892:	431c      	orrs	r4, r3
3000b894:	fa08 f30c 	lsl.w	r3, r8, ip
3000b898:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000b89c:	4319      	orrs	r1, r3
3000b89e:	b2a3      	uxth	r3, r4
3000b8a0:	0c0a      	lsrs	r2, r1, #16
3000b8a2:	fbbb f8f9 	udiv	r8, fp, r9
3000b8a6:	fb09 bb18 	mls	fp, r9, r8, fp
3000b8aa:	fb08 fa03 	mul.w	sl, r8, r3
3000b8ae:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000b8b2:	4592      	cmp	sl, r2
3000b8b4:	d93e      	bls.n	3000b934 <__udivmoddi4+0x29c>
3000b8b6:	18a2      	adds	r2, r4, r2
3000b8b8:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000b8bc:	d204      	bcs.n	3000b8c8 <__udivmoddi4+0x230>
3000b8be:	4592      	cmp	sl, r2
3000b8c0:	d902      	bls.n	3000b8c8 <__udivmoddi4+0x230>
3000b8c2:	f1a8 0002 	sub.w	r0, r8, #2
3000b8c6:	4422      	add	r2, r4
3000b8c8:	eba2 020a 	sub.w	r2, r2, sl
3000b8cc:	b289      	uxth	r1, r1
3000b8ce:	fbb2 f8f9 	udiv	r8, r2, r9
3000b8d2:	fb09 2218 	mls	r2, r9, r8, r2
3000b8d6:	fb08 f303 	mul.w	r3, r8, r3
3000b8da:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000b8de:	4293      	cmp	r3, r2
3000b8e0:	d92a      	bls.n	3000b938 <__udivmoddi4+0x2a0>
3000b8e2:	18a2      	adds	r2, r4, r2
3000b8e4:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000b8e8:	d204      	bcs.n	3000b8f4 <__udivmoddi4+0x25c>
3000b8ea:	4293      	cmp	r3, r2
3000b8ec:	d902      	bls.n	3000b8f4 <__udivmoddi4+0x25c>
3000b8ee:	f1a8 0102 	sub.w	r1, r8, #2
3000b8f2:	4422      	add	r2, r4
3000b8f4:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b8f8:	1ad2      	subs	r2, r2, r3
3000b8fa:	fba0 9805 	umull	r9, r8, r0, r5
3000b8fe:	4542      	cmp	r2, r8
3000b900:	464b      	mov	r3, r9
3000b902:	4641      	mov	r1, r8
3000b904:	d302      	bcc.n	3000b90c <__udivmoddi4+0x274>
3000b906:	d106      	bne.n	3000b916 <__udivmoddi4+0x27e>
3000b908:	454f      	cmp	r7, r9
3000b90a:	d204      	bcs.n	3000b916 <__udivmoddi4+0x27e>
3000b90c:	3801      	subs	r0, #1
3000b90e:	ebb9 0305 	subs.w	r3, r9, r5
3000b912:	eb68 0104 	sbc.w	r1, r8, r4
3000b916:	b18e      	cbz	r6, 3000b93c <__udivmoddi4+0x2a4>
3000b918:	1afc      	subs	r4, r7, r3
3000b91a:	eb62 0701 	sbc.w	r7, r2, r1
3000b91e:	fa07 fe0e 	lsl.w	lr, r7, lr
3000b922:	fa24 f30c 	lsr.w	r3, r4, ip
3000b926:	fa27 f70c 	lsr.w	r7, r7, ip
3000b92a:	ea4e 0303 	orr.w	r3, lr, r3
3000b92e:	e9c6 3700 	strd	r3, r7, [r6]
3000b932:	e79c      	b.n	3000b86e <__udivmoddi4+0x1d6>
3000b934:	4640      	mov	r0, r8
3000b936:	e7c7      	b.n	3000b8c8 <__udivmoddi4+0x230>
3000b938:	4641      	mov	r1, r8
3000b93a:	e7db      	b.n	3000b8f4 <__udivmoddi4+0x25c>
3000b93c:	4631      	mov	r1, r6
3000b93e:	e6fb      	b.n	3000b738 <__udivmoddi4+0xa0>

3000b940 <__aeabi_idiv0>:
3000b940:	4770      	bx	lr
3000b942:	bf00      	nop

3000b944 <_mbtowc_r>:
3000b944:	b430      	push	{r4, r5}
3000b946:	4c06      	ldr	r4, [pc, #24]	; (3000b960 <_mbtowc_r+0x1c>)
3000b948:	4d06      	ldr	r5, [pc, #24]	; (3000b964 <_mbtowc_r+0x20>)
3000b94a:	6824      	ldr	r4, [r4, #0]
3000b94c:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b94e:	2c00      	cmp	r4, #0
3000b950:	bf08      	it	eq
3000b952:	462c      	moveq	r4, r5
3000b954:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000b958:	46a4      	mov	ip, r4
3000b95a:	bc30      	pop	{r4, r5}
3000b95c:	4760      	bx	ip
3000b95e:	bf00      	nop
3000b960:	3000ef00 	.word	0x3000ef00
3000b964:	3000ed90 	.word	0x3000ed90

3000b968 <__ascii_mbtowc>:
3000b968:	b082      	sub	sp, #8
3000b96a:	b901      	cbnz	r1, 3000b96e <__ascii_mbtowc+0x6>
3000b96c:	a901      	add	r1, sp, #4
3000b96e:	b142      	cbz	r2, 3000b982 <__ascii_mbtowc+0x1a>
3000b970:	b14b      	cbz	r3, 3000b986 <__ascii_mbtowc+0x1e>
3000b972:	7813      	ldrb	r3, [r2, #0]
3000b974:	600b      	str	r3, [r1, #0]
3000b976:	7812      	ldrb	r2, [r2, #0]
3000b978:	1e10      	subs	r0, r2, #0
3000b97a:	bf18      	it	ne
3000b97c:	2001      	movne	r0, #1
3000b97e:	b002      	add	sp, #8
3000b980:	4770      	bx	lr
3000b982:	4610      	mov	r0, r2
3000b984:	e7fb      	b.n	3000b97e <__ascii_mbtowc+0x16>
3000b986:	f06f 0001 	mvn.w	r0, #1
3000b98a:	e7f8      	b.n	3000b97e <__ascii_mbtowc+0x16>

3000b98c <_wctomb_r>:
3000b98c:	b430      	push	{r4, r5}
3000b98e:	4c06      	ldr	r4, [pc, #24]	; (3000b9a8 <_wctomb_r+0x1c>)
3000b990:	4d06      	ldr	r5, [pc, #24]	; (3000b9ac <_wctomb_r+0x20>)
3000b992:	6824      	ldr	r4, [r4, #0]
3000b994:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b996:	2c00      	cmp	r4, #0
3000b998:	bf08      	it	eq
3000b99a:	462c      	moveq	r4, r5
3000b99c:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000b9a0:	46a4      	mov	ip, r4
3000b9a2:	bc30      	pop	{r4, r5}
3000b9a4:	4760      	bx	ip
3000b9a6:	bf00      	nop
3000b9a8:	3000ef00 	.word	0x3000ef00
3000b9ac:	3000ed90 	.word	0x3000ed90

3000b9b0 <__ascii_wctomb>:
3000b9b0:	b149      	cbz	r1, 3000b9c6 <__ascii_wctomb+0x16>
3000b9b2:	2aff      	cmp	r2, #255	; 0xff
3000b9b4:	bf8d      	iteet	hi
3000b9b6:	238a      	movhi	r3, #138	; 0x8a
3000b9b8:	2001      	movls	r0, #1
3000b9ba:	700a      	strbls	r2, [r1, #0]
3000b9bc:	6003      	strhi	r3, [r0, #0]
3000b9be:	bf88      	it	hi
3000b9c0:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000b9c4:	4770      	bx	lr
3000b9c6:	4608      	mov	r0, r1
3000b9c8:	4770      	bx	lr
3000b9ca:	bf00      	nop
3000b9cc:	0000      	movs	r0, r0
	...

3000b9d0 <__CRYPTO_SHA_Init_A_veneer>:
3000b9d0:	b401      	push	{r0}
3000b9d2:	4802      	ldr	r0, [pc, #8]	; (3000b9dc <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000b9d4:	4684      	mov	ip, r0
3000b9d6:	bc01      	pop	{r0}
3000b9d8:	4760      	bx	ip
3000b9da:	bf00      	nop
3000b9dc:	000022dd 	.word	0x000022dd

3000b9e0 <__io_assert_failed_veneer>:
3000b9e0:	b401      	push	{r0}
3000b9e2:	4802      	ldr	r0, [pc, #8]	; (3000b9ec <__io_assert_failed_veneer+0xc>)
3000b9e4:	4684      	mov	ip, r0
3000b9e6:	bc01      	pop	{r0}
3000b9e8:	4760      	bx	ip
3000b9ea:	bf00      	nop
3000b9ec:	0000992d 	.word	0x0000992d

3000b9f0 <__CRYPTO_SHA_Init_B_veneer>:
3000b9f0:	b401      	push	{r0}
3000b9f2:	4802      	ldr	r0, [pc, #8]	; (3000b9fc <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000b9f4:	4684      	mov	ip, r0
3000b9f6:	bc01      	pop	{r0}
3000b9f8:	4760      	bx	ip
3000b9fa:	bf00      	nop
3000b9fc:	00002451 	.word	0x00002451

3000ba00 <__DiagVSprintf_veneer>:
3000ba00:	b401      	push	{r0}
3000ba02:	4802      	ldr	r0, [pc, #8]	; (3000ba0c <__DiagVSprintf_veneer+0xc>)
3000ba04:	4684      	mov	ip, r0
3000ba06:	bc01      	pop	{r0}
3000ba08:	4760      	bx	ip
3000ba0a:	bf00      	nop
3000ba0c:	0000e331 	.word	0x0000e331

3000ba10 <__SBOOT_Validate_ImgHash_A_veneer>:
3000ba10:	b401      	push	{r0}
3000ba12:	4802      	ldr	r0, [pc, #8]	; (3000ba1c <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000ba14:	4684      	mov	ip, r0
3000ba16:	bc01      	pop	{r0}
3000ba18:	4760      	bx	ip
3000ba1a:	bf00      	nop
3000ba1c:	000033cd 	.word	0x000033cd

3000ba20 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000ba20:	b401      	push	{r0}
3000ba22:	4802      	ldr	r0, [pc, #8]	; (3000ba2c <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000ba24:	4684      	mov	ip, r0
3000ba26:	bc01      	pop	{r0}
3000ba28:	4760      	bx	ip
3000ba2a:	bf00      	nop
3000ba2c:	000059d9 	.word	0x000059d9

3000ba30 <__rtl_crypto_sha2_final_A_veneer>:
3000ba30:	b401      	push	{r0}
3000ba32:	4802      	ldr	r0, [pc, #8]	; (3000ba3c <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000ba34:	4684      	mov	ip, r0
3000ba36:	bc01      	pop	{r0}
3000ba38:	4760      	bx	ip
3000ba3a:	bf00      	nop
3000ba3c:	0000527d 	.word	0x0000527d

3000ba40 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000ba40:	b401      	push	{r0}
3000ba42:	4802      	ldr	r0, [pc, #8]	; (3000ba4c <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000ba44:	4684      	mov	ip, r0
3000ba46:	bc01      	pop	{r0}
3000ba48:	4760      	bx	ip
3000ba4a:	bf00      	nop
3000ba4c:	000024dd 	.word	0x000024dd

3000ba50 <__SBOOT_Validate_PubKey_B_veneer>:
3000ba50:	b401      	push	{r0}
3000ba52:	4802      	ldr	r0, [pc, #8]	; (3000ba5c <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000ba54:	4684      	mov	ip, r0
3000ba56:	bc01      	pop	{r0}
3000ba58:	4760      	bx	ip
3000ba5a:	bf00      	nop
3000ba5c:	00003495 	.word	0x00003495

3000ba60 <____wrap_memset_veneer>:
3000ba60:	b401      	push	{r0}
3000ba62:	4802      	ldr	r0, [pc, #8]	; (3000ba6c <____wrap_memset_veneer+0xc>)
3000ba64:	4684      	mov	ip, r0
3000ba66:	bc01      	pop	{r0}
3000ba68:	4760      	bx	ip
3000ba6a:	bf00      	nop
3000ba6c:	00012efd 	.word	0x00012efd

3000ba70 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000ba70:	b401      	push	{r0}
3000ba72:	4802      	ldr	r0, [pc, #8]	; (3000ba7c <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000ba74:	4684      	mov	ip, r0
3000ba76:	bc01      	pop	{r0}
3000ba78:	4760      	bx	ip
3000ba7a:	bf00      	nop
3000ba7c:	00005bb9 	.word	0x00005bb9

3000ba80 <__SBOOT_Validate_PubKey_A_veneer>:
3000ba80:	b401      	push	{r0}
3000ba82:	4802      	ldr	r0, [pc, #8]	; (3000ba8c <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000ba84:	4684      	mov	ip, r0
3000ba86:	bc01      	pop	{r0}
3000ba88:	4760      	bx	ip
3000ba8a:	bf00      	nop
3000ba8c:	00003321 	.word	0x00003321

3000ba90 <__INT_HardFault_C_veneer>:
3000ba90:	b401      	push	{r0}
3000ba92:	4802      	ldr	r0, [pc, #8]	; (3000ba9c <__INT_HardFault_C_veneer+0xc>)
3000ba94:	4684      	mov	ip, r0
3000ba96:	bc01      	pop	{r0}
3000ba98:	4760      	bx	ip
3000ba9a:	bf00      	nop
3000ba9c:	0000d8ed 	.word	0x0000d8ed

3000baa0 <__SBOOT_Validate_ImgHash_B_veneer>:
3000baa0:	b401      	push	{r0}
3000baa2:	4802      	ldr	r0, [pc, #8]	; (3000baac <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000baa4:	4684      	mov	ip, r0
3000baa6:	bc01      	pop	{r0}
3000baa8:	4760      	bx	ip
3000baaa:	bf00      	nop
3000baac:	00003541 	.word	0x00003541

3000bab0 <__SBOOT_Validate_Algorithm_A_veneer>:
3000bab0:	b401      	push	{r0}
3000bab2:	4802      	ldr	r0, [pc, #8]	; (3000babc <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000bab4:	4684      	mov	ip, r0
3000bab6:	bc01      	pop	{r0}
3000bab8:	4760      	bx	ip
3000baba:	bf00      	nop
3000babc:	00003279 	.word	0x00003279

3000bac0 <__SBOOT_Validate_Algorithm_B_veneer>:
3000bac0:	b401      	push	{r0}
3000bac2:	4802      	ldr	r0, [pc, #8]	; (3000bacc <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000bac4:	4684      	mov	ip, r0
3000bac6:	bc01      	pop	{r0}
3000bac8:	4760      	bx	ip
3000baca:	bf00      	nop
3000bacc:	000033ed 	.word	0x000033ed

3000bad0 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000bad0:	b401      	push	{r0}
3000bad2:	4802      	ldr	r0, [pc, #8]	; (3000badc <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000bad4:	4684      	mov	ip, r0
3000bad6:	bc01      	pop	{r0}
3000bad8:	4760      	bx	ip
3000bada:	bf00      	nop
3000badc:	00002369 	.word	0x00002369

3000bae0 <__rtl_crypto_sha2_init_B_veneer>:
3000bae0:	b401      	push	{r0}
3000bae2:	4802      	ldr	r0, [pc, #8]	; (3000baec <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000bae4:	4684      	mov	ip, r0
3000bae6:	bc01      	pop	{r0}
3000bae8:	4760      	bx	ip
3000baea:	bf00      	nop
3000baec:	00004ffd 	.word	0x00004ffd

3000baf0 <____wrap_memcpy_veneer>:
3000baf0:	b401      	push	{r0}
3000baf2:	4802      	ldr	r0, [pc, #8]	; (3000bafc <____wrap_memcpy_veneer+0xc>)
3000baf4:	4684      	mov	ip, r0
3000baf6:	bc01      	pop	{r0}
3000baf8:	4760      	bx	ip
3000bafa:	bf00      	nop
3000bafc:	00012f05 	.word	0x00012f05

3000bb00 <__SBOOT_GetMdType_A_veneer>:
3000bb00:	b401      	push	{r0}
3000bb02:	4802      	ldr	r0, [pc, #8]	; (3000bb0c <__SBOOT_GetMdType_A_veneer+0xc>)
3000bb04:	4684      	mov	ip, r0
3000bb06:	bc01      	pop	{r0}
3000bb08:	4760      	bx	ip
3000bb0a:	bf00      	nop
3000bb0c:	00003029 	.word	0x00003029

3000bb10 <__rtl_crypto_sha2_final_B_veneer>:
3000bb10:	b401      	push	{r0}
3000bb12:	4802      	ldr	r0, [pc, #8]	; (3000bb1c <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000bb14:	4684      	mov	ip, r0
3000bb16:	bc01      	pop	{r0}
3000bb18:	4760      	bx	ip
3000bb1a:	bf00      	nop
3000bb1c:	000053f1 	.word	0x000053f1

3000bb20 <__DiagPrintf_veneer>:
3000bb20:	b401      	push	{r0}
3000bb22:	4802      	ldr	r0, [pc, #8]	; (3000bb2c <__DiagPrintf_veneer+0xc>)
3000bb24:	4684      	mov	ip, r0
3000bb26:	bc01      	pop	{r0}
3000bb28:	4760      	bx	ip
3000bb2a:	bf00      	nop
3000bb2c:	0000e5f9 	.word	0x0000e5f9

3000bb30 <__SBOOT_Validate_Signature_B_veneer>:
3000bb30:	b401      	push	{r0}
3000bb32:	4802      	ldr	r0, [pc, #8]	; (3000bb3c <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000bb34:	4684      	mov	ip, r0
3000bb36:	bc01      	pop	{r0}
3000bb38:	4760      	bx	ip
3000bb3a:	bf00      	nop
3000bb3c:	00003431 	.word	0x00003431

3000bb40 <__SBOOT_GetMdType_B_veneer>:
3000bb40:	b401      	push	{r0}
3000bb42:	4802      	ldr	r0, [pc, #8]	; (3000bb4c <__SBOOT_GetMdType_B_veneer+0xc>)
3000bb44:	4684      	mov	ip, r0
3000bb46:	bc01      	pop	{r0}
3000bb48:	4760      	bx	ip
3000bb4a:	bf00      	nop
3000bb4c:	0000319d 	.word	0x0000319d

3000bb50 <__rtl_crypto_sha2_update_B_veneer>:
3000bb50:	b401      	push	{r0}
3000bb52:	4802      	ldr	r0, [pc, #8]	; (3000bb5c <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000bb54:	4684      	mov	ip, r0
3000bb56:	bc01      	pop	{r0}
3000bb58:	4760      	bx	ip
3000bb5a:	bf00      	nop
3000bb5c:	00005385 	.word	0x00005385

3000bb60 <__SBOOT_Validate_Signature_A_veneer>:
3000bb60:	b401      	push	{r0}
3000bb62:	4802      	ldr	r0, [pc, #8]	; (3000bb6c <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000bb64:	4684      	mov	ip, r0
3000bb66:	bc01      	pop	{r0}
3000bb68:	4760      	bx	ip
3000bb6a:	bf00      	nop
3000bb6c:	000032bd 	.word	0x000032bd

3000bb70 <__rtl_crypto_sha2_init_A_veneer>:
3000bb70:	b401      	push	{r0}
3000bb72:	4802      	ldr	r0, [pc, #8]	; (3000bb7c <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000bb74:	4684      	mov	ip, r0
3000bb76:	bc01      	pop	{r0}
3000bb78:	4760      	bx	ip
3000bb7a:	bf00      	nop
3000bb7c:	00004e89 	.word	0x00004e89

3000bb80 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000bb80:	b401      	push	{r0}
3000bb82:	4802      	ldr	r0, [pc, #8]	; (3000bb8c <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000bb84:	4684      	mov	ip, r0
3000bb86:	bc01      	pop	{r0}
3000bb88:	4760      	bx	ip
3000bb8a:	bf00      	nop
3000bb8c:	00005b4d 	.word	0x00005b4d

3000bb90 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000bb90:	b401      	push	{r0}
3000bb92:	4802      	ldr	r0, [pc, #8]	; (3000bb9c <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000bb94:	4684      	mov	ip, r0
3000bb96:	bc01      	pop	{r0}
3000bb98:	4760      	bx	ip
3000bb9a:	bf00      	nop
3000bb9c:	00005a45 	.word	0x00005a45

3000bba0 <__rtl_crypto_sha2_update_A_veneer>:
3000bba0:	b401      	push	{r0}
3000bba2:	4802      	ldr	r0, [pc, #8]	; (3000bbac <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000bba4:	4684      	mov	ip, r0
3000bba6:	bc01      	pop	{r0}
3000bba8:	4760      	bx	ip
3000bbaa:	bf00      	nop
3000bbac:	00005211 	.word	0x00005211
3000bbb0:	004f4931 	.word	0x004f4931
3000bbb4:	00004f32 	.word	0x00004f32
3000bbb8:	004f4932 	.word	0x004f4932
3000bbbc:	00004f34 	.word	0x00004f34
3000bbc0:	004f4934 	.word	0x004f4934
3000bbc4:	53414c46 	.word	0x53414c46
3000bbc8:	4b4c4348 	.word	0x4b4c4348
3000bbcc:	00000000 	.word	0x00000000
3000bbd0:	2d73255b 	.word	0x2d73255b
3000bbd4:	63205d49 	.word	0x63205d49
3000bbd8:	62696c61 	.word	0x62696c61
3000bbdc:	69746172 	.word	0x69746172
3000bbe0:	6f5f6e6f 	.word	0x6f5f6e6f
3000bbe4:	255b3a6b 	.word	0x255b3a6b
3000bbe8:	64253a64 	.word	0x64253a64
3000bbec:	5d64253a 	.word	0x5d64253a
3000bbf0:	00000a20 	.word	0x00000a20
3000bbf4:	2d73255b 	.word	0x2d73255b
3000bbf8:	46205d49 	.word	0x46205d49
3000bbfc:	4853414c 	.word	0x4853414c
3000bc00:	4c414320 	.word	0x4c414320
3000bc04:	305b4249 	.word	0x305b4249
3000bc08:	20782578 	.word	0x20782578
3000bc0c:	0a5d4b4f 	.word	0x0a5d4b4f
3000bc10:	00000000 	.word	0x00000000
3000bc14:	2d73255b 	.word	0x2d73255b
3000bc18:	46205d45 	.word	0x46205d45
3000bc1c:	4853414c 	.word	0x4853414c
3000bc20:	4c414320 	.word	0x4c414320
3000bc24:	305b4249 	.word	0x305b4249
3000bc28:	20782578 	.word	0x20782578
3000bc2c:	4c494146 	.word	0x4c494146
3000bc30:	00000a5d 	.word	0x00000a5d
3000bc34:	2d73255b 	.word	0x2d73255b
3000bc38:	46205d49 	.word	0x46205d49
3000bc3c:	6873616c 	.word	0x6873616c
3000bc40:	61655220 	.word	0x61655220
3000bc44:	73252064 	.word	0x73252064
3000bc48:	0000000a 	.word	0x0000000a
3000bc4c:	2d73255b 	.word	0x2d73255b
3000bc50:	46205d45 	.word	0x46205d45
3000bc54:	6873616c 	.word	0x6873616c
3000bc58:	69775320 	.word	0x69775320
3000bc5c:	20686374 	.word	0x20686374
3000bc60:	64616552 	.word	0x64616552
3000bc64:	646f4d20 	.word	0x646f4d20
3000bc68:	41462065 	.word	0x41462065
3000bc6c:	000a4c49 	.word	0x000a4c49
3000bc70:	2d73255b 	.word	0x2d73255b
3000bc74:	46205d49 	.word	0x46205d49
3000bc78:	6873616c 	.word	0x6873616c
3000bc7c:	3a444920 	.word	0x3a444920
3000bc80:	2d782520 	.word	0x2d782520
3000bc84:	252d7825 	.word	0x252d7825
3000bc88:	00000a78 	.word	0x00000a78
3000bc8c:	2d73255b 	.word	0x2d73255b
3000bc90:	54205d57 	.word	0x54205d57
3000bc94:	20736968 	.word	0x20736968
3000bc98:	73616c66 	.word	0x73616c66
3000bc9c:	79742068 	.word	0x79742068
3000bca0:	69206570 	.word	0x69206570
3000bca4:	6f6e2073 	.word	0x6f6e2073
3000bca8:	75732074 	.word	0x75732074
3000bcac:	726f7070 	.word	0x726f7070
3000bcb0:	21646574 	.word	0x21646574
3000bcb4:	0000000a 	.word	0x0000000a
3000bcb8:	2d73255b 	.word	0x2d73255b
3000bcbc:	4e205d49 	.word	0x4e205d49
3000bcc0:	20444e41 	.word	0x20444e41
3000bcc4:	203a4449 	.word	0x203a4449
3000bcc8:	252d7825 	.word	0x252d7825
3000bccc:	00000a78 	.word	0x00000a78
3000bcd0:	00003552 	.word	0x00003552
3000bcd4:	00003652 	.word	0x00003652
3000bcd8:	00003752 	.word	0x00003752
3000bcdc:	00003852 	.word	0x00003852
3000bce0:	00003952 	.word	0x00003952
3000bce4:	00303152 	.word	0x00303152
3000bce8:	00313152 	.word	0x00313152
3000bcec:	00003152 	.word	0x00003152
3000bcf0:	00003252 	.word	0x00003252
3000bcf4:	00003352 	.word	0x00003352
3000bcf8:	544f4f42 	.word	0x544f4f42
3000bcfc:	00000000 	.word	0x00000000
3000bd00:	2d73255b 	.word	0x2d73255b
3000bd04:	48205d41 	.word	0x48205d41
3000bd08:	20647261 	.word	0x20647261
3000bd0c:	6c756146 	.word	0x6c756146
3000bd10:	61502074 	.word	0x61502074
3000bd14:	20686374 	.word	0x20686374
3000bd18:	63655328 	.word	0x63655328
3000bd1c:	29657275 	.word	0x29657275
3000bd20:	00000a0d 	.word	0x00000a0d
3000bd24:	2d73255b 	.word	0x2d73255b
3000bd28:	45205d41 	.word	0x45205d41
3000bd2c:	70656378 	.word	0x70656378
3000bd30:	6e6f6974 	.word	0x6e6f6974
3000bd34:	6f726620 	.word	0x6f726620
3000bd38:	6573206d 	.word	0x6573206d
3000bd3c:	65727563 	.word	0x65727563
3000bd40:	726f7720 	.word	0x726f7720
3000bd44:	0d21646c 	.word	0x0d21646c
3000bd48:	0000000a 	.word	0x0000000a
3000bd4c:	2d73255b 	.word	0x2d73255b
3000bd50:	45205d41 	.word	0x45205d41
3000bd54:	70656378 	.word	0x70656378
3000bd58:	6e6f6974 	.word	0x6e6f6974
3000bd5c:	6f726620 	.word	0x6f726620
3000bd60:	6f6e206d 	.word	0x6f6e206d
3000bd64:	65732d6e 	.word	0x65732d6e
3000bd68:	65727563 	.word	0x65727563
3000bd6c:	726f7720 	.word	0x726f7720
3000bd70:	0d21646c 	.word	0x0d21646c
3000bd74:	0000000a 	.word	0x0000000a
3000bd78:	2d73255b 	.word	0x2d73255b
3000bd7c:	53205d41 	.word	0x53205d41
3000bd80:	20525346 	.word	0x20525346
3000bd84:	30203d20 	.word	0x30203d20
3000bd88:	38302578 	.word	0x38302578
3000bd8c:	000a0d78 	.word	0x000a0d78
3000bd90:	2d73255b 	.word	0x2d73255b
3000bd94:	53205d41 	.word	0x53205d41
3000bd98:	20524146 	.word	0x20524146
3000bd9c:	30203d20 	.word	0x30203d20
3000bda0:	38302578 	.word	0x38302578
3000bda4:	000a0d78 	.word	0x000a0d78
3000bda8:	2d73255b 	.word	0x2d73255b
3000bdac:	53205d41 	.word	0x53205d41
3000bdb0:	52534348 	.word	0x52534348
3000bdb4:	30203d20 	.word	0x30203d20
3000bdb8:	38302578 	.word	0x38302578
3000bdbc:	000a0d78 	.word	0x000a0d78
3000bdc0:	2d73255b 	.word	0x2d73255b
3000bdc4:	42205d41 	.word	0x42205d41
3000bdc8:	5f50554b 	.word	0x5f50554b
3000bdcc:	32474552 	.word	0x32474552
3000bdd0:	25783020 	.word	0x25783020
3000bdd4:	0a0d2078 	.word	0x0a0d2078
3000bdd8:	00000000 	.word	0x00000000
3000bddc:	4f4b4e55 	.word	0x4f4b4e55
3000bde0:	00004e57 	.word	0x00004e57
3000bde4:	203a7325 	.word	0x203a7325
3000bde8:	6f636552 	.word	0x6f636552
3000bdec:	79726576 	.word	0x79726576
3000bdf0:	616d6920 	.word	0x616d6920
3000bdf4:	43206567 	.word	0x43206567
3000bdf8:	63204352 	.word	0x63204352
3000bdfc:	6b636568 	.word	0x6b636568
3000be00:	69616620 	.word	0x69616620
3000be04:	72202c6c 	.word	0x72202c6c
3000be08:	79727465 	.word	0x79727465
3000be0c:	6d697420 	.word	0x6d697420
3000be10:	203a7365 	.word	0x203a7365
3000be14:	000a6425 	.word	0x000a6425
3000be18:	203a7325 	.word	0x203a7325
3000be1c:	6f636552 	.word	0x6f636552
3000be20:	79726576 	.word	0x79726576
3000be24:	616d6920 	.word	0x616d6920
3000be28:	43206567 	.word	0x43206567
3000be2c:	63204352 	.word	0x63204352
3000be30:	6b636568 	.word	0x6b636568
3000be34:	69616620 	.word	0x69616620
3000be38:	2064656c 	.word	0x2064656c
3000be3c:	65746661 	.word	0x65746661
3000be40:	20332072 	.word	0x20332072
3000be44:	72746572 	.word	0x72746572
3000be48:	0a736569 	.word	0x0a736569
3000be4c:	00000000 	.word	0x00000000
3000be50:	6f636552 	.word	0x6f636552
3000be54:	79726576 	.word	0x79726576
3000be58:	616d6920 	.word	0x616d6920
3000be5c:	73206567 	.word	0x73206567
3000be60:	63656c65 	.word	0x63656c65
3000be64:	20646574 	.word	0x20646574
3000be68:	66207369 	.word	0x66207369
3000be6c:	6f746361 	.word	0x6f746361
3000be70:	62207972 	.word	0x62207972
3000be74:	72616e69 	.word	0x72616e69
3000be78:	72202c79 	.word	0x72202c79
3000be7c:	766f6365 	.word	0x766f6365
3000be80:	20797265 	.word	0x20797265
3000be84:	6c696166 	.word	0x6c696166
3000be88:	202e6465 	.word	0x202e6465
3000be8c:	6f626552 	.word	0x6f626552
3000be90:	6220746f 	.word	0x6220746f
3000be94:	61772079 	.word	0x61772079
3000be98:	64686374 	.word	0x64686374
3000be9c:	0a0d676f 	.word	0x0a0d676f
3000bea0:	00000000 	.word	0x00000000
3000bea4:	3141544f 	.word	0x3141544f
3000bea8:	45535520 	.word	0x45535520
3000beac:	6576202c 	.word	0x6576202c
3000beb0:	6f697372 	.word	0x6f697372
3000beb4:	25203a6e 	.word	0x25203a6e
3000beb8:	00000a64 	.word	0x00000a64
3000bebc:	3241544f 	.word	0x3241544f
3000bec0:	45535520 	.word	0x45535520
3000bec4:	6576202c 	.word	0x6576202c
3000bec8:	6f697372 	.word	0x6f697372
3000becc:	25203a6e 	.word	0x25203a6e
3000bed0:	00000a64 	.word	0x00000a64
3000bed4:	2d73255b 	.word	0x2d73255b
3000bed8:	49205d49 	.word	0x49205d49
3000bedc:	2074696e 	.word	0x2074696e
3000bee0:	0a0d4257 	.word	0x0a0d4257
3000bee4:	00000000 	.word	0x00000000
3000bee8:	73616c66 	.word	0x73616c66
3000beec:	69735f68 	.word	0x69735f68
3000bef0:	203a657a 	.word	0x203a657a
3000bef4:	000a4d38 	.word	0x000a4d38
3000bef8:	73616c66 	.word	0x73616c66
3000befc:	69735f68 	.word	0x69735f68
3000bf00:	203a657a 	.word	0x203a657a
3000bf04:	0a4d3631 	.word	0x0a4d3631
3000bf08:	00000000 	.word	0x00000000
3000bf0c:	73616c66 	.word	0x73616c66
3000bf10:	69735f68 	.word	0x69735f68
3000bf14:	203a657a 	.word	0x203a657a
3000bf18:	0a4d3233 	.word	0x0a4d3233
3000bf1c:	00000000 	.word	0x00000000
3000bf20:	20315042 	.word	0x20315042
3000bf24:	73726576 	.word	0x73726576
3000bf28:	206e6f69 	.word	0x206e6f69
3000bf2c:	61766e69 	.word	0x61766e69
3000bf30:	0a64696c 	.word	0x0a64696c
3000bf34:	00000000 	.word	0x00000000
3000bf38:	20315042 	.word	0x20315042
3000bf3c:	6d726f66 	.word	0x6d726f66
3000bf40:	69207461 	.word	0x69207461
3000bf44:	6c61766e 	.word	0x6c61766e
3000bf48:	000a6469 	.word	0x000a6469
3000bf4c:	20315042 	.word	0x20315042
3000bf50:	69746361 	.word	0x69746361
3000bf54:	69206576 	.word	0x69206576
3000bf58:	7865646e 	.word	0x7865646e
3000bf5c:	766e6920 	.word	0x766e6920
3000bf60:	64696c61 	.word	0x64696c61
3000bf64:	0000000a 	.word	0x0000000a
3000bf68:	20315042 	.word	0x20315042
3000bf6c:	61746164 	.word	0x61746164
3000bf70:	6c617620 	.word	0x6c617620
3000bf74:	202c6469 	.word	0x202c6469
3000bf78:	73726576 	.word	0x73726576
3000bf7c:	3a6e6f69 	.word	0x3a6e6f69
3000bf80:	000a6425 	.word	0x000a6425
3000bf84:	20325042 	.word	0x20325042
3000bf88:	6d726f66 	.word	0x6d726f66
3000bf8c:	69207461 	.word	0x69207461
3000bf90:	6c61766e 	.word	0x6c61766e
3000bf94:	000a6469 	.word	0x000a6469
3000bf98:	20315042 	.word	0x20315042
3000bf9c:	33435243 	.word	0x33435243
3000bfa0:	616d2032 	.word	0x616d2032
3000bfa4:	2c686374 	.word	0x2c686374
3000bfa8:	74746120 	.word	0x74746120
3000bfac:	65686361 	.word	0x65686361
3000bfb0:	52432064 	.word	0x52432064
3000bfb4:	30203a43 	.word	0x30203a43
3000bfb8:	2c782578 	.word	0x2c782578
3000bfbc:	6c616320 	.word	0x6c616320
3000bfc0:	616c7563 	.word	0x616c7563
3000bfc4:	20646574 	.word	0x20646574
3000bfc8:	3a435243 	.word	0x3a435243
3000bfcc:	25783020 	.word	0x25783020
3000bfd0:	00000a78 	.word	0x00000a78
3000bfd4:	20315042 	.word	0x20315042
3000bfd8:	33435243 	.word	0x33435243
3000bfdc:	6f6e2032 	.word	0x6f6e2032
3000bfe0:	616d2074 	.word	0x616d2074
3000bfe4:	2c686374 	.word	0x2c686374
3000bfe8:	74746120 	.word	0x74746120
3000bfec:	65686361 	.word	0x65686361
3000bff0:	52432064 	.word	0x52432064
3000bff4:	30203a43 	.word	0x30203a43
3000bff8:	2c782578 	.word	0x2c782578
3000bffc:	6c616320 	.word	0x6c616320
3000c000:	616c7563 	.word	0x616c7563
3000c004:	20646574 	.word	0x20646574
3000c008:	3a435243 	.word	0x3a435243
3000c00c:	25783020 	.word	0x25783020
3000c010:	61662078 	.word	0x61662078
3000c014:	64656c69 	.word	0x64656c69
3000c018:	6425203a 	.word	0x6425203a
3000c01c:	0000000a 	.word	0x0000000a
3000c020:	20325042 	.word	0x20325042
3000c024:	33435243 	.word	0x33435243
3000c028:	616d2032 	.word	0x616d2032
3000c02c:	2c686374 	.word	0x2c686374
3000c030:	74746120 	.word	0x74746120
3000c034:	65686361 	.word	0x65686361
3000c038:	52432064 	.word	0x52432064
3000c03c:	30203a43 	.word	0x30203a43
3000c040:	2c782578 	.word	0x2c782578
3000c044:	6c616320 	.word	0x6c616320
3000c048:	616c7563 	.word	0x616c7563
3000c04c:	20646574 	.word	0x20646574
3000c050:	3a435243 	.word	0x3a435243
3000c054:	25783020 	.word	0x25783020
3000c058:	00000a78 	.word	0x00000a78
3000c05c:	20325042 	.word	0x20325042
3000c060:	33435243 	.word	0x33435243
3000c064:	6f6e2032 	.word	0x6f6e2032
3000c068:	616d2074 	.word	0x616d2074
3000c06c:	2c686374 	.word	0x2c686374
3000c070:	74746120 	.word	0x74746120
3000c074:	65686361 	.word	0x65686361
3000c078:	52432064 	.word	0x52432064
3000c07c:	30203a43 	.word	0x30203a43
3000c080:	2c782578 	.word	0x2c782578
3000c084:	6c616320 	.word	0x6c616320
3000c088:	616c7563 	.word	0x616c7563
3000c08c:	20646574 	.word	0x20646574
3000c090:	3a435243 	.word	0x3a435243
3000c094:	25783020 	.word	0x25783020
3000c098:	61662078 	.word	0x61662078
3000c09c:	64656c69 	.word	0x64656c69
3000c0a0:	6425203a 	.word	0x6425203a
3000c0a4:	0000000a 	.word	0x0000000a
3000c0a8:	20315042 	.word	0x20315042
3000c0ac:	3161746f 	.word	0x3161746f
3000c0b0:	64646120 	.word	0x64646120
3000c0b4:	73736572 	.word	0x73736572
3000c0b8:	766e6920 	.word	0x766e6920
3000c0bc:	64696c61 	.word	0x64696c61
3000c0c0:	0000000a 	.word	0x0000000a
3000c0c4:	20315042 	.word	0x20315042
3000c0c8:	3261746f 	.word	0x3261746f
3000c0cc:	64646120 	.word	0x64646120
3000c0d0:	73736572 	.word	0x73736572
3000c0d4:	766e6920 	.word	0x766e6920
3000c0d8:	64696c61 	.word	0x64696c61
3000c0dc:	0000000a 	.word	0x0000000a
3000c0e0:	68746f42 	.word	0x68746f42
3000c0e4:	20504220 	.word	0x20504220
3000c0e8:	20435243 	.word	0x20435243
3000c0ec:	696c6176 	.word	0x696c6176
3000c0f0:	00000a64 	.word	0x00000a64
3000c0f4:	20315042 	.word	0x20315042
3000c0f8:	656c6573 	.word	0x656c6573
3000c0fc:	64657463 	.word	0x64657463
3000c100:	0000000a 	.word	0x0000000a
3000c104:	20325042 	.word	0x20325042
3000c108:	656c6573 	.word	0x656c6573
3000c10c:	64657463 	.word	0x64657463
3000c110:	0000000a 	.word	0x0000000a
3000c114:	68746f42 	.word	0x68746f42
3000c118:	20504220 	.word	0x20504220
3000c11c:	61766e69 	.word	0x61766e69
3000c120:	0a64696c 	.word	0x0a64696c
3000c124:	00000000 	.word	0x00000000
3000c128:	6f636552 	.word	0x6f636552
3000c12c:	79726576 	.word	0x79726576
3000c130:	61747320 	.word	0x61747320
3000c134:	000a7472 	.word	0x000a7472
3000c138:	69746341 	.word	0x69746341
3000c13c:	49206576 	.word	0x49206576
3000c140:	7865646e 	.word	0x7865646e
3000c144:	6425203a 	.word	0x6425203a
3000c148:	0000000a 	.word	0x0000000a
3000c14c:	3141544f 	.word	0x3141544f
3000c150:	766e6920 	.word	0x766e6920
3000c154:	64696c61 	.word	0x64696c61
3000c158:	0000000a 	.word	0x0000000a
3000c15c:	7261500a 	.word	0x7261500a
3000c160:	69746974 	.word	0x69746974
3000c164:	41206e6f 	.word	0x41206e6f
3000c168:	20736920 	.word	0x20736920
3000c16c:	74636166 	.word	0x74636166
3000c170:	2079726f 	.word	0x2079726f
3000c174:	616e6962 	.word	0x616e6962
3000c178:	202c7972 	.word	0x202c7972
3000c17c:	6f636572 	.word	0x6f636572
3000c180:	65726576 	.word	0x65726576
3000c184:	61662064 	.word	0x61662064
3000c188:	64656c69 	.word	0x64656c69
3000c18c:	6552202e 	.word	0x6552202e
3000c190:	746f6f62 	.word	0x746f6f62
3000c194:	20796220 	.word	0x20796220
3000c198:	63746177 	.word	0x63746177
3000c19c:	676f6468 	.word	0x676f6468
3000c1a0:	0000000a 	.word	0x0000000a
3000c1a4:	3241544f 	.word	0x3241544f
3000c1a8:	766e6920 	.word	0x766e6920
3000c1ac:	64696c61 	.word	0x64696c61
3000c1b0:	0000000a 	.word	0x0000000a
3000c1b4:	7261500a 	.word	0x7261500a
3000c1b8:	69746974 	.word	0x69746974
3000c1bc:	42206e6f 	.word	0x42206e6f
3000c1c0:	20736920 	.word	0x20736920
3000c1c4:	74636166 	.word	0x74636166
3000c1c8:	2079726f 	.word	0x2079726f
3000c1cc:	616e6962 	.word	0x616e6962
3000c1d0:	202c7972 	.word	0x202c7972
3000c1d4:	6f636572 	.word	0x6f636572
3000c1d8:	65726576 	.word	0x65726576
3000c1dc:	61662064 	.word	0x61662064
3000c1e0:	64656c69 	.word	0x64656c69
3000c1e4:	6552202e 	.word	0x6552202e
3000c1e8:	746f6f62 	.word	0x746f6f62
3000c1ec:	20796220 	.word	0x20796220
3000c1f0:	63746177 	.word	0x63746177
3000c1f4:	676f6468 	.word	0x676f6468
3000c1f8:	0000000a 	.word	0x0000000a
3000c1fc:	3141544f 	.word	0x3141544f
3000c200:	43524320 	.word	0x43524320
3000c204:	6d203233 	.word	0x6d203233
3000c208:	68637461 	.word	0x68637461
3000c20c:	7461202c 	.word	0x7461202c
3000c210:	68636174 	.word	0x68636174
3000c214:	43206465 	.word	0x43206465
3000c218:	203a4352 	.word	0x203a4352
3000c21c:	78257830 	.word	0x78257830
3000c220:	6163202c 	.word	0x6163202c
3000c224:	6c75636c 	.word	0x6c75636c
3000c228:	64657461 	.word	0x64657461
3000c22c:	43524320 	.word	0x43524320
3000c230:	7830203a 	.word	0x7830203a
3000c234:	000a7825 	.word	0x000a7825
3000c238:	3141544f 	.word	0x3141544f
3000c23c:	766e6920 	.word	0x766e6920
3000c240:	64696c61 	.word	0x64696c61
3000c244:	69616620 	.word	0x69616620
3000c248:	3a64656c 	.word	0x3a64656c
3000c24c:	0a642520 	.word	0x0a642520
3000c250:	00000000 	.word	0x00000000
3000c254:	3141544f 	.word	0x3141544f
3000c258:	43524320 	.word	0x43524320
3000c25c:	6e203233 	.word	0x6e203233
3000c260:	6d20746f 	.word	0x6d20746f
3000c264:	68637461 	.word	0x68637461
3000c268:	7461202c 	.word	0x7461202c
3000c26c:	68636174 	.word	0x68636174
3000c270:	43206465 	.word	0x43206465
3000c274:	203a4352 	.word	0x203a4352
3000c278:	78257830 	.word	0x78257830
3000c27c:	6163202c 	.word	0x6163202c
3000c280:	6c75636c 	.word	0x6c75636c
3000c284:	64657461 	.word	0x64657461
3000c288:	43524320 	.word	0x43524320
3000c28c:	7830203a 	.word	0x7830203a
3000c290:	66207825 	.word	0x66207825
3000c294:	656c6961 	.word	0x656c6961
3000c298:	25203a64 	.word	0x25203a64
3000c29c:	00000a64 	.word	0x00000a64
3000c2a0:	3141544f 	.word	0x3141544f
3000c2a4:	43524320 	.word	0x43524320
3000c2a8:	6e203233 	.word	0x6e203233
3000c2ac:	6d20746f 	.word	0x6d20746f
3000c2b0:	68637461 	.word	0x68637461
3000c2b4:	7461202c 	.word	0x7461202c
3000c2b8:	68636174 	.word	0x68636174
3000c2bc:	43206465 	.word	0x43206465
3000c2c0:	203a4352 	.word	0x203a4352
3000c2c4:	78257830 	.word	0x78257830
3000c2c8:	6163202c 	.word	0x6163202c
3000c2cc:	6c75636c 	.word	0x6c75636c
3000c2d0:	64657461 	.word	0x64657461
3000c2d4:	43524320 	.word	0x43524320
3000c2d8:	7830203a 	.word	0x7830203a
3000c2dc:	202c7825 	.word	0x202c7825
3000c2e0:	73617265 	.word	0x73617265
3000c2e4:	544f2065 	.word	0x544f2065
3000c2e8:	61203141 	.word	0x61203141
3000c2ec:	72657466 	.word	0x72657466
3000c2f0:	69616620 	.word	0x69616620
3000c2f4:	2064656c 	.word	0x2064656c
3000c2f8:	65747461 	.word	0x65747461
3000c2fc:	7374706d 	.word	0x7374706d
3000c300:	0000000a 	.word	0x0000000a
3000c304:	3241544f 	.word	0x3241544f
3000c308:	43524320 	.word	0x43524320
3000c30c:	6d203233 	.word	0x6d203233
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
3000c33c:	000a7825 	.word	0x000a7825
3000c340:	3241544f 	.word	0x3241544f
3000c344:	766e6920 	.word	0x766e6920
3000c348:	64696c61 	.word	0x64696c61
3000c34c:	69616620 	.word	0x69616620
3000c350:	3a64656c 	.word	0x3a64656c
3000c354:	0a642520 	.word	0x0a642520
3000c358:	00000000 	.word	0x00000000
3000c35c:	3241544f 	.word	0x3241544f
3000c360:	43524320 	.word	0x43524320
3000c364:	6e203233 	.word	0x6e203233
3000c368:	6d20746f 	.word	0x6d20746f
3000c36c:	68637461 	.word	0x68637461
3000c370:	7461202c 	.word	0x7461202c
3000c374:	68636174 	.word	0x68636174
3000c378:	43206465 	.word	0x43206465
3000c37c:	203a4352 	.word	0x203a4352
3000c380:	78257830 	.word	0x78257830
3000c384:	6163202c 	.word	0x6163202c
3000c388:	6c75636c 	.word	0x6c75636c
3000c38c:	64657461 	.word	0x64657461
3000c390:	43524320 	.word	0x43524320
3000c394:	7830203a 	.word	0x7830203a
3000c398:	66207825 	.word	0x66207825
3000c39c:	656c6961 	.word	0x656c6961
3000c3a0:	25203a64 	.word	0x25203a64
3000c3a4:	00000a64 	.word	0x00000a64
3000c3a8:	3241544f 	.word	0x3241544f
3000c3ac:	43524320 	.word	0x43524320
3000c3b0:	6e203233 	.word	0x6e203233
3000c3b4:	6d20746f 	.word	0x6d20746f
3000c3b8:	68637461 	.word	0x68637461
3000c3bc:	7461202c 	.word	0x7461202c
3000c3c0:	68636174 	.word	0x68636174
3000c3c4:	43206465 	.word	0x43206465
3000c3c8:	203a4352 	.word	0x203a4352
3000c3cc:	78257830 	.word	0x78257830
3000c3d0:	6163202c 	.word	0x6163202c
3000c3d4:	6c75636c 	.word	0x6c75636c
3000c3d8:	64657461 	.word	0x64657461
3000c3dc:	43524320 	.word	0x43524320
3000c3e0:	7830203a 	.word	0x7830203a
3000c3e4:	202c7825 	.word	0x202c7825
3000c3e8:	73617265 	.word	0x73617265
3000c3ec:	544f2065 	.word	0x544f2065
3000c3f0:	61203241 	.word	0x61203241
3000c3f4:	72657466 	.word	0x72657466
3000c3f8:	69616620 	.word	0x69616620
3000c3fc:	2064656c 	.word	0x2064656c
3000c400:	65747461 	.word	0x65747461
3000c404:	7374706d 	.word	0x7374706d
3000c408:	0000000a 	.word	0x0000000a
3000c40c:	3141544f 	.word	0x3141544f
3000c410:	766e6920 	.word	0x766e6920
3000c414:	64696c61 	.word	0x64696c61
3000c418:	0a642520 	.word	0x0a642520
3000c41c:	00000000 	.word	0x00000000
3000c420:	3241544f 	.word	0x3241544f
3000c424:	766e6920 	.word	0x766e6920
3000c428:	64696c61 	.word	0x64696c61
3000c42c:	0a642520 	.word	0x0a642520
3000c430:	00000000 	.word	0x00000000
3000c434:	68746f42 	.word	0x68746f42
3000c438:	41544f20 	.word	0x41544f20
3000c43c:	616d6920 	.word	0x616d6920
3000c440:	76206567 	.word	0x76206567
3000c444:	66697265 	.word	0x66697265
3000c448:	61662079 	.word	0x61662079
3000c44c:	64656c69 	.word	0x64656c69
3000c450:	0000000a 	.word	0x0000000a
3000c454:	69746341 	.word	0x69746341
3000c458:	69206576 	.word	0x69206576
3000c45c:	7865646e 	.word	0x7865646e
3000c460:	41544f20 	.word	0x41544f20
3000c464:	766e6920 	.word	0x766e6920
3000c468:	64696c61 	.word	0x64696c61
3000c46c:	6572202c 	.word	0x6572202c
3000c470:	74697277 	.word	0x74697277
3000c474:	63612065 	.word	0x63612065
3000c478:	65766974 	.word	0x65766974
3000c47c:	646e6920 	.word	0x646e6920
3000c480:	74207865 	.word	0x74207865
3000c484:	6425206f 	.word	0x6425206f
3000c488:	0000000a 	.word	0x0000000a
3000c48c:	206c6c41 	.word	0x206c6c41
3000c490:	2041544f 	.word	0x2041544f
3000c494:	72646461 	.word	0x72646461
3000c498:	65737365 	.word	0x65737365
3000c49c:	6f6e2073 	.word	0x6f6e2073
3000c4a0:	61762074 	.word	0x61762074
3000c4a4:	2c64696c 	.word	0x2c64696c
3000c4a8:	63657220 	.word	0x63657220
3000c4ac:	7265766f 	.word	0x7265766f
3000c4b0:	61662079 	.word	0x61662079
3000c4b4:	000a6c69 	.word	0x000a6c69
3000c4b8:	20325042 	.word	0x20325042
3000c4bc:	3261746f 	.word	0x3261746f
3000c4c0:	64646120 	.word	0x64646120
3000c4c4:	73736572 	.word	0x73736572
3000c4c8:	766e6920 	.word	0x766e6920
3000c4cc:	64696c61 	.word	0x64696c61
3000c4d0:	0000000a 	.word	0x0000000a
3000c4d4:	20325042 	.word	0x20325042
3000c4d8:	73726576 	.word	0x73726576
3000c4dc:	206e6f69 	.word	0x206e6f69
3000c4e0:	61766e69 	.word	0x61766e69
3000c4e4:	0a64696c 	.word	0x0a64696c
3000c4e8:	00000000 	.word	0x00000000
3000c4ec:	20325042 	.word	0x20325042
3000c4f0:	69746361 	.word	0x69746361
3000c4f4:	69206576 	.word	0x69206576
3000c4f8:	7865646e 	.word	0x7865646e
3000c4fc:	766e6920 	.word	0x766e6920
3000c500:	64696c61 	.word	0x64696c61
3000c504:	0000000a 	.word	0x0000000a
3000c508:	20325042 	.word	0x20325042
3000c50c:	3161746f 	.word	0x3161746f
3000c510:	64646120 	.word	0x64646120
3000c514:	73736572 	.word	0x73736572
3000c518:	766e6920 	.word	0x766e6920
3000c51c:	64696c61 	.word	0x64696c61
3000c520:	0000000a 	.word	0x0000000a
3000c524:	20325042 	.word	0x20325042
3000c528:	61746164 	.word	0x61746164
3000c52c:	6c617620 	.word	0x6c617620
3000c530:	202c6469 	.word	0x202c6469
3000c534:	73726576 	.word	0x73726576
3000c538:	3a6e6f69 	.word	0x3a6e6f69
3000c53c:	000a6425 	.word	0x000a6425
3000c540:	2d73255b 	.word	0x2d73255b
3000c544:	44205d49 	.word	0x44205d49
3000c548:	62617369 	.word	0x62617369
3000c54c:	4b20656c 	.word	0x4b20656c
3000c550:	4220304d 	.word	0x4220304d
3000c554:	20746f6f 	.word	0x20746f6f
3000c558:	206e6920 	.word	0x206e6920
3000c55c:	74736554 	.word	0x74736554
3000c560:	646f6d20 	.word	0x646f6d20
3000c564:	000a0d65 	.word	0x000a0d65
3000c568:	2d73255b 	.word	0x2d73255b
3000c56c:	42205d49 	.word	0x42205d49
3000c570:	73617079 	.word	0x73617079
3000c574:	544f2073 	.word	0x544f2073
3000c578:	75612050 	.word	0x75612050
3000c57c:	6f6c6f74 	.word	0x6f6c6f74
3000c580:	0a0d6461 	.word	0x0a0d6461
3000c584:	00000000 	.word	0x00000000
3000c588:	2d73255b 	.word	0x2d73255b
3000c58c:	4e205d49 	.word	0x4e205d49
3000c590:	72462050 	.word	0x72462050
3000c594:	25207165 	.word	0x25207165
3000c598:	484d2064 	.word	0x484d2064
3000c59c:	00000a7a 	.word	0x00000a7a
3000c5a0:	2d73255b 	.word	0x2d73255b
3000c5a4:	41205d49 	.word	0x41205d49
3000c5a8:	72462050 	.word	0x72462050
3000c5ac:	25207165 	.word	0x25207165
3000c5b0:	484d2064 	.word	0x484d2064
3000c5b4:	00000a7a 	.word	0x00000a7a
3000c5b8:	31474d49 	.word	0x31474d49
3000c5bc:	4f425320 	.word	0x4f425320
3000c5c0:	4f20544f 	.word	0x4f20544f
3000c5c4:	000a4646 	.word	0x000a4646
3000c5c8:	31474d49 	.word	0x31474d49
3000c5cc:	4f425320 	.word	0x4f425320
3000c5d0:	5320544f 	.word	0x5320544f
3000c5d4:	45434355 	.word	0x45434355
3000c5d8:	000a5353 	.word	0x000a5353
3000c5dc:	746f6f62 	.word	0x746f6f62
3000c5e0:	64616f6c 	.word	0x64616f6c
3000c5e4:	765f7265 	.word	0x765f7265
3000c5e8:	69737265 	.word	0x69737265
3000c5ec:	25206e6f 	.word	0x25206e6f
3000c5f0:	00000a73 	.word	0x00000a73
3000c5f4:	444f4d5b 	.word	0x444f4d5b
3000c5f8:	5f454c55 	.word	0x5f454c55
3000c5fc:	544f4f42 	.word	0x544f4f42
3000c600:	56454c2d 	.word	0x56454c2d
3000c604:	495f4c45 	.word	0x495f4c45
3000c608:	5d4f464e 	.word	0x5d4f464e
3000c60c:	474d493a 	.word	0x474d493a
3000c610:	4e452031 	.word	0x4e452031
3000c614:	20524554 	.word	0x20524554
3000c618:	3a50534d 	.word	0x3a50534d
3000c61c:	3830255b 	.word	0x3830255b
3000c620:	000a5d78 	.word	0x000a5d78
3000c624:	444f4d5b 	.word	0x444f4d5b
3000c628:	5f454c55 	.word	0x5f454c55
3000c62c:	544f4f42 	.word	0x544f4f42
3000c630:	56454c2d 	.word	0x56454c2d
3000c634:	495f4c45 	.word	0x495f4c45
3000c638:	5d4f464e 	.word	0x5d4f464e
3000c63c:	474d493a 	.word	0x474d493a
3000c640:	45532031 	.word	0x45532031
3000c644:	45525543 	.word	0x45525543
3000c648:	41545320 	.word	0x41545320
3000c64c:	203a4554 	.word	0x203a4554
3000c650:	000a6425 	.word	0x000a6425
3000c654:	2d73255b 	.word	0x2d73255b
3000c658:	49205d49 	.word	0x49205d49
3000c65c:	2074696e 	.word	0x2074696e
3000c660:	41525350 	.word	0x41525350
3000c664:	000a0d4d 	.word	0x000a0d4d
3000c668:	2d73255b 	.word	0x2d73255b
3000c66c:	73205d57 	.word	0x73205d57
3000c670:	70207465 	.word	0x70207465
3000c674:	66206d66 	.word	0x66206d66
3000c678:	0d6c6961 	.word	0x0d6c6961
3000c67c:	0000000a 	.word	0x0000000a
3000c680:	2d73255b 	.word	0x2d73255b
3000c684:	45205d45 	.word	0x45205d45
3000c688:	524f5252 	.word	0x524f5252
3000c68c:	53202121 	.word	0x53202121
3000c690:	6c756f68 	.word	0x6c756f68
3000c694:	6f4e2064 	.word	0x6f4e2064
3000c698:	6e652074 	.word	0x6e652074
3000c69c:	656c6261 	.word	0x656c6261
3000c6a0:	6d654d20 	.word	0x6d654d20
3000c6a4:	5f727753 	.word	0x5f727753
3000c6a8:	796c6e4f 	.word	0x796c6e4f
3000c6ac:	206e6920 	.word	0x206e6920
3000c6b0:	20524444 	.word	0x20524444
3000c6b4:	70696843 	.word	0x70696843
3000c6b8:	0d212121 	.word	0x0d212121
3000c6bc:	0000000a 	.word	0x0000000a
3000c6c0:	2d73255b 	.word	0x2d73255b
3000c6c4:	49205d49 	.word	0x49205d49
3000c6c8:	2074696e 	.word	0x2074696e
3000c6cc:	0d524444 	.word	0x0d524444
3000c6d0:	0000000a 	.word	0x0000000a
3000c6d4:	2d73255b 	.word	0x2d73255b
3000c6d8:	42205d49 	.word	0x42205d49
3000c6dc:	5f50554b 	.word	0x5f50554b
3000c6e0:	32474552 	.word	0x32474552
3000c6e4:	25783020 	.word	0x25783020
3000c6e8:	0a0d2078 	.word	0x0a0d2078
3000c6ec:	00000000 	.word	0x00000000
3000c6f0:	55676f4c 	.word	0x55676f4c
3000c6f4:	20747261 	.word	0x20747261
3000c6f8:	64756142 	.word	0x64756142
3000c6fc:	65746172 	.word	0x65746172
3000c700:	6425203a 	.word	0x6425203a
3000c704:	0000000a 	.word	0x0000000a
3000c708:	726f4e0a 	.word	0x726f4e0a
3000c70c:	206c616d 	.word	0x206c616d
3000c710:	746f6f62 	.word	0x746f6f62
3000c714:	0000000a 	.word	0x0000000a
3000c718:	5f315042 	.word	0x5f315042
3000c71c:	3141544f 	.word	0x3141544f
3000c720:	00000000 	.word	0x00000000
3000c724:	5f315042 	.word	0x5f315042
3000c728:	3241544f 	.word	0x3241544f
3000c72c:	00000000 	.word	0x00000000
3000c730:	5f325042 	.word	0x5f325042
3000c734:	3141544f 	.word	0x3141544f
3000c738:	00000000 	.word	0x00000000
3000c73c:	5f325042 	.word	0x5f325042
3000c740:	3241544f 	.word	0x3241544f
3000c744:	00000000 	.word	0x00000000
3000c748:	20304d4b 	.word	0x20304d4b
3000c74c:	20504958 	.word	0x20504958
3000c750:	00474d49 	.word	0x00474d49
3000c754:	20304d4b 	.word	0x20304d4b
3000c758:	4d415253 	.word	0x4d415253
3000c75c:	00000000 	.word	0x00000000
3000c760:	20304d4b 	.word	0x20304d4b
3000c764:	4d415244 	.word	0x4d415244
3000c768:	00000000 	.word	0x00000000
3000c76c:	20344d4b 	.word	0x20344d4b
3000c770:	20504958 	.word	0x20504958
3000c774:	00474d49 	.word	0x00474d49
3000c778:	20344d4b 	.word	0x20344d4b
3000c77c:	4d415253 	.word	0x4d415253
3000c780:	00000000 	.word	0x00000000
3000c784:	20344d4b 	.word	0x20344d4b
3000c788:	4d415244 	.word	0x4d415244
3000c78c:	00000000 	.word	0x00000000
3000c790:	58205041 	.word	0x58205041
3000c794:	49205049 	.word	0x49205049
3000c798:	0000474d 	.word	0x0000474d
3000c79c:	42205041 	.word	0x42205041
3000c7a0:	5320314c 	.word	0x5320314c
3000c7a4:	004d4152 	.word	0x004d4152
3000c7a8:	42205041 	.word	0x42205041
3000c7ac:	4420314c 	.word	0x4420314c
3000c7b0:	004d4152 	.word	0x004d4152
3000c7b4:	46205041 	.word	0x46205041
3000c7b8:	00005049 	.word	0x00005049
3000c7bc:	2d73255b 	.word	0x2d73255b
3000c7c0:	25205d49 	.word	0x25205d49
3000c7c4:	6e492073 	.word	0x6e492073
3000c7c8:	696c6176 	.word	0x696c6176
3000c7cc:	00000a64 	.word	0x00000a64
3000c7d0:	2d73255b 	.word	0x2d73255b
3000c7d4:	25205d49 	.word	0x25205d49
3000c7d8:	30255b73 	.word	0x30255b73
3000c7dc:	253a7838 	.word	0x253a7838
3000c7e0:	000a5d78 	.word	0x000a5d78
3000c7e4:	2d73255b 	.word	0x2d73255b
3000c7e8:	52205d49 	.word	0x52205d49
3000c7ec:	45205044 	.word	0x45205044
3000c7f0:	00000a4e 	.word	0x00000a4e
3000c7f4:	2d73255b 	.word	0x2d73255b
3000c7f8:	46205d49 	.word	0x46205d49
3000c7fc:	206c6961 	.word	0x206c6961
3000c800:	6c206f74 	.word	0x6c206f74
3000c804:	2064616f 	.word	0x2064616f
3000c808:	20504452 	.word	0x20504452
3000c80c:	67616d69 	.word	0x67616d69
3000c810:	000a2165 	.word	0x000a2165
3000c814:	2d73255b 	.word	0x2d73255b
3000c818:	52205d49 	.word	0x52205d49
3000c81c:	4f205044 	.word	0x4f205044
3000c820:	000a4646 	.word	0x000a4646
3000c824:	2d73255b 	.word	0x2d73255b
3000c828:	49205d49 	.word	0x49205d49
3000c82c:	2032474d 	.word	0x2032474d
3000c830:	2046544f 	.word	0x2046544f
3000c834:	000a4e45 	.word	0x000a4e45
3000c838:	2d73255b 	.word	0x2d73255b
3000c83c:	4f205d45 	.word	0x4f205d45
3000c840:	4d204654 	.word	0x4d204654
3000c844:	2065646f 	.word	0x2065646f
3000c848:	6f727265 	.word	0x6f727265
3000c84c:	000a0d72 	.word	0x000a0d72
3000c850:	494c4156 	.word	0x494c4156
3000c854:	00000044 	.word	0x00000044
3000c858:	41564e49 	.word	0x41564e49
3000c85c:	0044494c 	.word	0x0044494c
3000c860:	74726563 	.word	0x74726563
3000c864:	63696669 	.word	0x63696669
3000c868:	20657461 	.word	0x20657461
3000c86c:	20434345 	.word	0x20434345
3000c870:	69726576 	.word	0x69726576
3000c874:	66207966 	.word	0x66207966
3000c878:	2c6c6961 	.word	0x2c6c6961
3000c87c:	6c6f7220 	.word	0x6c6f7220
3000c880:	6361626c 	.word	0x6361626c
3000c884:	000a0d6b 	.word	0x000a0d6b
3000c888:	444f4d5b 	.word	0x444f4d5b
3000c88c:	5f454c55 	.word	0x5f454c55
3000c890:	544f4f42 	.word	0x544f4f42
3000c894:	56454c2d 	.word	0x56454c2d
3000c898:	495f4c45 	.word	0x495f4c45
3000c89c:	5d4f464e 	.word	0x5d4f464e
3000c8a0:	474d493a 	.word	0x474d493a
3000c8a4:	544f2832 	.word	0x544f2832
3000c8a8:	29642541 	.word	0x29642541
3000c8ac:	2c732520 	.word	0x2c732520
3000c8b0:	74657220 	.word	0x74657220
3000c8b4:	6425203a 	.word	0x6425203a
3000c8b8:	0000000a 	.word	0x0000000a
3000c8bc:	32474d49 	.word	0x32474d49
3000c8c0:	43434520 	.word	0x43434520
3000c8c4:	72657620 	.word	0x72657620
3000c8c8:	20796669 	.word	0x20796669
3000c8cc:	6c696166 	.word	0x6c696166
3000c8d0:	6f72202c 	.word	0x6f72202c
3000c8d4:	61626c6c 	.word	0x61626c6c
3000c8d8:	0a0d6b63 	.word	0x0a0d6b63
3000c8dc:	00000000 	.word	0x00000000
3000c8e0:	5f41544f 	.word	0x5f41544f
3000c8e4:	45205041 	.word	0x45205041
3000c8e8:	76204343 	.word	0x76204343
3000c8ec:	66697265 	.word	0x66697265
3000c8f0:	61662079 	.word	0x61662079
3000c8f4:	202c6c69 	.word	0x202c6c69
3000c8f8:	6c6c6f72 	.word	0x6c6c6f72
3000c8fc:	6b636162 	.word	0x6b636162
3000c900:	00000a0d 	.word	0x00000a0d
3000c904:	64616f4c 	.word	0x64616f4c
3000c908:	41544f20 	.word	0x41544f20
3000c90c:	2050415f 	.word	0x2050415f
3000c910:	67616d69 	.word	0x67616d69
3000c914:	61662065 	.word	0x61662065
3000c918:	202c6c69 	.word	0x202c6c69
3000c91c:	6c6c6f72 	.word	0x6c6c6f72
3000c920:	6b636162 	.word	0x6b636162
3000c924:	00000a0d 	.word	0x00000a0d
3000c928:	444f4d5b 	.word	0x444f4d5b
3000c92c:	5f454c55 	.word	0x5f454c55
3000c930:	544f4f42 	.word	0x544f4f42
3000c934:	56454c2d 	.word	0x56454c2d
3000c938:	455f4c45 	.word	0x455f4c45
3000c93c:	524f5252 	.word	0x524f5252
3000c940:	61463a5d 	.word	0x61463a5d
3000c944:	74206c69 	.word	0x74206c69
3000c948:	6f6c206f 	.word	0x6f6c206f
3000c94c:	41206461 	.word	0x41206461
3000c950:	6d692050 	.word	0x6d692050
3000c954:	21656761 	.word	0x21656761
3000c958:	0000000a 	.word	0x0000000a
3000c95c:	20344d4b 	.word	0x20344d4b
3000c960:	33474d49 	.word	0x33474d49
3000c964:	00000000 	.word	0x00000000
3000c968:	20344d4b 	.word	0x20344d4b
3000c96c:	0043534e 	.word	0x0043534e
3000c970:	31474d49 	.word	0x31474d49
3000c974:	00000000 	.word	0x00000000
3000c978:	32474d49 	.word	0x32474d49
3000c97c:	00000000 	.word	0x00000000
3000c980:	42205041 	.word	0x42205041
3000c984:	4626314c 	.word	0x4626314c
3000c988:	00005049 	.word	0x00005049
3000c98c:	444f4d5b 	.word	0x444f4d5b
3000c990:	5f454c55 	.word	0x5f454c55
3000c994:	544f4f42 	.word	0x544f4f42
3000c998:	56454c2d 	.word	0x56454c2d
3000c99c:	495f4c45 	.word	0x495f4c45
3000c9a0:	5d4f464e 	.word	0x5d4f464e
3000c9a4:	5b73253a 	.word	0x5b73253a
3000c9a8:	78383025 	.word	0x78383025
3000c9ac:	5d78253a 	.word	0x5d78253a
3000c9b0:	0000000a 	.word	0x0000000a
3000c9b4:	20504452 	.word	0x20504452
3000c9b8:	4c494146 	.word	0x4c494146
3000c9bc:	0000000a 	.word	0x0000000a
3000c9c0:	20504452 	.word	0x20504452
3000c9c4:	43435553 	.word	0x43435553
3000c9c8:	0a535345 	.word	0x0a535345
3000c9cc:	00000000 	.word	0x00000000
3000c9d0:	059e4eb3 	.word	0x059e4eb3
3000c9d4:	4b11ea39 	.word	0x4b11ea39
3000c9d8:	2a1b920a 	.word	0x2a1b920a
3000c9dc:	29939cee 	.word	0x29939cee
3000c9e0:	00000000 	.word	0x00000000
3000c9e4:	42205041 	.word	0x42205041
3000c9e8:	0032334c 	.word	0x0032334c
3000c9ec:	2d73255b 	.word	0x2d73255b
3000c9f0:	25205d49 	.word	0x25205d49
3000c9f4:	45562073 	.word	0x45562073
3000c9f8:	59464952 	.word	0x59464952
3000c9fc:	53415020 	.word	0x53415020
3000ca00:	00000a53 	.word	0x00000a53
3000ca04:	2d73255b 	.word	0x2d73255b
3000ca08:	25205d45 	.word	0x25205d45
3000ca0c:	45562073 	.word	0x45562073
3000ca10:	59464952 	.word	0x59464952
3000ca14:	49414620 	.word	0x49414620
3000ca18:	72202c4c 	.word	0x72202c4c
3000ca1c:	3d207465 	.word	0x3d207465
3000ca20:	0a642520 	.word	0x0a642520
3000ca24:	00000000 	.word	0x00000000
3000ca28:	2d73255b 	.word	0x2d73255b
3000ca2c:	43205d45 	.word	0x43205d45
3000ca30:	69747265 	.word	0x69747265
3000ca34:	61636966 	.word	0x61636966
3000ca38:	56206574 	.word	0x56206574
3000ca3c:	46495245 	.word	0x46495245
3000ca40:	41462059 	.word	0x41462059
3000ca44:	202c4c49 	.word	0x202c4c49
3000ca48:	20746572 	.word	0x20746572
3000ca4c:	6425203d 	.word	0x6425203d
3000ca50:	0000000a 	.word	0x0000000a
3000ca54:	50494843 	.word	0x50494843
3000ca58:	4f464e49 	.word	0x4f464e49
3000ca5c:	00000000 	.word	0x00000000
3000ca60:	2d73255b 	.word	0x2d73255b
3000ca64:	49205d45 	.word	0x49205d45
3000ca68:	6c61766e 	.word	0x6c61766e
3000ca6c:	43206469 	.word	0x43206469
3000ca70:	696e6968 	.word	0x696e6968
3000ca74:	216f666e 	.word	0x216f666e
3000ca78:	65684320 	.word	0x65684320
3000ca7c:	4f206b63 	.word	0x4f206b63
3000ca80:	000a5054 	.word	0x000a5054
3000ca84:	2d73255b 	.word	0x2d73255b
3000ca88:	50205d57 	.word	0x50205d57
3000ca8c:	7361656c 	.word	0x7361656c
3000ca90:	72702065 	.word	0x72702065
3000ca94:	6172676f 	.word	0x6172676f
3000ca98:	6863206d 	.word	0x6863206d
3000ca9c:	6e697069 	.word	0x6e697069
3000caa0:	69206f66 	.word	0x69206f66
3000caa4:	544f206e 	.word	0x544f206e
3000caa8:	0a212050 	.word	0x0a212050
3000caac:	00000000 	.word	0x00000000

3000cab0 <Memory_Info>:
3000cab0:	03e00007 00060004 010202c0 02010100     ................
3000cac0:	02000102 00040302 02040400 01010102     ................
3000cad0:	01020221 02240401 03000102 00030103     !.....$.........
3000cae0:	01050500 06000002 01020206 01070700     ................
3000caf0:	08000002 01020208 02280801 09000102     ..........(.....
3000cb00:	00040309 03290901 0a000004 0003010a     ......).........
3000cb10:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000cb20:	0d000102 0004030d 010e0e00 ffff0003     ................
3000cb30:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000cb40:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000cb50:	62696c61 6f696172 000a0d6e              alibraion...

3000cb5c <ddrphy_tx_scan>:
3000cb5c:	00000000 00080000 00080808 00000000     ................
3000cb6c:	20000000 33333333 000c0022 00000000     ... 3333".......
3000cb7c:	07060012 000f0707 080f0f0f 00000008     ................
3000cb8c:	20000001 22222222 000c0012 34000000     ... """".......4
3000cb9c:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000cbac:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cbbc:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000cbcc:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cbdc:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000cbec:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cbfc:	0e0e001a 00170e0e 12171717 00000012     ................
3000cc0c:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000cc1c:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000cc2c:	200003c1 22222222 000c0012 34000000     ... """".......4
3000cc3c:	0f0f001b 00180f0f 13181818 00000013     ................
3000cc4c:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000cc5c:	1110001b 00191111 13191919 00000013     ................
3000cc6c:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000cc7c:	1313001c 001c1313 121c1c1c 00000012     ................
3000cc8c:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000cc9c <ddrphy_zq_rx_scan>:
	...
3000cca4:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000ccb4:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000ccc4:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000ccd4:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000cce4:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000ccf4:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000cd04:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000cd14:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000cd24:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000cd34:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000cd44:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000cd54:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000cd64 <Flash_AVL>:
3000cd64:	000000ef 000000ff 00000000 000043fc     .............C..
3000cd74:	00000000 000000a1 000000ff 00000000     ................
3000cd84:	0000fffc 00000000 0000000b 000000ff     ................
3000cd94:	00000000 000043fc 00000000 0000000e     .....C..........
3000cda4:	000000ff 00000000 000043fc 00000000     .........C......
3000cdb4:	000000c8 000000ff 00000001 000043fc     .............C..
3000cdc4:	00000000 000028c2 0000ffff 00000005     .....(..........
3000cdd4:	000200fc 00000000 000000c2 000000ff     ................
3000cde4:	00000002 000000fc 00000000 00000068     ............h...
3000cdf4:	000000ff 00000002 000000fc 00000000     ................
3000ce04:	00000051 000000ff 00000002 000000fc     Q...............
3000ce14:	00000000 0000001c 000000ff 00000003     ................
3000ce24:	000000fc 00000000 00000020 000000ff     ........ .......
3000ce34:	00000004 000000fc 00000000 00000000     ................
3000ce44:	000000ff 000000fe ffffffff 30007cd5     .............|.0
3000ce54:	000000ff ffffffff 000000ff ffffffff     ................
3000ce64:	00000000                                ....

3000ce68 <Flash_ReadMode>:
3000ce68:	                                         ..

3000ce6a <Flash_Speed>:
3000ce6a:	                                         ..

3000ce6c <NAND_AVL>:
3000ce6c:	000000ef 000000ff 00000000 ffffffff     ................
3000ce7c:	00000000 000000c8 000000ff 00000001     ................
3000ce8c:	ffffffff 00000000 000000e5 000000ff     ................
3000ce9c:	00000001 ffffffff 00000000 000000c2     ................
3000ceac:	000000ff 00000002 ffffffff 00000000     ................
3000cebc:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000ced4:	000000ff 000000fe ffffffff 30007d35     ............5}.0
3000cee4:	000000ff ffffffff 000000ff ffffffff     ................
3000cef4:	00000000 3000bbb0 3000bbb4 3000bbb8     .......0...0...0
3000cf04:	3000bbbc 3000bbc0                       ...0...0

3000cf0c <__FUNCTION__.0>:
3000cf0c:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000cf1c <__FUNCTION__.1>:
3000cf1c:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000cf2c:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000cf3c:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000cf4c:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000cf5c:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000cf6c:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000cf7c:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000cf8c:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000cf9c:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000cfac:	00000a20                                 ...

3000cfb0 <__FUNCTION__.1>:
3000cfb0:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000cfc0:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000cfd0:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000cfe0:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000cff0:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000d000:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000d010:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000d020:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000d030:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000d040:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000d050:	726f6620 41485320 00000a32               for SHA2...

3000d05c <__FUNCTION__.1>:
3000d05c:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d06c <__FUNCTION__.3>:
3000d06c:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d07c:	32616873 696e695f 00000074              sha2_init...

3000d088 <__FUNCTION__.4>:
3000d088:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d098:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000d0a8:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000d0b8:	6e207369 6320746f 69666e6f 65727567     is not configure
3000d0c8:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000d0d8:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000d0e8:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000d0f8:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000d108:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000d118:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000d128:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000d138:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000d148:	69625f6b 21217374 00000a21              k_bits!!!...

3000d154 <__FUNCTION__.0>:
3000d154:	33697872 445f3631 72536e79 6e695f65     rxi316_DynSre_in
3000d164:	00007469                                it..

3000d168 <__FUNCTION__.1>:
3000d168:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d178:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000d188:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000d198:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000d1a8:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000d1b8:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000d1c8:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000d1d8:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000d1e8:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000d1f8:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000d208:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000d218:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000d228:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000d238:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000d248:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000d258:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000d268:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000d278:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000d288:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000d298:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000d2a8:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000d2b8:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000d2c8:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000d2d8:	00000000                                ....

3000d2dc <register_string>:
3000d2dc:	00000000 3000d204 3000bcd0 3000bcd4     .......0...0...0
3000d2ec:	3000bcd8 3000bcdc 3000bce0 3000bce4     ...0...0...0...0
3000d2fc:	3000bce8 3000d200 3000bcec 3000bcf0     ...0...0...0...0
3000d30c:	3000bcf4 3000d2a4 65637845 6f697470     ...0...0Exceptio
3000d31c:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000d32c:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000d33c:	64657375 20796220 6c696166 76206465     used by failed v
3000d34c:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000d35c:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000d36c:	00000000 75636553 79746972 75616620     ....Security fau
3000d37c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d38c:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000d39c:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000d3ac:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000d3bc:	79746972 75616620 6920746c 61632073     rity fault is ca
3000d3cc:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000d3dc:	65746e69 74697267 69732079 74616e67     integrity signat
3000d3ec:	00657275 75636553 79746972 75616620     ure.Security fau
3000d3fc:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d40c:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000d41c:	6572206e 6e727574 00000000 75636553     n return....Secu
3000d42c:	79746972 75616620 6920746c 61632073     rity fault is ca
3000d43c:	64657375 20796220 72747441 74756269     used by Attribut
3000d44c:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000d45c:	00006e6f 75636553 79746972 75616620     on..Security fau
3000d46c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d47c:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000d48c:	00006e6f 75636553 79746972 75616620     on..Security fau
3000d49c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000d4ac:	797a614c 61747320 70206574 65736572     Lazy state prese
3000d4bc:	74617672 206e6f69 6f727265 00000072     rvation error...
3000d4cc:	75636553 79746972 75616620 6920746c     Security fault i
3000d4dc:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000d4ec:	61747320 65206574 726f7272 00000000      state error....
3000d4fc:	20656854 75636573 79746972 75616620     The security fau
3000d50c:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000d51c:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000d52c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d53c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000d54c:	61662074 20746c75 63207369 65737561     t fault is cause
3000d55c:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000d56c:	63636120 20737365 6c6f6976 6f697461      access violatio
3000d57c:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000d58c:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000d59c:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000d5ac:	20746c75 63207369 65737561 79622064     ult is caused by
3000d5bc:	74616420 63612061 73736563 6f697620      data access vio
3000d5cc:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000d5dc:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000d5ec:	63207369 65737561 79622064 736e7520     is caused by uns
3000d5fc:	6b636174 20676e69 6f727265 00000072     tacking error...
3000d60c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000d61c:	61662074 20746c75 63207369 65737561     t fault is cause
3000d62c:	79622064 61747320 6e696b63 72652067     d by stacking er
3000d63c:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000d64c:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000d65c:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000d66c:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000d67c:	70206574 65736572 74617672 006e6f69     te preservation.
3000d68c:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000d69c:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000d6ac:	64657272 64646120 73736572 00736920     rred address is.
3000d6bc:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000d6cc:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000d6dc:	73692074 75616320 20646573 69207962     t is caused by i
3000d6ec:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000d6fc:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000d70c:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000d71c:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000d72c:	75616320 20646573 70207962 69636572      caused by preci
3000d73c:	64206573 20617461 65636361 76207373     se data access v
3000d74c:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000d75c:	6c756166 73692074 75616320 20646573     fault is caused 
3000d76c:	69207962 6572706d 65736963 74616420     by imprecise dat
3000d77c:	63612061 73736563 6f697620 6974616c     a access violati
3000d78c:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000d79c:	75616320 20646573 75207962 6174736e      caused by unsta
3000d7ac:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000d7bc:	6c756166 73692074 75616320 20646573     fault is caused 
3000d7cc:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000d7dc:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000d7ec:	75616320 20646573 66207962 74616f6c      caused by float
3000d7fc:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000d80c:	65746174 65727020 76726573 6f697461     tate preservatio
3000d81c:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000d82c:	636f2074 72727563 61206465 65726464     t occurred addre
3000d83c:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000d84c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d85c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000d86c:	65737561 79622064 74746120 74706d65     aused by attempt
3000d87c:	6f742073 65786520 65747563 206e6120     s to execute an 
3000d88c:	65646e75 656e6966 6e692064 75727473     undefined instru
3000d89c:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000d8ac:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d8bc:	67617355 61662065 20746c75 63207369     Usage fault is c
3000d8cc:	65737561 79622064 74746120 74706d65     aused by attempt
3000d8dc:	6f742073 69777320 20686374 61206f74     s to switch to a
3000d8ec:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000d8fc:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000d90c:	61662065 20746c75 63207369 65737561     e fault is cause
3000d91c:	79622064 74746120 74706d65 6f742073     d by attempts to
3000d92c:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000d93c:	74697720 20612068 20646162 756c6176      with a bad valu
3000d94c:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000d95c:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000d96c:	61662065 20746c75 63207369 65737561     e fault is cause
3000d97c:	79622064 74746120 74706d65 6f742073     d by attempts to
3000d98c:	65786520 65747563 63206120 6f72706f      execute a copro
3000d99c:	73736563 6920726f 7274736e 69746375     cessor instructi
3000d9ac:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000d9bc:	63207369 65737561 79622064 646e6920     is caused by ind
3000d9cc:	74616369 74207365 20746168 74732061     icates that a st
3000d9dc:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000d9ec:	61776472 63206572 6b636568 61682029     rdware check) ha
3000d9fc:	61742073 206e656b 63616c70 00000065     s taken place...
3000da0c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000da1c:	65737561 79622064 646e6920 74616369     aused by indicat
3000da2c:	74207365 20746168 75206e61 696c616e     es that an unali
3000da3c:	64656e67 63636120 20737365 6c756166     gned access faul
3000da4c:	61682074 61742073 206e656b 63616c70     t has taken plac
3000da5c:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000da6c:	63207369 65737561 79622064 646e4920     is caused by Ind
3000da7c:	74616369 61207365 76696420 20656469     icates a divide 
3000da8c:	7a207962 206f7265 20736168 656b6174     by zero has take
3000da9c:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000daac:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000dabc:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000dacc:	75626544 61662067 20746c75 63207369     Debug fault is c
3000dadc:	65737561 79622064 6c616820 65722074     aused by halt re
3000daec:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000dafc:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000db0c:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000db1c:	20746c75 63207369 65737561 79622064     ult is caused by
3000db2c:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000db3c:	7865206e 74756365 00006465 75626544     n executed..Debu
3000db4c:	61662067 20746c75 63207369 65737561     g fault is cause
3000db5c:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000db6c:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000db7c:	20746c75 63207369 65737561 79622064     ult is caused by
3000db8c:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000db9c:	72727563 00006465 75626544 61662067     curred..Debug fa
3000dbac:	20746c75 63207369 65737561 79622064     ult is caused by
3000dbbc:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000dbcc:	72657373 00646574 00000208 0000020c     sserted.........
3000dbdc:	00000210 00000008 00000214 00000218     ................
3000dbec:	0000021c 0000000c                       ........

3000dbf4 <CSWTCH.373>:
3000dbf4:	3000c718 3000c724 3000c730 3000c73c     ...0$..00..0<..0

3000dc04 <__FUNCTION__.0>:
3000dc04:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.
3000dc14:	3000c748 3000c754 3000c760 3000c76c     H..0T..0`..0l..0
3000dc24:	3000c778 3000c784 3000c790 3000c79c     x..0...0...0...0
3000dc34:	3000c7a8 3000c7b4 3000c95c 3000c968     ...0...0\..0h..0
3000dc44:	3000c970 3000c978 3000c960 3000c980     p..0x..0`..0...0

3000dc54 <km4_bootloader_rev>:
3000dc54:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000dc64:	305f7265 32353464 64396236 30325f38     er_0d4526b9d8_20
3000dc74:	302f3632 33322f34 3a38312d 313a3030     26/04/23-18:00:1
3000dc84:	4f420031 5b00544f 572d7325 2a2a205d     1.BOOT.[%s-W] **
3000dc94:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000dca4:	2a2a2a2a 2a2a2a2a 52415720 474e494e     ******** WARNING
3000dcb4:	2a2a2a20 2a2a2a2a 2a2a2a2a 2a2a2a2a      ***************
3000dcc4:	2a2a2a2a 2a2a2a2a 255b000a 5d572d73     ********..[%s-W]
3000dcd4:	20202a20 20202020 20202020 20202020      *              
3000dce4:	20202020 20202020 20202020 20202020                     
3000dcf4:	20202020 20202020 20202020 20202020                     
3000dd04:	20202020 20202020 0a2a2020 73255b00               *..[%s
3000dd14:	205d572d 2020202a 20202020 20202020     -W] *           
3000dd24:	75432020 6e657272 68632074 76207069       Current chip v
3000dd34:	69737265 69206e6f 43412073 21217475     ersion is ACut!!
3000dd44:	20202021 20202020 20202020 000a2a20     !            *..
3000dd54:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000dd64:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000dd74:	2074726f 72727563 20746e65 73726576     ort current vers
3000dd84:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000dd94:	5b000a2a 572d7325 202a205d 20202020     *..[%s-W] *     
3000dda4:	656c5020 20657361 746e6f63 20746361      Please contact 
3000ddb4:	204b5452 66204d50 6d20726f 2065726f     RTK PM for more 
3000ddc4:	646f7270 20746375 6f666e69 20202021     product info!   
3000ddd4:	2a202020 255b000a 5d572d73 2a2a2a20        *..[%s-W] ***
3000dde4:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000ddf4:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000de04:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000de14:	2a2a2a2a 0a2a2a2a 73255b00 205d572d     *******..[%s-W] 
3000de24:	0043000a 49534f50 002e0058              ..C.POSIX...

3000de30 <_ctype_>:
3000de30:	20202000 20202020 28282020 20282828     .         ((((( 
3000de40:	20202020 20202020 20202020 20202020                     
3000de50:	10108820 10101010 10101010 10101010      ...............
3000de60:	04040410 04040404 10040404 10101010     ................
3000de70:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000de80:	01010101 01010101 01010101 10101010     ................
3000de90:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000dea0:	02020202 02020202 02020202 10101010     ................
3000deb0:	00000020 00000000 00000000 00000000      ...............
	...
