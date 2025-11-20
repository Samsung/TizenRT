
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
300032c4:	30008ef5 	.word	0x30008ef5
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	30008d7d 	.word	0x30008d7d
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
300033fc:	30008ef5 	.word	0x30008ef5
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	30008d7d 	.word	0x30008d7d
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
3000349c:	f007 fec6 	bl	3000b22c <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000b89c 	.word	0x3000b89c
300034bc:	3000b8a8 	.word	0x3000b8a8

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
300034f0:	f007 fe9c 	bl	3000b22c <rtk_log_write>
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
30003524:	f007 fe82 	bl	3000b22c <rtk_log_write>
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
30003540:	3000b89c 	.word	0x3000b89c
30003544:	3000b8ec 	.word	0x3000b8ec
30003548:	30008ef5 	.word	0x30008ef5
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000b8cc 	.word	0x3000b8cc
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
3000365a:	f007 fde7 	bl	3000b22c <rtk_log_write>
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
3000367a:	f007 fdd7 	bl	3000b22c <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000c908 	.word	0x3000c908
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000b89c 	.word	0x3000b89c
300036a8:	3000b924 	.word	0x3000b924
300036ac:	3000b90c 	.word	0x3000b90c
300036b0:	30008ef5 	.word	0x30008ef5
300036b4:	30008d7d 	.word	0x30008d7d
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
3000373e:	f007 fd75 	bl	3000b22c <rtk_log_write>
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
30003790:	f007 fd4c 	bl	3000b22c <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 f81d 	bl	3000b7d8 <__io_assert_failed_veneer>
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
300037ca:	f007 fd2f 	bl	3000b22c <rtk_log_write>
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
30003816:	f007 fd09 	bl	3000b22c <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f007 ffda 	bl	3000b7d8 <__io_assert_failed_veneer>
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
30003958:	3000c878 	.word	0x3000c878
3000395c:	3000c87a 	.word	0x3000c87a
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000b89c 	.word	0x3000b89c
3000396c:	3000b948 	.word	0x3000b948
30003970:	3000c774 	.word	0x3000c774
30003974:	3000c87c 	.word	0x3000c87c
30003978:	3000b964 	.word	0x3000b964
3000397c:	3000c92c 	.word	0x3000c92c
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000b990 	.word	0x3000b990
30003988:	3000c91c 	.word	0x3000c91c
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000ed31 	.word	0x3000ed31
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	30008ef5 	.word	0x30008ef5
300039b8:	3000ed34 	.word	0x3000ed34
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
30003a36:	f007 fecf 	bl	3000b7d8 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f007 fec6 	bl	3000b7d8 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000ed31 	.word	0x3000ed31
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000c91c 	.word	0x3000c91c
30003a78:	3000c92c 	.word	0x3000c92c
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
30003aac:	f007 fbbe 	bl	3000b22c <rtk_log_write>
30003ab0:	2244      	movs	r2, #68	; 0x44
30003ab2:	2100      	movs	r1, #0
30003ab4:	a803      	add	r0, sp, #12
30003ab6:	f007 fe2f 	bl	3000b718 <____wrap_memset_veneer>
30003aba:	0672      	lsls	r2, r6, #25
30003abc:	4b39      	ldr	r3, [pc, #228]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003abe:	d55f      	bpl.n	30003b80 <BOOT_FaultHandler+0xe8>
30003ac0:	4619      	mov	r1, r3
30003ac2:	4a3a      	ldr	r2, [pc, #232]	; (30003bac <BOOT_FaultHandler+0x114>)
30003ac4:	2001      	movs	r0, #1
30003ac6:	f007 fbb1 	bl	3000b22c <rtk_log_write>
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
30003b14:	f007 f8b4 	bl	3000ac80 <crash_dump>
30003b18:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30003b1c:	4b21      	ldr	r3, [pc, #132]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b1e:	2001      	movs	r0, #1
30003b20:	9100      	str	r1, [sp, #0]
30003b22:	4a24      	ldr	r2, [pc, #144]	; (30003bb4 <BOOT_FaultHandler+0x11c>)
30003b24:	4619      	mov	r1, r3
30003b26:	f007 fb81 	bl	3000b22c <rtk_log_write>
30003b2a:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30003b2e:	4b1d      	ldr	r3, [pc, #116]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b30:	2001      	movs	r0, #1
30003b32:	9200      	str	r2, [sp, #0]
30003b34:	4619      	mov	r1, r3
30003b36:	4a20      	ldr	r2, [pc, #128]	; (30003bb8 <BOOT_FaultHandler+0x120>)
30003b38:	f007 fb78 	bl	3000b22c <rtk_log_write>
30003b3c:	6a61      	ldr	r1, [r4, #36]	; 0x24
30003b3e:	4b19      	ldr	r3, [pc, #100]	; (30003ba4 <BOOT_FaultHandler+0x10c>)
30003b40:	2001      	movs	r0, #1
30003b42:	9100      	str	r1, [sp, #0]
30003b44:	4a1d      	ldr	r2, [pc, #116]	; (30003bbc <BOOT_FaultHandler+0x124>)
30003b46:	4619      	mov	r1, r3
30003b48:	f007 fb70 	bl	3000b22c <rtk_log_write>
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
30003b6c:	f007 fb5e 	bl	3000b22c <rtk_log_write>
30003b70:	463b      	mov	r3, r7
30003b72:	4632      	mov	r2, r6
30003b74:	4629      	mov	r1, r5
30003b76:	4620      	mov	r0, r4
30003b78:	f007 fdc6 	bl	3000b708 <__INT_HardFault_C_veneer>
30003b7c:	b015      	add	sp, #84	; 0x54
30003b7e:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003b80:	4a12      	ldr	r2, [pc, #72]	; (30003bcc <BOOT_FaultHandler+0x134>)
30003b82:	2001      	movs	r0, #1
30003b84:	4619      	mov	r1, r3
30003b86:	f007 fb51 	bl	3000b22c <rtk_log_write>
30003b8a:	f3ef 8394 	mrs	r3, CONTROL_NS
30003b8e:	f3ef 8e88 	mrs	lr, MSP_NS
30003b92:	f3ef 8589 	mrs	r5, PSP_NS
30003b96:	f016 0108 	ands.w	r1, r6, #8
30003b9a:	bf18      	it	ne
30003b9c:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30003ba0:	e79b      	b.n	30003ada <BOOT_FaultHandler+0x42>
30003ba2:	bf00      	nop
30003ba4:	3000b9d0 	.word	0x3000b9d0
30003ba8:	3000b9d8 	.word	0x3000b9d8
30003bac:	3000b9fc 	.word	0x3000b9fc
30003bb0:	e000ed00 	.word	0xe000ed00
30003bb4:	3000ba50 	.word	0x3000ba50
30003bb8:	3000ba68 	.word	0x3000ba68
30003bbc:	3000ba80 	.word	0x3000ba80
30003bc0:	0000994d 	.word	0x0000994d
30003bc4:	00009979 	.word	0x00009979
30003bc8:	3000ba98 	.word	0x3000ba98
30003bcc:	3000ba24 	.word	0x3000ba24

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

30003c78 <flash_read_id>:
30003c78:	b538      	push	{r3, r4, r5, lr}
30003c7a:	4d04      	ldr	r5, [pc, #16]	; (30003c8c <flash_read_id+0x14>)
30003c7c:	460c      	mov	r4, r1
30003c7e:	4602      	mov	r2, r0
30003c80:	4b03      	ldr	r3, [pc, #12]	; (30003c90 <flash_read_id+0x18>)
30003c82:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30003c86:	4798      	blx	r3
30003c88:	4620      	mov	r0, r4
30003c8a:	bd38      	pop	{r3, r4, r5, pc}
30003c8c:	2001c01c 	.word	0x2001c01c
30003c90:	00009db5 	.word	0x00009db5

30003c94 <mcc_ReverseBit>:
30003c94:	0843      	lsrs	r3, r0, #1
30003c96:	08c1      	lsrs	r1, r0, #3
30003c98:	f003 0308 	and.w	r3, r3, #8
30003c9c:	0942      	lsrs	r2, r0, #5
30003c9e:	f001 0104 	and.w	r1, r1, #4
30003ca2:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30003ca6:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30003caa:	f002 0202 	and.w	r2, r2, #2
30003cae:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
30003cb2:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30003cb6:	430b      	orrs	r3, r1
30003cb8:	00c1      	lsls	r1, r0, #3
30003cba:	4313      	orrs	r3, r2
30003cbc:	f001 0120 	and.w	r1, r1, #32
30003cc0:	0042      	lsls	r2, r0, #1
30003cc2:	ea43 000c 	orr.w	r0, r3, ip
30003cc6:	f002 0310 	and.w	r3, r2, #16
30003cca:	4308      	orrs	r0, r1
30003ccc:	4318      	orrs	r0, r3
30003cce:	b2c0      	uxtb	r0, r0
30003cd0:	4770      	bx	lr
30003cd2:	bf00      	nop

30003cd4 <mcc_init_crc32>:
30003cd4:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30003cd6:	2600      	movs	r6, #0
30003cd8:	4f16      	ldr	r7, [pc, #88]	; (30003d34 <mcc_init_crc32+0x60>)
30003cda:	4605      	mov	r5, r0
30003cdc:	b2f0      	uxtb	r0, r6
30003cde:	f7ff ffd9 	bl	30003c94 <mcc_ReverseBit>
30003ce2:	2208      	movs	r2, #8
30003ce4:	0604      	lsls	r4, r0, #24
30003ce6:	2c00      	cmp	r4, #0
30003ce8:	ea4f 0444 	mov.w	r4, r4, lsl #1
30003cec:	bfb8      	it	lt
30003cee:	407c      	eorlt	r4, r7
30003cf0:	3a01      	subs	r2, #1
30003cf2:	d1f8      	bne.n	30003ce6 <mcc_init_crc32+0x12>
30003cf4:	0e20      	lsrs	r0, r4, #24
30003cf6:	3504      	adds	r5, #4
30003cf8:	f7ff ffcc 	bl	30003c94 <mcc_ReverseBit>
30003cfc:	4603      	mov	r3, r0
30003cfe:	f3c4 4007 	ubfx	r0, r4, #16, #8
30003d02:	3601      	adds	r6, #1
30003d04:	f805 3c04 	strb.w	r3, [r5, #-4]
30003d08:	f7ff ffc4 	bl	30003c94 <mcc_ReverseBit>
30003d0c:	4603      	mov	r3, r0
30003d0e:	f3c4 2007 	ubfx	r0, r4, #8, #8
30003d12:	f805 3c03 	strb.w	r3, [r5, #-3]
30003d16:	f7ff ffbd 	bl	30003c94 <mcc_ReverseBit>
30003d1a:	4603      	mov	r3, r0
30003d1c:	b2e0      	uxtb	r0, r4
30003d1e:	f805 3c02 	strb.w	r3, [r5, #-2]
30003d22:	f7ff ffb7 	bl	30003c94 <mcc_ReverseBit>
30003d26:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30003d2a:	f805 0c01 	strb.w	r0, [r5, #-1]
30003d2e:	d1d5      	bne.n	30003cdc <mcc_init_crc32+0x8>
30003d30:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003d32:	bf00      	nop
30003d34:	04c11db7 	.word	0x04c11db7

30003d38 <mcc_crc32>:
30003d38:	b179      	cbz	r1, 30003d5a <mcc_crc32+0x22>
30003d3a:	eb00 0c01 	add.w	ip, r0, r1
30003d3e:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30003d42:	f810 3b01 	ldrb.w	r3, [r0], #1
30003d46:	404b      	eors	r3, r1
30003d48:	4560      	cmp	r0, ip
30003d4a:	b2db      	uxtb	r3, r3
30003d4c:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30003d50:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30003d54:	d1f5      	bne.n	30003d42 <mcc_crc32+0xa>
30003d56:	43c8      	mvns	r0, r1
30003d58:	4770      	bx	lr
30003d5a:	4608      	mov	r0, r1
30003d5c:	4770      	bx	lr
30003d5e:	bf00      	nop

30003d60 <recovery_check>:
30003d60:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30003d64:	4283      	cmp	r3, r0
30003d66:	d322      	bcc.n	30003dae <recovery_check+0x4e>
30003d68:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
30003d6c:	d91f      	bls.n	30003dae <recovery_check+0x4e>
30003d6e:	b510      	push	{r4, lr}
30003d70:	f8d0 400a 	ldr.w	r4, [r0, #10]
30003d74:	f104 030c 	add.w	r3, r4, #12
30003d78:	4299      	cmp	r1, r3
30003d7a:	d316      	bcc.n	30003daa <recovery_check+0x4a>
30003d7c:	f100 0c04 	add.w	ip, r0, #4
30003d80:	b183      	cbz	r3, 30003da4 <recovery_check+0x44>
30003d82:	f100 0e10 	add.w	lr, r0, #16
30003d86:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30003d8a:	44a6      	add	lr, r4
30003d8c:	4c0b      	ldr	r4, [pc, #44]	; (30003dbc <recovery_check+0x5c>)
30003d8e:	f81c 3b01 	ldrb.w	r3, [ip], #1
30003d92:	404b      	eors	r3, r1
30003d94:	45f4      	cmp	ip, lr
30003d96:	b2db      	uxtb	r3, r3
30003d98:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003d9c:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30003da0:	d1f5      	bne.n	30003d8e <recovery_check+0x2e>
30003da2:	43cb      	mvns	r3, r1
30003da4:	6801      	ldr	r1, [r0, #0]
30003da6:	4299      	cmp	r1, r3
30003da8:	d003      	beq.n	30003db2 <recovery_check+0x52>
30003daa:	2000      	movs	r0, #0
30003dac:	bd10      	pop	{r4, pc}
30003dae:	2000      	movs	r0, #0
30003db0:	4770      	bx	lr
30003db2:	f8d0 3006 	ldr.w	r3, [r0, #6]
30003db6:	2001      	movs	r0, #1
30003db8:	6013      	str	r3, [r2, #0]
30003dba:	bd10      	pop	{r4, pc}
30003dbc:	3000db04 	.word	0x3000db04

30003dc0 <rewrite_bp>:
30003dc0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30003dc4:	b0a3      	sub	sp, #140	; 0x8c
30003dc6:	4616      	mov	r6, r2
30003dc8:	4607      	mov	r7, r0
30003dca:	460d      	mov	r5, r1
30003dcc:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
30003dd0:	4699      	mov	r9, r3
30003dd2:	f10d 0808 	add.w	r8, sp, #8
30003dd6:	2280      	movs	r2, #128	; 0x80
30003dd8:	2c00      	cmp	r4, #0
30003dda:	f000 80b2 	beq.w	30003f42 <rewrite_bp+0x182>
30003dde:	4619      	mov	r1, r3
30003de0:	4640      	mov	r0, r8
30003de2:	f007 fc81 	bl	3000b6e8 <____wrap_memcpy_veneer>
30003de6:	2301      	movs	r3, #1
30003de8:	f109 0404 	add.w	r4, r9, #4
30003dec:	4649      	mov	r1, r9
30003dee:	2002      	movs	r0, #2
30003df0:	f88d 7014 	strb.w	r7, [sp, #20]
30003df4:	e9cd 3303 	strd	r3, r3, [sp, #12]
30003df8:	f7ff feea 	bl	30003bd0 <FLASH_Erase_With_Lock>
30003dfc:	4620      	mov	r0, r4
30003dfe:	217c      	movs	r1, #124	; 0x7c
30003e00:	aa03      	add	r2, sp, #12
30003e02:	f7ff ff0d 	bl	30003c20 <FLASH_TxData_With_Lock>
30003e06:	4620      	mov	r0, r4
30003e08:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
30003e0c:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30003e10:	4c54      	ldr	r4, [pc, #336]	; (30003f64 <rewrite_bp+0x1a4>)
30003e12:	f810 cb01 	ldrb.w	ip, [r0], #1
30003e16:	ea8c 0c03 	eor.w	ip, ip, r3
30003e1a:	4288      	cmp	r0, r1
30003e1c:	fa5f fc8c 	uxtb.w	ip, ip
30003e20:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
30003e24:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30003e28:	d1f3      	bne.n	30003e12 <rewrite_bp+0x52>
30003e2a:	43db      	mvns	r3, r3
30003e2c:	4649      	mov	r1, r9
30003e2e:	2002      	movs	r0, #2
30003e30:	9302      	str	r3, [sp, #8]
30003e32:	f7ff fecd 	bl	30003bd0 <FLASH_Erase_With_Lock>
30003e36:	4642      	mov	r2, r8
30003e38:	4648      	mov	r0, r9
30003e3a:	2180      	movs	r1, #128	; 0x80
30003e3c:	f7ff fef0 	bl	30003c20 <FLASH_TxData_With_Lock>
30003e40:	4b49      	ldr	r3, [pc, #292]	; (30003f68 <rewrite_bp+0x1a8>)
30003e42:	aa01      	add	r2, sp, #4
30003e44:	2104      	movs	r1, #4
30003e46:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30003e4a:	4b48      	ldr	r3, [pc, #288]	; (30003f6c <rewrite_bp+0x1ac>)
30003e4c:	4798      	blx	r3
30003e4e:	f89d 3006 	ldrb.w	r3, [sp, #6]
30003e52:	f04f 0c01 	mov.w	ip, #1
30003e56:	f8d5 e00a 	ldr.w	lr, [r5, #10]
30003e5a:	682a      	ldr	r2, [r5, #0]
30003e5c:	fa0c fc03 	lsl.w	ip, ip, r3
30003e60:	f10e 0e0c 	add.w	lr, lr, #12
30003e64:	f8d6 300a 	ldr.w	r3, [r6, #10]
30003e68:	45e6      	cmp	lr, ip
30003e6a:	6831      	ldr	r1, [r6, #0]
30003e6c:	f103 030c 	add.w	r3, r3, #12
30003e70:	bf28      	it	cs
30003e72:	46e6      	movcs	lr, ip
30003e74:	459c      	cmp	ip, r3
30003e76:	bf28      	it	cs
30003e78:	469c      	movcs	ip, r3
30003e7a:	bb6f      	cbnz	r7, 30003ed8 <rewrite_bp+0x118>
30003e7c:	1d28      	adds	r0, r5, #4
30003e7e:	f1be 0f00 	cmp.w	lr, #0
30003e82:	d00e      	beq.n	30003ea2 <rewrite_bp+0xe2>
30003e84:	4486      	add	lr, r0
30003e86:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003e8a:	f810 3b01 	ldrb.w	r3, [r0], #1
30003e8e:	407b      	eors	r3, r7
30003e90:	4570      	cmp	r0, lr
30003e92:	b2db      	uxtb	r3, r3
30003e94:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003e98:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003e9c:	d1f5      	bne.n	30003e8a <rewrite_bp+0xca>
30003e9e:	ea6f 0e07 	mvn.w	lr, r7
30003ea2:	4572      	cmp	r2, lr
30003ea4:	d042      	beq.n	30003f2c <rewrite_bp+0x16c>
30003ea6:	1d32      	adds	r2, r6, #4
30003ea8:	f1bc 0f00 	cmp.w	ip, #0
30003eac:	d00e      	beq.n	30003ecc <rewrite_bp+0x10c>
30003eae:	4494      	add	ip, r2
30003eb0:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003eb4:	f812 3b01 	ldrb.w	r3, [r2], #1
30003eb8:	407b      	eors	r3, r7
30003eba:	4562      	cmp	r2, ip
30003ebc:	b2db      	uxtb	r3, r3
30003ebe:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003ec2:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003ec6:	d1f5      	bne.n	30003eb4 <rewrite_bp+0xf4>
30003ec8:	ea6f 0c07 	mvn.w	ip, r7
30003ecc:	4561      	cmp	r1, ip
30003ece:	d03d      	beq.n	30003f4c <rewrite_bp+0x18c>
30003ed0:	4827      	ldr	r0, [pc, #156]	; (30003f70 <rewrite_bp+0x1b0>)
30003ed2:	f007 fc49 	bl	3000b768 <__DiagPrintf_veneer>
30003ed6:	e7fe      	b.n	30003ed6 <rewrite_bp+0x116>
30003ed8:	1d30      	adds	r0, r6, #4
30003eda:	f1bc 0f00 	cmp.w	ip, #0
30003ede:	d00e      	beq.n	30003efe <rewrite_bp+0x13e>
30003ee0:	4484      	add	ip, r0
30003ee2:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003ee6:	f810 3b01 	ldrb.w	r3, [r0], #1
30003eea:	407b      	eors	r3, r7
30003eec:	4560      	cmp	r0, ip
30003eee:	b2db      	uxtb	r3, r3
30003ef0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003ef4:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003ef8:	d1f5      	bne.n	30003ee6 <rewrite_bp+0x126>
30003efa:	ea6f 0c07 	mvn.w	ip, r7
30003efe:	4561      	cmp	r1, ip
30003f00:	d024      	beq.n	30003f4c <rewrite_bp+0x18c>
30003f02:	1d29      	adds	r1, r5, #4
30003f04:	f1be 0f00 	cmp.w	lr, #0
30003f08:	d00e      	beq.n	30003f28 <rewrite_bp+0x168>
30003f0a:	448e      	add	lr, r1
30003f0c:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003f10:	f811 3b01 	ldrb.w	r3, [r1], #1
30003f14:	407b      	eors	r3, r7
30003f16:	4571      	cmp	r1, lr
30003f18:	b2db      	uxtb	r3, r3
30003f1a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003f1e:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003f22:	d1f5      	bne.n	30003f10 <rewrite_bp+0x150>
30003f24:	ea6f 0e07 	mvn.w	lr, r7
30003f28:	4572      	cmp	r2, lr
30003f2a:	d1d1      	bne.n	30003ed0 <rewrite_bp+0x110>
30003f2c:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
30003f30:	4810      	ldr	r0, [pc, #64]	; (30003f74 <rewrite_bp+0x1b4>)
30003f32:	f8d5 1006 	ldr.w	r1, [r5, #6]
30003f36:	f007 fc17 	bl	3000b768 <__DiagPrintf_veneer>
30003f3a:	2001      	movs	r0, #1
30003f3c:	b023      	add	sp, #140	; 0x8c
30003f3e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003f42:	992a      	ldr	r1, [sp, #168]	; 0xa8
30003f44:	4640      	mov	r0, r8
30003f46:	f007 fbcf 	bl	3000b6e8 <____wrap_memcpy_veneer>
30003f4a:	e74c      	b.n	30003de6 <rewrite_bp+0x26>
30003f4c:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
30003f50:	4809      	ldr	r0, [pc, #36]	; (30003f78 <rewrite_bp+0x1b8>)
30003f52:	f8d6 1006 	ldr.w	r1, [r6, #6]
30003f56:	f007 fc07 	bl	3000b768 <__DiagPrintf_veneer>
30003f5a:	2002      	movs	r0, #2
30003f5c:	b023      	add	sp, #140	; 0x8c
30003f5e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003f62:	bf00      	nop
30003f64:	3000db04 	.word	0x3000db04
30003f68:	2001c01c 	.word	0x2001c01c
30003f6c:	00009db5 	.word	0x00009db5
30003f70:	3000bae4 	.word	0x3000bae4
30003f74:	3000bab4 	.word	0x3000bab4
30003f78:	3000bacc 	.word	0x3000bacc

30003f7c <BOOT_SectionInit>:
30003f7c:	4800      	ldr	r0, [pc, #0]	; (30003f80 <BOOT_SectionInit+0x4>)
30003f7e:	4770      	bx	lr
30003f80:	60000020 	.word	0x60000020

30003f84 <BOOT_NsStart>:
30003f84:	b570      	push	{r4, r5, r6, lr}
30003f86:	4c23      	ldr	r4, [pc, #140]	; (30004014 <BOOT_NsStart+0x90>)
30003f88:	4605      	mov	r5, r0
30003f8a:	f384 8808 	msr	MSP, r4
30003f8e:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003f92:	2100      	movs	r1, #0
30003f94:	4620      	mov	r0, r4
30003f96:	4e20      	ldr	r6, [pc, #128]	; (30004018 <BOOT_NsStart+0x94>)
30003f98:	47b0      	blx	r6
30003f9a:	4a20      	ldr	r2, [pc, #128]	; (3000401c <BOOT_NsStart+0x98>)
30003f9c:	6953      	ldr	r3, [r2, #20]
30003f9e:	03d9      	lsls	r1, r3, #15
30003fa0:	d50b      	bpl.n	30003fba <BOOT_NsStart+0x36>
30003fa2:	f3bf 8f4f 	dsb	sy
30003fa6:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
30003faa:	3420      	adds	r4, #32
30003fac:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
30003fb0:	d1f9      	bne.n	30003fa6 <BOOT_NsStart+0x22>
30003fb2:	f3bf 8f4f 	dsb	sy
30003fb6:	f3bf 8f6f 	isb	sy
30003fba:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
30003fbe:	f383 8808 	msr	MSP, r3
30003fc2:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003fc6:	2100      	movs	r1, #0
30003fc8:	4815      	ldr	r0, [pc, #84]	; (30004020 <BOOT_NsStart+0x9c>)
30003fca:	47b0      	blx	r6
30003fcc:	4a13      	ldr	r2, [pc, #76]	; (3000401c <BOOT_NsStart+0x98>)
30003fce:	6953      	ldr	r3, [r2, #20]
30003fd0:	03db      	lsls	r3, r3, #15
30003fd2:	d50c      	bpl.n	30003fee <BOOT_NsStart+0x6a>
30003fd4:	f3bf 8f4f 	dsb	sy
30003fd8:	4b11      	ldr	r3, [pc, #68]	; (30004020 <BOOT_NsStart+0x9c>)
30003fda:	490e      	ldr	r1, [pc, #56]	; (30004014 <BOOT_NsStart+0x90>)
30003fdc:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30003fe0:	3320      	adds	r3, #32
30003fe2:	428b      	cmp	r3, r1
30003fe4:	d1fa      	bne.n	30003fdc <BOOT_NsStart+0x58>
30003fe6:	f3bf 8f4f 	dsb	sy
30003fea:	f3bf 8f6f 	isb	sy
30003fee:	f025 0501 	bic.w	r5, r5, #1
30003ff2:	086d      	lsrs	r5, r5, #1
30003ff4:	006d      	lsls	r5, r5, #1
30003ff6:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30003ffa:	ed2d 8b10 	vpush	{d8-d15}
30003ffe:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
30004002:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
30004006:	47ac      	blxns	r5
30004008:	ecbd 8b10 	vpop	{d8-d15}
3000400c:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004010:	bd70      	pop	{r4, r5, r6, pc}
30004012:	bf00      	nop
30004014:	30002000 	.word	0x30002000
30004018:	00012be5 	.word	0x00012be5
3000401c:	e000ed00 	.word	0xe000ed00
30004020:	30001000 	.word	0x30001000

30004024 <BOOT_RccConfig>:
30004024:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004028:	4c2f      	ldr	r4, [pc, #188]	; (300040e8 <BOOT_RccConfig+0xc4>)
3000402a:	b090      	sub	sp, #64	; 0x40
3000402c:	4f2f      	ldr	r7, [pc, #188]	; (300040ec <BOOT_RccConfig+0xc8>)
3000402e:	466d      	mov	r5, sp
30004030:	f10d 0c20 	add.w	ip, sp, #32
30004034:	683e      	ldr	r6, [r7, #0]
30004036:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
3000403a:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
3000403e:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30004042:	f104 0310 	add.w	r3, r4, #16
30004046:	2400      	movs	r4, #0
30004048:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000404a:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
3000404e:	1c72      	adds	r2, r6, #1
30004050:	e9cd 4404 	strd	r4, r4, [sp, #16]
30004054:	e9cd 4406 	strd	r4, r4, [sp, #24]
30004058:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000405c:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30004060:	d03f      	beq.n	300040e2 <BOOT_RccConfig+0xbe>
30004062:	463a      	mov	r2, r7
30004064:	6891      	ldr	r1, [r2, #8]
30004066:	0fb3      	lsrs	r3, r6, #30
30004068:	a810      	add	r0, sp, #64	; 0x40
3000406a:	2901      	cmp	r1, #1
3000406c:	eb00 0383 	add.w	r3, r0, r3, lsl #2
30004070:	d10e      	bne.n	30004090 <BOOT_RccConfig+0x6c>
30004072:	f853 0c30 	ldr.w	r0, [r3, #-48]
30004076:	6854      	ldr	r4, [r2, #4]
30004078:	4330      	orrs	r0, r6
3000407a:	ae10      	add	r6, sp, #64	; 0x40
3000407c:	0fa1      	lsrs	r1, r4, #30
3000407e:	f843 0c30 	str.w	r0, [r3, #-48]
30004082:	eb06 0181 	add.w	r1, r6, r1, lsl #2
30004086:	f851 3c10 	ldr.w	r3, [r1, #-16]
3000408a:	4323      	orrs	r3, r4
3000408c:	f841 3c10 	str.w	r3, [r1, #-16]
30004090:	f852 6f0c 	ldr.w	r6, [r2, #12]!
30004094:	1c73      	adds	r3, r6, #1
30004096:	d1e5      	bne.n	30004064 <BOOT_RccConfig+0x40>
30004098:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000409a:	9c04      	ldr	r4, [sp, #16]
3000409c:	af0d      	add	r7, sp, #52	; 0x34
3000409e:	ae05      	add	r6, sp, #20
300040a0:	4a13      	ldr	r2, [pc, #76]	; (300040f0 <BOOT_RccConfig+0xcc>)
300040a2:	f85c 0b04 	ldr.w	r0, [ip], #4
300040a6:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300040aa:	f855 1b04 	ldr.w	r1, [r5], #4
300040ae:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300040b2:	f850 8002 	ldr.w	r8, [r0, r2]
300040b6:	45e6      	cmp	lr, ip
300040b8:	ea43 0308 	orr.w	r3, r3, r8
300040bc:	5083      	str	r3, [r0, r2]
300040be:	588b      	ldr	r3, [r1, r2]
300040c0:	ea44 0403 	orr.w	r4, r4, r3
300040c4:	508c      	str	r4, [r1, r2]
300040c6:	d004      	beq.n	300040d2 <BOOT_RccConfig+0xae>
300040c8:	f857 3b04 	ldr.w	r3, [r7], #4
300040cc:	f856 4b04 	ldr.w	r4, [r6], #4
300040d0:	e7e7      	b.n	300040a2 <BOOT_RccConfig+0x7e>
300040d2:	200a      	movs	r0, #10
300040d4:	4b07      	ldr	r3, [pc, #28]	; (300040f4 <BOOT_RccConfig+0xd0>)
300040d6:	4798      	blx	r3
300040d8:	4b07      	ldr	r3, [pc, #28]	; (300040f8 <BOOT_RccConfig+0xd4>)
300040da:	b010      	add	sp, #64	; 0x40
300040dc:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040e0:	4718      	bx	r3
300040e2:	4623      	mov	r3, r4
300040e4:	e7da      	b.n	3000409c <BOOT_RccConfig+0x78>
300040e6:	bf00      	nop
300040e8:	3000d5e4 	.word	0x3000d5e4
300040ec:	3000d948 	.word	0x3000d948
300040f0:	42008000 	.word	0x42008000
300040f4:	00009b2d 	.word	0x00009b2d
300040f8:	30007209 	.word	0x30007209

300040fc <BOOT_CACHEWRR_Set>:
300040fc:	4b14      	ldr	r3, [pc, #80]	; (30004150 <BOOT_CACHEWRR_Set+0x54>)
300040fe:	b430      	push	{r4, r5}
30004100:	681a      	ldr	r2, [r3, #0]
30004102:	2a01      	cmp	r2, #1
30004104:	d10f      	bne.n	30004126 <BOOT_CACHEWRR_Set+0x2a>
30004106:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
3000410a:	685d      	ldr	r5, [r3, #4]
3000410c:	689c      	ldr	r4, [r3, #8]
3000410e:	6882      	ldr	r2, [r0, #8]
30004110:	4910      	ldr	r1, [pc, #64]	; (30004154 <BOOT_CACHEWRR_Set+0x58>)
30004112:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004116:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
3000411a:	6082      	str	r2, [r0, #8]
3000411c:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30004120:	68da      	ldr	r2, [r3, #12]
30004122:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
30004126:	691a      	ldr	r2, [r3, #16]
30004128:	2a01      	cmp	r2, #1
3000412a:	d10f      	bne.n	3000414c <BOOT_CACHEWRR_Set+0x50>
3000412c:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30004130:	695d      	ldr	r5, [r3, #20]
30004132:	4a08      	ldr	r2, [pc, #32]	; (30004154 <BOOT_CACHEWRR_Set+0x58>)
30004134:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30004138:	688b      	ldr	r3, [r1, #8]
3000413a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000413e:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
30004142:	608b      	str	r3, [r1, #8]
30004144:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30004148:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
3000414c:	bc30      	pop	{r4, r5}
3000414e:	4770      	bx	lr
30004150:	3000dff4 	.word	0x3000dff4
30004154:	e0042000 	.word	0xe0042000

30004158 <BOOT_TCMSet>:
30004158:	2300      	movs	r3, #0
3000415a:	b5f0      	push	{r4, r5, r6, r7, lr}
3000415c:	4db3      	ldr	r5, [pc, #716]	; (3000442c <BOOT_TCMSet+0x2d4>)
3000415e:	461c      	mov	r4, r3
30004160:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
30004164:	3307      	adds	r3, #7
30004166:	4282      	cmp	r2, r0
30004168:	d03c      	beq.n	300041e4 <BOOT_TCMSet+0x8c>
3000416a:	3401      	adds	r4, #1
3000416c:	2c07      	cmp	r4, #7
3000416e:	d1f7      	bne.n	30004160 <BOOT_TCMSet+0x8>
30004170:	f3bf 8f4f 	dsb	sy
30004174:	f3bf 8f6f 	isb	sy
30004178:	48ad      	ldr	r0, [pc, #692]	; (30004430 <BOOT_TCMSet+0x2d8>)
3000417a:	2200      	movs	r2, #0
3000417c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30004180:	f3bf 8f4f 	dsb	sy
30004184:	f3bf 8f6f 	isb	sy
30004188:	6943      	ldr	r3, [r0, #20]
3000418a:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
3000418e:	6143      	str	r3, [r0, #20]
30004190:	f3bf 8f4f 	dsb	sy
30004194:	f3bf 8f6f 	isb	sy
30004198:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
3000419c:	f3bf 8f4f 	dsb	sy
300041a0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300041a4:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300041a8:	f3c5 344e 	ubfx	r4, r5, #13, #15
300041ac:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300041b0:	0164      	lsls	r4, r4, #5
300041b2:	ea04 0106 	and.w	r1, r4, r6
300041b6:	462b      	mov	r3, r5
300041b8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300041bc:	3b01      	subs	r3, #1
300041be:	1c5f      	adds	r7, r3, #1
300041c0:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300041c4:	d1f8      	bne.n	300041b8 <BOOT_TCMSet+0x60>
300041c6:	3c20      	subs	r4, #32
300041c8:	f114 0f20 	cmn.w	r4, #32
300041cc:	d1f1      	bne.n	300041b2 <BOOT_TCMSet+0x5a>
300041ce:	f3bf 8f4f 	dsb	sy
300041d2:	6943      	ldr	r3, [r0, #20]
300041d4:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300041d8:	6143      	str	r3, [r0, #20]
300041da:	f3bf 8f4f 	dsb	sy
300041de:	f3bf 8f6f 	isb	sy
300041e2:	bdf0      	pop	{r4, r5, r6, r7, pc}
300041e4:	2c00      	cmp	r4, #0
300041e6:	d0c3      	beq.n	30004170 <BOOT_TCMSet+0x18>
300041e8:	f3bf 8f4f 	dsb	sy
300041ec:	f3bf 8f6f 	isb	sy
300041f0:	4890      	ldr	r0, [pc, #576]	; (30004434 <BOOT_TCMSet+0x2dc>)
300041f2:	2300      	movs	r3, #0
300041f4:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
300041f8:	f3bf 8f4f 	dsb	sy
300041fc:	f3bf 8f6f 	isb	sy
30004200:	6942      	ldr	r2, [r0, #20]
30004202:	03d2      	lsls	r2, r2, #15
30004204:	d520      	bpl.n	30004248 <BOOT_TCMSet+0xf0>
30004206:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000420a:	f3bf 8f4f 	dsb	sy
3000420e:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30004212:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004216:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000421a:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000421e:	ea4f 1c43 	mov.w	ip, r3, lsl #5
30004222:	ea0c 0106 	and.w	r1, ip, r6
30004226:	4673      	mov	r3, lr
30004228:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000422c:	3b01      	subs	r3, #1
3000422e:	1c5f      	adds	r7, r3, #1
30004230:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004234:	d1f8      	bne.n	30004228 <BOOT_TCMSet+0xd0>
30004236:	f1ac 0c20 	sub.w	ip, ip, #32
3000423a:	f11c 0f20 	cmn.w	ip, #32
3000423e:	d1f0      	bne.n	30004222 <BOOT_TCMSet+0xca>
30004240:	f3bf 8f4f 	dsb	sy
30004244:	f3bf 8f6f 	isb	sy
30004248:	f3bf 8f4f 	dsb	sy
3000424c:	f3bf 8f6f 	isb	sy
30004250:	4878      	ldr	r0, [pc, #480]	; (30004434 <BOOT_TCMSet+0x2dc>)
30004252:	2200      	movs	r2, #0
30004254:	6943      	ldr	r3, [r0, #20]
30004256:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
3000425a:	6143      	str	r3, [r0, #20]
3000425c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30004260:	f3bf 8f4f 	dsb	sy
30004264:	f3bf 8f6f 	isb	sy
30004268:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
3000426c:	f3bf 8f4f 	dsb	sy
30004270:	6943      	ldr	r3, [r0, #20]
30004272:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30004276:	6143      	str	r3, [r0, #20]
30004278:	f3bf 8f4f 	dsb	sy
3000427c:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30004280:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30004284:	f3c3 06c9 	ubfx	r6, r3, #3, #10
30004288:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000428c:	ea4f 1c43 	mov.w	ip, r3, lsl #5
30004290:	ea0c 010e 	and.w	r1, ip, lr
30004294:	4633      	mov	r3, r6
30004296:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000429a:	3b01      	subs	r3, #1
3000429c:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300042a0:	1c5a      	adds	r2, r3, #1
300042a2:	d1f8      	bne.n	30004296 <BOOT_TCMSet+0x13e>
300042a4:	f1ac 0c20 	sub.w	ip, ip, #32
300042a8:	f11c 0f20 	cmn.w	ip, #32
300042ac:	d1f0      	bne.n	30004290 <BOOT_TCMSet+0x138>
300042ae:	f3bf 8f4f 	dsb	sy
300042b2:	f3bf 8f6f 	isb	sy
300042b6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300042ba:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300042be:	00e3      	lsls	r3, r4, #3
300042c0:	6b41      	ldr	r1, [r0, #52]	; 0x34
300042c2:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300042c6:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
300042ca:	6341      	str	r1, [r0, #52]	; 0x34
300042cc:	6911      	ldr	r1, [r2, #16]
300042ce:	2903      	cmp	r1, #3
300042d0:	d00f      	beq.n	300042f2 <BOOT_TCMSet+0x19a>
300042d2:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300042d6:	4858      	ldr	r0, [pc, #352]	; (30004438 <BOOT_TCMSet+0x2e0>)
300042d8:	f04f 0c00 	mov.w	ip, #0
300042dc:	4f57      	ldr	r7, [pc, #348]	; (3000443c <BOOT_TCMSet+0x2e4>)
300042de:	68b2      	ldr	r2, [r6, #8]
300042e0:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
300042e4:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
300042e8:	60b2      	str	r2, [r6, #8]
300042ea:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
300042ee:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
300042f2:	1b1a      	subs	r2, r3, r4
300042f4:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300042f8:	68d1      	ldr	r1, [r2, #12]
300042fa:	2903      	cmp	r1, #3
300042fc:	d00f      	beq.n	3000431e <BOOT_TCMSet+0x1c6>
300042fe:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
30004302:	484d      	ldr	r0, [pc, #308]	; (30004438 <BOOT_TCMSet+0x2e0>)
30004304:	f04f 0c00 	mov.w	ip, #0
30004308:	4f4c      	ldr	r7, [pc, #304]	; (3000443c <BOOT_TCMSet+0x2e4>)
3000430a:	68b2      	ldr	r2, [r6, #8]
3000430c:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004310:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
30004314:	60b2      	str	r2, [r6, #8]
30004316:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
3000431a:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
3000431e:	1b1a      	subs	r2, r3, r4
30004320:	4945      	ldr	r1, [pc, #276]	; (30004438 <BOOT_TCMSet+0x2e0>)
30004322:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004326:	6950      	ldr	r0, [r2, #20]
30004328:	6608      	str	r0, [r1, #96]	; 0x60
3000432a:	6990      	ldr	r0, [r2, #24]
3000432c:	6648      	str	r0, [r1, #100]	; 0x64
3000432e:	6852      	ldr	r2, [r2, #4]
30004330:	2a01      	cmp	r2, #1
30004332:	d04b      	beq.n	300043cc <BOOT_TCMSet+0x274>
30004334:	1b1b      	subs	r3, r3, r4
30004336:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000433a:	68ab      	ldr	r3, [r5, #8]
3000433c:	2b01      	cmp	r3, #1
3000433e:	f47f af50 	bne.w	300041e2 <BOOT_TCMSet+0x8a>
30004342:	483b      	ldr	r0, [pc, #236]	; (30004430 <BOOT_TCMSet+0x2d8>)
30004344:	2300      	movs	r3, #0
30004346:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000434a:	f3bf 8f4f 	dsb	sy
3000434e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004352:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004356:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000435a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000435e:	0164      	lsls	r4, r4, #5
30004360:	ea04 0106 	and.w	r1, r4, r6
30004364:	462b      	mov	r3, r5
30004366:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000436a:	3b01      	subs	r3, #1
3000436c:	1c5f      	adds	r7, r3, #1
3000436e:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30004372:	d1f8      	bne.n	30004366 <BOOT_TCMSet+0x20e>
30004374:	3c20      	subs	r4, #32
30004376:	f114 0f20 	cmn.w	r4, #32
3000437a:	d1f1      	bne.n	30004360 <BOOT_TCMSet+0x208>
3000437c:	f3bf 8f4f 	dsb	sy
30004380:	6943      	ldr	r3, [r0, #20]
30004382:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30004386:	6143      	str	r3, [r0, #20]
30004388:	f3bf 8f4f 	dsb	sy
3000438c:	f3bf 8f6f 	isb	sy
30004390:	4828      	ldr	r0, [pc, #160]	; (30004434 <BOOT_TCMSet+0x2dc>)
30004392:	2300      	movs	r3, #0
30004394:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004398:	f3bf 8f4f 	dsb	sy
3000439c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300043a0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300043a4:	f3c5 344e 	ubfx	r4, r5, #13, #15
300043a8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300043ac:	0164      	lsls	r4, r4, #5
300043ae:	ea04 0106 	and.w	r1, r4, r6
300043b2:	462b      	mov	r3, r5
300043b4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300043b8:	3b01      	subs	r3, #1
300043ba:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300043be:	1c5a      	adds	r2, r3, #1
300043c0:	d1f8      	bne.n	300043b4 <BOOT_TCMSet+0x25c>
300043c2:	3c20      	subs	r4, #32
300043c4:	f114 0f20 	cmn.w	r4, #32
300043c8:	d1f1      	bne.n	300043ae <BOOT_TCMSet+0x256>
300043ca:	e700      	b.n	300041ce <BOOT_TCMSet+0x76>
300043cc:	f3bf 8f4f 	dsb	sy
300043d0:	f3bf 8f6f 	isb	sy
300043d4:	4a16      	ldr	r2, [pc, #88]	; (30004430 <BOOT_TCMSet+0x2d8>)
300043d6:	2000      	movs	r0, #0
300043d8:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300043dc:	f3bf 8f4f 	dsb	sy
300043e0:	f3bf 8f6f 	isb	sy
300043e4:	6951      	ldr	r1, [r2, #20]
300043e6:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300043ea:	6151      	str	r1, [r2, #20]
300043ec:	f3bf 8f4f 	dsb	sy
300043f0:	f3bf 8f6f 	isb	sy
300043f4:	f3bf 8f4f 	dsb	sy
300043f8:	f3bf 8f6f 	isb	sy
300043fc:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30004400:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30004404:	f3bf 8f4f 	dsb	sy
30004408:	f3bf 8f6f 	isb	sy
3000440c:	6951      	ldr	r1, [r2, #20]
3000440e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30004412:	6151      	str	r1, [r2, #20]
30004414:	f3bf 8f4f 	dsb	sy
30004418:	f3bf 8f6f 	isb	sy
3000441c:	1b1b      	subs	r3, r3, r4
3000441e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30004422:	68ab      	ldr	r3, [r5, #8]
30004424:	2b01      	cmp	r3, #1
30004426:	f47f aedc 	bne.w	300041e2 <BOOT_TCMSet+0x8a>
3000442a:	e78a      	b.n	30004342 <BOOT_TCMSet+0x1ea>
3000442c:	3000df30 	.word	0x3000df30
30004430:	e002ed00 	.word	0xe002ed00
30004434:	e000ed00 	.word	0xe000ed00
30004438:	e0042000 	.word	0xe0042000
3000443c:	ffff0000 	.word	0xffff0000

30004440 <BOOT_GRstConfig>:
30004440:	b538      	push	{r3, r4, r5, lr}
30004442:	4c09      	ldr	r4, [pc, #36]	; (30004468 <BOOT_GRstConfig+0x28>)
30004444:	f241 050a 	movw	r5, #4106	; 0x100a
30004448:	2028      	movs	r0, #40	; 0x28
3000444a:	4a08      	ldr	r2, [pc, #32]	; (3000446c <BOOT_GRstConfig+0x2c>)
3000444c:	8823      	ldrh	r3, [r4, #0]
3000444e:	b29b      	uxth	r3, r3
30004450:	431d      	orrs	r5, r3
30004452:	8025      	strh	r5, [r4, #0]
30004454:	4790      	blx	r2
30004456:	88a3      	ldrh	r3, [r4, #4]
30004458:	b29b      	uxth	r3, r3
3000445a:	ea23 0305 	bic.w	r3, r3, r5
3000445e:	80a3      	strh	r3, [r4, #4]
30004460:	8863      	ldrh	r3, [r4, #2]
30004462:	8065      	strh	r5, [r4, #2]
30004464:	bd38      	pop	{r3, r4, r5, pc}
30004466:	bf00      	nop
30004468:	42008200 	.word	0x42008200
3000446c:	00009b2d 	.word	0x00009b2d

30004470 <BOOT_DDR_LCDC_HPR>:
30004470:	4a04      	ldr	r2, [pc, #16]	; (30004484 <BOOT_DDR_LCDC_HPR+0x14>)
30004472:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30004476:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
3000447a:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
3000447e:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30004482:	4770      	bx	lr
30004484:	42008000 	.word	0x42008000

30004488 <BOOT_DDR_Init>:
30004488:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000448c:	b530      	push	{r4, r5, lr}
3000448e:	4d15      	ldr	r5, [pc, #84]	; (300044e4 <BOOT_DDR_Init+0x5c>)
30004490:	b099      	sub	sp, #100	; 0x64
30004492:	6b1c      	ldr	r4, [r3, #48]	; 0x30
30004494:	47a8      	blx	r5
30004496:	2803      	cmp	r0, #3
30004498:	d11e      	bne.n	300044d8 <BOOT_DDR_Init+0x50>
3000449a:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
3000449e:	4a12      	ldr	r2, [pc, #72]	; (300044e8 <BOOT_DDR_Init+0x60>)
300044a0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300044a4:	4911      	ldr	r1, [pc, #68]	; (300044ec <BOOT_DDR_Init+0x64>)
300044a6:	a801      	add	r0, sp, #4
300044a8:	631c      	str	r4, [r3, #48]	; 0x30
300044aa:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300044ae:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300044b2:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300044b6:	4788      	blx	r1
300044b8:	a801      	add	r0, sp, #4
300044ba:	4b0d      	ldr	r3, [pc, #52]	; (300044f0 <BOOT_DDR_Init+0x68>)
300044bc:	4798      	blx	r3
300044be:	4b0d      	ldr	r3, [pc, #52]	; (300044f4 <BOOT_DDR_Init+0x6c>)
300044c0:	4798      	blx	r3
300044c2:	4b0d      	ldr	r3, [pc, #52]	; (300044f8 <BOOT_DDR_Init+0x70>)
300044c4:	4798      	blx	r3
300044c6:	4b0d      	ldr	r3, [pc, #52]	; (300044fc <BOOT_DDR_Init+0x74>)
300044c8:	4798      	blx	r3
300044ca:	2101      	movs	r1, #1
300044cc:	f44f 60e0 	mov.w	r0, #1792	; 0x700
300044d0:	4b0b      	ldr	r3, [pc, #44]	; (30004500 <BOOT_DDR_Init+0x78>)
300044d2:	4798      	blx	r3
300044d4:	b019      	add	sp, #100	; 0x64
300044d6:	bd30      	pop	{r4, r5, pc}
300044d8:	47a8      	blx	r5
300044da:	2802      	cmp	r0, #2
300044dc:	d0dd      	beq.n	3000449a <BOOT_DDR_Init+0x12>
300044de:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
300044e2:	e7dc      	b.n	3000449e <BOOT_DDR_Init+0x16>
300044e4:	0000c0f9 	.word	0x0000c0f9
300044e8:	42008000 	.word	0x42008000
300044ec:	30007325 	.word	0x30007325
300044f0:	3000789d 	.word	0x3000789d
300044f4:	3000a65d 	.word	0x3000a65d
300044f8:	3000796d 	.word	0x3000796d
300044fc:	30007971 	.word	0x30007971
30004500:	3000a5bd 	.word	0x3000a5bd

30004504 <BOOT_PSRAM_Init>:
30004504:	b500      	push	{lr}
30004506:	b089      	sub	sp, #36	; 0x24
30004508:	4b1b      	ldr	r3, [pc, #108]	; (30004578 <BOOT_PSRAM_Init+0x74>)
3000450a:	a801      	add	r0, sp, #4
3000450c:	4798      	blx	r3
3000450e:	a801      	add	r0, sp, #4
30004510:	4b1a      	ldr	r3, [pc, #104]	; (3000457c <BOOT_PSRAM_Init+0x78>)
30004512:	4798      	blx	r3
30004514:	4b1a      	ldr	r3, [pc, #104]	; (30004580 <BOOT_PSRAM_Init+0x7c>)
30004516:	4798      	blx	r3
30004518:	4b1a      	ldr	r3, [pc, #104]	; (30004584 <BOOT_PSRAM_Init+0x80>)
3000451a:	4798      	blx	r3
3000451c:	2801      	cmp	r0, #1
3000451e:	d01d      	beq.n	3000455c <BOOT_PSRAM_Init+0x58>
30004520:	4b19      	ldr	r3, [pc, #100]	; (30004588 <BOOT_PSRAM_Init+0x84>)
30004522:	2004      	movs	r0, #4
30004524:	4a19      	ldr	r2, [pc, #100]	; (3000458c <BOOT_PSRAM_Init+0x88>)
30004526:	4619      	mov	r1, r3
30004528:	f006 fe80 	bl	3000b22c <rtk_log_write>
3000452c:	4b18      	ldr	r3, [pc, #96]	; (30004590 <BOOT_PSRAM_Init+0x8c>)
3000452e:	4798      	blx	r3
30004530:	4b18      	ldr	r3, [pc, #96]	; (30004594 <BOOT_PSRAM_Init+0x90>)
30004532:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004536:	009b      	lsls	r3, r3, #2
30004538:	d517      	bpl.n	3000456a <BOOT_PSRAM_Init+0x66>
3000453a:	4a17      	ldr	r2, [pc, #92]	; (30004598 <BOOT_PSRAM_Init+0x94>)
3000453c:	4b17      	ldr	r3, [pc, #92]	; (3000459c <BOOT_PSRAM_Init+0x98>)
3000453e:	6812      	ldr	r2, [r2, #0]
30004540:	605a      	str	r2, [r3, #4]
30004542:	4a17      	ldr	r2, [pc, #92]	; (300045a0 <BOOT_PSRAM_Init+0x9c>)
30004544:	f04f 0c20 	mov.w	ip, #32
30004548:	2101      	movs	r1, #1
3000454a:	4b16      	ldr	r3, [pc, #88]	; (300045a4 <BOOT_PSRAM_Init+0xa0>)
3000454c:	6952      	ldr	r2, [r2, #20]
3000454e:	4608      	mov	r0, r1
30004550:	fbbc f2f2 	udiv	r2, ip, r2
30004554:	4798      	blx	r3
30004556:	b009      	add	sp, #36	; 0x24
30004558:	f85d fb04 	ldr.w	pc, [sp], #4
3000455c:	4b12      	ldr	r3, [pc, #72]	; (300045a8 <BOOT_PSRAM_Init+0xa4>)
3000455e:	4798      	blx	r3
30004560:	4b0c      	ldr	r3, [pc, #48]	; (30004594 <BOOT_PSRAM_Init+0x90>)
30004562:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004566:	009b      	lsls	r3, r3, #2
30004568:	d4e7      	bmi.n	3000453a <BOOT_PSRAM_Init+0x36>
3000456a:	4b10      	ldr	r3, [pc, #64]	; (300045ac <BOOT_PSRAM_Init+0xa8>)
3000456c:	4798      	blx	r3
3000456e:	4a0b      	ldr	r2, [pc, #44]	; (3000459c <BOOT_PSRAM_Init+0x98>)
30004570:	4b09      	ldr	r3, [pc, #36]	; (30004598 <BOOT_PSRAM_Init+0x94>)
30004572:	6852      	ldr	r2, [r2, #4]
30004574:	601a      	str	r2, [r3, #0]
30004576:	e7e4      	b.n	30004542 <BOOT_PSRAM_Init+0x3e>
30004578:	300080c9 	.word	0x300080c9
3000457c:	300080e9 	.word	0x300080e9
30004580:	30008151 	.word	0x30008151
30004584:	300071bd 	.word	0x300071bd
30004588:	3000b9d0 	.word	0x3000b9d0
3000458c:	3000bb04 	.word	0x3000bb04
30004590:	300083f5 	.word	0x300083f5
30004594:	42008000 	.word	0x42008000
30004598:	3000ee48 	.word	0x3000ee48
3000459c:	41002000 	.word	0x41002000
300045a0:	3000ed40 	.word	0x3000ed40
300045a4:	300086d5 	.word	0x300086d5
300045a8:	3000831d 	.word	0x3000831d
300045ac:	30008431 	.word	0x30008431

300045b0 <BOOT_WakeFromPG>:
300045b0:	b538      	push	{r3, r4, r5, lr}
300045b2:	4c46      	ldr	r4, [pc, #280]	; (300046cc <BOOT_WakeFromPG+0x11c>)
300045b4:	f002 fc86 	bl	30006ec4 <BOOT_RAM_TZCfg>
300045b8:	69a3      	ldr	r3, [r4, #24]
300045ba:	b10b      	cbz	r3, 300045c0 <BOOT_WakeFromPG+0x10>
300045bc:	4a44      	ldr	r2, [pc, #272]	; (300046d0 <BOOT_WakeFromPG+0x120>)
300045be:	6093      	str	r3, [r2, #8]
300045c0:	4b44      	ldr	r3, [pc, #272]	; (300046d4 <BOOT_WakeFromPG+0x124>)
300045c2:	f06f 0c1a 	mvn.w	ip, #26
300045c6:	4844      	ldr	r0, [pc, #272]	; (300046d8 <BOOT_WakeFromPG+0x128>)
300045c8:	f103 0e50 	add.w	lr, r3, #80	; 0x50
300045cc:	ebac 0c04 	sub.w	ip, ip, r4
300045d0:	eb0c 0203 	add.w	r2, ip, r3
300045d4:	f813 1f01 	ldrb.w	r1, [r3, #1]!
300045d8:	4402      	add	r2, r0
300045da:	4573      	cmp	r3, lr
300045dc:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
300045e0:	d1f6      	bne.n	300045d0 <BOOT_WakeFromPG+0x20>
300045e2:	6822      	ldr	r2, [r4, #0]
300045e4:	4b3a      	ldr	r3, [pc, #232]	; (300046d0 <BOOT_WakeFromPG+0x120>)
300045e6:	f422 7280 	bic.w	r2, r2, #256	; 0x100
300045ea:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
300045ee:	6002      	str	r2, [r0, #0]
300045f0:	6045      	str	r5, [r0, #4]
300045f2:	6081      	str	r1, [r0, #8]
300045f4:	6a59      	ldr	r1, [r3, #36]	; 0x24
300045f6:	4a39      	ldr	r2, [pc, #228]	; (300046dc <BOOT_WakeFromPG+0x12c>)
300045f8:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300045fc:	4d38      	ldr	r5, [pc, #224]	; (300046e0 <BOOT_WakeFromPG+0x130>)
300045fe:	6259      	str	r1, [r3, #36]	; 0x24
30004600:	6a51      	ldr	r1, [r2, #36]	; 0x24
30004602:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004606:	6251      	str	r1, [r2, #36]	; 0x24
30004608:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000460c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004610:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004614:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004618:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000461c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004620:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004624:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004628:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000462c:	f7ff fd66 	bl	300040fc <BOOT_CACHEWRR_Set>
30004630:	4b2c      	ldr	r3, [pc, #176]	; (300046e4 <BOOT_WakeFromPG+0x134>)
30004632:	6818      	ldr	r0, [r3, #0]
30004634:	f7ff fd90 	bl	30004158 <BOOT_TCMSet>
30004638:	2201      	movs	r2, #1
3000463a:	492b      	ldr	r1, [pc, #172]	; (300046e8 <BOOT_WakeFromPG+0x138>)
3000463c:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30004640:	47a8      	blx	r5
30004642:	4b2a      	ldr	r3, [pc, #168]	; (300046ec <BOOT_WakeFromPG+0x13c>)
30004644:	4798      	blx	r3
30004646:	4b2a      	ldr	r3, [pc, #168]	; (300046f0 <BOOT_WakeFromPG+0x140>)
30004648:	4798      	blx	r3
3000464a:	2801      	cmp	r0, #1
3000464c:	4604      	mov	r4, r0
3000464e:	d026      	beq.n	3000469e <BOOT_WakeFromPG+0xee>
30004650:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30004654:	2201      	movs	r2, #1
30004656:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
3000465a:	47a8      	blx	r5
3000465c:	4a25      	ldr	r2, [pc, #148]	; (300046f4 <BOOT_WakeFromPG+0x144>)
3000465e:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30004662:	f043 0302 	orr.w	r3, r3, #2
30004666:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
3000466a:	f7ff ff4b 	bl	30004504 <BOOT_PSRAM_Init>
3000466e:	2096      	movs	r0, #150	; 0x96
30004670:	4b21      	ldr	r3, [pc, #132]	; (300046f8 <BOOT_WakeFromPG+0x148>)
30004672:	4798      	blx	r3
30004674:	4a21      	ldr	r2, [pc, #132]	; (300046fc <BOOT_WakeFromPG+0x14c>)
30004676:	4c19      	ldr	r4, [pc, #100]	; (300046dc <BOOT_WakeFromPG+0x12c>)
30004678:	6893      	ldr	r3, [r2, #8]
3000467a:	4921      	ldr	r1, [pc, #132]	; (30004700 <BOOT_WakeFromPG+0x150>)
3000467c:	6852      	ldr	r2, [r2, #4]
3000467e:	4821      	ldr	r0, [pc, #132]	; (30004704 <BOOT_WakeFromPG+0x154>)
30004680:	605a      	str	r2, [r3, #4]
30004682:	60a3      	str	r3, [r4, #8]
30004684:	4a20      	ldr	r2, [pc, #128]	; (30004708 <BOOT_WakeFromPG+0x158>)
30004686:	61c8      	str	r0, [r1, #28]
30004688:	f382 8888 	msr	MSP_NS, r2
3000468c:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
30004690:	f382 8809 	msr	PSP, r2
30004694:	6858      	ldr	r0, [r3, #4]
30004696:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
3000469a:	f7ff bc73 	b.w	30003f84 <BOOT_NsStart>
3000469e:	4602      	mov	r2, r0
300046a0:	491a      	ldr	r1, [pc, #104]	; (3000470c <BOOT_WakeFromPG+0x15c>)
300046a2:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300046a6:	47a8      	blx	r5
300046a8:	4622      	mov	r2, r4
300046aa:	4919      	ldr	r1, [pc, #100]	; (30004710 <BOOT_WakeFromPG+0x160>)
300046ac:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300046b0:	47a8      	blx	r5
300046b2:	f7ff fee9 	bl	30004488 <BOOT_DDR_Init>
300046b6:	4a0f      	ldr	r2, [pc, #60]	; (300046f4 <BOOT_WakeFromPG+0x144>)
300046b8:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300046bc:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300046c0:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300046c4:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300046c8:	e7d4      	b.n	30004674 <BOOT_WakeFromPG+0xc4>
300046ca:	bf00      	nop
300046cc:	3000eddc 	.word	0x3000eddc
300046d0:	e000ed00 	.word	0xe000ed00
300046d4:	3000edf7 	.word	0x3000edf7
300046d8:	e000e100 	.word	0xe000e100
300046dc:	e002ed00 	.word	0xe002ed00
300046e0:	0000b479 	.word	0x0000b479
300046e4:	3000d940 	.word	0x3000d940
300046e8:	40001000 	.word	0x40001000
300046ec:	0000d835 	.word	0x0000d835
300046f0:	300070e5 	.word	0x300070e5
300046f4:	42008000 	.word	0x42008000
300046f8:	00009b2d 	.word	0x00009b2d
300046fc:	60000020 	.word	0x60000020
30004700:	30000000 	.word	0x30000000
30004704:	30003a81 	.word	0x30003a81
30004708:	2001bffc 	.word	0x2001bffc
3000470c:	40080000 	.word	0x40080000
30004710:	40040000 	.word	0x40040000

30004714 <BOOT_SCBConfig_HP>:
30004714:	4b0e      	ldr	r3, [pc, #56]	; (30004750 <BOOT_SCBConfig_HP+0x3c>)
30004716:	4a0f      	ldr	r2, [pc, #60]	; (30004754 <BOOT_SCBConfig_HP+0x40>)
30004718:	6a59      	ldr	r1, [r3, #36]	; 0x24
3000471a:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000471e:	6259      	str	r1, [r3, #36]	; 0x24
30004720:	6a51      	ldr	r1, [r2, #36]	; 0x24
30004722:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004726:	6251      	str	r1, [r2, #36]	; 0x24
30004728:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000472c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004730:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004734:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004738:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000473c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004740:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004744:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004748:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000474c:	4770      	bx	lr
3000474e:	bf00      	nop
30004750:	e000ed00 	.word	0xe000ed00
30004754:	e002ed00 	.word	0xe002ed00

30004758 <BOOT_SCBVTORBackup_HP>:
30004758:	4a02      	ldr	r2, [pc, #8]	; (30004764 <BOOT_SCBVTORBackup_HP+0xc>)
3000475a:	4b03      	ldr	r3, [pc, #12]	; (30004768 <BOOT_SCBVTORBackup_HP+0x10>)
3000475c:	6892      	ldr	r2, [r2, #8]
3000475e:	619a      	str	r2, [r3, #24]
30004760:	4770      	bx	lr
30004762:	bf00      	nop
30004764:	e000ed00 	.word	0xe000ed00
30004768:	3000eddc 	.word	0x3000eddc

3000476c <BOOT_SCBVTORReFill_HP>:
3000476c:	4a02      	ldr	r2, [pc, #8]	; (30004778 <BOOT_SCBVTORReFill_HP+0xc>)
3000476e:	4b03      	ldr	r3, [pc, #12]	; (3000477c <BOOT_SCBVTORReFill_HP+0x10>)
30004770:	6992      	ldr	r2, [r2, #24]
30004772:	609a      	str	r2, [r3, #8]
30004774:	4770      	bx	lr
30004776:	bf00      	nop
30004778:	3000eddc 	.word	0x3000eddc
3000477c:	e000ed00 	.word	0xe000ed00

30004780 <BOOT_NVICBackup_HP>:
30004780:	491a      	ldr	r1, [pc, #104]	; (300047ec <BOOT_NVICBackup_HP+0x6c>)
30004782:	f06f 001a 	mvn.w	r0, #26
30004786:	b410      	push	{r4}
30004788:	4c19      	ldr	r4, [pc, #100]	; (300047f0 <BOOT_NVICBackup_HP+0x70>)
3000478a:	680a      	ldr	r2, [r1, #0]
3000478c:	f104 031b 	add.w	r3, r4, #27
30004790:	1b00      	subs	r0, r0, r4
30004792:	6022      	str	r2, [r4, #0]
30004794:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
30004798:	684a      	ldr	r2, [r1, #4]
3000479a:	6062      	str	r2, [r4, #4]
3000479c:	688a      	ldr	r2, [r1, #8]
3000479e:	60a2      	str	r2, [r4, #8]
300047a0:	18c2      	adds	r2, r0, r3
300047a2:	440a      	add	r2, r1
300047a4:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
300047a8:	f803 2f01 	strb.w	r2, [r3, #1]!
300047ac:	4563      	cmp	r3, ip
300047ae:	d1f7      	bne.n	300047a0 <BOOT_NVICBackup_HP+0x20>
300047b0:	4b10      	ldr	r3, [pc, #64]	; (300047f4 <BOOT_NVICBackup_HP+0x74>)
300047b2:	695b      	ldr	r3, [r3, #20]
300047b4:	03db      	lsls	r3, r3, #15
300047b6:	d512      	bpl.n	300047de <BOOT_NVICBackup_HP+0x5e>
300047b8:	f014 011f 	ands.w	r1, r4, #31
300047bc:	4b0c      	ldr	r3, [pc, #48]	; (300047f0 <BOOT_NVICBackup_HP+0x70>)
300047be:	d111      	bne.n	300047e4 <BOOT_NVICBackup_HP+0x64>
300047c0:	216c      	movs	r1, #108	; 0x6c
300047c2:	f3bf 8f4f 	dsb	sy
300047c6:	480b      	ldr	r0, [pc, #44]	; (300047f4 <BOOT_NVICBackup_HP+0x74>)
300047c8:	4419      	add	r1, r3
300047ca:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
300047ce:	3320      	adds	r3, #32
300047d0:	1aca      	subs	r2, r1, r3
300047d2:	2a00      	cmp	r2, #0
300047d4:	dcf9      	bgt.n	300047ca <BOOT_NVICBackup_HP+0x4a>
300047d6:	f3bf 8f4f 	dsb	sy
300047da:	f3bf 8f6f 	isb	sy
300047de:	f85d 4b04 	ldr.w	r4, [sp], #4
300047e2:	4770      	bx	lr
300047e4:	f024 031f 	bic.w	r3, r4, #31
300047e8:	316c      	adds	r1, #108	; 0x6c
300047ea:	e7ea      	b.n	300047c2 <BOOT_NVICBackup_HP+0x42>
300047ec:	e000e100 	.word	0xe000e100
300047f0:	3000eddc 	.word	0x3000eddc
300047f4:	e000ed00 	.word	0xe000ed00

300047f8 <BOOT_NVICReFill_HP>:
300047f8:	b510      	push	{r4, lr}
300047fa:	f06f 0c1a 	mvn.w	ip, #26
300047fe:	4c0d      	ldr	r4, [pc, #52]	; (30004834 <BOOT_NVICReFill_HP+0x3c>)
30004800:	480d      	ldr	r0, [pc, #52]	; (30004838 <BOOT_NVICReFill_HP+0x40>)
30004802:	f104 031b 	add.w	r3, r4, #27
30004806:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
3000480a:	ebac 0c04 	sub.w	ip, ip, r4
3000480e:	eb0c 0203 	add.w	r2, ip, r3
30004812:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30004816:	4402      	add	r2, r0
30004818:	4573      	cmp	r3, lr
3000481a:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
3000481e:	d1f6      	bne.n	3000480e <BOOT_NVICReFill_HP+0x16>
30004820:	e9d4 3100 	ldrd	r3, r1, [r4]
30004824:	68a2      	ldr	r2, [r4, #8]
30004826:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000482a:	6003      	str	r3, [r0, #0]
3000482c:	6041      	str	r1, [r0, #4]
3000482e:	6082      	str	r2, [r0, #8]
30004830:	bd10      	pop	{r4, pc}
30004832:	bf00      	nop
30004834:	3000eddc 	.word	0x3000eddc
30004838:	e000e100 	.word	0xe000e100

3000483c <BOOT_ImgCopy>:
3000483c:	b510      	push	{r4, lr}
3000483e:	b082      	sub	sp, #8
30004840:	4b09      	ldr	r3, [pc, #36]	; (30004868 <BOOT_ImgCopy+0x2c>)
30004842:	4604      	mov	r4, r0
30004844:	e9cd 2100 	strd	r2, r1, [sp]
30004848:	4798      	blx	r3
3000484a:	e9dd 2100 	ldrd	r2, r1, [sp]
3000484e:	b128      	cbz	r0, 3000485c <BOOT_ImgCopy+0x20>
30004850:	4620      	mov	r0, r4
30004852:	4b06      	ldr	r3, [pc, #24]	; (3000486c <BOOT_ImgCopy+0x30>)
30004854:	b002      	add	sp, #8
30004856:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000485a:	4718      	bx	r3
3000485c:	4620      	mov	r0, r4
3000485e:	b002      	add	sp, #8
30004860:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004864:	f003 b9ea 	b.w	30007c3c <NandImgCopy>
30004868:	30008ef5 	.word	0x30008ef5
3000486c:	00012a1d 	.word	0x00012a1d

30004870 <BOOT_LoadImages>:
30004870:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004874:	4b59      	ldr	r3, [pc, #356]	; (300049dc <BOOT_LoadImages+0x16c>)
30004876:	2700      	movs	r7, #0
30004878:	2104      	movs	r1, #4
3000487a:	2601      	movs	r6, #1
3000487c:	ed2d 8b02 	vpush	{d8}
30004880:	b0b7      	sub	sp, #220	; 0xdc
30004882:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30004886:	4b56      	ldr	r3, [pc, #344]	; (300049e0 <BOOT_LoadImages+0x170>)
30004888:	aa11      	add	r2, sp, #68	; 0x44
3000488a:	9703      	str	r7, [sp, #12]
3000488c:	9705      	str	r7, [sp, #20]
3000488e:	4798      	blx	r3
30004890:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
30004894:	409e      	lsls	r6, r3
30004896:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
3000489a:	f000 823e 	beq.w	30004d1a <BOOT_LoadImages+0x4aa>
3000489e:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
300048a2:	f000 8379 	beq.w	30004f98 <BOOT_LoadImages+0x728>
300048a6:	f1b6 7f00 	cmp.w	r6, #33554432	; 0x2000000
300048aa:	f000 8383 	beq.w	30004fb4 <BOOT_LoadImages+0x744>
300048ae:	4c4d      	ldr	r4, [pc, #308]	; (300049e4 <BOOT_LoadImages+0x174>)
300048b0:	f04f 0800 	mov.w	r8, #0
300048b4:	4d4c      	ldr	r5, [pc, #304]	; (300049e8 <BOOT_LoadImages+0x178>)
300048b6:	eb06 0904 	add.w	r9, r6, r4
300048ba:	484c      	ldr	r0, [pc, #304]	; (300049ec <BOOT_LoadImages+0x17c>)
300048bc:	4435      	add	r5, r6
300048be:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
300048c2:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
300048c6:	f7ff fa05 	bl	30003cd4 <mcc_init_crc32>
300048ca:	f8d9 3004 	ldr.w	r3, [r9, #4]
300048ce:	f8d5 a008 	ldr.w	sl, [r5, #8]
300048d2:	f899 200c 	ldrb.w	r2, [r9, #12]
300048d6:	9308      	str	r3, [sp, #32]
300048d8:	3b01      	subs	r3, #1
300048da:	9206      	str	r2, [sp, #24]
300048dc:	3303      	adds	r3, #3
300048de:	686a      	ldr	r2, [r5, #4]
300048e0:	f895 b00c 	ldrb.w	fp, [r5, #12]
300048e4:	9204      	str	r2, [sp, #16]
300048e6:	f200 81bc 	bhi.w	30004c62 <BOOT_LoadImages+0x3f2>
300048ea:	f8d9 2008 	ldr.w	r2, [r9, #8]
300048ee:	9b04      	ldr	r3, [sp, #16]
300048f0:	3a01      	subs	r2, #1
300048f2:	3b01      	subs	r3, #1
300048f4:	3203      	adds	r2, #3
300048f6:	d867      	bhi.n	300049c8 <BOOT_LoadImages+0x158>
300048f8:	9a06      	ldr	r2, [sp, #24]
300048fa:	9307      	str	r3, [sp, #28]
300048fc:	2a01      	cmp	r2, #1
300048fe:	f240 8305 	bls.w	30004f0c <BOOT_LoadImages+0x69c>
30004902:	483b      	ldr	r0, [pc, #236]	; (300049f0 <BOOT_LoadImages+0x180>)
30004904:	f006 ff30 	bl	3000b768 <__DiagPrintf_veneer>
30004908:	9b07      	ldr	r3, [sp, #28]
3000490a:	3303      	adds	r3, #3
3000490c:	d863      	bhi.n	300049d6 <BOOT_LoadImages+0x166>
3000490e:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30004912:	f11a 0f03 	cmn.w	sl, #3
30004916:	f200 81aa 	bhi.w	30004c6e <BOOT_LoadImages+0x3fe>
3000491a:	f1bb 0f01 	cmp.w	fp, #1
3000491e:	f200 8347 	bhi.w	30004fb0 <BOOT_LoadImages+0x740>
30004922:	9904      	ldr	r1, [sp, #16]
30004924:	4833      	ldr	r0, [pc, #204]	; (300049f4 <BOOT_LoadImages+0x184>)
30004926:	f006 ff1f 	bl	3000b768 <__DiagPrintf_veneer>
3000492a:	f8d9 3011 	ldr.w	r3, [r9, #17]
3000492e:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30004932:	4c2e      	ldr	r4, [pc, #184]	; (300049ec <BOOT_LoadImages+0x17c>)
30004934:	9307      	str	r3, [sp, #28]
30004936:	2300      	movs	r3, #0
30004938:	f8d5 a00d 	ldr.w	sl, [r5, #13]
3000493c:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004940:	9309      	str	r3, [sp, #36]	; 0x24
30004942:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30004a0c <BOOT_LoadImages+0x19c>
30004946:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
3000494a:	6829      	ldr	r1, [r5, #0]
3000494c:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
30004950:	44b4      	add	ip, r6
30004952:	f81c 2b01 	ldrb.w	r2, [ip], #1
30004956:	4042      	eors	r2, r0
30004958:	459c      	cmp	ip, r3
3000495a:	b2d2      	uxtb	r2, r2
3000495c:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30004960:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30004964:	d1f5      	bne.n	30004952 <BOOT_LoadImages+0xe2>
30004966:	43c2      	mvns	r2, r0
30004968:	930f      	str	r3, [sp, #60]	; 0x3c
3000496a:	4291      	cmp	r1, r2
3000496c:	f000 8301 	beq.w	30004f72 <BOOT_LoadImages+0x702>
30004970:	4821      	ldr	r0, [pc, #132]	; (300049f8 <BOOT_LoadImages+0x188>)
30004972:	f006 fef9 	bl	3000b768 <__DiagPrintf_veneer>
30004976:	2200      	movs	r2, #0
30004978:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000497a:	9909      	ldr	r1, [sp, #36]	; 0x24
3000497c:	2900      	cmp	r1, #0
3000497e:	d04e      	beq.n	30004a1e <BOOT_LoadImages+0x1ae>
30004980:	f899 100c 	ldrb.w	r1, [r9, #12]
30004984:	455b      	cmp	r3, fp
30004986:	9106      	str	r1, [sp, #24]
30004988:	d342      	bcc.n	30004a10 <BOOT_LoadImages+0x1a0>
3000498a:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
3000498e:	45e3      	cmp	fp, ip
30004990:	d93e      	bls.n	30004a10 <BOOT_LoadImages+0x1a0>
30004992:	9807      	ldr	r0, [sp, #28]
30004994:	4560      	cmp	r0, ip
30004996:	f240 8203 	bls.w	30004da0 <BOOT_LoadImages+0x530>
3000499a:	4283      	cmp	r3, r0
3000499c:	f0c0 8200 	bcc.w	30004da0 <BOOT_LoadImages+0x530>
300049a0:	2a00      	cmp	r2, #0
300049a2:	f000 820d 	beq.w	30004dc0 <BOOT_LoadImages+0x550>
300049a6:	4553      	cmp	r3, sl
300049a8:	f080 8410 	bcs.w	300051cc <BOOT_LoadImages+0x95c>
300049ac:	4813      	ldr	r0, [pc, #76]	; (300049fc <BOOT_LoadImages+0x18c>)
300049ae:	f006 fedb 	bl	3000b768 <__DiagPrintf_veneer>
300049b2:	9f06      	ldr	r7, [sp, #24]
300049b4:	4812      	ldr	r0, [pc, #72]	; (30004a00 <BOOT_LoadImages+0x190>)
300049b6:	464d      	mov	r5, r9
300049b8:	f006 fed6 	bl	3000b768 <__DiagPrintf_veneer>
300049bc:	9b07      	ldr	r3, [sp, #28]
300049be:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
300049c2:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
300049c6:	e04a      	b.n	30004a5e <BOOT_LoadImages+0x1ee>
300049c8:	480e      	ldr	r0, [pc, #56]	; (30004a04 <BOOT_LoadImages+0x194>)
300049ca:	9307      	str	r3, [sp, #28]
300049cc:	f006 fecc 	bl	3000b768 <__DiagPrintf_veneer>
300049d0:	9b07      	ldr	r3, [sp, #28]
300049d2:	3303      	adds	r3, #3
300049d4:	d99b      	bls.n	3000490e <BOOT_LoadImages+0x9e>
300049d6:	480c      	ldr	r0, [pc, #48]	; (30004a08 <BOOT_LoadImages+0x198>)
300049d8:	e14a      	b.n	30004c70 <BOOT_LoadImages+0x400>
300049da:	bf00      	nop
300049dc:	2001c01c 	.word	0x2001c01c
300049e0:	00009db5 	.word	0x00009db5
300049e4:	07ffe000 	.word	0x07ffe000
300049e8:	07fff000 	.word	0x07fff000
300049ec:	3000db04 	.word	0x3000db04
300049f0:	3000bb7c 	.word	0x3000bb7c
300049f4:	3000bf64 	.word	0x3000bf64
300049f8:	3000bc80 	.word	0x3000bc80
300049fc:	3000bf48 	.word	0x3000bf48
30004a00:	3000bd0c 	.word	0x3000bd0c
30004a04:	3000bb68 	.word	0x3000bb68
30004a08:	3000bf14 	.word	0x3000bf14
30004a0c:	07fff004 	.word	0x07fff004
30004a10:	48c4      	ldr	r0, [pc, #784]	; (30004d24 <BOOT_LoadImages+0x4b4>)
30004a12:	930f      	str	r3, [sp, #60]	; 0x3c
30004a14:	9209      	str	r2, [sp, #36]	; 0x24
30004a16:	f006 fea7 	bl	3000b768 <__DiagPrintf_veneer>
30004a1a:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004a1c:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004a1e:	2a00      	cmp	r2, #0
30004a20:	f000 8131 	beq.w	30004c86 <BOOT_LoadImages+0x416>
30004a24:	459a      	cmp	sl, r3
30004a26:	f200 82c9 	bhi.w	30004fbc <BOOT_LoadImages+0x74c>
30004a2a:	f1ba 6f00 	cmp.w	sl, #134217728	; 0x8000000
30004a2e:	bf2c      	ite	cs
30004a30:	2200      	movcs	r2, #0
30004a32:	2201      	movcc	r2, #1
30004a34:	2a00      	cmp	r2, #0
30004a36:	f040 82c1 	bne.w	30004fbc <BOOT_LoadImages+0x74c>
30004a3a:	4598      	cmp	r8, r3
30004a3c:	f200 81b8 	bhi.w	30004db0 <BOOT_LoadImages+0x540>
30004a40:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30004a44:	f0c0 81b4 	bcc.w	30004db0 <BOOT_LoadImages+0x540>
30004a48:	7b2f      	ldrb	r7, [r5, #12]
30004a4a:	2a00      	cmp	r2, #0
30004a4c:	f040 81de 	bne.w	30004e0c <BOOT_LoadImages+0x59c>
30004a50:	48b5      	ldr	r0, [pc, #724]	; (30004d28 <BOOT_LoadImages+0x4b8>)
30004a52:	f006 fe89 	bl	3000b768 <__DiagPrintf_veneer>
30004a56:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30004a5a:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30004a5e:	4639      	mov	r1, r7
30004a60:	48b2      	ldr	r0, [pc, #712]	; (30004d2c <BOOT_LoadImages+0x4bc>)
30004a62:	f006 fe81 	bl	3000b768 <__DiagPrintf_veneer>
30004a66:	4bb2      	ldr	r3, [pc, #712]	; (30004d30 <BOOT_LoadImages+0x4c0>)
30004a68:	4798      	blx	r3
30004a6a:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004a6e:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30004a72:	f8d1 300a 	ldr.w	r3, [r1, #10]
30004a76:	6809      	ldr	r1, [r1, #0]
30004a78:	330c      	adds	r3, #12
30004a7a:	910b      	str	r1, [sp, #44]	; 0x2c
30004a7c:	42b3      	cmp	r3, r6
30004a7e:	bf28      	it	cs
30004a80:	4633      	movcs	r3, r6
30004a82:	930d      	str	r3, [sp, #52]	; 0x34
30004a84:	f8d2 300a 	ldr.w	r3, [r2, #10]
30004a88:	6812      	ldr	r2, [r2, #0]
30004a8a:	330c      	adds	r3, #12
30004a8c:	920a      	str	r2, [sp, #40]	; 0x28
30004a8e:	429e      	cmp	r6, r3
30004a90:	bf28      	it	cs
30004a92:	461e      	movcs	r6, r3
30004a94:	960c      	str	r6, [sp, #48]	; 0x30
30004a96:	2800      	cmp	r0, #0
30004a98:	f040 8194 	bne.w	30004dc4 <BOOT_LoadImages+0x554>
30004a9c:	4ba5      	ldr	r3, [pc, #660]	; (30004d34 <BOOT_LoadImages+0x4c4>)
30004a9e:	4606      	mov	r6, r0
30004aa0:	f8df 92bc 	ldr.w	r9, [pc, #700]	; 30004d60 <BOOT_LoadImages+0x4f0>
30004aa4:	4680      	mov	r8, r0
30004aa6:	f8df b2bc 	ldr.w	fp, [pc, #700]	; 30004d64 <BOOT_LoadImages+0x4f4>
30004aaa:	4682      	mov	sl, r0
30004aac:	e9cd 0303 	strd	r0, r3, [sp, #12]
30004ab0:	2f00      	cmp	r7, #0
30004ab2:	f040 81c9 	bne.w	30004e48 <BOOT_LoadImages+0x5d8>
30004ab6:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30004aba:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30004abc:	f10c 0104 	add.w	r1, ip, #4
30004ac0:	2b00      	cmp	r3, #0
30004ac2:	f000 83df 	beq.w	30005284 <BOOT_LoadImages+0xa14>
30004ac6:	1d18      	adds	r0, r3, #4
30004ac8:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004acc:	4460      	add	r0, ip
30004ace:	f811 3b01 	ldrb.w	r3, [r1], #1
30004ad2:	4053      	eors	r3, r2
30004ad4:	4281      	cmp	r1, r0
30004ad6:	b2db      	uxtb	r3, r3
30004ad8:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004adc:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004ae0:	d1f5      	bne.n	30004ace <BOOT_LoadImages+0x25e>
30004ae2:	43d2      	mvns	r2, r2
30004ae4:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30004ae6:	4293      	cmp	r3, r2
30004ae8:	f000 83a7 	beq.w	3000523a <BOOT_LoadImages+0x9ca>
30004aec:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004af0:	f088 0301 	eor.w	r3, r8, #1
30004af4:	3101      	adds	r1, #1
30004af6:	f003 0301 	and.w	r3, r3, #1
30004afa:	ea46 0603 	orr.w	r6, r6, r3
30004afe:	f000 8346 	beq.w	3000518e <BOOT_LoadImages+0x91e>
30004b02:	990b      	ldr	r1, [sp, #44]	; 0x2c
30004b04:	488c      	ldr	r0, [pc, #560]	; (30004d38 <BOOT_LoadImages+0x4c8>)
30004b06:	f006 fe2f 	bl	3000b768 <__DiagPrintf_veneer>
30004b0a:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004b0e:	2002      	movs	r0, #2
30004b10:	f7ff f85e 	bl	30003bd0 <FLASH_Erase_With_Lock>
30004b14:	2e00      	cmp	r6, #0
30004b16:	f040 8324 	bne.w	30005162 <BOOT_LoadImages+0x8f2>
30004b1a:	9605      	str	r6, [sp, #20]
30004b1c:	f08a 0601 	eor.w	r6, sl, #1
30004b20:	f04f 0801 	mov.w	r8, #1
30004b24:	f006 0601 	and.w	r6, r6, #1
30004b28:	9b03      	ldr	r3, [sp, #12]
30004b2a:	2b01      	cmp	r3, #1
30004b2c:	f000 8238 	beq.w	30004fa0 <BOOT_LoadImages+0x730>
30004b30:	9b03      	ldr	r3, [sp, #12]
30004b32:	2b02      	cmp	r3, #2
30004b34:	f000 83b2 	beq.w	3000529c <BOOT_LoadImages+0xa2c>
30004b38:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30004b3a:	4650      	mov	r0, sl
30004b3c:	f8c9 3000 	str.w	r3, [r9]
30004b40:	47d8      	blx	fp
30004b42:	1e03      	subs	r3, r0, #0
30004b44:	bf18      	it	ne
30004b46:	2301      	movne	r3, #1
30004b48:	2800      	cmp	r0, #0
30004b4a:	f000 8156 	beq.w	30004dfa <BOOT_LoadImages+0x58a>
30004b4e:	2e00      	cmp	r6, #0
30004b50:	f000 8153 	beq.w	30004dfa <BOOT_LoadImages+0x58a>
30004b54:	fab7 f787 	clz	r7, r7
30004b58:	9b05      	ldr	r3, [sp, #20]
30004b5a:	097f      	lsrs	r7, r7, #5
30004b5c:	2b00      	cmp	r3, #0
30004b5e:	f000 83ee 	beq.w	3000533e <BOOT_LoadImages+0xace>
30004b62:	469a      	mov	sl, r3
30004b64:	4c75      	ldr	r4, [pc, #468]	; (30004d3c <BOOT_LoadImages+0x4cc>)
30004b66:	2f00      	cmp	r7, #0
30004b68:	f040 8138 	bne.w	30004ddc <BOOT_LoadImages+0x56c>
30004b6c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004b70:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004b74:	3201      	adds	r2, #1
30004b76:	f000 82ad 	beq.w	300050d4 <BOOT_LoadImages+0x864>
30004b7a:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004b7e:	4870      	ldr	r0, [pc, #448]	; (30004d40 <BOOT_LoadImages+0x4d0>)
30004b80:	f006 fdf2 	bl	3000b768 <__DiagPrintf_veneer>
30004b84:	2301      	movs	r3, #1
30004b86:	9303      	str	r3, [sp, #12]
30004b88:	f04f 0801 	mov.w	r8, #1
30004b8c:	4646      	mov	r6, r8
30004b8e:	f8cd 8014 	str.w	r8, [sp, #20]
30004b92:	f1ba 0f00 	cmp.w	sl, #0
30004b96:	d0c7      	beq.n	30004b28 <BOOT_LoadImages+0x2b8>
30004b98:	ab16      	add	r3, sp, #88	; 0x58
30004b9a:	2280      	movs	r2, #128	; 0x80
30004b9c:	21ff      	movs	r1, #255	; 0xff
30004b9e:	46d0      	mov	r8, sl
30004ba0:	461e      	mov	r6, r3
30004ba2:	4618      	mov	r0, r3
30004ba4:	ee08 3a10 	vmov	s16, r3
30004ba8:	f006 fdb6 	bl	3000b718 <____wrap_memset_veneer>
30004bac:	2280      	movs	r2, #128	; 0x80
30004bae:	4629      	mov	r1, r5
30004bb0:	4630      	mov	r0, r6
30004bb2:	f006 fd99 	bl	3000b6e8 <____wrap_memcpy_veneer>
30004bb6:	2f01      	cmp	r7, #1
30004bb8:	f000 8327 	beq.w	3000520a <BOOT_LoadImages+0x99a>
30004bbc:	2100      	movs	r1, #0
30004bbe:	f8cd a014 	str.w	sl, [sp, #20]
30004bc2:	f04f 0a01 	mov.w	sl, #1
30004bc6:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30004bca:	1d2e      	adds	r6, r5, #4
30004bcc:	485d      	ldr	r0, [pc, #372]	; (30004d44 <BOOT_LoadImages+0x4d4>)
30004bce:	f006 fdcb 	bl	3000b768 <__DiagPrintf_veneer>
30004bd2:	4629      	mov	r1, r5
30004bd4:	2002      	movs	r0, #2
30004bd6:	f7fe fffb 	bl	30003bd0 <FLASH_Erase_With_Lock>
30004bda:	aa17      	add	r2, sp, #92	; 0x5c
30004bdc:	217c      	movs	r1, #124	; 0x7c
30004bde:	4630      	mov	r0, r6
30004be0:	f7ff f81e 	bl	30003c20 <FLASH_TxData_With_Lock>
30004be4:	4630      	mov	r0, r6
30004be6:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30004bea:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004bee:	f810 3b01 	ldrb.w	r3, [r0], #1
30004bf2:	4053      	eors	r3, r2
30004bf4:	4288      	cmp	r0, r1
30004bf6:	b2db      	uxtb	r3, r3
30004bf8:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004bfc:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004c00:	d1f5      	bne.n	30004bee <BOOT_LoadImages+0x37e>
30004c02:	43d2      	mvns	r2, r2
30004c04:	4629      	mov	r1, r5
30004c06:	2002      	movs	r0, #2
30004c08:	9216      	str	r2, [sp, #88]	; 0x58
30004c0a:	f7fe ffe1 	bl	30003bd0 <FLASH_Erase_With_Lock>
30004c0e:	ee18 2a10 	vmov	r2, s16
30004c12:	2180      	movs	r1, #128	; 0x80
30004c14:	4628      	mov	r0, r5
30004c16:	f7ff f803 	bl	30003c20 <FLASH_TxData_With_Lock>
30004c1a:	9b05      	ldr	r3, [sp, #20]
30004c1c:	2b00      	cmp	r3, #0
30004c1e:	f000 8143 	beq.w	30004ea8 <BOOT_LoadImages+0x638>
30004c22:	f1b8 0f00 	cmp.w	r8, #0
30004c26:	f040 837f 	bne.w	30005328 <BOOT_LoadImages+0xab8>
30004c2a:	2f01      	cmp	r7, #1
30004c2c:	f000 8268 	beq.w	30005100 <BOOT_LoadImages+0x890>
30004c30:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004c34:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004c38:	3201      	adds	r2, #1
30004c3a:	f000 81d4 	beq.w	30004fe6 <BOOT_LoadImages+0x776>
30004c3e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004c42:	4841      	ldr	r0, [pc, #260]	; (30004d48 <BOOT_LoadImages+0x4d8>)
30004c44:	f006 fd90 	bl	3000b768 <__DiagPrintf_veneer>
30004c48:	f08a 0601 	eor.w	r6, sl, #1
30004c4c:	f04f 0801 	mov.w	r8, #1
30004c50:	f006 0601 	and.w	r6, r6, #1
30004c54:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004c58:	3310      	adds	r3, #16
30004c5a:	930e      	str	r3, [sp, #56]	; 0x38
30004c5c:	2302      	movs	r3, #2
30004c5e:	9303      	str	r3, [sp, #12]
30004c60:	e76a      	b.n	30004b38 <BOOT_LoadImages+0x2c8>
30004c62:	483a      	ldr	r0, [pc, #232]	; (30004d4c <BOOT_LoadImages+0x4dc>)
30004c64:	f006 fd80 	bl	3000b768 <__DiagPrintf_veneer>
30004c68:	9b04      	ldr	r3, [sp, #16]
30004c6a:	3b01      	subs	r3, #1
30004c6c:	e64d      	b.n	3000490a <BOOT_LoadImages+0x9a>
30004c6e:	4838      	ldr	r0, [pc, #224]	; (30004d50 <BOOT_LoadImages+0x4e0>)
30004c70:	f006 fd7a 	bl	3000b768 <__DiagPrintf_veneer>
30004c74:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004c78:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30004c7c:	9307      	str	r3, [sp, #28]
30004c7e:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30004c82:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004c86:	4833      	ldr	r0, [pc, #204]	; (30004d54 <BOOT_LoadImages+0x4e4>)
30004c88:	f006 fd6e 	bl	3000b768 <__DiagPrintf_veneer>
30004c8c:	4832      	ldr	r0, [pc, #200]	; (30004d58 <BOOT_LoadImages+0x4e8>)
30004c8e:	f006 fd6b 	bl	3000b768 <__DiagPrintf_veneer>
30004c92:	4631      	mov	r1, r6
30004c94:	aa12      	add	r2, sp, #72	; 0x48
30004c96:	4658      	mov	r0, fp
30004c98:	f7ff f862 	bl	30003d60 <recovery_check>
30004c9c:	4631      	mov	r1, r6
30004c9e:	4604      	mov	r4, r0
30004ca0:	aa13      	add	r2, sp, #76	; 0x4c
30004ca2:	9807      	ldr	r0, [sp, #28]
30004ca4:	f7ff f85c 	bl	30003d60 <recovery_check>
30004ca8:	4631      	mov	r1, r6
30004caa:	aa14      	add	r2, sp, #80	; 0x50
30004cac:	9004      	str	r0, [sp, #16]
30004cae:	4650      	mov	r0, sl
30004cb0:	f7ff f856 	bl	30003d60 <recovery_check>
30004cb4:	4631      	mov	r1, r6
30004cb6:	aa15      	add	r2, sp, #84	; 0x54
30004cb8:	4606      	mov	r6, r0
30004cba:	4640      	mov	r0, r8
30004cbc:	f7ff f850 	bl	30003d60 <recovery_check>
30004cc0:	9b04      	ldr	r3, [sp, #16]
30004cc2:	4306      	orrs	r6, r0
30004cc4:	431c      	orrs	r4, r3
30004cc6:	d04f      	beq.n	30004d68 <BOOT_LoadImages+0x4f8>
30004cc8:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30004ccc:	2e00      	cmp	r6, #0
30004cce:	f000 818e 	beq.w	30004fee <BOOT_LoadImages+0x77e>
30004cd2:	4293      	cmp	r3, r2
30004cd4:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
30004cd8:	f0c0 827c 	bcc.w	300051d4 <BOOT_LoadImages+0x964>
30004cdc:	428b      	cmp	r3, r1
30004cde:	d302      	bcc.n	30004ce6 <BOOT_LoadImages+0x476>
30004ce0:	4283      	cmp	r3, r0
30004ce2:	f080 82fd 	bcs.w	300052e0 <BOOT_LoadImages+0xa70>
30004ce6:	4293      	cmp	r3, r2
30004ce8:	f000 8274 	beq.w	300051d4 <BOOT_LoadImages+0x964>
30004cec:	4293      	cmp	r3, r2
30004cee:	bf38      	it	cc
30004cf0:	4613      	movcc	r3, r2
30004cf2:	428b      	cmp	r3, r1
30004cf4:	f240 82e5 	bls.w	300052c2 <BOOT_LoadImages+0xa52>
30004cf8:	428b      	cmp	r3, r1
30004cfa:	bf38      	it	cc
30004cfc:	460b      	movcc	r3, r1
30004cfe:	4283      	cmp	r3, r0
30004d00:	f240 82f6 	bls.w	300052f0 <BOOT_LoadImages+0xa80>
30004d04:	f04f 0800 	mov.w	r8, #0
30004d08:	4b0a      	ldr	r3, [pc, #40]	; (30004d34 <BOOT_LoadImages+0x4c4>)
30004d0a:	2601      	movs	r6, #1
30004d0c:	f8df 9050 	ldr.w	r9, [pc, #80]	; 30004d60 <BOOT_LoadImages+0x4f0>
30004d10:	f8df b050 	ldr.w	fp, [pc, #80]	; 30004d64 <BOOT_LoadImages+0x4f4>
30004d14:	46c2      	mov	sl, r8
30004d16:	9304      	str	r3, [sp, #16]
30004d18:	e70e      	b.n	30004b38 <BOOT_LoadImages+0x2c8>
30004d1a:	4810      	ldr	r0, [pc, #64]	; (30004d5c <BOOT_LoadImages+0x4ec>)
30004d1c:	f006 fd24 	bl	3000b768 <__DiagPrintf_veneer>
30004d20:	e5c5      	b.n	300048ae <BOOT_LoadImages+0x3e>
30004d22:	bf00      	nop
30004d24:	3000bcc0 	.word	0x3000bcc0
30004d28:	3000bd1c 	.word	0x3000bd1c
30004d2c:	3000bd50 	.word	0x3000bd50
30004d30:	0000c149 	.word	0x0000c149
30004d34:	3000d93f 	.word	0x3000d93f
30004d38:	3000bdc0 	.word	0x3000bdc0
30004d3c:	3000db04 	.word	0x3000db04
30004d40:	3000bab4 	.word	0x3000bab4
30004d44:	3000be94 	.word	0x3000be94
30004d48:	3000bacc 	.word	0x3000bacc
30004d4c:	3000bb50 	.word	0x3000bb50
30004d50:	3000bbb4 	.word	0x3000bbb4
30004d54:	3000bd2c 	.word	0x3000bd2c
30004d58:	3000bd40 	.word	0x3000bd40
30004d5c:	3000bb18 	.word	0x3000bb18
30004d60:	3000edd8 	.word	0x3000edd8
30004d64:	300065d5 	.word	0x300065d5
30004d68:	2e00      	cmp	r6, #0
30004d6a:	f000 8285 	beq.w	30005278 <BOOT_LoadImages+0xa08>
30004d6e:	4651      	mov	r1, sl
30004d70:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
30004d74:	e9cd 5400 	strd	r5, r4, [sp]
30004d78:	429a      	cmp	r2, r3
30004d7a:	464b      	mov	r3, r9
30004d7c:	4642      	mov	r2, r8
30004d7e:	f080 8223 	bcs.w	300051c8 <BOOT_LoadImages+0x958>
30004d82:	2001      	movs	r0, #1
30004d84:	f7ff f81c 	bl	30003dc0 <rewrite_bp>
30004d88:	46a0      	mov	r8, r4
30004d8a:	b2c3      	uxtb	r3, r0
30004d8c:	2601      	movs	r6, #1
30004d8e:	46a2      	mov	sl, r4
30004d90:	f8df 92e8 	ldr.w	r9, [pc, #744]	; 3000507c <BOOT_LoadImages+0x80c>
30004d94:	9303      	str	r3, [sp, #12]
30004d96:	4ba3      	ldr	r3, [pc, #652]	; (30005024 <BOOT_LoadImages+0x7b4>)
30004d98:	f8df b2e4 	ldr.w	fp, [pc, #740]	; 30005080 <BOOT_LoadImages+0x810>
30004d9c:	9304      	str	r3, [sp, #16]
30004d9e:	e6c3      	b.n	30004b28 <BOOT_LoadImages+0x2b8>
30004da0:	48a1      	ldr	r0, [pc, #644]	; (30005028 <BOOT_LoadImages+0x7b8>)
30004da2:	930f      	str	r3, [sp, #60]	; 0x3c
30004da4:	9209      	str	r2, [sp, #36]	; 0x24
30004da6:	f006 fcdf 	bl	3000b768 <__DiagPrintf_veneer>
30004daa:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004dac:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004dae:	e636      	b.n	30004a1e <BOOT_LoadImages+0x1ae>
30004db0:	489e      	ldr	r0, [pc, #632]	; (3000502c <BOOT_LoadImages+0x7bc>)
30004db2:	9204      	str	r2, [sp, #16]
30004db4:	f006 fcd8 	bl	3000b768 <__DiagPrintf_veneer>
30004db8:	9a04      	ldr	r2, [sp, #16]
30004dba:	2a00      	cmp	r2, #0
30004dbc:	f43f af63 	beq.w	30004c86 <BOOT_LoadImages+0x416>
30004dc0:	9f06      	ldr	r7, [sp, #24]
30004dc2:	e5f7      	b.n	300049b4 <BOOT_LoadImages+0x144>
30004dc4:	2300      	movs	r3, #0
30004dc6:	4a97      	ldr	r2, [pc, #604]	; (30005024 <BOOT_LoadImages+0x7b4>)
30004dc8:	f8df 92b0 	ldr.w	r9, [pc, #688]	; 3000507c <BOOT_LoadImages+0x80c>
30004dcc:	f8df b2b0 	ldr.w	fp, [pc, #688]	; 30005080 <BOOT_LoadImages+0x810>
30004dd0:	469a      	mov	sl, r3
30004dd2:	9303      	str	r3, [sp, #12]
30004dd4:	9204      	str	r2, [sp, #16]
30004dd6:	2f00      	cmp	r7, #0
30004dd8:	f43f aec8 	beq.w	30004b6c <BOOT_LoadImages+0x2fc>
30004ddc:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004de0:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004de4:	3201      	adds	r2, #1
30004de6:	f000 819f 	beq.w	30005128 <BOOT_LoadImages+0x8b8>
30004dea:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004dee:	4890      	ldr	r0, [pc, #576]	; (30005030 <BOOT_LoadImages+0x7c0>)
30004df0:	f006 fcba 	bl	3000b768 <__DiagPrintf_veneer>
30004df4:	2302      	movs	r3, #2
30004df6:	9303      	str	r3, [sp, #12]
30004df8:	e6c6      	b.n	30004b88 <BOOT_LoadImages+0x318>
30004dfa:	f00a 0401 	and.w	r4, sl, #1
30004dfe:	b1c3      	cbz	r3, 30004e32 <BOOT_LoadImages+0x5c2>
30004e00:	b1bc      	cbz	r4, 30004e32 <BOOT_LoadImages+0x5c2>
30004e02:	4c8c      	ldr	r4, [pc, #560]	; (30005034 <BOOT_LoadImages+0x7c4>)
30004e04:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004e08:	47a0      	blx	r4
30004e0a:	e7fb      	b.n	30004e04 <BOOT_LoadImages+0x594>
30004e0c:	488a      	ldr	r0, [pc, #552]	; (30005038 <BOOT_LoadImages+0x7c8>)
30004e0e:	f006 fcab 	bl	3000b768 <__DiagPrintf_veneer>
30004e12:	9b08      	ldr	r3, [sp, #32]
30004e14:	9a04      	ldr	r2, [sp, #16]
30004e16:	4293      	cmp	r3, r2
30004e18:	f4ff ae1a 	bcc.w	30004a50 <BOOT_LoadImages+0x1e0>
30004e1c:	4887      	ldr	r0, [pc, #540]	; (3000503c <BOOT_LoadImages+0x7cc>)
30004e1e:	464d      	mov	r5, r9
30004e20:	f006 fca2 	bl	3000b768 <__DiagPrintf_veneer>
30004e24:	9b07      	ldr	r3, [sp, #28]
30004e26:	9f06      	ldr	r7, [sp, #24]
30004e28:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30004e2c:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004e30:	e615      	b.n	30004a5e <BOOT_LoadImages+0x1ee>
30004e32:	9b04      	ldr	r3, [sp, #16]
30004e34:	781b      	ldrb	r3, [r3, #0]
30004e36:	2b00      	cmp	r3, #0
30004e38:	f040 80c4 	bne.w	30004fc4 <BOOT_LoadImages+0x754>
30004e3c:	2001      	movs	r0, #1
30004e3e:	b037      	add	sp, #220	; 0xdc
30004e40:	ecbd 8b02 	vpop	{d8}
30004e44:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004e48:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30004e4c:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004e4e:	f10c 0104 	add.w	r1, ip, #4
30004e52:	2b00      	cmp	r3, #0
30004e54:	f000 8214 	beq.w	30005280 <BOOT_LoadImages+0xa10>
30004e58:	1d18      	adds	r0, r3, #4
30004e5a:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004e5e:	4460      	add	r0, ip
30004e60:	f811 3b01 	ldrb.w	r3, [r1], #1
30004e64:	4053      	eors	r3, r2
30004e66:	4281      	cmp	r1, r0
30004e68:	b2db      	uxtb	r3, r3
30004e6a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004e6e:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004e72:	d1f5      	bne.n	30004e60 <BOOT_LoadImages+0x5f0>
30004e74:	43d2      	mvns	r2, r2
30004e76:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004e78:	4293      	cmp	r3, r2
30004e7a:	f000 81cd 	beq.w	30005218 <BOOT_LoadImages+0x9a8>
30004e7e:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004e82:	f088 0301 	eor.w	r3, r8, #1
30004e86:	3101      	adds	r1, #1
30004e88:	f003 0301 	and.w	r3, r3, #1
30004e8c:	ea46 0603 	orr.w	r6, r6, r3
30004e90:	f000 8181 	beq.w	30005196 <BOOT_LoadImages+0x926>
30004e94:	990a      	ldr	r1, [sp, #40]	; 0x28
30004e96:	486a      	ldr	r0, [pc, #424]	; (30005040 <BOOT_LoadImages+0x7d0>)
30004e98:	f006 fc66 	bl	3000b768 <__DiagPrintf_veneer>
30004e9c:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30004ea0:	2002      	movs	r0, #2
30004ea2:	f7fe fe95 	bl	30003bd0 <FLASH_Erase_With_Lock>
30004ea6:	e635      	b.n	30004b14 <BOOT_LoadImages+0x2a4>
30004ea8:	f1b8 0f00 	cmp.w	r8, #0
30004eac:	f040 8227 	bne.w	300052fe <BOOT_LoadImages+0xa8e>
30004eb0:	2f01      	cmp	r7, #1
30004eb2:	f000 80e7 	beq.w	30005084 <BOOT_LoadImages+0x814>
30004eb6:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
30004eba:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004ebc:	1d31      	adds	r1, r6, #4
30004ebe:	2b00      	cmp	r3, #0
30004ec0:	f000 81ee 	beq.w	300052a0 <BOOT_LoadImages+0xa30>
30004ec4:	1d18      	adds	r0, r3, #4
30004ec6:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004eca:	4430      	add	r0, r6
30004ecc:	f811 3b01 	ldrb.w	r3, [r1], #1
30004ed0:	4053      	eors	r3, r2
30004ed2:	4281      	cmp	r1, r0
30004ed4:	b2db      	uxtb	r3, r3
30004ed6:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004eda:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004ede:	d1f5      	bne.n	30004ecc <BOOT_LoadImages+0x65c>
30004ee0:	43d2      	mvns	r2, r2
30004ee2:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004ee4:	4293      	cmp	r3, r2
30004ee6:	f000 81b8 	beq.w	3000525a <BOOT_LoadImages+0x9ea>
30004eea:	f8d6 300a 	ldr.w	r3, [r6, #10]
30004eee:	3301      	adds	r3, #1
30004ef0:	d079      	beq.n	30004fe6 <BOOT_LoadImages+0x776>
30004ef2:	990a      	ldr	r1, [sp, #40]	; 0x28
30004ef4:	4852      	ldr	r0, [pc, #328]	; (30005040 <BOOT_LoadImages+0x7d0>)
30004ef6:	f006 fc37 	bl	3000b768 <__DiagPrintf_veneer>
30004efa:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30004efe:	2002      	movs	r0, #2
30004f00:	f7fe fe66 	bl	30003bd0 <FLASH_Erase_With_Lock>
30004f04:	484f      	ldr	r0, [pc, #316]	; (30005044 <BOOT_LoadImages+0x7d4>)
30004f06:	f006 fc2f 	bl	3000b768 <__DiagPrintf_veneer>
30004f0a:	e7fe      	b.n	30004f0a <BOOT_LoadImages+0x69a>
30004f0c:	9908      	ldr	r1, [sp, #32]
30004f0e:	484e      	ldr	r0, [pc, #312]	; (30005048 <BOOT_LoadImages+0x7d8>)
30004f10:	f006 fc2a 	bl	3000b768 <__DiagPrintf_veneer>
30004f14:	9b07      	ldr	r3, [sp, #28]
30004f16:	3303      	adds	r3, #3
30004f18:	f240 81b6 	bls.w	30005288 <BOOT_LoadImages+0xa18>
30004f1c:	484b      	ldr	r0, [pc, #300]	; (3000504c <BOOT_LoadImages+0x7dc>)
30004f1e:	f006 fc23 	bl	3000b768 <__DiagPrintf_veneer>
30004f22:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
30004f26:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004f2a:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30004f2e:	9307      	str	r3, [sp, #28]
30004f30:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30004f34:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004f38:	4845      	ldr	r0, [pc, #276]	; (30005050 <BOOT_LoadImages+0x7e0>)
30004f3a:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004f3e:	5931      	ldr	r1, [r6, r4]
30004f40:	4430      	add	r0, r6
30004f42:	4c44      	ldr	r4, [pc, #272]	; (30005054 <BOOT_LoadImages+0x7e4>)
30004f44:	f810 3b01 	ldrb.w	r3, [r0], #1
30004f48:	4053      	eors	r3, r2
30004f4a:	4285      	cmp	r5, r0
30004f4c:	b2db      	uxtb	r3, r3
30004f4e:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004f52:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004f56:	d1f5      	bne.n	30004f44 <BOOT_LoadImages+0x6d4>
30004f58:	43d2      	mvns	r2, r2
30004f5a:	4291      	cmp	r1, r2
30004f5c:	d010      	beq.n	30004f80 <BOOT_LoadImages+0x710>
30004f5e:	483e      	ldr	r0, [pc, #248]	; (30005058 <BOOT_LoadImages+0x7e8>)
30004f60:	f006 fc02 	bl	3000b768 <__DiagPrintf_veneer>
30004f64:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004f66:	2b00      	cmp	r3, #0
30004f68:	f43f ae8d 	beq.w	30004c86 <BOOT_LoadImages+0x416>
30004f6c:	2300      	movs	r3, #0
30004f6e:	9309      	str	r3, [sp, #36]	; 0x24
30004f70:	e4e7      	b.n	30004942 <BOOT_LoadImages+0xd2>
30004f72:	460a      	mov	r2, r1
30004f74:	4839      	ldr	r0, [pc, #228]	; (3000505c <BOOT_LoadImages+0x7ec>)
30004f76:	f006 fbf7 	bl	3000b768 <__DiagPrintf_veneer>
30004f7a:	2201      	movs	r2, #1
30004f7c:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004f7e:	e4fc      	b.n	3000497a <BOOT_LoadImages+0x10a>
30004f80:	460a      	mov	r2, r1
30004f82:	4837      	ldr	r0, [pc, #220]	; (30005060 <BOOT_LoadImages+0x7f0>)
30004f84:	f006 fbf0 	bl	3000b768 <__DiagPrintf_veneer>
30004f88:	9b09      	ldr	r3, [sp, #36]	; 0x24
30004f8a:	2b00      	cmp	r3, #0
30004f8c:	f47f acd9 	bne.w	30004942 <BOOT_LoadImages+0xd2>
30004f90:	461a      	mov	r2, r3
30004f92:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30004f96:	e4f3      	b.n	30004980 <BOOT_LoadImages+0x110>
30004f98:	4832      	ldr	r0, [pc, #200]	; (30005064 <BOOT_LoadImages+0x7f4>)
30004f9a:	f006 fbe5 	bl	3000b768 <__DiagPrintf_veneer>
30004f9e:	e486      	b.n	300048ae <BOOT_LoadImages+0x3e>
30004fa0:	4c2c      	ldr	r4, [pc, #176]	; (30005054 <BOOT_LoadImages+0x7e4>)
30004fa2:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004fa6:	3310      	adds	r3, #16
30004fa8:	930e      	str	r3, [sp, #56]	; 0x38
30004faa:	2301      	movs	r3, #1
30004fac:	9303      	str	r3, [sp, #12]
30004fae:	e5c3      	b.n	30004b38 <BOOT_LoadImages+0x2c8>
30004fb0:	482d      	ldr	r0, [pc, #180]	; (30005068 <BOOT_LoadImages+0x7f8>)
30004fb2:	e65d      	b.n	30004c70 <BOOT_LoadImages+0x400>
30004fb4:	482d      	ldr	r0, [pc, #180]	; (3000506c <BOOT_LoadImages+0x7fc>)
30004fb6:	f006 fbd7 	bl	3000b768 <__DiagPrintf_veneer>
30004fba:	e478      	b.n	300048ae <BOOT_LoadImages+0x3e>
30004fbc:	482c      	ldr	r0, [pc, #176]	; (30005070 <BOOT_LoadImages+0x800>)
30004fbe:	f006 fbd3 	bl	3000b768 <__DiagPrintf_veneer>
30004fc2:	e660      	b.n	30004c86 <BOOT_LoadImages+0x416>
30004fc4:	4b2b      	ldr	r3, [pc, #172]	; (30005074 <BOOT_LoadImages+0x804>)
30004fc6:	4650      	mov	r0, sl
30004fc8:	4798      	blx	r3
30004fca:	1e03      	subs	r3, r0, #0
30004fcc:	bf18      	it	ne
30004fce:	2301      	movne	r3, #1
30004fd0:	b110      	cbz	r0, 30004fd8 <BOOT_LoadImages+0x768>
30004fd2:	2e00      	cmp	r6, #0
30004fd4:	f47f adbe 	bne.w	30004b54 <BOOT_LoadImages+0x2e4>
30004fd8:	2c00      	cmp	r4, #0
30004fda:	f43f af2f 	beq.w	30004e3c <BOOT_LoadImages+0x5cc>
30004fde:	2b00      	cmp	r3, #0
30004fe0:	f47f af0f 	bne.w	30004e02 <BOOT_LoadImages+0x592>
30004fe4:	e72a      	b.n	30004e3c <BOOT_LoadImages+0x5cc>
30004fe6:	4824      	ldr	r0, [pc, #144]	; (30005078 <BOOT_LoadImages+0x808>)
30004fe8:	f006 fbbe 	bl	3000b768 <__DiagPrintf_veneer>
30004fec:	e78a      	b.n	30004f04 <BOOT_LoadImages+0x694>
30004fee:	4293      	cmp	r3, r2
30004ff0:	f04f 0401 	mov.w	r4, #1
30004ff4:	464b      	mov	r3, r9
30004ff6:	4659      	mov	r1, fp
30004ff8:	bf28      	it	cs
30004ffa:	4630      	movcs	r0, r6
30004ffc:	9a07      	ldr	r2, [sp, #28]
30004ffe:	bf38      	it	cc
30005000:	4620      	movcc	r0, r4
30005002:	9500      	str	r5, [sp, #0]
30005004:	9401      	str	r4, [sp, #4]
30005006:	46b0      	mov	r8, r6
30005008:	f7fe feda 	bl	30003dc0 <rewrite_bp>
3000500c:	4626      	mov	r6, r4
3000500e:	b2c3      	uxtb	r3, r0
30005010:	f8df 9068 	ldr.w	r9, [pc, #104]	; 3000507c <BOOT_LoadImages+0x80c>
30005014:	46c2      	mov	sl, r8
30005016:	f8df b068 	ldr.w	fp, [pc, #104]	; 30005080 <BOOT_LoadImages+0x810>
3000501a:	9303      	str	r3, [sp, #12]
3000501c:	4b01      	ldr	r3, [pc, #4]	; (30005024 <BOOT_LoadImages+0x7b4>)
3000501e:	9304      	str	r3, [sp, #16]
30005020:	e582      	b.n	30004b28 <BOOT_LoadImages+0x2b8>
30005022:	bf00      	nop
30005024:	3000d93f 	.word	0x3000d93f
30005028:	3000bcdc 	.word	0x3000bcdc
3000502c:	3000bef8 	.word	0x3000bef8
30005030:	3000bacc 	.word	0x3000bacc
30005034:	00009be5 	.word	0x00009be5
30005038:	3000bcf8 	.word	0x3000bcf8
3000503c:	3000bd0c 	.word	0x3000bd0c
30005040:	3000be48 	.word	0x3000be48
30005044:	3000bae4 	.word	0x3000bae4
30005048:	3000bb98 	.word	0x3000bb98
3000504c:	3000bf14 	.word	0x3000bf14
30005050:	07ffe004 	.word	0x07ffe004
30005054:	3000db04 	.word	0x3000db04
30005058:	3000bc04 	.word	0x3000bc04
3000505c:	3000bc44 	.word	0x3000bc44
30005060:	3000bbc8 	.word	0x3000bbc8
30005064:	3000bb28 	.word	0x3000bb28
30005068:	3000bf2c 	.word	0x3000bf2c
3000506c:	3000bb3c 	.word	0x3000bb3c
30005070:	3000bf48 	.word	0x3000bf48
30005074:	300065d9 	.word	0x300065d9
30005078:	3000bd74 	.word	0x3000bd74
3000507c:	3000edd8 	.word	0x3000edd8
30005080:	300065d5 	.word	0x300065d5
30005084:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
30005088:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000508a:	1d31      	adds	r1, r6, #4
3000508c:	2b00      	cmp	r3, #0
3000508e:	f000 8123 	beq.w	300052d8 <BOOT_LoadImages+0xa68>
30005092:	1d18      	adds	r0, r3, #4
30005094:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30005098:	4430      	add	r0, r6
3000509a:	f811 3b01 	ldrb.w	r3, [r1], #1
3000509e:	4053      	eors	r3, r2
300050a0:	4281      	cmp	r1, r0
300050a2:	b2db      	uxtb	r3, r3
300050a4:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300050a8:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300050ac:	d1f5      	bne.n	3000509a <BOOT_LoadImages+0x82a>
300050ae:	43d2      	mvns	r2, r2
300050b0:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300050b2:	4293      	cmp	r3, r2
300050b4:	f000 80f6 	beq.w	300052a4 <BOOT_LoadImages+0xa34>
300050b8:	f8d6 300a 	ldr.w	r3, [r6, #10]
300050bc:	3301      	adds	r3, #1
300050be:	d07f      	beq.n	300051c0 <BOOT_LoadImages+0x950>
300050c0:	990b      	ldr	r1, [sp, #44]	; 0x2c
300050c2:	48a1      	ldr	r0, [pc, #644]	; (30005348 <BOOT_LoadImages+0xad8>)
300050c4:	f006 fb50 	bl	3000b768 <__DiagPrintf_veneer>
300050c8:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
300050cc:	2002      	movs	r0, #2
300050ce:	f7fe fd7f 	bl	30003bd0 <FLASH_Erase_With_Lock>
300050d2:	e717      	b.n	30004f04 <BOOT_LoadImages+0x694>
300050d4:	ab16      	add	r3, sp, #88	; 0x58
300050d6:	489d      	ldr	r0, [pc, #628]	; (3000534c <BOOT_LoadImages+0xadc>)
300050d8:	461e      	mov	r6, r3
300050da:	ee08 3a10 	vmov	s16, r3
300050de:	f006 fb43 	bl	3000b768 <__DiagPrintf_veneer>
300050e2:	2280      	movs	r2, #128	; 0x80
300050e4:	21ff      	movs	r1, #255	; 0xff
300050e6:	4630      	mov	r0, r6
300050e8:	f006 fb16 	bl	3000b718 <____wrap_memset_veneer>
300050ec:	2280      	movs	r2, #128	; 0x80
300050ee:	4629      	mov	r1, r5
300050f0:	4630      	mov	r0, r6
300050f2:	f006 faf9 	bl	3000b6e8 <____wrap_memcpy_veneer>
300050f6:	f1ba 0f00 	cmp.w	sl, #0
300050fa:	d050      	beq.n	3000519e <BOOT_LoadImages+0x92e>
300050fc:	46b8      	mov	r8, r7
300050fe:	e55d      	b.n	30004bbc <BOOT_LoadImages+0x34c>
30005100:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005104:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005108:	3201      	adds	r2, #1
3000510a:	d059      	beq.n	300051c0 <BOOT_LoadImages+0x950>
3000510c:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005110:	488f      	ldr	r0, [pc, #572]	; (30005350 <BOOT_LoadImages+0xae0>)
30005112:	f006 fb29 	bl	3000b768 <__DiagPrintf_veneer>
30005116:	9705      	str	r7, [sp, #20]
30005118:	f08a 0601 	eor.w	r6, sl, #1
3000511c:	f04f 0801 	mov.w	r8, #1
30005120:	f006 0601 	and.w	r6, r6, #1
30005124:	4647      	mov	r7, r8
30005126:	e73c      	b.n	30004fa2 <BOOT_LoadImages+0x732>
30005128:	ab16      	add	r3, sp, #88	; 0x58
3000512a:	488a      	ldr	r0, [pc, #552]	; (30005354 <BOOT_LoadImages+0xae4>)
3000512c:	461e      	mov	r6, r3
3000512e:	ee08 3a10 	vmov	s16, r3
30005132:	f006 fb19 	bl	3000b768 <__DiagPrintf_veneer>
30005136:	2280      	movs	r2, #128	; 0x80
30005138:	21ff      	movs	r1, #255	; 0xff
3000513a:	4630      	mov	r0, r6
3000513c:	f006 faec 	bl	3000b718 <____wrap_memset_veneer>
30005140:	2280      	movs	r2, #128	; 0x80
30005142:	4629      	mov	r1, r5
30005144:	4630      	mov	r0, r6
30005146:	f006 facf 	bl	3000b6e8 <____wrap_memcpy_veneer>
3000514a:	f1ba 0f00 	cmp.w	sl, #0
3000514e:	d130      	bne.n	300051b2 <BOOT_LoadImages+0x942>
30005150:	2301      	movs	r3, #1
30005152:	9305      	str	r3, [sp, #20]
30005154:	46d0      	mov	r8, sl
30005156:	f04f 0a00 	mov.w	sl, #0
3000515a:	4651      	mov	r1, sl
3000515c:	f88d a064 	strb.w	sl, [sp, #100]	; 0x64
30005160:	e533      	b.n	30004bca <BOOT_LoadImages+0x35a>
30005162:	ab16      	add	r3, sp, #88	; 0x58
30005164:	2280      	movs	r2, #128	; 0x80
30005166:	21ff      	movs	r1, #255	; 0xff
30005168:	461e      	mov	r6, r3
3000516a:	4618      	mov	r0, r3
3000516c:	ee08 3a10 	vmov	s16, r3
30005170:	f006 fad2 	bl	3000b718 <____wrap_memset_veneer>
30005174:	2280      	movs	r2, #128	; 0x80
30005176:	4629      	mov	r1, r5
30005178:	4630      	mov	r0, r6
3000517a:	f006 fab5 	bl	3000b6e8 <____wrap_memcpy_veneer>
3000517e:	f1ba 0f00 	cmp.w	sl, #0
30005182:	f040 80d9 	bne.w	30005338 <BOOT_LoadImages+0xac8>
30005186:	b9bf      	cbnz	r7, 300051b8 <BOOT_LoadImages+0x948>
30005188:	46c2      	mov	sl, r8
3000518a:	9705      	str	r7, [sp, #20]
3000518c:	e009      	b.n	300051a2 <BOOT_LoadImages+0x932>
3000518e:	486f      	ldr	r0, [pc, #444]	; (3000534c <BOOT_LoadImages+0xadc>)
30005190:	f006 faea 	bl	3000b768 <__DiagPrintf_veneer>
30005194:	e4be      	b.n	30004b14 <BOOT_LoadImages+0x2a4>
30005196:	486f      	ldr	r0, [pc, #444]	; (30005354 <BOOT_LoadImages+0xae4>)
30005198:	f006 fae6 	bl	3000b768 <__DiagPrintf_veneer>
3000519c:	e4ba      	b.n	30004b14 <BOOT_LoadImages+0x2a4>
3000519e:	2301      	movs	r3, #1
300051a0:	9305      	str	r3, [sp, #20]
300051a2:	46d0      	mov	r8, sl
300051a4:	2101      	movs	r1, #1
300051a6:	f04f 0a00 	mov.w	sl, #0
300051aa:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300051ae:	4657      	mov	r7, sl
300051b0:	e50b      	b.n	30004bca <BOOT_LoadImages+0x35a>
300051b2:	f04f 0800 	mov.w	r8, #0
300051b6:	e4fe      	b.n	30004bb6 <BOOT_LoadImages+0x346>
300051b8:	f8cd a014 	str.w	sl, [sp, #20]
300051bc:	46c2      	mov	sl, r8
300051be:	e7c9      	b.n	30005154 <BOOT_LoadImages+0x8e4>
300051c0:	4862      	ldr	r0, [pc, #392]	; (3000534c <BOOT_LoadImages+0xadc>)
300051c2:	f006 fad1 	bl	3000b768 <__DiagPrintf_veneer>
300051c6:	e69d      	b.n	30004f04 <BOOT_LoadImages+0x694>
300051c8:	4620      	mov	r0, r4
300051ca:	e5db      	b.n	30004d84 <BOOT_LoadImages+0x514>
300051cc:	45e2      	cmp	sl, ip
300051ce:	f67f abed 	bls.w	300049ac <BOOT_LoadImages+0x13c>
300051d2:	e432      	b.n	30004a3a <BOOT_LoadImages+0x1ca>
300051d4:	428a      	cmp	r2, r1
300051d6:	f4ff ad89 	bcc.w	30004cec <BOOT_LoadImages+0x47c>
300051da:	4282      	cmp	r2, r0
300051dc:	f4ff ad86 	bcc.w	30004cec <BOOT_LoadImages+0x47c>
300051e0:	2601      	movs	r6, #1
300051e2:	464b      	mov	r3, r9
300051e4:	9a07      	ldr	r2, [sp, #28]
300051e6:	4659      	mov	r1, fp
300051e8:	4630      	mov	r0, r6
300051ea:	9500      	str	r5, [sp, #0]
300051ec:	9601      	str	r6, [sp, #4]
300051ee:	f7fe fde7 	bl	30003dc0 <rewrite_bp>
300051f2:	f04f 0800 	mov.w	r8, #0
300051f6:	b2c3      	uxtb	r3, r0
300051f8:	f8df 9180 	ldr.w	r9, [pc, #384]	; 3000537c <BOOT_LoadImages+0xb0c>
300051fc:	f8df b180 	ldr.w	fp, [pc, #384]	; 30005380 <BOOT_LoadImages+0xb10>
30005200:	46c2      	mov	sl, r8
30005202:	9303      	str	r3, [sp, #12]
30005204:	4b54      	ldr	r3, [pc, #336]	; (30005358 <BOOT_LoadImages+0xae8>)
30005206:	9304      	str	r3, [sp, #16]
30005208:	e48e      	b.n	30004b28 <BOOT_LoadImages+0x2b8>
3000520a:	f8cd a014 	str.w	sl, [sp, #20]
3000520e:	4639      	mov	r1, r7
30005210:	46ba      	mov	sl, r7
30005212:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
30005216:	e4d8      	b.n	30004bca <BOOT_LoadImages+0x35a>
30005218:	461a      	mov	r2, r3
3000521a:	4619      	mov	r1, r3
3000521c:	484f      	ldr	r0, [pc, #316]	; (3000535c <BOOT_LoadImages+0xaec>)
3000521e:	f04f 0801 	mov.w	r8, #1
30005222:	f006 faa1 	bl	3000b768 <__DiagPrintf_veneer>
30005226:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000522a:	484d      	ldr	r0, [pc, #308]	; (30005360 <BOOT_LoadImages+0xaf0>)
3000522c:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005230:	f006 fa9a 	bl	3000b768 <__DiagPrintf_veneer>
30005234:	2302      	movs	r3, #2
30005236:	9303      	str	r3, [sp, #12]
30005238:	e46c      	b.n	30004b14 <BOOT_LoadImages+0x2a4>
3000523a:	461a      	mov	r2, r3
3000523c:	4619      	mov	r1, r3
3000523e:	4849      	ldr	r0, [pc, #292]	; (30005364 <BOOT_LoadImages+0xaf4>)
30005240:	f006 fa92 	bl	3000b768 <__DiagPrintf_veneer>
30005244:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005248:	4841      	ldr	r0, [pc, #260]	; (30005350 <BOOT_LoadImages+0xae0>)
3000524a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000524e:	f006 fa8b 	bl	3000b768 <__DiagPrintf_veneer>
30005252:	2301      	movs	r3, #1
30005254:	4698      	mov	r8, r3
30005256:	9303      	str	r3, [sp, #12]
30005258:	e45c      	b.n	30004b14 <BOOT_LoadImages+0x2a4>
3000525a:	461a      	mov	r2, r3
3000525c:	4619      	mov	r1, r3
3000525e:	483f      	ldr	r0, [pc, #252]	; (3000535c <BOOT_LoadImages+0xaec>)
30005260:	f006 fa82 	bl	3000b768 <__DiagPrintf_veneer>
30005264:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005268:	483d      	ldr	r0, [pc, #244]	; (30005360 <BOOT_LoadImages+0xaf0>)
3000526a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000526e:	f006 fa7b 	bl	3000b768 <__DiagPrintf_veneer>
30005272:	2300      	movs	r3, #0
30005274:	9305      	str	r3, [sp, #20]
30005276:	e4e7      	b.n	30004c48 <BOOT_LoadImages+0x3d8>
30005278:	483b      	ldr	r0, [pc, #236]	; (30005368 <BOOT_LoadImages+0xaf8>)
3000527a:	f006 fa75 	bl	3000b768 <__DiagPrintf_veneer>
3000527e:	e7fe      	b.n	3000527e <BOOT_LoadImages+0xa0e>
30005280:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30005282:	e5f8      	b.n	30004e76 <BOOT_LoadImages+0x606>
30005284:	9a0d      	ldr	r2, [sp, #52]	; 0x34
30005286:	e42d      	b.n	30004ae4 <BOOT_LoadImages+0x274>
30005288:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
3000528c:	f11a 0f03 	cmn.w	sl, #3
30005290:	d824      	bhi.n	300052dc <BOOT_LoadImages+0xa6c>
30005292:	f1bb 0f01 	cmp.w	fp, #1
30005296:	d937      	bls.n	30005308 <BOOT_LoadImages+0xa98>
30005298:	4834      	ldr	r0, [pc, #208]	; (3000536c <BOOT_LoadImages+0xafc>)
3000529a:	e640      	b.n	30004f1e <BOOT_LoadImages+0x6ae>
3000529c:	4c34      	ldr	r4, [pc, #208]	; (30005370 <BOOT_LoadImages+0xb00>)
3000529e:	e4d9      	b.n	30004c54 <BOOT_LoadImages+0x3e4>
300052a0:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300052a2:	e61e      	b.n	30004ee2 <BOOT_LoadImages+0x672>
300052a4:	461a      	mov	r2, r3
300052a6:	4619      	mov	r1, r3
300052a8:	482e      	ldr	r0, [pc, #184]	; (30005364 <BOOT_LoadImages+0xaf4>)
300052aa:	f006 fa5d 	bl	3000b768 <__DiagPrintf_veneer>
300052ae:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300052b2:	4827      	ldr	r0, [pc, #156]	; (30005350 <BOOT_LoadImages+0xae0>)
300052b4:	f8d3 1006 	ldr.w	r1, [r3, #6]
300052b8:	f006 fa56 	bl	3000b768 <__DiagPrintf_veneer>
300052bc:	2300      	movs	r3, #0
300052be:	9305      	str	r3, [sp, #20]
300052c0:	e72a      	b.n	30005118 <BOOT_LoadImages+0x8a8>
300052c2:	4281      	cmp	r1, r0
300052c4:	f4ff ad18 	bcc.w	30004cf8 <BOOT_LoadImages+0x488>
300052c8:	2400      	movs	r4, #0
300052ca:	464b      	mov	r3, r9
300052cc:	4642      	mov	r2, r8
300052ce:	4651      	mov	r1, sl
300052d0:	4620      	mov	r0, r4
300052d2:	9500      	str	r5, [sp, #0]
300052d4:	9401      	str	r4, [sp, #4]
300052d6:	e555      	b.n	30004d84 <BOOT_LoadImages+0x514>
300052d8:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300052da:	e6e9      	b.n	300050b0 <BOOT_LoadImages+0x840>
300052dc:	4825      	ldr	r0, [pc, #148]	; (30005374 <BOOT_LoadImages+0xb04>)
300052de:	e61e      	b.n	30004f1e <BOOT_LoadImages+0x6ae>
300052e0:	2601      	movs	r6, #1
300052e2:	464b      	mov	r3, r9
300052e4:	9a07      	ldr	r2, [sp, #28]
300052e6:	4659      	mov	r1, fp
300052e8:	2000      	movs	r0, #0
300052ea:	9500      	str	r5, [sp, #0]
300052ec:	9601      	str	r6, [sp, #4]
300052ee:	e77e      	b.n	300051ee <BOOT_LoadImages+0x97e>
300052f0:	2400      	movs	r4, #0
300052f2:	464b      	mov	r3, r9
300052f4:	4642      	mov	r2, r8
300052f6:	4651      	mov	r1, sl
300052f8:	9500      	str	r5, [sp, #0]
300052fa:	9401      	str	r4, [sp, #4]
300052fc:	e541      	b.n	30004d82 <BOOT_LoadImages+0x512>
300052fe:	f08a 0601 	eor.w	r6, sl, #1
30005302:	f006 0601 	and.w	r6, r6, #1
30005306:	e40f      	b.n	30004b28 <BOOT_LoadImages+0x2b8>
30005308:	9904      	ldr	r1, [sp, #16]
3000530a:	481b      	ldr	r0, [pc, #108]	; (30005378 <BOOT_LoadImages+0xb08>)
3000530c:	f006 fa2c 	bl	3000b768 <__DiagPrintf_veneer>
30005310:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005314:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30005318:	9307      	str	r3, [sp, #28]
3000531a:	2301      	movs	r3, #1
3000531c:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30005320:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005324:	9309      	str	r3, [sp, #36]	; 0x24
30005326:	e607      	b.n	30004f38 <BOOT_LoadImages+0x6c8>
30005328:	f08a 0601 	eor.w	r6, sl, #1
3000532c:	f8cd 8014 	str.w	r8, [sp, #20]
30005330:	f006 0601 	and.w	r6, r6, #1
30005334:	f7ff bbf8 	b.w	30004b28 <BOOT_LoadImages+0x2b8>
30005338:	f04f 0a00 	mov.w	sl, #0
3000533c:	e43b      	b.n	30004bb6 <BOOT_LoadImages+0x346>
3000533e:	2601      	movs	r6, #1
30005340:	4c0b      	ldr	r4, [pc, #44]	; (30005370 <BOOT_LoadImages+0xb00>)
30005342:	46b2      	mov	sl, r6
30005344:	f7ff bbb4 	b.w	30004ab0 <BOOT_LoadImages+0x240>
30005348:	3000bdc0 	.word	0x3000bdc0
3000534c:	3000bd64 	.word	0x3000bd64
30005350:	3000bab4 	.word	0x3000bab4
30005354:	3000bd74 	.word	0x3000bd74
30005358:	3000d93f 	.word	0x3000d93f
3000535c:	3000be0c 	.word	0x3000be0c
30005360:	3000bacc 	.word	0x3000bacc
30005364:	3000bd84 	.word	0x3000bd84
30005368:	3000becc 	.word	0x3000becc
3000536c:	3000bf2c 	.word	0x3000bf2c
30005370:	3000db04 	.word	0x3000db04
30005374:	3000bbb4 	.word	0x3000bbb4
30005378:	3000bf64 	.word	0x3000bf64
3000537c:	3000edd8 	.word	0x3000edd8
30005380:	300065d5 	.word	0x300065d5

30005384 <BOOT_ReasonSet>:
30005384:	4a03      	ldr	r2, [pc, #12]	; (30005394 <BOOT_ReasonSet+0x10>)
30005386:	4904      	ldr	r1, [pc, #16]	; (30005398 <BOOT_ReasonSet+0x14>)
30005388:	8c13      	ldrh	r3, [r2, #32]
3000538a:	b29b      	uxth	r3, r3
3000538c:	8413      	strh	r3, [r2, #32]
3000538e:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005392:	4770      	bx	lr
30005394:	42008000 	.word	0x42008000
30005398:	42008200 	.word	0x42008200

3000539c <BOOT_Enable_KM0>:
3000539c:	b510      	push	{r4, lr}
3000539e:	4c12      	ldr	r4, [pc, #72]	; (300053e8 <BOOT_Enable_KM0+0x4c>)
300053a0:	47a0      	blx	r4
300053a2:	b110      	cbz	r0, 300053aa <BOOT_Enable_KM0+0xe>
300053a4:	4b11      	ldr	r3, [pc, #68]	; (300053ec <BOOT_Enable_KM0+0x50>)
300053a6:	4798      	blx	r3
300053a8:	b1a8      	cbz	r0, 300053d6 <BOOT_Enable_KM0+0x3a>
300053aa:	47a0      	blx	r4
300053ac:	b940      	cbnz	r0, 300053c0 <BOOT_Enable_KM0+0x24>
300053ae:	4a10      	ldr	r2, [pc, #64]	; (300053f0 <BOOT_Enable_KM0+0x54>)
300053b0:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300053b4:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300053b8:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300053bc:	bf40      	sev
300053be:	bd10      	pop	{r4, pc}
300053c0:	4b0a      	ldr	r3, [pc, #40]	; (300053ec <BOOT_Enable_KM0+0x50>)
300053c2:	4798      	blx	r3
300053c4:	2800      	cmp	r0, #0
300053c6:	d0f2      	beq.n	300053ae <BOOT_Enable_KM0+0x12>
300053c8:	4b0a      	ldr	r3, [pc, #40]	; (300053f4 <BOOT_Enable_KM0+0x58>)
300053ca:	2004      	movs	r0, #4
300053cc:	4a0a      	ldr	r2, [pc, #40]	; (300053f8 <BOOT_Enable_KM0+0x5c>)
300053ce:	4619      	mov	r1, r3
300053d0:	f005 ff2c 	bl	3000b22c <rtk_log_write>
300053d4:	e7eb      	b.n	300053ae <BOOT_Enable_KM0+0x12>
300053d6:	4b07      	ldr	r3, [pc, #28]	; (300053f4 <BOOT_Enable_KM0+0x58>)
300053d8:	2004      	movs	r0, #4
300053da:	4a08      	ldr	r2, [pc, #32]	; (300053fc <BOOT_Enable_KM0+0x60>)
300053dc:	4619      	mov	r1, r3
300053de:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300053e2:	f005 bf23 	b.w	3000b22c <rtk_log_write>
300053e6:	bf00      	nop
300053e8:	0000c09d 	.word	0x0000c09d
300053ec:	0000c0ad 	.word	0x0000c0ad
300053f0:	42008000 	.word	0x42008000
300053f4:	3000b9d0 	.word	0x3000b9d0
300053f8:	3000bfa8 	.word	0x3000bfa8
300053fc:	3000bf80 	.word	0x3000bf80

30005400 <BOOT_AP_Clk_Get>:
30005400:	4a06      	ldr	r2, [pc, #24]	; (3000541c <BOOT_AP_Clk_Get+0x1c>)
30005402:	6913      	ldr	r3, [r2, #16]
30005404:	2b00      	cmp	r3, #0
30005406:	db03      	blt.n	30005410 <BOOT_AP_Clk_Get+0x10>
30005408:	6810      	ldr	r0, [r2, #0]
3000540a:	fbb0 f0f3 	udiv	r0, r0, r3
3000540e:	4770      	bx	lr
30005410:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30005414:	6850      	ldr	r0, [r2, #4]
30005416:	fbb0 f0f3 	udiv	r0, r0, r3
3000541a:	4770      	bx	lr
3000541c:	3000e014 	.word	0x3000e014

30005420 <BOOT_SOC_ClkSet>:
30005420:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005424:	4c5d      	ldr	r4, [pc, #372]	; (3000559c <BOOT_SOC_ClkSet+0x17c>)
30005426:	b083      	sub	sp, #12
30005428:	6926      	ldr	r6, [r4, #16]
3000542a:	6825      	ldr	r5, [r4, #0]
3000542c:	2e00      	cmp	r6, #0
3000542e:	db7a      	blt.n	30005526 <BOOT_SOC_ClkSet+0x106>
30005430:	fbb5 f6f6 	udiv	r6, r5, r6
30005434:	4b5a      	ldr	r3, [pc, #360]	; (300055a0 <BOOT_SOC_ClkSet+0x180>)
30005436:	68a7      	ldr	r7, [r4, #8]
30005438:	fba3 1305 	umull	r1, r3, r3, r5
3000543c:	68e2      	ldr	r2, [r4, #12]
3000543e:	2f01      	cmp	r7, #1
30005440:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
30005444:	ea4f 1b53 	mov.w	fp, r3, lsr #5
30005448:	ea4f 1393 	mov.w	r3, r3, lsr #6
3000544c:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30005450:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30005454:	f103 39ff 	add.w	r9, r3, #4294967295	; 0xffffffff
30005458:	fbb5 f5f2 	udiv	r5, r5, r2
3000545c:	d06f      	beq.n	3000553e <BOOT_SOC_ClkSet+0x11e>
3000545e:	4b51      	ldr	r3, [pc, #324]	; (300055a4 <BOOT_SOC_ClkSet+0x184>)
30005460:	2200      	movs	r2, #0
30005462:	2dfa      	cmp	r5, #250	; 0xfa
30005464:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30005468:	d863      	bhi.n	30005532 <BOOT_SOC_ClkSet+0x112>
3000546a:	4f4f      	ldr	r7, [pc, #316]	; (300055a8 <BOOT_SOC_ClkSet+0x188>)
3000546c:	783b      	ldrb	r3, [r7, #0]
3000546e:	2b01      	cmp	r3, #1
30005470:	d103      	bne.n	3000547a <BOOT_SOC_ClkSet+0x5a>
30005472:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
30005476:	f200 8085 	bhi.w	30005584 <BOOT_SOC_ClkSet+0x164>
3000547a:	2001      	movs	r0, #1
3000547c:	f8df 8154 	ldr.w	r8, [pc, #340]	; 300055d4 <BOOT_SOC_ClkSet+0x1b4>
30005480:	f001 fecc 	bl	3000721c <CLK_SWITCH_XTAL>
30005484:	6820      	ldr	r0, [r4, #0]
30005486:	4b49      	ldr	r3, [pc, #292]	; (300055ac <BOOT_SOC_ClkSet+0x18c>)
30005488:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
3000548c:	fb08 f000 	mul.w	r0, r8, r0
30005490:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
30005494:	4798      	blx	r3
30005496:	4a46      	ldr	r2, [pc, #280]	; (300055b0 <BOOT_SOC_ClkSet+0x190>)
30005498:	68e1      	ldr	r1, [r4, #12]
3000549a:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
3000549e:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
300054a2:	ea4f 5909 	mov.w	r9, r9, lsl #20
300054a6:	1e4b      	subs	r3, r1, #1
300054a8:	4942      	ldr	r1, [pc, #264]	; (300055b4 <BOOT_SOC_ClkSet+0x194>)
300054aa:	fa1f fb8b 	uxth.w	fp, fp
300054ae:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
300054b2:	4001      	ands	r1, r0
300054b4:	f003 0307 	and.w	r3, r3, #7
300054b8:	2000      	movs	r0, #0
300054ba:	430b      	orrs	r3, r1
300054bc:	ea4a 0303 	orr.w	r3, sl, r3
300054c0:	ea4b 0303 	orr.w	r3, fp, r3
300054c4:	ea49 0303 	orr.w	r3, r9, r3
300054c8:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
300054cc:	f001 fea6 	bl	3000721c <CLK_SWITCH_XTAL>
300054d0:	4b39      	ldr	r3, [pc, #228]	; (300055b8 <BOOT_SOC_ClkSet+0x198>)
300054d2:	4a3a      	ldr	r2, [pc, #232]	; (300055bc <BOOT_SOC_ClkSet+0x19c>)
300054d4:	2004      	movs	r0, #4
300054d6:	4619      	mov	r1, r3
300054d8:	9500      	str	r5, [sp, #0]
300054da:	f005 fea7 	bl	3000b22c <rtk_log_write>
300054de:	783b      	ldrb	r3, [r7, #0]
300054e0:	b1f3      	cbz	r3, 30005520 <BOOT_SOC_ClkSet+0x100>
300054e2:	6923      	ldr	r3, [r4, #16]
300054e4:	2b00      	cmp	r3, #0
300054e6:	db40      	blt.n	3000556a <BOOT_SOC_ClkSet+0x14a>
300054e8:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300054ec:	492d      	ldr	r1, [pc, #180]	; (300055a4 <BOOT_SOC_ClkSet+0x184>)
300054ee:	2000      	movs	r0, #0
300054f0:	6913      	ldr	r3, [r2, #16]
300054f2:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
300054f6:	f043 030c 	orr.w	r3, r3, #12
300054fa:	6113      	str	r3, [r2, #16]
300054fc:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
30005500:	6921      	ldr	r1, [r4, #16]
30005502:	4b2d      	ldr	r3, [pc, #180]	; (300055b8 <BOOT_SOC_ClkSet+0x198>)
30005504:	6928      	ldr	r0, [r5, #16]
30005506:	3901      	subs	r1, #1
30005508:	4a2d      	ldr	r2, [pc, #180]	; (300055c0 <BOOT_SOC_ClkSet+0x1a0>)
3000550a:	f020 0003 	bic.w	r0, r0, #3
3000550e:	f001 0103 	and.w	r1, r1, #3
30005512:	4301      	orrs	r1, r0
30005514:	2004      	movs	r0, #4
30005516:	6129      	str	r1, [r5, #16]
30005518:	4619      	mov	r1, r3
3000551a:	9600      	str	r6, [sp, #0]
3000551c:	f005 fe86 	bl	3000b22c <rtk_log_write>
30005520:	b003      	add	sp, #12
30005522:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005526:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
3000552a:	6863      	ldr	r3, [r4, #4]
3000552c:	fbb3 f6f6 	udiv	r6, r3, r6
30005530:	e780      	b.n	30005434 <BOOT_SOC_ClkSet+0x14>
30005532:	f240 61bf 	movw	r1, #1727	; 0x6bf
30005536:	4823      	ldr	r0, [pc, #140]	; (300055c4 <BOOT_SOC_ClkSet+0x1a4>)
30005538:	f006 f94e 	bl	3000b7d8 <__io_assert_failed_veneer>
3000553c:	e795      	b.n	3000546a <BOOT_SOC_ClkSet+0x4a>
3000553e:	4b22      	ldr	r3, [pc, #136]	; (300055c8 <BOOT_SOC_ClkSet+0x1a8>)
30005540:	4638      	mov	r0, r7
30005542:	4798      	blx	r3
30005544:	4b17      	ldr	r3, [pc, #92]	; (300055a4 <BOOT_SOC_ClkSet+0x184>)
30005546:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
3000554a:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
3000554e:	d21f      	bcs.n	30005590 <BOOT_SOC_ClkSet+0x170>
30005550:	4f15      	ldr	r7, [pc, #84]	; (300055a8 <BOOT_SOC_ClkSet+0x188>)
30005552:	783b      	ldrb	r3, [r7, #0]
30005554:	2b01      	cmp	r3, #1
30005556:	d190      	bne.n	3000547a <BOOT_SOC_ClkSet+0x5a>
30005558:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
3000555c:	d98d      	bls.n	3000547a <BOOT_SOC_ClkSet+0x5a>
3000555e:	f240 61bb 	movw	r1, #1723	; 0x6bb
30005562:	4818      	ldr	r0, [pc, #96]	; (300055c4 <BOOT_SOC_ClkSet+0x1a4>)
30005564:	f006 f938 	bl	3000b7d8 <__io_assert_failed_veneer>
30005568:	e787      	b.n	3000547a <BOOT_SOC_ClkSet+0x5a>
3000556a:	6860      	ldr	r0, [r4, #4]
3000556c:	4b17      	ldr	r3, [pc, #92]	; (300055cc <BOOT_SOC_ClkSet+0x1ac>)
3000556e:	fb08 f000 	mul.w	r0, r8, r0
30005572:	4798      	blx	r3
30005574:	4b16      	ldr	r3, [pc, #88]	; (300055d0 <BOOT_SOC_ClkSet+0x1b0>)
30005576:	2001      	movs	r0, #1
30005578:	4798      	blx	r3
3000557a:	4b0a      	ldr	r3, [pc, #40]	; (300055a4 <BOOT_SOC_ClkSet+0x184>)
3000557c:	2201      	movs	r2, #1
3000557e:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30005582:	e7bb      	b.n	300054fc <BOOT_SOC_ClkSet+0xdc>
30005584:	f240 61c1 	movw	r1, #1729	; 0x6c1
30005588:	480e      	ldr	r0, [pc, #56]	; (300055c4 <BOOT_SOC_ClkSet+0x1a4>)
3000558a:	f006 f925 	bl	3000b7d8 <__io_assert_failed_veneer>
3000558e:	e774      	b.n	3000547a <BOOT_SOC_ClkSet+0x5a>
30005590:	f240 61b9 	movw	r1, #1721	; 0x6b9
30005594:	480b      	ldr	r0, [pc, #44]	; (300055c4 <BOOT_SOC_ClkSet+0x1a4>)
30005596:	f006 f91f 	bl	3000b7d8 <__io_assert_failed_veneer>
3000559a:	e7d9      	b.n	30005550 <BOOT_SOC_ClkSet+0x130>
3000559c:	3000e014 	.word	0x3000e014
300055a0:	51eb851f 	.word	0x51eb851f
300055a4:	23020000 	.word	0x23020000
300055a8:	3000d93f 	.word	0x3000d93f
300055ac:	30007e45 	.word	0x30007e45
300055b0:	42008000 	.word	0x42008000
300055b4:	ff8f08f8 	.word	0xff8f08f8
300055b8:	3000b9d0 	.word	0x3000b9d0
300055bc:	3000bfc8 	.word	0x3000bfc8
300055c0:	3000bfe0 	.word	0x3000bfe0
300055c4:	3000d604 	.word	0x3000d604
300055c8:	3000abad 	.word	0x3000abad
300055cc:	30007ec5 	.word	0x30007ec5
300055d0:	30007f05 	.word	0x30007f05
300055d4:	000f4240 	.word	0x000f4240

300055d8 <BOOT_Disable_AP>:
300055d8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300055dc:	4906      	ldr	r1, [pc, #24]	; (300055f8 <BOOT_Disable_AP+0x20>)
300055de:	685a      	ldr	r2, [r3, #4]
300055e0:	f042 0202 	orr.w	r2, r2, #2
300055e4:	605a      	str	r2, [r3, #4]
300055e6:	685a      	ldr	r2, [r3, #4]
300055e8:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300055ec:	605a      	str	r2, [r3, #4]
300055ee:	6f8b      	ldr	r3, [r1, #120]	; 0x78
300055f0:	f043 0303 	orr.w	r3, r3, #3
300055f4:	678b      	str	r3, [r1, #120]	; 0x78
300055f6:	4770      	bx	lr
300055f8:	41000200 	.word	0x41000200

300055fc <BOOT_Enable_AP>:
300055fc:	b538      	push	{r3, r4, r5, lr}
300055fe:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30005602:	2201      	movs	r2, #1
30005604:	2104      	movs	r1, #4
30005606:	4b2b      	ldr	r3, [pc, #172]	; (300056b4 <BOOT_Enable_AP+0xb8>)
30005608:	482b      	ldr	r0, [pc, #172]	; (300056b8 <BOOT_Enable_AP+0xbc>)
3000560a:	4798      	blx	r3
3000560c:	6863      	ldr	r3, [r4, #4]
3000560e:	4d2b      	ldr	r5, [pc, #172]	; (300056bc <BOOT_Enable_AP+0xc0>)
30005610:	2032      	movs	r0, #50	; 0x32
30005612:	f043 0302 	orr.w	r3, r3, #2
30005616:	6063      	str	r3, [r4, #4]
30005618:	6863      	ldr	r3, [r4, #4]
3000561a:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000561e:	6063      	str	r3, [r4, #4]
30005620:	6823      	ldr	r3, [r4, #0]
30005622:	f043 0302 	orr.w	r3, r3, #2
30005626:	6023      	str	r3, [r4, #0]
30005628:	47a8      	blx	r5
3000562a:	6823      	ldr	r3, [r4, #0]
3000562c:	2032      	movs	r0, #50	; 0x32
3000562e:	f043 0304 	orr.w	r3, r3, #4
30005632:	6023      	str	r3, [r4, #0]
30005634:	6823      	ldr	r3, [r4, #0]
30005636:	f043 0301 	orr.w	r3, r3, #1
3000563a:	6023      	str	r3, [r4, #0]
3000563c:	6823      	ldr	r3, [r4, #0]
3000563e:	f043 0310 	orr.w	r3, r3, #16
30005642:	6023      	str	r3, [r4, #0]
30005644:	47a8      	blx	r5
30005646:	6823      	ldr	r3, [r4, #0]
30005648:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
3000564c:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30005650:	6023      	str	r3, [r4, #0]
30005652:	47a8      	blx	r5
30005654:	6863      	ldr	r3, [r4, #4]
30005656:	4a1a      	ldr	r2, [pc, #104]	; (300056c0 <BOOT_Enable_AP+0xc4>)
30005658:	f023 0310 	bic.w	r3, r3, #16
3000565c:	6063      	str	r3, [r4, #4]
3000565e:	6863      	ldr	r3, [r4, #4]
30005660:	f023 0302 	bic.w	r3, r3, #2
30005664:	6063      	str	r3, [r4, #4]
30005666:	4790      	blx	r2
30005668:	b178      	cbz	r0, 3000568a <BOOT_Enable_AP+0x8e>
3000566a:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000566e:	4815      	ldr	r0, [pc, #84]	; (300056c4 <BOOT_Enable_AP+0xc8>)
30005670:	4a15      	ldr	r2, [pc, #84]	; (300056c8 <BOOT_Enable_AP+0xcc>)
30005672:	68d9      	ldr	r1, [r3, #12]
30005674:	f041 0101 	orr.w	r1, r1, #1
30005678:	60d9      	str	r1, [r3, #12]
3000567a:	6899      	ldr	r1, [r3, #8]
3000567c:	f041 0101 	orr.w	r1, r1, #1
30005680:	6099      	str	r1, [r3, #8]
30005682:	6843      	ldr	r3, [r0, #4]
30005684:	431a      	orrs	r2, r3
30005686:	6042      	str	r2, [r0, #4]
30005688:	bd38      	pop	{r3, r4, r5, pc}
3000568a:	4604      	mov	r4, r0
3000568c:	4d0f      	ldr	r5, [pc, #60]	; (300056cc <BOOT_Enable_AP+0xd0>)
3000568e:	4601      	mov	r1, r0
30005690:	2041      	movs	r0, #65	; 0x41
30005692:	47a8      	blx	r5
30005694:	4621      	mov	r1, r4
30005696:	2042      	movs	r0, #66	; 0x42
30005698:	47a8      	blx	r5
3000569a:	4621      	mov	r1, r4
3000569c:	2043      	movs	r0, #67	; 0x43
3000569e:	47a8      	blx	r5
300056a0:	4621      	mov	r1, r4
300056a2:	2044      	movs	r0, #68	; 0x44
300056a4:	47a8      	blx	r5
300056a6:	4621      	mov	r1, r4
300056a8:	2045      	movs	r0, #69	; 0x45
300056aa:	47a8      	blx	r5
300056ac:	4621      	mov	r1, r4
300056ae:	2046      	movs	r0, #70	; 0x46
300056b0:	47a8      	blx	r5
300056b2:	e7da      	b.n	3000566a <BOOT_Enable_AP+0x6e>
300056b4:	0000aab9 	.word	0x0000aab9
300056b8:	4200c000 	.word	0x4200c000
300056bc:	00009b2d 	.word	0x00009b2d
300056c0:	30008f3d 	.word	0x30008f3d
300056c4:	41000200 	.word	0x41000200
300056c8:	01001111 	.word	0x01001111
300056cc:	0000b20d 	.word	0x0000b20d

300056d0 <BOOT_RAM_KeyDeriveFunc>:
300056d0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300056d4:	ed2d 8b04 	vpush	{d8-d9}
300056d8:	b0cb      	sub	sp, #300	; 0x12c
300056da:	4c6a      	ldr	r4, [pc, #424]	; (30005884 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
300056dc:	4680      	mov	r8, r0
300056de:	4689      	mov	r9, r1
300056e0:	4869      	ldr	r0, [pc, #420]	; (30005888 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
300056e2:	ee08 2a90 	vmov	s17, r2
300056e6:	4969      	ldr	r1, [pc, #420]	; (3000588c <BOOT_RAM_KeyDeriveFunc+0x1bc>)
300056e8:	2201      	movs	r2, #1
300056ea:	ee09 3a10 	vmov	s18, r3
300056ee:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
300056f2:	47a0      	blx	r4
300056f4:	2201      	movs	r2, #1
300056f6:	4966      	ldr	r1, [pc, #408]	; (30005890 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
300056f8:	ae02      	add	r6, sp, #8
300056fa:	4864      	ldr	r0, [pc, #400]	; (3000588c <BOOT_RAM_KeyDeriveFunc+0x1bc>)
300056fc:	47a0      	blx	r4
300056fe:	4b65      	ldr	r3, [pc, #404]	; (30005894 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30005700:	2000      	movs	r0, #0
30005702:	4798      	blx	r3
30005704:	22e0      	movs	r2, #224	; 0xe0
30005706:	2100      	movs	r1, #0
30005708:	a812      	add	r0, sp, #72	; 0x48
3000570a:	f006 f805 	bl	3000b718 <____wrap_memset_veneer>
3000570e:	2300      	movs	r3, #0
30005710:	f8ad 3004 	strh.w	r3, [sp, #4]
30005714:	f88d 3006 	strb.w	r3, [sp, #6]
30005718:	2301      	movs	r3, #1
3000571a:	f88d 3007 	strb.w	r3, [sp, #7]
3000571e:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005720:	2b00      	cmp	r3, #0
30005722:	f000 80ab 	beq.w	3000587c <BOOT_RAM_KeyDeriveFunc+0x1ac>
30005726:	ab0a      	add	r3, sp, #40	; 0x28
30005728:	f8df b17c 	ldr.w	fp, [pc, #380]	; 300058a8 <BOOT_RAM_KeyDeriveFunc+0x1d8>
3000572c:	4f5a      	ldr	r7, [pc, #360]	; (30005898 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
3000572e:	ee08 3a10 	vmov	s16, r3
30005732:	ab12      	add	r3, sp, #72	; 0x48
30005734:	464a      	mov	r2, r9
30005736:	4641      	mov	r1, r8
30005738:	2020      	movs	r0, #32
3000573a:	47d8      	blx	fp
3000573c:	aa12      	add	r2, sp, #72	; 0x48
3000573e:	ee19 1a10 	vmov	r1, s18
30005742:	ee18 0a90 	vmov	r0, s17
30005746:	47b8      	blx	r7
30005748:	ee18 5a10 	vmov	r5, s16
3000574c:	aa12      	add	r2, sp, #72	; 0x48
3000574e:	2104      	movs	r1, #4
30005750:	a801      	add	r0, sp, #4
30005752:	4634      	mov	r4, r6
30005754:	47b8      	blx	r7
30005756:	a912      	add	r1, sp, #72	; 0x48
30005758:	4b50      	ldr	r3, [pc, #320]	; (3000589c <BOOT_RAM_KeyDeriveFunc+0x1cc>)
3000575a:	ee18 0a10 	vmov	r0, s16
3000575e:	4798      	blx	r3
30005760:	f1ba 0f01 	cmp.w	sl, #1
30005764:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30005766:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30005768:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
3000576c:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30005770:	d91f      	bls.n	300057b2 <BOOT_RAM_KeyDeriveFunc+0xe2>
30005772:	2501      	movs	r5, #1
30005774:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30005778:	ab12      	add	r3, sp, #72	; 0x48
3000577a:	4641      	mov	r1, r8
3000577c:	464a      	mov	r2, r9
3000577e:	2020      	movs	r0, #32
30005780:	47d8      	blx	fp
30005782:	aa12      	add	r2, sp, #72	; 0x48
30005784:	2120      	movs	r1, #32
30005786:	4630      	mov	r0, r6
30005788:	47b8      	blx	r7
3000578a:	a912      	add	r1, sp, #72	; 0x48
3000578c:	4b43      	ldr	r3, [pc, #268]	; (3000589c <BOOT_RAM_KeyDeriveFunc+0x1cc>)
3000578e:	4630      	mov	r0, r6
30005790:	4798      	blx	r3
30005792:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30005796:	f10d 0107 	add.w	r1, sp, #7
3000579a:	f813 2f01 	ldrb.w	r2, [r3, #1]!
3000579e:	f811 0f01 	ldrb.w	r0, [r1, #1]!
300057a2:	42a3      	cmp	r3, r4
300057a4:	ea82 0200 	eor.w	r2, r2, r0
300057a8:	701a      	strb	r2, [r3, #0]
300057aa:	d1f6      	bne.n	3000579a <BOOT_RAM_KeyDeriveFunc+0xca>
300057ac:	3501      	adds	r5, #1
300057ae:	45aa      	cmp	sl, r5
300057b0:	d1e2      	bne.n	30005778 <BOOT_RAM_KeyDeriveFunc+0xa8>
300057b2:	9a59      	ldr	r2, [sp, #356]	; 0x164
300057b4:	ee18 1a10 	vmov	r1, s16
300057b8:	9b59      	ldr	r3, [sp, #356]	; 0x164
300057ba:	2a20      	cmp	r2, #32
300057bc:	985a      	ldr	r0, [sp, #360]	; 0x168
300057be:	bf28      	it	cs
300057c0:	2220      	movcs	r2, #32
300057c2:	1a9b      	subs	r3, r3, r2
300057c4:	9359      	str	r3, [sp, #356]	; 0x164
300057c6:	4603      	mov	r3, r0
300057c8:	4413      	add	r3, r2
300057ca:	935a      	str	r3, [sp, #360]	; 0x168
300057cc:	f005 ff8c 	bl	3000b6e8 <____wrap_memcpy_veneer>
300057d0:	f10d 0207 	add.w	r2, sp, #7
300057d4:	2104      	movs	r1, #4
300057d6:	7813      	ldrb	r3, [r2, #0]
300057d8:	3901      	subs	r1, #1
300057da:	3301      	adds	r3, #1
300057dc:	b2db      	uxtb	r3, r3
300057de:	f802 3901 	strb.w	r3, [r2], #-1
300057e2:	b90b      	cbnz	r3, 300057e8 <BOOT_RAM_KeyDeriveFunc+0x118>
300057e4:	2900      	cmp	r1, #0
300057e6:	d1f6      	bne.n	300057d6 <BOOT_RAM_KeyDeriveFunc+0x106>
300057e8:	9b59      	ldr	r3, [sp, #356]	; 0x164
300057ea:	2b00      	cmp	r3, #0
300057ec:	d1a1      	bne.n	30005732 <BOOT_RAM_KeyDeriveFunc+0x62>
300057ee:	2220      	movs	r2, #32
300057f0:	2100      	movs	r1, #0
300057f2:	4c2b      	ldr	r4, [pc, #172]	; (300058a0 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
300057f4:	4630      	mov	r0, r6
300057f6:	47a0      	blx	r4
300057f8:	2220      	movs	r2, #32
300057fa:	2100      	movs	r1, #0
300057fc:	ee18 0a10 	vmov	r0, s16
30005800:	47a0      	blx	r4
30005802:	4b28      	ldr	r3, [pc, #160]	; (300058a4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005804:	695b      	ldr	r3, [r3, #20]
30005806:	03da      	lsls	r2, r3, #15
30005808:	d512      	bpl.n	30005830 <BOOT_RAM_KeyDeriveFunc+0x160>
3000580a:	f016 021f 	ands.w	r2, r6, #31
3000580e:	4633      	mov	r3, r6
30005810:	d130      	bne.n	30005874 <BOOT_RAM_KeyDeriveFunc+0x1a4>
30005812:	2220      	movs	r2, #32
30005814:	f3bf 8f4f 	dsb	sy
30005818:	4822      	ldr	r0, [pc, #136]	; (300058a4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
3000581a:	441a      	add	r2, r3
3000581c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005820:	3320      	adds	r3, #32
30005822:	1ad1      	subs	r1, r2, r3
30005824:	2900      	cmp	r1, #0
30005826:	dcf9      	bgt.n	3000581c <BOOT_RAM_KeyDeriveFunc+0x14c>
30005828:	f3bf 8f4f 	dsb	sy
3000582c:	f3bf 8f6f 	isb	sy
30005830:	4b1c      	ldr	r3, [pc, #112]	; (300058a4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005832:	695b      	ldr	r3, [r3, #20]
30005834:	03db      	lsls	r3, r3, #15
30005836:	d513      	bpl.n	30005860 <BOOT_RAM_KeyDeriveFunc+0x190>
30005838:	ee18 3a10 	vmov	r3, s16
3000583c:	f013 021f 	ands.w	r2, r3, #31
30005840:	d114      	bne.n	3000586c <BOOT_RAM_KeyDeriveFunc+0x19c>
30005842:	2220      	movs	r2, #32
30005844:	f3bf 8f4f 	dsb	sy
30005848:	4816      	ldr	r0, [pc, #88]	; (300058a4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
3000584a:	441a      	add	r2, r3
3000584c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005850:	3320      	adds	r3, #32
30005852:	1ad1      	subs	r1, r2, r3
30005854:	2900      	cmp	r1, #0
30005856:	dcf9      	bgt.n	3000584c <BOOT_RAM_KeyDeriveFunc+0x17c>
30005858:	f3bf 8f4f 	dsb	sy
3000585c:	f3bf 8f6f 	isb	sy
30005860:	2000      	movs	r0, #0
30005862:	b04b      	add	sp, #300	; 0x12c
30005864:	ecbd 8b04 	vpop	{d8-d9}
30005868:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000586c:	f023 031f 	bic.w	r3, r3, #31
30005870:	3220      	adds	r2, #32
30005872:	e7e7      	b.n	30005844 <BOOT_RAM_KeyDeriveFunc+0x174>
30005874:	f026 031f 	bic.w	r3, r6, #31
30005878:	3220      	adds	r2, #32
3000587a:	e7cb      	b.n	30005814 <BOOT_RAM_KeyDeriveFunc+0x144>
3000587c:	ab0a      	add	r3, sp, #40	; 0x28
3000587e:	ee08 3a10 	vmov	s16, r3
30005882:	e7b4      	b.n	300057ee <BOOT_RAM_KeyDeriveFunc+0x11e>
30005884:	0000b479 	.word	0x0000b479
30005888:	40000008 	.word	0x40000008
3000588c:	40000004 	.word	0x40000004
30005890:	40000002 	.word	0x40000002
30005894:	300091d9 	.word	0x300091d9
30005898:	30008fdd 	.word	0x30008fdd
3000589c:	30009019 	.word	0x30009019
300058a0:	00012be5 	.word	0x00012be5
300058a4:	e000ed00 	.word	0xe000ed00
300058a8:	300088dd 	.word	0x300088dd

300058ac <BOOT_Share_Memory_Patch>:
300058ac:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300058b0:	4817      	ldr	r0, [pc, #92]	; (30005910 <BOOT_Share_Memory_Patch+0x64>)
300058b2:	4918      	ldr	r1, [pc, #96]	; (30005914 <BOOT_Share_Memory_Patch+0x68>)
300058b4:	b430      	push	{r4, r5}
300058b6:	6b13      	ldr	r3, [r2, #48]	; 0x30
300058b8:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
300058bc:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
300058c0:	6313      	str	r3, [r2, #48]	; 0x30
300058c2:	6823      	ldr	r3, [r4, #0]
300058c4:	6804      	ldr	r4, [r0, #0]
300058c6:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
300058ca:	6809      	ldr	r1, [r1, #0]
300058cc:	4423      	add	r3, r4
300058ce:	6805      	ldr	r5, [r0, #0]
300058d0:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
300058d4:	440b      	add	r3, r1
300058d6:	4910      	ldr	r1, [pc, #64]	; (30005918 <BOOT_Share_Memory_Patch+0x6c>)
300058d8:	6808      	ldr	r0, [r1, #0]
300058da:	442b      	add	r3, r5
300058dc:	6b11      	ldr	r1, [r2, #48]	; 0x30
300058de:	4403      	add	r3, r0
300058e0:	480e      	ldr	r0, [pc, #56]	; (3000591c <BOOT_Share_Memory_Patch+0x70>)
300058e2:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
300058e6:	6311      	str	r1, [r2, #48]	; 0x30
300058e8:	6821      	ldr	r1, [r4, #0]
300058ea:	4a0d      	ldr	r2, [pc, #52]	; (30005920 <BOOT_Share_Memory_Patch+0x74>)
300058ec:	6804      	ldr	r4, [r0, #0]
300058ee:	440b      	add	r3, r1
300058f0:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
300058f4:	6811      	ldr	r1, [r2, #0]
300058f6:	4423      	add	r3, r4
300058f8:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
300058fc:	6800      	ldr	r0, [r0, #0]
300058fe:	440b      	add	r3, r1
30005900:	6812      	ldr	r2, [r2, #0]
30005902:	4908      	ldr	r1, [pc, #32]	; (30005924 <BOOT_Share_Memory_Patch+0x78>)
30005904:	4403      	add	r3, r0
30005906:	6808      	ldr	r0, [r1, #0]
30005908:	4413      	add	r3, r2
3000590a:	bc30      	pop	{r4, r5}
3000590c:	4418      	add	r0, r3
3000590e:	4770      	bx	lr
30005910:	2200c000 	.word	0x2200c000
30005914:	22018000 	.word	0x22018000
30005918:	22100000 	.word	0x22100000
3000591c:	20010000 	.word	0x20010000
30005920:	20020000 	.word	0x20020000
30005924:	23010000 	.word	0x23010000

30005928 <BOOT_Image1>:
30005928:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000592c:	4bac      	ldr	r3, [pc, #688]	; (30005be0 <BOOT_Image1+0x2b8>)
3000592e:	b082      	sub	sp, #8
30005930:	4798      	blx	r3
30005932:	2800      	cmp	r0, #0
30005934:	f040 80c1 	bne.w	30005aba <BOOT_Image1+0x192>
30005938:	48aa      	ldr	r0, [pc, #680]	; (30005be4 <BOOT_Image1+0x2bc>)
3000593a:	f005 ff15 	bl	3000b768 <__DiagPrintf_veneer>
3000593e:	49aa      	ldr	r1, [pc, #680]	; (30005be8 <BOOT_Image1+0x2c0>)
30005940:	48aa      	ldr	r0, [pc, #680]	; (30005bec <BOOT_Image1+0x2c4>)
30005942:	f005 ff11 	bl	3000b768 <__DiagPrintf_veneer>
30005946:	48aa      	ldr	r0, [pc, #680]	; (30005bf0 <BOOT_Image1+0x2c8>)
30005948:	4aaa      	ldr	r2, [pc, #680]	; (30005bf4 <BOOT_Image1+0x2cc>)
3000594a:	2100      	movs	r1, #0
3000594c:	4baa      	ldr	r3, [pc, #680]	; (30005bf8 <BOOT_Image1+0x2d0>)
3000594e:	1a12      	subs	r2, r2, r0
30005950:	4798      	blx	r3
30005952:	4aaa      	ldr	r2, [pc, #680]	; (30005bfc <BOOT_Image1+0x2d4>)
30005954:	49aa      	ldr	r1, [pc, #680]	; (30005c00 <BOOT_Image1+0x2d8>)
30005956:	8c13      	ldrh	r3, [r2, #32]
30005958:	b29b      	uxth	r3, r3
3000595a:	8413      	strh	r3, [r2, #32]
3000595c:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005960:	f002 fede 	bl	30008720 <BOOT_Reason>
30005964:	2800      	cmp	r0, #0
30005966:	f000 80e9 	beq.w	30005b3c <BOOT_Image1+0x214>
3000596a:	4ea6      	ldr	r6, [pc, #664]	; (30005c04 <BOOT_Image1+0x2dc>)
3000596c:	4ba6      	ldr	r3, [pc, #664]	; (30005c08 <BOOT_Image1+0x2e0>)
3000596e:	4798      	blx	r3
30005970:	7834      	ldrb	r4, [r6, #0]
30005972:	2c00      	cmp	r4, #0
30005974:	f000 8097 	beq.w	30005aa6 <BOOT_Image1+0x17e>
30005978:	2000      	movs	r0, #0
3000597a:	4ba4      	ldr	r3, [pc, #656]	; (30005c0c <BOOT_Image1+0x2e4>)
3000597c:	4798      	blx	r3
3000597e:	4ba4      	ldr	r3, [pc, #656]	; (30005c10 <BOOT_Image1+0x2e8>)
30005980:	2001      	movs	r0, #1
30005982:	4798      	blx	r3
30005984:	4c9e      	ldr	r4, [pc, #632]	; (30005c00 <BOOT_Image1+0x2d8>)
30005986:	f7ff fd4b 	bl	30005420 <BOOT_SOC_ClkSet>
3000598a:	f241 070a 	movw	r7, #4106	; 0x100a
3000598e:	2028      	movs	r0, #40	; 0x28
30005990:	8823      	ldrh	r3, [r4, #0]
30005992:	4da0      	ldr	r5, [pc, #640]	; (30005c14 <BOOT_Image1+0x2ec>)
30005994:	b29b      	uxth	r3, r3
30005996:	431f      	orrs	r7, r3
30005998:	4b9f      	ldr	r3, [pc, #636]	; (30005c18 <BOOT_Image1+0x2f0>)
3000599a:	8027      	strh	r7, [r4, #0]
3000599c:	4798      	blx	r3
3000599e:	88a3      	ldrh	r3, [r4, #4]
300059a0:	b29b      	uxth	r3, r3
300059a2:	ea23 0307 	bic.w	r3, r3, r7
300059a6:	80a3      	strh	r3, [r4, #4]
300059a8:	8863      	ldrh	r3, [r4, #2]
300059aa:	8067      	strh	r7, [r4, #2]
300059ac:	47a8      	blx	r5
300059ae:	2802      	cmp	r0, #2
300059b0:	f000 808b 	beq.w	30005aca <BOOT_Image1+0x1a2>
300059b4:	4b99      	ldr	r3, [pc, #612]	; (30005c1c <BOOT_Image1+0x2f4>)
300059b6:	2401      	movs	r4, #1
300059b8:	4999      	ldr	r1, [pc, #612]	; (30005c20 <BOOT_Image1+0x2f8>)
300059ba:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300059be:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
300059c2:	4622      	mov	r2, r4
300059c4:	4f97      	ldr	r7, [pc, #604]	; (30005c24 <BOOT_Image1+0x2fc>)
300059c6:	47b8      	blx	r7
300059c8:	4622      	mov	r2, r4
300059ca:	4997      	ldr	r1, [pc, #604]	; (30005c28 <BOOT_Image1+0x300>)
300059cc:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300059d0:	47b8      	blx	r7
300059d2:	4a8a      	ldr	r2, [pc, #552]	; (30005bfc <BOOT_Image1+0x2d4>)
300059d4:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
300059d8:	f043 0303 	orr.w	r3, r3, #3
300059dc:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
300059e0:	4b92      	ldr	r3, [pc, #584]	; (30005c2c <BOOT_Image1+0x304>)
300059e2:	4798      	blx	r3
300059e4:	4a85      	ldr	r2, [pc, #532]	; (30005bfc <BOOT_Image1+0x2d4>)
300059e6:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
300059ea:	f023 0303 	bic.w	r3, r3, #3
300059ee:	b108      	cbz	r0, 300059f4 <BOOT_Image1+0xcc>
300059f0:	f043 0302 	orr.w	r3, r3, #2
300059f4:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
300059f8:	4b8d      	ldr	r3, [pc, #564]	; (30005c30 <BOOT_Image1+0x308>)
300059fa:	781b      	ldrb	r3, [r3, #0]
300059fc:	2b00      	cmp	r3, #0
300059fe:	d17f      	bne.n	30005b00 <BOOT_Image1+0x1d8>
30005a00:	4c8c      	ldr	r4, [pc, #560]	; (30005c34 <BOOT_Image1+0x30c>)
30005a02:	2102      	movs	r1, #2
30005a04:	2201      	movs	r2, #1
30005a06:	488c      	ldr	r0, [pc, #560]	; (30005c38 <BOOT_Image1+0x310>)
30005a08:	47a0      	blx	r4
30005a0a:	4c8c      	ldr	r4, [pc, #560]	; (30005c3c <BOOT_Image1+0x314>)
30005a0c:	68a3      	ldr	r3, [r4, #8]
30005a0e:	0799      	lsls	r1, r3, #30
30005a10:	d470      	bmi.n	30005af4 <BOOT_Image1+0x1cc>
30005a12:	498b      	ldr	r1, [pc, #556]	; (30005c40 <BOOT_Image1+0x318>)
30005a14:	e841 f100 	tt	r1, r1
30005a18:	68a3      	ldr	r3, [r4, #8]
30005a1a:	f3c1 5180 	ubfx	r1, r1, #22, #1
30005a1e:	079a      	lsls	r2, r3, #30
30005a20:	f100 8088 	bmi.w	30005b34 <BOOT_Image1+0x20c>
30005a24:	4b87      	ldr	r3, [pc, #540]	; (30005c44 <BOOT_Image1+0x31c>)
30005a26:	4f88      	ldr	r7, [pc, #544]	; (30005c48 <BOOT_Image1+0x320>)
30005a28:	681b      	ldr	r3, [r3, #0]
30005a2a:	4798      	blx	r3
30005a2c:	f7fe fafa 	bl	30004024 <BOOT_RccConfig>
30005a30:	f7fe fb64 	bl	300040fc <BOOT_CACHEWRR_Set>
30005a34:	4b85      	ldr	r3, [pc, #532]	; (30005c4c <BOOT_Image1+0x324>)
30005a36:	6818      	ldr	r0, [r3, #0]
30005a38:	f7fe fb8e 	bl	30004158 <BOOT_TCMSet>
30005a3c:	4b84      	ldr	r3, [pc, #528]	; (30005c50 <BOOT_Image1+0x328>)
30005a3e:	695b      	ldr	r3, [r3, #20]
30005a40:	03db      	lsls	r3, r3, #15
30005a42:	d512      	bpl.n	30005a6a <BOOT_Image1+0x142>
30005a44:	f017 011f 	ands.w	r1, r7, #31
30005a48:	d13b      	bne.n	30005ac2 <BOOT_Image1+0x19a>
30005a4a:	463b      	mov	r3, r7
30005a4c:	2174      	movs	r1, #116	; 0x74
30005a4e:	f3bf 8f4f 	dsb	sy
30005a52:	487f      	ldr	r0, [pc, #508]	; (30005c50 <BOOT_Image1+0x328>)
30005a54:	4419      	add	r1, r3
30005a56:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30005a5a:	3320      	adds	r3, #32
30005a5c:	1aca      	subs	r2, r1, r3
30005a5e:	2a00      	cmp	r2, #0
30005a60:	dcf9      	bgt.n	30005a56 <BOOT_Image1+0x12e>
30005a62:	f3bf 8f4f 	dsb	sy
30005a66:	f3bf 8f6f 	isb	sy
30005a6a:	4b6c      	ldr	r3, [pc, #432]	; (30005c1c <BOOT_Image1+0x2f4>)
30005a6c:	2200      	movs	r2, #0
30005a6e:	f8df 818c 	ldr.w	r8, [pc, #396]	; 30005bfc <BOOT_Image1+0x2d4>
30005a72:	f8c8 7270 	str.w	r7, [r8, #624]	; 0x270
30005a76:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30005a7a:	f7fd fe21 	bl	300036c0 <flash_highspeed_setup>
30005a7e:	47a8      	blx	r5
30005a80:	2802      	cmp	r0, #2
30005a82:	f000 8170 	beq.w	30005d66 <BOOT_Image1+0x43e>
30005a86:	7833      	ldrb	r3, [r6, #0]
30005a88:	2b00      	cmp	r3, #0
30005a8a:	d05e      	beq.n	30005b4a <BOOT_Image1+0x222>
30005a8c:	4c71      	ldr	r4, [pc, #452]	; (30005c54 <BOOT_Image1+0x32c>)
30005a8e:	4e72      	ldr	r6, [pc, #456]	; (30005c58 <BOOT_Image1+0x330>)
30005a90:	4d72      	ldr	r5, [pc, #456]	; (30005c5c <BOOT_Image1+0x334>)
30005a92:	4633      	mov	r3, r6
30005a94:	462a      	mov	r2, r5
30005a96:	4970      	ldr	r1, [pc, #448]	; (30005c58 <BOOT_Image1+0x330>)
30005a98:	2002      	movs	r0, #2
30005a9a:	f005 fbc7 	bl	3000b22c <rtk_log_write>
30005a9e:	f241 3088 	movw	r0, #5000	; 0x1388
30005aa2:	47a0      	blx	r4
30005aa4:	e7f5      	b.n	30005a92 <BOOT_Image1+0x16a>
30005aa6:	4b5a      	ldr	r3, [pc, #360]	; (30005c10 <BOOT_Image1+0x2e8>)
30005aa8:	2001      	movs	r0, #1
30005aaa:	4798      	blx	r3
30005aac:	4620      	mov	r0, r4
30005aae:	4b6c      	ldr	r3, [pc, #432]	; (30005c60 <BOOT_Image1+0x338>)
30005ab0:	4798      	blx	r3
30005ab2:	4b6c      	ldr	r3, [pc, #432]	; (30005c64 <BOOT_Image1+0x33c>)
30005ab4:	2001      	movs	r0, #1
30005ab6:	4798      	blx	r3
30005ab8:	e764      	b.n	30005984 <BOOT_Image1+0x5c>
30005aba:	486b      	ldr	r0, [pc, #428]	; (30005c68 <BOOT_Image1+0x340>)
30005abc:	f005 fe54 	bl	3000b768 <__DiagPrintf_veneer>
30005ac0:	e73d      	b.n	3000593e <BOOT_Image1+0x16>
30005ac2:	3174      	adds	r1, #116	; 0x74
30005ac4:	f027 031f 	bic.w	r3, r7, #31
30005ac8:	e7c1      	b.n	30005a4e <BOOT_Image1+0x126>
30005aca:	6de3      	ldr	r3, [r4, #92]	; 0x5c
30005acc:	2201      	movs	r2, #1
30005ace:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005ad2:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30005ad6:	65e3      	str	r3, [r4, #92]	; 0x5c
30005ad8:	4b50      	ldr	r3, [pc, #320]	; (30005c1c <BOOT_Image1+0x2f4>)
30005ada:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30005ade:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005ae2:	4b50      	ldr	r3, [pc, #320]	; (30005c24 <BOOT_Image1+0x2fc>)
30005ae4:	4798      	blx	r3
30005ae6:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
30005aea:	f043 0302 	orr.w	r3, r3, #2
30005aee:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
30005af2:	e775      	b.n	300059e0 <BOOT_Image1+0xb8>
30005af4:	f3ef 8108 	mrs	r1, MSP
30005af8:	485c      	ldr	r0, [pc, #368]	; (30005c6c <BOOT_Image1+0x344>)
30005afa:	f005 fe35 	bl	3000b768 <__DiagPrintf_veneer>
30005afe:	e788      	b.n	30005a12 <BOOT_Image1+0xea>
30005b00:	4f5b      	ldr	r7, [pc, #364]	; (30005c70 <BOOT_Image1+0x348>)
30005b02:	47b8      	blx	r7
30005b04:	4604      	mov	r4, r0
30005b06:	47b8      	blx	r7
30005b08:	2c02      	cmp	r4, #2
30005b0a:	f43f af79 	beq.w	30005a00 <BOOT_Image1+0xd8>
30005b0e:	2803      	cmp	r0, #3
30005b10:	f43f af76 	beq.w	30005a00 <BOOT_Image1+0xd8>
30005b14:	4b57      	ldr	r3, [pc, #348]	; (30005c74 <BOOT_Image1+0x34c>)
30005b16:	4798      	blx	r3
30005b18:	4c46      	ldr	r4, [pc, #280]	; (30005c34 <BOOT_Image1+0x30c>)
30005b1a:	2200      	movs	r2, #0
30005b1c:	2101      	movs	r1, #1
30005b1e:	4846      	ldr	r0, [pc, #280]	; (30005c38 <BOOT_Image1+0x310>)
30005b20:	47a0      	blx	r4
30005b22:	2101      	movs	r1, #1
30005b24:	4844      	ldr	r0, [pc, #272]	; (30005c38 <BOOT_Image1+0x310>)
30005b26:	4b54      	ldr	r3, [pc, #336]	; (30005c78 <BOOT_Image1+0x350>)
30005b28:	4798      	blx	r3
30005b2a:	2201      	movs	r2, #1
30005b2c:	4842      	ldr	r0, [pc, #264]	; (30005c38 <BOOT_Image1+0x310>)
30005b2e:	4611      	mov	r1, r2
30005b30:	47a0      	blx	r4
30005b32:	e766      	b.n	30005a02 <BOOT_Image1+0xda>
30005b34:	4851      	ldr	r0, [pc, #324]	; (30005c7c <BOOT_Image1+0x354>)
30005b36:	f005 fe17 	bl	3000b768 <__DiagPrintf_veneer>
30005b3a:	e773      	b.n	30005a24 <BOOT_Image1+0xfc>
30005b3c:	4601      	mov	r1, r0
30005b3e:	f44f 7201 	mov.w	r2, #516	; 0x204
30005b42:	4836      	ldr	r0, [pc, #216]	; (30005c1c <BOOT_Image1+0x2f4>)
30005b44:	f005 fde8 	bl	3000b718 <____wrap_memset_veneer>
30005b48:	e70f      	b.n	3000596a <BOOT_Image1+0x42>
30005b4a:	4b43      	ldr	r3, [pc, #268]	; (30005c58 <BOOT_Image1+0x330>)
30005b4c:	2004      	movs	r0, #4
30005b4e:	4a4c      	ldr	r2, [pc, #304]	; (30005c80 <BOOT_Image1+0x358>)
30005b50:	4619      	mov	r1, r3
30005b52:	f005 fb6b 	bl	3000b22c <rtk_log_write>
30005b56:	f7fe fc97 	bl	30004488 <BOOT_DDR_Init>
30005b5a:	f8d8 3290 	ldr.w	r3, [r8, #656]	; 0x290
30005b5e:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005b62:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005b66:	f8c8 3290 	str.w	r3, [r8, #656]	; 0x290
30005b6a:	2200      	movs	r2, #0
30005b6c:	4b23      	ldr	r3, [pc, #140]	; (30005bfc <BOOT_Image1+0x2d4>)
30005b6e:	4d45      	ldr	r5, [pc, #276]	; (30005c84 <BOOT_Image1+0x35c>)
30005b70:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
30005b74:	f8c3 22d4 	str.w	r2, [r3, #724]	; 0x2d4
30005b78:	f8c3 22d8 	str.w	r2, [r3, #728]	; 0x2d8
30005b7c:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
30005b80:	f7ff fe94 	bl	300058ac <BOOT_Share_Memory_Patch>
30005b84:	4b40      	ldr	r3, [pc, #256]	; (30005c88 <BOOT_Image1+0x360>)
30005b86:	2002      	movs	r0, #2
30005b88:	4798      	blx	r3
30005b8a:	4b33      	ldr	r3, [pc, #204]	; (30005c58 <BOOT_Image1+0x330>)
30005b8c:	9000      	str	r0, [sp, #0]
30005b8e:	2004      	movs	r0, #4
30005b90:	4619      	mov	r1, r3
30005b92:	4a3e      	ldr	r2, [pc, #248]	; (30005c8c <BOOT_Image1+0x364>)
30005b94:	f005 fb4a 	bl	3000b22c <rtk_log_write>
30005b98:	782b      	ldrb	r3, [r5, #0]
30005b9a:	2b00      	cmp	r3, #0
30005b9c:	f040 8107 	bne.w	30005dae <BOOT_Image1+0x486>
30005ba0:	6823      	ldr	r3, [r4, #0]
30005ba2:	4e3b      	ldr	r6, [pc, #236]	; (30005c90 <BOOT_Image1+0x368>)
30005ba4:	f043 0302 	orr.w	r3, r3, #2
30005ba8:	6023      	str	r3, [r4, #0]
30005baa:	47b0      	blx	r6
30005bac:	b190      	cbz	r0, 30005bd4 <BOOT_Image1+0x2ac>
30005bae:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30005bb2:	4838      	ldr	r0, [pc, #224]	; (30005c94 <BOOT_Image1+0x36c>)
30005bb4:	f005 fdd8 	bl	3000b768 <__DiagPrintf_veneer>
30005bb8:	4837      	ldr	r0, [pc, #220]	; (30005c98 <BOOT_Image1+0x370>)
30005bba:	f005 fdd5 	bl	3000b768 <__DiagPrintf_veneer>
30005bbe:	f005 fb71 	bl	3000b2a4 <BOOT_VerCheck>
30005bc2:	f7fe fe55 	bl	30004870 <BOOT_LoadImages>
30005bc6:	2800      	cmp	r0, #0
30005bc8:	d16a      	bne.n	30005ca0 <BOOT_Image1+0x378>
30005bca:	4c22      	ldr	r4, [pc, #136]	; (30005c54 <BOOT_Image1+0x32c>)
30005bcc:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005bd0:	47a0      	blx	r4
30005bd2:	e7fb      	b.n	30005bcc <BOOT_Image1+0x2a4>
30005bd4:	4931      	ldr	r1, [pc, #196]	; (30005c9c <BOOT_Image1+0x374>)
30005bd6:	482f      	ldr	r0, [pc, #188]	; (30005c94 <BOOT_Image1+0x36c>)
30005bd8:	f005 fdc6 	bl	3000b768 <__DiagPrintf_veneer>
30005bdc:	e7ec      	b.n	30005bb8 <BOOT_Image1+0x290>
30005bde:	bf00      	nop
30005be0:	0000c149 	.word	0x0000c149
30005be4:	3000bff8 	.word	0x3000bff8
30005be8:	3000d654 	.word	0x3000d654
30005bec:	3000c01c 	.word	0x3000c01c
30005bf0:	3000ed30 	.word	0x3000ed30
30005bf4:	3000f1d4 	.word	0x3000f1d4
30005bf8:	00012be5 	.word	0x00012be5
30005bfc:	42008000 	.word	0x42008000
30005c00:	42008200 	.word	0x42008200
30005c04:	3000d93e 	.word	0x3000d93e
30005c08:	3000a685 	.word	0x3000a685
30005c0c:	3000aa3d 	.word	0x3000aa3d
30005c10:	3000a95d 	.word	0x3000a95d
30005c14:	300070e5 	.word	0x300070e5
30005c18:	00009b2d 	.word	0x00009b2d
30005c1c:	23020000 	.word	0x23020000
30005c20:	40080000 	.word	0x40080000
30005c24:	0000b479 	.word	0x0000b479
30005c28:	40040000 	.word	0x40040000
30005c2c:	30008f3d 	.word	0x30008f3d
30005c30:	3000d93d 	.word	0x3000d93d
30005c34:	0000aab9 	.word	0x0000aab9
30005c38:	4200c000 	.word	0x4200c000
30005c3c:	2001c00c 	.word	0x2001c00c
30005c40:	30005929 	.word	0x30005929
30005c44:	00033000 	.word	0x00033000
30005c48:	2001c01c 	.word	0x2001c01c
30005c4c:	3000d940 	.word	0x3000d940
30005c50:	e000ed00 	.word	0xe000ed00
30005c54:	00009be5 	.word	0x00009be5
30005c58:	3000b9d0 	.word	0x3000b9d0
30005c5c:	3000c0c0 	.word	0x3000c0c0
30005c60:	3000ab8d 	.word	0x3000ab8d
30005c64:	3000aafd 	.word	0x3000aafd
30005c68:	3000c008 	.word	0x3000c008
30005c6c:	3000c034 	.word	0x3000c034
30005c70:	0000c0f9 	.word	0x0000c0f9
30005c74:	0000aea1 	.word	0x0000aea1
30005c78:	0000aaa5 	.word	0x0000aaa5
30005c7c:	3000c064 	.word	0x3000c064
30005c80:	3000c100 	.word	0x3000c100
30005c84:	3000d93c 	.word	0x3000d93c
30005c88:	00009979 	.word	0x00009979
30005c8c:	3000c114 	.word	0x3000c114
30005c90:	0000c169 	.word	0x0000c169
30005c94:	3000c130 	.word	0x3000c130
30005c98:	3000c148 	.word	0x3000c148
30005c9c:	0016e360 	.word	0x0016e360
30005ca0:	f7ff fb7c 	bl	3000539c <BOOT_Enable_KM0>
30005ca4:	2200      	movs	r2, #0
30005ca6:	2101      	movs	r1, #1
30005ca8:	484e      	ldr	r0, [pc, #312]	; (30005de4 <BOOT_Image1+0x4bc>)
30005caa:	4b4f      	ldr	r3, [pc, #316]	; (30005de8 <BOOT_Image1+0x4c0>)
30005cac:	4798      	blx	r3
30005cae:	4b4f      	ldr	r3, [pc, #316]	; (30005dec <BOOT_Image1+0x4c4>)
30005cb0:	2008      	movs	r0, #8
30005cb2:	4798      	blx	r3
30005cb4:	f001 f906 	bl	30006ec4 <BOOT_RAM_TZCfg>
30005cb8:	782b      	ldrb	r3, [r5, #0]
30005cba:	2b01      	cmp	r3, #1
30005cbc:	f000 8081 	beq.w	30005dc2 <BOOT_Image1+0x49a>
30005cc0:	4b4b      	ldr	r3, [pc, #300]	; (30005df0 <BOOT_Image1+0x4c8>)
30005cc2:	781b      	ldrb	r3, [r3, #0]
30005cc4:	2b00      	cmp	r3, #0
30005cc6:	d168      	bne.n	30005d9a <BOOT_Image1+0x472>
30005cc8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005ccc:	4949      	ldr	r1, [pc, #292]	; (30005df4 <BOOT_Image1+0x4cc>)
30005cce:	685a      	ldr	r2, [r3, #4]
30005cd0:	f042 0202 	orr.w	r2, r2, #2
30005cd4:	605a      	str	r2, [r3, #4]
30005cd6:	685a      	ldr	r2, [r3, #4]
30005cd8:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30005cdc:	605a      	str	r2, [r3, #4]
30005cde:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30005ce0:	f043 0303 	orr.w	r3, r3, #3
30005ce4:	678b      	str	r3, [r1, #120]	; 0x78
30005ce6:	782b      	ldrb	r3, [r5, #0]
30005ce8:	2b01      	cmp	r3, #1
30005cea:	d072      	beq.n	30005dd2 <BOOT_Image1+0x4aa>
30005cec:	4842      	ldr	r0, [pc, #264]	; (30005df8 <BOOT_Image1+0x4d0>)
30005cee:	4943      	ldr	r1, [pc, #268]	; (30005dfc <BOOT_Image1+0x4d4>)
30005cf0:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
30005cf4:	4a42      	ldr	r2, [pc, #264]	; (30005e00 <BOOT_Image1+0x4d8>)
30005cf6:	f043 0301 	orr.w	r3, r3, #1
30005cfa:	4d42      	ldr	r5, [pc, #264]	; (30005e04 <BOOT_Image1+0x4dc>)
30005cfc:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
30005d00:	688c      	ldr	r4, [r1, #8]
30005d02:	680b      	ldr	r3, [r1, #0]
30005d04:	4940      	ldr	r1, [pc, #256]	; (30005e08 <BOOT_Image1+0x4e0>)
30005d06:	6063      	str	r3, [r4, #4]
30005d08:	4b40      	ldr	r3, [pc, #256]	; (30005e0c <BOOT_Image1+0x4e4>)
30005d0a:	6094      	str	r4, [r2, #8]
30005d0c:	61e9      	str	r1, [r5, #28]
30005d0e:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005d10:	483f      	ldr	r0, [pc, #252]	; (30005e10 <BOOT_Image1+0x4e8>)
30005d12:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005d16:	6259      	str	r1, [r3, #36]	; 0x24
30005d18:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005d1a:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005d1e:	6251      	str	r1, [r2, #36]	; 0x24
30005d20:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005d24:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005d28:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005d2c:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005d30:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005d34:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005d38:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005d3c:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005d40:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005d44:	689b      	ldr	r3, [r3, #8]
30005d46:	6183      	str	r3, [r0, #24]
30005d48:	f7fe fd1a 	bl	30004780 <BOOT_NVICBackup_HP>
30005d4c:	4b31      	ldr	r3, [pc, #196]	; (30005e14 <BOOT_Image1+0x4ec>)
30005d4e:	f383 8888 	msr	MSP_NS, r3
30005d52:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
30005d56:	f383 8809 	msr	PSP, r3
30005d5a:	6860      	ldr	r0, [r4, #4]
30005d5c:	b002      	add	sp, #8
30005d5e:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30005d62:	f7fe b90f 	b.w	30003f84 <BOOT_NsStart>
30005d66:	4b2c      	ldr	r3, [pc, #176]	; (30005e18 <BOOT_Image1+0x4f0>)
30005d68:	2004      	movs	r0, #4
30005d6a:	4a2c      	ldr	r2, [pc, #176]	; (30005e1c <BOOT_Image1+0x4f4>)
30005d6c:	4619      	mov	r1, r3
30005d6e:	f005 fa5d 	bl	3000b22c <rtk_log_write>
30005d72:	4b2b      	ldr	r3, [pc, #172]	; (30005e20 <BOOT_Image1+0x4f8>)
30005d74:	4798      	blx	r3
30005d76:	f7fe fbc5 	bl	30004504 <BOOT_PSRAM_Init>
30005d7a:	7830      	ldrb	r0, [r6, #0]
30005d7c:	2800      	cmp	r0, #0
30005d7e:	f47f aef4 	bne.w	30005b6a <BOOT_Image1+0x242>
30005d82:	4b28      	ldr	r3, [pc, #160]	; (30005e24 <BOOT_Image1+0x4fc>)
30005d84:	4798      	blx	r3
30005d86:	2800      	cmp	r0, #0
30005d88:	f43f aeef 	beq.w	30005b6a <BOOT_Image1+0x242>
30005d8c:	4b22      	ldr	r3, [pc, #136]	; (30005e18 <BOOT_Image1+0x4f0>)
30005d8e:	2003      	movs	r0, #3
30005d90:	4a25      	ldr	r2, [pc, #148]	; (30005e28 <BOOT_Image1+0x500>)
30005d92:	4619      	mov	r1, r3
30005d94:	f005 fa4a 	bl	3000b22c <rtk_log_write>
30005d98:	e6e7      	b.n	30005b6a <BOOT_Image1+0x242>
30005d9a:	f7ff fc2f 	bl	300055fc <BOOT_Enable_AP>
30005d9e:	4a16      	ldr	r2, [pc, #88]	; (30005df8 <BOOT_Image1+0x4d0>)
30005da0:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
30005da4:	f043 0306 	orr.w	r3, r3, #6
30005da8:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
30005dac:	e79b      	b.n	30005ce6 <BOOT_Image1+0x3be>
30005dae:	4b1f      	ldr	r3, [pc, #124]	; (30005e2c <BOOT_Image1+0x504>)
30005db0:	4798      	blx	r3
30005db2:	2800      	cmp	r0, #0
30005db4:	f47f aef4 	bne.w	30005ba0 <BOOT_Image1+0x278>
30005db8:	68a3      	ldr	r3, [r4, #8]
30005dba:	f043 0302 	orr.w	r3, r3, #2
30005dbe:	60a3      	str	r3, [r4, #8]
30005dc0:	e6ee      	b.n	30005ba0 <BOOT_Image1+0x278>
30005dc2:	47b0      	blx	r6
30005dc4:	2801      	cmp	r0, #1
30005dc6:	4b1a      	ldr	r3, [pc, #104]	; (30005e30 <BOOT_Image1+0x508>)
30005dc8:	bf0c      	ite	eq
30005dca:	2019      	moveq	r0, #25
30005dcc:	2004      	movne	r0, #4
30005dce:	4798      	blx	r3
30005dd0:	e776      	b.n	30005cc0 <BOOT_Image1+0x398>
30005dd2:	47b0      	blx	r6
30005dd4:	2801      	cmp	r0, #1
30005dd6:	4b16      	ldr	r3, [pc, #88]	; (30005e30 <BOOT_Image1+0x508>)
30005dd8:	bf0c      	ite	eq
30005dda:	20e6      	moveq	r0, #230	; 0xe6
30005ddc:	2019      	movne	r0, #25
30005dde:	4798      	blx	r3
30005de0:	e784      	b.n	30005cec <BOOT_Image1+0x3c4>
30005de2:	bf00      	nop
30005de4:	4200c000 	.word	0x4200c000
30005de8:	0000ac6d 	.word	0x0000ac6d
30005dec:	0000e1d9 	.word	0x0000e1d9
30005df0:	3000d93f 	.word	0x3000d93f
30005df4:	41000200 	.word	0x41000200
30005df8:	42008200 	.word	0x42008200
30005dfc:	60000020 	.word	0x60000020
30005e00:	e002ed00 	.word	0xe002ed00
30005e04:	30000000 	.word	0x30000000
30005e08:	30003a81 	.word	0x30003a81
30005e0c:	e000ed00 	.word	0xe000ed00
30005e10:	3000eddc 	.word	0x3000eddc
30005e14:	2001bffc 	.word	0x2001bffc
30005e18:	3000b9d0 	.word	0x3000b9d0
30005e1c:	3000c094 	.word	0x3000c094
30005e20:	30007f65 	.word	0x30007f65
30005e24:	3000aa5d 	.word	0x3000aa5d
30005e28:	3000c0a8 	.word	0x3000c0a8
30005e2c:	0000c179 	.word	0x0000c179
30005e30:	00009be5 	.word	0x00009be5

30005e34 <BOOT_LoadSubImage>:
30005e34:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005e38:	b08f      	sub	sp, #60	; 0x3c
30005e3a:	469a      	mov	sl, r3
30005e3c:	460f      	mov	r7, r1
30005e3e:	f89d 3060 	ldrb.w	r3, [sp, #96]	; 0x60
30005e42:	9305      	str	r3, [sp, #20]
30005e44:	b39a      	cbz	r2, 30005eae <BOOT_LoadSubImage+0x7a>
30005e46:	4691      	mov	r9, r2
30005e48:	2600      	movs	r6, #0
30005e4a:	4605      	mov	r5, r0
30005e4c:	2220      	movs	r2, #32
30005e4e:	4639      	mov	r1, r7
30005e50:	a806      	add	r0, sp, #24
30005e52:	f7fe fcf3 	bl	3000483c <BOOT_ImgCopy>
30005e56:	9b06      	ldr	r3, [sp, #24]
30005e58:	4a49      	ldr	r2, [pc, #292]	; (30005f80 <BOOT_LoadSubImage+0x14c>)
30005e5a:	4293      	cmp	r3, r2
30005e5c:	d157      	bne.n	30005f0e <BOOT_LoadSubImage+0xda>
30005e5e:	f102 427b 	add.w	r2, r2, #4211081216	; 0xfb000000
30005e62:	9b07      	ldr	r3, [sp, #28]
30005e64:	f502 0278 	add.w	r2, r2, #16252928	; 0xf80000
30005e68:	f502 62c0 	add.w	r2, r2, #1536	; 0x600
30005e6c:	4293      	cmp	r3, r2
30005e6e:	d14e      	bne.n	30005f0e <BOOT_LoadSubImage+0xda>
30005e70:	4b44      	ldr	r3, [pc, #272]	; (30005f84 <BOOT_LoadSubImage+0x150>)
30005e72:	e9dd 4008 	ldrd	r4, r0, [sp, #32]
30005e76:	18c3      	adds	r3, r0, r3
30005e78:	f1a0 0820 	sub.w	r8, r0, #32
30005e7c:	3420      	adds	r4, #32
30005e7e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005e82:	d21a      	bcs.n	30005eba <BOOT_LoadSubImage+0x86>
30005e84:	b17d      	cbz	r5, 30005ea6 <BOOT_LoadSubImage+0x72>
30005e86:	1d2b      	adds	r3, r5, #4
30005e88:	f845 8036 	str.w	r8, [r5, r6, lsl #3]
30005e8c:	4a3e      	ldr	r2, [pc, #248]	; (30005f88 <BOOT_LoadSubImage+0x154>)
30005e8e:	2004      	movs	r0, #4
30005e90:	e9cd 8401 	strd	r8, r4, [sp, #4]
30005e94:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
30005e98:	f85a 3026 	ldr.w	r3, [sl, r6, lsl #2]
30005e9c:	9300      	str	r3, [sp, #0]
30005e9e:	4b3b      	ldr	r3, [pc, #236]	; (30005f8c <BOOT_LoadSubImage+0x158>)
30005ea0:	4619      	mov	r1, r3
30005ea2:	f005 f9c3 	bl	3000b22c <rtk_log_write>
30005ea6:	3601      	adds	r6, #1
30005ea8:	4427      	add	r7, r4
30005eaa:	454e      	cmp	r6, r9
30005eac:	d1ce      	bne.n	30005e4c <BOOT_LoadSubImage+0x18>
30005eae:	4b38      	ldr	r3, [pc, #224]	; (30005f90 <BOOT_LoadSubImage+0x15c>)
30005eb0:	2001      	movs	r0, #1
30005eb2:	601f      	str	r7, [r3, #0]
30005eb4:	b00f      	add	sp, #60	; 0x3c
30005eb6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005eba:	4639      	mov	r1, r7
30005ebc:	4622      	mov	r2, r4
30005ebe:	4640      	mov	r0, r8
30005ec0:	f7fe fcbc 	bl	3000483c <BOOT_ImgCopy>
30005ec4:	4933      	ldr	r1, [pc, #204]	; (30005f94 <BOOT_LoadSubImage+0x160>)
30005ec6:	694b      	ldr	r3, [r1, #20]
30005ec8:	03db      	lsls	r3, r3, #15
30005eca:	d5db      	bpl.n	30005e84 <BOOT_LoadSubImage+0x50>
30005ecc:	ea08 0304 	and.w	r3, r8, r4
30005ed0:	3301      	adds	r3, #1
30005ed2:	d023      	beq.n	30005f1c <BOOT_LoadSubImage+0xe8>
30005ed4:	f018 031f 	ands.w	r3, r8, #31
30005ed8:	d042      	beq.n	30005f60 <BOOT_LoadSubImage+0x12c>
30005eda:	4423      	add	r3, r4
30005edc:	f028 021f 	bic.w	r2, r8, #31
30005ee0:	f3bf 8f4f 	dsb	sy
30005ee4:	2b00      	cmp	r3, #0
30005ee6:	dd0d      	ble.n	30005f04 <BOOT_LoadSubImage+0xd0>
30005ee8:	1e59      	subs	r1, r3, #1
30005eea:	f102 0320 	add.w	r3, r2, #32
30005eee:	4829      	ldr	r0, [pc, #164]	; (30005f94 <BOOT_LoadSubImage+0x160>)
30005ef0:	f021 011f 	bic.w	r1, r1, #31
30005ef4:	4419      	add	r1, r3
30005ef6:	428b      	cmp	r3, r1
30005ef8:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30005efc:	461a      	mov	r2, r3
30005efe:	f103 0320 	add.w	r3, r3, #32
30005f02:	d1f8      	bne.n	30005ef6 <BOOT_LoadSubImage+0xc2>
30005f04:	f3bf 8f4f 	dsb	sy
30005f08:	f3bf 8f6f 	isb	sy
30005f0c:	e7ba      	b.n	30005e84 <BOOT_LoadSubImage+0x50>
30005f0e:	9b05      	ldr	r3, [sp, #20]
30005f10:	2b01      	cmp	r3, #1
30005f12:	d028      	beq.n	30005f66 <BOOT_LoadSubImage+0x132>
30005f14:	2000      	movs	r0, #0
30005f16:	b00f      	add	sp, #60	; 0x3c
30005f18:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005f1c:	2300      	movs	r3, #0
30005f1e:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30005f22:	f3bf 8f4f 	dsb	sy
30005f26:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30005f2a:	f643 7be0 	movw	fp, #16352	; 0x3fe0
30005f2e:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
30005f32:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30005f36:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30005f3a:	ea0c 000b 	and.w	r0, ip, fp
30005f3e:	4673      	mov	r3, lr
30005f40:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30005f44:	3b01      	subs	r3, #1
30005f46:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30005f4a:	d2f9      	bcs.n	30005f40 <BOOT_LoadSubImage+0x10c>
30005f4c:	f1ac 0c20 	sub.w	ip, ip, #32
30005f50:	f11c 0f20 	cmn.w	ip, #32
30005f54:	d1f1      	bne.n	30005f3a <BOOT_LoadSubImage+0x106>
30005f56:	f3bf 8f4f 	dsb	sy
30005f5a:	f3bf 8f6f 	isb	sy
30005f5e:	e791      	b.n	30005e84 <BOOT_LoadSubImage+0x50>
30005f60:	4623      	mov	r3, r4
30005f62:	4642      	mov	r2, r8
30005f64:	e7bc      	b.n	30005ee0 <BOOT_LoadSubImage+0xac>
30005f66:	f85a 4026 	ldr.w	r4, [sl, r6, lsl #2]
30005f6a:	2004      	movs	r0, #4
30005f6c:	4b07      	ldr	r3, [pc, #28]	; (30005f8c <BOOT_LoadSubImage+0x158>)
30005f6e:	4a0a      	ldr	r2, [pc, #40]	; (30005f98 <BOOT_LoadSubImage+0x164>)
30005f70:	4619      	mov	r1, r3
30005f72:	9400      	str	r4, [sp, #0]
30005f74:	f005 f95a 	bl	3000b22c <rtk_log_write>
30005f78:	2000      	movs	r0, #0
30005f7a:	b00f      	add	sp, #60	; 0x3c
30005f7c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005f80:	35393138 	.word	0x35393138
30005f84:	f7ffffe0 	.word	0xf7ffffe0
30005f88:	3000c1e0 	.word	0x3000c1e0
30005f8c:	3000b9d0 	.word	0x3000b9d0
30005f90:	3000df0c 	.word	0x3000df0c
30005f94:	e000ed00 	.word	0xe000ed00
30005f98:	3000c1cc 	.word	0x3000c1cc

30005f9c <BOOT_LoadAPSubImage>:
30005f9c:	4bb5      	ldr	r3, [pc, #724]	; (30006274 <BOOT_LoadAPSubImage+0x2d8>)
30005f9e:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30005fa2:	460c      	mov	r4, r1
30005fa4:	4606      	mov	r6, r0
30005fa6:	b08f      	sub	sp, #60	; 0x3c
30005fa8:	4615      	mov	r5, r2
30005faa:	2220      	movs	r2, #32
30005fac:	e893 0003 	ldmia.w	r3, {r0, r1}
30005fb0:	0c0b      	lsrs	r3, r1, #16
30005fb2:	9004      	str	r0, [sp, #16]
30005fb4:	f8ad 1014 	strh.w	r1, [sp, #20]
30005fb8:	a806      	add	r0, sp, #24
30005fba:	4621      	mov	r1, r4
30005fbc:	f88d 3016 	strb.w	r3, [sp, #22]
30005fc0:	f7fe fc3c 	bl	3000483c <BOOT_ImgCopy>
30005fc4:	4bac      	ldr	r3, [pc, #688]	; (30006278 <BOOT_LoadAPSubImage+0x2dc>)
30005fc6:	9a06      	ldr	r2, [sp, #24]
30005fc8:	429a      	cmp	r2, r3
30005fca:	d108      	bne.n	30005fde <BOOT_LoadAPSubImage+0x42>
30005fcc:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30005fd0:	9a07      	ldr	r2, [sp, #28]
30005fd2:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30005fd6:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30005fda:	429a      	cmp	r2, r3
30005fdc:	d006      	beq.n	30005fec <BOOT_LoadAPSubImage+0x50>
30005fde:	2d01      	cmp	r5, #1
30005fe0:	f000 80b3 	beq.w	3000614a <BOOT_LoadAPSubImage+0x1ae>
30005fe4:	2000      	movs	r0, #0
30005fe6:	b00f      	add	sp, #60	; 0x3c
30005fe8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30005fec:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005fee:	4ba3      	ldr	r3, [pc, #652]	; (3000627c <BOOT_LoadAPSubImage+0x2e0>)
30005ff0:	6ca7      	ldr	r7, [r4, #72]	; 0x48
30005ff2:	f1a2 0820 	sub.w	r8, r2, #32
30005ff6:	4413      	add	r3, r2
30005ff8:	37d0      	adds	r7, #208	; 0xd0
30005ffa:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005ffe:	d247      	bcs.n	30006090 <BOOT_LoadAPSubImage+0xf4>
30006000:	4b9f      	ldr	r3, [pc, #636]	; (30006280 <BOOT_LoadAPSubImage+0x2e4>)
30006002:	eb07 0508 	add.w	r5, r7, r8
30006006:	f8d4 8070 	ldr.w	r8, [r4, #112]	; 0x70
3000600a:	4798      	blx	r3
3000600c:	2801      	cmp	r0, #1
3000600e:	4427      	add	r7, r4
30006010:	4b9c      	ldr	r3, [pc, #624]	; (30006284 <BOOT_LoadAPSubImage+0x2e8>)
30006012:	d029      	beq.n	30006068 <BOOT_LoadAPSubImage+0xcc>
30006014:	4619      	mov	r1, r3
30006016:	4a9c      	ldr	r2, [pc, #624]	; (30006288 <BOOT_LoadAPSubImage+0x2ec>)
30006018:	2004      	movs	r0, #4
3000601a:	f005 f907 	bl	3000b22c <rtk_log_write>
3000601e:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30006022:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006026:	f080 809c 	bcs.w	30006162 <BOOT_LoadAPSubImage+0x1c6>
3000602a:	4445      	add	r5, r8
3000602c:	eb08 0107 	add.w	r1, r8, r7
30006030:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30006034:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006038:	d256      	bcs.n	300060e8 <BOOT_LoadAPSubImage+0x14c>
3000603a:	2e00      	cmp	r6, #0
3000603c:	f000 8081 	beq.w	30006142 <BOOT_LoadAPSubImage+0x1a6>
30006040:	4b90      	ldr	r3, [pc, #576]	; (30006284 <BOOT_LoadAPSubImage+0x2e8>)
30006042:	aa04      	add	r2, sp, #16
30006044:	2004      	movs	r0, #4
30006046:	9200      	str	r2, [sp, #0]
30006048:	4a90      	ldr	r2, [pc, #576]	; (3000628c <BOOT_LoadAPSubImage+0x2f0>)
3000604a:	e9dd 1408 	ldrd	r1, r4, [sp, #32]
3000604e:	3c20      	subs	r4, #32
30006050:	3120      	adds	r1, #32
30006052:	e9c6 4100 	strd	r4, r1, [r6]
30006056:	9102      	str	r1, [sp, #8]
30006058:	4619      	mov	r1, r3
3000605a:	9401      	str	r4, [sp, #4]
3000605c:	f005 f8e6 	bl	3000b22c <rtk_log_write>
30006060:	2001      	movs	r0, #1
30006062:	b00f      	add	sp, #60	; 0x3c
30006064:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006068:	4a89      	ldr	r2, [pc, #548]	; (30006290 <BOOT_LoadAPSubImage+0x2f4>)
3000606a:	2004      	movs	r0, #4
3000606c:	4619      	mov	r1, r3
3000606e:	f005 f8dd 	bl	3000b22c <rtk_log_write>
30006072:	4642      	mov	r2, r8
30006074:	4629      	mov	r1, r5
30006076:	4638      	mov	r0, r7
30006078:	f000 fbe4 	bl	30006844 <BOOT_LoadBL32RDPImg>
3000607c:	2800      	cmp	r0, #0
3000607e:	d1d4      	bne.n	3000602a <BOOT_LoadAPSubImage+0x8e>
30006080:	4b80      	ldr	r3, [pc, #512]	; (30006284 <BOOT_LoadAPSubImage+0x2e8>)
30006082:	2004      	movs	r0, #4
30006084:	4a83      	ldr	r2, [pc, #524]	; (30006294 <BOOT_LoadAPSubImage+0x2f8>)
30006086:	4619      	mov	r1, r3
30006088:	f005 f8d0 	bl	3000b22c <rtk_log_write>
3000608c:	20ff      	movs	r0, #255	; 0xff
3000608e:	e7aa      	b.n	30005fe6 <BOOT_LoadAPSubImage+0x4a>
30006090:	463a      	mov	r2, r7
30006092:	4621      	mov	r1, r4
30006094:	4640      	mov	r0, r8
30006096:	f7fe fbd1 	bl	3000483c <BOOT_ImgCopy>
3000609a:	497f      	ldr	r1, [pc, #508]	; (30006298 <BOOT_LoadAPSubImage+0x2fc>)
3000609c:	694b      	ldr	r3, [r1, #20]
3000609e:	03da      	lsls	r2, r3, #15
300060a0:	d5ae      	bpl.n	30006000 <BOOT_LoadAPSubImage+0x64>
300060a2:	ea07 0308 	and.w	r3, r7, r8
300060a6:	3301      	adds	r3, #1
300060a8:	f000 8086 	beq.w	300061b8 <BOOT_LoadAPSubImage+0x21c>
300060ac:	f018 031f 	ands.w	r3, r8, #31
300060b0:	f000 80bb 	beq.w	3000622a <BOOT_LoadAPSubImage+0x28e>
300060b4:	443b      	add	r3, r7
300060b6:	f028 021f 	bic.w	r2, r8, #31
300060ba:	f3bf 8f4f 	dsb	sy
300060be:	2b00      	cmp	r3, #0
300060c0:	dd0d      	ble.n	300060de <BOOT_LoadAPSubImage+0x142>
300060c2:	1e59      	subs	r1, r3, #1
300060c4:	f102 0320 	add.w	r3, r2, #32
300060c8:	4873      	ldr	r0, [pc, #460]	; (30006298 <BOOT_LoadAPSubImage+0x2fc>)
300060ca:	f021 011f 	bic.w	r1, r1, #31
300060ce:	4419      	add	r1, r3
300060d0:	428b      	cmp	r3, r1
300060d2:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300060d6:	461a      	mov	r2, r3
300060d8:	f103 0320 	add.w	r3, r3, #32
300060dc:	d1f8      	bne.n	300060d0 <BOOT_LoadAPSubImage+0x134>
300060de:	f3bf 8f4f 	dsb	sy
300060e2:	f3bf 8f6f 	isb	sy
300060e6:	e78b      	b.n	30006000 <BOOT_LoadAPSubImage+0x64>
300060e8:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
300060ec:	4628      	mov	r0, r5
300060ee:	4622      	mov	r2, r4
300060f0:	f7fe fba4 	bl	3000483c <BOOT_ImgCopy>
300060f4:	4968      	ldr	r1, [pc, #416]	; (30006298 <BOOT_LoadAPSubImage+0x2fc>)
300060f6:	694b      	ldr	r3, [r1, #20]
300060f8:	03d8      	lsls	r0, r3, #15
300060fa:	d59e      	bpl.n	3000603a <BOOT_LoadAPSubImage+0x9e>
300060fc:	ea05 0304 	and.w	r3, r5, r4
30006100:	3301      	adds	r3, #1
30006102:	d075      	beq.n	300061f0 <BOOT_LoadAPSubImage+0x254>
30006104:	f015 031f 	ands.w	r3, r5, #31
30006108:	d002      	beq.n	30006110 <BOOT_LoadAPSubImage+0x174>
3000610a:	f025 051f 	bic.w	r5, r5, #31
3000610e:	441c      	add	r4, r3
30006110:	f3bf 8f4f 	dsb	sy
30006114:	2c00      	cmp	r4, #0
30006116:	dd0d      	ble.n	30006134 <BOOT_LoadAPSubImage+0x198>
30006118:	1e62      	subs	r2, r4, #1
3000611a:	f105 0320 	add.w	r3, r5, #32
3000611e:	495e      	ldr	r1, [pc, #376]	; (30006298 <BOOT_LoadAPSubImage+0x2fc>)
30006120:	f022 021f 	bic.w	r2, r2, #31
30006124:	441a      	add	r2, r3
30006126:	4293      	cmp	r3, r2
30006128:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
3000612c:	461d      	mov	r5, r3
3000612e:	f103 0320 	add.w	r3, r3, #32
30006132:	d1f8      	bne.n	30006126 <BOOT_LoadAPSubImage+0x18a>
30006134:	f3bf 8f4f 	dsb	sy
30006138:	f3bf 8f6f 	isb	sy
3000613c:	2e00      	cmp	r6, #0
3000613e:	f47f af7f 	bne.w	30006040 <BOOT_LoadAPSubImage+0xa4>
30006142:	2001      	movs	r0, #1
30006144:	b00f      	add	sp, #60	; 0x3c
30006146:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000614a:	4b4e      	ldr	r3, [pc, #312]	; (30006284 <BOOT_LoadAPSubImage+0x2e8>)
3000614c:	ac04      	add	r4, sp, #16
3000614e:	4a53      	ldr	r2, [pc, #332]	; (3000629c <BOOT_LoadAPSubImage+0x300>)
30006150:	2004      	movs	r0, #4
30006152:	4619      	mov	r1, r3
30006154:	9400      	str	r4, [sp, #0]
30006156:	f005 f869 	bl	3000b22c <rtk_log_write>
3000615a:	2000      	movs	r0, #0
3000615c:	b00f      	add	sp, #60	; 0x3c
3000615e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006162:	4639      	mov	r1, r7
30006164:	4642      	mov	r2, r8
30006166:	4628      	mov	r0, r5
30006168:	f7fe fb68 	bl	3000483c <BOOT_ImgCopy>
3000616c:	494a      	ldr	r1, [pc, #296]	; (30006298 <BOOT_LoadAPSubImage+0x2fc>)
3000616e:	694b      	ldr	r3, [r1, #20]
30006170:	03db      	lsls	r3, r3, #15
30006172:	f57f af5a 	bpl.w	3000602a <BOOT_LoadAPSubImage+0x8e>
30006176:	ea08 0305 	and.w	r3, r8, r5
3000617a:	3301      	adds	r3, #1
3000617c:	d058      	beq.n	30006230 <BOOT_LoadAPSubImage+0x294>
3000617e:	f015 031f 	ands.w	r3, r5, #31
30006182:	d073      	beq.n	3000626c <BOOT_LoadAPSubImage+0x2d0>
30006184:	4443      	add	r3, r8
30006186:	f025 021f 	bic.w	r2, r5, #31
3000618a:	f3bf 8f4f 	dsb	sy
3000618e:	2b00      	cmp	r3, #0
30006190:	dd0d      	ble.n	300061ae <BOOT_LoadAPSubImage+0x212>
30006192:	1e59      	subs	r1, r3, #1
30006194:	f102 0320 	add.w	r3, r2, #32
30006198:	483f      	ldr	r0, [pc, #252]	; (30006298 <BOOT_LoadAPSubImage+0x2fc>)
3000619a:	f021 011f 	bic.w	r1, r1, #31
3000619e:	4419      	add	r1, r3
300061a0:	428b      	cmp	r3, r1
300061a2:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300061a6:	461a      	mov	r2, r3
300061a8:	f103 0320 	add.w	r3, r3, #32
300061ac:	d1f8      	bne.n	300061a0 <BOOT_LoadAPSubImage+0x204>
300061ae:	f3bf 8f4f 	dsb	sy
300061b2:	f3bf 8f6f 	isb	sy
300061b6:	e738      	b.n	3000602a <BOOT_LoadAPSubImage+0x8e>
300061b8:	2300      	movs	r3, #0
300061ba:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300061be:	f3bf 8f4f 	dsb	sy
300061c2:	f8d1 0080 	ldr.w	r0, [r1, #128]	; 0x80
300061c6:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300061ca:	f3c0 324e 	ubfx	r2, r0, #13, #15
300061ce:	f3c0 00c9 	ubfx	r0, r0, #3, #10
300061d2:	0152      	lsls	r2, r2, #5
300061d4:	ea02 0c0e 	and.w	ip, r2, lr
300061d8:	4603      	mov	r3, r0
300061da:	ea4c 7583 	orr.w	r5, ip, r3, lsl #30
300061de:	3b01      	subs	r3, #1
300061e0:	f8c1 5274 	str.w	r5, [r1, #628]	; 0x274
300061e4:	d2f9      	bcs.n	300061da <BOOT_LoadAPSubImage+0x23e>
300061e6:	3a20      	subs	r2, #32
300061e8:	f112 0f20 	cmn.w	r2, #32
300061ec:	d1f2      	bne.n	300061d4 <BOOT_LoadAPSubImage+0x238>
300061ee:	e776      	b.n	300060de <BOOT_LoadAPSubImage+0x142>
300061f0:	2300      	movs	r3, #0
300061f2:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300061f6:	f3bf 8f4f 	dsb	sy
300061fa:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
300061fe:	f643 77e0 	movw	r7, #16352	; 0x3fe0
30006202:	f3c5 344e 	ubfx	r4, r5, #13, #15
30006206:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000620a:	0164      	lsls	r4, r4, #5
3000620c:	ea04 0007 	and.w	r0, r4, r7
30006210:	462b      	mov	r3, r5
30006212:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30006216:	3b01      	subs	r3, #1
30006218:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
3000621c:	1c5a      	adds	r2, r3, #1
3000621e:	d1f8      	bne.n	30006212 <BOOT_LoadAPSubImage+0x276>
30006220:	3c20      	subs	r4, #32
30006222:	f114 0f20 	cmn.w	r4, #32
30006226:	d1f1      	bne.n	3000620c <BOOT_LoadAPSubImage+0x270>
30006228:	e784      	b.n	30006134 <BOOT_LoadAPSubImage+0x198>
3000622a:	463b      	mov	r3, r7
3000622c:	4642      	mov	r2, r8
3000622e:	e744      	b.n	300060ba <BOOT_LoadAPSubImage+0x11e>
30006230:	2300      	movs	r3, #0
30006232:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30006236:	f3bf 8f4f 	dsb	sy
3000623a:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
3000623e:	f643 79e0 	movw	r9, #16352	; 0x3fe0
30006242:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
30006246:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000624a:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
3000624e:	ea0c 0009 	and.w	r0, ip, r9
30006252:	4673      	mov	r3, lr
30006254:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30006258:	3b01      	subs	r3, #1
3000625a:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
3000625e:	d2f9      	bcs.n	30006254 <BOOT_LoadAPSubImage+0x2b8>
30006260:	f1ac 0c20 	sub.w	ip, ip, #32
30006264:	f11c 0f20 	cmn.w	ip, #32
30006268:	d1f1      	bne.n	3000624e <BOOT_LoadAPSubImage+0x2b2>
3000626a:	e7a0      	b.n	300061ae <BOOT_LoadAPSubImage+0x212>
3000626c:	4643      	mov	r3, r8
3000626e:	462a      	mov	r2, r5
30006270:	e78b      	b.n	3000618a <BOOT_LoadAPSubImage+0x1ee>
30006272:	bf00      	nop
30006274:	3000c1c4 	.word	0x3000c1c4
30006278:	35393138 	.word	0x35393138
3000627c:	f7ffffe0 	.word	0xf7ffffe0
30006280:	0000c109 	.word	0x0000c109
30006284:	3000b9d0 	.word	0x3000b9d0
30006288:	3000c224 	.word	0x3000c224
3000628c:	3000c1e0 	.word	0x3000c1e0
30006290:	3000c1f4 	.word	0x3000c1f4
30006294:	3000c204 	.word	0x3000c204
30006298:	e000ed00 	.word	0xe000ed00
3000629c:	3000c1cc 	.word	0x3000c1cc

300062a0 <BOOT_RSIPIvSet>:
300062a0:	b530      	push	{r4, r5, lr}
300062a2:	2300      	movs	r3, #0
300062a4:	b083      	sub	sp, #12
300062a6:	4a09      	ldr	r2, [pc, #36]	; (300062cc <BOOT_RSIPIvSet+0x2c>)
300062a8:	4605      	mov	r5, r0
300062aa:	460c      	mov	r4, r1
300062ac:	9300      	str	r3, [sp, #0]
300062ae:	9301      	str	r3, [sp, #4]
300062b0:	4790      	blx	r2
300062b2:	b148      	cbz	r0, 300062c8 <BOOT_RSIPIvSet+0x28>
300062b4:	f105 0120 	add.w	r1, r5, #32
300062b8:	4b05      	ldr	r3, [pc, #20]	; (300062d0 <BOOT_RSIPIvSet+0x30>)
300062ba:	2208      	movs	r2, #8
300062bc:	4668      	mov	r0, sp
300062be:	4798      	blx	r3
300062c0:	4669      	mov	r1, sp
300062c2:	b2e0      	uxtb	r0, r4
300062c4:	4b03      	ldr	r3, [pc, #12]	; (300062d4 <BOOT_RSIPIvSet+0x34>)
300062c6:	4798      	blx	r3
300062c8:	b003      	add	sp, #12
300062ca:	bd30      	pop	{r4, r5, pc}
300062cc:	0000c129 	.word	0x0000c129
300062d0:	00012a1d 	.word	0x00012a1d
300062d4:	30008c65 	.word	0x30008c65

300062d8 <BOOT_OTFCheck>:
300062d8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300062dc:	4d1c      	ldr	r5, [pc, #112]	; (30006350 <BOOT_OTFCheck+0x78>)
300062de:	b082      	sub	sp, #8
300062e0:	4604      	mov	r4, r0
300062e2:	4688      	mov	r8, r1
300062e4:	4617      	mov	r7, r2
300062e6:	461e      	mov	r6, r3
300062e8:	47a8      	blx	r5
300062ea:	b148      	cbz	r0, 30006300 <BOOT_OTFCheck+0x28>
300062ec:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
300062f0:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300062f4:	d204      	bcs.n	30006300 <BOOT_OTFCheck+0x28>
300062f6:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
300062fa:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300062fe:	d302      	bcc.n	30006306 <BOOT_OTFCheck+0x2e>
30006300:	b002      	add	sp, #8
30006302:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30006306:	4b13      	ldr	r3, [pc, #76]	; (30006354 <BOOT_OTFCheck+0x7c>)
30006308:	2004      	movs	r0, #4
3000630a:	4a13      	ldr	r2, [pc, #76]	; (30006358 <BOOT_OTFCheck+0x80>)
3000630c:	4619      	mov	r1, r3
3000630e:	f004 ff8d 	bl	3000b22c <rtk_log_write>
30006312:	4b12      	ldr	r3, [pc, #72]	; (3000635c <BOOT_OTFCheck+0x84>)
30006314:	4798      	blx	r3
30006316:	4603      	mov	r3, r0
30006318:	b150      	cbz	r0, 30006330 <BOOT_OTFCheck+0x58>
3000631a:	2801      	cmp	r0, #1
3000631c:	d009      	beq.n	30006332 <BOOT_OTFCheck+0x5a>
3000631e:	4b0d      	ldr	r3, [pc, #52]	; (30006354 <BOOT_OTFCheck+0x7c>)
30006320:	2002      	movs	r0, #2
30006322:	4a0f      	ldr	r2, [pc, #60]	; (30006360 <BOOT_OTFCheck+0x88>)
30006324:	4619      	mov	r1, r3
30006326:	b002      	add	sp, #8
30006328:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000632c:	f004 bf7e 	b.w	3000b22c <rtk_log_write>
30006330:	2302      	movs	r3, #2
30006332:	4630      	mov	r0, r6
30006334:	4621      	mov	r1, r4
30006336:	9301      	str	r3, [sp, #4]
30006338:	4642      	mov	r2, r8
3000633a:	2301      	movs	r3, #1
3000633c:	4c09      	ldr	r4, [pc, #36]	; (30006364 <BOOT_OTFCheck+0x8c>)
3000633e:	9700      	str	r7, [sp, #0]
30006340:	47a0      	blx	r4
30006342:	2001      	movs	r0, #1
30006344:	4b08      	ldr	r3, [pc, #32]	; (30006368 <BOOT_OTFCheck+0x90>)
30006346:	b002      	add	sp, #8
30006348:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000634c:	4718      	bx	r3
3000634e:	bf00      	nop
30006350:	0000c129 	.word	0x0000c129
30006354:	3000b9d0 	.word	0x3000b9d0
30006358:	3000c234 	.word	0x3000c234
3000635c:	0000c245 	.word	0x0000c245
30006360:	3000c248 	.word	0x3000c248
30006364:	30008c99 	.word	0x30008c99
30006368:	30008c81 	.word	0x30008c81

3000636c <BOOT_OTA_LoadIMG2>:
3000636c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006370:	4b6e      	ldr	r3, [pc, #440]	; (3000652c <BOOT_OTA_LoadIMG2+0x1c0>)
30006372:	b08d      	sub	sp, #52	; 0x34
30006374:	4a6e      	ldr	r2, [pc, #440]	; (30006530 <BOOT_OTA_LoadIMG2+0x1c4>)
30006376:	f103 040c 	add.w	r4, r3, #12
3000637a:	9003      	str	r0, [sp, #12]
3000637c:	f8d2 8000 	ldr.w	r8, [r2]
30006380:	ae06      	add	r6, sp, #24
30006382:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30006386:	ab09      	add	r3, sp, #36	; 0x24
30006388:	e886 0007 	stmia.w	r6, {r0, r1, r2}
3000638c:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30006390:	e883 0007 	stmia.w	r3, {r0, r1, r2}
30006394:	4641      	mov	r1, r8
30006396:	4867      	ldr	r0, [pc, #412]	; (30006534 <BOOT_OTA_LoadIMG2+0x1c8>)
30006398:	f44f 72b2 	mov.w	r2, #356	; 0x164
3000639c:	f7fe fa4e 	bl	3000483c <BOOT_ImgCopy>
300063a0:	4641      	mov	r1, r8
300063a2:	4864      	ldr	r0, [pc, #400]	; (30006534 <BOOT_OTA_LoadIMG2+0x1c8>)
300063a4:	f000 fd2a 	bl	30006dfc <BOOT_CertificateCheck>
300063a8:	2800      	cmp	r0, #0
300063aa:	f040 80b7 	bne.w	3000651c <BOOT_OTA_LoadIMG2+0x1b0>
300063ae:	4604      	mov	r4, r0
300063b0:	f44f 72c8 	mov.w	r2, #400	; 0x190
300063b4:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
300063b8:	485f      	ldr	r0, [pc, #380]	; (30006538 <BOOT_OTA_LoadIMG2+0x1cc>)
300063ba:	f7fe fa3f 	bl	3000483c <BOOT_ImgCopy>
300063be:	4b5f      	ldr	r3, [pc, #380]	; (3000653c <BOOT_OTA_LoadIMG2+0x1d0>)
300063c0:	e9cd 4404 	strd	r4, r4, [sp, #16]
300063c4:	4798      	blx	r3
300063c6:	2800      	cmp	r0, #0
300063c8:	f040 809e 	bne.w	30006508 <BOOT_OTA_LoadIMG2+0x19c>
300063cc:	f641 75f0 	movw	r5, #8176	; 0x1ff0
300063d0:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30006580 <BOOT_OTA_LoadIMG2+0x214>
300063d4:	4a5a      	ldr	r2, [pc, #360]	; (30006540 <BOOT_OTA_LoadIMG2+0x1d4>)
300063d6:	2000      	movs	r0, #0
300063d8:	4445      	add	r5, r8
300063da:	4659      	mov	r1, fp
300063dc:	4f59      	ldr	r7, [pc, #356]	; (30006544 <BOOT_OTA_LoadIMG2+0x1d8>)
300063de:	f04f 0801 	mov.w	r8, #1
300063e2:	462b      	mov	r3, r5
300063e4:	f8df 919c 	ldr.w	r9, [pc, #412]	; 30006584 <BOOT_OTA_LoadIMG2+0x218>
300063e8:	47b8      	blx	r7
300063ea:	2101      	movs	r1, #1
300063ec:	2000      	movs	r0, #0
300063ee:	4b56      	ldr	r3, [pc, #344]	; (30006548 <BOOT_OTA_LoadIMG2+0x1dc>)
300063f0:	4798      	blx	r3
300063f2:	2301      	movs	r3, #1
300063f4:	4952      	ldr	r1, [pc, #328]	; (30006540 <BOOT_OTA_LoadIMG2+0x1d4>)
300063f6:	4658      	mov	r0, fp
300063f8:	461a      	mov	r2, r3
300063fa:	f7ff ff6d 	bl	300062d8 <BOOT_OTFCheck>
300063fe:	47c8      	blx	r9
30006400:	2800      	cmp	r0, #0
30006402:	bf0c      	ite	eq
30006404:	46aa      	moveq	sl, r5
30006406:	46da      	movne	sl, fp
30006408:	4633      	mov	r3, r6
3000640a:	2203      	movs	r2, #3
3000640c:	4651      	mov	r1, sl
3000640e:	484f      	ldr	r0, [pc, #316]	; (3000654c <BOOT_OTA_LoadIMG2+0x1e0>)
30006410:	f8cd 8000 	str.w	r8, [sp]
30006414:	f7ff fd0e 	bl	30005e34 <BOOT_LoadSubImage>
30006418:	2800      	cmp	r0, #0
3000641a:	d070      	beq.n	300064fe <BOOT_OTA_LoadIMG2+0x192>
3000641c:	4e4b      	ldr	r6, [pc, #300]	; (3000654c <BOOT_OTA_LoadIMG2+0x1e0>)
3000641e:	47c8      	blx	r9
30006420:	2800      	cmp	r0, #0
30006422:	bf08      	it	eq
30006424:	46ab      	moveq	fp, r5
30006426:	4f47      	ldr	r7, [pc, #284]	; (30006544 <BOOT_OTA_LoadIMG2+0x1d8>)
30006428:	68f2      	ldr	r2, [r6, #12]
3000642a:	4640      	mov	r0, r8
3000642c:	6873      	ldr	r3, [r6, #4]
3000642e:	f8c6 b000 	str.w	fp, [r6]
30006432:	4413      	add	r3, r2
30006434:	6972      	ldr	r2, [r6, #20]
30006436:	f8df b150 	ldr.w	fp, [pc, #336]	; 30006588 <BOOT_OTA_LoadIMG2+0x21c>
3000643a:	441a      	add	r2, r3
3000643c:	4b44      	ldr	r3, [pc, #272]	; (30006550 <BOOT_OTA_LoadIMG2+0x1e4>)
3000643e:	eb0a 0102 	add.w	r1, sl, r2
30006442:	61b1      	str	r1, [r6, #24]
30006444:	1e51      	subs	r1, r2, #1
30006446:	400b      	ands	r3, r1
30006448:	4659      	mov	r1, fp
3000644a:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
3000644e:	441d      	add	r5, r3
30006450:	1a9b      	subs	r3, r3, r2
30006452:	4a40      	ldr	r2, [pc, #256]	; (30006554 <BOOT_OTA_LoadIMG2+0x1e8>)
30006454:	61f3      	str	r3, [r6, #28]
30006456:	462b      	mov	r3, r5
30006458:	47b8      	blx	r7
3000645a:	4641      	mov	r1, r8
3000645c:	4640      	mov	r0, r8
3000645e:	4b3a      	ldr	r3, [pc, #232]	; (30006548 <BOOT_OTA_LoadIMG2+0x1dc>)
30006460:	4798      	blx	r3
30006462:	4642      	mov	r2, r8
30006464:	2302      	movs	r3, #2
30006466:	493b      	ldr	r1, [pc, #236]	; (30006554 <BOOT_OTA_LoadIMG2+0x1e8>)
30006468:	4658      	mov	r0, fp
3000646a:	f7ff ff35 	bl	300062d8 <BOOT_OTFCheck>
3000646e:	47c8      	blx	r9
30006470:	2800      	cmp	r0, #0
30006472:	bf0c      	ite	eq
30006474:	462f      	moveq	r7, r5
30006476:	465f      	movne	r7, fp
30006478:	ab09      	add	r3, sp, #36	; 0x24
3000647a:	2203      	movs	r2, #3
3000647c:	f106 0020 	add.w	r0, r6, #32
30006480:	4639      	mov	r1, r7
30006482:	f8cd 8000 	str.w	r8, [sp]
30006486:	f7ff fcd5 	bl	30005e34 <BOOT_LoadSubImage>
3000648a:	2800      	cmp	r0, #0
3000648c:	d037      	beq.n	300064fe <BOOT_OTA_LoadIMG2+0x192>
3000648e:	47c8      	blx	r9
30006490:	6af2      	ldr	r2, [r6, #44]	; 0x2c
30006492:	6a73      	ldr	r3, [r6, #36]	; 0x24
30006494:	2800      	cmp	r0, #0
30006496:	bf08      	it	eq
30006498:	46ab      	moveq	fp, r5
3000649a:	4827      	ldr	r0, [pc, #156]	; (30006538 <BOOT_OTA_LoadIMG2+0x1cc>)
3000649c:	4413      	add	r3, r2
3000649e:	6b72      	ldr	r2, [r6, #52]	; 0x34
300064a0:	f8c6 b020 	str.w	fp, [r6, #32]
300064a4:	4413      	add	r3, r2
300064a6:	4a2a      	ldr	r2, [pc, #168]	; (30006550 <BOOT_OTA_LoadIMG2+0x1e4>)
300064a8:	1e59      	subs	r1, r3, #1
300064aa:	441f      	add	r7, r3
300064ac:	400a      	ands	r2, r1
300064ae:	2100      	movs	r1, #0
300064b0:	63b7      	str	r7, [r6, #56]	; 0x38
300064b2:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
300064b6:	9100      	str	r1, [sp, #0]
300064b8:	1ad1      	subs	r1, r2, r3
300064ba:	2900      	cmp	r1, #0
300064bc:	63f1      	str	r1, [r6, #60]	; 0x3c
300064be:	4631      	mov	r1, r6
300064c0:	bf18      	it	ne
300064c2:	4613      	movne	r3, r2
300064c4:	4a24      	ldr	r2, [pc, #144]	; (30006558 <BOOT_OTA_LoadIMG2+0x1ec>)
300064c6:	441d      	add	r5, r3
300064c8:	4b1a      	ldr	r3, [pc, #104]	; (30006534 <BOOT_OTA_LoadIMG2+0x1c8>)
300064ca:	6015      	str	r5, [r2, #0]
300064cc:	bf0c      	ite	eq
300064ce:	2207      	moveq	r2, #7
300064d0:	2208      	movne	r2, #8
300064d2:	f000 fb89 	bl	30006be8 <BOOT_SignatureCheck>
300064d6:	4b21      	ldr	r3, [pc, #132]	; (3000655c <BOOT_OTA_LoadIMG2+0x1f0>)
300064d8:	4605      	mov	r5, r0
300064da:	689b      	ldr	r3, [r3, #8]
300064dc:	079b      	lsls	r3, r3, #30
300064de:	d50a      	bpl.n	300064f6 <BOOT_OTA_LoadIMG2+0x18a>
300064e0:	4b1f      	ldr	r3, [pc, #124]	; (30006560 <BOOT_OTA_LoadIMG2+0x1f4>)
300064e2:	4a20      	ldr	r2, [pc, #128]	; (30006564 <BOOT_OTA_LoadIMG2+0x1f8>)
300064e4:	9903      	ldr	r1, [sp, #12]
300064e6:	2800      	cmp	r0, #0
300064e8:	bf18      	it	ne
300064ea:	461a      	movne	r2, r3
300064ec:	481e      	ldr	r0, [pc, #120]	; (30006568 <BOOT_OTA_LoadIMG2+0x1fc>)
300064ee:	3101      	adds	r1, #1
300064f0:	462b      	mov	r3, r5
300064f2:	f005 f939 	bl	3000b768 <__DiagPrintf_veneer>
300064f6:	b11d      	cbz	r5, 30006500 <BOOT_OTA_LoadIMG2+0x194>
300064f8:	481c      	ldr	r0, [pc, #112]	; (3000656c <BOOT_OTA_LoadIMG2+0x200>)
300064fa:	f005 f935 	bl	3000b768 <__DiagPrintf_veneer>
300064fe:	24ff      	movs	r4, #255	; 0xff
30006500:	4620      	mov	r0, r4
30006502:	b00d      	add	sp, #52	; 0x34
30006504:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006508:	2208      	movs	r2, #8
3000650a:	4919      	ldr	r1, [pc, #100]	; (30006570 <BOOT_OTA_LoadIMG2+0x204>)
3000650c:	4b19      	ldr	r3, [pc, #100]	; (30006574 <BOOT_OTA_LoadIMG2+0x208>)
3000650e:	a804      	add	r0, sp, #16
30006510:	4798      	blx	r3
30006512:	a904      	add	r1, sp, #16
30006514:	2001      	movs	r0, #1
30006516:	4b18      	ldr	r3, [pc, #96]	; (30006578 <BOOT_OTA_LoadIMG2+0x20c>)
30006518:	4798      	blx	r3
3000651a:	e757      	b.n	300063cc <BOOT_OTA_LoadIMG2+0x60>
3000651c:	24ff      	movs	r4, #255	; 0xff
3000651e:	4817      	ldr	r0, [pc, #92]	; (3000657c <BOOT_OTA_LoadIMG2+0x210>)
30006520:	f005 f922 	bl	3000b768 <__DiagPrintf_veneer>
30006524:	4620      	mov	r0, r4
30006526:	b00d      	add	sp, #52	; 0x34
30006528:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000652c:	3000d614 	.word	0x3000d614
30006530:	3000edd8 	.word	0x3000edd8
30006534:	3000ee4c 	.word	0x3000ee4c
30006538:	3000efb0 	.word	0x3000efb0
3000653c:	0000c129 	.word	0x0000c129
30006540:	0cffffe0 	.word	0x0cffffe0
30006544:	30008d15 	.word	0x30008d15
30006548:	30008d55 	.word	0x30008d55
3000654c:	3000f158 	.word	0x3000f158
30006550:	fffff000 	.word	0xfffff000
30006554:	0dffffe0 	.word	0x0dffffe0
30006558:	3000df0c 	.word	0x3000df0c
3000655c:	2001c00c 	.word	0x2001c00c
30006560:	3000c268 	.word	0x3000c268
30006564:	3000c260 	.word	0x3000c260
30006568:	3000c298 	.word	0x3000c298
3000656c:	3000c2cc 	.word	0x3000c2cc
30006570:	3000efd0 	.word	0x3000efd0
30006574:	00012a1d 	.word	0x00012a1d
30006578:	30008c65 	.word	0x30008c65
3000657c:	3000c270 	.word	0x3000c270
30006580:	0c000000 	.word	0x0c000000
30006584:	30008ef5 	.word	0x30008ef5
30006588:	0d000000 	.word	0x0d000000

3000658c <BOOT_OTA_Region_Init>:
3000658c:	b510      	push	{r4, lr}
3000658e:	4c10      	ldr	r4, [pc, #64]	; (300065d0 <BOOT_OTA_Region_Init+0x44>)
30006590:	2200      	movs	r2, #0
30006592:	2002      	movs	r0, #2
30006594:	4621      	mov	r1, r4
30006596:	f001 fa73 	bl	30007a80 <flash_get_layout_info>
3000659a:	2200      	movs	r2, #0
3000659c:	1d21      	adds	r1, r4, #4
3000659e:	2003      	movs	r0, #3
300065a0:	f001 fa6e 	bl	30007a80 <flash_get_layout_info>
300065a4:	2200      	movs	r2, #0
300065a6:	f104 0110 	add.w	r1, r4, #16
300065aa:	2004      	movs	r0, #4
300065ac:	f001 fa68 	bl	30007a80 <flash_get_layout_info>
300065b0:	2200      	movs	r2, #0
300065b2:	f104 0114 	add.w	r1, r4, #20
300065b6:	2005      	movs	r0, #5
300065b8:	f001 fa62 	bl	30007a80 <flash_get_layout_info>
300065bc:	e9d4 2300 	ldrd	r2, r3, [r4]
300065c0:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
300065c4:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
300065c8:	e9c4 2302 	strd	r2, r3, [r4, #8]
300065cc:	bd10      	pop	{r4, pc}
300065ce:	bf00      	nop
300065d0:	3000f140 	.word	0x3000f140

300065d4 <BOOT_OTA_IMG2>:
300065d4:	f7ff beca 	b.w	3000636c <BOOT_OTA_LoadIMG2>

300065d8 <BOOT_OTA_AP>:
300065d8:	b5f0      	push	{r4, r5, r6, r7, lr}
300065da:	4a32      	ldr	r2, [pc, #200]	; (300066a4 <BOOT_OTA_AP+0xcc>)
300065dc:	b0eb      	sub	sp, #428	; 0x1ac
300065de:	4b32      	ldr	r3, [pc, #200]	; (300066a8 <BOOT_OTA_AP+0xd0>)
300065e0:	6815      	ldr	r5, [r2, #0]
300065e2:	ac02      	add	r4, sp, #8
300065e4:	4e31      	ldr	r6, [pc, #196]	; (300066ac <BOOT_OTA_AP+0xd4>)
300065e6:	f505 5780 	add.w	r7, r5, #4096	; 0x1000
300065ea:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300065ec:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
300065f0:	4629      	mov	r1, r5
300065f2:	4d2f      	ldr	r5, [pc, #188]	; (300066b0 <BOOT_OTA_AP+0xd8>)
300065f4:	f44f 72c8 	mov.w	r2, #400	; 0x190
300065f8:	a806      	add	r0, sp, #24
300065fa:	f7fe f91f 	bl	3000483c <BOOT_ImgCopy>
300065fe:	4a2d      	ldr	r2, [pc, #180]	; (300066b4 <BOOT_OTA_AP+0xdc>)
30006600:	463b      	mov	r3, r7
30006602:	4629      	mov	r1, r5
30006604:	2003      	movs	r0, #3
30006606:	47b0      	blx	r6
30006608:	2101      	movs	r1, #1
3000660a:	4b2b      	ldr	r3, [pc, #172]	; (300066b8 <BOOT_OTA_AP+0xe0>)
3000660c:	2003      	movs	r0, #3
3000660e:	4798      	blx	r3
30006610:	2201      	movs	r2, #1
30006612:	4928      	ldr	r1, [pc, #160]	; (300066b4 <BOOT_OTA_AP+0xdc>)
30006614:	2303      	movs	r3, #3
30006616:	4628      	mov	r0, r5
30006618:	f7ff fe5e 	bl	300062d8 <BOOT_OTFCheck>
3000661c:	4b27      	ldr	r3, [pc, #156]	; (300066bc <BOOT_OTA_AP+0xe4>)
3000661e:	4798      	blx	r3
30006620:	4623      	mov	r3, r4
30006622:	2201      	movs	r2, #1
30006624:	4c26      	ldr	r4, [pc, #152]	; (300066c0 <BOOT_OTA_AP+0xe8>)
30006626:	2800      	cmp	r0, #0
30006628:	bf14      	ite	ne
3000662a:	4629      	movne	r1, r5
3000662c:	4639      	moveq	r1, r7
3000662e:	9200      	str	r2, [sp, #0]
30006630:	4620      	mov	r0, r4
30006632:	2204      	movs	r2, #4
30006634:	f7ff fbfe 	bl	30005e34 <BOOT_LoadSubImage>
30006638:	2801      	cmp	r0, #1
3000663a:	d122      	bne.n	30006682 <BOOT_OTA_AP+0xaa>
3000663c:	2302      	movs	r3, #2
3000663e:	4605      	mov	r5, r0
30006640:	2204      	movs	r2, #4
30006642:	a806      	add	r0, sp, #24
30006644:	9300      	str	r3, [sp, #0]
30006646:	4621      	mov	r1, r4
30006648:	4b1e      	ldr	r3, [pc, #120]	; (300066c4 <BOOT_OTA_AP+0xec>)
3000664a:	f000 facd 	bl	30006be8 <BOOT_SignatureCheck>
3000664e:	4606      	mov	r6, r0
30006650:	bb18      	cbnz	r0, 3000669a <BOOT_OTA_AP+0xc2>
30006652:	68e3      	ldr	r3, [r4, #12]
30006654:	462a      	mov	r2, r5
30006656:	6861      	ldr	r1, [r4, #4]
30006658:	f104 0018 	add.w	r0, r4, #24
3000665c:	4419      	add	r1, r3
3000665e:	6963      	ldr	r3, [r4, #20]
30006660:	4419      	add	r1, r3
30006662:	4439      	add	r1, r7
30006664:	f7ff fc9a 	bl	30005f9c <BOOT_LoadAPSubImage>
30006668:	2801      	cmp	r0, #1
3000666a:	d102      	bne.n	30006672 <BOOT_OTA_AP+0x9a>
3000666c:	4630      	mov	r0, r6
3000666e:	b06b      	add	sp, #428	; 0x1ac
30006670:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006672:	4b15      	ldr	r3, [pc, #84]	; (300066c8 <BOOT_OTA_AP+0xf0>)
30006674:	681b      	ldr	r3, [r3, #0]
30006676:	079b      	lsls	r3, r3, #30
30006678:	d40a      	bmi.n	30006690 <BOOT_OTA_AP+0xb8>
3000667a:	26ff      	movs	r6, #255	; 0xff
3000667c:	4630      	mov	r0, r6
3000667e:	b06b      	add	sp, #428	; 0x1ac
30006680:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006682:	26ff      	movs	r6, #255	; 0xff
30006684:	4811      	ldr	r0, [pc, #68]	; (300066cc <BOOT_OTA_AP+0xf4>)
30006686:	f005 f86f 	bl	3000b768 <__DiagPrintf_veneer>
3000668a:	4630      	mov	r0, r6
3000668c:	b06b      	add	sp, #428	; 0x1ac
3000668e:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006690:	480f      	ldr	r0, [pc, #60]	; (300066d0 <BOOT_OTA_AP+0xf8>)
30006692:	26ff      	movs	r6, #255	; 0xff
30006694:	f005 f868 	bl	3000b768 <__DiagPrintf_veneer>
30006698:	e7e8      	b.n	3000666c <BOOT_OTA_AP+0x94>
3000669a:	480e      	ldr	r0, [pc, #56]	; (300066d4 <BOOT_OTA_AP+0xfc>)
3000669c:	26ff      	movs	r6, #255	; 0xff
3000669e:	f005 f863 	bl	3000b768 <__DiagPrintf_veneer>
300066a2:	e7e3      	b.n	3000666c <BOOT_OTA_AP+0x94>
300066a4:	3000df0c 	.word	0x3000df0c
300066a8:	3000d62c 	.word	0x3000d62c
300066ac:	30008d15 	.word	0x30008d15
300066b0:	0e000000 	.word	0x0e000000
300066b4:	0fffffe0 	.word	0x0fffffe0
300066b8:	30008d55 	.word	0x30008d55
300066bc:	30008ef5 	.word	0x30008ef5
300066c0:	3000f158 	.word	0x3000f158
300066c4:	3000ee4c 	.word	0x3000ee4c
300066c8:	2001c00c 	.word	0x2001c00c
300066cc:	3000c314 	.word	0x3000c314
300066d0:	3000c338 	.word	0x3000c338
300066d4:	3000c2f0 	.word	0x3000c2f0

300066d8 <BOOT_LoadRDPImg>:
300066d8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300066dc:	4d4e      	ldr	r5, [pc, #312]	; (30006818 <BOOT_LoadRDPImg+0x140>)
300066de:	b0d9      	sub	sp, #356	; 0x164
300066e0:	4613      	mov	r3, r2
300066e2:	468b      	mov	fp, r1
300066e4:	4604      	mov	r4, r0
300066e6:	aa06      	add	r2, sp, #24
300066e8:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
300066ec:	e895 0003 	ldmia.w	r5, {r0, r1}
300066f0:	2500      	movs	r5, #0
300066f2:	e882 0003 	stmia.w	r2, {r0, r1}
300066f6:	4619      	mov	r1, r3
300066f8:	f44f 72c8 	mov.w	r2, #400	; 0x190
300066fc:	4620      	mov	r0, r4
300066fe:	f7fe f89d 	bl	3000483c <BOOT_ImgCopy>
30006702:	ab06      	add	r3, sp, #24
30006704:	2202      	movs	r2, #2
30006706:	4641      	mov	r1, r8
30006708:	4658      	mov	r0, fp
3000670a:	9500      	str	r5, [sp, #0]
3000670c:	f7ff fb92 	bl	30005e34 <BOOT_LoadSubImage>
30006710:	2801      	cmp	r0, #1
30006712:	d06f      	beq.n	300067f4 <BOOT_LoadRDPImg+0x11c>
30006714:	4b41      	ldr	r3, [pc, #260]	; (3000681c <BOOT_LoadRDPImg+0x144>)
30006716:	4628      	mov	r0, r5
30006718:	4798      	blx	r3
3000671a:	2101      	movs	r1, #1
3000671c:	4b40      	ldr	r3, [pc, #256]	; (30006820 <BOOT_LoadRDPImg+0x148>)
3000671e:	4628      	mov	r0, r5
30006720:	4798      	blx	r3
30006722:	4b40      	ldr	r3, [pc, #256]	; (30006824 <BOOT_LoadRDPImg+0x14c>)
30006724:	2120      	movs	r1, #32
30006726:	a810      	add	r0, sp, #64	; 0x40
30006728:	4798      	blx	r3
3000672a:	f8df a110 	ldr.w	sl, [pc, #272]	; 3000683c <BOOT_LoadRDPImg+0x164>
3000672e:	f104 0120 	add.w	r1, r4, #32
30006732:	2210      	movs	r2, #16
30006734:	a80c      	add	r0, sp, #48	; 0x30
30006736:	af08      	add	r7, sp, #32
30006738:	47d0      	blx	sl
3000673a:	f8df 9104 	ldr.w	r9, [pc, #260]	; 30006840 <BOOT_LoadRDPImg+0x168>
3000673e:	ab06      	add	r3, sp, #24
30006740:	9504      	str	r5, [sp, #16]
30006742:	9305      	str	r3, [sp, #20]
30006744:	4641      	mov	r1, r8
30006746:	a818      	add	r0, sp, #96	; 0x60
30006748:	2210      	movs	r2, #16
3000674a:	f7fe f877 	bl	3000483c <BOOT_ImgCopy>
3000674e:	2310      	movs	r3, #16
30006750:	aa0c      	add	r2, sp, #48	; 0x30
30006752:	a818      	add	r0, sp, #96	; 0x60
30006754:	4619      	mov	r1, r3
30006756:	9700      	str	r7, [sp, #0]
30006758:	47c8      	blx	r9
3000675a:	683b      	ldr	r3, [r7, #0]
3000675c:	4a32      	ldr	r2, [pc, #200]	; (30006828 <BOOT_LoadRDPImg+0x150>)
3000675e:	4293      	cmp	r3, r2
30006760:	d14c      	bne.n	300067fc <BOOT_LoadRDPImg+0x124>
30006762:	9a05      	ldr	r2, [sp, #20]
30006764:	687b      	ldr	r3, [r7, #4]
30006766:	f852 1b04 	ldr.w	r1, [r2], #4
3000676a:	9205      	str	r2, [sp, #20]
3000676c:	4a2f      	ldr	r2, [pc, #188]	; (3000682c <BOOT_LoadRDPImg+0x154>)
3000676e:	4293      	cmp	r3, r2
30006770:	d148      	bne.n	30006804 <BOOT_LoadRDPImg+0x12c>
30006772:	9100      	str	r1, [sp, #0]
30006774:	2004      	movs	r0, #4
30006776:	68fe      	ldr	r6, [r7, #12]
30006778:	4b2d      	ldr	r3, [pc, #180]	; (30006830 <BOOT_LoadRDPImg+0x158>)
3000677a:	3e20      	subs	r6, #32
3000677c:	4a2d      	ldr	r2, [pc, #180]	; (30006834 <BOOT_LoadRDPImg+0x15c>)
3000677e:	4619      	mov	r1, r3
30006780:	9601      	str	r6, [sp, #4]
30006782:	68bd      	ldr	r5, [r7, #8]
30006784:	3520      	adds	r5, #32
30006786:	9502      	str	r5, [sp, #8]
30006788:	f004 fd50 	bl	3000b22c <rtk_log_write>
3000678c:	f1bb 0f00 	cmp.w	fp, #0
30006790:	d006      	beq.n	300067a0 <BOOT_LoadRDPImg+0xc8>
30006792:	9a04      	ldr	r2, [sp, #16]
30006794:	f10b 0304 	add.w	r3, fp, #4
30006798:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
3000679c:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
300067a0:	b1dd      	cbz	r5, 300067da <BOOT_LoadRDPImg+0x102>
300067a2:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
300067a6:	462c      	mov	r4, r5
300067a8:	4641      	mov	r1, r8
300067aa:	a818      	add	r0, sp, #96	; 0x60
300067ac:	bf28      	it	cs
300067ae:	f44f 7480 	movcs.w	r4, #256	; 0x100
300067b2:	4622      	mov	r2, r4
300067b4:	44a0      	add	r8, r4
300067b6:	f7fe f841 	bl	3000483c <BOOT_ImgCopy>
300067ba:	aa0c      	add	r2, sp, #48	; 0x30
300067bc:	4621      	mov	r1, r4
300067be:	2310      	movs	r3, #16
300067c0:	9600      	str	r6, [sp, #0]
300067c2:	a818      	add	r0, sp, #96	; 0x60
300067c4:	47c8      	blx	r9
300067c6:	ab18      	add	r3, sp, #96	; 0x60
300067c8:	f1a4 0110 	sub.w	r1, r4, #16
300067cc:	2210      	movs	r2, #16
300067ce:	a80c      	add	r0, sp, #48	; 0x30
300067d0:	4426      	add	r6, r4
300067d2:	4419      	add	r1, r3
300067d4:	47d0      	blx	sl
300067d6:	1b2d      	subs	r5, r5, r4
300067d8:	d1e3      	bne.n	300067a2 <BOOT_LoadRDPImg+0xca>
300067da:	9b04      	ldr	r3, [sp, #16]
300067dc:	b13b      	cbz	r3, 300067ee <BOOT_LoadRDPImg+0x116>
300067de:	2100      	movs	r1, #0
300067e0:	4b0f      	ldr	r3, [pc, #60]	; (30006820 <BOOT_LoadRDPImg+0x148>)
300067e2:	4608      	mov	r0, r1
300067e4:	4798      	blx	r3
300067e6:	2002      	movs	r0, #2
300067e8:	b059      	add	sp, #356	; 0x164
300067ea:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300067ee:	2301      	movs	r3, #1
300067f0:	9304      	str	r3, [sp, #16]
300067f2:	e7a7      	b.n	30006744 <BOOT_LoadRDPImg+0x6c>
300067f4:	2002      	movs	r0, #2
300067f6:	b059      	add	sp, #356	; 0x164
300067f8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300067fc:	9b04      	ldr	r3, [sp, #16]
300067fe:	aa06      	add	r2, sp, #24
30006800:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30006804:	4b0a      	ldr	r3, [pc, #40]	; (30006830 <BOOT_LoadRDPImg+0x158>)
30006806:	2004      	movs	r0, #4
30006808:	9100      	str	r1, [sp, #0]
3000680a:	4a0b      	ldr	r2, [pc, #44]	; (30006838 <BOOT_LoadRDPImg+0x160>)
3000680c:	4619      	mov	r1, r3
3000680e:	f004 fd0d 	bl	3000b22c <rtk_log_write>
30006812:	2000      	movs	r0, #0
30006814:	e7e8      	b.n	300067e8 <BOOT_LoadRDPImg+0x110>
30006816:	bf00      	nop
30006818:	3000d63c 	.word	0x3000d63c
3000681c:	00002429 	.word	0x00002429
30006820:	00002479 	.word	0x00002479
30006824:	00004641 	.word	0x00004641
30006828:	35393138 	.word	0x35393138
3000682c:	31313738 	.word	0x31313738
30006830:	3000b9d0 	.word	0x3000b9d0
30006834:	3000c1e0 	.word	0x3000c1e0
30006838:	3000c1cc 	.word	0x3000c1cc
3000683c:	00012a1d 	.word	0x00012a1d
30006840:	000046f5 	.word	0x000046f5

30006844 <BOOT_LoadBL32RDPImg>:
30006844:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006848:	4b35      	ldr	r3, [pc, #212]	; (30006920 <BOOT_LoadBL32RDPImg+0xdc>)
3000684a:	b0d1      	sub	sp, #324	; 0x144
3000684c:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 3000693c <BOOT_LoadBL32RDPImg+0xf8>
30006850:	4607      	mov	r7, r0
30006852:	f10d 0810 	add.w	r8, sp, #16
30006856:	460e      	mov	r6, r1
30006858:	4615      	mov	r5, r2
3000685a:	ac02      	add	r4, sp, #8
3000685c:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30006940 <BOOT_LoadBL32RDPImg+0xfc>
30006860:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 30006944 <BOOT_LoadBL32RDPImg+0x100>
30006864:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006866:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
3000686a:	e89c 0003 	ldmia.w	ip, {r0, r1}
3000686e:	e884 0003 	stmia.w	r4, {r0, r1}
30006872:	2000      	movs	r0, #0
30006874:	47c8      	blx	r9
30006876:	2101      	movs	r1, #1
30006878:	2000      	movs	r0, #0
3000687a:	47d0      	blx	sl
3000687c:	2120      	movs	r1, #32
3000687e:	4b29      	ldr	r3, [pc, #164]	; (30006924 <BOOT_LoadBL32RDPImg+0xe0>)
30006880:	eb0d 0001 	add.w	r0, sp, r1
30006884:	4798      	blx	r3
30006886:	4b28      	ldr	r3, [pc, #160]	; (30006928 <BOOT_LoadBL32RDPImg+0xe4>)
30006888:	689b      	ldr	r3, [r3, #8]
3000688a:	079b      	lsls	r3, r3, #30
3000688c:	d436      	bmi.n	300068fc <BOOT_LoadBL32RDPImg+0xb8>
3000688e:	b1fd      	cbz	r5, 300068d0 <BOOT_LoadBL32RDPImg+0x8c>
30006890:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 30006948 <BOOT_LoadBL32RDPImg+0x104>
30006894:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 3000694c <BOOT_LoadBL32RDPImg+0x108>
30006898:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
3000689c:	462c      	mov	r4, r5
3000689e:	4639      	mov	r1, r7
300068a0:	a810      	add	r0, sp, #64	; 0x40
300068a2:	bf28      	it	cs
300068a4:	f44f 7480 	movcs.w	r4, #256	; 0x100
300068a8:	4622      	mov	r2, r4
300068aa:	4427      	add	r7, r4
300068ac:	f7fd ffc6 	bl	3000483c <BOOT_ImgCopy>
300068b0:	4642      	mov	r2, r8
300068b2:	4621      	mov	r1, r4
300068b4:	2310      	movs	r3, #16
300068b6:	9600      	str	r6, [sp, #0]
300068b8:	a810      	add	r0, sp, #64	; 0x40
300068ba:	47d8      	blx	fp
300068bc:	ab10      	add	r3, sp, #64	; 0x40
300068be:	f1a4 0110 	sub.w	r1, r4, #16
300068c2:	2210      	movs	r2, #16
300068c4:	4640      	mov	r0, r8
300068c6:	4426      	add	r6, r4
300068c8:	4419      	add	r1, r3
300068ca:	47c8      	blx	r9
300068cc:	1b2d      	subs	r5, r5, r4
300068ce:	d1e3      	bne.n	30006898 <BOOT_LoadBL32RDPImg+0x54>
300068d0:	f856 2c20 	ldr.w	r2, [r6, #-32]
300068d4:	3e20      	subs	r6, #32
300068d6:	4b15      	ldr	r3, [pc, #84]	; (3000692c <BOOT_LoadBL32RDPImg+0xe8>)
300068d8:	429a      	cmp	r2, r3
300068da:	d108      	bne.n	300068ee <BOOT_LoadBL32RDPImg+0xaa>
300068dc:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
300068e0:	6872      	ldr	r2, [r6, #4]
300068e2:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
300068e6:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
300068ea:	429a      	cmp	r2, r3
300068ec:	d00d      	beq.n	3000690a <BOOT_LoadBL32RDPImg+0xc6>
300068ee:	4810      	ldr	r0, [pc, #64]	; (30006930 <BOOT_LoadBL32RDPImg+0xec>)
300068f0:	f004 ff3a 	bl	3000b768 <__DiagPrintf_veneer>
300068f4:	2000      	movs	r0, #0
300068f6:	b051      	add	sp, #324	; 0x144
300068f8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300068fc:	4621      	mov	r1, r4
300068fe:	462b      	mov	r3, r5
30006900:	4632      	mov	r2, r6
30006902:	480c      	ldr	r0, [pc, #48]	; (30006934 <BOOT_LoadBL32RDPImg+0xf0>)
30006904:	f004 ff30 	bl	3000b768 <__DiagPrintf_veneer>
30006908:	e7c1      	b.n	3000688e <BOOT_LoadBL32RDPImg+0x4a>
3000690a:	480b      	ldr	r0, [pc, #44]	; (30006938 <BOOT_LoadBL32RDPImg+0xf4>)
3000690c:	f004 ff2c 	bl	3000b768 <__DiagPrintf_veneer>
30006910:	2100      	movs	r1, #0
30006912:	4608      	mov	r0, r1
30006914:	47d0      	blx	sl
30006916:	2001      	movs	r0, #1
30006918:	b051      	add	sp, #324	; 0x144
3000691a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000691e:	bf00      	nop
30006920:	3000c3e0 	.word	0x3000c3e0
30006924:	00004641 	.word	0x00004641
30006928:	2001c00c 	.word	0x2001c00c
3000692c:	35393138 	.word	0x35393138
30006930:	3000c3c4 	.word	0x3000c3c4
30006934:	3000c39c 	.word	0x3000c39c
30006938:	3000c3d0 	.word	0x3000c3d0
3000693c:	3000c3f4 	.word	0x3000c3f4
30006940:	00002429 	.word	0x00002429
30006944:	00002479 	.word	0x00002479
30006948:	000046f5 	.word	0x000046f5
3000694c:	00012a1d 	.word	0x00012a1d

30006950 <_BOOT_Validate_ImgHash>:
30006950:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006954:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
30006958:	4605      	mov	r5, r0
3000695a:	4690      	mov	r8, r2
3000695c:	22e0      	movs	r2, #224	; 0xe0
3000695e:	9103      	str	r1, [sp, #12]
30006960:	2100      	movs	r1, #0
30006962:	a818      	add	r0, sp, #96	; 0x60
30006964:	461c      	mov	r4, r3
30006966:	f88d 101f 	strb.w	r1, [sp, #31]
3000696a:	f004 fed5 	bl	3000b718 <____wrap_memset_veneer>
3000696e:	4628      	mov	r0, r5
30006970:	f10d 021f 	add.w	r2, sp, #31
30006974:	f10d 011e 	add.w	r1, sp, #30
30006978:	f002 faf2 	bl	30008f60 <SBOOT_GetMdType>
3000697c:	9001      	str	r0, [sp, #4]
3000697e:	2800      	cmp	r0, #0
30006980:	d166      	bne.n	30006a50 <_BOOT_Validate_ImgHash+0x100>
30006982:	f89d 301f 	ldrb.w	r3, [sp, #31]
30006986:	2b00      	cmp	r3, #0
30006988:	d067      	beq.n	30006a5a <_BOOT_Validate_ImgHash+0x10a>
3000698a:	2101      	movs	r1, #1
3000698c:	4605      	mov	r5, r0
3000698e:	4b80      	ldr	r3, [pc, #512]	; (30006b90 <_BOOT_Validate_ImgHash+0x240>)
30006990:	4608      	mov	r0, r1
30006992:	4798      	blx	r3
30006994:	4629      	mov	r1, r5
30006996:	ab18      	add	r3, sp, #96	; 0x60
30006998:	2220      	movs	r2, #32
3000699a:	f89d 001e 	ldrb.w	r0, [sp, #30]
3000699e:	4d7d      	ldr	r5, [pc, #500]	; (30006b94 <_BOOT_Validate_ImgHash+0x244>)
300069a0:	47a8      	blx	r5
300069a2:	2800      	cmp	r0, #0
300069a4:	f040 808b 	bne.w	30006abe <_BOOT_Validate_ImgHash+0x16e>
300069a8:	b32c      	cbz	r4, 300069f6 <_BOOT_Validate_ImgHash+0xa6>
300069aa:	aad0      	add	r2, sp, #832	; 0x340
300069ac:	1e63      	subs	r3, r4, #1
300069ae:	ae50      	add	r6, sp, #320	; 0x140
300069b0:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 30006bac <_BOOT_Validate_ImgHash+0x25c>
300069b4:	f012 0218 	ands.w	r2, r2, #24
300069b8:	b2db      	uxtb	r3, r3
300069ba:	f026 091f 	bic.w	r9, r6, #31
300069be:	4d76      	ldr	r5, [pc, #472]	; (30006b98 <_BOOT_Validate_ImgHash+0x248>)
300069c0:	9204      	str	r2, [sp, #16]
300069c2:	f108 0208 	add.w	r2, r8, #8
300069c6:	bf08      	it	eq
300069c8:	46b1      	moveq	r9, r6
300069ca:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300069ce:	9302      	str	r3, [sp, #8]
300069d0:	e9d8 b700 	ldrd	fp, r7, [r8]
300069d4:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300069d8:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300069dc:	f0c0 80a7 	bcc.w	30006b2e <_BOOT_Validate_ImgHash+0x1de>
300069e0:	4639      	mov	r1, r7
300069e2:	4658      	mov	r0, fp
300069e4:	aa18      	add	r2, sp, #96	; 0x60
300069e6:	47d0      	blx	sl
300069e8:	2800      	cmp	r0, #0
300069ea:	d168      	bne.n	30006abe <_BOOT_Validate_ImgHash+0x16e>
300069ec:	f108 0808 	add.w	r8, r8, #8
300069f0:	9b02      	ldr	r3, [sp, #8]
300069f2:	4543      	cmp	r3, r8
300069f4:	d1ec      	bne.n	300069d0 <_BOOT_Validate_ImgHash+0x80>
300069f6:	ac08      	add	r4, sp, #32
300069f8:	a918      	add	r1, sp, #96	; 0x60
300069fa:	4b68      	ldr	r3, [pc, #416]	; (30006b9c <_BOOT_Validate_ImgHash+0x24c>)
300069fc:	4620      	mov	r0, r4
300069fe:	4798      	blx	r3
30006a00:	4601      	mov	r1, r0
30006a02:	2800      	cmp	r0, #0
30006a04:	d15b      	bne.n	30006abe <_BOOT_Validate_ImgHash+0x16e>
30006a06:	2001      	movs	r0, #1
30006a08:	4b61      	ldr	r3, [pc, #388]	; (30006b90 <_BOOT_Validate_ImgHash+0x240>)
30006a0a:	4798      	blx	r3
30006a0c:	f89d 501e 	ldrb.w	r5, [sp, #30]
30006a10:	4b63      	ldr	r3, [pc, #396]	; (30006ba0 <_BOOT_Validate_ImgHash+0x250>)
30006a12:	4798      	blx	r3
30006a14:	fbb0 f3f5 	udiv	r3, r0, r5
30006a18:	fb03 0315 	mls	r3, r3, r5, r0
30006a1c:	b2db      	uxtb	r3, r3
30006a1e:	b1bd      	cbz	r5, 30006a50 <_BOOT_Validate_ImgHash+0x100>
30006a20:	2000      	movs	r0, #0
30006a22:	9f03      	ldr	r7, [sp, #12]
30006a24:	4606      	mov	r6, r0
30006a26:	1c5a      	adds	r2, r3, #1
30006a28:	5cf9      	ldrb	r1, [r7, r3]
30006a2a:	5ce3      	ldrb	r3, [r4, r3]
30006a2c:	3001      	adds	r0, #1
30006a2e:	b2d2      	uxtb	r2, r2
30006a30:	404b      	eors	r3, r1
30006a32:	431e      	orrs	r6, r3
30006a34:	fbb2 f3f5 	udiv	r3, r2, r5
30006a38:	fb05 2213 	mls	r2, r5, r3, r2
30006a3c:	b2d3      	uxtb	r3, r2
30006a3e:	b2c2      	uxtb	r2, r0
30006a40:	4295      	cmp	r5, r2
30006a42:	d8f0      	bhi.n	30006a26 <_BOOT_Validate_ImgHash+0xd6>
30006a44:	2e00      	cmp	r6, #0
30006a46:	9b01      	ldr	r3, [sp, #4]
30006a48:	bf18      	it	ne
30006a4a:	f06f 0306 	mvnne.w	r3, #6
30006a4e:	9301      	str	r3, [sp, #4]
30006a50:	9801      	ldr	r0, [sp, #4]
30006a52:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30006a56:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006a5a:	a918      	add	r1, sp, #96	; 0x60
30006a5c:	f89d 001e 	ldrb.w	r0, [sp, #30]
30006a60:	4b50      	ldr	r3, [pc, #320]	; (30006ba4 <_BOOT_Validate_ImgHash+0x254>)
30006a62:	4798      	blx	r3
30006a64:	bb58      	cbnz	r0, 30006abe <_BOOT_Validate_ImgHash+0x16e>
30006a66:	b31c      	cbz	r4, 30006ab0 <_BOOT_Validate_ImgHash+0x160>
30006a68:	aad0      	add	r2, sp, #832	; 0x340
30006a6a:	1e63      	subs	r3, r4, #1
30006a6c:	ae50      	add	r6, sp, #320	; 0x140
30006a6e:	f8df a140 	ldr.w	sl, [pc, #320]	; 30006bb0 <_BOOT_Validate_ImgHash+0x260>
30006a72:	f012 0218 	ands.w	r2, r2, #24
30006a76:	b2db      	uxtb	r3, r3
30006a78:	f026 091f 	bic.w	r9, r6, #31
30006a7c:	4d46      	ldr	r5, [pc, #280]	; (30006b98 <_BOOT_Validate_ImgHash+0x248>)
30006a7e:	9204      	str	r2, [sp, #16]
30006a80:	f108 0208 	add.w	r2, r8, #8
30006a84:	bf08      	it	eq
30006a86:	46b1      	moveq	r9, r6
30006a88:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30006a8c:	9302      	str	r3, [sp, #8]
30006a8e:	e9d8 b700 	ldrd	fp, r7, [r8]
30006a92:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30006a96:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30006a9a:	d318      	bcc.n	30006ace <_BOOT_Validate_ImgHash+0x17e>
30006a9c:	4639      	mov	r1, r7
30006a9e:	4658      	mov	r0, fp
30006aa0:	aa18      	add	r2, sp, #96	; 0x60
30006aa2:	47d0      	blx	sl
30006aa4:	b958      	cbnz	r0, 30006abe <_BOOT_Validate_ImgHash+0x16e>
30006aa6:	f108 0808 	add.w	r8, r8, #8
30006aaa:	9b02      	ldr	r3, [sp, #8]
30006aac:	4543      	cmp	r3, r8
30006aae:	d1ee      	bne.n	30006a8e <_BOOT_Validate_ImgHash+0x13e>
30006ab0:	ac08      	add	r4, sp, #32
30006ab2:	a918      	add	r1, sp, #96	; 0x60
30006ab4:	4b3c      	ldr	r3, [pc, #240]	; (30006ba8 <_BOOT_Validate_ImgHash+0x258>)
30006ab6:	4620      	mov	r0, r4
30006ab8:	4798      	blx	r3
30006aba:	2800      	cmp	r0, #0
30006abc:	d0a6      	beq.n	30006a0c <_BOOT_Validate_ImgHash+0xbc>
30006abe:	f06f 030a 	mvn.w	r3, #10
30006ac2:	9301      	str	r3, [sp, #4]
30006ac4:	9801      	ldr	r0, [sp, #4]
30006ac6:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30006aca:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ace:	2f00      	cmp	r7, #0
30006ad0:	d0e9      	beq.n	30006aa6 <_BOOT_Validate_ImgHash+0x156>
30006ad2:	9c04      	ldr	r4, [sp, #16]
30006ad4:	f8cd 8014 	str.w	r8, [sp, #20]
30006ad8:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30006adc:	46b8      	mov	r8, r7
30006ade:	4659      	mov	r1, fp
30006ae0:	4630      	mov	r0, r6
30006ae2:	bf28      	it	cs
30006ae4:	f44f 7800 	movcs.w	r8, #512	; 0x200
30006ae8:	4642      	mov	r2, r8
30006aea:	f7fd fea7 	bl	3000483c <BOOT_ImgCopy>
30006aee:	696a      	ldr	r2, [r5, #20]
30006af0:	03d3      	lsls	r3, r2, #15
30006af2:	d50f      	bpl.n	30006b14 <_BOOT_Validate_ImgHash+0x1c4>
30006af4:	eb08 0004 	add.w	r0, r8, r4
30006af8:	f3bf 8f4f 	dsb	sy
30006afc:	4448      	add	r0, r9
30006afe:	464a      	mov	r2, r9
30006b00:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30006b04:	3220      	adds	r2, #32
30006b06:	1a83      	subs	r3, r0, r2
30006b08:	2b00      	cmp	r3, #0
30006b0a:	dcf9      	bgt.n	30006b00 <_BOOT_Validate_ImgHash+0x1b0>
30006b0c:	f3bf 8f4f 	dsb	sy
30006b10:	f3bf 8f6f 	isb	sy
30006b14:	aa18      	add	r2, sp, #96	; 0x60
30006b16:	4641      	mov	r1, r8
30006b18:	4630      	mov	r0, r6
30006b1a:	47d0      	blx	sl
30006b1c:	2800      	cmp	r0, #0
30006b1e:	d1ce      	bne.n	30006abe <_BOOT_Validate_ImgHash+0x16e>
30006b20:	ebb7 0708 	subs.w	r7, r7, r8
30006b24:	44c3      	add	fp, r8
30006b26:	d1d7      	bne.n	30006ad8 <_BOOT_Validate_ImgHash+0x188>
30006b28:	f8dd 8014 	ldr.w	r8, [sp, #20]
30006b2c:	e7bb      	b.n	30006aa6 <_BOOT_Validate_ImgHash+0x156>
30006b2e:	2f00      	cmp	r7, #0
30006b30:	f43f af5c 	beq.w	300069ec <_BOOT_Validate_ImgHash+0x9c>
30006b34:	9c04      	ldr	r4, [sp, #16]
30006b36:	f8cd 8014 	str.w	r8, [sp, #20]
30006b3a:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30006b3e:	46b8      	mov	r8, r7
30006b40:	4659      	mov	r1, fp
30006b42:	4630      	mov	r0, r6
30006b44:	bf28      	it	cs
30006b46:	f44f 7800 	movcs.w	r8, #512	; 0x200
30006b4a:	4642      	mov	r2, r8
30006b4c:	f7fd fe76 	bl	3000483c <BOOT_ImgCopy>
30006b50:	696a      	ldr	r2, [r5, #20]
30006b52:	03d2      	lsls	r2, r2, #15
30006b54:	d50f      	bpl.n	30006b76 <_BOOT_Validate_ImgHash+0x226>
30006b56:	eb08 0004 	add.w	r0, r8, r4
30006b5a:	f3bf 8f4f 	dsb	sy
30006b5e:	4448      	add	r0, r9
30006b60:	464a      	mov	r2, r9
30006b62:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30006b66:	3220      	adds	r2, #32
30006b68:	1a83      	subs	r3, r0, r2
30006b6a:	2b00      	cmp	r3, #0
30006b6c:	dcf9      	bgt.n	30006b62 <_BOOT_Validate_ImgHash+0x212>
30006b6e:	f3bf 8f4f 	dsb	sy
30006b72:	f3bf 8f6f 	isb	sy
30006b76:	aa18      	add	r2, sp, #96	; 0x60
30006b78:	4641      	mov	r1, r8
30006b7a:	4630      	mov	r0, r6
30006b7c:	47d0      	blx	sl
30006b7e:	2800      	cmp	r0, #0
30006b80:	d19d      	bne.n	30006abe <_BOOT_Validate_ImgHash+0x16e>
30006b82:	ebb7 0708 	subs.w	r7, r7, r8
30006b86:	44c3      	add	fp, r8
30006b88:	d1d7      	bne.n	30006b3a <_BOOT_Validate_ImgHash+0x1ea>
30006b8a:	f8dd 8014 	ldr.w	r8, [sp, #20]
30006b8e:	e72d      	b.n	300069ec <_BOOT_Validate_ImgHash+0x9c>
30006b90:	30008f9d 	.word	0x30008f9d
30006b94:	300088dd 	.word	0x300088dd
30006b98:	e000ed00 	.word	0xe000ed00
30006b9c:	30009019 	.word	0x30009019
30006ba0:	0000ea4d 	.word	0x0000ea4d
30006ba4:	30009059 	.word	0x30009059
30006ba8:	300090d5 	.word	0x300090d5
30006bac:	30008fdd 	.word	0x30008fdd
30006bb0:	30009099 	.word	0x30009099

30006bb4 <BOOT_Validate_ImgHash>:
30006bb4:	b530      	push	{r4, r5, lr}
30006bb6:	b085      	sub	sp, #20
30006bb8:	4d0a      	ldr	r5, [pc, #40]	; (30006be4 <BOOT_Validate_ImgHash+0x30>)
30006bba:	4604      	mov	r4, r0
30006bbc:	9301      	str	r3, [sp, #4]
30006bbe:	e9cd 2102 	strd	r2, r1, [sp, #8]
30006bc2:	47a8      	blx	r5
30006bc4:	9903      	ldr	r1, [sp, #12]
30006bc6:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30006bca:	b128      	cbz	r0, 30006bd8 <BOOT_Validate_ImgHash+0x24>
30006bcc:	4620      	mov	r0, r4
30006bce:	b005      	add	sp, #20
30006bd0:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30006bd4:	f002 bae2 	b.w	3000919c <SBOOT_Validate_ImgHash>
30006bd8:	4620      	mov	r0, r4
30006bda:	b005      	add	sp, #20
30006bdc:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30006be0:	f7ff beb6 	b.w	30006950 <_BOOT_Validate_ImgHash>
30006be4:	30008ef5 	.word	0x30008ef5

30006be8 <BOOT_SignatureCheck>:
30006be8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30006bec:	461e      	mov	r6, r3
30006bee:	b089      	sub	sp, #36	; 0x24
30006bf0:	4b79      	ldr	r3, [pc, #484]	; (30006dd8 <BOOT_SignatureCheck+0x1f0>)
30006bf2:	4680      	mov	r8, r0
30006bf4:	4689      	mov	r9, r1
30006bf6:	4617      	mov	r7, r2
30006bf8:	f10d 0c20 	add.w	ip, sp, #32
30006bfc:	4c77      	ldr	r4, [pc, #476]	; (30006ddc <BOOT_SignatureCheck+0x1f4>)
30006bfe:	9d10      	ldr	r5, [sp, #64]	; 0x40
30006c00:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006c02:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30006c06:	47a0      	blx	r4
30006c08:	b910      	cbnz	r0, 30006c10 <BOOT_SignatureCheck+0x28>
30006c0a:	b009      	add	sp, #36	; 0x24
30006c0c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006c10:	4b73      	ldr	r3, [pc, #460]	; (30006de0 <BOOT_SignatureCheck+0x1f8>)
30006c12:	2000      	movs	r0, #0
30006c14:	4798      	blx	r3
30006c16:	f898 3013 	ldrb.w	r3, [r8, #19]
30006c1a:	f898 2012 	ldrb.w	r2, [r8, #18]
30006c1e:	f10d 010f 	add.w	r1, sp, #15
30006c22:	f10d 000e 	add.w	r0, sp, #14
30006c26:	f002 faf3 	bl	30009210 <SBOOT_Validate_Algorithm>
30006c2a:	4604      	mov	r4, r0
30006c2c:	b9e8      	cbnz	r0, 30006c6a <BOOT_SignatureCheck+0x82>
30006c2e:	69f1      	ldr	r1, [r6, #28]
30006c30:	b919      	cbnz	r1, 30006c3a <BOOT_SignatureCheck+0x52>
30006c32:	e0cf      	b.n	30006dd4 <BOOT_SignatureCheck+0x1ec>
30006c34:	b2d0      	uxtb	r0, r2
30006c36:	4288      	cmp	r0, r1
30006c38:	d208      	bcs.n	30006c4c <BOOT_SignatureCheck+0x64>
30006c3a:	eb00 04c0 	add.w	r4, r0, r0, lsl #3
30006c3e:	1c42      	adds	r2, r0, #1
30006c40:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30006c44:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30006c48:	42ab      	cmp	r3, r5
30006c4a:	d1f3      	bne.n	30006c34 <BOOT_SignatureCheck+0x4c>
30006c4c:	2224      	movs	r2, #36	; 0x24
30006c4e:	23b4      	movs	r3, #180	; 0xb4
30006c50:	fb12 3200 	smlabb	r2, r2, r0, r3
30006c54:	f108 0570 	add.w	r5, r8, #112	; 0x70
30006c58:	4432      	add	r2, r6
30006c5a:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006c5e:	4629      	mov	r1, r5
30006c60:	f002 fa58 	bl	30009114 <SBOOT_Validate_PubKey>
30006c64:	4604      	mov	r4, r0
30006c66:	2800      	cmp	r0, #0
30006c68:	d07a      	beq.n	30006d60 <BOOT_SignatureCheck+0x178>
30006c6a:	f898 3011 	ldrb.w	r3, [r8, #17]
30006c6e:	aa08      	add	r2, sp, #32
30006c70:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30006c74:	f853 1c10 	ldr.w	r1, [r3, #-16]
30006c78:	4b5a      	ldr	r3, [pc, #360]	; (30006de4 <BOOT_SignatureCheck+0x1fc>)
30006c7a:	2002      	movs	r0, #2
30006c7c:	4a5a      	ldr	r2, [pc, #360]	; (30006de8 <BOOT_SignatureCheck+0x200>)
30006c7e:	e9cd 1400 	strd	r1, r4, [sp]
30006c82:	4619      	mov	r1, r3
30006c84:	f004 fad2 	bl	3000b22c <rtk_log_write>
30006c88:	2f00      	cmp	r7, #0
30006c8a:	d045      	beq.n	30006d18 <BOOT_SignatureCheck+0x130>
30006c8c:	464d      	mov	r5, r9
30006c8e:	f107 39ff 	add.w	r9, r7, #4294967295	; 0xffffffff
30006c92:	f04f 0800 	mov.w	r8, #0
30006c96:	4f55      	ldr	r7, [pc, #340]	; (30006dec <BOOT_SignatureCheck+0x204>)
30006c98:	fa5f f389 	uxtb.w	r3, r9
30006c9c:	f105 0908 	add.w	r9, r5, #8
30006ca0:	1d2e      	adds	r6, r5, #4
30006ca2:	4c53      	ldr	r4, [pc, #332]	; (30006df0 <BOOT_SignatureCheck+0x208>)
30006ca4:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30006ca8:	e003      	b.n	30006cb2 <BOOT_SignatureCheck+0xca>
30006caa:	3508      	adds	r5, #8
30006cac:	3608      	adds	r6, #8
30006cae:	45a9      	cmp	r9, r5
30006cb0:	d032      	beq.n	30006d18 <BOOT_SignatureCheck+0x130>
30006cb2:	f856 0c04 	ldr.w	r0, [r6, #-4]
30006cb6:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30006cba:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006cbe:	d3f4      	bcc.n	30006caa <BOOT_SignatureCheck+0xc2>
30006cc0:	2100      	movs	r1, #0
30006cc2:	686a      	ldr	r2, [r5, #4]
30006cc4:	47b8      	blx	r7
30006cc6:	6963      	ldr	r3, [r4, #20]
30006cc8:	03d9      	lsls	r1, r3, #15
30006cca:	d5ee      	bpl.n	30006caa <BOOT_SignatureCheck+0xc2>
30006ccc:	f856 2c04 	ldr.w	r2, [r6, #-4]
30006cd0:	686b      	ldr	r3, [r5, #4]
30006cd2:	ea02 0103 	and.w	r1, r2, r3
30006cd6:	3101      	adds	r1, #1
30006cd8:	d022      	beq.n	30006d20 <BOOT_SignatureCheck+0x138>
30006cda:	f012 011f 	ands.w	r1, r2, #31
30006cde:	d002      	beq.n	30006ce6 <BOOT_SignatureCheck+0xfe>
30006ce0:	440b      	add	r3, r1
30006ce2:	f022 021f 	bic.w	r2, r2, #31
30006ce6:	f3bf 8f4f 	dsb	sy
30006cea:	2b00      	cmp	r3, #0
30006cec:	dd0c      	ble.n	30006d08 <BOOT_SignatureCheck+0x120>
30006cee:	1e59      	subs	r1, r3, #1
30006cf0:	f102 0320 	add.w	r3, r2, #32
30006cf4:	f021 011f 	bic.w	r1, r1, #31
30006cf8:	4419      	add	r1, r3
30006cfa:	428b      	cmp	r3, r1
30006cfc:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30006d00:	461a      	mov	r2, r3
30006d02:	f103 0320 	add.w	r3, r3, #32
30006d06:	d1f8      	bne.n	30006cfa <BOOT_SignatureCheck+0x112>
30006d08:	f3bf 8f4f 	dsb	sy
30006d0c:	f3bf 8f6f 	isb	sy
30006d10:	3508      	adds	r5, #8
30006d12:	3608      	adds	r6, #8
30006d14:	45a9      	cmp	r9, r5
30006d16:	d1cc      	bne.n	30006cb2 <BOOT_SignatureCheck+0xca>
30006d18:	20ff      	movs	r0, #255	; 0xff
30006d1a:	b009      	add	sp, #36	; 0x24
30006d1c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006d20:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30006d24:	f3bf 8f4f 	dsb	sy
30006d28:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30006d2c:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30006d30:	f3c3 304e 	ubfx	r0, r3, #13, #15
30006d34:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30006d38:	0140      	lsls	r0, r0, #5
30006d3a:	ea00 010e 	and.w	r1, r0, lr
30006d3e:	4663      	mov	r3, ip
30006d40:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30006d44:	3b01      	subs	r3, #1
30006d46:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30006d4a:	1c5a      	adds	r2, r3, #1
30006d4c:	d1f8      	bne.n	30006d40 <BOOT_SignatureCheck+0x158>
30006d4e:	3820      	subs	r0, #32
30006d50:	f110 0f20 	cmn.w	r0, #32
30006d54:	d1f1      	bne.n	30006d3a <BOOT_SignatureCheck+0x152>
30006d56:	f3bf 8f4f 	dsb	sy
30006d5a:	f3bf 8f6f 	isb	sy
30006d5e:	e7d7      	b.n	30006d10 <BOOT_SignatureCheck+0x128>
30006d60:	f508 7380 	add.w	r3, r8, #256	; 0x100
30006d64:	462a      	mov	r2, r5
30006d66:	f89d 100f 	ldrb.w	r1, [sp, #15]
30006d6a:	9301      	str	r3, [sp, #4]
30006d6c:	f44f 7380 	mov.w	r3, #256	; 0x100
30006d70:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006d74:	9300      	str	r3, [sp, #0]
30006d76:	4643      	mov	r3, r8
30006d78:	f002 f9ea 	bl	30009150 <SBOOT_Validate_Signature>
30006d7c:	4604      	mov	r4, r0
30006d7e:	2800      	cmp	r0, #0
30006d80:	f47f af73 	bne.w	30006c6a <BOOT_SignatureCheck+0x82>
30006d84:	f108 0530 	add.w	r5, r8, #48	; 0x30
30006d88:	4b1a      	ldr	r3, [pc, #104]	; (30006df4 <BOOT_SignatureCheck+0x20c>)
30006d8a:	f89d 400f 	ldrb.w	r4, [sp, #15]
30006d8e:	4798      	blx	r3
30006d90:	4629      	mov	r1, r5
30006d92:	b9c0      	cbnz	r0, 30006dc6 <BOOT_SignatureCheck+0x1de>
30006d94:	4620      	mov	r0, r4
30006d96:	463b      	mov	r3, r7
30006d98:	464a      	mov	r2, r9
30006d9a:	f7ff fdd9 	bl	30006950 <_BOOT_Validate_ImgHash>
30006d9e:	4604      	mov	r4, r0
30006da0:	f898 3011 	ldrb.w	r3, [r8, #17]
30006da4:	aa08      	add	r2, sp, #32
30006da6:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30006daa:	f853 1c10 	ldr.w	r1, [r3, #-16]
30006dae:	2c00      	cmp	r4, #0
30006db0:	f47f af62 	bne.w	30006c78 <BOOT_SignatureCheck+0x90>
30006db4:	4b0b      	ldr	r3, [pc, #44]	; (30006de4 <BOOT_SignatureCheck+0x1fc>)
30006db6:	2004      	movs	r0, #4
30006db8:	9100      	str	r1, [sp, #0]
30006dba:	4a0f      	ldr	r2, [pc, #60]	; (30006df8 <BOOT_SignatureCheck+0x210>)
30006dbc:	4619      	mov	r1, r3
30006dbe:	f004 fa35 	bl	3000b22c <rtk_log_write>
30006dc2:	4620      	mov	r0, r4
30006dc4:	e721      	b.n	30006c0a <BOOT_SignatureCheck+0x22>
30006dc6:	4620      	mov	r0, r4
30006dc8:	463b      	mov	r3, r7
30006dca:	464a      	mov	r2, r9
30006dcc:	f002 f9e6 	bl	3000919c <SBOOT_Validate_ImgHash>
30006dd0:	4604      	mov	r4, r0
30006dd2:	e7e5      	b.n	30006da0 <BOOT_SignatureCheck+0x1b8>
30006dd4:	22b4      	movs	r2, #180	; 0xb4
30006dd6:	e73d      	b.n	30006c54 <BOOT_SignatureCheck+0x6c>
30006dd8:	3000d644 	.word	0x3000d644
30006ddc:	0000c149 	.word	0x0000c149
30006de0:	300091d9 	.word	0x300091d9
30006de4:	3000b9d0 	.word	0x3000b9d0
30006de8:	3000c414 	.word	0x3000c414
30006dec:	00012be5 	.word	0x00012be5
30006df0:	e000ed00 	.word	0xe000ed00
30006df4:	30008ef5 	.word	0x30008ef5
30006df8:	3000c3fc 	.word	0x3000c3fc

30006dfc <BOOT_CertificateCheck>:
30006dfc:	b570      	push	{r4, r5, r6, lr}
30006dfe:	4b2b      	ldr	r3, [pc, #172]	; (30006eac <BOOT_CertificateCheck+0xb0>)
30006e00:	b0b0      	sub	sp, #192	; 0xc0
30006e02:	4604      	mov	r4, r0
30006e04:	460d      	mov	r5, r1
30006e06:	4798      	blx	r3
30006e08:	b910      	cbnz	r0, 30006e10 <BOOT_CertificateCheck+0x14>
30006e0a:	2000      	movs	r0, #0
30006e0c:	b030      	add	sp, #192	; 0xc0
30006e0e:	bd70      	pop	{r4, r5, r6, pc}
30006e10:	69a1      	ldr	r1, [r4, #24]
30006e12:	2290      	movs	r2, #144	; 0x90
30006e14:	a80c      	add	r0, sp, #48	; 0x30
30006e16:	4429      	add	r1, r5
30006e18:	f7fd fd10 	bl	3000483c <BOOT_ImgCopy>
30006e1c:	4b24      	ldr	r3, [pc, #144]	; (30006eb0 <BOOT_CertificateCheck+0xb4>)
30006e1e:	4798      	blx	r3
30006e20:	4924      	ldr	r1, [pc, #144]	; (30006eb4 <BOOT_CertificateCheck+0xb8>)
30006e22:	f000 031f 	and.w	r3, r0, #31
30006e26:	2220      	movs	r2, #32
30006e28:	6808      	ldr	r0, [r1, #0]
30006e2a:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30006e2e:	a930      	add	r1, sp, #192	; 0xc0
30006e30:	3a01      	subs	r2, #1
30006e32:	f813 c000 	ldrb.w	ip, [r3, r0]
30006e36:	4419      	add	r1, r3
30006e38:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30006e3c:	f103 0301 	add.w	r3, r3, #1
30006e40:	f801 ccb0 	strb.w	ip, [r1, #-176]
30006e44:	f003 031f 	and.w	r3, r3, #31
30006e48:	d1f1      	bne.n	30006e2e <BOOT_CertificateCheck+0x32>
30006e4a:	4610      	mov	r0, r2
30006e4c:	4b1a      	ldr	r3, [pc, #104]	; (30006eb8 <BOOT_CertificateCheck+0xbc>)
30006e4e:	4798      	blx	r3
30006e50:	7ce3      	ldrb	r3, [r4, #19]
30006e52:	7ca2      	ldrb	r2, [r4, #18]
30006e54:	f10d 010f 	add.w	r1, sp, #15
30006e58:	f10d 000e 	add.w	r0, sp, #14
30006e5c:	f002 f9d8 	bl	30009210 <SBOOT_Validate_Algorithm>
30006e60:	4605      	mov	r5, r0
30006e62:	b9c0      	cbnz	r0, 30006e96 <BOOT_CertificateCheck+0x9a>
30006e64:	f104 0620 	add.w	r6, r4, #32
30006e68:	aa04      	add	r2, sp, #16
30006e6a:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006e6e:	4631      	mov	r1, r6
30006e70:	f002 f950 	bl	30009114 <SBOOT_Validate_PubKey>
30006e74:	4605      	mov	r5, r0
30006e76:	b970      	cbnz	r0, 30006e96 <BOOT_CertificateCheck+0x9a>
30006e78:	69a0      	ldr	r0, [r4, #24]
30006e7a:	4623      	mov	r3, r4
30006e7c:	ac0c      	add	r4, sp, #48	; 0x30
30006e7e:	4632      	mov	r2, r6
30006e80:	f89d 100f 	ldrb.w	r1, [sp, #15]
30006e84:	e9cd 0400 	strd	r0, r4, [sp]
30006e88:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006e8c:	f002 f960 	bl	30009150 <SBOOT_Validate_Signature>
30006e90:	4605      	mov	r5, r0
30006e92:	2800      	cmp	r0, #0
30006e94:	d0b9      	beq.n	30006e0a <BOOT_CertificateCheck+0xe>
30006e96:	4b09      	ldr	r3, [pc, #36]	; (30006ebc <BOOT_CertificateCheck+0xc0>)
30006e98:	2002      	movs	r0, #2
30006e9a:	4a09      	ldr	r2, [pc, #36]	; (30006ec0 <BOOT_CertificateCheck+0xc4>)
30006e9c:	4619      	mov	r1, r3
30006e9e:	9500      	str	r5, [sp, #0]
30006ea0:	f004 f9c4 	bl	3000b22c <rtk_log_write>
30006ea4:	20ff      	movs	r0, #255	; 0xff
30006ea6:	b030      	add	sp, #192	; 0xc0
30006ea8:	bd70      	pop	{r4, r5, r6, pc}
30006eaa:	bf00      	nop
30006eac:	0000c149 	.word	0x0000c149
30006eb0:	0000ea4d 	.word	0x0000ea4d
30006eb4:	3000d944 	.word	0x3000d944
30006eb8:	300091d9 	.word	0x300091d9
30006ebc:	3000b9d0 	.word	0x3000b9d0
30006ec0:	3000c438 	.word	0x3000c438

30006ec4 <BOOT_RAM_TZCfg>:
30006ec4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006ec8:	b085      	sub	sp, #20
30006eca:	4e58      	ldr	r6, [pc, #352]	; (3000702c <BOOT_RAM_TZCfg+0x168>)
30006ecc:	4958      	ldr	r1, [pc, #352]	; (30007030 <BOOT_RAM_TZCfg+0x16c>)
30006ece:	2501      	movs	r5, #1
30006ed0:	f10d 0e08 	add.w	lr, sp, #8
30006ed4:	4a57      	ldr	r2, [pc, #348]	; (30007034 <BOOT_RAM_TZCfg+0x170>)
30006ed6:	4b58      	ldr	r3, [pc, #352]	; (30007038 <BOOT_RAM_TZCfg+0x174>)
30006ed8:	46ec      	mov	ip, sp
30006eda:	4677      	mov	r7, lr
30006edc:	f8df 8178 	ldr.w	r8, [pc, #376]	; 30007058 <BOOT_RAM_TZCfg+0x194>
30006ee0:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
30006ee4:	f8df 9174 	ldr.w	r9, [pc, #372]	; 3000705c <BOOT_RAM_TZCfg+0x198>
30006ee8:	9202      	str	r2, [sp, #8]
30006eea:	9303      	str	r3, [sp, #12]
30006eec:	e9cd 6100 	strd	r6, r1, [sp]
30006ef0:	f85c 2b04 	ldr.w	r2, [ip], #4
30006ef4:	2300      	movs	r3, #0
30006ef6:	f85e 1b04 	ldr.w	r1, [lr], #4
30006efa:	680c      	ldr	r4, [r1, #0]
30006efc:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30006f00:	fa05 f003 	lsl.w	r0, r5, r3
30006f04:	f1b4 3fff 	cmp.w	r4, #4294967295	; 0xffffffff
30006f08:	d00c      	beq.n	30006f24 <BOOT_RAM_TZCfg+0x60>
30006f0a:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30006f0e:	3301      	adds	r3, #1
30006f10:	684c      	ldr	r4, [r1, #4]
30006f12:	310c      	adds	r1, #12
30006f14:	2b08      	cmp	r3, #8
30006f16:	f8cb 4004 	str.w	r4, [fp, #4]
30006f1a:	6c14      	ldr	r4, [r2, #64]	; 0x40
30006f1c:	ea40 0004 	orr.w	r0, r0, r4
30006f20:	6410      	str	r0, [r2, #64]	; 0x40
30006f22:	d1ea      	bne.n	30006efa <BOOT_RAM_TZCfg+0x36>
30006f24:	42b2      	cmp	r2, r6
30006f26:	d072      	beq.n	3000700e <BOOT_RAM_TZCfg+0x14a>
30006f28:	6455      	str	r5, [r2, #68]	; 0x44
30006f2a:	45bc      	cmp	ip, r7
30006f2c:	d1e0      	bne.n	30006ef0 <BOOT_RAM_TZCfg+0x2c>
30006f2e:	4b43      	ldr	r3, [pc, #268]	; (3000703c <BOOT_RAM_TZCfg+0x178>)
30006f30:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
30006f34:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30006f38:	4841      	ldr	r0, [pc, #260]	; (30007040 <BOOT_RAM_TZCfg+0x17c>)
30006f3a:	6019      	str	r1, [r3, #0]
30006f3c:	2501      	movs	r5, #1
30006f3e:	605a      	str	r2, [r3, #4]
30006f40:	2100      	movs	r1, #0
30006f42:	4a40      	ldr	r2, [pc, #256]	; (30007044 <BOOT_RAM_TZCfg+0x180>)
30006f44:	4c40      	ldr	r4, [pc, #256]	; (30007048 <BOOT_RAM_TZCfg+0x184>)
30006f46:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
30006f4a:	6813      	ldr	r3, [r2, #0]
30006f4c:	3301      	adds	r3, #1
30006f4e:	d019      	beq.n	30006f84 <BOOT_RAM_TZCfg+0xc0>
30006f50:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
30006f54:	320c      	adds	r2, #12
30006f56:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30006f5a:	3101      	adds	r1, #1
30006f5c:	f023 031f 	bic.w	r3, r3, #31
30006f60:	2908      	cmp	r1, #8
30006f62:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
30006f66:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30006f6a:	ea4f 0343 	mov.w	r3, r3, lsl #1
30006f6e:	f020 001f 	bic.w	r0, r0, #31
30006f72:	f003 0302 	and.w	r3, r3, #2
30006f76:	ea43 0300 	orr.w	r3, r3, r0
30006f7a:	f043 0301 	orr.w	r3, r3, #1
30006f7e:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
30006f82:	d1e2      	bne.n	30006f4a <BOOT_RAM_TZCfg+0x86>
30006f84:	4830      	ldr	r0, [pc, #192]	; (30007048 <BOOT_RAM_TZCfg+0x184>)
30006f86:	2201      	movs	r2, #1
30006f88:	4b30      	ldr	r3, [pc, #192]	; (3000704c <BOOT_RAM_TZCfg+0x188>)
30006f8a:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30006f8e:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30006f92:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30006f96:	d11e      	bne.n	30006fd6 <BOOT_RAM_TZCfg+0x112>
30006f98:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30006f9c:	f3bf 8f4f 	dsb	sy
30006fa0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30006fa4:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30006fa8:	f3c5 344e 	ubfx	r4, r5, #13, #15
30006fac:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30006fb0:	0164      	lsls	r4, r4, #5
30006fb2:	ea04 0106 	and.w	r1, r4, r6
30006fb6:	462b      	mov	r3, r5
30006fb8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30006fbc:	3b01      	subs	r3, #1
30006fbe:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30006fc2:	1c5a      	adds	r2, r3, #1
30006fc4:	d1f8      	bne.n	30006fb8 <BOOT_RAM_TZCfg+0xf4>
30006fc6:	3c20      	subs	r4, #32
30006fc8:	f114 0f20 	cmn.w	r4, #32
30006fcc:	d1f1      	bne.n	30006fb2 <BOOT_RAM_TZCfg+0xee>
30006fce:	f3bf 8f4f 	dsb	sy
30006fd2:	f3bf 8f6f 	isb	sy
30006fd6:	4b1c      	ldr	r3, [pc, #112]	; (30007048 <BOOT_RAM_TZCfg+0x184>)
30006fd8:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30006fdc:	491c      	ldr	r1, [pc, #112]	; (30007050 <BOOT_RAM_TZCfg+0x18c>)
30006fde:	691c      	ldr	r4, [r3, #16]
30006fe0:	4a1c      	ldr	r2, [pc, #112]	; (30007054 <BOOT_RAM_TZCfg+0x190>)
30006fe2:	f044 0408 	orr.w	r4, r4, #8
30006fe6:	611c      	str	r4, [r3, #16]
30006fe8:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
30006fec:	68d8      	ldr	r0, [r3, #12]
30006fee:	4028      	ands	r0, r5
30006ff0:	4301      	orrs	r1, r0
30006ff2:	60d9      	str	r1, [r3, #12]
30006ff4:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30006ff8:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30006ffc:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30007000:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30007004:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30007008:	b005      	add	sp, #20
3000700a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000700e:	f898 3000 	ldrb.w	r3, [r8]
30007012:	2b00      	cmp	r3, #0
30007014:	d189      	bne.n	30006f2a <BOOT_RAM_TZCfg+0x66>
30007016:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
3000701a:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
3000701e:	6c33      	ldr	r3, [r6, #64]	; 0x40
30007020:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30007024:	6433      	str	r3, [r6, #64]	; 0x40
30007026:	6475      	str	r5, [r6, #68]	; 0x44
30007028:	e77f      	b.n	30006f2a <BOOT_RAM_TZCfg+0x66>
3000702a:	bf00      	nop
3000702c:	51001a00 	.word	0x51001a00
30007030:	51001200 	.word	0x51001200
30007034:	3000da44 	.word	0x3000da44
30007038:	3000d9e4 	.word	0x3000d9e4
3000703c:	51001b00 	.word	0x51001b00
30007040:	500e0000 	.word	0x500e0000
30007044:	3000daa4 	.word	0x3000daa4
30007048:	e000ed00 	.word	0xe000ed00
3000704c:	42008000 	.word	0x42008000
30007050:	05fa6000 	.word	0x05fa6000
30007054:	e000e100 	.word	0xe000e100
30007058:	3000d93f 	.word	0x3000d93f
3000705c:	005fffff 	.word	0x005fffff

30007060 <ChipInfo_Invalid>:
30007060:	4e06      	ldr	r6, [pc, #24]	; (3000707c <ChipInfo_Invalid+0x1c>)
30007062:	4d07      	ldr	r5, [pc, #28]	; (30007080 <ChipInfo_Invalid+0x20>)
30007064:	4c07      	ldr	r4, [pc, #28]	; (30007084 <ChipInfo_Invalid+0x24>)
30007066:	b508      	push	{r3, lr}
30007068:	462b      	mov	r3, r5
3000706a:	4622      	mov	r2, r4
3000706c:	4904      	ldr	r1, [pc, #16]	; (30007080 <ChipInfo_Invalid+0x20>)
3000706e:	2002      	movs	r0, #2
30007070:	f004 f8dc 	bl	3000b22c <rtk_log_write>
30007074:	f242 7010 	movw	r0, #10000	; 0x2710
30007078:	47b0      	blx	r6
3000707a:	e7f5      	b.n	30007068 <ChipInfo_Invalid+0x8>
3000707c:	00009be5 	.word	0x00009be5
30007080:	3000c464 	.word	0x3000c464
30007084:	3000c470 	.word	0x3000c470

30007088 <ChipInfo_Get>:
30007088:	b570      	push	{r4, r5, r6, lr}
3000708a:	4c11      	ldr	r4, [pc, #68]	; (300070d0 <ChipInfo_Get+0x48>)
3000708c:	7820      	ldrb	r0, [r4, #0]
3000708e:	28ff      	cmp	r0, #255	; 0xff
30007090:	d104      	bne.n	3000709c <ChipInfo_Get+0x14>
30007092:	4d10      	ldr	r5, [pc, #64]	; (300070d4 <ChipInfo_Get+0x4c>)
30007094:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007098:	b108      	cbz	r0, 3000709e <ChipInfo_Get+0x16>
3000709a:	7020      	strb	r0, [r4, #0]
3000709c:	bd70      	pop	{r4, r5, r6, pc}
3000709e:	4621      	mov	r1, r4
300070a0:	f240 70ff 	movw	r0, #2047	; 0x7ff
300070a4:	f000 fec4 	bl	30007e30 <OTP_Read8>
300070a8:	7820      	ldrb	r0, [r4, #0]
300070aa:	28ff      	cmp	r0, #255	; 0xff
300070ac:	d002      	beq.n	300070b4 <ChipInfo_Get+0x2c>
300070ae:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
300070b2:	bd70      	pop	{r4, r5, r6, pc}
300070b4:	4e08      	ldr	r6, [pc, #32]	; (300070d8 <ChipInfo_Get+0x50>)
300070b6:	4d09      	ldr	r5, [pc, #36]	; (300070dc <ChipInfo_Get+0x54>)
300070b8:	4c09      	ldr	r4, [pc, #36]	; (300070e0 <ChipInfo_Get+0x58>)
300070ba:	462b      	mov	r3, r5
300070bc:	4622      	mov	r2, r4
300070be:	4907      	ldr	r1, [pc, #28]	; (300070dc <ChipInfo_Get+0x54>)
300070c0:	2003      	movs	r0, #3
300070c2:	f004 f8b3 	bl	3000b22c <rtk_log_write>
300070c6:	f242 7010 	movw	r0, #10000	; 0x2710
300070ca:	47b0      	blx	r6
300070cc:	e7f5      	b.n	300070ba <ChipInfo_Get+0x32>
300070ce:	bf00      	nop
300070d0:	3000e028 	.word	0x3000e028
300070d4:	23020000 	.word	0x23020000
300070d8:	00009be5 	.word	0x00009be5
300070dc:	3000c464 	.word	0x3000c464
300070e0:	3000c494 	.word	0x3000c494

300070e4 <ChipInfo_MemoryType>:
300070e4:	b510      	push	{r4, lr}
300070e6:	4c11      	ldr	r4, [pc, #68]	; (3000712c <ChipInfo_MemoryType+0x48>)
300070e8:	47a0      	blx	r4
300070ea:	2802      	cmp	r0, #2
300070ec:	d019      	beq.n	30007122 <ChipInfo_MemoryType+0x3e>
300070ee:	47a0      	blx	r4
300070f0:	2803      	cmp	r0, #3
300070f2:	d016      	beq.n	30007122 <ChipInfo_MemoryType+0x3e>
300070f4:	4c0e      	ldr	r4, [pc, #56]	; (30007130 <ChipInfo_MemoryType+0x4c>)
300070f6:	4b0f      	ldr	r3, [pc, #60]	; (30007134 <ChipInfo_MemoryType+0x50>)
300070f8:	4798      	blx	r3
300070fa:	2100      	movs	r1, #0
300070fc:	23e0      	movs	r3, #224	; 0xe0
300070fe:	4622      	mov	r2, r4
30007100:	e004      	b.n	3000710c <ChipInfo_MemoryType+0x28>
30007102:	7a13      	ldrb	r3, [r2, #8]
30007104:	3101      	adds	r1, #1
30007106:	3206      	adds	r2, #6
30007108:	2bff      	cmp	r3, #255	; 0xff
3000710a:	d00c      	beq.n	30007126 <ChipInfo_MemoryType+0x42>
3000710c:	4298      	cmp	r0, r3
3000710e:	d1f8      	bne.n	30007102 <ChipInfo_MemoryType+0x1e>
30007110:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007114:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007118:	78cb      	ldrb	r3, [r1, #3]
3000711a:	2b02      	cmp	r3, #2
3000711c:	d001      	beq.n	30007122 <ChipInfo_MemoryType+0x3e>
3000711e:	2001      	movs	r0, #1
30007120:	bd10      	pop	{r4, pc}
30007122:	2002      	movs	r0, #2
30007124:	bd10      	pop	{r4, pc}
30007126:	f7ff ff9b 	bl	30007060 <ChipInfo_Invalid>
3000712a:	bf00      	nop
3000712c:	0000c0f9 	.word	0x0000c0f9
30007130:	3000c4c0 	.word	0x3000c4c0
30007134:	30007089 	.word	0x30007089

30007138 <ChipInfo_DDRType>:
30007138:	b510      	push	{r4, lr}
3000713a:	4b0f      	ldr	r3, [pc, #60]	; (30007178 <ChipInfo_DDRType+0x40>)
3000713c:	4798      	blx	r3
3000713e:	4b0f      	ldr	r3, [pc, #60]	; (3000717c <ChipInfo_DDRType+0x44>)
30007140:	4604      	mov	r4, r0
30007142:	4798      	blx	r3
30007144:	2801      	cmp	r0, #1
30007146:	d112      	bne.n	3000716e <ChipInfo_DDRType+0x36>
30007148:	480d      	ldr	r0, [pc, #52]	; (30007180 <ChipInfo_DDRType+0x48>)
3000714a:	2100      	movs	r1, #0
3000714c:	22e0      	movs	r2, #224	; 0xe0
3000714e:	4603      	mov	r3, r0
30007150:	e003      	b.n	3000715a <ChipInfo_DDRType+0x22>
30007152:	789a      	ldrb	r2, [r3, #2]
30007154:	3101      	adds	r1, #1
30007156:	2aff      	cmp	r2, #255	; 0xff
30007158:	d00b      	beq.n	30007172 <ChipInfo_DDRType+0x3a>
3000715a:	4294      	cmp	r4, r2
3000715c:	f103 0306 	add.w	r3, r3, #6
30007160:	d1f7      	bne.n	30007152 <ChipInfo_DDRType+0x1a>
30007162:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007166:	eb00 0141 	add.w	r1, r0, r1, lsl #1
3000716a:	78c8      	ldrb	r0, [r1, #3]
3000716c:	bd10      	pop	{r4, pc}
3000716e:	20ff      	movs	r0, #255	; 0xff
30007170:	bd10      	pop	{r4, pc}
30007172:	2000      	movs	r0, #0
30007174:	bd10      	pop	{r4, pc}
30007176:	bf00      	nop
30007178:	30007089 	.word	0x30007089
3000717c:	300070e5 	.word	0x300070e5
30007180:	3000c4c0 	.word	0x3000c4c0

30007184 <ChipInfo_MemorySize>:
30007184:	b510      	push	{r4, lr}
30007186:	4c0b      	ldr	r4, [pc, #44]	; (300071b4 <ChipInfo_MemorySize+0x30>)
30007188:	4b0b      	ldr	r3, [pc, #44]	; (300071b8 <ChipInfo_MemorySize+0x34>)
3000718a:	4798      	blx	r3
3000718c:	2100      	movs	r1, #0
3000718e:	23e0      	movs	r3, #224	; 0xe0
30007190:	4622      	mov	r2, r4
30007192:	e004      	b.n	3000719e <ChipInfo_MemorySize+0x1a>
30007194:	7a13      	ldrb	r3, [r2, #8]
30007196:	3101      	adds	r1, #1
30007198:	3206      	adds	r2, #6
3000719a:	2bff      	cmp	r3, #255	; 0xff
3000719c:	d007      	beq.n	300071ae <ChipInfo_MemorySize+0x2a>
3000719e:	4298      	cmp	r0, r3
300071a0:	d1f8      	bne.n	30007194 <ChipInfo_MemorySize+0x10>
300071a2:	eb01 0141 	add.w	r1, r1, r1, lsl #1
300071a6:	eb04 0141 	add.w	r1, r4, r1, lsl #1
300071aa:	7908      	ldrb	r0, [r1, #4]
300071ac:	bd10      	pop	{r4, pc}
300071ae:	f7ff ff57 	bl	30007060 <ChipInfo_Invalid>
300071b2:	bf00      	nop
300071b4:	3000c4c0 	.word	0x3000c4c0
300071b8:	30007089 	.word	0x30007089

300071bc <ChipInfo_MemoryVendor>:
300071bc:	b510      	push	{r4, lr}
300071be:	4c0f      	ldr	r4, [pc, #60]	; (300071fc <ChipInfo_MemoryVendor+0x40>)
300071c0:	47a0      	blx	r4
300071c2:	2803      	cmp	r0, #3
300071c4:	d016      	beq.n	300071f4 <ChipInfo_MemoryVendor+0x38>
300071c6:	47a0      	blx	r4
300071c8:	2802      	cmp	r0, #2
300071ca:	d012      	beq.n	300071f2 <ChipInfo_MemoryVendor+0x36>
300071cc:	4c0c      	ldr	r4, [pc, #48]	; (30007200 <ChipInfo_MemoryVendor+0x44>)
300071ce:	4b0d      	ldr	r3, [pc, #52]	; (30007204 <ChipInfo_MemoryVendor+0x48>)
300071d0:	4798      	blx	r3
300071d2:	2100      	movs	r1, #0
300071d4:	23e0      	movs	r3, #224	; 0xe0
300071d6:	4622      	mov	r2, r4
300071d8:	e004      	b.n	300071e4 <ChipInfo_MemoryVendor+0x28>
300071da:	7a13      	ldrb	r3, [r2, #8]
300071dc:	3101      	adds	r1, #1
300071de:	3206      	adds	r2, #6
300071e0:	2bff      	cmp	r3, #255	; 0xff
300071e2:	d009      	beq.n	300071f8 <ChipInfo_MemoryVendor+0x3c>
300071e4:	4298      	cmp	r0, r3
300071e6:	d1f8      	bne.n	300071da <ChipInfo_MemoryVendor+0x1e>
300071e8:	eb01 0141 	add.w	r1, r1, r1, lsl #1
300071ec:	eb04 0141 	add.w	r1, r4, r1, lsl #1
300071f0:	7948      	ldrb	r0, [r1, #5]
300071f2:	bd10      	pop	{r4, pc}
300071f4:	2001      	movs	r0, #1
300071f6:	bd10      	pop	{r4, pc}
300071f8:	f7ff ff32 	bl	30007060 <ChipInfo_Invalid>
300071fc:	0000c0f9 	.word	0x0000c0f9
30007200:	3000c4c0 	.word	0x3000c4c0
30007204:	30007089 	.word	0x30007089

30007208 <SDM32K_Enable>:
30007208:	4a02      	ldr	r2, [pc, #8]	; (30007214 <SDM32K_Enable+0xc>)
3000720a:	4b03      	ldr	r3, [pc, #12]	; (30007218 <SDM32K_Enable+0x10>)
3000720c:	6811      	ldr	r1, [r2, #0]
3000720e:	430b      	orrs	r3, r1
30007210:	6013      	str	r3, [r2, #0]
30007212:	4770      	bx	lr
30007214:	42008e00 	.word	0x42008e00
30007218:	c0060000 	.word	0xc0060000

3000721c <CLK_SWITCH_XTAL>:
3000721c:	b508      	push	{r3, lr}
3000721e:	2801      	cmp	r0, #1
30007220:	4b0a      	ldr	r3, [pc, #40]	; (3000724c <CLK_SWITCH_XTAL+0x30>)
30007222:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007226:	d00d      	beq.n	30007244 <CLK_SWITCH_XTAL+0x28>
30007228:	4b09      	ldr	r3, [pc, #36]	; (30007250 <CLK_SWITCH_XTAL+0x34>)
3000722a:	4313      	orrs	r3, r2
3000722c:	4907      	ldr	r1, [pc, #28]	; (3000724c <CLK_SWITCH_XTAL+0x30>)
3000722e:	4a09      	ldr	r2, [pc, #36]	; (30007254 <CLK_SWITCH_XTAL+0x38>)
30007230:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007234:	4790      	blx	r2
30007236:	4a08      	ldr	r2, [pc, #32]	; (30007258 <CLK_SWITCH_XTAL+0x3c>)
30007238:	4b08      	ldr	r3, [pc, #32]	; (3000725c <CLK_SWITCH_XTAL+0x40>)
3000723a:	fba2 2000 	umull	r2, r0, r2, r0
3000723e:	0c80      	lsrs	r0, r0, #18
30007240:	6018      	str	r0, [r3, #0]
30007242:	bd08      	pop	{r3, pc}
30007244:	4b06      	ldr	r3, [pc, #24]	; (30007260 <CLK_SWITCH_XTAL+0x44>)
30007246:	4013      	ands	r3, r2
30007248:	e7f0      	b.n	3000722c <CLK_SWITCH_XTAL+0x10>
3000724a:	bf00      	nop
3000724c:	42008000 	.word	0x42008000
30007250:	80000500 	.word	0x80000500
30007254:	30008bc5 	.word	0x30008bc5
30007258:	431bde83 	.word	0x431bde83
3000725c:	2001c700 	.word	0x2001c700
30007260:	7ffffaff 	.word	0x7ffffaff

30007264 <DDR_PHY_ChipInfo.part.0>:
30007264:	b508      	push	{r3, lr}
30007266:	4b24      	ldr	r3, [pc, #144]	; (300072f8 <DDR_PHY_ChipInfo.part.0+0x94>)
30007268:	4798      	blx	r3
3000726a:	280e      	cmp	r0, #14
3000726c:	d80d      	bhi.n	3000728a <DDR_PHY_ChipInfo.part.0+0x26>
3000726e:	2801      	cmp	r0, #1
30007270:	d912      	bls.n	30007298 <DDR_PHY_ChipInfo.part.0+0x34>
30007272:	1e83      	subs	r3, r0, #2
30007274:	2b0c      	cmp	r3, #12
30007276:	d80f      	bhi.n	30007298 <DDR_PHY_ChipInfo.part.0+0x34>
30007278:	e8df f003 	tbb	[pc, r3]
3000727c:	210e1d19 	.word	0x210e1d19
30007280:	2a0e260e 	.word	0x2a0e260e
30007284:	340e0e2f 	.word	0x340e0e2f
30007288:	39          	.byte	0x39
30007289:	00          	.byte	0x00
3000728a:	28e0      	cmp	r0, #224	; 0xe0
3000728c:	d104      	bne.n	30007298 <DDR_PHY_ChipInfo.part.0+0x34>
3000728e:	2301      	movs	r3, #1
30007290:	4a1a      	ldr	r2, [pc, #104]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
30007292:	4618      	mov	r0, r3
30007294:	7013      	strb	r3, [r2, #0]
30007296:	bd08      	pop	{r3, pc}
30007298:	4b19      	ldr	r3, [pc, #100]	; (30007300 <DDR_PHY_ChipInfo.part.0+0x9c>)
3000729a:	2002      	movs	r0, #2
3000729c:	4a19      	ldr	r2, [pc, #100]	; (30007304 <DDR_PHY_ChipInfo.part.0+0xa0>)
3000729e:	4619      	mov	r1, r3
300072a0:	f003 ffc4 	bl	3000b22c <rtk_log_write>
300072a4:	2300      	movs	r3, #0
300072a6:	4a15      	ldr	r2, [pc, #84]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072a8:	4618      	mov	r0, r3
300072aa:	7013      	strb	r3, [r2, #0]
300072ac:	bd08      	pop	{r3, pc}
300072ae:	4b13      	ldr	r3, [pc, #76]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072b0:	2202      	movs	r2, #2
300072b2:	701a      	strb	r2, [r3, #0]
300072b4:	bd08      	pop	{r3, pc}
300072b6:	4b11      	ldr	r3, [pc, #68]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072b8:	2203      	movs	r2, #3
300072ba:	701a      	strb	r2, [r3, #0]
300072bc:	bd08      	pop	{r3, pc}
300072be:	2304      	movs	r3, #4
300072c0:	4a0e      	ldr	r2, [pc, #56]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072c2:	4618      	mov	r0, r3
300072c4:	7013      	strb	r3, [r2, #0]
300072c6:	bd08      	pop	{r3, pc}
300072c8:	4b0c      	ldr	r3, [pc, #48]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072ca:	2207      	movs	r2, #7
300072cc:	701a      	strb	r2, [r3, #0]
300072ce:	bd08      	pop	{r3, pc}
300072d0:	2305      	movs	r3, #5
300072d2:	4a0a      	ldr	r2, [pc, #40]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072d4:	4618      	mov	r0, r3
300072d6:	7013      	strb	r3, [r2, #0]
300072d8:	bd08      	pop	{r3, pc}
300072da:	2306      	movs	r3, #6
300072dc:	4a07      	ldr	r2, [pc, #28]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072de:	4618      	mov	r0, r3
300072e0:	7013      	strb	r3, [r2, #0]
300072e2:	bd08      	pop	{r3, pc}
300072e4:	2308      	movs	r3, #8
300072e6:	4a05      	ldr	r2, [pc, #20]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072e8:	4618      	mov	r0, r3
300072ea:	7013      	strb	r3, [r2, #0]
300072ec:	bd08      	pop	{r3, pc}
300072ee:	2309      	movs	r3, #9
300072f0:	4a02      	ldr	r2, [pc, #8]	; (300072fc <DDR_PHY_ChipInfo.part.0+0x98>)
300072f2:	4618      	mov	r0, r3
300072f4:	7013      	strb	r3, [r2, #0]
300072f6:	bd08      	pop	{r3, pc}
300072f8:	30007089 	.word	0x30007089
300072fc:	3000e029 	.word	0x3000e029
30007300:	3000c544 	.word	0x3000c544
30007304:	3000c54c 	.word	0x3000c54c

30007308 <DDR_PHY_ChipInfo_ddrtype>:
30007308:	b510      	push	{r4, lr}
3000730a:	4c04      	ldr	r4, [pc, #16]	; (3000731c <DDR_PHY_ChipInfo_ddrtype+0x14>)
3000730c:	7820      	ldrb	r0, [r4, #0]
3000730e:	b100      	cbz	r0, 30007312 <DDR_PHY_ChipInfo_ddrtype+0xa>
30007310:	bd10      	pop	{r4, pc}
30007312:	4b03      	ldr	r3, [pc, #12]	; (30007320 <DDR_PHY_ChipInfo_ddrtype+0x18>)
30007314:	4798      	blx	r3
30007316:	7020      	strb	r0, [r4, #0]
30007318:	bd10      	pop	{r4, pc}
3000731a:	bf00      	nop
3000731c:	3000ed30 	.word	0x3000ed30
30007320:	30007139 	.word	0x30007139

30007324 <DDR_PHY_StructInit>:
30007324:	f240 2315 	movw	r3, #533	; 0x215
30007328:	6583      	str	r3, [r0, #88]	; 0x58
3000732a:	4770      	bx	lr

3000732c <DDR_PHY_DLL_CLK_DIV>:
3000732c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
3000732e:	4d70      	ldr	r5, [pc, #448]	; (300074f0 <DDR_PHY_DLL_CLK_DIV+0x1c4>)
30007330:	4c70      	ldr	r4, [pc, #448]	; (300074f4 <DDR_PHY_DLL_CLK_DIV+0x1c8>)
30007332:	fba5 3200 	umull	r3, r2, r5, r0
30007336:	2365      	movs	r3, #101	; 0x65
30007338:	eb00 0e04 	add.w	lr, r0, r4
3000733c:	0952      	lsrs	r2, r2, #5
3000733e:	fb00 f303 	mul.w	r3, r0, r3
30007342:	486d      	ldr	r0, [pc, #436]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007344:	f1a2 0c03 	sub.w	ip, r2, #3
30007348:	6a47      	ldr	r7, [r0, #36]	; 0x24
3000734a:	eb0c 018c 	add.w	r1, ip, ip, lsl #2
3000734e:	ea4f 2c0c 	mov.w	ip, ip, lsl #8
30007352:	ebae 0ec1 	sub.w	lr, lr, r1, lsl #3
30007356:	fa1f fc8c 	uxth.w	ip, ip
3000735a:	ea4f 2ece 	mov.w	lr, lr, lsl #11
3000735e:	fba5 1e0e 	umull	r1, lr, r5, lr
30007362:	4966      	ldr	r1, [pc, #408]	; (300074fc <DDR_PHY_DLL_CLK_DIV+0x1d0>)
30007364:	fba1 6103 	umull	r6, r1, r1, r3
30007368:	4e65      	ldr	r6, [pc, #404]	; (30007500 <DDR_PHY_DLL_CLK_DIV+0x1d4>)
3000736a:	0a09      	lsrs	r1, r1, #8
3000736c:	1a52      	subs	r2, r2, r1
3000736e:	3903      	subs	r1, #3
30007370:	02d2      	lsls	r2, r2, #11
30007372:	eb02 125e 	add.w	r2, r2, lr, lsr #5
30007376:	ea06 2ece 	and.w	lr, r6, lr, lsl #11
3000737a:	4e62      	ldr	r6, [pc, #392]	; (30007504 <DDR_PHY_DLL_CLK_DIV+0x1d8>)
3000737c:	403e      	ands	r6, r7
3000737e:	ea4e 0606 	orr.w	r6, lr, r6
30007382:	6246      	str	r6, [r0, #36]	; 0x24
30007384:	4e60      	ldr	r6, [pc, #384]	; (30007508 <DDR_PHY_DLL_CLK_DIV+0x1dc>)
30007386:	fba6 6303 	umull	r6, r3, r6, r3
3000738a:	eb04 1353 	add.w	r3, r4, r3, lsr #5
3000738e:	6a84      	ldr	r4, [r0, #40]	; 0x28
30007390:	f424 447f 	bic.w	r4, r4, #65280	; 0xff00
30007394:	ea4c 0404 	orr.w	r4, ip, r4
30007398:	6284      	str	r4, [r0, #40]	; 0x28
3000739a:	eb01 0481 	add.w	r4, r1, r1, lsl #2
3000739e:	b2c9      	uxtb	r1, r1
300073a0:	eba3 03c4 	sub.w	r3, r3, r4, lsl #3
300073a4:	02db      	lsls	r3, r3, #11
300073a6:	fba5 4303 	umull	r4, r3, r5, r3
300073aa:	6a45      	ldr	r5, [r0, #36]	; 0x24
300073ac:	4c57      	ldr	r4, [pc, #348]	; (3000750c <DDR_PHY_DLL_CLK_DIV+0x1e0>)
300073ae:	eba2 1253 	sub.w	r2, r2, r3, lsr #5
300073b2:	f3c3 134a 	ubfx	r3, r3, #5, #11
300073b6:	402c      	ands	r4, r5
300073b8:	4d55      	ldr	r5, [pc, #340]	; (30007510 <DDR_PHY_DLL_CLK_DIV+0x1e4>)
300073ba:	eb02 1242 	add.w	r2, r2, r2, lsl #5
300073be:	4323      	orrs	r3, r4
300073c0:	02d2      	lsls	r2, r2, #11
300073c2:	6243      	str	r3, [r0, #36]	; 0x24
300073c4:	6a84      	ldr	r4, [r0, #40]	; 0x28
300073c6:	fba5 3202 	umull	r3, r2, r5, r2
300073ca:	4b52      	ldr	r3, [pc, #328]	; (30007514 <DDR_PHY_DLL_CLK_DIV+0x1e8>)
300073cc:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
300073d0:	4321      	orrs	r1, r4
300073d2:	2401      	movs	r4, #1
300073d4:	6281      	str	r1, [r0, #40]	; 0x28
300073d6:	6a01      	ldr	r1, [r0, #32]
300073d8:	400b      	ands	r3, r1
300073da:	494f      	ldr	r1, [pc, #316]	; (30007518 <DDR_PHY_DLL_CLK_DIV+0x1ec>)
300073dc:	ea43 3292 	orr.w	r2, r3, r2, lsr #14
300073e0:	6202      	str	r2, [r0, #32]
300073e2:	6784      	str	r4, [r0, #120]	; 0x78
300073e4:	7808      	ldrb	r0, [r1, #0]
300073e6:	28ff      	cmp	r0, #255	; 0xff
300073e8:	d075      	beq.n	300074d6 <DDR_PHY_DLL_CLK_DIV+0x1aa>
300073ea:	4c4c      	ldr	r4, [pc, #304]	; (3000751c <DDR_PHY_DLL_CLK_DIV+0x1f0>)
300073ec:	0145      	lsls	r5, r0, #5
300073ee:	4b42      	ldr	r3, [pc, #264]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300073f0:	eb04 1040 	add.w	r0, r4, r0, lsl #5
300073f4:	5962      	ldr	r2, [r4, r5]
300073f6:	4e4a      	ldr	r6, [pc, #296]	; (30007520 <DDR_PHY_DLL_CLK_DIV+0x1f4>)
300073f8:	611a      	str	r2, [r3, #16]
300073fa:	6842      	ldr	r2, [r0, #4]
300073fc:	615a      	str	r2, [r3, #20]
300073fe:	6882      	ldr	r2, [r0, #8]
30007400:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
30007404:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
30007408:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
3000740c:	609a      	str	r2, [r3, #8]
3000740e:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
30007412:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30007416:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
3000741a:	7830      	ldrb	r0, [r6, #0]
3000741c:	2800      	cmp	r0, #0
3000741e:	d056      	beq.n	300074ce <DDR_PHY_DLL_CLK_DIV+0x1a2>
30007420:	2801      	cmp	r0, #1
30007422:	442c      	add	r4, r5
30007424:	4b34      	ldr	r3, [pc, #208]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007426:	d037      	beq.n	30007498 <DDR_PHY_DLL_CLK_DIV+0x16c>
30007428:	2222      	movs	r2, #34	; 0x22
3000742a:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
3000742e:	6962      	ldr	r2, [r4, #20]
30007430:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
30007434:	e9d4 1206 	ldrd	r1, r2, [r4, #24]
30007438:	f8c3 1084 	str.w	r1, [r3, #132]	; 0x84
3000743c:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
30007440:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30007444:	4937      	ldr	r1, [pc, #220]	; (30007524 <DDR_PHY_DLL_CLK_DIV+0x1f8>)
30007446:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
3000744a:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000744e:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
30007452:	009b      	lsls	r3, r3, #2
30007454:	d516      	bpl.n	30007484 <DDR_PHY_DLL_CLK_DIV+0x158>
30007456:	2800      	cmp	r0, #0
30007458:	d044      	beq.n	300074e4 <DDR_PHY_DLL_CLK_DIV+0x1b8>
3000745a:	4b27      	ldr	r3, [pc, #156]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000745c:	4a32      	ldr	r2, [pc, #200]	; (30007528 <DDR_PHY_DLL_CLK_DIV+0x1fc>)
3000745e:	601a      	str	r2, [r3, #0]
30007460:	4b25      	ldr	r3, [pc, #148]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007462:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30007466:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
3000746a:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
3000746e:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30007472:	699a      	ldr	r2, [r3, #24]
30007474:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
30007478:	619a      	str	r2, [r3, #24]
3000747a:	699a      	ldr	r2, [r3, #24]
3000747c:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30007480:	619a      	str	r2, [r3, #24]
30007482:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007484:	b920      	cbnz	r0, 30007490 <DDR_PHY_DLL_CLK_DIV+0x164>
30007486:	4b29      	ldr	r3, [pc, #164]	; (3000752c <DDR_PHY_DLL_CLK_DIV+0x200>)
30007488:	4798      	blx	r3
3000748a:	2801      	cmp	r0, #1
3000748c:	7030      	strb	r0, [r6, #0]
3000748e:	d025      	beq.n	300074dc <DDR_PHY_DLL_CLK_DIV+0x1b0>
30007490:	4b19      	ldr	r3, [pc, #100]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007492:	4a27      	ldr	r2, [pc, #156]	; (30007530 <DDR_PHY_DLL_CLK_DIV+0x204>)
30007494:	601a      	str	r2, [r3, #0]
30007496:	e7e3      	b.n	30007460 <DDR_PHY_DLL_CLK_DIV+0x134>
30007498:	2233      	movs	r2, #51	; 0x33
3000749a:	6965      	ldr	r5, [r4, #20]
3000749c:	4921      	ldr	r1, [pc, #132]	; (30007524 <DDR_PHY_DLL_CLK_DIV+0x1f8>)
3000749e:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
300074a2:	f8c3 5080 	str.w	r5, [r3, #128]	; 0x80
300074a6:	e9d4 0206 	ldrd	r0, r2, [r4, #24]
300074aa:	f8c3 0084 	str.w	r0, [r3, #132]	; 0x84
300074ae:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
300074b2:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300074b6:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
300074ba:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300074be:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300074c2:	009a      	lsls	r2, r3, #2
300074c4:	d50a      	bpl.n	300074dc <DDR_PHY_DLL_CLK_DIV+0x1b0>
300074c6:	4b0c      	ldr	r3, [pc, #48]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300074c8:	4a1a      	ldr	r2, [pc, #104]	; (30007534 <DDR_PHY_DLL_CLK_DIV+0x208>)
300074ca:	601a      	str	r2, [r3, #0]
300074cc:	e7c8      	b.n	30007460 <DDR_PHY_DLL_CLK_DIV+0x134>
300074ce:	4b17      	ldr	r3, [pc, #92]	; (3000752c <DDR_PHY_DLL_CLK_DIV+0x200>)
300074d0:	4798      	blx	r3
300074d2:	7030      	strb	r0, [r6, #0]
300074d4:	e7a4      	b.n	30007420 <DDR_PHY_DLL_CLK_DIV+0xf4>
300074d6:	f7ff fec5 	bl	30007264 <DDR_PHY_ChipInfo.part.0>
300074da:	e786      	b.n	300073ea <DDR_PHY_DLL_CLK_DIV+0xbe>
300074dc:	4b06      	ldr	r3, [pc, #24]	; (300074f8 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300074de:	4a16      	ldr	r2, [pc, #88]	; (30007538 <DDR_PHY_DLL_CLK_DIV+0x20c>)
300074e0:	601a      	str	r2, [r3, #0]
300074e2:	e7bd      	b.n	30007460 <DDR_PHY_DLL_CLK_DIV+0x134>
300074e4:	4b11      	ldr	r3, [pc, #68]	; (3000752c <DDR_PHY_DLL_CLK_DIV+0x200>)
300074e6:	4798      	blx	r3
300074e8:	2801      	cmp	r0, #1
300074ea:	7030      	strb	r0, [r6, #0]
300074ec:	d1b5      	bne.n	3000745a <DDR_PHY_DLL_CLK_DIV+0x12e>
300074ee:	e7ea      	b.n	300074c6 <DDR_PHY_DLL_CLK_DIV+0x19a>
300074f0:	cccccccd 	.word	0xcccccccd
300074f4:	001fff88 	.word	0x001fff88
300074f8:	41011000 	.word	0x41011000
300074fc:	10624dd3 	.word	0x10624dd3
30007500:	07ff0000 	.word	0x07ff0000
30007504:	f800ffff 	.word	0xf800ffff
30007508:	51eb851f 	.word	0x51eb851f
3000750c:	fffff800 	.word	0xfffff800
30007510:	d1b71759 	.word	0xd1b71759
30007514:	fff80000 	.word	0xfff80000
30007518:	3000e029 	.word	0x3000e029
3000751c:	3000c56c 	.word	0x3000c56c
30007520:	3000ed30 	.word	0x3000ed30
30007524:	42008000 	.word	0x42008000
30007528:	4c000821 	.word	0x4c000821
3000752c:	30007139 	.word	0x30007139
30007530:	40000021 	.word	0x40000021
30007534:	4c000903 	.word	0x4c000903
30007538:	40000103 	.word	0x40000103

3000753c <DDR_PHY_CRT_Init>:
3000753c:	4a32      	ldr	r2, [pc, #200]	; (30007608 <DDR_PHY_CRT_Init+0xcc>)
3000753e:	2005      	movs	r0, #5
30007540:	b538      	push	{r3, r4, r5, lr}
30007542:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30007546:	4c31      	ldr	r4, [pc, #196]	; (3000760c <DDR_PHY_CRT_Init+0xd0>)
30007548:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000754c:	4d30      	ldr	r5, [pc, #192]	; (30007610 <DDR_PHY_CRT_Init+0xd4>)
3000754e:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30007552:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007556:	f043 0301 	orr.w	r3, r3, #1
3000755a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000755e:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007562:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30007566:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000756a:	68e3      	ldr	r3, [r4, #12]
3000756c:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30007570:	60e3      	str	r3, [r4, #12]
30007572:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007576:	f043 0302 	orr.w	r3, r3, #2
3000757a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000757e:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007582:	f023 0308 	bic.w	r3, r3, #8
30007586:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000758a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000758e:	f043 0304 	orr.w	r3, r3, #4
30007592:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007596:	47a8      	blx	r5
30007598:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000759c:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
300075a0:	f043 0308 	orr.w	r3, r3, #8
300075a4:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300075a8:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300075aa:	f043 6380 	orr.w	r3, r3, #67108864	; 0x4000000
300075ae:	62a3      	str	r3, [r4, #40]	; 0x28
300075b0:	47a8      	blx	r5
300075b2:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300075b4:	f641 73ff 	movw	r3, #8191	; 0x1fff
300075b8:	4916      	ldr	r1, [pc, #88]	; (30007614 <DDR_PHY_CRT_Init+0xd8>)
300075ba:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
300075be:	62a2      	str	r2, [r4, #40]	; 0x28
300075c0:	69e2      	ldr	r2, [r4, #28]
300075c2:	f042 0202 	orr.w	r2, r2, #2
300075c6:	61e2      	str	r2, [r4, #28]
300075c8:	69e2      	ldr	r2, [r4, #28]
300075ca:	f042 0208 	orr.w	r2, r2, #8
300075ce:	61e2      	str	r2, [r4, #28]
300075d0:	68a2      	ldr	r2, [r4, #8]
300075d2:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
300075d6:	60a2      	str	r2, [r4, #8]
300075d8:	6862      	ldr	r2, [r4, #4]
300075da:	4313      	orrs	r3, r2
300075dc:	6063      	str	r3, [r4, #4]
300075de:	68a3      	ldr	r3, [r4, #8]
300075e0:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
300075e4:	60a3      	str	r3, [r4, #8]
300075e6:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
300075ea:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300075ee:	f023 0303 	bic.w	r3, r3, #3
300075f2:	f043 0302 	orr.w	r3, r3, #2
300075f6:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300075fa:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300075fe:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30007602:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007606:	bd38      	pop	{r3, r4, r5, pc}
30007608:	42008000 	.word	0x42008000
3000760c:	41011000 	.word	0x41011000
30007610:	00009b2d 	.word	0x00009b2d
30007614:	90161f00 	.word	0x90161f00

30007618 <DDR_PHY_R240_ZQ_CAL>:
30007618:	b538      	push	{r3, r4, r5, lr}
3000761a:	4b1e      	ldr	r3, [pc, #120]	; (30007694 <DDR_PHY_R240_ZQ_CAL+0x7c>)
3000761c:	7818      	ldrb	r0, [r3, #0]
3000761e:	28ff      	cmp	r0, #255	; 0xff
30007620:	d034      	beq.n	3000768c <DDR_PHY_R240_ZQ_CAL+0x74>
30007622:	4a1d      	ldr	r2, [pc, #116]	; (30007698 <DDR_PHY_R240_ZQ_CAL+0x80>)
30007624:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30007628:	4b1c      	ldr	r3, [pc, #112]	; (3000769c <DDR_PHY_R240_ZQ_CAL+0x84>)
3000762a:	2400      	movs	r4, #0
3000762c:	eb02 0180 	add.w	r1, r2, r0, lsl #2
30007630:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
30007634:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
30007638:	7c0a      	ldrb	r2, [r1, #16]
3000763a:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
3000763e:	0052      	lsls	r2, r2, #1
30007640:	f002 023e 	and.w	r2, r2, #62	; 0x3e
30007644:	4302      	orrs	r2, r0
30007646:	6848      	ldr	r0, [r1, #4]
30007648:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
3000764c:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
30007650:	4a13      	ldr	r2, [pc, #76]	; (300076a0 <DDR_PHY_R240_ZQ_CAL+0x88>)
30007652:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
30007656:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
3000765a:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
3000765e:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
30007662:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
30007666:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
3000766a:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
3000766e:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30007672:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30007676:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
3000767a:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000767e:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30007682:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
30007686:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000768a:	bd38      	pop	{r3, r4, r5, pc}
3000768c:	f7ff fdea 	bl	30007264 <DDR_PHY_ChipInfo.part.0>
30007690:	e7c7      	b.n	30007622 <DDR_PHY_R240_ZQ_CAL+0xa>
30007692:	bf00      	nop
30007694:	3000e029 	.word	0x3000e029
30007698:	3000c6ac 	.word	0x3000c6ac
3000769c:	41011000 	.word	0x41011000
300076a0:	00262626 	.word	0x00262626

300076a4 <DDR_PHY_ZQ_SET_SEL>:
300076a4:	b410      	push	{r4}
300076a6:	4b25      	ldr	r3, [pc, #148]	; (3000773c <DDR_PHY_ZQ_SET_SEL+0x98>)
300076a8:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300076ac:	4a24      	ldr	r2, [pc, #144]	; (30007740 <DDR_PHY_ZQ_SET_SEL+0x9c>)
300076ae:	4c25      	ldr	r4, [pc, #148]	; (30007744 <DDR_PHY_ZQ_SET_SEL+0xa0>)
300076b0:	4925      	ldr	r1, [pc, #148]	; (30007748 <DDR_PHY_ZQ_SET_SEL+0xa4>)
300076b2:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
300076b6:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
300076ba:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
300076be:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
300076c2:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
300076c6:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
300076ca:	f64f 74ff 	movw	r4, #65535	; 0xffff
300076ce:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
300076d2:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
300076d6:	2200      	movs	r2, #0
300076d8:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
300076dc:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
300076e0:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
300076e4:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
300076e8:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
300076ec:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
300076f0:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
300076f4:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
300076f8:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
300076fc:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30007700:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30007704:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
30007708:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
3000770c:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30007710:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30007714:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
30007718:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
3000771c:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
30007720:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30007724:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
30007728:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
3000772c:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
30007730:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30007734:	f85d 4b04 	ldr.w	r4, [sp], #4
30007738:	4770      	bx	lr
3000773a:	bf00      	nop
3000773c:	41011000 	.word	0x41011000
30007740:	ffff1111 	.word	0xffff1111
30007744:	ffff3333 	.word	0xffff3333
30007748:	ffff0000 	.word	0xffff0000

3000774c <DDR_PHY_DELAY_TAP_SET>:
3000774c:	b510      	push	{r4, lr}
3000774e:	4b15      	ldr	r3, [pc, #84]	; (300077a4 <DDR_PHY_DELAY_TAP_SET+0x58>)
30007750:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
30007754:	4914      	ldr	r1, [pc, #80]	; (300077a8 <DDR_PHY_DELAY_TAP_SET+0x5c>)
30007756:	4c15      	ldr	r4, [pc, #84]	; (300077ac <DDR_PHY_DELAY_TAP_SET+0x60>)
30007758:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000775c:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
30007760:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30007764:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
30007768:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
3000776c:	7820      	ldrb	r0, [r4, #0]
3000776e:	b1a0      	cbz	r0, 3000779a <DDR_PHY_DELAY_TAP_SET+0x4e>
30007770:	2801      	cmp	r0, #1
30007772:	4b0c      	ldr	r3, [pc, #48]	; (300077a4 <DDR_PHY_DELAY_TAP_SET+0x58>)
30007774:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
30007778:	bf0c      	ite	eq
3000777a:	4a0d      	ldreq	r2, [pc, #52]	; (300077b0 <DDR_PHY_DELAY_TAP_SET+0x64>)
3000777c:	4a0d      	ldrne	r2, [pc, #52]	; (300077b4 <DDR_PHY_DELAY_TAP_SET+0x68>)
3000777e:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
30007782:	f640 0288 	movw	r2, #2184	; 0x888
30007786:	4b07      	ldr	r3, [pc, #28]	; (300077a4 <DDR_PHY_DELAY_TAP_SET+0x58>)
30007788:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
3000778c:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30007790:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30007794:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30007798:	bd10      	pop	{r4, pc}
3000779a:	4b07      	ldr	r3, [pc, #28]	; (300077b8 <DDR_PHY_DELAY_TAP_SET+0x6c>)
3000779c:	4798      	blx	r3
3000779e:	7020      	strb	r0, [r4, #0]
300077a0:	e7e6      	b.n	30007770 <DDR_PHY_DELAY_TAP_SET+0x24>
300077a2:	bf00      	nop
300077a4:	41011000 	.word	0x41011000
300077a8:	20888888 	.word	0x20888888
300077ac:	3000ed30 	.word	0x3000ed30
300077b0:	7c033333 	.word	0x7c033333
300077b4:	7c011111 	.word	0x7c011111
300077b8:	30007139 	.word	0x30007139

300077bc <DDR_PHY_READ_CTRL>:
300077bc:	4b10      	ldr	r3, [pc, #64]	; (30007800 <DDR_PHY_READ_CTRL+0x44>)
300077be:	b510      	push	{r4, lr}
300077c0:	7818      	ldrb	r0, [r3, #0]
300077c2:	28ff      	cmp	r0, #255	; 0xff
300077c4:	d019      	beq.n	300077fa <DDR_PHY_READ_CTRL+0x3e>
300077c6:	4a0f      	ldr	r2, [pc, #60]	; (30007804 <DDR_PHY_READ_CTRL+0x48>)
300077c8:	eb00 0080 	add.w	r0, r0, r0, lsl #2
300077cc:	4b0e      	ldr	r3, [pc, #56]	; (30007808 <DDR_PHY_READ_CTRL+0x4c>)
300077ce:	2108      	movs	r1, #8
300077d0:	eb02 0080 	add.w	r0, r2, r0, lsl #2
300077d4:	22fe      	movs	r2, #254	; 0xfe
300077d6:	7c44      	ldrb	r4, [r0, #17]
300077d8:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
300077dc:	7c84      	ldrb	r4, [r0, #18]
300077de:	7cc0      	ldrb	r0, [r0, #19]
300077e0:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
300077e4:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
300077e8:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
300077ec:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
300077f0:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
300077f4:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
300077f8:	bd10      	pop	{r4, pc}
300077fa:	f7ff fd33 	bl	30007264 <DDR_PHY_ChipInfo.part.0>
300077fe:	e7e2      	b.n	300077c6 <DDR_PHY_READ_CTRL+0xa>
30007800:	3000e029 	.word	0x3000e029
30007804:	3000c6ac 	.word	0x3000c6ac
30007808:	41011000 	.word	0x41011000

3000780c <DDR_PHY_READ_LEVELING>:
3000780c:	b508      	push	{r3, lr}
3000780e:	4b21      	ldr	r3, [pc, #132]	; (30007894 <DDR_PHY_READ_LEVELING+0x88>)
30007810:	7818      	ldrb	r0, [r3, #0]
30007812:	28ff      	cmp	r0, #255	; 0xff
30007814:	d03b      	beq.n	3000788e <DDR_PHY_READ_LEVELING+0x82>
30007816:	4b20      	ldr	r3, [pc, #128]	; (30007898 <DDR_PHY_READ_LEVELING+0x8c>)
30007818:	b1e8      	cbz	r0, 30007856 <DDR_PHY_READ_LEVELING+0x4a>
3000781a:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
3000781e:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30007822:	220f      	movs	r2, #15
30007824:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
30007828:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
3000782c:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
30007830:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
30007834:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
30007838:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
3000783c:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
30007840:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
30007844:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
30007848:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
3000784c:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30007850:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
30007854:	bd08      	pop	{r3, pc}
30007856:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
3000785a:	2110      	movs	r1, #16
3000785c:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30007860:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30007864:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30007868:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
3000786c:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30007870:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30007874:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30007878:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
3000787c:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30007880:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30007884:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30007888:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
3000788c:	bd08      	pop	{r3, pc}
3000788e:	f7ff fce9 	bl	30007264 <DDR_PHY_ChipInfo.part.0>
30007892:	e7c0      	b.n	30007816 <DDR_PHY_READ_LEVELING+0xa>
30007894:	3000e029 	.word	0x3000e029
30007898:	41011000 	.word	0x41011000

3000789c <DDR_PHY_Init>:
3000789c:	b510      	push	{r4, lr}
3000789e:	6d80      	ldr	r0, [r0, #88]	; 0x58
300078a0:	4c2f      	ldr	r4, [pc, #188]	; (30007960 <DDR_PHY_Init+0xc4>)
300078a2:	f7ff fd43 	bl	3000732c <DDR_PHY_DLL_CLK_DIV>
300078a6:	f7ff fe49 	bl	3000753c <DDR_PHY_CRT_Init>
300078aa:	f7ff feb5 	bl	30007618 <DDR_PHY_R240_ZQ_CAL>
300078ae:	f7ff fef9 	bl	300076a4 <DDR_PHY_ZQ_SET_SEL>
300078b2:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
300078b6:	2300      	movs	r3, #0
300078b8:	f44f 5180 	mov.w	r1, #4096	; 0x1000
300078bc:	4829      	ldr	r0, [pc, #164]	; (30007964 <DDR_PHY_Init+0xc8>)
300078be:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
300078c2:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
300078c6:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
300078ca:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
300078ce:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
300078d2:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
300078d6:	f7ff ff39 	bl	3000774c <DDR_PHY_DELAY_TAP_SET>
300078da:	f7ff ff6f 	bl	300077bc <DDR_PHY_READ_CTRL>
300078de:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
300078e2:	4a21      	ldr	r2, [pc, #132]	; (30007968 <DDR_PHY_Init+0xcc>)
300078e4:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
300078e8:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
300078ec:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
300078f0:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
300078f4:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
300078f8:	f7ff ff88 	bl	3000780c <DDR_PHY_READ_LEVELING>
300078fc:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30007900:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30007904:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30007908:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
3000790c:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30007910:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30007914:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30007918:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
3000791c:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30007920:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30007924:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30007928:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
3000792c:	f043 0303 	orr.w	r3, r3, #3
30007930:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30007934:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30007938:	f043 030c 	orr.w	r3, r3, #12
3000793c:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30007940:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007944:	f023 0303 	bic.w	r3, r3, #3
30007948:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000794c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30007950:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
30007954:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30007958:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000795c:	bd10      	pop	{r4, pc}
3000795e:	bf00      	nop
30007960:	41011000 	.word	0x41011000
30007964:	00045500 	.word	0x00045500
30007968:	11117777 	.word	0x11117777

3000796c <DDR_PHY_CAL>:
3000796c:	4770      	bx	lr
3000796e:	bf00      	nop

30007970 <DDR_PHY_AutoGating>:
30007970:	4a0c      	ldr	r2, [pc, #48]	; (300079a4 <DDR_PHY_AutoGating+0x34>)
30007972:	2032      	movs	r0, #50	; 0x32
30007974:	490c      	ldr	r1, [pc, #48]	; (300079a8 <DDR_PHY_AutoGating+0x38>)
30007976:	b508      	push	{r3, lr}
30007978:	6893      	ldr	r3, [r2, #8]
3000797a:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
3000797e:	6093      	str	r3, [r2, #8]
30007980:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30007984:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30007988:	f043 0314 	orr.w	r3, r3, #20
3000798c:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30007990:	4788      	blx	r1
30007992:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30007996:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
3000799a:	f043 0303 	orr.w	r3, r3, #3
3000799e:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
300079a2:	bd08      	pop	{r3, pc}
300079a4:	41011000 	.word	0x41011000
300079a8:	00009b2d 	.word	0x00009b2d

300079ac <flash_init_userdef>:
300079ac:	4b14      	ldr	r3, [pc, #80]	; (30007a00 <flash_init_userdef+0x54>)
300079ae:	2201      	movs	r2, #1
300079b0:	2140      	movs	r1, #64	; 0x40
300079b2:	f44f 6000 	mov.w	r0, #2048	; 0x800
300079b6:	b430      	push	{r4, r5}
300079b8:	e9c3 1203 	strd	r1, r2, [r3, #12]
300079bc:	2502      	movs	r5, #2
300079be:	2400      	movs	r4, #0
300079c0:	f640 0104 	movw	r1, #2052	; 0x804
300079c4:	601a      	str	r2, [r3, #0]
300079c6:	22bb      	movs	r2, #187	; 0xbb
300079c8:	83d8      	strh	r0, [r3, #30]
300079ca:	8419      	strh	r1, [r3, #32]
300079cc:	203b      	movs	r0, #59	; 0x3b
300079ce:	21eb      	movs	r1, #235	; 0xeb
300079d0:	629a      	str	r2, [r3, #40]	; 0x28
300079d2:	4a0c      	ldr	r2, [pc, #48]	; (30007a04 <flash_init_userdef+0x58>)
300079d4:	6258      	str	r0, [r3, #36]	; 0x24
300079d6:	6319      	str	r1, [r3, #48]	; 0x30
300079d8:	f64a 3120 	movw	r1, #43808	; 0xab20
300079dc:	655a      	str	r2, [r3, #84]	; 0x54
300079de:	22b9      	movs	r2, #185	; 0xb9
300079e0:	4809      	ldr	r0, [pc, #36]	; (30007a08 <flash_init_userdef+0x5c>)
300079e2:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
300079e6:	6598      	str	r0, [r3, #88]	; 0x58
300079e8:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
300079ec:	e9c3 5405 	strd	r5, r4, [r3, #20]
300079f0:	2406      	movs	r4, #6
300079f2:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
300079f6:	246b      	movs	r4, #107	; 0x6b
300079f8:	62dc      	str	r4, [r3, #44]	; 0x2c
300079fa:	bc30      	pop	{r4, r5}
300079fc:	4770      	bx	lr
300079fe:	bf00      	nop
30007a00:	2001c01c 	.word	0x2001c01c
30007a04:	00059f06 	.word	0x00059f06
30007a08:	d8600001 	.word	0xd8600001

30007a0c <nand_init_userdef>:
30007a0c:	4b18      	ldr	r3, [pc, #96]	; (30007a70 <nand_init_userdef+0x64>)
30007a0e:	2001      	movs	r0, #1
30007a10:	2200      	movs	r2, #0
30007a12:	2102      	movs	r1, #2
30007a14:	b410      	push	{r4}
30007a16:	6018      	str	r0, [r3, #0]
30007a18:	f640 0408 	movw	r4, #2056	; 0x808
30007a1c:	6159      	str	r1, [r3, #20]
30007a1e:	83dc      	strh	r4, [r3, #30]
30007a20:	f44f 6400 	mov.w	r4, #2048	; 0x800
30007a24:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30007a28:	841c      	strh	r4, [r3, #32]
30007a2a:	246b      	movs	r4, #107	; 0x6b
30007a2c:	619a      	str	r2, [r3, #24]
30007a2e:	62dc      	str	r4, [r3, #44]	; 0x2c
30007a30:	4c10      	ldr	r4, [pc, #64]	; (30007a74 <nand_init_userdef+0x68>)
30007a32:	775a      	strb	r2, [r3, #29]
30007a34:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30007a38:	629a      	str	r2, [r3, #40]	; 0x28
30007a3a:	631a      	str	r2, [r3, #48]	; 0x30
30007a3c:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30007a40:	65da      	str	r2, [r3, #92]	; 0x5c
30007a42:	e9c3 0003 	strd	r0, r0, [r3, #12]
30007a46:	203b      	movs	r0, #59	; 0x3b
30007a48:	6258      	str	r0, [r3, #36]	; 0x24
30007a4a:	200b      	movs	r0, #11
30007a4c:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30007a50:	6698      	str	r0, [r3, #104]	; 0x68
30007a52:	2008      	movs	r0, #8
30007a54:	4908      	ldr	r1, [pc, #32]	; (30007a78 <nand_init_userdef+0x6c>)
30007a56:	64d8      	str	r0, [r3, #76]	; 0x4c
30007a58:	f241 0013 	movw	r0, #4115	; 0x1013
30007a5c:	6599      	str	r1, [r3, #88]	; 0x58
30007a5e:	4907      	ldr	r1, [pc, #28]	; (30007a7c <nand_init_userdef+0x70>)
30007a60:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30007a64:	6719      	str	r1, [r3, #112]	; 0x70
30007a66:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30007a6a:	f85d 4b04 	ldr.w	r4, [sp], #4
30007a6e:	4770      	bx	lr
30007a70:	2001c01c 	.word	0x2001c01c
30007a74:	000f9f06 	.word	0x000f9f06
30007a78:	d800001f 	.word	0xd800001f
30007a7c:	10300804 	.word	0x10300804

30007a80 <flash_get_layout_info>:
30007a80:	b570      	push	{r4, r5, r6, lr}
30007a82:	4b0f      	ldr	r3, [pc, #60]	; (30007ac0 <flash_get_layout_info+0x40>)
30007a84:	4615      	mov	r5, r2
30007a86:	4604      	mov	r4, r0
30007a88:	460e      	mov	r6, r1
30007a8a:	4798      	blx	r3
30007a8c:	4b0d      	ldr	r3, [pc, #52]	; (30007ac4 <flash_get_layout_info+0x44>)
30007a8e:	4a0e      	ldr	r2, [pc, #56]	; (30007ac8 <flash_get_layout_info+0x48>)
30007a90:	2800      	cmp	r0, #0
30007a92:	bf14      	ite	ne
30007a94:	469c      	movne	ip, r3
30007a96:	4694      	moveq	ip, r2
30007a98:	f8dc 3000 	ldr.w	r3, [ip]
30007a9c:	2bff      	cmp	r3, #255	; 0xff
30007a9e:	d104      	bne.n	30007aaa <flash_get_layout_info+0x2a>
30007aa0:	e00d      	b.n	30007abe <flash_get_layout_info+0x3e>
30007aa2:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
30007aa6:	2bff      	cmp	r3, #255	; 0xff
30007aa8:	d009      	beq.n	30007abe <flash_get_layout_info+0x3e>
30007aaa:	429c      	cmp	r4, r3
30007aac:	d1f9      	bne.n	30007aa2 <flash_get_layout_info+0x22>
30007aae:	b116      	cbz	r6, 30007ab6 <flash_get_layout_info+0x36>
30007ab0:	f8dc 3004 	ldr.w	r3, [ip, #4]
30007ab4:	6033      	str	r3, [r6, #0]
30007ab6:	b115      	cbz	r5, 30007abe <flash_get_layout_info+0x3e>
30007ab8:	f8dc 3008 	ldr.w	r3, [ip, #8]
30007abc:	602b      	str	r3, [r5, #0]
30007abe:	bd70      	pop	{r4, r5, r6, pc}
30007ac0:	30008ef5 	.word	0x30008ef5
30007ac4:	3000e104 	.word	0x3000e104
30007ac8:	3000e02c 	.word	0x3000e02c

30007acc <NAND_CHECK_IS_BAD_BLOCK>:
30007acc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007ad0:	4e24      	ldr	r6, [pc, #144]	; (30007b64 <NAND_CHECK_IS_BAD_BLOCK+0x98>)
30007ad2:	0907      	lsrs	r7, r0, #4
30007ad4:	0045      	lsls	r5, r0, #1
30007ad6:	b085      	sub	sp, #20
30007ad8:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30007adc:	f005 051e 	and.w	r5, r5, #30
30007ae0:	40eb      	lsrs	r3, r5
30007ae2:	f003 0303 	and.w	r3, r3, #3
30007ae6:	2b02      	cmp	r3, #2
30007ae8:	d02c      	beq.n	30007b44 <NAND_CHECK_IS_BAD_BLOCK+0x78>
30007aea:	2b01      	cmp	r3, #1
30007aec:	d026      	beq.n	30007b3c <NAND_CHECK_IS_BAD_BLOCK+0x70>
30007aee:	4b1e      	ldr	r3, [pc, #120]	; (30007b68 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30007af0:	f04f 0801 	mov.w	r8, #1
30007af4:	2204      	movs	r2, #4
30007af6:	4604      	mov	r4, r0
30007af8:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30007afc:	0180      	lsls	r0, r0, #6
30007afe:	ab03      	add	r3, sp, #12
30007b00:	f8df 9070 	ldr.w	r9, [pc, #112]	; 30007b74 <NAND_CHECK_IS_BAD_BLOCK+0xa8>
30007b04:	fa08 f101 	lsl.w	r1, r8, r1
30007b08:	47c8      	blx	r9
30007b0a:	f89d 300c 	ldrb.w	r3, [sp, #12]
30007b0e:	f89d 200d 	ldrb.w	r2, [sp, #13]
30007b12:	4013      	ands	r3, r2
30007b14:	2bff      	cmp	r3, #255	; 0xff
30007b16:	d019      	beq.n	30007b4c <NAND_CHECK_IS_BAD_BLOCK+0x80>
30007b18:	2002      	movs	r0, #2
30007b1a:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
30007b1e:	4b13      	ldr	r3, [pc, #76]	; (30007b6c <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30007b20:	fa00 f505 	lsl.w	r5, r0, r5
30007b24:	9400      	str	r4, [sp, #0]
30007b26:	4619      	mov	r1, r3
30007b28:	432a      	orrs	r2, r5
30007b2a:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
30007b2e:	4a10      	ldr	r2, [pc, #64]	; (30007b70 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30007b30:	f003 fb7c 	bl	3000b22c <rtk_log_write>
30007b34:	4640      	mov	r0, r8
30007b36:	b005      	add	sp, #20
30007b38:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007b3c:	2000      	movs	r0, #0
30007b3e:	b005      	add	sp, #20
30007b40:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007b44:	2001      	movs	r0, #1
30007b46:	b005      	add	sp, #20
30007b48:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007b4c:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30007b50:	fa08 f005 	lsl.w	r0, r8, r5
30007b54:	4303      	orrs	r3, r0
30007b56:	2000      	movs	r0, #0
30007b58:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30007b5c:	b005      	add	sp, #20
30007b5e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007b62:	bf00      	nop
30007b64:	3000ed38 	.word	0x3000ed38
30007b68:	2001c01c 	.word	0x2001c01c
30007b6c:	3000c940 	.word	0x3000c940
30007b70:	3000c94c 	.word	0x3000c94c
30007b74:	30008ec5 	.word	0x30008ec5

30007b78 <Nand_Get_NandAddr>:
30007b78:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007b7c:	4f10      	ldr	r7, [pc, #64]	; (30007bc0 <Nand_Get_NandAddr+0x48>)
30007b7e:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30007b82:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30007b86:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30007b8a:	40e8      	lsrs	r0, r5
30007b8c:	fa26 f505 	lsr.w	r5, r6, r5
30007b90:	09ad      	lsrs	r5, r5, #6
30007b92:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30007b96:	d90f      	bls.n	30007bb8 <Nand_Get_NandAddr+0x40>
30007b98:	0984      	lsrs	r4, r0, #6
30007b9a:	f04f 0840 	mov.w	r8, #64	; 0x40
30007b9e:	4620      	mov	r0, r4
30007ba0:	3401      	adds	r4, #1
30007ba2:	f7ff ff93 	bl	30007acc <NAND_CHECK_IS_BAD_BLOCK>
30007ba6:	b128      	cbz	r0, 30007bb4 <Nand_Get_NandAddr+0x3c>
30007ba8:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007bac:	3501      	adds	r5, #1
30007bae:	fa08 f303 	lsl.w	r3, r8, r3
30007bb2:	441e      	add	r6, r3
30007bb4:	42a5      	cmp	r5, r4
30007bb6:	d8f2      	bhi.n	30007b9e <Nand_Get_NandAddr+0x26>
30007bb8:	4630      	mov	r0, r6
30007bba:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007bbe:	bf00      	nop
30007bc0:	2001c01c 	.word	0x2001c01c

30007bc4 <Nand_L2P_Table>:
30007bc4:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007bc8:	b082      	sub	sp, #8
30007bca:	4604      	mov	r4, r0
30007bcc:	2002      	movs	r0, #2
30007bce:	aa01      	add	r2, sp, #4
30007bd0:	4669      	mov	r1, sp
30007bd2:	f7ff ff55 	bl	30007a80 <flash_get_layout_info>
30007bd6:	9800      	ldr	r0, [sp, #0]
30007bd8:	42a0      	cmp	r0, r4
30007bda:	d810      	bhi.n	30007bfe <Nand_L2P_Table+0x3a>
30007bdc:	9b01      	ldr	r3, [sp, #4]
30007bde:	42a3      	cmp	r3, r4
30007be0:	d805      	bhi.n	30007bee <Nand_L2P_Table+0x2a>
30007be2:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30007be6:	4620      	mov	r0, r4
30007be8:	b002      	add	sp, #8
30007bea:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007bee:	4621      	mov	r1, r4
30007bf0:	f7ff ffc2 	bl	30007b78 <Nand_Get_NandAddr>
30007bf4:	4604      	mov	r4, r0
30007bf6:	4620      	mov	r0, r4
30007bf8:	b002      	add	sp, #8
30007bfa:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007bfe:	4f0e      	ldr	r7, [pc, #56]	; (30007c38 <Nand_L2P_Table+0x74>)
30007c00:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30007c04:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
30007c08:	fa24 f606 	lsr.w	r6, r4, r6
30007c0c:	09b6      	lsrs	r6, r6, #6
30007c0e:	d0ea      	beq.n	30007be6 <Nand_L2P_Table+0x22>
30007c10:	2500      	movs	r5, #0
30007c12:	f04f 0840 	mov.w	r8, #64	; 0x40
30007c16:	4628      	mov	r0, r5
30007c18:	3501      	adds	r5, #1
30007c1a:	f7ff ff57 	bl	30007acc <NAND_CHECK_IS_BAD_BLOCK>
30007c1e:	b128      	cbz	r0, 30007c2c <Nand_L2P_Table+0x68>
30007c20:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007c24:	3601      	adds	r6, #1
30007c26:	fa08 f303 	lsl.w	r3, r8, r3
30007c2a:	441c      	add	r4, r3
30007c2c:	42b5      	cmp	r5, r6
30007c2e:	d3f2      	bcc.n	30007c16 <Nand_L2P_Table+0x52>
30007c30:	4620      	mov	r0, r4
30007c32:	b002      	add	sp, #8
30007c34:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007c38:	2001c01c 	.word	0x2001c01c

30007c3c <NandImgCopy>:
30007c3c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007c40:	4617      	mov	r7, r2
30007c42:	4603      	mov	r3, r0
30007c44:	b089      	sub	sp, #36	; 0x24
30007c46:	f8df 9100 	ldr.w	r9, [pc, #256]	; 30007d48 <NandImgCopy+0x10c>
30007c4a:	f107 38ff 	add.w	r8, r7, #4294967295	; 0xffffffff
30007c4e:	4608      	mov	r0, r1
30007c50:	9307      	str	r3, [sp, #28]
30007c52:	f7ff ffb7 	bl	30007bc4 <Nand_L2P_Table>
30007c56:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30007c5a:	4480      	add	r8, r0
30007c5c:	fa20 f406 	lsr.w	r4, r0, r6
30007c60:	fa28 f806 	lsr.w	r8, r8, r6
30007c64:	eba8 0804 	sub.w	r8, r8, r4
30007c68:	f1b8 3fff 	cmp.w	r8, #4294967295	; 0xffffffff
30007c6c:	d054      	beq.n	30007d18 <NandImgCopy+0xdc>
30007c6e:	2300      	movs	r3, #0
30007c70:	4605      	mov	r5, r0
30007c72:	f04f 0b40 	mov.w	fp, #64	; 0x40
30007c76:	469a      	mov	sl, r3
30007c78:	9306      	str	r3, [sp, #24]
30007c7a:	463b      	mov	r3, r7
30007c7c:	4647      	mov	r7, r8
30007c7e:	4698      	mov	r8, r3
30007c80:	e020      	b.n	30007cc4 <NandImgCopy+0x88>
30007c82:	f1c2 0320 	rsb	r3, r2, #32
30007c86:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30007c8a:	40d9      	lsrs	r1, r3
30007c8c:	9b07      	ldr	r3, [sp, #28]
30007c8e:	4029      	ands	r1, r5
30007c90:	4453      	add	r3, sl
30007c92:	b377      	cbz	r7, 30007cf2 <NandImgCopy+0xb6>
30007c94:	2601      	movs	r6, #1
30007c96:	4620      	mov	r0, r4
30007c98:	9105      	str	r1, [sp, #20]
30007c9a:	fa06 f202 	lsl.w	r2, r6, r2
30007c9e:	4e27      	ldr	r6, [pc, #156]	; (30007d3c <NandImgCopy+0x100>)
30007ca0:	9204      	str	r2, [sp, #16]
30007ca2:	1a52      	subs	r2, r2, r1
30007ca4:	4492      	add	sl, r2
30007ca6:	4415      	add	r5, r2
30007ca8:	47b0      	blx	r6
30007caa:	f010 0f70 	tst.w	r0, #112	; 0x70
30007cae:	9b04      	ldr	r3, [sp, #16]
30007cb0:	9905      	ldr	r1, [sp, #20]
30007cb2:	d134      	bne.n	30007d1e <NandImgCopy+0xe2>
30007cb4:	eba8 0803 	sub.w	r8, r8, r3
30007cb8:	4488      	add	r8, r1
30007cba:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30007cbe:	3f01      	subs	r7, #1
30007cc0:	fa25 f406 	lsr.w	r4, r5, r6
30007cc4:	9b06      	ldr	r3, [sp, #24]
30007cc6:	4632      	mov	r2, r6
30007cc8:	09a6      	lsrs	r6, r4, #6
30007cca:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30007cce:	d8d8      	bhi.n	30007c82 <NandImgCopy+0x46>
30007cd0:	e005      	b.n	30007cde <NandImgCopy+0xa2>
30007cd2:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30007cd6:	3440      	adds	r4, #64	; 0x40
30007cd8:	fa0b f303 	lsl.w	r3, fp, r3
30007cdc:	441d      	add	r5, r3
30007cde:	4630      	mov	r0, r6
30007ce0:	3601      	adds	r6, #1
30007ce2:	f7ff fef3 	bl	30007acc <NAND_CHECK_IS_BAD_BLOCK>
30007ce6:	2800      	cmp	r0, #0
30007ce8:	d1f3      	bne.n	30007cd2 <NandImgCopy+0x96>
30007cea:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30007cee:	9606      	str	r6, [sp, #24]
30007cf0:	e7c7      	b.n	30007c82 <NandImgCopy+0x46>
30007cf2:	4642      	mov	r2, r8
30007cf4:	4620      	mov	r0, r4
30007cf6:	4d11      	ldr	r5, [pc, #68]	; (30007d3c <NandImgCopy+0x100>)
30007cf8:	47a8      	blx	r5
30007cfa:	f010 0f70 	tst.w	r0, #112	; 0x70
30007cfe:	d00b      	beq.n	30007d18 <NandImgCopy+0xdc>
30007d00:	09a1      	lsrs	r1, r4, #6
30007d02:	4b0f      	ldr	r3, [pc, #60]	; (30007d40 <NandImgCopy+0x104>)
30007d04:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30007d08:	4a0e      	ldr	r2, [pc, #56]	; (30007d44 <NandImgCopy+0x108>)
30007d0a:	e9cd 1001 	strd	r1, r0, [sp, #4]
30007d0e:	9400      	str	r4, [sp, #0]
30007d10:	4619      	mov	r1, r3
30007d12:	2004      	movs	r0, #4
30007d14:	f003 fa8a 	bl	3000b22c <rtk_log_write>
30007d18:	b009      	add	sp, #36	; 0x24
30007d1a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007d1e:	1ac9      	subs	r1, r1, r3
30007d20:	09a3      	lsrs	r3, r4, #6
30007d22:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30007d26:	9002      	str	r0, [sp, #8]
30007d28:	9301      	str	r3, [sp, #4]
30007d2a:	4488      	add	r8, r1
30007d2c:	4b04      	ldr	r3, [pc, #16]	; (30007d40 <NandImgCopy+0x104>)
30007d2e:	2004      	movs	r0, #4
30007d30:	4a04      	ldr	r2, [pc, #16]	; (30007d44 <NandImgCopy+0x108>)
30007d32:	4619      	mov	r1, r3
30007d34:	9400      	str	r4, [sp, #0]
30007d36:	f003 fa79 	bl	3000b22c <rtk_log_write>
30007d3a:	e7be      	b.n	30007cba <NandImgCopy+0x7e>
30007d3c:	30008ec5 	.word	0x30008ec5
30007d40:	3000c940 	.word	0x3000c940
30007d44:	3000c964 	.word	0x3000c964
30007d48:	2001c01c 	.word	0x2001c01c

30007d4c <OTP_Read8.part.0>:
30007d4c:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30007d50:	4b30      	ldr	r3, [pc, #192]	; (30007e14 <OTP_Read8.part.0+0xc8>)
30007d52:	4680      	mov	r8, r0
30007d54:	e843 f300 	tt	r3, r3
30007d58:	f3c3 5380 	ubfx	r3, r3, #22, #1
30007d5c:	4a2e      	ldr	r2, [pc, #184]	; (30007e18 <OTP_Read8.part.0+0xcc>)
30007d5e:	4e2f      	ldr	r6, [pc, #188]	; (30007e1c <OTP_Read8.part.0+0xd0>)
30007d60:	b082      	sub	sp, #8
30007d62:	2b00      	cmp	r3, #0
30007d64:	4689      	mov	r9, r1
30007d66:	bf0e      	itee	eq
30007d68:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30007d6c:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30007d70:	4616      	movne	r6, r2
30007d72:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30007d74:	05d8      	lsls	r0, r3, #23
30007d76:	d505      	bpl.n	30007d84 <OTP_Read8.part.0+0x38>
30007d78:	4d29      	ldr	r5, [pc, #164]	; (30007e20 <OTP_Read8.part.0+0xd4>)
30007d7a:	2064      	movs	r0, #100	; 0x64
30007d7c:	47a8      	blx	r5
30007d7e:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30007d80:	05d9      	lsls	r1, r3, #23
30007d82:	d4fa      	bmi.n	30007d7a <OTP_Read8.part.0+0x2e>
30007d84:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30007d86:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007d8a:	643b      	str	r3, [r7, #64]	; 0x40
30007d8c:	4b25      	ldr	r3, [pc, #148]	; (30007e24 <OTP_Read8.part.0+0xd8>)
30007d8e:	681a      	ldr	r2, [r3, #0]
30007d90:	07d2      	lsls	r2, r2, #31
30007d92:	d403      	bmi.n	30007d9c <OTP_Read8.part.0+0x50>
30007d94:	681a      	ldr	r2, [r3, #0]
30007d96:	f042 0201 	orr.w	r2, r2, #1
30007d9a:	601a      	str	r2, [r3, #0]
30007d9c:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30007da0:	ea4f 2308 	mov.w	r3, r8, lsl #8
30007da4:	694a      	ldr	r2, [r1, #20]
30007da6:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30007daa:	614a      	str	r2, [r1, #20]
30007dac:	6033      	str	r3, [r6, #0]
30007dae:	6833      	ldr	r3, [r6, #0]
30007db0:	2b00      	cmp	r3, #0
30007db2:	db10      	blt.n	30007dd6 <OTP_Read8.part.0+0x8a>
30007db4:	2400      	movs	r4, #0
30007db6:	4d1a      	ldr	r5, [pc, #104]	; (30007e20 <OTP_Read8.part.0+0xd4>)
30007db8:	f644 6a20 	movw	sl, #20000	; 0x4e20
30007dbc:	e001      	b.n	30007dc2 <OTP_Read8.part.0+0x76>
30007dbe:	4554      	cmp	r4, sl
30007dc0:	d01a      	beq.n	30007df8 <OTP_Read8.part.0+0xac>
30007dc2:	2005      	movs	r0, #5
30007dc4:	3401      	adds	r4, #1
30007dc6:	47a8      	blx	r5
30007dc8:	6833      	ldr	r3, [r6, #0]
30007dca:	2b00      	cmp	r3, #0
30007dcc:	daf7      	bge.n	30007dbe <OTP_Read8.part.0+0x72>
30007dce:	f644 6320 	movw	r3, #20000	; 0x4e20
30007dd2:	429c      	cmp	r4, r3
30007dd4:	d010      	beq.n	30007df8 <OTP_Read8.part.0+0xac>
30007dd6:	6833      	ldr	r3, [r6, #0]
30007dd8:	2001      	movs	r0, #1
30007dda:	f889 3000 	strb.w	r3, [r9]
30007dde:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30007de2:	6953      	ldr	r3, [r2, #20]
30007de4:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30007de8:	6153      	str	r3, [r2, #20]
30007dea:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30007dec:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30007df0:	643b      	str	r3, [r7, #64]	; 0x40
30007df2:	b002      	add	sp, #8
30007df4:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30007df8:	22ff      	movs	r2, #255	; 0xff
30007dfa:	4b0b      	ldr	r3, [pc, #44]	; (30007e28 <OTP_Read8.part.0+0xdc>)
30007dfc:	2002      	movs	r0, #2
30007dfe:	f8cd 8000 	str.w	r8, [sp]
30007e02:	4619      	mov	r1, r3
30007e04:	f889 2000 	strb.w	r2, [r9]
30007e08:	4a08      	ldr	r2, [pc, #32]	; (30007e2c <OTP_Read8.part.0+0xe0>)
30007e0a:	f003 fa0f 	bl	3000b22c <rtk_log_write>
30007e0e:	2000      	movs	r0, #0
30007e10:	e7e5      	b.n	30007dde <OTP_Read8.part.0+0x92>
30007e12:	bf00      	nop
30007e14:	0000e5f9 	.word	0x0000e5f9
30007e18:	5200000c 	.word	0x5200000c
30007e1c:	42000008 	.word	0x42000008
30007e20:	00009b2d 	.word	0x00009b2d
30007e24:	42008000 	.word	0x42008000
30007e28:	3000c9a0 	.word	0x3000c9a0
30007e2c:	3000c9a4 	.word	0x3000c9a4

30007e30 <OTP_Read8>:
30007e30:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30007e34:	d201      	bcs.n	30007e3a <OTP_Read8+0xa>
30007e36:	f7ff bf89 	b.w	30007d4c <OTP_Read8.part.0>
30007e3a:	22ff      	movs	r2, #255	; 0xff
30007e3c:	2000      	movs	r0, #0
30007e3e:	700a      	strb	r2, [r1, #0]
30007e40:	4770      	bx	lr
30007e42:	bf00      	nop

30007e44 <PLL_NP_ClkSet>:
30007e44:	b570      	push	{r4, r5, r6, lr}
30007e46:	4604      	mov	r4, r0
30007e48:	4b1b      	ldr	r3, [pc, #108]	; (30007eb8 <PLL_NP_ClkSet+0x74>)
30007e4a:	4798      	blx	r3
30007e4c:	4d1b      	ldr	r5, [pc, #108]	; (30007ebc <PLL_NP_ClkSet+0x78>)
30007e4e:	08c2      	lsrs	r2, r0, #3
30007e50:	fbb4 f3f0 	udiv	r3, r4, r0
30007e54:	fb00 4c13 	mls	ip, r0, r3, r4
30007e58:	1e9c      	subs	r4, r3, #2
30007e5a:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30007e5c:	0164      	lsls	r4, r4, #5
30007e5e:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30007e62:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30007e66:	431c      	orrs	r4, r3
30007e68:	fbbc f3f2 	udiv	r3, ip, r2
30007e6c:	646c      	str	r4, [r5, #68]	; 0x44
30007e6e:	041c      	lsls	r4, r3, #16
30007e70:	fb02 c013 	mls	r0, r2, r3, ip
30007e74:	2300      	movs	r3, #0
30007e76:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30007e78:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30007e7c:	4619      	mov	r1, r3
30007e7e:	fa1f fc86 	uxth.w	ip, r6
30007e82:	4e0f      	ldr	r6, [pc, #60]	; (30007ec0 <PLL_NP_ClkSet+0x7c>)
30007e84:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30007e88:	ea44 040c 	orr.w	r4, r4, ip
30007e8c:	f003 fa58 	bl	3000b340 <__aeabi_uldivmod>
30007e90:	4603      	mov	r3, r0
30007e92:	2001      	movs	r0, #1
30007e94:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30007e98:	64ec      	str	r4, [r5, #76]	; 0x4c
30007e9a:	47b0      	blx	r6
30007e9c:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30007e9e:	203c      	movs	r0, #60	; 0x3c
30007ea0:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30007ea4:	646b      	str	r3, [r5, #68]	; 0x44
30007ea6:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30007ea8:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30007eac:	646b      	str	r3, [r5, #68]	; 0x44
30007eae:	4633      	mov	r3, r6
30007eb0:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30007eb4:	4718      	bx	r3
30007eb6:	bf00      	nop
30007eb8:	000099f5 	.word	0x000099f5
30007ebc:	42008800 	.word	0x42008800
30007ec0:	00009b2d 	.word	0x00009b2d

30007ec4 <PLL_AP_ClkSet>:
30007ec4:	b510      	push	{r4, lr}
30007ec6:	4604      	mov	r4, r0
30007ec8:	4b0b      	ldr	r3, [pc, #44]	; (30007ef8 <PLL_AP_ClkSet+0x34>)
30007eca:	4798      	blx	r3
30007ecc:	fbb4 f0f0 	udiv	r0, r4, r0
30007ed0:	f1a0 0319 	sub.w	r3, r0, #25
30007ed4:	1e84      	subs	r4, r0, #2
30007ed6:	2b0f      	cmp	r3, #15
30007ed8:	d807      	bhi.n	30007eea <PLL_AP_ClkSet+0x26>
30007eda:	4b08      	ldr	r3, [pc, #32]	; (30007efc <PLL_AP_ClkSet+0x38>)
30007edc:	b2e0      	uxtb	r0, r4
30007ede:	695c      	ldr	r4, [r3, #20]
30007ee0:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30007ee4:	4320      	orrs	r0, r4
30007ee6:	6158      	str	r0, [r3, #20]
30007ee8:	bd10      	pop	{r4, pc}
30007eea:	f240 1181 	movw	r1, #385	; 0x181
30007eee:	4804      	ldr	r0, [pc, #16]	; (30007f00 <PLL_AP_ClkSet+0x3c>)
30007ef0:	f003 fc72 	bl	3000b7d8 <__io_assert_failed_veneer>
30007ef4:	e7f1      	b.n	30007eda <PLL_AP_ClkSet+0x16>
30007ef6:	bf00      	nop
30007ef8:	000099f5 	.word	0x000099f5
30007efc:	41000300 	.word	0x41000300
30007f00:	3000c9c0 	.word	0x3000c9c0

30007f04 <PLL_AP>:
30007f04:	2801      	cmp	r0, #1
30007f06:	b538      	push	{r3, r4, r5, lr}
30007f08:	d009      	beq.n	30007f1e <PLL_AP+0x1a>
30007f0a:	4b13      	ldr	r3, [pc, #76]	; (30007f58 <PLL_AP+0x54>)
30007f0c:	681a      	ldr	r2, [r3, #0]
30007f0e:	f022 0209 	bic.w	r2, r2, #9
30007f12:	601a      	str	r2, [r3, #0]
30007f14:	681a      	ldr	r2, [r3, #0]
30007f16:	f022 0206 	bic.w	r2, r2, #6
30007f1a:	601a      	str	r2, [r3, #0]
30007f1c:	bd38      	pop	{r3, r4, r5, pc}
30007f1e:	4b0f      	ldr	r3, [pc, #60]	; (30007f5c <PLL_AP+0x58>)
30007f20:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30007f22:	f012 0f05 	tst.w	r2, #5
30007f26:	d00f      	beq.n	30007f48 <PLL_AP+0x44>
30007f28:	4d0d      	ldr	r5, [pc, #52]	; (30007f60 <PLL_AP+0x5c>)
30007f2a:	4c0b      	ldr	r4, [pc, #44]	; (30007f58 <PLL_AP+0x54>)
30007f2c:	2001      	movs	r0, #1
30007f2e:	6823      	ldr	r3, [r4, #0]
30007f30:	f043 0306 	orr.w	r3, r3, #6
30007f34:	6023      	str	r3, [r4, #0]
30007f36:	47a8      	blx	r5
30007f38:	6823      	ldr	r3, [r4, #0]
30007f3a:	f043 0309 	orr.w	r3, r3, #9
30007f3e:	6023      	str	r3, [r4, #0]
30007f40:	6863      	ldr	r3, [r4, #4]
30007f42:	2b00      	cmp	r3, #0
30007f44:	dafc      	bge.n	30007f40 <PLL_AP+0x3c>
30007f46:	bd38      	pop	{r3, r4, r5, pc}
30007f48:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30007f4a:	20a0      	movs	r0, #160	; 0xa0
30007f4c:	4d04      	ldr	r5, [pc, #16]	; (30007f60 <PLL_AP+0x5c>)
30007f4e:	f042 0207 	orr.w	r2, r2, #7
30007f52:	679a      	str	r2, [r3, #120]	; 0x78
30007f54:	47a8      	blx	r5
30007f56:	e7e8      	b.n	30007f2a <PLL_AP+0x26>
30007f58:	41000300 	.word	0x41000300
30007f5c:	42008800 	.word	0x42008800
30007f60:	00009b2d 	.word	0x00009b2d

30007f64 <PSRAM_INFO_Update>:
30007f64:	4b49      	ldr	r3, [pc, #292]	; (3000808c <PSRAM_INFO_Update+0x128>)
30007f66:	494a      	ldr	r1, [pc, #296]	; (30008090 <PSRAM_INFO_Update+0x12c>)
30007f68:	4a4a      	ldr	r2, [pc, #296]	; (30008094 <PSRAM_INFO_Update+0x130>)
30007f6a:	b530      	push	{r4, r5, lr}
30007f6c:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
30007f70:	b083      	sub	sp, #12
30007f72:	680d      	ldr	r5, [r1, #0]
30007f74:	f3c3 2302 	ubfx	r3, r3, #8, #3
30007f78:	4c47      	ldr	r4, [pc, #284]	; (30008098 <PSRAM_INFO_Update+0x134>)
30007f7a:	fb02 f505 	mul.w	r5, r2, r5
30007f7e:	3301      	adds	r3, #1
30007f80:	fbb5 f5f3 	udiv	r5, r5, r3
30007f84:	086a      	lsrs	r2, r5, #1
30007f86:	4b45      	ldr	r3, [pc, #276]	; (3000809c <PSRAM_INFO_Update+0x138>)
30007f88:	60e2      	str	r2, [r4, #12]
30007f8a:	4798      	blx	r3
30007f8c:	4b44      	ldr	r3, [pc, #272]	; (300080a0 <PSRAM_INFO_Update+0x13c>)
30007f8e:	6020      	str	r0, [r4, #0]
30007f90:	4798      	blx	r3
30007f92:	4b44      	ldr	r3, [pc, #272]	; (300080a4 <PSRAM_INFO_Update+0x140>)
30007f94:	220a      	movs	r2, #10
30007f96:	60a0      	str	r0, [r4, #8]
30007f98:	6122      	str	r2, [r4, #16]
30007f9a:	2220      	movs	r2, #32
30007f9c:	fbb3 f3f5 	udiv	r3, r3, r5
30007fa0:	6163      	str	r3, [r4, #20]
30007fa2:	fbb2 f3f3 	udiv	r3, r2, r3
30007fa6:	6822      	ldr	r2, [r4, #0]
30007fa8:	6223      	str	r3, [r4, #32]
30007faa:	2a01      	cmp	r2, #1
30007fac:	68e3      	ldr	r3, [r4, #12]
30007fae:	4a3e      	ldr	r2, [pc, #248]	; (300080a8 <PSRAM_INFO_Update+0x144>)
30007fb0:	d01d      	beq.n	30007fee <PSRAM_INFO_Update+0x8a>
30007fb2:	4293      	cmp	r3, r2
30007fb4:	d810      	bhi.n	30007fd8 <PSRAM_INFO_Update+0x74>
30007fb6:	2304      	movs	r3, #4
30007fb8:	2208      	movs	r2, #8
30007fba:	4618      	mov	r0, r3
30007fbc:	e9c4 3206 	strd	r3, r2, [r4, #24]
30007fc0:	4b3a      	ldr	r3, [pc, #232]	; (300080ac <PSRAM_INFO_Update+0x148>)
30007fc2:	493b      	ldr	r1, [pc, #236]	; (300080b0 <PSRAM_INFO_Update+0x14c>)
30007fc4:	4a3b      	ldr	r2, [pc, #236]	; (300080b4 <PSRAM_INFO_Update+0x150>)
30007fc6:	f881 0110 	strb.w	r0, [r1, #272]	; 0x110
30007fca:	4619      	mov	r1, r3
30007fcc:	2004      	movs	r0, #4
30007fce:	9500      	str	r5, [sp, #0]
30007fd0:	f003 f92c 	bl	3000b22c <rtk_log_write>
30007fd4:	b003      	add	sp, #12
30007fd6:	bd30      	pop	{r4, r5, pc}
30007fd8:	4a37      	ldr	r2, [pc, #220]	; (300080b8 <PSRAM_INFO_Update+0x154>)
30007fda:	4293      	cmp	r3, r2
30007fdc:	d918      	bls.n	30008010 <PSRAM_INFO_Update+0xac>
30007fde:	4a37      	ldr	r2, [pc, #220]	; (300080bc <PSRAM_INFO_Update+0x158>)
30007fe0:	4293      	cmp	r3, r2
30007fe2:	d827      	bhi.n	30008034 <PSRAM_INFO_Update+0xd0>
30007fe4:	2306      	movs	r3, #6
30007fe6:	4618      	mov	r0, r3
30007fe8:	e9c4 3306 	strd	r3, r3, [r4, #24]
30007fec:	e7e8      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
30007fee:	4293      	cmp	r3, r2
30007ff0:	d908      	bls.n	30008004 <PSRAM_INFO_Update+0xa0>
30007ff2:	4a31      	ldr	r2, [pc, #196]	; (300080b8 <PSRAM_INFO_Update+0x154>)
30007ff4:	4293      	cmp	r3, r2
30007ff6:	d811      	bhi.n	3000801c <PSRAM_INFO_Update+0xb8>
30007ff8:	2305      	movs	r3, #5
30007ffa:	220f      	movs	r2, #15
30007ffc:	4618      	mov	r0, r3
30007ffe:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008002:	e7dd      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
30008004:	2304      	movs	r3, #4
30008006:	220f      	movs	r2, #15
30008008:	4618      	mov	r0, r3
3000800a:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000800e:	e7d7      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
30008010:	2305      	movs	r3, #5
30008012:	2208      	movs	r2, #8
30008014:	4618      	mov	r0, r3
30008016:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000801a:	e7d1      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
3000801c:	4a27      	ldr	r2, [pc, #156]	; (300080bc <PSRAM_INFO_Update+0x158>)
3000801e:	4293      	cmp	r3, r2
30008020:	d918      	bls.n	30008054 <PSRAM_INFO_Update+0xf0>
30008022:	4a27      	ldr	r2, [pc, #156]	; (300080c0 <PSRAM_INFO_Update+0x15c>)
30008024:	4293      	cmp	r3, r2
30008026:	d821      	bhi.n	3000806c <PSRAM_INFO_Update+0x108>
30008028:	2307      	movs	r3, #7
3000802a:	2214      	movs	r2, #20
3000802c:	4618      	mov	r0, r3
3000802e:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008032:	e7c5      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
30008034:	4a22      	ldr	r2, [pc, #136]	; (300080c0 <PSRAM_INFO_Update+0x15c>)
30008036:	4293      	cmp	r3, r2
30008038:	d912      	bls.n	30008060 <PSRAM_INFO_Update+0xfc>
3000803a:	4a22      	ldr	r2, [pc, #136]	; (300080c4 <PSRAM_INFO_Update+0x160>)
3000803c:	4293      	cmp	r3, r2
3000803e:	bf8c      	ite	hi
30008040:	2300      	movhi	r3, #0
30008042:	2301      	movls	r3, #1
30008044:	2804      	cmp	r0, #4
30008046:	d100      	bne.n	3000804a <PSRAM_INFO_Update+0xe6>
30008048:	b953      	cbnz	r3, 30008060 <PSRAM_INFO_Update+0xfc>
3000804a:	2803      	cmp	r0, #3
3000804c:	d100      	bne.n	30008050 <PSRAM_INFO_Update+0xec>
3000804e:	b9b3      	cbnz	r3, 3000807e <PSRAM_INFO_Update+0x11a>
30008050:	7e20      	ldrb	r0, [r4, #24]
30008052:	e7b5      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
30008054:	2306      	movs	r3, #6
30008056:	2212      	movs	r2, #18
30008058:	4618      	mov	r0, r3
3000805a:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000805e:	e7af      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
30008060:	2307      	movs	r3, #7
30008062:	2206      	movs	r2, #6
30008064:	4618      	mov	r0, r3
30008066:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000806a:	e7a9      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
3000806c:	4a15      	ldr	r2, [pc, #84]	; (300080c4 <PSRAM_INFO_Update+0x160>)
3000806e:	4293      	cmp	r3, r2
30008070:	d8ee      	bhi.n	30008050 <PSRAM_INFO_Update+0xec>
30008072:	2308      	movs	r3, #8
30008074:	221c      	movs	r2, #28
30008076:	4618      	mov	r0, r3
30008078:	e9c4 3206 	strd	r3, r2, [r4, #24]
3000807c:	e7a0      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
3000807e:	230a      	movs	r3, #10
30008080:	2206      	movs	r2, #6
30008082:	4618      	mov	r0, r3
30008084:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008088:	e79a      	b.n	30007fc0 <PSRAM_INFO_Update+0x5c>
3000808a:	bf00      	nop
3000808c:	42008000 	.word	0x42008000
30008090:	3000e014 	.word	0x3000e014
30008094:	000f4240 	.word	0x000f4240
30008098:	3000ed40 	.word	0x3000ed40
3000809c:	300071bd 	.word	0x300071bd
300080a0:	30007185 	.word	0x30007185
300080a4:	3b9aca00 	.word	0x3b9aca00
300080a8:	0632ea00 	.word	0x0632ea00
300080ac:	3000c9d0 	.word	0x3000c9d0
300080b0:	23020000 	.word	0x23020000
300080b4:	3000c9d8 	.word	0x3000c9d8
300080b8:	07ed6b40 	.word	0x07ed6b40
300080bc:	09e4f580 	.word	0x09e4f580
300080c0:	0bebc200 	.word	0x0bebc200
300080c4:	0ee6b280 	.word	0x0ee6b280

300080c8 <PSRAM_PHY_StructInit>:
300080c8:	2203      	movs	r2, #3
300080ca:	2301      	movs	r3, #1
300080cc:	210a      	movs	r1, #10
300080ce:	b410      	push	{r4}
300080d0:	e9c0 2202 	strd	r2, r2, [r0, #8]
300080d4:	2402      	movs	r4, #2
300080d6:	2207      	movs	r2, #7
300080d8:	e9c0 3400 	strd	r3, r4, [r0]
300080dc:	e9c0 1304 	strd	r1, r3, [r0, #16]
300080e0:	f85d 4b04 	ldr.w	r4, [sp], #4
300080e4:	6182      	str	r2, [r0, #24]
300080e6:	4770      	bx	lr

300080e8 <PSRAM_PHY_Init>:
300080e8:	b538      	push	{r3, r4, r5, lr}
300080ea:	4604      	mov	r4, r0
300080ec:	6903      	ldr	r3, [r0, #16]
300080ee:	4d14      	ldr	r5, [pc, #80]	; (30008140 <PSRAM_PHY_Init+0x58>)
300080f0:	f003 031f 	and.w	r3, r3, #31
300080f4:	4913      	ldr	r1, [pc, #76]	; (30008144 <PSRAM_PHY_Init+0x5c>)
300080f6:	e9d0 0200 	ldrd	r0, r2, [r0]
300080fa:	0612      	lsls	r2, r2, #24
300080fc:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30008100:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30008104:	4313      	orrs	r3, r2
30008106:	68a2      	ldr	r2, [r4, #8]
30008108:	0412      	lsls	r2, r2, #16
3000810a:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000810e:	4313      	orrs	r3, r2
30008110:	68e2      	ldr	r2, [r4, #12]
30008112:	0212      	lsls	r2, r2, #8
30008114:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30008118:	4313      	orrs	r3, r2
3000811a:	606b      	str	r3, [r5, #4]
3000811c:	4788      	blx	r1
3000811e:	2801      	cmp	r0, #1
30008120:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30008124:	bf08      	it	eq
30008126:	4a08      	ldreq	r2, [pc, #32]	; (30008148 <PSRAM_PHY_Init+0x60>)
30008128:	ea4f 1303 	mov.w	r3, r3, lsl #4
3000812c:	f001 010f 	and.w	r1, r1, #15
30008130:	bf18      	it	ne
30008132:	4a06      	ldrne	r2, [pc, #24]	; (3000814c <PSRAM_PHY_Init+0x64>)
30008134:	b2db      	uxtb	r3, r3
30008136:	430b      	orrs	r3, r1
30008138:	431a      	orrs	r2, r3
3000813a:	61aa      	str	r2, [r5, #24]
3000813c:	bd38      	pop	{r3, r4, r5, pc}
3000813e:	bf00      	nop
30008140:	41002000 	.word	0x41002000
30008144:	300071bd 	.word	0x300071bd
30008148:	80000e00 	.word	0x80000e00
3000814c:	80000f00 	.word	0x80000f00

30008150 <PSRAM_CTRL_Init>:
30008150:	b470      	push	{r4, r5, r6}
30008152:	4b4c      	ldr	r3, [pc, #304]	; (30008284 <PSRAM_CTRL_Init+0x134>)
30008154:	2400      	movs	r4, #0
30008156:	4a4c      	ldr	r2, [pc, #304]	; (30008288 <PSRAM_CTRL_Init+0x138>)
30008158:	f24d 0101 	movw	r1, #53249	; 0xd001
3000815c:	609c      	str	r4, [r3, #8]
3000815e:	2001      	movs	r0, #1
30008160:	601a      	str	r2, [r3, #0]
30008162:	4a4a      	ldr	r2, [pc, #296]	; (3000828c <PSRAM_CTRL_Init+0x13c>)
30008164:	6158      	str	r0, [r3, #20]
30008166:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
3000816a:	6811      	ldr	r1, [r2, #0]
3000816c:	2902      	cmp	r1, #2
3000816e:	d04a      	beq.n	30008206 <PSRAM_CTRL_Init+0xb6>
30008170:	6954      	ldr	r4, [r2, #20]
30008172:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30008176:	69d0      	ldr	r0, [r2, #28]
30008178:	6915      	ldr	r5, [r2, #16]
3000817a:	fbb0 f0f4 	udiv	r0, r0, r4
3000817e:	fbb1 f1f4 	udiv	r1, r1, r4
30008182:	6994      	ldr	r4, [r2, #24]
30008184:	f005 020f 	and.w	r2, r5, #15
30008188:	f241 5551 	movw	r5, #5457	; 0x1551
3000818c:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30008190:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30008194:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008198:	094a      	lsrs	r2, r1, #5
3000819a:	0181      	lsls	r1, r0, #6
3000819c:	f000 003f 	and.w	r0, r0, #63	; 0x3f
300081a0:	4d3b      	ldr	r5, [pc, #236]	; (30008290 <PSRAM_CTRL_Init+0x140>)
300081a2:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
300081a6:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
300081aa:	00a2      	lsls	r2, r4, #2
300081ac:	4301      	orrs	r1, r0
300081ae:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
300081b2:	3a01      	subs	r2, #1
300081b4:	430d      	orrs	r5, r1
300081b6:	f242 0120 	movw	r1, #8224	; 0x2020
300081ba:	f3c2 020b 	ubfx	r2, r2, #0, #12
300081be:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
300081c2:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
300081c6:	f44f 2180 	mov.w	r1, #262144	; 0x40000
300081ca:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
300081ce:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
300081d2:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300081d6:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
300081da:	4420      	add	r0, r4
300081dc:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
300081e0:	f640 71fe 	movw	r1, #4094	; 0xffe
300081e4:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
300081e8:	ea01 0140 	and.w	r1, r1, r0, lsl #1
300081ec:	432a      	orrs	r2, r5
300081ee:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300081f2:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300081f6:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
300081fa:	4a22      	ldr	r2, [pc, #136]	; (30008284 <PSRAM_CTRL_Init+0x134>)
300081fc:	6a93      	ldr	r3, [r2, #40]	; 0x28
300081fe:	061b      	lsls	r3, r3, #24
30008200:	d5fc      	bpl.n	300081fc <PSRAM_CTRL_Init+0xac>
30008202:	bc70      	pop	{r4, r5, r6}
30008204:	4770      	bx	lr
30008206:	6955      	ldr	r5, [r2, #20]
30008208:	207d      	movs	r0, #125	; 0x7d
3000820a:	69d1      	ldr	r1, [r2, #28]
3000820c:	26a0      	movs	r6, #160	; 0xa0
3000820e:	6914      	ldr	r4, [r2, #16]
30008210:	6992      	ldr	r2, [r2, #24]
30008212:	f004 040f 	and.w	r4, r4, #15
30008216:	0092      	lsls	r2, r2, #2
30008218:	fbb1 f1f5 	udiv	r1, r1, r5
3000821c:	fbb0 f5f5 	udiv	r5, r0, r5
30008220:	f245 5051 	movw	r0, #21841	; 0x5551
30008224:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30008228:	f001 003f 	and.w	r0, r1, #63	; 0x3f
3000822c:	0189      	lsls	r1, r1, #6
3000822e:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
30008232:	f641 4570 	movw	r5, #7280	; 0x1c70
30008236:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
3000823a:	4325      	orrs	r5, r4
3000823c:	2420      	movs	r4, #32
3000823e:	4308      	orrs	r0, r1
30008240:	1e51      	subs	r1, r2, #1
30008242:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30008246:	3a02      	subs	r2, #2
30008248:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
3000824c:	f3c1 010b 	ubfx	r1, r1, #0, #12
30008250:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008254:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30008258:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
3000825c:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30008260:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30008264:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30008268:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
3000826c:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30008270:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30008274:	4301      	orrs	r1, r0
30008276:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
3000827a:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000827e:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30008282:	e7ba      	b.n	300081fa <PSRAM_CTRL_Init+0xaa>
30008284:	41012000 	.word	0x41012000
30008288:	00bfe000 	.word	0x00bfe000
3000828c:	3000ed40 	.word	0x3000ed40
30008290:	00107000 	.word	0x00107000

30008294 <PSRAM_REG_Write>:
30008294:	b430      	push	{r4, r5}
30008296:	4c20      	ldr	r4, [pc, #128]	; (30008318 <PSRAM_REG_Write+0x84>)
30008298:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000829a:	f013 0301 	ands.w	r3, r3, #1
3000829e:	d1fb      	bne.n	30008298 <PSRAM_REG_Write+0x4>
300082a0:	6825      	ldr	r5, [r4, #0]
300082a2:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
300082a6:	6025      	str	r5, [r4, #0]
300082a8:	60a3      	str	r3, [r4, #8]
300082aa:	6825      	ldr	r5, [r4, #0]
300082ac:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300082b0:	6025      	str	r5, [r4, #0]
300082b2:	6063      	str	r3, [r4, #4]
300082b4:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
300082b8:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
300082bc:	23c0      	movs	r3, #192	; 0xc0
300082be:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300082c2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300082c6:	0e03      	lsrs	r3, r0, #24
300082c8:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300082cc:	f3c0 4307 	ubfx	r3, r0, #16, #8
300082d0:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300082d4:	f3c0 2307 	ubfx	r3, r0, #8, #8
300082d8:	b2c0      	uxtb	r0, r0
300082da:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300082de:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300082e2:	b141      	cbz	r1, 300082f6 <PSRAM_REG_Write+0x62>
300082e4:	3a01      	subs	r2, #1
300082e6:	480c      	ldr	r0, [pc, #48]	; (30008318 <PSRAM_REG_Write+0x84>)
300082e8:	4411      	add	r1, r2
300082ea:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300082ee:	428a      	cmp	r2, r1
300082f0:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300082f4:	d1f9      	bne.n	300082ea <PSRAM_REG_Write+0x56>
300082f6:	4a08      	ldr	r2, [pc, #32]	; (30008318 <PSRAM_REG_Write+0x84>)
300082f8:	2301      	movs	r3, #1
300082fa:	6093      	str	r3, [r2, #8]
300082fc:	6893      	ldr	r3, [r2, #8]
300082fe:	07d9      	lsls	r1, r3, #31
30008300:	d4fc      	bmi.n	300082fc <PSRAM_REG_Write+0x68>
30008302:	4a05      	ldr	r2, [pc, #20]	; (30008318 <PSRAM_REG_Write+0x84>)
30008304:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008306:	07db      	lsls	r3, r3, #31
30008308:	d4fc      	bmi.n	30008304 <PSRAM_REG_Write+0x70>
3000830a:	6813      	ldr	r3, [r2, #0]
3000830c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008310:	6013      	str	r3, [r2, #0]
30008312:	bc30      	pop	{r4, r5}
30008314:	4770      	bx	lr
30008316:	bf00      	nop
30008318:	41012000 	.word	0x41012000

3000831c <PSRAM_APM_DEVIC_Init>:
3000831c:	4b12      	ldr	r3, [pc, #72]	; (30008368 <PSRAM_APM_DEVIC_Init+0x4c>)
3000831e:	2102      	movs	r1, #2
30008320:	2000      	movs	r0, #0
30008322:	b530      	push	{r4, r5, lr}
30008324:	4c11      	ldr	r4, [pc, #68]	; (3000836c <PSRAM_APM_DEVIC_Init+0x50>)
30008326:	b083      	sub	sp, #12
30008328:	699d      	ldr	r5, [r3, #24]
3000832a:	466a      	mov	r2, sp
3000832c:	eb04 0c05 	add.w	ip, r4, r5
30008330:	1eeb      	subs	r3, r5, #3
30008332:	4c0f      	ldr	r4, [pc, #60]	; (30008370 <PSRAM_APM_DEVIC_Init+0x54>)
30008334:	408b      	lsls	r3, r1
30008336:	f81c cc03 	ldrb.w	ip, [ip, #-3]
3000833a:	f003 031c 	and.w	r3, r3, #28
3000833e:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30008342:	f043 0320 	orr.w	r3, r3, #32
30008346:	fa5f fc8c 	uxtb.w	ip, ip
3000834a:	f88d 3000 	strb.w	r3, [sp]
3000834e:	f88d c004 	strb.w	ip, [sp, #4]
30008352:	f88d 3001 	strb.w	r3, [sp, #1]
30008356:	f88d c005 	strb.w	ip, [sp, #5]
3000835a:	47a0      	blx	r4
3000835c:	aa01      	add	r2, sp, #4
3000835e:	2102      	movs	r1, #2
30008360:	2004      	movs	r0, #4
30008362:	47a0      	blx	r4
30008364:	b003      	add	sp, #12
30008366:	bd30      	pop	{r4, r5, pc}
30008368:	3000ed40 	.word	0x3000ed40
3000836c:	3000e1dc 	.word	0x3000e1dc
30008370:	30008295 	.word	0x30008295

30008374 <PSRAM_WB_REG_Write>:
30008374:	b430      	push	{r4, r5}
30008376:	4c1e      	ldr	r4, [pc, #120]	; (300083f0 <PSRAM_WB_REG_Write+0x7c>)
30008378:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000837a:	f013 0301 	ands.w	r3, r3, #1
3000837e:	d1fb      	bne.n	30008378 <PSRAM_WB_REG_Write+0x4>
30008380:	6825      	ldr	r5, [r4, #0]
30008382:	b2c0      	uxtb	r0, r0
30008384:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008388:	6025      	str	r5, [r4, #0]
3000838a:	60a3      	str	r3, [r4, #8]
3000838c:	6825      	ldr	r5, [r4, #0]
3000838e:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008392:	6025      	str	r5, [r4, #0]
30008394:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30008398:	6063      	str	r3, [r4, #4]
3000839a:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
3000839e:	2560      	movs	r5, #96	; 0x60
300083a0:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300083a4:	2501      	movs	r5, #1
300083a6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300083aa:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300083ae:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300083b2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300083b6:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300083ba:	b141      	cbz	r1, 300083ce <PSRAM_WB_REG_Write+0x5a>
300083bc:	3a01      	subs	r2, #1
300083be:	480c      	ldr	r0, [pc, #48]	; (300083f0 <PSRAM_WB_REG_Write+0x7c>)
300083c0:	4411      	add	r1, r2
300083c2:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300083c6:	428a      	cmp	r2, r1
300083c8:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300083cc:	d1f9      	bne.n	300083c2 <PSRAM_WB_REG_Write+0x4e>
300083ce:	4a08      	ldr	r2, [pc, #32]	; (300083f0 <PSRAM_WB_REG_Write+0x7c>)
300083d0:	2301      	movs	r3, #1
300083d2:	6093      	str	r3, [r2, #8]
300083d4:	6893      	ldr	r3, [r2, #8]
300083d6:	07d9      	lsls	r1, r3, #31
300083d8:	d4fc      	bmi.n	300083d4 <PSRAM_WB_REG_Write+0x60>
300083da:	4a05      	ldr	r2, [pc, #20]	; (300083f0 <PSRAM_WB_REG_Write+0x7c>)
300083dc:	6a93      	ldr	r3, [r2, #40]	; 0x28
300083de:	07db      	lsls	r3, r3, #31
300083e0:	d4fc      	bmi.n	300083dc <PSRAM_WB_REG_Write+0x68>
300083e2:	6813      	ldr	r3, [r2, #0]
300083e4:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300083e8:	6013      	str	r3, [r2, #0]
300083ea:	bc30      	pop	{r4, r5}
300083ec:	4770      	bx	lr
300083ee:	bf00      	nop
300083f0:	41012000 	.word	0x41012000

300083f4 <PSRAM_WB_DEVIC_Init>:
300083f4:	490b      	ldr	r1, [pc, #44]	; (30008424 <PSRAM_WB_DEVIC_Init+0x30>)
300083f6:	4b0c      	ldr	r3, [pc, #48]	; (30008428 <PSRAM_WB_DEVIC_Init+0x34>)
300083f8:	b510      	push	{r4, lr}
300083fa:	6988      	ldr	r0, [r1, #24]
300083fc:	b082      	sub	sp, #8
300083fe:	248f      	movs	r4, #143	; 0x8f
30008400:	2102      	movs	r1, #2
30008402:	4403      	add	r3, r0
30008404:	aa01      	add	r2, sp, #4
30008406:	f88d 4004 	strb.w	r4, [sp, #4]
3000840a:	2000      	movs	r0, #0
3000840c:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30008410:	4c06      	ldr	r4, [pc, #24]	; (3000842c <PSRAM_WB_DEVIC_Init+0x38>)
30008412:	011b      	lsls	r3, r3, #4
30008414:	f043 030c 	orr.w	r3, r3, #12
30008418:	f88d 3005 	strb.w	r3, [sp, #5]
3000841c:	47a0      	blx	r4
3000841e:	b002      	add	sp, #8
30008420:	bd10      	pop	{r4, pc}
30008422:	bf00      	nop
30008424:	3000ed40 	.word	0x3000ed40
30008428:	3000e1fc 	.word	0x3000e1fc
3000842c:	30008375 	.word	0x30008375

30008430 <PSRAM_calibration>:
30008430:	4a9c      	ldr	r2, [pc, #624]	; (300086a4 <PSRAM_calibration+0x274>)
30008432:	2300      	movs	r3, #0
30008434:	499c      	ldr	r1, [pc, #624]	; (300086a8 <PSRAM_calibration+0x278>)
30008436:	489d      	ldr	r0, [pc, #628]	; (300086ac <PSRAM_calibration+0x27c>)
30008438:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000843c:	6852      	ldr	r2, [r2, #4]
3000843e:	b093      	sub	sp, #76	; 0x4c
30008440:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
30008444:	400a      	ands	r2, r1
30008446:	9311      	str	r3, [sp, #68]	; 0x44
30008448:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
3000844c:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
30008450:	920a      	str	r2, [sp, #40]	; 0x28
30008452:	6942      	ldr	r2, [r0, #20]
30008454:	03d1      	lsls	r1, r2, #15
30008456:	d51e      	bpl.n	30008496 <PSRAM_calibration+0x66>
30008458:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000845c:	f3bf 8f4f 	dsb	sy
30008460:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30008464:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30008468:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000846c:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30008470:	0164      	lsls	r4, r4, #5
30008472:	ea04 0106 	and.w	r1, r4, r6
30008476:	462b      	mov	r3, r5
30008478:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000847c:	3b01      	subs	r3, #1
3000847e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30008482:	1c5a      	adds	r2, r3, #1
30008484:	d1f8      	bne.n	30008478 <PSRAM_calibration+0x48>
30008486:	3c20      	subs	r4, #32
30008488:	f114 0f20 	cmn.w	r4, #32
3000848c:	d1f1      	bne.n	30008472 <PSRAM_calibration+0x42>
3000848e:	f3bf 8f4f 	dsb	sy
30008492:	f3bf 8f6f 	isb	sy
30008496:	4885      	ldr	r0, [pc, #532]	; (300086ac <PSRAM_calibration+0x27c>)
30008498:	2300      	movs	r3, #0
3000849a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000849e:	f3bf 8f4f 	dsb	sy
300084a2:	6943      	ldr	r3, [r0, #20]
300084a4:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300084a8:	6143      	str	r3, [r0, #20]
300084aa:	f3bf 8f4f 	dsb	sy
300084ae:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300084b2:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300084b6:	f3c5 344e 	ubfx	r4, r5, #13, #15
300084ba:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300084be:	0164      	lsls	r4, r4, #5
300084c0:	ea04 0106 	and.w	r1, r4, r6
300084c4:	462b      	mov	r3, r5
300084c6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300084ca:	3b01      	subs	r3, #1
300084cc:	1c5f      	adds	r7, r3, #1
300084ce:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300084d2:	d1f8      	bne.n	300084c6 <PSRAM_calibration+0x96>
300084d4:	3c20      	subs	r4, #32
300084d6:	f114 0f20 	cmn.w	r4, #32
300084da:	d1f1      	bne.n	300084c0 <PSRAM_calibration+0x90>
300084dc:	f3bf 8f4f 	dsb	sy
300084e0:	f3bf 8f6f 	isb	sy
300084e4:	2204      	movs	r2, #4
300084e6:	461e      	mov	r6, r3
300084e8:	9306      	str	r3, [sp, #24]
300084ea:	2700      	movs	r7, #0
300084ec:	9209      	str	r2, [sp, #36]	; 0x24
300084ee:	46b3      	mov	fp, r6
300084f0:	4a6c      	ldr	r2, [pc, #432]	; (300086a4 <PSRAM_calibration+0x274>)
300084f2:	4c6f      	ldr	r4, [pc, #444]	; (300086b0 <PSRAM_calibration+0x280>)
300084f4:	6813      	ldr	r3, [r2, #0]
300084f6:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 300086cc <PSRAM_calibration+0x29c>
300084fa:	f023 0301 	bic.w	r3, r3, #1
300084fe:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 300086d0 <PSRAM_calibration+0x2a0>
30008502:	6013      	str	r3, [r2, #0]
30008504:	2301      	movs	r3, #1
30008506:	9305      	str	r3, [sp, #20]
30008508:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000850a:	9a05      	ldr	r2, [sp, #20]
3000850c:	9704      	str	r7, [sp, #16]
3000850e:	2700      	movs	r7, #0
30008510:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30008514:	463d      	mov	r5, r7
30008516:	e9cd b607 	strd	fp, r6, [sp, #28]
3000851a:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
3000851e:	465e      	mov	r6, fp
30008520:	e010      	b.n	30008544 <PSRAM_calibration+0x114>
30008522:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
30008526:	bf08      	it	eq
30008528:	462e      	moveq	r6, r5
3000852a:	2d1f      	cmp	r5, #31
3000852c:	eb06 0b07 	add.w	fp, r6, r7
30008530:	f107 0701 	add.w	r7, r7, #1
30008534:	d102      	bne.n	3000853c <PSRAM_calibration+0x10c>
30008536:	9b04      	ldr	r3, [sp, #16]
30008538:	429f      	cmp	r7, r3
3000853a:	dc46      	bgt.n	300085ca <PSRAM_calibration+0x19a>
3000853c:	3501      	adds	r5, #1
3000853e:	2d20      	cmp	r5, #32
30008540:	f000 80ac 	beq.w	3000869c <PSRAM_calibration+0x26c>
30008544:	ea48 0205 	orr.w	r2, r8, r5
30008548:	4b56      	ldr	r3, [pc, #344]	; (300086a4 <PSRAM_calibration+0x274>)
3000854a:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
3000854e:	4959      	ldr	r1, [pc, #356]	; (300086b4 <PSRAM_calibration+0x284>)
30008550:	605a      	str	r2, [r3, #4]
30008552:	6822      	ldr	r2, [r4, #0]
30008554:	4b58      	ldr	r3, [pc, #352]	; (300086b8 <PSRAM_calibration+0x288>)
30008556:	f8ce 2000 	str.w	r2, [lr]
3000855a:	6860      	ldr	r0, [r4, #4]
3000855c:	4a57      	ldr	r2, [pc, #348]	; (300086bc <PSRAM_calibration+0x28c>)
3000855e:	f8c9 0000 	str.w	r0, [r9]
30008562:	68a0      	ldr	r0, [r4, #8]
30008564:	f8ca 0000 	str.w	r0, [sl]
30008568:	68e0      	ldr	r0, [r4, #12]
3000856a:	6018      	str	r0, [r3, #0]
3000856c:	6920      	ldr	r0, [r4, #16]
3000856e:	6008      	str	r0, [r1, #0]
30008570:	6960      	ldr	r0, [r4, #20]
30008572:	6010      	str	r0, [r2, #0]
30008574:	f8de 0000 	ldr.w	r0, [lr]
30008578:	900c      	str	r0, [sp, #48]	; 0x30
3000857a:	f8d9 0000 	ldr.w	r0, [r9]
3000857e:	900d      	str	r0, [sp, #52]	; 0x34
30008580:	f8da 0000 	ldr.w	r0, [sl]
30008584:	900e      	str	r0, [sp, #56]	; 0x38
30008586:	6818      	ldr	r0, [r3, #0]
30008588:	4b4d      	ldr	r3, [pc, #308]	; (300086c0 <PSRAM_calibration+0x290>)
3000858a:	900f      	str	r0, [sp, #60]	; 0x3c
3000858c:	a80c      	add	r0, sp, #48	; 0x30
3000858e:	6809      	ldr	r1, [r1, #0]
30008590:	9110      	str	r1, [sp, #64]	; 0x40
30008592:	6812      	ldr	r2, [r2, #0]
30008594:	4946      	ldr	r1, [pc, #280]	; (300086b0 <PSRAM_calibration+0x280>)
30008596:	9211      	str	r2, [sp, #68]	; 0x44
30008598:	2218      	movs	r2, #24
3000859a:	4798      	blx	r3
3000859c:	2800      	cmp	r0, #0
3000859e:	d0c0      	beq.n	30008522 <PSRAM_calibration+0xf2>
300085a0:	1c70      	adds	r0, r6, #1
300085a2:	d0cb      	beq.n	3000853c <PSRAM_calibration+0x10c>
300085a4:	9b04      	ldr	r3, [sp, #16]
300085a6:	42bb      	cmp	r3, r7
300085a8:	da0a      	bge.n	300085c0 <PSRAM_calibration+0x190>
300085aa:	9b05      	ldr	r3, [sp, #20]
300085ac:	f8cd b01c 	str.w	fp, [sp, #28]
300085b0:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300085b4:	9704      	str	r7, [sp, #16]
300085b6:	2700      	movs	r7, #0
300085b8:	9608      	str	r6, [sp, #32]
300085ba:	465e      	mov	r6, fp
300085bc:	9306      	str	r3, [sp, #24]
300085be:	e7bd      	b.n	3000853c <PSRAM_calibration+0x10c>
300085c0:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300085c4:	2700      	movs	r7, #0
300085c6:	465e      	mov	r6, fp
300085c8:	e7b8      	b.n	3000853c <PSRAM_calibration+0x10c>
300085ca:	9b05      	ldr	r3, [sp, #20]
300085cc:	9306      	str	r3, [sp, #24]
300085ce:	9b05      	ldr	r3, [sp, #20]
300085d0:	005b      	lsls	r3, r3, #1
300085d2:	9305      	str	r3, [sp, #20]
300085d4:	9b09      	ldr	r3, [sp, #36]	; 0x24
300085d6:	3b01      	subs	r3, #1
300085d8:	9309      	str	r3, [sp, #36]	; 0x24
300085da:	d195      	bne.n	30008508 <PSRAM_calibration+0xd8>
300085dc:	9b06      	ldr	r3, [sp, #24]
300085de:	2004      	movs	r0, #4
300085e0:	4a38      	ldr	r2, [pc, #224]	; (300086c4 <PSRAM_calibration+0x294>)
300085e2:	e9cd 7302 	strd	r7, r3, [sp, #8]
300085e6:	4b38      	ldr	r3, [pc, #224]	; (300086c8 <PSRAM_calibration+0x298>)
300085e8:	e9cd 6b00 	strd	r6, fp, [sp]
300085ec:	4619      	mov	r1, r3
300085ee:	f002 fe1d 	bl	3000b22c <rtk_log_write>
300085f2:	482e      	ldr	r0, [pc, #184]	; (300086ac <PSRAM_calibration+0x27c>)
300085f4:	6943      	ldr	r3, [r0, #20]
300085f6:	03d9      	lsls	r1, r3, #15
300085f8:	d425      	bmi.n	30008646 <PSRAM_calibration+0x216>
300085fa:	9b09      	ldr	r3, [sp, #36]	; 0x24
300085fc:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008600:	f3bf 8f4f 	dsb	sy
30008604:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30008608:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
3000860c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30008610:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30008614:	0164      	lsls	r4, r4, #5
30008616:	ea04 010c 	and.w	r1, r4, ip
3000861a:	462b      	mov	r3, r5
3000861c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30008620:	3b01      	subs	r3, #1
30008622:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30008626:	1c5a      	adds	r2, r3, #1
30008628:	d1f8      	bne.n	3000861c <PSRAM_calibration+0x1ec>
3000862a:	3c20      	subs	r4, #32
3000862c:	f114 0f20 	cmn.w	r4, #32
30008630:	d1f1      	bne.n	30008616 <PSRAM_calibration+0x1e6>
30008632:	f3bf 8f4f 	dsb	sy
30008636:	6943      	ldr	r3, [r0, #20]
30008638:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
3000863c:	6143      	str	r3, [r0, #20]
3000863e:	f3bf 8f4f 	dsb	sy
30008642:	f3bf 8f6f 	isb	sy
30008646:	2f08      	cmp	r7, #8
30008648:	dd24      	ble.n	30008694 <PSRAM_calibration+0x264>
3000864a:	ebbb 0306 	subs.w	r3, fp, r6
3000864e:	9806      	ldr	r0, [sp, #24]
30008650:	bf48      	it	mi
30008652:	3301      	addmi	r3, #1
30008654:	eb16 060b 	adds.w	r6, r6, fp
30008658:	ea4f 0363 	mov.w	r3, r3, asr #1
3000865c:	bf48      	it	mi
3000865e:	3601      	addmi	r6, #1
30008660:	1e9a      	subs	r2, r3, #2
30008662:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30008664:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30008668:	0414      	lsls	r4, r2, #16
3000866a:	f3c6 0344 	ubfx	r3, r6, #1, #5
3000866e:	0212      	lsls	r2, r2, #8
30008670:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30008674:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30008678:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000867c:	2001      	movs	r0, #1
3000867e:	430b      	orrs	r3, r1
30008680:	4908      	ldr	r1, [pc, #32]	; (300086a4 <PSRAM_calibration+0x274>)
30008682:	4323      	orrs	r3, r4
30008684:	4313      	orrs	r3, r2
30008686:	604b      	str	r3, [r1, #4]
30008688:	680b      	ldr	r3, [r1, #0]
3000868a:	4303      	orrs	r3, r0
3000868c:	600b      	str	r3, [r1, #0]
3000868e:	b013      	add	sp, #76	; 0x4c
30008690:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008694:	2000      	movs	r0, #0
30008696:	b013      	add	sp, #76	; 0x4c
30008698:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000869c:	9f04      	ldr	r7, [sp, #16]
3000869e:	e9dd b607 	ldrd	fp, r6, [sp, #28]
300086a2:	e794      	b.n	300085ce <PSRAM_calibration+0x19e>
300086a4:	41002000 	.word	0x41002000
300086a8:	0fffffe0 	.word	0x0fffffe0
300086ac:	e000ed00 	.word	0xe000ed00
300086b0:	3000e1e4 	.word	0x3000e1e4
300086b4:	60200000 	.word	0x60200000
300086b8:	60150000 	.word	0x60150000
300086bc:	60250000 	.word	0x60250000
300086c0:	000129bd 	.word	0x000129bd
300086c4:	3000c9f8 	.word	0x3000c9f8
300086c8:	3000c9d0 	.word	0x3000c9d0
300086cc:	60050000 	.word	0x60050000
300086d0:	60100000 	.word	0x60100000

300086d4 <PSRAM_AutoGating>:
300086d4:	b1c0      	cbz	r0, 30008708 <PSRAM_AutoGating+0x34>
300086d6:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300086da:	4810      	ldr	r0, [pc, #64]	; (3000871c <PSRAM_AutoGating+0x48>)
300086dc:	f3c1 0108 	ubfx	r1, r1, #0, #9
300086e0:	0252      	lsls	r2, r2, #9
300086e2:	b410      	push	{r4}
300086e4:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
300086e8:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
300086ec:	4020      	ands	r0, r4
300086ee:	4301      	orrs	r1, r0
300086f0:	430a      	orrs	r2, r1
300086f2:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
300086f6:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
300086fa:	f042 0201 	orr.w	r2, r2, #1
300086fe:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30008702:	f85d 4b04 	ldr.w	r4, [sp], #4
30008706:	4770      	bx	lr
30008708:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000870c:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30008710:	f023 0301 	bic.w	r3, r3, #1
30008714:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30008718:	4770      	bx	lr
3000871a:	bf00      	nop
3000871c:	ffffc000 	.word	0xffffc000

30008720 <BOOT_Reason>:
30008720:	4b02      	ldr	r3, [pc, #8]	; (3000872c <BOOT_Reason+0xc>)
30008722:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30008726:	b280      	uxth	r0, r0
30008728:	4770      	bx	lr
3000872a:	bf00      	nop
3000872c:	42008200 	.word	0x42008200

30008730 <CRYPTO_SetSecurityModeAD>:
30008730:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008734:	b083      	sub	sp, #12
30008736:	460d      	mov	r5, r1
30008738:	4616      	mov	r6, r2
3000873a:	4699      	mov	r9, r3
3000873c:	4604      	mov	r4, r0
3000873e:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
30008742:	2800      	cmp	r0, #0
30008744:	f000 80a9 	beq.w	3000889a <CRYPTO_SetSecurityModeAD+0x16a>
30008748:	7823      	ldrb	r3, [r4, #0]
3000874a:	2b01      	cmp	r3, #1
3000874c:	d003      	beq.n	30008756 <CRYPTO_SetSecurityModeAD+0x26>
3000874e:	2125      	movs	r1, #37	; 0x25
30008750:	485d      	ldr	r0, [pc, #372]	; (300088c8 <CRYPTO_SetSecurityModeAD+0x198>)
30008752:	f003 f841 	bl	3000b7d8 <__io_assert_failed_veneer>
30008756:	2300      	movs	r3, #0
30008758:	1c6a      	adds	r2, r5, #1
3000875a:	6065      	str	r5, [r4, #4]
3000875c:	8123      	strh	r3, [r4, #8]
3000875e:	d008      	beq.n	30008772 <CRYPTO_SetSecurityModeAD+0x42>
30008760:	f005 0330 	and.w	r3, r5, #48	; 0x30
30008764:	2b20      	cmp	r3, #32
30008766:	d07b      	beq.n	30008860 <CRYPTO_SetSecurityModeAD+0x130>
30008768:	f085 0580 	eor.w	r5, r5, #128	; 0x80
3000876c:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30008770:	7265      	strb	r5, [r4, #9]
30008772:	1c73      	adds	r3, r6, #1
30008774:	60e6      	str	r6, [r4, #12]
30008776:	f000 8082 	beq.w	3000887e <CRYPTO_SetSecurityModeAD+0x14e>
3000877a:	f006 0303 	and.w	r3, r6, #3
3000877e:	f3c6 0280 	ubfx	r2, r6, #2, #1
30008782:	7422      	strb	r2, [r4, #16]
30008784:	f1a3 0202 	sub.w	r2, r3, #2
30008788:	2b01      	cmp	r3, #1
3000878a:	fab2 f282 	clz	r2, r2
3000878e:	ea4f 1252 	mov.w	r2, r2, lsr #5
30008792:	7462      	strb	r2, [r4, #17]
30008794:	fab3 f283 	clz	r2, r3
30008798:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000879c:	74a2      	strb	r2, [r4, #18]
3000879e:	bf0c      	ite	eq
300087a0:	2201      	moveq	r2, #1
300087a2:	2200      	movne	r2, #0
300087a4:	74e2      	strb	r2, [r4, #19]
300087a6:	d13f      	bne.n	30008828 <CRYPTO_SetSecurityModeAD+0xf8>
300087a8:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
300087ac:	2b40      	cmp	r3, #64	; 0x40
300087ae:	f000 8083 	beq.w	300088b8 <CRYPTO_SetSecurityModeAD+0x188>
300087b2:	d86a      	bhi.n	3000888a <CRYPTO_SetSecurityModeAD+0x15a>
300087b4:	2b10      	cmp	r3, #16
300087b6:	d06d      	beq.n	30008894 <CRYPTO_SetSecurityModeAD+0x164>
300087b8:	2b20      	cmp	r3, #32
300087ba:	d173      	bne.n	300088a4 <CRYPTO_SetSecurityModeAD+0x174>
300087bc:	7523      	strb	r3, [r4, #20]
300087be:	62a3      	str	r3, [r4, #40]	; 0x28
300087c0:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300087c2:	6227      	str	r7, [r4, #32]
300087c4:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
300087c8:	2f00      	cmp	r7, #0
300087ca:	d038      	beq.n	3000883e <CRYPTO_SetSecurityModeAD+0x10e>
300087cc:	7d23      	ldrb	r3, [r4, #20]
300087ce:	f104 0064 	add.w	r0, r4, #100	; 0x64
300087d2:	2b30      	cmp	r3, #48	; 0x30
300087d4:	65e0      	str	r0, [r4, #92]	; 0x5c
300087d6:	d046      	beq.n	30008866 <CRYPTO_SetSecurityModeAD+0x136>
300087d8:	2b40      	cmp	r3, #64	; 0x40
300087da:	d044      	beq.n	30008866 <CRYPTO_SetSecurityModeAD+0x136>
300087dc:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
300087e0:	2240      	movs	r2, #64	; 0x40
300087e2:	2136      	movs	r1, #54	; 0x36
300087e4:	4d39      	ldr	r5, [pc, #228]	; (300088cc <CRYPTO_SetSecurityModeAD+0x19c>)
300087e6:	6623      	str	r3, [r4, #96]	; 0x60
300087e8:	47a8      	blx	r5
300087ea:	2240      	movs	r2, #64	; 0x40
300087ec:	215c      	movs	r1, #92	; 0x5c
300087ee:	6e20      	ldr	r0, [r4, #96]	; 0x60
300087f0:	47a8      	blx	r5
300087f2:	4b37      	ldr	r3, [pc, #220]	; (300088d0 <CRYPTO_SetSecurityModeAD+0x1a0>)
300087f4:	681d      	ldr	r5, [r3, #0]
300087f6:	bb25      	cbnz	r5, 30008842 <CRYPTO_SetSecurityModeAD+0x112>
300087f8:	f1b8 0f00 	cmp.w	r8, #0
300087fc:	d05f      	beq.n	300088be <CRYPTO_SetSecurityModeAD+0x18e>
300087fe:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30008802:	e001      	b.n	30008808 <CRYPTO_SetSecurityModeAD+0xd8>
30008804:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30008808:	6de2      	ldr	r2, [r4, #92]	; 0x5c
3000880a:	f818 3005 	ldrb.w	r3, [r8, r5]
3000880e:	5d51      	ldrb	r1, [r2, r5]
30008810:	404b      	eors	r3, r1
30008812:	5553      	strb	r3, [r2, r5]
30008814:	6e22      	ldr	r2, [r4, #96]	; 0x60
30008816:	6a63      	ldr	r3, [r4, #36]	; 0x24
30008818:	5d51      	ldrb	r1, [r2, r5]
3000881a:	5d5b      	ldrb	r3, [r3, r5]
3000881c:	404b      	eors	r3, r1
3000881e:	5553      	strb	r3, [r2, r5]
30008820:	3501      	adds	r5, #1
30008822:	42af      	cmp	r7, r5
30008824:	d1ee      	bne.n	30008804 <CRYPTO_SetSecurityModeAD+0xd4>
30008826:	e00c      	b.n	30008842 <CRYPTO_SetSecurityModeAD+0x112>
30008828:	2200      	movs	r2, #0
3000882a:	7522      	strb	r2, [r4, #20]
3000882c:	b99b      	cbnz	r3, 30008856 <CRYPTO_SetSecurityModeAD+0x126>
3000882e:	2314      	movs	r3, #20
30008830:	6227      	str	r7, [r4, #32]
30008832:	62a3      	str	r3, [r4, #40]	; 0x28
30008834:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008836:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
3000883a:	2f00      	cmp	r7, #0
3000883c:	d1c6      	bne.n	300087cc <CRYPTO_SetSecurityModeAD+0x9c>
3000883e:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30008842:	3601      	adds	r6, #1
30008844:	d003      	beq.n	3000884e <CRYPTO_SetSecurityModeAD+0x11e>
30008846:	2201      	movs	r2, #1
30008848:	2300      	movs	r3, #0
3000884a:	85a2      	strh	r2, [r4, #44]	; 0x2c
3000884c:	6323      	str	r3, [r4, #48]	; 0x30
3000884e:	2000      	movs	r0, #0
30008850:	b003      	add	sp, #12
30008852:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008856:	2b02      	cmp	r3, #2
30008858:	d114      	bne.n	30008884 <CRYPTO_SetSecurityModeAD+0x154>
3000885a:	2310      	movs	r3, #16
3000885c:	62a3      	str	r3, [r4, #40]	; 0x28
3000885e:	e7af      	b.n	300087c0 <CRYPTO_SetSecurityModeAD+0x90>
30008860:	2301      	movs	r3, #1
30008862:	7223      	strb	r3, [r4, #8]
30008864:	e780      	b.n	30008768 <CRYPTO_SetSecurityModeAD+0x38>
30008866:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
3000886a:	2280      	movs	r2, #128	; 0x80
3000886c:	2136      	movs	r1, #54	; 0x36
3000886e:	4d17      	ldr	r5, [pc, #92]	; (300088cc <CRYPTO_SetSecurityModeAD+0x19c>)
30008870:	6623      	str	r3, [r4, #96]	; 0x60
30008872:	47a8      	blx	r5
30008874:	2280      	movs	r2, #128	; 0x80
30008876:	215c      	movs	r1, #92	; 0x5c
30008878:	6e20      	ldr	r0, [r4, #96]	; 0x60
3000887a:	47a8      	blx	r5
3000887c:	e7b9      	b.n	300087f2 <CRYPTO_SetSecurityModeAD+0xc2>
3000887e:	2300      	movs	r3, #0
30008880:	6123      	str	r3, [r4, #16]
30008882:	7523      	strb	r3, [r4, #20]
30008884:	2300      	movs	r3, #0
30008886:	62a3      	str	r3, [r4, #40]	; 0x28
30008888:	e79a      	b.n	300087c0 <CRYPTO_SetSecurityModeAD+0x90>
3000888a:	2b80      	cmp	r3, #128	; 0x80
3000888c:	d10a      	bne.n	300088a4 <CRYPTO_SetSecurityModeAD+0x174>
3000888e:	2340      	movs	r3, #64	; 0x40
30008890:	7523      	strb	r3, [r4, #20]
30008892:	e794      	b.n	300087be <CRYPTO_SetSecurityModeAD+0x8e>
30008894:	231c      	movs	r3, #28
30008896:	7523      	strb	r3, [r4, #20]
30008898:	e7f5      	b.n	30008886 <CRYPTO_SetSecurityModeAD+0x156>
3000889a:	2124      	movs	r1, #36	; 0x24
3000889c:	480a      	ldr	r0, [pc, #40]	; (300088c8 <CRYPTO_SetSecurityModeAD+0x198>)
3000889e:	f002 ff9b 	bl	3000b7d8 <__io_assert_failed_veneer>
300088a2:	e751      	b.n	30008748 <CRYPTO_SetSecurityModeAD+0x18>
300088a4:	4b0b      	ldr	r3, [pc, #44]	; (300088d4 <CRYPTO_SetSecurityModeAD+0x1a4>)
300088a6:	2002      	movs	r0, #2
300088a8:	4a0b      	ldr	r2, [pc, #44]	; (300088d8 <CRYPTO_SetSecurityModeAD+0x1a8>)
300088aa:	4619      	mov	r1, r3
300088ac:	9600      	str	r6, [sp, #0]
300088ae:	f002 fcbd 	bl	3000b22c <rtk_log_write>
300088b2:	2300      	movs	r3, #0
300088b4:	7523      	strb	r3, [r4, #20]
300088b6:	e7e6      	b.n	30008886 <CRYPTO_SetSecurityModeAD+0x156>
300088b8:	2330      	movs	r3, #48	; 0x30
300088ba:	7523      	strb	r3, [r4, #20]
300088bc:	e7e3      	b.n	30008886 <CRYPTO_SetSecurityModeAD+0x156>
300088be:	2180      	movs	r1, #128	; 0x80
300088c0:	4801      	ldr	r0, [pc, #4]	; (300088c8 <CRYPTO_SetSecurityModeAD+0x198>)
300088c2:	f002 ff89 	bl	3000b7d8 <__io_assert_failed_veneer>
300088c6:	e79a      	b.n	300087fe <CRYPTO_SetSecurityModeAD+0xce>
300088c8:	3000ca98 	.word	0x3000ca98
300088cc:	00012be5 	.word	0x00012be5
300088d0:	2001c200 	.word	0x2001c200
300088d4:	3000ca38 	.word	0x3000ca38
300088d8:	3000ca44 	.word	0x3000ca44

300088dc <rtl_crypto_hmac_sha2_init>:
300088dc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300088e0:	4c97      	ldr	r4, [pc, #604]	; (30008b40 <rtl_crypto_hmac_sha2_init+0x264>)
300088e2:	4617      	mov	r7, r2
300088e4:	e844 f400 	tt	r4, r4
300088e8:	4a96      	ldr	r2, [pc, #600]	; (30008b44 <rtl_crypto_hmac_sha2_init+0x268>)
300088ea:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
300088ee:	461c      	mov	r4, r3
300088f0:	4b95      	ldr	r3, [pc, #596]	; (30008b48 <rtl_crypto_hmac_sha2_init+0x26c>)
300088f2:	bf18      	it	ne
300088f4:	4613      	movne	r3, r2
300088f6:	b085      	sub	sp, #20
300088f8:	4605      	mov	r5, r0
300088fa:	460e      	mov	r6, r1
300088fc:	6b5b      	ldr	r3, [r3, #52]	; 0x34
300088fe:	2b00      	cmp	r3, #0
30008900:	db05      	blt.n	3000890e <rtl_crypto_hmac_sha2_init+0x32>
30008902:	2900      	cmp	r1, #0
30008904:	f000 8118 	beq.w	30008b38 <rtl_crypto_hmac_sha2_init+0x25c>
30008908:	078b      	lsls	r3, r1, #30
3000890a:	f040 8112 	bne.w	30008b32 <rtl_crypto_hmac_sha2_init+0x256>
3000890e:	2d20      	cmp	r5, #32
30008910:	bf18      	it	ne
30008912:	2d1c      	cmpne	r5, #28
30008914:	bf0c      	ite	eq
30008916:	f04f 0901 	moveq.w	r9, #1
3000891a:	f04f 0900 	movne.w	r9, #0
3000891e:	d04d      	beq.n	300089bc <rtl_crypto_hmac_sha2_init+0xe0>
30008920:	2d40      	cmp	r5, #64	; 0x40
30008922:	bf18      	it	ne
30008924:	2d30      	cmpne	r5, #48	; 0x30
30008926:	d102      	bne.n	3000892e <rtl_crypto_hmac_sha2_init+0x52>
30008928:	2f80      	cmp	r7, #128	; 0x80
3000892a:	f200 80f9 	bhi.w	30008b20 <rtl_crypto_hmac_sha2_init+0x244>
3000892e:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30008b68 <rtl_crypto_hmac_sha2_init+0x28c>
30008932:	f898 3000 	ldrb.w	r3, [r8]
30008936:	2b01      	cmp	r3, #1
30008938:	d003      	beq.n	30008942 <rtl_crypto_hmac_sha2_init+0x66>
3000893a:	21b4      	movs	r1, #180	; 0xb4
3000893c:	4883      	ldr	r0, [pc, #524]	; (30008b4c <rtl_crypto_hmac_sha2_init+0x270>)
3000893e:	f002 ff4b 	bl	3000b7d8 <__io_assert_failed_veneer>
30008942:	2d30      	cmp	r5, #48	; 0x30
30008944:	f000 809b 	beq.w	30008a7e <rtl_crypto_hmac_sha2_init+0x1a2>
30008948:	d83f      	bhi.n	300089ca <rtl_crypto_hmac_sha2_init+0xee>
3000894a:	2d1c      	cmp	r5, #28
3000894c:	f000 80cb 	beq.w	30008ae6 <rtl_crypto_hmac_sha2_init+0x20a>
30008950:	f1b9 0f00 	cmp.w	r9, #0
30008954:	d078      	beq.n	30008a48 <rtl_crypto_hmac_sha2_init+0x16c>
30008956:	2c00      	cmp	r4, #0
30008958:	f000 808f 	beq.w	30008a7a <rtl_crypto_hmac_sha2_init+0x19e>
3000895c:	4b7c      	ldr	r3, [pc, #496]	; (30008b50 <rtl_crypto_hmac_sha2_init+0x274>)
3000895e:	f8df e210 	ldr.w	lr, [pc, #528]	; 30008b70 <rtl_crypto_hmac_sha2_init+0x294>
30008962:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30008b80 <rtl_crypto_hmac_sha2_init+0x2a4>
30008966:	4a7b      	ldr	r2, [pc, #492]	; (30008b54 <rtl_crypto_hmac_sha2_init+0x278>)
30008968:	e9c4 e302 	strd	lr, r3, [r4, #8]
3000896c:	4b7a      	ldr	r3, [pc, #488]	; (30008b58 <rtl_crypto_hmac_sha2_init+0x27c>)
3000896e:	e9c4 9304 	strd	r9, r3, [r4, #16]
30008972:	4b7a      	ldr	r3, [pc, #488]	; (30008b5c <rtl_crypto_hmac_sha2_init+0x280>)
30008974:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008978:	4b79      	ldr	r3, [pc, #484]	; (30008b60 <rtl_crypto_hmac_sha2_init+0x284>)
3000897a:	2225      	movs	r2, #37	; 0x25
3000897c:	6223      	str	r3, [r4, #32]
3000897e:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30008982:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30008986:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
3000898a:	6263      	str	r3, [r4, #36]	; 0x24
3000898c:	2300      	movs	r3, #0
3000898e:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008992:	4d74      	ldr	r5, [pc, #464]	; (30008b64 <rtl_crypto_hmac_sha2_init+0x288>)
30008994:	9300      	str	r3, [sp, #0]
30008996:	9702      	str	r7, [sp, #8]
30008998:	9601      	str	r6, [sp, #4]
3000899a:	4873      	ldr	r0, [pc, #460]	; (30008b68 <rtl_crypto_hmac_sha2_init+0x28c>)
3000899c:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
300089a0:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
300089a4:	47a8      	blx	r5
300089a6:	4605      	mov	r5, r0
300089a8:	4b70      	ldr	r3, [pc, #448]	; (30008b6c <rtl_crypto_hmac_sha2_init+0x290>)
300089aa:	4620      	mov	r0, r4
300089ac:	4798      	blx	r3
300089ae:	2301      	movs	r3, #1
300089b0:	4628      	mov	r0, r5
300089b2:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
300089b6:	b005      	add	sp, #20
300089b8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300089bc:	2f40      	cmp	r7, #64	; 0x40
300089be:	d9b6      	bls.n	3000892e <rtl_crypto_hmac_sha2_init+0x52>
300089c0:	21ae      	movs	r1, #174	; 0xae
300089c2:	4862      	ldr	r0, [pc, #392]	; (30008b4c <rtl_crypto_hmac_sha2_init+0x270>)
300089c4:	f002 ff08 	bl	3000b7d8 <__io_assert_failed_veneer>
300089c8:	e7b1      	b.n	3000892e <rtl_crypto_hmac_sha2_init+0x52>
300089ca:	2d40      	cmp	r5, #64	; 0x40
300089cc:	d13c      	bne.n	30008a48 <rtl_crypto_hmac_sha2_init+0x16c>
300089ce:	2c00      	cmp	r4, #0
300089d0:	d040      	beq.n	30008a54 <rtl_crypto_hmac_sha2_init+0x178>
300089d2:	4b67      	ldr	r3, [pc, #412]	; (30008b70 <rtl_crypto_hmac_sha2_init+0x294>)
300089d4:	4a67      	ldr	r2, [pc, #412]	; (30008b74 <rtl_crypto_hmac_sha2_init+0x298>)
300089d6:	4968      	ldr	r1, [pc, #416]	; (30008b78 <rtl_crypto_hmac_sha2_init+0x29c>)
300089d8:	4d68      	ldr	r5, [pc, #416]	; (30008b7c <rtl_crypto_hmac_sha2_init+0x2a0>)
300089da:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30008bc0 <rtl_crypto_hmac_sha2_init+0x2e4>
300089de:	e9c4 3202 	strd	r3, r2, [r4, #8]
300089e2:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
300089e6:	2285      	movs	r2, #133	; 0x85
300089e8:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
300089ec:	33d4      	adds	r3, #212	; 0xd4
300089ee:	6163      	str	r3, [r4, #20]
300089f0:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
300089f4:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
300089f8:	f203 734a 	addw	r3, r3, #1866	; 0x74a
300089fc:	6123      	str	r3, [r4, #16]
300089fe:	4b60      	ldr	r3, [pc, #384]	; (30008b80 <rtl_crypto_hmac_sha2_init+0x2a4>)
30008a00:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008a04:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30008a08:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30008a0c:	f203 337f 	addw	r3, r3, #895	; 0x37f
30008a10:	6263      	str	r3, [r4, #36]	; 0x24
30008a12:	4b51      	ldr	r3, [pc, #324]	; (30008b58 <rtl_crypto_hmac_sha2_init+0x27c>)
30008a14:	6223      	str	r3, [r4, #32]
30008a16:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30008a1a:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30008a1e:	f203 5397 	addw	r3, r3, #1431	; 0x597
30008a22:	62e3      	str	r3, [r4, #44]	; 0x2c
30008a24:	4b4b      	ldr	r3, [pc, #300]	; (30008b54 <rtl_crypto_hmac_sha2_init+0x278>)
30008a26:	62a3      	str	r3, [r4, #40]	; 0x28
30008a28:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30008a2c:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30008a30:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30008a34:	6363      	str	r3, [r4, #52]	; 0x34
30008a36:	4b49      	ldr	r3, [pc, #292]	; (30008b5c <rtl_crypto_hmac_sha2_init+0x280>)
30008a38:	6323      	str	r3, [r4, #48]	; 0x30
30008a3a:	4b49      	ldr	r3, [pc, #292]	; (30008b60 <rtl_crypto_hmac_sha2_init+0x284>)
30008a3c:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008a40:	4b50      	ldr	r3, [pc, #320]	; (30008b84 <rtl_crypto_hmac_sha2_init+0x2a8>)
30008a42:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008a46:	e7a1      	b.n	3000898c <rtl_crypto_hmac_sha2_init+0xb0>
30008a48:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
30008a4c:	4628      	mov	r0, r5
30008a4e:	b005      	add	sp, #20
30008a50:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008a54:	2285      	movs	r2, #133	; 0x85
30008a56:	2300      	movs	r3, #0
30008a58:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008a5c:	4c41      	ldr	r4, [pc, #260]	; (30008b64 <rtl_crypto_hmac_sha2_init+0x288>)
30008a5e:	9702      	str	r7, [sp, #8]
30008a60:	4841      	ldr	r0, [pc, #260]	; (30008b68 <rtl_crypto_hmac_sha2_init+0x28c>)
30008a62:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30008a66:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30008a6a:	e9cd 3600 	strd	r3, r6, [sp]
30008a6e:	47a0      	blx	r4
30008a70:	4605      	mov	r5, r0
30008a72:	4628      	mov	r0, r5
30008a74:	b005      	add	sp, #20
30008a76:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008a7a:	2225      	movs	r2, #37	; 0x25
30008a7c:	e7eb      	b.n	30008a56 <rtl_crypto_hmac_sha2_init+0x17a>
30008a7e:	2c00      	cmp	r4, #0
30008a80:	d053      	beq.n	30008b2a <rtl_crypto_hmac_sha2_init+0x24e>
30008a82:	4b41      	ldr	r3, [pc, #260]	; (30008b88 <rtl_crypto_hmac_sha2_init+0x2ac>)
30008a84:	f8df e130 	ldr.w	lr, [pc, #304]	; 30008bb8 <rtl_crypto_hmac_sha2_init+0x2dc>
30008a88:	4a40      	ldr	r2, [pc, #256]	; (30008b8c <rtl_crypto_hmac_sha2_init+0x2b0>)
30008a8a:	4941      	ldr	r1, [pc, #260]	; (30008b90 <rtl_crypto_hmac_sha2_init+0x2b4>)
30008a8c:	4d41      	ldr	r5, [pc, #260]	; (30008b94 <rtl_crypto_hmac_sha2_init+0x2b8>)
30008a8e:	f8df c12c 	ldr.w	ip, [pc, #300]	; 30008bbc <rtl_crypto_hmac_sha2_init+0x2e0>
30008a92:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30008a96:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30008a9a:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30008a9e:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30008aa2:	6163      	str	r3, [r4, #20]
30008aa4:	4b3c      	ldr	r3, [pc, #240]	; (30008b98 <rtl_crypto_hmac_sha2_init+0x2bc>)
30008aa6:	6123      	str	r3, [r4, #16]
30008aa8:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30008aac:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30008ab0:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30008ab4:	61e3      	str	r3, [r4, #28]
30008ab6:	4b39      	ldr	r3, [pc, #228]	; (30008b9c <rtl_crypto_hmac_sha2_init+0x2c0>)
30008ab8:	61a3      	str	r3, [r4, #24]
30008aba:	4b39      	ldr	r3, [pc, #228]	; (30008ba0 <rtl_crypto_hmac_sha2_init+0x2c4>)
30008abc:	e9c4 3208 	strd	r3, r2, [r4, #32]
30008ac0:	4b38      	ldr	r3, [pc, #224]	; (30008ba4 <rtl_crypto_hmac_sha2_init+0x2c8>)
30008ac2:	2245      	movs	r2, #69	; 0x45
30008ac4:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30008ac8:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30008acc:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30008ad0:	33aa      	adds	r3, #170	; 0xaa
30008ad2:	6363      	str	r3, [r4, #52]	; 0x34
30008ad4:	4b34      	ldr	r3, [pc, #208]	; (30008ba8 <rtl_crypto_hmac_sha2_init+0x2cc>)
30008ad6:	6323      	str	r3, [r4, #48]	; 0x30
30008ad8:	4b34      	ldr	r3, [pc, #208]	; (30008bac <rtl_crypto_hmac_sha2_init+0x2d0>)
30008ada:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008ade:	4b34      	ldr	r3, [pc, #208]	; (30008bb0 <rtl_crypto_hmac_sha2_init+0x2d4>)
30008ae0:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008ae4:	e752      	b.n	3000898c <rtl_crypto_hmac_sha2_init+0xb0>
30008ae6:	b314      	cbz	r4, 30008b2e <rtl_crypto_hmac_sha2_init+0x252>
30008ae8:	4b32      	ldr	r3, [pc, #200]	; (30008bb4 <rtl_crypto_hmac_sha2_init+0x2d8>)
30008aea:	2215      	movs	r2, #21
30008aec:	4932      	ldr	r1, [pc, #200]	; (30008bb8 <rtl_crypto_hmac_sha2_init+0x2dc>)
30008aee:	4827      	ldr	r0, [pc, #156]	; (30008b8c <rtl_crypto_hmac_sha2_init+0x2b0>)
30008af0:	4d28      	ldr	r5, [pc, #160]	; (30008b94 <rtl_crypto_hmac_sha2_init+0x2b8>)
30008af2:	e9c4 1302 	strd	r1, r3, [r4, #8]
30008af6:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
30008afa:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
30008afe:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30008b02:	6123      	str	r3, [r4, #16]
30008b04:	4b22      	ldr	r3, [pc, #136]	; (30008b90 <rtl_crypto_hmac_sha2_init+0x2b4>)
30008b06:	e9c4 0305 	strd	r0, r3, [r4, #20]
30008b0a:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30008b0e:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30008b12:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30008b16:	61e3      	str	r3, [r4, #28]
30008b18:	4b28      	ldr	r3, [pc, #160]	; (30008bbc <rtl_crypto_hmac_sha2_init+0x2e0>)
30008b1a:	e9c4 5308 	strd	r5, r3, [r4, #32]
30008b1e:	e735      	b.n	3000898c <rtl_crypto_hmac_sha2_init+0xb0>
30008b20:	21b1      	movs	r1, #177	; 0xb1
30008b22:	480a      	ldr	r0, [pc, #40]	; (30008b4c <rtl_crypto_hmac_sha2_init+0x270>)
30008b24:	f002 fe58 	bl	3000b7d8 <__io_assert_failed_veneer>
30008b28:	e701      	b.n	3000892e <rtl_crypto_hmac_sha2_init+0x52>
30008b2a:	2245      	movs	r2, #69	; 0x45
30008b2c:	e793      	b.n	30008a56 <rtl_crypto_hmac_sha2_init+0x17a>
30008b2e:	2215      	movs	r2, #21
30008b30:	e791      	b.n	30008a56 <rtl_crypto_hmac_sha2_init+0x17a>
30008b32:	f06f 0505 	mvn.w	r5, #5
30008b36:	e789      	b.n	30008a4c <rtl_crypto_hmac_sha2_init+0x170>
30008b38:	f06f 0503 	mvn.w	r5, #3
30008b3c:	e786      	b.n	30008a4c <rtl_crypto_hmac_sha2_init+0x170>
30008b3e:	bf00      	nop
30008b40:	0000e5f9 	.word	0x0000e5f9
30008b44:	500c8000 	.word	0x500c8000
30008b48:	400c8000 	.word	0x400c8000
30008b4c:	3000ca7c 	.word	0x3000ca7c
30008b50:	bb67ae85 	.word	0xbb67ae85
30008b54:	510e527f 	.word	0x510e527f
30008b58:	a54ff53a 	.word	0xa54ff53a
30008b5c:	9b05688c 	.word	0x9b05688c
30008b60:	1f83d9ab 	.word	0x1f83d9ab
30008b64:	30008731 	.word	0x30008731
30008b68:	2001c460 	.word	0x2001c460
30008b6c:	00004c99 	.word	0x00004c99
30008b70:	6a09e667 	.word	0x6a09e667
30008b74:	f3bcc908 	.word	0xf3bcc908
30008b78:	fe94f82b 	.word	0xfe94f82b
30008b7c:	fb41bd6b 	.word	0xfb41bd6b
30008b80:	3c6ef372 	.word	0x3c6ef372
30008b84:	5be0cd19 	.word	0x5be0cd19
30008b88:	cbbb9d5d 	.word	0xcbbb9d5d
30008b8c:	f70e5939 	.word	0xf70e5939
30008b90:	ffc00b31 	.word	0xffc00b31
30008b94:	64f98fa7 	.word	0x64f98fa7
30008b98:	629a292a 	.word	0x629a292a
30008b9c:	9159015a 	.word	0x9159015a
30008ba0:	152fecd8 	.word	0x152fecd8
30008ba4:	67332667 	.word	0x67332667
30008ba8:	8eb44a87 	.word	0x8eb44a87
30008bac:	db0c2e0d 	.word	0xdb0c2e0d
30008bb0:	47b5481d 	.word	0x47b5481d
30008bb4:	367cd507 	.word	0x367cd507
30008bb8:	c1059ed8 	.word	0xc1059ed8
30008bbc:	befa4fa4 	.word	0xbefa4fa4
30008bc0:	137e2179 	.word	0x137e2179

30008bc4 <CPU_ClkGet>:
30008bc4:	b570      	push	{r4, r5, r6, lr}
30008bc6:	4d1e      	ldr	r5, [pc, #120]	; (30008c40 <CPU_ClkGet+0x7c>)
30008bc8:	4b1e      	ldr	r3, [pc, #120]	; (30008c44 <CPU_ClkGet+0x80>)
30008bca:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
30008bce:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30008bd2:	f3c4 1481 	ubfx	r4, r4, #6, #2
30008bd6:	4798      	blx	r3
30008bd8:	b92c      	cbnz	r4, 30008be6 <CPU_ClkGet+0x22>
30008bda:	2801      	cmp	r0, #1
30008bdc:	d007      	beq.n	30008bee <CPU_ClkGet+0x2a>
30008bde:	07b2      	lsls	r2, r6, #30
30008be0:	d407      	bmi.n	30008bf2 <CPU_ClkGet+0x2e>
30008be2:	4819      	ldr	r0, [pc, #100]	; (30008c48 <CPU_ClkGet+0x84>)
30008be4:	bd70      	pop	{r4, r5, r6, pc}
30008be6:	2c01      	cmp	r4, #1
30008be8:	d007      	beq.n	30008bfa <CPU_ClkGet+0x36>
30008bea:	2000      	movs	r0, #0
30008bec:	bd70      	pop	{r4, r5, r6, pc}
30008bee:	4817      	ldr	r0, [pc, #92]	; (30008c4c <CPU_ClkGet+0x88>)
30008bf0:	bd70      	pop	{r4, r5, r6, pc}
30008bf2:	4b17      	ldr	r3, [pc, #92]	; (30008c50 <CPU_ClkGet+0x8c>)
30008bf4:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008bf8:	4718      	bx	r3
30008bfa:	2801      	cmp	r0, #1
30008bfc:	d011      	beq.n	30008c22 <CPU_ClkGet+0x5e>
30008bfe:	05f3      	lsls	r3, r6, #23
30008c00:	d51c      	bpl.n	30008c3c <CPU_ClkGet+0x78>
30008c02:	4914      	ldr	r1, [pc, #80]	; (30008c54 <CPU_ClkGet+0x90>)
30008c04:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30008c08:	6c48      	ldr	r0, [r1, #68]	; 0x44
30008c0a:	4a13      	ldr	r2, [pc, #76]	; (30008c58 <CPU_ClkGet+0x94>)
30008c0c:	f003 0307 	and.w	r3, r3, #7
30008c10:	f3c0 1045 	ubfx	r0, r0, #5, #6
30008c14:	3301      	adds	r3, #1
30008c16:	3002      	adds	r0, #2
30008c18:	fb02 f000 	mul.w	r0, r2, r0
30008c1c:	fbb0 f0f3 	udiv	r0, r0, r3
30008c20:	bd70      	pop	{r4, r5, r6, pc}
30008c22:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30008c26:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30008c2a:	f3c3 4007 	ubfx	r0, r3, #16, #8
30008c2e:	d003      	beq.n	30008c38 <CPU_ClkGet+0x74>
30008c30:	4b0a      	ldr	r3, [pc, #40]	; (30008c5c <CPU_ClkGet+0x98>)
30008c32:	fb03 f000 	mul.w	r0, r3, r0
30008c36:	bd70      	pop	{r4, r5, r6, pc}
30008c38:	4809      	ldr	r0, [pc, #36]	; (30008c60 <CPU_ClkGet+0x9c>)
30008c3a:	bd70      	pop	{r4, r5, r6, pc}
30008c3c:	4806      	ldr	r0, [pc, #24]	; (30008c58 <CPU_ClkGet+0x94>)
30008c3e:	bd70      	pop	{r4, r5, r6, pc}
30008c40:	42008000 	.word	0x42008000
30008c44:	0000c0f9 	.word	0x0000c0f9
30008c48:	003d0900 	.word	0x003d0900
30008c4c:	01312d00 	.word	0x01312d00
30008c50:	000099f5 	.word	0x000099f5
30008c54:	42008800 	.word	0x42008800
30008c58:	02625a00 	.word	0x02625a00
30008c5c:	000f4240 	.word	0x000f4240
30008c60:	02faf080 	.word	0x02faf080

30008c64 <RSIP_IV_Set>:
30008c64:	4b05      	ldr	r3, [pc, #20]	; (30008c7c <RSIP_IV_Set+0x18>)
30008c66:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30008c6a:	680b      	ldr	r3, [r1, #0]
30008c6c:	ba1b      	rev	r3, r3
30008c6e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008c72:	684b      	ldr	r3, [r1, #4]
30008c74:	ba1b      	rev	r3, r3
30008c76:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30008c7a:	4770      	bx	lr
30008c7c:	42008c00 	.word	0x42008c00

30008c80 <RSIP_OTF_Cmd>:
30008c80:	4a04      	ldr	r2, [pc, #16]	; (30008c94 <RSIP_OTF_Cmd+0x14>)
30008c82:	2801      	cmp	r0, #1
30008c84:	6953      	ldr	r3, [r2, #20]
30008c86:	bf0c      	ite	eq
30008c88:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30008c8c:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30008c90:	6153      	str	r3, [r2, #20]
30008c92:	4770      	bx	lr
30008c94:	42008c00 	.word	0x42008c00

30008c98 <RSIP_OTF_Enable>:
30008c98:	b570      	push	{r4, r5, r6, lr}
30008c9a:	4604      	mov	r4, r0
30008c9c:	b1f3      	cbz	r3, 30008cdc <RSIP_OTF_Enable+0x44>
30008c9e:	9b04      	ldr	r3, [sp, #16]
30008ca0:	460d      	mov	r5, r1
30008ca2:	4616      	mov	r6, r2
30008ca4:	2b07      	cmp	r3, #7
30008ca6:	d827      	bhi.n	30008cf8 <RSIP_OTF_Enable+0x60>
30008ca8:	9b05      	ldr	r3, [sp, #20]
30008caa:	3b01      	subs	r3, #1
30008cac:	2b01      	cmp	r3, #1
30008cae:	d81d      	bhi.n	30008cec <RSIP_OTF_Enable+0x54>
30008cb0:	9a04      	ldr	r2, [sp, #16]
30008cb2:	9b05      	ldr	r3, [sp, #20]
30008cb4:	f002 0007 	and.w	r0, r2, #7
30008cb8:	005b      	lsls	r3, r3, #1
30008cba:	4305      	orrs	r5, r0
30008cbc:	4813      	ldr	r0, [pc, #76]	; (30008d0c <RSIP_OTF_Enable+0x74>)
30008cbe:	f003 0306 	and.w	r3, r3, #6
30008cc2:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30008cc6:	ea43 0206 	orr.w	r2, r3, r6
30008cca:	6c63      	ldr	r3, [r4, #68]	; 0x44
30008ccc:	f042 0201 	orr.w	r2, r2, #1
30008cd0:	f023 0301 	bic.w	r3, r3, #1
30008cd4:	6463      	str	r3, [r4, #68]	; 0x44
30008cd6:	6425      	str	r5, [r4, #64]	; 0x40
30008cd8:	6462      	str	r2, [r4, #68]	; 0x44
30008cda:	bd70      	pop	{r4, r5, r6, pc}
30008cdc:	480b      	ldr	r0, [pc, #44]	; (30008d0c <RSIP_OTF_Enable+0x74>)
30008cde:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30008ce2:	6c63      	ldr	r3, [r4, #68]	; 0x44
30008ce4:	f023 0301 	bic.w	r3, r3, #1
30008ce8:	6463      	str	r3, [r4, #68]	; 0x44
30008cea:	bd70      	pop	{r4, r5, r6, pc}
30008cec:	f240 2131 	movw	r1, #561	; 0x231
30008cf0:	4807      	ldr	r0, [pc, #28]	; (30008d10 <RSIP_OTF_Enable+0x78>)
30008cf2:	f002 fd71 	bl	3000b7d8 <__io_assert_failed_veneer>
30008cf6:	e7db      	b.n	30008cb0 <RSIP_OTF_Enable+0x18>
30008cf8:	f44f 710c 	mov.w	r1, #560	; 0x230
30008cfc:	4804      	ldr	r0, [pc, #16]	; (30008d10 <RSIP_OTF_Enable+0x78>)
30008cfe:	f002 fd6b 	bl	3000b7d8 <__io_assert_failed_veneer>
30008d02:	9b05      	ldr	r3, [sp, #20]
30008d04:	3b01      	subs	r3, #1
30008d06:	2b01      	cmp	r3, #1
30008d08:	d9d2      	bls.n	30008cb0 <RSIP_OTF_Enable+0x18>
30008d0a:	e7ef      	b.n	30008cec <RSIP_OTF_Enable+0x54>
30008d0c:	42008c00 	.word	0x42008c00
30008d10:	3000ca6c 	.word	0x3000ca6c

30008d14 <RSIP_MMU_Config>:
30008d14:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30008d18:	311f      	adds	r1, #31
30008d1a:	321f      	adds	r2, #31
30008d1c:	331f      	adds	r3, #31
30008d1e:	0080      	lsls	r0, r0, #2
30008d20:	0949      	lsrs	r1, r1, #5
30008d22:	0952      	lsrs	r2, r2, #5
30008d24:	095b      	lsrs	r3, r3, #5
30008d26:	0249      	lsls	r1, r1, #9
30008d28:	0252      	lsls	r2, r2, #9
30008d2a:	025b      	lsls	r3, r3, #9
30008d2c:	b410      	push	{r4}
30008d2e:	4c08      	ldr	r4, [pc, #32]	; (30008d50 <RSIP_MMU_Config+0x3c>)
30008d30:	4404      	add	r4, r0
30008d32:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
30008d36:	f020 0001 	bic.w	r0, r0, #1
30008d3a:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
30008d3e:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
30008d42:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
30008d46:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
30008d4a:	f85d 4b04 	ldr.w	r4, [sp], #4
30008d4e:	4770      	bx	lr
30008d50:	42008c00 	.word	0x42008c00

30008d54 <RSIP_MMU_Cmd>:
30008d54:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30008d58:	4b07      	ldr	r3, [pc, #28]	; (30008d78 <RSIP_MMU_Cmd+0x24>)
30008d5a:	0080      	lsls	r0, r0, #2
30008d5c:	4403      	add	r3, r0
30008d5e:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
30008d62:	b121      	cbz	r1, 30008d6e <RSIP_MMU_Cmd+0x1a>
30008d64:	f042 0201 	orr.w	r2, r2, #1
30008d68:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30008d6c:	4770      	bx	lr
30008d6e:	f022 0201 	bic.w	r2, r2, #1
30008d72:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30008d76:	4770      	bx	lr
30008d78:	42008c00 	.word	0x42008c00

30008d7c <RSIP_MMU_Cache_Clean>:
30008d7c:	4a02      	ldr	r2, [pc, #8]	; (30008d88 <RSIP_MMU_Cache_Clean+0xc>)
30008d7e:	6993      	ldr	r3, [r2, #24]
30008d80:	f043 0304 	orr.w	r3, r3, #4
30008d84:	6193      	str	r3, [r2, #24]
30008d86:	4770      	bx	lr
30008d88:	42008c00 	.word	0x42008c00

30008d8c <NAND_RxData>:
30008d8c:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30008d90:	460c      	mov	r4, r1
30008d92:	4616      	mov	r6, r2
30008d94:	461f      	mov	r7, r3
30008d96:	2800      	cmp	r0, #0
30008d98:	f000 808a 	beq.w	30008eb0 <NAND_RxData+0x124>
30008d9c:	4605      	mov	r5, r0
30008d9e:	f8df 911c 	ldr.w	r9, [pc, #284]	; 30008ebc <NAND_RxData+0x130>
30008da2:	2000      	movs	r0, #0
30008da4:	f8df 8118 	ldr.w	r8, [pc, #280]	; 30008ec0 <NAND_RxData+0x134>
30008da8:	47c0      	blx	r8
30008daa:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30008dae:	2000      	movs	r0, #0
30008db0:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
30008db4:	6819      	ldr	r1, [r3, #0]
30008db6:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
30008dba:	6019      	str	r1, [r3, #0]
30008dbc:	6819      	ldr	r1, [r3, #0]
30008dbe:	f441 7140 	orr.w	r1, r1, #768	; 0x300
30008dc2:	6019      	str	r1, [r3, #0]
30008dc4:	605a      	str	r2, [r3, #4]
30008dc6:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
30008dca:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
30008dce:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
30008dd2:	ea4f 420c 	mov.w	r2, ip, lsl #16
30008dd6:	f1bc 0f03 	cmp.w	ip, #3
30008dda:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30008dde:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008de2:	ea42 0201 	orr.w	r2, r2, r1
30008de6:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30008dea:	f04f 0201 	mov.w	r2, #1
30008dee:	611a      	str	r2, [r3, #16]
30008df0:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30008df4:	bf08      	it	eq
30008df6:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30008dfa:	f3c4 2307 	ubfx	r3, r4, #8, #8
30008dfe:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
30008e02:	b2e4      	uxtb	r4, r4
30008e04:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30008e08:	2301      	movs	r3, #1
30008e0a:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30008e0e:	f017 0403 	ands.w	r4, r7, #3
30008e12:	6083      	str	r3, [r0, #8]
30008e14:	d137      	bne.n	30008e86 <NAND_RxData+0xfa>
30008e16:	463b      	mov	r3, r7
30008e18:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30008e1c:	42b4      	cmp	r4, r6
30008e1e:	d216      	bcs.n	30008e4e <NAND_RxData+0xc2>
30008e20:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
30008e24:	f012 0f3c 	tst.w	r2, #60	; 0x3c
30008e28:	f3c2 0583 	ubfx	r5, r2, #2, #4
30008e2c:	d00c      	beq.n	30008e48 <NAND_RxData+0xbc>
30008e2e:	00ad      	lsls	r5, r5, #2
30008e30:	1f19      	subs	r1, r3, #4
30008e32:	f1a5 0c04 	sub.w	ip, r5, #4
30008e36:	449c      	add	ip, r3
30008e38:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
30008e3c:	f841 0f04 	str.w	r0, [r1, #4]!
30008e40:	458c      	cmp	ip, r1
30008e42:	d1f9      	bne.n	30008e38 <NAND_RxData+0xac>
30008e44:	442b      	add	r3, r5
30008e46:	442c      	add	r4, r5
30008e48:	1b32      	subs	r2, r6, r4
30008e4a:	2a03      	cmp	r2, #3
30008e4c:	d8e6      	bhi.n	30008e1c <NAND_RxData+0x90>
30008e4e:	1e63      	subs	r3, r4, #1
30008e50:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30008e54:	441f      	add	r7, r3
30008e56:	e002      	b.n	30008e5e <NAND_RxData+0xd2>
30008e58:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008e5a:	071b      	lsls	r3, r3, #28
30008e5c:	d40d      	bmi.n	30008e7a <NAND_RxData+0xee>
30008e5e:	42b4      	cmp	r4, r6
30008e60:	d3fa      	bcc.n	30008e58 <NAND_RxData+0xcc>
30008e62:	2004      	movs	r0, #4
30008e64:	47c0      	blx	r8
30008e66:	2000      	movs	r0, #0
30008e68:	47c0      	blx	r8
30008e6a:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30008e6e:	6813      	ldr	r3, [r2, #0]
30008e70:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008e74:	6013      	str	r3, [r2, #0]
30008e76:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30008e7a:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
30008e7e:	3401      	adds	r4, #1
30008e80:	f807 3f01 	strb.w	r3, [r7, #1]!
30008e84:	e7eb      	b.n	30008e5e <NAND_RxData+0xd2>
30008e86:	f1c4 0404 	rsb	r4, r4, #4
30008e8a:	2200      	movs	r2, #0
30008e8c:	4639      	mov	r1, r7
30008e8e:	e00a      	b.n	30008ea6 <NAND_RxData+0x11a>
30008e90:	6a85      	ldr	r5, [r0, #40]	; 0x28
30008e92:	072d      	lsls	r5, r5, #28
30008e94:	d505      	bpl.n	30008ea2 <NAND_RxData+0x116>
30008e96:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
30008e9a:	3201      	adds	r2, #1
30008e9c:	700b      	strb	r3, [r1, #0]
30008e9e:	18b9      	adds	r1, r7, r2
30008ea0:	460b      	mov	r3, r1
30008ea2:	4294      	cmp	r4, r2
30008ea4:	d0b8      	beq.n	30008e18 <NAND_RxData+0x8c>
30008ea6:	42b2      	cmp	r2, r6
30008ea8:	460b      	mov	r3, r1
30008eaa:	d3f1      	bcc.n	30008e90 <NAND_RxData+0x104>
30008eac:	4614      	mov	r4, r2
30008eae:	e7ce      	b.n	30008e4e <NAND_RxData+0xc2>
30008eb0:	f8df 9008 	ldr.w	r9, [pc, #8]	; 30008ebc <NAND_RxData+0x130>
30008eb4:	f899 5008 	ldrb.w	r5, [r9, #8]
30008eb8:	e773      	b.n	30008da2 <NAND_RxData+0x16>
30008eba:	bf00      	nop
30008ebc:	2001c01c 	.word	0x2001c01c
30008ec0:	0000b9c1 	.word	0x0000b9c1

30008ec4 <NAND_Page_Read>:
30008ec4:	b530      	push	{r4, r5, lr}
30008ec6:	b085      	sub	sp, #20
30008ec8:	4c07      	ldr	r4, [pc, #28]	; (30008ee8 <NAND_Page_Read+0x24>)
30008eca:	4d08      	ldr	r5, [pc, #32]	; (30008eec <NAND_Page_Read+0x28>)
30008ecc:	9301      	str	r3, [sp, #4]
30008ece:	e9cd 2102 	strd	r2, r1, [sp, #8]
30008ed2:	47a0      	blx	r4
30008ed4:	4604      	mov	r4, r0
30008ed6:	9903      	ldr	r1, [sp, #12]
30008ed8:	7a28      	ldrb	r0, [r5, #8]
30008eda:	4d05      	ldr	r5, [pc, #20]	; (30008ef0 <NAND_Page_Read+0x2c>)
30008edc:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30008ee0:	47a8      	blx	r5
30008ee2:	4620      	mov	r0, r4
30008ee4:	b005      	add	sp, #20
30008ee6:	bd30      	pop	{r4, r5, pc}
30008ee8:	0000bd85 	.word	0x0000bd85
30008eec:	2001c01c 	.word	0x2001c01c
30008ef0:	30008d8d 	.word	0x30008d8d

30008ef4 <SYSCFG_BootFromNor>:
30008ef4:	4b0e      	ldr	r3, [pc, #56]	; (30008f30 <SYSCFG_BootFromNor+0x3c>)
30008ef6:	b510      	push	{r4, lr}
30008ef8:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30008efc:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30008f00:	0392      	lsls	r2, r2, #14
30008f02:	d50a      	bpl.n	30008f1a <SYSCFG_BootFromNor+0x26>
30008f04:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30008f08:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30008f0c:	d105      	bne.n	30008f1a <SYSCFG_BootFromNor+0x26>
30008f0e:	4b09      	ldr	r3, [pc, #36]	; (30008f34 <SYSCFG_BootFromNor+0x40>)
30008f10:	4798      	blx	r3
30008f12:	3800      	subs	r0, #0
30008f14:	bf18      	it	ne
30008f16:	2001      	movne	r0, #1
30008f18:	bd10      	pop	{r4, pc}
30008f1a:	4c07      	ldr	r4, [pc, #28]	; (30008f38 <SYSCFG_BootFromNor+0x44>)
30008f1c:	47a0      	blx	r4
30008f1e:	2802      	cmp	r0, #2
30008f20:	d004      	beq.n	30008f2c <SYSCFG_BootFromNor+0x38>
30008f22:	47a0      	blx	r4
30008f24:	2801      	cmp	r0, #1
30008f26:	d1f2      	bne.n	30008f0e <SYSCFG_BootFromNor+0x1a>
30008f28:	2000      	movs	r0, #0
30008f2a:	bd10      	pop	{r4, pc}
30008f2c:	2001      	movs	r0, #1
30008f2e:	bd10      	pop	{r4, pc}
30008f30:	42008000 	.word	0x42008000
30008f34:	0000c0bd 	.word	0x0000c0bd
30008f38:	0000c1a9 	.word	0x0000c1a9

30008f3c <SYSCFG_RLVersion>:
30008f3c:	4b07      	ldr	r3, [pc, #28]	; (30008f5c <SYSCFG_RLVersion+0x20>)
30008f3e:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30008f42:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30008f46:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30008f4a:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30008f4e:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30008f52:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30008f56:	f3c0 4003 	ubfx	r0, r0, #16, #4
30008f5a:	4770      	bx	lr
30008f5c:	42008000 	.word	0x42008000

30008f60 <SBOOT_GetMdType>:
30008f60:	b470      	push	{r4, r5, r6}
30008f62:	4c0c      	ldr	r4, [pc, #48]	; (30008f94 <SBOOT_GetMdType+0x34>)
30008f64:	4e0c      	ldr	r6, [pc, #48]	; (30008f98 <SBOOT_GetMdType+0x38>)
30008f66:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30008f6a:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30008f6e:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30008f72:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30008f76:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30008f7a:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30008f7e:	f3c3 4303 	ubfx	r3, r3, #16, #4
30008f82:	6033      	str	r3, [r6, #0]
30008f84:	b913      	cbnz	r3, 30008f8c <SBOOT_GetMdType+0x2c>
30008f86:	bc70      	pop	{r4, r5, r6}
30008f88:	f002 bc56 	b.w	3000b838 <__SBOOT_GetMdType_A_veneer>
30008f8c:	bc70      	pop	{r4, r5, r6}
30008f8e:	f002 bc43 	b.w	3000b818 <__SBOOT_GetMdType_B_veneer>
30008f92:	bf00      	nop
30008f94:	42008000 	.word	0x42008000
30008f98:	3000ed64 	.word	0x3000ed64

30008f9c <CRYPTO_OTPKey_SHA_Init>:
30008f9c:	4a0d      	ldr	r2, [pc, #52]	; (30008fd4 <CRYPTO_OTPKey_SHA_Init+0x38>)
30008f9e:	b410      	push	{r4}
30008fa0:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30008fa4:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30008fa8:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30008fac:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30008fb0:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30008fb4:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30008fb8:	f3c3 4303 	ubfx	r3, r3, #16, #4
30008fbc:	4a06      	ldr	r2, [pc, #24]	; (30008fd8 <CRYPTO_OTPKey_SHA_Init+0x3c>)
30008fbe:	6013      	str	r3, [r2, #0]
30008fc0:	b91b      	cbnz	r3, 30008fca <CRYPTO_OTPKey_SHA_Init+0x2e>
30008fc2:	f85d 4b04 	ldr.w	r4, [sp], #4
30008fc6:	f002 bb87 	b.w	3000b6d8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30008fca:	f85d 4b04 	ldr.w	r4, [sp], #4
30008fce:	f002 bb7b 	b.w	3000b6c8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
30008fd2:	bf00      	nop
30008fd4:	42008000 	.word	0x42008000
30008fd8:	3000ed64 	.word	0x3000ed64

30008fdc <rtl_crypto_hmac_sha2_update>:
30008fdc:	b470      	push	{r4, r5, r6}
30008fde:	4c0c      	ldr	r4, [pc, #48]	; (30009010 <rtl_crypto_hmac_sha2_update+0x34>)
30008fe0:	4e0c      	ldr	r6, [pc, #48]	; (30009014 <rtl_crypto_hmac_sha2_update+0x38>)
30008fe2:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30008fe6:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30008fea:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30008fee:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30008ff2:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30008ff6:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30008ffa:	f3c3 4303 	ubfx	r3, r3, #16, #4
30008ffe:	6033      	str	r3, [r6, #0]
30009000:	b913      	cbnz	r3, 30009008 <rtl_crypto_hmac_sha2_update+0x2c>
30009002:	bc70      	pop	{r4, r5, r6}
30009004:	f002 bc20 	b.w	3000b848 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009008:	bc70      	pop	{r4, r5, r6}
3000900a:	f002 bb4d 	b.w	3000b6a8 <__rtl_crypto_hmac_sha2_update_B_veneer>
3000900e:	bf00      	nop
30009010:	42008000 	.word	0x42008000
30009014:	3000ed64 	.word	0x3000ed64

30009018 <rtl_crypto_hmac_sha2_final>:
30009018:	4a0d      	ldr	r2, [pc, #52]	; (30009050 <rtl_crypto_hmac_sha2_final+0x38>)
3000901a:	b410      	push	{r4}
3000901c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009020:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009024:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009028:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
3000902c:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009030:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009034:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009038:	4a06      	ldr	r2, [pc, #24]	; (30009054 <rtl_crypto_hmac_sha2_final+0x3c>)
3000903a:	6013      	str	r3, [r2, #0]
3000903c:	b91b      	cbnz	r3, 30009046 <rtl_crypto_hmac_sha2_final+0x2e>
3000903e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009042:	f002 bb59 	b.w	3000b6f8 <__rtl_crypto_hmac_sha2_final_A_veneer>
30009046:	f85d 4b04 	ldr.w	r4, [sp], #4
3000904a:	f002 bbd5 	b.w	3000b7f8 <__rtl_crypto_hmac_sha2_final_B_veneer>
3000904e:	bf00      	nop
30009050:	42008000 	.word	0x42008000
30009054:	3000ed64 	.word	0x3000ed64

30009058 <rtl_crypto_sha2_init>:
30009058:	4a0d      	ldr	r2, [pc, #52]	; (30009090 <rtl_crypto_sha2_init+0x38>)
3000905a:	b410      	push	{r4}
3000905c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009060:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009064:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009068:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
3000906c:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009070:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009074:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009078:	4a06      	ldr	r2, [pc, #24]	; (30009094 <rtl_crypto_sha2_init+0x3c>)
3000907a:	6013      	str	r3, [r2, #0]
3000907c:	b91b      	cbnz	r3, 30009086 <rtl_crypto_sha2_init+0x2e>
3000907e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009082:	f002 bb81 	b.w	3000b788 <__rtl_crypto_sha2_init_A_veneer>
30009086:	f85d 4b04 	ldr.w	r4, [sp], #4
3000908a:	f002 bb75 	b.w	3000b778 <__rtl_crypto_sha2_init_B_veneer>
3000908e:	bf00      	nop
30009090:	42008000 	.word	0x42008000
30009094:	3000ed64 	.word	0x3000ed64

30009098 <rtl_crypto_sha2_update>:
30009098:	b470      	push	{r4, r5, r6}
3000909a:	4c0c      	ldr	r4, [pc, #48]	; (300090cc <rtl_crypto_sha2_update+0x34>)
3000909c:	4e0c      	ldr	r6, [pc, #48]	; (300090d0 <rtl_crypto_sha2_update+0x38>)
3000909e:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
300090a2:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
300090a6:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
300090aa:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
300090ae:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
300090b2:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
300090b6:	f3c3 4303 	ubfx	r3, r3, #16, #4
300090ba:	6033      	str	r3, [r6, #0]
300090bc:	b913      	cbnz	r3, 300090c4 <rtl_crypto_sha2_update+0x2c>
300090be:	bc70      	pop	{r4, r5, r6}
300090c0:	f002 bbb2 	b.w	3000b828 <__rtl_crypto_sha2_update_A_veneer>
300090c4:	bc70      	pop	{r4, r5, r6}
300090c6:	f002 bb3f 	b.w	3000b748 <__rtl_crypto_sha2_update_B_veneer>
300090ca:	bf00      	nop
300090cc:	42008000 	.word	0x42008000
300090d0:	3000ed64 	.word	0x3000ed64

300090d4 <rtl_crypto_sha2_final>:
300090d4:	4a0d      	ldr	r2, [pc, #52]	; (3000910c <rtl_crypto_sha2_final+0x38>)
300090d6:	b410      	push	{r4}
300090d8:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
300090dc:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
300090e0:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
300090e4:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
300090e8:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
300090ec:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
300090f0:	f3c3 4303 	ubfx	r3, r3, #16, #4
300090f4:	4a06      	ldr	r2, [pc, #24]	; (30009110 <rtl_crypto_sha2_final+0x3c>)
300090f6:	6013      	str	r3, [r2, #0]
300090f8:	b91b      	cbnz	r3, 30009102 <rtl_crypto_sha2_final+0x2e>
300090fa:	f85d 4b04 	ldr.w	r4, [sp], #4
300090fe:	f002 bb2b 	b.w	3000b758 <__rtl_crypto_sha2_final_A_veneer>
30009102:	f85d 4b04 	ldr.w	r4, [sp], #4
30009106:	f002 bb6f 	b.w	3000b7e8 <__rtl_crypto_sha2_final_B_veneer>
3000910a:	bf00      	nop
3000910c:	42008000 	.word	0x42008000
30009110:	3000ed64 	.word	0x3000ed64

30009114 <SBOOT_Validate_PubKey>:
30009114:	b470      	push	{r4, r5, r6}
30009116:	4c0c      	ldr	r4, [pc, #48]	; (30009148 <SBOOT_Validate_PubKey+0x34>)
30009118:	4e0c      	ldr	r6, [pc, #48]	; (3000914c <SBOOT_Validate_PubKey+0x38>)
3000911a:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000911e:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009122:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009126:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000912a:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000912e:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009132:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009136:	6033      	str	r3, [r6, #0]
30009138:	b913      	cbnz	r3, 30009140 <SBOOT_Validate_PubKey+0x2c>
3000913a:	bc70      	pop	{r4, r5, r6}
3000913c:	f002 bb3c 	b.w	3000b7b8 <__SBOOT_Validate_PubKey_A_veneer>
30009140:	bc70      	pop	{r4, r5, r6}
30009142:	f002 bab9 	b.w	3000b6b8 <__SBOOT_Validate_PubKey_B_veneer>
30009146:	bf00      	nop
30009148:	42008000 	.word	0x42008000
3000914c:	3000ed64 	.word	0x3000ed64

30009150 <SBOOT_Validate_Signature>:
30009150:	b5f0      	push	{r4, r5, r6, r7, lr}
30009152:	4d10      	ldr	r5, [pc, #64]	; (30009194 <SBOOT_Validate_Signature+0x44>)
30009154:	9f05      	ldr	r7, [sp, #20]
30009156:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000915a:	f8dd c018 	ldr.w	ip, [sp, #24]
3000915e:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009162:	f8df e034 	ldr.w	lr, [pc, #52]	; 30009198 <SBOOT_Validate_Signature+0x48>
30009166:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000916a:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000916e:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009172:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009176:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000917a:	e9cd 7c05 	strd	r7, ip, [sp, #20]
3000917e:	f8ce 4000 	str.w	r4, [lr]
30009182:	b91c      	cbnz	r4, 3000918c <SBOOT_Validate_Signature+0x3c>
30009184:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009188:	f002 bb06 	b.w	3000b798 <__SBOOT_Validate_Signature_A_veneer>
3000918c:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009190:	f002 bb72 	b.w	3000b878 <__SBOOT_Validate_Signature_B_veneer>
30009194:	42008000 	.word	0x42008000
30009198:	3000ed64 	.word	0x3000ed64

3000919c <SBOOT_Validate_ImgHash>:
3000919c:	b4f0      	push	{r4, r5, r6, r7}
3000919e:	4d0c      	ldr	r5, [pc, #48]	; (300091d0 <SBOOT_Validate_ImgHash+0x34>)
300091a0:	4f0c      	ldr	r7, [pc, #48]	; (300091d4 <SBOOT_Validate_ImgHash+0x38>)
300091a2:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
300091a6:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
300091aa:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
300091ae:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
300091b2:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
300091b6:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
300091ba:	f3c4 4403 	ubfx	r4, r4, #16, #4
300091be:	603c      	str	r4, [r7, #0]
300091c0:	b914      	cbnz	r4, 300091c8 <SBOOT_Validate_ImgHash+0x2c>
300091c2:	bcf0      	pop	{r4, r5, r6, r7}
300091c4:	f002 bb20 	b.w	3000b808 <__SBOOT_Validate_ImgHash_A_veneer>
300091c8:	bcf0      	pop	{r4, r5, r6, r7}
300091ca:	f002 baed 	b.w	3000b7a8 <__SBOOT_Validate_ImgHash_B_veneer>
300091ce:	bf00      	nop
300091d0:	42008000 	.word	0x42008000
300091d4:	3000ed64 	.word	0x3000ed64

300091d8 <CRYPTO_SHA_Init>:
300091d8:	4b0b      	ldr	r3, [pc, #44]	; (30009208 <CRYPTO_SHA_Init+0x30>)
300091da:	490c      	ldr	r1, [pc, #48]	; (3000920c <CRYPTO_SHA_Init+0x34>)
300091dc:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
300091e0:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
300091e4:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
300091e8:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
300091ec:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
300091f0:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
300091f4:	f3c0 4003 	ubfx	r0, r0, #16, #4
300091f8:	6008      	str	r0, [r1, #0]
300091fa:	b908      	cbnz	r0, 30009200 <CRYPTO_SHA_Init+0x28>
300091fc:	f002 bb34 	b.w	3000b868 <__CRYPTO_SHA_Init_A_veneer>
30009200:	2000      	movs	r0, #0
30009202:	f002 ba91 	b.w	3000b728 <__CRYPTO_SHA_Init_B_veneer>
30009206:	bf00      	nop
30009208:	42008000 	.word	0x42008000
3000920c:	3000ed64 	.word	0x3000ed64

30009210 <SBOOT_Validate_Algorithm>:
30009210:	b4f0      	push	{r4, r5, r6, r7}
30009212:	4d0c      	ldr	r5, [pc, #48]	; (30009244 <SBOOT_Validate_Algorithm+0x34>)
30009214:	4f0c      	ldr	r7, [pc, #48]	; (30009248 <SBOOT_Validate_Algorithm+0x38>)
30009216:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000921a:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000921e:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009222:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009226:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000922a:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000922e:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009232:	603c      	str	r4, [r7, #0]
30009234:	b914      	cbnz	r4, 3000923c <SBOOT_Validate_Algorithm+0x2c>
30009236:	bcf0      	pop	{r4, r5, r6, r7}
30009238:	f002 bac6 	b.w	3000b7c8 <__SBOOT_Validate_Algorithm_A_veneer>
3000923c:	bcf0      	pop	{r4, r5, r6, r7}
3000923e:	f002 bb0b 	b.w	3000b858 <__SBOOT_Validate_Algorithm_B_veneer>
30009242:	bf00      	nop
30009244:	42008000 	.word	0x42008000
30009248:	3000ed64 	.word	0x3000ed64

3000924c <ddr_init_index>:
3000924c:	b570      	push	{r4, r5, r6, lr}
3000924e:	4b0f      	ldr	r3, [pc, #60]	; (3000928c <ddr_init_index+0x40>)
30009250:	4798      	blx	r3
30009252:	4604      	mov	r4, r0
30009254:	4b0e      	ldr	r3, [pc, #56]	; (30009290 <ddr_init_index+0x44>)
30009256:	4798      	blx	r3
30009258:	2c03      	cmp	r4, #3
3000925a:	d014      	beq.n	30009286 <ddr_init_index+0x3a>
3000925c:	2c01      	cmp	r4, #1
3000925e:	d00c      	beq.n	3000927a <ddr_init_index+0x2e>
30009260:	4e0c      	ldr	r6, [pc, #48]	; (30009294 <ddr_init_index+0x48>)
30009262:	4d0d      	ldr	r5, [pc, #52]	; (30009298 <ddr_init_index+0x4c>)
30009264:	4c0d      	ldr	r4, [pc, #52]	; (3000929c <ddr_init_index+0x50>)
30009266:	462b      	mov	r3, r5
30009268:	4622      	mov	r2, r4
3000926a:	490b      	ldr	r1, [pc, #44]	; (30009298 <ddr_init_index+0x4c>)
3000926c:	2002      	movs	r0, #2
3000926e:	f001 ffdd 	bl	3000b22c <rtk_log_write>
30009272:	f242 7010 	movw	r0, #10000	; 0x2710
30009276:	47b0      	blx	r6
30009278:	e7f5      	b.n	30009266 <ddr_init_index+0x1a>
3000927a:	2802      	cmp	r0, #2
3000927c:	d003      	beq.n	30009286 <ddr_init_index+0x3a>
3000927e:	2803      	cmp	r0, #3
30009280:	d1ee      	bne.n	30009260 <ddr_init_index+0x14>
30009282:	2000      	movs	r0, #0
30009284:	bd70      	pop	{r4, r5, r6, pc}
30009286:	4620      	mov	r0, r4
30009288:	bd70      	pop	{r4, r5, r6, pc}
3000928a:	bf00      	nop
3000928c:	30007309 	.word	0x30007309
30009290:	30007185 	.word	0x30007185
30009294:	00009be5 	.word	0x00009be5
30009298:	3000cab4 	.word	0x3000cab4
3000929c:	3000cab8 	.word	0x3000cab8

300092a0 <rxi316_perf_tune>:
300092a0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300092a4:	4606      	mov	r6, r0
300092a6:	6800      	ldr	r0, [r0, #0]
300092a8:	b0c3      	sub	sp, #268	; 0x10c
300092aa:	7985      	ldrb	r5, [r0, #6]
300092ac:	2d00      	cmp	r5, #0
300092ae:	f000 8334 	beq.w	3000991a <rxi316_perf_tune+0x67a>
300092b2:	2d01      	cmp	r5, #1
300092b4:	d003      	beq.n	300092be <rxi316_perf_tune+0x1e>
300092b6:	2d03      	cmp	r5, #3
300092b8:	bf14      	ite	ne
300092ba:	2502      	movne	r5, #2
300092bc:	2500      	moveq	r5, #0
300092be:	68f3      	ldr	r3, [r6, #12]
300092c0:	2420      	movs	r4, #32
300092c2:	4684      	mov	ip, r0
300092c4:	6859      	ldr	r1, [r3, #4]
300092c6:	46a3      	mov	fp, r4
300092c8:	46a2      	mov	sl, r4
300092ca:	46a0      	mov	r8, r4
300092cc:	784b      	ldrb	r3, [r1, #1]
300092ce:	46a1      	mov	r9, r4
300092d0:	7bcf      	ldrb	r7, [r1, #15]
300092d2:	9323      	str	r3, [sp, #140]	; 0x8c
300092d4:	788b      	ldrb	r3, [r1, #2]
300092d6:	780a      	ldrb	r2, [r1, #0]
300092d8:	9324      	str	r3, [sp, #144]	; 0x90
300092da:	78cb      	ldrb	r3, [r1, #3]
300092dc:	9325      	str	r3, [sp, #148]	; 0x94
300092de:	790b      	ldrb	r3, [r1, #4]
300092e0:	9326      	str	r3, [sp, #152]	; 0x98
300092e2:	794b      	ldrb	r3, [r1, #5]
300092e4:	9327      	str	r3, [sp, #156]	; 0x9c
300092e6:	798b      	ldrb	r3, [r1, #6]
300092e8:	9328      	str	r3, [sp, #160]	; 0xa0
300092ea:	79cb      	ldrb	r3, [r1, #7]
300092ec:	9329      	str	r3, [sp, #164]	; 0xa4
300092ee:	7a0b      	ldrb	r3, [r1, #8]
300092f0:	932a      	str	r3, [sp, #168]	; 0xa8
300092f2:	7a4b      	ldrb	r3, [r1, #9]
300092f4:	932b      	str	r3, [sp, #172]	; 0xac
300092f6:	7a8b      	ldrb	r3, [r1, #10]
300092f8:	932c      	str	r3, [sp, #176]	; 0xb0
300092fa:	7acb      	ldrb	r3, [r1, #11]
300092fc:	932d      	str	r3, [sp, #180]	; 0xb4
300092fe:	7b0b      	ldrb	r3, [r1, #12]
30009300:	932e      	str	r3, [sp, #184]	; 0xb8
30009302:	7b4b      	ldrb	r3, [r1, #13]
30009304:	932f      	str	r3, [sp, #188]	; 0xbc
30009306:	7b8b      	ldrb	r3, [r1, #14]
30009308:	9330      	str	r3, [sp, #192]	; 0xc0
3000930a:	2300      	movs	r3, #0
3000930c:	9731      	str	r7, [sp, #196]	; 0xc4
3000930e:	7c0f      	ldrb	r7, [r1, #16]
30009310:	941d      	str	r4, [sp, #116]	; 0x74
30009312:	9732      	str	r7, [sp, #200]	; 0xc8
30009314:	7c4f      	ldrb	r7, [r1, #17]
30009316:	9733      	str	r7, [sp, #204]	; 0xcc
30009318:	7c8f      	ldrb	r7, [r1, #18]
3000931a:	9734      	str	r7, [sp, #208]	; 0xd0
3000931c:	7ccf      	ldrb	r7, [r1, #19]
3000931e:	9735      	str	r7, [sp, #212]	; 0xd4
30009320:	7d0f      	ldrb	r7, [r1, #20]
30009322:	9736      	str	r7, [sp, #216]	; 0xd8
30009324:	7d4f      	ldrb	r7, [r1, #21]
30009326:	9737      	str	r7, [sp, #220]	; 0xdc
30009328:	7d8f      	ldrb	r7, [r1, #22]
3000932a:	9738      	str	r7, [sp, #224]	; 0xe0
3000932c:	7dcf      	ldrb	r7, [r1, #23]
3000932e:	9739      	str	r7, [sp, #228]	; 0xe4
30009330:	7e0f      	ldrb	r7, [r1, #24]
30009332:	973a      	str	r7, [sp, #232]	; 0xe8
30009334:	7e4f      	ldrb	r7, [r1, #25]
30009336:	973b      	str	r7, [sp, #236]	; 0xec
30009338:	7e8f      	ldrb	r7, [r1, #26]
3000933a:	973c      	str	r7, [sp, #240]	; 0xf0
3000933c:	7ecf      	ldrb	r7, [r1, #27]
3000933e:	973d      	str	r7, [sp, #244]	; 0xf4
30009340:	7f0f      	ldrb	r7, [r1, #28]
30009342:	973e      	str	r7, [sp, #248]	; 0xf8
30009344:	7f4f      	ldrb	r7, [r1, #29]
30009346:	973f      	str	r7, [sp, #252]	; 0xfc
30009348:	7f8f      	ldrb	r7, [r1, #30]
3000934a:	7fc9      	ldrb	r1, [r1, #31]
3000934c:	9740      	str	r7, [sp, #256]	; 0x100
3000934e:	4627      	mov	r7, r4
30009350:	9141      	str	r1, [sp, #260]	; 0x104
30009352:	4619      	mov	r1, r3
30009354:	9409      	str	r4, [sp, #36]	; 0x24
30009356:	941c      	str	r4, [sp, #112]	; 0x70
30009358:	9404      	str	r4, [sp, #16]
3000935a:	941b      	str	r4, [sp, #108]	; 0x6c
3000935c:	940a      	str	r4, [sp, #40]	; 0x28
3000935e:	9303      	str	r3, [sp, #12]
30009360:	e9cd 4405 	strd	r4, r4, [sp, #20]
30009364:	e9cd 4401 	strd	r4, r4, [sp, #4]
30009368:	e9cd 4407 	strd	r4, r4, [sp, #28]
3000936c:	e9cd 4419 	strd	r4, r4, [sp, #100]	; 0x64
30009370:	e9cd 4417 	strd	r4, r4, [sp, #92]	; 0x5c
30009374:	e9cd 4415 	strd	r4, r4, [sp, #84]	; 0x54
30009378:	e9cd 4413 	strd	r4, r4, [sp, #76]	; 0x4c
3000937c:	e9cd 4411 	strd	r4, r4, [sp, #68]	; 0x44
30009380:	e9cd 440f 	strd	r4, r4, [sp, #60]	; 0x3c
30009384:	e9cd 440d 	strd	r4, r4, [sp, #52]	; 0x34
30009388:	e9cd 440b 	strd	r4, r4, [sp, #44]	; 0x2c
3000938c:	b94a      	cbnz	r2, 300093a2 <rxi316_perf_tune+0x102>
3000938e:	3101      	adds	r1, #1
30009390:	4699      	mov	r9, r3
30009392:	3301      	adds	r3, #1
30009394:	2b20      	cmp	r3, #32
30009396:	d069      	beq.n	3000946c <rxi316_perf_tune+0x1cc>
30009398:	aa22      	add	r2, sp, #136	; 0x88
3000939a:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000939e:	2a00      	cmp	r2, #0
300093a0:	d0f5      	beq.n	3000938e <rxi316_perf_tune+0xee>
300093a2:	2a01      	cmp	r2, #1
300093a4:	d05d      	beq.n	30009462 <rxi316_perf_tune+0x1c2>
300093a6:	2a02      	cmp	r2, #2
300093a8:	d06e      	beq.n	30009488 <rxi316_perf_tune+0x1e8>
300093aa:	2a03      	cmp	r2, #3
300093ac:	d06f      	beq.n	3000948e <rxi316_perf_tune+0x1ee>
300093ae:	2a04      	cmp	r2, #4
300093b0:	d070      	beq.n	30009494 <rxi316_perf_tune+0x1f4>
300093b2:	2a05      	cmp	r2, #5
300093b4:	d071      	beq.n	3000949a <rxi316_perf_tune+0x1fa>
300093b6:	2a06      	cmp	r2, #6
300093b8:	d072      	beq.n	300094a0 <rxi316_perf_tune+0x200>
300093ba:	2a07      	cmp	r2, #7
300093bc:	f000 82af 	beq.w	3000991e <rxi316_perf_tune+0x67e>
300093c0:	2a08      	cmp	r2, #8
300093c2:	f000 8381 	beq.w	30009ac8 <rxi316_perf_tune+0x828>
300093c6:	2a09      	cmp	r2, #9
300093c8:	f000 8386 	beq.w	30009ad8 <rxi316_perf_tune+0x838>
300093cc:	2a0a      	cmp	r2, #10
300093ce:	f000 838e 	beq.w	30009aee <rxi316_perf_tune+0x84e>
300093d2:	2a0b      	cmp	r2, #11
300093d4:	f000 8386 	beq.w	30009ae4 <rxi316_perf_tune+0x844>
300093d8:	2a0c      	cmp	r2, #12
300093da:	f000 838d 	beq.w	30009af8 <rxi316_perf_tune+0x858>
300093de:	2a0d      	cmp	r2, #13
300093e0:	f000 8394 	beq.w	30009b0c <rxi316_perf_tune+0x86c>
300093e4:	2a0e      	cmp	r2, #14
300093e6:	f000 838c 	beq.w	30009b02 <rxi316_perf_tune+0x862>
300093ea:	2a0f      	cmp	r2, #15
300093ec:	f000 8393 	beq.w	30009b16 <rxi316_perf_tune+0x876>
300093f0:	2a10      	cmp	r2, #16
300093f2:	f000 8395 	beq.w	30009b20 <rxi316_perf_tune+0x880>
300093f6:	2a11      	cmp	r2, #17
300093f8:	f000 8394 	beq.w	30009b24 <rxi316_perf_tune+0x884>
300093fc:	2a12      	cmp	r2, #18
300093fe:	f000 8393 	beq.w	30009b28 <rxi316_perf_tune+0x888>
30009402:	2a13      	cmp	r2, #19
30009404:	f000 8394 	beq.w	30009b30 <rxi316_perf_tune+0x890>
30009408:	2a14      	cmp	r2, #20
3000940a:	f000 838f 	beq.w	30009b2c <rxi316_perf_tune+0x88c>
3000940e:	2a15      	cmp	r2, #21
30009410:	f000 8396 	beq.w	30009b40 <rxi316_perf_tune+0x8a0>
30009414:	2a16      	cmp	r2, #22
30009416:	f000 8391 	beq.w	30009b3c <rxi316_perf_tune+0x89c>
3000941a:	2a17      	cmp	r2, #23
3000941c:	f000 83a0 	beq.w	30009b60 <rxi316_perf_tune+0x8c0>
30009420:	2a18      	cmp	r2, #24
30009422:	f000 839b 	beq.w	30009b5c <rxi316_perf_tune+0x8bc>
30009426:	2a19      	cmp	r2, #25
30009428:	f000 8396 	beq.w	30009b58 <rxi316_perf_tune+0x8b8>
3000942c:	2a1a      	cmp	r2, #26
3000942e:	f000 8391 	beq.w	30009b54 <rxi316_perf_tune+0x8b4>
30009432:	2a1b      	cmp	r2, #27
30009434:	f000 838c 	beq.w	30009b50 <rxi316_perf_tune+0x8b0>
30009438:	2a1c      	cmp	r2, #28
3000943a:	f000 8387 	beq.w	30009b4c <rxi316_perf_tune+0x8ac>
3000943e:	2a1d      	cmp	r2, #29
30009440:	f000 8382 	beq.w	30009b48 <rxi316_perf_tune+0x8a8>
30009444:	2a1e      	cmp	r2, #30
30009446:	f000 837d 	beq.w	30009b44 <rxi316_perf_tune+0x8a4>
3000944a:	2a1f      	cmp	r2, #31
3000944c:	f000 8374 	beq.w	30009b38 <rxi316_perf_tune+0x898>
30009450:	2a20      	cmp	r2, #32
30009452:	f000 836f 	beq.w	30009b34 <rxi316_perf_tune+0x894>
30009456:	9809      	ldr	r0, [sp, #36]	; 0x24
30009458:	2a21      	cmp	r2, #33	; 0x21
3000945a:	bf08      	it	eq
3000945c:	4618      	moveq	r0, r3
3000945e:	9009      	str	r0, [sp, #36]	; 0x24
30009460:	e797      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009462:	4698      	mov	r8, r3
30009464:	3301      	adds	r3, #1
30009466:	3101      	adds	r1, #1
30009468:	2b20      	cmp	r3, #32
3000946a:	d195      	bne.n	30009398 <rxi316_perf_tune+0xf8>
3000946c:	f89c 2002 	ldrb.w	r2, [ip, #2]
30009470:	f8dc 3008 	ldr.w	r3, [ip, #8]
30009474:	b1ba      	cbz	r2, 300094a6 <rxi316_perf_tune+0x206>
30009476:	2a01      	cmp	r2, #1
30009478:	f000 8331 	beq.w	30009ade <rxi316_perf_tune+0x83e>
3000947c:	2a03      	cmp	r2, #3
3000947e:	bf0c      	ite	eq
30009480:	2204      	moveq	r2, #4
30009482:	2203      	movne	r2, #3
30009484:	921e      	str	r2, [sp, #120]	; 0x78
30009486:	e010      	b.n	300094aa <rxi316_perf_tune+0x20a>
30009488:	3101      	adds	r1, #1
3000948a:	461f      	mov	r7, r3
3000948c:	e781      	b.n	30009392 <rxi316_perf_tune+0xf2>
3000948e:	3101      	adds	r1, #1
30009490:	469a      	mov	sl, r3
30009492:	e77e      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009494:	3101      	adds	r1, #1
30009496:	469b      	mov	fp, r3
30009498:	e77b      	b.n	30009392 <rxi316_perf_tune+0xf2>
3000949a:	3101      	adds	r1, #1
3000949c:	9301      	str	r3, [sp, #4]
3000949e:	e778      	b.n	30009392 <rxi316_perf_tune+0xf2>
300094a0:	3101      	adds	r1, #1
300094a2:	9302      	str	r3, [sp, #8]
300094a4:	e775      	b.n	30009392 <rxi316_perf_tune+0xf2>
300094a6:	2201      	movs	r2, #1
300094a8:	921e      	str	r2, [sp, #120]	; 0x78
300094aa:	3b02      	subs	r3, #2
300094ac:	428b      	cmp	r3, r1
300094ae:	d005      	beq.n	300094bc <rxi316_perf_tune+0x21c>
300094b0:	4bac      	ldr	r3, [pc, #688]	; (30009764 <rxi316_perf_tune+0x4c4>)
300094b2:	2002      	movs	r0, #2
300094b4:	4aac      	ldr	r2, [pc, #688]	; (30009768 <rxi316_perf_tune+0x4c8>)
300094b6:	4619      	mov	r1, r3
300094b8:	f001 feb8 	bl	3000b22c <rtk_log_write>
300094bc:	9b1e      	ldr	r3, [sp, #120]	; 0x78
300094be:	9a03      	ldr	r2, [sp, #12]
300094c0:	4293      	cmp	r3, r2
300094c2:	d005      	beq.n	300094d0 <rxi316_perf_tune+0x230>
300094c4:	4ba7      	ldr	r3, [pc, #668]	; (30009764 <rxi316_perf_tune+0x4c4>)
300094c6:	2002      	movs	r0, #2
300094c8:	4aa8      	ldr	r2, [pc, #672]	; (3000976c <rxi316_perf_tune+0x4cc>)
300094ca:	4619      	mov	r1, r3
300094cc:	f001 feae 	bl	3000b22c <rtk_log_write>
300094d0:	9b0e      	ldr	r3, [sp, #56]	; 0x38
300094d2:	2b20      	cmp	r3, #32
300094d4:	f000 82f1 	beq.w	30009aba <rxi316_perf_tune+0x81a>
300094d8:	1b5a      	subs	r2, r3, r5
300094da:	3a05      	subs	r2, #5
300094dc:	f002 030f 	and.w	r3, r2, #15
300094e0:	9303      	str	r3, [sp, #12]
300094e2:	9b0d      	ldr	r3, [sp, #52]	; 0x34
300094e4:	2b20      	cmp	r3, #32
300094e6:	f000 82e6 	beq.w	30009ab6 <rxi316_perf_tune+0x816>
300094ea:	1b5a      	subs	r2, r3, r5
300094ec:	3a06      	subs	r2, #6
300094ee:	0112      	lsls	r2, r2, #4
300094f0:	b2d2      	uxtb	r2, r2
300094f2:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300094f4:	2b20      	cmp	r3, #32
300094f6:	f000 82da 	beq.w	30009aae <rxi316_perf_tune+0x80e>
300094fa:	1b5b      	subs	r3, r3, r5
300094fc:	3b07      	subs	r3, #7
300094fe:	021b      	lsls	r3, r3, #8
30009500:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
30009504:	930c      	str	r3, [sp, #48]	; 0x30
30009506:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009508:	2b20      	cmp	r3, #32
3000950a:	f000 820b 	beq.w	30009924 <rxi316_perf_tune+0x684>
3000950e:	1b5b      	subs	r3, r3, r5
30009510:	3b08      	subs	r3, #8
30009512:	031b      	lsls	r3, r3, #12
30009514:	b29b      	uxth	r3, r3
30009516:	930b      	str	r3, [sp, #44]	; 0x2c
30009518:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000951a:	2b20      	cmp	r3, #32
3000951c:	f000 8209 	beq.w	30009932 <rxi316_perf_tune+0x692>
30009520:	1b5b      	subs	r3, r3, r5
30009522:	3b09      	subs	r3, #9
30009524:	041b      	lsls	r3, r3, #16
30009526:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000952a:	930a      	str	r3, [sp, #40]	; 0x28
3000952c:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000952e:	2b20      	cmp	r3, #32
30009530:	f000 8206 	beq.w	30009940 <rxi316_perf_tune+0x6a0>
30009534:	1b5b      	subs	r3, r3, r5
30009536:	3b0a      	subs	r3, #10
30009538:	051b      	lsls	r3, r3, #20
3000953a:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000953e:	930d      	str	r3, [sp, #52]	; 0x34
30009540:	9b10      	ldr	r3, [sp, #64]	; 0x40
30009542:	2b20      	cmp	r3, #32
30009544:	f000 8203 	beq.w	3000994e <rxi316_perf_tune+0x6ae>
30009548:	1b5b      	subs	r3, r3, r5
3000954a:	3b0b      	subs	r3, #11
3000954c:	061b      	lsls	r3, r3, #24
3000954e:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
30009552:	930e      	str	r3, [sp, #56]	; 0x38
30009554:	9b11      	ldr	r3, [sp, #68]	; 0x44
30009556:	2b20      	cmp	r3, #32
30009558:	f000 8200 	beq.w	3000995c <rxi316_perf_tune+0x6bc>
3000955c:	1b5b      	subs	r3, r3, r5
3000955e:	3b0c      	subs	r3, #12
30009560:	071b      	lsls	r3, r3, #28
30009562:	930f      	str	r3, [sp, #60]	; 0x3c
30009564:	9b12      	ldr	r3, [sp, #72]	; 0x48
30009566:	2b20      	cmp	r3, #32
30009568:	f000 81ff 	beq.w	3000996a <rxi316_perf_tune+0x6ca>
3000956c:	1b5b      	subs	r3, r3, r5
3000956e:	3b0d      	subs	r3, #13
30009570:	f003 030f 	and.w	r3, r3, #15
30009574:	9310      	str	r3, [sp, #64]	; 0x40
30009576:	9b13      	ldr	r3, [sp, #76]	; 0x4c
30009578:	2b20      	cmp	r3, #32
3000957a:	f000 81fc 	beq.w	30009976 <rxi316_perf_tune+0x6d6>
3000957e:	1b5b      	subs	r3, r3, r5
30009580:	3b0e      	subs	r3, #14
30009582:	011b      	lsls	r3, r3, #4
30009584:	b2db      	uxtb	r3, r3
30009586:	9311      	str	r3, [sp, #68]	; 0x44
30009588:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000958a:	2b20      	cmp	r3, #32
3000958c:	f000 81f9 	beq.w	30009982 <rxi316_perf_tune+0x6e2>
30009590:	1b5b      	subs	r3, r3, r5
30009592:	3b0f      	subs	r3, #15
30009594:	021b      	lsls	r3, r3, #8
30009596:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000959a:	9312      	str	r3, [sp, #72]	; 0x48
3000959c:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000959e:	2b20      	cmp	r3, #32
300095a0:	f000 81f6 	beq.w	30009990 <rxi316_perf_tune+0x6f0>
300095a4:	1b5b      	subs	r3, r3, r5
300095a6:	031b      	lsls	r3, r3, #12
300095a8:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
300095ac:	9321      	str	r3, [sp, #132]	; 0x84
300095ae:	9b16      	ldr	r3, [sp, #88]	; 0x58
300095b0:	2b20      	cmp	r3, #32
300095b2:	f000 81f4 	beq.w	3000999e <rxi316_perf_tune+0x6fe>
300095b6:	1b5b      	subs	r3, r3, r5
300095b8:	3b11      	subs	r3, #17
300095ba:	041b      	lsls	r3, r3, #16
300095bc:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
300095c0:	9320      	str	r3, [sp, #128]	; 0x80
300095c2:	9b17      	ldr	r3, [sp, #92]	; 0x5c
300095c4:	2b20      	cmp	r3, #32
300095c6:	f000 81f1 	beq.w	300099ac <rxi316_perf_tune+0x70c>
300095ca:	1b5b      	subs	r3, r3, r5
300095cc:	3b12      	subs	r3, #18
300095ce:	051b      	lsls	r3, r3, #20
300095d0:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
300095d4:	931f      	str	r3, [sp, #124]	; 0x7c
300095d6:	9b18      	ldr	r3, [sp, #96]	; 0x60
300095d8:	2b20      	cmp	r3, #32
300095da:	f000 81ee 	beq.w	300099ba <rxi316_perf_tune+0x71a>
300095de:	1b5b      	subs	r3, r3, r5
300095e0:	3b13      	subs	r3, #19
300095e2:	061b      	lsls	r3, r3, #24
300095e4:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
300095e8:	931e      	str	r3, [sp, #120]	; 0x78
300095ea:	9b19      	ldr	r3, [sp, #100]	; 0x64
300095ec:	2b20      	cmp	r3, #32
300095ee:	f000 81eb 	beq.w	300099c8 <rxi316_perf_tune+0x728>
300095f2:	1b5b      	subs	r3, r3, r5
300095f4:	3b14      	subs	r3, #20
300095f6:	071b      	lsls	r3, r3, #28
300095f8:	9318      	str	r3, [sp, #96]	; 0x60
300095fa:	9b1a      	ldr	r3, [sp, #104]	; 0x68
300095fc:	2b20      	cmp	r3, #32
300095fe:	f000 81ea 	beq.w	300099d6 <rxi316_perf_tune+0x736>
30009602:	1b5b      	subs	r3, r3, r5
30009604:	3b15      	subs	r3, #21
30009606:	f003 030f 	and.w	r3, r3, #15
3000960a:	9317      	str	r3, [sp, #92]	; 0x5c
3000960c:	9b07      	ldr	r3, [sp, #28]
3000960e:	2b20      	cmp	r3, #32
30009610:	f000 81e7 	beq.w	300099e2 <rxi316_perf_tune+0x742>
30009614:	1b5b      	subs	r3, r3, r5
30009616:	3b02      	subs	r3, #2
30009618:	f003 031f 	and.w	r3, r3, #31
3000961c:	9316      	str	r3, [sp, #88]	; 0x58
3000961e:	9b08      	ldr	r3, [sp, #32]
30009620:	2b20      	cmp	r3, #32
30009622:	f000 81e4 	beq.w	300099ee <rxi316_perf_tune+0x74e>
30009626:	1b5b      	subs	r3, r3, r5
30009628:	3b02      	subs	r3, #2
3000962a:	015b      	lsls	r3, r3, #5
3000962c:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
30009630:	9315      	str	r3, [sp, #84]	; 0x54
30009632:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
30009634:	2b20      	cmp	r3, #32
30009636:	f000 81e1 	beq.w	300099fc <rxi316_perf_tune+0x75c>
3000963a:	1b5b      	subs	r3, r3, r5
3000963c:	f1b9 0f20 	cmp.w	r9, #32
30009640:	f1a3 0302 	sub.w	r3, r3, #2
30009644:	ea4f 2383 	mov.w	r3, r3, lsl #10
30009648:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
3000964c:	9314      	str	r3, [sp, #80]	; 0x50
3000964e:	f000 81dc 	beq.w	30009a0a <rxi316_perf_tune+0x76a>
30009652:	eba9 0105 	sub.w	r1, r9, r5
30009656:	f1b8 0f20 	cmp.w	r8, #32
3000965a:	f1a1 0102 	sub.w	r1, r1, #2
3000965e:	f001 030f 	and.w	r3, r1, #15
30009662:	9313      	str	r3, [sp, #76]	; 0x4c
30009664:	f000 81d7 	beq.w	30009a16 <rxi316_perf_tune+0x776>
30009668:	eba8 0105 	sub.w	r1, r8, r5
3000966c:	2f20      	cmp	r7, #32
3000966e:	f1a1 0103 	sub.w	r1, r1, #3
30009672:	ea4f 1101 	mov.w	r1, r1, lsl #4
30009676:	b2c9      	uxtb	r1, r1
30009678:	f000 81d2 	beq.w	30009a20 <rxi316_perf_tune+0x780>
3000967c:	1b7b      	subs	r3, r7, r5
3000967e:	f1ba 0f20 	cmp.w	sl, #32
30009682:	f1a3 0304 	sub.w	r3, r3, #4
30009686:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000968a:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000968e:	9308      	str	r3, [sp, #32]
30009690:	f000 81cd 	beq.w	30009a2e <rxi316_perf_tune+0x78e>
30009694:	ebaa 0305 	sub.w	r3, sl, r5
30009698:	f1bb 0f20 	cmp.w	fp, #32
3000969c:	f1a3 0305 	sub.w	r3, r3, #5
300096a0:	ea4f 3303 	mov.w	r3, r3, lsl #12
300096a4:	b29b      	uxth	r3, r3
300096a6:	9307      	str	r3, [sp, #28]
300096a8:	f000 81c8 	beq.w	30009a3c <rxi316_perf_tune+0x79c>
300096ac:	ebab 0c05 	sub.w	ip, fp, r5
300096b0:	9b01      	ldr	r3, [sp, #4]
300096b2:	f1ac 0c06 	sub.w	ip, ip, #6
300096b6:	2b20      	cmp	r3, #32
300096b8:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
300096bc:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
300096c0:	f000 81c2 	beq.w	30009a48 <rxi316_perf_tune+0x7a8>
300096c4:	eba3 0e05 	sub.w	lr, r3, r5
300096c8:	9b02      	ldr	r3, [sp, #8]
300096ca:	f1ae 0e07 	sub.w	lr, lr, #7
300096ce:	2b20      	cmp	r3, #32
300096d0:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
300096d4:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
300096d8:	f000 81bc 	beq.w	30009a54 <rxi316_perf_tune+0x7b4>
300096dc:	eba3 0805 	sub.w	r8, r3, r5
300096e0:	9b04      	ldr	r3, [sp, #16]
300096e2:	f1a8 0808 	sub.w	r8, r8, #8
300096e6:	2b20      	cmp	r3, #32
300096e8:	ea4f 6808 	mov.w	r8, r8, lsl #24
300096ec:	f008 6870 	and.w	r8, r8, #251658240	; 0xf000000
300096f0:	f000 81b6 	beq.w	30009a60 <rxi316_perf_tune+0x7c0>
300096f4:	1b5b      	subs	r3, r3, r5
300096f6:	3b09      	subs	r3, #9
300096f8:	071b      	lsls	r3, r3, #28
300096fa:	9302      	str	r3, [sp, #8]
300096fc:	9b05      	ldr	r3, [sp, #20]
300096fe:	2b20      	cmp	r3, #32
30009700:	f000 81b5 	beq.w	30009a6e <rxi316_perf_tune+0x7ce>
30009704:	eba3 0905 	sub.w	r9, r3, r5
30009708:	9b06      	ldr	r3, [sp, #24]
3000970a:	f1a9 090a 	sub.w	r9, r9, #10
3000970e:	2b20      	cmp	r3, #32
30009710:	f009 090f 	and.w	r9, r9, #15
30009714:	f000 81b1 	beq.w	30009a7a <rxi316_perf_tune+0x7da>
30009718:	1b58      	subs	r0, r3, r5
3000971a:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000971c:	380b      	subs	r0, #11
3000971e:	2b20      	cmp	r3, #32
30009720:	ea4f 1000 	mov.w	r0, r0, lsl #4
30009724:	b2c0      	uxtb	r0, r0
30009726:	f000 81ad 	beq.w	30009a84 <rxi316_perf_tune+0x7e4>
3000972a:	eba3 0b05 	sub.w	fp, r3, r5
3000972e:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009730:	f1ab 0b02 	sub.w	fp, fp, #2
30009734:	2b20      	cmp	r3, #32
30009736:	f00b 0b1f 	and.w	fp, fp, #31
3000973a:	f000 81a9 	beq.w	30009a90 <rxi316_perf_tune+0x7f0>
3000973e:	eba3 0a05 	sub.w	sl, r3, r5
30009742:	9b1d      	ldr	r3, [sp, #116]	; 0x74
30009744:	f1aa 0a02 	sub.w	sl, sl, #2
30009748:	2b20      	cmp	r3, #32
3000974a:	f00a 0a1f 	and.w	sl, sl, #31
3000974e:	f000 81a5 	beq.w	30009a9c <rxi316_perf_tune+0x7fc>
30009752:	1b5f      	subs	r7, r3, r5
30009754:	2c20      	cmp	r4, #32
30009756:	f1a7 0702 	sub.w	r7, r7, #2
3000975a:	f007 071f 	and.w	r7, r7, #31
3000975e:	f000 81a2 	beq.w	30009aa6 <rxi316_perf_tune+0x806>
30009762:	e005      	b.n	30009770 <rxi316_perf_tune+0x4d0>
30009764:	3000cab4 	.word	0x3000cab4
30009768:	3000cadc 	.word	0x3000cadc
3000976c:	3000cb20 	.word	0x3000cb20
30009770:	1b63      	subs	r3, r4, r5
30009772:	3b02      	subs	r3, #2
30009774:	015b      	lsls	r3, r3, #5
30009776:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000977a:	9301      	str	r3, [sp, #4]
3000977c:	9c03      	ldr	r4, [sp, #12]
3000977e:	ea40 0009 	orr.w	r0, r0, r9
30009782:	4314      	orrs	r4, r2
30009784:	4622      	mov	r2, r4
30009786:	e9dd 4510 	ldrd	r4, r5, [sp, #64]	; 0x40
3000978a:	432c      	orrs	r4, r5
3000978c:	9d13      	ldr	r5, [sp, #76]	; 0x4c
3000978e:	4329      	orrs	r1, r5
30009790:	e9dd 5315 	ldrd	r5, r3, [sp, #84]	; 0x54
30009794:	431d      	orrs	r5, r3
30009796:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30009798:	431a      	orrs	r2, r3
3000979a:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000979c:	431c      	orrs	r4, r3
3000979e:	9b08      	ldr	r3, [sp, #32]
300097a0:	4319      	orrs	r1, r3
300097a2:	9b14      	ldr	r3, [sp, #80]	; 0x50
300097a4:	431d      	orrs	r5, r3
300097a6:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300097a8:	431a      	orrs	r2, r3
300097aa:	9b21      	ldr	r3, [sp, #132]	; 0x84
300097ac:	431c      	orrs	r4, r3
300097ae:	9b07      	ldr	r3, [sp, #28]
300097b0:	4319      	orrs	r1, r3
300097b2:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300097b4:	431a      	orrs	r2, r3
300097b6:	9b20      	ldr	r3, [sp, #128]	; 0x80
300097b8:	ea41 010c 	orr.w	r1, r1, ip
300097bc:	431c      	orrs	r4, r3
300097be:	9b01      	ldr	r3, [sp, #4]
300097c0:	ea41 010e 	orr.w	r1, r1, lr
300097c4:	431f      	orrs	r7, r3
300097c6:	9b0d      	ldr	r3, [sp, #52]	; 0x34
300097c8:	ea41 0108 	orr.w	r1, r1, r8
300097cc:	431a      	orrs	r2, r3
300097ce:	9b1f      	ldr	r3, [sp, #124]	; 0x7c
300097d0:	431c      	orrs	r4, r3
300097d2:	9b0e      	ldr	r3, [sp, #56]	; 0x38
300097d4:	431a      	orrs	r2, r3
300097d6:	9b1e      	ldr	r3, [sp, #120]	; 0x78
300097d8:	4323      	orrs	r3, r4
300097da:	9c0f      	ldr	r4, [sp, #60]	; 0x3c
300097dc:	4322      	orrs	r2, r4
300097de:	9c18      	ldr	r4, [sp, #96]	; 0x60
300097e0:	4323      	orrs	r3, r4
300097e2:	9c02      	ldr	r4, [sp, #8]
300097e4:	430c      	orrs	r4, r1
300097e6:	49ba      	ldr	r1, [pc, #744]	; (30009ad0 <rxi316_perf_tune+0x830>)
300097e8:	f8c1 2400 	str.w	r2, [r1, #1024]	; 0x400
300097ec:	f8c1 3404 	str.w	r3, [r1, #1028]	; 0x404
300097f0:	9b17      	ldr	r3, [sp, #92]	; 0x5c
300097f2:	f8c1 3408 	str.w	r3, [r1, #1032]	; 0x408
300097f6:	f8c1 540c 	str.w	r5, [r1, #1036]	; 0x40c
300097fa:	f8c1 4410 	str.w	r4, [r1, #1040]	; 0x410
300097fe:	f8c1 0414 	str.w	r0, [r1, #1044]	; 0x414
30009802:	f8c1 b418 	str.w	fp, [r1, #1048]	; 0x418
30009806:	f8c1 a41c 	str.w	sl, [r1, #1052]	; 0x41c
3000980a:	f8c1 7420 	str.w	r7, [r1, #1056]	; 0x420
3000980e:	68f3      	ldr	r3, [r6, #12]
30009810:	689a      	ldr	r2, [r3, #8]
30009812:	e9d2 3200 	ldrd	r3, r2, [r2]
30009816:	f003 0301 	and.w	r3, r3, #1
3000981a:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000981e:	f8c1 3460 	str.w	r3, [r1, #1120]	; 0x460
30009822:	68f3      	ldr	r3, [r6, #12]
30009824:	689a      	ldr	r2, [r3, #8]
30009826:	7a13      	ldrb	r3, [r2, #8]
30009828:	6950      	ldr	r0, [r2, #20]
3000982a:	ea43 6300 	orr.w	r3, r3, r0, lsl #24
3000982e:	e9d2 0203 	ldrd	r0, r2, [r2, #12]
30009832:	0200      	lsls	r0, r0, #8
30009834:	0412      	lsls	r2, r2, #16
30009836:	b280      	uxth	r0, r0
30009838:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000983c:	4303      	orrs	r3, r0
3000983e:	4313      	orrs	r3, r2
30009840:	f8c1 3464 	str.w	r3, [r1, #1124]	; 0x464
30009844:	68f3      	ldr	r3, [r6, #12]
30009846:	6898      	ldr	r0, [r3, #8]
30009848:	e9d0 3207 	ldrd	r3, r2, [r0, #28]
3000984c:	7e00      	ldrb	r0, [r0, #24]
3000984e:	021b      	lsls	r3, r3, #8
30009850:	0412      	lsls	r2, r2, #16
30009852:	b29b      	uxth	r3, r3
30009854:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
30009858:	4313      	orrs	r3, r2
3000985a:	4303      	orrs	r3, r0
3000985c:	f8c1 3468 	str.w	r3, [r1, #1128]	; 0x468
30009860:	68f3      	ldr	r3, [r6, #12]
30009862:	689a      	ldr	r2, [r3, #8]
30009864:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
30009868:	041b      	lsls	r3, r3, #16
3000986a:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000986e:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
30009872:	4313      	orrs	r3, r2
30009874:	f8c1 3480 	str.w	r3, [r1, #1152]	; 0x480
30009878:	6833      	ldr	r3, [r6, #0]
3000987a:	781b      	ldrb	r3, [r3, #0]
3000987c:	2b03      	cmp	r3, #3
3000987e:	f000 811f 	beq.w	30009ac0 <rxi316_perf_tune+0x820>
30009882:	68f3      	ldr	r3, [r6, #12]
30009884:	6899      	ldr	r1, [r3, #8]
30009886:	6b0b      	ldr	r3, [r1, #48]	; 0x30
30009888:	005b      	lsls	r3, r3, #1
3000988a:	f003 0302 	and.w	r3, r3, #2
3000988e:	6b8a      	ldr	r2, [r1, #56]	; 0x38
30009890:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
30009892:	0412      	lsls	r2, r2, #16
30009894:	6b49      	ldr	r1, [r1, #52]	; 0x34
30009896:	f000 0001 	and.w	r0, r0, #1
3000989a:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000989e:	0209      	lsls	r1, r1, #8
300098a0:	431a      	orrs	r2, r3
300098a2:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
300098a6:	4b8a      	ldr	r3, [pc, #552]	; (30009ad0 <rxi316_perf_tune+0x830>)
300098a8:	4302      	orrs	r2, r0
300098aa:	488a      	ldr	r0, [pc, #552]	; (30009ad4 <rxi316_perf_tune+0x834>)
300098ac:	430a      	orrs	r2, r1
300098ae:	4310      	orrs	r0, r2
300098b0:	f8c3 0484 	str.w	r0, [r3, #1156]	; 0x484
300098b4:	68f2      	ldr	r2, [r6, #12]
300098b6:	6892      	ldr	r2, [r2, #8]
300098b8:	6bd2      	ldr	r2, [r2, #60]	; 0x3c
300098ba:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
300098be:	68f2      	ldr	r2, [r6, #12]
300098c0:	6892      	ldr	r2, [r2, #8]
300098c2:	6c12      	ldr	r2, [r2, #64]	; 0x40
300098c4:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
300098c8:	68f2      	ldr	r2, [r6, #12]
300098ca:	6892      	ldr	r2, [r2, #8]
300098cc:	6c52      	ldr	r2, [r2, #68]	; 0x44
300098ce:	f8c3 2490 	str.w	r2, [r3, #1168]	; 0x490
300098d2:	68f2      	ldr	r2, [r6, #12]
300098d4:	6892      	ldr	r2, [r2, #8]
300098d6:	6c92      	ldr	r2, [r2, #72]	; 0x48
300098d8:	f8c3 2494 	str.w	r2, [r3, #1172]	; 0x494
300098dc:	68f2      	ldr	r2, [r6, #12]
300098de:	6892      	ldr	r2, [r2, #8]
300098e0:	6cd2      	ldr	r2, [r2, #76]	; 0x4c
300098e2:	f8c3 2498 	str.w	r2, [r3, #1176]	; 0x498
300098e6:	68f2      	ldr	r2, [r6, #12]
300098e8:	6892      	ldr	r2, [r2, #8]
300098ea:	6d12      	ldr	r2, [r2, #80]	; 0x50
300098ec:	f002 020f 	and.w	r2, r2, #15
300098f0:	f8c3 24a0 	str.w	r2, [r3, #1184]	; 0x4a0
300098f4:	f8d3 2470 	ldr.w	r2, [r3, #1136]	; 0x470
300098f8:	f022 023f 	bic.w	r2, r2, #63	; 0x3f
300098fc:	f042 0204 	orr.w	r2, r2, #4
30009900:	f8c3 2470 	str.w	r2, [r3, #1136]	; 0x470
30009904:	f8d3 2474 	ldr.w	r2, [r3, #1140]	; 0x474
30009908:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
3000990c:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
30009910:	f8c3 2474 	str.w	r2, [r3, #1140]	; 0x474
30009914:	b043      	add	sp, #268	; 0x10c
30009916:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000991a:	2502      	movs	r5, #2
3000991c:	e4cf      	b.n	300092be <rxi316_perf_tune+0x1e>
3000991e:	3101      	adds	r1, #1
30009920:	9304      	str	r3, [sp, #16]
30009922:	e536      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009924:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009928:	930b      	str	r3, [sp, #44]	; 0x2c
3000992a:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000992c:	2b20      	cmp	r3, #32
3000992e:	f47f adf7 	bne.w	30009520 <rxi316_perf_tune+0x280>
30009932:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
30009936:	930a      	str	r3, [sp, #40]	; 0x28
30009938:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000993a:	2b20      	cmp	r3, #32
3000993c:	f47f adfa 	bne.w	30009534 <rxi316_perf_tune+0x294>
30009940:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
30009944:	930d      	str	r3, [sp, #52]	; 0x34
30009946:	9b10      	ldr	r3, [sp, #64]	; 0x40
30009948:	2b20      	cmp	r3, #32
3000994a:	f47f adfd 	bne.w	30009548 <rxi316_perf_tune+0x2a8>
3000994e:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
30009952:	930e      	str	r3, [sp, #56]	; 0x38
30009954:	9b11      	ldr	r3, [sp, #68]	; 0x44
30009956:	2b20      	cmp	r3, #32
30009958:	f47f ae00 	bne.w	3000955c <rxi316_perf_tune+0x2bc>
3000995c:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009960:	930f      	str	r3, [sp, #60]	; 0x3c
30009962:	9b12      	ldr	r3, [sp, #72]	; 0x48
30009964:	2b20      	cmp	r3, #32
30009966:	f47f ae01 	bne.w	3000956c <rxi316_perf_tune+0x2cc>
3000996a:	230f      	movs	r3, #15
3000996c:	9310      	str	r3, [sp, #64]	; 0x40
3000996e:	9b13      	ldr	r3, [sp, #76]	; 0x4c
30009970:	2b20      	cmp	r3, #32
30009972:	f47f ae04 	bne.w	3000957e <rxi316_perf_tune+0x2de>
30009976:	23f0      	movs	r3, #240	; 0xf0
30009978:	9311      	str	r3, [sp, #68]	; 0x44
3000997a:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000997c:	2b20      	cmp	r3, #32
3000997e:	f47f ae07 	bne.w	30009590 <rxi316_perf_tune+0x2f0>
30009982:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009986:	9312      	str	r3, [sp, #72]	; 0x48
30009988:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000998a:	2b20      	cmp	r3, #32
3000998c:	f47f ae0a 	bne.w	300095a4 <rxi316_perf_tune+0x304>
30009990:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009994:	9321      	str	r3, [sp, #132]	; 0x84
30009996:	9b16      	ldr	r3, [sp, #88]	; 0x58
30009998:	2b20      	cmp	r3, #32
3000999a:	f47f ae0c 	bne.w	300095b6 <rxi316_perf_tune+0x316>
3000999e:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
300099a2:	9320      	str	r3, [sp, #128]	; 0x80
300099a4:	9b17      	ldr	r3, [sp, #92]	; 0x5c
300099a6:	2b20      	cmp	r3, #32
300099a8:	f47f ae0f 	bne.w	300095ca <rxi316_perf_tune+0x32a>
300099ac:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
300099b0:	931f      	str	r3, [sp, #124]	; 0x7c
300099b2:	9b18      	ldr	r3, [sp, #96]	; 0x60
300099b4:	2b20      	cmp	r3, #32
300099b6:	f47f ae12 	bne.w	300095de <rxi316_perf_tune+0x33e>
300099ba:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
300099be:	931e      	str	r3, [sp, #120]	; 0x78
300099c0:	9b19      	ldr	r3, [sp, #100]	; 0x64
300099c2:	2b20      	cmp	r3, #32
300099c4:	f47f ae15 	bne.w	300095f2 <rxi316_perf_tune+0x352>
300099c8:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
300099cc:	9318      	str	r3, [sp, #96]	; 0x60
300099ce:	9b1a      	ldr	r3, [sp, #104]	; 0x68
300099d0:	2b20      	cmp	r3, #32
300099d2:	f47f ae16 	bne.w	30009602 <rxi316_perf_tune+0x362>
300099d6:	230f      	movs	r3, #15
300099d8:	9317      	str	r3, [sp, #92]	; 0x5c
300099da:	9b07      	ldr	r3, [sp, #28]
300099dc:	2b20      	cmp	r3, #32
300099de:	f47f ae19 	bne.w	30009614 <rxi316_perf_tune+0x374>
300099e2:	231f      	movs	r3, #31
300099e4:	9316      	str	r3, [sp, #88]	; 0x58
300099e6:	9b08      	ldr	r3, [sp, #32]
300099e8:	2b20      	cmp	r3, #32
300099ea:	f47f ae1c 	bne.w	30009626 <rxi316_perf_tune+0x386>
300099ee:	f44f 7378 	mov.w	r3, #992	; 0x3e0
300099f2:	9315      	str	r3, [sp, #84]	; 0x54
300099f4:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
300099f6:	2b20      	cmp	r3, #32
300099f8:	f47f ae1f 	bne.w	3000963a <rxi316_perf_tune+0x39a>
300099fc:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
30009a00:	f1b9 0f20 	cmp.w	r9, #32
30009a04:	9314      	str	r3, [sp, #80]	; 0x50
30009a06:	f47f ae24 	bne.w	30009652 <rxi316_perf_tune+0x3b2>
30009a0a:	230f      	movs	r3, #15
30009a0c:	f1b8 0f20 	cmp.w	r8, #32
30009a10:	9313      	str	r3, [sp, #76]	; 0x4c
30009a12:	f47f ae29 	bne.w	30009668 <rxi316_perf_tune+0x3c8>
30009a16:	2f20      	cmp	r7, #32
30009a18:	f04f 01f0 	mov.w	r1, #240	; 0xf0
30009a1c:	f47f ae2e 	bne.w	3000967c <rxi316_perf_tune+0x3dc>
30009a20:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009a24:	f1ba 0f20 	cmp.w	sl, #32
30009a28:	9308      	str	r3, [sp, #32]
30009a2a:	f47f ae33 	bne.w	30009694 <rxi316_perf_tune+0x3f4>
30009a2e:	f44f 4370 	mov.w	r3, #61440	; 0xf000
30009a32:	f1bb 0f20 	cmp.w	fp, #32
30009a36:	9307      	str	r3, [sp, #28]
30009a38:	f47f ae38 	bne.w	300096ac <rxi316_perf_tune+0x40c>
30009a3c:	9b01      	ldr	r3, [sp, #4]
30009a3e:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
30009a42:	2b20      	cmp	r3, #32
30009a44:	f47f ae3e 	bne.w	300096c4 <rxi316_perf_tune+0x424>
30009a48:	9b02      	ldr	r3, [sp, #8]
30009a4a:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
30009a4e:	2b20      	cmp	r3, #32
30009a50:	f47f ae44 	bne.w	300096dc <rxi316_perf_tune+0x43c>
30009a54:	9b04      	ldr	r3, [sp, #16]
30009a56:	f04f 6870 	mov.w	r8, #251658240	; 0xf000000
30009a5a:	2b20      	cmp	r3, #32
30009a5c:	f47f ae4a 	bne.w	300096f4 <rxi316_perf_tune+0x454>
30009a60:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
30009a64:	9302      	str	r3, [sp, #8]
30009a66:	9b05      	ldr	r3, [sp, #20]
30009a68:	2b20      	cmp	r3, #32
30009a6a:	f47f ae4b 	bne.w	30009704 <rxi316_perf_tune+0x464>
30009a6e:	9b06      	ldr	r3, [sp, #24]
30009a70:	f04f 090f 	mov.w	r9, #15
30009a74:	2b20      	cmp	r3, #32
30009a76:	f47f ae4f 	bne.w	30009718 <rxi316_perf_tune+0x478>
30009a7a:	9b1c      	ldr	r3, [sp, #112]	; 0x70
30009a7c:	20f0      	movs	r0, #240	; 0xf0
30009a7e:	2b20      	cmp	r3, #32
30009a80:	f47f ae53 	bne.w	3000972a <rxi316_perf_tune+0x48a>
30009a84:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009a86:	f04f 0b1f 	mov.w	fp, #31
30009a8a:	2b20      	cmp	r3, #32
30009a8c:	f47f ae57 	bne.w	3000973e <rxi316_perf_tune+0x49e>
30009a90:	9b1d      	ldr	r3, [sp, #116]	; 0x74
30009a92:	f04f 0a1f 	mov.w	sl, #31
30009a96:	2b20      	cmp	r3, #32
30009a98:	f47f ae5b 	bne.w	30009752 <rxi316_perf_tune+0x4b2>
30009a9c:	2c20      	cmp	r4, #32
30009a9e:	f04f 071f 	mov.w	r7, #31
30009aa2:	f47f ae65 	bne.w	30009770 <rxi316_perf_tune+0x4d0>
30009aa6:	f44f 7378 	mov.w	r3, #992	; 0x3e0
30009aaa:	9301      	str	r3, [sp, #4]
30009aac:	e666      	b.n	3000977c <rxi316_perf_tune+0x4dc>
30009aae:	f44f 6370 	mov.w	r3, #3840	; 0xf00
30009ab2:	930c      	str	r3, [sp, #48]	; 0x30
30009ab4:	e527      	b.n	30009506 <rxi316_perf_tune+0x266>
30009ab6:	22f0      	movs	r2, #240	; 0xf0
30009ab8:	e51b      	b.n	300094f2 <rxi316_perf_tune+0x252>
30009aba:	230f      	movs	r3, #15
30009abc:	9303      	str	r3, [sp, #12]
30009abe:	e510      	b.n	300094e2 <rxi316_perf_tune+0x242>
30009ac0:	68f2      	ldr	r2, [r6, #12]
30009ac2:	2302      	movs	r3, #2
30009ac4:	6891      	ldr	r1, [r2, #8]
30009ac6:	e6e2      	b.n	3000988e <rxi316_perf_tune+0x5ee>
30009ac8:	3101      	adds	r1, #1
30009aca:	9305      	str	r3, [sp, #20]
30009acc:	e461      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009ace:	bf00      	nop
30009ad0:	41100000 	.word	0x41100000
30009ad4:	40000008 	.word	0x40000008
30009ad8:	3101      	adds	r1, #1
30009ada:	9306      	str	r3, [sp, #24]
30009adc:	e459      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009ade:	2202      	movs	r2, #2
30009ae0:	921e      	str	r2, [sp, #120]	; 0x78
30009ae2:	e4e2      	b.n	300094aa <rxi316_perf_tune+0x20a>
30009ae4:	9a03      	ldr	r2, [sp, #12]
30009ae6:	9308      	str	r3, [sp, #32]
30009ae8:	3201      	adds	r2, #1
30009aea:	9203      	str	r2, [sp, #12]
30009aec:	e451      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009aee:	9a03      	ldr	r2, [sp, #12]
30009af0:	9307      	str	r3, [sp, #28]
30009af2:	3201      	adds	r2, #1
30009af4:	9203      	str	r2, [sp, #12]
30009af6:	e44c      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009af8:	9a03      	ldr	r2, [sp, #12]
30009afa:	931b      	str	r3, [sp, #108]	; 0x6c
30009afc:	3201      	adds	r2, #1
30009afe:	9203      	str	r2, [sp, #12]
30009b00:	e447      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b02:	9a03      	ldr	r2, [sp, #12]
30009b04:	461c      	mov	r4, r3
30009b06:	3201      	adds	r2, #1
30009b08:	9203      	str	r2, [sp, #12]
30009b0a:	e442      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b0c:	9a03      	ldr	r2, [sp, #12]
30009b0e:	931d      	str	r3, [sp, #116]	; 0x74
30009b10:	3201      	adds	r2, #1
30009b12:	9203      	str	r2, [sp, #12]
30009b14:	e43d      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b16:	9a03      	ldr	r2, [sp, #12]
30009b18:	931c      	str	r3, [sp, #112]	; 0x70
30009b1a:	3201      	adds	r2, #1
30009b1c:	9203      	str	r2, [sp, #12]
30009b1e:	e438      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b20:	930e      	str	r3, [sp, #56]	; 0x38
30009b22:	e436      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b24:	930d      	str	r3, [sp, #52]	; 0x34
30009b26:	e434      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b28:	930c      	str	r3, [sp, #48]	; 0x30
30009b2a:	e432      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b2c:	930a      	str	r3, [sp, #40]	; 0x28
30009b2e:	e430      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b30:	930b      	str	r3, [sp, #44]	; 0x2c
30009b32:	e42e      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b34:	931a      	str	r3, [sp, #104]	; 0x68
30009b36:	e42c      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b38:	9319      	str	r3, [sp, #100]	; 0x64
30009b3a:	e42a      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b3c:	9310      	str	r3, [sp, #64]	; 0x40
30009b3e:	e428      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b40:	930f      	str	r3, [sp, #60]	; 0x3c
30009b42:	e426      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b44:	9318      	str	r3, [sp, #96]	; 0x60
30009b46:	e424      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b48:	9317      	str	r3, [sp, #92]	; 0x5c
30009b4a:	e422      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b4c:	9316      	str	r3, [sp, #88]	; 0x58
30009b4e:	e420      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b50:	9315      	str	r3, [sp, #84]	; 0x54
30009b52:	e41e      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b54:	9314      	str	r3, [sp, #80]	; 0x50
30009b56:	e41c      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b58:	9313      	str	r3, [sp, #76]	; 0x4c
30009b5a:	e41a      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b5c:	9312      	str	r3, [sp, #72]	; 0x48
30009b5e:	e418      	b.n	30009392 <rxi316_perf_tune+0xf2>
30009b60:	9311      	str	r3, [sp, #68]	; 0x44
30009b62:	e416      	b.n	30009392 <rxi316_perf_tune+0xf2>

30009b64 <rxi316_dram_init>:
30009b64:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009b68:	4604      	mov	r4, r0
30009b6a:	b08f      	sub	sp, #60	; 0x3c
30009b6c:	4b10      	ldr	r3, [pc, #64]	; (30009bb0 <rxi316_dram_init+0x4c>)
30009b6e:	4798      	blx	r3
30009b70:	f894 b014 	ldrb.w	fp, [r4, #20]
30009b74:	2101      	movs	r1, #1
30009b76:	900c      	str	r0, [sp, #48]	; 0x30
30009b78:	fa01 f30b 	lsl.w	r3, r1, fp
30009b7c:	9302      	str	r3, [sp, #8]
30009b7e:	6823      	ldr	r3, [r4, #0]
30009b80:	78da      	ldrb	r2, [r3, #3]
30009b82:	6923      	ldr	r3, [r4, #16]
30009b84:	fa03 f30b 	lsl.w	r3, r3, fp
30009b88:	b1a2      	cbz	r2, 30009bb4 <rxi316_dram_init+0x50>
30009b8a:	428a      	cmp	r2, r1
30009b8c:	f000 8469 	beq.w	3000a462 <rxi316_dram_init+0x8fe>
30009b90:	2a02      	cmp	r2, #2
30009b92:	bf0b      	itete	eq
30009b94:	2206      	moveq	r2, #6
30009b96:	220a      	movne	r2, #10
30009b98:	2540      	moveq	r5, #64	; 0x40
30009b9a:	2580      	movne	r5, #128	; 0x80
30009b9c:	9207      	str	r2, [sp, #28]
30009b9e:	bf0c      	ite	eq
30009ba0:	2204      	moveq	r2, #4
30009ba2:	2208      	movne	r2, #8
30009ba4:	9208      	str	r2, [sp, #32]
30009ba6:	bf0c      	ite	eq
30009ba8:	2208      	moveq	r2, #8
30009baa:	2210      	movne	r2, #16
30009bac:	9201      	str	r2, [sp, #4]
30009bae:	e007      	b.n	30009bc0 <rxi316_dram_init+0x5c>
30009bb0:	30007309 	.word	0x30007309
30009bb4:	2203      	movs	r2, #3
30009bb6:	25c0      	movs	r5, #192	; 0xc0
30009bb8:	9108      	str	r1, [sp, #32]
30009bba:	9207      	str	r2, [sp, #28]
30009bbc:	2202      	movs	r2, #2
30009bbe:	9201      	str	r2, [sp, #4]
30009bc0:	6860      	ldr	r0, [r4, #4]
30009bc2:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30009bc6:	4abf      	ldr	r2, [pc, #764]	; (30009ec4 <rxi316_dram_init+0x360>)
30009bc8:	6806      	ldr	r6, [r0, #0]
30009bca:	960d      	str	r6, [sp, #52]	; 0x34
30009bcc:	6846      	ldr	r6, [r0, #4]
30009bce:	6880      	ldr	r0, [r0, #8]
30009bd0:	9609      	str	r6, [sp, #36]	; 0x24
30009bd2:	9003      	str	r0, [sp, #12]
30009bd4:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
30009bd8:	6820      	ldr	r0, [r4, #0]
30009bda:	7981      	ldrb	r1, [r0, #6]
30009bdc:	6882      	ldr	r2, [r0, #8]
30009bde:	b129      	cbz	r1, 30009bec <rxi316_dram_init+0x88>
30009be0:	2901      	cmp	r1, #1
30009be2:	f000 8436 	beq.w	3000a452 <rxi316_dram_init+0x8ee>
30009be6:	2903      	cmp	r1, #3
30009be8:	f000 8443 	beq.w	3000a472 <rxi316_dram_init+0x90e>
30009bec:	3a06      	subs	r2, #6
30009bee:	7881      	ldrb	r1, [r0, #2]
30009bf0:	f002 020f 	and.w	r2, r2, #15
30009bf4:	7940      	ldrb	r0, [r0, #5]
30009bf6:	0109      	lsls	r1, r1, #4
30009bf8:	0280      	lsls	r0, r0, #10
30009bfa:	f001 0130 	and.w	r1, r1, #48	; 0x30
30009bfe:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
30009c02:	4301      	orrs	r1, r0
30009c04:	430d      	orrs	r5, r1
30009c06:	432a      	orrs	r2, r5
30009c08:	4dae      	ldr	r5, [pc, #696]	; (30009ec4 <rxi316_dram_init+0x360>)
30009c0a:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
30009c0e:	68e2      	ldr	r2, [r4, #12]
30009c10:	6812      	ldr	r2, [r2, #0]
30009c12:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
30009c16:	f002 0207 	and.w	r2, r2, #7
30009c1a:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
30009c1e:	68e2      	ldr	r2, [r4, #12]
30009c20:	6811      	ldr	r1, [r2, #0]
30009c22:	6822      	ldr	r2, [r4, #0]
30009c24:	6848      	ldr	r0, [r1, #4]
30009c26:	f892 9000 	ldrb.w	r9, [r2]
30009c2a:	688a      	ldr	r2, [r1, #8]
30009c2c:	9004      	str	r0, [sp, #16]
30009c2e:	0300      	lsls	r0, r0, #12
30009c30:	0412      	lsls	r2, r2, #16
30009c32:	f009 070f 	and.w	r7, r9, #15
30009c36:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
30009c3a:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
30009c3e:	4302      	orrs	r2, r0
30009c40:	68c8      	ldr	r0, [r1, #12]
30009c42:	0440      	lsls	r0, r0, #17
30009c44:	433a      	orrs	r2, r7
30009c46:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
30009c4a:	6908      	ldr	r0, [r1, #16]
30009c4c:	0480      	lsls	r0, r0, #18
30009c4e:	4332      	orrs	r2, r6
30009c50:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
30009c54:	4310      	orrs	r0, r2
30009c56:	694a      	ldr	r2, [r1, #20]
30009c58:	04d2      	lsls	r2, r2, #19
30009c5a:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
30009c5e:	4310      	orrs	r0, r2
30009c60:	698a      	ldr	r2, [r1, #24]
30009c62:	0552      	lsls	r2, r2, #21
30009c64:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
30009c68:	4310      	orrs	r0, r2
30009c6a:	69ca      	ldr	r2, [r1, #28]
30009c6c:	0592      	lsls	r2, r2, #22
30009c6e:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
30009c72:	4310      	orrs	r0, r2
30009c74:	6a8a      	ldr	r2, [r1, #40]	; 0x28
30009c76:	0612      	lsls	r2, r2, #24
30009c78:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
30009c7c:	4302      	orrs	r2, r0
30009c7e:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
30009c82:	06c0      	lsls	r0, r0, #27
30009c84:	0789      	lsls	r1, r1, #30
30009c86:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
30009c8a:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
30009c8e:	4310      	orrs	r0, r2
30009c90:	9a02      	ldr	r2, [sp, #8]
30009c92:	0212      	lsls	r2, r2, #8
30009c94:	4308      	orrs	r0, r1
30009c96:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
30009c9a:	4310      	orrs	r0, r2
30009c9c:	6068      	str	r0, [r5, #4]
30009c9e:	6822      	ldr	r2, [r4, #0]
30009ca0:	7992      	ldrb	r2, [r2, #6]
30009ca2:	2a00      	cmp	r2, #0
30009ca4:	f040 8321 	bne.w	3000a2ea <rxi316_dram_init+0x786>
30009ca8:	686a      	ldr	r2, [r5, #4]
30009caa:	f042 0210 	orr.w	r2, r2, #16
30009cae:	606a      	str	r2, [r5, #4]
30009cb0:	68e2      	ldr	r2, [r4, #12]
30009cb2:	9802      	ldr	r0, [sp, #8]
30009cb4:	6812      	ldr	r2, [r2, #0]
30009cb6:	f100 3aff 	add.w	sl, r0, #4294967295	; 0xffffffff
30009cba:	6861      	ldr	r1, [r4, #4]
30009cbc:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
30009cbe:	f8d1 e010 	ldr.w	lr, [r1, #16]
30009cc2:	9005      	str	r0, [sp, #20]
30009cc4:	6b10      	ldr	r0, [r2, #48]	; 0x30
30009cc6:	fa2e f80b 	lsr.w	r8, lr, fp
30009cca:	6949      	ldr	r1, [r1, #20]
30009ccc:	ea1e 0f0a 	tst.w	lr, sl
30009cd0:	9006      	str	r0, [sp, #24]
30009cd2:	6c55      	ldr	r5, [r2, #68]	; 0x44
30009cd4:	fa21 fe0b 	lsr.w	lr, r1, fp
30009cd8:	bf18      	it	ne
30009cda:	f108 0801 	addne.w	r8, r8, #1
30009cde:	ea11 0f0a 	tst.w	r1, sl
30009ce2:	9906      	ldr	r1, [sp, #24]
30009ce4:	950a      	str	r5, [sp, #40]	; 0x28
30009ce6:	bf18      	it	ne
30009ce8:	f10e 0e01 	addne.w	lr, lr, #1
30009cec:	6c95      	ldr	r5, [r2, #72]	; 0x48
30009cee:	0049      	lsls	r1, r1, #1
30009cf0:	ea4f 3808 	mov.w	r8, r8, lsl #12
30009cf4:	950b      	str	r5, [sp, #44]	; 0x2c
30009cf6:	f001 0102 	and.w	r1, r1, #2
30009cfa:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
30009cfe:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
30009d02:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
30009d06:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
30009d0a:	0080      	lsls	r0, r0, #2
30009d0c:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
30009d10:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
30009d14:	9006      	str	r0, [sp, #24]
30009d16:	013f      	lsls	r7, r7, #4
30009d18:	9805      	ldr	r0, [sp, #20]
30009d1a:	f00c 0c08 	and.w	ip, ip, #8
30009d1e:	0176      	lsls	r6, r6, #5
30009d20:	f007 0710 	and.w	r7, r7, #16
30009d24:	f000 0001 	and.w	r0, r0, #1
30009d28:	f006 0620 	and.w	r6, r6, #32
30009d2c:	4605      	mov	r5, r0
30009d2e:	9806      	ldr	r0, [sp, #24]
30009d30:	f000 0004 	and.w	r0, r0, #4
30009d34:	4301      	orrs	r1, r0
30009d36:	4329      	orrs	r1, r5
30009d38:	ea41 0c0c 	orr.w	ip, r1, ip
30009d3c:	990a      	ldr	r1, [sp, #40]	; 0x28
30009d3e:	ea4c 0707 	orr.w	r7, ip, r7
30009d42:	018d      	lsls	r5, r1, #6
30009d44:	990b      	ldr	r1, [sp, #44]	; 0x2c
30009d46:	4337      	orrs	r7, r6
30009d48:	f005 0540 	and.w	r5, r5, #64	; 0x40
30009d4c:	0208      	lsls	r0, r1, #8
30009d4e:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
30009d50:	432f      	orrs	r7, r5
30009d52:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
30009d56:	06c9      	lsls	r1, r1, #27
30009d58:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
30009d5c:	4307      	orrs	r7, r0
30009d5e:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
30009d62:	0736      	lsls	r6, r6, #28
30009d64:	430f      	orrs	r7, r1
30009d66:	0752      	lsls	r2, r2, #29
30009d68:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
30009d6c:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
30009d70:	433e      	orrs	r6, r7
30009d72:	4316      	orrs	r6, r2
30009d74:	4a53      	ldr	r2, [pc, #332]	; (30009ec4 <rxi316_dram_init+0x360>)
30009d76:	ea46 0608 	orr.w	r6, r6, r8
30009d7a:	ea46 060e 	orr.w	r6, r6, lr
30009d7e:	6096      	str	r6, [r2, #8]
30009d80:	6862      	ldr	r2, [r4, #4]
30009d82:	6990      	ldr	r0, [r2, #24]
30009d84:	9a01      	ldr	r2, [sp, #4]
30009d86:	2a10      	cmp	r2, #16
30009d88:	6822      	ldr	r2, [r4, #0]
30009d8a:	7811      	ldrb	r1, [r2, #0]
30009d8c:	7916      	ldrb	r6, [r2, #4]
30009d8e:	f000 8321 	beq.w	3000a3d4 <rxi316_dram_init+0x870>
30009d92:	f1a1 0502 	sub.w	r5, r1, #2
30009d96:	f1a1 0e09 	sub.w	lr, r1, #9
30009d9a:	9f01      	ldr	r7, [sp, #4]
30009d9c:	fab5 f585 	clz	r5, r5
30009da0:	fabe fe8e 	clz	lr, lr
30009da4:	2f08      	cmp	r7, #8
30009da6:	ea4f 1555 	mov.w	r5, r5, lsr #5
30009daa:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
30009dae:	ea45 020e 	orr.w	r2, r5, lr
30009db2:	f000 82a8 	beq.w	3000a306 <rxi316_dram_init+0x7a2>
30009db6:	9f01      	ldr	r7, [sp, #4]
30009db8:	2f04      	cmp	r7, #4
30009dba:	f000 837c 	beq.w	3000a4b6 <rxi316_dram_init+0x952>
30009dbe:	9f01      	ldr	r7, [sp, #4]
30009dc0:	2f02      	cmp	r7, #2
30009dc2:	bf08      	it	eq
30009dc4:	4310      	orreq	r0, r2
30009dc6:	2a00      	cmp	r2, #0
30009dc8:	f040 82a2 	bne.w	3000a310 <rxi316_dram_init+0x7ac>
30009dcc:	2903      	cmp	r1, #3
30009dce:	f000 8355 	beq.w	3000a47c <rxi316_dram_init+0x918>
30009dd2:	2500      	movs	r5, #0
30009dd4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30009dd8:	462f      	mov	r7, r5
30009dda:	433d      	orrs	r5, r7
30009ddc:	4f39      	ldr	r7, [pc, #228]	; (30009ec4 <rxi316_dram_init+0x360>)
30009dde:	430d      	orrs	r5, r1
30009de0:	4305      	orrs	r5, r0
30009de2:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
30009de6:	637e      	str	r6, [r7, #52]	; 0x34
30009de8:	6862      	ldr	r2, [r4, #4]
30009dea:	9903      	ldr	r1, [sp, #12]
30009dec:	69d2      	ldr	r2, [r2, #28]
30009dee:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
30009df2:	63ba      	str	r2, [r7, #56]	; 0x38
30009df4:	6822      	ldr	r2, [r4, #0]
30009df6:	7810      	ldrb	r0, [r2, #0]
30009df8:	6862      	ldr	r2, [r4, #4]
30009dfa:	2803      	cmp	r0, #3
30009dfc:	6a11      	ldr	r1, [r2, #32]
30009dfe:	f000 82dc 	beq.w	3000a3ba <rxi316_dram_init+0x856>
30009e02:	2809      	cmp	r0, #9
30009e04:	f000 82fa 	beq.w	3000a3fc <rxi316_dram_init+0x898>
30009e08:	482e      	ldr	r0, [pc, #184]	; (30009ec4 <rxi316_dram_init+0x360>)
30009e0a:	63c1      	str	r1, [r0, #60]	; 0x3c
30009e0c:	6862      	ldr	r2, [r4, #4]
30009e0e:	6a52      	ldr	r2, [r2, #36]	; 0x24
30009e10:	6402      	str	r2, [r0, #64]	; 0x40
30009e12:	6860      	ldr	r0, [r4, #4]
30009e14:	68c5      	ldr	r5, [r0, #12]
30009e16:	6802      	ldr	r2, [r0, #0]
30009e18:	6841      	ldr	r1, [r0, #4]
30009e1a:	442a      	add	r2, r5
30009e1c:	6880      	ldr	r0, [r0, #8]
30009e1e:	4429      	add	r1, r5
30009e20:	ea1a 0f02 	tst.w	sl, r2
30009e24:	fa22 f20b 	lsr.w	r2, r2, fp
30009e28:	fa21 f60b 	lsr.w	r6, r1, fp
30009e2c:	fa20 f50b 	lsr.w	r5, r0, fp
30009e30:	bf18      	it	ne
30009e32:	3201      	addne	r2, #1
30009e34:	ea1a 0f01 	tst.w	sl, r1
30009e38:	6821      	ldr	r1, [r4, #0]
30009e3a:	bf18      	it	ne
30009e3c:	3601      	addne	r6, #1
30009e3e:	ea1a 0f00 	tst.w	sl, r0
30009e42:	7808      	ldrb	r0, [r1, #0]
30009e44:	bf18      	it	ne
30009e46:	3501      	addne	r5, #1
30009e48:	68a1      	ldr	r1, [r4, #8]
30009e4a:	3802      	subs	r0, #2
30009e4c:	6dcf      	ldr	r7, [r1, #92]	; 0x5c
30009e4e:	2801      	cmp	r0, #1
30009e50:	f240 82ac 	bls.w	3000a3ac <rxi316_dram_init+0x848>
30009e54:	698f      	ldr	r7, [r1, #24]
30009e56:	fbb7 f0f3 	udiv	r0, r7, r3
30009e5a:	fb03 7710 	mls	r7, r3, r0, r7
30009e5e:	b107      	cbz	r7, 30009e62 <rxi316_dram_init+0x2fe>
30009e60:	3001      	adds	r0, #1
30009e62:	6a4f      	ldr	r7, [r1, #36]	; 0x24
30009e64:	fbb7 f1f3 	udiv	r1, r7, r3
30009e68:	fb03 7711 	mls	r7, r3, r1, r7
30009e6c:	b107      	cbz	r7, 30009e70 <rxi316_dram_init+0x30c>
30009e6e:	3101      	adds	r1, #1
30009e70:	0176      	lsls	r6, r6, #5
30009e72:	f002 021f 	and.w	r2, r2, #31
30009e76:	02ad      	lsls	r5, r5, #10
30009e78:	f406 7678 	and.w	r6, r6, #992	; 0x3e0
30009e7c:	0500      	lsls	r0, r0, #20
30009e7e:	f405 45f8 	and.w	r5, r5, #31744	; 0x7c00
30009e82:	4316      	orrs	r6, r2
30009e84:	f000 70f8 	and.w	r0, r0, #32505856	; 0x1f00000
30009e88:	4a0e      	ldr	r2, [pc, #56]	; (30009ec4 <rxi316_dram_init+0x360>)
30009e8a:	432e      	orrs	r6, r5
30009e8c:	4306      	orrs	r6, r0
30009e8e:	0648      	lsls	r0, r1, #25
30009e90:	f000 5078 	and.w	r0, r0, #1040187392	; 0x3e000000
30009e94:	4330      	orrs	r0, r6
30009e96:	6310      	str	r0, [r2, #48]	; 0x30
30009e98:	68a0      	ldr	r0, [r4, #8]
30009e9a:	6802      	ldr	r2, [r0, #0]
30009e9c:	fbb2 f1f3 	udiv	r1, r2, r3
30009ea0:	fb03 2211 	mls	r2, r3, r1, r2
30009ea4:	2a00      	cmp	r2, #0
30009ea6:	f000 827f 	beq.w	3000a3a8 <rxi316_dram_init+0x844>
30009eaa:	1c4a      	adds	r2, r1, #1
30009eac:	9205      	str	r2, [sp, #20]
30009eae:	6842      	ldr	r2, [r0, #4]
30009eb0:	fbb2 f0f3 	udiv	r0, r2, r3
30009eb4:	fb03 2210 	mls	r2, r3, r0, r2
30009eb8:	2a00      	cmp	r2, #0
30009eba:	f000 8273 	beq.w	3000a3a4 <rxi316_dram_init+0x840>
30009ebe:	38c7      	subs	r0, #199	; 0xc7
30009ec0:	e002      	b.n	30009ec8 <rxi316_dram_init+0x364>
30009ec2:	bf00      	nop
30009ec4:	41100000 	.word	0x41100000
30009ec8:	68e2      	ldr	r2, [r4, #12]
30009eca:	6811      	ldr	r1, [r2, #0]
30009ecc:	4ac3      	ldr	r2, [pc, #780]	; (3000a1dc <rxi316_dram_init+0x678>)
30009ece:	ea02 2000 	and.w	r0, r2, r0, lsl #8
30009ed2:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
30009ed6:	900a      	str	r0, [sp, #40]	; 0x28
30009ed8:	0712      	lsls	r2, r2, #28
30009eda:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
30009ede:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
30009ee2:	430a      	orrs	r2, r1
30009ee4:	f89d 1014 	ldrb.w	r1, [sp, #20]
30009ee8:	430a      	orrs	r2, r1
30009eea:	49bd      	ldr	r1, [pc, #756]	; (3000a1e0 <rxi316_dram_init+0x67c>)
30009eec:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
30009ef0:	610a      	str	r2, [r1, #16]
30009ef2:	68a5      	ldr	r5, [r4, #8]
30009ef4:	692a      	ldr	r2, [r5, #16]
30009ef6:	fbb2 fcf3 	udiv	ip, r2, r3
30009efa:	fb03 221c 	mls	r2, r3, ip, r2
30009efe:	2a00      	cmp	r2, #0
30009f00:	f000 824d 	beq.w	3000a39e <rxi316_dram_init+0x83a>
30009f04:	f10c 0201 	add.w	r2, ip, #1
30009f08:	9206      	str	r2, [sp, #24]
30009f0a:	69aa      	ldr	r2, [r5, #24]
30009f0c:	fbb2 f0f3 	udiv	r0, r2, r3
30009f10:	fb03 2210 	mls	r2, r3, r0, r2
30009f14:	b102      	cbz	r2, 30009f18 <rxi316_dram_init+0x3b4>
30009f16:	3001      	adds	r0, #1
30009f18:	68ea      	ldr	r2, [r5, #12]
30009f1a:	fbb2 f1f3 	udiv	r1, r2, r3
30009f1e:	fb03 2211 	mls	r2, r3, r1, r2
30009f22:	9103      	str	r1, [sp, #12]
30009f24:	b10a      	cbz	r2, 30009f2a <rxi316_dram_init+0x3c6>
30009f26:	3101      	adds	r1, #1
30009f28:	9103      	str	r1, [sp, #12]
30009f2a:	6a6a      	ldr	r2, [r5, #36]	; 0x24
30009f2c:	fbb2 f1f3 	udiv	r1, r2, r3
30009f30:	fb03 2211 	mls	r2, r3, r1, r2
30009f34:	b102      	cbz	r2, 30009f38 <rxi316_dram_init+0x3d4>
30009f36:	3101      	adds	r1, #1
30009f38:	6822      	ldr	r2, [r4, #0]
30009f3a:	7812      	ldrb	r2, [r2, #0]
30009f3c:	2a09      	cmp	r2, #9
30009f3e:	f000 8259 	beq.w	3000a3f4 <rxi316_dram_init+0x890>
30009f42:	2a02      	cmp	r2, #2
30009f44:	d104      	bne.n	30009f50 <rxi316_dram_init+0x3ec>
30009f46:	9a08      	ldr	r2, [sp, #32]
30009f48:	3a02      	subs	r2, #2
30009f4a:	fa22 f20b 	lsr.w	r2, r2, fp
30009f4e:	4411      	add	r1, r2
30009f50:	68e2      	ldr	r2, [r4, #12]
30009f52:	0349      	lsls	r1, r1, #13
30009f54:	6d2d      	ldr	r5, [r5, #80]	; 0x50
30009f56:	6812      	ldr	r2, [r2, #0]
30009f58:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
30009f5c:	fa25 f60b 	lsr.w	r6, r5, fp
30009f60:	ea15 0f0a 	tst.w	r5, sl
30009f64:	6e52      	ldr	r2, [r2, #100]	; 0x64
30009f66:	bf18      	it	ne
30009f68:	3601      	addne	r6, #1
30009f6a:	05d2      	lsls	r2, r2, #23
30009f6c:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
30009f70:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
30009f74:	9a06      	ldr	r2, [sp, #24]
30009f76:	0112      	lsls	r2, r2, #4
30009f78:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
30009f7c:	4332      	orrs	r2, r6
30009f7e:	0246      	lsls	r6, r0, #9
30009f80:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
30009f84:	4316      	orrs	r6, r2
30009f86:	9a03      	ldr	r2, [sp, #12]
30009f88:	f002 020f 	and.w	r2, r2, #15
30009f8c:	4316      	orrs	r6, r2
30009f8e:	4a94      	ldr	r2, [pc, #592]	; (3000a1e0 <rxi316_dram_init+0x67c>)
30009f90:	4331      	orrs	r1, r6
30009f92:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
30009f96:	6151      	str	r1, [r2, #20]
30009f98:	68a7      	ldr	r7, [r4, #8]
30009f9a:	697a      	ldr	r2, [r7, #20]
30009f9c:	fbb2 fef3 	udiv	lr, r2, r3
30009fa0:	fb03 221e 	mls	r2, r3, lr, r2
30009fa4:	b10a      	cbz	r2, 30009faa <rxi316_dram_init+0x446>
30009fa6:	f10e 0e01 	add.w	lr, lr, #1
30009faa:	6939      	ldr	r1, [r7, #16]
30009fac:	fbb1 f2f3 	udiv	r2, r1, r3
30009fb0:	fb03 1112 	mls	r1, r3, r2, r1
30009fb4:	b101      	cbz	r1, 30009fb8 <rxi316_dram_init+0x454>
30009fb6:	3201      	adds	r2, #1
30009fb8:	68fd      	ldr	r5, [r7, #12]
30009fba:	fbb5 f1f3 	udiv	r1, r5, r3
30009fbe:	fb03 5511 	mls	r5, r3, r1, r5
30009fc2:	b105      	cbz	r5, 30009fc6 <rxi316_dram_init+0x462>
30009fc4:	3101      	adds	r1, #1
30009fc6:	6b3d      	ldr	r5, [r7, #48]	; 0x30
30009fc8:	fbb5 f6f3 	udiv	r6, r5, r3
30009fcc:	fb03 5516 	mls	r5, r3, r6, r5
30009fd0:	b105      	cbz	r5, 30009fd4 <rxi316_dram_init+0x470>
30009fd2:	3601      	adds	r6, #1
30009fd4:	68bd      	ldr	r5, [r7, #8]
30009fd6:	4411      	add	r1, r2
30009fd8:	fbb5 f2f3 	udiv	r2, r5, r3
30009fdc:	42b1      	cmp	r1, r6
30009fde:	fb03 5512 	mls	r5, r3, r2, r5
30009fe2:	bf38      	it	cc
30009fe4:	4631      	movcc	r1, r6
30009fe6:	b905      	cbnz	r5, 30009fea <rxi316_dram_init+0x486>
30009fe8:	3a01      	subs	r2, #1
30009fea:	6afd      	ldr	r5, [r7, #44]	; 0x2c
30009fec:	ea15 0f0a 	tst.w	r5, sl
30009ff0:	fa25 fc0b 	lsr.w	ip, r5, fp
30009ff4:	69fd      	ldr	r5, [r7, #28]
30009ff6:	bf18      	it	ne
30009ff8:	f10c 0c01 	addne.w	ip, ip, #1
30009ffc:	fa25 f60b 	lsr.w	r6, r5, fp
3000a000:	ea15 0f0a 	tst.w	r5, sl
3000a004:	bf18      	it	ne
3000a006:	3601      	addne	r6, #1
3000a008:	f1b9 0f09 	cmp.w	r9, #9
3000a00c:	f000 81ed 	beq.w	3000a3ea <rxi316_dram_init+0x886>
3000a010:	f1b9 0f02 	cmp.w	r9, #2
3000a014:	f000 81f4 	beq.w	3000a400 <rxi316_dram_init+0x89c>
3000a018:	f1b9 0f03 	cmp.w	r9, #3
3000a01c:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000a01e:	f000 827d 	beq.w	3000a51c <rxi316_dram_init+0x9b8>
3000a022:	46a8      	mov	r8, r5
3000a024:	9d01      	ldr	r5, [sp, #4]
3000a026:	44a8      	add	r8, r5
3000a028:	fa28 f50b 	lsr.w	r5, r8, fp
3000a02c:	ea18 0f0a 	tst.w	r8, sl
3000a030:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000a034:	bf18      	it	ne
3000a036:	3501      	addne	r5, #1
3000a038:	fbb8 f7f3 	udiv	r7, r8, r3
3000a03c:	fb03 8817 	mls	r8, r3, r7, r8
3000a040:	f1b8 0f00 	cmp.w	r8, #0
3000a044:	d000      	beq.n	3000a048 <rxi316_dram_init+0x4e4>
3000a046:	3701      	adds	r7, #1
3000a048:	0109      	lsls	r1, r1, #4
3000a04a:	f00e 0e0f 	and.w	lr, lr, #15
3000a04e:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000a052:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000a056:	063f      	lsls	r7, r7, #24
3000a058:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000a05c:	ea41 0e0e 	orr.w	lr, r1, lr
3000a060:	0291      	lsls	r1, r2, #10
3000a062:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000a066:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000a06a:	ea4e 0e01 	orr.w	lr, lr, r1
3000a06e:	0471      	lsls	r1, r6, #17
3000a070:	ea4e 0c0c 	orr.w	ip, lr, ip
3000a074:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000a078:	ea4c 0c01 	orr.w	ip, ip, r1
3000a07c:	0529      	lsls	r1, r5, #20
3000a07e:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a082:	ea4c 0c01 	orr.w	ip, ip, r1
3000a086:	4956      	ldr	r1, [pc, #344]	; (3000a1e0 <rxi316_dram_init+0x67c>)
3000a088:	ea4c 0707 	orr.w	r7, ip, r7
3000a08c:	618f      	str	r7, [r1, #24]
3000a08e:	6821      	ldr	r1, [r4, #0]
3000a090:	780f      	ldrb	r7, [r1, #0]
3000a092:	2f02      	cmp	r7, #2
3000a094:	f000 8153 	beq.w	3000a33e <rxi316_dram_init+0x7da>
3000a098:	2f09      	cmp	r7, #9
3000a09a:	f000 8150 	beq.w	3000a33e <rxi316_dram_init+0x7da>
3000a09e:	68e1      	ldr	r1, [r4, #12]
3000a0a0:	2f03      	cmp	r7, #3
3000a0a2:	6809      	ldr	r1, [r1, #0]
3000a0a4:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000a0a6:	f001 080f 	and.w	r8, r1, #15
3000a0aa:	68a1      	ldr	r1, [r4, #8]
3000a0ac:	6a09      	ldr	r1, [r1, #32]
3000a0ae:	d005      	beq.n	3000a0bc <rxi316_dram_init+0x558>
3000a0b0:	ea0a 0701 	and.w	r7, sl, r1
3000a0b4:	fa21 f10b 	lsr.w	r1, r1, fp
3000a0b8:	b107      	cbz	r7, 3000a0bc <rxi316_dram_init+0x558>
3000a0ba:	3101      	adds	r1, #1
3000a0bc:	f1b9 0f02 	cmp.w	r9, #2
3000a0c0:	f000 8155 	beq.w	3000a36e <rxi316_dram_init+0x80a>
3000a0c4:	f1b9 0f03 	cmp.w	r9, #3
3000a0c8:	f000 819d 	beq.w	3000a406 <rxi316_dram_init+0x8a2>
3000a0cc:	f1b9 0f09 	cmp.w	r9, #9
3000a0d0:	f000 8214 	beq.w	3000a4fc <rxi316_dram_init+0x998>
3000a0d4:	f04f 0e00 	mov.w	lr, #0
3000a0d8:	46f1      	mov	r9, lr
3000a0da:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000a0de:	0109      	lsls	r1, r1, #4
3000a0e0:	9f06      	ldr	r7, [sp, #24]
3000a0e2:	b2c9      	uxtb	r1, r1
3000a0e4:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000a0e8:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a0ea:	ea41 0108 	orr.w	r1, r1, r8
3000a0ee:	4339      	orrs	r1, r7
3000a0f0:	4f3b      	ldr	r7, [pc, #236]	; (3000a1e0 <rxi316_dram_init+0x67c>)
3000a0f2:	ea41 0109 	orr.w	r1, r1, r9
3000a0f6:	ea41 010e 	orr.w	r1, r1, lr
3000a0fa:	61f9      	str	r1, [r7, #28]
3000a0fc:	68a7      	ldr	r7, [r4, #8]
3000a0fe:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000a100:	fbb4 f1f3 	udiv	r1, r4, r3
3000a104:	fb03 4311 	mls	r3, r3, r1, r4
3000a108:	b103      	cbz	r3, 3000a10c <rxi316_dram_init+0x5a8>
3000a10a:	3101      	adds	r1, #1
3000a10c:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000a10e:	094b      	lsrs	r3, r1, #5
3000a110:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000a114:	0912      	lsrs	r2, r2, #4
3000a116:	ea14 0f0a 	tst.w	r4, sl
3000a11a:	fa24 f80b 	lsr.w	r8, r4, fp
3000a11e:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000a120:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000a124:	bf18      	it	ne
3000a126:	f108 0801 	addne.w	r8, r8, #1
3000a12a:	9306      	str	r3, [sp, #24]
3000a12c:	ea14 0f0a 	tst.w	r4, sl
3000a130:	fa24 fe0b 	lsr.w	lr, r4, fp
3000a134:	9b01      	ldr	r3, [sp, #4]
3000a136:	fa29 f40b 	lsr.w	r4, r9, fp
3000a13a:	bf18      	it	ne
3000a13c:	f10e 0e01 	addne.w	lr, lr, #1
3000a140:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000a142:	ea19 0f0a 	tst.w	r9, sl
3000a146:	fa23 f90b 	lsr.w	r9, r3, fp
3000a14a:	9905      	ldr	r1, [sp, #20]
3000a14c:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000a150:	bf18      	it	ne
3000a152:	3401      	addne	r4, #1
3000a154:	45b9      	cmp	r9, r7
3000a156:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000a15a:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000a15e:	f240 80ec 	bls.w	3000a33a <rxi316_dram_init+0x7d6>
3000a162:	eba9 0707 	sub.w	r7, r9, r7
3000a166:	fa27 f40b 	lsr.w	r4, r7, fp
3000a16a:	ea17 070a 	ands.w	r7, r7, sl
3000a16e:	d001      	beq.n	3000a174 <rxi316_dram_init+0x610>
3000a170:	3401      	adds	r4, #1
3000a172:	2700      	movs	r7, #0
3000a174:	9b04      	ldr	r3, [sp, #16]
3000a176:	b10b      	cbz	r3, 3000a17c <rxi316_dram_init+0x618>
3000a178:	073b      	lsls	r3, r7, #28
3000a17a:	9304      	str	r3, [sp, #16]
3000a17c:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000a180:	9b06      	ldr	r3, [sp, #24]
3000a182:	0289      	lsls	r1, r1, #10
3000a184:	02c0      	lsls	r0, r0, #11
3000a186:	f00c 0c04 	and.w	ip, ip, #4
3000a18a:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000a18e:	01d2      	lsls	r2, r2, #7
3000a190:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000a194:	0236      	lsls	r6, r6, #8
3000a196:	ea4c 0c01 	orr.w	ip, ip, r1
3000a19a:	b2d2      	uxtb	r2, r2
3000a19c:	026d      	lsls	r5, r5, #9
3000a19e:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000a1a2:	ea4c 0c00 	orr.w	ip, ip, r0
3000a1a6:	00df      	lsls	r7, r3, #3
3000a1a8:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000a1ac:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000a1b0:	ea4c 0202 	orr.w	r2, ip, r2
3000a1b4:	f007 0708 	and.w	r7, r7, #8
3000a1b8:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000a1bc:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000a1c0:	4332      	orrs	r2, r6
3000a1c2:	0524      	lsls	r4, r4, #20
3000a1c4:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000a1c8:	4905      	ldr	r1, [pc, #20]	; (3000a1e0 <rxi316_dram_init+0x67c>)
3000a1ca:	ea42 0305 	orr.w	r3, r2, r5
3000a1ce:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000a1d2:	9a04      	ldr	r2, [sp, #16]
3000a1d4:	433b      	orrs	r3, r7
3000a1d6:	ea43 0308 	orr.w	r3, r3, r8
3000a1da:	e003      	b.n	3000a1e4 <rxi316_dram_init+0x680>
3000a1dc:	00ffff00 	.word	0x00ffff00
3000a1e0:	41100000 	.word	0x41100000
3000a1e4:	ea43 030e 	orr.w	r3, r3, lr
3000a1e8:	4323      	orrs	r3, r4
3000a1ea:	4313      	orrs	r3, r2
3000a1ec:	9a03      	ldr	r2, [sp, #12]
3000a1ee:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000a1f2:	620b      	str	r3, [r1, #32]
3000a1f4:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000a1f8:	9b02      	ldr	r3, [sp, #8]
3000a1fa:	2b01      	cmp	r3, #1
3000a1fc:	d002      	beq.n	3000a204 <rxi316_dram_init+0x6a0>
3000a1fe:	9b08      	ldr	r3, [sp, #32]
3000a200:	3303      	adds	r3, #3
3000a202:	9307      	str	r3, [sp, #28]
3000a204:	4bb2      	ldr	r3, [pc, #712]	; (3000a4d0 <rxi316_dram_init+0x96c>)
3000a206:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000a20a:	9a07      	ldr	r2, [sp, #28]
3000a20c:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000a210:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000a214:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000a218:	60da      	str	r2, [r3, #12]
3000a21a:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a21e:	430a      	orrs	r2, r1
3000a220:	49ac      	ldr	r1, [pc, #688]	; (3000a4d4 <rxi316_dram_init+0x970>)
3000a222:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a226:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000a22a:	0096      	lsls	r6, r2, #2
3000a22c:	d57f      	bpl.n	3000a32e <rxi316_dram_init+0x7ca>
3000a22e:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000a232:	4ca7      	ldr	r4, [pc, #668]	; (3000a4d0 <rxi316_dram_init+0x96c>)
3000a234:	601a      	str	r2, [r3, #0]
3000a236:	691a      	ldr	r2, [r3, #16]
3000a238:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000a23c:	611a      	str	r2, [r3, #16]
3000a23e:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000a242:	f043 0303 	orr.w	r3, r3, #3
3000a246:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000a24a:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000a24e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a250:	2b01      	cmp	r3, #1
3000a252:	4ba1      	ldr	r3, [pc, #644]	; (3000a4d8 <rxi316_dram_init+0x974>)
3000a254:	bf0c      	ite	eq
3000a256:	4aa1      	ldreq	r2, [pc, #644]	; (3000a4dc <rxi316_dram_init+0x978>)
3000a258:	4aa1      	ldrne	r2, [pc, #644]	; (3000a4e0 <rxi316_dram_init+0x97c>)
3000a25a:	601a      	str	r2, [r3, #0]
3000a25c:	2203      	movs	r2, #3
3000a25e:	68e0      	ldr	r0, [r4, #12]
3000a260:	4ba0      	ldr	r3, [pc, #640]	; (3000a4e4 <rxi316_dram_init+0x980>)
3000a262:	4003      	ands	r3, r0
3000a264:	60e3      	str	r3, [r4, #12]
3000a266:	2308      	movs	r3, #8
3000a268:	62a1      	str	r1, [r4, #40]	; 0x28
3000a26a:	62e2      	str	r2, [r4, #44]	; 0x2c
3000a26c:	6023      	str	r3, [r4, #0]
3000a26e:	6823      	ldr	r3, [r4, #0]
3000a270:	071d      	lsls	r5, r3, #28
3000a272:	d5fc      	bpl.n	3000a26e <rxi316_dram_init+0x70a>
3000a274:	4b9c      	ldr	r3, [pc, #624]	; (3000a4e8 <rxi316_dram_init+0x984>)
3000a276:	2001      	movs	r0, #1
3000a278:	4798      	blx	r3
3000a27a:	68e2      	ldr	r2, [r4, #12]
3000a27c:	4b99      	ldr	r3, [pc, #612]	; (3000a4e4 <rxi316_dram_init+0x980>)
3000a27e:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000a282:	4013      	ands	r3, r2
3000a284:	2203      	movs	r2, #3
3000a286:	60e3      	str	r3, [r4, #12]
3000a288:	2308      	movs	r3, #8
3000a28a:	62a1      	str	r1, [r4, #40]	; 0x28
3000a28c:	62e2      	str	r2, [r4, #44]	; 0x2c
3000a28e:	4a90      	ldr	r2, [pc, #576]	; (3000a4d0 <rxi316_dram_init+0x96c>)
3000a290:	6023      	str	r3, [r4, #0]
3000a292:	6813      	ldr	r3, [r2, #0]
3000a294:	071c      	lsls	r4, r3, #28
3000a296:	d5fc      	bpl.n	3000a292 <rxi316_dram_init+0x72e>
3000a298:	68d0      	ldr	r0, [r2, #12]
3000a29a:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000a29e:	4b91      	ldr	r3, [pc, #580]	; (3000a4e4 <rxi316_dram_init+0x980>)
3000a2a0:	4003      	ands	r3, r0
3000a2a2:	2008      	movs	r0, #8
3000a2a4:	60d3      	str	r3, [r2, #12]
3000a2a6:	6291      	str	r1, [r2, #40]	; 0x28
3000a2a8:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000a2aa:	4989      	ldr	r1, [pc, #548]	; (3000a4d0 <rxi316_dram_init+0x96c>)
3000a2ac:	f023 0302 	bic.w	r3, r3, #2
3000a2b0:	62d3      	str	r3, [r2, #44]	; 0x2c
3000a2b2:	6010      	str	r0, [r2, #0]
3000a2b4:	680b      	ldr	r3, [r1, #0]
3000a2b6:	0718      	lsls	r0, r3, #28
3000a2b8:	d5fc      	bpl.n	3000a2b4 <rxi316_dram_init+0x750>
3000a2ba:	4a85      	ldr	r2, [pc, #532]	; (3000a4d0 <rxi316_dram_init+0x96c>)
3000a2bc:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000a2c0:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a2c4:	60d3      	str	r3, [r2, #12]
3000a2c6:	980a      	ldr	r0, [sp, #40]	; 0x28
3000a2c8:	6913      	ldr	r3, [r2, #16]
3000a2ca:	4318      	orrs	r0, r3
3000a2cc:	6110      	str	r0, [r2, #16]
3000a2ce:	6913      	ldr	r3, [r2, #16]
3000a2d0:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000a2d4:	6113      	str	r3, [r2, #16]
3000a2d6:	6011      	str	r1, [r2, #0]
3000a2d8:	68d3      	ldr	r3, [r2, #12]
3000a2da:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000a2de:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000a2e2:	d1f9      	bne.n	3000a2d8 <rxi316_dram_init+0x774>
3000a2e4:	b00f      	add	sp, #60	; 0x3c
3000a2e6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a2ea:	2a01      	cmp	r2, #1
3000a2ec:	f000 80b4 	beq.w	3000a458 <rxi316_dram_init+0x8f4>
3000a2f0:	2a02      	cmp	r2, #2
3000a2f2:	f000 811d 	beq.w	3000a530 <rxi316_dram_init+0x9cc>
3000a2f6:	2a03      	cmp	r2, #3
3000a2f8:	f47f acda 	bne.w	30009cb0 <rxi316_dram_init+0x14c>
3000a2fc:	686a      	ldr	r2, [r5, #4]
3000a2fe:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a302:	606a      	str	r2, [r5, #4]
3000a304:	e4d4      	b.n	30009cb0 <rxi316_dram_init+0x14c>
3000a306:	2a00      	cmp	r2, #0
3000a308:	f43f ad60 	beq.w	30009dcc <rxi316_dram_init+0x268>
3000a30c:	2203      	movs	r2, #3
3000a30e:	4310      	orrs	r0, r2
3000a310:	68a2      	ldr	r2, [r4, #8]
3000a312:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a314:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000a316:	013f      	lsls	r7, r7, #4
3000a318:	2d00      	cmp	r5, #0
3000a31a:	f000 80ba 	beq.w	3000a492 <rxi316_dram_init+0x92e>
3000a31e:	1e91      	subs	r1, r2, #2
3000a320:	2906      	cmp	r1, #6
3000a322:	f200 8093 	bhi.w	3000a44c <rxi316_dram_init+0x8e8>
3000a326:	1e55      	subs	r5, r2, #1
3000a328:	2100      	movs	r1, #0
3000a32a:	026d      	lsls	r5, r5, #9
3000a32c:	e555      	b.n	30009dda <rxi316_dram_init+0x276>
3000a32e:	2201      	movs	r2, #1
3000a330:	601a      	str	r2, [r3, #0]
3000a332:	681a      	ldr	r2, [r3, #0]
3000a334:	07d2      	lsls	r2, r2, #31
3000a336:	d5fc      	bpl.n	3000a332 <rxi316_dram_init+0x7ce>
3000a338:	e7bf      	b.n	3000a2ba <rxi316_dram_init+0x756>
3000a33a:	2701      	movs	r7, #1
3000a33c:	e71a      	b.n	3000a174 <rxi316_dram_init+0x610>
3000a33e:	68a1      	ldr	r1, [r4, #8]
3000a340:	2f03      	cmp	r7, #3
3000a342:	6a09      	ldr	r1, [r1, #32]
3000a344:	f000 80a2 	beq.w	3000a48c <rxi316_dram_init+0x928>
3000a348:	2f02      	cmp	r7, #2
3000a34a:	f040 80fc 	bne.w	3000a546 <rxi316_dram_init+0x9e2>
3000a34e:	fbb1 fcf3 	udiv	ip, r1, r3
3000a352:	fb03 1e1c 	mls	lr, r3, ip, r1
3000a356:	4661      	mov	r1, ip
3000a358:	f1be 0f00 	cmp.w	lr, #0
3000a35c:	f000 808c 	beq.w	3000a478 <rxi316_dram_init+0x914>
3000a360:	f1b9 0f02 	cmp.w	r9, #2
3000a364:	f101 0101 	add.w	r1, r1, #1
3000a368:	46b8      	mov	r8, r7
3000a36a:	f47f aeab 	bne.w	3000a0c4 <rxi316_dram_init+0x560>
3000a36e:	4f5f      	ldr	r7, [pc, #380]	; (3000a4ec <rxi316_dram_init+0x988>)
3000a370:	f04f 0e00 	mov.w	lr, #0
3000a374:	f8df 9178 	ldr.w	r9, [pc, #376]	; 3000a4f0 <rxi316_dram_init+0x98c>
3000a378:	fbb7 f7f3 	udiv	r7, r7, r3
3000a37c:	fbb9 f9f3 	udiv	r9, r9, r3
3000a380:	09ff      	lsrs	r7, r7, #7
3000a382:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a386:	3701      	adds	r7, #1
3000a388:	f109 0c01 	add.w	ip, r9, #1
3000a38c:	f8df 9164 	ldr.w	r9, [pc, #356]	; 3000a4f4 <rxi316_dram_init+0x990>
3000a390:	023f      	lsls	r7, r7, #8
3000a392:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000a396:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000a39a:	9709      	str	r7, [sp, #36]	; 0x24
3000a39c:	e69f      	b.n	3000a0de <rxi316_dram_init+0x57a>
3000a39e:	f8cd c018 	str.w	ip, [sp, #24]
3000a3a2:	e5b2      	b.n	30009f0a <rxi316_dram_init+0x3a6>
3000a3a4:	38c8      	subs	r0, #200	; 0xc8
3000a3a6:	e58f      	b.n	30009ec8 <rxi316_dram_init+0x364>
3000a3a8:	9105      	str	r1, [sp, #20]
3000a3aa:	e580      	b.n	30009eae <rxi316_dram_init+0x34a>
3000a3ac:	ea1a 0f07 	tst.w	sl, r7
3000a3b0:	fa27 f00b 	lsr.w	r0, r7, fp
3000a3b4:	f43f ad55 	beq.w	30009e62 <rxi316_dram_init+0x2fe>
3000a3b8:	e552      	b.n	30009e60 <rxi316_dram_init+0x2fc>
3000a3ba:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000a3bc:	1f50      	subs	r0, r2, #5
3000a3be:	00c0      	lsls	r0, r0, #3
3000a3c0:	68e2      	ldr	r2, [r4, #12]
3000a3c2:	6812      	ldr	r2, [r2, #0]
3000a3c4:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000a3c6:	fab2 f282 	clz	r2, r2
3000a3ca:	0952      	lsrs	r2, r2, #5
3000a3cc:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000a3d0:	4301      	orrs	r1, r0
3000a3d2:	e519      	b.n	30009e08 <rxi316_dram_init+0x2a4>
3000a3d4:	f1a1 0502 	sub.w	r5, r1, #2
3000a3d8:	2909      	cmp	r1, #9
3000a3da:	fab5 f585 	clz	r5, r5
3000a3de:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000a3e2:	f000 80a3 	beq.w	3000a52c <rxi316_dram_init+0x9c8>
3000a3e6:	462a      	mov	r2, r5
3000a3e8:	e4ed      	b.n	30009dc6 <rxi316_dram_init+0x262>
3000a3ea:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000a3ec:	46a8      	mov	r8, r5
3000a3ee:	9d08      	ldr	r5, [sp, #32]
3000a3f0:	44a8      	add	r8, r5
3000a3f2:	e619      	b.n	3000a028 <rxi316_dram_init+0x4c4>
3000a3f4:	9a08      	ldr	r2, [sp, #32]
3000a3f6:	fa22 f10b 	lsr.w	r1, r2, fp
3000a3fa:	e5a9      	b.n	30009f50 <rxi316_dram_init+0x3ec>
3000a3fc:	2000      	movs	r0, #0
3000a3fe:	e7df      	b.n	3000a3c0 <rxi316_dram_init+0x85c>
3000a400:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000a404:	e610      	b.n	3000a028 <rxi316_dram_init+0x4c4>
3000a406:	f242 7710 	movw	r7, #10000	; 0x2710
3000a40a:	f8df 90ec 	ldr.w	r9, [pc, #236]	; 3000a4f8 <rxi316_dram_init+0x994>
3000a40e:	f8df e0e0 	ldr.w	lr, [pc, #224]	; 3000a4f0 <rxi316_dram_init+0x98c>
3000a412:	fbb7 fcf3 	udiv	ip, r7, r3
3000a416:	9f05      	ldr	r7, [sp, #20]
3000a418:	4467      	add	r7, ip
3000a41a:	fbb9 f9f3 	udiv	r9, r9, r3
3000a41e:	fbbe fef3 	udiv	lr, lr, r3
3000a422:	09ff      	lsrs	r7, r7, #7
3000a424:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a428:	3701      	adds	r7, #1
3000a42a:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000a42e:	f109 0c01 	add.w	ip, r9, #1
3000a432:	f8df 90c0 	ldr.w	r9, [pc, #192]	; 3000a4f4 <rxi316_dram_init+0x990>
3000a436:	023f      	lsls	r7, r7, #8
3000a438:	f10e 0e01 	add.w	lr, lr, #1
3000a43c:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000a440:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000a444:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000a448:	9709      	str	r7, [sp, #36]	; 0x24
3000a44a:	e648      	b.n	3000a0de <rxi316_dram_init+0x57a>
3000a44c:	2100      	movs	r1, #0
3000a44e:	460d      	mov	r5, r1
3000a450:	e4c3      	b.n	30009dda <rxi316_dram_init+0x276>
3000a452:	3a07      	subs	r2, #7
3000a454:	f7ff bbcb 	b.w	30009bee <rxi316_dram_init+0x8a>
3000a458:	686a      	ldr	r2, [r5, #4]
3000a45a:	f042 0220 	orr.w	r2, r2, #32
3000a45e:	606a      	str	r2, [r5, #4]
3000a460:	e426      	b.n	30009cb0 <rxi316_dram_init+0x14c>
3000a462:	2204      	movs	r2, #4
3000a464:	2102      	movs	r1, #2
3000a466:	2500      	movs	r5, #0
3000a468:	9201      	str	r2, [sp, #4]
3000a46a:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000a46e:	f7ff bba7 	b.w	30009bc0 <rxi316_dram_init+0x5c>
3000a472:	3a08      	subs	r2, #8
3000a474:	f7ff bbbb 	b.w	30009bee <rxi316_dram_init+0x8a>
3000a478:	46b8      	mov	r8, r7
3000a47a:	e61f      	b.n	3000a0bc <rxi316_dram_init+0x558>
3000a47c:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000a47e:	2a04      	cmp	r2, #4
3000a480:	d90c      	bls.n	3000a49c <rxi316_dram_init+0x938>
3000a482:	2a0b      	cmp	r2, #11
3000a484:	d86b      	bhi.n	3000a55e <rxi316_dram_init+0x9fa>
3000a486:	1f17      	subs	r7, r2, #4
3000a488:	013f      	lsls	r7, r7, #4
3000a48a:	e008      	b.n	3000a49e <rxi316_dram_init+0x93a>
3000a48c:	f04f 0802 	mov.w	r8, #2
3000a490:	e614      	b.n	3000a0bc <rxi316_dram_init+0x558>
3000a492:	2903      	cmp	r1, #3
3000a494:	d005      	beq.n	3000a4a2 <rxi316_dram_init+0x93e>
3000a496:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a49a:	e49e      	b.n	30009dda <rxi316_dram_init+0x276>
3000a49c:	2710      	movs	r7, #16
3000a49e:	68a2      	ldr	r2, [r4, #8]
3000a4a0:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000a4a2:	2a04      	cmp	r2, #4
3000a4a4:	d94a      	bls.n	3000a53c <rxi316_dram_init+0x9d8>
3000a4a6:	1f51      	subs	r1, r2, #5
3000a4a8:	2903      	cmp	r1, #3
3000a4aa:	d84f      	bhi.n	3000a54c <rxi316_dram_init+0x9e8>
3000a4ac:	1f15      	subs	r5, r2, #4
3000a4ae:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a4b2:	026d      	lsls	r5, r5, #9
3000a4b4:	e491      	b.n	30009dda <rxi316_dram_init+0x276>
3000a4b6:	f1a1 0c02 	sub.w	ip, r1, #2
3000a4ba:	f1bc 0f01 	cmp.w	ip, #1
3000a4be:	d903      	bls.n	3000a4c8 <rxi316_dram_init+0x964>
3000a4c0:	f1be 0f00 	cmp.w	lr, #0
3000a4c4:	f43f ac7f 	beq.w	30009dc6 <rxi316_dram_init+0x262>
3000a4c8:	f040 0002 	orr.w	r0, r0, #2
3000a4cc:	e47b      	b.n	30009dc6 <rxi316_dram_init+0x262>
3000a4ce:	bf00      	nop
3000a4d0:	41100000 	.word	0x41100000
3000a4d4:	42008000 	.word	0x42008000
3000a4d8:	41011000 	.word	0x41011000
3000a4dc:	44000103 	.word	0x44000103
3000a4e0:	44000021 	.word	0x44000021
3000a4e4:	fff8fffc 	.word	0xfff8fffc
3000a4e8:	00009b2d 	.word	0x00009b2d
3000a4ec:	00061a80 	.word	0x00061a80
3000a4f0:	0bebc200 	.word	0x0bebc200
3000a4f4:	003ff000 	.word	0x003ff000
3000a4f8:	1dcd6500 	.word	0x1dcd6500
3000a4fc:	4f1d      	ldr	r7, [pc, #116]	; (3000a574 <rxi316_dram_init+0xa10>)
3000a4fe:	f04f 0e00 	mov.w	lr, #0
3000a502:	fbb7 f9f3 	udiv	r9, r7, r3
3000a506:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000a50a:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000a50e:	f109 0701 	add.w	r7, r9, #1
3000a512:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000a578 <rxi316_dram_init+0xa14>
3000a516:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000a51a:	e5e0      	b.n	3000a0de <rxi316_dram_init+0x57a>
3000a51c:	f105 0802 	add.w	r8, r5, #2
3000a520:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000a522:	eba8 0805 	sub.w	r8, r8, r5
3000a526:	9d08      	ldr	r5, [sp, #32]
3000a528:	44a8      	add	r8, r5
3000a52a:	e57d      	b.n	3000a028 <rxi316_dram_init+0x4c4>
3000a52c:	2204      	movs	r2, #4
3000a52e:	e6ee      	b.n	3000a30e <rxi316_dram_init+0x7aa>
3000a530:	686a      	ldr	r2, [r5, #4]
3000a532:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000a536:	606a      	str	r2, [r5, #4]
3000a538:	f7ff bbba 	b.w	30009cb0 <rxi316_dram_init+0x14c>
3000a53c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a540:	f44f 7500 	mov.w	r5, #512	; 0x200
3000a544:	e449      	b.n	30009dda <rxi316_dram_init+0x276>
3000a546:	f04f 0802 	mov.w	r8, #2
3000a54a:	e5b1      	b.n	3000a0b0 <rxi316_dram_init+0x54c>
3000a54c:	f1a2 0109 	sub.w	r1, r2, #9
3000a550:	2905      	cmp	r1, #5
3000a552:	d80b      	bhi.n	3000a56c <rxi316_dram_init+0xa08>
3000a554:	0855      	lsrs	r5, r2, #1
3000a556:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a55a:	026d      	lsls	r5, r5, #9
3000a55c:	e43d      	b.n	30009dda <rxi316_dram_init+0x276>
3000a55e:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000a560:	f040 0004 	orr.w	r0, r0, #4
3000a564:	f1a2 070c 	sub.w	r7, r2, #12
3000a568:	013f      	lsls	r7, r7, #4
3000a56a:	e798      	b.n	3000a49e <rxi316_dram_init+0x93a>
3000a56c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000a570:	2500      	movs	r5, #0
3000a572:	e432      	b.n	30009dda <rxi316_dram_init+0x276>
3000a574:	0bebc200 	.word	0x0bebc200
3000a578:	003ff000 	.word	0x003ff000

3000a57c <rxi316_ftend_init>:
3000a57c:	2300      	movs	r3, #0
3000a57e:	b510      	push	{r4, lr}
3000a580:	4c0c      	ldr	r4, [pc, #48]	; (3000a5b4 <rxi316_ftend_init+0x38>)
3000a582:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000a586:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000a58a:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000a58e:	f7fe fe5d 	bl	3000924c <ddr_init_index>
3000a592:	4a09      	ldr	r2, [pc, #36]	; (3000a5b8 <rxi316_ftend_init+0x3c>)
3000a594:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000a598:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000a59c:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000a5a0:	68cb      	ldr	r3, [r1, #12]
3000a5a2:	689b      	ldr	r3, [r3, #8]
3000a5a4:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000a5a6:	3301      	adds	r3, #1
3000a5a8:	b2db      	uxtb	r3, r3
3000a5aa:	4313      	orrs	r3, r2
3000a5ac:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000a5b0:	bd10      	pop	{r4, pc}
3000a5b2:	bf00      	nop
3000a5b4:	41101000 	.word	0x41101000
3000a5b8:	3000e5f0 	.word	0x3000e5f0

3000a5bc <rxi316_DynSre_init>:
3000a5bc:	b538      	push	{r3, r4, r5, lr}
3000a5be:	4b22      	ldr	r3, [pc, #136]	; (3000a648 <rxi316_DynSre_init+0x8c>)
3000a5c0:	4604      	mov	r4, r0
3000a5c2:	460d      	mov	r5, r1
3000a5c4:	4798      	blx	r3
3000a5c6:	2802      	cmp	r0, #2
3000a5c8:	d937      	bls.n	3000a63a <rxi316_DynSre_init+0x7e>
3000a5ca:	4b20      	ldr	r3, [pc, #128]	; (3000a64c <rxi316_DynSre_init+0x90>)
3000a5cc:	691b      	ldr	r3, [r3, #16]
3000a5ce:	f3c3 230f 	ubfx	r3, r3, #8, #16
3000a5d2:	3b46      	subs	r3, #70	; 0x46
3000a5d4:	42a3      	cmp	r3, r4
3000a5d6:	d915      	bls.n	3000a604 <rxi316_DynSre_init+0x48>
3000a5d8:	2d01      	cmp	r5, #1
3000a5da:	d01a      	beq.n	3000a612 <rxi316_DynSre_init+0x56>
3000a5dc:	4a1b      	ldr	r2, [pc, #108]	; (3000a64c <rxi316_DynSre_init+0x90>)
3000a5de:	6893      	ldr	r3, [r2, #8]
3000a5e0:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000a5e4:	6093      	str	r3, [r2, #8]
3000a5e6:	4b19      	ldr	r3, [pc, #100]	; (3000a64c <rxi316_DynSre_init+0x90>)
3000a5e8:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000a5ec:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a5f0:	625a      	str	r2, [r3, #36]	; 0x24
3000a5f2:	629a      	str	r2, [r3, #40]	; 0x28
3000a5f4:	69da      	ldr	r2, [r3, #28]
3000a5f6:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000a5fa:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000a5fe:	61da      	str	r2, [r3, #28]
3000a600:	6019      	str	r1, [r3, #0]
3000a602:	bd38      	pop	{r3, r4, r5, pc}
3000a604:	f240 411e 	movw	r1, #1054	; 0x41e
3000a608:	4811      	ldr	r0, [pc, #68]	; (3000a650 <rxi316_DynSre_init+0x94>)
3000a60a:	f001 f8e5 	bl	3000b7d8 <__io_assert_failed_veneer>
3000a60e:	2d01      	cmp	r5, #1
3000a610:	d1e4      	bne.n	3000a5dc <rxi316_DynSre_init+0x20>
3000a612:	4b0e      	ldr	r3, [pc, #56]	; (3000a64c <rxi316_DynSre_init+0x90>)
3000a614:	f64f 70fe 	movw	r0, #65534	; 0xfffe
3000a618:	4a0e      	ldr	r2, [pc, #56]	; (3000a654 <rxi316_DynSre_init+0x98>)
3000a61a:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000a61e:	4004      	ands	r4, r0
3000a620:	400a      	ands	r2, r1
3000a622:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a626:	f8d3 0100 	ldr.w	r0, [r3, #256]	; 0x100
3000a62a:	4304      	orrs	r4, r0
3000a62c:	f8c3 4100 	str.w	r4, [r3, #256]	; 0x100
3000a630:	689a      	ldr	r2, [r3, #8]
3000a632:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a636:	609a      	str	r2, [r3, #8]
3000a638:	e7d5      	b.n	3000a5e6 <rxi316_DynSre_init+0x2a>
3000a63a:	4b07      	ldr	r3, [pc, #28]	; (3000a658 <rxi316_DynSre_init+0x9c>)
3000a63c:	4798      	blx	r3
3000a63e:	2801      	cmp	r0, #1
3000a640:	bf08      	it	eq
3000a642:	2400      	moveq	r4, #0
3000a644:	e7c8      	b.n	3000a5d8 <rxi316_DynSre_init+0x1c>
3000a646:	bf00      	nop
3000a648:	30008f3d 	.word	0x30008f3d
3000a64c:	41100000 	.word	0x41100000
3000a650:	3000cb64 	.word	0x3000cb64
3000a654:	ffff0000 	.word	0xffff0000
3000a658:	30007309 	.word	0x30007309

3000a65c <ddr_init>:
3000a65c:	b500      	push	{lr}
3000a65e:	b083      	sub	sp, #12
3000a660:	f7fe fdf4 	bl	3000924c <ddr_init_index>
3000a664:	4b06      	ldr	r3, [pc, #24]	; (3000a680 <ddr_init+0x24>)
3000a666:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000a66a:	9001      	str	r0, [sp, #4]
3000a66c:	f7fe fe18 	bl	300092a0 <rxi316_perf_tune>
3000a670:	9801      	ldr	r0, [sp, #4]
3000a672:	f7ff fa77 	bl	30009b64 <rxi316_dram_init>
3000a676:	b003      	add	sp, #12
3000a678:	f85d eb04 	ldr.w	lr, [sp], #4
3000a67c:	f7ff bf7e 	b.w	3000a57c <rxi316_ftend_init>
3000a680:	3000e5f0 	.word	0x3000e5f0

3000a684 <SWR_Calib_DCore>:
3000a684:	4b0f      	ldr	r3, [pc, #60]	; (3000a6c4 <SWR_Calib_DCore+0x40>)
3000a686:	f240 3231 	movw	r2, #817	; 0x331
3000a68a:	490f      	ldr	r1, [pc, #60]	; (3000a6c8 <SWR_Calib_DCore+0x44>)
3000a68c:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000a68e:	4001      	ands	r1, r0
3000a690:	430a      	orrs	r2, r1
3000a692:	651a      	str	r2, [r3, #80]	; 0x50
3000a694:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000a696:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000a69a:	641a      	str	r2, [r3, #64]	; 0x40
3000a69c:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000a6a0:	f042 0201 	orr.w	r2, r2, #1
3000a6a4:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000a6a8:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000a6ac:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000a6b0:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000a6b4:	685a      	ldr	r2, [r3, #4]
3000a6b6:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000a6ba:	f042 0204 	orr.w	r2, r2, #4
3000a6be:	605a      	str	r2, [r3, #4]
3000a6c0:	4770      	bx	lr
3000a6c2:	bf00      	nop
3000a6c4:	42008100 	.word	0x42008100
3000a6c8:	fffffccc 	.word	0xfffffccc

3000a6cc <SWR_Calib_MEM>:
3000a6cc:	4b51      	ldr	r3, [pc, #324]	; (3000a814 <SWR_Calib_MEM+0x148>)
3000a6ce:	f240 3231 	movw	r2, #817	; 0x331
3000a6d2:	4951      	ldr	r1, [pc, #324]	; (3000a818 <SWR_Calib_MEM+0x14c>)
3000a6d4:	f04f 0c00 	mov.w	ip, #0
3000a6d8:	b510      	push	{r4, lr}
3000a6da:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000a6de:	b082      	sub	sp, #8
3000a6e0:	4001      	ands	r1, r0
3000a6e2:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000a6e6:	430a      	orrs	r2, r1
3000a6e8:	f10d 0107 	add.w	r1, sp, #7
3000a6ec:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000a6f0:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000a6f4:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000a6f8:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000a6fc:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000a700:	f042 0201 	orr.w	r2, r2, #1
3000a704:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000a708:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000a70c:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000a710:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000a714:	f88d c007 	strb.w	ip, [sp, #7]
3000a718:	f7fd fb8a 	bl	30007e30 <OTP_Read8>
3000a71c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a720:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000a724:	2af0      	cmp	r2, #240	; 0xf0
3000a726:	d156      	bne.n	3000a7d6 <SWR_Calib_MEM+0x10a>
3000a728:	f003 030f 	and.w	r3, r3, #15
3000a72c:	2b0f      	cmp	r3, #15
3000a72e:	d04e      	beq.n	3000a7ce <SWR_Calib_MEM+0x102>
3000a730:	2302      	movs	r3, #2
3000a732:	4c3a      	ldr	r4, [pc, #232]	; (3000a81c <SWR_Calib_MEM+0x150>)
3000a734:	f10d 0106 	add.w	r1, sp, #6
3000a738:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000a73c:	7023      	strb	r3, [r4, #0]
3000a73e:	f7fd fb77 	bl	30007e30 <OTP_Read8>
3000a742:	7822      	ldrb	r2, [r4, #0]
3000a744:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000a748:	2a03      	cmp	r2, #3
3000a74a:	d05a      	beq.n	3000a802 <SWR_Calib_MEM+0x136>
3000a74c:	2a02      	cmp	r2, #2
3000a74e:	bf0c      	ite	eq
3000a750:	f003 030f 	andeq.w	r3, r3, #15
3000a754:	2300      	movne	r3, #0
3000a756:	492f      	ldr	r1, [pc, #188]	; (3000a814 <SWR_Calib_MEM+0x148>)
3000a758:	4831      	ldr	r0, [pc, #196]	; (3000a820 <SWR_Calib_MEM+0x154>)
3000a75a:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000a75e:	f022 020f 	bic.w	r2, r2, #15
3000a762:	4313      	orrs	r3, r2
3000a764:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000a768:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000a76a:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000a76e:	2b07      	cmp	r3, #7
3000a770:	d03a      	beq.n	3000a7e8 <SWR_Calib_MEM+0x11c>
3000a772:	2300      	movs	r3, #0
3000a774:	f10d 0107 	add.w	r1, sp, #7
3000a778:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000a77c:	f88d 3007 	strb.w	r3, [sp, #7]
3000a780:	f7fd fb56 	bl	30007e30 <OTP_Read8>
3000a784:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a788:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000a78c:	2af0      	cmp	r2, #240	; 0xf0
3000a78e:	d124      	bne.n	3000a7da <SWR_Calib_MEM+0x10e>
3000a790:	f003 030f 	and.w	r3, r3, #15
3000a794:	2b0f      	cmp	r3, #15
3000a796:	d022      	beq.n	3000a7de <SWR_Calib_MEM+0x112>
3000a798:	2302      	movs	r3, #2
3000a79a:	4c22      	ldr	r4, [pc, #136]	; (3000a824 <SWR_Calib_MEM+0x158>)
3000a79c:	f10d 0107 	add.w	r1, sp, #7
3000a7a0:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000a7a4:	7023      	strb	r3, [r4, #0]
3000a7a6:	f7fd fb43 	bl	30007e30 <OTP_Read8>
3000a7aa:	7822      	ldrb	r2, [r4, #0]
3000a7ac:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a7b0:	2a03      	cmp	r2, #3
3000a7b2:	d028      	beq.n	3000a806 <SWR_Calib_MEM+0x13a>
3000a7b4:	2a02      	cmp	r2, #2
3000a7b6:	d029      	beq.n	3000a80c <SWR_Calib_MEM+0x140>
3000a7b8:	2200      	movs	r2, #0
3000a7ba:	4916      	ldr	r1, [pc, #88]	; (3000a814 <SWR_Calib_MEM+0x148>)
3000a7bc:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000a7c0:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000a7c4:	4313      	orrs	r3, r2
3000a7c6:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000a7ca:	b002      	add	sp, #8
3000a7cc:	bd10      	pop	{r4, pc}
3000a7ce:	4b13      	ldr	r3, [pc, #76]	; (3000a81c <SWR_Calib_MEM+0x150>)
3000a7d0:	2201      	movs	r2, #1
3000a7d2:	701a      	strb	r2, [r3, #0]
3000a7d4:	e7cd      	b.n	3000a772 <SWR_Calib_MEM+0xa6>
3000a7d6:	2303      	movs	r3, #3
3000a7d8:	e7ab      	b.n	3000a732 <SWR_Calib_MEM+0x66>
3000a7da:	2303      	movs	r3, #3
3000a7dc:	e7dd      	b.n	3000a79a <SWR_Calib_MEM+0xce>
3000a7de:	4b11      	ldr	r3, [pc, #68]	; (3000a824 <SWR_Calib_MEM+0x158>)
3000a7e0:	2201      	movs	r2, #1
3000a7e2:	701a      	strb	r2, [r3, #0]
3000a7e4:	b002      	add	sp, #8
3000a7e6:	bd10      	pop	{r4, pc}
3000a7e8:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000a7ea:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000a7ee:	d1c0      	bne.n	3000a772 <SWR_Calib_MEM+0xa6>
3000a7f0:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000a7f4:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000a7f8:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000a7fc:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000a800:	e7b7      	b.n	3000a772 <SWR_Calib_MEM+0xa6>
3000a802:	091b      	lsrs	r3, r3, #4
3000a804:	e7a7      	b.n	3000a756 <SWR_Calib_MEM+0x8a>
3000a806:	091b      	lsrs	r3, r3, #4
3000a808:	059a      	lsls	r2, r3, #22
3000a80a:	e7d6      	b.n	3000a7ba <SWR_Calib_MEM+0xee>
3000a80c:	f003 030f 	and.w	r3, r3, #15
3000a810:	059a      	lsls	r2, r3, #22
3000a812:	e7d2      	b.n	3000a7ba <SWR_Calib_MEM+0xee>
3000a814:	42008800 	.word	0x42008800
3000a818:	fffffccc 	.word	0xfffffccc
3000a81c:	3000e78b 	.word	0x3000e78b
3000a820:	42008100 	.word	0x42008100
3000a824:	3000e78a 	.word	0x3000e78a

3000a828 <SWR_Calib_AUD>:
3000a828:	4b48      	ldr	r3, [pc, #288]	; (3000a94c <SWR_Calib_AUD+0x124>)
3000a82a:	f240 3231 	movw	r2, #817	; 0x331
3000a82e:	4948      	ldr	r1, [pc, #288]	; (3000a950 <SWR_Calib_AUD+0x128>)
3000a830:	f04f 0c00 	mov.w	ip, #0
3000a834:	b510      	push	{r4, lr}
3000a836:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000a83a:	b082      	sub	sp, #8
3000a83c:	4001      	ands	r1, r0
3000a83e:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000a842:	430a      	orrs	r2, r1
3000a844:	f10d 0107 	add.w	r1, sp, #7
3000a848:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000a84c:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000a850:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000a854:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000a858:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000a85c:	f042 0201 	orr.w	r2, r2, #1
3000a860:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000a864:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000a868:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000a86c:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000a870:	f88d c007 	strb.w	ip, [sp, #7]
3000a874:	f7fd fadc 	bl	30007e30 <OTP_Read8>
3000a878:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a87c:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000a880:	2af0      	cmp	r2, #240	; 0xf0
3000a882:	d150      	bne.n	3000a926 <SWR_Calib_AUD+0xfe>
3000a884:	f003 030f 	and.w	r3, r3, #15
3000a888:	2b0f      	cmp	r3, #15
3000a88a:	d048      	beq.n	3000a91e <SWR_Calib_AUD+0xf6>
3000a88c:	2302      	movs	r3, #2
3000a88e:	4c31      	ldr	r4, [pc, #196]	; (3000a954 <SWR_Calib_AUD+0x12c>)
3000a890:	f10d 0106 	add.w	r1, sp, #6
3000a894:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000a898:	7023      	strb	r3, [r4, #0]
3000a89a:	f7fd fac9 	bl	30007e30 <OTP_Read8>
3000a89e:	7822      	ldrb	r2, [r4, #0]
3000a8a0:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000a8a4:	2a03      	cmp	r2, #3
3000a8a6:	d047      	beq.n	3000a938 <SWR_Calib_AUD+0x110>
3000a8a8:	2a02      	cmp	r2, #2
3000a8aa:	bf0c      	ite	eq
3000a8ac:	f003 030f 	andeq.w	r3, r3, #15
3000a8b0:	2300      	movne	r3, #0
3000a8b2:	4926      	ldr	r1, [pc, #152]	; (3000a94c <SWR_Calib_AUD+0x124>)
3000a8b4:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000a8b8:	f022 020f 	bic.w	r2, r2, #15
3000a8bc:	4313      	orrs	r3, r2
3000a8be:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000a8c2:	2300      	movs	r3, #0
3000a8c4:	f10d 0107 	add.w	r1, sp, #7
3000a8c8:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000a8cc:	f88d 3007 	strb.w	r3, [sp, #7]
3000a8d0:	f7fd faae 	bl	30007e30 <OTP_Read8>
3000a8d4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a8d8:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000a8dc:	2af0      	cmp	r2, #240	; 0xf0
3000a8de:	d124      	bne.n	3000a92a <SWR_Calib_AUD+0x102>
3000a8e0:	f003 030f 	and.w	r3, r3, #15
3000a8e4:	2b0f      	cmp	r3, #15
3000a8e6:	d022      	beq.n	3000a92e <SWR_Calib_AUD+0x106>
3000a8e8:	2302      	movs	r3, #2
3000a8ea:	4c1b      	ldr	r4, [pc, #108]	; (3000a958 <SWR_Calib_AUD+0x130>)
3000a8ec:	f10d 0107 	add.w	r1, sp, #7
3000a8f0:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000a8f4:	7023      	strb	r3, [r4, #0]
3000a8f6:	f7fd fa9b 	bl	30007e30 <OTP_Read8>
3000a8fa:	7822      	ldrb	r2, [r4, #0]
3000a8fc:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a900:	2a03      	cmp	r2, #3
3000a902:	d01b      	beq.n	3000a93c <SWR_Calib_AUD+0x114>
3000a904:	2a02      	cmp	r2, #2
3000a906:	d01c      	beq.n	3000a942 <SWR_Calib_AUD+0x11a>
3000a908:	2200      	movs	r2, #0
3000a90a:	4910      	ldr	r1, [pc, #64]	; (3000a94c <SWR_Calib_AUD+0x124>)
3000a90c:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000a910:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000a914:	4313      	orrs	r3, r2
3000a916:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000a91a:	b002      	add	sp, #8
3000a91c:	bd10      	pop	{r4, pc}
3000a91e:	4b0d      	ldr	r3, [pc, #52]	; (3000a954 <SWR_Calib_AUD+0x12c>)
3000a920:	2201      	movs	r2, #1
3000a922:	701a      	strb	r2, [r3, #0]
3000a924:	e7cd      	b.n	3000a8c2 <SWR_Calib_AUD+0x9a>
3000a926:	2303      	movs	r3, #3
3000a928:	e7b1      	b.n	3000a88e <SWR_Calib_AUD+0x66>
3000a92a:	2303      	movs	r3, #3
3000a92c:	e7dd      	b.n	3000a8ea <SWR_Calib_AUD+0xc2>
3000a92e:	4b0a      	ldr	r3, [pc, #40]	; (3000a958 <SWR_Calib_AUD+0x130>)
3000a930:	2201      	movs	r2, #1
3000a932:	701a      	strb	r2, [r3, #0]
3000a934:	b002      	add	sp, #8
3000a936:	bd10      	pop	{r4, pc}
3000a938:	091b      	lsrs	r3, r3, #4
3000a93a:	e7ba      	b.n	3000a8b2 <SWR_Calib_AUD+0x8a>
3000a93c:	091b      	lsrs	r3, r3, #4
3000a93e:	059a      	lsls	r2, r3, #22
3000a940:	e7e3      	b.n	3000a90a <SWR_Calib_AUD+0xe2>
3000a942:	f003 030f 	and.w	r3, r3, #15
3000a946:	059a      	lsls	r2, r3, #22
3000a948:	e7df      	b.n	3000a90a <SWR_Calib_AUD+0xe2>
3000a94a:	bf00      	nop
3000a94c:	42008800 	.word	0x42008800
3000a950:	fffffccc 	.word	0xfffffccc
3000a954:	3000e789 	.word	0x3000e789
3000a958:	3000e788 	.word	0x3000e788

3000a95c <SWR_MEM>:
3000a95c:	2801      	cmp	r0, #1
3000a95e:	d00e      	beq.n	3000a97e <SWR_MEM+0x22>
3000a960:	4b31      	ldr	r3, [pc, #196]	; (3000aa28 <SWR_MEM+0xcc>)
3000a962:	2000      	movs	r0, #0
3000a964:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000a968:	f022 0201 	bic.w	r2, r2, #1
3000a96c:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000a970:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000a974:	f022 0202 	bic.w	r2, r2, #2
3000a978:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000a97c:	4770      	bx	lr
3000a97e:	b510      	push	{r4, lr}
3000a980:	4c29      	ldr	r4, [pc, #164]	; (3000aa28 <SWR_MEM+0xcc>)
3000a982:	4a2a      	ldr	r2, [pc, #168]	; (3000aa2c <SWR_MEM+0xd0>)
3000a984:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000a988:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000a98c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a990:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000a994:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a998:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000a99c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a9a0:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000a9a4:	f023 0303 	bic.w	r3, r3, #3
3000a9a8:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000a9ac:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a9b0:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000a9b4:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a9b8:	4790      	blx	r2
3000a9ba:	2803      	cmp	r0, #3
3000a9bc:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000a9c0:	d026      	beq.n	3000aa10 <SWR_MEM+0xb4>
3000a9c2:	f043 0303 	orr.w	r3, r3, #3
3000a9c6:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000a9ca:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a9ce:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000a9d2:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a9d6:	4c14      	ldr	r4, [pc, #80]	; (3000aa28 <SWR_MEM+0xcc>)
3000a9d8:	4b15      	ldr	r3, [pc, #84]	; (3000aa30 <SWR_MEM+0xd4>)
3000a9da:	4798      	blx	r3
3000a9dc:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a9e0:	4a14      	ldr	r2, [pc, #80]	; (3000aa34 <SWR_MEM+0xd8>)
3000a9e2:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000a9e6:	f043 0302 	orr.w	r3, r3, #2
3000a9ea:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a9ee:	4790      	blx	r2
3000a9f0:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000a9f4:	4b10      	ldr	r3, [pc, #64]	; (3000aa38 <SWR_MEM+0xdc>)
3000a9f6:	f042 0201 	orr.w	r2, r2, #1
3000a9fa:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000a9fe:	e001      	b.n	3000aa04 <SWR_MEM+0xa8>
3000aa00:	3b01      	subs	r3, #1
3000aa02:	d00e      	beq.n	3000aa22 <SWR_MEM+0xc6>
3000aa04:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000aa08:	03d2      	lsls	r2, r2, #15
3000aa0a:	d5f9      	bpl.n	3000aa00 <SWR_MEM+0xa4>
3000aa0c:	2000      	movs	r0, #0
3000aa0e:	bd10      	pop	{r4, pc}
3000aa10:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000aa14:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000aa18:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000aa1c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000aa20:	e7d9      	b.n	3000a9d6 <SWR_MEM+0x7a>
3000aa22:	2001      	movs	r0, #1
3000aa24:	bd10      	pop	{r4, pc}
3000aa26:	bf00      	nop
3000aa28:	42008800 	.word	0x42008800
3000aa2c:	30007139 	.word	0x30007139
3000aa30:	3000a6cd 	.word	0x3000a6cd
3000aa34:	00009b2d 	.word	0x00009b2d
3000aa38:	000186a0 	.word	0x000186a0

3000aa3c <SWR_MEM_Manual>:
3000aa3c:	4a06      	ldr	r2, [pc, #24]	; (3000aa58 <SWR_MEM_Manual+0x1c>)
3000aa3e:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000aa42:	b120      	cbz	r0, 3000aa4e <SWR_MEM_Manual+0x12>
3000aa44:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000aa48:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000aa4c:	4770      	bx	lr
3000aa4e:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000aa52:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000aa56:	4770      	bx	lr
3000aa58:	42008800 	.word	0x42008800

3000aa5c <SWR_MEM_Mode_Set>:
3000aa5c:	2801      	cmp	r0, #1
3000aa5e:	b538      	push	{r3, r4, r5, lr}
3000aa60:	d807      	bhi.n	3000aa72 <SWR_MEM_Mode_Set+0x16>
3000aa62:	d10a      	bne.n	3000aa7a <SWR_MEM_Mode_Set+0x1e>
3000aa64:	4c22      	ldr	r4, [pc, #136]	; (3000aaf0 <SWR_MEM_Mode_Set+0x94>)
3000aa66:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000aa6a:	035b      	lsls	r3, r3, #13
3000aa6c:	d51b      	bpl.n	3000aaa6 <SWR_MEM_Mode_Set+0x4a>
3000aa6e:	2000      	movs	r0, #0
3000aa70:	bd38      	pop	{r3, r4, r5, pc}
3000aa72:	217e      	movs	r1, #126	; 0x7e
3000aa74:	481f      	ldr	r0, [pc, #124]	; (3000aaf4 <SWR_MEM_Mode_Set+0x98>)
3000aa76:	f000 feaf 	bl	3000b7d8 <__io_assert_failed_veneer>
3000aa7a:	491d      	ldr	r1, [pc, #116]	; (3000aaf0 <SWR_MEM_Mode_Set+0x94>)
3000aa7c:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000aa80:	0358      	lsls	r0, r3, #13
3000aa82:	d5f4      	bpl.n	3000aa6e <SWR_MEM_Mode_Set+0x12>
3000aa84:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000aa88:	f242 7310 	movw	r3, #10000	; 0x2710
3000aa8c:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000aa90:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000aa94:	e001      	b.n	3000aa9a <SWR_MEM_Mode_Set+0x3e>
3000aa96:	3b01      	subs	r3, #1
3000aa98:	d027      	beq.n	3000aaea <SWR_MEM_Mode_Set+0x8e>
3000aa9a:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000aa9e:	0352      	lsls	r2, r2, #13
3000aaa0:	d4f9      	bmi.n	3000aa96 <SWR_MEM_Mode_Set+0x3a>
3000aaa2:	2000      	movs	r0, #0
3000aaa4:	e7e4      	b.n	3000aa70 <SWR_MEM_Mode_Set+0x14>
3000aaa6:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000aaaa:	2064      	movs	r0, #100	; 0x64
3000aaac:	4d12      	ldr	r5, [pc, #72]	; (3000aaf8 <SWR_MEM_Mode_Set+0x9c>)
3000aaae:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000aab2:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000aab6:	47a8      	blx	r5
3000aab8:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000aabc:	2064      	movs	r0, #100	; 0x64
3000aabe:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000aac2:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000aac6:	47a8      	blx	r5
3000aac8:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000aacc:	f242 7310 	movw	r3, #10000	; 0x2710
3000aad0:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000aad4:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000aad8:	e001      	b.n	3000aade <SWR_MEM_Mode_Set+0x82>
3000aada:	3b01      	subs	r3, #1
3000aadc:	d005      	beq.n	3000aaea <SWR_MEM_Mode_Set+0x8e>
3000aade:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000aae2:	0355      	lsls	r5, r2, #13
3000aae4:	d5f9      	bpl.n	3000aada <SWR_MEM_Mode_Set+0x7e>
3000aae6:	2000      	movs	r0, #0
3000aae8:	e7c2      	b.n	3000aa70 <SWR_MEM_Mode_Set+0x14>
3000aaea:	2001      	movs	r0, #1
3000aaec:	bd38      	pop	{r3, r4, r5, pc}
3000aaee:	bf00      	nop
3000aaf0:	42008800 	.word	0x42008800
3000aaf4:	3000cb78 	.word	0x3000cb78
3000aaf8:	00009b2d 	.word	0x00009b2d

3000aafc <SWR_AUDIO>:
3000aafc:	2801      	cmp	r0, #1
3000aafe:	d00e      	beq.n	3000ab1e <SWR_AUDIO+0x22>
3000ab00:	4b1e      	ldr	r3, [pc, #120]	; (3000ab7c <SWR_AUDIO+0x80>)
3000ab02:	2000      	movs	r0, #0
3000ab04:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ab08:	f022 0201 	bic.w	r2, r2, #1
3000ab0c:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000ab10:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ab14:	f022 0202 	bic.w	r2, r2, #2
3000ab18:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000ab1c:	4770      	bx	lr
3000ab1e:	b510      	push	{r4, lr}
3000ab20:	4c16      	ldr	r4, [pc, #88]	; (3000ab7c <SWR_AUDIO+0x80>)
3000ab22:	4b17      	ldr	r3, [pc, #92]	; (3000ab80 <SWR_AUDIO+0x84>)
3000ab24:	4798      	blx	r3
3000ab26:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000ab2a:	4a16      	ldr	r2, [pc, #88]	; (3000ab84 <SWR_AUDIO+0x88>)
3000ab2c:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000ab30:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000ab34:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000ab38:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000ab3c:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000ab40:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000ab44:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000ab48:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000ab4c:	f043 0302 	orr.w	r3, r3, #2
3000ab50:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000ab54:	4790      	blx	r2
3000ab56:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000ab5a:	4b0b      	ldr	r3, [pc, #44]	; (3000ab88 <SWR_AUDIO+0x8c>)
3000ab5c:	f042 0201 	orr.w	r2, r2, #1
3000ab60:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000ab64:	e001      	b.n	3000ab6a <SWR_AUDIO+0x6e>
3000ab66:	3b01      	subs	r3, #1
3000ab68:	d005      	beq.n	3000ab76 <SWR_AUDIO+0x7a>
3000ab6a:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000ab6e:	03d2      	lsls	r2, r2, #15
3000ab70:	d5f9      	bpl.n	3000ab66 <SWR_AUDIO+0x6a>
3000ab72:	2000      	movs	r0, #0
3000ab74:	bd10      	pop	{r4, pc}
3000ab76:	2001      	movs	r0, #1
3000ab78:	bd10      	pop	{r4, pc}
3000ab7a:	bf00      	nop
3000ab7c:	42008800 	.word	0x42008800
3000ab80:	3000a829 	.word	0x3000a829
3000ab84:	00009b2d 	.word	0x00009b2d
3000ab88:	000186a0 	.word	0x000186a0

3000ab8c <SWR_AUDIO_Manual>:
3000ab8c:	4a06      	ldr	r2, [pc, #24]	; (3000aba8 <SWR_AUDIO_Manual+0x1c>)
3000ab8e:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000ab92:	b120      	cbz	r0, 3000ab9e <SWR_AUDIO_Manual+0x12>
3000ab94:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000ab98:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000ab9c:	4770      	bx	lr
3000ab9e:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000aba2:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000aba6:	4770      	bx	lr
3000aba8:	42008800 	.word	0x42008800

3000abac <SWR_BST_MODE_Set>:
3000abac:	4a06      	ldr	r2, [pc, #24]	; (3000abc8 <SWR_BST_MODE_Set+0x1c>)
3000abae:	6813      	ldr	r3, [r2, #0]
3000abb0:	b118      	cbz	r0, 3000abba <SWR_BST_MODE_Set+0xe>
3000abb2:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000abb6:	6013      	str	r3, [r2, #0]
3000abb8:	4770      	bx	lr
3000abba:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000abbe:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000abc2:	6013      	str	r3, [r2, #0]
3000abc4:	4770      	bx	lr
3000abc6:	bf00      	nop
3000abc8:	42008100 	.word	0x42008100

3000abcc <crash_dump_memory>:
3000abcc:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000abd0:	460e      	mov	r6, r1
3000abd2:	4680      	mov	r8, r0
3000abd4:	4b21      	ldr	r3, [pc, #132]	; (3000ac5c <crash_dump_memory+0x90>)
3000abd6:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000abda:	4a21      	ldr	r2, [pc, #132]	; (3000ac60 <crash_dump_memory+0x94>)
3000abdc:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000abe0:	4644      	mov	r4, r8
3000abe2:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000abe6:	4b1f      	ldr	r3, [pc, #124]	; (3000ac64 <crash_dump_memory+0x98>)
3000abe8:	481f      	ldr	r0, [pc, #124]	; (3000ac68 <crash_dump_memory+0x9c>)
3000abea:	bf88      	it	hi
3000abec:	2680      	movhi	r6, #128	; 0x80
3000abee:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000abf2:	4f1e      	ldr	r7, [pc, #120]	; (3000ac6c <crash_dump_memory+0xa0>)
3000abf4:	bf14      	ite	ne
3000abf6:	4611      	movne	r1, r2
3000abf8:	4619      	moveq	r1, r3
3000abfa:	f000 fdb5 	bl	3000b768 <__DiagPrintf_veneer>
3000abfe:	4632      	mov	r2, r6
3000ac00:	4641      	mov	r1, r8
3000ac02:	481b      	ldr	r0, [pc, #108]	; (3000ac70 <crash_dump_memory+0xa4>)
3000ac04:	f000 fdb0 	bl	3000b768 <__DiagPrintf_veneer>
3000ac08:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000ac78 <crash_dump_memory+0xac>
3000ac0c:	2300      	movs	r3, #0
3000ac0e:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000ac7c <crash_dump_memory+0xb0>
3000ac12:	e007      	b.n	3000ac24 <crash_dump_memory+0x58>
3000ac14:	6821      	ldr	r1, [r4, #0]
3000ac16:	4638      	mov	r0, r7
3000ac18:	f000 fda6 	bl	3000b768 <__DiagPrintf_veneer>
3000ac1c:	42ae      	cmp	r6, r5
3000ac1e:	d00f      	beq.n	3000ac40 <crash_dump_memory+0x74>
3000ac20:	3404      	adds	r4, #4
3000ac22:	462b      	mov	r3, r5
3000ac24:	1c5d      	adds	r5, r3, #1
3000ac26:	b183      	cbz	r3, 3000ac4a <crash_dump_memory+0x7e>
3000ac28:	079b      	lsls	r3, r3, #30
3000ac2a:	d1f3      	bne.n	3000ac14 <crash_dump_memory+0x48>
3000ac2c:	4621      	mov	r1, r4
3000ac2e:	4648      	mov	r0, r9
3000ac30:	f000 fd9a 	bl	3000b768 <__DiagPrintf_veneer>
3000ac34:	6821      	ldr	r1, [r4, #0]
3000ac36:	4638      	mov	r0, r7
3000ac38:	f000 fd96 	bl	3000b768 <__DiagPrintf_veneer>
3000ac3c:	42ae      	cmp	r6, r5
3000ac3e:	d1ef      	bne.n	3000ac20 <crash_dump_memory+0x54>
3000ac40:	480c      	ldr	r0, [pc, #48]	; (3000ac74 <crash_dump_memory+0xa8>)
3000ac42:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000ac46:	f000 bd8f 	b.w	3000b768 <__DiagPrintf_veneer>
3000ac4a:	4641      	mov	r1, r8
3000ac4c:	4650      	mov	r0, sl
3000ac4e:	f000 fd8b 	bl	3000b768 <__DiagPrintf_veneer>
3000ac52:	6821      	ldr	r1, [r4, #0]
3000ac54:	4638      	mov	r0, r7
3000ac56:	f000 fd87 	bl	3000b768 <__DiagPrintf_veneer>
3000ac5a:	e7e1      	b.n	3000ac20 <crash_dump_memory+0x54>
3000ac5c:	42008000 	.word	0x42008000
3000ac60:	3000cb8c 	.word	0x3000cb8c
3000ac64:	3000cb90 	.word	0x3000cb90
3000ac68:	3000cb94 	.word	0x3000cb94
3000ac6c:	3000cbfc 	.word	0x3000cbfc
3000ac70:	3000cbb8 	.word	0x3000cbb8
3000ac74:	3000c0fc 	.word	0x3000c0fc
3000ac78:	3000cc04 	.word	0x3000cc04
3000ac7c:	3000cbf4 	.word	0x3000cbf4

3000ac80 <crash_dump>:
3000ac80:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000ac84:	4c4f      	ldr	r4, [pc, #316]	; (3000adc4 <crash_dump+0x144>)
3000ac86:	4615      	mov	r5, r2
3000ac88:	4688      	mov	r8, r1
3000ac8a:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000ac8e:	484e      	ldr	r0, [pc, #312]	; (3000adc8 <crash_dump+0x148>)
3000ac90:	462f      	mov	r7, r5
3000ac92:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000adf4 <crash_dump+0x174>
3000ac96:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000ac9a:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000ae00 <crash_dump+0x180>
3000ac9e:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000aca2:	e9c4 2100 	strd	r2, r1, [r4]
3000aca6:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000aca8:	60a2      	str	r2, [r4, #8]
3000acaa:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000acac:	60e2      	str	r2, [r4, #12]
3000acae:	686a      	ldr	r2, [r5, #4]
3000acb0:	6122      	str	r2, [r4, #16]
3000acb2:	68aa      	ldr	r2, [r5, #8]
3000acb4:	6162      	str	r2, [r4, #20]
3000acb6:	68ea      	ldr	r2, [r5, #12]
3000acb8:	61a2      	str	r2, [r4, #24]
3000acba:	692a      	ldr	r2, [r5, #16]
3000acbc:	61e2      	str	r2, [r4, #28]
3000acbe:	696a      	ldr	r2, [r5, #20]
3000acc0:	6222      	str	r2, [r4, #32]
3000acc2:	69aa      	ldr	r2, [r5, #24]
3000acc4:	6262      	str	r2, [r4, #36]	; 0x24
3000acc6:	69ea      	ldr	r2, [r5, #28]
3000acc8:	62a2      	str	r2, [r4, #40]	; 0x28
3000acca:	6a2a      	ldr	r2, [r5, #32]
3000accc:	62e2      	str	r2, [r4, #44]	; 0x2c
3000acce:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000acd0:	6322      	str	r2, [r4, #48]	; 0x30
3000acd2:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000acd4:	6362      	str	r2, [r4, #52]	; 0x34
3000acd6:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000acd8:	63a2      	str	r2, [r4, #56]	; 0x38
3000acda:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000acdc:	63e2      	str	r2, [r4, #60]	; 0x3c
3000acde:	682a      	ldr	r2, [r5, #0]
3000ace0:	6422      	str	r2, [r4, #64]	; 0x40
3000ace2:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000ace6:	6462      	str	r2, [r4, #68]	; 0x44
3000ace8:	4a38      	ldr	r2, [pc, #224]	; (3000adcc <crash_dump+0x14c>)
3000acea:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000acee:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000acf2:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000acf6:	64e1      	str	r1, [r4, #76]	; 0x4c
3000acf8:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000acfc:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000ad00:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000ad04:	6561      	str	r1, [r4, #84]	; 0x54
3000ad06:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000ad08:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000ad0c:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000ad10:	65e1      	str	r1, [r4, #92]	; 0x5c
3000ad12:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000ad14:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000ad16:	b289      	uxth	r1, r1
3000ad18:	b292      	uxth	r2, r2
3000ad1a:	6621      	str	r1, [r4, #96]	; 0x60
3000ad1c:	6662      	str	r2, [r4, #100]	; 0x64
3000ad1e:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000ad22:	66a2      	str	r2, [r4, #104]	; 0x68
3000ad24:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000ad28:	66e3      	str	r3, [r4, #108]	; 0x6c
3000ad2a:	f000 fd1d 	bl	3000b768 <__DiagPrintf_veneer>
3000ad2e:	4620      	mov	r0, r4
3000ad30:	462c      	mov	r4, r5
3000ad32:	f000 f867 	bl	3000ae04 <fault_diagnosis>
3000ad36:	4826      	ldr	r0, [pc, #152]	; (3000add0 <crash_dump+0x150>)
3000ad38:	f000 fd16 	bl	3000b768 <__DiagPrintf_veneer>
3000ad3c:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000ad3e:	4825      	ldr	r0, [pc, #148]	; (3000add4 <crash_dump+0x154>)
3000ad40:	f000 fd12 	bl	3000b768 <__DiagPrintf_veneer>
3000ad44:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000ad46:	4824      	ldr	r0, [pc, #144]	; (3000add8 <crash_dump+0x158>)
3000ad48:	f000 fd0e 	bl	3000b768 <__DiagPrintf_veneer>
3000ad4c:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000ad4e:	4823      	ldr	r0, [pc, #140]	; (3000addc <crash_dump+0x15c>)
3000ad50:	f000 fd0a 	bl	3000b768 <__DiagPrintf_veneer>
3000ad54:	f857 1b20 	ldr.w	r1, [r7], #32
3000ad58:	4821      	ldr	r0, [pc, #132]	; (3000ade0 <crash_dump+0x160>)
3000ad5a:	463e      	mov	r6, r7
3000ad5c:	f000 fd04 	bl	3000b768 <__DiagPrintf_veneer>
3000ad60:	4920      	ldr	r1, [pc, #128]	; (3000ade4 <crash_dump+0x164>)
3000ad62:	4658      	mov	r0, fp
3000ad64:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000ad68:	f000 fcfe 	bl	3000b768 <__DiagPrintf_veneer>
3000ad6c:	454e      	cmp	r6, r9
3000ad6e:	d008      	beq.n	3000ad82 <crash_dump+0x102>
3000ad70:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000ad74:	4658      	mov	r0, fp
3000ad76:	f85a 1b04 	ldr.w	r1, [sl], #4
3000ad7a:	f000 fcf5 	bl	3000b768 <__DiagPrintf_veneer>
3000ad7e:	454e      	cmp	r6, r9
3000ad80:	d1f6      	bne.n	3000ad70 <crash_dump+0xf0>
3000ad82:	4e19      	ldr	r6, [pc, #100]	; (3000ade8 <crash_dump+0x168>)
3000ad84:	4919      	ldr	r1, [pc, #100]	; (3000adec <crash_dump+0x16c>)
3000ad86:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000adf4 <crash_dump+0x174>
3000ad8a:	e001      	b.n	3000ad90 <crash_dump+0x110>
3000ad8c:	f856 1b04 	ldr.w	r1, [r6], #4
3000ad90:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000ad94:	4648      	mov	r0, r9
3000ad96:	f000 fce7 	bl	3000b768 <__DiagPrintf_veneer>
3000ad9a:	42bc      	cmp	r4, r7
3000ad9c:	d1f6      	bne.n	3000ad8c <crash_dump+0x10c>
3000ad9e:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000ada0:	4913      	ldr	r1, [pc, #76]	; (3000adf0 <crash_dump+0x170>)
3000ada2:	4814      	ldr	r0, [pc, #80]	; (3000adf4 <crash_dump+0x174>)
3000ada4:	f000 fce0 	bl	3000b768 <__DiagPrintf_veneer>
3000ada8:	2180      	movs	r1, #128	; 0x80
3000adaa:	4640      	mov	r0, r8
3000adac:	f7ff ff0e 	bl	3000abcc <crash_dump_memory>
3000adb0:	4811      	ldr	r0, [pc, #68]	; (3000adf8 <crash_dump+0x178>)
3000adb2:	f000 fcd9 	bl	3000b768 <__DiagPrintf_veneer>
3000adb6:	4811      	ldr	r0, [pc, #68]	; (3000adfc <crash_dump+0x17c>)
3000adb8:	f000 fcd6 	bl	3000b768 <__DiagPrintf_veneer>
3000adbc:	2000      	movs	r0, #0
3000adbe:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000adc2:	bf00      	nop
3000adc4:	3000ed68 	.word	0x3000ed68
3000adc8:	3000cc18 	.word	0x3000cc18
3000adcc:	e000ed00 	.word	0xe000ed00
3000add0:	3000cc3c 	.word	0x3000cc3c
3000add4:	3000cc64 	.word	0x3000cc64
3000add8:	3000cc74 	.word	0x3000cc74
3000addc:	3000cc84 	.word	0x3000cc84
3000ade0:	3000cc94 	.word	0x3000cc94
3000ade4:	3000cc10 	.word	0x3000cc10
3000ade8:	3000ccf4 	.word	0x3000ccf4
3000adec:	3000cc14 	.word	0x3000cc14
3000adf0:	3000ccb4 	.word	0x3000ccb4
3000adf4:	3000cca4 	.word	0x3000cca4
3000adf8:	3000ccb8 	.word	0x3000ccb8
3000adfc:	3000cce4 	.word	0x3000cce4
3000ae00:	3000cd14 	.word	0x3000cd14

3000ae04 <fault_diagnosis>:
3000ae04:	b510      	push	{r4, lr}
3000ae06:	4604      	mov	r4, r0
3000ae08:	4888      	ldr	r0, [pc, #544]	; (3000b02c <fault_diagnosis+0x228>)
3000ae0a:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000ae0c:	f000 fcac 	bl	3000b768 <__DiagPrintf_veneer>
3000ae10:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000ae14:	079b      	lsls	r3, r3, #30
3000ae16:	f100 808a 	bmi.w	3000af2e <fault_diagnosis+0x12a>
3000ae1a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ae1c:	b1e1      	cbz	r1, 3000ae58 <fault_diagnosis+0x54>
3000ae1e:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ae22:	07d8      	lsls	r0, r3, #31
3000ae24:	f100 8094 	bmi.w	3000af50 <fault_diagnosis+0x14c>
3000ae28:	079a      	lsls	r2, r3, #30
3000ae2a:	f100 809a 	bmi.w	3000af62 <fault_diagnosis+0x15e>
3000ae2e:	0758      	lsls	r0, r3, #29
3000ae30:	f100 80a1 	bmi.w	3000af76 <fault_diagnosis+0x172>
3000ae34:	0719      	lsls	r1, r3, #28
3000ae36:	f100 80a8 	bmi.w	3000af8a <fault_diagnosis+0x186>
3000ae3a:	06da      	lsls	r2, r3, #27
3000ae3c:	f100 80af 	bmi.w	3000af9e <fault_diagnosis+0x19a>
3000ae40:	0698      	lsls	r0, r3, #26
3000ae42:	f100 80b6 	bmi.w	3000afb2 <fault_diagnosis+0x1ae>
3000ae46:	0619      	lsls	r1, r3, #24
3000ae48:	f100 80bd 	bmi.w	3000afc6 <fault_diagnosis+0x1c2>
3000ae4c:	065a      	lsls	r2, r3, #25
3000ae4e:	d503      	bpl.n	3000ae58 <fault_diagnosis+0x54>
3000ae50:	f013 0f28 	tst.w	r3, #40	; 0x28
3000ae54:	f040 8186 	bne.w	3000b164 <fault_diagnosis+0x360>
3000ae58:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000ae5c:	b199      	cbz	r1, 3000ae86 <fault_diagnosis+0x82>
3000ae5e:	07cb      	lsls	r3, r1, #31
3000ae60:	f100 80b9 	bmi.w	3000afd6 <fault_diagnosis+0x1d2>
3000ae64:	0788      	lsls	r0, r1, #30
3000ae66:	f100 80bf 	bmi.w	3000afe8 <fault_diagnosis+0x1e4>
3000ae6a:	070a      	lsls	r2, r1, #28
3000ae6c:	f100 80c5 	bmi.w	3000affa <fault_diagnosis+0x1f6>
3000ae70:	06cb      	lsls	r3, r1, #27
3000ae72:	f100 80cb 	bmi.w	3000b00c <fault_diagnosis+0x208>
3000ae76:	0688      	lsls	r0, r1, #26
3000ae78:	f100 80d1 	bmi.w	3000b01e <fault_diagnosis+0x21a>
3000ae7c:	060a      	lsls	r2, r1, #24
3000ae7e:	d502      	bpl.n	3000ae86 <fault_diagnosis+0x82>
3000ae80:	078b      	lsls	r3, r1, #30
3000ae82:	f040 8194 	bne.w	3000b1ae <fault_diagnosis+0x3aa>
3000ae86:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ae8a:	b1b1      	cbz	r1, 3000aeba <fault_diagnosis+0xb6>
3000ae8c:	07c8      	lsls	r0, r1, #31
3000ae8e:	f100 80f7 	bmi.w	3000b080 <fault_diagnosis+0x27c>
3000ae92:	078a      	lsls	r2, r1, #30
3000ae94:	f100 80fd 	bmi.w	3000b092 <fault_diagnosis+0x28e>
3000ae98:	074b      	lsls	r3, r1, #29
3000ae9a:	f100 8103 	bmi.w	3000b0a4 <fault_diagnosis+0x2a0>
3000ae9e:	0708      	lsls	r0, r1, #28
3000aea0:	f100 8109 	bmi.w	3000b0b6 <fault_diagnosis+0x2b2>
3000aea4:	06ca      	lsls	r2, r1, #27
3000aea6:	f100 810f 	bmi.w	3000b0c8 <fault_diagnosis+0x2c4>
3000aeaa:	068b      	lsls	r3, r1, #26
3000aeac:	f100 8115 	bmi.w	3000b0da <fault_diagnosis+0x2d6>
3000aeb0:	0608      	lsls	r0, r1, #24
3000aeb2:	d502      	bpl.n	3000aeba <fault_diagnosis+0xb6>
3000aeb4:	0789      	lsls	r1, r1, #30
3000aeb6:	f100 8180 	bmi.w	3000b1ba <fault_diagnosis+0x3b6>
3000aeba:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000aebe:	b1b9      	cbz	r1, 3000aef0 <fault_diagnosis+0xec>
3000aec0:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000aec4:	07da      	lsls	r2, r3, #31
3000aec6:	f100 810f 	bmi.w	3000b0e8 <fault_diagnosis+0x2e4>
3000aeca:	0798      	lsls	r0, r3, #30
3000aecc:	f100 8115 	bmi.w	3000b0fa <fault_diagnosis+0x2f6>
3000aed0:	0759      	lsls	r1, r3, #29
3000aed2:	f100 811d 	bmi.w	3000b110 <fault_diagnosis+0x30c>
3000aed6:	071a      	lsls	r2, r3, #28
3000aed8:	f100 8125 	bmi.w	3000b126 <fault_diagnosis+0x322>
3000aedc:	06db      	lsls	r3, r3, #27
3000aede:	f100 812d 	bmi.w	3000b13c <fault_diagnosis+0x338>
3000aee2:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000aee6:	07d8      	lsls	r0, r3, #31
3000aee8:	f100 8133 	bmi.w	3000b152 <fault_diagnosis+0x34e>
3000aeec:	0799      	lsls	r1, r3, #30
3000aeee:	d424      	bmi.n	3000af3a <fault_diagnosis+0x136>
3000aef0:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000aef4:	2b00      	cmp	r3, #0
3000aef6:	db00      	blt.n	3000aefa <fault_diagnosis+0xf6>
3000aef8:	bd10      	pop	{r4, pc}
3000aefa:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000aefc:	2900      	cmp	r1, #0
3000aefe:	d0fb      	beq.n	3000aef8 <fault_diagnosis+0xf4>
3000af00:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000af04:	07da      	lsls	r2, r3, #31
3000af06:	f100 8143 	bmi.w	3000b190 <fault_diagnosis+0x38c>
3000af0a:	0798      	lsls	r0, r3, #30
3000af0c:	f100 8138 	bmi.w	3000b180 <fault_diagnosis+0x37c>
3000af10:	0759      	lsls	r1, r3, #29
3000af12:	f100 812d 	bmi.w	3000b170 <fault_diagnosis+0x36c>
3000af16:	071a      	lsls	r2, r3, #28
3000af18:	f100 8141 	bmi.w	3000b19e <fault_diagnosis+0x39a>
3000af1c:	06db      	lsls	r3, r3, #27
3000af1e:	d5eb      	bpl.n	3000aef8 <fault_diagnosis+0xf4>
3000af20:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000af22:	4a43      	ldr	r2, [pc, #268]	; (3000b030 <fault_diagnosis+0x22c>)
3000af24:	4843      	ldr	r0, [pc, #268]	; (3000b034 <fault_diagnosis+0x230>)
3000af26:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000af2a:	f000 bc1d 	b.w	3000b768 <__DiagPrintf_veneer>
3000af2e:	4a42      	ldr	r2, [pc, #264]	; (3000b038 <fault_diagnosis+0x234>)
3000af30:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000af32:	4842      	ldr	r0, [pc, #264]	; (3000b03c <fault_diagnosis+0x238>)
3000af34:	f000 fc18 	bl	3000b768 <__DiagPrintf_veneer>
3000af38:	e76f      	b.n	3000ae1a <fault_diagnosis+0x16>
3000af3a:	4a41      	ldr	r2, [pc, #260]	; (3000b040 <fault_diagnosis+0x23c>)
3000af3c:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000af40:	4840      	ldr	r0, [pc, #256]	; (3000b044 <fault_diagnosis+0x240>)
3000af42:	f000 fc11 	bl	3000b768 <__DiagPrintf_veneer>
3000af46:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000af4a:	2b00      	cmp	r3, #0
3000af4c:	dad4      	bge.n	3000aef8 <fault_diagnosis+0xf4>
3000af4e:	e7d4      	b.n	3000aefa <fault_diagnosis+0xf6>
3000af50:	4a3d      	ldr	r2, [pc, #244]	; (3000b048 <fault_diagnosis+0x244>)
3000af52:	483e      	ldr	r0, [pc, #248]	; (3000b04c <fault_diagnosis+0x248>)
3000af54:	f000 fc08 	bl	3000b768 <__DiagPrintf_veneer>
3000af58:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000af5c:	079a      	lsls	r2, r3, #30
3000af5e:	f57f af66 	bpl.w	3000ae2e <fault_diagnosis+0x2a>
3000af62:	4a3b      	ldr	r2, [pc, #236]	; (3000b050 <fault_diagnosis+0x24c>)
3000af64:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000af66:	4839      	ldr	r0, [pc, #228]	; (3000b04c <fault_diagnosis+0x248>)
3000af68:	f000 fbfe 	bl	3000b768 <__DiagPrintf_veneer>
3000af6c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000af70:	0758      	lsls	r0, r3, #29
3000af72:	f57f af5f 	bpl.w	3000ae34 <fault_diagnosis+0x30>
3000af76:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000af78:	4a36      	ldr	r2, [pc, #216]	; (3000b054 <fault_diagnosis+0x250>)
3000af7a:	4834      	ldr	r0, [pc, #208]	; (3000b04c <fault_diagnosis+0x248>)
3000af7c:	f000 fbf4 	bl	3000b768 <__DiagPrintf_veneer>
3000af80:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000af84:	0719      	lsls	r1, r3, #28
3000af86:	f57f af58 	bpl.w	3000ae3a <fault_diagnosis+0x36>
3000af8a:	4a33      	ldr	r2, [pc, #204]	; (3000b058 <fault_diagnosis+0x254>)
3000af8c:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000af8e:	482f      	ldr	r0, [pc, #188]	; (3000b04c <fault_diagnosis+0x248>)
3000af90:	f000 fbea 	bl	3000b768 <__DiagPrintf_veneer>
3000af94:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000af98:	06da      	lsls	r2, r3, #27
3000af9a:	f57f af51 	bpl.w	3000ae40 <fault_diagnosis+0x3c>
3000af9e:	4a2f      	ldr	r2, [pc, #188]	; (3000b05c <fault_diagnosis+0x258>)
3000afa0:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000afa2:	482a      	ldr	r0, [pc, #168]	; (3000b04c <fault_diagnosis+0x248>)
3000afa4:	f000 fbe0 	bl	3000b768 <__DiagPrintf_veneer>
3000afa8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000afac:	0698      	lsls	r0, r3, #26
3000afae:	f57f af4a 	bpl.w	3000ae46 <fault_diagnosis+0x42>
3000afb2:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000afb4:	4a2a      	ldr	r2, [pc, #168]	; (3000b060 <fault_diagnosis+0x25c>)
3000afb6:	4825      	ldr	r0, [pc, #148]	; (3000b04c <fault_diagnosis+0x248>)
3000afb8:	f000 fbd6 	bl	3000b768 <__DiagPrintf_veneer>
3000afbc:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000afc0:	0619      	lsls	r1, r3, #24
3000afc2:	f57f af43 	bpl.w	3000ae4c <fault_diagnosis+0x48>
3000afc6:	4a27      	ldr	r2, [pc, #156]	; (3000b064 <fault_diagnosis+0x260>)
3000afc8:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000afca:	4820      	ldr	r0, [pc, #128]	; (3000b04c <fault_diagnosis+0x248>)
3000afcc:	f000 fbcc 	bl	3000b768 <__DiagPrintf_veneer>
3000afd0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000afd4:	e73a      	b.n	3000ae4c <fault_diagnosis+0x48>
3000afd6:	4a24      	ldr	r2, [pc, #144]	; (3000b068 <fault_diagnosis+0x264>)
3000afd8:	4824      	ldr	r0, [pc, #144]	; (3000b06c <fault_diagnosis+0x268>)
3000afda:	f000 fbc5 	bl	3000b768 <__DiagPrintf_veneer>
3000afde:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000afe2:	0788      	lsls	r0, r1, #30
3000afe4:	f57f af41 	bpl.w	3000ae6a <fault_diagnosis+0x66>
3000afe8:	4a21      	ldr	r2, [pc, #132]	; (3000b070 <fault_diagnosis+0x26c>)
3000afea:	4820      	ldr	r0, [pc, #128]	; (3000b06c <fault_diagnosis+0x268>)
3000afec:	f000 fbbc 	bl	3000b768 <__DiagPrintf_veneer>
3000aff0:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000aff4:	070a      	lsls	r2, r1, #28
3000aff6:	f57f af3b 	bpl.w	3000ae70 <fault_diagnosis+0x6c>
3000affa:	4a1e      	ldr	r2, [pc, #120]	; (3000b074 <fault_diagnosis+0x270>)
3000affc:	481b      	ldr	r0, [pc, #108]	; (3000b06c <fault_diagnosis+0x268>)
3000affe:	f000 fbb3 	bl	3000b768 <__DiagPrintf_veneer>
3000b002:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b006:	06cb      	lsls	r3, r1, #27
3000b008:	f57f af35 	bpl.w	3000ae76 <fault_diagnosis+0x72>
3000b00c:	4a1a      	ldr	r2, [pc, #104]	; (3000b078 <fault_diagnosis+0x274>)
3000b00e:	4817      	ldr	r0, [pc, #92]	; (3000b06c <fault_diagnosis+0x268>)
3000b010:	f000 fbaa 	bl	3000b768 <__DiagPrintf_veneer>
3000b014:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b018:	0688      	lsls	r0, r1, #26
3000b01a:	f57f af2f 	bpl.w	3000ae7c <fault_diagnosis+0x78>
3000b01e:	4a17      	ldr	r2, [pc, #92]	; (3000b07c <fault_diagnosis+0x278>)
3000b020:	4812      	ldr	r0, [pc, #72]	; (3000b06c <fault_diagnosis+0x268>)
3000b022:	f000 fba1 	bl	3000b768 <__DiagPrintf_veneer>
3000b026:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000b02a:	e727      	b.n	3000ae7c <fault_diagnosis+0x78>
3000b02c:	3000cd24 	.word	0x3000cd24
3000b030:	3000d5b4 	.word	0x3000d5b4
3000b034:	3000d50c 	.word	0x3000d50c
3000b038:	3000cd3c 	.word	0x3000cd3c
3000b03c:	3000cd68 	.word	0x3000cd68
3000b040:	3000d470 	.word	0x3000d470
3000b044:	3000d2b4 	.word	0x3000d2b4
3000b048:	3000cd80 	.word	0x3000cd80
3000b04c:	3000cdb0 	.word	0x3000cdb0
3000b050:	3000cdc8 	.word	0x3000cdc8
3000b054:	3000ce00 	.word	0x3000ce00
3000b058:	3000ce38 	.word	0x3000ce38
3000b05c:	3000ce70 	.word	0x3000ce70
3000b060:	3000cea0 	.word	0x3000cea0
3000b064:	3000cedc 	.word	0x3000cedc
3000b068:	3000cf4c 	.word	0x3000cf4c
3000b06c:	3000cf90 	.word	0x3000cf90
3000b070:	3000cfa8 	.word	0x3000cfa8
3000b074:	3000cfe4 	.word	0x3000cfe4
3000b078:	3000d01c 	.word	0x3000d01c
3000b07c:	3000d050 	.word	0x3000d050
3000b080:	4a51      	ldr	r2, [pc, #324]	; (3000b1c8 <fault_diagnosis+0x3c4>)
3000b082:	4852      	ldr	r0, [pc, #328]	; (3000b1cc <fault_diagnosis+0x3c8>)
3000b084:	f000 fb70 	bl	3000b768 <__DiagPrintf_veneer>
3000b088:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b08c:	078a      	lsls	r2, r1, #30
3000b08e:	f57f af03 	bpl.w	3000ae98 <fault_diagnosis+0x94>
3000b092:	4a4f      	ldr	r2, [pc, #316]	; (3000b1d0 <fault_diagnosis+0x3cc>)
3000b094:	484d      	ldr	r0, [pc, #308]	; (3000b1cc <fault_diagnosis+0x3c8>)
3000b096:	f000 fb67 	bl	3000b768 <__DiagPrintf_veneer>
3000b09a:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b09e:	074b      	lsls	r3, r1, #29
3000b0a0:	f57f aefd 	bpl.w	3000ae9e <fault_diagnosis+0x9a>
3000b0a4:	4a4b      	ldr	r2, [pc, #300]	; (3000b1d4 <fault_diagnosis+0x3d0>)
3000b0a6:	4849      	ldr	r0, [pc, #292]	; (3000b1cc <fault_diagnosis+0x3c8>)
3000b0a8:	f000 fb5e 	bl	3000b768 <__DiagPrintf_veneer>
3000b0ac:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b0b0:	0708      	lsls	r0, r1, #28
3000b0b2:	f57f aef7 	bpl.w	3000aea4 <fault_diagnosis+0xa0>
3000b0b6:	4a48      	ldr	r2, [pc, #288]	; (3000b1d8 <fault_diagnosis+0x3d4>)
3000b0b8:	4844      	ldr	r0, [pc, #272]	; (3000b1cc <fault_diagnosis+0x3c8>)
3000b0ba:	f000 fb55 	bl	3000b768 <__DiagPrintf_veneer>
3000b0be:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b0c2:	06ca      	lsls	r2, r1, #27
3000b0c4:	f57f aef1 	bpl.w	3000aeaa <fault_diagnosis+0xa6>
3000b0c8:	4a44      	ldr	r2, [pc, #272]	; (3000b1dc <fault_diagnosis+0x3d8>)
3000b0ca:	4840      	ldr	r0, [pc, #256]	; (3000b1cc <fault_diagnosis+0x3c8>)
3000b0cc:	f000 fb4c 	bl	3000b768 <__DiagPrintf_veneer>
3000b0d0:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b0d4:	068b      	lsls	r3, r1, #26
3000b0d6:	f57f aeeb 	bpl.w	3000aeb0 <fault_diagnosis+0xac>
3000b0da:	4a41      	ldr	r2, [pc, #260]	; (3000b1e0 <fault_diagnosis+0x3dc>)
3000b0dc:	483b      	ldr	r0, [pc, #236]	; (3000b1cc <fault_diagnosis+0x3c8>)
3000b0de:	f000 fb43 	bl	3000b768 <__DiagPrintf_veneer>
3000b0e2:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000b0e6:	e6e3      	b.n	3000aeb0 <fault_diagnosis+0xac>
3000b0e8:	4a3e      	ldr	r2, [pc, #248]	; (3000b1e4 <fault_diagnosis+0x3e0>)
3000b0ea:	483f      	ldr	r0, [pc, #252]	; (3000b1e8 <fault_diagnosis+0x3e4>)
3000b0ec:	f000 fb3c 	bl	3000b768 <__DiagPrintf_veneer>
3000b0f0:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b0f4:	0798      	lsls	r0, r3, #30
3000b0f6:	f57f aeeb 	bpl.w	3000aed0 <fault_diagnosis+0xcc>
3000b0fa:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b0fe:	4a3b      	ldr	r2, [pc, #236]	; (3000b1ec <fault_diagnosis+0x3e8>)
3000b100:	4839      	ldr	r0, [pc, #228]	; (3000b1e8 <fault_diagnosis+0x3e4>)
3000b102:	f000 fb31 	bl	3000b768 <__DiagPrintf_veneer>
3000b106:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b10a:	0759      	lsls	r1, r3, #29
3000b10c:	f57f aee3 	bpl.w	3000aed6 <fault_diagnosis+0xd2>
3000b110:	4a37      	ldr	r2, [pc, #220]	; (3000b1f0 <fault_diagnosis+0x3ec>)
3000b112:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b116:	4834      	ldr	r0, [pc, #208]	; (3000b1e8 <fault_diagnosis+0x3e4>)
3000b118:	f000 fb26 	bl	3000b768 <__DiagPrintf_veneer>
3000b11c:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b120:	071a      	lsls	r2, r3, #28
3000b122:	f57f aedb 	bpl.w	3000aedc <fault_diagnosis+0xd8>
3000b126:	4a33      	ldr	r2, [pc, #204]	; (3000b1f4 <fault_diagnosis+0x3f0>)
3000b128:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b12c:	482e      	ldr	r0, [pc, #184]	; (3000b1e8 <fault_diagnosis+0x3e4>)
3000b12e:	f000 fb1b 	bl	3000b768 <__DiagPrintf_veneer>
3000b132:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000b136:	06db      	lsls	r3, r3, #27
3000b138:	f57f aed3 	bpl.w	3000aee2 <fault_diagnosis+0xde>
3000b13c:	4a2e      	ldr	r2, [pc, #184]	; (3000b1f8 <fault_diagnosis+0x3f4>)
3000b13e:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b142:	4829      	ldr	r0, [pc, #164]	; (3000b1e8 <fault_diagnosis+0x3e4>)
3000b144:	f000 fb10 	bl	3000b768 <__DiagPrintf_veneer>
3000b148:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b14c:	07d8      	lsls	r0, r3, #31
3000b14e:	f57f aecd 	bpl.w	3000aeec <fault_diagnosis+0xe8>
3000b152:	4a2a      	ldr	r2, [pc, #168]	; (3000b1fc <fault_diagnosis+0x3f8>)
3000b154:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000b158:	4823      	ldr	r0, [pc, #140]	; (3000b1e8 <fault_diagnosis+0x3e4>)
3000b15a:	f000 fb05 	bl	3000b768 <__DiagPrintf_veneer>
3000b15e:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000b162:	e6c3      	b.n	3000aeec <fault_diagnosis+0xe8>
3000b164:	4a26      	ldr	r2, [pc, #152]	; (3000b200 <fault_diagnosis+0x3fc>)
3000b166:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000b168:	4826      	ldr	r0, [pc, #152]	; (3000b204 <fault_diagnosis+0x400>)
3000b16a:	f000 fafd 	bl	3000b768 <__DiagPrintf_veneer>
3000b16e:	e673      	b.n	3000ae58 <fault_diagnosis+0x54>
3000b170:	4a25      	ldr	r2, [pc, #148]	; (3000b208 <fault_diagnosis+0x404>)
3000b172:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b174:	4825      	ldr	r0, [pc, #148]	; (3000b20c <fault_diagnosis+0x408>)
3000b176:	f000 faf7 	bl	3000b768 <__DiagPrintf_veneer>
3000b17a:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b17e:	e6ca      	b.n	3000af16 <fault_diagnosis+0x112>
3000b180:	4a23      	ldr	r2, [pc, #140]	; (3000b210 <fault_diagnosis+0x40c>)
3000b182:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b184:	4821      	ldr	r0, [pc, #132]	; (3000b20c <fault_diagnosis+0x408>)
3000b186:	f000 faef 	bl	3000b768 <__DiagPrintf_veneer>
3000b18a:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b18e:	e6bf      	b.n	3000af10 <fault_diagnosis+0x10c>
3000b190:	4a20      	ldr	r2, [pc, #128]	; (3000b214 <fault_diagnosis+0x410>)
3000b192:	481e      	ldr	r0, [pc, #120]	; (3000b20c <fault_diagnosis+0x408>)
3000b194:	f000 fae8 	bl	3000b768 <__DiagPrintf_veneer>
3000b198:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b19c:	e6b5      	b.n	3000af0a <fault_diagnosis+0x106>
3000b19e:	4a1e      	ldr	r2, [pc, #120]	; (3000b218 <fault_diagnosis+0x414>)
3000b1a0:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000b1a2:	481a      	ldr	r0, [pc, #104]	; (3000b20c <fault_diagnosis+0x408>)
3000b1a4:	f000 fae0 	bl	3000b768 <__DiagPrintf_veneer>
3000b1a8:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000b1ac:	e6b6      	b.n	3000af1c <fault_diagnosis+0x118>
3000b1ae:	4a1b      	ldr	r2, [pc, #108]	; (3000b21c <fault_diagnosis+0x418>)
3000b1b0:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000b1b2:	481b      	ldr	r0, [pc, #108]	; (3000b220 <fault_diagnosis+0x41c>)
3000b1b4:	f000 fad8 	bl	3000b768 <__DiagPrintf_veneer>
3000b1b8:	e665      	b.n	3000ae86 <fault_diagnosis+0x82>
3000b1ba:	4a1a      	ldr	r2, [pc, #104]	; (3000b224 <fault_diagnosis+0x420>)
3000b1bc:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000b1be:	481a      	ldr	r0, [pc, #104]	; (3000b228 <fault_diagnosis+0x424>)
3000b1c0:	f000 fad2 	bl	3000b768 <__DiagPrintf_veneer>
3000b1c4:	e679      	b.n	3000aeba <fault_diagnosis+0xb6>
3000b1c6:	bf00      	nop
3000b1c8:	3000d0e4 	.word	0x3000d0e4
3000b1cc:	3000d118 	.word	0x3000d118
3000b1d0:	3000d130 	.word	0x3000d130
3000b1d4:	3000d168 	.word	0x3000d168
3000b1d8:	3000d1a0 	.word	0x3000d1a0
3000b1dc:	3000d1c8 	.word	0x3000d1c8
3000b1e0:	3000d1f0 	.word	0x3000d1f0
3000b1e4:	3000d26c 	.word	0x3000d26c
3000b1e8:	3000d2b4 	.word	0x3000d2b4
3000b1ec:	3000d2cc 	.word	0x3000d2cc
3000b1f0:	3000d318 	.word	0x3000d318
3000b1f4:	3000d378 	.word	0x3000d378
3000b1f8:	3000d3c0 	.word	0x3000d3c0
3000b1fc:	3000d41c 	.word	0x3000d41c
3000b200:	3000cf0c 	.word	0x3000cf0c
3000b204:	3000cf34 	.word	0x3000cf34
3000b208:	3000d558 	.word	0x3000d558
3000b20c:	3000d50c 	.word	0x3000d50c
3000b210:	3000d524 	.word	0x3000d524
3000b214:	3000d4dc 	.word	0x3000d4dc
3000b218:	3000d584 	.word	0x3000d584
3000b21c:	3000d09c 	.word	0x3000d09c
3000b220:	3000d0cc 	.word	0x3000d0cc
3000b224:	3000d230 	.word	0x3000d230
3000b228:	3000d254 	.word	0x3000d254

3000b22c <rtk_log_write>:
3000b22c:	b40c      	push	{r2, r3}
3000b22e:	4b18      	ldr	r3, [pc, #96]	; (3000b290 <rtk_log_write+0x64>)
3000b230:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000b234:	681a      	ldr	r2, [r3, #0]
3000b236:	b083      	sub	sp, #12
3000b238:	4680      	mov	r8, r0
3000b23a:	460e      	mov	r6, r1
3000b23c:	2a02      	cmp	r2, #2
3000b23e:	d825      	bhi.n	3000b28c <rtk_log_write+0x60>
3000b240:	681f      	ldr	r7, [r3, #0]
3000b242:	b1a7      	cbz	r7, 3000b26e <rtk_log_write+0x42>
3000b244:	4d13      	ldr	r5, [pc, #76]	; (3000b294 <rtk_log_write+0x68>)
3000b246:	2400      	movs	r4, #0
3000b248:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000b2a0 <rtk_log_write+0x74>
3000b24c:	e002      	b.n	3000b254 <rtk_log_write+0x28>
3000b24e:	3401      	adds	r4, #1
3000b250:	42a7      	cmp	r7, r4
3000b252:	d90c      	bls.n	3000b26e <rtk_log_write+0x42>
3000b254:	4628      	mov	r0, r5
3000b256:	4631      	mov	r1, r6
3000b258:	350b      	adds	r5, #11
3000b25a:	47c8      	blx	r9
3000b25c:	2800      	cmp	r0, #0
3000b25e:	d1f6      	bne.n	3000b24e <rtk_log_write+0x22>
3000b260:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000b264:	4b0c      	ldr	r3, [pc, #48]	; (3000b298 <rtk_log_write+0x6c>)
3000b266:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000b26a:	5d1b      	ldrb	r3, [r3, r4]
3000b26c:	e001      	b.n	3000b272 <rtk_log_write+0x46>
3000b26e:	4b0b      	ldr	r3, [pc, #44]	; (3000b29c <rtk_log_write+0x70>)
3000b270:	781b      	ldrb	r3, [r3, #0]
3000b272:	4598      	cmp	r8, r3
3000b274:	d805      	bhi.n	3000b282 <rtk_log_write+0x56>
3000b276:	aa0b      	add	r2, sp, #44	; 0x2c
3000b278:	990a      	ldr	r1, [sp, #40]	; 0x28
3000b27a:	2000      	movs	r0, #0
3000b27c:	9201      	str	r2, [sp, #4]
3000b27e:	f000 fa5b 	bl	3000b738 <__DiagVSprintf_veneer>
3000b282:	b003      	add	sp, #12
3000b284:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000b288:	b002      	add	sp, #8
3000b28a:	4770      	bx	lr
3000b28c:	2703      	movs	r7, #3
3000b28e:	e7d9      	b.n	3000b244 <rtk_log_write+0x18>
3000b290:	3000f1a8 	.word	0x3000f1a8
3000b294:	3000f1ad 	.word	0x3000f1ad
3000b298:	3000f1ac 	.word	0x3000f1ac
3000b29c:	3000e78c 	.word	0x3000e78c
3000b2a0:	00012c89 	.word	0x00012c89

3000b2a4 <BOOT_VerCheck>:
3000b2a4:	b570      	push	{r4, r5, r6, lr}
3000b2a6:	4b1d      	ldr	r3, [pc, #116]	; (3000b31c <BOOT_VerCheck+0x78>)
3000b2a8:	4798      	blx	r3
3000b2aa:	2800      	cmp	r0, #0
3000b2ac:	d135      	bne.n	3000b31a <BOOT_VerCheck+0x76>
3000b2ae:	4b1c      	ldr	r3, [pc, #112]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b2b0:	2403      	movs	r4, #3
3000b2b2:	4d1c      	ldr	r5, [pc, #112]	; (3000b324 <BOOT_VerCheck+0x80>)
3000b2b4:	2003      	movs	r0, #3
3000b2b6:	4a1c      	ldr	r2, [pc, #112]	; (3000b328 <BOOT_VerCheck+0x84>)
3000b2b8:	4619      	mov	r1, r3
3000b2ba:	f7ff ffb7 	bl	3000b22c <rtk_log_write>
3000b2be:	4b18      	ldr	r3, [pc, #96]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b2c0:	462a      	mov	r2, r5
3000b2c2:	2003      	movs	r0, #3
3000b2c4:	4619      	mov	r1, r3
3000b2c6:	f7ff ffb1 	bl	3000b22c <rtk_log_write>
3000b2ca:	4b15      	ldr	r3, [pc, #84]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b2cc:	4a17      	ldr	r2, [pc, #92]	; (3000b32c <BOOT_VerCheck+0x88>)
3000b2ce:	2003      	movs	r0, #3
3000b2d0:	4619      	mov	r1, r3
3000b2d2:	f7ff ffab 	bl	3000b22c <rtk_log_write>
3000b2d6:	4b12      	ldr	r3, [pc, #72]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b2d8:	4a15      	ldr	r2, [pc, #84]	; (3000b330 <BOOT_VerCheck+0x8c>)
3000b2da:	2003      	movs	r0, #3
3000b2dc:	4619      	mov	r1, r3
3000b2de:	f7ff ffa5 	bl	3000b22c <rtk_log_write>
3000b2e2:	4b0f      	ldr	r3, [pc, #60]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b2e4:	4a13      	ldr	r2, [pc, #76]	; (3000b334 <BOOT_VerCheck+0x90>)
3000b2e6:	2003      	movs	r0, #3
3000b2e8:	4619      	mov	r1, r3
3000b2ea:	f7ff ff9f 	bl	3000b22c <rtk_log_write>
3000b2ee:	4b0c      	ldr	r3, [pc, #48]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b2f0:	462a      	mov	r2, r5
3000b2f2:	2003      	movs	r0, #3
3000b2f4:	4619      	mov	r1, r3
3000b2f6:	f7ff ff99 	bl	3000b22c <rtk_log_write>
3000b2fa:	4b09      	ldr	r3, [pc, #36]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b2fc:	4a0e      	ldr	r2, [pc, #56]	; (3000b338 <BOOT_VerCheck+0x94>)
3000b2fe:	2003      	movs	r0, #3
3000b300:	4619      	mov	r1, r3
3000b302:	f7ff ff93 	bl	3000b22c <rtk_log_write>
3000b306:	3c01      	subs	r4, #1
3000b308:	d1d9      	bne.n	3000b2be <BOOT_VerCheck+0x1a>
3000b30a:	4b05      	ldr	r3, [pc, #20]	; (3000b320 <BOOT_VerCheck+0x7c>)
3000b30c:	2003      	movs	r0, #3
3000b30e:	4a0b      	ldr	r2, [pc, #44]	; (3000b33c <BOOT_VerCheck+0x98>)
3000b310:	4619      	mov	r1, r3
3000b312:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000b316:	f7ff bf89 	b.w	3000b22c <rtk_log_write>
3000b31a:	bd70      	pop	{r4, r5, r6, pc}
3000b31c:	30008f3d 	.word	0x30008f3d
3000b320:	3000d686 	.word	0x3000d686
3000b324:	3000d6ce 	.word	0x3000d6ce
3000b328:	3000d68b 	.word	0x3000d68b
3000b32c:	3000d711 	.word	0x3000d711
3000b330:	3000d754 	.word	0x3000d754
3000b334:	3000d797 	.word	0x3000d797
3000b338:	3000d7da 	.word	0x3000d7da
3000b33c:	3000d81d 	.word	0x3000d81d

3000b340 <__aeabi_uldivmod>:
3000b340:	b953      	cbnz	r3, 3000b358 <__aeabi_uldivmod+0x18>
3000b342:	b94a      	cbnz	r2, 3000b358 <__aeabi_uldivmod+0x18>
3000b344:	2900      	cmp	r1, #0
3000b346:	bf08      	it	eq
3000b348:	2800      	cmpeq	r0, #0
3000b34a:	bf1c      	itt	ne
3000b34c:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000b350:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000b354:	f000 b960 	b.w	3000b618 <__aeabi_idiv0>
3000b358:	f1ad 0c08 	sub.w	ip, sp, #8
3000b35c:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000b360:	f000 f806 	bl	3000b370 <__udivmoddi4>
3000b364:	f8dd e004 	ldr.w	lr, [sp, #4]
3000b368:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000b36c:	b004      	add	sp, #16
3000b36e:	4770      	bx	lr

3000b370 <__udivmoddi4>:
3000b370:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b374:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000b376:	4688      	mov	r8, r1
3000b378:	468e      	mov	lr, r1
3000b37a:	4615      	mov	r5, r2
3000b37c:	4604      	mov	r4, r0
3000b37e:	4619      	mov	r1, r3
3000b380:	2b00      	cmp	r3, #0
3000b382:	f040 80c6 	bne.w	3000b512 <__udivmoddi4+0x1a2>
3000b386:	4542      	cmp	r2, r8
3000b388:	fab2 f782 	clz	r7, r2
3000b38c:	d946      	bls.n	3000b41c <__udivmoddi4+0xac>
3000b38e:	b14f      	cbz	r7, 3000b3a4 <__udivmoddi4+0x34>
3000b390:	f1c7 0c20 	rsb	ip, r7, #32
3000b394:	fa08 f307 	lsl.w	r3, r8, r7
3000b398:	40bd      	lsls	r5, r7
3000b39a:	40bc      	lsls	r4, r7
3000b39c:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b3a0:	ea4c 0e03 	orr.w	lr, ip, r3
3000b3a4:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000b3a8:	fa1f fc85 	uxth.w	ip, r5
3000b3ac:	0c22      	lsrs	r2, r4, #16
3000b3ae:	fbbe f9f8 	udiv	r9, lr, r8
3000b3b2:	fb08 e319 	mls	r3, r8, r9, lr
3000b3b6:	fb09 fa0c 	mul.w	sl, r9, ip
3000b3ba:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000b3be:	459a      	cmp	sl, r3
3000b3c0:	d928      	bls.n	3000b414 <__udivmoddi4+0xa4>
3000b3c2:	18eb      	adds	r3, r5, r3
3000b3c4:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000b3c8:	d204      	bcs.n	3000b3d4 <__udivmoddi4+0x64>
3000b3ca:	459a      	cmp	sl, r3
3000b3cc:	d902      	bls.n	3000b3d4 <__udivmoddi4+0x64>
3000b3ce:	f1a9 0002 	sub.w	r0, r9, #2
3000b3d2:	442b      	add	r3, r5
3000b3d4:	eba3 030a 	sub.w	r3, r3, sl
3000b3d8:	b2a4      	uxth	r4, r4
3000b3da:	fbb3 f2f8 	udiv	r2, r3, r8
3000b3de:	fb08 3312 	mls	r3, r8, r2, r3
3000b3e2:	fb02 fc0c 	mul.w	ip, r2, ip
3000b3e6:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b3ea:	45a4      	cmp	ip, r4
3000b3ec:	d914      	bls.n	3000b418 <__udivmoddi4+0xa8>
3000b3ee:	192c      	adds	r4, r5, r4
3000b3f0:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000b3f4:	d203      	bcs.n	3000b3fe <__udivmoddi4+0x8e>
3000b3f6:	45a4      	cmp	ip, r4
3000b3f8:	d901      	bls.n	3000b3fe <__udivmoddi4+0x8e>
3000b3fa:	1e93      	subs	r3, r2, #2
3000b3fc:	442c      	add	r4, r5
3000b3fe:	eba4 040c 	sub.w	r4, r4, ip
3000b402:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000b406:	b11e      	cbz	r6, 3000b410 <__udivmoddi4+0xa0>
3000b408:	40fc      	lsrs	r4, r7
3000b40a:	2300      	movs	r3, #0
3000b40c:	6034      	str	r4, [r6, #0]
3000b40e:	6073      	str	r3, [r6, #4]
3000b410:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b414:	4648      	mov	r0, r9
3000b416:	e7dd      	b.n	3000b3d4 <__udivmoddi4+0x64>
3000b418:	4613      	mov	r3, r2
3000b41a:	e7f0      	b.n	3000b3fe <__udivmoddi4+0x8e>
3000b41c:	b902      	cbnz	r2, 3000b420 <__udivmoddi4+0xb0>
3000b41e:	deff      	udf	#255	; 0xff
3000b420:	bb8f      	cbnz	r7, 3000b486 <__udivmoddi4+0x116>
3000b422:	eba8 0302 	sub.w	r3, r8, r2
3000b426:	2101      	movs	r1, #1
3000b428:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b42c:	b2aa      	uxth	r2, r5
3000b42e:	0c20      	lsrs	r0, r4, #16
3000b430:	fbb3 fcfe 	udiv	ip, r3, lr
3000b434:	fb0e 331c 	mls	r3, lr, ip, r3
3000b438:	fb0c f802 	mul.w	r8, ip, r2
3000b43c:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000b440:	4598      	cmp	r8, r3
3000b442:	d962      	bls.n	3000b50a <__udivmoddi4+0x19a>
3000b444:	18eb      	adds	r3, r5, r3
3000b446:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000b44a:	d204      	bcs.n	3000b456 <__udivmoddi4+0xe6>
3000b44c:	4598      	cmp	r8, r3
3000b44e:	d902      	bls.n	3000b456 <__udivmoddi4+0xe6>
3000b450:	f1ac 0002 	sub.w	r0, ip, #2
3000b454:	442b      	add	r3, r5
3000b456:	eba3 0308 	sub.w	r3, r3, r8
3000b45a:	b2a4      	uxth	r4, r4
3000b45c:	fbb3 fcfe 	udiv	ip, r3, lr
3000b460:	fb0e 331c 	mls	r3, lr, ip, r3
3000b464:	fb0c f202 	mul.w	r2, ip, r2
3000b468:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b46c:	42a2      	cmp	r2, r4
3000b46e:	d94e      	bls.n	3000b50e <__udivmoddi4+0x19e>
3000b470:	192c      	adds	r4, r5, r4
3000b472:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000b476:	d204      	bcs.n	3000b482 <__udivmoddi4+0x112>
3000b478:	42a2      	cmp	r2, r4
3000b47a:	d902      	bls.n	3000b482 <__udivmoddi4+0x112>
3000b47c:	f1ac 0302 	sub.w	r3, ip, #2
3000b480:	442c      	add	r4, r5
3000b482:	1aa4      	subs	r4, r4, r2
3000b484:	e7bd      	b.n	3000b402 <__udivmoddi4+0x92>
3000b486:	f1c7 0c20 	rsb	ip, r7, #32
3000b48a:	40bd      	lsls	r5, r7
3000b48c:	fa08 f307 	lsl.w	r3, r8, r7
3000b490:	40bc      	lsls	r4, r7
3000b492:	fa28 f90c 	lsr.w	r9, r8, ip
3000b496:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b49a:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b49e:	ea4c 0203 	orr.w	r2, ip, r3
3000b4a2:	b2ab      	uxth	r3, r5
3000b4a4:	fbb9 fcfe 	udiv	ip, r9, lr
3000b4a8:	0c11      	lsrs	r1, r2, #16
3000b4aa:	fb0e 901c 	mls	r0, lr, ip, r9
3000b4ae:	fb0c f803 	mul.w	r8, ip, r3
3000b4b2:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b4b6:	4580      	cmp	r8, r0
3000b4b8:	d923      	bls.n	3000b502 <__udivmoddi4+0x192>
3000b4ba:	1828      	adds	r0, r5, r0
3000b4bc:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000b4c0:	d204      	bcs.n	3000b4cc <__udivmoddi4+0x15c>
3000b4c2:	4580      	cmp	r8, r0
3000b4c4:	d902      	bls.n	3000b4cc <__udivmoddi4+0x15c>
3000b4c6:	f1ac 0102 	sub.w	r1, ip, #2
3000b4ca:	4428      	add	r0, r5
3000b4cc:	eba0 0008 	sub.w	r0, r0, r8
3000b4d0:	b292      	uxth	r2, r2
3000b4d2:	fbb0 fcfe 	udiv	ip, r0, lr
3000b4d6:	fb0e 001c 	mls	r0, lr, ip, r0
3000b4da:	fb0c f803 	mul.w	r8, ip, r3
3000b4de:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000b4e2:	4598      	cmp	r8, r3
3000b4e4:	d90f      	bls.n	3000b506 <__udivmoddi4+0x196>
3000b4e6:	18eb      	adds	r3, r5, r3
3000b4e8:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000b4ec:	d204      	bcs.n	3000b4f8 <__udivmoddi4+0x188>
3000b4ee:	4598      	cmp	r8, r3
3000b4f0:	d902      	bls.n	3000b4f8 <__udivmoddi4+0x188>
3000b4f2:	f1ac 0202 	sub.w	r2, ip, #2
3000b4f6:	442b      	add	r3, r5
3000b4f8:	eba3 0308 	sub.w	r3, r3, r8
3000b4fc:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000b500:	e792      	b.n	3000b428 <__udivmoddi4+0xb8>
3000b502:	4661      	mov	r1, ip
3000b504:	e7e2      	b.n	3000b4cc <__udivmoddi4+0x15c>
3000b506:	4662      	mov	r2, ip
3000b508:	e7f6      	b.n	3000b4f8 <__udivmoddi4+0x188>
3000b50a:	4660      	mov	r0, ip
3000b50c:	e7a3      	b.n	3000b456 <__udivmoddi4+0xe6>
3000b50e:	4663      	mov	r3, ip
3000b510:	e7b7      	b.n	3000b482 <__udivmoddi4+0x112>
3000b512:	4543      	cmp	r3, r8
3000b514:	d905      	bls.n	3000b522 <__udivmoddi4+0x1b2>
3000b516:	b10e      	cbz	r6, 3000b51c <__udivmoddi4+0x1ac>
3000b518:	e9c6 0800 	strd	r0, r8, [r6]
3000b51c:	2100      	movs	r1, #0
3000b51e:	4608      	mov	r0, r1
3000b520:	e776      	b.n	3000b410 <__udivmoddi4+0xa0>
3000b522:	fab3 fc83 	clz	ip, r3
3000b526:	f1bc 0f00 	cmp.w	ip, #0
3000b52a:	d110      	bne.n	3000b54e <__udivmoddi4+0x1de>
3000b52c:	4543      	cmp	r3, r8
3000b52e:	d301      	bcc.n	3000b534 <__udivmoddi4+0x1c4>
3000b530:	4282      	cmp	r2, r0
3000b532:	d80a      	bhi.n	3000b54a <__udivmoddi4+0x1da>
3000b534:	1a84      	subs	r4, r0, r2
3000b536:	eb68 0303 	sbc.w	r3, r8, r3
3000b53a:	2001      	movs	r0, #1
3000b53c:	469e      	mov	lr, r3
3000b53e:	2e00      	cmp	r6, #0
3000b540:	d068      	beq.n	3000b614 <__udivmoddi4+0x2a4>
3000b542:	e9c6 4e00 	strd	r4, lr, [r6]
3000b546:	2100      	movs	r1, #0
3000b548:	e762      	b.n	3000b410 <__udivmoddi4+0xa0>
3000b54a:	4660      	mov	r0, ip
3000b54c:	e7f7      	b.n	3000b53e <__udivmoddi4+0x1ce>
3000b54e:	f1cc 0e20 	rsb	lr, ip, #32
3000b552:	fa03 f30c 	lsl.w	r3, r3, ip
3000b556:	fa02 f50c 	lsl.w	r5, r2, ip
3000b55a:	fa00 f70c 	lsl.w	r7, r0, ip
3000b55e:	fa22 f40e 	lsr.w	r4, r2, lr
3000b562:	fa28 fb0e 	lsr.w	fp, r8, lr
3000b566:	fa20 f10e 	lsr.w	r1, r0, lr
3000b56a:	431c      	orrs	r4, r3
3000b56c:	fa08 f30c 	lsl.w	r3, r8, ip
3000b570:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000b574:	4319      	orrs	r1, r3
3000b576:	b2a3      	uxth	r3, r4
3000b578:	0c0a      	lsrs	r2, r1, #16
3000b57a:	fbbb f8f9 	udiv	r8, fp, r9
3000b57e:	fb09 bb18 	mls	fp, r9, r8, fp
3000b582:	fb08 fa03 	mul.w	sl, r8, r3
3000b586:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000b58a:	4592      	cmp	sl, r2
3000b58c:	d93e      	bls.n	3000b60c <__udivmoddi4+0x29c>
3000b58e:	18a2      	adds	r2, r4, r2
3000b590:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000b594:	d204      	bcs.n	3000b5a0 <__udivmoddi4+0x230>
3000b596:	4592      	cmp	sl, r2
3000b598:	d902      	bls.n	3000b5a0 <__udivmoddi4+0x230>
3000b59a:	f1a8 0002 	sub.w	r0, r8, #2
3000b59e:	4422      	add	r2, r4
3000b5a0:	eba2 020a 	sub.w	r2, r2, sl
3000b5a4:	b289      	uxth	r1, r1
3000b5a6:	fbb2 f8f9 	udiv	r8, r2, r9
3000b5aa:	fb09 2218 	mls	r2, r9, r8, r2
3000b5ae:	fb08 f303 	mul.w	r3, r8, r3
3000b5b2:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000b5b6:	4293      	cmp	r3, r2
3000b5b8:	d92a      	bls.n	3000b610 <__udivmoddi4+0x2a0>
3000b5ba:	18a2      	adds	r2, r4, r2
3000b5bc:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000b5c0:	d204      	bcs.n	3000b5cc <__udivmoddi4+0x25c>
3000b5c2:	4293      	cmp	r3, r2
3000b5c4:	d902      	bls.n	3000b5cc <__udivmoddi4+0x25c>
3000b5c6:	f1a8 0102 	sub.w	r1, r8, #2
3000b5ca:	4422      	add	r2, r4
3000b5cc:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b5d0:	1ad2      	subs	r2, r2, r3
3000b5d2:	fba0 9805 	umull	r9, r8, r0, r5
3000b5d6:	4542      	cmp	r2, r8
3000b5d8:	464b      	mov	r3, r9
3000b5da:	4641      	mov	r1, r8
3000b5dc:	d302      	bcc.n	3000b5e4 <__udivmoddi4+0x274>
3000b5de:	d106      	bne.n	3000b5ee <__udivmoddi4+0x27e>
3000b5e0:	454f      	cmp	r7, r9
3000b5e2:	d204      	bcs.n	3000b5ee <__udivmoddi4+0x27e>
3000b5e4:	3801      	subs	r0, #1
3000b5e6:	ebb9 0305 	subs.w	r3, r9, r5
3000b5ea:	eb68 0104 	sbc.w	r1, r8, r4
3000b5ee:	b18e      	cbz	r6, 3000b614 <__udivmoddi4+0x2a4>
3000b5f0:	1afc      	subs	r4, r7, r3
3000b5f2:	eb62 0701 	sbc.w	r7, r2, r1
3000b5f6:	fa07 fe0e 	lsl.w	lr, r7, lr
3000b5fa:	fa24 f30c 	lsr.w	r3, r4, ip
3000b5fe:	fa27 f70c 	lsr.w	r7, r7, ip
3000b602:	ea4e 0303 	orr.w	r3, lr, r3
3000b606:	e9c6 3700 	strd	r3, r7, [r6]
3000b60a:	e79c      	b.n	3000b546 <__udivmoddi4+0x1d6>
3000b60c:	4640      	mov	r0, r8
3000b60e:	e7c7      	b.n	3000b5a0 <__udivmoddi4+0x230>
3000b610:	4641      	mov	r1, r8
3000b612:	e7db      	b.n	3000b5cc <__udivmoddi4+0x25c>
3000b614:	4631      	mov	r1, r6
3000b616:	e6fb      	b.n	3000b410 <__udivmoddi4+0xa0>

3000b618 <__aeabi_idiv0>:
3000b618:	4770      	bx	lr
3000b61a:	bf00      	nop

3000b61c <_mbtowc_r>:
3000b61c:	b430      	push	{r4, r5}
3000b61e:	4c06      	ldr	r4, [pc, #24]	; (3000b638 <_mbtowc_r+0x1c>)
3000b620:	4d06      	ldr	r5, [pc, #24]	; (3000b63c <_mbtowc_r+0x20>)
3000b622:	6824      	ldr	r4, [r4, #0]
3000b624:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b626:	2c00      	cmp	r4, #0
3000b628:	bf08      	it	eq
3000b62a:	462c      	moveq	r4, r5
3000b62c:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000b630:	46a4      	mov	ip, r4
3000b632:	bc30      	pop	{r4, r5}
3000b634:	4760      	bx	ip
3000b636:	bf00      	nop
3000b638:	3000e900 	.word	0x3000e900
3000b63c:	3000e790 	.word	0x3000e790

3000b640 <__ascii_mbtowc>:
3000b640:	b082      	sub	sp, #8
3000b642:	b901      	cbnz	r1, 3000b646 <__ascii_mbtowc+0x6>
3000b644:	a901      	add	r1, sp, #4
3000b646:	b142      	cbz	r2, 3000b65a <__ascii_mbtowc+0x1a>
3000b648:	b14b      	cbz	r3, 3000b65e <__ascii_mbtowc+0x1e>
3000b64a:	7813      	ldrb	r3, [r2, #0]
3000b64c:	600b      	str	r3, [r1, #0]
3000b64e:	7812      	ldrb	r2, [r2, #0]
3000b650:	1e10      	subs	r0, r2, #0
3000b652:	bf18      	it	ne
3000b654:	2001      	movne	r0, #1
3000b656:	b002      	add	sp, #8
3000b658:	4770      	bx	lr
3000b65a:	4610      	mov	r0, r2
3000b65c:	e7fb      	b.n	3000b656 <__ascii_mbtowc+0x16>
3000b65e:	f06f 0001 	mvn.w	r0, #1
3000b662:	e7f8      	b.n	3000b656 <__ascii_mbtowc+0x16>

3000b664 <_wctomb_r>:
3000b664:	b430      	push	{r4, r5}
3000b666:	4c06      	ldr	r4, [pc, #24]	; (3000b680 <_wctomb_r+0x1c>)
3000b668:	4d06      	ldr	r5, [pc, #24]	; (3000b684 <_wctomb_r+0x20>)
3000b66a:	6824      	ldr	r4, [r4, #0]
3000b66c:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b66e:	2c00      	cmp	r4, #0
3000b670:	bf08      	it	eq
3000b672:	462c      	moveq	r4, r5
3000b674:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000b678:	46a4      	mov	ip, r4
3000b67a:	bc30      	pop	{r4, r5}
3000b67c:	4760      	bx	ip
3000b67e:	bf00      	nop
3000b680:	3000e900 	.word	0x3000e900
3000b684:	3000e790 	.word	0x3000e790

3000b688 <__ascii_wctomb>:
3000b688:	b149      	cbz	r1, 3000b69e <__ascii_wctomb+0x16>
3000b68a:	2aff      	cmp	r2, #255	; 0xff
3000b68c:	bf8d      	iteet	hi
3000b68e:	238a      	movhi	r3, #138	; 0x8a
3000b690:	2001      	movls	r0, #1
3000b692:	700a      	strbls	r2, [r1, #0]
3000b694:	6003      	strhi	r3, [r0, #0]
3000b696:	bf88      	it	hi
3000b698:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000b69c:	4770      	bx	lr
3000b69e:	4608      	mov	r0, r1
3000b6a0:	4770      	bx	lr
3000b6a2:	bf00      	nop
3000b6a4:	0000      	movs	r0, r0
	...

3000b6a8 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000b6a8:	b401      	push	{r0}
3000b6aa:	4802      	ldr	r0, [pc, #8]	; (3000b6b4 <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000b6ac:	4684      	mov	ip, r0
3000b6ae:	bc01      	pop	{r0}
3000b6b0:	4760      	bx	ip
3000b6b2:	bf00      	nop
3000b6b4:	00005b4d 	.word	0x00005b4d

3000b6b8 <__SBOOT_Validate_PubKey_B_veneer>:
3000b6b8:	b401      	push	{r0}
3000b6ba:	4802      	ldr	r0, [pc, #8]	; (3000b6c4 <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000b6bc:	4684      	mov	ip, r0
3000b6be:	bc01      	pop	{r0}
3000b6c0:	4760      	bx	ip
3000b6c2:	bf00      	nop
3000b6c4:	00003495 	.word	0x00003495

3000b6c8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000b6c8:	b401      	push	{r0}
3000b6ca:	4802      	ldr	r0, [pc, #8]	; (3000b6d4 <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000b6cc:	4684      	mov	ip, r0
3000b6ce:	bc01      	pop	{r0}
3000b6d0:	4760      	bx	ip
3000b6d2:	bf00      	nop
3000b6d4:	000024dd 	.word	0x000024dd

3000b6d8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000b6d8:	b401      	push	{r0}
3000b6da:	4802      	ldr	r0, [pc, #8]	; (3000b6e4 <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000b6dc:	4684      	mov	ip, r0
3000b6de:	bc01      	pop	{r0}
3000b6e0:	4760      	bx	ip
3000b6e2:	bf00      	nop
3000b6e4:	00002369 	.word	0x00002369

3000b6e8 <____wrap_memcpy_veneer>:
3000b6e8:	b401      	push	{r0}
3000b6ea:	4802      	ldr	r0, [pc, #8]	; (3000b6f4 <____wrap_memcpy_veneer+0xc>)
3000b6ec:	4684      	mov	ip, r0
3000b6ee:	bc01      	pop	{r0}
3000b6f0:	4760      	bx	ip
3000b6f2:	bf00      	nop
3000b6f4:	00012f05 	.word	0x00012f05

3000b6f8 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000b6f8:	b401      	push	{r0}
3000b6fa:	4802      	ldr	r0, [pc, #8]	; (3000b704 <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000b6fc:	4684      	mov	ip, r0
3000b6fe:	bc01      	pop	{r0}
3000b700:	4760      	bx	ip
3000b702:	bf00      	nop
3000b704:	00005a45 	.word	0x00005a45

3000b708 <__INT_HardFault_C_veneer>:
3000b708:	b401      	push	{r0}
3000b70a:	4802      	ldr	r0, [pc, #8]	; (3000b714 <__INT_HardFault_C_veneer+0xc>)
3000b70c:	4684      	mov	ip, r0
3000b70e:	bc01      	pop	{r0}
3000b710:	4760      	bx	ip
3000b712:	bf00      	nop
3000b714:	0000d8ed 	.word	0x0000d8ed

3000b718 <____wrap_memset_veneer>:
3000b718:	b401      	push	{r0}
3000b71a:	4802      	ldr	r0, [pc, #8]	; (3000b724 <____wrap_memset_veneer+0xc>)
3000b71c:	4684      	mov	ip, r0
3000b71e:	bc01      	pop	{r0}
3000b720:	4760      	bx	ip
3000b722:	bf00      	nop
3000b724:	00012efd 	.word	0x00012efd

3000b728 <__CRYPTO_SHA_Init_B_veneer>:
3000b728:	b401      	push	{r0}
3000b72a:	4802      	ldr	r0, [pc, #8]	; (3000b734 <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000b72c:	4684      	mov	ip, r0
3000b72e:	bc01      	pop	{r0}
3000b730:	4760      	bx	ip
3000b732:	bf00      	nop
3000b734:	00002451 	.word	0x00002451

3000b738 <__DiagVSprintf_veneer>:
3000b738:	b401      	push	{r0}
3000b73a:	4802      	ldr	r0, [pc, #8]	; (3000b744 <__DiagVSprintf_veneer+0xc>)
3000b73c:	4684      	mov	ip, r0
3000b73e:	bc01      	pop	{r0}
3000b740:	4760      	bx	ip
3000b742:	bf00      	nop
3000b744:	0000e331 	.word	0x0000e331

3000b748 <__rtl_crypto_sha2_update_B_veneer>:
3000b748:	b401      	push	{r0}
3000b74a:	4802      	ldr	r0, [pc, #8]	; (3000b754 <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000b74c:	4684      	mov	ip, r0
3000b74e:	bc01      	pop	{r0}
3000b750:	4760      	bx	ip
3000b752:	bf00      	nop
3000b754:	00005385 	.word	0x00005385

3000b758 <__rtl_crypto_sha2_final_A_veneer>:
3000b758:	b401      	push	{r0}
3000b75a:	4802      	ldr	r0, [pc, #8]	; (3000b764 <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000b75c:	4684      	mov	ip, r0
3000b75e:	bc01      	pop	{r0}
3000b760:	4760      	bx	ip
3000b762:	bf00      	nop
3000b764:	0000527d 	.word	0x0000527d

3000b768 <__DiagPrintf_veneer>:
3000b768:	b401      	push	{r0}
3000b76a:	4802      	ldr	r0, [pc, #8]	; (3000b774 <__DiagPrintf_veneer+0xc>)
3000b76c:	4684      	mov	ip, r0
3000b76e:	bc01      	pop	{r0}
3000b770:	4760      	bx	ip
3000b772:	bf00      	nop
3000b774:	0000e5f9 	.word	0x0000e5f9

3000b778 <__rtl_crypto_sha2_init_B_veneer>:
3000b778:	b401      	push	{r0}
3000b77a:	4802      	ldr	r0, [pc, #8]	; (3000b784 <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000b77c:	4684      	mov	ip, r0
3000b77e:	bc01      	pop	{r0}
3000b780:	4760      	bx	ip
3000b782:	bf00      	nop
3000b784:	00004ffd 	.word	0x00004ffd

3000b788 <__rtl_crypto_sha2_init_A_veneer>:
3000b788:	b401      	push	{r0}
3000b78a:	4802      	ldr	r0, [pc, #8]	; (3000b794 <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000b78c:	4684      	mov	ip, r0
3000b78e:	bc01      	pop	{r0}
3000b790:	4760      	bx	ip
3000b792:	bf00      	nop
3000b794:	00004e89 	.word	0x00004e89

3000b798 <__SBOOT_Validate_Signature_A_veneer>:
3000b798:	b401      	push	{r0}
3000b79a:	4802      	ldr	r0, [pc, #8]	; (3000b7a4 <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000b79c:	4684      	mov	ip, r0
3000b79e:	bc01      	pop	{r0}
3000b7a0:	4760      	bx	ip
3000b7a2:	bf00      	nop
3000b7a4:	000032bd 	.word	0x000032bd

3000b7a8 <__SBOOT_Validate_ImgHash_B_veneer>:
3000b7a8:	b401      	push	{r0}
3000b7aa:	4802      	ldr	r0, [pc, #8]	; (3000b7b4 <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000b7ac:	4684      	mov	ip, r0
3000b7ae:	bc01      	pop	{r0}
3000b7b0:	4760      	bx	ip
3000b7b2:	bf00      	nop
3000b7b4:	00003541 	.word	0x00003541

3000b7b8 <__SBOOT_Validate_PubKey_A_veneer>:
3000b7b8:	b401      	push	{r0}
3000b7ba:	4802      	ldr	r0, [pc, #8]	; (3000b7c4 <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000b7bc:	4684      	mov	ip, r0
3000b7be:	bc01      	pop	{r0}
3000b7c0:	4760      	bx	ip
3000b7c2:	bf00      	nop
3000b7c4:	00003321 	.word	0x00003321

3000b7c8 <__SBOOT_Validate_Algorithm_A_veneer>:
3000b7c8:	b401      	push	{r0}
3000b7ca:	4802      	ldr	r0, [pc, #8]	; (3000b7d4 <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000b7cc:	4684      	mov	ip, r0
3000b7ce:	bc01      	pop	{r0}
3000b7d0:	4760      	bx	ip
3000b7d2:	bf00      	nop
3000b7d4:	00003279 	.word	0x00003279

3000b7d8 <__io_assert_failed_veneer>:
3000b7d8:	b401      	push	{r0}
3000b7da:	4802      	ldr	r0, [pc, #8]	; (3000b7e4 <__io_assert_failed_veneer+0xc>)
3000b7dc:	4684      	mov	ip, r0
3000b7de:	bc01      	pop	{r0}
3000b7e0:	4760      	bx	ip
3000b7e2:	bf00      	nop
3000b7e4:	0000992d 	.word	0x0000992d

3000b7e8 <__rtl_crypto_sha2_final_B_veneer>:
3000b7e8:	b401      	push	{r0}
3000b7ea:	4802      	ldr	r0, [pc, #8]	; (3000b7f4 <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000b7ec:	4684      	mov	ip, r0
3000b7ee:	bc01      	pop	{r0}
3000b7f0:	4760      	bx	ip
3000b7f2:	bf00      	nop
3000b7f4:	000053f1 	.word	0x000053f1

3000b7f8 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000b7f8:	b401      	push	{r0}
3000b7fa:	4802      	ldr	r0, [pc, #8]	; (3000b804 <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000b7fc:	4684      	mov	ip, r0
3000b7fe:	bc01      	pop	{r0}
3000b800:	4760      	bx	ip
3000b802:	bf00      	nop
3000b804:	00005bb9 	.word	0x00005bb9

3000b808 <__SBOOT_Validate_ImgHash_A_veneer>:
3000b808:	b401      	push	{r0}
3000b80a:	4802      	ldr	r0, [pc, #8]	; (3000b814 <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000b80c:	4684      	mov	ip, r0
3000b80e:	bc01      	pop	{r0}
3000b810:	4760      	bx	ip
3000b812:	bf00      	nop
3000b814:	000033cd 	.word	0x000033cd

3000b818 <__SBOOT_GetMdType_B_veneer>:
3000b818:	b401      	push	{r0}
3000b81a:	4802      	ldr	r0, [pc, #8]	; (3000b824 <__SBOOT_GetMdType_B_veneer+0xc>)
3000b81c:	4684      	mov	ip, r0
3000b81e:	bc01      	pop	{r0}
3000b820:	4760      	bx	ip
3000b822:	bf00      	nop
3000b824:	0000319d 	.word	0x0000319d

3000b828 <__rtl_crypto_sha2_update_A_veneer>:
3000b828:	b401      	push	{r0}
3000b82a:	4802      	ldr	r0, [pc, #8]	; (3000b834 <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000b82c:	4684      	mov	ip, r0
3000b82e:	bc01      	pop	{r0}
3000b830:	4760      	bx	ip
3000b832:	bf00      	nop
3000b834:	00005211 	.word	0x00005211

3000b838 <__SBOOT_GetMdType_A_veneer>:
3000b838:	b401      	push	{r0}
3000b83a:	4802      	ldr	r0, [pc, #8]	; (3000b844 <__SBOOT_GetMdType_A_veneer+0xc>)
3000b83c:	4684      	mov	ip, r0
3000b83e:	bc01      	pop	{r0}
3000b840:	4760      	bx	ip
3000b842:	bf00      	nop
3000b844:	00003029 	.word	0x00003029

3000b848 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000b848:	b401      	push	{r0}
3000b84a:	4802      	ldr	r0, [pc, #8]	; (3000b854 <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000b84c:	4684      	mov	ip, r0
3000b84e:	bc01      	pop	{r0}
3000b850:	4760      	bx	ip
3000b852:	bf00      	nop
3000b854:	000059d9 	.word	0x000059d9

3000b858 <__SBOOT_Validate_Algorithm_B_veneer>:
3000b858:	b401      	push	{r0}
3000b85a:	4802      	ldr	r0, [pc, #8]	; (3000b864 <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000b85c:	4684      	mov	ip, r0
3000b85e:	bc01      	pop	{r0}
3000b860:	4760      	bx	ip
3000b862:	bf00      	nop
3000b864:	000033ed 	.word	0x000033ed

3000b868 <__CRYPTO_SHA_Init_A_veneer>:
3000b868:	b401      	push	{r0}
3000b86a:	4802      	ldr	r0, [pc, #8]	; (3000b874 <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000b86c:	4684      	mov	ip, r0
3000b86e:	bc01      	pop	{r0}
3000b870:	4760      	bx	ip
3000b872:	bf00      	nop
3000b874:	000022dd 	.word	0x000022dd

3000b878 <__SBOOT_Validate_Signature_B_veneer>:
3000b878:	b401      	push	{r0}
3000b87a:	4802      	ldr	r0, [pc, #8]	; (3000b884 <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000b87c:	4684      	mov	ip, r0
3000b87e:	bc01      	pop	{r0}
3000b880:	4760      	bx	ip
3000b882:	bf00      	nop
3000b884:	00003431 	.word	0x00003431
3000b888:	004f4931 	.word	0x004f4931
3000b88c:	00004f32 	.word	0x00004f32
3000b890:	004f4932 	.word	0x004f4932
3000b894:	00004f34 	.word	0x00004f34
3000b898:	004f4934 	.word	0x004f4934
3000b89c:	53414c46 	.word	0x53414c46
3000b8a0:	4b4c4348 	.word	0x4b4c4348
3000b8a4:	00000000 	.word	0x00000000
3000b8a8:	2d73255b 	.word	0x2d73255b
3000b8ac:	63205d49 	.word	0x63205d49
3000b8b0:	62696c61 	.word	0x62696c61
3000b8b4:	69746172 	.word	0x69746172
3000b8b8:	6f5f6e6f 	.word	0x6f5f6e6f
3000b8bc:	255b3a6b 	.word	0x255b3a6b
3000b8c0:	64253a64 	.word	0x64253a64
3000b8c4:	5d64253a 	.word	0x5d64253a
3000b8c8:	00000a20 	.word	0x00000a20
3000b8cc:	2d73255b 	.word	0x2d73255b
3000b8d0:	46205d49 	.word	0x46205d49
3000b8d4:	4853414c 	.word	0x4853414c
3000b8d8:	4c414320 	.word	0x4c414320
3000b8dc:	305b4249 	.word	0x305b4249
3000b8e0:	20782578 	.word	0x20782578
3000b8e4:	0a5d4b4f 	.word	0x0a5d4b4f
3000b8e8:	00000000 	.word	0x00000000
3000b8ec:	2d73255b 	.word	0x2d73255b
3000b8f0:	46205d45 	.word	0x46205d45
3000b8f4:	4853414c 	.word	0x4853414c
3000b8f8:	4c414320 	.word	0x4c414320
3000b8fc:	305b4249 	.word	0x305b4249
3000b900:	20782578 	.word	0x20782578
3000b904:	4c494146 	.word	0x4c494146
3000b908:	00000a5d 	.word	0x00000a5d
3000b90c:	2d73255b 	.word	0x2d73255b
3000b910:	46205d49 	.word	0x46205d49
3000b914:	6873616c 	.word	0x6873616c
3000b918:	61655220 	.word	0x61655220
3000b91c:	73252064 	.word	0x73252064
3000b920:	0000000a 	.word	0x0000000a
3000b924:	2d73255b 	.word	0x2d73255b
3000b928:	46205d45 	.word	0x46205d45
3000b92c:	6873616c 	.word	0x6873616c
3000b930:	69775320 	.word	0x69775320
3000b934:	20686374 	.word	0x20686374
3000b938:	64616552 	.word	0x64616552
3000b93c:	646f4d20 	.word	0x646f4d20
3000b940:	41462065 	.word	0x41462065
3000b944:	000a4c49 	.word	0x000a4c49
3000b948:	2d73255b 	.word	0x2d73255b
3000b94c:	46205d49 	.word	0x46205d49
3000b950:	6873616c 	.word	0x6873616c
3000b954:	3a444920 	.word	0x3a444920
3000b958:	2d782520 	.word	0x2d782520
3000b95c:	252d7825 	.word	0x252d7825
3000b960:	00000a78 	.word	0x00000a78
3000b964:	2d73255b 	.word	0x2d73255b
3000b968:	54205d57 	.word	0x54205d57
3000b96c:	20736968 	.word	0x20736968
3000b970:	73616c66 	.word	0x73616c66
3000b974:	79742068 	.word	0x79742068
3000b978:	69206570 	.word	0x69206570
3000b97c:	6f6e2073 	.word	0x6f6e2073
3000b980:	75732074 	.word	0x75732074
3000b984:	726f7070 	.word	0x726f7070
3000b988:	21646574 	.word	0x21646574
3000b98c:	0000000a 	.word	0x0000000a
3000b990:	2d73255b 	.word	0x2d73255b
3000b994:	4e205d49 	.word	0x4e205d49
3000b998:	20444e41 	.word	0x20444e41
3000b99c:	203a4449 	.word	0x203a4449
3000b9a0:	252d7825 	.word	0x252d7825
3000b9a4:	00000a78 	.word	0x00000a78
3000b9a8:	00003552 	.word	0x00003552
3000b9ac:	00003652 	.word	0x00003652
3000b9b0:	00003752 	.word	0x00003752
3000b9b4:	00003852 	.word	0x00003852
3000b9b8:	00003952 	.word	0x00003952
3000b9bc:	00303152 	.word	0x00303152
3000b9c0:	00313152 	.word	0x00313152
3000b9c4:	00003152 	.word	0x00003152
3000b9c8:	00003252 	.word	0x00003252
3000b9cc:	00003352 	.word	0x00003352
3000b9d0:	544f4f42 	.word	0x544f4f42
3000b9d4:	00000000 	.word	0x00000000
3000b9d8:	2d73255b 	.word	0x2d73255b
3000b9dc:	48205d41 	.word	0x48205d41
3000b9e0:	20647261 	.word	0x20647261
3000b9e4:	6c756146 	.word	0x6c756146
3000b9e8:	61502074 	.word	0x61502074
3000b9ec:	20686374 	.word	0x20686374
3000b9f0:	63655328 	.word	0x63655328
3000b9f4:	29657275 	.word	0x29657275
3000b9f8:	00000a0d 	.word	0x00000a0d
3000b9fc:	2d73255b 	.word	0x2d73255b
3000ba00:	45205d41 	.word	0x45205d41
3000ba04:	70656378 	.word	0x70656378
3000ba08:	6e6f6974 	.word	0x6e6f6974
3000ba0c:	6f726620 	.word	0x6f726620
3000ba10:	6573206d 	.word	0x6573206d
3000ba14:	65727563 	.word	0x65727563
3000ba18:	726f7720 	.word	0x726f7720
3000ba1c:	0d21646c 	.word	0x0d21646c
3000ba20:	0000000a 	.word	0x0000000a
3000ba24:	2d73255b 	.word	0x2d73255b
3000ba28:	45205d41 	.word	0x45205d41
3000ba2c:	70656378 	.word	0x70656378
3000ba30:	6e6f6974 	.word	0x6e6f6974
3000ba34:	6f726620 	.word	0x6f726620
3000ba38:	6f6e206d 	.word	0x6f6e206d
3000ba3c:	65732d6e 	.word	0x65732d6e
3000ba40:	65727563 	.word	0x65727563
3000ba44:	726f7720 	.word	0x726f7720
3000ba48:	0d21646c 	.word	0x0d21646c
3000ba4c:	0000000a 	.word	0x0000000a
3000ba50:	2d73255b 	.word	0x2d73255b
3000ba54:	53205d41 	.word	0x53205d41
3000ba58:	20525346 	.word	0x20525346
3000ba5c:	30203d20 	.word	0x30203d20
3000ba60:	38302578 	.word	0x38302578
3000ba64:	000a0d78 	.word	0x000a0d78
3000ba68:	2d73255b 	.word	0x2d73255b
3000ba6c:	53205d41 	.word	0x53205d41
3000ba70:	20524146 	.word	0x20524146
3000ba74:	30203d20 	.word	0x30203d20
3000ba78:	38302578 	.word	0x38302578
3000ba7c:	000a0d78 	.word	0x000a0d78
3000ba80:	2d73255b 	.word	0x2d73255b
3000ba84:	53205d41 	.word	0x53205d41
3000ba88:	52534348 	.word	0x52534348
3000ba8c:	30203d20 	.word	0x30203d20
3000ba90:	38302578 	.word	0x38302578
3000ba94:	000a0d78 	.word	0x000a0d78
3000ba98:	2d73255b 	.word	0x2d73255b
3000ba9c:	42205d41 	.word	0x42205d41
3000baa0:	5f50554b 	.word	0x5f50554b
3000baa4:	32474552 	.word	0x32474552
3000baa8:	25783020 	.word	0x25783020
3000baac:	0a0d2078 	.word	0x0a0d2078
3000bab0:	00000000 	.word	0x00000000
3000bab4:	3141544f 	.word	0x3141544f
3000bab8:	45535520 	.word	0x45535520
3000babc:	6576202c 	.word	0x6576202c
3000bac0:	6f697372 	.word	0x6f697372
3000bac4:	25203a6e 	.word	0x25203a6e
3000bac8:	00000a64 	.word	0x00000a64
3000bacc:	3241544f 	.word	0x3241544f
3000bad0:	45535520 	.word	0x45535520
3000bad4:	6576202c 	.word	0x6576202c
3000bad8:	6f697372 	.word	0x6f697372
3000badc:	25203a6e 	.word	0x25203a6e
3000bae0:	00000a64 	.word	0x00000a64
3000bae4:	68746f42 	.word	0x68746f42
3000bae8:	41544f20 	.word	0x41544f20
3000baec:	616d6920 	.word	0x616d6920
3000baf0:	76206567 	.word	0x76206567
3000baf4:	66697265 	.word	0x66697265
3000baf8:	61662079 	.word	0x61662079
3000bafc:	64656c69 	.word	0x64656c69
3000bb00:	0000000a 	.word	0x0000000a
3000bb04:	2d73255b 	.word	0x2d73255b
3000bb08:	49205d49 	.word	0x49205d49
3000bb0c:	2074696e 	.word	0x2074696e
3000bb10:	0a0d4257 	.word	0x0a0d4257
3000bb14:	00000000 	.word	0x00000000
3000bb18:	73616c66 	.word	0x73616c66
3000bb1c:	69735f68 	.word	0x69735f68
3000bb20:	203a657a 	.word	0x203a657a
3000bb24:	000a4d38 	.word	0x000a4d38
3000bb28:	73616c66 	.word	0x73616c66
3000bb2c:	69735f68 	.word	0x69735f68
3000bb30:	203a657a 	.word	0x203a657a
3000bb34:	0a4d3631 	.word	0x0a4d3631
3000bb38:	00000000 	.word	0x00000000
3000bb3c:	73616c66 	.word	0x73616c66
3000bb40:	69735f68 	.word	0x69735f68
3000bb44:	203a657a 	.word	0x203a657a
3000bb48:	0a4d3233 	.word	0x0a4d3233
3000bb4c:	00000000 	.word	0x00000000
3000bb50:	20315042 	.word	0x20315042
3000bb54:	73726576 	.word	0x73726576
3000bb58:	206e6f69 	.word	0x206e6f69
3000bb5c:	61766e69 	.word	0x61766e69
3000bb60:	0a64696c 	.word	0x0a64696c
3000bb64:	00000000 	.word	0x00000000
3000bb68:	20315042 	.word	0x20315042
3000bb6c:	6d726f66 	.word	0x6d726f66
3000bb70:	69207461 	.word	0x69207461
3000bb74:	6c61766e 	.word	0x6c61766e
3000bb78:	000a6469 	.word	0x000a6469
3000bb7c:	20315042 	.word	0x20315042
3000bb80:	69746361 	.word	0x69746361
3000bb84:	69206576 	.word	0x69206576
3000bb88:	7865646e 	.word	0x7865646e
3000bb8c:	766e6920 	.word	0x766e6920
3000bb90:	64696c61 	.word	0x64696c61
3000bb94:	0000000a 	.word	0x0000000a
3000bb98:	20315042 	.word	0x20315042
3000bb9c:	61746164 	.word	0x61746164
3000bba0:	6c617620 	.word	0x6c617620
3000bba4:	202c6469 	.word	0x202c6469
3000bba8:	73726576 	.word	0x73726576
3000bbac:	3a6e6f69 	.word	0x3a6e6f69
3000bbb0:	000a6425 	.word	0x000a6425
3000bbb4:	20325042 	.word	0x20325042
3000bbb8:	6d726f66 	.word	0x6d726f66
3000bbbc:	69207461 	.word	0x69207461
3000bbc0:	6c61766e 	.word	0x6c61766e
3000bbc4:	000a6469 	.word	0x000a6469
3000bbc8:	20315042 	.word	0x20315042
3000bbcc:	33435243 	.word	0x33435243
3000bbd0:	616d2032 	.word	0x616d2032
3000bbd4:	2c686374 	.word	0x2c686374
3000bbd8:	74746120 	.word	0x74746120
3000bbdc:	65686361 	.word	0x65686361
3000bbe0:	52432064 	.word	0x52432064
3000bbe4:	30203a43 	.word	0x30203a43
3000bbe8:	2c782578 	.word	0x2c782578
3000bbec:	6c616320 	.word	0x6c616320
3000bbf0:	616c7563 	.word	0x616c7563
3000bbf4:	20646574 	.word	0x20646574
3000bbf8:	3a435243 	.word	0x3a435243
3000bbfc:	25783020 	.word	0x25783020
3000bc00:	00000a78 	.word	0x00000a78
3000bc04:	20315042 	.word	0x20315042
3000bc08:	33435243 	.word	0x33435243
3000bc0c:	6f6e2032 	.word	0x6f6e2032
3000bc10:	616d2074 	.word	0x616d2074
3000bc14:	2c686374 	.word	0x2c686374
3000bc18:	74746120 	.word	0x74746120
3000bc1c:	65686361 	.word	0x65686361
3000bc20:	52432064 	.word	0x52432064
3000bc24:	30203a43 	.word	0x30203a43
3000bc28:	2c782578 	.word	0x2c782578
3000bc2c:	6c616320 	.word	0x6c616320
3000bc30:	616c7563 	.word	0x616c7563
3000bc34:	20646574 	.word	0x20646574
3000bc38:	3a435243 	.word	0x3a435243
3000bc3c:	25783020 	.word	0x25783020
3000bc40:	00000a78 	.word	0x00000a78
3000bc44:	20325042 	.word	0x20325042
3000bc48:	33435243 	.word	0x33435243
3000bc4c:	616d2032 	.word	0x616d2032
3000bc50:	2c686374 	.word	0x2c686374
3000bc54:	74746120 	.word	0x74746120
3000bc58:	65686361 	.word	0x65686361
3000bc5c:	52432064 	.word	0x52432064
3000bc60:	30203a43 	.word	0x30203a43
3000bc64:	2c782578 	.word	0x2c782578
3000bc68:	6c616320 	.word	0x6c616320
3000bc6c:	616c7563 	.word	0x616c7563
3000bc70:	20646574 	.word	0x20646574
3000bc74:	3a435243 	.word	0x3a435243
3000bc78:	25783020 	.word	0x25783020
3000bc7c:	00000a78 	.word	0x00000a78
3000bc80:	20325042 	.word	0x20325042
3000bc84:	33435243 	.word	0x33435243
3000bc88:	6f6e2032 	.word	0x6f6e2032
3000bc8c:	616d2074 	.word	0x616d2074
3000bc90:	2c686374 	.word	0x2c686374
3000bc94:	74746120 	.word	0x74746120
3000bc98:	65686361 	.word	0x65686361
3000bc9c:	52432064 	.word	0x52432064
3000bca0:	30203a43 	.word	0x30203a43
3000bca4:	2c782578 	.word	0x2c782578
3000bca8:	6c616320 	.word	0x6c616320
3000bcac:	616c7563 	.word	0x616c7563
3000bcb0:	20646574 	.word	0x20646574
3000bcb4:	3a435243 	.word	0x3a435243
3000bcb8:	25783020 	.word	0x25783020
3000bcbc:	00000a78 	.word	0x00000a78
3000bcc0:	20315042 	.word	0x20315042
3000bcc4:	3161746f 	.word	0x3161746f
3000bcc8:	64646120 	.word	0x64646120
3000bccc:	73736572 	.word	0x73736572
3000bcd0:	766e6920 	.word	0x766e6920
3000bcd4:	64696c61 	.word	0x64696c61
3000bcd8:	0000000a 	.word	0x0000000a
3000bcdc:	20315042 	.word	0x20315042
3000bce0:	3261746f 	.word	0x3261746f
3000bce4:	64646120 	.word	0x64646120
3000bce8:	73736572 	.word	0x73736572
3000bcec:	766e6920 	.word	0x766e6920
3000bcf0:	64696c61 	.word	0x64696c61
3000bcf4:	0000000a 	.word	0x0000000a
3000bcf8:	68746f42 	.word	0x68746f42
3000bcfc:	20504220 	.word	0x20504220
3000bd00:	20435243 	.word	0x20435243
3000bd04:	696c6176 	.word	0x696c6176
3000bd08:	00000a64 	.word	0x00000a64
3000bd0c:	20315042 	.word	0x20315042
3000bd10:	656c6573 	.word	0x656c6573
3000bd14:	64657463 	.word	0x64657463
3000bd18:	0000000a 	.word	0x0000000a
3000bd1c:	20325042 	.word	0x20325042
3000bd20:	656c6573 	.word	0x656c6573
3000bd24:	64657463 	.word	0x64657463
3000bd28:	0000000a 	.word	0x0000000a
3000bd2c:	68746f42 	.word	0x68746f42
3000bd30:	20504220 	.word	0x20504220
3000bd34:	61766e69 	.word	0x61766e69
3000bd38:	0a64696c 	.word	0x0a64696c
3000bd3c:	00000000 	.word	0x00000000
3000bd40:	6f636552 	.word	0x6f636552
3000bd44:	79726576 	.word	0x79726576
3000bd48:	61747320 	.word	0x61747320
3000bd4c:	000a7472 	.word	0x000a7472
3000bd50:	69746341 	.word	0x69746341
3000bd54:	49206576 	.word	0x49206576
3000bd58:	7865646e 	.word	0x7865646e
3000bd5c:	6425203a 	.word	0x6425203a
3000bd60:	0000000a 	.word	0x0000000a
3000bd64:	3141544f 	.word	0x3141544f
3000bd68:	766e6920 	.word	0x766e6920
3000bd6c:	64696c61 	.word	0x64696c61
3000bd70:	0000000a 	.word	0x0000000a
3000bd74:	3241544f 	.word	0x3241544f
3000bd78:	766e6920 	.word	0x766e6920
3000bd7c:	64696c61 	.word	0x64696c61
3000bd80:	0000000a 	.word	0x0000000a
3000bd84:	3141544f 	.word	0x3141544f
3000bd88:	43524320 	.word	0x43524320
3000bd8c:	6d203233 	.word	0x6d203233
3000bd90:	68637461 	.word	0x68637461
3000bd94:	7461202c 	.word	0x7461202c
3000bd98:	68636174 	.word	0x68636174
3000bd9c:	43206465 	.word	0x43206465
3000bda0:	203a4352 	.word	0x203a4352
3000bda4:	78257830 	.word	0x78257830
3000bda8:	6163202c 	.word	0x6163202c
3000bdac:	6c75636c 	.word	0x6c75636c
3000bdb0:	64657461 	.word	0x64657461
3000bdb4:	43524320 	.word	0x43524320
3000bdb8:	7830203a 	.word	0x7830203a
3000bdbc:	000a7825 	.word	0x000a7825
3000bdc0:	3141544f 	.word	0x3141544f
3000bdc4:	43524320 	.word	0x43524320
3000bdc8:	6e203233 	.word	0x6e203233
3000bdcc:	6d20746f 	.word	0x6d20746f
3000bdd0:	68637461 	.word	0x68637461
3000bdd4:	7461202c 	.word	0x7461202c
3000bdd8:	68636174 	.word	0x68636174
3000bddc:	43206465 	.word	0x43206465
3000bde0:	203a4352 	.word	0x203a4352
3000bde4:	78257830 	.word	0x78257830
3000bde8:	6163202c 	.word	0x6163202c
3000bdec:	6c75636c 	.word	0x6c75636c
3000bdf0:	64657461 	.word	0x64657461
3000bdf4:	43524320 	.word	0x43524320
3000bdf8:	7830203a 	.word	0x7830203a
3000bdfc:	202c7825 	.word	0x202c7825
3000be00:	73617265 	.word	0x73617265
3000be04:	544f2065 	.word	0x544f2065
3000be08:	000a3141 	.word	0x000a3141
3000be0c:	3241544f 	.word	0x3241544f
3000be10:	43524320 	.word	0x43524320
3000be14:	6d203233 	.word	0x6d203233
3000be18:	68637461 	.word	0x68637461
3000be1c:	7461202c 	.word	0x7461202c
3000be20:	68636174 	.word	0x68636174
3000be24:	43206465 	.word	0x43206465
3000be28:	203a4352 	.word	0x203a4352
3000be2c:	78257830 	.word	0x78257830
3000be30:	6163202c 	.word	0x6163202c
3000be34:	6c75636c 	.word	0x6c75636c
3000be38:	64657461 	.word	0x64657461
3000be3c:	43524320 	.word	0x43524320
3000be40:	7830203a 	.word	0x7830203a
3000be44:	000a7825 	.word	0x000a7825
3000be48:	3241544f 	.word	0x3241544f
3000be4c:	43524320 	.word	0x43524320
3000be50:	6e203233 	.word	0x6e203233
3000be54:	6d20746f 	.word	0x6d20746f
3000be58:	68637461 	.word	0x68637461
3000be5c:	7461202c 	.word	0x7461202c
3000be60:	68636174 	.word	0x68636174
3000be64:	43206465 	.word	0x43206465
3000be68:	203a4352 	.word	0x203a4352
3000be6c:	78257830 	.word	0x78257830
3000be70:	6163202c 	.word	0x6163202c
3000be74:	6c75636c 	.word	0x6c75636c
3000be78:	64657461 	.word	0x64657461
3000be7c:	43524320 	.word	0x43524320
3000be80:	7830203a 	.word	0x7830203a
3000be84:	202c7825 	.word	0x202c7825
3000be88:	73617265 	.word	0x73617265
3000be8c:	544f2065 	.word	0x544f2065
3000be90:	000a3241 	.word	0x000a3241
3000be94:	69746341 	.word	0x69746341
3000be98:	69206576 	.word	0x69206576
3000be9c:	7865646e 	.word	0x7865646e
3000bea0:	41544f20 	.word	0x41544f20
3000bea4:	766e6920 	.word	0x766e6920
3000bea8:	64696c61 	.word	0x64696c61
3000beac:	6572202c 	.word	0x6572202c
3000beb0:	74697277 	.word	0x74697277
3000beb4:	63612065 	.word	0x63612065
3000beb8:	65766974 	.word	0x65766974
3000bebc:	646e6920 	.word	0x646e6920
3000bec0:	74207865 	.word	0x74207865
3000bec4:	6425206f 	.word	0x6425206f
3000bec8:	0000000a 	.word	0x0000000a
3000becc:	206c6c41 	.word	0x206c6c41
3000bed0:	2041544f 	.word	0x2041544f
3000bed4:	72646461 	.word	0x72646461
3000bed8:	65737365 	.word	0x65737365
3000bedc:	6f6e2073 	.word	0x6f6e2073
3000bee0:	61762074 	.word	0x61762074
3000bee4:	2c64696c 	.word	0x2c64696c
3000bee8:	63657220 	.word	0x63657220
3000beec:	7265766f 	.word	0x7265766f
3000bef0:	61662079 	.word	0x61662079
3000bef4:	000a6c69 	.word	0x000a6c69
3000bef8:	20325042 	.word	0x20325042
3000befc:	3261746f 	.word	0x3261746f
3000bf00:	64646120 	.word	0x64646120
3000bf04:	73736572 	.word	0x73736572
3000bf08:	766e6920 	.word	0x766e6920
3000bf0c:	64696c61 	.word	0x64696c61
3000bf10:	0000000a 	.word	0x0000000a
3000bf14:	20325042 	.word	0x20325042
3000bf18:	73726576 	.word	0x73726576
3000bf1c:	206e6f69 	.word	0x206e6f69
3000bf20:	61766e69 	.word	0x61766e69
3000bf24:	0a64696c 	.word	0x0a64696c
3000bf28:	00000000 	.word	0x00000000
3000bf2c:	20325042 	.word	0x20325042
3000bf30:	69746361 	.word	0x69746361
3000bf34:	69206576 	.word	0x69206576
3000bf38:	7865646e 	.word	0x7865646e
3000bf3c:	766e6920 	.word	0x766e6920
3000bf40:	64696c61 	.word	0x64696c61
3000bf44:	0000000a 	.word	0x0000000a
3000bf48:	20325042 	.word	0x20325042
3000bf4c:	3161746f 	.word	0x3161746f
3000bf50:	64646120 	.word	0x64646120
3000bf54:	73736572 	.word	0x73736572
3000bf58:	766e6920 	.word	0x766e6920
3000bf5c:	64696c61 	.word	0x64696c61
3000bf60:	0000000a 	.word	0x0000000a
3000bf64:	20325042 	.word	0x20325042
3000bf68:	61746164 	.word	0x61746164
3000bf6c:	6c617620 	.word	0x6c617620
3000bf70:	202c6469 	.word	0x202c6469
3000bf74:	73726576 	.word	0x73726576
3000bf78:	3a6e6f69 	.word	0x3a6e6f69
3000bf7c:	000a6425 	.word	0x000a6425
3000bf80:	2d73255b 	.word	0x2d73255b
3000bf84:	44205d49 	.word	0x44205d49
3000bf88:	62617369 	.word	0x62617369
3000bf8c:	4b20656c 	.word	0x4b20656c
3000bf90:	4220304d 	.word	0x4220304d
3000bf94:	20746f6f 	.word	0x20746f6f
3000bf98:	206e6920 	.word	0x206e6920
3000bf9c:	74736554 	.word	0x74736554
3000bfa0:	646f6d20 	.word	0x646f6d20
3000bfa4:	000a0d65 	.word	0x000a0d65
3000bfa8:	2d73255b 	.word	0x2d73255b
3000bfac:	42205d49 	.word	0x42205d49
3000bfb0:	73617079 	.word	0x73617079
3000bfb4:	544f2073 	.word	0x544f2073
3000bfb8:	75612050 	.word	0x75612050
3000bfbc:	6f6c6f74 	.word	0x6f6c6f74
3000bfc0:	0a0d6461 	.word	0x0a0d6461
3000bfc4:	00000000 	.word	0x00000000
3000bfc8:	2d73255b 	.word	0x2d73255b
3000bfcc:	4e205d49 	.word	0x4e205d49
3000bfd0:	72462050 	.word	0x72462050
3000bfd4:	25207165 	.word	0x25207165
3000bfd8:	484d2064 	.word	0x484d2064
3000bfdc:	00000a7a 	.word	0x00000a7a
3000bfe0:	2d73255b 	.word	0x2d73255b
3000bfe4:	41205d49 	.word	0x41205d49
3000bfe8:	72462050 	.word	0x72462050
3000bfec:	25207165 	.word	0x25207165
3000bff0:	484d2064 	.word	0x484d2064
3000bff4:	00000a7a 	.word	0x00000a7a
3000bff8:	31474d49 	.word	0x31474d49
3000bffc:	4f425320 	.word	0x4f425320
3000c000:	4f20544f 	.word	0x4f20544f
3000c004:	000a4646 	.word	0x000a4646
3000c008:	31474d49 	.word	0x31474d49
3000c00c:	4f425320 	.word	0x4f425320
3000c010:	5320544f 	.word	0x5320544f
3000c014:	45434355 	.word	0x45434355
3000c018:	000a5353 	.word	0x000a5353
3000c01c:	746f6f62 	.word	0x746f6f62
3000c020:	64616f6c 	.word	0x64616f6c
3000c024:	765f7265 	.word	0x765f7265
3000c028:	69737265 	.word	0x69737265
3000c02c:	25206e6f 	.word	0x25206e6f
3000c030:	00000a73 	.word	0x00000a73
3000c034:	444f4d5b 	.word	0x444f4d5b
3000c038:	5f454c55 	.word	0x5f454c55
3000c03c:	544f4f42 	.word	0x544f4f42
3000c040:	56454c2d 	.word	0x56454c2d
3000c044:	495f4c45 	.word	0x495f4c45
3000c048:	5d4f464e 	.word	0x5d4f464e
3000c04c:	474d493a 	.word	0x474d493a
3000c050:	4e452031 	.word	0x4e452031
3000c054:	20524554 	.word	0x20524554
3000c058:	3a50534d 	.word	0x3a50534d
3000c05c:	3830255b 	.word	0x3830255b
3000c060:	000a5d78 	.word	0x000a5d78
3000c064:	444f4d5b 	.word	0x444f4d5b
3000c068:	5f454c55 	.word	0x5f454c55
3000c06c:	544f4f42 	.word	0x544f4f42
3000c070:	56454c2d 	.word	0x56454c2d
3000c074:	495f4c45 	.word	0x495f4c45
3000c078:	5d4f464e 	.word	0x5d4f464e
3000c07c:	474d493a 	.word	0x474d493a
3000c080:	45532031 	.word	0x45532031
3000c084:	45525543 	.word	0x45525543
3000c088:	41545320 	.word	0x41545320
3000c08c:	203a4554 	.word	0x203a4554
3000c090:	000a6425 	.word	0x000a6425
3000c094:	2d73255b 	.word	0x2d73255b
3000c098:	49205d49 	.word	0x49205d49
3000c09c:	2074696e 	.word	0x2074696e
3000c0a0:	41525350 	.word	0x41525350
3000c0a4:	000a0d4d 	.word	0x000a0d4d
3000c0a8:	2d73255b 	.word	0x2d73255b
3000c0ac:	73205d57 	.word	0x73205d57
3000c0b0:	70207465 	.word	0x70207465
3000c0b4:	66206d66 	.word	0x66206d66
3000c0b8:	0d6c6961 	.word	0x0d6c6961
3000c0bc:	0000000a 	.word	0x0000000a
3000c0c0:	2d73255b 	.word	0x2d73255b
3000c0c4:	45205d45 	.word	0x45205d45
3000c0c8:	524f5252 	.word	0x524f5252
3000c0cc:	53202121 	.word	0x53202121
3000c0d0:	6c756f68 	.word	0x6c756f68
3000c0d4:	6f4e2064 	.word	0x6f4e2064
3000c0d8:	6e652074 	.word	0x6e652074
3000c0dc:	656c6261 	.word	0x656c6261
3000c0e0:	6d654d20 	.word	0x6d654d20
3000c0e4:	5f727753 	.word	0x5f727753
3000c0e8:	796c6e4f 	.word	0x796c6e4f
3000c0ec:	206e6920 	.word	0x206e6920
3000c0f0:	20524444 	.word	0x20524444
3000c0f4:	70696843 	.word	0x70696843
3000c0f8:	0d212121 	.word	0x0d212121
3000c0fc:	0000000a 	.word	0x0000000a
3000c100:	2d73255b 	.word	0x2d73255b
3000c104:	49205d49 	.word	0x49205d49
3000c108:	2074696e 	.word	0x2074696e
3000c10c:	0d524444 	.word	0x0d524444
3000c110:	0000000a 	.word	0x0000000a
3000c114:	2d73255b 	.word	0x2d73255b
3000c118:	42205d49 	.word	0x42205d49
3000c11c:	5f50554b 	.word	0x5f50554b
3000c120:	32474552 	.word	0x32474552
3000c124:	25783020 	.word	0x25783020
3000c128:	0a0d2078 	.word	0x0a0d2078
3000c12c:	00000000 	.word	0x00000000
3000c130:	55676f4c 	.word	0x55676f4c
3000c134:	20747261 	.word	0x20747261
3000c138:	64756142 	.word	0x64756142
3000c13c:	65746172 	.word	0x65746172
3000c140:	6425203a 	.word	0x6425203a
3000c144:	0000000a 	.word	0x0000000a
3000c148:	726f4e0a 	.word	0x726f4e0a
3000c14c:	206c616d 	.word	0x206c616d
3000c150:	746f6f62 	.word	0x746f6f62
3000c154:	0000000a 	.word	0x0000000a
3000c158:	20304d4b 	.word	0x20304d4b
3000c15c:	20504958 	.word	0x20504958
3000c160:	00474d49 	.word	0x00474d49
3000c164:	20304d4b 	.word	0x20304d4b
3000c168:	4d415253 	.word	0x4d415253
3000c16c:	00000000 	.word	0x00000000
3000c170:	20304d4b 	.word	0x20304d4b
3000c174:	4d415244 	.word	0x4d415244
3000c178:	00000000 	.word	0x00000000
3000c17c:	20344d4b 	.word	0x20344d4b
3000c180:	20504958 	.word	0x20504958
3000c184:	00474d49 	.word	0x00474d49
3000c188:	20344d4b 	.word	0x20344d4b
3000c18c:	4d415253 	.word	0x4d415253
3000c190:	00000000 	.word	0x00000000
3000c194:	20344d4b 	.word	0x20344d4b
3000c198:	4d415244 	.word	0x4d415244
3000c19c:	00000000 	.word	0x00000000
3000c1a0:	58205041 	.word	0x58205041
3000c1a4:	49205049 	.word	0x49205049
3000c1a8:	0000474d 	.word	0x0000474d
3000c1ac:	42205041 	.word	0x42205041
3000c1b0:	5320314c 	.word	0x5320314c
3000c1b4:	004d4152 	.word	0x004d4152
3000c1b8:	42205041 	.word	0x42205041
3000c1bc:	4420314c 	.word	0x4420314c
3000c1c0:	004d4152 	.word	0x004d4152
3000c1c4:	46205041 	.word	0x46205041
3000c1c8:	00005049 	.word	0x00005049
3000c1cc:	2d73255b 	.word	0x2d73255b
3000c1d0:	25205d49 	.word	0x25205d49
3000c1d4:	6e492073 	.word	0x6e492073
3000c1d8:	696c6176 	.word	0x696c6176
3000c1dc:	00000a64 	.word	0x00000a64
3000c1e0:	2d73255b 	.word	0x2d73255b
3000c1e4:	25205d49 	.word	0x25205d49
3000c1e8:	30255b73 	.word	0x30255b73
3000c1ec:	253a7838 	.word	0x253a7838
3000c1f0:	000a5d78 	.word	0x000a5d78
3000c1f4:	2d73255b 	.word	0x2d73255b
3000c1f8:	52205d49 	.word	0x52205d49
3000c1fc:	45205044 	.word	0x45205044
3000c200:	00000a4e 	.word	0x00000a4e
3000c204:	2d73255b 	.word	0x2d73255b
3000c208:	46205d49 	.word	0x46205d49
3000c20c:	206c6961 	.word	0x206c6961
3000c210:	6c206f74 	.word	0x6c206f74
3000c214:	2064616f 	.word	0x2064616f
3000c218:	20504452 	.word	0x20504452
3000c21c:	67616d69 	.word	0x67616d69
3000c220:	000a2165 	.word	0x000a2165
3000c224:	2d73255b 	.word	0x2d73255b
3000c228:	52205d49 	.word	0x52205d49
3000c22c:	4f205044 	.word	0x4f205044
3000c230:	000a4646 	.word	0x000a4646
3000c234:	2d73255b 	.word	0x2d73255b
3000c238:	49205d49 	.word	0x49205d49
3000c23c:	2032474d 	.word	0x2032474d
3000c240:	2046544f 	.word	0x2046544f
3000c244:	000a4e45 	.word	0x000a4e45
3000c248:	2d73255b 	.word	0x2d73255b
3000c24c:	4f205d45 	.word	0x4f205d45
3000c250:	4d204654 	.word	0x4d204654
3000c254:	2065646f 	.word	0x2065646f
3000c258:	6f727265 	.word	0x6f727265
3000c25c:	000a0d72 	.word	0x000a0d72
3000c260:	494c4156 	.word	0x494c4156
3000c264:	00000044 	.word	0x00000044
3000c268:	41564e49 	.word	0x41564e49
3000c26c:	0044494c 	.word	0x0044494c
3000c270:	74726563 	.word	0x74726563
3000c274:	63696669 	.word	0x63696669
3000c278:	20657461 	.word	0x20657461
3000c27c:	20434345 	.word	0x20434345
3000c280:	69726576 	.word	0x69726576
3000c284:	66207966 	.word	0x66207966
3000c288:	2c6c6961 	.word	0x2c6c6961
3000c28c:	6c6f7220 	.word	0x6c6f7220
3000c290:	6361626c 	.word	0x6361626c
3000c294:	000a0d6b 	.word	0x000a0d6b
3000c298:	444f4d5b 	.word	0x444f4d5b
3000c29c:	5f454c55 	.word	0x5f454c55
3000c2a0:	544f4f42 	.word	0x544f4f42
3000c2a4:	56454c2d 	.word	0x56454c2d
3000c2a8:	495f4c45 	.word	0x495f4c45
3000c2ac:	5d4f464e 	.word	0x5d4f464e
3000c2b0:	474d493a 	.word	0x474d493a
3000c2b4:	544f2832 	.word	0x544f2832
3000c2b8:	29642541 	.word	0x29642541
3000c2bc:	2c732520 	.word	0x2c732520
3000c2c0:	74657220 	.word	0x74657220
3000c2c4:	6425203a 	.word	0x6425203a
3000c2c8:	0000000a 	.word	0x0000000a
3000c2cc:	32474d49 	.word	0x32474d49
3000c2d0:	43434520 	.word	0x43434520
3000c2d4:	72657620 	.word	0x72657620
3000c2d8:	20796669 	.word	0x20796669
3000c2dc:	6c696166 	.word	0x6c696166
3000c2e0:	6f72202c 	.word	0x6f72202c
3000c2e4:	61626c6c 	.word	0x61626c6c
3000c2e8:	0a0d6b63 	.word	0x0a0d6b63
3000c2ec:	00000000 	.word	0x00000000
3000c2f0:	5f41544f 	.word	0x5f41544f
3000c2f4:	45205041 	.word	0x45205041
3000c2f8:	76204343 	.word	0x76204343
3000c2fc:	66697265 	.word	0x66697265
3000c300:	61662079 	.word	0x61662079
3000c304:	202c6c69 	.word	0x202c6c69
3000c308:	6c6c6f72 	.word	0x6c6c6f72
3000c30c:	6b636162 	.word	0x6b636162
3000c310:	00000a0d 	.word	0x00000a0d
3000c314:	64616f4c 	.word	0x64616f4c
3000c318:	41544f20 	.word	0x41544f20
3000c31c:	2050415f 	.word	0x2050415f
3000c320:	67616d69 	.word	0x67616d69
3000c324:	61662065 	.word	0x61662065
3000c328:	202c6c69 	.word	0x202c6c69
3000c32c:	6c6c6f72 	.word	0x6c6c6f72
3000c330:	6b636162 	.word	0x6b636162
3000c334:	00000a0d 	.word	0x00000a0d
3000c338:	444f4d5b 	.word	0x444f4d5b
3000c33c:	5f454c55 	.word	0x5f454c55
3000c340:	544f4f42 	.word	0x544f4f42
3000c344:	56454c2d 	.word	0x56454c2d
3000c348:	455f4c45 	.word	0x455f4c45
3000c34c:	524f5252 	.word	0x524f5252
3000c350:	61463a5d 	.word	0x61463a5d
3000c354:	74206c69 	.word	0x74206c69
3000c358:	6f6c206f 	.word	0x6f6c206f
3000c35c:	41206461 	.word	0x41206461
3000c360:	6d692050 	.word	0x6d692050
3000c364:	21656761 	.word	0x21656761
3000c368:	0000000a 	.word	0x0000000a
3000c36c:	20344d4b 	.word	0x20344d4b
3000c370:	33474d49 	.word	0x33474d49
3000c374:	00000000 	.word	0x00000000
3000c378:	20344d4b 	.word	0x20344d4b
3000c37c:	0043534e 	.word	0x0043534e
3000c380:	31474d49 	.word	0x31474d49
3000c384:	00000000 	.word	0x00000000
3000c388:	32474d49 	.word	0x32474d49
3000c38c:	00000000 	.word	0x00000000
3000c390:	42205041 	.word	0x42205041
3000c394:	4626314c 	.word	0x4626314c
3000c398:	00005049 	.word	0x00005049
3000c39c:	444f4d5b 	.word	0x444f4d5b
3000c3a0:	5f454c55 	.word	0x5f454c55
3000c3a4:	544f4f42 	.word	0x544f4f42
3000c3a8:	56454c2d 	.word	0x56454c2d
3000c3ac:	495f4c45 	.word	0x495f4c45
3000c3b0:	5d4f464e 	.word	0x5d4f464e
3000c3b4:	5b73253a 	.word	0x5b73253a
3000c3b8:	78383025 	.word	0x78383025
3000c3bc:	5d78253a 	.word	0x5d78253a
3000c3c0:	0000000a 	.word	0x0000000a
3000c3c4:	20504452 	.word	0x20504452
3000c3c8:	4c494146 	.word	0x4c494146
3000c3cc:	0000000a 	.word	0x0000000a
3000c3d0:	20504452 	.word	0x20504452
3000c3d4:	43435553 	.word	0x43435553
3000c3d8:	0a535345 	.word	0x0a535345
3000c3dc:	00000000 	.word	0x00000000
3000c3e0:	059e4eb3 	.word	0x059e4eb3
3000c3e4:	4b11ea39 	.word	0x4b11ea39
3000c3e8:	2a1b920a 	.word	0x2a1b920a
3000c3ec:	29939cee 	.word	0x29939cee
3000c3f0:	00000000 	.word	0x00000000
3000c3f4:	42205041 	.word	0x42205041
3000c3f8:	0032334c 	.word	0x0032334c
3000c3fc:	2d73255b 	.word	0x2d73255b
3000c400:	25205d49 	.word	0x25205d49
3000c404:	45562073 	.word	0x45562073
3000c408:	59464952 	.word	0x59464952
3000c40c:	53415020 	.word	0x53415020
3000c410:	00000a53 	.word	0x00000a53
3000c414:	2d73255b 	.word	0x2d73255b
3000c418:	25205d45 	.word	0x25205d45
3000c41c:	45562073 	.word	0x45562073
3000c420:	59464952 	.word	0x59464952
3000c424:	49414620 	.word	0x49414620
3000c428:	72202c4c 	.word	0x72202c4c
3000c42c:	3d207465 	.word	0x3d207465
3000c430:	0a642520 	.word	0x0a642520
3000c434:	00000000 	.word	0x00000000
3000c438:	2d73255b 	.word	0x2d73255b
3000c43c:	43205d45 	.word	0x43205d45
3000c440:	69747265 	.word	0x69747265
3000c444:	61636966 	.word	0x61636966
3000c448:	56206574 	.word	0x56206574
3000c44c:	46495245 	.word	0x46495245
3000c450:	41462059 	.word	0x41462059
3000c454:	202c4c49 	.word	0x202c4c49
3000c458:	20746572 	.word	0x20746572
3000c45c:	6425203d 	.word	0x6425203d
3000c460:	0000000a 	.word	0x0000000a
3000c464:	50494843 	.word	0x50494843
3000c468:	4f464e49 	.word	0x4f464e49
3000c46c:	00000000 	.word	0x00000000
3000c470:	2d73255b 	.word	0x2d73255b
3000c474:	49205d45 	.word	0x49205d45
3000c478:	6c61766e 	.word	0x6c61766e
3000c47c:	43206469 	.word	0x43206469
3000c480:	696e6968 	.word	0x696e6968
3000c484:	216f666e 	.word	0x216f666e
3000c488:	65684320 	.word	0x65684320
3000c48c:	4f206b63 	.word	0x4f206b63
3000c490:	000a5054 	.word	0x000a5054
3000c494:	2d73255b 	.word	0x2d73255b
3000c498:	50205d57 	.word	0x50205d57
3000c49c:	7361656c 	.word	0x7361656c
3000c4a0:	72702065 	.word	0x72702065
3000c4a4:	6172676f 	.word	0x6172676f
3000c4a8:	6863206d 	.word	0x6863206d
3000c4ac:	6e697069 	.word	0x6e697069
3000c4b0:	69206f66 	.word	0x69206f66
3000c4b4:	544f206e 	.word	0x544f206e
3000c4b8:	0a212050 	.word	0x0a212050
3000c4bc:	00000000 	.word	0x00000000

3000c4c0 <Memory_Info>:
3000c4c0:	03e00007 00060004 010202c0 02010100     ................
3000c4d0:	02000102 00040302 02040400 01010102     ................
3000c4e0:	01020221 02240401 03000102 00030103     !.....$.........
3000c4f0:	01050500 06000002 01020206 01070700     ................
3000c500:	08000002 01020208 02280801 09000102     ..........(.....
3000c510:	00040309 03290901 0a000004 0003010a     ......).........
3000c520:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000c530:	0d000102 0004030d 010e0e00 ffff0003     ................
3000c540:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000c550:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000c560:	62696c61 6f696172 000a0d6e              alibraion...

3000c56c <ddrphy_tx_scan>:
3000c56c:	00000000 00080000 00080808 00000000     ................
3000c57c:	20000000 33333333 000c0022 00000000     ... 3333".......
3000c58c:	07060012 000f0707 080f0f0f 00000008     ................
3000c59c:	20000001 22222222 000c0012 34000000     ... """".......4
3000c5ac:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000c5bc:	200003c1 22222222 000c0012 34000000     ... """".......4
3000c5cc:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000c5dc:	200003c1 22222222 000c0012 34000000     ... """".......4
3000c5ec:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000c5fc:	200003c1 22222222 000c0012 34000000     ... """".......4
3000c60c:	0e0e001a 00170e0e 12171717 00000012     ................
3000c61c:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000c62c:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000c63c:	200003c1 22222222 000c0012 34000000     ... """".......4
3000c64c:	0f0f001b 00180f0f 13181818 00000013     ................
3000c65c:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000c66c:	1110001b 00191111 13191919 00000013     ................
3000c67c:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000c68c:	1313001c 001c1313 121c1c1c 00000012     ................
3000c69c:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000c6ac <ddrphy_zq_rx_scan>:
	...
3000c6b4:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000c6c4:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000c6d4:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000c6e4:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000c6f4:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000c704:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000c714:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000c724:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000c734:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000c744:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000c754:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000c764:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000c774 <Flash_AVL>:
3000c774:	000000ef 000000ff 00000000 000043fc     .............C..
3000c784:	00000000 000000a1 000000ff 00000000     ................
3000c794:	0000fffc 00000000 0000000b 000000ff     ................
3000c7a4:	00000000 000043fc 00000000 0000000e     .....C..........
3000c7b4:	000000ff 00000000 000043fc 00000000     .........C......
3000c7c4:	000000c8 000000ff 00000001 000043fc     .............C..
3000c7d4:	00000000 000028c2 0000ffff 00000005     .....(..........
3000c7e4:	000200fc 00000000 000000c2 000000ff     ................
3000c7f4:	00000002 000000fc 00000000 00000068     ............h...
3000c804:	000000ff 00000002 000000fc 00000000     ................
3000c814:	00000051 000000ff 00000002 000000fc     Q...............
3000c824:	00000000 0000001c 000000ff 00000003     ................
3000c834:	000000fc 00000000 00000020 000000ff     ........ .......
3000c844:	00000004 000000fc 00000000 00000000     ................
3000c854:	000000ff 000000fe ffffffff 300079ad     .............y.0
3000c864:	000000ff ffffffff 000000ff ffffffff     ................
3000c874:	00000000                                ....

3000c878 <Flash_ReadMode>:
3000c878:	                                         ..

3000c87a <Flash_Speed>:
3000c87a:	                                         ..

3000c87c <NAND_AVL>:
3000c87c:	000000ef 000000ff 00000000 ffffffff     ................
3000c88c:	00000000 000000c8 000000ff 00000001     ................
3000c89c:	ffffffff 00000000 000000e5 000000ff     ................
3000c8ac:	00000001 ffffffff 00000000 000000c2     ................
3000c8bc:	000000ff 00000002 ffffffff 00000000     ................
3000c8cc:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000c8e4:	000000ff 000000fe ffffffff 30007a0d     .............z.0
3000c8f4:	000000ff ffffffff 000000ff ffffffff     ................
3000c904:	00000000 3000b888 3000b88c 3000b890     .......0...0...0
3000c914:	3000b894 3000b898                       ...0...0

3000c91c <__FUNCTION__.0>:
3000c91c:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000c92c <__FUNCTION__.1>:
3000c92c:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000c93c:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000c94c:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000c95c:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000c96c:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000c97c:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000c98c:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000c99c:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000c9ac:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000c9bc:	00000a20                                 ...

3000c9c0 <__FUNCTION__.1>:
3000c9c0:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000c9d0:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000c9e0:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000c9f0:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000ca00:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000ca10:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000ca20:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000ca30:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000ca40:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000ca50:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000ca60:	726f6620 41485320 00000a32               for SHA2...

3000ca6c <__FUNCTION__.1>:
3000ca6c:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000ca7c <__FUNCTION__.3>:
3000ca7c:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000ca8c:	32616873 696e695f 00000074              sha2_init...

3000ca98 <__FUNCTION__.4>:
3000ca98:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000caa8:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000cab8:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000cac8:	6e207369 6320746f 69666e6f 65727567     is not configure
3000cad8:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000cae8:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000caf8:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000cb08:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000cb18:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000cb28:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000cb38:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000cb48:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000cb58:	69625f6b 21217374 00000a21              k_bits!!!...

3000cb64 <__FUNCTION__.0>:
3000cb64:	33697872 445f3631 72536e79 6e695f65     rxi316_DynSre_in
3000cb74:	00007469                                it..

3000cb78 <__FUNCTION__.1>:
3000cb78:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000cb88:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000cb98:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000cba8:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000cbb8:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000cbc8:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000cbd8:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000cbe8:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000cbf8:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000cc08:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000cc18:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000cc28:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000cc38:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000cc48:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000cc58:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000cc68:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000cc78:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000cc88:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000cc98:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000cca8:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000ccb8:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000ccc8:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000ccd8:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000cce8:	00000000                                ....

3000ccec <register_string>:
3000ccec:	00000000 3000cc14 3000b9a8 3000b9ac     .......0...0...0
3000ccfc:	3000b9b0 3000b9b4 3000b9b8 3000b9bc     ...0...0...0...0
3000cd0c:	3000b9c0 3000cc10 3000b9c4 3000b9c8     ...0...0...0...0
3000cd1c:	3000b9cc 3000ccb4 65637845 6f697470     ...0...0Exceptio
3000cd2c:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000cd3c:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000cd4c:	64657375 20796220 6c696166 76206465     used by failed v
3000cd5c:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000cd6c:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000cd7c:	00000000 75636553 79746972 75616620     ....Security fau
3000cd8c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000cd9c:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000cdac:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000cdbc:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000cdcc:	79746972 75616620 6920746c 61632073     rity fault is ca
3000cddc:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000cdec:	65746e69 74697267 69732079 74616e67     integrity signat
3000cdfc:	00657275 75636553 79746972 75616620     ure.Security fau
3000ce0c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000ce1c:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000ce2c:	6572206e 6e727574 00000000 75636553     n return....Secu
3000ce3c:	79746972 75616620 6920746c 61632073     rity fault is ca
3000ce4c:	64657375 20796220 72747441 74756269     used by Attribut
3000ce5c:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000ce6c:	00006e6f 75636553 79746972 75616620     on..Security fau
3000ce7c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000ce8c:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000ce9c:	00006e6f 75636553 79746972 75616620     on..Security fau
3000ceac:	6920746c 61632073 64657375 20796220     lt is caused by 
3000cebc:	797a614c 61747320 70206574 65736572     Lazy state prese
3000cecc:	74617672 206e6f69 6f727265 00000072     rvation error...
3000cedc:	75636553 79746972 75616620 6920746c     Security fault i
3000ceec:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000cefc:	61747320 65206574 726f7272 00000000      state error....
3000cf0c:	20656854 75636573 79746972 75616620     The security fau
3000cf1c:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000cf2c:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000cf3c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000cf4c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000cf5c:	61662074 20746c75 63207369 65737561     t fault is cause
3000cf6c:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000cf7c:	63636120 20737365 6c6f6976 6f697461      access violatio
3000cf8c:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000cf9c:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000cfac:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000cfbc:	20746c75 63207369 65737561 79622064     ult is caused by
3000cfcc:	74616420 63612061 73736563 6f697620      data access vio
3000cfdc:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000cfec:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000cffc:	63207369 65737561 79622064 736e7520     is caused by uns
3000d00c:	6b636174 20676e69 6f727265 00000072     tacking error...
3000d01c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000d02c:	61662074 20746c75 63207369 65737561     t fault is cause
3000d03c:	79622064 61747320 6e696b63 72652067     d by stacking er
3000d04c:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000d05c:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000d06c:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000d07c:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000d08c:	70206574 65736572 74617672 006e6f69     te preservation.
3000d09c:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000d0ac:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000d0bc:	64657272 64646120 73736572 00736920     rred address is.
3000d0cc:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000d0dc:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000d0ec:	73692074 75616320 20646573 69207962     t is caused by i
3000d0fc:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000d10c:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000d11c:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000d12c:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000d13c:	75616320 20646573 70207962 69636572      caused by preci
3000d14c:	64206573 20617461 65636361 76207373     se data access v
3000d15c:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000d16c:	6c756166 73692074 75616320 20646573     fault is caused 
3000d17c:	69207962 6572706d 65736963 74616420     by imprecise dat
3000d18c:	63612061 73736563 6f697620 6974616c     a access violati
3000d19c:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000d1ac:	75616320 20646573 75207962 6174736e      caused by unsta
3000d1bc:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000d1cc:	6c756166 73692074 75616320 20646573     fault is caused 
3000d1dc:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000d1ec:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000d1fc:	75616320 20646573 66207962 74616f6c      caused by float
3000d20c:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000d21c:	65746174 65727020 76726573 6f697461     tate preservatio
3000d22c:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000d23c:	636f2074 72727563 61206465 65726464     t occurred addre
3000d24c:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000d25c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d26c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000d27c:	65737561 79622064 74746120 74706d65     aused by attempt
3000d28c:	6f742073 65786520 65747563 206e6120     s to execute an 
3000d29c:	65646e75 656e6966 6e692064 75727473     undefined instru
3000d2ac:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000d2bc:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000d2cc:	67617355 61662065 20746c75 63207369     Usage fault is c
3000d2dc:	65737561 79622064 74746120 74706d65     aused by attempt
3000d2ec:	6f742073 69777320 20686374 61206f74     s to switch to a
3000d2fc:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000d30c:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000d31c:	61662065 20746c75 63207369 65737561     e fault is cause
3000d32c:	79622064 74746120 74706d65 6f742073     d by attempts to
3000d33c:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000d34c:	74697720 20612068 20646162 756c6176      with a bad valu
3000d35c:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000d36c:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000d37c:	61662065 20746c75 63207369 65737561     e fault is cause
3000d38c:	79622064 74746120 74706d65 6f742073     d by attempts to
3000d39c:	65786520 65747563 63206120 6f72706f      execute a copro
3000d3ac:	73736563 6920726f 7274736e 69746375     cessor instructi
3000d3bc:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000d3cc:	63207369 65737561 79622064 646e6920     is caused by ind
3000d3dc:	74616369 74207365 20746168 74732061     icates that a st
3000d3ec:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000d3fc:	61776472 63206572 6b636568 61682029     rdware check) ha
3000d40c:	61742073 206e656b 63616c70 00000065     s taken place...
3000d41c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000d42c:	65737561 79622064 646e6920 74616369     aused by indicat
3000d43c:	74207365 20746168 75206e61 696c616e     es that an unali
3000d44c:	64656e67 63636120 20737365 6c756166     gned access faul
3000d45c:	61682074 61742073 206e656b 63616c70     t has taken plac
3000d46c:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000d47c:	63207369 65737561 79622064 646e4920     is caused by Ind
3000d48c:	74616369 61207365 76696420 20656469     icates a divide 
3000d49c:	7a207962 206f7265 20736168 656b6174     by zero has take
3000d4ac:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000d4bc:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000d4cc:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000d4dc:	75626544 61662067 20746c75 63207369     Debug fault is c
3000d4ec:	65737561 79622064 6c616820 65722074     aused by halt re
3000d4fc:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000d50c:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000d51c:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000d52c:	20746c75 63207369 65737561 79622064     ult is caused by
3000d53c:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000d54c:	7865206e 74756365 00006465 75626544     n executed..Debu
3000d55c:	61662067 20746c75 63207369 65737561     g fault is cause
3000d56c:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000d57c:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000d58c:	20746c75 63207369 65737561 79622064     ult is caused by
3000d59c:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000d5ac:	72727563 00006465 75626544 61662067     curred..Debug fa
3000d5bc:	20746c75 63207369 65737561 79622064     ult is caused by
3000d5cc:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000d5dc:	72657373 00646574 00000208 0000020c     sserted.........
3000d5ec:	00000210 00000008 00000214 00000218     ................
3000d5fc:	0000021c 0000000c                       ........

3000d604 <__FUNCTION__.0>:
3000d604:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.
3000d614:	3000c158 3000c164 3000c170 3000c17c     X..0d..0p..0|..0
3000d624:	3000c188 3000c194 3000c1a0 3000c1ac     ...0...0...0...0
3000d634:	3000c1b8 3000c1c4 3000c36c 3000c378     ...0...0l..0x..0
3000d644:	3000c380 3000c388 3000c370 3000c390     ...0...0p..0...0

3000d654 <km4_bootloader_rev>:
3000d654:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000d664:	655f7265 66303235 36623035 30325f65     er_e520f50b6e_20
3000d674:	312f3532 34312f30 3a36312d 343a3332     25/10/14-16:23:4
3000d684:	4f420035 5b00544f 572d7325 2a2a205d     5.BOOT.[%s-W] **
3000d694:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000d6a4:	2a2a2a2a 2a2a2a2a 52415720 474e494e     ******** WARNING
3000d6b4:	2a2a2a20 2a2a2a2a 2a2a2a2a 2a2a2a2a      ***************
3000d6c4:	2a2a2a2a 2a2a2a2a 255b000a 5d572d73     ********..[%s-W]
3000d6d4:	20202a20 20202020 20202020 20202020      *              
3000d6e4:	20202020 20202020 20202020 20202020                     
3000d6f4:	20202020 20202020 20202020 20202020                     
3000d704:	20202020 20202020 0a2a2020 73255b00               *..[%s
3000d714:	205d572d 2020202a 20202020 20202020     -W] *           
3000d724:	75432020 6e657272 68632074 76207069       Current chip v
3000d734:	69737265 69206e6f 43412073 21217475     ersion is ACut!!
3000d744:	20202021 20202020 20202020 000a2a20     !            *..
3000d754:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000d764:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000d774:	2074726f 72727563 20746e65 73726576     ort current vers
3000d784:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000d794:	5b000a2a 572d7325 202a205d 20202020     *..[%s-W] *     
3000d7a4:	656c5020 20657361 746e6f63 20746361      Please contact 
3000d7b4:	204b5452 66204d50 6d20726f 2065726f     RTK PM for more 
3000d7c4:	646f7270 20746375 6f666e69 20202021     product info!   
3000d7d4:	2a202020 255b000a 5d572d73 2a2a2a20        *..[%s-W] ***
3000d7e4:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000d7f4:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000d804:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000d814:	2a2a2a2a 0a2a2a2a 73255b00 205d572d     *******..[%s-W] 
3000d824:	0043000a 49534f50 002e0058              ..C.POSIX...

3000d830 <_ctype_>:
3000d830:	20202000 20202020 28282020 20282828     .         ((((( 
3000d840:	20202020 20202020 20202020 20202020                     
3000d850:	10108820 10101010 10101010 10101010      ...............
3000d860:	04040410 04040404 10040404 10101010     ................
3000d870:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000d880:	01010101 01010101 01010101 10101010     ................
3000d890:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000d8a0:	02020202 02020202 02020202 10101010     ................
3000d8b0:	00000020 00000000 00000000 00000000      ...............
	...
