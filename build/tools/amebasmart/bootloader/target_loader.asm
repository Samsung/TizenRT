
/home/bbb/sambashare/remote/tizenrt_8730e_ld/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


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
3000307c:	30008f81 	.word	0x30008f81
30003080:	3000b7dc 	.word	0x3000b7dc
30003084:	3000b8e4 	.word	0x3000b8e4

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
300032b0:	30008f81 	.word	0x30008f81
300032b4:	2001c01c 	.word	0x2001c01c
300032b8:	00009c99 	.word	0x00009c99
300032bc:	30008d99 	.word	0x30008d99
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
300033b4:	30008f81 	.word	0x30008f81
300033b8:	2001c01c 	.word	0x2001c01c
300033bc:	00009c99 	.word	0x00009c99
300033c0:	30008d99 	.word	0x30008d99
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
3000344a:	f007 fd2d 	bl	3000aea8 <rtk_log_write>
3000344e:	2001      	movs	r0, #1
30003450:	f885 8061 	strb.w	r8, [r5, #97]	; 0x61
30003454:	772e      	strb	r6, [r5, #28]
30003456:	776e      	strb	r6, [r5, #29]
30003458:	b007      	add	sp, #28
3000345a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000345e:	bf00      	nop
30003460:	2001c01c 	.word	0x2001c01c
30003464:	3000b982 	.word	0x3000b982
30003468:	3000b98b 	.word	0x3000b98b

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
300034ac:	f007 fcfc 	bl	3000aea8 <rtk_log_write>
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
300034cc:	f007 fcec 	bl	3000aea8 <rtk_log_write>
300034d0:	4620      	mov	r0, r4
300034d2:	e7ee      	b.n	300034b2 <flash_calibration_highspeed+0x46>
300034d4:	2001c01c 	.word	0x2001c01c
300034d8:	30008f81 	.word	0x30008f81
300034dc:	0000a53d 	.word	0x0000a53d
300034e0:	3000b982 	.word	0x3000b982
300034e4:	3000b9ae 	.word	0x3000b9ae
300034e8:	0000c045 	.word	0x0000c045
300034ec:	0000b569 	.word	0x0000b569
300034f0:	3000b9cb 	.word	0x3000b9cb

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
3000353a:	f007 fcb5 	bl	3000aea8 <rtk_log_write>
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
300035e4:	f007 fc60 	bl	3000aea8 <rtk_log_write>
300035e8:	2001      	movs	r0, #1
300035ea:	e7a9      	b.n	30003540 <flash_rx_mode_switch+0x4c>
300035ec:	7770      	strb	r0, [r6, #29]
300035ee:	4620      	mov	r0, r4
300035f0:	47c0      	blx	r8
300035f2:	e7ce      	b.n	30003592 <flash_rx_mode_switch+0x9e>
300035f4:	461c      	mov	r4, r3
300035f6:	e795      	b.n	30003524 <flash_rx_mode_switch+0x30>
300035f8:	3000ba80 	.word	0x3000ba80
300035fc:	30008f81 	.word	0x30008f81
30003600:	2001c01c 	.word	0x2001c01c
30003604:	3000b982 	.word	0x3000b982
30003608:	3000ba00 	.word	0x3000ba00
3000360c:	00009db5 	.word	0x00009db5
30003610:	0000a149 	.word	0x0000a149
30003614:	e000ed00 	.word	0xe000ed00
30003618:	30008d99 	.word	0x30008d99
3000361c:	00054060 	.word	0x00054060
30003620:	000129bd 	.word	0x000129bd
30003624:	3000b9ea 	.word	0x3000b9ea
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
300036b0:	f007 fbfa 	bl	3000aea8 <rtk_log_write>
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
300036d8:	f007 fbe6 	bl	3000aea8 <rtk_log_write>
300036dc:	f240 2127 	movw	r1, #551	; 0x227
300036e0:	487c      	ldr	r0, [pc, #496]	; (300038d4 <flash_highspeed_setup+0x2a4>)
300036e2:	f007 fe35 	bl	3000b350 <__io_assert_failed_veneer>
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
300037ca:	f007 fdc1 	bl	3000b350 <__io_assert_failed_veneer>
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
3000382c:	f007 fb3c 	bl	3000aea8 <rtk_log_write>
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
30003854:	f007 fb28 	bl	3000aea8 <rtk_log_write>
30003858:	f240 215e 	movw	r1, #606	; 0x25e
3000385c:	4827      	ldr	r0, [pc, #156]	; (300038fc <flash_highspeed_setup+0x2cc>)
3000385e:	f007 fd77 	bl	3000b350 <__io_assert_failed_veneer>
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
300038b4:	3000b8e0 	.word	0x3000b8e0
300038b8:	3000b8e2 	.word	0x3000b8e2
300038bc:	2001c01c 	.word	0x2001c01c
300038c0:	3000eb44 	.word	0x3000eb44
300038c4:	30008f81 	.word	0x30008f81
300038c8:	3000ba24 	.word	0x3000ba24
300038cc:	3000b982 	.word	0x3000b982
300038d0:	3000ba3f 	.word	0x3000ba3f
300038d4:	3000baa4 	.word	0x3000baa4
300038d8:	0000c1d9 	.word	0x0000c1d9
300038dc:	000129bd 	.word	0x000129bd
300038e0:	0000c0f9 	.word	0x0000c0f9
300038e4:	0000a4c1 	.word	0x0000a4c1
300038e8:	0000a441 	.word	0x0000a441
300038ec:	0000a3c5 	.word	0x0000a3c5
300038f0:	0000a345 	.word	0x0000a345
300038f4:	3000eb41 	.word	0x3000eb41
300038f8:	3000ba69 	.word	0x3000ba69
300038fc:	3000ba94 	.word	0x3000ba94
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
3000392e:	f007 fd0f 	bl	3000b350 <__io_assert_failed_veneer>
30003932:	6833      	ldr	r3, [r6, #0]
30003934:	691b      	ldr	r3, [r3, #16]
30003936:	4798      	blx	r3
30003938:	e7a0      	b.n	3000387c <flash_highspeed_setup+0x24c>
3000393a:	bf00      	nop
3000393c:	3000ba94 	.word	0x3000ba94

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
300039f2:	f003 fb3d 	bl	30007070 <ymodem_uart_getdata>
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
30003a2c:	f007 fa3c 	bl	3000aea8 <rtk_log_write>
30003a30:	2244      	movs	r2, #68	; 0x44
30003a32:	2100      	movs	r1, #0
30003a34:	a803      	add	r0, sp, #12
30003a36:	f007 fc9b 	bl	3000b370 <____wrap_memset_veneer>
30003a3a:	0662      	lsls	r2, r4, #25
30003a3c:	4b2f      	ldr	r3, [pc, #188]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003a3e:	d54c      	bpl.n	30003ada <BOOT_FaultHandler+0xc2>
30003a40:	4619      	mov	r1, r3
30003a42:	4a30      	ldr	r2, [pc, #192]	; (30003b04 <BOOT_FaultHandler+0xec>)
30003a44:	2001      	movs	r0, #1
30003a46:	f007 fa2f 	bl	3000aea8 <rtk_log_write>
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
30003a8e:	f006 ff85 	bl	3000a99c <crash_dump>
30003a92:	4a1e      	ldr	r2, [pc, #120]	; (30003b0c <BOOT_FaultHandler+0xf4>)
30003a94:	f8d5 30e4 	ldr.w	r3, [r5, #228]	; 0xe4
30003a98:	2001      	movs	r0, #1
30003a9a:	9300      	str	r3, [sp, #0]
30003a9c:	4b17      	ldr	r3, [pc, #92]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003a9e:	4619      	mov	r1, r3
30003aa0:	f007 fa02 	bl	3000aea8 <rtk_log_write>
30003aa4:	f8d5 30e8 	ldr.w	r3, [r5, #232]	; 0xe8
30003aa8:	4a19      	ldr	r2, [pc, #100]	; (30003b10 <BOOT_FaultHandler+0xf8>)
30003aaa:	2001      	movs	r0, #1
30003aac:	9300      	str	r3, [sp, #0]
30003aae:	4b13      	ldr	r3, [pc, #76]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003ab0:	4619      	mov	r1, r3
30003ab2:	f007 f9f9 	bl	3000aea8 <rtk_log_write>
30003ab6:	6a6b      	ldr	r3, [r5, #36]	; 0x24
30003ab8:	4a16      	ldr	r2, [pc, #88]	; (30003b14 <BOOT_FaultHandler+0xfc>)
30003aba:	2001      	movs	r0, #1
30003abc:	9300      	str	r3, [sp, #0]
30003abe:	4b0f      	ldr	r3, [pc, #60]	; (30003afc <BOOT_FaultHandler+0xe4>)
30003ac0:	4619      	mov	r1, r3
30003ac2:	f007 f9f1 	bl	3000aea8 <rtk_log_write>
30003ac6:	f3ef 8088 	mrs	r0, MSP_NS
30003aca:	f3ef 8189 	mrs	r1, PSP_NS
30003ace:	463b      	mov	r3, r7
30003ad0:	4622      	mov	r2, r4
30003ad2:	f007 fc1d 	bl	3000b310 <__INT_HardFault_C_veneer>
30003ad6:	b015      	add	sp, #84	; 0x54
30003ad8:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003ada:	4a0f      	ldr	r2, [pc, #60]	; (30003b18 <BOOT_FaultHandler+0x100>)
30003adc:	4619      	mov	r1, r3
30003ade:	2001      	movs	r0, #1
30003ae0:	f007 f9e2 	bl	3000aea8 <rtk_log_write>
30003ae4:	f3ef 8394 	mrs	r3, CONTROL_NS
30003ae8:	f3ef 8188 	mrs	r1, MSP_NS
30003aec:	f3ef 8589 	mrs	r5, PSP_NS
30003af0:	f014 0208 	ands.w	r2, r4, #8
30003af4:	bf18      	it	ne
30003af6:	f3c3 0240 	ubfxne	r2, r3, #1, #1
30003afa:	e7ad      	b.n	30003a58 <BOOT_FaultHandler+0x40>
30003afc:	3000c79c 	.word	0x3000c79c
30003b00:	3000c7a1 	.word	0x3000c7a1
30003b04:	3000c7c4 	.word	0x3000c7c4
30003b08:	e000ed00 	.word	0xe000ed00
30003b0c:	3000c814 	.word	0x3000c814
30003b10:	3000c82c 	.word	0x3000c82c
30003b14:	3000c844 	.word	0x3000c844
30003b18:	3000c7ea 	.word	0x3000c7ea

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
30003cc8:	3000d78c 	.word	0x3000d78c

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
30003cea:	f007 fb61 	bl	3000b3b0 <____wrap_memcpy_veneer>
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
30003d7c:	f007 fad8 	bl	3000b330 <__DiagPrintf_veneer>
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
30003d9e:	f007 fac7 	bl	3000b330 <__DiagPrintf_veneer>
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
30003dca:	f007 fab1 	bl	3000b330 <__DiagPrintf_veneer>
30003dce:	e7fe      	b.n	30003dce <rewrite_bp+0x102>
30003dd0:	3000d78c 	.word	0x3000d78c
30003dd4:	3000c85c 	.word	0x3000c85c
30003dd8:	3000c873 	.word	0x3000c873
30003ddc:	3000c88a 	.word	0x3000c88a

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
30003ea4:	f007 fa64 	bl	3000b370 <____wrap_memset_veneer>
30003ea8:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
30003eac:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30003eb0:	2210      	movs	r2, #16
30003eb2:	2100      	movs	r1, #0
30003eb4:	a80c      	add	r0, sp, #48	; 0x30
30003eb6:	f007 fa5b 	bl	3000b370 <____wrap_memset_veneer>
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
30003f4c:	3000cf84 	.word	0x3000cf84
30003f50:	3000d5d0 	.word	0x3000d5d0
30003f54:	00009b2d 	.word	0x00009b2d
30003f58:	30007335 	.word	0x30007335

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
30003fb0:	3000ddf0 	.word	0x3000ddf0
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
30004268:	3000dd2c 	.word	0x3000dd2c
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
30004324:	30007421 	.word	0x30007421
30004328:	300078a1 	.word	0x300078a1
3000432c:	3000a415 	.word	0x3000a415
30004330:	30007931 	.word	0x30007931
30004334:	30007935 	.word	0x30007935
30004338:	3000a3c5 	.word	0x3000a3c5

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
3000438e:	f006 fd8b 	bl	3000aea8 <rtk_log_write>
30004392:	4b0f      	ldr	r3, [pc, #60]	; (300043d0 <BOOT_PSRAM_Init+0x94>)
30004394:	e7e1      	b.n	3000435a <BOOT_PSRAM_Init+0x1e>
30004396:	4b0f      	ldr	r3, [pc, #60]	; (300043d4 <BOOT_PSRAM_Init+0x98>)
30004398:	4798      	blx	r3
3000439a:	686b      	ldr	r3, [r5, #4]
3000439c:	6023      	str	r3, [r4, #0]
3000439e:	e7e7      	b.n	30004370 <BOOT_PSRAM_Init+0x34>
300043a0:	300081a5 	.word	0x300081a5
300043a4:	300081c1 	.word	0x300081c1
300043a8:	30008225 	.word	0x30008225
300043ac:	300072e9 	.word	0x300072e9
300043b0:	300083f5 	.word	0x300083f5
300043b4:	42008000 	.word	0x42008000
300043b8:	3000ec58 	.word	0x3000ec58
300043bc:	41002000 	.word	0x41002000
300043c0:	3000eb50 	.word	0x3000eb50
300043c4:	30008791 	.word	0x30008791
300043c8:	3000c79c 	.word	0x3000c79c
300043cc:	3000c8a8 	.word	0x3000c8a8
300043d0:	300084bd 	.word	0x300084bd
300043d4:	300084f9 	.word	0x300084f9

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
3000442c:	3000ebec 	.word	0x3000ebec

30004430 <BOOT_SCBVTORReFill_HP>:
30004430:	4b02      	ldr	r3, [pc, #8]	; (3000443c <BOOT_SCBVTORReFill_HP+0xc>)
30004432:	699a      	ldr	r2, [r3, #24]
30004434:	4b02      	ldr	r3, [pc, #8]	; (30004440 <BOOT_SCBVTORReFill_HP+0x10>)
30004436:	609a      	str	r2, [r3, #8]
30004438:	4770      	bx	lr
3000443a:	bf00      	nop
3000443c:	3000ebec 	.word	0x3000ebec
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
300044a8:	3000ebec 	.word	0x3000ebec
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
300044dc:	3000ec08 	.word	0x3000ec08
300044e0:	e000e100 	.word	0xe000e100
300044e4:	3000ebec 	.word	0x3000ebec

300044e8 <BOOT_WakeFromPG>:
300044e8:	b508      	push	{r3, lr}
300044ea:	f002 fb29 	bl	30006b40 <BOOT_RAM_TZCfg>
300044ee:	4b16      	ldr	r3, [pc, #88]	; (30004548 <BOOT_WakeFromPG+0x60>)
300044f0:	699b      	ldr	r3, [r3, #24]
300044f2:	b10b      	cbz	r3, 300044f8 <BOOT_WakeFromPG+0x10>
300044f4:	f7ff ff9c 	bl	30004430 <BOOT_SCBVTORReFill_HP>
300044f8:	f7ff ffda 	bl	300044b0 <BOOT_NVICReFill_HP>
300044fc:	f7ff ff6c 	bl	300043d8 <BOOT_SCBConfig_HP>
30004500:	f7ff fd2c 	bl	30003f5c <BOOT_CACHEWRR_Set>
30004504:	4b11      	ldr	r3, [pc, #68]	; (3000454c <BOOT_WakeFromPG+0x64>)
30004506:	6818      	ldr	r0, [r3, #0]
30004508:	f7ff fd56 	bl	30003fb8 <BOOT_TCMSet>
3000450c:	2201      	movs	r2, #1
3000450e:	4910      	ldr	r1, [pc, #64]	; (30004550 <BOOT_WakeFromPG+0x68>)
30004510:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30004514:	4b0f      	ldr	r3, [pc, #60]	; (30004554 <BOOT_WakeFromPG+0x6c>)
30004516:	4798      	blx	r3
30004518:	4b0f      	ldr	r3, [pc, #60]	; (30004558 <BOOT_WakeFromPG+0x70>)
3000451a:	4798      	blx	r3
3000451c:	4a0f      	ldr	r2, [pc, #60]	; (3000455c <BOOT_WakeFromPG+0x74>)
3000451e:	4910      	ldr	r1, [pc, #64]	; (30004560 <BOOT_WakeFromPG+0x78>)
30004520:	6893      	ldr	r3, [r2, #8]
30004522:	6852      	ldr	r2, [r2, #4]
30004524:	605a      	str	r2, [r3, #4]
30004526:	4a0f      	ldr	r2, [pc, #60]	; (30004564 <BOOT_WakeFromPG+0x7c>)
30004528:	6093      	str	r3, [r2, #8]
3000452a:	4a0f      	ldr	r2, [pc, #60]	; (30004568 <BOOT_WakeFromPG+0x80>)
3000452c:	61d1      	str	r1, [r2, #28]
3000452e:	4a0f      	ldr	r2, [pc, #60]	; (3000456c <BOOT_WakeFromPG+0x84>)
30004530:	f382 8888 	msr	MSP_NS, r2
30004534:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
30004538:	f382 8809 	msr	PSP, r2
3000453c:	6858      	ldr	r0, [r3, #4]
3000453e:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
30004542:	f7ff bc51 	b.w	30003de8 <BOOT_NsStart>
30004546:	bf00      	nop
30004548:	3000ebec 	.word	0x3000ebec
3000454c:	3000d5c8 	.word	0x3000d5c8
30004550:	40001000 	.word	0x40001000
30004554:	0000b479 	.word	0x0000b479
30004558:	0000d835 	.word	0x0000d835
3000455c:	60000020 	.word	0x60000020
30004560:	30003941 	.word	0x30003941
30004564:	e002ed00 	.word	0xe002ed00
30004568:	30000000 	.word	0x30000000
3000456c:	2001bffc 	.word	0x2001bffc

30004570 <BOOT_ImgCopy>:
30004570:	b513      	push	{r0, r1, r4, lr}
30004572:	4b0a      	ldr	r3, [pc, #40]	; (3000459c <BOOT_ImgCopy+0x2c>)
30004574:	4604      	mov	r4, r0
30004576:	e9cd 2100 	strd	r2, r1, [sp]
3000457a:	4798      	blx	r3
3000457c:	e9dd 2100 	ldrd	r2, r1, [sp]
30004580:	b128      	cbz	r0, 3000458e <BOOT_ImgCopy+0x1e>
30004582:	4620      	mov	r0, r4
30004584:	4b06      	ldr	r3, [pc, #24]	; (300045a0 <BOOT_ImgCopy+0x30>)
30004586:	b002      	add	sp, #8
30004588:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000458c:	4718      	bx	r3
3000458e:	4620      	mov	r0, r4
30004590:	b002      	add	sp, #8
30004592:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004596:	f003 bafb 	b.w	30007b90 <NandImgCopy>
3000459a:	bf00      	nop
3000459c:	30008f81 	.word	0x30008f81
300045a0:	00012a1d 	.word	0x00012a1d

300045a4 <BOOT_LoadImages>:
300045a4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300045a8:	ed2d 8b08 	vpush	{d8-d11}
300045ac:	b0af      	sub	sp, #188	; 0xbc
300045ae:	f04f 0900 	mov.w	r9, #0
300045b2:	2104      	movs	r1, #4
300045b4:	a809      	add	r0, sp, #36	; 0x24
300045b6:	f8cd 900c 	str.w	r9, [sp, #12]
300045ba:	f7ff fafd 	bl	30003bb8 <flash_read_id>
300045be:	f89d 3026 	ldrb.w	r3, [sp, #38]	; 0x26
300045c2:	2201      	movs	r2, #1
300045c4:	46ca      	mov	sl, r9
300045c6:	fa02 f803 	lsl.w	r8, r2, r3
300045ca:	f5b8 0f00 	cmp.w	r8, #8388608	; 0x800000
300045ce:	f040 80ab 	bne.w	30004728 <BOOT_LoadImages+0x184>
300045d2:	48b4      	ldr	r0, [pc, #720]	; (300048a4 <BOOT_LoadImages+0x300>)
300045d4:	f006 feac 	bl	3000b330 <__DiagPrintf_veneer>
300045d8:	f108 66ff 	add.w	r6, r8, #133693440	; 0x7f80000
300045dc:	f04f 0b00 	mov.w	fp, #0
300045e0:	48b1      	ldr	r0, [pc, #708]	; (300048a8 <BOOT_LoadImages+0x304>)
300045e2:	f506 25fe 	add.w	r5, r6, #520192	; 0x7f000
300045e6:	f506 26fc 	add.w	r6, r6, #516096	; 0x7e000
300045ea:	e9cd bb0a 	strd	fp, fp, [sp, #40]	; 0x28
300045ee:	e9cd bb0c 	strd	fp, fp, [sp, #48]	; 0x30
300045f2:	f7ff fb0c 	bl	30003c0e <mcc_init_crc32>
300045f6:	7b33      	ldrb	r3, [r6, #12]
300045f8:	7b2f      	ldrb	r7, [r5, #12]
300045fa:	9304      	str	r3, [sp, #16]
300045fc:	6873      	ldr	r3, [r6, #4]
300045fe:	9305      	str	r3, [sp, #20]
30004600:	686b      	ldr	r3, [r5, #4]
30004602:	68ac      	ldr	r4, [r5, #8]
30004604:	9306      	str	r3, [sp, #24]
30004606:	9b05      	ldr	r3, [sp, #20]
30004608:	3b01      	subs	r3, #1
3000460a:	3303      	adds	r3, #3
3000460c:	f240 8092 	bls.w	30004734 <BOOT_LoadImages+0x190>
30004610:	48a6      	ldr	r0, [pc, #664]	; (300048ac <BOOT_LoadImages+0x308>)
30004612:	f006 fe8d 	bl	3000b330 <__DiagPrintf_veneer>
30004616:	9b06      	ldr	r3, [sp, #24]
30004618:	3b01      	subs	r3, #1
3000461a:	3303      	adds	r3, #3
3000461c:	f240 809c 	bls.w	30004758 <BOOT_LoadImages+0x1b4>
30004620:	48a3      	ldr	r0, [pc, #652]	; (300048b0 <BOOT_LoadImages+0x30c>)
30004622:	2400      	movs	r4, #0
30004624:	f006 fe84 	bl	3000b330 <__DiagPrintf_veneer>
30004628:	f8d6 3011 	ldr.w	r3, [r6, #17]
3000462c:	f8d6 700d 	ldr.w	r7, [r6, #13]
30004630:	ee08 3a10 	vmov	s16, r3
30004634:	f8d5 300d 	ldr.w	r3, [r5, #13]
30004638:	ee08 3a90 	vmov	s17, r3
3000463c:	f8d5 3011 	ldr.w	r3, [r5, #17]
30004640:	ee09 3a10 	vmov	s18, r3
30004644:	f1bb 0f00 	cmp.w	fp, #0
30004648:	f000 8099 	beq.w	3000477e <BOOT_LoadImages+0x1da>
3000464c:	4899      	ldr	r0, [pc, #612]	; (300048b4 <BOOT_LoadImages+0x310>)
3000464e:	f640 71fc 	movw	r1, #4092	; 0xffc
30004652:	f8d6 b000 	ldr.w	fp, [r6]
30004656:	4a94      	ldr	r2, [pc, #592]	; (300048a8 <BOOT_LoadImages+0x304>)
30004658:	4440      	add	r0, r8
3000465a:	f7ff fb05 	bl	30003c68 <mcc_crc32>
3000465e:	4583      	cmp	fp, r0
30004660:	4602      	mov	r2, r0
30004662:	f040 8088 	bne.w	30004776 <BOOT_LoadImages+0x1d2>
30004666:	465a      	mov	r2, fp
30004668:	4659      	mov	r1, fp
3000466a:	4893      	ldr	r0, [pc, #588]	; (300048b8 <BOOT_LoadImages+0x314>)
3000466c:	f006 fe60 	bl	3000b330 <__DiagPrintf_veneer>
30004670:	b1ac      	cbz	r4, 3000469e <BOOT_LoadImages+0xfa>
30004672:	f108 6000 	add.w	r0, r8, #134217728	; 0x8000000
30004676:	f8d5 b000 	ldr.w	fp, [r5]
3000467a:	4a8b      	ldr	r2, [pc, #556]	; (300048a8 <BOOT_LoadImages+0x304>)
3000467c:	f640 71fc 	movw	r1, #4092	; 0xffc
30004680:	f6a0 70fc 	subw	r0, r0, #4092	; 0xffc
30004684:	f7ff faf0 	bl	30003c68 <mcc_crc32>
30004688:	4583      	cmp	fp, r0
3000468a:	4602      	mov	r2, r0
3000468c:	d17b      	bne.n	30004786 <BOOT_LoadImages+0x1e2>
3000468e:	465a      	mov	r2, fp
30004690:	4659      	mov	r1, fp
30004692:	488a      	ldr	r0, [pc, #552]	; (300048bc <BOOT_LoadImages+0x318>)
30004694:	f006 fe4c 	bl	3000b330 <__DiagPrintf_veneer>
30004698:	2c00      	cmp	r4, #0
3000469a:	f000 8087 	beq.w	300047ac <BOOT_LoadImages+0x208>
3000469e:	7b33      	ldrb	r3, [r6, #12]
300046a0:	9304      	str	r3, [sp, #16]
300046a2:	f108 6300 	add.w	r3, r8, #134217728	; 0x8000000
300046a6:	42bb      	cmp	r3, r7
300046a8:	d302      	bcc.n	300046b0 <BOOT_LoadImages+0x10c>
300046aa:	f1b7 6f00 	cmp.w	r7, #134217728	; 0x8000000
300046ae:	d272      	bcs.n	30004796 <BOOT_LoadImages+0x1f2>
300046b0:	4883      	ldr	r0, [pc, #524]	; (300048c0 <BOOT_LoadImages+0x31c>)
300046b2:	f006 fe3d 	bl	3000b330 <__DiagPrintf_veneer>
300046b6:	2c00      	cmp	r4, #0
300046b8:	d177      	bne.n	300047aa <BOOT_LoadImages+0x206>
300046ba:	4882      	ldr	r0, [pc, #520]	; (300048c4 <BOOT_LoadImages+0x320>)
300046bc:	f006 fe38 	bl	3000b330 <__DiagPrintf_veneer>
300046c0:	4881      	ldr	r0, [pc, #516]	; (300048c8 <BOOT_LoadImages+0x324>)
300046c2:	f006 fe35 	bl	3000b330 <__DiagPrintf_veneer>
300046c6:	aa0a      	add	r2, sp, #40	; 0x28
300046c8:	4641      	mov	r1, r8
300046ca:	4638      	mov	r0, r7
300046cc:	f7ff fadd 	bl	30003c8a <recovery_check>
300046d0:	aa0b      	add	r2, sp, #44	; 0x2c
300046d2:	4641      	mov	r1, r8
300046d4:	9004      	str	r0, [sp, #16]
300046d6:	ee18 0a10 	vmov	r0, s16
300046da:	f7ff fad6 	bl	30003c8a <recovery_check>
300046de:	aa0c      	add	r2, sp, #48	; 0x30
300046e0:	4683      	mov	fp, r0
300046e2:	4641      	mov	r1, r8
300046e4:	ee18 0a90 	vmov	r0, s17
300046e8:	f7ff facf 	bl	30003c8a <recovery_check>
300046ec:	aa0d      	add	r2, sp, #52	; 0x34
300046ee:	4604      	mov	r4, r0
300046f0:	4641      	mov	r1, r8
300046f2:	ee19 0a10 	vmov	r0, s18
300046f6:	f7ff fac8 	bl	30003c8a <recovery_check>
300046fa:	9b04      	ldr	r3, [sp, #16]
300046fc:	4304      	orrs	r4, r0
300046fe:	ea53 0b0b 	orrs.w	fp, r3, fp
30004702:	f040 81b1 	bne.w	30004a68 <BOOT_LoadImages+0x4c4>
30004706:	2c00      	cmp	r4, #0
30004708:	f000 8214 	beq.w	30004b34 <BOOT_LoadImages+0x590>
3000470c:	ee18 1a90 	vmov	r1, s17
30004710:	e9dd 230c 	ldrd	r2, r3, [sp, #48]	; 0x30
30004714:	e9cd 5b00 	strd	r5, fp, [sp]
30004718:	429a      	cmp	r2, r3
3000471a:	4633      	mov	r3, r6
3000471c:	ee19 2a10 	vmov	r2, s18
30004720:	f080 81b6 	bcs.w	30004a90 <BOOT_LoadImages+0x4ec>
30004724:	2001      	movs	r0, #1
30004726:	e1b4      	b.n	30004a92 <BOOT_LoadImages+0x4ee>
30004728:	f1b8 7f80 	cmp.w	r8, #16777216	; 0x1000000
3000472c:	f47f af54 	bne.w	300045d8 <BOOT_LoadImages+0x34>
30004730:	4866      	ldr	r0, [pc, #408]	; (300048cc <BOOT_LoadImages+0x328>)
30004732:	e74f      	b.n	300045d4 <BOOT_LoadImages+0x30>
30004734:	68b3      	ldr	r3, [r6, #8]
30004736:	3b01      	subs	r3, #1
30004738:	3303      	adds	r3, #3
3000473a:	d901      	bls.n	30004740 <BOOT_LoadImages+0x19c>
3000473c:	4864      	ldr	r0, [pc, #400]	; (300048d0 <BOOT_LoadImages+0x32c>)
3000473e:	e768      	b.n	30004612 <BOOT_LoadImages+0x6e>
30004740:	9b04      	ldr	r3, [sp, #16]
30004742:	2b01      	cmp	r3, #1
30004744:	d901      	bls.n	3000474a <BOOT_LoadImages+0x1a6>
30004746:	4863      	ldr	r0, [pc, #396]	; (300048d4 <BOOT_LoadImages+0x330>)
30004748:	e763      	b.n	30004612 <BOOT_LoadImages+0x6e>
3000474a:	9905      	ldr	r1, [sp, #20]
3000474c:	f04f 0b01 	mov.w	fp, #1
30004750:	4861      	ldr	r0, [pc, #388]	; (300048d8 <BOOT_LoadImages+0x334>)
30004752:	f006 fded 	bl	3000b330 <__DiagPrintf_veneer>
30004756:	e75e      	b.n	30004616 <BOOT_LoadImages+0x72>
30004758:	3c01      	subs	r4, #1
3000475a:	3403      	adds	r4, #3
3000475c:	d901      	bls.n	30004762 <BOOT_LoadImages+0x1be>
3000475e:	485f      	ldr	r0, [pc, #380]	; (300048dc <BOOT_LoadImages+0x338>)
30004760:	e75f      	b.n	30004622 <BOOT_LoadImages+0x7e>
30004762:	2f01      	cmp	r7, #1
30004764:	d901      	bls.n	3000476a <BOOT_LoadImages+0x1c6>
30004766:	485e      	ldr	r0, [pc, #376]	; (300048e0 <BOOT_LoadImages+0x33c>)
30004768:	e75b      	b.n	30004622 <BOOT_LoadImages+0x7e>
3000476a:	9906      	ldr	r1, [sp, #24]
3000476c:	2401      	movs	r4, #1
3000476e:	485d      	ldr	r0, [pc, #372]	; (300048e4 <BOOT_LoadImages+0x340>)
30004770:	f006 fdde 	bl	3000b330 <__DiagPrintf_veneer>
30004774:	e758      	b.n	30004628 <BOOT_LoadImages+0x84>
30004776:	4659      	mov	r1, fp
30004778:	485b      	ldr	r0, [pc, #364]	; (300048e8 <BOOT_LoadImages+0x344>)
3000477a:	f006 fdd9 	bl	3000b330 <__DiagPrintf_veneer>
3000477e:	2c00      	cmp	r4, #0
30004780:	d09b      	beq.n	300046ba <BOOT_LoadImages+0x116>
30004782:	2400      	movs	r4, #0
30004784:	e775      	b.n	30004672 <BOOT_LoadImages+0xce>
30004786:	4659      	mov	r1, fp
30004788:	4858      	ldr	r0, [pc, #352]	; (300048ec <BOOT_LoadImages+0x348>)
3000478a:	f006 fdd1 	bl	3000b330 <__DiagPrintf_veneer>
3000478e:	2c00      	cmp	r4, #0
30004790:	d093      	beq.n	300046ba <BOOT_LoadImages+0x116>
30004792:	2400      	movs	r4, #0
30004794:	e783      	b.n	3000469e <BOOT_LoadImages+0xfa>
30004796:	ee18 2a10 	vmov	r2, s16
3000479a:	4293      	cmp	r3, r2
3000479c:	d303      	bcc.n	300047a6 <BOOT_LoadImages+0x202>
3000479e:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300047a2:	f080 82d9 	bcs.w	30004d58 <BOOT_LoadImages+0x7b4>
300047a6:	4852      	ldr	r0, [pc, #328]	; (300048f0 <BOOT_LoadImages+0x34c>)
300047a8:	e783      	b.n	300046b2 <BOOT_LoadImages+0x10e>
300047aa:	2400      	movs	r4, #0
300047ac:	f108 6300 	add.w	r3, r8, #134217728	; 0x8000000
300047b0:	ee18 2a90 	vmov	r2, s17
300047b4:	4293      	cmp	r3, r2
300047b6:	d302      	bcc.n	300047be <BOOT_LoadImages+0x21a>
300047b8:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300047bc:	d210      	bcs.n	300047e0 <BOOT_LoadImages+0x23c>
300047be:	484d      	ldr	r0, [pc, #308]	; (300048f4 <BOOT_LoadImages+0x350>)
300047c0:	f006 fdb6 	bl	3000b330 <__DiagPrintf_veneer>
300047c4:	2c00      	cmp	r4, #0
300047c6:	f43f af78 	beq.w	300046ba <BOOT_LoadImages+0x116>
300047ca:	484b      	ldr	r0, [pc, #300]	; (300048f8 <BOOT_LoadImages+0x354>)
300047cc:	f006 fdb0 	bl	3000b330 <__DiagPrintf_veneer>
300047d0:	4b35      	ldr	r3, [pc, #212]	; (300048a8 <BOOT_LoadImages+0x304>)
300047d2:	ee18 2a10 	vmov	r2, s16
300047d6:	f8c3 7400 	str.w	r7, [r3, #1024]	; 0x400
300047da:	f8c3 2404 	str.w	r2, [r3, #1028]	; 0x404
300047de:	e01a      	b.n	30004816 <BOOT_LoadImages+0x272>
300047e0:	ee19 2a10 	vmov	r2, s18
300047e4:	4293      	cmp	r3, r2
300047e6:	d303      	bcc.n	300047f0 <BOOT_LoadImages+0x24c>
300047e8:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300047ec:	f080 8254 	bcs.w	30004c98 <BOOT_LoadImages+0x6f4>
300047f0:	4842      	ldr	r0, [pc, #264]	; (300048fc <BOOT_LoadImages+0x358>)
300047f2:	e7e5      	b.n	300047c0 <BOOT_LoadImages+0x21c>
300047f4:	4842      	ldr	r0, [pc, #264]	; (30004900 <BOOT_LoadImages+0x35c>)
300047f6:	f006 fd9b 	bl	3000b330 <__DiagPrintf_veneer>
300047fa:	e9dd 3205 	ldrd	r3, r2, [sp, #20]
300047fe:	4293      	cmp	r3, r2
30004800:	f0c0 8251 	bcc.w	30004ca6 <BOOT_LoadImages+0x702>
30004804:	483c      	ldr	r0, [pc, #240]	; (300048f8 <BOOT_LoadImages+0x354>)
30004806:	f006 fd93 	bl	3000b330 <__DiagPrintf_veneer>
3000480a:	ee18 3a10 	vmov	r3, s16
3000480e:	f8cb 7400 	str.w	r7, [fp, #1024]	; 0x400
30004812:	f8cb 3404 	str.w	r3, [fp, #1028]	; 0x404
30004816:	f8dd 9010 	ldr.w	r9, [sp, #16]
3000481a:	4635      	mov	r5, r6
3000481c:	e24e      	b.n	30004cbc <BOOT_LoadImages+0x718>
3000481e:	f04f 0a00 	mov.w	sl, #0
30004822:	4656      	mov	r6, sl
30004824:	e1a5      	b.n	30004b72 <BOOT_LoadImages+0x5ce>
30004826:	f04f 0a01 	mov.w	sl, #1
3000482a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000482e:	4835      	ldr	r0, [pc, #212]	; (30004904 <BOOT_LoadImages+0x360>)
30004830:	f006 fd7e 	bl	3000b330 <__DiagPrintf_veneer>
30004834:	f8cd a00c 	str.w	sl, [sp, #12]
30004838:	2e00      	cmp	r6, #0
3000483a:	f000 817f 	beq.w	30004b3c <BOOT_LoadImages+0x598>
3000483e:	4634      	mov	r4, r6
30004840:	e1a9      	b.n	30004b96 <BOOT_LoadImages+0x5f2>
30004842:	f8d3 3404 	ldr.w	r3, [r3, #1028]	; 0x404
30004846:	f8d3 200a 	ldr.w	r2, [r3, #10]
3000484a:	3201      	adds	r2, #1
3000484c:	d106      	bne.n	3000485c <BOOT_LoadImages+0x2b8>
3000484e:	482e      	ldr	r0, [pc, #184]	; (30004908 <BOOT_LoadImages+0x364>)
30004850:	2400      	movs	r4, #0
30004852:	f006 fd6d 	bl	3000b330 <__DiagPrintf_veneer>
30004856:	2301      	movs	r3, #1
30004858:	9303      	str	r3, [sp, #12]
3000485a:	e19c      	b.n	30004b96 <BOOT_LoadImages+0x5f2>
3000485c:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004860:	482a      	ldr	r0, [pc, #168]	; (3000490c <BOOT_LoadImages+0x368>)
30004862:	f006 fd65 	bl	3000b330 <__DiagPrintf_veneer>
30004866:	2301      	movs	r3, #1
30004868:	f04f 0a02 	mov.w	sl, #2
3000486c:	9303      	str	r3, [sp, #12]
3000486e:	e7e3      	b.n	30004838 <BOOT_LoadImages+0x294>
30004870:	2601      	movs	r6, #1
30004872:	e24a      	b.n	30004d0a <BOOT_LoadImages+0x766>
30004874:	f8d8 300a 	ldr.w	r3, [r8, #10]
30004878:	3301      	adds	r3, #1
3000487a:	d107      	bne.n	3000488c <BOOT_LoadImages+0x2e8>
3000487c:	4824      	ldr	r0, [pc, #144]	; (30004910 <BOOT_LoadImages+0x36c>)
3000487e:	f006 fd57 	bl	3000b330 <__DiagPrintf_veneer>
30004882:	2c00      	cmp	r4, #0
30004884:	d074      	beq.n	30004970 <BOOT_LoadImages+0x3cc>
30004886:	2300      	movs	r3, #0
30004888:	9303      	str	r3, [sp, #12]
3000488a:	e7d5      	b.n	30004838 <BOOT_LoadImages+0x294>
3000488c:	ee19 1a90 	vmov	r1, s19
30004890:	4820      	ldr	r0, [pc, #128]	; (30004914 <BOOT_LoadImages+0x370>)
30004892:	f006 fd4d 	bl	3000b330 <__DiagPrintf_veneer>
30004896:	f8d7 1400 	ldr.w	r1, [r7, #1024]	; 0x400
3000489a:	2002      	movs	r0, #2
3000489c:	f7ff f93e 	bl	30003b1c <FLASH_Erase_With_Lock>
300048a0:	e7ef      	b.n	30004882 <BOOT_LoadImages+0x2de>
300048a2:	bf00      	nop
300048a4:	3000c8b9 	.word	0x3000c8b9
300048a8:	3000d78c 	.word	0x3000d78c
300048ac:	3000c8da 	.word	0x3000c8da
300048b0:	3000c939 	.word	0x3000c939
300048b4:	07ffe004 	.word	0x07ffe004
300048b8:	3000c998 	.word	0x3000c998
300048bc:	3000ca12 	.word	0x3000ca12
300048c0:	3000ca8c 	.word	0x3000ca8c
300048c4:	3000cb23 	.word	0x3000cb23
300048c8:	3000cb34 	.word	0x3000cb34
300048cc:	3000c8c9 	.word	0x3000c8c9
300048d0:	3000c8ef 	.word	0x3000c8ef
300048d4:	3000c903 	.word	0x3000c903
300048d8:	3000c91d 	.word	0x3000c91d
300048dc:	3000c94e 	.word	0x3000c94e
300048e0:	3000c962 	.word	0x3000c962
300048e4:	3000c97c 	.word	0x3000c97c
300048e8:	3000c9d3 	.word	0x3000c9d3
300048ec:	3000ca4d 	.word	0x3000ca4d
300048f0:	3000caa6 	.word	0x3000caa6
300048f4:	3000cac0 	.word	0x3000cac0
300048f8:	3000cb07 	.word	0x3000cb07
300048fc:	3000cada 	.word	0x3000cada
30004900:	3000caf4 	.word	0x3000caf4
30004904:	3000c85c 	.word	0x3000c85c
30004908:	3000cb64 	.word	0x3000cb64
3000490c:	3000c873 	.word	0x3000c873
30004910:	3000cb56 	.word	0x3000cb56
30004914:	3000cbae 	.word	0x3000cbae
30004918:	f8d7 8404 	ldr.w	r8, [r7, #1028]	; 0x404
3000491c:	463a      	mov	r2, r7
3000491e:	ee1b 1a10 	vmov	r1, s22
30004922:	f108 0004 	add.w	r0, r8, #4
30004926:	f7ff f99f 	bl	30003c68 <mcc_crc32>
3000492a:	ee1a 3a10 	vmov	r3, s20
3000492e:	4602      	mov	r2, r0
30004930:	4298      	cmp	r0, r3
30004932:	d10f      	bne.n	30004954 <BOOT_LoadImages+0x3b0>
30004934:	ee1a 2a10 	vmov	r2, s20
30004938:	ee1a 1a10 	vmov	r1, s20
3000493c:	48be      	ldr	r0, [pc, #760]	; (30004c38 <BOOT_LoadImages+0x694>)
3000493e:	f006 fcf7 	bl	3000b330 <__DiagPrintf_veneer>
30004942:	f8d7 3404 	ldr.w	r3, [r7, #1028]	; 0x404
30004946:	48bd      	ldr	r0, [pc, #756]	; (30004c3c <BOOT_LoadImages+0x698>)
30004948:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000494c:	f006 fcf0 	bl	3000b330 <__DiagPrintf_veneer>
30004950:	2300      	movs	r3, #0
30004952:	e789      	b.n	30004868 <BOOT_LoadImages+0x2c4>
30004954:	f8d8 300a 	ldr.w	r3, [r8, #10]
30004958:	3301      	adds	r3, #1
3000495a:	d101      	bne.n	30004960 <BOOT_LoadImages+0x3bc>
3000495c:	48b8      	ldr	r0, [pc, #736]	; (30004c40 <BOOT_LoadImages+0x69c>)
3000495e:	e78e      	b.n	3000487e <BOOT_LoadImages+0x2da>
30004960:	ee1a 1a10 	vmov	r1, s20
30004964:	48b7      	ldr	r0, [pc, #732]	; (30004c44 <BOOT_LoadImages+0x6a0>)
30004966:	f006 fce3 	bl	3000b330 <__DiagPrintf_veneer>
3000496a:	f8d7 1404 	ldr.w	r1, [r7, #1028]	; 0x404
3000496e:	e794      	b.n	3000489a <BOOT_LoadImages+0x2f6>
30004970:	9403      	str	r4, [sp, #12]
30004972:	e110      	b.n	30004b96 <BOOT_LoadImages+0x5f2>
30004974:	f1b9 0f00 	cmp.w	r9, #0
30004978:	f000 811e 	beq.w	30004bb8 <BOOT_LoadImages+0x614>
3000497c:	2300      	movs	r3, #0
3000497e:	e11c      	b.n	30004bba <BOOT_LoadImages+0x616>
30004980:	464c      	mov	r4, r9
30004982:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004986:	48b0      	ldr	r0, [pc, #704]	; (30004c48 <BOOT_LoadImages+0x6a4>)
30004988:	f006 fcd2 	bl	3000b330 <__DiagPrintf_veneer>
3000498c:	9403      	str	r4, [sp, #12]
3000498e:	2401      	movs	r4, #1
30004990:	46a1      	mov	r9, r4
30004992:	e088      	b.n	30004aa6 <BOOT_LoadImages+0x502>
30004994:	f8d7 3404 	ldr.w	r3, [r7, #1028]	; 0x404
30004998:	f8d3 200a 	ldr.w	r2, [r3, #10]
3000499c:	3201      	adds	r2, #1
3000499e:	d149      	bne.n	30004a34 <BOOT_LoadImages+0x490>
300049a0:	48a7      	ldr	r0, [pc, #668]	; (30004c40 <BOOT_LoadImages+0x69c>)
300049a2:	e13e      	b.n	30004c22 <BOOT_LoadImages+0x67e>
300049a4:	2c00      	cmp	r4, #0
300049a6:	d17a      	bne.n	30004a9e <BOOT_LoadImages+0x4fa>
300049a8:	f1b9 0f01 	cmp.w	r9, #1
300049ac:	d12c      	bne.n	30004a08 <BOOT_LoadImages+0x464>
300049ae:	f8d7 8400 	ldr.w	r8, [r7, #1024]	; 0x400
300049b2:	463a      	mov	r2, r7
300049b4:	ee1a 1a90 	vmov	r1, s21
300049b8:	f108 0004 	add.w	r0, r8, #4
300049bc:	f7ff f954 	bl	30003c68 <mcc_crc32>
300049c0:	ee19 3a90 	vmov	r3, s19
300049c4:	4602      	mov	r2, r0
300049c6:	4298      	cmp	r0, r3
300049c8:	d10e      	bne.n	300049e8 <BOOT_LoadImages+0x444>
300049ca:	ee19 1a90 	vmov	r1, s19
300049ce:	ee19 2a90 	vmov	r2, s19
300049d2:	489e      	ldr	r0, [pc, #632]	; (30004c4c <BOOT_LoadImages+0x6a8>)
300049d4:	f006 fcac 	bl	3000b330 <__DiagPrintf_veneer>
300049d8:	f8d7 3400 	ldr.w	r3, [r7, #1024]	; 0x400
300049dc:	489a      	ldr	r0, [pc, #616]	; (30004c48 <BOOT_LoadImages+0x6a4>)
300049de:	f8d3 1006 	ldr.w	r1, [r3, #6]
300049e2:	f006 fca5 	bl	3000b330 <__DiagPrintf_veneer>
300049e6:	e7d1      	b.n	3000498c <BOOT_LoadImages+0x3e8>
300049e8:	f8d8 300a 	ldr.w	r3, [r8, #10]
300049ec:	3301      	adds	r3, #1
300049ee:	f000 8117 	beq.w	30004c20 <BOOT_LoadImages+0x67c>
300049f2:	ee19 1a90 	vmov	r1, s19
300049f6:	4896      	ldr	r0, [pc, #600]	; (30004c50 <BOOT_LoadImages+0x6ac>)
300049f8:	f006 fc9a 	bl	3000b330 <__DiagPrintf_veneer>
300049fc:	f8d7 1400 	ldr.w	r1, [r7, #1024]	; 0x400
30004a00:	2002      	movs	r0, #2
30004a02:	f7ff f88b 	bl	30003b1c <FLASH_Erase_With_Lock>
30004a06:	e10e      	b.n	30004c26 <BOOT_LoadImages+0x682>
30004a08:	f8d7 4404 	ldr.w	r4, [r7, #1028]	; 0x404
30004a0c:	463a      	mov	r2, r7
30004a0e:	ee1b 1a10 	vmov	r1, s22
30004a12:	1d20      	adds	r0, r4, #4
30004a14:	f7ff f928 	bl	30003c68 <mcc_crc32>
30004a18:	ee1a 3a10 	vmov	r3, s20
30004a1c:	4602      	mov	r2, r0
30004a1e:	4298      	cmp	r0, r3
30004a20:	d116      	bne.n	30004a50 <BOOT_LoadImages+0x4ac>
30004a22:	ee1a 2a10 	vmov	r2, s20
30004a26:	ee1a 1a10 	vmov	r1, s20
30004a2a:	4883      	ldr	r0, [pc, #524]	; (30004c38 <BOOT_LoadImages+0x694>)
30004a2c:	f006 fc80 	bl	3000b330 <__DiagPrintf_veneer>
30004a30:	f8d7 3404 	ldr.w	r3, [r7, #1028]	; 0x404
30004a34:	2401      	movs	r4, #1
30004a36:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004a3a:	4880      	ldr	r0, [pc, #512]	; (30004c3c <BOOT_LoadImages+0x698>)
30004a3c:	f006 fc78 	bl	3000b330 <__DiagPrintf_veneer>
30004a40:	4b84      	ldr	r3, [pc, #528]	; (30004c54 <BOOT_LoadImages+0x6b0>)
30004a42:	f04f 0a02 	mov.w	sl, #2
30004a46:	f8d3 3404 	ldr.w	r3, [r3, #1028]	; 0x404
30004a4a:	3310      	adds	r3, #16
30004a4c:	9307      	str	r3, [sp, #28]
30004a4e:	e07b      	b.n	30004b48 <BOOT_LoadImages+0x5a4>
30004a50:	f8d4 300a 	ldr.w	r3, [r4, #10]
30004a54:	3301      	adds	r3, #1
30004a56:	d0a3      	beq.n	300049a0 <BOOT_LoadImages+0x3fc>
30004a58:	ee1a 1a10 	vmov	r1, s20
30004a5c:	4879      	ldr	r0, [pc, #484]	; (30004c44 <BOOT_LoadImages+0x6a0>)
30004a5e:	f006 fc67 	bl	3000b330 <__DiagPrintf_veneer>
30004a62:	f8d7 1404 	ldr.w	r1, [r7, #1028]	; 0x404
30004a66:	e7cb      	b.n	30004a00 <BOOT_LoadImages+0x45c>
30004a68:	e9dd 320a 	ldrd	r3, r2, [sp, #40]	; 0x28
30004a6c:	2c00      	cmp	r4, #0
30004a6e:	d056      	beq.n	30004b1e <BOOT_LoadImages+0x57a>
30004a70:	4293      	cmp	r3, r2
30004a72:	e9dd 100c 	ldrd	r1, r0, [sp, #48]	; 0x30
30004a76:	d320      	bcc.n	30004aba <BOOT_LoadImages+0x516>
30004a78:	428b      	cmp	r3, r1
30004a7a:	f0c0 8173 	bcc.w	30004d64 <BOOT_LoadImages+0x7c0>
30004a7e:	4283      	cmp	r3, r0
30004a80:	d319      	bcc.n	30004ab6 <BOOT_LoadImages+0x512>
30004a82:	2301      	movs	r3, #1
30004a84:	ee18 2a10 	vmov	r2, s16
30004a88:	4639      	mov	r1, r7
30004a8a:	e9cd 5300 	strd	r5, r3, [sp]
30004a8e:	4633      	mov	r3, r6
30004a90:	2000      	movs	r0, #0
30004a92:	f7ff f91b 	bl	30003ccc <rewrite_bp>
30004a96:	2400      	movs	r4, #0
30004a98:	fa5f fa80 	uxtb.w	sl, r0
30004a9c:	4626      	mov	r6, r4
30004a9e:	f1ba 0f01 	cmp.w	sl, #1
30004aa2:	f040 80c4 	bne.w	30004c2e <BOOT_LoadImages+0x68a>
30004aa6:	4b6b      	ldr	r3, [pc, #428]	; (30004c54 <BOOT_LoadImages+0x6b0>)
30004aa8:	f04f 0a01 	mov.w	sl, #1
30004aac:	f8d3 3400 	ldr.w	r3, [r3, #1024]	; 0x400
30004ab0:	3310      	adds	r3, #16
30004ab2:	9307      	str	r3, [sp, #28]
30004ab4:	e048      	b.n	30004b48 <BOOT_LoadImages+0x5a4>
30004ab6:	4293      	cmp	r3, r2
30004ab8:	d10c      	bne.n	30004ad4 <BOOT_LoadImages+0x530>
30004aba:	428a      	cmp	r2, r1
30004abc:	f0c0 8150 	bcc.w	30004d60 <BOOT_LoadImages+0x7bc>
30004ac0:	4282      	cmp	r2, r0
30004ac2:	d307      	bcc.n	30004ad4 <BOOT_LoadImages+0x530>
30004ac4:	2001      	movs	r0, #1
30004ac6:	4633      	mov	r3, r6
30004ac8:	ee18 2a10 	vmov	r2, s16
30004acc:	4639      	mov	r1, r7
30004ace:	e9cd 5000 	strd	r5, r0, [sp]
30004ad2:	e7de      	b.n	30004a92 <BOOT_LoadImages+0x4ee>
30004ad4:	428b      	cmp	r3, r1
30004ad6:	d80d      	bhi.n	30004af4 <BOOT_LoadImages+0x550>
30004ad8:	428a      	cmp	r2, r1
30004ada:	d80b      	bhi.n	30004af4 <BOOT_LoadImages+0x550>
30004adc:	4288      	cmp	r0, r1
30004ade:	d809      	bhi.n	30004af4 <BOOT_LoadImages+0x550>
30004ae0:	2400      	movs	r4, #0
30004ae2:	4633      	mov	r3, r6
30004ae4:	ee19 2a10 	vmov	r2, s18
30004ae8:	ee18 1a90 	vmov	r1, s17
30004aec:	e9cd 5400 	strd	r5, r4, [sp]
30004af0:	4620      	mov	r0, r4
30004af2:	e00f      	b.n	30004b14 <BOOT_LoadImages+0x570>
30004af4:	4283      	cmp	r3, r0
30004af6:	d825      	bhi.n	30004b44 <BOOT_LoadImages+0x5a0>
30004af8:	4282      	cmp	r2, r0
30004afa:	d823      	bhi.n	30004b44 <BOOT_LoadImages+0x5a0>
30004afc:	4288      	cmp	r0, r1
30004afe:	f04f 0400 	mov.w	r4, #0
30004b02:	d320      	bcc.n	30004b46 <BOOT_LoadImages+0x5a2>
30004b04:	4633      	mov	r3, r6
30004b06:	ee19 2a10 	vmov	r2, s18
30004b0a:	ee18 1a90 	vmov	r1, s17
30004b0e:	2001      	movs	r0, #1
30004b10:	e9cd 5400 	strd	r5, r4, [sp]
30004b14:	f7ff f8da 	bl	30003ccc <rewrite_bp>
30004b18:	fa5f fa80 	uxtb.w	sl, r0
30004b1c:	e7be      	b.n	30004a9c <BOOT_LoadImages+0x4f8>
30004b1e:	4293      	cmp	r3, r2
30004b20:	f04f 0001 	mov.w	r0, #1
30004b24:	4633      	mov	r3, r6
30004b26:	ee18 2a10 	vmov	r2, s16
30004b2a:	4639      	mov	r1, r7
30004b2c:	e9cd 5000 	strd	r5, r0, [sp]
30004b30:	d3f0      	bcc.n	30004b14 <BOOT_LoadImages+0x570>
30004b32:	e7dd      	b.n	30004af0 <BOOT_LoadImages+0x54c>
30004b34:	4848      	ldr	r0, [pc, #288]	; (30004c58 <BOOT_LoadImages+0x6b4>)
30004b36:	f006 fbfb 	bl	3000b330 <__DiagPrintf_veneer>
30004b3a:	e7fe      	b.n	30004b3a <BOOT_LoadImages+0x596>
30004b3c:	2401      	movs	r4, #1
30004b3e:	e7ae      	b.n	30004a9e <BOOT_LoadImages+0x4fa>
30004b40:	9403      	str	r4, [sp, #12]
30004b42:	e7ac      	b.n	30004a9e <BOOT_LoadImages+0x4fa>
30004b44:	2400      	movs	r4, #0
30004b46:	4626      	mov	r6, r4
30004b48:	4b44      	ldr	r3, [pc, #272]	; (30004c5c <BOOT_LoadImages+0x6b8>)
30004b4a:	4630      	mov	r0, r6
30004b4c:	9a07      	ldr	r2, [sp, #28]
30004b4e:	601a      	str	r2, [r3, #0]
30004b50:	4b43      	ldr	r3, [pc, #268]	; (30004c60 <BOOT_LoadImages+0x6bc>)
30004b52:	4798      	blx	r3
30004b54:	2800      	cmp	r0, #0
30004b56:	f000 808b 	beq.w	30004c70 <BOOT_LoadImages+0x6cc>
30004b5a:	2e01      	cmp	r6, #1
30004b5c:	f000 8097 	beq.w	30004c8e <BOOT_LoadImages+0x6ea>
30004b60:	fab9 f989 	clz	r9, r9
30004b64:	9b03      	ldr	r3, [sp, #12]
30004b66:	ea4f 1959 	mov.w	r9, r9, lsr #5
30004b6a:	2b00      	cmp	r3, #0
30004b6c:	f43f ae80 	beq.w	30004870 <BOOT_LoadImages+0x2cc>
30004b70:	461e      	mov	r6, r3
30004b72:	4b38      	ldr	r3, [pc, #224]	; (30004c54 <BOOT_LoadImages+0x6b0>)
30004b74:	f1b9 0f00 	cmp.w	r9, #0
30004b78:	f47f ae63 	bne.w	30004842 <BOOT_LoadImages+0x29e>
30004b7c:	f8d3 3400 	ldr.w	r3, [r3, #1024]	; 0x400
30004b80:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004b84:	3201      	adds	r2, #1
30004b86:	f47f ae4e 	bne.w	30004826 <BOOT_LoadImages+0x282>
30004b8a:	4836      	ldr	r0, [pc, #216]	; (30004c64 <BOOT_LoadImages+0x6c0>)
30004b8c:	464c      	mov	r4, r9
30004b8e:	f006 fbcf 	bl	3000b330 <__DiagPrintf_veneer>
30004b92:	2301      	movs	r3, #1
30004b94:	9303      	str	r3, [sp, #12]
30004b96:	2280      	movs	r2, #128	; 0x80
30004b98:	21ff      	movs	r1, #255	; 0xff
30004b9a:	a80e      	add	r0, sp, #56	; 0x38
30004b9c:	f006 fbe8 	bl	3000b370 <____wrap_memset_veneer>
30004ba0:	2280      	movs	r2, #128	; 0x80
30004ba2:	4629      	mov	r1, r5
30004ba4:	a80e      	add	r0, sp, #56	; 0x38
30004ba6:	f006 fc03 	bl	3000b3b0 <____wrap_memcpy_veneer>
30004baa:	2e00      	cmp	r6, #0
30004bac:	f43f aee2 	beq.w	30004974 <BOOT_LoadImages+0x3d0>
30004bb0:	f1b9 0f01 	cmp.w	r9, #1
30004bb4:	f47f aee2 	bne.w	3000497c <BOOT_LoadImages+0x3d8>
30004bb8:	2301      	movs	r3, #1
30004bba:	f88d 3044 	strb.w	r3, [sp, #68]	; 0x44
30004bbe:	482a      	ldr	r0, [pc, #168]	; (30004c68 <BOOT_LoadImages+0x6c4>)
30004bc0:	f89d 1044 	ldrb.w	r1, [sp, #68]	; 0x44
30004bc4:	f006 fbb4 	bl	3000b330 <__DiagPrintf_veneer>
30004bc8:	4629      	mov	r1, r5
30004bca:	2002      	movs	r0, #2
30004bcc:	4f21      	ldr	r7, [pc, #132]	; (30004c54 <BOOT_LoadImages+0x6b0>)
30004bce:	f7fe ffa5 	bl	30003b1c <FLASH_Erase_With_Lock>
30004bd2:	1d28      	adds	r0, r5, #4
30004bd4:	aa0f      	add	r2, sp, #60	; 0x3c
30004bd6:	217c      	movs	r1, #124	; 0x7c
30004bd8:	9004      	str	r0, [sp, #16]
30004bda:	f7fe ffc5 	bl	30003b68 <FLASH_TxData_With_Lock>
30004bde:	463a      	mov	r2, r7
30004be0:	f640 71fc 	movw	r1, #4092	; 0xffc
30004be4:	9804      	ldr	r0, [sp, #16]
30004be6:	f7ff f83f 	bl	30003c68 <mcc_crc32>
30004bea:	4629      	mov	r1, r5
30004bec:	900e      	str	r0, [sp, #56]	; 0x38
30004bee:	2002      	movs	r0, #2
30004bf0:	f7fe ff94 	bl	30003b1c <FLASH_Erase_With_Lock>
30004bf4:	aa0e      	add	r2, sp, #56	; 0x38
30004bf6:	2180      	movs	r1, #128	; 0x80
30004bf8:	4628      	mov	r0, r5
30004bfa:	f7fe ffb5 	bl	30003b68 <FLASH_TxData_With_Lock>
30004bfe:	9b03      	ldr	r3, [sp, #12]
30004c00:	2b00      	cmp	r3, #0
30004c02:	f43f aecf 	beq.w	300049a4 <BOOT_LoadImages+0x400>
30004c06:	2c00      	cmp	r4, #0
30004c08:	d19a      	bne.n	30004b40 <BOOT_LoadImages+0x59c>
30004c0a:	f1b9 0f01 	cmp.w	r9, #1
30004c0e:	f47f aec1 	bne.w	30004994 <BOOT_LoadImages+0x3f0>
30004c12:	f8d7 3400 	ldr.w	r3, [r7, #1024]	; 0x400
30004c16:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004c1a:	3201      	adds	r2, #1
30004c1c:	f47f aeb0 	bne.w	30004980 <BOOT_LoadImages+0x3dc>
30004c20:	4810      	ldr	r0, [pc, #64]	; (30004c64 <BOOT_LoadImages+0x6c0>)
30004c22:	f006 fb85 	bl	3000b330 <__DiagPrintf_veneer>
30004c26:	4811      	ldr	r0, [pc, #68]	; (30004c6c <BOOT_LoadImages+0x6c8>)
30004c28:	f006 fb82 	bl	3000b330 <__DiagPrintf_veneer>
30004c2c:	e7fe      	b.n	30004c2c <BOOT_LoadImages+0x688>
30004c2e:	f1ba 0f02 	cmp.w	sl, #2
30004c32:	d189      	bne.n	30004b48 <BOOT_LoadImages+0x5a4>
30004c34:	e704      	b.n	30004a40 <BOOT_LoadImages+0x49c>
30004c36:	bf00      	nop
30004c38:	3000cbfa 	.word	0x3000cbfa
30004c3c:	3000c873 	.word	0x3000c873
30004c40:	3000cb64 	.word	0x3000cb64
30004c44:	3000cc36 	.word	0x3000cc36
30004c48:	3000c85c 	.word	0x3000c85c
30004c4c:	3000cb72 	.word	0x3000cb72
30004c50:	3000cbae 	.word	0x3000cbae
30004c54:	3000d78c 	.word	0x3000d78c
30004c58:	3000ccb8 	.word	0x3000ccb8
30004c5c:	3000ebe8 	.word	0x3000ebe8
30004c60:	30006315 	.word	0x30006315
30004c64:	3000cb56 	.word	0x3000cb56
30004c68:	3000cc82 	.word	0x3000cc82
30004c6c:	3000c88a 	.word	0x3000c88a
30004c70:	4b3e      	ldr	r3, [pc, #248]	; (30004d6c <BOOT_LoadImages+0x7c8>)
30004c72:	781b      	ldrb	r3, [r3, #0]
30004c74:	b12b      	cbz	r3, 30004c82 <BOOT_LoadImages+0x6de>
30004c76:	4630      	mov	r0, r6
30004c78:	4b3d      	ldr	r3, [pc, #244]	; (30004d70 <BOOT_LoadImages+0x7cc>)
30004c7a:	4798      	blx	r3
30004c7c:	2800      	cmp	r0, #0
30004c7e:	f47f af6c 	bne.w	30004b5a <BOOT_LoadImages+0x5b6>
30004c82:	2001      	movs	r0, #1
30004c84:	b02f      	add	sp, #188	; 0xbc
30004c86:	ecbd 8b08 	vpop	{d8-d11}
30004c8a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004c8e:	4c39      	ldr	r4, [pc, #228]	; (30004d74 <BOOT_LoadImages+0x7d0>)
30004c90:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004c94:	47a0      	blx	r4
30004c96:	e7fb      	b.n	30004c90 <BOOT_LoadImages+0x6ec>
30004c98:	f895 900c 	ldrb.w	r9, [r5, #12]
30004c9c:	f8df b0e4 	ldr.w	fp, [pc, #228]	; 30004d84 <BOOT_LoadImages+0x7e0>
30004ca0:	2c00      	cmp	r4, #0
30004ca2:	f47f ada7 	bne.w	300047f4 <BOOT_LoadImages+0x250>
30004ca6:	4834      	ldr	r0, [pc, #208]	; (30004d78 <BOOT_LoadImages+0x7d4>)
30004ca8:	f006 fb42 	bl	3000b330 <__DiagPrintf_veneer>
30004cac:	ee18 3a90 	vmov	r3, s17
30004cb0:	f8cb 3400 	str.w	r3, [fp, #1024]	; 0x400
30004cb4:	ee19 3a10 	vmov	r3, s18
30004cb8:	f8cb 3404 	str.w	r3, [fp, #1028]	; 0x404
30004cbc:	4649      	mov	r1, r9
30004cbe:	482f      	ldr	r0, [pc, #188]	; (30004d7c <BOOT_LoadImages+0x7d8>)
30004cc0:	f006 fb36 	bl	3000b330 <__DiagPrintf_veneer>
30004cc4:	4b2e      	ldr	r3, [pc, #184]	; (30004d80 <BOOT_LoadImages+0x7dc>)
30004cc6:	4798      	blx	r3
30004cc8:	4b2e      	ldr	r3, [pc, #184]	; (30004d84 <BOOT_LoadImages+0x7e0>)
30004cca:	f8d3 4400 	ldr.w	r4, [r3, #1024]	; 0x400
30004cce:	f8d3 1404 	ldr.w	r1, [r3, #1028]	; 0x404
30004cd2:	f8d4 200a 	ldr.w	r2, [r4, #10]
30004cd6:	f8d1 300a 	ldr.w	r3, [r1, #10]
30004cda:	320c      	adds	r2, #12
30004cdc:	330c      	adds	r3, #12
30004cde:	4542      	cmp	r2, r8
30004ce0:	bf28      	it	cs
30004ce2:	4642      	movcs	r2, r8
30004ce4:	4598      	cmp	r8, r3
30004ce6:	bf28      	it	cs
30004ce8:	4698      	movcs	r8, r3
30004cea:	6823      	ldr	r3, [r4, #0]
30004cec:	ee0a 2a90 	vmov	s21, r2
30004cf0:	ee09 3a90 	vmov	s19, r3
30004cf4:	680b      	ldr	r3, [r1, #0]
30004cf6:	ee0b 8a10 	vmov	s22, r8
30004cfa:	ee0a 3a10 	vmov	s20, r3
30004cfe:	2800      	cmp	r0, #0
30004d00:	f47f ad8d 	bne.w	3000481e <BOOT_LoadImages+0x27a>
30004d04:	4682      	mov	sl, r0
30004d06:	4604      	mov	r4, r0
30004d08:	4606      	mov	r6, r0
30004d0a:	4f1e      	ldr	r7, [pc, #120]	; (30004d84 <BOOT_LoadImages+0x7e0>)
30004d0c:	f1b9 0f00 	cmp.w	r9, #0
30004d10:	f47f ae02 	bne.w	30004918 <BOOT_LoadImages+0x374>
30004d14:	f8d7 8400 	ldr.w	r8, [r7, #1024]	; 0x400
30004d18:	463a      	mov	r2, r7
30004d1a:	ee1a 1a90 	vmov	r1, s21
30004d1e:	f108 0004 	add.w	r0, r8, #4
30004d22:	f7fe ffa1 	bl	30003c68 <mcc_crc32>
30004d26:	ee19 3a90 	vmov	r3, s19
30004d2a:	4602      	mov	r2, r0
30004d2c:	4298      	cmp	r0, r3
30004d2e:	f47f ada1 	bne.w	30004874 <BOOT_LoadImages+0x2d0>
30004d32:	ee19 2a90 	vmov	r2, s19
30004d36:	ee19 1a90 	vmov	r1, s19
30004d3a:	4813      	ldr	r0, [pc, #76]	; (30004d88 <BOOT_LoadImages+0x7e4>)
30004d3c:	f04f 0a01 	mov.w	sl, #1
30004d40:	f006 faf6 	bl	3000b330 <__DiagPrintf_veneer>
30004d44:	f8d7 3400 	ldr.w	r3, [r7, #1024]	; 0x400
30004d48:	4810      	ldr	r0, [pc, #64]	; (30004d8c <BOOT_LoadImages+0x7e8>)
30004d4a:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004d4e:	f006 faef 	bl	3000b330 <__DiagPrintf_veneer>
30004d52:	f8cd 900c 	str.w	r9, [sp, #12]
30004d56:	e56f      	b.n	30004838 <BOOT_LoadImages+0x294>
30004d58:	2c00      	cmp	r4, #0
30004d5a:	f47f ad27 	bne.w	300047ac <BOOT_LoadImages+0x208>
30004d5e:	e534      	b.n	300047ca <BOOT_LoadImages+0x226>
30004d60:	428b      	cmp	r3, r1
30004d62:	e6ba      	b.n	30004ada <BOOT_LoadImages+0x536>
30004d64:	4293      	cmp	r3, r2
30004d66:	f47f aeb7 	bne.w	30004ad8 <BOOT_LoadImages+0x534>
30004d6a:	e6a6      	b.n	30004aba <BOOT_LoadImages+0x516>
30004d6c:	3000d5c7 	.word	0x3000d5c7
30004d70:	30006319 	.word	0x30006319
30004d74:	00009be5 	.word	0x00009be5
30004d78:	3000cb15 	.word	0x3000cb15
30004d7c:	3000cb44 	.word	0x3000cb44
30004d80:	0000c149 	.word	0x0000c149
30004d84:	3000d78c 	.word	0x3000d78c
30004d88:	3000cb72 	.word	0x3000cb72
30004d8c:	3000c85c 	.word	0x3000c85c

30004d90 <BOOT_ReasonSet>:
30004d90:	4a03      	ldr	r2, [pc, #12]	; (30004da0 <BOOT_ReasonSet+0x10>)
30004d92:	8c13      	ldrh	r3, [r2, #32]
30004d94:	b29b      	uxth	r3, r3
30004d96:	8413      	strh	r3, [r2, #32]
30004d98:	f8a2 3264 	strh.w	r3, [r2, #612]	; 0x264
30004d9c:	4770      	bx	lr
30004d9e:	bf00      	nop
30004da0:	42008000 	.word	0x42008000

30004da4 <BOOT_Enable_KM0>:
30004da4:	b510      	push	{r4, lr}
30004da6:	4c11      	ldr	r4, [pc, #68]	; (30004dec <BOOT_Enable_KM0+0x48>)
30004da8:	47a0      	blx	r4
30004daa:	b150      	cbz	r0, 30004dc2 <BOOT_Enable_KM0+0x1e>
30004dac:	4b10      	ldr	r3, [pc, #64]	; (30004df0 <BOOT_Enable_KM0+0x4c>)
30004dae:	4798      	blx	r3
30004db0:	b938      	cbnz	r0, 30004dc2 <BOOT_Enable_KM0+0x1e>
30004db2:	4b10      	ldr	r3, [pc, #64]	; (30004df4 <BOOT_Enable_KM0+0x50>)
30004db4:	2004      	movs	r0, #4
30004db6:	4a10      	ldr	r2, [pc, #64]	; (30004df8 <BOOT_Enable_KM0+0x54>)
30004db8:	4619      	mov	r1, r3
30004dba:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004dbe:	f006 b873 	b.w	3000aea8 <rtk_log_write>
30004dc2:	47a0      	blx	r4
30004dc4:	b140      	cbz	r0, 30004dd8 <BOOT_Enable_KM0+0x34>
30004dc6:	4b0a      	ldr	r3, [pc, #40]	; (30004df0 <BOOT_Enable_KM0+0x4c>)
30004dc8:	4798      	blx	r3
30004dca:	b128      	cbz	r0, 30004dd8 <BOOT_Enable_KM0+0x34>
30004dcc:	4b09      	ldr	r3, [pc, #36]	; (30004df4 <BOOT_Enable_KM0+0x50>)
30004dce:	2004      	movs	r0, #4
30004dd0:	4a0a      	ldr	r2, [pc, #40]	; (30004dfc <BOOT_Enable_KM0+0x58>)
30004dd2:	4619      	mov	r1, r3
30004dd4:	f006 f868 	bl	3000aea8 <rtk_log_write>
30004dd8:	4a09      	ldr	r2, [pc, #36]	; (30004e00 <BOOT_Enable_KM0+0x5c>)
30004dda:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
30004dde:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
30004de2:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
30004de6:	bf40      	sev
30004de8:	bd10      	pop	{r4, pc}
30004dea:	bf00      	nop
30004dec:	0000c09d 	.word	0x0000c09d
30004df0:	0000c0ad 	.word	0x0000c0ad
30004df4:	3000c79c 	.word	0x3000c79c
30004df8:	3000cce4 	.word	0x3000cce4
30004dfc:	3000cd0c 	.word	0x3000cd0c
30004e00:	42008000 	.word	0x42008000

30004e04 <BOOT_AP_Clk_Get>:
30004e04:	4a05      	ldr	r2, [pc, #20]	; (30004e1c <BOOT_AP_Clk_Get+0x18>)
30004e06:	6913      	ldr	r3, [r2, #16]
30004e08:	2b00      	cmp	r3, #0
30004e0a:	bfba      	itte	lt
30004e0c:	f023 4300 	biclt.w	r3, r3, #2147483648	; 0x80000000
30004e10:	6850      	ldrlt	r0, [r2, #4]
30004e12:	6810      	ldrge	r0, [r2, #0]
30004e14:	fbb0 f0f3 	udiv	r0, r0, r3
30004e18:	4770      	bx	lr
30004e1a:	bf00      	nop
30004e1c:	3000de10 	.word	0x3000de10

30004e20 <BOOT_SOC_ClkSet>:
30004e20:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004e24:	4f57      	ldr	r7, [pc, #348]	; (30004f84 <BOOT_SOC_ClkSet+0x164>)
30004e26:	f7ff ffed 	bl	30004e04 <BOOT_AP_Clk_Get>
30004e2a:	f44f 7bc8 	mov.w	fp, #400	; 0x190
30004e2e:	2564      	movs	r5, #100	; 0x64
30004e30:	683c      	ldr	r4, [r7, #0]
30004e32:	4606      	mov	r6, r0
30004e34:	68fb      	ldr	r3, [r7, #12]
30004e36:	f8d7 a008 	ldr.w	sl, [r7, #8]
30004e3a:	f8df 9178 	ldr.w	r9, [pc, #376]	; 30004fb4 <BOOT_SOC_ClkSet+0x194>
30004e3e:	f1ba 0f01 	cmp.w	sl, #1
30004e42:	fbb4 f8f3 	udiv	r8, r4, r3
30004e46:	f04f 03c8 	mov.w	r3, #200	; 0xc8
30004e4a:	fbb4 fbfb 	udiv	fp, r4, fp
30004e4e:	fbb4 f5f5 	udiv	r5, r4, r5
30004e52:	fbb4 f4f3 	udiv	r4, r4, r3
30004e56:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30004e5a:	f105 35ff 	add.w	r5, r5, #4294967295	; 0xffffffff
30004e5e:	f104 34ff 	add.w	r4, r4, #4294967295	; 0xffffffff
30004e62:	4b49      	ldr	r3, [pc, #292]	; (30004f88 <BOOT_SOC_ClkSet+0x168>)
30004e64:	d16f      	bne.n	30004f46 <BOOT_SOC_ClkSet+0x126>
30004e66:	4650      	mov	r0, sl
30004e68:	4a48      	ldr	r2, [pc, #288]	; (30004f8c <BOOT_SOC_ClkSet+0x16c>)
30004e6a:	4790      	blx	r2
30004e6c:	4b46      	ldr	r3, [pc, #280]	; (30004f88 <BOOT_SOC_ClkSet+0x168>)
30004e6e:	f5b8 7fa7 	cmp.w	r8, #334	; 0x14e
30004e72:	f883 a10c 	strb.w	sl, [r3, #268]	; 0x10c
30004e76:	d304      	bcc.n	30004e82 <BOOT_SOC_ClkSet+0x62>
30004e78:	f44f 61d6 	mov.w	r1, #1712	; 0x6b0
30004e7c:	4844      	ldr	r0, [pc, #272]	; (30004f90 <BOOT_SOC_ClkSet+0x170>)
30004e7e:	f006 fa67 	bl	3000b350 <__io_assert_failed_veneer>
30004e82:	f899 3000 	ldrb.w	r3, [r9]
30004e86:	2b01      	cmp	r3, #1
30004e88:	d107      	bne.n	30004e9a <BOOT_SOC_ClkSet+0x7a>
30004e8a:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
30004e8e:	d904      	bls.n	30004e9a <BOOT_SOC_ClkSet+0x7a>
30004e90:	f240 61b2 	movw	r1, #1714	; 0x6b2
30004e94:	483e      	ldr	r0, [pc, #248]	; (30004f90 <BOOT_SOC_ClkSet+0x170>)
30004e96:	f006 fa5b 	bl	3000b350 <__io_assert_failed_veneer>
30004e9a:	2001      	movs	r0, #1
30004e9c:	f8df a118 	ldr.w	sl, [pc, #280]	; 30004fb8 <BOOT_SOC_ClkSet+0x198>
30004ea0:	f002 fa52 	bl	30007348 <CLK_SWITCH_XTAL>
30004ea4:	6838      	ldr	r0, [r7, #0]
30004ea6:	4b3b      	ldr	r3, [pc, #236]	; (30004f94 <BOOT_SOC_ClkSet+0x174>)
30004ea8:	ea4f 2b0b 	mov.w	fp, fp, lsl #8
30004eac:	fb0a f000 	mul.w	r0, sl, r0
30004eb0:	032d      	lsls	r5, r5, #12
30004eb2:	4798      	blx	r3
30004eb4:	4b38      	ldr	r3, [pc, #224]	; (30004f98 <BOOT_SOC_ClkSet+0x178>)
30004eb6:	4939      	ldr	r1, [pc, #228]	; (30004f9c <BOOT_SOC_ClkSet+0x17c>)
30004eb8:	f40b 6be0 	and.w	fp, fp, #1792	; 0x700
30004ebc:	f8d3 2228 	ldr.w	r2, [r3, #552]	; 0x228
30004ec0:	b2ad      	uxth	r5, r5
30004ec2:	0524      	lsls	r4, r4, #20
30004ec4:	2000      	movs	r0, #0
30004ec6:	4011      	ands	r1, r2
30004ec8:	68fa      	ldr	r2, [r7, #12]
30004eca:	f404 04e0 	and.w	r4, r4, #7340032	; 0x700000
30004ece:	3a01      	subs	r2, #1
30004ed0:	f002 0207 	and.w	r2, r2, #7
30004ed4:	430a      	orrs	r2, r1
30004ed6:	ea4b 0b02 	orr.w	fp, fp, r2
30004eda:	ea45 050b 	orr.w	r5, r5, fp
30004ede:	432c      	orrs	r4, r5
30004ee0:	f8c3 4228 	str.w	r4, [r3, #552]	; 0x228
30004ee4:	f002 fa30 	bl	30007348 <CLK_SWITCH_XTAL>
30004ee8:	4b2d      	ldr	r3, [pc, #180]	; (30004fa0 <BOOT_SOC_ClkSet+0x180>)
30004eea:	4a2e      	ldr	r2, [pc, #184]	; (30004fa4 <BOOT_SOC_ClkSet+0x184>)
30004eec:	2004      	movs	r0, #4
30004eee:	4619      	mov	r1, r3
30004ef0:	f8cd 8000 	str.w	r8, [sp]
30004ef4:	f005 ffd8 	bl	3000aea8 <rtk_log_write>
30004ef8:	f899 3000 	ldrb.w	r3, [r9]
30004efc:	b303      	cbz	r3, 30004f40 <BOOT_SOC_ClkSet+0x120>
30004efe:	693b      	ldr	r3, [r7, #16]
30004f00:	2b00      	cmp	r3, #0
30004f02:	da35      	bge.n	30004f70 <BOOT_SOC_ClkSet+0x150>
30004f04:	6878      	ldr	r0, [r7, #4]
30004f06:	4b28      	ldr	r3, [pc, #160]	; (30004fa8 <BOOT_SOC_ClkSet+0x188>)
30004f08:	fb0a f000 	mul.w	r0, sl, r0
30004f0c:	4798      	blx	r3
30004f0e:	4b27      	ldr	r3, [pc, #156]	; (30004fac <BOOT_SOC_ClkSet+0x18c>)
30004f10:	2001      	movs	r0, #1
30004f12:	4798      	blx	r3
30004f14:	4b1c      	ldr	r3, [pc, #112]	; (30004f88 <BOOT_SOC_ClkSet+0x168>)
30004f16:	2201      	movs	r2, #1
30004f18:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30004f1c:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30004f20:	693b      	ldr	r3, [r7, #16]
30004f22:	2004      	movs	r0, #4
30004f24:	690a      	ldr	r2, [r1, #16]
30004f26:	3b01      	subs	r3, #1
30004f28:	f022 0203 	bic.w	r2, r2, #3
30004f2c:	f003 0303 	and.w	r3, r3, #3
30004f30:	4313      	orrs	r3, r2
30004f32:	4a1f      	ldr	r2, [pc, #124]	; (30004fb0 <BOOT_SOC_ClkSet+0x190>)
30004f34:	610b      	str	r3, [r1, #16]
30004f36:	4b1a      	ldr	r3, [pc, #104]	; (30004fa0 <BOOT_SOC_ClkSet+0x180>)
30004f38:	9600      	str	r6, [sp, #0]
30004f3a:	4619      	mov	r1, r3
30004f3c:	f005 ffb4 	bl	3000aea8 <rtk_log_write>
30004f40:	b003      	add	sp, #12
30004f42:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f46:	2200      	movs	r2, #0
30004f48:	f1b8 0ffa 	cmp.w	r8, #250	; 0xfa
30004f4c:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30004f50:	d904      	bls.n	30004f5c <BOOT_SOC_ClkSet+0x13c>
30004f52:	f240 61b6 	movw	r1, #1718	; 0x6b6
30004f56:	480e      	ldr	r0, [pc, #56]	; (30004f90 <BOOT_SOC_ClkSet+0x170>)
30004f58:	f006 f9fa 	bl	3000b350 <__io_assert_failed_veneer>
30004f5c:	f899 3000 	ldrb.w	r3, [r9]
30004f60:	2b01      	cmp	r3, #1
30004f62:	d19a      	bne.n	30004e9a <BOOT_SOC_ClkSet+0x7a>
30004f64:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
30004f68:	d997      	bls.n	30004e9a <BOOT_SOC_ClkSet+0x7a>
30004f6a:	f44f 61d7 	mov.w	r1, #1720	; 0x6b8
30004f6e:	e791      	b.n	30004e94 <BOOT_SOC_ClkSet+0x74>
30004f70:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30004f74:	6913      	ldr	r3, [r2, #16]
30004f76:	f043 030c 	orr.w	r3, r3, #12
30004f7a:	6113      	str	r3, [r2, #16]
30004f7c:	2200      	movs	r2, #0
30004f7e:	4b02      	ldr	r3, [pc, #8]	; (30004f88 <BOOT_SOC_ClkSet+0x168>)
30004f80:	e7ca      	b.n	30004f18 <BOOT_SOC_ClkSet+0xf8>
30004f82:	bf00      	nop
30004f84:	3000de10 	.word	0x3000de10
30004f88:	23020000 	.word	0x23020000
30004f8c:	3000a8e5 	.word	0x3000a8e5
30004f90:	3000cfa4 	.word	0x3000cfa4
30004f94:	30007f6d 	.word	0x30007f6d
30004f98:	42008000 	.word	0x42008000
30004f9c:	ff8f08f8 	.word	0xff8f08f8
30004fa0:	3000c79c 	.word	0x3000c79c
30004fa4:	3000cd29 	.word	0x3000cd29
30004fa8:	30007fe9 	.word	0x30007fe9
30004fac:	30008025 	.word	0x30008025
30004fb0:	3000cd40 	.word	0x3000cd40
30004fb4:	3000d5c7 	.word	0x3000d5c7
30004fb8:	000f4240 	.word	0x000f4240

30004fbc <BOOT_Disable_AP>:
30004fbc:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30004fc0:	685a      	ldr	r2, [r3, #4]
30004fc2:	f042 0202 	orr.w	r2, r2, #2
30004fc6:	605a      	str	r2, [r3, #4]
30004fc8:	685a      	ldr	r2, [r3, #4]
30004fca:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30004fce:	605a      	str	r2, [r3, #4]
30004fd0:	4a02      	ldr	r2, [pc, #8]	; (30004fdc <BOOT_Disable_AP+0x20>)
30004fd2:	6f93      	ldr	r3, [r2, #120]	; 0x78
30004fd4:	f043 0303 	orr.w	r3, r3, #3
30004fd8:	6793      	str	r3, [r2, #120]	; 0x78
30004fda:	4770      	bx	lr
30004fdc:	41000200 	.word	0x41000200

30004fe0 <BOOT_Enable_AP>:
30004fe0:	b537      	push	{r0, r1, r2, r4, r5, lr}
30004fe2:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30004fe6:	2104      	movs	r1, #4
30004fe8:	2201      	movs	r2, #1
30004fea:	482b      	ldr	r0, [pc, #172]	; (30005098 <BOOT_Enable_AP+0xb8>)
30004fec:	4b2b      	ldr	r3, [pc, #172]	; (3000509c <BOOT_Enable_AP+0xbc>)
30004fee:	4798      	blx	r3
30004ff0:	6863      	ldr	r3, [r4, #4]
30004ff2:	4d2b      	ldr	r5, [pc, #172]	; (300050a0 <BOOT_Enable_AP+0xc0>)
30004ff4:	2032      	movs	r0, #50	; 0x32
30004ff6:	f043 0302 	orr.w	r3, r3, #2
30004ffa:	6063      	str	r3, [r4, #4]
30004ffc:	6863      	ldr	r3, [r4, #4]
30004ffe:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30005002:	6063      	str	r3, [r4, #4]
30005004:	6823      	ldr	r3, [r4, #0]
30005006:	f043 0302 	orr.w	r3, r3, #2
3000500a:	6023      	str	r3, [r4, #0]
3000500c:	47a8      	blx	r5
3000500e:	6823      	ldr	r3, [r4, #0]
30005010:	2032      	movs	r0, #50	; 0x32
30005012:	f043 0304 	orr.w	r3, r3, #4
30005016:	6023      	str	r3, [r4, #0]
30005018:	6823      	ldr	r3, [r4, #0]
3000501a:	f043 0301 	orr.w	r3, r3, #1
3000501e:	6023      	str	r3, [r4, #0]
30005020:	6823      	ldr	r3, [r4, #0]
30005022:	f043 0310 	orr.w	r3, r3, #16
30005026:	6023      	str	r3, [r4, #0]
30005028:	47a8      	blx	r5
3000502a:	6823      	ldr	r3, [r4, #0]
3000502c:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30005030:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30005034:	6023      	str	r3, [r4, #0]
30005036:	47a8      	blx	r5
30005038:	6863      	ldr	r3, [r4, #4]
3000503a:	f023 0310 	bic.w	r3, r3, #16
3000503e:	6063      	str	r3, [r4, #4]
30005040:	6863      	ldr	r3, [r4, #4]
30005042:	f023 0302 	bic.w	r3, r3, #2
30005046:	6063      	str	r3, [r4, #4]
30005048:	4b16      	ldr	r3, [pc, #88]	; (300050a4 <BOOT_Enable_AP+0xc4>)
3000504a:	4798      	blx	r3
3000504c:	4601      	mov	r1, r0
3000504e:	b990      	cbnz	r0, 30005076 <BOOT_Enable_AP+0x96>
30005050:	4c15      	ldr	r4, [pc, #84]	; (300050a8 <BOOT_Enable_AP+0xc8>)
30005052:	9001      	str	r0, [sp, #4]
30005054:	2041      	movs	r0, #65	; 0x41
30005056:	47a0      	blx	r4
30005058:	9901      	ldr	r1, [sp, #4]
3000505a:	2042      	movs	r0, #66	; 0x42
3000505c:	47a0      	blx	r4
3000505e:	9901      	ldr	r1, [sp, #4]
30005060:	2043      	movs	r0, #67	; 0x43
30005062:	47a0      	blx	r4
30005064:	9901      	ldr	r1, [sp, #4]
30005066:	2044      	movs	r0, #68	; 0x44
30005068:	47a0      	blx	r4
3000506a:	9901      	ldr	r1, [sp, #4]
3000506c:	2045      	movs	r0, #69	; 0x45
3000506e:	47a0      	blx	r4
30005070:	9901      	ldr	r1, [sp, #4]
30005072:	2046      	movs	r0, #70	; 0x46
30005074:	47a0      	blx	r4
30005076:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000507a:	68da      	ldr	r2, [r3, #12]
3000507c:	f042 0201 	orr.w	r2, r2, #1
30005080:	60da      	str	r2, [r3, #12]
30005082:	689a      	ldr	r2, [r3, #8]
30005084:	f042 0201 	orr.w	r2, r2, #1
30005088:	609a      	str	r2, [r3, #8]
3000508a:	4a08      	ldr	r2, [pc, #32]	; (300050ac <BOOT_Enable_AP+0xcc>)
3000508c:	4b08      	ldr	r3, [pc, #32]	; (300050b0 <BOOT_Enable_AP+0xd0>)
3000508e:	6851      	ldr	r1, [r2, #4]
30005090:	430b      	orrs	r3, r1
30005092:	6053      	str	r3, [r2, #4]
30005094:	b003      	add	sp, #12
30005096:	bd30      	pop	{r4, r5, pc}
30005098:	4200c000 	.word	0x4200c000
3000509c:	0000aab9 	.word	0x0000aab9
300050a0:	00009b2d 	.word	0x00009b2d
300050a4:	30008fc9 	.word	0x30008fc9
300050a8:	0000b20d 	.word	0x0000b20d
300050ac:	41000200 	.word	0x41000200
300050b0:	01001111 	.word	0x01001111

300050b4 <BOOT_RAM_KeyDeriveFunc>:
300050b4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300050b8:	ed2d 8b02 	vpush	{d8}
300050bc:	b0cb      	sub	sp, #300	; 0x12c
300050be:	4f61      	ldr	r7, [pc, #388]	; (30005244 <BOOT_RAM_KeyDeriveFunc+0x190>)
300050c0:	4605      	mov	r5, r0
300050c2:	460e      	mov	r6, r1
300050c4:	4860      	ldr	r0, [pc, #384]	; (30005248 <BOOT_RAM_KeyDeriveFunc+0x194>)
300050c6:	ee08 2a10 	vmov	s16, r2
300050ca:	4960      	ldr	r1, [pc, #384]	; (3000524c <BOOT_RAM_KeyDeriveFunc+0x198>)
300050cc:	2201      	movs	r2, #1
300050ce:	469b      	mov	fp, r3
300050d0:	9c57      	ldr	r4, [sp, #348]	; 0x15c
300050d2:	47b8      	blx	r7
300050d4:	2201      	movs	r2, #1
300050d6:	495e      	ldr	r1, [pc, #376]	; (30005250 <BOOT_RAM_KeyDeriveFunc+0x19c>)
300050d8:	485c      	ldr	r0, [pc, #368]	; (3000524c <BOOT_RAM_KeyDeriveFunc+0x198>)
300050da:	47b8      	blx	r7
300050dc:	4b5d      	ldr	r3, [pc, #372]	; (30005254 <BOOT_RAM_KeyDeriveFunc+0x1a0>)
300050de:	2000      	movs	r0, #0
300050e0:	4f5d      	ldr	r7, [pc, #372]	; (30005258 <BOOT_RAM_KeyDeriveFunc+0x1a4>)
300050e2:	4798      	blx	r3
300050e4:	22e0      	movs	r2, #224	; 0xe0
300050e6:	2100      	movs	r1, #0
300050e8:	a812      	add	r0, sp, #72	; 0x48
300050ea:	f006 f941 	bl	3000b370 <____wrap_memset_veneer>
300050ee:	2300      	movs	r3, #0
300050f0:	f8df 9170 	ldr.w	r9, [pc, #368]	; 30005264 <BOOT_RAM_KeyDeriveFunc+0x1b0>
300050f4:	f8ad 3004 	strh.w	r3, [sp, #4]
300050f8:	f88d 3006 	strb.w	r3, [sp, #6]
300050fc:	2301      	movs	r3, #1
300050fe:	f88d 3007 	strb.w	r3, [sp, #7]
30005102:	2c00      	cmp	r4, #0
30005104:	d140      	bne.n	30005188 <BOOT_RAM_KeyDeriveFunc+0xd4>
30005106:	2220      	movs	r2, #32
30005108:	4621      	mov	r1, r4
3000510a:	4d54      	ldr	r5, [pc, #336]	; (3000525c <BOOT_RAM_KeyDeriveFunc+0x1a8>)
3000510c:	a802      	add	r0, sp, #8
3000510e:	47a8      	blx	r5
30005110:	2220      	movs	r2, #32
30005112:	4621      	mov	r1, r4
30005114:	a80a      	add	r0, sp, #40	; 0x28
30005116:	47a8      	blx	r5
30005118:	4951      	ldr	r1, [pc, #324]	; (30005260 <BOOT_RAM_KeyDeriveFunc+0x1ac>)
3000511a:	694b      	ldr	r3, [r1, #20]
3000511c:	03da      	lsls	r2, r3, #15
3000511e:	d514      	bpl.n	3000514a <BOOT_RAM_KeyDeriveFunc+0x96>
30005120:	ab02      	add	r3, sp, #8
30005122:	f013 021f 	ands.w	r2, r3, #31
30005126:	bf1a      	itte	ne
30005128:	f023 031f 	bicne.w	r3, r3, #31
3000512c:	3220      	addne	r2, #32
3000512e:	2220      	moveq	r2, #32
30005130:	f3bf 8f4f 	dsb	sy
30005134:	441a      	add	r2, r3
30005136:	f8c1 3270 	str.w	r3, [r1, #624]	; 0x270
3000513a:	3320      	adds	r3, #32
3000513c:	1ad0      	subs	r0, r2, r3
3000513e:	2800      	cmp	r0, #0
30005140:	dcf9      	bgt.n	30005136 <BOOT_RAM_KeyDeriveFunc+0x82>
30005142:	f3bf 8f4f 	dsb	sy
30005146:	f3bf 8f6f 	isb	sy
3000514a:	4945      	ldr	r1, [pc, #276]	; (30005260 <BOOT_RAM_KeyDeriveFunc+0x1ac>)
3000514c:	694b      	ldr	r3, [r1, #20]
3000514e:	03db      	lsls	r3, r3, #15
30005150:	d514      	bpl.n	3000517c <BOOT_RAM_KeyDeriveFunc+0xc8>
30005152:	ab0a      	add	r3, sp, #40	; 0x28
30005154:	f013 021f 	ands.w	r2, r3, #31
30005158:	bf1a      	itte	ne
3000515a:	f023 031f 	bicne.w	r3, r3, #31
3000515e:	3220      	addne	r2, #32
30005160:	2220      	moveq	r2, #32
30005162:	f3bf 8f4f 	dsb	sy
30005166:	441a      	add	r2, r3
30005168:	f8c1 3270 	str.w	r3, [r1, #624]	; 0x270
3000516c:	3320      	adds	r3, #32
3000516e:	1ad0      	subs	r0, r2, r3
30005170:	2800      	cmp	r0, #0
30005172:	dcf9      	bgt.n	30005168 <BOOT_RAM_KeyDeriveFunc+0xb4>
30005174:	f3bf 8f4f 	dsb	sy
30005178:	f3bf 8f6f 	isb	sy
3000517c:	2000      	movs	r0, #0
3000517e:	b04b      	add	sp, #300	; 0x12c
30005180:	ecbd 8b02 	vpop	{d8}
30005184:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005188:	ab12      	add	r3, sp, #72	; 0x48
3000518a:	4632      	mov	r2, r6
3000518c:	4629      	mov	r1, r5
3000518e:	2020      	movs	r0, #32
30005190:	47b8      	blx	r7
30005192:	aa12      	add	r2, sp, #72	; 0x48
30005194:	4659      	mov	r1, fp
30005196:	ee18 0a10 	vmov	r0, s16
3000519a:	47c8      	blx	r9
3000519c:	2104      	movs	r1, #4
3000519e:	aa12      	add	r2, sp, #72	; 0x48
300051a0:	f8df 80c4 	ldr.w	r8, [pc, #196]	; 30005268 <BOOT_RAM_KeyDeriveFunc+0x1b4>
300051a4:	eb0d 0001 	add.w	r0, sp, r1
300051a8:	f04f 0a01 	mov.w	sl, #1
300051ac:	47c8      	blx	r9
300051ae:	a912      	add	r1, sp, #72	; 0x48
300051b0:	a80a      	add	r0, sp, #40	; 0x28
300051b2:	47c0      	blx	r8
300051b4:	f10d 0c28 	add.w	ip, sp, #40	; 0x28
300051b8:	f10d 0e08 	add.w	lr, sp, #8
300051bc:	e8bc 000f 	ldmia.w	ip!, {r0, r1, r2, r3}
300051c0:	e8ae 000f 	stmia.w	lr!, {r0, r1, r2, r3}
300051c4:	e89c 000f 	ldmia.w	ip, {r0, r1, r2, r3}
300051c8:	e88e 000f 	stmia.w	lr, {r0, r1, r2, r3}
300051cc:	9b56      	ldr	r3, [sp, #344]	; 0x158
300051ce:	459a      	cmp	sl, r3
300051d0:	d31c      	bcc.n	3000520c <BOOT_RAM_KeyDeriveFunc+0x158>
300051d2:	2c20      	cmp	r4, #32
300051d4:	46a0      	mov	r8, r4
300051d6:	a90a      	add	r1, sp, #40	; 0x28
300051d8:	9858      	ldr	r0, [sp, #352]	; 0x160
300051da:	bf28      	it	cs
300051dc:	f04f 0820 	movcs.w	r8, #32
300051e0:	4642      	mov	r2, r8
300051e2:	eba4 0408 	sub.w	r4, r4, r8
300051e6:	f006 f8e3 	bl	3000b3b0 <____wrap_memcpy_veneer>
300051ea:	9b58      	ldr	r3, [sp, #352]	; 0x160
300051ec:	f10d 0107 	add.w	r1, sp, #7
300051f0:	2204      	movs	r2, #4
300051f2:	4443      	add	r3, r8
300051f4:	9358      	str	r3, [sp, #352]	; 0x160
300051f6:	780b      	ldrb	r3, [r1, #0]
300051f8:	3301      	adds	r3, #1
300051fa:	b2db      	uxtb	r3, r3
300051fc:	f801 3901 	strb.w	r3, [r1], #-1
30005200:	2b00      	cmp	r3, #0
30005202:	f47f af7e 	bne.w	30005102 <BOOT_RAM_KeyDeriveFunc+0x4e>
30005206:	3a01      	subs	r2, #1
30005208:	d1f5      	bne.n	300051f6 <BOOT_RAM_KeyDeriveFunc+0x142>
3000520a:	e77a      	b.n	30005102 <BOOT_RAM_KeyDeriveFunc+0x4e>
3000520c:	ab12      	add	r3, sp, #72	; 0x48
3000520e:	4632      	mov	r2, r6
30005210:	4629      	mov	r1, r5
30005212:	2020      	movs	r0, #32
30005214:	47b8      	blx	r7
30005216:	aa12      	add	r2, sp, #72	; 0x48
30005218:	2120      	movs	r1, #32
3000521a:	a802      	add	r0, sp, #8
3000521c:	47c8      	blx	r9
3000521e:	a912      	add	r1, sp, #72	; 0x48
30005220:	a802      	add	r0, sp, #8
30005222:	47c0      	blx	r8
30005224:	a90a      	add	r1, sp, #40	; 0x28
30005226:	ab02      	add	r3, sp, #8
30005228:	2220      	movs	r2, #32
3000522a:	7808      	ldrb	r0, [r1, #0]
3000522c:	3a01      	subs	r2, #1
3000522e:	f813 cb01 	ldrb.w	ip, [r3], #1
30005232:	ea80 000c 	eor.w	r0, r0, ip
30005236:	f801 0b01 	strb.w	r0, [r1], #1
3000523a:	d1f6      	bne.n	3000522a <BOOT_RAM_KeyDeriveFunc+0x176>
3000523c:	f10a 0a01 	add.w	sl, sl, #1
30005240:	e7c4      	b.n	300051cc <BOOT_RAM_KeyDeriveFunc+0x118>
30005242:	bf00      	nop
30005244:	0000b479 	.word	0x0000b479
30005248:	40000008 	.word	0x40000008
3000524c:	40000004 	.word	0x40000004
30005250:	40000002 	.word	0x40000002
30005254:	3000921d 	.word	0x3000921d
30005258:	30008951 	.word	0x30008951
3000525c:	00012be5 	.word	0x00012be5
30005260:	e000ed00 	.word	0xe000ed00
30005264:	30009059 	.word	0x30009059
30005268:	30009091 	.word	0x30009091

3000526c <password_hash_check>:
3000526c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005270:	b091      	sub	sp, #68	; 0x44
30005272:	4614      	mov	r4, r2
30005274:	4d3e      	ldr	r5, [pc, #248]	; (30005370 <password_hash_check+0x104>)
30005276:	4606      	mov	r6, r0
30005278:	af04      	add	r7, sp, #16
3000527a:	e9c7 1302 	strd	r1, r3, [r7, #8]
3000527e:	f102 0308 	add.w	r3, r2, #8
30005282:	f8d7 b058 	ldr.w	fp, [r7, #88]	; 0x58
30005286:	2100      	movs	r1, #0
30005288:	f023 0307 	bic.w	r3, r3, #7
3000528c:	f10b 0208 	add.w	r2, fp, #8
30005290:	ebad 0d03 	sub.w	sp, sp, r3
30005294:	f022 0207 	bic.w	r2, r2, #7
30005298:	6dbb      	ldr	r3, [r7, #88]	; 0x58
3000529a:	f10d 0810 	add.w	r8, sp, #16
3000529e:	ebad 0d02 	sub.w	sp, sp, r2
300052a2:	3301      	adds	r3, #1
300052a4:	1c62      	adds	r2, r4, #1
300052a6:	4640      	mov	r0, r8
300052a8:	f10d 0910 	add.w	r9, sp, #16
300052ac:	607b      	str	r3, [r7, #4]
300052ae:	47a8      	blx	r5
300052b0:	687b      	ldr	r3, [r7, #4]
300052b2:	2100      	movs	r1, #0
300052b4:	4648      	mov	r0, r9
300052b6:	461a      	mov	r2, r3
300052b8:	47a8      	blx	r5
300052ba:	2300      	movs	r3, #0
300052bc:	1e61      	subs	r1, r4, #1
300052be:	4299      	cmp	r1, r3
300052c0:	d82a      	bhi.n	30005318 <password_hash_check+0xac>
300052c2:	5c73      	ldrb	r3, [r6, r1]
300052c4:	6dba      	ldr	r2, [r7, #88]	; 0x58
300052c6:	f808 300a 	strb.w	r3, [r8, sl]
300052ca:	2300      	movs	r3, #0
300052cc:	3a01      	subs	r2, #1
300052ce:	4619      	mov	r1, r3
300052d0:	429a      	cmp	r2, r3
300052d2:	d832      	bhi.n	3000533a <password_hash_check+0xce>
300052d4:	68fb      	ldr	r3, [r7, #12]
300052d6:	2620      	movs	r6, #32
300052d8:	4640      	mov	r0, r8
300052da:	5c9b      	ldrb	r3, [r3, r2]
300052dc:	464a      	mov	r2, r9
300052de:	f809 3001 	strb.w	r3, [r9, r1]
300052e2:	f107 0310 	add.w	r3, r7, #16
300052e6:	4621      	mov	r1, r4
300052e8:	9601      	str	r6, [sp, #4]
300052ea:	9302      	str	r3, [sp, #8]
300052ec:	68bb      	ldr	r3, [r7, #8]
300052ee:	9300      	str	r3, [sp, #0]
300052f0:	465b      	mov	r3, fp
300052f2:	f7ff fedf 	bl	300050b4 <BOOT_RAM_KeyDeriveFunc>
300052f6:	4632      	mov	r2, r6
300052f8:	491e      	ldr	r1, [pc, #120]	; (30005374 <password_hash_check+0x108>)
300052fa:	f107 0010 	add.w	r0, r7, #16
300052fe:	f006 f89f 	bl	3000b440 <____wrap_memcmp_veneer>
30005302:	4604      	mov	r4, r0
30005304:	b358      	cbz	r0, 3000535e <password_hash_check+0xf2>
30005306:	2401      	movs	r4, #1
30005308:	481b      	ldr	r0, [pc, #108]	; (30005378 <password_hash_check+0x10c>)
3000530a:	f006 f811 	bl	3000b330 <__DiagPrintf_veneer>
3000530e:	4620      	mov	r0, r4
30005310:	3734      	adds	r7, #52	; 0x34
30005312:	46bd      	mov	sp, r7
30005314:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005318:	18f0      	adds	r0, r6, r3
3000531a:	5cf2      	ldrb	r2, [r6, r3]
3000531c:	7840      	ldrb	r0, [r0, #1]
3000531e:	2830      	cmp	r0, #48	; 0x30
30005320:	d105      	bne.n	3000532e <password_hash_check+0xc2>
30005322:	2a5c      	cmp	r2, #92	; 0x5c
30005324:	bf02      	ittt	eq
30005326:	3301      	addeq	r3, #1
30005328:	f104 34ff 	addeq.w	r4, r4, #4294967295	; 0xffffffff
3000532c:	2200      	moveq	r2, #0
3000532e:	f808 200a 	strb.w	r2, [r8, sl]
30005332:	3301      	adds	r3, #1
30005334:	f10a 0a01 	add.w	sl, sl, #1
30005338:	e7c1      	b.n	300052be <password_hash_check+0x52>
3000533a:	68fe      	ldr	r6, [r7, #12]
3000533c:	68f8      	ldr	r0, [r7, #12]
3000533e:	441e      	add	r6, r3
30005340:	5cc0      	ldrb	r0, [r0, r3]
30005342:	7876      	ldrb	r6, [r6, #1]
30005344:	2e30      	cmp	r6, #48	; 0x30
30005346:	d105      	bne.n	30005354 <password_hash_check+0xe8>
30005348:	285c      	cmp	r0, #92	; 0x5c
3000534a:	bf02      	ittt	eq
3000534c:	3301      	addeq	r3, #1
3000534e:	f10b 3bff 	addeq.w	fp, fp, #4294967295	; 0xffffffff
30005352:	2000      	moveq	r0, #0
30005354:	f809 0001 	strb.w	r0, [r9, r1]
30005358:	3301      	adds	r3, #1
3000535a:	3101      	adds	r1, #1
3000535c:	e7b8      	b.n	300052d0 <password_hash_check+0x64>
3000535e:	4807      	ldr	r0, [pc, #28]	; (3000537c <password_hash_check+0x110>)
30005360:	f005 ffe6 	bl	3000b330 <__DiagPrintf_veneer>
30005364:	4632      	mov	r2, r6
30005366:	4621      	mov	r1, r4
30005368:	f107 0010 	add.w	r0, r7, #16
3000536c:	47a8      	blx	r5
3000536e:	e7ce      	b.n	3000530e <password_hash_check+0xa2>
30005370:	00012be5 	.word	0x00012be5
30005374:	3000db94 	.word	0x3000db94
30005378:	3000cd57 	.word	0x3000cd57
3000537c:	3000cd6e 	.word	0x3000cd6e

30005380 <BOOT_Share_Memory_Patch>:
30005380:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005384:	6b1a      	ldr	r2, [r3, #48]	; 0x30
30005386:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000538a:	631a      	str	r2, [r3, #48]	; 0x30
3000538c:	f04f 5208 	mov.w	r2, #570425344	; 0x22000000
30005390:	6810      	ldr	r0, [r2, #0]
30005392:	f502 4240 	add.w	r2, r2, #49152	; 0xc000
30005396:	6812      	ldr	r2, [r2, #0]
30005398:	4410      	add	r0, r2
3000539a:	4a11      	ldr	r2, [pc, #68]	; (300053e0 <BOOT_Share_Memory_Patch+0x60>)
3000539c:	6812      	ldr	r2, [r2, #0]
3000539e:	4410      	add	r0, r2
300053a0:	4a10      	ldr	r2, [pc, #64]	; (300053e4 <BOOT_Share_Memory_Patch+0x64>)
300053a2:	6812      	ldr	r2, [r2, #0]
300053a4:	4410      	add	r0, r2
300053a6:	4a10      	ldr	r2, [pc, #64]	; (300053e8 <BOOT_Share_Memory_Patch+0x68>)
300053a8:	6812      	ldr	r2, [r2, #0]
300053aa:	4410      	add	r0, r2
300053ac:	6b1a      	ldr	r2, [r3, #48]	; 0x30
300053ae:	f422 22c0 	bic.w	r2, r2, #393216	; 0x60000
300053b2:	631a      	str	r2, [r3, #48]	; 0x30
300053b4:	f04f 5300 	mov.w	r3, #536870912	; 0x20000000
300053b8:	681b      	ldr	r3, [r3, #0]
300053ba:	4418      	add	r0, r3
300053bc:	4b0b      	ldr	r3, [pc, #44]	; (300053ec <BOOT_Share_Memory_Patch+0x6c>)
300053be:	681b      	ldr	r3, [r3, #0]
300053c0:	4418      	add	r0, r3
300053c2:	4b0b      	ldr	r3, [pc, #44]	; (300053f0 <BOOT_Share_Memory_Patch+0x70>)
300053c4:	681b      	ldr	r3, [r3, #0]
300053c6:	4418      	add	r0, r3
300053c8:	4b0a      	ldr	r3, [pc, #40]	; (300053f4 <BOOT_Share_Memory_Patch+0x74>)
300053ca:	681b      	ldr	r3, [r3, #0]
300053cc:	4418      	add	r0, r3
300053ce:	f04f 530c 	mov.w	r3, #587202560	; 0x23000000
300053d2:	681b      	ldr	r3, [r3, #0]
300053d4:	4418      	add	r0, r3
300053d6:	4b08      	ldr	r3, [pc, #32]	; (300053f8 <BOOT_Share_Memory_Patch+0x78>)
300053d8:	681b      	ldr	r3, [r3, #0]
300053da:	4418      	add	r0, r3
300053dc:	4770      	bx	lr
300053de:	bf00      	nop
300053e0:	22018000 	.word	0x22018000
300053e4:	2202c000 	.word	0x2202c000
300053e8:	22100000 	.word	0x22100000
300053ec:	20010000 	.word	0x20010000
300053f0:	20020000 	.word	0x20020000
300053f4:	20030000 	.word	0x20030000
300053f8:	23010000 	.word	0x23010000

300053fc <BOOT_Image1>:
300053fc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005400:	4bb3      	ldr	r3, [pc, #716]	; (300056d0 <BOOT_Image1+0x2d4>)
30005402:	b08b      	sub	sp, #44	; 0x2c
30005404:	4798      	blx	r3
30005406:	2800      	cmp	r0, #0
30005408:	f040 81c9 	bne.w	3000579e <BOOT_Image1+0x3a2>
3000540c:	48b1      	ldr	r0, [pc, #708]	; (300056d4 <BOOT_Image1+0x2d8>)
3000540e:	f005 ff8f 	bl	3000b330 <__DiagPrintf_veneer>
30005412:	49b1      	ldr	r1, [pc, #708]	; (300056d8 <BOOT_Image1+0x2dc>)
30005414:	48b1      	ldr	r0, [pc, #708]	; (300056dc <BOOT_Image1+0x2e0>)
30005416:	f005 ff8b 	bl	3000b330 <__DiagPrintf_veneer>
3000541a:	48b1      	ldr	r0, [pc, #708]	; (300056e0 <BOOT_Image1+0x2e4>)
3000541c:	4ab1      	ldr	r2, [pc, #708]	; (300056e4 <BOOT_Image1+0x2e8>)
3000541e:	2100      	movs	r1, #0
30005420:	4bb1      	ldr	r3, [pc, #708]	; (300056e8 <BOOT_Image1+0x2ec>)
30005422:	1a12      	subs	r2, r2, r0
30005424:	4798      	blx	r3
30005426:	f7ff fcb3 	bl	30004d90 <BOOT_ReasonSet>
3000542a:	f003 f9cf 	bl	300087cc <BOOT_Reason>
3000542e:	4601      	mov	r1, r0
30005430:	b920      	cbnz	r0, 3000543c <BOOT_Image1+0x40>
30005432:	f44f 7201 	mov.w	r2, #516	; 0x204
30005436:	48ad      	ldr	r0, [pc, #692]	; (300056ec <BOOT_Image1+0x2f0>)
30005438:	f005 ff9a 	bl	3000b370 <____wrap_memset_veneer>
3000543c:	4dac      	ldr	r5, [pc, #688]	; (300056f0 <BOOT_Image1+0x2f4>)
3000543e:	4bad      	ldr	r3, [pc, #692]	; (300056f4 <BOOT_Image1+0x2f8>)
30005440:	4798      	blx	r3
30005442:	782c      	ldrb	r4, [r5, #0]
30005444:	4eac      	ldr	r6, [pc, #688]	; (300056f8 <BOOT_Image1+0x2fc>)
30005446:	2c00      	cmp	r4, #0
30005448:	f000 81ab 	beq.w	300057a2 <BOOT_Image1+0x3a6>
3000544c:	2000      	movs	r0, #0
3000544e:	4bab      	ldr	r3, [pc, #684]	; (300056fc <BOOT_Image1+0x300>)
30005450:	4798      	blx	r3
30005452:	2001      	movs	r0, #1
30005454:	47b0      	blx	r6
30005456:	4eaa      	ldr	r6, [pc, #680]	; (30005700 <BOOT_Image1+0x304>)
30005458:	f7ff fce2 	bl	30004e20 <BOOT_SOC_ClkSet>
3000545c:	f7fe ff0c 	bl	30004278 <BOOT_GRstConfig>
30005460:	4fa8      	ldr	r7, [pc, #672]	; (30005704 <BOOT_Image1+0x308>)
30005462:	47b0      	blx	r6
30005464:	2802      	cmp	r0, #2
30005466:	4aa1      	ldr	r2, [pc, #644]	; (300056ec <BOOT_Image1+0x2f0>)
30005468:	f040 81a4 	bne.w	300057b4 <BOOT_Image1+0x3b8>
3000546c:	4ca6      	ldr	r4, [pc, #664]	; (30005708 <BOOT_Image1+0x30c>)
3000546e:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005472:	f8d4 325c 	ldr.w	r3, [r4, #604]	; 0x25c
30005476:	f423 7398 	bic.w	r3, r3, #304	; 0x130
3000547a:	f8c4 325c 	str.w	r3, [r4, #604]	; 0x25c
3000547e:	f882 010e 	strb.w	r0, [r2, #270]	; 0x10e
30005482:	2201      	movs	r2, #1
30005484:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005488:	47b8      	blx	r7
3000548a:	f8d4 32fc 	ldr.w	r3, [r4, #764]	; 0x2fc
3000548e:	f043 0302 	orr.w	r3, r3, #2
30005492:	f8c4 32fc 	str.w	r3, [r4, #764]	; 0x2fc
30005496:	4b9d      	ldr	r3, [pc, #628]	; (3000570c <BOOT_Image1+0x310>)
30005498:	4798      	blx	r3
3000549a:	4a9b      	ldr	r2, [pc, #620]	; (30005708 <BOOT_Image1+0x30c>)
3000549c:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
300054a0:	f023 0303 	bic.w	r3, r3, #3
300054a4:	2800      	cmp	r0, #0
300054a6:	f040 819a 	bne.w	300057de <BOOT_Image1+0x3e2>
300054aa:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
300054ae:	4b98      	ldr	r3, [pc, #608]	; (30005710 <BOOT_Image1+0x314>)
300054b0:	4c98      	ldr	r4, [pc, #608]	; (30005714 <BOOT_Image1+0x318>)
300054b2:	781b      	ldrb	r3, [r3, #0]
300054b4:	b1b3      	cbz	r3, 300054e4 <BOOT_Image1+0xe8>
300054b6:	f8df 82cc 	ldr.w	r8, [pc, #716]	; 30005784 <BOOT_Image1+0x388>
300054ba:	47c0      	blx	r8
300054bc:	4607      	mov	r7, r0
300054be:	47c0      	blx	r8
300054c0:	2f02      	cmp	r7, #2
300054c2:	d00f      	beq.n	300054e4 <BOOT_Image1+0xe8>
300054c4:	2803      	cmp	r0, #3
300054c6:	d00d      	beq.n	300054e4 <BOOT_Image1+0xe8>
300054c8:	4b93      	ldr	r3, [pc, #588]	; (30005718 <BOOT_Image1+0x31c>)
300054ca:	4798      	blx	r3
300054cc:	2200      	movs	r2, #0
300054ce:	2101      	movs	r1, #1
300054d0:	4892      	ldr	r0, [pc, #584]	; (3000571c <BOOT_Image1+0x320>)
300054d2:	47a0      	blx	r4
300054d4:	2101      	movs	r1, #1
300054d6:	4891      	ldr	r0, [pc, #580]	; (3000571c <BOOT_Image1+0x320>)
300054d8:	4b91      	ldr	r3, [pc, #580]	; (30005720 <BOOT_Image1+0x324>)
300054da:	4798      	blx	r3
300054dc:	2201      	movs	r2, #1
300054de:	488f      	ldr	r0, [pc, #572]	; (3000571c <BOOT_Image1+0x320>)
300054e0:	4611      	mov	r1, r2
300054e2:	47a0      	blx	r4
300054e4:	2201      	movs	r2, #1
300054e6:	2102      	movs	r1, #2
300054e8:	488c      	ldr	r0, [pc, #560]	; (3000571c <BOOT_Image1+0x320>)
300054ea:	47a0      	blx	r4
300054ec:	4c8d      	ldr	r4, [pc, #564]	; (30005724 <BOOT_Image1+0x328>)
300054ee:	68a3      	ldr	r3, [r4, #8]
300054f0:	079f      	lsls	r7, r3, #30
300054f2:	d504      	bpl.n	300054fe <BOOT_Image1+0x102>
300054f4:	f3ef 8108 	mrs	r1, MSP
300054f8:	488b      	ldr	r0, [pc, #556]	; (30005728 <BOOT_Image1+0x32c>)
300054fa:	f005 ff19 	bl	3000b330 <__DiagPrintf_veneer>
300054fe:	498b      	ldr	r1, [pc, #556]	; (3000572c <BOOT_Image1+0x330>)
30005500:	e841 f100 	tt	r1, r1
30005504:	68a3      	ldr	r3, [r4, #8]
30005506:	f3c1 5180 	ubfx	r1, r1, #22, #1
3000550a:	0798      	lsls	r0, r3, #30
3000550c:	d502      	bpl.n	30005514 <BOOT_Image1+0x118>
3000550e:	4888      	ldr	r0, [pc, #544]	; (30005730 <BOOT_Image1+0x334>)
30005510:	f005 ff0e 	bl	3000b330 <__DiagPrintf_veneer>
30005514:	4b87      	ldr	r3, [pc, #540]	; (30005734 <BOOT_Image1+0x338>)
30005516:	681b      	ldr	r3, [r3, #0]
30005518:	4798      	blx	r3
3000551a:	f7fe fcb5 	bl	30003e88 <BOOT_RccConfig>
3000551e:	f7fe fd1d 	bl	30003f5c <BOOT_CACHEWRR_Set>
30005522:	4b85      	ldr	r3, [pc, #532]	; (30005738 <BOOT_Image1+0x33c>)
30005524:	6818      	ldr	r0, [r3, #0]
30005526:	f7fe fd47 	bl	30003fb8 <BOOT_TCMSet>
3000552a:	4884      	ldr	r0, [pc, #528]	; (3000573c <BOOT_Image1+0x340>)
3000552c:	4984      	ldr	r1, [pc, #528]	; (30005740 <BOOT_Image1+0x344>)
3000552e:	6943      	ldr	r3, [r0, #20]
30005530:	03da      	lsls	r2, r3, #15
30005532:	d514      	bpl.n	3000555e <BOOT_Image1+0x162>
30005534:	f011 021f 	ands.w	r2, r1, #31
30005538:	bf19      	ittee	ne
3000553a:	f021 031f 	bicne.w	r3, r1, #31
3000553e:	3274      	addne	r2, #116	; 0x74
30005540:	460b      	moveq	r3, r1
30005542:	2274      	moveq	r2, #116	; 0x74
30005544:	f3bf 8f4f 	dsb	sy
30005548:	441a      	add	r2, r3
3000554a:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
3000554e:	3320      	adds	r3, #32
30005550:	1ad7      	subs	r7, r2, r3
30005552:	2f00      	cmp	r7, #0
30005554:	dcf9      	bgt.n	3000554a <BOOT_Image1+0x14e>
30005556:	f3bf 8f4f 	dsb	sy
3000555a:	f3bf 8f6f 	isb	sy
3000555e:	4b6a      	ldr	r3, [pc, #424]	; (30005708 <BOOT_Image1+0x30c>)
30005560:	2200      	movs	r2, #0
30005562:	f8c3 1270 	str.w	r1, [r3, #624]	; 0x270
30005566:	4b61      	ldr	r3, [pc, #388]	; (300056ec <BOOT_Image1+0x2f0>)
30005568:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000556c:	f7fe f860 	bl	30003630 <flash_highspeed_setup>
30005570:	47b0      	blx	r6
30005572:	2802      	cmp	r0, #2
30005574:	4e73      	ldr	r6, [pc, #460]	; (30005744 <BOOT_Image1+0x348>)
30005576:	f040 8135 	bne.w	300057e4 <BOOT_Image1+0x3e8>
3000557a:	4b73      	ldr	r3, [pc, #460]	; (30005748 <BOOT_Image1+0x34c>)
3000557c:	2004      	movs	r0, #4
3000557e:	4a73      	ldr	r2, [pc, #460]	; (3000574c <BOOT_Image1+0x350>)
30005580:	4619      	mov	r1, r3
30005582:	f005 fc91 	bl	3000aea8 <rtk_log_write>
30005586:	4b72      	ldr	r3, [pc, #456]	; (30005750 <BOOT_Image1+0x354>)
30005588:	4798      	blx	r3
3000558a:	f7fe fed7 	bl	3000433c <BOOT_PSRAM_Init>
3000558e:	7828      	ldrb	r0, [r5, #0]
30005590:	b940      	cbnz	r0, 300055a4 <BOOT_Image1+0x1a8>
30005592:	4b70      	ldr	r3, [pc, #448]	; (30005754 <BOOT_Image1+0x358>)
30005594:	4798      	blx	r3
30005596:	b128      	cbz	r0, 300055a4 <BOOT_Image1+0x1a8>
30005598:	4b6b      	ldr	r3, [pc, #428]	; (30005748 <BOOT_Image1+0x34c>)
3000559a:	2003      	movs	r0, #3
3000559c:	4a6e      	ldr	r2, [pc, #440]	; (30005758 <BOOT_Image1+0x35c>)
3000559e:	4619      	mov	r1, r3
300055a0:	f005 fc82 	bl	3000aea8 <rtk_log_write>
300055a4:	4b58      	ldr	r3, [pc, #352]	; (30005708 <BOOT_Image1+0x30c>)
300055a6:	2500      	movs	r5, #0
300055a8:	f10d 011d 	add.w	r1, sp, #29
300055ac:	f8c3 52d0 	str.w	r5, [r3, #720]	; 0x2d0
300055b0:	f8c3 52d4 	str.w	r5, [r3, #724]	; 0x2d4
300055b4:	f8c3 52d8 	str.w	r5, [r3, #728]	; 0x2d8
300055b8:	f8c3 50d0 	str.w	r5, [r3, #208]	; 0xd0
300055bc:	f7ff fee0 	bl	30005380 <BOOT_Share_Memory_Patch>
300055c0:	f44f 7059 	mov.w	r0, #868	; 0x364
300055c4:	f002 fbc8 	bl	30007d58 <OTP_Read8>
300055c8:	f10d 011f 	add.w	r1, sp, #31
300055cc:	f44f 705a 	mov.w	r0, #872	; 0x368
300055d0:	f88d 501e 	strb.w	r5, [sp, #30]
300055d4:	f88d 501f 	strb.w	r5, [sp, #31]
300055d8:	f002 fbbe 	bl	30007d58 <OTP_Read8>
300055dc:	f10d 011e 	add.w	r1, sp, #30
300055e0:	f240 3069 	movw	r0, #873	; 0x369
300055e4:	f002 fbb8 	bl	30007d58 <OTP_Read8>
300055e8:	f89d 301d 	ldrb.w	r3, [sp, #29]
300055ec:	f013 0f1e 	tst.w	r3, #30
300055f0:	d125      	bne.n	3000563e <BOOT_Image1+0x242>
300055f2:	f89d 201e 	ldrb.w	r2, [sp, #30]
300055f6:	f89d 301f 	ldrb.w	r3, [sp, #31]
300055fa:	f002 0208 	and.w	r2, r2, #8
300055fe:	f003 0301 	and.w	r3, r3, #1
30005602:	4313      	orrs	r3, r2
30005604:	d01b      	beq.n	3000563e <BOOT_Image1+0x242>
30005606:	4855      	ldr	r0, [pc, #340]	; (3000575c <BOOT_Image1+0x360>)
30005608:	f005 fe92 	bl	3000b330 <__DiagPrintf_veneer>
3000560c:	f89d 101f 	ldrb.w	r1, [sp, #31]
30005610:	f89d 301e 	ldrb.w	r3, [sp, #30]
30005614:	f44f 705a 	mov.w	r0, #872	; 0x368
30005618:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
3000561c:	f023 0308 	bic.w	r3, r3, #8
30005620:	f88d 101f 	strb.w	r1, [sp, #31]
30005624:	f88d 301e 	strb.w	r3, [sp, #30]
30005628:	f002 fbfe 	bl	30007e28 <OTP_Write8>
3000562c:	f89d 101e 	ldrb.w	r1, [sp, #30]
30005630:	f240 3069 	movw	r0, #873	; 0x369
30005634:	f002 fbf8 	bl	30007e28 <OTP_Write8>
30005638:	4849      	ldr	r0, [pc, #292]	; (30005760 <BOOT_Image1+0x364>)
3000563a:	f005 fe79 	bl	3000b330 <__DiagPrintf_veneer>
3000563e:	4b49      	ldr	r3, [pc, #292]	; (30005764 <BOOT_Image1+0x368>)
30005640:	781b      	ldrb	r3, [r3, #0]
30005642:	b133      	cbz	r3, 30005652 <BOOT_Image1+0x256>
30005644:	4b48      	ldr	r3, [pc, #288]	; (30005768 <BOOT_Image1+0x36c>)
30005646:	4798      	blx	r3
30005648:	b918      	cbnz	r0, 30005652 <BOOT_Image1+0x256>
3000564a:	68a3      	ldr	r3, [r4, #8]
3000564c:	f043 0302 	orr.w	r3, r3, #2
30005650:	60a3      	str	r3, [r4, #8]
30005652:	6823      	ldr	r3, [r4, #0]
30005654:	f043 0302 	orr.w	r3, r3, #2
30005658:	6023      	str	r3, [r4, #0]
3000565a:	4c44      	ldr	r4, [pc, #272]	; (3000576c <BOOT_Image1+0x370>)
3000565c:	47a0      	blx	r4
3000565e:	2801      	cmp	r0, #1
30005660:	f04f 0100 	mov.w	r1, #0
30005664:	f04f 0002 	mov.w	r0, #2
30005668:	bf0c      	ite	eq
3000566a:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
3000566e:	4a40      	ldrne	r2, [pc, #256]	; (30005770 <BOOT_Image1+0x374>)
30005670:	f001 fcaa 	bl	30006fc8 <ymodem_uart_port_init>
30005674:	47a0      	blx	r4
30005676:	2800      	cmp	r0, #0
30005678:	f000 80cd 	beq.w	30005816 <BOOT_Image1+0x41a>
3000567c:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30005680:	483c      	ldr	r0, [pc, #240]	; (30005774 <BOOT_Image1+0x378>)
30005682:	2464      	movs	r4, #100	; 0x64
30005684:	f005 fe54 	bl	3000b330 <__DiagPrintf_veneer>
30005688:	483b      	ldr	r0, [pc, #236]	; (30005778 <BOOT_Image1+0x37c>)
3000568a:	f005 fee9 	bl	3000b460 <____wrap_strlen_veneer>
3000568e:	4601      	mov	r1, r0
30005690:	4839      	ldr	r0, [pc, #228]	; (30005778 <BOOT_Image1+0x37c>)
30005692:	f001 fcdf 	bl	30007054 <ymodem_uart_putdata>
30005696:	2001      	movs	r0, #1
30005698:	47b0      	blx	r6
3000569a:	f001 fcc1 	bl	30007020 <ymodem_uart_readable>
3000569e:	2800      	cmp	r0, #0
300056a0:	f040 80bb 	bne.w	3000581a <BOOT_Image1+0x41e>
300056a4:	3c01      	subs	r4, #1
300056a6:	d1f6      	bne.n	30005696 <BOOT_Image1+0x29a>
300056a8:	9404      	str	r4, [sp, #16]
300056aa:	f001 fcb9 	bl	30007020 <ymodem_uart_readable>
300056ae:	4604      	mov	r4, r0
300056b0:	2800      	cmp	r0, #0
300056b2:	f000 822f 	beq.w	30005b14 <BOOT_Image1+0x718>
300056b6:	2001      	movs	r0, #1
300056b8:	4c30      	ldr	r4, [pc, #192]	; (3000577c <BOOT_Image1+0x380>)
300056ba:	47b0      	blx	r6
300056bc:	2108      	movs	r1, #8
300056be:	4830      	ldr	r0, [pc, #192]	; (30005780 <BOOT_Image1+0x384>)
300056c0:	f001 fcd6 	bl	30007070 <ymodem_uart_getdata>
300056c4:	492e      	ldr	r1, [pc, #184]	; (30005780 <BOOT_Image1+0x384>)
300056c6:	2208      	movs	r2, #8
300056c8:	f101 0080 	add.w	r0, r1, #128	; 0x80
300056cc:	47a0      	blx	r4
300056ce:	e05b      	b.n	30005788 <BOOT_Image1+0x38c>
300056d0:	0000c149 	.word	0x0000c149
300056d4:	3000cd88 	.word	0x3000cd88
300056d8:	3000d2c8 	.word	0x3000d2c8
300056dc:	3000cdac 	.word	0x3000cdac
300056e0:	3000eb40 	.word	0x3000eb40
300056e4:	3000efe4 	.word	0x3000efe4
300056e8:	00012be5 	.word	0x00012be5
300056ec:	23020000 	.word	0x23020000
300056f0:	3000d5c6 	.word	0x3000d5c6
300056f4:	3000a47d 	.word	0x3000a47d
300056f8:	3000a6a5 	.word	0x3000a6a5
300056fc:	3000a77d 	.word	0x3000a77d
30005700:	3000721d 	.word	0x3000721d
30005704:	0000b479 	.word	0x0000b479
30005708:	42008000 	.word	0x42008000
3000570c:	30008fc9 	.word	0x30008fc9
30005710:	3000d5c5 	.word	0x3000d5c5
30005714:	0000aab9 	.word	0x0000aab9
30005718:	0000aea1 	.word	0x0000aea1
3000571c:	4200c000 	.word	0x4200c000
30005720:	0000aaa5 	.word	0x0000aaa5
30005724:	2001c00c 	.word	0x2001c00c
30005728:	3000cdc3 	.word	0x3000cdc3
3000572c:	300053fd 	.word	0x300053fd
30005730:	3000cdf3 	.word	0x3000cdf3
30005734:	00033000 	.word	0x00033000
30005738:	3000d5c8 	.word	0x3000d5c8
3000573c:	e000ed00 	.word	0xe000ed00
30005740:	2001c01c 	.word	0x2001c01c
30005744:	00009be5 	.word	0x00009be5
30005748:	3000c79c 	.word	0x3000c79c
3000574c:	3000ce23 	.word	0x3000ce23
30005750:	30008081 	.word	0x30008081
30005754:	3000a7a9 	.word	0x3000a7a9
30005758:	3000ce37 	.word	0x3000ce37
3000575c:	3000ceaf 	.word	0x3000ceaf
30005760:	3000cee5 	.word	0x3000cee5
30005764:	3000d5c4 	.word	0x3000d5c4
30005768:	0000c179 	.word	0x0000c179
3000576c:	0000c169 	.word	0x0000c169
30005770:	0016e360 	.word	0x0016e360
30005774:	3000cf06 	.word	0x3000cf06
30005778:	3000dbb4 	.word	0x3000dbb4
3000577c:	000129bd 	.word	0x000129bd
30005780:	3000dbd1 	.word	0x3000dbd1
30005784:	0000c0f9 	.word	0x0000c0f9
30005788:	9403      	str	r4, [sp, #12]
3000578a:	4681      	mov	r9, r0
3000578c:	2800      	cmp	r0, #0
3000578e:	d047      	beq.n	30005820 <BOOT_Image1+0x424>
30005790:	486d      	ldr	r0, [pc, #436]	; (30005948 <BOOT_Image1+0x54c>)
30005792:	f005 fdcd 	bl	3000b330 <__DiagPrintf_veneer>
30005796:	486d      	ldr	r0, [pc, #436]	; (3000594c <BOOT_Image1+0x550>)
30005798:	f005 fdca 	bl	3000b330 <__DiagPrintf_veneer>
3000579c:	e09d      	b.n	300058da <BOOT_Image1+0x4de>
3000579e:	486c      	ldr	r0, [pc, #432]	; (30005950 <BOOT_Image1+0x554>)
300057a0:	e635      	b.n	3000540e <BOOT_Image1+0x12>
300057a2:	2001      	movs	r0, #1
300057a4:	47b0      	blx	r6
300057a6:	4620      	mov	r0, r4
300057a8:	4b6a      	ldr	r3, [pc, #424]	; (30005954 <BOOT_Image1+0x558>)
300057aa:	4798      	blx	r3
300057ac:	4b6a      	ldr	r3, [pc, #424]	; (30005958 <BOOT_Image1+0x55c>)
300057ae:	2001      	movs	r0, #1
300057b0:	4798      	blx	r3
300057b2:	e650      	b.n	30005456 <BOOT_Image1+0x5a>
300057b4:	2401      	movs	r4, #1
300057b6:	4969      	ldr	r1, [pc, #420]	; (3000595c <BOOT_Image1+0x560>)
300057b8:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300057bc:	f882 410e 	strb.w	r4, [r2, #270]	; 0x10e
300057c0:	4622      	mov	r2, r4
300057c2:	47b8      	blx	r7
300057c4:	4622      	mov	r2, r4
300057c6:	4966      	ldr	r1, [pc, #408]	; (30005960 <BOOT_Image1+0x564>)
300057c8:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300057cc:	47b8      	blx	r7
300057ce:	4a65      	ldr	r2, [pc, #404]	; (30005964 <BOOT_Image1+0x568>)
300057d0:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
300057d4:	f043 0303 	orr.w	r3, r3, #3
300057d8:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
300057dc:	e65b      	b.n	30005496 <BOOT_Image1+0x9a>
300057de:	f043 0302 	orr.w	r3, r3, #2
300057e2:	e662      	b.n	300054aa <BOOT_Image1+0xae>
300057e4:	782b      	ldrb	r3, [r5, #0]
300057e6:	b15b      	cbz	r3, 30005800 <BOOT_Image1+0x404>
300057e8:	4d5f      	ldr	r5, [pc, #380]	; (30005968 <BOOT_Image1+0x56c>)
300057ea:	4c60      	ldr	r4, [pc, #384]	; (3000596c <BOOT_Image1+0x570>)
300057ec:	462b      	mov	r3, r5
300057ee:	4622      	mov	r2, r4
300057f0:	495d      	ldr	r1, [pc, #372]	; (30005968 <BOOT_Image1+0x56c>)
300057f2:	2002      	movs	r0, #2
300057f4:	f005 fb58 	bl	3000aea8 <rtk_log_write>
300057f8:	f241 3088 	movw	r0, #5000	; 0x1388
300057fc:	47b0      	blx	r6
300057fe:	e7f5      	b.n	300057ec <BOOT_Image1+0x3f0>
30005800:	68a3      	ldr	r3, [r4, #8]
30005802:	079b      	lsls	r3, r3, #30
30005804:	d502      	bpl.n	3000580c <BOOT_Image1+0x410>
30005806:	485a      	ldr	r0, [pc, #360]	; (30005970 <BOOT_Image1+0x574>)
30005808:	f005 fd92 	bl	3000b330 <__DiagPrintf_veneer>
3000580c:	f7fe fd58 	bl	300042c0 <BOOT_DDR_Init>
30005810:	f7fe fd4a 	bl	300042a8 <BOOT_DDR_LCDC_HPR>
30005814:	e6c6      	b.n	300055a4 <BOOT_Image1+0x1a8>
30005816:	4957      	ldr	r1, [pc, #348]	; (30005974 <BOOT_Image1+0x578>)
30005818:	e732      	b.n	30005680 <BOOT_Image1+0x284>
3000581a:	2301      	movs	r3, #1
3000581c:	9304      	str	r3, [sp, #16]
3000581e:	e744      	b.n	300056aa <BOOT_Image1+0x2ae>
30005820:	f001 fbfe 	bl	30007020 <ymodem_uart_readable>
30005824:	2800      	cmp	r0, #0
30005826:	d0fb      	beq.n	30005820 <BOOT_Image1+0x424>
30005828:	2101      	movs	r1, #1
3000582a:	4853      	ldr	r0, [pc, #332]	; (30005978 <BOOT_Image1+0x57c>)
3000582c:	f001 fc20 	bl	30007070 <ymodem_uart_getdata>
30005830:	f001 fbf6 	bl	30007020 <ymodem_uart_readable>
30005834:	2800      	cmp	r0, #0
30005836:	d0fb      	beq.n	30005830 <BOOT_Image1+0x434>
30005838:	4d50      	ldr	r5, [pc, #320]	; (3000597c <BOOT_Image1+0x580>)
3000583a:	2400      	movs	r4, #0
3000583c:	2101      	movs	r1, #1
3000583e:	484f      	ldr	r0, [pc, #316]	; (3000597c <BOOT_Image1+0x580>)
30005840:	f2a5 484e 	subw	r8, r5, #1102	; 0x44e
30005844:	f001 fc14 	bl	30007070 <ymodem_uart_getdata>
30005848:	f815 3b01 	ldrb.w	r3, [r5], #1
3000584c:	f898 24ce 	ldrb.w	r2, [r8, #1230]	; 0x4ce
30005850:	429a      	cmp	r2, r3
30005852:	d147      	bne.n	300058e4 <BOOT_Image1+0x4e8>
30005854:	4621      	mov	r1, r4
30005856:	f208 404e 	addw	r0, r8, #1102	; 0x44e
3000585a:	f7fe f890 	bl	3000397e <ascii_to_integer>
3000585e:	f8c8 04d0 	str.w	r0, [r8, #1232]	; 0x4d0
30005862:	f001 fbdd 	bl	30007020 <ymodem_uart_readable>
30005866:	2800      	cmp	r0, #0
30005868:	d0fb      	beq.n	30005862 <BOOT_Image1+0x466>
3000586a:	4d45      	ldr	r5, [pc, #276]	; (30005980 <BOOT_Image1+0x584>)
3000586c:	f104 070a 	add.w	r7, r4, #10
30005870:	f04f 0a00 	mov.w	sl, #0
30005874:	2101      	movs	r1, #1
30005876:	442f      	add	r7, r5
30005878:	4638      	mov	r0, r7
3000587a:	f001 fbf9 	bl	30007070 <ymodem_uart_getdata>
3000587e:	eb0a 0304 	add.w	r3, sl, r4
30005882:	f898 24ce 	ldrb.w	r2, [r8, #1230]	; 0x4ce
30005886:	9305      	str	r3, [sp, #20]
30005888:	f817 3b01 	ldrb.w	r3, [r7], #1
3000588c:	429a      	cmp	r2, r3
3000588e:	d133      	bne.n	300058f8 <BOOT_Image1+0x4fc>
30005890:	f001 fbc6 	bl	30007020 <ymodem_uart_readable>
30005894:	2800      	cmp	r0, #0
30005896:	d0fb      	beq.n	30005890 <BOOT_Image1+0x494>
30005898:	9b05      	ldr	r3, [sp, #20]
3000589a:	330b      	adds	r3, #11
3000589c:	eb05 0b03 	add.w	fp, r5, r3
300058a0:	2101      	movs	r1, #1
300058a2:	4658      	mov	r0, fp
300058a4:	f001 fbe4 	bl	30007070 <ymodem_uart_getdata>
300058a8:	f81b 2b01 	ldrb.w	r2, [fp], #1
300058ac:	f898 14ce 	ldrb.w	r1, [r8, #1230]	; 0x4ce
300058b0:	9b05      	ldr	r3, [sp, #20]
300058b2:	4291      	cmp	r1, r2
300058b4:	eb09 0703 	add.w	r7, r9, r3
300058b8:	d129      	bne.n	3000590e <BOOT_Image1+0x512>
300058ba:	f107 0b0c 	add.w	fp, r7, #12
300058be:	2101      	movs	r1, #1
300058c0:	eb05 000b 	add.w	r0, r5, fp
300058c4:	f001 fbd4 	bl	30007070 <ymodem_uart_getdata>
300058c8:	f815 100b 	ldrb.w	r1, [r5, fp]
300058cc:	290a      	cmp	r1, #10
300058ce:	f8c8 14d4 	str.w	r1, [r8, #1236]	; 0x4d4
300058d2:	d923      	bls.n	3000591c <BOOT_Image1+0x520>
300058d4:	482b      	ldr	r0, [pc, #172]	; (30005984 <BOOT_Image1+0x588>)
300058d6:	f005 fd2b 	bl	3000b330 <__DiagPrintf_veneer>
300058da:	9b04      	ldr	r3, [sp, #16]
300058dc:	2b00      	cmp	r3, #0
300058de:	f040 80bf 	bne.w	30005a60 <BOOT_Image1+0x664>
300058e2:	e0b7      	b.n	30005a54 <BOOT_Image1+0x658>
300058e4:	3401      	adds	r4, #1
300058e6:	f001 fb9b 	bl	30007020 <ymodem_uart_readable>
300058ea:	2800      	cmp	r0, #0
300058ec:	d0fb      	beq.n	300058e6 <BOOT_Image1+0x4ea>
300058ee:	2101      	movs	r1, #1
300058f0:	4628      	mov	r0, r5
300058f2:	f001 fbbd 	bl	30007070 <ymodem_uart_getdata>
300058f6:	e7a7      	b.n	30005848 <BOOT_Image1+0x44c>
300058f8:	f10a 0a01 	add.w	sl, sl, #1
300058fc:	f001 fb90 	bl	30007020 <ymodem_uart_readable>
30005900:	2800      	cmp	r0, #0
30005902:	d0fb      	beq.n	300058fc <BOOT_Image1+0x500>
30005904:	2101      	movs	r1, #1
30005906:	4638      	mov	r0, r7
30005908:	f001 fbb2 	bl	30007070 <ymodem_uart_getdata>
3000590c:	e7b7      	b.n	3000587e <BOOT_Image1+0x482>
3000590e:	f109 0901 	add.w	r9, r9, #1
30005912:	f001 fb85 	bl	30007020 <ymodem_uart_readable>
30005916:	2800      	cmp	r0, #0
30005918:	d0fb      	beq.n	30005912 <BOOT_Image1+0x516>
3000591a:	e7c1      	b.n	300058a0 <BOOT_Image1+0x4a4>
3000591c:	f107 0b0e 	add.w	fp, r7, #14
30005920:	f107 000d 	add.w	r0, r7, #13
30005924:	2101      	movs	r1, #1
30005926:	44ab      	add	fp, r5
30005928:	4428      	add	r0, r5
3000592a:	f001 fba1 	bl	30007070 <ymodem_uart_getdata>
3000592e:	2102      	movs	r1, #2
30005930:	4658      	mov	r0, fp
30005932:	f001 fb9d 	bl	30007070 <ymodem_uart_getdata>
30005936:	2202      	movs	r2, #2
30005938:	4659      	mov	r1, fp
3000593a:	4813      	ldr	r0, [pc, #76]	; (30005988 <BOOT_Image1+0x58c>)
3000593c:	9b03      	ldr	r3, [sp, #12]
3000593e:	4798      	blx	r3
30005940:	b330      	cbz	r0, 30005990 <BOOT_Image1+0x594>
30005942:	4812      	ldr	r0, [pc, #72]	; (3000598c <BOOT_Image1+0x590>)
30005944:	e728      	b.n	30005798 <BOOT_Image1+0x39c>
30005946:	bf00      	nop
30005948:	3000cf1c 	.word	0x3000cf1c
3000594c:	3000cf34 	.word	0x3000cf34
30005950:	3000cd98 	.word	0x3000cd98
30005954:	3000a8c9 	.word	0x3000a8c9
30005958:	3000a83d 	.word	0x3000a83d
3000595c:	40080000 	.word	0x40080000
30005960:	40040000 	.word	0x40040000
30005964:	42008000 	.word	0x42008000
30005968:	3000c79c 	.word	0x3000c79c
3000596c:	3000ce4d 	.word	0x3000ce4d
30005970:	3000ce8b 	.word	0x3000ce8b
30005974:	0016e360 	.word	0x0016e360
30005978:	3000dbd9 	.word	0x3000dbd9
3000597c:	3000dbda 	.word	0x3000dbda
30005980:	3000dbd1 	.word	0x3000dbd1
30005984:	3000cf42 	.word	0x3000cf42
30005988:	3000dc64 	.word	0x3000dc64
3000598c:	3000cf53 	.word	0x3000cf53
30005990:	f107 0010 	add.w	r0, r7, #16
30005994:	2101      	movs	r1, #1
30005996:	f107 0b11 	add.w	fp, r7, #17
3000599a:	4428      	add	r0, r5
3000599c:	f001 fb68 	bl	30007070 <ymodem_uart_getdata>
300059a0:	4b8e      	ldr	r3, [pc, #568]	; (30005bdc <BOOT_Image1+0x7e0>)
300059a2:	44ab      	add	fp, r5
300059a4:	f8d8 04d4 	ldr.w	r0, [r8, #1236]	; 0x4d4
300059a8:	f103 0228 	add.w	r2, r3, #40	; 0x28
300059ac:	f103 0130 	add.w	r1, r3, #48	; 0x30
300059b0:	f7fe f809 	bl	300039c6 <hotfix_get_addr_size>
300059b4:	2102      	movs	r1, #2
300059b6:	4658      	mov	r0, fp
300059b8:	f001 fb5a 	bl	30007070 <ymodem_uart_getdata>
300059bc:	2202      	movs	r2, #2
300059be:	4659      	mov	r1, fp
300059c0:	4887      	ldr	r0, [pc, #540]	; (30005be0 <BOOT_Image1+0x7e4>)
300059c2:	9b03      	ldr	r3, [sp, #12]
300059c4:	4798      	blx	r3
300059c6:	2800      	cmp	r0, #0
300059c8:	d187      	bne.n	300058da <BOOT_Image1+0x4de>
300059ca:	f107 0013 	add.w	r0, r7, #19
300059ce:	2101      	movs	r1, #1
300059d0:	f107 0b14 	add.w	fp, r7, #20
300059d4:	4428      	add	r0, r5
300059d6:	f001 fb4b 	bl	30007070 <ymodem_uart_getdata>
300059da:	4b82      	ldr	r3, [pc, #520]	; (30005be4 <BOOT_Image1+0x7e8>)
300059dc:	44ab      	add	fp, r5
300059de:	f8d8 04d4 	ldr.w	r0, [r8, #1236]	; 0x4d4
300059e2:	f1a3 0218 	sub.w	r2, r3, #24
300059e6:	f1a3 0110 	sub.w	r1, r3, #16
300059ea:	f7fd ffec 	bl	300039c6 <hotfix_get_addr_size>
300059ee:	2102      	movs	r1, #2
300059f0:	4658      	mov	r0, fp
300059f2:	f001 fb3d 	bl	30007070 <ymodem_uart_getdata>
300059f6:	2202      	movs	r2, #2
300059f8:	4659      	mov	r1, fp
300059fa:	487b      	ldr	r0, [pc, #492]	; (30005be8 <BOOT_Image1+0x7ec>)
300059fc:	9b03      	ldr	r3, [sp, #12]
300059fe:	4798      	blx	r3
30005a00:	2800      	cmp	r0, #0
30005a02:	f47f af6a 	bne.w	300058da <BOOT_Image1+0x4de>
30005a06:	f107 0016 	add.w	r0, r7, #22
30005a0a:	2101      	movs	r1, #1
30005a0c:	4428      	add	r0, r5
30005a0e:	f001 fb2f 	bl	30007070 <ymodem_uart_getdata>
30005a12:	f8d8 04d4 	ldr.w	r0, [r8, #1236]	; 0x4d4
30005a16:	f107 0817 	add.w	r8, r7, #23
30005a1a:	4b74      	ldr	r3, [pc, #464]	; (30005bec <BOOT_Image1+0x7f0>)
30005a1c:	44a8      	add	r8, r5
30005a1e:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
30005a22:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
30005a26:	f7fd ffce 	bl	300039c6 <hotfix_get_addr_size>
30005a2a:	2102      	movs	r1, #2
30005a2c:	4640      	mov	r0, r8
30005a2e:	f001 fb1f 	bl	30007070 <ymodem_uart_getdata>
30005a32:	2202      	movs	r2, #2
30005a34:	4641      	mov	r1, r8
30005a36:	486e      	ldr	r0, [pc, #440]	; (30005bf0 <BOOT_Image1+0x7f4>)
30005a38:	9b03      	ldr	r3, [sp, #12]
30005a3a:	4798      	blx	r3
30005a3c:	2800      	cmp	r0, #0
30005a3e:	f47f af4c 	bne.w	300058da <BOOT_Image1+0x4de>
30005a42:	f107 0019 	add.w	r0, r7, #25
30005a46:	2101      	movs	r1, #1
30005a48:	4428      	add	r0, r5
30005a4a:	f001 fb11 	bl	30007070 <ymodem_uart_getdata>
30005a4e:	9b04      	ldr	r3, [sp, #16]
30005a50:	2b00      	cmp	r3, #0
30005a52:	d162      	bne.n	30005b1a <BOOT_Image1+0x71e>
30005a54:	2002      	movs	r0, #2
30005a56:	f001 facf 	bl	30006ff8 <ymodem_uart_port_deinit>
30005a5a:	4866      	ldr	r0, [pc, #408]	; (30005bf4 <BOOT_Image1+0x7f8>)
30005a5c:	f005 fc68 	bl	3000b330 <__DiagPrintf_veneer>
30005a60:	f005 fa36 	bl	3000aed0 <BOOT_VerCheck>
30005a64:	f7fe fd9e 	bl	300045a4 <BOOT_LoadImages>
30005a68:	2800      	cmp	r0, #0
30005a6a:	f000 80b3 	beq.w	30005bd4 <BOOT_Image1+0x7d8>
30005a6e:	f7ff f999 	bl	30004da4 <BOOT_Enable_KM0>
30005a72:	2200      	movs	r2, #0
30005a74:	2101      	movs	r1, #1
30005a76:	4860      	ldr	r0, [pc, #384]	; (30005bf8 <BOOT_Image1+0x7fc>)
30005a78:	4b60      	ldr	r3, [pc, #384]	; (30005bfc <BOOT_Image1+0x800>)
30005a7a:	4798      	blx	r3
30005a7c:	4b60      	ldr	r3, [pc, #384]	; (30005c00 <BOOT_Image1+0x804>)
30005a7e:	2008      	movs	r0, #8
30005a80:	4798      	blx	r3
30005a82:	f001 f85d 	bl	30006b40 <BOOT_RAM_TZCfg>
30005a86:	4b5f      	ldr	r3, [pc, #380]	; (30005c04 <BOOT_Image1+0x808>)
30005a88:	781b      	ldrb	r3, [r3, #0]
30005a8a:	2b01      	cmp	r3, #1
30005a8c:	d106      	bne.n	30005a9c <BOOT_Image1+0x6a0>
30005a8e:	4b5e      	ldr	r3, [pc, #376]	; (30005c08 <BOOT_Image1+0x80c>)
30005a90:	4798      	blx	r3
30005a92:	2801      	cmp	r0, #1
30005a94:	bf0c      	ite	eq
30005a96:	2019      	moveq	r0, #25
30005a98:	2004      	movne	r0, #4
30005a9a:	47b0      	blx	r6
30005a9c:	4b5b      	ldr	r3, [pc, #364]	; (30005c0c <BOOT_Image1+0x810>)
30005a9e:	781b      	ldrb	r3, [r3, #0]
30005aa0:	2b00      	cmp	r3, #0
30005aa2:	f000 8094 	beq.w	30005bce <BOOT_Image1+0x7d2>
30005aa6:	f7ff fa9b 	bl	30004fe0 <BOOT_Enable_AP>
30005aaa:	4a59      	ldr	r2, [pc, #356]	; (30005c10 <BOOT_Image1+0x814>)
30005aac:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
30005ab0:	f043 0306 	orr.w	r3, r3, #6
30005ab4:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
30005ab8:	4b52      	ldr	r3, [pc, #328]	; (30005c04 <BOOT_Image1+0x808>)
30005aba:	781b      	ldrb	r3, [r3, #0]
30005abc:	2b01      	cmp	r3, #1
30005abe:	d106      	bne.n	30005ace <BOOT_Image1+0x6d2>
30005ac0:	4b51      	ldr	r3, [pc, #324]	; (30005c08 <BOOT_Image1+0x80c>)
30005ac2:	4798      	blx	r3
30005ac4:	2801      	cmp	r0, #1
30005ac6:	bf0c      	ite	eq
30005ac8:	20e6      	moveq	r0, #230	; 0xe6
30005aca:	2019      	movne	r0, #25
30005acc:	47b0      	blx	r6
30005ace:	4a50      	ldr	r2, [pc, #320]	; (30005c10 <BOOT_Image1+0x814>)
30005ad0:	f892 3067 	ldrb.w	r3, [r2, #103]	; 0x67
30005ad4:	f043 0301 	orr.w	r3, r3, #1
30005ad8:	f882 3067 	strb.w	r3, [r2, #103]	; 0x67
30005adc:	4b4d      	ldr	r3, [pc, #308]	; (30005c14 <BOOT_Image1+0x818>)
30005ade:	4a4e      	ldr	r2, [pc, #312]	; (30005c18 <BOOT_Image1+0x81c>)
30005ae0:	689c      	ldr	r4, [r3, #8]
30005ae2:	681b      	ldr	r3, [r3, #0]
30005ae4:	6063      	str	r3, [r4, #4]
30005ae6:	4b4d      	ldr	r3, [pc, #308]	; (30005c1c <BOOT_Image1+0x820>)
30005ae8:	609c      	str	r4, [r3, #8]
30005aea:	4b4d      	ldr	r3, [pc, #308]	; (30005c20 <BOOT_Image1+0x824>)
30005aec:	61da      	str	r2, [r3, #28]
30005aee:	f7fe fc73 	bl	300043d8 <BOOT_SCBConfig_HP>
30005af2:	f7fe fc93 	bl	3000441c <BOOT_SCBVTORBackup_HP>
30005af6:	f7fe fca5 	bl	30004444 <BOOT_NVICBackup_HP>
30005afa:	4b4a      	ldr	r3, [pc, #296]	; (30005c24 <BOOT_Image1+0x828>)
30005afc:	f383 8888 	msr	MSP_NS, r3
30005b00:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
30005b04:	f383 8809 	msr	PSP, r3
30005b08:	6860      	ldr	r0, [r4, #4]
30005b0a:	f7fe f96d 	bl	30003de8 <BOOT_NsStart>
30005b0e:	b00b      	add	sp, #44	; 0x2c
30005b10:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005b14:	4681      	mov	r9, r0
30005b16:	4682      	mov	sl, r0
30005b18:	e799      	b.n	30005a4e <BOOT_Image1+0x652>
30005b1a:	2004      	movs	r0, #4
30005b1c:	4d42      	ldr	r5, [pc, #264]	; (30005c28 <BOOT_Image1+0x82c>)
30005b1e:	f001 fa6b 	bl	30006ff8 <ymodem_uart_port_deinit>
30005b22:	eb04 030a 	add.w	r3, r4, sl
30005b26:	f205 4045 	addw	r0, r5, #1093	; 0x445
30005b2a:	340a      	adds	r4, #10
30005b2c:	330b      	adds	r3, #11
30005b2e:	f8cd 9000 	str.w	r9, [sp]
30005b32:	4652      	mov	r2, sl
30005b34:	f8d5 14d0 	ldr.w	r1, [r5, #1232]	; 0x4d0
30005b38:	4403      	add	r3, r0
30005b3a:	4420      	add	r0, r4
30005b3c:	f7ff fb96 	bl	3000526c <password_hash_check>
30005b40:	4604      	mov	r4, r0
30005b42:	2800      	cmp	r0, #0
30005b44:	d186      	bne.n	30005a54 <BOOT_Image1+0x658>
30005b46:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005b4a:	f505 69a9 	add.w	r9, r5, #1352	; 0x548
30005b4e:	47b0      	blx	r6
30005b50:	4836      	ldr	r0, [pc, #216]	; (30005c2c <BOOT_Image1+0x830>)
30005b52:	f005 fc85 	bl	3000b460 <____wrap_strlen_veneer>
30005b56:	4601      	mov	r1, r0
30005b58:	4834      	ldr	r0, [pc, #208]	; (30005c2c <BOOT_Image1+0x830>)
30005b5a:	f205 5a1c 	addw	sl, r5, #1308	; 0x51c
30005b5e:	f001 fa79 	bl	30007054 <ymodem_uart_putdata>
30005b62:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005b66:	47b0      	blx	r6
30005b68:	f8d5 24d4 	ldr.w	r2, [r5, #1236]	; 0x4d4
30005b6c:	f205 41dc 	addw	r1, r5, #1244	; 0x4dc
30005b70:	2002      	movs	r0, #2
30005b72:	f001 faf5 	bl	30007160 <ymodem_img_download>
30005b76:	2032      	movs	r0, #50	; 0x32
30005b78:	47b0      	blx	r6
30005b7a:	f8d5 34d4 	ldr.w	r3, [r5, #1236]	; 0x4d4
30005b7e:	42a3      	cmp	r3, r4
30005b80:	d811      	bhi.n	30005ba6 <BOOT_Image1+0x7aa>
30005b82:	482b      	ldr	r0, [pc, #172]	; (30005c30 <BOOT_Image1+0x834>)
30005b84:	f005 fbd4 	bl	3000b330 <__DiagPrintf_veneer>
30005b88:	a808      	add	r0, sp, #32
30005b8a:	4b2a      	ldr	r3, [pc, #168]	; (30005c34 <BOOT_Image1+0x838>)
30005b8c:	4798      	blx	r3
30005b8e:	f44f 73fa 	mov.w	r3, #500	; 0x1f4
30005b92:	4829      	ldr	r0, [pc, #164]	; (30005c38 <BOOT_Image1+0x83c>)
30005b94:	a908      	add	r1, sp, #32
30005b96:	f8ad 3022 	strh.w	r3, [sp, #34]	; 0x22
30005b9a:	4b28      	ldr	r3, [pc, #160]	; (30005c3c <BOOT_Image1+0x840>)
30005b9c:	4798      	blx	r3
30005b9e:	4826      	ldr	r0, [pc, #152]	; (30005c38 <BOOT_Image1+0x83c>)
30005ba0:	4b27      	ldr	r3, [pc, #156]	; (30005c40 <BOOT_Image1+0x844>)
30005ba2:	4798      	blx	r3
30005ba4:	e7fe      	b.n	30005ba4 <BOOT_Image1+0x7a8>
30005ba6:	f859 3b04 	ldr.w	r3, [r9], #4
30005baa:	2700      	movs	r7, #0
30005bac:	ea4f 3813 	mov.w	r8, r3, lsr #12
30005bb0:	4547      	cmp	r7, r8
30005bb2:	db01      	blt.n	30005bb8 <BOOT_Image1+0x7bc>
30005bb4:	3401      	adds	r4, #1
30005bb6:	e7e0      	b.n	30005b7a <BOOT_Image1+0x77e>
30005bb8:	f85a 1024 	ldr.w	r1, [sl, r4, lsl #2]
30005bbc:	2002      	movs	r0, #2
30005bbe:	eb01 3107 	add.w	r1, r1, r7, lsl #12
30005bc2:	3701      	adds	r7, #1
30005bc4:	f7fd ffaa 	bl	30003b1c <FLASH_Erase_With_Lock>
30005bc8:	2032      	movs	r0, #50	; 0x32
30005bca:	47b0      	blx	r6
30005bcc:	e7f0      	b.n	30005bb0 <BOOT_Image1+0x7b4>
30005bce:	f7ff f9f5 	bl	30004fbc <BOOT_Disable_AP>
30005bd2:	e771      	b.n	30005ab8 <BOOT_Image1+0x6bc>
30005bd4:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30005bd8:	47b0      	blx	r6
30005bda:	e7fb      	b.n	30005bd4 <BOOT_Image1+0x7d8>
30005bdc:	3000dc68 	.word	0x3000dc68
30005be0:	3000dca3 	.word	0x3000dca3
30005be4:	3000dca8 	.word	0x3000dca8
30005be8:	3000dcd0 	.word	0x3000dcd0
30005bec:	3000dcd4 	.word	0x3000dcd4
30005bf0:	3000dcfc 	.word	0x3000dcfc
30005bf4:	3000cf34 	.word	0x3000cf34
30005bf8:	4200c000 	.word	0x4200c000
30005bfc:	0000ac6d 	.word	0x0000ac6d
30005c00:	0000e1d9 	.word	0x0000e1d9
30005c04:	3000d5c4 	.word	0x3000d5c4
30005c08:	0000c169 	.word	0x0000c169
30005c0c:	3000d5c7 	.word	0x3000d5c7
30005c10:	42008200 	.word	0x42008200
30005c14:	60000020 	.word	0x60000020
30005c18:	30003941 	.word	0x30003941
30005c1c:	e002ed00 	.word	0xe002ed00
30005c20:	30000000 	.word	0x30000000
30005c24:	2001bffc 	.word	0x2001bffc
30005c28:	3000d78c 	.word	0x3000d78c
30005c2c:	3000e588 	.word	0x3000e588
30005c30:	3000cf64 	.word	0x3000cf64
30005c34:	0000d3f9 	.word	0x0000d3f9
30005c38:	41000440 	.word	0x41000440
30005c3c:	0000d409 	.word	0x0000d409
30005c40:	0000d489 	.word	0x0000d489

30005c44 <BOOT_LoadSubImage>:
30005c44:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005c48:	4607      	mov	r7, r0
30005c4a:	b08d      	sub	sp, #52	; 0x34
30005c4c:	460d      	mov	r5, r1
30005c4e:	4692      	mov	sl, r2
30005c50:	4699      	mov	r9, r3
30005c52:	2600      	movs	r6, #0
30005c54:	f8df 8120 	ldr.w	r8, [pc, #288]	; 30005d78 <BOOT_LoadSubImage+0x134>
30005c58:	4556      	cmp	r6, sl
30005c5a:	d303      	bcc.n	30005c64 <BOOT_LoadSubImage+0x20>
30005c5c:	4b40      	ldr	r3, [pc, #256]	; (30005d60 <BOOT_LoadSubImage+0x11c>)
30005c5e:	2001      	movs	r0, #1
30005c60:	601d      	str	r5, [r3, #0]
30005c62:	e00d      	b.n	30005c80 <BOOT_LoadSubImage+0x3c>
30005c64:	2220      	movs	r2, #32
30005c66:	4629      	mov	r1, r5
30005c68:	a804      	add	r0, sp, #16
30005c6a:	f7fe fc81 	bl	30004570 <BOOT_ImgCopy>
30005c6e:	9b04      	ldr	r3, [sp, #16]
30005c70:	4a3c      	ldr	r2, [pc, #240]	; (30005d64 <BOOT_LoadSubImage+0x120>)
30005c72:	4293      	cmp	r3, r2
30005c74:	d007      	beq.n	30005c86 <BOOT_LoadSubImage+0x42>
30005c76:	f89d 3058 	ldrb.w	r3, [sp, #88]	; 0x58
30005c7a:	2b01      	cmp	r3, #1
30005c7c:	d053      	beq.n	30005d26 <BOOT_LoadSubImage+0xe2>
30005c7e:	2000      	movs	r0, #0
30005c80:	b00d      	add	sp, #52	; 0x34
30005c82:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005c86:	9b05      	ldr	r3, [sp, #20]
30005c88:	4a37      	ldr	r2, [pc, #220]	; (30005d68 <BOOT_LoadSubImage+0x124>)
30005c8a:	4293      	cmp	r3, r2
30005c8c:	d1f3      	bne.n	30005c76 <BOOT_LoadSubImage+0x32>
30005c8e:	9b07      	ldr	r3, [sp, #28]
30005c90:	9c06      	ldr	r4, [sp, #24]
30005c92:	f1a3 0b20 	sub.w	fp, r3, #32
30005c96:	f103 4378 	add.w	r3, r3, #4160749568	; 0xf8000000
30005c9a:	3420      	adds	r4, #32
30005c9c:	3b20      	subs	r3, #32
30005c9e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005ca2:	d32c      	bcc.n	30005cfe <BOOT_LoadSubImage+0xba>
30005ca4:	4622      	mov	r2, r4
30005ca6:	4629      	mov	r1, r5
30005ca8:	4658      	mov	r0, fp
30005caa:	f7fe fc61 	bl	30004570 <BOOT_ImgCopy>
30005cae:	f8d8 3014 	ldr.w	r3, [r8, #20]
30005cb2:	03da      	lsls	r2, r3, #15
30005cb4:	d523      	bpl.n	30005cfe <BOOT_LoadSubImage+0xba>
30005cb6:	f1bb 3fff 	cmp.w	fp, #4294967295	; 0xffffffff
30005cba:	d13e      	bne.n	30005d3a <BOOT_LoadSubImage+0xf6>
30005cbc:	1c63      	adds	r3, r4, #1
30005cbe:	d13c      	bne.n	30005d3a <BOOT_LoadSubImage+0xf6>
30005cc0:	2300      	movs	r3, #0
30005cc2:	f8c8 3084 	str.w	r3, [r8, #132]	; 0x84
30005cc6:	f3bf 8f4f 	dsb	sy
30005cca:	f8d8 2080 	ldr.w	r2, [r8, #128]	; 0x80
30005cce:	f643 70e0 	movw	r0, #16352	; 0x3fe0
30005cd2:	f3c2 01c9 	ubfx	r1, r2, #3, #10
30005cd6:	f3c2 324e 	ubfx	r2, r2, #13, #15
30005cda:	0152      	lsls	r2, r2, #5
30005cdc:	ea02 0e00 	and.w	lr, r2, r0
30005ce0:	460b      	mov	r3, r1
30005ce2:	ea4e 7c83 	orr.w	ip, lr, r3, lsl #30
30005ce6:	3b01      	subs	r3, #1
30005ce8:	f8c8 c274 	str.w	ip, [r8, #628]	; 0x274
30005cec:	d2f9      	bcs.n	30005ce2 <BOOT_LoadSubImage+0x9e>
30005cee:	3a20      	subs	r2, #32
30005cf0:	f112 0f20 	cmn.w	r2, #32
30005cf4:	d1f2      	bne.n	30005cdc <BOOT_LoadSubImage+0x98>
30005cf6:	f3bf 8f4f 	dsb	sy
30005cfa:	f3bf 8f6f 	isb	sy
30005cfe:	b17f      	cbz	r7, 30005d20 <BOOT_LoadSubImage+0xdc>
30005d00:	1d3b      	adds	r3, r7, #4
30005d02:	f847 b036 	str.w	fp, [r7, r6, lsl #3]
30005d06:	4a19      	ldr	r2, [pc, #100]	; (30005d6c <BOOT_LoadSubImage+0x128>)
30005d08:	2004      	movs	r0, #4
30005d0a:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
30005d0e:	f859 3026 	ldr.w	r3, [r9, r6, lsl #2]
30005d12:	9300      	str	r3, [sp, #0]
30005d14:	4b16      	ldr	r3, [pc, #88]	; (30005d70 <BOOT_LoadSubImage+0x12c>)
30005d16:	e9cd b401 	strd	fp, r4, [sp, #4]
30005d1a:	4619      	mov	r1, r3
30005d1c:	f005 f8c4 	bl	3000aea8 <rtk_log_write>
30005d20:	4425      	add	r5, r4
30005d22:	3601      	adds	r6, #1
30005d24:	e798      	b.n	30005c58 <BOOT_LoadSubImage+0x14>
30005d26:	f859 3026 	ldr.w	r3, [r9, r6, lsl #2]
30005d2a:	2004      	movs	r0, #4
30005d2c:	4a11      	ldr	r2, [pc, #68]	; (30005d74 <BOOT_LoadSubImage+0x130>)
30005d2e:	9300      	str	r3, [sp, #0]
30005d30:	4b0f      	ldr	r3, [pc, #60]	; (30005d70 <BOOT_LoadSubImage+0x12c>)
30005d32:	4619      	mov	r1, r3
30005d34:	f005 f8b8 	bl	3000aea8 <rtk_log_write>
30005d38:	e7a1      	b.n	30005c7e <BOOT_LoadSubImage+0x3a>
30005d3a:	f01b 031f 	ands.w	r3, fp, #31
30005d3e:	bf19      	ittee	ne
30005d40:	f02b 021f 	bicne.w	r2, fp, #31
30005d44:	191b      	addne	r3, r3, r4
30005d46:	4623      	moveq	r3, r4
30005d48:	465a      	moveq	r2, fp
30005d4a:	f3bf 8f4f 	dsb	sy
30005d4e:	441a      	add	r2, r3
30005d50:	2b00      	cmp	r3, #0
30005d52:	eba2 0103 	sub.w	r1, r2, r3
30005d56:	ddce      	ble.n	30005cf6 <BOOT_LoadSubImage+0xb2>
30005d58:	3b20      	subs	r3, #32
30005d5a:	f8c8 1270 	str.w	r1, [r8, #624]	; 0x270
30005d5e:	e7f7      	b.n	30005d50 <BOOT_LoadSubImage+0x10c>
30005d60:	3000dd00 	.word	0x3000dd00
30005d64:	35393138 	.word	0x35393138
30005d68:	31313738 	.word	0x31313738
30005d6c:	3000d02d 	.word	0x3000d02d
30005d70:	3000c79c 	.word	0x3000c79c
30005d74:	3000d01a 	.word	0x3000d01a
30005d78:	e000ed00 	.word	0xe000ed00

30005d7c <BOOT_LoadAPSubImage>:
30005d7c:	4b9c      	ldr	r3, [pc, #624]	; (30005ff0 <BOOT_LoadAPSubImage+0x274>)
30005d7e:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30005d82:	b08e      	sub	sp, #56	; 0x38
30005d84:	4607      	mov	r7, r0
30005d86:	4614      	mov	r4, r2
30005d88:	6818      	ldr	r0, [r3, #0]
30005d8a:	889a      	ldrh	r2, [r3, #4]
30005d8c:	ae04      	add	r6, sp, #16
30005d8e:	799b      	ldrb	r3, [r3, #6]
30005d90:	4689      	mov	r9, r1
30005d92:	9004      	str	r0, [sp, #16]
30005d94:	a806      	add	r0, sp, #24
30005d96:	f8ad 2014 	strh.w	r2, [sp, #20]
30005d9a:	2220      	movs	r2, #32
30005d9c:	71b3      	strb	r3, [r6, #6]
30005d9e:	f7fe fbe7 	bl	30004570 <BOOT_ImgCopy>
30005da2:	4b94      	ldr	r3, [pc, #592]	; (30005ff4 <BOOT_LoadAPSubImage+0x278>)
30005da4:	9a06      	ldr	r2, [sp, #24]
30005da6:	429a      	cmp	r2, r3
30005da8:	d108      	bne.n	30005dbc <BOOT_LoadAPSubImage+0x40>
30005daa:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30005dae:	9a07      	ldr	r2, [sp, #28]
30005db0:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30005db4:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30005db8:	429a      	cmp	r2, r3
30005dba:	d00a      	beq.n	30005dd2 <BOOT_LoadAPSubImage+0x56>
30005dbc:	2c01      	cmp	r4, #1
30005dbe:	d106      	bne.n	30005dce <BOOT_LoadAPSubImage+0x52>
30005dc0:	4b8d      	ldr	r3, [pc, #564]	; (30005ff8 <BOOT_LoadAPSubImage+0x27c>)
30005dc2:	2004      	movs	r0, #4
30005dc4:	4a8d      	ldr	r2, [pc, #564]	; (30005ffc <BOOT_LoadAPSubImage+0x280>)
30005dc6:	4619      	mov	r1, r3
30005dc8:	9600      	str	r6, [sp, #0]
30005dca:	f005 f86d 	bl	3000aea8 <rtk_log_write>
30005dce:	2000      	movs	r0, #0
30005dd0:	e053      	b.n	30005e7a <BOOT_LoadAPSubImage+0xfe>
30005dd2:	9b09      	ldr	r3, [sp, #36]	; 0x24
30005dd4:	f8d9 5048 	ldr.w	r5, [r9, #72]	; 0x48
30005dd8:	f1a3 0420 	sub.w	r4, r3, #32
30005ddc:	f103 4378 	add.w	r3, r3, #4160749568	; 0xf8000000
30005de0:	35d0      	adds	r5, #208	; 0xd0
30005de2:	3b20      	subs	r3, #32
30005de4:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005de8:	d32b      	bcc.n	30005e42 <BOOT_LoadAPSubImage+0xc6>
30005dea:	462a      	mov	r2, r5
30005dec:	4649      	mov	r1, r9
30005dee:	4620      	mov	r0, r4
30005df0:	f7fe fbbe 	bl	30004570 <BOOT_ImgCopy>
30005df4:	4a82      	ldr	r2, [pc, #520]	; (30006000 <BOOT_LoadAPSubImage+0x284>)
30005df6:	6953      	ldr	r3, [r2, #20]
30005df8:	03d9      	lsls	r1, r3, #15
30005dfa:	d522      	bpl.n	30005e42 <BOOT_LoadAPSubImage+0xc6>
30005dfc:	1c63      	adds	r3, r4, #1
30005dfe:	d13f      	bne.n	30005e80 <BOOT_LoadAPSubImage+0x104>
30005e00:	1c68      	adds	r0, r5, #1
30005e02:	d13d      	bne.n	30005e80 <BOOT_LoadAPSubImage+0x104>
30005e04:	2300      	movs	r3, #0
30005e06:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30005e0a:	f3bf 8f4f 	dsb	sy
30005e0e:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30005e12:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30005e16:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30005e1a:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005e1e:	015b      	lsls	r3, r3, #5
30005e20:	ea03 080c 	and.w	r8, r3, ip
30005e24:	4601      	mov	r1, r0
30005e26:	ea48 7e81 	orr.w	lr, r8, r1, lsl #30
30005e2a:	3901      	subs	r1, #1
30005e2c:	f8c2 e274 	str.w	lr, [r2, #628]	; 0x274
30005e30:	d2f9      	bcs.n	30005e26 <BOOT_LoadAPSubImage+0xaa>
30005e32:	3b20      	subs	r3, #32
30005e34:	f113 0f20 	cmn.w	r3, #32
30005e38:	d1f2      	bne.n	30005e20 <BOOT_LoadAPSubImage+0xa4>
30005e3a:	f3bf 8f4f 	dsb	sy
30005e3e:	f3bf 8f6f 	isb	sy
30005e42:	4b70      	ldr	r3, [pc, #448]	; (30006004 <BOOT_LoadAPSubImage+0x288>)
30005e44:	442c      	add	r4, r5
30005e46:	f8d9 8070 	ldr.w	r8, [r9, #112]	; 0x70
30005e4a:	4798      	blx	r3
30005e4c:	2801      	cmp	r0, #1
30005e4e:	444d      	add	r5, r9
30005e50:	4b69      	ldr	r3, [pc, #420]	; (30005ff8 <BOOT_LoadAPSubImage+0x27c>)
30005e52:	d129      	bne.n	30005ea8 <BOOT_LoadAPSubImage+0x12c>
30005e54:	4a6c      	ldr	r2, [pc, #432]	; (30006008 <BOOT_LoadAPSubImage+0x28c>)
30005e56:	4619      	mov	r1, r3
30005e58:	2004      	movs	r0, #4
30005e5a:	f005 f825 	bl	3000aea8 <rtk_log_write>
30005e5e:	4642      	mov	r2, r8
30005e60:	4621      	mov	r1, r4
30005e62:	4628      	mov	r0, r5
30005e64:	f000 fb84 	bl	30006570 <BOOT_LoadBL32RDPImg>
30005e68:	2800      	cmp	r0, #0
30005e6a:	d153      	bne.n	30005f14 <BOOT_LoadAPSubImage+0x198>
30005e6c:	4b62      	ldr	r3, [pc, #392]	; (30005ff8 <BOOT_LoadAPSubImage+0x27c>)
30005e6e:	2004      	movs	r0, #4
30005e70:	4a66      	ldr	r2, [pc, #408]	; (3000600c <BOOT_LoadAPSubImage+0x290>)
30005e72:	4619      	mov	r1, r3
30005e74:	f005 f818 	bl	3000aea8 <rtk_log_write>
30005e78:	20ff      	movs	r0, #255	; 0xff
30005e7a:	b00e      	add	sp, #56	; 0x38
30005e7c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30005e80:	f014 031f 	ands.w	r3, r4, #31
30005e84:	bf19      	ittee	ne
30005e86:	f024 021f 	bicne.w	r2, r4, #31
30005e8a:	195b      	addne	r3, r3, r5
30005e8c:	462b      	moveq	r3, r5
30005e8e:	4622      	moveq	r2, r4
30005e90:	f3bf 8f4f 	dsb	sy
30005e94:	485a      	ldr	r0, [pc, #360]	; (30006000 <BOOT_LoadAPSubImage+0x284>)
30005e96:	441a      	add	r2, r3
30005e98:	2b00      	cmp	r3, #0
30005e9a:	eba2 0103 	sub.w	r1, r2, r3
30005e9e:	ddcc      	ble.n	30005e3a <BOOT_LoadAPSubImage+0xbe>
30005ea0:	3b20      	subs	r3, #32
30005ea2:	f8c0 1270 	str.w	r1, [r0, #624]	; 0x270
30005ea6:	e7f7      	b.n	30005e98 <BOOT_LoadAPSubImage+0x11c>
30005ea8:	4619      	mov	r1, r3
30005eaa:	4a59      	ldr	r2, [pc, #356]	; (30006010 <BOOT_LoadAPSubImage+0x294>)
30005eac:	2004      	movs	r0, #4
30005eae:	f004 fffb 	bl	3000aea8 <rtk_log_write>
30005eb2:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30005eb6:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005eba:	d32b      	bcc.n	30005f14 <BOOT_LoadAPSubImage+0x198>
30005ebc:	4642      	mov	r2, r8
30005ebe:	4629      	mov	r1, r5
30005ec0:	4620      	mov	r0, r4
30005ec2:	f7fe fb55 	bl	30004570 <BOOT_ImgCopy>
30005ec6:	4a4e      	ldr	r2, [pc, #312]	; (30006000 <BOOT_LoadAPSubImage+0x284>)
30005ec8:	6953      	ldr	r3, [r2, #20]
30005eca:	03d9      	lsls	r1, r3, #15
30005ecc:	d522      	bpl.n	30005f14 <BOOT_LoadAPSubImage+0x198>
30005ece:	ea08 0304 	and.w	r3, r8, r4
30005ed2:	3301      	adds	r3, #1
30005ed4:	d165      	bne.n	30005fa2 <BOOT_LoadAPSubImage+0x226>
30005ed6:	2300      	movs	r3, #0
30005ed8:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30005edc:	f3bf 8f4f 	dsb	sy
30005ee0:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30005ee4:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30005ee8:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30005eec:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005ef0:	015b      	lsls	r3, r3, #5
30005ef2:	ea03 0a0c 	and.w	sl, r3, ip
30005ef6:	4601      	mov	r1, r0
30005ef8:	ea4a 7e81 	orr.w	lr, sl, r1, lsl #30
30005efc:	3901      	subs	r1, #1
30005efe:	f8c2 e274 	str.w	lr, [r2, #628]	; 0x274
30005f02:	d2f9      	bcs.n	30005ef8 <BOOT_LoadAPSubImage+0x17c>
30005f04:	3b20      	subs	r3, #32
30005f06:	f113 0f20 	cmn.w	r3, #32
30005f0a:	d1f2      	bne.n	30005ef2 <BOOT_LoadAPSubImage+0x176>
30005f0c:	f3bf 8f4f 	dsb	sy
30005f10:	f3bf 8f6f 	isb	sy
30005f14:	4444      	add	r4, r8
30005f16:	eb08 0105 	add.w	r1, r8, r5
30005f1a:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30005f1e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30005f22:	d32c      	bcc.n	30005f7e <BOOT_LoadAPSubImage+0x202>
30005f24:	f8d9 5098 	ldr.w	r5, [r9, #152]	; 0x98
30005f28:	4620      	mov	r0, r4
30005f2a:	462a      	mov	r2, r5
30005f2c:	f7fe fb20 	bl	30004570 <BOOT_ImgCopy>
30005f30:	4a33      	ldr	r2, [pc, #204]	; (30006000 <BOOT_LoadAPSubImage+0x284>)
30005f32:	6953      	ldr	r3, [r2, #20]
30005f34:	03db      	lsls	r3, r3, #15
30005f36:	d522      	bpl.n	30005f7e <BOOT_LoadAPSubImage+0x202>
30005f38:	ea04 0305 	and.w	r3, r4, r5
30005f3c:	3301      	adds	r3, #1
30005f3e:	d144      	bne.n	30005fca <BOOT_LoadAPSubImage+0x24e>
30005f40:	2300      	movs	r3, #0
30005f42:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30005f46:	f3bf 8f4f 	dsb	sy
30005f4a:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30005f4e:	f643 74e0 	movw	r4, #16352	; 0x3fe0
30005f52:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30005f56:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005f5a:	015b      	lsls	r3, r3, #5
30005f5c:	ea03 0c04 	and.w	ip, r3, r4
30005f60:	4601      	mov	r1, r0
30005f62:	ea4c 7581 	orr.w	r5, ip, r1, lsl #30
30005f66:	3901      	subs	r1, #1
30005f68:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
30005f6c:	d2f9      	bcs.n	30005f62 <BOOT_LoadAPSubImage+0x1e6>
30005f6e:	3b20      	subs	r3, #32
30005f70:	f113 0f20 	cmn.w	r3, #32
30005f74:	d1f2      	bne.n	30005f5c <BOOT_LoadAPSubImage+0x1e0>
30005f76:	f3bf 8f4f 	dsb	sy
30005f7a:	f3bf 8f6f 	isb	sy
30005f7e:	b177      	cbz	r7, 30005f9e <BOOT_LoadAPSubImage+0x222>
30005f80:	9b09      	ldr	r3, [sp, #36]	; 0x24
30005f82:	2004      	movs	r0, #4
30005f84:	9a08      	ldr	r2, [sp, #32]
30005f86:	3b20      	subs	r3, #32
30005f88:	9600      	str	r6, [sp, #0]
30005f8a:	3220      	adds	r2, #32
30005f8c:	e9c7 3200 	strd	r3, r2, [r7]
30005f90:	e9cd 3201 	strd	r3, r2, [sp, #4]
30005f94:	4b18      	ldr	r3, [pc, #96]	; (30005ff8 <BOOT_LoadAPSubImage+0x27c>)
30005f96:	4a1f      	ldr	r2, [pc, #124]	; (30006014 <BOOT_LoadAPSubImage+0x298>)
30005f98:	4619      	mov	r1, r3
30005f9a:	f004 ff85 	bl	3000aea8 <rtk_log_write>
30005f9e:	2001      	movs	r0, #1
30005fa0:	e76b      	b.n	30005e7a <BOOT_LoadAPSubImage+0xfe>
30005fa2:	f014 031f 	ands.w	r3, r4, #31
30005fa6:	bf19      	ittee	ne
30005fa8:	f024 021f 	bicne.w	r2, r4, #31
30005fac:	4443      	addne	r3, r8
30005fae:	4643      	moveq	r3, r8
30005fb0:	4622      	moveq	r2, r4
30005fb2:	f3bf 8f4f 	dsb	sy
30005fb6:	4812      	ldr	r0, [pc, #72]	; (30006000 <BOOT_LoadAPSubImage+0x284>)
30005fb8:	441a      	add	r2, r3
30005fba:	2b00      	cmp	r3, #0
30005fbc:	eba2 0103 	sub.w	r1, r2, r3
30005fc0:	dda4      	ble.n	30005f0c <BOOT_LoadAPSubImage+0x190>
30005fc2:	3b20      	subs	r3, #32
30005fc4:	f8c0 1270 	str.w	r1, [r0, #624]	; 0x270
30005fc8:	e7f7      	b.n	30005fba <BOOT_LoadAPSubImage+0x23e>
30005fca:	f014 031f 	ands.w	r3, r4, #31
30005fce:	bf1c      	itt	ne
30005fd0:	f024 041f 	bicne.w	r4, r4, #31
30005fd4:	18ed      	addne	r5, r5, r3
30005fd6:	f3bf 8f4f 	dsb	sy
30005fda:	4a09      	ldr	r2, [pc, #36]	; (30006000 <BOOT_LoadAPSubImage+0x284>)
30005fdc:	442c      	add	r4, r5
30005fde:	2d00      	cmp	r5, #0
30005fe0:	eba4 0305 	sub.w	r3, r4, r5
30005fe4:	ddc7      	ble.n	30005f76 <BOOT_LoadAPSubImage+0x1fa>
30005fe6:	3d20      	subs	r5, #32
30005fe8:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30005fec:	e7f7      	b.n	30005fde <BOOT_LoadAPSubImage+0x262>
30005fee:	bf00      	nop
30005ff0:	3000d013 	.word	0x3000d013
30005ff4:	35393138 	.word	0x35393138
30005ff8:	3000c79c 	.word	0x3000c79c
30005ffc:	3000d01a 	.word	0x3000d01a
30006000:	e000ed00 	.word	0xe000ed00
30006004:	0000c109 	.word	0x0000c109
30006008:	3000d041 	.word	0x3000d041
3000600c:	3000d050 	.word	0x3000d050
30006010:	3000d070 	.word	0x3000d070
30006014:	3000d02d 	.word	0x3000d02d

30006018 <BOOT_RSIPIvSet>:
30006018:	2300      	movs	r3, #0
3000601a:	b537      	push	{r0, r1, r2, r4, r5, lr}
3000601c:	e9cd 3300 	strd	r3, r3, [sp]
30006020:	4b08      	ldr	r3, [pc, #32]	; (30006044 <BOOT_RSIPIvSet+0x2c>)
30006022:	4605      	mov	r5, r0
30006024:	460c      	mov	r4, r1
30006026:	4798      	blx	r3
30006028:	b148      	cbz	r0, 3000603e <BOOT_RSIPIvSet+0x26>
3000602a:	f105 0120 	add.w	r1, r5, #32
3000602e:	4b06      	ldr	r3, [pc, #24]	; (30006048 <BOOT_RSIPIvSet+0x30>)
30006030:	2208      	movs	r2, #8
30006032:	4668      	mov	r0, sp
30006034:	4798      	blx	r3
30006036:	4669      	mov	r1, sp
30006038:	b2e0      	uxtb	r0, r4
3000603a:	4b04      	ldr	r3, [pc, #16]	; (3000604c <BOOT_RSIPIvSet+0x34>)
3000603c:	4798      	blx	r3
3000603e:	b003      	add	sp, #12
30006040:	bd30      	pop	{r4, r5, pc}
30006042:	bf00      	nop
30006044:	0000c129 	.word	0x0000c129
30006048:	00012a1d 	.word	0x00012a1d
3000604c:	30008c85 	.word	0x30008c85

30006050 <BOOT_OTFCheck>:
30006050:	e92d 41f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, lr}
30006054:	461f      	mov	r7, r3
30006056:	4b1b      	ldr	r3, [pc, #108]	; (300060c4 <BOOT_OTFCheck+0x74>)
30006058:	4605      	mov	r5, r0
3000605a:	460e      	mov	r6, r1
3000605c:	4690      	mov	r8, r2
3000605e:	4798      	blx	r3
30006060:	b360      	cbz	r0, 300060bc <BOOT_OTFCheck+0x6c>
30006062:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30006066:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000606a:	d227      	bcs.n	300060bc <BOOT_OTFCheck+0x6c>
3000606c:	f106 4378 	add.w	r3, r6, #4160749568	; 0xf8000000
30006070:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006074:	d222      	bcs.n	300060bc <BOOT_OTFCheck+0x6c>
30006076:	4b14      	ldr	r3, [pc, #80]	; (300060c8 <BOOT_OTFCheck+0x78>)
30006078:	2004      	movs	r0, #4
3000607a:	4a14      	ldr	r2, [pc, #80]	; (300060cc <BOOT_OTFCheck+0x7c>)
3000607c:	4619      	mov	r1, r3
3000607e:	f004 ff13 	bl	3000aea8 <rtk_log_write>
30006082:	4b13      	ldr	r3, [pc, #76]	; (300060d0 <BOOT_OTFCheck+0x80>)
30006084:	4798      	blx	r3
30006086:	b150      	cbz	r0, 3000609e <BOOT_OTFCheck+0x4e>
30006088:	2801      	cmp	r0, #1
3000608a:	d009      	beq.n	300060a0 <BOOT_OTFCheck+0x50>
3000608c:	4b0e      	ldr	r3, [pc, #56]	; (300060c8 <BOOT_OTFCheck+0x78>)
3000608e:	2002      	movs	r0, #2
30006090:	4a10      	ldr	r2, [pc, #64]	; (300060d4 <BOOT_OTFCheck+0x84>)
30006092:	4619      	mov	r1, r3
30006094:	b002      	add	sp, #8
30006096:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000609a:	f004 bf05 	b.w	3000aea8 <rtk_log_write>
3000609e:	2002      	movs	r0, #2
300060a0:	2301      	movs	r3, #1
300060a2:	4632      	mov	r2, r6
300060a4:	4629      	mov	r1, r5
300060a6:	4c0c      	ldr	r4, [pc, #48]	; (300060d8 <BOOT_OTFCheck+0x88>)
300060a8:	e9cd 8000 	strd	r8, r0, [sp]
300060ac:	4638      	mov	r0, r7
300060ae:	47a0      	blx	r4
300060b0:	2001      	movs	r0, #1
300060b2:	4b0a      	ldr	r3, [pc, #40]	; (300060dc <BOOT_OTFCheck+0x8c>)
300060b4:	b002      	add	sp, #8
300060b6:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300060ba:	4718      	bx	r3
300060bc:	b002      	add	sp, #8
300060be:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300060c2:	bf00      	nop
300060c4:	0000c129 	.word	0x0000c129
300060c8:	3000c79c 	.word	0x3000c79c
300060cc:	3000d080 	.word	0x3000d080
300060d0:	0000c245 	.word	0x0000c245
300060d4:	3000d094 	.word	0x3000d094
300060d8:	30008cb9 	.word	0x30008cb9
300060dc:	30008ca1 	.word	0x30008ca1

300060e0 <BOOT_OTA_LoadIMG2>:
300060e0:	4b67      	ldr	r3, [pc, #412]	; (30006280 <BOOT_OTA_LoadIMG2+0x1a0>)
300060e2:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300060e6:	b08b      	sub	sp, #44	; 0x2c
300060e8:	681e      	ldr	r6, [r3, #0]
300060ea:	4b66      	ldr	r3, [pc, #408]	; (30006284 <BOOT_OTA_LoadIMG2+0x1a4>)
300060ec:	9003      	str	r0, [sp, #12]
300060ee:	ac04      	add	r4, sp, #16
300060f0:	f10d 0a1c 	add.w	sl, sp, #28
300060f4:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
300060f8:	330c      	adds	r3, #12
300060fa:	e884 0007 	stmia.w	r4, {r0, r1, r2}
300060fe:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30006102:	e88a 0007 	stmia.w	sl, {r0, r1, r2}
30006106:	f44f 72b2 	mov.w	r2, #356	; 0x164
3000610a:	4631      	mov	r1, r6
3000610c:	485e      	ldr	r0, [pc, #376]	; (30006288 <BOOT_OTA_LoadIMG2+0x1a8>)
3000610e:	f7fe fa2f 	bl	30004570 <BOOT_ImgCopy>
30006112:	4631      	mov	r1, r6
30006114:	485c      	ldr	r0, [pc, #368]	; (30006288 <BOOT_OTA_LoadIMG2+0x1a8>)
30006116:	f000 fcb5 	bl	30006a84 <BOOT_CertificateCheck>
3000611a:	4607      	mov	r7, r0
3000611c:	b118      	cbz	r0, 30006126 <BOOT_OTA_LoadIMG2+0x46>
3000611e:	485b      	ldr	r0, [pc, #364]	; (3000628c <BOOT_OTA_LoadIMG2+0x1ac>)
30006120:	f005 f906 	bl	3000b330 <__DiagPrintf_veneer>
30006124:	e030      	b.n	30006188 <BOOT_OTA_LoadIMG2+0xa8>
30006126:	f506 617f 	add.w	r1, r6, #4080	; 0xff0
3000612a:	f506 56ff 	add.w	r6, r6, #8160	; 0x1fe0
3000612e:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006132:	4d57      	ldr	r5, [pc, #348]	; (30006290 <BOOT_OTA_LoadIMG2+0x1b0>)
30006134:	4857      	ldr	r0, [pc, #348]	; (30006294 <BOOT_OTA_LoadIMG2+0x1b4>)
30006136:	3610      	adds	r6, #16
30006138:	f7fe fa1a 	bl	30004570 <BOOT_ImgCopy>
3000613c:	2101      	movs	r1, #1
3000613e:	4855      	ldr	r0, [pc, #340]	; (30006294 <BOOT_OTA_LoadIMG2+0x1b4>)
30006140:	f04f 0801 	mov.w	r8, #1
30006144:	f7ff ff68 	bl	30006018 <BOOT_RSIPIvSet>
30006148:	4c53      	ldr	r4, [pc, #332]	; (30006298 <BOOT_OTA_LoadIMG2+0x1b8>)
3000614a:	4a54      	ldr	r2, [pc, #336]	; (3000629c <BOOT_OTA_LoadIMG2+0x1bc>)
3000614c:	4633      	mov	r3, r6
3000614e:	4629      	mov	r1, r5
30006150:	4638      	mov	r0, r7
30006152:	47a0      	blx	r4
30006154:	2101      	movs	r1, #1
30006156:	4638      	mov	r0, r7
30006158:	4b51      	ldr	r3, [pc, #324]	; (300062a0 <BOOT_OTA_LoadIMG2+0x1c0>)
3000615a:	4798      	blx	r3
3000615c:	2301      	movs	r3, #1
3000615e:	494f      	ldr	r1, [pc, #316]	; (3000629c <BOOT_OTA_LoadIMG2+0x1bc>)
30006160:	4628      	mov	r0, r5
30006162:	461a      	mov	r2, r3
30006164:	f8df 9160 	ldr.w	r9, [pc, #352]	; 300062c8 <BOOT_OTA_LoadIMG2+0x1e8>
30006168:	f7ff ff72 	bl	30006050 <BOOT_OTFCheck>
3000616c:	47c8      	blx	r9
3000616e:	2800      	cmp	r0, #0
30006170:	bf0c      	ite	eq
30006172:	46b3      	moveq	fp, r6
30006174:	46ab      	movne	fp, r5
30006176:	ab04      	add	r3, sp, #16
30006178:	2203      	movs	r2, #3
3000617a:	4659      	mov	r1, fp
3000617c:	4849      	ldr	r0, [pc, #292]	; (300062a4 <BOOT_OTA_LoadIMG2+0x1c4>)
3000617e:	f8cd 8000 	str.w	r8, [sp]
30006182:	f7ff fd5f 	bl	30005c44 <BOOT_LoadSubImage>
30006186:	b920      	cbnz	r0, 30006192 <BOOT_OTA_LoadIMG2+0xb2>
30006188:	27ff      	movs	r7, #255	; 0xff
3000618a:	4638      	mov	r0, r7
3000618c:	b00b      	add	sp, #44	; 0x2c
3000618e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006192:	4c44      	ldr	r4, [pc, #272]	; (300062a4 <BOOT_OTA_LoadIMG2+0x1c4>)
30006194:	47c8      	blx	r9
30006196:	2800      	cmp	r0, #0
30006198:	bf08      	it	eq
3000619a:	4635      	moveq	r5, r6
3000619c:	4640      	mov	r0, r8
3000619e:	68e2      	ldr	r2, [r4, #12]
300061a0:	6863      	ldr	r3, [r4, #4]
300061a2:	6025      	str	r5, [r4, #0]
300061a4:	4413      	add	r3, r2
300061a6:	6962      	ldr	r2, [r4, #20]
300061a8:	4413      	add	r3, r2
300061aa:	4a3f      	ldr	r2, [pc, #252]	; (300062a8 <BOOT_OTA_LoadIMG2+0x1c8>)
300061ac:	1e5d      	subs	r5, r3, #1
300061ae:	eb0b 0103 	add.w	r1, fp, r3
300061b2:	f425 657f 	bic.w	r5, r5, #4080	; 0xff0
300061b6:	61a1      	str	r1, [r4, #24]
300061b8:	f025 050f 	bic.w	r5, r5, #15
300061bc:	f505 5580 	add.w	r5, r5, #4096	; 0x1000
300061c0:	1aeb      	subs	r3, r5, r3
300061c2:	61e3      	str	r3, [r4, #28]
300061c4:	19ab      	adds	r3, r5, r6
300061c6:	4e39      	ldr	r6, [pc, #228]	; (300062ac <BOOT_OTA_LoadIMG2+0x1cc>)
300061c8:	4d33      	ldr	r5, [pc, #204]	; (30006298 <BOOT_OTA_LoadIMG2+0x1b8>)
300061ca:	4631      	mov	r1, r6
300061cc:	9302      	str	r3, [sp, #8]
300061ce:	47a8      	blx	r5
300061d0:	4641      	mov	r1, r8
300061d2:	4640      	mov	r0, r8
300061d4:	4b32      	ldr	r3, [pc, #200]	; (300062a0 <BOOT_OTA_LoadIMG2+0x1c0>)
300061d6:	4798      	blx	r3
300061d8:	2302      	movs	r3, #2
300061da:	4642      	mov	r2, r8
300061dc:	4932      	ldr	r1, [pc, #200]	; (300062a8 <BOOT_OTA_LoadIMG2+0x1c8>)
300061de:	4630      	mov	r0, r6
300061e0:	f7ff ff36 	bl	30006050 <BOOT_OTFCheck>
300061e4:	47c8      	blx	r9
300061e6:	9b02      	ldr	r3, [sp, #8]
300061e8:	2203      	movs	r2, #3
300061ea:	f8cd 8000 	str.w	r8, [sp]
300061ee:	2800      	cmp	r0, #0
300061f0:	bf18      	it	ne
300061f2:	4633      	movne	r3, r6
300061f4:	f104 0020 	add.w	r0, r4, #32
300061f8:	469b      	mov	fp, r3
300061fa:	4653      	mov	r3, sl
300061fc:	4659      	mov	r1, fp
300061fe:	f7ff fd21 	bl	30005c44 <BOOT_LoadSubImage>
30006202:	2800      	cmp	r0, #0
30006204:	d0c0      	beq.n	30006188 <BOOT_OTA_LoadIMG2+0xa8>
30006206:	47c8      	blx	r9
30006208:	9b02      	ldr	r3, [sp, #8]
3000620a:	6ae2      	ldr	r2, [r4, #44]	; 0x2c
3000620c:	2800      	cmp	r0, #0
3000620e:	bf08      	it	eq
30006210:	461e      	moveq	r6, r3
30006212:	6a63      	ldr	r3, [r4, #36]	; 0x24
30006214:	481f      	ldr	r0, [pc, #124]	; (30006294 <BOOT_OTA_LoadIMG2+0x1b4>)
30006216:	4413      	add	r3, r2
30006218:	6b62      	ldr	r2, [r4, #52]	; 0x34
3000621a:	6226      	str	r6, [r4, #32]
3000621c:	4413      	add	r3, r2
3000621e:	1e5a      	subs	r2, r3, #1
30006220:	eb0b 0103 	add.w	r1, fp, r3
30006224:	f422 627f 	bic.w	r2, r2, #4080	; 0xff0
30006228:	63a1      	str	r1, [r4, #56]	; 0x38
3000622a:	f022 020f 	bic.w	r2, r2, #15
3000622e:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30006232:	1ad1      	subs	r1, r2, r3
30006234:	2900      	cmp	r1, #0
30006236:	63e1      	str	r1, [r4, #60]	; 0x3c
30006238:	4621      	mov	r1, r4
3000623a:	bf18      	it	ne
3000623c:	4613      	movne	r3, r2
3000623e:	9a02      	ldr	r2, [sp, #8]
30006240:	4413      	add	r3, r2
30006242:	4a1b      	ldr	r2, [pc, #108]	; (300062b0 <BOOT_OTA_LoadIMG2+0x1d0>)
30006244:	6013      	str	r3, [r2, #0]
30006246:	bf08      	it	eq
30006248:	2207      	moveq	r2, #7
3000624a:	4b0f      	ldr	r3, [pc, #60]	; (30006288 <BOOT_OTA_LoadIMG2+0x1a8>)
3000624c:	bf18      	it	ne
3000624e:	2208      	movne	r2, #8
30006250:	9700      	str	r7, [sp, #0]
30006252:	f000 fb47 	bl	300068e4 <BOOT_SignatureCheck>
30006256:	4b17      	ldr	r3, [pc, #92]	; (300062b4 <BOOT_OTA_LoadIMG2+0x1d4>)
30006258:	4604      	mov	r4, r0
3000625a:	689b      	ldr	r3, [r3, #8]
3000625c:	079b      	lsls	r3, r3, #30
3000625e:	d50a      	bpl.n	30006276 <BOOT_OTA_LoadIMG2+0x196>
30006260:	4915      	ldr	r1, [pc, #84]	; (300062b8 <BOOT_OTA_LoadIMG2+0x1d8>)
30006262:	4603      	mov	r3, r0
30006264:	4a15      	ldr	r2, [pc, #84]	; (300062bc <BOOT_OTA_LoadIMG2+0x1dc>)
30006266:	2800      	cmp	r0, #0
30006268:	bf08      	it	eq
3000626a:	460a      	moveq	r2, r1
3000626c:	9903      	ldr	r1, [sp, #12]
3000626e:	4814      	ldr	r0, [pc, #80]	; (300062c0 <BOOT_OTA_LoadIMG2+0x1e0>)
30006270:	3101      	adds	r1, #1
30006272:	f005 f85d 	bl	3000b330 <__DiagPrintf_veneer>
30006276:	2c00      	cmp	r4, #0
30006278:	d087      	beq.n	3000618a <BOOT_OTA_LoadIMG2+0xaa>
3000627a:	4812      	ldr	r0, [pc, #72]	; (300062c4 <BOOT_OTA_LoadIMG2+0x1e4>)
3000627c:	e750      	b.n	30006120 <BOOT_OTA_LoadIMG2+0x40>
3000627e:	bf00      	nop
30006280:	3000ebe8 	.word	0x3000ebe8
30006284:	3000d1a8 	.word	0x3000d1a8
30006288:	3000ec5c 	.word	0x3000ec5c
3000628c:	3000d0b4 	.word	0x3000d0b4
30006290:	0c000000 	.word	0x0c000000
30006294:	3000edc0 	.word	0x3000edc0
30006298:	30008d2d 	.word	0x30008d2d
3000629c:	0cffffe0 	.word	0x0cffffe0
300062a0:	30008d69 	.word	0x30008d69
300062a4:	3000ef68 	.word	0x3000ef68
300062a8:	0dffffe0 	.word	0x0dffffe0
300062ac:	0d000000 	.word	0x0d000000
300062b0:	3000dd00 	.word	0x3000dd00
300062b4:	2001c00c 	.word	0x2001c00c
300062b8:	3000d0ae 	.word	0x3000d0ae
300062bc:	3000d0ac 	.word	0x3000d0ac
300062c0:	3000d0dc 	.word	0x3000d0dc
300062c4:	3000d10e 	.word	0x3000d10e
300062c8:	30008f81 	.word	0x30008f81

300062cc <BOOT_OTA_Region_Init>:
300062cc:	b510      	push	{r4, lr}
300062ce:	4c10      	ldr	r4, [pc, #64]	; (30006310 <BOOT_OTA_Region_Init+0x44>)
300062d0:	2200      	movs	r2, #0
300062d2:	2002      	movs	r0, #2
300062d4:	4621      	mov	r1, r4
300062d6:	f001 fbb1 	bl	30007a3c <flash_get_layout_info>
300062da:	2200      	movs	r2, #0
300062dc:	1d21      	adds	r1, r4, #4
300062de:	2003      	movs	r0, #3
300062e0:	f001 fbac 	bl	30007a3c <flash_get_layout_info>
300062e4:	2200      	movs	r2, #0
300062e6:	f104 0110 	add.w	r1, r4, #16
300062ea:	2004      	movs	r0, #4
300062ec:	f001 fba6 	bl	30007a3c <flash_get_layout_info>
300062f0:	2200      	movs	r2, #0
300062f2:	f104 0114 	add.w	r1, r4, #20
300062f6:	2005      	movs	r0, #5
300062f8:	f001 fba0 	bl	30007a3c <flash_get_layout_info>
300062fc:	6823      	ldr	r3, [r4, #0]
300062fe:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006302:	60a3      	str	r3, [r4, #8]
30006304:	6863      	ldr	r3, [r4, #4]
30006306:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
3000630a:	60e3      	str	r3, [r4, #12]
3000630c:	bd10      	pop	{r4, pc}
3000630e:	bf00      	nop
30006310:	3000ef50 	.word	0x3000ef50

30006314 <BOOT_OTA_IMG2>:
30006314:	f7ff bee4 	b.w	300060e0 <BOOT_OTA_LoadIMG2>

30006318 <BOOT_OTA_AP>:
30006318:	b5f0      	push	{r4, r5, r6, r7, lr}
3000631a:	4b2c      	ldr	r3, [pc, #176]	; (300063cc <BOOT_OTA_AP+0xb4>)
3000631c:	b0eb      	sub	sp, #428	; 0x1ac
3000631e:	4f2c      	ldr	r7, [pc, #176]	; (300063d0 <BOOT_OTA_AP+0xb8>)
30006320:	ac02      	add	r4, sp, #8
30006322:	4d2c      	ldr	r5, [pc, #176]	; (300063d4 <BOOT_OTA_AP+0xbc>)
30006324:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006326:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
3000632a:	4b2b      	ldr	r3, [pc, #172]	; (300063d8 <BOOT_OTA_AP+0xc0>)
3000632c:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006330:	a806      	add	r0, sp, #24
30006332:	681e      	ldr	r6, [r3, #0]
30006334:	4631      	mov	r1, r6
30006336:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
3000633a:	f7fe f919 	bl	30004570 <BOOT_ImgCopy>
3000633e:	4a27      	ldr	r2, [pc, #156]	; (300063dc <BOOT_OTA_AP+0xc4>)
30006340:	4639      	mov	r1, r7
30006342:	4633      	mov	r3, r6
30006344:	2003      	movs	r0, #3
30006346:	47a8      	blx	r5
30006348:	2101      	movs	r1, #1
3000634a:	2003      	movs	r0, #3
3000634c:	4b24      	ldr	r3, [pc, #144]	; (300063e0 <BOOT_OTA_AP+0xc8>)
3000634e:	4798      	blx	r3
30006350:	2201      	movs	r2, #1
30006352:	4922      	ldr	r1, [pc, #136]	; (300063dc <BOOT_OTA_AP+0xc4>)
30006354:	4638      	mov	r0, r7
30006356:	2303      	movs	r3, #3
30006358:	4d22      	ldr	r5, [pc, #136]	; (300063e4 <BOOT_OTA_AP+0xcc>)
3000635a:	f7ff fe79 	bl	30006050 <BOOT_OTFCheck>
3000635e:	4b22      	ldr	r3, [pc, #136]	; (300063e8 <BOOT_OTA_AP+0xd0>)
30006360:	4798      	blx	r3
30006362:	2301      	movs	r3, #1
30006364:	2800      	cmp	r0, #0
30006366:	bf0c      	ite	eq
30006368:	4631      	moveq	r1, r6
3000636a:	4639      	movne	r1, r7
3000636c:	2204      	movs	r2, #4
3000636e:	9300      	str	r3, [sp, #0]
30006370:	4628      	mov	r0, r5
30006372:	4623      	mov	r3, r4
30006374:	f7ff fc66 	bl	30005c44 <BOOT_LoadSubImage>
30006378:	2801      	cmp	r0, #1
3000637a:	4607      	mov	r7, r0
3000637c:	d123      	bne.n	300063c6 <BOOT_OTA_AP+0xae>
3000637e:	2302      	movs	r3, #2
30006380:	2204      	movs	r2, #4
30006382:	4629      	mov	r1, r5
30006384:	a806      	add	r0, sp, #24
30006386:	9300      	str	r3, [sp, #0]
30006388:	4b18      	ldr	r3, [pc, #96]	; (300063ec <BOOT_OTA_AP+0xd4>)
3000638a:	f000 faab 	bl	300068e4 <BOOT_SignatureCheck>
3000638e:	4604      	mov	r4, r0
30006390:	b990      	cbnz	r0, 300063b8 <BOOT_OTA_AP+0xa0>
30006392:	68eb      	ldr	r3, [r5, #12]
30006394:	463a      	mov	r2, r7
30006396:	6869      	ldr	r1, [r5, #4]
30006398:	f105 0018 	add.w	r0, r5, #24
3000639c:	4419      	add	r1, r3
3000639e:	696b      	ldr	r3, [r5, #20]
300063a0:	4419      	add	r1, r3
300063a2:	4431      	add	r1, r6
300063a4:	f7ff fcea 	bl	30005d7c <BOOT_LoadAPSubImage>
300063a8:	2801      	cmp	r0, #1
300063aa:	d009      	beq.n	300063c0 <BOOT_OTA_AP+0xa8>
300063ac:	4b10      	ldr	r3, [pc, #64]	; (300063f0 <BOOT_OTA_AP+0xd8>)
300063ae:	681b      	ldr	r3, [r3, #0]
300063b0:	079b      	lsls	r3, r3, #30
300063b2:	d504      	bpl.n	300063be <BOOT_OTA_AP+0xa6>
300063b4:	480f      	ldr	r0, [pc, #60]	; (300063f4 <BOOT_OTA_AP+0xdc>)
300063b6:	e000      	b.n	300063ba <BOOT_OTA_AP+0xa2>
300063b8:	480f      	ldr	r0, [pc, #60]	; (300063f8 <BOOT_OTA_AP+0xe0>)
300063ba:	f004 ffb9 	bl	3000b330 <__DiagPrintf_veneer>
300063be:	24ff      	movs	r4, #255	; 0xff
300063c0:	4620      	mov	r0, r4
300063c2:	b06b      	add	sp, #428	; 0x1ac
300063c4:	bdf0      	pop	{r4, r5, r6, r7, pc}
300063c6:	480d      	ldr	r0, [pc, #52]	; (300063fc <BOOT_OTA_AP+0xe4>)
300063c8:	e7f7      	b.n	300063ba <BOOT_OTA_AP+0xa2>
300063ca:	bf00      	nop
300063cc:	3000d1c0 	.word	0x3000d1c0
300063d0:	0e000000 	.word	0x0e000000
300063d4:	30008d2d 	.word	0x30008d2d
300063d8:	3000dd00 	.word	0x3000dd00
300063dc:	0fffffe0 	.word	0x0fffffe0
300063e0:	30008d69 	.word	0x30008d69
300063e4:	3000ef68 	.word	0x3000ef68
300063e8:	30008f81 	.word	0x30008f81
300063ec:	3000ec5c 	.word	0x3000ec5c
300063f0:	2001c00c 	.word	0x2001c00c
300063f4:	3000d175 	.word	0x3000d175
300063f8:	3000d12f 	.word	0x3000d12f
300063fc:	3000d152 	.word	0x3000d152

30006400 <BOOT_LoadRDPImg>:
30006400:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006404:	4b4f      	ldr	r3, [pc, #316]	; (30006544 <BOOT_LoadRDPImg+0x144>)
30006406:	4615      	mov	r5, r2
30006408:	4606      	mov	r6, r0
3000640a:	468a      	mov	sl, r1
3000640c:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006410:	2400      	movs	r4, #0
30006412:	ed2d 8b02 	vpush	{d8}
30006416:	b0d7      	sub	sp, #348	; 0x15c
30006418:	e893 0003 	ldmia.w	r3, {r0, r1}
3000641c:	f10d 0910 	add.w	r9, sp, #16
30006420:	e889 0003 	stmia.w	r9, {r0, r1}
30006424:	4629      	mov	r1, r5
30006426:	4630      	mov	r0, r6
30006428:	f7fe f8a2 	bl	30004570 <BOOT_ImgCopy>
3000642c:	f505 5380 	add.w	r3, r5, #4096	; 0x1000
30006430:	2202      	movs	r2, #2
30006432:	4650      	mov	r0, sl
30006434:	ee08 3a10 	vmov	s16, r3
30006438:	9400      	str	r4, [sp, #0]
3000643a:	464b      	mov	r3, r9
3000643c:	ee18 1a10 	vmov	r1, s16
30006440:	f7ff fc00 	bl	30005c44 <BOOT_LoadSubImage>
30006444:	2801      	cmp	r0, #1
30006446:	d057      	beq.n	300064f8 <BOOT_LoadRDPImg+0xf8>
30006448:	4b3f      	ldr	r3, [pc, #252]	; (30006548 <BOOT_LoadRDPImg+0x148>)
3000644a:	4620      	mov	r0, r4
3000644c:	4798      	blx	r3
3000644e:	2101      	movs	r1, #1
30006450:	4b3e      	ldr	r3, [pc, #248]	; (3000654c <BOOT_LoadRDPImg+0x14c>)
30006452:	4620      	mov	r0, r4
30006454:	4798      	blx	r3
30006456:	f8df b114 	ldr.w	fp, [pc, #276]	; 3000656c <BOOT_LoadRDPImg+0x16c>
3000645a:	2120      	movs	r1, #32
3000645c:	4b3c      	ldr	r3, [pc, #240]	; (30006550 <BOOT_LoadRDPImg+0x150>)
3000645e:	a80e      	add	r0, sp, #56	; 0x38
30006460:	4798      	blx	r3
30006462:	2210      	movs	r2, #16
30006464:	f106 0120 	add.w	r1, r6, #32
30006468:	a80a      	add	r0, sp, #40	; 0x28
3000646a:	4b3a      	ldr	r3, [pc, #232]	; (30006554 <BOOT_LoadRDPImg+0x154>)
3000646c:	4798      	blx	r3
3000646e:	2210      	movs	r2, #16
30006470:	ee18 1a10 	vmov	r1, s16
30006474:	a816      	add	r0, sp, #88	; 0x58
30006476:	ae06      	add	r6, sp, #24
30006478:	f7fe f87a 	bl	30004570 <BOOT_ImgCopy>
3000647c:	2310      	movs	r3, #16
3000647e:	aa0a      	add	r2, sp, #40	; 0x28
30006480:	a816      	add	r0, sp, #88	; 0x58
30006482:	4619      	mov	r1, r3
30006484:	4f34      	ldr	r7, [pc, #208]	; (30006558 <BOOT_LoadRDPImg+0x158>)
30006486:	9600      	str	r6, [sp, #0]
30006488:	47b8      	blx	r7
3000648a:	6833      	ldr	r3, [r6, #0]
3000648c:	455b      	cmp	r3, fp
3000648e:	d105      	bne.n	3000649c <BOOT_LoadRDPImg+0x9c>
30006490:	6872      	ldr	r2, [r6, #4]
30006492:	4932      	ldr	r1, [pc, #200]	; (3000655c <BOOT_LoadRDPImg+0x15c>)
30006494:	f859 3024 	ldr.w	r3, [r9, r4, lsl #2]
30006498:	428a      	cmp	r2, r1
3000649a:	d010      	beq.n	300064be <BOOT_LoadRDPImg+0xbe>
3000649c:	f859 3024 	ldr.w	r3, [r9, r4, lsl #2]
300064a0:	2004      	movs	r0, #4
300064a2:	4a2f      	ldr	r2, [pc, #188]	; (30006560 <BOOT_LoadRDPImg+0x160>)
300064a4:	9300      	str	r3, [sp, #0]
300064a6:	4b2f      	ldr	r3, [pc, #188]	; (30006564 <BOOT_LoadRDPImg+0x164>)
300064a8:	4619      	mov	r1, r3
300064aa:	f004 fcfd 	bl	3000aea8 <rtk_log_write>
300064ae:	2000      	movs	r0, #0
300064b0:	b057      	add	sp, #348	; 0x15c
300064b2:	ecbd 8b02 	vpop	{d8}
300064b6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300064ba:	2401      	movs	r4, #1
300064bc:	e7d7      	b.n	3000646e <BOOT_LoadRDPImg+0x6e>
300064be:	68f7      	ldr	r7, [r6, #12]
300064c0:	2004      	movs	r0, #4
300064c2:	68b6      	ldr	r6, [r6, #8]
300064c4:	3f20      	subs	r7, #32
300064c6:	9300      	str	r3, [sp, #0]
300064c8:	3620      	adds	r6, #32
300064ca:	4b26      	ldr	r3, [pc, #152]	; (30006564 <BOOT_LoadRDPImg+0x164>)
300064cc:	4a26      	ldr	r2, [pc, #152]	; (30006568 <BOOT_LoadRDPImg+0x168>)
300064ce:	4619      	mov	r1, r3
300064d0:	e9cd 7601 	strd	r7, r6, [sp, #4]
300064d4:	f004 fce8 	bl	3000aea8 <rtk_log_write>
300064d8:	f1ba 0f00 	cmp.w	sl, #0
300064dc:	d005      	beq.n	300064ea <BOOT_LoadRDPImg+0xea>
300064de:	f10a 0304 	add.w	r3, sl, #4
300064e2:	f84a 7034 	str.w	r7, [sl, r4, lsl #3]
300064e6:	f843 6034 	str.w	r6, [r3, r4, lsl #3]
300064ea:	b93e      	cbnz	r6, 300064fc <BOOT_LoadRDPImg+0xfc>
300064ec:	2c00      	cmp	r4, #0
300064ee:	d0e4      	beq.n	300064ba <BOOT_LoadRDPImg+0xba>
300064f0:	4631      	mov	r1, r6
300064f2:	4630      	mov	r0, r6
300064f4:	4b15      	ldr	r3, [pc, #84]	; (3000654c <BOOT_LoadRDPImg+0x14c>)
300064f6:	4798      	blx	r3
300064f8:	2002      	movs	r0, #2
300064fa:	e7d9      	b.n	300064b0 <BOOT_LoadRDPImg+0xb0>
300064fc:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30006500:	46b0      	mov	r8, r6
30006502:	ee18 1a10 	vmov	r1, s16
30006506:	a816      	add	r0, sp, #88	; 0x58
30006508:	bf28      	it	cs
3000650a:	f44f 7880 	movcs.w	r8, #256	; 0x100
3000650e:	4d12      	ldr	r5, [pc, #72]	; (30006558 <BOOT_LoadRDPImg+0x158>)
30006510:	4642      	mov	r2, r8
30006512:	eba6 0608 	sub.w	r6, r6, r8
30006516:	f7fe f82b 	bl	30004570 <BOOT_ImgCopy>
3000651a:	2310      	movs	r3, #16
3000651c:	aa0a      	add	r2, sp, #40	; 0x28
3000651e:	4641      	mov	r1, r8
30006520:	9700      	str	r7, [sp, #0]
30006522:	a816      	add	r0, sp, #88	; 0x58
30006524:	47a8      	blx	r5
30006526:	ab16      	add	r3, sp, #88	; 0x58
30006528:	f1a8 0110 	sub.w	r1, r8, #16
3000652c:	2210      	movs	r2, #16
3000652e:	a80a      	add	r0, sp, #40	; 0x28
30006530:	4447      	add	r7, r8
30006532:	4419      	add	r1, r3
30006534:	4b07      	ldr	r3, [pc, #28]	; (30006554 <BOOT_LoadRDPImg+0x154>)
30006536:	4798      	blx	r3
30006538:	ee18 3a10 	vmov	r3, s16
3000653c:	4443      	add	r3, r8
3000653e:	ee08 3a10 	vmov	s16, r3
30006542:	e7d2      	b.n	300064ea <BOOT_LoadRDPImg+0xea>
30006544:	3000d2b0 	.word	0x3000d2b0
30006548:	00002429 	.word	0x00002429
3000654c:	00002479 	.word	0x00002479
30006550:	00004641 	.word	0x00004641
30006554:	00012a1d 	.word	0x00012a1d
30006558:	000046f5 	.word	0x000046f5
3000655c:	31313738 	.word	0x31313738
30006560:	3000d01a 	.word	0x3000d01a
30006564:	3000c79c 	.word	0x3000c79c
30006568:	3000d02d 	.word	0x3000d02d
3000656c:	35393138 	.word	0x35393138

30006570 <BOOT_LoadBL32RDPImg>:
30006570:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30006574:	4b34      	ldr	r3, [pc, #208]	; (30006648 <BOOT_LoadBL32RDPImg+0xd8>)
30006576:	b0d0      	sub	sp, #320	; 0x140
30006578:	4607      	mov	r7, r0
3000657a:	460d      	mov	r5, r1
3000657c:	4614      	mov	r4, r2
3000657e:	ae04      	add	r6, sp, #16
30006580:	f103 0c10 	add.w	ip, r3, #16
30006584:	6818      	ldr	r0, [r3, #0]
30006586:	3308      	adds	r3, #8
30006588:	f853 1c04 	ldr.w	r1, [r3, #-4]
3000658c:	4632      	mov	r2, r6
3000658e:	4563      	cmp	r3, ip
30006590:	c203      	stmia	r2!, {r0, r1}
30006592:	4616      	mov	r6, r2
30006594:	d1f6      	bne.n	30006584 <BOOT_LoadBL32RDPImg+0x14>
30006596:	4a2d      	ldr	r2, [pc, #180]	; (3000664c <BOOT_LoadBL32RDPImg+0xdc>)
30006598:	ab02      	add	r3, sp, #8
3000659a:	4e2d      	ldr	r6, [pc, #180]	; (30006650 <BOOT_LoadBL32RDPImg+0xe0>)
3000659c:	6810      	ldr	r0, [r2, #0]
3000659e:	6851      	ldr	r1, [r2, #4]
300065a0:	46b0      	mov	r8, r6
300065a2:	c303      	stmia	r3!, {r0, r1}
300065a4:	4b2b      	ldr	r3, [pc, #172]	; (30006654 <BOOT_LoadBL32RDPImg+0xe4>)
300065a6:	2000      	movs	r0, #0
300065a8:	4798      	blx	r3
300065aa:	2101      	movs	r1, #1
300065ac:	2000      	movs	r0, #0
300065ae:	47b0      	blx	r6
300065b0:	2120      	movs	r1, #32
300065b2:	4b29      	ldr	r3, [pc, #164]	; (30006658 <BOOT_LoadBL32RDPImg+0xe8>)
300065b4:	eb0d 0001 	add.w	r0, sp, r1
300065b8:	4798      	blx	r3
300065ba:	4b28      	ldr	r3, [pc, #160]	; (3000665c <BOOT_LoadBL32RDPImg+0xec>)
300065bc:	689b      	ldr	r3, [r3, #8]
300065be:	f013 0f02 	tst.w	r3, #2
300065c2:	d005      	beq.n	300065d0 <BOOT_LoadBL32RDPImg+0x60>
300065c4:	4623      	mov	r3, r4
300065c6:	462a      	mov	r2, r5
300065c8:	a902      	add	r1, sp, #8
300065ca:	4825      	ldr	r0, [pc, #148]	; (30006660 <BOOT_LoadBL32RDPImg+0xf0>)
300065cc:	f004 feb0 	bl	3000b330 <__DiagPrintf_veneer>
300065d0:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 30006674 <BOOT_LoadBL32RDPImg+0x104>
300065d4:	f8df a0a0 	ldr.w	sl, [pc, #160]	; 30006678 <BOOT_LoadBL32RDPImg+0x108>
300065d8:	b984      	cbnz	r4, 300065fc <BOOT_LoadBL32RDPImg+0x8c>
300065da:	f855 1c20 	ldr.w	r1, [r5, #-32]
300065de:	4a21      	ldr	r2, [pc, #132]	; (30006664 <BOOT_LoadBL32RDPImg+0xf4>)
300065e0:	4291      	cmp	r1, r2
300065e2:	d104      	bne.n	300065ee <BOOT_LoadBL32RDPImg+0x7e>
300065e4:	f855 2c1c 	ldr.w	r2, [r5, #-28]
300065e8:	4b1f      	ldr	r3, [pc, #124]	; (30006668 <BOOT_LoadBL32RDPImg+0xf8>)
300065ea:	429a      	cmp	r2, r3
300065ec:	d024      	beq.n	30006638 <BOOT_LoadBL32RDPImg+0xc8>
300065ee:	481f      	ldr	r0, [pc, #124]	; (3000666c <BOOT_LoadBL32RDPImg+0xfc>)
300065f0:	f004 fe9e 	bl	3000b330 <__DiagPrintf_veneer>
300065f4:	2000      	movs	r0, #0
300065f6:	b050      	add	sp, #320	; 0x140
300065f8:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300065fc:	f5b4 7f80 	cmp.w	r4, #256	; 0x100
30006600:	4626      	mov	r6, r4
30006602:	4639      	mov	r1, r7
30006604:	a810      	add	r0, sp, #64	; 0x40
30006606:	bf28      	it	cs
30006608:	f44f 7680 	movcs.w	r6, #256	; 0x100
3000660c:	4632      	mov	r2, r6
3000660e:	1ba4      	subs	r4, r4, r6
30006610:	f7fd ffae 	bl	30004570 <BOOT_ImgCopy>
30006614:	2310      	movs	r3, #16
30006616:	4631      	mov	r1, r6
30006618:	9500      	str	r5, [sp, #0]
3000661a:	eb0d 0203 	add.w	r2, sp, r3
3000661e:	a810      	add	r0, sp, #64	; 0x40
30006620:	47c8      	blx	r9
30006622:	2210      	movs	r2, #16
30006624:	ab10      	add	r3, sp, #64	; 0x40
30006626:	f1a6 0110 	sub.w	r1, r6, #16
3000662a:	eb0d 0002 	add.w	r0, sp, r2
3000662e:	4435      	add	r5, r6
30006630:	4419      	add	r1, r3
30006632:	4437      	add	r7, r6
30006634:	47d0      	blx	sl
30006636:	e7cf      	b.n	300065d8 <BOOT_LoadBL32RDPImg+0x68>
30006638:	480d      	ldr	r0, [pc, #52]	; (30006670 <BOOT_LoadBL32RDPImg+0x100>)
3000663a:	f004 fe79 	bl	3000b330 <__DiagPrintf_veneer>
3000663e:	4621      	mov	r1, r4
30006640:	4620      	mov	r0, r4
30006642:	47c0      	blx	r8
30006644:	2001      	movs	r0, #1
30006646:	e7d6      	b.n	300065f6 <BOOT_LoadBL32RDPImg+0x86>
30006648:	3000d233 	.word	0x3000d233
3000664c:	3000d244 	.word	0x3000d244
30006650:	00002479 	.word	0x00002479
30006654:	00002429 	.word	0x00002429
30006658:	00004641 	.word	0x00004641
3000665c:	2001c00c 	.word	0x2001c00c
30006660:	3000d1f6 	.word	0x3000d1f6
30006664:	35393138 	.word	0x35393138
30006668:	31313738 	.word	0x31313738
3000666c:	3000d21c 	.word	0x3000d21c
30006670:	3000d226 	.word	0x3000d226
30006674:	000046f5 	.word	0x000046f5
30006678:	00012a1d 	.word	0x00012a1d

3000667c <_BOOT_Validate_ImgHash>:
3000667c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006680:	f5ad 7d4d 	sub.w	sp, sp, #820	; 0x334
30006684:	4605      	mov	r5, r0
30006686:	4614      	mov	r4, r2
30006688:	22e0      	movs	r2, #224	; 0xe0
3000668a:	9101      	str	r1, [sp, #4]
3000668c:	2100      	movs	r1, #0
3000668e:	a814      	add	r0, sp, #80	; 0x50
30006690:	469b      	mov	fp, r3
30006692:	f88d 100f 	strb.w	r1, [sp, #15]
30006696:	f004 fe6b 	bl	3000b370 <____wrap_memset_veneer>
3000669a:	4628      	mov	r0, r5
3000669c:	f10d 020f 	add.w	r2, sp, #15
300066a0:	f10d 010e 	add.w	r1, sp, #14
300066a4:	f002 fca2 	bl	30008fec <SBOOT_GetMdType>
300066a8:	4605      	mov	r5, r0
300066aa:	b988      	cbnz	r0, 300066d0 <_BOOT_Validate_ImgHash+0x54>
300066ac:	f89d 300f 	ldrb.w	r3, [sp, #15]
300066b0:	2b00      	cmp	r3, #0
300066b2:	d07f      	beq.n	300067b4 <_BOOT_Validate_ImgHash+0x138>
300066b4:	2101      	movs	r1, #1
300066b6:	4b75      	ldr	r3, [pc, #468]	; (3000688c <_BOOT_Validate_ImgHash+0x210>)
300066b8:	4e75      	ldr	r6, [pc, #468]	; (30006890 <_BOOT_Validate_ImgHash+0x214>)
300066ba:	4608      	mov	r0, r1
300066bc:	4798      	blx	r3
300066be:	ab14      	add	r3, sp, #80	; 0x50
300066c0:	2220      	movs	r2, #32
300066c2:	4629      	mov	r1, r5
300066c4:	f89d 000e 	ldrb.w	r0, [sp, #14]
300066c8:	47b0      	blx	r6
300066ca:	b130      	cbz	r0, 300066da <_BOOT_Validate_ImgHash+0x5e>
300066cc:	f06f 050a 	mvn.w	r5, #10
300066d0:	4628      	mov	r0, r5
300066d2:	f50d 7d4d 	add.w	sp, sp, #820	; 0x334
300066d6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300066da:	ae4c      	add	r6, sp, #304	; 0x130
300066dc:	aacc      	add	r2, sp, #816	; 0x330
300066de:	eb04 0bcb 	add.w	fp, r4, fp, lsl #3
300066e2:	f8df 91c8 	ldr.w	r9, [pc, #456]	; 300068ac <_BOOT_Validate_ImgHash+0x230>
300066e6:	f012 0218 	ands.w	r2, r2, #24
300066ea:	f026 031f 	bic.w	r3, r6, #31
300066ee:	bf18      	it	ne
300066f0:	461e      	movne	r6, r3
300066f2:	9200      	str	r2, [sp, #0]
300066f4:	455c      	cmp	r4, fp
300066f6:	d11e      	bne.n	30006736 <_BOOT_Validate_ImgHash+0xba>
300066f8:	a914      	add	r1, sp, #80	; 0x50
300066fa:	a804      	add	r0, sp, #16
300066fc:	4b65      	ldr	r3, [pc, #404]	; (30006894 <_BOOT_Validate_ImgHash+0x218>)
300066fe:	4798      	blx	r3
30006700:	4601      	mov	r1, r0
30006702:	2800      	cmp	r0, #0
30006704:	d1e2      	bne.n	300066cc <_BOOT_Validate_ImgHash+0x50>
30006706:	2001      	movs	r0, #1
30006708:	4b60      	ldr	r3, [pc, #384]	; (3000688c <_BOOT_Validate_ImgHash+0x210>)
3000670a:	4798      	blx	r3
3000670c:	f89d 400e 	ldrb.w	r4, [sp, #14]
30006710:	ae04      	add	r6, sp, #16
30006712:	4b61      	ldr	r3, [pc, #388]	; (30006898 <_BOOT_Validate_ImgHash+0x21c>)
30006714:	4798      	blx	r3
30006716:	fbb0 f3f4 	udiv	r3, r0, r4
3000671a:	fb03 0014 	mls	r0, r3, r4, r0
3000671e:	2300      	movs	r3, #0
30006720:	b2c0      	uxtb	r0, r0
30006722:	461a      	mov	r2, r3
30006724:	b2d9      	uxtb	r1, r3
30006726:	428c      	cmp	r4, r1
30006728:	f200 80a1 	bhi.w	3000686e <_BOOT_Validate_ImgHash+0x1f2>
3000672c:	2a00      	cmp	r2, #0
3000672e:	bf18      	it	ne
30006730:	f06f 0506 	mvnne.w	r5, #6
30006734:	e7cc      	b.n	300066d0 <_BOOT_Validate_ImgHash+0x54>
30006736:	e9d4 7800 	ldrd	r7, r8, [r4]
3000673a:	f107 4378 	add.w	r3, r7, #4160749568	; 0xf8000000
3000673e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006742:	d22f      	bcs.n	300067a4 <_BOOT_Validate_ImgHash+0x128>
30006744:	f1b8 0f00 	cmp.w	r8, #0
30006748:	d101      	bne.n	3000674e <_BOOT_Validate_ImgHash+0xd2>
3000674a:	3408      	adds	r4, #8
3000674c:	e7d2      	b.n	300066f4 <_BOOT_Validate_ImgHash+0x78>
3000674e:	f5b8 7f00 	cmp.w	r8, #512	; 0x200
30006752:	46c2      	mov	sl, r8
30006754:	4639      	mov	r1, r7
30006756:	a84c      	add	r0, sp, #304	; 0x130
30006758:	bf28      	it	cs
3000675a:	f44f 7a00 	movcs.w	sl, #512	; 0x200
3000675e:	4652      	mov	r2, sl
30006760:	f7fd ff06 	bl	30004570 <BOOT_ImgCopy>
30006764:	f8d9 2014 	ldr.w	r2, [r9, #20]
30006768:	03d2      	lsls	r2, r2, #15
3000676a:	d510      	bpl.n	3000678e <_BOOT_Validate_ImgHash+0x112>
3000676c:	9b00      	ldr	r3, [sp, #0]
3000676e:	eb0a 0203 	add.w	r2, sl, r3
30006772:	f3bf 8f4f 	dsb	sy
30006776:	4631      	mov	r1, r6
30006778:	4432      	add	r2, r6
3000677a:	f8c9 1270 	str.w	r1, [r9, #624]	; 0x270
3000677e:	3120      	adds	r1, #32
30006780:	1a50      	subs	r0, r2, r1
30006782:	2800      	cmp	r0, #0
30006784:	dcf9      	bgt.n	3000677a <_BOOT_Validate_ImgHash+0xfe>
30006786:	f3bf 8f4f 	dsb	sy
3000678a:	f3bf 8f6f 	isb	sy
3000678e:	aa14      	add	r2, sp, #80	; 0x50
30006790:	4651      	mov	r1, sl
30006792:	a84c      	add	r0, sp, #304	; 0x130
30006794:	4b41      	ldr	r3, [pc, #260]	; (3000689c <_BOOT_Validate_ImgHash+0x220>)
30006796:	4798      	blx	r3
30006798:	2800      	cmp	r0, #0
3000679a:	d197      	bne.n	300066cc <_BOOT_Validate_ImgHash+0x50>
3000679c:	eba8 080a 	sub.w	r8, r8, sl
300067a0:	4457      	add	r7, sl
300067a2:	e7cf      	b.n	30006744 <_BOOT_Validate_ImgHash+0xc8>
300067a4:	aa14      	add	r2, sp, #80	; 0x50
300067a6:	4641      	mov	r1, r8
300067a8:	4638      	mov	r0, r7
300067aa:	4b3c      	ldr	r3, [pc, #240]	; (3000689c <_BOOT_Validate_ImgHash+0x220>)
300067ac:	4798      	blx	r3
300067ae:	2800      	cmp	r0, #0
300067b0:	d0cb      	beq.n	3000674a <_BOOT_Validate_ImgHash+0xce>
300067b2:	e78b      	b.n	300066cc <_BOOT_Validate_ImgHash+0x50>
300067b4:	a914      	add	r1, sp, #80	; 0x50
300067b6:	f89d 000e 	ldrb.w	r0, [sp, #14]
300067ba:	4b39      	ldr	r3, [pc, #228]	; (300068a0 <_BOOT_Validate_ImgHash+0x224>)
300067bc:	4798      	blx	r3
300067be:	2800      	cmp	r0, #0
300067c0:	d184      	bne.n	300066cc <_BOOT_Validate_ImgHash+0x50>
300067c2:	ae4c      	add	r6, sp, #304	; 0x130
300067c4:	aacc      	add	r2, sp, #816	; 0x330
300067c6:	eb04 0bcb 	add.w	fp, r4, fp, lsl #3
300067ca:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 300068ac <_BOOT_Validate_ImgHash+0x230>
300067ce:	f012 0218 	ands.w	r2, r2, #24
300067d2:	f026 031f 	bic.w	r3, r6, #31
300067d6:	bf18      	it	ne
300067d8:	461e      	movne	r6, r3
300067da:	9200      	str	r2, [sp, #0]
300067dc:	455c      	cmp	r4, fp
300067de:	d106      	bne.n	300067ee <_BOOT_Validate_ImgHash+0x172>
300067e0:	a914      	add	r1, sp, #80	; 0x50
300067e2:	a804      	add	r0, sp, #16
300067e4:	4b2f      	ldr	r3, [pc, #188]	; (300068a4 <_BOOT_Validate_ImgHash+0x228>)
300067e6:	4798      	blx	r3
300067e8:	2800      	cmp	r0, #0
300067ea:	d08f      	beq.n	3000670c <_BOOT_Validate_ImgHash+0x90>
300067ec:	e76e      	b.n	300066cc <_BOOT_Validate_ImgHash+0x50>
300067ee:	e9d4 7800 	ldrd	r7, r8, [r4]
300067f2:	f107 4378 	add.w	r3, r7, #4160749568	; 0xf8000000
300067f6:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300067fa:	d230      	bcs.n	3000685e <_BOOT_Validate_ImgHash+0x1e2>
300067fc:	f1b8 0f00 	cmp.w	r8, #0
30006800:	d101      	bne.n	30006806 <_BOOT_Validate_ImgHash+0x18a>
30006802:	3408      	adds	r4, #8
30006804:	e7ea      	b.n	300067dc <_BOOT_Validate_ImgHash+0x160>
30006806:	f5b8 7f00 	cmp.w	r8, #512	; 0x200
3000680a:	46c2      	mov	sl, r8
3000680c:	4639      	mov	r1, r7
3000680e:	a84c      	add	r0, sp, #304	; 0x130
30006810:	bf28      	it	cs
30006812:	f44f 7a00 	movcs.w	sl, #512	; 0x200
30006816:	4652      	mov	r2, sl
30006818:	f7fd feaa 	bl	30004570 <BOOT_ImgCopy>
3000681c:	f8d9 2014 	ldr.w	r2, [r9, #20]
30006820:	03d3      	lsls	r3, r2, #15
30006822:	d510      	bpl.n	30006846 <_BOOT_Validate_ImgHash+0x1ca>
30006824:	9b00      	ldr	r3, [sp, #0]
30006826:	eb0a 0203 	add.w	r2, sl, r3
3000682a:	f3bf 8f4f 	dsb	sy
3000682e:	4631      	mov	r1, r6
30006830:	4432      	add	r2, r6
30006832:	f8c9 1270 	str.w	r1, [r9, #624]	; 0x270
30006836:	3120      	adds	r1, #32
30006838:	1a50      	subs	r0, r2, r1
3000683a:	2800      	cmp	r0, #0
3000683c:	dcf9      	bgt.n	30006832 <_BOOT_Validate_ImgHash+0x1b6>
3000683e:	f3bf 8f4f 	dsb	sy
30006842:	f3bf 8f6f 	isb	sy
30006846:	aa14      	add	r2, sp, #80	; 0x50
30006848:	4651      	mov	r1, sl
3000684a:	a84c      	add	r0, sp, #304	; 0x130
3000684c:	4b16      	ldr	r3, [pc, #88]	; (300068a8 <_BOOT_Validate_ImgHash+0x22c>)
3000684e:	4798      	blx	r3
30006850:	2800      	cmp	r0, #0
30006852:	f47f af3b 	bne.w	300066cc <_BOOT_Validate_ImgHash+0x50>
30006856:	eba8 080a 	sub.w	r8, r8, sl
3000685a:	4457      	add	r7, sl
3000685c:	e7ce      	b.n	300067fc <_BOOT_Validate_ImgHash+0x180>
3000685e:	aa14      	add	r2, sp, #80	; 0x50
30006860:	4641      	mov	r1, r8
30006862:	4638      	mov	r0, r7
30006864:	4b10      	ldr	r3, [pc, #64]	; (300068a8 <_BOOT_Validate_ImgHash+0x22c>)
30006866:	4798      	blx	r3
30006868:	2800      	cmp	r0, #0
3000686a:	d0ca      	beq.n	30006802 <_BOOT_Validate_ImgHash+0x186>
3000686c:	e72e      	b.n	300066cc <_BOOT_Validate_ImgHash+0x50>
3000686e:	9901      	ldr	r1, [sp, #4]
30006870:	3301      	adds	r3, #1
30006872:	5c37      	ldrb	r7, [r6, r0]
30006874:	5c09      	ldrb	r1, [r1, r0]
30006876:	3001      	adds	r0, #1
30006878:	4079      	eors	r1, r7
3000687a:	b2c0      	uxtb	r0, r0
3000687c:	430a      	orrs	r2, r1
3000687e:	fbb0 f1f4 	udiv	r1, r0, r4
30006882:	fb04 0011 	mls	r0, r4, r1, r0
30006886:	b2c0      	uxtb	r0, r0
30006888:	e74c      	b.n	30006724 <_BOOT_Validate_ImgHash+0xa8>
3000688a:	bf00      	nop
3000688c:	30009025 	.word	0x30009025
30006890:	30008951 	.word	0x30008951
30006894:	30009091 	.word	0x30009091
30006898:	0000ea4d 	.word	0x0000ea4d
3000689c:	30009059 	.word	0x30009059
300068a0:	300090c5 	.word	0x300090c5
300068a4:	30009131 	.word	0x30009131
300068a8:	300090f9 	.word	0x300090f9
300068ac:	e000ed00 	.word	0xe000ed00

300068b0 <BOOT_Validate_ImgHash>:
300068b0:	b51f      	push	{r0, r1, r2, r3, r4, lr}
300068b2:	4604      	mov	r4, r0
300068b4:	480a      	ldr	r0, [pc, #40]	; (300068e0 <BOOT_Validate_ImgHash+0x30>)
300068b6:	9301      	str	r3, [sp, #4]
300068b8:	e9cd 2102 	strd	r2, r1, [sp, #8]
300068bc:	4780      	blx	r0
300068be:	9903      	ldr	r1, [sp, #12]
300068c0:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300068c4:	b128      	cbz	r0, 300068d2 <BOOT_Validate_ImgHash+0x22>
300068c6:	4620      	mov	r0, r4
300068c8:	b004      	add	sp, #16
300068ca:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300068ce:	f002 bc87 	b.w	300091e0 <SBOOT_Validate_ImgHash>
300068d2:	4620      	mov	r0, r4
300068d4:	b004      	add	sp, #16
300068d6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300068da:	f7ff becf 	b.w	3000667c <_BOOT_Validate_ImgHash>
300068de:	bf00      	nop
300068e0:	30008f81 	.word	0x30008f81

300068e4 <BOOT_SignatureCheck>:
300068e4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300068e8:	461f      	mov	r7, r3
300068ea:	b089      	sub	sp, #36	; 0x24
300068ec:	4b5d      	ldr	r3, [pc, #372]	; (30006a64 <BOOT_SignatureCheck+0x180>)
300068ee:	4606      	mov	r6, r0
300068f0:	4689      	mov	r9, r1
300068f2:	4690      	mov	r8, r2
300068f4:	ac08      	add	r4, sp, #32
300068f6:	9d10      	ldr	r5, [sp, #64]	; 0x40
300068f8:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300068fa:	e904 000f 	stmdb	r4, {r0, r1, r2, r3}
300068fe:	4b5a      	ldr	r3, [pc, #360]	; (30006a68 <BOOT_SignatureCheck+0x184>)
30006900:	4798      	blx	r3
30006902:	2800      	cmp	r0, #0
30006904:	d04e      	beq.n	300069a4 <BOOT_SignatureCheck+0xc0>
30006906:	4b59      	ldr	r3, [pc, #356]	; (30006a6c <BOOT_SignatureCheck+0x188>)
30006908:	2000      	movs	r0, #0
3000690a:	4798      	blx	r3
3000690c:	7cf3      	ldrb	r3, [r6, #19]
3000690e:	7cb2      	ldrb	r2, [r6, #18]
30006910:	f10d 010f 	add.w	r1, sp, #15
30006914:	f10d 000e 	add.w	r0, sp, #14
30006918:	f002 fc94 	bl	30009244 <SBOOT_Validate_Algorithm>
3000691c:	4604      	mov	r4, r0
3000691e:	2800      	cmp	r0, #0
30006920:	d143      	bne.n	300069aa <BOOT_SignatureCheck+0xc6>
30006922:	69fb      	ldr	r3, [r7, #28]
30006924:	2124      	movs	r1, #36	; 0x24
30006926:	b2c2      	uxtb	r2, r0
30006928:	4293      	cmp	r3, r2
3000692a:	d906      	bls.n	3000693a <BOOT_SignatureCheck+0x56>
3000692c:	fb01 7402 	mla	r4, r1, r2, r7
30006930:	3001      	adds	r0, #1
30006932:	f8d4 40b0 	ldr.w	r4, [r4, #176]	; 0xb0
30006936:	42ac      	cmp	r4, r5
30006938:	d1f5      	bne.n	30006926 <BOOT_SignatureCheck+0x42>
3000693a:	2324      	movs	r3, #36	; 0x24
3000693c:	f106 0570 	add.w	r5, r6, #112	; 0x70
30006940:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006944:	fb03 7202 	mla	r2, r3, r2, r7
30006948:	4629      	mov	r1, r5
3000694a:	32b4      	adds	r2, #180	; 0xb4
3000694c:	f002 fc0a 	bl	30009164 <SBOOT_Validate_PubKey>
30006950:	4604      	mov	r4, r0
30006952:	bb50      	cbnz	r0, 300069aa <BOOT_SignatureCheck+0xc6>
30006954:	f506 7380 	add.w	r3, r6, #256	; 0x100
30006958:	462a      	mov	r2, r5
3000695a:	f89d 100f 	ldrb.w	r1, [sp, #15]
3000695e:	9301      	str	r3, [sp, #4]
30006960:	f44f 7380 	mov.w	r3, #256	; 0x100
30006964:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006968:	9300      	str	r3, [sp, #0]
3000696a:	4633      	mov	r3, r6
3000696c:	f002 fc16 	bl	3000919c <SBOOT_Validate_Signature>
30006970:	4604      	mov	r4, r0
30006972:	b9d0      	cbnz	r0, 300069aa <BOOT_SignatureCheck+0xc6>
30006974:	4643      	mov	r3, r8
30006976:	464a      	mov	r2, r9
30006978:	f106 0130 	add.w	r1, r6, #48	; 0x30
3000697c:	f89d 000f 	ldrb.w	r0, [sp, #15]
30006980:	f7ff ff96 	bl	300068b0 <BOOT_Validate_ImgHash>
30006984:	4604      	mov	r4, r0
30006986:	b980      	cbnz	r0, 300069aa <BOOT_SignatureCheck+0xc6>
30006988:	7c73      	ldrb	r3, [r6, #17]
3000698a:	aa08      	add	r2, sp, #32
3000698c:	2004      	movs	r0, #4
3000698e:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30006992:	4a37      	ldr	r2, [pc, #220]	; (30006a70 <BOOT_SignatureCheck+0x18c>)
30006994:	f853 3c10 	ldr.w	r3, [r3, #-16]
30006998:	9300      	str	r3, [sp, #0]
3000699a:	4b36      	ldr	r3, [pc, #216]	; (30006a74 <BOOT_SignatureCheck+0x190>)
3000699c:	4619      	mov	r1, r3
3000699e:	f004 fa83 	bl	3000aea8 <rtk_log_write>
300069a2:	4620      	mov	r0, r4
300069a4:	b009      	add	sp, #36	; 0x24
300069a6:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300069aa:	9401      	str	r4, [sp, #4]
300069ac:	aa08      	add	r2, sp, #32
300069ae:	7c73      	ldrb	r3, [r6, #17]
300069b0:	eb09 08c8 	add.w	r8, r9, r8, lsl #3
300069b4:	4f30      	ldr	r7, [pc, #192]	; (30006a78 <BOOT_SignatureCheck+0x194>)
300069b6:	2002      	movs	r0, #2
300069b8:	eb02 0383 	add.w	r3, r2, r3, lsl #2
300069bc:	4e2f      	ldr	r6, [pc, #188]	; (30006a7c <BOOT_SignatureCheck+0x198>)
300069be:	4a30      	ldr	r2, [pc, #192]	; (30006a80 <BOOT_SignatureCheck+0x19c>)
300069c0:	f853 3c10 	ldr.w	r3, [r3, #-16]
300069c4:	9300      	str	r3, [sp, #0]
300069c6:	4b2b      	ldr	r3, [pc, #172]	; (30006a74 <BOOT_SignatureCheck+0x190>)
300069c8:	4619      	mov	r1, r3
300069ca:	f004 fa6d 	bl	3000aea8 <rtk_log_write>
300069ce:	45c8      	cmp	r8, r9
300069d0:	d101      	bne.n	300069d6 <BOOT_SignatureCheck+0xf2>
300069d2:	20ff      	movs	r0, #255	; 0xff
300069d4:	e7e6      	b.n	300069a4 <BOOT_SignatureCheck+0xc0>
300069d6:	f8d9 0000 	ldr.w	r0, [r9]
300069da:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
300069de:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300069e2:	d32b      	bcc.n	30006a3c <BOOT_SignatureCheck+0x158>
300069e4:	f8d9 2004 	ldr.w	r2, [r9, #4]
300069e8:	2100      	movs	r1, #0
300069ea:	47b8      	blx	r7
300069ec:	6973      	ldr	r3, [r6, #20]
300069ee:	03db      	lsls	r3, r3, #15
300069f0:	d524      	bpl.n	30006a3c <BOOT_SignatureCheck+0x158>
300069f2:	e9d9 3100 	ldrd	r3, r1, [r9]
300069f6:	ea03 0201 	and.w	r2, r3, r1
300069fa:	3201      	adds	r2, #1
300069fc:	d121      	bne.n	30006a42 <BOOT_SignatureCheck+0x15e>
300069fe:	2300      	movs	r3, #0
30006a00:	f8c6 3084 	str.w	r3, [r6, #132]	; 0x84
30006a04:	f3bf 8f4f 	dsb	sy
30006a08:	f8d6 1080 	ldr.w	r1, [r6, #128]	; 0x80
30006a0c:	f643 70e0 	movw	r0, #16352	; 0x3fe0
30006a10:	f3c1 02c9 	ubfx	r2, r1, #3, #10
30006a14:	f3c1 314e 	ubfx	r1, r1, #13, #15
30006a18:	0149      	lsls	r1, r1, #5
30006a1a:	ea01 0e00 	and.w	lr, r1, r0
30006a1e:	4613      	mov	r3, r2
30006a20:	ea4e 7483 	orr.w	r4, lr, r3, lsl #30
30006a24:	3b01      	subs	r3, #1
30006a26:	f8c6 4274 	str.w	r4, [r6, #628]	; 0x274
30006a2a:	d2f9      	bcs.n	30006a20 <BOOT_SignatureCheck+0x13c>
30006a2c:	3920      	subs	r1, #32
30006a2e:	f111 0f20 	cmn.w	r1, #32
30006a32:	d1f2      	bne.n	30006a1a <BOOT_SignatureCheck+0x136>
30006a34:	f3bf 8f4f 	dsb	sy
30006a38:	f3bf 8f6f 	isb	sy
30006a3c:	f109 0908 	add.w	r9, r9, #8
30006a40:	e7c5      	b.n	300069ce <BOOT_SignatureCheck+0xea>
30006a42:	f013 021f 	ands.w	r2, r3, #31
30006a46:	bf1c      	itt	ne
30006a48:	f023 031f 	bicne.w	r3, r3, #31
30006a4c:	1889      	addne	r1, r1, r2
30006a4e:	f3bf 8f4f 	dsb	sy
30006a52:	440b      	add	r3, r1
30006a54:	2900      	cmp	r1, #0
30006a56:	eba3 0201 	sub.w	r2, r3, r1
30006a5a:	ddeb      	ble.n	30006a34 <BOOT_SignatureCheck+0x150>
30006a5c:	3920      	subs	r1, #32
30006a5e:	f8c6 2270 	str.w	r2, [r6, #624]	; 0x270
30006a62:	e7f7      	b.n	30006a54 <BOOT_SignatureCheck+0x170>
30006a64:	3000d2b8 	.word	0x3000d2b8
30006a68:	0000c149 	.word	0x0000c149
30006a6c:	3000921d 	.word	0x3000921d
30006a70:	3000d24c 	.word	0x3000d24c
30006a74:	3000c79c 	.word	0x3000c79c
30006a78:	00012be5 	.word	0x00012be5
30006a7c:	e000ed00 	.word	0xe000ed00
30006a80:	3000d263 	.word	0x3000d263

30006a84 <BOOT_CertificateCheck>:
30006a84:	b570      	push	{r4, r5, r6, lr}
30006a86:	4b28      	ldr	r3, [pc, #160]	; (30006b28 <BOOT_CertificateCheck+0xa4>)
30006a88:	b0b0      	sub	sp, #192	; 0xc0
30006a8a:	4604      	mov	r4, r0
30006a8c:	460e      	mov	r6, r1
30006a8e:	4798      	blx	r3
30006a90:	b910      	cbnz	r0, 30006a98 <BOOT_CertificateCheck+0x14>
30006a92:	2000      	movs	r0, #0
30006a94:	b030      	add	sp, #192	; 0xc0
30006a96:	bd70      	pop	{r4, r5, r6, pc}
30006a98:	69a1      	ldr	r1, [r4, #24]
30006a9a:	ad0c      	add	r5, sp, #48	; 0x30
30006a9c:	2290      	movs	r2, #144	; 0x90
30006a9e:	4431      	add	r1, r6
30006aa0:	4628      	mov	r0, r5
30006aa2:	f7fd fd65 	bl	30004570 <BOOT_ImgCopy>
30006aa6:	4b21      	ldr	r3, [pc, #132]	; (30006b2c <BOOT_CertificateCheck+0xa8>)
30006aa8:	4798      	blx	r3
30006aaa:	4a21      	ldr	r2, [pc, #132]	; (30006b30 <BOOT_CertificateCheck+0xac>)
30006aac:	f000 031f 	and.w	r3, r0, #31
30006ab0:	2020      	movs	r0, #32
30006ab2:	6812      	ldr	r2, [r2, #0]
30006ab4:	f102 4284 	add.w	r2, r2, #1107296256	; 0x42000000
30006ab8:	a930      	add	r1, sp, #192	; 0xc0
30006aba:	3801      	subs	r0, #1
30006abc:	5c9e      	ldrb	r6, [r3, r2]
30006abe:	4419      	add	r1, r3
30006ac0:	f010 00ff 	ands.w	r0, r0, #255	; 0xff
30006ac4:	f103 0301 	add.w	r3, r3, #1
30006ac8:	f801 6cb0 	strb.w	r6, [r1, #-176]
30006acc:	f003 031f 	and.w	r3, r3, #31
30006ad0:	d1f2      	bne.n	30006ab8 <BOOT_CertificateCheck+0x34>
30006ad2:	4b18      	ldr	r3, [pc, #96]	; (30006b34 <BOOT_CertificateCheck+0xb0>)
30006ad4:	4798      	blx	r3
30006ad6:	7ce3      	ldrb	r3, [r4, #19]
30006ad8:	7ca2      	ldrb	r2, [r4, #18]
30006ada:	f10d 010f 	add.w	r1, sp, #15
30006ade:	f10d 000e 	add.w	r0, sp, #14
30006ae2:	f002 fbaf 	bl	30009244 <SBOOT_Validate_Algorithm>
30006ae6:	b9a8      	cbnz	r0, 30006b14 <BOOT_CertificateCheck+0x90>
30006ae8:	f104 0620 	add.w	r6, r4, #32
30006aec:	aa04      	add	r2, sp, #16
30006aee:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006af2:	4631      	mov	r1, r6
30006af4:	f002 fb36 	bl	30009164 <SBOOT_Validate_PubKey>
30006af8:	b960      	cbnz	r0, 30006b14 <BOOT_CertificateCheck+0x90>
30006afa:	69a3      	ldr	r3, [r4, #24]
30006afc:	4632      	mov	r2, r6
30006afe:	f89d 100f 	ldrb.w	r1, [sp, #15]
30006b02:	9300      	str	r3, [sp, #0]
30006b04:	4623      	mov	r3, r4
30006b06:	f89d 000e 	ldrb.w	r0, [sp, #14]
30006b0a:	9501      	str	r5, [sp, #4]
30006b0c:	f002 fb46 	bl	3000919c <SBOOT_Validate_Signature>
30006b10:	2800      	cmp	r0, #0
30006b12:	d0be      	beq.n	30006a92 <BOOT_CertificateCheck+0xe>
30006b14:	4b08      	ldr	r3, [pc, #32]	; (30006b38 <BOOT_CertificateCheck+0xb4>)
30006b16:	9000      	str	r0, [sp, #0]
30006b18:	2002      	movs	r0, #2
30006b1a:	4a08      	ldr	r2, [pc, #32]	; (30006b3c <BOOT_CertificateCheck+0xb8>)
30006b1c:	4619      	mov	r1, r3
30006b1e:	f004 f9c3 	bl	3000aea8 <rtk_log_write>
30006b22:	20ff      	movs	r0, #255	; 0xff
30006b24:	e7b6      	b.n	30006a94 <BOOT_CertificateCheck+0x10>
30006b26:	bf00      	nop
30006b28:	0000c149 	.word	0x0000c149
30006b2c:	0000ea4d 	.word	0x0000ea4d
30006b30:	3000d5cc 	.word	0x3000d5cc
30006b34:	3000921d 	.word	0x3000921d
30006b38:	3000c79c 	.word	0x3000c79c
30006b3c:	3000d284 	.word	0x3000d284

30006b40 <BOOT_RAM_TZCfg>:
30006b40:	e92d 47ff 	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, lr}
30006b44:	4a5b      	ldr	r2, [pc, #364]	; (30006cb4 <BOOT_RAM_TZCfg+0x174>)
30006b46:	466e      	mov	r6, sp
30006b48:	4b5b      	ldr	r3, [pc, #364]	; (30006cb8 <BOOT_RAM_TZCfg+0x178>)
30006b4a:	f10d 0c08 	add.w	ip, sp, #8
30006b4e:	2400      	movs	r4, #0
30006b50:	2501      	movs	r5, #1
30006b52:	f8df e188 	ldr.w	lr, [pc, #392]	; 30006cdc <BOOT_RAM_TZCfg+0x19c>
30006b56:	f44f 18c0 	mov.w	r8, #1572864	; 0x180000
30006b5a:	f8df 9184 	ldr.w	r9, [pc, #388]	; 30006ce0 <BOOT_RAM_TZCfg+0x1a0>
30006b5e:	e9cd 3200 	strd	r3, r2, [sp]
30006b62:	4a56      	ldr	r2, [pc, #344]	; (30006cbc <BOOT_RAM_TZCfg+0x17c>)
30006b64:	9202      	str	r2, [sp, #8]
30006b66:	4a56      	ldr	r2, [pc, #344]	; (30006cc0 <BOOT_RAM_TZCfg+0x180>)
30006b68:	9203      	str	r2, [sp, #12]
30006b6a:	f856 2b04 	ldr.w	r2, [r6], #4
30006b6e:	2100      	movs	r1, #0
30006b70:	f85c 0b04 	ldr.w	r0, [ip], #4
30006b74:	6807      	ldr	r7, [r0, #0]
30006b76:	f1b7 3fff 	cmp.w	r7, #4294967295	; 0xffffffff
30006b7a:	d011      	beq.n	30006ba0 <BOOT_RAM_TZCfg+0x60>
30006b7c:	f842 7031 	str.w	r7, [r2, r1, lsl #3]
30006b80:	eb02 0ac1 	add.w	sl, r2, r1, lsl #3
30006b84:	6847      	ldr	r7, [r0, #4]
30006b86:	300c      	adds	r0, #12
30006b88:	f8ca 7004 	str.w	r7, [sl, #4]
30006b8c:	fa05 f701 	lsl.w	r7, r5, r1
30006b90:	f8d2 a040 	ldr.w	sl, [r2, #64]	; 0x40
30006b94:	3101      	adds	r1, #1
30006b96:	ea47 070a 	orr.w	r7, r7, sl
30006b9a:	2908      	cmp	r1, #8
30006b9c:	6417      	str	r7, [r2, #64]	; 0x40
30006b9e:	d1e9      	bne.n	30006b74 <BOOT_RAM_TZCfg+0x34>
30006ba0:	429a      	cmp	r2, r3
30006ba2:	f040 8084 	bne.w	30006cae <BOOT_RAM_TZCfg+0x16e>
30006ba6:	f89e 2000 	ldrb.w	r2, [lr]
30006baa:	b942      	cbnz	r2, 30006bbe <BOOT_RAM_TZCfg+0x7e>
30006bac:	f8c3 8038 	str.w	r8, [r3, #56]	; 0x38
30006bb0:	f8c3 903c 	str.w	r9, [r3, #60]	; 0x3c
30006bb4:	6c1a      	ldr	r2, [r3, #64]	; 0x40
30006bb6:	f042 0280 	orr.w	r2, r2, #128	; 0x80
30006bba:	641a      	str	r2, [r3, #64]	; 0x40
30006bbc:	645d      	str	r5, [r3, #68]	; 0x44
30006bbe:	2c00      	cmp	r4, #0
30006bc0:	d073      	beq.n	30006caa <BOOT_RAM_TZCfg+0x16a>
30006bc2:	4b40      	ldr	r3, [pc, #256]	; (30006cc4 <BOOT_RAM_TZCfg+0x184>)
30006bc4:	f46f 3200 	mvn.w	r2, #131072	; 0x20000
30006bc8:	2100      	movs	r1, #0
30006bca:	483f      	ldr	r0, [pc, #252]	; (30006cc8 <BOOT_RAM_TZCfg+0x188>)
30006bcc:	601a      	str	r2, [r3, #0]
30006bce:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30006bd2:	605a      	str	r2, [r3, #4]
30006bd4:	4b3d      	ldr	r3, [pc, #244]	; (30006ccc <BOOT_RAM_TZCfg+0x18c>)
30006bd6:	4a3e      	ldr	r2, [pc, #248]	; (30006cd0 <BOOT_RAM_TZCfg+0x190>)
30006bd8:	f8c3 4398 	str.w	r4, [r3, #920]	; 0x398
30006bdc:	6813      	ldr	r3, [r2, #0]
30006bde:	3301      	adds	r3, #1
30006be0:	d01a      	beq.n	30006c18 <BOOT_RAM_TZCfg+0xd8>
30006be2:	f8c0 10d8 	str.w	r1, [r0, #216]	; 0xd8
30006be6:	3101      	adds	r1, #1
30006be8:	6813      	ldr	r3, [r2, #0]
30006bea:	320c      	adds	r2, #12
30006bec:	2908      	cmp	r1, #8
30006bee:	f023 031f 	bic.w	r3, r3, #31
30006bf2:	f8c0 30dc 	str.w	r3, [r0, #220]	; 0xdc
30006bf6:	f852 3c04 	ldr.w	r3, [r2, #-4]
30006bfa:	f852 4c08 	ldr.w	r4, [r2, #-8]
30006bfe:	ea4f 0343 	mov.w	r3, r3, lsl #1
30006c02:	f024 041f 	bic.w	r4, r4, #31
30006c06:	f003 0302 	and.w	r3, r3, #2
30006c0a:	ea43 0304 	orr.w	r3, r3, r4
30006c0e:	f043 0301 	orr.w	r3, r3, #1
30006c12:	f8c0 30e0 	str.w	r3, [r0, #224]	; 0xe0
30006c16:	d1e1      	bne.n	30006bdc <BOOT_RAM_TZCfg+0x9c>
30006c18:	2301      	movs	r3, #1
30006c1a:	4a2b      	ldr	r2, [pc, #172]	; (30006cc8 <BOOT_RAM_TZCfg+0x188>)
30006c1c:	f8c2 30d0 	str.w	r3, [r2, #208]	; 0xd0
30006c20:	4b2c      	ldr	r3, [pc, #176]	; (30006cd4 <BOOT_RAM_TZCfg+0x194>)
30006c22:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30006c26:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30006c2a:	d11d      	bne.n	30006c68 <BOOT_RAM_TZCfg+0x128>
30006c2c:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30006c30:	f3bf 8f4f 	dsb	sy
30006c34:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
30006c38:	f643 74e0 	movw	r4, #16352	; 0x3fe0
30006c3c:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30006c40:	f3c3 334e 	ubfx	r3, r3, #13, #15
30006c44:	015b      	lsls	r3, r3, #5
30006c46:	ea03 0604 	and.w	r6, r3, r4
30006c4a:	4601      	mov	r1, r0
30006c4c:	ea46 7581 	orr.w	r5, r6, r1, lsl #30
30006c50:	3901      	subs	r1, #1
30006c52:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
30006c56:	d2f9      	bcs.n	30006c4c <BOOT_RAM_TZCfg+0x10c>
30006c58:	3b20      	subs	r3, #32
30006c5a:	f113 0f20 	cmn.w	r3, #32
30006c5e:	d1f2      	bne.n	30006c46 <BOOT_RAM_TZCfg+0x106>
30006c60:	f3bf 8f4f 	dsb	sy
30006c64:	f3bf 8f6f 	isb	sy
30006c68:	4a17      	ldr	r2, [pc, #92]	; (30006cc8 <BOOT_RAM_TZCfg+0x188>)
30006c6a:	6913      	ldr	r3, [r2, #16]
30006c6c:	f043 0308 	orr.w	r3, r3, #8
30006c70:	6113      	str	r3, [r2, #16]
30006c72:	68d3      	ldr	r3, [r2, #12]
30006c74:	f423 43c0 	bic.w	r3, r3, #24576	; 0x6000
30006c78:	f023 0308 	bic.w	r3, r3, #8
30006c7c:	041b      	lsls	r3, r3, #16
30006c7e:	0c1b      	lsrs	r3, r3, #16
30006c80:	f043 63bf 	orr.w	r3, r3, #100139008	; 0x5f80000
30006c84:	f443 3318 	orr.w	r3, r3, #155648	; 0x26000
30006c88:	60d3      	str	r3, [r2, #12]
30006c8a:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30006c8e:	4b12      	ldr	r3, [pc, #72]	; (30006cd8 <BOOT_RAM_TZCfg+0x198>)
30006c90:	f8c3 2280 	str.w	r2, [r3, #640]	; 0x280
30006c94:	f46f 0280 	mvn.w	r2, #4194304	; 0x400000
30006c98:	f8c3 2284 	str.w	r2, [r3, #644]	; 0x284
30006c9c:	f06f 02c0 	mvn.w	r2, #192	; 0xc0
30006ca0:	f8c3 2288 	str.w	r2, [r3, #648]	; 0x288
30006ca4:	b004      	add	sp, #16
30006ca6:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30006caa:	2401      	movs	r4, #1
30006cac:	e75d      	b.n	30006b6a <BOOT_RAM_TZCfg+0x2a>
30006cae:	6455      	str	r5, [r2, #68]	; 0x44
30006cb0:	e785      	b.n	30006bbe <BOOT_RAM_TZCfg+0x7e>
30006cb2:	bf00      	nop
30006cb4:	51001200 	.word	0x51001200
30006cb8:	51001a00 	.word	0x51001a00
30006cbc:	3000d6cc 	.word	0x3000d6cc
30006cc0:	3000d66c 	.word	0x3000d66c
30006cc4:	51001b00 	.word	0x51001b00
30006cc8:	e000ed00 	.word	0xe000ed00
30006ccc:	500e0000 	.word	0x500e0000
30006cd0:	3000d72c 	.word	0x3000d72c
30006cd4:	42008000 	.word	0x42008000
30006cd8:	e000e100 	.word	0xe000e100
30006cdc:	3000d5c7 	.word	0x3000d5c7
30006ce0:	005fffff 	.word	0x005fffff

30006ce4 <ymodem_inquiry>:
30006ce4:	2806      	cmp	r0, #6
30006ce6:	b510      	push	{r4, lr}
30006ce8:	4604      	mov	r4, r0
30006cea:	d003      	beq.n	30006cf4 <ymodem_inquiry+0x10>
30006cec:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30006cf0:	4b03      	ldr	r3, [pc, #12]	; (30006d00 <ymodem_inquiry+0x1c>)
30006cf2:	4798      	blx	r3
30006cf4:	4620      	mov	r0, r4
30006cf6:	f000 f9a9 	bl	3000704c <ymodem_uart_putc>
30006cfa:	2001      	movs	r0, #1
30006cfc:	bd10      	pop	{r4, pc}
30006cfe:	bf00      	nop
30006d00:	00009b2d 	.word	0x00009b2d

30006d04 <ymodem_get_first>:
30006d04:	2300      	movs	r3, #0
30006d06:	b537      	push	{r0, r1, r2, r4, r5, lr}
30006d08:	4604      	mov	r4, r0
30006d0a:	4926      	ldr	r1, [pc, #152]	; (30006da4 <ymodem_get_first+0xa0>)
30006d0c:	f10d 0006 	add.w	r0, sp, #6
30006d10:	f88d 3006 	strb.w	r3, [sp, #6]
30006d14:	f000 f9ba 	bl	3000708c <ymodem_uart_getc_to>
30006d18:	b120      	cbz	r0, 30006d24 <ymodem_get_first+0x20>
30006d1a:	2301      	movs	r3, #1
30006d1c:	2008      	movs	r0, #8
30006d1e:	60a3      	str	r3, [r4, #8]
30006d20:	b003      	add	sp, #12
30006d22:	bd30      	pop	{r4, r5, pc}
30006d24:	f89d 3006 	ldrb.w	r3, [sp, #6]
30006d28:	2b04      	cmp	r3, #4
30006d2a:	d01b      	beq.n	30006d64 <ymodem_get_first+0x60>
30006d2c:	d80c      	bhi.n	30006d48 <ymodem_get_first+0x44>
30006d2e:	2b01      	cmp	r3, #1
30006d30:	d010      	beq.n	30006d54 <ymodem_get_first+0x50>
30006d32:	2b02      	cmp	r3, #2
30006d34:	d012      	beq.n	30006d5c <ymodem_get_first+0x58>
30006d36:	4d1b      	ldr	r5, [pc, #108]	; (30006da4 <ymodem_get_first+0xa0>)
30006d38:	4629      	mov	r1, r5
30006d3a:	f10d 0007 	add.w	r0, sp, #7
30006d3e:	f000 f9a5 	bl	3000708c <ymodem_uart_getc_to>
30006d42:	2800      	cmp	r0, #0
30006d44:	d0f8      	beq.n	30006d38 <ymodem_get_first+0x34>
30006d46:	e7e8      	b.n	30006d1a <ymodem_get_first+0x16>
30006d48:	2b18      	cmp	r3, #24
30006d4a:	d1f4      	bne.n	30006d36 <ymodem_get_first+0x32>
30006d4c:	2301      	movs	r3, #1
30006d4e:	2002      	movs	r0, #2
30006d50:	60a3      	str	r3, [r4, #8]
30006d52:	e7e5      	b.n	30006d20 <ymodem_get_first+0x1c>
30006d54:	2280      	movs	r2, #128	; 0x80
30006d56:	80a2      	strh	r2, [r4, #4]
30006d58:	2001      	movs	r0, #1
30006d5a:	e7e1      	b.n	30006d20 <ymodem_get_first+0x1c>
30006d5c:	f44f 6380 	mov.w	r3, #1024	; 0x400
30006d60:	80a3      	strh	r3, [r4, #4]
30006d62:	e7f9      	b.n	30006d58 <ymodem_get_first+0x54>
30006d64:	2006      	movs	r0, #6
30006d66:	f000 f971 	bl	3000704c <ymodem_uart_putc>
30006d6a:	2043      	movs	r0, #67	; 0x43
30006d6c:	f7ff ffba 	bl	30006ce4 <ymodem_inquiry>
30006d70:	2006      	movs	r0, #6
30006d72:	f000 f96b 	bl	3000704c <ymodem_uart_putc>
30006d76:	2043      	movs	r0, #67	; 0x43
30006d78:	f7ff ffb4 	bl	30006ce4 <ymodem_inquiry>
30006d7c:	2018      	movs	r0, #24
30006d7e:	f7ff ffb1 	bl	30006ce4 <ymodem_inquiry>
30006d82:	2018      	movs	r0, #24
30006d84:	f7ff ffae 	bl	30006ce4 <ymodem_inquiry>
30006d88:	2018      	movs	r0, #24
30006d8a:	f7ff ffab 	bl	30006ce4 <ymodem_inquiry>
30006d8e:	2018      	movs	r0, #24
30006d90:	f7ff ffa8 	bl	30006ce4 <ymodem_inquiry>
30006d94:	2018      	movs	r0, #24
30006d96:	f7ff ffa5 	bl	30006ce4 <ymodem_inquiry>
30006d9a:	2301      	movs	r3, #1
30006d9c:	2005      	movs	r0, #5
30006d9e:	60a3      	str	r3, [r4, #8]
30006da0:	e7be      	b.n	30006d20 <ymodem_get_first+0x1c>
30006da2:	bf00      	nop
30006da4:	000caa30 	.word	0x000caa30

30006da8 <ymodem_get_others>:
30006da8:	2300      	movs	r3, #0
30006daa:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
30006dae:	4605      	mov	r5, r0
30006db0:	494c      	ldr	r1, [pc, #304]	; (30006ee4 <ymodem_get_others+0x13c>)
30006db2:	a801      	add	r0, sp, #4
30006db4:	f88d 3006 	strb.w	r3, [sp, #6]
30006db8:	f88d 3007 	strb.w	r3, [sp, #7]
30006dbc:	f000 f966 	bl	3000708c <ymodem_uart_getc_to>
30006dc0:	b130      	cbz	r0, 30006dd0 <ymodem_get_others+0x28>
30006dc2:	2301      	movs	r3, #1
30006dc4:	2408      	movs	r4, #8
30006dc6:	60ab      	str	r3, [r5, #8]
30006dc8:	4620      	mov	r0, r4
30006dca:	b003      	add	sp, #12
30006dcc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006dd0:	4944      	ldr	r1, [pc, #272]	; (30006ee4 <ymodem_get_others+0x13c>)
30006dd2:	f10d 0005 	add.w	r0, sp, #5
30006dd6:	f000 f959 	bl	3000708c <ymodem_uart_getc_to>
30006dda:	4606      	mov	r6, r0
30006ddc:	2800      	cmp	r0, #0
30006dde:	d1f0      	bne.n	30006dc2 <ymodem_get_others+0x1a>
30006de0:	f89d 2004 	ldrb.w	r2, [sp, #4]
30006de4:	f89d 4005 	ldrb.w	r4, [sp, #5]
30006de8:	43d3      	mvns	r3, r2
30006dea:	b2db      	uxtb	r3, r3
30006dec:	429c      	cmp	r4, r3
30006dee:	bf0c      	ite	eq
30006df0:	2401      	moveq	r4, #1
30006df2:	2404      	movne	r4, #4
30006df4:	682b      	ldr	r3, [r5, #0]
30006df6:	429a      	cmp	r2, r3
30006df8:	d330      	bcc.n	30006e5c <ymodem_get_others+0xb4>
30006dfa:	d000      	beq.n	30006dfe <ymodem_get_others+0x56>
30006dfc:	2404      	movs	r4, #4
30006dfe:	2700      	movs	r7, #0
30006e00:	f8df 80e4 	ldr.w	r8, [pc, #228]	; 30006ee8 <ymodem_get_others+0x140>
30006e04:	f8df 90dc 	ldr.w	r9, [pc, #220]	; 30006ee4 <ymodem_get_others+0x13c>
30006e08:	88ab      	ldrh	r3, [r5, #4]
30006e0a:	42bb      	cmp	r3, r7
30006e0c:	d828      	bhi.n	30006e60 <ymodem_get_others+0xb8>
30006e0e:	4935      	ldr	r1, [pc, #212]	; (30006ee4 <ymodem_get_others+0x13c>)
30006e10:	f10d 0006 	add.w	r0, sp, #6
30006e14:	f000 f93a 	bl	3000708c <ymodem_uart_getc_to>
30006e18:	2800      	cmp	r0, #0
30006e1a:	d1d2      	bne.n	30006dc2 <ymodem_get_others+0x1a>
30006e1c:	4931      	ldr	r1, [pc, #196]	; (30006ee4 <ymodem_get_others+0x13c>)
30006e1e:	f10d 0007 	add.w	r0, sp, #7
30006e22:	f000 f933 	bl	3000708c <ymodem_uart_getc_to>
30006e26:	4603      	mov	r3, r0
30006e28:	2800      	cmp	r0, #0
30006e2a:	d1ca      	bne.n	30006dc2 <ymodem_get_others+0x1a>
30006e2c:	f89d 2006 	ldrb.w	r2, [sp, #6]
30006e30:	f241 0e21 	movw	lr, #4129	; 0x1021
30006e34:	f89d 0007 	ldrb.w	r0, [sp, #7]
30006e38:	492b      	ldr	r1, [pc, #172]	; (30006ee8 <ymodem_get_others+0x140>)
30006e3a:	ea40 2002 	orr.w	r0, r0, r2, lsl #8
30006e3e:	88aa      	ldrh	r2, [r5, #4]
30006e40:	eb02 0c01 	add.w	ip, r2, r1
30006e44:	4561      	cmp	r1, ip
30006e46:	d114      	bne.n	30006e72 <ymodem_get_others+0xca>
30006e48:	b29b      	uxth	r3, r3
30006e4a:	4298      	cmp	r0, r3
30006e4c:	d021      	beq.n	30006e92 <ymodem_get_others+0xea>
30006e4e:	2015      	movs	r0, #21
30006e50:	2404      	movs	r4, #4
30006e52:	f7ff ff47 	bl	30006ce4 <ymodem_inquiry>
30006e56:	2301      	movs	r3, #1
30006e58:	60ab      	str	r3, [r5, #8]
30006e5a:	e7b5      	b.n	30006dc8 <ymodem_get_others+0x20>
30006e5c:	2601      	movs	r6, #1
30006e5e:	e7ce      	b.n	30006dfe <ymodem_get_others+0x56>
30006e60:	4649      	mov	r1, r9
30006e62:	eb07 0008 	add.w	r0, r7, r8
30006e66:	f000 f911 	bl	3000708c <ymodem_uart_getc_to>
30006e6a:	2800      	cmp	r0, #0
30006e6c:	d1a9      	bne.n	30006dc2 <ymodem_get_others+0x1a>
30006e6e:	3701      	adds	r7, #1
30006e70:	e7ca      	b.n	30006e08 <ymodem_get_others+0x60>
30006e72:	f811 7b01 	ldrb.w	r7, [r1], #1
30006e76:	ea83 2307 	eor.w	r3, r3, r7, lsl #8
30006e7a:	2708      	movs	r7, #8
30006e7c:	f403 4800 	and.w	r8, r3, #32768	; 0x8000
30006e80:	005b      	lsls	r3, r3, #1
30006e82:	f1b8 0f00 	cmp.w	r8, #0
30006e86:	d001      	beq.n	30006e8c <ymodem_get_others+0xe4>
30006e88:	ea83 030e 	eor.w	r3, r3, lr
30006e8c:	3f01      	subs	r7, #1
30006e8e:	d1f5      	bne.n	30006e7c <ymodem_get_others+0xd4>
30006e90:	e7d8      	b.n	30006e44 <ymodem_get_others+0x9c>
30006e92:	b96e      	cbnz	r6, 30006eb0 <ymodem_get_others+0x108>
30006e94:	4f15      	ldr	r7, [pc, #84]	; (30006eec <ymodem_get_others+0x144>)
30006e96:	683b      	ldr	r3, [r7, #0]
30006e98:	b153      	cbz	r3, 30006eb0 <ymodem_get_others+0x108>
30006e9a:	68eb      	ldr	r3, [r5, #12]
30006e9c:	6879      	ldr	r1, [r7, #4]
30006e9e:	3301      	adds	r3, #1
30006ea0:	4811      	ldr	r0, [pc, #68]	; (30006ee8 <ymodem_get_others+0x140>)
30006ea2:	60eb      	str	r3, [r5, #12]
30006ea4:	f000 f920 	bl	300070e8 <UARTIMG_Write>
30006ea8:	687a      	ldr	r2, [r7, #4]
30006eaa:	88ab      	ldrh	r3, [r5, #4]
30006eac:	4413      	add	r3, r2
30006eae:	607b      	str	r3, [r7, #4]
30006eb0:	f000 f8fe 	bl	300070b0 <ymodem_uart_clean_rx>
30006eb4:	2c01      	cmp	r4, #1
30006eb6:	d111      	bne.n	30006edc <ymodem_get_others+0x134>
30006eb8:	60ac      	str	r4, [r5, #8]
30006eba:	2006      	movs	r0, #6
30006ebc:	4d0b      	ldr	r5, [pc, #44]	; (30006eec <ymodem_get_others+0x144>)
30006ebe:	f000 f8c5 	bl	3000704c <ymodem_uart_putc>
30006ec2:	682b      	ldr	r3, [r5, #0]
30006ec4:	b913      	cbnz	r3, 30006ecc <ymodem_get_others+0x124>
30006ec6:	2043      	movs	r0, #67	; 0x43
30006ec8:	f7ff ff0c 	bl	30006ce4 <ymodem_inquiry>
30006ecc:	682b      	ldr	r3, [r5, #0]
30006ece:	2e00      	cmp	r6, #0
30006ed0:	f103 0301 	add.w	r3, r3, #1
30006ed4:	bf18      	it	ne
30006ed6:	2404      	movne	r4, #4
30006ed8:	602b      	str	r3, [r5, #0]
30006eda:	e775      	b.n	30006dc8 <ymodem_get_others+0x20>
30006edc:	2c04      	cmp	r4, #4
30006ede:	f47f af73 	bne.w	30006dc8 <ymodem_get_others+0x20>
30006ee2:	e7b4      	b.n	30006e4e <ymodem_get_others+0xa6>
30006ee4:	000caa30 	.word	0x000caa30
30006ee8:	20016ffc 	.word	0x20016ffc
30006eec:	3000dd0c 	.word	0x3000dd0c

30006ef0 <ymodem_rx_frame>:
30006ef0:	2301      	movs	r3, #1
30006ef2:	b510      	push	{r4, lr}
30006ef4:	8083      	strh	r3, [r0, #4]
30006ef6:	2300      	movs	r3, #0
30006ef8:	4604      	mov	r4, r0
30006efa:	6083      	str	r3, [r0, #8]
30006efc:	f7ff ff02 	bl	30006d04 <ymodem_get_first>
30006f00:	68a3      	ldr	r3, [r4, #8]
30006f02:	b133      	cbz	r3, 30006f12 <ymodem_rx_frame+0x22>
30006f04:	2801      	cmp	r0, #1
30006f06:	bf01      	itttt	eq
30006f08:	6823      	ldreq	r3, [r4, #0]
30006f0a:	3301      	addeq	r3, #1
30006f0c:	b2db      	uxtbeq	r3, r3
30006f0e:	6023      	streq	r3, [r4, #0]
30006f10:	bd10      	pop	{r4, pc}
30006f12:	4620      	mov	r0, r4
30006f14:	f7ff ff48 	bl	30006da8 <ymodem_get_others>
30006f18:	e7f2      	b.n	30006f00 <ymodem_rx_frame+0x10>

30006f1a <ymodem_img_rxbuffer>:
30006f1a:	b570      	push	{r4, r5, r6, lr}
30006f1c:	2300      	movs	r3, #0
30006f1e:	4d1e      	ldr	r5, [pc, #120]	; (30006f98 <ymodem_img_rxbuffer+0x7e>)
30006f20:	4e1e      	ldr	r6, [pc, #120]	; (30006f9c <ymodem_img_rxbuffer+0x82>)
30006f22:	460c      	mov	r4, r1
30006f24:	60ab      	str	r3, [r5, #8]
30006f26:	616b      	str	r3, [r5, #20]
30006f28:	f000 f8c2 	bl	300070b0 <ymodem_uart_clean_rx>
30006f2c:	2043      	movs	r0, #67	; 0x43
30006f2e:	f000 f88d 	bl	3000704c <ymodem_uart_putc>
30006f32:	f24c 3050 	movw	r0, #50000	; 0xc350
30006f36:	47b0      	blx	r6
30006f38:	f000 f872 	bl	30007020 <ymodem_uart_readable>
30006f3c:	2800      	cmp	r0, #0
30006f3e:	d0f5      	beq.n	30006f2c <ymodem_img_rxbuffer+0x12>
30006f40:	2300      	movs	r3, #0
30006f42:	e9c5 3400 	strd	r3, r4, [r5]
30006f46:	4d16      	ldr	r5, [pc, #88]	; (30006fa0 <ymodem_img_rxbuffer+0x86>)
30006f48:	4621      	mov	r1, r4
30006f4a:	4628      	mov	r0, r5
30006f4c:	f7ff ffd0 	bl	30006ef0 <ymodem_rx_frame>
30006f50:	3801      	subs	r0, #1
30006f52:	2807      	cmp	r0, #7
30006f54:	d818      	bhi.n	30006f88 <ymodem_img_rxbuffer+0x6e>
30006f56:	a301      	add	r3, pc, #4	; (adr r3, 30006f5c <ymodem_img_rxbuffer+0x42>)
30006f58:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30006f5c:	30006f49 	.word	0x30006f49
30006f60:	30006f89 	.word	0x30006f89
30006f64:	30006f89 	.word	0x30006f89
30006f68:	30006f49 	.word	0x30006f49
30006f6c:	30006f8f 	.word	0x30006f8f
30006f70:	30006f89 	.word	0x30006f89
30006f74:	30006f89 	.word	0x30006f89
30006f78:	30006f7d 	.word	0x30006f7d
30006f7c:	f000 f898 	bl	300070b0 <ymodem_uart_clean_rx>
30006f80:	2015      	movs	r0, #21
30006f82:	f7ff feaf 	bl	30006ce4 <ymodem_inquiry>
30006f86:	e7df      	b.n	30006f48 <ymodem_img_rxbuffer+0x2e>
30006f88:	2018      	movs	r0, #24
30006f8a:	f7ff feab 	bl	30006ce4 <ymodem_inquiry>
30006f8e:	200a      	movs	r0, #10
30006f90:	f000 f898 	bl	300070c4 <ymodem_uart_waitbusy>
30006f94:	2001      	movs	r0, #1
30006f96:	bd70      	pop	{r4, r5, r6, pc}
30006f98:	3000dd0c 	.word	0x3000dd0c
30006f9c:	00009b2d 	.word	0x00009b2d
30006fa0:	3000dd14 	.word	0x3000dd14

30006fa4 <ymodem_log_open>:
30006fa4:	b510      	push	{r4, lr}
30006fa6:	2303      	movs	r3, #3
30006fa8:	4c04      	ldr	r4, [pc, #16]	; (30006fbc <ymodem_log_open+0x18>)
30006faa:	2200      	movs	r2, #0
30006fac:	2108      	movs	r1, #8
30006fae:	4804      	ldr	r0, [pc, #16]	; (30006fc0 <ymodem_log_open+0x1c>)
30006fb0:	47a0      	blx	r4
30006fb2:	2008      	movs	r0, #8
30006fb4:	4b03      	ldr	r3, [pc, #12]	; (30006fc4 <ymodem_log_open+0x20>)
30006fb6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30006fba:	4718      	bx	r3
30006fbc:	30008f55 	.word	0x30008f55
30006fc0:	0000ed39 	.word	0x0000ed39
30006fc4:	0000e1bd 	.word	0x0000e1bd

30006fc8 <ymodem_uart_port_init>:
30006fc8:	b507      	push	{r0, r1, r2, lr}
30006fca:	4b07      	ldr	r3, [pc, #28]	; (30006fe8 <ymodem_uart_port_init+0x20>)
30006fcc:	2008      	movs	r0, #8
30006fce:	9201      	str	r2, [sp, #4]
30006fd0:	4798      	blx	r3
30006fd2:	4b06      	ldr	r3, [pc, #24]	; (30006fec <ymodem_uart_port_init+0x24>)
30006fd4:	2008      	movs	r0, #8
30006fd6:	4798      	blx	r3
30006fd8:	9901      	ldr	r1, [sp, #4]
30006fda:	4805      	ldr	r0, [pc, #20]	; (30006ff0 <ymodem_uart_port_init+0x28>)
30006fdc:	4b05      	ldr	r3, [pc, #20]	; (30006ff4 <ymodem_uart_port_init+0x2c>)
30006fde:	b003      	add	sp, #12
30006fe0:	f85d eb04 	ldr.w	lr, [sp], #4
30006fe4:	4718      	bx	r3
30006fe6:	bf00      	nop
30006fe8:	0000e1d9 	.word	0x0000e1d9
30006fec:	0000e2fd 	.word	0x0000e2fd
30006ff0:	4200c000 	.word	0x4200c000
30006ff4:	0000a9e5 	.word	0x0000a9e5

30006ff8 <ymodem_uart_port_deinit>:
30006ff8:	2804      	cmp	r0, #4
30006ffa:	b508      	push	{r3, lr}
30006ffc:	d101      	bne.n	30007002 <ymodem_uart_port_deinit+0xa>
30006ffe:	f7ff ffd1 	bl	30006fa4 <ymodem_log_open>
30007002:	4b05      	ldr	r3, [pc, #20]	; (30007018 <ymodem_uart_port_deinit+0x20>)
30007004:	681b      	ldr	r3, [r3, #0]
30007006:	039b      	lsls	r3, r3, #14
30007008:	d504      	bpl.n	30007014 <ymodem_uart_port_deinit+0x1c>
3000700a:	4804      	ldr	r0, [pc, #16]	; (3000701c <ymodem_uart_port_deinit+0x24>)
3000700c:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
30007010:	f004 b98e 	b.w	3000b330 <__DiagPrintf_veneer>
30007014:	bd08      	pop	{r3, pc}
30007016:	bf00      	nop
30007018:	2001c00c 	.word	0x2001c00c
3000701c:	3000d2fa 	.word	0x3000d2fa

30007020 <ymodem_uart_readable>:
30007020:	4b00      	ldr	r3, [pc, #0]	; (30007024 <ymodem_uart_readable+0x4>)
30007022:	4718      	bx	r3
30007024:	0000ac39 	.word	0x0000ac39

30007028 <ymodem_uart_writable>:
30007028:	b508      	push	{r3, lr}
3000702a:	4b03      	ldr	r3, [pc, #12]	; (30007038 <ymodem_uart_writable+0x10>)
3000702c:	6818      	ldr	r0, [r3, #0]
3000702e:	4b03      	ldr	r3, [pc, #12]	; (3000703c <ymodem_uart_writable+0x14>)
30007030:	4798      	blx	r3
30007032:	b2c0      	uxtb	r0, r0
30007034:	bd08      	pop	{r3, pc}
30007036:	bf00      	nop
30007038:	3000dd24 	.word	0x3000dd24
3000703c:	0000d1a9 	.word	0x0000d1a9

30007040 <ymodem_uart_getc>:
30007040:	2000      	movs	r0, #0
30007042:	4b01      	ldr	r3, [pc, #4]	; (30007048 <ymodem_uart_getc+0x8>)
30007044:	4718      	bx	r3
30007046:	bf00      	nop
30007048:	0000ac1d 	.word	0x0000ac1d

3000704c <ymodem_uart_putc>:
3000704c:	4b00      	ldr	r3, [pc, #0]	; (30007050 <ymodem_uart_putc+0x4>)
3000704e:	4718      	bx	r3
30007050:	0000abb5 	.word	0x0000abb5

30007054 <ymodem_uart_putdata>:
30007054:	b570      	push	{r4, r5, r6, lr}
30007056:	4604      	mov	r4, r0
30007058:	1845      	adds	r5, r0, r1
3000705a:	4e04      	ldr	r6, [pc, #16]	; (3000706c <ymodem_uart_putdata+0x18>)
3000705c:	42ac      	cmp	r4, r5
3000705e:	d100      	bne.n	30007062 <ymodem_uart_putdata+0xe>
30007060:	bd70      	pop	{r4, r5, r6, pc}
30007062:	f814 0b01 	ldrb.w	r0, [r4], #1
30007066:	47b0      	blx	r6
30007068:	e7f8      	b.n	3000705c <ymodem_uart_putdata+0x8>
3000706a:	bf00      	nop
3000706c:	0000abb5 	.word	0x0000abb5

30007070 <ymodem_uart_getdata>:
30007070:	b570      	push	{r4, r5, r6, lr}
30007072:	4604      	mov	r4, r0
30007074:	1845      	adds	r5, r0, r1
30007076:	4e04      	ldr	r6, [pc, #16]	; (30007088 <ymodem_uart_getdata+0x18>)
30007078:	42ac      	cmp	r4, r5
3000707a:	d100      	bne.n	3000707e <ymodem_uart_getdata+0xe>
3000707c:	bd70      	pop	{r4, r5, r6, pc}
3000707e:	2001      	movs	r0, #1
30007080:	47b0      	blx	r6
30007082:	f804 0b01 	strb.w	r0, [r4], #1
30007086:	e7f7      	b.n	30007078 <ymodem_uart_getdata+0x8>
30007088:	0000ac1d 	.word	0x0000ac1d

3000708c <ymodem_uart_getc_to>:
3000708c:	b570      	push	{r4, r5, r6, lr}
3000708e:	4605      	mov	r5, r0
30007090:	460c      	mov	r4, r1
30007092:	4e06      	ldr	r6, [pc, #24]	; (300070ac <ymodem_uart_getc_to+0x20>)
30007094:	47b0      	blx	r6
30007096:	b120      	cbz	r0, 300070a2 <ymodem_uart_getc_to+0x16>
30007098:	f7ff ffd2 	bl	30007040 <ymodem_uart_getc>
3000709c:	7028      	strb	r0, [r5, #0]
3000709e:	2000      	movs	r0, #0
300070a0:	bd70      	pop	{r4, r5, r6, pc}
300070a2:	3c01      	subs	r4, #1
300070a4:	d1f6      	bne.n	30007094 <ymodem_uart_getc_to+0x8>
300070a6:	2001      	movs	r0, #1
300070a8:	e7fa      	b.n	300070a0 <ymodem_uart_getc_to+0x14>
300070aa:	bf00      	nop
300070ac:	0000ac39 	.word	0x0000ac39

300070b0 <ymodem_uart_clean_rx>:
300070b0:	b510      	push	{r4, lr}
300070b2:	4c03      	ldr	r4, [pc, #12]	; (300070c0 <ymodem_uart_clean_rx+0x10>)
300070b4:	47a0      	blx	r4
300070b6:	b900      	cbnz	r0, 300070ba <ymodem_uart_clean_rx+0xa>
300070b8:	bd10      	pop	{r4, pc}
300070ba:	f7ff ffc1 	bl	30007040 <ymodem_uart_getc>
300070be:	e7f9      	b.n	300070b4 <ymodem_uart_clean_rx+0x4>
300070c0:	0000ac39 	.word	0x0000ac39

300070c4 <ymodem_uart_waitbusy>:
300070c4:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
300070c6:	4604      	mov	r4, r0
300070c8:	2500      	movs	r5, #0
300070ca:	4f05      	ldr	r7, [pc, #20]	; (300070e0 <ymodem_uart_waitbusy+0x1c>)
300070cc:	4e05      	ldr	r6, [pc, #20]	; (300070e4 <ymodem_uart_waitbusy+0x20>)
300070ce:	697b      	ldr	r3, [r7, #20]
300070d0:	03db      	lsls	r3, r3, #15
300070d2:	d404      	bmi.n	300070de <ymodem_uart_waitbusy+0x1a>
300070d4:	3501      	adds	r5, #1
300070d6:	2064      	movs	r0, #100	; 0x64
300070d8:	47b0      	blx	r6
300070da:	42a5      	cmp	r5, r4
300070dc:	d9f7      	bls.n	300070ce <ymodem_uart_waitbusy+0xa>
300070de:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
300070e0:	4200c000 	.word	0x4200c000
300070e4:	00009b2d 	.word	0x00009b2d

300070e8 <UARTIMG_Write>:
300070e8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300070ec:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
300070f0:	4606      	mov	r6, r0
300070f2:	f1b5 6f00 	cmp.w	r5, #134217728	; 0x8000000
300070f6:	d229      	bcs.n	3000714c <UARTIMG_Write+0x64>
300070f8:	1e54      	subs	r4, r2, #1
300070fa:	4b15      	ldr	r3, [pc, #84]	; (30007150 <UARTIMG_Write+0x68>)
300070fc:	f8df 805c 	ldr.w	r8, [pc, #92]	; 3000715c <UARTIMG_Write+0x74>
30007100:	442c      	add	r4, r5
30007102:	ea05 0703 	and.w	r7, r5, r3
30007106:	401c      	ands	r4, r3
30007108:	f8d8 3000 	ldr.w	r3, [r8]
3000710c:	42bb      	cmp	r3, r7
3000710e:	d807      	bhi.n	30007120 <UARTIMG_Write+0x38>
30007110:	4639      	mov	r1, r7
30007112:	f507 5780 	add.w	r7, r7, #4096	; 0x1000
30007116:	2002      	movs	r0, #2
30007118:	4b0e      	ldr	r3, [pc, #56]	; (30007154 <UARTIMG_Write+0x6c>)
3000711a:	4798      	blx	r3
3000711c:	f8c8 7000 	str.w	r7, [r8]
30007120:	f8d8 3000 	ldr.w	r3, [r8]
30007124:	42a3      	cmp	r3, r4
30007126:	d807      	bhi.n	30007138 <UARTIMG_Write+0x50>
30007128:	4621      	mov	r1, r4
3000712a:	f504 5480 	add.w	r4, r4, #4096	; 0x1000
3000712e:	2002      	movs	r0, #2
30007130:	4b08      	ldr	r3, [pc, #32]	; (30007154 <UARTIMG_Write+0x6c>)
30007132:	4798      	blx	r3
30007134:	f8c8 4000 	str.w	r4, [r8]
30007138:	2400      	movs	r4, #0
3000713a:	4f07      	ldr	r7, [pc, #28]	; (30007158 <UARTIMG_Write+0x70>)
3000713c:	1932      	adds	r2, r6, r4
3000713e:	1928      	adds	r0, r5, r4
30007140:	3408      	adds	r4, #8
30007142:	2108      	movs	r1, #8
30007144:	47b8      	blx	r7
30007146:	f5b4 6f80 	cmp.w	r4, #1024	; 0x400
3000714a:	d1f7      	bne.n	3000713c <UARTIMG_Write+0x54>
3000714c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007150:	fffff000 	.word	0xfffff000
30007154:	0000a16d 	.word	0x0000a16d
30007158:	0000a04d 	.word	0x0000a04d
3000715c:	3000dd28 	.word	0x3000dd28

30007160 <ymodem_img_download>:
30007160:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007164:	2400      	movs	r4, #0
30007166:	f8df 802c 	ldr.w	r8, [pc, #44]	; 30007194 <ymodem_img_download+0x34>
3000716a:	460f      	mov	r7, r1
3000716c:	4616      	mov	r6, r2
3000716e:	2100      	movs	r1, #0
30007170:	4a08      	ldr	r2, [pc, #32]	; (30007194 <ymodem_img_download+0x34>)
30007172:	4605      	mov	r5, r0
30007174:	f7ff ff28 	bl	30006fc8 <ymodem_uart_port_init>
30007178:	42b4      	cmp	r4, r6
3000717a:	d104      	bne.n	30007186 <ymodem_img_download+0x26>
3000717c:	4628      	mov	r0, r5
3000717e:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007182:	f7ff bf39 	b.w	30006ff8 <ymodem_uart_port_deinit>
30007186:	f857 1024 	ldr.w	r1, [r7, r4, lsl #2]
3000718a:	4640      	mov	r0, r8
3000718c:	3401      	adds	r4, #1
3000718e:	f7ff fec4 	bl	30006f1a <ymodem_img_rxbuffer>
30007192:	e7f1      	b.n	30007178 <ymodem_img_download+0x18>
30007194:	0016e360 	.word	0x0016e360

30007198 <ChipInfo_Invalid>:
30007198:	4e06      	ldr	r6, [pc, #24]	; (300071b4 <ChipInfo_Invalid+0x1c>)
3000719a:	4d07      	ldr	r5, [pc, #28]	; (300071b8 <ChipInfo_Invalid+0x20>)
3000719c:	4c07      	ldr	r4, [pc, #28]	; (300071bc <ChipInfo_Invalid+0x24>)
3000719e:	b508      	push	{r3, lr}
300071a0:	4633      	mov	r3, r6
300071a2:	462a      	mov	r2, r5
300071a4:	4903      	ldr	r1, [pc, #12]	; (300071b4 <ChipInfo_Invalid+0x1c>)
300071a6:	2002      	movs	r0, #2
300071a8:	f003 fe7e 	bl	3000aea8 <rtk_log_write>
300071ac:	f242 7010 	movw	r0, #10000	; 0x2710
300071b0:	47a0      	blx	r4
300071b2:	e7f5      	b.n	300071a0 <ChipInfo_Invalid+0x8>
300071b4:	3000b4d0 	.word	0x3000b4d0
300071b8:	3000b4d9 	.word	0x3000b4d9
300071bc:	00009be5 	.word	0x00009be5

300071c0 <ChipInfo_Get>:
300071c0:	b570      	push	{r4, r5, r6, lr}
300071c2:	4c11      	ldr	r4, [pc, #68]	; (30007208 <ChipInfo_Get+0x48>)
300071c4:	7820      	ldrb	r0, [r4, #0]
300071c6:	28ff      	cmp	r0, #255	; 0xff
300071c8:	d104      	bne.n	300071d4 <ChipInfo_Get+0x14>
300071ca:	4d10      	ldr	r5, [pc, #64]	; (3000720c <ChipInfo_Get+0x4c>)
300071cc:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
300071d0:	b108      	cbz	r0, 300071d6 <ChipInfo_Get+0x16>
300071d2:	7020      	strb	r0, [r4, #0]
300071d4:	bd70      	pop	{r4, r5, r6, pc}
300071d6:	4621      	mov	r1, r4
300071d8:	f240 70ff 	movw	r0, #2047	; 0x7ff
300071dc:	f000 fdbc 	bl	30007d58 <OTP_Read8>
300071e0:	7820      	ldrb	r0, [r4, #0]
300071e2:	28ff      	cmp	r0, #255	; 0xff
300071e4:	d10c      	bne.n	30007200 <ChipInfo_Get+0x40>
300071e6:	4e0a      	ldr	r6, [pc, #40]	; (30007210 <ChipInfo_Get+0x50>)
300071e8:	4d0a      	ldr	r5, [pc, #40]	; (30007214 <ChipInfo_Get+0x54>)
300071ea:	4c0b      	ldr	r4, [pc, #44]	; (30007218 <ChipInfo_Get+0x58>)
300071ec:	4633      	mov	r3, r6
300071ee:	462a      	mov	r2, r5
300071f0:	4907      	ldr	r1, [pc, #28]	; (30007210 <ChipInfo_Get+0x50>)
300071f2:	2003      	movs	r0, #3
300071f4:	f003 fe58 	bl	3000aea8 <rtk_log_write>
300071f8:	f242 7010 	movw	r0, #10000	; 0x2710
300071fc:	47a0      	blx	r4
300071fe:	e7f5      	b.n	300071ec <ChipInfo_Get+0x2c>
30007200:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007204:	e7e6      	b.n	300071d4 <ChipInfo_Get+0x14>
30007206:	bf00      	nop
30007208:	3000de24 	.word	0x3000de24
3000720c:	23020000 	.word	0x23020000
30007210:	3000b4d0 	.word	0x3000b4d0
30007214:	3000b4fd 	.word	0x3000b4fd
30007218:	00009be5 	.word	0x00009be5

3000721c <ChipInfo_MemoryType>:
3000721c:	b510      	push	{r4, lr}
3000721e:	4c10      	ldr	r4, [pc, #64]	; (30007260 <ChipInfo_MemoryType+0x44>)
30007220:	47a0      	blx	r4
30007222:	2802      	cmp	r0, #2
30007224:	d01a      	beq.n	3000725c <ChipInfo_MemoryType+0x40>
30007226:	47a0      	blx	r4
30007228:	2803      	cmp	r0, #3
3000722a:	d017      	beq.n	3000725c <ChipInfo_MemoryType+0x40>
3000722c:	4b0d      	ldr	r3, [pc, #52]	; (30007264 <ChipInfo_MemoryType+0x48>)
3000722e:	4798      	blx	r3
30007230:	4b0d      	ldr	r3, [pc, #52]	; (30007268 <ChipInfo_MemoryType+0x4c>)
30007232:	2200      	movs	r2, #0
30007234:	4619      	mov	r1, r3
30007236:	789c      	ldrb	r4, [r3, #2]
30007238:	2cff      	cmp	r4, #255	; 0xff
3000723a:	d101      	bne.n	30007240 <ChipInfo_MemoryType+0x24>
3000723c:	f7ff ffac 	bl	30007198 <ChipInfo_Invalid>
30007240:	42a0      	cmp	r0, r4
30007242:	f103 0306 	add.w	r3, r3, #6
30007246:	d107      	bne.n	30007258 <ChipInfo_MemoryType+0x3c>
30007248:	2306      	movs	r3, #6
3000724a:	fb03 1202 	mla	r2, r3, r2, r1
3000724e:	78d3      	ldrb	r3, [r2, #3]
30007250:	2b02      	cmp	r3, #2
30007252:	d003      	beq.n	3000725c <ChipInfo_MemoryType+0x40>
30007254:	2001      	movs	r0, #1
30007256:	bd10      	pop	{r4, pc}
30007258:	3201      	adds	r2, #1
3000725a:	e7ec      	b.n	30007236 <ChipInfo_MemoryType+0x1a>
3000725c:	2002      	movs	r0, #2
3000725e:	e7fa      	b.n	30007256 <ChipInfo_MemoryType+0x3a>
30007260:	0000c0f9 	.word	0x0000c0f9
30007264:	300071c1 	.word	0x300071c1
30007268:	3000b526 	.word	0x3000b526

3000726c <ChipInfo_DDRType>:
3000726c:	b510      	push	{r4, lr}
3000726e:	4b0e      	ldr	r3, [pc, #56]	; (300072a8 <ChipInfo_DDRType+0x3c>)
30007270:	4798      	blx	r3
30007272:	4b0e      	ldr	r3, [pc, #56]	; (300072ac <ChipInfo_DDRType+0x40>)
30007274:	4604      	mov	r4, r0
30007276:	4798      	blx	r3
30007278:	2801      	cmp	r0, #1
3000727a:	d112      	bne.n	300072a2 <ChipInfo_DDRType+0x36>
3000727c:	4b0c      	ldr	r3, [pc, #48]	; (300072b0 <ChipInfo_DDRType+0x44>)
3000727e:	2200      	movs	r2, #0
30007280:	4619      	mov	r1, r3
30007282:	7898      	ldrb	r0, [r3, #2]
30007284:	28ff      	cmp	r0, #255	; 0xff
30007286:	d101      	bne.n	3000728c <ChipInfo_DDRType+0x20>
30007288:	2000      	movs	r0, #0
3000728a:	e007      	b.n	3000729c <ChipInfo_DDRType+0x30>
3000728c:	42a0      	cmp	r0, r4
3000728e:	f103 0306 	add.w	r3, r3, #6
30007292:	d104      	bne.n	3000729e <ChipInfo_DDRType+0x32>
30007294:	2306      	movs	r3, #6
30007296:	fb03 1202 	mla	r2, r3, r2, r1
3000729a:	78d0      	ldrb	r0, [r2, #3]
3000729c:	bd10      	pop	{r4, pc}
3000729e:	3201      	adds	r2, #1
300072a0:	e7ef      	b.n	30007282 <ChipInfo_DDRType+0x16>
300072a2:	20ff      	movs	r0, #255	; 0xff
300072a4:	e7fa      	b.n	3000729c <ChipInfo_DDRType+0x30>
300072a6:	bf00      	nop
300072a8:	300071c1 	.word	0x300071c1
300072ac:	3000721d 	.word	0x3000721d
300072b0:	3000b526 	.word	0x3000b526

300072b4 <ChipInfo_MemorySize>:
300072b4:	4b0a      	ldr	r3, [pc, #40]	; (300072e0 <ChipInfo_MemorySize+0x2c>)
300072b6:	b510      	push	{r4, lr}
300072b8:	4798      	blx	r3
300072ba:	4b0a      	ldr	r3, [pc, #40]	; (300072e4 <ChipInfo_MemorySize+0x30>)
300072bc:	2200      	movs	r2, #0
300072be:	4619      	mov	r1, r3
300072c0:	789c      	ldrb	r4, [r3, #2]
300072c2:	2cff      	cmp	r4, #255	; 0xff
300072c4:	d101      	bne.n	300072ca <ChipInfo_MemorySize+0x16>
300072c6:	f7ff ff67 	bl	30007198 <ChipInfo_Invalid>
300072ca:	4284      	cmp	r4, r0
300072cc:	f103 0306 	add.w	r3, r3, #6
300072d0:	d001      	beq.n	300072d6 <ChipInfo_MemorySize+0x22>
300072d2:	3201      	adds	r2, #1
300072d4:	e7f4      	b.n	300072c0 <ChipInfo_MemorySize+0xc>
300072d6:	2306      	movs	r3, #6
300072d8:	fb03 1202 	mla	r2, r3, r2, r1
300072dc:	7910      	ldrb	r0, [r2, #4]
300072de:	bd10      	pop	{r4, pc}
300072e0:	300071c1 	.word	0x300071c1
300072e4:	3000b526 	.word	0x3000b526

300072e8 <ChipInfo_MemoryVendor>:
300072e8:	b510      	push	{r4, lr}
300072ea:	4c0f      	ldr	r4, [pc, #60]	; (30007328 <ChipInfo_MemoryVendor+0x40>)
300072ec:	47a0      	blx	r4
300072ee:	2803      	cmp	r0, #3
300072f0:	d017      	beq.n	30007322 <ChipInfo_MemoryVendor+0x3a>
300072f2:	47a0      	blx	r4
300072f4:	2802      	cmp	r0, #2
300072f6:	d011      	beq.n	3000731c <ChipInfo_MemoryVendor+0x34>
300072f8:	4b0c      	ldr	r3, [pc, #48]	; (3000732c <ChipInfo_MemoryVendor+0x44>)
300072fa:	4798      	blx	r3
300072fc:	4b0c      	ldr	r3, [pc, #48]	; (30007330 <ChipInfo_MemoryVendor+0x48>)
300072fe:	2200      	movs	r2, #0
30007300:	4619      	mov	r1, r3
30007302:	789c      	ldrb	r4, [r3, #2]
30007304:	2cff      	cmp	r4, #255	; 0xff
30007306:	d101      	bne.n	3000730c <ChipInfo_MemoryVendor+0x24>
30007308:	f7ff ff46 	bl	30007198 <ChipInfo_Invalid>
3000730c:	4284      	cmp	r4, r0
3000730e:	f103 0306 	add.w	r3, r3, #6
30007312:	d104      	bne.n	3000731e <ChipInfo_MemoryVendor+0x36>
30007314:	2306      	movs	r3, #6
30007316:	fb03 1202 	mla	r2, r3, r2, r1
3000731a:	7950      	ldrb	r0, [r2, #5]
3000731c:	bd10      	pop	{r4, pc}
3000731e:	3201      	adds	r2, #1
30007320:	e7ef      	b.n	30007302 <ChipInfo_MemoryVendor+0x1a>
30007322:	2001      	movs	r0, #1
30007324:	e7fa      	b.n	3000731c <ChipInfo_MemoryVendor+0x34>
30007326:	bf00      	nop
30007328:	0000c0f9 	.word	0x0000c0f9
3000732c:	300071c1 	.word	0x300071c1
30007330:	3000b526 	.word	0x3000b526

30007334 <SDM32K_Enable>:
30007334:	4a03      	ldr	r2, [pc, #12]	; (30007344 <SDM32K_Enable+0x10>)
30007336:	6813      	ldr	r3, [r2, #0]
30007338:	f043 4340 	orr.w	r3, r3, #3221225472	; 0xc0000000
3000733c:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30007340:	6013      	str	r3, [r2, #0]
30007342:	4770      	bx	lr
30007344:	42008e00 	.word	0x42008e00

30007348 <CLK_SWITCH_XTAL>:
30007348:	4a0b      	ldr	r2, [pc, #44]	; (30007378 <CLK_SWITCH_XTAL+0x30>)
3000734a:	2801      	cmp	r0, #1
3000734c:	b508      	push	{r3, lr}
3000734e:	f8d2 3224 	ldr.w	r3, [r2, #548]	; 0x224
30007352:	bf0b      	itete	eq
30007354:	f023 4300 	biceq.w	r3, r3, #2147483648	; 0x80000000
30007358:	f043 4300 	orrne.w	r3, r3, #2147483648	; 0x80000000
3000735c:	f423 63a0 	biceq.w	r3, r3, #1280	; 0x500
30007360:	f443 63a0 	orrne.w	r3, r3, #1280	; 0x500
30007364:	f8c2 3224 	str.w	r3, [r2, #548]	; 0x224
30007368:	4b04      	ldr	r3, [pc, #16]	; (3000737c <CLK_SWITCH_XTAL+0x34>)
3000736a:	4798      	blx	r3
3000736c:	4b04      	ldr	r3, [pc, #16]	; (30007380 <CLK_SWITCH_XTAL+0x38>)
3000736e:	fbb0 f0f3 	udiv	r0, r0, r3
30007372:	4b04      	ldr	r3, [pc, #16]	; (30007384 <CLK_SWITCH_XTAL+0x3c>)
30007374:	6018      	str	r0, [r3, #0]
30007376:	bd08      	pop	{r3, pc}
30007378:	42008000 	.word	0x42008000
3000737c:	30008be9 	.word	0x30008be9
30007380:	000f4240 	.word	0x000f4240
30007384:	2001c700 	.word	0x2001c700

30007388 <DDR_PHY_ChipInfo>:
30007388:	b510      	push	{r4, lr}
3000738a:	4c1a      	ldr	r4, [pc, #104]	; (300073f4 <DDR_PHY_ChipInfo+0x6c>)
3000738c:	7820      	ldrb	r0, [r4, #0]
3000738e:	28ff      	cmp	r0, #255	; 0xff
30007390:	d116      	bne.n	300073c0 <DDR_PHY_ChipInfo+0x38>
30007392:	4b19      	ldr	r3, [pc, #100]	; (300073f8 <DDR_PHY_ChipInfo+0x70>)
30007394:	4798      	blx	r3
30007396:	280e      	cmp	r0, #14
30007398:	d80d      	bhi.n	300073b6 <DDR_PHY_ChipInfo+0x2e>
3000739a:	2801      	cmp	r0, #1
3000739c:	d921      	bls.n	300073e2 <DDR_PHY_ChipInfo+0x5a>
3000739e:	3802      	subs	r0, #2
300073a0:	280c      	cmp	r0, #12
300073a2:	d81e      	bhi.n	300073e2 <DDR_PHY_ChipInfo+0x5a>
300073a4:	e8df f000 	tbb	[pc, r0]
300073a8:	111d0f0d 	.word	0x111d0f0d
300073ac:	131d171d 	.word	0x131d171d
300073b0:	191d1d15 	.word	0x191d1d15
300073b4:	1b          	.byte	0x1b
300073b5:	00          	.byte	0x00
300073b6:	28e0      	cmp	r0, #224	; 0xe0
300073b8:	d113      	bne.n	300073e2 <DDR_PHY_ChipInfo+0x5a>
300073ba:	2301      	movs	r3, #1
300073bc:	7023      	strb	r3, [r4, #0]
300073be:	7820      	ldrb	r0, [r4, #0]
300073c0:	bd10      	pop	{r4, pc}
300073c2:	2302      	movs	r3, #2
300073c4:	e7fa      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073c6:	2303      	movs	r3, #3
300073c8:	e7f8      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073ca:	2304      	movs	r3, #4
300073cc:	e7f6      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073ce:	2305      	movs	r3, #5
300073d0:	e7f4      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073d2:	2306      	movs	r3, #6
300073d4:	e7f2      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073d6:	2307      	movs	r3, #7
300073d8:	e7f0      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073da:	2308      	movs	r3, #8
300073dc:	e7ee      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073de:	2309      	movs	r3, #9
300073e0:	e7ec      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073e2:	4b06      	ldr	r3, [pc, #24]	; (300073fc <DDR_PHY_ChipInfo+0x74>)
300073e4:	2002      	movs	r0, #2
300073e6:	4a06      	ldr	r2, [pc, #24]	; (30007400 <DDR_PHY_ChipInfo+0x78>)
300073e8:	4619      	mov	r1, r3
300073ea:	f003 fd5d 	bl	3000aea8 <rtk_log_write>
300073ee:	2300      	movs	r3, #0
300073f0:	e7e4      	b.n	300073bc <DDR_PHY_ChipInfo+0x34>
300073f2:	bf00      	nop
300073f4:	3000de25 	.word	0x3000de25
300073f8:	300071c1 	.word	0x300071c1
300073fc:	3000b5aa 	.word	0x3000b5aa
30007400:	3000b5b1 	.word	0x3000b5b1

30007404 <DDR_PHY_ChipInfo_ddrtype>:
30007404:	b510      	push	{r4, lr}
30007406:	4c04      	ldr	r4, [pc, #16]	; (30007418 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007408:	7823      	ldrb	r3, [r4, #0]
3000740a:	b913      	cbnz	r3, 30007412 <DDR_PHY_ChipInfo_ddrtype+0xe>
3000740c:	4b03      	ldr	r3, [pc, #12]	; (3000741c <DDR_PHY_ChipInfo_ddrtype+0x18>)
3000740e:	4798      	blx	r3
30007410:	7020      	strb	r0, [r4, #0]
30007412:	7820      	ldrb	r0, [r4, #0]
30007414:	bd10      	pop	{r4, pc}
30007416:	bf00      	nop
30007418:	3000eb40 	.word	0x3000eb40
3000741c:	3000726d 	.word	0x3000726d

30007420 <DDR_PHY_StructInit>:
30007420:	f240 2315 	movw	r3, #533	; 0x215
30007424:	6583      	str	r3, [r0, #88]	; 0x58
30007426:	4770      	bx	lr

30007428 <DDR_PHY_DLL_CLK_DIV>:
30007428:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
3000742a:	2328      	movs	r3, #40	; 0x28
3000742c:	4a3f      	ldr	r2, [pc, #252]	; (3000752c <DDR_PHY_DLL_CLK_DIV+0x104>)
3000742e:	4c40      	ldr	r4, [pc, #256]	; (30007530 <DDR_PHY_DLL_CLK_DIV+0x108>)
30007430:	4402      	add	r2, r0
30007432:	4d40      	ldr	r5, [pc, #256]	; (30007534 <DDR_PHY_DLL_CLK_DIV+0x10c>)
30007434:	4f40      	ldr	r7, [pc, #256]	; (30007538 <DDR_PHY_DLL_CLK_DIV+0x110>)
30007436:	fbb0 f1f3 	udiv	r1, r0, r3
3000743a:	3903      	subs	r1, #3
3000743c:	6a60      	ldr	r0, [r4, #36]	; 0x24
3000743e:	fb03 2211 	mls	r2, r3, r1, r2
30007442:	f020 60ff 	bic.w	r0, r0, #133693440	; 0x7f80000
30007446:	02d2      	lsls	r2, r2, #11
30007448:	f420 20e0 	bic.w	r0, r0, #458752	; 0x70000
3000744c:	fbb2 f2f3 	udiv	r2, r2, r3
30007450:	4b3a      	ldr	r3, [pc, #232]	; (3000753c <DDR_PHY_DLL_CLK_DIV+0x114>)
30007452:	ea03 4302 	and.w	r3, r3, r2, lsl #16
30007456:	f3c2 020a 	ubfx	r2, r2, #0, #11
3000745a:	4303      	orrs	r3, r0
3000745c:	6263      	str	r3, [r4, #36]	; 0x24
3000745e:	020b      	lsls	r3, r1, #8
30007460:	6aa0      	ldr	r0, [r4, #40]	; 0x28
30007462:	b2c9      	uxtb	r1, r1
30007464:	b29b      	uxth	r3, r3
30007466:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
3000746a:	4303      	orrs	r3, r0
3000746c:	62a3      	str	r3, [r4, #40]	; 0x28
3000746e:	6a63      	ldr	r3, [r4, #36]	; 0x24
30007470:	f423 63ff 	bic.w	r3, r3, #2040	; 0x7f8
30007474:	f023 0307 	bic.w	r3, r3, #7
30007478:	4313      	orrs	r3, r2
3000747a:	6263      	str	r3, [r4, #36]	; 0x24
3000747c:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000747e:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30007482:	4319      	orrs	r1, r3
30007484:	2301      	movs	r3, #1
30007486:	62a1      	str	r1, [r4, #40]	; 0x28
30007488:	67a3      	str	r3, [r4, #120]	; 0x78
3000748a:	f7ff ff7d 	bl	30007388 <DDR_PHY_ChipInfo>
3000748e:	0146      	lsls	r6, r0, #5
30007490:	eb05 1040 	add.w	r0, r5, r0, lsl #5
30007494:	59ab      	ldr	r3, [r5, r6]
30007496:	4435      	add	r5, r6
30007498:	6123      	str	r3, [r4, #16]
3000749a:	6843      	ldr	r3, [r0, #4]
3000749c:	6163      	str	r3, [r4, #20]
3000749e:	6883      	ldr	r3, [r0, #8]
300074a0:	f8c4 3098 	str.w	r3, [r4, #152]	; 0x98
300074a4:	68c3      	ldr	r3, [r0, #12]
300074a6:	f8c4 30a0 	str.w	r3, [r4, #160]	; 0xa0
300074aa:	6903      	ldr	r3, [r0, #16]
300074ac:	60a3      	str	r3, [r4, #8]
300074ae:	f8d4 309c 	ldr.w	r3, [r4, #156]	; 0x9c
300074b2:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
300074b6:	f8c4 309c 	str.w	r3, [r4, #156]	; 0x9c
300074ba:	47b8      	blx	r7
300074bc:	2801      	cmp	r0, #1
300074be:	bf0c      	ite	eq
300074c0:	2333      	moveq	r3, #51	; 0x33
300074c2:	2322      	movne	r3, #34	; 0x22
300074c4:	f8c4 3088 	str.w	r3, [r4, #136]	; 0x88
300074c8:	696b      	ldr	r3, [r5, #20]
300074ca:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
300074ce:	69ab      	ldr	r3, [r5, #24]
300074d0:	f8c4 3084 	str.w	r3, [r4, #132]	; 0x84
300074d4:	69eb      	ldr	r3, [r5, #28]
300074d6:	f8c4 3094 	str.w	r3, [r4, #148]	; 0x94
300074da:	f8d4 3084 	ldr.w	r3, [r4, #132]	; 0x84
300074de:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
300074e2:	f8c4 3084 	str.w	r3, [r4, #132]	; 0x84
300074e6:	4b16      	ldr	r3, [pc, #88]	; (30007540 <DDR_PHY_DLL_CLK_DIV+0x118>)
300074e8:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
300074ec:	009b      	lsls	r3, r3, #2
300074ee:	d517      	bpl.n	30007520 <DDR_PHY_DLL_CLK_DIV+0xf8>
300074f0:	47b8      	blx	r7
300074f2:	2801      	cmp	r0, #1
300074f4:	bf0c      	ite	eq
300074f6:	4b13      	ldreq	r3, [pc, #76]	; (30007544 <DDR_PHY_DLL_CLK_DIV+0x11c>)
300074f8:	4b13      	ldrne	r3, [pc, #76]	; (30007548 <DDR_PHY_DLL_CLK_DIV+0x120>)
300074fa:	6023      	str	r3, [r4, #0]
300074fc:	4b0c      	ldr	r3, [pc, #48]	; (30007530 <DDR_PHY_DLL_CLK_DIV+0x108>)
300074fe:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30007502:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
30007506:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
3000750a:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000750e:	699a      	ldr	r2, [r3, #24]
30007510:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
30007514:	619a      	str	r2, [r3, #24]
30007516:	699a      	ldr	r2, [r3, #24]
30007518:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000751c:	619a      	str	r2, [r3, #24]
3000751e:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007520:	47b8      	blx	r7
30007522:	2801      	cmp	r0, #1
30007524:	bf0c      	ite	eq
30007526:	4b09      	ldreq	r3, [pc, #36]	; (3000754c <DDR_PHY_DLL_CLK_DIV+0x124>)
30007528:	4b09      	ldrne	r3, [pc, #36]	; (30007550 <DDR_PHY_DLL_CLK_DIV+0x128>)
3000752a:	e7e6      	b.n	300074fa <DDR_PHY_DLL_CLK_DIV+0xd2>
3000752c:	001fff88 	.word	0x001fff88
30007530:	41011000 	.word	0x41011000
30007534:	3000b5d4 	.word	0x3000b5d4
30007538:	30007405 	.word	0x30007405
3000753c:	07ff0000 	.word	0x07ff0000
30007540:	42008000 	.word	0x42008000
30007544:	4c000903 	.word	0x4c000903
30007548:	4c000821 	.word	0x4c000821
3000754c:	40000103 	.word	0x40000103
30007550:	40000021 	.word	0x40000021

30007554 <DDR_PHY_CRT_Init>:
30007554:	4a2f      	ldr	r2, [pc, #188]	; (30007614 <DDR_PHY_CRT_Init+0xc0>)
30007556:	2005      	movs	r0, #5
30007558:	b510      	push	{r4, lr}
3000755a:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
3000755e:	4c2e      	ldr	r4, [pc, #184]	; (30007618 <DDR_PHY_CRT_Init+0xc4>)
30007560:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007564:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30007568:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000756c:	f043 0301 	orr.w	r3, r3, #1
30007570:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007574:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007578:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000757c:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007580:	68e3      	ldr	r3, [r4, #12]
30007582:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30007586:	60e3      	str	r3, [r4, #12]
30007588:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000758c:	f043 0302 	orr.w	r3, r3, #2
30007590:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30007594:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30007598:	f023 0308 	bic.w	r3, r3, #8
3000759c:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300075a0:	4b1e      	ldr	r3, [pc, #120]	; (3000761c <DDR_PHY_CRT_Init+0xc8>)
300075a2:	4798      	blx	r3
300075a4:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300075a8:	f043 0304 	orr.w	r3, r3, #4
300075ac:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300075b0:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300075b4:	f043 0308 	orr.w	r3, r3, #8
300075b8:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300075bc:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300075be:	f043 6380 	orr.w	r3, r3, #67108864	; 0x4000000
300075c2:	62a3      	str	r3, [r4, #40]	; 0x28
300075c4:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300075c6:	f023 6380 	bic.w	r3, r3, #67108864	; 0x4000000
300075ca:	62a3      	str	r3, [r4, #40]	; 0x28
300075cc:	69e3      	ldr	r3, [r4, #28]
300075ce:	f043 0308 	orr.w	r3, r3, #8
300075d2:	61e3      	str	r3, [r4, #28]
300075d4:	68a3      	ldr	r3, [r4, #8]
300075d6:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
300075da:	60a3      	str	r3, [r4, #8]
300075dc:	6863      	ldr	r3, [r4, #4]
300075de:	ea6f 3353 	mvn.w	r3, r3, lsr #13
300075e2:	ea6f 3343 	mvn.w	r3, r3, lsl #13
300075e6:	6063      	str	r3, [r4, #4]
300075e8:	68a3      	ldr	r3, [r4, #8]
300075ea:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
300075ee:	60a3      	str	r3, [r4, #8]
300075f0:	4b0b      	ldr	r3, [pc, #44]	; (30007620 <DDR_PHY_CRT_Init+0xcc>)
300075f2:	f8c4 3240 	str.w	r3, [r4, #576]	; 0x240
300075f6:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300075fa:	f023 0303 	bic.w	r3, r3, #3
300075fe:	f043 0302 	orr.w	r3, r3, #2
30007602:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007606:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000760a:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000760e:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30007612:	bd10      	pop	{r4, pc}
30007614:	42008000 	.word	0x42008000
30007618:	41011000 	.word	0x41011000
3000761c:	00009b2d 	.word	0x00009b2d
30007620:	90161f00 	.word	0x90161f00

30007624 <DDR_PHY_R240_ZQ_CAL>:
30007624:	b538      	push	{r3, r4, r5, lr}
30007626:	f7ff feaf 	bl	30007388 <DDR_PHY_ChipInfo>
3000762a:	2214      	movs	r2, #20
3000762c:	4d18      	ldr	r5, [pc, #96]	; (30007690 <DDR_PHY_R240_ZQ_CAL+0x6c>)
3000762e:	4350      	muls	r0, r2
30007630:	4b18      	ldr	r3, [pc, #96]	; (30007694 <DDR_PHY_R240_ZQ_CAL+0x70>)
30007632:	1829      	adds	r1, r5, r0
30007634:	f8d3 414c 	ldr.w	r4, [r3, #332]	; 0x14c
30007638:	7c0a      	ldrb	r2, [r1, #16]
3000763a:	f024 043e 	bic.w	r4, r4, #62	; 0x3e
3000763e:	0052      	lsls	r2, r2, #1
30007640:	f002 023e 	and.w	r2, r2, #62	; 0x3e
30007644:	4322      	orrs	r2, r4
30007646:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
3000764a:	582a      	ldr	r2, [r5, r0]
3000764c:	6848      	ldr	r0, [r1, #4]
3000764e:	f8c3 2180 	str.w	r2, [r3, #384]	; 0x180
30007652:	2200      	movs	r2, #0
30007654:	f8c3 2184 	str.w	r2, [r3, #388]	; 0x184
30007658:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
3000765c:	f8c3 218c 	str.w	r2, [r3, #396]	; 0x18c
30007660:	688a      	ldr	r2, [r1, #8]
30007662:	68c9      	ldr	r1, [r1, #12]
30007664:	f8c3 2190 	str.w	r2, [r3, #400]	; 0x190
30007668:	4a0b      	ldr	r2, [pc, #44]	; (30007698 <DDR_PHY_R240_ZQ_CAL+0x74>)
3000766a:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
3000766e:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
30007672:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30007676:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000767a:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
3000767e:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30007682:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30007686:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
3000768a:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000768e:	bd38      	pop	{r3, r4, r5, pc}
30007690:	3000b714 	.word	0x3000b714
30007694:	41011000 	.word	0x41011000
30007698:	00262626 	.word	0x00262626

3000769c <DDR_PHY_ZQ_SET_SEL>:
3000769c:	4b23      	ldr	r3, [pc, #140]	; (3000772c <DDR_PHY_ZQ_SET_SEL+0x90>)
3000769e:	4a24      	ldr	r2, [pc, #144]	; (30007730 <DDR_PHY_ZQ_SET_SEL+0x94>)
300076a0:	4924      	ldr	r1, [pc, #144]	; (30007734 <DDR_PHY_ZQ_SET_SEL+0x98>)
300076a2:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
300076a6:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
300076aa:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
300076ae:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
300076b2:	f8c3 11d0 	str.w	r1, [r3, #464]	; 0x1d0
300076b6:	f8c3 11d4 	str.w	r1, [r3, #468]	; 0x1d4
300076ba:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
300076be:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
300076c2:	f04f 3211 	mov.w	r2, #286331153	; 0x11111111
300076c6:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
300076ca:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
300076ce:	4a1a      	ldr	r2, [pc, #104]	; (30007738 <DDR_PHY_ZQ_SET_SEL+0x9c>)
300076d0:	f8c3 21e8 	str.w	r2, [r3, #488]	; 0x1e8
300076d4:	f8c3 21ec 	str.w	r2, [r3, #492]	; 0x1ec
300076d8:	2200      	movs	r2, #0
300076da:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
300076de:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
300076e2:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
300076e6:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
300076ea:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
300076ee:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
300076f2:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
300076f6:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
300076fa:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
300076fe:	f64f 72ff 	movw	r2, #65535	; 0xffff
30007702:	f8c3 2224 	str.w	r2, [r3, #548]	; 0x224
30007706:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
3000770a:	f8c3 2228 	str.w	r2, [r3, #552]	; 0x228
3000770e:	f8c3 222c 	str.w	r2, [r3, #556]	; 0x22c
30007712:	f8c3 2230 	str.w	r2, [r3, #560]	; 0x230
30007716:	f8c3 2234 	str.w	r2, [r3, #564]	; 0x234
3000771a:	f8c3 2328 	str.w	r2, [r3, #808]	; 0x328
3000771e:	f8c3 232c 	str.w	r2, [r3, #812]	; 0x32c
30007722:	f8c3 2330 	str.w	r2, [r3, #816]	; 0x330
30007726:	f8c3 2334 	str.w	r2, [r3, #820]	; 0x334
3000772a:	4770      	bx	lr
3000772c:	41011000 	.word	0x41011000
30007730:	ffff1111 	.word	0xffff1111
30007734:	ffff3333 	.word	0xffff3333
30007738:	ffff0000 	.word	0xffff0000

3000773c <DDR_PHY_DQ_DQS_SEL_EMPH>:
3000773c:	4b09      	ldr	r3, [pc, #36]	; (30007764 <DDR_PHY_DQ_DQS_SEL_EMPH+0x28>)
3000773e:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30007742:	f8c3 24f0 	str.w	r2, [r3, #1264]	; 0x4f0
30007746:	f8c3 24f4 	str.w	r2, [r3, #1268]	; 0x4f4
3000774a:	2200      	movs	r2, #0
3000774c:	f8c3 24e8 	str.w	r2, [r3, #1256]	; 0x4e8
30007750:	f8c3 24ec 	str.w	r2, [r3, #1260]	; 0x4ec
30007754:	4a04      	ldr	r2, [pc, #16]	; (30007768 <DDR_PHY_DQ_DQS_SEL_EMPH+0x2c>)
30007756:	f8c3 217c 	str.w	r2, [r3, #380]	; 0x17c
3000775a:	f44f 5280 	mov.w	r2, #4096	; 0x1000
3000775e:	f8c3 22a4 	str.w	r2, [r3, #676]	; 0x2a4
30007762:	4770      	bx	lr
30007764:	41011000 	.word	0x41011000
30007768:	00045500 	.word	0x00045500

3000776c <DDR_PHY_DELAY_TAP_SET>:
3000776c:	b538      	push	{r3, r4, r5, lr}
3000776e:	4c10      	ldr	r4, [pc, #64]	; (300077b0 <DDR_PHY_DELAY_TAP_SET+0x44>)
30007770:	f04f 3588 	mov.w	r5, #2290649224	; 0x88888888
30007774:	4b0f      	ldr	r3, [pc, #60]	; (300077b4 <DDR_PHY_DELAY_TAP_SET+0x48>)
30007776:	f8c4 5100 	str.w	r5, [r4, #256]	; 0x100
3000777a:	f8c4 5104 	str.w	r5, [r4, #260]	; 0x104
3000777e:	f8c4 5108 	str.w	r5, [r4, #264]	; 0x108
30007782:	f8c4 310c 	str.w	r3, [r4, #268]	; 0x10c
30007786:	4b0c      	ldr	r3, [pc, #48]	; (300077b8 <DDR_PHY_DELAY_TAP_SET+0x4c>)
30007788:	f8c4 5110 	str.w	r5, [r4, #272]	; 0x110
3000778c:	4798      	blx	r3
3000778e:	2801      	cmp	r0, #1
30007790:	bf0c      	ite	eq
30007792:	4b0a      	ldreq	r3, [pc, #40]	; (300077bc <DDR_PHY_DELAY_TAP_SET+0x50>)
30007794:	4b0a      	ldrne	r3, [pc, #40]	; (300077c0 <DDR_PHY_DELAY_TAP_SET+0x54>)
30007796:	f8c4 3154 	str.w	r3, [r4, #340]	; 0x154
3000779a:	f640 0388 	movw	r3, #2184	; 0x888
3000779e:	f8c4 5480 	str.w	r5, [r4, #1152]	; 0x480
300077a2:	f8c4 5484 	str.w	r5, [r4, #1156]	; 0x484
300077a6:	f8c4 3488 	str.w	r3, [r4, #1160]	; 0x488
300077aa:	f8c4 348c 	str.w	r3, [r4, #1164]	; 0x48c
300077ae:	bd38      	pop	{r3, r4, r5, pc}
300077b0:	41011000 	.word	0x41011000
300077b4:	20888888 	.word	0x20888888
300077b8:	30007405 	.word	0x30007405
300077bc:	7c033333 	.word	0x7c033333
300077c0:	7c011111 	.word	0x7c011111

300077c4 <DDR_PHY_READ_CTRL>:
300077c4:	b508      	push	{r3, lr}
300077c6:	f7ff fddf 	bl	30007388 <DDR_PHY_ChipInfo>
300077ca:	4b0c      	ldr	r3, [pc, #48]	; (300077fc <DDR_PHY_READ_CTRL+0x38>)
300077cc:	2214      	movs	r2, #20
300077ce:	fb02 3000 	mla	r0, r2, r0, r3
300077d2:	4b0b      	ldr	r3, [pc, #44]	; (30007800 <DDR_PHY_READ_CTRL+0x3c>)
300077d4:	7c42      	ldrb	r2, [r0, #17]
300077d6:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
300077da:	7c82      	ldrb	r2, [r0, #18]
300077dc:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
300077e0:	7cc2      	ldrb	r2, [r0, #19]
300077e2:	f8c3 2170 	str.w	r2, [r3, #368]	; 0x170
300077e6:	2208      	movs	r2, #8
300077e8:	f8c3 2408 	str.w	r2, [r3, #1032]	; 0x408
300077ec:	f8c3 240c 	str.w	r2, [r3, #1036]	; 0x40c
300077f0:	22fe      	movs	r2, #254	; 0xfe
300077f2:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
300077f6:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
300077fa:	bd08      	pop	{r3, pc}
300077fc:	3000b714 	.word	0x3000b714
30007800:	41011000 	.word	0x41011000

30007804 <DDR_PHY_3_POINT_CAL_RANGE>:
30007804:	4b07      	ldr	r3, [pc, #28]	; (30007824 <DDR_PHY_3_POINT_CAL_RANGE+0x20>)
30007806:	4a08      	ldr	r2, [pc, #32]	; (30007828 <DDR_PHY_3_POINT_CAL_RANGE+0x24>)
30007808:	f8c3 2178 	str.w	r2, [r3, #376]	; 0x178
3000780c:	f04f 3222 	mov.w	r2, #572662306	; 0x22222222
30007810:	f8c3 2420 	str.w	r2, [r3, #1056]	; 0x420
30007814:	f8c3 2424 	str.w	r2, [r3, #1060]	; 0x424
30007818:	f8c3 2430 	str.w	r2, [r3, #1072]	; 0x430
3000781c:	f8c3 2434 	str.w	r2, [r3, #1076]	; 0x434
30007820:	4770      	bx	lr
30007822:	bf00      	nop
30007824:	41011000 	.word	0x41011000
30007828:	11117777 	.word	0x11117777

3000782c <DDR_PHY_READ_LEVELING>:
3000782c:	b508      	push	{r3, lr}
3000782e:	f7ff fdab 	bl	30007388 <DDR_PHY_ChipInfo>
30007832:	4b1a      	ldr	r3, [pc, #104]	; (3000789c <DDR_PHY_READ_LEVELING+0x70>)
30007834:	b1e8      	cbz	r0, 30007872 <DDR_PHY_READ_LEVELING+0x46>
30007836:	f04f 3212 	mov.w	r2, #303174162	; 0x12121212
3000783a:	f04f 3111 	mov.w	r1, #286331153	; 0x11111111
3000783e:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30007842:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30007846:	f8c3 1444 	str.w	r1, [r3, #1092]	; 0x444
3000784a:	f8c3 144c 	str.w	r1, [r3, #1100]	; 0x44c
3000784e:	f8c3 1458 	str.w	r1, [r3, #1112]	; 0x458
30007852:	f8c3 1460 	str.w	r1, [r3, #1120]	; 0x460
30007856:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
3000785a:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
3000785e:	220f      	movs	r2, #15
30007860:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
30007864:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30007868:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
3000786c:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
30007870:	bd08      	pop	{r3, pc}
30007872:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
30007876:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
3000787a:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
3000787e:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30007882:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30007886:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
3000788a:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
3000788e:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30007892:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30007896:	2210      	movs	r2, #16
30007898:	e7e2      	b.n	30007860 <DDR_PHY_READ_LEVELING+0x34>
3000789a:	bf00      	nop
3000789c:	41011000 	.word	0x41011000

300078a0 <DDR_PHY_Init>:
300078a0:	b508      	push	{r3, lr}
300078a2:	6d80      	ldr	r0, [r0, #88]	; 0x58
300078a4:	f7ff fdc0 	bl	30007428 <DDR_PHY_DLL_CLK_DIV>
300078a8:	f7ff fe54 	bl	30007554 <DDR_PHY_CRT_Init>
300078ac:	f7ff feba 	bl	30007624 <DDR_PHY_R240_ZQ_CAL>
300078b0:	f7ff fef4 	bl	3000769c <DDR_PHY_ZQ_SET_SEL>
300078b4:	f7ff ff42 	bl	3000773c <DDR_PHY_DQ_DQS_SEL_EMPH>
300078b8:	f7ff ff58 	bl	3000776c <DDR_PHY_DELAY_TAP_SET>
300078bc:	f7ff ff82 	bl	300077c4 <DDR_PHY_READ_CTRL>
300078c0:	f7ff ffa0 	bl	30007804 <DDR_PHY_3_POINT_CAL_RANGE>
300078c4:	f7ff ffb2 	bl	3000782c <DDR_PHY_READ_LEVELING>
300078c8:	4b18      	ldr	r3, [pc, #96]	; (3000792c <DDR_PHY_Init+0x8c>)
300078ca:	f8d3 21f8 	ldr.w	r2, [r3, #504]	; 0x1f8
300078ce:	f422 427e 	bic.w	r2, r2, #65024	; 0xfe00
300078d2:	f442 4264 	orr.w	r2, r2, #58368	; 0xe400
300078d6:	f8c3 21f8 	str.w	r2, [r3, #504]	; 0x1f8
300078da:	f04f 32d8 	mov.w	r2, #3638089944	; 0xd8d8d8d8
300078de:	f8c3 21fc 	str.w	r2, [r3, #508]	; 0x1fc
300078e2:	f8d3 22d0 	ldr.w	r2, [r3, #720]	; 0x2d0
300078e6:	f042 022f 	orr.w	r2, r2, #47	; 0x2f
300078ea:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
300078ee:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
300078f2:	f8c3 21fc 	str.w	r2, [r3, #508]	; 0x1fc
300078f6:	f8d3 223c 	ldr.w	r2, [r3, #572]	; 0x23c
300078fa:	f042 0203 	orr.w	r2, r2, #3
300078fe:	f8c3 223c 	str.w	r2, [r3, #572]	; 0x23c
30007902:	f8d3 223c 	ldr.w	r2, [r3, #572]	; 0x23c
30007906:	f042 020c 	orr.w	r2, r2, #12
3000790a:	f8c3 223c 	str.w	r2, [r3, #572]	; 0x23c
3000790e:	f8d3 2238 	ldr.w	r2, [r3, #568]	; 0x238
30007912:	f022 0203 	bic.w	r2, r2, #3
30007916:	f8c3 2238 	str.w	r2, [r3, #568]	; 0x238
3000791a:	f8d3 2238 	ldr.w	r2, [r3, #568]	; 0x238
3000791e:	f422 527c 	bic.w	r2, r2, #16128	; 0x3f00
30007922:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
30007926:	f8c3 2238 	str.w	r2, [r3, #568]	; 0x238
3000792a:	bd08      	pop	{r3, pc}
3000792c:	41011000 	.word	0x41011000

30007930 <DDR_PHY_CAL>:
30007930:	4770      	bx	lr
	...

30007934 <DDR_PHY_AutoGating>:
30007934:	4a0c      	ldr	r2, [pc, #48]	; (30007968 <DDR_PHY_AutoGating+0x34>)
30007936:	2032      	movs	r0, #50	; 0x32
30007938:	b508      	push	{r3, lr}
3000793a:	6893      	ldr	r3, [r2, #8]
3000793c:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30007940:	6093      	str	r3, [r2, #8]
30007942:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30007946:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
3000794a:	f043 0314 	orr.w	r3, r3, #20
3000794e:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30007952:	4b06      	ldr	r3, [pc, #24]	; (3000796c <DDR_PHY_AutoGating+0x38>)
30007954:	4798      	blx	r3
30007956:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000795a:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
3000795e:	f043 0303 	orr.w	r3, r3, #3
30007962:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30007966:	bd08      	pop	{r3, pc}
30007968:	41011000 	.word	0x41011000
3000796c:	00009b2d 	.word	0x00009b2d

30007970 <flash_init_userdef>:
30007970:	4b13      	ldr	r3, [pc, #76]	; (300079c0 <flash_init_userdef+0x50>)
30007972:	2201      	movs	r2, #1
30007974:	2140      	movs	r1, #64	; 0x40
30007976:	20bb      	movs	r0, #187	; 0xbb
30007978:	601a      	str	r2, [r3, #0]
3000797a:	f04f 0ceb 	mov.w	ip, #235	; 0xeb
3000797e:	e9c3 1203 	strd	r1, r2, [r3, #12]
30007982:	2102      	movs	r1, #2
30007984:	2200      	movs	r2, #0
30007986:	e9c3 1205 	strd	r1, r2, [r3, #20]
3000798a:	f44f 6200 	mov.w	r2, #2048	; 0x800
3000798e:	490d      	ldr	r1, [pc, #52]	; (300079c4 <flash_init_userdef+0x54>)
30007990:	83da      	strh	r2, [r3, #30]
30007992:	f640 0204 	movw	r2, #2052	; 0x804
30007996:	841a      	strh	r2, [r3, #32]
30007998:	2206      	movs	r2, #6
3000799a:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
3000799e:	223b      	movs	r2, #59	; 0x3b
300079a0:	e9c3 2009 	strd	r2, r0, [r3, #36]	; 0x24
300079a4:	226b      	movs	r2, #107	; 0x6b
300079a6:	e9c3 2c0b 	strd	r2, ip, [r3, #44]	; 0x2c
300079aa:	4a07      	ldr	r2, [pc, #28]	; (300079c8 <flash_init_userdef+0x58>)
300079ac:	e9c3 1215 	strd	r1, r2, [r3, #84]	; 0x54
300079b0:	f64a 3220 	movw	r2, #43808	; 0xab20
300079b4:	f8a3 205c 	strh.w	r2, [r3, #92]	; 0x5c
300079b8:	22b9      	movs	r2, #185	; 0xb9
300079ba:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
300079be:	4770      	bx	lr
300079c0:	2001c01c 	.word	0x2001c01c
300079c4:	00059f06 	.word	0x00059f06
300079c8:	d8600001 	.word	0xd8600001

300079cc <nand_init_userdef>:
300079cc:	4b17      	ldr	r3, [pc, #92]	; (30007a2c <nand_init_userdef+0x60>)
300079ce:	f640 0008 	movw	r0, #2056	; 0x808
300079d2:	2201      	movs	r2, #1
300079d4:	2102      	movs	r1, #2
300079d6:	83d8      	strh	r0, [r3, #30]
300079d8:	f44f 6000 	mov.w	r0, #2048	; 0x800
300079dc:	601a      	str	r2, [r3, #0]
300079de:	8418      	strh	r0, [r3, #32]
300079e0:	203b      	movs	r0, #59	; 0x3b
300079e2:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
300079e6:	6258      	str	r0, [r3, #36]	; 0x24
300079e8:	206b      	movs	r0, #107	; 0x6b
300079ea:	e9c3 2203 	strd	r2, r2, [r3, #12]
300079ee:	2200      	movs	r2, #0
300079f0:	62d8      	str	r0, [r3, #44]	; 0x2c
300079f2:	200b      	movs	r0, #11
300079f4:	775a      	strb	r2, [r3, #29]
300079f6:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
300079fa:	629a      	str	r2, [r3, #40]	; 0x28
300079fc:	631a      	str	r2, [r3, #48]	; 0x30
300079fe:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30007a02:	65da      	str	r2, [r3, #92]	; 0x5c
30007a04:	6698      	str	r0, [r3, #104]	; 0x68
30007a06:	480a      	ldr	r0, [pc, #40]	; (30007a30 <nand_init_userdef+0x64>)
30007a08:	e9c3 1205 	strd	r1, r2, [r3, #20]
30007a0c:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30007a10:	2108      	movs	r1, #8
30007a12:	e9c3 1213 	strd	r1, r2, [r3, #76]	; 0x4c
30007a16:	f241 0213 	movw	r2, #4115	; 0x1013
30007a1a:	4906      	ldr	r1, [pc, #24]	; (30007a34 <nand_init_userdef+0x68>)
30007a1c:	f8a3 206e 	strh.w	r2, [r3, #110]	; 0x6e
30007a20:	4a05      	ldr	r2, [pc, #20]	; (30007a38 <nand_init_userdef+0x6c>)
30007a22:	e9c3 0115 	strd	r0, r1, [r3, #84]	; 0x54
30007a26:	671a      	str	r2, [r3, #112]	; 0x70
30007a28:	4770      	bx	lr
30007a2a:	bf00      	nop
30007a2c:	2001c01c 	.word	0x2001c01c
30007a30:	000f9f06 	.word	0x000f9f06
30007a34:	d800001f 	.word	0xd800001f
30007a38:	10300804 	.word	0x10300804

30007a3c <flash_get_layout_info>:
30007a3c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007a3e:	4b0c      	ldr	r3, [pc, #48]	; (30007a70 <flash_get_layout_info+0x34>)
30007a40:	4607      	mov	r7, r0
30007a42:	460d      	mov	r5, r1
30007a44:	4616      	mov	r6, r2
30007a46:	4c0b      	ldr	r4, [pc, #44]	; (30007a74 <flash_get_layout_info+0x38>)
30007a48:	4798      	blx	r3
30007a4a:	4b0b      	ldr	r3, [pc, #44]	; (30007a78 <flash_get_layout_info+0x3c>)
30007a4c:	2800      	cmp	r0, #0
30007a4e:	bf08      	it	eq
30007a50:	461c      	moveq	r4, r3
30007a52:	6822      	ldr	r2, [r4, #0]
30007a54:	4623      	mov	r3, r4
30007a56:	2aff      	cmp	r2, #255	; 0xff
30007a58:	d009      	beq.n	30007a6e <flash_get_layout_info+0x32>
30007a5a:	42ba      	cmp	r2, r7
30007a5c:	f103 040c 	add.w	r4, r3, #12
30007a60:	d1f7      	bne.n	30007a52 <flash_get_layout_info+0x16>
30007a62:	b10d      	cbz	r5, 30007a68 <flash_get_layout_info+0x2c>
30007a64:	685a      	ldr	r2, [r3, #4]
30007a66:	602a      	str	r2, [r5, #0]
30007a68:	b10e      	cbz	r6, 30007a6e <flash_get_layout_info+0x32>
30007a6a:	689b      	ldr	r3, [r3, #8]
30007a6c:	6033      	str	r3, [r6, #0]
30007a6e:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007a70:	30008f81 	.word	0x30008f81
30007a74:	3000df00 	.word	0x3000df00
30007a78:	3000de28 	.word	0x3000de28

30007a7c <NAND_CHECK_IS_BAD_BLOCK>:
30007a7c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007a80:	ea4f 1810 	mov.w	r8, r0, lsr #4
30007a84:	4606      	mov	r6, r0
30007a86:	4f1e      	ldr	r7, [pc, #120]	; (30007b00 <NAND_CHECK_IS_BAD_BLOCK+0x84>)
30007a88:	0040      	lsls	r0, r0, #1
30007a8a:	b085      	sub	sp, #20
30007a8c:	f000 051e 	and.w	r5, r0, #30
30007a90:	f857 3028 	ldr.w	r3, [r7, r8, lsl #2]
30007a94:	40eb      	lsrs	r3, r5
30007a96:	f003 0303 	and.w	r3, r3, #3
30007a9a:	2b02      	cmp	r3, #2
30007a9c:	d022      	beq.n	30007ae4 <NAND_CHECK_IS_BAD_BLOCK+0x68>
30007a9e:	2b01      	cmp	r3, #1
30007aa0:	d02b      	beq.n	30007afa <NAND_CHECK_IS_BAD_BLOCK+0x7e>
30007aa2:	4b18      	ldr	r3, [pc, #96]	; (30007b04 <NAND_CHECK_IS_BAD_BLOCK+0x88>)
30007aa4:	2401      	movs	r4, #1
30007aa6:	2204      	movs	r2, #4
30007aa8:	01b0      	lsls	r0, r6, #6
30007aaa:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30007aae:	ab03      	add	r3, sp, #12
30007ab0:	f8df 905c 	ldr.w	r9, [pc, #92]	; 30007b10 <NAND_CHECK_IS_BAD_BLOCK+0x94>
30007ab4:	fa04 f101 	lsl.w	r1, r4, r1
30007ab8:	47c8      	blx	r9
30007aba:	f89d 300c 	ldrb.w	r3, [sp, #12]
30007abe:	f89d 200d 	ldrb.w	r2, [sp, #13]
30007ac2:	4013      	ands	r3, r2
30007ac4:	2bff      	cmp	r3, #255	; 0xff
30007ac6:	d011      	beq.n	30007aec <NAND_CHECK_IS_BAD_BLOCK+0x70>
30007ac8:	2002      	movs	r0, #2
30007aca:	f857 3028 	ldr.w	r3, [r7, r8, lsl #2]
30007ace:	4a0e      	ldr	r2, [pc, #56]	; (30007b08 <NAND_CHECK_IS_BAD_BLOCK+0x8c>)
30007ad0:	fa00 f505 	lsl.w	r5, r0, r5
30007ad4:	9600      	str	r6, [sp, #0]
30007ad6:	432b      	orrs	r3, r5
30007ad8:	f847 3028 	str.w	r3, [r7, r8, lsl #2]
30007adc:	4b0b      	ldr	r3, [pc, #44]	; (30007b0c <NAND_CHECK_IS_BAD_BLOCK+0x90>)
30007ade:	4619      	mov	r1, r3
30007ae0:	f003 f9e2 	bl	3000aea8 <rtk_log_write>
30007ae4:	2001      	movs	r0, #1
30007ae6:	b005      	add	sp, #20
30007ae8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007aec:	fa04 f005 	lsl.w	r0, r4, r5
30007af0:	f857 4028 	ldr.w	r4, [r7, r8, lsl #2]
30007af4:	4304      	orrs	r4, r0
30007af6:	f847 4028 	str.w	r4, [r7, r8, lsl #2]
30007afa:	2000      	movs	r0, #0
30007afc:	e7f3      	b.n	30007ae6 <NAND_CHECK_IS_BAD_BLOCK+0x6a>
30007afe:	bf00      	nop
30007b00:	3000eb48 	.word	0x3000eb48
30007b04:	2001c01c 	.word	0x2001c01c
30007b08:	3000babf 	.word	0x3000babf
30007b0c:	3000bab5 	.word	0x3000bab5
30007b10:	30008ec1 	.word	0x30008ec1

30007b14 <Nand_Get_NandAddr>:
30007b14:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007b18:	4f0f      	ldr	r7, [pc, #60]	; (30007b58 <Nand_Get_NandAddr+0x44>)
30007b1a:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30007b1e:	f100 4478 	add.w	r4, r0, #4160749568	; 0xf8000000
30007b22:	f04f 0840 	mov.w	r8, #64	; 0x40
30007b26:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30007b2a:	40ec      	lsrs	r4, r5
30007b2c:	fa26 f505 	lsr.w	r5, r6, r5
30007b30:	09a4      	lsrs	r4, r4, #6
30007b32:	09ad      	lsrs	r5, r5, #6
30007b34:	42a5      	cmp	r5, r4
30007b36:	d802      	bhi.n	30007b3e <Nand_Get_NandAddr+0x2a>
30007b38:	4630      	mov	r0, r6
30007b3a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007b3e:	4620      	mov	r0, r4
30007b40:	f7ff ff9c 	bl	30007a7c <NAND_CHECK_IS_BAD_BLOCK>
30007b44:	b128      	cbz	r0, 30007b52 <Nand_Get_NandAddr+0x3e>
30007b46:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30007b4a:	3501      	adds	r5, #1
30007b4c:	fa08 f303 	lsl.w	r3, r8, r3
30007b50:	441e      	add	r6, r3
30007b52:	3401      	adds	r4, #1
30007b54:	e7ee      	b.n	30007b34 <Nand_Get_NandAddr+0x20>
30007b56:	bf00      	nop
30007b58:	2001c01c 	.word	0x2001c01c

30007b5c <Nand_L2P_Table>:
30007b5c:	b513      	push	{r0, r1, r4, lr}
30007b5e:	4604      	mov	r4, r0
30007b60:	aa01      	add	r2, sp, #4
30007b62:	2002      	movs	r0, #2
30007b64:	4669      	mov	r1, sp
30007b66:	f7ff ff69 	bl	30007a3c <flash_get_layout_info>
30007b6a:	9800      	ldr	r0, [sp, #0]
30007b6c:	42a0      	cmp	r0, r4
30007b6e:	d906      	bls.n	30007b7e <Nand_L2P_Table+0x22>
30007b70:	4621      	mov	r1, r4
30007b72:	f04f 6000 	mov.w	r0, #134217728	; 0x8000000
30007b76:	f7ff ffcd 	bl	30007b14 <Nand_Get_NandAddr>
30007b7a:	b002      	add	sp, #8
30007b7c:	bd10      	pop	{r4, pc}
30007b7e:	9b01      	ldr	r3, [sp, #4]
30007b80:	42a3      	cmp	r3, r4
30007b82:	d901      	bls.n	30007b88 <Nand_L2P_Table+0x2c>
30007b84:	4621      	mov	r1, r4
30007b86:	e7f6      	b.n	30007b76 <Nand_L2P_Table+0x1a>
30007b88:	f104 4078 	add.w	r0, r4, #4160749568	; 0xf8000000
30007b8c:	e7f5      	b.n	30007b7a <Nand_L2P_Table+0x1e>
	...

30007b90 <NandImgCopy>:
30007b90:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007b94:	4690      	mov	r8, r2
30007b96:	b087      	sub	sp, #28
30007b98:	f8df a0c8 	ldr.w	sl, [pc, #200]	; 30007c64 <NandImgCopy+0xd4>
30007b9c:	f04f 0900 	mov.w	r9, #0
30007ba0:	f108 34ff 	add.w	r4, r8, #4294967295	; 0xffffffff
30007ba4:	9005      	str	r0, [sp, #20]
30007ba6:	4608      	mov	r0, r1
30007ba8:	46cb      	mov	fp, r9
30007baa:	f7ff ffd7 	bl	30007b5c <Nand_L2P_Table>
30007bae:	f89a 306c 	ldrb.w	r3, [sl, #108]	; 0x6c
30007bb2:	4404      	add	r4, r0
30007bb4:	4606      	mov	r6, r0
30007bb6:	40dc      	lsrs	r4, r3
30007bb8:	fa20 f303 	lsr.w	r3, r0, r3
30007bbc:	1ae4      	subs	r4, r4, r3
30007bbe:	1c63      	adds	r3, r4, #1
30007bc0:	9304      	str	r3, [sp, #16]
30007bc2:	9b04      	ldr	r3, [sp, #16]
30007bc4:	b913      	cbnz	r3, 30007bcc <NandImgCopy+0x3c>
30007bc6:	b007      	add	sp, #28
30007bc8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007bcc:	f89a 506c 	ldrb.w	r5, [sl, #108]	; 0x6c
30007bd0:	fa26 f505 	lsr.w	r5, r6, r5
30007bd4:	ebbb 1f95 	cmp.w	fp, r5, lsr #6
30007bd8:	ea4f 1795 	mov.w	r7, r5, lsr #6
30007bdc:	d805      	bhi.n	30007bea <NandImgCopy+0x5a>
30007bde:	4638      	mov	r0, r7
30007be0:	f107 0b01 	add.w	fp, r7, #1
30007be4:	f7ff ff4a 	bl	30007a7c <NAND_CHECK_IS_BAD_BLOCK>
30007be8:	bb68      	cbnz	r0, 30007c46 <NandImgCopy+0xb6>
30007bea:	f89a 306c 	ldrb.w	r3, [sl, #108]	; 0x6c
30007bee:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30007bf2:	4628      	mov	r0, r5
30007bf4:	4c18      	ldr	r4, [pc, #96]	; (30007c58 <NandImgCopy+0xc8>)
30007bf6:	f1c3 0220 	rsb	r2, r3, #32
30007bfa:	40d1      	lsrs	r1, r2
30007bfc:	9a04      	ldr	r2, [sp, #16]
30007bfe:	2a01      	cmp	r2, #1
30007c00:	ea01 0106 	and.w	r1, r1, r6
30007c04:	bf16      	itet	ne
30007c06:	2701      	movne	r7, #1
30007c08:	4647      	moveq	r7, r8
30007c0a:	409f      	lslne	r7, r3
30007c0c:	9b05      	ldr	r3, [sp, #20]
30007c0e:	bf18      	it	ne
30007c10:	1a7f      	subne	r7, r7, r1
30007c12:	444b      	add	r3, r9
30007c14:	463a      	mov	r2, r7
30007c16:	47a0      	blx	r4
30007c18:	f010 0f70 	tst.w	r0, #112	; 0x70
30007c1c:	d00b      	beq.n	30007c36 <NandImgCopy+0xa6>
30007c1e:	09ab      	lsrs	r3, r5, #6
30007c20:	f005 053f 	and.w	r5, r5, #63	; 0x3f
30007c24:	9002      	str	r0, [sp, #8]
30007c26:	2004      	movs	r0, #4
30007c28:	9301      	str	r3, [sp, #4]
30007c2a:	4b0c      	ldr	r3, [pc, #48]	; (30007c5c <NandImgCopy+0xcc>)
30007c2c:	4a0c      	ldr	r2, [pc, #48]	; (30007c60 <NandImgCopy+0xd0>)
30007c2e:	4619      	mov	r1, r3
30007c30:	9500      	str	r5, [sp, #0]
30007c32:	f003 f939 	bl	3000aea8 <rtk_log_write>
30007c36:	9b04      	ldr	r3, [sp, #16]
30007c38:	44b9      	add	r9, r7
30007c3a:	443e      	add	r6, r7
30007c3c:	eba8 0807 	sub.w	r8, r8, r7
30007c40:	3b01      	subs	r3, #1
30007c42:	9304      	str	r3, [sp, #16]
30007c44:	e7bd      	b.n	30007bc2 <NandImgCopy+0x32>
30007c46:	f89a 306c 	ldrb.w	r3, [sl, #108]	; 0x6c
30007c4a:	2240      	movs	r2, #64	; 0x40
30007c4c:	3540      	adds	r5, #64	; 0x40
30007c4e:	465f      	mov	r7, fp
30007c50:	fa02 f303 	lsl.w	r3, r2, r3
30007c54:	441e      	add	r6, r3
30007c56:	e7c2      	b.n	30007bde <NandImgCopy+0x4e>
30007c58:	30008ec1 	.word	0x30008ec1
30007c5c:	3000bab5 	.word	0x3000bab5
30007c60:	3000bad7 	.word	0x3000bad7
30007c64:	2001c01c 	.word	0x2001c01c

30007c68 <OTPPowerSwitch.constprop.0>:
30007c68:	4b09      	ldr	r3, [pc, #36]	; (30007c90 <OTPPowerSwitch.constprop.0+0x28>)
30007c6a:	681a      	ldr	r2, [r3, #0]
30007c6c:	07d2      	lsls	r2, r2, #31
30007c6e:	bf5e      	ittt	pl
30007c70:	681a      	ldrpl	r2, [r3, #0]
30007c72:	f042 0201 	orrpl.w	r2, r2, #1
30007c76:	601a      	strpl	r2, [r3, #0]
30007c78:	f04f 4384 	mov.w	r3, #1107296256	; 0x42000000
30007c7c:	695a      	ldr	r2, [r3, #20]
30007c7e:	b118      	cbz	r0, 30007c88 <OTPPowerSwitch.constprop.0+0x20>
30007c80:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
30007c84:	615a      	str	r2, [r3, #20]
30007c86:	4770      	bx	lr
30007c88:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30007c8c:	e7fa      	b.n	30007c84 <OTPPowerSwitch.constprop.0+0x1c>
30007c8e:	bf00      	nop
30007c90:	42008000 	.word	0x42008000

30007c94 <OTP_ProgramMarginRead8.part.0>:
30007c94:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007c98:	4b29      	ldr	r3, [pc, #164]	; (30007d40 <OTP_ProgramMarginRead8.part.0+0xac>)
30007c9a:	4607      	mov	r7, r0
30007c9c:	e843 f300 	tt	r3, r3
30007ca0:	f3c3 5380 	ubfx	r3, r3, #22, #1
30007ca4:	4e27      	ldr	r6, [pc, #156]	; (30007d44 <OTP_ProgramMarginRead8.part.0+0xb0>)
30007ca6:	4688      	mov	r8, r1
30007ca8:	f8df 90a8 	ldr.w	r9, [pc, #168]	; 30007d54 <OTP_ProgramMarginRead8.part.0+0xc0>
30007cac:	2b00      	cmp	r3, #0
30007cae:	4b26      	ldr	r3, [pc, #152]	; (30007d48 <OTP_ProgramMarginRead8.part.0+0xb4>)
30007cb0:	bf0e      	itee	eq
30007cb2:	f04f 4584 	moveq.w	r5, #1107296256	; 0x42000000
30007cb6:	461e      	movne	r6, r3
30007cb8:	f04f 45a4 	movne.w	r5, #1375731712	; 0x52000000
30007cbc:	6c2c      	ldr	r4, [r5, #64]	; 0x40
30007cbe:	f414 7480 	ands.w	r4, r4, #256	; 0x100
30007cc2:	d11f      	bne.n	30007d04 <OTP_ProgramMarginRead8.part.0+0x70>
30007cc4:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007cc6:	4620      	mov	r0, r4
30007cc8:	46a1      	mov	r9, r4
30007cca:	f644 6a20 	movw	sl, #20000	; 0x4e20
30007cce:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007cd2:	f8df b080 	ldr.w	fp, [pc, #128]	; 30007d54 <OTP_ProgramMarginRead8.part.0+0xc0>
30007cd6:	642b      	str	r3, [r5, #64]	; 0x40
30007cd8:	f7ff ffc6 	bl	30007c68 <OTPPowerSwitch.constprop.0>
30007cdc:	023b      	lsls	r3, r7, #8
30007cde:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
30007ce2:	6033      	str	r3, [r6, #0]
30007ce4:	6833      	ldr	r3, [r6, #0]
30007ce6:	2b00      	cmp	r3, #0
30007ce8:	db14      	blt.n	30007d14 <OTP_ProgramMarginRead8.part.0+0x80>
30007cea:	45d1      	cmp	r9, sl
30007cec:	d10d      	bne.n	30007d0a <OTP_ProgramMarginRead8.part.0+0x76>
30007cee:	23ff      	movs	r3, #255	; 0xff
30007cf0:	4a16      	ldr	r2, [pc, #88]	; (30007d4c <OTP_ProgramMarginRead8.part.0+0xb8>)
30007cf2:	2002      	movs	r0, #2
30007cf4:	9700      	str	r7, [sp, #0]
30007cf6:	f888 3000 	strb.w	r3, [r8]
30007cfa:	4b15      	ldr	r3, [pc, #84]	; (30007d50 <OTP_ProgramMarginRead8.part.0+0xbc>)
30007cfc:	4619      	mov	r1, r3
30007cfe:	f003 f8d3 	bl	3000aea8 <rtk_log_write>
30007d02:	e00d      	b.n	30007d20 <OTP_ProgramMarginRead8.part.0+0x8c>
30007d04:	2064      	movs	r0, #100	; 0x64
30007d06:	47c8      	blx	r9
30007d08:	e7d8      	b.n	30007cbc <OTP_ProgramMarginRead8.part.0+0x28>
30007d0a:	2005      	movs	r0, #5
30007d0c:	f109 0901 	add.w	r9, r9, #1
30007d10:	47d8      	blx	fp
30007d12:	e7e7      	b.n	30007ce4 <OTP_ProgramMarginRead8.part.0+0x50>
30007d14:	45d1      	cmp	r9, sl
30007d16:	d0ea      	beq.n	30007cee <OTP_ProgramMarginRead8.part.0+0x5a>
30007d18:	6833      	ldr	r3, [r6, #0]
30007d1a:	2401      	movs	r4, #1
30007d1c:	f888 3000 	strb.w	r3, [r8]
30007d20:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30007d24:	f004 0001 	and.w	r0, r4, #1
30007d28:	6953      	ldr	r3, [r2, #20]
30007d2a:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30007d2e:	6153      	str	r3, [r2, #20]
30007d30:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007d32:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30007d36:	642b      	str	r3, [r5, #64]	; 0x40
30007d38:	b003      	add	sp, #12
30007d3a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007d3e:	bf00      	nop
30007d40:	0000e5f9 	.word	0x0000e5f9
30007d44:	42000008 	.word	0x42000008
30007d48:	5200000c 	.word	0x5200000c
30007d4c:	3000bb17 	.word	0x3000bb17
30007d50:	3000bb13 	.word	0x3000bb13
30007d54:	00009b2d 	.word	0x00009b2d

30007d58 <OTP_Read8>:
30007d58:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30007d5c:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007d60:	4606      	mov	r6, r0
30007d62:	460f      	mov	r7, r1
30007d64:	d306      	bcc.n	30007d74 <OTP_Read8+0x1c>
30007d66:	23ff      	movs	r3, #255	; 0xff
30007d68:	2400      	movs	r4, #0
30007d6a:	700b      	strb	r3, [r1, #0]
30007d6c:	4620      	mov	r0, r4
30007d6e:	b003      	add	sp, #12
30007d70:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007d74:	4b26      	ldr	r3, [pc, #152]	; (30007e10 <OTP_Read8+0xb8>)
30007d76:	e843 f300 	tt	r3, r3
30007d7a:	f3c3 5380 	ubfx	r3, r3, #22, #1
30007d7e:	4a25      	ldr	r2, [pc, #148]	; (30007e14 <OTP_Read8+0xbc>)
30007d80:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 30007e24 <OTP_Read8+0xcc>
30007d84:	2b00      	cmp	r3, #0
30007d86:	4b24      	ldr	r3, [pc, #144]	; (30007e18 <OTP_Read8+0xc0>)
30007d88:	bf0b      	itete	eq
30007d8a:	4690      	moveq	r8, r2
30007d8c:	4698      	movne	r8, r3
30007d8e:	f04f 4584 	moveq.w	r5, #1107296256	; 0x42000000
30007d92:	f04f 45a4 	movne.w	r5, #1375731712	; 0x52000000
30007d96:	6c2c      	ldr	r4, [r5, #64]	; 0x40
30007d98:	f414 7480 	ands.w	r4, r4, #256	; 0x100
30007d9c:	d11e      	bne.n	30007ddc <OTP_Read8+0x84>
30007d9e:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007da0:	4620      	mov	r0, r4
30007da2:	46a1      	mov	r9, r4
30007da4:	f644 6a20 	movw	sl, #20000	; 0x4e20
30007da8:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007dac:	f8df b074 	ldr.w	fp, [pc, #116]	; 30007e24 <OTP_Read8+0xcc>
30007db0:	642b      	str	r3, [r5, #64]	; 0x40
30007db2:	f7ff ff59 	bl	30007c68 <OTPPowerSwitch.constprop.0>
30007db6:	0233      	lsls	r3, r6, #8
30007db8:	f8c8 3000 	str.w	r3, [r8]
30007dbc:	f8d8 3000 	ldr.w	r3, [r8]
30007dc0:	2b00      	cmp	r3, #0
30007dc2:	db13      	blt.n	30007dec <OTP_Read8+0x94>
30007dc4:	45d1      	cmp	r9, sl
30007dc6:	d10c      	bne.n	30007de2 <OTP_Read8+0x8a>
30007dc8:	23ff      	movs	r3, #255	; 0xff
30007dca:	4a14      	ldr	r2, [pc, #80]	; (30007e1c <OTP_Read8+0xc4>)
30007dcc:	2002      	movs	r0, #2
30007dce:	9600      	str	r6, [sp, #0]
30007dd0:	703b      	strb	r3, [r7, #0]
30007dd2:	4b13      	ldr	r3, [pc, #76]	; (30007e20 <OTP_Read8+0xc8>)
30007dd4:	4619      	mov	r1, r3
30007dd6:	f003 f867 	bl	3000aea8 <rtk_log_write>
30007dda:	e00d      	b.n	30007df8 <OTP_Read8+0xa0>
30007ddc:	2064      	movs	r0, #100	; 0x64
30007dde:	47c8      	blx	r9
30007de0:	e7d9      	b.n	30007d96 <OTP_Read8+0x3e>
30007de2:	2005      	movs	r0, #5
30007de4:	f109 0901 	add.w	r9, r9, #1
30007de8:	47d8      	blx	fp
30007dea:	e7e7      	b.n	30007dbc <OTP_Read8+0x64>
30007dec:	45d1      	cmp	r9, sl
30007dee:	d0eb      	beq.n	30007dc8 <OTP_Read8+0x70>
30007df0:	f8d8 3000 	ldr.w	r3, [r8]
30007df4:	2401      	movs	r4, #1
30007df6:	703b      	strb	r3, [r7, #0]
30007df8:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30007dfc:	6953      	ldr	r3, [r2, #20]
30007dfe:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30007e02:	6153      	str	r3, [r2, #20]
30007e04:	6c2b      	ldr	r3, [r5, #64]	; 0x40
30007e06:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30007e0a:	642b      	str	r3, [r5, #64]	; 0x40
30007e0c:	e7ae      	b.n	30007d6c <OTP_Read8+0x14>
30007e0e:	bf00      	nop
30007e10:	0000e5f9 	.word	0x0000e5f9
30007e14:	42000008 	.word	0x42000008
30007e18:	5200000c 	.word	0x5200000c
30007e1c:	3000bb17 	.word	0x3000bb17
30007e20:	3000bb13 	.word	0x3000bb13
30007e24:	00009b2d 	.word	0x00009b2d

30007e28 <OTP_Write8>:
30007e28:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30007e2c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007e30:	4605      	mov	r5, r0
30007e32:	b087      	sub	sp, #28
30007e34:	4688      	mov	r8, r1
30007e36:	d30a      	bcc.n	30007e4e <OTP_Write8+0x26>
30007e38:	23ff      	movs	r3, #255	; 0xff
30007e3a:	f88d 3017 	strb.w	r3, [sp, #23]
30007e3e:	4b42      	ldr	r3, [pc, #264]	; (30007f48 <OTP_Write8+0x120>)
30007e40:	2002      	movs	r0, #2
30007e42:	4a42      	ldr	r2, [pc, #264]	; (30007f4c <OTP_Write8+0x124>)
30007e44:	4619      	mov	r1, r3
30007e46:	f003 f82f 	bl	3000aea8 <rtk_log_write>
30007e4a:	2400      	movs	r4, #0
30007e4c:	e054      	b.n	30007ef8 <OTP_Write8+0xd0>
30007e4e:	f10d 0117 	add.w	r1, sp, #23
30007e52:	f7ff ff1f 	bl	30007c94 <OTP_ProgramMarginRead8.part.0>
30007e56:	2800      	cmp	r0, #0
30007e58:	d0f1      	beq.n	30007e3e <OTP_Write8+0x16>
30007e5a:	022b      	lsls	r3, r5, #8
30007e5c:	4647      	mov	r7, r8
30007e5e:	f04f 0a02 	mov.w	sl, #2
30007e62:	f8df b104 	ldr.w	fp, [pc, #260]	; 30007f68 <OTP_Write8+0x140>
30007e66:	9302      	str	r3, [sp, #8]
30007e68:	f89d 3017 	ldrb.w	r3, [sp, #23]
30007e6c:	ea67 0703 	orn	r7, r7, r3
30007e70:	b2ff      	uxtb	r7, r7
30007e72:	2fff      	cmp	r7, #255	; 0xff
30007e74:	d05d      	beq.n	30007f32 <OTP_Write8+0x10a>
30007e76:	4b36      	ldr	r3, [pc, #216]	; (30007f50 <OTP_Write8+0x128>)
30007e78:	e843 f300 	tt	r3, r3
30007e7c:	f3c3 5380 	ubfx	r3, r3, #22, #1
30007e80:	4a34      	ldr	r2, [pc, #208]	; (30007f54 <OTP_Write8+0x12c>)
30007e82:	2b00      	cmp	r3, #0
30007e84:	4b34      	ldr	r3, [pc, #208]	; (30007f58 <OTP_Write8+0x130>)
30007e86:	bf0b      	itete	eq
30007e88:	4691      	moveq	r9, r2
30007e8a:	4699      	movne	r9, r3
30007e8c:	f04f 4684 	moveq.w	r6, #1107296256	; 0x42000000
30007e90:	f04f 46a4 	movne.w	r6, #1375731712	; 0x52000000
30007e94:	6c34      	ldr	r4, [r6, #64]	; 0x40
30007e96:	f414 7480 	ands.w	r4, r4, #256	; 0x100
30007e9a:	d131      	bne.n	30007f00 <OTP_Write8+0xd8>
30007e9c:	6c33      	ldr	r3, [r6, #64]	; 0x40
30007e9e:	2001      	movs	r0, #1
30007ea0:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30007ea4:	6433      	str	r3, [r6, #64]	; 0x40
30007ea6:	f7ff fedf 	bl	30007c68 <OTPPowerSwitch.constprop.0>
30007eaa:	9b02      	ldr	r3, [sp, #8]
30007eac:	433b      	orrs	r3, r7
30007eae:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
30007eb2:	f8c9 3000 	str.w	r3, [r9]
30007eb6:	4623      	mov	r3, r4
30007eb8:	f8d9 1000 	ldr.w	r1, [r9]
30007ebc:	f644 6220 	movw	r2, #20000	; 0x4e20
30007ec0:	2900      	cmp	r1, #0
30007ec2:	da26      	bge.n	30007f12 <OTP_Write8+0xea>
30007ec4:	4293      	cmp	r3, r2
30007ec6:	d11e      	bne.n	30007f06 <OTP_Write8+0xde>
30007ec8:	4b1f      	ldr	r3, [pc, #124]	; (30007f48 <OTP_Write8+0x120>)
30007eca:	2003      	movs	r0, #3
30007ecc:	4a23      	ldr	r2, [pc, #140]	; (30007f5c <OTP_Write8+0x134>)
30007ece:	4619      	mov	r1, r3
30007ed0:	9500      	str	r5, [sp, #0]
30007ed2:	f002 ffe9 	bl	3000aea8 <rtk_log_write>
30007ed6:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30007eda:	6953      	ldr	r3, [r2, #20]
30007edc:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30007ee0:	6153      	str	r3, [r2, #20]
30007ee2:	6c33      	ldr	r3, [r6, #64]	; 0x40
30007ee4:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30007ee8:	6433      	str	r3, [r6, #64]	; 0x40
30007eea:	bb14      	cbnz	r4, 30007f32 <OTP_Write8+0x10a>
30007eec:	4b16      	ldr	r3, [pc, #88]	; (30007f48 <OTP_Write8+0x120>)
30007eee:	4a1c      	ldr	r2, [pc, #112]	; (30007f60 <OTP_Write8+0x138>)
30007ef0:	4619      	mov	r1, r3
30007ef2:	2002      	movs	r0, #2
30007ef4:	f002 ffd8 	bl	3000aea8 <rtk_log_write>
30007ef8:	4620      	mov	r0, r4
30007efa:	b007      	add	sp, #28
30007efc:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007f00:	2064      	movs	r0, #100	; 0x64
30007f02:	47d8      	blx	fp
30007f04:	e7c6      	b.n	30007e94 <OTP_Write8+0x6c>
30007f06:	2005      	movs	r0, #5
30007f08:	9303      	str	r3, [sp, #12]
30007f0a:	47d8      	blx	fp
30007f0c:	9b03      	ldr	r3, [sp, #12]
30007f0e:	3301      	adds	r3, #1
30007f10:	e7d2      	b.n	30007eb8 <OTP_Write8+0x90>
30007f12:	4293      	cmp	r3, r2
30007f14:	d0d8      	beq.n	30007ec8 <OTP_Write8+0xa0>
30007f16:	2401      	movs	r4, #1
30007f18:	e7dd      	b.n	30007ed6 <OTP_Write8+0xae>
30007f1a:	f89d 3017 	ldrb.w	r3, [sp, #23]
30007f1e:	4543      	cmp	r3, r8
30007f20:	d005      	beq.n	30007f2e <OTP_Write8+0x106>
30007f22:	f1ba 0f01 	cmp.w	sl, #1
30007f26:	d090      	beq.n	30007e4a <OTP_Write8+0x22>
30007f28:	f04f 0a01 	mov.w	sl, #1
30007f2c:	e79c      	b.n	30007e68 <OTP_Write8+0x40>
30007f2e:	2401      	movs	r4, #1
30007f30:	e7e2      	b.n	30007ef8 <OTP_Write8+0xd0>
30007f32:	f10d 0117 	add.w	r1, sp, #23
30007f36:	4628      	mov	r0, r5
30007f38:	f7ff feac 	bl	30007c94 <OTP_ProgramMarginRead8.part.0>
30007f3c:	4604      	mov	r4, r0
30007f3e:	2800      	cmp	r0, #0
30007f40:	d1eb      	bne.n	30007f1a <OTP_Write8+0xf2>
30007f42:	4b01      	ldr	r3, [pc, #4]	; (30007f48 <OTP_Write8+0x120>)
30007f44:	4a07      	ldr	r2, [pc, #28]	; (30007f64 <OTP_Write8+0x13c>)
30007f46:	e7d3      	b.n	30007ef0 <OTP_Write8+0xc8>
30007f48:	3000bb13 	.word	0x3000bb13
30007f4c:	3000bb32 	.word	0x3000bb32
30007f50:	0000e5f9 	.word	0x0000e5f9
30007f54:	42000008 	.word	0x42000008
30007f58:	5200000c 	.word	0x5200000c
30007f5c:	3000bb4a 	.word	0x3000bb4a
30007f60:	3000bb66 	.word	0x3000bb66
30007f64:	3000bb81 	.word	0x3000bb81
30007f68:	00009b2d 	.word	0x00009b2d

30007f6c <PLL_NP_ClkSet>:
30007f6c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007f70:	4606      	mov	r6, r0
30007f72:	4b1a      	ldr	r3, [pc, #104]	; (30007fdc <PLL_NP_ClkSet+0x70>)
30007f74:	4798      	blx	r3
30007f76:	4d1a      	ldr	r5, [pc, #104]	; (30007fe0 <PLL_NP_ClkSet+0x74>)
30007f78:	08c2      	lsrs	r2, r0, #3
30007f7a:	6c69      	ldr	r1, [r5, #68]	; 0x44
30007f7c:	fbb6 f3f0 	udiv	r3, r6, r0
30007f80:	fb00 6613 	mls	r6, r0, r3, r6
30007f84:	3b02      	subs	r3, #2
30007f86:	f421 61fc 	bic.w	r1, r1, #2016	; 0x7e0
30007f8a:	015b      	lsls	r3, r3, #5
30007f8c:	fbb6 f7f2 	udiv	r7, r6, r2
30007f90:	f403 63fc 	and.w	r3, r3, #2016	; 0x7e0
30007f94:	fb02 6017 	mls	r0, r2, r7, r6
30007f98:	430b      	orrs	r3, r1
30007f9a:	0439      	lsls	r1, r7, #16
30007f9c:	646b      	str	r3, [r5, #68]	; 0x44
30007f9e:	f401 21e0 	and.w	r1, r1, #458752	; 0x70000
30007fa2:	6ceb      	ldr	r3, [r5, #76]	; 0x4c
30007fa4:	b29b      	uxth	r3, r3
30007fa6:	ea41 0403 	orr.w	r4, r1, r3
30007faa:	2300      	movs	r3, #0
30007fac:	4619      	mov	r1, r3
30007fae:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30007fb2:	f002 ffdb 	bl	3000af6c <__aeabi_uldivmod>
30007fb6:	ea44 40c0 	orr.w	r0, r4, r0, lsl #19
30007fba:	4c0a      	ldr	r4, [pc, #40]	; (30007fe4 <PLL_NP_ClkSet+0x78>)
30007fbc:	64e8      	str	r0, [r5, #76]	; 0x4c
30007fbe:	2001      	movs	r0, #1
30007fc0:	47a0      	blx	r4
30007fc2:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30007fc4:	203c      	movs	r0, #60	; 0x3c
30007fc6:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30007fca:	646b      	str	r3, [r5, #68]	; 0x44
30007fcc:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30007fce:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30007fd2:	646b      	str	r3, [r5, #68]	; 0x44
30007fd4:	4623      	mov	r3, r4
30007fd6:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007fda:	4718      	bx	r3
30007fdc:	000099f5 	.word	0x000099f5
30007fe0:	42008800 	.word	0x42008800
30007fe4:	00009b2d 	.word	0x00009b2d

30007fe8 <PLL_AP_ClkSet>:
30007fe8:	b510      	push	{r4, lr}
30007fea:	4604      	mov	r4, r0
30007fec:	4b0a      	ldr	r3, [pc, #40]	; (30008018 <PLL_AP_ClkSet+0x30>)
30007fee:	4798      	blx	r3
30007ff0:	fbb4 f0f0 	udiv	r0, r4, r0
30007ff4:	1e84      	subs	r4, r0, #2
30007ff6:	3819      	subs	r0, #25
30007ff8:	280f      	cmp	r0, #15
30007ffa:	d904      	bls.n	30008006 <PLL_AP_ClkSet+0x1e>
30007ffc:	f240 1181 	movw	r1, #385	; 0x181
30008000:	4806      	ldr	r0, [pc, #24]	; (3000801c <PLL_AP_ClkSet+0x34>)
30008002:	f003 f9a5 	bl	3000b350 <__io_assert_failed_veneer>
30008006:	4a06      	ldr	r2, [pc, #24]	; (30008020 <PLL_AP_ClkSet+0x38>)
30008008:	b2e4      	uxtb	r4, r4
3000800a:	6953      	ldr	r3, [r2, #20]
3000800c:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008010:	431c      	orrs	r4, r3
30008012:	6154      	str	r4, [r2, #20]
30008014:	bd10      	pop	{r4, pc}
30008016:	bf00      	nop
30008018:	000099f5 	.word	0x000099f5
3000801c:	3000bb9a 	.word	0x3000bb9a
30008020:	41000300 	.word	0x41000300

30008024 <PLL_AP>:
30008024:	2801      	cmp	r0, #1
30008026:	b538      	push	{r3, r4, r5, lr}
30008028:	d11a      	bne.n	30008060 <PLL_AP+0x3c>
3000802a:	4b12      	ldr	r3, [pc, #72]	; (30008074 <PLL_AP+0x50>)
3000802c:	4d12      	ldr	r5, [pc, #72]	; (30008078 <PLL_AP+0x54>)
3000802e:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008030:	f012 0f05 	tst.w	r2, #5
30008034:	d105      	bne.n	30008042 <PLL_AP+0x1e>
30008036:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008038:	20a0      	movs	r0, #160	; 0xa0
3000803a:	f042 0207 	orr.w	r2, r2, #7
3000803e:	679a      	str	r2, [r3, #120]	; 0x78
30008040:	47a8      	blx	r5
30008042:	4c0e      	ldr	r4, [pc, #56]	; (3000807c <PLL_AP+0x58>)
30008044:	2001      	movs	r0, #1
30008046:	6823      	ldr	r3, [r4, #0]
30008048:	f043 0306 	orr.w	r3, r3, #6
3000804c:	6023      	str	r3, [r4, #0]
3000804e:	47a8      	blx	r5
30008050:	6823      	ldr	r3, [r4, #0]
30008052:	f043 0309 	orr.w	r3, r3, #9
30008056:	6023      	str	r3, [r4, #0]
30008058:	6863      	ldr	r3, [r4, #4]
3000805a:	2b00      	cmp	r3, #0
3000805c:	dafc      	bge.n	30008058 <PLL_AP+0x34>
3000805e:	bd38      	pop	{r3, r4, r5, pc}
30008060:	4b06      	ldr	r3, [pc, #24]	; (3000807c <PLL_AP+0x58>)
30008062:	681a      	ldr	r2, [r3, #0]
30008064:	f022 0209 	bic.w	r2, r2, #9
30008068:	601a      	str	r2, [r3, #0]
3000806a:	681a      	ldr	r2, [r3, #0]
3000806c:	f022 0206 	bic.w	r2, r2, #6
30008070:	601a      	str	r2, [r3, #0]
30008072:	e7f4      	b.n	3000805e <PLL_AP+0x3a>
30008074:	42008800 	.word	0x42008800
30008078:	00009b2d 	.word	0x00009b2d
3000807c:	41000300 	.word	0x41000300

30008080 <PSRAM_INFO_Update>:
30008080:	4b39      	ldr	r3, [pc, #228]	; (30008168 <PSRAM_INFO_Update+0xe8>)
30008082:	b537      	push	{r0, r1, r2, r4, r5, lr}
30008084:	4a39      	ldr	r2, [pc, #228]	; (3000816c <PSRAM_INFO_Update+0xec>)
30008086:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
3000808a:	6815      	ldr	r5, [r2, #0]
3000808c:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008090:	4a37      	ldr	r2, [pc, #220]	; (30008170 <PSRAM_INFO_Update+0xf0>)
30008092:	4c38      	ldr	r4, [pc, #224]	; (30008174 <PSRAM_INFO_Update+0xf4>)
30008094:	4355      	muls	r5, r2
30008096:	3301      	adds	r3, #1
30008098:	fbb5 f5f3 	udiv	r5, r5, r3
3000809c:	086b      	lsrs	r3, r5, #1
3000809e:	60e3      	str	r3, [r4, #12]
300080a0:	4b35      	ldr	r3, [pc, #212]	; (30008178 <PSRAM_INFO_Update+0xf8>)
300080a2:	4798      	blx	r3
300080a4:	4b35      	ldr	r3, [pc, #212]	; (3000817c <PSRAM_INFO_Update+0xfc>)
300080a6:	6020      	str	r0, [r4, #0]
300080a8:	4798      	blx	r3
300080aa:	4b35      	ldr	r3, [pc, #212]	; (30008180 <PSRAM_INFO_Update+0x100>)
300080ac:	2220      	movs	r2, #32
300080ae:	210a      	movs	r1, #10
300080b0:	60a0      	str	r0, [r4, #8]
300080b2:	fbb3 f3f5 	udiv	r3, r3, r5
300080b6:	6163      	str	r3, [r4, #20]
300080b8:	6121      	str	r1, [r4, #16]
300080ba:	fbb2 f3f3 	udiv	r3, r2, r3
300080be:	6223      	str	r3, [r4, #32]
300080c0:	6823      	ldr	r3, [r4, #0]
300080c2:	4a30      	ldr	r2, [pc, #192]	; (30008184 <PSRAM_INFO_Update+0x104>)
300080c4:	2b01      	cmp	r3, #1
300080c6:	68e3      	ldr	r3, [r4, #12]
300080c8:	d128      	bne.n	3000811c <PSRAM_INFO_Update+0x9c>
300080ca:	4293      	cmp	r3, r2
300080cc:	d80c      	bhi.n	300080e8 <PSRAM_INFO_Update+0x68>
300080ce:	2304      	movs	r3, #4
300080d0:	61a3      	str	r3, [r4, #24]
300080d2:	230f      	movs	r3, #15
300080d4:	61e3      	str	r3, [r4, #28]
300080d6:	4b2c      	ldr	r3, [pc, #176]	; (30008188 <PSRAM_INFO_Update+0x108>)
300080d8:	2004      	movs	r0, #4
300080da:	4a2c      	ldr	r2, [pc, #176]	; (3000818c <PSRAM_INFO_Update+0x10c>)
300080dc:	4619      	mov	r1, r3
300080de:	9500      	str	r5, [sp, #0]
300080e0:	f002 fee2 	bl	3000aea8 <rtk_log_write>
300080e4:	b003      	add	sp, #12
300080e6:	bd30      	pop	{r4, r5, pc}
300080e8:	4a29      	ldr	r2, [pc, #164]	; (30008190 <PSRAM_INFO_Update+0x110>)
300080ea:	4293      	cmp	r3, r2
300080ec:	d801      	bhi.n	300080f2 <PSRAM_INFO_Update+0x72>
300080ee:	2305      	movs	r3, #5
300080f0:	e7ee      	b.n	300080d0 <PSRAM_INFO_Update+0x50>
300080f2:	4a28      	ldr	r2, [pc, #160]	; (30008194 <PSRAM_INFO_Update+0x114>)
300080f4:	4293      	cmp	r3, r2
300080f6:	d803      	bhi.n	30008100 <PSRAM_INFO_Update+0x80>
300080f8:	2306      	movs	r3, #6
300080fa:	61a3      	str	r3, [r4, #24]
300080fc:	2312      	movs	r3, #18
300080fe:	e7e9      	b.n	300080d4 <PSRAM_INFO_Update+0x54>
30008100:	4a25      	ldr	r2, [pc, #148]	; (30008198 <PSRAM_INFO_Update+0x118>)
30008102:	4293      	cmp	r3, r2
30008104:	d803      	bhi.n	3000810e <PSRAM_INFO_Update+0x8e>
30008106:	2307      	movs	r3, #7
30008108:	61a3      	str	r3, [r4, #24]
3000810a:	2314      	movs	r3, #20
3000810c:	e7e2      	b.n	300080d4 <PSRAM_INFO_Update+0x54>
3000810e:	4a23      	ldr	r2, [pc, #140]	; (3000819c <PSRAM_INFO_Update+0x11c>)
30008110:	4293      	cmp	r3, r2
30008112:	d8e0      	bhi.n	300080d6 <PSRAM_INFO_Update+0x56>
30008114:	2308      	movs	r3, #8
30008116:	61a3      	str	r3, [r4, #24]
30008118:	231c      	movs	r3, #28
3000811a:	e7db      	b.n	300080d4 <PSRAM_INFO_Update+0x54>
3000811c:	4293      	cmp	r3, r2
3000811e:	d808      	bhi.n	30008132 <PSRAM_INFO_Update+0xb2>
30008120:	2304      	movs	r3, #4
30008122:	61a3      	str	r3, [r4, #24]
30008124:	2308      	movs	r3, #8
30008126:	61e3      	str	r3, [r4, #28]
30008128:	69a2      	ldr	r2, [r4, #24]
3000812a:	4b1d      	ldr	r3, [pc, #116]	; (300081a0 <PSRAM_INFO_Update+0x120>)
3000812c:	f883 2110 	strb.w	r2, [r3, #272]	; 0x110
30008130:	e7d1      	b.n	300080d6 <PSRAM_INFO_Update+0x56>
30008132:	4a17      	ldr	r2, [pc, #92]	; (30008190 <PSRAM_INFO_Update+0x110>)
30008134:	4293      	cmp	r3, r2
30008136:	d801      	bhi.n	3000813c <PSRAM_INFO_Update+0xbc>
30008138:	2305      	movs	r3, #5
3000813a:	e7f2      	b.n	30008122 <PSRAM_INFO_Update+0xa2>
3000813c:	4a15      	ldr	r2, [pc, #84]	; (30008194 <PSRAM_INFO_Update+0x114>)
3000813e:	4293      	cmp	r3, r2
30008140:	d802      	bhi.n	30008148 <PSRAM_INFO_Update+0xc8>
30008142:	2306      	movs	r3, #6
30008144:	61a3      	str	r3, [r4, #24]
30008146:	e7ee      	b.n	30008126 <PSRAM_INFO_Update+0xa6>
30008148:	4a13      	ldr	r2, [pc, #76]	; (30008198 <PSRAM_INFO_Update+0x118>)
3000814a:	4293      	cmp	r3, r2
3000814c:	d803      	bhi.n	30008156 <PSRAM_INFO_Update+0xd6>
3000814e:	2307      	movs	r3, #7
30008150:	61a3      	str	r3, [r4, #24]
30008152:	2306      	movs	r3, #6
30008154:	e7e7      	b.n	30008126 <PSRAM_INFO_Update+0xa6>
30008156:	4a11      	ldr	r2, [pc, #68]	; (3000819c <PSRAM_INFO_Update+0x11c>)
30008158:	4293      	cmp	r3, r2
3000815a:	d8e5      	bhi.n	30008128 <PSRAM_INFO_Update+0xa8>
3000815c:	2804      	cmp	r0, #4
3000815e:	d0f6      	beq.n	3000814e <PSRAM_INFO_Update+0xce>
30008160:	2803      	cmp	r0, #3
30008162:	d1e1      	bne.n	30008128 <PSRAM_INFO_Update+0xa8>
30008164:	61a1      	str	r1, [r4, #24]
30008166:	e7f4      	b.n	30008152 <PSRAM_INFO_Update+0xd2>
30008168:	42008000 	.word	0x42008000
3000816c:	3000de10 	.word	0x3000de10
30008170:	000f4240 	.word	0x000f4240
30008174:	3000eb50 	.word	0x3000eb50
30008178:	300072e9 	.word	0x300072e9
3000817c:	300072b5 	.word	0x300072b5
30008180:	3b9aca00 	.word	0x3b9aca00
30008184:	0632ea00 	.word	0x0632ea00
30008188:	3000bba8 	.word	0x3000bba8
3000818c:	3000bbae 	.word	0x3000bbae
30008190:	07ed6b40 	.word	0x07ed6b40
30008194:	09e4f580 	.word	0x09e4f580
30008198:	0bebc200 	.word	0x0bebc200
3000819c:	0ee6b280 	.word	0x0ee6b280
300081a0:	23020000 	.word	0x23020000

300081a4 <PSRAM_PHY_StructInit>:
300081a4:	2301      	movs	r3, #1
300081a6:	2202      	movs	r2, #2
300081a8:	e9c0 3200 	strd	r3, r2, [r0]
300081ac:	2203      	movs	r2, #3
300081ae:	e9c0 2202 	strd	r2, r2, [r0, #8]
300081b2:	220a      	movs	r2, #10
300081b4:	e9c0 2304 	strd	r2, r3, [r0, #16]
300081b8:	2307      	movs	r3, #7
300081ba:	6183      	str	r3, [r0, #24]
300081bc:	4770      	bx	lr
	...

300081c0 <PSRAM_PHY_Init>:
300081c0:	b538      	push	{r3, r4, r5, lr}
300081c2:	6903      	ldr	r3, [r0, #16]
300081c4:	4605      	mov	r5, r0
300081c6:	6802      	ldr	r2, [r0, #0]
300081c8:	f003 031f 	and.w	r3, r3, #31
300081cc:	4c13      	ldr	r4, [pc, #76]	; (3000821c <PSRAM_PHY_Init+0x5c>)
300081ce:	ea43 7302 	orr.w	r3, r3, r2, lsl #28
300081d2:	6842      	ldr	r2, [r0, #4]
300081d4:	0612      	lsls	r2, r2, #24
300081d6:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
300081da:	4313      	orrs	r3, r2
300081dc:	6882      	ldr	r2, [r0, #8]
300081de:	0412      	lsls	r2, r2, #16
300081e0:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
300081e4:	4313      	orrs	r3, r2
300081e6:	68c2      	ldr	r2, [r0, #12]
300081e8:	0212      	lsls	r2, r2, #8
300081ea:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
300081ee:	4313      	orrs	r3, r2
300081f0:	6063      	str	r3, [r4, #4]
300081f2:	4b0b      	ldr	r3, [pc, #44]	; (30008220 <PSRAM_PHY_Init+0x60>)
300081f4:	4798      	blx	r3
300081f6:	2801      	cmp	r0, #1
300081f8:	e9d5 3105 	ldrd	r3, r1, [r5, #20]
300081fc:	ea4f 1303 	mov.w	r3, r3, lsl #4
30008200:	f001 010f 	and.w	r1, r1, #15
30008204:	b2db      	uxtb	r3, r3
30008206:	ea43 0301 	orr.w	r3, r3, r1
3000820a:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
3000820e:	bf0c      	ite	eq
30008210:	f443 6360 	orreq.w	r3, r3, #3584	; 0xe00
30008214:	f443 6370 	orrne.w	r3, r3, #3840	; 0xf00
30008218:	61a3      	str	r3, [r4, #24]
3000821a:	bd38      	pop	{r3, r4, r5, pc}
3000821c:	41002000 	.word	0x41002000
30008220:	300072e9 	.word	0x300072e9

30008224 <PSRAM_CTRL_Init>:
30008224:	4b50      	ldr	r3, [pc, #320]	; (30008368 <PSRAM_CTRL_Init+0x144>)
30008226:	2200      	movs	r2, #0
30008228:	4950      	ldr	r1, [pc, #320]	; (3000836c <PSRAM_CTRL_Init+0x148>)
3000822a:	b530      	push	{r4, r5, lr}
3000822c:	609a      	str	r2, [r3, #8]
3000822e:	4a50      	ldr	r2, [pc, #320]	; (30008370 <PSRAM_CTRL_Init+0x14c>)
30008230:	601a      	str	r2, [r3, #0]
30008232:	2201      	movs	r2, #1
30008234:	615a      	str	r2, [r3, #20]
30008236:	f24d 0201 	movw	r2, #53249	; 0xd001
3000823a:	f8c3 2120 	str.w	r2, [r3, #288]	; 0x120
3000823e:	6808      	ldr	r0, [r1, #0]
30008240:	2802      	cmp	r0, #2
30008242:	d15a      	bne.n	300082fa <PSRAM_CTRL_Init+0xd6>
30008244:	f245 5251 	movw	r2, #21841	; 0x5551
30008248:	247d      	movs	r4, #125	; 0x7d
3000824a:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
3000824e:	690a      	ldr	r2, [r1, #16]
30008250:	f002 020f 	and.w	r2, r2, #15
30008254:	f442 52e3 	orr.w	r2, r2, #7264	; 0x1c60
30008258:	f042 0210 	orr.w	r2, r2, #16
3000825c:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008260:	694d      	ldr	r5, [r1, #20]
30008262:	69ca      	ldr	r2, [r1, #28]
30008264:	fbb2 f2f5 	udiv	r2, r2, r5
30008268:	fbb4 f5f5 	udiv	r5, r4, r5
3000826c:	f002 043f 	and.w	r4, r2, #63	; 0x3f
30008270:	0192      	lsls	r2, r2, #6
30008272:	ea44 6405 	orr.w	r4, r4, r5, lsl #24
30008276:	f402 627c 	and.w	r2, r2, #4032	; 0xfc0
3000827a:	4322      	orrs	r2, r4
3000827c:	f442 1280 	orr.w	r2, r2, #1048576	; 0x100000
30008280:	f8c3 2138 	str.w	r2, [r3, #312]	; 0x138
30008284:	22a0      	movs	r2, #160	; 0xa0
30008286:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000828a:	2220      	movs	r2, #32
3000828c:	f8c3 20f4 	str.w	r2, [r3, #244]	; 0xf4
30008290:	f44f 22a0 	mov.w	r2, #327680	; 0x50000
30008294:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
30008298:	698a      	ldr	r2, [r1, #24]
3000829a:	0092      	lsls	r2, r2, #2
3000829c:	3a01      	subs	r2, #1
3000829e:	f3c2 020b 	ubfx	r2, r2, #0, #12
300082a2:	f442 22a2 	orr.w	r2, r2, #331776	; 0x51000
300082a6:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
300082aa:	2802      	cmp	r0, #2
300082ac:	698a      	ldr	r2, [r1, #24]
300082ae:	4c2e      	ldr	r4, [pc, #184]	; (30008368 <PSRAM_CTRL_Init+0x144>)
300082b0:	ea4f 0182 	mov.w	r1, r2, lsl #2
300082b4:	f8d4 511c 	ldr.w	r5, [r4, #284]	; 0x11c
300082b8:	f101 33ff 	add.w	r3, r1, #4294967295	; 0xffffffff
300082bc:	bf08      	it	eq
300082be:	3902      	subeq	r1, #2
300082c0:	f3c3 030b 	ubfx	r3, r3, #0, #12
300082c4:	bf08      	it	eq
300082c6:	f3c1 010b 	ubfxeq	r1, r1, #0, #12
300082ca:	ea43 0305 	orr.w	r3, r3, r5
300082ce:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
300082d2:	f8c4 311c 	str.w	r3, [r4, #284]	; 0x11c
300082d6:	bf1b      	ittet	ne
300082d8:	f102 4300 	addne.w	r3, r2, #2147483648	; 0x80000000
300082dc:	f640 72fe 	movwne	r2, #4094	; 0xffe
300082e0:	f8c4 113c 	streq.w	r1, [r4, #316]	; 0x13c
300082e4:	f103 33ff 	addne.w	r3, r3, #4294967295	; 0xffffffff
300082e8:	bf1c      	itt	ne
300082ea:	ea02 0343 	andne.w	r3, r2, r3, lsl #1
300082ee:	f8c4 313c 	strne.w	r3, [r4, #316]	; 0x13c
300082f2:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300082f4:	061b      	lsls	r3, r3, #24
300082f6:	d5fc      	bpl.n	300082f2 <PSRAM_CTRL_Init+0xce>
300082f8:	bd30      	pop	{r4, r5, pc}
300082fa:	f241 5251 	movw	r2, #5457	; 0x1551
300082fe:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30008302:	690a      	ldr	r2, [r1, #16]
30008304:	f002 020f 	and.w	r2, r2, #15
30008308:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
3000830c:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008310:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
30008314:	694d      	ldr	r5, [r1, #20]
30008316:	69cc      	ldr	r4, [r1, #28]
30008318:	fbb2 f2f5 	udiv	r2, r2, r5
3000831c:	fbb4 f4f5 	udiv	r4, r4, r5
30008320:	0955      	lsrs	r5, r2, #5
30008322:	01a2      	lsls	r2, r4, #6
30008324:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008328:	f402 627c 	and.w	r2, r2, #4032	; 0xfc0
3000832c:	ea42 6205 	orr.w	r2, r2, r5, lsl #24
30008330:	4322      	orrs	r2, r4
30008332:	f442 1283 	orr.w	r2, r2, #1073152	; 0x106000
30008336:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000833a:	f8c3 2138 	str.w	r2, [r3, #312]	; 0x138
3000833e:	f242 0220 	movw	r2, #8224	; 0x2020
30008342:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
30008346:	f24a 02a0 	movw	r2, #41120	; 0xa0a0
3000834a:	f8c3 20f4 	str.w	r2, [r3, #244]	; 0xf4
3000834e:	f44f 2280 	mov.w	r2, #262144	; 0x40000
30008352:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
30008356:	698a      	ldr	r2, [r1, #24]
30008358:	0092      	lsls	r2, r2, #2
3000835a:	3a01      	subs	r2, #1
3000835c:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008360:	f442 2284 	orr.w	r2, r2, #270336	; 0x42000
30008364:	e79f      	b.n	300082a6 <PSRAM_CTRL_Init+0x82>
30008366:	bf00      	nop
30008368:	41012000 	.word	0x41012000
3000836c:	3000eb50 	.word	0x3000eb50
30008370:	00bfe000 	.word	0x00bfe000

30008374 <PSRAM_REG_Write>:
30008374:	4b1e      	ldr	r3, [pc, #120]	; (300083f0 <PSRAM_REG_Write+0x7c>)
30008376:	b530      	push	{r4, r5, lr}
30008378:	6a9c      	ldr	r4, [r3, #40]	; 0x28
3000837a:	f014 0401 	ands.w	r4, r4, #1
3000837e:	d1fb      	bne.n	30008378 <PSRAM_REG_Write+0x4>
30008380:	681d      	ldr	r5, [r3, #0]
30008382:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008386:	601d      	str	r5, [r3, #0]
30008388:	609c      	str	r4, [r3, #8]
3000838a:	681d      	ldr	r5, [r3, #0]
3000838c:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008390:	601d      	str	r5, [r3, #0]
30008392:	605c      	str	r4, [r3, #4]
30008394:	f021 447f 	bic.w	r4, r1, #4278190080	; 0xff000000
30008398:	4411      	add	r1, r2
3000839a:	f8c3 4130 	str.w	r4, [r3, #304]	; 0x130
3000839e:	24c0      	movs	r4, #192	; 0xc0
300083a0:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300083a4:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300083a8:	0e04      	lsrs	r4, r0, #24
300083aa:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300083ae:	f3c0 4407 	ubfx	r4, r0, #16, #8
300083b2:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300083b6:	f3c0 2407 	ubfx	r4, r0, #8, #8
300083ba:	b2c0      	uxtb	r0, r0
300083bc:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
300083c0:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
300083c4:	4b0a      	ldr	r3, [pc, #40]	; (300083f0 <PSRAM_REG_Write+0x7c>)
300083c6:	428a      	cmp	r2, r1
300083c8:	d10d      	bne.n	300083e6 <PSRAM_REG_Write+0x72>
300083ca:	2201      	movs	r2, #1
300083cc:	609a      	str	r2, [r3, #8]
300083ce:	689a      	ldr	r2, [r3, #8]
300083d0:	07d1      	lsls	r1, r2, #31
300083d2:	d4fc      	bmi.n	300083ce <PSRAM_REG_Write+0x5a>
300083d4:	4b06      	ldr	r3, [pc, #24]	; (300083f0 <PSRAM_REG_Write+0x7c>)
300083d6:	6a9a      	ldr	r2, [r3, #40]	; 0x28
300083d8:	07d2      	lsls	r2, r2, #31
300083da:	d4fc      	bmi.n	300083d6 <PSRAM_REG_Write+0x62>
300083dc:	681a      	ldr	r2, [r3, #0]
300083de:	f022 4200 	bic.w	r2, r2, #2147483648	; 0x80000000
300083e2:	601a      	str	r2, [r3, #0]
300083e4:	bd30      	pop	{r4, r5, pc}
300083e6:	f812 0b01 	ldrb.w	r0, [r2], #1
300083ea:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
300083ee:	e7ea      	b.n	300083c6 <PSRAM_REG_Write+0x52>
300083f0:	41012000 	.word	0x41012000

300083f4 <PSRAM_APM_DEVIC_Init>:
300083f4:	4b10      	ldr	r3, [pc, #64]	; (30008438 <PSRAM_APM_DEVIC_Init+0x44>)
300083f6:	4a11      	ldr	r2, [pc, #68]	; (3000843c <PSRAM_APM_DEVIC_Init+0x48>)
300083f8:	b513      	push	{r0, r1, r4, lr}
300083fa:	6999      	ldr	r1, [r3, #24]
300083fc:	2000      	movs	r0, #0
300083fe:	4c10      	ldr	r4, [pc, #64]	; (30008440 <PSRAM_APM_DEVIC_Init+0x4c>)
30008400:	440a      	add	r2, r1
30008402:	1ecb      	subs	r3, r1, #3
30008404:	2102      	movs	r1, #2
30008406:	f812 2c03 	ldrb.w	r2, [r2, #-3]
3000840a:	009b      	lsls	r3, r3, #2
3000840c:	0152      	lsls	r2, r2, #5
3000840e:	f003 031c 	and.w	r3, r3, #28
30008412:	b2d2      	uxtb	r2, r2
30008414:	f043 0320 	orr.w	r3, r3, #32
30008418:	f88d 2004 	strb.w	r2, [sp, #4]
3000841c:	f88d 2005 	strb.w	r2, [sp, #5]
30008420:	466a      	mov	r2, sp
30008422:	f88d 3000 	strb.w	r3, [sp]
30008426:	f88d 3001 	strb.w	r3, [sp, #1]
3000842a:	47a0      	blx	r4
3000842c:	aa01      	add	r2, sp, #4
3000842e:	2102      	movs	r1, #2
30008430:	2004      	movs	r0, #4
30008432:	47a0      	blx	r4
30008434:	b002      	add	sp, #8
30008436:	bd10      	pop	{r4, pc}
30008438:	3000eb50 	.word	0x3000eb50
3000843c:	3000dfd8 	.word	0x3000dfd8
30008440:	30008375 	.word	0x30008375

30008444 <PSRAM_WB_REG_Write>:
30008444:	4b1c      	ldr	r3, [pc, #112]	; (300084b8 <PSRAM_WB_REG_Write+0x74>)
30008446:	b530      	push	{r4, r5, lr}
30008448:	6a9c      	ldr	r4, [r3, #40]	; 0x28
3000844a:	f014 0401 	ands.w	r4, r4, #1
3000844e:	d1fb      	bne.n	30008448 <PSRAM_WB_REG_Write+0x4>
30008450:	681d      	ldr	r5, [r3, #0]
30008452:	b2c0      	uxtb	r0, r0
30008454:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008458:	601d      	str	r5, [r3, #0]
3000845a:	609c      	str	r4, [r3, #8]
3000845c:	681d      	ldr	r5, [r3, #0]
3000845e:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008462:	601d      	str	r5, [r3, #0]
30008464:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30008468:	605c      	str	r4, [r3, #4]
3000846a:	4411      	add	r1, r2
3000846c:	f8c3 5130 	str.w	r5, [r3, #304]	; 0x130
30008470:	2560      	movs	r5, #96	; 0x60
30008472:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30008476:	2501      	movs	r5, #1
30008478:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
3000847c:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30008480:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
30008484:	f883 4060 	strb.w	r4, [r3, #96]	; 0x60
30008488:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
3000848c:	4b0a      	ldr	r3, [pc, #40]	; (300084b8 <PSRAM_WB_REG_Write+0x74>)
3000848e:	428a      	cmp	r2, r1
30008490:	d10d      	bne.n	300084ae <PSRAM_WB_REG_Write+0x6a>
30008492:	2201      	movs	r2, #1
30008494:	609a      	str	r2, [r3, #8]
30008496:	689a      	ldr	r2, [r3, #8]
30008498:	07d1      	lsls	r1, r2, #31
3000849a:	d4fc      	bmi.n	30008496 <PSRAM_WB_REG_Write+0x52>
3000849c:	4b06      	ldr	r3, [pc, #24]	; (300084b8 <PSRAM_WB_REG_Write+0x74>)
3000849e:	6a9a      	ldr	r2, [r3, #40]	; 0x28
300084a0:	07d2      	lsls	r2, r2, #31
300084a2:	d4fc      	bmi.n	3000849e <PSRAM_WB_REG_Write+0x5a>
300084a4:	681a      	ldr	r2, [r3, #0]
300084a6:	f022 4200 	bic.w	r2, r2, #2147483648	; 0x80000000
300084aa:	601a      	str	r2, [r3, #0]
300084ac:	bd30      	pop	{r4, r5, pc}
300084ae:	f812 0b01 	ldrb.w	r0, [r2], #1
300084b2:	f883 0060 	strb.w	r0, [r3, #96]	; 0x60
300084b6:	e7ea      	b.n	3000848e <PSRAM_WB_REG_Write+0x4a>
300084b8:	41012000 	.word	0x41012000

300084bc <PSRAM_WB_DEVIC_Init>:
300084bc:	238f      	movs	r3, #143	; 0x8f
300084be:	b507      	push	{r0, r1, r2, lr}
300084c0:	f88d 3004 	strb.w	r3, [sp, #4]
300084c4:	2102      	movs	r1, #2
300084c6:	4b09      	ldr	r3, [pc, #36]	; (300084ec <PSRAM_WB_DEVIC_Init+0x30>)
300084c8:	2000      	movs	r0, #0
300084ca:	699a      	ldr	r2, [r3, #24]
300084cc:	4b08      	ldr	r3, [pc, #32]	; (300084f0 <PSRAM_WB_DEVIC_Init+0x34>)
300084ce:	4413      	add	r3, r2
300084d0:	aa01      	add	r2, sp, #4
300084d2:	f813 3c03 	ldrb.w	r3, [r3, #-3]
300084d6:	011b      	lsls	r3, r3, #4
300084d8:	f043 030c 	orr.w	r3, r3, #12
300084dc:	f88d 3005 	strb.w	r3, [sp, #5]
300084e0:	4b04      	ldr	r3, [pc, #16]	; (300084f4 <PSRAM_WB_DEVIC_Init+0x38>)
300084e2:	4798      	blx	r3
300084e4:	b003      	add	sp, #12
300084e6:	f85d fb04 	ldr.w	pc, [sp], #4
300084ea:	bf00      	nop
300084ec:	3000eb50 	.word	0x3000eb50
300084f0:	3000dff8 	.word	0x3000dff8
300084f4:	30008445 	.word	0x30008445

300084f8 <PSRAM_calibration>:
300084f8:	4b9b      	ldr	r3, [pc, #620]	; (30008768 <PSRAM_calibration+0x270>)
300084fa:	2218      	movs	r2, #24
300084fc:	2100      	movs	r1, #0
300084fe:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008502:	b091      	sub	sp, #68	; 0x44
30008504:	685b      	ldr	r3, [r3, #4]
30008506:	9308      	str	r3, [sp, #32]
30008508:	f023 4370 	bic.w	r3, r3, #4026531840	; 0xf0000000
3000850c:	a80a      	add	r0, sp, #40	; 0x28
3000850e:	f023 031f 	bic.w	r3, r3, #31
30008512:	9307      	str	r3, [sp, #28]
30008514:	f002 ff2c 	bl	3000b370 <____wrap_memset_veneer>
30008518:	4a94      	ldr	r2, [pc, #592]	; (3000876c <PSRAM_calibration+0x274>)
3000851a:	6953      	ldr	r3, [r2, #20]
3000851c:	03d9      	lsls	r1, r3, #15
3000851e:	d51e      	bpl.n	3000855e <PSRAM_calibration+0x66>
30008520:	2300      	movs	r3, #0
30008522:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30008526:	f3bf 8f4f 	dsb	sy
3000852a:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000852e:	f643 74e0 	movw	r4, #16352	; 0x3fe0
30008532:	f3c3 00c9 	ubfx	r0, r3, #3, #10
30008536:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000853a:	015b      	lsls	r3, r3, #5
3000853c:	ea03 0604 	and.w	r6, r3, r4
30008540:	4601      	mov	r1, r0
30008542:	ea46 7581 	orr.w	r5, r6, r1, lsl #30
30008546:	3901      	subs	r1, #1
30008548:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
3000854c:	d2f9      	bcs.n	30008542 <PSRAM_calibration+0x4a>
3000854e:	3b20      	subs	r3, #32
30008550:	f113 0f20 	cmn.w	r3, #32
30008554:	d1f2      	bne.n	3000853c <PSRAM_calibration+0x44>
30008556:	f3bf 8f4f 	dsb	sy
3000855a:	f3bf 8f6f 	isb	sy
3000855e:	4a83      	ldr	r2, [pc, #524]	; (3000876c <PSRAM_calibration+0x274>)
30008560:	2300      	movs	r3, #0
30008562:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30008566:	f3bf 8f4f 	dsb	sy
3000856a:	6953      	ldr	r3, [r2, #20]
3000856c:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30008570:	6153      	str	r3, [r2, #20]
30008572:	f3bf 8f4f 	dsb	sy
30008576:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000857a:	f643 70e0 	movw	r0, #16352	; 0x3fe0
3000857e:	f3c3 01c9 	ubfx	r1, r3, #3, #10
30008582:	f3c3 334e 	ubfx	r3, r3, #13, #15
30008586:	015b      	lsls	r3, r3, #5
30008588:	ea03 0600 	and.w	r6, r3, r0
3000858c:	460c      	mov	r4, r1
3000858e:	ea46 7584 	orr.w	r5, r6, r4, lsl #30
30008592:	3c01      	subs	r4, #1
30008594:	f8c2 5274 	str.w	r5, [r2, #628]	; 0x274
30008598:	d2f9      	bcs.n	3000858e <PSRAM_calibration+0x96>
3000859a:	3b20      	subs	r3, #32
3000859c:	f113 0f20 	cmn.w	r3, #32
300085a0:	d1f2      	bne.n	30008588 <PSRAM_calibration+0x90>
300085a2:	f3bf 8f4f 	dsb	sy
300085a6:	f3bf 8f6f 	isb	sy
300085aa:	4a6f      	ldr	r2, [pc, #444]	; (30008768 <PSRAM_calibration+0x270>)
300085ac:	f04f 0800 	mov.w	r8, #0
300085b0:	f04f 0b01 	mov.w	fp, #1
300085b4:	f8df a1c0 	ldr.w	sl, [pc, #448]	; 30008778 <PSRAM_calibration+0x280>
300085b8:	6813      	ldr	r3, [r2, #0]
300085ba:	f023 0301 	bic.w	r3, r3, #1
300085be:	6013      	str	r3, [r2, #0]
300085c0:	2304      	movs	r3, #4
300085c2:	9404      	str	r4, [sp, #16]
300085c4:	9306      	str	r3, [sp, #24]
300085c6:	9405      	str	r4, [sp, #20]
300085c8:	9b07      	ldr	r3, [sp, #28]
300085ca:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
300085ce:	2500      	movs	r5, #0
300085d0:	ea43 730b 	orr.w	r3, r3, fp, lsl #28
300085d4:	463e      	mov	r6, r7
300085d6:	46a9      	mov	r9, r5
300085d8:	9309      	str	r3, [sp, #36]	; 0x24
300085da:	9b09      	ldr	r3, [sp, #36]	; 0x24
300085dc:	f04f 40c0 	mov.w	r0, #1610612736	; 0x60000000
300085e0:	4a61      	ldr	r2, [pc, #388]	; (30008768 <PSRAM_calibration+0x270>)
300085e2:	ea43 0309 	orr.w	r3, r3, r9
300085e6:	f8df e1a0 	ldr.w	lr, [pc, #416]	; 30008788 <PSRAM_calibration+0x290>
300085ea:	f8df c1a0 	ldr.w	ip, [pc, #416]	; 3000878c <PSRAM_calibration+0x294>
300085ee:	6053      	str	r3, [r2, #4]
300085f0:	f8da 3000 	ldr.w	r3, [sl]
300085f4:	4a5e      	ldr	r2, [pc, #376]	; (30008770 <PSRAM_calibration+0x278>)
300085f6:	6003      	str	r3, [r0, #0]
300085f8:	f8da 3004 	ldr.w	r3, [sl, #4]
300085fc:	6013      	str	r3, [r2, #0]
300085fe:	f502 2230 	add.w	r2, r2, #720896	; 0xb0000
30008602:	f8da 3008 	ldr.w	r3, [sl, #8]
30008606:	6013      	str	r3, [r2, #0]
30008608:	f8da 100c 	ldr.w	r1, [sl, #12]
3000860c:	f8ce 1000 	str.w	r1, [lr]
30008610:	f8da 1010 	ldr.w	r1, [sl, #16]
30008614:	f8cc 1000 	str.w	r1, [ip]
30008618:	4956      	ldr	r1, [pc, #344]	; (30008774 <PSRAM_calibration+0x27c>)
3000861a:	f8da 3014 	ldr.w	r3, [sl, #20]
3000861e:	600b      	str	r3, [r1, #0]
30008620:	6800      	ldr	r0, [r0, #0]
30008622:	900a      	str	r0, [sp, #40]	; 0x28
30008624:	4852      	ldr	r0, [pc, #328]	; (30008770 <PSRAM_calibration+0x278>)
30008626:	6800      	ldr	r0, [r0, #0]
30008628:	900b      	str	r0, [sp, #44]	; 0x2c
3000862a:	a80a      	add	r0, sp, #40	; 0x28
3000862c:	6812      	ldr	r2, [r2, #0]
3000862e:	920c      	str	r2, [sp, #48]	; 0x30
30008630:	2218      	movs	r2, #24
30008632:	f8de 3000 	ldr.w	r3, [lr]
30008636:	930d      	str	r3, [sp, #52]	; 0x34
30008638:	f8dc 3000 	ldr.w	r3, [ip]
3000863c:	930e      	str	r3, [sp, #56]	; 0x38
3000863e:	680b      	ldr	r3, [r1, #0]
30008640:	494d      	ldr	r1, [pc, #308]	; (30008778 <PSRAM_calibration+0x280>)
30008642:	930f      	str	r3, [sp, #60]	; 0x3c
30008644:	4b4d      	ldr	r3, [pc, #308]	; (3000877c <PSRAM_calibration+0x284>)
30008646:	4798      	blx	r3
30008648:	2800      	cmp	r0, #0
3000864a:	d17d      	bne.n	30008748 <PSRAM_calibration+0x250>
3000864c:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
30008650:	bf08      	it	eq
30008652:	464e      	moveq	r6, r9
30008654:	f1b9 0f1f 	cmp.w	r9, #31
30008658:	eb06 0705 	add.w	r7, r6, r5
3000865c:	f105 0501 	add.w	r5, r5, #1
30008660:	d105      	bne.n	3000866e <PSRAM_calibration+0x176>
30008662:	4545      	cmp	r5, r8
30008664:	dd03      	ble.n	3000866e <PSRAM_calibration+0x176>
30008666:	465c      	mov	r4, fp
30008668:	46a8      	mov	r8, r5
3000866a:	e9cd 7604 	strd	r7, r6, [sp, #16]
3000866e:	f109 0901 	add.w	r9, r9, #1
30008672:	f1b9 0f20 	cmp.w	r9, #32
30008676:	d1b0      	bne.n	300085da <PSRAM_calibration+0xe2>
30008678:	9b06      	ldr	r3, [sp, #24]
3000867a:	ea4f 0b4b 	mov.w	fp, fp, lsl #1
3000867e:	3b01      	subs	r3, #1
30008680:	9306      	str	r3, [sp, #24]
30008682:	d1a1      	bne.n	300085c8 <PSRAM_calibration+0xd0>
30008684:	9b04      	ldr	r3, [sp, #16]
30008686:	2004      	movs	r0, #4
30008688:	4a3d      	ldr	r2, [pc, #244]	; (30008780 <PSRAM_calibration+0x288>)
3000868a:	9301      	str	r3, [sp, #4]
3000868c:	9b05      	ldr	r3, [sp, #20]
3000868e:	9300      	str	r3, [sp, #0]
30008690:	4b3c      	ldr	r3, [pc, #240]	; (30008784 <PSRAM_calibration+0x28c>)
30008692:	e9cd 8402 	strd	r8, r4, [sp, #8]
30008696:	4619      	mov	r1, r3
30008698:	f002 fc06 	bl	3000aea8 <rtk_log_write>
3000869c:	4a33      	ldr	r2, [pc, #204]	; (3000876c <PSRAM_calibration+0x274>)
3000869e:	6953      	ldr	r3, [r2, #20]
300086a0:	03db      	lsls	r3, r3, #15
300086a2:	d424      	bmi.n	300086ee <PSRAM_calibration+0x1f6>
300086a4:	9b06      	ldr	r3, [sp, #24]
300086a6:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300086aa:	f3bf 8f4f 	dsb	sy
300086ae:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
300086b2:	f643 75e0 	movw	r5, #16352	; 0x3fe0
300086b6:	f3c3 00c9 	ubfx	r0, r3, #3, #10
300086ba:	f3c3 334e 	ubfx	r3, r3, #13, #15
300086be:	015b      	lsls	r3, r3, #5
300086c0:	ea03 0705 	and.w	r7, r3, r5
300086c4:	4601      	mov	r1, r0
300086c6:	ea47 7681 	orr.w	r6, r7, r1, lsl #30
300086ca:	3901      	subs	r1, #1
300086cc:	f8c2 6260 	str.w	r6, [r2, #608]	; 0x260
300086d0:	d2f9      	bcs.n	300086c6 <PSRAM_calibration+0x1ce>
300086d2:	3b20      	subs	r3, #32
300086d4:	f113 0f20 	cmn.w	r3, #32
300086d8:	d1f2      	bne.n	300086c0 <PSRAM_calibration+0x1c8>
300086da:	f3bf 8f4f 	dsb	sy
300086de:	6953      	ldr	r3, [r2, #20]
300086e0:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300086e4:	6153      	str	r3, [r2, #20]
300086e6:	f3bf 8f4f 	dsb	sy
300086ea:	f3bf 8f6f 	isb	sy
300086ee:	f1b8 0f08 	cmp.w	r8, #8
300086f2:	dd37      	ble.n	30008764 <PSRAM_calibration+0x26c>
300086f4:	2001      	movs	r0, #1
300086f6:	e9dd 3204 	ldrd	r3, r2, [sp, #16]
300086fa:	1a9b      	subs	r3, r3, r2
300086fc:	e9dd 2104 	ldrd	r2, r1, [sp, #16]
30008700:	eb03 73d3 	add.w	r3, r3, r3, lsr #31
30008704:	eb02 0b01 	add.w	fp, r2, r1
30008708:	9a08      	ldr	r2, [sp, #32]
3000870a:	105b      	asrs	r3, r3, #1
3000870c:	f002 627f 	and.w	r2, r2, #267386880	; 0xff00000
30008710:	eb0b 7bdb 	add.w	fp, fp, fp, lsr #31
30008714:	3b02      	subs	r3, #2
30008716:	f3cb 0b44 	ubfx	fp, fp, #1, #5
3000871a:	ea42 7404 	orr.w	r4, r2, r4, lsl #28
3000871e:	ea4b 0404 	orr.w	r4, fp, r4
30008722:	ea4f 4b03 	mov.w	fp, r3, lsl #16
30008726:	021b      	lsls	r3, r3, #8
30008728:	f40b 2b70 	and.w	fp, fp, #983040	; 0xf0000
3000872c:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
30008730:	ea44 040b 	orr.w	r4, r4, fp
30008734:	431c      	orrs	r4, r3
30008736:	4b0c      	ldr	r3, [pc, #48]	; (30008768 <PSRAM_calibration+0x270>)
30008738:	605c      	str	r4, [r3, #4]
3000873a:	681a      	ldr	r2, [r3, #0]
3000873c:	f042 0201 	orr.w	r2, r2, #1
30008740:	601a      	str	r2, [r3, #0]
30008742:	b011      	add	sp, #68	; 0x44
30008744:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008748:	1c72      	adds	r2, r6, #1
3000874a:	d090      	beq.n	3000866e <PSRAM_calibration+0x176>
3000874c:	45a8      	cmp	r8, r5
3000874e:	bfb8      	it	lt
30008750:	e9cd 7604 	strdlt	r7, r6, [sp, #16]
30008754:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30008758:	bfbc      	itt	lt
3000875a:	46a8      	movlt	r8, r5
3000875c:	465c      	movlt	r4, fp
3000875e:	2500      	movs	r5, #0
30008760:	463e      	mov	r6, r7
30008762:	e784      	b.n	3000866e <PSRAM_calibration+0x176>
30008764:	2000      	movs	r0, #0
30008766:	e7ec      	b.n	30008742 <PSRAM_calibration+0x24a>
30008768:	41002000 	.word	0x41002000
3000876c:	e000ed00 	.word	0xe000ed00
30008770:	60050000 	.word	0x60050000
30008774:	60250000 	.word	0x60250000
30008778:	3000dfe0 	.word	0x3000dfe0
3000877c:	000129bd 	.word	0x000129bd
30008780:	3000bbcd 	.word	0x3000bbcd
30008784:	3000bba8 	.word	0x3000bba8
30008788:	60150000 	.word	0x60150000
3000878c:	60200000 	.word	0x60200000

30008790 <PSRAM_AutoGating>:
30008790:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30008794:	b1a8      	cbz	r0, 300087c2 <PSRAM_AutoGating+0x32>
30008796:	f8d3 01e4 	ldr.w	r0, [r3, #484]	; 0x1e4
3000879a:	f3c1 0108 	ubfx	r1, r1, #0, #9
3000879e:	0252      	lsls	r2, r2, #9
300087a0:	f420 507f 	bic.w	r0, r0, #16320	; 0x3fc0
300087a4:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
300087a8:	f020 003f 	bic.w	r0, r0, #63	; 0x3f
300087ac:	4301      	orrs	r1, r0
300087ae:	430a      	orrs	r2, r1
300087b0:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
300087b4:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
300087b8:	f042 0201 	orr.w	r2, r2, #1
300087bc:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
300087c0:	4770      	bx	lr
300087c2:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
300087c6:	f022 0201 	bic.w	r2, r2, #1
300087ca:	e7f7      	b.n	300087bc <PSRAM_AutoGating+0x2c>

300087cc <BOOT_Reason>:
300087cc:	4b02      	ldr	r3, [pc, #8]	; (300087d8 <BOOT_Reason+0xc>)
300087ce:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
300087d2:	b280      	uxth	r0, r0
300087d4:	4770      	bx	lr
300087d6:	bf00      	nop
300087d8:	42008200 	.word	0x42008200

300087dc <CRYPTO_SetSecurityModeAD>:
300087dc:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
300087e0:	460d      	mov	r5, r1
300087e2:	4616      	mov	r6, r2
300087e4:	4699      	mov	r9, r3
300087e6:	4604      	mov	r4, r0
300087e8:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
300087ec:	b918      	cbnz	r0, 300087f6 <CRYPTO_SetSecurityModeAD+0x1a>
300087ee:	2124      	movs	r1, #36	; 0x24
300087f0:	4852      	ldr	r0, [pc, #328]	; (3000893c <CRYPTO_SetSecurityModeAD+0x160>)
300087f2:	f002 fdad 	bl	3000b350 <__io_assert_failed_veneer>
300087f6:	7823      	ldrb	r3, [r4, #0]
300087f8:	2b01      	cmp	r3, #1
300087fa:	d003      	beq.n	30008804 <CRYPTO_SetSecurityModeAD+0x28>
300087fc:	2125      	movs	r1, #37	; 0x25
300087fe:	484f      	ldr	r0, [pc, #316]	; (3000893c <CRYPTO_SetSecurityModeAD+0x160>)
30008800:	f002 fda6 	bl	3000b350 <__io_assert_failed_veneer>
30008804:	2300      	movs	r3, #0
30008806:	6065      	str	r5, [r4, #4]
30008808:	8123      	strh	r3, [r4, #8]
3000880a:	1c6b      	adds	r3, r5, #1
3000880c:	d00b      	beq.n	30008826 <CRYPTO_SetSecurityModeAD+0x4a>
3000880e:	f005 0330 	and.w	r3, r5, #48	; 0x30
30008812:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30008816:	2b20      	cmp	r3, #32
30008818:	f3c5 15c0 	ubfx	r5, r5, #7, #1
3000881c:	bf08      	it	eq
3000881e:	2301      	moveq	r3, #1
30008820:	7265      	strb	r5, [r4, #9]
30008822:	bf08      	it	eq
30008824:	7223      	strbeq	r3, [r4, #8]
30008826:	1c70      	adds	r0, r6, #1
30008828:	60e6      	str	r6, [r4, #12]
3000882a:	d074      	beq.n	30008916 <CRYPTO_SetSecurityModeAD+0x13a>
3000882c:	f3c6 0380 	ubfx	r3, r6, #2, #1
30008830:	7423      	strb	r3, [r4, #16]
30008832:	f006 0303 	and.w	r3, r6, #3
30008836:	1e99      	subs	r1, r3, #2
30008838:	424a      	negs	r2, r1
3000883a:	414a      	adcs	r2, r1
3000883c:	2b01      	cmp	r3, #1
3000883e:	7462      	strb	r2, [r4, #17]
30008840:	fab3 f283 	clz	r2, r3
30008844:	ea4f 1252 	mov.w	r2, r2, lsr #5
30008848:	74a2      	strb	r2, [r4, #18]
3000884a:	bf0c      	ite	eq
3000884c:	2201      	moveq	r2, #1
3000884e:	2200      	movne	r2, #0
30008850:	74e2      	strb	r2, [r4, #19]
30008852:	d159      	bne.n	30008908 <CRYPTO_SetSecurityModeAD+0x12c>
30008854:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30008858:	2b40      	cmp	r3, #64	; 0x40
3000885a:	d053      	beq.n	30008904 <CRYPTO_SetSecurityModeAD+0x128>
3000885c:	d80c      	bhi.n	30008878 <CRYPTO_SetSecurityModeAD+0x9c>
3000885e:	2b10      	cmp	r3, #16
30008860:	d00e      	beq.n	30008880 <CRYPTO_SetSecurityModeAD+0xa4>
30008862:	2b20      	cmp	r3, #32
30008864:	d00d      	beq.n	30008882 <CRYPTO_SetSecurityModeAD+0xa6>
30008866:	4b36      	ldr	r3, [pc, #216]	; (30008940 <CRYPTO_SetSecurityModeAD+0x164>)
30008868:	2002      	movs	r0, #2
3000886a:	4a36      	ldr	r2, [pc, #216]	; (30008944 <CRYPTO_SetSecurityModeAD+0x168>)
3000886c:	4619      	mov	r1, r3
3000886e:	9600      	str	r6, [sp, #0]
30008870:	f002 fb1a 	bl	3000aea8 <rtk_log_write>
30008874:	2300      	movs	r3, #0
30008876:	e004      	b.n	30008882 <CRYPTO_SetSecurityModeAD+0xa6>
30008878:	2b80      	cmp	r3, #128	; 0x80
3000887a:	d1f4      	bne.n	30008866 <CRYPTO_SetSecurityModeAD+0x8a>
3000887c:	2340      	movs	r3, #64	; 0x40
3000887e:	e000      	b.n	30008882 <CRYPTO_SetSecurityModeAD+0xa6>
30008880:	231c      	movs	r3, #28
30008882:	7523      	strb	r3, [r4, #20]
30008884:	7d23      	ldrb	r3, [r4, #20]
30008886:	62a3      	str	r3, [r4, #40]	; 0x28
30008888:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000888a:	6227      	str	r7, [r4, #32]
3000888c:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30008890:	2f00      	cmp	r7, #0
30008892:	d050      	beq.n	30008936 <CRYPTO_SetSecurityModeAD+0x15a>
30008894:	7d23      	ldrb	r3, [r4, #20]
30008896:	f104 0064 	add.w	r0, r4, #100	; 0x64
3000889a:	4d2b      	ldr	r5, [pc, #172]	; (30008948 <CRYPTO_SetSecurityModeAD+0x16c>)
3000889c:	2b30      	cmp	r3, #48	; 0x30
3000889e:	65e0      	str	r0, [r4, #92]	; 0x5c
300088a0:	d001      	beq.n	300088a6 <CRYPTO_SetSecurityModeAD+0xca>
300088a2:	2b40      	cmp	r3, #64	; 0x40
300088a4:	d13f      	bne.n	30008926 <CRYPTO_SetSecurityModeAD+0x14a>
300088a6:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
300088aa:	2280      	movs	r2, #128	; 0x80
300088ac:	2136      	movs	r1, #54	; 0x36
300088ae:	6623      	str	r3, [r4, #96]	; 0x60
300088b0:	47a8      	blx	r5
300088b2:	2280      	movs	r2, #128	; 0x80
300088b4:	215c      	movs	r1, #92	; 0x5c
300088b6:	6e20      	ldr	r0, [r4, #96]	; 0x60
300088b8:	47a8      	blx	r5
300088ba:	4b24      	ldr	r3, [pc, #144]	; (3000894c <CRYPTO_SetSecurityModeAD+0x170>)
300088bc:	681d      	ldr	r5, [r3, #0]
300088be:	b9bd      	cbnz	r5, 300088f0 <CRYPTO_SetSecurityModeAD+0x114>
300088c0:	f1b8 0f00 	cmp.w	r8, #0
300088c4:	d103      	bne.n	300088ce <CRYPTO_SetSecurityModeAD+0xf2>
300088c6:	2180      	movs	r1, #128	; 0x80
300088c8:	481c      	ldr	r0, [pc, #112]	; (3000893c <CRYPTO_SetSecurityModeAD+0x160>)
300088ca:	f002 fd41 	bl	3000b350 <__io_assert_failed_veneer>
300088ce:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
300088d2:	6de2      	ldr	r2, [r4, #92]	; 0x5c
300088d4:	6a63      	ldr	r3, [r4, #36]	; 0x24
300088d6:	5d51      	ldrb	r1, [r2, r5]
300088d8:	5d5b      	ldrb	r3, [r3, r5]
300088da:	404b      	eors	r3, r1
300088dc:	5553      	strb	r3, [r2, r5]
300088de:	6e22      	ldr	r2, [r4, #96]	; 0x60
300088e0:	6a63      	ldr	r3, [r4, #36]	; 0x24
300088e2:	5d51      	ldrb	r1, [r2, r5]
300088e4:	5d5b      	ldrb	r3, [r3, r5]
300088e6:	404b      	eors	r3, r1
300088e8:	5553      	strb	r3, [r2, r5]
300088ea:	3501      	adds	r5, #1
300088ec:	42af      	cmp	r7, r5
300088ee:	d1f0      	bne.n	300088d2 <CRYPTO_SetSecurityModeAD+0xf6>
300088f0:	3601      	adds	r6, #1
300088f2:	d003      	beq.n	300088fc <CRYPTO_SetSecurityModeAD+0x120>
300088f4:	2301      	movs	r3, #1
300088f6:	85a3      	strh	r3, [r4, #44]	; 0x2c
300088f8:	2300      	movs	r3, #0
300088fa:	6323      	str	r3, [r4, #48]	; 0x30
300088fc:	2000      	movs	r0, #0
300088fe:	b003      	add	sp, #12
30008900:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008904:	2330      	movs	r3, #48	; 0x30
30008906:	e7bc      	b.n	30008882 <CRYPTO_SetSecurityModeAD+0xa6>
30008908:	2200      	movs	r2, #0
3000890a:	7522      	strb	r2, [r4, #20]
3000890c:	b14b      	cbz	r3, 30008922 <CRYPTO_SetSecurityModeAD+0x146>
3000890e:	2b02      	cmp	r3, #2
30008910:	d005      	beq.n	3000891e <CRYPTO_SetSecurityModeAD+0x142>
30008912:	2300      	movs	r3, #0
30008914:	e7b7      	b.n	30008886 <CRYPTO_SetSecurityModeAD+0xaa>
30008916:	2300      	movs	r3, #0
30008918:	6123      	str	r3, [r4, #16]
3000891a:	7523      	strb	r3, [r4, #20]
3000891c:	e7f9      	b.n	30008912 <CRYPTO_SetSecurityModeAD+0x136>
3000891e:	2310      	movs	r3, #16
30008920:	e7b1      	b.n	30008886 <CRYPTO_SetSecurityModeAD+0xaa>
30008922:	2314      	movs	r3, #20
30008924:	e7af      	b.n	30008886 <CRYPTO_SetSecurityModeAD+0xaa>
30008926:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
3000892a:	2240      	movs	r2, #64	; 0x40
3000892c:	2136      	movs	r1, #54	; 0x36
3000892e:	6623      	str	r3, [r4, #96]	; 0x60
30008930:	47a8      	blx	r5
30008932:	2240      	movs	r2, #64	; 0x40
30008934:	e7be      	b.n	300088b4 <CRYPTO_SetSecurityModeAD+0xd8>
30008936:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
3000893a:	e7d9      	b.n	300088f0 <CRYPTO_SetSecurityModeAD+0x114>
3000893c:	3000bc9d 	.word	0x3000bc9d
30008940:	3000bc0a 	.word	0x3000bc0a
30008944:	3000bc13 	.word	0x3000bc13
30008948:	00012be5 	.word	0x00012be5
3000894c:	2001c200 	.word	0x2001c200

30008950 <rtl_crypto_hmac_sha2_init>:
30008950:	e92d 41ff 	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
30008954:	4617      	mov	r7, r2
30008956:	461c      	mov	r4, r3
30008958:	4a82      	ldr	r2, [pc, #520]	; (30008b64 <rtl_crypto_hmac_sha2_init+0x214>)
3000895a:	4605      	mov	r5, r0
3000895c:	4b82      	ldr	r3, [pc, #520]	; (30008b68 <rtl_crypto_hmac_sha2_init+0x218>)
3000895e:	460e      	mov	r6, r1
30008960:	e843 f300 	tt	r3, r3
30008964:	f413 0f80 	tst.w	r3, #4194304	; 0x400000
30008968:	4b80      	ldr	r3, [pc, #512]	; (30008b6c <rtl_crypto_hmac_sha2_init+0x21c>)
3000896a:	bf18      	it	ne
3000896c:	4613      	movne	r3, r2
3000896e:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30008970:	2b00      	cmp	r3, #0
30008972:	db05      	blt.n	30008980 <rtl_crypto_hmac_sha2_init+0x30>
30008974:	2900      	cmp	r1, #0
30008976:	f000 80ef 	beq.w	30008b58 <rtl_crypto_hmac_sha2_init+0x208>
3000897a:	078b      	lsls	r3, r1, #30
3000897c:	f040 80ef 	bne.w	30008b5e <rtl_crypto_hmac_sha2_init+0x20e>
30008980:	2d1c      	cmp	r5, #28
30008982:	d001      	beq.n	30008988 <rtl_crypto_hmac_sha2_init+0x38>
30008984:	2d20      	cmp	r5, #32
30008986:	d11e      	bne.n	300089c6 <rtl_crypto_hmac_sha2_init+0x76>
30008988:	2f40      	cmp	r7, #64	; 0x40
3000898a:	d903      	bls.n	30008994 <rtl_crypto_hmac_sha2_init+0x44>
3000898c:	21ae      	movs	r1, #174	; 0xae
3000898e:	4878      	ldr	r0, [pc, #480]	; (30008b70 <rtl_crypto_hmac_sha2_init+0x220>)
30008990:	f002 fcde 	bl	3000b350 <__io_assert_failed_veneer>
30008994:	f8df 8218 	ldr.w	r8, [pc, #536]	; 30008bb0 <rtl_crypto_hmac_sha2_init+0x260>
30008998:	f898 3000 	ldrb.w	r3, [r8]
3000899c:	2b01      	cmp	r3, #1
3000899e:	d003      	beq.n	300089a8 <rtl_crypto_hmac_sha2_init+0x58>
300089a0:	21b4      	movs	r1, #180	; 0xb4
300089a2:	4873      	ldr	r0, [pc, #460]	; (30008b70 <rtl_crypto_hmac_sha2_init+0x220>)
300089a4:	f002 fcd4 	bl	3000b350 <__io_assert_failed_veneer>
300089a8:	2d30      	cmp	r5, #48	; 0x30
300089aa:	f000 80a0 	beq.w	30008aee <rtl_crypto_hmac_sha2_init+0x19e>
300089ae:	d812      	bhi.n	300089d6 <rtl_crypto_hmac_sha2_init+0x86>
300089b0:	2d1c      	cmp	r5, #28
300089b2:	d04f      	beq.n	30008a54 <rtl_crypto_hmac_sha2_init+0x104>
300089b4:	2d20      	cmp	r5, #32
300089b6:	f000 8082 	beq.w	30008abe <rtl_crypto_hmac_sha2_init+0x16e>
300089ba:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
300089be:	4628      	mov	r0, r5
300089c0:	b004      	add	sp, #16
300089c2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300089c6:	2d30      	cmp	r5, #48	; 0x30
300089c8:	d001      	beq.n	300089ce <rtl_crypto_hmac_sha2_init+0x7e>
300089ca:	2d40      	cmp	r5, #64	; 0x40
300089cc:	d1e2      	bne.n	30008994 <rtl_crypto_hmac_sha2_init+0x44>
300089ce:	2f80      	cmp	r7, #128	; 0x80
300089d0:	d9e0      	bls.n	30008994 <rtl_crypto_hmac_sha2_init+0x44>
300089d2:	21b1      	movs	r1, #177	; 0xb1
300089d4:	e7db      	b.n	3000898e <rtl_crypto_hmac_sha2_init+0x3e>
300089d6:	2d40      	cmp	r5, #64	; 0x40
300089d8:	d1ef      	bne.n	300089ba <rtl_crypto_hmac_sha2_init+0x6a>
300089da:	2c00      	cmp	r4, #0
300089dc:	d038      	beq.n	30008a50 <rtl_crypto_hmac_sha2_init+0x100>
300089de:	4b65      	ldr	r3, [pc, #404]	; (30008b74 <rtl_crypto_hmac_sha2_init+0x224>)
300089e0:	4a65      	ldr	r2, [pc, #404]	; (30008b78 <rtl_crypto_hmac_sha2_init+0x228>)
300089e2:	4966      	ldr	r1, [pc, #408]	; (30008b7c <rtl_crypto_hmac_sha2_init+0x22c>)
300089e4:	4d66      	ldr	r5, [pc, #408]	; (30008b80 <rtl_crypto_hmac_sha2_init+0x230>)
300089e6:	f8df c1f8 	ldr.w	ip, [pc, #504]	; 30008be0 <rtl_crypto_hmac_sha2_init+0x290>
300089ea:	e9c4 3202 	strd	r3, r2, [r4, #8]
300089ee:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
300089f2:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
300089f6:	33d4      	adds	r3, #212	; 0xd4
300089f8:	6163      	str	r3, [r4, #20]
300089fa:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
300089fe:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30008a02:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30008a06:	6123      	str	r3, [r4, #16]
30008a08:	4b5e      	ldr	r3, [pc, #376]	; (30008b84 <rtl_crypto_hmac_sha2_init+0x234>)
30008a0a:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008a0e:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30008a12:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30008a16:	f203 337f 	addw	r3, r3, #895	; 0x37f
30008a1a:	6263      	str	r3, [r4, #36]	; 0x24
30008a1c:	4b5a      	ldr	r3, [pc, #360]	; (30008b88 <rtl_crypto_hmac_sha2_init+0x238>)
30008a1e:	6223      	str	r3, [r4, #32]
30008a20:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30008a24:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30008a28:	f203 5397 	addw	r3, r3, #1431	; 0x597
30008a2c:	62e3      	str	r3, [r4, #44]	; 0x2c
30008a2e:	4b57      	ldr	r3, [pc, #348]	; (30008b8c <rtl_crypto_hmac_sha2_init+0x23c>)
30008a30:	62a3      	str	r3, [r4, #40]	; 0x28
30008a32:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30008a36:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30008a3a:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30008a3e:	6363      	str	r3, [r4, #52]	; 0x34
30008a40:	4b53      	ldr	r3, [pc, #332]	; (30008b90 <rtl_crypto_hmac_sha2_init+0x240>)
30008a42:	6323      	str	r3, [r4, #48]	; 0x30
30008a44:	4b53      	ldr	r3, [pc, #332]	; (30008b94 <rtl_crypto_hmac_sha2_init+0x244>)
30008a46:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30008a4a:	4b53      	ldr	r3, [pc, #332]	; (30008b98 <rtl_crypto_hmac_sha2_init+0x248>)
30008a4c:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30008a50:	2285      	movs	r2, #133	; 0x85
30008a52:	e01d      	b.n	30008a90 <rtl_crypto_hmac_sha2_init+0x140>
30008a54:	b1dc      	cbz	r4, 30008a8e <rtl_crypto_hmac_sha2_init+0x13e>
30008a56:	4b51      	ldr	r3, [pc, #324]	; (30008b9c <rtl_crypto_hmac_sha2_init+0x24c>)
30008a58:	4d51      	ldr	r5, [pc, #324]	; (30008ba0 <rtl_crypto_hmac_sha2_init+0x250>)
30008a5a:	f8df c164 	ldr.w	ip, [pc, #356]	; 30008bc0 <rtl_crypto_hmac_sha2_init+0x270>
30008a5e:	f8df e184 	ldr.w	lr, [pc, #388]	; 30008be4 <rtl_crypto_hmac_sha2_init+0x294>
30008a62:	e9c4 5302 	strd	r5, r3, [r4, #8]
30008a66:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
30008a6a:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
30008a6e:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30008a72:	6123      	str	r3, [r4, #16]
30008a74:	4b4b      	ldr	r3, [pc, #300]	; (30008ba4 <rtl_crypto_hmac_sha2_init+0x254>)
30008a76:	e9c4 c305 	strd	ip, r3, [r4, #20]
30008a7a:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30008a7e:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30008a82:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30008a86:	61e3      	str	r3, [r4, #28]
30008a88:	4b47      	ldr	r3, [pc, #284]	; (30008ba8 <rtl_crypto_hmac_sha2_init+0x258>)
30008a8a:	e9c4 e308 	strd	lr, r3, [r4, #32]
30008a8e:	2215      	movs	r2, #21
30008a90:	2300      	movs	r3, #0
30008a92:	4d46      	ldr	r5, [pc, #280]	; (30008bac <rtl_crypto_hmac_sha2_init+0x25c>)
30008a94:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008a98:	4845      	ldr	r0, [pc, #276]	; (30008bb0 <rtl_crypto_hmac_sha2_init+0x260>)
30008a9a:	9300      	str	r3, [sp, #0]
30008a9c:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30008aa0:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30008aa4:	e9cd 6701 	strd	r6, r7, [sp, #4]
30008aa8:	47a8      	blx	r5
30008aaa:	4605      	mov	r5, r0
30008aac:	2c00      	cmp	r4, #0
30008aae:	d086      	beq.n	300089be <rtl_crypto_hmac_sha2_init+0x6e>
30008ab0:	4b40      	ldr	r3, [pc, #256]	; (30008bb4 <rtl_crypto_hmac_sha2_init+0x264>)
30008ab2:	4620      	mov	r0, r4
30008ab4:	4798      	blx	r3
30008ab6:	2301      	movs	r3, #1
30008ab8:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30008abc:	e77f      	b.n	300089be <rtl_crypto_hmac_sha2_init+0x6e>
30008abe:	b1a4      	cbz	r4, 30008aea <rtl_crypto_hmac_sha2_init+0x19a>
30008ac0:	4b3d      	ldr	r3, [pc, #244]	; (30008bb8 <rtl_crypto_hmac_sha2_init+0x268>)
30008ac2:	4a2c      	ldr	r2, [pc, #176]	; (30008b74 <rtl_crypto_hmac_sha2_init+0x224>)
30008ac4:	492f      	ldr	r1, [pc, #188]	; (30008b84 <rtl_crypto_hmac_sha2_init+0x234>)
30008ac6:	4831      	ldr	r0, [pc, #196]	; (30008b8c <rtl_crypto_hmac_sha2_init+0x23c>)
30008ac8:	e9c4 2302 	strd	r2, r3, [r4, #8]
30008acc:	4b2e      	ldr	r3, [pc, #184]	; (30008b88 <rtl_crypto_hmac_sha2_init+0x238>)
30008ace:	e9c4 1304 	strd	r1, r3, [r4, #16]
30008ad2:	4b2f      	ldr	r3, [pc, #188]	; (30008b90 <rtl_crypto_hmac_sha2_init+0x240>)
30008ad4:	e9c4 0306 	strd	r0, r3, [r4, #24]
30008ad8:	4b2e      	ldr	r3, [pc, #184]	; (30008b94 <rtl_crypto_hmac_sha2_init+0x244>)
30008ada:	6223      	str	r3, [r4, #32]
30008adc:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30008ae0:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30008ae4:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30008ae8:	6263      	str	r3, [r4, #36]	; 0x24
30008aea:	2225      	movs	r2, #37	; 0x25
30008aec:	e7d0      	b.n	30008a90 <rtl_crypto_hmac_sha2_init+0x140>
30008aee:	2c00      	cmp	r4, #0
30008af0:	d030      	beq.n	30008b54 <rtl_crypto_hmac_sha2_init+0x204>
30008af2:	4b32      	ldr	r3, [pc, #200]	; (30008bbc <rtl_crypto_hmac_sha2_init+0x26c>)
30008af4:	f8df e0a8 	ldr.w	lr, [pc, #168]	; 30008ba0 <rtl_crypto_hmac_sha2_init+0x250>
30008af8:	4931      	ldr	r1, [pc, #196]	; (30008bc0 <rtl_crypto_hmac_sha2_init+0x270>)
30008afa:	482a      	ldr	r0, [pc, #168]	; (30008ba4 <rtl_crypto_hmac_sha2_init+0x254>)
30008afc:	f8df c0e4 	ldr.w	ip, [pc, #228]	; 30008be4 <rtl_crypto_hmac_sha2_init+0x294>
30008b00:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30008b04:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30008b08:	f8df e09c 	ldr.w	lr, [pc, #156]	; 30008ba8 <rtl_crypto_hmac_sha2_init+0x258>
30008b0c:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30008b10:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30008b14:	6163      	str	r3, [r4, #20]
30008b16:	4b2b      	ldr	r3, [pc, #172]	; (30008bc4 <rtl_crypto_hmac_sha2_init+0x274>)
30008b18:	6123      	str	r3, [r4, #16]
30008b1a:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30008b1e:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30008b22:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30008b26:	61e3      	str	r3, [r4, #28]
30008b28:	4b27      	ldr	r3, [pc, #156]	; (30008bc8 <rtl_crypto_hmac_sha2_init+0x278>)
30008b2a:	61a3      	str	r3, [r4, #24]
30008b2c:	4b27      	ldr	r3, [pc, #156]	; (30008bcc <rtl_crypto_hmac_sha2_init+0x27c>)
30008b2e:	e9c4 3108 	strd	r3, r1, [r4, #32]
30008b32:	4b27      	ldr	r3, [pc, #156]	; (30008bd0 <rtl_crypto_hmac_sha2_init+0x280>)
30008b34:	e9c4 300a 	strd	r3, r0, [r4, #40]	; 0x28
30008b38:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30008b3c:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30008b40:	33aa      	adds	r3, #170	; 0xaa
30008b42:	6363      	str	r3, [r4, #52]	; 0x34
30008b44:	4b23      	ldr	r3, [pc, #140]	; (30008bd4 <rtl_crypto_hmac_sha2_init+0x284>)
30008b46:	6323      	str	r3, [r4, #48]	; 0x30
30008b48:	4b23      	ldr	r3, [pc, #140]	; (30008bd8 <rtl_crypto_hmac_sha2_init+0x288>)
30008b4a:	e9c4 3c0e 	strd	r3, ip, [r4, #56]	; 0x38
30008b4e:	4b23      	ldr	r3, [pc, #140]	; (30008bdc <rtl_crypto_hmac_sha2_init+0x28c>)
30008b50:	e9c4 3e10 	strd	r3, lr, [r4, #64]	; 0x40
30008b54:	2245      	movs	r2, #69	; 0x45
30008b56:	e79b      	b.n	30008a90 <rtl_crypto_hmac_sha2_init+0x140>
30008b58:	f06f 0503 	mvn.w	r5, #3
30008b5c:	e72f      	b.n	300089be <rtl_crypto_hmac_sha2_init+0x6e>
30008b5e:	f06f 0505 	mvn.w	r5, #5
30008b62:	e72c      	b.n	300089be <rtl_crypto_hmac_sha2_init+0x6e>
30008b64:	500c8000 	.word	0x500c8000
30008b68:	0000e5f9 	.word	0x0000e5f9
30008b6c:	400c8000 	.word	0x400c8000
30008b70:	3000bc83 	.word	0x3000bc83
30008b74:	6a09e667 	.word	0x6a09e667
30008b78:	f3bcc908 	.word	0xf3bcc908
30008b7c:	fe94f82b 	.word	0xfe94f82b
30008b80:	fb41bd6b 	.word	0xfb41bd6b
30008b84:	3c6ef372 	.word	0x3c6ef372
30008b88:	a54ff53a 	.word	0xa54ff53a
30008b8c:	510e527f 	.word	0x510e527f
30008b90:	9b05688c 	.word	0x9b05688c
30008b94:	1f83d9ab 	.word	0x1f83d9ab
30008b98:	5be0cd19 	.word	0x5be0cd19
30008b9c:	367cd507 	.word	0x367cd507
30008ba0:	c1059ed8 	.word	0xc1059ed8
30008ba4:	ffc00b31 	.word	0xffc00b31
30008ba8:	befa4fa4 	.word	0xbefa4fa4
30008bac:	300087dd 	.word	0x300087dd
30008bb0:	2001c460 	.word	0x2001c460
30008bb4:	00004c99 	.word	0x00004c99
30008bb8:	bb67ae85 	.word	0xbb67ae85
30008bbc:	cbbb9d5d 	.word	0xcbbb9d5d
30008bc0:	f70e5939 	.word	0xf70e5939
30008bc4:	629a292a 	.word	0x629a292a
30008bc8:	9159015a 	.word	0x9159015a
30008bcc:	152fecd8 	.word	0x152fecd8
30008bd0:	67332667 	.word	0x67332667
30008bd4:	8eb44a87 	.word	0x8eb44a87
30008bd8:	db0c2e0d 	.word	0xdb0c2e0d
30008bdc:	47b5481d 	.word	0x47b5481d
30008be0:	137e2179 	.word	0x137e2179
30008be4:	64f98fa7 	.word	0x64f98fa7

30008be8 <CPU_ClkGet>:
30008be8:	b570      	push	{r4, r5, r6, lr}
30008bea:	4c1d      	ldr	r4, [pc, #116]	; (30008c60 <CPU_ClkGet+0x78>)
30008bec:	4b1d      	ldr	r3, [pc, #116]	; (30008c64 <CPU_ClkGet+0x7c>)
30008bee:	f8d4 6224 	ldr.w	r6, [r4, #548]	; 0x224
30008bf2:	f8d4 527c 	ldr.w	r5, [r4, #636]	; 0x27c
30008bf6:	f3c5 1581 	ubfx	r5, r5, #6, #2
30008bfa:	4798      	blx	r3
30008bfc:	b93d      	cbnz	r5, 30008c0e <CPU_ClkGet+0x26>
30008bfe:	2801      	cmp	r0, #1
30008c00:	d025      	beq.n	30008c4e <CPU_ClkGet+0x66>
30008c02:	07b3      	lsls	r3, r6, #30
30008c04:	d525      	bpl.n	30008c52 <CPU_ClkGet+0x6a>
30008c06:	4b18      	ldr	r3, [pc, #96]	; (30008c68 <CPU_ClkGet+0x80>)
30008c08:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008c0c:	4718      	bx	r3
30008c0e:	2d01      	cmp	r5, #1
30008c10:	d121      	bne.n	30008c56 <CPU_ClkGet+0x6e>
30008c12:	2801      	cmp	r0, #1
30008c14:	d109      	bne.n	30008c2a <CPU_ClkGet+0x42>
30008c16:	f8d4 326c 	ldr.w	r3, [r4, #620]	; 0x26c
30008c1a:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30008c1e:	f3c3 4007 	ubfx	r0, r3, #16, #8
30008c22:	d01a      	beq.n	30008c5a <CPU_ClkGet+0x72>
30008c24:	4b11      	ldr	r3, [pc, #68]	; (30008c6c <CPU_ClkGet+0x84>)
30008c26:	4358      	muls	r0, r3
30008c28:	bd70      	pop	{r4, r5, r6, pc}
30008c2a:	f416 7f80 	tst.w	r6, #256	; 0x100
30008c2e:	4810      	ldr	r0, [pc, #64]	; (30008c70 <CPU_ClkGet+0x88>)
30008c30:	d0fa      	beq.n	30008c28 <CPU_ClkGet+0x40>
30008c32:	4b10      	ldr	r3, [pc, #64]	; (30008c74 <CPU_ClkGet+0x8c>)
30008c34:	f8d4 2228 	ldr.w	r2, [r4, #552]	; 0x228
30008c38:	6c5b      	ldr	r3, [r3, #68]	; 0x44
30008c3a:	f3c3 1345 	ubfx	r3, r3, #5, #6
30008c3e:	3302      	adds	r3, #2
30008c40:	4358      	muls	r0, r3
30008c42:	f002 0307 	and.w	r3, r2, #7
30008c46:	3301      	adds	r3, #1
30008c48:	fbb0 f0f3 	udiv	r0, r0, r3
30008c4c:	e7ec      	b.n	30008c28 <CPU_ClkGet+0x40>
30008c4e:	480a      	ldr	r0, [pc, #40]	; (30008c78 <CPU_ClkGet+0x90>)
30008c50:	e7ea      	b.n	30008c28 <CPU_ClkGet+0x40>
30008c52:	480a      	ldr	r0, [pc, #40]	; (30008c7c <CPU_ClkGet+0x94>)
30008c54:	e7e8      	b.n	30008c28 <CPU_ClkGet+0x40>
30008c56:	2000      	movs	r0, #0
30008c58:	e7e6      	b.n	30008c28 <CPU_ClkGet+0x40>
30008c5a:	4809      	ldr	r0, [pc, #36]	; (30008c80 <CPU_ClkGet+0x98>)
30008c5c:	e7e4      	b.n	30008c28 <CPU_ClkGet+0x40>
30008c5e:	bf00      	nop
30008c60:	42008000 	.word	0x42008000
30008c64:	0000c0f9 	.word	0x0000c0f9
30008c68:	000099f5 	.word	0x000099f5
30008c6c:	000f4240 	.word	0x000f4240
30008c70:	02625a00 	.word	0x02625a00
30008c74:	42008800 	.word	0x42008800
30008c78:	01312d00 	.word	0x01312d00
30008c7c:	003d0900 	.word	0x003d0900
30008c80:	02faf080 	.word	0x02faf080

30008c84 <RSIP_IV_Set>:
30008c84:	00c0      	lsls	r0, r0, #3
30008c86:	680b      	ldr	r3, [r1, #0]
30008c88:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30008c8c:	ba1b      	rev	r3, r3
30008c8e:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008c92:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30008c96:	684b      	ldr	r3, [r1, #4]
30008c98:	ba1b      	rev	r3, r3
30008c9a:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30008c9e:	4770      	bx	lr

30008ca0 <RSIP_OTF_Cmd>:
30008ca0:	4b04      	ldr	r3, [pc, #16]	; (30008cb4 <RSIP_OTF_Cmd+0x14>)
30008ca2:	2801      	cmp	r0, #1
30008ca4:	695a      	ldr	r2, [r3, #20]
30008ca6:	bf0c      	ite	eq
30008ca8:	f442 7280 	orreq.w	r2, r2, #256	; 0x100
30008cac:	f422 7280 	bicne.w	r2, r2, #256	; 0x100
30008cb0:	615a      	str	r2, [r3, #20]
30008cb2:	4770      	bx	lr
30008cb4:	42008c00 	.word	0x42008c00

30008cb8 <RSIP_OTF_Enable>:
30008cb8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008cbc:	4688      	mov	r8, r1
30008cbe:	4617      	mov	r7, r2
30008cc0:	00c4      	lsls	r4, r0, #3
30008cc2:	e9dd 6506 	ldrd	r6, r5, [sp, #24]
30008cc6:	b32b      	cbz	r3, 30008d14 <RSIP_OTF_Enable+0x5c>
30008cc8:	2e07      	cmp	r6, #7
30008cca:	d904      	bls.n	30008cd6 <RSIP_OTF_Enable+0x1e>
30008ccc:	f44f 710c 	mov.w	r1, #560	; 0x230
30008cd0:	4815      	ldr	r0, [pc, #84]	; (30008d28 <RSIP_OTF_Enable+0x70>)
30008cd2:	f002 fb3d 	bl	3000b350 <__io_assert_failed_veneer>
30008cd6:	1e6b      	subs	r3, r5, #1
30008cd8:	2b01      	cmp	r3, #1
30008cda:	d904      	bls.n	30008ce6 <RSIP_OTF_Enable+0x2e>
30008cdc:	f240 2131 	movw	r1, #561	; 0x231
30008ce0:	4811      	ldr	r0, [pc, #68]	; (30008d28 <RSIP_OTF_Enable+0x70>)
30008ce2:	f002 fb35 	bl	3000b350 <__io_assert_failed_veneer>
30008ce6:	f104 4084 	add.w	r0, r4, #1107296256	; 0x42000000
30008cea:	006d      	lsls	r5, r5, #1
30008cec:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008cf0:	f005 0506 	and.w	r5, r5, #6
30008cf4:	6c43      	ldr	r3, [r0, #68]	; 0x44
30008cf6:	ea45 0207 	orr.w	r2, r5, r7
30008cfa:	f023 0301 	bic.w	r3, r3, #1
30008cfe:	f042 0201 	orr.w	r2, r2, #1
30008d02:	6443      	str	r3, [r0, #68]	; 0x44
30008d04:	f006 0307 	and.w	r3, r6, #7
30008d08:	ea43 0308 	orr.w	r3, r3, r8
30008d0c:	6403      	str	r3, [r0, #64]	; 0x40
30008d0e:	6442      	str	r2, [r0, #68]	; 0x44
30008d10:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008d14:	f104 4084 	add.w	r0, r4, #1107296256	; 0x42000000
30008d18:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008d1c:	6c43      	ldr	r3, [r0, #68]	; 0x44
30008d1e:	f023 0301 	bic.w	r3, r3, #1
30008d22:	6443      	str	r3, [r0, #68]	; 0x44
30008d24:	e7f4      	b.n	30008d10 <RSIP_OTF_Enable+0x58>
30008d26:	bf00      	nop
30008d28:	3000bc73 	.word	0x3000bc73

30008d2c <RSIP_MMU_Config>:
30008d2c:	b510      	push	{r4, lr}
30008d2e:	240c      	movs	r4, #12
30008d30:	311f      	adds	r1, #31
30008d32:	321f      	adds	r2, #31
30008d34:	331f      	adds	r3, #31
30008d36:	4360      	muls	r0, r4
30008d38:	0949      	lsrs	r1, r1, #5
30008d3a:	0952      	lsrs	r2, r2, #5
30008d3c:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30008d40:	095b      	lsrs	r3, r3, #5
30008d42:	0249      	lsls	r1, r1, #9
30008d44:	f500 400c 	add.w	r0, r0, #35840	; 0x8c00
30008d48:	0252      	lsls	r2, r2, #9
30008d4a:	025b      	lsls	r3, r3, #9
30008d4c:	f8d0 4108 	ldr.w	r4, [r0, #264]	; 0x108
30008d50:	f024 0401 	bic.w	r4, r4, #1
30008d54:	f8c0 4108 	str.w	r4, [r0, #264]	; 0x108
30008d58:	f8c0 1100 	str.w	r1, [r0, #256]	; 0x100
30008d5c:	f8c0 2104 	str.w	r2, [r0, #260]	; 0x104
30008d60:	f8c0 3108 	str.w	r3, [r0, #264]	; 0x108
30008d64:	bd10      	pop	{r4, pc}
	...

30008d68 <RSIP_MMU_Cmd>:
30008d68:	4b0a      	ldr	r3, [pc, #40]	; (30008d94 <RSIP_MMU_Cmd+0x2c>)
30008d6a:	220c      	movs	r2, #12
30008d6c:	b141      	cbz	r1, 30008d80 <RSIP_MMU_Cmd+0x18>
30008d6e:	fb02 3000 	mla	r0, r2, r0, r3
30008d72:	f8d0 3108 	ldr.w	r3, [r0, #264]	; 0x108
30008d76:	f043 0301 	orr.w	r3, r3, #1
30008d7a:	f8c0 3108 	str.w	r3, [r0, #264]	; 0x108
30008d7e:	4770      	bx	lr
30008d80:	fb02 3300 	mla	r3, r2, r0, r3
30008d84:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
30008d88:	f022 0201 	bic.w	r2, r2, #1
30008d8c:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30008d90:	4770      	bx	lr
30008d92:	bf00      	nop
30008d94:	42008c00 	.word	0x42008c00

30008d98 <RSIP_MMU_Cache_Clean>:
30008d98:	4a02      	ldr	r2, [pc, #8]	; (30008da4 <RSIP_MMU_Cache_Clean+0xc>)
30008d9a:	6993      	ldr	r3, [r2, #24]
30008d9c:	f043 0304 	orr.w	r3, r3, #4
30008da0:	6193      	str	r3, [r2, #24]
30008da2:	4770      	bx	lr
30008da4:	42008c00 	.word	0x42008c00

30008da8 <NAND_UserModeEn>:
30008da8:	4b07      	ldr	r3, [pc, #28]	; (30008dc8 <NAND_UserModeEn+0x20>)
30008daa:	b510      	push	{r4, lr}
30008dac:	4604      	mov	r4, r0
30008dae:	2000      	movs	r0, #0
30008db0:	4798      	blx	r3
30008db2:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30008db6:	681a      	ldr	r2, [r3, #0]
30008db8:	b11c      	cbz	r4, 30008dc2 <NAND_UserModeEn+0x1a>
30008dba:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008dbe:	601a      	str	r2, [r3, #0]
30008dc0:	bd10      	pop	{r4, pc}
30008dc2:	f022 4200 	bic.w	r2, r2, #2147483648	; 0x80000000
30008dc6:	e7fa      	b.n	30008dbe <NAND_UserModeEn+0x16>
30008dc8:	0000b9c1 	.word	0x0000b9c1

30008dcc <NAND_RxData>:
30008dcc:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30008dd0:	460f      	mov	r7, r1
30008dd2:	4615      	mov	r5, r2
30008dd4:	461e      	mov	r6, r3
30008dd6:	4680      	mov	r8, r0
30008dd8:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30008ebc <NAND_RxData+0xf0>
30008ddc:	b908      	cbnz	r0, 30008de2 <NAND_RxData+0x16>
30008dde:	f899 8008 	ldrb.w	r8, [r9, #8]
30008de2:	f04f 4488 	mov.w	r4, #1140850688	; 0x44000000
30008de6:	2001      	movs	r0, #1
30008de8:	f7ff ffde 	bl	30008da8 <NAND_UserModeEn>
30008dec:	2100      	movs	r1, #0
30008dee:	6823      	ldr	r3, [r4, #0]
30008df0:	f443 7340 	orr.w	r3, r3, #768	; 0x300
30008df4:	6023      	str	r3, [r4, #0]
30008df6:	f025 437f 	bic.w	r3, r5, #4278190080	; 0xff000000
30008dfa:	6063      	str	r3, [r4, #4]
30008dfc:	f8c4 1130 	str.w	r1, [r4, #304]	; 0x130
30008e00:	f899 0062 	ldrb.w	r0, [r9, #98]	; 0x62
30008e04:	f8d4 2118 	ldr.w	r2, [r4, #280]	; 0x118
30008e08:	0403      	lsls	r3, r0, #16
30008e0a:	2803      	cmp	r0, #3
30008e0c:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
30008e10:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
30008e14:	ea43 0302 	orr.w	r3, r3, r2
30008e18:	f8c4 3118 	str.w	r3, [r4, #280]	; 0x118
30008e1c:	f04f 0301 	mov.w	r3, #1
30008e20:	6123      	str	r3, [r4, #16]
30008e22:	f3c7 2307 	ubfx	r3, r7, #8, #8
30008e26:	f884 8060 	strb.w	r8, [r4, #96]	; 0x60
30008e2a:	b2ff      	uxtb	r7, r7
30008e2c:	bf08      	it	eq
30008e2e:	f884 1060 	strbeq.w	r1, [r4, #96]	; 0x60
30008e32:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008e36:	2301      	movs	r3, #1
30008e38:	f884 7060 	strb.w	r7, [r4, #96]	; 0x60
30008e3c:	60a3      	str	r3, [r4, #8]
30008e3e:	f016 0303 	ands.w	r3, r6, #3
30008e42:	d127      	bne.n	30008e94 <NAND_RxData+0xc8>
30008e44:	4619      	mov	r1, r3
30008e46:	1870      	adds	r0, r6, r1
30008e48:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30008e4c:	42a9      	cmp	r1, r5
30008e4e:	d209      	bcs.n	30008e64 <NAND_RxData+0x98>
30008e50:	6a53      	ldr	r3, [r2, #36]	; 0x24
30008e52:	f003 033c 	and.w	r3, r3, #60	; 0x3c
30008e56:	18c4      	adds	r4, r0, r3
30008e58:	42a0      	cmp	r0, r4
30008e5a:	d121      	bne.n	30008ea0 <NAND_RxData+0xd4>
30008e5c:	4419      	add	r1, r3
30008e5e:	1a6b      	subs	r3, r5, r1
30008e60:	2b03      	cmp	r3, #3
30008e62:	d8f3      	bhi.n	30008e4c <NAND_RxData+0x80>
30008e64:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30008e68:	42a9      	cmp	r1, r5
30008e6a:	d31d      	bcc.n	30008ea8 <NAND_RxData+0xdc>
30008e6c:	4b12      	ldr	r3, [pc, #72]	; (30008eb8 <NAND_RxData+0xec>)
30008e6e:	2004      	movs	r0, #4
30008e70:	4798      	blx	r3
30008e72:	2000      	movs	r0, #0
30008e74:	e8bd 43f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30008e78:	f7ff bf96 	b.w	30008da8 <NAND_UserModeEn>
30008e7c:	6a90      	ldr	r0, [r2, #40]	; 0x28
30008e7e:	0700      	lsls	r0, r0, #28
30008e80:	bf42      	ittt	mi
30008e82:	f892 0060 	ldrbmi.w	r0, [r2, #96]	; 0x60
30008e86:	5470      	strbmi	r0, [r6, r1]
30008e88:	3101      	addmi	r1, #1
30008e8a:	428b      	cmp	r3, r1
30008e8c:	d0db      	beq.n	30008e46 <NAND_RxData+0x7a>
30008e8e:	42a9      	cmp	r1, r5
30008e90:	d3f4      	bcc.n	30008e7c <NAND_RxData+0xb0>
30008e92:	e7d8      	b.n	30008e46 <NAND_RxData+0x7a>
30008e94:	2100      	movs	r1, #0
30008e96:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30008e9a:	f1c3 0304 	rsb	r3, r3, #4
30008e9e:	e7f6      	b.n	30008e8e <NAND_RxData+0xc2>
30008ea0:	6e17      	ldr	r7, [r2, #96]	; 0x60
30008ea2:	f840 7b04 	str.w	r7, [r0], #4
30008ea6:	e7d7      	b.n	30008e58 <NAND_RxData+0x8c>
30008ea8:	6a9a      	ldr	r2, [r3, #40]	; 0x28
30008eaa:	0712      	lsls	r2, r2, #28
30008eac:	bf42      	ittt	mi
30008eae:	f893 2060 	ldrbmi.w	r2, [r3, #96]	; 0x60
30008eb2:	5472      	strbmi	r2, [r6, r1]
30008eb4:	3101      	addmi	r1, #1
30008eb6:	e7d7      	b.n	30008e68 <NAND_RxData+0x9c>
30008eb8:	0000b9c1 	.word	0x0000b9c1
30008ebc:	2001c01c 	.word	0x2001c01c

30008ec0 <NAND_Page_Read>:
30008ec0:	b530      	push	{r4, r5, lr}
30008ec2:	b085      	sub	sp, #20
30008ec4:	4c07      	ldr	r4, [pc, #28]	; (30008ee4 <NAND_Page_Read+0x24>)
30008ec6:	4d08      	ldr	r5, [pc, #32]	; (30008ee8 <NAND_Page_Read+0x28>)
30008ec8:	9301      	str	r3, [sp, #4]
30008eca:	e9cd 2102 	strd	r2, r1, [sp, #8]
30008ece:	47a0      	blx	r4
30008ed0:	4604      	mov	r4, r0
30008ed2:	4806      	ldr	r0, [pc, #24]	; (30008eec <NAND_Page_Read+0x2c>)
30008ed4:	9903      	ldr	r1, [sp, #12]
30008ed6:	7a00      	ldrb	r0, [r0, #8]
30008ed8:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30008edc:	47a8      	blx	r5
30008ede:	4620      	mov	r0, r4
30008ee0:	b005      	add	sp, #20
30008ee2:	bd30      	pop	{r4, r5, pc}
30008ee4:	0000bd85 	.word	0x0000bd85
30008ee8:	30008dcd 	.word	0x30008dcd
30008eec:	2001c01c 	.word	0x2001c01c

30008ef0 <irq_set_priority>:
30008ef0:	2907      	cmp	r1, #7
30008ef2:	b530      	push	{r4, r5, lr}
30008ef4:	4605      	mov	r5, r0
30008ef6:	b085      	sub	sp, #20
30008ef8:	460c      	mov	r4, r1
30008efa:	d90e      	bls.n	30008f1a <irq_set_priority+0x2a>
30008efc:	2307      	movs	r3, #7
30008efe:	9000      	str	r0, [sp, #0]
30008f00:	4a10      	ldr	r2, [pc, #64]	; (30008f44 <irq_set_priority+0x54>)
30008f02:	2003      	movs	r0, #3
30008f04:	e9cd 1301 	strd	r1, r3, [sp, #4]
30008f08:	4b0f      	ldr	r3, [pc, #60]	; (30008f48 <irq_set_priority+0x58>)
30008f0a:	4619      	mov	r1, r3
30008f0c:	f001 ffcc 	bl	3000aea8 <rtk_log_write>
30008f10:	f44f 7168 	mov.w	r1, #928	; 0x3a0
30008f14:	480d      	ldr	r0, [pc, #52]	; (30008f4c <irq_set_priority+0x5c>)
30008f16:	f002 fa1b 	bl	3000b350 <__io_assert_failed_veneer>
30008f1a:	2c07      	cmp	r4, #7
30008f1c:	bf28      	it	cs
30008f1e:	2407      	movcs	r4, #7
30008f20:	2d00      	cmp	r5, #0
30008f22:	ea4f 1444 	mov.w	r4, r4, lsl #5
30008f26:	bfae      	itee	ge
30008f28:	f105 4560 	addge.w	r5, r5, #3758096384	; 0xe0000000
30008f2c:	f005 050f 	andlt.w	r5, r5, #15
30008f30:	4b07      	ldrlt	r3, [pc, #28]	; (30008f50 <irq_set_priority+0x60>)
30008f32:	b2e4      	uxtb	r4, r4
30008f34:	bfaa      	itet	ge
30008f36:	f505 4561 	addge.w	r5, r5, #57600	; 0xe100
30008f3a:	555c      	strblt	r4, [r3, r5]
30008f3c:	f885 4300 	strbge.w	r4, [r5, #768]	; 0x300
30008f40:	b005      	add	sp, #20
30008f42:	bd30      	pop	{r4, r5, pc}
30008f44:	3000bc3a 	.word	0x3000bc3a
30008f48:	3000bc0a 	.word	0x3000bc0a
30008f4c:	3000bc62 	.word	0x3000bc62
30008f50:	e000ed14 	.word	0xe000ed14

30008f54 <irq_register>:
30008f54:	b510      	push	{r4, lr}
30008f56:	f040 0001 	orr.w	r0, r0, #1
30008f5a:	460c      	mov	r4, r1
30008f5c:	4905      	ldr	r1, [pc, #20]	; (30008f74 <irq_register+0x20>)
30008f5e:	f841 0024 	str.w	r0, [r1, r4, lsl #2]
30008f62:	4620      	mov	r0, r4
30008f64:	4904      	ldr	r1, [pc, #16]	; (30008f78 <irq_register+0x24>)
30008f66:	f841 2024 	str.w	r2, [r1, r4, lsl #2]
30008f6a:	4619      	mov	r1, r3
30008f6c:	4b03      	ldr	r3, [pc, #12]	; (30008f7c <irq_register+0x28>)
30008f6e:	4798      	blx	r3
30008f70:	2001      	movs	r0, #1
30008f72:	bd10      	pop	{r4, pc}
30008f74:	30000184 	.word	0x30000184
30008f78:	300002c4 	.word	0x300002c4
30008f7c:	30008ef1 	.word	0x30008ef1

30008f80 <SYSCFG_BootFromNor>:
30008f80:	4b0e      	ldr	r3, [pc, #56]	; (30008fbc <SYSCFG_BootFromNor+0x3c>)
30008f82:	b510      	push	{r4, lr}
30008f84:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30008f88:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30008f8c:	0392      	lsls	r2, r2, #14
30008f8e:	d50a      	bpl.n	30008fa6 <SYSCFG_BootFromNor+0x26>
30008f90:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30008f94:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30008f98:	d105      	bne.n	30008fa6 <SYSCFG_BootFromNor+0x26>
30008f9a:	4b09      	ldr	r3, [pc, #36]	; (30008fc0 <SYSCFG_BootFromNor+0x40>)
30008f9c:	4798      	blx	r3
30008f9e:	3800      	subs	r0, #0
30008fa0:	bf18      	it	ne
30008fa2:	2001      	movne	r0, #1
30008fa4:	bd10      	pop	{r4, pc}
30008fa6:	4c07      	ldr	r4, [pc, #28]	; (30008fc4 <SYSCFG_BootFromNor+0x44>)
30008fa8:	47a0      	blx	r4
30008faa:	2802      	cmp	r0, #2
30008fac:	d004      	beq.n	30008fb8 <SYSCFG_BootFromNor+0x38>
30008fae:	47a0      	blx	r4
30008fb0:	2801      	cmp	r0, #1
30008fb2:	d1f2      	bne.n	30008f9a <SYSCFG_BootFromNor+0x1a>
30008fb4:	2000      	movs	r0, #0
30008fb6:	e7f5      	b.n	30008fa4 <SYSCFG_BootFromNor+0x24>
30008fb8:	2001      	movs	r0, #1
30008fba:	e7f3      	b.n	30008fa4 <SYSCFG_BootFromNor+0x24>
30008fbc:	42008000 	.word	0x42008000
30008fc0:	0000c0bd 	.word	0x0000c0bd
30008fc4:	0000c1a9 	.word	0x0000c1a9

30008fc8 <SYSCFG_RLVersion>:
30008fc8:	4b07      	ldr	r3, [pc, #28]	; (30008fe8 <SYSCFG_RLVersion+0x20>)
30008fca:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30008fce:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30008fd2:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30008fd6:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30008fda:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30008fde:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30008fe2:	f3c0 4003 	ubfx	r0, r0, #16, #4
30008fe6:	4770      	bx	lr
30008fe8:	42008000 	.word	0x42008000

30008fec <SBOOT_GetMdType>:
30008fec:	b513      	push	{r0, r1, r4, lr}
30008fee:	4b0b      	ldr	r3, [pc, #44]	; (3000901c <SBOOT_GetMdType+0x30>)
30008ff0:	4604      	mov	r4, r0
30008ff2:	e9cd 2100 	strd	r2, r1, [sp]
30008ff6:	4798      	blx	r3
30008ff8:	4b09      	ldr	r3, [pc, #36]	; (30009020 <SBOOT_GetMdType+0x34>)
30008ffa:	e9dd 2100 	ldrd	r2, r1, [sp]
30008ffe:	6018      	str	r0, [r3, #0]
30009000:	b928      	cbnz	r0, 3000900e <SBOOT_GetMdType+0x22>
30009002:	4620      	mov	r0, r4
30009004:	b002      	add	sp, #8
30009006:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000900a:	f002 b9f1 	b.w	3000b3f0 <__SBOOT_GetMdType_A_veneer>
3000900e:	4620      	mov	r0, r4
30009010:	b002      	add	sp, #8
30009012:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009016:	f002 ba0b 	b.w	3000b430 <__SBOOT_GetMdType_B_veneer>
3000901a:	bf00      	nop
3000901c:	30008fc9 	.word	0x30008fc9
30009020:	3000eb74 	.word	0x3000eb74

30009024 <CRYPTO_OTPKey_SHA_Init>:
30009024:	b513      	push	{r0, r1, r4, lr}
30009026:	4b0a      	ldr	r3, [pc, #40]	; (30009050 <CRYPTO_OTPKey_SHA_Init+0x2c>)
30009028:	4604      	mov	r4, r0
3000902a:	9101      	str	r1, [sp, #4]
3000902c:	4798      	blx	r3
3000902e:	4b09      	ldr	r3, [pc, #36]	; (30009054 <CRYPTO_OTPKey_SHA_Init+0x30>)
30009030:	9901      	ldr	r1, [sp, #4]
30009032:	6018      	str	r0, [r3, #0]
30009034:	b928      	cbnz	r0, 30009042 <CRYPTO_OTPKey_SHA_Init+0x1e>
30009036:	4620      	mov	r0, r4
30009038:	b002      	add	sp, #8
3000903a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000903e:	f002 ba1f 	b.w	3000b480 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30009042:	4620      	mov	r0, r4
30009044:	b002      	add	sp, #8
30009046:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000904a:	f002 ba21 	b.w	3000b490 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
3000904e:	bf00      	nop
30009050:	30008fc9 	.word	0x30008fc9
30009054:	3000eb74 	.word	0x3000eb74

30009058 <rtl_crypto_hmac_sha2_update>:
30009058:	b513      	push	{r0, r1, r4, lr}
3000905a:	4b0b      	ldr	r3, [pc, #44]	; (30009088 <rtl_crypto_hmac_sha2_update+0x30>)
3000905c:	4604      	mov	r4, r0
3000905e:	e9cd 2100 	strd	r2, r1, [sp]
30009062:	4798      	blx	r3
30009064:	4b09      	ldr	r3, [pc, #36]	; (3000908c <rtl_crypto_hmac_sha2_update+0x34>)
30009066:	e9dd 2100 	ldrd	r2, r1, [sp]
3000906a:	6018      	str	r0, [r3, #0]
3000906c:	b928      	cbnz	r0, 3000907a <rtl_crypto_hmac_sha2_update+0x22>
3000906e:	4620      	mov	r0, r4
30009070:	b002      	add	sp, #8
30009072:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009076:	f002 b983 	b.w	3000b380 <__rtl_crypto_hmac_sha2_update_A_veneer>
3000907a:	4620      	mov	r0, r4
3000907c:	b002      	add	sp, #8
3000907e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009082:	f002 b925 	b.w	3000b2d0 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009086:	bf00      	nop
30009088:	30008fc9 	.word	0x30008fc9
3000908c:	3000eb74 	.word	0x3000eb74

30009090 <rtl_crypto_hmac_sha2_final>:
30009090:	b513      	push	{r0, r1, r4, lr}
30009092:	4b0a      	ldr	r3, [pc, #40]	; (300090bc <rtl_crypto_hmac_sha2_final+0x2c>)
30009094:	4604      	mov	r4, r0
30009096:	9101      	str	r1, [sp, #4]
30009098:	4798      	blx	r3
3000909a:	4b09      	ldr	r3, [pc, #36]	; (300090c0 <rtl_crypto_hmac_sha2_final+0x30>)
3000909c:	9901      	ldr	r1, [sp, #4]
3000909e:	6018      	str	r0, [r3, #0]
300090a0:	b928      	cbnz	r0, 300090ae <rtl_crypto_hmac_sha2_final+0x1e>
300090a2:	4620      	mov	r0, r4
300090a4:	b002      	add	sp, #8
300090a6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090aa:	f002 b959 	b.w	3000b360 <__rtl_crypto_hmac_sha2_final_A_veneer>
300090ae:	4620      	mov	r0, r4
300090b0:	b002      	add	sp, #8
300090b2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090b6:	f002 b9fb 	b.w	3000b4b0 <__rtl_crypto_hmac_sha2_final_B_veneer>
300090ba:	bf00      	nop
300090bc:	30008fc9 	.word	0x30008fc9
300090c0:	3000eb74 	.word	0x3000eb74

300090c4 <rtl_crypto_sha2_init>:
300090c4:	b513      	push	{r0, r1, r4, lr}
300090c6:	4b0a      	ldr	r3, [pc, #40]	; (300090f0 <rtl_crypto_sha2_init+0x2c>)
300090c8:	4604      	mov	r4, r0
300090ca:	9101      	str	r1, [sp, #4]
300090cc:	4798      	blx	r3
300090ce:	4b09      	ldr	r3, [pc, #36]	; (300090f4 <rtl_crypto_sha2_init+0x30>)
300090d0:	9901      	ldr	r1, [sp, #4]
300090d2:	6018      	str	r0, [r3, #0]
300090d4:	b928      	cbnz	r0, 300090e2 <rtl_crypto_sha2_init+0x1e>
300090d6:	4620      	mov	r0, r4
300090d8:	b002      	add	sp, #8
300090da:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090de:	f002 b9ef 	b.w	3000b4c0 <__rtl_crypto_sha2_init_A_veneer>
300090e2:	4620      	mov	r0, r4
300090e4:	b002      	add	sp, #8
300090e6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300090ea:	f002 b991 	b.w	3000b410 <__rtl_crypto_sha2_init_B_veneer>
300090ee:	bf00      	nop
300090f0:	30008fc9 	.word	0x30008fc9
300090f4:	3000eb74 	.word	0x3000eb74

300090f8 <rtl_crypto_sha2_update>:
300090f8:	b513      	push	{r0, r1, r4, lr}
300090fa:	4b0b      	ldr	r3, [pc, #44]	; (30009128 <rtl_crypto_sha2_update+0x30>)
300090fc:	4604      	mov	r4, r0
300090fe:	e9cd 2100 	strd	r2, r1, [sp]
30009102:	4798      	blx	r3
30009104:	4b09      	ldr	r3, [pc, #36]	; (3000912c <rtl_crypto_sha2_update+0x34>)
30009106:	e9dd 2100 	ldrd	r2, r1, [sp]
3000910a:	6018      	str	r0, [r3, #0]
3000910c:	b928      	cbnz	r0, 3000911a <rtl_crypto_sha2_update+0x22>
3000910e:	4620      	mov	r0, r4
30009110:	b002      	add	sp, #8
30009112:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009116:	f002 b9ab 	b.w	3000b470 <__rtl_crypto_sha2_update_A_veneer>
3000911a:	4620      	mov	r0, r4
3000911c:	b002      	add	sp, #8
3000911e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009122:	f002 b8ed 	b.w	3000b300 <__rtl_crypto_sha2_update_B_veneer>
30009126:	bf00      	nop
30009128:	30008fc9 	.word	0x30008fc9
3000912c:	3000eb74 	.word	0x3000eb74

30009130 <rtl_crypto_sha2_final>:
30009130:	b513      	push	{r0, r1, r4, lr}
30009132:	4b0a      	ldr	r3, [pc, #40]	; (3000915c <rtl_crypto_sha2_final+0x2c>)
30009134:	4604      	mov	r4, r0
30009136:	9101      	str	r1, [sp, #4]
30009138:	4798      	blx	r3
3000913a:	4b09      	ldr	r3, [pc, #36]	; (30009160 <rtl_crypto_sha2_final+0x30>)
3000913c:	9901      	ldr	r1, [sp, #4]
3000913e:	6018      	str	r0, [r3, #0]
30009140:	b928      	cbnz	r0, 3000914e <rtl_crypto_sha2_final+0x1e>
30009142:	4620      	mov	r0, r4
30009144:	b002      	add	sp, #8
30009146:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000914a:	f002 b949 	b.w	3000b3e0 <__rtl_crypto_sha2_final_A_veneer>
3000914e:	4620      	mov	r0, r4
30009150:	b002      	add	sp, #8
30009152:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009156:	f002 b963 	b.w	3000b420 <__rtl_crypto_sha2_final_B_veneer>
3000915a:	bf00      	nop
3000915c:	30008fc9 	.word	0x30008fc9
30009160:	3000eb74 	.word	0x3000eb74

30009164 <SBOOT_Validate_PubKey>:
30009164:	b513      	push	{r0, r1, r4, lr}
30009166:	4b0b      	ldr	r3, [pc, #44]	; (30009194 <SBOOT_Validate_PubKey+0x30>)
30009168:	4604      	mov	r4, r0
3000916a:	e9cd 2100 	strd	r2, r1, [sp]
3000916e:	4798      	blx	r3
30009170:	4b09      	ldr	r3, [pc, #36]	; (30009198 <SBOOT_Validate_PubKey+0x34>)
30009172:	e9dd 2100 	ldrd	r2, r1, [sp]
30009176:	6018      	str	r0, [r3, #0]
30009178:	b928      	cbnz	r0, 30009186 <SBOOT_Validate_PubKey+0x22>
3000917a:	4620      	mov	r0, r4
3000917c:	b002      	add	sp, #8
3000917e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30009182:	f002 b925 	b.w	3000b3d0 <__SBOOT_Validate_PubKey_A_veneer>
30009186:	4620      	mov	r0, r4
30009188:	b002      	add	sp, #8
3000918a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000918e:	f002 b8af 	b.w	3000b2f0 <__SBOOT_Validate_PubKey_B_veneer>
30009192:	bf00      	nop
30009194:	30008fc9 	.word	0x30008fc9
30009198:	3000eb74 	.word	0x3000eb74

3000919c <SBOOT_Validate_Signature>:
3000919c:	b5f0      	push	{r4, r5, r6, r7, lr}
3000919e:	b085      	sub	sp, #20
300091a0:	4f0d      	ldr	r7, [pc, #52]	; (300091d8 <SBOOT_Validate_Signature+0x3c>)
300091a2:	4604      	mov	r4, r0
300091a4:	480d      	ldr	r0, [pc, #52]	; (300091dc <SBOOT_Validate_Signature+0x40>)
300091a6:	9301      	str	r3, [sp, #4]
300091a8:	e9dd 560a 	ldrd	r5, r6, [sp, #40]	; 0x28
300091ac:	e9cd 2102 	strd	r2, r1, [sp, #8]
300091b0:	4780      	blx	r0
300091b2:	9903      	ldr	r1, [sp, #12]
300091b4:	6038      	str	r0, [r7, #0]
300091b6:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300091ba:	e9cd 560a 	strd	r5, r6, [sp, #40]	; 0x28
300091be:	b928      	cbnz	r0, 300091cc <SBOOT_Validate_Signature+0x30>
300091c0:	4620      	mov	r0, r4
300091c2:	b005      	add	sp, #20
300091c4:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
300091c8:	f002 b8ea 	b.w	3000b3a0 <__SBOOT_Validate_Signature_A_veneer>
300091cc:	4620      	mov	r0, r4
300091ce:	b005      	add	sp, #20
300091d0:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
300091d4:	f002 b884 	b.w	3000b2e0 <__SBOOT_Validate_Signature_B_veneer>
300091d8:	3000eb74 	.word	0x3000eb74
300091dc:	30008fc9 	.word	0x30008fc9

300091e0 <SBOOT_Validate_ImgHash>:
300091e0:	b530      	push	{r4, r5, lr}
300091e2:	4d0c      	ldr	r5, [pc, #48]	; (30009214 <SBOOT_Validate_ImgHash+0x34>)
300091e4:	b085      	sub	sp, #20
300091e6:	4604      	mov	r4, r0
300091e8:	480b      	ldr	r0, [pc, #44]	; (30009218 <SBOOT_Validate_ImgHash+0x38>)
300091ea:	9301      	str	r3, [sp, #4]
300091ec:	e9cd 2102 	strd	r2, r1, [sp, #8]
300091f0:	4780      	blx	r0
300091f2:	9903      	ldr	r1, [sp, #12]
300091f4:	6028      	str	r0, [r5, #0]
300091f6:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300091fa:	b928      	cbnz	r0, 30009208 <SBOOT_Validate_ImgHash+0x28>
300091fc:	4620      	mov	r0, r4
300091fe:	b005      	add	sp, #20
30009200:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30009204:	f002 b88c 	b.w	3000b320 <__SBOOT_Validate_ImgHash_A_veneer>
30009208:	4620      	mov	r0, r4
3000920a:	b005      	add	sp, #20
3000920c:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30009210:	f002 b896 	b.w	3000b340 <__SBOOT_Validate_ImgHash_B_veneer>
30009214:	3000eb74 	.word	0x3000eb74
30009218:	30008fc9 	.word	0x30008fc9

3000921c <CRYPTO_SHA_Init>:
3000921c:	b508      	push	{r3, lr}
3000921e:	4b07      	ldr	r3, [pc, #28]	; (3000923c <CRYPTO_SHA_Init+0x20>)
30009220:	4798      	blx	r3
30009222:	4a07      	ldr	r2, [pc, #28]	; (30009240 <CRYPTO_SHA_Init+0x24>)
30009224:	6010      	str	r0, [r2, #0]
30009226:	b918      	cbnz	r0, 30009230 <CRYPTO_SHA_Init+0x14>
30009228:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
3000922c:	f002 b8e8 	b.w	3000b400 <__CRYPTO_SHA_Init_A_veneer>
30009230:	2000      	movs	r0, #0
30009232:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
30009236:	f002 b933 	b.w	3000b4a0 <__CRYPTO_SHA_Init_B_veneer>
3000923a:	bf00      	nop
3000923c:	30008fc9 	.word	0x30008fc9
30009240:	3000eb74 	.word	0x3000eb74

30009244 <SBOOT_Validate_Algorithm>:
30009244:	b530      	push	{r4, r5, lr}
30009246:	4d0c      	ldr	r5, [pc, #48]	; (30009278 <SBOOT_Validate_Algorithm+0x34>)
30009248:	b085      	sub	sp, #20
3000924a:	4604      	mov	r4, r0
3000924c:	480b      	ldr	r0, [pc, #44]	; (3000927c <SBOOT_Validate_Algorithm+0x38>)
3000924e:	9301      	str	r3, [sp, #4]
30009250:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009254:	4780      	blx	r0
30009256:	9903      	ldr	r1, [sp, #12]
30009258:	6028      	str	r0, [r5, #0]
3000925a:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000925e:	b928      	cbnz	r0, 3000926c <SBOOT_Validate_Algorithm+0x28>
30009260:	4620      	mov	r0, r4
30009262:	b005      	add	sp, #20
30009264:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30009268:	f002 b8aa 	b.w	3000b3c0 <__SBOOT_Validate_Algorithm_A_veneer>
3000926c:	4620      	mov	r0, r4
3000926e:	b005      	add	sp, #20
30009270:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30009274:	f002 b8ec 	b.w	3000b450 <__SBOOT_Validate_Algorithm_B_veneer>
30009278:	3000eb74 	.word	0x3000eb74
3000927c:	30008fc9 	.word	0x30008fc9

30009280 <ddr_init_index>:
30009280:	b570      	push	{r4, r5, r6, lr}
30009282:	4b0f      	ldr	r3, [pc, #60]	; (300092c0 <ddr_init_index+0x40>)
30009284:	4798      	blx	r3
30009286:	4604      	mov	r4, r0
30009288:	4b0e      	ldr	r3, [pc, #56]	; (300092c4 <ddr_init_index+0x44>)
3000928a:	4798      	blx	r3
3000928c:	2c03      	cmp	r4, #3
3000928e:	d012      	beq.n	300092b6 <ddr_init_index+0x36>
30009290:	2c01      	cmp	r4, #1
30009292:	d103      	bne.n	3000929c <ddr_init_index+0x1c>
30009294:	2802      	cmp	r0, #2
30009296:	d00e      	beq.n	300092b6 <ddr_init_index+0x36>
30009298:	2803      	cmp	r0, #3
3000929a:	d00e      	beq.n	300092ba <ddr_init_index+0x3a>
3000929c:	4e0a      	ldr	r6, [pc, #40]	; (300092c8 <ddr_init_index+0x48>)
3000929e:	4d0b      	ldr	r5, [pc, #44]	; (300092cc <ddr_init_index+0x4c>)
300092a0:	4c0b      	ldr	r4, [pc, #44]	; (300092d0 <ddr_init_index+0x50>)
300092a2:	4633      	mov	r3, r6
300092a4:	462a      	mov	r2, r5
300092a6:	4908      	ldr	r1, [pc, #32]	; (300092c8 <ddr_init_index+0x48>)
300092a8:	2002      	movs	r0, #2
300092aa:	f001 fdfd 	bl	3000aea8 <rtk_log_write>
300092ae:	f242 7010 	movw	r0, #10000	; 0x2710
300092b2:	47a0      	blx	r4
300092b4:	e7f5      	b.n	300092a2 <ddr_init_index+0x22>
300092b6:	4620      	mov	r0, r4
300092b8:	bd70      	pop	{r4, r5, r6, pc}
300092ba:	2000      	movs	r0, #0
300092bc:	e7fc      	b.n	300092b8 <ddr_init_index+0x38>
300092be:	bf00      	nop
300092c0:	30007405 	.word	0x30007405
300092c4:	300072b5 	.word	0x300072b5
300092c8:	3000bcb6 	.word	0x3000bcb6
300092cc:	3000bcba 	.word	0x3000bcba
300092d0:	00009be5 	.word	0x00009be5

300092d4 <rxi316_perf_tune>:
300092d4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300092d8:	b0c1      	sub	sp, #260	; 0x104
300092da:	9000      	str	r0, [sp, #0]
300092dc:	6800      	ldr	r0, [r0, #0]
300092de:	7985      	ldrb	r5, [r0, #6]
300092e0:	2d00      	cmp	r5, #0
300092e2:	f000 82f7 	beq.w	300098d4 <rxi316_perf_tune+0x600>
300092e6:	2d01      	cmp	r5, #1
300092e8:	d003      	beq.n	300092f2 <rxi316_perf_tune+0x1e>
300092ea:	2d03      	cmp	r5, #3
300092ec:	bf14      	ite	ne
300092ee:	2502      	movne	r5, #2
300092f0:	2500      	moveq	r5, #0
300092f2:	9b00      	ldr	r3, [sp, #0]
300092f4:	2420      	movs	r4, #32
300092f6:	68db      	ldr	r3, [r3, #12]
300092f8:	4626      	mov	r6, r4
300092fa:	46a1      	mov	r9, r4
300092fc:	46a3      	mov	fp, r4
300092fe:	685b      	ldr	r3, [r3, #4]
30009300:	46a2      	mov	sl, r4
30009302:	46a0      	mov	r8, r4
30009304:	781a      	ldrb	r2, [r3, #0]
30009306:	9220      	str	r2, [sp, #128]	; 0x80
30009308:	785a      	ldrb	r2, [r3, #1]
3000930a:	9221      	str	r2, [sp, #132]	; 0x84
3000930c:	789a      	ldrb	r2, [r3, #2]
3000930e:	9222      	str	r2, [sp, #136]	; 0x88
30009310:	78da      	ldrb	r2, [r3, #3]
30009312:	9223      	str	r2, [sp, #140]	; 0x8c
30009314:	791a      	ldrb	r2, [r3, #4]
30009316:	9224      	str	r2, [sp, #144]	; 0x90
30009318:	795a      	ldrb	r2, [r3, #5]
3000931a:	9225      	str	r2, [sp, #148]	; 0x94
3000931c:	799a      	ldrb	r2, [r3, #6]
3000931e:	9226      	str	r2, [sp, #152]	; 0x98
30009320:	79da      	ldrb	r2, [r3, #7]
30009322:	9227      	str	r2, [sp, #156]	; 0x9c
30009324:	7a1a      	ldrb	r2, [r3, #8]
30009326:	9228      	str	r2, [sp, #160]	; 0xa0
30009328:	7a5a      	ldrb	r2, [r3, #9]
3000932a:	9229      	str	r2, [sp, #164]	; 0xa4
3000932c:	7a9a      	ldrb	r2, [r3, #10]
3000932e:	922a      	str	r2, [sp, #168]	; 0xa8
30009330:	7ada      	ldrb	r2, [r3, #11]
30009332:	922b      	str	r2, [sp, #172]	; 0xac
30009334:	7b1a      	ldrb	r2, [r3, #12]
30009336:	922c      	str	r2, [sp, #176]	; 0xb0
30009338:	7b5a      	ldrb	r2, [r3, #13]
3000933a:	922d      	str	r2, [sp, #180]	; 0xb4
3000933c:	7b9a      	ldrb	r2, [r3, #14]
3000933e:	922e      	str	r2, [sp, #184]	; 0xb8
30009340:	7bda      	ldrb	r2, [r3, #15]
30009342:	922f      	str	r2, [sp, #188]	; 0xbc
30009344:	7c1a      	ldrb	r2, [r3, #16]
30009346:	9230      	str	r2, [sp, #192]	; 0xc0
30009348:	7c5a      	ldrb	r2, [r3, #17]
3000934a:	9231      	str	r2, [sp, #196]	; 0xc4
3000934c:	7c9a      	ldrb	r2, [r3, #18]
3000934e:	9232      	str	r2, [sp, #200]	; 0xc8
30009350:	7cda      	ldrb	r2, [r3, #19]
30009352:	9233      	str	r2, [sp, #204]	; 0xcc
30009354:	7d1a      	ldrb	r2, [r3, #20]
30009356:	9234      	str	r2, [sp, #208]	; 0xd0
30009358:	7d5a      	ldrb	r2, [r3, #21]
3000935a:	9235      	str	r2, [sp, #212]	; 0xd4
3000935c:	7d9a      	ldrb	r2, [r3, #22]
3000935e:	9236      	str	r2, [sp, #216]	; 0xd8
30009360:	7dda      	ldrb	r2, [r3, #23]
30009362:	9237      	str	r2, [sp, #220]	; 0xdc
30009364:	7e1a      	ldrb	r2, [r3, #24]
30009366:	9238      	str	r2, [sp, #224]	; 0xe0
30009368:	7e5a      	ldrb	r2, [r3, #25]
3000936a:	9239      	str	r2, [sp, #228]	; 0xe4
3000936c:	7e9a      	ldrb	r2, [r3, #26]
3000936e:	923a      	str	r2, [sp, #232]	; 0xe8
30009370:	7eda      	ldrb	r2, [r3, #27]
30009372:	923b      	str	r2, [sp, #236]	; 0xec
30009374:	7f1a      	ldrb	r2, [r3, #28]
30009376:	923c      	str	r2, [sp, #240]	; 0xf0
30009378:	7f5a      	ldrb	r2, [r3, #29]
3000937a:	923d      	str	r2, [sp, #244]	; 0xf4
3000937c:	7f9a      	ldrb	r2, [r3, #30]
3000937e:	923e      	str	r2, [sp, #248]	; 0xf8
30009380:	7fdb      	ldrb	r3, [r3, #31]
30009382:	941d      	str	r4, [sp, #116]	; 0x74
30009384:	933f      	str	r3, [sp, #252]	; 0xfc
30009386:	2300      	movs	r3, #0
30009388:	9408      	str	r4, [sp, #32]
3000938a:	4619      	mov	r1, r3
3000938c:	461f      	mov	r7, r3
3000938e:	9405      	str	r4, [sp, #20]
30009390:	9401      	str	r4, [sp, #4]
30009392:	9403      	str	r4, [sp, #12]
30009394:	9402      	str	r4, [sp, #8]
30009396:	e9cd 441b 	strd	r4, r4, [sp, #108]	; 0x6c
3000939a:	e9cd 4419 	strd	r4, r4, [sp, #100]	; 0x64
3000939e:	e9cd 4417 	strd	r4, r4, [sp, #92]	; 0x5c
300093a2:	e9cd 4415 	strd	r4, r4, [sp, #84]	; 0x54
300093a6:	e9cd 4413 	strd	r4, r4, [sp, #76]	; 0x4c
300093aa:	e9cd 4411 	strd	r4, r4, [sp, #68]	; 0x44
300093ae:	e9cd 440f 	strd	r4, r4, [sp, #60]	; 0x3c
300093b2:	e9cd 440d 	strd	r4, r4, [sp, #52]	; 0x34
300093b6:	e9cd 440b 	strd	r4, r4, [sp, #44]	; 0x2c
300093ba:	e9cd 4409 	strd	r4, r4, [sp, #36]	; 0x24
300093be:	e9cd 4406 	strd	r4, r4, [sp, #24]
300093c2:	aa20      	add	r2, sp, #128	; 0x80
300093c4:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
300093c8:	2a00      	cmp	r2, #0
300093ca:	f040 8285 	bne.w	300098d8 <rxi316_perf_tune+0x604>
300093ce:	3101      	adds	r1, #1
300093d0:	9313      	str	r3, [sp, #76]	; 0x4c
300093d2:	3301      	adds	r3, #1
300093d4:	2b20      	cmp	r3, #32
300093d6:	d1f4      	bne.n	300093c2 <rxi316_perf_tune+0xee>
300093d8:	7882      	ldrb	r2, [r0, #2]
300093da:	6883      	ldr	r3, [r0, #8]
300093dc:	2a00      	cmp	r2, #0
300093de:	f000 8311 	beq.w	30009a04 <rxi316_perf_tune+0x730>
300093e2:	2a01      	cmp	r2, #1
300093e4:	f000 8310 	beq.w	30009a08 <rxi316_perf_tune+0x734>
300093e8:	2a03      	cmp	r2, #3
300093ea:	bf0c      	ite	eq
300093ec:	2204      	moveq	r2, #4
300093ee:	2203      	movne	r2, #3
300093f0:	3b02      	subs	r3, #2
300093f2:	9204      	str	r2, [sp, #16]
300093f4:	428b      	cmp	r3, r1
300093f6:	d005      	beq.n	30009404 <rxi316_perf_tune+0x130>
300093f8:	4b07      	ldr	r3, [pc, #28]	; (30009418 <rxi316_perf_tune+0x144>)
300093fa:	2002      	movs	r0, #2
300093fc:	4a07      	ldr	r2, [pc, #28]	; (3000941c <rxi316_perf_tune+0x148>)
300093fe:	4619      	mov	r1, r3
30009400:	f001 fd52 	bl	3000aea8 <rtk_log_write>
30009404:	9b04      	ldr	r3, [sp, #16]
30009406:	42bb      	cmp	r3, r7
30009408:	d00c      	beq.n	30009424 <rxi316_perf_tune+0x150>
3000940a:	4b03      	ldr	r3, [pc, #12]	; (30009418 <rxi316_perf_tune+0x144>)
3000940c:	2002      	movs	r0, #2
3000940e:	4a04      	ldr	r2, [pc, #16]	; (30009420 <rxi316_perf_tune+0x14c>)
30009410:	4619      	mov	r1, r3
30009412:	f001 fd49 	bl	3000aea8 <rtk_log_write>
30009416:	e005      	b.n	30009424 <rxi316_perf_tune+0x150>
30009418:	3000bcb6 	.word	0x3000bcb6
3000941c:	3000bcdd 	.word	0x3000bcdd
30009420:	3000bd20 	.word	0x3000bd20
30009424:	f1ba 0f20 	cmp.w	sl, #32
30009428:	bf16      	itet	ne
3000942a:	ebaa 0a05 	subne.w	sl, sl, r5
3000942e:	230f      	moveq	r3, #15
30009430:	f1aa 0305 	subne.w	r3, sl, #5
30009434:	9304      	str	r3, [sp, #16]
30009436:	9b03      	ldr	r3, [sp, #12]
30009438:	2b20      	cmp	r3, #32
3000943a:	bf16      	itet	ne
3000943c:	1b5b      	subne	r3, r3, r5
3000943e:	230f      	moveq	r3, #15
30009440:	3b06      	subne	r3, #6
30009442:	f1b8 0f20 	cmp.w	r8, #32
30009446:	bf18      	it	ne
30009448:	eba8 0805 	subne.w	r8, r8, r5
3000944c:	9303      	str	r3, [sp, #12]
3000944e:	bf0c      	ite	eq
30009450:	230f      	moveq	r3, #15
30009452:	f1a8 0307 	subne.w	r3, r8, #7
30009456:	931e      	str	r3, [sp, #120]	; 0x78
30009458:	9b02      	ldr	r3, [sp, #8]
3000945a:	2b20      	cmp	r3, #32
3000945c:	bf16      	itet	ne
3000945e:	1b5b      	subne	r3, r3, r5
30009460:	230f      	moveq	r3, #15
30009462:	3b08      	subne	r3, #8
30009464:	9302      	str	r3, [sp, #8]
30009466:	9b01      	ldr	r3, [sp, #4]
30009468:	2b20      	cmp	r3, #32
3000946a:	bf16      	itet	ne
3000946c:	1b5b      	subne	r3, r3, r5
3000946e:	230f      	moveq	r3, #15
30009470:	3b09      	subne	r3, #9
30009472:	9301      	str	r3, [sp, #4]
30009474:	9b05      	ldr	r3, [sp, #20]
30009476:	2b20      	cmp	r3, #32
30009478:	bf16      	itet	ne
3000947a:	1b5b      	subne	r3, r3, r5
3000947c:	230f      	moveq	r3, #15
3000947e:	3b0a      	subne	r3, #10
30009480:	9305      	str	r3, [sp, #20]
30009482:	9b06      	ldr	r3, [sp, #24]
30009484:	2b20      	cmp	r3, #32
30009486:	bf16      	itet	ne
30009488:	1b5b      	subne	r3, r3, r5
3000948a:	230f      	moveq	r3, #15
3000948c:	3b0b      	subne	r3, #11
3000948e:	9306      	str	r3, [sp, #24]
30009490:	9b07      	ldr	r3, [sp, #28]
30009492:	2b20      	cmp	r3, #32
30009494:	bf16      	itet	ne
30009496:	1b5b      	subne	r3, r3, r5
30009498:	230f      	moveq	r3, #15
3000949a:	3b0c      	subne	r3, #12
3000949c:	f1bb 0f20 	cmp.w	fp, #32
300094a0:	bf18      	it	ne
300094a2:	ebab 0b05 	subne.w	fp, fp, r5
300094a6:	9307      	str	r3, [sp, #28]
300094a8:	bf0c      	ite	eq
300094aa:	230f      	moveq	r3, #15
300094ac:	f1ab 030d 	subne.w	r3, fp, #13
300094b0:	931f      	str	r3, [sp, #124]	; 0x7c
300094b2:	9b08      	ldr	r3, [sp, #32]
300094b4:	2b20      	cmp	r3, #32
300094b6:	bf16      	itet	ne
300094b8:	1b5b      	subne	r3, r3, r5
300094ba:	230f      	moveq	r3, #15
300094bc:	3b0e      	subne	r3, #14
300094be:	9308      	str	r3, [sp, #32]
300094c0:	9b09      	ldr	r3, [sp, #36]	; 0x24
300094c2:	2b20      	cmp	r3, #32
300094c4:	bf16      	itet	ne
300094c6:	eba3 0b05 	subne.w	fp, r3, r5
300094ca:	230f      	moveq	r3, #15
300094cc:	f1ab 030f 	subne.w	r3, fp, #15
300094d0:	9309      	str	r3, [sp, #36]	; 0x24
300094d2:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300094d4:	2b20      	cmp	r3, #32
300094d6:	bf16      	itet	ne
300094d8:	eba3 0b05 	subne.w	fp, r3, r5
300094dc:	230f      	moveq	r3, #15
300094de:	f1ab 0310 	subne.w	r3, fp, #16
300094e2:	930a      	str	r3, [sp, #40]	; 0x28
300094e4:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300094e6:	2b20      	cmp	r3, #32
300094e8:	bf16      	itet	ne
300094ea:	eba3 0b05 	subne.w	fp, r3, r5
300094ee:	230f      	moveq	r3, #15
300094f0:	f1ab 0311 	subne.w	r3, fp, #17
300094f4:	930b      	str	r3, [sp, #44]	; 0x2c
300094f6:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300094f8:	2b20      	cmp	r3, #32
300094fa:	bf16      	itet	ne
300094fc:	eba3 0b05 	subne.w	fp, r3, r5
30009500:	230f      	moveq	r3, #15
30009502:	f1ab 0312 	subne.w	r3, fp, #18
30009506:	930c      	str	r3, [sp, #48]	; 0x30
30009508:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000950a:	2b20      	cmp	r3, #32
3000950c:	bf16      	itet	ne
3000950e:	eba3 0b05 	subne.w	fp, r3, r5
30009512:	230f      	moveq	r3, #15
30009514:	f1ab 0313 	subne.w	r3, fp, #19
30009518:	930d      	str	r3, [sp, #52]	; 0x34
3000951a:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000951c:	2b20      	cmp	r3, #32
3000951e:	bf16      	itet	ne
30009520:	eba3 0b05 	subne.w	fp, r3, r5
30009524:	230f      	moveq	r3, #15
30009526:	f1ab 0314 	subne.w	r3, fp, #20
3000952a:	930e      	str	r3, [sp, #56]	; 0x38
3000952c:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000952e:	2b20      	cmp	r3, #32
30009530:	bf16      	itet	ne
30009532:	1b5b      	subne	r3, r3, r5
30009534:	230f      	moveq	r3, #15
30009536:	3b15      	subne	r3, #21
30009538:	930f      	str	r3, [sp, #60]	; 0x3c
3000953a:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000953c:	2b20      	cmp	r3, #32
3000953e:	bf16      	itet	ne
30009540:	eba3 0b05 	subne.w	fp, r3, r5
30009544:	231f      	moveq	r3, #31
30009546:	f1ab 0302 	subne.w	r3, fp, #2
3000954a:	9310      	str	r3, [sp, #64]	; 0x40
3000954c:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000954e:	2b20      	cmp	r3, #32
30009550:	bf16      	itet	ne
30009552:	1b5b      	subne	r3, r3, r5
30009554:	231f      	moveq	r3, #31
30009556:	3b02      	subne	r3, #2
30009558:	9311      	str	r3, [sp, #68]	; 0x44
3000955a:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000955c:	2b20      	cmp	r3, #32
3000955e:	bf16      	itet	ne
30009560:	eba3 0b05 	subne.w	fp, r3, r5
30009564:	231f      	moveq	r3, #31
30009566:	f1ab 0302 	subne.w	r3, fp, #2
3000956a:	9312      	str	r3, [sp, #72]	; 0x48
3000956c:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000956e:	2b20      	cmp	r3, #32
30009570:	bf16      	itet	ne
30009572:	eba3 0b05 	subne.w	fp, r3, r5
30009576:	230f      	moveq	r3, #15
30009578:	f1ab 0302 	subne.w	r3, fp, #2
3000957c:	9313      	str	r3, [sp, #76]	; 0x4c
3000957e:	9b14      	ldr	r3, [sp, #80]	; 0x50
30009580:	2b20      	cmp	r3, #32
30009582:	bf16      	itet	ne
30009584:	eba3 0b05 	subne.w	fp, r3, r5
30009588:	230f      	moveq	r3, #15
3000958a:	f1ab 0303 	subne.w	r3, fp, #3
3000958e:	9314      	str	r3, [sp, #80]	; 0x50
30009590:	9b15      	ldr	r3, [sp, #84]	; 0x54
30009592:	2b20      	cmp	r3, #32
30009594:	bf16      	itet	ne
30009596:	1b5b      	subne	r3, r3, r5
30009598:	230f      	moveq	r3, #15
3000959a:	3b04      	subne	r3, #4
3000959c:	9315      	str	r3, [sp, #84]	; 0x54
3000959e:	9b16      	ldr	r3, [sp, #88]	; 0x58
300095a0:	2b20      	cmp	r3, #32
300095a2:	bf16      	itet	ne
300095a4:	1b5b      	subne	r3, r3, r5
300095a6:	230f      	moveq	r3, #15
300095a8:	3b05      	subne	r3, #5
300095aa:	9316      	str	r3, [sp, #88]	; 0x58
300095ac:	9b17      	ldr	r3, [sp, #92]	; 0x5c
300095ae:	2b20      	cmp	r3, #32
300095b0:	bf18      	it	ne
300095b2:	1b5f      	subne	r7, r3, r5
300095b4:	9b18      	ldr	r3, [sp, #96]	; 0x60
300095b6:	bf0c      	ite	eq
300095b8:	270f      	moveq	r7, #15
300095ba:	3f06      	subne	r7, #6
300095bc:	2b20      	cmp	r3, #32
300095be:	bf18      	it	ne
300095c0:	1b58      	subne	r0, r3, r5
300095c2:	9b19      	ldr	r3, [sp, #100]	; 0x64
300095c4:	bf08      	it	eq
300095c6:	200f      	moveq	r0, #15
300095c8:	ea4f 4707 	mov.w	r7, r7, lsl #16
300095cc:	bf18      	it	ne
300095ce:	3807      	subne	r0, #7
300095d0:	2b20      	cmp	r3, #32
300095d2:	f407 2770 	and.w	r7, r7, #983040	; 0xf0000
300095d6:	bf18      	it	ne
300095d8:	1b59      	subne	r1, r3, r5
300095da:	9b1a      	ldr	r3, [sp, #104]	; 0x68
300095dc:	bf08      	it	eq
300095de:	210f      	moveq	r1, #15
300095e0:	ea4f 5000 	mov.w	r0, r0, lsl #20
300095e4:	bf18      	it	ne
300095e6:	3908      	subne	r1, #8
300095e8:	2b20      	cmp	r3, #32
300095ea:	f400 0070 	and.w	r0, r0, #15728640	; 0xf00000
300095ee:	bf14      	ite	ne
300095f0:	eba3 0a05 	subne.w	sl, r3, r5
300095f4:	f04f 0a0f 	moveq.w	sl, #15
300095f8:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
300095fa:	ea4f 6101 	mov.w	r1, r1, lsl #24
300095fe:	bf18      	it	ne
30009600:	f1aa 0a09 	subne.w	sl, sl, #9
30009604:	f1b9 0f20 	cmp.w	r9, #32
30009608:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000960c:	bf16      	itet	ne
3000960e:	eba9 0905 	subne.w	r9, r9, r5
30009612:	f04f 090f 	moveq.w	r9, #15
30009616:	f1a9 090a 	subne.w	r9, r9, #10
3000961a:	2b20      	cmp	r3, #32
3000961c:	bf18      	it	ne
3000961e:	1b5a      	subne	r2, r3, r5
30009620:	9b1c      	ldr	r3, [sp, #112]	; 0x70
30009622:	bf08      	it	eq
30009624:	220f      	moveq	r2, #15
30009626:	f009 090f 	and.w	r9, r9, #15
3000962a:	bf18      	it	ne
3000962c:	3a0b      	subne	r2, #11
3000962e:	2b20      	cmp	r3, #32
30009630:	bf14      	ite	ne
30009632:	eba3 0805 	subne.w	r8, r3, r5
30009636:	f04f 081f 	moveq.w	r8, #31
3000963a:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000963c:	ea4f 1202 	mov.w	r2, r2, lsl #4
30009640:	bf18      	it	ne
30009642:	f1a8 0802 	subne.w	r8, r8, #2
30009646:	2e20      	cmp	r6, #32
30009648:	b2d2      	uxtb	r2, r2
3000964a:	bf14      	ite	ne
3000964c:	eba6 0c05 	subne.w	ip, r6, r5
30009650:	f04f 0c1f 	moveq.w	ip, #31
30009654:	ea42 0209 	orr.w	r2, r2, r9
30009658:	bf18      	it	ne
3000965a:	f1ac 0c02 	subne.w	ip, ip, #2
3000965e:	2b20      	cmp	r3, #32
30009660:	bf14      	ite	ne
30009662:	eba3 0e05 	subne.w	lr, r3, r5
30009666:	f04f 0e1f 	moveq.w	lr, #31
3000966a:	9b03      	ldr	r3, [sp, #12]
3000966c:	bf18      	it	ne
3000966e:	f1ae 0e02 	subne.w	lr, lr, #2
30009672:	2c20      	cmp	r4, #32
30009674:	ea4f 1303 	mov.w	r3, r3, lsl #4
30009678:	bf18      	it	ne
3000967a:	1b64      	subne	r4, r4, r5
3000967c:	9d04      	ldr	r5, [sp, #16]
3000967e:	b2db      	uxtb	r3, r3
30009680:	bf08      	it	eq
30009682:	241f      	moveq	r4, #31
30009684:	f005 050f 	and.w	r5, r5, #15
30009688:	bf18      	it	ne
3000968a:	3c02      	subne	r4, #2
3000968c:	f00e 0e1f 	and.w	lr, lr, #31
30009690:	432b      	orrs	r3, r5
30009692:	9d1e      	ldr	r5, [sp, #120]	; 0x78
30009694:	0164      	lsls	r4, r4, #5
30009696:	022d      	lsls	r5, r5, #8
30009698:	f404 7478 	and.w	r4, r4, #992	; 0x3e0
3000969c:	f405 6570 	and.w	r5, r5, #3840	; 0xf00
300096a0:	ea44 040e 	orr.w	r4, r4, lr
300096a4:	432b      	orrs	r3, r5
300096a6:	9d02      	ldr	r5, [sp, #8]
300096a8:	032d      	lsls	r5, r5, #12
300096aa:	b2ad      	uxth	r5, r5
300096ac:	432b      	orrs	r3, r5
300096ae:	9d01      	ldr	r5, [sp, #4]
300096b0:	042d      	lsls	r5, r5, #16
300096b2:	f405 2570 	and.w	r5, r5, #983040	; 0xf0000
300096b6:	432b      	orrs	r3, r5
300096b8:	9d05      	ldr	r5, [sp, #20]
300096ba:	052d      	lsls	r5, r5, #20
300096bc:	f405 0570 	and.w	r5, r5, #15728640	; 0xf00000
300096c0:	432b      	orrs	r3, r5
300096c2:	9d06      	ldr	r5, [sp, #24]
300096c4:	062d      	lsls	r5, r5, #24
300096c6:	f005 6570 	and.w	r5, r5, #251658240	; 0xf000000
300096ca:	431d      	orrs	r5, r3
300096cc:	9b07      	ldr	r3, [sp, #28]
300096ce:	ea45 7503 	orr.w	r5, r5, r3, lsl #28
300096d2:	4ba7      	ldr	r3, [pc, #668]	; (30009970 <rxi316_perf_tune+0x69c>)
300096d4:	f8c3 5400 	str.w	r5, [r3, #1024]	; 0x400
300096d8:	9d08      	ldr	r5, [sp, #32]
300096da:	9e1f      	ldr	r6, [sp, #124]	; 0x7c
300096dc:	012d      	lsls	r5, r5, #4
300096de:	f006 0b0f 	and.w	fp, r6, #15
300096e2:	9e09      	ldr	r6, [sp, #36]	; 0x24
300096e4:	b2ed      	uxtb	r5, r5
300096e6:	ea45 050b 	orr.w	r5, r5, fp
300096ea:	ea4f 2b06 	mov.w	fp, r6, lsl #8
300096ee:	9e0a      	ldr	r6, [sp, #40]	; 0x28
300096f0:	f40b 6b70 	and.w	fp, fp, #3840	; 0xf00
300096f4:	ea45 050b 	orr.w	r5, r5, fp
300096f8:	ea4f 3b06 	mov.w	fp, r6, lsl #12
300096fc:	9e0b      	ldr	r6, [sp, #44]	; 0x2c
300096fe:	fa1f fb8b 	uxth.w	fp, fp
30009702:	ea45 050b 	orr.w	r5, r5, fp
30009706:	ea4f 4b06 	mov.w	fp, r6, lsl #16
3000970a:	9e0c      	ldr	r6, [sp, #48]	; 0x30
3000970c:	f40b 2b70 	and.w	fp, fp, #983040	; 0xf0000
30009710:	ea45 050b 	orr.w	r5, r5, fp
30009714:	ea4f 5b06 	mov.w	fp, r6, lsl #20
30009718:	9e0d      	ldr	r6, [sp, #52]	; 0x34
3000971a:	f40b 0b70 	and.w	fp, fp, #15728640	; 0xf00000
3000971e:	ea45 050b 	orr.w	r5, r5, fp
30009722:	ea4f 6b06 	mov.w	fp, r6, lsl #24
30009726:	9e0e      	ldr	r6, [sp, #56]	; 0x38
30009728:	f00b 6b70 	and.w	fp, fp, #251658240	; 0xf000000
3000972c:	ea45 050b 	orr.w	r5, r5, fp
30009730:	ea45 7506 	orr.w	r5, r5, r6, lsl #28
30009734:	f8c3 5404 	str.w	r5, [r3, #1028]	; 0x404
30009738:	9d0f      	ldr	r5, [sp, #60]	; 0x3c
3000973a:	f005 050f 	and.w	r5, r5, #15
3000973e:	f8c3 5408 	str.w	r5, [r3, #1032]	; 0x408
30009742:	9d11      	ldr	r5, [sp, #68]	; 0x44
30009744:	9e10      	ldr	r6, [sp, #64]	; 0x40
30009746:	016d      	lsls	r5, r5, #5
30009748:	f006 0b1f 	and.w	fp, r6, #31
3000974c:	9e12      	ldr	r6, [sp, #72]	; 0x48
3000974e:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
30009752:	ea45 050b 	orr.w	r5, r5, fp
30009756:	ea4f 2b86 	mov.w	fp, r6, lsl #10
3000975a:	f40b 4bf8 	and.w	fp, fp, #31744	; 0x7c00
3000975e:	ea45 050b 	orr.w	r5, r5, fp
30009762:	f8c3 540c 	str.w	r5, [r3, #1036]	; 0x40c
30009766:	9d14      	ldr	r5, [sp, #80]	; 0x50
30009768:	9e16      	ldr	r6, [sp, #88]	; 0x58
3000976a:	ea4f 1b05 	mov.w	fp, r5, lsl #4
3000976e:	9d13      	ldr	r5, [sp, #76]	; 0x4c
30009770:	fa5f fb8b 	uxtb.w	fp, fp
30009774:	f005 050f 	and.w	r5, r5, #15
30009778:	ea4b 0b05 	orr.w	fp, fp, r5
3000977c:	9d15      	ldr	r5, [sp, #84]	; 0x54
3000977e:	022d      	lsls	r5, r5, #8
30009780:	f405 6570 	and.w	r5, r5, #3840	; 0xf00
30009784:	ea4b 0505 	orr.w	r5, fp, r5
30009788:	ea4f 3b06 	mov.w	fp, r6, lsl #12
3000978c:	fa1f fb8b 	uxth.w	fp, fp
30009790:	ea45 050b 	orr.w	r5, r5, fp
30009794:	433d      	orrs	r5, r7
30009796:	4328      	orrs	r0, r5
30009798:	4301      	orrs	r1, r0
3000979a:	ea41 710a 	orr.w	r1, r1, sl, lsl #28
3000979e:	f8c3 1410 	str.w	r1, [r3, #1040]	; 0x410
300097a2:	f8c3 2414 	str.w	r2, [r3, #1044]	; 0x414
300097a6:	f008 021f 	and.w	r2, r8, #31
300097aa:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
300097ae:	f00c 021f 	and.w	r2, ip, #31
300097b2:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
300097b6:	f8c3 4420 	str.w	r4, [r3, #1056]	; 0x420
300097ba:	9a00      	ldr	r2, [sp, #0]
300097bc:	68d2      	ldr	r2, [r2, #12]
300097be:	6891      	ldr	r1, [r2, #8]
300097c0:	e9d1 2000 	ldrd	r2, r0, [r1]
300097c4:	f002 0201 	and.w	r2, r2, #1
300097c8:	ea42 4200 	orr.w	r2, r2, r0, lsl #16
300097cc:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
300097d0:	9a00      	ldr	r2, [sp, #0]
300097d2:	68d2      	ldr	r2, [r2, #12]
300097d4:	6890      	ldr	r0, [r2, #8]
300097d6:	6941      	ldr	r1, [r0, #20]
300097d8:	7a02      	ldrb	r2, [r0, #8]
300097da:	ea42 6201 	orr.w	r2, r2, r1, lsl #24
300097de:	68c1      	ldr	r1, [r0, #12]
300097e0:	0209      	lsls	r1, r1, #8
300097e2:	b289      	uxth	r1, r1
300097e4:	430a      	orrs	r2, r1
300097e6:	6901      	ldr	r1, [r0, #16]
300097e8:	0409      	lsls	r1, r1, #16
300097ea:	f401 017f 	and.w	r1, r1, #16711680	; 0xff0000
300097ee:	430a      	orrs	r2, r1
300097f0:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
300097f4:	9a00      	ldr	r2, [sp, #0]
300097f6:	68d2      	ldr	r2, [r2, #12]
300097f8:	6890      	ldr	r0, [r2, #8]
300097fa:	69c2      	ldr	r2, [r0, #28]
300097fc:	6a01      	ldr	r1, [r0, #32]
300097fe:	0212      	lsls	r2, r2, #8
30009800:	0409      	lsls	r1, r1, #16
30009802:	b292      	uxth	r2, r2
30009804:	f401 017f 	and.w	r1, r1, #16711680	; 0xff0000
30009808:	430a      	orrs	r2, r1
3000980a:	7e01      	ldrb	r1, [r0, #24]
3000980c:	430a      	orrs	r2, r1
3000980e:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30009812:	9a00      	ldr	r2, [sp, #0]
30009814:	68d2      	ldr	r2, [r2, #12]
30009816:	6891      	ldr	r1, [r2, #8]
30009818:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000981a:	6a49      	ldr	r1, [r1, #36]	; 0x24
3000981c:	0412      	lsls	r2, r2, #16
3000981e:	f001 013f 	and.w	r1, r1, #63	; 0x3f
30009822:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009826:	430a      	orrs	r2, r1
30009828:	f8c3 2480 	str.w	r2, [r3, #1152]	; 0x480
3000982c:	9a00      	ldr	r2, [sp, #0]
3000982e:	68d2      	ldr	r2, [r2, #12]
30009830:	6890      	ldr	r0, [r2, #8]
30009832:	6b02      	ldr	r2, [r0, #48]	; 0x30
30009834:	6b81      	ldr	r1, [r0, #56]	; 0x38
30009836:	0052      	lsls	r2, r2, #1
30009838:	0409      	lsls	r1, r1, #16
3000983a:	f002 0202 	and.w	r2, r2, #2
3000983e:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
30009842:	430a      	orrs	r2, r1
30009844:	6ac1      	ldr	r1, [r0, #44]	; 0x2c
30009846:	f001 0101 	and.w	r1, r1, #1
3000984a:	430a      	orrs	r2, r1
3000984c:	6b41      	ldr	r1, [r0, #52]	; 0x34
3000984e:	0209      	lsls	r1, r1, #8
30009850:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
30009854:	430a      	orrs	r2, r1
30009856:	f042 4280 	orr.w	r2, r2, #1073741824	; 0x40000000
3000985a:	f042 0208 	orr.w	r2, r2, #8
3000985e:	f8c3 2484 	str.w	r2, [r3, #1156]	; 0x484
30009862:	9a00      	ldr	r2, [sp, #0]
30009864:	68d2      	ldr	r2, [r2, #12]
30009866:	6892      	ldr	r2, [r2, #8]
30009868:	6bd2      	ldr	r2, [r2, #60]	; 0x3c
3000986a:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
3000986e:	9a00      	ldr	r2, [sp, #0]
30009870:	68d2      	ldr	r2, [r2, #12]
30009872:	6892      	ldr	r2, [r2, #8]
30009874:	6c12      	ldr	r2, [r2, #64]	; 0x40
30009876:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
3000987a:	9a00      	ldr	r2, [sp, #0]
3000987c:	68d2      	ldr	r2, [r2, #12]
3000987e:	6892      	ldr	r2, [r2, #8]
30009880:	6c52      	ldr	r2, [r2, #68]	; 0x44
30009882:	f8c3 2490 	str.w	r2, [r3, #1168]	; 0x490
30009886:	9a00      	ldr	r2, [sp, #0]
30009888:	68d2      	ldr	r2, [r2, #12]
3000988a:	6892      	ldr	r2, [r2, #8]
3000988c:	6c92      	ldr	r2, [r2, #72]	; 0x48
3000988e:	f8c3 2494 	str.w	r2, [r3, #1172]	; 0x494
30009892:	9a00      	ldr	r2, [sp, #0]
30009894:	68d2      	ldr	r2, [r2, #12]
30009896:	6892      	ldr	r2, [r2, #8]
30009898:	6cd2      	ldr	r2, [r2, #76]	; 0x4c
3000989a:	f8c3 2498 	str.w	r2, [r3, #1176]	; 0x498
3000989e:	9a00      	ldr	r2, [sp, #0]
300098a0:	68d2      	ldr	r2, [r2, #12]
300098a2:	6892      	ldr	r2, [r2, #8]
300098a4:	6d12      	ldr	r2, [r2, #80]	; 0x50
300098a6:	f002 020f 	and.w	r2, r2, #15
300098aa:	f8c3 24a0 	str.w	r2, [r3, #1184]	; 0x4a0
300098ae:	f8d3 2470 	ldr.w	r2, [r3, #1136]	; 0x470
300098b2:	f022 023f 	bic.w	r2, r2, #63	; 0x3f
300098b6:	f042 0204 	orr.w	r2, r2, #4
300098ba:	f8c3 2470 	str.w	r2, [r3, #1136]	; 0x470
300098be:	f8d3 2474 	ldr.w	r2, [r3, #1140]	; 0x474
300098c2:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
300098c6:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
300098ca:	f8c3 2474 	str.w	r2, [r3, #1140]	; 0x474
300098ce:	b041      	add	sp, #260	; 0x104
300098d0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300098d4:	2502      	movs	r5, #2
300098d6:	e50c      	b.n	300092f2 <rxi316_perf_tune+0x1e>
300098d8:	2a01      	cmp	r2, #1
300098da:	d102      	bne.n	300098e2 <rxi316_perf_tune+0x60e>
300098dc:	3101      	adds	r1, #1
300098de:	9314      	str	r3, [sp, #80]	; 0x50
300098e0:	e577      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300098e2:	2a02      	cmp	r2, #2
300098e4:	d102      	bne.n	300098ec <rxi316_perf_tune+0x618>
300098e6:	3101      	adds	r1, #1
300098e8:	9315      	str	r3, [sp, #84]	; 0x54
300098ea:	e572      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300098ec:	2a03      	cmp	r2, #3
300098ee:	d102      	bne.n	300098f6 <rxi316_perf_tune+0x622>
300098f0:	3101      	adds	r1, #1
300098f2:	9316      	str	r3, [sp, #88]	; 0x58
300098f4:	e56d      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300098f6:	2a04      	cmp	r2, #4
300098f8:	d102      	bne.n	30009900 <rxi316_perf_tune+0x62c>
300098fa:	3101      	adds	r1, #1
300098fc:	9317      	str	r3, [sp, #92]	; 0x5c
300098fe:	e568      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009900:	2a05      	cmp	r2, #5
30009902:	d102      	bne.n	3000990a <rxi316_perf_tune+0x636>
30009904:	3101      	adds	r1, #1
30009906:	9318      	str	r3, [sp, #96]	; 0x60
30009908:	e563      	b.n	300093d2 <rxi316_perf_tune+0xfe>
3000990a:	2a06      	cmp	r2, #6
3000990c:	d102      	bne.n	30009914 <rxi316_perf_tune+0x640>
3000990e:	3101      	adds	r1, #1
30009910:	9319      	str	r3, [sp, #100]	; 0x64
30009912:	e55e      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009914:	2a07      	cmp	r2, #7
30009916:	d102      	bne.n	3000991e <rxi316_perf_tune+0x64a>
30009918:	3101      	adds	r1, #1
3000991a:	931a      	str	r3, [sp, #104]	; 0x68
3000991c:	e559      	b.n	300093d2 <rxi316_perf_tune+0xfe>
3000991e:	2a08      	cmp	r2, #8
30009920:	d102      	bne.n	30009928 <rxi316_perf_tune+0x654>
30009922:	3101      	adds	r1, #1
30009924:	4699      	mov	r9, r3
30009926:	e554      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009928:	2a09      	cmp	r2, #9
3000992a:	d102      	bne.n	30009932 <rxi316_perf_tune+0x65e>
3000992c:	3101      	adds	r1, #1
3000992e:	931b      	str	r3, [sp, #108]	; 0x6c
30009930:	e54f      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009932:	2a0a      	cmp	r2, #10
30009934:	d102      	bne.n	3000993c <rxi316_perf_tune+0x668>
30009936:	3701      	adds	r7, #1
30009938:	9310      	str	r3, [sp, #64]	; 0x40
3000993a:	e54a      	b.n	300093d2 <rxi316_perf_tune+0xfe>
3000993c:	2a0b      	cmp	r2, #11
3000993e:	d102      	bne.n	30009946 <rxi316_perf_tune+0x672>
30009940:	3701      	adds	r7, #1
30009942:	9311      	str	r3, [sp, #68]	; 0x44
30009944:	e545      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009946:	2a0c      	cmp	r2, #12
30009948:	d102      	bne.n	30009950 <rxi316_perf_tune+0x67c>
3000994a:	3701      	adds	r7, #1
3000994c:	9312      	str	r3, [sp, #72]	; 0x48
3000994e:	e540      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009950:	2a0d      	cmp	r2, #13
30009952:	d102      	bne.n	3000995a <rxi316_perf_tune+0x686>
30009954:	3701      	adds	r7, #1
30009956:	931d      	str	r3, [sp, #116]	; 0x74
30009958:	e53b      	b.n	300093d2 <rxi316_perf_tune+0xfe>
3000995a:	2a0e      	cmp	r2, #14
3000995c:	d102      	bne.n	30009964 <rxi316_perf_tune+0x690>
3000995e:	3701      	adds	r7, #1
30009960:	461c      	mov	r4, r3
30009962:	e536      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009964:	2a0f      	cmp	r2, #15
30009966:	d105      	bne.n	30009974 <rxi316_perf_tune+0x6a0>
30009968:	3701      	adds	r7, #1
3000996a:	931c      	str	r3, [sp, #112]	; 0x70
3000996c:	e531      	b.n	300093d2 <rxi316_perf_tune+0xfe>
3000996e:	bf00      	nop
30009970:	41100000 	.word	0x41100000
30009974:	2a10      	cmp	r2, #16
30009976:	d023      	beq.n	300099c0 <rxi316_perf_tune+0x6ec>
30009978:	2a11      	cmp	r2, #17
3000997a:	d023      	beq.n	300099c4 <rxi316_perf_tune+0x6f0>
3000997c:	2a12      	cmp	r2, #18
3000997e:	d023      	beq.n	300099c8 <rxi316_perf_tune+0x6f4>
30009980:	2a13      	cmp	r2, #19
30009982:	d023      	beq.n	300099cc <rxi316_perf_tune+0x6f8>
30009984:	2a14      	cmp	r2, #20
30009986:	d023      	beq.n	300099d0 <rxi316_perf_tune+0x6fc>
30009988:	2a15      	cmp	r2, #21
3000998a:	d023      	beq.n	300099d4 <rxi316_perf_tune+0x700>
3000998c:	2a16      	cmp	r2, #22
3000998e:	d023      	beq.n	300099d8 <rxi316_perf_tune+0x704>
30009990:	2a17      	cmp	r2, #23
30009992:	d023      	beq.n	300099dc <rxi316_perf_tune+0x708>
30009994:	2a18      	cmp	r2, #24
30009996:	d023      	beq.n	300099e0 <rxi316_perf_tune+0x70c>
30009998:	2a19      	cmp	r2, #25
3000999a:	d023      	beq.n	300099e4 <rxi316_perf_tune+0x710>
3000999c:	2a1a      	cmp	r2, #26
3000999e:	d023      	beq.n	300099e8 <rxi316_perf_tune+0x714>
300099a0:	2a1b      	cmp	r2, #27
300099a2:	d023      	beq.n	300099ec <rxi316_perf_tune+0x718>
300099a4:	2a1c      	cmp	r2, #28
300099a6:	d023      	beq.n	300099f0 <rxi316_perf_tune+0x71c>
300099a8:	2a1d      	cmp	r2, #29
300099aa:	d023      	beq.n	300099f4 <rxi316_perf_tune+0x720>
300099ac:	2a1e      	cmp	r2, #30
300099ae:	d023      	beq.n	300099f8 <rxi316_perf_tune+0x724>
300099b0:	2a1f      	cmp	r2, #31
300099b2:	d023      	beq.n	300099fc <rxi316_perf_tune+0x728>
300099b4:	2a20      	cmp	r2, #32
300099b6:	d023      	beq.n	30009a00 <rxi316_perf_tune+0x72c>
300099b8:	2a21      	cmp	r2, #33	; 0x21
300099ba:	bf08      	it	eq
300099bc:	461e      	moveq	r6, r3
300099be:	e508      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099c0:	469a      	mov	sl, r3
300099c2:	e506      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099c4:	9303      	str	r3, [sp, #12]
300099c6:	e504      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099c8:	4698      	mov	r8, r3
300099ca:	e502      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099cc:	9302      	str	r3, [sp, #8]
300099ce:	e500      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099d0:	9301      	str	r3, [sp, #4]
300099d2:	e4fe      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099d4:	9305      	str	r3, [sp, #20]
300099d6:	e4fc      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099d8:	9306      	str	r3, [sp, #24]
300099da:	e4fa      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099dc:	9307      	str	r3, [sp, #28]
300099de:	e4f8      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099e0:	469b      	mov	fp, r3
300099e2:	e4f6      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099e4:	9308      	str	r3, [sp, #32]
300099e6:	e4f4      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099e8:	9309      	str	r3, [sp, #36]	; 0x24
300099ea:	e4f2      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099ec:	930a      	str	r3, [sp, #40]	; 0x28
300099ee:	e4f0      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099f0:	930b      	str	r3, [sp, #44]	; 0x2c
300099f2:	e4ee      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099f4:	930c      	str	r3, [sp, #48]	; 0x30
300099f6:	e4ec      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099f8:	930d      	str	r3, [sp, #52]	; 0x34
300099fa:	e4ea      	b.n	300093d2 <rxi316_perf_tune+0xfe>
300099fc:	930e      	str	r3, [sp, #56]	; 0x38
300099fe:	e4e8      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009a00:	930f      	str	r3, [sp, #60]	; 0x3c
30009a02:	e4e6      	b.n	300093d2 <rxi316_perf_tune+0xfe>
30009a04:	2201      	movs	r2, #1
30009a06:	e4f3      	b.n	300093f0 <rxi316_perf_tune+0x11c>
30009a08:	2202      	movs	r2, #2
30009a0a:	e4f1      	b.n	300093f0 <rxi316_perf_tune+0x11c>

30009a0c <rxi316_dram_init>:
30009a0c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009a10:	4604      	mov	r4, r0
30009a12:	b08d      	sub	sp, #52	; 0x34
30009a14:	4bb1      	ldr	r3, [pc, #708]	; (30009cdc <rxi316_dram_init+0x2d0>)
30009a16:	4798      	blx	r3
30009a18:	f894 a014 	ldrb.w	sl, [r4, #20]
30009a1c:	2301      	movs	r3, #1
30009a1e:	6822      	ldr	r2, [r4, #0]
30009a20:	900b      	str	r0, [sp, #44]	; 0x2c
30009a22:	fa03 f30a 	lsl.w	r3, r3, sl
30009a26:	78d2      	ldrb	r2, [r2, #3]
30009a28:	9306      	str	r3, [sp, #24]
30009a2a:	6923      	ldr	r3, [r4, #16]
30009a2c:	fa03 f30a 	lsl.w	r3, r3, sl
30009a30:	b1ca      	cbz	r2, 30009a66 <rxi316_dram_init+0x5a>
30009a32:	2a01      	cmp	r2, #1
30009a34:	d01b      	beq.n	30009a6e <rxi316_dram_init+0x62>
30009a36:	2a02      	cmp	r2, #2
30009a38:	bf0b      	itete	eq
30009a3a:	2208      	moveq	r2, #8
30009a3c:	2210      	movne	r2, #16
30009a3e:	2501      	moveq	r5, #1
30009a40:	2502      	movne	r5, #2
30009a42:	9201      	str	r2, [sp, #4]
30009a44:	6862      	ldr	r2, [r4, #4]
30009a46:	6811      	ldr	r1, [r2, #0]
30009a48:	e9d2 8201 	ldrd	r8, r2, [r2, #4]
30009a4c:	910a      	str	r1, [sp, #40]	; 0x28
30009a4e:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30009a52:	9204      	str	r2, [sp, #16]
30009a54:	4aa2      	ldr	r2, [pc, #648]	; (30009ce0 <rxi316_dram_init+0x2d4>)
30009a56:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
30009a5a:	6820      	ldr	r0, [r4, #0]
30009a5c:	7981      	ldrb	r1, [r0, #6]
30009a5e:	6882      	ldr	r2, [r0, #8]
30009a60:	b949      	cbnz	r1, 30009a76 <rxi316_dram_init+0x6a>
30009a62:	3a06      	subs	r2, #6
30009a64:	e00b      	b.n	30009a7e <rxi316_dram_init+0x72>
30009a66:	2202      	movs	r2, #2
30009a68:	2503      	movs	r5, #3
30009a6a:	9201      	str	r2, [sp, #4]
30009a6c:	e7ea      	b.n	30009a44 <rxi316_dram_init+0x38>
30009a6e:	2204      	movs	r2, #4
30009a70:	2500      	movs	r5, #0
30009a72:	9201      	str	r2, [sp, #4]
30009a74:	e7e6      	b.n	30009a44 <rxi316_dram_init+0x38>
30009a76:	2901      	cmp	r1, #1
30009a78:	f040 80f0 	bne.w	30009c5c <rxi316_dram_init+0x250>
30009a7c:	3a07      	subs	r2, #7
30009a7e:	7881      	ldrb	r1, [r0, #2]
30009a80:	f002 020f 	and.w	r2, r2, #15
30009a84:	7940      	ldrb	r0, [r0, #5]
30009a86:	0109      	lsls	r1, r1, #4
30009a88:	0280      	lsls	r0, r0, #10
30009a8a:	f001 0130 	and.w	r1, r1, #48	; 0x30
30009a8e:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
30009a92:	4301      	orrs	r1, r0
30009a94:	ea41 1185 	orr.w	r1, r1, r5, lsl #6
30009a98:	4311      	orrs	r1, r2
30009a9a:	4a91      	ldr	r2, [pc, #580]	; (30009ce0 <rxi316_dram_init+0x2d4>)
30009a9c:	f8c2 1224 	str.w	r1, [r2, #548]	; 0x224
30009aa0:	68e1      	ldr	r1, [r4, #12]
30009aa2:	6809      	ldr	r1, [r1, #0]
30009aa4:	f891 106c 	ldrb.w	r1, [r1, #108]	; 0x6c
30009aa8:	f001 0107 	and.w	r1, r1, #7
30009aac:	f8c2 1228 	str.w	r1, [r2, #552]	; 0x228
30009ab0:	6821      	ldr	r1, [r4, #0]
30009ab2:	7809      	ldrb	r1, [r1, #0]
30009ab4:	9102      	str	r1, [sp, #8]
30009ab6:	68e1      	ldr	r1, [r4, #12]
30009ab8:	6809      	ldr	r1, [r1, #0]
30009aba:	6848      	ldr	r0, [r1, #4]
30009abc:	9003      	str	r0, [sp, #12]
30009abe:	9d03      	ldr	r5, [sp, #12]
30009ac0:	6888      	ldr	r0, [r1, #8]
30009ac2:	032d      	lsls	r5, r5, #12
30009ac4:	0400      	lsls	r0, r0, #16
30009ac6:	f405 5580 	and.w	r5, r5, #4096	; 0x1000
30009aca:	f400 3080 	and.w	r0, r0, #65536	; 0x10000
30009ace:	4305      	orrs	r5, r0
30009ad0:	9802      	ldr	r0, [sp, #8]
30009ad2:	f000 000f 	and.w	r0, r0, #15
30009ad6:	4305      	orrs	r5, r0
30009ad8:	68c8      	ldr	r0, [r1, #12]
30009ada:	0440      	lsls	r0, r0, #17
30009adc:	f400 3000 	and.w	r0, r0, #131072	; 0x20000
30009ae0:	4305      	orrs	r5, r0
30009ae2:	6908      	ldr	r0, [r1, #16]
30009ae4:	0480      	lsls	r0, r0, #18
30009ae6:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
30009aea:	4305      	orrs	r5, r0
30009aec:	6948      	ldr	r0, [r1, #20]
30009aee:	04c0      	lsls	r0, r0, #19
30009af0:	f400 2000 	and.w	r0, r0, #524288	; 0x80000
30009af4:	4305      	orrs	r5, r0
30009af6:	6988      	ldr	r0, [r1, #24]
30009af8:	0540      	lsls	r0, r0, #21
30009afa:	f400 1000 	and.w	r0, r0, #2097152	; 0x200000
30009afe:	4305      	orrs	r5, r0
30009b00:	69c8      	ldr	r0, [r1, #28]
30009b02:	0580      	lsls	r0, r0, #22
30009b04:	f400 0080 	and.w	r0, r0, #4194304	; 0x400000
30009b08:	4305      	orrs	r5, r0
30009b0a:	6a88      	ldr	r0, [r1, #40]	; 0x28
30009b0c:	0600      	lsls	r0, r0, #24
30009b0e:	f000 7040 	and.w	r0, r0, #50331648	; 0x3000000
30009b12:	4305      	orrs	r5, r0
30009b14:	6a48      	ldr	r0, [r1, #36]	; 0x24
30009b16:	06c0      	lsls	r0, r0, #27
30009b18:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
30009b1c:	4328      	orrs	r0, r5
30009b1e:	6a0d      	ldr	r5, [r1, #32]
30009b20:	9906      	ldr	r1, [sp, #24]
30009b22:	07ad      	lsls	r5, r5, #30
30009b24:	f005 4580 	and.w	r5, r5, #1073741824	; 0x40000000
30009b28:	4328      	orrs	r0, r5
30009b2a:	020d      	lsls	r5, r1, #8
30009b2c:	f405 65e0 	and.w	r5, r5, #1792	; 0x700
30009b30:	4328      	orrs	r0, r5
30009b32:	6050      	str	r0, [r2, #4]
30009b34:	6821      	ldr	r1, [r4, #0]
30009b36:	7989      	ldrb	r1, [r1, #6]
30009b38:	2900      	cmp	r1, #0
30009b3a:	f040 8094 	bne.w	30009c66 <rxi316_dram_init+0x25a>
30009b3e:	6851      	ldr	r1, [r2, #4]
30009b40:	f041 0110 	orr.w	r1, r1, #16
30009b44:	6051      	str	r1, [r2, #4]
30009b46:	68e2      	ldr	r2, [r4, #12]
30009b48:	6865      	ldr	r5, [r4, #4]
30009b4a:	6811      	ldr	r1, [r2, #0]
30009b4c:	6928      	ldr	r0, [r5, #16]
30009b4e:	e9d1 e20d 	ldrd	lr, r2, [r1, #52]	; 0x34
30009b52:	e9d1 9c0b 	ldrd	r9, ip, [r1, #44]	; 0x2c
30009b56:	9205      	str	r2, [sp, #20]
30009b58:	ea4f 0e8e 	mov.w	lr, lr, lsl #2
30009b5c:	6c4a      	ldr	r2, [r1, #68]	; 0x44
30009b5e:	f009 0901 	and.w	r9, r9, #1
30009b62:	f00e 0e04 	and.w	lr, lr, #4
30009b66:	9207      	str	r2, [sp, #28]
30009b68:	6c8a      	ldr	r2, [r1, #72]	; 0x48
30009b6a:	9208      	str	r2, [sp, #32]
30009b6c:	9a06      	ldr	r2, [sp, #24]
30009b6e:	f102 3bff 	add.w	fp, r2, #4294967295	; 0xffffffff
30009b72:	fa20 f20a 	lsr.w	r2, r0, sl
30009b76:	ea10 0f0b 	tst.w	r0, fp
30009b7a:	6968      	ldr	r0, [r5, #20]
30009b7c:	bf18      	it	ne
30009b7e:	3201      	addne	r2, #1
30009b80:	fa20 f50a 	lsr.w	r5, r0, sl
30009b84:	ea10 0f0b 	tst.w	r0, fp
30009b88:	ea4f 004c 	mov.w	r0, ip, lsl #1
30009b8c:	ea4f 3202 	mov.w	r2, r2, lsl #12
30009b90:	f000 0002 	and.w	r0, r0, #2
30009b94:	bf18      	it	ne
30009b96:	3501      	addne	r5, #1
30009b98:	f402 32f8 	and.w	r2, r2, #126976	; 0x1f000
30009b9c:	ea40 0e0e 	orr.w	lr, r0, lr
30009ba0:	9805      	ldr	r0, [sp, #20]
30009ba2:	052d      	lsls	r5, r5, #20
30009ba4:	ea4f 0cc0 	mov.w	ip, r0, lsl #3
30009ba8:	ea4e 0e09 	orr.w	lr, lr, r9
30009bac:	9807      	ldr	r0, [sp, #28]
30009bae:	f005 75f8 	and.w	r5, r5, #32505856	; 0x1f00000
30009bb2:	f00c 0c08 	and.w	ip, ip, #8
30009bb6:	0180      	lsls	r0, r0, #6
30009bb8:	ea4e 0e0c 	orr.w	lr, lr, ip
30009bbc:	f000 0040 	and.w	r0, r0, #64	; 0x40
30009bc0:	e9d1 760f 	ldrd	r7, r6, [r1, #60]	; 0x3c
30009bc4:	013f      	lsls	r7, r7, #4
30009bc6:	0176      	lsls	r6, r6, #5
30009bc8:	f007 0710 	and.w	r7, r7, #16
30009bcc:	f006 0620 	and.w	r6, r6, #32
30009bd0:	ea4e 0e07 	orr.w	lr, lr, r7
30009bd4:	ea4e 0e06 	orr.w	lr, lr, r6
30009bd8:	ea4e 0e00 	orr.w	lr, lr, r0
30009bdc:	9808      	ldr	r0, [sp, #32]
30009bde:	0206      	lsls	r6, r0, #8
30009be0:	6d08      	ldr	r0, [r1, #80]	; 0x50
30009be2:	f406 6670 	and.w	r6, r6, #3840	; 0xf00
30009be6:	0700      	lsls	r0, r0, #28
30009be8:	ea4e 0e06 	orr.w	lr, lr, r6
30009bec:	6cce      	ldr	r6, [r1, #76]	; 0x4c
30009bee:	f000 5080 	and.w	r0, r0, #268435456	; 0x10000000
30009bf2:	06f6      	lsls	r6, r6, #27
30009bf4:	f006 6600 	and.w	r6, r6, #134217728	; 0x8000000
30009bf8:	ea4e 0606 	orr.w	r6, lr, r6
30009bfc:	4330      	orrs	r0, r6
30009bfe:	6d4e      	ldr	r6, [r1, #84]	; 0x54
30009c00:	0776      	lsls	r6, r6, #29
30009c02:	f006 5600 	and.w	r6, r6, #536870912	; 0x20000000
30009c06:	4330      	orrs	r0, r6
30009c08:	4310      	orrs	r0, r2
30009c0a:	4a35      	ldr	r2, [pc, #212]	; (30009ce0 <rxi316_dram_init+0x2d4>)
30009c0c:	4328      	orrs	r0, r5
30009c0e:	6090      	str	r0, [r2, #8]
30009c10:	6862      	ldr	r2, [r4, #4]
30009c12:	6990      	ldr	r0, [r2, #24]
30009c14:	9a01      	ldr	r2, [sp, #4]
30009c16:	2a10      	cmp	r2, #16
30009c18:	6822      	ldr	r2, [r4, #0]
30009c1a:	d137      	bne.n	30009c8c <rxi316_dram_init+0x280>
30009c1c:	7812      	ldrb	r2, [r2, #0]
30009c1e:	2a09      	cmp	r2, #9
30009c20:	bf14      	ite	ne
30009c22:	2100      	movne	r1, #0
30009c24:	2104      	moveq	r1, #4
30009c26:	6822      	ldr	r2, [r4, #0]
30009c28:	f892 c000 	ldrb.w	ip, [r2]
30009c2c:	7917      	ldrb	r7, [r2, #4]
30009c2e:	f1bc 0f02 	cmp.w	ip, #2
30009c32:	f000 80cd 	beq.w	30009dd0 <rxi316_dram_init+0x3c4>
30009c36:	f1bc 0f09 	cmp.w	ip, #9
30009c3a:	f000 80c9 	beq.w	30009dd0 <rxi316_dram_init+0x3c4>
30009c3e:	f1bc 0f03 	cmp.w	ip, #3
30009c42:	f040 80c7 	bne.w	30009dd4 <rxi316_dram_init+0x3c8>
30009c46:	f1b8 0f04 	cmp.w	r8, #4
30009c4a:	f240 80c6 	bls.w	30009dda <rxi316_dram_init+0x3ce>
30009c4e:	f1b8 0f0b 	cmp.w	r8, #11
30009c52:	d847      	bhi.n	30009ce4 <rxi316_dram_init+0x2d8>
30009c54:	f1a8 0604 	sub.w	r6, r8, #4
30009c58:	2500      	movs	r5, #0
30009c5a:	e046      	b.n	30009cea <rxi316_dram_init+0x2de>
30009c5c:	2903      	cmp	r1, #3
30009c5e:	f47f af00 	bne.w	30009a62 <rxi316_dram_init+0x56>
30009c62:	3a08      	subs	r2, #8
30009c64:	e70b      	b.n	30009a7e <rxi316_dram_init+0x72>
30009c66:	2901      	cmp	r1, #1
30009c68:	d103      	bne.n	30009c72 <rxi316_dram_init+0x266>
30009c6a:	6851      	ldr	r1, [r2, #4]
30009c6c:	f041 0120 	orr.w	r1, r1, #32
30009c70:	e768      	b.n	30009b44 <rxi316_dram_init+0x138>
30009c72:	2902      	cmp	r1, #2
30009c74:	d103      	bne.n	30009c7e <rxi316_dram_init+0x272>
30009c76:	6851      	ldr	r1, [r2, #4]
30009c78:	f041 0140 	orr.w	r1, r1, #64	; 0x40
30009c7c:	e762      	b.n	30009b44 <rxi316_dram_init+0x138>
30009c7e:	2903      	cmp	r1, #3
30009c80:	f47f af61 	bne.w	30009b46 <rxi316_dram_init+0x13a>
30009c84:	6851      	ldr	r1, [r2, #4]
30009c86:	f041 0180 	orr.w	r1, r1, #128	; 0x80
30009c8a:	e75b      	b.n	30009b44 <rxi316_dram_init+0x138>
30009c8c:	7811      	ldrb	r1, [r2, #0]
30009c8e:	9a01      	ldr	r2, [sp, #4]
30009c90:	2a08      	cmp	r2, #8
30009c92:	d106      	bne.n	30009ca2 <rxi316_dram_init+0x296>
30009c94:	2902      	cmp	r1, #2
30009c96:	d019      	beq.n	30009ccc <rxi316_dram_init+0x2c0>
30009c98:	2909      	cmp	r1, #9
30009c9a:	bf14      	ite	ne
30009c9c:	2100      	movne	r1, #0
30009c9e:	2103      	moveq	r1, #3
30009ca0:	e7c1      	b.n	30009c26 <rxi316_dram_init+0x21a>
30009ca2:	9a01      	ldr	r2, [sp, #4]
30009ca4:	2a04      	cmp	r2, #4
30009ca6:	d107      	bne.n	30009cb8 <rxi316_dram_init+0x2ac>
30009ca8:	1e8a      	subs	r2, r1, #2
30009caa:	2a01      	cmp	r2, #1
30009cac:	d910      	bls.n	30009cd0 <rxi316_dram_init+0x2c4>
30009cae:	2909      	cmp	r1, #9
30009cb0:	bf14      	ite	ne
30009cb2:	2100      	movne	r1, #0
30009cb4:	2102      	moveq	r1, #2
30009cb6:	e7b6      	b.n	30009c26 <rxi316_dram_init+0x21a>
30009cb8:	9a01      	ldr	r2, [sp, #4]
30009cba:	2a02      	cmp	r2, #2
30009cbc:	d10a      	bne.n	30009cd4 <rxi316_dram_init+0x2c8>
30009cbe:	2902      	cmp	r1, #2
30009cc0:	d00a      	beq.n	30009cd8 <rxi316_dram_init+0x2cc>
30009cc2:	f1a1 0709 	sub.w	r7, r1, #9
30009cc6:	4279      	negs	r1, r7
30009cc8:	4179      	adcs	r1, r7
30009cca:	e7ac      	b.n	30009c26 <rxi316_dram_init+0x21a>
30009ccc:	2103      	movs	r1, #3
30009cce:	e7aa      	b.n	30009c26 <rxi316_dram_init+0x21a>
30009cd0:	2102      	movs	r1, #2
30009cd2:	e7a8      	b.n	30009c26 <rxi316_dram_init+0x21a>
30009cd4:	2100      	movs	r1, #0
30009cd6:	e7a6      	b.n	30009c26 <rxi316_dram_init+0x21a>
30009cd8:	2101      	movs	r1, #1
30009cda:	e7a4      	b.n	30009c26 <rxi316_dram_init+0x21a>
30009cdc:	30007405 	.word	0x30007405
30009ce0:	41100000 	.word	0x41100000
30009ce4:	f1a8 060c 	sub.w	r6, r8, #12
30009ce8:	2501      	movs	r5, #1
30009cea:	68a2      	ldr	r2, [r4, #8]
30009cec:	f1bc 0f02 	cmp.w	ip, #2
30009cf0:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
30009cf2:	d175      	bne.n	30009de0 <rxi316_dram_init+0x3d4>
30009cf4:	f1a2 0c02 	sub.w	ip, r2, #2
30009cf8:	f1bc 0f06 	cmp.w	ip, #6
30009cfc:	f200 8085 	bhi.w	30009e0a <rxi316_dram_init+0x3fe>
30009d00:	3a01      	subs	r2, #1
30009d02:	f04f 0c00 	mov.w	ip, #0
30009d06:	ea40 00c7 	orr.w	r0, r0, r7, lsl #3
30009d0a:	4308      	orrs	r0, r1
30009d0c:	4945      	ldr	r1, [pc, #276]	; (30009e24 <rxi316_dram_init+0x418>)
30009d0e:	ea40 0085 	orr.w	r0, r0, r5, lsl #2
30009d12:	ea40 1006 	orr.w	r0, r0, r6, lsl #4
30009d16:	ea40 2042 	orr.w	r0, r0, r2, lsl #9
30009d1a:	ea40 300c 	orr.w	r0, r0, ip, lsl #12
30009d1e:	6348      	str	r0, [r1, #52]	; 0x34
30009d20:	6862      	ldr	r2, [r4, #4]
30009d22:	9804      	ldr	r0, [sp, #16]
30009d24:	69d2      	ldr	r2, [r2, #28]
30009d26:	ea42 02c0 	orr.w	r2, r2, r0, lsl #3
30009d2a:	638a      	str	r2, [r1, #56]	; 0x38
30009d2c:	6862      	ldr	r2, [r4, #4]
30009d2e:	6a11      	ldr	r1, [r2, #32]
30009d30:	6822      	ldr	r2, [r4, #0]
30009d32:	7812      	ldrb	r2, [r2, #0]
30009d34:	2a03      	cmp	r2, #3
30009d36:	d16e      	bne.n	30009e16 <rxi316_dram_init+0x40a>
30009d38:	9a0a      	ldr	r2, [sp, #40]	; 0x28
30009d3a:	1f50      	subs	r0, r2, #5
30009d3c:	68e2      	ldr	r2, [r4, #12]
30009d3e:	6812      	ldr	r2, [r2, #0]
30009d40:	6b92      	ldr	r2, [r2, #56]	; 0x38
30009d42:	fab2 f282 	clz	r2, r2
30009d46:	0952      	lsrs	r2, r2, #5
30009d48:	0252      	lsls	r2, r2, #9
30009d4a:	ea42 00c0 	orr.w	r0, r2, r0, lsl #3
30009d4e:	4301      	orrs	r1, r0
30009d50:	4834      	ldr	r0, [pc, #208]	; (30009e24 <rxi316_dram_init+0x418>)
30009d52:	63c1      	str	r1, [r0, #60]	; 0x3c
30009d54:	6862      	ldr	r2, [r4, #4]
30009d56:	6a52      	ldr	r2, [r2, #36]	; 0x24
30009d58:	6402      	str	r2, [r0, #64]	; 0x40
30009d5a:	6860      	ldr	r0, [r4, #4]
30009d5c:	68c5      	ldr	r5, [r0, #12]
30009d5e:	6801      	ldr	r1, [r0, #0]
30009d60:	6842      	ldr	r2, [r0, #4]
30009d62:	4429      	add	r1, r5
30009d64:	6886      	ldr	r6, [r0, #8]
30009d66:	442a      	add	r2, r5
30009d68:	ea1b 0f01 	tst.w	fp, r1
30009d6c:	fa21 f10a 	lsr.w	r1, r1, sl
30009d70:	fa22 f50a 	lsr.w	r5, r2, sl
30009d74:	fa26 f00a 	lsr.w	r0, r6, sl
30009d78:	bf18      	it	ne
30009d7a:	3101      	addne	r1, #1
30009d7c:	ea1b 0f02 	tst.w	fp, r2
30009d80:	6822      	ldr	r2, [r4, #0]
30009d82:	bf18      	it	ne
30009d84:	3501      	addne	r5, #1
30009d86:	ea1b 0f06 	tst.w	fp, r6
30009d8a:	f892 c000 	ldrb.w	ip, [r2]
30009d8e:	bf18      	it	ne
30009d90:	3001      	addne	r0, #1
30009d92:	68a6      	ldr	r6, [r4, #8]
30009d94:	f1ac 0202 	sub.w	r2, ip, #2
30009d98:	2a01      	cmp	r2, #1
30009d9a:	d845      	bhi.n	30009e28 <rxi316_dram_init+0x41c>
30009d9c:	6df7      	ldr	r7, [r6, #92]	; 0x5c
30009d9e:	ea1b 0f07 	tst.w	fp, r7
30009da2:	fa27 f20a 	lsr.w	r2, r7, sl
30009da6:	d000      	beq.n	30009daa <rxi316_dram_init+0x39e>
30009da8:	3201      	adds	r2, #1
30009daa:	f1bc 0f02 	cmp.w	ip, #2
30009dae:	6a77      	ldr	r7, [r6, #36]	; 0x24
30009db0:	f040 8285 	bne.w	3000a2be <rxi316_dram_init+0x8b2>
30009db4:	9e01      	ldr	r6, [sp, #4]
30009db6:	0876      	lsrs	r6, r6, #1
30009db8:	3e02      	subs	r6, #2
30009dba:	fa26 f60a 	lsr.w	r6, r6, sl
30009dbe:	4437      	add	r7, r6
30009dc0:	fbb7 f6f3 	udiv	r6, r7, r3
30009dc4:	fb03 7716 	mls	r7, r3, r6, r7
30009dc8:	2f00      	cmp	r7, #0
30009dca:	d03c      	beq.n	30009e46 <rxi316_dram_init+0x43a>
30009dcc:	3601      	adds	r6, #1
30009dce:	e03a      	b.n	30009e46 <rxi316_dram_init+0x43a>
30009dd0:	4646      	mov	r6, r8
30009dd2:	e741      	b.n	30009c58 <rxi316_dram_init+0x24c>
30009dd4:	2500      	movs	r5, #0
30009dd6:	462e      	mov	r6, r5
30009dd8:	e787      	b.n	30009cea <rxi316_dram_init+0x2de>
30009dda:	2500      	movs	r5, #0
30009ddc:	2601      	movs	r6, #1
30009dde:	e784      	b.n	30009cea <rxi316_dram_init+0x2de>
30009de0:	f1bc 0f03 	cmp.w	ip, #3
30009de4:	d115      	bne.n	30009e12 <rxi316_dram_init+0x406>
30009de6:	2a04      	cmp	r2, #4
30009de8:	d911      	bls.n	30009e0e <rxi316_dram_init+0x402>
30009dea:	f1a2 0c05 	sub.w	ip, r2, #5
30009dee:	f1bc 0f03 	cmp.w	ip, #3
30009df2:	d803      	bhi.n	30009dfc <rxi316_dram_init+0x3f0>
30009df4:	3a04      	subs	r2, #4
30009df6:	f04f 0c01 	mov.w	ip, #1
30009dfa:	e784      	b.n	30009d06 <rxi316_dram_init+0x2fa>
30009dfc:	f1a2 0c09 	sub.w	ip, r2, #9
30009e00:	f1bc 0f05 	cmp.w	ip, #5
30009e04:	d805      	bhi.n	30009e12 <rxi316_dram_init+0x406>
30009e06:	0852      	lsrs	r2, r2, #1
30009e08:	e7f5      	b.n	30009df6 <rxi316_dram_init+0x3ea>
30009e0a:	2200      	movs	r2, #0
30009e0c:	e779      	b.n	30009d02 <rxi316_dram_init+0x2f6>
30009e0e:	2201      	movs	r2, #1
30009e10:	e7f1      	b.n	30009df6 <rxi316_dram_init+0x3ea>
30009e12:	2200      	movs	r2, #0
30009e14:	e7ef      	b.n	30009df6 <rxi316_dram_init+0x3ea>
30009e16:	2a09      	cmp	r2, #9
30009e18:	f04f 0000 	mov.w	r0, #0
30009e1c:	d08e      	beq.n	30009d3c <rxi316_dram_init+0x330>
30009e1e:	4602      	mov	r2, r0
30009e20:	e792      	b.n	30009d48 <rxi316_dram_init+0x33c>
30009e22:	bf00      	nop
30009e24:	41100000 	.word	0x41100000
30009e28:	69b7      	ldr	r7, [r6, #24]
30009e2a:	fbb7 f2f3 	udiv	r2, r7, r3
30009e2e:	fb03 7712 	mls	r7, r3, r2, r7
30009e32:	b107      	cbz	r7, 30009e36 <rxi316_dram_init+0x42a>
30009e34:	3201      	adds	r2, #1
30009e36:	f1bc 0f09 	cmp.w	ip, #9
30009e3a:	f040 8240 	bne.w	3000a2be <rxi316_dram_init+0x8b2>
30009e3e:	9e01      	ldr	r6, [sp, #4]
30009e40:	0876      	lsrs	r6, r6, #1
30009e42:	fa26 f60a 	lsr.w	r6, r6, sl
30009e46:	016d      	lsls	r5, r5, #5
30009e48:	f001 011f 	and.w	r1, r1, #31
30009e4c:	0280      	lsls	r0, r0, #10
30009e4e:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
30009e52:	0512      	lsls	r2, r2, #20
30009e54:	f400 40f8 	and.w	r0, r0, #31744	; 0x7c00
30009e58:	0676      	lsls	r6, r6, #25
30009e5a:	4329      	orrs	r1, r5
30009e5c:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
30009e60:	f006 5678 	and.w	r6, r6, #1040187392	; 0x3e000000
30009e64:	4301      	orrs	r1, r0
30009e66:	4311      	orrs	r1, r2
30009e68:	4ab3      	ldr	r2, [pc, #716]	; (3000a138 <rxi316_dram_init+0x72c>)
30009e6a:	4331      	orrs	r1, r6
30009e6c:	6311      	str	r1, [r2, #48]	; 0x30
30009e6e:	68a1      	ldr	r1, [r4, #8]
30009e70:	680a      	ldr	r2, [r1, #0]
30009e72:	fbb2 f9f3 	udiv	r9, r2, r3
30009e76:	fb03 2219 	mls	r2, r3, r9, r2
30009e7a:	b10a      	cbz	r2, 30009e80 <rxi316_dram_init+0x474>
30009e7c:	f109 0901 	add.w	r9, r9, #1
30009e80:	684a      	ldr	r2, [r1, #4]
30009e82:	fbb2 f0f3 	udiv	r0, r2, r3
30009e86:	fb03 2210 	mls	r2, r3, r0, r2
30009e8a:	2a00      	cmp	r2, #0
30009e8c:	f000 8219 	beq.w	3000a2c2 <rxi316_dram_init+0x8b6>
30009e90:	38c7      	subs	r0, #199	; 0xc7
30009e92:	68e2      	ldr	r2, [r4, #12]
30009e94:	6811      	ldr	r1, [r2, #0]
30009e96:	4aa9      	ldr	r2, [pc, #676]	; (3000a13c <rxi316_dram_init+0x730>)
30009e98:	ea02 2200 	and.w	r2, r2, r0, lsl #8
30009e9c:	9209      	str	r2, [sp, #36]	; 0x24
30009e9e:	6dca      	ldr	r2, [r1, #92]	; 0x5c
30009ea0:	0712      	lsls	r2, r2, #28
30009ea2:	f002 5080 	and.w	r0, r2, #268435456	; 0x10000000
30009ea6:	6e0a      	ldr	r2, [r1, #96]	; 0x60
30009ea8:	9909      	ldr	r1, [sp, #36]	; 0x24
30009eaa:	ea41 7242 	orr.w	r2, r1, r2, lsl #29
30009eae:	fa5f f189 	uxtb.w	r1, r9
30009eb2:	4302      	orrs	r2, r0
30009eb4:	430a      	orrs	r2, r1
30009eb6:	49a0      	ldr	r1, [pc, #640]	; (3000a138 <rxi316_dram_init+0x72c>)
30009eb8:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
30009ebc:	610a      	str	r2, [r1, #16]
30009ebe:	68a0      	ldr	r0, [r4, #8]
30009ec0:	6902      	ldr	r2, [r0, #16]
30009ec2:	fbb2 fef3 	udiv	lr, r2, r3
30009ec6:	fb03 221e 	mls	r2, r3, lr, r2
30009eca:	2a00      	cmp	r2, #0
30009ecc:	f000 81fb 	beq.w	3000a2c6 <rxi316_dram_init+0x8ba>
30009ed0:	f10e 0201 	add.w	r2, lr, #1
30009ed4:	9208      	str	r2, [sp, #32]
30009ed6:	6982      	ldr	r2, [r0, #24]
30009ed8:	fbb2 fcf3 	udiv	ip, r2, r3
30009edc:	fb03 221c 	mls	r2, r3, ip, r2
30009ee0:	b10a      	cbz	r2, 30009ee6 <rxi316_dram_init+0x4da>
30009ee2:	f10c 0c01 	add.w	ip, ip, #1
30009ee6:	68c2      	ldr	r2, [r0, #12]
30009ee8:	fbb2 f1f3 	udiv	r1, r2, r3
30009eec:	fb03 2211 	mls	r2, r3, r1, r2
30009ef0:	9104      	str	r1, [sp, #16]
30009ef2:	b112      	cbz	r2, 30009efa <rxi316_dram_init+0x4ee>
30009ef4:	460a      	mov	r2, r1
30009ef6:	3201      	adds	r2, #1
30009ef8:	9204      	str	r2, [sp, #16]
30009efa:	6a42      	ldr	r2, [r0, #36]	; 0x24
30009efc:	fbb2 f1f3 	udiv	r1, r2, r3
30009f00:	fb03 2211 	mls	r2, r3, r1, r2
30009f04:	b102      	cbz	r2, 30009f08 <rxi316_dram_init+0x4fc>
30009f06:	3101      	adds	r1, #1
30009f08:	68e2      	ldr	r2, [r4, #12]
30009f0a:	0349      	lsls	r1, r1, #13
30009f0c:	6d05      	ldr	r5, [r0, #80]	; 0x50
30009f0e:	6812      	ldr	r2, [r2, #0]
30009f10:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
30009f14:	fa25 f00a 	lsr.w	r0, r5, sl
30009f18:	ea15 0f0b 	tst.w	r5, fp
30009f1c:	6e52      	ldr	r2, [r2, #100]	; 0x64
30009f1e:	bf18      	it	ne
30009f20:	3001      	addne	r0, #1
30009f22:	05d2      	lsls	r2, r2, #23
30009f24:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
30009f28:	ea42 6000 	orr.w	r0, r2, r0, lsl #24
30009f2c:	9a08      	ldr	r2, [sp, #32]
30009f2e:	0112      	lsls	r2, r2, #4
30009f30:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
30009f34:	4310      	orrs	r0, r2
30009f36:	ea4f 224c 	mov.w	r2, ip, lsl #9
30009f3a:	f402 52f0 	and.w	r2, r2, #7680	; 0x1e00
30009f3e:	4310      	orrs	r0, r2
30009f40:	9a04      	ldr	r2, [sp, #16]
30009f42:	f002 020f 	and.w	r2, r2, #15
30009f46:	4310      	orrs	r0, r2
30009f48:	4a7b      	ldr	r2, [pc, #492]	; (3000a138 <rxi316_dram_init+0x72c>)
30009f4a:	4308      	orrs	r0, r1
30009f4c:	f440 10a0 	orr.w	r0, r0, #1310720	; 0x140000
30009f50:	6150      	str	r0, [r2, #20]
30009f52:	f8d4 e008 	ldr.w	lr, [r4, #8]
30009f56:	f8de 2014 	ldr.w	r2, [lr, #20]
30009f5a:	fbb2 f7f3 	udiv	r7, r2, r3
30009f5e:	fb03 2217 	mls	r2, r3, r7, r2
30009f62:	b102      	cbz	r2, 30009f66 <rxi316_dram_init+0x55a>
30009f64:	3701      	adds	r7, #1
30009f66:	f8de 2010 	ldr.w	r2, [lr, #16]
30009f6a:	fbb2 f1f3 	udiv	r1, r2, r3
30009f6e:	fb03 2211 	mls	r2, r3, r1, r2
30009f72:	b102      	cbz	r2, 30009f76 <rxi316_dram_init+0x56a>
30009f74:	3101      	adds	r1, #1
30009f76:	f8de 000c 	ldr.w	r0, [lr, #12]
30009f7a:	fbb0 f2f3 	udiv	r2, r0, r3
30009f7e:	fb03 0012 	mls	r0, r3, r2, r0
30009f82:	b100      	cbz	r0, 30009f86 <rxi316_dram_init+0x57a>
30009f84:	3201      	adds	r2, #1
30009f86:	f8de 5030 	ldr.w	r5, [lr, #48]	; 0x30
30009f8a:	fbb5 f0f3 	udiv	r0, r5, r3
30009f8e:	fb03 5510 	mls	r5, r3, r0, r5
30009f92:	b105      	cbz	r5, 30009f96 <rxi316_dram_init+0x58a>
30009f94:	3001      	adds	r0, #1
30009f96:	440a      	add	r2, r1
30009f98:	f8de 1008 	ldr.w	r1, [lr, #8]
30009f9c:	fbb1 f5f3 	udiv	r5, r1, r3
30009fa0:	4282      	cmp	r2, r0
30009fa2:	fb03 1115 	mls	r1, r3, r5, r1
30009fa6:	bf38      	it	cc
30009fa8:	4602      	movcc	r2, r0
30009faa:	b901      	cbnz	r1, 30009fae <rxi316_dram_init+0x5a2>
30009fac:	3d01      	subs	r5, #1
30009fae:	f8de 102c 	ldr.w	r1, [lr, #44]	; 0x2c
30009fb2:	f8de 001c 	ldr.w	r0, [lr, #28]
30009fb6:	ea11 0f0b 	tst.w	r1, fp
30009fba:	fa21 f60a 	lsr.w	r6, r1, sl
30009fbe:	fa20 f10a 	lsr.w	r1, r0, sl
30009fc2:	bf18      	it	ne
30009fc4:	3601      	addne	r6, #1
30009fc6:	ea10 0f0b 	tst.w	r0, fp
30009fca:	9801      	ldr	r0, [sp, #4]
30009fcc:	bf18      	it	ne
30009fce:	3101      	addne	r1, #1
30009fd0:	0840      	lsrs	r0, r0, #1
30009fd2:	9005      	str	r0, [sp, #20]
30009fd4:	3002      	adds	r0, #2
30009fd6:	9007      	str	r0, [sp, #28]
30009fd8:	9802      	ldr	r0, [sp, #8]
30009fda:	2809      	cmp	r0, #9
30009fdc:	f040 8176 	bne.w	3000a2cc <rxi316_dram_init+0x8c0>
30009fe0:	9805      	ldr	r0, [sp, #20]
30009fe2:	4480      	add	r8, r0
30009fe4:	fa28 f00a 	lsr.w	r0, r8, sl
30009fe8:	ea18 0f0b 	tst.w	r8, fp
30009fec:	f8de 8040 	ldr.w	r8, [lr, #64]	; 0x40
30009ff0:	bf18      	it	ne
30009ff2:	3001      	addne	r0, #1
30009ff4:	fbb8 fef3 	udiv	lr, r8, r3
30009ff8:	fb03 881e 	mls	r8, r3, lr, r8
30009ffc:	f1b8 0f00 	cmp.w	r8, #0
3000a000:	d001      	beq.n	3000a006 <rxi316_dram_init+0x5fa>
3000a002:	f10e 0e01 	add.w	lr, lr, #1
3000a006:	0112      	lsls	r2, r2, #4
3000a008:	f007 070f 	and.w	r7, r7, #15
3000a00c:	f402 727c 	and.w	r2, r2, #1008	; 0x3f0
3000a010:	4317      	orrs	r7, r2
3000a012:	02aa      	lsls	r2, r5, #10
3000a014:	f402 5270 	and.w	r2, r2, #15360	; 0x3c00
3000a018:	433a      	orrs	r2, r7
3000a01a:	03b7      	lsls	r7, r6, #14
3000a01c:	4e46      	ldr	r6, [pc, #280]	; (3000a138 <rxi316_dram_init+0x72c>)
3000a01e:	f407 37e0 	and.w	r7, r7, #114688	; 0x1c000
3000a022:	4317      	orrs	r7, r2
3000a024:	044a      	lsls	r2, r1, #17
3000a026:	f402 2260 	and.w	r2, r2, #917504	; 0xe0000
3000a02a:	4317      	orrs	r7, r2
3000a02c:	0502      	lsls	r2, r0, #20
3000a02e:	f402 0270 	and.w	r2, r2, #15728640	; 0xf00000
3000a032:	4317      	orrs	r7, r2
3000a034:	ea4f 620e 	mov.w	r2, lr, lsl #24
3000a038:	f002 52f8 	and.w	r2, r2, #520093696	; 0x1f000000
3000a03c:	433a      	orrs	r2, r7
3000a03e:	61b2      	str	r2, [r6, #24]
3000a040:	6822      	ldr	r2, [r4, #0]
3000a042:	7816      	ldrb	r6, [r2, #0]
3000a044:	68a2      	ldr	r2, [r4, #8]
3000a046:	2e02      	cmp	r6, #2
3000a048:	f000 8150 	beq.w	3000a2ec <rxi316_dram_init+0x8e0>
3000a04c:	2e09      	cmp	r6, #9
3000a04e:	6a17      	ldr	r7, [r2, #32]
3000a050:	f000 8155 	beq.w	3000a2fe <rxi316_dram_init+0x8f2>
3000a054:	68e2      	ldr	r2, [r4, #12]
3000a056:	2e03      	cmp	r6, #3
3000a058:	6812      	ldr	r2, [r2, #0]
3000a05a:	f8d2 8068 	ldr.w	r8, [r2, #104]	; 0x68
3000a05e:	d005      	beq.n	3000a06c <rxi316_dram_init+0x660>
3000a060:	ea0b 0207 	and.w	r2, fp, r7
3000a064:	fa27 f70a 	lsr.w	r7, r7, sl
3000a068:	b102      	cbz	r2, 3000a06c <rxi316_dram_init+0x660>
3000a06a:	3701      	adds	r7, #1
3000a06c:	9a02      	ldr	r2, [sp, #8]
3000a06e:	2a02      	cmp	r2, #2
3000a070:	f040 8148 	bne.w	3000a304 <rxi316_dram_init+0x8f8>
3000a074:	4e32      	ldr	r6, [pc, #200]	; (3000a140 <rxi316_dram_init+0x734>)
3000a076:	4a33      	ldr	r2, [pc, #204]	; (3000a144 <rxi316_dram_init+0x738>)
3000a078:	fbb6 f6f3 	udiv	r6, r6, r3
3000a07c:	fbb2 f2f3 	udiv	r2, r2, r3
3000a080:	0ab6      	lsrs	r6, r6, #10
3000a082:	09d2      	lsrs	r2, r2, #7
3000a084:	3601      	adds	r6, #1
3000a086:	3201      	adds	r2, #1
3000a088:	f04f 0e00 	mov.w	lr, #0
3000a08c:	013f      	lsls	r7, r7, #4
3000a08e:	f008 080f 	and.w	r8, r8, #15
3000a092:	0212      	lsls	r2, r2, #8
3000a094:	b2ff      	uxtb	r7, r7
3000a096:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000a09a:	ea47 0808 	orr.w	r8, r7, r8
3000a09e:	ea48 588e 	orr.w	r8, r8, lr, lsl #22
3000a0a2:	ea48 0802 	orr.w	r8, r8, r2
3000a0a6:	4a28      	ldr	r2, [pc, #160]	; (3000a148 <rxi316_dram_init+0x73c>)
3000a0a8:	ea02 3206 	and.w	r2, r2, r6, lsl #12
3000a0ac:	4e22      	ldr	r6, [pc, #136]	; (3000a138 <rxi316_dram_init+0x72c>)
3000a0ae:	ea48 0202 	orr.w	r2, r8, r2
3000a0b2:	61f2      	str	r2, [r6, #28]
3000a0b4:	f8d4 8008 	ldr.w	r8, [r4, #8]
3000a0b8:	9a08      	ldr	r2, [sp, #32]
3000a0ba:	f8d8 4040 	ldr.w	r4, [r8, #64]	; 0x40
3000a0be:	ea4f 1e52 	mov.w	lr, r2, lsr #5
3000a0c2:	fbb4 f2f3 	udiv	r2, r4, r3
3000a0c6:	fb03 4312 	mls	r3, r3, r2, r4
3000a0ca:	b103      	cbz	r3, 3000a0ce <rxi316_dram_init+0x6c2>
3000a0cc:	3201      	adds	r2, #1
3000a0ce:	08cc      	lsrs	r4, r1, #3
3000a0d0:	f8d8 1034 	ldr.w	r1, [r8, #52]	; 0x34
3000a0d4:	092b      	lsrs	r3, r5, #4
3000a0d6:	f8d8 503c 	ldr.w	r5, [r8, #60]	; 0x3c
3000a0da:	ea11 0f0b 	tst.w	r1, fp
3000a0de:	fa21 f70a 	lsr.w	r7, r1, sl
3000a0e2:	f8d8 1038 	ldr.w	r1, [r8, #56]	; 0x38
3000a0e6:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000a0ea:	bf18      	it	ne
3000a0ec:	3701      	addne	r7, #1
3000a0ee:	f8d8 802c 	ldr.w	r8, [r8, #44]	; 0x2c
3000a0f2:	ea11 0f0b 	tst.w	r1, fp
3000a0f6:	fa21 f60a 	lsr.w	r6, r1, sl
3000a0fa:	fa25 f10a 	lsr.w	r1, r5, sl
3000a0fe:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000a102:	bf18      	it	ne
3000a104:	3601      	addne	r6, #1
3000a106:	ea15 0f0b 	tst.w	r5, fp
3000a10a:	9d01      	ldr	r5, [sp, #4]
3000a10c:	ea4f 2919 	mov.w	r9, r9, lsr #8
3000a110:	bf18      	it	ne
3000a112:	3101      	addne	r1, #1
3000a114:	ea4f 1c1c 	mov.w	ip, ip, lsr #4
3000a118:	fa25 f50a 	lsr.w	r5, r5, sl
3000a11c:	9302      	str	r3, [sp, #8]
3000a11e:	4545      	cmp	r5, r8
3000a120:	f240 8115 	bls.w	3000a34e <rxi316_dram_init+0x942>
3000a124:	eba5 0508 	sub.w	r5, r5, r8
3000a128:	fa25 f10a 	lsr.w	r1, r5, sl
3000a12c:	ea15 050b 	ands.w	r5, r5, fp
3000a130:	d00c      	beq.n	3000a14c <rxi316_dram_init+0x740>
3000a132:	3101      	adds	r1, #1
3000a134:	2500      	movs	r5, #0
3000a136:	e009      	b.n	3000a14c <rxi316_dram_init+0x740>
3000a138:	41100000 	.word	0x41100000
3000a13c:	00ffff00 	.word	0x00ffff00
3000a140:	0bebc200 	.word	0x0bebc200
3000a144:	00061a80 	.word	0x00061a80
3000a148:	003ff000 	.word	0x003ff000
3000a14c:	9b03      	ldr	r3, [sp, #12]
3000a14e:	ea4f 0e8e 	mov.w	lr, lr, lsl #2
3000a152:	ea4f 2989 	mov.w	r9, r9, lsl #10
3000a156:	2b00      	cmp	r3, #0
3000a158:	f00e 0e04 	and.w	lr, lr, #4
3000a15c:	f409 6980 	and.w	r9, r9, #1024	; 0x400
3000a160:	ea4f 2ccc 	mov.w	ip, ip, lsl #11
3000a164:	bf18      	it	ne
3000a166:	462b      	movne	r3, r5
3000a168:	0224      	lsls	r4, r4, #8
3000a16a:	ea4e 0e09 	orr.w	lr, lr, r9
3000a16e:	f40c 6c00 	and.w	ip, ip, #2048	; 0x800
3000a172:	9303      	str	r3, [sp, #12]
3000a174:	0240      	lsls	r0, r0, #9
3000a176:	9b02      	ldr	r3, [sp, #8]
3000a178:	ea4e 0e0c 	orr.w	lr, lr, ip
3000a17c:	f400 7000 	and.w	r0, r0, #512	; 0x200
3000a180:	00d2      	lsls	r2, r2, #3
3000a182:	01db      	lsls	r3, r3, #7
3000a184:	f002 0208 	and.w	r2, r2, #8
3000a188:	03bf      	lsls	r7, r7, #14
3000a18a:	b2db      	uxtb	r3, r3
3000a18c:	f407 37e0 	and.w	r7, r7, #114688	; 0x1c000
3000a190:	0476      	lsls	r6, r6, #17
3000a192:	ea4e 0e03 	orr.w	lr, lr, r3
3000a196:	f404 7380 	and.w	r3, r4, #256	; 0x100
3000a19a:	f406 2660 	and.w	r6, r6, #917504	; 0xe0000
3000a19e:	0509      	lsls	r1, r1, #20
3000a1a0:	ea4e 0e03 	orr.w	lr, lr, r3
3000a1a4:	9b03      	ldr	r3, [sp, #12]
3000a1a6:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a1aa:	ea4e 0e00 	orr.w	lr, lr, r0
3000a1ae:	ea4e 0202 	orr.w	r2, lr, r2
3000a1b2:	4317      	orrs	r7, r2
3000a1b4:	433e      	orrs	r6, r7
3000a1b6:	4331      	orrs	r1, r6
3000a1b8:	ea41 7103 	orr.w	r1, r1, r3, lsl #28
3000a1bc:	4b68      	ldr	r3, [pc, #416]	; (3000a360 <rxi316_dram_init+0x954>)
3000a1be:	6219      	str	r1, [r3, #32]
3000a1c0:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000a1c4:	9a04      	ldr	r2, [sp, #16]
3000a1c6:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000a1ca:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000a1ce:	9a06      	ldr	r2, [sp, #24]
3000a1d0:	2a01      	cmp	r2, #1
3000a1d2:	bf1e      	ittt	ne
3000a1d4:	9a05      	ldrne	r2, [sp, #20]
3000a1d6:	3203      	addne	r2, #3
3000a1d8:	9207      	strne	r2, [sp, #28]
3000a1da:	9a07      	ldr	r2, [sp, #28]
3000a1dc:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000a1e0:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000a1e4:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000a1e8:	60da      	str	r2, [r3, #12]
3000a1ea:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a1ee:	430a      	orrs	r2, r1
3000a1f0:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a1f4:	4a5b      	ldr	r2, [pc, #364]	; (3000a364 <rxi316_dram_init+0x958>)
3000a1f6:	f8d2 1268 	ldr.w	r1, [r2, #616]	; 0x268
3000a1fa:	008e      	lsls	r6, r1, #2
3000a1fc:	f140 80a9 	bpl.w	3000a352 <rxi316_dram_init+0x946>
3000a200:	6919      	ldr	r1, [r3, #16]
3000a202:	4c57      	ldr	r4, [pc, #348]	; (3000a360 <rxi316_dram_init+0x954>)
3000a204:	f041 5180 	orr.w	r1, r1, #268435456	; 0x10000000
3000a208:	6119      	str	r1, [r3, #16]
3000a20a:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000a20e:	6019      	str	r1, [r3, #0]
3000a210:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000a214:	f043 0303 	orr.w	r3, r3, #3
3000a218:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
3000a21c:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a21e:	2b01      	cmp	r3, #1
3000a220:	4b51      	ldr	r3, [pc, #324]	; (3000a368 <rxi316_dram_init+0x95c>)
3000a222:	bf0c      	ite	eq
3000a224:	4a51      	ldreq	r2, [pc, #324]	; (3000a36c <rxi316_dram_init+0x960>)
3000a226:	4a52      	ldrne	r2, [pc, #328]	; (3000a370 <rxi316_dram_init+0x964>)
3000a228:	601a      	str	r2, [r3, #0]
3000a22a:	68e3      	ldr	r3, [r4, #12]
3000a22c:	f423 23e0 	bic.w	r3, r3, #458752	; 0x70000
3000a230:	f023 0303 	bic.w	r3, r3, #3
3000a234:	60e3      	str	r3, [r4, #12]
3000a236:	f44f 0360 	mov.w	r3, #14680064	; 0xe00000
3000a23a:	62a3      	str	r3, [r4, #40]	; 0x28
3000a23c:	2303      	movs	r3, #3
3000a23e:	62e3      	str	r3, [r4, #44]	; 0x2c
3000a240:	2308      	movs	r3, #8
3000a242:	6023      	str	r3, [r4, #0]
3000a244:	6823      	ldr	r3, [r4, #0]
3000a246:	071d      	lsls	r5, r3, #28
3000a248:	d5fc      	bpl.n	3000a244 <rxi316_dram_init+0x838>
3000a24a:	4b4a      	ldr	r3, [pc, #296]	; (3000a374 <rxi316_dram_init+0x968>)
3000a24c:	2001      	movs	r0, #1
3000a24e:	4798      	blx	r3
3000a250:	68e3      	ldr	r3, [r4, #12]
3000a252:	4949      	ldr	r1, [pc, #292]	; (3000a378 <rxi316_dram_init+0x96c>)
3000a254:	400b      	ands	r3, r1
3000a256:	60e3      	str	r3, [r4, #12]
3000a258:	f44f 1300 	mov.w	r3, #2097152	; 0x200000
3000a25c:	62a3      	str	r3, [r4, #40]	; 0x28
3000a25e:	2303      	movs	r3, #3
3000a260:	62e3      	str	r3, [r4, #44]	; 0x2c
3000a262:	2308      	movs	r3, #8
3000a264:	6023      	str	r3, [r4, #0]
3000a266:	4b3e      	ldr	r3, [pc, #248]	; (3000a360 <rxi316_dram_init+0x954>)
3000a268:	681a      	ldr	r2, [r3, #0]
3000a26a:	0710      	lsls	r0, r2, #28
3000a26c:	d5fc      	bpl.n	3000a268 <rxi316_dram_init+0x85c>
3000a26e:	68da      	ldr	r2, [r3, #12]
3000a270:	400a      	ands	r2, r1
3000a272:	60da      	str	r2, [r3, #12]
3000a274:	f04f 7290 	mov.w	r2, #18874368	; 0x1200000
3000a278:	629a      	str	r2, [r3, #40]	; 0x28
3000a27a:	6ada      	ldr	r2, [r3, #44]	; 0x2c
3000a27c:	f022 0202 	bic.w	r2, r2, #2
3000a280:	62da      	str	r2, [r3, #44]	; 0x2c
3000a282:	2208      	movs	r2, #8
3000a284:	601a      	str	r2, [r3, #0]
3000a286:	4a36      	ldr	r2, [pc, #216]	; (3000a360 <rxi316_dram_init+0x954>)
3000a288:	6813      	ldr	r3, [r2, #0]
3000a28a:	0719      	lsls	r1, r3, #28
3000a28c:	d5fc      	bpl.n	3000a288 <rxi316_dram_init+0x87c>
3000a28e:	4b34      	ldr	r3, [pc, #208]	; (3000a360 <rxi316_dram_init+0x954>)
3000a290:	f44f 62c0 	mov.w	r2, #1536	; 0x600
3000a294:	60da      	str	r2, [r3, #12]
3000a296:	9909      	ldr	r1, [sp, #36]	; 0x24
3000a298:	691a      	ldr	r2, [r3, #16]
3000a29a:	4311      	orrs	r1, r2
3000a29c:	6119      	str	r1, [r3, #16]
3000a29e:	691a      	ldr	r2, [r3, #16]
3000a2a0:	f022 5280 	bic.w	r2, r2, #268435456	; 0x10000000
3000a2a4:	611a      	str	r2, [r3, #16]
3000a2a6:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000a2aa:	601a      	str	r2, [r3, #0]
3000a2ac:	68da      	ldr	r2, [r3, #12]
3000a2ae:	f3c2 020a 	ubfx	r2, r2, #0, #11
3000a2b2:	f5b2 6fc0 	cmp.w	r2, #1536	; 0x600
3000a2b6:	d1f9      	bne.n	3000a2ac <rxi316_dram_init+0x8a0>
3000a2b8:	b00d      	add	sp, #52	; 0x34
3000a2ba:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a2be:	6a77      	ldr	r7, [r6, #36]	; 0x24
3000a2c0:	e57e      	b.n	30009dc0 <rxi316_dram_init+0x3b4>
3000a2c2:	38c8      	subs	r0, #200	; 0xc8
3000a2c4:	e5e5      	b.n	30009e92 <rxi316_dram_init+0x486>
3000a2c6:	f8cd e020 	str.w	lr, [sp, #32]
3000a2ca:	e604      	b.n	30009ed6 <rxi316_dram_init+0x4ca>
3000a2cc:	9802      	ldr	r0, [sp, #8]
3000a2ce:	2802      	cmp	r0, #2
3000a2d0:	d009      	beq.n	3000a2e6 <rxi316_dram_init+0x8da>
3000a2d2:	2803      	cmp	r0, #3
3000a2d4:	d105      	bne.n	3000a2e2 <rxi316_dram_init+0x8d6>
3000a2d6:	f108 0802 	add.w	r8, r8, #2
3000a2da:	980a      	ldr	r0, [sp, #40]	; 0x28
3000a2dc:	eba8 0800 	sub.w	r8, r8, r0
3000a2e0:	e67e      	b.n	30009fe0 <rxi316_dram_init+0x5d4>
3000a2e2:	9801      	ldr	r0, [sp, #4]
3000a2e4:	e67d      	b.n	30009fe2 <rxi316_dram_init+0x5d6>
3000a2e6:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000a2ea:	e67b      	b.n	30009fe4 <rxi316_dram_init+0x5d8>
3000a2ec:	6a12      	ldr	r2, [r2, #32]
3000a2ee:	fbb2 f7f3 	udiv	r7, r2, r3
3000a2f2:	fb03 2217 	mls	r2, r3, r7, r2
3000a2f6:	b102      	cbz	r2, 3000a2fa <rxi316_dram_init+0x8ee>
3000a2f8:	3701      	adds	r7, #1
3000a2fa:	46b0      	mov	r8, r6
3000a2fc:	e6b6      	b.n	3000a06c <rxi316_dram_init+0x660>
3000a2fe:	f04f 0802 	mov.w	r8, #2
3000a302:	e6ad      	b.n	3000a060 <rxi316_dram_init+0x654>
3000a304:	9a02      	ldr	r2, [sp, #8]
3000a306:	2a03      	cmp	r2, #3
3000a308:	d113      	bne.n	3000a332 <rxi316_dram_init+0x926>
3000a30a:	4a1c      	ldr	r2, [pc, #112]	; (3000a37c <rxi316_dram_init+0x970>)
3000a30c:	4e1c      	ldr	r6, [pc, #112]	; (3000a380 <rxi316_dram_init+0x974>)
3000a30e:	fbb2 fef3 	udiv	lr, r2, r3
3000a312:	f242 7210 	movw	r2, #10000	; 0x2710
3000a316:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000a31a:	fbb2 f2f3 	udiv	r2, r2, r3
3000a31e:	fbb6 f6f3 	udiv	r6, r6, r3
3000a322:	444a      	add	r2, r9
3000a324:	0ab6      	lsrs	r6, r6, #10
3000a326:	f10e 0e01 	add.w	lr, lr, #1
3000a32a:	09d2      	lsrs	r2, r2, #7
3000a32c:	3601      	adds	r6, #1
3000a32e:	3201      	adds	r2, #1
3000a330:	e6ac      	b.n	3000a08c <rxi316_dram_init+0x680>
3000a332:	9a02      	ldr	r2, [sp, #8]
3000a334:	2a09      	cmp	r2, #9
3000a336:	d106      	bne.n	3000a346 <rxi316_dram_init+0x93a>
3000a338:	4e10      	ldr	r6, [pc, #64]	; (3000a37c <rxi316_dram_init+0x970>)
3000a33a:	2200      	movs	r2, #0
3000a33c:	fbb6 f6f3 	udiv	r6, r6, r3
3000a340:	0ab6      	lsrs	r6, r6, #10
3000a342:	3601      	adds	r6, #1
3000a344:	e6a0      	b.n	3000a088 <rxi316_dram_init+0x67c>
3000a346:	2200      	movs	r2, #0
3000a348:	4616      	mov	r6, r2
3000a34a:	4696      	mov	lr, r2
3000a34c:	e69e      	b.n	3000a08c <rxi316_dram_init+0x680>
3000a34e:	2501      	movs	r5, #1
3000a350:	e6fc      	b.n	3000a14c <rxi316_dram_init+0x740>
3000a352:	2201      	movs	r2, #1
3000a354:	601a      	str	r2, [r3, #0]
3000a356:	4a02      	ldr	r2, [pc, #8]	; (3000a360 <rxi316_dram_init+0x954>)
3000a358:	6813      	ldr	r3, [r2, #0]
3000a35a:	07db      	lsls	r3, r3, #31
3000a35c:	d5fc      	bpl.n	3000a358 <rxi316_dram_init+0x94c>
3000a35e:	e796      	b.n	3000a28e <rxi316_dram_init+0x882>
3000a360:	41100000 	.word	0x41100000
3000a364:	42008000 	.word	0x42008000
3000a368:	41011000 	.word	0x41011000
3000a36c:	44000103 	.word	0x44000103
3000a370:	44000021 	.word	0x44000021
3000a374:	00009b2d 	.word	0x00009b2d
3000a378:	fff8fffc 	.word	0xfff8fffc
3000a37c:	0bebc200 	.word	0x0bebc200
3000a380:	1dcd6500 	.word	0x1dcd6500

3000a384 <rxi316_ftend_init>:
3000a384:	2300      	movs	r3, #0
3000a386:	b510      	push	{r4, lr}
3000a388:	4c0c      	ldr	r4, [pc, #48]	; (3000a3bc <rxi316_ftend_init+0x38>)
3000a38a:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000a38e:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000a392:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000a396:	f7fe ff73 	bl	30009280 <ddr_init_index>
3000a39a:	4b09      	ldr	r3, [pc, #36]	; (3000a3c0 <rxi316_ftend_init+0x3c>)
3000a39c:	f8d4 2104 	ldr.w	r2, [r4, #260]	; 0x104
3000a3a0:	f853 3020 	ldr.w	r3, [r3, r0, lsl #2]
3000a3a4:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000a3a8:	68db      	ldr	r3, [r3, #12]
3000a3aa:	689b      	ldr	r3, [r3, #8]
3000a3ac:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000a3ae:	3301      	adds	r3, #1
3000a3b0:	b2db      	uxtb	r3, r3
3000a3b2:	4313      	orrs	r3, r2
3000a3b4:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000a3b8:	bd10      	pop	{r4, pc}
3000a3ba:	bf00      	nop
3000a3bc:	41101000 	.word	0x41101000
3000a3c0:	3000e3ec 	.word	0x3000e3ec

3000a3c4 <rxi316_DynSre_init>:
3000a3c4:	2901      	cmp	r1, #1
3000a3c6:	4b12      	ldr	r3, [pc, #72]	; (3000a410 <rxi316_DynSre_init+0x4c>)
3000a3c8:	d11e      	bne.n	3000a408 <rxi316_DynSre_init+0x44>
3000a3ca:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a3ce:	b280      	uxth	r0, r0
3000a3d0:	0c12      	lsrs	r2, r2, #16
3000a3d2:	0412      	lsls	r2, r2, #16
3000a3d4:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000a3d8:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000a3dc:	4310      	orrs	r0, r2
3000a3de:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000a3e2:	689a      	ldr	r2, [r3, #8]
3000a3e4:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000a3e8:	609a      	str	r2, [r3, #8]
3000a3ea:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000a3ee:	4b08      	ldr	r3, [pc, #32]	; (3000a410 <rxi316_DynSre_init+0x4c>)
3000a3f0:	625a      	str	r2, [r3, #36]	; 0x24
3000a3f2:	629a      	str	r2, [r3, #40]	; 0x28
3000a3f4:	69da      	ldr	r2, [r3, #28]
3000a3f6:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000a3fa:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000a3fe:	61da      	str	r2, [r3, #28]
3000a400:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000a404:	601a      	str	r2, [r3, #0]
3000a406:	4770      	bx	lr
3000a408:	689a      	ldr	r2, [r3, #8]
3000a40a:	f022 0280 	bic.w	r2, r2, #128	; 0x80
3000a40e:	e7eb      	b.n	3000a3e8 <rxi316_DynSre_init+0x24>
3000a410:	41100000 	.word	0x41100000

3000a414 <ddr_init>:
3000a414:	b507      	push	{r0, r1, r2, lr}
3000a416:	f7fe ff33 	bl	30009280 <ddr_init_index>
3000a41a:	4b07      	ldr	r3, [pc, #28]	; (3000a438 <ddr_init+0x24>)
3000a41c:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000a420:	9001      	str	r0, [sp, #4]
3000a422:	f7fe ff57 	bl	300092d4 <rxi316_perf_tune>
3000a426:	9801      	ldr	r0, [sp, #4]
3000a428:	f7ff faf0 	bl	30009a0c <rxi316_dram_init>
3000a42c:	b003      	add	sp, #12
3000a42e:	f85d eb04 	ldr.w	lr, [sp], #4
3000a432:	f7ff bfa7 	b.w	3000a384 <rxi316_ftend_init>
3000a436:	bf00      	nop
3000a438:	3000e3ec 	.word	0x3000e3ec

3000a43c <SWR_Calib_Check>:
3000a43c:	2300      	movs	r3, #0
3000a43e:	b507      	push	{r0, r1, r2, lr}
3000a440:	3801      	subs	r0, #1
3000a442:	f88d 3007 	strb.w	r3, [sp, #7]
3000a446:	4b0c      	ldr	r3, [pc, #48]	; (3000a478 <SWR_Calib_Check+0x3c>)
3000a448:	f10d 0107 	add.w	r1, sp, #7
3000a44c:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000a450:	f7fd fc82 	bl	30007d58 <OTP_Read8>
3000a454:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a458:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000a45c:	2af0      	cmp	r2, #240	; 0xf0
3000a45e:	d108      	bne.n	3000a472 <SWR_Calib_Check+0x36>
3000a460:	f003 030f 	and.w	r3, r3, #15
3000a464:	2b0f      	cmp	r3, #15
3000a466:	bf14      	ite	ne
3000a468:	2002      	movne	r0, #2
3000a46a:	2001      	moveq	r0, #1
3000a46c:	b003      	add	sp, #12
3000a46e:	f85d fb04 	ldr.w	pc, [sp], #4
3000a472:	2003      	movs	r0, #3
3000a474:	e7fa      	b.n	3000a46c <SWR_Calib_Check+0x30>
3000a476:	bf00      	nop
3000a478:	3000bd64 	.word	0x3000bd64

3000a47c <SWR_Calib_DCore>:
3000a47c:	4b10      	ldr	r3, [pc, #64]	; (3000a4c0 <SWR_Calib_DCore+0x44>)
3000a47e:	6d1a      	ldr	r2, [r3, #80]	; 0x50
3000a480:	f422 724c 	bic.w	r2, r2, #816	; 0x330
3000a484:	f022 0203 	bic.w	r2, r2, #3
3000a488:	f442 724c 	orr.w	r2, r2, #816	; 0x330
3000a48c:	f042 0201 	orr.w	r2, r2, #1
3000a490:	651a      	str	r2, [r3, #80]	; 0x50
3000a492:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000a494:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000a498:	641a      	str	r2, [r3, #64]	; 0x40
3000a49a:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000a49e:	f042 0201 	orr.w	r2, r2, #1
3000a4a2:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000a4a6:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000a4aa:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000a4ae:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000a4b2:	685a      	ldr	r2, [r3, #4]
3000a4b4:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000a4b8:	f042 0204 	orr.w	r2, r2, #4
3000a4bc:	605a      	str	r2, [r3, #4]
3000a4be:	4770      	bx	lr
3000a4c0:	42008100 	.word	0x42008100

3000a4c4 <SWR_Calib_MEM>:
3000a4c4:	4a3c      	ldr	r2, [pc, #240]	; (3000a5b8 <SWR_Calib_MEM+0xf4>)
3000a4c6:	b513      	push	{r0, r1, r4, lr}
3000a4c8:	f8d2 30e0 	ldr.w	r3, [r2, #224]	; 0xe0
3000a4cc:	2002      	movs	r0, #2
3000a4ce:	4c3b      	ldr	r4, [pc, #236]	; (3000a5bc <SWR_Calib_MEM+0xf8>)
3000a4d0:	f423 734c 	bic.w	r3, r3, #816	; 0x330
3000a4d4:	f023 0303 	bic.w	r3, r3, #3
3000a4d8:	f443 734c 	orr.w	r3, r3, #816	; 0x330
3000a4dc:	f043 0301 	orr.w	r3, r3, #1
3000a4e0:	f8c2 30e0 	str.w	r3, [r2, #224]	; 0xe0
3000a4e4:	f8d2 30d0 	ldr.w	r3, [r2, #208]	; 0xd0
3000a4e8:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000a4ec:	f8c2 30d0 	str.w	r3, [r2, #208]	; 0xd0
3000a4f0:	f8d2 30f8 	ldr.w	r3, [r2, #248]	; 0xf8
3000a4f4:	f043 0301 	orr.w	r3, r3, #1
3000a4f8:	f8c2 30f8 	str.w	r3, [r2, #248]	; 0xf8
3000a4fc:	f8d2 30fc 	ldr.w	r3, [r2, #252]	; 0xfc
3000a500:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a504:	f8c2 30fc 	str.w	r3, [r2, #252]	; 0xfc
3000a508:	f7ff ff98 	bl	3000a43c <SWR_Calib_Check>
3000a50c:	b2c0      	uxtb	r0, r0
3000a50e:	2801      	cmp	r0, #1
3000a510:	7020      	strb	r0, [r4, #0]
3000a512:	d025      	beq.n	3000a560 <SWR_Calib_MEM+0x9c>
3000a514:	f10d 0106 	add.w	r1, sp, #6
3000a518:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000a51c:	f7fd fc1c 	bl	30007d58 <OTP_Read8>
3000a520:	7822      	ldrb	r2, [r4, #0]
3000a522:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000a526:	2a03      	cmp	r2, #3
3000a528:	d139      	bne.n	3000a59e <SWR_Calib_MEM+0xda>
3000a52a:	091b      	lsrs	r3, r3, #4
3000a52c:	4a22      	ldr	r2, [pc, #136]	; (3000a5b8 <SWR_Calib_MEM+0xf4>)
3000a52e:	f8d2 10d0 	ldr.w	r1, [r2, #208]	; 0xd0
3000a532:	f021 010f 	bic.w	r1, r1, #15
3000a536:	430b      	orrs	r3, r1
3000a538:	4921      	ldr	r1, [pc, #132]	; (3000a5c0 <SWR_Calib_MEM+0xfc>)
3000a53a:	f8c2 30d0 	str.w	r3, [r2, #208]	; 0xd0
3000a53e:	6c8b      	ldr	r3, [r1, #72]	; 0x48
3000a540:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000a544:	2b07      	cmp	r3, #7
3000a546:	d10b      	bne.n	3000a560 <SWR_Calib_MEM+0x9c>
3000a548:	6c8b      	ldr	r3, [r1, #72]	; 0x48
3000a54a:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000a54e:	bf01      	itttt	eq
3000a550:	f8d2 30e4 	ldreq.w	r3, [r2, #228]	; 0xe4
3000a554:	f423 73e0 	biceq.w	r3, r3, #448	; 0x1c0
3000a558:	f443 6360 	orreq.w	r3, r3, #3584	; 0xe00
3000a55c:	f8c2 30e4 	streq.w	r3, [r2, #228]	; 0xe4
3000a560:	2003      	movs	r0, #3
3000a562:	4c18      	ldr	r4, [pc, #96]	; (3000a5c4 <SWR_Calib_MEM+0x100>)
3000a564:	f7ff ff6a 	bl	3000a43c <SWR_Calib_Check>
3000a568:	b2c0      	uxtb	r0, r0
3000a56a:	2801      	cmp	r0, #1
3000a56c:	7020      	strb	r0, [r4, #0]
3000a56e:	d014      	beq.n	3000a59a <SWR_Calib_MEM+0xd6>
3000a570:	f10d 0107 	add.w	r1, sp, #7
3000a574:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000a578:	f7fd fbee 	bl	30007d58 <OTP_Read8>
3000a57c:	7822      	ldrb	r2, [r4, #0]
3000a57e:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a582:	2a03      	cmp	r2, #3
3000a584:	d111      	bne.n	3000a5aa <SWR_Calib_MEM+0xe6>
3000a586:	091b      	lsrs	r3, r3, #4
3000a588:	490b      	ldr	r1, [pc, #44]	; (3000a5b8 <SWR_Calib_MEM+0xf4>)
3000a58a:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000a58e:	f022 7270 	bic.w	r2, r2, #62914560	; 0x3c00000
3000a592:	ea42 5383 	orr.w	r3, r2, r3, lsl #22
3000a596:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000a59a:	b002      	add	sp, #8
3000a59c:	bd10      	pop	{r4, pc}
3000a59e:	2a02      	cmp	r2, #2
3000a5a0:	bf0c      	ite	eq
3000a5a2:	f003 030f 	andeq.w	r3, r3, #15
3000a5a6:	2300      	movne	r3, #0
3000a5a8:	e7c0      	b.n	3000a52c <SWR_Calib_MEM+0x68>
3000a5aa:	2a02      	cmp	r2, #2
3000a5ac:	bf0c      	ite	eq
3000a5ae:	f003 030f 	andeq.w	r3, r3, #15
3000a5b2:	2300      	movne	r3, #0
3000a5b4:	e7e8      	b.n	3000a588 <SWR_Calib_MEM+0xc4>
3000a5b6:	bf00      	nop
3000a5b8:	42008800 	.word	0x42008800
3000a5bc:	3000e587 	.word	0x3000e587
3000a5c0:	42008100 	.word	0x42008100
3000a5c4:	3000e586 	.word	0x3000e586

3000a5c8 <SWR_Calib_AUD>:
3000a5c8:	4a33      	ldr	r2, [pc, #204]	; (3000a698 <SWR_Calib_AUD+0xd0>)
3000a5ca:	b513      	push	{r0, r1, r4, lr}
3000a5cc:	f8d2 30a0 	ldr.w	r3, [r2, #160]	; 0xa0
3000a5d0:	2004      	movs	r0, #4
3000a5d2:	4c32      	ldr	r4, [pc, #200]	; (3000a69c <SWR_Calib_AUD+0xd4>)
3000a5d4:	f423 734c 	bic.w	r3, r3, #816	; 0x330
3000a5d8:	f023 0303 	bic.w	r3, r3, #3
3000a5dc:	f443 734c 	orr.w	r3, r3, #816	; 0x330
3000a5e0:	f043 0301 	orr.w	r3, r3, #1
3000a5e4:	f8c2 30a0 	str.w	r3, [r2, #160]	; 0xa0
3000a5e8:	f8d2 3090 	ldr.w	r3, [r2, #144]	; 0x90
3000a5ec:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000a5f0:	f8c2 3090 	str.w	r3, [r2, #144]	; 0x90
3000a5f4:	f8d2 30b8 	ldr.w	r3, [r2, #184]	; 0xb8
3000a5f8:	f043 0301 	orr.w	r3, r3, #1
3000a5fc:	f8c2 30b8 	str.w	r3, [r2, #184]	; 0xb8
3000a600:	f8d2 30bc 	ldr.w	r3, [r2, #188]	; 0xbc
3000a604:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a608:	f8c2 30bc 	str.w	r3, [r2, #188]	; 0xbc
3000a60c:	f7ff ff16 	bl	3000a43c <SWR_Calib_Check>
3000a610:	b2c0      	uxtb	r0, r0
3000a612:	2801      	cmp	r0, #1
3000a614:	7020      	strb	r0, [r4, #0]
3000a616:	d013      	beq.n	3000a640 <SWR_Calib_AUD+0x78>
3000a618:	f10d 0106 	add.w	r1, sp, #6
3000a61c:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000a620:	f7fd fb9a 	bl	30007d58 <OTP_Read8>
3000a624:	7822      	ldrb	r2, [r4, #0]
3000a626:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000a62a:	2a03      	cmp	r2, #3
3000a62c:	d127      	bne.n	3000a67e <SWR_Calib_AUD+0xb6>
3000a62e:	091b      	lsrs	r3, r3, #4
3000a630:	4919      	ldr	r1, [pc, #100]	; (3000a698 <SWR_Calib_AUD+0xd0>)
3000a632:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000a636:	f022 020f 	bic.w	r2, r2, #15
3000a63a:	4313      	orrs	r3, r2
3000a63c:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000a640:	2005      	movs	r0, #5
3000a642:	4c17      	ldr	r4, [pc, #92]	; (3000a6a0 <SWR_Calib_AUD+0xd8>)
3000a644:	f7ff fefa 	bl	3000a43c <SWR_Calib_Check>
3000a648:	b2c0      	uxtb	r0, r0
3000a64a:	2801      	cmp	r0, #1
3000a64c:	7020      	strb	r0, [r4, #0]
3000a64e:	d014      	beq.n	3000a67a <SWR_Calib_AUD+0xb2>
3000a650:	f10d 0107 	add.w	r1, sp, #7
3000a654:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000a658:	f7fd fb7e 	bl	30007d58 <OTP_Read8>
3000a65c:	7822      	ldrb	r2, [r4, #0]
3000a65e:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000a662:	2a03      	cmp	r2, #3
3000a664:	d111      	bne.n	3000a68a <SWR_Calib_AUD+0xc2>
3000a666:	091b      	lsrs	r3, r3, #4
3000a668:	490b      	ldr	r1, [pc, #44]	; (3000a698 <SWR_Calib_AUD+0xd0>)
3000a66a:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000a66e:	f022 7270 	bic.w	r2, r2, #62914560	; 0x3c00000
3000a672:	ea42 5383 	orr.w	r3, r2, r3, lsl #22
3000a676:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000a67a:	b002      	add	sp, #8
3000a67c:	bd10      	pop	{r4, pc}
3000a67e:	2a02      	cmp	r2, #2
3000a680:	bf0c      	ite	eq
3000a682:	f003 030f 	andeq.w	r3, r3, #15
3000a686:	2300      	movne	r3, #0
3000a688:	e7d2      	b.n	3000a630 <SWR_Calib_AUD+0x68>
3000a68a:	2a02      	cmp	r2, #2
3000a68c:	bf0c      	ite	eq
3000a68e:	f003 030f 	andeq.w	r3, r3, #15
3000a692:	2300      	movne	r3, #0
3000a694:	e7e8      	b.n	3000a668 <SWR_Calib_AUD+0xa0>
3000a696:	bf00      	nop
3000a698:	42008800 	.word	0x42008800
3000a69c:	3000e585 	.word	0x3000e585
3000a6a0:	3000e584 	.word	0x3000e584

3000a6a4 <SWR_MEM>:
3000a6a4:	2801      	cmp	r0, #1
3000a6a6:	b538      	push	{r3, r4, r5, lr}
3000a6a8:	4605      	mov	r5, r0
3000a6aa:	4c2f      	ldr	r4, [pc, #188]	; (3000a768 <SWR_MEM+0xc4>)
3000a6ac:	d14e      	bne.n	3000a74c <SWR_MEM+0xa8>
3000a6ae:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000a6b2:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000a6b6:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a6ba:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000a6be:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a6c2:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000a6c6:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a6ca:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000a6ce:	f023 0303 	bic.w	r3, r3, #3
3000a6d2:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000a6d6:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a6da:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000a6de:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a6e2:	4b22      	ldr	r3, [pc, #136]	; (3000a76c <SWR_MEM+0xc8>)
3000a6e4:	4798      	blx	r3
3000a6e6:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000a6ea:	2803      	cmp	r0, #3
3000a6ec:	bf13      	iteet	ne
3000a6ee:	f043 0303 	orrne.w	r3, r3, #3
3000a6f2:	f8c4 30d8 	streq.w	r3, [r4, #216]	; 0xd8
3000a6f6:	f8d4 30d0 	ldreq.w	r3, [r4, #208]	; 0xd0
3000a6fa:	f8c4 30d8 	strne.w	r3, [r4, #216]	; 0xd8
3000a6fe:	bf16      	itet	ne
3000a700:	f8d4 30d0 	ldrne.w	r3, [r4, #208]	; 0xd0
3000a704:	f443 0340 	orreq.w	r3, r3, #12582912	; 0xc00000
3000a708:	f043 7340 	orrne.w	r3, r3, #50331648	; 0x3000000
3000a70c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a710:	4c15      	ldr	r4, [pc, #84]	; (3000a768 <SWR_MEM+0xc4>)
3000a712:	4b17      	ldr	r3, [pc, #92]	; (3000a770 <SWR_MEM+0xcc>)
3000a714:	4798      	blx	r3
3000a716:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a71a:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000a71e:	f043 0302 	orr.w	r3, r3, #2
3000a722:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a726:	4b13      	ldr	r3, [pc, #76]	; (3000a774 <SWR_MEM+0xd0>)
3000a728:	4798      	blx	r3
3000a72a:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a72e:	f043 0301 	orr.w	r3, r3, #1
3000a732:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a736:	4b10      	ldr	r3, [pc, #64]	; (3000a778 <SWR_MEM+0xd4>)
3000a738:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000a73c:	03d2      	lsls	r2, r2, #15
3000a73e:	d501      	bpl.n	3000a744 <SWR_MEM+0xa0>
3000a740:	2500      	movs	r5, #0
3000a742:	e001      	b.n	3000a748 <SWR_MEM+0xa4>
3000a744:	3b01      	subs	r3, #1
3000a746:	d1f7      	bne.n	3000a738 <SWR_MEM+0x94>
3000a748:	4628      	mov	r0, r5
3000a74a:	bd38      	pop	{r3, r4, r5, pc}
3000a74c:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a750:	f023 0301 	bic.w	r3, r3, #1
3000a754:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a758:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000a75c:	f023 0302 	bic.w	r3, r3, #2
3000a760:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000a764:	e7ec      	b.n	3000a740 <SWR_MEM+0x9c>
3000a766:	bf00      	nop
3000a768:	42008800 	.word	0x42008800
3000a76c:	3000726d 	.word	0x3000726d
3000a770:	3000a4c5 	.word	0x3000a4c5
3000a774:	00009b2d 	.word	0x00009b2d
3000a778:	000186a0 	.word	0x000186a0

3000a77c <SWR_MEM_Manual>:
3000a77c:	4b05      	ldr	r3, [pc, #20]	; (3000a794 <SWR_MEM_Manual+0x18>)
3000a77e:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000a782:	b120      	cbz	r0, 3000a78e <SWR_MEM_Manual+0x12>
3000a784:	f442 2200 	orr.w	r2, r2, #524288	; 0x80000
3000a788:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000a78c:	4770      	bx	lr
3000a78e:	f422 2200 	bic.w	r2, r2, #524288	; 0x80000
3000a792:	e7f9      	b.n	3000a788 <SWR_MEM_Manual+0xc>
3000a794:	42008800 	.word	0x42008800

3000a798 <SWR_MEM_Mode_Get>:
3000a798:	4b02      	ldr	r3, [pc, #8]	; (3000a7a4 <SWR_MEM_Mode_Get+0xc>)
3000a79a:	f8d3 00c0 	ldr.w	r0, [r3, #192]	; 0xc0
3000a79e:	f3c0 4080 	ubfx	r0, r0, #18, #1
3000a7a2:	4770      	bx	lr
3000a7a4:	42008800 	.word	0x42008800

3000a7a8 <SWR_MEM_Mode_Set>:
3000a7a8:	2801      	cmp	r0, #1
3000a7aa:	b538      	push	{r3, r4, r5, lr}
3000a7ac:	4c1f      	ldr	r4, [pc, #124]	; (3000a82c <SWR_MEM_Mode_Set+0x84>)
3000a7ae:	d915      	bls.n	3000a7dc <SWR_MEM_Mode_Set+0x34>
3000a7b0:	217e      	movs	r1, #126	; 0x7e
3000a7b2:	481f      	ldr	r0, [pc, #124]	; (3000a830 <SWR_MEM_Mode_Set+0x88>)
3000a7b4:	f000 fdcc 	bl	3000b350 <__io_assert_failed_veneer>
3000a7b8:	47a0      	blx	r4
3000a7ba:	b198      	cbz	r0, 3000a7e4 <SWR_MEM_Mode_Set+0x3c>
3000a7bc:	4b1d      	ldr	r3, [pc, #116]	; (3000a834 <SWR_MEM_Mode_Set+0x8c>)
3000a7be:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000a7c2:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000a7c6:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000a7ca:	f242 7210 	movw	r2, #10000	; 0x2710
3000a7ce:	f8d3 10c0 	ldr.w	r1, [r3, #192]	; 0xc0
3000a7d2:	0349      	lsls	r1, r1, #13
3000a7d4:	d506      	bpl.n	3000a7e4 <SWR_MEM_Mode_Set+0x3c>
3000a7d6:	3a01      	subs	r2, #1
3000a7d8:	d1f9      	bne.n	3000a7ce <SWR_MEM_Mode_Set+0x26>
3000a7da:	e025      	b.n	3000a828 <SWR_MEM_Mode_Set+0x80>
3000a7dc:	d1ec      	bne.n	3000a7b8 <SWR_MEM_Mode_Set+0x10>
3000a7de:	47a0      	blx	r4
3000a7e0:	2801      	cmp	r0, #1
3000a7e2:	d101      	bne.n	3000a7e8 <SWR_MEM_Mode_Set+0x40>
3000a7e4:	2000      	movs	r0, #0
3000a7e6:	bd38      	pop	{r3, r4, r5, pc}
3000a7e8:	4c12      	ldr	r4, [pc, #72]	; (3000a834 <SWR_MEM_Mode_Set+0x8c>)
3000a7ea:	2064      	movs	r0, #100	; 0x64
3000a7ec:	4d12      	ldr	r5, [pc, #72]	; (3000a838 <SWR_MEM_Mode_Set+0x90>)
3000a7ee:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000a7f2:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000a7f6:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000a7fa:	47a8      	blx	r5
3000a7fc:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000a800:	2064      	movs	r0, #100	; 0x64
3000a802:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000a806:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000a80a:	47a8      	blx	r5
3000a80c:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000a810:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000a814:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000a818:	f242 7310 	movw	r3, #10000	; 0x2710
3000a81c:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000a820:	0350      	lsls	r0, r2, #13
3000a822:	d4df      	bmi.n	3000a7e4 <SWR_MEM_Mode_Set+0x3c>
3000a824:	3b01      	subs	r3, #1
3000a826:	d1f9      	bne.n	3000a81c <SWR_MEM_Mode_Set+0x74>
3000a828:	2001      	movs	r0, #1
3000a82a:	e7dc      	b.n	3000a7e6 <SWR_MEM_Mode_Set+0x3e>
3000a82c:	3000a799 	.word	0x3000a799
3000a830:	3000bd78 	.word	0x3000bd78
3000a834:	42008800 	.word	0x42008800
3000a838:	00009b2d 	.word	0x00009b2d

3000a83c <SWR_AUDIO>:
3000a83c:	2801      	cmp	r0, #1
3000a83e:	b538      	push	{r3, r4, r5, lr}
3000a840:	4605      	mov	r5, r0
3000a842:	4c1d      	ldr	r4, [pc, #116]	; (3000a8b8 <SWR_AUDIO+0x7c>)
3000a844:	d12a      	bne.n	3000a89c <SWR_AUDIO+0x60>
3000a846:	4b1d      	ldr	r3, [pc, #116]	; (3000a8bc <SWR_AUDIO+0x80>)
3000a848:	4798      	blx	r3
3000a84a:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000a84e:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000a852:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000a856:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000a85a:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000a85e:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000a862:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000a866:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000a86a:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a86e:	f043 0302 	orr.w	r3, r3, #2
3000a872:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a876:	4b12      	ldr	r3, [pc, #72]	; (3000a8c0 <SWR_AUDIO+0x84>)
3000a878:	4798      	blx	r3
3000a87a:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a87e:	f043 0301 	orr.w	r3, r3, #1
3000a882:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a886:	4b0f      	ldr	r3, [pc, #60]	; (3000a8c4 <SWR_AUDIO+0x88>)
3000a888:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000a88c:	03d2      	lsls	r2, r2, #15
3000a88e:	d501      	bpl.n	3000a894 <SWR_AUDIO+0x58>
3000a890:	2500      	movs	r5, #0
3000a892:	e001      	b.n	3000a898 <SWR_AUDIO+0x5c>
3000a894:	3b01      	subs	r3, #1
3000a896:	d1f7      	bne.n	3000a888 <SWR_AUDIO+0x4c>
3000a898:	4628      	mov	r0, r5
3000a89a:	bd38      	pop	{r3, r4, r5, pc}
3000a89c:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a8a0:	f023 0301 	bic.w	r3, r3, #1
3000a8a4:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a8a8:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000a8ac:	f023 0302 	bic.w	r3, r3, #2
3000a8b0:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000a8b4:	e7ec      	b.n	3000a890 <SWR_AUDIO+0x54>
3000a8b6:	bf00      	nop
3000a8b8:	42008800 	.word	0x42008800
3000a8bc:	3000a5c9 	.word	0x3000a5c9
3000a8c0:	00009b2d 	.word	0x00009b2d
3000a8c4:	000186a0 	.word	0x000186a0

3000a8c8 <SWR_AUDIO_Manual>:
3000a8c8:	4b05      	ldr	r3, [pc, #20]	; (3000a8e0 <SWR_AUDIO_Manual+0x18>)
3000a8ca:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000a8ce:	b120      	cbz	r0, 3000a8da <SWR_AUDIO_Manual+0x12>
3000a8d0:	f442 2200 	orr.w	r2, r2, #524288	; 0x80000
3000a8d4:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000a8d8:	4770      	bx	lr
3000a8da:	f422 2200 	bic.w	r2, r2, #524288	; 0x80000
3000a8de:	e7f9      	b.n	3000a8d4 <SWR_AUDIO_Manual+0xc>
3000a8e0:	42008800 	.word	0x42008800

3000a8e4 <SWR_BST_MODE_Set>:
3000a8e4:	4a05      	ldr	r2, [pc, #20]	; (3000a8fc <SWR_BST_MODE_Set+0x18>)
3000a8e6:	6813      	ldr	r3, [r2, #0]
3000a8e8:	b118      	cbz	r0, 3000a8f2 <SWR_BST_MODE_Set+0xe>
3000a8ea:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000a8ee:	6013      	str	r3, [r2, #0]
3000a8f0:	4770      	bx	lr
3000a8f2:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000a8f6:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000a8fa:	e7f8      	b.n	3000a8ee <SWR_BST_MODE_Set+0xa>
3000a8fc:	42008100 	.word	0x42008100

3000a900 <crash_dump_memory>:
3000a900:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a904:	f1a1 0380 	sub.w	r3, r1, #128	; 0x80
3000a908:	4606      	mov	r6, r0
3000a90a:	460c      	mov	r4, r1
3000a90c:	481a      	ldr	r0, [pc, #104]	; (3000a978 <crash_dump_memory+0x78>)
3000a90e:	f5b3 5f7e 	cmp.w	r3, #16256	; 0x3f80
3000a912:	4b1a      	ldr	r3, [pc, #104]	; (3000a97c <crash_dump_memory+0x7c>)
3000a914:	491a      	ldr	r1, [pc, #104]	; (3000a980 <crash_dump_memory+0x80>)
3000a916:	4637      	mov	r7, r6
3000a918:	f8d3 327c 	ldr.w	r3, [r3, #636]	; 0x27c
3000a91c:	bf88      	it	hi
3000a91e:	2480      	movhi	r4, #128	; 0x80
3000a920:	2500      	movs	r5, #0
3000a922:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000a990 <crash_dump_memory+0x90>
3000a926:	f013 0fc0 	tst.w	r3, #192	; 0xc0
3000a92a:	4b16      	ldr	r3, [pc, #88]	; (3000a984 <crash_dump_memory+0x84>)
3000a92c:	f8df b064 	ldr.w	fp, [pc, #100]	; 3000a994 <crash_dump_memory+0x94>
3000a930:	bf08      	it	eq
3000a932:	4619      	moveq	r1, r3
3000a934:	f8df 8060 	ldr.w	r8, [pc, #96]	; 3000a998 <crash_dump_memory+0x98>
3000a938:	f000 fcfa 	bl	3000b330 <__DiagPrintf_veneer>
3000a93c:	4622      	mov	r2, r4
3000a93e:	4631      	mov	r1, r6
3000a940:	4811      	ldr	r0, [pc, #68]	; (3000a988 <crash_dump_memory+0x88>)
3000a942:	f000 fcf5 	bl	3000b330 <__DiagPrintf_veneer>
3000a946:	46b9      	mov	r9, r7
3000a948:	b98d      	cbnz	r5, 3000a96e <crash_dump_memory+0x6e>
3000a94a:	4631      	mov	r1, r6
3000a94c:	4658      	mov	r0, fp
3000a94e:	f000 fcef 	bl	3000b330 <__DiagPrintf_veneer>
3000a952:	3501      	adds	r5, #1
3000a954:	f8d9 1000 	ldr.w	r1, [r9]
3000a958:	4640      	mov	r0, r8
3000a95a:	3704      	adds	r7, #4
3000a95c:	f000 fce8 	bl	3000b330 <__DiagPrintf_veneer>
3000a960:	42ac      	cmp	r4, r5
3000a962:	d1f0      	bne.n	3000a946 <crash_dump_memory+0x46>
3000a964:	4809      	ldr	r0, [pc, #36]	; (3000a98c <crash_dump_memory+0x8c>)
3000a966:	e8bd 4ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a96a:	f000 bce1 	b.w	3000b330 <__DiagPrintf_veneer>
3000a96e:	07ab      	lsls	r3, r5, #30
3000a970:	d1ef      	bne.n	3000a952 <crash_dump_memory+0x52>
3000a972:	4639      	mov	r1, r7
3000a974:	4650      	mov	r0, sl
3000a976:	e7ea      	b.n	3000a94e <crash_dump_memory+0x4e>
3000a978:	3000bd91 	.word	0x3000bd91
3000a97c:	42008000 	.word	0x42008000
3000a980:	3000bd89 	.word	0x3000bd89
3000a984:	3000bd8d 	.word	0x3000bd8d
3000a988:	3000bdb5 	.word	0x3000bdb5
3000a98c:	3000bebb 	.word	0x3000bebb
3000a990:	3000bdef 	.word	0x3000bdef
3000a994:	3000bdf1 	.word	0x3000bdf1
3000a998:	3000bdf9 	.word	0x3000bdf9

3000a99c <crash_dump>:
3000a99c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000a9a0:	4c48      	ldr	r4, [pc, #288]	; (3000aac4 <crash_dump+0x128>)
3000a9a2:	4615      	mov	r5, r2
3000a9a4:	6a53      	ldr	r3, [r2, #36]	; 0x24
3000a9a6:	460e      	mov	r6, r1
3000a9a8:	4847      	ldr	r0, [pc, #284]	; (3000aac8 <crash_dump+0x12c>)
3000a9aa:	6023      	str	r3, [r4, #0]
3000a9ac:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000a9ae:	4f47      	ldr	r7, [pc, #284]	; (3000aacc <crash_dump+0x130>)
3000a9b0:	6063      	str	r3, [r4, #4]
3000a9b2:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000a9b4:	f8df 8134 	ldr.w	r8, [pc, #308]	; 3000aaec <crash_dump+0x150>
3000a9b8:	60a3      	str	r3, [r4, #8]
3000a9ba:	6b13      	ldr	r3, [r2, #48]	; 0x30
3000a9bc:	60e3      	str	r3, [r4, #12]
3000a9be:	6853      	ldr	r3, [r2, #4]
3000a9c0:	6123      	str	r3, [r4, #16]
3000a9c2:	6893      	ldr	r3, [r2, #8]
3000a9c4:	6163      	str	r3, [r4, #20]
3000a9c6:	68d3      	ldr	r3, [r2, #12]
3000a9c8:	61a3      	str	r3, [r4, #24]
3000a9ca:	6913      	ldr	r3, [r2, #16]
3000a9cc:	61e3      	str	r3, [r4, #28]
3000a9ce:	6953      	ldr	r3, [r2, #20]
3000a9d0:	6223      	str	r3, [r4, #32]
3000a9d2:	6993      	ldr	r3, [r2, #24]
3000a9d4:	6263      	str	r3, [r4, #36]	; 0x24
3000a9d6:	69d3      	ldr	r3, [r2, #28]
3000a9d8:	62a3      	str	r3, [r4, #40]	; 0x28
3000a9da:	6a13      	ldr	r3, [r2, #32]
3000a9dc:	62e3      	str	r3, [r4, #44]	; 0x2c
3000a9de:	6b53      	ldr	r3, [r2, #52]	; 0x34
3000a9e0:	6323      	str	r3, [r4, #48]	; 0x30
3000a9e2:	6b93      	ldr	r3, [r2, #56]	; 0x38
3000a9e4:	6363      	str	r3, [r4, #52]	; 0x34
3000a9e6:	6bd3      	ldr	r3, [r2, #60]	; 0x3c
3000a9e8:	63a3      	str	r3, [r4, #56]	; 0x38
3000a9ea:	6c13      	ldr	r3, [r2, #64]	; 0x40
3000a9ec:	63e3      	str	r3, [r4, #60]	; 0x3c
3000a9ee:	6813      	ldr	r3, [r2, #0]
3000a9f0:	6423      	str	r3, [r4, #64]	; 0x40
3000a9f2:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000a9f6:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000a9fa:	6462      	str	r2, [r4, #68]	; 0x44
3000a9fc:	4a34      	ldr	r2, [pc, #208]	; (3000aad0 <crash_dump+0x134>)
3000a9fe:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000aa02:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000aa06:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000aa0a:	64e1      	str	r1, [r4, #76]	; 0x4c
3000aa0c:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000aa10:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000aa14:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000aa18:	6561      	str	r1, [r4, #84]	; 0x54
3000aa1a:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000aa1c:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000aa20:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000aa24:	65e1      	str	r1, [r4, #92]	; 0x5c
3000aa26:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000aa28:	b289      	uxth	r1, r1
3000aa2a:	6621      	str	r1, [r4, #96]	; 0x60
3000aa2c:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000aa2e:	b292      	uxth	r2, r2
3000aa30:	6662      	str	r2, [r4, #100]	; 0x64
3000aa32:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000aa36:	66a2      	str	r2, [r4, #104]	; 0x68
3000aa38:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000aa3c:	66e3      	str	r3, [r4, #108]	; 0x6c
3000aa3e:	f000 fc77 	bl	3000b330 <__DiagPrintf_veneer>
3000aa42:	4620      	mov	r0, r4
3000aa44:	2409      	movs	r4, #9
3000aa46:	f000 f857 	bl	3000aaf8 <fault_diagnosis>
3000aa4a:	4822      	ldr	r0, [pc, #136]	; (3000aad4 <crash_dump+0x138>)
3000aa4c:	f000 fc70 	bl	3000b330 <__DiagPrintf_veneer>
3000aa50:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000aa52:	4821      	ldr	r0, [pc, #132]	; (3000aad8 <crash_dump+0x13c>)
3000aa54:	f000 fc6c 	bl	3000b330 <__DiagPrintf_veneer>
3000aa58:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000aa5a:	4820      	ldr	r0, [pc, #128]	; (3000aadc <crash_dump+0x140>)
3000aa5c:	f000 fc68 	bl	3000b330 <__DiagPrintf_veneer>
3000aa60:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000aa62:	481f      	ldr	r0, [pc, #124]	; (3000aae0 <crash_dump+0x144>)
3000aa64:	f000 fc64 	bl	3000b330 <__DiagPrintf_veneer>
3000aa68:	6829      	ldr	r1, [r5, #0]
3000aa6a:	481e      	ldr	r0, [pc, #120]	; (3000aae4 <crash_dump+0x148>)
3000aa6c:	f000 fc60 	bl	3000b330 <__DiagPrintf_veneer>
3000aa70:	f855 2024 	ldr.w	r2, [r5, r4, lsl #2]
3000aa74:	4640      	mov	r0, r8
3000aa76:	f857 1024 	ldr.w	r1, [r7, r4, lsl #2]
3000aa7a:	3401      	adds	r4, #1
3000aa7c:	f000 fc58 	bl	3000b330 <__DiagPrintf_veneer>
3000aa80:	2c0d      	cmp	r4, #13
3000aa82:	d1f5      	bne.n	3000aa70 <crash_dump+0xd4>
3000aa84:	2401      	movs	r4, #1
3000aa86:	f8df 8064 	ldr.w	r8, [pc, #100]	; 3000aaec <crash_dump+0x150>
3000aa8a:	f855 2024 	ldr.w	r2, [r5, r4, lsl #2]
3000aa8e:	4640      	mov	r0, r8
3000aa90:	f857 1024 	ldr.w	r1, [r7, r4, lsl #2]
3000aa94:	3401      	adds	r4, #1
3000aa96:	f000 fc4b 	bl	3000b330 <__DiagPrintf_veneer>
3000aa9a:	2c09      	cmp	r4, #9
3000aa9c:	d1f5      	bne.n	3000aa8a <crash_dump+0xee>
3000aa9e:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000aaa0:	4911      	ldr	r1, [pc, #68]	; (3000aae8 <crash_dump+0x14c>)
3000aaa2:	4812      	ldr	r0, [pc, #72]	; (3000aaec <crash_dump+0x150>)
3000aaa4:	f000 fc44 	bl	3000b330 <__DiagPrintf_veneer>
3000aaa8:	2180      	movs	r1, #128	; 0x80
3000aaaa:	4630      	mov	r0, r6
3000aaac:	f7ff ff28 	bl	3000a900 <crash_dump_memory>
3000aab0:	480f      	ldr	r0, [pc, #60]	; (3000aaf0 <crash_dump+0x154>)
3000aab2:	f000 fc3d 	bl	3000b330 <__DiagPrintf_veneer>
3000aab6:	480f      	ldr	r0, [pc, #60]	; (3000aaf4 <crash_dump+0x158>)
3000aab8:	f000 fc3a 	bl	3000b330 <__DiagPrintf_veneer>
3000aabc:	2000      	movs	r0, #0
3000aabe:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000aac2:	bf00      	nop
3000aac4:	3000eb78 	.word	0x3000eb78
3000aac8:	3000bdff 	.word	0x3000bdff
3000aacc:	3000bee4 	.word	0x3000bee4
3000aad0:	e000ed00 	.word	0xe000ed00
3000aad4:	3000be21 	.word	0x3000be21
3000aad8:	3000be46 	.word	0x3000be46
3000aadc:	3000be54 	.word	0x3000be54
3000aae0:	3000be62 	.word	0x3000be62
3000aae4:	3000be70 	.word	0x3000be70
3000aae8:	3000be8b 	.word	0x3000be8b
3000aaec:	3000be7e 	.word	0x3000be7e
3000aaf0:	3000be8f 	.word	0x3000be8f
3000aaf4:	3000beb8 	.word	0x3000beb8

3000aaf8 <fault_diagnosis>:
3000aaf8:	b510      	push	{r4, lr}
3000aafa:	4604      	mov	r4, r0
3000aafc:	6b81      	ldr	r1, [r0, #56]	; 0x38
3000aafe:	489d      	ldr	r0, [pc, #628]	; (3000ad74 <fault_diagnosis+0x27c>)
3000ab00:	f000 fc16 	bl	3000b330 <__DiagPrintf_veneer>
3000ab04:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000ab08:	079b      	lsls	r3, r3, #30
3000ab0a:	d504      	bpl.n	3000ab16 <fault_diagnosis+0x1e>
3000ab0c:	4a9a      	ldr	r2, [pc, #616]	; (3000ad78 <fault_diagnosis+0x280>)
3000ab0e:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000ab10:	489a      	ldr	r0, [pc, #616]	; (3000ad7c <fault_diagnosis+0x284>)
3000ab12:	f000 fc0d 	bl	3000b330 <__DiagPrintf_veneer>
3000ab16:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab18:	2900      	cmp	r1, #0
3000ab1a:	d049      	beq.n	3000abb0 <fault_diagnosis+0xb8>
3000ab1c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab20:	07d8      	lsls	r0, r3, #31
3000ab22:	d503      	bpl.n	3000ab2c <fault_diagnosis+0x34>
3000ab24:	4a96      	ldr	r2, [pc, #600]	; (3000ad80 <fault_diagnosis+0x288>)
3000ab26:	4897      	ldr	r0, [pc, #604]	; (3000ad84 <fault_diagnosis+0x28c>)
3000ab28:	f000 fc02 	bl	3000b330 <__DiagPrintf_veneer>
3000ab2c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab30:	0799      	lsls	r1, r3, #30
3000ab32:	d504      	bpl.n	3000ab3e <fault_diagnosis+0x46>
3000ab34:	4a94      	ldr	r2, [pc, #592]	; (3000ad88 <fault_diagnosis+0x290>)
3000ab36:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab38:	4892      	ldr	r0, [pc, #584]	; (3000ad84 <fault_diagnosis+0x28c>)
3000ab3a:	f000 fbf9 	bl	3000b330 <__DiagPrintf_veneer>
3000ab3e:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab42:	075a      	lsls	r2, r3, #29
3000ab44:	d504      	bpl.n	3000ab50 <fault_diagnosis+0x58>
3000ab46:	4a91      	ldr	r2, [pc, #580]	; (3000ad8c <fault_diagnosis+0x294>)
3000ab48:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab4a:	488e      	ldr	r0, [pc, #568]	; (3000ad84 <fault_diagnosis+0x28c>)
3000ab4c:	f000 fbf0 	bl	3000b330 <__DiagPrintf_veneer>
3000ab50:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab54:	071b      	lsls	r3, r3, #28
3000ab56:	d504      	bpl.n	3000ab62 <fault_diagnosis+0x6a>
3000ab58:	4a8d      	ldr	r2, [pc, #564]	; (3000ad90 <fault_diagnosis+0x298>)
3000ab5a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab5c:	4889      	ldr	r0, [pc, #548]	; (3000ad84 <fault_diagnosis+0x28c>)
3000ab5e:	f000 fbe7 	bl	3000b330 <__DiagPrintf_veneer>
3000ab62:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab66:	06d8      	lsls	r0, r3, #27
3000ab68:	d504      	bpl.n	3000ab74 <fault_diagnosis+0x7c>
3000ab6a:	4a8a      	ldr	r2, [pc, #552]	; (3000ad94 <fault_diagnosis+0x29c>)
3000ab6c:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab6e:	4885      	ldr	r0, [pc, #532]	; (3000ad84 <fault_diagnosis+0x28c>)
3000ab70:	f000 fbde 	bl	3000b330 <__DiagPrintf_veneer>
3000ab74:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab78:	0699      	lsls	r1, r3, #26
3000ab7a:	d504      	bpl.n	3000ab86 <fault_diagnosis+0x8e>
3000ab7c:	4a86      	ldr	r2, [pc, #536]	; (3000ad98 <fault_diagnosis+0x2a0>)
3000ab7e:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab80:	4880      	ldr	r0, [pc, #512]	; (3000ad84 <fault_diagnosis+0x28c>)
3000ab82:	f000 fbd5 	bl	3000b330 <__DiagPrintf_veneer>
3000ab86:	f994 3068 	ldrsb.w	r3, [r4, #104]	; 0x68
3000ab8a:	2b00      	cmp	r3, #0
3000ab8c:	da04      	bge.n	3000ab98 <fault_diagnosis+0xa0>
3000ab8e:	4a83      	ldr	r2, [pc, #524]	; (3000ad9c <fault_diagnosis+0x2a4>)
3000ab90:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ab92:	487c      	ldr	r0, [pc, #496]	; (3000ad84 <fault_diagnosis+0x28c>)
3000ab94:	f000 fbcc 	bl	3000b330 <__DiagPrintf_veneer>
3000ab98:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ab9c:	065a      	lsls	r2, r3, #25
3000ab9e:	d507      	bpl.n	3000abb0 <fault_diagnosis+0xb8>
3000aba0:	f013 0f28 	tst.w	r3, #40	; 0x28
3000aba4:	d004      	beq.n	3000abb0 <fault_diagnosis+0xb8>
3000aba6:	4a7e      	ldr	r2, [pc, #504]	; (3000ada0 <fault_diagnosis+0x2a8>)
3000aba8:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000abaa:	487e      	ldr	r0, [pc, #504]	; (3000ada4 <fault_diagnosis+0x2ac>)
3000abac:	f000 fbc0 	bl	3000b330 <__DiagPrintf_veneer>
3000abb0:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000abb4:	2900      	cmp	r1, #0
3000abb6:	d032      	beq.n	3000ac1e <fault_diagnosis+0x126>
3000abb8:	07cb      	lsls	r3, r1, #31
3000abba:	d503      	bpl.n	3000abc4 <fault_diagnosis+0xcc>
3000abbc:	4a7a      	ldr	r2, [pc, #488]	; (3000ada8 <fault_diagnosis+0x2b0>)
3000abbe:	487b      	ldr	r0, [pc, #492]	; (3000adac <fault_diagnosis+0x2b4>)
3000abc0:	f000 fbb6 	bl	3000b330 <__DiagPrintf_veneer>
3000abc4:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000abc8:	0788      	lsls	r0, r1, #30
3000abca:	d503      	bpl.n	3000abd4 <fault_diagnosis+0xdc>
3000abcc:	4a78      	ldr	r2, [pc, #480]	; (3000adb0 <fault_diagnosis+0x2b8>)
3000abce:	4877      	ldr	r0, [pc, #476]	; (3000adac <fault_diagnosis+0x2b4>)
3000abd0:	f000 fbae 	bl	3000b330 <__DiagPrintf_veneer>
3000abd4:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000abd8:	070a      	lsls	r2, r1, #28
3000abda:	d503      	bpl.n	3000abe4 <fault_diagnosis+0xec>
3000abdc:	4a75      	ldr	r2, [pc, #468]	; (3000adb4 <fault_diagnosis+0x2bc>)
3000abde:	4873      	ldr	r0, [pc, #460]	; (3000adac <fault_diagnosis+0x2b4>)
3000abe0:	f000 fba6 	bl	3000b330 <__DiagPrintf_veneer>
3000abe4:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000abe8:	06cb      	lsls	r3, r1, #27
3000abea:	d503      	bpl.n	3000abf4 <fault_diagnosis+0xfc>
3000abec:	4a72      	ldr	r2, [pc, #456]	; (3000adb8 <fault_diagnosis+0x2c0>)
3000abee:	486f      	ldr	r0, [pc, #444]	; (3000adac <fault_diagnosis+0x2b4>)
3000abf0:	f000 fb9e 	bl	3000b330 <__DiagPrintf_veneer>
3000abf4:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000abf8:	0688      	lsls	r0, r1, #26
3000abfa:	d503      	bpl.n	3000ac04 <fault_diagnosis+0x10c>
3000abfc:	4a6f      	ldr	r2, [pc, #444]	; (3000adbc <fault_diagnosis+0x2c4>)
3000abfe:	486b      	ldr	r0, [pc, #428]	; (3000adac <fault_diagnosis+0x2b4>)
3000ac00:	f000 fb96 	bl	3000b330 <__DiagPrintf_veneer>
3000ac04:	f994 2048 	ldrsb.w	r2, [r4, #72]	; 0x48
3000ac08:	f894 3048 	ldrb.w	r3, [r4, #72]	; 0x48
3000ac0c:	2a00      	cmp	r2, #0
3000ac0e:	da06      	bge.n	3000ac1e <fault_diagnosis+0x126>
3000ac10:	079a      	lsls	r2, r3, #30
3000ac12:	d004      	beq.n	3000ac1e <fault_diagnosis+0x126>
3000ac14:	4a6a      	ldr	r2, [pc, #424]	; (3000adc0 <fault_diagnosis+0x2c8>)
3000ac16:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000ac18:	486a      	ldr	r0, [pc, #424]	; (3000adc4 <fault_diagnosis+0x2cc>)
3000ac1a:	f000 fb89 	bl	3000b330 <__DiagPrintf_veneer>
3000ac1e:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ac22:	2900      	cmp	r1, #0
3000ac24:	d03a      	beq.n	3000ac9c <fault_diagnosis+0x1a4>
3000ac26:	07cb      	lsls	r3, r1, #31
3000ac28:	d503      	bpl.n	3000ac32 <fault_diagnosis+0x13a>
3000ac2a:	4a67      	ldr	r2, [pc, #412]	; (3000adc8 <fault_diagnosis+0x2d0>)
3000ac2c:	4867      	ldr	r0, [pc, #412]	; (3000adcc <fault_diagnosis+0x2d4>)
3000ac2e:	f000 fb7f 	bl	3000b330 <__DiagPrintf_veneer>
3000ac32:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ac36:	0788      	lsls	r0, r1, #30
3000ac38:	d503      	bpl.n	3000ac42 <fault_diagnosis+0x14a>
3000ac3a:	4a65      	ldr	r2, [pc, #404]	; (3000add0 <fault_diagnosis+0x2d8>)
3000ac3c:	4863      	ldr	r0, [pc, #396]	; (3000adcc <fault_diagnosis+0x2d4>)
3000ac3e:	f000 fb77 	bl	3000b330 <__DiagPrintf_veneer>
3000ac42:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ac46:	074a      	lsls	r2, r1, #29
3000ac48:	d503      	bpl.n	3000ac52 <fault_diagnosis+0x15a>
3000ac4a:	4a62      	ldr	r2, [pc, #392]	; (3000add4 <fault_diagnosis+0x2dc>)
3000ac4c:	485f      	ldr	r0, [pc, #380]	; (3000adcc <fault_diagnosis+0x2d4>)
3000ac4e:	f000 fb6f 	bl	3000b330 <__DiagPrintf_veneer>
3000ac52:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ac56:	070b      	lsls	r3, r1, #28
3000ac58:	d503      	bpl.n	3000ac62 <fault_diagnosis+0x16a>
3000ac5a:	4a5f      	ldr	r2, [pc, #380]	; (3000add8 <fault_diagnosis+0x2e0>)
3000ac5c:	485b      	ldr	r0, [pc, #364]	; (3000adcc <fault_diagnosis+0x2d4>)
3000ac5e:	f000 fb67 	bl	3000b330 <__DiagPrintf_veneer>
3000ac62:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ac66:	06c8      	lsls	r0, r1, #27
3000ac68:	d503      	bpl.n	3000ac72 <fault_diagnosis+0x17a>
3000ac6a:	4a5c      	ldr	r2, [pc, #368]	; (3000addc <fault_diagnosis+0x2e4>)
3000ac6c:	4857      	ldr	r0, [pc, #348]	; (3000adcc <fault_diagnosis+0x2d4>)
3000ac6e:	f000 fb5f 	bl	3000b330 <__DiagPrintf_veneer>
3000ac72:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ac76:	068a      	lsls	r2, r1, #26
3000ac78:	d503      	bpl.n	3000ac82 <fault_diagnosis+0x18a>
3000ac7a:	4a59      	ldr	r2, [pc, #356]	; (3000ade0 <fault_diagnosis+0x2e8>)
3000ac7c:	4853      	ldr	r0, [pc, #332]	; (3000adcc <fault_diagnosis+0x2d4>)
3000ac7e:	f000 fb57 	bl	3000b330 <__DiagPrintf_veneer>
3000ac82:	f994 2050 	ldrsb.w	r2, [r4, #80]	; 0x50
3000ac86:	f894 3050 	ldrb.w	r3, [r4, #80]	; 0x50
3000ac8a:	2a00      	cmp	r2, #0
3000ac8c:	da06      	bge.n	3000ac9c <fault_diagnosis+0x1a4>
3000ac8e:	079b      	lsls	r3, r3, #30
3000ac90:	d504      	bpl.n	3000ac9c <fault_diagnosis+0x1a4>
3000ac92:	4a54      	ldr	r2, [pc, #336]	; (3000ade4 <fault_diagnosis+0x2ec>)
3000ac94:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000ac96:	4854      	ldr	r0, [pc, #336]	; (3000ade8 <fault_diagnosis+0x2f0>)
3000ac98:	f000 fb4a 	bl	3000b330 <__DiagPrintf_veneer>
3000ac9c:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000aca0:	2900      	cmp	r1, #0
3000aca2:	d043      	beq.n	3000ad2c <fault_diagnosis+0x234>
3000aca4:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000aca8:	07d8      	lsls	r0, r3, #31
3000acaa:	d503      	bpl.n	3000acb4 <fault_diagnosis+0x1bc>
3000acac:	4a4f      	ldr	r2, [pc, #316]	; (3000adec <fault_diagnosis+0x2f4>)
3000acae:	4850      	ldr	r0, [pc, #320]	; (3000adf0 <fault_diagnosis+0x2f8>)
3000acb0:	f000 fb3e 	bl	3000b330 <__DiagPrintf_veneer>
3000acb4:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000acb8:	0799      	lsls	r1, r3, #30
3000acba:	d505      	bpl.n	3000acc8 <fault_diagnosis+0x1d0>
3000acbc:	4a4d      	ldr	r2, [pc, #308]	; (3000adf4 <fault_diagnosis+0x2fc>)
3000acbe:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000acc2:	484b      	ldr	r0, [pc, #300]	; (3000adf0 <fault_diagnosis+0x2f8>)
3000acc4:	f000 fb34 	bl	3000b330 <__DiagPrintf_veneer>
3000acc8:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000accc:	075a      	lsls	r2, r3, #29
3000acce:	d505      	bpl.n	3000acdc <fault_diagnosis+0x1e4>
3000acd0:	4a49      	ldr	r2, [pc, #292]	; (3000adf8 <fault_diagnosis+0x300>)
3000acd2:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000acd6:	4846      	ldr	r0, [pc, #280]	; (3000adf0 <fault_diagnosis+0x2f8>)
3000acd8:	f000 fb2a 	bl	3000b330 <__DiagPrintf_veneer>
3000acdc:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000ace0:	071b      	lsls	r3, r3, #28
3000ace2:	d505      	bpl.n	3000acf0 <fault_diagnosis+0x1f8>
3000ace4:	4a45      	ldr	r2, [pc, #276]	; (3000adfc <fault_diagnosis+0x304>)
3000ace6:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000acea:	4841      	ldr	r0, [pc, #260]	; (3000adf0 <fault_diagnosis+0x2f8>)
3000acec:	f000 fb20 	bl	3000b330 <__DiagPrintf_veneer>
3000acf0:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000acf4:	06d8      	lsls	r0, r3, #27
3000acf6:	d505      	bpl.n	3000ad04 <fault_diagnosis+0x20c>
3000acf8:	4a41      	ldr	r2, [pc, #260]	; (3000ae00 <fault_diagnosis+0x308>)
3000acfa:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000acfe:	483c      	ldr	r0, [pc, #240]	; (3000adf0 <fault_diagnosis+0x2f8>)
3000ad00:	f000 fb16 	bl	3000b330 <__DiagPrintf_veneer>
3000ad04:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000ad08:	07d9      	lsls	r1, r3, #31
3000ad0a:	d505      	bpl.n	3000ad18 <fault_diagnosis+0x220>
3000ad0c:	4a3d      	ldr	r2, [pc, #244]	; (3000ae04 <fault_diagnosis+0x30c>)
3000ad0e:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad12:	4837      	ldr	r0, [pc, #220]	; (3000adf0 <fault_diagnosis+0x2f8>)
3000ad14:	f000 fb0c 	bl	3000b330 <__DiagPrintf_veneer>
3000ad18:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000ad1c:	079a      	lsls	r2, r3, #30
3000ad1e:	d505      	bpl.n	3000ad2c <fault_diagnosis+0x234>
3000ad20:	4a39      	ldr	r2, [pc, #228]	; (3000ae08 <fault_diagnosis+0x310>)
3000ad22:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000ad26:	4832      	ldr	r0, [pc, #200]	; (3000adf0 <fault_diagnosis+0x2f8>)
3000ad28:	f000 fb02 	bl	3000b330 <__DiagPrintf_veneer>
3000ad2c:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000ad30:	2b00      	cmp	r3, #0
3000ad32:	f280 8087 	bge.w	3000ae44 <fault_diagnosis+0x34c>
3000ad36:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000ad38:	2900      	cmp	r1, #0
3000ad3a:	f000 8083 	beq.w	3000ae44 <fault_diagnosis+0x34c>
3000ad3e:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000ad42:	07db      	lsls	r3, r3, #31
3000ad44:	d503      	bpl.n	3000ad4e <fault_diagnosis+0x256>
3000ad46:	4a31      	ldr	r2, [pc, #196]	; (3000ae0c <fault_diagnosis+0x314>)
3000ad48:	4831      	ldr	r0, [pc, #196]	; (3000ae10 <fault_diagnosis+0x318>)
3000ad4a:	f000 faf1 	bl	3000b330 <__DiagPrintf_veneer>
3000ad4e:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000ad52:	0798      	lsls	r0, r3, #30
3000ad54:	d504      	bpl.n	3000ad60 <fault_diagnosis+0x268>
3000ad56:	4a2f      	ldr	r2, [pc, #188]	; (3000ae14 <fault_diagnosis+0x31c>)
3000ad58:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000ad5a:	482d      	ldr	r0, [pc, #180]	; (3000ae10 <fault_diagnosis+0x318>)
3000ad5c:	f000 fae8 	bl	3000b330 <__DiagPrintf_veneer>
3000ad60:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000ad64:	0759      	lsls	r1, r3, #29
3000ad66:	d559      	bpl.n	3000ae1c <fault_diagnosis+0x324>
3000ad68:	4a2b      	ldr	r2, [pc, #172]	; (3000ae18 <fault_diagnosis+0x320>)
3000ad6a:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000ad6c:	4828      	ldr	r0, [pc, #160]	; (3000ae10 <fault_diagnosis+0x318>)
3000ad6e:	f000 fadf 	bl	3000b330 <__DiagPrintf_veneer>
3000ad72:	e053      	b.n	3000ae1c <fault_diagnosis+0x324>
3000ad74:	3000bf1c 	.word	0x3000bf1c
3000ad78:	3000bf34 	.word	0x3000bf34
3000ad7c:	3000bf60 	.word	0x3000bf60
3000ad80:	3000bf75 	.word	0x3000bf75
3000ad84:	3000bfa5 	.word	0x3000bfa5
3000ad88:	3000bfba 	.word	0x3000bfba
3000ad8c:	3000bff2 	.word	0x3000bff2
3000ad90:	3000c027 	.word	0x3000c027
3000ad94:	3000c05e 	.word	0x3000c05e
3000ad98:	3000c08d 	.word	0x3000c08d
3000ad9c:	3000c0c7 	.word	0x3000c0c7
3000ada0:	3000c0f4 	.word	0x3000c0f4
3000ada4:	3000c11b 	.word	0x3000c11b
3000ada8:	3000c130 	.word	0x3000c130
3000adac:	3000c172 	.word	0x3000c172
3000adb0:	3000c188 	.word	0x3000c188
3000adb4:	3000c1c3 	.word	0x3000c1c3
3000adb8:	3000c1f9 	.word	0x3000c1f9
3000adbc:	3000c22d 	.word	0x3000c22d
3000adc0:	3000c279 	.word	0x3000c279
3000adc4:	3000c2a9 	.word	0x3000c2a9
3000adc8:	3000c2be 	.word	0x3000c2be
3000adcc:	3000c2f2 	.word	0x3000c2f2
3000add0:	3000c307 	.word	0x3000c307
3000add4:	3000c33c 	.word	0x3000c33c
3000add8:	3000c373 	.word	0x3000c373
3000addc:	3000c39b 	.word	0x3000c39b
3000ade0:	3000c3c1 	.word	0x3000c3c1
3000ade4:	3000c3ff 	.word	0x3000c3ff
3000ade8:	3000c421 	.word	0x3000c421
3000adec:	3000c436 	.word	0x3000c436
3000adf0:	3000c47c 	.word	0x3000c47c
3000adf4:	3000c491 	.word	0x3000c491
3000adf8:	3000c4dd 	.word	0x3000c4dd
3000adfc:	3000c53c 	.word	0x3000c53c
3000ae00:	3000c583 	.word	0x3000c583
3000ae04:	3000c5dd 	.word	0x3000c5dd
3000ae08:	3000c62f 	.word	0x3000c62f
3000ae0c:	3000c699 	.word	0x3000c699
3000ae10:	3000c6c9 	.word	0x3000c6c9
3000ae14:	3000c6de 	.word	0x3000c6de
3000ae18:	3000c711 	.word	0x3000c711
3000ae1c:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000ae20:	071a      	lsls	r2, r3, #28
3000ae22:	d504      	bpl.n	3000ae2e <fault_diagnosis+0x336>
3000ae24:	4a08      	ldr	r2, [pc, #32]	; (3000ae48 <fault_diagnosis+0x350>)
3000ae26:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000ae28:	4808      	ldr	r0, [pc, #32]	; (3000ae4c <fault_diagnosis+0x354>)
3000ae2a:	f000 fa81 	bl	3000b330 <__DiagPrintf_veneer>
3000ae2e:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000ae32:	06db      	lsls	r3, r3, #27
3000ae34:	d506      	bpl.n	3000ae44 <fault_diagnosis+0x34c>
3000ae36:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000ae38:	4a05      	ldr	r2, [pc, #20]	; (3000ae50 <fault_diagnosis+0x358>)
3000ae3a:	4804      	ldr	r0, [pc, #16]	; (3000ae4c <fault_diagnosis+0x354>)
3000ae3c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000ae40:	f000 ba76 	b.w	3000b330 <__DiagPrintf_veneer>
3000ae44:	bd10      	pop	{r4, pc}
3000ae46:	bf00      	nop
3000ae48:	3000c73d 	.word	0x3000c73d
3000ae4c:	3000c6c9 	.word	0x3000c6c9
3000ae50:	3000c76c 	.word	0x3000c76c

3000ae54 <rtk_log_level_get>:
3000ae54:	4b0f      	ldr	r3, [pc, #60]	; (3000ae94 <rtk_log_level_get+0x40>)
3000ae56:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000ae5a:	681a      	ldr	r2, [r3, #0]
3000ae5c:	4606      	mov	r6, r0
3000ae5e:	4d0e      	ldr	r5, [pc, #56]	; (3000ae98 <rtk_log_level_get+0x44>)
3000ae60:	2400      	movs	r4, #0
3000ae62:	2a02      	cmp	r2, #2
3000ae64:	f8df 803c 	ldr.w	r8, [pc, #60]	; 3000aea4 <rtk_log_level_get+0x50>
3000ae68:	bf94      	ite	ls
3000ae6a:	681f      	ldrls	r7, [r3, #0]
3000ae6c:	2703      	movhi	r7, #3
3000ae6e:	42bc      	cmp	r4, r7
3000ae70:	d102      	bne.n	3000ae78 <rtk_log_level_get+0x24>
3000ae72:	4b0a      	ldr	r3, [pc, #40]	; (3000ae9c <rtk_log_level_get+0x48>)
3000ae74:	7818      	ldrb	r0, [r3, #0]
3000ae76:	e008      	b.n	3000ae8a <rtk_log_level_get+0x36>
3000ae78:	4628      	mov	r0, r5
3000ae7a:	4631      	mov	r1, r6
3000ae7c:	350b      	adds	r5, #11
3000ae7e:	47c0      	blx	r8
3000ae80:	b928      	cbnz	r0, 3000ae8e <rtk_log_level_get+0x3a>
3000ae82:	230b      	movs	r3, #11
3000ae84:	435c      	muls	r4, r3
3000ae86:	4b06      	ldr	r3, [pc, #24]	; (3000aea0 <rtk_log_level_get+0x4c>)
3000ae88:	5d18      	ldrb	r0, [r3, r4]
3000ae8a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000ae8e:	3401      	adds	r4, #1
3000ae90:	e7ed      	b.n	3000ae6e <rtk_log_level_get+0x1a>
3000ae92:	bf00      	nop
3000ae94:	3000efb8 	.word	0x3000efb8
3000ae98:	3000efbd 	.word	0x3000efbd
3000ae9c:	3000e598 	.word	0x3000e598
3000aea0:	3000efbc 	.word	0x3000efbc
3000aea4:	00012c89 	.word	0x00012c89

3000aea8 <rtk_log_write>:
3000aea8:	b40c      	push	{r2, r3}
3000aeaa:	b513      	push	{r0, r1, r4, lr}
3000aeac:	4604      	mov	r4, r0
3000aeae:	4608      	mov	r0, r1
3000aeb0:	f7ff ffd0 	bl	3000ae54 <rtk_log_level_get>
3000aeb4:	42a0      	cmp	r0, r4
3000aeb6:	d305      	bcc.n	3000aec4 <rtk_log_write+0x1c>
3000aeb8:	aa05      	add	r2, sp, #20
3000aeba:	9904      	ldr	r1, [sp, #16]
3000aebc:	2000      	movs	r0, #0
3000aebe:	9201      	str	r2, [sp, #4]
3000aec0:	f000 fa66 	bl	3000b390 <__DiagVSprintf_veneer>
3000aec4:	b002      	add	sp, #8
3000aec6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000aeca:	b002      	add	sp, #8
3000aecc:	4770      	bx	lr
	...

3000aed0 <BOOT_VerCheck>:
3000aed0:	b570      	push	{r4, r5, r6, lr}
3000aed2:	4b1d      	ldr	r3, [pc, #116]	; (3000af48 <BOOT_VerCheck+0x78>)
3000aed4:	4798      	blx	r3
3000aed6:	2800      	cmp	r0, #0
3000aed8:	d135      	bne.n	3000af46 <BOOT_VerCheck+0x76>
3000aeda:	4b1c      	ldr	r3, [pc, #112]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000aedc:	2403      	movs	r4, #3
3000aede:	4d1c      	ldr	r5, [pc, #112]	; (3000af50 <BOOT_VerCheck+0x80>)
3000aee0:	2003      	movs	r0, #3
3000aee2:	4a1c      	ldr	r2, [pc, #112]	; (3000af54 <BOOT_VerCheck+0x84>)
3000aee4:	4619      	mov	r1, r3
3000aee6:	f7ff ffdf 	bl	3000aea8 <rtk_log_write>
3000aeea:	4b18      	ldr	r3, [pc, #96]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000aeec:	462a      	mov	r2, r5
3000aeee:	2003      	movs	r0, #3
3000aef0:	4619      	mov	r1, r3
3000aef2:	f7ff ffd9 	bl	3000aea8 <rtk_log_write>
3000aef6:	4b15      	ldr	r3, [pc, #84]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000aef8:	4a17      	ldr	r2, [pc, #92]	; (3000af58 <BOOT_VerCheck+0x88>)
3000aefa:	2003      	movs	r0, #3
3000aefc:	4619      	mov	r1, r3
3000aefe:	f7ff ffd3 	bl	3000aea8 <rtk_log_write>
3000af02:	4b12      	ldr	r3, [pc, #72]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000af04:	4a15      	ldr	r2, [pc, #84]	; (3000af5c <BOOT_VerCheck+0x8c>)
3000af06:	2003      	movs	r0, #3
3000af08:	4619      	mov	r1, r3
3000af0a:	f7ff ffcd 	bl	3000aea8 <rtk_log_write>
3000af0e:	4b0f      	ldr	r3, [pc, #60]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000af10:	4a13      	ldr	r2, [pc, #76]	; (3000af60 <BOOT_VerCheck+0x90>)
3000af12:	2003      	movs	r0, #3
3000af14:	4619      	mov	r1, r3
3000af16:	f7ff ffc7 	bl	3000aea8 <rtk_log_write>
3000af1a:	4b0c      	ldr	r3, [pc, #48]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000af1c:	462a      	mov	r2, r5
3000af1e:	2003      	movs	r0, #3
3000af20:	4619      	mov	r1, r3
3000af22:	f7ff ffc1 	bl	3000aea8 <rtk_log_write>
3000af26:	4b09      	ldr	r3, [pc, #36]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000af28:	4a0e      	ldr	r2, [pc, #56]	; (3000af64 <BOOT_VerCheck+0x94>)
3000af2a:	2003      	movs	r0, #3
3000af2c:	4619      	mov	r1, r3
3000af2e:	f7ff ffbb 	bl	3000aea8 <rtk_log_write>
3000af32:	3c01      	subs	r4, #1
3000af34:	d1d9      	bne.n	3000aeea <BOOT_VerCheck+0x1a>
3000af36:	4b05      	ldr	r3, [pc, #20]	; (3000af4c <BOOT_VerCheck+0x7c>)
3000af38:	2003      	movs	r0, #3
3000af3a:	4a0b      	ldr	r2, [pc, #44]	; (3000af68 <BOOT_VerCheck+0x98>)
3000af3c:	4619      	mov	r1, r3
3000af3e:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000af42:	f7ff bfb1 	b.w	3000aea8 <rtk_log_write>
3000af46:	bd70      	pop	{r4, r5, r6, pc}
3000af48:	30008fc9 	.word	0x30008fc9
3000af4c:	3000c79c 	.word	0x3000c79c
3000af50:	3000d35b 	.word	0x3000d35b
3000af54:	3000d318 	.word	0x3000d318
3000af58:	3000d39e 	.word	0x3000d39e
3000af5c:	3000d3e1 	.word	0x3000d3e1
3000af60:	3000d424 	.word	0x3000d424
3000af64:	3000d467 	.word	0x3000d467
3000af68:	3000d4aa 	.word	0x3000d4aa

3000af6c <__aeabi_uldivmod>:
3000af6c:	b953      	cbnz	r3, 3000af84 <__aeabi_uldivmod+0x18>
3000af6e:	b94a      	cbnz	r2, 3000af84 <__aeabi_uldivmod+0x18>
3000af70:	2900      	cmp	r1, #0
3000af72:	bf08      	it	eq
3000af74:	2800      	cmpeq	r0, #0
3000af76:	bf1c      	itt	ne
3000af78:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000af7c:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000af80:	f000 b960 	b.w	3000b244 <__aeabi_idiv0>
3000af84:	f1ad 0c08 	sub.w	ip, sp, #8
3000af88:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000af8c:	f000 f806 	bl	3000af9c <__udivmoddi4>
3000af90:	f8dd e004 	ldr.w	lr, [sp, #4]
3000af94:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000af98:	b004      	add	sp, #16
3000af9a:	4770      	bx	lr

3000af9c <__udivmoddi4>:
3000af9c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000afa0:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000afa2:	4688      	mov	r8, r1
3000afa4:	468e      	mov	lr, r1
3000afa6:	4615      	mov	r5, r2
3000afa8:	4604      	mov	r4, r0
3000afaa:	4619      	mov	r1, r3
3000afac:	2b00      	cmp	r3, #0
3000afae:	f040 80c6 	bne.w	3000b13e <__udivmoddi4+0x1a2>
3000afb2:	4542      	cmp	r2, r8
3000afb4:	fab2 f782 	clz	r7, r2
3000afb8:	d946      	bls.n	3000b048 <__udivmoddi4+0xac>
3000afba:	b14f      	cbz	r7, 3000afd0 <__udivmoddi4+0x34>
3000afbc:	f1c7 0c20 	rsb	ip, r7, #32
3000afc0:	fa08 f307 	lsl.w	r3, r8, r7
3000afc4:	40bd      	lsls	r5, r7
3000afc6:	40bc      	lsls	r4, r7
3000afc8:	fa20 fc0c 	lsr.w	ip, r0, ip
3000afcc:	ea4c 0e03 	orr.w	lr, ip, r3
3000afd0:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000afd4:	fa1f fc85 	uxth.w	ip, r5
3000afd8:	0c22      	lsrs	r2, r4, #16
3000afda:	fbbe f9f8 	udiv	r9, lr, r8
3000afde:	fb08 e319 	mls	r3, r8, r9, lr
3000afe2:	fb09 fa0c 	mul.w	sl, r9, ip
3000afe6:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000afea:	459a      	cmp	sl, r3
3000afec:	d928      	bls.n	3000b040 <__udivmoddi4+0xa4>
3000afee:	18eb      	adds	r3, r5, r3
3000aff0:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000aff4:	d204      	bcs.n	3000b000 <__udivmoddi4+0x64>
3000aff6:	459a      	cmp	sl, r3
3000aff8:	d902      	bls.n	3000b000 <__udivmoddi4+0x64>
3000affa:	f1a9 0002 	sub.w	r0, r9, #2
3000affe:	442b      	add	r3, r5
3000b000:	eba3 030a 	sub.w	r3, r3, sl
3000b004:	b2a4      	uxth	r4, r4
3000b006:	fbb3 f2f8 	udiv	r2, r3, r8
3000b00a:	fb08 3312 	mls	r3, r8, r2, r3
3000b00e:	fb02 fc0c 	mul.w	ip, r2, ip
3000b012:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b016:	45a4      	cmp	ip, r4
3000b018:	d914      	bls.n	3000b044 <__udivmoddi4+0xa8>
3000b01a:	192c      	adds	r4, r5, r4
3000b01c:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000b020:	d203      	bcs.n	3000b02a <__udivmoddi4+0x8e>
3000b022:	45a4      	cmp	ip, r4
3000b024:	d901      	bls.n	3000b02a <__udivmoddi4+0x8e>
3000b026:	1e93      	subs	r3, r2, #2
3000b028:	442c      	add	r4, r5
3000b02a:	eba4 040c 	sub.w	r4, r4, ip
3000b02e:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000b032:	b11e      	cbz	r6, 3000b03c <__udivmoddi4+0xa0>
3000b034:	40fc      	lsrs	r4, r7
3000b036:	2300      	movs	r3, #0
3000b038:	6034      	str	r4, [r6, #0]
3000b03a:	6073      	str	r3, [r6, #4]
3000b03c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b040:	4648      	mov	r0, r9
3000b042:	e7dd      	b.n	3000b000 <__udivmoddi4+0x64>
3000b044:	4613      	mov	r3, r2
3000b046:	e7f0      	b.n	3000b02a <__udivmoddi4+0x8e>
3000b048:	b902      	cbnz	r2, 3000b04c <__udivmoddi4+0xb0>
3000b04a:	deff      	udf	#255	; 0xff
3000b04c:	bb8f      	cbnz	r7, 3000b0b2 <__udivmoddi4+0x116>
3000b04e:	eba8 0302 	sub.w	r3, r8, r2
3000b052:	2101      	movs	r1, #1
3000b054:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b058:	b2aa      	uxth	r2, r5
3000b05a:	0c20      	lsrs	r0, r4, #16
3000b05c:	fbb3 fcfe 	udiv	ip, r3, lr
3000b060:	fb0e 331c 	mls	r3, lr, ip, r3
3000b064:	fb0c f802 	mul.w	r8, ip, r2
3000b068:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000b06c:	4598      	cmp	r8, r3
3000b06e:	d962      	bls.n	3000b136 <__udivmoddi4+0x19a>
3000b070:	18eb      	adds	r3, r5, r3
3000b072:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000b076:	d204      	bcs.n	3000b082 <__udivmoddi4+0xe6>
3000b078:	4598      	cmp	r8, r3
3000b07a:	d902      	bls.n	3000b082 <__udivmoddi4+0xe6>
3000b07c:	f1ac 0002 	sub.w	r0, ip, #2
3000b080:	442b      	add	r3, r5
3000b082:	eba3 0308 	sub.w	r3, r3, r8
3000b086:	b2a4      	uxth	r4, r4
3000b088:	fbb3 fcfe 	udiv	ip, r3, lr
3000b08c:	fb0e 331c 	mls	r3, lr, ip, r3
3000b090:	fb0c f202 	mul.w	r2, ip, r2
3000b094:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000b098:	42a2      	cmp	r2, r4
3000b09a:	d94e      	bls.n	3000b13a <__udivmoddi4+0x19e>
3000b09c:	192c      	adds	r4, r5, r4
3000b09e:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000b0a2:	d204      	bcs.n	3000b0ae <__udivmoddi4+0x112>
3000b0a4:	42a2      	cmp	r2, r4
3000b0a6:	d902      	bls.n	3000b0ae <__udivmoddi4+0x112>
3000b0a8:	f1ac 0302 	sub.w	r3, ip, #2
3000b0ac:	442c      	add	r4, r5
3000b0ae:	1aa4      	subs	r4, r4, r2
3000b0b0:	e7bd      	b.n	3000b02e <__udivmoddi4+0x92>
3000b0b2:	f1c7 0c20 	rsb	ip, r7, #32
3000b0b6:	40bd      	lsls	r5, r7
3000b0b8:	fa08 f307 	lsl.w	r3, r8, r7
3000b0bc:	40bc      	lsls	r4, r7
3000b0be:	fa28 f90c 	lsr.w	r9, r8, ip
3000b0c2:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000b0c6:	fa20 fc0c 	lsr.w	ip, r0, ip
3000b0ca:	ea4c 0203 	orr.w	r2, ip, r3
3000b0ce:	b2ab      	uxth	r3, r5
3000b0d0:	fbb9 fcfe 	udiv	ip, r9, lr
3000b0d4:	0c11      	lsrs	r1, r2, #16
3000b0d6:	fb0e 901c 	mls	r0, lr, ip, r9
3000b0da:	fb0c f803 	mul.w	r8, ip, r3
3000b0de:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b0e2:	4580      	cmp	r8, r0
3000b0e4:	d923      	bls.n	3000b12e <__udivmoddi4+0x192>
3000b0e6:	1828      	adds	r0, r5, r0
3000b0e8:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000b0ec:	d204      	bcs.n	3000b0f8 <__udivmoddi4+0x15c>
3000b0ee:	4580      	cmp	r8, r0
3000b0f0:	d902      	bls.n	3000b0f8 <__udivmoddi4+0x15c>
3000b0f2:	f1ac 0102 	sub.w	r1, ip, #2
3000b0f6:	4428      	add	r0, r5
3000b0f8:	eba0 0008 	sub.w	r0, r0, r8
3000b0fc:	b292      	uxth	r2, r2
3000b0fe:	fbb0 fcfe 	udiv	ip, r0, lr
3000b102:	fb0e 001c 	mls	r0, lr, ip, r0
3000b106:	fb0c f803 	mul.w	r8, ip, r3
3000b10a:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000b10e:	4598      	cmp	r8, r3
3000b110:	d90f      	bls.n	3000b132 <__udivmoddi4+0x196>
3000b112:	18eb      	adds	r3, r5, r3
3000b114:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000b118:	d204      	bcs.n	3000b124 <__udivmoddi4+0x188>
3000b11a:	4598      	cmp	r8, r3
3000b11c:	d902      	bls.n	3000b124 <__udivmoddi4+0x188>
3000b11e:	f1ac 0202 	sub.w	r2, ip, #2
3000b122:	442b      	add	r3, r5
3000b124:	eba3 0308 	sub.w	r3, r3, r8
3000b128:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000b12c:	e792      	b.n	3000b054 <__udivmoddi4+0xb8>
3000b12e:	4661      	mov	r1, ip
3000b130:	e7e2      	b.n	3000b0f8 <__udivmoddi4+0x15c>
3000b132:	4662      	mov	r2, ip
3000b134:	e7f6      	b.n	3000b124 <__udivmoddi4+0x188>
3000b136:	4660      	mov	r0, ip
3000b138:	e7a3      	b.n	3000b082 <__udivmoddi4+0xe6>
3000b13a:	4663      	mov	r3, ip
3000b13c:	e7b7      	b.n	3000b0ae <__udivmoddi4+0x112>
3000b13e:	4543      	cmp	r3, r8
3000b140:	d905      	bls.n	3000b14e <__udivmoddi4+0x1b2>
3000b142:	b10e      	cbz	r6, 3000b148 <__udivmoddi4+0x1ac>
3000b144:	e9c6 0800 	strd	r0, r8, [r6]
3000b148:	2100      	movs	r1, #0
3000b14a:	4608      	mov	r0, r1
3000b14c:	e776      	b.n	3000b03c <__udivmoddi4+0xa0>
3000b14e:	fab3 fc83 	clz	ip, r3
3000b152:	f1bc 0f00 	cmp.w	ip, #0
3000b156:	d110      	bne.n	3000b17a <__udivmoddi4+0x1de>
3000b158:	4543      	cmp	r3, r8
3000b15a:	d301      	bcc.n	3000b160 <__udivmoddi4+0x1c4>
3000b15c:	4282      	cmp	r2, r0
3000b15e:	d80a      	bhi.n	3000b176 <__udivmoddi4+0x1da>
3000b160:	1a84      	subs	r4, r0, r2
3000b162:	eb68 0303 	sbc.w	r3, r8, r3
3000b166:	2001      	movs	r0, #1
3000b168:	469e      	mov	lr, r3
3000b16a:	2e00      	cmp	r6, #0
3000b16c:	d068      	beq.n	3000b240 <__udivmoddi4+0x2a4>
3000b16e:	e9c6 4e00 	strd	r4, lr, [r6]
3000b172:	2100      	movs	r1, #0
3000b174:	e762      	b.n	3000b03c <__udivmoddi4+0xa0>
3000b176:	4660      	mov	r0, ip
3000b178:	e7f7      	b.n	3000b16a <__udivmoddi4+0x1ce>
3000b17a:	f1cc 0e20 	rsb	lr, ip, #32
3000b17e:	fa03 f30c 	lsl.w	r3, r3, ip
3000b182:	fa02 f50c 	lsl.w	r5, r2, ip
3000b186:	fa00 f70c 	lsl.w	r7, r0, ip
3000b18a:	fa22 f40e 	lsr.w	r4, r2, lr
3000b18e:	fa28 fb0e 	lsr.w	fp, r8, lr
3000b192:	fa20 f10e 	lsr.w	r1, r0, lr
3000b196:	431c      	orrs	r4, r3
3000b198:	fa08 f30c 	lsl.w	r3, r8, ip
3000b19c:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000b1a0:	4319      	orrs	r1, r3
3000b1a2:	b2a3      	uxth	r3, r4
3000b1a4:	0c0a      	lsrs	r2, r1, #16
3000b1a6:	fbbb f8f9 	udiv	r8, fp, r9
3000b1aa:	fb09 bb18 	mls	fp, r9, r8, fp
3000b1ae:	fb08 fa03 	mul.w	sl, r8, r3
3000b1b2:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000b1b6:	4592      	cmp	sl, r2
3000b1b8:	d93e      	bls.n	3000b238 <__udivmoddi4+0x29c>
3000b1ba:	18a2      	adds	r2, r4, r2
3000b1bc:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000b1c0:	d204      	bcs.n	3000b1cc <__udivmoddi4+0x230>
3000b1c2:	4592      	cmp	sl, r2
3000b1c4:	d902      	bls.n	3000b1cc <__udivmoddi4+0x230>
3000b1c6:	f1a8 0002 	sub.w	r0, r8, #2
3000b1ca:	4422      	add	r2, r4
3000b1cc:	eba2 020a 	sub.w	r2, r2, sl
3000b1d0:	b289      	uxth	r1, r1
3000b1d2:	fbb2 f8f9 	udiv	r8, r2, r9
3000b1d6:	fb09 2218 	mls	r2, r9, r8, r2
3000b1da:	fb08 f303 	mul.w	r3, r8, r3
3000b1de:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000b1e2:	4293      	cmp	r3, r2
3000b1e4:	d92a      	bls.n	3000b23c <__udivmoddi4+0x2a0>
3000b1e6:	18a2      	adds	r2, r4, r2
3000b1e8:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000b1ec:	d204      	bcs.n	3000b1f8 <__udivmoddi4+0x25c>
3000b1ee:	4293      	cmp	r3, r2
3000b1f0:	d902      	bls.n	3000b1f8 <__udivmoddi4+0x25c>
3000b1f2:	f1a8 0102 	sub.w	r1, r8, #2
3000b1f6:	4422      	add	r2, r4
3000b1f8:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000b1fc:	1ad2      	subs	r2, r2, r3
3000b1fe:	fba0 9805 	umull	r9, r8, r0, r5
3000b202:	4542      	cmp	r2, r8
3000b204:	464b      	mov	r3, r9
3000b206:	4641      	mov	r1, r8
3000b208:	d302      	bcc.n	3000b210 <__udivmoddi4+0x274>
3000b20a:	d106      	bne.n	3000b21a <__udivmoddi4+0x27e>
3000b20c:	454f      	cmp	r7, r9
3000b20e:	d204      	bcs.n	3000b21a <__udivmoddi4+0x27e>
3000b210:	3801      	subs	r0, #1
3000b212:	ebb9 0305 	subs.w	r3, r9, r5
3000b216:	eb68 0104 	sbc.w	r1, r8, r4
3000b21a:	b18e      	cbz	r6, 3000b240 <__udivmoddi4+0x2a4>
3000b21c:	1afc      	subs	r4, r7, r3
3000b21e:	eb62 0701 	sbc.w	r7, r2, r1
3000b222:	fa07 fe0e 	lsl.w	lr, r7, lr
3000b226:	fa24 f30c 	lsr.w	r3, r4, ip
3000b22a:	fa27 f70c 	lsr.w	r7, r7, ip
3000b22e:	ea4e 0303 	orr.w	r3, lr, r3
3000b232:	e9c6 3700 	strd	r3, r7, [r6]
3000b236:	e79c      	b.n	3000b172 <__udivmoddi4+0x1d6>
3000b238:	4640      	mov	r0, r8
3000b23a:	e7c7      	b.n	3000b1cc <__udivmoddi4+0x230>
3000b23c:	4641      	mov	r1, r8
3000b23e:	e7db      	b.n	3000b1f8 <__udivmoddi4+0x25c>
3000b240:	4631      	mov	r1, r6
3000b242:	e6fb      	b.n	3000b03c <__udivmoddi4+0xa0>

3000b244 <__aeabi_idiv0>:
3000b244:	4770      	bx	lr
3000b246:	bf00      	nop

3000b248 <_mbtowc_r>:
3000b248:	b430      	push	{r4, r5}
3000b24a:	4c06      	ldr	r4, [pc, #24]	; (3000b264 <_mbtowc_r+0x1c>)
3000b24c:	4d06      	ldr	r5, [pc, #24]	; (3000b268 <_mbtowc_r+0x20>)
3000b24e:	6824      	ldr	r4, [r4, #0]
3000b250:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b252:	2c00      	cmp	r4, #0
3000b254:	bf08      	it	eq
3000b256:	462c      	moveq	r4, r5
3000b258:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000b25c:	46a4      	mov	ip, r4
3000b25e:	bc30      	pop	{r4, r5}
3000b260:	4760      	bx	ip
3000b262:	bf00      	nop
3000b264:	3000e708 	.word	0x3000e708
3000b268:	3000e59c 	.word	0x3000e59c

3000b26c <__ascii_mbtowc>:
3000b26c:	b082      	sub	sp, #8
3000b26e:	b901      	cbnz	r1, 3000b272 <__ascii_mbtowc+0x6>
3000b270:	a901      	add	r1, sp, #4
3000b272:	b142      	cbz	r2, 3000b286 <__ascii_mbtowc+0x1a>
3000b274:	b14b      	cbz	r3, 3000b28a <__ascii_mbtowc+0x1e>
3000b276:	7813      	ldrb	r3, [r2, #0]
3000b278:	600b      	str	r3, [r1, #0]
3000b27a:	7812      	ldrb	r2, [r2, #0]
3000b27c:	1e10      	subs	r0, r2, #0
3000b27e:	bf18      	it	ne
3000b280:	2001      	movne	r0, #1
3000b282:	b002      	add	sp, #8
3000b284:	4770      	bx	lr
3000b286:	4610      	mov	r0, r2
3000b288:	e7fb      	b.n	3000b282 <__ascii_mbtowc+0x16>
3000b28a:	f06f 0001 	mvn.w	r0, #1
3000b28e:	e7f8      	b.n	3000b282 <__ascii_mbtowc+0x16>

3000b290 <_wctomb_r>:
3000b290:	b430      	push	{r4, r5}
3000b292:	4c06      	ldr	r4, [pc, #24]	; (3000b2ac <_wctomb_r+0x1c>)
3000b294:	4d06      	ldr	r5, [pc, #24]	; (3000b2b0 <_wctomb_r+0x20>)
3000b296:	6824      	ldr	r4, [r4, #0]
3000b298:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000b29a:	2c00      	cmp	r4, #0
3000b29c:	bf08      	it	eq
3000b29e:	462c      	moveq	r4, r5
3000b2a0:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000b2a4:	46a4      	mov	ip, r4
3000b2a6:	bc30      	pop	{r4, r5}
3000b2a8:	4760      	bx	ip
3000b2aa:	bf00      	nop
3000b2ac:	3000e708 	.word	0x3000e708
3000b2b0:	3000e59c 	.word	0x3000e59c

3000b2b4 <__ascii_wctomb>:
3000b2b4:	b149      	cbz	r1, 3000b2ca <__ascii_wctomb+0x16>
3000b2b6:	2aff      	cmp	r2, #255	; 0xff
3000b2b8:	bf8d      	iteet	hi
3000b2ba:	238a      	movhi	r3, #138	; 0x8a
3000b2bc:	2001      	movls	r0, #1
3000b2be:	700a      	strbls	r2, [r1, #0]
3000b2c0:	6003      	strhi	r3, [r0, #0]
3000b2c2:	bf88      	it	hi
3000b2c4:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000b2c8:	4770      	bx	lr
3000b2ca:	4608      	mov	r0, r1
3000b2cc:	4770      	bx	lr
3000b2ce:	bf00      	nop

3000b2d0 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000b2d0:	b401      	push	{r0}
3000b2d2:	4802      	ldr	r0, [pc, #8]	; (3000b2dc <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000b2d4:	4684      	mov	ip, r0
3000b2d6:	bc01      	pop	{r0}
3000b2d8:	4760      	bx	ip
3000b2da:	bf00      	nop
3000b2dc:	00005b4d 	.word	0x00005b4d

3000b2e0 <__SBOOT_Validate_Signature_B_veneer>:
3000b2e0:	b401      	push	{r0}
3000b2e2:	4802      	ldr	r0, [pc, #8]	; (3000b2ec <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000b2e4:	4684      	mov	ip, r0
3000b2e6:	bc01      	pop	{r0}
3000b2e8:	4760      	bx	ip
3000b2ea:	bf00      	nop
3000b2ec:	00003431 	.word	0x00003431

3000b2f0 <__SBOOT_Validate_PubKey_B_veneer>:
3000b2f0:	b401      	push	{r0}
3000b2f2:	4802      	ldr	r0, [pc, #8]	; (3000b2fc <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000b2f4:	4684      	mov	ip, r0
3000b2f6:	bc01      	pop	{r0}
3000b2f8:	4760      	bx	ip
3000b2fa:	bf00      	nop
3000b2fc:	00003495 	.word	0x00003495

3000b300 <__rtl_crypto_sha2_update_B_veneer>:
3000b300:	b401      	push	{r0}
3000b302:	4802      	ldr	r0, [pc, #8]	; (3000b30c <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000b304:	4684      	mov	ip, r0
3000b306:	bc01      	pop	{r0}
3000b308:	4760      	bx	ip
3000b30a:	bf00      	nop
3000b30c:	00005385 	.word	0x00005385

3000b310 <__INT_HardFault_C_veneer>:
3000b310:	b401      	push	{r0}
3000b312:	4802      	ldr	r0, [pc, #8]	; (3000b31c <__INT_HardFault_C_veneer+0xc>)
3000b314:	4684      	mov	ip, r0
3000b316:	bc01      	pop	{r0}
3000b318:	4760      	bx	ip
3000b31a:	bf00      	nop
3000b31c:	0000d8ed 	.word	0x0000d8ed

3000b320 <__SBOOT_Validate_ImgHash_A_veneer>:
3000b320:	b401      	push	{r0}
3000b322:	4802      	ldr	r0, [pc, #8]	; (3000b32c <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000b324:	4684      	mov	ip, r0
3000b326:	bc01      	pop	{r0}
3000b328:	4760      	bx	ip
3000b32a:	bf00      	nop
3000b32c:	000033cd 	.word	0x000033cd

3000b330 <__DiagPrintf_veneer>:
3000b330:	b401      	push	{r0}
3000b332:	4802      	ldr	r0, [pc, #8]	; (3000b33c <__DiagPrintf_veneer+0xc>)
3000b334:	4684      	mov	ip, r0
3000b336:	bc01      	pop	{r0}
3000b338:	4760      	bx	ip
3000b33a:	bf00      	nop
3000b33c:	0000e5f9 	.word	0x0000e5f9

3000b340 <__SBOOT_Validate_ImgHash_B_veneer>:
3000b340:	b401      	push	{r0}
3000b342:	4802      	ldr	r0, [pc, #8]	; (3000b34c <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000b344:	4684      	mov	ip, r0
3000b346:	bc01      	pop	{r0}
3000b348:	4760      	bx	ip
3000b34a:	bf00      	nop
3000b34c:	00003541 	.word	0x00003541

3000b350 <__io_assert_failed_veneer>:
3000b350:	b401      	push	{r0}
3000b352:	4802      	ldr	r0, [pc, #8]	; (3000b35c <__io_assert_failed_veneer+0xc>)
3000b354:	4684      	mov	ip, r0
3000b356:	bc01      	pop	{r0}
3000b358:	4760      	bx	ip
3000b35a:	bf00      	nop
3000b35c:	0000992d 	.word	0x0000992d

3000b360 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000b360:	b401      	push	{r0}
3000b362:	4802      	ldr	r0, [pc, #8]	; (3000b36c <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000b364:	4684      	mov	ip, r0
3000b366:	bc01      	pop	{r0}
3000b368:	4760      	bx	ip
3000b36a:	bf00      	nop
3000b36c:	00005a45 	.word	0x00005a45

3000b370 <____wrap_memset_veneer>:
3000b370:	b401      	push	{r0}
3000b372:	4802      	ldr	r0, [pc, #8]	; (3000b37c <____wrap_memset_veneer+0xc>)
3000b374:	4684      	mov	ip, r0
3000b376:	bc01      	pop	{r0}
3000b378:	4760      	bx	ip
3000b37a:	bf00      	nop
3000b37c:	00012efd 	.word	0x00012efd

3000b380 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000b380:	b401      	push	{r0}
3000b382:	4802      	ldr	r0, [pc, #8]	; (3000b38c <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000b384:	4684      	mov	ip, r0
3000b386:	bc01      	pop	{r0}
3000b388:	4760      	bx	ip
3000b38a:	bf00      	nop
3000b38c:	000059d9 	.word	0x000059d9

3000b390 <__DiagVSprintf_veneer>:
3000b390:	b401      	push	{r0}
3000b392:	4802      	ldr	r0, [pc, #8]	; (3000b39c <__DiagVSprintf_veneer+0xc>)
3000b394:	4684      	mov	ip, r0
3000b396:	bc01      	pop	{r0}
3000b398:	4760      	bx	ip
3000b39a:	bf00      	nop
3000b39c:	0000e331 	.word	0x0000e331

3000b3a0 <__SBOOT_Validate_Signature_A_veneer>:
3000b3a0:	b401      	push	{r0}
3000b3a2:	4802      	ldr	r0, [pc, #8]	; (3000b3ac <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000b3a4:	4684      	mov	ip, r0
3000b3a6:	bc01      	pop	{r0}
3000b3a8:	4760      	bx	ip
3000b3aa:	bf00      	nop
3000b3ac:	000032bd 	.word	0x000032bd

3000b3b0 <____wrap_memcpy_veneer>:
3000b3b0:	b401      	push	{r0}
3000b3b2:	4802      	ldr	r0, [pc, #8]	; (3000b3bc <____wrap_memcpy_veneer+0xc>)
3000b3b4:	4684      	mov	ip, r0
3000b3b6:	bc01      	pop	{r0}
3000b3b8:	4760      	bx	ip
3000b3ba:	bf00      	nop
3000b3bc:	00012f05 	.word	0x00012f05

3000b3c0 <__SBOOT_Validate_Algorithm_A_veneer>:
3000b3c0:	b401      	push	{r0}
3000b3c2:	4802      	ldr	r0, [pc, #8]	; (3000b3cc <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000b3c4:	4684      	mov	ip, r0
3000b3c6:	bc01      	pop	{r0}
3000b3c8:	4760      	bx	ip
3000b3ca:	bf00      	nop
3000b3cc:	00003279 	.word	0x00003279

3000b3d0 <__SBOOT_Validate_PubKey_A_veneer>:
3000b3d0:	b401      	push	{r0}
3000b3d2:	4802      	ldr	r0, [pc, #8]	; (3000b3dc <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000b3d4:	4684      	mov	ip, r0
3000b3d6:	bc01      	pop	{r0}
3000b3d8:	4760      	bx	ip
3000b3da:	bf00      	nop
3000b3dc:	00003321 	.word	0x00003321

3000b3e0 <__rtl_crypto_sha2_final_A_veneer>:
3000b3e0:	b401      	push	{r0}
3000b3e2:	4802      	ldr	r0, [pc, #8]	; (3000b3ec <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000b3e4:	4684      	mov	ip, r0
3000b3e6:	bc01      	pop	{r0}
3000b3e8:	4760      	bx	ip
3000b3ea:	bf00      	nop
3000b3ec:	0000527d 	.word	0x0000527d

3000b3f0 <__SBOOT_GetMdType_A_veneer>:
3000b3f0:	b401      	push	{r0}
3000b3f2:	4802      	ldr	r0, [pc, #8]	; (3000b3fc <__SBOOT_GetMdType_A_veneer+0xc>)
3000b3f4:	4684      	mov	ip, r0
3000b3f6:	bc01      	pop	{r0}
3000b3f8:	4760      	bx	ip
3000b3fa:	bf00      	nop
3000b3fc:	00003029 	.word	0x00003029

3000b400 <__CRYPTO_SHA_Init_A_veneer>:
3000b400:	b401      	push	{r0}
3000b402:	4802      	ldr	r0, [pc, #8]	; (3000b40c <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000b404:	4684      	mov	ip, r0
3000b406:	bc01      	pop	{r0}
3000b408:	4760      	bx	ip
3000b40a:	bf00      	nop
3000b40c:	000022dd 	.word	0x000022dd

3000b410 <__rtl_crypto_sha2_init_B_veneer>:
3000b410:	b401      	push	{r0}
3000b412:	4802      	ldr	r0, [pc, #8]	; (3000b41c <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000b414:	4684      	mov	ip, r0
3000b416:	bc01      	pop	{r0}
3000b418:	4760      	bx	ip
3000b41a:	bf00      	nop
3000b41c:	00004ffd 	.word	0x00004ffd

3000b420 <__rtl_crypto_sha2_final_B_veneer>:
3000b420:	b401      	push	{r0}
3000b422:	4802      	ldr	r0, [pc, #8]	; (3000b42c <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000b424:	4684      	mov	ip, r0
3000b426:	bc01      	pop	{r0}
3000b428:	4760      	bx	ip
3000b42a:	bf00      	nop
3000b42c:	000053f1 	.word	0x000053f1

3000b430 <__SBOOT_GetMdType_B_veneer>:
3000b430:	b401      	push	{r0}
3000b432:	4802      	ldr	r0, [pc, #8]	; (3000b43c <__SBOOT_GetMdType_B_veneer+0xc>)
3000b434:	4684      	mov	ip, r0
3000b436:	bc01      	pop	{r0}
3000b438:	4760      	bx	ip
3000b43a:	bf00      	nop
3000b43c:	0000319d 	.word	0x0000319d

3000b440 <____wrap_memcmp_veneer>:
3000b440:	b401      	push	{r0}
3000b442:	4802      	ldr	r0, [pc, #8]	; (3000b44c <____wrap_memcmp_veneer+0xc>)
3000b444:	4684      	mov	ip, r0
3000b446:	bc01      	pop	{r0}
3000b448:	4760      	bx	ip
3000b44a:	bf00      	nop
3000b44c:	00012f0d 	.word	0x00012f0d

3000b450 <__SBOOT_Validate_Algorithm_B_veneer>:
3000b450:	b401      	push	{r0}
3000b452:	4802      	ldr	r0, [pc, #8]	; (3000b45c <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000b454:	4684      	mov	ip, r0
3000b456:	bc01      	pop	{r0}
3000b458:	4760      	bx	ip
3000b45a:	bf00      	nop
3000b45c:	000033ed 	.word	0x000033ed

3000b460 <____wrap_strlen_veneer>:
3000b460:	b401      	push	{r0}
3000b462:	4802      	ldr	r0, [pc, #8]	; (3000b46c <____wrap_strlen_veneer+0xc>)
3000b464:	4684      	mov	ip, r0
3000b466:	bc01      	pop	{r0}
3000b468:	4760      	bx	ip
3000b46a:	bf00      	nop
3000b46c:	00012f25 	.word	0x00012f25

3000b470 <__rtl_crypto_sha2_update_A_veneer>:
3000b470:	b401      	push	{r0}
3000b472:	4802      	ldr	r0, [pc, #8]	; (3000b47c <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000b474:	4684      	mov	ip, r0
3000b476:	bc01      	pop	{r0}
3000b478:	4760      	bx	ip
3000b47a:	bf00      	nop
3000b47c:	00005211 	.word	0x00005211

3000b480 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000b480:	b401      	push	{r0}
3000b482:	4802      	ldr	r0, [pc, #8]	; (3000b48c <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000b484:	4684      	mov	ip, r0
3000b486:	bc01      	pop	{r0}
3000b488:	4760      	bx	ip
3000b48a:	bf00      	nop
3000b48c:	00002369 	.word	0x00002369

3000b490 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000b490:	b401      	push	{r0}
3000b492:	4802      	ldr	r0, [pc, #8]	; (3000b49c <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000b494:	4684      	mov	ip, r0
3000b496:	bc01      	pop	{r0}
3000b498:	4760      	bx	ip
3000b49a:	bf00      	nop
3000b49c:	000024dd 	.word	0x000024dd

3000b4a0 <__CRYPTO_SHA_Init_B_veneer>:
3000b4a0:	b401      	push	{r0}
3000b4a2:	4802      	ldr	r0, [pc, #8]	; (3000b4ac <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000b4a4:	4684      	mov	ip, r0
3000b4a6:	bc01      	pop	{r0}
3000b4a8:	4760      	bx	ip
3000b4aa:	bf00      	nop
3000b4ac:	00002451 	.word	0x00002451

3000b4b0 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000b4b0:	b401      	push	{r0}
3000b4b2:	4802      	ldr	r0, [pc, #8]	; (3000b4bc <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000b4b4:	4684      	mov	ip, r0
3000b4b6:	bc01      	pop	{r0}
3000b4b8:	4760      	bx	ip
3000b4ba:	bf00      	nop
3000b4bc:	00005bb9 	.word	0x00005bb9

3000b4c0 <__rtl_crypto_sha2_init_A_veneer>:
3000b4c0:	b401      	push	{r0}
3000b4c2:	4802      	ldr	r0, [pc, #8]	; (3000b4cc <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000b4c4:	4684      	mov	ip, r0
3000b4c6:	bc01      	pop	{r0}
3000b4c8:	4760      	bx	ip
3000b4ca:	bf00      	nop
3000b4cc:	00004e89 	.word	0x00004e89
3000b4d0:	50494843 	.word	0x50494843
3000b4d4:	4f464e49 	.word	0x4f464e49
3000b4d8:	73255b00 	.word	0x73255b00
3000b4dc:	205d452d 	.word	0x205d452d
3000b4e0:	61766e49 	.word	0x61766e49
3000b4e4:	2064696c 	.word	0x2064696c
3000b4e8:	6e696843 	.word	0x6e696843
3000b4ec:	6f666e69 	.word	0x6f666e69
3000b4f0:	68432021 	.word	0x68432021
3000b4f4:	206b6365 	.word	0x206b6365
3000b4f8:	0a50544f 	.word	0x0a50544f
3000b4fc:	00          	.byte	0x00
3000b4fd:	5b          	.byte	0x5b
3000b4fe:	7325      	.short	0x7325
3000b500:	205d572d 	.word	0x205d572d
3000b504:	61656c50 	.word	0x61656c50
3000b508:	70206573 	.word	0x70206573
3000b50c:	72676f72 	.word	0x72676f72
3000b510:	63206d61 	.word	0x63206d61
3000b514:	69706968 	.word	0x69706968
3000b518:	206f666e 	.word	0x206f666e
3000b51c:	4f206e69 	.word	0x4f206e69
3000b520:	21205054 	.word	0x21205054
3000b524:	000a      	.short	0x000a

3000b526 <Memory_Info>:
3000b526:	0007 03e0 0004 0006 02c0 0102 0100 0201     ................
3000b536:	0102 0200 0302 0004 0400 0204 0102 0101     ................
3000b546:	0221 0102 0401 0224 0102 0300 0103 0003     !.....$.........
3000b556:	0500 0105 0002 0600 0206 0102 0700 0107     ................
3000b566:	0002 0800 0208 0102 0801 0228 0102 0900     ..........(.....
3000b576:	0309 0004 0901 0329 0004 0a00 010a 0003     ......).........
3000b586:	0a01 012a 0003 0b00 020b 0102 0c00 020c     ..*.............
3000b596:	0102 0d00 030d 0004 0e00 010e 0003 ffff     ................
3000b5a6:	ffff ffff 4444 5052 5948 5b00 7325 452d     ....DDRPHY.[%s-E
3000b5b6:	205d 5244 4d41 6920 2073 6f6e 2074 6143     ] DRAM is not Ca
3000b5c6:	696c 7262 6961 6e6f 0a0d 0000 0000          libraion......

3000b5d4 <ddrphy_tx_scan>:
3000b5d4:	0000 0000 0000 0008 0808 0008 0000 0000     ................
3000b5e4:	0000 2000 3333 3333 0022 000c 0000 0000     ... 3333".......
3000b5f4:	0012 0706 0707 000f 0f0f 080f 0008 0000     ................
3000b604:	0001 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b614:	0016 0b0b 0b0b 0013 1313 0e13 000e 0000     ................
3000b624:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b634:	0018 0e0e 0e0e 0017 1717 0e17 000e 0000     ................
3000b644:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b654:	0018 0a0a 0a0a 0013 1313 0e13 000e 0000     ................
3000b664:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b674:	001a 0e0e 0e0e 0017 1717 1217 0012 0000     ................
3000b684:	0fc1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b694:	0016 0f0f 0f0f 0018 1818 0e18 000e 0000     ................
3000b6a4:	03c1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b6b4:	001b 0f0f 0f0f 0018 1818 1318 0013 0000     ................
3000b6c4:	0fc1 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b6d4:	001b 1110 1111 0019 1919 1319 0013 0000     ................
3000b6e4:	0ffd 2000 2222 2222 0012 000c 0000 3400     ... """".......4
3000b6f4:	001c 1313 1313 001c 1c1c 121c 0012 0000     ................
3000b704:	0ffd 2000 2222 2222 0012 000c 0000 3400     ... """".......4

3000b714 <ddrphy_zq_rx_scan>:
	...
3000b71c:	2626 2626 2626 2626 030f 0303 1414 0e14     &&&&&&&&........
3000b72c:	0e0e 140e 362e 261f 3029 261c 020f 0302     .....6.&)0.&....
3000b73c:	1414 0e14 0e0e 140e 362e 261f 3029 261c     .........6.&)0.&
3000b74c:	020f 0302 0b0b 0a0c 0a0a 0b0b 2721 2617     ............!'.&
3000b75c:	231e 2615 0206 0402 0c0c 0a0e 0a0a 0c0b     .#.&............
3000b76c:	362c 261e 2620 2616 0203 0402 1414 0e14     ,6.& &.&........
3000b77c:	0e0e 140e 3c34 2624 3029 261c 030f 0303     ....4<$&)0.&....
3000b78c:	0a0a 090b 0909 0a0a 231e 2615 201b 2613     .........#.&. .&
3000b79c:	0204 0402 0c0c 0b0f 0b0b 0c0b 3a30 2623     ............0:#&
3000b7ac:	2722 2618 0205 0402 1414 0e14 0e0e 140e     "'.&............
3000b7bc:	3c34 2624 2f28 261b 030f 0303 0c0c 0a0c     4<$&(/.&........
3000b7cc:	0a0a 0c0a 251f 2616 221d 2614 0205 0402     .....%.&.".&....

3000b7dc <Flash_AVL>:
3000b7dc:	00ef 0000 00ff 0000 0000 0000 43fc 0000     .............C..
3000b7ec:	0000 0000 00a1 0000 00ff 0000 0000 0000     ................
3000b7fc:	fffc 0000 0000 0000 000b 0000 00ff 0000     ................
3000b80c:	0000 0000 43fc 0000 0000 0000 000e 0000     .....C..........
3000b81c:	00ff 0000 0000 0000 43fc 0000 0000 0000     .........C......
3000b82c:	00c8 0000 00ff 0000 0001 0000 43fc 0000     .............C..
3000b83c:	0000 0000 28c2 0000 ffff 0000 0005 0000     .....(..........
3000b84c:	00fc 0002 0000 0000 00c2 0000 00ff 0000     ................
3000b85c:	0002 0000 00fc 0000 0000 0000 0068 0000     ............h...
3000b86c:	00ff 0000 0002 0000 00fc 0000 0000 0000     ................
3000b87c:	0051 0000 00ff 0000 0002 0000 00fc 0000     Q...............
3000b88c:	0000 0000 001c 0000 00ff 0000 0003 0000     ................
3000b89c:	00fc 0000 0000 0000 0020 0000 00ff 0000     ........ .......
3000b8ac:	0004 0000 00fc 0000 0000 0000 0000 0000     ................
3000b8bc:	00ff 0000 00fe 0000 ffff ffff 7971 3000     ............qy.0
3000b8cc:	00ff 0000 ffff ffff 00ff 0000 ffff ffff     ................
3000b8dc:	0000 0000                                   ....

3000b8e0 <Flash_ReadMode>:
3000b8e0:	ffff                                        ..

3000b8e2 <Flash_Speed>:
3000b8e2:	07ff                                        ..

3000b8e4 <NAND_AVL>:
3000b8e4:	00ef 0000 00ff 0000 0000 0000 ffff ffff     ................
3000b8f4:	0000 0000 00c8 0000 00ff 0000 0001 0000     ................
3000b904:	ffff ffff 0000 0000 00e5 0000 00ff 0000     ................
3000b914:	0001 0000 ffff ffff 0000 0000 00c2 0000     ................
3000b924:	00ff 0000 0002 0000 ffff ffff 0000 0000     ................
3000b934:	002c 0000 00ff 0000 0004 0000 ffff ffff     ,...............
	...
3000b94c:	00ff 0000 00fe 0000 ffff ffff 79cd 3000     .............y.0
3000b95c:	00ff 0000 ffff ffff 00ff 0000 ffff ffff     ................
3000b96c:	0000 0000 4931 004f 4f32 3200 4f49 3400     ....1IO.2O.2IO.4
3000b97c:	004f 4934 004f 4c46 5341 4348 4b4c 5b00     O.4IO.FLASHCLK.[
3000b98c:	7325 492d 205d 6163 696c 7262 7461 6f69     %s-I] calibratio
3000b99c:	5f6e 6b6f 5b3a 6425 253a 3a64 6425 205d     n_ok:[%d:%d:%d] 
3000b9ac:	000a 255b 2d73 5d49 4620 414c 4853 4320     ..[%s-I] FLASH C
3000b9bc:	4c41 4249 305b 2578 2078 4b4f 0a5d 5b00     ALIB[0x%x OK]..[
3000b9cc:	7325 452d 205d 4c46 5341 2048 4143 494c     %s-E] FLASH CALI
3000b9dc:	5b42 7830 7825 4620 4941 5d4c 000a 255b     B[0x%x FAIL]..[%
3000b9ec:	2d73 5d49 4620 616c 6873 5220 6165 2064     s-I] Flash Read 
3000b9fc:	7325 000a 255b 2d73 5d45 4620 616c 6873     %s..[%s-E] Flash
3000ba0c:	5320 6977 6374 2068 6552 6461 4d20 646f      Switch Read Mod
3000ba1c:	2065 4146 4c49 000a 255b 2d73 5d49 4620     e FAIL..[%s-I] F
3000ba2c:	616c 6873 4920 3a44 2520 2d78 7825 252d     lash ID: %x-%x-%
3000ba3c:	0a78 5b00 7325 572d 205d 6854 7369 6620     x..[%s-W] This f
3000ba4c:	616c 6873 7420 7079 2065 7369 6e20 746f     lash type is not
3000ba5c:	7320 7075 6f70 7472 6465 0a21 5b00 7325      supported!..[%s
3000ba6c:	492d 205d 414e 444e 4920 3a44 2520 2d78     -I] NAND ID: %x-
3000ba7c:	7825 000a b970 3000 b974 3000 b977 3000     %x..p..0t..0w..0
3000ba8c:	b97b 3000 b97e 3000                         {..0~..0

3000ba94 <__FUNCTION__.0>:
3000ba94:	616e 646e 675f 7465 765f 6e65 6f64 0072     nand_get_vendor.

3000baa4 <__FUNCTION__.1>:
3000baa4:	6c66 7361 5f68 6567 5f74 6576 646e 726f     flash_get_vendor
3000bab4:	4e00 4e41 4644 414c 4853 5b00 7325 452d     .NANDFLASH.[%s-E
3000bac4:	205d 6c42 206b 7830 7825 6920 2073 4242     ] Blk 0x%x is BB
3000bad4:	0a21 5b00 7325 492d 205d 6552 6461 5020     !..[%s-I] Read P
3000bae4:	6761 2065 7830 7825 6f20 2066 6c42 636f     age 0x%x of Bloc
3000baf4:	206b 7830 7825 4d20 7961 4620 6961 2c6c     k 0x%x May Fail,
3000bb04:	7320 6174 7574 2073 7830 7825 0a21 4f00      status 0x%x!..O
3000bb14:	5054 5b00 7325 452d 205d 544f 5f50 6552     TP.[%s-E] OTP_Re
3000bb24:	6461 2038 6146 6c69 2520 2078 000a 255b     ad8 Fail %x ..[%
3000bb34:	2d73 5d45 5020 524d 5220 6165 2064 7265     s-E] PMR Read er
3000bb44:	6f72 2172 000a 255b 2d73 5d57 4f20 5054     ror!..[%s-W] OTP
3000bb54:	575f 6972 6574 2038 6146 6c69 2520 2078     _Write8 Fail %x 
3000bb64:	000a 255b 2d73 5d45 4f20 5054 7020 6f72     ..[%s-E] OTP pro
3000bb74:	7267 6d61 6520 7272 726f 0a21 5b00 7325     gram error!..[%s
3000bb84:	452d 205d 4d50 3252 5220 6165 2064 7265     -E] PMR2 Read er
3000bb94:	6f72 2172 000a                              ror!..

3000bb9a <__FUNCTION__.1>:
3000bb9a:	4c50 5f4c 5041 435f 6b6c 6553 0074 5350     PLL_AP_ClkSet.PS
3000bbaa:	4152 004d 255b 2d73 5d49 5020 5253 4d41     RAM.[%s-I] PSRAM
3000bbba:	4320 7274 206c 4c43 3a4b 2520 2064 7a48      Ctrl CLK: %d Hz
3000bbca:	0a20 5b00 7325 492d 205d 6143 4e6c 696d      ..[%s-I] CalNmi
3000bbda:	206e 203d 7825 4320 6c61 6d4e 7861 3d20     n = %x CalNmax =
3000bbea:	2520 2078 6957 646e 776f 6953 657a 3d20      %x WindowSize =
3000bbfa:	2520 2078 6870 7361 3a65 2520 2078 000a      %x phase: %x ..
3000bc0a:	4f52 504d 5441 4843 5b00 7325 452d 205d     ROMPATCH.[%s-E] 
3000bc1a:	6f4e 7420 6968 2073 7561 6874 745f 7079     No this auth_typ
3000bc2a:	2865 6425 2029 6f66 2072 4853 3241 000a     e(%d) for SHA2..
3000bc3a:	255b 2d73 5d57 6920 7172 255b 5d64 7020     [%s-W] irq[%d] p
3000bc4a:	6972 726f 7469 2079 6425 7320 6168 6c6c     riority %d shall
3000bc5a:	3c20 203d 6425 000a                          <= %d..

3000bc62 <__FUNCTION__.0>:
3000bc62:	7269 5f71 6573 5f74 7270 6f69 6972 7974     irq_set_priority
	...

3000bc73 <__FUNCTION__.1>:
3000bc73:	5352 5049 4f5f 4654 455f 616e 6c62 0065     RSIP_OTF_Enable.

3000bc83 <__FUNCTION__.3>:
3000bc83:	7472 5f6c 7263 7079 6f74 685f 616d 5f63     rtl_crypto_hmac_
3000bc93:	6873 3261 695f 696e 0074                    sha2_init.

3000bc9d <__FUNCTION__.4>:
3000bc9d:	5243 5059 4f54 535f 7465 6553 7563 6972     CRYPTO_SetSecuri
3000bcad:	7974 6f4d 6564 4441 4400 5244 5b00 7325     tyModeAD.DDR.[%s
3000bcbd:	452d 205d 4444 2052 6953 657a 6920 2073     -E] DDR Size is 
3000bccd:	6f6e 2074 6f63 666e 6769 7275 6465 000a     not configured..
3000bcdd:	255b 2d73 5d45 3e20 5b20 4441 5244 5220     [%s-E] > [ADDR R
3000bced:	4d45 5041 4520 5252 524f 3a5d 6320 6c6f     EMAP ERROR]: col
3000bcfd:	5f75 6572 616d 5f70 6962 7374 2120 203d     u_remap_bits != 
3000bd0d:	7264 6d61 635f 6c6f 5f75 6962 7374 2121     dram_colu_bits!!
3000bd1d:	0a21 5b00 7325 452d 205d 203e 415b 4444     !..[%s-E] > [ADD
3000bd2d:	2052 4552 414d 2050 5245 4f52 5d52 203a     R REMAP ERROR]: 
3000bd3d:	6162 6b6e 725f 6d65 7061 625f 7469 2073     bank_remap_bits 
3000bd4d:	3d21 6420 6172 5f6d 6162 6b6e 625f 7469     != dram_bank_bit
3000bd5d:	2173 2121 000a                               s!!!...

3000bd64 <SWR_IDX_FLAG>:
3000bd64:	07e0 0000 07e4 0000 07e5 0000 07e6 0000     ................
3000bd74:	07e7 0000                                   ....

3000bd78 <__FUNCTION__.1>:
3000bd78:	5753 5f52 454d 5f4d 6f4d 6564 535f 7465     SWR_MEM_Mode_Set
3000bd88:	4b00 344d 4b00 304d 3d00 3d3d 3d3d 3d3d     .KM4.KM0.=======
3000bd98:	3d3d 253d 2073 7453 6361 206b 7544 706d     ===%s Stack Dump
3000bda8:	3d20 3d3d 3d3d 3d3d 3d3d 0a3d 4300 7275      ==========..Cur
3000bdb8:	6572 746e 5320 6174 6b63 5020 696f 746e     rent Stack Point
3000bdc8:	7265 3d20 3020 2578 2c70 6120 646e 6420     er = 0x%p, and d
3000bdd8:	6d75 2070 7473 6361 206b 6564 7470 2068     ump stack depth 
3000bde8:	203d 6425 0a0d 0d00 5b0a 3025 7838 205d     = %d.....[%08x] 
3000bdf8:	2500 3830 2078 3d00 3d3d 3d3d 3d3d 3d3d     .%08x .=========
3000be08:	203d 7243 7361 2068 7544 706d 3d20 3d3d     = Crash Dump ===
3000be18:	3d3d 3d3d 3d3d 0a3d 3d00 3d3d 3d3d 3d3d     =======..=======
3000be28:	3d3d 203d 6552 6967 7473 7265 4420 6d75     === Register Dum
3000be38:	2070 3d3d 3d3d 3d3d 3d3d 3d3d 000a 205b     p ==========..[ 
3000be48:	4c20 5d52 3020 2578 7838 000a 205b 5020      LR] 0x%8x..[  P
3000be58:	5d43 3020 2578 7838 000a 785b 5350 5d52     C] 0x%8x..[xPSR]
3000be68:	3020 2578 7838 000a 455b 4358 5d52 3020      0x%8x..[EXCR] 0
3000be78:	2578 7838 000a 205b 7325 205d 7830 3825     x%8x..[ %s] 0x%8
3000be88:	0a78 5200 3231 3d00 3d3d 3d3d 3d3d 3d3d     x..R12.=========
3000be98:	203d 6e45 2064 666f 4320 6172 6873 4420     = End of Crash D
3000bea8:	6d75 2070 3d3d 3d3d 3d3d 3d3d 3d3d 000a     ump ==========..
3000beb8:	0a0d 0a0d 5200 0034 3552 5200 0036 3752     .....R4.R5.R6.R7
3000bec8:	5200 0038 3952 5200 3031 5200 3131 5200     .R8.R9.R10.R11.R
3000bed8:	0030 3152 5200 0032 3352 0000               0.R1.R2.R3..

3000bee4 <register_string>:
3000bee4:	0000 0000 bebd 3000 bec0 3000 bec3 3000     .......0...0...0
3000bef4:	bec6 3000 bec9 3000 becc 3000 becf 3000     ...0...0...0...0
3000bf04:	bed3 3000 bed7 3000 beda 3000 bedd 3000     ...0...0...0...0
3000bf14:	bee0 3000 be8b 3000 7845 6563 7470 6f69     ...0...0Exceptio
3000bf24:	206e 6163 6775 7468 6f20 206e 7025 000a     n caught on %p..
3000bf34:	6148 6472 6620 7561 746c 6920 2073 6163     Hard fault is ca
3000bf44:	7375 6465 6220 2079 6166 6c69 6465 7620     used by failed v
3000bf54:	6365 6f74 2072 6566 6374 0068 4648 5253     ector fetch.HFSR
3000bf64:	203a 305b 2578 7838 205d 3e2d 2520 0a73     : [0x%8x] -> %s.
3000bf74:	5300 6365 7275 7469 2079 6166 6c75 2074     .Security fault 
3000bf84:	7369 6320 7561 6573 2064 7962 4920 766e     is caused by Inv
3000bf94:	6c61 6469 6520 746e 7972 7020 696f 746e     alid entry point
3000bfa4:	5300 5346 3a52 5b20 7830 3825 5d78 2d20     .SFSR: [0x%8x] -
3000bfb4:	203e 7325 000a 6553 7563 6972 7974 6620     > %s..Security f
3000bfc4:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000bfd4:	2079 6e49 6176 696c 2064 6e69 6574 7267     y Invalid integr
3000bfe4:	7469 2079 6973 6e67 7461 7275 0065 6553     ity signature.Se
3000bff4:	7563 6972 7974 6620 7561 746c 6920 2073     curity fault is 
3000c004:	6163 7375 6465 6220 2079 6e49 6176 696c     caused by Invali
3000c014:	2064 7865 6563 7470 6f69 206e 6572 7574     d exception retu
3000c024:	6e72 5300 6365 7275 7469 2079 6166 6c75     rn.Security faul
3000c034:	2074 7369 6320 7561 6573 2064 7962 4120     t is caused by A
3000c044:	7474 6972 7562 6974 6e6f 7520 696e 2074     ttribution unit 
3000c054:	6976 6c6f 7461 6f69 006e 6553 7563 6972     violation.Securi
3000c064:	7974 6620 7561 746c 6920 2073 6163 7375     ty fault is caus
3000c074:	6465 6220 2079 6e49 6176 696c 2064 7274     ed by Invalid tr
3000c084:	6e61 6973 6974 6e6f 5300 6365 7275 7469     ansition.Securit
3000c094:	2079 6166 6c75 2074 7369 6320 7561 6573     y fault is cause
3000c0a4:	2064 7962 4c20 7a61 2079 7473 7461 2065     d by Lazy state 
3000c0b4:	7270 7365 7265 6176 6974 6e6f 6520 7272     preservation err
3000c0c4:	726f 5300 6365 7275 7469 2079 6166 6c75     or.Security faul
3000c0d4:	2074 7369 6320 7561 6573 2064 7962 4c20     t is caused by L
3000c0e4:	7a61 2079 7473 7461 2065 7265 6f72 0072     azy state error.
3000c0f4:	6854 2065 6573 7563 6972 7974 6620 7561     The security fau
3000c104:	746c 6f20 6363 7275 6572 2064 6461 7264     lt occurred addr
3000c114:	7365 2073 7369 5300 4146 3a52 5b20 7830     ess is.SFAR: [0x
3000c124:	3825 5d78 2d20 203e 7325 000a 654d 6f6d     %8x] -> %s..Memo
3000c134:	7972 6d20 6e61 6761 6d65 6e65 2074 6166     ry management fa
3000c144:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c154:	6920 736e 7274 6375 6974 6e6f 6120 6363      instruction acc
3000c164:	7365 2073 6976 6c6f 7461 6f69 006e 4d4d     ess violation.MM
3000c174:	5346 3a52 5b20 7830 3825 5d78 2d20 203e     FSR: [0x%8x] -> 
3000c184:	7325 000a 654d 6f6d 7972 6d20 6e61 6761     %s..Memory manag
3000c194:	6d65 6e65 2074 6166 6c75 2074 7369 6320     ement fault is c
3000c1a4:	7561 6573 2064 7962 6420 7461 2061 6361     aused by data ac
3000c1b4:	6563 7373 7620 6f69 616c 6974 6e6f 4d00     cess violation.M
3000c1c4:	6d65 726f 2079 616d 616e 6567 656d 746e     emory management
3000c1d4:	6620 7561 746c 6920 2073 6163 7375 6465      fault is caused
3000c1e4:	6220 2079 6e75 7473 6361 696b 676e 6520      by unstacking e
3000c1f4:	7272 726f 4d00 6d65 726f 2079 616d 616e     rror.Memory mana
3000c204:	6567 656d 746e 6620 7561 746c 6920 2073     gement fault is 
3000c214:	6163 7375 6465 6220 2079 7473 6361 696b     caused by stacki
3000c224:	676e 6520 7272 726f 4d00 6d65 726f 2079     ng error.Memory 
3000c234:	616d 616e 6567 656d 746e 6620 7561 746c     management fault
3000c244:	6920 2073 6163 7375 6465 6220 2079 6c66      is caused by fl
3000c254:	616f 6974 676e 702d 696f 746e 6c20 7a61     oating-point laz
3000c264:	2079 7473 7461 2065 7270 7365 7265 6176     y state preserva
3000c274:	6974 6e6f 5400 6568 6d20 6d65 726f 2079     tion.The memory 
3000c284:	616d 616e 6567 656d 746e 6620 7561 746c     management fault
3000c294:	6f20 6363 7275 6572 2064 6461 7264 7365      occurred addres
3000c2a4:	2073 7369 4d00 414d 3a52 5b20 7830 3825     s is.MMAR: [0x%8
3000c2b4:	5d78 2d20 203e 7325 000a 7542 2073 6166     x] -> %s..Bus fa
3000c2c4:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c2d4:	6920 736e 7274 6375 6974 6e6f 6120 6363      instruction acc
3000c2e4:	7365 2073 6976 6c6f 7461 6f69 006e 4642     ess violation.BF
3000c2f4:	5253 203a 305b 2578 7838 205d 3e2d 2520     SR: [0x%8x] -> %
3000c304:	0a73 4200 7375 6620 7561 746c 6920 2073     s..Bus fault is 
3000c314:	6163 7375 6465 6220 2079 7270 6365 7369     caused by precis
3000c324:	2065 6164 6174 6120 6363 7365 2073 6976     e data access vi
3000c334:	6c6f 7461 6f69 006e 7542 2073 6166 6c75     olation.Bus faul
3000c344:	2074 7369 6320 7561 6573 2064 7962 6920     t is caused by i
3000c354:	706d 6572 6963 6573 6420 7461 2061 6361     mprecise data ac
3000c364:	6563 7373 7620 6f69 616c 6974 6e6f 4200     cess violation.B
3000c374:	7375 6620 7561 746c 6920 2073 6163 7375     us fault is caus
3000c384:	6465 6220 2079 6e75 7473 6361 696b 676e     ed by unstacking
3000c394:	6520 7272 726f 4200 7375 6620 7561 746c      error.Bus fault
3000c3a4:	6920 2073 6163 7375 6465 6220 2079 7473      is caused by st
3000c3b4:	6361 696b 676e 6520 7272 726f 4200 7375     acking error.Bus
3000c3c4:	6620 7561 746c 6920 2073 6163 7375 6465      fault is caused
3000c3d4:	6220 2079 6c66 616f 6974 676e 702d 696f      by floating-poi
3000c3e4:	746e 6c20 7a61 2079 7473 7461 2065 7270     nt lazy state pr
3000c3f4:	7365 7265 6176 6974 6e6f 5400 6568 6220     eservation.The b
3000c404:	7375 6620 7561 746c 6f20 6363 7275 6572     us fault occurre
3000c414:	2064 6461 7264 7365 2073 7369 4200 4146     d address is.BFA
3000c424:	3a52 5b20 7830 3825 5d78 2d20 203e 7325     R: [0x%8x] -> %s
3000c434:	000a 7355 6761 2065 6166 6c75 2074 7369     ..Usage fault is
3000c444:	6320 7561 6573 2064 7962 6120 7474 6d65      caused by attem
3000c454:	7470 2073 6f74 6520 6578 7563 6574 6120     pts to execute a
3000c464:	206e 6e75 6564 6966 656e 2064 6e69 7473     n undefined inst
3000c474:	7572 7463 6f69 006e 4655 5253 203a 305b     ruction.UFSR: [0
3000c484:	2578 7838 205d 3e2d 2520 0a73 5500 6173     x%8x] -> %s..Usa
3000c494:	6567 6620 7561 746c 6920 2073 6163 7375     ge fault is caus
3000c4a4:	6465 6220 2079 7461 6574 706d 7374 7420     ed by attempts t
3000c4b4:	206f 7773 7469 6863 7420 206f 6e61 6920     o switch to an i
3000c4c4:	766e 6c61 6469 7320 6174 6574 2820 2e65     nvalid state (e.
3000c4d4:	2e67 202c 5241 294d 5500 6173 6567 6620     g., ARM).Usage f
3000c4e4:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000c4f4:	2079 7461 6574 706d 7374 7420 206f 6f64     y attempts to do
3000c504:	6120 206e 7865 6563 7470 6f69 206e 6977      an exception wi
3000c514:	6874 6120 6220 6461 7620 6c61 6575 6920     th a bad value i
3000c524:	206e 6874 2065 5845 5f43 4552 5554 4e52     n the EXC_RETURN
3000c534:	6e20 6d75 6562 0072 7355 6761 2065 6166      number.Usage fa
3000c544:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c554:	6120 7474 6d65 7470 2073 6f74 6520 6578      attempts to exe
3000c564:	7563 6574 6120 6320 706f 6f72 6563 7373     cute a coprocess
3000c574:	726f 6920 736e 7274 6375 6974 6e6f 5500     or instruction.U
3000c584:	6173 6567 6620 7561 746c 6920 2073 6163     sage fault is ca
3000c594:	7375 6465 6220 2079 6e69 6964 6163 6574     used by indicate
3000c5a4:	2073 6874 7461 6120 7320 6174 6b63 6f20     s that a stack o
3000c5b4:	6576 6672 6f6c 2077 6828 7261 7764 7261     verflow (hardwar
3000c5c4:	2065 6863 6365 296b 6820 7361 7420 6b61     e check) has tak
3000c5d4:	6e65 7020 616c 6563 5500 6173 6567 6620     en place.Usage f
3000c5e4:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000c5f4:	2079 6e69 6964 6163 6574 2073 6874 7461     y indicates that
3000c604:	6120 206e 6e75 6c61 6769 656e 2064 6361      an unaligned ac
3000c614:	6563 7373 6620 7561 746c 6820 7361 7420     cess fault has t
3000c624:	6b61 6e65 7020 616c 6563 5500 6173 6567     aken place.Usage
3000c634:	6620 7561 746c 6920 2073 6163 7375 6465      fault is caused
3000c644:	6220 2079 6e49 6964 6163 6574 2073 2061      by Indicates a 
3000c654:	6964 6976 6564 6220 2079 657a 6f72 6820     divide by zero h
3000c664:	7361 7420 6b61 6e65 7020 616c 6563 2820     as taken place (
3000c674:	6163 206e 6562 7320 7465 6f20 6c6e 2079     can be set only 
3000c684:	6669 4420 5649 305f 545f 5052 6920 2073     if DIV_0_TRP is 
3000c694:	6573 2974 4400 6265 6775 6620 7561 746c     set).Debug fault
3000c6a4:	6920 2073 6163 7375 6465 6220 2079 6168      is caused by ha
3000c6b4:	746c 7220 7165 6575 7473 6465 6920 206e     lt requested in 
3000c6c4:	564e 4349 4400 5346 3a52 5b20 7830 3825     NVIC.DFSR: [0x%8
3000c6d4:	5d78 2d20 203e 7325 000a 6544 7562 2067     x] -> %s..Debug 
3000c6e4:	6166 6c75 2074 7369 6320 7561 6573 2064     fault is caused 
3000c6f4:	7962 4220 504b 2054 6e69 7473 7572 7463     by BKPT instruct
3000c704:	6f69 206e 7865 6365 7475 6465 4400 6265     ion executed.Deb
3000c714:	6775 6620 7561 746c 6920 2073 6163 7375     ug fault is caus
3000c724:	6465 6220 2079 5744 2054 616d 6374 2068     ed by DWT match 
3000c734:	636f 7563 7272 6465 4400 6265 6775 6620     occurred.Debug f
3000c744:	7561 746c 6920 2073 6163 7375 6465 6220     ault is caused b
3000c754:	2079 6556 7463 726f 6620 7465 6863 6f20     y Vector fetch o
3000c764:	6363 7275 6572 0064 6544 7562 2067 6166     ccurred.Debug fa
3000c774:	6c75 2074 7369 6320 7561 6573 2064 7962     ult is caused by
3000c784:	4520 4244 5247 2051 6973 6e67 6c61 6120      EDBGRQ signal a
3000c794:	7373 7265 6574 0064 4f42 544f 5b00 7325     sserted.BOOT.[%s
3000c7a4:	412d 205d 6148 6472 4620 7561 746c 5020     -A] Hard Fault P
3000c7b4:	7461 6863 2820 6553 7563 6572 0d29 000a     atch (Secure)...
3000c7c4:	255b 2d73 5d41 4520 6378 7065 6974 6e6f     [%s-A] Exception
3000c7d4:	6620 6f72 206d 6573 7563 6572 7720 726f      from secure wor
3000c7e4:	646c 0d21 000a 255b 2d73 5d41 4520 6378     ld!...[%s-A] Exc
3000c7f4:	7065 6974 6e6f 6620 6f72 206d 6f6e 2d6e     eption from non-
3000c804:	6573 7563 6572 7720 726f 646c 0d21 000a     secure world!...
3000c814:	255b 2d73 5d41 5320 5346 2052 3d20 3020     [%s-A] SFSR  = 0
3000c824:	2578 3830 0d78 000a 255b 2d73 5d41 5320     x%08x...[%s-A] S
3000c834:	4146 2052 3d20 3020 2578 3830 0d78 000a     FAR  = 0x%08x...
3000c844:	255b 2d73 5d41 5320 4348 5253 3d20 3020     [%s-A] SHCSR = 0
3000c854:	2578 3830 0d78 000a 544f 3141 5520 4553     x%08x...OTA1 USE
3000c864:	202c 6576 7372 6f69 3a6e 2520 0a64 4f00     , version: %d..O
3000c874:	4154 2032 5355 2c45 7620 7265 6973 6e6f     TA2 USE, version
3000c884:	203a 6425 000a 6f42 6874 4f20 4154 6920     : %d..Both OTA i
3000c894:	616d 6567 7620 7265 6669 2079 6166 6c69     mage verify fail
3000c8a4:	6465 000a 255b 2d73 5d49 4920 696e 2074     ed..[%s-I] Init 
3000c8b4:	4257 0a0d 6600 616c 6873 735f 7a69 3a65     WB...flash_size:
3000c8c4:	3820 0a4d 6600 616c 6873 735f 7a69 3a65      8M..flash_size:
3000c8d4:	3120 4d36 000a 5042 2031 6576 7372 6f69      16M..BP1 versio
3000c8e4:	206e 6e69 6176 696c 0a64 4200 3150 6620     n invalid..BP1 f
3000c8f4:	726f 616d 2074 6e69 6176 696c 0a64 4200     ormat invalid..B
3000c904:	3150 6120 7463 7669 2065 6e69 6564 2078     P1 active index 
3000c914:	6e69 6176 696c 0a64 4200 3150 6420 7461     invalid..BP1 dat
3000c924:	2061 6176 696c 2c64 7620 7265 6973 6e6f     a valid, version
3000c934:	253a 0a64 4200 3250 7620 7265 6973 6e6f     :%d..BP2 version
3000c944:	6920 766e 6c61 6469 000a 5042 2032 6f66      invalid..BP2 fo
3000c954:	6d72 7461 6920 766e 6c61 6469 000a 5042     rmat invalid..BP
3000c964:	2032 6361 6974 6576 6920 646e 7865 6920     2 active index i
3000c974:	766e 6c61 6469 000a 5042 2032 6164 6174     nvalid..BP2 data
3000c984:	7620 6c61 6469 202c 6576 7372 6f69 3a6e      valid, version:
3000c994:	6425 000a 5042 2031 5243 3343 2032 616d     %d..BP1 CRC32 ma
3000c9a4:	6374 2c68 6120 7474 6361 6568 2064 5243     tch, attached CR
3000c9b4:	3a43 3020 2578 2c78 6320 6c61 7563 616c     C: 0x%x, calcula
3000c9c4:	6574 2064 5243 3a43 3020 2578 0a78 4200     ted CRC: 0x%x..B
3000c9d4:	3150 4320 4352 3233 6e20 746f 6d20 7461     P1 CRC32 not mat
3000c9e4:	6863 202c 7461 6174 6863 6465 4320 4352     ch, attached CRC
3000c9f4:	203a 7830 7825 202c 6163 636c 6c75 7461     : 0x%x, calculat
3000ca04:	6465 4320 4352 203a 7830 7825 000a 5042     ed CRC: 0x%x..BP
3000ca14:	2032 5243 3343 2032 616d 6374 2c68 6120     2 CRC32 match, a
3000ca24:	7474 6361 6568 2064 5243 3a43 3020 2578     ttached CRC: 0x%
3000ca34:	2c78 6320 6c61 7563 616c 6574 2064 5243     x, calculated CR
3000ca44:	3a43 3020 2578 0a78 4200 3250 4320 4352     C: 0x%x..BP2 CRC
3000ca54:	3233 6e20 746f 6d20 7461 6863 202c 7461     32 not match, at
3000ca64:	6174 6863 6465 4320 4352 203a 7830 7825     tached CRC: 0x%x
3000ca74:	202c 6163 636c 6c75 7461 6465 4320 4352     , calculated CRC
3000ca84:	203a 7830 7825 000a 5042 2031 746f 3161     : 0x%x..BP1 ota1
3000ca94:	6120 6464 6572 7373 6920 766e 6c61 6469      address invalid
3000caa4:	000a 5042 2031 746f 3261 6120 6464 6572     ..BP1 ota2 addre
3000cab4:	7373 6920 766e 6c61 6469 000a 5042 2032     ss invalid..BP2 
3000cac4:	746f 3161 6120 6464 6572 7373 6920 766e     ota1 address inv
3000cad4:	6c61 6469 000a 5042 2032 746f 3261 6120     alid..BP2 ota2 a
3000cae4:	6464 6572 7373 6920 766e 6c61 6469 000a     ddress invalid..
3000caf4:	6f42 6874 4220 2050 5243 2043 6176 696c     Both BP CRC vali
3000cb04:	0a64 4200 3150 7320 6c65 6365 6574 0a64     d..BP1 selected.
3000cb14:	4200 3250 7320 6c65 6365 6574 0a64 4200     .BP2 selected..B
3000cb24:	746f 2068 5042 6920 766e 6c61 6469 000a     oth BP invalid..
3000cb34:	6552 6f63 6576 7972 7320 6174 7472 000a     Recovery start..
3000cb44:	6341 6974 6576 4920 646e 7865 203a 6425     Active Index: %d
3000cb54:	000a 544f 3141 6920 766e 6c61 6469 000a     ..OTA1 invalid..
3000cb64:	544f 3241 6920 766e 6c61 6469 000a 544f     OTA2 invalid..OT
3000cb74:	3141 4320 4352 3233 6d20 7461 6863 202c     A1 CRC32 match, 
3000cb84:	7461 6174 6863 6465 4320 4352 203a 7830     attached CRC: 0x
3000cb94:	7825 202c 6163 636c 6c75 7461 6465 4320     %x, calculated C
3000cba4:	4352 203a 7830 7825 000a 544f 3141 4320     RC: 0x%x..OTA1 C
3000cbb4:	4352 3233 6e20 746f 6d20 7461 6863 202c     RC32 not match, 
3000cbc4:	7461 6174 6863 6465 4320 4352 203a 7830     attached CRC: 0x
3000cbd4:	7825 202c 6163 636c 6c75 7461 6465 4320     %x, calculated C
3000cbe4:	4352 203a 7830 7825 202c 7265 7361 2065     RC: 0x%x, erase 
3000cbf4:	544f 3141 000a 544f 3241 4320 4352 3233     OTA1..OTA2 CRC32
3000cc04:	6d20 7461 6863 202c 7461 6174 6863 6465      match, attached
3000cc14:	4320 4352 203a 7830 7825 202c 6163 636c      CRC: 0x%x, calc
3000cc24:	6c75 7461 6465 4320 4352 203a 7830 7825     ulated CRC: 0x%x
3000cc34:	000a 544f 3241 4320 4352 3233 6e20 746f     ..OTA2 CRC32 not
3000cc44:	6d20 7461 6863 202c 7461 6174 6863 6465      match, attached
3000cc54:	4320 4352 203a 7830 7825 202c 6163 636c      CRC: 0x%x, calc
3000cc64:	6c75 7461 6465 4320 4352 203a 7830 7825     ulated CRC: 0x%x
3000cc74:	202c 7265 7361 2065 544f 3241 000a 6341     , erase OTA2..Ac
3000cc84:	6974 6576 6920 646e 7865 4f20 4154 6920     tive index OTA i
3000cc94:	766e 6c61 6469 202c 6572 7277 7469 2065     nvalid, rewrite 
3000cca4:	6361 6974 6576 6920 646e 7865 7420 206f     active index to 
3000ccb4:	6425 000a 6c41 206c 544f 2041 6461 7264     %d..All OTA addr
3000ccc4:	7365 6573 2073 6f6e 2074 6176 696c 2c64     esses not valid,
3000ccd4:	7220 6365 766f 7265 2079 6166 6c69 000a      recovery fail..
3000cce4:	255b 2d73 5d49 4420 7369 6261 656c 4b20     [%s-I] Disable K
3000ccf4:	304d 4220 6f6f 2074 6920 206e 6554 7473     M0 Boot  in Test
3000cd04:	6d20 646f 0d65 000a 255b 2d73 5d49 4220      mode...[%s-I] B
3000cd14:	7079 7361 2073 544f 2050 7561 6f74 6f6c     ypass OTP autolo
3000cd24:	6461 0a0d 5b00 7325 492d 205d 504e 4620     ad...[%s-I] NP F
3000cd34:	6572 2071 6425 4d20 7a48 000a 255b 2d73     req %d MHz..[%s-
3000cd44:	5d49 4120 2050 7246 7165 2520 2064 484d     I] AP Freq %d MH
3000cd54:	0a7a 0a00 6170 7373 6f77 6472 7620 7265     z...password ver
3000cd64:	6669 2079 6166 6c69 000a 700a 7361 7773     ify fail...passw
3000cd74:	726f 2064 6576 6972 7966 7320 6375 6563     ord verify succe
3000cd84:	7373 000a 4d49 3147 5320 4f42 544f 4f20     ss..IMG1 SBOOT O
3000cd94:	4646 000a 4d49 3147 5320 4f42 544f 5320     FF..IMG1 SBOOT S
3000cda4:	4355 4543 5353 000a 6f62 746f 6f6c 6461     UCCESS..bootload
3000cdb4:	7265 765f 7265 6973 6e6f 2520 0a73 5b00     er_version %s..[
3000cdc4:	4f4d 5544 454c 425f 4f4f 2d54 454c 4556     MODULE_BOOT-LEVE
3000cdd4:	5f4c 4e49 4f46 3a5d 4d49 3147 4520 544e     L_INFO]:IMG1 ENT
3000cde4:	5245 4d20 5053 5b3a 3025 7838 0a5d 5b00     ER MSP:[%08x]..[
3000cdf4:	4f4d 5544 454c 425f 4f4f 2d54 454c 4556     MODULE_BOOT-LEVE
3000ce04:	5f4c 4e49 4f46 3a5d 4d49 3147 5320 4345     L_INFO]:IMG1 SEC
3000ce14:	5255 2045 5453 5441 3a45 2520 0a64 5b00     URE STATE: %d..[
3000ce24:	7325 492d 205d 6e49 7469 5020 5253 4d41     %s-I] Init PSRAM
3000ce34:	0a0d 5b00 7325 572d 205d 6573 2074 6670     ...[%s-W] set pf
3000ce44:	206d 6166 6c69 0a0d 5b00 7325 452d 205d     m fail...[%s-E] 
3000ce54:	5245 4f52 2152 2021 6853 756f 646c 4e20     ERROR!! Should N
3000ce64:	746f 6520 616e 6c62 2065 654d 536d 7277     ot enable MemSwr
3000ce74:	4f5f 6c6e 2079 6e69 4420 5244 4320 6968     _Only in DDR Chi
3000ce84:	2170 2121 0a0d 5b00 4f4d 5544 454c 425f     p!!!...[MODULE_B
3000ce94:	4f4f 2d54 454c 4556 5f4c 4e49 4f46 3a5d     OOT-LEVEL_INFO]:
3000cea4:	6e49 7469 4420 5244 0a0d 5300 4457 6920     Init DDR...SWD i
3000ceb4:	2073 6964 6173 6c62 6465 202c 7270 636f     s disabled, proc
3000cec4:	6565 2064 6f74 6420 7369 6261 656c 5520     eed to disable U
3000ced4:	7261 2074 6f44 6e77 6f6c 6461 2e2e 0a2e     art Download....
3000cee4:	5500 7261 2074 6f44 6e77 6f6c 6461 6820     .Uart Download h
3000cef4:	7361 6220 6565 206e 6964 6173 6c62 6465     as been disabled
3000cf04:	000a 6f4c 5567 7261 2074 6142 6475 6172     ..LogUart Baudra
3000cf14:	6574 203a 6425 000a 530a 4848 544f 4946     te: %d...SHHOTFI
3000cf24:	2058 6f6e 2074 6572 6563 7669 6465 000a     X not received..
3000cf34:	4e0a 726f 616d 206c 6f62 746f 000a 6962     .Normal boot..bi
3000cf44:	5f6e 6f6e 313e 2c30 3d20 2520 0a64 6400     n_no>10, = %d..d
3000cf54:	2061 6f6e 2074 6572 6563 7669 6465 000a     a not received..
3000cf64:	680a 746f 6966 2078 6f64 656e 2e2e 202e     .hotfix done... 
3000cf74:	6552 6f62 746f 6e69 2e67 0a2e 0000 0000     Rebooting.......
3000cf84:	0208 0000 020c 0000 0210 0000 0008 0000     ................
3000cf94:	0214 0000 0218 0000 021c 0000 000c 0000     ................

3000cfa4 <__FUNCTION__.0>:
3000cfa4:	4f42 544f 535f 434f 435f 6b6c 6553 0074     BOOT_SOC_ClkSet.
3000cfb4:	4d4b 2030 4958 2050 4d49 0047 4d4b 2030     KM0 XIP IMG.KM0 
3000cfc4:	5253 4d41 4b00 304d 4420 4152 004d 4d4b     SRAM.KM0 DRAM.KM
3000cfd4:	2034 4958 2050 4d49 0047 4d4b 2034 5253     4 XIP IMG.KM4 SR
3000cfe4:	4d41 4b00 344d 4420 4152 004d 5041 5820     AM.KM4 DRAM.AP X
3000cff4:	5049 4920 474d 4100 2050 4c42 2031 5253     IP IMG.AP BL1 SR
3000d004:	4d41 4100 2050 4c42 2031 5244 4d41 4100     AM.AP BL1 DRAM.A
3000d014:	2050 4946 0050 255b 2d73 5d49 2520 2073     P FIP.[%s-I] %s 
3000d024:	6e49 6176 696c 0a64 5b00 7325 492d 205d     Invalid..[%s-I] 
3000d034:	7325 255b 3830 3a78 7825 0a5d 5b00 7325     %s[%08x:%x]..[%s
3000d044:	492d 205d 4452 2050 4e45 000a 255b 2d73     -I] RDP EN..[%s-
3000d054:	5d49 4620 6961 206c 6f74 6c20 616f 2064     I] Fail to load 
3000d064:	4452 2050 6d69 6761 2165 000a 255b 2d73     RDP image!..[%s-
3000d074:	5d49 5220 5044 4f20 4646 000a 255b 2d73     I] RDP OFF..[%s-
3000d084:	5d49 4920 474d 2032 544f 2046 4e45 000a     I] IMG2 OTF EN..
3000d094:	255b 2d73 5d45 4f20 4654 4d20 646f 2065     [%s-E] OTF Mode 
3000d0a4:	7265 6f72 0d72 000a 4e49 4156 494c 0044     error...INVALID.
3000d0b4:	6563 7472 6669 6369 7461 2065 4345 2043     certificate ECC 
3000d0c4:	6576 6972 7966 6620 6961 2c6c 7220 6c6f     verify fail, rol
3000d0d4:	626c 6361 0d6b 000a 4d5b 444f 4c55 5f45     lback...[MODULE_
3000d0e4:	4f42 544f 4c2d 5645 4c45 495f 464e 5d4f     BOOT-LEVEL_INFO]
3000d0f4:	493a 474d 2832 544f 2541 2964 2520 2c73     :IMG2(OTA%d) %s,
3000d104:	7220 7465 203a 6425 000a 4d49 3247 4520      ret: %d..IMG2 E
3000d114:	4343 7620 7265 6669 2079 6166 6c69 202c     CC verify fail, 
3000d124:	6f72 6c6c 6162 6b63 0a0d 4f00 4154 415f     rollback...OTA_A
3000d134:	2050 4345 2043 6576 6972 7966 6620 6961     P ECC verify fai
3000d144:	2c6c 7220 6c6f 626c 6361 0d6b 000a 6f4c     l, rollback...Lo
3000d154:	6461 4f20 4154 415f 2050 6d69 6761 2065     ad OTA_AP image 
3000d164:	6166 6c69 202c 6f72 6c6c 6162 6b63 0a0d     fail, rollback..
3000d174:	5b00 4f4d 5544 454c 425f 4f4f 2d54 454c     .[MODULE_BOOT-LE
3000d184:	4556 5f4c 5245 4f52 5d52 463a 6961 206c     VEL_ERROR]:Fail 
3000d194:	6f74 6c20 616f 2064 5041 6920 616d 6567     to load AP image
3000d1a4:	0a21 0000 cfb4 3000 cfc0 3000 cfc9 3000     !......0...0...0
3000d1b4:	cfd2 3000 cfde 3000 cfe7 3000 cff0 3000     ...0...0...0...0
3000d1c4:	cffb 3000 d007 3000 d013 3000 4d4b 2034     ...0...0...0KM4 
3000d1d4:	4d49 3347 4b00 344d 4e20 4353 4900 474d     IMG3.KM4 NSC.IMG
3000d1e4:	0031 4d49 3247 4100 2050 4c42 2631 4946     1.IMG2.AP BL1&FI
3000d1f4:	0050 4d5b 444f 4c55 5f45 4f42 544f 4c2d     P.[MODULE_BOOT-L
3000d204:	5645 4c45 495f 464e 5d4f 253a 5b73 3025     EVEL_INFO]:%s[%0
3000d214:	7838 253a 5d78 000a 4452 2050 4146 4c49     8x:%x]..RDP FAIL
3000d224:	000a 4452 2050 5553 4343 5345 0a53 b300     ..RDP SUCCESS...
3000d234:	9e4e 3905 11ea 0a4b 1b92 ee2a 939c 0029     N..9..K...*...).
3000d244:	5041 4220 334c 0032 255b 2d73 5d49 2520     AP BL32.[%s-I] %
3000d254:	2073 4556 4952 5946 5020 5341 0a53 5b00     s VERIFY PASS..[
3000d264:	7325 452d 205d 7325 5620 5245 4649 2059     %s-E] %s VERIFY 
3000d274:	4146 4c49 202c 6572 2074 203d 6425 000a     FAIL, ret = %d..
3000d284:	255b 2d73 5d45 4320 7265 6974 6966 6163     [%s-E] Certifica
3000d294:	6574 5620 5245 4649 2059 4146 4c49 202c     te VERIFY FAIL, 
3000d2a4:	6572 2074 203d 6425 000a 0000 d1d0 3000     ret = %d.......0
3000d2b4:	d1d9 3000 d1e1 3000 d1e6 3000 d1d4 3000     ...0...0...0...0
3000d2c4:	d1eb 3000                                   ...0

3000d2c8 <km4_bootloader_rev>:
3000d2c8:	6d6b 5f34 6f62 746f 6f6c 6461 7265 765f     km4_bootloader_v
3000d2d8:	7265 325f 3064 3166 6562 3330 5f32 3032     er_2d0f1be032_20
3000d2e8:	3432 302f 2f34 3932 302d 3a39 3135 313a     24/04/29-09:51:1
3000d2f8:	0037 0a0d 6c63 736f 2065 4d79 646f 6d65     7...close yModem
3000d308:	5420 6172 736e 6566 2072 2e2e 0d2e 000a      Transfer ......
3000d318:	255b 2d73 5d57 2a20 2a2a 2a2a 2a2a 2a2a     [%s-W] *********
3000d328:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d338:	202a 4157 4e52 4e49 2047 2a2a 2a2a 2a2a     * WARNING ******
3000d348:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d358:	0a2a 5b00 7325 572d 205d 202a 2020 2020     *..[%s-W] *     
3000d368:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d378:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d388:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d398:	2020 2a20 000a 255b 2d73 5d57 2a20 2020        *..[%s-W] *  
3000d3a8:	2020 2020 2020 2020 2020 4320 7275 6572                Curre
3000d3b8:	746e 6320 6968 2070 6576 7372 6f69 206e     nt chip version 
3000d3c8:	7369 4120 7543 2174 2121 2020 2020 2020     is ACut!!!      
3000d3d8:	2020 2020 2020 0a2a 5b00 7325 572d 205d           *..[%s-W] 
3000d3e8:	202a 2020 5452 204b 4453 204b 6f64 7365     *   RTK SDK does
3000d3f8:	6e20 746f 7320 7075 6f70 7472 6320 7275      not support cur
3000d408:	6572 746e 7620 7265 6973 6e6f 6120 796e     rent version any
3000d418:	6d20 726f 2e65 2020 2a20 000a 255b 2d73      more.   *..[%s-
3000d428:	5d57 2a20 2020 2020 2020 6c50 6165 6573     W] *      Please
3000d438:	6320 6e6f 6174 7463 5220 4b54 5020 204d      contact RTK PM 
3000d448:	6f66 2072 6f6d 6572 7020 6f72 7564 7463     for more product
3000d458:	6920 666e 216f 2020 2020 2020 0a2a 5b00      info!      *..[
3000d468:	7325 572d 205d 2a2a 2a2a 2a2a 2a2a 2a2a     %s-W] **********
3000d478:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d488:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d498:	2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a 2a2a     ****************
3000d4a8:	000a 255b 2d73 5d57 0a20 5000 534f 5849     ..[%s-W] ..POSIX
3000d4b8:	2e00                                         ...

3000d4bb <_ctype_>:
3000d4bb:	2000 2020 2020 2020 2020 2828 2828 2028     .         ((((( 
3000d4cb:	2020 2020 2020 2020 2020 2020 2020 2020                     
3000d4db:	8820 1010 1010 1010 1010 1010 1010 1010      ...............
3000d4eb:	0410 0404 0404 0404 0404 1004 1010 1010     ................
3000d4fb:	1010 4141 4141 4141 0101 0101 0101 0101     ..AAAAAA........
3000d50b:	0101 0101 0101 0101 0101 0101 1010 1010     ................
3000d51b:	1010 4242 4242 4242 0202 0202 0202 0202     ..BBBBBB........
3000d52b:	0202 0202 0202 0202 0202 0202 1010 1010     ................
3000d53b:	0020 0000 0000 0000 0000 0000 0000 0000      ...............
	...
