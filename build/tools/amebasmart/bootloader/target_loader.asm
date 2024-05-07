
/home/edo/sambashare/misc_sdk/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


Disassembly of section .ram_image1.text:

30003050 <flash_get_chip_info>:
30003050:	4b0a      	ldr	r3, [pc, #40]	; (3000307c <flash_get_chip_info+0x2c>)
30003052:	b510      	push	{r4, lr}
30003054:	4604      	mov	r4, r0
30003056:	4798      	blx	r3
30003058:	4b09      	ldr	r3, [pc, #36]	; (30003080 <flash_get_chip_info+0x30>)
3000305a:	4a0a      	ldr	r2, [pc, #40]	; (30003084 <flash_get_chip_info+0x34>)
3000305c:	2800      	cmp	r0, #0
3000305e:	bf08      	it	eq
30003060:	4613      	moveq	r3, r2
30003062:	7a1a      	ldrb	r2, [r3, #8]
30003064:	4618      	mov	r0, r3
30003066:	2aff      	cmp	r2, #255	; 0xff
30003068:	d101      	bne.n	3000306e <flash_get_chip_info+0x1e>
3000306a:	2000      	movs	r0, #0
3000306c:	e005      	b.n	3000307a <flash_get_chip_info+0x2a>
3000306e:	6842      	ldr	r2, [r0, #4]
30003070:	f853 1b14 	ldr.w	r1, [r3], #20
30003074:	4022      	ands	r2, r4
30003076:	4291      	cmp	r1, r2
30003078:	d1f3      	bne.n	30003062 <flash_get_chip_info+0x12>
3000307a:	bd10      	pop	{r4, pc}
3000307c:	30008ff5 	.word	0x30008ff5
30003080:	3000b754 	.word	0x3000b754
30003084:	3000b85c 	.word	0x3000b85c

30003088 <FLASH_ClockDiv>:
30003088:	2200      	movs	r2, #0
3000308a:	2180      	movs	r1, #128	; 0x80
3000308c:	b538      	push	{r3, r4, r5, lr}
3000308e:	4604      	mov	r4, r0
30003090:	4d1d      	ldr	r5, [pc, #116]	; (30003108 <FLASH_ClockDiv+0x80>)
30003092:	4610      	mov	r0, r2
30003094:	47a8      	blx	r5
30003096:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000309a:	0224      	lsls	r4, r4, #8
3000309c:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000309e:	f404 6470 	and.w	r4, r4, #3840	; 0xf00
300030a2:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
300030a6:	641a      	str	r2, [r3, #64]	; 0x40
300030a8:	6c1a      	ldr	r2, [r3, #64]	; 0x40
300030aa:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
300030ae:	4314      	orrs	r4, r2
300030b0:	4a16      	ldr	r2, [pc, #88]	; (3000310c <FLASH_ClockDiv+0x84>)
300030b2:	641c      	str	r4, [r3, #64]	; 0x40
300030b4:	230a      	movs	r3, #10
300030b6:	462c      	mov	r4, r5
300030b8:	6c11      	ldr	r1, [r2, #64]	; 0x40
300030ba:	2900      	cmp	r1, #0
300030bc:	db01      	blt.n	300030c2 <FLASH_ClockDiv+0x3a>
300030be:	3b01      	subs	r3, #1
300030c0:	d1fa      	bne.n	300030b8 <FLASH_ClockDiv+0x30>
300030c2:	230a      	movs	r3, #10
300030c4:	4a11      	ldr	r2, [pc, #68]	; (3000310c <FLASH_ClockDiv+0x84>)
300030c6:	6c11      	ldr	r1, [r2, #64]	; 0x40
300030c8:	0089      	lsls	r1, r1, #2
300030ca:	d401      	bmi.n	300030d0 <FLASH_ClockDiv+0x48>
300030cc:	3b01      	subs	r3, #1
300030ce:	d1fa      	bne.n	300030c6 <FLASH_ClockDiv+0x3e>
300030d0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300030d4:	6c1a      	ldr	r2, [r3, #64]	; 0x40
300030d6:	f442 5200 	orr.w	r2, r2, #8192	; 0x2000
300030da:	641a      	str	r2, [r3, #64]	; 0x40
300030dc:	6c1a      	ldr	r2, [r3, #64]	; 0x40
300030de:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
300030e2:	641a      	str	r2, [r3, #64]	; 0x40
300030e4:	220a      	movs	r2, #10
300030e6:	6c19      	ldr	r1, [r3, #64]	; 0x40
300030e8:	f001 0103 	and.w	r1, r1, #3
300030ec:	2903      	cmp	r1, #3
300030ee:	d001      	beq.n	300030f4 <FLASH_ClockDiv+0x6c>
300030f0:	3a01      	subs	r2, #1
300030f2:	d1f8      	bne.n	300030e6 <FLASH_ClockDiv+0x5e>
300030f4:	2201      	movs	r2, #1
300030f6:	2180      	movs	r1, #128	; 0x80
300030f8:	f44f 7080 	mov.w	r0, #256	; 0x100
300030fc:	47a0      	blx	r4
300030fe:	2003      	movs	r0, #3
30003100:	4b03      	ldr	r3, [pc, #12]	; (30003110 <FLASH_ClockDiv+0x88>)
30003102:	4798      	blx	r3
30003104:	2001      	movs	r0, #1
30003106:	bd38      	pop	{r3, r4, r5, pc}
30003108:	0000b479 	.word	0x0000b479
3000310c:	42008800 	.word	0x42008800
30003110:	0000b569 	.word	0x0000b569

30003114 <FLASH_CalibrationPhaseIdx>:
30003114:	2200      	movs	r2, #0
30003116:	2180      	movs	r1, #128	; 0x80
30003118:	b538      	push	{r3, r4, r5, lr}
3000311a:	4604      	mov	r4, r0
3000311c:	4d19      	ldr	r5, [pc, #100]	; (30003184 <FLASH_CalibrationPhaseIdx+0x70>)
3000311e:	4610      	mov	r0, r2
30003120:	47a8      	blx	r5
30003122:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30003126:	6c1a      	ldr	r2, [r3, #64]	; 0x40
30003128:	f422 5280 	bic.w	r2, r2, #4096	; 0x1000
3000312c:	641a      	str	r2, [r3, #64]	; 0x40
3000312e:	6c1a      	ldr	r2, [r3, #64]	; 0x40
30003130:	f422 5200 	bic.w	r2, r2, #8192	; 0x2000
30003134:	641a      	str	r2, [r3, #64]	; 0x40
30003136:	220a      	movs	r2, #10
30003138:	6c19      	ldr	r1, [r3, #64]	; 0x40
3000313a:	0789      	lsls	r1, r1, #30
3000313c:	d001      	beq.n	30003142 <FLASH_CalibrationPhaseIdx+0x2e>
3000313e:	3a01      	subs	r2, #1
30003140:	d1fa      	bne.n	30003138 <FLASH_CalibrationPhaseIdx+0x24>
30003142:	6c1a      	ldr	r2, [r3, #64]	; 0x40
30003144:	00a4      	lsls	r4, r4, #2
30003146:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
3000314a:	f022 027c 	bic.w	r2, r2, #124	; 0x7c
3000314e:	f004 047c 	and.w	r4, r4, #124	; 0x7c
30003152:	4314      	orrs	r4, r2
30003154:	641c      	str	r4, [r3, #64]	; 0x40
30003156:	6c1a      	ldr	r2, [r3, #64]	; 0x40
30003158:	f442 5200 	orr.w	r2, r2, #8192	; 0x2000
3000315c:	641a      	str	r2, [r3, #64]	; 0x40
3000315e:	6c1a      	ldr	r2, [r3, #64]	; 0x40
30003160:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
30003164:	641a      	str	r2, [r3, #64]	; 0x40
30003166:	230a      	movs	r3, #10
30003168:	6c0a      	ldr	r2, [r1, #64]	; 0x40
3000316a:	f002 0203 	and.w	r2, r2, #3
3000316e:	2a03      	cmp	r2, #3
30003170:	d001      	beq.n	30003176 <FLASH_CalibrationPhaseIdx+0x62>
30003172:	3b01      	subs	r3, #1
30003174:	d1f8      	bne.n	30003168 <FLASH_CalibrationPhaseIdx+0x54>
30003176:	2201      	movs	r2, #1
30003178:	2180      	movs	r1, #128	; 0x80
3000317a:	2000      	movs	r0, #0
3000317c:	47a8      	blx	r5
3000317e:	2001      	movs	r0, #1
30003180:	bd38      	pop	{r3, r4, r5, pc}
30003182:	bf00      	nop
30003184:	0000b479 	.word	0x0000b479

30003188 <FLASH_CalibrationNewCmd>:
30003188:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000318c:	2801      	cmp	r0, #1
3000318e:	f04f 0001 	mov.w	r0, #1
30003192:	6c13      	ldr	r3, [r2, #64]	; 0x40
30003194:	bf0c      	ite	eq
30003196:	f443 4300 	orreq.w	r3, r3, #32768	; 0x8000
3000319a:	f423 4300 	bicne.w	r3, r3, #32768	; 0x8000
3000319e:	6413      	str	r3, [r2, #64]	; 0x40
300031a0:	4770      	bx	lr

300031a2 <FLASH_CalibrationNew>:
300031a2:	0052      	lsls	r2, r2, #1
300031a4:	3201      	adds	r2, #1
300031a6:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300031aa:	4d41      	ldr	r5, [pc, #260]	; (300032b0 <FLASH_CalibrationNew+0x10e>)
300031ac:	b087      	sub	sp, #28
300031ae:	4698      	mov	r8, r3
300031b0:	b2d3      	uxtb	r3, r2
300031b2:	9103      	str	r1, [sp, #12]
300031b4:	f89d 4044 	ldrb.w	r4, [sp, #68]	; 0x44
300031b8:	9001      	str	r0, [sp, #4]
300031ba:	9300      	str	r3, [sp, #0]
300031bc:	47a8      	blx	r5
300031be:	9903      	ldr	r1, [sp, #12]
300031c0:	9502      	str	r5, [sp, #8]
300031c2:	2800      	cmp	r0, #0
300031c4:	d04d      	beq.n	30003262 <FLASH_CalibrationNew+0xc0>
300031c6:	483b      	ldr	r0, [pc, #236]	; (300032b4 <FLASH_CalibrationNew+0x112>)
300031c8:	4b3b      	ldr	r3, [pc, #236]	; (300032b8 <FLASH_CalibrationNew+0x116>)
300031ca:	f8df 90e8 	ldr.w	r9, [pc, #232]	; 300032b4 <FLASH_CalibrationNew+0x112>
300031ce:	4798      	blx	r3
300031d0:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
300031d4:	f04f 36ff 	mov.w	r6, #4294967295	; 0xffffffff
300031d8:	f899 3005 	ldrb.w	r3, [r9, #5]
300031dc:	2000      	movs	r0, #0
300031de:	2500      	movs	r5, #0
300031e0:	4637      	mov	r7, r6
300031e2:	6153      	str	r3, [r2, #20]
300031e4:	f89d 3040 	ldrb.w	r3, [sp, #64]	; 0x40
300031e8:	f8d2 111c 	ldr.w	r1, [r2, #284]	; 0x11c
300031ec:	031b      	lsls	r3, r3, #12
300031ee:	f8df a0d8 	ldr.w	sl, [pc, #216]	; 300032c8 <FLASH_CalibrationNew+0x126>
300031f2:	f421 4170 	bic.w	r1, r1, #61440	; 0xf000
300031f6:	f8df b0d4 	ldr.w	fp, [pc, #212]	; 300032cc <FLASH_CalibrationNew+0x12a>
300031fa:	b29b      	uxth	r3, r3
300031fc:	430b      	orrs	r3, r1
300031fe:	f8c2 311c 	str.w	r3, [r2, #284]	; 0x11c
30003202:	f7ff ffc1 	bl	30003188 <FLASH_CalibrationNewCmd>
30003206:	2001      	movs	r0, #1
30003208:	f7ff ffbe 	bl	30003188 <FLASH_CalibrationNewCmd>
3000320c:	9b00      	ldr	r3, [sp, #0]
3000320e:	42a3      	cmp	r3, r4
30003210:	d348      	bcc.n	300032a4 <FLASH_CalibrationNew+0x102>
30003212:	4620      	mov	r0, r4
30003214:	f7ff ff7e 	bl	30003114 <FLASH_CalibrationPhaseIdx>
30003218:	f8da 3014 	ldr.w	r3, [sl, #20]
3000321c:	03da      	lsls	r2, r3, #15
3000321e:	d509      	bpl.n	30003234 <FLASH_CalibrationNew+0x92>
30003220:	f3bf 8f4f 	dsb	sy
30003224:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
30003228:	f8ca 325c 	str.w	r3, [sl, #604]	; 0x25c
3000322c:	f3bf 8f4f 	dsb	sy
30003230:	f3bf 8f6f 	isb	sy
30003234:	4b21      	ldr	r3, [pc, #132]	; (300032bc <FLASH_CalibrationNew+0x11a>)
30003236:	4798      	blx	r3
30003238:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
3000323c:	4659      	mov	r1, fp
3000323e:	a804      	add	r0, sp, #16
30003240:	681a      	ldr	r2, [r3, #0]
30003242:	9204      	str	r2, [sp, #16]
30003244:	2208      	movs	r2, #8
30003246:	685b      	ldr	r3, [r3, #4]
30003248:	9305      	str	r3, [sp, #20]
3000324a:	4b1d      	ldr	r3, [pc, #116]	; (300032c0 <FLASH_CalibrationNew+0x11e>)
3000324c:	4798      	blx	r3
3000324e:	b958      	cbnz	r0, 30003268 <FLASH_CalibrationNew+0xc6>
30003250:	1c7b      	adds	r3, r7, #1
30003252:	4626      	mov	r6, r4
30003254:	bf12      	itee	ne
30003256:	4445      	addne	r5, r8
30003258:	4627      	moveq	r7, r4
3000325a:	2501      	moveq	r5, #1
3000325c:	4444      	add	r4, r8
3000325e:	b2e4      	uxtb	r4, r4
30003260:	e7d4      	b.n	3000320c <FLASH_CalibrationNew+0x6a>
30003262:	4814      	ldr	r0, [pc, #80]	; (300032b4 <FLASH_CalibrationNew+0x112>)
30003264:	4b17      	ldr	r3, [pc, #92]	; (300032c4 <FLASH_CalibrationNew+0x122>)
30003266:	e7b0      	b.n	300031ca <FLASH_CalibrationNew+0x28>
30003268:	2d00      	cmp	r5, #0
3000326a:	ddf7      	ble.n	3000325c <FLASH_CalibrationNew+0xba>
3000326c:	eb07 0365 	add.w	r3, r7, r5, asr #1
30003270:	9a01      	ldr	r2, [sp, #4]
30003272:	f882 3061 	strb.w	r3, [r2, #97]	; 0x61
30003276:	2000      	movs	r0, #0
30003278:	f7ff ff86 	bl	30003188 <FLASH_CalibrationNewCmd>
3000327c:	f899 2006 	ldrb.w	r2, [r9, #6]
30003280:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30003284:	615a      	str	r2, [r3, #20]
30003286:	9b02      	ldr	r3, [sp, #8]
30003288:	4798      	blx	r3
3000328a:	f899 1004 	ldrb.w	r1, [r9, #4]
3000328e:	b160      	cbz	r0, 300032aa <FLASH_CalibrationNew+0x108>
30003290:	4808      	ldr	r0, [pc, #32]	; (300032b4 <FLASH_CalibrationNew+0x112>)
30003292:	4b09      	ldr	r3, [pc, #36]	; (300032b8 <FLASH_CalibrationNew+0x116>)
30003294:	4798      	blx	r3
30003296:	0630      	lsls	r0, r6, #24
30003298:	ea40 4007 	orr.w	r0, r0, r7, lsl #16
3000329c:	4328      	orrs	r0, r5
3000329e:	b007      	add	sp, #28
300032a0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300032a4:	2d00      	cmp	r5, #0
300032a6:	dde6      	ble.n	30003276 <FLASH_CalibrationNew+0xd4>
300032a8:	e7e0      	b.n	3000326c <FLASH_CalibrationNew+0xca>
300032aa:	4802      	ldr	r0, [pc, #8]	; (300032b4 <FLASH_CalibrationNew+0x112>)
300032ac:	4b05      	ldr	r3, [pc, #20]	; (300032c4 <FLASH_CalibrationNew+0x122>)
300032ae:	e7f1      	b.n	30003294 <FLASH_CalibrationNew+0xf2>
300032b0:	30008ff5 	.word	0x30008ff5
300032b4:	2001c01c 	.word	0x2001c01c
300032b8:	00009c99 	.word	0x00009c99
300032bc:	30008e0d 	.word	0x30008e0d
300032c0:	000129bd 	.word	0x000129bd
300032c4:	0000b7e9 	.word	0x0000b7e9
300032c8:	e000ed00 	.word	0xe000ed00
300032cc:	00054060 	.word	0x00054060

300032d0 <FLASH_Calibration>:
300032d0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300032d4:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300032d8:	b085      	sub	sp, #20
300032da:	4693      	mov	fp, r2
300032dc:	7944      	ldrb	r4, [r0, #5]
300032de:	9103      	str	r1, [sp, #12]
300032e0:	4680      	mov	r8, r0
300032e2:	695a      	ldr	r2, [r3, #20]
300032e4:	2000      	movs	r0, #0
300032e6:	4f33      	ldr	r7, [pc, #204]	; (300033b4 <FLASH_Calibration+0xe4>)
300032e8:	9201      	str	r2, [sp, #4]
300032ea:	f8d3 311c 	ldr.w	r3, [r3, #284]	; 0x11c
300032ee:	9302      	str	r3, [sp, #8]
300032f0:	f7ff ff4a 	bl	30003188 <FLASH_CalibrationNewCmd>
300032f4:	47b8      	blx	r7
300032f6:	9903      	ldr	r1, [sp, #12]
300032f8:	b350      	cbz	r0, 30003350 <FLASH_Calibration+0x80>
300032fa:	482f      	ldr	r0, [pc, #188]	; (300033b8 <FLASH_Calibration+0xe8>)
300032fc:	4b2f      	ldr	r3, [pc, #188]	; (300033bc <FLASH_Calibration+0xec>)
300032fe:	4798      	blx	r3
30003300:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30003304:	eb0b 0b44 	add.w	fp, fp, r4, lsl #1
30003308:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
3000330c:	615c      	str	r4, [r3, #20]
3000330e:	2400      	movs	r4, #0
30003310:	f8df a0b8 	ldr.w	sl, [pc, #184]	; 300033cc <FLASH_Calibration+0xfc>
30003314:	4626      	mov	r6, r4
30003316:	46a1      	mov	r9, r4
30003318:	4929      	ldr	r1, [pc, #164]	; (300033c0 <FLASH_Calibration+0xf0>)
3000331a:	45b3      	cmp	fp, r6
3000331c:	d11b      	bne.n	30003356 <FLASH_Calibration+0x86>
3000331e:	2c00      	cmp	r4, #0
30003320:	bfc4      	itt	gt
30003322:	eb05 0464 	addgt.w	r4, r5, r4, asr #1
30003326:	f888 401d 	strbgt.w	r4, [r8, #29]
3000332a:	47b8      	blx	r7
3000332c:	4b22      	ldr	r3, [pc, #136]	; (300033b8 <FLASH_Calibration+0xe8>)
3000332e:	7919      	ldrb	r1, [r3, #4]
30003330:	2800      	cmp	r0, #0
30003332:	d03c      	beq.n	300033ae <FLASH_Calibration+0xde>
30003334:	4618      	mov	r0, r3
30003336:	4b21      	ldr	r3, [pc, #132]	; (300033bc <FLASH_Calibration+0xec>)
30003338:	4798      	blx	r3
3000333a:	9a01      	ldr	r2, [sp, #4]
3000333c:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30003340:	4648      	mov	r0, r9
30003342:	615a      	str	r2, [r3, #20]
30003344:	9a02      	ldr	r2, [sp, #8]
30003346:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
3000334a:	b005      	add	sp, #20
3000334c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003350:	4819      	ldr	r0, [pc, #100]	; (300033b8 <FLASH_Calibration+0xe8>)
30003352:	4b1c      	ldr	r3, [pc, #112]	; (300033c4 <FLASH_Calibration+0xf4>)
30003354:	e7d3      	b.n	300032fe <FLASH_Calibration+0x2e>
30003356:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
3000335a:	0333      	lsls	r3, r6, #12
3000335c:	f8de 211c 	ldr.w	r2, [lr, #284]	; 0x11c
30003360:	b29b      	uxth	r3, r3
30003362:	f422 4c70 	bic.w	ip, r2, #61440	; 0xf000
30003366:	ea43 030c 	orr.w	r3, r3, ip
3000336a:	f8ce 311c 	str.w	r3, [lr, #284]	; 0x11c
3000336e:	f8da 3014 	ldr.w	r3, [sl, #20]
30003372:	03da      	lsls	r2, r3, #15
30003374:	d509      	bpl.n	3000338a <FLASH_Calibration+0xba>
30003376:	f3bf 8f4f 	dsb	sy
3000337a:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
3000337e:	f8ca 325c 	str.w	r3, [sl, #604]	; 0x25c
30003382:	f3bf 8f4f 	dsb	sy
30003386:	f3bf 8f6f 	isb	sy
3000338a:	4788      	blx	r1
3000338c:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
30003390:	480d      	ldr	r0, [pc, #52]	; (300033c8 <FLASH_Calibration+0xf8>)
30003392:	681b      	ldr	r3, [r3, #0]
30003394:	6802      	ldr	r2, [r0, #0]
30003396:	490a      	ldr	r1, [pc, #40]	; (300033c0 <FLASH_Calibration+0xf0>)
30003398:	429a      	cmp	r2, r3
3000339a:	d106      	bne.n	300033aa <FLASH_Calibration+0xda>
3000339c:	1c6b      	adds	r3, r5, #1
3000339e:	f104 0401 	add.w	r4, r4, #1
300033a2:	f04f 0901 	mov.w	r9, #1
300033a6:	bf08      	it	eq
300033a8:	4635      	moveq	r5, r6
300033aa:	3601      	adds	r6, #1
300033ac:	e7b5      	b.n	3000331a <FLASH_Calibration+0x4a>
300033ae:	4618      	mov	r0, r3
300033b0:	4b04      	ldr	r3, [pc, #16]	; (300033c4 <FLASH_Calibration+0xf4>)
300033b2:	e7c1      	b.n	30003338 <FLASH_Calibration+0x68>
300033b4:	30008ff5 	.word	0x30008ff5
300033b8:	2001c01c 	.word	0x2001c01c
300033bc:	00009c99 	.word	0x00009c99
300033c0:	30008e0d 	.word	0x30008e0d
300033c4:	0000b7e9 	.word	0x0000b7e9
300033c8:	00054060 	.word	0x00054060
300033cc:	e000ed00 	.word	0xe000ed00

300033d0 <_flash_calibration_highspeed>:
300033d0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300033d4:	2400      	movs	r4, #0
300033d6:	b087      	sub	sp, #28
300033d8:	4681      	mov	r9, r0
300033da:	460a      	mov	r2, r1
300033dc:	46a0      	mov	r8, r4
300033de:	4626      	mov	r6, r4
300033e0:	4627      	mov	r7, r4
300033e2:	4d1f      	ldr	r5, [pc, #124]	; (30003460 <_flash_calibration_highspeed+0x90>)
300033e4:	f04f 0b01 	mov.w	fp, #1
300033e8:	46a2      	mov	sl, r4
300033ea:	b2fb      	uxtb	r3, r7
300033ec:	4649      	mov	r1, r9
300033ee:	481c      	ldr	r0, [pc, #112]	; (30003460 <_flash_calibration_highspeed+0x90>)
300033f0:	9300      	str	r3, [sp, #0]
300033f2:	2301      	movs	r3, #1
300033f4:	f8cd b004 	str.w	fp, [sp, #4]
300033f8:	9205      	str	r2, [sp, #20]
300033fa:	f7ff fed2 	bl	300031a2 <FLASH_CalibrationNew>
300033fe:	f895 3061 	ldrb.w	r3, [r5, #97]	; 0x61
30003402:	b2c0      	uxtb	r0, r0
30003404:	9a05      	ldr	r2, [sp, #20]
30003406:	b12b      	cbz	r3, 30003414 <_flash_calibration_highspeed+0x44>
30003408:	2802      	cmp	r0, #2
3000340a:	bf84      	itt	hi
3000340c:	f043 0301 	orrhi.w	r3, r3, #1
30003410:	f885 3061 	strbhi.w	r3, [r5, #97]	; 0x61
30003414:	42a0      	cmp	r0, r4
30003416:	d90c      	bls.n	30003432 <_flash_calibration_highspeed+0x62>
30003418:	f895 8061 	ldrb.w	r8, [r5, #97]	; 0x61
3000341c:	463e      	mov	r6, r7
3000341e:	4604      	mov	r4, r0
30003420:	3701      	adds	r7, #1
30003422:	f885 a061 	strb.w	sl, [r5, #97]	; 0x61
30003426:	2f05      	cmp	r7, #5
30003428:	d1df      	bne.n	300033ea <_flash_calibration_highspeed+0x1a>
3000342a:	b934      	cbnz	r4, 3000343a <_flash_calibration_highspeed+0x6a>
3000342c:	4620      	mov	r0, r4
3000342e:	776c      	strb	r4, [r5, #29]
30003430:	e012      	b.n	30003458 <_flash_calibration_highspeed+0x88>
30003432:	2c00      	cmp	r4, #0
30003434:	d0f4      	beq.n	30003420 <_flash_calibration_highspeed+0x50>
30003436:	2800      	cmp	r0, #0
30003438:	d1f2      	bne.n	30003420 <_flash_calibration_highspeed+0x50>
3000343a:	4b0a      	ldr	r3, [pc, #40]	; (30003464 <_flash_calibration_highspeed+0x94>)
3000343c:	2004      	movs	r0, #4
3000343e:	9600      	str	r6, [sp, #0]
30003440:	b2f6      	uxtb	r6, r6
30003442:	4a09      	ldr	r2, [pc, #36]	; (30003468 <_flash_calibration_highspeed+0x98>)
30003444:	4619      	mov	r1, r3
30003446:	e9cd 4801 	strd	r4, r8, [sp, #4]
3000344a:	f007 fd67 	bl	3000af1c <rtk_log_write>
3000344e:	2001      	movs	r0, #1
30003450:	f885 8061 	strb.w	r8, [r5, #97]	; 0x61
30003454:	772e      	strb	r6, [r5, #28]
30003456:	776e      	strb	r6, [r5, #29]
30003458:	b007      	add	sp, #28
3000345a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000345e:	bf00      	nop
30003460:	2001c01c 	.word	0x2001c01c
30003464:	3000b8fa 	.word	0x3000b8fa
30003468:	3000b903 	.word	0x3000b903

3000346c <flash_calibration_highspeed>:
3000346c:	b5f7      	push	{r0, r1, r2, r4, r5, r6, r7, lr}
3000346e:	4f19      	ldr	r7, [pc, #100]	; (300034d4 <flash_calibration_highspeed+0x68>)
30003470:	4605      	mov	r5, r0
30003472:	2400      	movs	r4, #0
30003474:	793e      	ldrb	r6, [r7, #4]
30003476:	f887 4060 	strb.w	r4, [r7, #96]	; 0x60
3000347a:	f7ff fe05 	bl	30003088 <FLASH_ClockDiv>
3000347e:	4629      	mov	r1, r5
30003480:	4630      	mov	r0, r6
30003482:	f7ff ffa5 	bl	300033d0 <_flash_calibration_highspeed>
30003486:	2801      	cmp	r0, #1
30003488:	d118      	bne.n	300034bc <flash_calibration_highspeed+0x50>
3000348a:	f7ff fe7d 	bl	30003188 <FLASH_CalibrationNewCmd>
3000348e:	f897 0061 	ldrb.w	r0, [r7, #97]	; 0x61
30003492:	f7ff fe3f 	bl	30003114 <FLASH_CalibrationPhaseIdx>
30003496:	4b10      	ldr	r3, [pc, #64]	; (300034d8 <flash_calibration_highspeed+0x6c>)
30003498:	4798      	blx	r3
3000349a:	b160      	cbz	r0, 300034b6 <flash_calibration_highspeed+0x4a>
3000349c:	4630      	mov	r0, r6
3000349e:	4b0f      	ldr	r3, [pc, #60]	; (300034dc <flash_calibration_highspeed+0x70>)
300034a0:	4798      	blx	r3
300034a2:	4b0f      	ldr	r3, [pc, #60]	; (300034e0 <flash_calibration_highspeed+0x74>)
300034a4:	2004      	movs	r0, #4
300034a6:	4a0f      	ldr	r2, [pc, #60]	; (300034e4 <flash_calibration_highspeed+0x78>)
300034a8:	4619      	mov	r1, r3
300034aa:	9500      	str	r5, [sp, #0]
300034ac:	f007 fd36 	bl	3000af1c <rtk_log_write>
300034b0:	2001      	movs	r0, #1
300034b2:	b003      	add	sp, #12
300034b4:	bdf0      	pop	{r4, r5, r6, r7, pc}
300034b6:	4630      	mov	r0, r6
300034b8:	4b0b      	ldr	r3, [pc, #44]	; (300034e8 <flash_calibration_highspeed+0x7c>)
300034ba:	e7f1      	b.n	300034a0 <flash_calibration_highspeed+0x34>
300034bc:	2001      	movs	r0, #1
300034be:	4b0b      	ldr	r3, [pc, #44]	; (300034ec <flash_calibration_highspeed+0x80>)
300034c0:	4798      	blx	r3
300034c2:	4b07      	ldr	r3, [pc, #28]	; (300034e0 <flash_calibration_highspeed+0x74>)
300034c4:	2002      	movs	r0, #2
300034c6:	4a0a      	ldr	r2, [pc, #40]	; (300034f0 <flash_calibration_highspeed+0x84>)
300034c8:	4619      	mov	r1, r3
300034ca:	9500      	str	r5, [sp, #0]
300034cc:	f007 fd26 	bl	3000af1c <rtk_log_write>
300034d0:	4620      	mov	r0, r4
300034d2:	e7ee      	b.n	300034b2 <flash_calibration_highspeed+0x46>
300034d4:	2001c01c 	.word	0x2001c01c
300034d8:	30008ff5 	.word	0x30008ff5
300034dc:	0000a53d 	.word	0x0000a53d
300034e0:	3000b8fa 	.word	0x3000b8fa
300034e4:	3000b926 	.word	0x3000b926
300034e8:	0000c045 	.word	0x0000c045
300034ec:	0000b569 	.word	0x0000b569
300034f0:	3000b943 	.word	0x3000b943

300034f4 <flash_rx_mode_switch>:
300034f4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300034f8:	2300      	movs	r3, #0
300034fa:	b08b      	sub	sp, #44	; 0x2c
300034fc:	4e3e      	ldr	r6, [pc, #248]	; (300035f8 <flash_rx_mode_switch+0x104>)
300034fe:	4605      	mov	r5, r0
30003500:	ac05      	add	r4, sp, #20
30003502:	f88d 300b 	strb.w	r3, [sp, #11]
30003506:	4f3d      	ldr	r7, [pc, #244]	; (300035fc <flash_rx_mode_switch+0x108>)
30003508:	f8df 811c 	ldr.w	r8, [pc, #284]	; 30003628 <flash_rx_mode_switch+0x134>
3000350c:	f8df 911c 	ldr.w	r9, [pc, #284]	; 3000362c <flash_rx_mode_switch+0x138>
30003510:	ce0f      	ldmia	r6!, {r0, r1, r2, r3}
30003512:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30003514:	6833      	ldr	r3, [r6, #0]
30003516:	f1c5 0004 	rsb	r0, r5, #4
3000351a:	4e39      	ldr	r6, [pc, #228]	; (30003600 <flash_rx_mode_switch+0x10c>)
3000351c:	6023      	str	r3, [r4, #0]
3000351e:	b2c4      	uxtb	r4, r0
30003520:	fa46 f580 	sxtab	r5, r6, r0
30003524:	f1c4 0304 	rsb	r3, r4, #4
30003528:	b2db      	uxtb	r3, r3
3000352a:	2b04      	cmp	r3, #4
3000352c:	d90b      	bls.n	30003546 <flash_rx_mode_switch+0x52>
3000352e:	2b05      	cmp	r3, #5
30003530:	d15a      	bne.n	300035e8 <flash_rx_mode_switch+0xf4>
30003532:	4b34      	ldr	r3, [pc, #208]	; (30003604 <flash_rx_mode_switch+0x110>)
30003534:	2002      	movs	r0, #2
30003536:	4a34      	ldr	r2, [pc, #208]	; (30003608 <flash_rx_mode_switch+0x114>)
30003538:	4619      	mov	r1, r3
3000353a:	f007 fcef 	bl	3000af1c <rtk_log_write>
3000353e:	2000      	movs	r0, #0
30003540:	b00b      	add	sp, #44	; 0x2c
30003542:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003546:	47b8      	blx	r7
30003548:	2800      	cmp	r0, #0
3000354a:	d04f      	beq.n	300035ec <flash_rx_mode_switch+0xf8>
3000354c:	2c04      	cmp	r4, #4
3000354e:	d001      	beq.n	30003554 <flash_rx_mode_switch+0x60>
30003550:	2c02      	cmp	r4, #2
30003552:	d102      	bne.n	3000355a <flash_rx_mode_switch+0x66>
30003554:	2301      	movs	r3, #1
30003556:	f886 3063 	strb.w	r3, [r6, #99]	; 0x63
3000355a:	6833      	ldr	r3, [r6, #0]
3000355c:	2b03      	cmp	r3, #3
3000355e:	d114      	bne.n	3000358a <flash_rx_mode_switch+0x96>
30003560:	f10d 020b 	add.w	r2, sp, #11
30003564:	2101      	movs	r1, #1
30003566:	2085      	movs	r0, #133	; 0x85
30003568:	4b28      	ldr	r3, [pc, #160]	; (3000360c <flash_rx_mode_switch+0x118>)
3000356a:	4798      	blx	r3
3000356c:	f89d 300b 	ldrb.w	r3, [sp, #11]
30003570:	7faa      	ldrb	r2, [r5, #30]
30003572:	2101      	movs	r1, #1
30003574:	f003 030f 	and.w	r3, r3, #15
30003578:	2081      	movs	r0, #129	; 0x81
3000357a:	ea43 1302 	orr.w	r3, r3, r2, lsl #4
3000357e:	f10d 020b 	add.w	r2, sp, #11
30003582:	f88d 300b 	strb.w	r3, [sp, #11]
30003586:	4b22      	ldr	r3, [pc, #136]	; (30003610 <flash_rx_mode_switch+0x11c>)
30003588:	4798      	blx	r3
3000358a:	2300      	movs	r3, #0
3000358c:	4620      	mov	r0, r4
3000358e:	7773      	strb	r3, [r6, #29]
30003590:	47c8      	blx	r9
30003592:	4b20      	ldr	r3, [pc, #128]	; (30003614 <flash_rx_mode_switch+0x120>)
30003594:	695a      	ldr	r2, [r3, #20]
30003596:	03d2      	lsls	r2, r2, #15
30003598:	d509      	bpl.n	300035ae <flash_rx_mode_switch+0xba>
3000359a:	f3bf 8f4f 	dsb	sy
3000359e:	f04f 6200 	mov.w	r2, #134217728	; 0x8000000
300035a2:	f8c3 225c 	str.w	r2, [r3, #604]	; 0x25c
300035a6:	f3bf 8f4f 	dsb	sy
300035aa:	f3bf 8f6f 	isb	sy
300035ae:	4b1a      	ldr	r3, [pc, #104]	; (30003618 <flash_rx_mode_switch+0x124>)
300035b0:	3d01      	subs	r5, #1
300035b2:	4798      	blx	r3
300035b4:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
300035b8:	4918      	ldr	r1, [pc, #96]	; (3000361c <flash_rx_mode_switch+0x128>)
300035ba:	a803      	add	r0, sp, #12
300035bc:	681a      	ldr	r2, [r3, #0]
300035be:	9203      	str	r2, [sp, #12]
300035c0:	2208      	movs	r2, #8
300035c2:	685b      	ldr	r3, [r3, #4]
300035c4:	9304      	str	r3, [sp, #16]
300035c6:	4b16      	ldr	r3, [pc, #88]	; (30003620 <flash_rx_mode_switch+0x12c>)
300035c8:	4798      	blx	r3
300035ca:	1e63      	subs	r3, r4, #1
300035cc:	b2db      	uxtb	r3, r3
300035ce:	b988      	cbnz	r0, 300035f4 <flash_rx_mode_switch+0x100>
300035d0:	ab0a      	add	r3, sp, #40	; 0x28
300035d2:	4a14      	ldr	r2, [pc, #80]	; (30003624 <flash_rx_mode_switch+0x130>)
300035d4:	2004      	movs	r0, #4
300035d6:	eb03 0484 	add.w	r4, r3, r4, lsl #2
300035da:	f854 3c14 	ldr.w	r3, [r4, #-20]
300035de:	9300      	str	r3, [sp, #0]
300035e0:	4b08      	ldr	r3, [pc, #32]	; (30003604 <flash_rx_mode_switch+0x110>)
300035e2:	4619      	mov	r1, r3
300035e4:	f007 fc9a 	bl	3000af1c <rtk_log_write>
300035e8:	2001      	movs	r0, #1
300035ea:	e7a9      	b.n	30003540 <flash_rx_mode_switch+0x4c>
300035ec:	7770      	strb	r0, [r6, #29]
300035ee:	4620      	mov	r0, r4
300035f0:	47c0      	blx	r8
300035f2:	e7ce      	b.n	30003592 <flash_rx_mode_switch+0x9e>
300035f4:	461c      	mov	r4, r3
300035f6:	e795      	b.n	30003524 <flash_rx_mode_switch+0x30>
300035f8:	3000b9f8 	.word	0x3000b9f8
300035fc:	30008ff5 	.word	0x30008ff5
30003600:	2001c01c 	.word	0x2001c01c
30003604:	3000b8fa 	.word	0x3000b8fa
30003608:	3000b978 	.word	0x3000b978
3000360c:	00009db5 	.word	0x00009db5
30003610:	0000a149 	.word	0x0000a149
30003614:	e000ed00 	.word	0xe000ed00
30003618:	30008e0d 	.word	0x30008e0d
3000361c:	00054060 	.word	0x00054060
30003620:	000129bd 	.word	0x000129bd
30003624:	3000b962 	.word	0x3000b962
30003628:	0000c045 	.word	0x0000c045
3000362c:	0000a53d 	.word	0x0000a53d

30003630 <flash_highspeed_setup>:
30003630:	4ba0      	ldr	r3, [pc, #640]	; (300038b4 <flash_highspeed_setup+0x284>)
30003632:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30003636:	881a      	ldrh	r2, [r3, #0]
30003638:	b086      	sub	sp, #24
3000363a:	2700      	movs	r7, #0
3000363c:	f44f 4300 	mov.w	r3, #32768	; 0x8000
30003640:	421a      	tst	r2, r3
30003642:	d104      	bne.n	3000364e <flash_highspeed_setup+0x1e>
30003644:	3701      	adds	r7, #1
30003646:	085b      	lsrs	r3, r3, #1
30003648:	b2ff      	uxtb	r7, r7
3000364a:	2f10      	cmp	r7, #16
3000364c:	d1f8      	bne.n	30003640 <flash_highspeed_setup+0x10>
3000364e:	4b9a      	ldr	r3, [pc, #616]	; (300038b8 <flash_highspeed_setup+0x288>)
30003650:	2500      	movs	r5, #0
30003652:	881a      	ldrh	r2, [r3, #0]
30003654:	f44f 4300 	mov.w	r3, #32768	; 0x8000
30003658:	421a      	tst	r2, r3
3000365a:	d104      	bne.n	30003666 <flash_highspeed_setup+0x36>
3000365c:	3501      	adds	r5, #1
3000365e:	085b      	lsrs	r3, r3, #1
30003660:	b2ed      	uxtb	r5, r5
30003662:	2d10      	cmp	r5, #16
30003664:	d1f8      	bne.n	30003658 <flash_highspeed_setup+0x28>
30003666:	b672      	cpsid	i
30003668:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000366c:	f10d 0914 	add.w	r9, sp, #20
30003670:	4c92      	ldr	r4, [pc, #584]	; (300038bc <flash_highspeed_setup+0x28c>)
30003672:	6813      	ldr	r3, [r2, #0]
30003674:	4e92      	ldr	r6, [pc, #584]	; (300038c0 <flash_highspeed_setup+0x290>)
30003676:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
3000367a:	6013      	str	r3, [r2, #0]
3000367c:	4b91      	ldr	r3, [pc, #580]	; (300038c4 <flash_highspeed_setup+0x294>)
3000367e:	4798      	blx	r3
30003680:	4602      	mov	r2, r0
30003682:	2800      	cmp	r0, #0
30003684:	f000 80bf 	beq.w	30003806 <flash_highspeed_setup+0x1d6>
30003688:	464a      	mov	r2, r9
3000368a:	2103      	movs	r1, #3
3000368c:	f894 0055 	ldrb.w	r0, [r4, #85]	; 0x55
30003690:	f8df 8284 	ldr.w	r8, [pc, #644]	; 30003918 <flash_highspeed_setup+0x2e8>
30003694:	47c0      	blx	r8
30003696:	f89d 3016 	ldrb.w	r3, [sp, #22]
3000369a:	4a8b      	ldr	r2, [pc, #556]	; (300038c8 <flash_highspeed_setup+0x298>)
3000369c:	2004      	movs	r0, #4
3000369e:	9302      	str	r3, [sp, #8]
300036a0:	f89d 3015 	ldrb.w	r3, [sp, #21]
300036a4:	9301      	str	r3, [sp, #4]
300036a6:	f89d 3014 	ldrb.w	r3, [sp, #20]
300036aa:	9300      	str	r3, [sp, #0]
300036ac:	4b87      	ldr	r3, [pc, #540]	; (300038cc <flash_highspeed_setup+0x29c>)
300036ae:	4619      	mov	r1, r3
300036b0:	f007 fc34 	bl	3000af1c <rtk_log_write>
300036b4:	f89d 3015 	ldrb.w	r3, [sp, #21]
300036b8:	f89d 0016 	ldrb.w	r0, [sp, #22]
300036bc:	021b      	lsls	r3, r3, #8
300036be:	ea43 4300 	orr.w	r3, r3, r0, lsl #16
300036c2:	f89d 0014 	ldrb.w	r0, [sp, #20]
300036c6:	4318      	orrs	r0, r3
300036c8:	f7ff fcc2 	bl	30003050 <flash_get_chip_info>
300036cc:	6030      	str	r0, [r6, #0]
300036ce:	b950      	cbnz	r0, 300036e6 <flash_highspeed_setup+0xb6>
300036d0:	4b7e      	ldr	r3, [pc, #504]	; (300038cc <flash_highspeed_setup+0x29c>)
300036d2:	2003      	movs	r0, #3
300036d4:	4a7e      	ldr	r2, [pc, #504]	; (300038d0 <flash_highspeed_setup+0x2a0>)
300036d6:	4619      	mov	r1, r3
300036d8:	f007 fc20 	bl	3000af1c <rtk_log_write>
300036dc:	f240 2127 	movw	r1, #551	; 0x227
300036e0:	487c      	ldr	r0, [pc, #496]	; (300038d4 <flash_highspeed_setup+0x2a4>)
300036e2:	f007 fead 	bl	3000b440 <__io_assert_failed_veneer>
300036e6:	6832      	ldr	r2, [r6, #0]
300036e8:	7a13      	ldrb	r3, [r2, #8]
300036ea:	2b05      	cmp	r3, #5
300036ec:	d808      	bhi.n	30003700 <flash_highspeed_setup+0xd0>
300036ee:	3b01      	subs	r3, #1
300036f0:	4872      	ldr	r0, [pc, #456]	; (300038bc <flash_highspeed_setup+0x28c>)
300036f2:	2b04      	cmp	r3, #4
300036f4:	d848      	bhi.n	30003788 <flash_highspeed_setup+0x158>
300036f6:	e8df f003 	tbb	[pc, r3]
300036fa:	544a      	.short	0x544a
300036fc:	5b56      	.short	0x5b56
300036fe:	5d          	.byte	0x5d
300036ff:	00          	.byte	0x00
30003700:	2bfe      	cmp	r3, #254	; 0xfe
30003702:	d05d      	beq.n	300037c0 <flash_highspeed_setup+0x190>
30003704:	4b74      	ldr	r3, [pc, #464]	; (300038d8 <flash_highspeed_setup+0x2a8>)
30003706:	4798      	blx	r3
30003708:	b110      	cbz	r0, 30003710 <flash_highspeed_setup+0xe0>
3000370a:	2304      	movs	r3, #4
3000370c:	f884 3062 	strb.w	r3, [r4, #98]	; 0x62
30003710:	6833      	ldr	r3, [r6, #0]
30003712:	464a      	mov	r2, r9
30003714:	2101      	movs	r1, #1
30003716:	f894 0056 	ldrb.w	r0, [r4, #86]	; 0x56
3000371a:	f8d3 a00c 	ldr.w	sl, [r3, #12]
3000371e:	68e3      	ldr	r3, [r4, #12]
30003720:	9304      	str	r3, [sp, #16]
30003722:	47c0      	blx	r8
30003724:	69a3      	ldr	r3, [r4, #24]
30003726:	2b00      	cmp	r3, #0
30003728:	d055      	beq.n	300037d6 <flash_highspeed_setup+0x1a6>
3000372a:	2602      	movs	r6, #2
3000372c:	f10d 0215 	add.w	r2, sp, #21
30003730:	2101      	movs	r1, #1
30003732:	f894 0057 	ldrb.w	r0, [r4, #87]	; 0x57
30003736:	47c0      	blx	r8
30003738:	9b05      	ldr	r3, [sp, #20]
3000373a:	4632      	mov	r2, r6
3000373c:	a904      	add	r1, sp, #16
3000373e:	4648      	mov	r0, r9
30003740:	ea03 030a 	and.w	r3, r3, sl
30003744:	9305      	str	r3, [sp, #20]
30003746:	4b65      	ldr	r3, [pc, #404]	; (300038dc <flash_highspeed_setup+0x2ac>)
30003748:	4798      	blx	r3
3000374a:	b150      	cbz	r0, 30003762 <flash_highspeed_setup+0x132>
3000374c:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
30003750:	aa04      	add	r2, sp, #16
30003752:	f894 0058 	ldrb.w	r0, [r4, #88]	; 0x58
30003756:	f8df 81c4 	ldr.w	r8, [pc, #452]	; 3000391c <flash_highspeed_setup+0x2ec>
3000375a:	2b00      	cmp	r3, #0
3000375c:	d14b      	bne.n	300037f6 <flash_highspeed_setup+0x1c6>
3000375e:	4631      	mov	r1, r6
30003760:	47c0      	blx	r8
30003762:	4638      	mov	r0, r7
30003764:	f7ff fec6 	bl	300034f4 <flash_rx_mode_switch>
30003768:	4b5d      	ldr	r3, [pc, #372]	; (300038e0 <flash_highspeed_setup+0x2b0>)
3000376a:	4798      	blx	r3
3000376c:	2801      	cmp	r0, #1
3000376e:	d007      	beq.n	30003780 <flash_highspeed_setup+0x150>
30003770:	2d06      	cmp	r5, #6
30003772:	bf28      	it	cs
30003774:	2506      	movcs	r5, #6
30003776:	f1c5 0009 	rsb	r0, r5, #9
3000377a:	b2c0      	uxtb	r0, r0
3000377c:	f7ff fe76 	bl	3000346c <flash_calibration_highspeed>
30003780:	b662      	cpsie	i
30003782:	b006      	add	sp, #24
30003784:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003788:	4b56      	ldr	r3, [pc, #344]	; (300038e4 <flash_highspeed_setup+0x2b4>)
3000378a:	4798      	blx	r3
3000378c:	e7ba      	b.n	30003704 <flash_highspeed_setup+0xd4>
3000378e:	4b56      	ldr	r3, [pc, #344]	; (300038e8 <flash_highspeed_setup+0x2b8>)
30003790:	4798      	blx	r3
30003792:	f89d 3016 	ldrb.w	r3, [sp, #22]
30003796:	2b15      	cmp	r3, #21
30003798:	d9b4      	bls.n	30003704 <flash_highspeed_setup+0xd4>
3000379a:	2331      	movs	r3, #49	; 0x31
3000379c:	f884 3059 	strb.w	r3, [r4, #89]	; 0x59
300037a0:	e7b0      	b.n	30003704 <flash_highspeed_setup+0xd4>
300037a2:	4b52      	ldr	r3, [pc, #328]	; (300038ec <flash_highspeed_setup+0x2bc>)
300037a4:	e7f1      	b.n	3000378a <flash_highspeed_setup+0x15a>
300037a6:	4b51      	ldr	r3, [pc, #324]	; (300038ec <flash_highspeed_setup+0x2bc>)
300037a8:	4798      	blx	r3
300037aa:	2300      	movs	r3, #0
300037ac:	60e3      	str	r3, [r4, #12]
300037ae:	e7a9      	b.n	30003704 <flash_highspeed_setup+0xd4>
300037b0:	4b4f      	ldr	r3, [pc, #316]	; (300038f0 <flash_highspeed_setup+0x2c0>)
300037b2:	e7ea      	b.n	3000378a <flash_highspeed_setup+0x15a>
300037b4:	4b4d      	ldr	r3, [pc, #308]	; (300038ec <flash_highspeed_setup+0x2bc>)
300037b6:	4798      	blx	r3
300037b8:	4b4e      	ldr	r3, [pc, #312]	; (300038f4 <flash_highspeed_setup+0x2c4>)
300037ba:	2201      	movs	r2, #1
300037bc:	701a      	strb	r2, [r3, #0]
300037be:	e7a1      	b.n	30003704 <flash_highspeed_setup+0xd4>
300037c0:	6913      	ldr	r3, [r2, #16]
300037c2:	b923      	cbnz	r3, 300037ce <flash_highspeed_setup+0x19e>
300037c4:	f44f 7111 	mov.w	r1, #580	; 0x244
300037c8:	4842      	ldr	r0, [pc, #264]	; (300038d4 <flash_highspeed_setup+0x2a4>)
300037ca:	f007 fe39 	bl	3000b440 <__io_assert_failed_veneer>
300037ce:	6833      	ldr	r3, [r6, #0]
300037d0:	691b      	ldr	r3, [r3, #16]
300037d2:	4798      	blx	r3
300037d4:	e796      	b.n	30003704 <flash_highspeed_setup+0xd4>
300037d6:	4b47      	ldr	r3, [pc, #284]	; (300038f4 <flash_highspeed_setup+0x2c4>)
300037d8:	781b      	ldrb	r3, [r3, #0]
300037da:	b153      	cbz	r3, 300037f2 <flash_highspeed_setup+0x1c2>
300037dc:	f10d 0215 	add.w	r2, sp, #21
300037e0:	2102      	movs	r1, #2
300037e2:	2015      	movs	r0, #21
300037e4:	2603      	movs	r6, #3
300037e6:	47c0      	blx	r8
300037e8:	9b04      	ldr	r3, [sp, #16]
300037ea:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
300037ee:	9304      	str	r3, [sp, #16]
300037f0:	e7a2      	b.n	30003738 <flash_highspeed_setup+0x108>
300037f2:	2601      	movs	r6, #1
300037f4:	e7a0      	b.n	30003738 <flash_highspeed_setup+0x108>
300037f6:	2101      	movs	r1, #1
300037f8:	47c0      	blx	r8
300037fa:	f10d 0211 	add.w	r2, sp, #17
300037fe:	2101      	movs	r1, #1
30003800:	f894 0059 	ldrb.w	r0, [r4, #89]	; 0x59
30003804:	e7ac      	b.n	30003760 <flash_highspeed_setup+0x130>
30003806:	4601      	mov	r1, r0
30003808:	2302      	movs	r3, #2
3000380a:	f8cd 9000 	str.w	r9, [sp]
3000380e:	f8df 8110 	ldr.w	r8, [pc, #272]	; 30003920 <flash_highspeed_setup+0x2f0>
30003812:	f894 0055 	ldrb.w	r0, [r4, #85]	; 0x55
30003816:	47c0      	blx	r8
30003818:	f89d 3015 	ldrb.w	r3, [sp, #21]
3000381c:	4a36      	ldr	r2, [pc, #216]	; (300038f8 <flash_highspeed_setup+0x2c8>)
3000381e:	2004      	movs	r0, #4
30003820:	9301      	str	r3, [sp, #4]
30003822:	f89d 3014 	ldrb.w	r3, [sp, #20]
30003826:	9300      	str	r3, [sp, #0]
30003828:	4b28      	ldr	r3, [pc, #160]	; (300038cc <flash_highspeed_setup+0x29c>)
3000382a:	4619      	mov	r1, r3
3000382c:	f007 fb76 	bl	3000af1c <rtk_log_write>
30003830:	f89d 3015 	ldrb.w	r3, [sp, #21]
30003834:	f89d 0016 	ldrb.w	r0, [sp, #22]
30003838:	021b      	lsls	r3, r3, #8
3000383a:	ea43 4300 	orr.w	r3, r3, r0, lsl #16
3000383e:	f89d 0014 	ldrb.w	r0, [sp, #20]
30003842:	4318      	orrs	r0, r3
30003844:	f7ff fc04 	bl	30003050 <flash_get_chip_info>
30003848:	6030      	str	r0, [r6, #0]
3000384a:	b950      	cbnz	r0, 30003862 <flash_highspeed_setup+0x232>
3000384c:	4b1f      	ldr	r3, [pc, #124]	; (300038cc <flash_highspeed_setup+0x29c>)
3000384e:	2003      	movs	r0, #3
30003850:	4a1f      	ldr	r2, [pc, #124]	; (300038d0 <flash_highspeed_setup+0x2a0>)
30003852:	4619      	mov	r1, r3
30003854:	f007 fb62 	bl	3000af1c <rtk_log_write>
30003858:	f240 215e 	movw	r1, #606	; 0x25e
3000385c:	4827      	ldr	r0, [pc, #156]	; (300038fc <flash_highspeed_setup+0x2cc>)
3000385e:	f007 fdef 	bl	3000b440 <__io_assert_failed_veneer>
30003862:	6832      	ldr	r2, [r6, #0]
30003864:	7a13      	ldrb	r3, [r2, #8]
30003866:	2b04      	cmp	r3, #4
30003868:	d806      	bhi.n	30003878 <flash_highspeed_setup+0x248>
3000386a:	2b04      	cmp	r3, #4
3000386c:	d806      	bhi.n	3000387c <flash_highspeed_setup+0x24c>
3000386e:	e8df f003 	tbb	[pc, r3]
30003872:	1814      	.short	0x1814
30003874:	051b      	.short	0x051b
30003876:	1e          	.byte	0x1e
30003877:	00          	.byte	0x00
30003878:	2bfe      	cmp	r3, #254	; 0xfe
3000387a:	d053      	beq.n	30003924 <flash_highspeed_setup+0x2f4>
3000387c:	68e3      	ldr	r3, [r4, #12]
3000387e:	4c20      	ldr	r4, [pc, #128]	; (30003900 <flash_highspeed_setup+0x2d0>)
30003880:	b13b      	cbz	r3, 30003892 <flash_highspeed_setup+0x262>
30003882:	20b0      	movs	r0, #176	; 0xb0
30003884:	4b1f      	ldr	r3, [pc, #124]	; (30003904 <flash_highspeed_setup+0x2d4>)
30003886:	4798      	blx	r3
30003888:	f040 0001 	orr.w	r0, r0, #1
3000388c:	b2c1      	uxtb	r1, r0
3000388e:	20b0      	movs	r0, #176	; 0xb0
30003890:	47a0      	blx	r4
30003892:	2100      	movs	r1, #0
30003894:	20a0      	movs	r0, #160	; 0xa0
30003896:	47a0      	blx	r4
30003898:	e763      	b.n	30003762 <flash_highspeed_setup+0x132>
3000389a:	4808      	ldr	r0, [pc, #32]	; (300038bc <flash_highspeed_setup+0x28c>)
3000389c:	4b1a      	ldr	r3, [pc, #104]	; (30003908 <flash_highspeed_setup+0x2d8>)
3000389e:	4798      	blx	r3
300038a0:	e7ec      	b.n	3000387c <flash_highspeed_setup+0x24c>
300038a2:	4806      	ldr	r0, [pc, #24]	; (300038bc <flash_highspeed_setup+0x28c>)
300038a4:	4b19      	ldr	r3, [pc, #100]	; (3000390c <flash_highspeed_setup+0x2dc>)
300038a6:	e7fa      	b.n	3000389e <flash_highspeed_setup+0x26e>
300038a8:	4804      	ldr	r0, [pc, #16]	; (300038bc <flash_highspeed_setup+0x28c>)
300038aa:	4b19      	ldr	r3, [pc, #100]	; (30003910 <flash_highspeed_setup+0x2e0>)
300038ac:	e7f7      	b.n	3000389e <flash_highspeed_setup+0x26e>
300038ae:	4803      	ldr	r0, [pc, #12]	; (300038bc <flash_highspeed_setup+0x28c>)
300038b0:	4b18      	ldr	r3, [pc, #96]	; (30003914 <flash_highspeed_setup+0x2e4>)
300038b2:	e7f4      	b.n	3000389e <flash_highspeed_setup+0x26e>
300038b4:	3000b858 	.word	0x3000b858
300038b8:	3000b85a 	.word	0x3000b85a
300038bc:	2001c01c 	.word	0x2001c01c
300038c0:	3000ead4 	.word	0x3000ead4
300038c4:	30008ff5 	.word	0x30008ff5
300038c8:	3000b99c 	.word	0x3000b99c
300038cc:	3000b8fa 	.word	0x3000b8fa
300038d0:	3000b9b7 	.word	0x3000b9b7
300038d4:	3000ba1c 	.word	0x3000ba1c
300038d8:	0000c1d9 	.word	0x0000c1d9
300038dc:	000129bd 	.word	0x000129bd
300038e0:	0000c0f9 	.word	0x0000c0f9
300038e4:	0000a4c1 	.word	0x0000a4c1
300038e8:	0000a441 	.word	0x0000a441
300038ec:	0000a3c5 	.word	0x0000a3c5
300038f0:	0000a345 	.word	0x0000a345
300038f4:	3000ead1 	.word	0x3000ead1
300038f8:	3000b9e1 	.word	0x3000b9e1
300038fc:	3000ba0c 	.word	0x3000ba0c
30003900:	0000bd0d 	.word	0x0000bd0d
30003904:	0000b991 	.word	0x0000b991
30003908:	0000bfcd 	.word	0x0000bfcd
3000390c:	0000bf59 	.word	0x0000bf59
30003910:	0000bee9 	.word	0x0000bee9
30003914:	0000be71 	.word	0x0000be71
30003918:	00009db5 	.word	0x00009db5
3000391c:	0000a149 	.word	0x0000a149
30003920:	0000b8c1 	.word	0x0000b8c1
30003924:	6913      	ldr	r3, [r2, #16]
30003926:	b923      	cbnz	r3, 30003932 <flash_highspeed_setup+0x302>
30003928:	f44f 711c 	mov.w	r1, #624	; 0x270
3000392c:	4803      	ldr	r0, [pc, #12]	; (3000393c <flash_highspeed_setup+0x30c>)
3000392e:	f007 fd87 	bl	3000b440 <__io_assert_failed_veneer>
30003932:	6833      	ldr	r3, [r6, #0]
30003934:	691b      	ldr	r3, [r3, #16]
30003936:	4798      	blx	r3
30003938:	e7a0      	b.n	3000387c <flash_highspeed_setup+0x24c>
3000393a:	bf00      	nop
3000393c:	3000ba0c 	.word	0x3000ba0c

30003940 <BOOT_INT_SecureFault>:
30003940:	f3ef 8008 	mrs	r0, MSP
30003944:	f3ef 8109 	mrs	r1, PSP
30003948:	4672      	mov	r2, lr
3000394a:	f04f 0304 	mov.w	r3, #4
3000394e:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30003952:	f000 b861 	b.w	30003a18 <BOOT_FaultHandler>
30003956:	4770      	bx	lr

30003958 <AsciiToHex>:
30003958:	f1a0 0341 	sub.w	r3, r0, #65	; 0x41
3000395c:	2b05      	cmp	r3, #5
3000395e:	d802      	bhi.n	30003966 <AsciiToHex+0xe>
30003960:	3837      	subs	r0, #55	; 0x37
30003962:	b2c0      	uxtb	r0, r0
30003964:	4770      	bx	lr
30003966:	f1a0 0361 	sub.w	r3, r0, #97	; 0x61
3000396a:	2b05      	cmp	r3, #5
3000396c:	d801      	bhi.n	30003972 <AsciiToHex+0x1a>
3000396e:	3857      	subs	r0, #87	; 0x57
30003970:	e7f7      	b.n	30003962 <AsciiToHex+0xa>
30003972:	3830      	subs	r0, #48	; 0x30
30003974:	b2c0      	uxtb	r0, r0
30003976:	280a      	cmp	r0, #10
30003978:	bf28      	it	cs
3000397a:	20ff      	movcs	r0, #255	; 0xff
3000397c:	4770      	bx	lr

3000397e <ascii_to_integer>:
3000397e:	4602      	mov	r2, r0
30003980:	2000      	movs	r0, #0
30003982:	b530      	push	{r4, r5, lr}
30003984:	4411      	add	r1, r2
30003986:	240a      	movs	r4, #10
30003988:	1a8b      	subs	r3, r1, r2
3000398a:	2b00      	cmp	r3, #0
3000398c:	dc00      	bgt.n	30003990 <ascii_to_integer+0x12>
3000398e:	bd30      	pop	{r4, r5, pc}
30003990:	f812 3b01 	ldrb.w	r3, [r2], #1
30003994:	f1a3 0530 	sub.w	r5, r3, #48	; 0x30
30003998:	2d09      	cmp	r5, #9
3000399a:	d804      	bhi.n	300039a6 <ascii_to_integer+0x28>
3000399c:	fb04 3300 	mla	r3, r4, r0, r3
300039a0:	f1a3 0030 	sub.w	r0, r3, #48	; 0x30
300039a4:	e7f0      	b.n	30003988 <ascii_to_integer+0xa>
300039a6:	2000      	movs	r0, #0
300039a8:	e7f1      	b.n	3000398e <ascii_to_integer+0x10>

300039aa <eight_4bit_to_int>:
300039aa:	1dc1      	adds	r1, r0, #7
300039ac:	4602      	mov	r2, r0
300039ae:	2300      	movs	r3, #0
300039b0:	b510      	push	{r4, lr}
300039b2:	f812 4b01 	ldrb.w	r4, [r2], #1
300039b6:	4323      	orrs	r3, r4
300039b8:	428a      	cmp	r2, r1
300039ba:	ea4f 1303 	mov.w	r3, r3, lsl #4
300039be:	d1f8      	bne.n	300039b2 <eight_4bit_to_int+0x8>
300039c0:	79c0      	ldrb	r0, [r0, #7]
300039c2:	4318      	orrs	r0, r3
300039c4:	bd10      	pop	{r4, pc}

300039c6 <hotfix_get_addr_size>:
300039c6:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300039ca:	2600      	movs	r6, #0
300039cc:	4680      	mov	r8, r0
300039ce:	460c      	mov	r4, r1
300039d0:	4615      	mov	r5, r2
300039d2:	4699      	mov	r9, r3
300039d4:	4637      	mov	r7, r6
300039d6:	f101 0a09 	add.w	sl, r1, #9
300039da:	4546      	cmp	r6, r8
300039dc:	d101      	bne.n	300039e2 <hotfix_get_addr_size+0x1c>
300039de:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300039e2:	6067      	str	r7, [r4, #4]
300039e4:	210b      	movs	r1, #11
300039e6:	6027      	str	r7, [r4, #0]
300039e8:	4620      	mov	r0, r4
300039ea:	f8c4 7007 	str.w	r7, [r4, #7]
300039ee:	602f      	str	r7, [r5, #0]
300039f0:	606f      	str	r7, [r5, #4]
300039f2:	f003 fb77 	bl	300070e4 <ymodem_uart_getdata>
300039f6:	1c62      	adds	r2, r4, #1
300039f8:	1e69      	subs	r1, r5, #1
300039fa:	f812 0f01 	ldrb.w	r0, [r2, #1]!
300039fe:	f7ff ffab 	bl	30003958 <AsciiToHex>
30003a02:	f801 0f01 	strb.w	r0, [r1, #1]!
30003a06:	4552      	cmp	r2, sl
30003a08:	d1f7      	bne.n	300039fa <hotfix_get_addr_size+0x34>
30003a0a:	4628      	mov	r0, r5
30003a0c:	f7ff ffcd 	bl	300039aa <eight_4bit_to_int>
30003a10:	f849 0026 	str.w	r0, [r9, r6, lsl #2]
30003a14:	3601      	adds	r6, #1
30003a16:	e7e0      	b.n	300039da <hotfix_get_addr_size+0x14>

30003a18 <BOOT_FaultHandler>:
30003a18:	b5f0      	push	{r4, r5, r6, r7, lr}
30003a1a:	461f      	mov	r7, r3
30003a1c:	4b37      	ldr	r3, [pc, #220]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003a1e:	b095      	sub	sp, #84	; 0x54
30003a20:	4614      	mov	r4, r2
30003a22:	4606      	mov	r6, r0
30003a24:	4a36      	ldr	r2, [pc, #216]	; (30003b00 <BOOT_FaultHandler+0xe8>)
30003a26:	2001      	movs	r0, #1
30003a28:	460d      	mov	r5, r1
30003a2a:	4619      	mov	r1, r3
30003a2c:	f007 fa76 	bl	3000af1c <rtk_log_write>
30003a30:	2244      	movs	r2, #68	; 0x44
30003a32:	2100      	movs	r1, #0
30003a34:	a803      	add	r0, sp, #12
30003a36:	f007 fcc3 	bl	3000b3c0 <____wrap_memset_veneer>
30003a3a:	0662      	lsls	r2, r4, #25
30003a3c:	4b2f      	ldr	r3, [pc, #188]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003a3e:	d54c      	bpl.n	30003ada <BOOT_FaultHandler+0xc2>
30003a40:	4619      	mov	r1, r3
30003a42:	4a30      	ldr	r2, [pc, #192]	; (30003b04 <BOOT_FaultHandler+0xec>)
30003a44:	2001      	movs	r0, #1
30003a46:	f007 fa69 	bl	3000af1c <rtk_log_write>
30003a4a:	f004 020c 	and.w	r2, r4, #12
30003a4e:	4631      	mov	r1, r6
30003a50:	f1a2 030c 	sub.w	r3, r2, #12
30003a54:	425a      	negs	r2, r3
30003a56:	415a      	adcs	r2, r3
30003a58:	f1a6 0320 	sub.w	r3, r6, #32
30003a5c:	a804      	add	r0, sp, #16
30003a5e:	9403      	str	r4, [sp, #12]
30003a60:	f853 cb04 	ldr.w	ip, [r3], #4
30003a64:	42b3      	cmp	r3, r6
30003a66:	f840 cb04 	str.w	ip, [r0], #4
30003a6a:	d1f9      	bne.n	30003a60 <BOOT_FaultHandler+0x48>
30003a6c:	2a00      	cmp	r2, #0
30003a6e:	bf08      	it	eq
30003a70:	460d      	moveq	r5, r1
30003a72:	aa0c      	add	r2, sp, #48	; 0x30
30003a74:	1f2b      	subs	r3, r5, #4
30003a76:	f105 011c 	add.w	r1, r5, #28
30003a7a:	f853 0f04 	ldr.w	r0, [r3, #4]!
30003a7e:	4299      	cmp	r1, r3
30003a80:	f842 0b04 	str.w	r0, [r2], #4
30003a84:	d1f9      	bne.n	30003a7a <BOOT_FaultHandler+0x62>
30003a86:	4629      	mov	r1, r5
30003a88:	6be8      	ldr	r0, [r5, #60]	; 0x3c
30003a8a:	4d1f      	ldr	r5, [pc, #124]	; (30003b08 <BOOT_FaultHandler+0xf0>)
30003a8c:	aa03      	add	r2, sp, #12
30003a8e:	f006 ffbf 	bl	3000aa10 <crash_dump>
30003a92:	4a1e      	ldr	r2, [pc, #120]	; (30003b0c <BOOT_FaultHandler+0xf4>)
30003a94:	f8d5 30e4 	ldr.w	r3, [r5, #228]	; 0xe4
30003a98:	2001      	movs	r0, #1
30003a9a:	9300      	str	r3, [sp, #0]
30003a9c:	4b17      	ldr	r3, [pc, #92]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003a9e:	4619      	mov	r1, r3
30003aa0:	f007 fa3c 	bl	3000af1c <rtk_log_write>
30003aa4:	f8d5 30e8 	ldr.w	r3, [r5, #232]	; 0xe8
30003aa8:	4a19      	ldr	r2, [pc, #100]	; (30003b10 <BOOT_FaultHandler+0xf8>)
30003aaa:	2001      	movs	r0, #1
30003aac:	9300      	str	r3, [sp, #0]
30003aae:	4b13      	ldr	r3, [pc, #76]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003ab0:	4619      	mov	r1, r3
30003ab2:	f007 fa33 	bl	3000af1c <rtk_log_write>
30003ab6:	6a6b      	ldr	r3, [r5, #36]	; 0x24
30003ab8:	4a16      	ldr	r2, [pc, #88]	; (30003b14 <BOOT_FaultHandler+0xfc>)
30003aba:	2001      	movs	r0, #1
30003abc:	9300      	str	r3, [sp, #0]
30003abe:	4b0f      	ldr	r3, [pc, #60]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003ac0:	4619      	mov	r1, r3
30003ac2:	f007 fa2b 	bl	3000af1c <rtk_log_write>
30003ac6:	f3ef 8088 	mrs	r0, MSP_NS
30003aca:	f3ef 8189 	mrs	r1, PSP_NS
30003ace:	463b      	mov	r3, r7
30003ad0:	4622      	mov	r2, r4
30003ad2:	f007 fcb1 	bl	3000b438 <__INT_HardFault_C_veneer>
30003ad6:	b015      	add	sp, #84	; 0x54
30003ad8:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003ada:	4a0f      	ldr	r2, [pc, #60]	; (30003b18 <BOOT_FaultHandler+0x100>)
30003adc:	4619      	mov	r1, r3
30003ade:	2001      	movs	r0, #1
30003ae0:	f007 fa1c 	bl	3000af1c <rtk_log_write>
30003ae4:	f3ef 8394 	mrs	r3, CONTROL_NS
30003ae8:	f3ef 8188 	mrs	r1, MSP_NS
30003aec:	f3ef 8589 	mrs	r5, PSP_NS
30003af0:	f014 0208 	ands.w	r2, r4, #8
30003af4:	bf18      	it	ne
30003af6:	f3c3 0240 	ubfxne	r2, r3, #1, #1
30003afa:	e7ad      	b.n	30003a58 <BOOT_FaultHandler+0x40>
30003afc:	3000c714 	.word	0x3000c714
30003b00:	3000c719 	.word	0x3000c719
30003b04:	3000c73c 	.word	0x3000c73c
30003b08:	e000ed00 	.word	0xe000ed00
30003b0c:	3000c78c 	.word	0x3000c78c
30003b10:	3000c7a4 	.word	0x3000c7a4
30003b14:	3000c7bc 	.word	0x3000c7bc
30003b18:	3000c762 	.word	0x3000c762

30003b1c <FLASH_Erase_With_Lock>:
30003b1c:	4b10      	ldr	r3, [pc, #64]	; (30003b60 <FLASH_Erase_With_Lock+0x44>)
30003b1e:	b510      	push	{r4, lr}
30003b20:	460c      	mov	r4, r1
30003b22:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30003b26:	4798      	blx	r3
30003b28:	4a0e      	ldr	r2, [pc, #56]	; (30003b64 <FLASH_Erase_With_Lock+0x48>)
30003b2a:	6953      	ldr	r3, [r2, #20]
30003b2c:	03db      	lsls	r3, r3, #15
30003b2e:	d515      	bpl.n	30003b5c <FLASH_Erase_With_Lock+0x40>
30003b30:	f014 031f 	ands.w	r3, r4, #31
30003b34:	bf1a      	itte	ne
30003b36:	f024 041f 	bicne.w	r4, r4, #31
30003b3a:	f503 5380 	addne.w	r3, r3, #4096	; 0x1000
30003b3e:	f44f 5380 	moveq.w	r3, #4096	; 0x1000
30003b42:	f3bf 8f4f 	dsb	sy
30003b46:	4423      	add	r3, r4
30003b48:	f8c2 425c 	str.w	r4, [r2, #604]	; 0x25c
30003b4c:	3420      	adds	r4, #32
30003b4e:	1b19      	subs	r1, r3, r4
30003b50:	2900      	cmp	r1, #0
30003b52:	dcf9      	bgt.n	30003b48 <FLASH_Erase_With_Lock+0x2c>
30003b54:	f3bf 8f4f 	dsb	sy
30003b58:	f3bf 8f6f 	isb	sy
30003b5c:	bd10      	pop	{r4, pc}
30003b5e:	bf00      	nop
30003b60:	0000a16d 	.word	0x0000a16d
30003b64:	e000ed00 	.word	0xe000ed00

30003b68 <FLASH_TxData_With_Lock>:
30003b68:	b538      	push	{r3, r4, r5, lr}
30003b6a:	4b11      	ldr	r3, [pc, #68]	; (30003bb0 <FLASH_TxData_With_Lock+0x48>)
30003b6c:	4605      	mov	r5, r0
30003b6e:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30003b72:	460c      	mov	r4, r1
30003b74:	4798      	blx	r3
30003b76:	4b0f      	ldr	r3, [pc, #60]	; (30003bb4 <FLASH_TxData_With_Lock+0x4c>)
30003b78:	695b      	ldr	r3, [r3, #20]
30003b7a:	03db      	lsls	r3, r3, #15
30003b7c:	d512      	bpl.n	30003ba4 <FLASH_TxData_With_Lock+0x3c>
30003b7e:	f015 031f 	ands.w	r3, r5, #31
30003b82:	bf1c      	itt	ne
30003b84:	18e4      	addne	r4, r4, r3
30003b86:	f025 051f 	bicne.w	r5, r5, #31
30003b8a:	4621      	mov	r1, r4
30003b8c:	f3bf 8f4f 	dsb	sy
30003b90:	4a08      	ldr	r2, [pc, #32]	; (30003bb4 <FLASH_TxData_With_Lock+0x4c>)
30003b92:	4425      	add	r5, r4
30003b94:	2900      	cmp	r1, #0
30003b96:	eba5 0301 	sub.w	r3, r5, r1
30003b9a:	dc04      	bgt.n	30003ba6 <FLASH_TxData_With_Lock+0x3e>
30003b9c:	f3bf 8f4f 	dsb	sy
30003ba0:	f3bf 8f6f 	isb	sy
30003ba4:	bd38      	pop	{r3, r4, r5, pc}
30003ba6:	3920      	subs	r1, #32
30003ba8:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30003bac:	e7f2      	b.n	30003b94 <FLASH_TxData_With_Lock+0x2c>
30003bae:	bf00      	nop
30003bb0:	0000a04d 	.word	0x0000a04d
30003bb4:	e000ed00 	.word	0xe000ed00

30003bb8 <flash_read_id>:
30003bb8:	4b04      	ldr	r3, [pc, #16]	; (30003bcc <flash_read_id+0x14>)
30003bba:	4602      	mov	r2, r0
30003bbc:	b510      	push	{r4, lr}
30003bbe:	460c      	mov	r4, r1
30003bc0:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30003bc4:	4b02      	ldr	r3, [pc, #8]	; (30003bd0 <flash_read_id+0x18>)
30003bc6:	4798      	blx	r3
30003bc8:	4620      	mov	r0, r4
30003bca:	bd10      	pop	{r4, pc}
30003bcc:	2001c01c 	.word	0x2001c01c
30003bd0:	00009db5 	.word	0x00009db5

30003bd4 <mcc_ReverseBit>:
30003bd4:	0843      	lsrs	r3, r0, #1
30003bd6:	08c2      	lsrs	r2, r0, #3
30003bd8:	f003 0308 	and.w	r3, r3, #8
30003bdc:	f002 0204 	and.w	r2, r2, #4
30003be0:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30003be4:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30003be8:	4313      	orrs	r3, r2
30003bea:	0942      	lsrs	r2, r0, #5
30003bec:	f002 0202 	and.w	r2, r2, #2
30003bf0:	4313      	orrs	r3, r2
30003bf2:	0142      	lsls	r2, r0, #5
30003bf4:	f002 0240 	and.w	r2, r2, #64	; 0x40
30003bf8:	4313      	orrs	r3, r2
30003bfa:	00c2      	lsls	r2, r0, #3
30003bfc:	0040      	lsls	r0, r0, #1
30003bfe:	f002 0220 	and.w	r2, r2, #32
30003c02:	f000 0010 	and.w	r0, r0, #16
30003c06:	4313      	orrs	r3, r2
30003c08:	4318      	orrs	r0, r3
30003c0a:	b2c0      	uxtb	r0, r0
30003c0c:	4770      	bx	lr

30003c0e <mcc_init_crc32>:
30003c0e:	b570      	push	{r4, r5, r6, lr}
30003c10:	4604      	mov	r4, r0
30003c12:	2500      	movs	r5, #0
30003c14:	4e13      	ldr	r6, [pc, #76]	; (30003c64 <mcc_init_crc32+0x56>)
30003c16:	b2e8      	uxtb	r0, r5
30003c18:	f7ff ffdc 	bl	30003bd4 <mcc_ReverseBit>
30003c1c:	2308      	movs	r3, #8
30003c1e:	0601      	lsls	r1, r0, #24
30003c20:	2900      	cmp	r1, #0
30003c22:	ea4f 0141 	mov.w	r1, r1, lsl #1
30003c26:	bfb8      	it	lt
30003c28:	4071      	eorlt	r1, r6
30003c2a:	3b01      	subs	r3, #1
30003c2c:	d1f8      	bne.n	30003c20 <mcc_init_crc32+0x12>
30003c2e:	0e08      	lsrs	r0, r1, #24
30003c30:	3501      	adds	r5, #1
30003c32:	f7ff ffcf 	bl	30003bd4 <mcc_ReverseBit>
30003c36:	7020      	strb	r0, [r4, #0]
30003c38:	f3c1 4007 	ubfx	r0, r1, #16, #8
30003c3c:	3404      	adds	r4, #4
30003c3e:	f7ff ffc9 	bl	30003bd4 <mcc_ReverseBit>
30003c42:	f804 0c03 	strb.w	r0, [r4, #-3]
30003c46:	f3c1 2007 	ubfx	r0, r1, #8, #8
30003c4a:	f7ff ffc3 	bl	30003bd4 <mcc_ReverseBit>
30003c4e:	f804 0c02 	strb.w	r0, [r4, #-2]
30003c52:	b2c8      	uxtb	r0, r1
30003c54:	f7ff ffbe 	bl	30003bd4 <mcc_ReverseBit>
30003c58:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30003c5c:	f804 0c01 	strb.w	r0, [r4, #-1]
30003c60:	d1d9      	bne.n	30003c16 <mcc_init_crc32+0x8>
30003c62:	bd70      	pop	{r4, r5, r6, pc}
30003c64:	04c11db7 	.word	0x04c11db7

30003c68 <mcc_crc32>:
30003c68:	b510      	push	{r4, lr}
30003c6a:	4401      	add	r1, r0
30003c6c:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
30003c70:	4288      	cmp	r0, r1
30003c72:	d101      	bne.n	30003c78 <mcc_crc32+0x10>
30003c74:	43e0      	mvns	r0, r4
30003c76:	bd10      	pop	{r4, pc}
30003c78:	f810 3b01 	ldrb.w	r3, [r0], #1
30003c7c:	4063      	eors	r3, r4
30003c7e:	b2db      	uxtb	r3, r3
30003c80:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30003c84:	ea83 2414 	eor.w	r4, r3, r4, lsr #8
30003c88:	e7f2      	b.n	30003c70 <mcc_crc32+0x8>

30003c8a <recovery_check>:
30003c8a:	b538      	push	{r3, r4, r5, lr}
30003c8c:	4604      	mov	r4, r0
30003c8e:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30003c92:	4608      	mov	r0, r1
30003c94:	4615      	mov	r5, r2
30003c96:	42a3      	cmp	r3, r4
30003c98:	d314      	bcc.n	30003cc4 <recovery_check+0x3a>
30003c9a:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30003c9e:	d911      	bls.n	30003cc4 <recovery_check+0x3a>
30003ca0:	f8d4 300a 	ldr.w	r3, [r4, #10]
30003ca4:	f103 010c 	add.w	r1, r3, #12
30003ca8:	4288      	cmp	r0, r1
30003caa:	d30b      	bcc.n	30003cc4 <recovery_check+0x3a>
30003cac:	4a06      	ldr	r2, [pc, #24]	; (30003cc8 <recovery_check+0x3e>)
30003cae:	1d20      	adds	r0, r4, #4
30003cb0:	f7ff ffda 	bl	30003c68 <mcc_crc32>
30003cb4:	6823      	ldr	r3, [r4, #0]
30003cb6:	4283      	cmp	r3, r0
30003cb8:	d104      	bne.n	30003cc4 <recovery_check+0x3a>
30003cba:	f8d4 3006 	ldr.w	r3, [r4, #6]
30003cbe:	2001      	movs	r0, #1
30003cc0:	602b      	str	r3, [r5, #0]
30003cc2:	bd38      	pop	{r3, r4, r5, pc}
30003cc4:	2000      	movs	r0, #0
30003cc6:	e7fc      	b.n	30003cc2 <recovery_check+0x38>
30003cc8:	3000d718 	.word	0x3000d718

30003ccc <rewrite_bp>:
30003ccc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003cd0:	b0a5      	sub	sp, #148	; 0x94
30003cd2:	4698      	mov	r8, r3
30003cd4:	4616      	mov	r6, r2
30003cd6:	4681      	mov	r9, r0
30003cd8:	f89d 30bc 	ldrb.w	r3, [sp, #188]	; 0xbc
30003cdc:	460d      	mov	r5, r1
30003cde:	2280      	movs	r2, #128	; 0x80
30003ce0:	2b00      	cmp	r3, #0
30003ce2:	d04f      	beq.n	30003d84 <rewrite_bp+0xb8>
30003ce4:	4641      	mov	r1, r8
30003ce6:	2401      	movs	r4, #1
30003ce8:	a804      	add	r0, sp, #16
30003cea:	f007 fb39 	bl	3000b360 <____wrap_memcpy_veneer>
30003cee:	4641      	mov	r1, r8
30003cf0:	2002      	movs	r0, #2
30003cf2:	4f37      	ldr	r7, [pc, #220]	; (30003dd0 <rewrite_bp+0x104>)
30003cf4:	f88d 901c 	strb.w	r9, [sp, #28]
30003cf8:	e9cd 4405 	strd	r4, r4, [sp, #20]
30003cfc:	f7ff ff0e 	bl	30003b1c <FLASH_Erase_With_Lock>
30003d00:	f108 0004 	add.w	r0, r8, #4
30003d04:	aa05      	add	r2, sp, #20
30003d06:	217c      	movs	r1, #124	; 0x7c
30003d08:	9001      	str	r0, [sp, #4]
30003d0a:	f7ff ff2d 	bl	30003b68 <FLASH_TxData_With_Lock>
30003d0e:	463a      	mov	r2, r7
30003d10:	f640 71fc 	movw	r1, #4092	; 0xffc
30003d14:	9801      	ldr	r0, [sp, #4]
30003d16:	f7ff ffa7 	bl	30003c68 <mcc_crc32>
30003d1a:	4641      	mov	r1, r8
30003d1c:	9004      	str	r0, [sp, #16]
30003d1e:	2002      	movs	r0, #2
30003d20:	f7ff fefc 	bl	30003b1c <FLASH_Erase_With_Lock>
30003d24:	aa04      	add	r2, sp, #16
30003d26:	4640      	mov	r0, r8
30003d28:	2180      	movs	r1, #128	; 0x80
30003d2a:	f7ff ff1d 	bl	30003b68 <FLASH_TxData_With_Lock>
30003d2e:	2104      	movs	r1, #4
30003d30:	a803      	add	r0, sp, #12
30003d32:	f7ff ff41 	bl	30003bb8 <flash_read_id>
30003d36:	f89d 800e 	ldrb.w	r8, [sp, #14]
30003d3a:	f8d5 300a 	ldr.w	r3, [r5, #10]
30003d3e:	fa04 f808 	lsl.w	r8, r4, r8
30003d42:	f8d6 200a 	ldr.w	r2, [r6, #10]
30003d46:	330c      	adds	r3, #12
30003d48:	f8d5 a000 	ldr.w	sl, [r5]
30003d4c:	320c      	adds	r2, #12
30003d4e:	f8d6 b000 	ldr.w	fp, [r6]
30003d52:	4543      	cmp	r3, r8
30003d54:	bf28      	it	cs
30003d56:	4643      	movcs	r3, r8
30003d58:	4590      	cmp	r8, r2
30003d5a:	bf28      	it	cs
30003d5c:	4690      	movcs	r8, r2
30003d5e:	f1b9 0f00 	cmp.w	r9, #0
30003d62:	d122      	bne.n	30003daa <rewrite_bp+0xde>
30003d64:	463a      	mov	r2, r7
30003d66:	4619      	mov	r1, r3
30003d68:	1d28      	adds	r0, r5, #4
30003d6a:	f7ff ff7d 	bl	30003c68 <mcc_crc32>
30003d6e:	4582      	cmp	sl, r0
30003d70:	d10a      	bne.n	30003d88 <rewrite_bp+0xbc>
30003d72:	f8c7 5400 	str.w	r5, [r7, #1024]	; 0x400
30003d76:	4817      	ldr	r0, [pc, #92]	; (30003dd4 <rewrite_bp+0x108>)
30003d78:	f8d5 1006 	ldr.w	r1, [r5, #6]
30003d7c:	f007 fb24 	bl	3000b3c8 <__DiagPrintf_veneer>
30003d80:	4620      	mov	r0, r4
30003d82:	e00f      	b.n	30003da4 <rewrite_bp+0xd8>
30003d84:	992e      	ldr	r1, [sp, #184]	; 0xb8
30003d86:	e7ae      	b.n	30003ce6 <rewrite_bp+0x1a>
30003d88:	4641      	mov	r1, r8
30003d8a:	1d30      	adds	r0, r6, #4
30003d8c:	f7ff ff6c 	bl	30003c68 <mcc_crc32>
30003d90:	4583      	cmp	fp, r0
30003d92:	d119      	bne.n	30003dc8 <rewrite_bp+0xfc>
30003d94:	f8c7 6404 	str.w	r6, [r7, #1028]	; 0x404
30003d98:	480f      	ldr	r0, [pc, #60]	; (30003dd8 <rewrite_bp+0x10c>)
30003d9a:	f8d6 1006 	ldr.w	r1, [r6, #6]
30003d9e:	f007 fb13 	bl	3000b3c8 <__DiagPrintf_veneer>
30003da2:	2002      	movs	r0, #2
30003da4:	b025      	add	sp, #148	; 0x94
30003da6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003daa:	463a      	mov	r2, r7
30003dac:	4641      	mov	r1, r8
30003dae:	1d30      	adds	r0, r6, #4
30003db0:	9301      	str	r3, [sp, #4]
30003db2:	f7ff ff59 	bl	30003c68 <mcc_crc32>
30003db6:	4583      	cmp	fp, r0
30003db8:	9b01      	ldr	r3, [sp, #4]
30003dba:	d0eb      	beq.n	30003d94 <rewrite_bp+0xc8>
30003dbc:	4619      	mov	r1, r3
30003dbe:	1d28      	adds	r0, r5, #4
30003dc0:	f7ff ff52 	bl	30003c68 <mcc_crc32>
30003dc4:	4582      	cmp	sl, r0
30003dc6:	d0d4      	beq.n	30003d72 <rewrite_bp+0xa6>
30003dc8:	4804      	ldr	r0, [pc, #16]	; (30003ddc <rewrite_bp+0x110>)
30003dca:	f007 fafd 	bl	3000b3c8 <__DiagPrintf_veneer>
30003dce:	e7fe      	b.n	30003dce <rewrite_bp+0x102>
30003dd0:	3000d718 	.word	0x3000d718
30003dd4:	3000c7d4 	.word	0x3000c7d4
30003dd8:	3000c7eb 	.word	0x3000c7eb
30003ddc:	3000c802 	.word	0x3000c802

30003de0 <BOOT_SectionInit>:
30003de0:	4800      	ldr	r0, [pc, #0]	; (30003de4 <BOOT_SectionInit+0x4>)
30003de2:	4770      	bx	lr
30003de4:	60000020 	.word	0x60000020

30003de8 <BOOT_NsStart>:
30003de8:	b570      	push	{r4, r5, r6, lr}
30003dea:	4605      	mov	r5, r0
30003dec:	4c22      	ldr	r4, [pc, #136]	; (30003e78 <BOOT_NsStart+0x90>)
30003dee:	f384 8808 	msr	MSP, r4
30003df2:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003df6:	2100      	movs	r1, #0
30003df8:	4620      	mov	r0, r4
30003dfa:	4e20      	ldr	r6, [pc, #128]	; (30003e7c <BOOT_NsStart+0x94>)
30003dfc:	47b0      	blx	r6
30003dfe:	4a20      	ldr	r2, [pc, #128]	; (30003e80 <BOOT_NsStart+0x98>)
30003e00:	6953      	ldr	r3, [r2, #20]
30003e02:	03d9      	lsls	r1, r3, #15
30003e04:	d50b      	bpl.n	30003e1e <BOOT_NsStart+0x36>
30003e06:	f3bf 8f4f 	dsb	sy
30003e0a:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
30003e0e:	3420      	adds	r4, #32
30003e10:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
30003e14:	d1f9      	bne.n	30003e0a <BOOT_NsStart+0x22>
30003e16:	f3bf 8f4f 	dsb	sy
30003e1a:	f3bf 8f6f 	isb	sy
30003e1e:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
30003e22:	f383 8808 	msr	MSP, r3
30003e26:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003e2a:	2100      	movs	r1, #0
30003e2c:	4815      	ldr	r0, [pc, #84]	; (30003e84 <BOOT_NsStart+0x9c>)
30003e2e:	47b0      	blx	r6
30003e30:	4a13      	ldr	r2, [pc, #76]	; (30003e80 <BOOT_NsStart+0x98>)
30003e32:	6953      	ldr	r3, [r2, #20]
30003e34:	03db      	lsls	r3, r3, #15
30003e36:	d50c      	bpl.n	30003e52 <BOOT_NsStart+0x6a>
30003e38:	f3bf 8f4f 	dsb	sy
30003e3c:	4b11      	ldr	r3, [pc, #68]	; (30003e84 <BOOT_NsStart+0x9c>)
30003e3e:	490e      	ldr	r1, [pc, #56]	; (30003e78 <BOOT_NsStart+0x90>)
30003e40:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30003e44:	3320      	adds	r3, #32
30003e46:	428b      	cmp	r3, r1
30003e48:	d1fa      	bne.n	30003e40 <BOOT_NsStart+0x58>
30003e4a:	f3bf 8f4f 	dsb	sy
30003e4e:	f3bf 8f6f 	isb	sy
30003e52:	f025 0501 	bic.w	r5, r5, #1
30003e56:	086d      	lsrs	r5, r5, #1
30003e58:	006d      	lsls	r5, r5, #1
30003e5a:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30003e5e:	ed2d 8b10 	vpush	{d8-d15}
30003e62:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
30003e66:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
30003e6a:	47ac      	blxns	r5
30003e6c:	ecbd 8b10 	vpop	{d8-d15}
30003e70:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30003e74:	bd70      	pop	{r4, r5, r6, pc}
30003e76:	bf00      	nop
30003e78:	30002000 	.word	0x30002000
30003e7c:	00012be5 	.word	0x00012be5
30003e80:	e000ed00 	.word	0xe000ed00
30003e84:	30001000 	.word	0x30001000

30003e88 <BOOT_RccConfig>:
30003e88:	b5f0      	push	{r4, r5, r6, r7, lr}
30003e8a:	4c30      	ldr	r4, [pc, #192]	; (30003f4c <BOOT_RccConfig+0xc4>)
30003e8c:	b091      	sub	sp, #68	; 0x44
30003e8e:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
30003e92:	466e      	mov	r6, sp
30003e94:	3410      	adds	r4, #16
30003e96:	ad08      	add	r5, sp, #32
30003e98:	e886 000f 	stmia.w	r6, {r0, r1, r2, r3}
30003e9c:	2210      	movs	r2, #16
30003e9e:	2100      	movs	r1, #0
30003ea0:	eb0d 0002 	add.w	r0, sp, r2
30003ea4:	f007 fa8c 	bl	3000b3c0 <____wrap_memset_veneer>
30003ea8:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
30003eac:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30003eb0:	2210      	movs	r2, #16
30003eb2:	2100      	movs	r1, #0
30003eb4:	a80c      	add	r0, sp, #48	; 0x30
30003eb6:	f007 fa83 	bl	3000b3c0 <____wrap_memset_veneer>
30003eba:	4b25      	ldr	r3, [pc, #148]	; (30003f50 <BOOT_RccConfig+0xc8>)
30003ebc:	6818      	ldr	r0, [r3, #0]
30003ebe:	1c42      	adds	r2, r0, #1
30003ec0:	d017      	beq.n	30003ef2 <BOOT_RccConfig+0x6a>
30003ec2:	689a      	ldr	r2, [r3, #8]
30003ec4:	2a01      	cmp	r2, #1
30003ec6:	d112      	bne.n	30003eee <BOOT_RccConfig+0x66>
30003ec8:	685f      	ldr	r7, [r3, #4]
30003eca:	ac10      	add	r4, sp, #64	; 0x40
30003ecc:	0f82      	lsrs	r2, r0, #30
30003ece:	0fb9      	lsrs	r1, r7, #30
30003ed0:	eb04 0181 	add.w	r1, r4, r1, lsl #2
30003ed4:	f851 4c10 	ldr.w	r4, [r1, #-16]
30003ed8:	433c      	orrs	r4, r7
30003eda:	f841 4c10 	str.w	r4, [r1, #-16]
30003ede:	a910      	add	r1, sp, #64	; 0x40
30003ee0:	eb01 0282 	add.w	r2, r1, r2, lsl #2
30003ee4:	f852 1c30 	ldr.w	r1, [r2, #-48]
30003ee8:	4301      	orrs	r1, r0
30003eea:	f842 1c30 	str.w	r1, [r2, #-48]
30003eee:	330c      	adds	r3, #12
30003ef0:	e7e4      	b.n	30003ebc <BOOT_RccConfig+0x34>
30003ef2:	462a      	mov	r2, r5
30003ef4:	4633      	mov	r3, r6
30003ef6:	ad0c      	add	r5, sp, #48	; 0x30
30003ef8:	ac04      	add	r4, sp, #16
30003efa:	2600      	movs	r6, #0
30003efc:	f852 1b04 	ldr.w	r1, [r2], #4
30003f00:	3601      	adds	r6, #1
30003f02:	f855 0b04 	ldr.w	r0, [r5], #4
30003f06:	f101 4184 	add.w	r1, r1, #1107296256	; 0x42000000
30003f0a:	2e04      	cmp	r6, #4
30003f0c:	f020 4040 	bic.w	r0, r0, #3221225472	; 0xc0000000
30003f10:	f501 4100 	add.w	r1, r1, #32768	; 0x8000
30003f14:	680f      	ldr	r7, [r1, #0]
30003f16:	ea40 0007 	orr.w	r0, r0, r7
30003f1a:	6008      	str	r0, [r1, #0]
30003f1c:	f853 1b04 	ldr.w	r1, [r3], #4
30003f20:	f854 0b04 	ldr.w	r0, [r4], #4
30003f24:	f101 4184 	add.w	r1, r1, #1107296256	; 0x42000000
30003f28:	f020 4040 	bic.w	r0, r0, #3221225472	; 0xc0000000
30003f2c:	f501 4100 	add.w	r1, r1, #32768	; 0x8000
30003f30:	680f      	ldr	r7, [r1, #0]
30003f32:	ea40 0007 	orr.w	r0, r0, r7
30003f36:	6008      	str	r0, [r1, #0]
30003f38:	d1e0      	bne.n	30003efc <BOOT_RccConfig+0x74>
30003f3a:	200a      	movs	r0, #10
30003f3c:	4b05      	ldr	r3, [pc, #20]	; (30003f54 <BOOT_RccConfig+0xcc>)
30003f3e:	4798      	blx	r3
30003f40:	4b05      	ldr	r3, [pc, #20]	; (30003f58 <BOOT_RccConfig+0xd0>)
30003f42:	b011      	add	sp, #68	; 0x44
30003f44:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30003f48:	4718      	bx	r3
30003f4a:	bf00      	nop
30003f4c:	3000cf10 	.word	0x3000cf10
30003f50:	3000d55c 	.word	0x3000d55c
30003f54:	00009b2d 	.word	0x00009b2d
30003f58:	300073a9 	.word	0x300073a9

30003f5c <BOOT_CACHEWRR_Set>:
30003f5c:	4b14      	ldr	r3, [pc, #80]	; (30003fb0 <BOOT_CACHEWRR_Set+0x54>)
30003f5e:	b530      	push	{r4, r5, lr}
30003f60:	681a      	ldr	r2, [r3, #0]
30003f62:	2a01      	cmp	r2, #1
30003f64:	d10f      	bne.n	30003f86 <BOOT_CACHEWRR_Set+0x2a>
30003f66:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30003f6a:	685d      	ldr	r5, [r3, #4]
30003f6c:	688a      	ldr	r2, [r1, #8]
30003f6e:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30003f72:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
30003f76:	e9d3 4002 	ldrd	r4, r0, [r3, #8]
30003f7a:	608a      	str	r2, [r1, #8]
30003f7c:	4a0d      	ldr	r2, [pc, #52]	; (30003fb4 <BOOT_CACHEWRR_Set+0x58>)
30003f7e:	f8c2 40a0 	str.w	r4, [r2, #160]	; 0xa0
30003f82:	f8c2 00a4 	str.w	r0, [r2, #164]	; 0xa4
30003f86:	691a      	ldr	r2, [r3, #16]
30003f88:	2a01      	cmp	r2, #1
30003f8a:	d10f      	bne.n	30003fac <BOOT_CACHEWRR_Set+0x50>
30003f8c:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30003f90:	688a      	ldr	r2, [r1, #8]
30003f92:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30003f96:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
30003f9a:	695b      	ldr	r3, [r3, #20]
30003f9c:	ea42 5303 	orr.w	r3, r2, r3, lsl #20
30003fa0:	608b      	str	r3, [r1, #8]
30003fa2:	4b04      	ldr	r3, [pc, #16]	; (30003fb4 <BOOT_CACHEWRR_Set+0x58>)
30003fa4:	f8c3 40a8 	str.w	r4, [r3, #168]	; 0xa8
30003fa8:	f8c3 00ac 	str.w	r0, [r3, #172]	; 0xac
30003fac:	bd30      	pop	{r4, r5, pc}
30003fae:	bf00      	nop
30003fb0:	3000dd7c 	.word	0x3000dd7c
30003fb4:	e0042000 	.word	0xe0042000

30003fb8 <BOOT_TCMSet>:
30003fb8:	b5f0      	push	{r4, r5, r6, r7, lr}
30003fba:	2300      	movs	r3, #0
30003fbc:	221c      	movs	r2, #28
30003fbe:	4caa      	ldr	r4, [pc, #680]	; (30004268 <BOOT_TCMSet+0x2b0>)
30003fc0:	fb02 f103 	mul.w	r1, r2, r3
30003fc4:	5909      	ldr	r1, [r1, r4]
30003fc6:	4281      	cmp	r1, r0
30003fc8:	d03b      	beq.n	30004042 <BOOT_TCMSet+0x8a>
30003fca:	3301      	adds	r3, #1
30003fcc:	2b07      	cmp	r3, #7
30003fce:	d1f7      	bne.n	30003fc0 <BOOT_TCMSet+0x8>
30003fd0:	f3bf 8f4f 	dsb	sy
30003fd4:	f3bf 8f6f 	isb	sy
30003fd8:	4aa4      	ldr	r2, [pc, #656]	; (3000426c <BOOT_TCMSet+0x2b4>)
30003fda:	2100      	movs	r1, #0
30003fdc:	f8c2 1250 	str.w	r1, [r2, #592]	; 0x250
30003fe0:	f3bf 8f4f 	dsb	sy
30003fe4:	f3bf 8f6f 	isb	sy
30003fe8:	6953      	ldr	r3, [r2, #20]
30003fea:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
30003fee:	6153      	str	r3, [r2, #20]
30003ff0:	f3bf 8f4f 	dsb	sy
30003ff4:	f3bf 8f6f 	isb	sy
30003ff8:	f8c2 1084 	str.w	r1, [r2, #132]	; 0x84
30003ffc:	f3bf 8f4f 	dsb	sy
30004000:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30004004:	f643 74e0 	movw	r4, #16352	; 0x3fe0
30004008:	f3c3 00c9 	ubfx	r0, r3, #3, #10
3000400c:	f3c3 334e 	ubfx	r3, r3, #13, #15
30004010:	015b      	lsls	r3, r3, #5
30004012:	ea03 0604 	and.w	r6, r3, r4
30004016:	4601      	mov	r1, r0
30004018:	ea46 7581 	orr.w	r5, r6, r1, lsl #30
3000401c:	3901      	subs	r1, #1
3000401e:	f8c2 5260 	str.w	r5, [r2, #608]	; 0x260
30004022:	d2f9      	bcs.n	30004018 <BOOT_TCMSet+0x60>
30004024:	3b20      	subs	r3, #32
30004026:	f113 0f20 	cmn.w	r3, #32
3000402a:	d1f2      	bne.n	30004012 <BOOT_TCMSet+0x5a>
3000402c:	f3bf 8f4f 	dsb	sy
30004030:	6953      	ldr	r3, [r2, #20]
30004032:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30004036:	6153      	str	r3, [r2, #20]
30004038:	f3bf 8f4f 	dsb	sy
3000403c:	f3bf 8f6f 	isb	sy
30004040:	bdf0      	pop	{r4, r5, r6, r7, pc}
30004042:	2b00      	cmp	r3, #0
30004044:	d0c4      	beq.n	30003fd0 <BOOT_TCMSet+0x18>
30004046:	f3bf 8f4f 	dsb	sy
3000404a:	f3bf 8f6f 	isb	sy
3000404e:	4988      	ldr	r1, [pc, #544]	; (30004270 <BOOT_TCMSet+0x2b8>)
30004050:	2200      	movs	r2, #0
30004052:	f8c1 2250 	str.w	r2, [r1, #592]	; 0x250
30004056:	f3bf 8f4f 	dsb	sy
3000405a:	f3bf 8f6f 	isb	sy
3000405e:	6948      	ldr	r0, [r1, #20]
30004060:	03c0      	lsls	r0, r0, #15
30004062:	d51d      	bpl.n	300040a0 <BOOT_TCMSet+0xe8>
30004064:	f8c1 2084 	str.w	r2, [r1, #132]	; 0x84
30004068:	f3bf 8f4f 	dsb	sy
3000406c:	f8d1 2080 	ldr.w	r2, [r1, #128]	; 0x80
30004070:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004074:	f3c2 05c9 	ubfx	r5, r2, #3, #10
30004078:	f3c2 324e 	ubfx	r2, r2, #13, #15
3000407c:	0152      	lsls	r2, r2, #5
3000407e:	ea02 0c06 	and.w	ip, r2, r6
30004082:	4628      	mov	r0, r5
30004084:	ea4c 7780 	orr.w	r7, ip, r0, lsl #30
30004088:	3801      	subs	r0, #1
3000408a:	f8c1 7274 	str.w	r7, [r1, #628]	; 0x274
3000408e:	d2f9      	bcs.n	30004084 <BOOT_TCMSet+0xcc>
30004090:	3a20      	subs	r2, #32
30004092:	f112 0f20 	cmn.w	r2, #32
30004096:	d1f2      	bne.n	3000407e <BOOT_TCMSet+0xc6>
30004098:	f3bf 8f4f 	dsb	sy
3000409c:	f3bf 8f6f 	isb	sy
300040a0:	f3bf 8f4f 	dsb	sy
300040a4:	f3bf 8f6f 	isb	sy
300040a8:	4971      	ldr	r1, [pc, #452]	; (30004270 <BOOT_TCMSet+0x2b8>)
300040aa:	694a      	ldr	r2, [r1, #20]
300040ac:	f422 3200 	bic.w	r2, r2, #131072	; 0x20000
300040b0:	614a      	str	r2, [r1, #20]
300040b2:	2200      	movs	r2, #0
300040b4:	f8c1 2250 	str.w	r2, [r1, #592]	; 0x250
300040b8:	f3bf 8f4f 	dsb	sy
300040bc:	f3bf 8f6f 	isb	sy
300040c0:	f8c1 2084 	str.w	r2, [r1, #132]	; 0x84
300040c4:	f3bf 8f4f 	dsb	sy
300040c8:	694a      	ldr	r2, [r1, #20]
300040ca:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
300040ce:	614a      	str	r2, [r1, #20]
300040d0:	f3bf 8f4f 	dsb	sy
300040d4:	f8d1 2080 	ldr.w	r2, [r1, #128]	; 0x80
300040d8:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300040dc:	f3c2 05c9 	ubfx	r5, r2, #3, #10
300040e0:	f3c2 324e 	ubfx	r2, r2, #13, #15
300040e4:	0152      	lsls	r2, r2, #5
300040e6:	ea02 0c06 	and.w	ip, r2, r6
300040ea:	4628      	mov	r0, r5
300040ec:	ea4c 7780 	orr.w	r7, ip, r0, lsl #30
300040f0:	3801      	subs	r0, #1
300040f2:	f8c1 7274 	str.w	r7, [r1, #628]	; 0x274
300040f6:	d2f9      	bcs.n	300040ec <BOOT_TCMSet+0x134>
300040f8:	3a20      	subs	r2, #32
300040fa:	f112 0f20 	cmn.w	r2, #32
300040fe:	d1f2      	bne.n	300040e6 <BOOT_TCMSet+0x12e>
30004100:	f3bf 8f4f 	dsb	sy
30004104:	f3bf 8f6f 	isb	sy
30004108:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
3000410c:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000410e:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
30004112:	634a      	str	r2, [r1, #52]	; 0x34
30004114:	221c      	movs	r2, #28
30004116:	fb02 4203 	mla	r2, r2, r3, r4
3000411a:	6910      	ldr	r0, [r2, #16]
3000411c:	2803      	cmp	r0, #3
3000411e:	d00f      	beq.n	30004140 <BOOT_TCMSet+0x188>
30004120:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30004124:	688a      	ldr	r2, [r1, #8]
30004126:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
3000412a:	ea42 5200 	orr.w	r2, r2, r0, lsl #20
3000412e:	608a      	str	r2, [r1, #8]
30004130:	2100      	movs	r1, #0
30004132:	4a50      	ldr	r2, [pc, #320]	; (30004274 <BOOT_TCMSet+0x2bc>)
30004134:	f8c2 10a8 	str.w	r1, [r2, #168]	; 0xa8
30004138:	f5a1 3180 	sub.w	r1, r1, #65536	; 0x10000
3000413c:	f8c2 10ac 	str.w	r1, [r2, #172]	; 0xac
30004140:	221c      	movs	r2, #28
30004142:	fb02 4203 	mla	r2, r2, r3, r4
30004146:	68d0      	ldr	r0, [r2, #12]
30004148:	2803      	cmp	r0, #3
3000414a:	d00f      	beq.n	3000416c <BOOT_TCMSet+0x1b4>
3000414c:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30004150:	688a      	ldr	r2, [r1, #8]
30004152:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004156:	ea42 3200 	orr.w	r2, r2, r0, lsl #12
3000415a:	608a      	str	r2, [r1, #8]
3000415c:	2100      	movs	r1, #0
3000415e:	4a45      	ldr	r2, [pc, #276]	; (30004274 <BOOT_TCMSet+0x2bc>)
30004160:	f8c2 10a0 	str.w	r1, [r2, #160]	; 0xa0
30004164:	f5a1 3180 	sub.w	r1, r1, #65536	; 0x10000
30004168:	f8c2 10a4 	str.w	r1, [r2, #164]	; 0xa4
3000416c:	221c      	movs	r2, #28
3000416e:	4941      	ldr	r1, [pc, #260]	; (30004274 <BOOT_TCMSet+0x2bc>)
30004170:	fb02 4203 	mla	r2, r2, r3, r4
30004174:	6950      	ldr	r0, [r2, #20]
30004176:	6608      	str	r0, [r1, #96]	; 0x60
30004178:	6990      	ldr	r0, [r2, #24]
3000417a:	6648      	str	r0, [r1, #100]	; 0x64
3000417c:	6852      	ldr	r2, [r2, #4]
3000417e:	2a01      	cmp	r2, #1
30004180:	d127      	bne.n	300041d2 <BOOT_TCMSet+0x21a>
30004182:	f3bf 8f4f 	dsb	sy
30004186:	f3bf 8f6f 	isb	sy
3000418a:	4a38      	ldr	r2, [pc, #224]	; (3000426c <BOOT_TCMSet+0x2b4>)
3000418c:	2000      	movs	r0, #0
3000418e:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30004192:	f3bf 8f4f 	dsb	sy
30004196:	f3bf 8f6f 	isb	sy
3000419a:	6951      	ldr	r1, [r2, #20]
3000419c:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300041a0:	6151      	str	r1, [r2, #20]
300041a2:	f3bf 8f4f 	dsb	sy
300041a6:	f3bf 8f6f 	isb	sy
300041aa:	f3bf 8f4f 	dsb	sy
300041ae:	f3bf 8f6f 	isb	sy
300041b2:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
300041b6:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300041ba:	f3bf 8f4f 	dsb	sy
300041be:	f3bf 8f6f 	isb	sy
300041c2:	6951      	ldr	r1, [r2, #20]
300041c4:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300041c8:	6151      	str	r1, [r2, #20]
300041ca:	f3bf 8f4f 	dsb	sy
300041ce:	f3bf 8f6f 	isb	sy
300041d2:	221c      	movs	r2, #28
300041d4:	fb02 4303 	mla	r3, r2, r3, r4
300041d8:	689b      	ldr	r3, [r3, #8]
300041da:	2b01      	cmp	r3, #1
300041dc:	f47f af30 	bne.w	30004040 <BOOT_TCMSet+0x88>
300041e0:	4a22      	ldr	r2, [pc, #136]	; (3000426c <BOOT_TCMSet+0x2b4>)
300041e2:	2300      	movs	r3, #0
300041e4:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300041e8:	f3bf 8f4f 	dsb	sy
300041ec:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
300041f0:	f643 74e0 	movw	r4, #16352	; 0x3fe0
300041f4:	f3c3 00c9 	ubfx	r0, r3, #3, #10
300041f8:	f3c3 334e 	ubfx	r3, r3, #13, #15
300041fc:	015b      	lsls	r3, r3, #5
300041fe:	ea03 0604 	and.w	r6, r3, r4
30004202:	4601      	mov	r1, r0
30004204:	ea46 7581 	orr.w	r5, r6, r1, lsl #30
30004208:	3901      	subs	r1, #1
3000420a:	f8c2 5260 	str.w	r5, [r2, #608]	; 0x260
3000420e:	d2f9      	bcs.n	30004204 <BOOT_TCMSet+0x24c>
30004210:	3b20      	subs	r3, #32
30004212:	f113 0f20 	cmn.w	r3, #32
30004216:	d1f2      	bne.n	300041fe <BOOT_TCMSet+0x246>
30004218:	f3bf 8f4f 	dsb	sy
3000421c:	6953      	ldr	r3, [r2, #20]
3000421e:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30004222:	6153      	str	r3, [r2, #20]
30004224:	f3bf 8f4f 	dsb	sy
30004228:	f3bf 8f6f 	isb	sy
3000422c:	4a10      	ldr	r2, [pc, #64]	; (30004270 <BOOT_TCMSet+0x2b8>)
3000422e:	2300      	movs	r3, #0
30004230:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30004234:	f3bf 8f4f 	dsb	sy
30004238:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000423c:	f643 74e0 	movw	r4, #16352	; 0x3fe0
30004240:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30004244:	f3c3 334e 	ubfx	r3, r3, #13, #15
30004248:	015b      	lsls	r3, r3, #5
3000424a:	ea03 0604 	and.w	r6, r3, r4
3000424e:	4601      	mov	r1, r0
30004250:	ea46 7581 	orr.w	r5, r6, r1, lsl #30
30004254:	3901      	subs	r1, #1
30004256:	f8c2 5260 	str.w	r5, [r2, #608]	; 0x260
3000425a:	d2f9      	bcs.n	30004250 <BOOT_TCMSet+0x298>
3000425c:	3b20      	subs	r3, #32
3000425e:	f113 0f20 	cmn.w	r3, #32
30004262:	d1f2      	bne.n	3000424a <BOOT_TCMSet+0x292>
30004264:	e6e2      	b.n	3000402c <BOOT_TCMSet+0x74>
30004266:	bf00      	nop
30004268:	3000dcb8 	.word	0x3000dcb8
3000426c:	e002ed00 	.word	0xe002ed00
30004270:	e000ed00 	.word	0xe000ed00
30004274:	e0042000 	.word	0xe0042000

30004278 <BOOT_GRstConfig>:
30004278:	b538      	push	{r3, r4, r5, lr}
3000427a:	4d09      	ldr	r5, [pc, #36]	; (300042a0 <BOOT_GRstConfig+0x28>)
3000427c:	2028      	movs	r0, #40	; 0x28
3000427e:	4b09      	ldr	r3, [pc, #36]	; (300042a4 <BOOT_GRstConfig+0x2c>)
30004280:	882c      	ldrh	r4, [r5, #0]
30004282:	b2a4      	uxth	r4, r4
30004284:	f444 5480 	orr.w	r4, r4, #4096	; 0x1000
30004288:	f044 040a 	orr.w	r4, r4, #10
3000428c:	802c      	strh	r4, [r5, #0]
3000428e:	4798      	blx	r3
30004290:	88ab      	ldrh	r3, [r5, #4]
30004292:	b29b      	uxth	r3, r3
30004294:	ea23 0304 	bic.w	r3, r3, r4
30004298:	80ab      	strh	r3, [r5, #4]
3000429a:	886b      	ldrh	r3, [r5, #2]
3000429c:	806c      	strh	r4, [r5, #2]
3000429e:	bd38      	pop	{r3, r4, r5, pc}
300042a0:	42008200 	.word	0x42008200
300042a4:	00009b2d 	.word	0x00009b2d

300042a8 <BOOT_DDR_LCDC_HPR>:
300042a8:	4a04      	ldr	r2, [pc, #16]	; (300042bc <BOOT_DDR_LCDC_HPR+0x14>)
300042aa:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300042ae:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300042b2:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300042b6:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300042ba:	4770      	bx	lr
300042bc:	42008000 	.word	0x42008000

300042c0 <BOOT_DDR_Init>:
300042c0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300042c4:	b530      	push	{r4, r5, lr}
300042c6:	4d15      	ldr	r5, [pc, #84]	; (3000431c <BOOT_DDR_Init+0x5c>)
300042c8:	b099      	sub	sp, #100	; 0x64
300042ca:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300042cc:	47a8      	blx	r5
300042ce:	2803      	cmp	r0, #3
300042d0:	d11e      	bne.n	30004310 <BOOT_DDR_Init+0x50>
300042d2:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300042d6:	4a12      	ldr	r2, [pc, #72]	; (30004320 <BOOT_DDR_Init+0x60>)
300042d8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300042dc:	a801      	add	r0, sp, #4
300042de:	631c      	str	r4, [r3, #48]	; 0x30
300042e0:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300042e4:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300042e8:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300042ec:	4b0d      	ldr	r3, [pc, #52]	; (30004324 <BOOT_DDR_Init+0x64>)
300042ee:	4798      	blx	r3
300042f0:	a801      	add	r0, sp, #4
300042f2:	4b0d      	ldr	r3, [pc, #52]	; (30004328 <BOOT_DDR_Init+0x68>)
300042f4:	4798      	blx	r3
300042f6:	4b0d      	ldr	r3, [pc, #52]	; (3000432c <BOOT_DDR_Init+0x6c>)
300042f8:	4798      	blx	r3
300042fa:	4b0d      	ldr	r3, [pc, #52]	; (30004330 <BOOT_DDR_Init+0x70>)
300042fc:	4798      	blx	r3
300042fe:	4b0d      	ldr	r3, [pc, #52]	; (30004334 <BOOT_DDR_Init+0x74>)
30004300:	4798      	blx	r3
30004302:	2101      	movs	r1, #1
30004304:	f240 10ff 	movw	r0, #511	; 0x1ff
30004308:	4b0b      	ldr	r3, [pc, #44]	; (30004338 <BOOT_DDR_Init+0x78>)
3000430a:	4798      	blx	r3
3000430c:	b019      	add	sp, #100	; 0x64
3000430e:	bd30      	pop	{r4, r5, pc}
30004310:	47a8      	blx	r5
30004312:	2802      	cmp	r0, #2
30004314:	d0dd      	beq.n	300042d2 <BOOT_DDR_Init+0x12>
30004316:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
3000431a:	e7dc      	b.n	300042d6 <BOOT_DDR_Init+0x16>
3000431c:	0000c0f9 	.word	0x0000c0f9
30004320:	42008000 	.word	0x42008000
30004324:	30007495 	.word	0x30007495
30004328:	30007915 	.word	0x30007915
3000432c:	3000a489 	.word	0x3000a489
30004330:	300079a5 	.word	0x300079a5
30004334:	300079a9 	.word	0x300079a9
30004338:	3000a439 	.word	0x3000a439

3000433c <BOOT_PSRAM_Init>:
3000433c:	b530      	push	{r4, r5, lr}
3000433e:	b089      	sub	sp, #36	; 0x24
30004340:	4b17      	ldr	r3, [pc, #92]	; (300043a0 <BOOT_PSRAM_Init+0x64>)
30004342:	a801      	add	r0, sp, #4
30004344:	4798      	blx	r3
30004346:	a801      	add	r0, sp, #4
30004348:	4b16      	ldr	r3, [pc, #88]	; (300043a4 <BOOT_PSRAM_Init+0x68>)
3000434a:	4798      	blx	r3
3000434c:	4b16      	ldr	r3, [pc, #88]	; (300043a8 <BOOT_PSRAM_Init+0x6c>)
3000434e:	4798      	blx	r3
30004350:	4b16      	ldr	r3, [pc, #88]	; (300043ac <BOOT_PSRAM_Init+0x70>)
30004352:	4798      	blx	r3
30004354:	2801      	cmp	r0, #1
30004356:	d116      	bne.n	30004386 <BOOT_PSRAM_Init+0x4a>
30004358:	4b15      	ldr	r3, [pc, #84]	; (300043b0 <BOOT_PSRAM_Init+0x74>)
3000435a:	4798      	blx	r3
3000435c:	4b15      	ldr	r3, [pc, #84]	; (300043b4 <BOOT_PSRAM_Init+0x78>)
3000435e:	4c16      	ldr	r4, [pc, #88]	; (300043b8 <BOOT_PSRAM_Init+0x7c>)
30004360:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004364:	4d15      	ldr	r5, [pc, #84]	; (300043bc <BOOT_PSRAM_Init+0x80>)
30004366:	f013 5f00 	tst.w	r3, #536870912	; 0x20000000
3000436a:	d014      	beq.n	30004396 <BOOT_PSRAM_Init+0x5a>
3000436c:	6823      	ldr	r3, [r4, #0]
3000436e:	606b      	str	r3, [r5, #4]
30004370:	4b13      	ldr	r3, [pc, #76]	; (300043c0 <BOOT_PSRAM_Init+0x84>)
30004372:	2220      	movs	r2, #32
30004374:	2101      	movs	r1, #1
30004376:	695b      	ldr	r3, [r3, #20]
30004378:	4608      	mov	r0, r1
3000437a:	fbb2 f2f3 	udiv	r2, r2, r3
3000437e:	4b11      	ldr	r3, [pc, #68]	; (300043c4 <BOOT_PSRAM_Init+0x88>)
30004380:	4798      	blx	r3
30004382:	b009      	add	sp, #36	; 0x24
30004384:	bd30      	pop	{r4, r5, pc}
30004386:	4b10      	ldr	r3, [pc, #64]	; (300043c8 <BOOT_PSRAM_Init+0x8c>)
30004388:	2004      	movs	r0, #4
3000438a:	4a10      	ldr	r2, [pc, #64]	; (300043cc <BOOT_PSRAM_Init+0x90>)
3000438c:	4619      	mov	r1, r3
3000438e:	f006 fdc5 	bl	3000af1c <rtk_log_write>
30004392:	4b0f      	ldr	r3, [pc, #60]	; (300043d0 <BOOT_PSRAM_Init+0x94>)
30004394:	e7e1      	b.n	3000435a <BOOT_PSRAM_Init+0x1e>
30004396:	4b0f      	ldr	r3, [pc, #60]	; (300043d4 <BOOT_PSRAM_Init+0x98>)
30004398:	4798      	blx	r3
3000439a:	686b      	ldr	r3, [r5, #4]
3000439c:	6023      	str	r3, [r4, #0]
3000439e:	e7e7      	b.n	30004370 <BOOT_PSRAM_Init+0x34>
300043a0:	30008219 	.word	0x30008219
300043a4:	30008235 	.word	0x30008235
300043a8:	30008299 	.word	0x30008299
300043ac:	3000735d 	.word	0x3000735d
300043b0:	30008469 	.word	0x30008469
300043b4:	42008000 	.word	0x42008000
300043b8:	3000ebe8 	.word	0x3000ebe8
300043bc:	41002000 	.word	0x41002000
300043c0:	3000eae0 	.word	0x3000eae0
300043c4:	30008805 	.word	0x30008805
300043c8:	3000c714 	.word	0x3000c714
300043cc:	3000c820 	.word	0x3000c820
300043d0:	30008531 	.word	0x30008531
300043d4:	3000856d 	.word	0x3000856d

300043d8 <BOOT_SCBConfig_HP>:
300043d8:	4b0e      	ldr	r3, [pc, #56]	; (30004414 <BOOT_SCBConfig_HP+0x3c>)
300043da:	6a5a      	ldr	r2, [r3, #36]	; 0x24
300043dc:	f442 2270 	orr.w	r2, r2, #983040	; 0xf0000
300043e0:	625a      	str	r2, [r3, #36]	; 0x24
300043e2:	4a0d      	ldr	r2, [pc, #52]	; (30004418 <BOOT_SCBConfig_HP+0x40>)
300043e4:	6a51      	ldr	r1, [r2, #36]	; 0x24
300043e6:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
300043ea:	6251      	str	r1, [r2, #36]	; 0x24
300043ec:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
300043f0:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300043f4:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
300043f8:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
300043fc:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004400:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004404:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004408:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
3000440c:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30004410:	4770      	bx	lr
30004412:	bf00      	nop
30004414:	e000ed00 	.word	0xe000ed00
30004418:	e002ed00 	.word	0xe002ed00

3000441c <BOOT_SCBVTORBackup_HP>:
3000441c:	4b02      	ldr	r3, [pc, #8]	; (30004428 <BOOT_SCBVTORBackup_HP+0xc>)
3000441e:	689a      	ldr	r2, [r3, #8]
30004420:	4b02      	ldr	r3, [pc, #8]	; (3000442c <BOOT_SCBVTORBackup_HP+0x10>)
30004422:	619a      	str	r2, [r3, #24]
30004424:	4770      	bx	lr
30004426:	bf00      	nop
30004428:	e000ed00 	.word	0xe000ed00
3000442c:	3000eb7c 	.word	0x3000eb7c

30004430 <BOOT_SCBVTORReFill_HP>:
30004430:	4b02      	ldr	r3, [pc, #8]	; (3000443c <BOOT_SCBVTORReFill_HP+0xc>)
30004432:	699a      	ldr	r2, [r3, #24]
30004434:	4b02      	ldr	r3, [pc, #8]	; (30004440 <BOOT_SCBVTORReFill_HP+0x10>)
30004436:	609a      	str	r2, [r3, #8]
30004438:	4770      	bx	lr
3000443a:	bf00      	nop
3000443c:	3000eb7c 	.word	0x3000eb7c
30004440:	e000ed00 	.word	0xe000ed00

30004444 <BOOT_NVICBackup_HP>:
30004444:	4917      	ldr	r1, [pc, #92]	; (300044a4 <BOOT_NVICBackup_HP+0x60>)
30004446:	4a18      	ldr	r2, [pc, #96]	; (300044a8 <BOOT_NVICBackup_HP+0x64>)
30004448:	b510      	push	{r4, lr}
3000444a:	680b      	ldr	r3, [r1, #0]
3000444c:	f102 001c 	add.w	r0, r2, #28
30004450:	6013      	str	r3, [r2, #0]
30004452:	684b      	ldr	r3, [r1, #4]
30004454:	6053      	str	r3, [r2, #4]
30004456:	688b      	ldr	r3, [r1, #8]
30004458:	6093      	str	r3, [r2, #8]
3000445a:	2300      	movs	r3, #0
3000445c:	18cc      	adds	r4, r1, r3
3000445e:	3301      	adds	r3, #1
30004460:	f894 4300 	ldrb.w	r4, [r4, #768]	; 0x300
30004464:	2b50      	cmp	r3, #80	; 0x50
30004466:	f800 4b01 	strb.w	r4, [r0], #1
3000446a:	d1f7      	bne.n	3000445c <BOOT_NVICBackup_HP+0x18>
3000446c:	480f      	ldr	r0, [pc, #60]	; (300044ac <BOOT_NVICBackup_HP+0x68>)
3000446e:	6943      	ldr	r3, [r0, #20]
30004470:	03db      	lsls	r3, r3, #15
30004472:	d515      	bpl.n	300044a0 <BOOT_NVICBackup_HP+0x5c>
30004474:	f012 011f 	ands.w	r1, r2, #31
30004478:	4b0b      	ldr	r3, [pc, #44]	; (300044a8 <BOOT_NVICBackup_HP+0x64>)
3000447a:	bf0e      	itee	eq
3000447c:	226c      	moveq	r2, #108	; 0x6c
3000447e:	f022 031f 	bicne.w	r3, r2, #31
30004482:	f101 026c 	addne.w	r2, r1, #108	; 0x6c
30004486:	f3bf 8f4f 	dsb	sy
3000448a:	441a      	add	r2, r3
3000448c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30004490:	3320      	adds	r3, #32
30004492:	1ad1      	subs	r1, r2, r3
30004494:	2900      	cmp	r1, #0
30004496:	dcf9      	bgt.n	3000448c <BOOT_NVICBackup_HP+0x48>
30004498:	f3bf 8f4f 	dsb	sy
3000449c:	f3bf 8f6f 	isb	sy
300044a0:	bd10      	pop	{r4, pc}
300044a2:	bf00      	nop
300044a4:	e000e100 	.word	0xe000e100
300044a8:	3000eb7c 	.word	0x3000eb7c
300044ac:	e000ed00 	.word	0xe000ed00

300044b0 <BOOT_NVICReFill_HP>:
300044b0:	490a      	ldr	r1, [pc, #40]	; (300044dc <BOOT_NVICReFill_HP+0x2c>)
300044b2:	2200      	movs	r2, #0
300044b4:	4b0a      	ldr	r3, [pc, #40]	; (300044e0 <BOOT_NVICReFill_HP+0x30>)
300044b6:	b510      	push	{r4, lr}
300044b8:	1898      	adds	r0, r3, r2
300044ba:	3201      	adds	r2, #1
300044bc:	f811 4b01 	ldrb.w	r4, [r1], #1
300044c0:	2a50      	cmp	r2, #80	; 0x50
300044c2:	f880 4300 	strb.w	r4, [r0, #768]	; 0x300
300044c6:	d1f7      	bne.n	300044b8 <BOOT_NVICReFill_HP+0x8>
300044c8:	4a06      	ldr	r2, [pc, #24]	; (300044e4 <BOOT_NVICReFill_HP+0x34>)
300044ca:	6811      	ldr	r1, [r2, #0]
300044cc:	f421 7180 	bic.w	r1, r1, #256	; 0x100
300044d0:	6019      	str	r1, [r3, #0]
300044d2:	6851      	ldr	r1, [r2, #4]
300044d4:	6059      	str	r1, [r3, #4]
300044d6:	6892      	ldr	r2, [r2, #8]
300044d8:	609a      	str	r2, [r3, #8]
300044da:	bd10      	pop	{r4, pc}
300044dc:	3000eb98 	.word	0x3000eb98
300044e0:	e000e100 	.word	0xe000e100
300044e4:	3000eb7c 	.word	0x3000eb7c

300044e8 <BOOT_WakeFromPG>:
300044e8:	b538      	push	{r3, r4, r5, lr}
300044ea:	f002 fb63 	bl	30006bb4 <BOOT_RAM_TZCfg>
300044ee:	4b2c      	ldr	r3, [pc, #176]	; (300045a0 <BOOT_WakeFromPG+0xb8>)
300044f0:	699b      	ldr	r3, [r3, #24]
300044f2:	b10b      	cbz	r3, 300044f8 <BOOT_WakeFromPG+0x10>
300044f4:	f7ff ff9c 	bl	30004430 <BOOT_SCBVTORReFill_HP>
300044f8:	f7ff ffda 	bl	300044b0 <BOOT_NVICReFill_HP>
300044fc:	4d29      	ldr	r5, [pc, #164]	; (300045a4 <BOOT_WakeFromPG+0xbc>)
300044fe:	f7ff ff6b 	bl	300043d8 <BOOT_SCBConfig_HP>
30004502:	f7ff fd2b 	bl	30003f5c <BOOT_CACHEWRR_Set>
30004506:	4b28      	ldr	r3, [pc, #160]	; (300045a8 <BOOT_WakeFromPG+0xc0>)
30004508:	6818      	ldr	r0, [r3, #0]
3000450a:	f7ff fd55 	bl	30003fb8 <BOOT_TCMSet>
3000450e:	2201      	movs	r2, #1
30004510:	4926      	ldr	r1, [pc, #152]	; (300045ac <BOOT_WakeFromPG+0xc4>)
30004512:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30004516:	47a8      	blx	r5
30004518:	4b25      	ldr	r3, [pc, #148]	; (300045b0 <BOOT_WakeFromPG+0xc8>)
3000451a:	4798      	blx	r3
3000451c:	4b25      	ldr	r3, [pc, #148]	; (300045b4 <BOOT_WakeFromPG+0xcc>)
3000451e:	4798      	blx	r3
30004520:	2801      	cmp	r0, #1
30004522:	4604      	mov	r4, r0
30004524:	d128      	bne.n	30004578 <BOOT_WakeFromPG+0x90>
30004526:	4b24      	ldr	r3, [pc, #144]	; (300045b8 <BOOT_WakeFromPG+0xd0>)
30004528:	2004      	movs	r0, #4
3000452a:	4a24      	ldr	r2, [pc, #144]	; (300045bc <BOOT_WakeFromPG+0xd4>)
3000452c:	4619      	mov	r1, r3
3000452e:	f006 fcf5 	bl	3000af1c <rtk_log_write>
30004532:	4622      	mov	r2, r4
30004534:	4922      	ldr	r1, [pc, #136]	; (300045c0 <BOOT_WakeFromPG+0xd8>)
30004536:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
3000453a:	47a8      	blx	r5
3000453c:	4622      	mov	r2, r4
3000453e:	4921      	ldr	r1, [pc, #132]	; (300045c4 <BOOT_WakeFromPG+0xdc>)
30004540:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30004544:	47a8      	blx	r5
30004546:	f7ff febb 	bl	300042c0 <BOOT_DDR_Init>
3000454a:	f7ff fead 	bl	300042a8 <BOOT_DDR_LCDC_HPR>
3000454e:	4a1e      	ldr	r2, [pc, #120]	; (300045c8 <BOOT_WakeFromPG+0xe0>)
30004550:	491e      	ldr	r1, [pc, #120]	; (300045cc <BOOT_WakeFromPG+0xe4>)
30004552:	6893      	ldr	r3, [r2, #8]
30004554:	6852      	ldr	r2, [r2, #4]
30004556:	605a      	str	r2, [r3, #4]
30004558:	4a1d      	ldr	r2, [pc, #116]	; (300045d0 <BOOT_WakeFromPG+0xe8>)
3000455a:	6093      	str	r3, [r2, #8]
3000455c:	4a1d      	ldr	r2, [pc, #116]	; (300045d4 <BOOT_WakeFromPG+0xec>)
3000455e:	61d1      	str	r1, [r2, #28]
30004560:	4a1d      	ldr	r2, [pc, #116]	; (300045d8 <BOOT_WakeFromPG+0xf0>)
30004562:	f382 8888 	msr	MSP_NS, r2
30004566:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
3000456a:	f382 8809 	msr	PSP, r2
3000456e:	6858      	ldr	r0, [r3, #4]
30004570:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
30004574:	f7ff bc38 	b.w	30003de8 <BOOT_NsStart>
30004578:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
3000457c:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30004580:	2201      	movs	r2, #1
30004582:	47a8      	blx	r5
30004584:	4a15      	ldr	r2, [pc, #84]	; (300045dc <BOOT_WakeFromPG+0xf4>)
30004586:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000458a:	f043 0302 	orr.w	r3, r3, #2
3000458e:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30004592:	f7ff fed3 	bl	3000433c <BOOT_PSRAM_Init>
30004596:	2096      	movs	r0, #150	; 0x96
30004598:	4b11      	ldr	r3, [pc, #68]	; (300045e0 <BOOT_WakeFromPG+0xf8>)
3000459a:	4798      	blx	r3
3000459c:	e7d7      	b.n	3000454e <BOOT_WakeFromPG+0x66>
3000459e:	bf00      	nop
300045a0:	3000eb7c 	.word	0x3000eb7c
300045a4:	0000b479 	.word	0x0000b479
300045a8:	3000d554 	.word	0x3000d554
300045ac:	40001000 	.word	0x40001000
300045b0:	0000d835 	.word	0x0000d835
300045b4:	30007291 	.word	0x30007291
300045b8:	3000c714 	.word	0x3000c714
300045bc:	3000c831 	.word	0x3000c831
300045c0:	40080000 	.word	0x40080000
300045c4:	40040000 	.word	0x40040000
300045c8:	60000020 	.word	0x60000020
300045cc:	30003941 	.word	0x30003941
300045d0:	e002ed00 	.word	0xe002ed00
300045d4:	30000000 	.word	0x30000000
300045d8:	2001bffc 	.word	0x2001bffc
300045dc:	42008000 	.word	0x42008000
300045e0:	00009b2d 	.word	0x00009b2d

300045e4 <BOOT_ImgCopy>:
300045e4:	b513      	push	{r0, r1, r4, lr}
300045e6:	4b0a      	ldr	r3, [pc, #40]	; (30004610 <BOOT_ImgCopy+0x2c>)
300045e8:	4604      	mov	r4, r0
300045ea:	e9cd 2100 	strd	r2, r1, [sp]
300045ee:	4798      	blx	r3
300045f0:	e9dd 2100 	ldrd	r2, r1, [sp]
300045f4:	b128      	cbz	r0, 30004602 <BOOT_ImgCopy+0x1e>
300045f6:	4620      	mov	r0, r4
300045f8:	4b06      	ldr	r3, [pc, #24]	; (30004614 <BOOT_ImgCopy+0x30>)
300045fa:	b002      	add	sp, #8
300045fc:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004600:	4718      	bx	r3
30004602:	4620      	mov	r0, r4
30004604:	b002      	add	sp, #8
30004606:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000460a:	f003 bafb 	b.w	30007c04 <NandImgCopy>
3000460e:	bf00      	nop
30004610:	30008ff5 	.word	0x30008ff5
30004614:	00012a1d 	.word	0x00012a1d

30004618 <BOOT_LoadImages>:
30004618:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000461c:	ed2d 8b08 	vpush	{d8-d11}
30004620:	b0af      	sub	sp, #188	; 0xbc
30004622:	f04f 0900 	mov.w	r9, #0
30004626:	2104      	movs	r1, #4
30004628:	a809      	add	r0, sp, #36	; 0x24
3000462a:	f8cd 900c 	str.w	r9, [sp, #12]
3000462e:	f7ff fac3 	bl	30003bb8 <flash_read_id>
30004632:	f89d 3026 	ldrb.w	r3, [sp, #38]	; 0x26
30004636:	2201      	movs	r2, #1
30004638:	46ca      	mov	sl, r9
3000463a:	fa02 f803 	lsl.w	r8, r2, r3
3000463e:	f5b8 0f00 	cmp.w	r8, #8388608	; 0x800000
30004642:	f040 80ab 	bne.w	3000479c <BOOT_LoadImages+0x184>
30004646:	48b4      	ldr	r0, [pc, #720]	; (30004918 <BOOT_LoadImages+0x300>)
30004648:	f006 febe 	bl	3000b3c8 <__DiagPrintf_veneer>
3000464c:	f108 66ff 	add.w	r6, r8, #133693440	; 0x7f80000
30004650:	f04f 0b00 	mov.w	fp, #0
30004654:	48b1      	ldr	r0, [pc, #708]	; (3000491c <BOOT_LoadImages+0x304>)
30004656:	f506 25fe 	add.w	r5, r6, #520192	; 0x7f000
3000465a:	f506 26fc 	add.w	r6, r6, #516096	; 0x7e000
3000465e:	e9cd bb0a 	strd	fp, fp, [sp, #40]	; 0x28
30004662:	e9cd bb0c 	strd	fp, fp, [sp, #48]	; 0x30
30004666:	f7ff fad2 	bl	30003c0e <mcc_init_crc32>
3000466a:	7b33      	ldrb	r3, [r6, #12]
3000466c:	7b2f      	ldrb	r7, [r5, #12]
3000466e:	9304      	str	r3, [sp, #16]
30004670:	6873      	ldr	r3, [r6, #4]
30004672:	9305      	str	r3, [sp, #20]
30004674:	686b      	ldr	r3, [r5, #4]
30004676:	68ac      	ldr	r4, [r5, #8]
30004678:	9306      	str	r3, [sp, #24]
3000467a:	9b05      	ldr	r3, [sp, #20]
3000467c:	3b01      	subs	r3, #1
3000467e:	3303      	adds	r3, #3
30004680:	f240 8092 	bls.w	300047a8 <BOOT_LoadImages+0x190>
30004684:	48a6      	ldr	r0, [pc, #664]	; (30004920 <BOOT_LoadImages+0x308>)
30004686:	f006 fe9f 	bl	3000b3c8 <__DiagPrintf_veneer>
3000468a:	9b06      	ldr	r3, [sp, #24]
3000468c:	3b01      	subs	r3, #1
3000468e:	3303      	adds	r3, #3
30004690:	f240 809c 	bls.w	300047cc <BOOT_LoadImages+0x1b4>
30004694:	48a3      	ldr	r0, [pc, #652]	; (30004924 <BOOT_LoadImages+0x30c>)
30004696:	2400      	movs	r4, #0
30004698:	f006 fe96 	bl	3000b3c8 <__DiagPrintf_veneer>
3000469c:	f8d6 3011 	ldr.w	r3, [r6, #17]
300046a0:	f8d6 700d 	ldr.w	r7, [r6, #13]
300046a4:	ee08 3a10 	vmov	s16, r3
300046a8:	f8d5 300d 	ldr.w	r3, [r5, #13]
300046ac:	ee08 3a90 	vmov	s17, r3
300046b0:	f8d5 3011 	ldr.w	r3, [r5, #17]
300046b4:	ee09 3a10 	vmov	s18, r3
300046b8:	f1bb 0f00 	cmp.w	fp, #0
300046bc:	f000 8099 	beq.w	300047f2 <BOOT_LoadImages+0x1da>
300046c0:	4899      	ldr	r0, [pc, #612]	; (30004928 <BOOT_LoadImages+0x310>)
300046c2:	f640 71fc 	movw	r1, #4092	; 0xffc
300046c6:	f8d6 b000 	ldr.w	fp, [r6]
300046ca:	4a94      	ldr	r2, [pc, #592]	; (3000491c <BOOT_LoadImages+0x304>)
300046cc:	4440      	add	r0, r8
300046ce:	f7ff facb 	bl	30003c68 <mcc_crc32>
300046d2:	4583      	cmp	fp, r0
300046d4:	4602      	mov	r2, r0
300046d6:	f040 8088 	bne.w	300047ea <BOOT_LoadImages+0x1d2>
300046da:	465a      	mov	r2, fp
300046dc:	4659      	mov	r1, fp
300046de:	4893      	ldr	r0, [pc, #588]	; (3000492c <BOOT_LoadImages+0x314>)
300046e0:	f006 fe72 	bl	3000b3c8 <__DiagPrintf_veneer>
300046e4:	b1ac      	cbz	r4, 30004712 <BOOT_LoadImages+0xfa>
300046e6:	f108 6000 	add.w	r0, r8, #134217728	; 0x8000000
300046ea:	f8d5 b000 	ldr.w	fp, [r5]
300046ee:	4a8b      	ldr	r2, [pc, #556]	; (3000491c <BOOT_LoadImages+0x304>)
300046f0:	f640 71fc 	movw	r1, #4092	; 0xffc
300046f4:	f6a0 70fc 	subw	r0, r0, #4092	; 0xffc
300046f8:	f7ff fab6 	bl	30003c68 <mcc_crc32>
300046fc:	4583      	cmp	fp, r0
300046fe:	4602      	mov	r2, r0
30004700:	d17b      	bne.n	300047fa <BOOT_LoadImages+0x1e2>
30004702:	465a      	mov	r2, fp
30004704:	4659      	mov	r1, fp
30004706:	488a      	ldr	r0, [pc, #552]	; (30004930 <BOOT_LoadImages+0x318>)
30004708:	f006 fe5e 	bl	3000b3c8 <__DiagPrintf_veneer>
3000470c:	2c00      	cmp	r4, #0
3000470e:	f000 8087 	beq.w	30004820 <BOOT_LoadImages+0x208>
30004712:	7b33      	ldrb	r3, [r6, #12]
30004714:	9304      	str	r3, [sp, #16]
30004716:	f108 6300 	add.w	r3, r8, #134217728	; 0x8000000
3000471a:	42bb      	cmp	r3, r7
3000471c:	d302      	bcc.n	30004724 <BOOT_LoadImages+0x10c>
3000471e:	f1b7 6f00 	cmp.w	r7, #134217728	; 0x8000000
30004722:	d272      	bcs.n	3000480a <BOOT_LoadImages+0x1f2>
30004724:	4883      	ldr	r0, [pc, #524]	; (30004934 <BOOT_LoadImages+0x31c>)
30004726:	f006 fe4f 	bl	3000b3c8 <__DiagPrintf_veneer>
3000472a:	2c00      	cmp	r4, #0
3000472c:	d177      	bne.n	3000481e <BOOT_LoadImages+0x206>
3000472e:	4882      	ldr	r0, [pc, #520]	; (30004938 <BOOT_LoadImages+0x320>)
30004730:	f006 fe4a 	bl	3000b3c8 <__DiagPrintf_veneer>
30004734:	4881      	ldr	r0, [pc, #516]	; (3000493c <BOOT_LoadImages+0x324>)
30004736:	f006 fe47 	bl	3000b3c8 <__DiagPrintf_veneer>
3000473a:	aa0a      	add	r2, sp, #40	; 0x28
3000473c:	4641      	mov	r1, r8
3000473e:	4638      	mov	r0, r7
30004740:	f7ff faa3 	bl	30003c8a <recovery_check>
30004744:	aa0b      	add	r2, sp, #44	; 0x2c
30004746:	4641      	mov	r1, r8
30004748:	9004      	str	r0, [sp, #16]
3000474a:	ee18 0a10 	vmov	r0, s16
3000474e:	f7ff fa9c 	bl	30003c8a <recovery_check>
30004752:	aa0c      	add	r2, sp, #48	; 0x30
30004754:	4683      	mov	fp, r0
30004756:	4641      	mov	r1, r8
30004758:	ee18 0a90 	vmov	r0, s17
3000475c:	f7ff fa95 	bl	30003c8a <recovery_check>
30004760:	aa0d      	add	r2, sp, #52	; 0x34
30004762:	4604      	mov	r4, r0
30004764:	4641      	mov	r1, r8
30004766:	ee19 0a10 	vmov	r0, s18
3000476a:	f7ff fa8e 	bl	30003c8a <recovery_check>
3000476e:	9b04      	ldr	r3, [sp, #16]
30004770:	4304      	orrs	r4, r0
30004772:	ea53 0b0b 	orrs.w	fp, r3, fp
30004776:	f040 81b1 	bne.w	30004adc <BOOT_LoadImages+0x4c4>
3000477a:	2c00      	cmp	r4, #0
3000477c:	f000 8214 	beq.w	30004ba8 <BOOT_LoadImages+0x590>
30004780:	ee18 1a90 	vmov	r1, s17
30004784:	e9dd 230c 	ldrd	r2, r3, [sp, #48]	; 0x30
30004788:	e9cd 5b00 	strd	r5, fp, [sp]
3000478c:	429a      	cmp	r2, r3
3000478e:	4633      	mov	r3, r6
30004790:	ee19 2a10 	vmov	r2, s18
30004794:	f080 81b6 	bcs.w	30004b04 <BOOT_LoadImages+0x4ec>
30004798:	2001      	movs	r0, #1
3000479a:	e1b4      	b.n	30004b06 <BOOT_LoadImages+0x4ee>
3000479c:	f1b8 7f80 	cmp.w	r8, #16777216	; 0x1000000
300047a0:	f47f af54 	bne.w	3000464c <BOOT_LoadImages+0x34>
300047a4:	4866      	ldr	r0, [pc, #408]	; (30004940 <BOOT_LoadImages+0x328>)
300047a6:	e74f      	b.n	30004648 <BOOT_LoadImages+0x30>
300047a8:	68b3      	ldr	r3, [r6, #8]
300047aa:	3b01      	subs	r3, #1
300047ac:	3303      	adds	r3, #3
300047ae:	d901      	bls.n	300047b4 <BOOT_LoadImages+0x19c>
300047b0:	4864      	ldr	r0, [pc, #400]	; (30004944 <BOOT_LoadImages+0x32c>)
300047b2:	e768      	b.n	30004686 <BOOT_LoadImages+0x6e>
300047b4:	9b04      	ldr	r3, [sp, #16]
300047b6:	2b01      	cmp	r3, #1
300047b8:	d901      	bls.n	300047be <BOOT_LoadImages+0x1a6>
300047ba:	4863      	ldr	r0, [pc, #396]	; (30004948 <BOOT_LoadImages+0x330>)
300047bc:	e763      	b.n	30004686 <BOOT_LoadImages+0x6e>
300047be:	9905      	ldr	r1, [sp, #20]
300047c0:	f04f 0b01 	mov.w	fp, #1
300047c4:	4861      	ldr	r0, [pc, #388]	; (3000494c <BOOT_LoadImages+0x334>)
300047c6:	f006 fdff 	bl	3000b3c8 <__DiagPrintf_veneer>
300047ca:	e75e      	b.n	3000468a <BOOT_LoadImages+0x72>
300047cc:	3c01      	subs	r4, #1
300047ce:	3403      	adds	r4, #3
300047d0:	d901      	bls.n	300047d6 <BOOT_LoadImages+0x1be>
300047d2:	485f      	ldr	r0, [pc, #380]	; (30004950 <BOOT_LoadImages+0x338>)
300047d4:	e75f      	b.n	30004696 <BOOT_LoadImages+0x7e>
300047d6:	2f01      	cmp	r7, #1
300047d8:	d901      	bls.n	300047de <BOOT_LoadImages+0x1c6>
300047da:	485e      	ldr	r0, [pc, #376]	; (30004954 <BOOT_LoadImages+0x33c>)
300047dc:	e75b      	b.n	30004696 <BOOT_LoadImages+0x7e>
300047de:	9906      	ldr	r1, [sp, #24]
300047e0:	2401      	movs	r4, #1
300047e2:	485d      	ldr	r0, [pc, #372]	; (30004958 <BOOT_LoadImages+0x340>)
300047e4:	f006 fdf0 	bl	3000b3c8 <__DiagPrintf_veneer>
300047e8:	e758      	b.n	3000469c <BOOT_LoadImages+0x84>
300047ea:	4659      	mov	r1, fp
300047ec:	485b      	ldr	r0, [pc, #364]	; (3000495c <BOOT_LoadImages+0x344>)
300047ee:	f006 fdeb 	bl	3000b3c8 <__DiagPrintf_veneer>
300047f2:	2c00      	cmp	r4, #0
300047f4:	d09b      	beq.n	3000472e <BOOT_LoadImages+0x116>
300047f6:	2400      	movs	r4, #0
300047f8:	e775      	b.n	300046e6 <BOOT_LoadImages+0xce>
300047fa:	4659      	mov	r1, fp
300047fc:	4858      	ldr	r0, [pc, #352]	; (30004960 <BOOT_LoadImages+0x348>)
300047fe:	f006 fde3 	bl	3000b3c8 <__DiagPrintf_veneer>
30004802:	2c00      	cmp	r4, #0
30004804:	d093      	beq.n	3000472e <BOOT_LoadImages+0x116>
30004806:	2400      	movs	r4, #0
30004808:	e783      	b.n	30004712 <BOOT_LoadImages+0xfa>
3000480a:	ee18 2a10 	vmov	r2, s16
3000480e:	4293      	cmp	r3, r2
30004810:	d303      	bcc.n	3000481a <BOOT_LoadImages+0x202>
30004812:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004816:	f080 82d9 	bcs.w	30004dcc <BOOT_LoadImages+0x7b4>
3000481a:	4852      	ldr	r0, [pc, #328]	; (30004964 <BOOT_LoadImages+0x34c>)
3000481c:	e783      	b.n	30004726 <BOOT_LoadImages+0x10e>
3000481e:	2400      	movs	r4, #0
30004820:	f108 6300 	add.w	r3, r8, #134217728	; 0x8000000
30004824:	ee18 2a90 	vmov	r2, s17
30004828:	4293      	cmp	r3, r2
3000482a:	d302      	bcc.n	30004832 <BOOT_LoadImages+0x21a>
3000482c:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004830:	d210      	bcs.n	30004854 <BOOT_LoadImages+0x23c>
30004832:	484d      	ldr	r0, [pc, #308]	; (30004968 <BOOT_LoadImages+0x350>)
30004834:	f006 fdc8 	bl	3000b3c8 <__DiagPrintf_veneer>
30004838:	2c00      	cmp	r4, #0
3000483a:	f43f af78 	beq.w	3000472e <BOOT_LoadImages+0x116>
3000483e:	484b      	ldr	r0, [pc, #300]	; (3000496c <BOOT_LoadImages+0x354>)
30004840:	f006 fdc2 	bl	3000b3c8 <__DiagPrintf_veneer>
30004844:	4b35      	ldr	r3, [pc, #212]	; (3000491c <BOOT_LoadImages+0x304>)
30004846:	ee18 2a10 	vmov	r2, s16
3000484a:	f8c3 7400 	str.w	r7, [r3, #1024]	; 0x400
3000484e:	f8c3 2404 	str.w	r2, [r3, #1028]	; 0x404
30004852:	e01a      	b.n	3000488a <BOOT_LoadImages+0x272>
30004854:	ee19 2a10 	vmov	r2, s18
30004858:	4293      	cmp	r3, r2
3000485a:	d303      	bcc.n	30004864 <BOOT_LoadImages+0x24c>
3000485c:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004860:	f080 8254 	bcs.w	30004d0c <BOOT_LoadImages+0x6f4>
30004864:	4842      	ldr	r0, [pc, #264]	; (30004970 <BOOT_LoadImages+0x358>)
30004866:	e7e5      	b.n	30004834 <BOOT_LoadImages+0x21c>
30004868:	4842      	ldr	r0, [pc, #264]	; (30004974 <BOOT_LoadImages+0x35c>)
3000486a:	f006 fdad 	bl	3000b3c8 <__DiagPrintf_veneer>
3000486e:	e9dd 3205 	ldrd	r3, r2, [sp, #20]
30004872:	4293      	cmp	r3, r2
30004874:	f0c0 8251 	bcc.w	30004d1a <BOOT_LoadImages+0x702>
30004878:	483c      	ldr	r0, [pc, #240]	; (3000496c <BOOT_LoadImages+0x354>)
3000487a:	f006 fda5 	bl	3000b3c8 <__DiagPrintf_veneer>
3000487e:	ee18 3a10 	vmov	r3, s16
30004882:	f8cb 7400 	str.w	r7, [fp, #1024]	; 0x400
30004886:	f8cb 3404 	str.w	r3, [fp, #1028]	; 0x404
3000488a:	f8dd 9010 	ldr.w	r9, [sp, #16]
3000488e:	4635      	mov	r5, r6
30004890:	e24e      	b.n	30004d30 <BOOT_LoadImages+0x718>
30004892:	f04f 0a00 	mov.w	sl, #0
30004896:	4656      	mov	r6, sl
30004898:	e1a5      	b.n	30004be6 <BOOT_LoadImages+0x5ce>
3000489a:	f04f 0a01 	mov.w	sl, #1
3000489e:	f8d3 1006 	ldr.w	r1, [r3, #6]
300048a2:	4835      	ldr	r0, [pc, #212]	; (30004978 <BOOT_LoadImages+0x360>)
300048a4:	f006 fd90 	bl	3000b3c8 <__DiagPrintf_veneer>
300048a8:	f8cd a00c 	str.w	sl, [sp, #12]
300048ac:	2e00      	cmp	r6, #0
300048ae:	f000 817f 	beq.w	30004bb0 <BOOT_LoadImages+0x598>
300048b2:	4634      	mov	r4, r6
300048b4:	e1a9      	b.n	30004c0a <BOOT_LoadImages+0x5f2>
300048b6:	f8d3 3404 	ldr.w	r3, [r3, #1028]	; 0x404
300048ba:	f8d3 200a 	ldr.w	r2, [r3, #10]
300048be:	3201      	adds	r2, #1
300048c0:	d106      	bne.n	300048d0 <BOOT_LoadImages+0x2b8>
300048c2:	482e      	ldr	r0, [pc, #184]	; (3000497c <BOOT_LoadImages+0x364>)
300048c4:	2400      	movs	r4, #0
300048c6:	f006 fd7f 	bl	3000b3c8 <__DiagPrintf_veneer>
300048ca:	2301      	movs	r3, #1
300048cc:	9303      	str	r3, [sp, #12]
300048ce:	e19c      	b.n	30004c0a <BOOT_LoadImages+0x5f2>
300048d0:	f8d3 1006 	ldr.w	r1, [r3, #6]
300048d4:	482a      	ldr	r0, [pc, #168]	; (30004980 <BOOT_LoadImages+0x368>)
300048d6:	f006 fd77 	bl	3000b3c8 <__DiagPrintf_veneer>
300048da:	2301      	movs	r3, #1
300048dc:	f04f 0a02 	mov.w	sl, #2
300048e0:	9303      	str	r3, [sp, #12]
300048e2:	e7e3      	b.n	300048ac <BOOT_LoadImages+0x294>
300048e4:	2601      	movs	r6, #1
300048e6:	e24a      	b.n	30004d7e <BOOT_LoadImages+0x766>
300048e8:	f8d8 300a 	ldr.w	r3, [r8, #10]
300048ec:	3301      	adds	r3, #1
300048ee:	d107      	bne.n	30004900 <BOOT_LoadImages+0x2e8>
300048f0:	4824      	ldr	r0, [pc, #144]	; (30004984 <BOOT_LoadImages+0x36c>)
300048f2:	f006 fd69 	bl	3000b3c8 <__DiagPrintf_veneer>
300048f6:	2c00      	cmp	r4, #0
300048f8:	d074      	beq.n	300049e4 <BOOT_LoadImages+0x3cc>
300048fa:	2300      	movs	r3, #0
300048fc:	9303      	str	r3, [sp, #12]
300048fe:	e7d5      	b.n	300048ac <BOOT_LoadImages+0x294>
30004900:	ee19 1a90 	vmov	r1, s19
30004904:	4820      	ldr	r0, [pc, #128]	; (30004988 <BOOT_LoadImages+0x370>)
30004906:	f006 fd5f 	bl	3000b3c8 <__DiagPrintf_veneer>
3000490a:	f8d7 1400 	ldr.w	r1, [r7, #1024]	; 0x400
3000490e:	2002      	movs	r0, #2
30004910:	f7ff f904 	bl	30003b1c <FLASH_Erase_With_Lock>
30004914:	e7ef      	b.n	300048f6 <BOOT_LoadImages+0x2de>
30004916:	bf00      	nop
30004918:	3000c845 	.word	0x3000c845
3000491c:	3000d718 	.word	0x3000d718
30004920:	3000c866 	.word	0x3000c866
30004924:	3000c8c5 	.word	0x3000c8c5
30004928:	07ffe004 	.word	0x07ffe004
3000492c:	3000c924 	.word	0x3000c924
30004930:	3000c99e 	.word	0x3000c99e
30004934:	3000ca18 	.word	0x3000ca18
30004938:	3000caaf 	.word	0x3000caaf
3000493c:	3000cac0 	.word	0x3000cac0
30004940:	3000c855 	.word	0x3000c855
30004944:	3000c87b 	.word	0x3000c87b
30004948:	3000c88f 	.word	0x3000c88f
3000494c:	3000c8a9 	.word	0x3000c8a9
30004950:	3000c8da 	.word	0x3000c8da
30004954:	3000c8ee 	.word	0x3000c8ee
30004958:	3000c908 	.word	0x3000c908
3000495c:	3000c95f 	.word	0x3000c95f
30004960:	3000c9d9 	.word	0x3000c9d9
30004964:	3000ca32 	.word	0x3000ca32
30004968:	3000ca4c 	.word	0x3000ca4c
3000496c:	3000ca93 	.word	0x3000ca93
30004970:	3000ca66 	.word	0x3000ca66
30004974:	3000ca80 	.word	0x3000ca80
30004978:	3000c7d4 	.word	0x3000c7d4
3000497c:	3000caf0 	.word	0x3000caf0
30004980:	3000c7eb 	.word	0x3000c7eb
30004984:	3000cae2 	.word	0x3000cae2
30004988:	3000cb3a 	.word	0x3000cb3a
3000498c:	f8d7 8404 	ldr.w	r8, [r7, #1028]	; 0x404
30004990:	463a      	mov	r2, r7
30004992:	ee1b 1a10 	vmov	r1, s22
30004996:	f108 0004 	add.w	r0, r8, #4
3000499a:	f7ff f965 	bl	30003c68 <mcc_crc32>
3000499e:	ee1a 3a10 	vmov	r3, s20
300049a2:	4602      	mov	r2, r0
300049a4:	4298      	cmp	r0, r3
300049a6:	d10f      	bne.n	300049c8 <BOOT_LoadImages+0x3b0>
300049a8:	ee1a 2a10 	vmov	r2, s20
300049ac:	ee1a 1a10 	vmov	r1, s20
300049b0:	48be      	ldr	r0, [pc, #760]	; (30004cac <BOOT_LoadImages+0x694>)
300049b2:	f006 fd09 	bl	3000b3c8 <__DiagPrintf_veneer>
300049b6:	f8d7 3404 	ldr.w	r3, [r7, #1028]	; 0x404
300049ba:	48bd      	ldr	r0, [pc, #756]	; (30004cb0 <BOOT_LoadImages+0x698>)
300049bc:	f8d3 1006 	ldr.w	r1, [r3, #6]
300049c0:	f006 fd02 	bl	3000b3c8 <__DiagPrintf_veneer>
300049c4:	2300      	movs	r3, #0
300049c6:	e789      	b.n	300048dc <BOOT_LoadImages+0x2c4>
300049c8:	f8d8 300a 	ldr.w	r3, [r8, #10]
300049cc:	3301      	adds	r3, #1
300049ce:	d101      	bne.n	300049d4 <BOOT_LoadImages+0x3bc>
300049d0:	48b8      	ldr	r0, [pc, #736]	; (30004cb4 <BOOT_LoadImages+0x69c>)
300049d2:	e78e      	b.n	300048f2 <BOOT_LoadImages+0x2da>
300049d4:	ee1a 1a10 	vmov	r1, s20
300049d8:	48b7      	ldr	r0, [pc, #732]	; (30004cb8 <BOOT_LoadImages+0x6a0>)
300049da:	f006 fcf5 	bl	3000b3c8 <__DiagPrintf_veneer>
300049de:	f8d7 1404 	ldr.w	r1, [r7, #1028]	; 0x404
300049e2:	e794      	b.n	3000490e <BOOT_LoadImages+0x2f6>
300049e4:	9403      	str	r4, [sp, #12]
300049e6:	e110      	b.n	30004c0a <BOOT_LoadImages+0x5f2>
300049e8:	f1b9 0f00 	cmp.w	r9, #0
300049ec:	f000 811e 	beq.w	30004c2c <BOOT_LoadImages+0x614>
300049f0:	2300      	movs	r3, #0
300049f2:	e11c      	b.n	30004c2e <BOOT_LoadImages+0x616>
300049f4:	464c      	mov	r4, r9
300049f6:	f8d3 1006 	ldr.w	r1, [r3, #6]
300049fa:	48b0      	ldr	r0, [pc, #704]	; (30004cbc <BOOT_LoadImages+0x6a4>)
300049fc:	f006 fce4 	bl	3000b3c8 <__DiagPrintf_veneer>
30004a00:	9403      	str	r4, [sp, #12]
30004a02:	2401      	movs	r4, #1
30004a04:	46a1      	mov	r9, r4
30004a06:	e088      	b.n	30004b1a <BOOT_LoadImages+0x502>
30004a08:	f8d7 3404 	ldr.w	r3, [r7, #1028]	; 0x404
30004a0c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004a10:	3201      	adds	r2, #1
30004a12:	d149      	bne.n	30004aa8 <BOOT_LoadImages+0x490>
30004a14:	48a7      	ldr	r0, [pc, #668]	; (30004cb4 <BOOT_LoadImages+0x69c>)
30004a16:	e13e      	b.n	30004c96 <BOOT_LoadImages+0x67e>
30004a18:	2c00      	cmp	r4, #0
30004a1a:	d17a      	bne.n	30004b12 <BOOT_LoadImages+0x4fa>
30004a1c:	f1b9 0f01 	cmp.w	r9, #1
30004a20:	d12c      	bne.n	30004a7c <BOOT_LoadImages+0x464>
30004a22:	f8d7 8400 	ldr.w	r8, [r7, #1024]	; 0x400
30004a26:	463a      	mov	r2, r7
30004a28:	ee1a 1a90 	vmov	r1, s21
30004a2c:	f108 0004 	add.w	r0, r8, #4
30004a30:	f7ff f91a 	bl	30003c68 <mcc_crc32>
30004a34:	ee19 3a90 	vmov	r3, s19
30004a38:	4602      	mov	r2, r0
30004a3a:	4298      	cmp	r0, r3
30004a3c:	d10e      	bne.n	30004a5c <BOOT_LoadImages+0x444>
30004a3e:	ee19 1a90 	vmov	r1, s19
30004a42:	ee19 2a90 	vmov	r2, s19
30004a46:	489e      	ldr	r0, [pc, #632]	; (30004cc0 <BOOT_LoadImages+0x6a8>)
30004a48:	f006 fcbe 	bl	3000b3c8 <__DiagPrintf_veneer>
30004a4c:	f8d7 3400 	ldr.w	r3, [r7, #1024]	; 0x400
30004a50:	489a      	ldr	r0, [pc, #616]	; (30004cbc <BOOT_LoadImages+0x6a4>)
30004a52:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004a56:	f006 fcb7 	bl	3000b3c8 <__DiagPrintf_veneer>
30004a5a:	e7d1      	b.n	30004a00 <BOOT_LoadImages+0x3e8>
30004a5c:	f8d8 300a 	ldr.w	r3, [r8, #10]
30004a60:	3301      	adds	r3, #1
30004a62:	f000 8117 	beq.w	30004c94 <BOOT_LoadImages+0x67c>
30004a66:	ee19 1a90 	vmov	r1, s19
30004a6a:	4896      	ldr	r0, [pc, #600]	; (30004cc4 <BOOT_LoadImages+0x6ac>)
30004a6c:	f006 fcac 	bl	3000b3c8 <__DiagPrintf_veneer>
30004a70:	f8d7 1400 	ldr.w	r1, [r7, #1024]	; 0x400
30004a74:	2002      	movs	r0, #2
30004a76:	f7ff f851 	bl	30003b1c <FLASH_Erase_With_Lock>
30004a7a:	e10e      	b.n	30004c9a <BOOT_LoadImages+0x682>
30004a7c:	f8d7 4404 	ldr.w	r4, [r7, #1028]	; 0x404
30004a80:	463a      	mov	r2, r7
30004a82:	ee1b 1a10 	vmov	r1, s22
30004a86:	1d20      	adds	r0, r4, #4
30004a88:	f7ff f8ee 	bl	30003c68 <mcc_crc32>
30004a8c:	ee1a 3a10 	vmov	r3, s20
30004a90:	4602      	mov	r2, r0
30004a92:	4298      	cmp	r0, r3
30004a94:	d116      	bne.n	30004ac4 <BOOT_LoadImages+0x4ac>
30004a96:	ee1a 2a10 	vmov	r2, s20
30004a9a:	ee1a 1a10 	vmov	r1, s20
30004a9e:	4883      	ldr	r0, [pc, #524]	; (30004cac <BOOT_LoadImages+0x694>)
30004aa0:	f006 fc92 	bl	3000b3c8 <__DiagPrintf_veneer>
30004aa4:	f8d7 3404 	ldr.w	r3, [r7, #1028]	; 0x404
30004aa8:	2401      	movs	r4, #1
30004aaa:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004aae:	4880      	ldr	r0, [pc, #512]	; (30004cb0 <BOOT_LoadImages+0x698>)
30004ab0:	f006 fc8a 	bl	3000b3c8 <__DiagPrintf_veneer>
30004ab4:	4b84      	ldr	r3, [pc, #528]	; (30004cc8 <BOOT_LoadImages+0x6b0>)
30004ab6:	f04f 0a02 	mov.w	sl, #2
30004aba:	f8d3 3404 	ldr.w	r3, [r3, #1028]	; 0x404
30004abe:	3310      	adds	r3, #16
30004ac0:	9307      	str	r3, [sp, #28]
30004ac2:	e07b      	b.n	30004bbc <BOOT_LoadImages+0x5a4>
30004ac4:	f8d4 300a 	ldr.w	r3, [r4, #10]
30004ac8:	3301      	adds	r3, #1
30004aca:	d0a3      	beq.n	30004a14 <BOOT_LoadImages+0x3fc>
30004acc:	ee1a 1a10 	vmov	r1, s20
30004ad0:	4879      	ldr	r0, [pc, #484]	; (30004cb8 <BOOT_LoadImages+0x6a0>)
30004ad2:	f006 fc79 	bl	3000b3c8 <__DiagPrintf_veneer>
30004ad6:	f8d7 1404 	ldr.w	r1, [r7, #1028]	; 0x404
30004ada:	e7cb      	b.n	30004a74 <BOOT_LoadImages+0x45c>
30004adc:	e9dd 320a 	ldrd	r3, r2, [sp, #40]	; 0x28
30004ae0:	2c00      	cmp	r4, #0
30004ae2:	d056      	beq.n	30004b92 <BOOT_LoadImages+0x57a>
30004ae4:	4293      	cmp	r3, r2
30004ae6:	e9dd 100c 	ldrd	r1, r0, [sp, #48]	; 0x30
30004aea:	d320      	bcc.n	30004b2e <BOOT_LoadImages+0x516>
30004aec:	428b      	cmp	r3, r1
30004aee:	f0c0 8173 	bcc.w	30004dd8 <BOOT_LoadImages+0x7c0>
30004af2:	4283      	cmp	r3, r0
30004af4:	d319      	bcc.n	30004b2a <BOOT_LoadImages+0x512>
30004af6:	2301      	movs	r3, #1
30004af8:	ee18 2a10 	vmov	r2, s16
30004afc:	4639      	mov	r1, r7
30004afe:	e9cd 5300 	strd	r5, r3, [sp]
30004b02:	4633      	mov	r3, r6
30004b04:	2000      	movs	r0, #0
30004b06:	f7ff f8e1 	bl	30003ccc <rewrite_bp>
30004b0a:	2400      	movs	r4, #0
30004b0c:	fa5f fa80 	uxtb.w	sl, r0
30004b10:	4626      	mov	r6, r4
30004b12:	f1ba 0f01 	cmp.w	sl, #1
30004b16:	f040 80c4 	bne.w	30004ca2 <BOOT_LoadImages+0x68a>
30004b1a:	4b6b      	ldr	r3, [pc, #428]	; (30004cc8 <BOOT_LoadImages+0x6b0>)
30004b1c:	f04f 0a01 	mov.w	sl, #1
30004b20:	f8d3 3400 	ldr.w	r3, [r3, #1024]	; 0x400
30004b24:	3310      	adds	r3, #16
30004b26:	9307      	str	r3, [sp, #28]
30004b28:	e048      	b.n	30004bbc <BOOT_LoadImages+0x5a4>
30004b2a:	4293      	cmp	r3, r2
30004b2c:	d10c      	bne.n	30004b48 <BOOT_LoadImages+0x530>
30004b2e:	428a      	cmp	r2, r1
30004b30:	f0c0 8150 	bcc.w	30004dd4 <BOOT_LoadImages+0x7bc>
30004b34:	4282      	cmp	r2, r0
30004b36:	d307      	bcc.n	30004b48 <BOOT_LoadImages+0x530>
30004b38:	2001      	movs	r0, #1
30004b3a:	4633      	mov	r3, r6
30004b3c:	ee18 2a10 	vmov	r2, s16
30004b40:	4639      	mov	r1, r7
30004b42:	e9cd 5000 	strd	r5, r0, [sp]
30004b46:	e7de      	b.n	30004b06 <BOOT_LoadImages+0x4ee>
30004b48:	428b      	cmp	r3, r1
30004b4a:	d80d      	bhi.n	30004b68 <BOOT_LoadImages+0x550>
30004b4c:	428a      	cmp	r2, r1
30004b4e:	d80b      	bhi.n	30004b68 <BOOT_LoadImages+0x550>
30004b50:	4288      	cmp	r0, r1
30004b52:	d809      	bhi.n	30004b68 <BOOT_LoadImages+0x550>
30004b54:	2400      	movs	r4, #0
30004b56:	4633      	mov	r3, r6
30004b58:	ee19 2a10 	vmov	r2, s18
30004b5c:	ee18 1a90 	vmov	r1, s17
30004b60:	e9cd 5400 	strd	r5, r4, [sp]
30004b64:	4620      	mov	r0, r4
30004b66:	e00f      	b.n	30004b88 <BOOT_LoadImages+0x570>
30004b68:	4283      	cmp	r3, r0
30004b6a:	d825      	bhi.n	30004bb8 <BOOT_LoadImages+0x5a0>
30004b6c:	4282      	cmp	r2, r0
30004b6e:	d823      	bhi.n	30004bb8 <BOOT_LoadImages+0x5a0>
30004b70:	4288      	cmp	r0, r1
30004b72:	f04f 0400 	mov.w	r4, #0
30004b76:	d320      	bcc.n	30004bba <BOOT_LoadImages+0x5a2>
30004b78:	4633      	mov	r3, r6
30004b7a:	ee19 2a10 	vmov	r2, s18
30004b7e:	ee18 1a90 	vmov	r1, s17
30004b82:	2001      	movs	r0, #1
30004b84:	e9cd 5400 	strd	r5, r4, [sp]
30004b88:	f7ff f8a0 	bl	30003ccc <rewrite_bp>
30004b8c:	fa5f fa80 	uxtb.w	sl, r0
30004b90:	e7be      	b.n	30004b10 <BOOT_LoadImages+0x4f8>
30004b92:	4293      	cmp	r3, r2
30004b94:	f04f 0001 	mov.w	r0, #1
30004b98:	4633      	mov	r3, r6
30004b9a:	ee18 2a10 	vmov	r2, s16
30004b9e:	4639      	mov	r1, r7
30004ba0:	e9cd 5000 	strd	r5, r0, [sp]
30004ba4:	d3f0      	bcc.n	30004b88 <BOOT_LoadImages+0x570>
30004ba6:	e7dd      	b.n	30004b64 <BOOT_LoadImages+0x54c>
30004ba8:	4848      	ldr	r0, [pc, #288]	; (30004ccc <BOOT_LoadImages+0x6b4>)
30004baa:	f006 fc0d 	bl	3000b3c8 <__DiagPrintf_veneer>
30004bae:	e7fe      	b.n	30004bae <BOOT_LoadImages+0x596>
30004bb0:	2401      	movs	r4, #1
30004bb2:	e7ae      	b.n	30004b12 <BOOT_LoadImages+0x4fa>
30004bb4:	9403      	str	r4, [sp, #12]
30004bb6:	e7ac      	b.n	30004b12 <BOOT_LoadImages+0x4fa>
30004bb8:	2400      	movs	r4, #0
30004bba:	4626      	mov	r6, r4
30004bbc:	4b44      	ldr	r3, [pc, #272]	; (30004cd0 <BOOT_LoadImages+0x6b8>)
30004bbe:	4630      	mov	r0, r6
30004bc0:	9a07      	ldr	r2, [sp, #28]
30004bc2:	601a      	str	r2, [r3, #0]
30004bc4:	4b43      	ldr	r3, [pc, #268]	; (30004cd4 <BOOT_LoadImages+0x6bc>)
30004bc6:	4798      	blx	r3
30004bc8:	2800      	cmp	r0, #0
30004bca:	f000 808b 	beq.w	30004ce4 <BOOT_LoadImages+0x6cc>
30004bce:	2e01      	cmp	r6, #1
30004bd0:	f000 8097 	beq.w	30004d02 <BOOT_LoadImages+0x6ea>
30004bd4:	fab9 f989 	clz	r9, r9
30004bd8:	9b03      	ldr	r3, [sp, #12]
30004bda:	ea4f 1959 	mov.w	r9, r9, lsr #5
30004bde:	2b00      	cmp	r3, #0
30004be0:	f43f ae80 	beq.w	300048e4 <BOOT_LoadImages+0x2cc>
30004be4:	461e      	mov	r6, r3
30004be6:	4b38      	ldr	r3, [pc, #224]	; (30004cc8 <BOOT_LoadImages+0x6b0>)
30004be8:	f1b9 0f00 	cmp.w	r9, #0
30004bec:	f47f ae63 	bne.w	300048b6 <BOOT_LoadImages+0x29e>
30004bf0:	f8d3 3400 	ldr.w	r3, [r3, #1024]	; 0x400
30004bf4:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004bf8:	3201      	adds	r2, #1
30004bfa:	f47f ae4e 	bne.w	3000489a <BOOT_LoadImages+0x282>
30004bfe:	4836      	ldr	r0, [pc, #216]	; (30004cd8 <BOOT_LoadImages+0x6c0>)
30004c00:	464c      	mov	r4, r9
30004c02:	f006 fbe1 	bl	3000b3c8 <__DiagPrintf_veneer>
30004c06:	2301      	movs	r3, #1
30004c08:	9303      	str	r3, [sp, #12]
30004c0a:	2280      	movs	r2, #128	; 0x80
30004c0c:	21ff      	movs	r1, #255	; 0xff
30004c0e:	a80e      	add	r0, sp, #56	; 0x38
30004c10:	f006 fbd6 	bl	3000b3c0 <____wrap_memset_veneer>
30004c14:	2280      	movs	r2, #128	; 0x80
30004c16:	4629      	mov	r1, r5
30004c18:	a80e      	add	r0, sp, #56	; 0x38
30004c1a:	f006 fba1 	bl	3000b360 <____wrap_memcpy_veneer>
30004c1e:	2e00      	cmp	r6, #0
30004c20:	f43f aee2 	beq.w	300049e8 <BOOT_LoadImages+0x3d0>
30004c24:	f1b9 0f01 	cmp.w	r9, #1
30004c28:	f47f aee2 	bne.w	300049f0 <BOOT_LoadImages+0x3d8>
30004c2c:	2301      	movs	r3, #1
30004c2e:	f88d 3044 	strb.w	r3, [sp, #68]	; 0x44
30004c32:	482a      	ldr	r0, [pc, #168]	; (30004cdc <BOOT_LoadImages+0x6c4>)
30004c34:	f89d 1044 	ldrb.w	r1, [sp, #68]	; 0x44
30004c38:	f006 fbc6 	bl	3000b3c8 <__DiagPrintf_veneer>
30004c3c:	4629      	mov	r1, r5
30004c3e:	2002      	movs	r0, #2
30004c40:	4f21      	ldr	r7, [pc, #132]	; (30004cc8 <BOOT_LoadImages+0x6b0>)
30004c42:	f7fe ff6b 	bl	30003b1c <FLASH_Erase_With_Lock>
30004c46:	1d28      	adds	r0, r5, #4
30004c48:	aa0f      	add	r2, sp, #60	; 0x3c
30004c4a:	217c      	movs	r1, #124	; 0x7c
30004c4c:	9004      	str	r0, [sp, #16]
30004c4e:	f7fe ff8b 	bl	30003b68 <FLASH_TxData_With_Lock>
30004c52:	463a      	mov	r2, r7
30004c54:	f640 71fc 	movw	r1, #4092	; 0xffc
30004c58:	9804      	ldr	r0, [sp, #16]
30004c5a:	f7ff f805 	bl	30003c68 <mcc_crc32>
30004c5e:	4629      	mov	r1, r5
30004c60:	900e      	str	r0, [sp, #56]	; 0x38
30004c62:	2002      	movs	r0, #2
30004c64:	f7fe ff5a 	bl	30003b1c <FLASH_Erase_With_Lock>
30004c68:	aa0e      	add	r2, sp, #56	; 0x38
30004c6a:	2180      	movs	r1, #128	; 0x80
30004c6c:	4628      	mov	r0, r5
30004c6e:	f7fe ff7b 	bl	30003b68 <FLASH_TxData_With_Lock>
30004c72:	9b03      	ldr	r3, [sp, #12]
30004c74:	2b00      	cmp	r3, #0
30004c76:	f43f aecf 	beq.w	30004a18 <BOOT_LoadImages+0x400>
30004c7a:	2c00      	cmp	r4, #0
30004c7c:	d19a      	bne.n	30004bb4 <BOOT_LoadImages+0x59c>
30004c7e:	f1b9 0f01 	cmp.w	r9, #1
30004c82:	f47f aec1 	bne.w	30004a08 <BOOT_LoadImages+0x3f0>
30004c86:	f8d7 3400 	ldr.w	r3, [r7, #1024]	; 0x400
30004c8a:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004c8e:	3201      	adds	r2, #1
30004c90:	f47f aeb0 	bne.w	300049f4 <BOOT_LoadImages+0x3dc>
30004c94:	4810      	ldr	r0, [pc, #64]	; (30004cd8 <BOOT_LoadImages+0x6c0>)
30004c96:	f006 fb97 	bl	3000b3c8 <__DiagPrintf_veneer>
30004c9a:	4811      	ldr	r0, [pc, #68]	; (30004ce0 <BOOT_LoadImages+0x6c8>)
30004c9c:	f006 fb94 	bl	3000b3c8 <__DiagPrintf_veneer>
30004ca0:	e7fe      	b.n	30004ca0 <BOOT_LoadImages+0x688>
30004ca2:	f1ba 0f02 	cmp.w	sl, #2
30004ca6:	d189      	bne.n	30004bbc <BOOT_LoadImages+0x5a4>
30004ca8:	e704      	b.n	30004ab4 <BOOT_LoadImages+0x49c>
30004caa:	bf00      	nop
30004cac:	3000cb86 	.word	0x3000cb86
30004cb0:	3000c7eb 	.word	0x3000c7eb
30004cb4:	3000caf0 	.word	0x3000caf0
30004cb8:	3000cbc2 	.word	0x3000cbc2
30004cbc:	3000c7d4 	.word	0x3000c7d4
30004cc0:	3000cafe 	.word	0x3000cafe
30004cc4:	3000cb3a 	.word	0x3000cb3a
30004cc8:	3000d718 	.word	0x3000d718
30004ccc:	3000cc44 	.word	0x3000cc44
30004cd0:	3000eb78 	.word	0x3000eb78
30004cd4:	30006389 	.word	0x30006389
30004cd8:	3000cae2 	.word	0x3000cae2
30004cdc:	3000cc0e 	.word	0x3000cc0e
30004ce0:	3000c802 	.word	0x3000c802
30004ce4:	4b3e      	ldr	r3, [pc, #248]	; (30004de0 <BOOT_LoadImages+0x7c8>)
30004ce6:	781b      	ldrb	r3, [r3, #0]
30004ce8:	b12b      	cbz	r3, 30004cf6 <BOOT_LoadImages+0x6de>
30004cea:	4630      	mov	r0, r6
30004cec:	4b3d      	ldr	r3, [pc, #244]	; (30004de4 <BOOT_LoadImages+0x7cc>)
30004cee:	4798      	blx	r3
30004cf0:	2800      	cmp	r0, #0
30004cf2:	f47f af6c 	bne.w	30004bce <BOOT_LoadImages+0x5b6>
30004cf6:	2001      	movs	r0, #1
30004cf8:	b02f      	add	sp, #188	; 0xbc
30004cfa:	ecbd 8b08 	vpop	{d8-d11}
30004cfe:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004d02:	4c39      	ldr	r4, [pc, #228]	; (30004de8 <BOOT_LoadImages+0x7d0>)
30004d04:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004d08:	47a0      	blx	r4
30004d0a:	e7fb      	b.n	30004d04 <BOOT_LoadImages+0x6ec>
30004d0c:	f895 900c 	ldrb.w	r9, [r5, #12]
30004d10:	f8df b0e4 	ldr.w	fp, [pc, #228]	; 30004df8 <BOOT_LoadImages+0x7e0>
30004d14:	2c00      	cmp	r4, #0
30004d16:	f47f ada7 	bne.w	30004868 <BOOT_LoadImages+0x250>
30004d1a:	4834      	ldr	r0, [pc, #208]	; (30004dec <BOOT_LoadImages+0x7d4>)
30004d1c:	f006 fb54 	bl	3000b3c8 <__DiagPrintf_veneer>
30004d20:	ee18 3a90 	vmov	r3, s17
30004d24:	f8cb 3400 	str.w	r3, [fp, #1024]	; 0x400
30004d28:	ee19 3a10 	vmov	r3, s18
30004d2c:	f8cb 3404 	str.w	r3, [fp, #1028]	; 0x404
30004d30:	4649      	mov	r1, r9
30004d32:	482f      	ldr	r0, [pc, #188]	; (30004df0 <BOOT_LoadImages+0x7d8>)
30004d34:	f006 fb48 	bl	3000b3c8 <__DiagPrintf_veneer>
30004d38:	4b2e      	ldr	r3, [pc, #184]	; (30004df4 <BOOT_LoadImages+0x7dc>)
30004d3a:	4798      	blx	r3
30004d3c:	4b2e      	ldr	r3, [pc, #184]	; (30004df8 <BOOT_LoadImages+0x7e0>)
30004d3e:	f8d3 4400 	ldr.w	r4, [r3, #1024]	; 0x400
30004d42:	f8d3 1404 	ldr.w	r1, [r3, #1028]	; 0x404
30004d46:	f8d4 200a 	ldr.w	r2, [r4, #10]
30004d4a:	f8d1 300a 	ldr.w	r3, [r1, #10]
30004d4e:	320c      	adds	r2, #12
30004d50:	330c      	adds	r3, #12
30004d52:	4542      	cmp	r2, r8
30004d54:	bf28      	it	cs
30004d56:	4642      	movcs	r2, r8
30004d58:	4598      	cmp	r8, r3
30004d5a:	bf28      	it	cs
30004d5c:	4698      	movcs	r8, r3
30004d5e:	6823      	ldr	r3, [r4, #0]
30004d60:	ee0a 2a90 	vmov	s21, r2
30004d64:	ee09 3a90 	vmov	s19, r3
30004d68:	680b      	ldr	r3, [r1, #0]
30004d6a:	ee0b 8a10 	vmov	s22, r8
30004d6e:	ee0a 3a10 	vmov	s20, r3
30004d72:	2800      	cmp	r0, #0
30004d74:	f47f ad8d 	bne.w	30004892 <BOOT_LoadImages+0x27a>
30004d78:	4682      	mov	sl, r0
30004d7a:	4604      	mov	r4, r0
30004d7c:	4606      	mov	r6, r0
30004d7e:	4f1e      	ldr	r7, [pc, #120]	; (30004df8 <BOOT_LoadImages+0x7e0>)
30004d80:	f1b9 0f00 	cmp.w	r9, #0
30004d84:	f47f ae02 	bne.w	3000498c <BOOT_LoadImages+0x374>
30004d88:	f8d7 8400 	ldr.w	r8, [r7, #1024]	; 0x400
30004d8c:	463a      	mov	r2, r7
30004d8e:	ee1a 1a90 	vmov	r1, s21
30004d92:	f108 0004 	add.w	r0, r8, #4
30004d96:	f7fe ff67 	bl	30003c68 <mcc_crc32>
30004d9a:	ee19 3a90 	vmov	r3, s19
30004d9e:	4602      	mov	r2, r0
30004da0:	4298      	cmp	r0, r3
30004da2:	f47f ada1 	bne.w	300048e8 <BOOT_LoadImages+0x2d0>
30004da6:	ee19 2a90 	vmov	r2, s19
30004daa:	ee19 1a90 	vmov	r1, s19
30004dae:	4813      	ldr	r0, [pc, #76]	; (30004dfc <BOOT_LoadImages+0x7e4>)
30004db0:	f04f 0a01 	mov.w	sl, #1
30004db4:	f006 fb08 	bl	3000b3c8 <__DiagPrintf_veneer>
30004db8:	f8d7 3400 	ldr.w	r3, [r7, #1024]	; 0x400
30004dbc:	4810      	ldr	r0, [pc, #64]	; (30004e00 <BOOT_LoadImages+0x7e8>)
30004dbe:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004dc2:	f006 fb01 	bl	3000b3c8 <__DiagPrintf_veneer>
30004dc6:	f8cd 900c 	str.w	r9, [sp, #12]
30004dca:	e56f      	b.n	300048ac <BOOT_LoadImages+0x294>
30004dcc:	2c00      	cmp	r4, #0
30004dce:	f47f ad27 	bne.w	30004820 <BOOT_LoadImages+0x208>
30004dd2:	e534      	b.n	3000483e <BOOT_LoadImages+0x226>
30004dd4:	428b      	cmp	r3, r1
30004dd6:	e6ba      	b.n	30004b4e <BOOT_LoadImages+0x536>
30004dd8:	4293      	cmp	r3, r2
30004dda:	f47f aeb7 	bne.w	30004b4c <BOOT_LoadImages+0x534>
30004dde:	e6a6      	b.n	30004b2e <BOOT_LoadImages+0x516>
30004de0:	3000d553 	.word	0x3000d553
30004de4:	3000638d 	.word	0x3000638d
30004de8:	00009be5 	.word	0x00009be5
30004dec:	3000caa1 	.word	0x3000caa1
30004df0:	3000cad0 	.word	0x3000cad0
30004df4:	0000c149 	.word	0x0000c149
30004df8:	3000d718 	.word	0x3000d718
30004dfc:	3000cafe 	.word	0x3000cafe
30004e00:	3000c7d4 	.word	0x3000c7d4

30004e04 <BOOT_ReasonSet>:
30004e04:	4a03      	ldr	r2, [pc, #12]	; (30004e14 <BOOT_ReasonSet+0x10>)
30004e06:	8c13      	ldrh	r3, [r2, #32]
30004e08:	b29b      	uxth	r3, r3
30004e0a:	8413      	strh	r3, [r2, #32]
30004e0c:	f8a2 3264 	strh.w	r3, [r2, #612]	; 0x264
30004e10:	4770      	bx	lr
30004e12:	bf00      	nop
30004e14:	42008000 	.word	0x42008000

30004e18 <BOOT_Enable_KM0>:
30004e18:	b510      	push	{r4, lr}
30004e1a:	4c11      	ldr	r4, [pc, #68]	; (30004e60 <BOOT_Enable_KM0+0x48>)
30004e1c:	47a0      	blx	r4
30004e1e:	b150      	cbz	r0, 30004e36 <BOOT_Enable_KM0+0x1e>
30004e20:	4b10      	ldr	r3, [pc, #64]	; (30004e64 <BOOT_Enable_KM0+0x4c>)
30004e22:	4798      	blx	r3
30004e24:	b938      	cbnz	r0, 30004e36 <BOOT_Enable_KM0+0x1e>
30004e26:	4b10      	ldr	r3, [pc, #64]	; (30004e68 <BOOT_Enable_KM0+0x50>)
30004e28:	2004      	movs	r0, #4
30004e2a:	4a10      	ldr	r2, [pc, #64]	; (30004e6c <BOOT_Enable_KM0+0x54>)
30004e2c:	4619      	mov	r1, r3
30004e2e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004e32:	f006 b873 	b.w	3000af1c <rtk_log_write>
30004e36:	47a0      	blx	r4
30004e38:	b140      	cbz	r0, 30004e4c <BOOT_Enable_KM0+0x34>
30004e3a:	4b0a      	ldr	r3, [pc, #40]	; (30004e64 <BOOT_Enable_KM0+0x4c>)
30004e3c:	4798      	blx	r3
30004e3e:	b128      	cbz	r0, 30004e4c <BOOT_Enable_KM0+0x34>
30004e40:	4b09      	ldr	r3, [pc, #36]	; (30004e68 <BOOT_Enable_KM0+0x50>)
30004e42:	2004      	movs	r0, #4
30004e44:	4a0a      	ldr	r2, [pc, #40]	; (30004e70 <BOOT_Enable_KM0+0x58>)
30004e46:	4619      	mov	r1, r3
30004e48:	f006 f868 	bl	3000af1c <rtk_log_write>
30004e4c:	4a09      	ldr	r2, [pc, #36]	; (30004e74 <BOOT_Enable_KM0+0x5c>)
30004e4e:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
30004e52:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
30004e56:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
30004e5a:	bf40      	sev
30004e5c:	bd10      	pop	{r4, pc}
30004e5e:	bf00      	nop
30004e60:	0000c09d 	.word	0x0000c09d
30004e64:	0000c0ad 	.word	0x0000c0ad
30004e68:	3000c714 	.word	0x3000c714
30004e6c:	3000cc70 	.word	0x3000cc70
30004e70:	3000cc98 	.word	0x3000cc98
30004e74:	42008000 	.word	0x42008000

30004e78 <BOOT_AP_Clk_Get>:
30004e78:	4a05      	ldr	r2, [pc, #20]	; (30004e90 <BOOT_AP_Clk_Get+0x18>)
30004e7a:	6913      	ldr	r3, [r2, #16]
30004e7c:	2b00      	cmp	r3, #0
30004e7e:	bfba      	itte	lt
30004e80:	f023 4300 	biclt.w	r3, r3, #2147483648	; 0x80000000
30004e84:	6850      	ldrlt	r0, [r2, #4]
30004e86:	6810      	ldrge	r0, [r2, #0]
30004e88:	fbb0 f0f3 	udiv	r0, r0, r3
30004e8c:	4770      	bx	lr
30004e8e:	bf00      	nop
30004e90:	3000dd9c 	.word	0x3000dd9c

30004e94 <BOOT_SOC_ClkSet>:
30004e94:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004e98:	4f57      	ldr	r7, [pc, #348]	; (30004ff8 <BOOT_SOC_ClkSet+0x164>)
30004e9a:	f7ff ffed 	bl	30004e78 <BOOT_AP_Clk_Get>
30004e9e:	f44f 7bc8 	mov.w	fp, #400	; 0x190
30004ea2:	2564      	movs	r5, #100	; 0x64
30004ea4:	683c      	ldr	r4, [r7, #0]
30004ea6:	4606      	mov	r6, r0
30004ea8:	68fb      	ldr	r3, [r7, #12]
30004eaa:	f8d7 a008 	ldr.w	sl, [r7, #8]
30004eae:	f8df 9178 	ldr.w	r9, [pc, #376]	; 30005028 <BOOT_SOC_ClkSet+0x194>
30004eb2:	f1ba 0f01 	cmp.w	sl, #1
30004eb6:	fbb4 f8f3 	udiv	r8, r4, r3
30004eba:	f04f 03c8 	mov.w	r3, #200	; 0xc8
30004ebe:	fbb4 fbfb 	udiv	fp, r4, fp
30004ec2:	fbb4 f5f5 	udiv	r5, r4, r5
30004ec6:	fbb4 f4f3 	udiv	r4, r4, r3
30004eca:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30004ece:	f105 35ff 	add.w	r5, r5, #4294967295	; 0xffffffff
30004ed2:	f104 34ff 	add.w	r4, r4, #4294967295	; 0xffffffff
30004ed6:	4b49      	ldr	r3, [pc, #292]	; (30004ffc <BOOT_SOC_ClkSet+0x168>)
30004ed8:	d16f      	bne.n	30004fba <BOOT_SOC_ClkSet+0x126>
30004eda:	4650      	mov	r0, sl
30004edc:	4a48      	ldr	r2, [pc, #288]	; (30005000 <BOOT_SOC_ClkSet+0x16c>)
30004ede:	4790      	blx	r2
30004ee0:	4b46      	ldr	r3, [pc, #280]	; (30004ffc <BOOT_SOC_ClkSet+0x168>)
30004ee2:	f5b8 7fa7 	cmp.w	r8, #334	; 0x14e
30004ee6:	f883 a10c 	strb.w	sl, [r3, #268]	; 0x10c
30004eea:	d304      	bcc.n	30004ef6 <BOOT_SOC_ClkSet+0x62>
30004eec:	f44f 61d6 	mov.w	r1, #1712	; 0x6b0
30004ef0:	4844      	ldr	r0, [pc, #272]	; (30005004 <BOOT_SOC_ClkSet+0x170>)
30004ef2:	f006 faa5 	bl	3000b440 <__io_assert_failed_veneer>
30004ef6:	f899 3000 	ldrb.w	r3, [r9]
30004efa:	2b01      	cmp	r3, #1
30004efc:	d107      	bne.n	30004f0e <BOOT_SOC_ClkSet+0x7a>
30004efe:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
30004f02:	d904      	bls.n	30004f0e <BOOT_SOC_ClkSet+0x7a>
30004f04:	f240 61b2 	movw	r1, #1714	; 0x6b2
30004f08:	483e      	ldr	r0, [pc, #248]	; (30005004 <BOOT_SOC_ClkSet+0x170>)
30004f0a:	f006 fa99 	bl	3000b440 <__io_assert_failed_veneer>
30004f0e:	2001      	movs	r0, #1
30004f10:	f8df a118 	ldr.w	sl, [pc, #280]	; 3000502c <BOOT_SOC_ClkSet+0x198>
30004f14:	f002 fa52 	bl	300073bc <CLK_SWITCH_XTAL>
30004f18:	6838      	ldr	r0, [r7, #0]
30004f1a:	4b3b      	ldr	r3, [pc, #236]	; (30005008 <BOOT_SOC_ClkSet+0x174>)
30004f1c:	ea4f 2b0b 	mov.w	fp, fp, lsl #8
30004f20:	fb0a f000 	mul.w	r0, sl, r0
30004f24:	032d      	lsls	r5, r5, #12
30004f26:	4798      	blx	r3
30004f28:	4b38      	ldr	r3, [pc, #224]	; (3000500c <BOOT_SOC_ClkSet+0x178>)
30004f2a:	4939      	ldr	r1, [pc, #228]	; (30005010 <BOOT_SOC_ClkSet+0x17c>)
30004f2c:	f40b 6be0 	and.w	fp, fp, #1792	; 0x700
30004f30:	f8d3 2228 	ldr.w	r2, [r3, #552]	; 0x228
30004f34:	b2ad      	uxth	r5, r5
30004f36:	0524      	lsls	r4, r4, #20
30004f38:	2000      	movs	r0, #0
30004f3a:	4011      	ands	r1, r2
30004f3c:	68fa      	ldr	r2, [r7, #12]
30004f3e:	f404 04e0 	and.w	r4, r4, #7340032	; 0x700000
30004f42:	3a01      	subs	r2, #1
30004f44:	f002 0207 	and.w	r2, r2, #7
30004f48:	430a      	orrs	r2, r1
30004f4a:	ea4b 0b02 	orr.w	fp, fp, r2
30004f4e:	ea45 050b 	orr.w	r5, r5, fp
30004f52:	432c      	orrs	r4, r5
30004f54:	f8c3 4228 	str.w	r4, [r3, #552]	; 0x228
30004f58:	f002 fa30 	bl	300073bc <CLK_SWITCH_XTAL>
30004f5c:	4b2d      	ldr	r3, [pc, #180]	; (30005014 <BOOT_SOC_ClkSet+0x180>)
30004f5e:	4a2e      	ldr	r2, [pc, #184]	; (30005018 <BOOT_SOC_ClkSet+0x184>)
30004f60:	2004      	movs	r0, #4
30004f62:	4619      	mov	r1, r3
30004f64:	f8cd 8000 	str.w	r8, [sp]
30004f68:	f005 ffd8 	bl	3000af1c <rtk_log_write>
30004f6c:	f899 3000 	ldrb.w	r3, [r9]
30004f70:	b303      	cbz	r3, 30004fb4 <BOOT_SOC_ClkSet+0x120>
30004f72:	693b      	ldr	r3, [r7, #16]
30004f74:	2b00      	cmp	r3, #0
30004f76:	da35      	bge.n	30004fe4 <BOOT_SOC_ClkSet+0x150>
30004f78:	6878      	ldr	r0, [r7, #4]
30004f7a:	4b28      	ldr	r3, [pc, #160]	; (3000501c <BOOT_SOC_ClkSet+0x188>)
30004f7c:	fb0a f000 	mul.w	r0, sl, r0
30004f80:	4798      	blx	r3
30004f82:	4b27      	ldr	r3, [pc, #156]	; (30005020 <BOOT_SOC_ClkSet+0x18c>)
30004f84:	2001      	movs	r0, #1
30004f86:	4798      	blx	r3
30004f88:	4b1c      	ldr	r3, [pc, #112]	; (30004ffc <BOOT_SOC_ClkSet+0x168>)
30004f8a:	2201      	movs	r2, #1
30004f8c:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30004f90:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30004f94:	693b      	ldr	r3, [r7, #16]
30004f96:	2004      	movs	r0, #4
30004f98:	690a      	ldr	r2, [r1, #16]
30004f9a:	3b01      	subs	r3, #1
30004f9c:	f022 0203 	bic.w	r2, r2, #3
30004fa0:	f003 0303 	and.w	r3, r3, #3
30004fa4:	4313      	orrs	r3, r2
30004fa6:	4a1f      	ldr	r2, [pc, #124]	; (30005024 <BOOT_SOC_ClkSet+0x190>)
30004fa8:	610b      	str	r3, [r1, #16]
30004faa:	4b1a      	ldr	r3, [pc, #104]	; (30005014 <BOOT_SOC_ClkSet+0x180>)
30004fac:	9600      	str	r6, [sp, #0]
30004fae:	4619      	mov	r1, r3
30004fb0:	f005 ffb4 	bl	3000af1c <rtk_log_write>
30004fb4:	b003      	add	sp, #12
30004fb6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004fba:	2200      	movs	r2, #0
30004fbc:	f1b8 0ffa 	cmp.w	r8, #250	; 0xfa
30004fc0:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30004fc4:	d904      	bls.n	30004fd0 <BOOT_SOC_ClkSet+0x13c>
30004fc6:	f240 61b6 	movw	r1, #1718	; 0x6b6
30004fca:	480e      	ldr	r0, [pc, #56]	; (30005004 <BOOT_SOC_ClkSet+0x170>)
30004fcc:	f006 fa38 	bl	3000b440 <__io_assert_failed_veneer>
30004fd0:	f899 3000 	ldrb.w	r3, [r9]
30004fd4:	2b01      	cmp	r3, #1
30004fd6:	d19a      	bne.n	30004f0e <BOOT_SOC_ClkSet+0x7a>
30004fd8:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
30004fdc:	d997      	bls.n	30004f0e <BOOT_SOC_ClkSet+0x7a>
30004fde:	f44f 61d7 	mov.w	r1, #1720	; 0x6b8
30004fe2:	e791      	b.n	30004f08 <BOOT_SOC_ClkSet+0x74>
30004fe4:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30004fe8:	6913      	ldr	r3, [r2, #16]
30004fea:	f043 030c 	orr.w	r3, r3, #12
30004fee:	6113      	str	r3, [r2, #16]
30004ff0:	2200      	movs	r2, #0
30004ff2:	4b02      	ldr	r3, [pc, #8]	; (30004ffc <BOOT_SOC_ClkSet+0x168>)
30004ff4:	e7ca      	b.n	30004f8c <BOOT_SOC_ClkSet+0xf8>
30004ff6:	bf00      	nop
30004ff8:	3000dd9c 	.word	0x3000dd9c
30004ffc:	23020000 	.word	0x23020000
30005000:	3000a959 	.word	0x3000a959
30005004:	3000cf30 	.word	0x3000cf30
30005008:	30007fe1 	.word	0x30007fe1
3000500c:	42008000 	.word	0x42008000
30005010:	ff8f08f8 	.word	0xff8f08f8
30005014:	3000c714 	.word	0x3000c714
30005018:	3000ccb5 	.word	0x3000ccb5
3000501c:	3000805d 	.word	0x3000805d
30005020:	30008099 	.word	0x30008099
30005024:	3000cccc 	.word	0x3000cccc
30005028:	3000d553 	.word	0x3000d553
3000502c:	000f4240 	.word	0x000f4240

30005030 <BOOT_Disable_AP>:
30005030:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005034:	685a      	ldr	r2, [r3, #4]
30005036:	f042 0202 	orr.w	r2, r2, #2
3000503a:	605a      	str	r2, [r3, #4]
3000503c:	685a      	ldr	r2, [r3, #4]
3000503e:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30005042:	605a      	str	r2, [r3, #4]
30005044:	4a02      	ldr	r2, [pc, #8]	; (30005050 <BOOT_Disable_AP+0x20>)
30005046:	6f93      	ldr	r3, [r2, #120]	; 0x78
30005048:	f043 0303 	orr.w	r3, r3, #3
3000504c:	6793      	str	r3, [r2, #120]	; 0x78
3000504e:	4770      	bx	lr
30005050:	41000200 	.word	0x41000200

30005054 <BOOT_Enable_AP>:
30005054:	b537      	push	{r0, r1, r2, r4, r5, lr}
30005056:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
3000505a:	2104      	movs	r1, #4
3000505c:	2201      	movs	r2, #1
3000505e:	482b      	ldr	r0, [pc, #172]	; (3000510c <BOOT_Enable_AP+0xb8>)
30005060:	4b2b      	ldr	r3, [pc, #172]	; (30005110 <BOOT_Enable_AP+0xbc>)
30005062:	4798      	blx	r3
30005064:	6863      	ldr	r3, [r4, #4]
30005066:	4d2b      	ldr	r5, [pc, #172]	; (30005114 <BOOT_Enable_AP+0xc0>)
30005068:	2032      	movs	r0, #50	; 0x32
3000506a:	f043 0302 	orr.w	r3, r3, #2
3000506e:	6063      	str	r3, [r4, #4]
30005070:	6863      	ldr	r3, [r4, #4]
30005072:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30005076:	6063      	str	r3, [r4, #4]
30005078:	6823      	ldr	r3, [r4, #0]
3000507a:	f043 0302 	orr.w	r3, r3, #2
3000507e:	6023      	str	r3, [r4, #0]
30005080:	47a8      	blx	r5
30005082:	6823      	ldr	r3, [r4, #0]
30005084:	2032      	movs	r0, #50	; 0x32
30005086:	f043 0304 	orr.w	r3, r3, #4
3000508a:	6023      	str	r3, [r4, #0]
3000508c:	6823      	ldr	r3, [r4, #0]
3000508e:	f043 0301 	orr.w	r3, r3, #1
30005092:	6023      	str	r3, [r4, #0]
30005094:	6823      	ldr	r3, [r4, #0]
30005096:	f043 0310 	orr.w	r3, r3, #16
3000509a:	6023      	str	r3, [r4, #0]
3000509c:	47a8      	blx	r5
3000509e:	6823      	ldr	r3, [r4, #0]
300050a0:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
300050a4:	f043 0340 	orr.w	r3, r3, #64	; 0x40
300050a8:	6023      	str	r3, [r4, #0]
300050aa:	47a8      	blx	r5
300050ac:	6863      	ldr	r3, [r4, #4]
300050ae:	f023 0310 	bic.w	r3, r3, #16
300050b2:	6063      	str	r3, [r4, #4]
300050b4:	6863      	ldr	r3, [r4, #4]
300050b6:	f023 0302 	bic.w	r3, r3, #2
300050ba:	6063      	str	r3, [r4, #4]
300050bc:	4b16      	ldr	r3, [pc, #88]	; (30005118 <BOOT_Enable_AP+0xc4>)
300050be:	4798      	blx	r3
300050c0:	4601      	mov	r1, r0
300050c2:	b990      	cbnz	r0, 300050ea <BOOT_Enable_AP+0x96>
300050c4:	4c15      	ldr	r4, [pc, #84]	; (3000511c <BOOT_Enable_AP+0xc8>)
300050c6:	9001      	str	r0, [sp, #4]
300050c8:	2041      	movs	r0, #65	; 0x41
300050ca:	47a0      	blx	r4
300050cc:	9901      	ldr	r1, [sp, #4]
300050ce:	2042      	movs	r0, #66	; 0x42
300050d0:	47a0      	blx	r4
300050d2:	9901      	ldr	r1, [sp, #4]
300050d4:	2043      	movs	r0, #67	; 0x43
300050d6:	47a0      	blx	r4
300050d8:	9901      	ldr	r1, [sp, #4]
300050da:	2044      	movs	r0, #68	; 0x44
300050dc:	47a0      	blx	r4
300050de:	9901      	ldr	r1, [sp, #4]
300050e0:	2045      	movs	r0, #69	; 0x45
300050e2:	47a0      	blx	r4
300050e4:	9901      	ldr	r1, [sp, #4]
300050e6:	2046      	movs	r0, #70	; 0x46
300050e8:	47a0      	blx	r4
300050ea:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300050ee:	68da      	ldr	r2, [r3, #12]
300050f0:	f042 0201 	orr.w	r2, r2, #1
300050f4:	60da      	str	r2, [r3, #12]
300050f6:	689a      	ldr	r2, [r3, #8]
300050f8:	f042 0201 	orr.w	r2, r2, #1
300050fc:	609a      	str	r2, [r3, #8]
300050fe:	4a08      	ldr	r2, [pc, #32]	; (30005120 <BOOT_Enable_AP+0xcc>)
30005100:	4b08      	ldr	r3, [pc, #32]	; (30005124 <BOOT_Enable_AP+0xd0>)
30005102:	6851      	ldr	r1, [r2, #4]
30005104:	430b      	orrs	r3, r1
30005106:	6053      	str	r3, [r2, #4]
30005108:	b003      	add	sp, #12
3000510a:	bd30      	pop	{r4, r5, pc}
3000510c:	4200c000 	.word	0x4200c000
30005110:	0000aab9 	.word	0x0000aab9
30005114:	00009b2d 	.word	0x00009b2d
30005118:	3000903d 	.word	0x3000903d
3000511c:	0000b20d 	.word	0x0000b20d
30005120:	41000200 	.word	0x41000200
30005124:	01001111 	.word	0x01001111

30005128 <BOOT_RAM_KeyDeriveFunc>:
30005128:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000512c:	ed2d 8b02 	vpush	{d8}
30005130:	b0cb      	sub	sp, #300	; 0x12c
30005132:	4f61      	ldr	r7, [pc, #388]	; (300052b8 <BOOT_RAM_KeyDeriveFunc+0x190>)
30005134:	4605      	mov	r5, r0
30005136:	460e      	mov	r6, r1
30005138:	4860      	ldr	r0, [pc, #384]	; (300052bc <BOOT_RAM_KeyDeriveFunc+0x194>)
3000513a:	ee08 2a10 	vmov	s16, r2
3000513e:	4960      	ldr	r1, [pc, #384]	; (300052c0 <BOOT_RAM_KeyDeriveFunc+0x198>)
30005140:	2201      	movs	r2, #1
30005142:	469b      	mov	fp, r3
30005144:	9c57      	ldr	r4, [sp, #348]	; 0x15c
30005146:	47b8      	blx	r7
30005148:	2201      	movs	r2, #1
3000514a:	495e      	ldr	r1, [pc, #376]	; (300052c4 <BOOT_RAM_KeyDeriveFunc+0x19c>)
3000514c:	485c      	ldr	r0, [pc, #368]	; (300052c0 <BOOT_RAM_KeyDeriveFunc+0x198>)
3000514e:	47b8      	blx	r7
30005150:	4b5d      	ldr	r3, [pc, #372]	; (300052c8 <BOOT_RAM_KeyDeriveFunc+0x1a0>)
30005152:	2000      	movs	r0, #0
30005154:	4f5d      	ldr	r7, [pc, #372]	; (300052cc <BOOT_RAM_KeyDeriveFunc+0x1a4>)
30005156:	4798      	blx	r3
30005158:	22e0      	movs	r2, #224	; 0xe0
3000515a:	2100      	movs	r1, #0
3000515c:	a812      	add	r0, sp, #72	; 0x48
3000515e:	f006 f92f 	bl	3000b3c0 <____wrap_memset_veneer>
30005162:	2300      	movs	r3, #0
30005164:	f8df 9170 	ldr.w	r9, [pc, #368]	; 300052d8 <BOOT_RAM_KeyDeriveFunc+0x1b0>
30005168:	f8ad 3004 	strh.w	r3, [sp, #4]
3000516c:	f88d 3006 	strb.w	r3, [sp, #6]
30005170:	2301      	movs	r3, #1
30005172:	f88d 3007 	strb.w	r3, [sp, #7]
30005176:	2c00      	cmp	r4, #0
30005178:	d140      	bne.n	300051fc <BOOT_RAM_KeyDeriveFunc+0xd4>
3000517a:	2220      	movs	r2, #32
3000517c:	4621      	mov	r1, r4
3000517e:	4d54      	ldr	r5, [pc, #336]	; (300052d0 <BOOT_RAM_KeyDeriveFunc+0x1a8>)
30005180:	a802      	add	r0, sp, #8
30005182:	47a8      	blx	r5
30005184:	2220      	movs	r2, #32
30005186:	4621      	mov	r1, r4
30005188:	a80a      	add	r0, sp, #40	; 0x28
3000518a:	47a8      	blx	r5
3000518c:	4951      	ldr	r1, [pc, #324]	; (300052d4 <BOOT_RAM_KeyDeriveFunc+0x1ac>)
3000518e:	694b      	ldr	r3, [r1, #20]
30005190:	03da      	lsls	r2, r3, #15
30005192:	d514      	bpl.n	300051be <BOOT_RAM_KeyDeriveFunc+0x96>
30005194:	ab02      	add	r3, sp, #8
30005196:	f013 021f 	ands.w	r2, r3, #31
3000519a:	bf1a      	itte	ne
3000519c:	f023 031f 	bicne.w	r3, r3, #31
300051a0:	3220      	addne	r2, #32
300051a2:	2220      	moveq	r2, #32
300051a4:	f3bf 8f4f 	dsb	sy
300051a8:	441a      	add	r2, r3
300051aa:	f8c1 3270 	str.w	r3, [r1, #624]	; 0x270
300051ae:	3320      	adds	r3, #32
300051b0:	1ad0      	subs	r0, r2, r3
300051b2:	2800      	cmp	r0, #0
300051b4:	dcf9      	bgt.n	300051aa <BOOT_RAM_KeyDeriveFunc+0x82>
300051b6:	f3bf 8f4f 	dsb	sy
300051ba:	f3bf 8f6f 	isb	sy
300051be:	4945      	ldr	r1, [pc, #276]	; (300052d4 <BOOT_RAM_KeyDeriveFunc+0x1ac>)
300051c0:	694b      	ldr	r3, [r1, #20]
300051c2:	03db      	lsls	r3, r3, #15
300051c4:	d514      	bpl.n	300051f0 <BOOT_RAM_KeyDeriveFunc+0xc8>
300051c6:	ab0a      	add	r3, sp, #40	; 0x28
300051c8:	f013 021f 	ands.w	r2, r3, #31
300051cc:	bf1a      	itte	ne
300051ce:	f023 031f 	bicne.w	r3, r3, #31
300051d2:	3220      	addne	r2, #32
300051d4:	2220      	moveq	r2, #32
300051d6:	f3bf 8f4f 	dsb	sy
300051da:	441a      	add	r2, r3
300051dc:	f8c1 3270 	str.w	r3, [r1, #624]	; 0x270
300051e0:	3320      	adds	r3, #32
300051e2:	1ad0      	subs	r0, r2, r3
300051e4:	2800      	cmp	r0, #0
300051e6:	dcf9      	bgt.n	300051dc <BOOT_RAM_KeyDeriveFunc+0xb4>
300051e8:	f3bf 8f4f 	dsb	sy
300051ec:	f3bf 8f6f 	isb	sy
300051f0:	2000      	movs	r0, #0
300051f2:	b04b      	add	sp, #300	; 0x12c
300051f4:	ecbd 8b02 	vpop	{d8}
300051f8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300051fc:	ab12      	add	r3, sp, #72	; 0x48
300051fe:	4632      	mov	r2, r6
30005200:	4629      	mov	r1, r5
30005202:	2020      	movs	r0, #32
30005204:	47b8      	blx	r7
30005206:	aa12      	add	r2, sp, #72	; 0x48
30005208:	4659      	mov	r1, fp
3000520a:	ee18 0a10 	vmov	r0, s16
3000520e:	47c8      	blx	r9
30005210:	2104      	movs	r1, #4
30005212:	aa12      	add	r2, sp, #72	; 0x48
30005214:	f8df 80c4 	ldr.w	r8, [pc, #196]	; 300052dc <BOOT_RAM_KeyDeriveFunc+0x1b4>
30005218:	eb0d 0001 	add.w	r0, sp, r1
3000521c:	f04f 0a01 	mov.w	sl, #1
30005220:	47c8      	blx	r9
30005222:	a912      	add	r1, sp, #72	; 0x48
30005224:	a80a      	add	r0, sp, #40	; 0x28
30005226:	47c0      	blx	r8
30005228:	f10d 0c28 	add.w	ip, sp, #40	; 0x28
3000522c:	f10d 0e08 	add.w	lr, sp, #8
30005230:	e8bc 000f 	ldmia.w	ip!, {r0, r1, r2, r3}
30005234:	e8ae 000f 	stmia.w	lr!, {r0, r1, r2, r3}
30005238:	e89c 000f 	ldmia.w	ip, {r0, r1, r2, r3}
3000523c:	e88e 000f 	stmia.w	lr, {r0, r1, r2, r3}
30005240:	9b56      	ldr	r3, [sp, #344]	; 0x158
30005242:	459a      	cmp	sl, r3
30005244:	d31c      	bcc.n	30005280 <BOOT_RAM_KeyDeriveFunc+0x158>
30005246:	2c20      	cmp	r4, #32
30005248:	46a0      	mov	r8, r4
3000524a:	a90a      	add	r1, sp, #40	; 0x28
3000524c:	9858      	ldr	r0, [sp, #352]	; 0x160
3000524e:	bf28      	it	cs
30005250:	f04f 0820 	movcs.w	r8, #32
30005254:	4642      	mov	r2, r8
30005256:	eba4 0408 	sub.w	r4, r4, r8
3000525a:	f006 f881 	bl	3000b360 <____wrap_memcpy_veneer>
3000525e:	9b58      	ldr	r3, [sp, #352]	; 0x160
30005260:	f10d 0107 	add.w	r1, sp, #7
30005264:	2204      	movs	r2, #4
30005266:	4443      	add	r3, r8
30005268:	9358      	str	r3, [sp, #352]	; 0x160
3000526a:	780b      	ldrb	r3, [r1, #0]
3000526c:	3301      	adds	r3, #1
3000526e:	b2db      	uxtb	r3, r3
30005270:	f801 3901 	strb.w	r3, [r1], #-1
30005274:	2b00      	cmp	r3, #0
30005276:	f47f af7e 	bne.w	30005176 <BOOT_RAM_KeyDeriveFunc+0x4e>
3000527a:	3a01      	subs	r2, #1
3000527c:	d1f5      	bne.n	3000526a <BOOT_RAM_KeyDeriveFunc+0x142>
3000527e:	e77a      	b.n	30005176 <BOOT_RAM_KeyDeriveFunc+0x4e>
30005280:	ab12      	add	r3, sp, #72	; 0x48
30005282:	4632      	mov	r2, r6
30005284:	4629      	mov	r1, r5
30005286:	2020      	movs	r0, #32
30005288:	47b8      	blx	r7
3000528a:	aa12      	add	r2, sp, #72	; 0x48
3000528c:	2120      	movs	r1, #32
3000528e:	a802      	add	r0, sp, #8
30005290:	47c8      	blx	r9
30005292:	a912      	add	r1, sp, #72	; 0x48
30005294:	a802      	add	r0, sp, #8
30005296:	47c0      	blx	r8
30005298:	a90a      	add	r1, sp, #40	; 0x28
3000529a:	ab02      	add	r3, sp, #8
3000529c:	2220      	movs	r2, #32
3000529e:	7808      	ldrb	r0, [r1, #0]
300052a0:	3a01      	subs	r2, #1
300052a2:	f813 cb01 	ldrb.w	ip, [r3], #1
300052a6:	ea80 000c 	eor.w	r0, r0, ip
300052aa:	f801 0b01 	strb.w	r0, [r1], #1
300052ae:	d1f6      	bne.n	3000529e <BOOT_RAM_KeyDeriveFunc+0x176>
300052b0:	f10a 0a01 	add.w	sl, sl, #1
300052b4:	e7c4      	b.n	30005240 <BOOT_RAM_KeyDeriveFunc+0x118>
300052b6:	bf00      	nop
300052b8:	0000b479 	.word	0x0000b479
300052bc:	40000008 	.word	0x40000008
300052c0:	40000004 	.word	0x40000004
300052c4:	40000002 	.word	0x40000002
300052c8:	30009291 	.word	0x30009291
300052cc:	300089c5 	.word	0x300089c5
300052d0:	00012be5 	.word	0x00012be5
300052d4:	e000ed00 	.word	0xe000ed00
300052d8:	300090cd 	.word	0x300090cd
300052dc:	30009105 	.word	0x30009105

300052e0 <password_hash_check>:
300052e0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300052e4:	b091      	sub	sp, #68	; 0x44
300052e6:	4614      	mov	r4, r2
300052e8:	4d3e      	ldr	r5, [pc, #248]	; (300053e4 <password_hash_check+0x104>)
300052ea:	4606      	mov	r6, r0
300052ec:	af04      	add	r7, sp, #16
300052ee:	e9c7 1302 	strd	r1, r3, [r7, #8]
300052f2:	f102 0308 	add.w	r3, r2, #8
300052f6:	f8d7 b058 	ldr.w	fp, [r7, #88]	; 0x58
300052fa:	2100      	movs	r1, #0
300052fc:	f023 0307 	bic.w	r3, r3, #7
30005300:	f10b 0208 	add.w	r2, fp, #8
30005304:	ebad 0d03 	sub.w	sp, sp, r3
30005308:	f022 0207 	bic.w	r2, r2, #7
3000530c:	6dbb      	ldr	r3, [r7, #88]	; 0x58
3000530e:	f10d 0810 	add.w	r8, sp, #16
30005312:	ebad 0d02 	sub.w	sp, sp, r2
30005316:	3301      	adds	r3, #1
30005318:	1c62      	adds	r2, r4, #1
3000531a:	4640      	mov	r0, r8
3000531c:	f10d 0910 	add.w	r9, sp, #16
30005320:	607b      	str	r3, [r7, #4]
30005322:	47a8      	blx	r5
30005324:	687b      	ldr	r3, [r7, #4]
30005326:	2100      	movs	r1, #0
30005328:	4648      	mov	r0, r9
3000532a:	461a      	mov	r2, r3
3000532c:	47a8      	blx	r5
3000532e:	2300      	movs	r3, #0
30005330:	1e61      	subs	r1, r4, #1
30005332:	4299      	cmp	r1, r3
30005334:	d82a      	bhi.n	3000538c <password_hash_check+0xac>
30005336:	5c73      	ldrb	r3, [r6, r1]
30005338:	6dba      	ldr	r2, [r7, #88]	; 0x58
3000533a:	f808 300a 	strb.w	r3, [r8, sl]
3000533e:	2300      	movs	r3, #0
30005340:	3a01      	subs	r2, #1
30005342:	4619      	mov	r1, r3
30005344:	429a      	cmp	r2, r3
30005346:	d832      	bhi.n	300053ae <password_hash_check+0xce>
30005348:	68fb      	ldr	r3, [r7, #12]
3000534a:	2620      	movs	r6, #32
3000534c:	4640      	mov	r0, r8
3000534e:	5c9b      	ldrb	r3, [r3, r2]
30005350:	464a      	mov	r2, r9
30005352:	f809 3001 	strb.w	r3, [r9, r1]
30005356:	f107 0310 	add.w	r3, r7, #16
3000535a:	4621      	mov	r1, r4
3000535c:	9601      	str	r6, [sp, #4]
3000535e:	9302      	str	r3, [sp, #8]
30005360:	68bb      	ldr	r3, [r7, #8]
30005362:	9300      	str	r3, [sp, #0]
30005364:	465b      	mov	r3, fp
30005366:	f7ff fedf 	bl	30005128 <BOOT_RAM_KeyDeriveFunc>
3000536a:	4632      	mov	r2, r6
3000536c:	491e      	ldr	r1, [pc, #120]	; (300053e8 <password_hash_check+0x108>)
3000536e:	f107 0010 	add.w	r0, r7, #16
30005372:	f006 f80d 	bl	3000b390 <____wrap_memcmp_veneer>
30005376:	4604      	mov	r4, r0
30005378:	b358      	cbz	r0, 300053d2 <password_hash_check+0xf2>
3000537a:	2401      	movs	r4, #1
3000537c:	481b      	ldr	r0, [pc, #108]	; (300053ec <password_hash_check+0x10c>)
3000537e:	f006 f823 	bl	3000b3c8 <__DiagPrintf_veneer>
30005382:	4620      	mov	r0, r4
30005384:	3734      	adds	r7, #52	; 0x34
30005386:	46bd      	mov	sp, r7
30005388:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000538c:	18f0      	adds	r0, r6, r3
3000538e:	5cf2      	ldrb	r2, [r6, r3]
30005390:	7840      	ldrb	r0, [r0, #1]
30005392:	2830      	cmp	r0, #48	; 0x30
30005394:	d105      	bne.n	300053a2 <password_hash_check+0xc2>
30005396:	2a5c      	cmp	r2, #92	; 0x5c
30005398:	bf02      	ittt	eq
3000539a:	3301      	addeq	r3, #1
3000539c:	f104 34ff 	addeq.w	r4, r4, #4294967295	; 0xffffffff
300053a0:	2200      	moveq	r2, #0
300053a2:	f808 200a 	strb.w	r2, [r8, sl]
300053a6:	3301      	adds	r3, #1
300053a8:	f10a 0a01 	add.w	sl, sl, #1
300053ac:	e7c1      	b.n	30005332 <password_hash_check+0x52>
300053ae:	68fe      	ldr	r6, [r7, #12]
300053b0:	68f8      	ldr	r0, [r7, #12]
300053b2:	441e      	add	r6, r3
300053b4:	5cc0      	ldrb	r0, [r0, r3]
300053b6:	7876      	ldrb	r6, [r6, #1]
300053b8:	2e30      	cmp	r6, #48	; 0x30
300053ba:	d105      	bne.n	300053c8 <password_hash_check+0xe8>
300053bc:	285c      	cmp	r0, #92	; 0x5c
300053be:	bf02      	ittt	eq
300053c0:	3301      	addeq	r3, #1
300053c2:	f10b 3bff 	addeq.w	fp, fp, #4294967295	; 0xffffffff
300053c6:	2000      	moveq	r0, #0
300053c8:	f809 0001 	strb.w	r0, [r9, r1]
300053cc:	3301      	adds	r3, #1
300053ce:	3101      	adds	r1, #1
300053d0:	e7b8      	b.n	30005344 <password_hash_check+0x64>
300053d2:	4807      	ldr	r0, [pc, #28]	; (300053f0 <password_hash_check+0x110>)
300053d4:	f005 fff8 	bl	3000b3c8 <__DiagPrintf_veneer>
300053d8:	4632      	mov	r2, r6
300053da:	4621      	mov	r1, r4
300053dc:	f107 0010 	add.w	r0, r7, #16
300053e0:	47a8      	blx	r5
300053e2:	e7ce      	b.n	30005382 <password_hash_check+0xa2>
300053e4:	00012be5 	.word	0x00012be5
300053e8:	3000db20 	.word	0x3000db20
300053ec:	3000cce3 	.word	0x3000cce3
300053f0:	3000ccfa 	.word	0x3000ccfa

300053f4 <BOOT_Share_Memory_Patch>:
300053f4:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300053f8:	6b1a      	ldr	r2, [r3, #48]	; 0x30
300053fa:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
300053fe:	631a      	str	r2, [r3, #48]	; 0x30
30005400:	f04f 5208 	mov.w	r2, #570425344	; 0x22000000
30005404:	6810      	ldr	r0, [r2, #0]
30005406:	f502 4240 	add.w	r2, r2, #49152	; 0xc000
3000540a:	6812      	ldr	r2, [r2, #0]
3000540c:	4410      	add	r0, r2
3000540e:	4a11      	ldr	r2, [pc, #68]	; (30005454 <BOOT_Share_Memory_Patch+0x60>)
30005410:	6812      	ldr	r2, [r2, #0]
30005412:	4410      	add	r0, r2
30005414:	4a10      	ldr	r2, [pc, #64]	; (30005458 <BOOT_Share_Memory_Patch+0x64>)
30005416:	6812      	ldr	r2, [r2, #0]
30005418:	4410      	add	r0, r2
3000541a:	4a10      	ldr	r2, [pc, #64]	; (3000545c <BOOT_Share_Memory_Patch+0x68>)
3000541c:	6812      	ldr	r2, [r2, #0]
3000541e:	4410      	add	r0, r2
30005420:	6b1a      	ldr	r2, [r3, #48]	; 0x30
30005422:	f422 22c0 	bic.w	r2, r2, #393216	; 0x60000
30005426:	631a      	str	r2, [r3, #48]	; 0x30
30005428:	f04f 5300 	mov.w	r3, #536870912	; 0x20000000
3000542c:	681b      	ldr	r3, [r3, #0]
3000542e:	4418      	add	r0, r3
30005430:	4b0b      	ldr	r3, [pc, #44]	; (30005460 <BOOT_Share_Memory_Patch+0x6c>)
30005432:	681b      	ldr	r3, [r3, #0]
30005434:	4418      	add	r0, r3
30005436:	4b0b      	ldr	r3, [pc, #44]	; (30005464 <BOOT_Share_Memory_Patch+0x70>)
30005438:	681b      	ldr	r3, [r3, #0]
3000543a:	4418      	add	r0, r3
3000543c:	4b0a      	ldr	r3, [pc, #40]	; (30005468 <BOOT_Share_Memory_Patch+0x74>)
3000543e:	681b      	ldr	r3, [r3, #0]
30005440:	4418      	add	r0, r3
30005442:	f04f 530c 	mov.w	r3, #587202560	; 0x23000000
30005446:	681b      	ldr	r3, [r3, #0]
30005448:	4418      	add	r0, r3
3000544a:	4b08      	ldr	r3, [pc, #32]	; (3000546c <BOOT_Share_Memory_Patch+0x78>)
3000544c:	681b      	ldr	r3, [r3, #0]
3000544e:	4418      	add	r0, r3
30005450:	4770      	bx	lr
30005452:	bf00      	nop
30005454:	22018000 	.word	0x22018000
30005458:	2202c000 	.word	0x2202c000
3000545c:	22100000 	.word	0x22100000
30005460:	20010000 	.word	0x20010000
30005464:	20020000 	.word	0x20020000
30005468:	20030000 	.word	0x20030000
3000546c:	23010000 	.word	0x23010000

30005470 <BOOT_Image1>:
30005470:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005474:	4bb3      	ldr	r3, [pc, #716]	; (30005744 <BOOT_Image1+0x2d4>)
30005476:	b08b      	sub	sp, #44	; 0x2c
30005478:	4798      	blx	r3
3000547a:	2800      	cmp	r0, #0
3000547c:	f040 81c9 	bne.w	30005812 <BOOT_Image1+0x3a2>
30005480:	48b1      	ldr	r0, [pc, #708]	; (30005748 <BOOT_Image1+0x2d8>)
30005482:	f005 ffa1 	bl	3000b3c8 <__DiagPrintf_veneer>
30005486:	49b1      	ldr	r1, [pc, #708]	; (3000574c <BOOT_Image1+0x2dc>)
30005488:	48b1      	ldr	r0, [pc, #708]	; (30005750 <BOOT_Image1+0x2e0>)
3000548a:	f005 ff9d 	bl	3000b3c8 <__DiagPrintf_veneer>
3000548e:	48b1      	ldr	r0, [pc, #708]	; (30005754 <BOOT_Image1+0x2e4>)
30005490:	4ab1      	ldr	r2, [pc, #708]	; (30005758 <BOOT_Image1+0x2e8>)
30005492:	2100      	movs	r1, #0
30005494:	4bb1      	ldr	r3, [pc, #708]	; (3000575c <BOOT_Image1+0x2ec>)
30005496:	1a12      	subs	r2, r2, r0
30005498:	4798      	blx	r3
3000549a:	f7ff fcb3 	bl	30004e04 <BOOT_ReasonSet>
3000549e:	f003 f9cf 	bl	30008840 <BOOT_Reason>
300054a2:	4601      	mov	r1, r0
300054a4:	b920      	cbnz	r0, 300054b0 <BOOT_Image1+0x40>
300054a6:	f44f 7201 	mov.w	r2, #516	; 0x204
300054aa:	48ad      	ldr	r0, [pc, #692]	; (30005760 <BOOT_Image1+0x2f0>)
300054ac:	f005 ff88 	bl	3000b3c0 <____wrap_memset_veneer>
300054b0:	4dac      	ldr	r5, [pc, #688]	; (30005764 <BOOT_Image1+0x2f4>)
300054b2:	4bad      	ldr	r3, [pc, #692]	; (30005768 <BOOT_Image1+0x2f8>)
300054b4:	4798      	blx	r3
300054b6:	782c      	ldrb	r4, [r5, #0]
300054b8:	4eac      	ldr	r6, [pc, #688]	; (3000576c <BOOT_Image1+0x2fc>)
300054ba:	2c00      	cmp	r4, #0
300054bc:	f000 81ab 	beq.w	30005816 <BOOT_Image1+0x3a6>
300054c0:	2000      	movs	r0, #0
300054c2:	4bab      	ldr	r3, [pc, #684]	; (30005770 <BOOT_Image1+0x300>)
300054c4:	4798      	blx	r3
300054c6:	2001      	movs	r0, #1
300054c8:	47b0      	blx	r6
300054ca:	4eaa      	ldr	r6, [pc, #680]	; (30005774 <BOOT_Image1+0x304>)
300054cc:	f7ff fce2 	bl	30004e94 <BOOT_SOC_ClkSet>
300054d0:	f7fe fed2 	bl	30004278 <BOOT_GRstConfig>
300054d4:	4fa8      	ldr	r7, [pc, #672]	; (30005778 <BOOT_Image1+0x308>)
300054d6:	47b0      	blx	r6
300054d8:	2802      	cmp	r0, #2
300054da:	4aa1      	ldr	r2, [pc, #644]	; (30005760 <BOOT_Image1+0x2f0>)
300054dc:	f040 81a4 	bne.w	30005828 <BOOT_Image1+0x3b8>
300054e0:	4ca6      	ldr	r4, [pc, #664]	; (3000577c <BOOT_Image1+0x30c>)
300054e2:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
300054e6:	f8d4 325c 	ldr.w	r3, [r4, #604]	; 0x25c
300054ea:	f423 7398 	bic.w	r3, r3, #304	; 0x130
300054ee:	f8c4 325c 	str.w	r3, [r4, #604]	; 0x25c
300054f2:	f882 010e 	strb.w	r0, [r2, #270]	; 0x10e
300054f6:	2201      	movs	r2, #1
300054f8:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
300054fc:	47b8      	blx	r7
300054fe:	f8d4 32fc 	ldr.w	r3, [r4, #764]	; 0x2fc
30005502:	f043 0302 	orr.w	r3, r3, #2
30005506:	f8c4 32fc 	str.w	r3, [r4, #764]	; 0x2fc
3000550a:	4b9d      	ldr	r3, [pc, #628]	; (30005780 <BOOT_Image1+0x310>)
3000550c:	4798      	blx	r3
3000550e:	4a9b      	ldr	r2, [pc, #620]	; (3000577c <BOOT_Image1+0x30c>)
30005510:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30005514:	f023 0303 	bic.w	r3, r3, #3
30005518:	2800      	cmp	r0, #0
3000551a:	f040 819a 	bne.w	30005852 <BOOT_Image1+0x3e2>
3000551e:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30005522:	4b98      	ldr	r3, [pc, #608]	; (30005784 <BOOT_Image1+0x314>)
30005524:	4c98      	ldr	r4, [pc, #608]	; (30005788 <BOOT_Image1+0x318>)
30005526:	781b      	ldrb	r3, [r3, #0]
30005528:	b1b3      	cbz	r3, 30005558 <BOOT_Image1+0xe8>
3000552a:	f8df 82cc 	ldr.w	r8, [pc, #716]	; 300057f8 <BOOT_Image1+0x388>
3000552e:	47c0      	blx	r8
30005530:	4607      	mov	r7, r0
30005532:	47c0      	blx	r8
30005534:	2f02      	cmp	r7, #2
30005536:	d00f      	beq.n	30005558 <BOOT_Image1+0xe8>
30005538:	2803      	cmp	r0, #3
3000553a:	d00d      	beq.n	30005558 <BOOT_Image1+0xe8>
3000553c:	4b93      	ldr	r3, [pc, #588]	; (3000578c <BOOT_Image1+0x31c>)
3000553e:	4798      	blx	r3
30005540:	2200      	movs	r2, #0
30005542:	2101      	movs	r1, #1
30005544:	4892      	ldr	r0, [pc, #584]	; (30005790 <BOOT_Image1+0x320>)
30005546:	47a0      	blx	r4
30005548:	2101      	movs	r1, #1
3000554a:	4891      	ldr	r0, [pc, #580]	; (30005790 <BOOT_Image1+0x320>)
3000554c:	4b91      	ldr	r3, [pc, #580]	; (30005794 <BOOT_Image1+0x324>)
3000554e:	4798      	blx	r3
30005550:	2201      	movs	r2, #1
30005552:	488f      	ldr	r0, [pc, #572]	; (30005790 <BOOT_Image1+0x320>)
30005554:	4611      	mov	r1, r2
30005556:	47a0      	blx	r4
30005558:	2201      	movs	r2, #1
3000555a:	2102      	movs	r1, #2
3000555c:	488c      	ldr	r0, [pc, #560]	; (30005790 <BOOT_Image1+0x320>)
3000555e:	47a0      	blx	r4
30005560:	4c8d      	ldr	r4, [pc, #564]	; (30005798 <BOOT_Image1+0x328>)
30005562:	68a3      	ldr	r3, [r4, #8]
30005564:	079f      	lsls	r7, r3, #30
30005566:	d504      	bpl.n	30005572 <BOOT_Image1+0x102>
30005568:	f3ef 8108 	mrs	r1, MSP
3000556c:	488b      	ldr	r0, [pc, #556]	; (3000579c <BOOT_Image1+0x32c>)
3000556e:	f005 ff2b 	bl	3000b3c8 <__DiagPrintf_veneer>
30005572:	498b      	ldr	r1, [pc, #556]	; (300057a0 <BOOT_Image1+0x330>)
30005574:	e841 f100 	tt	r1, r1
30005578:	68a3      	ldr	r3, [r4, #8]
3000557a:	f3c1 5180 	ubfx	r1, r1, #22, #1
3000557e:	0798      	lsls	r0, r3, #30
30005580:	d502      	bpl.n	30005588 <BOOT_Image1+0x118>
30005582:	4888      	ldr	r0, [pc, #544]	; (300057a4 <BOOT_Image1+0x334>)
30005584:	f005 ff20 	bl	3000b3c8 <__DiagPrintf_veneer>
30005588:	4b87      	ldr	r3, [pc, #540]	; (300057a8 <BOOT_Image1+0x338>)
3000558a:	681b      	ldr	r3, [r3, #0]
3000558c:	4798      	blx	r3
3000558e:	f7fe fc7b 	bl	30003e88 <BOOT_RccConfig>
30005592:	f7fe fce3 	bl	30003f5c <BOOT_CACHEWRR_Set>
30005596:	4b85      	ldr	r3, [pc, #532]	; (300057ac <BOOT_Image1+0x33c>)
30005598:	6818      	ldr	r0, [r3, #0]
3000559a:	f7fe fd0d 	bl	30003fb8 <BOOT_TCMSet>
3000559e:	4884      	ldr	r0, [pc, #528]	; (300057b0 <BOOT_Image1+0x340>)
300055a0:	4984      	ldr	r1, [pc, #528]	; (300057b4 <BOOT_Image1+0x344>)
300055a2:	6943      	ldr	r3, [r0, #20]
300055a4:	03da      	lsls	r2, r3, #15
300055a6:	d514      	bpl.n	300055d2 <BOOT_Image1+0x162>
300055a8:	f011 021f 	ands.w	r2, r1, #31
300055ac:	bf19      	ittee	ne
300055ae:	f021 031f 	bicne.w	r3, r1, #31
300055b2:	3274      	addne	r2, #116	; 0x74
300055b4:	460b      	moveq	r3, r1
300055b6:	2274      	moveq	r2, #116	; 0x74
300055b8:	f3bf 8f4f 	dsb	sy
300055bc:	441a      	add	r2, r3
300055be:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
300055c2:	3320      	adds	r3, #32
300055c4:	1ad7      	subs	r7, r2, r3
300055c6:	2f00      	cmp	r7, #0
300055c8:	dcf9      	bgt.n	300055be <BOOT_Image1+0x14e>
300055ca:	f3bf 8f4f 	dsb	sy
300055ce:	f3bf 8f6f 	isb	sy
300055d2:	4b6a      	ldr	r3, [pc, #424]	; (3000577c <BOOT_Image1+0x30c>)
300055d4:	2200      	movs	r2, #0
300055d6:	f8c3 1270 	str.w	r1, [r3, #624]	; 0x270
300055da:	4b61      	ldr	r3, [pc, #388]	; (30005760 <BOOT_Image1+0x2f0>)
300055dc:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300055e0:	f7fe f826 	bl	30003630 <flash_highspeed_setup>
300055e4:	47b0      	blx	r6
300055e6:	2802      	cmp	r0, #2
300055e8:	4e73      	ldr	r6, [pc, #460]	; (300057b8 <BOOT_Image1+0x348>)
300055ea:	f040 8135 	bne.w	30005858 <BOOT_Image1+0x3e8>
300055ee:	4b73      	ldr	r3, [pc, #460]	; (300057bc <BOOT_Image1+0x34c>)
300055f0:	2004      	movs	r0, #4
300055f2:	4a73      	ldr	r2, [pc, #460]	; (300057c0 <BOOT_Image1+0x350>)
300055f4:	4619      	mov	r1, r3
300055f6:	f005 fc91 	bl	3000af1c <rtk_log_write>
300055fa:	4b72      	ldr	r3, [pc, #456]	; (300057c4 <BOOT_Image1+0x354>)
300055fc:	4798      	blx	r3
300055fe:	f7fe fe9d 	bl	3000433c <BOOT_PSRAM_Init>
30005602:	7828      	ldrb	r0, [r5, #0]
30005604:	b940      	cbnz	r0, 30005618 <BOOT_Image1+0x1a8>
30005606:	4b70      	ldr	r3, [pc, #448]	; (300057c8 <BOOT_Image1+0x358>)
30005608:	4798      	blx	r3
3000560a:	b128      	cbz	r0, 30005618 <BOOT_Image1+0x1a8>
3000560c:	4b6b      	ldr	r3, [pc, #428]	; (300057bc <BOOT_Image1+0x34c>)
3000560e:	2003      	movs	r0, #3
30005610:	4a6e      	ldr	r2, [pc, #440]	; (300057cc <BOOT_Image1+0x35c>)
30005612:	4619      	mov	r1, r3
30005614:	f005 fc82 	bl	3000af1c <rtk_log_write>
30005618:	4b58      	ldr	r3, [pc, #352]	; (3000577c <BOOT_Image1+0x30c>)
3000561a:	2500      	movs	r5, #0
3000561c:	f10d 011d 	add.w	r1, sp, #29
30005620:	f8c3 52d0 	str.w	r5, [r3, #720]	; 0x2d0
30005624:	f8c3 52d4 	str.w	r5, [r3, #724]	; 0x2d4
30005628:	f8c3 52d8 	str.w	r5, [r3, #728]	; 0x2d8
3000562c:	f8c3 50d0 	str.w	r5, [r3, #208]	; 0xd0
30005630:	f7ff fee0 	bl	300053f4 <BOOT_Share_Memory_Patch>
30005634:	f44f 7059 	mov.w	r0, #868	; 0x364
30005638:	f002 fbc8 	bl	30007dcc <OTP_Read8>
3000563c:	f10d 011f 	add.w	r1, sp, #31
30005640:	f44f 705a 	mov.w	r0, #872	; 0x368
30005644:	f88d 501e 	strb.w	r5, [sp, #30]
30005648:	f88d 501f 	strb.w	r5, [sp, #31]
3000564c:	f002 fbbe 	bl	30007dcc <OTP_Read8>
30005650:	f10d 011e 	add.w	r1, sp, #30
30005654:	f240 3069 	movw	r0, #873	; 0x369
30005658:	f002 fbb8 	bl	30007dcc <OTP_Read8>
3000565c:	f89d 301d 	ldrb.w	r3, [sp, #29]
30005660:	f013 0f1e 	tst.w	r3, #30
30005664:	d125      	bne.n	300056b2 <BOOT_Image1+0x242>
30005666:	f89d 201e 	ldrb.w	r2, [sp, #30]
3000566a:	f89d 301f 	ldrb.w	r3, [sp, #31]
3000566e:	f002 0208 	and.w	r2, r2, #8
30005672:	f003 0301 	and.w	r3, r3, #1
30005676:	4313      	orrs	r3, r2
30005678:	d01b      	beq.n	300056b2 <BOOT_Image1+0x242>
3000567a:	4855      	ldr	r0, [pc, #340]	; (300057d0 <BOOT_Image1+0x360>)
3000567c:	f005 fea4 	bl	3000b3c8 <__DiagPrintf_veneer>
30005680:	f89d 101f 	ldrb.w	r1, [sp, #31]
30005684:	f89d 301e 	ldrb.w	r3, [sp, #30]
30005688:	f44f 705a 	mov.w	r0, #872	; 0x368
3000568c:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
30005690:	f023 0308 	bic.w	r3, r3, #8
30005694:	f88d 101f 	strb.w	r1, [sp, #31]
30005698:	f88d 301e 	strb.w	r3, [sp, #30]
3000569c:	f002 fbfe 	bl	30007e9c <OTP_Write8>
300056a0:	f89d 101e 	ldrb.w	r1, [sp, #30]
300056a4:	f240 3069 	movw	r0, #873	; 0x369
300056a8:	f002 fbf8 	bl	30007e9c <OTP_Write8>
300056ac:	4849      	ldr	r0, [pc, #292]	; (300057d4 <BOOT_Image1+0x364>)
300056ae:	f005 fe8b 	bl	3000b3c8 <__DiagPrintf_veneer>
300056b2:	4b49      	ldr	r3, [pc, #292]	; (300057d8 <BOOT_Image1+0x368>)
300056b4:	781b      	ldrb	r3, [r3, #0]
300056b6:	b133      	cbz	r3, 300056c6 <BOOT_Image1+0x256>
300056b8:	4b48      	ldr	r3, [pc, #288]	; (300057dc <BOOT_Image1+0x36c>)
300056ba:	4798      	blx	r3
300056bc:	b918      	cbnz	r0, 300056c6 <BOOT_Image1+0x256>
300056be:	68a3      	ldr	r3, [r4, #8]
300056c0:	f043 0302 	orr.w	r3, r3, #2
300056c4:	60a3      	str	r3, [r4, #8]
300056c6:	6823      	ldr	r3, [r4, #0]
300056c8:	f043 0302 	orr.w	r3, r3, #2
300056cc:	6023      	str	r3, [r4, #0]
300056ce:	4c44      	ldr	r4, [pc, #272]	; (300057e0 <BOOT_Image1+0x370>)
300056d0:	47a0      	blx	r4
300056d2:	2801      	cmp	r0, #1
300056d4:	f04f 0100 	mov.w	r1, #0
300056d8:	f04f 0002 	mov.w	r0, #2
300056dc:	bf0c      	ite	eq
300056de:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
300056e2:	4a40      	ldrne	r2, [pc, #256]	; (300057e4 <BOOT_Image1+0x374>)
300056e4:	f001 fcaa 	bl	3000703c <ymodem_uart_port_init>
300056e8:	47a0      	blx	r4
300056ea:	2800      	cmp	r0, #0
300056ec:	f000 80cd 	beq.w	3000588a <BOOT_Image1+0x41a>
300056f0:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
300056f4:	483c      	ldr	r0, [pc, #240]	; (300057e8 <BOOT_Image1+0x378>)
300056f6:	2464      	movs	r4, #100	; 0x64
300056f8:	f005 fe66 	bl	3000b3c8 <__DiagPrintf_veneer>
300056fc:	483b      	ldr	r0, [pc, #236]	; (300057ec <BOOT_Image1+0x37c>)
300056fe:	f005 fe43 	bl	3000b388 <____wrap_strlen_veneer>
30005702:	4601      	mov	r1, r0
30005704:	4839      	ldr	r0, [pc, #228]	; (300057ec <BOOT_Image1+0x37c>)
30005706:	f001 fcdf 	bl	300070c8 <ymodem_uart_putdata>
3000570a:	2001      	movs	r0, #1
3000570c:	47b0      	blx	r6
3000570e:	f001 fcc1 	bl	30007094 <ymodem_uart_readable>
30005712:	2800      	cmp	r0, #0
30005714:	f040 80bb 	bne.w	3000588e <BOOT_Image1+0x41e>
30005718:	3c01      	subs	r4, #1
3000571a:	d1f6      	bne.n	3000570a <BOOT_Image1+0x29a>
3000571c:	9404      	str	r4, [sp, #16]
3000571e:	f001 fcb9 	bl	30007094 <ymodem_uart_readable>
30005722:	4604      	mov	r4, r0
30005724:	2800      	cmp	r0, #0
30005726:	f000 822f 	beq.w	30005b88 <BOOT_Image1+0x718>
3000572a:	2001      	movs	r0, #1
3000572c:	4c30      	ldr	r4, [pc, #192]	; (300057f0 <BOOT_Image1+0x380>)
3000572e:	47b0      	blx	r6
30005730:	2108      	movs	r1, #8
30005732:	4830      	ldr	r0, [pc, #192]	; (300057f4 <BOOT_Image1+0x384>)
30005734:	f001 fcd6 	bl	300070e4 <ymodem_uart_getdata>
30005738:	492e      	ldr	r1, [pc, #184]	; (300057f4 <BOOT_Image1+0x384>)
3000573a:	2208      	movs	r2, #8
3000573c:	f101 0080 	add.w	r0, r1, #128	; 0x80
30005740:	47a0      	blx	r4
30005742:	e05b      	b.n	300057fc <BOOT_Image1+0x38c>
30005744:	0000c149 	.word	0x0000c149
30005748:	3000cd14 	.word	0x3000cd14
3000574c:	3000d254 	.word	0x3000d254
30005750:	3000cd38 	.word	0x3000cd38
30005754:	3000ead0 	.word	0x3000ead0
30005758:	3000ef74 	.word	0x3000ef74
3000575c:	00012be5 	.word	0x00012be5
30005760:	23020000 	.word	0x23020000
30005764:	3000d552 	.word	0x3000d552
30005768:	3000a4f1 	.word	0x3000a4f1
3000576c:	3000a719 	.word	0x3000a719
30005770:	3000a7f1 	.word	0x3000a7f1
30005774:	30007291 	.word	0x30007291
30005778:	0000b479 	.word	0x0000b479
3000577c:	42008000 	.word	0x42008000
30005780:	3000903d 	.word	0x3000903d
30005784:	3000d551 	.word	0x3000d551
30005788:	0000aab9 	.word	0x0000aab9
3000578c:	0000aea1 	.word	0x0000aea1
30005790:	4200c000 	.word	0x4200c000
30005794:	0000aaa5 	.word	0x0000aaa5
30005798:	2001c00c 	.word	0x2001c00c
3000579c:	3000cd4f 	.word	0x3000cd4f
300057a0:	30005471 	.word	0x30005471
300057a4:	3000cd7f 	.word	0x3000cd7f
300057a8:	00033000 	.word	0x00033000
300057ac:	3000d554 	.word	0x3000d554
300057b0:	e000ed00 	.word	0xe000ed00
300057b4:	2001c01c 	.word	0x2001c01c
300057b8:	00009be5 	.word	0x00009be5
300057bc:	3000c714 	.word	0x3000c714
300057c0:	3000cdaf 	.word	0x3000cdaf
300057c4:	300080f5 	.word	0x300080f5
300057c8:	3000a81d 	.word	0x3000a81d
300057cc:	3000cdc3 	.word	0x3000cdc3
300057d0:	3000ce3b 	.word	0x3000ce3b
300057d4:	3000ce71 	.word	0x3000ce71
300057d8:	3000d550 	.word	0x3000d550
300057dc:	0000c179 	.word	0x0000c179
300057e0:	0000c169 	.word	0x0000c169
300057e4:	0016e360 	.word	0x0016e360
300057e8:	3000ce92 	.word	0x3000ce92
300057ec:	3000db40 	.word	0x3000db40
300057f0:	000129bd 	.word	0x000129bd
300057f4:	3000db5d 	.word	0x3000db5d
300057f8:	0000c0f9 	.word	0x0000c0f9
300057fc:	9403      	str	r4, [sp, #12]
300057fe:	4681      	mov	r9, r0
30005800:	2800      	cmp	r0, #0
30005802:	d047      	beq.n	30005894 <BOOT_Image1+0x424>
30005804:	486d      	ldr	r0, [pc, #436]	; (300059bc <BOOT_Image1+0x54c>)
30005806:	f005 fddf 	bl	3000b3c8 <__DiagPrintf_veneer>
3000580a:	486d      	ldr	r0, [pc, #436]	; (300059c0 <BOOT_Image1+0x550>)
3000580c:	f005 fddc 	bl	3000b3c8 <__DiagPrintf_veneer>
30005810:	e09d      	b.n	3000594e <BOOT_Image1+0x4de>
30005812:	486c      	ldr	r0, [pc, #432]	; (300059c4 <BOOT_Image1+0x554>)
30005814:	e635      	b.n	30005482 <BOOT_Image1+0x12>
30005816:	2001      	movs	r0, #1
30005818:	47b0      	blx	r6
3000581a:	4620      	mov	r0, r4
3000581c:	4b6a      	ldr	r3, [pc, #424]	; (300059c8 <BOOT_Image1+0x558>)
3000581e:	4798      	blx	r3
30005820:	4b6a      	ldr	r3, [pc, #424]	; (300059cc <BOOT_Image1+0x55c>)
30005822:	2001      	movs	r0, #1
30005824:	4798      	blx	r3
30005826:	e650      	b.n	300054ca <BOOT_Image1+0x5a>
30005828:	2401      	movs	r4, #1
3000582a:	4969      	ldr	r1, [pc, #420]	; (300059d0 <BOOT_Image1+0x560>)
3000582c:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005830:	f882 410e 	strb.w	r4, [r2, #270]	; 0x10e
30005834:	4622      	mov	r2, r4
30005836:	47b8      	blx	r7
30005838:	4622      	mov	r2, r4
3000583a:	4966      	ldr	r1, [pc, #408]	; (300059d4 <BOOT_Image1+0x564>)
3000583c:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005840:	47b8      	blx	r7
30005842:	4a65      	ldr	r2, [pc, #404]	; (300059d8 <BOOT_Image1+0x568>)
30005844:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005848:	f043 0303 	orr.w	r3, r3, #3
3000584c:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005850:	e65b      	b.n	3000550a <BOOT_Image1+0x9a>
30005852:	f043 0302 	orr.w	r3, r3, #2
30005856:	e662      	b.n	3000551e <BOOT_Image1+0xae>
30005858:	782b      	ldrb	r3, [r5, #0]
3000585a:	b15b      	cbz	r3, 30005874 <BOOT_Image1+0x404>
3000585c:	4d5f      	ldr	r5, [pc, #380]	; (300059dc <BOOT_Image1+0x56c>)
3000585e:	4c60      	ldr	r4, [pc, #384]	; (300059e0 <BOOT_Image1+0x570>)
30005860:	462b      	mov	r3, r5
30005862:	4622      	mov	r2, r4
30005864:	495d      	ldr	r1, [pc, #372]	; (300059dc <BOOT_Image1+0x56c>)
30005866:	2002      	movs	r0, #2
30005868:	f005 fb58 	bl	3000af1c <rtk_log_write>
3000586c:	f241 3088 	movw	r0, #5000	; 0x1388
30005870:	47b0      	blx	r6
30005872:	e7f5      	b.n	30005860 <BOOT_Image1+0x3f0>
30005874:	68a3      	ldr	r3, [r4, #8]
30005876:	079b      	lsls	r3, r3, #30
30005878:	d502      	bpl.n	30005880 <BOOT_Image1+0x410>
3000587a:	485a      	ldr	r0, [pc, #360]	; (300059e4 <BOOT_Image1+0x574>)
3000587c:	f005 fda4 	bl	3000b3c8 <__DiagPrintf_veneer>
30005880:	f7fe fd1e 	bl	300042c0 <BOOT_DDR_Init>
30005884:	f7fe fd10 	bl	300042a8 <BOOT_DDR_LCDC_HPR>
30005888:	e6c6      	b.n	30005618 <BOOT_Image1+0x1a8>
3000588a:	4957      	ldr	r1, [pc, #348]	; (300059e8 <BOOT_Image1+0x578>)
3000588c:	e732      	b.n	300056f4 <BOOT_Image1+0x284>
3000588e:	2301      	movs	r3, #1
30005890:	9304      	str	r3, [sp, #16]
30005892:	e744      	b.n	3000571e <BOOT_Image1+0x2ae>
30005894:	f001 fbfe 	bl	30007094 <ymodem_uart_readable>
30005898:	2800      	cmp	r0, #0
3000589a:	d0fb      	beq.n	30005894 <BOOT_Image1+0x424>
3000589c:	2101      	movs	r1, #1
3000589e:	4853      	ldr	r0, [pc, #332]	; (300059ec <BOOT_Image1+0x57c>)
300058a0:	f001 fc20 	bl	300070e4 <ymodem_uart_getdata>
300058a4:	f001 fbf6 	bl	30007094 <ymodem_uart_readable>
300058a8:	2800      	cmp	r0, #0
300058aa:	d0fb      	beq.n	300058a4 <BOOT_Image1+0x434>
300058ac:	4d50      	ldr	r5, [pc, #320]	; (300059f0 <BOOT_Image1+0x580>)
300058ae:	2400      	movs	r4, #0
300058b0:	2101      	movs	r1, #1
300058b2:	484f      	ldr	r0, [pc, #316]	; (300059f0 <BOOT_Image1+0x580>)
300058b4:	f2a5 484e 	subw	r8, r5, #1102	; 0x44e
300058b8:	f001 fc14 	bl	300070e4 <ymodem_uart_getdata>
300058bc:	f815 3b01 	ldrb.w	r3, [r5], #1
300058c0:	f898 24ce 	ldrb.w	r2, [r8, #1230]	; 0x4ce
300058c4:	429a      	cmp	r2, r3
300058c6:	d147      	bne.n	30005958 <BOOT_Image1+0x4e8>
300058c8:	4621      	mov	r1, r4
300058ca:	f208 404e 	addw	r0, r8, #1102	; 0x44e
300058ce:	f7fe f856 	bl	3000397e <ascii_to_integer>
300058d2:	f8c8 04d0 	str.w	r0, [r8, #1232]	; 0x4d0
300058d6:	f001 fbdd 	bl	30007094 <ymodem_uart_readable>
300058da:	2800      	cmp	r0, #0
300058dc:	d0fb      	beq.n	300058d6 <BOOT_Image1+0x466>
300058de:	4d45      	ldr	r5, [pc, #276]	; (300059f4 <BOOT_Image1+0x584>)
300058e0:	f104 070a 	add.w	r7, r4, #10
300058e4:	f04f 0a00 	mov.w	sl, #0
300058e8:	2101      	movs	r1, #1
300058ea:	442f      	add	r7, r5
300058ec:	4638      	mov	r0, r7
300058ee:	f001 fbf9 	bl	300070e4 <ymodem_uart_getdata>
300058f2:	eb0a 0304 	add.w	r3, sl, r4
300058f6:	f898 24ce 	ldrb.w	r2, [r8, #1230]	; 0x4ce
300058fa:	9305      	str	r3, [sp, #20]
300058fc:	f817 3b01 	ldrb.w	r3, [r7], #1
30005900:	429a      	cmp	r2, r3
30005902:	d133      	bne.n	3000596c <BOOT_Image1+0x4fc>
30005904:	f001 fbc6 	bl	30007094 <ymodem_uart_readable>
30005908:	2800      	cmp	r0, #0
3000590a:	d0fb      	beq.n	30005904 <BOOT_Image1+0x494>
3000590c:	9b05      	ldr	r3, [sp, #20]
3000590e:	330b      	adds	r3, #11
30005910:	eb05 0b03 	add.w	fp, r5, r3
30005914:	2101      	movs	r1, #1
30005916:	4658      	mov	r0, fp
30005918:	f001 fbe4 	bl	300070e4 <ymodem_uart_getdata>
3000591c:	f81b 2b01 	ldrb.w	r2, [fp], #1
30005920:	f898 14ce 	ldrb.w	r1, [r8, #1230]	; 0x4ce
30005924:	9b05      	ldr	r3, [sp, #20]
30005926:	4291      	cmp	r1, r2
30005928:	eb09 0703 	add.w	r7, r9, r3
3000592c:	d129      	bne.n	30005982 <BOOT_Image1+0x512>
3000592e:	f107 0b0c 	add.w	fp, r7, #12
30005932:	2101      	movs	r1, #1
30005934:	eb05 000b 	add.w	r0, r5, fp
30005938:	f001 fbd4 	bl	300070e4 <ymodem_uart_getdata>
3000593c:	f815 100b 	ldrb.w	r1, [r5, fp]
30005940:	290a      	cmp	r1, #10
30005942:	f8c8 14d4 	str.w	r1, [r8, #1236]	; 0x4d4
30005946:	d923      	bls.n	30005990 <BOOT_Image1+0x520>
30005948:	482b      	ldr	r0, [pc, #172]	; (300059f8 <BOOT_Image1+0x588>)
3000594a:	f005 fd3d 	bl	3000b3c8 <__DiagPrintf_veneer>
3000594e:	9b04      	ldr	r3, [sp, #16]
30005950:	2b00      	cmp	r3, #0
30005952:	f040 80bf 	bne.w	30005ad4 <BOOT_Image1+0x664>
30005956:	e0b7      	b.n	30005ac8 <BOOT_Image1+0x658>
30005958:	3401      	adds	r4, #1
3000595a:	f001 fb9b 	bl	30007094 <ymodem_uart_readable>
3000595e:	2800      	cmp	r0, #0
30005960:	d0fb      	beq.n	3000595a <BOOT_Image1+0x4ea>
30005962:	2101      	movs	r1, #1
30005964:	4628      	mov	r0, r5
30005966:	f001 fbbd 	bl	300070e4 <ymodem_uart_getdata>
3000596a:	e7a7      	b.n	300058bc <BOOT_Image1+0x44c>
3000596c:	f10a 0a01 	add.w	sl, sl, #1
30005970:	f001 fb90 	bl	30007094 <ymodem_uart_readable>
30005974:	2800      	cmp	r0, #0
30005976:	d0fb      	beq.n	30005970 <BOOT_Image1+0x500>
30005978:	2101      	movs	r1, #1
3000597a:	4638      	mov	r0, r7
3000597c:	f001 fbb2 	bl	300070e4 <ymodem_uart_getdata>
30005980:	e7b7      	b.n	300058f2 <BOOT_Image1+0x482>
30005982:	f109 0901 	add.w	r9, r9, #1
30005986:	f001 fb85 	bl	30007094 <ymodem_uart_readable>
3000598a:	2800      	cmp	r0, #0
3000598c:	d0fb      	beq.n	30005986 <BOOT_Image1+0x516>
3000598e:	e7c1      	b.n	30005914 <BOOT_Image1+0x4a4>
30005990:	f107 0b0e 	add.w	fp, r7, #14
30005994:	f107 000d 	add.w	r0, r7, #13
30005998:	2101      	movs	r1, #1
3000599a:	44ab      	add	fp, r5
3000599c:	4428      	add	r0, r5
3000599e:	f001 fba1 	bl	300070e4 <ymodem_uart_getdata>
300059a2:	2102      	movs	r1, #2
300059a4:	4658      	mov	r0, fp
300059a6:	f001 fb9d 	bl	300070e4 <ymodem_uart_getdata>
300059aa:	2202      	movs	r2, #2
300059ac:	4659      	mov	r1, fp
300059ae:	4813      	ldr	r0, [pc, #76]	; (300059fc <BOOT_Image1+0x58c>)
300059b0:	9b03      	ldr	r3, [sp, #12]
300059b2:	4798      	blx	r3
300059b4:	b330      	cbz	r0, 30005a04 <BOOT_Image1+0x594>
300059b6:	4812      	ldr	r0, [pc, #72]	; (30005a00 <BOOT_Image1+0x590>)
300059b8:	e728      	b.n	3000580c <BOOT_Image1+0x39c>
300059ba:	bf00      	nop
300059bc:	3000cea8 	.word	0x3000cea8
300059c0:	3000cec0 	.word	0x3000cec0
300059c4:	3000cd24 	.word	0x3000cd24
300059c8:	3000a93d 	.word	0x3000a93d
300059cc:	3000a8b1 	.word	0x3000a8b1
300059d0:	40080000 	.word	0x40080000
300059d4:	40040000 	.word	0x40040000
300059d8:	42008000 	.word	0x42008000
300059dc:	3000c714 	.word	0x3000c714
300059e0:	3000cdd9 	.word	0x3000cdd9
300059e4:	3000ce17 	.word	0x3000ce17
300059e8:	0016e360 	.word	0x0016e360
300059ec:	3000db65 	.word	0x3000db65
300059f0:	3000db66 	.word	0x3000db66
300059f4:	3000db5d 	.word	0x3000db5d
300059f8:	3000cece 	.word	0x3000cece
300059fc:	3000dbf0 	.word	0x3000dbf0
30005a00:	3000cedf 	.word	0x3000cedf
30005a04:	f107 0010 	add.w	r0, r7, #16
30005a08:	2101      	movs	r1, #1
30005a0a:	f107 0b11 	add.w	fp, r7, #17
30005a0e:	4428      	add	r0, r5
30005a10:	f001 fb68 	bl	300070e4 <ymodem_uart_getdata>
30005a14:	4b8e      	ldr	r3, [pc, #568]	; (30005c50 <BOOT_Image1+0x7e0>)
30005a16:	44ab      	add	fp, r5
30005a18:	f8d8 04d4 	ldr.w	r0, [r8, #1236]	; 0x4d4
30005a1c:	f103 0228 	add.w	r2, r3, #40	; 0x28
30005a20:	f103 0130 	add.w	r1, r3, #48	; 0x30
30005a24:	f7fd ffcf 	bl	300039c6 <hotfix_get_addr_size>
30005a28:	2102      	movs	r1, #2
30005a2a:	4658      	mov	r0, fp
30005a2c:	f001 fb5a 	bl	300070e4 <ymodem_uart_getdata>
30005a30:	2202      	movs	r2, #2
30005a32:	4659      	mov	r1, fp
30005a34:	4887      	ldr	r0, [pc, #540]	; (30005c54 <BOOT_Image1+0x7e4>)
30005a36:	9b03      	ldr	r3, [sp, #12]
30005a38:	4798      	blx	r3
30005a3a:	2800      	cmp	r0, #0
30005a3c:	d187      	bne.n	3000594e <BOOT_Image1+0x4de>
30005a3e:	f107 0013 	add.w	r0, r7, #19
30005a42:	2101      	movs	r1, #1
30005a44:	f107 0b14 	add.w	fp, r7, #20
30005a48:	4428      	add	r0, r5
30005a4a:	f001 fb4b 	bl	300070e4 <ymodem_uart_getdata>
30005a4e:	4b82      	ldr	r3, [pc, #520]	; (30005c58 <BOOT_Image1+0x7e8>)
30005a50:	44ab      	add	fp, r5
30005a52:	f8d8 04d4 	ldr.w	r0, [r8, #1236]	; 0x4d4
30005a56:	f1a3 0218 	sub.w	r2, r3, #24
30005a5a:	f1a3 0110 	sub.w	r1, r3, #16
30005a5e:	f7fd ffb2 	bl	300039c6 <hotfix_get_addr_size>
30005a62:	2102      	movs	r1, #2
30005a64:	4658      	mov	r0, fp
30005a66:	f001 fb3d 	bl	300070e4 <ymodem_uart_getdata>
30005a6a:	2202      	movs	r2, #2
30005a6c:	4659      	mov	r1, fp
30005a6e:	487b      	ldr	r0, [pc, #492]	; (30005c5c <BOOT_Image1+0x7ec>)
30005a70:	9b03      	ldr	r3, [sp, #12]
30005a72:	4798      	blx	r3
30005a74:	2800      	cmp	r0, #0
30005a76:	f47f af6a 	bne.w	3000594e <BOOT_Image1+0x4de>
30005a7a:	f107 0016 	add.w	r0, r7, #22
30005a7e:	2101      	movs	r1, #1
30005a80:	4428      	add	r0, r5
30005a82:	f001 fb2f 	bl	300070e4 <ymodem_uart_getdata>
30005a86:	f8d8 04d4 	ldr.w	r0, [r8, #1236]	; 0x4d4
30005a8a:	f107 0817 	add.w	r8, r7, #23
30005a8e:	4b74      	ldr	r3, [pc, #464]	; (30005c60 <BOOT_Image1+0x7f0>)
30005a90:	44a8      	add	r8, r5
30005a92:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
30005a96:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
30005a9a:	f7fd ff94 	bl	300039c6 <hotfix_get_addr_size>
30005a9e:	2102      	movs	r1, #2
30005aa0:	4640      	mov	r0, r8
30005aa2:	f001 fb1f 	bl	300070e4 <ymodem_uart_getdata>
30005aa6:	2202      	movs	r2, #2
30005aa8:	4641      	mov	r1, r8
30005aaa:	486e      	ldr	r0, [pc, #440]	; (30005c64 <BOOT_Image1+0x7f4>)
30005aac:	9b03      	ldr	r3, [sp, #12]
30005aae:	4798      	blx	r3
30005ab0:	2800      	cmp	r0, #0
30005ab2:	f47f af4c 	bne.w	3000594e <BOOT_Image1+0x4de>
30005ab6:	f107 0019 	add.w	r0, r7, #25
30005aba:	2101      	movs	r1, #1
30005abc:	4428      	add	r0, r5
30005abe:	f001 fb11 	bl	300070e4 <ymodem_uart_getdata>
30005ac2:	9b04      	ldr	r3, [sp, #16]
30005ac4:	2b00      	cmp	r3, #0
30005ac6:	d162      	bne.n	30005b8e <BOOT_Image1+0x71e>
30005ac8:	2002      	movs	r0, #2
30005aca:	f001 facf 	bl	3000706c <ymodem_uart_port_deinit>
30005ace:	4866      	ldr	r0, [pc, #408]	; (30005c68 <BOOT_Image1+0x7f8>)
30005ad0:	f005 fc7a 	bl	3000b3c8 <__DiagPrintf_veneer>
30005ad4:	f005 fa36 	bl	3000af44 <BOOT_VerCheck>
30005ad8:	f7fe fd9e 	bl	30004618 <BOOT_LoadImages>
30005adc:	2800      	cmp	r0, #0
30005ade:	f000 80b3 	beq.w	30005c48 <BOOT_Image1+0x7d8>
30005ae2:	f7ff f999 	bl	30004e18 <BOOT_Enable_KM0>
30005ae6:	2200      	movs	r2, #0
30005ae8:	2101      	movs	r1, #1
30005aea:	4860      	ldr	r0, [pc, #384]	; (30005c6c <BOOT_Image1+0x7fc>)
30005aec:	4b60      	ldr	r3, [pc, #384]	; (30005c70 <BOOT_Image1+0x800>)
30005aee:	4798      	blx	r3
30005af0:	4b60      	ldr	r3, [pc, #384]	; (30005c74 <BOOT_Image1+0x804>)
30005af2:	2008      	movs	r0, #8
30005af4:	4798      	blx	r3
30005af6:	f001 f85d 	bl	30006bb4 <BOOT_RAM_TZCfg>
30005afa:	4b5f      	ldr	r3, [pc, #380]	; (30005c78 <BOOT_Image1+0x808>)
30005afc:	781b      	ldrb	r3, [r3, #0]
30005afe:	2b01      	cmp	r3, #1
30005b00:	d106      	bne.n	30005b10 <BOOT_Image1+0x6a0>
30005b02:	4b5e      	ldr	r3, [pc, #376]	; (30005c7c <BOOT_Image1+0x80c>)
30005b04:	4798      	blx	r3
30005b06:	2801      	cmp	r0, #1
30005b08:	bf0c      	ite	eq
30005b0a:	2019      	moveq	r0, #25
30005b0c:	2004      	movne	r0, #4
30005b0e:	47b0      	blx	r6
30005b10:	4b5b      	ldr	r3, [pc, #364]	; (30005c80 <BOOT_Image1+0x810>)
30005b12:	781b      	ldrb	r3, [r3, #0]
30005b14:	2b00      	cmp	r3, #0
30005b16:	f000 8094 	beq.w	30005c42 <BOOT_Image1+0x7d2>
30005b1a:	f7ff fa9b 	bl	30005054 <BOOT_Enable_AP>
30005b1e:	4a59      	ldr	r2, [pc, #356]	; (30005c84 <BOOT_Image1+0x814>)
30005b20:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
30005b24:	f043 0306 	orr.w	r3, r3, #6
30005b28:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
30005b2c:	4b52      	ldr	r3, [pc, #328]	; (30005c78 <BOOT_Image1+0x808>)
30005b2e:	781b      	ldrb	r3, [r3, #0]
30005b30:	2b01      	cmp	r3, #1
30005b32:	d106      	bne.n	30005b42 <BOOT_Image1+0x6d2>
30005b34:	4b51      	ldr	r3, [pc, #324]	; (30005c7c <BOOT_Image1+0x80c>)
30005b36:	4798      	blx	r3
30005b38:	2801      	cmp	r0, #1
30005b3a:	bf0c      	ite	eq
30005b3c:	20e6      	moveq	r0, #230	; 0xe6
30005b3e:	2019      	movne	r0, #25
30005b40:	47b0      	blx	r6
30005b42:	4a50      	ldr	r2, [pc, #320]	; (30005c84 <BOOT_Image1+0x814>)
30005b44:	f892 3067 	ldrb.w	r3, [r2, #103]	; 0x67
30005b48:	f043 0301 	orr.w	r3, r3, #1
30005b4c:	f882 3067 	strb.w	r3, [r2, #103]	; 0x67
30005b50:	4b4d      	ldr	r3, [pc, #308]	; (30005c88 <BOOT_Image1+0x818>)
30005b52:	4a4e      	ldr	r2, [pc, #312]	; (30005c8c <BOOT_Image1+0x81c>)
30005b54:	689c      	ldr	r4, [r3, #8]
30005b56:	681b      	ldr	r3, [r3, #0]
30005b58:	6063      	str	r3, [r4, #4]
30005b5a:	4b4d      	ldr	r3, [pc, #308]	; (30005c90 <BOOT_Image1+0x820>)
30005b5c:	609c      	str	r4, [r3, #8]
30005b5e:	4b4d      	ldr	r3, [pc, #308]	; (30005c94 <BOOT_Image1+0x824>)
30005b60:	61da      	str	r2, [r3, #28]
30005b62:	f7fe fc39 	bl	300043d8 <BOOT_SCBConfig_HP>
30005b66:	f7fe fc59 	bl	3000441c <BOOT_SCBVTORBackup_HP>
30005b6a:	f7fe fc6b 	bl	30004444 <BOOT_NVICBackup_HP>
30005b6e:	4b4a      	ldr	r3, [pc, #296]	; (30005c98 <BOOT_Image1+0x828>)
30005b70:	f383 8888 	msr	MSP_NS, r3
30005b74:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
30005b78:	f383 8809 	msr	PSP, r3
30005b7c:	6860      	ldr	r0, [r4, #4]
30005b7e:	f7fe f933 	bl	30003de8 <BOOT_NsStart>
30005b82:	b00b      	add	sp, #44	; 0x2c
30005b84:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005b88:	4681      	mov	r9, r0
30005b8a:	4682      	mov	sl, r0
30005b8c:	e799      	b.n	30005ac2 <BOOT_Image1+0x652>
30005b8e:	2004      	movs	r0, #4
30005b90:	4d42      	ldr	r5, [pc, #264]	; (30005c9c <BOOT_Image1+0x82c>)
30005b92:	f001 fa6b 	bl	3000706c <ymodem_uart_port_deinit>
30005b96:	eb04 030a 	add.w	r3, r4, sl
30005b9a:	f205 4045 	addw	r0, r5, #1093	; 0x445
30005b9e:	340a      	adds	r4, #10
30005ba0:	330b      	adds	r3, #11
30005ba2:	f8cd 9000 	str.w	r9, [sp]
30005ba6:	4652      	mov	r2, sl
30005ba8:	f8d5 14d0 	ldr.w	r1, [r5, #1232]	; 0x4d0
30005bac:	4403      	add	r3, r0
30005bae:	4420      	add	r0, r4
30005bb0:	f7ff fb96 	bl	300052e0 <password_hash_check>
30005bb4:	4604      	mov	r4, r0
30005bb6:	2800      	cmp	r0, #0
30005bb8:	d186      	bne.n	30005ac8 <BOOT_Image1+0x658>
30005bba:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005bbe:	f505 69a9 	add.w	r9, r5, #1352	; 0x548
30005bc2:	47b0      	blx	r6
30005bc4:	4836      	ldr	r0, [pc, #216]	; (30005ca0 <BOOT_Image1+0x830>)
30005bc6:	f005 fbdf 	bl	3000b388 <____wrap_strlen_veneer>
30005bca:	4601      	mov	r1, r0
30005bcc:	4834      	ldr	r0, [pc, #208]	; (30005ca0 <BOOT_Image1+0x830>)
30005bce:	f205 5a1c 	addw	sl, r5, #1308	; 0x51c
30005bd2:	f001 fa79 	bl	300070c8 <ymodem_uart_putdata>
30005bd6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005bda:	47b0      	blx	r6
30005bdc:	f8d5 24d4 	ldr.w	r2, [r5, #1236]	; 0x4d4
30005be0:	f205 41dc 	addw	r1, r5, #1244	; 0x4dc
30005be4:	2002      	movs	r0, #2
30005be6:	f001 faf5 	bl	300071d4 <ymodem_img_download>
30005bea:	2032      	movs	r0, #50	; 0x32
30005bec:	47b0      	blx	r6
30005bee:	f8d5 34d4 	ldr.w	r3, [r5, #1236]	; 0x4d4
30005bf2:	42a3      	cmp	r3, r4
30005bf4:	d811      	bhi.n	30005c1a <BOOT_Image1+0x7aa>
30005bf6:	482b      	ldr	r0, [pc, #172]	; (30005ca4 <BOOT_Image1+0x834>)
30005bf8:	f005 fbe6 	bl	3000b3c8 <__DiagPrintf_veneer>
30005bfc:	a808      	add	r0, sp, #32
30005bfe:	4b2a      	ldr	r3, [pc, #168]	; (30005ca8 <BOOT_Image1+0x838>)
30005c00:	4798      	blx	r3
30005c02:	f44f 73fa 	mov.w	r3, #500	; 0x1f4
30005c06:	4829      	ldr	r0, [pc, #164]	; (30005cac <BOOT_Image1+0x83c>)
30005c08:	a908      	add	r1, sp, #32
30005c0a:	f8ad 3022 	strh.w	r3, [sp, #34]	; 0x22
30005c0e:	4b28      	ldr	r3, [pc, #160]	; (30005cb0 <BOOT_Image1+0x840>)
30005c10:	4798      	blx	r3
30005c12:	4826      	ldr	r0, [pc, #152]	; (30005cac <BOOT_Image1+0x83c>)
30005c14:	4b27      	ldr	r3, [pc, #156]	; (30005cb4 <BOOT_Image1+0x844>)
30005c16:	4798      	blx	r3
30005c18:	e7fe      	b.n	30005c18 <BOOT_Image1+0x7a8>
30005c1a:	f859 3b04 	ldr.w	r3, [r9], #4
30005c1e:	2700      	movs	r7, #0
30005c20:	ea4f 3813 	mov.w	r8, r3, lsr #12
30005c24:	4547      	cmp	r7, r8
30005c26:	db01      	blt.n	30005c2c <BOOT_Image1+0x7bc>
30005c28:	3401      	adds	r4, #1
30005c2a:	e7e0      	b.n	30005bee <BOOT_Image1+0x77e>
30005c2c:	f85a 1024 	ldr.w	r1, [sl, r4, lsl #2]
30005c30:	2002      	movs	r0, #2
30005c32:	eb01 3107 	add.w	r1, r1, r7, lsl #12
30005c36:	3701      	adds	r7, #1
30005c38:	f7fd ff70 	bl	30003b1c <FLASH_Erase_With_Lock>
30005c3c:	2032      	movs	r0, #50	; 0x32
30005c3e:	47b0      	blx	r6
30005c40:	e7f0      	b.n	30005c24 <BOOT_Image1+0x7b4>
30005c42:	f7ff f9f5 	bl	30005030 <BOOT_Disable_AP>
30005c46:	e771      	b.n	30005b2c <BOOT_Image1+0x6bc>
30005c48:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005c4c:	47b0      	blx	r6
30005c4e:	e7fb      	b.n	30005c48 <BOOT_Image1+0x7d8>
30005c50:	3000dbf4 	.word	0x3000dbf4
30005c54:	3000dc2f 	.word	0x3000dc2f
30005c58:	3000dc34 	.word	0x3000dc34
30005c5c:	3000dc5c 	.word	0x3000dc5c
30005c60:	3000dc60 	.word	0x3000dc60
30005c64:	3000dc88 	.word	0x3000dc88
30005c68:	3000cec0 	.word	0x3000cec0
30005c6c:	4200c000 	.word	0x4200c000
30005c70:	0000ac6d 	.word	0x0000ac6d
30005c74:	0000e1d9 	.word	0x0000e1d9
30005c78:	3000d550 	.word	0x3000d550
30005c7c:	0000c169 	.word	0x0000c169
30005c80:	3000d553 	.word	0x3000d553
30005c84:	42008200 	.word	0x42008200
30005c88:	60000020 	.word	0x60000020
30005c8c:	30003941 	.word	0x30003941
30005c90:	e002ed00 	.word	0xe002ed00
30005c94:	30000000 	.word	0x30000000
30005c98:	2001bffc 	.word	0x2001bffc
30005c9c:	3000d718 	.word	0x3000d718
30005ca0:	3000e514 	.word	0x3000e514
30005ca4:	3000cef0 	.word	0x3000cef0
30005ca8:	0000d3f9 	.word	0x0000d3f9
30005cac:	41000440 	.word	0x41000440
30005cb0:	0000d409 	.word	0x0000d409
30005cb4:	0000d489 	.word	0x0000d489

30005cb8 <BOOT_LoadSubImage>:
30005cb8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005cbc:	4607      	mov	r7, r0
30005cbe:	b08d      	sub	sp, #52	; 0x34
30005cc0:	460d      	mov	r5, r1
30005cc2:	4692      	mov	sl, r2
30005cc4:	4699      	mov	r9, r3
30005cc6:	2600      	movs	r6, #0
30005cc8:	f8df 8120 	ldr.w	r8, [pc, #288]	; 30005dec <BOOT_LoadSubImage+0x134>
30005ccc:	4556      	cmp	r6, sl
30005cce:	d303      	bcc.n	30005cd8 <BOOT_LoadSubImage+0x20>
30005cd0:	4b40      	ldr	r3, [pc, #256]	; (30005dd4 <BOOT_LoadSubImage+0x11c>)
30005cd2:	2001      	movs	r0, #1
30005cd4:	601d      	str	r5, [r3, #0]
30005cd6:	e00d      	b.n	30005cf4 <BOOT_LoadSubImage+0x3c>
30005cd8:	2220      	movs	r2, #32
30005cda:	4629      	mov	r1, r5
30005cdc:	a804      	add	r0, sp, #16
30005cde:	f7fe fc81 	bl	300045e4 <BOOT_ImgCopy>
30005ce2:	9b04      	ldr	r3, [sp, #16]
30005ce4:	4a3c      	ldr	r2, [pc, #240]	; (30005dd8 <BOOT_LoadSubImage+0x120>)
30005ce6:	4293      	cmp	r3, r2
30005ce8:	d007      	beq.n	30005cfa <BOOT_LoadSubImage+0x42>
30005cea:	f89d 3058 	ldrb.w	r3, [sp, #88]	; 0x58
30005cee:	2b01      	cmp	r3, #1
30005cf0:	d053      	beq.n	30005d9a <BOOT_LoadSubImage+0xe2>
30005cf2:	2000      	movs	r0, #0
30005cf4:	b00d      	add	sp, #52	; 0x34
30005cf6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005cfa:	9b05      	ldr	r3, [sp, #20]
30005cfc:	4a37      	ldr	r2, [pc, #220]	; (30005ddc <BOOT_LoadSubImage+0x124>)
30005cfe:	4293      	cmp	r3, r2
30005d00:	d1f3      	bne.n	30005cea <BOOT_LoadSubImage+0x32>
30005d02:	9b07      	ldr	r3, [sp, #28]
30005d04:	9c06      	ldr	r4, [sp, #24]
30005d06:	f1a3 0b20 	sub.w	fp, r3, #32
30005d0a:	f103 4378 	add.w	r3, r3, #4160749568	; 0xf8000000
30005d0e:	3420      	adds	r4, #32
30005d10:	3b20      	subs	r3, #32
30005d12:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005d16:	d32c      	bcc.n	30005d72 <BOOT_LoadSubImage+0xba>
30005d18:	4622      	mov	r2, r4
30005d1a:	4629      	mov	r1, r5
30005d1c:	4658      	mov	r0, fp
30005d1e:	f7fe fc61 	bl	300045e4 <BOOT_ImgCopy>
30005d22:	f8d8 3014 	ldr.w	r3, [r8, #20]
30005d26:	03da      	lsls	r2, r3, #15
30005d28:	d523      	bpl.n	30005d72 <BOOT_LoadSubImage+0xba>
30005d2a:	f1bb 3fff 	cmp.w	fp, #4294967295	; 0xffffffff
30005d2e:	d13e      	bne.n	30005dae <BOOT_LoadSubImage+0xf6>
30005d30:	1c63      	adds	r3, r4, #1
30005d32:	d13c      	bne.n	30005dae <BOOT_LoadSubImage+0xf6>
30005d34:	2300      	movs	r3, #0
30005d36:	f8c8 3084 	str.w	r3, [r8, #132]	; 0x84
30005d3a:	f3bf 8f4f 	dsb	sy
30005d3e:	f8d8 2080 	ldr.w	r2, [r8, #128]	; 0x80
30005d42:	f643 70e0 	movw	r0, #16352	; 0x3fe0
30005d46:	f3c2 01c9 	ubfx	r1, r2, #3, #10
30005d4a:	f3c2 324e 	ubfx	r2, r2, #13, #15
30005d4e:	0152      	lsls	r2, r2, #5
30005d50:	ea02 0e00 	and.w	lr, r2, r0
30005d54:	460b      	mov	r3, r1
30005d56:	ea4e 7c83 	orr.w	ip, lr, r3, lsl #30
30005d5a:	3b01      	subs	r3, #1
30005d5c:	f8c8 c274 	str.w	ip, [r8, #628]	; 0x274
30005d60:	d2f9      	bcs.n	30005d56 <BOOT_LoadSubImage+0x9e>
30005d62:	3a20      	subs	r2, #32
30005d64:	f112 0f20 	cmn.w	r2, #32
30005d68:	d1f2      	bne.n	30005d50 <BOOT_LoadSubImage+0x98>
30005d6a:	f3bf 8f4f 	dsb	sy
30005d6e:	f3bf 8f6f 	isb	sy
30005d72:	b17f      	cbz	r7, 30005d94 <BOOT_LoadSubImage+0xdc>
30005d74:	1d3b      	adds	r3, r7, #4
30005d76:	f847 b036 	str.w	fp, [r7, r6, lsl #3]
30005d7a:	4a19      	ldr	r2, [pc, #100]	; (30005de0 <BOOT_LoadSubImage+0x128>)
30005d7c:	2004      	movs	r0, #4
30005d7e:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
30005d82:	f859 3026 	ldr.w	r3, [r9, r6, lsl #2]
30005d86:	9300      	str	r3, [sp, #0]
30005d88:	4b16      	ldr	r3, [pc, #88]	; (30005de4 <BOOT_LoadSubImage+0x12c>)
30005d8a:	e9cd b401 	strd	fp, r4, [sp, #4]
30005d8e:	4619      	mov	r1, r3
30005d90:	f005 f8c4 	bl	3000af1c <rtk_log_write>
30005d94:	4425      	add	r5, r4
30005d96:	3601      	adds	r6, #1
30005d98:	e798      	b.n	30005ccc <BOOT_LoadSubImage+0x14>
30005d9a:	f859 3026 	ldr.w	r3, [r9, r6, lsl #2]
30005d9e:	2004      	movs	r0, #4
30005da0:	4a11      	ldr	r2, [pc, #68]	; (30005de8 <BOOT_LoadSubImage+0x130>)
30005da2:	9300      	str	r3, [sp, #0]
30005da4:	4b0f      	ldr	r3, [pc, #60]	; (30005de4 <BOOT_LoadSubImage+0x12c>)
30005da6:	4619      	mov	r1, r3
30005da8:	f005 f8b8 	bl	3000af1c <rtk_log_write>
30005dac:	e7a1      	b.n	30005cf2 <BOOT_LoadSubImage+0x3a>
30005dae:	f01b 031f 	ands.w	r3, fp, #31
30005db2:	bf19      	ittee	ne
30005db4:	f02b 021f 	bicne.w	r2, fp, #31
30005db8:	191b      	addne	r3, r3, r4
30005dba:	4623      	moveq	r3, r4
30005dbc:	465a      	moveq	r2, fp
30005dbe:	f3bf 8f4f 	dsb	sy
30005dc2:	441a      	add	r2, r3
30005dc4:	2b00      	cmp	r3, #0
30005dc6:	eba2 0103 	sub.w	r1, r2, r3
30005dca:	ddce      	ble.n	30005d6a <BOOT_LoadSubImage+0xb2>
30005dcc:	3b20      	subs	r3, #32
30005dce:	f8c8 1270 	str.w	r1, [r8, #624]	; 0x270
30005dd2:	e7f7      	b.n	30005dc4 <BOOT_LoadSubImage+0x10c>
30005dd4:	3000dc8c 	.word	0x3000dc8c
30005dd8:	35393138 	.word	0x35393138
30005ddc:	31313738 	.word	0x31313738
30005de0:	3000cfb9 	.word	0x3000cfb9
30005de4:	3000c714 	.word	0x3000c714
30005de8:	3000cfa6 	.word	0x3000cfa6
30005dec:	e000ed00 	.word	0xe000ed00

30005df0 <BOOT_LoadAPSubImage>:
30005df0:	4b9c      	ldr	r3, [pc, #624]	; (30006064 <BOOT_LoadAPSubImage+0x274>)
30005df2:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30005df6:	b08e      	sub	sp, #56	; 0x38
30005df8:	4607      	mov	r7, r0
30005dfa:	4614      	mov	r4, r2
30005dfc:	6818      	ldr	r0, [r3, #0]
30005dfe:	889a      	ldrh	r2, [r3, #4]
30005e00:	ae04      	add	r6, sp, #16
30005e02:	799b      	ldrb	r3, [r3, #6]
30005e04:	4689      	mov	r9, r1
30005e06:	9004      	str	r0, [sp, #16]
30005e08:	a806      	add	r0, sp, #24
30005e0a:	f8ad 2014 	strh.w	r2, [sp, #20]
30005e0e:	2220      	movs	r2, #32
30005e10:	71b3      	strb	r3, [r6, #6]
30005e12:	f7fe fbe7 	bl	300045e4 <BOOT_ImgCopy>
30005e16:	4b94      	ldr	r3, [pc, #592]	; (30006068 <BOOT_LoadAPSubImage+0x278>)
30005e18:	9a06      	ldr	r2, [sp, #24]
30005e1a:	429a      	cmp	r2, r3
30005e1c:	d108      	bne.n	30005e30 <BOOT_LoadAPSubImage+0x40>
30005e1e:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30005e22:	9a07      	ldr	r2, [sp, #28]
30005e24:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30005e28:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30005e2c:	429a      	cmp	r2, r3
30005e2e:	d00a      	beq.n	30005e46 <BOOT_LoadAPSubImage+0x56>
30005e30:	2c01      	cmp	r4, #1
30005e32:	d106      	bne.n	30005e42 <BOOT_LoadAPSubImage+0x52>
30005e34:	4b8d      	ldr	r3, [pc, #564]	; (3000606c <BOOT_LoadAPSubImage+0x27c>)
30005e36:	2004      	movs	r0, #4
30005e38:	4a8d      	ldr	r2, [pc, #564]	; (30006070 <BOOT_LoadAPSubImage+0x280>)
30005e3a:	4619      	mov	r1, r3
30005e3c:	9600      	str	r6, [sp, #0]
30005e3e:	f005 f86d 	bl	3000af1c <rtk_log_write>
30005e42:	2000      	movs	r0, #0
30005e44:	e053      	b.n	30005eee <BOOT_LoadAPSubImage+0xfe>
30005e46:	9b09      	ldr	r3, [sp, #36]	; 0x24
30005e48:	f8d9 5048 	ldr.w	r5, [r9, #72]	; 0x48
30005e4c:	f1a3 0420 	sub.w	r4, r3, #32
30005e50:	f103 4378 	add.w	r3, r3, #4160749568	; 0xf8000000
30005e54:	35d0      	adds	r5, #208	; 0xd0
30005e56:	3b20      	subs	r3, #32
30005e58:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005e5c:	d32b      	bcc.n	30005eb6 <BOOT_LoadAPSubImage+0xc6>
30005e5e:	462a      	mov	r2, r5
30005e60:	4649      	mov	r1, r9
30005e62:	4620      	mov	r0, r4
30005e64:	f7fe fbbe 	bl	300045e4 <BOOT_ImgCopy>
30005e68:	4a82      	ldr	r2, [pc, #520]	; (30006074 <BOOT_LoadAPSubImage+0x284>)
30005e6a:	6953      	ldr	r3, [r2, #20]
30005e6c:	03d9      	lsls	r1, r3, #15
30005e6e:	d522      	bpl.n	30005eb6 <BOOT_LoadAPSubImage+0xc6>
30005e70:	1c63      	adds	r3, r4, #1
30005e72:	d13f      	bne.n	30005ef4 <BOOT_LoadAPSubImage+0x104>
30005e74:	1c68      	adds	r0, r5, #1
30005e76:	d13d      	bne.n	30005ef4 <BOOT_LoadAPSubImage+0x104>
30005e78:	2300      	movs	r3, #0
30005e7a:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30005e7e:	f3bf 8f4f 	dsb	sy
30005e82:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30005e86:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30005e8a:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30005e8e:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005e92:	015b      	lsls	r3, r3, #5
30005e94:	ea03 080c 	and.w	r8, r3, ip
30005e98:	4601      	mov	r1, r0
30005e9a:	ea48 7e81 	orr.w	lr, r8, r1, lsl #30
30005e9e:	3901      	subs	r1, #1
30005ea0:	f8c2 e274 	str.w	lr, [r2, #628]	; 0x274
30005ea4:	d2f9      	bcs.n	30005e9a <BOOT_LoadAPSubImage+0xaa>
30005ea6:	3b20      	subs	r3, #32
30005ea8:	f113 0f20 	cmn.w	r3, #32
30005eac:	d1f2      	bne.n	30005e94 <BOOT_LoadAPSubImage+0xa4>
30005eae:	f3bf 8f4f 	dsb	sy
30005eb2:	f3bf 8f6f 	isb	sy
30005eb6:	4b70      	ldr	r3, [pc, #448]	; (30006078 <BOOT_LoadAPSubImage+0x288>)
30005eb8:	442c      	add	r4, r5
30005eba:	f8d9 8070 	ldr.w	r8, [r9, #112]	; 0x70
30005ebe:	4798      	blx	r3
30005ec0:	2801      	cmp	r0, #1
30005ec2:	444d      	add	r5, r9
30005ec4:	4b69      	ldr	r3, [pc, #420]	; (3000606c <BOOT_LoadAPSubImage+0x27c>)
30005ec6:	d129      	bne.n	30005f1c <BOOT_LoadAPSubImage+0x12c>
30005ec8:	4a6c      	ldr	r2, [pc, #432]	; (3000607c <BOOT_LoadAPSubImage+0x28c>)
30005eca:	4619      	mov	r1, r3
30005ecc:	2004      	movs	r0, #4
30005ece:	f005 f825 	bl	3000af1c <rtk_log_write>
30005ed2:	4642      	mov	r2, r8
30005ed4:	4621      	mov	r1, r4
30005ed6:	4628      	mov	r0, r5
30005ed8:	f000 fb84 	bl	300065e4 <BOOT_LoadBL32RDPImg>
30005edc:	2800      	cmp	r0, #0
30005ede:	d153      	bne.n	30005f88 <BOOT_LoadAPSubImage+0x198>
30005ee0:	4b62      	ldr	r3, [pc, #392]	; (3000606c <BOOT_LoadAPSubImage+0x27c>)
30005ee2:	2004      	movs	r0, #4
30005ee4:	4a66      	ldr	r2, [pc, #408]	; (30006080 <BOOT_LoadAPSubImage+0x290>)
30005ee6:	4619      	mov	r1, r3
30005ee8:	f005 f818 	bl	3000af1c <rtk_log_write>
30005eec:	20ff      	movs	r0, #255	; 0xff
30005eee:	b00e      	add	sp, #56	; 0x38
30005ef0:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30005ef4:	f014 031f 	ands.w	r3, r4, #31
30005ef8:	bf19      	ittee	ne
30005efa:	f024 021f 	bicne.w	r2, r4, #31
30005efe:	195b      	addne	r3, r3, r5
30005f00:	462b      	moveq	r3, r5
30005f02:	4622      	moveq	r2, r4
30005f04:	f3bf 8f4f 	dsb	sy
30005f08:	485a      	ldr	r0, [pc, #360]	; (30006074 <BOOT_LoadAPSubImage+0x284>)
30005f0a:	441a      	add	r2, r3
30005f0c:	2b00      	cmp	r3, #0
30005f0e:	eba2 0103 	sub.w	r1, r2, r3
30005f12:	ddcc      	ble.n	30005eae <BOOT_LoadAPSubImage+0xbe>
30005f14:	3b20      	subs	r3, #32
30005f16:	f8c0 1270 	str.w	r1, [r0, #624]	; 0x270
30005f1a:	e7f7      	b.n	30005f0c <BOOT_LoadAPSubImage+0x11c>
30005f1c:	4619      	mov	r1, r3
30005f1e:	4a59      	ldr	r2, [pc, #356]	; (30006084 <BOOT_LoadAPSubImage+0x294>)
30005f20:	2004      	movs	r0, #4
30005f22:	f004 fffb 	bl	3000af1c <rtk_log_write>
30005f26:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30005f2a:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005f2e:	d32b      	bcc.n	30005f88 <BOOT_LoadAPSubImage+0x198>
30005f30:	4642      	mov	r2, r8
30005f32:	4629      	mov	r1, r5
30005f34:	4620      	mov	r0, r4
30005f36:	f7fe fb55 	bl	300045e4 <BOOT_ImgCopy>
30005f3a:	4a4e      	ldr	r2, [pc, #312]	; (30006074 <BOOT_LoadAPSubImage+0x284>)
30005f3c:	6953      	ldr	r3, [r2, #20]
30005f3e:	03d9      	lsls	r1, r3, #15
30005f40:	d522      	bpl.n	30005f88 <BOOT_LoadAPSubImage+0x198>
30005f42:	ea08 0304 	and.w	r3, r8, r4
30005f46:	3301      	adds	r3, #1
30005f48:	d165      	bne.n	30006016 <BOOT_LoadAPSubImage+0x226>
30005f4a:	2300      	movs	r3, #0
30005f4c:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30005f50:	f3bf 8f4f 	dsb	sy
30005f54:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30005f58:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30005f5c:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30005f60:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005f64:	015b      	lsls	r3, r3, #5
30005f66:	ea03 0a0c 	and.w	sl, r3, ip
30005f6a:	4601      	mov	r1, r0
30005f6c:	ea4a 7e81 	orr.w	lr, sl, r1, lsl #30
30005f70:	3901      	subs	r1, #1
30005f72:	f8c2 e274 	str.w	lr, [r2, #628]	; 0x274
30005f76:	d2f9      	bcs.n	30005f6c <BOOT_LoadAPSubImage+0x17c>
30005f78:	3b20      	subs	r3, #32
30005f7a:	f113 0f20 	cmn.w	r3, #32
30005f7e:	d1f2      	bne.n	30005f66 <BOOT_LoadAPSubImage+0x176>
30005f80:	f3bf 8f4f 	dsb	sy
30005f84:	f3bf 8f6f 	isb	sy
30005f88:	4444      	add	r4, r8
30005f8a:	eb08 0105 	add.w	r1, r8, r5
30005f8e:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30005f92:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005f96:	d32c      	bcc.n	30005ff2 <BOOT_LoadAPSubImage+0x202>
30005f98:	f8d9 5098 	ldr.w	r5, [r9, #152]	; 0x98
30005f9c:	4620      	mov	r0, r4
30005f9e:	462a      	mov	r2, r5
30005fa0:	f7fe fb20 	bl	300045e4 <BOOT_ImgCopy>
30005fa4:	4a33      	ldr	r2, [pc, #204]	; (30006074 <BOOT_LoadAPSubImage+0x284>)
30005fa6:	6953      	ldr	r3, [r2, #20]
30005fa8:	03db      	lsls	r3, r3, #15
30005faa:	d522      	bpl.n	30005ff2 <BOOT_LoadAPSubImage+0x202>
30005fac:	ea04 0305 	and.w	r3, r4, r5
30005fb0:	3301      	adds	r3, #1
30005fb2:	d144      	bne.n	3000603e <BOOT_LoadAPSubImage+0x24e>
30005fb4:	2300      	movs	r3, #0
30005fb6:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30005fba:	f3bf 8f4f 	dsb	sy
30005fbe:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30005fc2:	f643 74e0 	movw	r4, #16352	; 0x3fe0
30005fc6:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30005fca:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005fce:	015b      	lsls	r3, r3, #5
30005fd0:	ea03 0c04 	and.w	ip, r3, r4
30005fd4:	4601      	mov	r1, r0
30005fd6:	ea4c 7581 	orr.w	r5, ip, r1, lsl #30
30005fda:	3901      	subs	r1, #1
30005fdc:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
30005fe0:	d2f9      	bcs.n	30005fd6 <BOOT_LoadAPSubImage+0x1e6>
30005fe2:	3b20      	subs	r3, #32
30005fe4:	f113 0f20 	cmn.w	r3, #32
30005fe8:	d1f2      	bne.n	30005fd0 <BOOT_LoadAPSubImage+0x1e0>
30005fea:	f3bf 8f4f 	dsb	sy
30005fee:	f3bf 8f6f 	isb	sy
30005ff2:	b177      	cbz	r7, 30006012 <BOOT_LoadAPSubImage+0x222>
30005ff4:	9b09      	ldr	r3, [sp, #36]	; 0x24
30005ff6:	2004      	movs	r0, #4
30005ff8:	9a08      	ldr	r2, [sp, #32]
30005ffa:	3b20      	subs	r3, #32
30005ffc:	9600      	str	r6, [sp, #0]
30005ffe:	3220      	adds	r2, #32
30006000:	e9c7 3200 	strd	r3, r2, [r7]
30006004:	e9cd 3201 	strd	r3, r2, [sp, #4]
30006008:	4b18      	ldr	r3, [pc, #96]	; (3000606c <BOOT_LoadAPSubImage+0x27c>)
3000600a:	4a1f      	ldr	r2, [pc, #124]	; (30006088 <BOOT_LoadAPSubImage+0x298>)
3000600c:	4619      	mov	r1, r3
3000600e:	f004 ff85 	bl	3000af1c <rtk_log_write>
30006012:	2001      	movs	r0, #1
30006014:	e76b      	b.n	30005eee <BOOT_LoadAPSubImage+0xfe>
30006016:	f014 031f 	ands.w	r3, r4, #31
3000601a:	bf19      	ittee	ne
3000601c:	f024 021f 	bicne.w	r2, r4, #31
30006020:	4443      	addne	r3, r8
30006022:	4643      	moveq	r3, r8
30006024:	4622      	moveq	r2, r4
30006026:	f3bf 8f4f 	dsb	sy
3000602a:	4812      	ldr	r0, [pc, #72]	; (30006074 <BOOT_LoadAPSubImage+0x284>)
3000602c:	441a      	add	r2, r3
3000602e:	2b00      	cmp	r3, #0
30006030:	eba2 0103 	sub.w	r1, r2, r3
30006034:	dda4      	ble.n	30005f80 <BOOT_LoadAPSubImage+0x190>
30006036:	3b20      	subs	r3, #32
30006038:	f8c0 1270 	str.w	r1, [r0, #624]	; 0x270
3000603c:	e7f7      	b.n	3000602e <BOOT_LoadAPSubImage+0x23e>
3000603e:	f014 031f 	ands.w	r3, r4, #31
30006042:	bf1c      	itt	ne
30006044:	f024 041f 	bicne.w	r4, r4, #31
30006048:	18ed      	addne	r5, r5, r3
3000604a:	f3bf 8f4f 	dsb	sy
3000604e:	4a09      	ldr	r2, [pc, #36]	; (30006074 <BOOT_LoadAPSubImage+0x284>)
30006050:	442c      	add	r4, r5
30006052:	2d00      	cmp	r5, #0
30006054:	eba4 0305 	sub.w	r3, r4, r5
30006058:	ddc7      	ble.n	30005fea <BOOT_LoadAPSubImage+0x1fa>
3000605a:	3d20      	subs	r5, #32
3000605c:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30006060:	e7f7      	b.n	30006052 <BOOT_LoadAPSubImage+0x262>
30006062:	bf00      	nop
30006064:	3000cf9f 	.word	0x3000cf9f
30006068:	35393138 	.word	0x35393138
3000606c:	3000c714 	.word	0x3000c714
30006070:	3000cfa6 	.word	0x3000cfa6
30006074:	e000ed00 	.word	0xe000ed00
30006078:	0000c109 	.word	0x0000c109
3000607c:	3000cfcd 	.word	0x3000cfcd
30006080:	3000cfdc 	.word	0x3000cfdc
30006084:	3000cffc 	.word	0x3000cffc
30006088:	3000cfb9 	.word	0x3000cfb9

3000608c <BOOT_RSIPIvSet>:
3000608c:	2300      	movs	r3, #0
3000608e:	b537      	push	{r0, r1, r2, r4, r5, lr}
30006090:	e9cd 3300 	strd	r3, r3, [sp]
30006094:	4b08      	ldr	r3, [pc, #32]	; (300060b8 <BOOT_RSIPIvSet+0x2c>)
30006096:	4605      	mov	r5, r0
30006098:	460c      	mov	r4, r1
3000609a:	4798      	blx	r3
3000609c:	b148      	cbz	r0, 300060b2 <BOOT_RSIPIvSet+0x26>
3000609e:	f105 0120 	add.w	r1, r5, #32
300060a2:	4b06      	ldr	r3, [pc, #24]	; (300060bc <BOOT_RSIPIvSet+0x30>)
300060a4:	2208      	movs	r2, #8
300060a6:	4668      	mov	r0, sp
300060a8:	4798      	blx	r3
300060aa:	4669      	mov	r1, sp
300060ac:	b2e0      	uxtb	r0, r4
300060ae:	4b04      	ldr	r3, [pc, #16]	; (300060c0 <BOOT_RSIPIvSet+0x34>)
300060b0:	4798      	blx	r3
300060b2:	b003      	add	sp, #12
300060b4:	bd30      	pop	{r4, r5, pc}
300060b6:	bf00      	nop
300060b8:	0000c129 	.word	0x0000c129
300060bc:	00012a1d 	.word	0x00012a1d
300060c0:	30008cf9 	.word	0x30008cf9

300060c4 <BOOT_OTFCheck>:
300060c4:	e92d 41f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, lr}
300060c8:	461f      	mov	r7, r3
300060ca:	4b1b      	ldr	r3, [pc, #108]	; (30006138 <BOOT_OTFCheck+0x74>)
300060cc:	4605      	mov	r5, r0
300060ce:	460e      	mov	r6, r1
300060d0:	4690      	mov	r8, r2
300060d2:	4798      	blx	r3
300060d4:	b360      	cbz	r0, 30006130 <BOOT_OTFCheck+0x6c>
300060d6:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
300060da:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300060de:	d227      	bcs.n	30006130 <BOOT_OTFCheck+0x6c>
300060e0:	f106 4378 	add.w	r3, r6, #4160749568	; 0xf8000000
300060e4:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300060e8:	d222      	bcs.n	30006130 <BOOT_OTFCheck+0x6c>
300060ea:	4b14      	ldr	r3, [pc, #80]	; (3000613c <BOOT_OTFCheck+0x78>)
300060ec:	2004      	movs	r0, #4
300060ee:	4a14      	ldr	r2, [pc, #80]	; (30006140 <BOOT_OTFCheck+0x7c>)
300060f0:	4619      	mov	r1, r3
300060f2:	f004 ff13 	bl	3000af1c <rtk_log_write>
300060f6:	4b13      	ldr	r3, [pc, #76]	; (30006144 <BOOT_OTFCheck+0x80>)
300060f8:	4798      	blx	r3
300060fa:	b150      	cbz	r0, 30006112 <BOOT_OTFCheck+0x4e>
300060fc:	2801      	cmp	r0, #1
300060fe:	d009      	beq.n	30006114 <BOOT_OTFCheck+0x50>
30006100:	4b0e      	ldr	r3, [pc, #56]	; (3000613c <BOOT_OTFCheck+0x78>)
30006102:	2002      	movs	r0, #2
30006104:	4a10      	ldr	r2, [pc, #64]	; (30006148 <BOOT_OTFCheck+0x84>)
30006106:	4619      	mov	r1, r3
30006108:	b002      	add	sp, #8
3000610a:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000610e:	f004 bf05 	b.w	3000af1c <rtk_log_write>
30006112:	2002      	movs	r0, #2
30006114:	2301      	movs	r3, #1
30006116:	4632      	mov	r2, r6
30006118:	4629      	mov	r1, r5
3000611a:	4c0c      	ldr	r4, [pc, #48]	; (3000614c <BOOT_OTFCheck+0x88>)
3000611c:	e9cd 8000 	strd	r8, r0, [sp]
30006120:	4638      	mov	r0, r7
30006122:	47a0      	blx	r4
30006124:	2001      	movs	r0, #1
30006126:	4b0a      	ldr	r3, [pc, #40]	; (30006150 <BOOT_OTFCheck+0x8c>)
30006128:	b002      	add	sp, #8
3000612a:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000612e:	4718      	bx	r3
30006130:	b002      	add	sp, #8
30006132:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30006136:	bf00      	nop
30006138:	0000c129 	.word	0x0000c129
3000613c:	3000c714 	.word	0x3000c714
30006140:	3000d00c 	.word	0x3000d00c
30006144:	0000c245 	.word	0x0000c245
30006148:	3000d020 	.word	0x3000d020
3000614c:	30008d2d 	.word	0x30008d2d
30006150:	30008d15 	.word	0x30008d15

30006154 <BOOT_OTA_LoadIMG2>:
30006154:	4b67      	ldr	r3, [pc, #412]	; (300062f4 <BOOT_OTA_LoadIMG2+0x1a0>)
30006156:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000615a:	b08b      	sub	sp, #44	; 0x2c
3000615c:	681e      	ldr	r6, [r3, #0]
3000615e:	4b66      	ldr	r3, [pc, #408]	; (300062f8 <BOOT_OTA_LoadIMG2+0x1a4>)
30006160:	9003      	str	r0, [sp, #12]
30006162:	ac04      	add	r4, sp, #16
30006164:	f10d 0a1c 	add.w	sl, sp, #28
30006168:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
3000616c:	330c      	adds	r3, #12
3000616e:	e884 0007 	stmia.w	r4, {r0, r1, r2}
30006172:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30006176:	e88a 0007 	stmia.w	sl, {r0, r1, r2}
3000617a:	f44f 72b2 	mov.w	r2, #356	; 0x164
3000617e:	4631      	mov	r1, r6
30006180:	485e      	ldr	r0, [pc, #376]	; (300062fc <BOOT_OTA_LoadIMG2+0x1a8>)
30006182:	f7fe fa2f 	bl	300045e4 <BOOT_ImgCopy>
30006186:	4631      	mov	r1, r6
30006188:	485c      	ldr	r0, [pc, #368]	; (300062fc <BOOT_OTA_LoadIMG2+0x1a8>)
3000618a:	f000 fcb5 	bl	30006af8 <BOOT_CertificateCheck>
3000618e:	4607      	mov	r7, r0
30006190:	b118      	cbz	r0, 3000619a <BOOT_OTA_LoadIMG2+0x46>
30006192:	485b      	ldr	r0, [pc, #364]	; (30006300 <BOOT_OTA_LoadIMG2+0x1ac>)
30006194:	f005 f918 	bl	3000b3c8 <__DiagPrintf_veneer>
30006198:	e030      	b.n	300061fc <BOOT_OTA_LoadIMG2+0xa8>
3000619a:	f506 617f 	add.w	r1, r6, #4080	; 0xff0
3000619e:	f506 56ff 	add.w	r6, r6, #8160	; 0x1fe0
300061a2:	f44f 72c8 	mov.w	r2, #400	; 0x190
300061a6:	4d57      	ldr	r5, [pc, #348]	; (30006304 <BOOT_OTA_LoadIMG2+0x1b0>)
300061a8:	4857      	ldr	r0, [pc, #348]	; (30006308 <BOOT_OTA_LoadIMG2+0x1b4>)
300061aa:	3610      	adds	r6, #16
300061ac:	f7fe fa1a 	bl	300045e4 <BOOT_ImgCopy>
300061b0:	2101      	movs	r1, #1
300061b2:	4855      	ldr	r0, [pc, #340]	; (30006308 <BOOT_OTA_LoadIMG2+0x1b4>)
300061b4:	f04f 0801 	mov.w	r8, #1
300061b8:	f7ff ff68 	bl	3000608c <BOOT_RSIPIvSet>
300061bc:	4c53      	ldr	r4, [pc, #332]	; (3000630c <BOOT_OTA_LoadIMG2+0x1b8>)
300061be:	4a54      	ldr	r2, [pc, #336]	; (30006310 <BOOT_OTA_LoadIMG2+0x1bc>)
300061c0:	4633      	mov	r3, r6
300061c2:	4629      	mov	r1, r5
300061c4:	4638      	mov	r0, r7
300061c6:	47a0      	blx	r4
300061c8:	2101      	movs	r1, #1
300061ca:	4638      	mov	r0, r7
300061cc:	4b51      	ldr	r3, [pc, #324]	; (30006314 <BOOT_OTA_LoadIMG2+0x1c0>)
300061ce:	4798      	blx	r3
300061d0:	2301      	movs	r3, #1
300061d2:	494f      	ldr	r1, [pc, #316]	; (30006310 <BOOT_OTA_LoadIMG2+0x1bc>)
300061d4:	4628      	mov	r0, r5
300061d6:	461a      	mov	r2, r3
300061d8:	f8df 9160 	ldr.w	r9, [pc, #352]	; 3000633c <BOOT_OTA_LoadIMG2+0x1e8>
300061dc:	f7ff ff72 	bl	300060c4 <BOOT_OTFCheck>
300061e0:	47c8      	blx	r9
300061e2:	2800      	cmp	r0, #0
300061e4:	bf0c      	ite	eq
300061e6:	46b3      	moveq	fp, r6
300061e8:	46ab      	movne	fp, r5
300061ea:	ab04      	add	r3, sp, #16
300061ec:	2203      	movs	r2, #3
300061ee:	4659      	mov	r1, fp
300061f0:	4849      	ldr	r0, [pc, #292]	; (30006318 <BOOT_OTA_LoadIMG2+0x1c4>)
300061f2:	f8cd 8000 	str.w	r8, [sp]
300061f6:	f7ff fd5f 	bl	30005cb8 <BOOT_LoadSubImage>
300061fa:	b920      	cbnz	r0, 30006206 <BOOT_OTA_LoadIMG2+0xb2>
300061fc:	27ff      	movs	r7, #255	; 0xff
300061fe:	4638      	mov	r0, r7
30006200:	b00b      	add	sp, #44	; 0x2c
30006202:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006206:	4c44      	ldr	r4, [pc, #272]	; (30006318 <BOOT_OTA_LoadIMG2+0x1c4>)
30006208:	47c8      	blx	r9
3000620a:	2800      	cmp	r0, #0
3000620c:	bf08      	it	eq
3000620e:	4635      	moveq	r5, r6
30006210:	4640      	mov	r0, r8
30006212:	68e2      	ldr	r2, [r4, #12]
30006214:	6863      	ldr	r3, [r4, #4]
30006216:	6025      	str	r5, [r4, #0]
30006218:	4413      	add	r3, r2
3000621a:	6962      	ldr	r2, [r4, #20]
3000621c:	4413      	add	r3, r2
3000621e:	4a3f      	ldr	r2, [pc, #252]	; (3000631c <BOOT_OTA_LoadIMG2+0x1c8>)
30006220:	1e5d      	subs	r5, r3, #1
30006222:	eb0b 0103 	add.w	r1, fp, r3
30006226:	f425 657f 	bic.w	r5, r5, #4080	; 0xff0
3000622a:	61a1      	str	r1, [r4, #24]
3000622c:	f025 050f 	bic.w	r5, r5, #15
30006230:	f505 5580 	add.w	r5, r5, #4096	; 0x1000
30006234:	1aeb      	subs	r3, r5, r3
30006236:	61e3      	str	r3, [r4, #28]
30006238:	19ab      	adds	r3, r5, r6
3000623a:	4e39      	ldr	r6, [pc, #228]	; (30006320 <BOOT_OTA_LoadIMG2+0x1cc>)
3000623c:	4d33      	ldr	r5, [pc, #204]	; (3000630c <BOOT_OTA_LoadIMG2+0x1b8>)
3000623e:	4631      	mov	r1, r6
30006240:	9302      	str	r3, [sp, #8]
30006242:	47a8      	blx	r5
30006244:	4641      	mov	r1, r8
30006246:	4640      	mov	r0, r8
30006248:	4b32      	ldr	r3, [pc, #200]	; (30006314 <BOOT_OTA_LoadIMG2+0x1c0>)
3000624a:	4798      	blx	r3
3000624c:	2302      	movs	r3, #2
3000624e:	4642      	mov	r2, r8
30006250:	4932      	ldr	r1, [pc, #200]	; (3000631c <BOOT_OTA_LoadIMG2+0x1c8>)
30006252:	4630      	mov	r0, r6
30006254:	f7ff ff36 	bl	300060c4 <BOOT_OTFCheck>
30006258:	47c8      	blx	r9
3000625a:	9b02      	ldr	r3, [sp, #8]
3000625c:	2203      	movs	r2, #3
3000625e:	f8cd 8000 	str.w	r8, [sp]
30006262:	2800      	cmp	r0, #0
30006264:	bf18      	it	ne
30006266:	4633      	movne	r3, r6
30006268:	f104 0020 	add.w	r0, r4, #32
3000626c:	469b      	mov	fp, r3
3000626e:	4653      	mov	r3, sl
30006270:	4659      	mov	r1, fp
30006272:	f7ff fd21 	bl	30005cb8 <BOOT_LoadSubImage>
30006276:	2800      	cmp	r0, #0
30006278:	d0c0      	beq.n	300061fc <BOOT_OTA_LoadIMG2+0xa8>
3000627a:	47c8      	blx	r9
3000627c:	9b02      	ldr	r3, [sp, #8]
3000627e:	6ae2      	ldr	r2, [r4, #44]	; 0x2c
30006280:	2800      	cmp	r0, #0
30006282:	bf08      	it	eq
30006284:	461e      	moveq	r6, r3
30006286:	6a63      	ldr	r3, [r4, #36]	; 0x24
30006288:	481f      	ldr	r0, [pc, #124]	; (30006308 <BOOT_OTA_LoadIMG2+0x1b4>)
3000628a:	4413      	add	r3, r2
3000628c:	6b62      	ldr	r2, [r4, #52]	; 0x34
3000628e:	6226      	str	r6, [r4, #32]
30006290:	4413      	add	r3, r2
30006292:	1e5a      	subs	r2, r3, #1
30006294:	eb0b 0103 	add.w	r1, fp, r3
30006298:	f422 627f 	bic.w	r2, r2, #4080	; 0xff0
3000629c:	63a1      	str	r1, [r4, #56]	; 0x38
3000629e:	f022 020f 	bic.w	r2, r2, #15
300062a2:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
300062a6:	1ad1      	subs	r1, r2, r3
300062a8:	2900      	cmp	r1, #0
300062aa:	63e1      	str	r1, [r4, #60]	; 0x3c
300062ac:	4621      	mov	r1, r4
300062ae:	bf18      	it	ne
300062b0:	4613      	movne	r3, r2
300062b2:	9a02      	ldr	r2, [sp, #8]
300062b4:	4413      	add	r3, r2
300062b6:	4a1b      	ldr	r2, [pc, #108]	; (30006324 <BOOT_OTA_LoadIMG2+0x1d0>)
300062b8:	6013      	str	r3, [r2, #0]
300062ba:	bf08      	it	eq
300062bc:	2207      	moveq	r2, #7
300062be:	4b0f      	ldr	r3, [pc, #60]	; (300062fc <BOOT_OTA_LoadIMG2+0x1a8>)
300062c0:	bf18      	it	ne
300062c2:	2208      	movne	r2, #8
300062c4:	9700      	str	r7, [sp, #0]
300062c6:	f000 fb47 	bl	30006958 <BOOT_SignatureCheck>
300062ca:	4b17      	ldr	r3, [pc, #92]	; (30006328 <BOOT_OTA_LoadIMG2+0x1d4>)
300062cc:	4604      	mov	r4, r0
300062ce:	689b      	ldr	r3, [r3, #8]
300062d0:	079b      	lsls	r3, r3, #30
300062d2:	d50a      	bpl.n	300062ea <BOOT_OTA_LoadIMG2+0x196>
300062d4:	4915      	ldr	r1, [pc, #84]	; (3000632c <BOOT_OTA_LoadIMG2+0x1d8>)
300062d6:	4603      	mov	r3, r0
300062d8:	4a15      	ldr	r2, [pc, #84]	; (30006330 <BOOT_OTA_LoadIMG2+0x1dc>)
300062da:	2800      	cmp	r0, #0
300062dc:	bf08      	it	eq
300062de:	460a      	moveq	r2, r1
300062e0:	9903      	ldr	r1, [sp, #12]
300062e2:	4814      	ldr	r0, [pc, #80]	; (30006334 <BOOT_OTA_LoadIMG2+0x1e0>)
300062e4:	3101      	adds	r1, #1
300062e6:	f005 f86f 	bl	3000b3c8 <__DiagPrintf_veneer>
300062ea:	2c00      	cmp	r4, #0
300062ec:	d087      	beq.n	300061fe <BOOT_OTA_LoadIMG2+0xaa>
300062ee:	4812      	ldr	r0, [pc, #72]	; (30006338 <BOOT_OTA_LoadIMG2+0x1e4>)
300062f0:	e750      	b.n	30006194 <BOOT_OTA_LoadIMG2+0x40>
300062f2:	bf00      	nop
300062f4:	3000eb78 	.word	0x3000eb78
300062f8:	3000d134 	.word	0x3000d134
300062fc:	3000ebec 	.word	0x3000ebec
30006300:	3000d040 	.word	0x3000d040
30006304:	0c000000 	.word	0x0c000000
30006308:	3000ed50 	.word	0x3000ed50
3000630c:	30008da1 	.word	0x30008da1
30006310:	0cffffe0 	.word	0x0cffffe0
30006314:	30008ddd 	.word	0x30008ddd
30006318:	3000eef8 	.word	0x3000eef8
3000631c:	0dffffe0 	.word	0x0dffffe0
30006320:	0d000000 	.word	0x0d000000
30006324:	3000dc8c 	.word	0x3000dc8c
30006328:	2001c00c 	.word	0x2001c00c
3000632c:	3000d03a 	.word	0x3000d03a
30006330:	3000d038 	.word	0x3000d038
30006334:	3000d068 	.word	0x3000d068
30006338:	3000d09a 	.word	0x3000d09a
3000633c:	30008ff5 	.word	0x30008ff5

30006340 <BOOT_OTA_Region_Init>:
30006340:	b510      	push	{r4, lr}
30006342:	4c10      	ldr	r4, [pc, #64]	; (30006384 <BOOT_OTA_Region_Init+0x44>)
30006344:	2200      	movs	r2, #0
30006346:	2002      	movs	r0, #2
30006348:	4621      	mov	r1, r4
3000634a:	f001 fbb1 	bl	30007ab0 <flash_get_layout_info>
3000634e:	2200      	movs	r2, #0
30006350:	1d21      	adds	r1, r4, #4
30006352:	2003      	movs	r0, #3
30006354:	f001 fbac 	bl	30007ab0 <flash_get_layout_info>
30006358:	2200      	movs	r2, #0
3000635a:	f104 0110 	add.w	r1, r4, #16
3000635e:	2004      	movs	r0, #4
30006360:	f001 fba6 	bl	30007ab0 <flash_get_layout_info>
30006364:	2200      	movs	r2, #0
30006366:	f104 0114 	add.w	r1, r4, #20
3000636a:	2005      	movs	r0, #5
3000636c:	f001 fba0 	bl	30007ab0 <flash_get_layout_info>
30006370:	6823      	ldr	r3, [r4, #0]
30006372:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006376:	60a3      	str	r3, [r4, #8]
30006378:	6863      	ldr	r3, [r4, #4]
3000637a:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
3000637e:	60e3      	str	r3, [r4, #12]
30006380:	bd10      	pop	{r4, pc}
30006382:	bf00      	nop
30006384:	3000eee0 	.word	0x3000eee0

30006388 <BOOT_OTA_IMG2>:
30006388:	f7ff bee4 	b.w	30006154 <BOOT_OTA_LoadIMG2>

3000638c <BOOT_OTA_AP>:
3000638c:	b5f0      	push	{r4, r5, r6, r7, lr}
3000638e:	4b2c      	ldr	r3, [pc, #176]	; (30006440 <BOOT_OTA_AP+0xb4>)
30006390:	b0eb      	sub	sp, #428	; 0x1ac
30006392:	4f2c      	ldr	r7, [pc, #176]	; (30006444 <BOOT_OTA_AP+0xb8>)
30006394:	ac02      	add	r4, sp, #8
30006396:	4d2c      	ldr	r5, [pc, #176]	; (30006448 <BOOT_OTA_AP+0xbc>)
30006398:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000639a:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
3000639e:	4b2b      	ldr	r3, [pc, #172]	; (3000644c <BOOT_OTA_AP+0xc0>)
300063a0:	f44f 72c8 	mov.w	r2, #400	; 0x190
300063a4:	a806      	add	r0, sp, #24
300063a6:	681e      	ldr	r6, [r3, #0]
300063a8:	4631      	mov	r1, r6
300063aa:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
300063ae:	f7fe f919 	bl	300045e4 <BOOT_ImgCopy>
300063b2:	4a27      	ldr	r2, [pc, #156]	; (30006450 <BOOT_OTA_AP+0xc4>)
300063b4:	4639      	mov	r1, r7
300063b6:	4633      	mov	r3, r6
300063b8:	2003      	movs	r0, #3
300063ba:	47a8      	blx	r5
300063bc:	2101      	movs	r1, #1
300063be:	2003      	movs	r0, #3
300063c0:	4b24      	ldr	r3, [pc, #144]	; (30006454 <BOOT_OTA_AP+0xc8>)
300063c2:	4798      	blx	r3
300063c4:	2201      	movs	r2, #1
300063c6:	4922      	ldr	r1, [pc, #136]	; (30006450 <BOOT_OTA_AP+0xc4>)
300063c8:	4638      	mov	r0, r7
300063ca:	2303      	movs	r3, #3
300063cc:	4d22      	ldr	r5, [pc, #136]	; (30006458 <BOOT_OTA_AP+0xcc>)
300063ce:	f7ff fe79 	bl	300060c4 <BOOT_OTFCheck>
300063d2:	4b22      	ldr	r3, [pc, #136]	; (3000645c <BOOT_OTA_AP+0xd0>)
300063d4:	4798      	blx	r3
300063d6:	2301      	movs	r3, #1
300063d8:	2800      	cmp	r0, #0
300063da:	bf0c      	ite	eq
300063dc:	4631      	moveq	r1, r6
300063de:	4639      	movne	r1, r7
300063e0:	2204      	movs	r2, #4
300063e2:	9300      	str	r3, [sp, #0]
300063e4:	4628      	mov	r0, r5
300063e6:	4623      	mov	r3, r4
300063e8:	f7ff fc66 	bl	30005cb8 <BOOT_LoadSubImage>
300063ec:	2801      	cmp	r0, #1
300063ee:	4607      	mov	r7, r0
300063f0:	d123      	bne.n	3000643a <BOOT_OTA_AP+0xae>
300063f2:	2302      	movs	r3, #2
300063f4:	2204      	movs	r2, #4
300063f6:	4629      	mov	r1, r5
300063f8:	a806      	add	r0, sp, #24
300063fa:	9300      	str	r3, [sp, #0]
300063fc:	4b18      	ldr	r3, [pc, #96]	; (30006460 <BOOT_OTA_AP+0xd4>)
300063fe:	f000 faab 	bl	30006958 <BOOT_SignatureCheck>
30006402:	4604      	mov	r4, r0
30006404:	b990      	cbnz	r0, 3000642c <BOOT_OTA_AP+0xa0>
30006406:	68eb      	ldr	r3, [r5, #12]
30006408:	463a      	mov	r2, r7
3000640a:	6869      	ldr	r1, [r5, #4]
3000640c:	f105 0018 	add.w	r0, r5, #24
30006410:	4419      	add	r1, r3
30006412:	696b      	ldr	r3, [r5, #20]
30006414:	4419      	add	r1, r3
30006416:	4431      	add	r1, r6
30006418:	f7ff fcea 	bl	30005df0 <BOOT_LoadAPSubImage>
3000641c:	2801      	cmp	r0, #1
3000641e:	d009      	beq.n	30006434 <BOOT_OTA_AP+0xa8>
30006420:	4b10      	ldr	r3, [pc, #64]	; (30006464 <BOOT_OTA_AP+0xd8>)
30006422:	681b      	ldr	r3, [r3, #0]
30006424:	079b      	lsls	r3, r3, #30
30006426:	d504      	bpl.n	30006432 <BOOT_OTA_AP+0xa6>
30006428:	480f      	ldr	r0, [pc, #60]	; (30006468 <BOOT_OTA_AP+0xdc>)
3000642a:	e000      	b.n	3000642e <BOOT_OTA_AP+0xa2>
3000642c:	480f      	ldr	r0, [pc, #60]	; (3000646c <BOOT_OTA_AP+0xe0>)
3000642e:	f004 ffcb 	bl	3000b3c8 <__DiagPrintf_veneer>
30006432:	24ff      	movs	r4, #255	; 0xff
30006434:	4620      	mov	r0, r4
30006436:	b06b      	add	sp, #428	; 0x1ac
30006438:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000643a:	480d      	ldr	r0, [pc, #52]	; (30006470 <BOOT_OTA_AP+0xe4>)
3000643c:	e7f7      	b.n	3000642e <BOOT_OTA_AP+0xa2>
3000643e:	bf00      	nop
30006440:	3000d14c 	.word	0x3000d14c
30006444:	0e000000 	.word	0x0e000000
30006448:	30008da1 	.word	0x30008da1
3000644c:	3000dc8c 	.word	0x3000dc8c
30006450:	0fffffe0 	.word	0x0fffffe0
30006454:	30008ddd 	.word	0x30008ddd
30006458:	3000eef8 	.word	0x3000eef8
3000645c:	30008ff5 	.word	0x30008ff5
30006460:	3000ebec 	.word	0x3000ebec
30006464:	2001c00c 	.word	0x2001c00c
30006468:	3000d101 	.word	0x3000d101
3000646c:	3000d0bb 	.word	0x3000d0bb
30006470:	3000d0de 	.word	0x3000d0de

30006474 <BOOT_LoadRDPImg>:
30006474:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006478:	4b4f      	ldr	r3, [pc, #316]	; (300065b8 <BOOT_LoadRDPImg+0x144>)
3000647a:	4615      	mov	r5, r2
3000647c:	4606      	mov	r6, r0
3000647e:	468a      	mov	sl, r1
30006480:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006484:	2400      	movs	r4, #0
30006486:	ed2d 8b02 	vpush	{d8}
3000648a:	b0d7      	sub	sp, #348	; 0x15c
3000648c:	e893 0003 	ldmia.w	r3, {r0, r1}
30006490:	f10d 0910 	add.w	r9, sp, #16
30006494:	e889 0003 	stmia.w	r9, {r0, r1}
30006498:	4629      	mov	r1, r5
3000649a:	4630      	mov	r0, r6
3000649c:	f7fe f8a2 	bl	300045e4 <BOOT_ImgCopy>
300064a0:	f505 5380 	add.w	r3, r5, #4096	; 0x1000
300064a4:	2202      	movs	r2, #2
300064a6:	4650      	mov	r0, sl
300064a8:	ee08 3a10 	vmov	s16, r3
300064ac:	9400      	str	r4, [sp, #0]
300064ae:	464b      	mov	r3, r9
300064b0:	ee18 1a10 	vmov	r1, s16
300064b4:	f7ff fc00 	bl	30005cb8 <BOOT_LoadSubImage>
300064b8:	2801      	cmp	r0, #1
300064ba:	d057      	beq.n	3000656c <BOOT_LoadRDPImg+0xf8>
300064bc:	4b3f      	ldr	r3, [pc, #252]	; (300065bc <BOOT_LoadRDPImg+0x148>)
300064be:	4620      	mov	r0, r4
300064c0:	4798      	blx	r3
300064c2:	2101      	movs	r1, #1
300064c4:	4b3e      	ldr	r3, [pc, #248]	; (300065c0 <BOOT_LoadRDPImg+0x14c>)
300064c6:	4620      	mov	r0, r4
300064c8:	4798      	blx	r3
300064ca:	f8df b114 	ldr.w	fp, [pc, #276]	; 300065e0 <BOOT_LoadRDPImg+0x16c>
300064ce:	2120      	movs	r1, #32
300064d0:	4b3c      	ldr	r3, [pc, #240]	; (300065c4 <BOOT_LoadRDPImg+0x150>)
300064d2:	a80e      	add	r0, sp, #56	; 0x38
300064d4:	4798      	blx	r3
300064d6:	2210      	movs	r2, #16
300064d8:	f106 0120 	add.w	r1, r6, #32
300064dc:	a80a      	add	r0, sp, #40	; 0x28
300064de:	4b3a      	ldr	r3, [pc, #232]	; (300065c8 <BOOT_LoadRDPImg+0x154>)
300064e0:	4798      	blx	r3
300064e2:	2210      	movs	r2, #16
300064e4:	ee18 1a10 	vmov	r1, s16
300064e8:	a816      	add	r0, sp, #88	; 0x58
300064ea:	ae06      	add	r6, sp, #24
300064ec:	f7fe f87a 	bl	300045e4 <BOOT_ImgCopy>
300064f0:	2310      	movs	r3, #16
300064f2:	aa0a      	add	r2, sp, #40	; 0x28
300064f4:	a816      	add	r0, sp, #88	; 0x58
300064f6:	4619      	mov	r1, r3
300064f8:	4f34      	ldr	r7, [pc, #208]	; (300065cc <BOOT_LoadRDPImg+0x158>)
300064fa:	9600      	str	r6, [sp, #0]
300064fc:	47b8      	blx	r7
300064fe:	6833      	ldr	r3, [r6, #0]
30006500:	455b      	cmp	r3, fp
30006502:	d105      	bne.n	30006510 <BOOT_LoadRDPImg+0x9c>
30006504:	6872      	ldr	r2, [r6, #4]
30006506:	4932      	ldr	r1, [pc, #200]	; (300065d0 <BOOT_LoadRDPImg+0x15c>)
30006508:	f859 3024 	ldr.w	r3, [r9, r4, lsl #2]
3000650c:	428a      	cmp	r2, r1
3000650e:	d010      	beq.n	30006532 <BOOT_LoadRDPImg+0xbe>
30006510:	f859 3024 	ldr.w	r3, [r9, r4, lsl #2]
30006514:	2004      	movs	r0, #4
30006516:	4a2f      	ldr	r2, [pc, #188]	; (300065d4 <BOOT_LoadRDPImg+0x160>)
30006518:	9300      	str	r3, [sp, #0]
3000651a:	4b2f      	ldr	r3, [pc, #188]	; (300065d8 <BOOT_LoadRDPImg+0x164>)
3000651c:	4619      	mov	r1, r3
3000651e:	f004 fcfd 	bl	3000af1c <rtk_log_write>
30006522:	2000      	movs	r0, #0
30006524:	b057      	add	sp, #348	; 0x15c
30006526:	ecbd 8b02 	vpop	{d8}
3000652a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000652e:	2401      	movs	r4, #1
30006530:	e7d7      	b.n	300064e2 <BOOT_LoadRDPImg+0x6e>
30006532:	68f7      	ldr	r7, [r6, #12]
30006534:	2004      	movs	r0, #4
30006536:	68b6      	ldr	r6, [r6, #8]
30006538:	3f20      	subs	r7, #32
3000653a:	9300      	str	r3, [sp, #0]
3000653c:	3620      	adds	r6, #32
3000653e:	4b26      	ldr	r3, [pc, #152]	; (300065d8 <BOOT_LoadRDPImg+0x164>)
30006540:	4a26      	ldr	r2, [pc, #152]	; (300065dc <BOOT_LoadRDPImg+0x168>)
30006542:	4619      	mov	r1, r3
30006544:	e9cd 7601 	strd	r7, r6, [sp, #4]
30006548:	f004 fce8 	bl	3000af1c <rtk_log_write>
3000654c:	f1ba 0f00 	cmp.w	sl, #0
30006550:	d005      	beq.n	3000655e <BOOT_LoadRDPImg+0xea>
30006552:	f10a 0304 	add.w	r3, sl, #4
30006556:	f84a 7034 	str.w	r7, [sl, r4, lsl #3]
3000655a:	f843 6034 	str.w	r6, [r3, r4, lsl #3]
3000655e:	b93e      	cbnz	r6, 30006570 <BOOT_LoadRDPImg+0xfc>
30006560:	2c00      	cmp	r4, #0
30006562:	d0e4      	beq.n	3000652e <BOOT_LoadRDPImg+0xba>
30006564:	4631      	mov	r1, r6
30006566:	4630      	mov	r0, r6
30006568:	4b15      	ldr	r3, [pc, #84]	; (300065c0 <BOOT_LoadRDPImg+0x14c>)
3000656a:	4798      	blx	r3
3000656c:	2002      	movs	r0, #2
3000656e:	e7d9      	b.n	30006524 <BOOT_LoadRDPImg+0xb0>
30006570:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30006574:	46b0      	mov	r8, r6
30006576:	ee18 1a10 	vmov	r1, s16
3000657a:	a816      	add	r0, sp, #88	; 0x58
3000657c:	bf28      	it	cs
3000657e:	f44f 7880 	movcs.w	r8, #256	; 0x100
30006582:	4d12      	ldr	r5, [pc, #72]	; (300065cc <BOOT_LoadRDPImg+0x158>)
30006584:	4642      	mov	r2, r8
30006586:	eba6 0608 	sub.w	r6, r6, r8
3000658a:	f7fe f82b 	bl	300045e4 <BOOT_ImgCopy>
3000658e:	2310      	movs	r3, #16
30006590:	aa0a      	add	r2, sp, #40	; 0x28
30006592:	4641      	mov	r1, r8
30006594:	9700      	str	r7, [sp, #0]
30006596:	a816      	add	r0, sp, #88	; 0x58
30006598:	47a8      	blx	r5
3000659a:	ab16      	add	r3, sp, #88	; 0x58
3000659c:	f1a8 0110 	sub.w	r1, r8, #16
300065a0:	2210      	movs	r2, #16
300065a2:	a80a      	add	r0, sp, #40	; 0x28
300065a4:	4447      	add	r7, r8
300065a6:	4419      	add	r1, r3
300065a8:	4b07      	ldr	r3, [pc, #28]	; (300065c8 <BOOT_LoadRDPImg+0x154>)
300065aa:	4798      	blx	r3
300065ac:	ee18 3a10 	vmov	r3, s16
300065b0:	4443      	add	r3, r8
300065b2:	ee08 3a10 	vmov	s16, r3
300065b6:	e7d2      	b.n	3000655e <BOOT_LoadRDPImg+0xea>
300065b8:	3000d23c 	.word	0x3000d23c
300065bc:	00002429 	.word	0x00002429
300065c0:	00002479 	.word	0x00002479
300065c4:	00004641 	.word	0x00004641
300065c8:	00012a1d 	.word	0x00012a1d
300065cc:	000046f5 	.word	0x000046f5
300065d0:	31313738 	.word	0x31313738
300065d4:	3000cfa6 	.word	0x3000cfa6
300065d8:	3000c714 	.word	0x3000c714
300065dc:	3000cfb9 	.word	0x3000cfb9
300065e0:	35393138 	.word	0x35393138

300065e4 <BOOT_LoadBL32RDPImg>:
300065e4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300065e8:	4b34      	ldr	r3, [pc, #208]	; (300066bc <BOOT_LoadBL32RDPImg+0xd8>)
300065ea:	b0d0      	sub	sp, #320	; 0x140
300065ec:	4607      	mov	r7, r0
300065ee:	460d      	mov	r5, r1
300065f0:	4614      	mov	r4, r2
300065f2:	ae04      	add	r6, sp, #16
300065f4:	f103 0c10 	add.w	ip, r3, #16
300065f8:	6818      	ldr	r0, [r3, #0]
300065fa:	3308      	adds	r3, #8
300065fc:	f853 1c04 	ldr.w	r1, [r3, #-4]
30006600:	4632      	mov	r2, r6
30006602:	4563      	cmp	r3, ip
30006604:	c203      	stmia	r2!, {r0, r1}
30006606:	4616      	mov	r6, r2
30006608:	d1f6      	bne.n	300065f8 <BOOT_LoadBL32RDPImg+0x14>
3000660a:	4a2d      	ldr	r2, [pc, #180]	; (300066c0 <BOOT_LoadBL32RDPImg+0xdc>)
3000660c:	ab02      	add	r3, sp, #8
3000660e:	4e2d      	ldr	r6, [pc, #180]	; (300066c4 <BOOT_LoadBL32RDPImg+0xe0>)
30006610:	6810      	ldr	r0, [r2, #0]
30006612:	6851      	ldr	r1, [r2, #4]
30006614:	46b0      	mov	r8, r6
30006616:	c303      	stmia	r3!, {r0, r1}
30006618:	4b2b      	ldr	r3, [pc, #172]	; (300066c8 <BOOT_LoadBL32RDPImg+0xe4>)
3000661a:	2000      	movs	r0, #0
3000661c:	4798      	blx	r3
3000661e:	2101      	movs	r1, #1
30006620:	2000      	movs	r0, #0
30006622:	47b0      	blx	r6
30006624:	2120      	movs	r1, #32
30006626:	4b29      	ldr	r3, [pc, #164]	; (300066cc <BOOT_LoadBL32RDPImg+0xe8>)
30006628:	eb0d 0001 	add.w	r0, sp, r1
3000662c:	4798      	blx	r3
3000662e:	4b28      	ldr	r3, [pc, #160]	; (300066d0 <BOOT_LoadBL32RDPImg+0xec>)
30006630:	689b      	ldr	r3, [r3, #8]
30006632:	f013 0f02 	tst.w	r3, #2
30006636:	d005      	beq.n	30006644 <BOOT_LoadBL32RDPImg+0x60>
30006638:	4623      	mov	r3, r4
3000663a:	462a      	mov	r2, r5
3000663c:	a902      	add	r1, sp, #8
3000663e:	4825      	ldr	r0, [pc, #148]	; (300066d4 <BOOT_LoadBL32RDPImg+0xf0>)
30006640:	f004 fec2 	bl	3000b3c8 <__DiagPrintf_veneer>
30006644:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 300066e8 <BOOT_LoadBL32RDPImg+0x104>
30006648:	f8df a0a0 	ldr.w	sl, [pc, #160]	; 300066ec <BOOT_LoadBL32RDPImg+0x108>
3000664c:	b984      	cbnz	r4, 30006670 <BOOT_LoadBL32RDPImg+0x8c>
3000664e:	f855 1c20 	ldr.w	r1, [r5, #-32]
30006652:	4a21      	ldr	r2, [pc, #132]	; (300066d8 <BOOT_LoadBL32RDPImg+0xf4>)
30006654:	4291      	cmp	r1, r2
30006656:	d104      	bne.n	30006662 <BOOT_LoadBL32RDPImg+0x7e>
30006658:	f855 2c1c 	ldr.w	r2, [r5, #-28]
3000665c:	4b1f      	ldr	r3, [pc, #124]	; (300066dc <BOOT_LoadBL32RDPImg+0xf8>)
3000665e:	429a      	cmp	r2, r3
30006660:	d024      	beq.n	300066ac <BOOT_LoadBL32RDPImg+0xc8>
30006662:	481f      	ldr	r0, [pc, #124]	; (300066e0 <BOOT_LoadBL32RDPImg+0xfc>)
30006664:	f004 feb0 	bl	3000b3c8 <__DiagPrintf_veneer>
30006668:	2000      	movs	r0, #0
3000666a:	b050      	add	sp, #320	; 0x140
3000666c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30006670:	f5b4 7f80 	cmp.w	r4, #256	; 0x100
30006674:	4626      	mov	r6, r4
30006676:	4639      	mov	r1, r7
30006678:	a810      	add	r0, sp, #64	; 0x40
3000667a:	bf28      	it	cs
3000667c:	f44f 7680 	movcs.w	r6, #256	; 0x100
30006680:	4632      	mov	r2, r6
30006682:	1ba4      	subs	r4, r4, r6
30006684:	f7fd ffae 	bl	300045e4 <BOOT_ImgCopy>
30006688:	2310      	movs	r3, #16
3000668a:	4631      	mov	r1, r6
3000668c:	9500      	str	r5, [sp, #0]
3000668e:	eb0d 0203 	add.w	r2, sp, r3
30006692:	a810      	add	r0, sp, #64	; 0x40
30006694:	47c8      	blx	r9
30006696:	2210      	movs	r2, #16
30006698:	ab10      	add	r3, sp, #64	; 0x40
3000669a:	f1a6 0110 	sub.w	r1, r6, #16
3000669e:	eb0d 0002 	add.w	r0, sp, r2
300066a2:	4435      	add	r5, r6
300066a4:	4419      	add	r1, r3
300066a6:	4437      	add	r7, r6
300066a8:	47d0      	blx	sl
300066aa:	e7cf      	b.n	3000664c <BOOT_LoadBL32RDPImg+0x68>
300066ac:	480d      	ldr	r0, [pc, #52]	; (300066e4 <BOOT_LoadBL32RDPImg+0x100>)
300066ae:	f004 fe8b 	bl	3000b3c8 <__DiagPrintf_veneer>
300066b2:	4621      	mov	r1, r4
300066b4:	4620      	mov	r0, r4
300066b6:	47c0      	blx	r8
300066b8:	2001      	movs	r0, #1
300066ba:	e7d6      	b.n	3000666a <BOOT_LoadBL32RDPImg+0x86>
300066bc:	3000d1bf 	.word	0x3000d1bf
300066c0:	3000d1d0 	.word	0x3000d1d0
300066c4:	00002479 	.word	0x00002479
300066c8:	00002429 	.word	0x00002429
300066cc:	00004641 	.word	0x00004641
300066d0:	2001c00c 	.word	0x2001c00c
300066d4:	3000d182 	.word	0x3000d182
300066d8:	35393138 	.word	0x35393138
300066dc:	31313738 	.word	0x31313738
300066e0:	3000d1a8 	.word	0x3000d1a8
300066e4:	3000d1b2 	.word	0x3000d1b2
300066e8:	000046f5 	.word	0x000046f5
300066ec:	00012a1d 	.word	0x00012a1d

300066f0 <_BOOT_Validate_ImgHash>:
300066f0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300066f4:	f5ad 7d4d 	sub.w	sp, sp, #820	; 0x334
300066f8:	4605      	mov	r5, r0
300066fa:	4614      	mov	r4, r2
300066fc:	22e0      	movs	r2, #224	; 0xe0
300066fe:	9101      	str	r1, [sp, #4]
30006700:	2100      	movs	r1, #0
30006702:	a814      	add	r0, sp, #80	; 0x50
30006704:	469b      	mov	fp, r3
30006706:	f88d 100f 	strb.w	r1, [sp, #15]
3000670a:	f004 fe59 	bl	3000b3c0 <____wrap_memset_veneer>
3000670e:	4628      	mov	r0, r5
30006710:	f10d 020f 	add.w	r2, sp, #15
30006714:	f10d 010e 	add.w	r1, sp, #14
30006718:	f002 fca2 	bl	30009060 <SBOOT_GetMdType>
3000671c:	4605      	mov	r5, r0
3000671e:	b988      	cbnz	r0, 30006744 <_BOOT_Validate_ImgHash+0x54>
30006720:	f89d 300f 	ldrb.w	r3, [sp, #15]
30006724:	2b00      	cmp	r3, #0
30006726:	d07f      	beq.n	30006828 <_BOOT_Validate_ImgHash+0x138>
30006728:	2101      	movs	r1, #1
3000672a:	4b75      	ldr	r3, [pc, #468]	; (30006900 <_BOOT_Validate_ImgHash+0x210>)
3000672c:	4e75      	ldr	r6, [pc, #468]	; (30006904 <_BOOT_Validate_ImgHash+0x214>)
3000672e:	4608      	mov	r0, r1
30006730:	4798      	blx	r3
30006732:	ab14      	add	r3, sp, #80	; 0x50
30006734:	2220      	movs	r2, #32
30006736:	4629      	mov	r1, r5
30006738:	f89d 000e 	ldrb.w	r0, [sp, #14]
3000673c:	47b0      	blx	r6
3000673e:	b130      	cbz	r0, 3000674e <_BOOT_Validate_ImgHash+0x5e>
30006740:	f06f 050a 	mvn.w	r5, #10
30006744:	4628      	mov	r0, r5
30006746:	f50d 7d4d 	add.w	sp, sp, #820	; 0x334
3000674a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000674e:	ae4c      	add	r6, sp, #304	; 0x130
30006750:	aacc      	add	r2, sp, #816	; 0x330
30006752:	eb04 0bcb 	add.w	fp, r4, fp, lsl #3
30006756:	f8df 91c8 	ldr.w	r9, [pc, #456]	; 30006920 <_BOOT_Validate_ImgHash+0x230>
3000675a:	f012 0218 	ands.w	r2, r2, #24
3000675e:	f026 031f 	bic.w	r3, r6, #31
30006762:	bf18      	it	ne
30006764:	461e      	movne	r6, r3
30006766:	9200      	str	r2, [sp, #0]
30006768:	455c      	cmp	r4, fp
3000676a:	d11e      	bne.n	300067aa <_BOOT_Validate_ImgHash+0xba>
3000676c:	a914      	add	r1, sp, #80	; 0x50
3000676e:	a804      	add	r0, sp, #16
30006770:	4b65      	ldr	r3, [pc, #404]	; (30006908 <_BOOT_Validate_ImgHash+0x218>)
30006772:	4798      	blx	r3
30006774:	4601      	mov	r1, r0
30006776:	2800      	cmp	r0, #0
30006778:	d1e2      	bne.n	30006740 <_BOOT_Validate_ImgHash+0x50>
3000677a:	2001      	movs	r0, #1
3000677c:	4b60      	ldr	r3, [pc, #384]	; (30006900 <_BOOT_Validate_ImgHash+0x210>)
3000677e:	4798      	blx	r3
30006780:	f89d 400e 	ldrb.w	r4, [sp, #14]
30006784:	ae04      	add	r6, sp, #16
30006786:	4b61      	ldr	r3, [pc, #388]	; (3000690c <_BOOT_Validate_ImgHash+0x21c>)
30006788:	4798      	blx	r3
3000678a:	fbb0 f3f4 	udiv	r3, r0, r4
3000678e:	fb03 0014 	mls	r0, r3, r4, r0
30006792:	2300      	movs	r3, #0
30006794:	b2c0      	uxtb	r0, r0
30006796:	461a      	mov	r2, r3
30006798:	b2d9      	uxtb	r1, r3
3000679a:	428c      	cmp	r4, r1
3000679c:	f200 80a1 	bhi.w	300068e2 <_BOOT_Validate_ImgHash+0x1f2>
300067a0:	2a00      	cmp	r2, #0
300067a2:	bf18      	it	ne
300067a4:	f06f 0506 	mvnne.w	r5, #6
300067a8:	e7cc      	b.n	30006744 <_BOOT_Validate_ImgHash+0x54>
300067aa:	e9d4 7800 	ldrd	r7, r8, [r4]
300067ae:	f107 4378 	add.w	r3, r7, #4160749568	; 0xf8000000
300067b2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300067b6:	d22f      	bcs.n	30006818 <_BOOT_Validate_ImgHash+0x128>
300067b8:	f1b8 0f00 	cmp.w	r8, #0
300067bc:	d101      	bne.n	300067c2 <_BOOT_Validate_ImgHash+0xd2>
300067be:	3408      	adds	r4, #8
300067c0:	e7d2      	b.n	30006768 <_BOOT_Validate_ImgHash+0x78>
300067c2:	f5b8 7f00 	cmp.w	r8, #512	; 0x200
300067c6:	46c2      	mov	sl, r8
300067c8:	4639      	mov	r1, r7
300067ca:	a84c      	add	r0, sp, #304	; 0x130
300067cc:	bf28      	it	cs
300067ce:	f44f 7a00 	movcs.w	sl, #512	; 0x200
300067d2:	4652      	mov	r2, sl
300067d4:	f7fd ff06 	bl	300045e4 <BOOT_ImgCopy>
300067d8:	f8d9 2014 	ldr.w	r2, [r9, #20]
300067dc:	03d2      	lsls	r2, r2, #15
300067de:	d510      	bpl.n	30006802 <_BOOT_Validate_ImgHash+0x112>
300067e0:	9b00      	ldr	r3, [sp, #0]
300067e2:	eb0a 0203 	add.w	r2, sl, r3
300067e6:	f3bf 8f4f 	dsb	sy
300067ea:	4631      	mov	r1, r6
300067ec:	4432      	add	r2, r6
300067ee:	f8c9 1270 	str.w	r1, [r9, #624]	; 0x270
300067f2:	3120      	adds	r1, #32
300067f4:	1a50      	subs	r0, r2, r1
300067f6:	2800      	cmp	r0, #0
300067f8:	dcf9      	bgt.n	300067ee <_BOOT_Validate_ImgHash+0xfe>
300067fa:	f3bf 8f4f 	dsb	sy
300067fe:	f3bf 8f6f 	isb	sy
30006802:	aa14      	add	r2, sp, #80	; 0x50
30006804:	4651      	mov	r1, sl
30006806:	a84c      	add	r0, sp, #304	; 0x130
30006808:	4b41      	ldr	r3, [pc, #260]	; (30006910 <_BOOT_Validate_ImgHash+0x220>)
3000680a:	4798      	blx	r3
3000680c:	2800      	cmp	r0, #0
3000680e:	d197      	bne.n	30006740 <_BOOT_Validate_ImgHash+0x50>
30006810:	eba8 080a 	sub.w	r8, r8, sl
30006814:	4457      	add	r7, sl
30006816:	e7cf      	b.n	300067b8 <_BOOT_Validate_ImgHash+0xc8>
30006818:	aa14      	add	r2, sp, #80	; 0x50
3000681a:	4641      	mov	r1, r8
3000681c:	4638      	mov	r0, r7
3000681e:	4b3c      	ldr	r3, [pc, #240]	; (30006910 <_BOOT_Validate_ImgHash+0x220>)
30006820:	4798      	blx	r3
30006822:	2800      	cmp	r0, #0
30006824:	d0cb      	beq.n	300067be <_BOOT_Validate_ImgHash+0xce>
30006826:	e78b      	b.n	30006740 <_BOOT_Validate_ImgHash+0x50>
30006828:	a914      	add	r1, sp, #80	; 0x50
3000682a:	f89d 000e 	ldrb.w	r0, [sp, #14]
3000682e:	4b39      	ldr	r3, [pc, #228]	; (30006914 <_BOOT_Validate_ImgHash+0x224>)
30006830:	4798      	blx	r3
30006832:	2800      	cmp	r0, #0
30006834:	d184      	bne.n	30006740 <_BOOT_Validate_ImgHash+0x50>
30006836:	ae4c      	add	r6, sp, #304	; 0x130
30006838:	aacc      	add	r2, sp, #816	; 0x330
3000683a:	eb04 0bcb 	add.w	fp, r4, fp, lsl #3
3000683e:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30006920 <_BOOT_Validate_ImgHash+0x230>
30006842:	f012 0218 	ands.w	r2, r2, #24
30006846:	f026 031f 	bic.w	r3, r6, #31
3000684a:	bf18      	it	ne
3000684c:	461e      	movne	r6, r3
3000684e:	9200      	str	r2, [sp, #0]
30006850:	455c      	cmp	r4, fp
30006852:	d106      	bne.n	30006862 <_BOOT_Validate_ImgHash+0x172>
30006854:	a914      	add	r1, sp, #80	; 0x50
30006856:	a804      	add	r0, sp, #16
30006858:	4b2f      	ldr	r3, [pc, #188]	; (30006918 <_BOOT_Validate_ImgHash+0x228>)
3000685a:	4798      	blx	r3
3000685c:	2800      	cmp	r0, #0
3000685e:	d08f      	beq.n	30006780 <_BOOT_Validate_ImgHash+0x90>
30006860:	e76e      	b.n	30006740 <_BOOT_Validate_ImgHash+0x50>
30006862:	e9d4 7800 	ldrd	r7, r8, [r4]
30006866:	f107 4378 	add.w	r3, r7, #4160749568	; 0xf8000000
3000686a:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000686e:	d230      	bcs.n	300068d2 <_BOOT_Validate_ImgHash+0x1e2>
30006870:	f1b8 0f00 	cmp.w	r8, #0
30006874:	d101      	bne.n	3000687a <_BOOT_Validate_ImgHash+0x18a>
30006876:	3408      	adds	r4, #8
30006878:	e7ea      	b.n	30006850 <_BOOT_Validate_ImgHash+0x160>
3000687a:	f5b8 7f00 	cmp.w	r8, #512	; 0x200
3000687e:	46c2      	mov	sl, r8
30006880:	4639      	mov	r1, r7
30006882:	a84c      	add	r0, sp, #304	; 0x130
30006884:	bf28      	it	cs
30006886:	f44f 7a00 	movcs.w	sl, #512	; 0x200
3000688a:	4652      	mov	r2, sl
3000688c:	f7fd feaa 	bl	300045e4 <BOOT_ImgCopy>
30006890:	f8d9 2014 	ldr.w	r2, [r9, #20]
30006894:	03d3      	lsls	r3, r2, #15
30006896:	d510      	bpl.n	300068ba <_BOOT_Validate_ImgHash+0x1ca>
30006898:	9b00      	ldr	r3, [sp, #0]
3000689a:	eb0a 0203 	add.w	r2, sl, r3
3000689e:	f3bf 8f4f 	dsb	sy
300068a2:	4631      	mov	r1, r6
300068a4:	4432      	add	r2, r6
300068a6:	f8c9 1270 	str.w	r1, [r9, #624]	; 0x270
300068aa:	3120      	adds	r1, #32
300068ac:	1a50      	subs	r0, r2, r1
300068ae:	2800      	cmp	r0, #0
300068b0:	dcf9      	bgt.n	300068a6 <_BOOT_Validate_ImgHash+0x1b6>
300068b2:	f3bf 8f4f 	dsb	sy
300068b6:	f3bf 8f6f 	isb	sy
300068ba:	aa14      	add	r2, sp, #80	; 0x50
300068bc:	4651      	mov	r1, sl
300068be:	a84c      	add	r0, sp, #304	; 0x130
300068c0:	4b16      	ldr	r3, [pc, #88]	; (3000691c <_BOOT_Validate_ImgHash+0x22c>)
300068c2:	4798      	blx	r3
300068c4:	2800      	cmp	r0, #0
300068c6:	f47f af3b 	bne.w	30006740 <_BOOT_Validate_ImgHash+0x50>
300068ca:	eba8 080a 	sub.w	r8, r8, sl
300068ce:	4457      	add	r7, sl
300068d0:	e7ce      	b.n	30006870 <_BOOT_Validate_ImgHash+0x180>
300068d2:	aa14      	add	r2, sp, #80	; 0x50
300068d4:	4641      	mov	r1, r8
300068d6:	4638      	mov	r0, r7
300068d8:	4b10      	ldr	r3, [pc, #64]	; (3000691c <_BOOT_Validate_ImgHash+0x22c>)
300068da:	4798      	blx	r3
300068dc:	2800      	cmp	r0, #0
300068de:	d0ca      	beq.n	30006876 <_BOOT_Validate_ImgHash+0x186>
300068e0:	e72e      	b.n	30006740 <_BOOT_Validate_ImgHash+0x50>
300068e2:	9901      	ldr	r1, [sp, #4]
300068e4:	3301      	adds	r3, #1
300068e6:	5c37      	ldrb	r7, [r6, r0]
300068e8:	5c09      	ldrb	r1, [r1, r0]
300068ea:	3001      	adds	r0, #1
300068ec:	4079      	eors	r1, r7
300068ee:	b2c0      	uxtb	r0, r0
300068f0:	430a      	orrs	r2, r1
300068f2:	fbb0 f1f4 	udiv	r1, r0, r4
300068f6:	fb04 0011 	mls	r0, r4, r1, r0
300068fa:	b2c0      	uxtb	r0, r0
300068fc:	e74c      	b.n	30006798 <_BOOT_Validate_ImgHash+0xa8>
300068fe:	bf00      	nop
30006900:	30009099 	.word	0x30009099
30006904:	300089c5 	.word	0x300089c5
30006908:	30009105 	.word	0x30009105
3000690c:	0000ea4d 	.word	0x0000ea4d
30006910:	300090cd 	.word	0x300090cd
30006914:	30009139 	.word	0x30009139
30006918:	300091a5 	.word	0x300091a5
3000691c:	3000916d 	.word	0x3000916d
30006920:	e000ed00 	.word	0xe000ed00

30006924 <BOOT_Validate_ImgHash>:
30006924:	b51f      	push	{r0, r1, r2, r3, r4, lr}
30006926:	4604      	mov	r4, r0
30006928:	480a      	ldr	r0, [pc, #40]	; (30006954 <BOOT_Validate_ImgHash+0x30>)
3000692a:	9301      	str	r3, [sp, #4]
3000692c:	e9cd 2102 	strd	r2, r1, [sp, #8]
30006930:	4780      	blx	r0
30006932:	9903      	ldr	r1, [sp, #12]
30006934:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30006938:	b128      	cbz	r0, 30006946 <BOOT_Validate_ImgHash+0x22>
3000693a:	4620      	mov	r0, r4
3000693c:	b004      	add	sp, #16
3000693e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30006942:	f002 bc87 	b.w	30009254 <SBOOT_Validate_ImgHash>
30006946:	4620      	mov	r0, r4
30006948:	b004      	add	sp, #16
3000694a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000694e:	f7ff becf 	b.w	300066f0 <_BOOT_Validate_ImgHash>
30006952:	bf00      	nop
30006954:	30008ff5 	.word	0x30008ff5

30006958 <BOOT_SignatureCheck>:
30006958:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000695c:	461f      	mov	r7, r3
3000695e:	b089      	sub	sp, #36	; 0x24
30006960:	4b5d      	ldr	r3, [pc, #372]	; (30006ad8 <BOOT_SignatureCheck+0x180>)
30006962:	4606      	mov	r6, r0
30006964:	4689      	mov	r9, r1
30006966:	4690      	mov	r8, r2
30006968:	ac08      	add	r4, sp, #32
3000696a:	9d10      	ldr	r5, [sp, #64]	; 0x40
3000696c:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000696e:	e904 000f 	stmdb	r4, {r0, r1, r2, r3}
30006972:	4b5a      	ldr	r3, [pc, #360]	; (30006adc <BOOT_SignatureCheck+0x184>)
30006974:	4798      	blx	r3
30006976:	2800      	cmp	r0, #0
30006978:	d04e      	beq.n	30006a18 <BOOT_SignatureCheck+0xc0>
3000697a:	4b59      	ldr	r3, [pc, #356]	; (30006ae0 <BOOT_SignatureCheck+0x188>)
3000697c:	2000      	movs	r0, #0
3000697e:	4798      	blx	r3
30006980:	7cf3      	ldrb	r3, [r6, #19]
30006982:	7cb2      	ldrb	r2, [r6, #18]
30006984:	f10d 010f 	add.w	r1, sp, #15
30006988:	f10d 000e 	add.w	r0, sp, #14
3000698c:	f002 fc94 	bl	300092b8 <SBOOT_Validate_Algorithm>
30006990:	4604      	mov	r4, r0
30006992:	2800      	cmp	r0, #0
30006994:	d143      	bne.n	30006a1e <BOOT_SignatureCheck+0xc6>
30006996:	69fb      	ldr	r3, [r7, #28]
30006998:	2124      	movs	r1, #36	; 0x24
3000699a:	b2c2      	uxtb	r2, r0
3000699c:	4293      	cmp	r3, r2
3000699e:	d906      	bls.n	300069ae <BOOT_SignatureCheck+0x56>
300069a0:	fb01 7402 	mla	r4, r1, r2, r7
300069a4:	3001      	adds	r0, #1
300069a6:	f8d4 40b0 	ldr.w	r4, [r4, #176]	; 0xb0
300069aa:	42ac      	cmp	r4, r5
300069ac:	d1f5      	bne.n	3000699a <BOOT_SignatureCheck+0x42>
300069ae:	2324      	movs	r3, #36	; 0x24
300069b0:	f106 0570 	add.w	r5, r6, #112	; 0x70
300069b4:	f89d 000e 	ldrb.w	r0, [sp, #14]
300069b8:	fb03 7202 	mla	r2, r3, r2, r7
300069bc:	4629      	mov	r1, r5
300069be:	32b4      	adds	r2, #180	; 0xb4
300069c0:	f002 fc0a 	bl	300091d8 <SBOOT_Validate_PubKey>
300069c4:	4604      	mov	r4, r0
300069c6:	bb50      	cbnz	r0, 30006a1e <BOOT_SignatureCheck+0xc6>
300069c8:	f506 7380 	add.w	r3, r6, #256	; 0x100
300069cc:	462a      	mov	r2, r5
300069ce:	f89d 100f 	ldrb.w	r1, [sp, #15]
300069d2:	9301      	str	r3, [sp, #4]
300069d4:	f44f 7380 	mov.w	r3, #256	; 0x100
300069d8:	f89d 000e 	ldrb.w	r0, [sp, #14]
300069dc:	9300      	str	r3, [sp, #0]
300069de:	4633      	mov	r3, r6
300069e0:	f002 fc16 	bl	30009210 <SBOOT_Validate_Signature>
300069e4:	4604      	mov	r4, r0
300069e6:	b9d0      	cbnz	r0, 30006a1e <BOOT_SignatureCheck+0xc6>
300069e8:	4643      	mov	r3, r8
300069ea:	464a      	mov	r2, r9
300069ec:	f106 0130 	add.w	r1, r6, #48	; 0x30
300069f0:	f89d 000f 	ldrb.w	r0, [sp, #15]
300069f4:	f7ff ff96 	bl	30006924 <BOOT_Validate_ImgHash>
300069f8:	4604      	mov	r4, r0
300069fa:	b980      	cbnz	r0, 30006a1e <BOOT_SignatureCheck+0xc6>
300069fc:	7c73      	ldrb	r3, [r6, #17]
300069fe:	aa08      	add	r2, sp, #32
30006a00:	2004      	movs	r0, #4
30006a02:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30006a06:	4a37      	ldr	r2, [pc, #220]	; (30006ae4 <BOOT_SignatureCheck+0x18c>)
30006a08:	f853 3c10 	ldr.w	r3, [r3, #-16]
30006a0c:	9300      	str	r3, [sp, #0]
30006a0e:	4b36      	ldr	r3, [pc, #216]	; (30006ae8 <BOOT_SignatureCheck+0x190>)
30006a10:	4619      	mov	r1, r3
30006a12:	f004 fa83 	bl	3000af1c <rtk_log_write>
30006a16:	4620      	mov	r0, r4
30006a18:	b009      	add	sp, #36	; 0x24
30006a1a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006a1e:	9401      	str	r4, [sp, #4]
30006a20:	aa08      	add	r2, sp, #32
30006a22:	7c73      	ldrb	r3, [r6, #17]
30006a24:	eb09 08c8 	add.w	r8, r9, r8, lsl #3
30006a28:	4f30      	ldr	r7, [pc, #192]	; (30006aec <BOOT_SignatureCheck+0x194>)
30006a2a:	2002      	movs	r0, #2
30006a2c:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30006a30:	4e2f      	ldr	r6, [pc, #188]	; (30006af0 <BOOT_SignatureCheck+0x198>)
30006a32:	4a30      	ldr	r2, [pc, #192]	; (30006af4 <BOOT_SignatureCheck+0x19c>)
30006a34:	f853 3c10 	ldr.w	r3, [r3, #-16]
30006a38:	9300      	str	r3, [sp, #0]
30006a3a:	4b2b      	ldr	r3, [pc, #172]	; (30006ae8 <BOOT_SignatureCheck+0x190>)
30006a3c:	4619      	mov	r1, r3
30006a3e:	f004 fa6d 	bl	3000af1c <rtk_log_write>
30006a42:	45c8      	cmp	r8, r9
30006a44:	d101      	bne.n	30006a4a <BOOT_SignatureCheck+0xf2>
30006a46:	20ff      	movs	r0, #255	; 0xff
30006a48:	e7e6      	b.n	30006a18 <BOOT_SignatureCheck+0xc0>
30006a4a:	f8d9 0000 	ldr.w	r0, [r9]
30006a4e:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30006a52:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006a56:	d32b      	bcc.n	30006ab0 <BOOT_SignatureCheck+0x158>
30006a58:	f8d9 2004 	ldr.w	r2, [r9, #4]
30006a5c:	2100      	movs	r1, #0
30006a5e:	47b8      	blx	r7
30006a60:	6973      	ldr	r3, [r6, #20]
30006a62:	03db      	lsls	r3, r3, #15
30006a64:	d524      	bpl.n	30006ab0 <BOOT_SignatureCheck+0x158>
30006a66:	e9d9 3100 	ldrd	r3, r1, [r9]
30006a6a:	ea03 0201 	and.w	r2, r3, r1
30006a6e:	3201      	adds	r2, #1
30006a70:	d121      	bne.n	30006ab6 <BOOT_SignatureCheck+0x15e>
30006a72:	2300      	movs	r3, #0
30006a74:	f8c6 3084 	str.w	r3, [r6, #132]	; 0x84
30006a78:	f3bf 8f4f 	dsb	sy
30006a7c:	f8d6 1080 	ldr.w	r1, [r6, #128]	; 0x80
30006a80:	f643 70e0 	movw	r0, #16352	; 0x3fe0
30006a84:	f3c1 02c9 	ubfx	r2, r1, #3, #10
30006a88:	f3c1 314e 	ubfx	r1, r1, #13, #15
30006a8c:	0149      	lsls	r1, r1, #5
30006a8e:	ea01 0e00 	and.w	lr, r1, r0
30006a92:	4613      	mov	r3, r2
30006a94:	ea4e 7483 	orr.w	r4, lr, r3, lsl #30
30006a98:	3b01      	subs	r3, #1
30006a9a:	f8c6 4274 	str.w	r4, [r6, #628]	; 0x274
30006a9e:	d2f9      	bcs.n	30006a94 <BOOT_SignatureCheck+0x13c>
30006aa0:	3920      	subs	r1, #32
30006aa2:	f111 0f20 	cmn.w	r1, #32
30006aa6:	d1f2      	bne.n	30006a8e <BOOT_SignatureCheck+0x136>
30006aa8:	f3bf 8f4f 	dsb	sy
30006aac:	f3bf 8f6f 	isb	sy
30006ab0:	f109 0908 	add.w	r9, r9, #8
30006ab4:	e7c5      	b.n	30006a42 <BOOT_SignatureCheck+0xea>
30006ab6:	f013 021f 	ands.w	r2, r3, #31
30006aba:	bf1c      	itt	ne
30006abc:	f023 031f 	bicne.w	r3, r3, #31
30006ac0:	1889      	addne	r1, r1, r2
30006ac2:	f3bf 8f4f 	dsb	sy
30006ac6:	440b      	add	r3, r1
30006ac8:	2900      	cmp	r1, #0
30006aca:	eba3 0201 	sub.w	r2, r3, r1
30006ace:	ddeb      	ble.n	30006aa8 <BOOT_SignatureCheck+0x150>
30006ad0:	3920      	subs	r1, #32
30006ad2:	f8c6 2270 	str.w	r2, [r6, #624]	; 0x270
30006ad6:	e7f7      	b.n	30006ac8 <BOOT_SignatureCheck+0x170>
30006ad8:	3000d244 	.word	0x3000d244
30006adc:	0000c149 	.word	0x0000c149
30006ae0:	30009291 	.word	0x30009291
30006ae4:	3000d1d8 	.word	0x3000d1d8
30006ae8:	3000c714 	.word	0x3000c714
30006aec:	00012be5 	.word	0x00012be5
30006af0:	e000ed00 	.word	0xe000ed00
30006af4:	3000d1ef 	.word	0x3000d1ef

30006af8 <BOOT_CertificateCheck>:
30006af8:	b570      	push	{r4, r5, r6, lr}
30006afa:	4b28      	ldr	r3, [pc, #160]	; (30006b9c <BOOT_CertificateCheck+0xa4>)
30006afc:	b0b0      	sub	sp, #192	; 0xc0
30006afe:	4604      	mov	r4, r0
30006b00:	460e      	mov	r6, r1
30006b02:	4798      	blx	r3
30006b04:	b910      	cbnz	r0, 30006b0c <BOOT_CertificateCheck+0x14>
30006b06:	2000      	movs	r0, #0
30006b08:	b030      	add	sp, #192	; 0xc0
30006b0a:	bd70      	pop	{r4, r5, r6, pc}
30006b0c:	69a1      	ldr	r1, [r4, #24]
30006b0e:	ad0c      	add	r5, sp, #48	; 0x30
30006b10:	2290      	movs	r2, #144	; 0x90
30006b12:	4431      	add	r1, r6
30006b14:	4628      	mov	r0, r5
30006b16:	f7fd fd65 	bl	300045e4 <BOOT_ImgCopy>
30006b1a:	4b21      	ldr	r3, [pc, #132]	; (30006ba0 <BOOT_CertificateCheck+0xa8>)
30006b1c:	4798      	blx	r3
30006b1e:	4a21      	ldr	r2, [pc, #132]	; (30006ba4 <BOOT_CertificateCheck+0xac>)
30006b20:	f000 031f 	and.w	r3, r0, #31
30006b24:	2020      	movs	r0, #32
30006b26:	6812      	ldr	r2, [r2, #0]
30006b28:	f102 4284 	add.w	r2, r2, #1107296256	; 0x42000000
30006b2c:	a930      	add	r1, sp, #192	; 0xc0
30006b2e:	3801      	subs	r0, #1
30006b30:	5c9e      	ldrb	r6, [r3, r2]
30006b32:	4419      	add	r1, r3
30006b34:	f010 00ff 	ands.w	r0, r0, #255	; 0xff
30006b38:	f103 0301 	add.w	r3, r3, #1
30006b3c:	f801 6cb0 	strb.w	r6, [r1, #-176]
30006b40:	f003 031f 	and.w	r3, r3, #31
30006b44:	d1f2      	bne.n	30006b2c <BOOT_CertificateCheck+0x34>
30006b46:	4b18      	ldr	r3, [pc, #96]	; (30006ba8 <BOOT_CertificateCheck+0xb0>)
30006b48:	4798      	blx	r3
30006b4a:	7ce3      	ldrb	r3, [r4, #19]
30006b4c:	7ca2      	ldrb	r2, [r4, #18]
30006b4e:	f10d 010f 	add.w	r1, sp, #15
30006b52:	f10d 000e 	add.w	r0, sp, #14
30006b56:	f002 fbaf 	bl	300092b8 <SBOOT_Validate_Algorithm>
30006b5a:	b9a8      	cbnz	r0, 30006b88 <BOOT_CertificateCheck+0x90>
30006b5c:	f104 0620 	add.w	r6, r4, #32
30006b60:	aa04      	add	r2, sp, #16
30006b62:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006b66:	4631      	mov	r1, r6
30006b68:	f002 fb36 	bl	300091d8 <SBOOT_Validate_PubKey>
30006b6c:	b960      	cbnz	r0, 30006b88 <BOOT_CertificateCheck+0x90>
30006b6e:	69a3      	ldr	r3, [r4, #24]
30006b70:	4632      	mov	r2, r6
30006b72:	f89d 100f 	ldrb.w	r1, [sp, #15]
30006b76:	9300      	str	r3, [sp, #0]
30006b78:	4623      	mov	r3, r4
30006b7a:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006b7e:	9501      	str	r5, [sp, #4]
30006b80:	f002 fb46 	bl	30009210 <SBOOT_Validate_Signature>
30006b84:	2800      	cmp	r0, #0
30006b86:	d0be      	beq.n	30006b06 <BOOT_CertificateCheck+0xe>
30006b88:	4b08      	ldr	r3, [pc, #32]	; (30006bac <BOOT_CertificateCheck+0xb4>)
30006b8a:	9000      	str	r0, [sp, #0]
30006b8c:	2002      	movs	r0, #2
30006b8e:	4a08      	ldr	r2, [pc, #32]	; (30006bb0 <BOOT_CertificateCheck+0xb8>)
30006b90:	4619      	mov	r1, r3
30006b92:	f004 f9c3 	bl	3000af1c <rtk_log_write>
30006b96:	20ff      	movs	r0, #255	; 0xff
30006b98:	e7b6      	b.n	30006b08 <BOOT_CertificateCheck+0x10>
30006b9a:	bf00      	nop
30006b9c:	0000c149 	.word	0x0000c149
30006ba0:	0000ea4d 	.word	0x0000ea4d
30006ba4:	3000d558 	.word	0x3000d558
30006ba8:	30009291 	.word	0x30009291
30006bac:	3000c714 	.word	0x3000c714
30006bb0:	3000d210 	.word	0x3000d210

30006bb4 <BOOT_RAM_TZCfg>:
30006bb4:	e92d 47ff 	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, lr}
30006bb8:	4a5b      	ldr	r2, [pc, #364]	; (30006d28 <BOOT_RAM_TZCfg+0x174>)
30006bba:	466e      	mov	r6, sp
30006bbc:	4b5b      	ldr	r3, [pc, #364]	; (30006d2c <BOOT_RAM_TZCfg+0x178>)
30006bbe:	f10d 0c08 	add.w	ip, sp, #8
30006bc2:	2400      	movs	r4, #0
30006bc4:	2501      	movs	r5, #1
30006bc6:	f8df e188 	ldr.w	lr, [pc, #392]	; 30006d50 <BOOT_RAM_TZCfg+0x19c>
30006bca:	f44f 18c0 	mov.w	r8, #1572864	; 0x180000
30006bce:	f8df 9184 	ldr.w	r9, [pc, #388]	; 30006d54 <BOOT_RAM_TZCfg+0x1a0>
30006bd2:	e9cd 3200 	strd	r3, r2, [sp]
30006bd6:	4a56      	ldr	r2, [pc, #344]	; (30006d30 <BOOT_RAM_TZCfg+0x17c>)
30006bd8:	9202      	str	r2, [sp, #8]
30006bda:	4a56      	ldr	r2, [pc, #344]	; (30006d34 <BOOT_RAM_TZCfg+0x180>)
30006bdc:	9203      	str	r2, [sp, #12]
30006bde:	f856 2b04 	ldr.w	r2, [r6], #4
30006be2:	2100      	movs	r1, #0
30006be4:	f85c 0b04 	ldr.w	r0, [ip], #4
30006be8:	6807      	ldr	r7, [r0, #0]
30006bea:	f1b7 3fff 	cmp.w	r7, #4294967295	; 0xffffffff
30006bee:	d011      	beq.n	30006c14 <BOOT_RAM_TZCfg+0x60>
30006bf0:	f842 7031 	str.w	r7, [r2, r1, lsl #3]
30006bf4:	eb02 0ac1 	add.w	sl, r2, r1, lsl #3
30006bf8:	6847      	ldr	r7, [r0, #4]
30006bfa:	300c      	adds	r0, #12
30006bfc:	f8ca 7004 	str.w	r7, [sl, #4]
30006c00:	fa05 f701 	lsl.w	r7, r5, r1
30006c04:	f8d2 a040 	ldr.w	sl, [r2, #64]	; 0x40
30006c08:	3101      	adds	r1, #1
30006c0a:	ea47 070a 	orr.w	r7, r7, sl
30006c0e:	2908      	cmp	r1, #8
30006c10:	6417      	str	r7, [r2, #64]	; 0x40
30006c12:	d1e9      	bne.n	30006be8 <BOOT_RAM_TZCfg+0x34>
30006c14:	429a      	cmp	r2, r3
30006c16:	f040 8084 	bne.w	30006d22 <BOOT_RAM_TZCfg+0x16e>
30006c1a:	f89e 2000 	ldrb.w	r2, [lr]
30006c1e:	b942      	cbnz	r2, 30006c32 <BOOT_RAM_TZCfg+0x7e>
30006c20:	f8c3 8038 	str.w	r8, [r3, #56]	; 0x38
30006c24:	f8c3 903c 	str.w	r9, [r3, #60]	; 0x3c
30006c28:	6c1a      	ldr	r2, [r3, #64]	; 0x40
30006c2a:	f042 0280 	orr.w	r2, r2, #128	; 0x80
30006c2e:	641a      	str	r2, [r3, #64]	; 0x40
30006c30:	645d      	str	r5, [r3, #68]	; 0x44
30006c32:	2c00      	cmp	r4, #0
30006c34:	d073      	beq.n	30006d1e <BOOT_RAM_TZCfg+0x16a>
30006c36:	4b40      	ldr	r3, [pc, #256]	; (30006d38 <BOOT_RAM_TZCfg+0x184>)
30006c38:	f46f 3200 	mvn.w	r2, #131072	; 0x20000
30006c3c:	2100      	movs	r1, #0
30006c3e:	483f      	ldr	r0, [pc, #252]	; (30006d3c <BOOT_RAM_TZCfg+0x188>)
30006c40:	601a      	str	r2, [r3, #0]
30006c42:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30006c46:	605a      	str	r2, [r3, #4]
30006c48:	4b3d      	ldr	r3, [pc, #244]	; (30006d40 <BOOT_RAM_TZCfg+0x18c>)
30006c4a:	4a3e      	ldr	r2, [pc, #248]	; (30006d44 <BOOT_RAM_TZCfg+0x190>)
30006c4c:	f8c3 4398 	str.w	r4, [r3, #920]	; 0x398
30006c50:	6813      	ldr	r3, [r2, #0]
30006c52:	3301      	adds	r3, #1
30006c54:	d01a      	beq.n	30006c8c <BOOT_RAM_TZCfg+0xd8>
30006c56:	f8c0 10d8 	str.w	r1, [r0, #216]	; 0xd8
30006c5a:	3101      	adds	r1, #1
30006c5c:	6813      	ldr	r3, [r2, #0]
30006c5e:	320c      	adds	r2, #12
30006c60:	2908      	cmp	r1, #8
30006c62:	f023 031f 	bic.w	r3, r3, #31
30006c66:	f8c0 30dc 	str.w	r3, [r0, #220]	; 0xdc
30006c6a:	f852 3c04 	ldr.w	r3, [r2, #-4]
30006c6e:	f852 4c08 	ldr.w	r4, [r2, #-8]
30006c72:	ea4f 0343 	mov.w	r3, r3, lsl #1
30006c76:	f024 041f 	bic.w	r4, r4, #31
30006c7a:	f003 0302 	and.w	r3, r3, #2
30006c7e:	ea43 0304 	orr.w	r3, r3, r4
30006c82:	f043 0301 	orr.w	r3, r3, #1
30006c86:	f8c0 30e0 	str.w	r3, [r0, #224]	; 0xe0
30006c8a:	d1e1      	bne.n	30006c50 <BOOT_RAM_TZCfg+0x9c>
30006c8c:	2301      	movs	r3, #1
30006c8e:	4a2b      	ldr	r2, [pc, #172]	; (30006d3c <BOOT_RAM_TZCfg+0x188>)
30006c90:	f8c2 30d0 	str.w	r3, [r2, #208]	; 0xd0
30006c94:	4b2c      	ldr	r3, [pc, #176]	; (30006d48 <BOOT_RAM_TZCfg+0x194>)
30006c96:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30006c9a:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30006c9e:	d11d      	bne.n	30006cdc <BOOT_RAM_TZCfg+0x128>
30006ca0:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30006ca4:	f3bf 8f4f 	dsb	sy
30006ca8:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30006cac:	f643 74e0 	movw	r4, #16352	; 0x3fe0
30006cb0:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30006cb4:	f3c3 334e 	ubfx	r3, r3, #13, #15
30006cb8:	015b      	lsls	r3, r3, #5
30006cba:	ea03 0604 	and.w	r6, r3, r4
30006cbe:	4601      	mov	r1, r0
30006cc0:	ea46 7581 	orr.w	r5, r6, r1, lsl #30
30006cc4:	3901      	subs	r1, #1
30006cc6:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
30006cca:	d2f9      	bcs.n	30006cc0 <BOOT_RAM_TZCfg+0x10c>
30006ccc:	3b20      	subs	r3, #32
30006cce:	f113 0f20 	cmn.w	r3, #32
30006cd2:	d1f2      	bne.n	30006cba <BOOT_RAM_TZCfg+0x106>
30006cd4:	f3bf 8f4f 	dsb	sy
30006cd8:	f3bf 8f6f 	isb	sy
30006cdc:	4a17      	ldr	r2, [pc, #92]	; (30006d3c <BOOT_RAM_TZCfg+0x188>)
30006cde:	6913      	ldr	r3, [r2, #16]
30006ce0:	f043 0308 	orr.w	r3, r3, #8
30006ce4:	6113      	str	r3, [r2, #16]
30006ce6:	68d3      	ldr	r3, [r2, #12]
30006ce8:	f423 43c0 	bic.w	r3, r3, #24576	; 0x6000
30006cec:	f023 0308 	bic.w	r3, r3, #8
30006cf0:	041b      	lsls	r3, r3, #16
30006cf2:	0c1b      	lsrs	r3, r3, #16
30006cf4:	f043 63bf 	orr.w	r3, r3, #100139008	; 0x5f80000
30006cf8:	f443 3318 	orr.w	r3, r3, #155648	; 0x26000
30006cfc:	60d3      	str	r3, [r2, #12]
30006cfe:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30006d02:	4b12      	ldr	r3, [pc, #72]	; (30006d4c <BOOT_RAM_TZCfg+0x198>)
30006d04:	f8c3 2280 	str.w	r2, [r3, #640]	; 0x280
30006d08:	f46f 0280 	mvn.w	r2, #4194304	; 0x400000
30006d0c:	f8c3 2284 	str.w	r2, [r3, #644]	; 0x284
30006d10:	f06f 02c0 	mvn.w	r2, #192	; 0xc0
30006d14:	f8c3 2288 	str.w	r2, [r3, #648]	; 0x288
30006d18:	b004      	add	sp, #16
30006d1a:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30006d1e:	2401      	movs	r4, #1
30006d20:	e75d      	b.n	30006bde <BOOT_RAM_TZCfg+0x2a>
30006d22:	6455      	str	r5, [r2, #68]	; 0x44
30006d24:	e785      	b.n	30006c32 <BOOT_RAM_TZCfg+0x7e>
30006d26:	bf00      	nop
30006d28:	51001200 	.word	0x51001200
30006d2c:	51001a00 	.word	0x51001a00
30006d30:	3000d658 	.word	0x3000d658
30006d34:	3000d5f8 	.word	0x3000d5f8
30006d38:	51001b00 	.word	0x51001b00
30006d3c:	e000ed00 	.word	0xe000ed00
30006d40:	500e0000 	.word	0x500e0000
30006d44:	3000d6b8 	.word	0x3000d6b8
30006d48:	42008000 	.word	0x42008000
30006d4c:	e000e100 	.word	0xe000e100
30006d50:	3000d553 	.word	0x3000d553
30006d54:	005fffff 	.word	0x005fffff

30006d58 <ymodem_inquiry>:
30006d58:	2806      	cmp	r0, #6
30006d5a:	b510      	push	{r4, lr}
30006d5c:	4604      	mov	r4, r0
30006d5e:	d003      	beq.n	30006d68 <ymodem_inquiry+0x10>
30006d60:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30006d64:	4b03      	ldr	r3, [pc, #12]	; (30006d74 <ymodem_inquiry+0x1c>)
30006d66:	4798      	blx	r3
30006d68:	4620      	mov	r0, r4
30006d6a:	f000 f9a9 	bl	300070c0 <ymodem_uart_putc>
30006d6e:	2001      	movs	r0, #1
30006d70:	bd10      	pop	{r4, pc}
30006d72:	bf00      	nop
30006d74:	00009b2d 	.word	0x00009b2d

30006d78 <ymodem_get_first>:
30006d78:	2300      	movs	r3, #0
30006d7a:	b537      	push	{r0, r1, r2, r4, r5, lr}
30006d7c:	4604      	mov	r4, r0
30006d7e:	4926      	ldr	r1, [pc, #152]	; (30006e18 <ymodem_get_first+0xa0>)
30006d80:	f10d 0006 	add.w	r0, sp, #6
30006d84:	f88d 3006 	strb.w	r3, [sp, #6]
30006d88:	f000 f9ba 	bl	30007100 <ymodem_uart_getc_to>
30006d8c:	b120      	cbz	r0, 30006d98 <ymodem_get_first+0x20>
30006d8e:	2301      	movs	r3, #1
30006d90:	2008      	movs	r0, #8
30006d92:	60a3      	str	r3, [r4, #8]
30006d94:	b003      	add	sp, #12
30006d96:	bd30      	pop	{r4, r5, pc}
30006d98:	f89d 3006 	ldrb.w	r3, [sp, #6]
30006d9c:	2b04      	cmp	r3, #4
30006d9e:	d01b      	beq.n	30006dd8 <ymodem_get_first+0x60>
30006da0:	d80c      	bhi.n	30006dbc <ymodem_get_first+0x44>
30006da2:	2b01      	cmp	r3, #1
30006da4:	d010      	beq.n	30006dc8 <ymodem_get_first+0x50>
30006da6:	2b02      	cmp	r3, #2
30006da8:	d012      	beq.n	30006dd0 <ymodem_get_first+0x58>
30006daa:	4d1b      	ldr	r5, [pc, #108]	; (30006e18 <ymodem_get_first+0xa0>)
30006dac:	4629      	mov	r1, r5
30006dae:	f10d 0007 	add.w	r0, sp, #7
30006db2:	f000 f9a5 	bl	30007100 <ymodem_uart_getc_to>
30006db6:	2800      	cmp	r0, #0
30006db8:	d0f8      	beq.n	30006dac <ymodem_get_first+0x34>
30006dba:	e7e8      	b.n	30006d8e <ymodem_get_first+0x16>
30006dbc:	2b18      	cmp	r3, #24
30006dbe:	d1f4      	bne.n	30006daa <ymodem_get_first+0x32>
30006dc0:	2301      	movs	r3, #1
30006dc2:	2002      	movs	r0, #2
30006dc4:	60a3      	str	r3, [r4, #8]
30006dc6:	e7e5      	b.n	30006d94 <ymodem_get_first+0x1c>
30006dc8:	2280      	movs	r2, #128	; 0x80
30006dca:	80a2      	strh	r2, [r4, #4]
30006dcc:	2001      	movs	r0, #1
30006dce:	e7e1      	b.n	30006d94 <ymodem_get_first+0x1c>
30006dd0:	f44f 6380 	mov.w	r3, #1024	; 0x400
30006dd4:	80a3      	strh	r3, [r4, #4]
30006dd6:	e7f9      	b.n	30006dcc <ymodem_get_first+0x54>
30006dd8:	2006      	movs	r0, #6
30006dda:	f000 f971 	bl	300070c0 <ymodem_uart_putc>
30006dde:	2043      	movs	r0, #67	; 0x43
30006de0:	f7ff ffba 	bl	30006d58 <ymodem_inquiry>
30006de4:	2006      	movs	r0, #6
30006de6:	f000 f96b 	bl	300070c0 <ymodem_uart_putc>
30006dea:	2043      	movs	r0, #67	; 0x43
30006dec:	f7ff ffb4 	bl	30006d58 <ymodem_inquiry>
30006df0:	2018      	movs	r0, #24
30006df2:	f7ff ffb1 	bl	30006d58 <ymodem_inquiry>
30006df6:	2018      	movs	r0, #24
30006df8:	f7ff ffae 	bl	30006d58 <ymodem_inquiry>
30006dfc:	2018      	movs	r0, #24
30006dfe:	f7ff ffab 	bl	30006d58 <ymodem_inquiry>
30006e02:	2018      	movs	r0, #24
30006e04:	f7ff ffa8 	bl	30006d58 <ymodem_inquiry>
30006e08:	2018      	movs	r0, #24
30006e0a:	f7ff ffa5 	bl	30006d58 <ymodem_inquiry>
30006e0e:	2301      	movs	r3, #1
30006e10:	2005      	movs	r0, #5
30006e12:	60a3      	str	r3, [r4, #8]
30006e14:	e7be      	b.n	30006d94 <ymodem_get_first+0x1c>
30006e16:	bf00      	nop
30006e18:	000caa30 	.word	0x000caa30

30006e1c <ymodem_get_others>:
30006e1c:	2300      	movs	r3, #0
30006e1e:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
30006e22:	4605      	mov	r5, r0
30006e24:	494c      	ldr	r1, [pc, #304]	; (30006f58 <ymodem_get_others+0x13c>)
30006e26:	a801      	add	r0, sp, #4
30006e28:	f88d 3006 	strb.w	r3, [sp, #6]
30006e2c:	f88d 3007 	strb.w	r3, [sp, #7]
30006e30:	f000 f966 	bl	30007100 <ymodem_uart_getc_to>
30006e34:	b130      	cbz	r0, 30006e44 <ymodem_get_others+0x28>
30006e36:	2301      	movs	r3, #1
30006e38:	2408      	movs	r4, #8
30006e3a:	60ab      	str	r3, [r5, #8]
30006e3c:	4620      	mov	r0, r4
30006e3e:	b003      	add	sp, #12
30006e40:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006e44:	4944      	ldr	r1, [pc, #272]	; (30006f58 <ymodem_get_others+0x13c>)
30006e46:	f10d 0005 	add.w	r0, sp, #5
30006e4a:	f000 f959 	bl	30007100 <ymodem_uart_getc_to>
30006e4e:	4606      	mov	r6, r0
30006e50:	2800      	cmp	r0, #0
30006e52:	d1f0      	bne.n	30006e36 <ymodem_get_others+0x1a>
30006e54:	f89d 2004 	ldrb.w	r2, [sp, #4]
30006e58:	f89d 4005 	ldrb.w	r4, [sp, #5]
30006e5c:	43d3      	mvns	r3, r2
30006e5e:	b2db      	uxtb	r3, r3
30006e60:	429c      	cmp	r4, r3
30006e62:	bf0c      	ite	eq
30006e64:	2401      	moveq	r4, #1
30006e66:	2404      	movne	r4, #4
30006e68:	682b      	ldr	r3, [r5, #0]
30006e6a:	429a      	cmp	r2, r3
30006e6c:	d330      	bcc.n	30006ed0 <ymodem_get_others+0xb4>
30006e6e:	d000      	beq.n	30006e72 <ymodem_get_others+0x56>
30006e70:	2404      	movs	r4, #4
30006e72:	2700      	movs	r7, #0
30006e74:	f8df 80e4 	ldr.w	r8, [pc, #228]	; 30006f5c <ymodem_get_others+0x140>
30006e78:	f8df 90dc 	ldr.w	r9, [pc, #220]	; 30006f58 <ymodem_get_others+0x13c>
30006e7c:	88ab      	ldrh	r3, [r5, #4]
30006e7e:	42bb      	cmp	r3, r7
30006e80:	d828      	bhi.n	30006ed4 <ymodem_get_others+0xb8>
30006e82:	4935      	ldr	r1, [pc, #212]	; (30006f58 <ymodem_get_others+0x13c>)
30006e84:	f10d 0006 	add.w	r0, sp, #6
30006e88:	f000 f93a 	bl	30007100 <ymodem_uart_getc_to>
30006e8c:	2800      	cmp	r0, #0
30006e8e:	d1d2      	bne.n	30006e36 <ymodem_get_others+0x1a>
30006e90:	4931      	ldr	r1, [pc, #196]	; (30006f58 <ymodem_get_others+0x13c>)
30006e92:	f10d 0007 	add.w	r0, sp, #7
30006e96:	f000 f933 	bl	30007100 <ymodem_uart_getc_to>
30006e9a:	4603      	mov	r3, r0
30006e9c:	2800      	cmp	r0, #0
30006e9e:	d1ca      	bne.n	30006e36 <ymodem_get_others+0x1a>
30006ea0:	f89d 2006 	ldrb.w	r2, [sp, #6]
30006ea4:	f241 0e21 	movw	lr, #4129	; 0x1021
30006ea8:	f89d 0007 	ldrb.w	r0, [sp, #7]
30006eac:	492b      	ldr	r1, [pc, #172]	; (30006f5c <ymodem_get_others+0x140>)
30006eae:	ea40 2002 	orr.w	r0, r0, r2, lsl #8
30006eb2:	88aa      	ldrh	r2, [r5, #4]
30006eb4:	eb02 0c01 	add.w	ip, r2, r1
30006eb8:	4561      	cmp	r1, ip
30006eba:	d114      	bne.n	30006ee6 <ymodem_get_others+0xca>
30006ebc:	b29b      	uxth	r3, r3
30006ebe:	4298      	cmp	r0, r3
30006ec0:	d021      	beq.n	30006f06 <ymodem_get_others+0xea>
30006ec2:	2015      	movs	r0, #21
30006ec4:	2404      	movs	r4, #4
30006ec6:	f7ff ff47 	bl	30006d58 <ymodem_inquiry>
30006eca:	2301      	movs	r3, #1
30006ecc:	60ab      	str	r3, [r5, #8]
30006ece:	e7b5      	b.n	30006e3c <ymodem_get_others+0x20>
30006ed0:	2601      	movs	r6, #1
30006ed2:	e7ce      	b.n	30006e72 <ymodem_get_others+0x56>
30006ed4:	4649      	mov	r1, r9
30006ed6:	eb07 0008 	add.w	r0, r7, r8
30006eda:	f000 f911 	bl	30007100 <ymodem_uart_getc_to>
30006ede:	2800      	cmp	r0, #0
30006ee0:	d1a9      	bne.n	30006e36 <ymodem_get_others+0x1a>
30006ee2:	3701      	adds	r7, #1
30006ee4:	e7ca      	b.n	30006e7c <ymodem_get_others+0x60>
30006ee6:	f811 7b01 	ldrb.w	r7, [r1], #1
30006eea:	ea83 2307 	eor.w	r3, r3, r7, lsl #8
30006eee:	2708      	movs	r7, #8
30006ef0:	f403 4800 	and.w	r8, r3, #32768	; 0x8000
30006ef4:	005b      	lsls	r3, r3, #1
30006ef6:	f1b8 0f00 	cmp.w	r8, #0
30006efa:	d001      	beq.n	30006f00 <ymodem_get_others+0xe4>
30006efc:	ea83 030e 	eor.w	r3, r3, lr
30006f00:	3f01      	subs	r7, #1
30006f02:	d1f5      	bne.n	30006ef0 <ymodem_get_others+0xd4>
30006f04:	e7d8      	b.n	30006eb8 <ymodem_get_others+0x9c>
30006f06:	b96e      	cbnz	r6, 30006f24 <ymodem_get_others+0x108>
30006f08:	4f15      	ldr	r7, [pc, #84]	; (30006f60 <ymodem_get_others+0x144>)
30006f0a:	683b      	ldr	r3, [r7, #0]
30006f0c:	b153      	cbz	r3, 30006f24 <ymodem_get_others+0x108>
30006f0e:	68eb      	ldr	r3, [r5, #12]
30006f10:	6879      	ldr	r1, [r7, #4]
30006f12:	3301      	adds	r3, #1
30006f14:	4811      	ldr	r0, [pc, #68]	; (30006f5c <ymodem_get_others+0x140>)
30006f16:	60eb      	str	r3, [r5, #12]
30006f18:	f000 f920 	bl	3000715c <UARTIMG_Write>
30006f1c:	687a      	ldr	r2, [r7, #4]
30006f1e:	88ab      	ldrh	r3, [r5, #4]
30006f20:	4413      	add	r3, r2
30006f22:	607b      	str	r3, [r7, #4]
30006f24:	f000 f8fe 	bl	30007124 <ymodem_uart_clean_rx>
30006f28:	2c01      	cmp	r4, #1
30006f2a:	d111      	bne.n	30006f50 <ymodem_get_others+0x134>
30006f2c:	60ac      	str	r4, [r5, #8]
30006f2e:	2006      	movs	r0, #6
30006f30:	4d0b      	ldr	r5, [pc, #44]	; (30006f60 <ymodem_get_others+0x144>)
30006f32:	f000 f8c5 	bl	300070c0 <ymodem_uart_putc>
30006f36:	682b      	ldr	r3, [r5, #0]
30006f38:	b913      	cbnz	r3, 30006f40 <ymodem_get_others+0x124>
30006f3a:	2043      	movs	r0, #67	; 0x43
30006f3c:	f7ff ff0c 	bl	30006d58 <ymodem_inquiry>
30006f40:	682b      	ldr	r3, [r5, #0]
30006f42:	2e00      	cmp	r6, #0
30006f44:	f103 0301 	add.w	r3, r3, #1
30006f48:	bf18      	it	ne
30006f4a:	2404      	movne	r4, #4
30006f4c:	602b      	str	r3, [r5, #0]
30006f4e:	e775      	b.n	30006e3c <ymodem_get_others+0x20>
30006f50:	2c04      	cmp	r4, #4
30006f52:	f47f af73 	bne.w	30006e3c <ymodem_get_others+0x20>
30006f56:	e7b4      	b.n	30006ec2 <ymodem_get_others+0xa6>
30006f58:	000caa30 	.word	0x000caa30
30006f5c:	20016ffc 	.word	0x20016ffc
30006f60:	3000dc98 	.word	0x3000dc98

30006f64 <ymodem_rx_frame>:
30006f64:	2301      	movs	r3, #1
30006f66:	b510      	push	{r4, lr}
30006f68:	8083      	strh	r3, [r0, #4]
30006f6a:	2300      	movs	r3, #0
30006f6c:	4604      	mov	r4, r0
30006f6e:	6083      	str	r3, [r0, #8]
30006f70:	f7ff ff02 	bl	30006d78 <ymodem_get_first>
30006f74:	68a3      	ldr	r3, [r4, #8]
30006f76:	b133      	cbz	r3, 30006f86 <ymodem_rx_frame+0x22>
30006f78:	2801      	cmp	r0, #1
30006f7a:	bf01      	itttt	eq
30006f7c:	6823      	ldreq	r3, [r4, #0]
30006f7e:	3301      	addeq	r3, #1
30006f80:	b2db      	uxtbeq	r3, r3
30006f82:	6023      	streq	r3, [r4, #0]
30006f84:	bd10      	pop	{r4, pc}
30006f86:	4620      	mov	r0, r4
30006f88:	f7ff ff48 	bl	30006e1c <ymodem_get_others>
30006f8c:	e7f2      	b.n	30006f74 <ymodem_rx_frame+0x10>

30006f8e <ymodem_img_rxbuffer>:
30006f8e:	b570      	push	{r4, r5, r6, lr}
30006f90:	2300      	movs	r3, #0
30006f92:	4d1e      	ldr	r5, [pc, #120]	; (3000700c <ymodem_img_rxbuffer+0x7e>)
30006f94:	4e1e      	ldr	r6, [pc, #120]	; (30007010 <ymodem_img_rxbuffer+0x82>)
30006f96:	460c      	mov	r4, r1
30006f98:	60ab      	str	r3, [r5, #8]
30006f9a:	616b      	str	r3, [r5, #20]
30006f9c:	f000 f8c2 	bl	30007124 <ymodem_uart_clean_rx>
30006fa0:	2043      	movs	r0, #67	; 0x43
30006fa2:	f000 f88d 	bl	300070c0 <ymodem_uart_putc>
30006fa6:	f24c 3050 	movw	r0, #50000	; 0xc350
30006faa:	47b0      	blx	r6
30006fac:	f000 f872 	bl	30007094 <ymodem_uart_readable>
30006fb0:	2800      	cmp	r0, #0
30006fb2:	d0f5      	beq.n	30006fa0 <ymodem_img_rxbuffer+0x12>
30006fb4:	2300      	movs	r3, #0
30006fb6:	e9c5 3400 	strd	r3, r4, [r5]
30006fba:	4d16      	ldr	r5, [pc, #88]	; (30007014 <ymodem_img_rxbuffer+0x86>)
30006fbc:	4621      	mov	r1, r4
30006fbe:	4628      	mov	r0, r5
30006fc0:	f7ff ffd0 	bl	30006f64 <ymodem_rx_frame>
30006fc4:	3801      	subs	r0, #1
30006fc6:	2807      	cmp	r0, #7
30006fc8:	d818      	bhi.n	30006ffc <ymodem_img_rxbuffer+0x6e>
30006fca:	a301      	add	r3, pc, #4	; (adr r3, 30006fd0 <ymodem_img_rxbuffer+0x42>)
30006fcc:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30006fd0:	30006fbd 	.word	0x30006fbd
30006fd4:	30006ffd 	.word	0x30006ffd
30006fd8:	30006ffd 	.word	0x30006ffd
30006fdc:	30006fbd 	.word	0x30006fbd
30006fe0:	30007003 	.word	0x30007003
30006fe4:	30006ffd 	.word	0x30006ffd
30006fe8:	30006ffd 	.word	0x30006ffd
30006fec:	30006ff1 	.word	0x30006ff1
30006ff0:	f000 f898 	bl	30007124 <ymodem_uart_clean_rx>
30006ff4:	2015      	movs	r0, #21
30006ff6:	f7ff feaf 	bl	30006d58 <ymodem_inquiry>
30006ffa:	e7df      	b.n	30006fbc <ymodem_img_rxbuffer+0x2e>
30006ffc:	2018      	movs	r0, #24
30006ffe:	f7ff feab 	bl	30006d58 <ymodem_inquiry>
30007002:	200a      	movs	r0, #10
30007004:	f000 f898 	bl	30007138 <ymodem_uart_waitbusy>
30007008:	2001      	movs	r0, #1
3000700a:	bd70      	pop	{r4, r5, r6, pc}
3000700c:	3000dc98 	.word	0x3000dc98
30007010:	00009b2d 	.word	0x00009b2d
30007014:	3000dca0 	.word	0x3000dca0

30007018 <ymodem_log_open>:
30007018:	b510      	push	{r4, lr}
3000701a:	2303      	movs	r3, #3
3000701c:	4c04      	ldr	r4, [pc, #16]	; (30007030 <ymodem_log_open+0x18>)
3000701e:	2200      	movs	r2, #0
30007020:	2108      	movs	r1, #8
30007022:	4804      	ldr	r0, [pc, #16]	; (30007034 <ymodem_log_open+0x1c>)
30007024:	47a0      	blx	r4
30007026:	2008      	movs	r0, #8
30007028:	4b03      	ldr	r3, [pc, #12]	; (30007038 <ymodem_log_open+0x20>)
3000702a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000702e:	4718      	bx	r3
30007030:	30008fc9 	.word	0x30008fc9
30007034:	0000ed39 	.word	0x0000ed39
30007038:	0000e1bd 	.word	0x0000e1bd

3000703c <ymodem_uart_port_init>:
3000703c:	b507      	push	{r0, r1, r2, lr}
3000703e:	4b07      	ldr	r3, [pc, #28]	; (3000705c <ymodem_uart_port_init+0x20>)
30007040:	2008      	movs	r0, #8
30007042:	9201      	str	r2, [sp, #4]
30007044:	4798      	blx	r3
30007046:	4b06      	ldr	r3, [pc, #24]	; (30007060 <ymodem_uart_port_init+0x24>)
30007048:	2008      	movs	r0, #8
3000704a:	4798      	blx	r3
3000704c:	9901      	ldr	r1, [sp, #4]
3000704e:	4805      	ldr	r0, [pc, #20]	; (30007064 <ymodem_uart_port_init+0x28>)
30007050:	4b05      	ldr	r3, [pc, #20]	; (30007068 <ymodem_uart_port_init+0x2c>)
30007052:	b003      	add	sp, #12
30007054:	f85d eb04 	ldr.w	lr, [sp], #4
30007058:	4718      	bx	r3
3000705a:	bf00      	nop
3000705c:	0000e1d9 	.word	0x0000e1d9
30007060:	0000e2fd 	.word	0x0000e2fd
30007064:	4200c000 	.word	0x4200c000
30007068:	0000a9e5 	.word	0x0000a9e5

3000706c <ymodem_uart_port_deinit>:
3000706c:	2804      	cmp	r0, #4
3000706e:	b508      	push	{r3, lr}
30007070:	d101      	bne.n	30007076 <ymodem_uart_port_deinit+0xa>
30007072:	f7ff ffd1 	bl	30007018 <ymodem_log_open>
30007076:	4b05      	ldr	r3, [pc, #20]	; (3000708c <ymodem_uart_port_deinit+0x20>)
30007078:	681b      	ldr	r3, [r3, #0]
3000707a:	039b      	lsls	r3, r3, #14
3000707c:	d504      	bpl.n	30007088 <ymodem_uart_port_deinit+0x1c>
3000707e:	4804      	ldr	r0, [pc, #16]	; (30007090 <ymodem_uart_port_deinit+0x24>)
30007080:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
30007084:	f004 b9a0 	b.w	3000b3c8 <__DiagPrintf_veneer>
30007088:	bd08      	pop	{r3, pc}
3000708a:	bf00      	nop
3000708c:	2001c00c 	.word	0x2001c00c
30007090:	3000d286 	.word	0x3000d286

30007094 <ymodem_uart_readable>:
30007094:	4b00      	ldr	r3, [pc, #0]	; (30007098 <ymodem_uart_readable+0x4>)
30007096:	4718      	bx	r3
30007098:	0000ac39 	.word	0x0000ac39

3000709c <ymodem_uart_writable>:
3000709c:	b508      	push	{r3, lr}
3000709e:	4b03      	ldr	r3, [pc, #12]	; (300070ac <ymodem_uart_writable+0x10>)
300070a0:	6818      	ldr	r0, [r3, #0]
300070a2:	4b03      	ldr	r3, [pc, #12]	; (300070b0 <ymodem_uart_writable+0x14>)
300070a4:	4798      	blx	r3
300070a6:	b2c0      	uxtb	r0, r0
300070a8:	bd08      	pop	{r3, pc}
300070aa:	bf00      	nop
300070ac:	3000dcb0 	.word	0x3000dcb0
300070b0:	0000d1a9 	.word	0x0000d1a9

300070b4 <ymodem_uart_getc>:
300070b4:	2000      	movs	r0, #0
300070b6:	4b01      	ldr	r3, [pc, #4]	; (300070bc <ymodem_uart_getc+0x8>)
300070b8:	4718      	bx	r3
300070ba:	bf00      	nop
300070bc:	0000ac1d 	.word	0x0000ac1d

300070c0 <ymodem_uart_putc>:
300070c0:	4b00      	ldr	r3, [pc, #0]	; (300070c4 <ymodem_uart_putc+0x4>)
300070c2:	4718      	bx	r3
300070c4:	0000abb5 	.word	0x0000abb5

300070c8 <ymodem_uart_putdata>:
300070c8:	b570      	push	{r4, r5, r6, lr}
300070ca:	4604      	mov	r4, r0
300070cc:	1845      	adds	r5, r0, r1
300070ce:	4e04      	ldr	r6, [pc, #16]	; (300070e0 <ymodem_uart_putdata+0x18>)
300070d0:	42ac      	cmp	r4, r5
300070d2:	d100      	bne.n	300070d6 <ymodem_uart_putdata+0xe>
300070d4:	bd70      	pop	{r4, r5, r6, pc}
300070d6:	f814 0b01 	ldrb.w	r0, [r4], #1
300070da:	47b0      	blx	r6
300070dc:	e7f8      	b.n	300070d0 <ymodem_uart_putdata+0x8>
300070de:	bf00      	nop
300070e0:	0000abb5 	.word	0x0000abb5

300070e4 <ymodem_uart_getdata>:
300070e4:	b570      	push	{r4, r5, r6, lr}
300070e6:	4604      	mov	r4, r0
300070e8:	1845      	adds	r5, r0, r1
300070ea:	4e04      	ldr	r6, [pc, #16]	; (300070fc <ymodem_uart_getdata+0x18>)
300070ec:	42ac      	cmp	r4, r5
300070ee:	d100      	bne.n	300070f2 <ymodem_uart_getdata+0xe>
300070f0:	bd70      	pop	{r4, r5, r6, pc}
300070f2:	2001      	movs	r0, #1
300070f4:	47b0      	blx	r6
300070f6:	f804 0b01 	strb.w	r0, [r4], #1
300070fa:	e7f7      	b.n	300070ec <ymodem_uart_getdata+0x8>
300070fc:	0000ac1d 	.word	0x0000ac1d

30007100 <ymodem_uart_getc_to>:
30007100:	b570      	push	{r4, r5, r6, lr}
30007102:	4605      	mov	r5, r0
30007104:	460c      	mov	r4, r1
30007106:	4e06      	ldr	r6, [pc, #24]	; (30007120 <ymodem_uart_getc_to+0x20>)
30007108:	47b0      	blx	r6
3000710a:	b120      	cbz	r0, 30007116 <ymodem_uart_getc_to+0x16>
3000710c:	f7ff ffd2 	bl	300070b4 <ymodem_uart_getc>
30007110:	7028      	strb	r0, [r5, #0]
30007112:	2000      	movs	r0, #0
30007114:	bd70      	pop	{r4, r5, r6, pc}
30007116:	3c01      	subs	r4, #1
30007118:	d1f6      	bne.n	30007108 <ymodem_uart_getc_to+0x8>
3000711a:	2001      	movs	r0, #1
3000711c:	e7fa      	b.n	30007114 <ymodem_uart_getc_to+0x14>
3000711e:	bf00      	nop
30007120:	0000ac39 	.word	0x0000ac39

30007124 <ymodem_uart_clean_rx>:
30007124:	b510      	push	{r4, lr}
30007126:	4c03      	ldr	r4, [pc, #12]	; (30007134 <ymodem_uart_clean_rx+0x10>)
30007128:	47a0      	blx	r4
3000712a:	b900      	cbnz	r0, 3000712e <ymodem_uart_clean_rx+0xa>
3000712c:	bd10      	pop	{r4, pc}
3000712e:	f7ff ffc1 	bl	300070b4 <ymodem_uart_getc>
30007132:	e7f9      	b.n	30007128 <ymodem_uart_clean_rx+0x4>
30007134:	0000ac39 	.word	0x0000ac39

30007138 <ymodem_uart_waitbusy>:
30007138:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
3000713a:	4604      	mov	r4, r0
3000713c:	2500      	movs	r5, #0
3000713e:	4f05      	ldr	r7, [pc, #20]	; (30007154 <ymodem_uart_waitbusy+0x1c>)
30007140:	4e05      	ldr	r6, [pc, #20]	; (30007158 <ymodem_uart_waitbusy+0x20>)
30007142:	697b      	ldr	r3, [r7, #20]
30007144:	03db      	lsls	r3, r3, #15
30007146:	d404      	bmi.n	30007152 <ymodem_uart_waitbusy+0x1a>
30007148:	3501      	adds	r5, #1
3000714a:	2064      	movs	r0, #100	; 0x64
3000714c:	47b0      	blx	r6
3000714e:	42a5      	cmp	r5, r4
30007150:	d9f7      	bls.n	30007142 <ymodem_uart_waitbusy+0xa>
30007152:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007154:	4200c000 	.word	0x4200c000
30007158:	00009b2d 	.word	0x00009b2d

3000715c <UARTIMG_Write>:
3000715c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007160:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30007164:	4606      	mov	r6, r0
30007166:	f1b5 6f00 	cmp.w	r5, #134217728	; 0x8000000
3000716a:	d229      	bcs.n	300071c0 <UARTIMG_Write+0x64>
3000716c:	1e54      	subs	r4, r2, #1
3000716e:	4b15      	ldr	r3, [pc, #84]	; (300071c4 <UARTIMG_Write+0x68>)
30007170:	f8df 805c 	ldr.w	r8, [pc, #92]	; 300071d0 <UARTIMG_Write+0x74>
30007174:	442c      	add	r4, r5
30007176:	ea05 0703 	and.w	r7, r5, r3
3000717a:	401c      	ands	r4, r3
3000717c:	f8d8 3000 	ldr.w	r3, [r8]
30007180:	42bb      	cmp	r3, r7
30007182:	d807      	bhi.n	30007194 <UARTIMG_Write+0x38>
30007184:	4639      	mov	r1, r7
30007186:	f507 5780 	add.w	r7, r7, #4096	; 0x1000
3000718a:	2002      	movs	r0, #2
3000718c:	4b0e      	ldr	r3, [pc, #56]	; (300071c8 <UARTIMG_Write+0x6c>)
3000718e:	4798      	blx	r3
30007190:	f8c8 7000 	str.w	r7, [r8]
30007194:	f8d8 3000 	ldr.w	r3, [r8]
30007198:	42a3      	cmp	r3, r4
3000719a:	d807      	bhi.n	300071ac <UARTIMG_Write+0x50>
3000719c:	4621      	mov	r1, r4
3000719e:	f504 5480 	add.w	r4, r4, #4096	; 0x1000
300071a2:	2002      	movs	r0, #2
300071a4:	4b08      	ldr	r3, [pc, #32]	; (300071c8 <UARTIMG_Write+0x6c>)
300071a6:	4798      	blx	r3
300071a8:	f8c8 4000 	str.w	r4, [r8]
300071ac:	2400      	movs	r4, #0
300071ae:	4f07      	ldr	r7, [pc, #28]	; (300071cc <UARTIMG_Write+0x70>)
300071b0:	1932      	adds	r2, r6, r4
300071b2:	1928      	adds	r0, r5, r4
300071b4:	3408      	adds	r4, #8
300071b6:	2108      	movs	r1, #8
300071b8:	47b8      	blx	r7
300071ba:	f5b4 6f80 	cmp.w	r4, #1024	; 0x400
300071be:	d1f7      	bne.n	300071b0 <UARTIMG_Write+0x54>
300071c0:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300071c4:	fffff000 	.word	0xfffff000
300071c8:	0000a16d 	.word	0x0000a16d
300071cc:	0000a04d 	.word	0x0000a04d
300071d0:	3000dcb4 	.word	0x3000dcb4

300071d4 <ymodem_img_download>:
300071d4:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300071d8:	2400      	movs	r4, #0
300071da:	f8df 802c 	ldr.w	r8, [pc, #44]	; 30007208 <ymodem_img_download+0x34>
300071de:	460f      	mov	r7, r1
300071e0:	4616      	mov	r6, r2
300071e2:	2100      	movs	r1, #0
300071e4:	4a08      	ldr	r2, [pc, #32]	; (30007208 <ymodem_img_download+0x34>)
300071e6:	4605      	mov	r5, r0
300071e8:	f7ff ff28 	bl	3000703c <ymodem_uart_port_init>
300071ec:	42b4      	cmp	r4, r6
300071ee:	d104      	bne.n	300071fa <ymodem_img_download+0x26>
300071f0:	4628      	mov	r0, r5
300071f2:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300071f6:	f7ff bf39 	b.w	3000706c <ymodem_uart_port_deinit>
300071fa:	f857 1024 	ldr.w	r1, [r7, r4, lsl #2]
300071fe:	4640      	mov	r0, r8
30007200:	3401      	adds	r4, #1
30007202:	f7ff fec4 	bl	30006f8e <ymodem_img_rxbuffer>
30007206:	e7f1      	b.n	300071ec <ymodem_img_download+0x18>
30007208:	0016e360 	.word	0x0016e360

3000720c <ChipInfo_Invalid>:
3000720c:	4e06      	ldr	r6, [pc, #24]	; (30007228 <ChipInfo_Invalid+0x1c>)
3000720e:	4d07      	ldr	r5, [pc, #28]	; (3000722c <ChipInfo_Invalid+0x20>)
30007210:	4c07      	ldr	r4, [pc, #28]	; (30007230 <ChipInfo_Invalid+0x24>)
30007212:	b508      	push	{r3, lr}
30007214:	4633      	mov	r3, r6
30007216:	462a      	mov	r2, r5
30007218:	4903      	ldr	r1, [pc, #12]	; (30007228 <ChipInfo_Invalid+0x1c>)
3000721a:	2002      	movs	r0, #2
3000721c:	f003 fe7e 	bl	3000af1c <rtk_log_write>
30007220:	f242 7010 	movw	r0, #10000	; 0x2710
30007224:	47a0      	blx	r4
30007226:	e7f5      	b.n	30007214 <ChipInfo_Invalid+0x8>
30007228:	3000b448 	.word	0x3000b448
3000722c:	3000b451 	.word	0x3000b451
30007230:	00009be5 	.word	0x00009be5

30007234 <ChipInfo_Get>:
30007234:	b570      	push	{r4, r5, r6, lr}
30007236:	4c11      	ldr	r4, [pc, #68]	; (3000727c <ChipInfo_Get+0x48>)
30007238:	7820      	ldrb	r0, [r4, #0]
3000723a:	28ff      	cmp	r0, #255	; 0xff
3000723c:	d104      	bne.n	30007248 <ChipInfo_Get+0x14>
3000723e:	4d10      	ldr	r5, [pc, #64]	; (30007280 <ChipInfo_Get+0x4c>)
30007240:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007244:	b108      	cbz	r0, 3000724a <ChipInfo_Get+0x16>
30007246:	7020      	strb	r0, [r4, #0]
30007248:	bd70      	pop	{r4, r5, r6, pc}
3000724a:	4621      	mov	r1, r4
3000724c:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007250:	f000 fdbc 	bl	30007dcc <OTP_Read8>
30007254:	7820      	ldrb	r0, [r4, #0]
30007256:	28ff      	cmp	r0, #255	; 0xff
30007258:	d10c      	bne.n	30007274 <ChipInfo_Get+0x40>
3000725a:	4e0a      	ldr	r6, [pc, #40]	; (30007284 <ChipInfo_Get+0x50>)
3000725c:	4d0a      	ldr	r5, [pc, #40]	; (30007288 <ChipInfo_Get+0x54>)
3000725e:	4c0b      	ldr	r4, [pc, #44]	; (3000728c <ChipInfo_Get+0x58>)
30007260:	4633      	mov	r3, r6
30007262:	462a      	mov	r2, r5
30007264:	4907      	ldr	r1, [pc, #28]	; (30007284 <ChipInfo_Get+0x50>)
30007266:	2003      	movs	r0, #3
30007268:	f003 fe58 	bl	3000af1c <rtk_log_write>
3000726c:	f242 7010 	movw	r0, #10000	; 0x2710
30007270:	47a0      	blx	r4
30007272:	e7f5      	b.n	30007260 <ChipInfo_Get+0x2c>
30007274:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007278:	e7e6      	b.n	30007248 <ChipInfo_Get+0x14>
3000727a:	bf00      	nop
3000727c:	3000ddb0 	.word	0x3000ddb0
30007280:	23020000 	.word	0x23020000
30007284:	3000b448 	.word	0x3000b448
30007288:	3000b475 	.word	0x3000b475
3000728c:	00009be5 	.word	0x00009be5

30007290 <ChipInfo_MemoryType>:
30007290:	b510      	push	{r4, lr}
30007292:	4c10      	ldr	r4, [pc, #64]	; (300072d4 <ChipInfo_MemoryType+0x44>)
30007294:	47a0      	blx	r4
30007296:	2802      	cmp	r0, #2
30007298:	d01a      	beq.n	300072d0 <ChipInfo_MemoryType+0x40>
3000729a:	47a0      	blx	r4
3000729c:	2803      	cmp	r0, #3
3000729e:	d017      	beq.n	300072d0 <ChipInfo_MemoryType+0x40>
300072a0:	4b0d      	ldr	r3, [pc, #52]	; (300072d8 <ChipInfo_MemoryType+0x48>)
300072a2:	4798      	blx	r3
300072a4:	4b0d      	ldr	r3, [pc, #52]	; (300072dc <ChipInfo_MemoryType+0x4c>)
300072a6:	2200      	movs	r2, #0
300072a8:	4619      	mov	r1, r3
300072aa:	789c      	ldrb	r4, [r3, #2]
300072ac:	2cff      	cmp	r4, #255	; 0xff
300072ae:	d101      	bne.n	300072b4 <ChipInfo_MemoryType+0x24>
300072b0:	f7ff ffac 	bl	3000720c <ChipInfo_Invalid>
300072b4:	42a0      	cmp	r0, r4
300072b6:	f103 0306 	add.w	r3, r3, #6
300072ba:	d107      	bne.n	300072cc <ChipInfo_MemoryType+0x3c>
300072bc:	2306      	movs	r3, #6
300072be:	fb03 1202 	mla	r2, r3, r2, r1
300072c2:	78d3      	ldrb	r3, [r2, #3]
300072c4:	2b02      	cmp	r3, #2
300072c6:	d003      	beq.n	300072d0 <ChipInfo_MemoryType+0x40>
300072c8:	2001      	movs	r0, #1
300072ca:	bd10      	pop	{r4, pc}
300072cc:	3201      	adds	r2, #1
300072ce:	e7ec      	b.n	300072aa <ChipInfo_MemoryType+0x1a>
300072d0:	2002      	movs	r0, #2
300072d2:	e7fa      	b.n	300072ca <ChipInfo_MemoryType+0x3a>
300072d4:	0000c0f9 	.word	0x0000c0f9
300072d8:	30007235 	.word	0x30007235
300072dc:	3000b49e 	.word	0x3000b49e

300072e0 <ChipInfo_DDRType>:
300072e0:	b510      	push	{r4, lr}
300072e2:	4b0e      	ldr	r3, [pc, #56]	; (3000731c <ChipInfo_DDRType+0x3c>)
300072e4:	4798      	blx	r3
300072e6:	4b0e      	ldr	r3, [pc, #56]	; (30007320 <ChipInfo_DDRType+0x40>)
300072e8:	4604      	mov	r4, r0
300072ea:	4798      	blx	r3
300072ec:	2801      	cmp	r0, #1
300072ee:	d112      	bne.n	30007316 <ChipInfo_DDRType+0x36>
300072f0:	4b0c      	ldr	r3, [pc, #48]	; (30007324 <ChipInfo_DDRType+0x44>)
300072f2:	2200      	movs	r2, #0
300072f4:	4619      	mov	r1, r3
300072f6:	7898      	ldrb	r0, [r3, #2]
300072f8:	28ff      	cmp	r0, #255	; 0xff
300072fa:	d101      	bne.n	30007300 <ChipInfo_DDRType+0x20>
300072fc:	2000      	movs	r0, #0
300072fe:	e007      	b.n	30007310 <ChipInfo_DDRType+0x30>
30007300:	42a0      	cmp	r0, r4
30007302:	f103 0306 	add.w	r3, r3, #6
30007306:	d104      	bne.n	30007312 <ChipInfo_DDRType+0x32>
30007308:	2306      	movs	r3, #6
3000730a:	fb03 1202 	mla	r2, r3, r2, r1
3000730e:	78d0      	ldrb	r0, [r2, #3]
30007310:	bd10      	pop	{r4, pc}
30007312:	3201      	adds	r2, #1
30007314:	e7ef      	b.n	300072f6 <ChipInfo_DDRType+0x16>
30007316:	20ff      	movs	r0, #255	; 0xff
30007318:	e7fa      	b.n	30007310 <ChipInfo_DDRType+0x30>
3000731a:	bf00      	nop
3000731c:	30007235 	.word	0x30007235
30007320:	30007291 	.word	0x30007291
30007324:	3000b49e 	.word	0x3000b49e

30007328 <ChipInfo_MemorySize>:
30007328:	4b0a      	ldr	r3, [pc, #40]	; (30007354 <ChipInfo_MemorySize+0x2c>)
3000732a:	b510      	push	{r4, lr}
3000732c:	4798      	blx	r3
3000732e:	4b0a      	ldr	r3, [pc, #40]	; (30007358 <ChipInfo_MemorySize+0x30>)
30007330:	2200      	movs	r2, #0
30007332:	4619      	mov	r1, r3
30007334:	789c      	ldrb	r4, [r3, #2]
30007336:	2cff      	cmp	r4, #255	; 0xff
30007338:	d101      	bne.n	3000733e <ChipInfo_MemorySize+0x16>
3000733a:	f7ff ff67 	bl	3000720c <ChipInfo_Invalid>
3000733e:	4284      	cmp	r4, r0
30007340:	f103 0306 	add.w	r3, r3, #6
30007344:	d001      	beq.n	3000734a <ChipInfo_MemorySize+0x22>
30007346:	3201      	adds	r2, #1
30007348:	e7f4      	b.n	30007334 <ChipInfo_MemorySize+0xc>
3000734a:	2306      	movs	r3, #6
3000734c:	fb03 1202 	mla	r2, r3, r2, r1
30007350:	7910      	ldrb	r0, [r2, #4]
30007352:	bd10      	pop	{r4, pc}
30007354:	30007235 	.word	0x30007235
30007358:	3000b49e 	.word	0x3000b49e

3000735c <ChipInfo_MemoryVendor>:
3000735c:	b510      	push	{r4, lr}
3000735e:	4c0f      	ldr	r4, [pc, #60]	; (3000739c <ChipInfo_MemoryVendor+0x40>)
30007360:	47a0      	blx	r4
30007362:	2803      	cmp	r0, #3
30007364:	d017      	beq.n	30007396 <ChipInfo_MemoryVendor+0x3a>
30007366:	47a0      	blx	r4
30007368:	2802      	cmp	r0, #2
3000736a:	d011      	beq.n	30007390 <ChipInfo_MemoryVendor+0x34>
3000736c:	4b0c      	ldr	r3, [pc, #48]	; (300073a0 <ChipInfo_MemoryVendor+0x44>)
3000736e:	4798      	blx	r3
30007370:	4b0c      	ldr	r3, [pc, #48]	; (300073a4 <ChipInfo_MemoryVendor+0x48>)
30007372:	2200      	movs	r2, #0
30007374:	4619      	mov	r1, r3
30007376:	789c      	ldrb	r4, [r3, #2]
30007378:	2cff      	cmp	r4, #255	; 0xff
3000737a:	d101      	bne.n	30007380 <ChipInfo_MemoryVendor+0x24>
3000737c:	f7ff ff46 	bl	3000720c <ChipInfo_Invalid>
30007380:	4284      	cmp	r4, r0
30007382:	f103 0306 	add.w	r3, r3, #6
30007386:	d104      	bne.n	30007392 <ChipInfo_MemoryVendor+0x36>
30007388:	2306      	movs	r3, #6
3000738a:	fb03 1202 	mla	r2, r3, r2, r1
3000738e:	7950      	ldrb	r0, [r2, #5]
30007390:	bd10      	pop	{r4, pc}
30007392:	3201      	adds	r2, #1
30007394:	e7ef      	b.n	30007376 <ChipInfo_MemoryVendor+0x1a>
30007396:	2001      	movs	r0, #1
30007398:	e7fa      	b.n	30007390 <ChipInfo_MemoryVendor+0x34>
3000739a:	bf00      	nop
3000739c:	0000c0f9 	.word	0x0000c0f9
300073a0:	30007235 	.word	0x30007235
300073a4:	3000b49e 	.word	0x3000b49e

300073a8 <SDM32K_Enable>:
300073a8:	4a03      	ldr	r2, [pc, #12]	; (300073b8 <SDM32K_Enable+0x10>)
300073aa:	6813      	ldr	r3, [r2, #0]
300073ac:	f043 4340 	orr.w	r3, r3, #3221225472	; 0xc0000000
300073b0:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
300073b4:	6013      	str	r3, [r2, #0]
300073b6:	4770      	bx	lr
300073b8:	42008e00 	.word	0x42008e00

300073bc <CLK_SWITCH_XTAL>:
300073bc:	4a0b      	ldr	r2, [pc, #44]	; (300073ec <CLK_SWITCH_XTAL+0x30>)
300073be:	2801      	cmp	r0, #1
300073c0:	b508      	push	{r3, lr}
300073c2:	f8d2 3224 	ldr.w	r3, [r2, #548]	; 0x224
300073c6:	bf0b      	itete	eq
300073c8:	f023 4300 	biceq.w	r3, r3, #2147483648	; 0x80000000
300073cc:	f043 4300 	orrne.w	r3, r3, #2147483648	; 0x80000000
300073d0:	f423 63a0 	biceq.w	r3, r3, #1280	; 0x500
300073d4:	f443 63a0 	orrne.w	r3, r3, #1280	; 0x500
300073d8:	f8c2 3224 	str.w	r3, [r2, #548]	; 0x224
300073dc:	4b04      	ldr	r3, [pc, #16]	; (300073f0 <CLK_SWITCH_XTAL+0x34>)
300073de:	4798      	blx	r3
300073e0:	4b04      	ldr	r3, [pc, #16]	; (300073f4 <CLK_SWITCH_XTAL+0x38>)
300073e2:	fbb0 f0f3 	udiv	r0, r0, r3
300073e6:	4b04      	ldr	r3, [pc, #16]	; (300073f8 <CLK_SWITCH_XTAL+0x3c>)
300073e8:	6018      	str	r0, [r3, #0]
300073ea:	bd08      	pop	{r3, pc}
300073ec:	42008000 	.word	0x42008000
300073f0:	30008c5d 	.word	0x30008c5d
300073f4:	000f4240 	.word	0x000f4240
300073f8:	2001c700 	.word	0x2001c700

300073fc <DDR_PHY_ChipInfo>:
300073fc:	b510      	push	{r4, lr}
300073fe:	4c1a      	ldr	r4, [pc, #104]	; (30007468 <DDR_PHY_ChipInfo+0x6c>)
30007400:	7820      	ldrb	r0, [r4, #0]
30007402:	28ff      	cmp	r0, #255	; 0xff
30007404:	d116      	bne.n	30007434 <DDR_PHY_ChipInfo+0x38>
30007406:	4b19      	ldr	r3, [pc, #100]	; (3000746c <DDR_PHY_ChipInfo+0x70>)
30007408:	4798      	blx	r3
3000740a:	280e      	cmp	r0, #14
3000740c:	d80d      	bhi.n	3000742a <DDR_PHY_ChipInfo+0x2e>
3000740e:	2801      	cmp	r0, #1
30007410:	d921      	bls.n	30007456 <DDR_PHY_ChipInfo+0x5a>
30007412:	3802      	subs	r0, #2
30007414:	280c      	cmp	r0, #12
30007416:	d81e      	bhi.n	30007456 <DDR_PHY_ChipInfo+0x5a>
30007418:	e8df f000 	tbb	[pc, r0]
3000741c:	111d0f0d 	.word	0x111d0f0d
30007420:	131d171d 	.word	0x131d171d
30007424:	191d1d15 	.word	0x191d1d15
30007428:	1b          	.byte	0x1b
30007429:	00          	.byte	0x00
3000742a:	28e0      	cmp	r0, #224	; 0xe0
3000742c:	d113      	bne.n	30007456 <DDR_PHY_ChipInfo+0x5a>
3000742e:	2301      	movs	r3, #1
30007430:	7023      	strb	r3, [r4, #0]
30007432:	7820      	ldrb	r0, [r4, #0]
30007434:	bd10      	pop	{r4, pc}
30007436:	2302      	movs	r3, #2
30007438:	e7fa      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
3000743a:	2303      	movs	r3, #3
3000743c:	e7f8      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
3000743e:	2304      	movs	r3, #4
30007440:	e7f6      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
30007442:	2305      	movs	r3, #5
30007444:	e7f4      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
30007446:	2306      	movs	r3, #6
30007448:	e7f2      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
3000744a:	2307      	movs	r3, #7
3000744c:	e7f0      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
3000744e:	2308      	movs	r3, #8
30007450:	e7ee      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
30007452:	2309      	movs	r3, #9
30007454:	e7ec      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
30007456:	4b06      	ldr	r3, [pc, #24]	; (30007470 <DDR_PHY_ChipInfo+0x74>)
30007458:	2002      	movs	r0, #2
3000745a:	4a06      	ldr	r2, [pc, #24]	; (30007474 <DDR_PHY_ChipInfo+0x78>)
3000745c:	4619      	mov	r1, r3
3000745e:	f003 fd5d 	bl	3000af1c <rtk_log_write>
30007462:	2300      	movs	r3, #0
30007464:	e7e4      	b.n	30007430 <DDR_PHY_ChipInfo+0x34>
30007466:	bf00      	nop
30007468:	3000ddb1 	.word	0x3000ddb1
3000746c:	30007235 	.word	0x30007235
30007470:	3000b522 	.word	0x3000b522
30007474:	3000b529 	.word	0x3000b529

30007478 <DDR_PHY_ChipInfo_ddrtype>:
30007478:	b510      	push	{r4, lr}
3000747a:	4c04      	ldr	r4, [pc, #16]	; (3000748c <DDR_PHY_ChipInfo_ddrtype+0x14>)
3000747c:	7823      	ldrb	r3, [r4, #0]
3000747e:	b913      	cbnz	r3, 30007486 <DDR_PHY_ChipInfo_ddrtype+0xe>
30007480:	4b03      	ldr	r3, [pc, #12]	; (30007490 <DDR_PHY_ChipInfo_ddrtype+0x18>)
30007482:	4798      	blx	r3
30007484:	7020      	strb	r0, [r4, #0]
30007486:	7820      	ldrb	r0, [r4, #0]
30007488:	bd10      	pop	{r4, pc}
3000748a:	bf00      	nop
3000748c:	3000ead0 	.word	0x3000ead0
30007490:	300072e1 	.word	0x300072e1

30007494 <DDR_PHY_StructInit>:
30007494:	f240 2315 	movw	r3, #533	; 0x215
30007498:	6583      	str	r3, [r0, #88]	; 0x58
3000749a:	4770      	bx	lr

3000749c <DDR_PHY_DLL_CLK_DIV>:
3000749c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
3000749e:	2328      	movs	r3, #40	; 0x28
300074a0:	4a3f      	ldr	r2, [pc, #252]	; (300075a0 <DDR_PHY_DLL_CLK_DIV+0x104>)
300074a2:	4c40      	ldr	r4, [pc, #256]	; (300075a4 <DDR_PHY_DLL_CLK_DIV+0x108>)
300074a4:	4402      	add	r2, r0
300074a6:	4d40      	ldr	r5, [pc, #256]	; (300075a8 <DDR_PHY_DLL_CLK_DIV+0x10c>)
300074a8:	4f40      	ldr	r7, [pc, #256]	; (300075ac <DDR_PHY_DLL_CLK_DIV+0x110>)
300074aa:	fbb0 f1f3 	udiv	r1, r0, r3
300074ae:	3903      	subs	r1, #3
300074b0:	6a60      	ldr	r0, [r4, #36]	; 0x24
300074b2:	fb03 2211 	mls	r2, r3, r1, r2
300074b6:	f020 60ff 	bic.w	r0, r0, #133693440	; 0x7f80000
300074ba:	02d2      	lsls	r2, r2, #11
300074bc:	f420 20e0 	bic.w	r0, r0, #458752	; 0x70000
300074c0:	fbb2 f2f3 	udiv	r2, r2, r3
300074c4:	4b3a      	ldr	r3, [pc, #232]	; (300075b0 <DDR_PHY_DLL_CLK_DIV+0x114>)
300074c6:	ea03 4302 	and.w	r3, r3, r2, lsl #16
300074ca:	f3c2 020a 	ubfx	r2, r2, #0, #11
300074ce:	4303      	orrs	r3, r0
300074d0:	6263      	str	r3, [r4, #36]	; 0x24
300074d2:	020b      	lsls	r3, r1, #8
300074d4:	6aa0      	ldr	r0, [r4, #40]	; 0x28
300074d6:	b2c9      	uxtb	r1, r1
300074d8:	b29b      	uxth	r3, r3
300074da:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
300074de:	4303      	orrs	r3, r0
300074e0:	62a3      	str	r3, [r4, #40]	; 0x28
300074e2:	6a63      	ldr	r3, [r4, #36]	; 0x24
300074e4:	f423 63ff 	bic.w	r3, r3, #2040	; 0x7f8
300074e8:	f023 0307 	bic.w	r3, r3, #7
300074ec:	4313      	orrs	r3, r2
300074ee:	6263      	str	r3, [r4, #36]	; 0x24
300074f0:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300074f2:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
300074f6:	4319      	orrs	r1, r3
300074f8:	2301      	movs	r3, #1
300074fa:	62a1      	str	r1, [r4, #40]	; 0x28
300074fc:	67a3      	str	r3, [r4, #120]	; 0x78
300074fe:	f7ff ff7d 	bl	300073fc <DDR_PHY_ChipInfo>
30007502:	0146      	lsls	r6, r0, #5
30007504:	eb05 1040 	add.w	r0, r5, r0, lsl #5
30007508:	59ab      	ldr	r3, [r5, r6]
3000750a:	4435      	add	r5, r6
3000750c:	6123      	str	r3, [r4, #16]
3000750e:	6843      	ldr	r3, [r0, #4]
30007510:	6163      	str	r3, [r4, #20]
30007512:	6883      	ldr	r3, [r0, #8]
30007514:	f8c4 3098 	str.w	r3, [r4, #152]	; 0x98
30007518:	68c3      	ldr	r3, [r0, #12]
3000751a:	f8c4 30a0 	str.w	r3, [r4, #160]	; 0xa0
3000751e:	6903      	ldr	r3, [r0, #16]
30007520:	60a3      	str	r3, [r4, #8]
30007522:	f8d4 309c 	ldr.w	r3, [r4, #156]	; 0x9c
30007526:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
3000752a:	f8c4 309c 	str.w	r3, [r4, #156]	; 0x9c
3000752e:	47b8      	blx	r7
30007530:	2801      	cmp	r0, #1
30007532:	bf0c      	ite	eq
30007534:	2333      	moveq	r3, #51	; 0x33
30007536:	2322      	movne	r3, #34	; 0x22
30007538:	f8c4 3088 	str.w	r3, [r4, #136]	; 0x88
3000753c:	696b      	ldr	r3, [r5, #20]
3000753e:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
30007542:	69ab      	ldr	r3, [r5, #24]
30007544:	f8c4 3084 	str.w	r3, [r4, #132]	; 0x84
30007548:	69eb      	ldr	r3, [r5, #28]
3000754a:	f8c4 3094 	str.w	r3, [r4, #148]	; 0x94
3000754e:	f8d4 3084 	ldr.w	r3, [r4, #132]	; 0x84
30007552:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
30007556:	f8c4 3084 	str.w	r3, [r4, #132]	; 0x84
3000755a:	4b16      	ldr	r3, [pc, #88]	; (300075b4 <DDR_PHY_DLL_CLK_DIV+0x118>)
3000755c:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30007560:	009b      	lsls	r3, r3, #2
30007562:	d517      	bpl.n	30007594 <DDR_PHY_DLL_CLK_DIV+0xf8>
30007564:	47b8      	blx	r7
30007566:	2801      	cmp	r0, #1
30007568:	bf0c      	ite	eq
3000756a:	4b13      	ldreq	r3, [pc, #76]	; (300075b8 <DDR_PHY_DLL_CLK_DIV+0x11c>)
3000756c:	4b13      	ldrne	r3, [pc, #76]	; (300075bc <DDR_PHY_DLL_CLK_DIV+0x120>)
3000756e:	6023      	str	r3, [r4, #0]
30007570:	4b0c      	ldr	r3, [pc, #48]	; (300075a4 <DDR_PHY_DLL_CLK_DIV+0x108>)
30007572:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30007576:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
3000757a:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
3000757e:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30007582:	699a      	ldr	r2, [r3, #24]
30007584:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
30007588:	619a      	str	r2, [r3, #24]
3000758a:	699a      	ldr	r2, [r3, #24]
3000758c:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30007590:	619a      	str	r2, [r3, #24]
30007592:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007594:	47b8      	blx	r7
30007596:	2801      	cmp	r0, #1
30007598:	bf0c      	ite	eq
3000759a:	4b09      	ldreq	r3, [pc, #36]	; (300075c0 <DDR_PHY_DLL_CLK_DIV+0x124>)
3000759c:	4b09      	ldrne	r3, [pc, #36]	; (300075c4 <DDR_PHY_DLL_CLK_DIV+0x128>)
3000759e:	e7e6      	b.n	3000756e <DDR_PHY_DLL_CLK_DIV+0xd2>
300075a0:	001fff88 	.word	0x001fff88
300075a4:	41011000 	.word	0x41011000
300075a8:	3000b54c 	.word	0x3000b54c
300075ac:	30007479 	.word	0x30007479
300075b0:	07ff0000 	.word	0x07ff0000
300075b4:	42008000 	.word	0x42008000
300075b8:	4c000903 	.word	0x4c000903
300075bc:	4c000821 	.word	0x4c000821
300075c0:	40000103 	.word	0x40000103
300075c4:	40000021 	.word	0x40000021

300075c8 <DDR_PHY_CRT_Init>:
300075c8:	4a2f      	ldr	r2, [pc, #188]	; (30007688 <DDR_PHY_CRT_Init+0xc0>)
300075ca:	2005      	movs	r0, #5
300075cc:	b510      	push	{r4, lr}
300075ce:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
300075d2:	4c2e      	ldr	r4, [pc, #184]	; (3000768c <DDR_PHY_CRT_Init+0xc4>)
300075d4:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300075d8:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
300075dc:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300075e0:	f043 0301 	orr.w	r3, r3, #1
300075e4:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300075e8:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300075ec:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300075f0:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300075f4:	68e3      	ldr	r3, [r4, #12]
300075f6:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
300075fa:	60e3      	str	r3, [r4, #12]
300075fc:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007600:	f043 0302 	orr.w	r3, r3, #2
30007604:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007608:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000760c:	f023 0308 	bic.w	r3, r3, #8
30007610:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007614:	4b1e      	ldr	r3, [pc, #120]	; (30007690 <DDR_PHY_CRT_Init+0xc8>)
30007616:	4798      	blx	r3
30007618:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000761c:	f043 0304 	orr.w	r3, r3, #4
30007620:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007624:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007628:	f043 0308 	orr.w	r3, r3, #8
3000762c:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007630:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30007632:	f043 6380 	orr.w	r3, r3, #67108864	; 0x4000000
30007636:	62a3      	str	r3, [r4, #40]	; 0x28
30007638:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000763a:	f023 6380 	bic.w	r3, r3, #67108864	; 0x4000000
3000763e:	62a3      	str	r3, [r4, #40]	; 0x28
30007640:	69e3      	ldr	r3, [r4, #28]
30007642:	f043 0308 	orr.w	r3, r3, #8
30007646:	61e3      	str	r3, [r4, #28]
30007648:	68a3      	ldr	r3, [r4, #8]
3000764a:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
3000764e:	60a3      	str	r3, [r4, #8]
30007650:	6863      	ldr	r3, [r4, #4]
30007652:	ea6f 3353 	mvn.w	r3, r3, lsr #13
30007656:	ea6f 3343 	mvn.w	r3, r3, lsl #13
3000765a:	6063      	str	r3, [r4, #4]
3000765c:	68a3      	ldr	r3, [r4, #8]
3000765e:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
30007662:	60a3      	str	r3, [r4, #8]
30007664:	4b0b      	ldr	r3, [pc, #44]	; (30007694 <DDR_PHY_CRT_Init+0xcc>)
30007666:	f8c4 3240 	str.w	r3, [r4, #576]	; 0x240
3000766a:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000766e:	f023 0303 	bic.w	r3, r3, #3
30007672:	f043 0302 	orr.w	r3, r3, #2
30007676:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000767a:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000767e:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30007682:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007686:	bd10      	pop	{r4, pc}
30007688:	42008000 	.word	0x42008000
3000768c:	41011000 	.word	0x41011000
30007690:	00009b2d 	.word	0x00009b2d
30007694:	90161f00 	.word	0x90161f00

30007698 <DDR_PHY_R240_ZQ_CAL>:
30007698:	b538      	push	{r3, r4, r5, lr}
3000769a:	f7ff feaf 	bl	300073fc <DDR_PHY_ChipInfo>
3000769e:	2214      	movs	r2, #20
300076a0:	4d18      	ldr	r5, [pc, #96]	; (30007704 <DDR_PHY_R240_ZQ_CAL+0x6c>)
300076a2:	4350      	muls	r0, r2
300076a4:	4b18      	ldr	r3, [pc, #96]	; (30007708 <DDR_PHY_R240_ZQ_CAL+0x70>)
300076a6:	1829      	adds	r1, r5, r0
300076a8:	f8d3 414c 	ldr.w	r4, [r3, #332]	; 0x14c
300076ac:	7c0a      	ldrb	r2, [r1, #16]
300076ae:	f024 043e 	bic.w	r4, r4, #62	; 0x3e
300076b2:	0052      	lsls	r2, r2, #1
300076b4:	f002 023e 	and.w	r2, r2, #62	; 0x3e
300076b8:	4322      	orrs	r2, r4
300076ba:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
300076be:	582a      	ldr	r2, [r5, r0]
300076c0:	6848      	ldr	r0, [r1, #4]
300076c2:	f8c3 2180 	str.w	r2, [r3, #384]	; 0x180
300076c6:	2200      	movs	r2, #0
300076c8:	f8c3 2184 	str.w	r2, [r3, #388]	; 0x184
300076cc:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300076d0:	f8c3 218c 	str.w	r2, [r3, #396]	; 0x18c
300076d4:	688a      	ldr	r2, [r1, #8]
300076d6:	68c9      	ldr	r1, [r1, #12]
300076d8:	f8c3 2190 	str.w	r2, [r3, #400]	; 0x190
300076dc:	4a0b      	ldr	r2, [pc, #44]	; (3000770c <DDR_PHY_R240_ZQ_CAL+0x74>)
300076de:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300076e2:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
300076e6:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300076ea:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300076ee:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
300076f2:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300076f6:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300076fa:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
300076fe:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30007702:	bd38      	pop	{r3, r4, r5, pc}
30007704:	3000b68c 	.word	0x3000b68c
30007708:	41011000 	.word	0x41011000
3000770c:	00262626 	.word	0x00262626

30007710 <DDR_PHY_ZQ_SET_SEL>:
30007710:	4b23      	ldr	r3, [pc, #140]	; (300077a0 <DDR_PHY_ZQ_SET_SEL+0x90>)
30007712:	4a24      	ldr	r2, [pc, #144]	; (300077a4 <DDR_PHY_ZQ_SET_SEL+0x94>)
30007714:	4924      	ldr	r1, [pc, #144]	; (300077a8 <DDR_PHY_ZQ_SET_SEL+0x98>)
30007716:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
3000771a:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
3000771e:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
30007722:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
30007726:	f8c3 11d0 	str.w	r1, [r3, #464]	; 0x1d0
3000772a:	f8c3 11d4 	str.w	r1, [r3, #468]	; 0x1d4
3000772e:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
30007732:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
30007736:	f04f 3211 	mov.w	r2, #286331153	; 0x11111111
3000773a:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
3000773e:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30007742:	4a1a      	ldr	r2, [pc, #104]	; (300077ac <DDR_PHY_ZQ_SET_SEL+0x9c>)
30007744:	f8c3 21e8 	str.w	r2, [r3, #488]	; 0x1e8
30007748:	f8c3 21ec 	str.w	r2, [r3, #492]	; 0x1ec
3000774c:	2200      	movs	r2, #0
3000774e:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30007752:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30007756:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
3000775a:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
3000775e:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30007762:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30007766:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
3000776a:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
3000776e:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30007772:	f64f 72ff 	movw	r2, #65535	; 0xffff
30007776:	f8c3 2224 	str.w	r2, [r3, #548]	; 0x224
3000777a:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
3000777e:	f8c3 2228 	str.w	r2, [r3, #552]	; 0x228
30007782:	f8c3 222c 	str.w	r2, [r3, #556]	; 0x22c
30007786:	f8c3 2230 	str.w	r2, [r3, #560]	; 0x230
3000778a:	f8c3 2234 	str.w	r2, [r3, #564]	; 0x234
3000778e:	f8c3 2328 	str.w	r2, [r3, #808]	; 0x328
30007792:	f8c3 232c 	str.w	r2, [r3, #812]	; 0x32c
30007796:	f8c3 2330 	str.w	r2, [r3, #816]	; 0x330
3000779a:	f8c3 2334 	str.w	r2, [r3, #820]	; 0x334
3000779e:	4770      	bx	lr
300077a0:	41011000 	.word	0x41011000
300077a4:	ffff1111 	.word	0xffff1111
300077a8:	ffff3333 	.word	0xffff3333
300077ac:	ffff0000 	.word	0xffff0000

300077b0 <DDR_PHY_DQ_DQS_SEL_EMPH>:
300077b0:	4b09      	ldr	r3, [pc, #36]	; (300077d8 <DDR_PHY_DQ_DQS_SEL_EMPH+0x28>)
300077b2:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
300077b6:	f8c3 24f0 	str.w	r2, [r3, #1264]	; 0x4f0
300077ba:	f8c3 24f4 	str.w	r2, [r3, #1268]	; 0x4f4
300077be:	2200      	movs	r2, #0
300077c0:	f8c3 24e8 	str.w	r2, [r3, #1256]	; 0x4e8
300077c4:	f8c3 24ec 	str.w	r2, [r3, #1260]	; 0x4ec
300077c8:	4a04      	ldr	r2, [pc, #16]	; (300077dc <DDR_PHY_DQ_DQS_SEL_EMPH+0x2c>)
300077ca:	f8c3 217c 	str.w	r2, [r3, #380]	; 0x17c
300077ce:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300077d2:	f8c3 22a4 	str.w	r2, [r3, #676]	; 0x2a4
300077d6:	4770      	bx	lr
300077d8:	41011000 	.word	0x41011000
300077dc:	00045500 	.word	0x00045500

300077e0 <DDR_PHY_DELAY_TAP_SET>:
300077e0:	b538      	push	{r3, r4, r5, lr}
300077e2:	4c10      	ldr	r4, [pc, #64]	; (30007824 <DDR_PHY_DELAY_TAP_SET+0x44>)
300077e4:	f04f 3588 	mov.w	r5, #2290649224	; 0x88888888
300077e8:	4b0f      	ldr	r3, [pc, #60]	; (30007828 <DDR_PHY_DELAY_TAP_SET+0x48>)
300077ea:	f8c4 5100 	str.w	r5, [r4, #256]	; 0x100
300077ee:	f8c4 5104 	str.w	r5, [r4, #260]	; 0x104
300077f2:	f8c4 5108 	str.w	r5, [r4, #264]	; 0x108
300077f6:	f8c4 310c 	str.w	r3, [r4, #268]	; 0x10c
300077fa:	4b0c      	ldr	r3, [pc, #48]	; (3000782c <DDR_PHY_DELAY_TAP_SET+0x4c>)
300077fc:	f8c4 5110 	str.w	r5, [r4, #272]	; 0x110
30007800:	4798      	blx	r3
30007802:	2801      	cmp	r0, #1
30007804:	bf0c      	ite	eq
30007806:	4b0a      	ldreq	r3, [pc, #40]	; (30007830 <DDR_PHY_DELAY_TAP_SET+0x50>)
30007808:	4b0a      	ldrne	r3, [pc, #40]	; (30007834 <DDR_PHY_DELAY_TAP_SET+0x54>)
3000780a:	f8c4 3154 	str.w	r3, [r4, #340]	; 0x154
3000780e:	f640 0388 	movw	r3, #2184	; 0x888
30007812:	f8c4 5480 	str.w	r5, [r4, #1152]	; 0x480
30007816:	f8c4 5484 	str.w	r5, [r4, #1156]	; 0x484
3000781a:	f8c4 3488 	str.w	r3, [r4, #1160]	; 0x488
3000781e:	f8c4 348c 	str.w	r3, [r4, #1164]	; 0x48c
30007822:	bd38      	pop	{r3, r4, r5, pc}
30007824:	41011000 	.word	0x41011000
30007828:	20888888 	.word	0x20888888
3000782c:	30007479 	.word	0x30007479
30007830:	7c033333 	.word	0x7c033333
30007834:	7c011111 	.word	0x7c011111

30007838 <DDR_PHY_READ_CTRL>:
30007838:	b508      	push	{r3, lr}
3000783a:	f7ff fddf 	bl	300073fc <DDR_PHY_ChipInfo>
3000783e:	4b0c      	ldr	r3, [pc, #48]	; (30007870 <DDR_PHY_READ_CTRL+0x38>)
30007840:	2214      	movs	r2, #20
30007842:	fb02 3000 	mla	r0, r2, r0, r3
30007846:	4b0b      	ldr	r3, [pc, #44]	; (30007874 <DDR_PHY_READ_CTRL+0x3c>)
30007848:	7c42      	ldrb	r2, [r0, #17]
3000784a:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
3000784e:	7c82      	ldrb	r2, [r0, #18]
30007850:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
30007854:	7cc2      	ldrb	r2, [r0, #19]
30007856:	f8c3 2170 	str.w	r2, [r3, #368]	; 0x170
3000785a:	2208      	movs	r2, #8
3000785c:	f8c3 2408 	str.w	r2, [r3, #1032]	; 0x408
30007860:	f8c3 240c 	str.w	r2, [r3, #1036]	; 0x40c
30007864:	22fe      	movs	r2, #254	; 0xfe
30007866:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
3000786a:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
3000786e:	bd08      	pop	{r3, pc}
30007870:	3000b68c 	.word	0x3000b68c
30007874:	41011000 	.word	0x41011000

30007878 <DDR_PHY_3_POINT_CAL_RANGE>:
30007878:	4b07      	ldr	r3, [pc, #28]	; (30007898 <DDR_PHY_3_POINT_CAL_RANGE+0x20>)
3000787a:	4a08      	ldr	r2, [pc, #32]	; (3000789c <DDR_PHY_3_POINT_CAL_RANGE+0x24>)
3000787c:	f8c3 2178 	str.w	r2, [r3, #376]	; 0x178
30007880:	f04f 3222 	mov.w	r2, #572662306	; 0x22222222
30007884:	f8c3 2420 	str.w	r2, [r3, #1056]	; 0x420
30007888:	f8c3 2424 	str.w	r2, [r3, #1060]	; 0x424
3000788c:	f8c3 2430 	str.w	r2, [r3, #1072]	; 0x430
30007890:	f8c3 2434 	str.w	r2, [r3, #1076]	; 0x434
30007894:	4770      	bx	lr
30007896:	bf00      	nop
30007898:	41011000 	.word	0x41011000
3000789c:	11117777 	.word	0x11117777

300078a0 <DDR_PHY_READ_LEVELING>:
300078a0:	b508      	push	{r3, lr}
300078a2:	f7ff fdab 	bl	300073fc <DDR_PHY_ChipInfo>
300078a6:	4b1a      	ldr	r3, [pc, #104]	; (30007910 <DDR_PHY_READ_LEVELING+0x70>)
300078a8:	b1e8      	cbz	r0, 300078e6 <DDR_PHY_READ_LEVELING+0x46>
300078aa:	f04f 3212 	mov.w	r2, #303174162	; 0x12121212
300078ae:	f04f 3111 	mov.w	r1, #286331153	; 0x11111111
300078b2:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300078b6:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
300078ba:	f8c3 1444 	str.w	r1, [r3, #1092]	; 0x444
300078be:	f8c3 144c 	str.w	r1, [r3, #1100]	; 0x44c
300078c2:	f8c3 1458 	str.w	r1, [r3, #1112]	; 0x458
300078c6:	f8c3 1460 	str.w	r1, [r3, #1120]	; 0x460
300078ca:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
300078ce:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
300078d2:	220f      	movs	r2, #15
300078d4:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
300078d8:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
300078dc:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
300078e0:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
300078e4:	bd08      	pop	{r3, pc}
300078e6:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300078ea:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300078ee:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
300078f2:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
300078f6:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
300078fa:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
300078fe:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30007902:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30007906:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
3000790a:	2210      	movs	r2, #16
3000790c:	e7e2      	b.n	300078d4 <DDR_PHY_READ_LEVELING+0x34>
3000790e:	bf00      	nop
30007910:	41011000 	.word	0x41011000

30007914 <DDR_PHY_Init>:
30007914:	b508      	push	{r3, lr}
30007916:	6d80      	ldr	r0, [r0, #88]	; 0x58
30007918:	f7ff fdc0 	bl	3000749c <DDR_PHY_DLL_CLK_DIV>
3000791c:	f7ff fe54 	bl	300075c8 <DDR_PHY_CRT_Init>
30007920:	f7ff feba 	bl	30007698 <DDR_PHY_R240_ZQ_CAL>
30007924:	f7ff fef4 	bl	30007710 <DDR_PHY_ZQ_SET_SEL>
30007928:	f7ff ff42 	bl	300077b0 <DDR_PHY_DQ_DQS_SEL_EMPH>
3000792c:	f7ff ff58 	bl	300077e0 <DDR_PHY_DELAY_TAP_SET>
30007930:	f7ff ff82 	bl	30007838 <DDR_PHY_READ_CTRL>
30007934:	f7ff ffa0 	bl	30007878 <DDR_PHY_3_POINT_CAL_RANGE>
30007938:	f7ff ffb2 	bl	300078a0 <DDR_PHY_READ_LEVELING>
3000793c:	4b18      	ldr	r3, [pc, #96]	; (300079a0 <DDR_PHY_Init+0x8c>)
3000793e:	f8d3 21f8 	ldr.w	r2, [r3, #504]	; 0x1f8
30007942:	f422 427e 	bic.w	r2, r2, #65024	; 0xfe00
30007946:	f442 4264 	orr.w	r2, r2, #58368	; 0xe400
3000794a:	f8c3 21f8 	str.w	r2, [r3, #504]	; 0x1f8
3000794e:	f04f 32d8 	mov.w	r2, #3638089944	; 0xd8d8d8d8
30007952:	f8c3 21fc 	str.w	r2, [r3, #508]	; 0x1fc
30007956:	f8d3 22d0 	ldr.w	r2, [r3, #720]	; 0x2d0
3000795a:	f042 022f 	orr.w	r2, r2, #47	; 0x2f
3000795e:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
30007962:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30007966:	f8c3 21fc 	str.w	r2, [r3, #508]	; 0x1fc
3000796a:	f8d3 223c 	ldr.w	r2, [r3, #572]	; 0x23c
3000796e:	f042 0203 	orr.w	r2, r2, #3
30007972:	f8c3 223c 	str.w	r2, [r3, #572]	; 0x23c
30007976:	f8d3 223c 	ldr.w	r2, [r3, #572]	; 0x23c
3000797a:	f042 020c 	orr.w	r2, r2, #12
3000797e:	f8c3 223c 	str.w	r2, [r3, #572]	; 0x23c
30007982:	f8d3 2238 	ldr.w	r2, [r3, #568]	; 0x238
30007986:	f022 0203 	bic.w	r2, r2, #3
3000798a:	f8c3 2238 	str.w	r2, [r3, #568]	; 0x238
3000798e:	f8d3 2238 	ldr.w	r2, [r3, #568]	; 0x238
30007992:	f422 527c 	bic.w	r2, r2, #16128	; 0x3f00
30007996:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000799a:	f8c3 2238 	str.w	r2, [r3, #568]	; 0x238
3000799e:	bd08      	pop	{r3, pc}
300079a0:	41011000 	.word	0x41011000

300079a4 <DDR_PHY_CAL>:
300079a4:	4770      	bx	lr
	...

300079a8 <DDR_PHY_AutoGating>:
300079a8:	4a0c      	ldr	r2, [pc, #48]	; (300079dc <DDR_PHY_AutoGating+0x34>)
300079aa:	2032      	movs	r0, #50	; 0x32
300079ac:	b508      	push	{r3, lr}
300079ae:	6893      	ldr	r3, [r2, #8]
300079b0:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
300079b4:	6093      	str	r3, [r2, #8]
300079b6:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
300079ba:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
300079be:	f043 0314 	orr.w	r3, r3, #20
300079c2:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
300079c6:	4b06      	ldr	r3, [pc, #24]	; (300079e0 <DDR_PHY_AutoGating+0x38>)
300079c8:	4798      	blx	r3
300079ca:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300079ce:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
300079d2:	f043 0303 	orr.w	r3, r3, #3
300079d6:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
300079da:	bd08      	pop	{r3, pc}
300079dc:	41011000 	.word	0x41011000
300079e0:	00009b2d 	.word	0x00009b2d

300079e4 <flash_init_userdef>:
300079e4:	4b13      	ldr	r3, [pc, #76]	; (30007a34 <flash_init_userdef+0x50>)
300079e6:	2201      	movs	r2, #1
300079e8:	2140      	movs	r1, #64	; 0x40
300079ea:	20bb      	movs	r0, #187	; 0xbb
300079ec:	601a      	str	r2, [r3, #0]
300079ee:	f04f 0ceb 	mov.w	ip, #235	; 0xeb
300079f2:	e9c3 1203 	strd	r1, r2, [r3, #12]
300079f6:	2102      	movs	r1, #2
300079f8:	2200      	movs	r2, #0
300079fa:	e9c3 1205 	strd	r1, r2, [r3, #20]
300079fe:	f44f 6200 	mov.w	r2, #2048	; 0x800
30007a02:	490d      	ldr	r1, [pc, #52]	; (30007a38 <flash_init_userdef+0x54>)
30007a04:	83da      	strh	r2, [r3, #30]
30007a06:	f640 0204 	movw	r2, #2052	; 0x804
30007a0a:	841a      	strh	r2, [r3, #32]
30007a0c:	2206      	movs	r2, #6
30007a0e:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30007a12:	223b      	movs	r2, #59	; 0x3b
30007a14:	e9c3 2009 	strd	r2, r0, [r3, #36]	; 0x24
30007a18:	226b      	movs	r2, #107	; 0x6b
30007a1a:	e9c3 2c0b 	strd	r2, ip, [r3, #44]	; 0x2c
30007a1e:	4a07      	ldr	r2, [pc, #28]	; (30007a3c <flash_init_userdef+0x58>)
30007a20:	e9c3 1215 	strd	r1, r2, [r3, #84]	; 0x54
30007a24:	f64a 3220 	movw	r2, #43808	; 0xab20
30007a28:	f8a3 205c 	strh.w	r2, [r3, #92]	; 0x5c
30007a2c:	22b9      	movs	r2, #185	; 0xb9
30007a2e:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30007a32:	4770      	bx	lr
30007a34:	2001c01c 	.word	0x2001c01c
30007a38:	00059f06 	.word	0x00059f06
30007a3c:	d8600001 	.word	0xd8600001

30007a40 <nand_init_userdef>:
30007a40:	4b17      	ldr	r3, [pc, #92]	; (30007aa0 <nand_init_userdef+0x60>)
30007a42:	f640 0008 	movw	r0, #2056	; 0x808
30007a46:	2201      	movs	r2, #1
30007a48:	2102      	movs	r1, #2
30007a4a:	83d8      	strh	r0, [r3, #30]
30007a4c:	f44f 6000 	mov.w	r0, #2048	; 0x800
30007a50:	601a      	str	r2, [r3, #0]
30007a52:	8418      	strh	r0, [r3, #32]
30007a54:	203b      	movs	r0, #59	; 0x3b
30007a56:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30007a5a:	6258      	str	r0, [r3, #36]	; 0x24
30007a5c:	206b      	movs	r0, #107	; 0x6b
30007a5e:	e9c3 2203 	strd	r2, r2, [r3, #12]
30007a62:	2200      	movs	r2, #0
30007a64:	62d8      	str	r0, [r3, #44]	; 0x2c
30007a66:	200b      	movs	r0, #11
30007a68:	775a      	strb	r2, [r3, #29]
30007a6a:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30007a6e:	629a      	str	r2, [r3, #40]	; 0x28
30007a70:	631a      	str	r2, [r3, #48]	; 0x30
30007a72:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30007a76:	65da      	str	r2, [r3, #92]	; 0x5c
30007a78:	6698      	str	r0, [r3, #104]	; 0x68
30007a7a:	480a      	ldr	r0, [pc, #40]	; (30007aa4 <nand_init_userdef+0x64>)
30007a7c:	e9c3 1205 	strd	r1, r2, [r3, #20]
30007a80:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30007a84:	2108      	movs	r1, #8
30007a86:	e9c3 1213 	strd	r1, r2, [r3, #76]	; 0x4c
30007a8a:	f241 0213 	movw	r2, #4115	; 0x1013
30007a8e:	4906      	ldr	r1, [pc, #24]	; (30007aa8 <nand_init_userdef+0x68>)
30007a90:	f8a3 206e 	strh.w	r2, [r3, #110]	; 0x6e
30007a94:	4a05      	ldr	r2, [pc, #20]	; (30007aac <nand_init_userdef+0x6c>)
30007a96:	e9c3 0115 	strd	r0, r1, [r3, #84]	; 0x54
30007a9a:	671a      	str	r2, [r3, #112]	; 0x70
30007a9c:	4770      	bx	lr
30007a9e:	bf00      	nop
30007aa0:	2001c01c 	.word	0x2001c01c
30007aa4:	000f9f06 	.word	0x000f9f06
30007aa8:	d800001f 	.word	0xd800001f
30007aac:	10300804 	.word	0x10300804

30007ab0 <flash_get_layout_info>:
30007ab0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007ab2:	4b0c      	ldr	r3, [pc, #48]	; (30007ae4 <flash_get_layout_info+0x34>)
30007ab4:	4607      	mov	r7, r0
30007ab6:	460d      	mov	r5, r1
30007ab8:	4616      	mov	r6, r2
30007aba:	4c0b      	ldr	r4, [pc, #44]	; (30007ae8 <flash_get_layout_info+0x38>)
30007abc:	4798      	blx	r3
30007abe:	4b0b      	ldr	r3, [pc, #44]	; (30007aec <flash_get_layout_info+0x3c>)
30007ac0:	2800      	cmp	r0, #0
30007ac2:	bf08      	it	eq
30007ac4:	461c      	moveq	r4, r3
30007ac6:	6822      	ldr	r2, [r4, #0]
30007ac8:	4623      	mov	r3, r4
30007aca:	2aff      	cmp	r2, #255	; 0xff
30007acc:	d009      	beq.n	30007ae2 <flash_get_layout_info+0x32>
30007ace:	42ba      	cmp	r2, r7
30007ad0:	f103 040c 	add.w	r4, r3, #12
30007ad4:	d1f7      	bne.n	30007ac6 <flash_get_layout_info+0x16>
30007ad6:	b10d      	cbz	r5, 30007adc <flash_get_layout_info+0x2c>
30007ad8:	685a      	ldr	r2, [r3, #4]
30007ada:	602a      	str	r2, [r5, #0]
30007adc:	b10e      	cbz	r6, 30007ae2 <flash_get_layout_info+0x32>
30007ade:	689b      	ldr	r3, [r3, #8]
30007ae0:	6033      	str	r3, [r6, #0]
30007ae2:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007ae4:	30008ff5 	.word	0x30008ff5
30007ae8:	3000de8c 	.word	0x3000de8c
30007aec:	3000ddb4 	.word	0x3000ddb4

30007af0 <NAND_CHECK_IS_BAD_BLOCK>:
30007af0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007af4:	ea4f 1810 	mov.w	r8, r0, lsr #4
30007af8:	4606      	mov	r6, r0
30007afa:	4f1e      	ldr	r7, [pc, #120]	; (30007b74 <NAND_CHECK_IS_BAD_BLOCK+0x84>)
30007afc:	0040      	lsls	r0, r0, #1
30007afe:	b085      	sub	sp, #20
30007b00:	f000 051e 	and.w	r5, r0, #30
30007b04:	f857 3028 	ldr.w	r3, [r7, r8, lsl #2]
30007b08:	40eb      	lsrs	r3, r5
30007b0a:	f003 0303 	and.w	r3, r3, #3
30007b0e:	2b02      	cmp	r3, #2
30007b10:	d022      	beq.n	30007b58 <NAND_CHECK_IS_BAD_BLOCK+0x68>
30007b12:	2b01      	cmp	r3, #1
30007b14:	d02b      	beq.n	30007b6e <NAND_CHECK_IS_BAD_BLOCK+0x7e>
30007b16:	4b18      	ldr	r3, [pc, #96]	; (30007b78 <NAND_CHECK_IS_BAD_BLOCK+0x88>)
30007b18:	2401      	movs	r4, #1
30007b1a:	2204      	movs	r2, #4
30007b1c:	01b0      	lsls	r0, r6, #6
30007b1e:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30007b22:	ab03      	add	r3, sp, #12
30007b24:	f8df 905c 	ldr.w	r9, [pc, #92]	; 30007b84 <NAND_CHECK_IS_BAD_BLOCK+0x94>
30007b28:	fa04 f101 	lsl.w	r1, r4, r1
30007b2c:	47c8      	blx	r9
30007b2e:	f89d 300c 	ldrb.w	r3, [sp, #12]
30007b32:	f89d 200d 	ldrb.w	r2, [sp, #13]
30007b36:	4013      	ands	r3, r2
30007b38:	2bff      	cmp	r3, #255	; 0xff
30007b3a:	d011      	beq.n	30007b60 <NAND_CHECK_IS_BAD_BLOCK+0x70>
30007b3c:	2002      	movs	r0, #2
30007b3e:	f857 3028 	ldr.w	r3, [r7, r8, lsl #2]
30007b42:	4a0e      	ldr	r2, [pc, #56]	; (30007b7c <NAND_CHECK_IS_BAD_BLOCK+0x8c>)
30007b44:	fa00 f505 	lsl.w	r5, r0, r5
30007b48:	9600      	str	r6, [sp, #0]
30007b4a:	432b      	orrs	r3, r5
30007b4c:	f847 3028 	str.w	r3, [r7, r8, lsl #2]
30007b50:	4b0b      	ldr	r3, [pc, #44]	; (30007b80 <NAND_CHECK_IS_BAD_BLOCK+0x90>)
30007b52:	4619      	mov	r1, r3
30007b54:	f003 f9e2 	bl	3000af1c <rtk_log_write>
30007b58:	2001      	movs	r0, #1
30007b5a:	b005      	add	sp, #20
30007b5c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007b60:	fa04 f005 	lsl.w	r0, r4, r5
30007b64:	f857 4028 	ldr.w	r4, [r7, r8, lsl #2]
30007b68:	4304      	orrs	r4, r0
30007b6a:	f847 4028 	str.w	r4, [r7, r8, lsl #2]
30007b6e:	2000      	movs	r0, #0
30007b70:	e7f3      	b.n	30007b5a <NAND_CHECK_IS_BAD_BLOCK+0x6a>
30007b72:	bf00      	nop
30007b74:	3000ead8 	.word	0x3000ead8
30007b78:	2001c01c 	.word	0x2001c01c
30007b7c:	3000ba37 	.word	0x3000ba37
30007b80:	3000ba2d 	.word	0x3000ba2d
30007b84:	30008f35 	.word	0x30008f35

30007b88 <Nand_Get_NandAddr>:
30007b88:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007b8c:	4f0f      	ldr	r7, [pc, #60]	; (30007bcc <Nand_Get_NandAddr+0x44>)
30007b8e:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30007b92:	f100 4478 	add.w	r4, r0, #4160749568	; 0xf8000000
30007b96:	f04f 0840 	mov.w	r8, #64	; 0x40
30007b9a:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30007b9e:	40ec      	lsrs	r4, r5
30007ba0:	fa26 f505 	lsr.w	r5, r6, r5
30007ba4:	09a4      	lsrs	r4, r4, #6
30007ba6:	09ad      	lsrs	r5, r5, #6
30007ba8:	42a5      	cmp	r5, r4
30007baa:	d802      	bhi.n	30007bb2 <Nand_Get_NandAddr+0x2a>
30007bac:	4630      	mov	r0, r6
30007bae:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007bb2:	4620      	mov	r0, r4
30007bb4:	f7ff ff9c 	bl	30007af0 <NAND_CHECK_IS_BAD_BLOCK>
30007bb8:	b128      	cbz	r0, 30007bc6 <Nand_Get_NandAddr+0x3e>
30007bba:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007bbe:	3501      	adds	r5, #1
30007bc0:	fa08 f303 	lsl.w	r3, r8, r3
30007bc4:	441e      	add	r6, r3
30007bc6:	3401      	adds	r4, #1
30007bc8:	e7ee      	b.n	30007ba8 <Nand_Get_NandAddr+0x20>
30007bca:	bf00      	nop
30007bcc:	2001c01c 	.word	0x2001c01c

30007bd0 <Nand_L2P_Table>:
30007bd0:	b513      	push	{r0, r1, r4, lr}
30007bd2:	4604      	mov	r4, r0
30007bd4:	aa01      	add	r2, sp, #4
30007bd6:	2002      	movs	r0, #2
30007bd8:	4669      	mov	r1, sp
30007bda:	f7ff ff69 	bl	30007ab0 <flash_get_layout_info>
30007bde:	9800      	ldr	r0, [sp, #0]
30007be0:	42a0      	cmp	r0, r4
30007be2:	d906      	bls.n	30007bf2 <Nand_L2P_Table+0x22>
30007be4:	4621      	mov	r1, r4
30007be6:	f04f 6000 	mov.w	r0, #134217728	; 0x8000000
30007bea:	f7ff ffcd 	bl	30007b88 <Nand_Get_NandAddr>
30007bee:	b002      	add	sp, #8
30007bf0:	bd10      	pop	{r4, pc}
30007bf2:	9b01      	ldr	r3, [sp, #4]
30007bf4:	42a3      	cmp	r3, r4
30007bf6:	d901      	bls.n	30007bfc <Nand_L2P_Table+0x2c>
30007bf8:	4621      	mov	r1, r4
30007bfa:	e7f6      	b.n	30007bea <Nand_L2P_Table+0x1a>
30007bfc:	f104 4078 	add.w	r0, r4, #4160749568	; 0xf8000000
30007c00:	e7f5      	b.n	30007bee <Nand_L2P_Table+0x1e>
	...

30007c04 <NandImgCopy>:
30007c04:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007c08:	4690      	mov	r8, r2
30007c0a:	b087      	sub	sp, #28
30007c0c:	f8df a0c8 	ldr.w	sl, [pc, #200]	; 30007cd8 <NandImgCopy+0xd4>
30007c10:	f04f 0900 	mov.w	r9, #0
30007c14:	f108 34ff 	add.w	r4, r8, #4294967295	; 0xffffffff
30007c18:	9005      	str	r0, [sp, #20]
30007c1a:	4608      	mov	r0, r1
30007c1c:	46cb      	mov	fp, r9
30007c1e:	f7ff ffd7 	bl	30007bd0 <Nand_L2P_Table>
30007c22:	f89a 306c 	ldrb.w	r3, [sl, #108]	; 0x6c
30007c26:	4404      	add	r4, r0
30007c28:	4606      	mov	r6, r0
30007c2a:	40dc      	lsrs	r4, r3
30007c2c:	fa20 f303 	lsr.w	r3, r0, r3
30007c30:	1ae4      	subs	r4, r4, r3
30007c32:	1c63      	adds	r3, r4, #1
30007c34:	9304      	str	r3, [sp, #16]
30007c36:	9b04      	ldr	r3, [sp, #16]
30007c38:	b913      	cbnz	r3, 30007c40 <NandImgCopy+0x3c>
30007c3a:	b007      	add	sp, #28
30007c3c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007c40:	f89a 506c 	ldrb.w	r5, [sl, #108]	; 0x6c
30007c44:	fa26 f505 	lsr.w	r5, r6, r5
30007c48:	ebbb 1f95 	cmp.w	fp, r5, lsr #6
30007c4c:	ea4f 1795 	mov.w	r7, r5, lsr #6
30007c50:	d805      	bhi.n	30007c5e <NandImgCopy+0x5a>
30007c52:	4638      	mov	r0, r7
30007c54:	f107 0b01 	add.w	fp, r7, #1
30007c58:	f7ff ff4a 	bl	30007af0 <NAND_CHECK_IS_BAD_BLOCK>
30007c5c:	bb68      	cbnz	r0, 30007cba <NandImgCopy+0xb6>
30007c5e:	f89a 306c 	ldrb.w	r3, [sl, #108]	; 0x6c
30007c62:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30007c66:	4628      	mov	r0, r5
30007c68:	4c18      	ldr	r4, [pc, #96]	; (30007ccc <NandImgCopy+0xc8>)
30007c6a:	f1c3 0220 	rsb	r2, r3, #32
30007c6e:	40d1      	lsrs	r1, r2
30007c70:	9a04      	ldr	r2, [sp, #16]
30007c72:	2a01      	cmp	r2, #1
30007c74:	ea01 0106 	and.w	r1, r1, r6
30007c78:	bf16      	itet	ne
30007c7a:	2701      	movne	r7, #1
30007c7c:	4647      	moveq	r7, r8
30007c7e:	409f      	lslne	r7, r3
30007c80:	9b05      	ldr	r3, [sp, #20]
30007c82:	bf18      	it	ne
30007c84:	1a7f      	subne	r7, r7, r1
30007c86:	444b      	add	r3, r9
30007c88:	463a      	mov	r2, r7
30007c8a:	47a0      	blx	r4
30007c8c:	f010 0f70 	tst.w	r0, #112	; 0x70
30007c90:	d00b      	beq.n	30007caa <NandImgCopy+0xa6>
30007c92:	09ab      	lsrs	r3, r5, #6
30007c94:	f005 053f 	and.w	r5, r5, #63	; 0x3f
30007c98:	9002      	str	r0, [sp, #8]
30007c9a:	2004      	movs	r0, #4
30007c9c:	9301      	str	r3, [sp, #4]
30007c9e:	4b0c      	ldr	r3, [pc, #48]	; (30007cd0 <NandImgCopy+0xcc>)
30007ca0:	4a0c      	ldr	r2, [pc, #48]	; (30007cd4 <NandImgCopy+0xd0>)
30007ca2:	4619      	mov	r1, r3
30007ca4:	9500      	str	r5, [sp, #0]
30007ca6:	f003 f939 	bl	3000af1c <rtk_log_write>
30007caa:	9b04      	ldr	r3, [sp, #16]
30007cac:	44b9      	add	r9, r7
30007cae:	443e      	add	r6, r7
30007cb0:	eba8 0807 	sub.w	r8, r8, r7
30007cb4:	3b01      	subs	r3, #1
30007cb6:	9304      	str	r3, [sp, #16]
30007cb8:	e7bd      	b.n	30007c36 <NandImgCopy+0x32>
30007cba:	f89a 306c 	ldrb.w	r3, [sl, #108]	; 0x6c
30007cbe:	2240      	movs	r2, #64	; 0x40
30007cc0:	3540      	adds	r5, #64	; 0x40
30007cc2:	465f      	mov	r7, fp
30007cc4:	fa02 f303 	lsl.w	r3, r2, r3
30007cc8:	441e      	add	r6, r3
30007cca:	e7c2      	b.n	30007c52 <NandImgCopy+0x4e>
30007ccc:	30008f35 	.word	0x30008f35
30007cd0:	3000ba2d 	.word	0x3000ba2d
30007cd4:	3000ba4f 	.word	0x3000ba4f
30007cd8:	2001c01c 	.word	0x2001c01c

30007cdc <OTPPowerSwitch.constprop.0>:
30007cdc:	4b09      	ldr	r3, [pc, #36]	; (30007d04 <OTPPowerSwitch.constprop.0+0x28>)
30007cde:	681a      	ldr	r2, [r3, #0]
30007ce0:	07d2      	lsls	r2, r2, #31
30007ce2:	bf5e      	ittt	pl
30007ce4:	681a      	ldrpl	r2, [r3, #0]
30007ce6:	f042 0201 	orrpl.w	r2, r2, #1
30007cea:	601a      	strpl	r2, [r3, #0]
30007cec:	f04f 4384 	mov.w	r3, #1107296256	; 0x42000000
30007cf0:	695a      	ldr	r2, [r3, #20]
30007cf2:	b118      	cbz	r0, 30007cfc <OTPPowerSwitch.constprop.0+0x20>
30007cf4:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
30007cf8:	615a      	str	r2, [r3, #20]
30007cfa:	4770      	bx	lr
30007cfc:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30007d00:	e7fa      	b.n	30007cf8 <OTPPowerSwitch.constprop.0+0x1c>
30007d02:	bf00      	nop
30007d04:	42008000 	.word	0x42008000

30007d08 <OTP_ProgramMarginRead8.part.0>:
30007d08:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007d0c:	4b29      	ldr	r3, [pc, #164]	; (30007db4 <OTP_ProgramMarginRead8.part.0+0xac>)
30007d0e:	4607      	mov	r7, r0
30007d10:	e843 f300 	tt	r3, r3
30007d14:	f3c3 5380 	ubfx	r3, r3, #22, #1
30007d18:	4e27      	ldr	r6, [pc, #156]	; (30007db8 <OTP_ProgramMarginRead8.part.0+0xb0>)
30007d1a:	4688      	mov	r8, r1
30007d1c:	f8df 90a8 	ldr.w	r9, [pc, #168]	; 30007dc8 <OTP_ProgramMarginRead8.part.0+0xc0>
30007d20:	2b00      	cmp	r3, #0
30007d22:	4b26      	ldr	r3, [pc, #152]	; (30007dbc <OTP_ProgramMarginRead8.part.0+0xb4>)
30007d24:	bf0e      	itee	eq
30007d26:	f04f 4584 	moveq.w	r5, #1107296256	; 0x42000000
30007d2a:	461e      	movne	r6, r3
30007d2c:	f04f 45a4 	movne.w	r5, #1375731712	; 0x52000000
30007d30:	6c2c      	ldr	r4, [r5, #64]	; 0x40
30007d32:	f414 7480 	ands.w	r4, r4, #256	; 0x100
30007d36:	d11f      	bne.n	30007d78 <OTP_ProgramMarginRead8.part.0+0x70>
30007d38:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007d3a:	4620      	mov	r0, r4
30007d3c:	46a1      	mov	r9, r4
30007d3e:	f644 6a20 	movw	sl, #20000	; 0x4e20
30007d42:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007d46:	f8df b080 	ldr.w	fp, [pc, #128]	; 30007dc8 <OTP_ProgramMarginRead8.part.0+0xc0>
30007d4a:	642b      	str	r3, [r5, #64]	; 0x40
30007d4c:	f7ff ffc6 	bl	30007cdc <OTPPowerSwitch.constprop.0>
30007d50:	023b      	lsls	r3, r7, #8
30007d52:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
30007d56:	6033      	str	r3, [r6, #0]
30007d58:	6833      	ldr	r3, [r6, #0]
30007d5a:	2b00      	cmp	r3, #0
30007d5c:	db14      	blt.n	30007d88 <OTP_ProgramMarginRead8.part.0+0x80>
30007d5e:	45d1      	cmp	r9, sl
30007d60:	d10d      	bne.n	30007d7e <OTP_ProgramMarginRead8.part.0+0x76>
30007d62:	23ff      	movs	r3, #255	; 0xff
30007d64:	4a16      	ldr	r2, [pc, #88]	; (30007dc0 <OTP_ProgramMarginRead8.part.0+0xb8>)
30007d66:	2002      	movs	r0, #2
30007d68:	9700      	str	r7, [sp, #0]
30007d6a:	f888 3000 	strb.w	r3, [r8]
30007d6e:	4b15      	ldr	r3, [pc, #84]	; (30007dc4 <OTP_ProgramMarginRead8.part.0+0xbc>)
30007d70:	4619      	mov	r1, r3
30007d72:	f003 f8d3 	bl	3000af1c <rtk_log_write>
30007d76:	e00d      	b.n	30007d94 <OTP_ProgramMarginRead8.part.0+0x8c>
30007d78:	2064      	movs	r0, #100	; 0x64
30007d7a:	47c8      	blx	r9
30007d7c:	e7d8      	b.n	30007d30 <OTP_ProgramMarginRead8.part.0+0x28>
30007d7e:	2005      	movs	r0, #5
30007d80:	f109 0901 	add.w	r9, r9, #1
30007d84:	47d8      	blx	fp
30007d86:	e7e7      	b.n	30007d58 <OTP_ProgramMarginRead8.part.0+0x50>
30007d88:	45d1      	cmp	r9, sl
30007d8a:	d0ea      	beq.n	30007d62 <OTP_ProgramMarginRead8.part.0+0x5a>
30007d8c:	6833      	ldr	r3, [r6, #0]
30007d8e:	2401      	movs	r4, #1
30007d90:	f888 3000 	strb.w	r3, [r8]
30007d94:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30007d98:	f004 0001 	and.w	r0, r4, #1
30007d9c:	6953      	ldr	r3, [r2, #20]
30007d9e:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30007da2:	6153      	str	r3, [r2, #20]
30007da4:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007da6:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30007daa:	642b      	str	r3, [r5, #64]	; 0x40
30007dac:	b003      	add	sp, #12
30007dae:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007db2:	bf00      	nop
30007db4:	0000e5f9 	.word	0x0000e5f9
30007db8:	42000008 	.word	0x42000008
30007dbc:	5200000c 	.word	0x5200000c
30007dc0:	3000ba8f 	.word	0x3000ba8f
30007dc4:	3000ba8b 	.word	0x3000ba8b
30007dc8:	00009b2d 	.word	0x00009b2d

30007dcc <OTP_Read8>:
30007dcc:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30007dd0:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007dd4:	4606      	mov	r6, r0
30007dd6:	460f      	mov	r7, r1
30007dd8:	d306      	bcc.n	30007de8 <OTP_Read8+0x1c>
30007dda:	23ff      	movs	r3, #255	; 0xff
30007ddc:	2400      	movs	r4, #0
30007dde:	700b      	strb	r3, [r1, #0]
30007de0:	4620      	mov	r0, r4
30007de2:	b003      	add	sp, #12
30007de4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007de8:	4b26      	ldr	r3, [pc, #152]	; (30007e84 <OTP_Read8+0xb8>)
30007dea:	e843 f300 	tt	r3, r3
30007dee:	f3c3 5380 	ubfx	r3, r3, #22, #1
30007df2:	4a25      	ldr	r2, [pc, #148]	; (30007e88 <OTP_Read8+0xbc>)
30007df4:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 30007e98 <OTP_Read8+0xcc>
30007df8:	2b00      	cmp	r3, #0
30007dfa:	4b24      	ldr	r3, [pc, #144]	; (30007e8c <OTP_Read8+0xc0>)
30007dfc:	bf0b      	itete	eq
30007dfe:	4690      	moveq	r8, r2
30007e00:	4698      	movne	r8, r3
30007e02:	f04f 4584 	moveq.w	r5, #1107296256	; 0x42000000
30007e06:	f04f 45a4 	movne.w	r5, #1375731712	; 0x52000000
30007e0a:	6c2c      	ldr	r4, [r5, #64]	; 0x40
30007e0c:	f414 7480 	ands.w	r4, r4, #256	; 0x100
30007e10:	d11e      	bne.n	30007e50 <OTP_Read8+0x84>
30007e12:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007e14:	4620      	mov	r0, r4
30007e16:	46a1      	mov	r9, r4
30007e18:	f644 6a20 	movw	sl, #20000	; 0x4e20
30007e1c:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007e20:	f8df b074 	ldr.w	fp, [pc, #116]	; 30007e98 <OTP_Read8+0xcc>
30007e24:	642b      	str	r3, [r5, #64]	; 0x40
30007e26:	f7ff ff59 	bl	30007cdc <OTPPowerSwitch.constprop.0>
30007e2a:	0233      	lsls	r3, r6, #8
30007e2c:	f8c8 3000 	str.w	r3, [r8]
30007e30:	f8d8 3000 	ldr.w	r3, [r8]
30007e34:	2b00      	cmp	r3, #0
30007e36:	db13      	blt.n	30007e60 <OTP_Read8+0x94>
30007e38:	45d1      	cmp	r9, sl
30007e3a:	d10c      	bne.n	30007e56 <OTP_Read8+0x8a>
30007e3c:	23ff      	movs	r3, #255	; 0xff
30007e3e:	4a14      	ldr	r2, [pc, #80]	; (30007e90 <OTP_Read8+0xc4>)
30007e40:	2002      	movs	r0, #2
30007e42:	9600      	str	r6, [sp, #0]
30007e44:	703b      	strb	r3, [r7, #0]
30007e46:	4b13      	ldr	r3, [pc, #76]	; (30007e94 <OTP_Read8+0xc8>)
30007e48:	4619      	mov	r1, r3
30007e4a:	f003 f867 	bl	3000af1c <rtk_log_write>
30007e4e:	e00d      	b.n	30007e6c <OTP_Read8+0xa0>
30007e50:	2064      	movs	r0, #100	; 0x64
30007e52:	47c8      	blx	r9
30007e54:	e7d9      	b.n	30007e0a <OTP_Read8+0x3e>
30007e56:	2005      	movs	r0, #5
30007e58:	f109 0901 	add.w	r9, r9, #1
30007e5c:	47d8      	blx	fp
30007e5e:	e7e7      	b.n	30007e30 <OTP_Read8+0x64>
30007e60:	45d1      	cmp	r9, sl
30007e62:	d0eb      	beq.n	30007e3c <OTP_Read8+0x70>
30007e64:	f8d8 3000 	ldr.w	r3, [r8]
30007e68:	2401      	movs	r4, #1
30007e6a:	703b      	strb	r3, [r7, #0]
30007e6c:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30007e70:	6953      	ldr	r3, [r2, #20]
30007e72:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30007e76:	6153      	str	r3, [r2, #20]
30007e78:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007e7a:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30007e7e:	642b      	str	r3, [r5, #64]	; 0x40
30007e80:	e7ae      	b.n	30007de0 <OTP_Read8+0x14>
30007e82:	bf00      	nop
30007e84:	0000e5f9 	.word	0x0000e5f9
30007e88:	42000008 	.word	0x42000008
30007e8c:	5200000c 	.word	0x5200000c
30007e90:	3000ba8f 	.word	0x3000ba8f
30007e94:	3000ba8b 	.word	0x3000ba8b
30007e98:	00009b2d 	.word	0x00009b2d

30007e9c <OTP_Write8>:
30007e9c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30007ea0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007ea4:	4605      	mov	r5, r0
30007ea6:	b087      	sub	sp, #28
30007ea8:	4688      	mov	r8, r1
30007eaa:	d30a      	bcc.n	30007ec2 <OTP_Write8+0x26>
30007eac:	23ff      	movs	r3, #255	; 0xff
30007eae:	f88d 3017 	strb.w	r3, [sp, #23]
30007eb2:	4b42      	ldr	r3, [pc, #264]	; (30007fbc <OTP_Write8+0x120>)
30007eb4:	2002      	movs	r0, #2
30007eb6:	4a42      	ldr	r2, [pc, #264]	; (30007fc0 <OTP_Write8+0x124>)
30007eb8:	4619      	mov	r1, r3
30007eba:	f003 f82f 	bl	3000af1c <rtk_log_write>
30007ebe:	2400      	movs	r4, #0
30007ec0:	e054      	b.n	30007f6c <OTP_Write8+0xd0>
30007ec2:	f10d 0117 	add.w	r1, sp, #23
30007ec6:	f7ff ff1f 	bl	30007d08 <OTP_ProgramMarginRead8.part.0>
30007eca:	2800      	cmp	r0, #0
30007ecc:	d0f1      	beq.n	30007eb2 <OTP_Write8+0x16>
30007ece:	022b      	lsls	r3, r5, #8
30007ed0:	4647      	mov	r7, r8
30007ed2:	f04f 0a02 	mov.w	sl, #2
30007ed6:	f8df b104 	ldr.w	fp, [pc, #260]	; 30007fdc <OTP_Write8+0x140>
30007eda:	9302      	str	r3, [sp, #8]
30007edc:	f89d 3017 	ldrb.w	r3, [sp, #23]
30007ee0:	ea67 0703 	orn	r7, r7, r3
30007ee4:	b2ff      	uxtb	r7, r7
30007ee6:	2fff      	cmp	r7, #255	; 0xff
30007ee8:	d05d      	beq.n	30007fa6 <OTP_Write8+0x10a>
30007eea:	4b36      	ldr	r3, [pc, #216]	; (30007fc4 <OTP_Write8+0x128>)
30007eec:	e843 f300 	tt	r3, r3
30007ef0:	f3c3 5380 	ubfx	r3, r3, #22, #1
30007ef4:	4a34      	ldr	r2, [pc, #208]	; (30007fc8 <OTP_Write8+0x12c>)
30007ef6:	2b00      	cmp	r3, #0
30007ef8:	4b34      	ldr	r3, [pc, #208]	; (30007fcc <OTP_Write8+0x130>)
30007efa:	bf0b      	itete	eq
30007efc:	4691      	moveq	r9, r2
30007efe:	4699      	movne	r9, r3
30007f00:	f04f 4684 	moveq.w	r6, #1107296256	; 0x42000000
30007f04:	f04f 46a4 	movne.w	r6, #1375731712	; 0x52000000
30007f08:	6c34      	ldr	r4, [r6, #64]	; 0x40
30007f0a:	f414 7480 	ands.w	r4, r4, #256	; 0x100
30007f0e:	d131      	bne.n	30007f74 <OTP_Write8+0xd8>
30007f10:	6c33      	ldr	r3, [r6, #64]	; 0x40
30007f12:	2001      	movs	r0, #1
30007f14:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007f18:	6433      	str	r3, [r6, #64]	; 0x40
30007f1a:	f7ff fedf 	bl	30007cdc <OTPPowerSwitch.constprop.0>
30007f1e:	9b02      	ldr	r3, [sp, #8]
30007f20:	433b      	orrs	r3, r7
30007f22:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
30007f26:	f8c9 3000 	str.w	r3, [r9]
30007f2a:	4623      	mov	r3, r4
30007f2c:	f8d9 1000 	ldr.w	r1, [r9]
30007f30:	f644 6220 	movw	r2, #20000	; 0x4e20
30007f34:	2900      	cmp	r1, #0
30007f36:	da26      	bge.n	30007f86 <OTP_Write8+0xea>
30007f38:	4293      	cmp	r3, r2
30007f3a:	d11e      	bne.n	30007f7a <OTP_Write8+0xde>
30007f3c:	4b1f      	ldr	r3, [pc, #124]	; (30007fbc <OTP_Write8+0x120>)
30007f3e:	2003      	movs	r0, #3
30007f40:	4a23      	ldr	r2, [pc, #140]	; (30007fd0 <OTP_Write8+0x134>)
30007f42:	4619      	mov	r1, r3
30007f44:	9500      	str	r5, [sp, #0]
30007f46:	f002 ffe9 	bl	3000af1c <rtk_log_write>
30007f4a:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30007f4e:	6953      	ldr	r3, [r2, #20]
30007f50:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30007f54:	6153      	str	r3, [r2, #20]
30007f56:	6c33      	ldr	r3, [r6, #64]	; 0x40
30007f58:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30007f5c:	6433      	str	r3, [r6, #64]	; 0x40
30007f5e:	bb14      	cbnz	r4, 30007fa6 <OTP_Write8+0x10a>
30007f60:	4b16      	ldr	r3, [pc, #88]	; (30007fbc <OTP_Write8+0x120>)
30007f62:	4a1c      	ldr	r2, [pc, #112]	; (30007fd4 <OTP_Write8+0x138>)
30007f64:	4619      	mov	r1, r3
30007f66:	2002      	movs	r0, #2
30007f68:	f002 ffd8 	bl	3000af1c <rtk_log_write>
30007f6c:	4620      	mov	r0, r4
30007f6e:	b007      	add	sp, #28
30007f70:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007f74:	2064      	movs	r0, #100	; 0x64
30007f76:	47d8      	blx	fp
30007f78:	e7c6      	b.n	30007f08 <OTP_Write8+0x6c>
30007f7a:	2005      	movs	r0, #5
30007f7c:	9303      	str	r3, [sp, #12]
30007f7e:	47d8      	blx	fp
30007f80:	9b03      	ldr	r3, [sp, #12]
30007f82:	3301      	adds	r3, #1
30007f84:	e7d2      	b.n	30007f2c <OTP_Write8+0x90>
30007f86:	4293      	cmp	r3, r2
30007f88:	d0d8      	beq.n	30007f3c <OTP_Write8+0xa0>
30007f8a:	2401      	movs	r4, #1
30007f8c:	e7dd      	b.n	30007f4a <OTP_Write8+0xae>
30007f8e:	f89d 3017 	ldrb.w	r3, [sp, #23]
30007f92:	4543      	cmp	r3, r8
30007f94:	d005      	beq.n	30007fa2 <OTP_Write8+0x106>
30007f96:	f1ba 0f01 	cmp.w	sl, #1
30007f9a:	d090      	beq.n	30007ebe <OTP_Write8+0x22>
30007f9c:	f04f 0a01 	mov.w	sl, #1
30007fa0:	e79c      	b.n	30007edc <OTP_Write8+0x40>
30007fa2:	2401      	movs	r4, #1
30007fa4:	e7e2      	b.n	30007f6c <OTP_Write8+0xd0>
30007fa6:	f10d 0117 	add.w	r1, sp, #23
30007faa:	4628      	mov	r0, r5
30007fac:	f7ff feac 	bl	30007d08 <OTP_ProgramMarginRead8.part.0>
30007fb0:	4604      	mov	r4, r0
30007fb2:	2800      	cmp	r0, #0
30007fb4:	d1eb      	bne.n	30007f8e <OTP_Write8+0xf2>
30007fb6:	4b01      	ldr	r3, [pc, #4]	; (30007fbc <OTP_Write8+0x120>)
30007fb8:	4a07      	ldr	r2, [pc, #28]	; (30007fd8 <OTP_Write8+0x13c>)
30007fba:	e7d3      	b.n	30007f64 <OTP_Write8+0xc8>
30007fbc:	3000ba8b 	.word	0x3000ba8b
30007fc0:	3000baaa 	.word	0x3000baaa
30007fc4:	0000e5f9 	.word	0x0000e5f9
30007fc8:	42000008 	.word	0x42000008
30007fcc:	5200000c 	.word	0x5200000c
30007fd0:	3000bac2 	.word	0x3000bac2
30007fd4:	3000bade 	.word	0x3000bade
30007fd8:	3000baf9 	.word	0x3000baf9
30007fdc:	00009b2d 	.word	0x00009b2d

30007fe0 <PLL_NP_ClkSet>:
30007fe0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007fe4:	4606      	mov	r6, r0
30007fe6:	4b1a      	ldr	r3, [pc, #104]	; (30008050 <PLL_NP_ClkSet+0x70>)
30007fe8:	4798      	blx	r3
30007fea:	4d1a      	ldr	r5, [pc, #104]	; (30008054 <PLL_NP_ClkSet+0x74>)
30007fec:	08c2      	lsrs	r2, r0, #3
30007fee:	6c69      	ldr	r1, [r5, #68]	; 0x44
30007ff0:	fbb6 f3f0 	udiv	r3, r6, r0
30007ff4:	fb00 6613 	mls	r6, r0, r3, r6
30007ff8:	3b02      	subs	r3, #2
30007ffa:	f421 61fc 	bic.w	r1, r1, #2016	; 0x7e0
30007ffe:	015b      	lsls	r3, r3, #5
30008000:	fbb6 f7f2 	udiv	r7, r6, r2
30008004:	f403 63fc 	and.w	r3, r3, #2016	; 0x7e0
30008008:	fb02 6017 	mls	r0, r2, r7, r6
3000800c:	430b      	orrs	r3, r1
3000800e:	0439      	lsls	r1, r7, #16
30008010:	646b      	str	r3, [r5, #68]	; 0x44
30008012:	f401 21e0 	and.w	r1, r1, #458752	; 0x70000
30008016:	6ceb      	ldr	r3, [r5, #76]	; 0x4c
30008018:	b29b      	uxth	r3, r3
3000801a:	ea41 0403 	orr.w	r4, r1, r3
3000801e:	2300      	movs	r3, #0
30008020:	4619      	mov	r1, r3
30008022:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30008026:	f002 ffdb 	bl	3000afe0 <__aeabi_uldivmod>
3000802a:	ea44 40c0 	orr.w	r0, r4, r0, lsl #19
3000802e:	4c0a      	ldr	r4, [pc, #40]	; (30008058 <PLL_NP_ClkSet+0x78>)
30008030:	64e8      	str	r0, [r5, #76]	; 0x4c
30008032:	2001      	movs	r0, #1
30008034:	47a0      	blx	r4
30008036:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008038:	203c      	movs	r0, #60	; 0x3c
3000803a:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
3000803e:	646b      	str	r3, [r5, #68]	; 0x44
30008040:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008042:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30008046:	646b      	str	r3, [r5, #68]	; 0x44
30008048:	4623      	mov	r3, r4
3000804a:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000804e:	4718      	bx	r3
30008050:	000099f5 	.word	0x000099f5
30008054:	42008800 	.word	0x42008800
30008058:	00009b2d 	.word	0x00009b2d

3000805c <PLL_AP_ClkSet>:
3000805c:	b510      	push	{r4, lr}
3000805e:	4604      	mov	r4, r0
30008060:	4b0a      	ldr	r3, [pc, #40]	; (3000808c <PLL_AP_ClkSet+0x30>)
30008062:	4798      	blx	r3
30008064:	fbb4 f0f0 	udiv	r0, r4, r0
30008068:	1e84      	subs	r4, r0, #2
3000806a:	3819      	subs	r0, #25
3000806c:	280f      	cmp	r0, #15
3000806e:	d904      	bls.n	3000807a <PLL_AP_ClkSet+0x1e>
30008070:	f240 1181 	movw	r1, #385	; 0x181
30008074:	4806      	ldr	r0, [pc, #24]	; (30008090 <PLL_AP_ClkSet+0x34>)
30008076:	f003 f9e3 	bl	3000b440 <__io_assert_failed_veneer>
3000807a:	4a06      	ldr	r2, [pc, #24]	; (30008094 <PLL_AP_ClkSet+0x38>)
3000807c:	b2e4      	uxtb	r4, r4
3000807e:	6953      	ldr	r3, [r2, #20]
30008080:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008084:	431c      	orrs	r4, r3
30008086:	6154      	str	r4, [r2, #20]
30008088:	bd10      	pop	{r4, pc}
3000808a:	bf00      	nop
3000808c:	000099f5 	.word	0x000099f5
30008090:	3000bb12 	.word	0x3000bb12
30008094:	41000300 	.word	0x41000300

30008098 <PLL_AP>:
30008098:	2801      	cmp	r0, #1
3000809a:	b538      	push	{r3, r4, r5, lr}
3000809c:	d11a      	bne.n	300080d4 <PLL_AP+0x3c>
3000809e:	4b12      	ldr	r3, [pc, #72]	; (300080e8 <PLL_AP+0x50>)
300080a0:	4d12      	ldr	r5, [pc, #72]	; (300080ec <PLL_AP+0x54>)
300080a2:	6f9a      	ldr	r2, [r3, #120]	; 0x78
300080a4:	f012 0f05 	tst.w	r2, #5
300080a8:	d105      	bne.n	300080b6 <PLL_AP+0x1e>
300080aa:	6f9a      	ldr	r2, [r3, #120]	; 0x78
300080ac:	20a0      	movs	r0, #160	; 0xa0
300080ae:	f042 0207 	orr.w	r2, r2, #7
300080b2:	679a      	str	r2, [r3, #120]	; 0x78
300080b4:	47a8      	blx	r5
300080b6:	4c0e      	ldr	r4, [pc, #56]	; (300080f0 <PLL_AP+0x58>)
300080b8:	2001      	movs	r0, #1
300080ba:	6823      	ldr	r3, [r4, #0]
300080bc:	f043 0306 	orr.w	r3, r3, #6
300080c0:	6023      	str	r3, [r4, #0]
300080c2:	47a8      	blx	r5
300080c4:	6823      	ldr	r3, [r4, #0]
300080c6:	f043 0309 	orr.w	r3, r3, #9
300080ca:	6023      	str	r3, [r4, #0]
300080cc:	6863      	ldr	r3, [r4, #4]
300080ce:	2b00      	cmp	r3, #0
300080d0:	dafc      	bge.n	300080cc <PLL_AP+0x34>
300080d2:	bd38      	pop	{r3, r4, r5, pc}
300080d4:	4b06      	ldr	r3, [pc, #24]	; (300080f0 <PLL_AP+0x58>)
300080d6:	681a      	ldr	r2, [r3, #0]
300080d8:	f022 0209 	bic.w	r2, r2, #9
300080dc:	601a      	str	r2, [r3, #0]
300080de:	681a      	ldr	r2, [r3, #0]
300080e0:	f022 0206 	bic.w	r2, r2, #6
300080e4:	601a      	str	r2, [r3, #0]
300080e6:	e7f4      	b.n	300080d2 <PLL_AP+0x3a>
300080e8:	42008800 	.word	0x42008800
300080ec:	00009b2d 	.word	0x00009b2d
300080f0:	41000300 	.word	0x41000300

300080f4 <PSRAM_INFO_Update>:
300080f4:	4b39      	ldr	r3, [pc, #228]	; (300081dc <PSRAM_INFO_Update+0xe8>)
300080f6:	b537      	push	{r0, r1, r2, r4, r5, lr}
300080f8:	4a39      	ldr	r2, [pc, #228]	; (300081e0 <PSRAM_INFO_Update+0xec>)
300080fa:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
300080fe:	6815      	ldr	r5, [r2, #0]
30008100:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008104:	4a37      	ldr	r2, [pc, #220]	; (300081e4 <PSRAM_INFO_Update+0xf0>)
30008106:	4c38      	ldr	r4, [pc, #224]	; (300081e8 <PSRAM_INFO_Update+0xf4>)
30008108:	4355      	muls	r5, r2
3000810a:	3301      	adds	r3, #1
3000810c:	fbb5 f5f3 	udiv	r5, r5, r3
30008110:	086b      	lsrs	r3, r5, #1
30008112:	60e3      	str	r3, [r4, #12]
30008114:	4b35      	ldr	r3, [pc, #212]	; (300081ec <PSRAM_INFO_Update+0xf8>)
30008116:	4798      	blx	r3
30008118:	4b35      	ldr	r3, [pc, #212]	; (300081f0 <PSRAM_INFO_Update+0xfc>)
3000811a:	6020      	str	r0, [r4, #0]
3000811c:	4798      	blx	r3
3000811e:	4b35      	ldr	r3, [pc, #212]	; (300081f4 <PSRAM_INFO_Update+0x100>)
30008120:	2220      	movs	r2, #32
30008122:	210a      	movs	r1, #10
30008124:	60a0      	str	r0, [r4, #8]
30008126:	fbb3 f3f5 	udiv	r3, r3, r5
3000812a:	6163      	str	r3, [r4, #20]
3000812c:	6121      	str	r1, [r4, #16]
3000812e:	fbb2 f3f3 	udiv	r3, r2, r3
30008132:	6223      	str	r3, [r4, #32]
30008134:	6823      	ldr	r3, [r4, #0]
30008136:	4a30      	ldr	r2, [pc, #192]	; (300081f8 <PSRAM_INFO_Update+0x104>)
30008138:	2b01      	cmp	r3, #1
3000813a:	68e3      	ldr	r3, [r4, #12]
3000813c:	d128      	bne.n	30008190 <PSRAM_INFO_Update+0x9c>
3000813e:	4293      	cmp	r3, r2
30008140:	d80c      	bhi.n	3000815c <PSRAM_INFO_Update+0x68>
30008142:	2304      	movs	r3, #4
30008144:	61a3      	str	r3, [r4, #24]
30008146:	230f      	movs	r3, #15
30008148:	61e3      	str	r3, [r4, #28]
3000814a:	4b2c      	ldr	r3, [pc, #176]	; (300081fc <PSRAM_INFO_Update+0x108>)
3000814c:	2004      	movs	r0, #4
3000814e:	4a2c      	ldr	r2, [pc, #176]	; (30008200 <PSRAM_INFO_Update+0x10c>)
30008150:	4619      	mov	r1, r3
30008152:	9500      	str	r5, [sp, #0]
30008154:	f002 fee2 	bl	3000af1c <rtk_log_write>
30008158:	b003      	add	sp, #12
3000815a:	bd30      	pop	{r4, r5, pc}
3000815c:	4a29      	ldr	r2, [pc, #164]	; (30008204 <PSRAM_INFO_Update+0x110>)
3000815e:	4293      	cmp	r3, r2
30008160:	d801      	bhi.n	30008166 <PSRAM_INFO_Update+0x72>
30008162:	2305      	movs	r3, #5
30008164:	e7ee      	b.n	30008144 <PSRAM_INFO_Update+0x50>
30008166:	4a28      	ldr	r2, [pc, #160]	; (30008208 <PSRAM_INFO_Update+0x114>)
30008168:	4293      	cmp	r3, r2
3000816a:	d803      	bhi.n	30008174 <PSRAM_INFO_Update+0x80>
3000816c:	2306      	movs	r3, #6
3000816e:	61a3      	str	r3, [r4, #24]
30008170:	2312      	movs	r3, #18
30008172:	e7e9      	b.n	30008148 <PSRAM_INFO_Update+0x54>
30008174:	4a25      	ldr	r2, [pc, #148]	; (3000820c <PSRAM_INFO_Update+0x118>)
30008176:	4293      	cmp	r3, r2
30008178:	d803      	bhi.n	30008182 <PSRAM_INFO_Update+0x8e>
3000817a:	2307      	movs	r3, #7
3000817c:	61a3      	str	r3, [r4, #24]
3000817e:	2314      	movs	r3, #20
30008180:	e7e2      	b.n	30008148 <PSRAM_INFO_Update+0x54>
30008182:	4a23      	ldr	r2, [pc, #140]	; (30008210 <PSRAM_INFO_Update+0x11c>)
30008184:	4293      	cmp	r3, r2
30008186:	d8e0      	bhi.n	3000814a <PSRAM_INFO_Update+0x56>
30008188:	2308      	movs	r3, #8
3000818a:	61a3      	str	r3, [r4, #24]
3000818c:	231c      	movs	r3, #28
3000818e:	e7db      	b.n	30008148 <PSRAM_INFO_Update+0x54>
30008190:	4293      	cmp	r3, r2
30008192:	d808      	bhi.n	300081a6 <PSRAM_INFO_Update+0xb2>
30008194:	2304      	movs	r3, #4
30008196:	61a3      	str	r3, [r4, #24]
30008198:	2308      	movs	r3, #8
3000819a:	61e3      	str	r3, [r4, #28]
3000819c:	69a2      	ldr	r2, [r4, #24]
3000819e:	4b1d      	ldr	r3, [pc, #116]	; (30008214 <PSRAM_INFO_Update+0x120>)
300081a0:	f883 2110 	strb.w	r2, [r3, #272]	; 0x110
300081a4:	e7d1      	b.n	3000814a <PSRAM_INFO_Update+0x56>
300081a6:	4a17      	ldr	r2, [pc, #92]	; (30008204 <PSRAM_INFO_Update+0x110>)
300081a8:	4293      	cmp	r3, r2
300081aa:	d801      	bhi.n	300081b0 <PSRAM_INFO_Update+0xbc>
300081ac:	2305      	movs	r3, #5
300081ae:	e7f2      	b.n	30008196 <PSRAM_INFO_Update+0xa2>
300081b0:	4a15      	ldr	r2, [pc, #84]	; (30008208 <PSRAM_INFO_Update+0x114>)
300081b2:	4293      	cmp	r3, r2
300081b4:	d802      	bhi.n	300081bc <PSRAM_INFO_Update+0xc8>
300081b6:	2306      	movs	r3, #6
300081b8:	61a3      	str	r3, [r4, #24]
300081ba:	e7ee      	b.n	3000819a <PSRAM_INFO_Update+0xa6>
300081bc:	4a13      	ldr	r2, [pc, #76]	; (3000820c <PSRAM_INFO_Update+0x118>)
300081be:	4293      	cmp	r3, r2
300081c0:	d803      	bhi.n	300081ca <PSRAM_INFO_Update+0xd6>
300081c2:	2307      	movs	r3, #7
300081c4:	61a3      	str	r3, [r4, #24]
300081c6:	2306      	movs	r3, #6
300081c8:	e7e7      	b.n	3000819a <PSRAM_INFO_Update+0xa6>
300081ca:	4a11      	ldr	r2, [pc, #68]	; (30008210 <PSRAM_INFO_Update+0x11c>)
300081cc:	4293      	cmp	r3, r2
300081ce:	d8e5      	bhi.n	3000819c <PSRAM_INFO_Update+0xa8>
300081d0:	2804      	cmp	r0, #4
300081d2:	d0f6      	beq.n	300081c2 <PSRAM_INFO_Update+0xce>
300081d4:	2803      	cmp	r0, #3
300081d6:	d1e1      	bne.n	3000819c <PSRAM_INFO_Update+0xa8>
300081d8:	61a1      	str	r1, [r4, #24]
300081da:	e7f4      	b.n	300081c6 <PSRAM_INFO_Update+0xd2>
300081dc:	42008000 	.word	0x42008000
300081e0:	3000dd9c 	.word	0x3000dd9c
300081e4:	000f4240 	.word	0x000f4240
300081e8:	3000eae0 	.word	0x3000eae0
300081ec:	3000735d 	.word	0x3000735d
300081f0:	30007329 	.word	0x30007329
300081f4:	3b9aca00 	.word	0x3b9aca00
300081f8:	0632ea00 	.word	0x0632ea00
300081fc:	3000bb20 	.word	0x3000bb20
30008200:	3000bb26 	.word	0x3000bb26
30008204:	07ed6b40 	.word	0x07ed6b40
30008208:	09e4f580 	.word	0x09e4f580
3000820c:	0bebc200 	.word	0x0bebc200
30008210:	0ee6b280 	.word	0x0ee6b280
30008214:	23020000 	.word	0x23020000

30008218 <PSRAM_PHY_StructInit>:
30008218:	2301      	movs	r3, #1
3000821a:	2202      	movs	r2, #2
3000821c:	e9c0 3200 	strd	r3, r2, [r0]
30008220:	2203      	movs	r2, #3
30008222:	e9c0 2202 	strd	r2, r2, [r0, #8]
30008226:	220a      	movs	r2, #10
30008228:	e9c0 2304 	strd	r2, r3, [r0, #16]
3000822c:	2307      	movs	r3, #7
3000822e:	6183      	str	r3, [r0, #24]
30008230:	4770      	bx	lr
	...

30008234 <PSRAM_PHY_Init>:
30008234:	b538      	push	{r3, r4, r5, lr}
30008236:	6903      	ldr	r3, [r0, #16]
30008238:	4605      	mov	r5, r0
3000823a:	6802      	ldr	r2, [r0, #0]
3000823c:	f003 031f 	and.w	r3, r3, #31
30008240:	4c13      	ldr	r4, [pc, #76]	; (30008290 <PSRAM_PHY_Init+0x5c>)
30008242:	ea43 7302 	orr.w	r3, r3, r2, lsl #28
30008246:	6842      	ldr	r2, [r0, #4]
30008248:	0612      	lsls	r2, r2, #24
3000824a:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
3000824e:	4313      	orrs	r3, r2
30008250:	6882      	ldr	r2, [r0, #8]
30008252:	0412      	lsls	r2, r2, #16
30008254:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008258:	4313      	orrs	r3, r2
3000825a:	68c2      	ldr	r2, [r0, #12]
3000825c:	0212      	lsls	r2, r2, #8
3000825e:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30008262:	4313      	orrs	r3, r2
30008264:	6063      	str	r3, [r4, #4]
30008266:	4b0b      	ldr	r3, [pc, #44]	; (30008294 <PSRAM_PHY_Init+0x60>)
30008268:	4798      	blx	r3
3000826a:	2801      	cmp	r0, #1
3000826c:	e9d5 3105 	ldrd	r3, r1, [r5, #20]
30008270:	ea4f 1303 	mov.w	r3, r3, lsl #4
30008274:	f001 010f 	and.w	r1, r1, #15
30008278:	b2db      	uxtb	r3, r3
3000827a:	ea43 0301 	orr.w	r3, r3, r1
3000827e:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
30008282:	bf0c      	ite	eq
30008284:	f443 6360 	orreq.w	r3, r3, #3584	; 0xe00
30008288:	f443 6370 	orrne.w	r3, r3, #3840	; 0xf00
3000828c:	61a3      	str	r3, [r4, #24]
3000828e:	bd38      	pop	{r3, r4, r5, pc}
30008290:	41002000 	.word	0x41002000
30008294:	3000735d 	.word	0x3000735d

30008298 <PSRAM_CTRL_Init>:
30008298:	4b50      	ldr	r3, [pc, #320]	; (300083dc <PSRAM_CTRL_Init+0x144>)
3000829a:	2200      	movs	r2, #0
3000829c:	4950      	ldr	r1, [pc, #320]	; (300083e0 <PSRAM_CTRL_Init+0x148>)
3000829e:	b530      	push	{r4, r5, lr}
300082a0:	609a      	str	r2, [r3, #8]
300082a2:	4a50      	ldr	r2, [pc, #320]	; (300083e4 <PSRAM_CTRL_Init+0x14c>)
300082a4:	601a      	str	r2, [r3, #0]
300082a6:	2201      	movs	r2, #1
300082a8:	615a      	str	r2, [r3, #20]
300082aa:	f24d 0201 	movw	r2, #53249	; 0xd001
300082ae:	f8c3 2120 	str.w	r2, [r3, #288]	; 0x120
300082b2:	6808      	ldr	r0, [r1, #0]
300082b4:	2802      	cmp	r0, #2
300082b6:	d15a      	bne.n	3000836e <PSRAM_CTRL_Init+0xd6>
300082b8:	f245 5251 	movw	r2, #21841	; 0x5551
300082bc:	247d      	movs	r4, #125	; 0x7d
300082be:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
300082c2:	690a      	ldr	r2, [r1, #16]
300082c4:	f002 020f 	and.w	r2, r2, #15
300082c8:	f442 52e3 	orr.w	r2, r2, #7264	; 0x1c60
300082cc:	f042 0210 	orr.w	r2, r2, #16
300082d0:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
300082d4:	694d      	ldr	r5, [r1, #20]
300082d6:	69ca      	ldr	r2, [r1, #28]
300082d8:	fbb2 f2f5 	udiv	r2, r2, r5
300082dc:	fbb4 f5f5 	udiv	r5, r4, r5
300082e0:	f002 043f 	and.w	r4, r2, #63	; 0x3f
300082e4:	0192      	lsls	r2, r2, #6
300082e6:	ea44 6405 	orr.w	r4, r4, r5, lsl #24
300082ea:	f402 627c 	and.w	r2, r2, #4032	; 0xfc0
300082ee:	4322      	orrs	r2, r4
300082f0:	f442 1280 	orr.w	r2, r2, #1048576	; 0x100000
300082f4:	f8c3 2138 	str.w	r2, [r3, #312]	; 0x138
300082f8:	22a0      	movs	r2, #160	; 0xa0
300082fa:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
300082fe:	2220      	movs	r2, #32
30008300:	f8c3 20f4 	str.w	r2, [r3, #244]	; 0xf4
30008304:	f44f 22a0 	mov.w	r2, #327680	; 0x50000
30008308:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
3000830c:	698a      	ldr	r2, [r1, #24]
3000830e:	0092      	lsls	r2, r2, #2
30008310:	3a01      	subs	r2, #1
30008312:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008316:	f442 22a2 	orr.w	r2, r2, #331776	; 0x51000
3000831a:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
3000831e:	2802      	cmp	r0, #2
30008320:	698a      	ldr	r2, [r1, #24]
30008322:	4c2e      	ldr	r4, [pc, #184]	; (300083dc <PSRAM_CTRL_Init+0x144>)
30008324:	ea4f 0182 	mov.w	r1, r2, lsl #2
30008328:	f8d4 511c 	ldr.w	r5, [r4, #284]	; 0x11c
3000832c:	f101 33ff 	add.w	r3, r1, #4294967295	; 0xffffffff
30008330:	bf08      	it	eq
30008332:	3902      	subeq	r1, #2
30008334:	f3c3 030b 	ubfx	r3, r3, #0, #12
30008338:	bf08      	it	eq
3000833a:	f3c1 010b 	ubfxeq	r1, r1, #0, #12
3000833e:	ea43 0305 	orr.w	r3, r3, r5
30008342:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30008346:	f8c4 311c 	str.w	r3, [r4, #284]	; 0x11c
3000834a:	bf1b      	ittet	ne
3000834c:	f102 4300 	addne.w	r3, r2, #2147483648	; 0x80000000
30008350:	f640 72fe 	movwne	r2, #4094	; 0xffe
30008354:	f8c4 113c 	streq.w	r1, [r4, #316]	; 0x13c
30008358:	f103 33ff 	addne.w	r3, r3, #4294967295	; 0xffffffff
3000835c:	bf1c      	itt	ne
3000835e:	ea02 0343 	andne.w	r3, r2, r3, lsl #1
30008362:	f8c4 313c 	strne.w	r3, [r4, #316]	; 0x13c
30008366:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008368:	061b      	lsls	r3, r3, #24
3000836a:	d5fc      	bpl.n	30008366 <PSRAM_CTRL_Init+0xce>
3000836c:	bd30      	pop	{r4, r5, pc}
3000836e:	f241 5251 	movw	r2, #5457	; 0x1551
30008372:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30008376:	690a      	ldr	r2, [r1, #16]
30008378:	f002 020f 	and.w	r2, r2, #15
3000837c:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30008380:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008384:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30008388:	694d      	ldr	r5, [r1, #20]
3000838a:	69cc      	ldr	r4, [r1, #28]
3000838c:	fbb2 f2f5 	udiv	r2, r2, r5
30008390:	fbb4 f4f5 	udiv	r4, r4, r5
30008394:	0955      	lsrs	r5, r2, #5
30008396:	01a2      	lsls	r2, r4, #6
30008398:	f004 043f 	and.w	r4, r4, #63	; 0x3f
3000839c:	f402 627c 	and.w	r2, r2, #4032	; 0xfc0
300083a0:	ea42 6205 	orr.w	r2, r2, r5, lsl #24
300083a4:	4322      	orrs	r2, r4
300083a6:	f442 1283 	orr.w	r2, r2, #1073152	; 0x106000
300083aa:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
300083ae:	f8c3 2138 	str.w	r2, [r3, #312]	; 0x138
300083b2:	f242 0220 	movw	r2, #8224	; 0x2020
300083b6:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
300083ba:	f24a 02a0 	movw	r2, #41120	; 0xa0a0
300083be:	f8c3 20f4 	str.w	r2, [r3, #244]	; 0xf4
300083c2:	f44f 2280 	mov.w	r2, #262144	; 0x40000
300083c6:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300083ca:	698a      	ldr	r2, [r1, #24]
300083cc:	0092      	lsls	r2, r2, #2
300083ce:	3a01      	subs	r2, #1
300083d0:	f3c2 020b 	ubfx	r2, r2, #0, #12
300083d4:	f442 2284 	orr.w	r2, r2, #270336	; 0x42000
300083d8:	e79f      	b.n	3000831a <PSRAM_CTRL_Init+0x82>
300083da:	bf00      	nop
300083dc:	41012000 	.word	0x41012000
300083e0:	3000eae0 	.word	0x3000eae0
300083e4:	00bfe000 	.word	0x00bfe000

300083e8 <PSRAM_REG_Write>:
300083e8:	4b1e      	ldr	r3, [pc, #120]	; (30008464 <PSRAM_REG_Write+0x7c>)
300083ea:	b530      	push	{r4, r5, lr}
300083ec:	6a9c      	ldr	r4, [r3, #40]	; 0x28
300083ee:	f014 0401 	ands.w	r4, r4, #1
300083f2:	d1fb      	bne.n	300083ec <PSRAM_REG_Write+0x4>
300083f4:	681d      	ldr	r5, [r3, #0]
300083f6:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
300083fa:	601d      	str	r5, [r3, #0]
300083fc:	609c      	str	r4, [r3, #8]
300083fe:	681d      	ldr	r5, [r3, #0]
30008400:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008404:	601d      	str	r5, [r3, #0]
30008406:	605c      	str	r4, [r3, #4]
30008408:	f021 447f 	bic.w	r4, r1, #4278190080	; 0xff000000
3000840c:	4411      	add	r1, r2
3000840e:	f8c3 4130 	str.w	r4, [r3, #304]	; 0x130
30008412:	24c0      	movs	r4, #192	; 0xc0
30008414:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
30008418:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
3000841c:	0e04      	lsrs	r4, r0, #24
3000841e:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
30008422:	f3c0 4407 	ubfx	r4, r0, #16, #8
30008426:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
3000842a:	f3c0 2407 	ubfx	r4, r0, #8, #8
3000842e:	b2c0      	uxtb	r0, r0
30008430:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
30008434:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
30008438:	4b0a      	ldr	r3, [pc, #40]	; (30008464 <PSRAM_REG_Write+0x7c>)
3000843a:	428a      	cmp	r2, r1
3000843c:	d10d      	bne.n	3000845a <PSRAM_REG_Write+0x72>
3000843e:	2201      	movs	r2, #1
30008440:	609a      	str	r2, [r3, #8]
30008442:	689a      	ldr	r2, [r3, #8]
30008444:	07d1      	lsls	r1, r2, #31
30008446:	d4fc      	bmi.n	30008442 <PSRAM_REG_Write+0x5a>
30008448:	4b06      	ldr	r3, [pc, #24]	; (30008464 <PSRAM_REG_Write+0x7c>)
3000844a:	6a9a      	ldr	r2, [r3, #40]	; 0x28
3000844c:	07d2      	lsls	r2, r2, #31
3000844e:	d4fc      	bmi.n	3000844a <PSRAM_REG_Write+0x62>
30008450:	681a      	ldr	r2, [r3, #0]
30008452:	f022 4200 	bic.w	r2, r2, #2147483648	; 0x80000000
30008456:	601a      	str	r2, [r3, #0]
30008458:	bd30      	pop	{r4, r5, pc}
3000845a:	f812 0b01 	ldrb.w	r0, [r2], #1
3000845e:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
30008462:	e7ea      	b.n	3000843a <PSRAM_REG_Write+0x52>
30008464:	41012000 	.word	0x41012000

30008468 <PSRAM_APM_DEVIC_Init>:
30008468:	4b10      	ldr	r3, [pc, #64]	; (300084ac <PSRAM_APM_DEVIC_Init+0x44>)
3000846a:	4a11      	ldr	r2, [pc, #68]	; (300084b0 <PSRAM_APM_DEVIC_Init+0x48>)
3000846c:	b513      	push	{r0, r1, r4, lr}
3000846e:	6999      	ldr	r1, [r3, #24]
30008470:	2000      	movs	r0, #0
30008472:	4c10      	ldr	r4, [pc, #64]	; (300084b4 <PSRAM_APM_DEVIC_Init+0x4c>)
30008474:	440a      	add	r2, r1
30008476:	1ecb      	subs	r3, r1, #3
30008478:	2102      	movs	r1, #2
3000847a:	f812 2c03 	ldrb.w	r2, [r2, #-3]
3000847e:	009b      	lsls	r3, r3, #2
30008480:	0152      	lsls	r2, r2, #5
30008482:	f003 031c 	and.w	r3, r3, #28
30008486:	b2d2      	uxtb	r2, r2
30008488:	f043 0320 	orr.w	r3, r3, #32
3000848c:	f88d 2004 	strb.w	r2, [sp, #4]
30008490:	f88d 2005 	strb.w	r2, [sp, #5]
30008494:	466a      	mov	r2, sp
30008496:	f88d 3000 	strb.w	r3, [sp]
3000849a:	f88d 3001 	strb.w	r3, [sp, #1]
3000849e:	47a0      	blx	r4
300084a0:	aa01      	add	r2, sp, #4
300084a2:	2102      	movs	r1, #2
300084a4:	2004      	movs	r0, #4
300084a6:	47a0      	blx	r4
300084a8:	b002      	add	sp, #8
300084aa:	bd10      	pop	{r4, pc}
300084ac:	3000eae0 	.word	0x3000eae0
300084b0:	3000df64 	.word	0x3000df64
300084b4:	300083e9 	.word	0x300083e9

300084b8 <PSRAM_WB_REG_Write>:
300084b8:	4b1c      	ldr	r3, [pc, #112]	; (3000852c <PSRAM_WB_REG_Write+0x74>)
300084ba:	b530      	push	{r4, r5, lr}
300084bc:	6a9c      	ldr	r4, [r3, #40]	; 0x28
300084be:	f014 0401 	ands.w	r4, r4, #1
300084c2:	d1fb      	bne.n	300084bc <PSRAM_WB_REG_Write+0x4>
300084c4:	681d      	ldr	r5, [r3, #0]
300084c6:	b2c0      	uxtb	r0, r0
300084c8:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
300084cc:	601d      	str	r5, [r3, #0]
300084ce:	609c      	str	r4, [r3, #8]
300084d0:	681d      	ldr	r5, [r3, #0]
300084d2:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300084d6:	601d      	str	r5, [r3, #0]
300084d8:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
300084dc:	605c      	str	r4, [r3, #4]
300084de:	4411      	add	r1, r2
300084e0:	f8c3 5130 	str.w	r5, [r3, #304]	; 0x130
300084e4:	2560      	movs	r5, #96	; 0x60
300084e6:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
300084ea:	2501      	movs	r5, #1
300084ec:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300084f0:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
300084f4:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300084f8:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300084fc:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
30008500:	4b0a      	ldr	r3, [pc, #40]	; (3000852c <PSRAM_WB_REG_Write+0x74>)
30008502:	428a      	cmp	r2, r1
30008504:	d10d      	bne.n	30008522 <PSRAM_WB_REG_Write+0x6a>
30008506:	2201      	movs	r2, #1
30008508:	609a      	str	r2, [r3, #8]
3000850a:	689a      	ldr	r2, [r3, #8]
3000850c:	07d1      	lsls	r1, r2, #31
3000850e:	d4fc      	bmi.n	3000850a <PSRAM_WB_REG_Write+0x52>
30008510:	4b06      	ldr	r3, [pc, #24]	; (3000852c <PSRAM_WB_REG_Write+0x74>)
30008512:	6a9a      	ldr	r2, [r3, #40]	; 0x28
30008514:	07d2      	lsls	r2, r2, #31
30008516:	d4fc      	bmi.n	30008512 <PSRAM_WB_REG_Write+0x5a>
30008518:	681a      	ldr	r2, [r3, #0]
3000851a:	f022 4200 	bic.w	r2, r2, #2147483648	; 0x80000000
3000851e:	601a      	str	r2, [r3, #0]
30008520:	bd30      	pop	{r4, r5, pc}
30008522:	f812 0b01 	ldrb.w	r0, [r2], #1
30008526:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
3000852a:	e7ea      	b.n	30008502 <PSRAM_WB_REG_Write+0x4a>
3000852c:	41012000 	.word	0x41012000

30008530 <PSRAM_WB_DEVIC_Init>:
30008530:	238f      	movs	r3, #143	; 0x8f
30008532:	b507      	push	{r0, r1, r2, lr}
30008534:	f88d 3004 	strb.w	r3, [sp, #4]
30008538:	2102      	movs	r1, #2
3000853a:	4b09      	ldr	r3, [pc, #36]	; (30008560 <PSRAM_WB_DEVIC_Init+0x30>)
3000853c:	2000      	movs	r0, #0
3000853e:	699a      	ldr	r2, [r3, #24]
30008540:	4b08      	ldr	r3, [pc, #32]	; (30008564 <PSRAM_WB_DEVIC_Init+0x34>)
30008542:	4413      	add	r3, r2
30008544:	aa01      	add	r2, sp, #4
30008546:	f813 3c03 	ldrb.w	r3, [r3, #-3]
3000854a:	011b      	lsls	r3, r3, #4
3000854c:	f043 030c 	orr.w	r3, r3, #12
30008550:	f88d 3005 	strb.w	r3, [sp, #5]
30008554:	4b04      	ldr	r3, [pc, #16]	; (30008568 <PSRAM_WB_DEVIC_Init+0x38>)
30008556:	4798      	blx	r3
30008558:	b003      	add	sp, #12
3000855a:	f85d fb04 	ldr.w	pc, [sp], #4
3000855e:	bf00      	nop
30008560:	3000eae0 	.word	0x3000eae0
30008564:	3000df84 	.word	0x3000df84
30008568:	300084b9 	.word	0x300084b9

3000856c <PSRAM_calibration>:
3000856c:	4b9b      	ldr	r3, [pc, #620]	; (300087dc <PSRAM_calibration+0x270>)
3000856e:	2218      	movs	r2, #24
30008570:	2100      	movs	r1, #0
30008572:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008576:	b091      	sub	sp, #68	; 0x44
30008578:	685b      	ldr	r3, [r3, #4]
3000857a:	9308      	str	r3, [sp, #32]
3000857c:	f023 4370 	bic.w	r3, r3, #4026531840	; 0xf0000000
30008580:	a80a      	add	r0, sp, #40	; 0x28
30008582:	f023 031f 	bic.w	r3, r3, #31
30008586:	9307      	str	r3, [sp, #28]
30008588:	f002 ff1a 	bl	3000b3c0 <____wrap_memset_veneer>
3000858c:	4a94      	ldr	r2, [pc, #592]	; (300087e0 <PSRAM_calibration+0x274>)
3000858e:	6953      	ldr	r3, [r2, #20]
30008590:	03d9      	lsls	r1, r3, #15
30008592:	d51e      	bpl.n	300085d2 <PSRAM_calibration+0x66>
30008594:	2300      	movs	r3, #0
30008596:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000859a:	f3bf 8f4f 	dsb	sy
3000859e:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
300085a2:	f643 74e0 	movw	r4, #16352	; 0x3fe0
300085a6:	f3c3 00c9 	ubfx	r0, r3, #3, #10
300085aa:	f3c3 334e 	ubfx	r3, r3, #13, #15
300085ae:	015b      	lsls	r3, r3, #5
300085b0:	ea03 0604 	and.w	r6, r3, r4
300085b4:	4601      	mov	r1, r0
300085b6:	ea46 7581 	orr.w	r5, r6, r1, lsl #30
300085ba:	3901      	subs	r1, #1
300085bc:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
300085c0:	d2f9      	bcs.n	300085b6 <PSRAM_calibration+0x4a>
300085c2:	3b20      	subs	r3, #32
300085c4:	f113 0f20 	cmn.w	r3, #32
300085c8:	d1f2      	bne.n	300085b0 <PSRAM_calibration+0x44>
300085ca:	f3bf 8f4f 	dsb	sy
300085ce:	f3bf 8f6f 	isb	sy
300085d2:	4a83      	ldr	r2, [pc, #524]	; (300087e0 <PSRAM_calibration+0x274>)
300085d4:	2300      	movs	r3, #0
300085d6:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300085da:	f3bf 8f4f 	dsb	sy
300085de:	6953      	ldr	r3, [r2, #20]
300085e0:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300085e4:	6153      	str	r3, [r2, #20]
300085e6:	f3bf 8f4f 	dsb	sy
300085ea:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
300085ee:	f643 70e0 	movw	r0, #16352	; 0x3fe0
300085f2:	f3c3 01c9 	ubfx	r1, r3, #3, #10
300085f6:	f3c3 334e 	ubfx	r3, r3, #13, #15
300085fa:	015b      	lsls	r3, r3, #5
300085fc:	ea03 0600 	and.w	r6, r3, r0
30008600:	460c      	mov	r4, r1
30008602:	ea46 7584 	orr.w	r5, r6, r4, lsl #30
30008606:	3c01      	subs	r4, #1
30008608:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
3000860c:	d2f9      	bcs.n	30008602 <PSRAM_calibration+0x96>
3000860e:	3b20      	subs	r3, #32
30008610:	f113 0f20 	cmn.w	r3, #32
30008614:	d1f2      	bne.n	300085fc <PSRAM_calibration+0x90>
30008616:	f3bf 8f4f 	dsb	sy
3000861a:	f3bf 8f6f 	isb	sy
3000861e:	4a6f      	ldr	r2, [pc, #444]	; (300087dc <PSRAM_calibration+0x270>)
30008620:	f04f 0800 	mov.w	r8, #0
30008624:	f04f 0b01 	mov.w	fp, #1
30008628:	f8df a1c0 	ldr.w	sl, [pc, #448]	; 300087ec <PSRAM_calibration+0x280>
3000862c:	6813      	ldr	r3, [r2, #0]
3000862e:	f023 0301 	bic.w	r3, r3, #1
30008632:	6013      	str	r3, [r2, #0]
30008634:	2304      	movs	r3, #4
30008636:	9404      	str	r4, [sp, #16]
30008638:	9306      	str	r3, [sp, #24]
3000863a:	9405      	str	r4, [sp, #20]
3000863c:	9b07      	ldr	r3, [sp, #28]
3000863e:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30008642:	2500      	movs	r5, #0
30008644:	ea43 730b 	orr.w	r3, r3, fp, lsl #28
30008648:	463e      	mov	r6, r7
3000864a:	46a9      	mov	r9, r5
3000864c:	9309      	str	r3, [sp, #36]	; 0x24
3000864e:	9b09      	ldr	r3, [sp, #36]	; 0x24
30008650:	f04f 40c0 	mov.w	r0, #1610612736	; 0x60000000
30008654:	4a61      	ldr	r2, [pc, #388]	; (300087dc <PSRAM_calibration+0x270>)
30008656:	ea43 0309 	orr.w	r3, r3, r9
3000865a:	f8df e1a0 	ldr.w	lr, [pc, #416]	; 300087fc <PSRAM_calibration+0x290>
3000865e:	f8df c1a0 	ldr.w	ip, [pc, #416]	; 30008800 <PSRAM_calibration+0x294>
30008662:	6053      	str	r3, [r2, #4]
30008664:	f8da 3000 	ldr.w	r3, [sl]
30008668:	4a5e      	ldr	r2, [pc, #376]	; (300087e4 <PSRAM_calibration+0x278>)
3000866a:	6003      	str	r3, [r0, #0]
3000866c:	f8da 3004 	ldr.w	r3, [sl, #4]
30008670:	6013      	str	r3, [r2, #0]
30008672:	f502 2230 	add.w	r2, r2, #720896	; 0xb0000
30008676:	f8da 3008 	ldr.w	r3, [sl, #8]
3000867a:	6013      	str	r3, [r2, #0]
3000867c:	f8da 100c 	ldr.w	r1, [sl, #12]
30008680:	f8ce 1000 	str.w	r1, [lr]
30008684:	f8da 1010 	ldr.w	r1, [sl, #16]
30008688:	f8cc 1000 	str.w	r1, [ip]
3000868c:	4956      	ldr	r1, [pc, #344]	; (300087e8 <PSRAM_calibration+0x27c>)
3000868e:	f8da 3014 	ldr.w	r3, [sl, #20]
30008692:	600b      	str	r3, [r1, #0]
30008694:	6800      	ldr	r0, [r0, #0]
30008696:	900a      	str	r0, [sp, #40]	; 0x28
30008698:	4852      	ldr	r0, [pc, #328]	; (300087e4 <PSRAM_calibration+0x278>)
3000869a:	6800      	ldr	r0, [r0, #0]
3000869c:	900b      	str	r0, [sp, #44]	; 0x2c
3000869e:	a80a      	add	r0, sp, #40	; 0x28
300086a0:	6812      	ldr	r2, [r2, #0]
300086a2:	920c      	str	r2, [sp, #48]	; 0x30
300086a4:	2218      	movs	r2, #24
300086a6:	f8de 3000 	ldr.w	r3, [lr]
300086aa:	930d      	str	r3, [sp, #52]	; 0x34
300086ac:	f8dc 3000 	ldr.w	r3, [ip]
300086b0:	930e      	str	r3, [sp, #56]	; 0x38
300086b2:	680b      	ldr	r3, [r1, #0]
300086b4:	494d      	ldr	r1, [pc, #308]	; (300087ec <PSRAM_calibration+0x280>)
300086b6:	930f      	str	r3, [sp, #60]	; 0x3c
300086b8:	4b4d      	ldr	r3, [pc, #308]	; (300087f0 <PSRAM_calibration+0x284>)
300086ba:	4798      	blx	r3
300086bc:	2800      	cmp	r0, #0
300086be:	d17d      	bne.n	300087bc <PSRAM_calibration+0x250>
300086c0:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
300086c4:	bf08      	it	eq
300086c6:	464e      	moveq	r6, r9
300086c8:	f1b9 0f1f 	cmp.w	r9, #31
300086cc:	eb06 0705 	add.w	r7, r6, r5
300086d0:	f105 0501 	add.w	r5, r5, #1
300086d4:	d105      	bne.n	300086e2 <PSRAM_calibration+0x176>
300086d6:	4545      	cmp	r5, r8
300086d8:	dd03      	ble.n	300086e2 <PSRAM_calibration+0x176>
300086da:	465c      	mov	r4, fp
300086dc:	46a8      	mov	r8, r5
300086de:	e9cd 7604 	strd	r7, r6, [sp, #16]
300086e2:	f109 0901 	add.w	r9, r9, #1
300086e6:	f1b9 0f20 	cmp.w	r9, #32
300086ea:	d1b0      	bne.n	3000864e <PSRAM_calibration+0xe2>
300086ec:	9b06      	ldr	r3, [sp, #24]
300086ee:	ea4f 0b4b 	mov.w	fp, fp, lsl #1
300086f2:	3b01      	subs	r3, #1
300086f4:	9306      	str	r3, [sp, #24]
300086f6:	d1a1      	bne.n	3000863c <PSRAM_calibration+0xd0>
300086f8:	9b04      	ldr	r3, [sp, #16]
300086fa:	2004      	movs	r0, #4
300086fc:	4a3d      	ldr	r2, [pc, #244]	; (300087f4 <PSRAM_calibration+0x288>)
300086fe:	9301      	str	r3, [sp, #4]
30008700:	9b05      	ldr	r3, [sp, #20]
30008702:	9300      	str	r3, [sp, #0]
30008704:	4b3c      	ldr	r3, [pc, #240]	; (300087f8 <PSRAM_calibration+0x28c>)
30008706:	e9cd 8402 	strd	r8, r4, [sp, #8]
3000870a:	4619      	mov	r1, r3
3000870c:	f002 fc06 	bl	3000af1c <rtk_log_write>
30008710:	4a33      	ldr	r2, [pc, #204]	; (300087e0 <PSRAM_calibration+0x274>)
30008712:	6953      	ldr	r3, [r2, #20]
30008714:	03db      	lsls	r3, r3, #15
30008716:	d424      	bmi.n	30008762 <PSRAM_calibration+0x1f6>
30008718:	9b06      	ldr	r3, [sp, #24]
3000871a:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000871e:	f3bf 8f4f 	dsb	sy
30008722:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30008726:	f643 75e0 	movw	r5, #16352	; 0x3fe0
3000872a:	f3c3 00c9 	ubfx	r0, r3, #3, #10
3000872e:	f3c3 334e 	ubfx	r3, r3, #13, #15
30008732:	015b      	lsls	r3, r3, #5
30008734:	ea03 0705 	and.w	r7, r3, r5
30008738:	4601      	mov	r1, r0
3000873a:	ea47 7681 	orr.w	r6, r7, r1, lsl #30
3000873e:	3901      	subs	r1, #1
30008740:	f8c2 6260 	str.w	r6, [r2, #608]	; 0x260
30008744:	d2f9      	bcs.n	3000873a <PSRAM_calibration+0x1ce>
30008746:	3b20      	subs	r3, #32
30008748:	f113 0f20 	cmn.w	r3, #32
3000874c:	d1f2      	bne.n	30008734 <PSRAM_calibration+0x1c8>
3000874e:	f3bf 8f4f 	dsb	sy
30008752:	6953      	ldr	r3, [r2, #20]
30008754:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30008758:	6153      	str	r3, [r2, #20]
3000875a:	f3bf 8f4f 	dsb	sy
3000875e:	f3bf 8f6f 	isb	sy
30008762:	f1b8 0f08 	cmp.w	r8, #8
30008766:	dd37      	ble.n	300087d8 <PSRAM_calibration+0x26c>
30008768:	2001      	movs	r0, #1
3000876a:	e9dd 3204 	ldrd	r3, r2, [sp, #16]
3000876e:	1a9b      	subs	r3, r3, r2
30008770:	e9dd 2104 	ldrd	r2, r1, [sp, #16]
30008774:	eb03 73d3 	add.w	r3, r3, r3, lsr #31
30008778:	eb02 0b01 	add.w	fp, r2, r1
3000877c:	9a08      	ldr	r2, [sp, #32]
3000877e:	105b      	asrs	r3, r3, #1
30008780:	f002 627f 	and.w	r2, r2, #267386880	; 0xff00000
30008784:	eb0b 7bdb 	add.w	fp, fp, fp, lsr #31
30008788:	3b02      	subs	r3, #2
3000878a:	f3cb 0b44 	ubfx	fp, fp, #1, #5
3000878e:	ea42 7404 	orr.w	r4, r2, r4, lsl #28
30008792:	ea4b 0404 	orr.w	r4, fp, r4
30008796:	ea4f 4b03 	mov.w	fp, r3, lsl #16
3000879a:	021b      	lsls	r3, r3, #8
3000879c:	f40b 2b70 	and.w	fp, fp, #983040	; 0xf0000
300087a0:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
300087a4:	ea44 040b 	orr.w	r4, r4, fp
300087a8:	431c      	orrs	r4, r3
300087aa:	4b0c      	ldr	r3, [pc, #48]	; (300087dc <PSRAM_calibration+0x270>)
300087ac:	605c      	str	r4, [r3, #4]
300087ae:	681a      	ldr	r2, [r3, #0]
300087b0:	f042 0201 	orr.w	r2, r2, #1
300087b4:	601a      	str	r2, [r3, #0]
300087b6:	b011      	add	sp, #68	; 0x44
300087b8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300087bc:	1c72      	adds	r2, r6, #1
300087be:	d090      	beq.n	300086e2 <PSRAM_calibration+0x176>
300087c0:	45a8      	cmp	r8, r5
300087c2:	bfb8      	it	lt
300087c4:	e9cd 7604 	strdlt	r7, r6, [sp, #16]
300087c8:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
300087cc:	bfbc      	itt	lt
300087ce:	46a8      	movlt	r8, r5
300087d0:	465c      	movlt	r4, fp
300087d2:	2500      	movs	r5, #0
300087d4:	463e      	mov	r6, r7
300087d6:	e784      	b.n	300086e2 <PSRAM_calibration+0x176>
300087d8:	2000      	movs	r0, #0
300087da:	e7ec      	b.n	300087b6 <PSRAM_calibration+0x24a>
300087dc:	41002000 	.word	0x41002000
300087e0:	e000ed00 	.word	0xe000ed00
300087e4:	60050000 	.word	0x60050000
300087e8:	60250000 	.word	0x60250000
300087ec:	3000df6c 	.word	0x3000df6c
300087f0:	000129bd 	.word	0x000129bd
300087f4:	3000bb45 	.word	0x3000bb45
300087f8:	3000bb20 	.word	0x3000bb20
300087fc:	60150000 	.word	0x60150000
30008800:	60200000 	.word	0x60200000

30008804 <PSRAM_AutoGating>:
30008804:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30008808:	b1a8      	cbz	r0, 30008836 <PSRAM_AutoGating+0x32>
3000880a:	f8d3 01e4 	ldr.w	r0, [r3, #484]	; 0x1e4
3000880e:	f3c1 0108 	ubfx	r1, r1, #0, #9
30008812:	0252      	lsls	r2, r2, #9
30008814:	f420 507f 	bic.w	r0, r0, #16320	; 0x3fc0
30008818:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
3000881c:	f020 003f 	bic.w	r0, r0, #63	; 0x3f
30008820:	4301      	orrs	r1, r0
30008822:	430a      	orrs	r2, r1
30008824:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30008828:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
3000882c:	f042 0201 	orr.w	r2, r2, #1
30008830:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30008834:	4770      	bx	lr
30008836:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
3000883a:	f022 0201 	bic.w	r2, r2, #1
3000883e:	e7f7      	b.n	30008830 <PSRAM_AutoGating+0x2c>

30008840 <BOOT_Reason>:
30008840:	4b02      	ldr	r3, [pc, #8]	; (3000884c <BOOT_Reason+0xc>)
30008842:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30008846:	b280      	uxth	r0, r0
30008848:	4770      	bx	lr
3000884a:	bf00      	nop
3000884c:	42008200 	.word	0x42008200

30008850 <CRYPTO_SetSecurityModeAD>:
30008850:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
30008854:	460d      	mov	r5, r1
30008856:	4616      	mov	r6, r2
30008858:	4699      	mov	r9, r3
3000885a:	4604      	mov	r4, r0
3000885c:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
30008860:	b918      	cbnz	r0, 3000886a <CRYPTO_SetSecurityModeAD+0x1a>
30008862:	2124      	movs	r1, #36	; 0x24
30008864:	4852      	ldr	r0, [pc, #328]	; (300089b0 <CRYPTO_SetSecurityModeAD+0x160>)
30008866:	f002 fdeb 	bl	3000b440 <__io_assert_failed_veneer>
3000886a:	7823      	ldrb	r3, [r4, #0]
3000886c:	2b01      	cmp	r3, #1
3000886e:	d003      	beq.n	30008878 <CRYPTO_SetSecurityModeAD+0x28>
30008870:	2125      	movs	r1, #37	; 0x25
30008872:	484f      	ldr	r0, [pc, #316]	; (300089b0 <CRYPTO_SetSecurityModeAD+0x160>)
30008874:	f002 fde4 	bl	3000b440 <__io_assert_failed_veneer>
30008878:	2300      	movs	r3, #0
3000887a:	6065      	str	r5, [r4, #4]
3000887c:	8123      	strh	r3, [r4, #8]
3000887e:	1c6b      	adds	r3, r5, #1
30008880:	d00b      	beq.n	3000889a <CRYPTO_SetSecurityModeAD+0x4a>
30008882:	f005 0330 	and.w	r3, r5, #48	; 0x30
30008886:	f085 0580 	eor.w	r5, r5, #128	; 0x80
3000888a:	2b20      	cmp	r3, #32
3000888c:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30008890:	bf08      	it	eq
30008892:	2301      	moveq	r3, #1
30008894:	7265      	strb	r5, [r4, #9]
30008896:	bf08      	it	eq
30008898:	7223      	strbeq	r3, [r4, #8]
3000889a:	1c70      	adds	r0, r6, #1
3000889c:	60e6      	str	r6, [r4, #12]
3000889e:	d074      	beq.n	3000898a <CRYPTO_SetSecurityModeAD+0x13a>
300088a0:	f3c6 0380 	ubfx	r3, r6, #2, #1
300088a4:	7423      	strb	r3, [r4, #16]
300088a6:	f006 0303 	and.w	r3, r6, #3
300088aa:	1e99      	subs	r1, r3, #2
300088ac:	424a      	negs	r2, r1
300088ae:	414a      	adcs	r2, r1
300088b0:	2b01      	cmp	r3, #1
300088b2:	7462      	strb	r2, [r4, #17]
300088b4:	fab3 f283 	clz	r2, r3
300088b8:	ea4f 1252 	mov.w	r2, r2, lsr #5
300088bc:	74a2      	strb	r2, [r4, #18]
300088be:	bf0c      	ite	eq
300088c0:	2201      	moveq	r2, #1
300088c2:	2200      	movne	r2, #0
300088c4:	74e2      	strb	r2, [r4, #19]
300088c6:	d159      	bne.n	3000897c <CRYPTO_SetSecurityModeAD+0x12c>
300088c8:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
300088cc:	2b40      	cmp	r3, #64	; 0x40
300088ce:	d053      	beq.n	30008978 <CRYPTO_SetSecurityModeAD+0x128>
300088d0:	d80c      	bhi.n	300088ec <CRYPTO_SetSecurityModeAD+0x9c>
300088d2:	2b10      	cmp	r3, #16
300088d4:	d00e      	beq.n	300088f4 <CRYPTO_SetSecurityModeAD+0xa4>
300088d6:	2b20      	cmp	r3, #32
300088d8:	d00d      	beq.n	300088f6 <CRYPTO_SetSecurityModeAD+0xa6>
300088da:	4b36      	ldr	r3, [pc, #216]	; (300089b4 <CRYPTO_SetSecurityModeAD+0x164>)
300088dc:	2002      	movs	r0, #2
300088de:	4a36      	ldr	r2, [pc, #216]	; (300089b8 <CRYPTO_SetSecurityModeAD+0x168>)
300088e0:	4619      	mov	r1, r3
300088e2:	9600      	str	r6, [sp, #0]
300088e4:	f002 fb1a 	bl	3000af1c <rtk_log_write>
300088e8:	2300      	movs	r3, #0
300088ea:	e004      	b.n	300088f6 <CRYPTO_SetSecurityModeAD+0xa6>
300088ec:	2b80      	cmp	r3, #128	; 0x80
300088ee:	d1f4      	bne.n	300088da <CRYPTO_SetSecurityModeAD+0x8a>
300088f0:	2340      	movs	r3, #64	; 0x40
300088f2:	e000      	b.n	300088f6 <CRYPTO_SetSecurityModeAD+0xa6>
300088f4:	231c      	movs	r3, #28
300088f6:	7523      	strb	r3, [r4, #20]
300088f8:	7d23      	ldrb	r3, [r4, #20]
300088fa:	62a3      	str	r3, [r4, #40]	; 0x28
300088fc:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300088fe:	6227      	str	r7, [r4, #32]
30008900:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30008904:	2f00      	cmp	r7, #0
30008906:	d050      	beq.n	300089aa <CRYPTO_SetSecurityModeAD+0x15a>
30008908:	7d23      	ldrb	r3, [r4, #20]
3000890a:	f104 0064 	add.w	r0, r4, #100	; 0x64
3000890e:	4d2b      	ldr	r5, [pc, #172]	; (300089bc <CRYPTO_SetSecurityModeAD+0x16c>)
30008910:	2b30      	cmp	r3, #48	; 0x30
30008912:	65e0      	str	r0, [r4, #92]	; 0x5c
30008914:	d001      	beq.n	3000891a <CRYPTO_SetSecurityModeAD+0xca>
30008916:	2b40      	cmp	r3, #64	; 0x40
30008918:	d13f      	bne.n	3000899a <CRYPTO_SetSecurityModeAD+0x14a>
3000891a:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
3000891e:	2280      	movs	r2, #128	; 0x80
30008920:	2136      	movs	r1, #54	; 0x36
30008922:	6623      	str	r3, [r4, #96]	; 0x60
30008924:	47a8      	blx	r5
30008926:	2280      	movs	r2, #128	; 0x80
30008928:	215c      	movs	r1, #92	; 0x5c
3000892a:	6e20      	ldr	r0, [r4, #96]	; 0x60
3000892c:	47a8      	blx	r5
3000892e:	4b24      	ldr	r3, [pc, #144]	; (300089c0 <CRYPTO_SetSecurityModeAD+0x170>)
30008930:	681d      	ldr	r5, [r3, #0]
30008932:	b9bd      	cbnz	r5, 30008964 <CRYPTO_SetSecurityModeAD+0x114>
30008934:	f1b8 0f00 	cmp.w	r8, #0
30008938:	d103      	bne.n	30008942 <CRYPTO_SetSecurityModeAD+0xf2>
3000893a:	2180      	movs	r1, #128	; 0x80
3000893c:	481c      	ldr	r0, [pc, #112]	; (300089b0 <CRYPTO_SetSecurityModeAD+0x160>)
3000893e:	f002 fd7f 	bl	3000b440 <__io_assert_failed_veneer>
30008942:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30008946:	6de2      	ldr	r2, [r4, #92]	; 0x5c
30008948:	6a63      	ldr	r3, [r4, #36]	; 0x24
3000894a:	5d51      	ldrb	r1, [r2, r5]
3000894c:	5d5b      	ldrb	r3, [r3, r5]
3000894e:	404b      	eors	r3, r1
30008950:	5553      	strb	r3, [r2, r5]
30008952:	6e22      	ldr	r2, [r4, #96]	; 0x60
30008954:	6a63      	ldr	r3, [r4, #36]	; 0x24
30008956:	5d51      	ldrb	r1, [r2, r5]
30008958:	5d5b      	ldrb	r3, [r3, r5]
3000895a:	404b      	eors	r3, r1
3000895c:	5553      	strb	r3, [r2, r5]
3000895e:	3501      	adds	r5, #1
30008960:	42af      	cmp	r7, r5
30008962:	d1f0      	bne.n	30008946 <CRYPTO_SetSecurityModeAD+0xf6>
30008964:	3601      	adds	r6, #1
30008966:	d003      	beq.n	30008970 <CRYPTO_SetSecurityModeAD+0x120>
30008968:	2301      	movs	r3, #1
3000896a:	85a3      	strh	r3, [r4, #44]	; 0x2c
3000896c:	2300      	movs	r3, #0
3000896e:	6323      	str	r3, [r4, #48]	; 0x30
30008970:	2000      	movs	r0, #0
30008972:	b003      	add	sp, #12
30008974:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008978:	2330      	movs	r3, #48	; 0x30
3000897a:	e7bc      	b.n	300088f6 <CRYPTO_SetSecurityModeAD+0xa6>
3000897c:	2200      	movs	r2, #0
3000897e:	7522      	strb	r2, [r4, #20]
30008980:	b14b      	cbz	r3, 30008996 <CRYPTO_SetSecurityModeAD+0x146>
30008982:	2b02      	cmp	r3, #2
30008984:	d005      	beq.n	30008992 <CRYPTO_SetSecurityModeAD+0x142>
30008986:	2300      	movs	r3, #0
30008988:	e7b7      	b.n	300088fa <CRYPTO_SetSecurityModeAD+0xaa>
3000898a:	2300      	movs	r3, #0
3000898c:	6123      	str	r3, [r4, #16]
3000898e:	7523      	strb	r3, [r4, #20]
30008990:	e7f9      	b.n	30008986 <CRYPTO_SetSecurityModeAD+0x136>
30008992:	2310      	movs	r3, #16
30008994:	e7b1      	b.n	300088fa <CRYPTO_SetSecurityModeAD+0xaa>
30008996:	2314      	movs	r3, #20
30008998:	e7af      	b.n	300088fa <CRYPTO_SetSecurityModeAD+0xaa>
3000899a:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
3000899e:	2240      	movs	r2, #64	; 0x40
300089a0:	2136      	movs	r1, #54	; 0x36
300089a2:	6623      	str	r3, [r4, #96]	; 0x60
300089a4:	47a8      	blx	r5
300089a6:	2240      	movs	r2, #64	; 0x40
300089a8:	e7be      	b.n	30008928 <CRYPTO_SetSecurityModeAD+0xd8>
300089aa:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
300089ae:	e7d9      	b.n	30008964 <CRYPTO_SetSecurityModeAD+0x114>
300089b0:	3000bc15 	.word	0x3000bc15
300089b4:	3000bb82 	.word	0x3000bb82
300089b8:	3000bb8b 	.word	0x3000bb8b
300089bc:	00012be5 	.word	0x00012be5
300089c0:	2001c200 	.word	0x2001c200

300089c4 <rtl_crypto_hmac_sha2_init>:
300089c4:	e92d 41ff 	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
300089c8:	4617      	mov	r7, r2
300089ca:	461c      	mov	r4, r3
300089cc:	4a82      	ldr	r2, [pc, #520]	; (30008bd8 <rtl_crypto_hmac_sha2_init+0x214>)
300089ce:	4605      	mov	r5, r0
300089d0:	4b82      	ldr	r3, [pc, #520]	; (30008bdc <rtl_crypto_hmac_sha2_init+0x218>)
300089d2:	460e      	mov	r6, r1
300089d4:	e843 f300 	tt	r3, r3
300089d8:	f413 0f80 	tst.w	r3, #4194304	; 0x400000
300089dc:	4b80      	ldr	r3, [pc, #512]	; (30008be0 <rtl_crypto_hmac_sha2_init+0x21c>)
300089de:	bf18      	it	ne
300089e0:	4613      	movne	r3, r2
300089e2:	6b5b      	ldr	r3, [r3, #52]	; 0x34
300089e4:	2b00      	cmp	r3, #0
300089e6:	db05      	blt.n	300089f4 <rtl_crypto_hmac_sha2_init+0x30>
300089e8:	2900      	cmp	r1, #0
300089ea:	f000 80ef 	beq.w	30008bcc <rtl_crypto_hmac_sha2_init+0x208>
300089ee:	078b      	lsls	r3, r1, #30
300089f0:	f040 80ef 	bne.w	30008bd2 <rtl_crypto_hmac_sha2_init+0x20e>
300089f4:	2d1c      	cmp	r5, #28
300089f6:	d001      	beq.n	300089fc <rtl_crypto_hmac_sha2_init+0x38>
300089f8:	2d20      	cmp	r5, #32
300089fa:	d11e      	bne.n	30008a3a <rtl_crypto_hmac_sha2_init+0x76>
300089fc:	2f40      	cmp	r7, #64	; 0x40
300089fe:	d903      	bls.n	30008a08 <rtl_crypto_hmac_sha2_init+0x44>
30008a00:	21ae      	movs	r1, #174	; 0xae
30008a02:	4878      	ldr	r0, [pc, #480]	; (30008be4 <rtl_crypto_hmac_sha2_init+0x220>)
30008a04:	f002 fd1c 	bl	3000b440 <__io_assert_failed_veneer>
30008a08:	f8df 8218 	ldr.w	r8, [pc, #536]	; 30008c24 <rtl_crypto_hmac_sha2_init+0x260>
30008a0c:	f898 3000 	ldrb.w	r3, [r8]
30008a10:	2b01      	cmp	r3, #1
30008a12:	d003      	beq.n	30008a1c <rtl_crypto_hmac_sha2_init+0x58>
30008a14:	21b4      	movs	r1, #180	; 0xb4
30008a16:	4873      	ldr	r0, [pc, #460]	; (30008be4 <rtl_crypto_hmac_sha2_init+0x220>)
30008a18:	f002 fd12 	bl	3000b440 <__io_assert_failed_veneer>
30008a1c:	2d30      	cmp	r5, #48	; 0x30
30008a1e:	f000 80a0 	beq.w	30008b62 <rtl_crypto_hmac_sha2_init+0x19e>
30008a22:	d812      	bhi.n	30008a4a <rtl_crypto_hmac_sha2_init+0x86>
30008a24:	2d1c      	cmp	r5, #28
30008a26:	d04f      	beq.n	30008ac8 <rtl_crypto_hmac_sha2_init+0x104>
30008a28:	2d20      	cmp	r5, #32
30008a2a:	f000 8082 	beq.w	30008b32 <rtl_crypto_hmac_sha2_init+0x16e>
30008a2e:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
30008a32:	4628      	mov	r0, r5
30008a34:	b004      	add	sp, #16
30008a36:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008a3a:	2d30      	cmp	r5, #48	; 0x30
30008a3c:	d001      	beq.n	30008a42 <rtl_crypto_hmac_sha2_init+0x7e>
30008a3e:	2d40      	cmp	r5, #64	; 0x40
30008a40:	d1e2      	bne.n	30008a08 <rtl_crypto_hmac_sha2_init+0x44>
30008a42:	2f80      	cmp	r7, #128	; 0x80
30008a44:	d9e0      	bls.n	30008a08 <rtl_crypto_hmac_sha2_init+0x44>
30008a46:	21b1      	movs	r1, #177	; 0xb1
30008a48:	e7db      	b.n	30008a02 <rtl_crypto_hmac_sha2_init+0x3e>
30008a4a:	2d40      	cmp	r5, #64	; 0x40
30008a4c:	d1ef      	bne.n	30008a2e <rtl_crypto_hmac_sha2_init+0x6a>
30008a4e:	2c00      	cmp	r4, #0
30008a50:	d038      	beq.n	30008ac4 <rtl_crypto_hmac_sha2_init+0x100>
30008a52:	4b65      	ldr	r3, [pc, #404]	; (30008be8 <rtl_crypto_hmac_sha2_init+0x224>)
30008a54:	4a65      	ldr	r2, [pc, #404]	; (30008bec <rtl_crypto_hmac_sha2_init+0x228>)
30008a56:	4966      	ldr	r1, [pc, #408]	; (30008bf0 <rtl_crypto_hmac_sha2_init+0x22c>)
30008a58:	4d66      	ldr	r5, [pc, #408]	; (30008bf4 <rtl_crypto_hmac_sha2_init+0x230>)
30008a5a:	f8df c1f8 	ldr.w	ip, [pc, #504]	; 30008c54 <rtl_crypto_hmac_sha2_init+0x290>
30008a5e:	e9c4 3202 	strd	r3, r2, [r4, #8]
30008a62:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30008a66:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30008a6a:	33d4      	adds	r3, #212	; 0xd4
30008a6c:	6163      	str	r3, [r4, #20]
30008a6e:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30008a72:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30008a76:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30008a7a:	6123      	str	r3, [r4, #16]
30008a7c:	4b5e      	ldr	r3, [pc, #376]	; (30008bf8 <rtl_crypto_hmac_sha2_init+0x234>)
30008a7e:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008a82:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30008a86:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30008a8a:	f203 337f 	addw	r3, r3, #895	; 0x37f
30008a8e:	6263      	str	r3, [r4, #36]	; 0x24
30008a90:	4b5a      	ldr	r3, [pc, #360]	; (30008bfc <rtl_crypto_hmac_sha2_init+0x238>)
30008a92:	6223      	str	r3, [r4, #32]
30008a94:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30008a98:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30008a9c:	f203 5397 	addw	r3, r3, #1431	; 0x597
30008aa0:	62e3      	str	r3, [r4, #44]	; 0x2c
30008aa2:	4b57      	ldr	r3, [pc, #348]	; (30008c00 <rtl_crypto_hmac_sha2_init+0x23c>)
30008aa4:	62a3      	str	r3, [r4, #40]	; 0x28
30008aa6:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30008aaa:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30008aae:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30008ab2:	6363      	str	r3, [r4, #52]	; 0x34
30008ab4:	4b53      	ldr	r3, [pc, #332]	; (30008c04 <rtl_crypto_hmac_sha2_init+0x240>)
30008ab6:	6323      	str	r3, [r4, #48]	; 0x30
30008ab8:	4b53      	ldr	r3, [pc, #332]	; (30008c08 <rtl_crypto_hmac_sha2_init+0x244>)
30008aba:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008abe:	4b53      	ldr	r3, [pc, #332]	; (30008c0c <rtl_crypto_hmac_sha2_init+0x248>)
30008ac0:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008ac4:	2285      	movs	r2, #133	; 0x85
30008ac6:	e01d      	b.n	30008b04 <rtl_crypto_hmac_sha2_init+0x140>
30008ac8:	b1dc      	cbz	r4, 30008b02 <rtl_crypto_hmac_sha2_init+0x13e>
30008aca:	4b51      	ldr	r3, [pc, #324]	; (30008c10 <rtl_crypto_hmac_sha2_init+0x24c>)
30008acc:	4d51      	ldr	r5, [pc, #324]	; (30008c14 <rtl_crypto_hmac_sha2_init+0x250>)
30008ace:	f8df c164 	ldr.w	ip, [pc, #356]	; 30008c34 <rtl_crypto_hmac_sha2_init+0x270>
30008ad2:	f8df e184 	ldr.w	lr, [pc, #388]	; 30008c58 <rtl_crypto_hmac_sha2_init+0x294>
30008ad6:	e9c4 5302 	strd	r5, r3, [r4, #8]
30008ada:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
30008ade:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
30008ae2:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30008ae6:	6123      	str	r3, [r4, #16]
30008ae8:	4b4b      	ldr	r3, [pc, #300]	; (30008c18 <rtl_crypto_hmac_sha2_init+0x254>)
30008aea:	e9c4 c305 	strd	ip, r3, [r4, #20]
30008aee:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30008af2:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30008af6:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30008afa:	61e3      	str	r3, [r4, #28]
30008afc:	4b47      	ldr	r3, [pc, #284]	; (30008c1c <rtl_crypto_hmac_sha2_init+0x258>)
30008afe:	e9c4 e308 	strd	lr, r3, [r4, #32]
30008b02:	2215      	movs	r2, #21
30008b04:	2300      	movs	r3, #0
30008b06:	4d46      	ldr	r5, [pc, #280]	; (30008c20 <rtl_crypto_hmac_sha2_init+0x25c>)
30008b08:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008b0c:	4845      	ldr	r0, [pc, #276]	; (30008c24 <rtl_crypto_hmac_sha2_init+0x260>)
30008b0e:	9300      	str	r3, [sp, #0]
30008b10:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30008b14:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30008b18:	e9cd 6701 	strd	r6, r7, [sp, #4]
30008b1c:	47a8      	blx	r5
30008b1e:	4605      	mov	r5, r0
30008b20:	2c00      	cmp	r4, #0
30008b22:	d086      	beq.n	30008a32 <rtl_crypto_hmac_sha2_init+0x6e>
30008b24:	4b40      	ldr	r3, [pc, #256]	; (30008c28 <rtl_crypto_hmac_sha2_init+0x264>)
30008b26:	4620      	mov	r0, r4
30008b28:	4798      	blx	r3
30008b2a:	2301      	movs	r3, #1
30008b2c:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30008b30:	e77f      	b.n	30008a32 <rtl_crypto_hmac_sha2_init+0x6e>
30008b32:	b1a4      	cbz	r4, 30008b5e <rtl_crypto_hmac_sha2_init+0x19a>
30008b34:	4b3d      	ldr	r3, [pc, #244]	; (30008c2c <rtl_crypto_hmac_sha2_init+0x268>)
30008b36:	4a2c      	ldr	r2, [pc, #176]	; (30008be8 <rtl_crypto_hmac_sha2_init+0x224>)
30008b38:	492f      	ldr	r1, [pc, #188]	; (30008bf8 <rtl_crypto_hmac_sha2_init+0x234>)
30008b3a:	4831      	ldr	r0, [pc, #196]	; (30008c00 <rtl_crypto_hmac_sha2_init+0x23c>)
30008b3c:	e9c4 2302 	strd	r2, r3, [r4, #8]
30008b40:	4b2e      	ldr	r3, [pc, #184]	; (30008bfc <rtl_crypto_hmac_sha2_init+0x238>)
30008b42:	e9c4 1304 	strd	r1, r3, [r4, #16]
30008b46:	4b2f      	ldr	r3, [pc, #188]	; (30008c04 <rtl_crypto_hmac_sha2_init+0x240>)
30008b48:	e9c4 0306 	strd	r0, r3, [r4, #24]
30008b4c:	4b2e      	ldr	r3, [pc, #184]	; (30008c08 <rtl_crypto_hmac_sha2_init+0x244>)
30008b4e:	6223      	str	r3, [r4, #32]
30008b50:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30008b54:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30008b58:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30008b5c:	6263      	str	r3, [r4, #36]	; 0x24
30008b5e:	2225      	movs	r2, #37	; 0x25
30008b60:	e7d0      	b.n	30008b04 <rtl_crypto_hmac_sha2_init+0x140>
30008b62:	2c00      	cmp	r4, #0
30008b64:	d030      	beq.n	30008bc8 <rtl_crypto_hmac_sha2_init+0x204>
30008b66:	4b32      	ldr	r3, [pc, #200]	; (30008c30 <rtl_crypto_hmac_sha2_init+0x26c>)
30008b68:	f8df e0a8 	ldr.w	lr, [pc, #168]	; 30008c14 <rtl_crypto_hmac_sha2_init+0x250>
30008b6c:	4931      	ldr	r1, [pc, #196]	; (30008c34 <rtl_crypto_hmac_sha2_init+0x270>)
30008b6e:	482a      	ldr	r0, [pc, #168]	; (30008c18 <rtl_crypto_hmac_sha2_init+0x254>)
30008b70:	f8df c0e4 	ldr.w	ip, [pc, #228]	; 30008c58 <rtl_crypto_hmac_sha2_init+0x294>
30008b74:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30008b78:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30008b7c:	f8df e09c 	ldr.w	lr, [pc, #156]	; 30008c1c <rtl_crypto_hmac_sha2_init+0x258>
30008b80:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30008b84:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30008b88:	6163      	str	r3, [r4, #20]
30008b8a:	4b2b      	ldr	r3, [pc, #172]	; (30008c38 <rtl_crypto_hmac_sha2_init+0x274>)
30008b8c:	6123      	str	r3, [r4, #16]
30008b8e:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30008b92:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30008b96:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30008b9a:	61e3      	str	r3, [r4, #28]
30008b9c:	4b27      	ldr	r3, [pc, #156]	; (30008c3c <rtl_crypto_hmac_sha2_init+0x278>)
30008b9e:	61a3      	str	r3, [r4, #24]
30008ba0:	4b27      	ldr	r3, [pc, #156]	; (30008c40 <rtl_crypto_hmac_sha2_init+0x27c>)
30008ba2:	e9c4 3108 	strd	r3, r1, [r4, #32]
30008ba6:	4b27      	ldr	r3, [pc, #156]	; (30008c44 <rtl_crypto_hmac_sha2_init+0x280>)
30008ba8:	e9c4 300a 	strd	r3, r0, [r4, #40]	; 0x28
30008bac:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30008bb0:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30008bb4:	33aa      	adds	r3, #170	; 0xaa
30008bb6:	6363      	str	r3, [r4, #52]	; 0x34
30008bb8:	4b23      	ldr	r3, [pc, #140]	; (30008c48 <rtl_crypto_hmac_sha2_init+0x284>)
30008bba:	6323      	str	r3, [r4, #48]	; 0x30
30008bbc:	4b23      	ldr	r3, [pc, #140]	; (30008c4c <rtl_crypto_hmac_sha2_init+0x288>)
30008bbe:	e9c4 3c0e 	strd	r3, ip, [r4, #56]	; 0x38
30008bc2:	4b23      	ldr	r3, [pc, #140]	; (30008c50 <rtl_crypto_hmac_sha2_init+0x28c>)
30008bc4:	e9c4 3e10 	strd	r3, lr, [r4, #64]	; 0x40
30008bc8:	2245      	movs	r2, #69	; 0x45
30008bca:	e79b      	b.n	30008b04 <rtl_crypto_hmac_sha2_init+0x140>
30008bcc:	f06f 0503 	mvn.w	r5, #3
30008bd0:	e72f      	b.n	30008a32 <rtl_crypto_hmac_sha2_init+0x6e>
30008bd2:	f06f 0505 	mvn.w	r5, #5
30008bd6:	e72c      	b.n	30008a32 <rtl_crypto_hmac_sha2_init+0x6e>
30008bd8:	500c8000 	.word	0x500c8000
30008bdc:	0000e5f9 	.word	0x0000e5f9
30008be0:	400c8000 	.word	0x400c8000
30008be4:	3000bbfb 	.word	0x3000bbfb
30008be8:	6a09e667 	.word	0x6a09e667
30008bec:	f3bcc908 	.word	0xf3bcc908
30008bf0:	fe94f82b 	.word	0xfe94f82b
30008bf4:	fb41bd6b 	.word	0xfb41bd6b
30008bf8:	3c6ef372 	.word	0x3c6ef372
30008bfc:	a54ff53a 	.word	0xa54ff53a
30008c00:	510e527f 	.word	0x510e527f
30008c04:	9b05688c 	.word	0x9b05688c
30008c08:	1f83d9ab 	.word	0x1f83d9ab
30008c0c:	5be0cd19 	.word	0x5be0cd19
30008c10:	367cd507 	.word	0x367cd507
30008c14:	c1059ed8 	.word	0xc1059ed8
30008c18:	ffc00b31 	.word	0xffc00b31
30008c1c:	befa4fa4 	.word	0xbefa4fa4
30008c20:	30008851 	.word	0x30008851
30008c24:	2001c460 	.word	0x2001c460
30008c28:	00004c99 	.word	0x00004c99
30008c2c:	bb67ae85 	.word	0xbb67ae85
30008c30:	cbbb9d5d 	.word	0xcbbb9d5d
30008c34:	f70e5939 	.word	0xf70e5939
30008c38:	629a292a 	.word	0x629a292a
30008c3c:	9159015a 	.word	0x9159015a
30008c40:	152fecd8 	.word	0x152fecd8
30008c44:	67332667 	.word	0x67332667
30008c48:	8eb44a87 	.word	0x8eb44a87
30008c4c:	db0c2e0d 	.word	0xdb0c2e0d
30008c50:	47b5481d 	.word	0x47b5481d
30008c54:	137e2179 	.word	0x137e2179
30008c58:	64f98fa7 	.word	0x64f98fa7

30008c5c <CPU_ClkGet>:
30008c5c:	b570      	push	{r4, r5, r6, lr}
30008c5e:	4c1d      	ldr	r4, [pc, #116]	; (30008cd4 <CPU_ClkGet+0x78>)
30008c60:	4b1d      	ldr	r3, [pc, #116]	; (30008cd8 <CPU_ClkGet+0x7c>)
30008c62:	f8d4 6224 	ldr.w	r6, [r4, #548]	; 0x224
30008c66:	f8d4 527c 	ldr.w	r5, [r4, #636]	; 0x27c
30008c6a:	f3c5 1581 	ubfx	r5, r5, #6, #2
30008c6e:	4798      	blx	r3
30008c70:	b93d      	cbnz	r5, 30008c82 <CPU_ClkGet+0x26>
30008c72:	2801      	cmp	r0, #1
30008c74:	d025      	beq.n	30008cc2 <CPU_ClkGet+0x66>
30008c76:	07b3      	lsls	r3, r6, #30
30008c78:	d525      	bpl.n	30008cc6 <CPU_ClkGet+0x6a>
30008c7a:	4b18      	ldr	r3, [pc, #96]	; (30008cdc <CPU_ClkGet+0x80>)
30008c7c:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008c80:	4718      	bx	r3
30008c82:	2d01      	cmp	r5, #1
30008c84:	d121      	bne.n	30008cca <CPU_ClkGet+0x6e>
30008c86:	2801      	cmp	r0, #1
30008c88:	d109      	bne.n	30008c9e <CPU_ClkGet+0x42>
30008c8a:	f8d4 326c 	ldr.w	r3, [r4, #620]	; 0x26c
30008c8e:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30008c92:	f3c3 4007 	ubfx	r0, r3, #16, #8
30008c96:	d01a      	beq.n	30008cce <CPU_ClkGet+0x72>
30008c98:	4b11      	ldr	r3, [pc, #68]	; (30008ce0 <CPU_ClkGet+0x84>)
30008c9a:	4358      	muls	r0, r3
30008c9c:	bd70      	pop	{r4, r5, r6, pc}
30008c9e:	f416 7f80 	tst.w	r6, #256	; 0x100
30008ca2:	4810      	ldr	r0, [pc, #64]	; (30008ce4 <CPU_ClkGet+0x88>)
30008ca4:	d0fa      	beq.n	30008c9c <CPU_ClkGet+0x40>
30008ca6:	4b10      	ldr	r3, [pc, #64]	; (30008ce8 <CPU_ClkGet+0x8c>)
30008ca8:	f8d4 2228 	ldr.w	r2, [r4, #552]	; 0x228
30008cac:	6c5b      	ldr	r3, [r3, #68]	; 0x44
30008cae:	f3c3 1345 	ubfx	r3, r3, #5, #6
30008cb2:	3302      	adds	r3, #2
30008cb4:	4358      	muls	r0, r3
30008cb6:	f002 0307 	and.w	r3, r2, #7
30008cba:	3301      	adds	r3, #1
30008cbc:	fbb0 f0f3 	udiv	r0, r0, r3
30008cc0:	e7ec      	b.n	30008c9c <CPU_ClkGet+0x40>
30008cc2:	480a      	ldr	r0, [pc, #40]	; (30008cec <CPU_ClkGet+0x90>)
30008cc4:	e7ea      	b.n	30008c9c <CPU_ClkGet+0x40>
30008cc6:	480a      	ldr	r0, [pc, #40]	; (30008cf0 <CPU_ClkGet+0x94>)
30008cc8:	e7e8      	b.n	30008c9c <CPU_ClkGet+0x40>
30008cca:	2000      	movs	r0, #0
30008ccc:	e7e6      	b.n	30008c9c <CPU_ClkGet+0x40>
30008cce:	4809      	ldr	r0, [pc, #36]	; (30008cf4 <CPU_ClkGet+0x98>)
30008cd0:	e7e4      	b.n	30008c9c <CPU_ClkGet+0x40>
30008cd2:	bf00      	nop
30008cd4:	42008000 	.word	0x42008000
30008cd8:	0000c0f9 	.word	0x0000c0f9
30008cdc:	000099f5 	.word	0x000099f5
30008ce0:	000f4240 	.word	0x000f4240
30008ce4:	02625a00 	.word	0x02625a00
30008ce8:	42008800 	.word	0x42008800
30008cec:	01312d00 	.word	0x01312d00
30008cf0:	003d0900 	.word	0x003d0900
30008cf4:	02faf080 	.word	0x02faf080

30008cf8 <RSIP_IV_Set>:
30008cf8:	00c0      	lsls	r0, r0, #3
30008cfa:	680b      	ldr	r3, [r1, #0]
30008cfc:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30008d00:	ba1b      	rev	r3, r3
30008d02:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008d06:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008d0a:	684b      	ldr	r3, [r1, #4]
30008d0c:	ba1b      	rev	r3, r3
30008d0e:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30008d12:	4770      	bx	lr

30008d14 <RSIP_OTF_Cmd>:
30008d14:	4b04      	ldr	r3, [pc, #16]	; (30008d28 <RSIP_OTF_Cmd+0x14>)
30008d16:	2801      	cmp	r0, #1
30008d18:	695a      	ldr	r2, [r3, #20]
30008d1a:	bf0c      	ite	eq
30008d1c:	f442 7280 	orreq.w	r2, r2, #256	; 0x100
30008d20:	f422 7280 	bicne.w	r2, r2, #256	; 0x100
30008d24:	615a      	str	r2, [r3, #20]
30008d26:	4770      	bx	lr
30008d28:	42008c00 	.word	0x42008c00

30008d2c <RSIP_OTF_Enable>:
30008d2c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008d30:	4688      	mov	r8, r1
30008d32:	4617      	mov	r7, r2
30008d34:	00c4      	lsls	r4, r0, #3
30008d36:	e9dd 6506 	ldrd	r6, r5, [sp, #24]
30008d3a:	b32b      	cbz	r3, 30008d88 <RSIP_OTF_Enable+0x5c>
30008d3c:	2e07      	cmp	r6, #7
30008d3e:	d904      	bls.n	30008d4a <RSIP_OTF_Enable+0x1e>
30008d40:	f44f 710c 	mov.w	r1, #560	; 0x230
30008d44:	4815      	ldr	r0, [pc, #84]	; (30008d9c <RSIP_OTF_Enable+0x70>)
30008d46:	f002 fb7b 	bl	3000b440 <__io_assert_failed_veneer>
30008d4a:	1e6b      	subs	r3, r5, #1
30008d4c:	2b01      	cmp	r3, #1
30008d4e:	d904      	bls.n	30008d5a <RSIP_OTF_Enable+0x2e>
30008d50:	f240 2131 	movw	r1, #561	; 0x231
30008d54:	4811      	ldr	r0, [pc, #68]	; (30008d9c <RSIP_OTF_Enable+0x70>)
30008d56:	f002 fb73 	bl	3000b440 <__io_assert_failed_veneer>
30008d5a:	f104 4084 	add.w	r0, r4, #1107296256	; 0x42000000
30008d5e:	006d      	lsls	r5, r5, #1
30008d60:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008d64:	f005 0506 	and.w	r5, r5, #6
30008d68:	6c43      	ldr	r3, [r0, #68]	; 0x44
30008d6a:	ea45 0207 	orr.w	r2, r5, r7
30008d6e:	f023 0301 	bic.w	r3, r3, #1
30008d72:	f042 0201 	orr.w	r2, r2, #1
30008d76:	6443      	str	r3, [r0, #68]	; 0x44
30008d78:	f006 0307 	and.w	r3, r6, #7
30008d7c:	ea43 0308 	orr.w	r3, r3, r8
30008d80:	6403      	str	r3, [r0, #64]	; 0x40
30008d82:	6442      	str	r2, [r0, #68]	; 0x44
30008d84:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008d88:	f104 4084 	add.w	r0, r4, #1107296256	; 0x42000000
30008d8c:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008d90:	6c43      	ldr	r3, [r0, #68]	; 0x44
30008d92:	f023 0301 	bic.w	r3, r3, #1
30008d96:	6443      	str	r3, [r0, #68]	; 0x44
30008d98:	e7f4      	b.n	30008d84 <RSIP_OTF_Enable+0x58>
30008d9a:	bf00      	nop
30008d9c:	3000bbeb 	.word	0x3000bbeb

30008da0 <RSIP_MMU_Config>:
30008da0:	b510      	push	{r4, lr}
30008da2:	240c      	movs	r4, #12
30008da4:	311f      	adds	r1, #31
30008da6:	321f      	adds	r2, #31
30008da8:	331f      	adds	r3, #31
30008daa:	4360      	muls	r0, r4
30008dac:	0949      	lsrs	r1, r1, #5
30008dae:	0952      	lsrs	r2, r2, #5
30008db0:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30008db4:	095b      	lsrs	r3, r3, #5
30008db6:	0249      	lsls	r1, r1, #9
30008db8:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008dbc:	0252      	lsls	r2, r2, #9
30008dbe:	025b      	lsls	r3, r3, #9
30008dc0:	f8d0 4108 	ldr.w	r4, [r0, #264]	; 0x108
30008dc4:	f024 0401 	bic.w	r4, r4, #1
30008dc8:	f8c0 4108 	str.w	r4, [r0, #264]	; 0x108
30008dcc:	f8c0 1100 	str.w	r1, [r0, #256]	; 0x100
30008dd0:	f8c0 2104 	str.w	r2, [r0, #260]	; 0x104
30008dd4:	f8c0 3108 	str.w	r3, [r0, #264]	; 0x108
30008dd8:	bd10      	pop	{r4, pc}
	...

30008ddc <RSIP_MMU_Cmd>:
30008ddc:	4b0a      	ldr	r3, [pc, #40]	; (30008e08 <RSIP_MMU_Cmd+0x2c>)
30008dde:	220c      	movs	r2, #12
30008de0:	b141      	cbz	r1, 30008df4 <RSIP_MMU_Cmd+0x18>
30008de2:	fb02 3000 	mla	r0, r2, r0, r3
30008de6:	f8d0 3108 	ldr.w	r3, [r0, #264]	; 0x108
30008dea:	f043 0301 	orr.w	r3, r3, #1
30008dee:	f8c0 3108 	str.w	r3, [r0, #264]	; 0x108
30008df2:	4770      	bx	lr
30008df4:	fb02 3300 	mla	r3, r2, r0, r3
30008df8:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
30008dfc:	f022 0201 	bic.w	r2, r2, #1
30008e00:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30008e04:	4770      	bx	lr
30008e06:	bf00      	nop
30008e08:	42008c00 	.word	0x42008c00

30008e0c <RSIP_MMU_Cache_Clean>:
30008e0c:	4a02      	ldr	r2, [pc, #8]	; (30008e18 <RSIP_MMU_Cache_Clean+0xc>)
30008e0e:	6993      	ldr	r3, [r2, #24]
30008e10:	f043 0304 	orr.w	r3, r3, #4
30008e14:	6193      	str	r3, [r2, #24]
30008e16:	4770      	bx	lr
30008e18:	42008c00 	.word	0x42008c00

30008e1c <NAND_UserModeEn>:
30008e1c:	4b07      	ldr	r3, [pc, #28]	; (30008e3c <NAND_UserModeEn+0x20>)
30008e1e:	b510      	push	{r4, lr}
30008e20:	4604      	mov	r4, r0
30008e22:	2000      	movs	r0, #0
30008e24:	4798      	blx	r3
30008e26:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30008e2a:	681a      	ldr	r2, [r3, #0]
30008e2c:	b11c      	cbz	r4, 30008e36 <NAND_UserModeEn+0x1a>
30008e2e:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008e32:	601a      	str	r2, [r3, #0]
30008e34:	bd10      	pop	{r4, pc}
30008e36:	f022 4200 	bic.w	r2, r2, #2147483648	; 0x80000000
30008e3a:	e7fa      	b.n	30008e32 <NAND_UserModeEn+0x16>
30008e3c:	0000b9c1 	.word	0x0000b9c1

30008e40 <NAND_RxData>:
30008e40:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30008e44:	460f      	mov	r7, r1
30008e46:	4615      	mov	r5, r2
30008e48:	461e      	mov	r6, r3
30008e4a:	4680      	mov	r8, r0
30008e4c:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30008f30 <NAND_RxData+0xf0>
30008e50:	b908      	cbnz	r0, 30008e56 <NAND_RxData+0x16>
30008e52:	f899 8008 	ldrb.w	r8, [r9, #8]
30008e56:	f04f 4488 	mov.w	r4, #1140850688	; 0x44000000
30008e5a:	2001      	movs	r0, #1
30008e5c:	f7ff ffde 	bl	30008e1c <NAND_UserModeEn>
30008e60:	2100      	movs	r1, #0
30008e62:	6823      	ldr	r3, [r4, #0]
30008e64:	f443 7340 	orr.w	r3, r3, #768	; 0x300
30008e68:	6023      	str	r3, [r4, #0]
30008e6a:	f025 437f 	bic.w	r3, r5, #4278190080	; 0xff000000
30008e6e:	6063      	str	r3, [r4, #4]
30008e70:	f8c4 1130 	str.w	r1, [r4, #304]	; 0x130
30008e74:	f899 0062 	ldrb.w	r0, [r9, #98]	; 0x62
30008e78:	f8d4 2118 	ldr.w	r2, [r4, #280]	; 0x118
30008e7c:	0403      	lsls	r3, r0, #16
30008e7e:	2803      	cmp	r0, #3
30008e80:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
30008e84:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
30008e88:	ea43 0302 	orr.w	r3, r3, r2
30008e8c:	f8c4 3118 	str.w	r3, [r4, #280]	; 0x118
30008e90:	f04f 0301 	mov.w	r3, #1
30008e94:	6123      	str	r3, [r4, #16]
30008e96:	f3c7 2307 	ubfx	r3, r7, #8, #8
30008e9a:	f884 8060 	strb.w	r8, [r4, #96]	; 0x60
30008e9e:	b2ff      	uxtb	r7, r7
30008ea0:	bf08      	it	eq
30008ea2:	f884 1060 	strbeq.w	r1, [r4, #96]	; 0x60
30008ea6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008eaa:	2301      	movs	r3, #1
30008eac:	f884 7060 	strb.w	r7, [r4, #96]	; 0x60
30008eb0:	60a3      	str	r3, [r4, #8]
30008eb2:	f016 0303 	ands.w	r3, r6, #3
30008eb6:	d127      	bne.n	30008f08 <NAND_RxData+0xc8>
30008eb8:	4619      	mov	r1, r3
30008eba:	1870      	adds	r0, r6, r1
30008ebc:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30008ec0:	42a9      	cmp	r1, r5
30008ec2:	d209      	bcs.n	30008ed8 <NAND_RxData+0x98>
30008ec4:	6a53      	ldr	r3, [r2, #36]	; 0x24
30008ec6:	f003 033c 	and.w	r3, r3, #60	; 0x3c
30008eca:	18c4      	adds	r4, r0, r3
30008ecc:	42a0      	cmp	r0, r4
30008ece:	d121      	bne.n	30008f14 <NAND_RxData+0xd4>
30008ed0:	4419      	add	r1, r3
30008ed2:	1a6b      	subs	r3, r5, r1
30008ed4:	2b03      	cmp	r3, #3
30008ed6:	d8f3      	bhi.n	30008ec0 <NAND_RxData+0x80>
30008ed8:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30008edc:	42a9      	cmp	r1, r5
30008ede:	d31d      	bcc.n	30008f1c <NAND_RxData+0xdc>
30008ee0:	4b12      	ldr	r3, [pc, #72]	; (30008f2c <NAND_RxData+0xec>)
30008ee2:	2004      	movs	r0, #4
30008ee4:	4798      	blx	r3
30008ee6:	2000      	movs	r0, #0
30008ee8:	e8bd 43f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30008eec:	f7ff bf96 	b.w	30008e1c <NAND_UserModeEn>
30008ef0:	6a90      	ldr	r0, [r2, #40]	; 0x28
30008ef2:	0700      	lsls	r0, r0, #28
30008ef4:	bf42      	ittt	mi
30008ef6:	f892 0060 	ldrbmi.w	r0, [r2, #96]	; 0x60
30008efa:	5470      	strbmi	r0, [r6, r1]
30008efc:	3101      	addmi	r1, #1
30008efe:	428b      	cmp	r3, r1
30008f00:	d0db      	beq.n	30008eba <NAND_RxData+0x7a>
30008f02:	42a9      	cmp	r1, r5
30008f04:	d3f4      	bcc.n	30008ef0 <NAND_RxData+0xb0>
30008f06:	e7d8      	b.n	30008eba <NAND_RxData+0x7a>
30008f08:	2100      	movs	r1, #0
30008f0a:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30008f0e:	f1c3 0304 	rsb	r3, r3, #4
30008f12:	e7f6      	b.n	30008f02 <NAND_RxData+0xc2>
30008f14:	6e17      	ldr	r7, [r2, #96]	; 0x60
30008f16:	f840 7b04 	str.w	r7, [r0], #4
30008f1a:	e7d7      	b.n	30008ecc <NAND_RxData+0x8c>
30008f1c:	6a9a      	ldr	r2, [r3, #40]	; 0x28
30008f1e:	0712      	lsls	r2, r2, #28
30008f20:	bf42      	ittt	mi
30008f22:	f893 2060 	ldrbmi.w	r2, [r3, #96]	; 0x60
30008f26:	5472      	strbmi	r2, [r6, r1]
30008f28:	3101      	addmi	r1, #1
30008f2a:	e7d7      	b.n	30008edc <NAND_RxData+0x9c>
30008f2c:	0000b9c1 	.word	0x0000b9c1
30008f30:	2001c01c 	.word	0x2001c01c

30008f34 <NAND_Page_Read>:
30008f34:	b530      	push	{r4, r5, lr}
30008f36:	b085      	sub	sp, #20
30008f38:	4c07      	ldr	r4, [pc, #28]	; (30008f58 <NAND_Page_Read+0x24>)
30008f3a:	4d08      	ldr	r5, [pc, #32]	; (30008f5c <NAND_Page_Read+0x28>)
30008f3c:	9301      	str	r3, [sp, #4]
30008f3e:	e9cd 2102 	strd	r2, r1, [sp, #8]
30008f42:	47a0      	blx	r4
30008f44:	4604      	mov	r4, r0
30008f46:	4806      	ldr	r0, [pc, #24]	; (30008f60 <NAND_Page_Read+0x2c>)
30008f48:	9903      	ldr	r1, [sp, #12]
30008f4a:	7a00      	ldrb	r0, [r0, #8]
30008f4c:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30008f50:	47a8      	blx	r5
30008f52:	4620      	mov	r0, r4
30008f54:	b005      	add	sp, #20
30008f56:	bd30      	pop	{r4, r5, pc}
30008f58:	0000bd85 	.word	0x0000bd85
30008f5c:	30008e41 	.word	0x30008e41
30008f60:	2001c01c 	.word	0x2001c01c

30008f64 <irq_set_priority>:
30008f64:	2907      	cmp	r1, #7
30008f66:	b530      	push	{r4, r5, lr}
30008f68:	4605      	mov	r5, r0
30008f6a:	b085      	sub	sp, #20
30008f6c:	460c      	mov	r4, r1
30008f6e:	d90e      	bls.n	30008f8e <irq_set_priority+0x2a>
30008f70:	2307      	movs	r3, #7
30008f72:	9000      	str	r0, [sp, #0]
30008f74:	4a10      	ldr	r2, [pc, #64]	; (30008fb8 <irq_set_priority+0x54>)
30008f76:	2003      	movs	r0, #3
30008f78:	e9cd 1301 	strd	r1, r3, [sp, #4]
30008f7c:	4b0f      	ldr	r3, [pc, #60]	; (30008fbc <irq_set_priority+0x58>)
30008f7e:	4619      	mov	r1, r3
30008f80:	f001 ffcc 	bl	3000af1c <rtk_log_write>
30008f84:	f44f 7168 	mov.w	r1, #928	; 0x3a0
30008f88:	480d      	ldr	r0, [pc, #52]	; (30008fc0 <irq_set_priority+0x5c>)
30008f8a:	f002 fa59 	bl	3000b440 <__io_assert_failed_veneer>
30008f8e:	2c07      	cmp	r4, #7
30008f90:	bf28      	it	cs
30008f92:	2407      	movcs	r4, #7
30008f94:	2d00      	cmp	r5, #0
30008f96:	ea4f 1444 	mov.w	r4, r4, lsl #5
30008f9a:	bfae      	itee	ge
30008f9c:	f105 4560 	addge.w	r5, r5, #3758096384	; 0xe0000000
30008fa0:	f005 050f 	andlt.w	r5, r5, #15
30008fa4:	4b07      	ldrlt	r3, [pc, #28]	; (30008fc4 <irq_set_priority+0x60>)
30008fa6:	b2e4      	uxtb	r4, r4
30008fa8:	bfaa      	itet	ge
30008faa:	f505 4561 	addge.w	r5, r5, #57600	; 0xe100
30008fae:	555c      	strblt	r4, [r3, r5]
30008fb0:	f885 4300 	strbge.w	r4, [r5, #768]	; 0x300
30008fb4:	b005      	add	sp, #20
30008fb6:	bd30      	pop	{r4, r5, pc}
30008fb8:	3000bbb2 	.word	0x3000bbb2
30008fbc:	3000bb82 	.word	0x3000bb82
30008fc0:	3000bbda 	.word	0x3000bbda
30008fc4:	e000ed14 	.word	0xe000ed14

30008fc8 <irq_register>:
30008fc8:	b510      	push	{r4, lr}
30008fca:	f040 0001 	orr.w	r0, r0, #1
30008fce:	460c      	mov	r4, r1
30008fd0:	4905      	ldr	r1, [pc, #20]	; (30008fe8 <irq_register+0x20>)
30008fd2:	f841 0024 	str.w	r0, [r1, r4, lsl #2]
30008fd6:	4620      	mov	r0, r4
30008fd8:	4904      	ldr	r1, [pc, #16]	; (30008fec <irq_register+0x24>)
30008fda:	f841 2024 	str.w	r2, [r1, r4, lsl #2]
30008fde:	4619      	mov	r1, r3
30008fe0:	4b03      	ldr	r3, [pc, #12]	; (30008ff0 <irq_register+0x28>)
30008fe2:	4798      	blx	r3
30008fe4:	2001      	movs	r0, #1
30008fe6:	bd10      	pop	{r4, pc}
30008fe8:	30000184 	.word	0x30000184
30008fec:	300002c4 	.word	0x300002c4
30008ff0:	30008f65 	.word	0x30008f65

30008ff4 <SYSCFG_BootFromNor>:
30008ff4:	4b0e      	ldr	r3, [pc, #56]	; (30009030 <SYSCFG_BootFromNor+0x3c>)
30008ff6:	b510      	push	{r4, lr}
30008ff8:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30008ffc:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009000:	0392      	lsls	r2, r2, #14
30009002:	d50a      	bpl.n	3000901a <SYSCFG_BootFromNor+0x26>
30009004:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009008:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
3000900c:	d105      	bne.n	3000901a <SYSCFG_BootFromNor+0x26>
3000900e:	4b09      	ldr	r3, [pc, #36]	; (30009034 <SYSCFG_BootFromNor+0x40>)
30009010:	4798      	blx	r3
30009012:	3800      	subs	r0, #0
30009014:	bf18      	it	ne
30009016:	2001      	movne	r0, #1
30009018:	bd10      	pop	{r4, pc}
3000901a:	4c07      	ldr	r4, [pc, #28]	; (30009038 <SYSCFG_BootFromNor+0x44>)
3000901c:	47a0      	blx	r4
3000901e:	2802      	cmp	r0, #2
30009020:	d004      	beq.n	3000902c <SYSCFG_BootFromNor+0x38>
30009022:	47a0      	blx	r4
30009024:	2801      	cmp	r0, #1
30009026:	d1f2      	bne.n	3000900e <SYSCFG_BootFromNor+0x1a>
30009028:	2000      	movs	r0, #0
3000902a:	e7f5      	b.n	30009018 <SYSCFG_BootFromNor+0x24>
3000902c:	2001      	movs	r0, #1
3000902e:	e7f3      	b.n	30009018 <SYSCFG_BootFromNor+0x24>
30009030:	42008000 	.word	0x42008000
30009034:	0000c0bd 	.word	0x0000c0bd
30009038:	0000c1a9 	.word	0x0000c1a9

3000903c <SYSCFG_RLVersion>:
3000903c:	4b07      	ldr	r3, [pc, #28]	; (3000905c <SYSCFG_RLVersion+0x20>)
3000903e:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009042:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009046:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
3000904a:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
3000904e:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009052:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009056:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000905a:	4770      	bx	lr
3000905c:	42008000 	.word	0x42008000

30009060 <SBOOT_GetMdType>:
30009060:	b513      	push	{r0, r1, r4, lr}
30009062:	4b0b      	ldr	r3, [pc, #44]	; (30009090 <SBOOT_GetMdType+0x30>)
30009064:	4604      	mov	r4, r0
30009066:	e9cd 2100 	strd	r2, r1, [sp]
3000906a:	4798      	blx	r3
3000906c:	4b09      	ldr	r3, [pc, #36]	; (30009094 <SBOOT_GetMdType+0x34>)
3000906e:	e9dd 2100 	ldrd	r2, r1, [sp]
30009072:	6018      	str	r0, [r3, #0]
30009074:	b928      	cbnz	r0, 30009082 <SBOOT_GetMdType+0x22>
30009076:	4620      	mov	r0, r4
30009078:	b002      	add	sp, #8
3000907a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000907e:	f002 b9cf 	b.w	3000b420 <__SBOOT_GetMdType_A_veneer>
30009082:	4620      	mov	r0, r4
30009084:	b002      	add	sp, #8
30009086:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000908a:	f002 b989 	b.w	3000b3a0 <__SBOOT_GetMdType_B_veneer>
3000908e:	bf00      	nop
30009090:	3000903d 	.word	0x3000903d
30009094:	3000eb04 	.word	0x3000eb04

30009098 <CRYPTO_OTPKey_SHA_Init>:
30009098:	b513      	push	{r0, r1, r4, lr}
3000909a:	4b0a      	ldr	r3, [pc, #40]	; (300090c4 <CRYPTO_OTPKey_SHA_Init+0x2c>)
3000909c:	4604      	mov	r4, r0
3000909e:	9101      	str	r1, [sp, #4]
300090a0:	4798      	blx	r3
300090a2:	4b09      	ldr	r3, [pc, #36]	; (300090c8 <CRYPTO_OTPKey_SHA_Init+0x30>)
300090a4:	9901      	ldr	r1, [sp, #4]
300090a6:	6018      	str	r0, [r3, #0]
300090a8:	b928      	cbnz	r0, 300090b6 <CRYPTO_OTPKey_SHA_Init+0x1e>
300090aa:	4620      	mov	r0, r4
300090ac:	b002      	add	sp, #8
300090ae:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090b2:	f002 b959 	b.w	3000b368 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
300090b6:	4620      	mov	r0, r4
300090b8:	b002      	add	sp, #8
300090ba:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090be:	f002 b973 	b.w	3000b3a8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
300090c2:	bf00      	nop
300090c4:	3000903d 	.word	0x3000903d
300090c8:	3000eb04 	.word	0x3000eb04

300090cc <rtl_crypto_hmac_sha2_update>:
300090cc:	b513      	push	{r0, r1, r4, lr}
300090ce:	4b0b      	ldr	r3, [pc, #44]	; (300090fc <rtl_crypto_hmac_sha2_update+0x30>)
300090d0:	4604      	mov	r4, r0
300090d2:	e9cd 2100 	strd	r2, r1, [sp]
300090d6:	4798      	blx	r3
300090d8:	4b09      	ldr	r3, [pc, #36]	; (30009100 <rtl_crypto_hmac_sha2_update+0x34>)
300090da:	e9dd 2100 	ldrd	r2, r1, [sp]
300090de:	6018      	str	r0, [r3, #0]
300090e0:	b928      	cbnz	r0, 300090ee <rtl_crypto_hmac_sha2_update+0x22>
300090e2:	4620      	mov	r0, r4
300090e4:	b002      	add	sp, #8
300090e6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090ea:	f002 b985 	b.w	3000b3f8 <__rtl_crypto_hmac_sha2_update_A_veneer>
300090ee:	4620      	mov	r0, r4
300090f0:	b002      	add	sp, #8
300090f2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090f6:	f002 b96f 	b.w	3000b3d8 <__rtl_crypto_hmac_sha2_update_B_veneer>
300090fa:	bf00      	nop
300090fc:	3000903d 	.word	0x3000903d
30009100:	3000eb04 	.word	0x3000eb04

30009104 <rtl_crypto_hmac_sha2_final>:
30009104:	b513      	push	{r0, r1, r4, lr}
30009106:	4b0a      	ldr	r3, [pc, #40]	; (30009130 <rtl_crypto_hmac_sha2_final+0x2c>)
30009108:	4604      	mov	r4, r0
3000910a:	9101      	str	r1, [sp, #4]
3000910c:	4798      	blx	r3
3000910e:	4b09      	ldr	r3, [pc, #36]	; (30009134 <rtl_crypto_hmac_sha2_final+0x30>)
30009110:	9901      	ldr	r1, [sp, #4]
30009112:	6018      	str	r0, [r3, #0]
30009114:	b928      	cbnz	r0, 30009122 <rtl_crypto_hmac_sha2_final+0x1e>
30009116:	4620      	mov	r0, r4
30009118:	b002      	add	sp, #8
3000911a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000911e:	f002 b987 	b.w	3000b430 <__rtl_crypto_hmac_sha2_final_A_veneer>
30009122:	4620      	mov	r0, r4
30009124:	b002      	add	sp, #8
30009126:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000912a:	f002 b959 	b.w	3000b3e0 <__rtl_crypto_hmac_sha2_final_B_veneer>
3000912e:	bf00      	nop
30009130:	3000903d 	.word	0x3000903d
30009134:	3000eb04 	.word	0x3000eb04

30009138 <rtl_crypto_sha2_init>:
30009138:	b513      	push	{r0, r1, r4, lr}
3000913a:	4b0a      	ldr	r3, [pc, #40]	; (30009164 <rtl_crypto_sha2_init+0x2c>)
3000913c:	4604      	mov	r4, r0
3000913e:	9101      	str	r1, [sp, #4]
30009140:	4798      	blx	r3
30009142:	4b09      	ldr	r3, [pc, #36]	; (30009168 <rtl_crypto_sha2_init+0x30>)
30009144:	9901      	ldr	r1, [sp, #4]
30009146:	6018      	str	r0, [r3, #0]
30009148:	b928      	cbnz	r0, 30009156 <rtl_crypto_sha2_init+0x1e>
3000914a:	4620      	mov	r0, r4
3000914c:	b002      	add	sp, #8
3000914e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009152:	f002 b92d 	b.w	3000b3b0 <__rtl_crypto_sha2_init_A_veneer>
30009156:	4620      	mov	r0, r4
30009158:	b002      	add	sp, #8
3000915a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000915e:	f002 b92b 	b.w	3000b3b8 <__rtl_crypto_sha2_init_B_veneer>
30009162:	bf00      	nop
30009164:	3000903d 	.word	0x3000903d
30009168:	3000eb04 	.word	0x3000eb04

3000916c <rtl_crypto_sha2_update>:
3000916c:	b513      	push	{r0, r1, r4, lr}
3000916e:	4b0b      	ldr	r3, [pc, #44]	; (3000919c <rtl_crypto_sha2_update+0x30>)
30009170:	4604      	mov	r4, r0
30009172:	e9cd 2100 	strd	r2, r1, [sp]
30009176:	4798      	blx	r3
30009178:	4b09      	ldr	r3, [pc, #36]	; (300091a0 <rtl_crypto_sha2_update+0x34>)
3000917a:	e9dd 2100 	ldrd	r2, r1, [sp]
3000917e:	6018      	str	r0, [r3, #0]
30009180:	b928      	cbnz	r0, 3000918e <rtl_crypto_sha2_update+0x22>
30009182:	4620      	mov	r0, r4
30009184:	b002      	add	sp, #8
30009186:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000918a:	f002 b931 	b.w	3000b3f0 <__rtl_crypto_sha2_update_A_veneer>
3000918e:	4620      	mov	r0, r4
30009190:	b002      	add	sp, #8
30009192:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009196:	f002 b93f 	b.w	3000b418 <__rtl_crypto_sha2_update_B_veneer>
3000919a:	bf00      	nop
3000919c:	3000903d 	.word	0x3000903d
300091a0:	3000eb04 	.word	0x3000eb04

300091a4 <rtl_crypto_sha2_final>:
300091a4:	b513      	push	{r0, r1, r4, lr}
300091a6:	4b0a      	ldr	r3, [pc, #40]	; (300091d0 <rtl_crypto_sha2_final+0x2c>)
300091a8:	4604      	mov	r4, r0
300091aa:	9101      	str	r1, [sp, #4]
300091ac:	4798      	blx	r3
300091ae:	4b09      	ldr	r3, [pc, #36]	; (300091d4 <rtl_crypto_sha2_final+0x30>)
300091b0:	9901      	ldr	r1, [sp, #4]
300091b2:	6018      	str	r0, [r3, #0]
300091b4:	b928      	cbnz	r0, 300091c2 <rtl_crypto_sha2_final+0x1e>
300091b6:	4620      	mov	r0, r4
300091b8:	b002      	add	sp, #8
300091ba:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300091be:	f002 b8d7 	b.w	3000b370 <__rtl_crypto_sha2_final_A_veneer>
300091c2:	4620      	mov	r0, r4
300091c4:	b002      	add	sp, #8
300091c6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300091ca:	f002 b919 	b.w	3000b400 <__rtl_crypto_sha2_final_B_veneer>
300091ce:	bf00      	nop
300091d0:	3000903d 	.word	0x3000903d
300091d4:	3000eb04 	.word	0x3000eb04

300091d8 <SBOOT_Validate_PubKey>:
300091d8:	b513      	push	{r0, r1, r4, lr}
300091da:	4b0b      	ldr	r3, [pc, #44]	; (30009208 <SBOOT_Validate_PubKey+0x30>)
300091dc:	4604      	mov	r4, r0
300091de:	e9cd 2100 	strd	r2, r1, [sp]
300091e2:	4798      	blx	r3
300091e4:	4b09      	ldr	r3, [pc, #36]	; (3000920c <SBOOT_Validate_PubKey+0x34>)
300091e6:	e9dd 2100 	ldrd	r2, r1, [sp]
300091ea:	6018      	str	r0, [r3, #0]
300091ec:	b928      	cbnz	r0, 300091fa <SBOOT_Validate_PubKey+0x22>
300091ee:	4620      	mov	r0, r4
300091f0:	b002      	add	sp, #8
300091f2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300091f6:	f002 b90b 	b.w	3000b410 <__SBOOT_Validate_PubKey_A_veneer>
300091fa:	4620      	mov	r0, r4
300091fc:	b002      	add	sp, #8
300091fe:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009202:	f002 b8f1 	b.w	3000b3e8 <__SBOOT_Validate_PubKey_B_veneer>
30009206:	bf00      	nop
30009208:	3000903d 	.word	0x3000903d
3000920c:	3000eb04 	.word	0x3000eb04

30009210 <SBOOT_Validate_Signature>:
30009210:	b5f0      	push	{r4, r5, r6, r7, lr}
30009212:	b085      	sub	sp, #20
30009214:	4f0d      	ldr	r7, [pc, #52]	; (3000924c <SBOOT_Validate_Signature+0x3c>)
30009216:	4604      	mov	r4, r0
30009218:	480d      	ldr	r0, [pc, #52]	; (30009250 <SBOOT_Validate_Signature+0x40>)
3000921a:	9301      	str	r3, [sp, #4]
3000921c:	e9dd 560a 	ldrd	r5, r6, [sp, #40]	; 0x28
30009220:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009224:	4780      	blx	r0
30009226:	9903      	ldr	r1, [sp, #12]
30009228:	6038      	str	r0, [r7, #0]
3000922a:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000922e:	e9cd 560a 	strd	r5, r6, [sp, #40]	; 0x28
30009232:	b928      	cbnz	r0, 30009240 <SBOOT_Validate_Signature+0x30>
30009234:	4620      	mov	r0, r4
30009236:	b005      	add	sp, #20
30009238:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
3000923c:	f002 b8c8 	b.w	3000b3d0 <__SBOOT_Validate_Signature_A_veneer>
30009240:	4620      	mov	r0, r4
30009242:	b005      	add	sp, #20
30009244:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009248:	f002 b886 	b.w	3000b358 <__SBOOT_Validate_Signature_B_veneer>
3000924c:	3000eb04 	.word	0x3000eb04
30009250:	3000903d 	.word	0x3000903d

30009254 <SBOOT_Validate_ImgHash>:
30009254:	b530      	push	{r4, r5, lr}
30009256:	4d0c      	ldr	r5, [pc, #48]	; (30009288 <SBOOT_Validate_ImgHash+0x34>)
30009258:	b085      	sub	sp, #20
3000925a:	4604      	mov	r4, r0
3000925c:	480b      	ldr	r0, [pc, #44]	; (3000928c <SBOOT_Validate_ImgHash+0x38>)
3000925e:	9301      	str	r3, [sp, #4]
30009260:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009264:	4780      	blx	r0
30009266:	9903      	ldr	r1, [sp, #12]
30009268:	6028      	str	r0, [r5, #0]
3000926a:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000926e:	b928      	cbnz	r0, 3000927c <SBOOT_Validate_ImgHash+0x28>
30009270:	4620      	mov	r0, r4
30009272:	b005      	add	sp, #20
30009274:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30009278:	f002 b86a 	b.w	3000b350 <__SBOOT_Validate_ImgHash_A_veneer>
3000927c:	4620      	mov	r0, r4
3000927e:	b005      	add	sp, #20
30009280:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30009284:	f002 b8c0 	b.w	3000b408 <__SBOOT_Validate_ImgHash_B_veneer>
30009288:	3000eb04 	.word	0x3000eb04
3000928c:	3000903d 	.word	0x3000903d

30009290 <CRYPTO_SHA_Init>:
30009290:	b508      	push	{r3, lr}
30009292:	4b07      	ldr	r3, [pc, #28]	; (300092b0 <CRYPTO_SHA_Init+0x20>)
30009294:	4798      	blx	r3
30009296:	4a07      	ldr	r2, [pc, #28]	; (300092b4 <CRYPTO_SHA_Init+0x24>)
30009298:	6010      	str	r0, [r2, #0]
3000929a:	b918      	cbnz	r0, 300092a4 <CRYPTO_SHA_Init+0x14>
3000929c:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
300092a0:	f002 b86a 	b.w	3000b378 <__CRYPTO_SHA_Init_A_veneer>
300092a4:	2000      	movs	r0, #0
300092a6:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
300092aa:	f002 b869 	b.w	3000b380 <__CRYPTO_SHA_Init_B_veneer>
300092ae:	bf00      	nop
300092b0:	3000903d 	.word	0x3000903d
300092b4:	3000eb04 	.word	0x3000eb04

300092b8 <SBOOT_Validate_Algorithm>:
300092b8:	b530      	push	{r4, r5, lr}
300092ba:	4d0c      	ldr	r5, [pc, #48]	; (300092ec <SBOOT_Validate_Algorithm+0x34>)
300092bc:	b085      	sub	sp, #20
300092be:	4604      	mov	r4, r0
300092c0:	480b      	ldr	r0, [pc, #44]	; (300092f0 <SBOOT_Validate_Algorithm+0x38>)
300092c2:	9301      	str	r3, [sp, #4]
300092c4:	e9cd 2102 	strd	r2, r1, [sp, #8]
300092c8:	4780      	blx	r0
300092ca:	9903      	ldr	r1, [sp, #12]
300092cc:	6028      	str	r0, [r5, #0]
300092ce:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300092d2:	b928      	cbnz	r0, 300092e0 <SBOOT_Validate_Algorithm+0x28>
300092d4:	4620      	mov	r0, r4
300092d6:	b005      	add	sp, #20
300092d8:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
300092dc:	f002 b8a4 	b.w	3000b428 <__SBOOT_Validate_Algorithm_A_veneer>
300092e0:	4620      	mov	r0, r4
300092e2:	b005      	add	sp, #20
300092e4:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
300092e8:	f002 b82e 	b.w	3000b348 <__SBOOT_Validate_Algorithm_B_veneer>
300092ec:	3000eb04 	.word	0x3000eb04
300092f0:	3000903d 	.word	0x3000903d

300092f4 <ddr_init_index>:
300092f4:	b570      	push	{r4, r5, r6, lr}
300092f6:	4b0f      	ldr	r3, [pc, #60]	; (30009334 <ddr_init_index+0x40>)
300092f8:	4798      	blx	r3
300092fa:	4604      	mov	r4, r0
300092fc:	4b0e      	ldr	r3, [pc, #56]	; (30009338 <ddr_init_index+0x44>)
300092fe:	4798      	blx	r3
30009300:	2c03      	cmp	r4, #3
30009302:	d012      	beq.n	3000932a <ddr_init_index+0x36>
30009304:	2c01      	cmp	r4, #1
30009306:	d103      	bne.n	30009310 <ddr_init_index+0x1c>
30009308:	2802      	cmp	r0, #2
3000930a:	d00e      	beq.n	3000932a <ddr_init_index+0x36>
3000930c:	2803      	cmp	r0, #3
3000930e:	d00e      	beq.n	3000932e <ddr_init_index+0x3a>
30009310:	4e0a      	ldr	r6, [pc, #40]	; (3000933c <ddr_init_index+0x48>)
30009312:	4d0b      	ldr	r5, [pc, #44]	; (30009340 <ddr_init_index+0x4c>)
30009314:	4c0b      	ldr	r4, [pc, #44]	; (30009344 <ddr_init_index+0x50>)
30009316:	4633      	mov	r3, r6
30009318:	462a      	mov	r2, r5
3000931a:	4908      	ldr	r1, [pc, #32]	; (3000933c <ddr_init_index+0x48>)
3000931c:	2002      	movs	r0, #2
3000931e:	f001 fdfd 	bl	3000af1c <rtk_log_write>
30009322:	f242 7010 	movw	r0, #10000	; 0x2710
30009326:	47a0      	blx	r4
30009328:	e7f5      	b.n	30009316 <ddr_init_index+0x22>
3000932a:	4620      	mov	r0, r4
3000932c:	bd70      	pop	{r4, r5, r6, pc}
3000932e:	2000      	movs	r0, #0
30009330:	e7fc      	b.n	3000932c <ddr_init_index+0x38>
30009332:	bf00      	nop
30009334:	30007479 	.word	0x30007479
30009338:	30007329 	.word	0x30007329
3000933c:	3000bc2e 	.word	0x3000bc2e
30009340:	3000bc32 	.word	0x3000bc32
30009344:	00009be5 	.word	0x00009be5

30009348 <rxi316_perf_tune>:
30009348:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000934c:	b0c1      	sub	sp, #260	; 0x104
3000934e:	9000      	str	r0, [sp, #0]
30009350:	6800      	ldr	r0, [r0, #0]
30009352:	7985      	ldrb	r5, [r0, #6]
30009354:	2d00      	cmp	r5, #0
30009356:	f000 82f7 	beq.w	30009948 <rxi316_perf_tune+0x600>
3000935a:	2d01      	cmp	r5, #1
3000935c:	d003      	beq.n	30009366 <rxi316_perf_tune+0x1e>
3000935e:	2d03      	cmp	r5, #3
30009360:	bf14      	ite	ne
30009362:	2502      	movne	r5, #2
30009364:	2500      	moveq	r5, #0
30009366:	9b00      	ldr	r3, [sp, #0]
30009368:	2420      	movs	r4, #32
3000936a:	68db      	ldr	r3, [r3, #12]
3000936c:	4626      	mov	r6, r4
3000936e:	46a1      	mov	r9, r4
30009370:	46a3      	mov	fp, r4
30009372:	685b      	ldr	r3, [r3, #4]
30009374:	46a2      	mov	sl, r4
30009376:	46a0      	mov	r8, r4
30009378:	781a      	ldrb	r2, [r3, #0]
3000937a:	9220      	str	r2, [sp, #128]	; 0x80
3000937c:	785a      	ldrb	r2, [r3, #1]
3000937e:	9221      	str	r2, [sp, #132]	; 0x84
30009380:	789a      	ldrb	r2, [r3, #2]
30009382:	9222      	str	r2, [sp, #136]	; 0x88
30009384:	78da      	ldrb	r2, [r3, #3]
30009386:	9223      	str	r2, [sp, #140]	; 0x8c
30009388:	791a      	ldrb	r2, [r3, #4]
3000938a:	9224      	str	r2, [sp, #144]	; 0x90
3000938c:	795a      	ldrb	r2, [r3, #5]
3000938e:	9225      	str	r2, [sp, #148]	; 0x94
30009390:	799a      	ldrb	r2, [r3, #6]
30009392:	9226      	str	r2, [sp, #152]	; 0x98
30009394:	79da      	ldrb	r2, [r3, #7]
30009396:	9227      	str	r2, [sp, #156]	; 0x9c
30009398:	7a1a      	ldrb	r2, [r3, #8]
3000939a:	9228      	str	r2, [sp, #160]	; 0xa0
3000939c:	7a5a      	ldrb	r2, [r3, #9]
3000939e:	9229      	str	r2, [sp, #164]	; 0xa4
300093a0:	7a9a      	ldrb	r2, [r3, #10]
300093a2:	922a      	str	r2, [sp, #168]	; 0xa8
300093a4:	7ada      	ldrb	r2, [r3, #11]
300093a6:	922b      	str	r2, [sp, #172]	; 0xac
300093a8:	7b1a      	ldrb	r2, [r3, #12]
300093aa:	922c      	str	r2, [sp, #176]	; 0xb0
300093ac:	7b5a      	ldrb	r2, [r3, #13]
300093ae:	922d      	str	r2, [sp, #180]	; 0xb4
300093b0:	7b9a      	ldrb	r2, [r3, #14]
300093b2:	922e      	str	r2, [sp, #184]	; 0xb8
300093b4:	7bda      	ldrb	r2, [r3, #15]
300093b6:	922f      	str	r2, [sp, #188]	; 0xbc
300093b8:	7c1a      	ldrb	r2, [r3, #16]
300093ba:	9230      	str	r2, [sp, #192]	; 0xc0
300093bc:	7c5a      	ldrb	r2, [r3, #17]
300093be:	9231      	str	r2, [sp, #196]	; 0xc4
300093c0:	7c9a      	ldrb	r2, [r3, #18]
300093c2:	9232      	str	r2, [sp, #200]	; 0xc8
300093c4:	7cda      	ldrb	r2, [r3, #19]
300093c6:	9233      	str	r2, [sp, #204]	; 0xcc
300093c8:	7d1a      	ldrb	r2, [r3, #20]
300093ca:	9234      	str	r2, [sp, #208]	; 0xd0
300093cc:	7d5a      	ldrb	r2, [r3, #21]
300093ce:	9235      	str	r2, [sp, #212]	; 0xd4
300093d0:	7d9a      	ldrb	r2, [r3, #22]
300093d2:	9236      	str	r2, [sp, #216]	; 0xd8
300093d4:	7dda      	ldrb	r2, [r3, #23]
300093d6:	9237      	str	r2, [sp, #220]	; 0xdc
300093d8:	7e1a      	ldrb	r2, [r3, #24]
300093da:	9238      	str	r2, [sp, #224]	; 0xe0
300093dc:	7e5a      	ldrb	r2, [r3, #25]
300093de:	9239      	str	r2, [sp, #228]	; 0xe4
300093e0:	7e9a      	ldrb	r2, [r3, #26]
300093e2:	923a      	str	r2, [sp, #232]	; 0xe8
300093e4:	7eda      	ldrb	r2, [r3, #27]
300093e6:	923b      	str	r2, [sp, #236]	; 0xec
300093e8:	7f1a      	ldrb	r2, [r3, #28]
300093ea:	923c      	str	r2, [sp, #240]	; 0xf0
300093ec:	7f5a      	ldrb	r2, [r3, #29]
300093ee:	923d      	str	r2, [sp, #244]	; 0xf4
300093f0:	7f9a      	ldrb	r2, [r3, #30]
300093f2:	923e      	str	r2, [sp, #248]	; 0xf8
300093f4:	7fdb      	ldrb	r3, [r3, #31]
300093f6:	941d      	str	r4, [sp, #116]	; 0x74
300093f8:	933f      	str	r3, [sp, #252]	; 0xfc
300093fa:	2300      	movs	r3, #0
300093fc:	9408      	str	r4, [sp, #32]
300093fe:	4619      	mov	r1, r3
30009400:	461f      	mov	r7, r3
30009402:	9405      	str	r4, [sp, #20]
30009404:	9401      	str	r4, [sp, #4]
30009406:	9403      	str	r4, [sp, #12]
30009408:	9402      	str	r4, [sp, #8]
3000940a:	e9cd 441b 	strd	r4, r4, [sp, #108]	; 0x6c
3000940e:	e9cd 4419 	strd	r4, r4, [sp, #100]	; 0x64
30009412:	e9cd 4417 	strd	r4, r4, [sp, #92]	; 0x5c
30009416:	e9cd 4415 	strd	r4, r4, [sp, #84]	; 0x54
3000941a:	e9cd 4413 	strd	r4, r4, [sp, #76]	; 0x4c
3000941e:	e9cd 4411 	strd	r4, r4, [sp, #68]	; 0x44
30009422:	e9cd 440f 	strd	r4, r4, [sp, #60]	; 0x3c
30009426:	e9cd 440d 	strd	r4, r4, [sp, #52]	; 0x34
3000942a:	e9cd 440b 	strd	r4, r4, [sp, #44]	; 0x2c
3000942e:	e9cd 4409 	strd	r4, r4, [sp, #36]	; 0x24
30009432:	e9cd 4406 	strd	r4, r4, [sp, #24]
30009436:	aa20      	add	r2, sp, #128	; 0x80
30009438:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000943c:	2a00      	cmp	r2, #0
3000943e:	f040 8285 	bne.w	3000994c <rxi316_perf_tune+0x604>
30009442:	3101      	adds	r1, #1
30009444:	9313      	str	r3, [sp, #76]	; 0x4c
30009446:	3301      	adds	r3, #1
30009448:	2b20      	cmp	r3, #32
3000944a:	d1f4      	bne.n	30009436 <rxi316_perf_tune+0xee>
3000944c:	7882      	ldrb	r2, [r0, #2]
3000944e:	6883      	ldr	r3, [r0, #8]
30009450:	2a00      	cmp	r2, #0
30009452:	f000 8311 	beq.w	30009a78 <rxi316_perf_tune+0x730>
30009456:	2a01      	cmp	r2, #1
30009458:	f000 8310 	beq.w	30009a7c <rxi316_perf_tune+0x734>
3000945c:	2a03      	cmp	r2, #3
3000945e:	bf0c      	ite	eq
30009460:	2204      	moveq	r2, #4
30009462:	2203      	movne	r2, #3
30009464:	3b02      	subs	r3, #2
30009466:	9204      	str	r2, [sp, #16]
30009468:	428b      	cmp	r3, r1
3000946a:	d005      	beq.n	30009478 <rxi316_perf_tune+0x130>
3000946c:	4b07      	ldr	r3, [pc, #28]	; (3000948c <rxi316_perf_tune+0x144>)
3000946e:	2002      	movs	r0, #2
30009470:	4a07      	ldr	r2, [pc, #28]	; (30009490 <rxi316_perf_tune+0x148>)
30009472:	4619      	mov	r1, r3
30009474:	f001 fd52 	bl	3000af1c <rtk_log_write>
30009478:	9b04      	ldr	r3, [sp, #16]
3000947a:	42bb      	cmp	r3, r7
3000947c:	d00c      	beq.n	30009498 <rxi316_perf_tune+0x150>
3000947e:	4b03      	ldr	r3, [pc, #12]	; (3000948c <rxi316_perf_tune+0x144>)
30009480:	2002      	movs	r0, #2
30009482:	4a04      	ldr	r2, [pc, #16]	; (30009494 <rxi316_perf_tune+0x14c>)
30009484:	4619      	mov	r1, r3
30009486:	f001 fd49 	bl	3000af1c <rtk_log_write>
3000948a:	e005      	b.n	30009498 <rxi316_perf_tune+0x150>
3000948c:	3000bc2e 	.word	0x3000bc2e
30009490:	3000bc55 	.word	0x3000bc55
30009494:	3000bc98 	.word	0x3000bc98
30009498:	f1ba 0f20 	cmp.w	sl, #32
3000949c:	bf16      	itet	ne
3000949e:	ebaa 0a05 	subne.w	sl, sl, r5
300094a2:	230f      	moveq	r3, #15
300094a4:	f1aa 0305 	subne.w	r3, sl, #5
300094a8:	9304      	str	r3, [sp, #16]
300094aa:	9b03      	ldr	r3, [sp, #12]
300094ac:	2b20      	cmp	r3, #32
300094ae:	bf16      	itet	ne
300094b0:	1b5b      	subne	r3, r3, r5
300094b2:	230f      	moveq	r3, #15
300094b4:	3b06      	subne	r3, #6
300094b6:	f1b8 0f20 	cmp.w	r8, #32
300094ba:	bf18      	it	ne
300094bc:	eba8 0805 	subne.w	r8, r8, r5
300094c0:	9303      	str	r3, [sp, #12]
300094c2:	bf0c      	ite	eq
300094c4:	230f      	moveq	r3, #15
300094c6:	f1a8 0307 	subne.w	r3, r8, #7
300094ca:	931e      	str	r3, [sp, #120]	; 0x78
300094cc:	9b02      	ldr	r3, [sp, #8]
300094ce:	2b20      	cmp	r3, #32
300094d0:	bf16      	itet	ne
300094d2:	1b5b      	subne	r3, r3, r5
300094d4:	230f      	moveq	r3, #15
300094d6:	3b08      	subne	r3, #8
300094d8:	9302      	str	r3, [sp, #8]
300094da:	9b01      	ldr	r3, [sp, #4]
300094dc:	2b20      	cmp	r3, #32
300094de:	bf16      	itet	ne
300094e0:	1b5b      	subne	r3, r3, r5
300094e2:	230f      	moveq	r3, #15
300094e4:	3b09      	subne	r3, #9
300094e6:	9301      	str	r3, [sp, #4]
300094e8:	9b05      	ldr	r3, [sp, #20]
300094ea:	2b20      	cmp	r3, #32
300094ec:	bf16      	itet	ne
300094ee:	1b5b      	subne	r3, r3, r5
300094f0:	230f      	moveq	r3, #15
300094f2:	3b0a      	subne	r3, #10
300094f4:	9305      	str	r3, [sp, #20]
300094f6:	9b06      	ldr	r3, [sp, #24]
300094f8:	2b20      	cmp	r3, #32
300094fa:	bf16      	itet	ne
300094fc:	1b5b      	subne	r3, r3, r5
300094fe:	230f      	moveq	r3, #15
30009500:	3b0b      	subne	r3, #11
30009502:	9306      	str	r3, [sp, #24]
30009504:	9b07      	ldr	r3, [sp, #28]
30009506:	2b20      	cmp	r3, #32
30009508:	bf16      	itet	ne
3000950a:	1b5b      	subne	r3, r3, r5
3000950c:	230f      	moveq	r3, #15
3000950e:	3b0c      	subne	r3, #12
30009510:	f1bb 0f20 	cmp.w	fp, #32
30009514:	bf18      	it	ne
30009516:	ebab 0b05 	subne.w	fp, fp, r5
3000951a:	9307      	str	r3, [sp, #28]
3000951c:	bf0c      	ite	eq
3000951e:	230f      	moveq	r3, #15
30009520:	f1ab 030d 	subne.w	r3, fp, #13
30009524:	931f      	str	r3, [sp, #124]	; 0x7c
30009526:	9b08      	ldr	r3, [sp, #32]
30009528:	2b20      	cmp	r3, #32
3000952a:	bf16      	itet	ne
3000952c:	1b5b      	subne	r3, r3, r5
3000952e:	230f      	moveq	r3, #15
30009530:	3b0e      	subne	r3, #14
30009532:	9308      	str	r3, [sp, #32]
30009534:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009536:	2b20      	cmp	r3, #32
30009538:	bf16      	itet	ne
3000953a:	eba3 0b05 	subne.w	fp, r3, r5
3000953e:	230f      	moveq	r3, #15
30009540:	f1ab 030f 	subne.w	r3, fp, #15
30009544:	9309      	str	r3, [sp, #36]	; 0x24
30009546:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009548:	2b20      	cmp	r3, #32
3000954a:	bf16      	itet	ne
3000954c:	eba3 0b05 	subne.w	fp, r3, r5
30009550:	230f      	moveq	r3, #15
30009552:	f1ab 0310 	subne.w	r3, fp, #16
30009556:	930a      	str	r3, [sp, #40]	; 0x28
30009558:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000955a:	2b20      	cmp	r3, #32
3000955c:	bf16      	itet	ne
3000955e:	eba3 0b05 	subne.w	fp, r3, r5
30009562:	230f      	moveq	r3, #15
30009564:	f1ab 0311 	subne.w	r3, fp, #17
30009568:	930b      	str	r3, [sp, #44]	; 0x2c
3000956a:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000956c:	2b20      	cmp	r3, #32
3000956e:	bf16      	itet	ne
30009570:	eba3 0b05 	subne.w	fp, r3, r5
30009574:	230f      	moveq	r3, #15
30009576:	f1ab 0312 	subne.w	r3, fp, #18
3000957a:	930c      	str	r3, [sp, #48]	; 0x30
3000957c:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000957e:	2b20      	cmp	r3, #32
30009580:	bf16      	itet	ne
30009582:	eba3 0b05 	subne.w	fp, r3, r5
30009586:	230f      	moveq	r3, #15
30009588:	f1ab 0313 	subne.w	r3, fp, #19
3000958c:	930d      	str	r3, [sp, #52]	; 0x34
3000958e:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30009590:	2b20      	cmp	r3, #32
30009592:	bf16      	itet	ne
30009594:	eba3 0b05 	subne.w	fp, r3, r5
30009598:	230f      	moveq	r3, #15
3000959a:	f1ab 0314 	subne.w	r3, fp, #20
3000959e:	930e      	str	r3, [sp, #56]	; 0x38
300095a0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300095a2:	2b20      	cmp	r3, #32
300095a4:	bf16      	itet	ne
300095a6:	1b5b      	subne	r3, r3, r5
300095a8:	230f      	moveq	r3, #15
300095aa:	3b15      	subne	r3, #21
300095ac:	930f      	str	r3, [sp, #60]	; 0x3c
300095ae:	9b10      	ldr	r3, [sp, #64]	; 0x40
300095b0:	2b20      	cmp	r3, #32
300095b2:	bf16      	itet	ne
300095b4:	eba3 0b05 	subne.w	fp, r3, r5
300095b8:	231f      	moveq	r3, #31
300095ba:	f1ab 0302 	subne.w	r3, fp, #2
300095be:	9310      	str	r3, [sp, #64]	; 0x40
300095c0:	9b11      	ldr	r3, [sp, #68]	; 0x44
300095c2:	2b20      	cmp	r3, #32
300095c4:	bf16      	itet	ne
300095c6:	1b5b      	subne	r3, r3, r5
300095c8:	231f      	moveq	r3, #31
300095ca:	3b02      	subne	r3, #2
300095cc:	9311      	str	r3, [sp, #68]	; 0x44
300095ce:	9b12      	ldr	r3, [sp, #72]	; 0x48
300095d0:	2b20      	cmp	r3, #32
300095d2:	bf16      	itet	ne
300095d4:	eba3 0b05 	subne.w	fp, r3, r5
300095d8:	231f      	moveq	r3, #31
300095da:	f1ab 0302 	subne.w	r3, fp, #2
300095de:	9312      	str	r3, [sp, #72]	; 0x48
300095e0:	9b13      	ldr	r3, [sp, #76]	; 0x4c
300095e2:	2b20      	cmp	r3, #32
300095e4:	bf16      	itet	ne
300095e6:	eba3 0b05 	subne.w	fp, r3, r5
300095ea:	230f      	moveq	r3, #15
300095ec:	f1ab 0302 	subne.w	r3, fp, #2
300095f0:	9313      	str	r3, [sp, #76]	; 0x4c
300095f2:	9b14      	ldr	r3, [sp, #80]	; 0x50
300095f4:	2b20      	cmp	r3, #32
300095f6:	bf16      	itet	ne
300095f8:	eba3 0b05 	subne.w	fp, r3, r5
300095fc:	230f      	moveq	r3, #15
300095fe:	f1ab 0303 	subne.w	r3, fp, #3
30009602:	9314      	str	r3, [sp, #80]	; 0x50
30009604:	9b15      	ldr	r3, [sp, #84]	; 0x54
30009606:	2b20      	cmp	r3, #32
30009608:	bf16      	itet	ne
3000960a:	1b5b      	subne	r3, r3, r5
3000960c:	230f      	moveq	r3, #15
3000960e:	3b04      	subne	r3, #4
30009610:	9315      	str	r3, [sp, #84]	; 0x54
30009612:	9b16      	ldr	r3, [sp, #88]	; 0x58
30009614:	2b20      	cmp	r3, #32
30009616:	bf16      	itet	ne
30009618:	1b5b      	subne	r3, r3, r5
3000961a:	230f      	moveq	r3, #15
3000961c:	3b05      	subne	r3, #5
3000961e:	9316      	str	r3, [sp, #88]	; 0x58
30009620:	9b17      	ldr	r3, [sp, #92]	; 0x5c
30009622:	2b20      	cmp	r3, #32
30009624:	bf18      	it	ne
30009626:	1b5f      	subne	r7, r3, r5
30009628:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000962a:	bf0c      	ite	eq
3000962c:	270f      	moveq	r7, #15
3000962e:	3f06      	subne	r7, #6
30009630:	2b20      	cmp	r3, #32
30009632:	bf18      	it	ne
30009634:	1b58      	subne	r0, r3, r5
30009636:	9b19      	ldr	r3, [sp, #100]	; 0x64
30009638:	bf08      	it	eq
3000963a:	200f      	moveq	r0, #15
3000963c:	ea4f 4707 	mov.w	r7, r7, lsl #16
30009640:	bf18      	it	ne
30009642:	3807      	subne	r0, #7
30009644:	2b20      	cmp	r3, #32
30009646:	f407 2770 	and.w	r7, r7, #983040	; 0xf0000
3000964a:	bf18      	it	ne
3000964c:	1b59      	subne	r1, r3, r5
3000964e:	9b1a      	ldr	r3, [sp, #104]	; 0x68
30009650:	bf08      	it	eq
30009652:	210f      	moveq	r1, #15
30009654:	ea4f 5000 	mov.w	r0, r0, lsl #20
30009658:	bf18      	it	ne
3000965a:	3908      	subne	r1, #8
3000965c:	2b20      	cmp	r3, #32
3000965e:	f400 0070 	and.w	r0, r0, #15728640	; 0xf00000
30009662:	bf14      	ite	ne
30009664:	eba3 0a05 	subne.w	sl, r3, r5
30009668:	f04f 0a0f 	moveq.w	sl, #15
3000966c:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000966e:	ea4f 6101 	mov.w	r1, r1, lsl #24
30009672:	bf18      	it	ne
30009674:	f1aa 0a09 	subne.w	sl, sl, #9
30009678:	f1b9 0f20 	cmp.w	r9, #32
3000967c:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
30009680:	bf16      	itet	ne
30009682:	eba9 0905 	subne.w	r9, r9, r5
30009686:	f04f 090f 	moveq.w	r9, #15
3000968a:	f1a9 090a 	subne.w	r9, r9, #10
3000968e:	2b20      	cmp	r3, #32
30009690:	bf18      	it	ne
30009692:	1b5a      	subne	r2, r3, r5
30009694:	9b1c      	ldr	r3, [sp, #112]	; 0x70
30009696:	bf08      	it	eq
30009698:	220f      	moveq	r2, #15
3000969a:	f009 090f 	and.w	r9, r9, #15
3000969e:	bf18      	it	ne
300096a0:	3a0b      	subne	r2, #11
300096a2:	2b20      	cmp	r3, #32
300096a4:	bf14      	ite	ne
300096a6:	eba3 0805 	subne.w	r8, r3, r5
300096aa:	f04f 081f 	moveq.w	r8, #31
300096ae:	9b1d      	ldr	r3, [sp, #116]	; 0x74
300096b0:	ea4f 1202 	mov.w	r2, r2, lsl #4
300096b4:	bf18      	it	ne
300096b6:	f1a8 0802 	subne.w	r8, r8, #2
300096ba:	2e20      	cmp	r6, #32
300096bc:	b2d2      	uxtb	r2, r2
300096be:	bf14      	ite	ne
300096c0:	eba6 0c05 	subne.w	ip, r6, r5
300096c4:	f04f 0c1f 	moveq.w	ip, #31
300096c8:	ea42 0209 	orr.w	r2, r2, r9
300096cc:	bf18      	it	ne
300096ce:	f1ac 0c02 	subne.w	ip, ip, #2
300096d2:	2b20      	cmp	r3, #32
300096d4:	bf14      	ite	ne
300096d6:	eba3 0e05 	subne.w	lr, r3, r5
300096da:	f04f 0e1f 	moveq.w	lr, #31
300096de:	9b03      	ldr	r3, [sp, #12]
300096e0:	bf18      	it	ne
300096e2:	f1ae 0e02 	subne.w	lr, lr, #2
300096e6:	2c20      	cmp	r4, #32
300096e8:	ea4f 1303 	mov.w	r3, r3, lsl #4
300096ec:	bf18      	it	ne
300096ee:	1b64      	subne	r4, r4, r5
300096f0:	9d04      	ldr	r5, [sp, #16]
300096f2:	b2db      	uxtb	r3, r3
300096f4:	bf08      	it	eq
300096f6:	241f      	moveq	r4, #31
300096f8:	f005 050f 	and.w	r5, r5, #15
300096fc:	bf18      	it	ne
300096fe:	3c02      	subne	r4, #2
30009700:	f00e 0e1f 	and.w	lr, lr, #31
30009704:	432b      	orrs	r3, r5
30009706:	9d1e      	ldr	r5, [sp, #120]	; 0x78
30009708:	0164      	lsls	r4, r4, #5
3000970a:	022d      	lsls	r5, r5, #8
3000970c:	f404 7478 	and.w	r4, r4, #992	; 0x3e0
30009710:	f405 6570 	and.w	r5, r5, #3840	; 0xf00
30009714:	ea44 040e 	orr.w	r4, r4, lr
30009718:	432b      	orrs	r3, r5
3000971a:	9d02      	ldr	r5, [sp, #8]
3000971c:	032d      	lsls	r5, r5, #12
3000971e:	b2ad      	uxth	r5, r5
30009720:	432b      	orrs	r3, r5
30009722:	9d01      	ldr	r5, [sp, #4]
30009724:	042d      	lsls	r5, r5, #16
30009726:	f405 2570 	and.w	r5, r5, #983040	; 0xf0000
3000972a:	432b      	orrs	r3, r5
3000972c:	9d05      	ldr	r5, [sp, #20]
3000972e:	052d      	lsls	r5, r5, #20
30009730:	f405 0570 	and.w	r5, r5, #15728640	; 0xf00000
30009734:	432b      	orrs	r3, r5
30009736:	9d06      	ldr	r5, [sp, #24]
30009738:	062d      	lsls	r5, r5, #24
3000973a:	f005 6570 	and.w	r5, r5, #251658240	; 0xf000000
3000973e:	431d      	orrs	r5, r3
30009740:	9b07      	ldr	r3, [sp, #28]
30009742:	ea45 7503 	orr.w	r5, r5, r3, lsl #28
30009746:	4ba7      	ldr	r3, [pc, #668]	; (300099e4 <rxi316_perf_tune+0x69c>)
30009748:	f8c3 5400 	str.w	r5, [r3, #1024]	; 0x400
3000974c:	9d08      	ldr	r5, [sp, #32]
3000974e:	9e1f      	ldr	r6, [sp, #124]	; 0x7c
30009750:	012d      	lsls	r5, r5, #4
30009752:	f006 0b0f 	and.w	fp, r6, #15
30009756:	9e09      	ldr	r6, [sp, #36]	; 0x24
30009758:	b2ed      	uxtb	r5, r5
3000975a:	ea45 050b 	orr.w	r5, r5, fp
3000975e:	ea4f 2b06 	mov.w	fp, r6, lsl #8
30009762:	9e0a      	ldr	r6, [sp, #40]	; 0x28
30009764:	f40b 6b70 	and.w	fp, fp, #3840	; 0xf00
30009768:	ea45 050b 	orr.w	r5, r5, fp
3000976c:	ea4f 3b06 	mov.w	fp, r6, lsl #12
30009770:	9e0b      	ldr	r6, [sp, #44]	; 0x2c
30009772:	fa1f fb8b 	uxth.w	fp, fp
30009776:	ea45 050b 	orr.w	r5, r5, fp
3000977a:	ea4f 4b06 	mov.w	fp, r6, lsl #16
3000977e:	9e0c      	ldr	r6, [sp, #48]	; 0x30
30009780:	f40b 2b70 	and.w	fp, fp, #983040	; 0xf0000
30009784:	ea45 050b 	orr.w	r5, r5, fp
30009788:	ea4f 5b06 	mov.w	fp, r6, lsl #20
3000978c:	9e0d      	ldr	r6, [sp, #52]	; 0x34
3000978e:	f40b 0b70 	and.w	fp, fp, #15728640	; 0xf00000
30009792:	ea45 050b 	orr.w	r5, r5, fp
30009796:	ea4f 6b06 	mov.w	fp, r6, lsl #24
3000979a:	9e0e      	ldr	r6, [sp, #56]	; 0x38
3000979c:	f00b 6b70 	and.w	fp, fp, #251658240	; 0xf000000
300097a0:	ea45 050b 	orr.w	r5, r5, fp
300097a4:	ea45 7506 	orr.w	r5, r5, r6, lsl #28
300097a8:	f8c3 5404 	str.w	r5, [r3, #1028]	; 0x404
300097ac:	9d0f      	ldr	r5, [sp, #60]	; 0x3c
300097ae:	f005 050f 	and.w	r5, r5, #15
300097b2:	f8c3 5408 	str.w	r5, [r3, #1032]	; 0x408
300097b6:	9d11      	ldr	r5, [sp, #68]	; 0x44
300097b8:	9e10      	ldr	r6, [sp, #64]	; 0x40
300097ba:	016d      	lsls	r5, r5, #5
300097bc:	f006 0b1f 	and.w	fp, r6, #31
300097c0:	9e12      	ldr	r6, [sp, #72]	; 0x48
300097c2:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
300097c6:	ea45 050b 	orr.w	r5, r5, fp
300097ca:	ea4f 2b86 	mov.w	fp, r6, lsl #10
300097ce:	f40b 4bf8 	and.w	fp, fp, #31744	; 0x7c00
300097d2:	ea45 050b 	orr.w	r5, r5, fp
300097d6:	f8c3 540c 	str.w	r5, [r3, #1036]	; 0x40c
300097da:	9d14      	ldr	r5, [sp, #80]	; 0x50
300097dc:	9e16      	ldr	r6, [sp, #88]	; 0x58
300097de:	ea4f 1b05 	mov.w	fp, r5, lsl #4
300097e2:	9d13      	ldr	r5, [sp, #76]	; 0x4c
300097e4:	fa5f fb8b 	uxtb.w	fp, fp
300097e8:	f005 050f 	and.w	r5, r5, #15
300097ec:	ea4b 0b05 	orr.w	fp, fp, r5
300097f0:	9d15      	ldr	r5, [sp, #84]	; 0x54
300097f2:	022d      	lsls	r5, r5, #8
300097f4:	f405 6570 	and.w	r5, r5, #3840	; 0xf00
300097f8:	ea4b 0505 	orr.w	r5, fp, r5
300097fc:	ea4f 3b06 	mov.w	fp, r6, lsl #12
30009800:	fa1f fb8b 	uxth.w	fp, fp
30009804:	ea45 050b 	orr.w	r5, r5, fp
30009808:	433d      	orrs	r5, r7
3000980a:	4328      	orrs	r0, r5
3000980c:	4301      	orrs	r1, r0
3000980e:	ea41 710a 	orr.w	r1, r1, sl, lsl #28
30009812:	f8c3 1410 	str.w	r1, [r3, #1040]	; 0x410
30009816:	f8c3 2414 	str.w	r2, [r3, #1044]	; 0x414
3000981a:	f008 021f 	and.w	r2, r8, #31
3000981e:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30009822:	f00c 021f 	and.w	r2, ip, #31
30009826:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
3000982a:	f8c3 4420 	str.w	r4, [r3, #1056]	; 0x420
3000982e:	9a00      	ldr	r2, [sp, #0]
30009830:	68d2      	ldr	r2, [r2, #12]
30009832:	6891      	ldr	r1, [r2, #8]
30009834:	e9d1 2000 	ldrd	r2, r0, [r1]
30009838:	f002 0201 	and.w	r2, r2, #1
3000983c:	ea42 4200 	orr.w	r2, r2, r0, lsl #16
30009840:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30009844:	9a00      	ldr	r2, [sp, #0]
30009846:	68d2      	ldr	r2, [r2, #12]
30009848:	6890      	ldr	r0, [r2, #8]
3000984a:	6941      	ldr	r1, [r0, #20]
3000984c:	7a02      	ldrb	r2, [r0, #8]
3000984e:	ea42 6201 	orr.w	r2, r2, r1, lsl #24
30009852:	68c1      	ldr	r1, [r0, #12]
30009854:	0209      	lsls	r1, r1, #8
30009856:	b289      	uxth	r1, r1
30009858:	430a      	orrs	r2, r1
3000985a:	6901      	ldr	r1, [r0, #16]
3000985c:	0409      	lsls	r1, r1, #16
3000985e:	f401 017f 	and.w	r1, r1, #16711680	; 0xff0000
30009862:	430a      	orrs	r2, r1
30009864:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30009868:	9a00      	ldr	r2, [sp, #0]
3000986a:	68d2      	ldr	r2, [r2, #12]
3000986c:	6890      	ldr	r0, [r2, #8]
3000986e:	69c2      	ldr	r2, [r0, #28]
30009870:	6a01      	ldr	r1, [r0, #32]
30009872:	0212      	lsls	r2, r2, #8
30009874:	0409      	lsls	r1, r1, #16
30009876:	b292      	uxth	r2, r2
30009878:	f401 017f 	and.w	r1, r1, #16711680	; 0xff0000
3000987c:	430a      	orrs	r2, r1
3000987e:	7e01      	ldrb	r1, [r0, #24]
30009880:	430a      	orrs	r2, r1
30009882:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30009886:	9a00      	ldr	r2, [sp, #0]
30009888:	68d2      	ldr	r2, [r2, #12]
3000988a:	6891      	ldr	r1, [r2, #8]
3000988c:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000988e:	6a49      	ldr	r1, [r1, #36]	; 0x24
30009890:	0412      	lsls	r2, r2, #16
30009892:	f001 013f 	and.w	r1, r1, #63	; 0x3f
30009896:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000989a:	430a      	orrs	r2, r1
3000989c:	f8c3 2480 	str.w	r2, [r3, #1152]	; 0x480
300098a0:	9a00      	ldr	r2, [sp, #0]
300098a2:	68d2      	ldr	r2, [r2, #12]
300098a4:	6890      	ldr	r0, [r2, #8]
300098a6:	6b02      	ldr	r2, [r0, #48]	; 0x30
300098a8:	6b81      	ldr	r1, [r0, #56]	; 0x38
300098aa:	0052      	lsls	r2, r2, #1
300098ac:	0409      	lsls	r1, r1, #16
300098ae:	f002 0202 	and.w	r2, r2, #2
300098b2:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
300098b6:	430a      	orrs	r2, r1
300098b8:	6ac1      	ldr	r1, [r0, #44]	; 0x2c
300098ba:	f001 0101 	and.w	r1, r1, #1
300098be:	430a      	orrs	r2, r1
300098c0:	6b41      	ldr	r1, [r0, #52]	; 0x34
300098c2:	0209      	lsls	r1, r1, #8
300098c4:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
300098c8:	430a      	orrs	r2, r1
300098ca:	f042 4280 	orr.w	r2, r2, #1073741824	; 0x40000000
300098ce:	f042 0208 	orr.w	r2, r2, #8
300098d2:	f8c3 2484 	str.w	r2, [r3, #1156]	; 0x484
300098d6:	9a00      	ldr	r2, [sp, #0]
300098d8:	68d2      	ldr	r2, [r2, #12]
300098da:	6892      	ldr	r2, [r2, #8]
300098dc:	6bd2      	ldr	r2, [r2, #60]	; 0x3c
300098de:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
300098e2:	9a00      	ldr	r2, [sp, #0]
300098e4:	68d2      	ldr	r2, [r2, #12]
300098e6:	6892      	ldr	r2, [r2, #8]
300098e8:	6c12      	ldr	r2, [r2, #64]	; 0x40
300098ea:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
300098ee:	9a00      	ldr	r2, [sp, #0]
300098f0:	68d2      	ldr	r2, [r2, #12]
300098f2:	6892      	ldr	r2, [r2, #8]
300098f4:	6c52      	ldr	r2, [r2, #68]	; 0x44
300098f6:	f8c3 2490 	str.w	r2, [r3, #1168]	; 0x490
300098fa:	9a00      	ldr	r2, [sp, #0]
300098fc:	68d2      	ldr	r2, [r2, #12]
300098fe:	6892      	ldr	r2, [r2, #8]
30009900:	6c92      	ldr	r2, [r2, #72]	; 0x48
30009902:	f8c3 2494 	str.w	r2, [r3, #1172]	; 0x494
30009906:	9a00      	ldr	r2, [sp, #0]
30009908:	68d2      	ldr	r2, [r2, #12]
3000990a:	6892      	ldr	r2, [r2, #8]
3000990c:	6cd2      	ldr	r2, [r2, #76]	; 0x4c
3000990e:	f8c3 2498 	str.w	r2, [r3, #1176]	; 0x498
30009912:	9a00      	ldr	r2, [sp, #0]
30009914:	68d2      	ldr	r2, [r2, #12]
30009916:	6892      	ldr	r2, [r2, #8]
30009918:	6d12      	ldr	r2, [r2, #80]	; 0x50
3000991a:	f002 020f 	and.w	r2, r2, #15
3000991e:	f8c3 24a0 	str.w	r2, [r3, #1184]	; 0x4a0
30009922:	f8d3 2470 	ldr.w	r2, [r3, #1136]	; 0x470
30009926:	f022 023f 	bic.w	r2, r2, #63	; 0x3f
3000992a:	f042 0204 	orr.w	r2, r2, #4
3000992e:	f8c3 2470 	str.w	r2, [r3, #1136]	; 0x470
30009932:	f8d3 2474 	ldr.w	r2, [r3, #1140]	; 0x474
30009936:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
3000993a:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
3000993e:	f8c3 2474 	str.w	r2, [r3, #1140]	; 0x474
30009942:	b041      	add	sp, #260	; 0x104
30009944:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009948:	2502      	movs	r5, #2
3000994a:	e50c      	b.n	30009366 <rxi316_perf_tune+0x1e>
3000994c:	2a01      	cmp	r2, #1
3000994e:	d102      	bne.n	30009956 <rxi316_perf_tune+0x60e>
30009950:	3101      	adds	r1, #1
30009952:	9314      	str	r3, [sp, #80]	; 0x50
30009954:	e577      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009956:	2a02      	cmp	r2, #2
30009958:	d102      	bne.n	30009960 <rxi316_perf_tune+0x618>
3000995a:	3101      	adds	r1, #1
3000995c:	9315      	str	r3, [sp, #84]	; 0x54
3000995e:	e572      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009960:	2a03      	cmp	r2, #3
30009962:	d102      	bne.n	3000996a <rxi316_perf_tune+0x622>
30009964:	3101      	adds	r1, #1
30009966:	9316      	str	r3, [sp, #88]	; 0x58
30009968:	e56d      	b.n	30009446 <rxi316_perf_tune+0xfe>
3000996a:	2a04      	cmp	r2, #4
3000996c:	d102      	bne.n	30009974 <rxi316_perf_tune+0x62c>
3000996e:	3101      	adds	r1, #1
30009970:	9317      	str	r3, [sp, #92]	; 0x5c
30009972:	e568      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009974:	2a05      	cmp	r2, #5
30009976:	d102      	bne.n	3000997e <rxi316_perf_tune+0x636>
30009978:	3101      	adds	r1, #1
3000997a:	9318      	str	r3, [sp, #96]	; 0x60
3000997c:	e563      	b.n	30009446 <rxi316_perf_tune+0xfe>
3000997e:	2a06      	cmp	r2, #6
30009980:	d102      	bne.n	30009988 <rxi316_perf_tune+0x640>
30009982:	3101      	adds	r1, #1
30009984:	9319      	str	r3, [sp, #100]	; 0x64
30009986:	e55e      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009988:	2a07      	cmp	r2, #7
3000998a:	d102      	bne.n	30009992 <rxi316_perf_tune+0x64a>
3000998c:	3101      	adds	r1, #1
3000998e:	931a      	str	r3, [sp, #104]	; 0x68
30009990:	e559      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009992:	2a08      	cmp	r2, #8
30009994:	d102      	bne.n	3000999c <rxi316_perf_tune+0x654>
30009996:	3101      	adds	r1, #1
30009998:	4699      	mov	r9, r3
3000999a:	e554      	b.n	30009446 <rxi316_perf_tune+0xfe>
3000999c:	2a09      	cmp	r2, #9
3000999e:	d102      	bne.n	300099a6 <rxi316_perf_tune+0x65e>
300099a0:	3101      	adds	r1, #1
300099a2:	931b      	str	r3, [sp, #108]	; 0x6c
300099a4:	e54f      	b.n	30009446 <rxi316_perf_tune+0xfe>
300099a6:	2a0a      	cmp	r2, #10
300099a8:	d102      	bne.n	300099b0 <rxi316_perf_tune+0x668>
300099aa:	3701      	adds	r7, #1
300099ac:	9310      	str	r3, [sp, #64]	; 0x40
300099ae:	e54a      	b.n	30009446 <rxi316_perf_tune+0xfe>
300099b0:	2a0b      	cmp	r2, #11
300099b2:	d102      	bne.n	300099ba <rxi316_perf_tune+0x672>
300099b4:	3701      	adds	r7, #1
300099b6:	9311      	str	r3, [sp, #68]	; 0x44
300099b8:	e545      	b.n	30009446 <rxi316_perf_tune+0xfe>
300099ba:	2a0c      	cmp	r2, #12
300099bc:	d102      	bne.n	300099c4 <rxi316_perf_tune+0x67c>
300099be:	3701      	adds	r7, #1
300099c0:	9312      	str	r3, [sp, #72]	; 0x48
300099c2:	e540      	b.n	30009446 <rxi316_perf_tune+0xfe>
300099c4:	2a0d      	cmp	r2, #13
300099c6:	d102      	bne.n	300099ce <rxi316_perf_tune+0x686>
300099c8:	3701      	adds	r7, #1
300099ca:	931d      	str	r3, [sp, #116]	; 0x74
300099cc:	e53b      	b.n	30009446 <rxi316_perf_tune+0xfe>
300099ce:	2a0e      	cmp	r2, #14
300099d0:	d102      	bne.n	300099d8 <rxi316_perf_tune+0x690>
300099d2:	3701      	adds	r7, #1
300099d4:	461c      	mov	r4, r3
300099d6:	e536      	b.n	30009446 <rxi316_perf_tune+0xfe>
300099d8:	2a0f      	cmp	r2, #15
300099da:	d105      	bne.n	300099e8 <rxi316_perf_tune+0x6a0>
300099dc:	3701      	adds	r7, #1
300099de:	931c      	str	r3, [sp, #112]	; 0x70
300099e0:	e531      	b.n	30009446 <rxi316_perf_tune+0xfe>
300099e2:	bf00      	nop
300099e4:	41100000 	.word	0x41100000
300099e8:	2a10      	cmp	r2, #16
300099ea:	d023      	beq.n	30009a34 <rxi316_perf_tune+0x6ec>
300099ec:	2a11      	cmp	r2, #17
300099ee:	d023      	beq.n	30009a38 <rxi316_perf_tune+0x6f0>
300099f0:	2a12      	cmp	r2, #18
300099f2:	d023      	beq.n	30009a3c <rxi316_perf_tune+0x6f4>
300099f4:	2a13      	cmp	r2, #19
300099f6:	d023      	beq.n	30009a40 <rxi316_perf_tune+0x6f8>
300099f8:	2a14      	cmp	r2, #20
300099fa:	d023      	beq.n	30009a44 <rxi316_perf_tune+0x6fc>
300099fc:	2a15      	cmp	r2, #21
300099fe:	d023      	beq.n	30009a48 <rxi316_perf_tune+0x700>
30009a00:	2a16      	cmp	r2, #22
30009a02:	d023      	beq.n	30009a4c <rxi316_perf_tune+0x704>
30009a04:	2a17      	cmp	r2, #23
30009a06:	d023      	beq.n	30009a50 <rxi316_perf_tune+0x708>
30009a08:	2a18      	cmp	r2, #24
30009a0a:	d023      	beq.n	30009a54 <rxi316_perf_tune+0x70c>
30009a0c:	2a19      	cmp	r2, #25
30009a0e:	d023      	beq.n	30009a58 <rxi316_perf_tune+0x710>
30009a10:	2a1a      	cmp	r2, #26
30009a12:	d023      	beq.n	30009a5c <rxi316_perf_tune+0x714>
30009a14:	2a1b      	cmp	r2, #27
30009a16:	d023      	beq.n	30009a60 <rxi316_perf_tune+0x718>
30009a18:	2a1c      	cmp	r2, #28
30009a1a:	d023      	beq.n	30009a64 <rxi316_perf_tune+0x71c>
30009a1c:	2a1d      	cmp	r2, #29
30009a1e:	d023      	beq.n	30009a68 <rxi316_perf_tune+0x720>
30009a20:	2a1e      	cmp	r2, #30
30009a22:	d023      	beq.n	30009a6c <rxi316_perf_tune+0x724>
30009a24:	2a1f      	cmp	r2, #31
30009a26:	d023      	beq.n	30009a70 <rxi316_perf_tune+0x728>
30009a28:	2a20      	cmp	r2, #32
30009a2a:	d023      	beq.n	30009a74 <rxi316_perf_tune+0x72c>
30009a2c:	2a21      	cmp	r2, #33	; 0x21
30009a2e:	bf08      	it	eq
30009a30:	461e      	moveq	r6, r3
30009a32:	e508      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a34:	469a      	mov	sl, r3
30009a36:	e506      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a38:	9303      	str	r3, [sp, #12]
30009a3a:	e504      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a3c:	4698      	mov	r8, r3
30009a3e:	e502      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a40:	9302      	str	r3, [sp, #8]
30009a42:	e500      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a44:	9301      	str	r3, [sp, #4]
30009a46:	e4fe      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a48:	9305      	str	r3, [sp, #20]
30009a4a:	e4fc      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a4c:	9306      	str	r3, [sp, #24]
30009a4e:	e4fa      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a50:	9307      	str	r3, [sp, #28]
30009a52:	e4f8      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a54:	469b      	mov	fp, r3
30009a56:	e4f6      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a58:	9308      	str	r3, [sp, #32]
30009a5a:	e4f4      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a5c:	9309      	str	r3, [sp, #36]	; 0x24
30009a5e:	e4f2      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a60:	930a      	str	r3, [sp, #40]	; 0x28
30009a62:	e4f0      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a64:	930b      	str	r3, [sp, #44]	; 0x2c
30009a66:	e4ee      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a68:	930c      	str	r3, [sp, #48]	; 0x30
30009a6a:	e4ec      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a6c:	930d      	str	r3, [sp, #52]	; 0x34
30009a6e:	e4ea      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a70:	930e      	str	r3, [sp, #56]	; 0x38
30009a72:	e4e8      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a74:	930f      	str	r3, [sp, #60]	; 0x3c
30009a76:	e4e6      	b.n	30009446 <rxi316_perf_tune+0xfe>
30009a78:	2201      	movs	r2, #1
30009a7a:	e4f3      	b.n	30009464 <rxi316_perf_tune+0x11c>
30009a7c:	2202      	movs	r2, #2
30009a7e:	e4f1      	b.n	30009464 <rxi316_perf_tune+0x11c>

30009a80 <rxi316_dram_init>:
30009a80:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009a84:	4604      	mov	r4, r0
30009a86:	b08d      	sub	sp, #52	; 0x34
30009a88:	4bb1      	ldr	r3, [pc, #708]	; (30009d50 <rxi316_dram_init+0x2d0>)
30009a8a:	4798      	blx	r3
30009a8c:	f894 a014 	ldrb.w	sl, [r4, #20]
30009a90:	2301      	movs	r3, #1
30009a92:	6822      	ldr	r2, [r4, #0]
30009a94:	900b      	str	r0, [sp, #44]	; 0x2c
30009a96:	fa03 f30a 	lsl.w	r3, r3, sl
30009a9a:	78d2      	ldrb	r2, [r2, #3]
30009a9c:	9306      	str	r3, [sp, #24]
30009a9e:	6923      	ldr	r3, [r4, #16]
30009aa0:	fa03 f30a 	lsl.w	r3, r3, sl
30009aa4:	b1ca      	cbz	r2, 30009ada <rxi316_dram_init+0x5a>
30009aa6:	2a01      	cmp	r2, #1
30009aa8:	d01b      	beq.n	30009ae2 <rxi316_dram_init+0x62>
30009aaa:	2a02      	cmp	r2, #2
30009aac:	bf0b      	itete	eq
30009aae:	2208      	moveq	r2, #8
30009ab0:	2210      	movne	r2, #16
30009ab2:	2501      	moveq	r5, #1
30009ab4:	2502      	movne	r5, #2
30009ab6:	9201      	str	r2, [sp, #4]
30009ab8:	6862      	ldr	r2, [r4, #4]
30009aba:	6811      	ldr	r1, [r2, #0]
30009abc:	e9d2 8201 	ldrd	r8, r2, [r2, #4]
30009ac0:	910a      	str	r1, [sp, #40]	; 0x28
30009ac2:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30009ac6:	9204      	str	r2, [sp, #16]
30009ac8:	4aa2      	ldr	r2, [pc, #648]	; (30009d54 <rxi316_dram_init+0x2d4>)
30009aca:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
30009ace:	6820      	ldr	r0, [r4, #0]
30009ad0:	7981      	ldrb	r1, [r0, #6]
30009ad2:	6882      	ldr	r2, [r0, #8]
30009ad4:	b949      	cbnz	r1, 30009aea <rxi316_dram_init+0x6a>
30009ad6:	3a06      	subs	r2, #6
30009ad8:	e00b      	b.n	30009af2 <rxi316_dram_init+0x72>
30009ada:	2202      	movs	r2, #2
30009adc:	2503      	movs	r5, #3
30009ade:	9201      	str	r2, [sp, #4]
30009ae0:	e7ea      	b.n	30009ab8 <rxi316_dram_init+0x38>
30009ae2:	2204      	movs	r2, #4
30009ae4:	2500      	movs	r5, #0
30009ae6:	9201      	str	r2, [sp, #4]
30009ae8:	e7e6      	b.n	30009ab8 <rxi316_dram_init+0x38>
30009aea:	2901      	cmp	r1, #1
30009aec:	f040 80f0 	bne.w	30009cd0 <rxi316_dram_init+0x250>
30009af0:	3a07      	subs	r2, #7
30009af2:	7881      	ldrb	r1, [r0, #2]
30009af4:	f002 020f 	and.w	r2, r2, #15
30009af8:	7940      	ldrb	r0, [r0, #5]
30009afa:	0109      	lsls	r1, r1, #4
30009afc:	0280      	lsls	r0, r0, #10
30009afe:	f001 0130 	and.w	r1, r1, #48	; 0x30
30009b02:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
30009b06:	4301      	orrs	r1, r0
30009b08:	ea41 1185 	orr.w	r1, r1, r5, lsl #6
30009b0c:	4311      	orrs	r1, r2
30009b0e:	4a91      	ldr	r2, [pc, #580]	; (30009d54 <rxi316_dram_init+0x2d4>)
30009b10:	f8c2 1224 	str.w	r1, [r2, #548]	; 0x224
30009b14:	68e1      	ldr	r1, [r4, #12]
30009b16:	6809      	ldr	r1, [r1, #0]
30009b18:	f891 106c 	ldrb.w	r1, [r1, #108]	; 0x6c
30009b1c:	f001 0107 	and.w	r1, r1, #7
30009b20:	f8c2 1228 	str.w	r1, [r2, #552]	; 0x228
30009b24:	6821      	ldr	r1, [r4, #0]
30009b26:	7809      	ldrb	r1, [r1, #0]
30009b28:	9102      	str	r1, [sp, #8]
30009b2a:	68e1      	ldr	r1, [r4, #12]
30009b2c:	6809      	ldr	r1, [r1, #0]
30009b2e:	6848      	ldr	r0, [r1, #4]
30009b30:	9003      	str	r0, [sp, #12]
30009b32:	9d03      	ldr	r5, [sp, #12]
30009b34:	6888      	ldr	r0, [r1, #8]
30009b36:	032d      	lsls	r5, r5, #12
30009b38:	0400      	lsls	r0, r0, #16
30009b3a:	f405 5580 	and.w	r5, r5, #4096	; 0x1000
30009b3e:	f400 3080 	and.w	r0, r0, #65536	; 0x10000
30009b42:	4305      	orrs	r5, r0
30009b44:	9802      	ldr	r0, [sp, #8]
30009b46:	f000 000f 	and.w	r0, r0, #15
30009b4a:	4305      	orrs	r5, r0
30009b4c:	68c8      	ldr	r0, [r1, #12]
30009b4e:	0440      	lsls	r0, r0, #17
30009b50:	f400 3000 	and.w	r0, r0, #131072	; 0x20000
30009b54:	4305      	orrs	r5, r0
30009b56:	6908      	ldr	r0, [r1, #16]
30009b58:	0480      	lsls	r0, r0, #18
30009b5a:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
30009b5e:	4305      	orrs	r5, r0
30009b60:	6948      	ldr	r0, [r1, #20]
30009b62:	04c0      	lsls	r0, r0, #19
30009b64:	f400 2000 	and.w	r0, r0, #524288	; 0x80000
30009b68:	4305      	orrs	r5, r0
30009b6a:	6988      	ldr	r0, [r1, #24]
30009b6c:	0540      	lsls	r0, r0, #21
30009b6e:	f400 1000 	and.w	r0, r0, #2097152	; 0x200000
30009b72:	4305      	orrs	r5, r0
30009b74:	69c8      	ldr	r0, [r1, #28]
30009b76:	0580      	lsls	r0, r0, #22
30009b78:	f400 0080 	and.w	r0, r0, #4194304	; 0x400000
30009b7c:	4305      	orrs	r5, r0
30009b7e:	6a88      	ldr	r0, [r1, #40]	; 0x28
30009b80:	0600      	lsls	r0, r0, #24
30009b82:	f000 7040 	and.w	r0, r0, #50331648	; 0x3000000
30009b86:	4305      	orrs	r5, r0
30009b88:	6a48      	ldr	r0, [r1, #36]	; 0x24
30009b8a:	06c0      	lsls	r0, r0, #27
30009b8c:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
30009b90:	4328      	orrs	r0, r5
30009b92:	6a0d      	ldr	r5, [r1, #32]
30009b94:	9906      	ldr	r1, [sp, #24]
30009b96:	07ad      	lsls	r5, r5, #30
30009b98:	f005 4580 	and.w	r5, r5, #1073741824	; 0x40000000
30009b9c:	4328      	orrs	r0, r5
30009b9e:	020d      	lsls	r5, r1, #8
30009ba0:	f405 65e0 	and.w	r5, r5, #1792	; 0x700
30009ba4:	4328      	orrs	r0, r5
30009ba6:	6050      	str	r0, [r2, #4]
30009ba8:	6821      	ldr	r1, [r4, #0]
30009baa:	7989      	ldrb	r1, [r1, #6]
30009bac:	2900      	cmp	r1, #0
30009bae:	f040 8094 	bne.w	30009cda <rxi316_dram_init+0x25a>
30009bb2:	6851      	ldr	r1, [r2, #4]
30009bb4:	f041 0110 	orr.w	r1, r1, #16
30009bb8:	6051      	str	r1, [r2, #4]
30009bba:	68e2      	ldr	r2, [r4, #12]
30009bbc:	6865      	ldr	r5, [r4, #4]
30009bbe:	6811      	ldr	r1, [r2, #0]
30009bc0:	6928      	ldr	r0, [r5, #16]
30009bc2:	e9d1 e20d 	ldrd	lr, r2, [r1, #52]	; 0x34
30009bc6:	e9d1 9c0b 	ldrd	r9, ip, [r1, #44]	; 0x2c
30009bca:	9205      	str	r2, [sp, #20]
30009bcc:	ea4f 0e8e 	mov.w	lr, lr, lsl #2
30009bd0:	6c4a      	ldr	r2, [r1, #68]	; 0x44
30009bd2:	f009 0901 	and.w	r9, r9, #1
30009bd6:	f00e 0e04 	and.w	lr, lr, #4
30009bda:	9207      	str	r2, [sp, #28]
30009bdc:	6c8a      	ldr	r2, [r1, #72]	; 0x48
30009bde:	9208      	str	r2, [sp, #32]
30009be0:	9a06      	ldr	r2, [sp, #24]
30009be2:	f102 3bff 	add.w	fp, r2, #4294967295	; 0xffffffff
30009be6:	fa20 f20a 	lsr.w	r2, r0, sl
30009bea:	ea10 0f0b 	tst.w	r0, fp
30009bee:	6968      	ldr	r0, [r5, #20]
30009bf0:	bf18      	it	ne
30009bf2:	3201      	addne	r2, #1
30009bf4:	fa20 f50a 	lsr.w	r5, r0, sl
30009bf8:	ea10 0f0b 	tst.w	r0, fp
30009bfc:	ea4f 004c 	mov.w	r0, ip, lsl #1
30009c00:	ea4f 3202 	mov.w	r2, r2, lsl #12
30009c04:	f000 0002 	and.w	r0, r0, #2
30009c08:	bf18      	it	ne
30009c0a:	3501      	addne	r5, #1
30009c0c:	f402 32f8 	and.w	r2, r2, #126976	; 0x1f000
30009c10:	ea40 0e0e 	orr.w	lr, r0, lr
30009c14:	9805      	ldr	r0, [sp, #20]
30009c16:	052d      	lsls	r5, r5, #20
30009c18:	ea4f 0cc0 	mov.w	ip, r0, lsl #3
30009c1c:	ea4e 0e09 	orr.w	lr, lr, r9
30009c20:	9807      	ldr	r0, [sp, #28]
30009c22:	f005 75f8 	and.w	r5, r5, #32505856	; 0x1f00000
30009c26:	f00c 0c08 	and.w	ip, ip, #8
30009c2a:	0180      	lsls	r0, r0, #6
30009c2c:	ea4e 0e0c 	orr.w	lr, lr, ip
30009c30:	f000 0040 	and.w	r0, r0, #64	; 0x40
30009c34:	e9d1 760f 	ldrd	r7, r6, [r1, #60]	; 0x3c
30009c38:	013f      	lsls	r7, r7, #4
30009c3a:	0176      	lsls	r6, r6, #5
30009c3c:	f007 0710 	and.w	r7, r7, #16
30009c40:	f006 0620 	and.w	r6, r6, #32
30009c44:	ea4e 0e07 	orr.w	lr, lr, r7
30009c48:	ea4e 0e06 	orr.w	lr, lr, r6
30009c4c:	ea4e 0e00 	orr.w	lr, lr, r0
30009c50:	9808      	ldr	r0, [sp, #32]
30009c52:	0206      	lsls	r6, r0, #8
30009c54:	6d08      	ldr	r0, [r1, #80]	; 0x50
30009c56:	f406 6670 	and.w	r6, r6, #3840	; 0xf00
30009c5a:	0700      	lsls	r0, r0, #28
30009c5c:	ea4e 0e06 	orr.w	lr, lr, r6
30009c60:	6cce      	ldr	r6, [r1, #76]	; 0x4c
30009c62:	f000 5080 	and.w	r0, r0, #268435456	; 0x10000000
30009c66:	06f6      	lsls	r6, r6, #27
30009c68:	f006 6600 	and.w	r6, r6, #134217728	; 0x8000000
30009c6c:	ea4e 0606 	orr.w	r6, lr, r6
30009c70:	4330      	orrs	r0, r6
30009c72:	6d4e      	ldr	r6, [r1, #84]	; 0x54
30009c74:	0776      	lsls	r6, r6, #29
30009c76:	f006 5600 	and.w	r6, r6, #536870912	; 0x20000000
30009c7a:	4330      	orrs	r0, r6
30009c7c:	4310      	orrs	r0, r2
30009c7e:	4a35      	ldr	r2, [pc, #212]	; (30009d54 <rxi316_dram_init+0x2d4>)
30009c80:	4328      	orrs	r0, r5
30009c82:	6090      	str	r0, [r2, #8]
30009c84:	6862      	ldr	r2, [r4, #4]
30009c86:	6990      	ldr	r0, [r2, #24]
30009c88:	9a01      	ldr	r2, [sp, #4]
30009c8a:	2a10      	cmp	r2, #16
30009c8c:	6822      	ldr	r2, [r4, #0]
30009c8e:	d137      	bne.n	30009d00 <rxi316_dram_init+0x280>
30009c90:	7812      	ldrb	r2, [r2, #0]
30009c92:	2a09      	cmp	r2, #9
30009c94:	bf14      	ite	ne
30009c96:	2100      	movne	r1, #0
30009c98:	2104      	moveq	r1, #4
30009c9a:	6822      	ldr	r2, [r4, #0]
30009c9c:	f892 c000 	ldrb.w	ip, [r2]
30009ca0:	7917      	ldrb	r7, [r2, #4]
30009ca2:	f1bc 0f02 	cmp.w	ip, #2
30009ca6:	f000 80cd 	beq.w	30009e44 <rxi316_dram_init+0x3c4>
30009caa:	f1bc 0f09 	cmp.w	ip, #9
30009cae:	f000 80c9 	beq.w	30009e44 <rxi316_dram_init+0x3c4>
30009cb2:	f1bc 0f03 	cmp.w	ip, #3
30009cb6:	f040 80c7 	bne.w	30009e48 <rxi316_dram_init+0x3c8>
30009cba:	f1b8 0f04 	cmp.w	r8, #4
30009cbe:	f240 80c6 	bls.w	30009e4e <rxi316_dram_init+0x3ce>
30009cc2:	f1b8 0f0b 	cmp.w	r8, #11
30009cc6:	d847      	bhi.n	30009d58 <rxi316_dram_init+0x2d8>
30009cc8:	f1a8 0604 	sub.w	r6, r8, #4
30009ccc:	2500      	movs	r5, #0
30009cce:	e046      	b.n	30009d5e <rxi316_dram_init+0x2de>
30009cd0:	2903      	cmp	r1, #3
30009cd2:	f47f af00 	bne.w	30009ad6 <rxi316_dram_init+0x56>
30009cd6:	3a08      	subs	r2, #8
30009cd8:	e70b      	b.n	30009af2 <rxi316_dram_init+0x72>
30009cda:	2901      	cmp	r1, #1
30009cdc:	d103      	bne.n	30009ce6 <rxi316_dram_init+0x266>
30009cde:	6851      	ldr	r1, [r2, #4]
30009ce0:	f041 0120 	orr.w	r1, r1, #32
30009ce4:	e768      	b.n	30009bb8 <rxi316_dram_init+0x138>
30009ce6:	2902      	cmp	r1, #2
30009ce8:	d103      	bne.n	30009cf2 <rxi316_dram_init+0x272>
30009cea:	6851      	ldr	r1, [r2, #4]
30009cec:	f041 0140 	orr.w	r1, r1, #64	; 0x40
30009cf0:	e762      	b.n	30009bb8 <rxi316_dram_init+0x138>
30009cf2:	2903      	cmp	r1, #3
30009cf4:	f47f af61 	bne.w	30009bba <rxi316_dram_init+0x13a>
30009cf8:	6851      	ldr	r1, [r2, #4]
30009cfa:	f041 0180 	orr.w	r1, r1, #128	; 0x80
30009cfe:	e75b      	b.n	30009bb8 <rxi316_dram_init+0x138>
30009d00:	7811      	ldrb	r1, [r2, #0]
30009d02:	9a01      	ldr	r2, [sp, #4]
30009d04:	2a08      	cmp	r2, #8
30009d06:	d106      	bne.n	30009d16 <rxi316_dram_init+0x296>
30009d08:	2902      	cmp	r1, #2
30009d0a:	d019      	beq.n	30009d40 <rxi316_dram_init+0x2c0>
30009d0c:	2909      	cmp	r1, #9
30009d0e:	bf14      	ite	ne
30009d10:	2100      	movne	r1, #0
30009d12:	2103      	moveq	r1, #3
30009d14:	e7c1      	b.n	30009c9a <rxi316_dram_init+0x21a>
30009d16:	9a01      	ldr	r2, [sp, #4]
30009d18:	2a04      	cmp	r2, #4
30009d1a:	d107      	bne.n	30009d2c <rxi316_dram_init+0x2ac>
30009d1c:	1e8a      	subs	r2, r1, #2
30009d1e:	2a01      	cmp	r2, #1
30009d20:	d910      	bls.n	30009d44 <rxi316_dram_init+0x2c4>
30009d22:	2909      	cmp	r1, #9
30009d24:	bf14      	ite	ne
30009d26:	2100      	movne	r1, #0
30009d28:	2102      	moveq	r1, #2
30009d2a:	e7b6      	b.n	30009c9a <rxi316_dram_init+0x21a>
30009d2c:	9a01      	ldr	r2, [sp, #4]
30009d2e:	2a02      	cmp	r2, #2
30009d30:	d10a      	bne.n	30009d48 <rxi316_dram_init+0x2c8>
30009d32:	2902      	cmp	r1, #2
30009d34:	d00a      	beq.n	30009d4c <rxi316_dram_init+0x2cc>
30009d36:	f1a1 0709 	sub.w	r7, r1, #9
30009d3a:	4279      	negs	r1, r7
30009d3c:	4179      	adcs	r1, r7
30009d3e:	e7ac      	b.n	30009c9a <rxi316_dram_init+0x21a>
30009d40:	2103      	movs	r1, #3
30009d42:	e7aa      	b.n	30009c9a <rxi316_dram_init+0x21a>
30009d44:	2102      	movs	r1, #2
30009d46:	e7a8      	b.n	30009c9a <rxi316_dram_init+0x21a>
30009d48:	2100      	movs	r1, #0
30009d4a:	e7a6      	b.n	30009c9a <rxi316_dram_init+0x21a>
30009d4c:	2101      	movs	r1, #1
30009d4e:	e7a4      	b.n	30009c9a <rxi316_dram_init+0x21a>
30009d50:	30007479 	.word	0x30007479
30009d54:	41100000 	.word	0x41100000
30009d58:	f1a8 060c 	sub.w	r6, r8, #12
30009d5c:	2501      	movs	r5, #1
30009d5e:	68a2      	ldr	r2, [r4, #8]
30009d60:	f1bc 0f02 	cmp.w	ip, #2
30009d64:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
30009d66:	d175      	bne.n	30009e54 <rxi316_dram_init+0x3d4>
30009d68:	f1a2 0c02 	sub.w	ip, r2, #2
30009d6c:	f1bc 0f06 	cmp.w	ip, #6
30009d70:	f200 8085 	bhi.w	30009e7e <rxi316_dram_init+0x3fe>
30009d74:	3a01      	subs	r2, #1
30009d76:	f04f 0c00 	mov.w	ip, #0
30009d7a:	ea40 00c7 	orr.w	r0, r0, r7, lsl #3
30009d7e:	4308      	orrs	r0, r1
30009d80:	4945      	ldr	r1, [pc, #276]	; (30009e98 <rxi316_dram_init+0x418>)
30009d82:	ea40 0085 	orr.w	r0, r0, r5, lsl #2
30009d86:	ea40 1006 	orr.w	r0, r0, r6, lsl #4
30009d8a:	ea40 2042 	orr.w	r0, r0, r2, lsl #9
30009d8e:	ea40 300c 	orr.w	r0, r0, ip, lsl #12
30009d92:	6348      	str	r0, [r1, #52]	; 0x34
30009d94:	6862      	ldr	r2, [r4, #4]
30009d96:	9804      	ldr	r0, [sp, #16]
30009d98:	69d2      	ldr	r2, [r2, #28]
30009d9a:	ea42 02c0 	orr.w	r2, r2, r0, lsl #3
30009d9e:	638a      	str	r2, [r1, #56]	; 0x38
30009da0:	6862      	ldr	r2, [r4, #4]
30009da2:	6a11      	ldr	r1, [r2, #32]
30009da4:	6822      	ldr	r2, [r4, #0]
30009da6:	7812      	ldrb	r2, [r2, #0]
30009da8:	2a03      	cmp	r2, #3
30009daa:	d16e      	bne.n	30009e8a <rxi316_dram_init+0x40a>
30009dac:	9a0a      	ldr	r2, [sp, #40]	; 0x28
30009dae:	1f50      	subs	r0, r2, #5
30009db0:	68e2      	ldr	r2, [r4, #12]
30009db2:	6812      	ldr	r2, [r2, #0]
30009db4:	6b92      	ldr	r2, [r2, #56]	; 0x38
30009db6:	fab2 f282 	clz	r2, r2
30009dba:	0952      	lsrs	r2, r2, #5
30009dbc:	0252      	lsls	r2, r2, #9
30009dbe:	ea42 00c0 	orr.w	r0, r2, r0, lsl #3
30009dc2:	4301      	orrs	r1, r0
30009dc4:	4834      	ldr	r0, [pc, #208]	; (30009e98 <rxi316_dram_init+0x418>)
30009dc6:	63c1      	str	r1, [r0, #60]	; 0x3c
30009dc8:	6862      	ldr	r2, [r4, #4]
30009dca:	6a52      	ldr	r2, [r2, #36]	; 0x24
30009dcc:	6402      	str	r2, [r0, #64]	; 0x40
30009dce:	6860      	ldr	r0, [r4, #4]
30009dd0:	68c5      	ldr	r5, [r0, #12]
30009dd2:	6801      	ldr	r1, [r0, #0]
30009dd4:	6842      	ldr	r2, [r0, #4]
30009dd6:	4429      	add	r1, r5
30009dd8:	6886      	ldr	r6, [r0, #8]
30009dda:	442a      	add	r2, r5
30009ddc:	ea1b 0f01 	tst.w	fp, r1
30009de0:	fa21 f10a 	lsr.w	r1, r1, sl
30009de4:	fa22 f50a 	lsr.w	r5, r2, sl
30009de8:	fa26 f00a 	lsr.w	r0, r6, sl
30009dec:	bf18      	it	ne
30009dee:	3101      	addne	r1, #1
30009df0:	ea1b 0f02 	tst.w	fp, r2
30009df4:	6822      	ldr	r2, [r4, #0]
30009df6:	bf18      	it	ne
30009df8:	3501      	addne	r5, #1
30009dfa:	ea1b 0f06 	tst.w	fp, r6
30009dfe:	f892 c000 	ldrb.w	ip, [r2]
30009e02:	bf18      	it	ne
30009e04:	3001      	addne	r0, #1
30009e06:	68a6      	ldr	r6, [r4, #8]
30009e08:	f1ac 0202 	sub.w	r2, ip, #2
30009e0c:	2a01      	cmp	r2, #1
30009e0e:	d845      	bhi.n	30009e9c <rxi316_dram_init+0x41c>
30009e10:	6df7      	ldr	r7, [r6, #92]	; 0x5c
30009e12:	ea1b 0f07 	tst.w	fp, r7
30009e16:	fa27 f20a 	lsr.w	r2, r7, sl
30009e1a:	d000      	beq.n	30009e1e <rxi316_dram_init+0x39e>
30009e1c:	3201      	adds	r2, #1
30009e1e:	f1bc 0f02 	cmp.w	ip, #2
30009e22:	6a77      	ldr	r7, [r6, #36]	; 0x24
30009e24:	f040 8285 	bne.w	3000a332 <rxi316_dram_init+0x8b2>
30009e28:	9e01      	ldr	r6, [sp, #4]
30009e2a:	0876      	lsrs	r6, r6, #1
30009e2c:	3e02      	subs	r6, #2
30009e2e:	fa26 f60a 	lsr.w	r6, r6, sl
30009e32:	4437      	add	r7, r6
30009e34:	fbb7 f6f3 	udiv	r6, r7, r3
30009e38:	fb03 7716 	mls	r7, r3, r6, r7
30009e3c:	2f00      	cmp	r7, #0
30009e3e:	d03c      	beq.n	30009eba <rxi316_dram_init+0x43a>
30009e40:	3601      	adds	r6, #1
30009e42:	e03a      	b.n	30009eba <rxi316_dram_init+0x43a>
30009e44:	4646      	mov	r6, r8
30009e46:	e741      	b.n	30009ccc <rxi316_dram_init+0x24c>
30009e48:	2500      	movs	r5, #0
30009e4a:	462e      	mov	r6, r5
30009e4c:	e787      	b.n	30009d5e <rxi316_dram_init+0x2de>
30009e4e:	2500      	movs	r5, #0
30009e50:	2601      	movs	r6, #1
30009e52:	e784      	b.n	30009d5e <rxi316_dram_init+0x2de>
30009e54:	f1bc 0f03 	cmp.w	ip, #3
30009e58:	d115      	bne.n	30009e86 <rxi316_dram_init+0x406>
30009e5a:	2a04      	cmp	r2, #4
30009e5c:	d911      	bls.n	30009e82 <rxi316_dram_init+0x402>
30009e5e:	f1a2 0c05 	sub.w	ip, r2, #5
30009e62:	f1bc 0f03 	cmp.w	ip, #3
30009e66:	d803      	bhi.n	30009e70 <rxi316_dram_init+0x3f0>
30009e68:	3a04      	subs	r2, #4
30009e6a:	f04f 0c01 	mov.w	ip, #1
30009e6e:	e784      	b.n	30009d7a <rxi316_dram_init+0x2fa>
30009e70:	f1a2 0c09 	sub.w	ip, r2, #9
30009e74:	f1bc 0f05 	cmp.w	ip, #5
30009e78:	d805      	bhi.n	30009e86 <rxi316_dram_init+0x406>
30009e7a:	0852      	lsrs	r2, r2, #1
30009e7c:	e7f5      	b.n	30009e6a <rxi316_dram_init+0x3ea>
30009e7e:	2200      	movs	r2, #0
30009e80:	e779      	b.n	30009d76 <rxi316_dram_init+0x2f6>
30009e82:	2201      	movs	r2, #1
30009e84:	e7f1      	b.n	30009e6a <rxi316_dram_init+0x3ea>
30009e86:	2200      	movs	r2, #0
30009e88:	e7ef      	b.n	30009e6a <rxi316_dram_init+0x3ea>
30009e8a:	2a09      	cmp	r2, #9
30009e8c:	f04f 0000 	mov.w	r0, #0
30009e90:	d08e      	beq.n	30009db0 <rxi316_dram_init+0x330>
30009e92:	4602      	mov	r2, r0
30009e94:	e792      	b.n	30009dbc <rxi316_dram_init+0x33c>
30009e96:	bf00      	nop
30009e98:	41100000 	.word	0x41100000
30009e9c:	69b7      	ldr	r7, [r6, #24]
30009e9e:	fbb7 f2f3 	udiv	r2, r7, r3
30009ea2:	fb03 7712 	mls	r7, r3, r2, r7
30009ea6:	b107      	cbz	r7, 30009eaa <rxi316_dram_init+0x42a>
30009ea8:	3201      	adds	r2, #1
30009eaa:	f1bc 0f09 	cmp.w	ip, #9
30009eae:	f040 8240 	bne.w	3000a332 <rxi316_dram_init+0x8b2>
30009eb2:	9e01      	ldr	r6, [sp, #4]
30009eb4:	0876      	lsrs	r6, r6, #1
30009eb6:	fa26 f60a 	lsr.w	r6, r6, sl
30009eba:	016d      	lsls	r5, r5, #5
30009ebc:	f001 011f 	and.w	r1, r1, #31
30009ec0:	0280      	lsls	r0, r0, #10
30009ec2:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
30009ec6:	0512      	lsls	r2, r2, #20
30009ec8:	f400 40f8 	and.w	r0, r0, #31744	; 0x7c00
30009ecc:	0676      	lsls	r6, r6, #25
30009ece:	4329      	orrs	r1, r5
30009ed0:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
30009ed4:	f006 5678 	and.w	r6, r6, #1040187392	; 0x3e000000
30009ed8:	4301      	orrs	r1, r0
30009eda:	4311      	orrs	r1, r2
30009edc:	4ab3      	ldr	r2, [pc, #716]	; (3000a1ac <rxi316_dram_init+0x72c>)
30009ede:	4331      	orrs	r1, r6
30009ee0:	6311      	str	r1, [r2, #48]	; 0x30
30009ee2:	68a1      	ldr	r1, [r4, #8]
30009ee4:	680a      	ldr	r2, [r1, #0]
30009ee6:	fbb2 f9f3 	udiv	r9, r2, r3
30009eea:	fb03 2219 	mls	r2, r3, r9, r2
30009eee:	b10a      	cbz	r2, 30009ef4 <rxi316_dram_init+0x474>
30009ef0:	f109 0901 	add.w	r9, r9, #1
30009ef4:	684a      	ldr	r2, [r1, #4]
30009ef6:	fbb2 f0f3 	udiv	r0, r2, r3
30009efa:	fb03 2210 	mls	r2, r3, r0, r2
30009efe:	2a00      	cmp	r2, #0
30009f00:	f000 8219 	beq.w	3000a336 <rxi316_dram_init+0x8b6>
30009f04:	38c7      	subs	r0, #199	; 0xc7
30009f06:	68e2      	ldr	r2, [r4, #12]
30009f08:	6811      	ldr	r1, [r2, #0]
30009f0a:	4aa9      	ldr	r2, [pc, #676]	; (3000a1b0 <rxi316_dram_init+0x730>)
30009f0c:	ea02 2200 	and.w	r2, r2, r0, lsl #8
30009f10:	9209      	str	r2, [sp, #36]	; 0x24
30009f12:	6dca      	ldr	r2, [r1, #92]	; 0x5c
30009f14:	0712      	lsls	r2, r2, #28
30009f16:	f002 5080 	and.w	r0, r2, #268435456	; 0x10000000
30009f1a:	6e0a      	ldr	r2, [r1, #96]	; 0x60
30009f1c:	9909      	ldr	r1, [sp, #36]	; 0x24
30009f1e:	ea41 7242 	orr.w	r2, r1, r2, lsl #29
30009f22:	fa5f f189 	uxtb.w	r1, r9
30009f26:	4302      	orrs	r2, r0
30009f28:	430a      	orrs	r2, r1
30009f2a:	49a0      	ldr	r1, [pc, #640]	; (3000a1ac <rxi316_dram_init+0x72c>)
30009f2c:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
30009f30:	610a      	str	r2, [r1, #16]
30009f32:	68a0      	ldr	r0, [r4, #8]
30009f34:	6902      	ldr	r2, [r0, #16]
30009f36:	fbb2 fef3 	udiv	lr, r2, r3
30009f3a:	fb03 221e 	mls	r2, r3, lr, r2
30009f3e:	2a00      	cmp	r2, #0
30009f40:	f000 81fb 	beq.w	3000a33a <rxi316_dram_init+0x8ba>
30009f44:	f10e 0201 	add.w	r2, lr, #1
30009f48:	9208      	str	r2, [sp, #32]
30009f4a:	6982      	ldr	r2, [r0, #24]
30009f4c:	fbb2 fcf3 	udiv	ip, r2, r3
30009f50:	fb03 221c 	mls	r2, r3, ip, r2
30009f54:	b10a      	cbz	r2, 30009f5a <rxi316_dram_init+0x4da>
30009f56:	f10c 0c01 	add.w	ip, ip, #1
30009f5a:	68c2      	ldr	r2, [r0, #12]
30009f5c:	fbb2 f1f3 	udiv	r1, r2, r3
30009f60:	fb03 2211 	mls	r2, r3, r1, r2
30009f64:	9104      	str	r1, [sp, #16]
30009f66:	b112      	cbz	r2, 30009f6e <rxi316_dram_init+0x4ee>
30009f68:	460a      	mov	r2, r1
30009f6a:	3201      	adds	r2, #1
30009f6c:	9204      	str	r2, [sp, #16]
30009f6e:	6a42      	ldr	r2, [r0, #36]	; 0x24
30009f70:	fbb2 f1f3 	udiv	r1, r2, r3
30009f74:	fb03 2211 	mls	r2, r3, r1, r2
30009f78:	b102      	cbz	r2, 30009f7c <rxi316_dram_init+0x4fc>
30009f7a:	3101      	adds	r1, #1
30009f7c:	68e2      	ldr	r2, [r4, #12]
30009f7e:	0349      	lsls	r1, r1, #13
30009f80:	6d05      	ldr	r5, [r0, #80]	; 0x50
30009f82:	6812      	ldr	r2, [r2, #0]
30009f84:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
30009f88:	fa25 f00a 	lsr.w	r0, r5, sl
30009f8c:	ea15 0f0b 	tst.w	r5, fp
30009f90:	6e52      	ldr	r2, [r2, #100]	; 0x64
30009f92:	bf18      	it	ne
30009f94:	3001      	addne	r0, #1
30009f96:	05d2      	lsls	r2, r2, #23
30009f98:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
30009f9c:	ea42 6000 	orr.w	r0, r2, r0, lsl #24
30009fa0:	9a08      	ldr	r2, [sp, #32]
30009fa2:	0112      	lsls	r2, r2, #4
30009fa4:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
30009fa8:	4310      	orrs	r0, r2
30009faa:	ea4f 224c 	mov.w	r2, ip, lsl #9
30009fae:	f402 52f0 	and.w	r2, r2, #7680	; 0x1e00
30009fb2:	4310      	orrs	r0, r2
30009fb4:	9a04      	ldr	r2, [sp, #16]
30009fb6:	f002 020f 	and.w	r2, r2, #15
30009fba:	4310      	orrs	r0, r2
30009fbc:	4a7b      	ldr	r2, [pc, #492]	; (3000a1ac <rxi316_dram_init+0x72c>)
30009fbe:	4308      	orrs	r0, r1
30009fc0:	f440 10a0 	orr.w	r0, r0, #1310720	; 0x140000
30009fc4:	6150      	str	r0, [r2, #20]
30009fc6:	f8d4 e008 	ldr.w	lr, [r4, #8]
30009fca:	f8de 2014 	ldr.w	r2, [lr, #20]
30009fce:	fbb2 f7f3 	udiv	r7, r2, r3
30009fd2:	fb03 2217 	mls	r2, r3, r7, r2
30009fd6:	b102      	cbz	r2, 30009fda <rxi316_dram_init+0x55a>
30009fd8:	3701      	adds	r7, #1
30009fda:	f8de 2010 	ldr.w	r2, [lr, #16]
30009fde:	fbb2 f1f3 	udiv	r1, r2, r3
30009fe2:	fb03 2211 	mls	r2, r3, r1, r2
30009fe6:	b102      	cbz	r2, 30009fea <rxi316_dram_init+0x56a>
30009fe8:	3101      	adds	r1, #1
30009fea:	f8de 000c 	ldr.w	r0, [lr, #12]
30009fee:	fbb0 f2f3 	udiv	r2, r0, r3
30009ff2:	fb03 0012 	mls	r0, r3, r2, r0
30009ff6:	b100      	cbz	r0, 30009ffa <rxi316_dram_init+0x57a>
30009ff8:	3201      	adds	r2, #1
30009ffa:	f8de 5030 	ldr.w	r5, [lr, #48]	; 0x30
30009ffe:	fbb5 f0f3 	udiv	r0, r5, r3
3000a002:	fb03 5510 	mls	r5, r3, r0, r5
3000a006:	b105      	cbz	r5, 3000a00a <rxi316_dram_init+0x58a>
3000a008:	3001      	adds	r0, #1
3000a00a:	440a      	add	r2, r1
3000a00c:	f8de 1008 	ldr.w	r1, [lr, #8]
3000a010:	fbb1 f5f3 	udiv	r5, r1, r3
3000a014:	4282      	cmp	r2, r0
3000a016:	fb03 1115 	mls	r1, r3, r5, r1
3000a01a:	bf38      	it	cc
3000a01c:	4602      	movcc	r2, r0
3000a01e:	b901      	cbnz	r1, 3000a022 <rxi316_dram_init+0x5a2>
3000a020:	3d01      	subs	r5, #1
3000a022:	f8de 102c 	ldr.w	r1, [lr, #44]	; 0x2c
3000a026:	f8de 001c 	ldr.w	r0, [lr, #28]
3000a02a:	ea11 0f0b 	tst.w	r1, fp
3000a02e:	fa21 f60a 	lsr.w	r6, r1, sl
3000a032:	fa20 f10a 	lsr.w	r1, r0, sl
3000a036:	bf18      	it	ne
3000a038:	3601      	addne	r6, #1
3000a03a:	ea10 0f0b 	tst.w	r0, fp
3000a03e:	9801      	ldr	r0, [sp, #4]
3000a040:	bf18      	it	ne
3000a042:	3101      	addne	r1, #1
3000a044:	0840      	lsrs	r0, r0, #1
3000a046:	9005      	str	r0, [sp, #20]
3000a048:	3002      	adds	r0, #2
3000a04a:	9007      	str	r0, [sp, #28]
3000a04c:	9802      	ldr	r0, [sp, #8]
3000a04e:	2809      	cmp	r0, #9
3000a050:	f040 8176 	bne.w	3000a340 <rxi316_dram_init+0x8c0>
3000a054:	9805      	ldr	r0, [sp, #20]
3000a056:	4480      	add	r8, r0
3000a058:	fa28 f00a 	lsr.w	r0, r8, sl
3000a05c:	ea18 0f0b 	tst.w	r8, fp
3000a060:	f8de 8040 	ldr.w	r8, [lr, #64]	; 0x40
3000a064:	bf18      	it	ne
3000a066:	3001      	addne	r0, #1
3000a068:	fbb8 fef3 	udiv	lr, r8, r3
3000a06c:	fb03 881e 	mls	r8, r3, lr, r8
3000a070:	f1b8 0f00 	cmp.w	r8, #0
3000a074:	d001      	beq.n	3000a07a <rxi316_dram_init+0x5fa>
3000a076:	f10e 0e01 	add.w	lr, lr, #1
3000a07a:	0112      	lsls	r2, r2, #4
3000a07c:	f007 070f 	and.w	r7, r7, #15
3000a080:	f402 727c 	and.w	r2, r2, #1008	; 0x3f0
3000a084:	4317      	orrs	r7, r2
3000a086:	02aa      	lsls	r2, r5, #10
3000a088:	f402 5270 	and.w	r2, r2, #15360	; 0x3c00
3000a08c:	433a      	orrs	r2, r7
3000a08e:	03b7      	lsls	r7, r6, #14
3000a090:	4e46      	ldr	r6, [pc, #280]	; (3000a1ac <rxi316_dram_init+0x72c>)
3000a092:	f407 37e0 	and.w	r7, r7, #114688	; 0x1c000
3000a096:	4317      	orrs	r7, r2
3000a098:	044a      	lsls	r2, r1, #17
3000a09a:	f402 2260 	and.w	r2, r2, #917504	; 0xe0000
3000a09e:	4317      	orrs	r7, r2
3000a0a0:	0502      	lsls	r2, r0, #20
3000a0a2:	f402 0270 	and.w	r2, r2, #15728640	; 0xf00000
3000a0a6:	4317      	orrs	r7, r2
3000a0a8:	ea4f 620e 	mov.w	r2, lr, lsl #24
3000a0ac:	f002 52f8 	and.w	r2, r2, #520093696	; 0x1f000000
3000a0b0:	433a      	orrs	r2, r7
3000a0b2:	61b2      	str	r2, [r6, #24]
3000a0b4:	6822      	ldr	r2, [r4, #0]
3000a0b6:	7816      	ldrb	r6, [r2, #0]
3000a0b8:	68a2      	ldr	r2, [r4, #8]
3000a0ba:	2e02      	cmp	r6, #2
3000a0bc:	f000 8150 	beq.w	3000a360 <rxi316_dram_init+0x8e0>
3000a0c0:	2e09      	cmp	r6, #9
3000a0c2:	6a17      	ldr	r7, [r2, #32]
3000a0c4:	f000 8155 	beq.w	3000a372 <rxi316_dram_init+0x8f2>
3000a0c8:	68e2      	ldr	r2, [r4, #12]
3000a0ca:	2e03      	cmp	r6, #3
3000a0cc:	6812      	ldr	r2, [r2, #0]
3000a0ce:	f8d2 8068 	ldr.w	r8, [r2, #104]	; 0x68
3000a0d2:	d005      	beq.n	3000a0e0 <rxi316_dram_init+0x660>
3000a0d4:	ea0b 0207 	and.w	r2, fp, r7
3000a0d8:	fa27 f70a 	lsr.w	r7, r7, sl
3000a0dc:	b102      	cbz	r2, 3000a0e0 <rxi316_dram_init+0x660>
3000a0de:	3701      	adds	r7, #1
3000a0e0:	9a02      	ldr	r2, [sp, #8]
3000a0e2:	2a02      	cmp	r2, #2
3000a0e4:	f040 8148 	bne.w	3000a378 <rxi316_dram_init+0x8f8>
3000a0e8:	4e32      	ldr	r6, [pc, #200]	; (3000a1b4 <rxi316_dram_init+0x734>)
3000a0ea:	4a33      	ldr	r2, [pc, #204]	; (3000a1b8 <rxi316_dram_init+0x738>)
3000a0ec:	fbb6 f6f3 	udiv	r6, r6, r3
3000a0f0:	fbb2 f2f3 	udiv	r2, r2, r3
3000a0f4:	0ab6      	lsrs	r6, r6, #10
3000a0f6:	09d2      	lsrs	r2, r2, #7
3000a0f8:	3601      	adds	r6, #1
3000a0fa:	3201      	adds	r2, #1
3000a0fc:	f04f 0e00 	mov.w	lr, #0
3000a100:	013f      	lsls	r7, r7, #4
3000a102:	f008 080f 	and.w	r8, r8, #15
3000a106:	0212      	lsls	r2, r2, #8
3000a108:	b2ff      	uxtb	r7, r7
3000a10a:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000a10e:	ea47 0808 	orr.w	r8, r7, r8
3000a112:	ea48 588e 	orr.w	r8, r8, lr, lsl #22
3000a116:	ea48 0802 	orr.w	r8, r8, r2
3000a11a:	4a28      	ldr	r2, [pc, #160]	; (3000a1bc <rxi316_dram_init+0x73c>)
3000a11c:	ea02 3206 	and.w	r2, r2, r6, lsl #12
3000a120:	4e22      	ldr	r6, [pc, #136]	; (3000a1ac <rxi316_dram_init+0x72c>)
3000a122:	ea48 0202 	orr.w	r2, r8, r2
3000a126:	61f2      	str	r2, [r6, #28]
3000a128:	f8d4 8008 	ldr.w	r8, [r4, #8]
3000a12c:	9a08      	ldr	r2, [sp, #32]
3000a12e:	f8d8 4040 	ldr.w	r4, [r8, #64]	; 0x40
3000a132:	ea4f 1e52 	mov.w	lr, r2, lsr #5
3000a136:	fbb4 f2f3 	udiv	r2, r4, r3
3000a13a:	fb03 4312 	mls	r3, r3, r2, r4
3000a13e:	b103      	cbz	r3, 3000a142 <rxi316_dram_init+0x6c2>
3000a140:	3201      	adds	r2, #1
3000a142:	08cc      	lsrs	r4, r1, #3
3000a144:	f8d8 1034 	ldr.w	r1, [r8, #52]	; 0x34
3000a148:	092b      	lsrs	r3, r5, #4
3000a14a:	f8d8 503c 	ldr.w	r5, [r8, #60]	; 0x3c
3000a14e:	ea11 0f0b 	tst.w	r1, fp
3000a152:	fa21 f70a 	lsr.w	r7, r1, sl
3000a156:	f8d8 1038 	ldr.w	r1, [r8, #56]	; 0x38
3000a15a:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000a15e:	bf18      	it	ne
3000a160:	3701      	addne	r7, #1
3000a162:	f8d8 802c 	ldr.w	r8, [r8, #44]	; 0x2c
3000a166:	ea11 0f0b 	tst.w	r1, fp
3000a16a:	fa21 f60a 	lsr.w	r6, r1, sl
3000a16e:	fa25 f10a 	lsr.w	r1, r5, sl
3000a172:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000a176:	bf18      	it	ne
3000a178:	3601      	addne	r6, #1
3000a17a:	ea15 0f0b 	tst.w	r5, fp
3000a17e:	9d01      	ldr	r5, [sp, #4]
3000a180:	ea4f 2919 	mov.w	r9, r9, lsr #8
3000a184:	bf18      	it	ne
3000a186:	3101      	addne	r1, #1
3000a188:	ea4f 1c1c 	mov.w	ip, ip, lsr #4
3000a18c:	fa25 f50a 	lsr.w	r5, r5, sl
3000a190:	9302      	str	r3, [sp, #8]
3000a192:	4545      	cmp	r5, r8
3000a194:	f240 8115 	bls.w	3000a3c2 <rxi316_dram_init+0x942>
3000a198:	eba5 0508 	sub.w	r5, r5, r8
3000a19c:	fa25 f10a 	lsr.w	r1, r5, sl
3000a1a0:	ea15 050b 	ands.w	r5, r5, fp
3000a1a4:	d00c      	beq.n	3000a1c0 <rxi316_dram_init+0x740>
3000a1a6:	3101      	adds	r1, #1
3000a1a8:	2500      	movs	r5, #0
3000a1aa:	e009      	b.n	3000a1c0 <rxi316_dram_init+0x740>
3000a1ac:	41100000 	.word	0x41100000
3000a1b0:	00ffff00 	.word	0x00ffff00
3000a1b4:	0bebc200 	.word	0x0bebc200
3000a1b8:	00061a80 	.word	0x00061a80
3000a1bc:	003ff000 	.word	0x003ff000
3000a1c0:	9b03      	ldr	r3, [sp, #12]
3000a1c2:	ea4f 0e8e 	mov.w	lr, lr, lsl #2
3000a1c6:	ea4f 2989 	mov.w	r9, r9, lsl #10
3000a1ca:	2b00      	cmp	r3, #0
3000a1cc:	f00e 0e04 	and.w	lr, lr, #4
3000a1d0:	f409 6980 	and.w	r9, r9, #1024	; 0x400
3000a1d4:	ea4f 2ccc 	mov.w	ip, ip, lsl #11
3000a1d8:	bf18      	it	ne
3000a1da:	462b      	movne	r3, r5
3000a1dc:	0224      	lsls	r4, r4, #8
3000a1de:	ea4e 0e09 	orr.w	lr, lr, r9
3000a1e2:	f40c 6c00 	and.w	ip, ip, #2048	; 0x800
3000a1e6:	9303      	str	r3, [sp, #12]
3000a1e8:	0240      	lsls	r0, r0, #9
3000a1ea:	9b02      	ldr	r3, [sp, #8]
3000a1ec:	ea4e 0e0c 	orr.w	lr, lr, ip
3000a1f0:	f400 7000 	and.w	r0, r0, #512	; 0x200
3000a1f4:	00d2      	lsls	r2, r2, #3
3000a1f6:	01db      	lsls	r3, r3, #7
3000a1f8:	f002 0208 	and.w	r2, r2, #8
3000a1fc:	03bf      	lsls	r7, r7, #14
3000a1fe:	b2db      	uxtb	r3, r3
3000a200:	f407 37e0 	and.w	r7, r7, #114688	; 0x1c000
3000a204:	0476      	lsls	r6, r6, #17
3000a206:	ea4e 0e03 	orr.w	lr, lr, r3
3000a20a:	f404 7380 	and.w	r3, r4, #256	; 0x100
3000a20e:	f406 2660 	and.w	r6, r6, #917504	; 0xe0000
3000a212:	0509      	lsls	r1, r1, #20
3000a214:	ea4e 0e03 	orr.w	lr, lr, r3
3000a218:	9b03      	ldr	r3, [sp, #12]
3000a21a:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a21e:	ea4e 0e00 	orr.w	lr, lr, r0
3000a222:	ea4e 0202 	orr.w	r2, lr, r2
3000a226:	4317      	orrs	r7, r2
3000a228:	433e      	orrs	r6, r7
3000a22a:	4331      	orrs	r1, r6
3000a22c:	ea41 7103 	orr.w	r1, r1, r3, lsl #28
3000a230:	4b68      	ldr	r3, [pc, #416]	; (3000a3d4 <rxi316_dram_init+0x954>)
3000a232:	6219      	str	r1, [r3, #32]
3000a234:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000a238:	9a04      	ldr	r2, [sp, #16]
3000a23a:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000a23e:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000a242:	9a06      	ldr	r2, [sp, #24]
3000a244:	2a01      	cmp	r2, #1
3000a246:	bf1e      	ittt	ne
3000a248:	9a05      	ldrne	r2, [sp, #20]
3000a24a:	3203      	addne	r2, #3
3000a24c:	9207      	strne	r2, [sp, #28]
3000a24e:	9a07      	ldr	r2, [sp, #28]
3000a250:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000a254:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000a258:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000a25c:	60da      	str	r2, [r3, #12]
3000a25e:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a262:	430a      	orrs	r2, r1
3000a264:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a268:	4a5b      	ldr	r2, [pc, #364]	; (3000a3d8 <rxi316_dram_init+0x958>)
3000a26a:	f8d2 1268 	ldr.w	r1, [r2, #616]	; 0x268
3000a26e:	008e      	lsls	r6, r1, #2
3000a270:	f140 80a9 	bpl.w	3000a3c6 <rxi316_dram_init+0x946>
3000a274:	6919      	ldr	r1, [r3, #16]
3000a276:	4c57      	ldr	r4, [pc, #348]	; (3000a3d4 <rxi316_dram_init+0x954>)
3000a278:	f041 5180 	orr.w	r1, r1, #268435456	; 0x10000000
3000a27c:	6119      	str	r1, [r3, #16]
3000a27e:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a282:	6019      	str	r1, [r3, #0]
3000a284:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000a288:	f043 0303 	orr.w	r3, r3, #3
3000a28c:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
3000a290:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a292:	2b01      	cmp	r3, #1
3000a294:	4b51      	ldr	r3, [pc, #324]	; (3000a3dc <rxi316_dram_init+0x95c>)
3000a296:	bf0c      	ite	eq
3000a298:	4a51      	ldreq	r2, [pc, #324]	; (3000a3e0 <rxi316_dram_init+0x960>)
3000a29a:	4a52      	ldrne	r2, [pc, #328]	; (3000a3e4 <rxi316_dram_init+0x964>)
3000a29c:	601a      	str	r2, [r3, #0]
3000a29e:	68e3      	ldr	r3, [r4, #12]
3000a2a0:	f423 23e0 	bic.w	r3, r3, #458752	; 0x70000
3000a2a4:	f023 0303 	bic.w	r3, r3, #3
3000a2a8:	60e3      	str	r3, [r4, #12]
3000a2aa:	f44f 0360 	mov.w	r3, #14680064	; 0xe00000
3000a2ae:	62a3      	str	r3, [r4, #40]	; 0x28
3000a2b0:	2303      	movs	r3, #3
3000a2b2:	62e3      	str	r3, [r4, #44]	; 0x2c
3000a2b4:	2308      	movs	r3, #8
3000a2b6:	6023      	str	r3, [r4, #0]
3000a2b8:	6823      	ldr	r3, [r4, #0]
3000a2ba:	071d      	lsls	r5, r3, #28
3000a2bc:	d5fc      	bpl.n	3000a2b8 <rxi316_dram_init+0x838>
3000a2be:	4b4a      	ldr	r3, [pc, #296]	; (3000a3e8 <rxi316_dram_init+0x968>)
3000a2c0:	2001      	movs	r0, #1
3000a2c2:	4798      	blx	r3
3000a2c4:	68e3      	ldr	r3, [r4, #12]
3000a2c6:	4949      	ldr	r1, [pc, #292]	; (3000a3ec <rxi316_dram_init+0x96c>)
3000a2c8:	400b      	ands	r3, r1
3000a2ca:	60e3      	str	r3, [r4, #12]
3000a2cc:	f44f 1300 	mov.w	r3, #2097152	; 0x200000
3000a2d0:	62a3      	str	r3, [r4, #40]	; 0x28
3000a2d2:	2303      	movs	r3, #3
3000a2d4:	62e3      	str	r3, [r4, #44]	; 0x2c
3000a2d6:	2308      	movs	r3, #8
3000a2d8:	6023      	str	r3, [r4, #0]
3000a2da:	4b3e      	ldr	r3, [pc, #248]	; (3000a3d4 <rxi316_dram_init+0x954>)
3000a2dc:	681a      	ldr	r2, [r3, #0]
3000a2de:	0710      	lsls	r0, r2, #28
3000a2e0:	d5fc      	bpl.n	3000a2dc <rxi316_dram_init+0x85c>
3000a2e2:	68da      	ldr	r2, [r3, #12]
3000a2e4:	400a      	ands	r2, r1
3000a2e6:	60da      	str	r2, [r3, #12]
3000a2e8:	f04f 7290 	mov.w	r2, #18874368	; 0x1200000
3000a2ec:	629a      	str	r2, [r3, #40]	; 0x28
3000a2ee:	6ada      	ldr	r2, [r3, #44]	; 0x2c
3000a2f0:	f022 0202 	bic.w	r2, r2, #2
3000a2f4:	62da      	str	r2, [r3, #44]	; 0x2c
3000a2f6:	2208      	movs	r2, #8
3000a2f8:	601a      	str	r2, [r3, #0]
3000a2fa:	4a36      	ldr	r2, [pc, #216]	; (3000a3d4 <rxi316_dram_init+0x954>)
3000a2fc:	6813      	ldr	r3, [r2, #0]
3000a2fe:	0719      	lsls	r1, r3, #28
3000a300:	d5fc      	bpl.n	3000a2fc <rxi316_dram_init+0x87c>
3000a302:	4b34      	ldr	r3, [pc, #208]	; (3000a3d4 <rxi316_dram_init+0x954>)
3000a304:	f44f 62c0 	mov.w	r2, #1536	; 0x600
3000a308:	60da      	str	r2, [r3, #12]
3000a30a:	9909      	ldr	r1, [sp, #36]	; 0x24
3000a30c:	691a      	ldr	r2, [r3, #16]
3000a30e:	4311      	orrs	r1, r2
3000a310:	6119      	str	r1, [r3, #16]
3000a312:	691a      	ldr	r2, [r3, #16]
3000a314:	f022 5280 	bic.w	r2, r2, #268435456	; 0x10000000
3000a318:	611a      	str	r2, [r3, #16]
3000a31a:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000a31e:	601a      	str	r2, [r3, #0]
3000a320:	68da      	ldr	r2, [r3, #12]
3000a322:	f3c2 020a 	ubfx	r2, r2, #0, #11
3000a326:	f5b2 6fc0 	cmp.w	r2, #1536	; 0x600
3000a32a:	d1f9      	bne.n	3000a320 <rxi316_dram_init+0x8a0>
3000a32c:	b00d      	add	sp, #52	; 0x34
3000a32e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a332:	6a77      	ldr	r7, [r6, #36]	; 0x24
3000a334:	e57e      	b.n	30009e34 <rxi316_dram_init+0x3b4>
3000a336:	38c8      	subs	r0, #200	; 0xc8
3000a338:	e5e5      	b.n	30009f06 <rxi316_dram_init+0x486>
3000a33a:	f8cd e020 	str.w	lr, [sp, #32]
3000a33e:	e604      	b.n	30009f4a <rxi316_dram_init+0x4ca>
3000a340:	9802      	ldr	r0, [sp, #8]
3000a342:	2802      	cmp	r0, #2
3000a344:	d009      	beq.n	3000a35a <rxi316_dram_init+0x8da>
3000a346:	2803      	cmp	r0, #3
3000a348:	d105      	bne.n	3000a356 <rxi316_dram_init+0x8d6>
3000a34a:	f108 0802 	add.w	r8, r8, #2
3000a34e:	980a      	ldr	r0, [sp, #40]	; 0x28
3000a350:	eba8 0800 	sub.w	r8, r8, r0
3000a354:	e67e      	b.n	3000a054 <rxi316_dram_init+0x5d4>
3000a356:	9801      	ldr	r0, [sp, #4]
3000a358:	e67d      	b.n	3000a056 <rxi316_dram_init+0x5d6>
3000a35a:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000a35e:	e67b      	b.n	3000a058 <rxi316_dram_init+0x5d8>
3000a360:	6a12      	ldr	r2, [r2, #32]
3000a362:	fbb2 f7f3 	udiv	r7, r2, r3
3000a366:	fb03 2217 	mls	r2, r3, r7, r2
3000a36a:	b102      	cbz	r2, 3000a36e <rxi316_dram_init+0x8ee>
3000a36c:	3701      	adds	r7, #1
3000a36e:	46b0      	mov	r8, r6
3000a370:	e6b6      	b.n	3000a0e0 <rxi316_dram_init+0x660>
3000a372:	f04f 0802 	mov.w	r8, #2
3000a376:	e6ad      	b.n	3000a0d4 <rxi316_dram_init+0x654>
3000a378:	9a02      	ldr	r2, [sp, #8]
3000a37a:	2a03      	cmp	r2, #3
3000a37c:	d113      	bne.n	3000a3a6 <rxi316_dram_init+0x926>
3000a37e:	4a1c      	ldr	r2, [pc, #112]	; (3000a3f0 <rxi316_dram_init+0x970>)
3000a380:	4e1c      	ldr	r6, [pc, #112]	; (3000a3f4 <rxi316_dram_init+0x974>)
3000a382:	fbb2 fef3 	udiv	lr, r2, r3
3000a386:	f242 7210 	movw	r2, #10000	; 0x2710
3000a38a:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000a38e:	fbb2 f2f3 	udiv	r2, r2, r3
3000a392:	fbb6 f6f3 	udiv	r6, r6, r3
3000a396:	444a      	add	r2, r9
3000a398:	0ab6      	lsrs	r6, r6, #10
3000a39a:	f10e 0e01 	add.w	lr, lr, #1
3000a39e:	09d2      	lsrs	r2, r2, #7
3000a3a0:	3601      	adds	r6, #1
3000a3a2:	3201      	adds	r2, #1
3000a3a4:	e6ac      	b.n	3000a100 <rxi316_dram_init+0x680>
3000a3a6:	9a02      	ldr	r2, [sp, #8]
3000a3a8:	2a09      	cmp	r2, #9
3000a3aa:	d106      	bne.n	3000a3ba <rxi316_dram_init+0x93a>
3000a3ac:	4e10      	ldr	r6, [pc, #64]	; (3000a3f0 <rxi316_dram_init+0x970>)
3000a3ae:	2200      	movs	r2, #0
3000a3b0:	fbb6 f6f3 	udiv	r6, r6, r3
3000a3b4:	0ab6      	lsrs	r6, r6, #10
3000a3b6:	3601      	adds	r6, #1
3000a3b8:	e6a0      	b.n	3000a0fc <rxi316_dram_init+0x67c>
3000a3ba:	2200      	movs	r2, #0
3000a3bc:	4616      	mov	r6, r2
3000a3be:	4696      	mov	lr, r2
3000a3c0:	e69e      	b.n	3000a100 <rxi316_dram_init+0x680>
3000a3c2:	2501      	movs	r5, #1
3000a3c4:	e6fc      	b.n	3000a1c0 <rxi316_dram_init+0x740>
3000a3c6:	2201      	movs	r2, #1
3000a3c8:	601a      	str	r2, [r3, #0]
3000a3ca:	4a02      	ldr	r2, [pc, #8]	; (3000a3d4 <rxi316_dram_init+0x954>)
3000a3cc:	6813      	ldr	r3, [r2, #0]
3000a3ce:	07db      	lsls	r3, r3, #31
3000a3d0:	d5fc      	bpl.n	3000a3cc <rxi316_dram_init+0x94c>
3000a3d2:	e796      	b.n	3000a302 <rxi316_dram_init+0x882>
3000a3d4:	41100000 	.word	0x41100000
3000a3d8:	42008000 	.word	0x42008000
3000a3dc:	41011000 	.word	0x41011000
3000a3e0:	44000103 	.word	0x44000103
3000a3e4:	44000021 	.word	0x44000021
3000a3e8:	00009b2d 	.word	0x00009b2d
3000a3ec:	fff8fffc 	.word	0xfff8fffc
3000a3f0:	0bebc200 	.word	0x0bebc200
3000a3f4:	1dcd6500 	.word	0x1dcd6500

3000a3f8 <rxi316_ftend_init>:
3000a3f8:	2300      	movs	r3, #0
3000a3fa:	b510      	push	{r4, lr}
3000a3fc:	4c0c      	ldr	r4, [pc, #48]	; (3000a430 <rxi316_ftend_init+0x38>)
3000a3fe:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000a402:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000a406:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000a40a:	f7fe ff73 	bl	300092f4 <ddr_init_index>
3000a40e:	4b09      	ldr	r3, [pc, #36]	; (3000a434 <rxi316_ftend_init+0x3c>)
3000a410:	f8d4 2104 	ldr.w	r2, [r4, #260]	; 0x104
3000a414:	f853 3020 	ldr.w	r3, [r3, r0, lsl #2]
3000a418:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000a41c:	68db      	ldr	r3, [r3, #12]
3000a41e:	689b      	ldr	r3, [r3, #8]
3000a420:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000a422:	3301      	adds	r3, #1
3000a424:	b2db      	uxtb	r3, r3
3000a426:	4313      	orrs	r3, r2
3000a428:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000a42c:	bd10      	pop	{r4, pc}
3000a42e:	bf00      	nop
3000a430:	41101000 	.word	0x41101000
3000a434:	3000e378 	.word	0x3000e378

3000a438 <rxi316_DynSre_init>:
3000a438:	2901      	cmp	r1, #1
3000a43a:	4b12      	ldr	r3, [pc, #72]	; (3000a484 <rxi316_DynSre_init+0x4c>)
3000a43c:	d11e      	bne.n	3000a47c <rxi316_DynSre_init+0x44>
3000a43e:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a442:	b280      	uxth	r0, r0
3000a444:	0c12      	lsrs	r2, r2, #16
3000a446:	0412      	lsls	r2, r2, #16
3000a448:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a44c:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a450:	4310      	orrs	r0, r2
3000a452:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000a456:	689a      	ldr	r2, [r3, #8]
3000a458:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a45c:	609a      	str	r2, [r3, #8]
3000a45e:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000a462:	4b08      	ldr	r3, [pc, #32]	; (3000a484 <rxi316_DynSre_init+0x4c>)
3000a464:	625a      	str	r2, [r3, #36]	; 0x24
3000a466:	629a      	str	r2, [r3, #40]	; 0x28
3000a468:	69da      	ldr	r2, [r3, #28]
3000a46a:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000a46e:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000a472:	61da      	str	r2, [r3, #28]
3000a474:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000a478:	601a      	str	r2, [r3, #0]
3000a47a:	4770      	bx	lr
3000a47c:	689a      	ldr	r2, [r3, #8]
3000a47e:	f022 0280 	bic.w	r2, r2, #128	; 0x80
3000a482:	e7eb      	b.n	3000a45c <rxi316_DynSre_init+0x24>
3000a484:	41100000 	.word	0x41100000

3000a488 <ddr_init>:
3000a488:	b507      	push	{r0, r1, r2, lr}
3000a48a:	f7fe ff33 	bl	300092f4 <ddr_init_index>
3000a48e:	4b07      	ldr	r3, [pc, #28]	; (3000a4ac <ddr_init+0x24>)
3000a490:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000a494:	9001      	str	r0, [sp, #4]
3000a496:	f7fe ff57 	bl	30009348 <rxi316_perf_tune>
3000a49a:	9801      	ldr	r0, [sp, #4]
3000a49c:	f7ff faf0 	bl	30009a80 <rxi316_dram_init>
3000a4a0:	b003      	add	sp, #12
3000a4a2:	f85d eb04 	ldr.w	lr, [sp], #4
3000a4a6:	f7ff bfa7 	b.w	3000a3f8 <rxi316_ftend_init>
3000a4aa:	bf00      	nop
3000a4ac:	3000e378 	.word	0x3000e378

3000a4b0 <SWR_Calib_Check>:
3000a4b0:	2300      	movs	r3, #0
3000a4b2:	b507      	push	{r0, r1, r2, lr}
3000a4b4:	3801      	subs	r0, #1
3000a4b6:	f88d 3007 	strb.w	r3, [sp, #7]
3000a4ba:	4b0c      	ldr	r3, [pc, #48]	; (3000a4ec <SWR_Calib_Check+0x3c>)
3000a4bc:	f10d 0107 	add.w	r1, sp, #7
3000a4c0:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000a4c4:	f7fd fc82 	bl	30007dcc <OTP_Read8>
3000a4c8:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a4cc:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000a4d0:	2af0      	cmp	r2, #240	; 0xf0
3000a4d2:	d108      	bne.n	3000a4e6 <SWR_Calib_Check+0x36>
3000a4d4:	f003 030f 	and.w	r3, r3, #15
3000a4d8:	2b0f      	cmp	r3, #15
3000a4da:	bf14      	ite	ne
3000a4dc:	2002      	movne	r0, #2
3000a4de:	2001      	moveq	r0, #1
3000a4e0:	b003      	add	sp, #12
3000a4e2:	f85d fb04 	ldr.w	pc, [sp], #4
3000a4e6:	2003      	movs	r0, #3
3000a4e8:	e7fa      	b.n	3000a4e0 <SWR_Calib_Check+0x30>
3000a4ea:	bf00      	nop
3000a4ec:	3000bcdc 	.word	0x3000bcdc

3000a4f0 <SWR_Calib_DCore>:
3000a4f0:	4b10      	ldr	r3, [pc, #64]	; (3000a534 <SWR_Calib_DCore+0x44>)
3000a4f2:	6d1a      	ldr	r2, [r3, #80]	; 0x50
3000a4f4:	f422 724c 	bic.w	r2, r2, #816	; 0x330
3000a4f8:	f022 0203 	bic.w	r2, r2, #3
3000a4fc:	f442 724c 	orr.w	r2, r2, #816	; 0x330
3000a500:	f042 0201 	orr.w	r2, r2, #1
3000a504:	651a      	str	r2, [r3, #80]	; 0x50
3000a506:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000a508:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000a50c:	641a      	str	r2, [r3, #64]	; 0x40
3000a50e:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000a512:	f042 0201 	orr.w	r2, r2, #1
3000a516:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000a51a:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000a51e:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000a522:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000a526:	685a      	ldr	r2, [r3, #4]
3000a528:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000a52c:	f042 0204 	orr.w	r2, r2, #4
3000a530:	605a      	str	r2, [r3, #4]
3000a532:	4770      	bx	lr
3000a534:	42008100 	.word	0x42008100

3000a538 <SWR_Calib_MEM>:
3000a538:	4a3c      	ldr	r2, [pc, #240]	; (3000a62c <SWR_Calib_MEM+0xf4>)
3000a53a:	b513      	push	{r0, r1, r4, lr}
3000a53c:	f8d2 30e0 	ldr.w	r3, [r2, #224]	; 0xe0
3000a540:	2002      	movs	r0, #2
3000a542:	4c3b      	ldr	r4, [pc, #236]	; (3000a630 <SWR_Calib_MEM+0xf8>)
3000a544:	f423 734c 	bic.w	r3, r3, #816	; 0x330
3000a548:	f023 0303 	bic.w	r3, r3, #3
3000a54c:	f443 734c 	orr.w	r3, r3, #816	; 0x330
3000a550:	f043 0301 	orr.w	r3, r3, #1
3000a554:	f8c2 30e0 	str.w	r3, [r2, #224]	; 0xe0
3000a558:	f8d2 30d0 	ldr.w	r3, [r2, #208]	; 0xd0
3000a55c:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000a560:	f8c2 30d0 	str.w	r3, [r2, #208]	; 0xd0
3000a564:	f8d2 30f8 	ldr.w	r3, [r2, #248]	; 0xf8
3000a568:	f043 0301 	orr.w	r3, r3, #1
3000a56c:	f8c2 30f8 	str.w	r3, [r2, #248]	; 0xf8
3000a570:	f8d2 30fc 	ldr.w	r3, [r2, #252]	; 0xfc
3000a574:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a578:	f8c2 30fc 	str.w	r3, [r2, #252]	; 0xfc
3000a57c:	f7ff ff98 	bl	3000a4b0 <SWR_Calib_Check>
3000a580:	b2c0      	uxtb	r0, r0
3000a582:	2801      	cmp	r0, #1
3000a584:	7020      	strb	r0, [r4, #0]
3000a586:	d025      	beq.n	3000a5d4 <SWR_Calib_MEM+0x9c>
3000a588:	f10d 0106 	add.w	r1, sp, #6
3000a58c:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000a590:	f7fd fc1c 	bl	30007dcc <OTP_Read8>
3000a594:	7822      	ldrb	r2, [r4, #0]
3000a596:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000a59a:	2a03      	cmp	r2, #3
3000a59c:	d139      	bne.n	3000a612 <SWR_Calib_MEM+0xda>
3000a59e:	091b      	lsrs	r3, r3, #4
3000a5a0:	4a22      	ldr	r2, [pc, #136]	; (3000a62c <SWR_Calib_MEM+0xf4>)
3000a5a2:	f8d2 10d0 	ldr.w	r1, [r2, #208]	; 0xd0
3000a5a6:	f021 010f 	bic.w	r1, r1, #15
3000a5aa:	430b      	orrs	r3, r1
3000a5ac:	4921      	ldr	r1, [pc, #132]	; (3000a634 <SWR_Calib_MEM+0xfc>)
3000a5ae:	f8c2 30d0 	str.w	r3, [r2, #208]	; 0xd0
3000a5b2:	6c8b      	ldr	r3, [r1, #72]	; 0x48
3000a5b4:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000a5b8:	2b07      	cmp	r3, #7
3000a5ba:	d10b      	bne.n	3000a5d4 <SWR_Calib_MEM+0x9c>
3000a5bc:	6c8b      	ldr	r3, [r1, #72]	; 0x48
3000a5be:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000a5c2:	bf01      	itttt	eq
3000a5c4:	f8d2 30e4 	ldreq.w	r3, [r2, #228]	; 0xe4
3000a5c8:	f423 73e0 	biceq.w	r3, r3, #448	; 0x1c0
3000a5cc:	f443 6360 	orreq.w	r3, r3, #3584	; 0xe00
3000a5d0:	f8c2 30e4 	streq.w	r3, [r2, #228]	; 0xe4
3000a5d4:	2003      	movs	r0, #3
3000a5d6:	4c18      	ldr	r4, [pc, #96]	; (3000a638 <SWR_Calib_MEM+0x100>)
3000a5d8:	f7ff ff6a 	bl	3000a4b0 <SWR_Calib_Check>
3000a5dc:	b2c0      	uxtb	r0, r0
3000a5de:	2801      	cmp	r0, #1
3000a5e0:	7020      	strb	r0, [r4, #0]
3000a5e2:	d014      	beq.n	3000a60e <SWR_Calib_MEM+0xd6>
3000a5e4:	f10d 0107 	add.w	r1, sp, #7
3000a5e8:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000a5ec:	f7fd fbee 	bl	30007dcc <OTP_Read8>
3000a5f0:	7822      	ldrb	r2, [r4, #0]
3000a5f2:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a5f6:	2a03      	cmp	r2, #3
3000a5f8:	d111      	bne.n	3000a61e <SWR_Calib_MEM+0xe6>
3000a5fa:	091b      	lsrs	r3, r3, #4
3000a5fc:	490b      	ldr	r1, [pc, #44]	; (3000a62c <SWR_Calib_MEM+0xf4>)
3000a5fe:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000a602:	f022 7270 	bic.w	r2, r2, #62914560	; 0x3c00000
3000a606:	ea42 5383 	orr.w	r3, r2, r3, lsl #22
3000a60a:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000a60e:	b002      	add	sp, #8
3000a610:	bd10      	pop	{r4, pc}
3000a612:	2a02      	cmp	r2, #2
3000a614:	bf0c      	ite	eq
3000a616:	f003 030f 	andeq.w	r3, r3, #15
3000a61a:	2300      	movne	r3, #0
3000a61c:	e7c0      	b.n	3000a5a0 <SWR_Calib_MEM+0x68>
3000a61e:	2a02      	cmp	r2, #2
3000a620:	bf0c      	ite	eq
3000a622:	f003 030f 	andeq.w	r3, r3, #15
3000a626:	2300      	movne	r3, #0
3000a628:	e7e8      	b.n	3000a5fc <SWR_Calib_MEM+0xc4>
3000a62a:	bf00      	nop
3000a62c:	42008800 	.word	0x42008800
3000a630:	3000e513 	.word	0x3000e513
3000a634:	42008100 	.word	0x42008100
3000a638:	3000e512 	.word	0x3000e512

3000a63c <SWR_Calib_AUD>:
3000a63c:	4a33      	ldr	r2, [pc, #204]	; (3000a70c <SWR_Calib_AUD+0xd0>)
3000a63e:	b513      	push	{r0, r1, r4, lr}
3000a640:	f8d2 30a0 	ldr.w	r3, [r2, #160]	; 0xa0
3000a644:	2004      	movs	r0, #4
3000a646:	4c32      	ldr	r4, [pc, #200]	; (3000a710 <SWR_Calib_AUD+0xd4>)
3000a648:	f423 734c 	bic.w	r3, r3, #816	; 0x330
3000a64c:	f023 0303 	bic.w	r3, r3, #3
3000a650:	f443 734c 	orr.w	r3, r3, #816	; 0x330
3000a654:	f043 0301 	orr.w	r3, r3, #1
3000a658:	f8c2 30a0 	str.w	r3, [r2, #160]	; 0xa0
3000a65c:	f8d2 3090 	ldr.w	r3, [r2, #144]	; 0x90
3000a660:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000a664:	f8c2 3090 	str.w	r3, [r2, #144]	; 0x90
3000a668:	f8d2 30b8 	ldr.w	r3, [r2, #184]	; 0xb8
3000a66c:	f043 0301 	orr.w	r3, r3, #1
3000a670:	f8c2 30b8 	str.w	r3, [r2, #184]	; 0xb8
3000a674:	f8d2 30bc 	ldr.w	r3, [r2, #188]	; 0xbc
3000a678:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a67c:	f8c2 30bc 	str.w	r3, [r2, #188]	; 0xbc
3000a680:	f7ff ff16 	bl	3000a4b0 <SWR_Calib_Check>
3000a684:	b2c0      	uxtb	r0, r0
3000a686:	2801      	cmp	r0, #1
3000a688:	7020      	strb	r0, [r4, #0]
3000a68a:	d013      	beq.n	3000a6b4 <SWR_Calib_AUD+0x78>
3000a68c:	f10d 0106 	add.w	r1, sp, #6
3000a690:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000a694:	f7fd fb9a 	bl	30007dcc <OTP_Read8>
3000a698:	7822      	ldrb	r2, [r4, #0]
3000a69a:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000a69e:	2a03      	cmp	r2, #3
3000a6a0:	d127      	bne.n	3000a6f2 <SWR_Calib_AUD+0xb6>
3000a6a2:	091b      	lsrs	r3, r3, #4
3000a6a4:	4919      	ldr	r1, [pc, #100]	; (3000a70c <SWR_Calib_AUD+0xd0>)
3000a6a6:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000a6aa:	f022 020f 	bic.w	r2, r2, #15
3000a6ae:	4313      	orrs	r3, r2
3000a6b0:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000a6b4:	2005      	movs	r0, #5
3000a6b6:	4c17      	ldr	r4, [pc, #92]	; (3000a714 <SWR_Calib_AUD+0xd8>)
3000a6b8:	f7ff fefa 	bl	3000a4b0 <SWR_Calib_Check>
3000a6bc:	b2c0      	uxtb	r0, r0
3000a6be:	2801      	cmp	r0, #1
3000a6c0:	7020      	strb	r0, [r4, #0]
3000a6c2:	d014      	beq.n	3000a6ee <SWR_Calib_AUD+0xb2>
3000a6c4:	f10d 0107 	add.w	r1, sp, #7
3000a6c8:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000a6cc:	f7fd fb7e 	bl	30007dcc <OTP_Read8>
3000a6d0:	7822      	ldrb	r2, [r4, #0]
3000a6d2:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a6d6:	2a03      	cmp	r2, #3
3000a6d8:	d111      	bne.n	3000a6fe <SWR_Calib_AUD+0xc2>
3000a6da:	091b      	lsrs	r3, r3, #4
3000a6dc:	490b      	ldr	r1, [pc, #44]	; (3000a70c <SWR_Calib_AUD+0xd0>)
3000a6de:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000a6e2:	f022 7270 	bic.w	r2, r2, #62914560	; 0x3c00000
3000a6e6:	ea42 5383 	orr.w	r3, r2, r3, lsl #22
3000a6ea:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000a6ee:	b002      	add	sp, #8
3000a6f0:	bd10      	pop	{r4, pc}
3000a6f2:	2a02      	cmp	r2, #2
3000a6f4:	bf0c      	ite	eq
3000a6f6:	f003 030f 	andeq.w	r3, r3, #15
3000a6fa:	2300      	movne	r3, #0
3000a6fc:	e7d2      	b.n	3000a6a4 <SWR_Calib_AUD+0x68>
3000a6fe:	2a02      	cmp	r2, #2
3000a700:	bf0c      	ite	eq
3000a702:	f003 030f 	andeq.w	r3, r3, #15
3000a706:	2300      	movne	r3, #0
3000a708:	e7e8      	b.n	3000a6dc <SWR_Calib_AUD+0xa0>
3000a70a:	bf00      	nop
3000a70c:	42008800 	.word	0x42008800
3000a710:	3000e511 	.word	0x3000e511
3000a714:	3000e510 	.word	0x3000e510

3000a718 <SWR_MEM>:
3000a718:	2801      	cmp	r0, #1
3000a71a:	b538      	push	{r3, r4, r5, lr}
3000a71c:	4605      	mov	r5, r0
3000a71e:	4c2f      	ldr	r4, [pc, #188]	; (3000a7dc <SWR_MEM+0xc4>)
3000a720:	d14e      	bne.n	3000a7c0 <SWR_MEM+0xa8>
3000a722:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000a726:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000a72a:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a72e:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000a732:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a736:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000a73a:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a73e:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000a742:	f023 0303 	bic.w	r3, r3, #3
3000a746:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000a74a:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a74e:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000a752:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a756:	4b22      	ldr	r3, [pc, #136]	; (3000a7e0 <SWR_MEM+0xc8>)
3000a758:	4798      	blx	r3
3000a75a:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000a75e:	2803      	cmp	r0, #3
3000a760:	bf13      	iteet	ne
3000a762:	f043 0303 	orrne.w	r3, r3, #3
3000a766:	f8c4 30d8 	streq.w	r3, [r4, #216]	; 0xd8
3000a76a:	f8d4 30d0 	ldreq.w	r3, [r4, #208]	; 0xd0
3000a76e:	f8c4 30d8 	strne.w	r3, [r4, #216]	; 0xd8
3000a772:	bf16      	itet	ne
3000a774:	f8d4 30d0 	ldrne.w	r3, [r4, #208]	; 0xd0
3000a778:	f443 0340 	orreq.w	r3, r3, #12582912	; 0xc00000
3000a77c:	f043 7340 	orrne.w	r3, r3, #50331648	; 0x3000000
3000a780:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a784:	4c15      	ldr	r4, [pc, #84]	; (3000a7dc <SWR_MEM+0xc4>)
3000a786:	4b17      	ldr	r3, [pc, #92]	; (3000a7e4 <SWR_MEM+0xcc>)
3000a788:	4798      	blx	r3
3000a78a:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a78e:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000a792:	f043 0302 	orr.w	r3, r3, #2
3000a796:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a79a:	4b13      	ldr	r3, [pc, #76]	; (3000a7e8 <SWR_MEM+0xd0>)
3000a79c:	4798      	blx	r3
3000a79e:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a7a2:	f043 0301 	orr.w	r3, r3, #1
3000a7a6:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a7aa:	4b10      	ldr	r3, [pc, #64]	; (3000a7ec <SWR_MEM+0xd4>)
3000a7ac:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000a7b0:	03d2      	lsls	r2, r2, #15
3000a7b2:	d501      	bpl.n	3000a7b8 <SWR_MEM+0xa0>
3000a7b4:	2500      	movs	r5, #0
3000a7b6:	e001      	b.n	3000a7bc <SWR_MEM+0xa4>
3000a7b8:	3b01      	subs	r3, #1
3000a7ba:	d1f7      	bne.n	3000a7ac <SWR_MEM+0x94>
3000a7bc:	4628      	mov	r0, r5
3000a7be:	bd38      	pop	{r3, r4, r5, pc}
3000a7c0:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a7c4:	f023 0301 	bic.w	r3, r3, #1
3000a7c8:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a7cc:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a7d0:	f023 0302 	bic.w	r3, r3, #2
3000a7d4:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a7d8:	e7ec      	b.n	3000a7b4 <SWR_MEM+0x9c>
3000a7da:	bf00      	nop
3000a7dc:	42008800 	.word	0x42008800
3000a7e0:	300072e1 	.word	0x300072e1
3000a7e4:	3000a539 	.word	0x3000a539
3000a7e8:	00009b2d 	.word	0x00009b2d
3000a7ec:	000186a0 	.word	0x000186a0

3000a7f0 <SWR_MEM_Manual>:
3000a7f0:	4b05      	ldr	r3, [pc, #20]	; (3000a808 <SWR_MEM_Manual+0x18>)
3000a7f2:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000a7f6:	b120      	cbz	r0, 3000a802 <SWR_MEM_Manual+0x12>
3000a7f8:	f442 2200 	orr.w	r2, r2, #524288	; 0x80000
3000a7fc:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000a800:	4770      	bx	lr
3000a802:	f422 2200 	bic.w	r2, r2, #524288	; 0x80000
3000a806:	e7f9      	b.n	3000a7fc <SWR_MEM_Manual+0xc>
3000a808:	42008800 	.word	0x42008800

3000a80c <SWR_MEM_Mode_Get>:
3000a80c:	4b02      	ldr	r3, [pc, #8]	; (3000a818 <SWR_MEM_Mode_Get+0xc>)
3000a80e:	f8d3 00c0 	ldr.w	r0, [r3, #192]	; 0xc0
3000a812:	f3c0 4080 	ubfx	r0, r0, #18, #1
3000a816:	4770      	bx	lr
3000a818:	42008800 	.word	0x42008800

3000a81c <SWR_MEM_Mode_Set>:
3000a81c:	2801      	cmp	r0, #1
3000a81e:	b538      	push	{r3, r4, r5, lr}
3000a820:	4c1f      	ldr	r4, [pc, #124]	; (3000a8a0 <SWR_MEM_Mode_Set+0x84>)
3000a822:	d915      	bls.n	3000a850 <SWR_MEM_Mode_Set+0x34>
3000a824:	217e      	movs	r1, #126	; 0x7e
3000a826:	481f      	ldr	r0, [pc, #124]	; (3000a8a4 <SWR_MEM_Mode_Set+0x88>)
3000a828:	f000 fe0a 	bl	3000b440 <__io_assert_failed_veneer>
3000a82c:	47a0      	blx	r4
3000a82e:	b198      	cbz	r0, 3000a858 <SWR_MEM_Mode_Set+0x3c>
3000a830:	4b1d      	ldr	r3, [pc, #116]	; (3000a8a8 <SWR_MEM_Mode_Set+0x8c>)
3000a832:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000a836:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000a83a:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000a83e:	f242 7210 	movw	r2, #10000	; 0x2710
3000a842:	f8d3 10c0 	ldr.w	r1, [r3, #192]	; 0xc0
3000a846:	0349      	lsls	r1, r1, #13
3000a848:	d506      	bpl.n	3000a858 <SWR_MEM_Mode_Set+0x3c>
3000a84a:	3a01      	subs	r2, #1
3000a84c:	d1f9      	bne.n	3000a842 <SWR_MEM_Mode_Set+0x26>
3000a84e:	e025      	b.n	3000a89c <SWR_MEM_Mode_Set+0x80>
3000a850:	d1ec      	bne.n	3000a82c <SWR_MEM_Mode_Set+0x10>
3000a852:	47a0      	blx	r4
3000a854:	2801      	cmp	r0, #1
3000a856:	d101      	bne.n	3000a85c <SWR_MEM_Mode_Set+0x40>
3000a858:	2000      	movs	r0, #0
3000a85a:	bd38      	pop	{r3, r4, r5, pc}
3000a85c:	4c12      	ldr	r4, [pc, #72]	; (3000a8a8 <SWR_MEM_Mode_Set+0x8c>)
3000a85e:	2064      	movs	r0, #100	; 0x64
3000a860:	4d12      	ldr	r5, [pc, #72]	; (3000a8ac <SWR_MEM_Mode_Set+0x90>)
3000a862:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000a866:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000a86a:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000a86e:	47a8      	blx	r5
3000a870:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a874:	2064      	movs	r0, #100	; 0x64
3000a876:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000a87a:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a87e:	47a8      	blx	r5
3000a880:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000a884:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000a888:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000a88c:	f242 7310 	movw	r3, #10000	; 0x2710
3000a890:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000a894:	0350      	lsls	r0, r2, #13
3000a896:	d4df      	bmi.n	3000a858 <SWR_MEM_Mode_Set+0x3c>
3000a898:	3b01      	subs	r3, #1
3000a89a:	d1f9      	bne.n	3000a890 <SWR_MEM_Mode_Set+0x74>
3000a89c:	2001      	movs	r0, #1
3000a89e:	e7dc      	b.n	3000a85a <SWR_MEM_Mode_Set+0x3e>
3000a8a0:	3000a80d 	.word	0x3000a80d
3000a8a4:	3000bcf0 	.word	0x3000bcf0
3000a8a8:	42008800 	.word	0x42008800
3000a8ac:	00009b2d 	.word	0x00009b2d

3000a8b0 <SWR_AUDIO>:
3000a8b0:	2801      	cmp	r0, #1
3000a8b2:	b538      	push	{r3, r4, r5, lr}
3000a8b4:	4605      	mov	r5, r0
3000a8b6:	4c1d      	ldr	r4, [pc, #116]	; (3000a92c <SWR_AUDIO+0x7c>)
3000a8b8:	d12a      	bne.n	3000a910 <SWR_AUDIO+0x60>
3000a8ba:	4b1d      	ldr	r3, [pc, #116]	; (3000a930 <SWR_AUDIO+0x80>)
3000a8bc:	4798      	blx	r3
3000a8be:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000a8c2:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000a8c6:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000a8ca:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a8ce:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000a8d2:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000a8d6:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000a8da:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000a8de:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a8e2:	f043 0302 	orr.w	r3, r3, #2
3000a8e6:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a8ea:	4b12      	ldr	r3, [pc, #72]	; (3000a934 <SWR_AUDIO+0x84>)
3000a8ec:	4798      	blx	r3
3000a8ee:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a8f2:	f043 0301 	orr.w	r3, r3, #1
3000a8f6:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a8fa:	4b0f      	ldr	r3, [pc, #60]	; (3000a938 <SWR_AUDIO+0x88>)
3000a8fc:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000a900:	03d2      	lsls	r2, r2, #15
3000a902:	d501      	bpl.n	3000a908 <SWR_AUDIO+0x58>
3000a904:	2500      	movs	r5, #0
3000a906:	e001      	b.n	3000a90c <SWR_AUDIO+0x5c>
3000a908:	3b01      	subs	r3, #1
3000a90a:	d1f7      	bne.n	3000a8fc <SWR_AUDIO+0x4c>
3000a90c:	4628      	mov	r0, r5
3000a90e:	bd38      	pop	{r3, r4, r5, pc}
3000a910:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a914:	f023 0301 	bic.w	r3, r3, #1
3000a918:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a91c:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a920:	f023 0302 	bic.w	r3, r3, #2
3000a924:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a928:	e7ec      	b.n	3000a904 <SWR_AUDIO+0x54>
3000a92a:	bf00      	nop
3000a92c:	42008800 	.word	0x42008800
3000a930:	3000a63d 	.word	0x3000a63d
3000a934:	00009b2d 	.word	0x00009b2d
3000a938:	000186a0 	.word	0x000186a0

3000a93c <SWR_AUDIO_Manual>:
3000a93c:	4b05      	ldr	r3, [pc, #20]	; (3000a954 <SWR_AUDIO_Manual+0x18>)
3000a93e:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000a942:	b120      	cbz	r0, 3000a94e <SWR_AUDIO_Manual+0x12>
3000a944:	f442 2200 	orr.w	r2, r2, #524288	; 0x80000
3000a948:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000a94c:	4770      	bx	lr
3000a94e:	f422 2200 	bic.w	r2, r2, #524288	; 0x80000
3000a952:	e7f9      	b.n	3000a948 <SWR_AUDIO_Manual+0xc>
3000a954:	42008800 	.word	0x42008800

3000a958 <SWR_BST_MODE_Set>:
3000a958:	4a05      	ldr	r2, [pc, #20]	; (3000a970 <SWR_BST_MODE_Set+0x18>)
3000a95a:	6813      	ldr	r3, [r2, #0]
3000a95c:	b118      	cbz	r0, 3000a966 <SWR_BST_MODE_Set+0xe>
3000a95e:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000a962:	6013      	str	r3, [r2, #0]
3000a964:	4770      	bx	lr
3000a966:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000a96a:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000a96e:	e7f8      	b.n	3000a962 <SWR_BST_MODE_Set+0xa>
3000a970:	42008100 	.word	0x42008100

3000a974 <crash_dump_memory>:
3000a974:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a978:	f1a1 0380 	sub.w	r3, r1, #128	; 0x80
3000a97c:	4606      	mov	r6, r0
3000a97e:	460c      	mov	r4, r1
3000a980:	481a      	ldr	r0, [pc, #104]	; (3000a9ec <crash_dump_memory+0x78>)
3000a982:	f5b3 5f7e 	cmp.w	r3, #16256	; 0x3f80
3000a986:	4b1a      	ldr	r3, [pc, #104]	; (3000a9f0 <crash_dump_memory+0x7c>)
3000a988:	491a      	ldr	r1, [pc, #104]	; (3000a9f4 <crash_dump_memory+0x80>)
3000a98a:	4637      	mov	r7, r6
3000a98c:	f8d3 327c 	ldr.w	r3, [r3, #636]	; 0x27c
3000a990:	bf88      	it	hi
3000a992:	2480      	movhi	r4, #128	; 0x80
3000a994:	2500      	movs	r5, #0
3000a996:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000aa04 <crash_dump_memory+0x90>
3000a99a:	f013 0fc0 	tst.w	r3, #192	; 0xc0
3000a99e:	4b16      	ldr	r3, [pc, #88]	; (3000a9f8 <crash_dump_memory+0x84>)
3000a9a0:	f8df b064 	ldr.w	fp, [pc, #100]	; 3000aa08 <crash_dump_memory+0x94>
3000a9a4:	bf08      	it	eq
3000a9a6:	4619      	moveq	r1, r3
3000a9a8:	f8df 8060 	ldr.w	r8, [pc, #96]	; 3000aa0c <crash_dump_memory+0x98>
3000a9ac:	f000 fd0c 	bl	3000b3c8 <__DiagPrintf_veneer>
3000a9b0:	4622      	mov	r2, r4
3000a9b2:	4631      	mov	r1, r6
3000a9b4:	4811      	ldr	r0, [pc, #68]	; (3000a9fc <crash_dump_memory+0x88>)
3000a9b6:	f000 fd07 	bl	3000b3c8 <__DiagPrintf_veneer>
3000a9ba:	46b9      	mov	r9, r7
3000a9bc:	b98d      	cbnz	r5, 3000a9e2 <crash_dump_memory+0x6e>
3000a9be:	4631      	mov	r1, r6
3000a9c0:	4658      	mov	r0, fp
3000a9c2:	f000 fd01 	bl	3000b3c8 <__DiagPrintf_veneer>
3000a9c6:	3501      	adds	r5, #1
3000a9c8:	f8d9 1000 	ldr.w	r1, [r9]
3000a9cc:	4640      	mov	r0, r8
3000a9ce:	3704      	adds	r7, #4
3000a9d0:	f000 fcfa 	bl	3000b3c8 <__DiagPrintf_veneer>
3000a9d4:	42ac      	cmp	r4, r5
3000a9d6:	d1f0      	bne.n	3000a9ba <crash_dump_memory+0x46>
3000a9d8:	4809      	ldr	r0, [pc, #36]	; (3000aa00 <crash_dump_memory+0x8c>)
3000a9da:	e8bd 4ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a9de:	f000 bcf3 	b.w	3000b3c8 <__DiagPrintf_veneer>
3000a9e2:	07ab      	lsls	r3, r5, #30
3000a9e4:	d1ef      	bne.n	3000a9c6 <crash_dump_memory+0x52>
3000a9e6:	4639      	mov	r1, r7
3000a9e8:	4650      	mov	r0, sl
3000a9ea:	e7ea      	b.n	3000a9c2 <crash_dump_memory+0x4e>
3000a9ec:	3000bd09 	.word	0x3000bd09
3000a9f0:	42008000 	.word	0x42008000
3000a9f4:	3000bd01 	.word	0x3000bd01
3000a9f8:	3000bd05 	.word	0x3000bd05
3000a9fc:	3000bd2d 	.word	0x3000bd2d
3000aa00:	3000be33 	.word	0x3000be33
3000aa04:	3000bd67 	.word	0x3000bd67
3000aa08:	3000bd69 	.word	0x3000bd69
3000aa0c:	3000bd71 	.word	0x3000bd71

3000aa10 <crash_dump>:
3000aa10:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000aa14:	4c48      	ldr	r4, [pc, #288]	; (3000ab38 <crash_dump+0x128>)
3000aa16:	4615      	mov	r5, r2
3000aa18:	6a53      	ldr	r3, [r2, #36]	; 0x24
3000aa1a:	460e      	mov	r6, r1
3000aa1c:	4847      	ldr	r0, [pc, #284]	; (3000ab3c <crash_dump+0x12c>)
3000aa1e:	6023      	str	r3, [r4, #0]
3000aa20:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000aa22:	4f47      	ldr	r7, [pc, #284]	; (3000ab40 <crash_dump+0x130>)
3000aa24:	6063      	str	r3, [r4, #4]
3000aa26:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000aa28:	f8df 8134 	ldr.w	r8, [pc, #308]	; 3000ab60 <crash_dump+0x150>
3000aa2c:	60a3      	str	r3, [r4, #8]
3000aa2e:	6b13      	ldr	r3, [r2, #48]	; 0x30
3000aa30:	60e3      	str	r3, [r4, #12]
3000aa32:	6853      	ldr	r3, [r2, #4]
3000aa34:	6123      	str	r3, [r4, #16]
3000aa36:	6893      	ldr	r3, [r2, #8]
3000aa38:	6163      	str	r3, [r4, #20]
3000aa3a:	68d3      	ldr	r3, [r2, #12]
3000aa3c:	61a3      	str	r3, [r4, #24]
3000aa3e:	6913      	ldr	r3, [r2, #16]
3000aa40:	61e3      	str	r3, [r4, #28]
3000aa42:	6953      	ldr	r3, [r2, #20]
3000aa44:	6223      	str	r3, [r4, #32]
3000aa46:	6993      	ldr	r3, [r2, #24]
3000aa48:	6263      	str	r3, [r4, #36]	; 0x24
3000aa4a:	69d3      	ldr	r3, [r2, #28]
3000aa4c:	62a3      	str	r3, [r4, #40]	; 0x28
3000aa4e:	6a13      	ldr	r3, [r2, #32]
3000aa50:	62e3      	str	r3, [r4, #44]	; 0x2c
3000aa52:	6b53      	ldr	r3, [r2, #52]	; 0x34
3000aa54:	6323      	str	r3, [r4, #48]	; 0x30
3000aa56:	6b93      	ldr	r3, [r2, #56]	; 0x38
3000aa58:	6363      	str	r3, [r4, #52]	; 0x34
3000aa5a:	6bd3      	ldr	r3, [r2, #60]	; 0x3c
3000aa5c:	63a3      	str	r3, [r4, #56]	; 0x38
3000aa5e:	6c13      	ldr	r3, [r2, #64]	; 0x40
3000aa60:	63e3      	str	r3, [r4, #60]	; 0x3c
3000aa62:	6813      	ldr	r3, [r2, #0]
3000aa64:	6423      	str	r3, [r4, #64]	; 0x40
3000aa66:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000aa6a:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000aa6e:	6462      	str	r2, [r4, #68]	; 0x44
3000aa70:	4a34      	ldr	r2, [pc, #208]	; (3000ab44 <crash_dump+0x134>)
3000aa72:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000aa76:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000aa7a:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000aa7e:	64e1      	str	r1, [r4, #76]	; 0x4c
3000aa80:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000aa84:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000aa88:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000aa8c:	6561      	str	r1, [r4, #84]	; 0x54
3000aa8e:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000aa90:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000aa94:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000aa98:	65e1      	str	r1, [r4, #92]	; 0x5c
3000aa9a:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000aa9c:	b289      	uxth	r1, r1
3000aa9e:	6621      	str	r1, [r4, #96]	; 0x60
3000aaa0:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000aaa2:	b292      	uxth	r2, r2
3000aaa4:	6662      	str	r2, [r4, #100]	; 0x64
3000aaa6:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000aaaa:	66a2      	str	r2, [r4, #104]	; 0x68
3000aaac:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000aab0:	66e3      	str	r3, [r4, #108]	; 0x6c
3000aab2:	f000 fc89 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aab6:	4620      	mov	r0, r4
3000aab8:	2409      	movs	r4, #9
3000aaba:	f000 f857 	bl	3000ab6c <fault_diagnosis>
3000aabe:	4822      	ldr	r0, [pc, #136]	; (3000ab48 <crash_dump+0x138>)
3000aac0:	f000 fc82 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aac4:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000aac6:	4821      	ldr	r0, [pc, #132]	; (3000ab4c <crash_dump+0x13c>)
3000aac8:	f000 fc7e 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aacc:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000aace:	4820      	ldr	r0, [pc, #128]	; (3000ab50 <crash_dump+0x140>)
3000aad0:	f000 fc7a 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aad4:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000aad6:	481f      	ldr	r0, [pc, #124]	; (3000ab54 <crash_dump+0x144>)
3000aad8:	f000 fc76 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aadc:	6829      	ldr	r1, [r5, #0]
3000aade:	481e      	ldr	r0, [pc, #120]	; (3000ab58 <crash_dump+0x148>)
3000aae0:	f000 fc72 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aae4:	f855 2024 	ldr.w	r2, [r5, r4, lsl #2]
3000aae8:	4640      	mov	r0, r8
3000aaea:	f857 1024 	ldr.w	r1, [r7, r4, lsl #2]
3000aaee:	3401      	adds	r4, #1
3000aaf0:	f000 fc6a 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aaf4:	2c0d      	cmp	r4, #13
3000aaf6:	d1f5      	bne.n	3000aae4 <crash_dump+0xd4>
3000aaf8:	2401      	movs	r4, #1
3000aafa:	f8df 8064 	ldr.w	r8, [pc, #100]	; 3000ab60 <crash_dump+0x150>
3000aafe:	f855 2024 	ldr.w	r2, [r5, r4, lsl #2]
3000ab02:	4640      	mov	r0, r8
3000ab04:	f857 1024 	ldr.w	r1, [r7, r4, lsl #2]
3000ab08:	3401      	adds	r4, #1
3000ab0a:	f000 fc5d 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ab0e:	2c09      	cmp	r4, #9
3000ab10:	d1f5      	bne.n	3000aafe <crash_dump+0xee>
3000ab12:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000ab14:	4911      	ldr	r1, [pc, #68]	; (3000ab5c <crash_dump+0x14c>)
3000ab16:	4812      	ldr	r0, [pc, #72]	; (3000ab60 <crash_dump+0x150>)
3000ab18:	f000 fc56 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ab1c:	2180      	movs	r1, #128	; 0x80
3000ab1e:	4630      	mov	r0, r6
3000ab20:	f7ff ff28 	bl	3000a974 <crash_dump_memory>
3000ab24:	480f      	ldr	r0, [pc, #60]	; (3000ab64 <crash_dump+0x154>)
3000ab26:	f000 fc4f 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ab2a:	480f      	ldr	r0, [pc, #60]	; (3000ab68 <crash_dump+0x158>)
3000ab2c:	f000 fc4c 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ab30:	2000      	movs	r0, #0
3000ab32:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000ab36:	bf00      	nop
3000ab38:	3000eb08 	.word	0x3000eb08
3000ab3c:	3000bd77 	.word	0x3000bd77
3000ab40:	3000be5c 	.word	0x3000be5c
3000ab44:	e000ed00 	.word	0xe000ed00
3000ab48:	3000bd99 	.word	0x3000bd99
3000ab4c:	3000bdbe 	.word	0x3000bdbe
3000ab50:	3000bdcc 	.word	0x3000bdcc
3000ab54:	3000bdda 	.word	0x3000bdda
3000ab58:	3000bde8 	.word	0x3000bde8
3000ab5c:	3000be03 	.word	0x3000be03
3000ab60:	3000bdf6 	.word	0x3000bdf6
3000ab64:	3000be07 	.word	0x3000be07
3000ab68:	3000be30 	.word	0x3000be30

3000ab6c <fault_diagnosis>:
3000ab6c:	b510      	push	{r4, lr}
3000ab6e:	4604      	mov	r4, r0
3000ab70:	6b81      	ldr	r1, [r0, #56]	; 0x38
3000ab72:	489d      	ldr	r0, [pc, #628]	; (3000ade8 <fault_diagnosis+0x27c>)
3000ab74:	f000 fc28 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ab78:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000ab7c:	079b      	lsls	r3, r3, #30
3000ab7e:	d504      	bpl.n	3000ab8a <fault_diagnosis+0x1e>
3000ab80:	4a9a      	ldr	r2, [pc, #616]	; (3000adec <fault_diagnosis+0x280>)
3000ab82:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000ab84:	489a      	ldr	r0, [pc, #616]	; (3000adf0 <fault_diagnosis+0x284>)
3000ab86:	f000 fc1f 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ab8a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab8c:	2900      	cmp	r1, #0
3000ab8e:	d049      	beq.n	3000ac24 <fault_diagnosis+0xb8>
3000ab90:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab94:	07d8      	lsls	r0, r3, #31
3000ab96:	d503      	bpl.n	3000aba0 <fault_diagnosis+0x34>
3000ab98:	4a96      	ldr	r2, [pc, #600]	; (3000adf4 <fault_diagnosis+0x288>)
3000ab9a:	4897      	ldr	r0, [pc, #604]	; (3000adf8 <fault_diagnosis+0x28c>)
3000ab9c:	f000 fc14 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aba0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000aba4:	0799      	lsls	r1, r3, #30
3000aba6:	d504      	bpl.n	3000abb2 <fault_diagnosis+0x46>
3000aba8:	4a94      	ldr	r2, [pc, #592]	; (3000adfc <fault_diagnosis+0x290>)
3000abaa:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000abac:	4892      	ldr	r0, [pc, #584]	; (3000adf8 <fault_diagnosis+0x28c>)
3000abae:	f000 fc0b 	bl	3000b3c8 <__DiagPrintf_veneer>
3000abb2:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000abb6:	075a      	lsls	r2, r3, #29
3000abb8:	d504      	bpl.n	3000abc4 <fault_diagnosis+0x58>
3000abba:	4a91      	ldr	r2, [pc, #580]	; (3000ae00 <fault_diagnosis+0x294>)
3000abbc:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000abbe:	488e      	ldr	r0, [pc, #568]	; (3000adf8 <fault_diagnosis+0x28c>)
3000abc0:	f000 fc02 	bl	3000b3c8 <__DiagPrintf_veneer>
3000abc4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000abc8:	071b      	lsls	r3, r3, #28
3000abca:	d504      	bpl.n	3000abd6 <fault_diagnosis+0x6a>
3000abcc:	4a8d      	ldr	r2, [pc, #564]	; (3000ae04 <fault_diagnosis+0x298>)
3000abce:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000abd0:	4889      	ldr	r0, [pc, #548]	; (3000adf8 <fault_diagnosis+0x28c>)
3000abd2:	f000 fbf9 	bl	3000b3c8 <__DiagPrintf_veneer>
3000abd6:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000abda:	06d8      	lsls	r0, r3, #27
3000abdc:	d504      	bpl.n	3000abe8 <fault_diagnosis+0x7c>
3000abde:	4a8a      	ldr	r2, [pc, #552]	; (3000ae08 <fault_diagnosis+0x29c>)
3000abe0:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000abe2:	4885      	ldr	r0, [pc, #532]	; (3000adf8 <fault_diagnosis+0x28c>)
3000abe4:	f000 fbf0 	bl	3000b3c8 <__DiagPrintf_veneer>
3000abe8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000abec:	0699      	lsls	r1, r3, #26
3000abee:	d504      	bpl.n	3000abfa <fault_diagnosis+0x8e>
3000abf0:	4a86      	ldr	r2, [pc, #536]	; (3000ae0c <fault_diagnosis+0x2a0>)
3000abf2:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000abf4:	4880      	ldr	r0, [pc, #512]	; (3000adf8 <fault_diagnosis+0x28c>)
3000abf6:	f000 fbe7 	bl	3000b3c8 <__DiagPrintf_veneer>
3000abfa:	f994 3068 	ldrsb.w	r3, [r4, #104]	; 0x68
3000abfe:	2b00      	cmp	r3, #0
3000ac00:	da04      	bge.n	3000ac0c <fault_diagnosis+0xa0>
3000ac02:	4a83      	ldr	r2, [pc, #524]	; (3000ae10 <fault_diagnosis+0x2a4>)
3000ac04:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ac06:	487c      	ldr	r0, [pc, #496]	; (3000adf8 <fault_diagnosis+0x28c>)
3000ac08:	f000 fbde 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac0c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ac10:	065a      	lsls	r2, r3, #25
3000ac12:	d507      	bpl.n	3000ac24 <fault_diagnosis+0xb8>
3000ac14:	f013 0f28 	tst.w	r3, #40	; 0x28
3000ac18:	d004      	beq.n	3000ac24 <fault_diagnosis+0xb8>
3000ac1a:	4a7e      	ldr	r2, [pc, #504]	; (3000ae14 <fault_diagnosis+0x2a8>)
3000ac1c:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000ac1e:	487e      	ldr	r0, [pc, #504]	; (3000ae18 <fault_diagnosis+0x2ac>)
3000ac20:	f000 fbd2 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac24:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000ac28:	2900      	cmp	r1, #0
3000ac2a:	d032      	beq.n	3000ac92 <fault_diagnosis+0x126>
3000ac2c:	07cb      	lsls	r3, r1, #31
3000ac2e:	d503      	bpl.n	3000ac38 <fault_diagnosis+0xcc>
3000ac30:	4a7a      	ldr	r2, [pc, #488]	; (3000ae1c <fault_diagnosis+0x2b0>)
3000ac32:	487b      	ldr	r0, [pc, #492]	; (3000ae20 <fault_diagnosis+0x2b4>)
3000ac34:	f000 fbc8 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac38:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000ac3c:	0788      	lsls	r0, r1, #30
3000ac3e:	d503      	bpl.n	3000ac48 <fault_diagnosis+0xdc>
3000ac40:	4a78      	ldr	r2, [pc, #480]	; (3000ae24 <fault_diagnosis+0x2b8>)
3000ac42:	4877      	ldr	r0, [pc, #476]	; (3000ae20 <fault_diagnosis+0x2b4>)
3000ac44:	f000 fbc0 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac48:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000ac4c:	070a      	lsls	r2, r1, #28
3000ac4e:	d503      	bpl.n	3000ac58 <fault_diagnosis+0xec>
3000ac50:	4a75      	ldr	r2, [pc, #468]	; (3000ae28 <fault_diagnosis+0x2bc>)
3000ac52:	4873      	ldr	r0, [pc, #460]	; (3000ae20 <fault_diagnosis+0x2b4>)
3000ac54:	f000 fbb8 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac58:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000ac5c:	06cb      	lsls	r3, r1, #27
3000ac5e:	d503      	bpl.n	3000ac68 <fault_diagnosis+0xfc>
3000ac60:	4a72      	ldr	r2, [pc, #456]	; (3000ae2c <fault_diagnosis+0x2c0>)
3000ac62:	486f      	ldr	r0, [pc, #444]	; (3000ae20 <fault_diagnosis+0x2b4>)
3000ac64:	f000 fbb0 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac68:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000ac6c:	0688      	lsls	r0, r1, #26
3000ac6e:	d503      	bpl.n	3000ac78 <fault_diagnosis+0x10c>
3000ac70:	4a6f      	ldr	r2, [pc, #444]	; (3000ae30 <fault_diagnosis+0x2c4>)
3000ac72:	486b      	ldr	r0, [pc, #428]	; (3000ae20 <fault_diagnosis+0x2b4>)
3000ac74:	f000 fba8 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac78:	f994 2048 	ldrsb.w	r2, [r4, #72]	; 0x48
3000ac7c:	f894 3048 	ldrb.w	r3, [r4, #72]	; 0x48
3000ac80:	2a00      	cmp	r2, #0
3000ac82:	da06      	bge.n	3000ac92 <fault_diagnosis+0x126>
3000ac84:	079a      	lsls	r2, r3, #30
3000ac86:	d004      	beq.n	3000ac92 <fault_diagnosis+0x126>
3000ac88:	4a6a      	ldr	r2, [pc, #424]	; (3000ae34 <fault_diagnosis+0x2c8>)
3000ac8a:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000ac8c:	486a      	ldr	r0, [pc, #424]	; (3000ae38 <fault_diagnosis+0x2cc>)
3000ac8e:	f000 fb9b 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ac92:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ac96:	2900      	cmp	r1, #0
3000ac98:	d03a      	beq.n	3000ad10 <fault_diagnosis+0x1a4>
3000ac9a:	07cb      	lsls	r3, r1, #31
3000ac9c:	d503      	bpl.n	3000aca6 <fault_diagnosis+0x13a>
3000ac9e:	4a67      	ldr	r2, [pc, #412]	; (3000ae3c <fault_diagnosis+0x2d0>)
3000aca0:	4867      	ldr	r0, [pc, #412]	; (3000ae40 <fault_diagnosis+0x2d4>)
3000aca2:	f000 fb91 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aca6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000acaa:	0788      	lsls	r0, r1, #30
3000acac:	d503      	bpl.n	3000acb6 <fault_diagnosis+0x14a>
3000acae:	4a65      	ldr	r2, [pc, #404]	; (3000ae44 <fault_diagnosis+0x2d8>)
3000acb0:	4863      	ldr	r0, [pc, #396]	; (3000ae40 <fault_diagnosis+0x2d4>)
3000acb2:	f000 fb89 	bl	3000b3c8 <__DiagPrintf_veneer>
3000acb6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000acba:	074a      	lsls	r2, r1, #29
3000acbc:	d503      	bpl.n	3000acc6 <fault_diagnosis+0x15a>
3000acbe:	4a62      	ldr	r2, [pc, #392]	; (3000ae48 <fault_diagnosis+0x2dc>)
3000acc0:	485f      	ldr	r0, [pc, #380]	; (3000ae40 <fault_diagnosis+0x2d4>)
3000acc2:	f000 fb81 	bl	3000b3c8 <__DiagPrintf_veneer>
3000acc6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000acca:	070b      	lsls	r3, r1, #28
3000accc:	d503      	bpl.n	3000acd6 <fault_diagnosis+0x16a>
3000acce:	4a5f      	ldr	r2, [pc, #380]	; (3000ae4c <fault_diagnosis+0x2e0>)
3000acd0:	485b      	ldr	r0, [pc, #364]	; (3000ae40 <fault_diagnosis+0x2d4>)
3000acd2:	f000 fb79 	bl	3000b3c8 <__DiagPrintf_veneer>
3000acd6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000acda:	06c8      	lsls	r0, r1, #27
3000acdc:	d503      	bpl.n	3000ace6 <fault_diagnosis+0x17a>
3000acde:	4a5c      	ldr	r2, [pc, #368]	; (3000ae50 <fault_diagnosis+0x2e4>)
3000ace0:	4857      	ldr	r0, [pc, #348]	; (3000ae40 <fault_diagnosis+0x2d4>)
3000ace2:	f000 fb71 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ace6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000acea:	068a      	lsls	r2, r1, #26
3000acec:	d503      	bpl.n	3000acf6 <fault_diagnosis+0x18a>
3000acee:	4a59      	ldr	r2, [pc, #356]	; (3000ae54 <fault_diagnosis+0x2e8>)
3000acf0:	4853      	ldr	r0, [pc, #332]	; (3000ae40 <fault_diagnosis+0x2d4>)
3000acf2:	f000 fb69 	bl	3000b3c8 <__DiagPrintf_veneer>
3000acf6:	f994 2050 	ldrsb.w	r2, [r4, #80]	; 0x50
3000acfa:	f894 3050 	ldrb.w	r3, [r4, #80]	; 0x50
3000acfe:	2a00      	cmp	r2, #0
3000ad00:	da06      	bge.n	3000ad10 <fault_diagnosis+0x1a4>
3000ad02:	079b      	lsls	r3, r3, #30
3000ad04:	d504      	bpl.n	3000ad10 <fault_diagnosis+0x1a4>
3000ad06:	4a54      	ldr	r2, [pc, #336]	; (3000ae58 <fault_diagnosis+0x2ec>)
3000ad08:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000ad0a:	4854      	ldr	r0, [pc, #336]	; (3000ae5c <fault_diagnosis+0x2f0>)
3000ad0c:	f000 fb5c 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ad10:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad14:	2900      	cmp	r1, #0
3000ad16:	d043      	beq.n	3000ada0 <fault_diagnosis+0x234>
3000ad18:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000ad1c:	07d8      	lsls	r0, r3, #31
3000ad1e:	d503      	bpl.n	3000ad28 <fault_diagnosis+0x1bc>
3000ad20:	4a4f      	ldr	r2, [pc, #316]	; (3000ae60 <fault_diagnosis+0x2f4>)
3000ad22:	4850      	ldr	r0, [pc, #320]	; (3000ae64 <fault_diagnosis+0x2f8>)
3000ad24:	f000 fb50 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ad28:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000ad2c:	0799      	lsls	r1, r3, #30
3000ad2e:	d505      	bpl.n	3000ad3c <fault_diagnosis+0x1d0>
3000ad30:	4a4d      	ldr	r2, [pc, #308]	; (3000ae68 <fault_diagnosis+0x2fc>)
3000ad32:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad36:	484b      	ldr	r0, [pc, #300]	; (3000ae64 <fault_diagnosis+0x2f8>)
3000ad38:	f000 fb46 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ad3c:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000ad40:	075a      	lsls	r2, r3, #29
3000ad42:	d505      	bpl.n	3000ad50 <fault_diagnosis+0x1e4>
3000ad44:	4a49      	ldr	r2, [pc, #292]	; (3000ae6c <fault_diagnosis+0x300>)
3000ad46:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad4a:	4846      	ldr	r0, [pc, #280]	; (3000ae64 <fault_diagnosis+0x2f8>)
3000ad4c:	f000 fb3c 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ad50:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000ad54:	071b      	lsls	r3, r3, #28
3000ad56:	d505      	bpl.n	3000ad64 <fault_diagnosis+0x1f8>
3000ad58:	4a45      	ldr	r2, [pc, #276]	; (3000ae70 <fault_diagnosis+0x304>)
3000ad5a:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad5e:	4841      	ldr	r0, [pc, #260]	; (3000ae64 <fault_diagnosis+0x2f8>)
3000ad60:	f000 fb32 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ad64:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000ad68:	06d8      	lsls	r0, r3, #27
3000ad6a:	d505      	bpl.n	3000ad78 <fault_diagnosis+0x20c>
3000ad6c:	4a41      	ldr	r2, [pc, #260]	; (3000ae74 <fault_diagnosis+0x308>)
3000ad6e:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad72:	483c      	ldr	r0, [pc, #240]	; (3000ae64 <fault_diagnosis+0x2f8>)
3000ad74:	f000 fb28 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ad78:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000ad7c:	07d9      	lsls	r1, r3, #31
3000ad7e:	d505      	bpl.n	3000ad8c <fault_diagnosis+0x220>
3000ad80:	4a3d      	ldr	r2, [pc, #244]	; (3000ae78 <fault_diagnosis+0x30c>)
3000ad82:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad86:	4837      	ldr	r0, [pc, #220]	; (3000ae64 <fault_diagnosis+0x2f8>)
3000ad88:	f000 fb1e 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ad8c:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000ad90:	079a      	lsls	r2, r3, #30
3000ad92:	d505      	bpl.n	3000ada0 <fault_diagnosis+0x234>
3000ad94:	4a39      	ldr	r2, [pc, #228]	; (3000ae7c <fault_diagnosis+0x310>)
3000ad96:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad9a:	4832      	ldr	r0, [pc, #200]	; (3000ae64 <fault_diagnosis+0x2f8>)
3000ad9c:	f000 fb14 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ada0:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000ada4:	2b00      	cmp	r3, #0
3000ada6:	f280 8087 	bge.w	3000aeb8 <fault_diagnosis+0x34c>
3000adaa:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000adac:	2900      	cmp	r1, #0
3000adae:	f000 8083 	beq.w	3000aeb8 <fault_diagnosis+0x34c>
3000adb2:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000adb6:	07db      	lsls	r3, r3, #31
3000adb8:	d503      	bpl.n	3000adc2 <fault_diagnosis+0x256>
3000adba:	4a31      	ldr	r2, [pc, #196]	; (3000ae80 <fault_diagnosis+0x314>)
3000adbc:	4831      	ldr	r0, [pc, #196]	; (3000ae84 <fault_diagnosis+0x318>)
3000adbe:	f000 fb03 	bl	3000b3c8 <__DiagPrintf_veneer>
3000adc2:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000adc6:	0798      	lsls	r0, r3, #30
3000adc8:	d504      	bpl.n	3000add4 <fault_diagnosis+0x268>
3000adca:	4a2f      	ldr	r2, [pc, #188]	; (3000ae88 <fault_diagnosis+0x31c>)
3000adcc:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000adce:	482d      	ldr	r0, [pc, #180]	; (3000ae84 <fault_diagnosis+0x318>)
3000add0:	f000 fafa 	bl	3000b3c8 <__DiagPrintf_veneer>
3000add4:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000add8:	0759      	lsls	r1, r3, #29
3000adda:	d559      	bpl.n	3000ae90 <fault_diagnosis+0x324>
3000addc:	4a2b      	ldr	r2, [pc, #172]	; (3000ae8c <fault_diagnosis+0x320>)
3000adde:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000ade0:	4828      	ldr	r0, [pc, #160]	; (3000ae84 <fault_diagnosis+0x318>)
3000ade2:	f000 faf1 	bl	3000b3c8 <__DiagPrintf_veneer>
3000ade6:	e053      	b.n	3000ae90 <fault_diagnosis+0x324>
3000ade8:	3000be94 	.word	0x3000be94
3000adec:	3000beac 	.word	0x3000beac
3000adf0:	3000bed8 	.word	0x3000bed8
3000adf4:	3000beed 	.word	0x3000beed
3000adf8:	3000bf1d 	.word	0x3000bf1d
3000adfc:	3000bf32 	.word	0x3000bf32
3000ae00:	3000bf6a 	.word	0x3000bf6a
3000ae04:	3000bf9f 	.word	0x3000bf9f
3000ae08:	3000bfd6 	.word	0x3000bfd6
3000ae0c:	3000c005 	.word	0x3000c005
3000ae10:	3000c03f 	.word	0x3000c03f
3000ae14:	3000c06c 	.word	0x3000c06c
3000ae18:	3000c093 	.word	0x3000c093
3000ae1c:	3000c0a8 	.word	0x3000c0a8
3000ae20:	3000c0ea 	.word	0x3000c0ea
3000ae24:	3000c100 	.word	0x3000c100
3000ae28:	3000c13b 	.word	0x3000c13b
3000ae2c:	3000c171 	.word	0x3000c171
3000ae30:	3000c1a5 	.word	0x3000c1a5
3000ae34:	3000c1f1 	.word	0x3000c1f1
3000ae38:	3000c221 	.word	0x3000c221
3000ae3c:	3000c236 	.word	0x3000c236
3000ae40:	3000c26a 	.word	0x3000c26a
3000ae44:	3000c27f 	.word	0x3000c27f
3000ae48:	3000c2b4 	.word	0x3000c2b4
3000ae4c:	3000c2eb 	.word	0x3000c2eb
3000ae50:	3000c313 	.word	0x3000c313
3000ae54:	3000c339 	.word	0x3000c339
3000ae58:	3000c377 	.word	0x3000c377
3000ae5c:	3000c399 	.word	0x3000c399
3000ae60:	3000c3ae 	.word	0x3000c3ae
3000ae64:	3000c3f4 	.word	0x3000c3f4
3000ae68:	3000c409 	.word	0x3000c409
3000ae6c:	3000c455 	.word	0x3000c455
3000ae70:	3000c4b4 	.word	0x3000c4b4
3000ae74:	3000c4fb 	.word	0x3000c4fb
3000ae78:	3000c555 	.word	0x3000c555
3000ae7c:	3000c5a7 	.word	0x3000c5a7
3000ae80:	3000c611 	.word	0x3000c611
3000ae84:	3000c641 	.word	0x3000c641
3000ae88:	3000c656 	.word	0x3000c656
3000ae8c:	3000c689 	.word	0x3000c689
3000ae90:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000ae94:	071a      	lsls	r2, r3, #28
3000ae96:	d504      	bpl.n	3000aea2 <fault_diagnosis+0x336>
3000ae98:	4a08      	ldr	r2, [pc, #32]	; (3000aebc <fault_diagnosis+0x350>)
3000ae9a:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000ae9c:	4808      	ldr	r0, [pc, #32]	; (3000aec0 <fault_diagnosis+0x354>)
3000ae9e:	f000 fa93 	bl	3000b3c8 <__DiagPrintf_veneer>
3000aea2:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000aea6:	06db      	lsls	r3, r3, #27
3000aea8:	d506      	bpl.n	3000aeb8 <fault_diagnosis+0x34c>
3000aeaa:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000aeac:	4a05      	ldr	r2, [pc, #20]	; (3000aec4 <fault_diagnosis+0x358>)
3000aeae:	4804      	ldr	r0, [pc, #16]	; (3000aec0 <fault_diagnosis+0x354>)
3000aeb0:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000aeb4:	f000 ba88 	b.w	3000b3c8 <__DiagPrintf_veneer>
3000aeb8:	bd10      	pop	{r4, pc}
3000aeba:	bf00      	nop
3000aebc:	3000c6b5 	.word	0x3000c6b5
3000aec0:	3000c641 	.word	0x3000c641
3000aec4:	3000c6e4 	.word	0x3000c6e4

3000aec8 <rtk_log_level_get>:
3000aec8:	4b0f      	ldr	r3, [pc, #60]	; (3000af08 <rtk_log_level_get+0x40>)
3000aeca:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000aece:	681a      	ldr	r2, [r3, #0]
3000aed0:	4606      	mov	r6, r0
3000aed2:	4d0e      	ldr	r5, [pc, #56]	; (3000af0c <rtk_log_level_get+0x44>)
3000aed4:	2400      	movs	r4, #0
3000aed6:	2a02      	cmp	r2, #2
3000aed8:	f8df 803c 	ldr.w	r8, [pc, #60]	; 3000af18 <rtk_log_level_get+0x50>
3000aedc:	bf94      	ite	ls
3000aede:	681f      	ldrls	r7, [r3, #0]
3000aee0:	2703      	movhi	r7, #3
3000aee2:	42bc      	cmp	r4, r7
3000aee4:	d102      	bne.n	3000aeec <rtk_log_level_get+0x24>
3000aee6:	4b0a      	ldr	r3, [pc, #40]	; (3000af10 <rtk_log_level_get+0x48>)
3000aee8:	7818      	ldrb	r0, [r3, #0]
3000aeea:	e008      	b.n	3000aefe <rtk_log_level_get+0x36>
3000aeec:	4628      	mov	r0, r5
3000aeee:	4631      	mov	r1, r6
3000aef0:	350b      	adds	r5, #11
3000aef2:	47c0      	blx	r8
3000aef4:	b928      	cbnz	r0, 3000af02 <rtk_log_level_get+0x3a>
3000aef6:	230b      	movs	r3, #11
3000aef8:	435c      	muls	r4, r3
3000aefa:	4b06      	ldr	r3, [pc, #24]	; (3000af14 <rtk_log_level_get+0x4c>)
3000aefc:	5d18      	ldrb	r0, [r3, r4]
3000aefe:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000af02:	3401      	adds	r4, #1
3000af04:	e7ed      	b.n	3000aee2 <rtk_log_level_get+0x1a>
3000af06:	bf00      	nop
3000af08:	3000ef48 	.word	0x3000ef48
3000af0c:	3000ef4d 	.word	0x3000ef4d
3000af10:	3000e524 	.word	0x3000e524
3000af14:	3000ef4c 	.word	0x3000ef4c
3000af18:	00012c89 	.word	0x00012c89

3000af1c <rtk_log_write>:
3000af1c:	b40c      	push	{r2, r3}
3000af1e:	b513      	push	{r0, r1, r4, lr}
3000af20:	4604      	mov	r4, r0
3000af22:	4608      	mov	r0, r1
3000af24:	f7ff ffd0 	bl	3000aec8 <rtk_log_level_get>
3000af28:	42a0      	cmp	r0, r4
3000af2a:	d305      	bcc.n	3000af38 <rtk_log_write+0x1c>
3000af2c:	aa05      	add	r2, sp, #20
3000af2e:	9904      	ldr	r1, [sp, #16]
3000af30:	2000      	movs	r0, #0
3000af32:	9201      	str	r2, [sp, #4]
3000af34:	f000 fa30 	bl	3000b398 <__DiagVSprintf_veneer>
3000af38:	b002      	add	sp, #8
3000af3a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000af3e:	b002      	add	sp, #8
3000af40:	4770      	bx	lr
	...

3000af44 <BOOT_VerCheck>:
3000af44:	b570      	push	{r4, r5, r6, lr}
3000af46:	4b1d      	ldr	r3, [pc, #116]	; (3000afbc <BOOT_VerCheck+0x78>)
3000af48:	4798      	blx	r3
3000af4a:	2800      	cmp	r0, #0
3000af4c:	d135      	bne.n	3000afba <BOOT_VerCheck+0x76>
3000af4e:	4b1c      	ldr	r3, [pc, #112]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000af50:	2403      	movs	r4, #3
3000af52:	4d1c      	ldr	r5, [pc, #112]	; (3000afc4 <BOOT_VerCheck+0x80>)
3000af54:	2003      	movs	r0, #3
3000af56:	4a1c      	ldr	r2, [pc, #112]	; (3000afc8 <BOOT_VerCheck+0x84>)
3000af58:	4619      	mov	r1, r3
3000af5a:	f7ff ffdf 	bl	3000af1c <rtk_log_write>
3000af5e:	4b18      	ldr	r3, [pc, #96]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000af60:	462a      	mov	r2, r5
3000af62:	2003      	movs	r0, #3
3000af64:	4619      	mov	r1, r3
3000af66:	f7ff ffd9 	bl	3000af1c <rtk_log_write>
3000af6a:	4b15      	ldr	r3, [pc, #84]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000af6c:	4a17      	ldr	r2, [pc, #92]	; (3000afcc <BOOT_VerCheck+0x88>)
3000af6e:	2003      	movs	r0, #3
3000af70:	4619      	mov	r1, r3
3000af72:	f7ff ffd3 	bl	3000af1c <rtk_log_write>
3000af76:	4b12      	ldr	r3, [pc, #72]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000af78:	4a15      	ldr	r2, [pc, #84]	; (3000afd0 <BOOT_VerCheck+0x8c>)
3000af7a:	2003      	movs	r0, #3
3000af7c:	4619      	mov	r1, r3
3000af7e:	f7ff ffcd 	bl	3000af1c <rtk_log_write>
3000af82:	4b0f      	ldr	r3, [pc, #60]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000af84:	4a13      	ldr	r2, [pc, #76]	; (3000afd4 <BOOT_VerCheck+0x90>)
3000af86:	2003      	movs	r0, #3
3000af88:	4619      	mov	r1, r3
3000af8a:	f7ff ffc7 	bl	3000af1c <rtk_log_write>
3000af8e:	4b0c      	ldr	r3, [pc, #48]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000af90:	462a      	mov	r2, r5
3000af92:	2003      	movs	r0, #3
3000af94:	4619      	mov	r1, r3
3000af96:	f7ff ffc1 	bl	3000af1c <rtk_log_write>
3000af9a:	4b09      	ldr	r3, [pc, #36]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000af9c:	4a0e      	ldr	r2, [pc, #56]	; (3000afd8 <BOOT_VerCheck+0x94>)
3000af9e:	2003      	movs	r0, #3
3000afa0:	4619      	mov	r1, r3
3000afa2:	f7ff ffbb 	bl	3000af1c <rtk_log_write>
3000afa6:	3c01      	subs	r4, #1
3000afa8:	d1d9      	bne.n	3000af5e <BOOT_VerCheck+0x1a>
3000afaa:	4b05      	ldr	r3, [pc, #20]	; (3000afc0 <BOOT_VerCheck+0x7c>)
3000afac:	2003      	movs	r0, #3
3000afae:	4a0b      	ldr	r2, [pc, #44]	; (3000afdc <BOOT_VerCheck+0x98>)
3000afb0:	4619      	mov	r1, r3
3000afb2:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000afb6:	f7ff bfb1 	b.w	3000af1c <rtk_log_write>
3000afba:	bd70      	pop	{r4, r5, r6, pc}
3000afbc:	3000903d 	.word	0x3000903d
3000afc0:	3000c714 	.word	0x3000c714
3000afc4:	3000d2e7 	.word	0x3000d2e7
3000afc8:	3000d2a4 	.word	0x3000d2a4
3000afcc:	3000d32a 	.word	0x3000d32a
3000afd0:	3000d36d 	.word	0x3000d36d
3000afd4:	3000d3b0 	.word	0x3000d3b0
3000afd8:	3000d3f3 	.word	0x3000d3f3
3000afdc:	3000d436 	.word	0x3000d436

3000afe0 <__aeabi_uldivmod>:
3000afe0:	b953      	cbnz	r3, 3000aff8 <__aeabi_uldivmod+0x18>
3000afe2:	b94a      	cbnz	r2, 3000aff8 <__aeabi_uldivmod+0x18>
3000afe4:	2900      	cmp	r1, #0
3000afe6:	bf08      	it	eq
3000afe8:	2800      	cmpeq	r0, #0
3000afea:	bf1c      	itt	ne
3000afec:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000aff0:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000aff4:	f000 b960 	b.w	3000b2b8 <__aeabi_idiv0>
3000aff8:	f1ad 0c08 	sub.w	ip, sp, #8
3000affc:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000b000:	f000 f806 	bl	3000b010 <__udivmoddi4>
3000b004:	f8dd e004 	ldr.w	lr, [sp, #4]
3000b008:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000b00c:	b004      	add	sp, #16
3000b00e:	4770      	bx	lr

3000b010 <__udivmoddi4>:
3000b010:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b014:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000b016:	4688      	mov	r8, r1
3000b018:	468e      	mov	lr, r1
3000b01a:	4615      	mov	r5, r2
3000b01c:	4604      	mov	r4, r0
3000b01e:	4619      	mov	r1, r3
3000b020:	2b00      	cmp	r3, #0
3000b022:	f040 80c6 	bne.w	3000b1b2 <__udivmoddi4+0x1a2>
3000b026:	4542      	cmp	r2, r8
3000b028:	fab2 f782 	clz	r7, r2
3000b02c:	d946      	bls.n	3000b0bc <__udivmoddi4+0xac>
3000b02e:	b14f      	cbz	r7, 3000b044 <__udivmoddi4+0x34>
3000b030:	f1c7 0c20 	rsb	ip, r7, #32
3000b034:	fa08 f307 	lsl.w	r3, r8, r7
3000b038:	40bd      	lsls	r5, r7
3000b03a:	40bc      	lsls	r4, r7
3000b03c:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b040:	ea4c 0e03 	orr.w	lr, ip, r3
3000b044:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000b048:	fa1f fc85 	uxth.w	ip, r5
3000b04c:	0c22      	lsrs	r2, r4, #16
3000b04e:	fbbe f9f8 	udiv	r9, lr, r8
3000b052:	fb08 e319 	mls	r3, r8, r9, lr
3000b056:	fb09 fa0c 	mul.w	sl, r9, ip
3000b05a:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000b05e:	459a      	cmp	sl, r3
3000b060:	d928      	bls.n	3000b0b4 <__udivmoddi4+0xa4>
3000b062:	18eb      	adds	r3, r5, r3
3000b064:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000b068:	d204      	bcs.n	3000b074 <__udivmoddi4+0x64>
3000b06a:	459a      	cmp	sl, r3
3000b06c:	d902      	bls.n	3000b074 <__udivmoddi4+0x64>
3000b06e:	f1a9 0002 	sub.w	r0, r9, #2
3000b072:	442b      	add	r3, r5
3000b074:	eba3 030a 	sub.w	r3, r3, sl
3000b078:	b2a4      	uxth	r4, r4
3000b07a:	fbb3 f2f8 	udiv	r2, r3, r8
3000b07e:	fb08 3312 	mls	r3, r8, r2, r3
3000b082:	fb02 fc0c 	mul.w	ip, r2, ip
3000b086:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b08a:	45a4      	cmp	ip, r4
3000b08c:	d914      	bls.n	3000b0b8 <__udivmoddi4+0xa8>
3000b08e:	192c      	adds	r4, r5, r4
3000b090:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000b094:	d203      	bcs.n	3000b09e <__udivmoddi4+0x8e>
3000b096:	45a4      	cmp	ip, r4
3000b098:	d901      	bls.n	3000b09e <__udivmoddi4+0x8e>
3000b09a:	1e93      	subs	r3, r2, #2
3000b09c:	442c      	add	r4, r5
3000b09e:	eba4 040c 	sub.w	r4, r4, ip
3000b0a2:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000b0a6:	b11e      	cbz	r6, 3000b0b0 <__udivmoddi4+0xa0>
3000b0a8:	40fc      	lsrs	r4, r7
3000b0aa:	2300      	movs	r3, #0
3000b0ac:	6034      	str	r4, [r6, #0]
3000b0ae:	6073      	str	r3, [r6, #4]
3000b0b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b0b4:	4648      	mov	r0, r9
3000b0b6:	e7dd      	b.n	3000b074 <__udivmoddi4+0x64>
3000b0b8:	4613      	mov	r3, r2
3000b0ba:	e7f0      	b.n	3000b09e <__udivmoddi4+0x8e>
3000b0bc:	b902      	cbnz	r2, 3000b0c0 <__udivmoddi4+0xb0>
3000b0be:	deff      	udf	#255	; 0xff
3000b0c0:	bb8f      	cbnz	r7, 3000b126 <__udivmoddi4+0x116>
3000b0c2:	eba8 0302 	sub.w	r3, r8, r2
3000b0c6:	2101      	movs	r1, #1
3000b0c8:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b0cc:	b2aa      	uxth	r2, r5
3000b0ce:	0c20      	lsrs	r0, r4, #16
3000b0d0:	fbb3 fcfe 	udiv	ip, r3, lr
3000b0d4:	fb0e 331c 	mls	r3, lr, ip, r3
3000b0d8:	fb0c f802 	mul.w	r8, ip, r2
3000b0dc:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000b0e0:	4598      	cmp	r8, r3
3000b0e2:	d962      	bls.n	3000b1aa <__udivmoddi4+0x19a>
3000b0e4:	18eb      	adds	r3, r5, r3
3000b0e6:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000b0ea:	d204      	bcs.n	3000b0f6 <__udivmoddi4+0xe6>
3000b0ec:	4598      	cmp	r8, r3
3000b0ee:	d902      	bls.n	3000b0f6 <__udivmoddi4+0xe6>
3000b0f0:	f1ac 0002 	sub.w	r0, ip, #2
3000b0f4:	442b      	add	r3, r5
3000b0f6:	eba3 0308 	sub.w	r3, r3, r8
3000b0fa:	b2a4      	uxth	r4, r4
3000b0fc:	fbb3 fcfe 	udiv	ip, r3, lr
3000b100:	fb0e 331c 	mls	r3, lr, ip, r3
3000b104:	fb0c f202 	mul.w	r2, ip, r2
3000b108:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b10c:	42a2      	cmp	r2, r4
3000b10e:	d94e      	bls.n	3000b1ae <__udivmoddi4+0x19e>
3000b110:	192c      	adds	r4, r5, r4
3000b112:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000b116:	d204      	bcs.n	3000b122 <__udivmoddi4+0x112>
3000b118:	42a2      	cmp	r2, r4
3000b11a:	d902      	bls.n	3000b122 <__udivmoddi4+0x112>
3000b11c:	f1ac 0302 	sub.w	r3, ip, #2
3000b120:	442c      	add	r4, r5
3000b122:	1aa4      	subs	r4, r4, r2
3000b124:	e7bd      	b.n	3000b0a2 <__udivmoddi4+0x92>
3000b126:	f1c7 0c20 	rsb	ip, r7, #32
3000b12a:	40bd      	lsls	r5, r7
3000b12c:	fa08 f307 	lsl.w	r3, r8, r7
3000b130:	40bc      	lsls	r4, r7
3000b132:	fa28 f90c 	lsr.w	r9, r8, ip
3000b136:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b13a:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b13e:	ea4c 0203 	orr.w	r2, ip, r3
3000b142:	b2ab      	uxth	r3, r5
3000b144:	fbb9 fcfe 	udiv	ip, r9, lr
3000b148:	0c11      	lsrs	r1, r2, #16
3000b14a:	fb0e 901c 	mls	r0, lr, ip, r9
3000b14e:	fb0c f803 	mul.w	r8, ip, r3
3000b152:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b156:	4580      	cmp	r8, r0
3000b158:	d923      	bls.n	3000b1a2 <__udivmoddi4+0x192>
3000b15a:	1828      	adds	r0, r5, r0
3000b15c:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000b160:	d204      	bcs.n	3000b16c <__udivmoddi4+0x15c>
3000b162:	4580      	cmp	r8, r0
3000b164:	d902      	bls.n	3000b16c <__udivmoddi4+0x15c>
3000b166:	f1ac 0102 	sub.w	r1, ip, #2
3000b16a:	4428      	add	r0, r5
3000b16c:	eba0 0008 	sub.w	r0, r0, r8
3000b170:	b292      	uxth	r2, r2
3000b172:	fbb0 fcfe 	udiv	ip, r0, lr
3000b176:	fb0e 001c 	mls	r0, lr, ip, r0
3000b17a:	fb0c f803 	mul.w	r8, ip, r3
3000b17e:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000b182:	4598      	cmp	r8, r3
3000b184:	d90f      	bls.n	3000b1a6 <__udivmoddi4+0x196>
3000b186:	18eb      	adds	r3, r5, r3
3000b188:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000b18c:	d204      	bcs.n	3000b198 <__udivmoddi4+0x188>
3000b18e:	4598      	cmp	r8, r3
3000b190:	d902      	bls.n	3000b198 <__udivmoddi4+0x188>
3000b192:	f1ac 0202 	sub.w	r2, ip, #2
3000b196:	442b      	add	r3, r5
3000b198:	eba3 0308 	sub.w	r3, r3, r8
3000b19c:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000b1a0:	e792      	b.n	3000b0c8 <__udivmoddi4+0xb8>
3000b1a2:	4661      	mov	r1, ip
3000b1a4:	e7e2      	b.n	3000b16c <__udivmoddi4+0x15c>
3000b1a6:	4662      	mov	r2, ip
3000b1a8:	e7f6      	b.n	3000b198 <__udivmoddi4+0x188>
3000b1aa:	4660      	mov	r0, ip
3000b1ac:	e7a3      	b.n	3000b0f6 <__udivmoddi4+0xe6>
3000b1ae:	4663      	mov	r3, ip
3000b1b0:	e7b7      	b.n	3000b122 <__udivmoddi4+0x112>
3000b1b2:	4543      	cmp	r3, r8
3000b1b4:	d905      	bls.n	3000b1c2 <__udivmoddi4+0x1b2>
3000b1b6:	b10e      	cbz	r6, 3000b1bc <__udivmoddi4+0x1ac>
3000b1b8:	e9c6 0800 	strd	r0, r8, [r6]
3000b1bc:	2100      	movs	r1, #0
3000b1be:	4608      	mov	r0, r1
3000b1c0:	e776      	b.n	3000b0b0 <__udivmoddi4+0xa0>
3000b1c2:	fab3 fc83 	clz	ip, r3
3000b1c6:	f1bc 0f00 	cmp.w	ip, #0
3000b1ca:	d110      	bne.n	3000b1ee <__udivmoddi4+0x1de>
3000b1cc:	4543      	cmp	r3, r8
3000b1ce:	d301      	bcc.n	3000b1d4 <__udivmoddi4+0x1c4>
3000b1d0:	4282      	cmp	r2, r0
3000b1d2:	d80a      	bhi.n	3000b1ea <__udivmoddi4+0x1da>
3000b1d4:	1a84      	subs	r4, r0, r2
3000b1d6:	eb68 0303 	sbc.w	r3, r8, r3
3000b1da:	2001      	movs	r0, #1
3000b1dc:	469e      	mov	lr, r3
3000b1de:	2e00      	cmp	r6, #0
3000b1e0:	d068      	beq.n	3000b2b4 <__udivmoddi4+0x2a4>
3000b1e2:	e9c6 4e00 	strd	r4, lr, [r6]
3000b1e6:	2100      	movs	r1, #0
3000b1e8:	e762      	b.n	3000b0b0 <__udivmoddi4+0xa0>
3000b1ea:	4660      	mov	r0, ip
3000b1ec:	e7f7      	b.n	3000b1de <__udivmoddi4+0x1ce>
3000b1ee:	f1cc 0e20 	rsb	lr, ip, #32
3000b1f2:	fa03 f30c 	lsl.w	r3, r3, ip
3000b1f6:	fa02 f50c 	lsl.w	r5, r2, ip
3000b1fa:	fa00 f70c 	lsl.w	r7, r0, ip
3000b1fe:	fa22 f40e 	lsr.w	r4, r2, lr
3000b202:	fa28 fb0e 	lsr.w	fp, r8, lr
3000b206:	fa20 f10e 	lsr.w	r1, r0, lr
3000b20a:	431c      	orrs	r4, r3
3000b20c:	fa08 f30c 	lsl.w	r3, r8, ip
3000b210:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000b214:	4319      	orrs	r1, r3
3000b216:	b2a3      	uxth	r3, r4
3000b218:	0c0a      	lsrs	r2, r1, #16
3000b21a:	fbbb f8f9 	udiv	r8, fp, r9
3000b21e:	fb09 bb18 	mls	fp, r9, r8, fp
3000b222:	fb08 fa03 	mul.w	sl, r8, r3
3000b226:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000b22a:	4592      	cmp	sl, r2
3000b22c:	d93e      	bls.n	3000b2ac <__udivmoddi4+0x29c>
3000b22e:	18a2      	adds	r2, r4, r2
3000b230:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000b234:	d204      	bcs.n	3000b240 <__udivmoddi4+0x230>
3000b236:	4592      	cmp	sl, r2
3000b238:	d902      	bls.n	3000b240 <__udivmoddi4+0x230>
3000b23a:	f1a8 0002 	sub.w	r0, r8, #2
3000b23e:	4422      	add	r2, r4
3000b240:	eba2 020a 	sub.w	r2, r2, sl
3000b244:	b289      	uxth	r1, r1
3000b246:	fbb2 f8f9 	udiv	r8, r2, r9
3000b24a:	fb09 2218 	mls	r2, r9, r8, r2
3000b24e:	fb08 f303 	mul.w	r3, r8, r3
3000b252:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000b256:	4293      	cmp	r3, r2
3000b258:	d92a      	bls.n	3000b2b0 <__udivmoddi4+0x2a0>
3000b25a:	18a2      	adds	r2, r4, r2
3000b25c:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000b260:	d204      	bcs.n	3000b26c <__udivmoddi4+0x25c>
3000b262:	4293      	cmp	r3, r2
3000b264:	d902      	bls.n	3000b26c <__udivmoddi4+0x25c>
3000b266:	f1a8 0102 	sub.w	r1, r8, #2
3000b26a:	4422      	add	r2, r4
3000b26c:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b270:	1ad2      	subs	r2, r2, r3
3000b272:	fba0 9805 	umull	r9, r8, r0, r5
3000b276:	4542      	cmp	r2, r8
3000b278:	464b      	mov	r3, r9
3000b27a:	4641      	mov	r1, r8
3000b27c:	d302      	bcc.n	3000b284 <__udivmoddi4+0x274>
3000b27e:	d106      	bne.n	3000b28e <__udivmoddi4+0x27e>
3000b280:	454f      	cmp	r7, r9
3000b282:	d204      	bcs.n	3000b28e <__udivmoddi4+0x27e>
3000b284:	3801      	subs	r0, #1
3000b286:	ebb9 0305 	subs.w	r3, r9, r5
3000b28a:	eb68 0104 	sbc.w	r1, r8, r4
3000b28e:	b18e      	cbz	r6, 3000b2b4 <__udivmoddi4+0x2a4>
3000b290:	1afc      	subs	r4, r7, r3
3000b292:	eb62 0701 	sbc.w	r7, r2, r1
3000b296:	fa07 fe0e 	lsl.w	lr, r7, lr
3000b29a:	fa24 f30c 	lsr.w	r3, r4, ip
3000b29e:	fa27 f70c 	lsr.w	r7, r7, ip
3000b2a2:	ea4e 0303 	orr.w	r3, lr, r3
3000b2a6:	e9c6 3700 	strd	r3, r7, [r6]
3000b2aa:	e79c      	b.n	3000b1e6 <__udivmoddi4+0x1d6>
3000b2ac:	4640      	mov	r0, r8
3000b2ae:	e7c7      	b.n	3000b240 <__udivmoddi4+0x230>
3000b2b0:	4641      	mov	r1, r8
3000b2b2:	e7db      	b.n	3000b26c <__udivmoddi4+0x25c>
3000b2b4:	4631      	mov	r1, r6
3000b2b6:	e6fb      	b.n	3000b0b0 <__udivmoddi4+0xa0>

3000b2b8 <__aeabi_idiv0>:
3000b2b8:	4770      	bx	lr
3000b2ba:	bf00      	nop

3000b2bc <_mbtowc_r>:
3000b2bc:	b430      	push	{r4, r5}
3000b2be:	4c06      	ldr	r4, [pc, #24]	; (3000b2d8 <_mbtowc_r+0x1c>)
3000b2c0:	4d06      	ldr	r5, [pc, #24]	; (3000b2dc <_mbtowc_r+0x20>)
3000b2c2:	6824      	ldr	r4, [r4, #0]
3000b2c4:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b2c6:	2c00      	cmp	r4, #0
3000b2c8:	bf08      	it	eq
3000b2ca:	462c      	moveq	r4, r5
3000b2cc:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000b2d0:	46a4      	mov	ip, r4
3000b2d2:	bc30      	pop	{r4, r5}
3000b2d4:	4760      	bx	ip
3000b2d6:	bf00      	nop
3000b2d8:	3000e698 	.word	0x3000e698
3000b2dc:	3000e528 	.word	0x3000e528

3000b2e0 <__ascii_mbtowc>:
3000b2e0:	b082      	sub	sp, #8
3000b2e2:	b901      	cbnz	r1, 3000b2e6 <__ascii_mbtowc+0x6>
3000b2e4:	a901      	add	r1, sp, #4
3000b2e6:	b142      	cbz	r2, 3000b2fa <__ascii_mbtowc+0x1a>
3000b2e8:	b14b      	cbz	r3, 3000b2fe <__ascii_mbtowc+0x1e>
3000b2ea:	7813      	ldrb	r3, [r2, #0]
3000b2ec:	600b      	str	r3, [r1, #0]
3000b2ee:	7812      	ldrb	r2, [r2, #0]
3000b2f0:	1e10      	subs	r0, r2, #0
3000b2f2:	bf18      	it	ne
3000b2f4:	2001      	movne	r0, #1
3000b2f6:	b002      	add	sp, #8
3000b2f8:	4770      	bx	lr
3000b2fa:	4610      	mov	r0, r2
3000b2fc:	e7fb      	b.n	3000b2f6 <__ascii_mbtowc+0x16>
3000b2fe:	f06f 0001 	mvn.w	r0, #1
3000b302:	e7f8      	b.n	3000b2f6 <__ascii_mbtowc+0x16>

3000b304 <_wctomb_r>:
3000b304:	b430      	push	{r4, r5}
3000b306:	4c06      	ldr	r4, [pc, #24]	; (3000b320 <_wctomb_r+0x1c>)
3000b308:	4d06      	ldr	r5, [pc, #24]	; (3000b324 <_wctomb_r+0x20>)
3000b30a:	6824      	ldr	r4, [r4, #0]
3000b30c:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b30e:	2c00      	cmp	r4, #0
3000b310:	bf08      	it	eq
3000b312:	462c      	moveq	r4, r5
3000b314:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000b318:	46a4      	mov	ip, r4
3000b31a:	bc30      	pop	{r4, r5}
3000b31c:	4760      	bx	ip
3000b31e:	bf00      	nop
3000b320:	3000e698 	.word	0x3000e698
3000b324:	3000e528 	.word	0x3000e528

3000b328 <__ascii_wctomb>:
3000b328:	b149      	cbz	r1, 3000b33e <__ascii_wctomb+0x16>
3000b32a:	2aff      	cmp	r2, #255	; 0xff
3000b32c:	bf8d      	iteet	hi
3000b32e:	238a      	movhi	r3, #138	; 0x8a
3000b330:	2001      	movls	r0, #1
3000b332:	700a      	strbls	r2, [r1, #0]
3000b334:	6003      	strhi	r3, [r0, #0]
3000b336:	bf88      	it	hi
3000b338:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000b33c:	4770      	bx	lr
3000b33e:	4608      	mov	r0, r1
3000b340:	4770      	bx	lr
3000b342:	bf00      	nop
3000b344:	0000      	movs	r0, r0
	...

3000b348 <__SBOOT_Validate_Algorithm_B_veneer>:
3000b348:	f85f f000 	ldr.w	pc, [pc]	; 3000b34c <__SBOOT_Validate_Algorithm_B_veneer+0x4>
3000b34c:	000033ed 	.word	0x000033ed

3000b350 <__SBOOT_Validate_ImgHash_A_veneer>:
3000b350:	f85f f000 	ldr.w	pc, [pc]	; 3000b354 <__SBOOT_Validate_ImgHash_A_veneer+0x4>
3000b354:	000033cd 	.word	0x000033cd

3000b358 <__SBOOT_Validate_Signature_B_veneer>:
3000b358:	f85f f000 	ldr.w	pc, [pc]	; 3000b35c <__SBOOT_Validate_Signature_B_veneer+0x4>
3000b35c:	00003431 	.word	0x00003431

3000b360 <____wrap_memcpy_veneer>:
3000b360:	f85f f000 	ldr.w	pc, [pc]	; 3000b364 <____wrap_memcpy_veneer+0x4>
3000b364:	00012f05 	.word	0x00012f05

3000b368 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000b368:	f85f f000 	ldr.w	pc, [pc]	; 3000b36c <__CRYPTO_OTPKey_SHA_Init_A_veneer+0x4>
3000b36c:	00002369 	.word	0x00002369

3000b370 <__rtl_crypto_sha2_final_A_veneer>:
3000b370:	f85f f000 	ldr.w	pc, [pc]	; 3000b374 <__rtl_crypto_sha2_final_A_veneer+0x4>
3000b374:	0000527d 	.word	0x0000527d

3000b378 <__CRYPTO_SHA_Init_A_veneer>:
3000b378:	f85f f000 	ldr.w	pc, [pc]	; 3000b37c <__CRYPTO_SHA_Init_A_veneer+0x4>
3000b37c:	000022dd 	.word	0x000022dd

3000b380 <__CRYPTO_SHA_Init_B_veneer>:
3000b380:	f85f f000 	ldr.w	pc, [pc]	; 3000b384 <__CRYPTO_SHA_Init_B_veneer+0x4>
3000b384:	00002451 	.word	0x00002451

3000b388 <____wrap_strlen_veneer>:
3000b388:	f85f f000 	ldr.w	pc, [pc]	; 3000b38c <____wrap_strlen_veneer+0x4>
3000b38c:	00012f25 	.word	0x00012f25

3000b390 <____wrap_memcmp_veneer>:
3000b390:	f85f f000 	ldr.w	pc, [pc]	; 3000b394 <____wrap_memcmp_veneer+0x4>
3000b394:	00012f0d 	.word	0x00012f0d

3000b398 <__DiagVSprintf_veneer>:
3000b398:	f85f f000 	ldr.w	pc, [pc]	; 3000b39c <__DiagVSprintf_veneer+0x4>
3000b39c:	0000e331 	.word	0x0000e331

3000b3a0 <__SBOOT_GetMdType_B_veneer>:
3000b3a0:	f85f f000 	ldr.w	pc, [pc]	; 3000b3a4 <__SBOOT_GetMdType_B_veneer+0x4>
3000b3a4:	0000319d 	.word	0x0000319d

3000b3a8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000b3a8:	f85f f000 	ldr.w	pc, [pc]	; 3000b3ac <__CRYPTO_OTPKey_SHA_Init_B_veneer+0x4>
3000b3ac:	000024dd 	.word	0x000024dd

3000b3b0 <__rtl_crypto_sha2_init_A_veneer>:
3000b3b0:	f85f f000 	ldr.w	pc, [pc]	; 3000b3b4 <__rtl_crypto_sha2_init_A_veneer+0x4>
3000b3b4:	00004e89 	.word	0x00004e89

3000b3b8 <__rtl_crypto_sha2_init_B_veneer>:
3000b3b8:	f85f f000 	ldr.w	pc, [pc]	; 3000b3bc <__rtl_crypto_sha2_init_B_veneer+0x4>
3000b3bc:	00004ffd 	.word	0x00004ffd

3000b3c0 <____wrap_memset_veneer>:
3000b3c0:	f85f f000 	ldr.w	pc, [pc]	; 3000b3c4 <____wrap_memset_veneer+0x4>
3000b3c4:	00012efd 	.word	0x00012efd

3000b3c8 <__DiagPrintf_veneer>:
3000b3c8:	f85f f000 	ldr.w	pc, [pc]	; 3000b3cc <__DiagPrintf_veneer+0x4>
3000b3cc:	0000e5f9 	.word	0x0000e5f9

3000b3d0 <__SBOOT_Validate_Signature_A_veneer>:
3000b3d0:	f85f f000 	ldr.w	pc, [pc]	; 3000b3d4 <__SBOOT_Validate_Signature_A_veneer+0x4>
3000b3d4:	000032bd 	.word	0x000032bd

3000b3d8 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000b3d8:	f85f f000 	ldr.w	pc, [pc]	; 3000b3dc <__rtl_crypto_hmac_sha2_update_B_veneer+0x4>
3000b3dc:	00005b4d 	.word	0x00005b4d

3000b3e0 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000b3e0:	f85f f000 	ldr.w	pc, [pc]	; 3000b3e4 <__rtl_crypto_hmac_sha2_final_B_veneer+0x4>
3000b3e4:	00005bb9 	.word	0x00005bb9

3000b3e8 <__SBOOT_Validate_PubKey_B_veneer>:
3000b3e8:	f85f f000 	ldr.w	pc, [pc]	; 3000b3ec <__SBOOT_Validate_PubKey_B_veneer+0x4>
3000b3ec:	00003495 	.word	0x00003495

3000b3f0 <__rtl_crypto_sha2_update_A_veneer>:
3000b3f0:	f85f f000 	ldr.w	pc, [pc]	; 3000b3f4 <__rtl_crypto_sha2_update_A_veneer+0x4>
3000b3f4:	00005211 	.word	0x00005211

3000b3f8 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000b3f8:	f85f f000 	ldr.w	pc, [pc]	; 3000b3fc <__rtl_crypto_hmac_sha2_update_A_veneer+0x4>
3000b3fc:	000059d9 	.word	0x000059d9

3000b400 <__rtl_crypto_sha2_final_B_veneer>:
3000b400:	f85f f000 	ldr.w	pc, [pc]	; 3000b404 <__rtl_crypto_sha2_final_B_veneer+0x4>
3000b404:	000053f1 	.word	0x000053f1

3000b408 <__SBOOT_Validate_ImgHash_B_veneer>:
3000b408:	f85f f000 	ldr.w	pc, [pc]	; 3000b40c <__SBOOT_Validate_ImgHash_B_veneer+0x4>
3000b40c:	00003541 	.word	0x00003541

3000b410 <__SBOOT_Validate_PubKey_A_veneer>:
3000b410:	f85f f000 	ldr.w	pc, [pc]	; 3000b414 <__SBOOT_Validate_PubKey_A_veneer+0x4>
3000b414:	00003321 	.word	0x00003321

3000b418 <__rtl_crypto_sha2_update_B_veneer>:
3000b418:	f85f f000 	ldr.w	pc, [pc]	; 3000b41c <__rtl_crypto_sha2_update_B_veneer+0x4>
3000b41c:	00005385 	.word	0x00005385

3000b420 <__SBOOT_GetMdType_A_veneer>:
3000b420:	f85f f000 	ldr.w	pc, [pc]	; 3000b424 <__SBOOT_GetMdType_A_veneer+0x4>
3000b424:	00003029 	.word	0x00003029

3000b428 <__SBOOT_Validate_Algorithm_A_veneer>:
3000b428:	f85f f000 	ldr.w	pc, [pc]	; 3000b42c <__SBOOT_Validate_Algorithm_A_veneer+0x4>
3000b42c:	00003279 	.word	0x00003279

3000b430 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000b430:	f85f f000 	ldr.w	pc, [pc]	; 3000b434 <__rtl_crypto_hmac_sha2_final_A_veneer+0x4>
3000b434:	00005a45 	.word	0x00005a45

3000b438 <__INT_HardFault_C_veneer>:
3000b438:	f85f f000 	ldr.w	pc, [pc]	; 3000b43c <__INT_HardFault_C_veneer+0x4>
3000b43c:	0000d8ed 	.word	0x0000d8ed

3000b440 <__io_assert_failed_veneer>:
3000b440:	f85f f000 	ldr.w	pc, [pc]	; 3000b444 <__io_assert_failed_veneer+0x4>
3000b444:	0000992d 	.word	0x0000992d
3000b448:	50494843 	.word	0x50494843
3000b44c:	4f464e49 	.word	0x4f464e49
3000b450:	73255b00 	.word	0x73255b00
3000b454:	205d452d 	.word	0x205d452d
3000b458:	61766e49 	.word	0x61766e49
3000b45c:	2064696c 	.word	0x2064696c
3000b460:	6e696843 	.word	0x6e696843
3000b464:	6f666e69 	.word	0x6f666e69
3000b468:	68432021 	.word	0x68432021
3000b46c:	206b6365 	.word	0x206b6365
3000b470:	0a50544f 	.word	0x0a50544f
3000b474:	00          	.byte	0x00
3000b475:	5b          	.byte	0x5b
3000b476:	7325      	.short	0x7325
3000b478:	205d572d 	.word	0x205d572d
3000b47c:	61656c50 	.word	0x61656c50
3000b480:	70206573 	.word	0x70206573
3000b484:	72676f72 	.word	0x72676f72
3000b488:	63206d61 	.word	0x63206d61
3000b48c:	69706968 	.word	0x69706968
3000b490:	206f666e 	.word	0x206f666e
3000b494:	4f206e69 	.word	0x4f206e69
3000b498:	21205054 	.word	0x21205054
3000b49c:	000a      	.short	0x000a

3000b49e <Memory_Info>:
3000b49e:	0007 03e0 0004 0006 02c0 0102 0100 0201     ................
3000b4ae:	0102 0200 0302 0004 0400 0204 0102 0101     ................
3000b4be:	0221 0102 0401 0224 0102 0300 0103 0003     !.....$.........
3000b4ce:	0500 0105 0002 0600 0206 0102 0700 0107     ................
3000b4de:	0002 0800 0208 0102 0801 0228 0102 0900     ..........(.....
3000b4ee:	0309 0004 0901 0329 0004 0a00 010a 0003     ......).........
3000b4fe:	0a01 012a 0003 0b00 020b 0102 0c00 020c     ..*.............
3000b50e:	0102 0d00 030d 0004 0e00 010e 0003 ffff     ................
3000b51e:	ffff ffff 4444 5052 5948 5b00 7325 452d     ....DDRPHY.[%s-E
3000b52e:	205d 5244 4d41 6920 2073 6f6e 2074 6143     ] DRAM is not Ca
3000b53e:	696c 7262 6961 6e6f 0a0d 0000 0000          libraion......

3000b54c <ddrphy_tx_scan>:
3000b54c:	0000 0000 0000 0008 0808 0008 0000 0000     ................
3000b55c:	0000 2000 3333 3333 0022 000c 0000 0000     ... 3333".......
3000b56c:	0012 0706 0707 000f 0f0f 080f 0008 0000     ................
3000b57c:	0001 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b58c:	0016 0b0b 0b0b 0013 1313 0e13 000e 0000     ................
3000b59c:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b5ac:	0018 0e0e 0e0e 0017 1717 0e17 000e 0000     ................
3000b5bc:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b5cc:	0018 0a0a 0a0a 0013 1313 0e13 000e 0000     ................
3000b5dc:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b5ec:	001a 0e0e 0e0e 0017 1717 1217 0012 0000     ................
3000b5fc:	0fc1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b60c:	0016 0f0f 0f0f 0018 1818 0e18 000e 0000     ................
3000b61c:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b62c:	001b 0f0f 0f0f 0018 1818 1318 0013 0000     ................
3000b63c:	0fc1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b64c:	001b 1110 1111 0019 1919 1319 0013 0000     ................
3000b65c:	0ffd 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b66c:	001c 1313 1313 001c 1c1c 121c 0012 0000     ................
3000b67c:	0ffd 2000 2222 2222 0012 000c 0000 3400     ... """".......4

3000b68c <ddrphy_zq_rx_scan>:
	...
3000b694:	2626 2626 2626 2626 030f 0303 1414 0e14     &&&&&&&&........
3000b6a4:	0e0e 140e 362e 261f 3029 261c 020f 0302     .....6.&)0.&....
3000b6b4:	1414 0e14 0e0e 140e 362e 261f 3029 261c     .........6.&)0.&
3000b6c4:	020f 0302 0b0b 0a0c 0a0a 0b0b 2721 2617     ............!'.&
3000b6d4:	231e 2615 0206 0402 0c0c 0a0e 0a0a 0c0b     .#.&............
3000b6e4:	362c 261e 2620 2616 0203 0402 1414 0e14     ,6.& &.&........
3000b6f4:	0e0e 140e 3c34 2624 3029 261c 030f 0303     ....4<$&)0.&....
3000b704:	0a0a 090b 0909 0a0a 231e 2615 201b 2613     .........#.&. .&
3000b714:	0204 0402 0c0c 0b0f 0b0b 0c0b 3a30 2623     ............0:#&
3000b724:	2722 2618 0205 0402 1414 0e14 0e0e 140e     "'.&............
3000b734:	3c34 2624 2f28 261b 030f 0303 0c0c 0a0c     4<$&(/.&........
3000b744:	0a0a 0c0a 251f 2616 221d 2614 0205 0402     .....%.&.".&....

3000b754 <Flash_AVL>:
3000b754:	00ef 0000 00ff 0000 0000 0000 43fc 0000     .............C..
3000b764:	0000 0000 00a1 0000 00ff 0000 0000 0000     ................
3000b774:	fffc 0000 0000 0000 000b 0000 00ff 0000     ................
3000b784:	0000 0000 43fc 0000 0000 0000 000e 0000     .....C..........
3000b794:	00ff 0000 0000 0000 43fc 0000 0000 0000     .........C......
3000b7a4:	00c8 0000 00ff 0000 0001 0000 43fc 0000     .............C..
3000b7b4:	0000 0000 28c2 0000 ffff 0000 0005 0000     .....(..........
3000b7c4:	00fc 0002 0000 0000 00c2 0000 00ff 0000     ................
3000b7d4:	0002 0000 00fc 0000 0000 0000 0068 0000     ............h...
3000b7e4:	00ff 0000 0002 0000 00fc 0000 0000 0000     ................
3000b7f4:	0051 0000 00ff 0000 0002 0000 00fc 0000     Q...............
3000b804:	0000 0000 001c 0000 00ff 0000 0003 0000     ................
3000b814:	00fc 0000 0000 0000 0020 0000 00ff 0000     ........ .......
3000b824:	0004 0000 00fc 0000 0000 0000 0000 0000     ................
3000b834:	00ff 0000 00fe 0000 ffff ffff 79e5 3000     .............y.0
3000b844:	00ff 0000 ffff ffff 00ff 0000 ffff ffff     ................
3000b854:	0000 0000                                   ....

3000b858 <Flash_ReadMode>:
3000b858:	ffff                                        ..

3000b85a <Flash_Speed>:
3000b85a:	07ff                                        ..

3000b85c <NAND_AVL>:
3000b85c:	00ef 0000 00ff 0000 0000 0000 ffff ffff     ................
3000b86c:	0000 0000 00c8 0000 00ff 0000 0001 0000     ................
3000b87c:	ffff ffff 0000 0000 00e5 0000 00ff 0000     ................
3000b88c:	0001 0000 ffff ffff 0000 0000 00c2 0000     ................
3000b89c:	00ff 0000 0002 0000 ffff ffff 0000 0000     ................
3000b8ac:	002c 0000 00ff 0000 0004 0000 ffff ffff     ,...............
	...
3000b8c4:	00ff 0000 00fe 0000 ffff ffff 7a41 3000     ............Az.0
3000b8d4:	00ff 0000 ffff ffff 00ff 0000 ffff ffff     ................
3000b8e4:	0000 0000 4931 004f 4f32 3200 4f49 3400     ....1IO.2O.2IO.4
3000b8f4:	004f 4934 004f 4c46 5341 4348 4b4c 5b00     O.4IO.FLASHCLK.[
3000b904:	7325 492d 205d 6163 696c 7262 7461 6f69     %s-I] calibratio
3000b914:	5f6e 6b6f 5b3a 6425 253a 3a64 6425 205d     n_ok:[%d:%d:%d] 
3000b924:	000a 255b 2d73 5d49 4620 414c 4853 4320     ..[%s-I] FLASH C
3000b934:	4c41 4249 305b 2578 2078 4b4f 0a5d 5b00     ALIB[0x%x OK]..[
3000b944:	7325 452d 205d 4c46 5341 2048 4143 494c     %s-E] FLASH CALI
3000b954:	5b42 7830 7825 4620 4941 5d4c 000a 255b     B[0x%x FAIL]..[%
3000b964:	2d73 5d49 4620 616c 6873 5220 6165 2064     s-I] Flash Read 
3000b974:	7325 000a 255b 2d73 5d45 4620 616c 6873     %s..[%s-E] Flash
3000b984:	5320 6977 6374 2068 6552 6461 4d20 646f      Switch Read Mod
3000b994:	2065 4146 4c49 000a 255b 2d73 5d49 4620     e FAIL..[%s-I] F
3000b9a4:	616c 6873 4920 3a44 2520 2d78 7825 252d     lash ID: %x-%x-%
3000b9b4:	0a78 5b00 7325 572d 205d 6854 7369 6620     x..[%s-W] This f
3000b9c4:	616c 6873 7420 7079 2065 7369 6e20 746f     lash type is not
3000b9d4:	7320 7075 6f70 7472 6465 0a21 5b00 7325      supported!..[%s
3000b9e4:	492d 205d 414e 444e 4920 3a44 2520 2d78     -I] NAND ID: %x-
3000b9f4:	7825 000a b8e8 3000 b8ec 3000 b8ef 3000     %x.....0...0...0
3000ba04:	b8f3 3000 b8f6 3000                         ...0...0

3000ba0c <__FUNCTION__.0>:
3000ba0c:	616e 646e 675f 7465 765f 6e65 6f64 0072     nand_get_vendor.

3000ba1c <__FUNCTION__.1>:
3000ba1c:	6c66 7361 5f68 6567 5f74 6576 646e 726f     flash_get_vendor
3000ba2c:	4e00 4e41 4644 414c 4853 5b00 7325 452d     .NANDFLASH.[%s-E
3000ba3c:	205d 6c42 206b 7830 7825 6920 2073 4242     ] Blk 0x%x is BB
3000ba4c:	0a21 5b00 7325 492d 205d 6552 6461 5020     !..[%s-I] Read P
3000ba5c:	6761 2065 7830 7825 6f20 2066 6c42 636f     age 0x%x of Bloc
3000ba6c:	206b 7830 7825 4d20 7961 4620 6961 2c6c     k 0x%x May Fail,
3000ba7c:	7320 6174 7574 2073 7830 7825 0a21 4f00      status 0x%x!..O
3000ba8c:	5054 5b00 7325 452d 205d 544f 5f50 6552     TP.[%s-E] OTP_Re
3000ba9c:	6461 2038 6146 6c69 2520 2078 000a 255b     ad8 Fail %x ..[%
3000baac:	2d73 5d45 5020 524d 5220 6165 2064 7265     s-E] PMR Read er
3000babc:	6f72 2172 000a 255b 2d73 5d57 4f20 5054     ror!..[%s-W] OTP
3000bacc:	575f 6972 6574 2038 6146 6c69 2520 2078     _Write8 Fail %x 
3000badc:	000a 255b 2d73 5d45 4f20 5054 7020 6f72     ..[%s-E] OTP pro
3000baec:	7267 6d61 6520 7272 726f 0a21 5b00 7325     gram error!..[%s
3000bafc:	452d 205d 4d50 3252 5220 6165 2064 7265     -E] PMR2 Read er
3000bb0c:	6f72 2172 000a                              ror!..

3000bb12 <__FUNCTION__.1>:
3000bb12:	4c50 5f4c 5041 435f 6b6c 6553 0074 5350     PLL_AP_ClkSet.PS
3000bb22:	4152 004d 255b 2d73 5d49 5020 5253 4d41     RAM.[%s-I] PSRAM
3000bb32:	4320 7274 206c 4c43 3a4b 2520 2064 7a48      Ctrl CLK: %d Hz
3000bb42:	0a20 5b00 7325 492d 205d 6143 4e6c 696d      ..[%s-I] CalNmi
3000bb52:	206e 203d 7825 4320 6c61 6d4e 7861 3d20     n = %x CalNmax =
3000bb62:	2520 2078 6957 646e 776f 6953 657a 3d20      %x WindowSize =
3000bb72:	2520 2078 6870 7361 3a65 2520 2078 000a      %x phase: %x ..
3000bb82:	4f52 504d 5441 4843 5b00 7325 452d 205d     ROMPATCH.[%s-E] 
3000bb92:	6f4e 7420 6968 2073 7561 6874 745f 7079     No this auth_typ
3000bba2:	2865 6425 2029 6f66 2072 4853 3241 000a     e(%d) for SHA2..
3000bbb2:	255b 2d73 5d57 6920 7172 255b 5d64 7020     [%s-W] irq[%d] p
3000bbc2:	6972 726f 7469 2079 6425 7320 6168 6c6c     riority %d shall
3000bbd2:	3c20 203d 6425 000a                          <= %d..

3000bbda <__FUNCTION__.0>:
3000bbda:	7269 5f71 6573 5f74 7270 6f69 6972 7974     irq_set_priority
	...

3000bbeb <__FUNCTION__.1>:
3000bbeb:	5352 5049 4f5f 4654 455f 616e 6c62 0065     RSIP_OTF_Enable.

3000bbfb <__FUNCTION__.3>:
3000bbfb:	7472 5f6c 7263 7079 6f74 685f 616d 5f63     rtl_crypto_hmac_
3000bc0b:	6873 3261 695f 696e 0074                    sha2_init.

3000bc15 <__FUNCTION__.4>:
3000bc15:	5243 5059 4f54 535f 7465 6553 7563 6972     CRYPTO_SetSecuri
3000bc25:	7974 6f4d 6564 4441 4400 5244 5b00 7325     tyModeAD.DDR.[%s
3000bc35:	452d 205d 4444 2052 6953 657a 6920 2073     -E] DDR Size is 
3000bc45:	6f6e 2074 6f63 666e 6769 7275 6465 000a     not configured..
3000bc55:	255b 2d73 5d45 3e20 5b20 4441 5244 5220     [%s-E] > [ADDR R
3000bc65:	4d45 5041 4520 5252 524f 3a5d 6320 6c6f     EMAP ERROR]: col
3000bc75:	5f75 6572 616d 5f70 6962 7374 2120 203d     u_remap_bits != 
3000bc85:	7264 6d61 635f 6c6f 5f75 6962 7374 2121     dram_colu_bits!!
3000bc95:	0a21 5b00 7325 452d 205d 203e 415b 4444     !..[%s-E] > [ADD
3000bca5:	2052 4552 414d 2050 5245 4f52 5d52 203a     R REMAP ERROR]: 
3000bcb5:	6162 6b6e 725f 6d65 7061 625f 7469 2073     bank_remap_bits 
3000bcc5:	3d21 6420 6172 5f6d 6162 6b6e 625f 7469     != dram_bank_bit
3000bcd5:	2173 2121 000a                               s!!!...

3000bcdc <SWR_IDX_FLAG>:
3000bcdc:	07e0 0000 07e4 0000 07e5 0000 07e6 0000     ................
3000bcec:	07e7 0000                                   ....

3000bcf0 <__FUNCTION__.1>:
3000bcf0:	5753 5f52 454d 5f4d 6f4d 6564 535f 7465     SWR_MEM_Mode_Set
3000bd00:	4b00 344d 4b00 304d 3d00 3d3d 3d3d 3d3d     .KM4.KM0.=======
3000bd10:	3d3d 253d 2073 7453 6361 206b 7544 706d     ===%s Stack Dump
3000bd20:	3d20 3d3d 3d3d 3d3d 3d3d 0a3d 4300 7275      ==========..Cur
3000bd30:	6572 746e 5320 6174 6b63 5020 696f 746e     rent Stack Point
3000bd40:	7265 3d20 3020 2578 2c70 6120 646e 6420     er = 0x%p, and d
3000bd50:	6d75 2070 7473 6361 206b 6564 7470 2068     ump stack depth 
3000bd60:	203d 6425 0a0d 0d00 5b0a 3025 7838 205d     = %d.....[%08x] 
3000bd70:	2500 3830 2078 3d00 3d3d 3d3d 3d3d 3d3d     .%08x .=========
3000bd80:	203d 7243 7361 2068 7544 706d 3d20 3d3d     = Crash Dump ===
3000bd90:	3d3d 3d3d 3d3d 0a3d 3d00 3d3d 3d3d 3d3d     =======..=======
3000bda0:	3d3d 203d 6552 6967 7473 7265 4420 6d75     === Register Dum
3000bdb0:	2070 3d3d 3d3d 3d3d 3d3d 3d3d 000a 205b     p ==========..[ 
3000bdc0:	4c20 5d52 3020 2578 7838 000a 205b 5020      LR] 0x%8x..[  P
3000bdd0:	5d43 3020 2578 7838 000a 785b 5350 5d52     C] 0x%8x..[xPSR]
3000bde0:	3020 2578 7838 000a 455b 4358 5d52 3020      0x%8x..[EXCR] 0
3000bdf0:	2578 7838 000a 205b 7325 205d 7830 3825     x%8x..[ %s] 0x%8
3000be00:	0a78 5200 3231 3d00 3d3d 3d3d 3d3d 3d3d     x..R12.=========
3000be10:	203d 6e45 2064 666f 4320 6172 6873 4420     = End of Crash D
3000be20:	6d75 2070 3d3d 3d3d 3d3d 3d3d 3d3d 000a     ump ==========..
3000be30:	0a0d 0a0d 5200 0034 3552 5200 0036 3752     .....R4.R5.R6.R7
3000be40:	5200 0038 3952 5200 3031 5200 3131 5200     .R8.R9.R10.R11.R
3000be50:	0030 3152 5200 0032 3352 0000               0.R1.R2.R3..

3000be5c <register_string>:
3000be5c:	0000 0000 be35 3000 be38 3000 be3b 3000     ....5..08..0;..0
3000be6c:	be3e 3000 be41 3000 be44 3000 be47 3000     >..0A..0D..0G..0
3000be7c:	be4b 3000 be4f 3000 be52 3000 be55 3000     K..0O..0R..0U..0
3000be8c:	be58 3000 be03 3000 7845 6563 7470 6f69     X..0...0Exceptio
3000be9c:	206e 6163 6775 7468 6f20 206e 7025 000a     n caught on %p..
3000beac:	6148 6472 6620 7561 746c 6920 2073 6163     Hard fault is ca
3000bebc:	7375 6465 6220 2079 6166 6c69 6465 7620     used by failed v
3000becc:	6365 6f74 2072 6566 6374 0068 4648 5253     ector fetch.HFSR
3000bedc:	203a 305b 2578 7838 205d 3e2d 2520 0a73     : [0x%8x] -> %s.
3000beec:	5300 6365 7275 7469 2079 6166 6c75 2074     .Security fault 
3000befc:	7369 6320 7561 6573 2064 7962 4920 766e     is caused by Inv
3000bf0c:	6c61 6469 6520 746e 7972 7020 696f 746e     alid entry point
3000bf1c:	5300 5346 3a52 5b20 7830 3825 5d78 2d20     .SFSR: [0x%8x] -
3000bf2c:	203e 7325 000a 6553 7563 6972 7974 6620     > %s..Security f
3000bf3c:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000bf4c:	2079 6e49 6176 696c 2064 6e69 6574 7267     y Invalid integr
3000bf5c:	7469 2079 6973 6e67 7461 7275 0065 6553     ity signature.Se
3000bf6c:	7563 6972 7974 6620 7561 746c 6920 2073     curity fault is 
3000bf7c:	6163 7375 6465 6220 2079 6e49 6176 696c     caused by Invali
3000bf8c:	2064 7865 6563 7470 6f69 206e 6572 7574     d exception retu
3000bf9c:	6e72 5300 6365 7275 7469 2079 6166 6c75     rn.Security faul
3000bfac:	2074 7369 6320 7561 6573 2064 7962 4120     t is caused by A
3000bfbc:	7474 6972 7562 6974 6e6f 7520 696e 2074     ttribution unit 
3000bfcc:	6976 6c6f 7461 6f69 006e 6553 7563 6972     violation.Securi
3000bfdc:	7974 6620 7561 746c 6920 2073 6163 7375     ty fault is caus
3000bfec:	6465 6220 2079 6e49 6176 696c 2064 7274     ed by Invalid tr
3000bffc:	6e61 6973 6974 6e6f 5300 6365 7275 7469     ansition.Securit
3000c00c:	2079 6166 6c75 2074 7369 6320 7561 6573     y fault is cause
3000c01c:	2064 7962 4c20 7a61 2079 7473 7461 2065     d by Lazy state 
3000c02c:	7270 7365 7265 6176 6974 6e6f 6520 7272     preservation err
3000c03c:	726f 5300 6365 7275 7469 2079 6166 6c75     or.Security faul
3000c04c:	2074 7369 6320 7561 6573 2064 7962 4c20     t is caused by L
3000c05c:	7a61 2079 7473 7461 2065 7265 6f72 0072     azy state error.
3000c06c:	6854 2065 6573 7563 6972 7974 6620 7561     The security fau
3000c07c:	746c 6f20 6363 7275 6572 2064 6461 7264     lt occurred addr
3000c08c:	7365 2073 7369 5300 4146 3a52 5b20 7830     ess is.SFAR: [0x
3000c09c:	3825 5d78 2d20 203e 7325 000a 654d 6f6d     %8x] -> %s..Memo
3000c0ac:	7972 6d20 6e61 6761 6d65 6e65 2074 6166     ry management fa
3000c0bc:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c0cc:	6920 736e 7274 6375 6974 6e6f 6120 6363      instruction acc
3000c0dc:	7365 2073 6976 6c6f 7461 6f69 006e 4d4d     ess violation.MM
3000c0ec:	5346 3a52 5b20 7830 3825 5d78 2d20 203e     FSR: [0x%8x] -> 
3000c0fc:	7325 000a 654d 6f6d 7972 6d20 6e61 6761     %s..Memory manag
3000c10c:	6d65 6e65 2074 6166 6c75 2074 7369 6320     ement fault is c
3000c11c:	7561 6573 2064 7962 6420 7461 2061 6361     aused by data ac
3000c12c:	6563 7373 7620 6f69 616c 6974 6e6f 4d00     cess violation.M
3000c13c:	6d65 726f 2079 616d 616e 6567 656d 746e     emory management
3000c14c:	6620 7561 746c 6920 2073 6163 7375 6465      fault is caused
3000c15c:	6220 2079 6e75 7473 6361 696b 676e 6520      by unstacking e
3000c16c:	7272 726f 4d00 6d65 726f 2079 616d 616e     rror.Memory mana
3000c17c:	6567 656d 746e 6620 7561 746c 6920 2073     gement fault is 
3000c18c:	6163 7375 6465 6220 2079 7473 6361 696b     caused by stacki
3000c19c:	676e 6520 7272 726f 4d00 6d65 726f 2079     ng error.Memory 
3000c1ac:	616d 616e 6567 656d 746e 6620 7561 746c     management fault
3000c1bc:	6920 2073 6163 7375 6465 6220 2079 6c66      is caused by fl
3000c1cc:	616f 6974 676e 702d 696f 746e 6c20 7a61     oating-point laz
3000c1dc:	2079 7473 7461 2065 7270 7365 7265 6176     y state preserva
3000c1ec:	6974 6e6f 5400 6568 6d20 6d65 726f 2079     tion.The memory 
3000c1fc:	616d 616e 6567 656d 746e 6620 7561 746c     management fault
3000c20c:	6f20 6363 7275 6572 2064 6461 7264 7365      occurred addres
3000c21c:	2073 7369 4d00 414d 3a52 5b20 7830 3825     s is.MMAR: [0x%8
3000c22c:	5d78 2d20 203e 7325 000a 7542 2073 6166     x] -> %s..Bus fa
3000c23c:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c24c:	6920 736e 7274 6375 6974 6e6f 6120 6363      instruction acc
3000c25c:	7365 2073 6976 6c6f 7461 6f69 006e 4642     ess violation.BF
3000c26c:	5253 203a 305b 2578 7838 205d 3e2d 2520     SR: [0x%8x] -> %
3000c27c:	0a73 4200 7375 6620 7561 746c 6920 2073     s..Bus fault is 
3000c28c:	6163 7375 6465 6220 2079 7270 6365 7369     caused by precis
3000c29c:	2065 6164 6174 6120 6363 7365 2073 6976     e data access vi
3000c2ac:	6c6f 7461 6f69 006e 7542 2073 6166 6c75     olation.Bus faul
3000c2bc:	2074 7369 6320 7561 6573 2064 7962 6920     t is caused by i
3000c2cc:	706d 6572 6963 6573 6420 7461 2061 6361     mprecise data ac
3000c2dc:	6563 7373 7620 6f69 616c 6974 6e6f 4200     cess violation.B
3000c2ec:	7375 6620 7561 746c 6920 2073 6163 7375     us fault is caus
3000c2fc:	6465 6220 2079 6e75 7473 6361 696b 676e     ed by unstacking
3000c30c:	6520 7272 726f 4200 7375 6620 7561 746c      error.Bus fault
3000c31c:	6920 2073 6163 7375 6465 6220 2079 7473      is caused by st
3000c32c:	6361 696b 676e 6520 7272 726f 4200 7375     acking error.Bus
3000c33c:	6620 7561 746c 6920 2073 6163 7375 6465      fault is caused
3000c34c:	6220 2079 6c66 616f 6974 676e 702d 696f      by floating-poi
3000c35c:	746e 6c20 7a61 2079 7473 7461 2065 7270     nt lazy state pr
3000c36c:	7365 7265 6176 6974 6e6f 5400 6568 6220     eservation.The b
3000c37c:	7375 6620 7561 746c 6f20 6363 7275 6572     us fault occurre
3000c38c:	2064 6461 7264 7365 2073 7369 4200 4146     d address is.BFA
3000c39c:	3a52 5b20 7830 3825 5d78 2d20 203e 7325     R: [0x%8x] -> %s
3000c3ac:	000a 7355 6761 2065 6166 6c75 2074 7369     ..Usage fault is
3000c3bc:	6320 7561 6573 2064 7962 6120 7474 6d65      caused by attem
3000c3cc:	7470 2073 6f74 6520 6578 7563 6574 6120     pts to execute a
3000c3dc:	206e 6e75 6564 6966 656e 2064 6e69 7473     n undefined inst
3000c3ec:	7572 7463 6f69 006e 4655 5253 203a 305b     ruction.UFSR: [0
3000c3fc:	2578 7838 205d 3e2d 2520 0a73 5500 6173     x%8x] -> %s..Usa
3000c40c:	6567 6620 7561 746c 6920 2073 6163 7375     ge fault is caus
3000c41c:	6465 6220 2079 7461 6574 706d 7374 7420     ed by attempts t
3000c42c:	206f 7773 7469 6863 7420 206f 6e61 6920     o switch to an i
3000c43c:	766e 6c61 6469 7320 6174 6574 2820 2e65     nvalid state (e.
3000c44c:	2e67 202c 5241 294d 5500 6173 6567 6620     g., ARM).Usage f
3000c45c:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000c46c:	2079 7461 6574 706d 7374 7420 206f 6f64     y attempts to do
3000c47c:	6120 206e 7865 6563 7470 6f69 206e 6977      an exception wi
3000c48c:	6874 6120 6220 6461 7620 6c61 6575 6920     th a bad value i
3000c49c:	206e 6874 2065 5845 5f43 4552 5554 4e52     n the EXC_RETURN
3000c4ac:	6e20 6d75 6562 0072 7355 6761 2065 6166      number.Usage fa
3000c4bc:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c4cc:	6120 7474 6d65 7470 2073 6f74 6520 6578      attempts to exe
3000c4dc:	7563 6574 6120 6320 706f 6f72 6563 7373     cute a coprocess
3000c4ec:	726f 6920 736e 7274 6375 6974 6e6f 5500     or instruction.U
3000c4fc:	6173 6567 6620 7561 746c 6920 2073 6163     sage fault is ca
3000c50c:	7375 6465 6220 2079 6e69 6964 6163 6574     used by indicate
3000c51c:	2073 6874 7461 6120 7320 6174 6b63 6f20     s that a stack o
3000c52c:	6576 6672 6f6c 2077 6828 7261 7764 7261     verflow (hardwar
3000c53c:	2065 6863 6365 296b 6820 7361 7420 6b61     e check) has tak
3000c54c:	6e65 7020 616c 6563 5500 6173 6567 6620     en place.Usage f
3000c55c:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000c56c:	2079 6e69 6964 6163 6574 2073 6874 7461     y indicates that
3000c57c:	6120 206e 6e75 6c61 6769 656e 2064 6361      an unaligned ac
3000c58c:	6563 7373 6620 7561 746c 6820 7361 7420     cess fault has t
3000c59c:	6b61 6e65 7020 616c 6563 5500 6173 6567     aken place.Usage
3000c5ac:	6620 7561 746c 6920 2073 6163 7375 6465      fault is caused
3000c5bc:	6220 2079 6e49 6964 6163 6574 2073 2061      by Indicates a 
3000c5cc:	6964 6976 6564 6220 2079 657a 6f72 6820     divide by zero h
3000c5dc:	7361 7420 6b61 6e65 7020 616c 6563 2820     as taken place (
3000c5ec:	6163 206e 6562 7320 7465 6f20 6c6e 2079     can be set only 
3000c5fc:	6669 4420 5649 305f 545f 5052 6920 2073     if DIV_0_TRP is 
3000c60c:	6573 2974 4400 6265 6775 6620 7561 746c     set).Debug fault
3000c61c:	6920 2073 6163 7375 6465 6220 2079 6168      is caused by ha
3000c62c:	746c 7220 7165 6575 7473 6465 6920 206e     lt requested in 
3000c63c:	564e 4349 4400 5346 3a52 5b20 7830 3825     NVIC.DFSR: [0x%8
3000c64c:	5d78 2d20 203e 7325 000a 6544 7562 2067     x] -> %s..Debug 
3000c65c:	6166 6c75 2074 7369 6320 7561 6573 2064     fault is caused 
3000c66c:	7962 4220 504b 2054 6e69 7473 7572 7463     by BKPT instruct
3000c67c:	6f69 206e 7865 6365 7475 6465 4400 6265     ion executed.Deb
3000c68c:	6775 6620 7561 746c 6920 2073 6163 7375     ug fault is caus
3000c69c:	6465 6220 2079 5744 2054 616d 6374 2068     ed by DWT match 
3000c6ac:	636f 7563 7272 6465 4400 6265 6775 6620     occurred.Debug f
3000c6bc:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000c6cc:	2079 6556 7463 726f 6620 7465 6863 6f20     y Vector fetch o
3000c6dc:	6363 7275 6572 0064 6544 7562 2067 6166     ccurred.Debug fa
3000c6ec:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c6fc:	4520 4244 5247 2051 6973 6e67 6c61 6120      EDBGRQ signal a
3000c70c:	7373 7265 6574 0064 4f42 544f 5b00 7325     sserted.BOOT.[%s
3000c71c:	412d 205d 6148 6472 4620 7561 746c 5020     -A] Hard Fault P
3000c72c:	7461 6863 2820 6553 7563 6572 0d29 000a     atch (Secure)...
3000c73c:	255b 2d73 5d41 4520 6378 7065 6974 6e6f     [%s-A] Exception
3000c74c:	6620 6f72 206d 6573 7563 6572 7720 726f      from secure wor
3000c75c:	646c 0d21 000a 255b 2d73 5d41 4520 6378     ld!...[%s-A] Exc
3000c76c:	7065 6974 6e6f 6620 6f72 206d 6f6e 2d6e     eption from non-
3000c77c:	6573 7563 6572 7720 726f 646c 0d21 000a     secure world!...
3000c78c:	255b 2d73 5d41 5320 5346 2052 3d20 3020     [%s-A] SFSR  = 0
3000c79c:	2578 3830 0d78 000a 255b 2d73 5d41 5320     x%08x...[%s-A] S
3000c7ac:	4146 2052 3d20 3020 2578 3830 0d78 000a     FAR  = 0x%08x...
3000c7bc:	255b 2d73 5d41 5320 4348 5253 3d20 3020     [%s-A] SHCSR = 0
3000c7cc:	2578 3830 0d78 000a 544f 3141 5520 4553     x%08x...OTA1 USE
3000c7dc:	202c 6576 7372 6f69 3a6e 2520 0a64 4f00     , version: %d..O
3000c7ec:	4154 2032 5355 2c45 7620 7265 6973 6e6f     TA2 USE, version
3000c7fc:	203a 6425 000a 6f42 6874 4f20 4154 6920     : %d..Both OTA i
3000c80c:	616d 6567 7620 7265 6669 2079 6166 6c69     mage verify fail
3000c81c:	6465 000a 255b 2d73 5d49 4920 696e 2074     ed..[%s-I] Init 
3000c82c:	4257 0a0d 5b00 7325 492d 205d 6552 6e49     WB...[%s-I] ReIn
3000c83c:	7469 4420 5244 0a0d 6600 616c 6873 735f     it DDR...flash_s
3000c84c:	7a69 3a65 3820 0a4d 6600 616c 6873 735f     ize: 8M..flash_s
3000c85c:	7a69 3a65 3120 4d36 000a 5042 2031 6576     ize: 16M..BP1 ve
3000c86c:	7372 6f69 206e 6e69 6176 696c 0a64 4200     rsion invalid..B
3000c87c:	3150 6620 726f 616d 2074 6e69 6176 696c     P1 format invali
3000c88c:	0a64 4200 3150 6120 7463 7669 2065 6e69     d..BP1 active in
3000c89c:	6564 2078 6e69 6176 696c 0a64 4200 3150     dex invalid..BP1
3000c8ac:	6420 7461 2061 6176 696c 2c64 7620 7265      data valid, ver
3000c8bc:	6973 6e6f 253a 0a64 4200 3250 7620 7265     sion:%d..BP2 ver
3000c8cc:	6973 6e6f 6920 766e 6c61 6469 000a 5042     sion invalid..BP
3000c8dc:	2032 6f66 6d72 7461 6920 766e 6c61 6469     2 format invalid
3000c8ec:	000a 5042 2032 6361 6974 6576 6920 646e     ..BP2 active ind
3000c8fc:	7865 6920 766e 6c61 6469 000a 5042 2032     ex invalid..BP2 
3000c90c:	6164 6174 7620 6c61 6469 202c 6576 7372     data valid, vers
3000c91c:	6f69 3a6e 6425 000a 5042 2031 5243 3343     ion:%d..BP1 CRC3
3000c92c:	2032 616d 6374 2c68 6120 7474 6361 6568     2 match, attache
3000c93c:	2064 5243 3a43 3020 2578 2c78 6320 6c61     d CRC: 0x%x, cal
3000c94c:	7563 616c 6574 2064 5243 3a43 3020 2578     culated CRC: 0x%
3000c95c:	0a78 4200 3150 4320 4352 3233 6e20 746f     x..BP1 CRC32 not
3000c96c:	6d20 7461 6863 202c 7461 6174 6863 6465      match, attached
3000c97c:	4320 4352 203a 7830 7825 202c 6163 636c      CRC: 0x%x, calc
3000c98c:	6c75 7461 6465 4320 4352 203a 7830 7825     ulated CRC: 0x%x
3000c99c:	000a 5042 2032 5243 3343 2032 616d 6374     ..BP2 CRC32 matc
3000c9ac:	2c68 6120 7474 6361 6568 2064 5243 3a43     h, attached CRC:
3000c9bc:	3020 2578 2c78 6320 6c61 7563 616c 6574      0x%x, calculate
3000c9cc:	2064 5243 3a43 3020 2578 0a78 4200 3250     d CRC: 0x%x..BP2
3000c9dc:	4320 4352 3233 6e20 746f 6d20 7461 6863      CRC32 not match
3000c9ec:	202c 7461 6174 6863 6465 4320 4352 203a     , attached CRC: 
3000c9fc:	7830 7825 202c 6163 636c 6c75 7461 6465     0x%x, calculated
3000ca0c:	4320 4352 203a 7830 7825 000a 5042 2031      CRC: 0x%x..BP1 
3000ca1c:	746f 3161 6120 6464 6572 7373 6920 766e     ota1 address inv
3000ca2c:	6c61 6469 000a 5042 2031 746f 3261 6120     alid..BP1 ota2 a
3000ca3c:	6464 6572 7373 6920 766e 6c61 6469 000a     ddress invalid..
3000ca4c:	5042 2032 746f 3161 6120 6464 6572 7373     BP2 ota1 address
3000ca5c:	6920 766e 6c61 6469 000a 5042 2032 746f      invalid..BP2 ot
3000ca6c:	3261 6120 6464 6572 7373 6920 766e 6c61     a2 address inval
3000ca7c:	6469 000a 6f42 6874 4220 2050 5243 2043     id..Both BP CRC 
3000ca8c:	6176 696c 0a64 4200 3150 7320 6c65 6365     valid..BP1 selec
3000ca9c:	6574 0a64 4200 3250 7320 6c65 6365 6574     ted..BP2 selecte
3000caac:	0a64 4200 746f 2068 5042 6920 766e 6c61     d..Both BP inval
3000cabc:	6469 000a 6552 6f63 6576 7972 7320 6174     id..Recovery sta
3000cacc:	7472 000a 6341 6974 6576 4920 646e 7865     rt..Active Index
3000cadc:	203a 6425 000a 544f 3141 6920 766e 6c61     : %d..OTA1 inval
3000caec:	6469 000a 544f 3241 6920 766e 6c61 6469     id..OTA2 invalid
3000cafc:	000a 544f 3141 4320 4352 3233 6d20 7461     ..OTA1 CRC32 mat
3000cb0c:	6863 202c 7461 6174 6863 6465 4320 4352     ch, attached CRC
3000cb1c:	203a 7830 7825 202c 6163 636c 6c75 7461     : 0x%x, calculat
3000cb2c:	6465 4320 4352 203a 7830 7825 000a 544f     ed CRC: 0x%x..OT
3000cb3c:	3141 4320 4352 3233 6e20 746f 6d20 7461     A1 CRC32 not mat
3000cb4c:	6863 202c 7461 6174 6863 6465 4320 4352     ch, attached CRC
3000cb5c:	203a 7830 7825 202c 6163 636c 6c75 7461     : 0x%x, calculat
3000cb6c:	6465 4320 4352 203a 7830 7825 202c 7265     ed CRC: 0x%x, er
3000cb7c:	7361 2065 544f 3141 000a 544f 3241 4320     ase OTA1..OTA2 C
3000cb8c:	4352 3233 6d20 7461 6863 202c 7461 6174     RC32 match, atta
3000cb9c:	6863 6465 4320 4352 203a 7830 7825 202c     ched CRC: 0x%x, 
3000cbac:	6163 636c 6c75 7461 6465 4320 4352 203a     calculated CRC: 
3000cbbc:	7830 7825 000a 544f 3241 4320 4352 3233     0x%x..OTA2 CRC32
3000cbcc:	6e20 746f 6d20 7461 6863 202c 7461 6174      not match, atta
3000cbdc:	6863 6465 4320 4352 203a 7830 7825 202c     ched CRC: 0x%x, 
3000cbec:	6163 636c 6c75 7461 6465 4320 4352 203a     calculated CRC: 
3000cbfc:	7830 7825 202c 7265 7361 2065 544f 3241     0x%x, erase OTA2
3000cc0c:	000a 6341 6974 6576 6920 646e 7865 4f20     ..Active index O
3000cc1c:	4154 6920 766e 6c61 6469 202c 6572 7277     TA invalid, rewr
3000cc2c:	7469 2065 6361 6974 6576 6920 646e 7865     ite active index
3000cc3c:	7420 206f 6425 000a 6c41 206c 544f 2041      to %d..All OTA 
3000cc4c:	6461 7264 7365 6573 2073 6f6e 2074 6176     addresses not va
3000cc5c:	696c 2c64 7220 6365 766f 7265 2079 6166     lid, recovery fa
3000cc6c:	6c69 000a 255b 2d73 5d49 4420 7369 6261     il..[%s-I] Disab
3000cc7c:	656c 4b20 304d 4220 6f6f 2074 6920 206e     le KM0 Boot  in 
3000cc8c:	6554 7473 6d20 646f 0d65 000a 255b 2d73     Test mode...[%s-
3000cc9c:	5d49 4220 7079 7361 2073 544f 2050 7561     I] Bypass OTP au
3000ccac:	6f74 6f6c 6461 0a0d 5b00 7325 492d 205d     toload...[%s-I] 
3000ccbc:	504e 4620 6572 2071 6425 4d20 7a48 000a     NP Freq %d MHz..
3000cccc:	255b 2d73 5d49 4120 2050 7246 7165 2520     [%s-I] AP Freq %
3000ccdc:	2064 484d 0a7a 0a00 6170 7373 6f77 6472     d MHz...password
3000ccec:	7620 7265 6669 2079 6166 6c69 000a 700a      verify fail...p
3000ccfc:	7361 7773 726f 2064 6576 6972 7966 7320     assword verify s
3000cd0c:	6375 6563 7373 000a 4d49 3147 5320 4f42     uccess..IMG1 SBO
3000cd1c:	544f 4f20 4646 000a 4d49 3147 5320 4f42     OT OFF..IMG1 SBO
3000cd2c:	544f 5320 4355 4543 5353 000a 6f62 746f     OT SUCCESS..boot
3000cd3c:	6f6c 6461 7265 765f 7265 6973 6e6f 2520     loader_version %
3000cd4c:	0a73 5b00 4f4d 5544 454c 425f 4f4f 2d54     s..[MODULE_BOOT-
3000cd5c:	454c 4556 5f4c 4e49 4f46 3a5d 4d49 3147     LEVEL_INFO]:IMG1
3000cd6c:	4520 544e 5245 4d20 5053 5b3a 3025 7838      ENTER MSP:[%08x
3000cd7c:	0a5d 5b00 4f4d 5544 454c 425f 4f4f 2d54     ]..[MODULE_BOOT-
3000cd8c:	454c 4556 5f4c 4e49 4f46 3a5d 4d49 3147     LEVEL_INFO]:IMG1
3000cd9c:	5320 4345 5255 2045 5453 5441 3a45 2520      SECURE STATE: %
3000cdac:	0a64 5b00 7325 492d 205d 6e49 7469 5020     d..[%s-I] Init P
3000cdbc:	5253 4d41 0a0d 5b00 7325 572d 205d 6573     SRAM...[%s-W] se
3000cdcc:	2074 6670 206d 6166 6c69 0a0d 5b00 7325     t pfm fail...[%s
3000cddc:	452d 205d 5245 4f52 2152 2021 6853 756f     -E] ERROR!! Shou
3000cdec:	646c 4e20 746f 6520 616e 6c62 2065 654d     ld Not enable Me
3000cdfc:	536d 7277 4f5f 6c6e 2079 6e69 4420 5244     mSwr_Only in DDR
3000ce0c:	4320 6968 2170 2121 0a0d 5b00 4f4d 5544      Chip!!!...[MODU
3000ce1c:	454c 425f 4f4f 2d54 454c 4556 5f4c 4e49     LE_BOOT-LEVEL_IN
3000ce2c:	4f46 3a5d 6e49 7469 4420 5244 0a0d 5300     FO]:Init DDR...S
3000ce3c:	4457 6920 2073 6964 6173 6c62 6465 202c     WD is disabled, 
3000ce4c:	7270 636f 6565 2064 6f74 6420 7369 6261     proceed to disab
3000ce5c:	656c 5520 7261 2074 6f44 6e77 6f6c 6461     le Uart Download
3000ce6c:	2e2e 0a2e 5500 7261 2074 6f44 6e77 6f6c     .....Uart Downlo
3000ce7c:	6461 6820 7361 6220 6565 206e 6964 6173     ad has been disa
3000ce8c:	6c62 6465 000a 6f4c 5567 7261 2074 6142     bled..LogUart Ba
3000ce9c:	6475 6172 6574 203a 6425 000a 530a 4848     udrate: %d...SHH
3000ceac:	544f 4946 2058 6f6e 2074 6572 6563 7669     OTFIX not receiv
3000cebc:	6465 000a 4e0a 726f 616d 206c 6f62 746f     ed...Normal boot
3000cecc:	000a 6962 5f6e 6f6e 313e 2c30 3d20 2520     ..bin_no>10, = %
3000cedc:	0a64 6400 2061 6f6e 2074 6572 6563 7669     d..da not receiv
3000ceec:	6465 000a 680a 746f 6966 2078 6f64 656e     ed...hotfix done
3000cefc:	2e2e 202e 6552 6f62 746f 6e69 2e67 0a2e     ... Rebooting...
3000cf0c:	0000 0000 0208 0000 020c 0000 0210 0000     ................
3000cf1c:	0008 0000 0214 0000 0218 0000 021c 0000     ................
3000cf2c:	000c 0000                                   ....

3000cf30 <__FUNCTION__.0>:
3000cf30:	4f42 544f 535f 434f 435f 6b6c 6553 0074     BOOT_SOC_ClkSet.
3000cf40:	4d4b 2030 4958 2050 4d49 0047 4d4b 2030     KM0 XIP IMG.KM0 
3000cf50:	5253 4d41 4b00 304d 4420 4152 004d 4d4b     SRAM.KM0 DRAM.KM
3000cf60:	2034 4958 2050 4d49 0047 4d4b 2034 5253     4 XIP IMG.KM4 SR
3000cf70:	4d41 4b00 344d 4420 4152 004d 5041 5820     AM.KM4 DRAM.AP X
3000cf80:	5049 4920 474d 4100 2050 4c42 2031 5253     IP IMG.AP BL1 SR
3000cf90:	4d41 4100 2050 4c42 2031 5244 4d41 4100     AM.AP BL1 DRAM.A
3000cfa0:	2050 4946 0050 255b 2d73 5d49 2520 2073     P FIP.[%s-I] %s 
3000cfb0:	6e49 6176 696c 0a64 5b00 7325 492d 205d     Invalid..[%s-I] 
3000cfc0:	7325 255b 3830 3a78 7825 0a5d 5b00 7325     %s[%08x:%x]..[%s
3000cfd0:	492d 205d 4452 2050 4e45 000a 255b 2d73     -I] RDP EN..[%s-
3000cfe0:	5d49 4620 6961 206c 6f74 6c20 616f 2064     I] Fail to load 
3000cff0:	4452 2050 6d69 6761 2165 000a 255b 2d73     RDP image!..[%s-
3000d000:	5d49 5220 5044 4f20 4646 000a 255b 2d73     I] RDP OFF..[%s-
3000d010:	5d49 4920 474d 2032 544f 2046 4e45 000a     I] IMG2 OTF EN..
3000d020:	255b 2d73 5d45 4f20 4654 4d20 646f 2065     [%s-E] OTF Mode 
3000d030:	7265 6f72 0d72 000a 4e49 4156 494c 0044     error...INVALID.
3000d040:	6563 7472 6669 6369 7461 2065 4345 2043     certificate ECC 
3000d050:	6576 6972 7966 6620 6961 2c6c 7220 6c6f     verify fail, rol
3000d060:	626c 6361 0d6b 000a 4d5b 444f 4c55 5f45     lback...[MODULE_
3000d070:	4f42 544f 4c2d 5645 4c45 495f 464e 5d4f     BOOT-LEVEL_INFO]
3000d080:	493a 474d 2832 544f 2541 2964 2520 2c73     :IMG2(OTA%d) %s,
3000d090:	7220 7465 203a 6425 000a 4d49 3247 4520      ret: %d..IMG2 E
3000d0a0:	4343 7620 7265 6669 2079 6166 6c69 202c     CC verify fail, 
3000d0b0:	6f72 6c6c 6162 6b63 0a0d 4f00 4154 415f     rollback...OTA_A
3000d0c0:	2050 4345 2043 6576 6972 7966 6620 6961     P ECC verify fai
3000d0d0:	2c6c 7220 6c6f 626c 6361 0d6b 000a 6f4c     l, rollback...Lo
3000d0e0:	6461 4f20 4154 415f 2050 6d69 6761 2065     ad OTA_AP image 
3000d0f0:	6166 6c69 202c 6f72 6c6c 6162 6b63 0a0d     fail, rollback..
3000d100:	5b00 4f4d 5544 454c 425f 4f4f 2d54 454c     .[MODULE_BOOT-LE
3000d110:	4556 5f4c 5245 4f52 5d52 463a 6961 206c     VEL_ERROR]:Fail 
3000d120:	6f74 6c20 616f 2064 5041 6920 616d 6567     to load AP image
3000d130:	0a21 0000 cf40 3000 cf4c 3000 cf55 3000     !...@..0L..0U..0
3000d140:	cf5e 3000 cf6a 3000 cf73 3000 cf7c 3000     ^..0j..0s..0|..0
3000d150:	cf87 3000 cf93 3000 cf9f 3000 4d4b 2034     ...0...0...0KM4 
3000d160:	4d49 3347 4b00 344d 4e20 4353 4900 474d     IMG3.KM4 NSC.IMG
3000d170:	0031 4d49 3247 4100 2050 4c42 2631 4946     1.IMG2.AP BL1&FI
3000d180:	0050 4d5b 444f 4c55 5f45 4f42 544f 4c2d     P.[MODULE_BOOT-L
3000d190:	5645 4c45 495f 464e 5d4f 253a 5b73 3025     EVEL_INFO]:%s[%0
3000d1a0:	7838 253a 5d78 000a 4452 2050 4146 4c49     8x:%x]..RDP FAIL
3000d1b0:	000a 4452 2050 5553 4343 5345 0a53 b300     ..RDP SUCCESS...
3000d1c0:	9e4e 3905 11ea 0a4b 1b92 ee2a 939c 0029     N..9..K...*...).
3000d1d0:	5041 4220 334c 0032 255b 2d73 5d49 2520     AP BL32.[%s-I] %
3000d1e0:	2073 4556 4952 5946 5020 5341 0a53 5b00     s VERIFY PASS..[
3000d1f0:	7325 452d 205d 7325 5620 5245 4649 2059     %s-E] %s VERIFY 
3000d200:	4146 4c49 202c 6572 2074 203d 6425 000a     FAIL, ret = %d..
3000d210:	255b 2d73 5d45 4320 7265 6974 6966 6163     [%s-E] Certifica
3000d220:	6574 5620 5245 4649 2059 4146 4c49 202c     te VERIFY FAIL, 
3000d230:	6572 2074 203d 6425 000a 0000 d15c 3000     ret = %d....\..0
3000d240:	d165 3000 d16d 3000 d172 3000 d160 3000     e..0m..0r..0`..0
3000d250:	d177 3000                                   w..0

3000d254 <km4_bootloader_rev>:
3000d254:	6d6b 5f34 6f62 746f 6f6c 6461 7265 765f     km4_bootloader_v
3000d264:	7265 325f 6261 3365 6161 3531 5f63 3032     er_2abe3aa15c_20
3000d274:	3432 302f 2f35 3730 312d 3a37 3035 353a     24/05/07-17:50:5
3000d284:	0037 0a0d 6c63 736f 2065 4d79 646f 6d65     7...close yModem
3000d294:	5420 6172 736e 6566 2072 2e2e 0d2e 000a      Transfer ......
3000d2a4:	255b 2d73 5d57 2a20 2a2a 2a2a 2a2a 2a2a     [%s-W] *********
3000d2b4:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d2c4:	202a 4157 4e52 4e49 2047 2a2a 2a2a 2a2a     * WARNING ******
3000d2d4:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d2e4:	0a2a 5b00 7325 572d 205d 202a 2020 2020     *..[%s-W] *     
3000d2f4:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d304:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d314:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d324:	2020 2a20 000a 255b 2d73 5d57 2a20 2020        *..[%s-W] *  
3000d334:	2020 2020 2020 2020 2020 4320 7275 6572                Curre
3000d344:	746e 6320 6968 2070 6576 7372 6f69 206e     nt chip version 
3000d354:	7369 4120 7543 2174 2121 2020 2020 2020     is ACut!!!      
3000d364:	2020 2020 2020 0a2a 5b00 7325 572d 205d           *..[%s-W] 
3000d374:	202a 2020 5452 204b 4453 204b 6f64 7365     *   RTK SDK does
3000d384:	6e20 746f 7320 7075 6f70 7472 6320 7275      not support cur
3000d394:	6572 746e 7620 7265 6973 6e6f 6120 796e     rent version any
3000d3a4:	6d20 726f 2e65 2020 2a20 000a 255b 2d73      more.   *..[%s-
3000d3b4:	5d57 2a20 2020 2020 2020 6c50 6165 6573     W] *      Please
3000d3c4:	6320 6e6f 6174 7463 5220 4b54 5020 204d      contact RTK PM 
3000d3d4:	6f66 2072 6f6d 6572 7020 6f72 7564 7463     for more product
3000d3e4:	6920 666e 216f 2020 2020 2020 0a2a 5b00      info!      *..[
3000d3f4:	7325 572d 205d 2a2a 2a2a 2a2a 2a2a 2a2a     %s-W] **********
3000d404:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d414:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d424:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d434:	000a 255b 2d73 5d57 0a20 5000 534f 5849     ..[%s-W] ..POSIX
3000d444:	2e00                                         ...

3000d447 <_ctype_>:
3000d447:	2000 2020 2020 2020 2020 2828 2828 2028     .         ((((( 
3000d457:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d467:	8820 1010 1010 1010 1010 1010 1010 1010      ...............
3000d477:	0410 0404 0404 0404 0404 1004 1010 1010     ................
3000d487:	1010 4141 4141 4141 0101 0101 0101 0101     ..AAAAAA........
3000d497:	0101 0101 0101 0101 0101 0101 1010 1010     ................
3000d4a7:	1010 4242 4242 4242 0202 0202 0202 0202     ..BBBBBB........
3000d4b7:	0202 0202 0202 0202 0202 0202 1010 1010     ................
3000d4c7:	0020 0000 0000 0000 0000 0000 0000 0000      ...............
	...
