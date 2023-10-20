
D:/sdk-RTL8730E_v8.2b_beta/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


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
30003178:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000317c:	b087      	sub	sp, #28
3000317e:	0052      	lsls	r2, r2, #1
30003180:	4699      	mov	r9, r3
30003182:	4682      	mov	sl, r0
30003184:	f89d 3040 	ldrb.w	r3, [sp, #64]	; 0x40
30003188:	3201      	adds	r2, #1
3000318a:	9101      	str	r1, [sp, #4]
3000318c:	9303      	str	r3, [sp, #12]
3000318e:	fa5f f882 	uxtb.w	r8, r2
30003192:	4b76      	ldr	r3, [pc, #472]	; (3000336c <FLASH_CalibrationNew+0x1f4>)
30003194:	f89d 4044 	ldrb.w	r4, [sp, #68]	; 0x44
30003198:	4798      	blx	r3
3000319a:	9901      	ldr	r1, [sp, #4]
3000319c:	2800      	cmp	r0, #0
3000319e:	f000 80a7 	beq.w	300032f0 <FLASH_CalibrationNew+0x178>
300031a2:	4873      	ldr	r0, [pc, #460]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300031a4:	4b73      	ldr	r3, [pc, #460]	; (30003374 <FLASH_CalibrationNew+0x1fc>)
300031a6:	4798      	blx	r3
300031a8:	4971      	ldr	r1, [pc, #452]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300031aa:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
300031ae:	9b03      	ldr	r3, [sp, #12]
300031b0:	45a0      	cmp	r8, r4
300031b2:	7949      	ldrb	r1, [r1, #5]
300031b4:	ea4f 3303 	mov.w	r3, r3, lsl #12
300031b8:	6151      	str	r1, [r2, #20]
300031ba:	f8d2 111c 	ldr.w	r1, [r2, #284]	; 0x11c
300031be:	b29b      	uxth	r3, r3
300031c0:	f421 4170 	bic.w	r1, r1, #61440	; 0xf000
300031c4:	ea43 0301 	orr.w	r3, r3, r1
300031c8:	f8c2 311c 	str.w	r3, [r2, #284]	; 0x11c
300031cc:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300031d0:	6c1a      	ldr	r2, [r3, #64]	; 0x40
300031d2:	f422 4200 	bic.w	r2, r2, #32768	; 0x8000
300031d6:	641a      	str	r2, [r3, #64]	; 0x40
300031d8:	6c1a      	ldr	r2, [r3, #64]	; 0x40
300031da:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300031de:	641a      	str	r2, [r3, #64]	; 0x40
300031e0:	f0c0 80a4 	bcc.w	3000332c <FLASH_CalibrationNew+0x1b4>
300031e4:	f04f 33ff 	mov.w	r3, #4294967295
300031e8:	2700      	movs	r7, #0
300031ea:	f8df b1b4 	ldr.w	fp, [pc, #436]	; 300033a0 <FLASH_CalibrationNew+0x228>
300031ee:	f04f 6500 	mov.w	r5, #134217728	; 0x8000000
300031f2:	4e61      	ldr	r6, [pc, #388]	; (30003378 <FLASH_CalibrationNew+0x200>)
300031f4:	9302      	str	r3, [sp, #8]
300031f6:	9301      	str	r3, [sp, #4]
300031f8:	e008      	b.n	3000320c <FLASH_CalibrationNew+0x94>
300031fa:	9b01      	ldr	r3, [sp, #4]
300031fc:	9402      	str	r4, [sp, #8]
300031fe:	3301      	adds	r3, #1
30003200:	d05f      	beq.n	300032c2 <FLASH_CalibrationNew+0x14a>
30003202:	444f      	add	r7, r9
30003204:	444c      	add	r4, r9
30003206:	b2e4      	uxtb	r4, r4
30003208:	45a0      	cmp	r8, r4
3000320a:	d360      	bcc.n	300032ce <FLASH_CalibrationNew+0x156>
3000320c:	4620      	mov	r0, r4
3000320e:	f7ff ff67 	bl	300030e0 <FLASH_CalibrationPhaseIdx>
30003212:	6973      	ldr	r3, [r6, #20]
30003214:	03db      	lsls	r3, r3, #15
30003216:	d507      	bpl.n	30003228 <FLASH_CalibrationNew+0xb0>
30003218:	f3bf 8f4f 	dsb	sy
3000321c:	f8c6 525c 	str.w	r5, [r6, #604]	; 0x25c
30003220:	f3bf 8f4f 	dsb	sy
30003224:	f3bf 8f6f 	isb	sy
30003228:	47d8      	blx	fp
3000322a:	6829      	ldr	r1, [r5, #0]
3000322c:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
30003230:	9104      	str	r1, [sp, #16]
30003232:	686a      	ldr	r2, [r5, #4]
30003234:	9205      	str	r2, [sp, #20]
30003236:	b11b      	cbz	r3, 30003240 <FLASH_CalibrationNew+0xc8>
30003238:	4b50      	ldr	r3, [pc, #320]	; (3000337c <FLASH_CalibrationNew+0x204>)
3000323a:	681b      	ldr	r3, [r3, #0]
3000323c:	0398      	lsls	r0, r3, #14
3000323e:	d452      	bmi.n	300032e6 <FLASH_CalibrationNew+0x16e>
30003240:	2208      	movs	r2, #8
30003242:	494f      	ldr	r1, [pc, #316]	; (30003380 <FLASH_CalibrationNew+0x208>)
30003244:	a804      	add	r0, sp, #16
30003246:	4b4f      	ldr	r3, [pc, #316]	; (30003384 <FLASH_CalibrationNew+0x20c>)
30003248:	4798      	blx	r3
3000324a:	2800      	cmp	r0, #0
3000324c:	d0d5      	beq.n	300031fa <FLASH_CalibrationNew+0x82>
3000324e:	2f00      	cmp	r7, #0
30003250:	ddd8      	ble.n	30003204 <FLASH_CalibrationNew+0x8c>
30003252:	9b02      	ldr	r3, [sp, #8]
30003254:	9a01      	ldr	r2, [sp, #4]
30003256:	061c      	lsls	r4, r3, #24
30003258:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
3000325c:	ea44 4402 	orr.w	r4, r4, r2, lsl #16
30003260:	433c      	orrs	r4, r7
30003262:	2b00      	cmp	r3, #0
30003264:	d05d      	beq.n	30003322 <FLASH_CalibrationNew+0x1aa>
30003266:	4d45      	ldr	r5, [pc, #276]	; (3000337c <FLASH_CalibrationNew+0x204>)
30003268:	682a      	ldr	r2, [r5, #0]
3000326a:	0390      	lsls	r0, r2, #14
3000326c:	d508      	bpl.n	30003280 <FLASH_CalibrationNew+0x108>
3000326e:	9903      	ldr	r1, [sp, #12]
30003270:	4845      	ldr	r0, [pc, #276]	; (30003388 <FLASH_CalibrationNew+0x210>)
30003272:	f008 ffa1 	bl	3000c1b8 <__DiagPrintf_veneer>
30003276:	682b      	ldr	r3, [r5, #0]
30003278:	039b      	lsls	r3, r3, #14
3000327a:	d46c      	bmi.n	30003356 <FLASH_CalibrationNew+0x1de>
3000327c:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
30003280:	9a01      	ldr	r2, [sp, #4]
30003282:	eb02 0167 	add.w	r1, r2, r7, asr #1
30003286:	b2c9      	uxtb	r1, r1
30003288:	f88a 1061 	strb.w	r1, [sl, #97]	; 0x61
3000328c:	b11b      	cbz	r3, 30003296 <FLASH_CalibrationNew+0x11e>
3000328e:	4b3b      	ldr	r3, [pc, #236]	; (3000337c <FLASH_CalibrationNew+0x204>)
30003290:	681b      	ldr	r3, [r3, #0]
30003292:	039d      	lsls	r5, r3, #14
30003294:	d465      	bmi.n	30003362 <FLASH_CalibrationNew+0x1ea>
30003296:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000329a:	4d35      	ldr	r5, [pc, #212]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
3000329c:	6c13      	ldr	r3, [r2, #64]	; 0x40
3000329e:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300032a2:	6413      	str	r3, [r2, #64]	; 0x40
300032a4:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300032a8:	79aa      	ldrb	r2, [r5, #6]
300032aa:	615a      	str	r2, [r3, #20]
300032ac:	4b2f      	ldr	r3, [pc, #188]	; (3000336c <FLASH_CalibrationNew+0x1f4>)
300032ae:	4798      	blx	r3
300032b0:	b310      	cbz	r0, 300032f8 <FLASH_CalibrationNew+0x180>
300032b2:	4628      	mov	r0, r5
300032b4:	7929      	ldrb	r1, [r5, #4]
300032b6:	4b2f      	ldr	r3, [pc, #188]	; (30003374 <FLASH_CalibrationNew+0x1fc>)
300032b8:	4798      	blx	r3
300032ba:	4620      	mov	r0, r4
300032bc:	b007      	add	sp, #28
300032be:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300032c2:	9401      	str	r4, [sp, #4]
300032c4:	444c      	add	r4, r9
300032c6:	2701      	movs	r7, #1
300032c8:	b2e4      	uxtb	r4, r4
300032ca:	45a0      	cmp	r8, r4
300032cc:	d29e      	bcs.n	3000320c <FLASH_CalibrationNew+0x94>
300032ce:	9b02      	ldr	r3, [sp, #8]
300032d0:	061c      	lsls	r4, r3, #24
300032d2:	9b01      	ldr	r3, [sp, #4]
300032d4:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
300032d8:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
300032dc:	433c      	orrs	r4, r7
300032de:	b9a3      	cbnz	r3, 3000330a <FLASH_CalibrationNew+0x192>
300032e0:	2f00      	cmp	r7, #0
300032e2:	ddd8      	ble.n	30003296 <FLASH_CalibrationNew+0x11e>
300032e4:	e7ca      	b.n	3000327c <FLASH_CalibrationNew+0x104>
300032e6:	4623      	mov	r3, r4
300032e8:	4828      	ldr	r0, [pc, #160]	; (3000338c <FLASH_CalibrationNew+0x214>)
300032ea:	f008 ff65 	bl	3000c1b8 <__DiagPrintf_veneer>
300032ee:	e7a7      	b.n	30003240 <FLASH_CalibrationNew+0xc8>
300032f0:	481f      	ldr	r0, [pc, #124]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300032f2:	4b27      	ldr	r3, [pc, #156]	; (30003390 <FLASH_CalibrationNew+0x218>)
300032f4:	4798      	blx	r3
300032f6:	e757      	b.n	300031a8 <FLASH_CalibrationNew+0x30>
300032f8:	4b1d      	ldr	r3, [pc, #116]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300032fa:	7919      	ldrb	r1, [r3, #4]
300032fc:	4618      	mov	r0, r3
300032fe:	4b24      	ldr	r3, [pc, #144]	; (30003390 <FLASH_CalibrationNew+0x218>)
30003300:	4798      	blx	r3
30003302:	4620      	mov	r0, r4
30003304:	b007      	add	sp, #28
30003306:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000330a:	4d1c      	ldr	r5, [pc, #112]	; (3000337c <FLASH_CalibrationNew+0x204>)
3000330c:	682b      	ldr	r3, [r5, #0]
3000330e:	039b      	lsls	r3, r3, #14
30003310:	d5e6      	bpl.n	300032e0 <FLASH_CalibrationNew+0x168>
30003312:	9903      	ldr	r1, [sp, #12]
30003314:	481c      	ldr	r0, [pc, #112]	; (30003388 <FLASH_CalibrationNew+0x210>)
30003316:	f008 ff4f 	bl	3000c1b8 <__DiagPrintf_veneer>
3000331a:	682b      	ldr	r3, [r5, #0]
3000331c:	039e      	lsls	r6, r3, #14
3000331e:	d41a      	bmi.n	30003356 <FLASH_CalibrationNew+0x1de>
30003320:	e7de      	b.n	300032e0 <FLASH_CalibrationNew+0x168>
30003322:	eb02 0767 	add.w	r7, r2, r7, asr #1
30003326:	f88a 7061 	strb.w	r7, [sl, #97]	; 0x61
3000332a:	e7b4      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
3000332c:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
30003330:	b90b      	cbnz	r3, 30003336 <FLASH_CalibrationNew+0x1be>
30003332:	4c18      	ldr	r4, [pc, #96]	; (30003394 <FLASH_CalibrationNew+0x21c>)
30003334:	e7af      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
30003336:	4c11      	ldr	r4, [pc, #68]	; (3000337c <FLASH_CalibrationNew+0x204>)
30003338:	6823      	ldr	r3, [r4, #0]
3000333a:	0399      	lsls	r1, r3, #14
3000333c:	d5f9      	bpl.n	30003332 <FLASH_CalibrationNew+0x1ba>
3000333e:	9903      	ldr	r1, [sp, #12]
30003340:	4811      	ldr	r0, [pc, #68]	; (30003388 <FLASH_CalibrationNew+0x210>)
30003342:	f008 ff39 	bl	3000c1b8 <__DiagPrintf_veneer>
30003346:	6823      	ldr	r3, [r4, #0]
30003348:	039a      	lsls	r2, r3, #14
3000334a:	d5f2      	bpl.n	30003332 <FLASH_CalibrationNew+0x1ba>
3000334c:	f04f 33ff 	mov.w	r3, #4294967295
30003350:	2700      	movs	r7, #0
30003352:	4c10      	ldr	r4, [pc, #64]	; (30003394 <FLASH_CalibrationNew+0x21c>)
30003354:	9301      	str	r3, [sp, #4]
30003356:	463a      	mov	r2, r7
30003358:	9901      	ldr	r1, [sp, #4]
3000335a:	480f      	ldr	r0, [pc, #60]	; (30003398 <FLASH_CalibrationNew+0x220>)
3000335c:	f008 ff2c 	bl	3000c1b8 <__DiagPrintf_veneer>
30003360:	e7be      	b.n	300032e0 <FLASH_CalibrationNew+0x168>
30003362:	480e      	ldr	r0, [pc, #56]	; (3000339c <FLASH_CalibrationNew+0x224>)
30003364:	f008 ff28 	bl	3000c1b8 <__DiagPrintf_veneer>
30003368:	e795      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
3000336a:	bf00      	nop
3000336c:	3000a501 	.word	0x3000a501
30003370:	2001c01c 	.word	0x2001c01c
30003374:	00009c99 	.word	0x00009c99
30003378:	e000ed00 	.word	0xe000ed00
3000337c:	2001c00c 	.word	0x2001c00c
30003380:	00054060 	.word	0x00054060
30003384:	000129bd 	.word	0x000129bd
30003388:	3000c258 	.word	0x3000c258
3000338c:	3000c22c 	.word	0x3000c22c
30003390:	0000b7e9 	.word	0x0000b7e9
30003394:	ffff0000 	.word	0xffff0000
30003398:	3000c27c 	.word	0x3000c27c
3000339c:	3000c2b8 	.word	0x3000c2b8
300033a0:	3000a311 	.word	0x3000a311

300033a4 <FLASH_Calibration>:
300033a4:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300033a8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300033ac:	695c      	ldr	r4, [r3, #20]
300033ae:	b087      	sub	sp, #28
300033b0:	4616      	mov	r6, r2
300033b2:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300033b6:	9403      	str	r4, [sp, #12]
300033b8:	4607      	mov	r7, r0
300033ba:	f8d3 311c 	ldr.w	r3, [r3, #284]	; 0x11c
300033be:	9100      	str	r1, [sp, #0]
300033c0:	9304      	str	r3, [sp, #16]
300033c2:	7943      	ldrb	r3, [r0, #5]
300033c4:	9302      	str	r3, [sp, #8]
300033c6:	6c13      	ldr	r3, [r2, #64]	; 0x40
300033c8:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300033cc:	6413      	str	r3, [r2, #64]	; 0x40
300033ce:	4b44      	ldr	r3, [pc, #272]	; (300034e0 <FLASH_Calibration+0x13c>)
300033d0:	4798      	blx	r3
300033d2:	9900      	ldr	r1, [sp, #0]
300033d4:	2800      	cmp	r0, #0
300033d6:	d067      	beq.n	300034a8 <FLASH_Calibration+0x104>
300033d8:	4842      	ldr	r0, [pc, #264]	; (300034e4 <FLASH_Calibration+0x140>)
300033da:	4b43      	ldr	r3, [pc, #268]	; (300034e8 <FLASH_Calibration+0x144>)
300033dc:	4798      	blx	r3
300033de:	9a02      	ldr	r2, [sp, #8]
300033e0:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300033e4:	eb16 0642 	adds.w	r6, r6, r2, lsl #1
300033e8:	615a      	str	r2, [r3, #20]
300033ea:	d077      	beq.n	300034dc <FLASH_Calibration+0x138>
300033ec:	f04f 0800 	mov.w	r8, #0
300033f0:	f04f 33ff 	mov.w	r3, #4294967295
300033f4:	f8df 910c 	ldr.w	r9, [pc, #268]	; 30003504 <FLASH_Calibration+0x160>
300033f8:	f04f 6400 	mov.w	r4, #134217728	; 0x8000000
300033fc:	46c2      	mov	sl, r8
300033fe:	4d3b      	ldr	r5, [pc, #236]	; (300034ec <FLASH_Calibration+0x148>)
30003400:	f8df b0f4 	ldr.w	fp, [pc, #244]	; 300034f8 <FLASH_Calibration+0x154>
30003404:	9301      	str	r3, [sp, #4]
30003406:	f8cd 8000 	str.w	r8, [sp]
3000340a:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000340e:	ea4f 330a 	mov.w	r3, sl, lsl #12
30003412:	f8d0 111c 	ldr.w	r1, [r0, #284]	; 0x11c
30003416:	b29b      	uxth	r3, r3
30003418:	f421 4170 	bic.w	r1, r1, #61440	; 0xf000
3000341c:	430b      	orrs	r3, r1
3000341e:	f8c0 311c 	str.w	r3, [r0, #284]	; 0x11c
30003422:	696b      	ldr	r3, [r5, #20]
30003424:	03d9      	lsls	r1, r3, #15
30003426:	d507      	bpl.n	30003438 <FLASH_Calibration+0x94>
30003428:	f3bf 8f4f 	dsb	sy
3000342c:	f8c5 425c 	str.w	r4, [r5, #604]	; 0x25c
30003430:	f3bf 8f4f 	dsb	sy
30003434:	f3bf 8f6f 	isb	sy
30003438:	4b2d      	ldr	r3, [pc, #180]	; (300034f0 <FLASH_Calibration+0x14c>)
3000343a:	4798      	blx	r3
3000343c:	f897 3060 	ldrb.w	r3, [r7, #96]	; 0x60
30003440:	6821      	ldr	r1, [r4, #0]
30003442:	b11b      	cbz	r3, 3000344c <FLASH_Calibration+0xa8>
30003444:	f8db 3000 	ldr.w	r3, [fp]
30003448:	039a      	lsls	r2, r3, #14
3000344a:	d43f      	bmi.n	300034cc <FLASH_Calibration+0x128>
3000344c:	f8d9 3000 	ldr.w	r3, [r9]
30003450:	428b      	cmp	r3, r1
30003452:	d109      	bne.n	30003468 <FLASH_Calibration+0xc4>
30003454:	9b01      	ldr	r3, [sp, #4]
30003456:	f108 0801 	add.w	r8, r8, #1
3000345a:	3301      	adds	r3, #1
3000345c:	f04f 0301 	mov.w	r3, #1
30003460:	bf08      	it	eq
30003462:	f8cd a004 	streq.w	sl, [sp, #4]
30003466:	9300      	str	r3, [sp, #0]
30003468:	f10a 0a01 	add.w	sl, sl, #1
3000346c:	4556      	cmp	r6, sl
3000346e:	d1cc      	bne.n	3000340a <FLASH_Calibration+0x66>
30003470:	f1b8 0f00 	cmp.w	r8, #0
30003474:	dd06      	ble.n	30003484 <FLASH_Calibration+0xe0>
30003476:	9b01      	ldr	r3, [sp, #4]
30003478:	eb03 0168 	add.w	r1, r3, r8, asr #1
3000347c:	f897 3060 	ldrb.w	r3, [r7, #96]	; 0x60
30003480:	7779      	strb	r1, [r7, #29]
30003482:	b9d3      	cbnz	r3, 300034ba <FLASH_Calibration+0x116>
30003484:	4b16      	ldr	r3, [pc, #88]	; (300034e0 <FLASH_Calibration+0x13c>)
30003486:	4798      	blx	r3
30003488:	b190      	cbz	r0, 300034b0 <FLASH_Calibration+0x10c>
3000348a:	4816      	ldr	r0, [pc, #88]	; (300034e4 <FLASH_Calibration+0x140>)
3000348c:	4b16      	ldr	r3, [pc, #88]	; (300034e8 <FLASH_Calibration+0x144>)
3000348e:	7901      	ldrb	r1, [r0, #4]
30003490:	4798      	blx	r3
30003492:	9a03      	ldr	r2, [sp, #12]
30003494:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30003498:	9800      	ldr	r0, [sp, #0]
3000349a:	615a      	str	r2, [r3, #20]
3000349c:	9a04      	ldr	r2, [sp, #16]
3000349e:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300034a2:	b007      	add	sp, #28
300034a4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034a8:	480e      	ldr	r0, [pc, #56]	; (300034e4 <FLASH_Calibration+0x140>)
300034aa:	4b12      	ldr	r3, [pc, #72]	; (300034f4 <FLASH_Calibration+0x150>)
300034ac:	4798      	blx	r3
300034ae:	e796      	b.n	300033de <FLASH_Calibration+0x3a>
300034b0:	480c      	ldr	r0, [pc, #48]	; (300034e4 <FLASH_Calibration+0x140>)
300034b2:	4b10      	ldr	r3, [pc, #64]	; (300034f4 <FLASH_Calibration+0x150>)
300034b4:	7901      	ldrb	r1, [r0, #4]
300034b6:	4798      	blx	r3
300034b8:	e7eb      	b.n	30003492 <FLASH_Calibration+0xee>
300034ba:	4b0f      	ldr	r3, [pc, #60]	; (300034f8 <FLASH_Calibration+0x154>)
300034bc:	681b      	ldr	r3, [r3, #0]
300034be:	039b      	lsls	r3, r3, #14
300034c0:	d5e0      	bpl.n	30003484 <FLASH_Calibration+0xe0>
300034c2:	9a02      	ldr	r2, [sp, #8]
300034c4:	480d      	ldr	r0, [pc, #52]	; (300034fc <FLASH_Calibration+0x158>)
300034c6:	f008 fe77 	bl	3000c1b8 <__DiagPrintf_veneer>
300034ca:	e7db      	b.n	30003484 <FLASH_Calibration+0xe0>
300034cc:	4652      	mov	r2, sl
300034ce:	9b02      	ldr	r3, [sp, #8]
300034d0:	480b      	ldr	r0, [pc, #44]	; (30003500 <FLASH_Calibration+0x15c>)
300034d2:	9105      	str	r1, [sp, #20]
300034d4:	f008 fe70 	bl	3000c1b8 <__DiagPrintf_veneer>
300034d8:	9905      	ldr	r1, [sp, #20]
300034da:	e7b7      	b.n	3000344c <FLASH_Calibration+0xa8>
300034dc:	9600      	str	r6, [sp, #0]
300034de:	e7d1      	b.n	30003484 <FLASH_Calibration+0xe0>
300034e0:	3000a501 	.word	0x3000a501
300034e4:	2001c01c 	.word	0x2001c01c
300034e8:	00009c99 	.word	0x00009c99
300034ec:	e000ed00 	.word	0xe000ed00
300034f0:	3000a311 	.word	0x3000a311
300034f4:	0000b7e9 	.word	0x0000b7e9
300034f8:	2001c00c 	.word	0x2001c00c
300034fc:	3000c314 	.word	0x3000c314
30003500:	3000c2e4 	.word	0x3000c2e4
30003504:	00054060 	.word	0x00054060

30003508 <_flash_calibration_highspeed>:
30003508:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000350c:	2400      	movs	r4, #0
3000350e:	b085      	sub	sp, #20
30003510:	4d24      	ldr	r5, [pc, #144]	; (300035a4 <_flash_calibration_highspeed+0x9c>)
30003512:	f04f 0801 	mov.w	r8, #1
30003516:	46a3      	mov	fp, r4
30003518:	4626      	mov	r6, r4
3000351a:	46a1      	mov	r9, r4
3000351c:	460a      	mov	r2, r1
3000351e:	4627      	mov	r7, r4
30003520:	4682      	mov	sl, r0
30003522:	b2e0      	uxtb	r0, r4
30003524:	2301      	movs	r3, #1
30003526:	4651      	mov	r1, sl
30003528:	9203      	str	r2, [sp, #12]
3000352a:	e9cd 0800 	strd	r0, r8, [sp]
3000352e:	4628      	mov	r0, r5
30003530:	f7ff fe22 	bl	30003178 <FLASH_CalibrationNew>
30003534:	f895 3061 	ldrb.w	r3, [r5, #97]	; 0x61
30003538:	b2c0      	uxtb	r0, r0
3000353a:	9a03      	ldr	r2, [sp, #12]
3000353c:	b10b      	cbz	r3, 30003542 <_flash_calibration_highspeed+0x3a>
3000353e:	2802      	cmp	r0, #2
30003540:	d80f      	bhi.n	30003562 <_flash_calibration_highspeed+0x5a>
30003542:	42b0      	cmp	r0, r6
30003544:	d811      	bhi.n	3000356a <_flash_calibration_highspeed+0x62>
30003546:	b106      	cbz	r6, 3000354a <_flash_calibration_highspeed+0x42>
30003548:	b198      	cbz	r0, 30003572 <_flash_calibration_highspeed+0x6a>
3000354a:	3401      	adds	r4, #1
3000354c:	f885 9061 	strb.w	r9, [r5, #97]	; 0x61
30003550:	2c05      	cmp	r4, #5
30003552:	d1e6      	bne.n	30003522 <_flash_calibration_highspeed+0x1a>
30003554:	46ba      	mov	sl, r7
30003556:	b96e      	cbnz	r6, 30003574 <_flash_calibration_highspeed+0x6c>
30003558:	4630      	mov	r0, r6
3000355a:	776e      	strb	r6, [r5, #29]
3000355c:	b005      	add	sp, #20
3000355e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003562:	42b0      	cmp	r0, r6
30003564:	f043 0301 	orr.w	r3, r3, #1
30003568:	d9ef      	bls.n	3000354a <_flash_calibration_highspeed+0x42>
3000356a:	469b      	mov	fp, r3
3000356c:	4627      	mov	r7, r4
3000356e:	4606      	mov	r6, r0
30003570:	e7eb      	b.n	3000354a <_flash_calibration_highspeed+0x42>
30003572:	46ba      	mov	sl, r7
30003574:	4b0c      	ldr	r3, [pc, #48]	; (300035a8 <_flash_calibration_highspeed+0xa0>)
30003576:	689b      	ldr	r3, [r3, #8]
30003578:	079b      	lsls	r3, r3, #30
3000357a:	d40b      	bmi.n	30003594 <_flash_calibration_highspeed+0x8c>
3000357c:	fa5f fa8a 	uxtb.w	sl, sl
30003580:	2001      	movs	r0, #1
30003582:	f885 b061 	strb.w	fp, [r5, #97]	; 0x61
30003586:	f885 a01c 	strb.w	sl, [r5, #28]
3000358a:	f885 a01d 	strb.w	sl, [r5, #29]
3000358e:	b005      	add	sp, #20
30003590:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003594:	465b      	mov	r3, fp
30003596:	4632      	mov	r2, r6
30003598:	4651      	mov	r1, sl
3000359a:	4804      	ldr	r0, [pc, #16]	; (300035ac <_flash_calibration_highspeed+0xa4>)
3000359c:	f008 fe0c 	bl	3000c1b8 <__DiagPrintf_veneer>
300035a0:	e7ec      	b.n	3000357c <_flash_calibration_highspeed+0x74>
300035a2:	bf00      	nop
300035a4:	2001c01c 	.word	0x2001c01c
300035a8:	2001c00c 	.word	0x2001c00c
300035ac:	3000c340 	.word	0x3000c340

300035b0 <flash_calibration_highspeed>:
300035b0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
300035b2:	4d20      	ldr	r5, [pc, #128]	; (30003634 <flash_calibration_highspeed+0x84>)
300035b4:	4604      	mov	r4, r0
300035b6:	2700      	movs	r7, #0
300035b8:	792e      	ldrb	r6, [r5, #4]
300035ba:	f885 7060 	strb.w	r7, [r5, #96]	; 0x60
300035be:	f7ff fd47 	bl	30003050 <FLASH_ClockDiv>
300035c2:	4621      	mov	r1, r4
300035c4:	4630      	mov	r0, r6
300035c6:	f7ff ff9f 	bl	30003508 <_flash_calibration_highspeed>
300035ca:	2801      	cmp	r0, #1
300035cc:	d00e      	beq.n	300035ec <flash_calibration_highspeed+0x3c>
300035ce:	2001      	movs	r0, #1
300035d0:	4b19      	ldr	r3, [pc, #100]	; (30003638 <flash_calibration_highspeed+0x88>)
300035d2:	4798      	blx	r3
300035d4:	4b19      	ldr	r3, [pc, #100]	; (3000363c <flash_calibration_highspeed+0x8c>)
300035d6:	6898      	ldr	r0, [r3, #8]
300035d8:	f010 0002 	ands.w	r0, r0, #2
300035dc:	d100      	bne.n	300035e0 <flash_calibration_highspeed+0x30>
300035de:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
300035e0:	4621      	mov	r1, r4
300035e2:	4817      	ldr	r0, [pc, #92]	; (30003640 <flash_calibration_highspeed+0x90>)
300035e4:	f008 fde8 	bl	3000c1b8 <__DiagPrintf_veneer>
300035e8:	4638      	mov	r0, r7
300035ea:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
300035ec:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300035f0:	f895 0061 	ldrb.w	r0, [r5, #97]	; 0x61
300035f4:	6c13      	ldr	r3, [r2, #64]	; 0x40
300035f6:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
300035fa:	6413      	str	r3, [r2, #64]	; 0x40
300035fc:	f7ff fd70 	bl	300030e0 <FLASH_CalibrationPhaseIdx>
30003600:	4b10      	ldr	r3, [pc, #64]	; (30003644 <flash_calibration_highspeed+0x94>)
30003602:	4798      	blx	r3
30003604:	b940      	cbnz	r0, 30003618 <flash_calibration_highspeed+0x68>
30003606:	4b10      	ldr	r3, [pc, #64]	; (30003648 <flash_calibration_highspeed+0x98>)
30003608:	4630      	mov	r0, r6
3000360a:	4798      	blx	r3
3000360c:	4b0b      	ldr	r3, [pc, #44]	; (3000363c <flash_calibration_highspeed+0x8c>)
3000360e:	689b      	ldr	r3, [r3, #8]
30003610:	079b      	lsls	r3, r3, #30
30003612:	d408      	bmi.n	30003626 <flash_calibration_highspeed+0x76>
30003614:	2001      	movs	r0, #1
30003616:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003618:	4b0c      	ldr	r3, [pc, #48]	; (3000364c <flash_calibration_highspeed+0x9c>)
3000361a:	4630      	mov	r0, r6
3000361c:	4798      	blx	r3
3000361e:	4b07      	ldr	r3, [pc, #28]	; (3000363c <flash_calibration_highspeed+0x8c>)
30003620:	689b      	ldr	r3, [r3, #8]
30003622:	079b      	lsls	r3, r3, #30
30003624:	d5f6      	bpl.n	30003614 <flash_calibration_highspeed+0x64>
30003626:	4621      	mov	r1, r4
30003628:	4809      	ldr	r0, [pc, #36]	; (30003650 <flash_calibration_highspeed+0xa0>)
3000362a:	f008 fdc5 	bl	3000c1b8 <__DiagPrintf_veneer>
3000362e:	2001      	movs	r0, #1
30003630:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003632:	bf00      	nop
30003634:	2001c01c 	.word	0x2001c01c
30003638:	0000b569 	.word	0x0000b569
3000363c:	2001c00c 	.word	0x2001c00c
30003640:	3000c3a8 	.word	0x3000c3a8
30003644:	3000a501 	.word	0x3000a501
30003648:	0000c045 	.word	0x0000c045
3000364c:	0000a53d 	.word	0x0000a53d
30003650:	3000c378 	.word	0x3000c378

30003654 <flash_rx_mode_switch>:
30003654:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003658:	2300      	movs	r3, #0
3000365a:	b089      	sub	sp, #36	; 0x24
3000365c:	4d51      	ldr	r5, [pc, #324]	; (300037a4 <flash_rx_mode_switch+0x150>)
3000365e:	4684      	mov	ip, r0
30003660:	ac03      	add	r4, sp, #12
30003662:	f88d 3003 	strb.w	r3, [sp, #3]
30003666:	f1bc 0f04 	cmp.w	ip, #4
3000366a:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
3000366c:	c40f      	stmia	r4!, {r0, r1, r2, r3}
3000366e:	682b      	ldr	r3, [r5, #0]
30003670:	6023      	str	r3, [r4, #0]
30003672:	d876      	bhi.n	30003762 <flash_rx_mode_switch+0x10e>
30003674:	fa4f f08c 	sxtb.w	r0, ip
30003678:	ab03      	add	r3, sp, #12
3000367a:	4e4b      	ldr	r6, [pc, #300]	; (300037a8 <flash_rx_mode_switch+0x154>)
3000367c:	f1c0 0404 	rsb	r4, r0, #4
30003680:	f1c0 0022 	rsb	r0, r0, #34	; 0x22
30003684:	f8df a148 	ldr.w	sl, [pc, #328]	; 300037d0 <flash_rx_mode_switch+0x17c>
30003688:	b267      	sxtb	r7, r4
3000368a:	fa46 f580 	sxtab	r5, r6, r0
3000368e:	b2e4      	uxtb	r4, r4
30003690:	f8df 9140 	ldr.w	r9, [pc, #320]	; 300037d4 <flash_rx_mode_switch+0x180>
30003694:	eb03 0787 	add.w	r7, r3, r7, lsl #2
30003698:	f8df 813c 	ldr.w	r8, [pc, #316]	; 300037d8 <flash_rx_mode_switch+0x184>
3000369c:	f8df b13c 	ldr.w	fp, [pc, #316]	; 300037dc <flash_rx_mode_switch+0x188>
300036a0:	e035      	b.n	3000370e <flash_rx_mode_switch+0xba>
300036a2:	2c04      	cmp	r4, #4
300036a4:	d03b      	beq.n	3000371e <flash_rx_mode_switch+0xca>
300036a6:	2c02      	cmp	r4, #2
300036a8:	d039      	beq.n	3000371e <flash_rx_mode_switch+0xca>
300036aa:	6833      	ldr	r3, [r6, #0]
300036ac:	2b03      	cmp	r3, #3
300036ae:	d03c      	beq.n	3000372a <flash_rx_mode_switch+0xd6>
300036b0:	2300      	movs	r3, #0
300036b2:	4620      	mov	r0, r4
300036b4:	7773      	strb	r3, [r6, #29]
300036b6:	47d8      	blx	fp
300036b8:	4b3c      	ldr	r3, [pc, #240]	; (300037ac <flash_rx_mode_switch+0x158>)
300036ba:	695a      	ldr	r2, [r3, #20]
300036bc:	03d1      	lsls	r1, r2, #15
300036be:	d509      	bpl.n	300036d4 <flash_rx_mode_switch+0x80>
300036c0:	f3bf 8f4f 	dsb	sy
300036c4:	f04f 6200 	mov.w	r2, #134217728	; 0x8000000
300036c8:	f8c3 225c 	str.w	r2, [r3, #604]	; 0x25c
300036cc:	f3bf 8f4f 	dsb	sy
300036d0:	f3bf 8f6f 	isb	sy
300036d4:	47c8      	blx	r9
300036d6:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
300036da:	4935      	ldr	r1, [pc, #212]	; (300037b0 <flash_rx_mode_switch+0x15c>)
300036dc:	a801      	add	r0, sp, #4
300036de:	681a      	ldr	r2, [r3, #0]
300036e0:	9201      	str	r2, [sp, #4]
300036e2:	2208      	movs	r2, #8
300036e4:	685b      	ldr	r3, [r3, #4]
300036e6:	9302      	str	r3, [sp, #8]
300036e8:	47c0      	blx	r8
300036ea:	2800      	cmp	r0, #0
300036ec:	d048      	beq.n	30003780 <flash_rx_mode_switch+0x12c>
300036ee:	f896 3060 	ldrb.w	r3, [r6, #96]	; 0x60
300036f2:	b11b      	cbz	r3, 300036fc <flash_rx_mode_switch+0xa8>
300036f4:	4b2f      	ldr	r3, [pc, #188]	; (300037b4 <flash_rx_mode_switch+0x160>)
300036f6:	689b      	ldr	r3, [r3, #8]
300036f8:	079b      	lsls	r3, r3, #30
300036fa:	d42c      	bmi.n	30003756 <flash_rx_mode_switch+0x102>
300036fc:	1e62      	subs	r2, r4, #1
300036fe:	f1c4 0305 	rsb	r3, r4, #5
30003702:	3f04      	subs	r7, #4
30003704:	3d01      	subs	r5, #1
30003706:	b2d4      	uxtb	r4, r2
30003708:	b2db      	uxtb	r3, r3
3000370a:	2cff      	cmp	r4, #255	; 0xff
3000370c:	d028      	beq.n	30003760 <flash_rx_mode_switch+0x10c>
3000370e:	47d0      	blx	sl
30003710:	2800      	cmp	r0, #0
30003712:	d1c6      	bne.n	300036a2 <flash_rx_mode_switch+0x4e>
30003714:	7770      	strb	r0, [r6, #29]
30003716:	4620      	mov	r0, r4
30003718:	4b27      	ldr	r3, [pc, #156]	; (300037b8 <flash_rx_mode_switch+0x164>)
3000371a:	4798      	blx	r3
3000371c:	e7cc      	b.n	300036b8 <flash_rx_mode_switch+0x64>
3000371e:	2301      	movs	r3, #1
30003720:	f886 3063 	strb.w	r3, [r6, #99]	; 0x63
30003724:	6833      	ldr	r3, [r6, #0]
30003726:	2b03      	cmp	r3, #3
30003728:	d1c2      	bne.n	300036b0 <flash_rx_mode_switch+0x5c>
3000372a:	f10d 0203 	add.w	r2, sp, #3
3000372e:	2101      	movs	r1, #1
30003730:	2085      	movs	r0, #133	; 0x85
30003732:	4b22      	ldr	r3, [pc, #136]	; (300037bc <flash_rx_mode_switch+0x168>)
30003734:	4798      	blx	r3
30003736:	f89d 3003 	ldrb.w	r3, [sp, #3]
3000373a:	7829      	ldrb	r1, [r5, #0]
3000373c:	f10d 0203 	add.w	r2, sp, #3
30003740:	f003 030f 	and.w	r3, r3, #15
30003744:	2081      	movs	r0, #129	; 0x81
30003746:	ea43 1301 	orr.w	r3, r3, r1, lsl #4
3000374a:	2101      	movs	r1, #1
3000374c:	f88d 3003 	strb.w	r3, [sp, #3]
30003750:	4b1b      	ldr	r3, [pc, #108]	; (300037c0 <flash_rx_mode_switch+0x16c>)
30003752:	4798      	blx	r3
30003754:	e7ac      	b.n	300036b0 <flash_rx_mode_switch+0x5c>
30003756:	6839      	ldr	r1, [r7, #0]
30003758:	481a      	ldr	r0, [pc, #104]	; (300037c4 <flash_rx_mode_switch+0x170>)
3000375a:	f008 fd2d 	bl	3000c1b8 <__DiagPrintf_veneer>
3000375e:	e7cd      	b.n	300036fc <flash_rx_mode_switch+0xa8>
30003760:	469c      	mov	ip, r3
30003762:	f1bc 0f05 	cmp.w	ip, #5
30003766:	d10f      	bne.n	30003788 <flash_rx_mode_switch+0x134>
30003768:	4b12      	ldr	r3, [pc, #72]	; (300037b4 <flash_rx_mode_switch+0x160>)
3000376a:	6818      	ldr	r0, [r3, #0]
3000376c:	f010 0002 	ands.w	r0, r0, #2
30003770:	d00b      	beq.n	3000378a <flash_rx_mode_switch+0x136>
30003772:	4815      	ldr	r0, [pc, #84]	; (300037c8 <flash_rx_mode_switch+0x174>)
30003774:	f008 fd20 	bl	3000c1b8 <__DiagPrintf_veneer>
30003778:	2000      	movs	r0, #0
3000377a:	b009      	add	sp, #36	; 0x24
3000377c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003780:	4b0c      	ldr	r3, [pc, #48]	; (300037b4 <flash_rx_mode_switch+0x160>)
30003782:	689b      	ldr	r3, [r3, #8]
30003784:	079a      	lsls	r2, r3, #30
30003786:	d403      	bmi.n	30003790 <flash_rx_mode_switch+0x13c>
30003788:	2001      	movs	r0, #1
3000378a:	b009      	add	sp, #36	; 0x24
3000378c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003790:	ab08      	add	r3, sp, #32
30003792:	480e      	ldr	r0, [pc, #56]	; (300037cc <flash_rx_mode_switch+0x178>)
30003794:	eb03 0484 	add.w	r4, r3, r4, lsl #2
30003798:	f854 1c14 	ldr.w	r1, [r4, #-20]
3000379c:	f008 fd0c 	bl	3000c1b8 <__DiagPrintf_veneer>
300037a0:	e7f2      	b.n	30003788 <flash_rx_mode_switch+0x134>
300037a2:	bf00      	nop
300037a4:	3000d368 	.word	0x3000d368
300037a8:	2001c01c 	.word	0x2001c01c
300037ac:	e000ed00 	.word	0xe000ed00
300037b0:	00054060 	.word	0x00054060
300037b4:	2001c00c 	.word	0x2001c00c
300037b8:	0000c045 	.word	0x0000c045
300037bc:	00009db5 	.word	0x00009db5
300037c0:	0000a149 	.word	0x0000a149
300037c4:	3000c404 	.word	0x3000c404
300037c8:	3000c434 	.word	0x3000c434
300037cc:	3000c3dc 	.word	0x3000c3dc
300037d0:	3000a501 	.word	0x3000a501
300037d4:	3000a311 	.word	0x3000a311
300037d8:	000129bd 	.word	0x000129bd
300037dc:	0000a53d 	.word	0x0000a53d

300037e0 <flash_highspeed_setup>:
300037e0:	4aaa      	ldr	r2, [pc, #680]	; (30003a8c <flash_highspeed_setup+0x2ac>)
300037e2:	f44f 4300 	mov.w	r3, #32768	; 0x8000
300037e6:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300037ea:	2500      	movs	r5, #0
300037ec:	b084      	sub	sp, #16
300037ee:	8811      	ldrh	r1, [r2, #0]
300037f0:	4219      	tst	r1, r3
300037f2:	f105 0201 	add.w	r2, r5, #1
300037f6:	ea4f 0353 	mov.w	r3, r3, lsr #1
300037fa:	d102      	bne.n	30003802 <flash_highspeed_setup+0x22>
300037fc:	b2d5      	uxtb	r5, r2
300037fe:	2d10      	cmp	r5, #16
30003800:	d1f6      	bne.n	300037f0 <flash_highspeed_setup+0x10>
30003802:	4aa3      	ldr	r2, [pc, #652]	; (30003a90 <flash_highspeed_setup+0x2b0>)
30003804:	2400      	movs	r4, #0
30003806:	f44f 4300 	mov.w	r3, #32768	; 0x8000
3000380a:	8811      	ldrh	r1, [r2, #0]
3000380c:	4219      	tst	r1, r3
3000380e:	f104 0201 	add.w	r2, r4, #1
30003812:	ea4f 0353 	mov.w	r3, r3, lsr #1
30003816:	d102      	bne.n	3000381e <flash_highspeed_setup+0x3e>
30003818:	b2d4      	uxtb	r4, r2
3000381a:	2c10      	cmp	r4, #16
3000381c:	d1f6      	bne.n	3000380c <flash_highspeed_setup+0x2c>
3000381e:	b672      	cpsid	i
30003820:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30003824:	f8df a2b4 	ldr.w	sl, [pc, #692]	; 30003adc <flash_highspeed_setup+0x2fc>
30003828:	4e9a      	ldr	r6, [pc, #616]	; (30003a94 <flash_highspeed_setup+0x2b4>)
3000382a:	6813      	ldr	r3, [r2, #0]
3000382c:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30003830:	6013      	str	r3, [r2, #0]
30003832:	47d0      	blx	sl
30003834:	2800      	cmp	r0, #0
30003836:	d03a      	beq.n	300038ae <flash_highspeed_setup+0xce>
30003838:	4f97      	ldr	r7, [pc, #604]	; (30003a98 <flash_highspeed_setup+0x2b8>)
3000383a:	2103      	movs	r1, #3
3000383c:	f896 0055 	ldrb.w	r0, [r6, #85]	; 0x55
30003840:	aa03      	add	r2, sp, #12
30003842:	f8df 829c 	ldr.w	r8, [pc, #668]	; 30003ae0 <flash_highspeed_setup+0x300>
30003846:	47c0      	blx	r8
30003848:	68bb      	ldr	r3, [r7, #8]
3000384a:	0798      	lsls	r0, r3, #30
3000384c:	f100 80f6 	bmi.w	30003a3c <flash_highspeed_setup+0x25c>
30003850:	f89d 900d 	ldrb.w	r9, [sp, #13]
30003854:	f89d 300e 	ldrb.w	r3, [sp, #14]
30003858:	ea4f 2909 	mov.w	r9, r9, lsl #8
3000385c:	ea49 4903 	orr.w	r9, r9, r3, lsl #16
30003860:	f89d 300c 	ldrb.w	r3, [sp, #12]
30003864:	ea49 0903 	orr.w	r9, r9, r3
30003868:	47d0      	blx	sl
3000386a:	4b8c      	ldr	r3, [pc, #560]	; (30003a9c <flash_highspeed_setup+0x2bc>)
3000386c:	4a8c      	ldr	r2, [pc, #560]	; (30003aa0 <flash_highspeed_setup+0x2c0>)
3000386e:	2800      	cmp	r0, #0
30003870:	bf08      	it	eq
30003872:	4613      	moveq	r3, r2
30003874:	e007      	b.n	30003886 <flash_highspeed_setup+0xa6>
30003876:	e9d3 1200 	ldrd	r1, r2, [r3]
3000387a:	4618      	mov	r0, r3
3000387c:	3314      	adds	r3, #20
3000387e:	ea09 0202 	and.w	r2, r9, r2
30003882:	428a      	cmp	r2, r1
30003884:	d075      	beq.n	30003972 <flash_highspeed_setup+0x192>
30003886:	7a1a      	ldrb	r2, [r3, #8]
30003888:	2aff      	cmp	r2, #255	; 0xff
3000388a:	d1f4      	bne.n	30003876 <flash_highspeed_setup+0x96>
3000388c:	f8df 9254 	ldr.w	r9, [pc, #596]	; 30003ae4 <flash_highspeed_setup+0x304>
30003890:	2300      	movs	r3, #0
30003892:	f8c9 3000 	str.w	r3, [r9]
30003896:	683b      	ldr	r3, [r7, #0]
30003898:	0399      	lsls	r1, r3, #14
3000389a:	f100 8175 	bmi.w	30003b88 <flash_highspeed_setup+0x3a8>
3000389e:	4881      	ldr	r0, [pc, #516]	; (30003aa4 <flash_highspeed_setup+0x2c4>)
300038a0:	f44f 710b 	mov.w	r1, #556	; 0x22c
300038a4:	f008 fca8 	bl	3000c1f8 <__io_assert_failed_veneer>
300038a8:	f8d9 0000 	ldr.w	r0, [r9]
300038ac:	e067      	b.n	3000397e <flash_highspeed_setup+0x19e>
300038ae:	ab03      	add	r3, sp, #12
300038b0:	4602      	mov	r2, r0
300038b2:	4601      	mov	r1, r0
300038b4:	4f7c      	ldr	r7, [pc, #496]	; (30003aa8 <flash_highspeed_setup+0x2c8>)
300038b6:	9300      	str	r3, [sp, #0]
300038b8:	2302      	movs	r3, #2
300038ba:	f896 0055 	ldrb.w	r0, [r6, #85]	; 0x55
300038be:	47b8      	blx	r7
300038c0:	4f75      	ldr	r7, [pc, #468]	; (30003a98 <flash_highspeed_setup+0x2b8>)
300038c2:	68bb      	ldr	r3, [r7, #8]
300038c4:	079a      	lsls	r2, r3, #30
300038c6:	f100 80c3 	bmi.w	30003a50 <flash_highspeed_setup+0x270>
300038ca:	f89d 800d 	ldrb.w	r8, [sp, #13]
300038ce:	f89d 300e 	ldrb.w	r3, [sp, #14]
300038d2:	ea4f 2808 	mov.w	r8, r8, lsl #8
300038d6:	ea48 4803 	orr.w	r8, r8, r3, lsl #16
300038da:	f89d 300c 	ldrb.w	r3, [sp, #12]
300038de:	ea48 0803 	orr.w	r8, r8, r3
300038e2:	47d0      	blx	sl
300038e4:	4b6d      	ldr	r3, [pc, #436]	; (30003a9c <flash_highspeed_setup+0x2bc>)
300038e6:	4a6e      	ldr	r2, [pc, #440]	; (30003aa0 <flash_highspeed_setup+0x2c0>)
300038e8:	2800      	cmp	r0, #0
300038ea:	bf08      	it	eq
300038ec:	4613      	moveq	r3, r2
300038ee:	e008      	b.n	30003902 <flash_highspeed_setup+0x122>
300038f0:	e9d3 1200 	ldrd	r1, r2, [r3]
300038f4:	4618      	mov	r0, r3
300038f6:	3314      	adds	r3, #20
300038f8:	ea08 0202 	and.w	r2, r8, r2
300038fc:	428a      	cmp	r2, r1
300038fe:	f000 8084 	beq.w	30003a0a <flash_highspeed_setup+0x22a>
30003902:	7a1a      	ldrb	r2, [r3, #8]
30003904:	2aff      	cmp	r2, #255	; 0xff
30003906:	d1f3      	bne.n	300038f0 <flash_highspeed_setup+0x110>
30003908:	f8df 91d8 	ldr.w	r9, [pc, #472]	; 30003ae4 <flash_highspeed_setup+0x304>
3000390c:	2300      	movs	r3, #0
3000390e:	f8c9 3000 	str.w	r3, [r9]
30003912:	683b      	ldr	r3, [r7, #0]
30003914:	039b      	lsls	r3, r3, #14
30003916:	f100 813b 	bmi.w	30003b90 <flash_highspeed_setup+0x3b0>
3000391a:	4864      	ldr	r0, [pc, #400]	; (30003aac <flash_highspeed_setup+0x2cc>)
3000391c:	f240 2163 	movw	r1, #611	; 0x263
30003920:	f008 fc6a 	bl	3000c1f8 <__io_assert_failed_veneer>
30003924:	f8d9 0000 	ldr.w	r0, [r9]
30003928:	7a03      	ldrb	r3, [r0, #8]
3000392a:	2b04      	cmp	r3, #4
3000392c:	d977      	bls.n	30003a1e <flash_highspeed_setup+0x23e>
3000392e:	2bfe      	cmp	r3, #254	; 0xfe
30003930:	d104      	bne.n	3000393c <flash_highspeed_setup+0x15c>
30003932:	6903      	ldr	r3, [r0, #16]
30003934:	2b00      	cmp	r3, #0
30003936:	f000 8115 	beq.w	30003b64 <flash_highspeed_setup+0x384>
3000393a:	4798      	blx	r3
3000393c:	68f3      	ldr	r3, [r6, #12]
3000393e:	2b00      	cmp	r3, #0
30003940:	f040 80fa 	bne.w	30003b38 <flash_highspeed_setup+0x358>
30003944:	4e5a      	ldr	r6, [pc, #360]	; (30003ab0 <flash_highspeed_setup+0x2d0>)
30003946:	2100      	movs	r1, #0
30003948:	20a0      	movs	r0, #160	; 0xa0
3000394a:	47b0      	blx	r6
3000394c:	4628      	mov	r0, r5
3000394e:	f7ff fe81 	bl	30003654 <flash_rx_mode_switch>
30003952:	4b58      	ldr	r3, [pc, #352]	; (30003ab4 <flash_highspeed_setup+0x2d4>)
30003954:	4798      	blx	r3
30003956:	2801      	cmp	r0, #1
30003958:	d007      	beq.n	3000396a <flash_highspeed_setup+0x18a>
3000395a:	2c06      	cmp	r4, #6
3000395c:	bf28      	it	cs
3000395e:	2406      	movcs	r4, #6
30003960:	f1c4 0009 	rsb	r0, r4, #9
30003964:	b2c0      	uxtb	r0, r0
30003966:	f7ff fe23 	bl	300035b0 <flash_calibration_highspeed>
3000396a:	b662      	cpsie	i
3000396c:	b004      	add	sp, #16
3000396e:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003972:	f8df 9170 	ldr.w	r9, [pc, #368]	; 30003ae4 <flash_highspeed_setup+0x304>
30003976:	f8c9 0000 	str.w	r0, [r9]
3000397a:	2800      	cmp	r0, #0
3000397c:	d08b      	beq.n	30003896 <flash_highspeed_setup+0xb6>
3000397e:	7a03      	ldrb	r3, [r0, #8]
30003980:	2b05      	cmp	r3, #5
30003982:	d80b      	bhi.n	3000399c <flash_highspeed_setup+0x1bc>
30003984:	3b01      	subs	r3, #1
30003986:	4843      	ldr	r0, [pc, #268]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003988:	2b04      	cmp	r3, #4
3000398a:	f200 8105 	bhi.w	30003b98 <flash_highspeed_setup+0x3b8>
3000398e:	e8df f013 	tbh	[pc, r3, lsl #1]
30003992:	00b2      	.short	0x00b2
30003994:	00c600af 	.word	0x00c600af
30003998:	00bd00c3 	.word	0x00bd00c3
3000399c:	2bfe      	cmp	r3, #254	; 0xfe
3000399e:	d104      	bne.n	300039aa <flash_highspeed_setup+0x1ca>
300039a0:	6903      	ldr	r3, [r0, #16]
300039a2:	2b00      	cmp	r3, #0
300039a4:	f000 80e7 	beq.w	30003b76 <flash_highspeed_setup+0x396>
300039a8:	4798      	blx	r3
300039aa:	4b43      	ldr	r3, [pc, #268]	; (30003ab8 <flash_highspeed_setup+0x2d8>)
300039ac:	4798      	blx	r3
300039ae:	b110      	cbz	r0, 300039b6 <flash_highspeed_setup+0x1d6>
300039b0:	2304      	movs	r3, #4
300039b2:	f886 3062 	strb.w	r3, [r6, #98]	; 0x62
300039b6:	f8d9 3000 	ldr.w	r3, [r9]
300039ba:	aa03      	add	r2, sp, #12
300039bc:	2101      	movs	r1, #1
300039be:	f896 0056 	ldrb.w	r0, [r6, #86]	; 0x56
300039c2:	f8d3 900c 	ldr.w	r9, [r3, #12]
300039c6:	68f3      	ldr	r3, [r6, #12]
300039c8:	9302      	str	r3, [sp, #8]
300039ca:	47c0      	blx	r8
300039cc:	69b3      	ldr	r3, [r6, #24]
300039ce:	2b00      	cmp	r3, #0
300039d0:	f040 80aa 	bne.w	30003b28 <flash_highspeed_setup+0x348>
300039d4:	4b39      	ldr	r3, [pc, #228]	; (30003abc <flash_highspeed_setup+0x2dc>)
300039d6:	781b      	ldrb	r3, [r3, #0]
300039d8:	2b00      	cmp	r3, #0
300039da:	f040 80b8 	bne.w	30003b4e <flash_highspeed_setup+0x36e>
300039de:	2701      	movs	r7, #1
300039e0:	9b03      	ldr	r3, [sp, #12]
300039e2:	a803      	add	r0, sp, #12
300039e4:	463a      	mov	r2, r7
300039e6:	a902      	add	r1, sp, #8
300039e8:	ea03 0309 	and.w	r3, r3, r9
300039ec:	9303      	str	r3, [sp, #12]
300039ee:	4b34      	ldr	r3, [pc, #208]	; (30003ac0 <flash_highspeed_setup+0x2e0>)
300039f0:	4798      	blx	r3
300039f2:	2800      	cmp	r0, #0
300039f4:	d0aa      	beq.n	3000394c <flash_highspeed_setup+0x16c>
300039f6:	f896 3059 	ldrb.w	r3, [r6, #89]	; 0x59
300039fa:	aa02      	add	r2, sp, #8
300039fc:	f896 0058 	ldrb.w	r0, [r6, #88]	; 0x58
30003a00:	bb73      	cbnz	r3, 30003a60 <flash_highspeed_setup+0x280>
30003a02:	4639      	mov	r1, r7
30003a04:	4b2f      	ldr	r3, [pc, #188]	; (30003ac4 <flash_highspeed_setup+0x2e4>)
30003a06:	4798      	blx	r3
30003a08:	e7a0      	b.n	3000394c <flash_highspeed_setup+0x16c>
30003a0a:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 30003ae4 <flash_highspeed_setup+0x304>
30003a0e:	f8c9 0000 	str.w	r0, [r9]
30003a12:	2800      	cmp	r0, #0
30003a14:	f43f af7d 	beq.w	30003912 <flash_highspeed_setup+0x132>
30003a18:	7a03      	ldrb	r3, [r0, #8]
30003a1a:	2b04      	cmp	r3, #4
30003a1c:	d887      	bhi.n	3000392e <flash_highspeed_setup+0x14e>
30003a1e:	2b04      	cmp	r3, #4
30003a20:	d88c      	bhi.n	3000393c <flash_highspeed_setup+0x15c>
30003a22:	a201      	add	r2, pc, #4	; (adr r2, 30003a28 <flash_highspeed_setup+0x248>)
30003a24:	f852 f023 	ldr.w	pc, [r2, r3, lsl #2]
30003a28:	30003a85 	.word	0x30003a85
30003a2c:	30003ae9 	.word	0x30003ae9
30003a30:	30003a7d 	.word	0x30003a7d
30003a34:	3000393d 	.word	0x3000393d
30003a38:	30003a75 	.word	0x30003a75
30003a3c:	f89d 300e 	ldrb.w	r3, [sp, #14]
30003a40:	f89d 200d 	ldrb.w	r2, [sp, #13]
30003a44:	f89d 100c 	ldrb.w	r1, [sp, #12]
30003a48:	481f      	ldr	r0, [pc, #124]	; (30003ac8 <flash_highspeed_setup+0x2e8>)
30003a4a:	f008 fbb5 	bl	3000c1b8 <__DiagPrintf_veneer>
30003a4e:	e6ff      	b.n	30003850 <flash_highspeed_setup+0x70>
30003a50:	f89d 200d 	ldrb.w	r2, [sp, #13]
30003a54:	f89d 100c 	ldrb.w	r1, [sp, #12]
30003a58:	481c      	ldr	r0, [pc, #112]	; (30003acc <flash_highspeed_setup+0x2ec>)
30003a5a:	f008 fbad 	bl	3000c1b8 <__DiagPrintf_veneer>
30003a5e:	e734      	b.n	300038ca <flash_highspeed_setup+0xea>
30003a60:	2101      	movs	r1, #1
30003a62:	4f18      	ldr	r7, [pc, #96]	; (30003ac4 <flash_highspeed_setup+0x2e4>)
30003a64:	47b8      	blx	r7
30003a66:	f10d 0209 	add.w	r2, sp, #9
30003a6a:	f896 0059 	ldrb.w	r0, [r6, #89]	; 0x59
30003a6e:	2101      	movs	r1, #1
30003a70:	47b8      	blx	r7
30003a72:	e76b      	b.n	3000394c <flash_highspeed_setup+0x16c>
30003a74:	4807      	ldr	r0, [pc, #28]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003a76:	4b16      	ldr	r3, [pc, #88]	; (30003ad0 <flash_highspeed_setup+0x2f0>)
30003a78:	4798      	blx	r3
30003a7a:	e75f      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003a7c:	4805      	ldr	r0, [pc, #20]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003a7e:	4b15      	ldr	r3, [pc, #84]	; (30003ad4 <flash_highspeed_setup+0x2f4>)
30003a80:	4798      	blx	r3
30003a82:	e75b      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003a84:	4803      	ldr	r0, [pc, #12]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003a86:	4b14      	ldr	r3, [pc, #80]	; (30003ad8 <flash_highspeed_setup+0x2f8>)
30003a88:	4798      	blx	r3
30003a8a:	e757      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003a8c:	3000d2d8 	.word	0x3000d2d8
30003a90:	3000d2da 	.word	0x3000d2da
30003a94:	2001c01c 	.word	0x2001c01c
30003a98:	2001c00c 	.word	0x2001c00c
30003a9c:	3000d1d4 	.word	0x3000d1d4
30003aa0:	3000d2dc 	.word	0x3000d2dc
30003aa4:	3000d38c 	.word	0x3000d38c
30003aa8:	0000b8c1 	.word	0x0000b8c1
30003aac:	3000d37c 	.word	0x3000d37c
30003ab0:	0000bd0d 	.word	0x0000bd0d
30003ab4:	0000c0f9 	.word	0x0000c0f9
30003ab8:	0000c1d9 	.word	0x0000c1d9
30003abc:	3000eaa0 	.word	0x3000eaa0
30003ac0:	000129bd 	.word	0x000129bd
30003ac4:	0000a149 	.word	0x0000a149
30003ac8:	3000c46c 	.word	0x3000c46c
30003acc:	3000c4c0 	.word	0x3000c4c0
30003ad0:	0000be71 	.word	0x0000be71
30003ad4:	0000bee9 	.word	0x0000bee9
30003ad8:	0000bfcd 	.word	0x0000bfcd
30003adc:	3000a501 	.word	0x3000a501
30003ae0:	00009db5 	.word	0x00009db5
30003ae4:	3000eaa4 	.word	0x3000eaa4
30003ae8:	482d      	ldr	r0, [pc, #180]	; (30003ba0 <flash_highspeed_setup+0x3c0>)
30003aea:	4b2e      	ldr	r3, [pc, #184]	; (30003ba4 <flash_highspeed_setup+0x3c4>)
30003aec:	4798      	blx	r3
30003aee:	e725      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003af0:	4b2d      	ldr	r3, [pc, #180]	; (30003ba8 <flash_highspeed_setup+0x3c8>)
30003af2:	4798      	blx	r3
30003af4:	e759      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003af6:	4b2d      	ldr	r3, [pc, #180]	; (30003bac <flash_highspeed_setup+0x3cc>)
30003af8:	4798      	blx	r3
30003afa:	f89d 300e 	ldrb.w	r3, [sp, #14]
30003afe:	2b15      	cmp	r3, #21
30003b00:	f67f af53 	bls.w	300039aa <flash_highspeed_setup+0x1ca>
30003b04:	2331      	movs	r3, #49	; 0x31
30003b06:	f886 3059 	strb.w	r3, [r6, #89]	; 0x59
30003b0a:	e74e      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b0c:	4b26      	ldr	r3, [pc, #152]	; (30003ba8 <flash_highspeed_setup+0x3c8>)
30003b0e:	4798      	blx	r3
30003b10:	4b27      	ldr	r3, [pc, #156]	; (30003bb0 <flash_highspeed_setup+0x3d0>)
30003b12:	2201      	movs	r2, #1
30003b14:	701a      	strb	r2, [r3, #0]
30003b16:	e748      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b18:	4b26      	ldr	r3, [pc, #152]	; (30003bb4 <flash_highspeed_setup+0x3d4>)
30003b1a:	4798      	blx	r3
30003b1c:	e745      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b1e:	4b22      	ldr	r3, [pc, #136]	; (30003ba8 <flash_highspeed_setup+0x3c8>)
30003b20:	4798      	blx	r3
30003b22:	2300      	movs	r3, #0
30003b24:	60f3      	str	r3, [r6, #12]
30003b26:	e740      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b28:	f10d 020d 	add.w	r2, sp, #13
30003b2c:	2101      	movs	r1, #1
30003b2e:	f896 0057 	ldrb.w	r0, [r6, #87]	; 0x57
30003b32:	2702      	movs	r7, #2
30003b34:	47c0      	blx	r8
30003b36:	e753      	b.n	300039e0 <flash_highspeed_setup+0x200>
30003b38:	4b1f      	ldr	r3, [pc, #124]	; (30003bb8 <flash_highspeed_setup+0x3d8>)
30003b3a:	20b0      	movs	r0, #176	; 0xb0
30003b3c:	4798      	blx	r3
30003b3e:	4601      	mov	r1, r0
30003b40:	4e1e      	ldr	r6, [pc, #120]	; (30003bbc <flash_highspeed_setup+0x3dc>)
30003b42:	20b0      	movs	r0, #176	; 0xb0
30003b44:	f041 0101 	orr.w	r1, r1, #1
30003b48:	b2c9      	uxtb	r1, r1
30003b4a:	47b0      	blx	r6
30003b4c:	e6fb      	b.n	30003946 <flash_highspeed_setup+0x166>
30003b4e:	f10d 020d 	add.w	r2, sp, #13
30003b52:	2102      	movs	r1, #2
30003b54:	2015      	movs	r0, #21
30003b56:	2703      	movs	r7, #3
30003b58:	47c0      	blx	r8
30003b5a:	9b02      	ldr	r3, [sp, #8]
30003b5c:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
30003b60:	9302      	str	r3, [sp, #8]
30003b62:	e73d      	b.n	300039e0 <flash_highspeed_setup+0x200>
30003b64:	f240 2175 	movw	r1, #629	; 0x275
30003b68:	4815      	ldr	r0, [pc, #84]	; (30003bc0 <flash_highspeed_setup+0x3e0>)
30003b6a:	f008 fb45 	bl	3000c1f8 <__io_assert_failed_veneer>
30003b6e:	f8d9 3000 	ldr.w	r3, [r9]
30003b72:	691b      	ldr	r3, [r3, #16]
30003b74:	e6e1      	b.n	3000393a <flash_highspeed_setup+0x15a>
30003b76:	f240 2149 	movw	r1, #585	; 0x249
30003b7a:	4812      	ldr	r0, [pc, #72]	; (30003bc4 <flash_highspeed_setup+0x3e4>)
30003b7c:	f008 fb3c 	bl	3000c1f8 <__io_assert_failed_veneer>
30003b80:	f8d9 3000 	ldr.w	r3, [r9]
30003b84:	691b      	ldr	r3, [r3, #16]
30003b86:	e70f      	b.n	300039a8 <flash_highspeed_setup+0x1c8>
30003b88:	480f      	ldr	r0, [pc, #60]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b8a:	f008 fb15 	bl	3000c1b8 <__DiagPrintf_veneer>
30003b8e:	e686      	b.n	3000389e <flash_highspeed_setup+0xbe>
30003b90:	480d      	ldr	r0, [pc, #52]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b92:	f008 fb11 	bl	3000c1b8 <__DiagPrintf_veneer>
30003b96:	e6c0      	b.n	3000391a <flash_highspeed_setup+0x13a>
30003b98:	4b0c      	ldr	r3, [pc, #48]	; (30003bcc <flash_highspeed_setup+0x3ec>)
30003b9a:	4798      	blx	r3
30003b9c:	e705      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b9e:	bf00      	nop
30003ba0:	2001c01c 	.word	0x2001c01c
30003ba4:	0000bf59 	.word	0x0000bf59
30003ba8:	0000a3c5 	.word	0x0000a3c5
30003bac:	0000a441 	.word	0x0000a441
30003bb0:	3000eaa0 	.word	0x3000eaa0
30003bb4:	0000a345 	.word	0x0000a345
30003bb8:	0000b991 	.word	0x0000b991
30003bbc:	0000bd0d 	.word	0x0000bd0d
30003bc0:	3000d37c 	.word	0x3000d37c
30003bc4:	3000d38c 	.word	0x3000d38c
30003bc8:	3000c49c 	.word	0x3000c49c
30003bcc:	0000a4c1 	.word	0x0000a4c1

30003bd0 <BOOT_LoadSubImage>:
30003bd0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003bd4:	b08b      	sub	sp, #44	; 0x2c
30003bd6:	460f      	mov	r7, r1
30003bd8:	9300      	str	r3, [sp, #0]
30003bda:	f89d 3050 	ldrb.w	r3, [sp, #80]	; 0x50
30003bde:	9301      	str	r3, [sp, #4]
30003be0:	b34a      	cbz	r2, 30003c36 <BOOT_LoadSubImage+0x66>
30003be2:	4691      	mov	r9, r2
30003be4:	2600      	movs	r6, #0
30003be6:	f8df a14c 	ldr.w	sl, [pc, #332]	; 30003d34 <BOOT_LoadSubImage+0x164>
30003bea:	4605      	mov	r5, r0
30003bec:	2220      	movs	r2, #32
30003bee:	4639      	mov	r1, r7
30003bf0:	a802      	add	r0, sp, #8
30003bf2:	f002 f857 	bl	30005ca4 <BOOT_ImgCopy>
30003bf6:	9b02      	ldr	r3, [sp, #8]
30003bf8:	4553      	cmp	r3, sl
30003bfa:	d155      	bne.n	30003ca8 <BOOT_LoadSubImage+0xd8>
30003bfc:	9b03      	ldr	r3, [sp, #12]
30003bfe:	4a46      	ldr	r2, [pc, #280]	; (30003d18 <BOOT_LoadSubImage+0x148>)
30003c00:	4293      	cmp	r3, r2
30003c02:	d151      	bne.n	30003ca8 <BOOT_LoadSubImage+0xd8>
30003c04:	4b45      	ldr	r3, [pc, #276]	; (30003d1c <BOOT_LoadSubImage+0x14c>)
30003c06:	e9dd 2004 	ldrd	r2, r0, [sp, #16]
30003c0a:	18c3      	adds	r3, r0, r3
30003c0c:	f1a0 0b20 	sub.w	fp, r0, #32
30003c10:	f102 0820 	add.w	r8, r2, #32
30003c14:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003c18:	d213      	bcs.n	30003c42 <BOOT_LoadSubImage+0x72>
30003c1a:	b145      	cbz	r5, 30003c2e <BOOT_LoadSubImage+0x5e>
30003c1c:	1d2b      	adds	r3, r5, #4
30003c1e:	f845 b036 	str.w	fp, [r5, r6, lsl #3]
30003c22:	f843 8036 	str.w	r8, [r3, r6, lsl #3]
30003c26:	4b3e      	ldr	r3, [pc, #248]	; (30003d20 <BOOT_LoadSubImage+0x150>)
30003c28:	689b      	ldr	r3, [r3, #8]
30003c2a:	079b      	lsls	r3, r3, #30
30003c2c:	d433      	bmi.n	30003c96 <BOOT_LoadSubImage+0xc6>
30003c2e:	3601      	adds	r6, #1
30003c30:	4447      	add	r7, r8
30003c32:	454e      	cmp	r6, r9
30003c34:	d1da      	bne.n	30003bec <BOOT_LoadSubImage+0x1c>
30003c36:	4b3b      	ldr	r3, [pc, #236]	; (30003d24 <BOOT_LoadSubImage+0x154>)
30003c38:	2001      	movs	r0, #1
30003c3a:	601f      	str	r7, [r3, #0]
30003c3c:	b00b      	add	sp, #44	; 0x2c
30003c3e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003c42:	4c39      	ldr	r4, [pc, #228]	; (30003d28 <BOOT_LoadSubImage+0x158>)
30003c44:	4642      	mov	r2, r8
30003c46:	4639      	mov	r1, r7
30003c48:	4658      	mov	r0, fp
30003c4a:	f002 f82b 	bl	30005ca4 <BOOT_ImgCopy>
30003c4e:	6963      	ldr	r3, [r4, #20]
30003c50:	03da      	lsls	r2, r3, #15
30003c52:	d5e2      	bpl.n	30003c1a <BOOT_LoadSubImage+0x4a>
30003c54:	ea0b 0308 	and.w	r3, fp, r8
30003c58:	3301      	adds	r3, #1
30003c5a:	d02c      	beq.n	30003cb6 <BOOT_LoadSubImage+0xe6>
30003c5c:	f01b 031f 	ands.w	r3, fp, #31
30003c60:	d049      	beq.n	30003cf6 <BOOT_LoadSubImage+0x126>
30003c62:	4443      	add	r3, r8
30003c64:	f02b 021f 	bic.w	r2, fp, #31
30003c68:	f3bf 8f4f 	dsb	sy
30003c6c:	2b00      	cmp	r3, #0
30003c6e:	dd0d      	ble.n	30003c8c <BOOT_LoadSubImage+0xbc>
30003c70:	1e59      	subs	r1, r3, #1
30003c72:	f102 0320 	add.w	r3, r2, #32
30003c76:	482c      	ldr	r0, [pc, #176]	; (30003d28 <BOOT_LoadSubImage+0x158>)
30003c78:	f021 011f 	bic.w	r1, r1, #31
30003c7c:	4419      	add	r1, r3
30003c7e:	4299      	cmp	r1, r3
30003c80:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003c84:	461a      	mov	r2, r3
30003c86:	f103 0320 	add.w	r3, r3, #32
30003c8a:	d1f8      	bne.n	30003c7e <BOOT_LoadSubImage+0xae>
30003c8c:	f3bf 8f4f 	dsb	sy
30003c90:	f3bf 8f6f 	isb	sy
30003c94:	e7c1      	b.n	30003c1a <BOOT_LoadSubImage+0x4a>
30003c96:	9900      	ldr	r1, [sp, #0]
30003c98:	465a      	mov	r2, fp
30003c9a:	4643      	mov	r3, r8
30003c9c:	4823      	ldr	r0, [pc, #140]	; (30003d2c <BOOT_LoadSubImage+0x15c>)
30003c9e:	f851 1026 	ldr.w	r1, [r1, r6, lsl #2]
30003ca2:	f008 fa89 	bl	3000c1b8 <__DiagPrintf_veneer>
30003ca6:	e7c2      	b.n	30003c2e <BOOT_LoadSubImage+0x5e>
30003ca8:	9b01      	ldr	r3, [sp, #4]
30003caa:	2b01      	cmp	r3, #1
30003cac:	d026      	beq.n	30003cfc <BOOT_LoadSubImage+0x12c>
30003cae:	2000      	movs	r0, #0
30003cb0:	b00b      	add	sp, #44	; 0x2c
30003cb2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003cb6:	2300      	movs	r3, #0
30003cb8:	f8c4 3084 	str.w	r3, [r4, #132]	; 0x84
30003cbc:	f3bf 8f4f 	dsb	sy
30003cc0:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30003cc4:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003cc8:	f3c3 304e 	ubfx	r0, r3, #13, #15
30003ccc:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30003cd0:	0140      	lsls	r0, r0, #5
30003cd2:	ea00 010e 	and.w	r1, r0, lr
30003cd6:	4663      	mov	r3, ip
30003cd8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30003cdc:	3b01      	subs	r3, #1
30003cde:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30003ce2:	d2f9      	bcs.n	30003cd8 <BOOT_LoadSubImage+0x108>
30003ce4:	3820      	subs	r0, #32
30003ce6:	f110 0f20 	cmn.w	r0, #32
30003cea:	d1f2      	bne.n	30003cd2 <BOOT_LoadSubImage+0x102>
30003cec:	f3bf 8f4f 	dsb	sy
30003cf0:	f3bf 8f6f 	isb	sy
30003cf4:	e791      	b.n	30003c1a <BOOT_LoadSubImage+0x4a>
30003cf6:	4643      	mov	r3, r8
30003cf8:	465a      	mov	r2, fp
30003cfa:	e7b5      	b.n	30003c68 <BOOT_LoadSubImage+0x98>
30003cfc:	4b08      	ldr	r3, [pc, #32]	; (30003d20 <BOOT_LoadSubImage+0x150>)
30003cfe:	6898      	ldr	r0, [r3, #8]
30003d00:	f010 0002 	ands.w	r0, r0, #2
30003d04:	d0d4      	beq.n	30003cb0 <BOOT_LoadSubImage+0xe0>
30003d06:	9b00      	ldr	r3, [sp, #0]
30003d08:	4809      	ldr	r0, [pc, #36]	; (30003d30 <BOOT_LoadSubImage+0x160>)
30003d0a:	f853 1026 	ldr.w	r1, [r3, r6, lsl #2]
30003d0e:	f008 fa53 	bl	3000c1b8 <__DiagPrintf_veneer>
30003d12:	2000      	movs	r0, #0
30003d14:	e7cc      	b.n	30003cb0 <BOOT_LoadSubImage+0xe0>
30003d16:	bf00      	nop
30003d18:	31313738 	.word	0x31313738
30003d1c:	f7ffffe0 	.word	0xf7ffffe0
30003d20:	2001c00c 	.word	0x2001c00c
30003d24:	3000e2a4 	.word	0x3000e2a4
30003d28:	e000ed00 	.word	0xe000ed00
30003d2c:	3000c57c 	.word	0x3000c57c
30003d30:	3000c554 	.word	0x3000c554
30003d34:	35393138 	.word	0x35393138

30003d38 <BOOT_LoadAPSubImage>:
30003d38:	4baf      	ldr	r3, [pc, #700]	; (30003ff8 <BOOT_LoadAPSubImage+0x2c0>)
30003d3a:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30003d3e:	460c      	mov	r4, r1
30003d40:	4606      	mov	r6, r0
30003d42:	b08b      	sub	sp, #44	; 0x2c
30003d44:	4615      	mov	r5, r2
30003d46:	2220      	movs	r2, #32
30003d48:	e893 0003 	ldmia.w	r3, {r0, r1}
30003d4c:	0c0b      	lsrs	r3, r1, #16
30003d4e:	9000      	str	r0, [sp, #0]
30003d50:	f8ad 1004 	strh.w	r1, [sp, #4]
30003d54:	a802      	add	r0, sp, #8
30003d56:	4621      	mov	r1, r4
30003d58:	f88d 3006 	strb.w	r3, [sp, #6]
30003d5c:	f001 ffa2 	bl	30005ca4 <BOOT_ImgCopy>
30003d60:	4ba6      	ldr	r3, [pc, #664]	; (30003ffc <BOOT_LoadAPSubImage+0x2c4>)
30003d62:	9a02      	ldr	r2, [sp, #8]
30003d64:	429a      	cmp	r2, r3
30003d66:	d108      	bne.n	30003d7a <BOOT_LoadAPSubImage+0x42>
30003d68:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30003d6c:	9a03      	ldr	r2, [sp, #12]
30003d6e:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30003d72:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30003d76:	429a      	cmp	r2, r3
30003d78:	d010      	beq.n	30003d9c <BOOT_LoadAPSubImage+0x64>
30003d7a:	2d01      	cmp	r5, #1
30003d7c:	d003      	beq.n	30003d86 <BOOT_LoadAPSubImage+0x4e>
30003d7e:	2000      	movs	r0, #0
30003d80:	b00b      	add	sp, #44	; 0x2c
30003d82:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003d86:	4b9e      	ldr	r3, [pc, #632]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003d88:	6898      	ldr	r0, [r3, #8]
30003d8a:	f010 0002 	ands.w	r0, r0, #2
30003d8e:	d0f7      	beq.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d90:	4669      	mov	r1, sp
30003d92:	489c      	ldr	r0, [pc, #624]	; (30004004 <BOOT_LoadAPSubImage+0x2cc>)
30003d94:	f008 fa10 	bl	3000c1b8 <__DiagPrintf_veneer>
30003d98:	2000      	movs	r0, #0
30003d9a:	e7f1      	b.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d9c:	9a05      	ldr	r2, [sp, #20]
30003d9e:	4b9a      	ldr	r3, [pc, #616]	; (30004008 <BOOT_LoadAPSubImage+0x2d0>)
30003da0:	6ca7      	ldr	r7, [r4, #72]	; 0x48
30003da2:	f1a2 0820 	sub.w	r8, r2, #32
30003da6:	4413      	add	r3, r2
30003da8:	37d0      	adds	r7, #208	; 0xd0
30003daa:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003dae:	d23b      	bcs.n	30003e28 <BOOT_LoadAPSubImage+0xf0>
30003db0:	4b96      	ldr	r3, [pc, #600]	; (3000400c <BOOT_LoadAPSubImage+0x2d4>)
30003db2:	44b8      	add	r8, r7
30003db4:	f8d4 9070 	ldr.w	r9, [r4, #112]	; 0x70
30003db8:	4798      	blx	r3
30003dba:	2801      	cmp	r0, #1
30003dbc:	4427      	add	r7, r4
30003dbe:	d01e      	beq.n	30003dfe <BOOT_LoadAPSubImage+0xc6>
30003dc0:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30003dc4:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003dc8:	f080 8084 	bcs.w	30003ed4 <BOOT_LoadAPSubImage+0x19c>
30003dcc:	eb08 0509 	add.w	r5, r8, r9
30003dd0:	eb07 0109 	add.w	r1, r7, r9
30003dd4:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003dd8:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003ddc:	d24f      	bcs.n	30003e7e <BOOT_LoadAPSubImage+0x146>
30003dde:	b156      	cbz	r6, 30003df6 <BOOT_LoadAPSubImage+0xbe>
30003de0:	4987      	ldr	r1, [pc, #540]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003de2:	e9dd 3204 	ldrd	r3, r2, [sp, #16]
30003de6:	3a20      	subs	r2, #32
30003de8:	3320      	adds	r3, #32
30003dea:	e9c6 2300 	strd	r2, r3, [r6]
30003dee:	6889      	ldr	r1, [r1, #8]
30003df0:	0789      	lsls	r1, r1, #30
30003df2:	f100 80b6 	bmi.w	30003f62 <BOOT_LoadAPSubImage+0x22a>
30003df6:	2001      	movs	r0, #1
30003df8:	b00b      	add	sp, #44	; 0x2c
30003dfa:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003dfe:	4d80      	ldr	r5, [pc, #512]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003e00:	68ab      	ldr	r3, [r5, #8]
30003e02:	0799      	lsls	r1, r3, #30
30003e04:	f100 80b2 	bmi.w	30003f6c <BOOT_LoadAPSubImage+0x234>
30003e08:	464a      	mov	r2, r9
30003e0a:	4641      	mov	r1, r8
30003e0c:	4638      	mov	r0, r7
30003e0e:	f000 fc4b 	bl	300046a8 <BOOT_LoadBL32RDPImg>
30003e12:	2800      	cmp	r0, #0
30003e14:	d1da      	bne.n	30003dcc <BOOT_LoadAPSubImage+0x94>
30003e16:	682b      	ldr	r3, [r5, #0]
30003e18:	079a      	lsls	r2, r3, #30
30003e1a:	f100 80ca 	bmi.w	30003fb2 <BOOT_LoadAPSubImage+0x27a>
30003e1e:	4c7c      	ldr	r4, [pc, #496]	; (30004010 <BOOT_LoadAPSubImage+0x2d8>)
30003e20:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30003e24:	47a0      	blx	r4
30003e26:	e7fb      	b.n	30003e20 <BOOT_LoadAPSubImage+0xe8>
30003e28:	4d7a      	ldr	r5, [pc, #488]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003e2a:	4640      	mov	r0, r8
30003e2c:	463a      	mov	r2, r7
30003e2e:	4621      	mov	r1, r4
30003e30:	f001 ff38 	bl	30005ca4 <BOOT_ImgCopy>
30003e34:	696b      	ldr	r3, [r5, #20]
30003e36:	03d8      	lsls	r0, r3, #15
30003e38:	d5ba      	bpl.n	30003db0 <BOOT_LoadAPSubImage+0x78>
30003e3a:	ea08 0307 	and.w	r3, r8, r7
30003e3e:	3301      	adds	r3, #1
30003e40:	d073      	beq.n	30003f2a <BOOT_LoadAPSubImage+0x1f2>
30003e42:	f018 031f 	ands.w	r3, r8, #31
30003e46:	f000 8095 	beq.w	30003f74 <BOOT_LoadAPSubImage+0x23c>
30003e4a:	443b      	add	r3, r7
30003e4c:	f028 021f 	bic.w	r2, r8, #31
30003e50:	f3bf 8f4f 	dsb	sy
30003e54:	2b00      	cmp	r3, #0
30003e56:	dd0d      	ble.n	30003e74 <BOOT_LoadAPSubImage+0x13c>
30003e58:	1e59      	subs	r1, r3, #1
30003e5a:	f102 0320 	add.w	r3, r2, #32
30003e5e:	486d      	ldr	r0, [pc, #436]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003e60:	f021 011f 	bic.w	r1, r1, #31
30003e64:	4419      	add	r1, r3
30003e66:	428b      	cmp	r3, r1
30003e68:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003e6c:	461a      	mov	r2, r3
30003e6e:	f103 0320 	add.w	r3, r3, #32
30003e72:	d1f8      	bne.n	30003e66 <BOOT_LoadAPSubImage+0x12e>
30003e74:	f3bf 8f4f 	dsb	sy
30003e78:	f3bf 8f6f 	isb	sy
30003e7c:	e798      	b.n	30003db0 <BOOT_LoadAPSubImage+0x78>
30003e7e:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30003e82:	4628      	mov	r0, r5
30003e84:	4622      	mov	r2, r4
30003e86:	f001 ff0d 	bl	30005ca4 <BOOT_ImgCopy>
30003e8a:	4962      	ldr	r1, [pc, #392]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003e8c:	694b      	ldr	r3, [r1, #20]
30003e8e:	03d8      	lsls	r0, r3, #15
30003e90:	d5a5      	bpl.n	30003dde <BOOT_LoadAPSubImage+0xa6>
30003e92:	ea05 0304 	and.w	r3, r5, r4
30003e96:	3301      	adds	r3, #1
30003e98:	d06f      	beq.n	30003f7a <BOOT_LoadAPSubImage+0x242>
30003e9a:	f015 031f 	ands.w	r3, r5, #31
30003e9e:	d002      	beq.n	30003ea6 <BOOT_LoadAPSubImage+0x16e>
30003ea0:	f025 051f 	bic.w	r5, r5, #31
30003ea4:	441c      	add	r4, r3
30003ea6:	f3bf 8f4f 	dsb	sy
30003eaa:	2c00      	cmp	r4, #0
30003eac:	dd0d      	ble.n	30003eca <BOOT_LoadAPSubImage+0x192>
30003eae:	1e62      	subs	r2, r4, #1
30003eb0:	f105 0320 	add.w	r3, r5, #32
30003eb4:	4957      	ldr	r1, [pc, #348]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003eb6:	f022 021f 	bic.w	r2, r2, #31
30003eba:	441a      	add	r2, r3
30003ebc:	4293      	cmp	r3, r2
30003ebe:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30003ec2:	461d      	mov	r5, r3
30003ec4:	f103 0320 	add.w	r3, r3, #32
30003ec8:	d1f8      	bne.n	30003ebc <BOOT_LoadAPSubImage+0x184>
30003eca:	f3bf 8f4f 	dsb	sy
30003ece:	f3bf 8f6f 	isb	sy
30003ed2:	e784      	b.n	30003dde <BOOT_LoadAPSubImage+0xa6>
30003ed4:	4d4f      	ldr	r5, [pc, #316]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003ed6:	464a      	mov	r2, r9
30003ed8:	4639      	mov	r1, r7
30003eda:	4640      	mov	r0, r8
30003edc:	f001 fee2 	bl	30005ca4 <BOOT_ImgCopy>
30003ee0:	696b      	ldr	r3, [r5, #20]
30003ee2:	03db      	lsls	r3, r3, #15
30003ee4:	f57f af72 	bpl.w	30003dcc <BOOT_LoadAPSubImage+0x94>
30003ee8:	ea08 0309 	and.w	r3, r8, r9
30003eec:	3301      	adds	r3, #1
30003eee:	d067      	beq.n	30003fc0 <BOOT_LoadAPSubImage+0x288>
30003ef0:	f018 031f 	ands.w	r3, r8, #31
30003ef4:	d061      	beq.n	30003fba <BOOT_LoadAPSubImage+0x282>
30003ef6:	444b      	add	r3, r9
30003ef8:	f028 021f 	bic.w	r2, r8, #31
30003efc:	f3bf 8f4f 	dsb	sy
30003f00:	2b00      	cmp	r3, #0
30003f02:	dd0d      	ble.n	30003f20 <BOOT_LoadAPSubImage+0x1e8>
30003f04:	1e59      	subs	r1, r3, #1
30003f06:	f102 0320 	add.w	r3, r2, #32
30003f0a:	4842      	ldr	r0, [pc, #264]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003f0c:	f021 011f 	bic.w	r1, r1, #31
30003f10:	4419      	add	r1, r3
30003f12:	428b      	cmp	r3, r1
30003f14:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003f18:	461a      	mov	r2, r3
30003f1a:	f103 0320 	add.w	r3, r3, #32
30003f1e:	d1f8      	bne.n	30003f12 <BOOT_LoadAPSubImage+0x1da>
30003f20:	f3bf 8f4f 	dsb	sy
30003f24:	f3bf 8f6f 	isb	sy
30003f28:	e750      	b.n	30003dcc <BOOT_LoadAPSubImage+0x94>
30003f2a:	2300      	movs	r3, #0
30003f2c:	f8c5 3084 	str.w	r3, [r5, #132]	; 0x84
30003f30:	f3bf 8f4f 	dsb	sy
30003f34:	f8d5 0080 	ldr.w	r0, [r5, #128]	; 0x80
30003f38:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003f3c:	f3c0 324e 	ubfx	r2, r0, #13, #15
30003f40:	f3c0 00c9 	ubfx	r0, r0, #3, #10
30003f44:	0152      	lsls	r2, r2, #5
30003f46:	ea02 0c0e 	and.w	ip, r2, lr
30003f4a:	4603      	mov	r3, r0
30003f4c:	ea4c 7183 	orr.w	r1, ip, r3, lsl #30
30003f50:	3b01      	subs	r3, #1
30003f52:	f8c5 1274 	str.w	r1, [r5, #628]	; 0x274
30003f56:	d2f9      	bcs.n	30003f4c <BOOT_LoadAPSubImage+0x214>
30003f58:	3a20      	subs	r2, #32
30003f5a:	f112 0f20 	cmn.w	r2, #32
30003f5e:	d1f2      	bne.n	30003f46 <BOOT_LoadAPSubImage+0x20e>
30003f60:	e788      	b.n	30003e74 <BOOT_LoadAPSubImage+0x13c>
30003f62:	4669      	mov	r1, sp
30003f64:	482c      	ldr	r0, [pc, #176]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003f66:	f008 f927 	bl	3000c1b8 <__DiagPrintf_veneer>
30003f6a:	e744      	b.n	30003df6 <BOOT_LoadAPSubImage+0xbe>
30003f6c:	482b      	ldr	r0, [pc, #172]	; (3000401c <BOOT_LoadAPSubImage+0x2e4>)
30003f6e:	f008 f923 	bl	3000c1b8 <__DiagPrintf_veneer>
30003f72:	e749      	b.n	30003e08 <BOOT_LoadAPSubImage+0xd0>
30003f74:	463b      	mov	r3, r7
30003f76:	4642      	mov	r2, r8
30003f78:	e76a      	b.n	30003e50 <BOOT_LoadAPSubImage+0x118>
30003f7a:	2300      	movs	r3, #0
30003f7c:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003f80:	f3bf 8f4f 	dsb	sy
30003f84:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
30003f88:	f643 77e0 	movw	r7, #16352	; 0x3fe0
30003f8c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30003f90:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30003f94:	0164      	lsls	r4, r4, #5
30003f96:	ea04 0007 	and.w	r0, r4, r7
30003f9a:	462b      	mov	r3, r5
30003f9c:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30003fa0:	3b01      	subs	r3, #1
30003fa2:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30003fa6:	d2f9      	bcs.n	30003f9c <BOOT_LoadAPSubImage+0x264>
30003fa8:	3c20      	subs	r4, #32
30003faa:	f114 0f20 	cmn.w	r4, #32
30003fae:	d1f2      	bne.n	30003f96 <BOOT_LoadAPSubImage+0x25e>
30003fb0:	e78b      	b.n	30003eca <BOOT_LoadAPSubImage+0x192>
30003fb2:	481b      	ldr	r0, [pc, #108]	; (30004020 <BOOT_LoadAPSubImage+0x2e8>)
30003fb4:	f008 f900 	bl	3000c1b8 <__DiagPrintf_veneer>
30003fb8:	e731      	b.n	30003e1e <BOOT_LoadAPSubImage+0xe6>
30003fba:	464b      	mov	r3, r9
30003fbc:	4642      	mov	r2, r8
30003fbe:	e79d      	b.n	30003efc <BOOT_LoadAPSubImage+0x1c4>
30003fc0:	2300      	movs	r3, #0
30003fc2:	f8c5 3084 	str.w	r3, [r5, #132]	; 0x84
30003fc6:	f3bf 8f4f 	dsb	sy
30003fca:	f8d5 1080 	ldr.w	r1, [r5, #128]	; 0x80
30003fce:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003fd2:	f3c1 324e 	ubfx	r2, r1, #13, #15
30003fd6:	f3c1 01c9 	ubfx	r1, r1, #3, #10
30003fda:	0152      	lsls	r2, r2, #5
30003fdc:	ea02 0c0e 	and.w	ip, r2, lr
30003fe0:	460b      	mov	r3, r1
30003fe2:	ea4c 7083 	orr.w	r0, ip, r3, lsl #30
30003fe6:	3b01      	subs	r3, #1
30003fe8:	f8c5 0274 	str.w	r0, [r5, #628]	; 0x274
30003fec:	d2f9      	bcs.n	30003fe2 <BOOT_LoadAPSubImage+0x2aa>
30003fee:	3a20      	subs	r2, #32
30003ff0:	f112 0f20 	cmn.w	r2, #32
30003ff4:	d1f2      	bne.n	30003fdc <BOOT_LoadAPSubImage+0x2a4>
30003ff6:	e793      	b.n	30003f20 <BOOT_LoadAPSubImage+0x1e8>
30003ff8:	3000c54c 	.word	0x3000c54c
30003ffc:	35393138 	.word	0x35393138
30004000:	2001c00c 	.word	0x2001c00c
30004004:	3000c554 	.word	0x3000c554
30004008:	f7ffffe0 	.word	0xf7ffffe0
3000400c:	0000c109 	.word	0x0000c109
30004010:	00009be5 	.word	0x00009be5
30004014:	e000ed00 	.word	0xe000ed00
30004018:	3000c57c 	.word	0x3000c57c
3000401c:	3000c5a4 	.word	0x3000c5a4
30004020:	3000c5c8 	.word	0x3000c5c8

30004024 <BOOT_RSIPIvSet>:
30004024:	b530      	push	{r4, r5, lr}
30004026:	2300      	movs	r3, #0
30004028:	b083      	sub	sp, #12
3000402a:	4a09      	ldr	r2, [pc, #36]	; (30004050 <BOOT_RSIPIvSet+0x2c>)
3000402c:	4605      	mov	r5, r0
3000402e:	460c      	mov	r4, r1
30004030:	9300      	str	r3, [sp, #0]
30004032:	9301      	str	r3, [sp, #4]
30004034:	4790      	blx	r2
30004036:	b148      	cbz	r0, 3000404c <BOOT_RSIPIvSet+0x28>
30004038:	f105 0120 	add.w	r1, r5, #32
3000403c:	4b05      	ldr	r3, [pc, #20]	; (30004054 <BOOT_RSIPIvSet+0x30>)
3000403e:	2208      	movs	r2, #8
30004040:	4668      	mov	r0, sp
30004042:	4798      	blx	r3
30004044:	4669      	mov	r1, sp
30004046:	b2e0      	uxtb	r0, r4
30004048:	4b03      	ldr	r3, [pc, #12]	; (30004058 <BOOT_RSIPIvSet+0x34>)
3000404a:	4798      	blx	r3
3000404c:	b003      	add	sp, #12
3000404e:	bd30      	pop	{r4, r5, pc}
30004050:	0000c129 	.word	0x0000c129
30004054:	00012a1d 	.word	0x00012a1d
30004058:	3000a1f9 	.word	0x3000a1f9

3000405c <BOOT_OTFCheck>:
3000405c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004060:	4d1d      	ldr	r5, [pc, #116]	; (300040d8 <BOOT_OTFCheck+0x7c>)
30004062:	b082      	sub	sp, #8
30004064:	4604      	mov	r4, r0
30004066:	4688      	mov	r8, r1
30004068:	4617      	mov	r7, r2
3000406a:	461e      	mov	r6, r3
3000406c:	47a8      	blx	r5
3000406e:	b1b0      	cbz	r0, 3000409e <BOOT_OTFCheck+0x42>
30004070:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30004074:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004078:	d211      	bcs.n	3000409e <BOOT_OTFCheck+0x42>
3000407a:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
3000407e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004082:	d20c      	bcs.n	3000409e <BOOT_OTFCheck+0x42>
30004084:	4d15      	ldr	r5, [pc, #84]	; (300040dc <BOOT_OTFCheck+0x80>)
30004086:	682b      	ldr	r3, [r5, #0]
30004088:	039a      	lsls	r2, r3, #14
3000408a:	d420      	bmi.n	300040ce <BOOT_OTFCheck+0x72>
3000408c:	4b14      	ldr	r3, [pc, #80]	; (300040e0 <BOOT_OTFCheck+0x84>)
3000408e:	4798      	blx	r3
30004090:	4603      	mov	r3, r0
30004092:	b138      	cbz	r0, 300040a4 <BOOT_OTFCheck+0x48>
30004094:	2801      	cmp	r0, #1
30004096:	d006      	beq.n	300040a6 <BOOT_OTFCheck+0x4a>
30004098:	682b      	ldr	r3, [r5, #0]
3000409a:	039b      	lsls	r3, r3, #14
3000409c:	d411      	bmi.n	300040c2 <BOOT_OTFCheck+0x66>
3000409e:	b002      	add	sp, #8
300040a0:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300040a4:	2302      	movs	r3, #2
300040a6:	4630      	mov	r0, r6
300040a8:	4621      	mov	r1, r4
300040aa:	9301      	str	r3, [sp, #4]
300040ac:	4642      	mov	r2, r8
300040ae:	2301      	movs	r3, #1
300040b0:	4c0c      	ldr	r4, [pc, #48]	; (300040e4 <BOOT_OTFCheck+0x88>)
300040b2:	9700      	str	r7, [sp, #0]
300040b4:	47a0      	blx	r4
300040b6:	2001      	movs	r0, #1
300040b8:	4b0b      	ldr	r3, [pc, #44]	; (300040e8 <BOOT_OTFCheck+0x8c>)
300040ba:	b002      	add	sp, #8
300040bc:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040c0:	4718      	bx	r3
300040c2:	480a      	ldr	r0, [pc, #40]	; (300040ec <BOOT_OTFCheck+0x90>)
300040c4:	b002      	add	sp, #8
300040c6:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040ca:	f008 b875 	b.w	3000c1b8 <__DiagPrintf_veneer>
300040ce:	4808      	ldr	r0, [pc, #32]	; (300040f0 <BOOT_OTFCheck+0x94>)
300040d0:	f008 f872 	bl	3000c1b8 <__DiagPrintf_veneer>
300040d4:	e7da      	b.n	3000408c <BOOT_OTFCheck+0x30>
300040d6:	bf00      	nop
300040d8:	0000c129 	.word	0x0000c129
300040dc:	2001c00c 	.word	0x2001c00c
300040e0:	0000c245 	.word	0x0000c245
300040e4:	3000a22d 	.word	0x3000a22d
300040e8:	3000a215 	.word	0x3000a215
300040ec:	3000c60c 	.word	0x3000c60c
300040f0:	3000c5fc 	.word	0x3000c5fc

300040f4 <BOOT_OTA_LoadIMG2>:
300040f4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300040f8:	4b6e      	ldr	r3, [pc, #440]	; (300042b4 <BOOT_OTA_LoadIMG2+0x1c0>)
300040fa:	b08d      	sub	sp, #52	; 0x34
300040fc:	4a6e      	ldr	r2, [pc, #440]	; (300042b8 <BOOT_OTA_LoadIMG2+0x1c4>)
300040fe:	f103 040c 	add.w	r4, r3, #12
30004102:	9003      	str	r0, [sp, #12]
30004104:	f8d2 8000 	ldr.w	r8, [r2]
30004108:	ae06      	add	r6, sp, #24
3000410a:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
3000410e:	ab09      	add	r3, sp, #36	; 0x24
30004110:	e886 0007 	stmia.w	r6, {r0, r1, r2}
30004114:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30004118:	e883 0007 	stmia.w	r3, {r0, r1, r2}
3000411c:	4641      	mov	r1, r8
3000411e:	4867      	ldr	r0, [pc, #412]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c8>)
30004120:	f44f 72b2 	mov.w	r2, #356	; 0x164
30004124:	f001 fdbe 	bl	30005ca4 <BOOT_ImgCopy>
30004128:	4641      	mov	r1, r8
3000412a:	4864      	ldr	r0, [pc, #400]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c8>)
3000412c:	f000 fd86 	bl	30004c3c <BOOT_CertificateCheck>
30004130:	2800      	cmp	r0, #0
30004132:	f040 80b7 	bne.w	300042a4 <BOOT_OTA_LoadIMG2+0x1b0>
30004136:	4604      	mov	r4, r0
30004138:	f44f 72c8 	mov.w	r2, #400	; 0x190
3000413c:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30004140:	485f      	ldr	r0, [pc, #380]	; (300042c0 <BOOT_OTA_LoadIMG2+0x1cc>)
30004142:	f001 fdaf 	bl	30005ca4 <BOOT_ImgCopy>
30004146:	4b5f      	ldr	r3, [pc, #380]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1d0>)
30004148:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000414c:	4798      	blx	r3
3000414e:	2800      	cmp	r0, #0
30004150:	f040 809e 	bne.w	30004290 <BOOT_OTA_LoadIMG2+0x19c>
30004154:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30004158:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30004308 <BOOT_OTA_LoadIMG2+0x214>
3000415c:	4a5a      	ldr	r2, [pc, #360]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1d4>)
3000415e:	2000      	movs	r0, #0
30004160:	4445      	add	r5, r8
30004162:	4659      	mov	r1, fp
30004164:	4f59      	ldr	r7, [pc, #356]	; (300042cc <BOOT_OTA_LoadIMG2+0x1d8>)
30004166:	f04f 0801 	mov.w	r8, #1
3000416a:	462b      	mov	r3, r5
3000416c:	f8df 919c 	ldr.w	r9, [pc, #412]	; 3000430c <BOOT_OTA_LoadIMG2+0x218>
30004170:	47b8      	blx	r7
30004172:	2101      	movs	r1, #1
30004174:	2000      	movs	r0, #0
30004176:	4b56      	ldr	r3, [pc, #344]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1dc>)
30004178:	4798      	blx	r3
3000417a:	2301      	movs	r3, #1
3000417c:	4952      	ldr	r1, [pc, #328]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1d4>)
3000417e:	4658      	mov	r0, fp
30004180:	461a      	mov	r2, r3
30004182:	f7ff ff6b 	bl	3000405c <BOOT_OTFCheck>
30004186:	47c8      	blx	r9
30004188:	2800      	cmp	r0, #0
3000418a:	bf0c      	ite	eq
3000418c:	46aa      	moveq	sl, r5
3000418e:	46da      	movne	sl, fp
30004190:	4633      	mov	r3, r6
30004192:	2203      	movs	r2, #3
30004194:	4651      	mov	r1, sl
30004196:	484f      	ldr	r0, [pc, #316]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1e0>)
30004198:	f8cd 8000 	str.w	r8, [sp]
3000419c:	f7ff fd18 	bl	30003bd0 <BOOT_LoadSubImage>
300041a0:	2800      	cmp	r0, #0
300041a2:	d070      	beq.n	30004286 <BOOT_OTA_LoadIMG2+0x192>
300041a4:	4e4b      	ldr	r6, [pc, #300]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1e0>)
300041a6:	47c8      	blx	r9
300041a8:	2800      	cmp	r0, #0
300041aa:	bf08      	it	eq
300041ac:	46ab      	moveq	fp, r5
300041ae:	4f47      	ldr	r7, [pc, #284]	; (300042cc <BOOT_OTA_LoadIMG2+0x1d8>)
300041b0:	68f2      	ldr	r2, [r6, #12]
300041b2:	4640      	mov	r0, r8
300041b4:	6873      	ldr	r3, [r6, #4]
300041b6:	f8c6 b000 	str.w	fp, [r6]
300041ba:	4413      	add	r3, r2
300041bc:	6972      	ldr	r2, [r6, #20]
300041be:	f8df b150 	ldr.w	fp, [pc, #336]	; 30004310 <BOOT_OTA_LoadIMG2+0x21c>
300041c2:	441a      	add	r2, r3
300041c4:	4b44      	ldr	r3, [pc, #272]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1e4>)
300041c6:	eb0a 0102 	add.w	r1, sl, r2
300041ca:	61b1      	str	r1, [r6, #24]
300041cc:	1e51      	subs	r1, r2, #1
300041ce:	400b      	ands	r3, r1
300041d0:	4659      	mov	r1, fp
300041d2:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
300041d6:	441d      	add	r5, r3
300041d8:	1a9b      	subs	r3, r3, r2
300041da:	4a40      	ldr	r2, [pc, #256]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e8>)
300041dc:	61f3      	str	r3, [r6, #28]
300041de:	462b      	mov	r3, r5
300041e0:	47b8      	blx	r7
300041e2:	4641      	mov	r1, r8
300041e4:	4640      	mov	r0, r8
300041e6:	4b3a      	ldr	r3, [pc, #232]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1dc>)
300041e8:	4798      	blx	r3
300041ea:	4642      	mov	r2, r8
300041ec:	2302      	movs	r3, #2
300041ee:	493b      	ldr	r1, [pc, #236]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e8>)
300041f0:	4658      	mov	r0, fp
300041f2:	f7ff ff33 	bl	3000405c <BOOT_OTFCheck>
300041f6:	47c8      	blx	r9
300041f8:	2800      	cmp	r0, #0
300041fa:	bf0c      	ite	eq
300041fc:	462f      	moveq	r7, r5
300041fe:	465f      	movne	r7, fp
30004200:	ab09      	add	r3, sp, #36	; 0x24
30004202:	2203      	movs	r2, #3
30004204:	f106 0020 	add.w	r0, r6, #32
30004208:	4639      	mov	r1, r7
3000420a:	f8cd 8000 	str.w	r8, [sp]
3000420e:	f7ff fcdf 	bl	30003bd0 <BOOT_LoadSubImage>
30004212:	2800      	cmp	r0, #0
30004214:	d037      	beq.n	30004286 <BOOT_OTA_LoadIMG2+0x192>
30004216:	47c8      	blx	r9
30004218:	6af2      	ldr	r2, [r6, #44]	; 0x2c
3000421a:	6a73      	ldr	r3, [r6, #36]	; 0x24
3000421c:	2800      	cmp	r0, #0
3000421e:	bf08      	it	eq
30004220:	46ab      	moveq	fp, r5
30004222:	4827      	ldr	r0, [pc, #156]	; (300042c0 <BOOT_OTA_LoadIMG2+0x1cc>)
30004224:	4413      	add	r3, r2
30004226:	6b72      	ldr	r2, [r6, #52]	; 0x34
30004228:	f8c6 b020 	str.w	fp, [r6, #32]
3000422c:	4413      	add	r3, r2
3000422e:	4a2a      	ldr	r2, [pc, #168]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1e4>)
30004230:	1e59      	subs	r1, r3, #1
30004232:	441f      	add	r7, r3
30004234:	400a      	ands	r2, r1
30004236:	2100      	movs	r1, #0
30004238:	63b7      	str	r7, [r6, #56]	; 0x38
3000423a:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
3000423e:	9100      	str	r1, [sp, #0]
30004240:	1ad1      	subs	r1, r2, r3
30004242:	2900      	cmp	r1, #0
30004244:	63f1      	str	r1, [r6, #60]	; 0x3c
30004246:	4631      	mov	r1, r6
30004248:	bf18      	it	ne
3000424a:	4613      	movne	r3, r2
3000424c:	4a24      	ldr	r2, [pc, #144]	; (300042e0 <BOOT_OTA_LoadIMG2+0x1ec>)
3000424e:	441d      	add	r5, r3
30004250:	4b1a      	ldr	r3, [pc, #104]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c8>)
30004252:	6015      	str	r5, [r2, #0]
30004254:	bf0c      	ite	eq
30004256:	2207      	moveq	r2, #7
30004258:	2208      	movne	r2, #8
3000425a:	f000 fbdb 	bl	30004a14 <BOOT_SignatureCheck>
3000425e:	4b21      	ldr	r3, [pc, #132]	; (300042e4 <BOOT_OTA_LoadIMG2+0x1f0>)
30004260:	4605      	mov	r5, r0
30004262:	689b      	ldr	r3, [r3, #8]
30004264:	079b      	lsls	r3, r3, #30
30004266:	d50a      	bpl.n	3000427e <BOOT_OTA_LoadIMG2+0x18a>
30004268:	4b1f      	ldr	r3, [pc, #124]	; (300042e8 <BOOT_OTA_LoadIMG2+0x1f4>)
3000426a:	4a20      	ldr	r2, [pc, #128]	; (300042ec <BOOT_OTA_LoadIMG2+0x1f8>)
3000426c:	9903      	ldr	r1, [sp, #12]
3000426e:	2800      	cmp	r0, #0
30004270:	bf18      	it	ne
30004272:	461a      	movne	r2, r3
30004274:	481e      	ldr	r0, [pc, #120]	; (300042f0 <BOOT_OTA_LoadIMG2+0x1fc>)
30004276:	3101      	adds	r1, #1
30004278:	462b      	mov	r3, r5
3000427a:	f007 ff9d 	bl	3000c1b8 <__DiagPrintf_veneer>
3000427e:	b11d      	cbz	r5, 30004288 <BOOT_OTA_LoadIMG2+0x194>
30004280:	481c      	ldr	r0, [pc, #112]	; (300042f4 <BOOT_OTA_LoadIMG2+0x200>)
30004282:	f007 ff99 	bl	3000c1b8 <__DiagPrintf_veneer>
30004286:	24ff      	movs	r4, #255	; 0xff
30004288:	4620      	mov	r0, r4
3000428a:	b00d      	add	sp, #52	; 0x34
3000428c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004290:	2208      	movs	r2, #8
30004292:	4919      	ldr	r1, [pc, #100]	; (300042f8 <BOOT_OTA_LoadIMG2+0x204>)
30004294:	4b19      	ldr	r3, [pc, #100]	; (300042fc <BOOT_OTA_LoadIMG2+0x208>)
30004296:	a804      	add	r0, sp, #16
30004298:	4798      	blx	r3
3000429a:	a904      	add	r1, sp, #16
3000429c:	2001      	movs	r0, #1
3000429e:	4b18      	ldr	r3, [pc, #96]	; (30004300 <BOOT_OTA_LoadIMG2+0x20c>)
300042a0:	4798      	blx	r3
300042a2:	e757      	b.n	30004154 <BOOT_OTA_LoadIMG2+0x60>
300042a4:	24ff      	movs	r4, #255	; 0xff
300042a6:	4817      	ldr	r0, [pc, #92]	; (30004304 <BOOT_OTA_LoadIMG2+0x210>)
300042a8:	f007 ff86 	bl	3000c1b8 <__DiagPrintf_veneer>
300042ac:	4620      	mov	r0, r4
300042ae:	b00d      	add	sp, #52	; 0x34
300042b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300042b4:	3000d6f0 	.word	0x3000d6f0
300042b8:	3000ee30 	.word	0x3000ee30
300042bc:	3000ead4 	.word	0x3000ead4
300042c0:	3000ec38 	.word	0x3000ec38
300042c4:	0000c129 	.word	0x0000c129
300042c8:	0cffffe0 	.word	0x0cffffe0
300042cc:	3000a2a9 	.word	0x3000a2a9
300042d0:	3000a2e9 	.word	0x3000a2e9
300042d4:	3000ede0 	.word	0x3000ede0
300042d8:	fffff000 	.word	0xfffff000
300042dc:	0effffe0 	.word	0x0effffe0
300042e0:	3000e2a4 	.word	0x3000e2a4
300042e4:	2001c00c 	.word	0x2001c00c
300042e8:	3000c628 	.word	0x3000c628
300042ec:	3000c620 	.word	0x3000c620
300042f0:	3000c658 	.word	0x3000c658
300042f4:	3000c68c 	.word	0x3000c68c
300042f8:	3000ec58 	.word	0x3000ec58
300042fc:	00012a1d 	.word	0x00012a1d
30004300:	3000a1f9 	.word	0x3000a1f9
30004304:	3000c630 	.word	0x3000c630
30004308:	0c000000 	.word	0x0c000000
3000430c:	3000a501 	.word	0x3000a501
30004310:	0e000000 	.word	0x0e000000

30004314 <BOOT_OTA_Region_Init>:
30004314:	4b24      	ldr	r3, [pc, #144]	; (300043a8 <BOOT_OTA_Region_Init+0x94>)
30004316:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000431a:	4798      	blx	r3
3000431c:	4b23      	ldr	r3, [pc, #140]	; (300043ac <BOOT_OTA_Region_Init+0x98>)
3000431e:	4a24      	ldr	r2, [pc, #144]	; (300043b0 <BOOT_OTA_Region_Init+0x9c>)
30004320:	4924      	ldr	r1, [pc, #144]	; (300043b4 <BOOT_OTA_Region_Init+0xa0>)
30004322:	2800      	cmp	r0, #0
30004324:	bf08      	it	eq
30004326:	461a      	moveq	r2, r3
30004328:	6813      	ldr	r3, [r2, #0]
3000432a:	2bff      	cmp	r3, #255	; 0xff
3000432c:	e9d1 0400 	ldrd	r0, r4, [r1]
30004330:	d029      	beq.n	30004386 <BOOT_OTA_Region_Init+0x72>
30004332:	2500      	movs	r5, #0
30004334:	e9d1 e804 	ldrd	lr, r8, [r1, #16]
30004338:	46ac      	mov	ip, r5
3000433a:	462f      	mov	r7, r5
3000433c:	462e      	mov	r6, r5
3000433e:	e00c      	b.n	3000435a <BOOT_OTA_Region_Init+0x46>
30004340:	2b03      	cmp	r3, #3
30004342:	d028      	beq.n	30004396 <BOOT_OTA_Region_Init+0x82>
30004344:	2b04      	cmp	r3, #4
30004346:	d029      	beq.n	3000439c <BOOT_OTA_Region_Init+0x88>
30004348:	2b05      	cmp	r3, #5
3000434a:	d102      	bne.n	30004352 <BOOT_OTA_Region_Init+0x3e>
3000434c:	f8d2 8004 	ldr.w	r8, [r2, #4]
30004350:	2501      	movs	r5, #1
30004352:	f852 3f0c 	ldr.w	r3, [r2, #12]!
30004356:	2bff      	cmp	r3, #255	; 0xff
30004358:	d007      	beq.n	3000436a <BOOT_OTA_Region_Init+0x56>
3000435a:	2b02      	cmp	r3, #2
3000435c:	d1f0      	bne.n	30004340 <BOOT_OTA_Region_Init+0x2c>
3000435e:	6850      	ldr	r0, [r2, #4]
30004360:	2601      	movs	r6, #1
30004362:	f852 3f0c 	ldr.w	r3, [r2, #12]!
30004366:	2bff      	cmp	r3, #255	; 0xff
30004368:	d1f7      	bne.n	3000435a <BOOT_OTA_Region_Init+0x46>
3000436a:	b106      	cbz	r6, 3000436e <BOOT_OTA_Region_Init+0x5a>
3000436c:	6008      	str	r0, [r1, #0]
3000436e:	b107      	cbz	r7, 30004372 <BOOT_OTA_Region_Init+0x5e>
30004370:	604c      	str	r4, [r1, #4]
30004372:	f1bc 0f00 	cmp.w	ip, #0
30004376:	d001      	beq.n	3000437c <BOOT_OTA_Region_Init+0x68>
30004378:	f8c1 e010 	str.w	lr, [r1, #16]
3000437c:	e9d1 0400 	ldrd	r0, r4, [r1]
30004380:	b10d      	cbz	r5, 30004386 <BOOT_OTA_Region_Init+0x72>
30004382:	f8c1 8014 	str.w	r8, [r1, #20]
30004386:	f500 5080 	add.w	r0, r0, #4096	; 0x1000
3000438a:	f504 5480 	add.w	r4, r4, #4096	; 0x1000
3000438e:	e9c1 0402 	strd	r0, r4, [r1, #8]
30004392:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30004396:	6854      	ldr	r4, [r2, #4]
30004398:	2701      	movs	r7, #1
3000439a:	e7da      	b.n	30004352 <BOOT_OTA_Region_Init+0x3e>
3000439c:	f8d2 e004 	ldr.w	lr, [r2, #4]
300043a0:	f04f 0c01 	mov.w	ip, #1
300043a4:	e7d5      	b.n	30004352 <BOOT_OTA_Region_Init+0x3e>
300043a6:	bf00      	nop
300043a8:	3000a501 	.word	0x3000a501
300043ac:	3000e3cc 	.word	0x3000e3cc
300043b0:	3000e444 	.word	0x3000e444
300043b4:	3000edc8 	.word	0x3000edc8

300043b8 <BOOT_OTA_IMG2>:
300043b8:	f7ff be9c 	b.w	300040f4 <BOOT_OTA_LoadIMG2>

300043bc <BOOT_OTA_AP>:
300043bc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300043c0:	4c31      	ldr	r4, [pc, #196]	; (30004488 <BOOT_OTA_AP+0xcc>)
300043c2:	b0ed      	sub	sp, #436	; 0x1b4
300043c4:	4b31      	ldr	r3, [pc, #196]	; (3000448c <BOOT_OTA_AP+0xd0>)
300043c6:	ad05      	add	r5, sp, #20
300043c8:	af03      	add	r7, sp, #12
300043ca:	681b      	ldr	r3, [r3, #0]
300043cc:	4e30      	ldr	r6, [pc, #192]	; (30004490 <BOOT_OTA_AP+0xd4>)
300043ce:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
300043d2:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
300043d6:	340c      	adds	r4, #12
300043d8:	e885 0007 	stmia.w	r5, {r0, r1, r2}
300043dc:	e894 0003 	ldmia.w	r4, {r0, r1}
300043e0:	f44f 72c8 	mov.w	r2, #400	; 0x190
300043e4:	e887 0003 	stmia.w	r7, {r0, r1}
300043e8:	4619      	mov	r1, r3
300043ea:	a808      	add	r0, sp, #32
300043ec:	f001 fc5a 	bl	30005ca4 <BOOT_ImgCopy>
300043f0:	2201      	movs	r2, #1
300043f2:	462b      	mov	r3, r5
300043f4:	4641      	mov	r1, r8
300043f6:	9200      	str	r2, [sp, #0]
300043f8:	4630      	mov	r0, r6
300043fa:	2203      	movs	r2, #3
300043fc:	f7ff fbe8 	bl	30003bd0 <BOOT_LoadSubImage>
30004400:	2801      	cmp	r0, #1
30004402:	d122      	bne.n	3000444a <BOOT_OTA_AP+0x8e>
30004404:	f04f 0902 	mov.w	r9, #2
30004408:	4604      	mov	r4, r0
3000440a:	4b22      	ldr	r3, [pc, #136]	; (30004494 <BOOT_OTA_AP+0xd8>)
3000440c:	a808      	add	r0, sp, #32
3000440e:	2203      	movs	r2, #3
30004410:	4631      	mov	r1, r6
30004412:	f8cd 9000 	str.w	r9, [sp]
30004416:	f000 fafd 	bl	30004a14 <BOOT_SignatureCheck>
3000441a:	4605      	mov	r5, r0
3000441c:	bb30      	cbnz	r0, 3000446c <BOOT_OTA_AP+0xb0>
3000441e:	464a      	mov	r2, r9
30004420:	463b      	mov	r3, r7
30004422:	4641      	mov	r1, r8
30004424:	4630      	mov	r0, r6
30004426:	9400      	str	r4, [sp, #0]
30004428:	f7ff fbd2 	bl	30003bd0 <BOOT_LoadSubImage>
3000442c:	6871      	ldr	r1, [r6, #4]
3000442e:	68f3      	ldr	r3, [r6, #12]
30004430:	4622      	mov	r2, r4
30004432:	f106 0010 	add.w	r0, r6, #16
30004436:	4419      	add	r1, r3
30004438:	4441      	add	r1, r8
3000443a:	f7ff fc7d 	bl	30003d38 <BOOT_LoadAPSubImage>
3000443e:	2801      	cmp	r0, #1
30004440:	d10b      	bne.n	3000445a <BOOT_OTA_AP+0x9e>
30004442:	4628      	mov	r0, r5
30004444:	b06d      	add	sp, #436	; 0x1b4
30004446:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000444a:	25ff      	movs	r5, #255	; 0xff
3000444c:	4812      	ldr	r0, [pc, #72]	; (30004498 <BOOT_OTA_AP+0xdc>)
3000444e:	f007 feb3 	bl	3000c1b8 <__DiagPrintf_veneer>
30004452:	4628      	mov	r0, r5
30004454:	b06d      	add	sp, #436	; 0x1b4
30004456:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000445a:	4b10      	ldr	r3, [pc, #64]	; (3000449c <BOOT_OTA_AP+0xe0>)
3000445c:	681b      	ldr	r3, [r3, #0]
3000445e:	079b      	lsls	r3, r3, #30
30004460:	d40c      	bmi.n	3000447c <BOOT_OTA_AP+0xc0>
30004462:	25ff      	movs	r5, #255	; 0xff
30004464:	4628      	mov	r0, r5
30004466:	b06d      	add	sp, #436	; 0x1b4
30004468:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000446c:	25ff      	movs	r5, #255	; 0xff
3000446e:	480c      	ldr	r0, [pc, #48]	; (300044a0 <BOOT_OTA_AP+0xe4>)
30004470:	f007 fea2 	bl	3000c1b8 <__DiagPrintf_veneer>
30004474:	4628      	mov	r0, r5
30004476:	b06d      	add	sp, #436	; 0x1b4
30004478:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000447c:	4809      	ldr	r0, [pc, #36]	; (300044a4 <BOOT_OTA_AP+0xe8>)
3000447e:	25ff      	movs	r5, #255	; 0xff
30004480:	f007 fe9a 	bl	3000c1b8 <__DiagPrintf_veneer>
30004484:	e7dd      	b.n	30004442 <BOOT_OTA_AP+0x86>
30004486:	bf00      	nop
30004488:	3000d708 	.word	0x3000d708
3000448c:	3000e2a4 	.word	0x3000e2a4
30004490:	3000ede0 	.word	0x3000ede0
30004494:	3000ead4 	.word	0x3000ead4
30004498:	3000c6d4 	.word	0x3000c6d4
3000449c:	2001c00c 	.word	0x2001c00c
300044a0:	3000c6b0 	.word	0x3000c6b0
300044a4:	3000c6f8 	.word	0x3000c6f8

300044a8 <BOOT_OTFMaskConfig>:
300044a8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300044ac:	4c1c      	ldr	r4, [pc, #112]	; (30004520 <BOOT_OTFMaskConfig+0x78>)
300044ae:	b082      	sub	sp, #8
300044b0:	e9d4 1200 	ldrd	r1, r2, [r4]
300044b4:	f8d4 c010 	ldr.w	ip, [r4, #16]
300044b8:	1c4d      	adds	r5, r1, #1
300044ba:	68a0      	ldr	r0, [r4, #8]
300044bc:	d027      	beq.n	3000450e <BOOT_OTFMaskConfig+0x66>
300044be:	2500      	movs	r5, #0
300044c0:	4e18      	ldr	r6, [pc, #96]	; (30004524 <BOOT_OTFMaskConfig+0x7c>)
300044c2:	f8df 806c 	ldr.w	r8, [pc, #108]	; 30004530 <BOOT_OTFMaskConfig+0x88>
300044c6:	4f18      	ldr	r7, [pc, #96]	; (30004528 <BOOT_OTFMaskConfig+0x80>)
300044c8:	e00d      	b.n	300044e6 <BOOT_OTFMaskConfig+0x3e>
300044ca:	6833      	ldr	r3, [r6, #0]
300044cc:	079a      	lsls	r2, r3, #30
300044ce:	d416      	bmi.n	300044fe <BOOT_OTFMaskConfig+0x56>
300044d0:	6961      	ldr	r1, [r4, #20]
300044d2:	3501      	adds	r5, #1
300044d4:	69a2      	ldr	r2, [r4, #24]
300044d6:	3414      	adds	r4, #20
300044d8:	1c4b      	adds	r3, r1, #1
300044da:	f8d4 c010 	ldr.w	ip, [r4, #16]
300044de:	68a0      	ldr	r0, [r4, #8]
300044e0:	d015      	beq.n	3000450e <BOOT_OTFMaskConfig+0x66>
300044e2:	2d08      	cmp	r5, #8
300044e4:	d00f      	beq.n	30004506 <BOOT_OTFMaskConfig+0x5e>
300044e6:	f101 4378 	add.w	r3, r1, #4160749568	; 0xf8000000
300044ea:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300044ee:	d2ec      	bcs.n	300044ca <BOOT_OTFMaskConfig+0x22>
300044f0:	9001      	str	r0, [sp, #4]
300044f2:	2301      	movs	r3, #1
300044f4:	4628      	mov	r0, r5
300044f6:	f8cd c000 	str.w	ip, [sp]
300044fa:	47b8      	blx	r7
300044fc:	e7e8      	b.n	300044d0 <BOOT_OTFMaskConfig+0x28>
300044fe:	4640      	mov	r0, r8
30004500:	f007 fe5a 	bl	3000c1b8 <__DiagPrintf_veneer>
30004504:	e7e4      	b.n	300044d0 <BOOT_OTFMaskConfig+0x28>
30004506:	4b07      	ldr	r3, [pc, #28]	; (30004524 <BOOT_OTFMaskConfig+0x7c>)
30004508:	681b      	ldr	r3, [r3, #0]
3000450a:	0799      	lsls	r1, r3, #30
3000450c:	d402      	bmi.n	30004514 <BOOT_OTFMaskConfig+0x6c>
3000450e:	b002      	add	sp, #8
30004510:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30004514:	4805      	ldr	r0, [pc, #20]	; (3000452c <BOOT_OTFMaskConfig+0x84>)
30004516:	b002      	add	sp, #8
30004518:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000451c:	f007 be4c 	b.w	3000c1b8 <__DiagPrintf_veneer>
30004520:	3000d7b4 	.word	0x3000d7b4
30004524:	2001c00c 	.word	0x2001c00c
30004528:	3000a22d 	.word	0x3000a22d
3000452c:	3000c75c 	.word	0x3000c75c
30004530:	3000c7a4 	.word	0x3000c7a4

30004534 <BOOT_LoadRDPImg>:
30004534:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004538:	4d50      	ldr	r5, [pc, #320]	; (3000467c <BOOT_LoadRDPImg+0x148>)
3000453a:	b0d7      	sub	sp, #348	; 0x15c
3000453c:	4613      	mov	r3, r2
3000453e:	468b      	mov	fp, r1
30004540:	4604      	mov	r4, r0
30004542:	aa04      	add	r2, sp, #16
30004544:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30004548:	e895 0003 	ldmia.w	r5, {r0, r1}
3000454c:	2500      	movs	r5, #0
3000454e:	e882 0003 	stmia.w	r2, {r0, r1}
30004552:	4619      	mov	r1, r3
30004554:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004558:	4620      	mov	r0, r4
3000455a:	f001 fba3 	bl	30005ca4 <BOOT_ImgCopy>
3000455e:	ab04      	add	r3, sp, #16
30004560:	2202      	movs	r2, #2
30004562:	4641      	mov	r1, r8
30004564:	4658      	mov	r0, fp
30004566:	9500      	str	r5, [sp, #0]
30004568:	f7ff fb32 	bl	30003bd0 <BOOT_LoadSubImage>
3000456c:	2801      	cmp	r0, #1
3000456e:	d070      	beq.n	30004652 <BOOT_LoadRDPImg+0x11e>
30004570:	4b43      	ldr	r3, [pc, #268]	; (30004680 <BOOT_LoadRDPImg+0x14c>)
30004572:	4628      	mov	r0, r5
30004574:	4798      	blx	r3
30004576:	2101      	movs	r1, #1
30004578:	4b42      	ldr	r3, [pc, #264]	; (30004684 <BOOT_LoadRDPImg+0x150>)
3000457a:	4628      	mov	r0, r5
3000457c:	4798      	blx	r3
3000457e:	af06      	add	r7, sp, #24
30004580:	2120      	movs	r1, #32
30004582:	4b41      	ldr	r3, [pc, #260]	; (30004688 <BOOT_LoadRDPImg+0x154>)
30004584:	a80e      	add	r0, sp, #56	; 0x38
30004586:	f8df 9118 	ldr.w	r9, [pc, #280]	; 300046a0 <BOOT_LoadRDPImg+0x16c>
3000458a:	4798      	blx	r3
3000458c:	f8df a114 	ldr.w	sl, [pc, #276]	; 300046a4 <BOOT_LoadRDPImg+0x170>
30004590:	f104 0120 	add.w	r1, r4, #32
30004594:	2210      	movs	r2, #16
30004596:	a80a      	add	r0, sp, #40	; 0x28
30004598:	47d0      	blx	sl
3000459a:	9503      	str	r5, [sp, #12]
3000459c:	4641      	mov	r1, r8
3000459e:	a816      	add	r0, sp, #88	; 0x58
300045a0:	2210      	movs	r2, #16
300045a2:	f001 fb7f 	bl	30005ca4 <BOOT_ImgCopy>
300045a6:	2310      	movs	r3, #16
300045a8:	aa0a      	add	r2, sp, #40	; 0x28
300045aa:	a816      	add	r0, sp, #88	; 0x58
300045ac:	4619      	mov	r1, r3
300045ae:	9700      	str	r7, [sp, #0]
300045b0:	47c8      	blx	r9
300045b2:	683b      	ldr	r3, [r7, #0]
300045b4:	4a35      	ldr	r2, [pc, #212]	; (3000468c <BOOT_LoadRDPImg+0x158>)
300045b6:	4293      	cmp	r3, r2
300045b8:	4b35      	ldr	r3, [pc, #212]	; (30004690 <BOOT_LoadRDPImg+0x15c>)
300045ba:	d14e      	bne.n	3000465a <BOOT_LoadRDPImg+0x126>
300045bc:	687a      	ldr	r2, [r7, #4]
300045be:	4935      	ldr	r1, [pc, #212]	; (30004694 <BOOT_LoadRDPImg+0x160>)
300045c0:	689b      	ldr	r3, [r3, #8]
300045c2:	428a      	cmp	r2, r1
300045c4:	f003 0302 	and.w	r3, r3, #2
300045c8:	d155      	bne.n	30004676 <BOOT_LoadRDPImg+0x142>
300045ca:	68fe      	ldr	r6, [r7, #12]
300045cc:	68bd      	ldr	r5, [r7, #8]
300045ce:	3e20      	subs	r6, #32
300045d0:	3520      	adds	r5, #32
300045d2:	2b00      	cmp	r3, #0
300045d4:	d133      	bne.n	3000463e <BOOT_LoadRDPImg+0x10a>
300045d6:	f1bb 0f00 	cmp.w	fp, #0
300045da:	d006      	beq.n	300045ea <BOOT_LoadRDPImg+0xb6>
300045dc:	9a03      	ldr	r2, [sp, #12]
300045de:	f10b 0304 	add.w	r3, fp, #4
300045e2:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
300045e6:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
300045ea:	b1dd      	cbz	r5, 30004624 <BOOT_LoadRDPImg+0xf0>
300045ec:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
300045f0:	462c      	mov	r4, r5
300045f2:	4641      	mov	r1, r8
300045f4:	a816      	add	r0, sp, #88	; 0x58
300045f6:	bf28      	it	cs
300045f8:	f44f 7480 	movcs.w	r4, #256	; 0x100
300045fc:	4622      	mov	r2, r4
300045fe:	44a0      	add	r8, r4
30004600:	f001 fb50 	bl	30005ca4 <BOOT_ImgCopy>
30004604:	aa0a      	add	r2, sp, #40	; 0x28
30004606:	4621      	mov	r1, r4
30004608:	2310      	movs	r3, #16
3000460a:	9600      	str	r6, [sp, #0]
3000460c:	a816      	add	r0, sp, #88	; 0x58
3000460e:	47c8      	blx	r9
30004610:	ab16      	add	r3, sp, #88	; 0x58
30004612:	f1a4 0110 	sub.w	r1, r4, #16
30004616:	2210      	movs	r2, #16
30004618:	a80a      	add	r0, sp, #40	; 0x28
3000461a:	4426      	add	r6, r4
3000461c:	4419      	add	r1, r3
3000461e:	47d0      	blx	sl
30004620:	1b2d      	subs	r5, r5, r4
30004622:	d1e3      	bne.n	300045ec <BOOT_LoadRDPImg+0xb8>
30004624:	9b03      	ldr	r3, [sp, #12]
30004626:	b13b      	cbz	r3, 30004638 <BOOT_LoadRDPImg+0x104>
30004628:	2100      	movs	r1, #0
3000462a:	4b16      	ldr	r3, [pc, #88]	; (30004684 <BOOT_LoadRDPImg+0x150>)
3000462c:	4608      	mov	r0, r1
3000462e:	4798      	blx	r3
30004630:	2002      	movs	r0, #2
30004632:	b057      	add	sp, #348	; 0x15c
30004634:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004638:	2301      	movs	r3, #1
3000463a:	9303      	str	r3, [sp, #12]
3000463c:	e7ae      	b.n	3000459c <BOOT_LoadRDPImg+0x68>
3000463e:	a804      	add	r0, sp, #16
30004640:	9903      	ldr	r1, [sp, #12]
30004642:	462b      	mov	r3, r5
30004644:	4632      	mov	r2, r6
30004646:	f850 1021 	ldr.w	r1, [r0, r1, lsl #2]
3000464a:	4813      	ldr	r0, [pc, #76]	; (30004698 <BOOT_LoadRDPImg+0x164>)
3000464c:	f007 fdb4 	bl	3000c1b8 <__DiagPrintf_veneer>
30004650:	e7c1      	b.n	300045d6 <BOOT_LoadRDPImg+0xa2>
30004652:	2002      	movs	r0, #2
30004654:	b057      	add	sp, #348	; 0x15c
30004656:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000465a:	6898      	ldr	r0, [r3, #8]
3000465c:	f000 0002 	and.w	r0, r0, #2
30004660:	2800      	cmp	r0, #0
30004662:	d0e6      	beq.n	30004632 <BOOT_LoadRDPImg+0xfe>
30004664:	9b03      	ldr	r3, [sp, #12]
30004666:	aa04      	add	r2, sp, #16
30004668:	480c      	ldr	r0, [pc, #48]	; (3000469c <BOOT_LoadRDPImg+0x168>)
3000466a:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
3000466e:	f007 fda3 	bl	3000c1b8 <__DiagPrintf_veneer>
30004672:	2000      	movs	r0, #0
30004674:	e7dd      	b.n	30004632 <BOOT_LoadRDPImg+0xfe>
30004676:	4618      	mov	r0, r3
30004678:	e7f2      	b.n	30004660 <BOOT_LoadRDPImg+0x12c>
3000467a:	bf00      	nop
3000467c:	3000d71c 	.word	0x3000d71c
30004680:	00002429 	.word	0x00002429
30004684:	00002479 	.word	0x00002479
30004688:	00004641 	.word	0x00004641
3000468c:	35393138 	.word	0x35393138
30004690:	2001c00c 	.word	0x2001c00c
30004694:	31313738 	.word	0x31313738
30004698:	3000c57c 	.word	0x3000c57c
3000469c:	3000c554 	.word	0x3000c554
300046a0:	000046f5 	.word	0x000046f5
300046a4:	00012a1d 	.word	0x00012a1d

300046a8 <BOOT_LoadBL32RDPImg>:
300046a8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300046ac:	4b28      	ldr	r3, [pc, #160]	; (30004750 <BOOT_LoadBL32RDPImg+0xa8>)
300046ae:	b0d1      	sub	sp, #324	; 0x144
300046b0:	f8df c0ac 	ldr.w	ip, [pc, #172]	; 30004760 <BOOT_LoadBL32RDPImg+0xb8>
300046b4:	4607      	mov	r7, r0
300046b6:	f10d 0810 	add.w	r8, sp, #16
300046ba:	460e      	mov	r6, r1
300046bc:	4615      	mov	r5, r2
300046be:	ac02      	add	r4, sp, #8
300046c0:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 30004764 <BOOT_LoadBL32RDPImg+0xbc>
300046c4:	f8df b0a0 	ldr.w	fp, [pc, #160]	; 30004768 <BOOT_LoadBL32RDPImg+0xc0>
300046c8:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300046ca:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
300046ce:	e89c 0003 	ldmia.w	ip, {r0, r1}
300046d2:	e884 0003 	stmia.w	r4, {r0, r1}
300046d6:	2000      	movs	r0, #0
300046d8:	47c8      	blx	r9
300046da:	2101      	movs	r1, #1
300046dc:	2000      	movs	r0, #0
300046de:	47d8      	blx	fp
300046e0:	2120      	movs	r1, #32
300046e2:	4b1c      	ldr	r3, [pc, #112]	; (30004754 <BOOT_LoadBL32RDPImg+0xac>)
300046e4:	eb0d 0001 	add.w	r0, sp, r1
300046e8:	4798      	blx	r3
300046ea:	4b1b      	ldr	r3, [pc, #108]	; (30004758 <BOOT_LoadBL32RDPImg+0xb0>)
300046ec:	689b      	ldr	r3, [r3, #8]
300046ee:	079b      	lsls	r3, r3, #30
300046f0:	d427      	bmi.n	30004742 <BOOT_LoadBL32RDPImg+0x9a>
300046f2:	b1fd      	cbz	r5, 30004734 <BOOT_LoadBL32RDPImg+0x8c>
300046f4:	f8df a074 	ldr.w	sl, [pc, #116]	; 3000476c <BOOT_LoadBL32RDPImg+0xc4>
300046f8:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30004770 <BOOT_LoadBL32RDPImg+0xc8>
300046fc:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30004700:	462c      	mov	r4, r5
30004702:	4639      	mov	r1, r7
30004704:	a810      	add	r0, sp, #64	; 0x40
30004706:	bf28      	it	cs
30004708:	f44f 7480 	movcs.w	r4, #256	; 0x100
3000470c:	4622      	mov	r2, r4
3000470e:	4427      	add	r7, r4
30004710:	f001 fac8 	bl	30005ca4 <BOOT_ImgCopy>
30004714:	4642      	mov	r2, r8
30004716:	4621      	mov	r1, r4
30004718:	2310      	movs	r3, #16
3000471a:	9600      	str	r6, [sp, #0]
3000471c:	a810      	add	r0, sp, #64	; 0x40
3000471e:	47d0      	blx	sl
30004720:	ab10      	add	r3, sp, #64	; 0x40
30004722:	f1a4 0110 	sub.w	r1, r4, #16
30004726:	2210      	movs	r2, #16
30004728:	4640      	mov	r0, r8
3000472a:	4426      	add	r6, r4
3000472c:	4419      	add	r1, r3
3000472e:	47c8      	blx	r9
30004730:	1b2d      	subs	r5, r5, r4
30004732:	d1e3      	bne.n	300046fc <BOOT_LoadBL32RDPImg+0x54>
30004734:	2100      	movs	r1, #0
30004736:	4608      	mov	r0, r1
30004738:	47d8      	blx	fp
3000473a:	2001      	movs	r0, #1
3000473c:	b051      	add	sp, #324	; 0x144
3000473e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004742:	4621      	mov	r1, r4
30004744:	462b      	mov	r3, r5
30004746:	4632      	mov	r2, r6
30004748:	4804      	ldr	r0, [pc, #16]	; (3000475c <BOOT_LoadBL32RDPImg+0xb4>)
3000474a:	f007 fd35 	bl	3000c1b8 <__DiagPrintf_veneer>
3000474e:	e7d0      	b.n	300046f2 <BOOT_LoadBL32RDPImg+0x4a>
30004750:	3000c7dc 	.word	0x3000c7dc
30004754:	00004641 	.word	0x00004641
30004758:	2001c00c 	.word	0x2001c00c
3000475c:	3000c57c 	.word	0x3000c57c
30004760:	3000c7f0 	.word	0x3000c7f0
30004764:	00002429 	.word	0x00002429
30004768:	00002479 	.word	0x00002479
3000476c:	000046f5 	.word	0x000046f5
30004770:	00012a1d 	.word	0x00012a1d

30004774 <_BOOT_Validate_ImgHash>:
30004774:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004778:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
3000477c:	4605      	mov	r5, r0
3000477e:	4690      	mov	r8, r2
30004780:	22e0      	movs	r2, #224	; 0xe0
30004782:	9103      	str	r1, [sp, #12]
30004784:	2100      	movs	r1, #0
30004786:	a818      	add	r0, sp, #96	; 0x60
30004788:	461c      	mov	r4, r3
3000478a:	f88d 101f 	strb.w	r1, [sp, #31]
3000478e:	f007 fcf3 	bl	3000c178 <____wrap_memset_veneer>
30004792:	4628      	mov	r0, r5
30004794:	f10d 021f 	add.w	r2, sp, #31
30004798:	f10d 011e 	add.w	r1, sp, #30
3000479c:	f007 fd24 	bl	3000c1e8 <__SBOOT_GetMdType_veneer>
300047a0:	9001      	str	r0, [sp, #4]
300047a2:	2800      	cmp	r0, #0
300047a4:	d166      	bne.n	30004874 <_BOOT_Validate_ImgHash+0x100>
300047a6:	f89d 301f 	ldrb.w	r3, [sp, #31]
300047aa:	2b00      	cmp	r3, #0
300047ac:	d067      	beq.n	3000487e <_BOOT_Validate_ImgHash+0x10a>
300047ae:	2101      	movs	r1, #1
300047b0:	4605      	mov	r5, r0
300047b2:	4b80      	ldr	r3, [pc, #512]	; (300049b4 <_BOOT_Validate_ImgHash+0x240>)
300047b4:	4608      	mov	r0, r1
300047b6:	4798      	blx	r3
300047b8:	4629      	mov	r1, r5
300047ba:	ab18      	add	r3, sp, #96	; 0x60
300047bc:	2220      	movs	r2, #32
300047be:	f89d 001e 	ldrb.w	r0, [sp, #30]
300047c2:	4d7d      	ldr	r5, [pc, #500]	; (300049b8 <_BOOT_Validate_ImgHash+0x244>)
300047c4:	47a8      	blx	r5
300047c6:	2800      	cmp	r0, #0
300047c8:	f040 808b 	bne.w	300048e2 <_BOOT_Validate_ImgHash+0x16e>
300047cc:	b32c      	cbz	r4, 3000481a <_BOOT_Validate_ImgHash+0xa6>
300047ce:	aad0      	add	r2, sp, #832	; 0x340
300047d0:	1e63      	subs	r3, r4, #1
300047d2:	ae50      	add	r6, sp, #320	; 0x140
300047d4:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 300049d0 <_BOOT_Validate_ImgHash+0x25c>
300047d8:	f012 0218 	ands.w	r2, r2, #24
300047dc:	b2db      	uxtb	r3, r3
300047de:	f026 091f 	bic.w	r9, r6, #31
300047e2:	4d76      	ldr	r5, [pc, #472]	; (300049bc <_BOOT_Validate_ImgHash+0x248>)
300047e4:	9204      	str	r2, [sp, #16]
300047e6:	f108 0208 	add.w	r2, r8, #8
300047ea:	bf08      	it	eq
300047ec:	46b1      	moveq	r9, r6
300047ee:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300047f2:	9302      	str	r3, [sp, #8]
300047f4:	e9d8 b700 	ldrd	fp, r7, [r8]
300047f8:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300047fc:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004800:	f0c0 80a7 	bcc.w	30004952 <_BOOT_Validate_ImgHash+0x1de>
30004804:	4639      	mov	r1, r7
30004806:	4658      	mov	r0, fp
30004808:	aa18      	add	r2, sp, #96	; 0x60
3000480a:	47d0      	blx	sl
3000480c:	2800      	cmp	r0, #0
3000480e:	d168      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
30004810:	f108 0808 	add.w	r8, r8, #8
30004814:	9b02      	ldr	r3, [sp, #8]
30004816:	4543      	cmp	r3, r8
30004818:	d1ec      	bne.n	300047f4 <_BOOT_Validate_ImgHash+0x80>
3000481a:	ac08      	add	r4, sp, #32
3000481c:	a918      	add	r1, sp, #96	; 0x60
3000481e:	4b68      	ldr	r3, [pc, #416]	; (300049c0 <_BOOT_Validate_ImgHash+0x24c>)
30004820:	4620      	mov	r0, r4
30004822:	4798      	blx	r3
30004824:	4601      	mov	r1, r0
30004826:	2800      	cmp	r0, #0
30004828:	d15b      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
3000482a:	2001      	movs	r0, #1
3000482c:	4b61      	ldr	r3, [pc, #388]	; (300049b4 <_BOOT_Validate_ImgHash+0x240>)
3000482e:	4798      	blx	r3
30004830:	f89d 501e 	ldrb.w	r5, [sp, #30]
30004834:	4b63      	ldr	r3, [pc, #396]	; (300049c4 <_BOOT_Validate_ImgHash+0x250>)
30004836:	4798      	blx	r3
30004838:	fbb0 f3f5 	udiv	r3, r0, r5
3000483c:	fb03 0315 	mls	r3, r3, r5, r0
30004840:	b2db      	uxtb	r3, r3
30004842:	b1bd      	cbz	r5, 30004874 <_BOOT_Validate_ImgHash+0x100>
30004844:	2000      	movs	r0, #0
30004846:	9f03      	ldr	r7, [sp, #12]
30004848:	4606      	mov	r6, r0
3000484a:	1c5a      	adds	r2, r3, #1
3000484c:	5cf9      	ldrb	r1, [r7, r3]
3000484e:	5ce3      	ldrb	r3, [r4, r3]
30004850:	3001      	adds	r0, #1
30004852:	b2d2      	uxtb	r2, r2
30004854:	404b      	eors	r3, r1
30004856:	431e      	orrs	r6, r3
30004858:	fbb2 f3f5 	udiv	r3, r2, r5
3000485c:	fb05 2213 	mls	r2, r5, r3, r2
30004860:	b2d3      	uxtb	r3, r2
30004862:	b2c2      	uxtb	r2, r0
30004864:	4295      	cmp	r5, r2
30004866:	d8f0      	bhi.n	3000484a <_BOOT_Validate_ImgHash+0xd6>
30004868:	2e00      	cmp	r6, #0
3000486a:	9b01      	ldr	r3, [sp, #4]
3000486c:	bf18      	it	ne
3000486e:	f06f 0306 	mvnne.w	r3, #6
30004872:	9301      	str	r3, [sp, #4]
30004874:	9801      	ldr	r0, [sp, #4]
30004876:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
3000487a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000487e:	a918      	add	r1, sp, #96	; 0x60
30004880:	f89d 001e 	ldrb.w	r0, [sp, #30]
30004884:	4b50      	ldr	r3, [pc, #320]	; (300049c8 <_BOOT_Validate_ImgHash+0x254>)
30004886:	4798      	blx	r3
30004888:	bb58      	cbnz	r0, 300048e2 <_BOOT_Validate_ImgHash+0x16e>
3000488a:	b31c      	cbz	r4, 300048d4 <_BOOT_Validate_ImgHash+0x160>
3000488c:	aad0      	add	r2, sp, #832	; 0x340
3000488e:	1e63      	subs	r3, r4, #1
30004890:	ae50      	add	r6, sp, #320	; 0x140
30004892:	f8df a140 	ldr.w	sl, [pc, #320]	; 300049d4 <_BOOT_Validate_ImgHash+0x260>
30004896:	f012 0218 	ands.w	r2, r2, #24
3000489a:	b2db      	uxtb	r3, r3
3000489c:	f026 091f 	bic.w	r9, r6, #31
300048a0:	4d46      	ldr	r5, [pc, #280]	; (300049bc <_BOOT_Validate_ImgHash+0x248>)
300048a2:	9204      	str	r2, [sp, #16]
300048a4:	f108 0208 	add.w	r2, r8, #8
300048a8:	bf08      	it	eq
300048aa:	46b1      	moveq	r9, r6
300048ac:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300048b0:	9302      	str	r3, [sp, #8]
300048b2:	e9d8 b700 	ldrd	fp, r7, [r8]
300048b6:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300048ba:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300048be:	d318      	bcc.n	300048f2 <_BOOT_Validate_ImgHash+0x17e>
300048c0:	4639      	mov	r1, r7
300048c2:	4658      	mov	r0, fp
300048c4:	aa18      	add	r2, sp, #96	; 0x60
300048c6:	47d0      	blx	sl
300048c8:	b958      	cbnz	r0, 300048e2 <_BOOT_Validate_ImgHash+0x16e>
300048ca:	f108 0808 	add.w	r8, r8, #8
300048ce:	9b02      	ldr	r3, [sp, #8]
300048d0:	4543      	cmp	r3, r8
300048d2:	d1ee      	bne.n	300048b2 <_BOOT_Validate_ImgHash+0x13e>
300048d4:	ac08      	add	r4, sp, #32
300048d6:	a918      	add	r1, sp, #96	; 0x60
300048d8:	4b3c      	ldr	r3, [pc, #240]	; (300049cc <_BOOT_Validate_ImgHash+0x258>)
300048da:	4620      	mov	r0, r4
300048dc:	4798      	blx	r3
300048de:	2800      	cmp	r0, #0
300048e0:	d0a6      	beq.n	30004830 <_BOOT_Validate_ImgHash+0xbc>
300048e2:	f06f 030a 	mvn.w	r3, #10
300048e6:	9301      	str	r3, [sp, #4]
300048e8:	9801      	ldr	r0, [sp, #4]
300048ea:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300048ee:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300048f2:	2f00      	cmp	r7, #0
300048f4:	d0e9      	beq.n	300048ca <_BOOT_Validate_ImgHash+0x156>
300048f6:	9c04      	ldr	r4, [sp, #16]
300048f8:	f8cd 8014 	str.w	r8, [sp, #20]
300048fc:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30004900:	46b8      	mov	r8, r7
30004902:	4659      	mov	r1, fp
30004904:	4630      	mov	r0, r6
30004906:	bf28      	it	cs
30004908:	f44f 7800 	movcs.w	r8, #512	; 0x200
3000490c:	4642      	mov	r2, r8
3000490e:	f001 f9c9 	bl	30005ca4 <BOOT_ImgCopy>
30004912:	696a      	ldr	r2, [r5, #20]
30004914:	03d3      	lsls	r3, r2, #15
30004916:	d50f      	bpl.n	30004938 <_BOOT_Validate_ImgHash+0x1c4>
30004918:	eb08 0004 	add.w	r0, r8, r4
3000491c:	f3bf 8f4f 	dsb	sy
30004920:	4448      	add	r0, r9
30004922:	464a      	mov	r2, r9
30004924:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30004928:	3220      	adds	r2, #32
3000492a:	1a83      	subs	r3, r0, r2
3000492c:	2b00      	cmp	r3, #0
3000492e:	dcf9      	bgt.n	30004924 <_BOOT_Validate_ImgHash+0x1b0>
30004930:	f3bf 8f4f 	dsb	sy
30004934:	f3bf 8f6f 	isb	sy
30004938:	aa18      	add	r2, sp, #96	; 0x60
3000493a:	4641      	mov	r1, r8
3000493c:	4630      	mov	r0, r6
3000493e:	47d0      	blx	sl
30004940:	2800      	cmp	r0, #0
30004942:	d1ce      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
30004944:	ebb7 0708 	subs.w	r7, r7, r8
30004948:	44c3      	add	fp, r8
3000494a:	d1d7      	bne.n	300048fc <_BOOT_Validate_ImgHash+0x188>
3000494c:	f8dd 8014 	ldr.w	r8, [sp, #20]
30004950:	e7bb      	b.n	300048ca <_BOOT_Validate_ImgHash+0x156>
30004952:	2f00      	cmp	r7, #0
30004954:	f43f af5c 	beq.w	30004810 <_BOOT_Validate_ImgHash+0x9c>
30004958:	9c04      	ldr	r4, [sp, #16]
3000495a:	f8cd 8014 	str.w	r8, [sp, #20]
3000495e:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30004962:	46b8      	mov	r8, r7
30004964:	4659      	mov	r1, fp
30004966:	4630      	mov	r0, r6
30004968:	bf28      	it	cs
3000496a:	f44f 7800 	movcs.w	r8, #512	; 0x200
3000496e:	4642      	mov	r2, r8
30004970:	f001 f998 	bl	30005ca4 <BOOT_ImgCopy>
30004974:	696a      	ldr	r2, [r5, #20]
30004976:	03d2      	lsls	r2, r2, #15
30004978:	d50f      	bpl.n	3000499a <_BOOT_Validate_ImgHash+0x226>
3000497a:	eb08 0004 	add.w	r0, r8, r4
3000497e:	f3bf 8f4f 	dsb	sy
30004982:	4448      	add	r0, r9
30004984:	464a      	mov	r2, r9
30004986:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
3000498a:	3220      	adds	r2, #32
3000498c:	1a83      	subs	r3, r0, r2
3000498e:	2b00      	cmp	r3, #0
30004990:	dcf9      	bgt.n	30004986 <_BOOT_Validate_ImgHash+0x212>
30004992:	f3bf 8f4f 	dsb	sy
30004996:	f3bf 8f6f 	isb	sy
3000499a:	aa18      	add	r2, sp, #96	; 0x60
3000499c:	4641      	mov	r1, r8
3000499e:	4630      	mov	r0, r6
300049a0:	47d0      	blx	sl
300049a2:	2800      	cmp	r0, #0
300049a4:	d19d      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
300049a6:	ebb7 0708 	subs.w	r7, r7, r8
300049aa:	44c3      	add	fp, r8
300049ac:	d1d7      	bne.n	3000495e <_BOOT_Validate_ImgHash+0x1ea>
300049ae:	f8dd 8014 	ldr.w	r8, [sp, #20]
300049b2:	e72d      	b.n	30004810 <_BOOT_Validate_ImgHash+0x9c>
300049b4:	000024dd 	.word	0x000024dd
300049b8:	30009e71 	.word	0x30009e71
300049bc:	e000ed00 	.word	0xe000ed00
300049c0:	00005bb9 	.word	0x00005bb9
300049c4:	0000ea4d 	.word	0x0000ea4d
300049c8:	00004ffd 	.word	0x00004ffd
300049cc:	000053f1 	.word	0x000053f1
300049d0:	00005b4d 	.word	0x00005b4d
300049d4:	00005385 	.word	0x00005385

300049d8 <BOOT_Validate_PubKey>:
300049d8:	f007 bc16 	b.w	3000c208 <__SBOOT_Validate_PubKey_veneer>

300049dc <BOOT_Validate_Signature>:
300049dc:	f007 bbf4 	b.w	3000c1c8 <__SBOOT_Validate_Signature_veneer>

300049e0 <BOOT_Validate_ImgHash>:
300049e0:	b530      	push	{r4, r5, lr}
300049e2:	b085      	sub	sp, #20
300049e4:	4d0a      	ldr	r5, [pc, #40]	; (30004a10 <BOOT_Validate_ImgHash+0x30>)
300049e6:	4604      	mov	r4, r0
300049e8:	9301      	str	r3, [sp, #4]
300049ea:	e9cd 2102 	strd	r2, r1, [sp, #8]
300049ee:	47a8      	blx	r5
300049f0:	9903      	ldr	r1, [sp, #12]
300049f2:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300049f6:	b128      	cbz	r0, 30004a04 <BOOT_Validate_ImgHash+0x24>
300049f8:	4620      	mov	r0, r4
300049fa:	b005      	add	sp, #20
300049fc:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004a00:	f007 bbd2 	b.w	3000c1a8 <__SBOOT_Validate_ImgHash_veneer>
30004a04:	4620      	mov	r0, r4
30004a06:	b005      	add	sp, #20
30004a08:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004a0c:	f7ff beb2 	b.w	30004774 <_BOOT_Validate_ImgHash>
30004a10:	3000a501 	.word	0x3000a501

30004a14 <BOOT_SignatureCheck>:
30004a14:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30004a18:	461e      	mov	r6, r3
30004a1a:	b08b      	sub	sp, #44	; 0x2c
30004a1c:	4b7e      	ldr	r3, [pc, #504]	; (30004c18 <BOOT_SignatureCheck+0x204>)
30004a1e:	4680      	mov	r8, r0
30004a20:	4689      	mov	r9, r1
30004a22:	4617      	mov	r7, r2
30004a24:	f10d 0c28 	add.w	ip, sp, #40	; 0x28
30004a28:	4c7c      	ldr	r4, [pc, #496]	; (30004c1c <BOOT_SignatureCheck+0x208>)
30004a2a:	9d12      	ldr	r5, [sp, #72]	; 0x48
30004a2c:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30004a2e:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30004a32:	47a0      	blx	r4
30004a34:	b918      	cbnz	r0, 30004a3e <BOOT_SignatureCheck+0x2a>
30004a36:	2000      	movs	r0, #0
30004a38:	b00b      	add	sp, #44	; 0x2c
30004a3a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004a3e:	4b78      	ldr	r3, [pc, #480]	; (30004c20 <BOOT_SignatureCheck+0x20c>)
30004a40:	2000      	movs	r0, #0
30004a42:	4798      	blx	r3
30004a44:	f898 2012 	ldrb.w	r2, [r8, #18]
30004a48:	f898 3013 	ldrb.w	r3, [r8, #19]
30004a4c:	f10d 0117 	add.w	r1, sp, #23
30004a50:	f10d 0016 	add.w	r0, sp, #22
30004a54:	f007 fbc0 	bl	3000c1d8 <__SBOOT_Validate_Algorithm_veneer>
30004a58:	4602      	mov	r2, r0
30004a5a:	b9e8      	cbnz	r0, 30004a98 <BOOT_SignatureCheck+0x84>
30004a5c:	69f0      	ldr	r0, [r6, #28]
30004a5e:	b918      	cbnz	r0, 30004a68 <BOOT_SignatureCheck+0x54>
30004a60:	e0d7      	b.n	30004c12 <BOOT_SignatureCheck+0x1fe>
30004a62:	b2ca      	uxtb	r2, r1
30004a64:	4282      	cmp	r2, r0
30004a66:	d208      	bcs.n	30004a7a <BOOT_SignatureCheck+0x66>
30004a68:	eb02 04c2 	add.w	r4, r2, r2, lsl #3
30004a6c:	1c51      	adds	r1, r2, #1
30004a6e:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30004a72:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30004a76:	42ab      	cmp	r3, r5
30004a78:	d1f3      	bne.n	30004a62 <BOOT_SignatureCheck+0x4e>
30004a7a:	2124      	movs	r1, #36	; 0x24
30004a7c:	23b4      	movs	r3, #180	; 0xb4
30004a7e:	fb11 3202 	smlabb	r2, r1, r2, r3
30004a82:	f108 0470 	add.w	r4, r8, #112	; 0x70
30004a86:	4432      	add	r2, r6
30004a88:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004a8c:	4621      	mov	r1, r4
30004a8e:	f007 fbbb 	bl	3000c208 <__SBOOT_Validate_PubKey_veneer>
30004a92:	4602      	mov	r2, r0
30004a94:	2800      	cmp	r0, #0
30004a96:	d072      	beq.n	30004b7e <BOOT_SignatureCheck+0x16a>
30004a98:	4b62      	ldr	r3, [pc, #392]	; (30004c24 <BOOT_SignatureCheck+0x210>)
30004a9a:	681b      	ldr	r3, [r3, #0]
30004a9c:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004aa0:	2b00      	cmp	r3, #0
30004aa2:	f040 80a4 	bne.w	30004bee <BOOT_SignatureCheck+0x1da>
30004aa6:	2f00      	cmp	r7, #0
30004aa8:	d045      	beq.n	30004b36 <BOOT_SignatureCheck+0x122>
30004aaa:	464d      	mov	r5, r9
30004aac:	f107 39ff 	add.w	r9, r7, #4294967295
30004ab0:	f04f 0800 	mov.w	r8, #0
30004ab4:	4f5c      	ldr	r7, [pc, #368]	; (30004c28 <BOOT_SignatureCheck+0x214>)
30004ab6:	fa5f f389 	uxtb.w	r3, r9
30004aba:	f105 0908 	add.w	r9, r5, #8
30004abe:	1d2e      	adds	r6, r5, #4
30004ac0:	4c5a      	ldr	r4, [pc, #360]	; (30004c2c <BOOT_SignatureCheck+0x218>)
30004ac2:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30004ac6:	e003      	b.n	30004ad0 <BOOT_SignatureCheck+0xbc>
30004ac8:	3508      	adds	r5, #8
30004aca:	3608      	adds	r6, #8
30004acc:	45a9      	cmp	r9, r5
30004ace:	d032      	beq.n	30004b36 <BOOT_SignatureCheck+0x122>
30004ad0:	f856 0c04 	ldr.w	r0, [r6, #-4]
30004ad4:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30004ad8:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004adc:	d3f4      	bcc.n	30004ac8 <BOOT_SignatureCheck+0xb4>
30004ade:	2100      	movs	r1, #0
30004ae0:	686a      	ldr	r2, [r5, #4]
30004ae2:	47b8      	blx	r7
30004ae4:	6963      	ldr	r3, [r4, #20]
30004ae6:	03d9      	lsls	r1, r3, #15
30004ae8:	d5ee      	bpl.n	30004ac8 <BOOT_SignatureCheck+0xb4>
30004aea:	f856 2c04 	ldr.w	r2, [r6, #-4]
30004aee:	686b      	ldr	r3, [r5, #4]
30004af0:	ea02 0103 	and.w	r1, r2, r3
30004af4:	3101      	adds	r1, #1
30004af6:	d022      	beq.n	30004b3e <BOOT_SignatureCheck+0x12a>
30004af8:	f012 011f 	ands.w	r1, r2, #31
30004afc:	d002      	beq.n	30004b04 <BOOT_SignatureCheck+0xf0>
30004afe:	440b      	add	r3, r1
30004b00:	f022 021f 	bic.w	r2, r2, #31
30004b04:	f3bf 8f4f 	dsb	sy
30004b08:	2b00      	cmp	r3, #0
30004b0a:	dd0c      	ble.n	30004b26 <BOOT_SignatureCheck+0x112>
30004b0c:	1e59      	subs	r1, r3, #1
30004b0e:	f102 0320 	add.w	r3, r2, #32
30004b12:	f021 011f 	bic.w	r1, r1, #31
30004b16:	4419      	add	r1, r3
30004b18:	428b      	cmp	r3, r1
30004b1a:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30004b1e:	461a      	mov	r2, r3
30004b20:	f103 0320 	add.w	r3, r3, #32
30004b24:	d1f8      	bne.n	30004b18 <BOOT_SignatureCheck+0x104>
30004b26:	f3bf 8f4f 	dsb	sy
30004b2a:	f3bf 8f6f 	isb	sy
30004b2e:	3508      	adds	r5, #8
30004b30:	3608      	adds	r6, #8
30004b32:	45a9      	cmp	r9, r5
30004b34:	d1cc      	bne.n	30004ad0 <BOOT_SignatureCheck+0xbc>
30004b36:	20ff      	movs	r0, #255	; 0xff
30004b38:	b00b      	add	sp, #44	; 0x2c
30004b3a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004b3e:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30004b42:	f3bf 8f4f 	dsb	sy
30004b46:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30004b4a:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30004b4e:	f3c3 304e 	ubfx	r0, r3, #13, #15
30004b52:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30004b56:	0140      	lsls	r0, r0, #5
30004b58:	ea00 010e 	and.w	r1, r0, lr
30004b5c:	4663      	mov	r3, ip
30004b5e:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004b62:	3b01      	subs	r3, #1
30004b64:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30004b68:	1c5a      	adds	r2, r3, #1
30004b6a:	d1f8      	bne.n	30004b5e <BOOT_SignatureCheck+0x14a>
30004b6c:	3820      	subs	r0, #32
30004b6e:	f110 0f20 	cmn.w	r0, #32
30004b72:	d1f1      	bne.n	30004b58 <BOOT_SignatureCheck+0x144>
30004b74:	f3bf 8f4f 	dsb	sy
30004b78:	f3bf 8f6f 	isb	sy
30004b7c:	e7d7      	b.n	30004b2e <BOOT_SignatureCheck+0x11a>
30004b7e:	f508 7380 	add.w	r3, r8, #256	; 0x100
30004b82:	4622      	mov	r2, r4
30004b84:	f89d 1017 	ldrb.w	r1, [sp, #23]
30004b88:	9301      	str	r3, [sp, #4]
30004b8a:	f44f 7380 	mov.w	r3, #256	; 0x100
30004b8e:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004b92:	9300      	str	r3, [sp, #0]
30004b94:	4643      	mov	r3, r8
30004b96:	f007 fb17 	bl	3000c1c8 <__SBOOT_Validate_Signature_veneer>
30004b9a:	4602      	mov	r2, r0
30004b9c:	2800      	cmp	r0, #0
30004b9e:	f47f af7b 	bne.w	30004a98 <BOOT_SignatureCheck+0x84>
30004ba2:	f108 0530 	add.w	r5, r8, #48	; 0x30
30004ba6:	4b22      	ldr	r3, [pc, #136]	; (30004c30 <BOOT_SignatureCheck+0x21c>)
30004ba8:	f89d 4017 	ldrb.w	r4, [sp, #23]
30004bac:	4798      	blx	r3
30004bae:	4629      	mov	r1, r5
30004bb0:	bb40      	cbnz	r0, 30004c04 <BOOT_SignatureCheck+0x1f0>
30004bb2:	464a      	mov	r2, r9
30004bb4:	4620      	mov	r0, r4
30004bb6:	463b      	mov	r3, r7
30004bb8:	f7ff fddc 	bl	30004774 <_BOOT_Validate_ImgHash>
30004bbc:	4602      	mov	r2, r0
30004bbe:	4b19      	ldr	r3, [pc, #100]	; (30004c24 <BOOT_SignatureCheck+0x210>)
30004bc0:	681b      	ldr	r3, [r3, #0]
30004bc2:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004bc6:	2a00      	cmp	r2, #0
30004bc8:	f47f af6a 	bne.w	30004aa0 <BOOT_SignatureCheck+0x8c>
30004bcc:	2b00      	cmp	r3, #0
30004bce:	f43f af32 	beq.w	30004a36 <BOOT_SignatureCheck+0x22>
30004bd2:	f898 3011 	ldrb.w	r3, [r8, #17]
30004bd6:	9203      	str	r2, [sp, #12]
30004bd8:	aa0a      	add	r2, sp, #40	; 0x28
30004bda:	4816      	ldr	r0, [pc, #88]	; (30004c34 <BOOT_SignatureCheck+0x220>)
30004bdc:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30004be0:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004be4:	f007 fae8 	bl	3000c1b8 <__DiagPrintf_veneer>
30004be8:	9a03      	ldr	r2, [sp, #12]
30004bea:	4610      	mov	r0, r2
30004bec:	e724      	b.n	30004a38 <BOOT_SignatureCheck+0x24>
30004bee:	a90a      	add	r1, sp, #40	; 0x28
30004bf0:	f898 3011 	ldrb.w	r3, [r8, #17]
30004bf4:	4810      	ldr	r0, [pc, #64]	; (30004c38 <BOOT_SignatureCheck+0x224>)
30004bf6:	eb01 0383 	add.w	r3, r1, r3, lsl #2
30004bfa:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004bfe:	f007 fadb 	bl	3000c1b8 <__DiagPrintf_veneer>
30004c02:	e750      	b.n	30004aa6 <BOOT_SignatureCheck+0x92>
30004c04:	464a      	mov	r2, r9
30004c06:	4620      	mov	r0, r4
30004c08:	463b      	mov	r3, r7
30004c0a:	f007 facd 	bl	3000c1a8 <__SBOOT_Validate_ImgHash_veneer>
30004c0e:	4602      	mov	r2, r0
30004c10:	e7d5      	b.n	30004bbe <BOOT_SignatureCheck+0x1aa>
30004c12:	22b4      	movs	r2, #180	; 0xb4
30004c14:	e735      	b.n	30004a82 <BOOT_SignatureCheck+0x6e>
30004c16:	bf00      	nop
30004c18:	3000d724 	.word	0x3000d724
30004c1c:	0000c149 	.word	0x0000c149
30004c20:	00002451 	.word	0x00002451
30004c24:	2001c00c 	.word	0x2001c00c
30004c28:	00012be5 	.word	0x00012be5
30004c2c:	e000ed00 	.word	0xe000ed00
30004c30:	3000a501 	.word	0x3000a501
30004c34:	3000c7f8 	.word	0x3000c7f8
30004c38:	3000c80c 	.word	0x3000c80c

30004c3c <BOOT_CertificateCheck>:
30004c3c:	b530      	push	{r4, r5, lr}
30004c3e:	4b2b      	ldr	r3, [pc, #172]	; (30004cec <BOOT_CertificateCheck+0xb0>)
30004c40:	b0b1      	sub	sp, #196	; 0xc4
30004c42:	4604      	mov	r4, r0
30004c44:	460d      	mov	r5, r1
30004c46:	4798      	blx	r3
30004c48:	b910      	cbnz	r0, 30004c50 <BOOT_CertificateCheck+0x14>
30004c4a:	2000      	movs	r0, #0
30004c4c:	b031      	add	sp, #196	; 0xc4
30004c4e:	bd30      	pop	{r4, r5, pc}
30004c50:	69a1      	ldr	r1, [r4, #24]
30004c52:	2290      	movs	r2, #144	; 0x90
30004c54:	a80c      	add	r0, sp, #48	; 0x30
30004c56:	4429      	add	r1, r5
30004c58:	f001 f824 	bl	30005ca4 <BOOT_ImgCopy>
30004c5c:	4b24      	ldr	r3, [pc, #144]	; (30004cf0 <BOOT_CertificateCheck+0xb4>)
30004c5e:	4798      	blx	r3
30004c60:	4924      	ldr	r1, [pc, #144]	; (30004cf4 <BOOT_CertificateCheck+0xb8>)
30004c62:	f000 031f 	and.w	r3, r0, #31
30004c66:	2220      	movs	r2, #32
30004c68:	6808      	ldr	r0, [r1, #0]
30004c6a:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30004c6e:	a930      	add	r1, sp, #192	; 0xc0
30004c70:	3a01      	subs	r2, #1
30004c72:	f813 c000 	ldrb.w	ip, [r3, r0]
30004c76:	4419      	add	r1, r3
30004c78:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30004c7c:	f103 0301 	add.w	r3, r3, #1
30004c80:	f801 ccb0 	strb.w	ip, [r1, #-176]
30004c84:	f003 031f 	and.w	r3, r3, #31
30004c88:	d1f1      	bne.n	30004c6e <BOOT_CertificateCheck+0x32>
30004c8a:	4610      	mov	r0, r2
30004c8c:	4b1a      	ldr	r3, [pc, #104]	; (30004cf8 <BOOT_CertificateCheck+0xbc>)
30004c8e:	4798      	blx	r3
30004c90:	7ce3      	ldrb	r3, [r4, #19]
30004c92:	7ca2      	ldrb	r2, [r4, #18]
30004c94:	f10d 010f 	add.w	r1, sp, #15
30004c98:	f10d 000e 	add.w	r0, sp, #14
30004c9c:	f007 fa9c 	bl	3000c1d8 <__SBOOT_Validate_Algorithm_veneer>
30004ca0:	b940      	cbnz	r0, 30004cb4 <BOOT_CertificateCheck+0x78>
30004ca2:	f104 0520 	add.w	r5, r4, #32
30004ca6:	aa04      	add	r2, sp, #16
30004ca8:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004cac:	4629      	mov	r1, r5
30004cae:	f007 faab 	bl	3000c208 <__SBOOT_Validate_PubKey_veneer>
30004cb2:	b130      	cbz	r0, 30004cc2 <BOOT_CertificateCheck+0x86>
30004cb4:	4b11      	ldr	r3, [pc, #68]	; (30004cfc <BOOT_CertificateCheck+0xc0>)
30004cb6:	681b      	ldr	r3, [r3, #0]
30004cb8:	039b      	lsls	r3, r3, #14
30004cba:	d411      	bmi.n	30004ce0 <BOOT_CertificateCheck+0xa4>
30004cbc:	20ff      	movs	r0, #255	; 0xff
30004cbe:	b031      	add	sp, #196	; 0xc4
30004cc0:	bd30      	pop	{r4, r5, pc}
30004cc2:	69a0      	ldr	r0, [r4, #24]
30004cc4:	4623      	mov	r3, r4
30004cc6:	ac0c      	add	r4, sp, #48	; 0x30
30004cc8:	462a      	mov	r2, r5
30004cca:	f89d 100f 	ldrb.w	r1, [sp, #15]
30004cce:	e9cd 0400 	strd	r0, r4, [sp]
30004cd2:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004cd6:	f007 fa77 	bl	3000c1c8 <__SBOOT_Validate_Signature_veneer>
30004cda:	2800      	cmp	r0, #0
30004cdc:	d0b5      	beq.n	30004c4a <BOOT_CertificateCheck+0xe>
30004cde:	e7e9      	b.n	30004cb4 <BOOT_CertificateCheck+0x78>
30004ce0:	4601      	mov	r1, r0
30004ce2:	4807      	ldr	r0, [pc, #28]	; (30004d00 <BOOT_CertificateCheck+0xc4>)
30004ce4:	f007 fa68 	bl	3000c1b8 <__DiagPrintf_veneer>
30004ce8:	20ff      	movs	r0, #255	; 0xff
30004cea:	e7af      	b.n	30004c4c <BOOT_CertificateCheck+0x10>
30004cec:	0000c149 	.word	0x0000c149
30004cf0:	0000ea4d 	.word	0x0000ea4d
30004cf4:	3000d7a4 	.word	0x3000d7a4
30004cf8:	00002451 	.word	0x00002451
30004cfc:	2001c00c 	.word	0x2001c00c
30004d00:	3000c828 	.word	0x3000c828

30004d04 <BOOT_RAM_TZCfg>:
30004d04:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004d08:	b085      	sub	sp, #20
30004d0a:	4e56      	ldr	r6, [pc, #344]	; (30004e64 <BOOT_RAM_TZCfg+0x160>)
30004d0c:	4956      	ldr	r1, [pc, #344]	; (30004e68 <BOOT_RAM_TZCfg+0x164>)
30004d0e:	2501      	movs	r5, #1
30004d10:	f10d 0e08 	add.w	lr, sp, #8
30004d14:	4a55      	ldr	r2, [pc, #340]	; (30004e6c <BOOT_RAM_TZCfg+0x168>)
30004d16:	4b56      	ldr	r3, [pc, #344]	; (30004e70 <BOOT_RAM_TZCfg+0x16c>)
30004d18:	46ec      	mov	ip, sp
30004d1a:	4677      	mov	r7, lr
30004d1c:	f8df 816c 	ldr.w	r8, [pc, #364]	; 30004e8c <BOOT_RAM_TZCfg+0x188>
30004d20:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
30004d24:	f8df 9168 	ldr.w	r9, [pc, #360]	; 30004e90 <BOOT_RAM_TZCfg+0x18c>
30004d28:	9202      	str	r2, [sp, #8]
30004d2a:	9303      	str	r3, [sp, #12]
30004d2c:	e9cd 6100 	strd	r6, r1, [sp]
30004d30:	f85c 2b04 	ldr.w	r2, [ip], #4
30004d34:	2300      	movs	r3, #0
30004d36:	f85e 1b04 	ldr.w	r1, [lr], #4
30004d3a:	680c      	ldr	r4, [r1, #0]
30004d3c:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30004d40:	fa05 f003 	lsl.w	r0, r5, r3
30004d44:	f1b4 3fff 	cmp.w	r4, #4294967295
30004d48:	d00c      	beq.n	30004d64 <BOOT_RAM_TZCfg+0x60>
30004d4a:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30004d4e:	3301      	adds	r3, #1
30004d50:	684c      	ldr	r4, [r1, #4]
30004d52:	310c      	adds	r1, #12
30004d54:	2b08      	cmp	r3, #8
30004d56:	f8cb 4004 	str.w	r4, [fp, #4]
30004d5a:	6c14      	ldr	r4, [r2, #64]	; 0x40
30004d5c:	ea40 0004 	orr.w	r0, r0, r4
30004d60:	6410      	str	r0, [r2, #64]	; 0x40
30004d62:	d1ea      	bne.n	30004d3a <BOOT_RAM_TZCfg+0x36>
30004d64:	42b2      	cmp	r2, r6
30004d66:	d06e      	beq.n	30004e46 <BOOT_RAM_TZCfg+0x142>
30004d68:	6455      	str	r5, [r2, #68]	; 0x44
30004d6a:	45bc      	cmp	ip, r7
30004d6c:	d1e0      	bne.n	30004d30 <BOOT_RAM_TZCfg+0x2c>
30004d6e:	4b41      	ldr	r3, [pc, #260]	; (30004e74 <BOOT_RAM_TZCfg+0x170>)
30004d70:	f46f 3400 	mvn.w	r4, #131072	; 0x20000
30004d74:	f04f 30ff 	mov.w	r0, #4294967295
30004d78:	4a3f      	ldr	r2, [pc, #252]	; (30004e78 <BOOT_RAM_TZCfg+0x174>)
30004d7a:	601c      	str	r4, [r3, #0]
30004d7c:	2100      	movs	r1, #0
30004d7e:	4c3f      	ldr	r4, [pc, #252]	; (30004e7c <BOOT_RAM_TZCfg+0x178>)
30004d80:	6058      	str	r0, [r3, #4]
30004d82:	6813      	ldr	r3, [r2, #0]
30004d84:	3301      	adds	r3, #1
30004d86:	d019      	beq.n	30004dbc <BOOT_RAM_TZCfg+0xb8>
30004d88:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
30004d8c:	320c      	adds	r2, #12
30004d8e:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30004d92:	3101      	adds	r1, #1
30004d94:	f023 031f 	bic.w	r3, r3, #31
30004d98:	2908      	cmp	r1, #8
30004d9a:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
30004d9e:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30004da2:	ea4f 0343 	mov.w	r3, r3, lsl #1
30004da6:	f020 001f 	bic.w	r0, r0, #31
30004daa:	f003 0302 	and.w	r3, r3, #2
30004dae:	ea43 0300 	orr.w	r3, r3, r0
30004db2:	f043 0301 	orr.w	r3, r3, #1
30004db6:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
30004dba:	d1e2      	bne.n	30004d82 <BOOT_RAM_TZCfg+0x7e>
30004dbc:	482f      	ldr	r0, [pc, #188]	; (30004e7c <BOOT_RAM_TZCfg+0x178>)
30004dbe:	2201      	movs	r2, #1
30004dc0:	4b2f      	ldr	r3, [pc, #188]	; (30004e80 <BOOT_RAM_TZCfg+0x17c>)
30004dc2:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30004dc6:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004dca:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30004dce:	d11e      	bne.n	30004e0e <BOOT_RAM_TZCfg+0x10a>
30004dd0:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004dd4:	f3bf 8f4f 	dsb	sy
30004dd8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004ddc:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004de0:	f3c5 344e 	ubfx	r4, r5, #13, #15
30004de4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30004de8:	0164      	lsls	r4, r4, #5
30004dea:	ea04 0106 	and.w	r1, r4, r6
30004dee:	462b      	mov	r3, r5
30004df0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004df4:	3b01      	subs	r3, #1
30004df6:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004dfa:	1c5a      	adds	r2, r3, #1
30004dfc:	d1f8      	bne.n	30004df0 <BOOT_RAM_TZCfg+0xec>
30004dfe:	3c20      	subs	r4, #32
30004e00:	f114 0f20 	cmn.w	r4, #32
30004e04:	d1f1      	bne.n	30004dea <BOOT_RAM_TZCfg+0xe6>
30004e06:	f3bf 8f4f 	dsb	sy
30004e0a:	f3bf 8f6f 	isb	sy
30004e0e:	4b1b      	ldr	r3, [pc, #108]	; (30004e7c <BOOT_RAM_TZCfg+0x178>)
30004e10:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30004e14:	491b      	ldr	r1, [pc, #108]	; (30004e84 <BOOT_RAM_TZCfg+0x180>)
30004e16:	691c      	ldr	r4, [r3, #16]
30004e18:	4a1b      	ldr	r2, [pc, #108]	; (30004e88 <BOOT_RAM_TZCfg+0x184>)
30004e1a:	f044 0408 	orr.w	r4, r4, #8
30004e1e:	611c      	str	r4, [r3, #16]
30004e20:	f04f 34ff 	mov.w	r4, #4294967295
30004e24:	68d8      	ldr	r0, [r3, #12]
30004e26:	4028      	ands	r0, r5
30004e28:	4301      	orrs	r1, r0
30004e2a:	60d9      	str	r1, [r3, #12]
30004e2c:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30004e30:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30004e34:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30004e38:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30004e3c:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30004e40:	b005      	add	sp, #20
30004e42:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004e46:	f898 3000 	ldrb.w	r3, [r8]
30004e4a:	2b00      	cmp	r3, #0
30004e4c:	d18d      	bne.n	30004d6a <BOOT_RAM_TZCfg+0x66>
30004e4e:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
30004e52:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
30004e56:	6c33      	ldr	r3, [r6, #64]	; 0x40
30004e58:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30004e5c:	6433      	str	r3, [r6, #64]	; 0x40
30004e5e:	6475      	str	r5, [r6, #68]	; 0x44
30004e60:	e783      	b.n	30004d6a <BOOT_RAM_TZCfg+0x66>
30004e62:	bf00      	nop
30004e64:	51001a00 	.word	0x51001a00
30004e68:	51001200 	.word	0x51001200
30004e6c:	3000dc70 	.word	0x3000dc70
30004e70:	3000dc10 	.word	0x3000dc10
30004e74:	51001b00 	.word	0x51001b00
30004e78:	3000dcd0 	.word	0x3000dcd0
30004e7c:	e000ed00 	.word	0xe000ed00
30004e80:	42008000 	.word	0x42008000
30004e84:	05fa6000 	.word	0x05fa6000
30004e88:	e000e100 	.word	0xe000e100
30004e8c:	3000d79e 	.word	0x3000d79e
30004e90:	005fffff 	.word	0x005fffff

30004e94 <AsciiToHex>:
30004e94:	f1a0 0341 	sub.w	r3, r0, #65	; 0x41
30004e98:	2b05      	cmp	r3, #5
30004e9a:	d802      	bhi.n	30004ea2 <AsciiToHex+0xe>
30004e9c:	3837      	subs	r0, #55	; 0x37
30004e9e:	b2c0      	uxtb	r0, r0
30004ea0:	4770      	bx	lr
30004ea2:	f1a0 0361 	sub.w	r3, r0, #97	; 0x61
30004ea6:	2b05      	cmp	r3, #5
30004ea8:	d802      	bhi.n	30004eb0 <AsciiToHex+0x1c>
30004eaa:	3857      	subs	r0, #87	; 0x57
30004eac:	b2c0      	uxtb	r0, r0
30004eae:	4770      	bx	lr
30004eb0:	3830      	subs	r0, #48	; 0x30
30004eb2:	b2c0      	uxtb	r0, r0
30004eb4:	280a      	cmp	r0, #10
30004eb6:	bf28      	it	cs
30004eb8:	20ff      	movcs	r0, #255	; 0xff
30004eba:	4770      	bx	lr

30004ebc <hotfix_get_addr_size>:
30004ebc:	2800      	cmp	r0, #0
30004ebe:	d054      	beq.n	30004f6a <hotfix_get_addr_size+0xae>
30004ec0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004ec4:	f04f 0a00 	mov.w	sl, #0
30004ec8:	b083      	sub	sp, #12
30004eca:	f1a3 0b04 	sub.w	fp, r3, #4
30004ece:	1c4b      	adds	r3, r1, #1
30004ed0:	4688      	mov	r8, r1
30004ed2:	4681      	mov	r9, r0
30004ed4:	9300      	str	r3, [sp, #0]
30004ed6:	4617      	mov	r7, r2
30004ed8:	1e53      	subs	r3, r2, #1
30004eda:	f101 0409 	add.w	r4, r1, #9
30004ede:	1dd5      	adds	r5, r2, #7
30004ee0:	4656      	mov	r6, sl
30004ee2:	9301      	str	r3, [sp, #4]
30004ee4:	f8c8 6004 	str.w	r6, [r8, #4]
30004ee8:	210b      	movs	r1, #11
30004eea:	f8c8 6000 	str.w	r6, [r8]
30004eee:	4640      	mov	r0, r8
30004ef0:	f8c8 6007 	str.w	r6, [r8, #7]
30004ef4:	603e      	str	r6, [r7, #0]
30004ef6:	607e      	str	r6, [r7, #4]
30004ef8:	f002 fef2 	bl	30007ce0 <ymodem_uart_getdata>
30004efc:	e9dd 1000 	ldrd	r1, r0, [sp]
30004f00:	e004      	b.n	30004f0c <hotfix_get_addr_size+0x50>
30004f02:	b2d3      	uxtb	r3, r2
30004f04:	428c      	cmp	r4, r1
30004f06:	f800 3f01 	strb.w	r3, [r0, #1]!
30004f0a:	d01a      	beq.n	30004f42 <hotfix_get_addr_size+0x86>
30004f0c:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30004f10:	f1a2 0e41 	sub.w	lr, r2, #65	; 0x41
30004f14:	f1a2 0337 	sub.w	r3, r2, #55	; 0x37
30004f18:	f1a2 0c61 	sub.w	ip, r2, #97	; 0x61
30004f1c:	f1be 0f05 	cmp.w	lr, #5
30004f20:	b2db      	uxtb	r3, r3
30004f22:	d9ef      	bls.n	30004f04 <hotfix_get_addr_size+0x48>
30004f24:	f1a2 0330 	sub.w	r3, r2, #48	; 0x30
30004f28:	f1bc 0f05 	cmp.w	ip, #5
30004f2c:	f1a2 0257 	sub.w	r2, r2, #87	; 0x57
30004f30:	b2db      	uxtb	r3, r3
30004f32:	d9e6      	bls.n	30004f02 <hotfix_get_addr_size+0x46>
30004f34:	2b0a      	cmp	r3, #10
30004f36:	bf28      	it	cs
30004f38:	23ff      	movcs	r3, #255	; 0xff
30004f3a:	428c      	cmp	r4, r1
30004f3c:	f800 3f01 	strb.w	r3, [r0, #1]!
30004f40:	d1e4      	bne.n	30004f0c <hotfix_get_addr_size+0x50>
30004f42:	4639      	mov	r1, r7
30004f44:	2300      	movs	r3, #0
30004f46:	f811 2b01 	ldrb.w	r2, [r1], #1
30004f4a:	4313      	orrs	r3, r2
30004f4c:	42a9      	cmp	r1, r5
30004f4e:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004f52:	d1f8      	bne.n	30004f46 <hotfix_get_addr_size+0x8a>
30004f54:	79fa      	ldrb	r2, [r7, #7]
30004f56:	f10a 0a01 	add.w	sl, sl, #1
30004f5a:	4313      	orrs	r3, r2
30004f5c:	45ca      	cmp	sl, r9
30004f5e:	f84b 3f04 	str.w	r3, [fp, #4]!
30004f62:	d1bf      	bne.n	30004ee4 <hotfix_get_addr_size+0x28>
30004f64:	b003      	add	sp, #12
30004f66:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f6a:	4770      	bx	lr

30004f6c <ascii_to_integer>:
30004f6c:	2900      	cmp	r1, #0
30004f6e:	4684      	mov	ip, r0
30004f70:	dd12      	ble.n	30004f98 <ascii_to_integer+0x2c>
30004f72:	3901      	subs	r1, #1
30004f74:	1e42      	subs	r2, r0, #1
30004f76:	2000      	movs	r0, #0
30004f78:	448c      	add	ip, r1
30004f7a:	e001      	b.n	30004f80 <ascii_to_integer+0x14>
30004f7c:	4562      	cmp	r2, ip
30004f7e:	d00d      	beq.n	30004f9c <ascii_to_integer+0x30>
30004f80:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30004f84:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30004f88:	f1a3 0130 	sub.w	r1, r3, #48	; 0x30
30004f8c:	eb03 0040 	add.w	r0, r3, r0, lsl #1
30004f90:	2909      	cmp	r1, #9
30004f92:	f1a0 0030 	sub.w	r0, r0, #48	; 0x30
30004f96:	d9f1      	bls.n	30004f7c <ascii_to_integer+0x10>
30004f98:	2000      	movs	r0, #0
30004f9a:	4770      	bx	lr
30004f9c:	4770      	bx	lr
30004f9e:	bf00      	nop

30004fa0 <eight_4bit_to_int>:
30004fa0:	2300      	movs	r3, #0
30004fa2:	f100 0c07 	add.w	ip, r0, #7
30004fa6:	4602      	mov	r2, r0
30004fa8:	f812 1b01 	ldrb.w	r1, [r2], #1
30004fac:	430b      	orrs	r3, r1
30004fae:	4562      	cmp	r2, ip
30004fb0:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004fb4:	d1f8      	bne.n	30004fa8 <eight_4bit_to_int+0x8>
30004fb6:	79c0      	ldrb	r0, [r0, #7]
30004fb8:	4318      	orrs	r0, r3
30004fba:	4770      	bx	lr

30004fbc <FLASH_Erase_With_Lock>:
30004fbc:	4b11      	ldr	r3, [pc, #68]	; (30005004 <FLASH_Erase_With_Lock+0x48>)
30004fbe:	b510      	push	{r4, lr}
30004fc0:	460c      	mov	r4, r1
30004fc2:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30004fc6:	4798      	blx	r3
30004fc8:	4b0f      	ldr	r3, [pc, #60]	; (30005008 <FLASH_Erase_With_Lock+0x4c>)
30004fca:	695b      	ldr	r3, [r3, #20]
30004fcc:	03db      	lsls	r3, r3, #15
30004fce:	d512      	bpl.n	30004ff6 <FLASH_Erase_With_Lock+0x3a>
30004fd0:	f014 021f 	ands.w	r2, r4, #31
30004fd4:	d110      	bne.n	30004ff8 <FLASH_Erase_With_Lock+0x3c>
30004fd6:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30004fda:	f3bf 8f4f 	dsb	sy
30004fde:	480a      	ldr	r0, [pc, #40]	; (30005008 <FLASH_Erase_With_Lock+0x4c>)
30004fe0:	4422      	add	r2, r4
30004fe2:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
30004fe6:	3420      	adds	r4, #32
30004fe8:	1b13      	subs	r3, r2, r4
30004fea:	2b00      	cmp	r3, #0
30004fec:	dcf9      	bgt.n	30004fe2 <FLASH_Erase_With_Lock+0x26>
30004fee:	f3bf 8f4f 	dsb	sy
30004ff2:	f3bf 8f6f 	isb	sy
30004ff6:	bd10      	pop	{r4, pc}
30004ff8:	f024 041f 	bic.w	r4, r4, #31
30004ffc:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30005000:	e7eb      	b.n	30004fda <FLASH_Erase_With_Lock+0x1e>
30005002:	bf00      	nop
30005004:	0000a16d 	.word	0x0000a16d
30005008:	e000ed00 	.word	0xe000ed00

3000500c <FLASH_TxData_With_Lock>:
3000500c:	b538      	push	{r3, r4, r5, lr}
3000500e:	4b13      	ldr	r3, [pc, #76]	; (3000505c <FLASH_TxData_With_Lock+0x50>)
30005010:	4604      	mov	r4, r0
30005012:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30005016:	460d      	mov	r5, r1
30005018:	4798      	blx	r3
3000501a:	4b11      	ldr	r3, [pc, #68]	; (30005060 <FLASH_TxData_With_Lock+0x54>)
3000501c:	695b      	ldr	r3, [r3, #20]
3000501e:	03db      	lsls	r3, r3, #15
30005020:	d517      	bpl.n	30005052 <FLASH_TxData_With_Lock+0x46>
30005022:	f014 031f 	ands.w	r3, r4, #31
30005026:	d115      	bne.n	30005054 <FLASH_TxData_With_Lock+0x48>
30005028:	f3bf 8f4f 	dsb	sy
3000502c:	b16d      	cbz	r5, 3000504a <FLASH_TxData_With_Lock+0x3e>
3000502e:	1e6a      	subs	r2, r5, #1
30005030:	f104 0320 	add.w	r3, r4, #32
30005034:	490a      	ldr	r1, [pc, #40]	; (30005060 <FLASH_TxData_With_Lock+0x54>)
30005036:	f022 021f 	bic.w	r2, r2, #31
3000503a:	441a      	add	r2, r3
3000503c:	4293      	cmp	r3, r2
3000503e:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30005042:	461c      	mov	r4, r3
30005044:	f103 0320 	add.w	r3, r3, #32
30005048:	d1f8      	bne.n	3000503c <FLASH_TxData_With_Lock+0x30>
3000504a:	f3bf 8f4f 	dsb	sy
3000504e:	f3bf 8f6f 	isb	sy
30005052:	bd38      	pop	{r3, r4, r5, pc}
30005054:	f024 041f 	bic.w	r4, r4, #31
30005058:	441d      	add	r5, r3
3000505a:	e7e5      	b.n	30005028 <FLASH_TxData_With_Lock+0x1c>
3000505c:	0000a04d 	.word	0x0000a04d
30005060:	e000ed00 	.word	0xe000ed00

30005064 <flash_read_id>:
30005064:	b538      	push	{r3, r4, r5, lr}
30005066:	4d04      	ldr	r5, [pc, #16]	; (30005078 <flash_read_id+0x14>)
30005068:	460c      	mov	r4, r1
3000506a:	4602      	mov	r2, r0
3000506c:	4b03      	ldr	r3, [pc, #12]	; (3000507c <flash_read_id+0x18>)
3000506e:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30005072:	4798      	blx	r3
30005074:	4620      	mov	r0, r4
30005076:	bd38      	pop	{r3, r4, r5, pc}
30005078:	2001c01c 	.word	0x2001c01c
3000507c:	00009db5 	.word	0x00009db5

30005080 <mcc_ReverseBit>:
30005080:	0843      	lsrs	r3, r0, #1
30005082:	08c1      	lsrs	r1, r0, #3
30005084:	f003 0308 	and.w	r3, r3, #8
30005088:	0942      	lsrs	r2, r0, #5
3000508a:	f001 0104 	and.w	r1, r1, #4
3000508e:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30005092:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30005096:	f002 0202 	and.w	r2, r2, #2
3000509a:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
3000509e:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
300050a2:	430b      	orrs	r3, r1
300050a4:	00c1      	lsls	r1, r0, #3
300050a6:	4313      	orrs	r3, r2
300050a8:	f001 0120 	and.w	r1, r1, #32
300050ac:	0042      	lsls	r2, r0, #1
300050ae:	ea43 000c 	orr.w	r0, r3, ip
300050b2:	f002 0310 	and.w	r3, r2, #16
300050b6:	4308      	orrs	r0, r1
300050b8:	4318      	orrs	r0, r3
300050ba:	b2c0      	uxtb	r0, r0
300050bc:	4770      	bx	lr
300050be:	bf00      	nop

300050c0 <mcc_init_crc32>:
300050c0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
300050c2:	2600      	movs	r6, #0
300050c4:	4f16      	ldr	r7, [pc, #88]	; (30005120 <mcc_init_crc32+0x60>)
300050c6:	4605      	mov	r5, r0
300050c8:	b2f0      	uxtb	r0, r6
300050ca:	f7ff ffd9 	bl	30005080 <mcc_ReverseBit>
300050ce:	2208      	movs	r2, #8
300050d0:	0604      	lsls	r4, r0, #24
300050d2:	2c00      	cmp	r4, #0
300050d4:	ea4f 0444 	mov.w	r4, r4, lsl #1
300050d8:	bfb8      	it	lt
300050da:	407c      	eorlt	r4, r7
300050dc:	3a01      	subs	r2, #1
300050de:	d1f8      	bne.n	300050d2 <mcc_init_crc32+0x12>
300050e0:	0e20      	lsrs	r0, r4, #24
300050e2:	3504      	adds	r5, #4
300050e4:	f7ff ffcc 	bl	30005080 <mcc_ReverseBit>
300050e8:	4603      	mov	r3, r0
300050ea:	f3c4 4007 	ubfx	r0, r4, #16, #8
300050ee:	3601      	adds	r6, #1
300050f0:	f805 3c04 	strb.w	r3, [r5, #-4]
300050f4:	f7ff ffc4 	bl	30005080 <mcc_ReverseBit>
300050f8:	4603      	mov	r3, r0
300050fa:	f3c4 2007 	ubfx	r0, r4, #8, #8
300050fe:	f805 3c03 	strb.w	r3, [r5, #-3]
30005102:	f7ff ffbd 	bl	30005080 <mcc_ReverseBit>
30005106:	4603      	mov	r3, r0
30005108:	b2e0      	uxtb	r0, r4
3000510a:	f805 3c02 	strb.w	r3, [r5, #-2]
3000510e:	f7ff ffb7 	bl	30005080 <mcc_ReverseBit>
30005112:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30005116:	f805 0c01 	strb.w	r0, [r5, #-1]
3000511a:	d1d5      	bne.n	300050c8 <mcc_init_crc32+0x8>
3000511c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
3000511e:	bf00      	nop
30005120:	04c11db7 	.word	0x04c11db7

30005124 <mcc_crc32>:
30005124:	b179      	cbz	r1, 30005146 <mcc_crc32+0x22>
30005126:	eb00 0c01 	add.w	ip, r0, r1
3000512a:	f04f 31ff 	mov.w	r1, #4294967295
3000512e:	f810 3b01 	ldrb.w	r3, [r0], #1
30005132:	404b      	eors	r3, r1
30005134:	4560      	cmp	r0, ip
30005136:	b2db      	uxtb	r3, r3
30005138:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
3000513c:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30005140:	d1f5      	bne.n	3000512e <mcc_crc32+0xa>
30005142:	43c8      	mvns	r0, r1
30005144:	4770      	bx	lr
30005146:	4608      	mov	r0, r1
30005148:	4770      	bx	lr
3000514a:	bf00      	nop

3000514c <recovery_check>:
3000514c:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30005150:	4283      	cmp	r3, r0
30005152:	d322      	bcc.n	3000519a <recovery_check+0x4e>
30005154:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
30005158:	d91f      	bls.n	3000519a <recovery_check+0x4e>
3000515a:	b510      	push	{r4, lr}
3000515c:	f8d0 400a 	ldr.w	r4, [r0, #10]
30005160:	f104 030c 	add.w	r3, r4, #12
30005164:	4299      	cmp	r1, r3
30005166:	d316      	bcc.n	30005196 <recovery_check+0x4a>
30005168:	f100 0c04 	add.w	ip, r0, #4
3000516c:	b183      	cbz	r3, 30005190 <recovery_check+0x44>
3000516e:	f100 0e10 	add.w	lr, r0, #16
30005172:	f04f 31ff 	mov.w	r1, #4294967295
30005176:	44a6      	add	lr, r4
30005178:	4c0b      	ldr	r4, [pc, #44]	; (300051a8 <recovery_check+0x5c>)
3000517a:	f81c 3b01 	ldrb.w	r3, [ip], #1
3000517e:	404b      	eors	r3, r1
30005180:	45f4      	cmp	ip, lr
30005182:	b2db      	uxtb	r3, r3
30005184:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005188:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
3000518c:	d1f5      	bne.n	3000517a <recovery_check+0x2e>
3000518e:	43cb      	mvns	r3, r1
30005190:	6801      	ldr	r1, [r0, #0]
30005192:	4299      	cmp	r1, r3
30005194:	d003      	beq.n	3000519e <recovery_check+0x52>
30005196:	2000      	movs	r0, #0
30005198:	bd10      	pop	{r4, pc}
3000519a:	2000      	movs	r0, #0
3000519c:	4770      	bx	lr
3000519e:	f8d0 3006 	ldr.w	r3, [r0, #6]
300051a2:	2001      	movs	r0, #1
300051a4:	6013      	str	r3, [r2, #0]
300051a6:	bd10      	pop	{r4, pc}
300051a8:	3000dd30 	.word	0x3000dd30

300051ac <rewrite_bp>:
300051ac:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300051b0:	b0a3      	sub	sp, #140	; 0x8c
300051b2:	4616      	mov	r6, r2
300051b4:	4607      	mov	r7, r0
300051b6:	460d      	mov	r5, r1
300051b8:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
300051bc:	4699      	mov	r9, r3
300051be:	f10d 0808 	add.w	r8, sp, #8
300051c2:	2280      	movs	r2, #128	; 0x80
300051c4:	2c00      	cmp	r4, #0
300051c6:	f000 80b2 	beq.w	3000532e <rewrite_bp+0x182>
300051ca:	4619      	mov	r1, r3
300051cc:	4640      	mov	r0, r8
300051ce:	f006 ffcb 	bl	3000c168 <____wrap_memcpy_veneer>
300051d2:	2301      	movs	r3, #1
300051d4:	f109 0404 	add.w	r4, r9, #4
300051d8:	4649      	mov	r1, r9
300051da:	2002      	movs	r0, #2
300051dc:	f88d 7014 	strb.w	r7, [sp, #20]
300051e0:	e9cd 3303 	strd	r3, r3, [sp, #12]
300051e4:	f7ff feea 	bl	30004fbc <FLASH_Erase_With_Lock>
300051e8:	4620      	mov	r0, r4
300051ea:	217c      	movs	r1, #124	; 0x7c
300051ec:	aa03      	add	r2, sp, #12
300051ee:	f7ff ff0d 	bl	3000500c <FLASH_TxData_With_Lock>
300051f2:	4620      	mov	r0, r4
300051f4:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
300051f8:	f04f 33ff 	mov.w	r3, #4294967295
300051fc:	4c5c      	ldr	r4, [pc, #368]	; (30005370 <rewrite_bp+0x1c4>)
300051fe:	f810 cb01 	ldrb.w	ip, [r0], #1
30005202:	ea8c 0c03 	eor.w	ip, ip, r3
30005206:	4288      	cmp	r0, r1
30005208:	fa5f fc8c 	uxtb.w	ip, ip
3000520c:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
30005210:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30005214:	d1f3      	bne.n	300051fe <rewrite_bp+0x52>
30005216:	43db      	mvns	r3, r3
30005218:	4649      	mov	r1, r9
3000521a:	2002      	movs	r0, #2
3000521c:	9302      	str	r3, [sp, #8]
3000521e:	f7ff fecd 	bl	30004fbc <FLASH_Erase_With_Lock>
30005222:	4642      	mov	r2, r8
30005224:	4648      	mov	r0, r9
30005226:	2180      	movs	r1, #128	; 0x80
30005228:	f7ff fef0 	bl	3000500c <FLASH_TxData_With_Lock>
3000522c:	4b51      	ldr	r3, [pc, #324]	; (30005374 <rewrite_bp+0x1c8>)
3000522e:	aa01      	add	r2, sp, #4
30005230:	2104      	movs	r1, #4
30005232:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005236:	4b50      	ldr	r3, [pc, #320]	; (30005378 <rewrite_bp+0x1cc>)
30005238:	4798      	blx	r3
3000523a:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000523e:	f04f 0c01 	mov.w	ip, #1
30005242:	f8d5 e00a 	ldr.w	lr, [r5, #10]
30005246:	682a      	ldr	r2, [r5, #0]
30005248:	fa0c fc03 	lsl.w	ip, ip, r3
3000524c:	f10e 0e0c 	add.w	lr, lr, #12
30005250:	f8d6 300a 	ldr.w	r3, [r6, #10]
30005254:	45e6      	cmp	lr, ip
30005256:	6831      	ldr	r1, [r6, #0]
30005258:	f103 030c 	add.w	r3, r3, #12
3000525c:	bf28      	it	cs
3000525e:	46e6      	movcs	lr, ip
30005260:	459c      	cmp	ip, r3
30005262:	bf28      	it	cs
30005264:	469c      	movcs	ip, r3
30005266:	bb9f      	cbnz	r7, 300052d0 <rewrite_bp+0x124>
30005268:	1d28      	adds	r0, r5, #4
3000526a:	f1be 0f00 	cmp.w	lr, #0
3000526e:	d00e      	beq.n	3000528e <rewrite_bp+0xe2>
30005270:	4486      	add	lr, r0
30005272:	f04f 37ff 	mov.w	r7, #4294967295
30005276:	f810 3b01 	ldrb.w	r3, [r0], #1
3000527a:	407b      	eors	r3, r7
3000527c:	4570      	cmp	r0, lr
3000527e:	b2db      	uxtb	r3, r3
30005280:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005284:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30005288:	d1f5      	bne.n	30005276 <rewrite_bp+0xca>
3000528a:	ea6f 0e07 	mvn.w	lr, r7
3000528e:	4572      	cmp	r2, lr
30005290:	d052      	beq.n	30005338 <rewrite_bp+0x18c>
30005292:	1d32      	adds	r2, r6, #4
30005294:	f1bc 0f00 	cmp.w	ip, #0
30005298:	d00e      	beq.n	300052b8 <rewrite_bp+0x10c>
3000529a:	4494      	add	ip, r2
3000529c:	f04f 37ff 	mov.w	r7, #4294967295
300052a0:	f812 3b01 	ldrb.w	r3, [r2], #1
300052a4:	407b      	eors	r3, r7
300052a6:	4562      	cmp	r2, ip
300052a8:	b2db      	uxtb	r3, r3
300052aa:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300052ae:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300052b2:	d1f5      	bne.n	300052a0 <rewrite_bp+0xf4>
300052b4:	ea6f 0c07 	mvn.w	ip, r7
300052b8:	4561      	cmp	r1, ip
300052ba:	d133      	bne.n	30005324 <rewrite_bp+0x178>
300052bc:	4b2f      	ldr	r3, [pc, #188]	; (3000537c <rewrite_bp+0x1d0>)
300052be:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
300052c2:	689b      	ldr	r3, [r3, #8]
300052c4:	0799      	lsls	r1, r3, #30
300052c6:	d44c      	bmi.n	30005362 <rewrite_bp+0x1b6>
300052c8:	2002      	movs	r0, #2
300052ca:	b023      	add	sp, #140	; 0x8c
300052cc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300052d0:	1d30      	adds	r0, r6, #4
300052d2:	f1bc 0f00 	cmp.w	ip, #0
300052d6:	d00e      	beq.n	300052f6 <rewrite_bp+0x14a>
300052d8:	4484      	add	ip, r0
300052da:	f04f 37ff 	mov.w	r7, #4294967295
300052de:	f810 3b01 	ldrb.w	r3, [r0], #1
300052e2:	407b      	eors	r3, r7
300052e4:	4560      	cmp	r0, ip
300052e6:	b2db      	uxtb	r3, r3
300052e8:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300052ec:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300052f0:	d1f5      	bne.n	300052de <rewrite_bp+0x132>
300052f2:	ea6f 0c07 	mvn.w	ip, r7
300052f6:	4561      	cmp	r1, ip
300052f8:	d0e0      	beq.n	300052bc <rewrite_bp+0x110>
300052fa:	1d29      	adds	r1, r5, #4
300052fc:	f1be 0f00 	cmp.w	lr, #0
30005300:	d00e      	beq.n	30005320 <rewrite_bp+0x174>
30005302:	448e      	add	lr, r1
30005304:	f04f 37ff 	mov.w	r7, #4294967295
30005308:	f811 3b01 	ldrb.w	r3, [r1], #1
3000530c:	407b      	eors	r3, r7
3000530e:	4571      	cmp	r1, lr
30005310:	b2db      	uxtb	r3, r3
30005312:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005316:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
3000531a:	d1f5      	bne.n	30005308 <rewrite_bp+0x15c>
3000531c:	ea6f 0e07 	mvn.w	lr, r7
30005320:	4572      	cmp	r2, lr
30005322:	d009      	beq.n	30005338 <rewrite_bp+0x18c>
30005324:	4b15      	ldr	r3, [pc, #84]	; (3000537c <rewrite_bp+0x1d0>)
30005326:	689b      	ldr	r3, [r3, #8]
30005328:	079b      	lsls	r3, r3, #30
3000532a:	d40f      	bmi.n	3000534c <rewrite_bp+0x1a0>
3000532c:	e7fe      	b.n	3000532c <rewrite_bp+0x180>
3000532e:	992a      	ldr	r1, [sp, #168]	; 0xa8
30005330:	4640      	mov	r0, r8
30005332:	f006 ff19 	bl	3000c168 <____wrap_memcpy_veneer>
30005336:	e74c      	b.n	300051d2 <rewrite_bp+0x26>
30005338:	4b10      	ldr	r3, [pc, #64]	; (3000537c <rewrite_bp+0x1d0>)
3000533a:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
3000533e:	689b      	ldr	r3, [r3, #8]
30005340:	079a      	lsls	r2, r3, #30
30005342:	d407      	bmi.n	30005354 <rewrite_bp+0x1a8>
30005344:	2001      	movs	r0, #1
30005346:	b023      	add	sp, #140	; 0x8c
30005348:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000534c:	480c      	ldr	r0, [pc, #48]	; (30005380 <rewrite_bp+0x1d4>)
3000534e:	f006 ff33 	bl	3000c1b8 <__DiagPrintf_veneer>
30005352:	e7eb      	b.n	3000532c <rewrite_bp+0x180>
30005354:	f8d5 1006 	ldr.w	r1, [r5, #6]
30005358:	480a      	ldr	r0, [pc, #40]	; (30005384 <rewrite_bp+0x1d8>)
3000535a:	f006 ff2d 	bl	3000c1b8 <__DiagPrintf_veneer>
3000535e:	2001      	movs	r0, #1
30005360:	e7b3      	b.n	300052ca <rewrite_bp+0x11e>
30005362:	f8d6 1006 	ldr.w	r1, [r6, #6]
30005366:	4808      	ldr	r0, [pc, #32]	; (30005388 <rewrite_bp+0x1dc>)
30005368:	f006 ff26 	bl	3000c1b8 <__DiagPrintf_veneer>
3000536c:	e7ac      	b.n	300052c8 <rewrite_bp+0x11c>
3000536e:	bf00      	nop
30005370:	3000dd30 	.word	0x3000dd30
30005374:	2001c01c 	.word	0x2001c01c
30005378:	00009db5 	.word	0x00009db5
3000537c:	2001c00c 	.word	0x2001c00c
30005380:	3000c8ac 	.word	0x3000c8ac
30005384:	3000c84c 	.word	0x3000c84c
30005388:	3000c87c 	.word	0x3000c87c

3000538c <BOOT_SectionInit>:
3000538c:	4800      	ldr	r0, [pc, #0]	; (30005390 <BOOT_SectionInit+0x4>)
3000538e:	4770      	bx	lr
30005390:	60000020 	.word	0x60000020

30005394 <BOOT_NsStart>:
30005394:	b570      	push	{r4, r5, r6, lr}
30005396:	4c23      	ldr	r4, [pc, #140]	; (30005424 <BOOT_NsStart+0x90>)
30005398:	4605      	mov	r5, r0
3000539a:	f384 8808 	msr	MSP, r4
3000539e:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300053a2:	2100      	movs	r1, #0
300053a4:	4620      	mov	r0, r4
300053a6:	4e20      	ldr	r6, [pc, #128]	; (30005428 <BOOT_NsStart+0x94>)
300053a8:	47b0      	blx	r6
300053aa:	4a20      	ldr	r2, [pc, #128]	; (3000542c <BOOT_NsStart+0x98>)
300053ac:	6953      	ldr	r3, [r2, #20]
300053ae:	03d9      	lsls	r1, r3, #15
300053b0:	d50b      	bpl.n	300053ca <BOOT_NsStart+0x36>
300053b2:	f3bf 8f4f 	dsb	sy
300053b6:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
300053ba:	3420      	adds	r4, #32
300053bc:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
300053c0:	d1f9      	bne.n	300053b6 <BOOT_NsStart+0x22>
300053c2:	f3bf 8f4f 	dsb	sy
300053c6:	f3bf 8f6f 	isb	sy
300053ca:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
300053ce:	f383 8808 	msr	MSP, r3
300053d2:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300053d6:	2100      	movs	r1, #0
300053d8:	4815      	ldr	r0, [pc, #84]	; (30005430 <BOOT_NsStart+0x9c>)
300053da:	47b0      	blx	r6
300053dc:	4a13      	ldr	r2, [pc, #76]	; (3000542c <BOOT_NsStart+0x98>)
300053de:	6953      	ldr	r3, [r2, #20]
300053e0:	03db      	lsls	r3, r3, #15
300053e2:	d50c      	bpl.n	300053fe <BOOT_NsStart+0x6a>
300053e4:	f3bf 8f4f 	dsb	sy
300053e8:	4b11      	ldr	r3, [pc, #68]	; (30005430 <BOOT_NsStart+0x9c>)
300053ea:	490e      	ldr	r1, [pc, #56]	; (30005424 <BOOT_NsStart+0x90>)
300053ec:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
300053f0:	3320      	adds	r3, #32
300053f2:	428b      	cmp	r3, r1
300053f4:	d1fa      	bne.n	300053ec <BOOT_NsStart+0x58>
300053f6:	f3bf 8f4f 	dsb	sy
300053fa:	f3bf 8f6f 	isb	sy
300053fe:	f025 0501 	bic.w	r5, r5, #1
30005402:	086d      	lsrs	r5, r5, #1
30005404:	006d      	lsls	r5, r5, #1
30005406:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
3000540a:	ed2d 8b10 	vpush	{d8-d15}
3000540e:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
30005412:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
30005416:	47ac      	blxns	r5
30005418:	ecbd 8b10 	vpop	{d8-d15}
3000541c:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005420:	bd70      	pop	{r4, r5, r6, pc}
30005422:	bf00      	nop
30005424:	30002000 	.word	0x30002000
30005428:	00012be5 	.word	0x00012be5
3000542c:	e000ed00 	.word	0xe000ed00
30005430:	30001000 	.word	0x30001000

30005434 <BOOT_RccConfig>:
30005434:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30005438:	4c2f      	ldr	r4, [pc, #188]	; (300054f8 <BOOT_RccConfig+0xc4>)
3000543a:	b090      	sub	sp, #64	; 0x40
3000543c:	4f2f      	ldr	r7, [pc, #188]	; (300054fc <BOOT_RccConfig+0xc8>)
3000543e:	466d      	mov	r5, sp
30005440:	f10d 0c20 	add.w	ip, sp, #32
30005444:	683e      	ldr	r6, [r7, #0]
30005446:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
3000544a:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
3000544e:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30005452:	f104 0310 	add.w	r3, r4, #16
30005456:	2400      	movs	r4, #0
30005458:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000545a:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
3000545e:	1c72      	adds	r2, r6, #1
30005460:	e9cd 4404 	strd	r4, r4, [sp, #16]
30005464:	e9cd 4406 	strd	r4, r4, [sp, #24]
30005468:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000546c:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30005470:	d03f      	beq.n	300054f2 <BOOT_RccConfig+0xbe>
30005472:	463a      	mov	r2, r7
30005474:	6891      	ldr	r1, [r2, #8]
30005476:	0fb3      	lsrs	r3, r6, #30
30005478:	a810      	add	r0, sp, #64	; 0x40
3000547a:	2901      	cmp	r1, #1
3000547c:	eb00 0383 	add.w	r3, r0, r3, lsl #2
30005480:	d10e      	bne.n	300054a0 <BOOT_RccConfig+0x6c>
30005482:	f853 0c30 	ldr.w	r0, [r3, #-48]
30005486:	6854      	ldr	r4, [r2, #4]
30005488:	4330      	orrs	r0, r6
3000548a:	ae10      	add	r6, sp, #64	; 0x40
3000548c:	0fa1      	lsrs	r1, r4, #30
3000548e:	f843 0c30 	str.w	r0, [r3, #-48]
30005492:	eb06 0181 	add.w	r1, r6, r1, lsl #2
30005496:	f851 3c10 	ldr.w	r3, [r1, #-16]
3000549a:	4323      	orrs	r3, r4
3000549c:	f841 3c10 	str.w	r3, [r1, #-16]
300054a0:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300054a4:	1c73      	adds	r3, r6, #1
300054a6:	d1e5      	bne.n	30005474 <BOOT_RccConfig+0x40>
300054a8:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300054aa:	9c04      	ldr	r4, [sp, #16]
300054ac:	af0d      	add	r7, sp, #52	; 0x34
300054ae:	ae05      	add	r6, sp, #20
300054b0:	4a13      	ldr	r2, [pc, #76]	; (30005500 <BOOT_RccConfig+0xcc>)
300054b2:	f85c 0b04 	ldr.w	r0, [ip], #4
300054b6:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300054ba:	f855 1b04 	ldr.w	r1, [r5], #4
300054be:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300054c2:	f850 8002 	ldr.w	r8, [r0, r2]
300054c6:	45e6      	cmp	lr, ip
300054c8:	ea43 0308 	orr.w	r3, r3, r8
300054cc:	5083      	str	r3, [r0, r2]
300054ce:	588b      	ldr	r3, [r1, r2]
300054d0:	ea44 0403 	orr.w	r4, r4, r3
300054d4:	508c      	str	r4, [r1, r2]
300054d6:	d004      	beq.n	300054e2 <BOOT_RccConfig+0xae>
300054d8:	f857 3b04 	ldr.w	r3, [r7], #4
300054dc:	f856 4b04 	ldr.w	r4, [r6], #4
300054e0:	e7e7      	b.n	300054b2 <BOOT_RccConfig+0x7e>
300054e2:	200a      	movs	r0, #10
300054e4:	4b07      	ldr	r3, [pc, #28]	; (30005504 <BOOT_RccConfig+0xd0>)
300054e6:	4798      	blx	r3
300054e8:	4b07      	ldr	r3, [pc, #28]	; (30005508 <BOOT_RccConfig+0xd4>)
300054ea:	b010      	add	sp, #64	; 0x40
300054ec:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300054f0:	4718      	bx	r3
300054f2:	4623      	mov	r3, r4
300054f4:	e7da      	b.n	300054ac <BOOT_RccConfig+0x78>
300054f6:	bf00      	nop
300054f8:	3000d734 	.word	0x3000d734
300054fc:	3000d888 	.word	0x3000d888
30005500:	42008000 	.word	0x42008000
30005504:	00009b2d 	.word	0x00009b2d
30005508:	30007fe9 	.word	0x30007fe9

3000550c <BOOT_CACHEWRR_Set>:
3000550c:	4b14      	ldr	r3, [pc, #80]	; (30005560 <BOOT_CACHEWRR_Set+0x54>)
3000550e:	b430      	push	{r4, r5}
30005510:	681a      	ldr	r2, [r3, #0]
30005512:	2a01      	cmp	r2, #1
30005514:	d10f      	bne.n	30005536 <BOOT_CACHEWRR_Set+0x2a>
30005516:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
3000551a:	685d      	ldr	r5, [r3, #4]
3000551c:	689c      	ldr	r4, [r3, #8]
3000551e:	6882      	ldr	r2, [r0, #8]
30005520:	4910      	ldr	r1, [pc, #64]	; (30005564 <BOOT_CACHEWRR_Set+0x58>)
30005522:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30005526:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
3000552a:	6082      	str	r2, [r0, #8]
3000552c:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30005530:	68da      	ldr	r2, [r3, #12]
30005532:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
30005536:	691a      	ldr	r2, [r3, #16]
30005538:	2a01      	cmp	r2, #1
3000553a:	d10f      	bne.n	3000555c <BOOT_CACHEWRR_Set+0x50>
3000553c:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30005540:	695d      	ldr	r5, [r3, #20]
30005542:	4a08      	ldr	r2, [pc, #32]	; (30005564 <BOOT_CACHEWRR_Set+0x58>)
30005544:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30005548:	688b      	ldr	r3, [r1, #8]
3000554a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000554e:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
30005552:	608b      	str	r3, [r1, #8]
30005554:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30005558:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
3000555c:	bc30      	pop	{r4, r5}
3000555e:	4770      	bx	lr
30005560:	3000e394 	.word	0x3000e394
30005564:	e0042000 	.word	0xe0042000

30005568 <BOOT_TCMSet>:
30005568:	2300      	movs	r3, #0
3000556a:	b5f0      	push	{r4, r5, r6, r7, lr}
3000556c:	4db3      	ldr	r5, [pc, #716]	; (3000583c <BOOT_TCMSet+0x2d4>)
3000556e:	461c      	mov	r4, r3
30005570:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
30005574:	3307      	adds	r3, #7
30005576:	4282      	cmp	r2, r0
30005578:	d03c      	beq.n	300055f4 <BOOT_TCMSet+0x8c>
3000557a:	3401      	adds	r4, #1
3000557c:	2c07      	cmp	r4, #7
3000557e:	d1f7      	bne.n	30005570 <BOOT_TCMSet+0x8>
30005580:	f3bf 8f4f 	dsb	sy
30005584:	f3bf 8f6f 	isb	sy
30005588:	48ad      	ldr	r0, [pc, #692]	; (30005840 <BOOT_TCMSet+0x2d8>)
3000558a:	2200      	movs	r2, #0
3000558c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005590:	f3bf 8f4f 	dsb	sy
30005594:	f3bf 8f6f 	isb	sy
30005598:	6943      	ldr	r3, [r0, #20]
3000559a:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
3000559e:	6143      	str	r3, [r0, #20]
300055a0:	f3bf 8f4f 	dsb	sy
300055a4:	f3bf 8f6f 	isb	sy
300055a8:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300055ac:	f3bf 8f4f 	dsb	sy
300055b0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300055b4:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300055b8:	f3c5 344e 	ubfx	r4, r5, #13, #15
300055bc:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300055c0:	0164      	lsls	r4, r4, #5
300055c2:	ea04 0106 	and.w	r1, r4, r6
300055c6:	462b      	mov	r3, r5
300055c8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300055cc:	3b01      	subs	r3, #1
300055ce:	1c5f      	adds	r7, r3, #1
300055d0:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300055d4:	d1f8      	bne.n	300055c8 <BOOT_TCMSet+0x60>
300055d6:	3c20      	subs	r4, #32
300055d8:	f114 0f20 	cmn.w	r4, #32
300055dc:	d1f1      	bne.n	300055c2 <BOOT_TCMSet+0x5a>
300055de:	f3bf 8f4f 	dsb	sy
300055e2:	6943      	ldr	r3, [r0, #20]
300055e4:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300055e8:	6143      	str	r3, [r0, #20]
300055ea:	f3bf 8f4f 	dsb	sy
300055ee:	f3bf 8f6f 	isb	sy
300055f2:	bdf0      	pop	{r4, r5, r6, r7, pc}
300055f4:	2c00      	cmp	r4, #0
300055f6:	d0c3      	beq.n	30005580 <BOOT_TCMSet+0x18>
300055f8:	f3bf 8f4f 	dsb	sy
300055fc:	f3bf 8f6f 	isb	sy
30005600:	4890      	ldr	r0, [pc, #576]	; (30005844 <BOOT_TCMSet+0x2dc>)
30005602:	2300      	movs	r3, #0
30005604:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30005608:	f3bf 8f4f 	dsb	sy
3000560c:	f3bf 8f6f 	isb	sy
30005610:	6942      	ldr	r2, [r0, #20]
30005612:	03d2      	lsls	r2, r2, #15
30005614:	d520      	bpl.n	30005658 <BOOT_TCMSet+0xf0>
30005616:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000561a:	f3bf 8f4f 	dsb	sy
3000561e:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30005622:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005626:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000562a:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000562e:	ea4f 1c43 	mov.w	ip, r3, lsl #5
30005632:	ea0c 0106 	and.w	r1, ip, r6
30005636:	4673      	mov	r3, lr
30005638:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000563c:	3b01      	subs	r3, #1
3000563e:	1c5f      	adds	r7, r3, #1
30005640:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30005644:	d1f8      	bne.n	30005638 <BOOT_TCMSet+0xd0>
30005646:	f1ac 0c20 	sub.w	ip, ip, #32
3000564a:	f11c 0f20 	cmn.w	ip, #32
3000564e:	d1f0      	bne.n	30005632 <BOOT_TCMSet+0xca>
30005650:	f3bf 8f4f 	dsb	sy
30005654:	f3bf 8f6f 	isb	sy
30005658:	f3bf 8f4f 	dsb	sy
3000565c:	f3bf 8f6f 	isb	sy
30005660:	4878      	ldr	r0, [pc, #480]	; (30005844 <BOOT_TCMSet+0x2dc>)
30005662:	2200      	movs	r2, #0
30005664:	6943      	ldr	r3, [r0, #20]
30005666:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
3000566a:	6143      	str	r3, [r0, #20]
3000566c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005670:	f3bf 8f4f 	dsb	sy
30005674:	f3bf 8f6f 	isb	sy
30005678:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
3000567c:	f3bf 8f4f 	dsb	sy
30005680:	6943      	ldr	r3, [r0, #20]
30005682:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30005686:	6143      	str	r3, [r0, #20]
30005688:	f3bf 8f4f 	dsb	sy
3000568c:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30005690:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30005694:	f3c3 06c9 	ubfx	r6, r3, #3, #10
30005698:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000569c:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300056a0:	ea0c 010e 	and.w	r1, ip, lr
300056a4:	4633      	mov	r3, r6
300056a6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300056aa:	3b01      	subs	r3, #1
300056ac:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300056b0:	1c5a      	adds	r2, r3, #1
300056b2:	d1f8      	bne.n	300056a6 <BOOT_TCMSet+0x13e>
300056b4:	f1ac 0c20 	sub.w	ip, ip, #32
300056b8:	f11c 0f20 	cmn.w	ip, #32
300056bc:	d1f0      	bne.n	300056a0 <BOOT_TCMSet+0x138>
300056be:	f3bf 8f4f 	dsb	sy
300056c2:	f3bf 8f6f 	isb	sy
300056c6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300056ca:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300056ce:	00e3      	lsls	r3, r4, #3
300056d0:	6b41      	ldr	r1, [r0, #52]	; 0x34
300056d2:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300056d6:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
300056da:	6341      	str	r1, [r0, #52]	; 0x34
300056dc:	6911      	ldr	r1, [r2, #16]
300056de:	2903      	cmp	r1, #3
300056e0:	d00f      	beq.n	30005702 <BOOT_TCMSet+0x19a>
300056e2:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300056e6:	4858      	ldr	r0, [pc, #352]	; (30005848 <BOOT_TCMSet+0x2e0>)
300056e8:	f04f 0c00 	mov.w	ip, #0
300056ec:	4f57      	ldr	r7, [pc, #348]	; (3000584c <BOOT_TCMSet+0x2e4>)
300056ee:	68b2      	ldr	r2, [r6, #8]
300056f0:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
300056f4:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
300056f8:	60b2      	str	r2, [r6, #8]
300056fa:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
300056fe:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
30005702:	1b1a      	subs	r2, r3, r4
30005704:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005708:	68d1      	ldr	r1, [r2, #12]
3000570a:	2903      	cmp	r1, #3
3000570c:	d00f      	beq.n	3000572e <BOOT_TCMSet+0x1c6>
3000570e:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
30005712:	484d      	ldr	r0, [pc, #308]	; (30005848 <BOOT_TCMSet+0x2e0>)
30005714:	f04f 0c00 	mov.w	ip, #0
30005718:	4f4c      	ldr	r7, [pc, #304]	; (3000584c <BOOT_TCMSet+0x2e4>)
3000571a:	68b2      	ldr	r2, [r6, #8]
3000571c:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30005720:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
30005724:	60b2      	str	r2, [r6, #8]
30005726:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
3000572a:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
3000572e:	1b1a      	subs	r2, r3, r4
30005730:	4945      	ldr	r1, [pc, #276]	; (30005848 <BOOT_TCMSet+0x2e0>)
30005732:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005736:	6950      	ldr	r0, [r2, #20]
30005738:	6608      	str	r0, [r1, #96]	; 0x60
3000573a:	6990      	ldr	r0, [r2, #24]
3000573c:	6648      	str	r0, [r1, #100]	; 0x64
3000573e:	6852      	ldr	r2, [r2, #4]
30005740:	2a01      	cmp	r2, #1
30005742:	d04b      	beq.n	300057dc <BOOT_TCMSet+0x274>
30005744:	1b1b      	subs	r3, r3, r4
30005746:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000574a:	68ab      	ldr	r3, [r5, #8]
3000574c:	2b01      	cmp	r3, #1
3000574e:	f47f af50 	bne.w	300055f2 <BOOT_TCMSet+0x8a>
30005752:	483b      	ldr	r0, [pc, #236]	; (30005840 <BOOT_TCMSet+0x2d8>)
30005754:	2300      	movs	r3, #0
30005756:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000575a:	f3bf 8f4f 	dsb	sy
3000575e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30005762:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005766:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000576a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000576e:	0164      	lsls	r4, r4, #5
30005770:	ea04 0106 	and.w	r1, r4, r6
30005774:	462b      	mov	r3, r5
30005776:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000577a:	3b01      	subs	r3, #1
3000577c:	1c5f      	adds	r7, r3, #1
3000577e:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30005782:	d1f8      	bne.n	30005776 <BOOT_TCMSet+0x20e>
30005784:	3c20      	subs	r4, #32
30005786:	f114 0f20 	cmn.w	r4, #32
3000578a:	d1f1      	bne.n	30005770 <BOOT_TCMSet+0x208>
3000578c:	f3bf 8f4f 	dsb	sy
30005790:	6943      	ldr	r3, [r0, #20]
30005792:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30005796:	6143      	str	r3, [r0, #20]
30005798:	f3bf 8f4f 	dsb	sy
3000579c:	f3bf 8f6f 	isb	sy
300057a0:	4828      	ldr	r0, [pc, #160]	; (30005844 <BOOT_TCMSet+0x2dc>)
300057a2:	2300      	movs	r3, #0
300057a4:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300057a8:	f3bf 8f4f 	dsb	sy
300057ac:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300057b0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300057b4:	f3c5 344e 	ubfx	r4, r5, #13, #15
300057b8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300057bc:	0164      	lsls	r4, r4, #5
300057be:	ea04 0106 	and.w	r1, r4, r6
300057c2:	462b      	mov	r3, r5
300057c4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300057c8:	3b01      	subs	r3, #1
300057ca:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300057ce:	1c5a      	adds	r2, r3, #1
300057d0:	d1f8      	bne.n	300057c4 <BOOT_TCMSet+0x25c>
300057d2:	3c20      	subs	r4, #32
300057d4:	f114 0f20 	cmn.w	r4, #32
300057d8:	d1f1      	bne.n	300057be <BOOT_TCMSet+0x256>
300057da:	e700      	b.n	300055de <BOOT_TCMSet+0x76>
300057dc:	f3bf 8f4f 	dsb	sy
300057e0:	f3bf 8f6f 	isb	sy
300057e4:	4a16      	ldr	r2, [pc, #88]	; (30005840 <BOOT_TCMSet+0x2d8>)
300057e6:	2000      	movs	r0, #0
300057e8:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300057ec:	f3bf 8f4f 	dsb	sy
300057f0:	f3bf 8f6f 	isb	sy
300057f4:	6951      	ldr	r1, [r2, #20]
300057f6:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300057fa:	6151      	str	r1, [r2, #20]
300057fc:	f3bf 8f4f 	dsb	sy
30005800:	f3bf 8f6f 	isb	sy
30005804:	f3bf 8f4f 	dsb	sy
30005808:	f3bf 8f6f 	isb	sy
3000580c:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30005810:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30005814:	f3bf 8f4f 	dsb	sy
30005818:	f3bf 8f6f 	isb	sy
3000581c:	6951      	ldr	r1, [r2, #20]
3000581e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30005822:	6151      	str	r1, [r2, #20]
30005824:	f3bf 8f4f 	dsb	sy
30005828:	f3bf 8f6f 	isb	sy
3000582c:	1b1b      	subs	r3, r3, r4
3000582e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30005832:	68ab      	ldr	r3, [r5, #8]
30005834:	2b01      	cmp	r3, #1
30005836:	f47f aedc 	bne.w	300055f2 <BOOT_TCMSet+0x8a>
3000583a:	e78a      	b.n	30005752 <BOOT_TCMSet+0x1ea>
3000583c:	3000e2d0 	.word	0x3000e2d0
30005840:	e002ed00 	.word	0xe002ed00
30005844:	e000ed00 	.word	0xe000ed00
30005848:	e0042000 	.word	0xe0042000
3000584c:	ffff0000 	.word	0xffff0000

30005850 <BOOT_GRstConfig>:
30005850:	4b13      	ldr	r3, [pc, #76]	; (300058a0 <BOOT_GRstConfig+0x50>)
30005852:	b570      	push	{r4, r5, r6, lr}
30005854:	681a      	ldr	r2, [r3, #0]
30005856:	1c50      	adds	r0, r2, #1
30005858:	d01e      	beq.n	30005898 <BOOT_GRstConfig+0x48>
3000585a:	2000      	movs	r0, #0
3000585c:	6859      	ldr	r1, [r3, #4]
3000585e:	2901      	cmp	r1, #1
30005860:	bf08      	it	eq
30005862:	4310      	orreq	r0, r2
30005864:	f853 2f08 	ldr.w	r2, [r3, #8]!
30005868:	1c51      	adds	r1, r2, #1
3000586a:	d1f7      	bne.n	3000585c <BOOT_GRstConfig+0xc>
3000586c:	b284      	uxth	r4, r0
3000586e:	43e3      	mvns	r3, r4
30005870:	b29e      	uxth	r6, r3
30005872:	4d0c      	ldr	r5, [pc, #48]	; (300058a4 <BOOT_GRstConfig+0x54>)
30005874:	2028      	movs	r0, #40	; 0x28
30005876:	4a0c      	ldr	r2, [pc, #48]	; (300058a8 <BOOT_GRstConfig+0x58>)
30005878:	882b      	ldrh	r3, [r5, #0]
3000587a:	b29b      	uxth	r3, r3
3000587c:	4323      	orrs	r3, r4
3000587e:	802b      	strh	r3, [r5, #0]
30005880:	4790      	blx	r2
30005882:	4a0a      	ldr	r2, [pc, #40]	; (300058ac <BOOT_GRstConfig+0x5c>)
30005884:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
30005888:	4033      	ands	r3, r6
3000588a:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
3000588e:	8868      	ldrh	r0, [r5, #2]
30005890:	b280      	uxth	r0, r0
30005892:	4320      	orrs	r0, r4
30005894:	8068      	strh	r0, [r5, #2]
30005896:	bd70      	pop	{r4, r5, r6, pc}
30005898:	f64f 76ff 	movw	r6, #65535	; 0xffff
3000589c:	2400      	movs	r4, #0
3000589e:	e7e8      	b.n	30005872 <BOOT_GRstConfig+0x22>
300058a0:	3000d868 	.word	0x3000d868
300058a4:	42008200 	.word	0x42008200
300058a8:	00009b2d 	.word	0x00009b2d
300058ac:	42008000 	.word	0x42008000

300058b0 <BOOT_DDR_LCDC_HPR>:
300058b0:	4a04      	ldr	r2, [pc, #16]	; (300058c4 <BOOT_DDR_LCDC_HPR+0x14>)
300058b2:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300058b6:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300058ba:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300058be:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300058c2:	4770      	bx	lr
300058c4:	42008000 	.word	0x42008000

300058c8 <BOOT_DDR_Init>:
300058c8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300058cc:	b530      	push	{r4, r5, lr}
300058ce:	4d15      	ldr	r5, [pc, #84]	; (30005924 <BOOT_DDR_Init+0x5c>)
300058d0:	b099      	sub	sp, #100	; 0x64
300058d2:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300058d4:	47a8      	blx	r5
300058d6:	2803      	cmp	r0, #3
300058d8:	d11e      	bne.n	30005918 <BOOT_DDR_Init+0x50>
300058da:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300058de:	4a12      	ldr	r2, [pc, #72]	; (30005928 <BOOT_DDR_Init+0x60>)
300058e0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300058e4:	4911      	ldr	r1, [pc, #68]	; (3000592c <BOOT_DDR_Init+0x64>)
300058e6:	a801      	add	r0, sp, #4
300058e8:	631c      	str	r4, [r3, #48]	; 0x30
300058ea:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300058ee:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300058f2:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300058f6:	4788      	blx	r1
300058f8:	a801      	add	r0, sp, #4
300058fa:	4b0d      	ldr	r3, [pc, #52]	; (30005930 <BOOT_DDR_Init+0x68>)
300058fc:	4798      	blx	r3
300058fe:	4b0d      	ldr	r3, [pc, #52]	; (30005934 <BOOT_DDR_Init+0x6c>)
30005900:	4798      	blx	r3
30005902:	4b0d      	ldr	r3, [pc, #52]	; (30005938 <BOOT_DDR_Init+0x70>)
30005904:	4798      	blx	r3
30005906:	4b0d      	ldr	r3, [pc, #52]	; (3000593c <BOOT_DDR_Init+0x74>)
30005908:	4798      	blx	r3
3000590a:	2101      	movs	r1, #1
3000590c:	f240 10ff 	movw	r0, #511	; 0x1ff
30005910:	4b0b      	ldr	r3, [pc, #44]	; (30005940 <BOOT_DDR_Init+0x78>)
30005912:	4798      	blx	r3
30005914:	b019      	add	sp, #100	; 0x64
30005916:	bd30      	pop	{r4, r5, pc}
30005918:	47a8      	blx	r5
3000591a:	2802      	cmp	r0, #2
3000591c:	d0dd      	beq.n	300058da <BOOT_DDR_Init+0x12>
3000591e:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
30005922:	e7dc      	b.n	300058de <BOOT_DDR_Init+0x16>
30005924:	0000c0f9 	.word	0x0000c0f9
30005928:	42008000 	.word	0x42008000
3000592c:	30008061 	.word	0x30008061
30005930:	30008a39 	.word	0x30008a39
30005934:	3000b94d 	.word	0x3000b94d
30005938:	30008b09 	.word	0x30008b09
3000593c:	30008b0d 	.word	0x30008b0d
30005940:	3000b8f5 	.word	0x3000b8f5

30005944 <BOOT_PSRAM_Init>:
30005944:	4a1f      	ldr	r2, [pc, #124]	; (300059c4 <BOOT_PSRAM_Init+0x80>)
30005946:	4920      	ldr	r1, [pc, #128]	; (300059c8 <BOOT_PSRAM_Init+0x84>)
30005948:	b500      	push	{lr}
3000594a:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
3000594e:	b089      	sub	sp, #36	; 0x24
30005950:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005954:	a801      	add	r0, sp, #4
30005956:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
3000595a:	4788      	blx	r1
3000595c:	a801      	add	r0, sp, #4
3000595e:	4b1b      	ldr	r3, [pc, #108]	; (300059cc <BOOT_PSRAM_Init+0x88>)
30005960:	4798      	blx	r3
30005962:	4b1b      	ldr	r3, [pc, #108]	; (300059d0 <BOOT_PSRAM_Init+0x8c>)
30005964:	4798      	blx	r3
30005966:	4b1b      	ldr	r3, [pc, #108]	; (300059d4 <BOOT_PSRAM_Init+0x90>)
30005968:	4798      	blx	r3
3000596a:	2801      	cmp	r0, #1
3000596c:	d026      	beq.n	300059bc <BOOT_PSRAM_Init+0x78>
3000596e:	4b1a      	ldr	r3, [pc, #104]	; (300059d8 <BOOT_PSRAM_Init+0x94>)
30005970:	689b      	ldr	r3, [r3, #8]
30005972:	079a      	lsls	r2, r3, #30
30005974:	d41e      	bmi.n	300059b4 <BOOT_PSRAM_Init+0x70>
30005976:	4b19      	ldr	r3, [pc, #100]	; (300059dc <BOOT_PSRAM_Init+0x98>)
30005978:	4798      	blx	r3
3000597a:	4b12      	ldr	r3, [pc, #72]	; (300059c4 <BOOT_PSRAM_Init+0x80>)
3000597c:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30005980:	009b      	lsls	r3, r3, #2
30005982:	d510      	bpl.n	300059a6 <BOOT_PSRAM_Init+0x62>
30005984:	4a16      	ldr	r2, [pc, #88]	; (300059e0 <BOOT_PSRAM_Init+0x9c>)
30005986:	4b17      	ldr	r3, [pc, #92]	; (300059e4 <BOOT_PSRAM_Init+0xa0>)
30005988:	6812      	ldr	r2, [r2, #0]
3000598a:	605a      	str	r2, [r3, #4]
3000598c:	4a16      	ldr	r2, [pc, #88]	; (300059e8 <BOOT_PSRAM_Init+0xa4>)
3000598e:	f04f 0c20 	mov.w	ip, #32
30005992:	2101      	movs	r1, #1
30005994:	4b15      	ldr	r3, [pc, #84]	; (300059ec <BOOT_PSRAM_Init+0xa8>)
30005996:	6952      	ldr	r2, [r2, #20]
30005998:	4608      	mov	r0, r1
3000599a:	fbbc f2f2 	udiv	r2, ip, r2
3000599e:	4798      	blx	r3
300059a0:	b009      	add	sp, #36	; 0x24
300059a2:	f85d fb04 	ldr.w	pc, [sp], #4
300059a6:	4b12      	ldr	r3, [pc, #72]	; (300059f0 <BOOT_PSRAM_Init+0xac>)
300059a8:	4798      	blx	r3
300059aa:	4a0e      	ldr	r2, [pc, #56]	; (300059e4 <BOOT_PSRAM_Init+0xa0>)
300059ac:	4b0c      	ldr	r3, [pc, #48]	; (300059e0 <BOOT_PSRAM_Init+0x9c>)
300059ae:	6852      	ldr	r2, [r2, #4]
300059b0:	601a      	str	r2, [r3, #0]
300059b2:	e7eb      	b.n	3000598c <BOOT_PSRAM_Init+0x48>
300059b4:	480f      	ldr	r0, [pc, #60]	; (300059f4 <BOOT_PSRAM_Init+0xb0>)
300059b6:	f006 fbff 	bl	3000c1b8 <__DiagPrintf_veneer>
300059ba:	e7dc      	b.n	30005976 <BOOT_PSRAM_Init+0x32>
300059bc:	4b0e      	ldr	r3, [pc, #56]	; (300059f8 <BOOT_PSRAM_Init+0xb4>)
300059be:	4798      	blx	r3
300059c0:	e7db      	b.n	3000597a <BOOT_PSRAM_Init+0x36>
300059c2:	bf00      	nop
300059c4:	42008000 	.word	0x42008000
300059c8:	3000965d 	.word	0x3000965d
300059cc:	3000967d 	.word	0x3000967d
300059d0:	300096e5 	.word	0x300096e5
300059d4:	30007f9d 	.word	0x30007f9d
300059d8:	2001c00c 	.word	0x2001c00c
300059dc:	30009989 	.word	0x30009989
300059e0:	3000eea0 	.word	0x3000eea0
300059e4:	41002000 	.word	0x41002000
300059e8:	3000eab0 	.word	0x3000eab0
300059ec:	30009c6d 	.word	0x30009c6d
300059f0:	300099c5 	.word	0x300099c5
300059f4:	3000c8e4 	.word	0x3000c8e4
300059f8:	300098b1 	.word	0x300098b1

300059fc <BOOT_WakeFromPG>:
300059fc:	b538      	push	{r3, r4, r5, lr}
300059fe:	4c4b      	ldr	r4, [pc, #300]	; (30005b2c <BOOT_WakeFromPG+0x130>)
30005a00:	f7ff f980 	bl	30004d04 <BOOT_RAM_TZCfg>
30005a04:	69a3      	ldr	r3, [r4, #24]
30005a06:	b10b      	cbz	r3, 30005a0c <BOOT_WakeFromPG+0x10>
30005a08:	4a49      	ldr	r2, [pc, #292]	; (30005b30 <BOOT_WakeFromPG+0x134>)
30005a0a:	6093      	str	r3, [r2, #8]
30005a0c:	4b49      	ldr	r3, [pc, #292]	; (30005b34 <BOOT_WakeFromPG+0x138>)
30005a0e:	f06f 0c1a 	mvn.w	ip, #26
30005a12:	4849      	ldr	r0, [pc, #292]	; (30005b38 <BOOT_WakeFromPG+0x13c>)
30005a14:	f103 0e50 	add.w	lr, r3, #80	; 0x50
30005a18:	ebac 0c04 	sub.w	ip, ip, r4
30005a1c:	eb0c 0203 	add.w	r2, ip, r3
30005a20:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005a24:	4402      	add	r2, r0
30005a26:	4573      	cmp	r3, lr
30005a28:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005a2c:	d1f6      	bne.n	30005a1c <BOOT_WakeFromPG+0x20>
30005a2e:	6822      	ldr	r2, [r4, #0]
30005a30:	4b3f      	ldr	r3, [pc, #252]	; (30005b30 <BOOT_WakeFromPG+0x134>)
30005a32:	f422 7280 	bic.w	r2, r2, #256	; 0x100
30005a36:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
30005a3a:	6002      	str	r2, [r0, #0]
30005a3c:	6045      	str	r5, [r0, #4]
30005a3e:	6081      	str	r1, [r0, #8]
30005a40:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005a42:	4a3e      	ldr	r2, [pc, #248]	; (30005b3c <BOOT_WakeFromPG+0x140>)
30005a44:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005a48:	4c3d      	ldr	r4, [pc, #244]	; (30005b40 <BOOT_WakeFromPG+0x144>)
30005a4a:	6259      	str	r1, [r3, #36]	; 0x24
30005a4c:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005a4e:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005a52:	6251      	str	r1, [r2, #36]	; 0x24
30005a54:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005a58:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a5c:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005a60:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005a64:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a68:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005a6c:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005a70:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005a74:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005a78:	f7ff fd48 	bl	3000550c <BOOT_CACHEWRR_Set>
30005a7c:	4b31      	ldr	r3, [pc, #196]	; (30005b44 <BOOT_WakeFromPG+0x148>)
30005a7e:	6818      	ldr	r0, [r3, #0]
30005a80:	f7ff fd72 	bl	30005568 <BOOT_TCMSet>
30005a84:	2201      	movs	r2, #1
30005a86:	4930      	ldr	r1, [pc, #192]	; (30005b48 <BOOT_WakeFromPG+0x14c>)
30005a88:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30005a8c:	47a0      	blx	r4
30005a8e:	4b2f      	ldr	r3, [pc, #188]	; (30005b4c <BOOT_WakeFromPG+0x150>)
30005a90:	4798      	blx	r3
30005a92:	4b2f      	ldr	r3, [pc, #188]	; (30005b50 <BOOT_WakeFromPG+0x154>)
30005a94:	4798      	blx	r3
30005a96:	2801      	cmp	r0, #1
30005a98:	d130      	bne.n	30005afc <BOOT_WakeFromPG+0x100>
30005a9a:	4b2e      	ldr	r3, [pc, #184]	; (30005b54 <BOOT_WakeFromPG+0x158>)
30005a9c:	689b      	ldr	r3, [r3, #8]
30005a9e:	079b      	lsls	r3, r3, #30
30005aa0:	d43f      	bmi.n	30005b22 <BOOT_WakeFromPG+0x126>
30005aa2:	2201      	movs	r2, #1
30005aa4:	492c      	ldr	r1, [pc, #176]	; (30005b58 <BOOT_WakeFromPG+0x15c>)
30005aa6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005aaa:	47a0      	blx	r4
30005aac:	2201      	movs	r2, #1
30005aae:	492b      	ldr	r1, [pc, #172]	; (30005b5c <BOOT_WakeFromPG+0x160>)
30005ab0:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005ab4:	47a0      	blx	r4
30005ab6:	f7ff ff07 	bl	300058c8 <BOOT_DDR_Init>
30005aba:	4a29      	ldr	r2, [pc, #164]	; (30005b60 <BOOT_WakeFromPG+0x164>)
30005abc:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30005ac0:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005ac4:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005ac8:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30005acc:	4925      	ldr	r1, [pc, #148]	; (30005b64 <BOOT_WakeFromPG+0x168>)
30005ace:	4a26      	ldr	r2, [pc, #152]	; (30005b68 <BOOT_WakeFromPG+0x16c>)
30005ad0:	f891 3067 	ldrb.w	r3, [r1, #103]	; 0x67
30005ad4:	4c19      	ldr	r4, [pc, #100]	; (30005b3c <BOOT_WakeFromPG+0x140>)
30005ad6:	f043 0301 	orr.w	r3, r3, #1
30005ada:	4824      	ldr	r0, [pc, #144]	; (30005b6c <BOOT_WakeFromPG+0x170>)
30005adc:	f881 3067 	strb.w	r3, [r1, #103]	; 0x67
30005ae0:	6893      	ldr	r3, [r2, #8]
30005ae2:	6852      	ldr	r2, [r2, #4]
30005ae4:	605a      	str	r2, [r3, #4]
30005ae6:	60a3      	str	r3, [r4, #8]
30005ae8:	f380 8888 	msr	MSP_NS, r0
30005aec:	4a20      	ldr	r2, [pc, #128]	; (30005b70 <BOOT_WakeFromPG+0x174>)
30005aee:	f382 8809 	msr	PSP, r2
30005af2:	6858      	ldr	r0, [r3, #4]
30005af4:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
30005af8:	f7ff bc4c 	b.w	30005394 <BOOT_NsStart>
30005afc:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005b00:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005b04:	2201      	movs	r2, #1
30005b06:	47a0      	blx	r4
30005b08:	4a15      	ldr	r2, [pc, #84]	; (30005b60 <BOOT_WakeFromPG+0x164>)
30005b0a:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005b0e:	f043 0302 	orr.w	r3, r3, #2
30005b12:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005b16:	f7ff ff15 	bl	30005944 <BOOT_PSRAM_Init>
30005b1a:	2096      	movs	r0, #150	; 0x96
30005b1c:	4b15      	ldr	r3, [pc, #84]	; (30005b74 <BOOT_WakeFromPG+0x178>)
30005b1e:	4798      	blx	r3
30005b20:	e7d4      	b.n	30005acc <BOOT_WakeFromPG+0xd0>
30005b22:	4815      	ldr	r0, [pc, #84]	; (30005b78 <BOOT_WakeFromPG+0x17c>)
30005b24:	f006 fb48 	bl	3000c1b8 <__DiagPrintf_veneer>
30005b28:	e7bb      	b.n	30005aa2 <BOOT_WakeFromPG+0xa6>
30005b2a:	bf00      	nop
30005b2c:	3000ee34 	.word	0x3000ee34
30005b30:	e000ed00 	.word	0xe000ed00
30005b34:	3000ee4f 	.word	0x3000ee4f
30005b38:	e000e100 	.word	0xe000e100
30005b3c:	e002ed00 	.word	0xe002ed00
30005b40:	0000b479 	.word	0x0000b479
30005b44:	3000d7a0 	.word	0x3000d7a0
30005b48:	40001000 	.word	0x40001000
30005b4c:	0000d835 	.word	0x0000d835
30005b50:	30007ec5 	.word	0x30007ec5
30005b54:	2001c00c 	.word	0x2001c00c
30005b58:	40080000 	.word	0x40080000
30005b5c:	40040000 	.word	0x40040000
30005b60:	42008000 	.word	0x42008000
30005b64:	42008200 	.word	0x42008200
30005b68:	60000020 	.word	0x60000020
30005b6c:	2001bffc 	.word	0x2001bffc
30005b70:	2001b7fc 	.word	0x2001b7fc
30005b74:	00009b2d 	.word	0x00009b2d
30005b78:	3000c908 	.word	0x3000c908

30005b7c <BOOT_SCBConfig_HP>:
30005b7c:	4b0e      	ldr	r3, [pc, #56]	; (30005bb8 <BOOT_SCBConfig_HP+0x3c>)
30005b7e:	4a0f      	ldr	r2, [pc, #60]	; (30005bbc <BOOT_SCBConfig_HP+0x40>)
30005b80:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005b82:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005b86:	6259      	str	r1, [r3, #36]	; 0x24
30005b88:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005b8a:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005b8e:	6251      	str	r1, [r2, #36]	; 0x24
30005b90:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005b94:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005b98:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005b9c:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005ba0:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005ba4:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005ba8:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005bac:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005bb0:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005bb4:	4770      	bx	lr
30005bb6:	bf00      	nop
30005bb8:	e000ed00 	.word	0xe000ed00
30005bbc:	e002ed00 	.word	0xe002ed00

30005bc0 <BOOT_SCBVTORBackup_HP>:
30005bc0:	4a02      	ldr	r2, [pc, #8]	; (30005bcc <BOOT_SCBVTORBackup_HP+0xc>)
30005bc2:	4b03      	ldr	r3, [pc, #12]	; (30005bd0 <BOOT_SCBVTORBackup_HP+0x10>)
30005bc4:	6892      	ldr	r2, [r2, #8]
30005bc6:	619a      	str	r2, [r3, #24]
30005bc8:	4770      	bx	lr
30005bca:	bf00      	nop
30005bcc:	e000ed00 	.word	0xe000ed00
30005bd0:	3000ee34 	.word	0x3000ee34

30005bd4 <BOOT_SCBVTORReFill_HP>:
30005bd4:	4a02      	ldr	r2, [pc, #8]	; (30005be0 <BOOT_SCBVTORReFill_HP+0xc>)
30005bd6:	4b03      	ldr	r3, [pc, #12]	; (30005be4 <BOOT_SCBVTORReFill_HP+0x10>)
30005bd8:	6992      	ldr	r2, [r2, #24]
30005bda:	609a      	str	r2, [r3, #8]
30005bdc:	4770      	bx	lr
30005bde:	bf00      	nop
30005be0:	3000ee34 	.word	0x3000ee34
30005be4:	e000ed00 	.word	0xe000ed00

30005be8 <BOOT_NVICBackup_HP>:
30005be8:	491a      	ldr	r1, [pc, #104]	; (30005c54 <BOOT_NVICBackup_HP+0x6c>)
30005bea:	f06f 001a 	mvn.w	r0, #26
30005bee:	b410      	push	{r4}
30005bf0:	4c19      	ldr	r4, [pc, #100]	; (30005c58 <BOOT_NVICBackup_HP+0x70>)
30005bf2:	680a      	ldr	r2, [r1, #0]
30005bf4:	f104 031b 	add.w	r3, r4, #27
30005bf8:	1b00      	subs	r0, r0, r4
30005bfa:	6022      	str	r2, [r4, #0]
30005bfc:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
30005c00:	684a      	ldr	r2, [r1, #4]
30005c02:	6062      	str	r2, [r4, #4]
30005c04:	688a      	ldr	r2, [r1, #8]
30005c06:	60a2      	str	r2, [r4, #8]
30005c08:	18c2      	adds	r2, r0, r3
30005c0a:	440a      	add	r2, r1
30005c0c:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
30005c10:	f803 2f01 	strb.w	r2, [r3, #1]!
30005c14:	4563      	cmp	r3, ip
30005c16:	d1f7      	bne.n	30005c08 <BOOT_NVICBackup_HP+0x20>
30005c18:	4b10      	ldr	r3, [pc, #64]	; (30005c5c <BOOT_NVICBackup_HP+0x74>)
30005c1a:	695b      	ldr	r3, [r3, #20]
30005c1c:	03db      	lsls	r3, r3, #15
30005c1e:	d512      	bpl.n	30005c46 <BOOT_NVICBackup_HP+0x5e>
30005c20:	f014 011f 	ands.w	r1, r4, #31
30005c24:	4b0c      	ldr	r3, [pc, #48]	; (30005c58 <BOOT_NVICBackup_HP+0x70>)
30005c26:	d111      	bne.n	30005c4c <BOOT_NVICBackup_HP+0x64>
30005c28:	216c      	movs	r1, #108	; 0x6c
30005c2a:	f3bf 8f4f 	dsb	sy
30005c2e:	480b      	ldr	r0, [pc, #44]	; (30005c5c <BOOT_NVICBackup_HP+0x74>)
30005c30:	4419      	add	r1, r3
30005c32:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005c36:	3320      	adds	r3, #32
30005c38:	1aca      	subs	r2, r1, r3
30005c3a:	2a00      	cmp	r2, #0
30005c3c:	dcf9      	bgt.n	30005c32 <BOOT_NVICBackup_HP+0x4a>
30005c3e:	f3bf 8f4f 	dsb	sy
30005c42:	f3bf 8f6f 	isb	sy
30005c46:	f85d 4b04 	ldr.w	r4, [sp], #4
30005c4a:	4770      	bx	lr
30005c4c:	f024 031f 	bic.w	r3, r4, #31
30005c50:	316c      	adds	r1, #108	; 0x6c
30005c52:	e7ea      	b.n	30005c2a <BOOT_NVICBackup_HP+0x42>
30005c54:	e000e100 	.word	0xe000e100
30005c58:	3000ee34 	.word	0x3000ee34
30005c5c:	e000ed00 	.word	0xe000ed00

30005c60 <BOOT_NVICReFill_HP>:
30005c60:	b510      	push	{r4, lr}
30005c62:	f06f 0c1a 	mvn.w	ip, #26
30005c66:	4c0d      	ldr	r4, [pc, #52]	; (30005c9c <BOOT_NVICReFill_HP+0x3c>)
30005c68:	480d      	ldr	r0, [pc, #52]	; (30005ca0 <BOOT_NVICReFill_HP+0x40>)
30005c6a:	f104 031b 	add.w	r3, r4, #27
30005c6e:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30005c72:	ebac 0c04 	sub.w	ip, ip, r4
30005c76:	eb0c 0203 	add.w	r2, ip, r3
30005c7a:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005c7e:	4402      	add	r2, r0
30005c80:	4573      	cmp	r3, lr
30005c82:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005c86:	d1f6      	bne.n	30005c76 <BOOT_NVICReFill_HP+0x16>
30005c88:	e9d4 3100 	ldrd	r3, r1, [r4]
30005c8c:	68a2      	ldr	r2, [r4, #8]
30005c8e:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005c92:	6003      	str	r3, [r0, #0]
30005c94:	6041      	str	r1, [r0, #4]
30005c96:	6082      	str	r2, [r0, #8]
30005c98:	bd10      	pop	{r4, pc}
30005c9a:	bf00      	nop
30005c9c:	3000ee34 	.word	0x3000ee34
30005ca0:	e000e100 	.word	0xe000e100

30005ca4 <BOOT_ImgCopy>:
30005ca4:	b510      	push	{r4, lr}
30005ca6:	b082      	sub	sp, #8
30005ca8:	4b09      	ldr	r3, [pc, #36]	; (30005cd0 <BOOT_ImgCopy+0x2c>)
30005caa:	4604      	mov	r4, r0
30005cac:	e9cd 2100 	strd	r2, r1, [sp]
30005cb0:	4798      	blx	r3
30005cb2:	e9dd 2100 	ldrd	r2, r1, [sp]
30005cb6:	b128      	cbz	r0, 30005cc4 <BOOT_ImgCopy+0x20>
30005cb8:	4620      	mov	r0, r4
30005cba:	4b06      	ldr	r3, [pc, #24]	; (30005cd4 <BOOT_ImgCopy+0x30>)
30005cbc:	b002      	add	sp, #8
30005cbe:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005cc2:	4718      	bx	r3
30005cc4:	4620      	mov	r0, r4
30005cc6:	b002      	add	sp, #8
30005cc8:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005ccc:	f003 b900 	b.w	30008ed0 <NandImgCopy>
30005cd0:	3000a501 	.word	0x3000a501
30005cd4:	00012a1d 	.word	0x00012a1d

30005cd8 <BOOT_LoadImages>:
30005cd8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005cdc:	4b56      	ldr	r3, [pc, #344]	; (30005e38 <BOOT_LoadImages+0x160>)
30005cde:	b0b7      	sub	sp, #220	; 0xdc
30005ce0:	2700      	movs	r7, #0
30005ce2:	2104      	movs	r1, #4
30005ce4:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005ce8:	aa11      	add	r2, sp, #68	; 0x44
30005cea:	4b54      	ldr	r3, [pc, #336]	; (30005e3c <BOOT_LoadImages+0x164>)
30005cec:	2601      	movs	r6, #1
30005cee:	9703      	str	r7, [sp, #12]
30005cf0:	9705      	str	r7, [sp, #20]
30005cf2:	4798      	blx	r3
30005cf4:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
30005cf8:	409e      	lsls	r6, r3
30005cfa:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
30005cfe:	f000 8236 	beq.w	3000616e <BOOT_LoadImages+0x496>
30005d02:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
30005d06:	f000 8373 	beq.w	300063f0 <BOOT_LoadImages+0x718>
30005d0a:	4c4d      	ldr	r4, [pc, #308]	; (30005e40 <BOOT_LoadImages+0x168>)
30005d0c:	f04f 0800 	mov.w	r8, #0
30005d10:	4d4c      	ldr	r5, [pc, #304]	; (30005e44 <BOOT_LoadImages+0x16c>)
30005d12:	eb06 0904 	add.w	r9, r6, r4
30005d16:	484c      	ldr	r0, [pc, #304]	; (30005e48 <BOOT_LoadImages+0x170>)
30005d18:	4435      	add	r5, r6
30005d1a:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
30005d1e:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
30005d22:	f7ff f9cd 	bl	300050c0 <mcc_init_crc32>
30005d26:	f8d9 3004 	ldr.w	r3, [r9, #4]
30005d2a:	f8d5 a008 	ldr.w	sl, [r5, #8]
30005d2e:	f899 200c 	ldrb.w	r2, [r9, #12]
30005d32:	9308      	str	r3, [sp, #32]
30005d34:	3b01      	subs	r3, #1
30005d36:	9206      	str	r2, [sp, #24]
30005d38:	3303      	adds	r3, #3
30005d3a:	686a      	ldr	r2, [r5, #4]
30005d3c:	f895 b00c 	ldrb.w	fp, [r5, #12]
30005d40:	9204      	str	r2, [sp, #16]
30005d42:	f200 81b8 	bhi.w	300060b6 <BOOT_LoadImages+0x3de>
30005d46:	f8d9 2008 	ldr.w	r2, [r9, #8]
30005d4a:	9b04      	ldr	r3, [sp, #16]
30005d4c:	3a01      	subs	r2, #1
30005d4e:	3b01      	subs	r3, #1
30005d50:	3203      	adds	r2, #3
30005d52:	d868      	bhi.n	30005e26 <BOOT_LoadImages+0x14e>
30005d54:	9a06      	ldr	r2, [sp, #24]
30005d56:	9307      	str	r3, [sp, #28]
30005d58:	2a01      	cmp	r2, #1
30005d5a:	f240 8303 	bls.w	30006364 <BOOT_LoadImages+0x68c>
30005d5e:	483b      	ldr	r0, [pc, #236]	; (30005e4c <BOOT_LoadImages+0x174>)
30005d60:	f006 fa2a 	bl	3000c1b8 <__DiagPrintf_veneer>
30005d64:	9b07      	ldr	r3, [sp, #28]
30005d66:	3303      	adds	r3, #3
30005d68:	d864      	bhi.n	30005e34 <BOOT_LoadImages+0x15c>
30005d6a:	f10a 3aff 	add.w	sl, sl, #4294967295
30005d6e:	f11a 0f03 	cmn.w	sl, #3
30005d72:	f200 81a6 	bhi.w	300060c2 <BOOT_LoadImages+0x3ea>
30005d76:	f1bb 0f01 	cmp.w	fp, #1
30005d7a:	f200 8345 	bhi.w	30006408 <BOOT_LoadImages+0x730>
30005d7e:	9904      	ldr	r1, [sp, #16]
30005d80:	4833      	ldr	r0, [pc, #204]	; (30005e50 <BOOT_LoadImages+0x178>)
30005d82:	f006 fa19 	bl	3000c1b8 <__DiagPrintf_veneer>
30005d86:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005d8a:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005d8e:	4c2e      	ldr	r4, [pc, #184]	; (30005e48 <BOOT_LoadImages+0x170>)
30005d90:	9307      	str	r3, [sp, #28]
30005d92:	2300      	movs	r3, #0
30005d94:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005d98:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005d9c:	9309      	str	r3, [sp, #36]	; 0x24
30005d9e:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30005e68 <BOOT_LoadImages+0x190>
30005da2:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30005da6:	6829      	ldr	r1, [r5, #0]
30005da8:	f04f 30ff 	mov.w	r0, #4294967295
30005dac:	44b4      	add	ip, r6
30005dae:	f81c 2b01 	ldrb.w	r2, [ip], #1
30005db2:	4042      	eors	r2, r0
30005db4:	459c      	cmp	ip, r3
30005db6:	b2d2      	uxtb	r2, r2
30005db8:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005dbc:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30005dc0:	d1f5      	bne.n	30005dae <BOOT_LoadImages+0xd6>
30005dc2:	43c2      	mvns	r2, r0
30005dc4:	930f      	str	r3, [sp, #60]	; 0x3c
30005dc6:	4291      	cmp	r1, r2
30005dc8:	f000 82ff 	beq.w	300063ca <BOOT_LoadImages+0x6f2>
30005dcc:	4821      	ldr	r0, [pc, #132]	; (30005e54 <BOOT_LoadImages+0x17c>)
30005dce:	f006 f9f3 	bl	3000c1b8 <__DiagPrintf_veneer>
30005dd2:	2200      	movs	r2, #0
30005dd4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005dd6:	9909      	ldr	r1, [sp, #36]	; 0x24
30005dd8:	2900      	cmp	r1, #0
30005dda:	d04e      	beq.n	30005e7a <BOOT_LoadImages+0x1a2>
30005ddc:	f899 100c 	ldrb.w	r1, [r9, #12]
30005de0:	4553      	cmp	r3, sl
30005de2:	9106      	str	r1, [sp, #24]
30005de4:	d342      	bcc.n	30005e6c <BOOT_LoadImages+0x194>
30005de6:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30005dea:	45e2      	cmp	sl, ip
30005dec:	d93e      	bls.n	30005e6c <BOOT_LoadImages+0x194>
30005dee:	9807      	ldr	r0, [sp, #28]
30005df0:	4560      	cmp	r0, ip
30005df2:	f240 81fd 	bls.w	300061f0 <BOOT_LoadImages+0x518>
30005df6:	4283      	cmp	r3, r0
30005df8:	f0c0 81fa 	bcc.w	300061f0 <BOOT_LoadImages+0x518>
30005dfc:	2a00      	cmp	r2, #0
30005dfe:	f000 8207 	beq.w	30006210 <BOOT_LoadImages+0x538>
30005e02:	459b      	cmp	fp, r3
30005e04:	d801      	bhi.n	30005e0a <BOOT_LoadImages+0x132>
30005e06:	45e3      	cmp	fp, ip
30005e08:	d845      	bhi.n	30005e96 <BOOT_LoadImages+0x1be>
30005e0a:	4813      	ldr	r0, [pc, #76]	; (30005e58 <BOOT_LoadImages+0x180>)
30005e0c:	f006 f9d4 	bl	3000c1b8 <__DiagPrintf_veneer>
30005e10:	9f06      	ldr	r7, [sp, #24]
30005e12:	4812      	ldr	r0, [pc, #72]	; (30005e5c <BOOT_LoadImages+0x184>)
30005e14:	464d      	mov	r5, r9
30005e16:	f006 f9cf 	bl	3000c1b8 <__DiagPrintf_veneer>
30005e1a:	9b07      	ldr	r3, [sp, #28]
30005e1c:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30005e20:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30005e24:	e049      	b.n	30005eba <BOOT_LoadImages+0x1e2>
30005e26:	480e      	ldr	r0, [pc, #56]	; (30005e60 <BOOT_LoadImages+0x188>)
30005e28:	9307      	str	r3, [sp, #28]
30005e2a:	f006 f9c5 	bl	3000c1b8 <__DiagPrintf_veneer>
30005e2e:	9b07      	ldr	r3, [sp, #28]
30005e30:	3303      	adds	r3, #3
30005e32:	d99a      	bls.n	30005d6a <BOOT_LoadImages+0x92>
30005e34:	480b      	ldr	r0, [pc, #44]	; (30005e64 <BOOT_LoadImages+0x18c>)
30005e36:	e145      	b.n	300060c4 <BOOT_LoadImages+0x3ec>
30005e38:	2001c01c 	.word	0x2001c01c
30005e3c:	00009db5 	.word	0x00009db5
30005e40:	07ffe000 	.word	0x07ffe000
30005e44:	07fff000 	.word	0x07fff000
30005e48:	3000dd30 	.word	0x3000dd30
30005e4c:	3000c980 	.word	0x3000c980
30005e50:	3000cd68 	.word	0x3000cd68
30005e54:	3000ca84 	.word	0x3000ca84
30005e58:	3000cd4c 	.word	0x3000cd4c
30005e5c:	3000cb10 	.word	0x3000cb10
30005e60:	3000c96c 	.word	0x3000c96c
30005e64:	3000cd18 	.word	0x3000cd18
30005e68:	07fff004 	.word	0x07fff004
30005e6c:	48c2      	ldr	r0, [pc, #776]	; (30006178 <BOOT_LoadImages+0x4a0>)
30005e6e:	930f      	str	r3, [sp, #60]	; 0x3c
30005e70:	9209      	str	r2, [sp, #36]	; 0x24
30005e72:	f006 f9a1 	bl	3000c1b8 <__DiagPrintf_veneer>
30005e76:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005e78:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005e7a:	2a00      	cmp	r2, #0
30005e7c:	f000 812d 	beq.w	300060da <BOOT_LoadImages+0x402>
30005e80:	455b      	cmp	r3, fp
30005e82:	f0c0 82c3 	bcc.w	3000640c <BOOT_LoadImages+0x734>
30005e86:	f1bb 6f00 	cmp.w	fp, #134217728	; 0x8000000
30005e8a:	bf2c      	ite	cs
30005e8c:	2200      	movcs	r2, #0
30005e8e:	2201      	movcc	r2, #1
30005e90:	2a00      	cmp	r2, #0
30005e92:	f040 82bb 	bne.w	3000640c <BOOT_LoadImages+0x734>
30005e96:	4598      	cmp	r8, r3
30005e98:	f200 81b2 	bhi.w	30006200 <BOOT_LoadImages+0x528>
30005e9c:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30005ea0:	f0c0 81ae 	bcc.w	30006200 <BOOT_LoadImages+0x528>
30005ea4:	7b2f      	ldrb	r7, [r5, #12]
30005ea6:	2a00      	cmp	r2, #0
30005ea8:	f040 81d8 	bne.w	3000625c <BOOT_LoadImages+0x584>
30005eac:	48b3      	ldr	r0, [pc, #716]	; (3000617c <BOOT_LoadImages+0x4a4>)
30005eae:	f006 f983 	bl	3000c1b8 <__DiagPrintf_veneer>
30005eb2:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30005eb6:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30005eba:	4639      	mov	r1, r7
30005ebc:	48b0      	ldr	r0, [pc, #704]	; (30006180 <BOOT_LoadImages+0x4a8>)
30005ebe:	f006 f97b 	bl	3000c1b8 <__DiagPrintf_veneer>
30005ec2:	4bb0      	ldr	r3, [pc, #704]	; (30006184 <BOOT_LoadImages+0x4ac>)
30005ec4:	4798      	blx	r3
30005ec6:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005eca:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30005ece:	f8d1 300a 	ldr.w	r3, [r1, #10]
30005ed2:	6809      	ldr	r1, [r1, #0]
30005ed4:	330c      	adds	r3, #12
30005ed6:	910b      	str	r1, [sp, #44]	; 0x2c
30005ed8:	42b3      	cmp	r3, r6
30005eda:	bf28      	it	cs
30005edc:	4633      	movcs	r3, r6
30005ede:	930d      	str	r3, [sp, #52]	; 0x34
30005ee0:	f8d2 300a 	ldr.w	r3, [r2, #10]
30005ee4:	6812      	ldr	r2, [r2, #0]
30005ee6:	330c      	adds	r3, #12
30005ee8:	920a      	str	r2, [sp, #40]	; 0x28
30005eea:	429e      	cmp	r6, r3
30005eec:	bf28      	it	cs
30005eee:	461e      	movcs	r6, r3
30005ef0:	960c      	str	r6, [sp, #48]	; 0x30
30005ef2:	2800      	cmp	r0, #0
30005ef4:	f040 818e 	bne.w	30006214 <BOOT_LoadImages+0x53c>
30005ef8:	4ba3      	ldr	r3, [pc, #652]	; (30006188 <BOOT_LoadImages+0x4b0>)
30005efa:	4606      	mov	r6, r0
30005efc:	f8df b2b4 	ldr.w	fp, [pc, #692]	; 300061b4 <BOOT_LoadImages+0x4dc>
30005f00:	4680      	mov	r8, r0
30005f02:	9304      	str	r3, [sp, #16]
30005f04:	4681      	mov	r9, r0
30005f06:	4ba1      	ldr	r3, [pc, #644]	; (3000618c <BOOT_LoadImages+0x4b4>)
30005f08:	9003      	str	r0, [sp, #12]
30005f0a:	9306      	str	r3, [sp, #24]
30005f0c:	2f00      	cmp	r7, #0
30005f0e:	f040 81c1 	bne.w	30006294 <BOOT_LoadImages+0x5bc>
30005f12:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30005f16:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30005f18:	f10c 0104 	add.w	r1, ip, #4
30005f1c:	2b00      	cmp	r3, #0
30005f1e:	f000 83d8 	beq.w	300066d2 <BOOT_LoadImages+0x9fa>
30005f22:	1d18      	adds	r0, r3, #4
30005f24:	f04f 32ff 	mov.w	r2, #4294967295
30005f28:	4460      	add	r0, ip
30005f2a:	f811 3b01 	ldrb.w	r3, [r1], #1
30005f2e:	4053      	eors	r3, r2
30005f30:	4281      	cmp	r1, r0
30005f32:	b2db      	uxtb	r3, r3
30005f34:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005f38:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005f3c:	d1f5      	bne.n	30005f2a <BOOT_LoadImages+0x252>
30005f3e:	43d2      	mvns	r2, r2
30005f40:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30005f42:	4293      	cmp	r3, r2
30005f44:	f000 839e 	beq.w	30006684 <BOOT_LoadImages+0x9ac>
30005f48:	f8dc 100a 	ldr.w	r1, [ip, #10]
30005f4c:	f088 0301 	eor.w	r3, r8, #1
30005f50:	3101      	adds	r1, #1
30005f52:	f003 0301 	and.w	r3, r3, #1
30005f56:	ea46 0603 	orr.w	r6, r6, r3
30005f5a:	f000 8327 	beq.w	300065ac <BOOT_LoadImages+0x8d4>
30005f5e:	990b      	ldr	r1, [sp, #44]	; 0x2c
30005f60:	488b      	ldr	r0, [pc, #556]	; (30006190 <BOOT_LoadImages+0x4b8>)
30005f62:	f006 f929 	bl	3000c1b8 <__DiagPrintf_veneer>
30005f66:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005f6a:	2002      	movs	r0, #2
30005f6c:	f7ff f826 	bl	30004fbc <FLASH_Erase_With_Lock>
30005f70:	2e00      	cmp	r6, #0
30005f72:	f040 824f 	bne.w	30006414 <BOOT_LoadImages+0x73c>
30005f76:	9605      	str	r6, [sp, #20]
30005f78:	f089 0601 	eor.w	r6, r9, #1
30005f7c:	f04f 0801 	mov.w	r8, #1
30005f80:	f006 0601 	and.w	r6, r6, #1
30005f84:	9b03      	ldr	r3, [sp, #12]
30005f86:	2b01      	cmp	r3, #1
30005f88:	f000 8236 	beq.w	300063f8 <BOOT_LoadImages+0x720>
30005f8c:	9b03      	ldr	r3, [sp, #12]
30005f8e:	2b02      	cmp	r3, #2
30005f90:	f000 8406 	beq.w	300067a0 <BOOT_LoadImages+0xac8>
30005f94:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30005f96:	4648      	mov	r0, r9
30005f98:	f8cb 3000 	str.w	r3, [fp]
30005f9c:	9b04      	ldr	r3, [sp, #16]
30005f9e:	4798      	blx	r3
30005fa0:	1e03      	subs	r3, r0, #0
30005fa2:	bf18      	it	ne
30005fa4:	2301      	movne	r3, #1
30005fa6:	2800      	cmp	r0, #0
30005fa8:	f000 814f 	beq.w	3000624a <BOOT_LoadImages+0x572>
30005fac:	2e00      	cmp	r6, #0
30005fae:	f000 814c 	beq.w	3000624a <BOOT_LoadImages+0x572>
30005fb2:	fab7 f787 	clz	r7, r7
30005fb6:	9b05      	ldr	r3, [sp, #20]
30005fb8:	097f      	lsrs	r7, r7, #5
30005fba:	2b00      	cmp	r3, #0
30005fbc:	f000 83f7 	beq.w	300067ae <BOOT_LoadImages+0xad6>
30005fc0:	4699      	mov	r9, r3
30005fc2:	4c74      	ldr	r4, [pc, #464]	; (30006194 <BOOT_LoadImages+0x4bc>)
30005fc4:	2f00      	cmp	r7, #0
30005fc6:	f040 8131 	bne.w	3000622c <BOOT_LoadImages+0x554>
30005fca:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005fce:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005fd2:	3201      	adds	r2, #1
30005fd4:	f000 8237 	beq.w	30006446 <BOOT_LoadImages+0x76e>
30005fd8:	4a6f      	ldr	r2, [pc, #444]	; (30006198 <BOOT_LoadImages+0x4c0>)
30005fda:	6892      	ldr	r2, [r2, #8]
30005fdc:	0791      	lsls	r1, r2, #30
30005fde:	f100 82ed 	bmi.w	300065bc <BOOT_LoadImages+0x8e4>
30005fe2:	2301      	movs	r3, #1
30005fe4:	9303      	str	r3, [sp, #12]
30005fe6:	f04f 0801 	mov.w	r8, #1
30005fea:	4646      	mov	r6, r8
30005fec:	f8cd 8014 	str.w	r8, [sp, #20]
30005ff0:	f1b9 0f00 	cmp.w	r9, #0
30005ff4:	d0c6      	beq.n	30005f84 <BOOT_LoadImages+0x2ac>
30005ff6:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30005ffa:	2280      	movs	r2, #128	; 0x80
30005ffc:	21ff      	movs	r1, #255	; 0xff
30005ffe:	46c8      	mov	r8, r9
30006000:	4650      	mov	r0, sl
30006002:	f006 f8b9 	bl	3000c178 <____wrap_memset_veneer>
30006006:	2280      	movs	r2, #128	; 0x80
30006008:	4629      	mov	r1, r5
3000600a:	4650      	mov	r0, sl
3000600c:	f006 f8ac 	bl	3000c168 <____wrap_memcpy_veneer>
30006010:	2f01      	cmp	r7, #1
30006012:	f000 830c 	beq.w	3000662e <BOOT_LoadImages+0x956>
30006016:	2100      	movs	r1, #0
30006018:	f8cd 9014 	str.w	r9, [sp, #20]
3000601c:	f04f 0901 	mov.w	r9, #1
30006020:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30006024:	1d2e      	adds	r6, r5, #4
30006026:	485d      	ldr	r0, [pc, #372]	; (3000619c <BOOT_LoadImages+0x4c4>)
30006028:	f006 f8c6 	bl	3000c1b8 <__DiagPrintf_veneer>
3000602c:	4629      	mov	r1, r5
3000602e:	2002      	movs	r0, #2
30006030:	f7fe ffc4 	bl	30004fbc <FLASH_Erase_With_Lock>
30006034:	aa17      	add	r2, sp, #92	; 0x5c
30006036:	217c      	movs	r1, #124	; 0x7c
30006038:	4630      	mov	r0, r6
3000603a:	f7fe ffe7 	bl	3000500c <FLASH_TxData_With_Lock>
3000603e:	4630      	mov	r0, r6
30006040:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30006044:	f04f 32ff 	mov.w	r2, #4294967295
30006048:	f810 3b01 	ldrb.w	r3, [r0], #1
3000604c:	4053      	eors	r3, r2
3000604e:	4288      	cmp	r0, r1
30006050:	b2db      	uxtb	r3, r3
30006052:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006056:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000605a:	d1f5      	bne.n	30006048 <BOOT_LoadImages+0x370>
3000605c:	43d2      	mvns	r2, r2
3000605e:	4629      	mov	r1, r5
30006060:	2002      	movs	r0, #2
30006062:	9216      	str	r2, [sp, #88]	; 0x58
30006064:	f7fe ffaa 	bl	30004fbc <FLASH_Erase_With_Lock>
30006068:	4652      	mov	r2, sl
3000606a:	2180      	movs	r1, #128	; 0x80
3000606c:	4628      	mov	r0, r5
3000606e:	f7fe ffcd 	bl	3000500c <FLASH_TxData_With_Lock>
30006072:	9b05      	ldr	r3, [sp, #20]
30006074:	2b00      	cmp	r3, #0
30006076:	f000 813d 	beq.w	300062f4 <BOOT_LoadImages+0x61c>
3000607a:	f1b8 0f00 	cmp.w	r8, #0
3000607e:	f040 83b1 	bne.w	300067e4 <BOOT_LoadImages+0xb0c>
30006082:	2f01      	cmp	r7, #1
30006084:	f000 827c 	beq.w	30006580 <BOOT_LoadImages+0x8a8>
30006088:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000608c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006090:	3201      	adds	r2, #1
30006092:	f000 8287 	beq.w	300065a4 <BOOT_LoadImages+0x8cc>
30006096:	4a40      	ldr	r2, [pc, #256]	; (30006198 <BOOT_LoadImages+0x4c0>)
30006098:	6892      	ldr	r2, [r2, #8]
3000609a:	0792      	lsls	r2, r2, #30
3000609c:	f100 82ff 	bmi.w	3000669e <BOOT_LoadImages+0x9c6>
300060a0:	f089 0601 	eor.w	r6, r9, #1
300060a4:	f04f 0801 	mov.w	r8, #1
300060a8:	f006 0601 	and.w	r6, r6, #1
300060ac:	3310      	adds	r3, #16
300060ae:	930e      	str	r3, [sp, #56]	; 0x38
300060b0:	2302      	movs	r3, #2
300060b2:	9303      	str	r3, [sp, #12]
300060b4:	e76e      	b.n	30005f94 <BOOT_LoadImages+0x2bc>
300060b6:	483a      	ldr	r0, [pc, #232]	; (300061a0 <BOOT_LoadImages+0x4c8>)
300060b8:	f006 f87e 	bl	3000c1b8 <__DiagPrintf_veneer>
300060bc:	9b04      	ldr	r3, [sp, #16]
300060be:	3b01      	subs	r3, #1
300060c0:	e651      	b.n	30005d66 <BOOT_LoadImages+0x8e>
300060c2:	4838      	ldr	r0, [pc, #224]	; (300061a4 <BOOT_LoadImages+0x4cc>)
300060c4:	f006 f878 	bl	3000c1b8 <__DiagPrintf_veneer>
300060c8:	f8d9 3011 	ldr.w	r3, [r9, #17]
300060cc:	f8d9 a00d 	ldr.w	sl, [r9, #13]
300060d0:	9307      	str	r3, [sp, #28]
300060d2:	f8d5 b00d 	ldr.w	fp, [r5, #13]
300060d6:	f8d5 8011 	ldr.w	r8, [r5, #17]
300060da:	4833      	ldr	r0, [pc, #204]	; (300061a8 <BOOT_LoadImages+0x4d0>)
300060dc:	f006 f86c 	bl	3000c1b8 <__DiagPrintf_veneer>
300060e0:	4832      	ldr	r0, [pc, #200]	; (300061ac <BOOT_LoadImages+0x4d4>)
300060e2:	f006 f869 	bl	3000c1b8 <__DiagPrintf_veneer>
300060e6:	4631      	mov	r1, r6
300060e8:	aa12      	add	r2, sp, #72	; 0x48
300060ea:	4650      	mov	r0, sl
300060ec:	f7ff f82e 	bl	3000514c <recovery_check>
300060f0:	4631      	mov	r1, r6
300060f2:	4604      	mov	r4, r0
300060f4:	aa13      	add	r2, sp, #76	; 0x4c
300060f6:	9807      	ldr	r0, [sp, #28]
300060f8:	f7ff f828 	bl	3000514c <recovery_check>
300060fc:	4631      	mov	r1, r6
300060fe:	aa14      	add	r2, sp, #80	; 0x50
30006100:	9004      	str	r0, [sp, #16]
30006102:	4658      	mov	r0, fp
30006104:	f7ff f822 	bl	3000514c <recovery_check>
30006108:	4631      	mov	r1, r6
3000610a:	aa15      	add	r2, sp, #84	; 0x54
3000610c:	4606      	mov	r6, r0
3000610e:	4640      	mov	r0, r8
30006110:	f7ff f81c 	bl	3000514c <recovery_check>
30006114:	9b04      	ldr	r3, [sp, #16]
30006116:	4306      	orrs	r6, r0
30006118:	431c      	orrs	r4, r3
3000611a:	d04d      	beq.n	300061b8 <BOOT_LoadImages+0x4e0>
3000611c:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30006120:	2e00      	cmp	r6, #0
30006122:	f000 81eb 	beq.w	300064fc <BOOT_LoadImages+0x824>
30006126:	4293      	cmp	r3, r2
30006128:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
3000612c:	f0c0 8258 	bcc.w	300065e0 <BOOT_LoadImages+0x908>
30006130:	428b      	cmp	r3, r1
30006132:	d302      	bcc.n	3000613a <BOOT_LoadImages+0x462>
30006134:	4283      	cmp	r3, r0
30006136:	f080 8324 	bcs.w	30006782 <BOOT_LoadImages+0xaaa>
3000613a:	4293      	cmp	r3, r2
3000613c:	f000 8250 	beq.w	300065e0 <BOOT_LoadImages+0x908>
30006140:	4293      	cmp	r3, r2
30006142:	bf38      	it	cc
30006144:	4613      	movcc	r3, r2
30006146:	428b      	cmp	r3, r1
30006148:	f240 830c 	bls.w	30006764 <BOOT_LoadImages+0xa8c>
3000614c:	428b      	cmp	r3, r1
3000614e:	bf38      	it	cc
30006150:	460b      	movcc	r3, r1
30006152:	4283      	cmp	r3, r0
30006154:	f240 831d 	bls.w	30006792 <BOOT_LoadImages+0xaba>
30006158:	4b0b      	ldr	r3, [pc, #44]	; (30006188 <BOOT_LoadImages+0x4b0>)
3000615a:	f04f 0800 	mov.w	r8, #0
3000615e:	2601      	movs	r6, #1
30006160:	f8df b050 	ldr.w	fp, [pc, #80]	; 300061b4 <BOOT_LoadImages+0x4dc>
30006164:	9304      	str	r3, [sp, #16]
30006166:	46c1      	mov	r9, r8
30006168:	4b08      	ldr	r3, [pc, #32]	; (3000618c <BOOT_LoadImages+0x4b4>)
3000616a:	9306      	str	r3, [sp, #24]
3000616c:	e712      	b.n	30005f94 <BOOT_LoadImages+0x2bc>
3000616e:	4810      	ldr	r0, [pc, #64]	; (300061b0 <BOOT_LoadImages+0x4d8>)
30006170:	f006 f822 	bl	3000c1b8 <__DiagPrintf_veneer>
30006174:	e5c9      	b.n	30005d0a <BOOT_LoadImages+0x32>
30006176:	bf00      	nop
30006178:	3000cac4 	.word	0x3000cac4
3000617c:	3000cb20 	.word	0x3000cb20
30006180:	3000cb54 	.word	0x3000cb54
30006184:	0000c149 	.word	0x0000c149
30006188:	300043b9 	.word	0x300043b9
3000618c:	3000d79e 	.word	0x3000d79e
30006190:	3000cbc4 	.word	0x3000cbc4
30006194:	3000dd30 	.word	0x3000dd30
30006198:	2001c00c 	.word	0x2001c00c
3000619c:	3000cc98 	.word	0x3000cc98
300061a0:	3000c954 	.word	0x3000c954
300061a4:	3000c9b8 	.word	0x3000c9b8
300061a8:	3000cb30 	.word	0x3000cb30
300061ac:	3000cb44 	.word	0x3000cb44
300061b0:	3000c930 	.word	0x3000c930
300061b4:	3000ee30 	.word	0x3000ee30
300061b8:	2e00      	cmp	r6, #0
300061ba:	f000 8286 	beq.w	300066ca <BOOT_LoadImages+0x9f2>
300061be:	4659      	mov	r1, fp
300061c0:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
300061c4:	e9cd 5400 	strd	r5, r4, [sp]
300061c8:	429a      	cmp	r2, r3
300061ca:	464b      	mov	r3, r9
300061cc:	4642      	mov	r2, r8
300061ce:	f080 8205 	bcs.w	300065dc <BOOT_LoadImages+0x904>
300061d2:	2001      	movs	r0, #1
300061d4:	f7fe ffea 	bl	300051ac <rewrite_bp>
300061d8:	46a0      	mov	r8, r4
300061da:	b2c3      	uxtb	r3, r0
300061dc:	2601      	movs	r6, #1
300061de:	46a1      	mov	r9, r4
300061e0:	f8df b314 	ldr.w	fp, [pc, #788]	; 300064f8 <BOOT_LoadImages+0x820>
300061e4:	9303      	str	r3, [sp, #12]
300061e6:	4bae      	ldr	r3, [pc, #696]	; (300064a0 <BOOT_LoadImages+0x7c8>)
300061e8:	9304      	str	r3, [sp, #16]
300061ea:	4bae      	ldr	r3, [pc, #696]	; (300064a4 <BOOT_LoadImages+0x7cc>)
300061ec:	9306      	str	r3, [sp, #24]
300061ee:	e6c9      	b.n	30005f84 <BOOT_LoadImages+0x2ac>
300061f0:	48ad      	ldr	r0, [pc, #692]	; (300064a8 <BOOT_LoadImages+0x7d0>)
300061f2:	930f      	str	r3, [sp, #60]	; 0x3c
300061f4:	9209      	str	r2, [sp, #36]	; 0x24
300061f6:	f005 ffdf 	bl	3000c1b8 <__DiagPrintf_veneer>
300061fa:	9a09      	ldr	r2, [sp, #36]	; 0x24
300061fc:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300061fe:	e63c      	b.n	30005e7a <BOOT_LoadImages+0x1a2>
30006200:	48aa      	ldr	r0, [pc, #680]	; (300064ac <BOOT_LoadImages+0x7d4>)
30006202:	9204      	str	r2, [sp, #16]
30006204:	f005 ffd8 	bl	3000c1b8 <__DiagPrintf_veneer>
30006208:	9a04      	ldr	r2, [sp, #16]
3000620a:	2a00      	cmp	r2, #0
3000620c:	f43f af65 	beq.w	300060da <BOOT_LoadImages+0x402>
30006210:	9f06      	ldr	r7, [sp, #24]
30006212:	e5fe      	b.n	30005e12 <BOOT_LoadImages+0x13a>
30006214:	4aa2      	ldr	r2, [pc, #648]	; (300064a0 <BOOT_LoadImages+0x7c8>)
30006216:	2300      	movs	r3, #0
30006218:	f8df b2dc 	ldr.w	fp, [pc, #732]	; 300064f8 <BOOT_LoadImages+0x820>
3000621c:	9204      	str	r2, [sp, #16]
3000621e:	4699      	mov	r9, r3
30006220:	4aa0      	ldr	r2, [pc, #640]	; (300064a4 <BOOT_LoadImages+0x7cc>)
30006222:	9303      	str	r3, [sp, #12]
30006224:	9206      	str	r2, [sp, #24]
30006226:	2f00      	cmp	r7, #0
30006228:	f43f aecf 	beq.w	30005fca <BOOT_LoadImages+0x2f2>
3000622c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006230:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006234:	3201      	adds	r2, #1
30006236:	f000 8201 	beq.w	3000663c <BOOT_LoadImages+0x964>
3000623a:	4a9d      	ldr	r2, [pc, #628]	; (300064b0 <BOOT_LoadImages+0x7d8>)
3000623c:	6892      	ldr	r2, [r2, #8]
3000623e:	0792      	lsls	r2, r2, #30
30006240:	f100 81c4 	bmi.w	300065cc <BOOT_LoadImages+0x8f4>
30006244:	2302      	movs	r3, #2
30006246:	9303      	str	r3, [sp, #12]
30006248:	e6cd      	b.n	30005fe6 <BOOT_LoadImages+0x30e>
3000624a:	f009 0401 	and.w	r4, r9, #1
3000624e:	b1c3      	cbz	r3, 30006282 <BOOT_LoadImages+0x5aa>
30006250:	b1bc      	cbz	r4, 30006282 <BOOT_LoadImages+0x5aa>
30006252:	4c98      	ldr	r4, [pc, #608]	; (300064b4 <BOOT_LoadImages+0x7dc>)
30006254:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30006258:	47a0      	blx	r4
3000625a:	e7fb      	b.n	30006254 <BOOT_LoadImages+0x57c>
3000625c:	4896      	ldr	r0, [pc, #600]	; (300064b8 <BOOT_LoadImages+0x7e0>)
3000625e:	f005 ffab 	bl	3000c1b8 <__DiagPrintf_veneer>
30006262:	9b08      	ldr	r3, [sp, #32]
30006264:	9a04      	ldr	r2, [sp, #16]
30006266:	4293      	cmp	r3, r2
30006268:	f4ff ae20 	bcc.w	30005eac <BOOT_LoadImages+0x1d4>
3000626c:	4893      	ldr	r0, [pc, #588]	; (300064bc <BOOT_LoadImages+0x7e4>)
3000626e:	464d      	mov	r5, r9
30006270:	f005 ffa2 	bl	3000c1b8 <__DiagPrintf_veneer>
30006274:	9b07      	ldr	r3, [sp, #28]
30006276:	9f06      	ldr	r7, [sp, #24]
30006278:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
3000627c:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30006280:	e61b      	b.n	30005eba <BOOT_LoadImages+0x1e2>
30006282:	9b06      	ldr	r3, [sp, #24]
30006284:	781b      	ldrb	r3, [r3, #0]
30006286:	2b00      	cmp	r3, #0
30006288:	f040 80f9 	bne.w	3000647e <BOOT_LoadImages+0x7a6>
3000628c:	2001      	movs	r0, #1
3000628e:	b037      	add	sp, #220	; 0xdc
30006290:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006294:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30006298:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000629a:	f10c 0104 	add.w	r1, ip, #4
3000629e:	2b00      	cmp	r3, #0
300062a0:	f000 8219 	beq.w	300066d6 <BOOT_LoadImages+0x9fe>
300062a4:	1d18      	adds	r0, r3, #4
300062a6:	f04f 32ff 	mov.w	r2, #4294967295
300062aa:	4460      	add	r0, ip
300062ac:	f811 3b01 	ldrb.w	r3, [r1], #1
300062b0:	4053      	eors	r3, r2
300062b2:	4281      	cmp	r1, r0
300062b4:	b2db      	uxtb	r3, r3
300062b6:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300062ba:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300062be:	d1f5      	bne.n	300062ac <BOOT_LoadImages+0x5d4>
300062c0:	43d2      	mvns	r2, r2
300062c2:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300062c4:	4293      	cmp	r3, r2
300062c6:	f000 81cf 	beq.w	30006668 <BOOT_LoadImages+0x990>
300062ca:	f8dc 100a 	ldr.w	r1, [ip, #10]
300062ce:	f088 0301 	eor.w	r3, r8, #1
300062d2:	3101      	adds	r1, #1
300062d4:	f003 0301 	and.w	r3, r3, #1
300062d8:	ea46 0603 	orr.w	r6, r6, r3
300062dc:	f000 816a 	beq.w	300065b4 <BOOT_LoadImages+0x8dc>
300062e0:	990a      	ldr	r1, [sp, #40]	; 0x28
300062e2:	4877      	ldr	r0, [pc, #476]	; (300064c0 <BOOT_LoadImages+0x7e8>)
300062e4:	f005 ff68 	bl	3000c1b8 <__DiagPrintf_veneer>
300062e8:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
300062ec:	2002      	movs	r0, #2
300062ee:	f7fe fe65 	bl	30004fbc <FLASH_Erase_With_Lock>
300062f2:	e63d      	b.n	30005f70 <BOOT_LoadImages+0x298>
300062f4:	f1b8 0f00 	cmp.w	r8, #0
300062f8:	f040 826e 	bne.w	300067d8 <BOOT_LoadImages+0xb00>
300062fc:	2f01      	cmp	r7, #1
300062fe:	f000 8117 	beq.w	30006530 <BOOT_LoadImages+0x858>
30006302:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
30006306:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30006308:	1d31      	adds	r1, r6, #4
3000630a:	2b00      	cmp	r3, #0
3000630c:	f000 81f2 	beq.w	300066f4 <BOOT_LoadImages+0xa1c>
30006310:	1d18      	adds	r0, r3, #4
30006312:	f04f 32ff 	mov.w	r2, #4294967295
30006316:	4430      	add	r0, r6
30006318:	f811 3b01 	ldrb.w	r3, [r1], #1
3000631c:	4053      	eors	r3, r2
3000631e:	4281      	cmp	r1, r0
30006320:	b2db      	uxtb	r3, r3
30006322:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006326:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000632a:	d1f5      	bne.n	30006318 <BOOT_LoadImages+0x640>
3000632c:	43d2      	mvns	r2, r2
3000632e:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30006330:	4293      	cmp	r3, r2
30006332:	f000 81bc 	beq.w	300066ae <BOOT_LoadImages+0x9d6>
30006336:	f8d6 300a 	ldr.w	r3, [r6, #10]
3000633a:	3301      	adds	r3, #1
3000633c:	f000 8132 	beq.w	300065a4 <BOOT_LoadImages+0x8cc>
30006340:	990a      	ldr	r1, [sp, #40]	; 0x28
30006342:	485f      	ldr	r0, [pc, #380]	; (300064c0 <BOOT_LoadImages+0x7e8>)
30006344:	f005 ff38 	bl	3000c1b8 <__DiagPrintf_veneer>
30006348:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
3000634c:	2002      	movs	r0, #2
3000634e:	f7fe fe35 	bl	30004fbc <FLASH_Erase_With_Lock>
30006352:	4b57      	ldr	r3, [pc, #348]	; (300064b0 <BOOT_LoadImages+0x7d8>)
30006354:	689b      	ldr	r3, [r3, #8]
30006356:	079b      	lsls	r3, r3, #30
30006358:	d400      	bmi.n	3000635c <BOOT_LoadImages+0x684>
3000635a:	e7fe      	b.n	3000635a <BOOT_LoadImages+0x682>
3000635c:	4859      	ldr	r0, [pc, #356]	; (300064c4 <BOOT_LoadImages+0x7ec>)
3000635e:	f005 ff2b 	bl	3000c1b8 <__DiagPrintf_veneer>
30006362:	e7fa      	b.n	3000635a <BOOT_LoadImages+0x682>
30006364:	9908      	ldr	r1, [sp, #32]
30006366:	4858      	ldr	r0, [pc, #352]	; (300064c8 <BOOT_LoadImages+0x7f0>)
30006368:	f005 ff26 	bl	3000c1b8 <__DiagPrintf_veneer>
3000636c:	9b07      	ldr	r3, [sp, #28]
3000636e:	3303      	adds	r3, #3
30006370:	f240 81b3 	bls.w	300066da <BOOT_LoadImages+0xa02>
30006374:	4855      	ldr	r0, [pc, #340]	; (300064cc <BOOT_LoadImages+0x7f4>)
30006376:	f005 ff1f 	bl	3000c1b8 <__DiagPrintf_veneer>
3000637a:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
3000637e:	f8d9 3011 	ldr.w	r3, [r9, #17]
30006382:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006386:	9307      	str	r3, [sp, #28]
30006388:	f8d5 b00d 	ldr.w	fp, [r5, #13]
3000638c:	f8d5 8011 	ldr.w	r8, [r5, #17]
30006390:	484f      	ldr	r0, [pc, #316]	; (300064d0 <BOOT_LoadImages+0x7f8>)
30006392:	f04f 32ff 	mov.w	r2, #4294967295
30006396:	5931      	ldr	r1, [r6, r4]
30006398:	4430      	add	r0, r6
3000639a:	4c4e      	ldr	r4, [pc, #312]	; (300064d4 <BOOT_LoadImages+0x7fc>)
3000639c:	f810 3b01 	ldrb.w	r3, [r0], #1
300063a0:	4053      	eors	r3, r2
300063a2:	4285      	cmp	r5, r0
300063a4:	b2db      	uxtb	r3, r3
300063a6:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300063aa:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300063ae:	d1f5      	bne.n	3000639c <BOOT_LoadImages+0x6c4>
300063b0:	43d2      	mvns	r2, r2
300063b2:	4291      	cmp	r1, r2
300063b4:	d010      	beq.n	300063d8 <BOOT_LoadImages+0x700>
300063b6:	4848      	ldr	r0, [pc, #288]	; (300064d8 <BOOT_LoadImages+0x800>)
300063b8:	f005 fefe 	bl	3000c1b8 <__DiagPrintf_veneer>
300063bc:	9b09      	ldr	r3, [sp, #36]	; 0x24
300063be:	2b00      	cmp	r3, #0
300063c0:	f43f ae8b 	beq.w	300060da <BOOT_LoadImages+0x402>
300063c4:	2300      	movs	r3, #0
300063c6:	9309      	str	r3, [sp, #36]	; 0x24
300063c8:	e4e9      	b.n	30005d9e <BOOT_LoadImages+0xc6>
300063ca:	460a      	mov	r2, r1
300063cc:	4843      	ldr	r0, [pc, #268]	; (300064dc <BOOT_LoadImages+0x804>)
300063ce:	f005 fef3 	bl	3000c1b8 <__DiagPrintf_veneer>
300063d2:	2201      	movs	r2, #1
300063d4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300063d6:	e4fe      	b.n	30005dd6 <BOOT_LoadImages+0xfe>
300063d8:	460a      	mov	r2, r1
300063da:	4841      	ldr	r0, [pc, #260]	; (300064e0 <BOOT_LoadImages+0x808>)
300063dc:	f005 feec 	bl	3000c1b8 <__DiagPrintf_veneer>
300063e0:	9b09      	ldr	r3, [sp, #36]	; 0x24
300063e2:	2b00      	cmp	r3, #0
300063e4:	f47f acdb 	bne.w	30005d9e <BOOT_LoadImages+0xc6>
300063e8:	461a      	mov	r2, r3
300063ea:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
300063ee:	e4f5      	b.n	30005ddc <BOOT_LoadImages+0x104>
300063f0:	483c      	ldr	r0, [pc, #240]	; (300064e4 <BOOT_LoadImages+0x80c>)
300063f2:	f005 fee1 	bl	3000c1b8 <__DiagPrintf_veneer>
300063f6:	e488      	b.n	30005d0a <BOOT_LoadImages+0x32>
300063f8:	4c36      	ldr	r4, [pc, #216]	; (300064d4 <BOOT_LoadImages+0x7fc>)
300063fa:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300063fe:	3310      	adds	r3, #16
30006400:	930e      	str	r3, [sp, #56]	; 0x38
30006402:	2301      	movs	r3, #1
30006404:	9303      	str	r3, [sp, #12]
30006406:	e5c5      	b.n	30005f94 <BOOT_LoadImages+0x2bc>
30006408:	4837      	ldr	r0, [pc, #220]	; (300064e8 <BOOT_LoadImages+0x810>)
3000640a:	e65b      	b.n	300060c4 <BOOT_LoadImages+0x3ec>
3000640c:	4837      	ldr	r0, [pc, #220]	; (300064ec <BOOT_LoadImages+0x814>)
3000640e:	f005 fed3 	bl	3000c1b8 <__DiagPrintf_veneer>
30006412:	e662      	b.n	300060da <BOOT_LoadImages+0x402>
30006414:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006418:	2280      	movs	r2, #128	; 0x80
3000641a:	21ff      	movs	r1, #255	; 0xff
3000641c:	4650      	mov	r0, sl
3000641e:	f005 feab 	bl	3000c178 <____wrap_memset_veneer>
30006422:	2280      	movs	r2, #128	; 0x80
30006424:	4629      	mov	r1, r5
30006426:	4650      	mov	r0, sl
30006428:	f005 fe9e 	bl	3000c168 <____wrap_memcpy_veneer>
3000642c:	f1b9 0f00 	cmp.w	r9, #0
30006430:	f040 81ba 	bne.w	300067a8 <BOOT_LoadImages+0xad0>
30006434:	b1e7      	cbz	r7, 30006470 <BOOT_LoadImages+0x798>
30006436:	f8cd 9014 	str.w	r9, [sp, #20]
3000643a:	f04f 0900 	mov.w	r9, #0
3000643e:	4649      	mov	r1, r9
30006440:	f88d 9064 	strb.w	r9, [sp, #100]	; 0x64
30006444:	e5ee      	b.n	30006024 <BOOT_LoadImages+0x34c>
30006446:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
3000644a:	4829      	ldr	r0, [pc, #164]	; (300064f0 <BOOT_LoadImages+0x818>)
3000644c:	f005 feb4 	bl	3000c1b8 <__DiagPrintf_veneer>
30006450:	2280      	movs	r2, #128	; 0x80
30006452:	21ff      	movs	r1, #255	; 0xff
30006454:	4650      	mov	r0, sl
30006456:	f005 fe8f 	bl	3000c178 <____wrap_memset_veneer>
3000645a:	2280      	movs	r2, #128	; 0x80
3000645c:	4629      	mov	r1, r5
3000645e:	4650      	mov	r0, sl
30006460:	46b8      	mov	r8, r7
30006462:	f005 fe81 	bl	3000c168 <____wrap_memcpy_veneer>
30006466:	f1b9 0f00 	cmp.w	r9, #0
3000646a:	f47f add4 	bne.w	30006016 <BOOT_LoadImages+0x33e>
3000646e:	2701      	movs	r7, #1
30006470:	2101      	movs	r1, #1
30006472:	9705      	str	r7, [sp, #20]
30006474:	2700      	movs	r7, #0
30006476:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
3000647a:	46b9      	mov	r9, r7
3000647c:	e5d2      	b.n	30006024 <BOOT_LoadImages+0x34c>
3000647e:	4b1d      	ldr	r3, [pc, #116]	; (300064f4 <BOOT_LoadImages+0x81c>)
30006480:	4648      	mov	r0, r9
30006482:	4798      	blx	r3
30006484:	1e03      	subs	r3, r0, #0
30006486:	bf18      	it	ne
30006488:	2301      	movne	r3, #1
3000648a:	b110      	cbz	r0, 30006492 <BOOT_LoadImages+0x7ba>
3000648c:	2e00      	cmp	r6, #0
3000648e:	f47f ad90 	bne.w	30005fb2 <BOOT_LoadImages+0x2da>
30006492:	2c00      	cmp	r4, #0
30006494:	f43f aefa 	beq.w	3000628c <BOOT_LoadImages+0x5b4>
30006498:	2b00      	cmp	r3, #0
3000649a:	f47f aeda 	bne.w	30006252 <BOOT_LoadImages+0x57a>
3000649e:	e6f5      	b.n	3000628c <BOOT_LoadImages+0x5b4>
300064a0:	300043b9 	.word	0x300043b9
300064a4:	3000d79e 	.word	0x3000d79e
300064a8:	3000cae0 	.word	0x3000cae0
300064ac:	3000ccfc 	.word	0x3000ccfc
300064b0:	2001c00c 	.word	0x2001c00c
300064b4:	00009be5 	.word	0x00009be5
300064b8:	3000cafc 	.word	0x3000cafc
300064bc:	3000cb10 	.word	0x3000cb10
300064c0:	3000cc4c 	.word	0x3000cc4c
300064c4:	3000c8ac 	.word	0x3000c8ac
300064c8:	3000c99c 	.word	0x3000c99c
300064cc:	3000cd18 	.word	0x3000cd18
300064d0:	07ffe004 	.word	0x07ffe004
300064d4:	3000dd30 	.word	0x3000dd30
300064d8:	3000ca08 	.word	0x3000ca08
300064dc:	3000ca48 	.word	0x3000ca48
300064e0:	3000c9cc 	.word	0x3000c9cc
300064e4:	3000c940 	.word	0x3000c940
300064e8:	3000cd30 	.word	0x3000cd30
300064ec:	3000cd4c 	.word	0x3000cd4c
300064f0:	3000cb68 	.word	0x3000cb68
300064f4:	300043bd 	.word	0x300043bd
300064f8:	3000ee30 	.word	0x3000ee30
300064fc:	4293      	cmp	r3, r2
300064fe:	f04f 0401 	mov.w	r4, #1
30006502:	464b      	mov	r3, r9
30006504:	9a07      	ldr	r2, [sp, #28]
30006506:	bf28      	it	cs
30006508:	4630      	movcs	r0, r6
3000650a:	4651      	mov	r1, sl
3000650c:	bf38      	it	cc
3000650e:	4620      	movcc	r0, r4
30006510:	9500      	str	r5, [sp, #0]
30006512:	9401      	str	r4, [sp, #4]
30006514:	46b0      	mov	r8, r6
30006516:	f7fe fe49 	bl	300051ac <rewrite_bp>
3000651a:	4626      	mov	r6, r4
3000651c:	b2c3      	uxtb	r3, r0
3000651e:	f8df b310 	ldr.w	fp, [pc, #784]	; 30006830 <BOOT_LoadImages+0xb58>
30006522:	46c1      	mov	r9, r8
30006524:	9303      	str	r3, [sp, #12]
30006526:	4bb3      	ldr	r3, [pc, #716]	; (300067f4 <BOOT_LoadImages+0xb1c>)
30006528:	9304      	str	r3, [sp, #16]
3000652a:	4bb3      	ldr	r3, [pc, #716]	; (300067f8 <BOOT_LoadImages+0xb20>)
3000652c:	9306      	str	r3, [sp, #24]
3000652e:	e529      	b.n	30005f84 <BOOT_LoadImages+0x2ac>
30006530:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
30006534:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30006536:	1d31      	adds	r1, r6, #4
30006538:	2b00      	cmp	r3, #0
3000653a:	f000 811e 	beq.w	3000677a <BOOT_LoadImages+0xaa2>
3000653e:	1d18      	adds	r0, r3, #4
30006540:	f04f 32ff 	mov.w	r2, #4294967295
30006544:	4430      	add	r0, r6
30006546:	f811 3b01 	ldrb.w	r3, [r1], #1
3000654a:	4053      	eors	r3, r2
3000654c:	4281      	cmp	r1, r0
3000654e:	b2db      	uxtb	r3, r3
30006550:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006554:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006558:	d1f5      	bne.n	30006546 <BOOT_LoadImages+0x86e>
3000655a:	43d2      	mvns	r2, r2
3000655c:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000655e:	4293      	cmp	r3, r2
30006560:	f000 80e1 	beq.w	30006726 <BOOT_LoadImages+0xa4e>
30006564:	f8d6 300a 	ldr.w	r3, [r6, #10]
30006568:	3301      	adds	r3, #1
3000656a:	d054      	beq.n	30006616 <BOOT_LoadImages+0x93e>
3000656c:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000656e:	48a3      	ldr	r0, [pc, #652]	; (300067fc <BOOT_LoadImages+0xb24>)
30006570:	f005 fe22 	bl	3000c1b8 <__DiagPrintf_veneer>
30006574:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30006578:	2002      	movs	r0, #2
3000657a:	f7fe fd1f 	bl	30004fbc <FLASH_Erase_With_Lock>
3000657e:	e6e8      	b.n	30006352 <BOOT_LoadImages+0x67a>
30006580:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006584:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006588:	3201      	adds	r2, #1
3000658a:	d044      	beq.n	30006616 <BOOT_LoadImages+0x93e>
3000658c:	4a9c      	ldr	r2, [pc, #624]	; (30006800 <BOOT_LoadImages+0xb28>)
3000658e:	6892      	ldr	r2, [r2, #8]
30006590:	0791      	lsls	r1, r2, #30
30006592:	d444      	bmi.n	3000661e <BOOT_LoadImages+0x946>
30006594:	f089 0601 	eor.w	r6, r9, #1
30006598:	9705      	str	r7, [sp, #20]
3000659a:	2701      	movs	r7, #1
3000659c:	f006 0601 	and.w	r6, r6, #1
300065a0:	46b8      	mov	r8, r7
300065a2:	e72c      	b.n	300063fe <BOOT_LoadImages+0x726>
300065a4:	4897      	ldr	r0, [pc, #604]	; (30006804 <BOOT_LoadImages+0xb2c>)
300065a6:	f005 fe07 	bl	3000c1b8 <__DiagPrintf_veneer>
300065aa:	e6d2      	b.n	30006352 <BOOT_LoadImages+0x67a>
300065ac:	4896      	ldr	r0, [pc, #600]	; (30006808 <BOOT_LoadImages+0xb30>)
300065ae:	f005 fe03 	bl	3000c1b8 <__DiagPrintf_veneer>
300065b2:	e4dd      	b.n	30005f70 <BOOT_LoadImages+0x298>
300065b4:	4893      	ldr	r0, [pc, #588]	; (30006804 <BOOT_LoadImages+0xb2c>)
300065b6:	f005 fdff 	bl	3000c1b8 <__DiagPrintf_veneer>
300065ba:	e4d9      	b.n	30005f70 <BOOT_LoadImages+0x298>
300065bc:	f8d3 1006 	ldr.w	r1, [r3, #6]
300065c0:	4892      	ldr	r0, [pc, #584]	; (3000680c <BOOT_LoadImages+0xb34>)
300065c2:	f005 fdf9 	bl	3000c1b8 <__DiagPrintf_veneer>
300065c6:	2301      	movs	r3, #1
300065c8:	9303      	str	r3, [sp, #12]
300065ca:	e50c      	b.n	30005fe6 <BOOT_LoadImages+0x30e>
300065cc:	f8d3 1006 	ldr.w	r1, [r3, #6]
300065d0:	488f      	ldr	r0, [pc, #572]	; (30006810 <BOOT_LoadImages+0xb38>)
300065d2:	f005 fdf1 	bl	3000c1b8 <__DiagPrintf_veneer>
300065d6:	2302      	movs	r3, #2
300065d8:	9303      	str	r3, [sp, #12]
300065da:	e504      	b.n	30005fe6 <BOOT_LoadImages+0x30e>
300065dc:	4620      	mov	r0, r4
300065de:	e5f9      	b.n	300061d4 <BOOT_LoadImages+0x4fc>
300065e0:	428a      	cmp	r2, r1
300065e2:	f4ff adad 	bcc.w	30006140 <BOOT_LoadImages+0x468>
300065e6:	4282      	cmp	r2, r0
300065e8:	f4ff adaa 	bcc.w	30006140 <BOOT_LoadImages+0x468>
300065ec:	2601      	movs	r6, #1
300065ee:	464b      	mov	r3, r9
300065f0:	9a07      	ldr	r2, [sp, #28]
300065f2:	4651      	mov	r1, sl
300065f4:	4630      	mov	r0, r6
300065f6:	9500      	str	r5, [sp, #0]
300065f8:	9601      	str	r6, [sp, #4]
300065fa:	f7fe fdd7 	bl	300051ac <rewrite_bp>
300065fe:	f04f 0800 	mov.w	r8, #0
30006602:	b2c3      	uxtb	r3, r0
30006604:	f8df b228 	ldr.w	fp, [pc, #552]	; 30006830 <BOOT_LoadImages+0xb58>
30006608:	46c1      	mov	r9, r8
3000660a:	9303      	str	r3, [sp, #12]
3000660c:	4b79      	ldr	r3, [pc, #484]	; (300067f4 <BOOT_LoadImages+0xb1c>)
3000660e:	9304      	str	r3, [sp, #16]
30006610:	4b79      	ldr	r3, [pc, #484]	; (300067f8 <BOOT_LoadImages+0xb20>)
30006612:	9306      	str	r3, [sp, #24]
30006614:	e4b6      	b.n	30005f84 <BOOT_LoadImages+0x2ac>
30006616:	487c      	ldr	r0, [pc, #496]	; (30006808 <BOOT_LoadImages+0xb30>)
30006618:	f005 fdce 	bl	3000c1b8 <__DiagPrintf_veneer>
3000661c:	e699      	b.n	30006352 <BOOT_LoadImages+0x67a>
3000661e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006622:	487a      	ldr	r0, [pc, #488]	; (3000680c <BOOT_LoadImages+0xb34>)
30006624:	f005 fdc8 	bl	3000c1b8 <__DiagPrintf_veneer>
30006628:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000662c:	e7b2      	b.n	30006594 <BOOT_LoadImages+0x8bc>
3000662e:	f8cd 9014 	str.w	r9, [sp, #20]
30006632:	4639      	mov	r1, r7
30006634:	46b9      	mov	r9, r7
30006636:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
3000663a:	e4f3      	b.n	30006024 <BOOT_LoadImages+0x34c>
3000663c:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006640:	4870      	ldr	r0, [pc, #448]	; (30006804 <BOOT_LoadImages+0xb2c>)
30006642:	f005 fdb9 	bl	3000c1b8 <__DiagPrintf_veneer>
30006646:	2280      	movs	r2, #128	; 0x80
30006648:	21ff      	movs	r1, #255	; 0xff
3000664a:	4650      	mov	r0, sl
3000664c:	f005 fd94 	bl	3000c178 <____wrap_memset_veneer>
30006650:	2280      	movs	r2, #128	; 0x80
30006652:	4629      	mov	r1, r5
30006654:	4650      	mov	r0, sl
30006656:	f005 fd87 	bl	3000c168 <____wrap_memcpy_veneer>
3000665a:	f1b9 0f00 	cmp.w	r9, #0
3000665e:	d146      	bne.n	300066ee <BOOT_LoadImages+0xa16>
30006660:	46c8      	mov	r8, r9
30006662:	f04f 0901 	mov.w	r9, #1
30006666:	e6e6      	b.n	30006436 <BOOT_LoadImages+0x75e>
30006668:	461a      	mov	r2, r3
3000666a:	4619      	mov	r1, r3
3000666c:	4869      	ldr	r0, [pc, #420]	; (30006814 <BOOT_LoadImages+0xb3c>)
3000666e:	f005 fda3 	bl	3000c1b8 <__DiagPrintf_veneer>
30006672:	4b63      	ldr	r3, [pc, #396]	; (30006800 <BOOT_LoadImages+0xb28>)
30006674:	689b      	ldr	r3, [r3, #8]
30006676:	0798      	lsls	r0, r3, #30
30006678:	d449      	bmi.n	3000670e <BOOT_LoadImages+0xa36>
3000667a:	2302      	movs	r3, #2
3000667c:	f04f 0801 	mov.w	r8, #1
30006680:	9303      	str	r3, [sp, #12]
30006682:	e475      	b.n	30005f70 <BOOT_LoadImages+0x298>
30006684:	461a      	mov	r2, r3
30006686:	4619      	mov	r1, r3
30006688:	4863      	ldr	r0, [pc, #396]	; (30006818 <BOOT_LoadImages+0xb40>)
3000668a:	f005 fd95 	bl	3000c1b8 <__DiagPrintf_veneer>
3000668e:	4b5c      	ldr	r3, [pc, #368]	; (30006800 <BOOT_LoadImages+0xb28>)
30006690:	689b      	ldr	r3, [r3, #8]
30006692:	079b      	lsls	r3, r3, #30
30006694:	d430      	bmi.n	300066f8 <BOOT_LoadImages+0xa20>
30006696:	2301      	movs	r3, #1
30006698:	4698      	mov	r8, r3
3000669a:	9303      	str	r3, [sp, #12]
3000669c:	e468      	b.n	30005f70 <BOOT_LoadImages+0x298>
3000669e:	f8d3 1006 	ldr.w	r1, [r3, #6]
300066a2:	485b      	ldr	r0, [pc, #364]	; (30006810 <BOOT_LoadImages+0xb38>)
300066a4:	f005 fd88 	bl	3000c1b8 <__DiagPrintf_veneer>
300066a8:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300066ac:	e4f8      	b.n	300060a0 <BOOT_LoadImages+0x3c8>
300066ae:	461a      	mov	r2, r3
300066b0:	4619      	mov	r1, r3
300066b2:	4858      	ldr	r0, [pc, #352]	; (30006814 <BOOT_LoadImages+0xb3c>)
300066b4:	f005 fd80 	bl	3000c1b8 <__DiagPrintf_veneer>
300066b8:	4b51      	ldr	r3, [pc, #324]	; (30006800 <BOOT_LoadImages+0xb28>)
300066ba:	689a      	ldr	r2, [r3, #8]
300066bc:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300066c0:	f012 0202 	ands.w	r2, r2, #2
300066c4:	d145      	bne.n	30006752 <BOOT_LoadImages+0xa7a>
300066c6:	9205      	str	r2, [sp, #20]
300066c8:	e4ea      	b.n	300060a0 <BOOT_LoadImages+0x3c8>
300066ca:	4854      	ldr	r0, [pc, #336]	; (3000681c <BOOT_LoadImages+0xb44>)
300066cc:	f005 fd74 	bl	3000c1b8 <__DiagPrintf_veneer>
300066d0:	e7fe      	b.n	300066d0 <BOOT_LoadImages+0x9f8>
300066d2:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300066d4:	e434      	b.n	30005f40 <BOOT_LoadImages+0x268>
300066d6:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300066d8:	e5f3      	b.n	300062c2 <BOOT_LoadImages+0x5ea>
300066da:	f10a 3aff 	add.w	sl, sl, #4294967295
300066de:	f11a 0f03 	cmn.w	sl, #3
300066e2:	d84c      	bhi.n	3000677e <BOOT_LoadImages+0xaa6>
300066e4:	f1bb 0f01 	cmp.w	fp, #1
300066e8:	d966      	bls.n	300067b8 <BOOT_LoadImages+0xae0>
300066ea:	484d      	ldr	r0, [pc, #308]	; (30006820 <BOOT_LoadImages+0xb48>)
300066ec:	e643      	b.n	30006376 <BOOT_LoadImages+0x69e>
300066ee:	f04f 0800 	mov.w	r8, #0
300066f2:	e48d      	b.n	30006010 <BOOT_LoadImages+0x338>
300066f4:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300066f6:	e61a      	b.n	3000632e <BOOT_LoadImages+0x656>
300066f8:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300066fc:	4843      	ldr	r0, [pc, #268]	; (3000680c <BOOT_LoadImages+0xb34>)
300066fe:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006702:	f005 fd59 	bl	3000c1b8 <__DiagPrintf_veneer>
30006706:	2301      	movs	r3, #1
30006708:	4698      	mov	r8, r3
3000670a:	9303      	str	r3, [sp, #12]
3000670c:	e430      	b.n	30005f70 <BOOT_LoadImages+0x298>
3000670e:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006712:	f04f 0801 	mov.w	r8, #1
30006716:	483e      	ldr	r0, [pc, #248]	; (30006810 <BOOT_LoadImages+0xb38>)
30006718:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000671c:	f005 fd4c 	bl	3000c1b8 <__DiagPrintf_veneer>
30006720:	2302      	movs	r3, #2
30006722:	9303      	str	r3, [sp, #12]
30006724:	e424      	b.n	30005f70 <BOOT_LoadImages+0x298>
30006726:	461a      	mov	r2, r3
30006728:	4619      	mov	r1, r3
3000672a:	483b      	ldr	r0, [pc, #236]	; (30006818 <BOOT_LoadImages+0xb40>)
3000672c:	f005 fd44 	bl	3000c1b8 <__DiagPrintf_veneer>
30006730:	4b33      	ldr	r3, [pc, #204]	; (30006800 <BOOT_LoadImages+0xb28>)
30006732:	689f      	ldr	r7, [r3, #8]
30006734:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006738:	f017 0702 	ands.w	r7, r7, #2
3000673c:	f43f af2a 	beq.w	30006594 <BOOT_LoadImages+0x8bc>
30006740:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006744:	2700      	movs	r7, #0
30006746:	4831      	ldr	r0, [pc, #196]	; (3000680c <BOOT_LoadImages+0xb34>)
30006748:	f005 fd36 	bl	3000c1b8 <__DiagPrintf_veneer>
3000674c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006750:	e720      	b.n	30006594 <BOOT_LoadImages+0x8bc>
30006752:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006756:	482e      	ldr	r0, [pc, #184]	; (30006810 <BOOT_LoadImages+0xb38>)
30006758:	f005 fd2e 	bl	3000c1b8 <__DiagPrintf_veneer>
3000675c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006760:	2200      	movs	r2, #0
30006762:	e7b0      	b.n	300066c6 <BOOT_LoadImages+0x9ee>
30006764:	4281      	cmp	r1, r0
30006766:	f4ff acf1 	bcc.w	3000614c <BOOT_LoadImages+0x474>
3000676a:	2400      	movs	r4, #0
3000676c:	464b      	mov	r3, r9
3000676e:	4642      	mov	r2, r8
30006770:	4659      	mov	r1, fp
30006772:	4620      	mov	r0, r4
30006774:	9500      	str	r5, [sp, #0]
30006776:	9401      	str	r4, [sp, #4]
30006778:	e52c      	b.n	300061d4 <BOOT_LoadImages+0x4fc>
3000677a:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000677c:	e6ee      	b.n	3000655c <BOOT_LoadImages+0x884>
3000677e:	4829      	ldr	r0, [pc, #164]	; (30006824 <BOOT_LoadImages+0xb4c>)
30006780:	e5f9      	b.n	30006376 <BOOT_LoadImages+0x69e>
30006782:	2601      	movs	r6, #1
30006784:	464b      	mov	r3, r9
30006786:	9a07      	ldr	r2, [sp, #28]
30006788:	4651      	mov	r1, sl
3000678a:	2000      	movs	r0, #0
3000678c:	9500      	str	r5, [sp, #0]
3000678e:	9601      	str	r6, [sp, #4]
30006790:	e733      	b.n	300065fa <BOOT_LoadImages+0x922>
30006792:	2400      	movs	r4, #0
30006794:	464b      	mov	r3, r9
30006796:	4642      	mov	r2, r8
30006798:	4659      	mov	r1, fp
3000679a:	9500      	str	r5, [sp, #0]
3000679c:	9401      	str	r4, [sp, #4]
3000679e:	e518      	b.n	300061d2 <BOOT_LoadImages+0x4fa>
300067a0:	4c21      	ldr	r4, [pc, #132]	; (30006828 <BOOT_LoadImages+0xb50>)
300067a2:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300067a6:	e481      	b.n	300060ac <BOOT_LoadImages+0x3d4>
300067a8:	f04f 0900 	mov.w	r9, #0
300067ac:	e430      	b.n	30006010 <BOOT_LoadImages+0x338>
300067ae:	2601      	movs	r6, #1
300067b0:	4c1d      	ldr	r4, [pc, #116]	; (30006828 <BOOT_LoadImages+0xb50>)
300067b2:	46b1      	mov	r9, r6
300067b4:	f7ff bbaa 	b.w	30005f0c <BOOT_LoadImages+0x234>
300067b8:	9904      	ldr	r1, [sp, #16]
300067ba:	481c      	ldr	r0, [pc, #112]	; (3000682c <BOOT_LoadImages+0xb54>)
300067bc:	f005 fcfc 	bl	3000c1b8 <__DiagPrintf_veneer>
300067c0:	f8d9 3011 	ldr.w	r3, [r9, #17]
300067c4:	f8d9 a00d 	ldr.w	sl, [r9, #13]
300067c8:	9307      	str	r3, [sp, #28]
300067ca:	2301      	movs	r3, #1
300067cc:	f8d5 b00d 	ldr.w	fp, [r5, #13]
300067d0:	f8d5 8011 	ldr.w	r8, [r5, #17]
300067d4:	9309      	str	r3, [sp, #36]	; 0x24
300067d6:	e5db      	b.n	30006390 <BOOT_LoadImages+0x6b8>
300067d8:	f089 0601 	eor.w	r6, r9, #1
300067dc:	f006 0601 	and.w	r6, r6, #1
300067e0:	f7ff bbd0 	b.w	30005f84 <BOOT_LoadImages+0x2ac>
300067e4:	f089 0601 	eor.w	r6, r9, #1
300067e8:	f8cd 8014 	str.w	r8, [sp, #20]
300067ec:	f006 0601 	and.w	r6, r6, #1
300067f0:	f7ff bbc8 	b.w	30005f84 <BOOT_LoadImages+0x2ac>
300067f4:	300043b9 	.word	0x300043b9
300067f8:	3000d79e 	.word	0x3000d79e
300067fc:	3000cbc4 	.word	0x3000cbc4
30006800:	2001c00c 	.word	0x2001c00c
30006804:	3000cb78 	.word	0x3000cb78
30006808:	3000cb68 	.word	0x3000cb68
3000680c:	3000c84c 	.word	0x3000c84c
30006810:	3000c87c 	.word	0x3000c87c
30006814:	3000cc10 	.word	0x3000cc10
30006818:	3000cb88 	.word	0x3000cb88
3000681c:	3000ccd0 	.word	0x3000ccd0
30006820:	3000cd30 	.word	0x3000cd30
30006824:	3000c9b8 	.word	0x3000c9b8
30006828:	3000dd30 	.word	0x3000dd30
3000682c:	3000cd68 	.word	0x3000cd68
30006830:	3000ee30 	.word	0x3000ee30

30006834 <BOOT_ReasonSet>:
30006834:	4a03      	ldr	r2, [pc, #12]	; (30006844 <BOOT_ReasonSet+0x10>)
30006836:	4904      	ldr	r1, [pc, #16]	; (30006848 <BOOT_ReasonSet+0x14>)
30006838:	8c13      	ldrh	r3, [r2, #32]
3000683a:	b29b      	uxth	r3, r3
3000683c:	8413      	strh	r3, [r2, #32]
3000683e:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006842:	4770      	bx	lr
30006844:	42008000 	.word	0x42008000
30006848:	42008200 	.word	0x42008200

3000684c <BOOT_Enable_KM0>:
3000684c:	b510      	push	{r4, lr}
3000684e:	4c13      	ldr	r4, [pc, #76]	; (3000689c <BOOT_Enable_KM0+0x50>)
30006850:	47a0      	blx	r4
30006852:	b110      	cbz	r0, 3000685a <BOOT_Enable_KM0+0xe>
30006854:	4b12      	ldr	r3, [pc, #72]	; (300068a0 <BOOT_Enable_KM0+0x54>)
30006856:	4798      	blx	r3
30006858:	b1b0      	cbz	r0, 30006888 <BOOT_Enable_KM0+0x3c>
3000685a:	47a0      	blx	r4
3000685c:	b940      	cbnz	r0, 30006870 <BOOT_Enable_KM0+0x24>
3000685e:	4a11      	ldr	r2, [pc, #68]	; (300068a4 <BOOT_Enable_KM0+0x58>)
30006860:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
30006864:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
30006868:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
3000686c:	bf40      	sev
3000686e:	bd10      	pop	{r4, pc}
30006870:	4b0b      	ldr	r3, [pc, #44]	; (300068a0 <BOOT_Enable_KM0+0x54>)
30006872:	4798      	blx	r3
30006874:	2800      	cmp	r0, #0
30006876:	d0f2      	beq.n	3000685e <BOOT_Enable_KM0+0x12>
30006878:	4b0b      	ldr	r3, [pc, #44]	; (300068a8 <BOOT_Enable_KM0+0x5c>)
3000687a:	689b      	ldr	r3, [r3, #8]
3000687c:	079b      	lsls	r3, r3, #30
3000687e:	d5ee      	bpl.n	3000685e <BOOT_Enable_KM0+0x12>
30006880:	480a      	ldr	r0, [pc, #40]	; (300068ac <BOOT_Enable_KM0+0x60>)
30006882:	f005 fc99 	bl	3000c1b8 <__DiagPrintf_veneer>
30006886:	e7ea      	b.n	3000685e <BOOT_Enable_KM0+0x12>
30006888:	4b07      	ldr	r3, [pc, #28]	; (300068a8 <BOOT_Enable_KM0+0x5c>)
3000688a:	689b      	ldr	r3, [r3, #8]
3000688c:	079a      	lsls	r2, r3, #30
3000688e:	d5ee      	bpl.n	3000686e <BOOT_Enable_KM0+0x22>
30006890:	4807      	ldr	r0, [pc, #28]	; (300068b0 <BOOT_Enable_KM0+0x64>)
30006892:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30006896:	f005 bc8f 	b.w	3000c1b8 <__DiagPrintf_veneer>
3000689a:	bf00      	nop
3000689c:	0000c09d 	.word	0x0000c09d
300068a0:	0000c0ad 	.word	0x0000c0ad
300068a4:	42008000 	.word	0x42008000
300068a8:	2001c00c 	.word	0x2001c00c
300068ac:	3000cdc0 	.word	0x3000cdc0
300068b0:	3000cd84 	.word	0x3000cd84

300068b4 <BOOT_AP_Clk_Get>:
300068b4:	4a06      	ldr	r2, [pc, #24]	; (300068d0 <BOOT_AP_Clk_Get+0x1c>)
300068b6:	6913      	ldr	r3, [r2, #16]
300068b8:	2b00      	cmp	r3, #0
300068ba:	db03      	blt.n	300068c4 <BOOT_AP_Clk_Get+0x10>
300068bc:	6810      	ldr	r0, [r2, #0]
300068be:	fbb0 f0f3 	udiv	r0, r0, r3
300068c2:	4770      	bx	lr
300068c4:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300068c8:	6850      	ldr	r0, [r2, #4]
300068ca:	fbb0 f0f3 	udiv	r0, r0, r3
300068ce:	4770      	bx	lr
300068d0:	3000e3b4 	.word	0x3000e3b4

300068d4 <BOOT_SOC_ClkSet>:
300068d4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300068d8:	4f5a      	ldr	r7, [pc, #360]	; (30006a44 <BOOT_SOC_ClkSet+0x170>)
300068da:	693d      	ldr	r5, [r7, #16]
300068dc:	683e      	ldr	r6, [r7, #0]
300068de:	2d00      	cmp	r5, #0
300068e0:	db73      	blt.n	300069ca <BOOT_SOC_ClkSet+0xf6>
300068e2:	fbb6 f5f5 	udiv	r5, r6, r5
300068e6:	4b58      	ldr	r3, [pc, #352]	; (30006a48 <BOOT_SOC_ClkSet+0x174>)
300068e8:	f8d7 a008 	ldr.w	sl, [r7, #8]
300068ec:	fba3 1306 	umull	r1, r3, r3, r6
300068f0:	68fa      	ldr	r2, [r7, #12]
300068f2:	f1ba 0f01 	cmp.w	sl, #1
300068f6:	ea4f 19d3 	mov.w	r9, r3, lsr #7
300068fa:	ea4f 1853 	mov.w	r8, r3, lsr #5
300068fe:	ea4f 1393 	mov.w	r3, r3, lsr #6
30006902:	f109 39ff 	add.w	r9, r9, #4294967295
30006906:	f108 38ff 	add.w	r8, r8, #4294967295
3000690a:	f103 34ff 	add.w	r4, r3, #4294967295
3000690e:	fbb6 f6f2 	udiv	r6, r6, r2
30006912:	d071      	beq.n	300069f8 <BOOT_SOC_ClkSet+0x124>
30006914:	4b4d      	ldr	r3, [pc, #308]	; (30006a4c <BOOT_SOC_ClkSet+0x178>)
30006916:	2200      	movs	r2, #0
30006918:	2efa      	cmp	r6, #250	; 0xfa
3000691a:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
3000691e:	d865      	bhi.n	300069ec <BOOT_SOC_ClkSet+0x118>
30006920:	f5b5 7f66 	cmp.w	r5, #920	; 0x398
30006924:	d85c      	bhi.n	300069e0 <BOOT_SOC_ClkSet+0x10c>
30006926:	2001      	movs	r0, #1
30006928:	ea4f 2909 	mov.w	r9, r9, lsl #8
3000692c:	f001 fb66 	bl	30007ffc <CLK_SWITCH_XTAL>
30006930:	683b      	ldr	r3, [r7, #0]
30006932:	4847      	ldr	r0, [pc, #284]	; (30006a50 <BOOT_SOC_ClkSet+0x17c>)
30006934:	ea4f 3808 	mov.w	r8, r8, lsl #12
30006938:	f409 69e0 	and.w	r9, r9, #1792	; 0x700
3000693c:	0524      	lsls	r4, r4, #20
3000693e:	fb03 f000 	mul.w	r0, r3, r0
30006942:	4b44      	ldr	r3, [pc, #272]	; (30006a54 <BOOT_SOC_ClkSet+0x180>)
30006944:	4798      	blx	r3
30006946:	4944      	ldr	r1, [pc, #272]	; (30006a58 <BOOT_SOC_ClkSet+0x184>)
30006948:	68fb      	ldr	r3, [r7, #12]
3000694a:	fa1f f888 	uxth.w	r8, r8
3000694e:	f8d1 0228 	ldr.w	r0, [r1, #552]	; 0x228
30006952:	f404 04e0 	and.w	r4, r4, #7340032	; 0x700000
30006956:	3b01      	subs	r3, #1
30006958:	4a40      	ldr	r2, [pc, #256]	; (30006a5c <BOOT_SOC_ClkSet+0x188>)
3000695a:	f003 0307 	and.w	r3, r3, #7
3000695e:	4002      	ands	r2, r0
30006960:	2000      	movs	r0, #0
30006962:	4313      	orrs	r3, r2
30006964:	ea49 0303 	orr.w	r3, r9, r3
30006968:	ea48 0303 	orr.w	r3, r8, r3
3000696c:	4323      	orrs	r3, r4
3000696e:	4c3c      	ldr	r4, [pc, #240]	; (30006a60 <BOOT_SOC_ClkSet+0x18c>)
30006970:	f8c1 3228 	str.w	r3, [r1, #552]	; 0x228
30006974:	f001 fb42 	bl	30007ffc <CLK_SWITCH_XTAL>
30006978:	68a3      	ldr	r3, [r4, #8]
3000697a:	079a      	lsls	r2, r3, #30
3000697c:	d42b      	bmi.n	300069d6 <BOOT_SOC_ClkSet+0x102>
3000697e:	4b39      	ldr	r3, [pc, #228]	; (30006a64 <BOOT_SOC_ClkSet+0x190>)
30006980:	781b      	ldrb	r3, [r3, #0]
30006982:	b303      	cbz	r3, 300069c6 <BOOT_SOC_ClkSet+0xf2>
30006984:	693b      	ldr	r3, [r7, #16]
30006986:	2b00      	cmp	r3, #0
30006988:	db48      	blt.n	30006a1c <BOOT_SOC_ClkSet+0x148>
3000698a:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000698e:	6913      	ldr	r3, [r2, #16]
30006990:	f043 030c 	orr.w	r3, r3, #12
30006994:	6113      	str	r3, [r2, #16]
30006996:	2200      	movs	r2, #0
30006998:	4b2c      	ldr	r3, [pc, #176]	; (30006a4c <BOOT_SOC_ClkSet+0x178>)
3000699a:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
3000699e:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
300069a2:	693b      	ldr	r3, [r7, #16]
300069a4:	68a0      	ldr	r0, [r4, #8]
300069a6:	690a      	ldr	r2, [r1, #16]
300069a8:	3b01      	subs	r3, #1
300069aa:	f022 0203 	bic.w	r2, r2, #3
300069ae:	f003 0303 	and.w	r3, r3, #3
300069b2:	4313      	orrs	r3, r2
300069b4:	610b      	str	r3, [r1, #16]
300069b6:	0783      	lsls	r3, r0, #30
300069b8:	d505      	bpl.n	300069c6 <BOOT_SOC_ClkSet+0xf2>
300069ba:	4629      	mov	r1, r5
300069bc:	482a      	ldr	r0, [pc, #168]	; (30006a68 <BOOT_SOC_ClkSet+0x194>)
300069be:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300069c2:	f005 bbf9 	b.w	3000c1b8 <__DiagPrintf_veneer>
300069c6:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300069ca:	f025 4500 	bic.w	r5, r5, #2147483648	; 0x80000000
300069ce:	687b      	ldr	r3, [r7, #4]
300069d0:	fbb3 f5f5 	udiv	r5, r3, r5
300069d4:	e787      	b.n	300068e6 <BOOT_SOC_ClkSet+0x12>
300069d6:	4631      	mov	r1, r6
300069d8:	4824      	ldr	r0, [pc, #144]	; (30006a6c <BOOT_SOC_ClkSet+0x198>)
300069da:	f005 fbed 	bl	3000c1b8 <__DiagPrintf_veneer>
300069de:	e7ce      	b.n	3000697e <BOOT_SOC_ClkSet+0xaa>
300069e0:	f240 6166 	movw	r1, #1638	; 0x666
300069e4:	4822      	ldr	r0, [pc, #136]	; (30006a70 <BOOT_SOC_ClkSet+0x19c>)
300069e6:	f005 fc07 	bl	3000c1f8 <__io_assert_failed_veneer>
300069ea:	e79c      	b.n	30006926 <BOOT_SOC_ClkSet+0x52>
300069ec:	f240 6165 	movw	r1, #1637	; 0x665
300069f0:	481f      	ldr	r0, [pc, #124]	; (30006a70 <BOOT_SOC_ClkSet+0x19c>)
300069f2:	f005 fc01 	bl	3000c1f8 <__io_assert_failed_veneer>
300069f6:	e793      	b.n	30006920 <BOOT_SOC_ClkSet+0x4c>
300069f8:	4b1e      	ldr	r3, [pc, #120]	; (30006a74 <BOOT_SOC_ClkSet+0x1a0>)
300069fa:	4650      	mov	r0, sl
300069fc:	4798      	blx	r3
300069fe:	4b13      	ldr	r3, [pc, #76]	; (30006a4c <BOOT_SOC_ClkSet+0x178>)
30006a00:	f5b6 7fa7 	cmp.w	r6, #334	; 0x14e
30006a04:	f883 a10c 	strb.w	sl, [r3, #268]	; 0x10c
30006a08:	d216      	bcs.n	30006a38 <BOOT_SOC_ClkSet+0x164>
30006a0a:	f5b5 6f96 	cmp.w	r5, #1200	; 0x4b0
30006a0e:	d98a      	bls.n	30006926 <BOOT_SOC_ClkSet+0x52>
30006a10:	f240 6162 	movw	r1, #1634	; 0x662
30006a14:	4816      	ldr	r0, [pc, #88]	; (30006a70 <BOOT_SOC_ClkSet+0x19c>)
30006a16:	f005 fbef 	bl	3000c1f8 <__io_assert_failed_veneer>
30006a1a:	e784      	b.n	30006926 <BOOT_SOC_ClkSet+0x52>
30006a1c:	687b      	ldr	r3, [r7, #4]
30006a1e:	480c      	ldr	r0, [pc, #48]	; (30006a50 <BOOT_SOC_ClkSet+0x17c>)
30006a20:	fb03 f000 	mul.w	r0, r3, r0
30006a24:	4b14      	ldr	r3, [pc, #80]	; (30006a78 <BOOT_SOC_ClkSet+0x1a4>)
30006a26:	4798      	blx	r3
30006a28:	4b14      	ldr	r3, [pc, #80]	; (30006a7c <BOOT_SOC_ClkSet+0x1a8>)
30006a2a:	2001      	movs	r0, #1
30006a2c:	4798      	blx	r3
30006a2e:	4b07      	ldr	r3, [pc, #28]	; (30006a4c <BOOT_SOC_ClkSet+0x178>)
30006a30:	2201      	movs	r2, #1
30006a32:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30006a36:	e7b2      	b.n	3000699e <BOOT_SOC_ClkSet+0xca>
30006a38:	f240 6161 	movw	r1, #1633	; 0x661
30006a3c:	480c      	ldr	r0, [pc, #48]	; (30006a70 <BOOT_SOC_ClkSet+0x19c>)
30006a3e:	f005 fbdb 	bl	3000c1f8 <__io_assert_failed_veneer>
30006a42:	e7e2      	b.n	30006a0a <BOOT_SOC_ClkSet+0x136>
30006a44:	3000e3b4 	.word	0x3000e3b4
30006a48:	51eb851f 	.word	0x51eb851f
30006a4c:	23020000 	.word	0x23020000
30006a50:	000f4240 	.word	0x000f4240
30006a54:	300093f9 	.word	0x300093f9
30006a58:	42008000 	.word	0x42008000
30006a5c:	ff8f08f8 	.word	0xff8f08f8
30006a60:	2001c00c 	.word	0x2001c00c
30006a64:	3000d79e 	.word	0x3000d79e
30006a68:	3000ce1c 	.word	0x3000ce1c
30006a6c:	3000cdf0 	.word	0x3000cdf0
30006a70:	3000d754 	.word	0x3000d754
30006a74:	3000bbc5 	.word	0x3000bbc5
30006a78:	30009479 	.word	0x30009479
30006a7c:	300094b9 	.word	0x300094b9

30006a80 <BOOT_Disable_AP>:
30006a80:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006a84:	4906      	ldr	r1, [pc, #24]	; (30006aa0 <BOOT_Disable_AP+0x20>)
30006a86:	685a      	ldr	r2, [r3, #4]
30006a88:	f042 0202 	orr.w	r2, r2, #2
30006a8c:	605a      	str	r2, [r3, #4]
30006a8e:	685a      	ldr	r2, [r3, #4]
30006a90:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006a94:	605a      	str	r2, [r3, #4]
30006a96:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006a98:	f043 0303 	orr.w	r3, r3, #3
30006a9c:	678b      	str	r3, [r1, #120]	; 0x78
30006a9e:	4770      	bx	lr
30006aa0:	41000200 	.word	0x41000200

30006aa4 <BOOT_Enable_AP>:
30006aa4:	b538      	push	{r3, r4, r5, lr}
30006aa6:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30006aaa:	2201      	movs	r2, #1
30006aac:	2104      	movs	r1, #4
30006aae:	4b2b      	ldr	r3, [pc, #172]	; (30006b5c <BOOT_Enable_AP+0xb8>)
30006ab0:	482b      	ldr	r0, [pc, #172]	; (30006b60 <BOOT_Enable_AP+0xbc>)
30006ab2:	4798      	blx	r3
30006ab4:	6863      	ldr	r3, [r4, #4]
30006ab6:	4d2b      	ldr	r5, [pc, #172]	; (30006b64 <BOOT_Enable_AP+0xc0>)
30006ab8:	2032      	movs	r0, #50	; 0x32
30006aba:	f043 0302 	orr.w	r3, r3, #2
30006abe:	6063      	str	r3, [r4, #4]
30006ac0:	6863      	ldr	r3, [r4, #4]
30006ac2:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30006ac6:	6063      	str	r3, [r4, #4]
30006ac8:	6823      	ldr	r3, [r4, #0]
30006aca:	f043 0302 	orr.w	r3, r3, #2
30006ace:	6023      	str	r3, [r4, #0]
30006ad0:	47a8      	blx	r5
30006ad2:	6823      	ldr	r3, [r4, #0]
30006ad4:	2032      	movs	r0, #50	; 0x32
30006ad6:	f043 0304 	orr.w	r3, r3, #4
30006ada:	6023      	str	r3, [r4, #0]
30006adc:	6823      	ldr	r3, [r4, #0]
30006ade:	f043 0301 	orr.w	r3, r3, #1
30006ae2:	6023      	str	r3, [r4, #0]
30006ae4:	6823      	ldr	r3, [r4, #0]
30006ae6:	f043 0310 	orr.w	r3, r3, #16
30006aea:	6023      	str	r3, [r4, #0]
30006aec:	47a8      	blx	r5
30006aee:	6823      	ldr	r3, [r4, #0]
30006af0:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30006af4:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30006af8:	6023      	str	r3, [r4, #0]
30006afa:	47a8      	blx	r5
30006afc:	6863      	ldr	r3, [r4, #4]
30006afe:	4a1a      	ldr	r2, [pc, #104]	; (30006b68 <BOOT_Enable_AP+0xc4>)
30006b00:	f023 0310 	bic.w	r3, r3, #16
30006b04:	6063      	str	r3, [r4, #4]
30006b06:	6863      	ldr	r3, [r4, #4]
30006b08:	f023 0302 	bic.w	r3, r3, #2
30006b0c:	6063      	str	r3, [r4, #4]
30006b0e:	4790      	blx	r2
30006b10:	b178      	cbz	r0, 30006b32 <BOOT_Enable_AP+0x8e>
30006b12:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006b16:	4815      	ldr	r0, [pc, #84]	; (30006b6c <BOOT_Enable_AP+0xc8>)
30006b18:	4a15      	ldr	r2, [pc, #84]	; (30006b70 <BOOT_Enable_AP+0xcc>)
30006b1a:	68d9      	ldr	r1, [r3, #12]
30006b1c:	f041 0101 	orr.w	r1, r1, #1
30006b20:	60d9      	str	r1, [r3, #12]
30006b22:	6899      	ldr	r1, [r3, #8]
30006b24:	f041 0101 	orr.w	r1, r1, #1
30006b28:	6099      	str	r1, [r3, #8]
30006b2a:	6843      	ldr	r3, [r0, #4]
30006b2c:	431a      	orrs	r2, r3
30006b2e:	6042      	str	r2, [r0, #4]
30006b30:	bd38      	pop	{r3, r4, r5, pc}
30006b32:	4604      	mov	r4, r0
30006b34:	4d0f      	ldr	r5, [pc, #60]	; (30006b74 <BOOT_Enable_AP+0xd0>)
30006b36:	4601      	mov	r1, r0
30006b38:	2041      	movs	r0, #65	; 0x41
30006b3a:	47a8      	blx	r5
30006b3c:	4621      	mov	r1, r4
30006b3e:	2042      	movs	r0, #66	; 0x42
30006b40:	47a8      	blx	r5
30006b42:	4621      	mov	r1, r4
30006b44:	2043      	movs	r0, #67	; 0x43
30006b46:	47a8      	blx	r5
30006b48:	4621      	mov	r1, r4
30006b4a:	2044      	movs	r0, #68	; 0x44
30006b4c:	47a8      	blx	r5
30006b4e:	4621      	mov	r1, r4
30006b50:	2045      	movs	r0, #69	; 0x45
30006b52:	47a8      	blx	r5
30006b54:	4621      	mov	r1, r4
30006b56:	2046      	movs	r0, #70	; 0x46
30006b58:	47a8      	blx	r5
30006b5a:	e7da      	b.n	30006b12 <BOOT_Enable_AP+0x6e>
30006b5c:	0000aab9 	.word	0x0000aab9
30006b60:	4200c000 	.word	0x4200c000
30006b64:	00009b2d 	.word	0x00009b2d
30006b68:	3000a549 	.word	0x3000a549
30006b6c:	41000200 	.word	0x41000200
30006b70:	01001111 	.word	0x01001111
30006b74:	0000b20d 	.word	0x0000b20d

30006b78 <BOOT_RAM_KeyDeriveFunc>:
30006b78:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006b7c:	ed2d 8b04 	vpush	{d8-d9}
30006b80:	b0cb      	sub	sp, #300	; 0x12c
30006b82:	4c6a      	ldr	r4, [pc, #424]	; (30006d2c <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30006b84:	4680      	mov	r8, r0
30006b86:	4689      	mov	r9, r1
30006b88:	4869      	ldr	r0, [pc, #420]	; (30006d30 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30006b8a:	ee08 2a90 	vmov	s17, r2
30006b8e:	4969      	ldr	r1, [pc, #420]	; (30006d34 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006b90:	2201      	movs	r2, #1
30006b92:	ee09 3a10 	vmov	s18, r3
30006b96:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30006b9a:	47a0      	blx	r4
30006b9c:	2201      	movs	r2, #1
30006b9e:	4966      	ldr	r1, [pc, #408]	; (30006d38 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30006ba0:	ae02      	add	r6, sp, #8
30006ba2:	4864      	ldr	r0, [pc, #400]	; (30006d34 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006ba4:	47a0      	blx	r4
30006ba6:	4b65      	ldr	r3, [pc, #404]	; (30006d3c <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30006ba8:	2000      	movs	r0, #0
30006baa:	4798      	blx	r3
30006bac:	22e0      	movs	r2, #224	; 0xe0
30006bae:	2100      	movs	r1, #0
30006bb0:	a812      	add	r0, sp, #72	; 0x48
30006bb2:	f005 fae1 	bl	3000c178 <____wrap_memset_veneer>
30006bb6:	2300      	movs	r3, #0
30006bb8:	f8ad 3004 	strh.w	r3, [sp, #4]
30006bbc:	f88d 3006 	strb.w	r3, [sp, #6]
30006bc0:	2301      	movs	r3, #1
30006bc2:	f88d 3007 	strb.w	r3, [sp, #7]
30006bc6:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006bc8:	2b00      	cmp	r3, #0
30006bca:	f000 80ab 	beq.w	30006d24 <BOOT_RAM_KeyDeriveFunc+0x1ac>
30006bce:	ab0a      	add	r3, sp, #40	; 0x28
30006bd0:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30006d50 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30006bd4:	4f5a      	ldr	r7, [pc, #360]	; (30006d40 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30006bd6:	ee08 3a10 	vmov	s16, r3
30006bda:	ab12      	add	r3, sp, #72	; 0x48
30006bdc:	464a      	mov	r2, r9
30006bde:	4641      	mov	r1, r8
30006be0:	2020      	movs	r0, #32
30006be2:	47d8      	blx	fp
30006be4:	aa12      	add	r2, sp, #72	; 0x48
30006be6:	ee19 1a10 	vmov	r1, s18
30006bea:	ee18 0a90 	vmov	r0, s17
30006bee:	47b8      	blx	r7
30006bf0:	ee18 5a10 	vmov	r5, s16
30006bf4:	aa12      	add	r2, sp, #72	; 0x48
30006bf6:	2104      	movs	r1, #4
30006bf8:	a801      	add	r0, sp, #4
30006bfa:	4634      	mov	r4, r6
30006bfc:	47b8      	blx	r7
30006bfe:	a912      	add	r1, sp, #72	; 0x48
30006c00:	4b50      	ldr	r3, [pc, #320]	; (30006d44 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c02:	ee18 0a10 	vmov	r0, s16
30006c06:	4798      	blx	r3
30006c08:	f1ba 0f01 	cmp.w	sl, #1
30006c0c:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30006c0e:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30006c10:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30006c14:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006c18:	d91f      	bls.n	30006c5a <BOOT_RAM_KeyDeriveFunc+0xe2>
30006c1a:	2501      	movs	r5, #1
30006c1c:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30006c20:	ab12      	add	r3, sp, #72	; 0x48
30006c22:	4641      	mov	r1, r8
30006c24:	464a      	mov	r2, r9
30006c26:	2020      	movs	r0, #32
30006c28:	47d8      	blx	fp
30006c2a:	aa12      	add	r2, sp, #72	; 0x48
30006c2c:	2120      	movs	r1, #32
30006c2e:	4630      	mov	r0, r6
30006c30:	47b8      	blx	r7
30006c32:	a912      	add	r1, sp, #72	; 0x48
30006c34:	4b43      	ldr	r3, [pc, #268]	; (30006d44 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c36:	4630      	mov	r0, r6
30006c38:	4798      	blx	r3
30006c3a:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30006c3e:	f10d 0107 	add.w	r1, sp, #7
30006c42:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30006c46:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30006c4a:	42a3      	cmp	r3, r4
30006c4c:	ea82 0200 	eor.w	r2, r2, r0
30006c50:	701a      	strb	r2, [r3, #0]
30006c52:	d1f6      	bne.n	30006c42 <BOOT_RAM_KeyDeriveFunc+0xca>
30006c54:	3501      	adds	r5, #1
30006c56:	45aa      	cmp	sl, r5
30006c58:	d1e2      	bne.n	30006c20 <BOOT_RAM_KeyDeriveFunc+0xa8>
30006c5a:	9a59      	ldr	r2, [sp, #356]	; 0x164
30006c5c:	ee18 1a10 	vmov	r1, s16
30006c60:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c62:	2a20      	cmp	r2, #32
30006c64:	985a      	ldr	r0, [sp, #360]	; 0x168
30006c66:	bf28      	it	cs
30006c68:	2220      	movcs	r2, #32
30006c6a:	1a9b      	subs	r3, r3, r2
30006c6c:	9359      	str	r3, [sp, #356]	; 0x164
30006c6e:	4603      	mov	r3, r0
30006c70:	4413      	add	r3, r2
30006c72:	935a      	str	r3, [sp, #360]	; 0x168
30006c74:	f005 fa78 	bl	3000c168 <____wrap_memcpy_veneer>
30006c78:	f10d 0207 	add.w	r2, sp, #7
30006c7c:	2104      	movs	r1, #4
30006c7e:	7813      	ldrb	r3, [r2, #0]
30006c80:	3901      	subs	r1, #1
30006c82:	3301      	adds	r3, #1
30006c84:	b2db      	uxtb	r3, r3
30006c86:	f802 3901 	strb.w	r3, [r2], #-1
30006c8a:	b90b      	cbnz	r3, 30006c90 <BOOT_RAM_KeyDeriveFunc+0x118>
30006c8c:	2900      	cmp	r1, #0
30006c8e:	d1f6      	bne.n	30006c7e <BOOT_RAM_KeyDeriveFunc+0x106>
30006c90:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c92:	2b00      	cmp	r3, #0
30006c94:	d1a1      	bne.n	30006bda <BOOT_RAM_KeyDeriveFunc+0x62>
30006c96:	2220      	movs	r2, #32
30006c98:	2100      	movs	r1, #0
30006c9a:	4c2b      	ldr	r4, [pc, #172]	; (30006d48 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30006c9c:	4630      	mov	r0, r6
30006c9e:	47a0      	blx	r4
30006ca0:	2220      	movs	r2, #32
30006ca2:	2100      	movs	r1, #0
30006ca4:	ee18 0a10 	vmov	r0, s16
30006ca8:	47a0      	blx	r4
30006caa:	4b28      	ldr	r3, [pc, #160]	; (30006d4c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cac:	695b      	ldr	r3, [r3, #20]
30006cae:	03da      	lsls	r2, r3, #15
30006cb0:	d512      	bpl.n	30006cd8 <BOOT_RAM_KeyDeriveFunc+0x160>
30006cb2:	f016 021f 	ands.w	r2, r6, #31
30006cb6:	4633      	mov	r3, r6
30006cb8:	d130      	bne.n	30006d1c <BOOT_RAM_KeyDeriveFunc+0x1a4>
30006cba:	2220      	movs	r2, #32
30006cbc:	f3bf 8f4f 	dsb	sy
30006cc0:	4822      	ldr	r0, [pc, #136]	; (30006d4c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cc2:	441a      	add	r2, r3
30006cc4:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006cc8:	3320      	adds	r3, #32
30006cca:	1ad1      	subs	r1, r2, r3
30006ccc:	2900      	cmp	r1, #0
30006cce:	dcf9      	bgt.n	30006cc4 <BOOT_RAM_KeyDeriveFunc+0x14c>
30006cd0:	f3bf 8f4f 	dsb	sy
30006cd4:	f3bf 8f6f 	isb	sy
30006cd8:	4b1c      	ldr	r3, [pc, #112]	; (30006d4c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cda:	695b      	ldr	r3, [r3, #20]
30006cdc:	03db      	lsls	r3, r3, #15
30006cde:	d513      	bpl.n	30006d08 <BOOT_RAM_KeyDeriveFunc+0x190>
30006ce0:	ee18 3a10 	vmov	r3, s16
30006ce4:	f013 021f 	ands.w	r2, r3, #31
30006ce8:	d114      	bne.n	30006d14 <BOOT_RAM_KeyDeriveFunc+0x19c>
30006cea:	2220      	movs	r2, #32
30006cec:	f3bf 8f4f 	dsb	sy
30006cf0:	4816      	ldr	r0, [pc, #88]	; (30006d4c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cf2:	441a      	add	r2, r3
30006cf4:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006cf8:	3320      	adds	r3, #32
30006cfa:	1ad1      	subs	r1, r2, r3
30006cfc:	2900      	cmp	r1, #0
30006cfe:	dcf9      	bgt.n	30006cf4 <BOOT_RAM_KeyDeriveFunc+0x17c>
30006d00:	f3bf 8f4f 	dsb	sy
30006d04:	f3bf 8f6f 	isb	sy
30006d08:	2000      	movs	r0, #0
30006d0a:	b04b      	add	sp, #300	; 0x12c
30006d0c:	ecbd 8b04 	vpop	{d8-d9}
30006d10:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006d14:	f023 031f 	bic.w	r3, r3, #31
30006d18:	3220      	adds	r2, #32
30006d1a:	e7e7      	b.n	30006cec <BOOT_RAM_KeyDeriveFunc+0x174>
30006d1c:	f026 031f 	bic.w	r3, r6, #31
30006d20:	3220      	adds	r2, #32
30006d22:	e7cb      	b.n	30006cbc <BOOT_RAM_KeyDeriveFunc+0x144>
30006d24:	ab0a      	add	r3, sp, #40	; 0x28
30006d26:	ee08 3a10 	vmov	s16, r3
30006d2a:	e7b4      	b.n	30006c96 <BOOT_RAM_KeyDeriveFunc+0x11e>
30006d2c:	0000b479 	.word	0x0000b479
30006d30:	40000008 	.word	0x40000008
30006d34:	40000004 	.word	0x40000004
30006d38:	40000002 	.word	0x40000002
30006d3c:	00002451 	.word	0x00002451
30006d40:	00005b4d 	.word	0x00005b4d
30006d44:	00005bb9 	.word	0x00005bb9
30006d48:	00012be5 	.word	0x00012be5
30006d4c:	e000ed00 	.word	0xe000ed00
30006d50:	30009e71 	.word	0x30009e71

30006d54 <password_hash_check>:
30006d54:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006d58:	b08f      	sub	sp, #60	; 0x3c
30006d5a:	4690      	mov	r8, r2
30006d5c:	461d      	mov	r5, r3
30006d5e:	4606      	mov	r6, r0
30006d60:	af04      	add	r7, sp, #16
30006d62:	3201      	adds	r2, #1
30006d64:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006d66:	6079      	str	r1, [r7, #4]
30006d68:	2100      	movs	r1, #0
30006d6a:	f103 0008 	add.w	r0, r3, #8
30006d6e:	f108 0308 	add.w	r3, r8, #8
30006d72:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
30006d76:	f023 0307 	bic.w	r3, r3, #7
30006d7a:	f020 0007 	bic.w	r0, r0, #7
30006d7e:	ebad 0d03 	sub.w	sp, sp, r3
30006d82:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006d84:	f10d 0910 	add.w	r9, sp, #16
30006d88:	3301      	adds	r3, #1
30006d8a:	ebad 0d00 	sub.w	sp, sp, r0
30006d8e:	603b      	str	r3, [r7, #0]
30006d90:	4648      	mov	r0, r9
30006d92:	4b3f      	ldr	r3, [pc, #252]	; (30006e90 <password_hash_check+0x13c>)
30006d94:	f10d 0a10 	add.w	sl, sp, #16
30006d98:	4798      	blx	r3
30006d9a:	683b      	ldr	r3, [r7, #0]
30006d9c:	2100      	movs	r1, #0
30006d9e:	4650      	mov	r0, sl
30006da0:	461a      	mov	r2, r3
30006da2:	4b3b      	ldr	r3, [pc, #236]	; (30006e90 <password_hash_check+0x13c>)
30006da4:	4798      	blx	r3
30006da6:	f1b8 0001 	subs.w	r0, r8, #1
30006daa:	d01d      	beq.n	30006de8 <password_hash_check+0x94>
30006dac:	2200      	movs	r2, #0
30006dae:	f109 33ff 	add.w	r3, r9, #4294967295
30006db2:	4611      	mov	r1, r2
30006db4:	e00b      	b.n	30006dce <password_hash_check+0x7a>
30006db6:	f108 38ff 	add.w	r8, r8, #4294967295
30006dba:	2100      	movs	r1, #0
30006dbc:	1c9c      	adds	r4, r3, #2
30006dbe:	4282      	cmp	r2, r0
30006dc0:	7059      	strb	r1, [r3, #1]
30006dc2:	f103 0301 	add.w	r3, r3, #1
30006dc6:	4611      	mov	r1, r2
30006dc8:	eba4 0409 	sub.w	r4, r4, r9
30006dcc:	d20b      	bcs.n	30006de6 <password_hash_check+0x92>
30006dce:	1874      	adds	r4, r6, r1
30006dd0:	f102 0c01 	add.w	ip, r2, #1
30006dd4:	5c71      	ldrb	r1, [r6, r1]
30006dd6:	3202      	adds	r2, #2
30006dd8:	7864      	ldrb	r4, [r4, #1]
30006dda:	2c30      	cmp	r4, #48	; 0x30
30006ddc:	d101      	bne.n	30006de2 <password_hash_check+0x8e>
30006dde:	295c      	cmp	r1, #92	; 0x5c
30006de0:	d0e9      	beq.n	30006db6 <password_hash_check+0x62>
30006de2:	4662      	mov	r2, ip
30006de4:	e7ea      	b.n	30006dbc <password_hash_check+0x68>
30006de6:	4406      	add	r6, r0
30006de8:	7833      	ldrb	r3, [r6, #0]
30006dea:	f809 3004 	strb.w	r3, [r9, r4]
30006dee:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006df0:	1e5c      	subs	r4, r3, #1
30006df2:	d04a      	beq.n	30006e8a <password_hash_check+0x136>
30006df4:	2200      	movs	r2, #0
30006df6:	f10a 33ff 	add.w	r3, sl, #4294967295
30006dfa:	4611      	mov	r1, r2
30006dfc:	e00b      	b.n	30006e16 <password_hash_check+0xc2>
30006dfe:	f10b 3bff 	add.w	fp, fp, #4294967295
30006e02:	2100      	movs	r1, #0
30006e04:	1c98      	adds	r0, r3, #2
30006e06:	42a2      	cmp	r2, r4
30006e08:	7059      	strb	r1, [r3, #1]
30006e0a:	f103 0301 	add.w	r3, r3, #1
30006e0e:	4611      	mov	r1, r2
30006e10:	eba0 000a 	sub.w	r0, r0, sl
30006e14:	d20a      	bcs.n	30006e2c <password_hash_check+0xd8>
30006e16:	1868      	adds	r0, r5, r1
30006e18:	1c56      	adds	r6, r2, #1
30006e1a:	5c69      	ldrb	r1, [r5, r1]
30006e1c:	3202      	adds	r2, #2
30006e1e:	7840      	ldrb	r0, [r0, #1]
30006e20:	2830      	cmp	r0, #48	; 0x30
30006e22:	d101      	bne.n	30006e28 <password_hash_check+0xd4>
30006e24:	295c      	cmp	r1, #92	; 0x5c
30006e26:	d0ea      	beq.n	30006dfe <password_hash_check+0xaa>
30006e28:	4632      	mov	r2, r6
30006e2a:	e7eb      	b.n	30006e04 <password_hash_check+0xb0>
30006e2c:	4425      	add	r5, r4
30006e2e:	782b      	ldrb	r3, [r5, #0]
30006e30:	f107 0408 	add.w	r4, r7, #8
30006e34:	2520      	movs	r5, #32
30006e36:	4641      	mov	r1, r8
30006e38:	f80a 3000 	strb.w	r3, [sl, r0]
30006e3c:	4652      	mov	r2, sl
30006e3e:	687b      	ldr	r3, [r7, #4]
30006e40:	4648      	mov	r0, r9
30006e42:	9402      	str	r4, [sp, #8]
30006e44:	9300      	str	r3, [sp, #0]
30006e46:	465b      	mov	r3, fp
30006e48:	9501      	str	r5, [sp, #4]
30006e4a:	f7ff fe95 	bl	30006b78 <BOOT_RAM_KeyDeriveFunc>
30006e4e:	4620      	mov	r0, r4
30006e50:	462a      	mov	r2, r5
30006e52:	4910      	ldr	r1, [pc, #64]	; (30006e94 <password_hash_check+0x140>)
30006e54:	f005 f998 	bl	3000c188 <____wrap_memcmp_veneer>
30006e58:	4604      	mov	r4, r0
30006e5a:	b968      	cbnz	r0, 30006e78 <password_hash_check+0x124>
30006e5c:	480e      	ldr	r0, [pc, #56]	; (30006e98 <password_hash_check+0x144>)
30006e5e:	f005 f9ab 	bl	3000c1b8 <__DiagPrintf_veneer>
30006e62:	462a      	mov	r2, r5
30006e64:	4621      	mov	r1, r4
30006e66:	f107 0008 	add.w	r0, r7, #8
30006e6a:	4b09      	ldr	r3, [pc, #36]	; (30006e90 <password_hash_check+0x13c>)
30006e6c:	4798      	blx	r3
30006e6e:	4620      	mov	r0, r4
30006e70:	372c      	adds	r7, #44	; 0x2c
30006e72:	46bd      	mov	sp, r7
30006e74:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e78:	2401      	movs	r4, #1
30006e7a:	4808      	ldr	r0, [pc, #32]	; (30006e9c <password_hash_check+0x148>)
30006e7c:	f005 f99c 	bl	3000c1b8 <__DiagPrintf_veneer>
30006e80:	372c      	adds	r7, #44	; 0x2c
30006e82:	4620      	mov	r0, r4
30006e84:	46bd      	mov	sp, r7
30006e86:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e8a:	4620      	mov	r0, r4
30006e8c:	e7cf      	b.n	30006e2e <password_hash_check+0xda>
30006e8e:	bf00      	nop
30006e90:	00012be5 	.word	0x00012be5
30006e94:	3000e138 	.word	0x3000e138
30006e98:	3000ce60 	.word	0x3000ce60
30006e9c:	3000ce48 	.word	0x3000ce48

30006ea0 <BOOT_Image1>:
30006ea0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006ea4:	49b5      	ldr	r1, [pc, #724]	; (3000717c <BOOT_Image1+0x2dc>)
30006ea6:	b08b      	sub	sp, #44	; 0x2c
30006ea8:	48b5      	ldr	r0, [pc, #724]	; (30007180 <BOOT_Image1+0x2e0>)
30006eaa:	f005 f985 	bl	3000c1b8 <__DiagPrintf_veneer>
30006eae:	48b5      	ldr	r0, [pc, #724]	; (30007184 <BOOT_Image1+0x2e4>)
30006eb0:	4ab5      	ldr	r2, [pc, #724]	; (30007188 <BOOT_Image1+0x2e8>)
30006eb2:	2100      	movs	r1, #0
30006eb4:	4bb5      	ldr	r3, [pc, #724]	; (3000718c <BOOT_Image1+0x2ec>)
30006eb6:	1a12      	subs	r2, r2, r0
30006eb8:	4798      	blx	r3
30006eba:	4ab5      	ldr	r2, [pc, #724]	; (30007190 <BOOT_Image1+0x2f0>)
30006ebc:	49b5      	ldr	r1, [pc, #724]	; (30007194 <BOOT_Image1+0x2f4>)
30006ebe:	8c13      	ldrh	r3, [r2, #32]
30006ec0:	b29b      	uxth	r3, r3
30006ec2:	8413      	strh	r3, [r2, #32]
30006ec4:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006ec8:	f002 fef6 	bl	30009cb8 <BOOT_Reason>
30006ecc:	2800      	cmp	r0, #0
30006ece:	f000 831c 	beq.w	3000750a <BOOT_Image1+0x66a>
30006ed2:	4bb1      	ldr	r3, [pc, #708]	; (30007198 <BOOT_Image1+0x2f8>)
30006ed4:	781b      	ldrb	r3, [r3, #0]
30006ed6:	2b00      	cmp	r3, #0
30006ed8:	f040 8322 	bne.w	30007520 <BOOT_Image1+0x680>
30006edc:	4daf      	ldr	r5, [pc, #700]	; (3000719c <BOOT_Image1+0x2fc>)
30006ede:	682b      	ldr	r3, [r5, #0]
30006ee0:	4eaf      	ldr	r6, [pc, #700]	; (300071a0 <BOOT_Image1+0x300>)
30006ee2:	f043 0302 	orr.w	r3, r3, #2
30006ee6:	4aaf      	ldr	r2, [pc, #700]	; (300071a4 <BOOT_Image1+0x304>)
30006ee8:	602b      	str	r3, [r5, #0]
30006eea:	4790      	blx	r2
30006eec:	7834      	ldrb	r4, [r6, #0]
30006eee:	2c00      	cmp	r4, #0
30006ef0:	f000 82d2 	beq.w	30007498 <BOOT_Image1+0x5f8>
30006ef4:	2000      	movs	r0, #0
30006ef6:	4bac      	ldr	r3, [pc, #688]	; (300071a8 <BOOT_Image1+0x308>)
30006ef8:	4798      	blx	r3
30006efa:	4bac      	ldr	r3, [pc, #688]	; (300071ac <BOOT_Image1+0x30c>)
30006efc:	2001      	movs	r0, #1
30006efe:	4798      	blx	r3
30006f00:	4cab      	ldr	r4, [pc, #684]	; (300071b0 <BOOT_Image1+0x310>)
30006f02:	f7ff fce7 	bl	300068d4 <BOOT_SOC_ClkSet>
30006f06:	f7fe fca3 	bl	30005850 <BOOT_GRstConfig>
30006f0a:	47a0      	blx	r4
30006f0c:	2802      	cmp	r0, #2
30006f0e:	4ba9      	ldr	r3, [pc, #676]	; (300071b4 <BOOT_Image1+0x314>)
30006f10:	f000 82cc 	beq.w	300074ac <BOOT_Image1+0x60c>
30006f14:	2701      	movs	r7, #1
30006f16:	49a8      	ldr	r1, [pc, #672]	; (300071b8 <BOOT_Image1+0x318>)
30006f18:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30006f1c:	f8df 82ec 	ldr.w	r8, [pc, #748]	; 3000720c <BOOT_Image1+0x36c>
30006f20:	f883 710e 	strb.w	r7, [r3, #270]	; 0x10e
30006f24:	463a      	mov	r2, r7
30006f26:	47c0      	blx	r8
30006f28:	463a      	mov	r2, r7
30006f2a:	49a4      	ldr	r1, [pc, #656]	; (300071bc <BOOT_Image1+0x31c>)
30006f2c:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30006f30:	47c0      	blx	r8
30006f32:	4a97      	ldr	r2, [pc, #604]	; (30007190 <BOOT_Image1+0x2f0>)
30006f34:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30006f38:	f043 0303 	orr.w	r3, r3, #3
30006f3c:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30006f40:	4b9f      	ldr	r3, [pc, #636]	; (300071c0 <BOOT_Image1+0x320>)
30006f42:	4798      	blx	r3
30006f44:	b930      	cbnz	r0, 30006f54 <BOOT_Image1+0xb4>
30006f46:	4a92      	ldr	r2, [pc, #584]	; (30007190 <BOOT_Image1+0x2f0>)
30006f48:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30006f4c:	f023 0303 	bic.w	r3, r3, #3
30006f50:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30006f54:	4b9b      	ldr	r3, [pc, #620]	; (300071c4 <BOOT_Image1+0x324>)
30006f56:	781b      	ldrb	r3, [r3, #0]
30006f58:	2b00      	cmp	r3, #0
30006f5a:	f040 8335 	bne.w	300075c8 <BOOT_Image1+0x728>
30006f5e:	4f9a      	ldr	r7, [pc, #616]	; (300071c8 <BOOT_Image1+0x328>)
30006f60:	2102      	movs	r1, #2
30006f62:	2201      	movs	r2, #1
30006f64:	4899      	ldr	r0, [pc, #612]	; (300071cc <BOOT_Image1+0x32c>)
30006f66:	47b8      	blx	r7
30006f68:	68ab      	ldr	r3, [r5, #8]
30006f6a:	0799      	lsls	r1, r3, #30
30006f6c:	f100 8326 	bmi.w	300075bc <BOOT_Image1+0x71c>
30006f70:	4997      	ldr	r1, [pc, #604]	; (300071d0 <BOOT_Image1+0x330>)
30006f72:	e841 f100 	tt	r1, r1
30006f76:	68ab      	ldr	r3, [r5, #8]
30006f78:	f3c1 5180 	ubfx	r1, r1, #22, #1
30006f7c:	079a      	lsls	r2, r3, #30
30006f7e:	f100 82da 	bmi.w	30007536 <BOOT_Image1+0x696>
30006f82:	4b94      	ldr	r3, [pc, #592]	; (300071d4 <BOOT_Image1+0x334>)
30006f84:	4f94      	ldr	r7, [pc, #592]	; (300071d8 <BOOT_Image1+0x338>)
30006f86:	681b      	ldr	r3, [r3, #0]
30006f88:	4798      	blx	r3
30006f8a:	f7fe fa53 	bl	30005434 <BOOT_RccConfig>
30006f8e:	f7fe fabd 	bl	3000550c <BOOT_CACHEWRR_Set>
30006f92:	4b92      	ldr	r3, [pc, #584]	; (300071dc <BOOT_Image1+0x33c>)
30006f94:	6818      	ldr	r0, [r3, #0]
30006f96:	f7fe fae7 	bl	30005568 <BOOT_TCMSet>
30006f9a:	4b91      	ldr	r3, [pc, #580]	; (300071e0 <BOOT_Image1+0x340>)
30006f9c:	695b      	ldr	r3, [r3, #20]
30006f9e:	03db      	lsls	r3, r3, #15
30006fa0:	d513      	bpl.n	30006fca <BOOT_Image1+0x12a>
30006fa2:	f017 011f 	ands.w	r1, r7, #31
30006fa6:	f040 8256 	bne.w	30007456 <BOOT_Image1+0x5b6>
30006faa:	463b      	mov	r3, r7
30006fac:	2174      	movs	r1, #116	; 0x74
30006fae:	f3bf 8f4f 	dsb	sy
30006fb2:	488b      	ldr	r0, [pc, #556]	; (300071e0 <BOOT_Image1+0x340>)
30006fb4:	4419      	add	r1, r3
30006fb6:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30006fba:	3320      	adds	r3, #32
30006fbc:	1aca      	subs	r2, r1, r3
30006fbe:	2a00      	cmp	r2, #0
30006fc0:	dcf9      	bgt.n	30006fb6 <BOOT_Image1+0x116>
30006fc2:	f3bf 8f4f 	dsb	sy
30006fc6:	f3bf 8f6f 	isb	sy
30006fca:	4b71      	ldr	r3, [pc, #452]	; (30007190 <BOOT_Image1+0x2f0>)
30006fcc:	2200      	movs	r2, #0
30006fce:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30006fd2:	4b78      	ldr	r3, [pc, #480]	; (300071b4 <BOOT_Image1+0x314>)
30006fd4:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30006fd8:	f7fc fc02 	bl	300037e0 <flash_highspeed_setup>
30006fdc:	f001 fe18 	bl	30008c10 <flash_layout_init>
30006fe0:	47a0      	blx	r4
30006fe2:	2802      	cmp	r0, #2
30006fe4:	f000 8273 	beq.w	300074ce <BOOT_Image1+0x62e>
30006fe8:	7832      	ldrb	r2, [r6, #0]
30006fea:	68ab      	ldr	r3, [r5, #8]
30006fec:	b162      	cbz	r2, 30007008 <BOOT_Image1+0x168>
30006fee:	4c7d      	ldr	r4, [pc, #500]	; (300071e4 <BOOT_Image1+0x344>)
30006ff0:	4e7d      	ldr	r6, [pc, #500]	; (300071e8 <BOOT_Image1+0x348>)
30006ff2:	f013 0f02 	tst.w	r3, #2
30006ff6:	4630      	mov	r0, r6
30006ff8:	d001      	beq.n	30006ffe <BOOT_Image1+0x15e>
30006ffa:	f005 f8dd 	bl	3000c1b8 <__DiagPrintf_veneer>
30006ffe:	f241 3088 	movw	r0, #5000	; 0x1388
30007002:	47a0      	blx	r4
30007004:	68ab      	ldr	r3, [r5, #8]
30007006:	e7f4      	b.n	30006ff2 <BOOT_Image1+0x152>
30007008:	079a      	lsls	r2, r3, #30
3000700a:	f100 836b 	bmi.w	300076e4 <BOOT_Image1+0x844>
3000700e:	f7fe fc5b 	bl	300058c8 <BOOT_DDR_Init>
30007012:	4a5f      	ldr	r2, [pc, #380]	; (30007190 <BOOT_Image1+0x2f0>)
30007014:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30007018:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
3000701c:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30007020:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30007024:	2400      	movs	r4, #0
30007026:	4a5a      	ldr	r2, [pc, #360]	; (30007190 <BOOT_Image1+0x2f0>)
30007028:	2307      	movs	r3, #7
3000702a:	f44f 7159 	mov.w	r1, #868	; 0x364
3000702e:	f8c2 42d0 	str.w	r4, [r2, #720]	; 0x2d0
30007032:	4620      	mov	r0, r4
30007034:	f8c2 42d4 	str.w	r4, [r2, #724]	; 0x2d4
30007038:	f8c2 42d8 	str.w	r4, [r2, #728]	; 0x2d8
3000703c:	f8c2 40d0 	str.w	r4, [r2, #208]	; 0xd0
30007040:	f10d 021d 	add.w	r2, sp, #29
30007044:	f002 f9c0 	bl	300093c8 <EFUSERead8>
30007048:	f10d 011f 	add.w	r1, sp, #31
3000704c:	f44f 705a 	mov.w	r0, #872	; 0x368
30007050:	f88d 401e 	strb.w	r4, [sp, #30]
30007054:	f88d 401f 	strb.w	r4, [sp, #31]
30007058:	f002 f8d6 	bl	30009208 <OTP_Read8>
3000705c:	f10d 011e 	add.w	r1, sp, #30
30007060:	f240 3069 	movw	r0, #873	; 0x369
30007064:	f002 f8d0 	bl	30009208 <OTP_Read8>
30007068:	f89d 301d 	ldrb.w	r3, [sp, #29]
3000706c:	f013 0f1e 	tst.w	r3, #30
30007070:	d10a      	bne.n	30007088 <BOOT_Image1+0x1e8>
30007072:	f89d 201e 	ldrb.w	r2, [sp, #30]
30007076:	f89d 301f 	ldrb.w	r3, [sp, #31]
3000707a:	f002 0208 	and.w	r2, r2, #8
3000707e:	f003 0301 	and.w	r3, r3, #1
30007082:	4313      	orrs	r3, r2
30007084:	f040 81eb 	bne.w	3000745e <BOOT_Image1+0x5be>
30007088:	4b58      	ldr	r3, [pc, #352]	; (300071ec <BOOT_Image1+0x34c>)
3000708a:	4798      	blx	r3
3000708c:	2801      	cmp	r0, #1
3000708e:	f04f 0100 	mov.w	r1, #0
30007092:	f04f 0002 	mov.w	r0, #2
30007096:	bf0c      	ite	eq
30007098:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
3000709c:	4a54      	ldrne	r2, [pc, #336]	; (300071f0 <BOOT_Image1+0x350>)
3000709e:	f000 fdb7 	bl	30007c10 <ymodem_uart_port_init>
300070a2:	4b52      	ldr	r3, [pc, #328]	; (300071ec <BOOT_Image1+0x34c>)
300070a4:	4798      	blx	r3
300070a6:	2800      	cmp	r0, #0
300070a8:	f000 822a 	beq.w	30007500 <BOOT_Image1+0x660>
300070ac:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
300070b0:	4850      	ldr	r0, [pc, #320]	; (300071f4 <BOOT_Image1+0x354>)
300070b2:	f005 f881 	bl	3000c1b8 <__DiagPrintf_veneer>
300070b6:	4850      	ldr	r0, [pc, #320]	; (300071f8 <BOOT_Image1+0x358>)
300070b8:	2632      	movs	r6, #50	; 0x32
300070ba:	f005 f86d 	bl	3000c198 <____wrap_strlen_veneer>
300070be:	4601      	mov	r1, r0
300070c0:	484d      	ldr	r0, [pc, #308]	; (300071f8 <BOOT_Image1+0x358>)
300070c2:	4c48      	ldr	r4, [pc, #288]	; (300071e4 <BOOT_Image1+0x344>)
300070c4:	f000 fdfe 	bl	30007cc4 <ymodem_uart_putdata>
300070c8:	e002      	b.n	300070d0 <BOOT_Image1+0x230>
300070ca:	3e01      	subs	r6, #1
300070cc:	f000 82f0 	beq.w	300076b0 <BOOT_Image1+0x810>
300070d0:	2001      	movs	r0, #1
300070d2:	47a0      	blx	r4
300070d4:	f000 fddc 	bl	30007c90 <ymodem_uart_readable>
300070d8:	2800      	cmp	r0, #0
300070da:	d0f6      	beq.n	300070ca <BOOT_Image1+0x22a>
300070dc:	f000 fdd8 	bl	30007c90 <ymodem_uart_readable>
300070e0:	2800      	cmp	r0, #0
300070e2:	f000 830d 	beq.w	30007700 <BOOT_Image1+0x860>
300070e6:	2301      	movs	r3, #1
300070e8:	9303      	str	r3, [sp, #12]
300070ea:	2001      	movs	r0, #1
300070ec:	47a0      	blx	r4
300070ee:	2108      	movs	r1, #8
300070f0:	4842      	ldr	r0, [pc, #264]	; (300071fc <BOOT_Image1+0x35c>)
300070f2:	f000 fdf5 	bl	30007ce0 <ymodem_uart_getdata>
300070f6:	4941      	ldr	r1, [pc, #260]	; (300071fc <BOOT_Image1+0x35c>)
300070f8:	2208      	movs	r2, #8
300070fa:	4b41      	ldr	r3, [pc, #260]	; (30007200 <BOOT_Image1+0x360>)
300070fc:	f101 0080 	add.w	r0, r1, #128	; 0x80
30007100:	4798      	blx	r3
30007102:	9004      	str	r0, [sp, #16]
30007104:	2800      	cmp	r0, #0
30007106:	f040 8370 	bne.w	300077ea <BOOT_Image1+0x94a>
3000710a:	f000 fdc1 	bl	30007c90 <ymodem_uart_readable>
3000710e:	2800      	cmp	r0, #0
30007110:	d0fb      	beq.n	3000710a <BOOT_Image1+0x26a>
30007112:	2101      	movs	r1, #1
30007114:	483b      	ldr	r0, [pc, #236]	; (30007204 <BOOT_Image1+0x364>)
30007116:	f000 fde3 	bl	30007ce0 <ymodem_uart_getdata>
3000711a:	f000 fdb9 	bl	30007c90 <ymodem_uart_readable>
3000711e:	2800      	cmp	r0, #0
30007120:	d0fb      	beq.n	3000711a <BOOT_Image1+0x27a>
30007122:	4e39      	ldr	r6, [pc, #228]	; (30007208 <BOOT_Image1+0x368>)
30007124:	2101      	movs	r1, #1
30007126:	f206 404d 	addw	r0, r6, #1101	; 0x44d
3000712a:	f000 fdd9 	bl	30007ce0 <ymodem_uart_getdata>
3000712e:	f896 244d 	ldrb.w	r2, [r6, #1101]	; 0x44d
30007132:	f896 34cd 	ldrb.w	r3, [r6, #1229]	; 0x4cd
30007136:	429a      	cmp	r2, r3
30007138:	f000 8362 	beq.w	30007800 <BOOT_Image1+0x960>
3000713c:	f206 484e 	addw	r8, r6, #1102	; 0x44e
30007140:	2700      	movs	r7, #0
30007142:	46b9      	mov	r9, r7
30007144:	3701      	adds	r7, #1
30007146:	f000 fda3 	bl	30007c90 <ymodem_uart_readable>
3000714a:	2800      	cmp	r0, #0
3000714c:	d0fb      	beq.n	30007146 <BOOT_Image1+0x2a6>
3000714e:	4640      	mov	r0, r8
30007150:	2101      	movs	r1, #1
30007152:	f000 fdc5 	bl	30007ce0 <ymodem_uart_getdata>
30007156:	f898 2000 	ldrb.w	r2, [r8]
3000715a:	f896 34cd 	ldrb.w	r3, [r6, #1229]	; 0x4cd
3000715e:	f109 090b 	add.w	r9, r9, #11
30007162:	f108 0801 	add.w	r8, r8, #1
30007166:	46bb      	mov	fp, r7
30007168:	429a      	cmp	r2, r3
3000716a:	d1ea      	bne.n	30007142 <BOOT_Image1+0x2a2>
3000716c:	2f00      	cmp	r7, #0
3000716e:	dd5e      	ble.n	3000722e <BOOT_Image1+0x38e>
30007170:	f206 414c 	addw	r1, r6, #1100	; 0x44c
30007174:	2300      	movs	r3, #0
30007176:	19c8      	adds	r0, r1, r7
30007178:	e04c      	b.n	30007214 <BOOT_Image1+0x374>
3000717a:	bf00      	nop
3000717c:	3000d764 	.word	0x3000d764
30007180:	3000ce7c 	.word	0x3000ce7c
30007184:	3000ea70 	.word	0x3000ea70
30007188:	3000eea4 	.word	0x3000eea4
3000718c:	00012be5 	.word	0x00012be5
30007190:	42008000 	.word	0x42008000
30007194:	42008200 	.word	0x42008200
30007198:	3000d7b1 	.word	0x3000d7b1
3000719c:	2001c00c 	.word	0x2001c00c
300071a0:	3000d79d 	.word	0x3000d79d
300071a4:	3000bbe5 	.word	0x3000bbe5
300071a8:	3000ba55 	.word	0x3000ba55
300071ac:	3000b975 	.word	0x3000b975
300071b0:	30007ec5 	.word	0x30007ec5
300071b4:	23020000 	.word	0x23020000
300071b8:	40080000 	.word	0x40080000
300071bc:	40040000 	.word	0x40040000
300071c0:	3000a549 	.word	0x3000a549
300071c4:	3000d79c 	.word	0x3000d79c
300071c8:	0000aab9 	.word	0x0000aab9
300071cc:	4200c000 	.word	0x4200c000
300071d0:	30006ea1 	.word	0x30006ea1
300071d4:	00033000 	.word	0x00033000
300071d8:	2001c01c 	.word	0x2001c01c
300071dc:	3000d7a0 	.word	0x3000d7a0
300071e0:	e000ed00 	.word	0xe000ed00
300071e4:	00009be5 	.word	0x00009be5
300071e8:	3000cf2c 	.word	0x3000cf2c
300071ec:	0000c169 	.word	0x0000c169
300071f0:	0016e360 	.word	0x0016e360
300071f4:	3000cffc 	.word	0x3000cffc
300071f8:	3000e158 	.word	0x3000e158
300071fc:	3000e174 	.word	0x3000e174
30007200:	000129bd 	.word	0x000129bd
30007204:	3000e17c 	.word	0x3000e17c
30007208:	3000dd30 	.word	0x3000dd30
3000720c:	0000b479 	.word	0x0000b479
30007210:	4288      	cmp	r0, r1
30007212:	d00d      	beq.n	30007230 <BOOT_Image1+0x390>
30007214:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30007218:	eb03 0383 	add.w	r3, r3, r3, lsl #2
3000721c:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
30007220:	eb02 0343 	add.w	r3, r2, r3, lsl #1
30007224:	f1bc 0f09 	cmp.w	ip, #9
30007228:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
3000722c:	d9f0      	bls.n	30007210 <BOOT_Image1+0x370>
3000722e:	2300      	movs	r3, #0
30007230:	f8c6 34d0 	str.w	r3, [r6, #1232]	; 0x4d0
30007234:	f000 fd2c 	bl	30007c90 <ymodem_uart_readable>
30007238:	2800      	cmp	r0, #0
3000723a:	d0fb      	beq.n	30007234 <BOOT_Image1+0x394>
3000723c:	49c0      	ldr	r1, [pc, #768]	; (30007540 <BOOT_Image1+0x6a0>)
3000723e:	460b      	mov	r3, r1
30007240:	2101      	movs	r1, #1
30007242:	eb03 0009 	add.w	r0, r3, r9
30007246:	9302      	str	r3, [sp, #8]
30007248:	f000 fd4a 	bl	30007ce0 <ymodem_uart_getdata>
3000724c:	9902      	ldr	r1, [sp, #8]
3000724e:	f896 34cd 	ldrb.w	r3, [r6, #1229]	; 0x4cd
30007252:	f811 2009 	ldrb.w	r2, [r1, r9]
30007256:	429a      	cmp	r2, r3
30007258:	f000 82da 	beq.w	30007810 <BOOT_Image1+0x970>
3000725c:	f10b 080b 	add.w	r8, fp, #11
30007260:	f04f 0900 	mov.w	r9, #0
30007264:	4488      	add	r8, r1
30007266:	f109 0901 	add.w	r9, r9, #1
3000726a:	f000 fd11 	bl	30007c90 <ymodem_uart_readable>
3000726e:	2800      	cmp	r0, #0
30007270:	d0fb      	beq.n	3000726a <BOOT_Image1+0x3ca>
30007272:	4640      	mov	r0, r8
30007274:	2101      	movs	r1, #1
30007276:	f000 fd33 	bl	30007ce0 <ymodem_uart_getdata>
3000727a:	f898 2000 	ldrb.w	r2, [r8]
3000727e:	f896 34cd 	ldrb.w	r3, [r6, #1229]	; 0x4cd
30007282:	f108 0801 	add.w	r8, r8, #1
30007286:	eb09 0a0b 	add.w	sl, r9, fp
3000728a:	429a      	cmp	r2, r3
3000728c:	d1eb      	bne.n	30007266 <BOOT_Image1+0x3c6>
3000728e:	f000 fcff 	bl	30007c90 <ymodem_uart_readable>
30007292:	2800      	cmp	r0, #0
30007294:	d0fb      	beq.n	3000728e <BOOT_Image1+0x3ee>
30007296:	9b02      	ldr	r3, [sp, #8]
30007298:	f10a 080b 	add.w	r8, sl, #11
3000729c:	2101      	movs	r1, #1
3000729e:	eb03 0008 	add.w	r0, r3, r8
300072a2:	f000 fd1d 	bl	30007ce0 <ymodem_uart_getdata>
300072a6:	9902      	ldr	r1, [sp, #8]
300072a8:	f896 24cd 	ldrb.w	r2, [r6, #1229]	; 0x4cd
300072ac:	f811 3008 	ldrb.w	r3, [r1, r8]
300072b0:	429a      	cmp	r2, r3
300072b2:	f000 82ab 	beq.w	3000780c <BOOT_Image1+0x96c>
300072b6:	f10a 080c 	add.w	r8, sl, #12
300072ba:	46d3      	mov	fp, sl
300072bc:	f8dd a010 	ldr.w	sl, [sp, #16]
300072c0:	4488      	add	r8, r1
300072c2:	9405      	str	r4, [sp, #20]
300072c4:	f10a 0a01 	add.w	sl, sl, #1
300072c8:	f000 fce2 	bl	30007c90 <ymodem_uart_readable>
300072cc:	2800      	cmp	r0, #0
300072ce:	d0fb      	beq.n	300072c8 <BOOT_Image1+0x428>
300072d0:	4640      	mov	r0, r8
300072d2:	2101      	movs	r1, #1
300072d4:	f000 fd04 	bl	30007ce0 <ymodem_uart_getdata>
300072d8:	f898 2000 	ldrb.w	r2, [r8]
300072dc:	f896 34cd 	ldrb.w	r3, [r6, #1229]	; 0x4cd
300072e0:	f108 0801 	add.w	r8, r8, #1
300072e4:	eb0a 010b 	add.w	r1, sl, fp
300072e8:	429a      	cmp	r2, r3
300072ea:	d1eb      	bne.n	300072c4 <BOOT_Image1+0x424>
300072ec:	468b      	mov	fp, r1
300072ee:	9c05      	ldr	r4, [sp, #20]
300072f0:	f8cd a010 	str.w	sl, [sp, #16]
300072f4:	9b02      	ldr	r3, [sp, #8]
300072f6:	f10b 080c 	add.w	r8, fp, #12
300072fa:	2101      	movs	r1, #1
300072fc:	eb03 0008 	add.w	r0, r3, r8
30007300:	f000 fcee 	bl	30007ce0 <ymodem_uart_getdata>
30007304:	9b02      	ldr	r3, [sp, #8]
30007306:	f813 1008 	ldrb.w	r1, [r3, r8]
3000730a:	290a      	cmp	r1, #10
3000730c:	f8c6 14d4 	str.w	r1, [r6, #1236]	; 0x4d4
30007310:	f200 8267 	bhi.w	300077e2 <BOOT_Image1+0x942>
30007314:	9b02      	ldr	r3, [sp, #8]
30007316:	f10b 000d 	add.w	r0, fp, #13
3000731a:	2101      	movs	r1, #1
3000731c:	f10b 080e 	add.w	r8, fp, #14
30007320:	4418      	add	r0, r3
30007322:	f000 fcdd 	bl	30007ce0 <ymodem_uart_getdata>
30007326:	9b02      	ldr	r3, [sp, #8]
30007328:	2102      	movs	r1, #2
3000732a:	4498      	add	r8, r3
3000732c:	4640      	mov	r0, r8
3000732e:	f000 fcd7 	bl	30007ce0 <ymodem_uart_getdata>
30007332:	4641      	mov	r1, r8
30007334:	2202      	movs	r2, #2
30007336:	4883      	ldr	r0, [pc, #524]	; (30007544 <BOOT_Image1+0x6a4>)
30007338:	4b83      	ldr	r3, [pc, #524]	; (30007548 <BOOT_Image1+0x6a8>)
3000733a:	4798      	blx	r3
3000733c:	2800      	cmp	r0, #0
3000733e:	f040 825b 	bne.w	300077f8 <BOOT_Image1+0x958>
30007342:	9b02      	ldr	r3, [sp, #8]
30007344:	f10b 0010 	add.w	r0, fp, #16
30007348:	2101      	movs	r1, #1
3000734a:	f10b 0811 	add.w	r8, fp, #17
3000734e:	4418      	add	r0, r3
30007350:	f000 fcc6 	bl	30007ce0 <ymodem_uart_getdata>
30007354:	4b7d      	ldr	r3, [pc, #500]	; (3000754c <BOOT_Image1+0x6ac>)
30007356:	f8d6 04d4 	ldr.w	r0, [r6, #1236]	; 0x4d4
3000735a:	f103 0228 	add.w	r2, r3, #40	; 0x28
3000735e:	f103 0130 	add.w	r1, r3, #48	; 0x30
30007362:	f7fd fdab 	bl	30004ebc <hotfix_get_addr_size>
30007366:	9b02      	ldr	r3, [sp, #8]
30007368:	2102      	movs	r1, #2
3000736a:	4498      	add	r8, r3
3000736c:	4640      	mov	r0, r8
3000736e:	f000 fcb7 	bl	30007ce0 <ymodem_uart_getdata>
30007372:	4641      	mov	r1, r8
30007374:	2202      	movs	r2, #2
30007376:	4876      	ldr	r0, [pc, #472]	; (30007550 <BOOT_Image1+0x6b0>)
30007378:	4b73      	ldr	r3, [pc, #460]	; (30007548 <BOOT_Image1+0x6a8>)
3000737a:	4798      	blx	r3
3000737c:	2800      	cmp	r0, #0
3000737e:	f000 8145 	beq.w	3000760c <BOOT_Image1+0x76c>
30007382:	9b03      	ldr	r3, [sp, #12]
30007384:	2b00      	cmp	r3, #0
30007386:	f000 818c 	beq.w	300076a2 <BOOT_Image1+0x802>
3000738a:	f7fe fca5 	bl	30005cd8 <BOOT_LoadImages>
3000738e:	2800      	cmp	r0, #0
30007390:	f000 8135 	beq.w	300075fe <BOOT_Image1+0x75e>
30007394:	f7ff fa5a 	bl	3000684c <BOOT_Enable_KM0>
30007398:	2008      	movs	r0, #8
3000739a:	4b6e      	ldr	r3, [pc, #440]	; (30007554 <BOOT_Image1+0x6b4>)
3000739c:	4798      	blx	r3
3000739e:	4b6e      	ldr	r3, [pc, #440]	; (30007558 <BOOT_Image1+0x6b8>)
300073a0:	2201      	movs	r2, #1
300073a2:	f8c3 2398 	str.w	r2, [r3, #920]	; 0x398
300073a6:	f7fd fcad 	bl	30004d04 <BOOT_RAM_TZCfg>
300073aa:	4b6c      	ldr	r3, [pc, #432]	; (3000755c <BOOT_Image1+0x6bc>)
300073ac:	781b      	ldrb	r3, [r3, #0]
300073ae:	2b01      	cmp	r3, #1
300073b0:	f000 8207 	beq.w	300077c2 <BOOT_Image1+0x922>
300073b4:	4b6a      	ldr	r3, [pc, #424]	; (30007560 <BOOT_Image1+0x6c0>)
300073b6:	781b      	ldrb	r3, [r3, #0]
300073b8:	2b00      	cmp	r3, #0
300073ba:	f000 817f 	beq.w	300076bc <BOOT_Image1+0x81c>
300073be:	f7ff fb71 	bl	30006aa4 <BOOT_Enable_AP>
300073c2:	4a68      	ldr	r2, [pc, #416]	; (30007564 <BOOT_Image1+0x6c4>)
300073c4:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
300073c8:	f043 0302 	orr.w	r3, r3, #2
300073cc:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
300073d0:	4b62      	ldr	r3, [pc, #392]	; (3000755c <BOOT_Image1+0x6bc>)
300073d2:	781b      	ldrb	r3, [r3, #0]
300073d4:	2b01      	cmp	r3, #1
300073d6:	f000 81fc 	beq.w	300077d2 <BOOT_Image1+0x932>
300073da:	4962      	ldr	r1, [pc, #392]	; (30007564 <BOOT_Image1+0x6c4>)
300073dc:	4a62      	ldr	r2, [pc, #392]	; (30007568 <BOOT_Image1+0x6c8>)
300073de:	f891 3067 	ldrb.w	r3, [r1, #103]	; 0x67
300073e2:	6894      	ldr	r4, [r2, #8]
300073e4:	f043 0301 	orr.w	r3, r3, #1
300073e8:	f881 3067 	strb.w	r3, [r1, #103]	; 0x67
300073ec:	4b5f      	ldr	r3, [pc, #380]	; (3000756c <BOOT_Image1+0x6cc>)
300073ee:	6811      	ldr	r1, [r2, #0]
300073f0:	4a5f      	ldr	r2, [pc, #380]	; (30007570 <BOOT_Image1+0x6d0>)
300073f2:	6061      	str	r1, [r4, #4]
300073f4:	6094      	str	r4, [r2, #8]
300073f6:	6a59      	ldr	r1, [r3, #36]	; 0x24
300073f8:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300073fc:	6259      	str	r1, [r3, #36]	; 0x24
300073fe:	6a51      	ldr	r1, [r2, #36]	; 0x24
30007400:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30007404:	6251      	str	r1, [r2, #36]	; 0x24
30007406:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000740a:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000740e:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30007412:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30007416:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000741a:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
3000741e:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30007422:	4954      	ldr	r1, [pc, #336]	; (30007574 <BOOT_Image1+0x6d4>)
30007424:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30007428:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000742c:	689b      	ldr	r3, [r3, #8]
3000742e:	618b      	str	r3, [r1, #24]
30007430:	f7fe fbda 	bl	30005be8 <BOOT_NVICBackup_HP>
30007434:	4b50      	ldr	r3, [pc, #320]	; (30007578 <BOOT_Image1+0x6d8>)
30007436:	f383 8888 	msr	MSP_NS, r3
3000743a:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
3000743e:	f383 8809 	msr	PSP, r3
30007442:	68ab      	ldr	r3, [r5, #8]
30007444:	079b      	lsls	r3, r3, #30
30007446:	f100 8151 	bmi.w	300076ec <BOOT_Image1+0x84c>
3000744a:	6860      	ldr	r0, [r4, #4]
3000744c:	f7fd ffa2 	bl	30005394 <BOOT_NsStart>
30007450:	b00b      	add	sp, #44	; 0x2c
30007452:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007456:	3174      	adds	r1, #116	; 0x74
30007458:	f027 031f 	bic.w	r3, r7, #31
3000745c:	e5a7      	b.n	30006fae <BOOT_Image1+0x10e>
3000745e:	4847      	ldr	r0, [pc, #284]	; (3000757c <BOOT_Image1+0x6dc>)
30007460:	f004 feaa 	bl	3000c1b8 <__DiagPrintf_veneer>
30007464:	f89d 101f 	ldrb.w	r1, [sp, #31]
30007468:	f89d 301e 	ldrb.w	r3, [sp, #30]
3000746c:	f44f 705a 	mov.w	r0, #872	; 0x368
30007470:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
30007474:	f023 0308 	bic.w	r3, r3, #8
30007478:	f88d 101f 	strb.w	r1, [sp, #31]
3000747c:	f88d 301e 	strb.w	r3, [sp, #30]
30007480:	f001 fecc 	bl	3000921c <OTP_Write8>
30007484:	f89d 101e 	ldrb.w	r1, [sp, #30]
30007488:	f240 3069 	movw	r0, #873	; 0x369
3000748c:	f001 fec6 	bl	3000921c <OTP_Write8>
30007490:	483b      	ldr	r0, [pc, #236]	; (30007580 <BOOT_Image1+0x6e0>)
30007492:	f004 fe91 	bl	3000c1b8 <__DiagPrintf_veneer>
30007496:	e5f7      	b.n	30007088 <BOOT_Image1+0x1e8>
30007498:	4b3a      	ldr	r3, [pc, #232]	; (30007584 <BOOT_Image1+0x6e4>)
3000749a:	2001      	movs	r0, #1
3000749c:	4798      	blx	r3
3000749e:	4620      	mov	r0, r4
300074a0:	4b39      	ldr	r3, [pc, #228]	; (30007588 <BOOT_Image1+0x6e8>)
300074a2:	4798      	blx	r3
300074a4:	4b39      	ldr	r3, [pc, #228]	; (3000758c <BOOT_Image1+0x6ec>)
300074a6:	2001      	movs	r0, #1
300074a8:	4798      	blx	r3
300074aa:	e529      	b.n	30006f00 <BOOT_Image1+0x60>
300074ac:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
300074b0:	2201      	movs	r2, #1
300074b2:	4b37      	ldr	r3, [pc, #220]	; (30007590 <BOOT_Image1+0x6f0>)
300074b4:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
300074b8:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
300074bc:	4798      	blx	r3
300074be:	4a35      	ldr	r2, [pc, #212]	; (30007594 <BOOT_Image1+0x6f4>)
300074c0:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
300074c4:	f043 0302 	orr.w	r3, r3, #2
300074c8:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
300074cc:	e538      	b.n	30006f40 <BOOT_Image1+0xa0>
300074ce:	68ab      	ldr	r3, [r5, #8]
300074d0:	0798      	lsls	r0, r3, #30
300074d2:	f100 8103 	bmi.w	300076dc <BOOT_Image1+0x83c>
300074d6:	4b30      	ldr	r3, [pc, #192]	; (30007598 <BOOT_Image1+0x6f8>)
300074d8:	4798      	blx	r3
300074da:	f7fe fa33 	bl	30005944 <BOOT_PSRAM_Init>
300074de:	7830      	ldrb	r0, [r6, #0]
300074e0:	2800      	cmp	r0, #0
300074e2:	f47f ad9f 	bne.w	30007024 <BOOT_Image1+0x184>
300074e6:	4b2d      	ldr	r3, [pc, #180]	; (3000759c <BOOT_Image1+0x6fc>)
300074e8:	4798      	blx	r3
300074ea:	2800      	cmp	r0, #0
300074ec:	f43f ad9a 	beq.w	30007024 <BOOT_Image1+0x184>
300074f0:	682b      	ldr	r3, [r5, #0]
300074f2:	0399      	lsls	r1, r3, #14
300074f4:	f57f ad96 	bpl.w	30007024 <BOOT_Image1+0x184>
300074f8:	4829      	ldr	r0, [pc, #164]	; (300075a0 <BOOT_Image1+0x700>)
300074fa:	f004 fe5d 	bl	3000c1b8 <__DiagPrintf_veneer>
300074fe:	e591      	b.n	30007024 <BOOT_Image1+0x184>
30007500:	4928      	ldr	r1, [pc, #160]	; (300075a4 <BOOT_Image1+0x704>)
30007502:	4829      	ldr	r0, [pc, #164]	; (300075a8 <BOOT_Image1+0x708>)
30007504:	f004 fe58 	bl	3000c1b8 <__DiagPrintf_veneer>
30007508:	e5d5      	b.n	300070b6 <BOOT_Image1+0x216>
3000750a:	4601      	mov	r1, r0
3000750c:	f44f 7201 	mov.w	r2, #516	; 0x204
30007510:	4826      	ldr	r0, [pc, #152]	; (300075ac <BOOT_Image1+0x70c>)
30007512:	f004 fe31 	bl	3000c178 <____wrap_memset_veneer>
30007516:	4b11      	ldr	r3, [pc, #68]	; (3000755c <BOOT_Image1+0x6bc>)
30007518:	781b      	ldrb	r3, [r3, #0]
3000751a:	2b00      	cmp	r3, #0
3000751c:	f43f acde 	beq.w	30006edc <BOOT_Image1+0x3c>
30007520:	4b23      	ldr	r3, [pc, #140]	; (300075b0 <BOOT_Image1+0x710>)
30007522:	4d24      	ldr	r5, [pc, #144]	; (300075b4 <BOOT_Image1+0x714>)
30007524:	4798      	blx	r3
30007526:	2800      	cmp	r0, #0
30007528:	f47f acd9 	bne.w	30006ede <BOOT_Image1+0x3e>
3000752c:	68ab      	ldr	r3, [r5, #8]
3000752e:	f043 0302 	orr.w	r3, r3, #2
30007532:	60ab      	str	r3, [r5, #8]
30007534:	e4d3      	b.n	30006ede <BOOT_Image1+0x3e>
30007536:	4820      	ldr	r0, [pc, #128]	; (300075b8 <BOOT_Image1+0x718>)
30007538:	f004 fe3e 	bl	3000c1b8 <__DiagPrintf_veneer>
3000753c:	e521      	b.n	30006f82 <BOOT_Image1+0xe2>
3000753e:	bf00      	nop
30007540:	3000e174 	.word	0x3000e174
30007544:	3000e208 	.word	0x3000e208
30007548:	000129bd 	.word	0x000129bd
3000754c:	3000e20c 	.word	0x3000e20c
30007550:	3000e248 	.word	0x3000e248
30007554:	0000e1d9 	.word	0x0000e1d9
30007558:	400e0000 	.word	0x400e0000
3000755c:	3000d7b1 	.word	0x3000d7b1
30007560:	3000d79e 	.word	0x3000d79e
30007564:	42008200 	.word	0x42008200
30007568:	60000020 	.word	0x60000020
3000756c:	e000ed00 	.word	0xe000ed00
30007570:	e002ed00 	.word	0xe002ed00
30007574:	3000ee34 	.word	0x3000ee34
30007578:	2001bffc 	.word	0x2001bffc
3000757c:	3000cfa0 	.word	0x3000cfa0
30007580:	3000cfd8 	.word	0x3000cfd8
30007584:	3000b975 	.word	0x3000b975
30007588:	3000bba5 	.word	0x3000bba5
3000758c:	3000bb15 	.word	0x3000bb15
30007590:	0000b479 	.word	0x0000b479
30007594:	42008000 	.word	0x42008000
30007598:	30009519 	.word	0x30009519
3000759c:	3000ba75 	.word	0x3000ba75
300075a0:	3000cf1c 	.word	0x3000cf1c
300075a4:	0016e360 	.word	0x0016e360
300075a8:	3000cffc 	.word	0x3000cffc
300075ac:	23020000 	.word	0x23020000
300075b0:	0000c179 	.word	0x0000c179
300075b4:	2001c00c 	.word	0x2001c00c
300075b8:	3000cec4 	.word	0x3000cec4
300075bc:	f3ef 8108 	mrs	r1, MSP
300075c0:	4899      	ldr	r0, [pc, #612]	; (30007828 <BOOT_Image1+0x988>)
300075c2:	f004 fdf9 	bl	3000c1b8 <__DiagPrintf_veneer>
300075c6:	e4d3      	b.n	30006f70 <BOOT_Image1+0xd0>
300075c8:	f8df 82c8 	ldr.w	r8, [pc, #712]	; 30007894 <BOOT_Image1+0x9f4>
300075cc:	47c0      	blx	r8
300075ce:	4607      	mov	r7, r0
300075d0:	47c0      	blx	r8
300075d2:	2f02      	cmp	r7, #2
300075d4:	f43f acc3 	beq.w	30006f5e <BOOT_Image1+0xbe>
300075d8:	2803      	cmp	r0, #3
300075da:	f43f acc0 	beq.w	30006f5e <BOOT_Image1+0xbe>
300075de:	4b93      	ldr	r3, [pc, #588]	; (3000782c <BOOT_Image1+0x98c>)
300075e0:	4798      	blx	r3
300075e2:	4f93      	ldr	r7, [pc, #588]	; (30007830 <BOOT_Image1+0x990>)
300075e4:	2200      	movs	r2, #0
300075e6:	2101      	movs	r1, #1
300075e8:	4892      	ldr	r0, [pc, #584]	; (30007834 <BOOT_Image1+0x994>)
300075ea:	47b8      	blx	r7
300075ec:	2101      	movs	r1, #1
300075ee:	4891      	ldr	r0, [pc, #580]	; (30007834 <BOOT_Image1+0x994>)
300075f0:	4b91      	ldr	r3, [pc, #580]	; (30007838 <BOOT_Image1+0x998>)
300075f2:	4798      	blx	r3
300075f4:	2201      	movs	r2, #1
300075f6:	488f      	ldr	r0, [pc, #572]	; (30007834 <BOOT_Image1+0x994>)
300075f8:	4611      	mov	r1, r2
300075fa:	47b8      	blx	r7
300075fc:	e4b0      	b.n	30006f60 <BOOT_Image1+0xc0>
300075fe:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007602:	47a0      	blx	r4
30007604:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007608:	47a0      	blx	r4
3000760a:	e7f8      	b.n	300075fe <BOOT_Image1+0x75e>
3000760c:	9b02      	ldr	r3, [sp, #8]
3000760e:	f10b 0013 	add.w	r0, fp, #19
30007612:	2101      	movs	r1, #1
30007614:	f10b 0814 	add.w	r8, fp, #20
30007618:	4418      	add	r0, r3
3000761a:	f000 fb61 	bl	30007ce0 <ymodem_uart_getdata>
3000761e:	4b87      	ldr	r3, [pc, #540]	; (3000783c <BOOT_Image1+0x99c>)
30007620:	f8d6 04d4 	ldr.w	r0, [r6, #1236]	; 0x4d4
30007624:	f1a3 0218 	sub.w	r2, r3, #24
30007628:	f1a3 0110 	sub.w	r1, r3, #16
3000762c:	f7fd fc46 	bl	30004ebc <hotfix_get_addr_size>
30007630:	9b02      	ldr	r3, [sp, #8]
30007632:	2102      	movs	r1, #2
30007634:	4498      	add	r8, r3
30007636:	4640      	mov	r0, r8
30007638:	f000 fb52 	bl	30007ce0 <ymodem_uart_getdata>
3000763c:	4641      	mov	r1, r8
3000763e:	2202      	movs	r2, #2
30007640:	487f      	ldr	r0, [pc, #508]	; (30007840 <BOOT_Image1+0x9a0>)
30007642:	4b80      	ldr	r3, [pc, #512]	; (30007844 <BOOT_Image1+0x9a4>)
30007644:	4798      	blx	r3
30007646:	2800      	cmp	r0, #0
30007648:	f47f ae9b 	bne.w	30007382 <BOOT_Image1+0x4e2>
3000764c:	9b02      	ldr	r3, [sp, #8]
3000764e:	f10b 0016 	add.w	r0, fp, #22
30007652:	2101      	movs	r1, #1
30007654:	f10b 0817 	add.w	r8, fp, #23
30007658:	4418      	add	r0, r3
3000765a:	f000 fb41 	bl	30007ce0 <ymodem_uart_getdata>
3000765e:	4b7a      	ldr	r3, [pc, #488]	; (30007848 <BOOT_Image1+0x9a8>)
30007660:	f8d6 04d4 	ldr.w	r0, [r6, #1236]	; 0x4d4
30007664:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
30007668:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
3000766c:	f7fd fc26 	bl	30004ebc <hotfix_get_addr_size>
30007670:	9a02      	ldr	r2, [sp, #8]
30007672:	2102      	movs	r1, #2
30007674:	4490      	add	r8, r2
30007676:	4640      	mov	r0, r8
30007678:	f000 fb32 	bl	30007ce0 <ymodem_uart_getdata>
3000767c:	4641      	mov	r1, r8
3000767e:	2202      	movs	r2, #2
30007680:	4872      	ldr	r0, [pc, #456]	; (3000784c <BOOT_Image1+0x9ac>)
30007682:	4b70      	ldr	r3, [pc, #448]	; (30007844 <BOOT_Image1+0x9a4>)
30007684:	4798      	blx	r3
30007686:	2800      	cmp	r0, #0
30007688:	f47f ae7b 	bne.w	30007382 <BOOT_Image1+0x4e2>
3000768c:	9b02      	ldr	r3, [sp, #8]
3000768e:	f10b 0019 	add.w	r0, fp, #25
30007692:	2101      	movs	r1, #1
30007694:	4418      	add	r0, r3
30007696:	f000 fb23 	bl	30007ce0 <ymodem_uart_getdata>
3000769a:	9b03      	ldr	r3, [sp, #12]
3000769c:	2b00      	cmp	r3, #0
3000769e:	f040 80bb 	bne.w	30007818 <BOOT_Image1+0x978>
300076a2:	2002      	movs	r0, #2
300076a4:	f000 facc 	bl	30007c40 <ymodem_uart_port_deinit>
300076a8:	4869      	ldr	r0, [pc, #420]	; (30007850 <BOOT_Image1+0x9b0>)
300076aa:	f004 fd85 	bl	3000c1b8 <__DiagPrintf_veneer>
300076ae:	e66c      	b.n	3000738a <BOOT_Image1+0x4ea>
300076b0:	f000 faee 	bl	30007c90 <ymodem_uart_readable>
300076b4:	2800      	cmp	r0, #0
300076b6:	d0f4      	beq.n	300076a2 <BOOT_Image1+0x802>
300076b8:	9603      	str	r6, [sp, #12]
300076ba:	e516      	b.n	300070ea <BOOT_Image1+0x24a>
300076bc:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300076c0:	4964      	ldr	r1, [pc, #400]	; (30007854 <BOOT_Image1+0x9b4>)
300076c2:	685a      	ldr	r2, [r3, #4]
300076c4:	f042 0202 	orr.w	r2, r2, #2
300076c8:	605a      	str	r2, [r3, #4]
300076ca:	685a      	ldr	r2, [r3, #4]
300076cc:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300076d0:	605a      	str	r2, [r3, #4]
300076d2:	6f8b      	ldr	r3, [r1, #120]	; 0x78
300076d4:	f043 0303 	orr.w	r3, r3, #3
300076d8:	678b      	str	r3, [r1, #120]	; 0x78
300076da:	e679      	b.n	300073d0 <BOOT_Image1+0x530>
300076dc:	485e      	ldr	r0, [pc, #376]	; (30007858 <BOOT_Image1+0x9b8>)
300076de:	f004 fd6b 	bl	3000c1b8 <__DiagPrintf_veneer>
300076e2:	e6f8      	b.n	300074d6 <BOOT_Image1+0x636>
300076e4:	485d      	ldr	r0, [pc, #372]	; (3000785c <BOOT_Image1+0x9bc>)
300076e6:	f004 fd67 	bl	3000c1b8 <__DiagPrintf_veneer>
300076ea:	e490      	b.n	3000700e <BOOT_Image1+0x16e>
300076ec:	6861      	ldr	r1, [r4, #4]
300076ee:	485c      	ldr	r0, [pc, #368]	; (30007860 <BOOT_Image1+0x9c0>)
300076f0:	f004 fd62 	bl	3000c1b8 <__DiagPrintf_veneer>
300076f4:	6860      	ldr	r0, [r4, #4]
300076f6:	f7fd fe4d 	bl	30005394 <BOOT_NsStart>
300076fa:	b00b      	add	sp, #44	; 0x2c
300076fc:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007700:	4e58      	ldr	r6, [pc, #352]	; (30007864 <BOOT_Image1+0x9c4>)
30007702:	4680      	mov	r8, r0
30007704:	f04f 0a0b 	mov.w	sl, #11
30007708:	270a      	movs	r7, #10
3000770a:	f206 4344 	addw	r3, r6, #1092	; 0x444
3000770e:	4681      	mov	r9, r0
30007710:	9302      	str	r3, [sp, #8]
30007712:	2004      	movs	r0, #4
30007714:	f000 fa94 	bl	30007c40 <ymodem_uart_port_deinit>
30007718:	9a02      	ldr	r2, [sp, #8]
3000771a:	f8cd 8000 	str.w	r8, [sp]
3000771e:	4613      	mov	r3, r2
30007720:	19d0      	adds	r0, r2, r7
30007722:	f8d6 14d0 	ldr.w	r1, [r6, #1232]	; 0x4d0
30007726:	464a      	mov	r2, r9
30007728:	4453      	add	r3, sl
3000772a:	f7ff fb13 	bl	30006d54 <password_hash_check>
3000772e:	4680      	mov	r8, r0
30007730:	2800      	cmp	r0, #0
30007732:	d1b6      	bne.n	300076a2 <BOOT_Image1+0x802>
30007734:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007738:	47a0      	blx	r4
3000773a:	484b      	ldr	r0, [pc, #300]	; (30007868 <BOOT_Image1+0x9c8>)
3000773c:	f004 fd2c 	bl	3000c198 <____wrap_strlen_veneer>
30007740:	4601      	mov	r1, r0
30007742:	4849      	ldr	r0, [pc, #292]	; (30007868 <BOOT_Image1+0x9c8>)
30007744:	f000 fabe 	bl	30007cc4 <ymodem_uart_putdata>
30007748:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000774c:	47a0      	blx	r4
3000774e:	f8d6 24d4 	ldr.w	r2, [r6, #1236]	; 0x4d4
30007752:	4946      	ldr	r1, [pc, #280]	; (3000786c <BOOT_Image1+0x9cc>)
30007754:	2002      	movs	r0, #2
30007756:	f000 fb55 	bl	30007e04 <ymodem_img_download>
3000775a:	2032      	movs	r0, #50	; 0x32
3000775c:	47a0      	blx	r4
3000775e:	f8d6 34d4 	ldr.w	r3, [r6, #1236]	; 0x4d4
30007762:	b1e3      	cbz	r3, 3000779e <BOOT_Image1+0x8fe>
30007764:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30007848 <BOOT_Image1+0x9a8>
30007768:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
3000776c:	f859 7b04 	ldr.w	r7, [r9], #4
30007770:	0b3f      	lsrs	r7, r7, #12
30007772:	d00c      	beq.n	3000778e <BOOT_Image1+0x8ee>
30007774:	2500      	movs	r5, #0
30007776:	f8da 1000 	ldr.w	r1, [sl]
3000777a:	2002      	movs	r0, #2
3000777c:	eb01 3105 	add.w	r1, r1, r5, lsl #12
30007780:	3501      	adds	r5, #1
30007782:	f7fd fc1b 	bl	30004fbc <FLASH_Erase_With_Lock>
30007786:	2032      	movs	r0, #50	; 0x32
30007788:	47a0      	blx	r4
3000778a:	42af      	cmp	r7, r5
3000778c:	d1f3      	bne.n	30007776 <BOOT_Image1+0x8d6>
3000778e:	f108 0801 	add.w	r8, r8, #1
30007792:	f8d6 34d4 	ldr.w	r3, [r6, #1236]	; 0x4d4
30007796:	f10a 0a04 	add.w	sl, sl, #4
3000779a:	4543      	cmp	r3, r8
3000779c:	d8e6      	bhi.n	3000776c <BOOT_Image1+0x8cc>
3000779e:	4834      	ldr	r0, [pc, #208]	; (30007870 <BOOT_Image1+0x9d0>)
300077a0:	f004 fd0a 	bl	3000c1b8 <__DiagPrintf_veneer>
300077a4:	4b33      	ldr	r3, [pc, #204]	; (30007874 <BOOT_Image1+0x9d4>)
300077a6:	a808      	add	r0, sp, #32
300077a8:	4798      	blx	r3
300077aa:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
300077ae:	4832      	ldr	r0, [pc, #200]	; (30007878 <BOOT_Image1+0x9d8>)
300077b0:	a908      	add	r1, sp, #32
300077b2:	4b32      	ldr	r3, [pc, #200]	; (3000787c <BOOT_Image1+0x9dc>)
300077b4:	f8ad 2022 	strh.w	r2, [sp, #34]	; 0x22
300077b8:	4798      	blx	r3
300077ba:	482f      	ldr	r0, [pc, #188]	; (30007878 <BOOT_Image1+0x9d8>)
300077bc:	4b30      	ldr	r3, [pc, #192]	; (30007880 <BOOT_Image1+0x9e0>)
300077be:	4798      	blx	r3
300077c0:	e7fe      	b.n	300077c0 <BOOT_Image1+0x920>
300077c2:	4b30      	ldr	r3, [pc, #192]	; (30007884 <BOOT_Image1+0x9e4>)
300077c4:	4798      	blx	r3
300077c6:	2801      	cmp	r0, #1
300077c8:	bf0c      	ite	eq
300077ca:	2019      	moveq	r0, #25
300077cc:	2004      	movne	r0, #4
300077ce:	47a0      	blx	r4
300077d0:	e5f0      	b.n	300073b4 <BOOT_Image1+0x514>
300077d2:	4b2c      	ldr	r3, [pc, #176]	; (30007884 <BOOT_Image1+0x9e4>)
300077d4:	4798      	blx	r3
300077d6:	2801      	cmp	r0, #1
300077d8:	bf0c      	ite	eq
300077da:	20e6      	moveq	r0, #230	; 0xe6
300077dc:	2019      	movne	r0, #25
300077de:	47a0      	blx	r4
300077e0:	e5fb      	b.n	300073da <BOOT_Image1+0x53a>
300077e2:	4829      	ldr	r0, [pc, #164]	; (30007888 <BOOT_Image1+0x9e8>)
300077e4:	f004 fce8 	bl	3000c1b8 <__DiagPrintf_veneer>
300077e8:	e5cb      	b.n	30007382 <BOOT_Image1+0x4e2>
300077ea:	4828      	ldr	r0, [pc, #160]	; (3000788c <BOOT_Image1+0x9ec>)
300077ec:	f004 fce4 	bl	3000c1b8 <__DiagPrintf_veneer>
300077f0:	4817      	ldr	r0, [pc, #92]	; (30007850 <BOOT_Image1+0x9b0>)
300077f2:	f004 fce1 	bl	3000c1b8 <__DiagPrintf_veneer>
300077f6:	e5c4      	b.n	30007382 <BOOT_Image1+0x4e2>
300077f8:	4825      	ldr	r0, [pc, #148]	; (30007890 <BOOT_Image1+0x9f0>)
300077fa:	f004 fcdd 	bl	3000c1b8 <__DiagPrintf_veneer>
300077fe:	e5c0      	b.n	30007382 <BOOT_Image1+0x4e2>
30007800:	2700      	movs	r7, #0
30007802:	f04f 090a 	mov.w	r9, #10
30007806:	463b      	mov	r3, r7
30007808:	46bb      	mov	fp, r7
3000780a:	e511      	b.n	30007230 <BOOT_Image1+0x390>
3000780c:	46d3      	mov	fp, sl
3000780e:	e571      	b.n	300072f4 <BOOT_Image1+0x454>
30007810:	46da      	mov	sl, fp
30007812:	f04f 0900 	mov.w	r9, #0
30007816:	e53a      	b.n	3000728e <BOOT_Image1+0x3ee>
30007818:	f107 0a0b 	add.w	sl, r7, #11
3000781c:	f8dd 8010 	ldr.w	r8, [sp, #16]
30007820:	370a      	adds	r7, #10
30007822:	44ca      	add	sl, r9
30007824:	e775      	b.n	30007712 <BOOT_Image1+0x872>
30007826:	bf00      	nop
30007828:	3000ce94 	.word	0x3000ce94
3000782c:	0000aea1 	.word	0x0000aea1
30007830:	0000aab9 	.word	0x0000aab9
30007834:	4200c000 	.word	0x4200c000
30007838:	0000aaa5 	.word	0x0000aaa5
3000783c:	3000e24c 	.word	0x3000e24c
30007840:	3000e274 	.word	0x3000e274
30007844:	000129bd 	.word	0x000129bd
30007848:	3000e278 	.word	0x3000e278
3000784c:	3000e2a0 	.word	0x3000e2a0
30007850:	3000d02c 	.word	0x3000d02c
30007854:	41000200 	.word	0x41000200
30007858:	3000cef4 	.word	0x3000cef4
3000785c:	3000cf7c 	.word	0x3000cf7c
30007860:	3000d084 	.word	0x3000d084
30007864:	3000dd30 	.word	0x3000dd30
30007868:	3000ea58 	.word	0x3000ea58
3000786c:	3000e20c 	.word	0x3000e20c
30007870:	3000d064 	.word	0x3000d064
30007874:	0000d3f9 	.word	0x0000d3f9
30007878:	41000440 	.word	0x41000440
3000787c:	0000d409 	.word	0x0000d409
30007880:	0000d489 	.word	0x0000d489
30007884:	0000c169 	.word	0x0000c169
30007888:	3000d03c 	.word	0x3000d03c
3000788c:	3000d014 	.word	0x3000d014
30007890:	3000d050 	.word	0x3000d050
30007894:	0000c0f9 	.word	0x0000c0f9

30007898 <ymodem_inquiry>:
30007898:	2806      	cmp	r0, #6
3000789a:	b510      	push	{r4, lr}
3000789c:	4604      	mov	r4, r0
3000789e:	d003      	beq.n	300078a8 <ymodem_inquiry+0x10>
300078a0:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078a4:	4b03      	ldr	r3, [pc, #12]	; (300078b4 <ymodem_inquiry+0x1c>)
300078a6:	4798      	blx	r3
300078a8:	4620      	mov	r0, r4
300078aa:	f000 fa07 	bl	30007cbc <ymodem_uart_putc>
300078ae:	2001      	movs	r0, #1
300078b0:	bd10      	pop	{r4, pc}
300078b2:	bf00      	nop
300078b4:	00009b2d 	.word	0x00009b2d

300078b8 <ymodem_get_first>:
300078b8:	b530      	push	{r4, r5, lr}
300078ba:	b083      	sub	sp, #12
300078bc:	2300      	movs	r3, #0
300078be:	4604      	mov	r4, r0
300078c0:	4936      	ldr	r1, [pc, #216]	; (3000799c <ymodem_get_first+0xe4>)
300078c2:	f10d 0006 	add.w	r0, sp, #6
300078c6:	f88d 3006 	strb.w	r3, [sp, #6]
300078ca:	f000 fa19 	bl	30007d00 <ymodem_uart_getc_to>
300078ce:	b9d0      	cbnz	r0, 30007906 <ymodem_get_first+0x4e>
300078d0:	f89d 3006 	ldrb.w	r3, [sp, #6]
300078d4:	3b01      	subs	r3, #1
300078d6:	2b17      	cmp	r3, #23
300078d8:	d80d      	bhi.n	300078f6 <ymodem_get_first+0x3e>
300078da:	e8df f003 	tbb	[pc, r3]
300078de:	231e      	.short	0x231e
300078e0:	0c0c290c 	.word	0x0c0c290c
300078e4:	0c0c0c0c 	.word	0x0c0c0c0c
300078e8:	0c0c0c0c 	.word	0x0c0c0c0c
300078ec:	0c0c0c0c 	.word	0x0c0c0c0c
300078f0:	0c0c0c0c 	.word	0x0c0c0c0c
300078f4:	190c      	.short	0x190c
300078f6:	4d29      	ldr	r5, [pc, #164]	; (3000799c <ymodem_get_first+0xe4>)
300078f8:	4629      	mov	r1, r5
300078fa:	f10d 0007 	add.w	r0, sp, #7
300078fe:	f000 f9ff 	bl	30007d00 <ymodem_uart_getc_to>
30007902:	2800      	cmp	r0, #0
30007904:	d0f8      	beq.n	300078f8 <ymodem_get_first+0x40>
30007906:	2301      	movs	r3, #1
30007908:	2008      	movs	r0, #8
3000790a:	60a3      	str	r3, [r4, #8]
3000790c:	b003      	add	sp, #12
3000790e:	bd30      	pop	{r4, r5, pc}
30007910:	2301      	movs	r3, #1
30007912:	2002      	movs	r0, #2
30007914:	60a3      	str	r3, [r4, #8]
30007916:	b003      	add	sp, #12
30007918:	bd30      	pop	{r4, r5, pc}
3000791a:	2380      	movs	r3, #128	; 0x80
3000791c:	2001      	movs	r0, #1
3000791e:	80a3      	strh	r3, [r4, #4]
30007920:	b003      	add	sp, #12
30007922:	bd30      	pop	{r4, r5, pc}
30007924:	f44f 6380 	mov.w	r3, #1024	; 0x400
30007928:	2001      	movs	r0, #1
3000792a:	80a3      	strh	r3, [r4, #4]
3000792c:	b003      	add	sp, #12
3000792e:	bd30      	pop	{r4, r5, pc}
30007930:	2006      	movs	r0, #6
30007932:	4d1b      	ldr	r5, [pc, #108]	; (300079a0 <ymodem_get_first+0xe8>)
30007934:	f000 f9c2 	bl	30007cbc <ymodem_uart_putc>
30007938:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000793c:	47a8      	blx	r5
3000793e:	2043      	movs	r0, #67	; 0x43
30007940:	f000 f9bc 	bl	30007cbc <ymodem_uart_putc>
30007944:	2006      	movs	r0, #6
30007946:	f000 f9b9 	bl	30007cbc <ymodem_uart_putc>
3000794a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000794e:	47a8      	blx	r5
30007950:	2043      	movs	r0, #67	; 0x43
30007952:	f000 f9b3 	bl	30007cbc <ymodem_uart_putc>
30007956:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000795a:	47a8      	blx	r5
3000795c:	2018      	movs	r0, #24
3000795e:	f000 f9ad 	bl	30007cbc <ymodem_uart_putc>
30007962:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007966:	47a8      	blx	r5
30007968:	2018      	movs	r0, #24
3000796a:	f000 f9a7 	bl	30007cbc <ymodem_uart_putc>
3000796e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007972:	47a8      	blx	r5
30007974:	2018      	movs	r0, #24
30007976:	f000 f9a1 	bl	30007cbc <ymodem_uart_putc>
3000797a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000797e:	47a8      	blx	r5
30007980:	2018      	movs	r0, #24
30007982:	f000 f99b 	bl	30007cbc <ymodem_uart_putc>
30007986:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000798a:	47a8      	blx	r5
3000798c:	2018      	movs	r0, #24
3000798e:	f000 f995 	bl	30007cbc <ymodem_uart_putc>
30007992:	2301      	movs	r3, #1
30007994:	2005      	movs	r0, #5
30007996:	60a3      	str	r3, [r4, #8]
30007998:	b003      	add	sp, #12
3000799a:	bd30      	pop	{r4, r5, pc}
3000799c:	000caa30 	.word	0x000caa30
300079a0:	00009b2d 	.word	0x00009b2d

300079a4 <ymodem_get_others>:
300079a4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300079a8:	b083      	sub	sp, #12
300079aa:	2300      	movs	r3, #0
300079ac:	4605      	mov	r5, r0
300079ae:	4952      	ldr	r1, [pc, #328]	; (30007af8 <ymodem_get_others+0x154>)
300079b0:	a801      	add	r0, sp, #4
300079b2:	f88d 3006 	strb.w	r3, [sp, #6]
300079b6:	f88d 3007 	strb.w	r3, [sp, #7]
300079ba:	f000 f9a1 	bl	30007d00 <ymodem_uart_getc_to>
300079be:	bb38      	cbnz	r0, 30007a10 <ymodem_get_others+0x6c>
300079c0:	494d      	ldr	r1, [pc, #308]	; (30007af8 <ymodem_get_others+0x154>)
300079c2:	f10d 0005 	add.w	r0, sp, #5
300079c6:	f000 f99b 	bl	30007d00 <ymodem_uart_getc_to>
300079ca:	4607      	mov	r7, r0
300079cc:	bb00      	cbnz	r0, 30007a10 <ymodem_get_others+0x6c>
300079ce:	f89d 2004 	ldrb.w	r2, [sp, #4]
300079d2:	f89d 6005 	ldrb.w	r6, [sp, #5]
300079d6:	43d3      	mvns	r3, r2
300079d8:	b2db      	uxtb	r3, r3
300079da:	429e      	cmp	r6, r3
300079dc:	bf0c      	ite	eq
300079de:	2601      	moveq	r6, #1
300079e0:	2604      	movne	r6, #4
300079e2:	682b      	ldr	r3, [r5, #0]
300079e4:	429a      	cmp	r2, r3
300079e6:	d21a      	bcs.n	30007a1e <ymodem_get_others+0x7a>
300079e8:	2701      	movs	r7, #1
300079ea:	88ab      	ldrh	r3, [r5, #4]
300079ec:	b1d3      	cbz	r3, 30007a24 <ymodem_get_others+0x80>
300079ee:	2400      	movs	r4, #0
300079f0:	f8df 9108 	ldr.w	r9, [pc, #264]	; 30007afc <ymodem_get_others+0x158>
300079f4:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007af8 <ymodem_get_others+0x154>
300079f8:	e002      	b.n	30007a00 <ymodem_get_others+0x5c>
300079fa:	88ab      	ldrh	r3, [r5, #4]
300079fc:	42a3      	cmp	r3, r4
300079fe:	d911      	bls.n	30007a24 <ymodem_get_others+0x80>
30007a00:	eb04 0009 	add.w	r0, r4, r9
30007a04:	4641      	mov	r1, r8
30007a06:	3401      	adds	r4, #1
30007a08:	f000 f97a 	bl	30007d00 <ymodem_uart_getc_to>
30007a0c:	2800      	cmp	r0, #0
30007a0e:	d0f4      	beq.n	300079fa <ymodem_get_others+0x56>
30007a10:	2301      	movs	r3, #1
30007a12:	2608      	movs	r6, #8
30007a14:	60ab      	str	r3, [r5, #8]
30007a16:	4630      	mov	r0, r6
30007a18:	b003      	add	sp, #12
30007a1a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007a1e:	d0e4      	beq.n	300079ea <ymodem_get_others+0x46>
30007a20:	2604      	movs	r6, #4
30007a22:	e7e2      	b.n	300079ea <ymodem_get_others+0x46>
30007a24:	4934      	ldr	r1, [pc, #208]	; (30007af8 <ymodem_get_others+0x154>)
30007a26:	f10d 0006 	add.w	r0, sp, #6
30007a2a:	f000 f969 	bl	30007d00 <ymodem_uart_getc_to>
30007a2e:	2800      	cmp	r0, #0
30007a30:	d1ee      	bne.n	30007a10 <ymodem_get_others+0x6c>
30007a32:	4931      	ldr	r1, [pc, #196]	; (30007af8 <ymodem_get_others+0x154>)
30007a34:	f10d 0007 	add.w	r0, sp, #7
30007a38:	f000 f962 	bl	30007d00 <ymodem_uart_getc_to>
30007a3c:	2800      	cmp	r0, #0
30007a3e:	d1e7      	bne.n	30007a10 <ymodem_get_others+0x6c>
30007a40:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007a44:	f89d e007 	ldrb.w	lr, [sp, #7]
30007a48:	88aa      	ldrh	r2, [r5, #4]
30007a4a:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
30007a4e:	b36a      	cbz	r2, 30007aac <ymodem_get_others+0x108>
30007a50:	4603      	mov	r3, r0
30007a52:	482a      	ldr	r0, [pc, #168]	; (30007afc <ymodem_get_others+0x158>)
30007a54:	f241 0c21 	movw	ip, #4129	; 0x1021
30007a58:	eb02 0800 	add.w	r8, r2, r0
30007a5c:	7801      	ldrb	r1, [r0, #0]
30007a5e:	3001      	adds	r0, #1
30007a60:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
30007a64:	2108      	movs	r1, #8
30007a66:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
30007a6a:	005b      	lsls	r3, r3, #1
30007a6c:	b10c      	cbz	r4, 30007a72 <ymodem_get_others+0xce>
30007a6e:	ea83 030c 	eor.w	r3, r3, ip
30007a72:	3901      	subs	r1, #1
30007a74:	d1f7      	bne.n	30007a66 <ymodem_get_others+0xc2>
30007a76:	4580      	cmp	r8, r0
30007a78:	d1f0      	bne.n	30007a5c <ymodem_get_others+0xb8>
30007a7a:	b29b      	uxth	r3, r3
30007a7c:	459e      	cmp	lr, r3
30007a7e:	d00a      	beq.n	30007a96 <ymodem_get_others+0xf2>
30007a80:	4b1f      	ldr	r3, [pc, #124]	; (30007b00 <ymodem_get_others+0x15c>)
30007a82:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a86:	4798      	blx	r3
30007a88:	2015      	movs	r0, #21
30007a8a:	f000 f917 	bl	30007cbc <ymodem_uart_putc>
30007a8e:	2301      	movs	r3, #1
30007a90:	2604      	movs	r6, #4
30007a92:	60ab      	str	r3, [r5, #8]
30007a94:	e7bf      	b.n	30007a16 <ymodem_get_others+0x72>
30007a96:	b917      	cbnz	r7, 30007a9e <ymodem_get_others+0xfa>
30007a98:	4c1a      	ldr	r4, [pc, #104]	; (30007b04 <ymodem_get_others+0x160>)
30007a9a:	6823      	ldr	r3, [r4, #0]
30007a9c:	b9b3      	cbnz	r3, 30007acc <ymodem_get_others+0x128>
30007a9e:	f000 f947 	bl	30007d30 <ymodem_uart_clean_rx>
30007aa2:	2e01      	cmp	r6, #1
30007aa4:	d004      	beq.n	30007ab0 <ymodem_get_others+0x10c>
30007aa6:	2e04      	cmp	r6, #4
30007aa8:	d1b5      	bne.n	30007a16 <ymodem_get_others+0x72>
30007aaa:	e7e9      	b.n	30007a80 <ymodem_get_others+0xdc>
30007aac:	4613      	mov	r3, r2
30007aae:	e7e5      	b.n	30007a7c <ymodem_get_others+0xd8>
30007ab0:	4c14      	ldr	r4, [pc, #80]	; (30007b04 <ymodem_get_others+0x160>)
30007ab2:	2006      	movs	r0, #6
30007ab4:	60ae      	str	r6, [r5, #8]
30007ab6:	f000 f901 	bl	30007cbc <ymodem_uart_putc>
30007aba:	6823      	ldr	r3, [r4, #0]
30007abc:	b193      	cbz	r3, 30007ae4 <ymodem_get_others+0x140>
30007abe:	2f00      	cmp	r7, #0
30007ac0:	f103 0301 	add.w	r3, r3, #1
30007ac4:	bf18      	it	ne
30007ac6:	2604      	movne	r6, #4
30007ac8:	6023      	str	r3, [r4, #0]
30007aca:	e7a4      	b.n	30007a16 <ymodem_get_others+0x72>
30007acc:	68eb      	ldr	r3, [r5, #12]
30007ace:	6861      	ldr	r1, [r4, #4]
30007ad0:	3301      	adds	r3, #1
30007ad2:	480a      	ldr	r0, [pc, #40]	; (30007afc <ymodem_get_others+0x158>)
30007ad4:	60eb      	str	r3, [r5, #12]
30007ad6:	f000 f94f 	bl	30007d78 <UARTIMG_Write>
30007ada:	6862      	ldr	r2, [r4, #4]
30007adc:	88ab      	ldrh	r3, [r5, #4]
30007ade:	4413      	add	r3, r2
30007ae0:	6063      	str	r3, [r4, #4]
30007ae2:	e7dc      	b.n	30007a9e <ymodem_get_others+0xfa>
30007ae4:	4b06      	ldr	r3, [pc, #24]	; (30007b00 <ymodem_get_others+0x15c>)
30007ae6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007aea:	4798      	blx	r3
30007aec:	2043      	movs	r0, #67	; 0x43
30007aee:	f000 f8e5 	bl	30007cbc <ymodem_uart_putc>
30007af2:	6823      	ldr	r3, [r4, #0]
30007af4:	e7e3      	b.n	30007abe <ymodem_get_others+0x11a>
30007af6:	bf00      	nop
30007af8:	000caa30 	.word	0x000caa30
30007afc:	20016ffc 	.word	0x20016ffc
30007b00:	00009b2d 	.word	0x00009b2d
30007b04:	3000e2b0 	.word	0x3000e2b0

30007b08 <ymodem_rx_frame>:
30007b08:	2300      	movs	r3, #0
30007b0a:	2201      	movs	r2, #1
30007b0c:	b510      	push	{r4, lr}
30007b0e:	4604      	mov	r4, r0
30007b10:	6083      	str	r3, [r0, #8]
30007b12:	8082      	strh	r2, [r0, #4]
30007b14:	f7ff fed0 	bl	300078b8 <ymodem_get_first>
30007b18:	68a3      	ldr	r3, [r4, #8]
30007b1a:	b92b      	cbnz	r3, 30007b28 <ymodem_rx_frame+0x20>
30007b1c:	4620      	mov	r0, r4
30007b1e:	f7ff ff41 	bl	300079a4 <ymodem_get_others>
30007b22:	68a3      	ldr	r3, [r4, #8]
30007b24:	2b00      	cmp	r3, #0
30007b26:	d0f9      	beq.n	30007b1c <ymodem_rx_frame+0x14>
30007b28:	2801      	cmp	r0, #1
30007b2a:	d000      	beq.n	30007b2e <ymodem_rx_frame+0x26>
30007b2c:	bd10      	pop	{r4, pc}
30007b2e:	6823      	ldr	r3, [r4, #0]
30007b30:	3301      	adds	r3, #1
30007b32:	b2db      	uxtb	r3, r3
30007b34:	6023      	str	r3, [r4, #0]
30007b36:	bd10      	pop	{r4, pc}

30007b38 <ymodem_img_rxbuffer>:
30007b38:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007b3c:	2300      	movs	r3, #0
30007b3e:	4c28      	ldr	r4, [pc, #160]	; (30007be0 <ymodem_img_rxbuffer+0xa8>)
30007b40:	4e28      	ldr	r6, [pc, #160]	; (30007be4 <ymodem_img_rxbuffer+0xac>)
30007b42:	460d      	mov	r5, r1
30007b44:	60a3      	str	r3, [r4, #8]
30007b46:	6163      	str	r3, [r4, #20]
30007b48:	f000 f8f2 	bl	30007d30 <ymodem_uart_clean_rx>
30007b4c:	2043      	movs	r0, #67	; 0x43
30007b4e:	f000 f8b5 	bl	30007cbc <ymodem_uart_putc>
30007b52:	f24c 3050 	movw	r0, #50000	; 0xc350
30007b56:	47b0      	blx	r6
30007b58:	f000 f89a 	bl	30007c90 <ymodem_uart_readable>
30007b5c:	2800      	cmp	r0, #0
30007b5e:	d0f5      	beq.n	30007b4c <ymodem_img_rxbuffer+0x14>
30007b60:	2700      	movs	r7, #0
30007b62:	f04f 0801 	mov.w	r8, #1
30007b66:	e9c4 7500 	strd	r7, r5, [r4]
30007b6a:	4d1f      	ldr	r5, [pc, #124]	; (30007be8 <ymodem_img_rxbuffer+0xb0>)
30007b6c:	4628      	mov	r0, r5
30007b6e:	f8a4 800c 	strh.w	r8, [r4, #12]
30007b72:	6127      	str	r7, [r4, #16]
30007b74:	f7ff fea0 	bl	300078b8 <ymodem_get_first>
30007b78:	6923      	ldr	r3, [r4, #16]
30007b7a:	b92b      	cbnz	r3, 30007b88 <ymodem_img_rxbuffer+0x50>
30007b7c:	4628      	mov	r0, r5
30007b7e:	f7ff ff11 	bl	300079a4 <ymodem_get_others>
30007b82:	6923      	ldr	r3, [r4, #16]
30007b84:	2b00      	cmp	r3, #0
30007b86:	d0f9      	beq.n	30007b7c <ymodem_img_rxbuffer+0x44>
30007b88:	2801      	cmp	r0, #1
30007b8a:	d024      	beq.n	30007bd6 <ymodem_img_rxbuffer+0x9e>
30007b8c:	3804      	subs	r0, #4
30007b8e:	2804      	cmp	r0, #4
30007b90:	d80c      	bhi.n	30007bac <ymodem_img_rxbuffer+0x74>
30007b92:	a301      	add	r3, pc, #4	; (adr r3, 30007b98 <ymodem_img_rxbuffer+0x60>)
30007b94:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007b98:	30007b6d 	.word	0x30007b6d
30007b9c:	30007bb9 	.word	0x30007bb9
30007ba0:	30007bad 	.word	0x30007bad
30007ba4:	30007bad 	.word	0x30007bad
30007ba8:	30007bc5 	.word	0x30007bc5
30007bac:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007bb0:	47b0      	blx	r6
30007bb2:	2018      	movs	r0, #24
30007bb4:	f000 f882 	bl	30007cbc <ymodem_uart_putc>
30007bb8:	200a      	movs	r0, #10
30007bba:	f000 f8c9 	bl	30007d50 <ymodem_uart_waitbusy>
30007bbe:	2001      	movs	r0, #1
30007bc0:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007bc4:	f000 f8b4 	bl	30007d30 <ymodem_uart_clean_rx>
30007bc8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007bcc:	47b0      	blx	r6
30007bce:	2015      	movs	r0, #21
30007bd0:	f000 f874 	bl	30007cbc <ymodem_uart_putc>
30007bd4:	e7ca      	b.n	30007b6c <ymodem_img_rxbuffer+0x34>
30007bd6:	68a3      	ldr	r3, [r4, #8]
30007bd8:	3301      	adds	r3, #1
30007bda:	b2db      	uxtb	r3, r3
30007bdc:	60a3      	str	r3, [r4, #8]
30007bde:	e7c5      	b.n	30007b6c <ymodem_img_rxbuffer+0x34>
30007be0:	3000e2b0 	.word	0x3000e2b0
30007be4:	00009b2d 	.word	0x00009b2d
30007be8:	3000e2b8 	.word	0x3000e2b8

30007bec <ymodem_log_open>:
30007bec:	b510      	push	{r4, lr}
30007bee:	2303      	movs	r3, #3
30007bf0:	4c04      	ldr	r4, [pc, #16]	; (30007c04 <ymodem_log_open+0x18>)
30007bf2:	2200      	movs	r2, #0
30007bf4:	2108      	movs	r1, #8
30007bf6:	4804      	ldr	r0, [pc, #16]	; (30007c08 <ymodem_log_open+0x1c>)
30007bf8:	47a0      	blx	r4
30007bfa:	2008      	movs	r0, #8
30007bfc:	4b03      	ldr	r3, [pc, #12]	; (30007c0c <ymodem_log_open+0x20>)
30007bfe:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007c02:	4718      	bx	r3
30007c04:	3000a489 	.word	0x3000a489
30007c08:	0000ed39 	.word	0x0000ed39
30007c0c:	0000e1bd 	.word	0x0000e1bd

30007c10 <ymodem_uart_port_init>:
30007c10:	b500      	push	{lr}
30007c12:	b083      	sub	sp, #12
30007c14:	2008      	movs	r0, #8
30007c16:	4b06      	ldr	r3, [pc, #24]	; (30007c30 <ymodem_uart_port_init+0x20>)
30007c18:	9201      	str	r2, [sp, #4]
30007c1a:	4798      	blx	r3
30007c1c:	4b05      	ldr	r3, [pc, #20]	; (30007c34 <ymodem_uart_port_init+0x24>)
30007c1e:	2008      	movs	r0, #8
30007c20:	4798      	blx	r3
30007c22:	9901      	ldr	r1, [sp, #4]
30007c24:	4804      	ldr	r0, [pc, #16]	; (30007c38 <ymodem_uart_port_init+0x28>)
30007c26:	4b05      	ldr	r3, [pc, #20]	; (30007c3c <ymodem_uart_port_init+0x2c>)
30007c28:	b003      	add	sp, #12
30007c2a:	f85d eb04 	ldr.w	lr, [sp], #4
30007c2e:	4718      	bx	r3
30007c30:	0000e1d9 	.word	0x0000e1d9
30007c34:	0000e2fd 	.word	0x0000e2fd
30007c38:	4200c000 	.word	0x4200c000
30007c3c:	0000a9e5 	.word	0x0000a9e5

30007c40 <ymodem_uart_port_deinit>:
30007c40:	2804      	cmp	r0, #4
30007c42:	d004      	beq.n	30007c4e <ymodem_uart_port_deinit+0xe>
30007c44:	4b0d      	ldr	r3, [pc, #52]	; (30007c7c <ymodem_uart_port_deinit+0x3c>)
30007c46:	681b      	ldr	r3, [r3, #0]
30007c48:	039b      	lsls	r3, r3, #14
30007c4a:	d40f      	bmi.n	30007c6c <ymodem_uart_port_deinit+0x2c>
30007c4c:	4770      	bx	lr
30007c4e:	2200      	movs	r2, #0
30007c50:	2303      	movs	r3, #3
30007c52:	2108      	movs	r1, #8
30007c54:	480a      	ldr	r0, [pc, #40]	; (30007c80 <ymodem_uart_port_deinit+0x40>)
30007c56:	b510      	push	{r4, lr}
30007c58:	4c0a      	ldr	r4, [pc, #40]	; (30007c84 <ymodem_uart_port_deinit+0x44>)
30007c5a:	47a0      	blx	r4
30007c5c:	4b0a      	ldr	r3, [pc, #40]	; (30007c88 <ymodem_uart_port_deinit+0x48>)
30007c5e:	2008      	movs	r0, #8
30007c60:	4798      	blx	r3
30007c62:	4b06      	ldr	r3, [pc, #24]	; (30007c7c <ymodem_uart_port_deinit+0x3c>)
30007c64:	681b      	ldr	r3, [r3, #0]
30007c66:	039a      	lsls	r2, r3, #14
30007c68:	d403      	bmi.n	30007c72 <ymodem_uart_port_deinit+0x32>
30007c6a:	bd10      	pop	{r4, pc}
30007c6c:	4807      	ldr	r0, [pc, #28]	; (30007c8c <ymodem_uart_port_deinit+0x4c>)
30007c6e:	f004 baa3 	b.w	3000c1b8 <__DiagPrintf_veneer>
30007c72:	4806      	ldr	r0, [pc, #24]	; (30007c8c <ymodem_uart_port_deinit+0x4c>)
30007c74:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007c78:	f004 ba9e 	b.w	3000c1b8 <__DiagPrintf_veneer>
30007c7c:	2001c00c 	.word	0x2001c00c
30007c80:	0000ed39 	.word	0x0000ed39
30007c84:	3000a489 	.word	0x3000a489
30007c88:	0000e1bd 	.word	0x0000e1bd
30007c8c:	3000d0bc 	.word	0x3000d0bc

30007c90 <ymodem_uart_readable>:
30007c90:	4b00      	ldr	r3, [pc, #0]	; (30007c94 <ymodem_uart_readable+0x4>)
30007c92:	4718      	bx	r3
30007c94:	0000ac39 	.word	0x0000ac39

30007c98 <ymodem_uart_writable>:
30007c98:	4a03      	ldr	r2, [pc, #12]	; (30007ca8 <ymodem_uart_writable+0x10>)
30007c9a:	b508      	push	{r3, lr}
30007c9c:	4b03      	ldr	r3, [pc, #12]	; (30007cac <ymodem_uart_writable+0x14>)
30007c9e:	6810      	ldr	r0, [r2, #0]
30007ca0:	4798      	blx	r3
30007ca2:	b2c0      	uxtb	r0, r0
30007ca4:	bd08      	pop	{r3, pc}
30007ca6:	bf00      	nop
30007ca8:	3000e2c8 	.word	0x3000e2c8
30007cac:	0000d1a9 	.word	0x0000d1a9

30007cb0 <ymodem_uart_getc>:
30007cb0:	2000      	movs	r0, #0
30007cb2:	4b01      	ldr	r3, [pc, #4]	; (30007cb8 <ymodem_uart_getc+0x8>)
30007cb4:	4718      	bx	r3
30007cb6:	bf00      	nop
30007cb8:	0000ac1d 	.word	0x0000ac1d

30007cbc <ymodem_uart_putc>:
30007cbc:	4b00      	ldr	r3, [pc, #0]	; (30007cc0 <ymodem_uart_putc+0x4>)
30007cbe:	4718      	bx	r3
30007cc0:	0000abb5 	.word	0x0000abb5

30007cc4 <ymodem_uart_putdata>:
30007cc4:	b149      	cbz	r1, 30007cda <ymodem_uart_putdata+0x16>
30007cc6:	b570      	push	{r4, r5, r6, lr}
30007cc8:	4604      	mov	r4, r0
30007cca:	1845      	adds	r5, r0, r1
30007ccc:	4e03      	ldr	r6, [pc, #12]	; (30007cdc <ymodem_uart_putdata+0x18>)
30007cce:	f814 0b01 	ldrb.w	r0, [r4], #1
30007cd2:	47b0      	blx	r6
30007cd4:	42ac      	cmp	r4, r5
30007cd6:	d1fa      	bne.n	30007cce <ymodem_uart_putdata+0xa>
30007cd8:	bd70      	pop	{r4, r5, r6, pc}
30007cda:	4770      	bx	lr
30007cdc:	0000abb5 	.word	0x0000abb5

30007ce0 <ymodem_uart_getdata>:
30007ce0:	b151      	cbz	r1, 30007cf8 <ymodem_uart_getdata+0x18>
30007ce2:	b570      	push	{r4, r5, r6, lr}
30007ce4:	4604      	mov	r4, r0
30007ce6:	1845      	adds	r5, r0, r1
30007ce8:	4e04      	ldr	r6, [pc, #16]	; (30007cfc <ymodem_uart_getdata+0x1c>)
30007cea:	2001      	movs	r0, #1
30007cec:	47b0      	blx	r6
30007cee:	f804 0b01 	strb.w	r0, [r4], #1
30007cf2:	42ac      	cmp	r4, r5
30007cf4:	d1f9      	bne.n	30007cea <ymodem_uart_getdata+0xa>
30007cf6:	bd70      	pop	{r4, r5, r6, pc}
30007cf8:	4770      	bx	lr
30007cfa:	bf00      	nop
30007cfc:	0000ac1d 	.word	0x0000ac1d

30007d00 <ymodem_uart_getc_to>:
30007d00:	b570      	push	{r4, r5, r6, lr}
30007d02:	4d09      	ldr	r5, [pc, #36]	; (30007d28 <ymodem_uart_getc_to+0x28>)
30007d04:	4606      	mov	r6, r0
30007d06:	460c      	mov	r4, r1
30007d08:	e001      	b.n	30007d0e <ymodem_uart_getc_to+0xe>
30007d0a:	3c01      	subs	r4, #1
30007d0c:	d009      	beq.n	30007d22 <ymodem_uart_getc_to+0x22>
30007d0e:	47a8      	blx	r5
30007d10:	2800      	cmp	r0, #0
30007d12:	d0fa      	beq.n	30007d0a <ymodem_uart_getc_to+0xa>
30007d14:	4b05      	ldr	r3, [pc, #20]	; (30007d2c <ymodem_uart_getc_to+0x2c>)
30007d16:	2000      	movs	r0, #0
30007d18:	4798      	blx	r3
30007d1a:	4603      	mov	r3, r0
30007d1c:	2000      	movs	r0, #0
30007d1e:	7033      	strb	r3, [r6, #0]
30007d20:	bd70      	pop	{r4, r5, r6, pc}
30007d22:	2001      	movs	r0, #1
30007d24:	bd70      	pop	{r4, r5, r6, pc}
30007d26:	bf00      	nop
30007d28:	0000ac39 	.word	0x0000ac39
30007d2c:	0000ac1d 	.word	0x0000ac1d

30007d30 <ymodem_uart_clean_rx>:
30007d30:	b538      	push	{r3, r4, r5, lr}
30007d32:	4c05      	ldr	r4, [pc, #20]	; (30007d48 <ymodem_uart_clean_rx+0x18>)
30007d34:	4d05      	ldr	r5, [pc, #20]	; (30007d4c <ymodem_uart_clean_rx+0x1c>)
30007d36:	e000      	b.n	30007d3a <ymodem_uart_clean_rx+0xa>
30007d38:	47a8      	blx	r5
30007d3a:	47a0      	blx	r4
30007d3c:	4603      	mov	r3, r0
30007d3e:	2000      	movs	r0, #0
30007d40:	2b00      	cmp	r3, #0
30007d42:	d1f9      	bne.n	30007d38 <ymodem_uart_clean_rx+0x8>
30007d44:	bd38      	pop	{r3, r4, r5, pc}
30007d46:	bf00      	nop
30007d48:	0000ac39 	.word	0x0000ac39
30007d4c:	0000ac1d 	.word	0x0000ac1d

30007d50 <ymodem_uart_waitbusy>:
30007d50:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007d52:	2400      	movs	r4, #0
30007d54:	4e06      	ldr	r6, [pc, #24]	; (30007d70 <ymodem_uart_waitbusy+0x20>)
30007d56:	4f07      	ldr	r7, [pc, #28]	; (30007d74 <ymodem_uart_waitbusy+0x24>)
30007d58:	4605      	mov	r5, r0
30007d5a:	e002      	b.n	30007d62 <ymodem_uart_waitbusy+0x12>
30007d5c:	47b8      	blx	r7
30007d5e:	42ac      	cmp	r4, r5
30007d60:	d804      	bhi.n	30007d6c <ymodem_uart_waitbusy+0x1c>
30007d62:	6973      	ldr	r3, [r6, #20]
30007d64:	3401      	adds	r4, #1
30007d66:	2064      	movs	r0, #100	; 0x64
30007d68:	03db      	lsls	r3, r3, #15
30007d6a:	d5f7      	bpl.n	30007d5c <ymodem_uart_waitbusy+0xc>
30007d6c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007d6e:	bf00      	nop
30007d70:	4200c000 	.word	0x4200c000
30007d74:	00009b2d 	.word	0x00009b2d

30007d78 <UARTIMG_Write>:
30007d78:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007d7c:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007d80:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007d84:	d21d      	bcs.n	30007dc2 <UARTIMG_Write+0x4a>
30007d86:	3a01      	subs	r2, #1
30007d88:	4f19      	ldr	r7, [pc, #100]	; (30007df0 <UARTIMG_Write+0x78>)
30007d8a:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007e00 <UARTIMG_Write+0x88>
30007d8e:	4688      	mov	r8, r1
30007d90:	1916      	adds	r6, r2, r4
30007d92:	4605      	mov	r5, r0
30007d94:	f8d9 3000 	ldr.w	r3, [r9]
30007d98:	403e      	ands	r6, r7
30007d9a:	4027      	ands	r7, r4
30007d9c:	42bb      	cmp	r3, r7
30007d9e:	d91b      	bls.n	30007dd8 <UARTIMG_Write+0x60>
30007da0:	42b3      	cmp	r3, r6
30007da2:	d910      	bls.n	30007dc6 <UARTIMG_Write+0x4e>
30007da4:	4e13      	ldr	r6, [pc, #76]	; (30007df4 <UARTIMG_Write+0x7c>)
30007da6:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007daa:	4f13      	ldr	r7, [pc, #76]	; (30007df8 <UARTIMG_Write+0x80>)
30007dac:	4446      	add	r6, r8
30007dae:	eba3 0808 	sub.w	r8, r3, r8
30007db2:	eb08 0204 	add.w	r2, r8, r4
30007db6:	4620      	mov	r0, r4
30007db8:	3408      	adds	r4, #8
30007dba:	2108      	movs	r1, #8
30007dbc:	47b8      	blx	r7
30007dbe:	42b4      	cmp	r4, r6
30007dc0:	d1f7      	bne.n	30007db2 <UARTIMG_Write+0x3a>
30007dc2:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007dc6:	4631      	mov	r1, r6
30007dc8:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007dcc:	2002      	movs	r0, #2
30007dce:	4b0b      	ldr	r3, [pc, #44]	; (30007dfc <UARTIMG_Write+0x84>)
30007dd0:	4798      	blx	r3
30007dd2:	f8c9 6000 	str.w	r6, [r9]
30007dd6:	e7e5      	b.n	30007da4 <UARTIMG_Write+0x2c>
30007dd8:	4b08      	ldr	r3, [pc, #32]	; (30007dfc <UARTIMG_Write+0x84>)
30007dda:	4639      	mov	r1, r7
30007ddc:	2002      	movs	r0, #2
30007dde:	4798      	blx	r3
30007de0:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007de4:	42b3      	cmp	r3, r6
30007de6:	f8c9 3000 	str.w	r3, [r9]
30007dea:	d8db      	bhi.n	30007da4 <UARTIMG_Write+0x2c>
30007dec:	e7eb      	b.n	30007dc6 <UARTIMG_Write+0x4e>
30007dee:	bf00      	nop
30007df0:	fffff000 	.word	0xfffff000
30007df4:	f8000400 	.word	0xf8000400
30007df8:	0000a04d 	.word	0x0000a04d
30007dfc:	0000a16d 	.word	0x0000a16d
30007e00:	3000e2cc 	.word	0x3000e2cc

30007e04 <ymodem_img_download>:
30007e04:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007e08:	4616      	mov	r6, r2
30007e0a:	460d      	mov	r5, r1
30007e0c:	4a0a      	ldr	r2, [pc, #40]	; (30007e38 <ymodem_img_download+0x34>)
30007e0e:	2100      	movs	r1, #0
30007e10:	4680      	mov	r8, r0
30007e12:	f7ff fefd 	bl	30007c10 <ymodem_uart_port_init>
30007e16:	b156      	cbz	r6, 30007e2e <ymodem_img_download+0x2a>
30007e18:	3d04      	subs	r5, #4
30007e1a:	2400      	movs	r4, #0
30007e1c:	4f06      	ldr	r7, [pc, #24]	; (30007e38 <ymodem_img_download+0x34>)
30007e1e:	3401      	adds	r4, #1
30007e20:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007e24:	4638      	mov	r0, r7
30007e26:	f7ff fe87 	bl	30007b38 <ymodem_img_rxbuffer>
30007e2a:	42b4      	cmp	r4, r6
30007e2c:	d1f7      	bne.n	30007e1e <ymodem_img_download+0x1a>
30007e2e:	4640      	mov	r0, r8
30007e30:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007e34:	f7ff bf04 	b.w	30007c40 <ymodem_uart_port_deinit>
30007e38:	0016e360 	.word	0x0016e360

30007e3c <ChipInfo_Invalid>:
30007e3c:	4d07      	ldr	r5, [pc, #28]	; (30007e5c <ChipInfo_Invalid+0x20>)
30007e3e:	4c08      	ldr	r4, [pc, #32]	; (30007e60 <ChipInfo_Invalid+0x24>)
30007e40:	4e08      	ldr	r6, [pc, #32]	; (30007e64 <ChipInfo_Invalid+0x28>)
30007e42:	b508      	push	{r3, lr}
30007e44:	e002      	b.n	30007e4c <ChipInfo_Invalid+0x10>
30007e46:	f242 7010 	movw	r0, #10000	; 0x2710
30007e4a:	47a0      	blx	r4
30007e4c:	682b      	ldr	r3, [r5, #0]
30007e4e:	039b      	lsls	r3, r3, #14
30007e50:	d5f9      	bpl.n	30007e46 <ChipInfo_Invalid+0xa>
30007e52:	4630      	mov	r0, r6
30007e54:	f004 f9b0 	bl	3000c1b8 <__DiagPrintf_veneer>
30007e58:	e7f5      	b.n	30007e46 <ChipInfo_Invalid+0xa>
30007e5a:	bf00      	nop
30007e5c:	2001c00c 	.word	0x2001c00c
30007e60:	00009be5 	.word	0x00009be5
30007e64:	3000d0dc 	.word	0x3000d0dc

30007e68 <ChipInfo_Get>:
30007e68:	b570      	push	{r4, r5, r6, lr}
30007e6a:	4c11      	ldr	r4, [pc, #68]	; (30007eb0 <ChipInfo_Get+0x48>)
30007e6c:	7820      	ldrb	r0, [r4, #0]
30007e6e:	28ff      	cmp	r0, #255	; 0xff
30007e70:	d104      	bne.n	30007e7c <ChipInfo_Get+0x14>
30007e72:	4d10      	ldr	r5, [pc, #64]	; (30007eb4 <ChipInfo_Get+0x4c>)
30007e74:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007e78:	b108      	cbz	r0, 30007e7e <ChipInfo_Get+0x16>
30007e7a:	7020      	strb	r0, [r4, #0]
30007e7c:	bd70      	pop	{r4, r5, r6, pc}
30007e7e:	4621      	mov	r1, r4
30007e80:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007e84:	f001 f9c0 	bl	30009208 <OTP_Read8>
30007e88:	7820      	ldrb	r0, [r4, #0]
30007e8a:	28ff      	cmp	r0, #255	; 0xff
30007e8c:	d002      	beq.n	30007e94 <ChipInfo_Get+0x2c>
30007e8e:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007e92:	bd70      	pop	{r4, r5, r6, pc}
30007e94:	4d08      	ldr	r5, [pc, #32]	; (30007eb8 <ChipInfo_Get+0x50>)
30007e96:	4c09      	ldr	r4, [pc, #36]	; (30007ebc <ChipInfo_Get+0x54>)
30007e98:	4e09      	ldr	r6, [pc, #36]	; (30007ec0 <ChipInfo_Get+0x58>)
30007e9a:	e002      	b.n	30007ea2 <ChipInfo_Get+0x3a>
30007e9c:	f242 7010 	movw	r0, #10000	; 0x2710
30007ea0:	47a0      	blx	r4
30007ea2:	682b      	ldr	r3, [r5, #0]
30007ea4:	039b      	lsls	r3, r3, #14
30007ea6:	d5f9      	bpl.n	30007e9c <ChipInfo_Get+0x34>
30007ea8:	4630      	mov	r0, r6
30007eaa:	f004 f985 	bl	3000c1b8 <__DiagPrintf_veneer>
30007eae:	e7f5      	b.n	30007e9c <ChipInfo_Get+0x34>
30007eb0:	3000e3c8 	.word	0x3000e3c8
30007eb4:	23020000 	.word	0x23020000
30007eb8:	2001c00c 	.word	0x2001c00c
30007ebc:	00009be5 	.word	0x00009be5
30007ec0:	3000d0fc 	.word	0x3000d0fc

30007ec4 <ChipInfo_MemoryType>:
30007ec4:	b510      	push	{r4, lr}
30007ec6:	4c11      	ldr	r4, [pc, #68]	; (30007f0c <ChipInfo_MemoryType+0x48>)
30007ec8:	47a0      	blx	r4
30007eca:	2802      	cmp	r0, #2
30007ecc:	d019      	beq.n	30007f02 <ChipInfo_MemoryType+0x3e>
30007ece:	47a0      	blx	r4
30007ed0:	2803      	cmp	r0, #3
30007ed2:	d016      	beq.n	30007f02 <ChipInfo_MemoryType+0x3e>
30007ed4:	4c0e      	ldr	r4, [pc, #56]	; (30007f10 <ChipInfo_MemoryType+0x4c>)
30007ed6:	4b0f      	ldr	r3, [pc, #60]	; (30007f14 <ChipInfo_MemoryType+0x50>)
30007ed8:	4798      	blx	r3
30007eda:	2100      	movs	r1, #0
30007edc:	23e0      	movs	r3, #224	; 0xe0
30007ede:	4622      	mov	r2, r4
30007ee0:	e004      	b.n	30007eec <ChipInfo_MemoryType+0x28>
30007ee2:	7a13      	ldrb	r3, [r2, #8]
30007ee4:	3101      	adds	r1, #1
30007ee6:	3206      	adds	r2, #6
30007ee8:	2bff      	cmp	r3, #255	; 0xff
30007eea:	d00c      	beq.n	30007f06 <ChipInfo_MemoryType+0x42>
30007eec:	4298      	cmp	r0, r3
30007eee:	d1f8      	bne.n	30007ee2 <ChipInfo_MemoryType+0x1e>
30007ef0:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007ef4:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007ef8:	78cb      	ldrb	r3, [r1, #3]
30007efa:	2b02      	cmp	r3, #2
30007efc:	d001      	beq.n	30007f02 <ChipInfo_MemoryType+0x3e>
30007efe:	2001      	movs	r0, #1
30007f00:	bd10      	pop	{r4, pc}
30007f02:	2002      	movs	r0, #2
30007f04:	bd10      	pop	{r4, pc}
30007f06:	f7ff ff99 	bl	30007e3c <ChipInfo_Invalid>
30007f0a:	bf00      	nop
30007f0c:	0000c0f9 	.word	0x0000c0f9
30007f10:	3000d120 	.word	0x3000d120
30007f14:	30007e69 	.word	0x30007e69

30007f18 <ChipInfo_DDRType>:
30007f18:	b510      	push	{r4, lr}
30007f1a:	4b0f      	ldr	r3, [pc, #60]	; (30007f58 <ChipInfo_DDRType+0x40>)
30007f1c:	4798      	blx	r3
30007f1e:	4b0f      	ldr	r3, [pc, #60]	; (30007f5c <ChipInfo_DDRType+0x44>)
30007f20:	4604      	mov	r4, r0
30007f22:	4798      	blx	r3
30007f24:	2801      	cmp	r0, #1
30007f26:	d112      	bne.n	30007f4e <ChipInfo_DDRType+0x36>
30007f28:	480d      	ldr	r0, [pc, #52]	; (30007f60 <ChipInfo_DDRType+0x48>)
30007f2a:	2100      	movs	r1, #0
30007f2c:	22e0      	movs	r2, #224	; 0xe0
30007f2e:	4603      	mov	r3, r0
30007f30:	e003      	b.n	30007f3a <ChipInfo_DDRType+0x22>
30007f32:	789a      	ldrb	r2, [r3, #2]
30007f34:	3101      	adds	r1, #1
30007f36:	2aff      	cmp	r2, #255	; 0xff
30007f38:	d00b      	beq.n	30007f52 <ChipInfo_DDRType+0x3a>
30007f3a:	4294      	cmp	r4, r2
30007f3c:	f103 0306 	add.w	r3, r3, #6
30007f40:	d1f7      	bne.n	30007f32 <ChipInfo_DDRType+0x1a>
30007f42:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007f46:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007f4a:	78c8      	ldrb	r0, [r1, #3]
30007f4c:	bd10      	pop	{r4, pc}
30007f4e:	20ff      	movs	r0, #255	; 0xff
30007f50:	bd10      	pop	{r4, pc}
30007f52:	2000      	movs	r0, #0
30007f54:	bd10      	pop	{r4, pc}
30007f56:	bf00      	nop
30007f58:	30007e69 	.word	0x30007e69
30007f5c:	30007ec5 	.word	0x30007ec5
30007f60:	3000d120 	.word	0x3000d120

30007f64 <ChipInfo_MemorySize>:
30007f64:	b510      	push	{r4, lr}
30007f66:	4c0b      	ldr	r4, [pc, #44]	; (30007f94 <ChipInfo_MemorySize+0x30>)
30007f68:	4b0b      	ldr	r3, [pc, #44]	; (30007f98 <ChipInfo_MemorySize+0x34>)
30007f6a:	4798      	blx	r3
30007f6c:	2100      	movs	r1, #0
30007f6e:	23e0      	movs	r3, #224	; 0xe0
30007f70:	4622      	mov	r2, r4
30007f72:	e004      	b.n	30007f7e <ChipInfo_MemorySize+0x1a>
30007f74:	7a13      	ldrb	r3, [r2, #8]
30007f76:	3101      	adds	r1, #1
30007f78:	3206      	adds	r2, #6
30007f7a:	2bff      	cmp	r3, #255	; 0xff
30007f7c:	d007      	beq.n	30007f8e <ChipInfo_MemorySize+0x2a>
30007f7e:	4298      	cmp	r0, r3
30007f80:	d1f8      	bne.n	30007f74 <ChipInfo_MemorySize+0x10>
30007f82:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007f86:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007f8a:	7908      	ldrb	r0, [r1, #4]
30007f8c:	bd10      	pop	{r4, pc}
30007f8e:	f7ff ff55 	bl	30007e3c <ChipInfo_Invalid>
30007f92:	bf00      	nop
30007f94:	3000d120 	.word	0x3000d120
30007f98:	30007e69 	.word	0x30007e69

30007f9c <ChipInfo_MemoryVendor>:
30007f9c:	b510      	push	{r4, lr}
30007f9e:	4c0f      	ldr	r4, [pc, #60]	; (30007fdc <ChipInfo_MemoryVendor+0x40>)
30007fa0:	47a0      	blx	r4
30007fa2:	2803      	cmp	r0, #3
30007fa4:	d016      	beq.n	30007fd4 <ChipInfo_MemoryVendor+0x38>
30007fa6:	47a0      	blx	r4
30007fa8:	2802      	cmp	r0, #2
30007faa:	d012      	beq.n	30007fd2 <ChipInfo_MemoryVendor+0x36>
30007fac:	4c0c      	ldr	r4, [pc, #48]	; (30007fe0 <ChipInfo_MemoryVendor+0x44>)
30007fae:	4b0d      	ldr	r3, [pc, #52]	; (30007fe4 <ChipInfo_MemoryVendor+0x48>)
30007fb0:	4798      	blx	r3
30007fb2:	2100      	movs	r1, #0
30007fb4:	23e0      	movs	r3, #224	; 0xe0
30007fb6:	4622      	mov	r2, r4
30007fb8:	e004      	b.n	30007fc4 <ChipInfo_MemoryVendor+0x28>
30007fba:	7a13      	ldrb	r3, [r2, #8]
30007fbc:	3101      	adds	r1, #1
30007fbe:	3206      	adds	r2, #6
30007fc0:	2bff      	cmp	r3, #255	; 0xff
30007fc2:	d009      	beq.n	30007fd8 <ChipInfo_MemoryVendor+0x3c>
30007fc4:	4298      	cmp	r0, r3
30007fc6:	d1f8      	bne.n	30007fba <ChipInfo_MemoryVendor+0x1e>
30007fc8:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007fcc:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007fd0:	7948      	ldrb	r0, [r1, #5]
30007fd2:	bd10      	pop	{r4, pc}
30007fd4:	2001      	movs	r0, #1
30007fd6:	bd10      	pop	{r4, pc}
30007fd8:	f7ff ff30 	bl	30007e3c <ChipInfo_Invalid>
30007fdc:	0000c0f9 	.word	0x0000c0f9
30007fe0:	3000d120 	.word	0x3000d120
30007fe4:	30007e69 	.word	0x30007e69

30007fe8 <SDM32K_Enable>:
30007fe8:	4a02      	ldr	r2, [pc, #8]	; (30007ff4 <SDM32K_Enable+0xc>)
30007fea:	4b03      	ldr	r3, [pc, #12]	; (30007ff8 <SDM32K_Enable+0x10>)
30007fec:	6811      	ldr	r1, [r2, #0]
30007fee:	430b      	orrs	r3, r1
30007ff0:	6013      	str	r3, [r2, #0]
30007ff2:	4770      	bx	lr
30007ff4:	42008e00 	.word	0x42008e00
30007ff8:	c0060000 	.word	0xc0060000

30007ffc <CLK_SWITCH_XTAL>:
30007ffc:	b508      	push	{r3, lr}
30007ffe:	2801      	cmp	r0, #1
30008000:	4b0a      	ldr	r3, [pc, #40]	; (3000802c <CLK_SWITCH_XTAL+0x30>)
30008002:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30008006:	d00d      	beq.n	30008024 <CLK_SWITCH_XTAL+0x28>
30008008:	4b09      	ldr	r3, [pc, #36]	; (30008030 <CLK_SWITCH_XTAL+0x34>)
3000800a:	4313      	orrs	r3, r2
3000800c:	4907      	ldr	r1, [pc, #28]	; (3000802c <CLK_SWITCH_XTAL+0x30>)
3000800e:	4a09      	ldr	r2, [pc, #36]	; (30008034 <CLK_SWITCH_XTAL+0x38>)
30008010:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30008014:	4790      	blx	r2
30008016:	4a08      	ldr	r2, [pc, #32]	; (30008038 <CLK_SWITCH_XTAL+0x3c>)
30008018:	4b08      	ldr	r3, [pc, #32]	; (3000803c <CLK_SWITCH_XTAL+0x40>)
3000801a:	fba2 2000 	umull	r2, r0, r2, r0
3000801e:	0c80      	lsrs	r0, r0, #18
30008020:	6018      	str	r0, [r3, #0]
30008022:	bd08      	pop	{r3, pc}
30008024:	4b06      	ldr	r3, [pc, #24]	; (30008040 <CLK_SWITCH_XTAL+0x44>)
30008026:	4013      	ands	r3, r2
30008028:	e7f0      	b.n	3000800c <CLK_SWITCH_XTAL+0x10>
3000802a:	bf00      	nop
3000802c:	42008000 	.word	0x42008000
30008030:	80000500 	.word	0x80000500
30008034:	3000a159 	.word	0x3000a159
30008038:	431bde83 	.word	0x431bde83
3000803c:	2001c700 	.word	0x2001c700
30008040:	7ffffaff 	.word	0x7ffffaff

30008044 <DDR_PHY_ChipInfo_ddrtype>:
30008044:	b510      	push	{r4, lr}
30008046:	4c04      	ldr	r4, [pc, #16]	; (30008058 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30008048:	7820      	ldrb	r0, [r4, #0]
3000804a:	b100      	cbz	r0, 3000804e <DDR_PHY_ChipInfo_ddrtype+0xa>
3000804c:	bd10      	pop	{r4, pc}
3000804e:	4b03      	ldr	r3, [pc, #12]	; (3000805c <DDR_PHY_ChipInfo_ddrtype+0x18>)
30008050:	4798      	blx	r3
30008052:	7020      	strb	r0, [r4, #0]
30008054:	bd10      	pop	{r4, pc}
30008056:	bf00      	nop
30008058:	3000ea71 	.word	0x3000ea71
3000805c:	30007f19 	.word	0x30007f19

30008060 <DDR_PHY_StructInit>:
30008060:	f240 2315 	movw	r3, #533	; 0x215
30008064:	6583      	str	r3, [r0, #88]	; 0x58
30008066:	4770      	bx	lr

30008068 <DDR_PHY_DLL_CLK_DIV>:
30008068:	b570      	push	{r4, r5, r6, lr}
3000806a:	4d96      	ldr	r5, [pc, #600]	; (300082c4 <DDR_PHY_DLL_CLK_DIV+0x25c>)
3000806c:	4b96      	ldr	r3, [pc, #600]	; (300082c8 <DDR_PHY_DLL_CLK_DIV+0x260>)
3000806e:	fba5 4200 	umull	r4, r2, r5, r0
30008072:	4996      	ldr	r1, [pc, #600]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
30008074:	4403      	add	r3, r0
30008076:	4c96      	ldr	r4, [pc, #600]	; (300082d0 <DDR_PHY_DLL_CLK_DIV+0x268>)
30008078:	0952      	lsrs	r2, r2, #5
3000807a:	6a4e      	ldr	r6, [r1, #36]	; 0x24
3000807c:	4895      	ldr	r0, [pc, #596]	; (300082d4 <DDR_PHY_DLL_CLK_DIV+0x26c>)
3000807e:	3a03      	subs	r2, #3
30008080:	4034      	ands	r4, r6
30008082:	eb02 0c82 	add.w	ip, r2, r2, lsl #2
30008086:	eba3 03cc 	sub.w	r3, r3, ip, lsl #3
3000808a:	02db      	lsls	r3, r3, #11
3000808c:	fba5 5303 	umull	r5, r3, r5, r3
30008090:	2501      	movs	r5, #1
30008092:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
30008096:	f3c3 134a 	ubfx	r3, r3, #5, #11
3000809a:	4320      	orrs	r0, r4
3000809c:	0214      	lsls	r4, r2, #8
3000809e:	b2d2      	uxtb	r2, r2
300080a0:	6248      	str	r0, [r1, #36]	; 0x24
300080a2:	b2a4      	uxth	r4, r4
300080a4:	6a88      	ldr	r0, [r1, #40]	; 0x28
300080a6:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
300080aa:	4304      	orrs	r4, r0
300080ac:	488a      	ldr	r0, [pc, #552]	; (300082d8 <DDR_PHY_DLL_CLK_DIV+0x270>)
300080ae:	628c      	str	r4, [r1, #40]	; 0x28
300080b0:	6a4c      	ldr	r4, [r1, #36]	; 0x24
300080b2:	4020      	ands	r0, r4
300080b4:	4c89      	ldr	r4, [pc, #548]	; (300082dc <DDR_PHY_DLL_CLK_DIV+0x274>)
300080b6:	4303      	orrs	r3, r0
300080b8:	624b      	str	r3, [r1, #36]	; 0x24
300080ba:	6a8b      	ldr	r3, [r1, #40]	; 0x28
300080bc:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
300080c0:	431a      	orrs	r2, r3
300080c2:	628a      	str	r2, [r1, #40]	; 0x28
300080c4:	678d      	str	r5, [r1, #120]	; 0x78
300080c6:	7820      	ldrb	r0, [r4, #0]
300080c8:	2800      	cmp	r0, #0
300080ca:	f000 8083 	beq.w	300081d4 <DDR_PHY_DLL_CLK_DIV+0x16c>
300080ce:	28e0      	cmp	r0, #224	; 0xe0
300080d0:	d15a      	bne.n	30008188 <DDR_PHY_DLL_CLK_DIV+0x120>
300080d2:	4b7e      	ldr	r3, [pc, #504]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
300080d4:	2108      	movs	r1, #8
300080d6:	4c82      	ldr	r4, [pc, #520]	; (300082e0 <DDR_PHY_DLL_CLK_DIV+0x278>)
300080d8:	4a82      	ldr	r2, [pc, #520]	; (300082e4 <DDR_PHY_DLL_CLK_DIV+0x27c>)
300080da:	4883      	ldr	r0, [pc, #524]	; (300082e8 <DDR_PHY_DLL_CLK_DIV+0x280>)
300080dc:	611c      	str	r4, [r3, #16]
300080de:	615a      	str	r2, [r3, #20]
300080e0:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
300080e4:	2022      	movs	r0, #34	; 0x22
300080e6:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
300080ea:	f04f 3122 	mov.w	r1, #572662306	; 0x22222222
300080ee:	689c      	ldr	r4, [r3, #8]
300080f0:	4a7e      	ldr	r2, [pc, #504]	; (300082ec <DDR_PHY_DLL_CLK_DIV+0x284>)
300080f2:	4022      	ands	r2, r4
300080f4:	f042 0201 	orr.w	r2, r2, #1
300080f8:	609a      	str	r2, [r3, #8]
300080fa:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300080fe:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30008102:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
30008106:	f8c3 1080 	str.w	r1, [r3, #128]	; 0x80
3000810a:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
3000810e:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008112:	f022 0277 	bic.w	r2, r2, #119	; 0x77
30008116:	4c76      	ldr	r4, [pc, #472]	; (300082f0 <DDR_PHY_DLL_CLK_DIV+0x288>)
30008118:	f042 0212 	orr.w	r2, r2, #18
3000811c:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008120:	f8c3 0088 	str.w	r0, [r3, #136]	; 0x88
30008124:	f8c3 1094 	str.w	r1, [r3, #148]	; 0x94
30008128:	7820      	ldrb	r0, [r4, #0]
3000812a:	4a68      	ldr	r2, [pc, #416]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000812c:	4971      	ldr	r1, [pc, #452]	; (300082f4 <DDR_PHY_DLL_CLK_DIV+0x28c>)
3000812e:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008132:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
30008136:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000813a:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
3000813e:	009b      	lsls	r3, r3, #2
30008140:	d519      	bpl.n	30008176 <DDR_PHY_DLL_CLK_DIV+0x10e>
30008142:	2800      	cmp	r0, #0
30008144:	d04e      	beq.n	300081e4 <DDR_PHY_DLL_CLK_DIV+0x17c>
30008146:	2801      	cmp	r0, #1
30008148:	4b60      	ldr	r3, [pc, #384]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000814a:	bf0c      	ite	eq
3000814c:	4a6a      	ldreq	r2, [pc, #424]	; (300082f8 <DDR_PHY_DLL_CLK_DIV+0x290>)
3000814e:	4a6b      	ldrne	r2, [pc, #428]	; (300082fc <DDR_PHY_DLL_CLK_DIV+0x294>)
30008150:	601a      	str	r2, [r3, #0]
30008152:	4b5e      	ldr	r3, [pc, #376]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
30008154:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008158:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
3000815c:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
30008160:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008164:	699a      	ldr	r2, [r3, #24]
30008166:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
3000816a:	619a      	str	r2, [r3, #24]
3000816c:	699a      	ldr	r2, [r3, #24]
3000816e:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30008172:	619a      	str	r2, [r3, #24]
30008174:	bd70      	pop	{r4, r5, r6, pc}
30008176:	2800      	cmp	r0, #0
30008178:	d030      	beq.n	300081dc <DDR_PHY_DLL_CLK_DIV+0x174>
3000817a:	2801      	cmp	r0, #1
3000817c:	4b53      	ldr	r3, [pc, #332]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000817e:	bf0c      	ite	eq
30008180:	4a5f      	ldreq	r2, [pc, #380]	; (30008300 <DDR_PHY_DLL_CLK_DIV+0x298>)
30008182:	4a60      	ldrne	r2, [pc, #384]	; (30008304 <DDR_PHY_DLL_CLK_DIV+0x29c>)
30008184:	601a      	str	r2, [r3, #0]
30008186:	e7e4      	b.n	30008152 <DDR_PHY_DLL_CLK_DIV+0xea>
30008188:	2800      	cmp	r0, #0
3000818a:	d062      	beq.n	30008252 <DDR_PHY_DLL_CLK_DIV+0x1ea>
3000818c:	2802      	cmp	r0, #2
3000818e:	d12d      	bne.n	300081ec <DDR_PHY_DLL_CLK_DIV+0x184>
30008190:	4b4e      	ldr	r3, [pc, #312]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
30008192:	220e      	movs	r2, #14
30008194:	495c      	ldr	r1, [pc, #368]	; (30008308 <DDR_PHY_DLL_CLK_DIV+0x2a0>)
30008196:	485d      	ldr	r0, [pc, #372]	; (3000830c <DDR_PHY_DLL_CLK_DIV+0x2a4>)
30008198:	4c5d      	ldr	r4, [pc, #372]	; (30008310 <DDR_PHY_DLL_CLK_DIV+0x2a8>)
3000819a:	611c      	str	r4, [r3, #16]
3000819c:	6159      	str	r1, [r3, #20]
3000819e:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
300081a2:	4952      	ldr	r1, [pc, #328]	; (300082ec <DDR_PHY_DLL_CLK_DIV+0x284>)
300081a4:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
300081a8:	f240 32c1 	movw	r2, #961	; 0x3c1
300081ac:	6898      	ldr	r0, [r3, #8]
300081ae:	4001      	ands	r1, r0
300081b0:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
300081b4:	2022      	movs	r0, #34	; 0x22
300081b6:	430a      	orrs	r2, r1
300081b8:	609a      	str	r2, [r3, #8]
300081ba:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
300081be:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300081c2:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300081c6:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300081ca:	f8c3 4080 	str.w	r4, [r3, #128]	; 0x80
300081ce:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300081d2:	e79e      	b.n	30008112 <DDR_PHY_DLL_CLK_DIV+0xaa>
300081d4:	4b4f      	ldr	r3, [pc, #316]	; (30008314 <DDR_PHY_DLL_CLK_DIV+0x2ac>)
300081d6:	4798      	blx	r3
300081d8:	7020      	strb	r0, [r4, #0]
300081da:	e778      	b.n	300080ce <DDR_PHY_DLL_CLK_DIV+0x66>
300081dc:	4b4e      	ldr	r3, [pc, #312]	; (30008318 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
300081de:	4798      	blx	r3
300081e0:	7020      	strb	r0, [r4, #0]
300081e2:	e7ca      	b.n	3000817a <DDR_PHY_DLL_CLK_DIV+0x112>
300081e4:	4b4c      	ldr	r3, [pc, #304]	; (30008318 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
300081e6:	4798      	blx	r3
300081e8:	7020      	strb	r0, [r4, #0]
300081ea:	e7ac      	b.n	30008146 <DDR_PHY_DLL_CLK_DIV+0xde>
300081ec:	2805      	cmp	r0, #5
300081ee:	d03b      	beq.n	30008268 <DDR_PHY_DLL_CLK_DIV+0x200>
300081f0:	2803      	cmp	r0, #3
300081f2:	f040 80e6 	bne.w	300083c2 <DDR_PHY_DLL_CLK_DIV+0x35a>
300081f6:	4a35      	ldr	r2, [pc, #212]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
300081f8:	230e      	movs	r3, #14
300081fa:	4c48      	ldr	r4, [pc, #288]	; (3000831c <DDR_PHY_DLL_CLK_DIV+0x2b4>)
300081fc:	4948      	ldr	r1, [pc, #288]	; (30008320 <DDR_PHY_DLL_CLK_DIV+0x2b8>)
300081fe:	4849      	ldr	r0, [pc, #292]	; (30008324 <DDR_PHY_DLL_CLK_DIV+0x2bc>)
30008200:	6114      	str	r4, [r2, #16]
30008202:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
30008206:	6151      	str	r1, [r2, #20]
30008208:	f8c2 0098 	str.w	r0, [r2, #152]	; 0x98
3000820c:	4937      	ldr	r1, [pc, #220]	; (300082ec <DDR_PHY_DLL_CLK_DIV+0x284>)
3000820e:	f8c2 30a0 	str.w	r3, [r2, #160]	; 0xa0
30008212:	f240 33c1 	movw	r3, #961	; 0x3c1
30008216:	6890      	ldr	r0, [r2, #8]
30008218:	4001      	ands	r1, r0
3000821a:	2033      	movs	r0, #51	; 0x33
3000821c:	430b      	orrs	r3, r1
3000821e:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
30008222:	6093      	str	r3, [r2, #8]
30008224:	f8d2 309c 	ldr.w	r3, [r2, #156]	; 0x9c
30008228:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
3000822c:	f8c2 309c 	str.w	r3, [r2, #156]	; 0x9c
30008230:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
30008234:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008238:	4c2d      	ldr	r4, [pc, #180]	; (300082f0 <DDR_PHY_DLL_CLK_DIV+0x288>)
3000823a:	f023 0377 	bic.w	r3, r3, #119	; 0x77
3000823e:	f043 0312 	orr.w	r3, r3, #18
30008242:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30008246:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
3000824a:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
3000824e:	7820      	ldrb	r0, [r4, #0]
30008250:	e76b      	b.n	3000812a <DDR_PHY_DLL_CLK_DIV+0xc2>
30008252:	4d30      	ldr	r5, [pc, #192]	; (30008314 <DDR_PHY_DLL_CLK_DIV+0x2ac>)
30008254:	47a8      	blx	r5
30008256:	2802      	cmp	r0, #2
30008258:	7020      	strb	r0, [r4, #0]
3000825a:	d099      	beq.n	30008190 <DDR_PHY_DLL_CLK_DIV+0x128>
3000825c:	2800      	cmp	r0, #0
3000825e:	d1c5      	bne.n	300081ec <DDR_PHY_DLL_CLK_DIV+0x184>
30008260:	47a8      	blx	r5
30008262:	2805      	cmp	r0, #5
30008264:	7020      	strb	r0, [r4, #0]
30008266:	d163      	bne.n	30008330 <DDR_PHY_DLL_CLK_DIV+0x2c8>
30008268:	4918      	ldr	r1, [pc, #96]	; (300082cc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000826a:	230e      	movs	r3, #14
3000826c:	4c2e      	ldr	r4, [pc, #184]	; (30008328 <DDR_PHY_DLL_CLK_DIV+0x2c0>)
3000826e:	4a2f      	ldr	r2, [pc, #188]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x2c4>)
30008270:	4826      	ldr	r0, [pc, #152]	; (3000830c <DDR_PHY_DLL_CLK_DIV+0x2a4>)
30008272:	610c      	str	r4, [r1, #16]
30008274:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
30008278:	614a      	str	r2, [r1, #20]
3000827a:	f8c1 0098 	str.w	r0, [r1, #152]	; 0x98
3000827e:	4a1b      	ldr	r2, [pc, #108]	; (300082ec <DDR_PHY_DLL_CLK_DIV+0x284>)
30008280:	f8c1 30a0 	str.w	r3, [r1, #160]	; 0xa0
30008284:	f240 33c1 	movw	r3, #961	; 0x3c1
30008288:	6888      	ldr	r0, [r1, #8]
3000828a:	4002      	ands	r2, r0
3000828c:	2033      	movs	r0, #51	; 0x33
3000828e:	4313      	orrs	r3, r2
30008290:	f04f 5250 	mov.w	r2, #872415232	; 0x34000000
30008294:	608b      	str	r3, [r1, #8]
30008296:	f8d1 309c 	ldr.w	r3, [r1, #156]	; 0x9c
3000829a:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
3000829e:	f8c1 309c 	str.w	r3, [r1, #156]	; 0x9c
300082a2:	f8c1 4080 	str.w	r4, [r1, #128]	; 0x80
300082a6:	f8d1 3084 	ldr.w	r3, [r1, #132]	; 0x84
300082aa:	4c11      	ldr	r4, [pc, #68]	; (300082f0 <DDR_PHY_DLL_CLK_DIV+0x288>)
300082ac:	f023 0377 	bic.w	r3, r3, #119	; 0x77
300082b0:	f043 0312 	orr.w	r3, r3, #18
300082b4:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300082b8:	f8c1 0088 	str.w	r0, [r1, #136]	; 0x88
300082bc:	f8c1 2094 	str.w	r2, [r1, #148]	; 0x94
300082c0:	7820      	ldrb	r0, [r4, #0]
300082c2:	e732      	b.n	3000812a <DDR_PHY_DLL_CLK_DIV+0xc2>
300082c4:	cccccccd 	.word	0xcccccccd
300082c8:	001fff88 	.word	0x001fff88
300082cc:	41011000 	.word	0x41011000
300082d0:	f800ffff 	.word	0xf800ffff
300082d4:	07ff0000 	.word	0x07ff0000
300082d8:	fffff800 	.word	0xfffff800
300082dc:	3000ea70 	.word	0x3000ea70
300082e0:	07060012 	.word	0x07060012
300082e4:	000f0707 	.word	0x000f0707
300082e8:	080f0f0f 	.word	0x080f0f0f
300082ec:	ffffe000 	.word	0xffffe000
300082f0:	3000ea71 	.word	0x3000ea71
300082f4:	42008000 	.word	0x42008000
300082f8:	4c000903 	.word	0x4c000903
300082fc:	4c000821 	.word	0x4c000821
30008300:	40000103 	.word	0x40000103
30008304:	40000021 	.word	0x40000021
30008308:	00130b0b 	.word	0x00130b0b
3000830c:	0e131313 	.word	0x0e131313
30008310:	0b0b0016 	.word	0x0b0b0016
30008314:	30007e69 	.word	0x30007e69
30008318:	30007f19 	.word	0x30007f19
3000831c:	0e0e0018 	.word	0x0e0e0018
30008320:	00170e0e 	.word	0x00170e0e
30008324:	0e171717 	.word	0x0e171717
30008328:	0a0a0018 	.word	0x0a0a0018
3000832c:	00130a0a 	.word	0x00130a0a
30008330:	2800      	cmp	r0, #0
30008332:	f47f af5d 	bne.w	300081f0 <DDR_PHY_DLL_CLK_DIV+0x188>
30008336:	47a8      	blx	r5
30008338:	2803      	cmp	r0, #3
3000833a:	7020      	strb	r0, [r4, #0]
3000833c:	f43f af5b 	beq.w	300081f6 <DDR_PHY_DLL_CLK_DIV+0x18e>
30008340:	2800      	cmp	r0, #0
30008342:	d13e      	bne.n	300083c2 <DDR_PHY_DLL_CLK_DIV+0x35a>
30008344:	47a8      	blx	r5
30008346:	280a      	cmp	r0, #10
30008348:	7020      	strb	r0, [r4, #0]
3000834a:	d03c      	beq.n	300083c6 <DDR_PHY_DLL_CLK_DIV+0x35e>
3000834c:	b908      	cbnz	r0, 30008352 <DDR_PHY_DLL_CLK_DIV+0x2ea>
3000834e:	47a8      	blx	r5
30008350:	7020      	strb	r0, [r4, #0]
30008352:	2809      	cmp	r0, #9
30008354:	d04c      	beq.n	300083f0 <DDR_PHY_DLL_CLK_DIV+0x388>
30008356:	4b32      	ldr	r3, [pc, #200]	; (30008420 <DDR_PHY_DLL_CLK_DIV+0x3b8>)
30008358:	681b      	ldr	r3, [r3, #0]
3000835a:	079a      	lsls	r2, r3, #30
3000835c:	d45c      	bmi.n	30008418 <DDR_PHY_DLL_CLK_DIV+0x3b0>
3000835e:	4b31      	ldr	r3, [pc, #196]	; (30008424 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008360:	2100      	movs	r1, #0
30008362:	f44f 2400 	mov.w	r4, #524288	; 0x80000
30008366:	4830      	ldr	r0, [pc, #192]	; (30008428 <DDR_PHY_DLL_CLK_DIV+0x3c0>)
30008368:	6119      	str	r1, [r3, #16]
3000836a:	4a30      	ldr	r2, [pc, #192]	; (3000842c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
3000836c:	615c      	str	r4, [r3, #20]
3000836e:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008372:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
30008376:	6899      	ldr	r1, [r3, #8]
30008378:	4c2d      	ldr	r4, [pc, #180]	; (30008430 <DDR_PHY_DLL_CLK_DIV+0x3c8>)
3000837a:	400a      	ands	r2, r1
3000837c:	609a      	str	r2, [r3, #8]
3000837e:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
30008382:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30008386:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
3000838a:	7820      	ldrb	r0, [r4, #0]
3000838c:	2800      	cmp	r0, #0
3000838e:	d03f      	beq.n	30008410 <DDR_PHY_DLL_CLK_DIV+0x3a8>
30008390:	2801      	cmp	r0, #1
30008392:	4b24      	ldr	r3, [pc, #144]	; (30008424 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008394:	f04f 0100 	mov.w	r1, #0
30008398:	bf0c      	ite	eq
3000839a:	2233      	moveq	r2, #51	; 0x33
3000839c:	2222      	movne	r2, #34	; 0x22
3000839e:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
300083a2:	f04f 3333 	mov.w	r3, #858993459	; 0x33333333
300083a6:	4a1f      	ldr	r2, [pc, #124]	; (30008424 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300083a8:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
300083ac:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300083b0:	f023 0377 	bic.w	r3, r3, #119	; 0x77
300083b4:	f043 0322 	orr.w	r3, r3, #34	; 0x22
300083b8:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300083bc:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
300083c0:	e6b3      	b.n	3000812a <DDR_PHY_DLL_CLK_DIV+0xc2>
300083c2:	280a      	cmp	r0, #10
300083c4:	d1c5      	bne.n	30008352 <DDR_PHY_DLL_CLK_DIV+0x2ea>
300083c6:	4b17      	ldr	r3, [pc, #92]	; (30008424 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300083c8:	220e      	movs	r2, #14
300083ca:	4c1a      	ldr	r4, [pc, #104]	; (30008434 <DDR_PHY_DLL_CLK_DIV+0x3cc>)
300083cc:	491a      	ldr	r1, [pc, #104]	; (30008438 <DDR_PHY_DLL_CLK_DIV+0x3d0>)
300083ce:	481b      	ldr	r0, [pc, #108]	; (3000843c <DDR_PHY_DLL_CLK_DIV+0x3d4>)
300083d0:	611c      	str	r4, [r3, #16]
300083d2:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
300083d6:	6159      	str	r1, [r3, #20]
300083d8:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
300083dc:	4913      	ldr	r1, [pc, #76]	; (3000842c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
300083de:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
300083e2:	f240 32c1 	movw	r2, #961	; 0x3c1
300083e6:	6898      	ldr	r0, [r3, #8]
300083e8:	4001      	ands	r1, r0
300083ea:	2033      	movs	r0, #51	; 0x33
300083ec:	430a      	orrs	r2, r1
300083ee:	e6e3      	b.n	300081b8 <DDR_PHY_DLL_CLK_DIV+0x150>
300083f0:	4b0c      	ldr	r3, [pc, #48]	; (30008424 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300083f2:	2212      	movs	r2, #18
300083f4:	4912      	ldr	r1, [pc, #72]	; (30008440 <DDR_PHY_DLL_CLK_DIV+0x3d8>)
300083f6:	4813      	ldr	r0, [pc, #76]	; (30008444 <DDR_PHY_DLL_CLK_DIV+0x3dc>)
300083f8:	4c13      	ldr	r4, [pc, #76]	; (30008448 <DDR_PHY_DLL_CLK_DIV+0x3e0>)
300083fa:	611c      	str	r4, [r3, #16]
300083fc:	6159      	str	r1, [r3, #20]
300083fe:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008402:	490a      	ldr	r1, [pc, #40]	; (3000842c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
30008404:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
30008408:	f640 72c1 	movw	r2, #4033	; 0xfc1
3000840c:	6898      	ldr	r0, [r3, #8]
3000840e:	e6ce      	b.n	300081ae <DDR_PHY_DLL_CLK_DIV+0x146>
30008410:	4b0e      	ldr	r3, [pc, #56]	; (3000844c <DDR_PHY_DLL_CLK_DIV+0x3e4>)
30008412:	4798      	blx	r3
30008414:	7020      	strb	r0, [r4, #0]
30008416:	e7bb      	b.n	30008390 <DDR_PHY_DLL_CLK_DIV+0x328>
30008418:	480d      	ldr	r0, [pc, #52]	; (30008450 <DDR_PHY_DLL_CLK_DIV+0x3e8>)
3000841a:	f003 fecd 	bl	3000c1b8 <__DiagPrintf_veneer>
3000841e:	e79e      	b.n	3000835e <DDR_PHY_DLL_CLK_DIV+0x2f6>
30008420:	2001c00c 	.word	0x2001c00c
30008424:	41011000 	.word	0x41011000
30008428:	00080808 	.word	0x00080808
3000842c:	ffffe000 	.word	0xffffe000
30008430:	3000ea71 	.word	0x3000ea71
30008434:	0f0f0016 	.word	0x0f0f0016
30008438:	00180f0f 	.word	0x00180f0f
3000843c:	0e181818 	.word	0x0e181818
30008440:	00170e0e 	.word	0x00170e0e
30008444:	12171717 	.word	0x12171717
30008448:	0e0e001a 	.word	0x0e0e001a
3000844c:	30007f19 	.word	0x30007f19
30008450:	3000d1a0 	.word	0x3000d1a0

30008454 <DDR_PHY_CRT_Init>:
30008454:	492f      	ldr	r1, [pc, #188]	; (30008514 <DDR_PHY_CRT_Init+0xc0>)
30008456:	2005      	movs	r0, #5
30008458:	4a2f      	ldr	r2, [pc, #188]	; (30008518 <DDR_PHY_CRT_Init+0xc4>)
3000845a:	b510      	push	{r4, lr}
3000845c:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
30008460:	4c2e      	ldr	r4, [pc, #184]	; (3000851c <DDR_PHY_CRT_Init+0xc8>)
30008462:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008466:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
3000846a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000846e:	f043 0301 	orr.w	r3, r3, #1
30008472:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008476:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000847a:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000847e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008482:	68e3      	ldr	r3, [r4, #12]
30008484:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008488:	60e3      	str	r3, [r4, #12]
3000848a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000848e:	f043 0302 	orr.w	r3, r3, #2
30008492:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008496:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000849a:	f023 0308 	bic.w	r3, r3, #8
3000849e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300084a2:	4790      	blx	r2
300084a4:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
300084a8:	f641 73ff 	movw	r3, #8191	; 0x1fff
300084ac:	491c      	ldr	r1, [pc, #112]	; (30008520 <DDR_PHY_CRT_Init+0xcc>)
300084ae:	f042 0204 	orr.w	r2, r2, #4
300084b2:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
300084b6:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
300084ba:	f042 0208 	orr.w	r2, r2, #8
300084be:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
300084c2:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300084c4:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
300084c8:	62a2      	str	r2, [r4, #40]	; 0x28
300084ca:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300084cc:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
300084d0:	62a2      	str	r2, [r4, #40]	; 0x28
300084d2:	69e2      	ldr	r2, [r4, #28]
300084d4:	f042 0208 	orr.w	r2, r2, #8
300084d8:	61e2      	str	r2, [r4, #28]
300084da:	68a2      	ldr	r2, [r4, #8]
300084dc:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
300084e0:	60a2      	str	r2, [r4, #8]
300084e2:	6862      	ldr	r2, [r4, #4]
300084e4:	4313      	orrs	r3, r2
300084e6:	6063      	str	r3, [r4, #4]
300084e8:	68a3      	ldr	r3, [r4, #8]
300084ea:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
300084ee:	60a3      	str	r3, [r4, #8]
300084f0:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
300084f4:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300084f8:	f023 0303 	bic.w	r3, r3, #3
300084fc:	f043 0302 	orr.w	r3, r3, #2
30008500:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008504:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008508:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000850c:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008510:	bd10      	pop	{r4, pc}
30008512:	bf00      	nop
30008514:	42008000 	.word	0x42008000
30008518:	00009b2d 	.word	0x00009b2d
3000851c:	41011000 	.word	0x41011000
30008520:	90161f00 	.word	0x90161f00

30008524 <DDR_PHY_R240_ZQ_CAL>:
30008524:	b538      	push	{r3, r4, r5, lr}
30008526:	4c84      	ldr	r4, [pc, #528]	; (30008738 <DDR_PHY_R240_ZQ_CAL+0x214>)
30008528:	7823      	ldrb	r3, [r4, #0]
3000852a:	2b00      	cmp	r3, #0
3000852c:	d05e      	beq.n	300085ec <DDR_PHY_R240_ZQ_CAL+0xc8>
3000852e:	4c83      	ldr	r4, [pc, #524]	; (3000873c <DDR_PHY_R240_ZQ_CAL+0x218>)
30008530:	2b03      	cmp	r3, #3
30008532:	7820      	ldrb	r0, [r4, #0]
30008534:	d136      	bne.n	300085a4 <DDR_PHY_R240_ZQ_CAL+0x80>
30008536:	2800      	cmp	r0, #0
30008538:	d155      	bne.n	300085e6 <DDR_PHY_R240_ZQ_CAL+0xc2>
3000853a:	4d81      	ldr	r5, [pc, #516]	; (30008740 <DDR_PHY_R240_ZQ_CAL+0x21c>)
3000853c:	47a8      	blx	r5
3000853e:	28e0      	cmp	r0, #224	; 0xe0
30008540:	7020      	strb	r0, [r4, #0]
30008542:	f040 808f 	bne.w	30008664 <DDR_PHY_R240_ZQ_CAL+0x140>
30008546:	4b7f      	ldr	r3, [pc, #508]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
30008548:	2100      	movs	r1, #0
3000854a:	4c7f      	ldr	r4, [pc, #508]	; (30008748 <DDR_PHY_R240_ZQ_CAL+0x224>)
3000854c:	487f      	ldr	r0, [pc, #508]	; (3000874c <DDR_PHY_R240_ZQ_CAL+0x228>)
3000854e:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
30008552:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
30008556:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000855a:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
3000855e:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
30008562:	f504 2432 	add.w	r4, r4, #729088	; 0xb2000
30008566:	4a7a      	ldr	r2, [pc, #488]	; (30008750 <DDR_PHY_R240_ZQ_CAL+0x22c>)
30008568:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
3000856c:	f204 241a 	addw	r4, r4, #538	; 0x21a
30008570:	f200 201b 	addw	r0, r0, #539	; 0x21b
30008574:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
30008578:	f8c3 4190 	str.w	r4, [r3, #400]	; 0x190
3000857c:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
30008580:	f8c3 0198 	str.w	r0, [r3, #408]	; 0x198
30008584:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30008588:	4b6e      	ldr	r3, [pc, #440]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000858a:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000858e:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
30008592:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008596:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000859a:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
3000859e:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300085a2:	bd38      	pop	{r3, r4, r5, pc}
300085a4:	2800      	cmp	r0, #0
300085a6:	d040      	beq.n	3000862a <DDR_PHY_R240_ZQ_CAL+0x106>
300085a8:	2805      	cmp	r0, #5
300085aa:	d124      	bne.n	300085f6 <DDR_PHY_R240_ZQ_CAL+0xd2>
300085ac:	4a65      	ldr	r2, [pc, #404]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
300085ae:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
300085b2:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
300085b6:	f043 0306 	orr.w	r3, r3, #6
300085ba:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
300085be:	4b61      	ldr	r3, [pc, #388]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
300085c0:	2100      	movs	r1, #0
300085c2:	4c64      	ldr	r4, [pc, #400]	; (30008754 <DDR_PHY_R240_ZQ_CAL+0x230>)
300085c4:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
300085c8:	4863      	ldr	r0, [pc, #396]	; (30008758 <DDR_PHY_R240_ZQ_CAL+0x234>)
300085ca:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
300085ce:	f104 54e0 	add.w	r4, r4, #469762048	; 0x1c000000
300085d2:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
300085d6:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
300085da:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300085de:	485f      	ldr	r0, [pc, #380]	; (3000875c <DDR_PHY_R240_ZQ_CAL+0x238>)
300085e0:	f504 6422 	add.w	r4, r4, #2592	; 0xa20
300085e4:	e7c6      	b.n	30008574 <DDR_PHY_R240_ZQ_CAL+0x50>
300085e6:	28e0      	cmp	r0, #224	; 0xe0
300085e8:	d0ad      	beq.n	30008546 <DDR_PHY_R240_ZQ_CAL+0x22>
300085ea:	e03e      	b.n	3000866a <DDR_PHY_R240_ZQ_CAL+0x146>
300085ec:	4b5c      	ldr	r3, [pc, #368]	; (30008760 <DDR_PHY_R240_ZQ_CAL+0x23c>)
300085ee:	4798      	blx	r3
300085f0:	4603      	mov	r3, r0
300085f2:	7020      	strb	r0, [r4, #0]
300085f4:	e79b      	b.n	3000852e <DDR_PHY_R240_ZQ_CAL+0xa>
300085f6:	2803      	cmp	r0, #3
300085f8:	d125      	bne.n	30008646 <DDR_PHY_R240_ZQ_CAL+0x122>
300085fa:	4a52      	ldr	r2, [pc, #328]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
300085fc:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
30008600:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
30008604:	f043 030c 	orr.w	r3, r3, #12
30008608:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
3000860c:	4b4d      	ldr	r3, [pc, #308]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000860e:	2100      	movs	r1, #0
30008610:	4c54      	ldr	r4, [pc, #336]	; (30008764 <DDR_PHY_R240_ZQ_CAL+0x240>)
30008612:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008616:	4854      	ldr	r0, [pc, #336]	; (30008768 <DDR_PHY_R240_ZQ_CAL+0x244>)
30008618:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
3000861c:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
30008620:	4c52      	ldr	r4, [pc, #328]	; (3000876c <DDR_PHY_R240_ZQ_CAL+0x248>)
30008622:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008626:	4852      	ldr	r0, [pc, #328]	; (30008770 <DDR_PHY_R240_ZQ_CAL+0x24c>)
30008628:	e7a4      	b.n	30008574 <DDR_PHY_R240_ZQ_CAL+0x50>
3000862a:	4d45      	ldr	r5, [pc, #276]	; (30008740 <DDR_PHY_R240_ZQ_CAL+0x21c>)
3000862c:	47a8      	blx	r5
3000862e:	2805      	cmp	r0, #5
30008630:	7020      	strb	r0, [r4, #0]
30008632:	d0bb      	beq.n	300085ac <DDR_PHY_R240_ZQ_CAL+0x88>
30008634:	2800      	cmp	r0, #0
30008636:	d1de      	bne.n	300085f6 <DDR_PHY_R240_ZQ_CAL+0xd2>
30008638:	47a8      	blx	r5
3000863a:	2803      	cmp	r0, #3
3000863c:	7020      	strb	r0, [r4, #0]
3000863e:	d0dc      	beq.n	300085fa <DDR_PHY_R240_ZQ_CAL+0xd6>
30008640:	b908      	cbnz	r0, 30008646 <DDR_PHY_R240_ZQ_CAL+0x122>
30008642:	47a8      	blx	r5
30008644:	7020      	strb	r0, [r4, #0]
30008646:	4a3f      	ldr	r2, [pc, #252]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
30008648:	280a      	cmp	r0, #10
3000864a:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
3000864e:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
30008652:	d056      	beq.n	30008702 <DDR_PHY_R240_ZQ_CAL+0x1de>
30008654:	f043 031e 	orr.w	r3, r3, #30
30008658:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
3000865c:	2800      	cmp	r0, #0
3000865e:	f43f af6c 	beq.w	3000853a <DDR_PHY_R240_ZQ_CAL+0x16>
30008662:	e7c0      	b.n	300085e6 <DDR_PHY_R240_ZQ_CAL+0xc2>
30008664:	b908      	cbnz	r0, 3000866a <DDR_PHY_R240_ZQ_CAL+0x146>
30008666:	47a8      	blx	r5
30008668:	7020      	strb	r0, [r4, #0]
3000866a:	2802      	cmp	r0, #2
3000866c:	f43f af6b 	beq.w	30008546 <DDR_PHY_R240_ZQ_CAL+0x22>
30008670:	b910      	cbnz	r0, 30008678 <DDR_PHY_R240_ZQ_CAL+0x154>
30008672:	4b33      	ldr	r3, [pc, #204]	; (30008740 <DDR_PHY_R240_ZQ_CAL+0x21c>)
30008674:	4798      	blx	r3
30008676:	7020      	strb	r0, [r4, #0]
30008678:	2805      	cmp	r0, #5
3000867a:	d0a0      	beq.n	300085be <DDR_PHY_R240_ZQ_CAL+0x9a>
3000867c:	bb20      	cbnz	r0, 300086c8 <DDR_PHY_R240_ZQ_CAL+0x1a4>
3000867e:	4d30      	ldr	r5, [pc, #192]	; (30008740 <DDR_PHY_R240_ZQ_CAL+0x21c>)
30008680:	47a8      	blx	r5
30008682:	2803      	cmp	r0, #3
30008684:	7020      	strb	r0, [r4, #0]
30008686:	d0c1      	beq.n	3000860c <DDR_PHY_R240_ZQ_CAL+0xe8>
30008688:	bb00      	cbnz	r0, 300086cc <DDR_PHY_R240_ZQ_CAL+0x1a8>
3000868a:	47a8      	blx	r5
3000868c:	280a      	cmp	r0, #10
3000868e:	7020      	strb	r0, [r4, #0]
30008690:	d01e      	beq.n	300086d0 <DDR_PHY_R240_ZQ_CAL+0x1ac>
30008692:	b908      	cbnz	r0, 30008698 <DDR_PHY_R240_ZQ_CAL+0x174>
30008694:	47a8      	blx	r5
30008696:	7020      	strb	r0, [r4, #0]
30008698:	2809      	cmp	r0, #9
3000869a:	4b2a      	ldr	r3, [pc, #168]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000869c:	f04f 0100 	mov.w	r1, #0
300086a0:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
300086a4:	d032      	beq.n	3000870c <DDR_PHY_R240_ZQ_CAL+0x1e8>
300086a6:	f8c3 1180 	str.w	r1, [r3, #384]	; 0x180
300086aa:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
300086ae:	f8c3 1188 	str.w	r1, [r3, #392]	; 0x188
300086b2:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
300086b6:	f8c3 2190 	str.w	r2, [r3, #400]	; 0x190
300086ba:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300086be:	f8c3 2198 	str.w	r2, [r3, #408]	; 0x198
300086c2:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300086c6:	e75f      	b.n	30008588 <DDR_PHY_R240_ZQ_CAL+0x64>
300086c8:	2803      	cmp	r0, #3
300086ca:	d09f      	beq.n	3000860c <DDR_PHY_R240_ZQ_CAL+0xe8>
300086cc:	280a      	cmp	r0, #10
300086ce:	d1e3      	bne.n	30008698 <DDR_PHY_R240_ZQ_CAL+0x174>
300086d0:	4b1c      	ldr	r3, [pc, #112]	; (30008744 <DDR_PHY_R240_ZQ_CAL+0x220>)
300086d2:	2100      	movs	r1, #0
300086d4:	4c27      	ldr	r4, [pc, #156]	; (30008774 <DDR_PHY_R240_ZQ_CAL+0x250>)
300086d6:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
300086da:	4827      	ldr	r0, [pc, #156]	; (30008778 <DDR_PHY_R240_ZQ_CAL+0x254>)
300086dc:	f8c3 41a0 	str.w	r4, [r3, #416]	; 0x1a0
300086e0:	f8c3 11a4 	str.w	r1, [r3, #420]	; 0x1a4
300086e4:	4c22      	ldr	r4, [pc, #136]	; (30008770 <DDR_PHY_R240_ZQ_CAL+0x24c>)
300086e6:	f8c3 01a8 	str.w	r0, [r3, #424]	; 0x1a8
300086ea:	4824      	ldr	r0, [pc, #144]	; (3000877c <DDR_PHY_R240_ZQ_CAL+0x258>)
300086ec:	f8c3 11ac 	str.w	r1, [r3, #428]	; 0x1ac
300086f0:	f8c3 41b0 	str.w	r4, [r3, #432]	; 0x1b0
300086f4:	f8c3 21b4 	str.w	r2, [r3, #436]	; 0x1b4
300086f8:	f8c3 01b8 	str.w	r0, [r3, #440]	; 0x1b8
300086fc:	f8c3 21bc 	str.w	r2, [r3, #444]	; 0x1bc
30008700:	e742      	b.n	30008588 <DDR_PHY_R240_ZQ_CAL+0x64>
30008702:	f043 0308 	orr.w	r3, r3, #8
30008706:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
3000870a:	e7e1      	b.n	300086d0 <DDR_PHY_R240_ZQ_CAL+0x1ac>
3000870c:	4c0e      	ldr	r4, [pc, #56]	; (30008748 <DDR_PHY_R240_ZQ_CAL+0x224>)
3000870e:	480f      	ldr	r0, [pc, #60]	; (3000874c <DDR_PHY_R240_ZQ_CAL+0x228>)
30008710:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
30008714:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
30008718:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000871c:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008720:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
30008724:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
30008728:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
3000872c:	f504 6402 	add.w	r4, r4, #2080	; 0x820
30008730:	f200 201b 	addw	r0, r0, #539	; 0x21b
30008734:	e71e      	b.n	30008574 <DDR_PHY_R240_ZQ_CAL+0x50>
30008736:	bf00      	nop
30008738:	3000ea71 	.word	0x3000ea71
3000873c:	3000ea70 	.word	0x3000ea70
30008740:	30007e69 	.word	0x30007e69
30008744:	41011000 	.word	0x41011000
30008748:	0e141414 	.word	0x0e141414
3000874c:	140e0e0e 	.word	0x140e0e0e
30008750:	00262626 	.word	0x00262626
30008754:	0a0e0c0c 	.word	0x0a0e0c0c
30008758:	0c0b0a0a 	.word	0x0c0b0a0a
3000875c:	26162620 	.word	0x26162620
30008760:	30007f19 	.word	0x30007f19
30008764:	0a0c0b0b 	.word	0x0a0c0b0b
30008768:	0b0b0a0a 	.word	0x0b0b0a0a
3000876c:	26172721 	.word	0x26172721
30008770:	2615231e 	.word	0x2615231e
30008774:	090b0a0a 	.word	0x090b0a0a
30008778:	0a0a0909 	.word	0x0a0a0909
3000877c:	2613201b 	.word	0x2613201b

30008780 <DDR_PHY_ZQ_SET_SEL>:
30008780:	b410      	push	{r4}
30008782:	4b25      	ldr	r3, [pc, #148]	; (30008818 <DDR_PHY_ZQ_SET_SEL+0x98>)
30008784:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008788:	4a24      	ldr	r2, [pc, #144]	; (3000881c <DDR_PHY_ZQ_SET_SEL+0x9c>)
3000878a:	4c25      	ldr	r4, [pc, #148]	; (30008820 <DDR_PHY_ZQ_SET_SEL+0xa0>)
3000878c:	4925      	ldr	r1, [pc, #148]	; (30008824 <DDR_PHY_ZQ_SET_SEL+0xa4>)
3000878e:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
30008792:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
30008796:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
3000879a:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
3000879e:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
300087a2:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
300087a6:	f64f 74ff 	movw	r4, #65535	; 0xffff
300087aa:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
300087ae:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
300087b2:	2200      	movs	r2, #0
300087b4:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
300087b8:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
300087bc:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
300087c0:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
300087c4:	f04f 31ff 	mov.w	r1, #4294967295
300087c8:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
300087cc:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
300087d0:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
300087d4:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
300087d8:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
300087dc:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
300087e0:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
300087e4:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
300087e8:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
300087ec:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
300087f0:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
300087f4:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
300087f8:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
300087fc:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30008800:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
30008804:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
30008808:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
3000880c:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30008810:	f85d 4b04 	ldr.w	r4, [sp], #4
30008814:	4770      	bx	lr
30008816:	bf00      	nop
30008818:	41011000 	.word	0x41011000
3000881c:	ffff1111 	.word	0xffff1111
30008820:	ffff3333 	.word	0xffff3333
30008824:	ffff0000 	.word	0xffff0000

30008828 <DDR_PHY_DELAY_TAP_SET>:
30008828:	b510      	push	{r4, lr}
3000882a:	4b15      	ldr	r3, [pc, #84]	; (30008880 <DDR_PHY_DELAY_TAP_SET+0x58>)
3000882c:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
30008830:	4914      	ldr	r1, [pc, #80]	; (30008884 <DDR_PHY_DELAY_TAP_SET+0x5c>)
30008832:	4c15      	ldr	r4, [pc, #84]	; (30008888 <DDR_PHY_DELAY_TAP_SET+0x60>)
30008834:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
30008838:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
3000883c:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30008840:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
30008844:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30008848:	7820      	ldrb	r0, [r4, #0]
3000884a:	b1a0      	cbz	r0, 30008876 <DDR_PHY_DELAY_TAP_SET+0x4e>
3000884c:	2801      	cmp	r0, #1
3000884e:	4b0c      	ldr	r3, [pc, #48]	; (30008880 <DDR_PHY_DELAY_TAP_SET+0x58>)
30008850:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
30008854:	bf0c      	ite	eq
30008856:	4a0d      	ldreq	r2, [pc, #52]	; (3000888c <DDR_PHY_DELAY_TAP_SET+0x64>)
30008858:	4a0d      	ldrne	r2, [pc, #52]	; (30008890 <DDR_PHY_DELAY_TAP_SET+0x68>)
3000885a:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
3000885e:	f640 0288 	movw	r2, #2184	; 0x888
30008862:	4b07      	ldr	r3, [pc, #28]	; (30008880 <DDR_PHY_DELAY_TAP_SET+0x58>)
30008864:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30008868:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
3000886c:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30008870:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30008874:	bd10      	pop	{r4, pc}
30008876:	4b07      	ldr	r3, [pc, #28]	; (30008894 <DDR_PHY_DELAY_TAP_SET+0x6c>)
30008878:	4798      	blx	r3
3000887a:	7020      	strb	r0, [r4, #0]
3000887c:	e7e6      	b.n	3000884c <DDR_PHY_DELAY_TAP_SET+0x24>
3000887e:	bf00      	nop
30008880:	41011000 	.word	0x41011000
30008884:	20888888 	.word	0x20888888
30008888:	3000ea71 	.word	0x3000ea71
3000888c:	7c033333 	.word	0x7c033333
30008890:	7c011111 	.word	0x7c011111
30008894:	30007f19 	.word	0x30007f19

30008898 <DDR_PHY_READ_CTRL>:
30008898:	b538      	push	{r3, r4, r5, lr}
3000889a:	4c2a      	ldr	r4, [pc, #168]	; (30008944 <DDR_PHY_READ_CTRL+0xac>)
3000889c:	7820      	ldrb	r0, [r4, #0]
3000889e:	b330      	cbz	r0, 300088ee <DDR_PHY_READ_CTRL+0x56>
300088a0:	28e0      	cmp	r0, #224	; 0xe0
300088a2:	d114      	bne.n	300088ce <DDR_PHY_READ_CTRL+0x36>
300088a4:	4b28      	ldr	r3, [pc, #160]	; (30008948 <DDR_PHY_READ_CTRL+0xb0>)
300088a6:	2202      	movs	r2, #2
300088a8:	2103      	movs	r1, #3
300088aa:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
300088ae:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
300088b2:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
300088b6:	4b24      	ldr	r3, [pc, #144]	; (30008948 <DDR_PHY_READ_CTRL+0xb0>)
300088b8:	2108      	movs	r1, #8
300088ba:	22fe      	movs	r2, #254	; 0xfe
300088bc:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
300088c0:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
300088c4:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
300088c8:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
300088cc:	bd38      	pop	{r3, r4, r5, pc}
300088ce:	2802      	cmp	r0, #2
300088d0:	d0e8      	beq.n	300088a4 <DDR_PHY_READ_CTRL+0xc>
300088d2:	2805      	cmp	r0, #5
300088d4:	d001      	beq.n	300088da <DDR_PHY_READ_CTRL+0x42>
300088d6:	2803      	cmp	r0, #3
300088d8:	d131      	bne.n	3000893e <DDR_PHY_READ_CTRL+0xa6>
300088da:	4b1b      	ldr	r3, [pc, #108]	; (30008948 <DDR_PHY_READ_CTRL+0xb0>)
300088dc:	2202      	movs	r2, #2
300088de:	2104      	movs	r1, #4
300088e0:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
300088e4:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
300088e8:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
300088ec:	e7e3      	b.n	300088b6 <DDR_PHY_READ_CTRL+0x1e>
300088ee:	4d17      	ldr	r5, [pc, #92]	; (3000894c <DDR_PHY_READ_CTRL+0xb4>)
300088f0:	47a8      	blx	r5
300088f2:	28e0      	cmp	r0, #224	; 0xe0
300088f4:	7020      	strb	r0, [r4, #0]
300088f6:	d0d5      	beq.n	300088a4 <DDR_PHY_READ_CTRL+0xc>
300088f8:	2800      	cmp	r0, #0
300088fa:	d1e8      	bne.n	300088ce <DDR_PHY_READ_CTRL+0x36>
300088fc:	47a8      	blx	r5
300088fe:	2802      	cmp	r0, #2
30008900:	7020      	strb	r0, [r4, #0]
30008902:	d0cf      	beq.n	300088a4 <DDR_PHY_READ_CTRL+0xc>
30008904:	2800      	cmp	r0, #0
30008906:	d1e4      	bne.n	300088d2 <DDR_PHY_READ_CTRL+0x3a>
30008908:	47a8      	blx	r5
3000890a:	2805      	cmp	r0, #5
3000890c:	7020      	strb	r0, [r4, #0]
3000890e:	d0e4      	beq.n	300088da <DDR_PHY_READ_CTRL+0x42>
30008910:	2800      	cmp	r0, #0
30008912:	d1e0      	bne.n	300088d6 <DDR_PHY_READ_CTRL+0x3e>
30008914:	47a8      	blx	r5
30008916:	2803      	cmp	r0, #3
30008918:	7020      	strb	r0, [r4, #0]
3000891a:	d0de      	beq.n	300088da <DDR_PHY_READ_CTRL+0x42>
3000891c:	b978      	cbnz	r0, 3000893e <DDR_PHY_READ_CTRL+0xa6>
3000891e:	47a8      	blx	r5
30008920:	280a      	cmp	r0, #10
30008922:	7020      	strb	r0, [r4, #0]
30008924:	d0d9      	beq.n	300088da <DDR_PHY_READ_CTRL+0x42>
30008926:	b908      	cbnz	r0, 3000892c <DDR_PHY_READ_CTRL+0x94>
30008928:	47a8      	blx	r5
3000892a:	7020      	strb	r0, [r4, #0]
3000892c:	4b06      	ldr	r3, [pc, #24]	; (30008948 <DDR_PHY_READ_CTRL+0xb0>)
3000892e:	2203      	movs	r2, #3
30008930:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
30008934:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
30008938:	f8c3 2170 	str.w	r2, [r3, #368]	; 0x170
3000893c:	e7bb      	b.n	300088b6 <DDR_PHY_READ_CTRL+0x1e>
3000893e:	280a      	cmp	r0, #10
30008940:	d0cb      	beq.n	300088da <DDR_PHY_READ_CTRL+0x42>
30008942:	e7f3      	b.n	3000892c <DDR_PHY_READ_CTRL+0x94>
30008944:	3000ea70 	.word	0x3000ea70
30008948:	41011000 	.word	0x41011000
3000894c:	30007e69 	.word	0x30007e69

30008950 <DDR_PHY_READ_LEVELING>:
30008950:	b538      	push	{r3, r4, r5, lr}
30008952:	4c36      	ldr	r4, [pc, #216]	; (30008a2c <DDR_PHY_READ_LEVELING+0xdc>)
30008954:	7820      	ldrb	r0, [r4, #0]
30008956:	b348      	cbz	r0, 300089ac <DDR_PHY_READ_LEVELING+0x5c>
30008958:	28e0      	cmp	r0, #224	; 0xe0
3000895a:	d11e      	bne.n	3000899a <DDR_PHY_READ_LEVELING+0x4a>
3000895c:	4b34      	ldr	r3, [pc, #208]	; (30008a30 <DDR_PHY_READ_LEVELING+0xe0>)
3000895e:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
30008962:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008966:	220f      	movs	r2, #15
30008968:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
3000896c:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
30008970:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
30008974:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
30008978:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
3000897c:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
30008980:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
30008984:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
30008988:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
3000898c:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30008990:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30008994:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
30008998:	bd38      	pop	{r3, r4, r5, pc}
3000899a:	2802      	cmp	r0, #2
3000899c:	d0de      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
3000899e:	2805      	cmp	r0, #5
300089a0:	d0dc      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089a2:	2803      	cmp	r0, #3
300089a4:	d0da      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089a6:	280a      	cmp	r0, #10
300089a8:	d0d8      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089aa:	e01f      	b.n	300089ec <DDR_PHY_READ_LEVELING+0x9c>
300089ac:	4d21      	ldr	r5, [pc, #132]	; (30008a34 <DDR_PHY_READ_LEVELING+0xe4>)
300089ae:	47a8      	blx	r5
300089b0:	28e0      	cmp	r0, #224	; 0xe0
300089b2:	7020      	strb	r0, [r4, #0]
300089b4:	d0d2      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089b6:	2800      	cmp	r0, #0
300089b8:	d1ef      	bne.n	3000899a <DDR_PHY_READ_LEVELING+0x4a>
300089ba:	47a8      	blx	r5
300089bc:	2802      	cmp	r0, #2
300089be:	7020      	strb	r0, [r4, #0]
300089c0:	d0cc      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089c2:	2800      	cmp	r0, #0
300089c4:	d1eb      	bne.n	3000899e <DDR_PHY_READ_LEVELING+0x4e>
300089c6:	47a8      	blx	r5
300089c8:	2805      	cmp	r0, #5
300089ca:	7020      	strb	r0, [r4, #0]
300089cc:	d0c6      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089ce:	2800      	cmp	r0, #0
300089d0:	d1e7      	bne.n	300089a2 <DDR_PHY_READ_LEVELING+0x52>
300089d2:	47a8      	blx	r5
300089d4:	2803      	cmp	r0, #3
300089d6:	7020      	strb	r0, [r4, #0]
300089d8:	d0c0      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089da:	2800      	cmp	r0, #0
300089dc:	d1e3      	bne.n	300089a6 <DDR_PHY_READ_LEVELING+0x56>
300089de:	47a8      	blx	r5
300089e0:	280a      	cmp	r0, #10
300089e2:	7020      	strb	r0, [r4, #0]
300089e4:	d0ba      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089e6:	b908      	cbnz	r0, 300089ec <DDR_PHY_READ_LEVELING+0x9c>
300089e8:	47a8      	blx	r5
300089ea:	7020      	strb	r0, [r4, #0]
300089ec:	2809      	cmp	r0, #9
300089ee:	d0b5      	beq.n	3000895c <DDR_PHY_READ_LEVELING+0xc>
300089f0:	4b0f      	ldr	r3, [pc, #60]	; (30008a30 <DDR_PHY_READ_LEVELING+0xe0>)
300089f2:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300089f6:	2110      	movs	r1, #16
300089f8:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300089fc:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30008a00:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30008a04:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30008a08:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30008a0c:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30008a10:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30008a14:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30008a18:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30008a1c:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30008a20:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30008a24:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30008a28:	bd38      	pop	{r3, r4, r5, pc}
30008a2a:	bf00      	nop
30008a2c:	3000ea70 	.word	0x3000ea70
30008a30:	41011000 	.word	0x41011000
30008a34:	30007e69 	.word	0x30007e69

30008a38 <DDR_PHY_Init>:
30008a38:	b510      	push	{r4, lr}
30008a3a:	6d80      	ldr	r0, [r0, #88]	; 0x58
30008a3c:	4c2f      	ldr	r4, [pc, #188]	; (30008afc <DDR_PHY_Init+0xc4>)
30008a3e:	f7ff fb13 	bl	30008068 <DDR_PHY_DLL_CLK_DIV>
30008a42:	f7ff fd07 	bl	30008454 <DDR_PHY_CRT_Init>
30008a46:	f7ff fd6d 	bl	30008524 <DDR_PHY_R240_ZQ_CAL>
30008a4a:	f7ff fe99 	bl	30008780 <DDR_PHY_ZQ_SET_SEL>
30008a4e:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30008a52:	2300      	movs	r3, #0
30008a54:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30008a58:	4829      	ldr	r0, [pc, #164]	; (30008b00 <DDR_PHY_Init+0xc8>)
30008a5a:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30008a5e:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
30008a62:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
30008a66:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
30008a6a:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30008a6e:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
30008a72:	f7ff fed9 	bl	30008828 <DDR_PHY_DELAY_TAP_SET>
30008a76:	f7ff ff0f 	bl	30008898 <DDR_PHY_READ_CTRL>
30008a7a:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30008a7e:	4a21      	ldr	r2, [pc, #132]	; (30008b04 <DDR_PHY_Init+0xcc>)
30008a80:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
30008a84:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008a88:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30008a8c:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008a90:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30008a94:	f7ff ff5c 	bl	30008950 <DDR_PHY_READ_LEVELING>
30008a98:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30008a9c:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008aa0:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30008aa4:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008aa8:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30008aac:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30008ab0:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30008ab4:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30008ab8:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30008abc:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30008ac0:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30008ac4:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008ac8:	f043 0303 	orr.w	r3, r3, #3
30008acc:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008ad0:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008ad4:	f043 030c 	orr.w	r3, r3, #12
30008ad8:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008adc:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008ae0:	f023 0303 	bic.w	r3, r3, #3
30008ae4:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008ae8:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008aec:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
30008af0:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30008af4:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008af8:	bd10      	pop	{r4, pc}
30008afa:	bf00      	nop
30008afc:	41011000 	.word	0x41011000
30008b00:	00045500 	.word	0x00045500
30008b04:	11117777 	.word	0x11117777

30008b08 <DDR_PHY_CAL>:
30008b08:	4770      	bx	lr
30008b0a:	bf00      	nop

30008b0c <DDR_PHY_AutoGating>:
30008b0c:	4a0a      	ldr	r2, [pc, #40]	; (30008b38 <DDR_PHY_AutoGating+0x2c>)
30008b0e:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30008b12:	6893      	ldr	r3, [r2, #8]
30008b14:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30008b18:	6093      	str	r3, [r2, #8]
30008b1a:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30008b1e:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30008b22:	f043 0314 	orr.w	r3, r3, #20
30008b26:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008b2a:	f8d1 31e0 	ldr.w	r3, [r1, #480]	; 0x1e0
30008b2e:	f043 0303 	orr.w	r3, r3, #3
30008b32:	f8c1 31e0 	str.w	r3, [r1, #480]	; 0x1e0
30008b36:	4770      	bx	lr
30008b38:	41011000 	.word	0x41011000

30008b3c <flash_init_userdef>:
30008b3c:	4b14      	ldr	r3, [pc, #80]	; (30008b90 <flash_init_userdef+0x54>)
30008b3e:	2201      	movs	r2, #1
30008b40:	2140      	movs	r1, #64	; 0x40
30008b42:	f44f 6000 	mov.w	r0, #2048	; 0x800
30008b46:	b430      	push	{r4, r5}
30008b48:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008b4c:	2502      	movs	r5, #2
30008b4e:	2400      	movs	r4, #0
30008b50:	f640 0104 	movw	r1, #2052	; 0x804
30008b54:	601a      	str	r2, [r3, #0]
30008b56:	22bb      	movs	r2, #187	; 0xbb
30008b58:	83d8      	strh	r0, [r3, #30]
30008b5a:	8419      	strh	r1, [r3, #32]
30008b5c:	203b      	movs	r0, #59	; 0x3b
30008b5e:	21eb      	movs	r1, #235	; 0xeb
30008b60:	629a      	str	r2, [r3, #40]	; 0x28
30008b62:	4a0c      	ldr	r2, [pc, #48]	; (30008b94 <flash_init_userdef+0x58>)
30008b64:	6258      	str	r0, [r3, #36]	; 0x24
30008b66:	6319      	str	r1, [r3, #48]	; 0x30
30008b68:	f64a 3120 	movw	r1, #43808	; 0xab20
30008b6c:	655a      	str	r2, [r3, #84]	; 0x54
30008b6e:	22b9      	movs	r2, #185	; 0xb9
30008b70:	4809      	ldr	r0, [pc, #36]	; (30008b98 <flash_init_userdef+0x5c>)
30008b72:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30008b76:	6598      	str	r0, [r3, #88]	; 0x58
30008b78:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008b7c:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008b80:	2406      	movs	r4, #6
30008b82:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30008b86:	246b      	movs	r4, #107	; 0x6b
30008b88:	62dc      	str	r4, [r3, #44]	; 0x2c
30008b8a:	bc30      	pop	{r4, r5}
30008b8c:	4770      	bx	lr
30008b8e:	bf00      	nop
30008b90:	2001c01c 	.word	0x2001c01c
30008b94:	00059f06 	.word	0x00059f06
30008b98:	d8600001 	.word	0xd8600001

30008b9c <nand_init_userdef>:
30008b9c:	4b18      	ldr	r3, [pc, #96]	; (30008c00 <nand_init_userdef+0x64>)
30008b9e:	2001      	movs	r0, #1
30008ba0:	2200      	movs	r2, #0
30008ba2:	2102      	movs	r1, #2
30008ba4:	b410      	push	{r4}
30008ba6:	6018      	str	r0, [r3, #0]
30008ba8:	f640 0408 	movw	r4, #2056	; 0x808
30008bac:	6159      	str	r1, [r3, #20]
30008bae:	83dc      	strh	r4, [r3, #30]
30008bb0:	f44f 6400 	mov.w	r4, #2048	; 0x800
30008bb4:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30008bb8:	841c      	strh	r4, [r3, #32]
30008bba:	246b      	movs	r4, #107	; 0x6b
30008bbc:	619a      	str	r2, [r3, #24]
30008bbe:	62dc      	str	r4, [r3, #44]	; 0x2c
30008bc0:	4c10      	ldr	r4, [pc, #64]	; (30008c04 <nand_init_userdef+0x68>)
30008bc2:	775a      	strb	r2, [r3, #29]
30008bc4:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30008bc8:	629a      	str	r2, [r3, #40]	; 0x28
30008bca:	631a      	str	r2, [r3, #48]	; 0x30
30008bcc:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30008bd0:	65da      	str	r2, [r3, #92]	; 0x5c
30008bd2:	e9c3 0003 	strd	r0, r0, [r3, #12]
30008bd6:	203b      	movs	r0, #59	; 0x3b
30008bd8:	6258      	str	r0, [r3, #36]	; 0x24
30008bda:	200b      	movs	r0, #11
30008bdc:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30008be0:	6698      	str	r0, [r3, #104]	; 0x68
30008be2:	2008      	movs	r0, #8
30008be4:	4908      	ldr	r1, [pc, #32]	; (30008c08 <nand_init_userdef+0x6c>)
30008be6:	64d8      	str	r0, [r3, #76]	; 0x4c
30008be8:	f241 0013 	movw	r0, #4115	; 0x1013
30008bec:	6599      	str	r1, [r3, #88]	; 0x58
30008bee:	4907      	ldr	r1, [pc, #28]	; (30008c0c <nand_init_userdef+0x70>)
30008bf0:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30008bf4:	6719      	str	r1, [r3, #112]	; 0x70
30008bf6:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30008bfa:	f85d 4b04 	ldr.w	r4, [sp], #4
30008bfe:	4770      	bx	lr
30008c00:	2001c01c 	.word	0x2001c01c
30008c04:	000f9f06 	.word	0x000f9f06
30008c08:	d800001f 	.word	0xd800001f
30008c0c:	10300804 	.word	0x10300804

30008c10 <flash_layout_init>:
30008c10:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008c14:	4b65      	ldr	r3, [pc, #404]	; (30008dac <flash_layout_init+0x19c>)
30008c16:	b08f      	sub	sp, #60	; 0x3c
30008c18:	4798      	blx	r3
30008c1a:	4b65      	ldr	r3, [pc, #404]	; (30008db0 <flash_layout_init+0x1a0>)
30008c1c:	4a65      	ldr	r2, [pc, #404]	; (30008db4 <flash_layout_init+0x1a4>)
30008c1e:	2800      	cmp	r0, #0
30008c20:	bf08      	it	eq
30008c22:	4613      	moveq	r3, r2
30008c24:	681f      	ldr	r7, [r3, #0]
30008c26:	2fff      	cmp	r7, #255	; 0xff
30008c28:	d05a      	beq.n	30008ce0 <flash_layout_init+0xd0>
30008c2a:	f8df b19c 	ldr.w	fp, [pc, #412]	; 30008dc8 <flash_layout_init+0x1b8>
30008c2e:	f04f 0900 	mov.w	r9, #0
30008c32:	4a61      	ldr	r2, [pc, #388]	; (30008db8 <flash_layout_init+0x1a8>)
30008c34:	f8db 1000 	ldr.w	r1, [fp]
30008c38:	46c8      	mov	r8, r9
30008c3a:	f8df e190 	ldr.w	lr, [pc, #400]	; 30008dcc <flash_layout_init+0x1bc>
30008c3e:	464e      	mov	r6, r9
30008c40:	910d      	str	r1, [sp, #52]	; 0x34
30008c42:	464c      	mov	r4, r9
30008c44:	495d      	ldr	r1, [pc, #372]	; (30008dbc <flash_layout_init+0x1ac>)
30008c46:	6812      	ldr	r2, [r2, #0]
30008c48:	6809      	ldr	r1, [r1, #0]
30008c4a:	f8df c184 	ldr.w	ip, [pc, #388]	; 30008dd0 <flash_layout_init+0x1c0>
30008c4e:	9109      	str	r1, [sp, #36]	; 0x24
30008c50:	495b      	ldr	r1, [pc, #364]	; (30008dc0 <flash_layout_init+0x1b0>)
30008c52:	920c      	str	r2, [sp, #48]	; 0x30
30008c54:	6809      	ldr	r1, [r1, #0]
30008c56:	4a5b      	ldr	r2, [pc, #364]	; (30008dc4 <flash_layout_init+0x1b4>)
30008c58:	910b      	str	r1, [sp, #44]	; 0x2c
30008c5a:	f8de 1000 	ldr.w	r1, [lr]
30008c5e:	f8cd 9028 	str.w	r9, [sp, #40]	; 0x28
30008c62:	9107      	str	r1, [sp, #28]
30008c64:	f89c 1000 	ldrb.w	r1, [ip]
30008c68:	f8cd 9018 	str.w	r9, [sp, #24]
30008c6c:	9108      	str	r1, [sp, #32]
30008c6e:	6951      	ldr	r1, [r2, #20]
30008c70:	f8cd 9010 	str.w	r9, [sp, #16]
30008c74:	9105      	str	r1, [sp, #20]
30008c76:	6911      	ldr	r1, [r2, #16]
30008c78:	f8cd 9008 	str.w	r9, [sp, #8]
30008c7c:	9103      	str	r1, [sp, #12]
30008c7e:	68d1      	ldr	r1, [r2, #12]
30008c80:	9101      	str	r1, [sp, #4]
30008c82:	6891      	ldr	r1, [r2, #8]
30008c84:	e9d2 0500 	ldrd	r0, r5, [r2]
30008c88:	9100      	str	r1, [sp, #0]
30008c8a:	4649      	mov	r1, r9
30008c8c:	2f00      	cmp	r7, #0
30008c8e:	d12a      	bne.n	30008ce6 <flash_layout_init+0xd6>
30008c90:	6858      	ldr	r0, [r3, #4]
30008c92:	2101      	movs	r1, #1
30008c94:	f853 7f0c 	ldr.w	r7, [r3, #12]!
30008c98:	2fff      	cmp	r7, #255	; 0xff
30008c9a:	d1f7      	bne.n	30008c8c <flash_layout_init+0x7c>
30008c9c:	b101      	cbz	r1, 30008ca0 <flash_layout_init+0x90>
30008c9e:	6010      	str	r0, [r2, #0]
30008ca0:	b104      	cbz	r4, 30008ca4 <flash_layout_init+0x94>
30008ca2:	6055      	str	r5, [r2, #4]
30008ca4:	b10e      	cbz	r6, 30008caa <flash_layout_init+0x9a>
30008ca6:	9b00      	ldr	r3, [sp, #0]
30008ca8:	6093      	str	r3, [r2, #8]
30008caa:	f1b8 0f00 	cmp.w	r8, #0
30008cae:	d001      	beq.n	30008cb4 <flash_layout_init+0xa4>
30008cb0:	9b01      	ldr	r3, [sp, #4]
30008cb2:	60d3      	str	r3, [r2, #12]
30008cb4:	9b02      	ldr	r3, [sp, #8]
30008cb6:	b10b      	cbz	r3, 30008cbc <flash_layout_init+0xac>
30008cb8:	9b03      	ldr	r3, [sp, #12]
30008cba:	6113      	str	r3, [r2, #16]
30008cbc:	9b04      	ldr	r3, [sp, #16]
30008cbe:	2b00      	cmp	r3, #0
30008cc0:	d169      	bne.n	30008d96 <flash_layout_init+0x186>
30008cc2:	9b06      	ldr	r3, [sp, #24]
30008cc4:	2b00      	cmp	r3, #0
30008cc6:	d15f      	bne.n	30008d88 <flash_layout_init+0x178>
30008cc8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008cca:	2b00      	cmp	r3, #0
30008ccc:	d166      	bne.n	30008d9c <flash_layout_init+0x18c>
30008cce:	f1b9 0f00 	cmp.w	r9, #0
30008cd2:	d005      	beq.n	30008ce0 <flash_layout_init+0xd0>
30008cd4:	4b38      	ldr	r3, [pc, #224]	; (30008db8 <flash_layout_init+0x1a8>)
30008cd6:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30008cd8:	601a      	str	r2, [r3, #0]
30008cda:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30008cdc:	f8cb 3000 	str.w	r3, [fp]
30008ce0:	b00f      	add	sp, #60	; 0x3c
30008ce2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008ce6:	2f01      	cmp	r7, #1
30008ce8:	d01a      	beq.n	30008d20 <flash_layout_init+0x110>
30008cea:	2f02      	cmp	r7, #2
30008cec:	d01b      	beq.n	30008d26 <flash_layout_init+0x116>
30008cee:	2f03      	cmp	r7, #3
30008cf0:	d01d      	beq.n	30008d2e <flash_layout_init+0x11e>
30008cf2:	2f04      	cmp	r7, #4
30008cf4:	d020      	beq.n	30008d38 <flash_layout_init+0x128>
30008cf6:	2f05      	cmp	r7, #5
30008cf8:	d023      	beq.n	30008d42 <flash_layout_init+0x132>
30008cfa:	2f06      	cmp	r7, #6
30008cfc:	d026      	beq.n	30008d4c <flash_layout_init+0x13c>
30008cfe:	2f07      	cmp	r7, #7
30008d00:	d035      	beq.n	30008d6e <flash_layout_init+0x15e>
30008d02:	2f08      	cmp	r7, #8
30008d04:	d1c6      	bne.n	30008c94 <flash_layout_init+0x84>
30008d06:	689f      	ldr	r7, [r3, #8]
30008d08:	f8d3 9004 	ldr.w	r9, [r3, #4]
30008d0c:	3701      	adds	r7, #1
30008d0e:	eba7 0709 	sub.w	r7, r7, r9
30008d12:	970d      	str	r7, [sp, #52]	; 0x34
30008d14:	f109 4778 	add.w	r7, r9, #4160749568	; 0xf8000000
30008d18:	f04f 0901 	mov.w	r9, #1
30008d1c:	970c      	str	r7, [sp, #48]	; 0x30
30008d1e:	e7b9      	b.n	30008c94 <flash_layout_init+0x84>
30008d20:	463c      	mov	r4, r7
30008d22:	685d      	ldr	r5, [r3, #4]
30008d24:	e7b6      	b.n	30008c94 <flash_layout_init+0x84>
30008d26:	685e      	ldr	r6, [r3, #4]
30008d28:	9600      	str	r6, [sp, #0]
30008d2a:	2601      	movs	r6, #1
30008d2c:	e7b2      	b.n	30008c94 <flash_layout_init+0x84>
30008d2e:	685f      	ldr	r7, [r3, #4]
30008d30:	f04f 0801 	mov.w	r8, #1
30008d34:	9701      	str	r7, [sp, #4]
30008d36:	e7ad      	b.n	30008c94 <flash_layout_init+0x84>
30008d38:	685f      	ldr	r7, [r3, #4]
30008d3a:	9703      	str	r7, [sp, #12]
30008d3c:	2701      	movs	r7, #1
30008d3e:	9702      	str	r7, [sp, #8]
30008d40:	e7a8      	b.n	30008c94 <flash_layout_init+0x84>
30008d42:	685f      	ldr	r7, [r3, #4]
30008d44:	9705      	str	r7, [sp, #20]
30008d46:	2701      	movs	r7, #1
30008d48:	9704      	str	r7, [sp, #16]
30008d4a:	e7a3      	b.n	30008c94 <flash_layout_init+0x84>
30008d4c:	689f      	ldr	r7, [r3, #8]
30008d4e:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008d52:	3701      	adds	r7, #1
30008d54:	eba7 070a 	sub.w	r7, r7, sl
30008d58:	9706      	str	r7, [sp, #24]
30008d5a:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008d5e:	9707      	str	r7, [sp, #28]
30008d60:	9f06      	ldr	r7, [sp, #24]
30008d62:	f3c7 3707 	ubfx	r7, r7, #12, #8
30008d66:	9708      	str	r7, [sp, #32]
30008d68:	2701      	movs	r7, #1
30008d6a:	9706      	str	r7, [sp, #24]
30008d6c:	e792      	b.n	30008c94 <flash_layout_init+0x84>
30008d6e:	689f      	ldr	r7, [r3, #8]
30008d70:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008d74:	3701      	adds	r7, #1
30008d76:	eba7 070a 	sub.w	r7, r7, sl
30008d7a:	970b      	str	r7, [sp, #44]	; 0x2c
30008d7c:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008d80:	9709      	str	r7, [sp, #36]	; 0x24
30008d82:	2701      	movs	r7, #1
30008d84:	970a      	str	r7, [sp, #40]	; 0x28
30008d86:	e785      	b.n	30008c94 <flash_layout_init+0x84>
30008d88:	9b07      	ldr	r3, [sp, #28]
30008d8a:	f8ce 3000 	str.w	r3, [lr]
30008d8e:	9b08      	ldr	r3, [sp, #32]
30008d90:	f88c 3000 	strb.w	r3, [ip]
30008d94:	e798      	b.n	30008cc8 <flash_layout_init+0xb8>
30008d96:	9b05      	ldr	r3, [sp, #20]
30008d98:	6153      	str	r3, [r2, #20]
30008d9a:	e792      	b.n	30008cc2 <flash_layout_init+0xb2>
30008d9c:	4b07      	ldr	r3, [pc, #28]	; (30008dbc <flash_layout_init+0x1ac>)
30008d9e:	9a09      	ldr	r2, [sp, #36]	; 0x24
30008da0:	601a      	str	r2, [r3, #0]
30008da2:	4b07      	ldr	r3, [pc, #28]	; (30008dc0 <flash_layout_init+0x1b0>)
30008da4:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30008da6:	601a      	str	r2, [r3, #0]
30008da8:	e791      	b.n	30008cce <flash_layout_init+0xbe>
30008daa:	bf00      	nop
30008dac:	3000a501 	.word	0x3000a501
30008db0:	3000e444 	.word	0x3000e444
30008db4:	3000e3cc 	.word	0x3000e3cc
30008db8:	3000ea94 	.word	0x3000ea94
30008dbc:	3000ea8c 	.word	0x3000ea8c
30008dc0:	3000ea90 	.word	0x3000ea90
30008dc4:	3000ea74 	.word	0x3000ea74
30008dc8:	3000ea98 	.word	0x3000ea98
30008dcc:	3000ea9c 	.word	0x3000ea9c
30008dd0:	3000e4a4 	.word	0x3000e4a4

30008dd4 <NAND_CHECK_IS_BAD_BLOCK>:
30008dd4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008dd8:	4e25      	ldr	r6, [pc, #148]	; (30008e70 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008dda:	0907      	lsrs	r7, r0, #4
30008ddc:	0045      	lsls	r5, r0, #1
30008dde:	b083      	sub	sp, #12
30008de0:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008de4:	f005 051e 	and.w	r5, r5, #30
30008de8:	40eb      	lsrs	r3, r5
30008dea:	f003 0303 	and.w	r3, r3, #3
30008dee:	2b02      	cmp	r3, #2
30008df0:	d022      	beq.n	30008e38 <NAND_CHECK_IS_BAD_BLOCK+0x64>
30008df2:	2b01      	cmp	r3, #1
30008df4:	d024      	beq.n	30008e40 <NAND_CHECK_IS_BAD_BLOCK+0x6c>
30008df6:	4b1f      	ldr	r3, [pc, #124]	; (30008e74 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30008df8:	f04f 0801 	mov.w	r8, #1
30008dfc:	2204      	movs	r2, #4
30008dfe:	4604      	mov	r4, r0
30008e00:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008e04:	0180      	lsls	r0, r0, #6
30008e06:	ab01      	add	r3, sp, #4
30008e08:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30008e80 <NAND_CHECK_IS_BAD_BLOCK+0xac>
30008e0c:	fa08 f101 	lsl.w	r1, r8, r1
30008e10:	47c8      	blx	r9
30008e12:	f89d 3004 	ldrb.w	r3, [sp, #4]
30008e16:	f89d 2005 	ldrb.w	r2, [sp, #5]
30008e1a:	4013      	ands	r3, r2
30008e1c:	2bff      	cmp	r3, #255	; 0xff
30008e1e:	d013      	beq.n	30008e48 <NAND_CHECK_IS_BAD_BLOCK+0x74>
30008e20:	2302      	movs	r3, #2
30008e22:	fa03 f505 	lsl.w	r5, r3, r5
30008e26:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008e2a:	432b      	orrs	r3, r5
30008e2c:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008e30:	4b11      	ldr	r3, [pc, #68]	; (30008e78 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30008e32:	681b      	ldr	r3, [r3, #0]
30008e34:	079b      	lsls	r3, r3, #30
30008e36:	d412      	bmi.n	30008e5e <NAND_CHECK_IS_BAD_BLOCK+0x8a>
30008e38:	2001      	movs	r0, #1
30008e3a:	b003      	add	sp, #12
30008e3c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008e40:	2000      	movs	r0, #0
30008e42:	b003      	add	sp, #12
30008e44:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008e48:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008e4c:	fa08 f005 	lsl.w	r0, r8, r5
30008e50:	4303      	orrs	r3, r0
30008e52:	2000      	movs	r0, #0
30008e54:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008e58:	b003      	add	sp, #12
30008e5a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008e5e:	4621      	mov	r1, r4
30008e60:	4806      	ldr	r0, [pc, #24]	; (30008e7c <NAND_CHECK_IS_BAD_BLOCK+0xa8>)
30008e62:	f003 f9a9 	bl	3000c1b8 <__DiagPrintf_veneer>
30008e66:	4640      	mov	r0, r8
30008e68:	b003      	add	sp, #12
30008e6a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008e6e:	bf00      	nop
30008e70:	3000eaa8 	.word	0x3000eaa8
30008e74:	2001c01c 	.word	0x2001c01c
30008e78:	2001c00c 	.word	0x2001c00c
30008e7c:	3000d3a0 	.word	0x3000d3a0
30008e80:	3000a459 	.word	0x3000a459

30008e84 <Nand_Get_NandAddr>:
30008e84:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008e88:	4f10      	ldr	r7, [pc, #64]	; (30008ecc <Nand_Get_NandAddr+0x48>)
30008e8a:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008e8e:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30008e92:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30008e96:	40e8      	lsrs	r0, r5
30008e98:	fa26 f505 	lsr.w	r5, r6, r5
30008e9c:	09ad      	lsrs	r5, r5, #6
30008e9e:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30008ea2:	d90f      	bls.n	30008ec4 <Nand_Get_NandAddr+0x40>
30008ea4:	0984      	lsrs	r4, r0, #6
30008ea6:	f04f 0840 	mov.w	r8, #64	; 0x40
30008eaa:	4620      	mov	r0, r4
30008eac:	3401      	adds	r4, #1
30008eae:	f7ff ff91 	bl	30008dd4 <NAND_CHECK_IS_BAD_BLOCK>
30008eb2:	b128      	cbz	r0, 30008ec0 <Nand_Get_NandAddr+0x3c>
30008eb4:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008eb8:	3501      	adds	r5, #1
30008eba:	fa08 f303 	lsl.w	r3, r8, r3
30008ebe:	441e      	add	r6, r3
30008ec0:	42a5      	cmp	r5, r4
30008ec2:	d8f2      	bhi.n	30008eaa <Nand_Get_NandAddr+0x26>
30008ec4:	4630      	mov	r0, r6
30008ec6:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008eca:	bf00      	nop
30008ecc:	2001c01c 	.word	0x2001c01c

30008ed0 <NandImgCopy>:
30008ed0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008ed4:	4b56      	ldr	r3, [pc, #344]	; (30009030 <NandImgCopy+0x160>)
30008ed6:	b085      	sub	sp, #20
30008ed8:	4617      	mov	r7, r2
30008eda:	9003      	str	r0, [sp, #12]
30008edc:	6898      	ldr	r0, [r3, #8]
30008ede:	4281      	cmp	r1, r0
30008ee0:	f0c0 8089 	bcc.w	30008ff6 <NandImgCopy+0x126>
30008ee4:	691b      	ldr	r3, [r3, #16]
30008ee6:	4299      	cmp	r1, r3
30008ee8:	d37b      	bcc.n	30008fe2 <NandImgCopy+0x112>
30008eea:	f8df 9154 	ldr.w	r9, [pc, #340]	; 30009040 <NandImgCopy+0x170>
30008eee:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30008ef2:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008ef6:	fa25 f406 	lsr.w	r4, r5, r6
30008efa:	f107 38ff 	add.w	r8, r7, #4294967295
30008efe:	f1c4 0301 	rsb	r3, r4, #1
30008f02:	44a8      	add	r8, r5
30008f04:	fa28 f806 	lsr.w	r8, r8, r6
30008f08:	eb18 0803 	adds.w	r8, r8, r3
30008f0c:	d05e      	beq.n	30008fcc <NandImgCopy+0xfc>
30008f0e:	f108 38ff 	add.w	r8, r8, #4294967295
30008f12:	463b      	mov	r3, r7
30008f14:	f04f 0a00 	mov.w	sl, #0
30008f18:	f04f 0b40 	mov.w	fp, #64	; 0x40
30008f1c:	4647      	mov	r7, r8
30008f1e:	4698      	mov	r8, r3
30008f20:	f8cd a008 	str.w	sl, [sp, #8]
30008f24:	e00a      	b.n	30008f3c <NandImgCopy+0x6c>
30008f26:	1ac9      	subs	r1, r1, r3
30008f28:	4b42      	ldr	r3, [pc, #264]	; (30009034 <NandImgCopy+0x164>)
30008f2a:	689b      	ldr	r3, [r3, #8]
30008f2c:	4488      	add	r8, r1
30008f2e:	079a      	lsls	r2, r3, #30
30008f30:	d44f      	bmi.n	30008fd2 <NandImgCopy+0x102>
30008f32:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008f36:	3f01      	subs	r7, #1
30008f38:	fa25 f406 	lsr.w	r4, r5, r6
30008f3c:	9b02      	ldr	r3, [sp, #8]
30008f3e:	4632      	mov	r2, r6
30008f40:	09a6      	lsrs	r6, r4, #6
30008f42:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008f46:	d922      	bls.n	30008f8e <NandImgCopy+0xbe>
30008f48:	f1c2 0320 	rsb	r3, r2, #32
30008f4c:	f04f 31ff 	mov.w	r1, #4294967295
30008f50:	40d9      	lsrs	r1, r3
30008f52:	9b03      	ldr	r3, [sp, #12]
30008f54:	4029      	ands	r1, r5
30008f56:	4453      	add	r3, sl
30008f58:	b31f      	cbz	r7, 30008fa2 <NandImgCopy+0xd2>
30008f5a:	2601      	movs	r6, #1
30008f5c:	4620      	mov	r0, r4
30008f5e:	9101      	str	r1, [sp, #4]
30008f60:	fa06 f202 	lsl.w	r2, r6, r2
30008f64:	4e34      	ldr	r6, [pc, #208]	; (30009038 <NandImgCopy+0x168>)
30008f66:	9200      	str	r2, [sp, #0]
30008f68:	1a52      	subs	r2, r2, r1
30008f6a:	4492      	add	sl, r2
30008f6c:	4415      	add	r5, r2
30008f6e:	47b0      	blx	r6
30008f70:	f010 0f70 	tst.w	r0, #112	; 0x70
30008f74:	9b00      	ldr	r3, [sp, #0]
30008f76:	9901      	ldr	r1, [sp, #4]
30008f78:	d1d5      	bne.n	30008f26 <NandImgCopy+0x56>
30008f7a:	eba8 0803 	sub.w	r8, r8, r3
30008f7e:	4488      	add	r8, r1
30008f80:	e7d7      	b.n	30008f32 <NandImgCopy+0x62>
30008f82:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008f86:	3440      	adds	r4, #64	; 0x40
30008f88:	fa0b f303 	lsl.w	r3, fp, r3
30008f8c:	441d      	add	r5, r3
30008f8e:	4630      	mov	r0, r6
30008f90:	3601      	adds	r6, #1
30008f92:	f7ff ff1f 	bl	30008dd4 <NAND_CHECK_IS_BAD_BLOCK>
30008f96:	2800      	cmp	r0, #0
30008f98:	d1f3      	bne.n	30008f82 <NandImgCopy+0xb2>
30008f9a:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008f9e:	9602      	str	r6, [sp, #8]
30008fa0:	e7d2      	b.n	30008f48 <NandImgCopy+0x78>
30008fa2:	4642      	mov	r2, r8
30008fa4:	4620      	mov	r0, r4
30008fa6:	4d24      	ldr	r5, [pc, #144]	; (30009038 <NandImgCopy+0x168>)
30008fa8:	47a8      	blx	r5
30008faa:	f010 0f70 	tst.w	r0, #112	; 0x70
30008fae:	d00d      	beq.n	30008fcc <NandImgCopy+0xfc>
30008fb0:	4b20      	ldr	r3, [pc, #128]	; (30009034 <NandImgCopy+0x164>)
30008fb2:	689b      	ldr	r3, [r3, #8]
30008fb4:	079b      	lsls	r3, r3, #30
30008fb6:	d509      	bpl.n	30008fcc <NandImgCopy+0xfc>
30008fb8:	4603      	mov	r3, r0
30008fba:	09a2      	lsrs	r2, r4, #6
30008fbc:	f004 013f 	and.w	r1, r4, #63	; 0x3f
30008fc0:	481e      	ldr	r0, [pc, #120]	; (3000903c <NandImgCopy+0x16c>)
30008fc2:	b005      	add	sp, #20
30008fc4:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008fc8:	f003 b8f6 	b.w	3000c1b8 <__DiagPrintf_veneer>
30008fcc:	b005      	add	sp, #20
30008fce:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008fd2:	4603      	mov	r3, r0
30008fd4:	09a2      	lsrs	r2, r4, #6
30008fd6:	f004 013f 	and.w	r1, r4, #63	; 0x3f
30008fda:	4818      	ldr	r0, [pc, #96]	; (3000903c <NandImgCopy+0x16c>)
30008fdc:	f003 f8ec 	bl	3000c1b8 <__DiagPrintf_veneer>
30008fe0:	e7a7      	b.n	30008f32 <NandImgCopy+0x62>
30008fe2:	f8df 905c 	ldr.w	r9, [pc, #92]	; 30009040 <NandImgCopy+0x170>
30008fe6:	f7ff ff4d 	bl	30008e84 <Nand_Get_NandAddr>
30008fea:	4605      	mov	r5, r0
30008fec:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008ff0:	fa20 f406 	lsr.w	r4, r0, r6
30008ff4:	e781      	b.n	30008efa <NandImgCopy+0x2a>
30008ff6:	f8df 9048 	ldr.w	r9, [pc, #72]	; 30009040 <NandImgCopy+0x170>
30008ffa:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30008ffe:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30009002:	fa25 f406 	lsr.w	r4, r5, r6
30009006:	ea5f 1894 	movs.w	r8, r4, lsr #6
3000900a:	f43f af76 	beq.w	30008efa <NandImgCopy+0x2a>
3000900e:	2400      	movs	r4, #0
30009010:	2640      	movs	r6, #64	; 0x40
30009012:	4620      	mov	r0, r4
30009014:	3401      	adds	r4, #1
30009016:	f7ff fedd 	bl	30008dd4 <NAND_CHECK_IS_BAD_BLOCK>
3000901a:	b130      	cbz	r0, 3000902a <NandImgCopy+0x15a>
3000901c:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30009020:	f108 0801 	add.w	r8, r8, #1
30009024:	fa06 f303 	lsl.w	r3, r6, r3
30009028:	441d      	add	r5, r3
3000902a:	4544      	cmp	r4, r8
3000902c:	d3f1      	bcc.n	30009012 <NandImgCopy+0x142>
3000902e:	e760      	b.n	30008ef2 <NandImgCopy+0x22>
30009030:	3000edc8 	.word	0x3000edc8
30009034:	2001c00c 	.word	0x2001c00c
30009038:	3000a459 	.word	0x3000a459
3000903c:	3000d3cc 	.word	0x3000d3cc
30009040:	2001c01c 	.word	0x2001c01c

30009044 <OTP_Read8.part.0>:
30009044:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30009048:	4b2f      	ldr	r3, [pc, #188]	; (30009108 <OTP_Read8.part.0+0xc4>)
3000904a:	4680      	mov	r8, r0
3000904c:	e843 f300 	tt	r3, r3
30009050:	f3c3 5380 	ubfx	r3, r3, #22, #1
30009054:	4a2d      	ldr	r2, [pc, #180]	; (3000910c <OTP_Read8.part.0+0xc8>)
30009056:	4e2e      	ldr	r6, [pc, #184]	; (30009110 <OTP_Read8.part.0+0xcc>)
30009058:	4689      	mov	r9, r1
3000905a:	2b00      	cmp	r3, #0
3000905c:	bf0e      	itee	eq
3000905e:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30009062:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30009066:	4616      	movne	r6, r2
30009068:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000906a:	05d8      	lsls	r0, r3, #23
3000906c:	d505      	bpl.n	3000907a <OTP_Read8.part.0+0x36>
3000906e:	4d29      	ldr	r5, [pc, #164]	; (30009114 <OTP_Read8.part.0+0xd0>)
30009070:	2064      	movs	r0, #100	; 0x64
30009072:	47a8      	blx	r5
30009074:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009076:	05d9      	lsls	r1, r3, #23
30009078:	d4fa      	bmi.n	30009070 <OTP_Read8.part.0+0x2c>
3000907a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000907c:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30009080:	643b      	str	r3, [r7, #64]	; 0x40
30009082:	4b25      	ldr	r3, [pc, #148]	; (30009118 <OTP_Read8.part.0+0xd4>)
30009084:	681a      	ldr	r2, [r3, #0]
30009086:	07d2      	lsls	r2, r2, #31
30009088:	d403      	bmi.n	30009092 <OTP_Read8.part.0+0x4e>
3000908a:	681a      	ldr	r2, [r3, #0]
3000908c:	f042 0201 	orr.w	r2, r2, #1
30009090:	601a      	str	r2, [r3, #0]
30009092:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30009096:	ea4f 2308 	mov.w	r3, r8, lsl #8
3000909a:	694a      	ldr	r2, [r1, #20]
3000909c:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
300090a0:	614a      	str	r2, [r1, #20]
300090a2:	6033      	str	r3, [r6, #0]
300090a4:	6833      	ldr	r3, [r6, #0]
300090a6:	2b00      	cmp	r3, #0
300090a8:	db10      	blt.n	300090cc <OTP_Read8.part.0+0x88>
300090aa:	2400      	movs	r4, #0
300090ac:	4d19      	ldr	r5, [pc, #100]	; (30009114 <OTP_Read8.part.0+0xd0>)
300090ae:	f644 6a20 	movw	sl, #20000	; 0x4e20
300090b2:	e001      	b.n	300090b8 <OTP_Read8.part.0+0x74>
300090b4:	4554      	cmp	r4, sl
300090b6:	d019      	beq.n	300090ec <OTP_Read8.part.0+0xa8>
300090b8:	2005      	movs	r0, #5
300090ba:	3401      	adds	r4, #1
300090bc:	47a8      	blx	r5
300090be:	6833      	ldr	r3, [r6, #0]
300090c0:	2b00      	cmp	r3, #0
300090c2:	daf7      	bge.n	300090b4 <OTP_Read8.part.0+0x70>
300090c4:	f644 6320 	movw	r3, #20000	; 0x4e20
300090c8:	429c      	cmp	r4, r3
300090ca:	d00f      	beq.n	300090ec <OTP_Read8.part.0+0xa8>
300090cc:	6833      	ldr	r3, [r6, #0]
300090ce:	2001      	movs	r0, #1
300090d0:	f889 3000 	strb.w	r3, [r9]
300090d4:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300090d8:	6953      	ldr	r3, [r2, #20]
300090da:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300090de:	6153      	str	r3, [r2, #20]
300090e0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300090e2:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300090e6:	643b      	str	r3, [r7, #64]	; 0x40
300090e8:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300090ec:	22ff      	movs	r2, #255	; 0xff
300090ee:	4b0b      	ldr	r3, [pc, #44]	; (3000911c <OTP_Read8.part.0+0xd8>)
300090f0:	f889 2000 	strb.w	r2, [r9]
300090f4:	6818      	ldr	r0, [r3, #0]
300090f6:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
300090fa:	d0eb      	beq.n	300090d4 <OTP_Read8.part.0+0x90>
300090fc:	4641      	mov	r1, r8
300090fe:	4808      	ldr	r0, [pc, #32]	; (30009120 <OTP_Read8.part.0+0xdc>)
30009100:	f003 f85a 	bl	3000c1b8 <__DiagPrintf_veneer>
30009104:	2000      	movs	r0, #0
30009106:	e7e5      	b.n	300090d4 <OTP_Read8.part.0+0x90>
30009108:	0000e5f9 	.word	0x0000e5f9
3000910c:	5200000c 	.word	0x5200000c
30009110:	42000008 	.word	0x42000008
30009114:	00009b2d 	.word	0x00009b2d
30009118:	42008000 	.word	0x42008000
3000911c:	2001c00c 	.word	0x2001c00c
30009120:	3000d41c 	.word	0x3000d41c

30009124 <OTP_ProgramMarginRead8.part.0>:
30009124:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30009128:	4b30      	ldr	r3, [pc, #192]	; (300091ec <OTP_ProgramMarginRead8.part.0+0xc8>)
3000912a:	4680      	mov	r8, r0
3000912c:	e843 f300 	tt	r3, r3
30009130:	f3c3 5380 	ubfx	r3, r3, #22, #1
30009134:	4a2e      	ldr	r2, [pc, #184]	; (300091f0 <OTP_ProgramMarginRead8.part.0+0xcc>)
30009136:	4e2f      	ldr	r6, [pc, #188]	; (300091f4 <OTP_ProgramMarginRead8.part.0+0xd0>)
30009138:	4689      	mov	r9, r1
3000913a:	2b00      	cmp	r3, #0
3000913c:	bf0e      	itee	eq
3000913e:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30009142:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30009146:	4616      	movne	r6, r2
30009148:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000914a:	05d8      	lsls	r0, r3, #23
3000914c:	d505      	bpl.n	3000915a <OTP_ProgramMarginRead8.part.0+0x36>
3000914e:	4d2a      	ldr	r5, [pc, #168]	; (300091f8 <OTP_ProgramMarginRead8.part.0+0xd4>)
30009150:	2064      	movs	r0, #100	; 0x64
30009152:	47a8      	blx	r5
30009154:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009156:	05d9      	lsls	r1, r3, #23
30009158:	d4fa      	bmi.n	30009150 <OTP_ProgramMarginRead8.part.0+0x2c>
3000915a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000915c:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30009160:	643b      	str	r3, [r7, #64]	; 0x40
30009162:	4b26      	ldr	r3, [pc, #152]	; (300091fc <OTP_ProgramMarginRead8.part.0+0xd8>)
30009164:	681a      	ldr	r2, [r3, #0]
30009166:	07d2      	lsls	r2, r2, #31
30009168:	d403      	bmi.n	30009172 <OTP_ProgramMarginRead8.part.0+0x4e>
3000916a:	681a      	ldr	r2, [r3, #0]
3000916c:	f042 0201 	orr.w	r2, r2, #1
30009170:	601a      	str	r2, [r3, #0]
30009172:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30009176:	ea4f 2308 	mov.w	r3, r8, lsl #8
3000917a:	694a      	ldr	r2, [r1, #20]
3000917c:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
30009180:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30009184:	614a      	str	r2, [r1, #20]
30009186:	6033      	str	r3, [r6, #0]
30009188:	6833      	ldr	r3, [r6, #0]
3000918a:	2b00      	cmp	r3, #0
3000918c:	db10      	blt.n	300091b0 <OTP_ProgramMarginRead8.part.0+0x8c>
3000918e:	2400      	movs	r4, #0
30009190:	4d19      	ldr	r5, [pc, #100]	; (300091f8 <OTP_ProgramMarginRead8.part.0+0xd4>)
30009192:	f644 6a20 	movw	sl, #20000	; 0x4e20
30009196:	e001      	b.n	3000919c <OTP_ProgramMarginRead8.part.0+0x78>
30009198:	4554      	cmp	r4, sl
3000919a:	d019      	beq.n	300091d0 <OTP_ProgramMarginRead8.part.0+0xac>
3000919c:	2005      	movs	r0, #5
3000919e:	3401      	adds	r4, #1
300091a0:	47a8      	blx	r5
300091a2:	6833      	ldr	r3, [r6, #0]
300091a4:	2b00      	cmp	r3, #0
300091a6:	daf7      	bge.n	30009198 <OTP_ProgramMarginRead8.part.0+0x74>
300091a8:	f644 6320 	movw	r3, #20000	; 0x4e20
300091ac:	429c      	cmp	r4, r3
300091ae:	d00f      	beq.n	300091d0 <OTP_ProgramMarginRead8.part.0+0xac>
300091b0:	6833      	ldr	r3, [r6, #0]
300091b2:	2001      	movs	r0, #1
300091b4:	f889 3000 	strb.w	r3, [r9]
300091b8:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300091bc:	6953      	ldr	r3, [r2, #20]
300091be:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300091c2:	6153      	str	r3, [r2, #20]
300091c4:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300091c6:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300091ca:	643b      	str	r3, [r7, #64]	; 0x40
300091cc:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300091d0:	22ff      	movs	r2, #255	; 0xff
300091d2:	4b0b      	ldr	r3, [pc, #44]	; (30009200 <OTP_ProgramMarginRead8.part.0+0xdc>)
300091d4:	f889 2000 	strb.w	r2, [r9]
300091d8:	6818      	ldr	r0, [r3, #0]
300091da:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
300091de:	d0eb      	beq.n	300091b8 <OTP_ProgramMarginRead8.part.0+0x94>
300091e0:	4641      	mov	r1, r8
300091e2:	4808      	ldr	r0, [pc, #32]	; (30009204 <OTP_ProgramMarginRead8.part.0+0xe0>)
300091e4:	f002 ffe8 	bl	3000c1b8 <__DiagPrintf_veneer>
300091e8:	2000      	movs	r0, #0
300091ea:	e7e5      	b.n	300091b8 <OTP_ProgramMarginRead8.part.0+0x94>
300091ec:	0000e5f9 	.word	0x0000e5f9
300091f0:	5200000c 	.word	0x5200000c
300091f4:	42000008 	.word	0x42000008
300091f8:	00009b2d 	.word	0x00009b2d
300091fc:	42008000 	.word	0x42008000
30009200:	2001c00c 	.word	0x2001c00c
30009204:	3000d41c 	.word	0x3000d41c

30009208 <OTP_Read8>:
30009208:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
3000920c:	d201      	bcs.n	30009212 <OTP_Read8+0xa>
3000920e:	f7ff bf19 	b.w	30009044 <OTP_Read8.part.0>
30009212:	22ff      	movs	r2, #255	; 0xff
30009214:	2000      	movs	r0, #0
30009216:	700a      	strb	r2, [r1, #0]
30009218:	4770      	bx	lr
3000921a:	bf00      	nop

3000921c <OTP_Write8>:
3000921c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009220:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009224:	b085      	sub	sp, #20
30009226:	d277      	bcs.n	30009318 <OTP_Write8+0xfc>
30009228:	4689      	mov	r9, r1
3000922a:	f10d 010f 	add.w	r1, sp, #15
3000922e:	4680      	mov	r8, r0
30009230:	f7ff ff78 	bl	30009124 <OTP_ProgramMarginRead8.part.0>
30009234:	2800      	cmp	r0, #0
30009236:	d072      	beq.n	3000931e <OTP_Write8+0x102>
30009238:	ea4f 2308 	mov.w	r3, r8, lsl #8
3000923c:	2202      	movs	r2, #2
3000923e:	46ca      	mov	sl, r9
30009240:	9301      	str	r3, [sp, #4]
30009242:	f89d 300f 	ldrb.w	r3, [sp, #15]
30009246:	9200      	str	r2, [sp, #0]
30009248:	ea6a 0a03 	orn	sl, sl, r3
3000924c:	fa5f fa8a 	uxtb.w	sl, sl
30009250:	f1ba 0fff 	cmp.w	sl, #255	; 0xff
30009254:	d04f      	beq.n	300092f6 <OTP_Write8+0xda>
30009256:	4b52      	ldr	r3, [pc, #328]	; (300093a0 <OTP_Write8+0x184>)
30009258:	e843 f300 	tt	r3, r3
3000925c:	f3c3 5380 	ubfx	r3, r3, #22, #1
30009260:	4a50      	ldr	r2, [pc, #320]	; (300093a4 <OTP_Write8+0x188>)
30009262:	2b00      	cmp	r3, #0
30009264:	4b50      	ldr	r3, [pc, #320]	; (300093a8 <OTP_Write8+0x18c>)
30009266:	bf0b      	itete	eq
30009268:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
3000926c:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30009270:	461d      	moveq	r5, r3
30009272:	4615      	movne	r5, r2
30009274:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009276:	05dc      	lsls	r4, r3, #23
30009278:	d505      	bpl.n	30009286 <OTP_Write8+0x6a>
3000927a:	4c4c      	ldr	r4, [pc, #304]	; (300093ac <OTP_Write8+0x190>)
3000927c:	2064      	movs	r0, #100	; 0x64
3000927e:	47a0      	blx	r4
30009280:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009282:	05d8      	lsls	r0, r3, #23
30009284:	d4fa      	bmi.n	3000927c <OTP_Write8+0x60>
30009286:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009288:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000928c:	643b      	str	r3, [r7, #64]	; 0x40
3000928e:	4b48      	ldr	r3, [pc, #288]	; (300093b0 <OTP_Write8+0x194>)
30009290:	681a      	ldr	r2, [r3, #0]
30009292:	07d1      	lsls	r1, r2, #31
30009294:	d403      	bmi.n	3000929e <OTP_Write8+0x82>
30009296:	681a      	ldr	r2, [r3, #0]
30009298:	f042 0201 	orr.w	r2, r2, #1
3000929c:	601a      	str	r2, [r3, #0]
3000929e:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
300092a2:	9b01      	ldr	r3, [sp, #4]
300092a4:	694a      	ldr	r2, [r1, #20]
300092a6:	ea4a 0303 	orr.w	r3, sl, r3
300092aa:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
300092ae:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
300092b2:	614a      	str	r2, [r1, #20]
300092b4:	602b      	str	r3, [r5, #0]
300092b6:	682b      	ldr	r3, [r5, #0]
300092b8:	2b00      	cmp	r3, #0
300092ba:	da12      	bge.n	300092e2 <OTP_Write8+0xc6>
300092bc:	f04f 0b00 	mov.w	fp, #0
300092c0:	4c3a      	ldr	r4, [pc, #232]	; (300093ac <OTP_Write8+0x190>)
300092c2:	f644 6620 	movw	r6, #20000	; 0x4e20
300092c6:	e001      	b.n	300092cc <OTP_Write8+0xb0>
300092c8:	45b3      	cmp	fp, r6
300092ca:	d037      	beq.n	3000933c <OTP_Write8+0x120>
300092cc:	2005      	movs	r0, #5
300092ce:	f10b 0b01 	add.w	fp, fp, #1
300092d2:	47a0      	blx	r4
300092d4:	682b      	ldr	r3, [r5, #0]
300092d6:	2b00      	cmp	r3, #0
300092d8:	dbf6      	blt.n	300092c8 <OTP_Write8+0xac>
300092da:	f644 6220 	movw	r2, #20000	; 0x4e20
300092de:	4593      	cmp	fp, r2
300092e0:	d02c      	beq.n	3000933c <OTP_Write8+0x120>
300092e2:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300092e6:	6953      	ldr	r3, [r2, #20]
300092e8:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300092ec:	6153      	str	r3, [r2, #20]
300092ee:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300092f0:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300092f4:	643b      	str	r3, [r7, #64]	; 0x40
300092f6:	f10d 010f 	add.w	r1, sp, #15
300092fa:	4640      	mov	r0, r8
300092fc:	f7ff ff12 	bl	30009124 <OTP_ProgramMarginRead8.part.0>
30009300:	2800      	cmp	r0, #0
30009302:	d040      	beq.n	30009386 <OTP_Write8+0x16a>
30009304:	f89d 300f 	ldrb.w	r3, [sp, #15]
30009308:	454b      	cmp	r3, r9
3000930a:	d046      	beq.n	3000939a <OTP_Write8+0x17e>
3000930c:	9a00      	ldr	r2, [sp, #0]
3000930e:	2a01      	cmp	r2, #1
30009310:	d009      	beq.n	30009326 <OTP_Write8+0x10a>
30009312:	2201      	movs	r2, #1
30009314:	9200      	str	r2, [sp, #0]
30009316:	e797      	b.n	30009248 <OTP_Write8+0x2c>
30009318:	23ff      	movs	r3, #255	; 0xff
3000931a:	f88d 300f 	strb.w	r3, [sp, #15]
3000931e:	4b25      	ldr	r3, [pc, #148]	; (300093b4 <OTP_Write8+0x198>)
30009320:	681b      	ldr	r3, [r3, #0]
30009322:	041d      	lsls	r5, r3, #16
30009324:	d403      	bmi.n	3000932e <OTP_Write8+0x112>
30009326:	2000      	movs	r0, #0
30009328:	b005      	add	sp, #20
3000932a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000932e:	4822      	ldr	r0, [pc, #136]	; (300093b8 <OTP_Write8+0x19c>)
30009330:	f002 ff42 	bl	3000c1b8 <__DiagPrintf_veneer>
30009334:	2000      	movs	r0, #0
30009336:	b005      	add	sp, #20
30009338:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000933c:	4c1d      	ldr	r4, [pc, #116]	; (300093b4 <OTP_Write8+0x198>)
3000933e:	6820      	ldr	r0, [r4, #0]
30009340:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
30009344:	d10a      	bne.n	3000935c <OTP_Write8+0x140>
30009346:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
3000934a:	6953      	ldr	r3, [r2, #20]
3000934c:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30009350:	6153      	str	r3, [r2, #20]
30009352:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009354:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30009358:	643b      	str	r3, [r7, #64]	; 0x40
3000935a:	e7e5      	b.n	30009328 <OTP_Write8+0x10c>
3000935c:	4641      	mov	r1, r8
3000935e:	4817      	ldr	r0, [pc, #92]	; (300093bc <OTP_Write8+0x1a0>)
30009360:	f002 ff2a 	bl	3000c1b8 <__DiagPrintf_veneer>
30009364:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30009368:	6953      	ldr	r3, [r2, #20]
3000936a:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
3000936e:	6153      	str	r3, [r2, #20]
30009370:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009372:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30009376:	643b      	str	r3, [r7, #64]	; 0x40
30009378:	6823      	ldr	r3, [r4, #0]
3000937a:	041a      	lsls	r2, r3, #16
3000937c:	d5d3      	bpl.n	30009326 <OTP_Write8+0x10a>
3000937e:	4810      	ldr	r0, [pc, #64]	; (300093c0 <OTP_Write8+0x1a4>)
30009380:	f002 ff1a 	bl	3000c1b8 <__DiagPrintf_veneer>
30009384:	e7cf      	b.n	30009326 <OTP_Write8+0x10a>
30009386:	4b0b      	ldr	r3, [pc, #44]	; (300093b4 <OTP_Write8+0x198>)
30009388:	4604      	mov	r4, r0
3000938a:	681b      	ldr	r3, [r3, #0]
3000938c:	041b      	lsls	r3, r3, #16
3000938e:	d5ca      	bpl.n	30009326 <OTP_Write8+0x10a>
30009390:	480c      	ldr	r0, [pc, #48]	; (300093c4 <OTP_Write8+0x1a8>)
30009392:	f002 ff11 	bl	3000c1b8 <__DiagPrintf_veneer>
30009396:	4620      	mov	r0, r4
30009398:	e7c6      	b.n	30009328 <OTP_Write8+0x10c>
3000939a:	2001      	movs	r0, #1
3000939c:	e7c4      	b.n	30009328 <OTP_Write8+0x10c>
3000939e:	bf00      	nop
300093a0:	0000e5f9 	.word	0x0000e5f9
300093a4:	5200000c 	.word	0x5200000c
300093a8:	42000008 	.word	0x42000008
300093ac:	00009b2d 	.word	0x00009b2d
300093b0:	42008000 	.word	0x42008000
300093b4:	2001c00c 	.word	0x2001c00c
300093b8:	3000d44c 	.word	0x3000d44c
300093bc:	3000d478 	.word	0x3000d478
300093c0:	3000d4a8 	.word	0x3000d4a8
300093c4:	3000d4d8 	.word	0x3000d4d8

300093c8 <EFUSERead8>:
300093c8:	4608      	mov	r0, r1
300093ca:	4611      	mov	r1, r2
300093cc:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
300093d0:	d201      	bcs.n	300093d6 <EFUSERead8+0xe>
300093d2:	f7ff be37 	b.w	30009044 <OTP_Read8.part.0>
300093d6:	23ff      	movs	r3, #255	; 0xff
300093d8:	2000      	movs	r0, #0
300093da:	7013      	strb	r3, [r2, #0]
300093dc:	4770      	bx	lr
300093de:	bf00      	nop

300093e0 <EFUSE_PMAP_READ8>:
300093e0:	4608      	mov	r0, r1
300093e2:	4611      	mov	r1, r2
300093e4:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
300093e8:	d201      	bcs.n	300093ee <EFUSE_PMAP_READ8+0xe>
300093ea:	f7ff be2b 	b.w	30009044 <OTP_Read8.part.0>
300093ee:	23ff      	movs	r3, #255	; 0xff
300093f0:	2000      	movs	r0, #0
300093f2:	7013      	strb	r3, [r2, #0]
300093f4:	4770      	bx	lr
300093f6:	bf00      	nop

300093f8 <PLL_NP_ClkSet>:
300093f8:	b570      	push	{r4, r5, r6, lr}
300093fa:	4604      	mov	r4, r0
300093fc:	4b1b      	ldr	r3, [pc, #108]	; (3000946c <PLL_NP_ClkSet+0x74>)
300093fe:	4798      	blx	r3
30009400:	4d1b      	ldr	r5, [pc, #108]	; (30009470 <PLL_NP_ClkSet+0x78>)
30009402:	08c2      	lsrs	r2, r0, #3
30009404:	fbb4 f3f0 	udiv	r3, r4, r0
30009408:	fb00 4c13 	mls	ip, r0, r3, r4
3000940c:	1e9c      	subs	r4, r3, #2
3000940e:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009410:	0164      	lsls	r4, r4, #5
30009412:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30009416:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
3000941a:	431c      	orrs	r4, r3
3000941c:	fbbc f3f2 	udiv	r3, ip, r2
30009420:	646c      	str	r4, [r5, #68]	; 0x44
30009422:	041c      	lsls	r4, r3, #16
30009424:	fb02 c013 	mls	r0, r2, r3, ip
30009428:	2300      	movs	r3, #0
3000942a:	6cee      	ldr	r6, [r5, #76]	; 0x4c
3000942c:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30009430:	4619      	mov	r1, r3
30009432:	fa1f fc86 	uxth.w	ip, r6
30009436:	4e0f      	ldr	r6, [pc, #60]	; (30009474 <PLL_NP_ClkSet+0x7c>)
30009438:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
3000943c:	ea44 040c 	orr.w	r4, r4, ip
30009440:	f002 fd22 	bl	3000be88 <__aeabi_uldivmod>
30009444:	4603      	mov	r3, r0
30009446:	2001      	movs	r0, #1
30009448:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
3000944c:	64ec      	str	r4, [r5, #76]	; 0x4c
3000944e:	47b0      	blx	r6
30009450:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009452:	203c      	movs	r0, #60	; 0x3c
30009454:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30009458:	646b      	str	r3, [r5, #68]	; 0x44
3000945a:	6c6b      	ldr	r3, [r5, #68]	; 0x44
3000945c:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30009460:	646b      	str	r3, [r5, #68]	; 0x44
30009462:	4633      	mov	r3, r6
30009464:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30009468:	4718      	bx	r3
3000946a:	bf00      	nop
3000946c:	000099f5 	.word	0x000099f5
30009470:	42008800 	.word	0x42008800
30009474:	00009b2d 	.word	0x00009b2d

30009478 <PLL_AP_ClkSet>:
30009478:	b510      	push	{r4, lr}
3000947a:	4604      	mov	r4, r0
3000947c:	4b0b      	ldr	r3, [pc, #44]	; (300094ac <PLL_AP_ClkSet+0x34>)
3000947e:	4798      	blx	r3
30009480:	fbb4 f0f0 	udiv	r0, r4, r0
30009484:	f1a0 0319 	sub.w	r3, r0, #25
30009488:	1e84      	subs	r4, r0, #2
3000948a:	2b0f      	cmp	r3, #15
3000948c:	d807      	bhi.n	3000949e <PLL_AP_ClkSet+0x26>
3000948e:	4b08      	ldr	r3, [pc, #32]	; (300094b0 <PLL_AP_ClkSet+0x38>)
30009490:	b2e0      	uxtb	r0, r4
30009492:	695c      	ldr	r4, [r3, #20]
30009494:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30009498:	4320      	orrs	r0, r4
3000949a:	6158      	str	r0, [r3, #20]
3000949c:	bd10      	pop	{r4, pc}
3000949e:	f240 1175 	movw	r1, #373	; 0x175
300094a2:	4804      	ldr	r0, [pc, #16]	; (300094b4 <PLL_AP_ClkSet+0x3c>)
300094a4:	f002 fea8 	bl	3000c1f8 <__io_assert_failed_veneer>
300094a8:	e7f1      	b.n	3000948e <PLL_AP_ClkSet+0x16>
300094aa:	bf00      	nop
300094ac:	000099f5 	.word	0x000099f5
300094b0:	41000300 	.word	0x41000300
300094b4:	3000d504 	.word	0x3000d504

300094b8 <PLL_AP>:
300094b8:	2801      	cmp	r0, #1
300094ba:	b538      	push	{r3, r4, r5, lr}
300094bc:	d009      	beq.n	300094d2 <PLL_AP+0x1a>
300094be:	4b13      	ldr	r3, [pc, #76]	; (3000950c <PLL_AP+0x54>)
300094c0:	681a      	ldr	r2, [r3, #0]
300094c2:	f022 0209 	bic.w	r2, r2, #9
300094c6:	601a      	str	r2, [r3, #0]
300094c8:	681a      	ldr	r2, [r3, #0]
300094ca:	f022 0206 	bic.w	r2, r2, #6
300094ce:	601a      	str	r2, [r3, #0]
300094d0:	bd38      	pop	{r3, r4, r5, pc}
300094d2:	4b0f      	ldr	r3, [pc, #60]	; (30009510 <PLL_AP+0x58>)
300094d4:	6f9a      	ldr	r2, [r3, #120]	; 0x78
300094d6:	f012 0f05 	tst.w	r2, #5
300094da:	d00f      	beq.n	300094fc <PLL_AP+0x44>
300094dc:	4d0d      	ldr	r5, [pc, #52]	; (30009514 <PLL_AP+0x5c>)
300094de:	4c0b      	ldr	r4, [pc, #44]	; (3000950c <PLL_AP+0x54>)
300094e0:	2001      	movs	r0, #1
300094e2:	6823      	ldr	r3, [r4, #0]
300094e4:	f043 0306 	orr.w	r3, r3, #6
300094e8:	6023      	str	r3, [r4, #0]
300094ea:	47a8      	blx	r5
300094ec:	6823      	ldr	r3, [r4, #0]
300094ee:	f043 0309 	orr.w	r3, r3, #9
300094f2:	6023      	str	r3, [r4, #0]
300094f4:	6863      	ldr	r3, [r4, #4]
300094f6:	2b00      	cmp	r3, #0
300094f8:	dafc      	bge.n	300094f4 <PLL_AP+0x3c>
300094fa:	bd38      	pop	{r3, r4, r5, pc}
300094fc:	6f9a      	ldr	r2, [r3, #120]	; 0x78
300094fe:	20a0      	movs	r0, #160	; 0xa0
30009500:	4d04      	ldr	r5, [pc, #16]	; (30009514 <PLL_AP+0x5c>)
30009502:	f042 0207 	orr.w	r2, r2, #7
30009506:	679a      	str	r2, [r3, #120]	; 0x78
30009508:	47a8      	blx	r5
3000950a:	e7e8      	b.n	300094de <PLL_AP+0x26>
3000950c:	41000300 	.word	0x41000300
30009510:	42008800 	.word	0x42008800
30009514:	00009b2d 	.word	0x00009b2d

30009518 <PSRAM_INFO_Update>:
30009518:	4942      	ldr	r1, [pc, #264]	; (30009624 <PSRAM_INFO_Update+0x10c>)
3000951a:	4a43      	ldr	r2, [pc, #268]	; (30009628 <PSRAM_INFO_Update+0x110>)
3000951c:	b538      	push	{r3, r4, r5, lr}
3000951e:	f8d1 1228 	ldr.w	r1, [r1, #552]	; 0x228
30009522:	6815      	ldr	r5, [r2, #0]
30009524:	4b41      	ldr	r3, [pc, #260]	; (3000962c <PSRAM_INFO_Update+0x114>)
30009526:	f3c1 2102 	ubfx	r1, r1, #8, #3
3000952a:	4c41      	ldr	r4, [pc, #260]	; (30009630 <PSRAM_INFO_Update+0x118>)
3000952c:	3101      	adds	r1, #1
3000952e:	fb03 f505 	mul.w	r5, r3, r5
30009532:	4b40      	ldr	r3, [pc, #256]	; (30009634 <PSRAM_INFO_Update+0x11c>)
30009534:	fbb5 f5f1 	udiv	r5, r5, r1
30009538:	086a      	lsrs	r2, r5, #1
3000953a:	60e2      	str	r2, [r4, #12]
3000953c:	4798      	blx	r3
3000953e:	4b3e      	ldr	r3, [pc, #248]	; (30009638 <PSRAM_INFO_Update+0x120>)
30009540:	6020      	str	r0, [r4, #0]
30009542:	4798      	blx	r3
30009544:	4b3d      	ldr	r3, [pc, #244]	; (3000963c <PSRAM_INFO_Update+0x124>)
30009546:	220a      	movs	r2, #10
30009548:	60a0      	str	r0, [r4, #8]
3000954a:	6122      	str	r2, [r4, #16]
3000954c:	2220      	movs	r2, #32
3000954e:	fbb3 f3f5 	udiv	r3, r3, r5
30009552:	6163      	str	r3, [r4, #20]
30009554:	fbb2 f3f3 	udiv	r3, r2, r3
30009558:	6822      	ldr	r2, [r4, #0]
3000955a:	6223      	str	r3, [r4, #32]
3000955c:	2a01      	cmp	r2, #1
3000955e:	68e3      	ldr	r3, [r4, #12]
30009560:	4a37      	ldr	r2, [pc, #220]	; (30009640 <PSRAM_INFO_Update+0x128>)
30009562:	d014      	beq.n	3000958e <PSRAM_INFO_Update+0x76>
30009564:	4293      	cmp	r3, r2
30009566:	d808      	bhi.n	3000957a <PSRAM_INFO_Update+0x62>
30009568:	2204      	movs	r2, #4
3000956a:	2308      	movs	r3, #8
3000956c:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009570:	4b34      	ldr	r3, [pc, #208]	; (30009644 <PSRAM_INFO_Update+0x12c>)
30009572:	689b      	ldr	r3, [r3, #8]
30009574:	079b      	lsls	r3, r3, #30
30009576:	d414      	bmi.n	300095a2 <PSRAM_INFO_Update+0x8a>
30009578:	bd38      	pop	{r3, r4, r5, pc}
3000957a:	4a33      	ldr	r2, [pc, #204]	; (30009648 <PSRAM_INFO_Update+0x130>)
3000957c:	4293      	cmp	r3, r2
3000957e:	d91b      	bls.n	300095b8 <PSRAM_INFO_Update+0xa0>
30009580:	4a32      	ldr	r2, [pc, #200]	; (3000964c <PSRAM_INFO_Update+0x134>)
30009582:	4293      	cmp	r3, r2
30009584:	d828      	bhi.n	300095d8 <PSRAM_INFO_Update+0xc0>
30009586:	2306      	movs	r3, #6
30009588:	e9c4 3306 	strd	r3, r3, [r4, #24]
3000958c:	e7f0      	b.n	30009570 <PSRAM_INFO_Update+0x58>
3000958e:	4293      	cmp	r3, r2
30009590:	d90d      	bls.n	300095ae <PSRAM_INFO_Update+0x96>
30009592:	4a2d      	ldr	r2, [pc, #180]	; (30009648 <PSRAM_INFO_Update+0x130>)
30009594:	4293      	cmp	r3, r2
30009596:	d814      	bhi.n	300095c2 <PSRAM_INFO_Update+0xaa>
30009598:	2205      	movs	r2, #5
3000959a:	230f      	movs	r3, #15
3000959c:	e9c4 2306 	strd	r2, r3, [r4, #24]
300095a0:	e7e6      	b.n	30009570 <PSRAM_INFO_Update+0x58>
300095a2:	4629      	mov	r1, r5
300095a4:	482a      	ldr	r0, [pc, #168]	; (30009650 <PSRAM_INFO_Update+0x138>)
300095a6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300095aa:	f002 be05 	b.w	3000c1b8 <__DiagPrintf_veneer>
300095ae:	2204      	movs	r2, #4
300095b0:	230f      	movs	r3, #15
300095b2:	e9c4 2306 	strd	r2, r3, [r4, #24]
300095b6:	e7db      	b.n	30009570 <PSRAM_INFO_Update+0x58>
300095b8:	2205      	movs	r2, #5
300095ba:	2308      	movs	r3, #8
300095bc:	e9c4 2306 	strd	r2, r3, [r4, #24]
300095c0:	e7d6      	b.n	30009570 <PSRAM_INFO_Update+0x58>
300095c2:	4a22      	ldr	r2, [pc, #136]	; (3000964c <PSRAM_INFO_Update+0x134>)
300095c4:	4293      	cmp	r3, r2
300095c6:	d91b      	bls.n	30009600 <PSRAM_INFO_Update+0xe8>
300095c8:	4a22      	ldr	r2, [pc, #136]	; (30009654 <PSRAM_INFO_Update+0x13c>)
300095ca:	4293      	cmp	r3, r2
300095cc:	d822      	bhi.n	30009614 <PSRAM_INFO_Update+0xfc>
300095ce:	2207      	movs	r2, #7
300095d0:	2314      	movs	r3, #20
300095d2:	e9c4 2306 	strd	r2, r3, [r4, #24]
300095d6:	e7cb      	b.n	30009570 <PSRAM_INFO_Update+0x58>
300095d8:	4a1e      	ldr	r2, [pc, #120]	; (30009654 <PSRAM_INFO_Update+0x13c>)
300095da:	4293      	cmp	r3, r2
300095dc:	d915      	bls.n	3000960a <PSRAM_INFO_Update+0xf2>
300095de:	4a1e      	ldr	r2, [pc, #120]	; (30009658 <PSRAM_INFO_Update+0x140>)
300095e0:	4293      	cmp	r3, r2
300095e2:	bf8c      	ite	hi
300095e4:	2300      	movhi	r3, #0
300095e6:	2301      	movls	r3, #1
300095e8:	2804      	cmp	r0, #4
300095ea:	d100      	bne.n	300095ee <PSRAM_INFO_Update+0xd6>
300095ec:	b96b      	cbnz	r3, 3000960a <PSRAM_INFO_Update+0xf2>
300095ee:	2803      	cmp	r0, #3
300095f0:	d1be      	bne.n	30009570 <PSRAM_INFO_Update+0x58>
300095f2:	2b00      	cmp	r3, #0
300095f4:	d0bc      	beq.n	30009570 <PSRAM_INFO_Update+0x58>
300095f6:	220a      	movs	r2, #10
300095f8:	2306      	movs	r3, #6
300095fa:	e9c4 2306 	strd	r2, r3, [r4, #24]
300095fe:	e7b7      	b.n	30009570 <PSRAM_INFO_Update+0x58>
30009600:	2206      	movs	r2, #6
30009602:	2312      	movs	r3, #18
30009604:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009608:	e7b2      	b.n	30009570 <PSRAM_INFO_Update+0x58>
3000960a:	2207      	movs	r2, #7
3000960c:	2306      	movs	r3, #6
3000960e:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009612:	e7ad      	b.n	30009570 <PSRAM_INFO_Update+0x58>
30009614:	4a10      	ldr	r2, [pc, #64]	; (30009658 <PSRAM_INFO_Update+0x140>)
30009616:	4293      	cmp	r3, r2
30009618:	d8aa      	bhi.n	30009570 <PSRAM_INFO_Update+0x58>
3000961a:	2208      	movs	r2, #8
3000961c:	231c      	movs	r3, #28
3000961e:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009622:	e7a5      	b.n	30009570 <PSRAM_INFO_Update+0x58>
30009624:	42008000 	.word	0x42008000
30009628:	3000e3b4 	.word	0x3000e3b4
3000962c:	000f4240 	.word	0x000f4240
30009630:	3000eab0 	.word	0x3000eab0
30009634:	30007f9d 	.word	0x30007f9d
30009638:	30007f65 	.word	0x30007f65
3000963c:	3b9aca00 	.word	0x3b9aca00
30009640:	0632ea00 	.word	0x0632ea00
30009644:	2001c00c 	.word	0x2001c00c
30009648:	07ed6b40 	.word	0x07ed6b40
3000964c:	09e4f580 	.word	0x09e4f580
30009650:	3000d514 	.word	0x3000d514
30009654:	0bebc200 	.word	0x0bebc200
30009658:	0ee6b280 	.word	0x0ee6b280

3000965c <PSRAM_PHY_StructInit>:
3000965c:	2203      	movs	r2, #3
3000965e:	2301      	movs	r3, #1
30009660:	210a      	movs	r1, #10
30009662:	b410      	push	{r4}
30009664:	e9c0 2202 	strd	r2, r2, [r0, #8]
30009668:	2402      	movs	r4, #2
3000966a:	2207      	movs	r2, #7
3000966c:	e9c0 3400 	strd	r3, r4, [r0]
30009670:	e9c0 1304 	strd	r1, r3, [r0, #16]
30009674:	f85d 4b04 	ldr.w	r4, [sp], #4
30009678:	6182      	str	r2, [r0, #24]
3000967a:	4770      	bx	lr

3000967c <PSRAM_PHY_Init>:
3000967c:	b538      	push	{r3, r4, r5, lr}
3000967e:	4604      	mov	r4, r0
30009680:	6903      	ldr	r3, [r0, #16]
30009682:	4d14      	ldr	r5, [pc, #80]	; (300096d4 <PSRAM_PHY_Init+0x58>)
30009684:	f003 031f 	and.w	r3, r3, #31
30009688:	4913      	ldr	r1, [pc, #76]	; (300096d8 <PSRAM_PHY_Init+0x5c>)
3000968a:	e9d0 0200 	ldrd	r0, r2, [r0]
3000968e:	0612      	lsls	r2, r2, #24
30009690:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30009694:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30009698:	4313      	orrs	r3, r2
3000969a:	68a2      	ldr	r2, [r4, #8]
3000969c:	0412      	lsls	r2, r2, #16
3000969e:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
300096a2:	4313      	orrs	r3, r2
300096a4:	68e2      	ldr	r2, [r4, #12]
300096a6:	0212      	lsls	r2, r2, #8
300096a8:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
300096ac:	4313      	orrs	r3, r2
300096ae:	606b      	str	r3, [r5, #4]
300096b0:	4788      	blx	r1
300096b2:	2801      	cmp	r0, #1
300096b4:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
300096b8:	bf08      	it	eq
300096ba:	4a08      	ldreq	r2, [pc, #32]	; (300096dc <PSRAM_PHY_Init+0x60>)
300096bc:	ea4f 1303 	mov.w	r3, r3, lsl #4
300096c0:	f001 010f 	and.w	r1, r1, #15
300096c4:	bf18      	it	ne
300096c6:	4a06      	ldrne	r2, [pc, #24]	; (300096e0 <PSRAM_PHY_Init+0x64>)
300096c8:	b2db      	uxtb	r3, r3
300096ca:	430b      	orrs	r3, r1
300096cc:	431a      	orrs	r2, r3
300096ce:	61aa      	str	r2, [r5, #24]
300096d0:	bd38      	pop	{r3, r4, r5, pc}
300096d2:	bf00      	nop
300096d4:	41002000 	.word	0x41002000
300096d8:	30007f9d 	.word	0x30007f9d
300096dc:	80000e00 	.word	0x80000e00
300096e0:	80000f00 	.word	0x80000f00

300096e4 <PSRAM_CTRL_Init>:
300096e4:	b470      	push	{r4, r5, r6}
300096e6:	4b4c      	ldr	r3, [pc, #304]	; (30009818 <PSRAM_CTRL_Init+0x134>)
300096e8:	2400      	movs	r4, #0
300096ea:	4a4c      	ldr	r2, [pc, #304]	; (3000981c <PSRAM_CTRL_Init+0x138>)
300096ec:	f24d 0101 	movw	r1, #53249	; 0xd001
300096f0:	609c      	str	r4, [r3, #8]
300096f2:	2001      	movs	r0, #1
300096f4:	601a      	str	r2, [r3, #0]
300096f6:	4a4a      	ldr	r2, [pc, #296]	; (30009820 <PSRAM_CTRL_Init+0x13c>)
300096f8:	6158      	str	r0, [r3, #20]
300096fa:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
300096fe:	6811      	ldr	r1, [r2, #0]
30009700:	2902      	cmp	r1, #2
30009702:	d04a      	beq.n	3000979a <PSRAM_CTRL_Init+0xb6>
30009704:	6954      	ldr	r4, [r2, #20]
30009706:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
3000970a:	69d0      	ldr	r0, [r2, #28]
3000970c:	6915      	ldr	r5, [r2, #16]
3000970e:	fbb0 f0f4 	udiv	r0, r0, r4
30009712:	fbb1 f1f4 	udiv	r1, r1, r4
30009716:	6994      	ldr	r4, [r2, #24]
30009718:	f005 020f 	and.w	r2, r5, #15
3000971c:	f241 5551 	movw	r5, #5457	; 0x1551
30009720:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30009724:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30009728:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
3000972c:	094a      	lsrs	r2, r1, #5
3000972e:	0181      	lsls	r1, r0, #6
30009730:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30009734:	4d3b      	ldr	r5, [pc, #236]	; (30009824 <PSRAM_CTRL_Init+0x140>)
30009736:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
3000973a:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
3000973e:	00a2      	lsls	r2, r4, #2
30009740:	4301      	orrs	r1, r0
30009742:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
30009746:	3a01      	subs	r2, #1
30009748:	430d      	orrs	r5, r1
3000974a:	f242 0120 	movw	r1, #8224	; 0x2020
3000974e:	f3c2 020b 	ubfx	r2, r2, #0, #12
30009752:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
30009756:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
3000975a:	f44f 2180 	mov.w	r1, #262144	; 0x40000
3000975e:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
30009762:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
30009766:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000976a:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
3000976e:	4420      	add	r0, r4
30009770:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
30009774:	f640 71fe 	movw	r1, #4094	; 0xffe
30009778:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
3000977c:	ea01 0140 	and.w	r1, r1, r0, lsl #1
30009780:	432a      	orrs	r2, r5
30009782:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
30009786:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
3000978a:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
3000978e:	4a22      	ldr	r2, [pc, #136]	; (30009818 <PSRAM_CTRL_Init+0x134>)
30009790:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009792:	061b      	lsls	r3, r3, #24
30009794:	d5fc      	bpl.n	30009790 <PSRAM_CTRL_Init+0xac>
30009796:	bc70      	pop	{r4, r5, r6}
30009798:	4770      	bx	lr
3000979a:	6955      	ldr	r5, [r2, #20]
3000979c:	207d      	movs	r0, #125	; 0x7d
3000979e:	69d1      	ldr	r1, [r2, #28]
300097a0:	26a0      	movs	r6, #160	; 0xa0
300097a2:	6914      	ldr	r4, [r2, #16]
300097a4:	6992      	ldr	r2, [r2, #24]
300097a6:	f004 040f 	and.w	r4, r4, #15
300097aa:	0092      	lsls	r2, r2, #2
300097ac:	fbb1 f1f5 	udiv	r1, r1, r5
300097b0:	fbb0 f5f5 	udiv	r5, r0, r5
300097b4:	f245 5051 	movw	r0, #21841	; 0x5551
300097b8:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
300097bc:	f001 003f 	and.w	r0, r1, #63	; 0x3f
300097c0:	0189      	lsls	r1, r1, #6
300097c2:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
300097c6:	f641 4570 	movw	r5, #7280	; 0x1c70
300097ca:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
300097ce:	4325      	orrs	r5, r4
300097d0:	2420      	movs	r4, #32
300097d2:	4308      	orrs	r0, r1
300097d4:	1e51      	subs	r1, r2, #1
300097d6:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
300097da:	3a02      	subs	r2, #2
300097dc:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
300097e0:	f3c1 010b 	ubfx	r1, r1, #0, #12
300097e4:	f3c2 020b 	ubfx	r2, r2, #0, #12
300097e8:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
300097ec:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
300097f0:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
300097f4:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
300097f8:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
300097fc:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30009800:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30009804:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30009808:	4301      	orrs	r1, r0
3000980a:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
3000980e:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30009812:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30009816:	e7ba      	b.n	3000978e <PSRAM_CTRL_Init+0xaa>
30009818:	41012000 	.word	0x41012000
3000981c:	00bfe000 	.word	0x00bfe000
30009820:	3000eab0 	.word	0x3000eab0
30009824:	00107000 	.word	0x00107000

30009828 <PSRAM_REG_Write>:
30009828:	b430      	push	{r4, r5}
3000982a:	4c20      	ldr	r4, [pc, #128]	; (300098ac <PSRAM_REG_Write+0x84>)
3000982c:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000982e:	f013 0301 	ands.w	r3, r3, #1
30009832:	d1fb      	bne.n	3000982c <PSRAM_REG_Write+0x4>
30009834:	6825      	ldr	r5, [r4, #0]
30009836:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
3000983a:	6025      	str	r5, [r4, #0]
3000983c:	60a3      	str	r3, [r4, #8]
3000983e:	6825      	ldr	r5, [r4, #0]
30009840:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009844:	6025      	str	r5, [r4, #0]
30009846:	6063      	str	r3, [r4, #4]
30009848:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
3000984c:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
30009850:	23c0      	movs	r3, #192	; 0xc0
30009852:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009856:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000985a:	0e03      	lsrs	r3, r0, #24
3000985c:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009860:	f3c0 4307 	ubfx	r3, r0, #16, #8
30009864:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009868:	f3c0 2307 	ubfx	r3, r0, #8, #8
3000986c:	b2c0      	uxtb	r0, r0
3000986e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009872:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30009876:	b141      	cbz	r1, 3000988a <PSRAM_REG_Write+0x62>
30009878:	3a01      	subs	r2, #1
3000987a:	480c      	ldr	r0, [pc, #48]	; (300098ac <PSRAM_REG_Write+0x84>)
3000987c:	4411      	add	r1, r2
3000987e:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30009882:	428a      	cmp	r2, r1
30009884:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009888:	d1f9      	bne.n	3000987e <PSRAM_REG_Write+0x56>
3000988a:	4a08      	ldr	r2, [pc, #32]	; (300098ac <PSRAM_REG_Write+0x84>)
3000988c:	2301      	movs	r3, #1
3000988e:	6093      	str	r3, [r2, #8]
30009890:	6893      	ldr	r3, [r2, #8]
30009892:	07d9      	lsls	r1, r3, #31
30009894:	d4fc      	bmi.n	30009890 <PSRAM_REG_Write+0x68>
30009896:	4a05      	ldr	r2, [pc, #20]	; (300098ac <PSRAM_REG_Write+0x84>)
30009898:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000989a:	07db      	lsls	r3, r3, #31
3000989c:	d4fc      	bmi.n	30009898 <PSRAM_REG_Write+0x70>
3000989e:	6813      	ldr	r3, [r2, #0]
300098a0:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300098a4:	6013      	str	r3, [r2, #0]
300098a6:	bc30      	pop	{r4, r5}
300098a8:	4770      	bx	lr
300098aa:	bf00      	nop
300098ac:	41012000 	.word	0x41012000

300098b0 <PSRAM_APM_DEVIC_Init>:
300098b0:	4b12      	ldr	r3, [pc, #72]	; (300098fc <PSRAM_APM_DEVIC_Init+0x4c>)
300098b2:	2102      	movs	r1, #2
300098b4:	2000      	movs	r0, #0
300098b6:	b530      	push	{r4, r5, lr}
300098b8:	4c11      	ldr	r4, [pc, #68]	; (30009900 <PSRAM_APM_DEVIC_Init+0x50>)
300098ba:	b083      	sub	sp, #12
300098bc:	699d      	ldr	r5, [r3, #24]
300098be:	466a      	mov	r2, sp
300098c0:	eb04 0c05 	add.w	ip, r4, r5
300098c4:	1eeb      	subs	r3, r5, #3
300098c6:	4c0f      	ldr	r4, [pc, #60]	; (30009904 <PSRAM_APM_DEVIC_Init+0x54>)
300098c8:	408b      	lsls	r3, r1
300098ca:	f81c cc03 	ldrb.w	ip, [ip, #-3]
300098ce:	f003 031c 	and.w	r3, r3, #28
300098d2:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
300098d6:	f043 0320 	orr.w	r3, r3, #32
300098da:	fa5f fc8c 	uxtb.w	ip, ip
300098de:	f88d 3000 	strb.w	r3, [sp]
300098e2:	f88d c004 	strb.w	ip, [sp, #4]
300098e6:	f88d 3001 	strb.w	r3, [sp, #1]
300098ea:	f88d c005 	strb.w	ip, [sp, #5]
300098ee:	47a0      	blx	r4
300098f0:	aa01      	add	r2, sp, #4
300098f2:	2102      	movs	r1, #2
300098f4:	2004      	movs	r0, #4
300098f6:	47a0      	blx	r4
300098f8:	b003      	add	sp, #12
300098fa:	bd30      	pop	{r4, r5, pc}
300098fc:	3000eab0 	.word	0x3000eab0
30009900:	3000e4a8 	.word	0x3000e4a8
30009904:	30009829 	.word	0x30009829

30009908 <PSRAM_WB_REG_Write>:
30009908:	b430      	push	{r4, r5}
3000990a:	4c1e      	ldr	r4, [pc, #120]	; (30009984 <PSRAM_WB_REG_Write+0x7c>)
3000990c:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000990e:	f013 0301 	ands.w	r3, r3, #1
30009912:	d1fb      	bne.n	3000990c <PSRAM_WB_REG_Write+0x4>
30009914:	6825      	ldr	r5, [r4, #0]
30009916:	b2c0      	uxtb	r0, r0
30009918:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
3000991c:	6025      	str	r5, [r4, #0]
3000991e:	60a3      	str	r3, [r4, #8]
30009920:	6825      	ldr	r5, [r4, #0]
30009922:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009926:	6025      	str	r5, [r4, #0]
30009928:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
3000992c:	6063      	str	r3, [r4, #4]
3000992e:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
30009932:	2560      	movs	r5, #96	; 0x60
30009934:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30009938:	2501      	movs	r5, #1
3000993a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000993e:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30009942:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009946:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000994a:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
3000994e:	b141      	cbz	r1, 30009962 <PSRAM_WB_REG_Write+0x5a>
30009950:	3a01      	subs	r2, #1
30009952:	480c      	ldr	r0, [pc, #48]	; (30009984 <PSRAM_WB_REG_Write+0x7c>)
30009954:	4411      	add	r1, r2
30009956:	f812 3f01 	ldrb.w	r3, [r2, #1]!
3000995a:	428a      	cmp	r2, r1
3000995c:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009960:	d1f9      	bne.n	30009956 <PSRAM_WB_REG_Write+0x4e>
30009962:	4a08      	ldr	r2, [pc, #32]	; (30009984 <PSRAM_WB_REG_Write+0x7c>)
30009964:	2301      	movs	r3, #1
30009966:	6093      	str	r3, [r2, #8]
30009968:	6893      	ldr	r3, [r2, #8]
3000996a:	07d9      	lsls	r1, r3, #31
3000996c:	d4fc      	bmi.n	30009968 <PSRAM_WB_REG_Write+0x60>
3000996e:	4a05      	ldr	r2, [pc, #20]	; (30009984 <PSRAM_WB_REG_Write+0x7c>)
30009970:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009972:	07db      	lsls	r3, r3, #31
30009974:	d4fc      	bmi.n	30009970 <PSRAM_WB_REG_Write+0x68>
30009976:	6813      	ldr	r3, [r2, #0]
30009978:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000997c:	6013      	str	r3, [r2, #0]
3000997e:	bc30      	pop	{r4, r5}
30009980:	4770      	bx	lr
30009982:	bf00      	nop
30009984:	41012000 	.word	0x41012000

30009988 <PSRAM_WB_DEVIC_Init>:
30009988:	490b      	ldr	r1, [pc, #44]	; (300099b8 <PSRAM_WB_DEVIC_Init+0x30>)
3000998a:	4b0c      	ldr	r3, [pc, #48]	; (300099bc <PSRAM_WB_DEVIC_Init+0x34>)
3000998c:	b510      	push	{r4, lr}
3000998e:	6988      	ldr	r0, [r1, #24]
30009990:	b082      	sub	sp, #8
30009992:	248f      	movs	r4, #143	; 0x8f
30009994:	2102      	movs	r1, #2
30009996:	4403      	add	r3, r0
30009998:	aa01      	add	r2, sp, #4
3000999a:	f88d 4004 	strb.w	r4, [sp, #4]
3000999e:	2000      	movs	r0, #0
300099a0:	f813 3c03 	ldrb.w	r3, [r3, #-3]
300099a4:	4c06      	ldr	r4, [pc, #24]	; (300099c0 <PSRAM_WB_DEVIC_Init+0x38>)
300099a6:	011b      	lsls	r3, r3, #4
300099a8:	f043 030c 	orr.w	r3, r3, #12
300099ac:	f88d 3005 	strb.w	r3, [sp, #5]
300099b0:	47a0      	blx	r4
300099b2:	b002      	add	sp, #8
300099b4:	bd10      	pop	{r4, pc}
300099b6:	bf00      	nop
300099b8:	3000eab0 	.word	0x3000eab0
300099bc:	3000e4c8 	.word	0x3000e4c8
300099c0:	30009909 	.word	0x30009909

300099c4 <PSRAM_calibration>:
300099c4:	4a9d      	ldr	r2, [pc, #628]	; (30009c3c <PSRAM_calibration+0x278>)
300099c6:	2300      	movs	r3, #0
300099c8:	499d      	ldr	r1, [pc, #628]	; (30009c40 <PSRAM_calibration+0x27c>)
300099ca:	489e      	ldr	r0, [pc, #632]	; (30009c44 <PSRAM_calibration+0x280>)
300099cc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300099d0:	6852      	ldr	r2, [r2, #4]
300099d2:	b091      	sub	sp, #68	; 0x44
300099d4:	e9cd 2309 	strd	r2, r3, [sp, #36]	; 0x24
300099d8:	400a      	ands	r2, r1
300099da:	930f      	str	r3, [sp, #60]	; 0x3c
300099dc:	e9cd 330b 	strd	r3, r3, [sp, #44]	; 0x2c
300099e0:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
300099e4:	9208      	str	r2, [sp, #32]
300099e6:	6942      	ldr	r2, [r0, #20]
300099e8:	03d1      	lsls	r1, r2, #15
300099ea:	d51e      	bpl.n	30009a2a <PSRAM_calibration+0x66>
300099ec:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300099f0:	f3bf 8f4f 	dsb	sy
300099f4:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300099f8:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300099fc:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009a00:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009a04:	0164      	lsls	r4, r4, #5
30009a06:	ea04 0106 	and.w	r1, r4, r6
30009a0a:	462b      	mov	r3, r5
30009a0c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009a10:	3b01      	subs	r3, #1
30009a12:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009a16:	1c5a      	adds	r2, r3, #1
30009a18:	d1f8      	bne.n	30009a0c <PSRAM_calibration+0x48>
30009a1a:	3c20      	subs	r4, #32
30009a1c:	f114 0f20 	cmn.w	r4, #32
30009a20:	d1f1      	bne.n	30009a06 <PSRAM_calibration+0x42>
30009a22:	f3bf 8f4f 	dsb	sy
30009a26:	f3bf 8f6f 	isb	sy
30009a2a:	4886      	ldr	r0, [pc, #536]	; (30009c44 <PSRAM_calibration+0x280>)
30009a2c:	2300      	movs	r3, #0
30009a2e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009a32:	f3bf 8f4f 	dsb	sy
30009a36:	6943      	ldr	r3, [r0, #20]
30009a38:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30009a3c:	6143      	str	r3, [r0, #20]
30009a3e:	f3bf 8f4f 	dsb	sy
30009a42:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009a46:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009a4a:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009a4e:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009a52:	0164      	lsls	r4, r4, #5
30009a54:	ea04 0106 	and.w	r1, r4, r6
30009a58:	462b      	mov	r3, r5
30009a5a:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009a5e:	3b01      	subs	r3, #1
30009a60:	1c5f      	adds	r7, r3, #1
30009a62:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009a66:	d1f8      	bne.n	30009a5a <PSRAM_calibration+0x96>
30009a68:	3c20      	subs	r4, #32
30009a6a:	f114 0f20 	cmn.w	r4, #32
30009a6e:	d1f1      	bne.n	30009a54 <PSRAM_calibration+0x90>
30009a70:	f3bf 8f4f 	dsb	sy
30009a74:	f3bf 8f6f 	isb	sy
30009a78:	2204      	movs	r2, #4
30009a7a:	461e      	mov	r6, r3
30009a7c:	9304      	str	r3, [sp, #16]
30009a7e:	2700      	movs	r7, #0
30009a80:	9207      	str	r2, [sp, #28]
30009a82:	46b3      	mov	fp, r6
30009a84:	4a6d      	ldr	r2, [pc, #436]	; (30009c3c <PSRAM_calibration+0x278>)
30009a86:	4c70      	ldr	r4, [pc, #448]	; (30009c48 <PSRAM_calibration+0x284>)
30009a88:	6813      	ldr	r3, [r2, #0]
30009a8a:	f8df 91d8 	ldr.w	r9, [pc, #472]	; 30009c64 <PSRAM_calibration+0x2a0>
30009a8e:	f023 0301 	bic.w	r3, r3, #1
30009a92:	f8df a1d4 	ldr.w	sl, [pc, #468]	; 30009c68 <PSRAM_calibration+0x2a4>
30009a96:	6013      	str	r3, [r2, #0]
30009a98:	2301      	movs	r3, #1
30009a9a:	9303      	str	r3, [sp, #12]
30009a9c:	9b08      	ldr	r3, [sp, #32]
30009a9e:	9a03      	ldr	r2, [sp, #12]
30009aa0:	9702      	str	r7, [sp, #8]
30009aa2:	2700      	movs	r7, #0
30009aa4:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30009aa8:	463d      	mov	r5, r7
30009aaa:	e9cd 6b05 	strd	r6, fp, [sp, #20]
30009aae:	f04f 3bff 	mov.w	fp, #4294967295
30009ab2:	465e      	mov	r6, fp
30009ab4:	e010      	b.n	30009ad8 <PSRAM_calibration+0x114>
30009ab6:	f1b6 3fff 	cmp.w	r6, #4294967295
30009aba:	bf08      	it	eq
30009abc:	462e      	moveq	r6, r5
30009abe:	2d1f      	cmp	r5, #31
30009ac0:	eb06 0b07 	add.w	fp, r6, r7
30009ac4:	f107 0701 	add.w	r7, r7, #1
30009ac8:	d102      	bne.n	30009ad0 <PSRAM_calibration+0x10c>
30009aca:	9b02      	ldr	r3, [sp, #8]
30009acc:	429f      	cmp	r7, r3
30009ace:	dc46      	bgt.n	30009b5e <PSRAM_calibration+0x19a>
30009ad0:	3501      	adds	r5, #1
30009ad2:	2d20      	cmp	r5, #32
30009ad4:	f000 80ae 	beq.w	30009c34 <PSRAM_calibration+0x270>
30009ad8:	ea48 0205 	orr.w	r2, r8, r5
30009adc:	4b57      	ldr	r3, [pc, #348]	; (30009c3c <PSRAM_calibration+0x278>)
30009ade:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
30009ae2:	495a      	ldr	r1, [pc, #360]	; (30009c4c <PSRAM_calibration+0x288>)
30009ae4:	605a      	str	r2, [r3, #4]
30009ae6:	6822      	ldr	r2, [r4, #0]
30009ae8:	4b59      	ldr	r3, [pc, #356]	; (30009c50 <PSRAM_calibration+0x28c>)
30009aea:	f8ce 2000 	str.w	r2, [lr]
30009aee:	6860      	ldr	r0, [r4, #4]
30009af0:	4a58      	ldr	r2, [pc, #352]	; (30009c54 <PSRAM_calibration+0x290>)
30009af2:	f8c9 0000 	str.w	r0, [r9]
30009af6:	68a0      	ldr	r0, [r4, #8]
30009af8:	f8ca 0000 	str.w	r0, [sl]
30009afc:	68e0      	ldr	r0, [r4, #12]
30009afe:	6018      	str	r0, [r3, #0]
30009b00:	6920      	ldr	r0, [r4, #16]
30009b02:	6008      	str	r0, [r1, #0]
30009b04:	6960      	ldr	r0, [r4, #20]
30009b06:	6010      	str	r0, [r2, #0]
30009b08:	f8de 0000 	ldr.w	r0, [lr]
30009b0c:	900a      	str	r0, [sp, #40]	; 0x28
30009b0e:	f8d9 0000 	ldr.w	r0, [r9]
30009b12:	900b      	str	r0, [sp, #44]	; 0x2c
30009b14:	f8da 0000 	ldr.w	r0, [sl]
30009b18:	900c      	str	r0, [sp, #48]	; 0x30
30009b1a:	6818      	ldr	r0, [r3, #0]
30009b1c:	4b4e      	ldr	r3, [pc, #312]	; (30009c58 <PSRAM_calibration+0x294>)
30009b1e:	900d      	str	r0, [sp, #52]	; 0x34
30009b20:	a80a      	add	r0, sp, #40	; 0x28
30009b22:	6809      	ldr	r1, [r1, #0]
30009b24:	910e      	str	r1, [sp, #56]	; 0x38
30009b26:	6812      	ldr	r2, [r2, #0]
30009b28:	4947      	ldr	r1, [pc, #284]	; (30009c48 <PSRAM_calibration+0x284>)
30009b2a:	920f      	str	r2, [sp, #60]	; 0x3c
30009b2c:	2218      	movs	r2, #24
30009b2e:	4798      	blx	r3
30009b30:	2800      	cmp	r0, #0
30009b32:	d0c0      	beq.n	30009ab6 <PSRAM_calibration+0xf2>
30009b34:	1c70      	adds	r0, r6, #1
30009b36:	d0cb      	beq.n	30009ad0 <PSRAM_calibration+0x10c>
30009b38:	9b02      	ldr	r3, [sp, #8]
30009b3a:	42bb      	cmp	r3, r7
30009b3c:	da0a      	bge.n	30009b54 <PSRAM_calibration+0x190>
30009b3e:	9b03      	ldr	r3, [sp, #12]
30009b40:	f8cd b018 	str.w	fp, [sp, #24]
30009b44:	f04f 3bff 	mov.w	fp, #4294967295
30009b48:	9702      	str	r7, [sp, #8]
30009b4a:	2700      	movs	r7, #0
30009b4c:	9605      	str	r6, [sp, #20]
30009b4e:	465e      	mov	r6, fp
30009b50:	9304      	str	r3, [sp, #16]
30009b52:	e7bd      	b.n	30009ad0 <PSRAM_calibration+0x10c>
30009b54:	f04f 3bff 	mov.w	fp, #4294967295
30009b58:	2700      	movs	r7, #0
30009b5a:	465e      	mov	r6, fp
30009b5c:	e7b8      	b.n	30009ad0 <PSRAM_calibration+0x10c>
30009b5e:	9b03      	ldr	r3, [sp, #12]
30009b60:	9304      	str	r3, [sp, #16]
30009b62:	9b03      	ldr	r3, [sp, #12]
30009b64:	005b      	lsls	r3, r3, #1
30009b66:	9303      	str	r3, [sp, #12]
30009b68:	9b07      	ldr	r3, [sp, #28]
30009b6a:	3b01      	subs	r3, #1
30009b6c:	9307      	str	r3, [sp, #28]
30009b6e:	d195      	bne.n	30009a9c <PSRAM_calibration+0xd8>
30009b70:	4b3a      	ldr	r3, [pc, #232]	; (30009c5c <PSRAM_calibration+0x298>)
30009b72:	681b      	ldr	r3, [r3, #0]
30009b74:	0399      	lsls	r1, r3, #14
30009b76:	d454      	bmi.n	30009c22 <PSRAM_calibration+0x25e>
30009b78:	4832      	ldr	r0, [pc, #200]	; (30009c44 <PSRAM_calibration+0x280>)
30009b7a:	6943      	ldr	r3, [r0, #20]
30009b7c:	f413 3380 	ands.w	r3, r3, #65536	; 0x10000
30009b80:	d124      	bne.n	30009bcc <PSRAM_calibration+0x208>
30009b82:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009b86:	f3bf 8f4f 	dsb	sy
30009b8a:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009b8e:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30009b92:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009b96:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009b9a:	0164      	lsls	r4, r4, #5
30009b9c:	ea04 010c 	and.w	r1, r4, ip
30009ba0:	462b      	mov	r3, r5
30009ba2:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009ba6:	3b01      	subs	r3, #1
30009ba8:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30009bac:	1c5a      	adds	r2, r3, #1
30009bae:	d1f8      	bne.n	30009ba2 <PSRAM_calibration+0x1de>
30009bb0:	3c20      	subs	r4, #32
30009bb2:	f114 0f20 	cmn.w	r4, #32
30009bb6:	d1f1      	bne.n	30009b9c <PSRAM_calibration+0x1d8>
30009bb8:	f3bf 8f4f 	dsb	sy
30009bbc:	6943      	ldr	r3, [r0, #20]
30009bbe:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30009bc2:	6143      	str	r3, [r0, #20]
30009bc4:	f3bf 8f4f 	dsb	sy
30009bc8:	f3bf 8f6f 	isb	sy
30009bcc:	2f08      	cmp	r7, #8
30009bce:	dd24      	ble.n	30009c1a <PSRAM_calibration+0x256>
30009bd0:	ebbb 0306 	subs.w	r3, fp, r6
30009bd4:	9804      	ldr	r0, [sp, #16]
30009bd6:	bf48      	it	mi
30009bd8:	3301      	addmi	r3, #1
30009bda:	eb1b 0606 	adds.w	r6, fp, r6
30009bde:	ea4f 0363 	mov.w	r3, r3, asr #1
30009be2:	bf48      	it	mi
30009be4:	3601      	addmi	r6, #1
30009be6:	1e9a      	subs	r2, r3, #2
30009be8:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009bea:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009bee:	0414      	lsls	r4, r2, #16
30009bf0:	f3c6 0344 	ubfx	r3, r6, #1, #5
30009bf4:	0212      	lsls	r2, r2, #8
30009bf6:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009bfa:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009bfe:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009c02:	2001      	movs	r0, #1
30009c04:	430b      	orrs	r3, r1
30009c06:	490d      	ldr	r1, [pc, #52]	; (30009c3c <PSRAM_calibration+0x278>)
30009c08:	4323      	orrs	r3, r4
30009c0a:	4313      	orrs	r3, r2
30009c0c:	604b      	str	r3, [r1, #4]
30009c0e:	680b      	ldr	r3, [r1, #0]
30009c10:	4303      	orrs	r3, r0
30009c12:	600b      	str	r3, [r1, #0]
30009c14:	b011      	add	sp, #68	; 0x44
30009c16:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009c1a:	2000      	movs	r0, #0
30009c1c:	b011      	add	sp, #68	; 0x44
30009c1e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009c22:	9b04      	ldr	r3, [sp, #16]
30009c24:	465a      	mov	r2, fp
30009c26:	4631      	mov	r1, r6
30009c28:	480d      	ldr	r0, [pc, #52]	; (30009c60 <PSRAM_calibration+0x29c>)
30009c2a:	9300      	str	r3, [sp, #0]
30009c2c:	463b      	mov	r3, r7
30009c2e:	f002 fac3 	bl	3000c1b8 <__DiagPrintf_veneer>
30009c32:	e7a1      	b.n	30009b78 <PSRAM_calibration+0x1b4>
30009c34:	9f02      	ldr	r7, [sp, #8]
30009c36:	e9dd 6b05 	ldrd	r6, fp, [sp, #20]
30009c3a:	e792      	b.n	30009b62 <PSRAM_calibration+0x19e>
30009c3c:	41002000 	.word	0x41002000
30009c40:	0fffffe0 	.word	0x0fffffe0
30009c44:	e000ed00 	.word	0xe000ed00
30009c48:	3000e4b0 	.word	0x3000e4b0
30009c4c:	60200000 	.word	0x60200000
30009c50:	60150000 	.word	0x60150000
30009c54:	60250000 	.word	0x60250000
30009c58:	000129bd 	.word	0x000129bd
30009c5c:	2001c00c 	.word	0x2001c00c
30009c60:	3000d548 	.word	0x3000d548
30009c64:	60050000 	.word	0x60050000
30009c68:	60100000 	.word	0x60100000

30009c6c <PSRAM_AutoGating>:
30009c6c:	b1c0      	cbz	r0, 30009ca0 <PSRAM_AutoGating+0x34>
30009c6e:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30009c72:	4810      	ldr	r0, [pc, #64]	; (30009cb4 <PSRAM_AutoGating+0x48>)
30009c74:	f3c1 0108 	ubfx	r1, r1, #0, #9
30009c78:	0252      	lsls	r2, r2, #9
30009c7a:	b410      	push	{r4}
30009c7c:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
30009c80:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
30009c84:	4020      	ands	r0, r4
30009c86:	4301      	orrs	r1, r0
30009c88:	430a      	orrs	r2, r1
30009c8a:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30009c8e:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
30009c92:	f042 0201 	orr.w	r2, r2, #1
30009c96:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30009c9a:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c9e:	4770      	bx	lr
30009ca0:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30009ca4:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30009ca8:	f023 0301 	bic.w	r3, r3, #1
30009cac:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30009cb0:	4770      	bx	lr
30009cb2:	bf00      	nop
30009cb4:	ffffc000 	.word	0xffffc000

30009cb8 <BOOT_Reason>:
30009cb8:	4b02      	ldr	r3, [pc, #8]	; (30009cc4 <BOOT_Reason+0xc>)
30009cba:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009cbe:	b280      	uxth	r0, r0
30009cc0:	4770      	bx	lr
30009cc2:	bf00      	nop
30009cc4:	42008200 	.word	0x42008200

30009cc8 <CRYPTO_SetSecurityModeAD>:
30009cc8:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009ccc:	460d      	mov	r5, r1
30009cce:	4616      	mov	r6, r2
30009cd0:	4699      	mov	r9, r3
30009cd2:	4604      	mov	r4, r0
30009cd4:	e9dd 8709 	ldrd	r8, r7, [sp, #36]	; 0x24
30009cd8:	2800      	cmp	r0, #0
30009cda:	f000 80a6 	beq.w	30009e2a <CRYPTO_SetSecurityModeAD+0x162>
30009cde:	7823      	ldrb	r3, [r4, #0]
30009ce0:	2b01      	cmp	r3, #1
30009ce2:	d003      	beq.n	30009cec <CRYPTO_SetSecurityModeAD+0x24>
30009ce4:	2124      	movs	r1, #36	; 0x24
30009ce6:	485d      	ldr	r0, [pc, #372]	; (30009e5c <CRYPTO_SetSecurityModeAD+0x194>)
30009ce8:	f002 fa86 	bl	3000c1f8 <__io_assert_failed_veneer>
30009cec:	2300      	movs	r3, #0
30009cee:	1c69      	adds	r1, r5, #1
30009cf0:	6065      	str	r5, [r4, #4]
30009cf2:	8123      	strh	r3, [r4, #8]
30009cf4:	d008      	beq.n	30009d08 <CRYPTO_SetSecurityModeAD+0x40>
30009cf6:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009cfa:	2b20      	cmp	r3, #32
30009cfc:	d078      	beq.n	30009df0 <CRYPTO_SetSecurityModeAD+0x128>
30009cfe:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30009d02:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30009d06:	7265      	strb	r5, [r4, #9]
30009d08:	1c72      	adds	r2, r6, #1
30009d0a:	60e6      	str	r6, [r4, #12]
30009d0c:	d07f      	beq.n	30009e0e <CRYPTO_SetSecurityModeAD+0x146>
30009d0e:	f006 0303 	and.w	r3, r6, #3
30009d12:	f3c6 0280 	ubfx	r2, r6, #2, #1
30009d16:	7422      	strb	r2, [r4, #16]
30009d18:	f1a3 0202 	sub.w	r2, r3, #2
30009d1c:	2b01      	cmp	r3, #1
30009d1e:	fab2 f282 	clz	r2, r2
30009d22:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009d26:	7462      	strb	r2, [r4, #17]
30009d28:	fab3 f283 	clz	r2, r3
30009d2c:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009d30:	74a2      	strb	r2, [r4, #18]
30009d32:	bf0c      	ite	eq
30009d34:	2201      	moveq	r2, #1
30009d36:	2200      	movne	r2, #0
30009d38:	74e2      	strb	r2, [r4, #19]
30009d3a:	d13e      	bne.n	30009dba <CRYPTO_SetSecurityModeAD+0xf2>
30009d3c:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30009d40:	2b40      	cmp	r3, #64	; 0x40
30009d42:	d07e      	beq.n	30009e42 <CRYPTO_SetSecurityModeAD+0x17a>
30009d44:	d869      	bhi.n	30009e1a <CRYPTO_SetSecurityModeAD+0x152>
30009d46:	2b10      	cmp	r3, #16
30009d48:	d06c      	beq.n	30009e24 <CRYPTO_SetSecurityModeAD+0x15c>
30009d4a:	2b20      	cmp	r3, #32
30009d4c:	d172      	bne.n	30009e34 <CRYPTO_SetSecurityModeAD+0x16c>
30009d4e:	7523      	strb	r3, [r4, #20]
30009d50:	62a3      	str	r3, [r4, #40]	; 0x28
30009d52:	9b08      	ldr	r3, [sp, #32]
30009d54:	6227      	str	r7, [r4, #32]
30009d56:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009d5a:	2f00      	cmp	r7, #0
30009d5c:	d038      	beq.n	30009dd0 <CRYPTO_SetSecurityModeAD+0x108>
30009d5e:	7d23      	ldrb	r3, [r4, #20]
30009d60:	f104 0064 	add.w	r0, r4, #100	; 0x64
30009d64:	2b30      	cmp	r3, #48	; 0x30
30009d66:	65e0      	str	r0, [r4, #92]	; 0x5c
30009d68:	d045      	beq.n	30009df6 <CRYPTO_SetSecurityModeAD+0x12e>
30009d6a:	2b40      	cmp	r3, #64	; 0x40
30009d6c:	d043      	beq.n	30009df6 <CRYPTO_SetSecurityModeAD+0x12e>
30009d6e:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30009d72:	2240      	movs	r2, #64	; 0x40
30009d74:	2136      	movs	r1, #54	; 0x36
30009d76:	4d3a      	ldr	r5, [pc, #232]	; (30009e60 <CRYPTO_SetSecurityModeAD+0x198>)
30009d78:	6623      	str	r3, [r4, #96]	; 0x60
30009d7a:	47a8      	blx	r5
30009d7c:	2240      	movs	r2, #64	; 0x40
30009d7e:	215c      	movs	r1, #92	; 0x5c
30009d80:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009d82:	47a8      	blx	r5
30009d84:	4b37      	ldr	r3, [pc, #220]	; (30009e64 <CRYPTO_SetSecurityModeAD+0x19c>)
30009d86:	681d      	ldr	r5, [r3, #0]
30009d88:	bb25      	cbnz	r5, 30009dd4 <CRYPTO_SetSecurityModeAD+0x10c>
30009d8a:	f1b8 0f00 	cmp.w	r8, #0
30009d8e:	d060      	beq.n	30009e52 <CRYPTO_SetSecurityModeAD+0x18a>
30009d90:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30009d94:	e001      	b.n	30009d9a <CRYPTO_SetSecurityModeAD+0xd2>
30009d96:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30009d9a:	6de2      	ldr	r2, [r4, #92]	; 0x5c
30009d9c:	f818 3005 	ldrb.w	r3, [r8, r5]
30009da0:	5d51      	ldrb	r1, [r2, r5]
30009da2:	404b      	eors	r3, r1
30009da4:	5553      	strb	r3, [r2, r5]
30009da6:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009da8:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009daa:	5d51      	ldrb	r1, [r2, r5]
30009dac:	5d5b      	ldrb	r3, [r3, r5]
30009dae:	404b      	eors	r3, r1
30009db0:	5553      	strb	r3, [r2, r5]
30009db2:	3501      	adds	r5, #1
30009db4:	42af      	cmp	r7, r5
30009db6:	d1ee      	bne.n	30009d96 <CRYPTO_SetSecurityModeAD+0xce>
30009db8:	e00c      	b.n	30009dd4 <CRYPTO_SetSecurityModeAD+0x10c>
30009dba:	2200      	movs	r2, #0
30009dbc:	7522      	strb	r2, [r4, #20]
30009dbe:	b993      	cbnz	r3, 30009de6 <CRYPTO_SetSecurityModeAD+0x11e>
30009dc0:	2314      	movs	r3, #20
30009dc2:	6227      	str	r7, [r4, #32]
30009dc4:	62a3      	str	r3, [r4, #40]	; 0x28
30009dc6:	9b08      	ldr	r3, [sp, #32]
30009dc8:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009dcc:	2f00      	cmp	r7, #0
30009dce:	d1c6      	bne.n	30009d5e <CRYPTO_SetSecurityModeAD+0x96>
30009dd0:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009dd4:	3601      	adds	r6, #1
30009dd6:	d003      	beq.n	30009de0 <CRYPTO_SetSecurityModeAD+0x118>
30009dd8:	2201      	movs	r2, #1
30009dda:	2300      	movs	r3, #0
30009ddc:	85a2      	strh	r2, [r4, #44]	; 0x2c
30009dde:	6323      	str	r3, [r4, #48]	; 0x30
30009de0:	2000      	movs	r0, #0
30009de2:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009de6:	2b02      	cmp	r3, #2
30009de8:	d114      	bne.n	30009e14 <CRYPTO_SetSecurityModeAD+0x14c>
30009dea:	2310      	movs	r3, #16
30009dec:	62a3      	str	r3, [r4, #40]	; 0x28
30009dee:	e7b0      	b.n	30009d52 <CRYPTO_SetSecurityModeAD+0x8a>
30009df0:	2301      	movs	r3, #1
30009df2:	7223      	strb	r3, [r4, #8]
30009df4:	e783      	b.n	30009cfe <CRYPTO_SetSecurityModeAD+0x36>
30009df6:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30009dfa:	2280      	movs	r2, #128	; 0x80
30009dfc:	2136      	movs	r1, #54	; 0x36
30009dfe:	4d18      	ldr	r5, [pc, #96]	; (30009e60 <CRYPTO_SetSecurityModeAD+0x198>)
30009e00:	6623      	str	r3, [r4, #96]	; 0x60
30009e02:	47a8      	blx	r5
30009e04:	2280      	movs	r2, #128	; 0x80
30009e06:	215c      	movs	r1, #92	; 0x5c
30009e08:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009e0a:	47a8      	blx	r5
30009e0c:	e7ba      	b.n	30009d84 <CRYPTO_SetSecurityModeAD+0xbc>
30009e0e:	2300      	movs	r3, #0
30009e10:	6123      	str	r3, [r4, #16]
30009e12:	7523      	strb	r3, [r4, #20]
30009e14:	2300      	movs	r3, #0
30009e16:	62a3      	str	r3, [r4, #40]	; 0x28
30009e18:	e79b      	b.n	30009d52 <CRYPTO_SetSecurityModeAD+0x8a>
30009e1a:	2b80      	cmp	r3, #128	; 0x80
30009e1c:	d10a      	bne.n	30009e34 <CRYPTO_SetSecurityModeAD+0x16c>
30009e1e:	2340      	movs	r3, #64	; 0x40
30009e20:	7523      	strb	r3, [r4, #20]
30009e22:	e795      	b.n	30009d50 <CRYPTO_SetSecurityModeAD+0x88>
30009e24:	231c      	movs	r3, #28
30009e26:	7523      	strb	r3, [r4, #20]
30009e28:	e7f5      	b.n	30009e16 <CRYPTO_SetSecurityModeAD+0x14e>
30009e2a:	2123      	movs	r1, #35	; 0x23
30009e2c:	480b      	ldr	r0, [pc, #44]	; (30009e5c <CRYPTO_SetSecurityModeAD+0x194>)
30009e2e:	f002 f9e3 	bl	3000c1f8 <__io_assert_failed_veneer>
30009e32:	e754      	b.n	30009cde <CRYPTO_SetSecurityModeAD+0x16>
30009e34:	4b0c      	ldr	r3, [pc, #48]	; (30009e68 <CRYPTO_SetSecurityModeAD+0x1a0>)
30009e36:	681b      	ldr	r3, [r3, #0]
30009e38:	049b      	lsls	r3, r3, #18
30009e3a:	d405      	bmi.n	30009e48 <CRYPTO_SetSecurityModeAD+0x180>
30009e3c:	2300      	movs	r3, #0
30009e3e:	7523      	strb	r3, [r4, #20]
30009e40:	e7e9      	b.n	30009e16 <CRYPTO_SetSecurityModeAD+0x14e>
30009e42:	2330      	movs	r3, #48	; 0x30
30009e44:	7523      	strb	r3, [r4, #20]
30009e46:	e7e6      	b.n	30009e16 <CRYPTO_SetSecurityModeAD+0x14e>
30009e48:	4631      	mov	r1, r6
30009e4a:	4808      	ldr	r0, [pc, #32]	; (30009e6c <CRYPTO_SetSecurityModeAD+0x1a4>)
30009e4c:	f002 f9b4 	bl	3000c1b8 <__DiagPrintf_veneer>
30009e50:	e7f4      	b.n	30009e3c <CRYPTO_SetSecurityModeAD+0x174>
30009e52:	217f      	movs	r1, #127	; 0x7f
30009e54:	4801      	ldr	r0, [pc, #4]	; (30009e5c <CRYPTO_SetSecurityModeAD+0x194>)
30009e56:	f002 f9cf 	bl	3000c1f8 <__io_assert_failed_veneer>
30009e5a:	e799      	b.n	30009d90 <CRYPTO_SetSecurityModeAD+0xc8>
30009e5c:	3000d620 	.word	0x3000d620
30009e60:	00012be5 	.word	0x00012be5
30009e64:	2001c200 	.word	0x2001c200
30009e68:	2001c00c 	.word	0x2001c00c
30009e6c:	3000d580 	.word	0x3000d580

30009e70 <rtl_crypto_hmac_sha2_init>:
30009e70:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009e74:	4c97      	ldr	r4, [pc, #604]	; (3000a0d4 <rtl_crypto_hmac_sha2_init+0x264>)
30009e76:	4617      	mov	r7, r2
30009e78:	e844 f400 	tt	r4, r4
30009e7c:	4a96      	ldr	r2, [pc, #600]	; (3000a0d8 <rtl_crypto_hmac_sha2_init+0x268>)
30009e7e:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
30009e82:	461c      	mov	r4, r3
30009e84:	4b95      	ldr	r3, [pc, #596]	; (3000a0dc <rtl_crypto_hmac_sha2_init+0x26c>)
30009e86:	bf18      	it	ne
30009e88:	4613      	movne	r3, r2
30009e8a:	b085      	sub	sp, #20
30009e8c:	4605      	mov	r5, r0
30009e8e:	460e      	mov	r6, r1
30009e90:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30009e92:	2b00      	cmp	r3, #0
30009e94:	db05      	blt.n	30009ea2 <rtl_crypto_hmac_sha2_init+0x32>
30009e96:	2900      	cmp	r1, #0
30009e98:	f000 8118 	beq.w	3000a0cc <rtl_crypto_hmac_sha2_init+0x25c>
30009e9c:	078b      	lsls	r3, r1, #30
30009e9e:	f040 8112 	bne.w	3000a0c6 <rtl_crypto_hmac_sha2_init+0x256>
30009ea2:	2d20      	cmp	r5, #32
30009ea4:	bf18      	it	ne
30009ea6:	2d1c      	cmpne	r5, #28
30009ea8:	bf0c      	ite	eq
30009eaa:	f04f 0901 	moveq.w	r9, #1
30009eae:	f04f 0900 	movne.w	r9, #0
30009eb2:	d04d      	beq.n	30009f50 <rtl_crypto_hmac_sha2_init+0xe0>
30009eb4:	2d40      	cmp	r5, #64	; 0x40
30009eb6:	bf18      	it	ne
30009eb8:	2d30      	cmpne	r5, #48	; 0x30
30009eba:	d102      	bne.n	30009ec2 <rtl_crypto_hmac_sha2_init+0x52>
30009ebc:	2f80      	cmp	r7, #128	; 0x80
30009ebe:	f200 80f9 	bhi.w	3000a0b4 <rtl_crypto_hmac_sha2_init+0x244>
30009ec2:	f8df 8238 	ldr.w	r8, [pc, #568]	; 3000a0fc <rtl_crypto_hmac_sha2_init+0x28c>
30009ec6:	f898 3000 	ldrb.w	r3, [r8]
30009eca:	2b01      	cmp	r3, #1
30009ecc:	d003      	beq.n	30009ed6 <rtl_crypto_hmac_sha2_init+0x66>
30009ece:	21b3      	movs	r1, #179	; 0xb3
30009ed0:	4883      	ldr	r0, [pc, #524]	; (3000a0e0 <rtl_crypto_hmac_sha2_init+0x270>)
30009ed2:	f002 f991 	bl	3000c1f8 <__io_assert_failed_veneer>
30009ed6:	2d30      	cmp	r5, #48	; 0x30
30009ed8:	f000 809b 	beq.w	3000a012 <rtl_crypto_hmac_sha2_init+0x1a2>
30009edc:	d83f      	bhi.n	30009f5e <rtl_crypto_hmac_sha2_init+0xee>
30009ede:	2d1c      	cmp	r5, #28
30009ee0:	f000 80cb 	beq.w	3000a07a <rtl_crypto_hmac_sha2_init+0x20a>
30009ee4:	f1b9 0f00 	cmp.w	r9, #0
30009ee8:	d078      	beq.n	30009fdc <rtl_crypto_hmac_sha2_init+0x16c>
30009eea:	2c00      	cmp	r4, #0
30009eec:	f000 808f 	beq.w	3000a00e <rtl_crypto_hmac_sha2_init+0x19e>
30009ef0:	4b7c      	ldr	r3, [pc, #496]	; (3000a0e4 <rtl_crypto_hmac_sha2_init+0x274>)
30009ef2:	f8df e210 	ldr.w	lr, [pc, #528]	; 3000a104 <rtl_crypto_hmac_sha2_init+0x294>
30009ef6:	f8df 921c 	ldr.w	r9, [pc, #540]	; 3000a114 <rtl_crypto_hmac_sha2_init+0x2a4>
30009efa:	4a7b      	ldr	r2, [pc, #492]	; (3000a0e8 <rtl_crypto_hmac_sha2_init+0x278>)
30009efc:	e9c4 e302 	strd	lr, r3, [r4, #8]
30009f00:	4b7a      	ldr	r3, [pc, #488]	; (3000a0ec <rtl_crypto_hmac_sha2_init+0x27c>)
30009f02:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009f06:	4b7a      	ldr	r3, [pc, #488]	; (3000a0f0 <rtl_crypto_hmac_sha2_init+0x280>)
30009f08:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009f0c:	4b79      	ldr	r3, [pc, #484]	; (3000a0f4 <rtl_crypto_hmac_sha2_init+0x284>)
30009f0e:	2225      	movs	r2, #37	; 0x25
30009f10:	6223      	str	r3, [r4, #32]
30009f12:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009f16:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009f1a:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30009f1e:	6263      	str	r3, [r4, #36]	; 0x24
30009f20:	2300      	movs	r3, #0
30009f22:	f04f 31ff 	mov.w	r1, #4294967295
30009f26:	4d74      	ldr	r5, [pc, #464]	; (3000a0f8 <rtl_crypto_hmac_sha2_init+0x288>)
30009f28:	9300      	str	r3, [sp, #0]
30009f2a:	9702      	str	r7, [sp, #8]
30009f2c:	9601      	str	r6, [sp, #4]
30009f2e:	4873      	ldr	r0, [pc, #460]	; (3000a0fc <rtl_crypto_hmac_sha2_init+0x28c>)
30009f30:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009f34:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009f38:	47a8      	blx	r5
30009f3a:	4605      	mov	r5, r0
30009f3c:	4b70      	ldr	r3, [pc, #448]	; (3000a100 <rtl_crypto_hmac_sha2_init+0x290>)
30009f3e:	4620      	mov	r0, r4
30009f40:	4798      	blx	r3
30009f42:	2301      	movs	r3, #1
30009f44:	4628      	mov	r0, r5
30009f46:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30009f4a:	b005      	add	sp, #20
30009f4c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009f50:	2f40      	cmp	r7, #64	; 0x40
30009f52:	d9b6      	bls.n	30009ec2 <rtl_crypto_hmac_sha2_init+0x52>
30009f54:	21ad      	movs	r1, #173	; 0xad
30009f56:	4862      	ldr	r0, [pc, #392]	; (3000a0e0 <rtl_crypto_hmac_sha2_init+0x270>)
30009f58:	f002 f94e 	bl	3000c1f8 <__io_assert_failed_veneer>
30009f5c:	e7b1      	b.n	30009ec2 <rtl_crypto_hmac_sha2_init+0x52>
30009f5e:	2d40      	cmp	r5, #64	; 0x40
30009f60:	d13c      	bne.n	30009fdc <rtl_crypto_hmac_sha2_init+0x16c>
30009f62:	2c00      	cmp	r4, #0
30009f64:	d040      	beq.n	30009fe8 <rtl_crypto_hmac_sha2_init+0x178>
30009f66:	4b67      	ldr	r3, [pc, #412]	; (3000a104 <rtl_crypto_hmac_sha2_init+0x294>)
30009f68:	4a67      	ldr	r2, [pc, #412]	; (3000a108 <rtl_crypto_hmac_sha2_init+0x298>)
30009f6a:	4968      	ldr	r1, [pc, #416]	; (3000a10c <rtl_crypto_hmac_sha2_init+0x29c>)
30009f6c:	4d68      	ldr	r5, [pc, #416]	; (3000a110 <rtl_crypto_hmac_sha2_init+0x2a0>)
30009f6e:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 3000a154 <rtl_crypto_hmac_sha2_init+0x2e4>
30009f72:	e9c4 3202 	strd	r3, r2, [r4, #8]
30009f76:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30009f7a:	2285      	movs	r2, #133	; 0x85
30009f7c:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30009f80:	33d4      	adds	r3, #212	; 0xd4
30009f82:	6163      	str	r3, [r4, #20]
30009f84:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30009f88:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30009f8c:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30009f90:	6123      	str	r3, [r4, #16]
30009f92:	4b60      	ldr	r3, [pc, #384]	; (3000a114 <rtl_crypto_hmac_sha2_init+0x2a4>)
30009f94:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009f98:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30009f9c:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30009fa0:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009fa4:	6263      	str	r3, [r4, #36]	; 0x24
30009fa6:	4b51      	ldr	r3, [pc, #324]	; (3000a0ec <rtl_crypto_hmac_sha2_init+0x27c>)
30009fa8:	6223      	str	r3, [r4, #32]
30009faa:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30009fae:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30009fb2:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009fb6:	62e3      	str	r3, [r4, #44]	; 0x2c
30009fb8:	4b4b      	ldr	r3, [pc, #300]	; (3000a0e8 <rtl_crypto_hmac_sha2_init+0x278>)
30009fba:	62a3      	str	r3, [r4, #40]	; 0x28
30009fbc:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30009fc0:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30009fc4:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009fc8:	6363      	str	r3, [r4, #52]	; 0x34
30009fca:	4b49      	ldr	r3, [pc, #292]	; (3000a0f0 <rtl_crypto_hmac_sha2_init+0x280>)
30009fcc:	6323      	str	r3, [r4, #48]	; 0x30
30009fce:	4b49      	ldr	r3, [pc, #292]	; (3000a0f4 <rtl_crypto_hmac_sha2_init+0x284>)
30009fd0:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009fd4:	4b50      	ldr	r3, [pc, #320]	; (3000a118 <rtl_crypto_hmac_sha2_init+0x2a8>)
30009fd6:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009fda:	e7a1      	b.n	30009f20 <rtl_crypto_hmac_sha2_init+0xb0>
30009fdc:	f04f 35ff 	mov.w	r5, #4294967295
30009fe0:	4628      	mov	r0, r5
30009fe2:	b005      	add	sp, #20
30009fe4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009fe8:	2285      	movs	r2, #133	; 0x85
30009fea:	2300      	movs	r3, #0
30009fec:	f04f 31ff 	mov.w	r1, #4294967295
30009ff0:	4c41      	ldr	r4, [pc, #260]	; (3000a0f8 <rtl_crypto_hmac_sha2_init+0x288>)
30009ff2:	9702      	str	r7, [sp, #8]
30009ff4:	4841      	ldr	r0, [pc, #260]	; (3000a0fc <rtl_crypto_hmac_sha2_init+0x28c>)
30009ff6:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009ffa:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009ffe:	e9cd 3600 	strd	r3, r6, [sp]
3000a002:	47a0      	blx	r4
3000a004:	4605      	mov	r5, r0
3000a006:	4628      	mov	r0, r5
3000a008:	b005      	add	sp, #20
3000a00a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000a00e:	2225      	movs	r2, #37	; 0x25
3000a010:	e7eb      	b.n	30009fea <rtl_crypto_hmac_sha2_init+0x17a>
3000a012:	2c00      	cmp	r4, #0
3000a014:	d053      	beq.n	3000a0be <rtl_crypto_hmac_sha2_init+0x24e>
3000a016:	4b41      	ldr	r3, [pc, #260]	; (3000a11c <rtl_crypto_hmac_sha2_init+0x2ac>)
3000a018:	f8df e130 	ldr.w	lr, [pc, #304]	; 3000a14c <rtl_crypto_hmac_sha2_init+0x2dc>
3000a01c:	4a40      	ldr	r2, [pc, #256]	; (3000a120 <rtl_crypto_hmac_sha2_init+0x2b0>)
3000a01e:	4941      	ldr	r1, [pc, #260]	; (3000a124 <rtl_crypto_hmac_sha2_init+0x2b4>)
3000a020:	4d41      	ldr	r5, [pc, #260]	; (3000a128 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000a022:	f8df c12c 	ldr.w	ip, [pc, #300]	; 3000a150 <rtl_crypto_hmac_sha2_init+0x2e0>
3000a026:	e9c4 3e02 	strd	r3, lr, [r4, #8]
3000a02a:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
3000a02e:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
3000a032:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
3000a036:	6163      	str	r3, [r4, #20]
3000a038:	4b3c      	ldr	r3, [pc, #240]	; (3000a12c <rtl_crypto_hmac_sha2_init+0x2bc>)
3000a03a:	6123      	str	r3, [r4, #16]
3000a03c:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
3000a040:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
3000a044:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
3000a048:	61e3      	str	r3, [r4, #28]
3000a04a:	4b39      	ldr	r3, [pc, #228]	; (3000a130 <rtl_crypto_hmac_sha2_init+0x2c0>)
3000a04c:	61a3      	str	r3, [r4, #24]
3000a04e:	4b39      	ldr	r3, [pc, #228]	; (3000a134 <rtl_crypto_hmac_sha2_init+0x2c4>)
3000a050:	e9c4 3208 	strd	r3, r2, [r4, #32]
3000a054:	4b38      	ldr	r3, [pc, #224]	; (3000a138 <rtl_crypto_hmac_sha2_init+0x2c8>)
3000a056:	2245      	movs	r2, #69	; 0x45
3000a058:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
3000a05c:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
3000a060:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
3000a064:	33aa      	adds	r3, #170	; 0xaa
3000a066:	6363      	str	r3, [r4, #52]	; 0x34
3000a068:	4b34      	ldr	r3, [pc, #208]	; (3000a13c <rtl_crypto_hmac_sha2_init+0x2cc>)
3000a06a:	6323      	str	r3, [r4, #48]	; 0x30
3000a06c:	4b34      	ldr	r3, [pc, #208]	; (3000a140 <rtl_crypto_hmac_sha2_init+0x2d0>)
3000a06e:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
3000a072:	4b34      	ldr	r3, [pc, #208]	; (3000a144 <rtl_crypto_hmac_sha2_init+0x2d4>)
3000a074:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
3000a078:	e752      	b.n	30009f20 <rtl_crypto_hmac_sha2_init+0xb0>
3000a07a:	b314      	cbz	r4, 3000a0c2 <rtl_crypto_hmac_sha2_init+0x252>
3000a07c:	4b32      	ldr	r3, [pc, #200]	; (3000a148 <rtl_crypto_hmac_sha2_init+0x2d8>)
3000a07e:	2215      	movs	r2, #21
3000a080:	4932      	ldr	r1, [pc, #200]	; (3000a14c <rtl_crypto_hmac_sha2_init+0x2dc>)
3000a082:	4827      	ldr	r0, [pc, #156]	; (3000a120 <rtl_crypto_hmac_sha2_init+0x2b0>)
3000a084:	4d28      	ldr	r5, [pc, #160]	; (3000a128 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000a086:	e9c4 1302 	strd	r1, r3, [r4, #8]
3000a08a:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
3000a08e:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
3000a092:	f503 6301 	add.w	r3, r3, #2064	; 0x810
3000a096:	6123      	str	r3, [r4, #16]
3000a098:	4b22      	ldr	r3, [pc, #136]	; (3000a124 <rtl_crypto_hmac_sha2_init+0x2b4>)
3000a09a:	e9c4 0305 	strd	r0, r3, [r4, #20]
3000a09e:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
3000a0a2:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
3000a0a6:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
3000a0aa:	61e3      	str	r3, [r4, #28]
3000a0ac:	4b28      	ldr	r3, [pc, #160]	; (3000a150 <rtl_crypto_hmac_sha2_init+0x2e0>)
3000a0ae:	e9c4 5308 	strd	r5, r3, [r4, #32]
3000a0b2:	e735      	b.n	30009f20 <rtl_crypto_hmac_sha2_init+0xb0>
3000a0b4:	21b0      	movs	r1, #176	; 0xb0
3000a0b6:	480a      	ldr	r0, [pc, #40]	; (3000a0e0 <rtl_crypto_hmac_sha2_init+0x270>)
3000a0b8:	f002 f89e 	bl	3000c1f8 <__io_assert_failed_veneer>
3000a0bc:	e701      	b.n	30009ec2 <rtl_crypto_hmac_sha2_init+0x52>
3000a0be:	2245      	movs	r2, #69	; 0x45
3000a0c0:	e793      	b.n	30009fea <rtl_crypto_hmac_sha2_init+0x17a>
3000a0c2:	2215      	movs	r2, #21
3000a0c4:	e791      	b.n	30009fea <rtl_crypto_hmac_sha2_init+0x17a>
3000a0c6:	f06f 0505 	mvn.w	r5, #5
3000a0ca:	e789      	b.n	30009fe0 <rtl_crypto_hmac_sha2_init+0x170>
3000a0cc:	f06f 0503 	mvn.w	r5, #3
3000a0d0:	e786      	b.n	30009fe0 <rtl_crypto_hmac_sha2_init+0x170>
3000a0d2:	bf00      	nop
3000a0d4:	0000e5f9 	.word	0x0000e5f9
3000a0d8:	500c8000 	.word	0x500c8000
3000a0dc:	400c8000 	.word	0x400c8000
3000a0e0:	3000d604 	.word	0x3000d604
3000a0e4:	bb67ae85 	.word	0xbb67ae85
3000a0e8:	510e527f 	.word	0x510e527f
3000a0ec:	a54ff53a 	.word	0xa54ff53a
3000a0f0:	9b05688c 	.word	0x9b05688c
3000a0f4:	1f83d9ab 	.word	0x1f83d9ab
3000a0f8:	30009cc9 	.word	0x30009cc9
3000a0fc:	2001c460 	.word	0x2001c460
3000a100:	00004c99 	.word	0x00004c99
3000a104:	6a09e667 	.word	0x6a09e667
3000a108:	f3bcc908 	.word	0xf3bcc908
3000a10c:	fe94f82b 	.word	0xfe94f82b
3000a110:	fb41bd6b 	.word	0xfb41bd6b
3000a114:	3c6ef372 	.word	0x3c6ef372
3000a118:	5be0cd19 	.word	0x5be0cd19
3000a11c:	cbbb9d5d 	.word	0xcbbb9d5d
3000a120:	f70e5939 	.word	0xf70e5939
3000a124:	ffc00b31 	.word	0xffc00b31
3000a128:	64f98fa7 	.word	0x64f98fa7
3000a12c:	629a292a 	.word	0x629a292a
3000a130:	9159015a 	.word	0x9159015a
3000a134:	152fecd8 	.word	0x152fecd8
3000a138:	67332667 	.word	0x67332667
3000a13c:	8eb44a87 	.word	0x8eb44a87
3000a140:	db0c2e0d 	.word	0xdb0c2e0d
3000a144:	47b5481d 	.word	0x47b5481d
3000a148:	367cd507 	.word	0x367cd507
3000a14c:	c1059ed8 	.word	0xc1059ed8
3000a150:	befa4fa4 	.word	0xbefa4fa4
3000a154:	137e2179 	.word	0x137e2179

3000a158 <CPU_ClkGet>:
3000a158:	b570      	push	{r4, r5, r6, lr}
3000a15a:	4d1e      	ldr	r5, [pc, #120]	; (3000a1d4 <CPU_ClkGet+0x7c>)
3000a15c:	4b1e      	ldr	r3, [pc, #120]	; (3000a1d8 <CPU_ClkGet+0x80>)
3000a15e:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
3000a162:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
3000a166:	f3c4 1481 	ubfx	r4, r4, #6, #2
3000a16a:	4798      	blx	r3
3000a16c:	b92c      	cbnz	r4, 3000a17a <CPU_ClkGet+0x22>
3000a16e:	2801      	cmp	r0, #1
3000a170:	d007      	beq.n	3000a182 <CPU_ClkGet+0x2a>
3000a172:	07b2      	lsls	r2, r6, #30
3000a174:	d407      	bmi.n	3000a186 <CPU_ClkGet+0x2e>
3000a176:	4819      	ldr	r0, [pc, #100]	; (3000a1dc <CPU_ClkGet+0x84>)
3000a178:	bd70      	pop	{r4, r5, r6, pc}
3000a17a:	2c01      	cmp	r4, #1
3000a17c:	d007      	beq.n	3000a18e <CPU_ClkGet+0x36>
3000a17e:	2000      	movs	r0, #0
3000a180:	bd70      	pop	{r4, r5, r6, pc}
3000a182:	4817      	ldr	r0, [pc, #92]	; (3000a1e0 <CPU_ClkGet+0x88>)
3000a184:	bd70      	pop	{r4, r5, r6, pc}
3000a186:	4b17      	ldr	r3, [pc, #92]	; (3000a1e4 <CPU_ClkGet+0x8c>)
3000a188:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000a18c:	4718      	bx	r3
3000a18e:	2801      	cmp	r0, #1
3000a190:	d011      	beq.n	3000a1b6 <CPU_ClkGet+0x5e>
3000a192:	05f3      	lsls	r3, r6, #23
3000a194:	d51c      	bpl.n	3000a1d0 <CPU_ClkGet+0x78>
3000a196:	4914      	ldr	r1, [pc, #80]	; (3000a1e8 <CPU_ClkGet+0x90>)
3000a198:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
3000a19c:	6c48      	ldr	r0, [r1, #68]	; 0x44
3000a19e:	4a13      	ldr	r2, [pc, #76]	; (3000a1ec <CPU_ClkGet+0x94>)
3000a1a0:	f003 0307 	and.w	r3, r3, #7
3000a1a4:	f3c0 1045 	ubfx	r0, r0, #5, #6
3000a1a8:	3301      	adds	r3, #1
3000a1aa:	3002      	adds	r0, #2
3000a1ac:	fb02 f000 	mul.w	r0, r2, r0
3000a1b0:	fbb0 f0f3 	udiv	r0, r0, r3
3000a1b4:	bd70      	pop	{r4, r5, r6, pc}
3000a1b6:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
3000a1ba:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
3000a1be:	f3c3 4007 	ubfx	r0, r3, #16, #8
3000a1c2:	d003      	beq.n	3000a1cc <CPU_ClkGet+0x74>
3000a1c4:	4b0a      	ldr	r3, [pc, #40]	; (3000a1f0 <CPU_ClkGet+0x98>)
3000a1c6:	fb03 f000 	mul.w	r0, r3, r0
3000a1ca:	bd70      	pop	{r4, r5, r6, pc}
3000a1cc:	4809      	ldr	r0, [pc, #36]	; (3000a1f4 <CPU_ClkGet+0x9c>)
3000a1ce:	bd70      	pop	{r4, r5, r6, pc}
3000a1d0:	4806      	ldr	r0, [pc, #24]	; (3000a1ec <CPU_ClkGet+0x94>)
3000a1d2:	bd70      	pop	{r4, r5, r6, pc}
3000a1d4:	42008000 	.word	0x42008000
3000a1d8:	0000c0f9 	.word	0x0000c0f9
3000a1dc:	003d0900 	.word	0x003d0900
3000a1e0:	01312d00 	.word	0x01312d00
3000a1e4:	000099f5 	.word	0x000099f5
3000a1e8:	42008800 	.word	0x42008800
3000a1ec:	02625a00 	.word	0x02625a00
3000a1f0:	000f4240 	.word	0x000f4240
3000a1f4:	02faf080 	.word	0x02faf080

3000a1f8 <RSIP_IV_Set>:
3000a1f8:	4b05      	ldr	r3, [pc, #20]	; (3000a210 <RSIP_IV_Set+0x18>)
3000a1fa:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
3000a1fe:	680b      	ldr	r3, [r1, #0]
3000a200:	ba1b      	rev	r3, r3
3000a202:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000a206:	684b      	ldr	r3, [r1, #4]
3000a208:	ba1b      	rev	r3, r3
3000a20a:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
3000a20e:	4770      	bx	lr
3000a210:	42008c00 	.word	0x42008c00

3000a214 <RSIP_OTF_Cmd>:
3000a214:	4a04      	ldr	r2, [pc, #16]	; (3000a228 <RSIP_OTF_Cmd+0x14>)
3000a216:	2801      	cmp	r0, #1
3000a218:	6953      	ldr	r3, [r2, #20]
3000a21a:	bf0c      	ite	eq
3000a21c:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
3000a220:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
3000a224:	6153      	str	r3, [r2, #20]
3000a226:	4770      	bx	lr
3000a228:	42008c00 	.word	0x42008c00

3000a22c <RSIP_OTF_Enable>:
3000a22c:	b570      	push	{r4, r5, r6, lr}
3000a22e:	4604      	mov	r4, r0
3000a230:	b1f3      	cbz	r3, 3000a270 <RSIP_OTF_Enable+0x44>
3000a232:	9b04      	ldr	r3, [sp, #16]
3000a234:	460d      	mov	r5, r1
3000a236:	4616      	mov	r6, r2
3000a238:	2b07      	cmp	r3, #7
3000a23a:	d827      	bhi.n	3000a28c <RSIP_OTF_Enable+0x60>
3000a23c:	9b05      	ldr	r3, [sp, #20]
3000a23e:	3b01      	subs	r3, #1
3000a240:	2b01      	cmp	r3, #1
3000a242:	d81d      	bhi.n	3000a280 <RSIP_OTF_Enable+0x54>
3000a244:	9a04      	ldr	r2, [sp, #16]
3000a246:	9b05      	ldr	r3, [sp, #20]
3000a248:	f002 0007 	and.w	r0, r2, #7
3000a24c:	005b      	lsls	r3, r3, #1
3000a24e:	4305      	orrs	r5, r0
3000a250:	4813      	ldr	r0, [pc, #76]	; (3000a2a0 <RSIP_OTF_Enable+0x74>)
3000a252:	f003 0306 	and.w	r3, r3, #6
3000a256:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000a25a:	ea43 0206 	orr.w	r2, r3, r6
3000a25e:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000a260:	f042 0201 	orr.w	r2, r2, #1
3000a264:	f023 0301 	bic.w	r3, r3, #1
3000a268:	6463      	str	r3, [r4, #68]	; 0x44
3000a26a:	6425      	str	r5, [r4, #64]	; 0x40
3000a26c:	6462      	str	r2, [r4, #68]	; 0x44
3000a26e:	bd70      	pop	{r4, r5, r6, pc}
3000a270:	480b      	ldr	r0, [pc, #44]	; (3000a2a0 <RSIP_OTF_Enable+0x74>)
3000a272:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000a276:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000a278:	f023 0301 	bic.w	r3, r3, #1
3000a27c:	6463      	str	r3, [r4, #68]	; 0x44
3000a27e:	bd70      	pop	{r4, r5, r6, pc}
3000a280:	f44f 71f3 	mov.w	r1, #486	; 0x1e6
3000a284:	4807      	ldr	r0, [pc, #28]	; (3000a2a4 <RSIP_OTF_Enable+0x78>)
3000a286:	f001 ffb7 	bl	3000c1f8 <__io_assert_failed_veneer>
3000a28a:	e7db      	b.n	3000a244 <RSIP_OTF_Enable+0x18>
3000a28c:	f240 11e5 	movw	r1, #485	; 0x1e5
3000a290:	4804      	ldr	r0, [pc, #16]	; (3000a2a4 <RSIP_OTF_Enable+0x78>)
3000a292:	f001 ffb1 	bl	3000c1f8 <__io_assert_failed_veneer>
3000a296:	9b05      	ldr	r3, [sp, #20]
3000a298:	3b01      	subs	r3, #1
3000a29a:	2b01      	cmp	r3, #1
3000a29c:	d9d2      	bls.n	3000a244 <RSIP_OTF_Enable+0x18>
3000a29e:	e7ef      	b.n	3000a280 <RSIP_OTF_Enable+0x54>
3000a2a0:	42008c00 	.word	0x42008c00
3000a2a4:	3000d5f4 	.word	0x3000d5f4

3000a2a8 <RSIP_MMU_Config>:
3000a2a8:	eb00 0040 	add.w	r0, r0, r0, lsl #1
3000a2ac:	311f      	adds	r1, #31
3000a2ae:	321f      	adds	r2, #31
3000a2b0:	331f      	adds	r3, #31
3000a2b2:	0080      	lsls	r0, r0, #2
3000a2b4:	0949      	lsrs	r1, r1, #5
3000a2b6:	0952      	lsrs	r2, r2, #5
3000a2b8:	095b      	lsrs	r3, r3, #5
3000a2ba:	0249      	lsls	r1, r1, #9
3000a2bc:	0252      	lsls	r2, r2, #9
3000a2be:	025b      	lsls	r3, r3, #9
3000a2c0:	b410      	push	{r4}
3000a2c2:	4c08      	ldr	r4, [pc, #32]	; (3000a2e4 <RSIP_MMU_Config+0x3c>)
3000a2c4:	4404      	add	r4, r0
3000a2c6:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
3000a2ca:	f020 0001 	bic.w	r0, r0, #1
3000a2ce:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
3000a2d2:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
3000a2d6:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
3000a2da:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
3000a2de:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a2e2:	4770      	bx	lr
3000a2e4:	42008c00 	.word	0x42008c00

3000a2e8 <RSIP_MMU_Cmd>:
3000a2e8:	eb00 0040 	add.w	r0, r0, r0, lsl #1
3000a2ec:	4b07      	ldr	r3, [pc, #28]	; (3000a30c <RSIP_MMU_Cmd+0x24>)
3000a2ee:	0080      	lsls	r0, r0, #2
3000a2f0:	4403      	add	r3, r0
3000a2f2:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
3000a2f6:	b121      	cbz	r1, 3000a302 <RSIP_MMU_Cmd+0x1a>
3000a2f8:	f042 0201 	orr.w	r2, r2, #1
3000a2fc:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a300:	4770      	bx	lr
3000a302:	f022 0201 	bic.w	r2, r2, #1
3000a306:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a30a:	4770      	bx	lr
3000a30c:	42008c00 	.word	0x42008c00

3000a310 <RSIP_MMU_Cache_Clean>:
3000a310:	4a02      	ldr	r2, [pc, #8]	; (3000a31c <RSIP_MMU_Cache_Clean+0xc>)
3000a312:	6993      	ldr	r3, [r2, #24]
3000a314:	f043 0304 	orr.w	r3, r3, #4
3000a318:	6193      	str	r3, [r2, #24]
3000a31a:	4770      	bx	lr
3000a31c:	42008c00 	.word	0x42008c00

3000a320 <NAND_RxData>:
3000a320:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
3000a324:	460c      	mov	r4, r1
3000a326:	4616      	mov	r6, r2
3000a328:	461f      	mov	r7, r3
3000a32a:	2800      	cmp	r0, #0
3000a32c:	f000 808a 	beq.w	3000a444 <NAND_RxData+0x124>
3000a330:	4605      	mov	r5, r0
3000a332:	f8df 911c 	ldr.w	r9, [pc, #284]	; 3000a450 <NAND_RxData+0x130>
3000a336:	2000      	movs	r0, #0
3000a338:	f8df 8118 	ldr.w	r8, [pc, #280]	; 3000a454 <NAND_RxData+0x134>
3000a33c:	47c0      	blx	r8
3000a33e:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
3000a342:	2000      	movs	r0, #0
3000a344:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
3000a348:	6819      	ldr	r1, [r3, #0]
3000a34a:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
3000a34e:	6019      	str	r1, [r3, #0]
3000a350:	6819      	ldr	r1, [r3, #0]
3000a352:	f441 7140 	orr.w	r1, r1, #768	; 0x300
3000a356:	6019      	str	r1, [r3, #0]
3000a358:	605a      	str	r2, [r3, #4]
3000a35a:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
3000a35e:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
3000a362:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
3000a366:	ea4f 420c 	mov.w	r2, ip, lsl #16
3000a36a:	f1bc 0f03 	cmp.w	ip, #3
3000a36e:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
3000a372:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a376:	ea42 0201 	orr.w	r2, r2, r1
3000a37a:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
3000a37e:	f04f 0201 	mov.w	r2, #1
3000a382:	611a      	str	r2, [r3, #16]
3000a384:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
3000a388:	bf08      	it	eq
3000a38a:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
3000a38e:	f3c4 2307 	ubfx	r3, r4, #8, #8
3000a392:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000a396:	b2e4      	uxtb	r4, r4
3000a398:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000a39c:	2301      	movs	r3, #1
3000a39e:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
3000a3a2:	f017 0403 	ands.w	r4, r7, #3
3000a3a6:	6083      	str	r3, [r0, #8]
3000a3a8:	d137      	bne.n	3000a41a <NAND_RxData+0xfa>
3000a3aa:	463b      	mov	r3, r7
3000a3ac:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
3000a3b0:	42b4      	cmp	r4, r6
3000a3b2:	d216      	bcs.n	3000a3e2 <NAND_RxData+0xc2>
3000a3b4:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
3000a3b8:	f012 0f3c 	tst.w	r2, #60	; 0x3c
3000a3bc:	f3c2 0583 	ubfx	r5, r2, #2, #4
3000a3c0:	d00c      	beq.n	3000a3dc <NAND_RxData+0xbc>
3000a3c2:	00ad      	lsls	r5, r5, #2
3000a3c4:	1f19      	subs	r1, r3, #4
3000a3c6:	f1a5 0c04 	sub.w	ip, r5, #4
3000a3ca:	449c      	add	ip, r3
3000a3cc:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
3000a3d0:	f841 0f04 	str.w	r0, [r1, #4]!
3000a3d4:	458c      	cmp	ip, r1
3000a3d6:	d1f9      	bne.n	3000a3cc <NAND_RxData+0xac>
3000a3d8:	442b      	add	r3, r5
3000a3da:	442c      	add	r4, r5
3000a3dc:	1b32      	subs	r2, r6, r4
3000a3de:	2a03      	cmp	r2, #3
3000a3e0:	d8e6      	bhi.n	3000a3b0 <NAND_RxData+0x90>
3000a3e2:	1e63      	subs	r3, r4, #1
3000a3e4:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a3e8:	441f      	add	r7, r3
3000a3ea:	e002      	b.n	3000a3f2 <NAND_RxData+0xd2>
3000a3ec:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000a3ee:	071b      	lsls	r3, r3, #28
3000a3f0:	d40d      	bmi.n	3000a40e <NAND_RxData+0xee>
3000a3f2:	42b4      	cmp	r4, r6
3000a3f4:	d3fa      	bcc.n	3000a3ec <NAND_RxData+0xcc>
3000a3f6:	2004      	movs	r0, #4
3000a3f8:	47c0      	blx	r8
3000a3fa:	2000      	movs	r0, #0
3000a3fc:	47c0      	blx	r8
3000a3fe:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a402:	6813      	ldr	r3, [r2, #0]
3000a404:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000a408:	6013      	str	r3, [r2, #0]
3000a40a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
3000a40e:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
3000a412:	3401      	adds	r4, #1
3000a414:	f807 3f01 	strb.w	r3, [r7, #1]!
3000a418:	e7eb      	b.n	3000a3f2 <NAND_RxData+0xd2>
3000a41a:	f1c4 0404 	rsb	r4, r4, #4
3000a41e:	2200      	movs	r2, #0
3000a420:	4639      	mov	r1, r7
3000a422:	e00a      	b.n	3000a43a <NAND_RxData+0x11a>
3000a424:	6a85      	ldr	r5, [r0, #40]	; 0x28
3000a426:	072d      	lsls	r5, r5, #28
3000a428:	d505      	bpl.n	3000a436 <NAND_RxData+0x116>
3000a42a:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
3000a42e:	3201      	adds	r2, #1
3000a430:	700b      	strb	r3, [r1, #0]
3000a432:	18b9      	adds	r1, r7, r2
3000a434:	460b      	mov	r3, r1
3000a436:	4294      	cmp	r4, r2
3000a438:	d0b8      	beq.n	3000a3ac <NAND_RxData+0x8c>
3000a43a:	42b2      	cmp	r2, r6
3000a43c:	460b      	mov	r3, r1
3000a43e:	d3f1      	bcc.n	3000a424 <NAND_RxData+0x104>
3000a440:	4614      	mov	r4, r2
3000a442:	e7ce      	b.n	3000a3e2 <NAND_RxData+0xc2>
3000a444:	f8df 9008 	ldr.w	r9, [pc, #8]	; 3000a450 <NAND_RxData+0x130>
3000a448:	f899 5008 	ldrb.w	r5, [r9, #8]
3000a44c:	e773      	b.n	3000a336 <NAND_RxData+0x16>
3000a44e:	bf00      	nop
3000a450:	2001c01c 	.word	0x2001c01c
3000a454:	0000b9c1 	.word	0x0000b9c1

3000a458 <NAND_Page_Read>:
3000a458:	b530      	push	{r4, r5, lr}
3000a45a:	b085      	sub	sp, #20
3000a45c:	4c07      	ldr	r4, [pc, #28]	; (3000a47c <NAND_Page_Read+0x24>)
3000a45e:	4d08      	ldr	r5, [pc, #32]	; (3000a480 <NAND_Page_Read+0x28>)
3000a460:	9301      	str	r3, [sp, #4]
3000a462:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000a466:	47a0      	blx	r4
3000a468:	4604      	mov	r4, r0
3000a46a:	9903      	ldr	r1, [sp, #12]
3000a46c:	7a28      	ldrb	r0, [r5, #8]
3000a46e:	4d05      	ldr	r5, [pc, #20]	; (3000a484 <NAND_Page_Read+0x2c>)
3000a470:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000a474:	47a8      	blx	r5
3000a476:	4620      	mov	r0, r4
3000a478:	b005      	add	sp, #20
3000a47a:	bd30      	pop	{r4, r5, pc}
3000a47c:	0000bd85 	.word	0x0000bd85
3000a480:	2001c01c 	.word	0x2001c01c
3000a484:	3000a321 	.word	0x3000a321

3000a488 <irq_register>:
3000a488:	b538      	push	{r3, r4, r5, lr}
3000a48a:	f040 0001 	orr.w	r0, r0, #1
3000a48e:	461c      	mov	r4, r3
3000a490:	4b14      	ldr	r3, [pc, #80]	; (3000a4e4 <irq_register+0x5c>)
3000a492:	460d      	mov	r5, r1
3000a494:	2c07      	cmp	r4, #7
3000a496:	f843 0021 	str.w	r0, [r3, r1, lsl #2]
3000a49a:	4b13      	ldr	r3, [pc, #76]	; (3000a4e8 <irq_register+0x60>)
3000a49c:	f843 2021 	str.w	r2, [r3, r1, lsl #2]
3000a4a0:	d908      	bls.n	3000a4b4 <irq_register+0x2c>
3000a4a2:	4b12      	ldr	r3, [pc, #72]	; (3000a4ec <irq_register+0x64>)
3000a4a4:	681b      	ldr	r3, [r3, #0]
3000a4a6:	039b      	lsls	r3, r3, #14
3000a4a8:	d416      	bmi.n	3000a4d8 <irq_register+0x50>
3000a4aa:	f44f 7144 	mov.w	r1, #784	; 0x310
3000a4ae:	4810      	ldr	r0, [pc, #64]	; (3000a4f0 <irq_register+0x68>)
3000a4b0:	f001 fea2 	bl	3000c1f8 <__io_assert_failed_veneer>
3000a4b4:	2c07      	cmp	r4, #7
3000a4b6:	bf28      	it	cs
3000a4b8:	2407      	movcs	r4, #7
3000a4ba:	2d00      	cmp	r5, #0
3000a4bc:	ea4f 1444 	mov.w	r4, r4, lsl #5
3000a4c0:	b2e4      	uxtb	r4, r4
3000a4c2:	db03      	blt.n	3000a4cc <irq_register+0x44>
3000a4c4:	4b0b      	ldr	r3, [pc, #44]	; (3000a4f4 <irq_register+0x6c>)
3000a4c6:	2001      	movs	r0, #1
3000a4c8:	555c      	strb	r4, [r3, r5]
3000a4ca:	bd38      	pop	{r3, r4, r5, pc}
3000a4cc:	f005 050f 	and.w	r5, r5, #15
3000a4d0:	4b09      	ldr	r3, [pc, #36]	; (3000a4f8 <irq_register+0x70>)
3000a4d2:	2001      	movs	r0, #1
3000a4d4:	555c      	strb	r4, [r3, r5]
3000a4d6:	bd38      	pop	{r3, r4, r5, pc}
3000a4d8:	2307      	movs	r3, #7
3000a4da:	4622      	mov	r2, r4
3000a4dc:	4807      	ldr	r0, [pc, #28]	; (3000a4fc <irq_register+0x74>)
3000a4de:	f001 fe6b 	bl	3000c1b8 <__DiagPrintf_veneer>
3000a4e2:	e7e2      	b.n	3000a4aa <irq_register+0x22>
3000a4e4:	30000184 	.word	0x30000184
3000a4e8:	300002c4 	.word	0x300002c4
3000a4ec:	2001c00c 	.word	0x2001c00c
3000a4f0:	3000d5e0 	.word	0x3000d5e0
3000a4f4:	e000e400 	.word	0xe000e400
3000a4f8:	e000ed14 	.word	0xe000ed14
3000a4fc:	3000d5bc 	.word	0x3000d5bc

3000a500 <SYSCFG_BootFromNor>:
3000a500:	4b0e      	ldr	r3, [pc, #56]	; (3000a53c <SYSCFG_BootFromNor+0x3c>)
3000a502:	b510      	push	{r4, lr}
3000a504:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
3000a508:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
3000a50c:	0392      	lsls	r2, r2, #14
3000a50e:	d50a      	bpl.n	3000a526 <SYSCFG_BootFromNor+0x26>
3000a510:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
3000a514:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
3000a518:	d105      	bne.n	3000a526 <SYSCFG_BootFromNor+0x26>
3000a51a:	4b09      	ldr	r3, [pc, #36]	; (3000a540 <SYSCFG_BootFromNor+0x40>)
3000a51c:	4798      	blx	r3
3000a51e:	3800      	subs	r0, #0
3000a520:	bf18      	it	ne
3000a522:	2001      	movne	r0, #1
3000a524:	bd10      	pop	{r4, pc}
3000a526:	4c07      	ldr	r4, [pc, #28]	; (3000a544 <SYSCFG_BootFromNor+0x44>)
3000a528:	47a0      	blx	r4
3000a52a:	2802      	cmp	r0, #2
3000a52c:	d004      	beq.n	3000a538 <SYSCFG_BootFromNor+0x38>
3000a52e:	47a0      	blx	r4
3000a530:	2801      	cmp	r0, #1
3000a532:	d1f2      	bne.n	3000a51a <SYSCFG_BootFromNor+0x1a>
3000a534:	2000      	movs	r0, #0
3000a536:	bd10      	pop	{r4, pc}
3000a538:	2001      	movs	r0, #1
3000a53a:	bd10      	pop	{r4, pc}
3000a53c:	42008000 	.word	0x42008000
3000a540:	0000c0bd 	.word	0x0000c0bd
3000a544:	0000c1a9 	.word	0x0000c1a9

3000a548 <SYSCFG_RLVersion>:
3000a548:	4b07      	ldr	r3, [pc, #28]	; (3000a568 <SYSCFG_RLVersion+0x20>)
3000a54a:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000a54e:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000a552:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
3000a556:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
3000a55a:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000a55e:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000a562:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000a566:	4770      	bx	lr
3000a568:	42008000 	.word	0x42008000

3000a56c <ddr_init_index>:
3000a56c:	b570      	push	{r4, r5, r6, lr}
3000a56e:	4b0f      	ldr	r3, [pc, #60]	; (3000a5ac <ddr_init_index+0x40>)
3000a570:	4798      	blx	r3
3000a572:	4604      	mov	r4, r0
3000a574:	4b0e      	ldr	r3, [pc, #56]	; (3000a5b0 <ddr_init_index+0x44>)
3000a576:	4798      	blx	r3
3000a578:	2c03      	cmp	r4, #3
3000a57a:	d015      	beq.n	3000a5a8 <ddr_init_index+0x3c>
3000a57c:	2c01      	cmp	r4, #1
3000a57e:	d00d      	beq.n	3000a59c <ddr_init_index+0x30>
3000a580:	4d0c      	ldr	r5, [pc, #48]	; (3000a5b4 <ddr_init_index+0x48>)
3000a582:	4c0d      	ldr	r4, [pc, #52]	; (3000a5b8 <ddr_init_index+0x4c>)
3000a584:	4e0d      	ldr	r6, [pc, #52]	; (3000a5bc <ddr_init_index+0x50>)
3000a586:	e002      	b.n	3000a58e <ddr_init_index+0x22>
3000a588:	f242 7010 	movw	r0, #10000	; 0x2710
3000a58c:	47a0      	blx	r4
3000a58e:	682b      	ldr	r3, [r5, #0]
3000a590:	039b      	lsls	r3, r3, #14
3000a592:	d5f9      	bpl.n	3000a588 <ddr_init_index+0x1c>
3000a594:	4630      	mov	r0, r6
3000a596:	f001 fe0f 	bl	3000c1b8 <__DiagPrintf_veneer>
3000a59a:	e7f5      	b.n	3000a588 <ddr_init_index+0x1c>
3000a59c:	2802      	cmp	r0, #2
3000a59e:	d003      	beq.n	3000a5a8 <ddr_init_index+0x3c>
3000a5a0:	2803      	cmp	r0, #3
3000a5a2:	d1ed      	bne.n	3000a580 <ddr_init_index+0x14>
3000a5a4:	2000      	movs	r0, #0
3000a5a6:	bd70      	pop	{r4, r5, r6, pc}
3000a5a8:	4620      	mov	r0, r4
3000a5aa:	bd70      	pop	{r4, r5, r6, pc}
3000a5ac:	30008045 	.word	0x30008045
3000a5b0:	30007f65 	.word	0x30007f65
3000a5b4:	2001c00c 	.word	0x2001c00c
3000a5b8:	00009be5 	.word	0x00009be5
3000a5bc:	3000d63c 	.word	0x3000d63c

3000a5c0 <rxi316_perf_tune>:
3000a5c0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a5c4:	f8d0 e000 	ldr.w	lr, [r0]
3000a5c8:	b0bf      	sub	sp, #252	; 0xfc
3000a5ca:	4606      	mov	r6, r0
3000a5cc:	f89e 5006 	ldrb.w	r5, [lr, #6]
3000a5d0:	2d00      	cmp	r5, #0
3000a5d2:	f000 833e 	beq.w	3000ac52 <rxi316_perf_tune+0x692>
3000a5d6:	2d01      	cmp	r5, #1
3000a5d8:	d003      	beq.n	3000a5e2 <rxi316_perf_tune+0x22>
3000a5da:	2d03      	cmp	r5, #3
3000a5dc:	bf14      	ite	ne
3000a5de:	2502      	movne	r5, #2
3000a5e0:	2500      	moveq	r5, #0
3000a5e2:	68f3      	ldr	r3, [r6, #12]
3000a5e4:	2420      	movs	r4, #32
3000a5e6:	2100      	movs	r1, #0
3000a5e8:	685b      	ldr	r3, [r3, #4]
3000a5ea:	4627      	mov	r7, r4
3000a5ec:	46a0      	mov	r8, r4
3000a5ee:	46a1      	mov	r9, r4
3000a5f0:	785a      	ldrb	r2, [r3, #1]
3000a5f2:	46a2      	mov	sl, r4
3000a5f4:	7818      	ldrb	r0, [r3, #0]
3000a5f6:	46a3      	mov	fp, r4
3000a5f8:	921f      	str	r2, [sp, #124]	; 0x7c
3000a5fa:	468c      	mov	ip, r1
3000a5fc:	789a      	ldrb	r2, [r3, #2]
3000a5fe:	9220      	str	r2, [sp, #128]	; 0x80
3000a600:	78da      	ldrb	r2, [r3, #3]
3000a602:	9221      	str	r2, [sp, #132]	; 0x84
3000a604:	791a      	ldrb	r2, [r3, #4]
3000a606:	9222      	str	r2, [sp, #136]	; 0x88
3000a608:	795a      	ldrb	r2, [r3, #5]
3000a60a:	9223      	str	r2, [sp, #140]	; 0x8c
3000a60c:	799a      	ldrb	r2, [r3, #6]
3000a60e:	9224      	str	r2, [sp, #144]	; 0x90
3000a610:	79da      	ldrb	r2, [r3, #7]
3000a612:	9225      	str	r2, [sp, #148]	; 0x94
3000a614:	7a1a      	ldrb	r2, [r3, #8]
3000a616:	9226      	str	r2, [sp, #152]	; 0x98
3000a618:	7a5a      	ldrb	r2, [r3, #9]
3000a61a:	9227      	str	r2, [sp, #156]	; 0x9c
3000a61c:	7a9a      	ldrb	r2, [r3, #10]
3000a61e:	9228      	str	r2, [sp, #160]	; 0xa0
3000a620:	7ada      	ldrb	r2, [r3, #11]
3000a622:	9229      	str	r2, [sp, #164]	; 0xa4
3000a624:	7b1a      	ldrb	r2, [r3, #12]
3000a626:	922a      	str	r2, [sp, #168]	; 0xa8
3000a628:	7b5a      	ldrb	r2, [r3, #13]
3000a62a:	922b      	str	r2, [sp, #172]	; 0xac
3000a62c:	7b9a      	ldrb	r2, [r3, #14]
3000a62e:	922c      	str	r2, [sp, #176]	; 0xb0
3000a630:	7bda      	ldrb	r2, [r3, #15]
3000a632:	922d      	str	r2, [sp, #180]	; 0xb4
3000a634:	7c1a      	ldrb	r2, [r3, #16]
3000a636:	941a      	str	r4, [sp, #104]	; 0x68
3000a638:	922e      	str	r2, [sp, #184]	; 0xb8
3000a63a:	7c5a      	ldrb	r2, [r3, #17]
3000a63c:	922f      	str	r2, [sp, #188]	; 0xbc
3000a63e:	7c9a      	ldrb	r2, [r3, #18]
3000a640:	9230      	str	r2, [sp, #192]	; 0xc0
3000a642:	7cda      	ldrb	r2, [r3, #19]
3000a644:	9231      	str	r2, [sp, #196]	; 0xc4
3000a646:	7d1a      	ldrb	r2, [r3, #20]
3000a648:	9232      	str	r2, [sp, #200]	; 0xc8
3000a64a:	7d5a      	ldrb	r2, [r3, #21]
3000a64c:	9233      	str	r2, [sp, #204]	; 0xcc
3000a64e:	7d9a      	ldrb	r2, [r3, #22]
3000a650:	9234      	str	r2, [sp, #208]	; 0xd0
3000a652:	7dda      	ldrb	r2, [r3, #23]
3000a654:	9235      	str	r2, [sp, #212]	; 0xd4
3000a656:	7e1a      	ldrb	r2, [r3, #24]
3000a658:	9236      	str	r2, [sp, #216]	; 0xd8
3000a65a:	7e5a      	ldrb	r2, [r3, #25]
3000a65c:	9237      	str	r2, [sp, #220]	; 0xdc
3000a65e:	7e9a      	ldrb	r2, [r3, #26]
3000a660:	9238      	str	r2, [sp, #224]	; 0xe0
3000a662:	7eda      	ldrb	r2, [r3, #27]
3000a664:	9239      	str	r2, [sp, #228]	; 0xe4
3000a666:	7f1a      	ldrb	r2, [r3, #28]
3000a668:	923a      	str	r2, [sp, #232]	; 0xe8
3000a66a:	7f5a      	ldrb	r2, [r3, #29]
3000a66c:	923b      	str	r2, [sp, #236]	; 0xec
3000a66e:	7f9a      	ldrb	r2, [r3, #30]
3000a670:	7fdb      	ldrb	r3, [r3, #31]
3000a672:	923c      	str	r2, [sp, #240]	; 0xf0
3000a674:	460a      	mov	r2, r1
3000a676:	933d      	str	r3, [sp, #244]	; 0xf4
3000a678:	4623      	mov	r3, r4
3000a67a:	9406      	str	r4, [sp, #24]
3000a67c:	9419      	str	r4, [sp, #100]	; 0x64
3000a67e:	9418      	str	r4, [sp, #96]	; 0x60
3000a680:	9407      	str	r4, [sp, #28]
3000a682:	951b      	str	r5, [sp, #108]	; 0x6c
3000a684:	e9cd 4402 	strd	r4, r4, [sp, #8]
3000a688:	e9cd 4400 	strd	r4, r4, [sp]
3000a68c:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000a690:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
3000a694:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
3000a698:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
3000a69c:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
3000a6a0:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000a6a4:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000a6a8:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
3000a6ac:	e9cd 4408 	strd	r4, r4, [sp, #32]
3000a6b0:	b950      	cbnz	r0, 3000a6c8 <rxi316_perf_tune+0x108>
3000a6b2:	f10c 0c01 	add.w	ip, ip, #1
3000a6b6:	4613      	mov	r3, r2
3000a6b8:	3201      	adds	r2, #1
3000a6ba:	2a20      	cmp	r2, #32
3000a6bc:	d06f      	beq.n	3000a79e <rxi316_perf_tune+0x1de>
3000a6be:	a81e      	add	r0, sp, #120	; 0x78
3000a6c0:	f850 0022 	ldr.w	r0, [r0, r2, lsl #2]
3000a6c4:	2800      	cmp	r0, #0
3000a6c6:	d0f4      	beq.n	3000a6b2 <rxi316_perf_tune+0xf2>
3000a6c8:	2801      	cmp	r0, #1
3000a6ca:	d062      	beq.n	3000a792 <rxi316_perf_tune+0x1d2>
3000a6cc:	2802      	cmp	r0, #2
3000a6ce:	f000 8083 	beq.w	3000a7d8 <rxi316_perf_tune+0x218>
3000a6d2:	2803      	cmp	r0, #3
3000a6d4:	f000 8084 	beq.w	3000a7e0 <rxi316_perf_tune+0x220>
3000a6d8:	2804      	cmp	r0, #4
3000a6da:	f000 8085 	beq.w	3000a7e8 <rxi316_perf_tune+0x228>
3000a6de:	2805      	cmp	r0, #5
3000a6e0:	f000 8086 	beq.w	3000a7f0 <rxi316_perf_tune+0x230>
3000a6e4:	2806      	cmp	r0, #6
3000a6e6:	f000 8087 	beq.w	3000a7f8 <rxi316_perf_tune+0x238>
3000a6ea:	2807      	cmp	r0, #7
3000a6ec:	f000 82b3 	beq.w	3000ac56 <rxi316_perf_tune+0x696>
3000a6f0:	2808      	cmp	r0, #8
3000a6f2:	f000 8375 	beq.w	3000ade0 <rxi316_perf_tune+0x820>
3000a6f6:	2809      	cmp	r0, #9
3000a6f8:	f000 8376 	beq.w	3000ade8 <rxi316_perf_tune+0x828>
3000a6fc:	280a      	cmp	r0, #10
3000a6fe:	f000 837c 	beq.w	3000adfa <rxi316_perf_tune+0x83a>
3000a702:	280b      	cmp	r0, #11
3000a704:	f000 8376 	beq.w	3000adf4 <rxi316_perf_tune+0x834>
3000a708:	280c      	cmp	r0, #12
3000a70a:	f000 8383 	beq.w	3000ae14 <rxi316_perf_tune+0x854>
3000a70e:	280d      	cmp	r0, #13
3000a710:	f000 8386 	beq.w	3000ae20 <rxi316_perf_tune+0x860>
3000a714:	280e      	cmp	r0, #14
3000a716:	f000 8380 	beq.w	3000ae1a <rxi316_perf_tune+0x85a>
3000a71a:	280f      	cmp	r0, #15
3000a71c:	f000 8383 	beq.w	3000ae26 <rxi316_perf_tune+0x866>
3000a720:	2810      	cmp	r0, #16
3000a722:	f000 8383 	beq.w	3000ae2c <rxi316_perf_tune+0x86c>
3000a726:	2811      	cmp	r0, #17
3000a728:	f000 8382 	beq.w	3000ae30 <rxi316_perf_tune+0x870>
3000a72c:	2812      	cmp	r0, #18
3000a72e:	f000 8381 	beq.w	3000ae34 <rxi316_perf_tune+0x874>
3000a732:	2813      	cmp	r0, #19
3000a734:	f000 8382 	beq.w	3000ae3c <rxi316_perf_tune+0x87c>
3000a738:	2814      	cmp	r0, #20
3000a73a:	f000 837d 	beq.w	3000ae38 <rxi316_perf_tune+0x878>
3000a73e:	2815      	cmp	r0, #21
3000a740:	f000 8384 	beq.w	3000ae4c <rxi316_perf_tune+0x88c>
3000a744:	2816      	cmp	r0, #22
3000a746:	f000 837f 	beq.w	3000ae48 <rxi316_perf_tune+0x888>
3000a74a:	2817      	cmp	r0, #23
3000a74c:	f000 838e 	beq.w	3000ae6c <rxi316_perf_tune+0x8ac>
3000a750:	2818      	cmp	r0, #24
3000a752:	f000 8389 	beq.w	3000ae68 <rxi316_perf_tune+0x8a8>
3000a756:	2819      	cmp	r0, #25
3000a758:	f000 8384 	beq.w	3000ae64 <rxi316_perf_tune+0x8a4>
3000a75c:	281a      	cmp	r0, #26
3000a75e:	f000 837f 	beq.w	3000ae60 <rxi316_perf_tune+0x8a0>
3000a762:	281b      	cmp	r0, #27
3000a764:	f000 837a 	beq.w	3000ae5c <rxi316_perf_tune+0x89c>
3000a768:	281c      	cmp	r0, #28
3000a76a:	f000 8375 	beq.w	3000ae58 <rxi316_perf_tune+0x898>
3000a76e:	281d      	cmp	r0, #29
3000a770:	f000 8370 	beq.w	3000ae54 <rxi316_perf_tune+0x894>
3000a774:	281e      	cmp	r0, #30
3000a776:	f000 836b 	beq.w	3000ae50 <rxi316_perf_tune+0x890>
3000a77a:	281f      	cmp	r0, #31
3000a77c:	f000 8362 	beq.w	3000ae44 <rxi316_perf_tune+0x884>
3000a780:	2820      	cmp	r0, #32
3000a782:	f000 835d 	beq.w	3000ae40 <rxi316_perf_tune+0x880>
3000a786:	9d06      	ldr	r5, [sp, #24]
3000a788:	2821      	cmp	r0, #33	; 0x21
3000a78a:	bf08      	it	eq
3000a78c:	4615      	moveq	r5, r2
3000a78e:	9506      	str	r5, [sp, #24]
3000a790:	e792      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000a792:	4693      	mov	fp, r2
3000a794:	3201      	adds	r2, #1
3000a796:	f10c 0c01 	add.w	ip, ip, #1
3000a79a:	2a20      	cmp	r2, #32
3000a79c:	d18f      	bne.n	3000a6be <rxi316_perf_tune+0xfe>
3000a79e:	f89e 2002 	ldrb.w	r2, [lr, #2]
3000a7a2:	9d1b      	ldr	r5, [sp, #108]	; 0x6c
3000a7a4:	f8de 0008 	ldr.w	r0, [lr, #8]
3000a7a8:	b372      	cbz	r2, 3000a808 <rxi316_perf_tune+0x248>
3000a7aa:	2a01      	cmp	r2, #1
3000a7ac:	f000 8320 	beq.w	3000adf0 <rxi316_perf_tune+0x830>
3000a7b0:	2a03      	cmp	r2, #3
3000a7b2:	f1a0 0002 	sub.w	r0, r0, #2
3000a7b6:	bf0c      	ite	eq
3000a7b8:	2204      	moveq	r2, #4
3000a7ba:	2203      	movne	r2, #3
3000a7bc:	4560      	cmp	r0, ip
3000a7be:	d127      	bne.n	3000a810 <rxi316_perf_tune+0x250>
3000a7c0:	4291      	cmp	r1, r2
3000a7c2:	d02a      	beq.n	3000a81a <rxi316_perf_tune+0x25a>
3000a7c4:	4a0e      	ldr	r2, [pc, #56]	; (3000a800 <rxi316_perf_tune+0x240>)
3000a7c6:	6812      	ldr	r2, [r2, #0]
3000a7c8:	0392      	lsls	r2, r2, #14
3000a7ca:	d526      	bpl.n	3000a81a <rxi316_perf_tune+0x25a>
3000a7cc:	480d      	ldr	r0, [pc, #52]	; (3000a804 <rxi316_perf_tune+0x244>)
3000a7ce:	931b      	str	r3, [sp, #108]	; 0x6c
3000a7d0:	f001 fcf2 	bl	3000c1b8 <__DiagPrintf_veneer>
3000a7d4:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a7d6:	e020      	b.n	3000a81a <rxi316_perf_tune+0x25a>
3000a7d8:	f10c 0c01 	add.w	ip, ip, #1
3000a7dc:	4692      	mov	sl, r2
3000a7de:	e76b      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000a7e0:	f10c 0c01 	add.w	ip, ip, #1
3000a7e4:	4691      	mov	r9, r2
3000a7e6:	e767      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000a7e8:	f10c 0c01 	add.w	ip, ip, #1
3000a7ec:	4690      	mov	r8, r2
3000a7ee:	e763      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000a7f0:	f10c 0c01 	add.w	ip, ip, #1
3000a7f4:	4617      	mov	r7, r2
3000a7f6:	e75f      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000a7f8:	f10c 0c01 	add.w	ip, ip, #1
3000a7fc:	9200      	str	r2, [sp, #0]
3000a7fe:	e75b      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000a800:	2001c00c 	.word	0x2001c00c
3000a804:	3000d69c 	.word	0x3000d69c
3000a808:	2201      	movs	r2, #1
3000a80a:	3802      	subs	r0, #2
3000a80c:	4560      	cmp	r0, ip
3000a80e:	d0d7      	beq.n	3000a7c0 <rxi316_perf_tune+0x200>
3000a810:	48c4      	ldr	r0, [pc, #784]	; (3000ab24 <rxi316_perf_tune+0x564>)
3000a812:	6800      	ldr	r0, [r0, #0]
3000a814:	0380      	lsls	r0, r0, #14
3000a816:	f100 82f3 	bmi.w	3000ae00 <rxi316_perf_tune+0x840>
3000a81a:	9a07      	ldr	r2, [sp, #28]
3000a81c:	2a20      	cmp	r2, #32
3000a81e:	f000 82dc 	beq.w	3000adda <rxi316_perf_tune+0x81a>
3000a822:	1b52      	subs	r2, r2, r5
3000a824:	3a05      	subs	r2, #5
3000a826:	f002 020f 	and.w	r2, r2, #15
3000a82a:	9207      	str	r2, [sp, #28]
3000a82c:	9a08      	ldr	r2, [sp, #32]
3000a82e:	2a20      	cmp	r2, #32
3000a830:	f000 82d1 	beq.w	3000add6 <rxi316_perf_tune+0x816>
3000a834:	1b52      	subs	r2, r2, r5
3000a836:	3a06      	subs	r2, #6
3000a838:	0112      	lsls	r2, r2, #4
3000a83a:	b2d2      	uxtb	r2, r2
3000a83c:	9909      	ldr	r1, [sp, #36]	; 0x24
3000a83e:	2920      	cmp	r1, #32
3000a840:	f000 82c5 	beq.w	3000adce <rxi316_perf_tune+0x80e>
3000a844:	1b49      	subs	r1, r1, r5
3000a846:	3907      	subs	r1, #7
3000a848:	0209      	lsls	r1, r1, #8
3000a84a:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a84e:	9108      	str	r1, [sp, #32]
3000a850:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a852:	2920      	cmp	r1, #32
3000a854:	f000 821a 	beq.w	3000ac8c <rxi316_perf_tune+0x6cc>
3000a858:	1b49      	subs	r1, r1, r5
3000a85a:	3908      	subs	r1, #8
3000a85c:	0309      	lsls	r1, r1, #12
3000a85e:	b289      	uxth	r1, r1
3000a860:	9109      	str	r1, [sp, #36]	; 0x24
3000a862:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a864:	2920      	cmp	r1, #32
3000a866:	f000 8218 	beq.w	3000ac9a <rxi316_perf_tune+0x6da>
3000a86a:	1b49      	subs	r1, r1, r5
3000a86c:	3909      	subs	r1, #9
3000a86e:	0409      	lsls	r1, r1, #16
3000a870:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a874:	910a      	str	r1, [sp, #40]	; 0x28
3000a876:	990c      	ldr	r1, [sp, #48]	; 0x30
3000a878:	2920      	cmp	r1, #32
3000a87a:	f000 8215 	beq.w	3000aca8 <rxi316_perf_tune+0x6e8>
3000a87e:	1b49      	subs	r1, r1, r5
3000a880:	390a      	subs	r1, #10
3000a882:	0509      	lsls	r1, r1, #20
3000a884:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a888:	910b      	str	r1, [sp, #44]	; 0x2c
3000a88a:	990d      	ldr	r1, [sp, #52]	; 0x34
3000a88c:	2920      	cmp	r1, #32
3000a88e:	f000 8212 	beq.w	3000acb6 <rxi316_perf_tune+0x6f6>
3000a892:	1b49      	subs	r1, r1, r5
3000a894:	390b      	subs	r1, #11
3000a896:	0609      	lsls	r1, r1, #24
3000a898:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a89c:	910c      	str	r1, [sp, #48]	; 0x30
3000a89e:	990e      	ldr	r1, [sp, #56]	; 0x38
3000a8a0:	2920      	cmp	r1, #32
3000a8a2:	f000 820f 	beq.w	3000acc4 <rxi316_perf_tune+0x704>
3000a8a6:	1b49      	subs	r1, r1, r5
3000a8a8:	390c      	subs	r1, #12
3000a8aa:	0709      	lsls	r1, r1, #28
3000a8ac:	910d      	str	r1, [sp, #52]	; 0x34
3000a8ae:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000a8b0:	2920      	cmp	r1, #32
3000a8b2:	f000 820e 	beq.w	3000acd2 <rxi316_perf_tune+0x712>
3000a8b6:	1b49      	subs	r1, r1, r5
3000a8b8:	390d      	subs	r1, #13
3000a8ba:	f001 010f 	and.w	r1, r1, #15
3000a8be:	910e      	str	r1, [sp, #56]	; 0x38
3000a8c0:	9910      	ldr	r1, [sp, #64]	; 0x40
3000a8c2:	2920      	cmp	r1, #32
3000a8c4:	f000 820b 	beq.w	3000acde <rxi316_perf_tune+0x71e>
3000a8c8:	1b49      	subs	r1, r1, r5
3000a8ca:	390e      	subs	r1, #14
3000a8cc:	0109      	lsls	r1, r1, #4
3000a8ce:	b2c9      	uxtb	r1, r1
3000a8d0:	910f      	str	r1, [sp, #60]	; 0x3c
3000a8d2:	9911      	ldr	r1, [sp, #68]	; 0x44
3000a8d4:	2920      	cmp	r1, #32
3000a8d6:	f000 8208 	beq.w	3000acea <rxi316_perf_tune+0x72a>
3000a8da:	1b49      	subs	r1, r1, r5
3000a8dc:	390f      	subs	r1, #15
3000a8de:	0209      	lsls	r1, r1, #8
3000a8e0:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a8e4:	9110      	str	r1, [sp, #64]	; 0x40
3000a8e6:	9912      	ldr	r1, [sp, #72]	; 0x48
3000a8e8:	2920      	cmp	r1, #32
3000a8ea:	f000 8205 	beq.w	3000acf8 <rxi316_perf_tune+0x738>
3000a8ee:	1b49      	subs	r1, r1, r5
3000a8f0:	0309      	lsls	r1, r1, #12
3000a8f2:	f401 4170 	and.w	r1, r1, #61440	; 0xf000
3000a8f6:	9111      	str	r1, [sp, #68]	; 0x44
3000a8f8:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000a8fa:	2920      	cmp	r1, #32
3000a8fc:	f000 8203 	beq.w	3000ad06 <rxi316_perf_tune+0x746>
3000a900:	1b49      	subs	r1, r1, r5
3000a902:	3911      	subs	r1, #17
3000a904:	0409      	lsls	r1, r1, #16
3000a906:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a90a:	9112      	str	r1, [sp, #72]	; 0x48
3000a90c:	9914      	ldr	r1, [sp, #80]	; 0x50
3000a90e:	2920      	cmp	r1, #32
3000a910:	f000 8200 	beq.w	3000ad14 <rxi316_perf_tune+0x754>
3000a914:	1b49      	subs	r1, r1, r5
3000a916:	3912      	subs	r1, #18
3000a918:	0509      	lsls	r1, r1, #20
3000a91a:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a91e:	9113      	str	r1, [sp, #76]	; 0x4c
3000a920:	9915      	ldr	r1, [sp, #84]	; 0x54
3000a922:	2920      	cmp	r1, #32
3000a924:	f000 81fd 	beq.w	3000ad22 <rxi316_perf_tune+0x762>
3000a928:	1b49      	subs	r1, r1, r5
3000a92a:	3913      	subs	r1, #19
3000a92c:	0609      	lsls	r1, r1, #24
3000a92e:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a932:	911d      	str	r1, [sp, #116]	; 0x74
3000a934:	9916      	ldr	r1, [sp, #88]	; 0x58
3000a936:	2920      	cmp	r1, #32
3000a938:	f000 81fa 	beq.w	3000ad30 <rxi316_perf_tune+0x770>
3000a93c:	1b49      	subs	r1, r1, r5
3000a93e:	3914      	subs	r1, #20
3000a940:	0709      	lsls	r1, r1, #28
3000a942:	911c      	str	r1, [sp, #112]	; 0x70
3000a944:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000a946:	2920      	cmp	r1, #32
3000a948:	f000 81f9 	beq.w	3000ad3e <rxi316_perf_tune+0x77e>
3000a94c:	1b49      	subs	r1, r1, r5
3000a94e:	3915      	subs	r1, #21
3000a950:	f001 010f 	and.w	r1, r1, #15
3000a954:	911b      	str	r1, [sp, #108]	; 0x6c
3000a956:	9904      	ldr	r1, [sp, #16]
3000a958:	2920      	cmp	r1, #32
3000a95a:	f000 81f6 	beq.w	3000ad4a <rxi316_perf_tune+0x78a>
3000a95e:	1b49      	subs	r1, r1, r5
3000a960:	3902      	subs	r1, #2
3000a962:	f001 011f 	and.w	r1, r1, #31
3000a966:	9117      	str	r1, [sp, #92]	; 0x5c
3000a968:	9905      	ldr	r1, [sp, #20]
3000a96a:	2920      	cmp	r1, #32
3000a96c:	f000 81f3 	beq.w	3000ad56 <rxi316_perf_tune+0x796>
3000a970:	1b49      	subs	r1, r1, r5
3000a972:	3902      	subs	r1, #2
3000a974:	0149      	lsls	r1, r1, #5
3000a976:	f401 7178 	and.w	r1, r1, #992	; 0x3e0
3000a97a:	9116      	str	r1, [sp, #88]	; 0x58
3000a97c:	9918      	ldr	r1, [sp, #96]	; 0x60
3000a97e:	2920      	cmp	r1, #32
3000a980:	f000 81f0 	beq.w	3000ad64 <rxi316_perf_tune+0x7a4>
3000a984:	1b49      	subs	r1, r1, r5
3000a986:	2b20      	cmp	r3, #32
3000a988:	f1a1 0102 	sub.w	r1, r1, #2
3000a98c:	ea4f 2181 	mov.w	r1, r1, lsl #10
3000a990:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000a994:	9115      	str	r1, [sp, #84]	; 0x54
3000a996:	f000 81eb 	beq.w	3000ad70 <rxi316_perf_tune+0x7b0>
3000a99a:	1b59      	subs	r1, r3, r5
3000a99c:	f1bb 0f20 	cmp.w	fp, #32
3000a9a0:	f1a1 0102 	sub.w	r1, r1, #2
3000a9a4:	f001 030f 	and.w	r3, r1, #15
3000a9a8:	9314      	str	r3, [sp, #80]	; 0x50
3000a9aa:	f000 81e7 	beq.w	3000ad7c <rxi316_perf_tune+0x7bc>
3000a9ae:	ebab 0105 	sub.w	r1, fp, r5
3000a9b2:	f1ba 0f20 	cmp.w	sl, #32
3000a9b6:	f1a1 0103 	sub.w	r1, r1, #3
3000a9ba:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a9be:	b2c9      	uxtb	r1, r1
3000a9c0:	f000 81e2 	beq.w	3000ad88 <rxi316_perf_tune+0x7c8>
3000a9c4:	ebaa 0305 	sub.w	r3, sl, r5
3000a9c8:	f1b9 0f20 	cmp.w	r9, #32
3000a9cc:	f1a3 0304 	sub.w	r3, r3, #4
3000a9d0:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a9d4:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a9d8:	9305      	str	r3, [sp, #20]
3000a9da:	f000 81dc 	beq.w	3000ad96 <rxi316_perf_tune+0x7d6>
3000a9de:	eba9 0305 	sub.w	r3, r9, r5
3000a9e2:	f1b8 0f20 	cmp.w	r8, #32
3000a9e6:	f1a3 0305 	sub.w	r3, r3, #5
3000a9ea:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a9ee:	b29b      	uxth	r3, r3
3000a9f0:	9304      	str	r3, [sp, #16]
3000a9f2:	f000 81d7 	beq.w	3000ada4 <rxi316_perf_tune+0x7e4>
3000a9f6:	eba8 0c05 	sub.w	ip, r8, r5
3000a9fa:	2f20      	cmp	r7, #32
3000a9fc:	f1ac 0c06 	sub.w	ip, ip, #6
3000aa00:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000aa04:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000aa08:	f000 81d1 	beq.w	3000adae <rxi316_perf_tune+0x7ee>
3000aa0c:	eba7 0e05 	sub.w	lr, r7, r5
3000aa10:	9b00      	ldr	r3, [sp, #0]
3000aa12:	f1ae 0e07 	sub.w	lr, lr, #7
3000aa16:	2b20      	cmp	r3, #32
3000aa18:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000aa1c:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000aa20:	f000 81cb 	beq.w	3000adba <rxi316_perf_tune+0x7fa>
3000aa24:	1b5f      	subs	r7, r3, r5
3000aa26:	9b01      	ldr	r3, [sp, #4]
3000aa28:	3f08      	subs	r7, #8
3000aa2a:	2b20      	cmp	r3, #32
3000aa2c:	ea4f 6707 	mov.w	r7, r7, lsl #24
3000aa30:	f007 6770 	and.w	r7, r7, #251658240	; 0xf000000
3000aa34:	f000 81c7 	beq.w	3000adc6 <rxi316_perf_tune+0x806>
3000aa38:	1b5b      	subs	r3, r3, r5
3000aa3a:	3b09      	subs	r3, #9
3000aa3c:	071b      	lsls	r3, r3, #28
3000aa3e:	9301      	str	r3, [sp, #4]
3000aa40:	9b02      	ldr	r3, [sp, #8]
3000aa42:	2b20      	cmp	r3, #32
3000aa44:	f000 811f 	beq.w	3000ac86 <rxi316_perf_tune+0x6c6>
3000aa48:	eba3 0805 	sub.w	r8, r3, r5
3000aa4c:	f1a8 080a 	sub.w	r8, r8, #10
3000aa50:	f008 080f 	and.w	r8, r8, #15
3000aa54:	9b03      	ldr	r3, [sp, #12]
3000aa56:	2b20      	cmp	r3, #32
3000aa58:	f000 8113 	beq.w	3000ac82 <rxi316_perf_tune+0x6c2>
3000aa5c:	1b58      	subs	r0, r3, r5
3000aa5e:	380b      	subs	r0, #11
3000aa60:	0100      	lsls	r0, r0, #4
3000aa62:	b2c0      	uxtb	r0, r0
3000aa64:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000aa66:	2b20      	cmp	r3, #32
3000aa68:	f000 8108 	beq.w	3000ac7c <rxi316_perf_tune+0x6bc>
3000aa6c:	eba3 0a05 	sub.w	sl, r3, r5
3000aa70:	f1aa 0a02 	sub.w	sl, sl, #2
3000aa74:	f00a 0a1f 	and.w	sl, sl, #31
3000aa78:	9b06      	ldr	r3, [sp, #24]
3000aa7a:	2b20      	cmp	r3, #32
3000aa7c:	f000 80ef 	beq.w	3000ac5e <rxi316_perf_tune+0x69e>
3000aa80:	eba3 0905 	sub.w	r9, r3, r5
3000aa84:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000aa86:	f1a9 0902 	sub.w	r9, r9, #2
3000aa8a:	2b20      	cmp	r3, #32
3000aa8c:	f009 091f 	and.w	r9, r9, #31
3000aa90:	f000 80eb 	beq.w	3000ac6a <rxi316_perf_tune+0x6aa>
3000aa94:	eba3 0b05 	sub.w	fp, r3, r5
3000aa98:	2c20      	cmp	r4, #32
3000aa9a:	f1ab 0b02 	sub.w	fp, fp, #2
3000aa9e:	f00b 0b1f 	and.w	fp, fp, #31
3000aaa2:	f000 80e7 	beq.w	3000ac74 <rxi316_perf_tune+0x6b4>
3000aaa6:	1b63      	subs	r3, r4, r5
3000aaa8:	3b02      	subs	r3, #2
3000aaaa:	015b      	lsls	r3, r3, #5
3000aaac:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000aab0:	9300      	str	r3, [sp, #0]
3000aab2:	9c07      	ldr	r4, [sp, #28]
3000aab4:	ea40 0008 	orr.w	r0, r0, r8
3000aab8:	4314      	orrs	r4, r2
3000aaba:	4622      	mov	r2, r4
3000aabc:	e9dd 450e 	ldrd	r4, r5, [sp, #56]	; 0x38
3000aac0:	432c      	orrs	r4, r5
3000aac2:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000aac4:	4329      	orrs	r1, r5
3000aac6:	e9dd 5316 	ldrd	r5, r3, [sp, #88]	; 0x58
3000aaca:	431d      	orrs	r5, r3
3000aacc:	9b08      	ldr	r3, [sp, #32]
3000aace:	431a      	orrs	r2, r3
3000aad0:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000aad2:	431c      	orrs	r4, r3
3000aad4:	9b05      	ldr	r3, [sp, #20]
3000aad6:	4319      	orrs	r1, r3
3000aad8:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000aada:	431d      	orrs	r5, r3
3000aadc:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000aade:	431a      	orrs	r2, r3
3000aae0:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000aae2:	431c      	orrs	r4, r3
3000aae4:	9b04      	ldr	r3, [sp, #16]
3000aae6:	4319      	orrs	r1, r3
3000aae8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000aaea:	ea41 0c0c 	orr.w	ip, r1, ip
3000aaee:	431a      	orrs	r2, r3
3000aaf0:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000aaf2:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000aaf4:	ea4c 0c0e 	orr.w	ip, ip, lr
3000aaf8:	430a      	orrs	r2, r1
3000aafa:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000aafc:	431c      	orrs	r4, r3
3000aafe:	ea4c 0c07 	orr.w	ip, ip, r7
3000ab02:	4f09      	ldr	r7, [pc, #36]	; (3000ab28 <rxi316_perf_tune+0x568>)
3000ab04:	430c      	orrs	r4, r1
3000ab06:	990c      	ldr	r1, [sp, #48]	; 0x30
3000ab08:	9b00      	ldr	r3, [sp, #0]
3000ab0a:	430a      	orrs	r2, r1
3000ab0c:	991d      	ldr	r1, [sp, #116]	; 0x74
3000ab0e:	ea43 030b 	orr.w	r3, r3, fp
3000ab12:	430c      	orrs	r4, r1
3000ab14:	990d      	ldr	r1, [sp, #52]	; 0x34
3000ab16:	430a      	orrs	r2, r1
3000ab18:	991c      	ldr	r1, [sp, #112]	; 0x70
3000ab1a:	4321      	orrs	r1, r4
3000ab1c:	9c01      	ldr	r4, [sp, #4]
3000ab1e:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000ab22:	e003      	b.n	3000ab2c <rxi316_perf_tune+0x56c>
3000ab24:	2001c00c 	.word	0x2001c00c
3000ab28:	41100000 	.word	0x41100000
3000ab2c:	f8c7 1404 	str.w	r1, [r7, #1028]	; 0x404
3000ab30:	ea4c 0404 	orr.w	r4, ip, r4
3000ab34:	9a1b      	ldr	r2, [sp, #108]	; 0x6c
3000ab36:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000ab3a:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000ab3e:	f8c7 4410 	str.w	r4, [r7, #1040]	; 0x410
3000ab42:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000ab46:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000ab4a:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000ab4e:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000ab52:	68f3      	ldr	r3, [r6, #12]
3000ab54:	689a      	ldr	r2, [r3, #8]
3000ab56:	e9d2 3200 	ldrd	r3, r2, [r2]
3000ab5a:	f003 0301 	and.w	r3, r3, #1
3000ab5e:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000ab62:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000ab66:	68f3      	ldr	r3, [r6, #12]
3000ab68:	689a      	ldr	r2, [r3, #8]
3000ab6a:	7a13      	ldrb	r3, [r2, #8]
3000ab6c:	6951      	ldr	r1, [r2, #20]
3000ab6e:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000ab72:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000ab76:	0209      	lsls	r1, r1, #8
3000ab78:	0412      	lsls	r2, r2, #16
3000ab7a:	b289      	uxth	r1, r1
3000ab7c:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000ab80:	430b      	orrs	r3, r1
3000ab82:	4313      	orrs	r3, r2
3000ab84:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000ab88:	68f3      	ldr	r3, [r6, #12]
3000ab8a:	6899      	ldr	r1, [r3, #8]
3000ab8c:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000ab90:	7e09      	ldrb	r1, [r1, #24]
3000ab92:	021b      	lsls	r3, r3, #8
3000ab94:	0412      	lsls	r2, r2, #16
3000ab96:	b29b      	uxth	r3, r3
3000ab98:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000ab9c:	4313      	orrs	r3, r2
3000ab9e:	430b      	orrs	r3, r1
3000aba0:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000aba4:	68f3      	ldr	r3, [r6, #12]
3000aba6:	689a      	ldr	r2, [r3, #8]
3000aba8:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000abac:	041b      	lsls	r3, r3, #16
3000abae:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000abb2:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000abb6:	4313      	orrs	r3, r2
3000abb8:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000abbc:	68f3      	ldr	r3, [r6, #12]
3000abbe:	6899      	ldr	r1, [r3, #8]
3000abc0:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000abc2:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000abc4:	005b      	lsls	r3, r3, #1
3000abc6:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000abc8:	0412      	lsls	r2, r2, #16
3000abca:	f003 0302 	and.w	r3, r3, #2
3000abce:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000abd2:	4313      	orrs	r3, r2
3000abd4:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000abd6:	f000 0101 	and.w	r1, r0, #1
3000abda:	0212      	lsls	r2, r2, #8
3000abdc:	430b      	orrs	r3, r1
3000abde:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000abe2:	4313      	orrs	r3, r2
3000abe4:	4aa2      	ldr	r2, [pc, #648]	; (3000ae70 <rxi316_perf_tune+0x8b0>)
3000abe6:	431a      	orrs	r2, r3
3000abe8:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000abec:	68f3      	ldr	r3, [r6, #12]
3000abee:	689b      	ldr	r3, [r3, #8]
3000abf0:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000abf2:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000abf6:	68f3      	ldr	r3, [r6, #12]
3000abf8:	689b      	ldr	r3, [r3, #8]
3000abfa:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000abfc:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000ac00:	68f3      	ldr	r3, [r6, #12]
3000ac02:	689b      	ldr	r3, [r3, #8]
3000ac04:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000ac06:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000ac0a:	68f3      	ldr	r3, [r6, #12]
3000ac0c:	689b      	ldr	r3, [r3, #8]
3000ac0e:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000ac10:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000ac14:	68f3      	ldr	r3, [r6, #12]
3000ac16:	689b      	ldr	r3, [r3, #8]
3000ac18:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000ac1a:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000ac1e:	68f3      	ldr	r3, [r6, #12]
3000ac20:	689b      	ldr	r3, [r3, #8]
3000ac22:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000ac24:	f003 030f 	and.w	r3, r3, #15
3000ac28:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000ac2c:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000ac30:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000ac34:	f043 0304 	orr.w	r3, r3, #4
3000ac38:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000ac3c:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000ac40:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000ac44:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000ac48:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000ac4c:	b03f      	add	sp, #252	; 0xfc
3000ac4e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000ac52:	2502      	movs	r5, #2
3000ac54:	e4c5      	b.n	3000a5e2 <rxi316_perf_tune+0x22>
3000ac56:	f10c 0c01 	add.w	ip, ip, #1
3000ac5a:	9201      	str	r2, [sp, #4]
3000ac5c:	e52c      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ac5e:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000ac60:	f04f 091f 	mov.w	r9, #31
3000ac64:	2b20      	cmp	r3, #32
3000ac66:	f47f af15 	bne.w	3000aa94 <rxi316_perf_tune+0x4d4>
3000ac6a:	2c20      	cmp	r4, #32
3000ac6c:	f04f 0b1f 	mov.w	fp, #31
3000ac70:	f47f af19 	bne.w	3000aaa6 <rxi316_perf_tune+0x4e6>
3000ac74:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000ac78:	9300      	str	r3, [sp, #0]
3000ac7a:	e71a      	b.n	3000aab2 <rxi316_perf_tune+0x4f2>
3000ac7c:	f04f 0a1f 	mov.w	sl, #31
3000ac80:	e6fa      	b.n	3000aa78 <rxi316_perf_tune+0x4b8>
3000ac82:	20f0      	movs	r0, #240	; 0xf0
3000ac84:	e6ee      	b.n	3000aa64 <rxi316_perf_tune+0x4a4>
3000ac86:	f04f 080f 	mov.w	r8, #15
3000ac8a:	e6e3      	b.n	3000aa54 <rxi316_perf_tune+0x494>
3000ac8c:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000ac90:	9109      	str	r1, [sp, #36]	; 0x24
3000ac92:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000ac94:	2920      	cmp	r1, #32
3000ac96:	f47f ade8 	bne.w	3000a86a <rxi316_perf_tune+0x2aa>
3000ac9a:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000ac9e:	910a      	str	r1, [sp, #40]	; 0x28
3000aca0:	990c      	ldr	r1, [sp, #48]	; 0x30
3000aca2:	2920      	cmp	r1, #32
3000aca4:	f47f adeb 	bne.w	3000a87e <rxi316_perf_tune+0x2be>
3000aca8:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000acac:	910b      	str	r1, [sp, #44]	; 0x2c
3000acae:	990d      	ldr	r1, [sp, #52]	; 0x34
3000acb0:	2920      	cmp	r1, #32
3000acb2:	f47f adee 	bne.w	3000a892 <rxi316_perf_tune+0x2d2>
3000acb6:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000acba:	910c      	str	r1, [sp, #48]	; 0x30
3000acbc:	990e      	ldr	r1, [sp, #56]	; 0x38
3000acbe:	2920      	cmp	r1, #32
3000acc0:	f47f adf1 	bne.w	3000a8a6 <rxi316_perf_tune+0x2e6>
3000acc4:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000acc8:	910d      	str	r1, [sp, #52]	; 0x34
3000acca:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000accc:	2920      	cmp	r1, #32
3000acce:	f47f adf2 	bne.w	3000a8b6 <rxi316_perf_tune+0x2f6>
3000acd2:	210f      	movs	r1, #15
3000acd4:	910e      	str	r1, [sp, #56]	; 0x38
3000acd6:	9910      	ldr	r1, [sp, #64]	; 0x40
3000acd8:	2920      	cmp	r1, #32
3000acda:	f47f adf5 	bne.w	3000a8c8 <rxi316_perf_tune+0x308>
3000acde:	21f0      	movs	r1, #240	; 0xf0
3000ace0:	910f      	str	r1, [sp, #60]	; 0x3c
3000ace2:	9911      	ldr	r1, [sp, #68]	; 0x44
3000ace4:	2920      	cmp	r1, #32
3000ace6:	f47f adf8 	bne.w	3000a8da <rxi316_perf_tune+0x31a>
3000acea:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000acee:	9110      	str	r1, [sp, #64]	; 0x40
3000acf0:	9912      	ldr	r1, [sp, #72]	; 0x48
3000acf2:	2920      	cmp	r1, #32
3000acf4:	f47f adfb 	bne.w	3000a8ee <rxi316_perf_tune+0x32e>
3000acf8:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000acfc:	9111      	str	r1, [sp, #68]	; 0x44
3000acfe:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000ad00:	2920      	cmp	r1, #32
3000ad02:	f47f adfd 	bne.w	3000a900 <rxi316_perf_tune+0x340>
3000ad06:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000ad0a:	9112      	str	r1, [sp, #72]	; 0x48
3000ad0c:	9914      	ldr	r1, [sp, #80]	; 0x50
3000ad0e:	2920      	cmp	r1, #32
3000ad10:	f47f ae00 	bne.w	3000a914 <rxi316_perf_tune+0x354>
3000ad14:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000ad18:	9113      	str	r1, [sp, #76]	; 0x4c
3000ad1a:	9915      	ldr	r1, [sp, #84]	; 0x54
3000ad1c:	2920      	cmp	r1, #32
3000ad1e:	f47f ae03 	bne.w	3000a928 <rxi316_perf_tune+0x368>
3000ad22:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000ad26:	911d      	str	r1, [sp, #116]	; 0x74
3000ad28:	9916      	ldr	r1, [sp, #88]	; 0x58
3000ad2a:	2920      	cmp	r1, #32
3000ad2c:	f47f ae06 	bne.w	3000a93c <rxi316_perf_tune+0x37c>
3000ad30:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000ad34:	911c      	str	r1, [sp, #112]	; 0x70
3000ad36:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000ad38:	2920      	cmp	r1, #32
3000ad3a:	f47f ae07 	bne.w	3000a94c <rxi316_perf_tune+0x38c>
3000ad3e:	210f      	movs	r1, #15
3000ad40:	911b      	str	r1, [sp, #108]	; 0x6c
3000ad42:	9904      	ldr	r1, [sp, #16]
3000ad44:	2920      	cmp	r1, #32
3000ad46:	f47f ae0a 	bne.w	3000a95e <rxi316_perf_tune+0x39e>
3000ad4a:	211f      	movs	r1, #31
3000ad4c:	9117      	str	r1, [sp, #92]	; 0x5c
3000ad4e:	9905      	ldr	r1, [sp, #20]
3000ad50:	2920      	cmp	r1, #32
3000ad52:	f47f ae0d 	bne.w	3000a970 <rxi316_perf_tune+0x3b0>
3000ad56:	f44f 7178 	mov.w	r1, #992	; 0x3e0
3000ad5a:	9116      	str	r1, [sp, #88]	; 0x58
3000ad5c:	9918      	ldr	r1, [sp, #96]	; 0x60
3000ad5e:	2920      	cmp	r1, #32
3000ad60:	f47f ae10 	bne.w	3000a984 <rxi316_perf_tune+0x3c4>
3000ad64:	f44f 41f8 	mov.w	r1, #31744	; 0x7c00
3000ad68:	2b20      	cmp	r3, #32
3000ad6a:	9115      	str	r1, [sp, #84]	; 0x54
3000ad6c:	f47f ae15 	bne.w	3000a99a <rxi316_perf_tune+0x3da>
3000ad70:	230f      	movs	r3, #15
3000ad72:	f1bb 0f20 	cmp.w	fp, #32
3000ad76:	9314      	str	r3, [sp, #80]	; 0x50
3000ad78:	f47f ae19 	bne.w	3000a9ae <rxi316_perf_tune+0x3ee>
3000ad7c:	f1ba 0f20 	cmp.w	sl, #32
3000ad80:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000ad84:	f47f ae1e 	bne.w	3000a9c4 <rxi316_perf_tune+0x404>
3000ad88:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000ad8c:	f1b9 0f20 	cmp.w	r9, #32
3000ad90:	9305      	str	r3, [sp, #20]
3000ad92:	f47f ae24 	bne.w	3000a9de <rxi316_perf_tune+0x41e>
3000ad96:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000ad9a:	f1b8 0f20 	cmp.w	r8, #32
3000ad9e:	9304      	str	r3, [sp, #16]
3000ada0:	f47f ae29 	bne.w	3000a9f6 <rxi316_perf_tune+0x436>
3000ada4:	2f20      	cmp	r7, #32
3000ada6:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000adaa:	f47f ae2f 	bne.w	3000aa0c <rxi316_perf_tune+0x44c>
3000adae:	9b00      	ldr	r3, [sp, #0]
3000adb0:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000adb4:	2b20      	cmp	r3, #32
3000adb6:	f47f ae35 	bne.w	3000aa24 <rxi316_perf_tune+0x464>
3000adba:	9b01      	ldr	r3, [sp, #4]
3000adbc:	f04f 6770 	mov.w	r7, #251658240	; 0xf000000
3000adc0:	2b20      	cmp	r3, #32
3000adc2:	f47f ae39 	bne.w	3000aa38 <rxi316_perf_tune+0x478>
3000adc6:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000adca:	9301      	str	r3, [sp, #4]
3000adcc:	e638      	b.n	3000aa40 <rxi316_perf_tune+0x480>
3000adce:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000add2:	9108      	str	r1, [sp, #32]
3000add4:	e53c      	b.n	3000a850 <rxi316_perf_tune+0x290>
3000add6:	22f0      	movs	r2, #240	; 0xf0
3000add8:	e530      	b.n	3000a83c <rxi316_perf_tune+0x27c>
3000adda:	220f      	movs	r2, #15
3000addc:	9207      	str	r2, [sp, #28]
3000adde:	e525      	b.n	3000a82c <rxi316_perf_tune+0x26c>
3000ade0:	f10c 0c01 	add.w	ip, ip, #1
3000ade4:	9202      	str	r2, [sp, #8]
3000ade6:	e467      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ade8:	f10c 0c01 	add.w	ip, ip, #1
3000adec:	9203      	str	r2, [sp, #12]
3000adee:	e463      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000adf0:	2202      	movs	r2, #2
3000adf2:	e50a      	b.n	3000a80a <rxi316_perf_tune+0x24a>
3000adf4:	3101      	adds	r1, #1
3000adf6:	9205      	str	r2, [sp, #20]
3000adf8:	e45e      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000adfa:	3101      	adds	r1, #1
3000adfc:	9204      	str	r2, [sp, #16]
3000adfe:	e45b      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae00:	481c      	ldr	r0, [pc, #112]	; (3000ae74 <rxi316_perf_tune+0x8b4>)
3000ae02:	911b      	str	r1, [sp, #108]	; 0x6c
3000ae04:	e9cd 231c 	strd	r2, r3, [sp, #112]	; 0x70
3000ae08:	f001 f9d6 	bl	3000c1b8 <__DiagPrintf_veneer>
3000ae0c:	991b      	ldr	r1, [sp, #108]	; 0x6c
3000ae0e:	e9dd 231c 	ldrd	r2, r3, [sp, #112]	; 0x70
3000ae12:	e4d5      	b.n	3000a7c0 <rxi316_perf_tune+0x200>
3000ae14:	3101      	adds	r1, #1
3000ae16:	9218      	str	r2, [sp, #96]	; 0x60
3000ae18:	e44e      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae1a:	3101      	adds	r1, #1
3000ae1c:	4614      	mov	r4, r2
3000ae1e:	e44b      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae20:	3101      	adds	r1, #1
3000ae22:	921a      	str	r2, [sp, #104]	; 0x68
3000ae24:	e448      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae26:	3101      	adds	r1, #1
3000ae28:	9219      	str	r2, [sp, #100]	; 0x64
3000ae2a:	e445      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae2c:	9207      	str	r2, [sp, #28]
3000ae2e:	e443      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae30:	9208      	str	r2, [sp, #32]
3000ae32:	e441      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae34:	9209      	str	r2, [sp, #36]	; 0x24
3000ae36:	e43f      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae38:	920b      	str	r2, [sp, #44]	; 0x2c
3000ae3a:	e43d      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae3c:	920a      	str	r2, [sp, #40]	; 0x28
3000ae3e:	e43b      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae40:	9217      	str	r2, [sp, #92]	; 0x5c
3000ae42:	e439      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae44:	9216      	str	r2, [sp, #88]	; 0x58
3000ae46:	e437      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae48:	920d      	str	r2, [sp, #52]	; 0x34
3000ae4a:	e435      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae4c:	920c      	str	r2, [sp, #48]	; 0x30
3000ae4e:	e433      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae50:	9215      	str	r2, [sp, #84]	; 0x54
3000ae52:	e431      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae54:	9214      	str	r2, [sp, #80]	; 0x50
3000ae56:	e42f      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae58:	9213      	str	r2, [sp, #76]	; 0x4c
3000ae5a:	e42d      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae5c:	9212      	str	r2, [sp, #72]	; 0x48
3000ae5e:	e42b      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae60:	9211      	str	r2, [sp, #68]	; 0x44
3000ae62:	e429      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae64:	9210      	str	r2, [sp, #64]	; 0x40
3000ae66:	e427      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae68:	920f      	str	r2, [sp, #60]	; 0x3c
3000ae6a:	e425      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae6c:	920e      	str	r2, [sp, #56]	; 0x38
3000ae6e:	e423      	b.n	3000a6b8 <rxi316_perf_tune+0xf8>
3000ae70:	40000008 	.word	0x40000008
3000ae74:	3000d65c 	.word	0x3000d65c

3000ae78 <rxi316_dram_init>:
3000ae78:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000ae7c:	4604      	mov	r4, r0
3000ae7e:	b08f      	sub	sp, #60	; 0x3c
3000ae80:	4b10      	ldr	r3, [pc, #64]	; (3000aec4 <rxi316_dram_init+0x4c>)
3000ae82:	4798      	blx	r3
3000ae84:	f894 b014 	ldrb.w	fp, [r4, #20]
3000ae88:	2101      	movs	r1, #1
3000ae8a:	900c      	str	r0, [sp, #48]	; 0x30
3000ae8c:	fa01 f30b 	lsl.w	r3, r1, fp
3000ae90:	9302      	str	r3, [sp, #8]
3000ae92:	6823      	ldr	r3, [r4, #0]
3000ae94:	78da      	ldrb	r2, [r3, #3]
3000ae96:	6923      	ldr	r3, [r4, #16]
3000ae98:	fa03 f30b 	lsl.w	r3, r3, fp
3000ae9c:	b1a2      	cbz	r2, 3000aec8 <rxi316_dram_init+0x50>
3000ae9e:	428a      	cmp	r2, r1
3000aea0:	f000 847c 	beq.w	3000b79c <rxi316_dram_init+0x924>
3000aea4:	2a02      	cmp	r2, #2
3000aea6:	bf0b      	itete	eq
3000aea8:	2206      	moveq	r2, #6
3000aeaa:	220a      	movne	r2, #10
3000aeac:	2540      	moveq	r5, #64	; 0x40
3000aeae:	2580      	movne	r5, #128	; 0x80
3000aeb0:	9207      	str	r2, [sp, #28]
3000aeb2:	bf0c      	ite	eq
3000aeb4:	2204      	moveq	r2, #4
3000aeb6:	2208      	movne	r2, #8
3000aeb8:	9208      	str	r2, [sp, #32]
3000aeba:	bf0c      	ite	eq
3000aebc:	2208      	moveq	r2, #8
3000aebe:	2210      	movne	r2, #16
3000aec0:	9201      	str	r2, [sp, #4]
3000aec2:	e007      	b.n	3000aed4 <rxi316_dram_init+0x5c>
3000aec4:	30008045 	.word	0x30008045
3000aec8:	2203      	movs	r2, #3
3000aeca:	25c0      	movs	r5, #192	; 0xc0
3000aecc:	9108      	str	r1, [sp, #32]
3000aece:	9207      	str	r2, [sp, #28]
3000aed0:	2202      	movs	r2, #2
3000aed2:	9201      	str	r2, [sp, #4]
3000aed4:	6860      	ldr	r0, [r4, #4]
3000aed6:	f04f 31ff 	mov.w	r1, #4294967295
3000aeda:	4ac2      	ldr	r2, [pc, #776]	; (3000b1e4 <rxi316_dram_init+0x36c>)
3000aedc:	6806      	ldr	r6, [r0, #0]
3000aede:	960d      	str	r6, [sp, #52]	; 0x34
3000aee0:	6846      	ldr	r6, [r0, #4]
3000aee2:	6880      	ldr	r0, [r0, #8]
3000aee4:	9609      	str	r6, [sp, #36]	; 0x24
3000aee6:	9003      	str	r0, [sp, #12]
3000aee8:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000aeec:	6820      	ldr	r0, [r4, #0]
3000aeee:	7981      	ldrb	r1, [r0, #6]
3000aef0:	6882      	ldr	r2, [r0, #8]
3000aef2:	b129      	cbz	r1, 3000af00 <rxi316_dram_init+0x88>
3000aef4:	2901      	cmp	r1, #1
3000aef6:	f000 8449 	beq.w	3000b78c <rxi316_dram_init+0x914>
3000aefa:	2903      	cmp	r1, #3
3000aefc:	f000 8456 	beq.w	3000b7ac <rxi316_dram_init+0x934>
3000af00:	3a06      	subs	r2, #6
3000af02:	7881      	ldrb	r1, [r0, #2]
3000af04:	f002 020f 	and.w	r2, r2, #15
3000af08:	7940      	ldrb	r0, [r0, #5]
3000af0a:	0109      	lsls	r1, r1, #4
3000af0c:	0280      	lsls	r0, r0, #10
3000af0e:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000af12:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000af16:	4301      	orrs	r1, r0
3000af18:	430d      	orrs	r5, r1
3000af1a:	432a      	orrs	r2, r5
3000af1c:	4db1      	ldr	r5, [pc, #708]	; (3000b1e4 <rxi316_dram_init+0x36c>)
3000af1e:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000af22:	68e2      	ldr	r2, [r4, #12]
3000af24:	6812      	ldr	r2, [r2, #0]
3000af26:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000af2a:	f002 0207 	and.w	r2, r2, #7
3000af2e:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000af32:	68e2      	ldr	r2, [r4, #12]
3000af34:	6811      	ldr	r1, [r2, #0]
3000af36:	6822      	ldr	r2, [r4, #0]
3000af38:	6848      	ldr	r0, [r1, #4]
3000af3a:	f892 9000 	ldrb.w	r9, [r2]
3000af3e:	688a      	ldr	r2, [r1, #8]
3000af40:	9004      	str	r0, [sp, #16]
3000af42:	0300      	lsls	r0, r0, #12
3000af44:	0412      	lsls	r2, r2, #16
3000af46:	f009 070f 	and.w	r7, r9, #15
3000af4a:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000af4e:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000af52:	4302      	orrs	r2, r0
3000af54:	68c8      	ldr	r0, [r1, #12]
3000af56:	0440      	lsls	r0, r0, #17
3000af58:	433a      	orrs	r2, r7
3000af5a:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000af5e:	6908      	ldr	r0, [r1, #16]
3000af60:	0480      	lsls	r0, r0, #18
3000af62:	4332      	orrs	r2, r6
3000af64:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000af68:	4310      	orrs	r0, r2
3000af6a:	694a      	ldr	r2, [r1, #20]
3000af6c:	04d2      	lsls	r2, r2, #19
3000af6e:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000af72:	4310      	orrs	r0, r2
3000af74:	698a      	ldr	r2, [r1, #24]
3000af76:	0552      	lsls	r2, r2, #21
3000af78:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000af7c:	4310      	orrs	r0, r2
3000af7e:	69ca      	ldr	r2, [r1, #28]
3000af80:	0592      	lsls	r2, r2, #22
3000af82:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000af86:	4310      	orrs	r0, r2
3000af88:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000af8a:	0612      	lsls	r2, r2, #24
3000af8c:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000af90:	4302      	orrs	r2, r0
3000af92:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000af96:	06c0      	lsls	r0, r0, #27
3000af98:	0789      	lsls	r1, r1, #30
3000af9a:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000af9e:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000afa2:	4310      	orrs	r0, r2
3000afa4:	9a02      	ldr	r2, [sp, #8]
3000afa6:	0212      	lsls	r2, r2, #8
3000afa8:	4308      	orrs	r0, r1
3000afaa:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000afae:	4310      	orrs	r0, r2
3000afb0:	6068      	str	r0, [r5, #4]
3000afb2:	6822      	ldr	r2, [r4, #0]
3000afb4:	7992      	ldrb	r2, [r2, #6]
3000afb6:	2a00      	cmp	r2, #0
3000afb8:	f040 8317 	bne.w	3000b5ea <rxi316_dram_init+0x772>
3000afbc:	686a      	ldr	r2, [r5, #4]
3000afbe:	f042 0210 	orr.w	r2, r2, #16
3000afc2:	606a      	str	r2, [r5, #4]
3000afc4:	68e2      	ldr	r2, [r4, #12]
3000afc6:	9802      	ldr	r0, [sp, #8]
3000afc8:	6812      	ldr	r2, [r2, #0]
3000afca:	f100 3aff 	add.w	sl, r0, #4294967295
3000afce:	6861      	ldr	r1, [r4, #4]
3000afd0:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000afd2:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000afd6:	9005      	str	r0, [sp, #20]
3000afd8:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000afda:	fa2e f80b 	lsr.w	r8, lr, fp
3000afde:	6949      	ldr	r1, [r1, #20]
3000afe0:	ea1e 0f0a 	tst.w	lr, sl
3000afe4:	9006      	str	r0, [sp, #24]
3000afe6:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000afe8:	fa21 fe0b 	lsr.w	lr, r1, fp
3000afec:	bf18      	it	ne
3000afee:	f108 0801 	addne.w	r8, r8, #1
3000aff2:	ea11 0f0a 	tst.w	r1, sl
3000aff6:	9906      	ldr	r1, [sp, #24]
3000aff8:	950a      	str	r5, [sp, #40]	; 0x28
3000affa:	bf18      	it	ne
3000affc:	f10e 0e01 	addne.w	lr, lr, #1
3000b000:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000b002:	0049      	lsls	r1, r1, #1
3000b004:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000b008:	950b      	str	r5, [sp, #44]	; 0x2c
3000b00a:	f001 0102 	and.w	r1, r1, #2
3000b00e:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000b012:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000b016:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000b01a:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000b01e:	0080      	lsls	r0, r0, #2
3000b020:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000b024:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000b028:	9006      	str	r0, [sp, #24]
3000b02a:	013f      	lsls	r7, r7, #4
3000b02c:	9805      	ldr	r0, [sp, #20]
3000b02e:	f00c 0c08 	and.w	ip, ip, #8
3000b032:	0176      	lsls	r6, r6, #5
3000b034:	f007 0710 	and.w	r7, r7, #16
3000b038:	f000 0001 	and.w	r0, r0, #1
3000b03c:	f006 0620 	and.w	r6, r6, #32
3000b040:	4605      	mov	r5, r0
3000b042:	9806      	ldr	r0, [sp, #24]
3000b044:	f000 0004 	and.w	r0, r0, #4
3000b048:	4301      	orrs	r1, r0
3000b04a:	4329      	orrs	r1, r5
3000b04c:	ea41 0c0c 	orr.w	ip, r1, ip
3000b050:	990a      	ldr	r1, [sp, #40]	; 0x28
3000b052:	ea4c 0707 	orr.w	r7, ip, r7
3000b056:	018d      	lsls	r5, r1, #6
3000b058:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000b05a:	4337      	orrs	r7, r6
3000b05c:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000b060:	0208      	lsls	r0, r1, #8
3000b062:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000b064:	432f      	orrs	r7, r5
3000b066:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000b06a:	06c9      	lsls	r1, r1, #27
3000b06c:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000b070:	4307      	orrs	r7, r0
3000b072:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000b076:	0736      	lsls	r6, r6, #28
3000b078:	430f      	orrs	r7, r1
3000b07a:	0752      	lsls	r2, r2, #29
3000b07c:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000b080:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000b084:	433e      	orrs	r6, r7
3000b086:	4316      	orrs	r6, r2
3000b088:	4a56      	ldr	r2, [pc, #344]	; (3000b1e4 <rxi316_dram_init+0x36c>)
3000b08a:	ea46 0608 	orr.w	r6, r6, r8
3000b08e:	ea46 060e 	orr.w	r6, r6, lr
3000b092:	6096      	str	r6, [r2, #8]
3000b094:	6862      	ldr	r2, [r4, #4]
3000b096:	6990      	ldr	r0, [r2, #24]
3000b098:	9a01      	ldr	r2, [sp, #4]
3000b09a:	2a10      	cmp	r2, #16
3000b09c:	6822      	ldr	r2, [r4, #0]
3000b09e:	7811      	ldrb	r1, [r2, #0]
3000b0a0:	7916      	ldrb	r6, [r2, #4]
3000b0a2:	f000 833d 	beq.w	3000b720 <rxi316_dram_init+0x8a8>
3000b0a6:	f1a1 0502 	sub.w	r5, r1, #2
3000b0aa:	f1a1 0e09 	sub.w	lr, r1, #9
3000b0ae:	9f01      	ldr	r7, [sp, #4]
3000b0b0:	fab5 f585 	clz	r5, r5
3000b0b4:	fabe fe8e 	clz	lr, lr
3000b0b8:	2f08      	cmp	r7, #8
3000b0ba:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b0be:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000b0c2:	ea45 020e 	orr.w	r2, r5, lr
3000b0c6:	f000 829e 	beq.w	3000b606 <rxi316_dram_init+0x78e>
3000b0ca:	9f01      	ldr	r7, [sp, #4]
3000b0cc:	2f04      	cmp	r7, #4
3000b0ce:	f000 83a5 	beq.w	3000b81c <rxi316_dram_init+0x9a4>
3000b0d2:	9f01      	ldr	r7, [sp, #4]
3000b0d4:	2f02      	cmp	r7, #2
3000b0d6:	bf08      	it	eq
3000b0d8:	4310      	orreq	r0, r2
3000b0da:	2a00      	cmp	r2, #0
3000b0dc:	f040 8298 	bne.w	3000b610 <rxi316_dram_init+0x798>
3000b0e0:	2903      	cmp	r1, #3
3000b0e2:	f000 8368 	beq.w	3000b7b6 <rxi316_dram_init+0x93e>
3000b0e6:	2500      	movs	r5, #0
3000b0e8:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b0ec:	462f      	mov	r7, r5
3000b0ee:	433d      	orrs	r5, r7
3000b0f0:	4f3c      	ldr	r7, [pc, #240]	; (3000b1e4 <rxi316_dram_init+0x36c>)
3000b0f2:	430d      	orrs	r5, r1
3000b0f4:	4305      	orrs	r5, r0
3000b0f6:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000b0fa:	637e      	str	r6, [r7, #52]	; 0x34
3000b0fc:	6862      	ldr	r2, [r4, #4]
3000b0fe:	9903      	ldr	r1, [sp, #12]
3000b100:	69d2      	ldr	r2, [r2, #28]
3000b102:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000b106:	63ba      	str	r2, [r7, #56]	; 0x38
3000b108:	6822      	ldr	r2, [r4, #0]
3000b10a:	7810      	ldrb	r0, [r2, #0]
3000b10c:	6862      	ldr	r2, [r4, #4]
3000b10e:	2803      	cmp	r0, #3
3000b110:	6a11      	ldr	r1, [r2, #32]
3000b112:	f000 82f8 	beq.w	3000b706 <rxi316_dram_init+0x88e>
3000b116:	2809      	cmp	r0, #9
3000b118:	f000 830d 	beq.w	3000b736 <rxi316_dram_init+0x8be>
3000b11c:	4831      	ldr	r0, [pc, #196]	; (3000b1e4 <rxi316_dram_init+0x36c>)
3000b11e:	63c1      	str	r1, [r0, #60]	; 0x3c
3000b120:	6862      	ldr	r2, [r4, #4]
3000b122:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000b124:	6402      	str	r2, [r0, #64]	; 0x40
3000b126:	6865      	ldr	r5, [r4, #4]
3000b128:	68e8      	ldr	r0, [r5, #12]
3000b12a:	682a      	ldr	r2, [r5, #0]
3000b12c:	6869      	ldr	r1, [r5, #4]
3000b12e:	4402      	add	r2, r0
3000b130:	68ae      	ldr	r6, [r5, #8]
3000b132:	4401      	add	r1, r0
3000b134:	ea1a 0f02 	tst.w	sl, r2
3000b138:	f000 82b8 	beq.w	3000b6ac <rxi316_dram_init+0x834>
3000b13c:	fa22 f20b 	lsr.w	r2, r2, fp
3000b140:	1c50      	adds	r0, r2, #1
3000b142:	ea1a 0f01 	tst.w	sl, r1
3000b146:	fa21 f50b 	lsr.w	r5, r1, fp
3000b14a:	6822      	ldr	r2, [r4, #0]
3000b14c:	fa26 f10b 	lsr.w	r1, r6, fp
3000b150:	bf18      	it	ne
3000b152:	3501      	addne	r5, #1
3000b154:	ea1a 0f06 	tst.w	sl, r6
3000b158:	7816      	ldrb	r6, [r2, #0]
3000b15a:	bf18      	it	ne
3000b15c:	3101      	addne	r1, #1
3000b15e:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000b162:	1eb2      	subs	r2, r6, #2
3000b164:	2a01      	cmp	r2, #1
3000b166:	f240 82a4 	bls.w	3000b6b2 <rxi316_dram_init+0x83a>
3000b16a:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000b16e:	fbb7 f2f3 	udiv	r2, r7, r3
3000b172:	fb03 7712 	mls	r7, r3, r2, r7
3000b176:	b107      	cbz	r7, 3000b17a <rxi316_dram_init+0x302>
3000b178:	3201      	adds	r2, #1
3000b17a:	2e09      	cmp	r6, #9
3000b17c:	f040 82b3 	bne.w	3000b6e6 <rxi316_dram_init+0x86e>
3000b180:	9e08      	ldr	r6, [sp, #32]
3000b182:	fa26 f60b 	lsr.w	r6, r6, fp
3000b186:	016d      	lsls	r5, r5, #5
3000b188:	f000 001f 	and.w	r0, r0, #31
3000b18c:	0289      	lsls	r1, r1, #10
3000b18e:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000b192:	0512      	lsls	r2, r2, #20
3000b194:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000b198:	4305      	orrs	r5, r0
3000b19a:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000b19e:	430d      	orrs	r5, r1
3000b1a0:	4910      	ldr	r1, [pc, #64]	; (3000b1e4 <rxi316_dram_init+0x36c>)
3000b1a2:	4315      	orrs	r5, r2
3000b1a4:	0672      	lsls	r2, r6, #25
3000b1a6:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000b1aa:	432a      	orrs	r2, r5
3000b1ac:	630a      	str	r2, [r1, #48]	; 0x30
3000b1ae:	68a0      	ldr	r0, [r4, #8]
3000b1b0:	6802      	ldr	r2, [r0, #0]
3000b1b2:	fbb2 f1f3 	udiv	r1, r2, r3
3000b1b6:	fb03 2211 	mls	r2, r3, r1, r2
3000b1ba:	2a00      	cmp	r2, #0
3000b1bc:	f000 8274 	beq.w	3000b6a8 <rxi316_dram_init+0x830>
3000b1c0:	1c4a      	adds	r2, r1, #1
3000b1c2:	9205      	str	r2, [sp, #20]
3000b1c4:	6842      	ldr	r2, [r0, #4]
3000b1c6:	fbb2 f0f3 	udiv	r0, r2, r3
3000b1ca:	fb03 2210 	mls	r2, r3, r0, r2
3000b1ce:	2a00      	cmp	r2, #0
3000b1d0:	f000 8268 	beq.w	3000b6a4 <rxi316_dram_init+0x82c>
3000b1d4:	38c7      	subs	r0, #199	; 0xc7
3000b1d6:	68e2      	ldr	r2, [r4, #12]
3000b1d8:	6811      	ldr	r1, [r2, #0]
3000b1da:	4a03      	ldr	r2, [pc, #12]	; (3000b1e8 <rxi316_dram_init+0x370>)
3000b1dc:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000b1e0:	e004      	b.n	3000b1ec <rxi316_dram_init+0x374>
3000b1e2:	bf00      	nop
3000b1e4:	41100000 	.word	0x41100000
3000b1e8:	00ffff00 	.word	0x00ffff00
3000b1ec:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000b1f0:	900a      	str	r0, [sp, #40]	; 0x28
3000b1f2:	0712      	lsls	r2, r2, #28
3000b1f4:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000b1f8:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000b1fc:	430a      	orrs	r2, r1
3000b1fe:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000b202:	430a      	orrs	r2, r1
3000b204:	49c3      	ldr	r1, [pc, #780]	; (3000b514 <rxi316_dram_init+0x69c>)
3000b206:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000b20a:	610a      	str	r2, [r1, #16]
3000b20c:	68a2      	ldr	r2, [r4, #8]
3000b20e:	6911      	ldr	r1, [r2, #16]
3000b210:	fbb1 fcf3 	udiv	ip, r1, r3
3000b214:	fb03 111c 	mls	r1, r3, ip, r1
3000b218:	2900      	cmp	r1, #0
3000b21a:	f000 8240 	beq.w	3000b69e <rxi316_dram_init+0x826>
3000b21e:	f10c 0101 	add.w	r1, ip, #1
3000b222:	9106      	str	r1, [sp, #24]
3000b224:	6991      	ldr	r1, [r2, #24]
3000b226:	fbb1 f0f3 	udiv	r0, r1, r3
3000b22a:	fb03 1110 	mls	r1, r3, r0, r1
3000b22e:	b101      	cbz	r1, 3000b232 <rxi316_dram_init+0x3ba>
3000b230:	3001      	adds	r0, #1
3000b232:	68d1      	ldr	r1, [r2, #12]
3000b234:	fbb1 f5f3 	udiv	r5, r1, r3
3000b238:	fb03 1115 	mls	r1, r3, r5, r1
3000b23c:	9503      	str	r5, [sp, #12]
3000b23e:	b109      	cbz	r1, 3000b244 <rxi316_dram_init+0x3cc>
3000b240:	3501      	adds	r5, #1
3000b242:	9503      	str	r5, [sp, #12]
3000b244:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000b246:	fbb5 f1f3 	udiv	r1, r5, r3
3000b24a:	fb03 5511 	mls	r5, r3, r1, r5
3000b24e:	b105      	cbz	r5, 3000b252 <rxi316_dram_init+0x3da>
3000b250:	3101      	adds	r1, #1
3000b252:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000b254:	0349      	lsls	r1, r1, #13
3000b256:	68e2      	ldr	r2, [r4, #12]
3000b258:	fa25 f60b 	lsr.w	r6, r5, fp
3000b25c:	ea15 0f0a 	tst.w	r5, sl
3000b260:	6812      	ldr	r2, [r2, #0]
3000b262:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000b266:	bf18      	it	ne
3000b268:	3601      	addne	r6, #1
3000b26a:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000b26c:	05d2      	lsls	r2, r2, #23
3000b26e:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000b272:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000b276:	9a06      	ldr	r2, [sp, #24]
3000b278:	0112      	lsls	r2, r2, #4
3000b27a:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000b27e:	4332      	orrs	r2, r6
3000b280:	0246      	lsls	r6, r0, #9
3000b282:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000b286:	4316      	orrs	r6, r2
3000b288:	9a03      	ldr	r2, [sp, #12]
3000b28a:	f002 020f 	and.w	r2, r2, #15
3000b28e:	4316      	orrs	r6, r2
3000b290:	4aa0      	ldr	r2, [pc, #640]	; (3000b514 <rxi316_dram_init+0x69c>)
3000b292:	4331      	orrs	r1, r6
3000b294:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000b298:	6151      	str	r1, [r2, #20]
3000b29a:	68a7      	ldr	r7, [r4, #8]
3000b29c:	697a      	ldr	r2, [r7, #20]
3000b29e:	fbb2 fef3 	udiv	lr, r2, r3
3000b2a2:	fb03 221e 	mls	r2, r3, lr, r2
3000b2a6:	b10a      	cbz	r2, 3000b2ac <rxi316_dram_init+0x434>
3000b2a8:	f10e 0e01 	add.w	lr, lr, #1
3000b2ac:	6939      	ldr	r1, [r7, #16]
3000b2ae:	fbb1 f2f3 	udiv	r2, r1, r3
3000b2b2:	fb03 1112 	mls	r1, r3, r2, r1
3000b2b6:	b101      	cbz	r1, 3000b2ba <rxi316_dram_init+0x442>
3000b2b8:	3201      	adds	r2, #1
3000b2ba:	68fd      	ldr	r5, [r7, #12]
3000b2bc:	fbb5 f1f3 	udiv	r1, r5, r3
3000b2c0:	fb03 5511 	mls	r5, r3, r1, r5
3000b2c4:	b105      	cbz	r5, 3000b2c8 <rxi316_dram_init+0x450>
3000b2c6:	3101      	adds	r1, #1
3000b2c8:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000b2ca:	fbb5 f6f3 	udiv	r6, r5, r3
3000b2ce:	fb03 5516 	mls	r5, r3, r6, r5
3000b2d2:	b105      	cbz	r5, 3000b2d6 <rxi316_dram_init+0x45e>
3000b2d4:	3601      	adds	r6, #1
3000b2d6:	68bd      	ldr	r5, [r7, #8]
3000b2d8:	4411      	add	r1, r2
3000b2da:	fbb5 f2f3 	udiv	r2, r5, r3
3000b2de:	42b1      	cmp	r1, r6
3000b2e0:	fb03 5512 	mls	r5, r3, r2, r5
3000b2e4:	bf38      	it	cc
3000b2e6:	4631      	movcc	r1, r6
3000b2e8:	b905      	cbnz	r5, 3000b2ec <rxi316_dram_init+0x474>
3000b2ea:	3a01      	subs	r2, #1
3000b2ec:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000b2ee:	ea15 0f0a 	tst.w	r5, sl
3000b2f2:	fa25 fc0b 	lsr.w	ip, r5, fp
3000b2f6:	69fd      	ldr	r5, [r7, #28]
3000b2f8:	bf18      	it	ne
3000b2fa:	f10c 0c01 	addne.w	ip, ip, #1
3000b2fe:	fa25 f60b 	lsr.w	r6, r5, fp
3000b302:	ea15 0f0a 	tst.w	r5, sl
3000b306:	bf18      	it	ne
3000b308:	3601      	addne	r6, #1
3000b30a:	f1b9 0f09 	cmp.w	r9, #9
3000b30e:	f000 81f5 	beq.w	3000b6fc <rxi316_dram_init+0x884>
3000b312:	f1b9 0f02 	cmp.w	r9, #2
3000b316:	f000 8233 	beq.w	3000b780 <rxi316_dram_init+0x908>
3000b31a:	f1b9 0f03 	cmp.w	r9, #3
3000b31e:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b320:	f000 8298 	beq.w	3000b854 <rxi316_dram_init+0x9dc>
3000b324:	46a8      	mov	r8, r5
3000b326:	9d01      	ldr	r5, [sp, #4]
3000b328:	44a8      	add	r8, r5
3000b32a:	fa28 f50b 	lsr.w	r5, r8, fp
3000b32e:	ea18 0f0a 	tst.w	r8, sl
3000b332:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000b336:	bf18      	it	ne
3000b338:	3501      	addne	r5, #1
3000b33a:	fbb8 f7f3 	udiv	r7, r8, r3
3000b33e:	fb03 8817 	mls	r8, r3, r7, r8
3000b342:	f1b8 0f00 	cmp.w	r8, #0
3000b346:	d000      	beq.n	3000b34a <rxi316_dram_init+0x4d2>
3000b348:	3701      	adds	r7, #1
3000b34a:	0109      	lsls	r1, r1, #4
3000b34c:	f00e 0e0f 	and.w	lr, lr, #15
3000b350:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000b354:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000b358:	063f      	lsls	r7, r7, #24
3000b35a:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000b35e:	ea41 0e0e 	orr.w	lr, r1, lr
3000b362:	0291      	lsls	r1, r2, #10
3000b364:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000b368:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000b36c:	ea4e 0e01 	orr.w	lr, lr, r1
3000b370:	0471      	lsls	r1, r6, #17
3000b372:	ea4e 0c0c 	orr.w	ip, lr, ip
3000b376:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000b37a:	ea4c 0c01 	orr.w	ip, ip, r1
3000b37e:	0529      	lsls	r1, r5, #20
3000b380:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000b384:	ea4c 0c01 	orr.w	ip, ip, r1
3000b388:	4962      	ldr	r1, [pc, #392]	; (3000b514 <rxi316_dram_init+0x69c>)
3000b38a:	ea4c 0707 	orr.w	r7, ip, r7
3000b38e:	618f      	str	r7, [r1, #24]
3000b390:	6821      	ldr	r1, [r4, #0]
3000b392:	780f      	ldrb	r7, [r1, #0]
3000b394:	2f02      	cmp	r7, #2
3000b396:	f000 8152 	beq.w	3000b63e <rxi316_dram_init+0x7c6>
3000b39a:	2f09      	cmp	r7, #9
3000b39c:	f000 814f 	beq.w	3000b63e <rxi316_dram_init+0x7c6>
3000b3a0:	68e1      	ldr	r1, [r4, #12]
3000b3a2:	2f03      	cmp	r7, #3
3000b3a4:	6809      	ldr	r1, [r1, #0]
3000b3a6:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000b3a8:	f001 080f 	and.w	r8, r1, #15
3000b3ac:	68a1      	ldr	r1, [r4, #8]
3000b3ae:	6a09      	ldr	r1, [r1, #32]
3000b3b0:	d005      	beq.n	3000b3be <rxi316_dram_init+0x546>
3000b3b2:	ea0a 0701 	and.w	r7, sl, r1
3000b3b6:	fa21 f10b 	lsr.w	r1, r1, fp
3000b3ba:	b107      	cbz	r7, 3000b3be <rxi316_dram_init+0x546>
3000b3bc:	3101      	adds	r1, #1
3000b3be:	f1b9 0f02 	cmp.w	r9, #2
3000b3c2:	f000 8154 	beq.w	3000b66e <rxi316_dram_init+0x7f6>
3000b3c6:	f1b9 0f03 	cmp.w	r9, #3
3000b3ca:	f000 81b6 	beq.w	3000b73a <rxi316_dram_init+0x8c2>
3000b3ce:	f1b9 0f09 	cmp.w	r9, #9
3000b3d2:	f000 822f 	beq.w	3000b834 <rxi316_dram_init+0x9bc>
3000b3d6:	f04f 0e00 	mov.w	lr, #0
3000b3da:	46f1      	mov	r9, lr
3000b3dc:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b3e0:	0109      	lsls	r1, r1, #4
3000b3e2:	9f06      	ldr	r7, [sp, #24]
3000b3e4:	b2c9      	uxtb	r1, r1
3000b3e6:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000b3ea:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b3ec:	ea41 0108 	orr.w	r1, r1, r8
3000b3f0:	4339      	orrs	r1, r7
3000b3f2:	4f48      	ldr	r7, [pc, #288]	; (3000b514 <rxi316_dram_init+0x69c>)
3000b3f4:	ea41 0109 	orr.w	r1, r1, r9
3000b3f8:	ea41 010e 	orr.w	r1, r1, lr
3000b3fc:	61f9      	str	r1, [r7, #28]
3000b3fe:	68a7      	ldr	r7, [r4, #8]
3000b400:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000b402:	fbb4 f1f3 	udiv	r1, r4, r3
3000b406:	fb03 4311 	mls	r3, r3, r1, r4
3000b40a:	b103      	cbz	r3, 3000b40e <rxi316_dram_init+0x596>
3000b40c:	3101      	adds	r1, #1
3000b40e:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000b410:	094b      	lsrs	r3, r1, #5
3000b412:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000b416:	0912      	lsrs	r2, r2, #4
3000b418:	ea14 0f0a 	tst.w	r4, sl
3000b41c:	fa24 f80b 	lsr.w	r8, r4, fp
3000b420:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000b422:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000b426:	bf18      	it	ne
3000b428:	f108 0801 	addne.w	r8, r8, #1
3000b42c:	9306      	str	r3, [sp, #24]
3000b42e:	ea14 0f0a 	tst.w	r4, sl
3000b432:	fa24 fe0b 	lsr.w	lr, r4, fp
3000b436:	9b01      	ldr	r3, [sp, #4]
3000b438:	fa29 f40b 	lsr.w	r4, r9, fp
3000b43c:	bf18      	it	ne
3000b43e:	f10e 0e01 	addne.w	lr, lr, #1
3000b442:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000b444:	ea19 0f0a 	tst.w	r9, sl
3000b448:	fa23 f90b 	lsr.w	r9, r3, fp
3000b44c:	9905      	ldr	r1, [sp, #20]
3000b44e:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000b452:	bf18      	it	ne
3000b454:	3401      	addne	r4, #1
3000b456:	45b9      	cmp	r9, r7
3000b458:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000b45c:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000b460:	f240 80eb 	bls.w	3000b63a <rxi316_dram_init+0x7c2>
3000b464:	eba9 0707 	sub.w	r7, r9, r7
3000b468:	fa27 f40b 	lsr.w	r4, r7, fp
3000b46c:	ea17 070a 	ands.w	r7, r7, sl
3000b470:	d001      	beq.n	3000b476 <rxi316_dram_init+0x5fe>
3000b472:	3401      	adds	r4, #1
3000b474:	2700      	movs	r7, #0
3000b476:	9b04      	ldr	r3, [sp, #16]
3000b478:	b10b      	cbz	r3, 3000b47e <rxi316_dram_init+0x606>
3000b47a:	073b      	lsls	r3, r7, #28
3000b47c:	9304      	str	r3, [sp, #16]
3000b47e:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000b482:	9b06      	ldr	r3, [sp, #24]
3000b484:	0289      	lsls	r1, r1, #10
3000b486:	02c0      	lsls	r0, r0, #11
3000b488:	f00c 0c04 	and.w	ip, ip, #4
3000b48c:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000b490:	01d2      	lsls	r2, r2, #7
3000b492:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000b496:	0236      	lsls	r6, r6, #8
3000b498:	ea4c 0c01 	orr.w	ip, ip, r1
3000b49c:	b2d2      	uxtb	r2, r2
3000b49e:	026d      	lsls	r5, r5, #9
3000b4a0:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000b4a4:	ea4c 0c00 	orr.w	ip, ip, r0
3000b4a8:	00df      	lsls	r7, r3, #3
3000b4aa:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000b4ae:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000b4b2:	ea4c 0202 	orr.w	r2, ip, r2
3000b4b6:	f007 0708 	and.w	r7, r7, #8
3000b4ba:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000b4be:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000b4c2:	4332      	orrs	r2, r6
3000b4c4:	0524      	lsls	r4, r4, #20
3000b4c6:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000b4ca:	4912      	ldr	r1, [pc, #72]	; (3000b514 <rxi316_dram_init+0x69c>)
3000b4cc:	ea42 0305 	orr.w	r3, r2, r5
3000b4d0:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000b4d4:	9a04      	ldr	r2, [sp, #16]
3000b4d6:	433b      	orrs	r3, r7
3000b4d8:	ea43 0308 	orr.w	r3, r3, r8
3000b4dc:	ea43 030e 	orr.w	r3, r3, lr
3000b4e0:	4323      	orrs	r3, r4
3000b4e2:	4313      	orrs	r3, r2
3000b4e4:	9a03      	ldr	r2, [sp, #12]
3000b4e6:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000b4ea:	620b      	str	r3, [r1, #32]
3000b4ec:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000b4f0:	9b02      	ldr	r3, [sp, #8]
3000b4f2:	2b01      	cmp	r3, #1
3000b4f4:	d002      	beq.n	3000b4fc <rxi316_dram_init+0x684>
3000b4f6:	9b08      	ldr	r3, [sp, #32]
3000b4f8:	3303      	adds	r3, #3
3000b4fa:	9307      	str	r3, [sp, #28]
3000b4fc:	4b05      	ldr	r3, [pc, #20]	; (3000b514 <rxi316_dram_init+0x69c>)
3000b4fe:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000b502:	9a07      	ldr	r2, [sp, #28]
3000b504:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000b508:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000b50c:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000b510:	e002      	b.n	3000b518 <rxi316_dram_init+0x6a0>
3000b512:	bf00      	nop
3000b514:	41100000 	.word	0x41100000
3000b518:	60da      	str	r2, [r3, #12]
3000b51a:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b51e:	430a      	orrs	r2, r1
3000b520:	49b3      	ldr	r1, [pc, #716]	; (3000b7f0 <rxi316_dram_init+0x978>)
3000b522:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b526:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000b52a:	0096      	lsls	r6, r2, #2
3000b52c:	d57f      	bpl.n	3000b62e <rxi316_dram_init+0x7b6>
3000b52e:	691a      	ldr	r2, [r3, #16]
3000b530:	4cb0      	ldr	r4, [pc, #704]	; (3000b7f4 <rxi316_dram_init+0x97c>)
3000b532:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000b536:	611a      	str	r2, [r3, #16]
3000b538:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000b53c:	601a      	str	r2, [r3, #0]
3000b53e:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000b542:	f043 0303 	orr.w	r3, r3, #3
3000b546:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000b54a:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000b54e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000b550:	2b01      	cmp	r3, #1
3000b552:	4ba9      	ldr	r3, [pc, #676]	; (3000b7f8 <rxi316_dram_init+0x980>)
3000b554:	bf0c      	ite	eq
3000b556:	4aa9      	ldreq	r2, [pc, #676]	; (3000b7fc <rxi316_dram_init+0x984>)
3000b558:	4aa9      	ldrne	r2, [pc, #676]	; (3000b800 <rxi316_dram_init+0x988>)
3000b55a:	601a      	str	r2, [r3, #0]
3000b55c:	2203      	movs	r2, #3
3000b55e:	68e0      	ldr	r0, [r4, #12]
3000b560:	4ba8      	ldr	r3, [pc, #672]	; (3000b804 <rxi316_dram_init+0x98c>)
3000b562:	4003      	ands	r3, r0
3000b564:	60e3      	str	r3, [r4, #12]
3000b566:	2308      	movs	r3, #8
3000b568:	62a1      	str	r1, [r4, #40]	; 0x28
3000b56a:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b56c:	6023      	str	r3, [r4, #0]
3000b56e:	6823      	ldr	r3, [r4, #0]
3000b570:	071d      	lsls	r5, r3, #28
3000b572:	d5fc      	bpl.n	3000b56e <rxi316_dram_init+0x6f6>
3000b574:	4ba4      	ldr	r3, [pc, #656]	; (3000b808 <rxi316_dram_init+0x990>)
3000b576:	2001      	movs	r0, #1
3000b578:	4798      	blx	r3
3000b57a:	68e2      	ldr	r2, [r4, #12]
3000b57c:	4ba1      	ldr	r3, [pc, #644]	; (3000b804 <rxi316_dram_init+0x98c>)
3000b57e:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000b582:	4013      	ands	r3, r2
3000b584:	2203      	movs	r2, #3
3000b586:	60e3      	str	r3, [r4, #12]
3000b588:	2308      	movs	r3, #8
3000b58a:	62a1      	str	r1, [r4, #40]	; 0x28
3000b58c:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b58e:	4a99      	ldr	r2, [pc, #612]	; (3000b7f4 <rxi316_dram_init+0x97c>)
3000b590:	6023      	str	r3, [r4, #0]
3000b592:	6813      	ldr	r3, [r2, #0]
3000b594:	071c      	lsls	r4, r3, #28
3000b596:	d5fc      	bpl.n	3000b592 <rxi316_dram_init+0x71a>
3000b598:	68d0      	ldr	r0, [r2, #12]
3000b59a:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000b59e:	4b99      	ldr	r3, [pc, #612]	; (3000b804 <rxi316_dram_init+0x98c>)
3000b5a0:	4003      	ands	r3, r0
3000b5a2:	2008      	movs	r0, #8
3000b5a4:	60d3      	str	r3, [r2, #12]
3000b5a6:	6291      	str	r1, [r2, #40]	; 0x28
3000b5a8:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000b5aa:	4992      	ldr	r1, [pc, #584]	; (3000b7f4 <rxi316_dram_init+0x97c>)
3000b5ac:	f023 0302 	bic.w	r3, r3, #2
3000b5b0:	62d3      	str	r3, [r2, #44]	; 0x2c
3000b5b2:	6010      	str	r0, [r2, #0]
3000b5b4:	680b      	ldr	r3, [r1, #0]
3000b5b6:	0718      	lsls	r0, r3, #28
3000b5b8:	d5fc      	bpl.n	3000b5b4 <rxi316_dram_init+0x73c>
3000b5ba:	4a8e      	ldr	r2, [pc, #568]	; (3000b7f4 <rxi316_dram_init+0x97c>)
3000b5bc:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000b5c0:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b5c4:	60d3      	str	r3, [r2, #12]
3000b5c6:	980a      	ldr	r0, [sp, #40]	; 0x28
3000b5c8:	6913      	ldr	r3, [r2, #16]
3000b5ca:	4318      	orrs	r0, r3
3000b5cc:	6110      	str	r0, [r2, #16]
3000b5ce:	6913      	ldr	r3, [r2, #16]
3000b5d0:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000b5d4:	6113      	str	r3, [r2, #16]
3000b5d6:	6011      	str	r1, [r2, #0]
3000b5d8:	68d3      	ldr	r3, [r2, #12]
3000b5da:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000b5de:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b5e2:	d1f9      	bne.n	3000b5d8 <rxi316_dram_init+0x760>
3000b5e4:	b00f      	add	sp, #60	; 0x3c
3000b5e6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b5ea:	2a01      	cmp	r2, #1
3000b5ec:	f000 80d1 	beq.w	3000b792 <rxi316_dram_init+0x91a>
3000b5f0:	2a02      	cmp	r2, #2
3000b5f2:	f000 8139 	beq.w	3000b868 <rxi316_dram_init+0x9f0>
3000b5f6:	2a03      	cmp	r2, #3
3000b5f8:	f47f ace4 	bne.w	3000afc4 <rxi316_dram_init+0x14c>
3000b5fc:	686a      	ldr	r2, [r5, #4]
3000b5fe:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b602:	606a      	str	r2, [r5, #4]
3000b604:	e4de      	b.n	3000afc4 <rxi316_dram_init+0x14c>
3000b606:	2a00      	cmp	r2, #0
3000b608:	f43f ad6a 	beq.w	3000b0e0 <rxi316_dram_init+0x268>
3000b60c:	2203      	movs	r2, #3
3000b60e:	4310      	orrs	r0, r2
3000b610:	68a2      	ldr	r2, [r4, #8]
3000b612:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b614:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b616:	013f      	lsls	r7, r7, #4
3000b618:	2d00      	cmp	r5, #0
3000b61a:	f000 80d7 	beq.w	3000b7cc <rxi316_dram_init+0x954>
3000b61e:	1e91      	subs	r1, r2, #2
3000b620:	2906      	cmp	r1, #6
3000b622:	f200 80b0 	bhi.w	3000b786 <rxi316_dram_init+0x90e>
3000b626:	1e55      	subs	r5, r2, #1
3000b628:	2100      	movs	r1, #0
3000b62a:	026d      	lsls	r5, r5, #9
3000b62c:	e55f      	b.n	3000b0ee <rxi316_dram_init+0x276>
3000b62e:	2201      	movs	r2, #1
3000b630:	601a      	str	r2, [r3, #0]
3000b632:	681a      	ldr	r2, [r3, #0]
3000b634:	07d2      	lsls	r2, r2, #31
3000b636:	d5fc      	bpl.n	3000b632 <rxi316_dram_init+0x7ba>
3000b638:	e7bf      	b.n	3000b5ba <rxi316_dram_init+0x742>
3000b63a:	2701      	movs	r7, #1
3000b63c:	e71b      	b.n	3000b476 <rxi316_dram_init+0x5fe>
3000b63e:	68a1      	ldr	r1, [r4, #8]
3000b640:	2f03      	cmp	r7, #3
3000b642:	6a09      	ldr	r1, [r1, #32]
3000b644:	f000 80bf 	beq.w	3000b7c6 <rxi316_dram_init+0x94e>
3000b648:	2f02      	cmp	r7, #2
3000b64a:	f040 8118 	bne.w	3000b87e <rxi316_dram_init+0xa06>
3000b64e:	fbb1 fcf3 	udiv	ip, r1, r3
3000b652:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b656:	4661      	mov	r1, ip
3000b658:	f1be 0f00 	cmp.w	lr, #0
3000b65c:	f000 80a9 	beq.w	3000b7b2 <rxi316_dram_init+0x93a>
3000b660:	f1b9 0f02 	cmp.w	r9, #2
3000b664:	f101 0101 	add.w	r1, r1, #1
3000b668:	46b8      	mov	r8, r7
3000b66a:	f47f aeac 	bne.w	3000b3c6 <rxi316_dram_init+0x54e>
3000b66e:	4f67      	ldr	r7, [pc, #412]	; (3000b80c <rxi316_dram_init+0x994>)
3000b670:	f04f 0e00 	mov.w	lr, #0
3000b674:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b810 <rxi316_dram_init+0x998>
3000b678:	fbb7 f7f3 	udiv	r7, r7, r3
3000b67c:	fbb9 f9f3 	udiv	r9, r9, r3
3000b680:	09ff      	lsrs	r7, r7, #7
3000b682:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b686:	3701      	adds	r7, #1
3000b688:	f109 0c01 	add.w	ip, r9, #1
3000b68c:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b814 <rxi316_dram_init+0x99c>
3000b690:	023f      	lsls	r7, r7, #8
3000b692:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b696:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b69a:	9709      	str	r7, [sp, #36]	; 0x24
3000b69c:	e6a0      	b.n	3000b3e0 <rxi316_dram_init+0x568>
3000b69e:	f8cd c018 	str.w	ip, [sp, #24]
3000b6a2:	e5bf      	b.n	3000b224 <rxi316_dram_init+0x3ac>
3000b6a4:	38c8      	subs	r0, #200	; 0xc8
3000b6a6:	e596      	b.n	3000b1d6 <rxi316_dram_init+0x35e>
3000b6a8:	9105      	str	r1, [sp, #20]
3000b6aa:	e58b      	b.n	3000b1c4 <rxi316_dram_init+0x34c>
3000b6ac:	fa22 f00b 	lsr.w	r0, r2, fp
3000b6b0:	e547      	b.n	3000b142 <rxi316_dram_init+0x2ca>
3000b6b2:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000b6b6:	ea1a 0f07 	tst.w	sl, r7
3000b6ba:	fa27 f20b 	lsr.w	r2, r7, fp
3000b6be:	d000      	beq.n	3000b6c2 <rxi316_dram_init+0x84a>
3000b6c0:	3201      	adds	r2, #1
3000b6c2:	2e02      	cmp	r6, #2
3000b6c4:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b6c8:	d10f      	bne.n	3000b6ea <rxi316_dram_init+0x872>
3000b6ca:	9e08      	ldr	r6, [sp, #32]
3000b6cc:	3e02      	subs	r6, #2
3000b6ce:	fa26 f60b 	lsr.w	r6, r6, fp
3000b6d2:	4437      	add	r7, r6
3000b6d4:	fbb7 f6f3 	udiv	r6, r7, r3
3000b6d8:	fb03 7716 	mls	r7, r3, r6, r7
3000b6dc:	2f00      	cmp	r7, #0
3000b6de:	f43f ad52 	beq.w	3000b186 <rxi316_dram_init+0x30e>
3000b6e2:	3601      	adds	r6, #1
3000b6e4:	e54f      	b.n	3000b186 <rxi316_dram_init+0x30e>
3000b6e6:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b6ea:	fbb7 f6f3 	udiv	r6, r7, r3
3000b6ee:	fb03 7716 	mls	r7, r3, r6, r7
3000b6f2:	2f00      	cmp	r7, #0
3000b6f4:	f43f ad47 	beq.w	3000b186 <rxi316_dram_init+0x30e>
3000b6f8:	3601      	adds	r6, #1
3000b6fa:	e544      	b.n	3000b186 <rxi316_dram_init+0x30e>
3000b6fc:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b6fe:	46a8      	mov	r8, r5
3000b700:	9d08      	ldr	r5, [sp, #32]
3000b702:	44a8      	add	r8, r5
3000b704:	e611      	b.n	3000b32a <rxi316_dram_init+0x4b2>
3000b706:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b708:	1f50      	subs	r0, r2, #5
3000b70a:	00c0      	lsls	r0, r0, #3
3000b70c:	68e2      	ldr	r2, [r4, #12]
3000b70e:	6812      	ldr	r2, [r2, #0]
3000b710:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b712:	fab2 f282 	clz	r2, r2
3000b716:	0952      	lsrs	r2, r2, #5
3000b718:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b71c:	4301      	orrs	r1, r0
3000b71e:	e4fd      	b.n	3000b11c <rxi316_dram_init+0x2a4>
3000b720:	f1a1 0502 	sub.w	r5, r1, #2
3000b724:	2909      	cmp	r1, #9
3000b726:	fab5 f585 	clz	r5, r5
3000b72a:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b72e:	f000 8099 	beq.w	3000b864 <rxi316_dram_init+0x9ec>
3000b732:	462a      	mov	r2, r5
3000b734:	e4d1      	b.n	3000b0da <rxi316_dram_init+0x262>
3000b736:	2000      	movs	r0, #0
3000b738:	e7e8      	b.n	3000b70c <rxi316_dram_init+0x894>
3000b73a:	f242 7710 	movw	r7, #10000	; 0x2710
3000b73e:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b818 <rxi316_dram_init+0x9a0>
3000b742:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b810 <rxi316_dram_init+0x998>
3000b746:	fbb7 fcf3 	udiv	ip, r7, r3
3000b74a:	9f05      	ldr	r7, [sp, #20]
3000b74c:	4467      	add	r7, ip
3000b74e:	fbb9 f9f3 	udiv	r9, r9, r3
3000b752:	fbbe fef3 	udiv	lr, lr, r3
3000b756:	09ff      	lsrs	r7, r7, #7
3000b758:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b75c:	3701      	adds	r7, #1
3000b75e:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b762:	f109 0c01 	add.w	ip, r9, #1
3000b766:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b814 <rxi316_dram_init+0x99c>
3000b76a:	023f      	lsls	r7, r7, #8
3000b76c:	f10e 0e01 	add.w	lr, lr, #1
3000b770:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b774:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b778:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b77c:	9709      	str	r7, [sp, #36]	; 0x24
3000b77e:	e62f      	b.n	3000b3e0 <rxi316_dram_init+0x568>
3000b780:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b784:	e5d1      	b.n	3000b32a <rxi316_dram_init+0x4b2>
3000b786:	2100      	movs	r1, #0
3000b788:	460d      	mov	r5, r1
3000b78a:	e4b0      	b.n	3000b0ee <rxi316_dram_init+0x276>
3000b78c:	3a07      	subs	r2, #7
3000b78e:	f7ff bbb8 	b.w	3000af02 <rxi316_dram_init+0x8a>
3000b792:	686a      	ldr	r2, [r5, #4]
3000b794:	f042 0220 	orr.w	r2, r2, #32
3000b798:	606a      	str	r2, [r5, #4]
3000b79a:	e413      	b.n	3000afc4 <rxi316_dram_init+0x14c>
3000b79c:	2204      	movs	r2, #4
3000b79e:	2102      	movs	r1, #2
3000b7a0:	2500      	movs	r5, #0
3000b7a2:	9201      	str	r2, [sp, #4]
3000b7a4:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b7a8:	f7ff bb94 	b.w	3000aed4 <rxi316_dram_init+0x5c>
3000b7ac:	3a08      	subs	r2, #8
3000b7ae:	f7ff bba8 	b.w	3000af02 <rxi316_dram_init+0x8a>
3000b7b2:	46b8      	mov	r8, r7
3000b7b4:	e603      	b.n	3000b3be <rxi316_dram_init+0x546>
3000b7b6:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b7b8:	2a04      	cmp	r2, #4
3000b7ba:	d90c      	bls.n	3000b7d6 <rxi316_dram_init+0x95e>
3000b7bc:	2a0b      	cmp	r2, #11
3000b7be:	d86a      	bhi.n	3000b896 <rxi316_dram_init+0xa1e>
3000b7c0:	1f17      	subs	r7, r2, #4
3000b7c2:	013f      	lsls	r7, r7, #4
3000b7c4:	e008      	b.n	3000b7d8 <rxi316_dram_init+0x960>
3000b7c6:	f04f 0802 	mov.w	r8, #2
3000b7ca:	e5f8      	b.n	3000b3be <rxi316_dram_init+0x546>
3000b7cc:	2903      	cmp	r1, #3
3000b7ce:	d005      	beq.n	3000b7dc <rxi316_dram_init+0x964>
3000b7d0:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b7d4:	e48b      	b.n	3000b0ee <rxi316_dram_init+0x276>
3000b7d6:	2710      	movs	r7, #16
3000b7d8:	68a2      	ldr	r2, [r4, #8]
3000b7da:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b7dc:	2a04      	cmp	r2, #4
3000b7de:	d949      	bls.n	3000b874 <rxi316_dram_init+0x9fc>
3000b7e0:	1f51      	subs	r1, r2, #5
3000b7e2:	2903      	cmp	r1, #3
3000b7e4:	d84e      	bhi.n	3000b884 <rxi316_dram_init+0xa0c>
3000b7e6:	1f15      	subs	r5, r2, #4
3000b7e8:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b7ec:	026d      	lsls	r5, r5, #9
3000b7ee:	e47e      	b.n	3000b0ee <rxi316_dram_init+0x276>
3000b7f0:	42008000 	.word	0x42008000
3000b7f4:	41100000 	.word	0x41100000
3000b7f8:	41011000 	.word	0x41011000
3000b7fc:	44000103 	.word	0x44000103
3000b800:	44000021 	.word	0x44000021
3000b804:	fff8fffc 	.word	0xfff8fffc
3000b808:	00009b2d 	.word	0x00009b2d
3000b80c:	00061a80 	.word	0x00061a80
3000b810:	0bebc200 	.word	0x0bebc200
3000b814:	003ff000 	.word	0x003ff000
3000b818:	1dcd6500 	.word	0x1dcd6500
3000b81c:	f1a1 0c02 	sub.w	ip, r1, #2
3000b820:	f1bc 0f01 	cmp.w	ip, #1
3000b824:	d903      	bls.n	3000b82e <rxi316_dram_init+0x9b6>
3000b826:	f1be 0f00 	cmp.w	lr, #0
3000b82a:	f43f ac56 	beq.w	3000b0da <rxi316_dram_init+0x262>
3000b82e:	f040 0002 	orr.w	r0, r0, #2
3000b832:	e452      	b.n	3000b0da <rxi316_dram_init+0x262>
3000b834:	4f1d      	ldr	r7, [pc, #116]	; (3000b8ac <rxi316_dram_init+0xa34>)
3000b836:	f04f 0e00 	mov.w	lr, #0
3000b83a:	fbb7 f9f3 	udiv	r9, r7, r3
3000b83e:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b842:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b846:	f109 0701 	add.w	r7, r9, #1
3000b84a:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b8b0 <rxi316_dram_init+0xa38>
3000b84e:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b852:	e5c5      	b.n	3000b3e0 <rxi316_dram_init+0x568>
3000b854:	f105 0802 	add.w	r8, r5, #2
3000b858:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b85a:	eba8 0805 	sub.w	r8, r8, r5
3000b85e:	9d08      	ldr	r5, [sp, #32]
3000b860:	44a8      	add	r8, r5
3000b862:	e562      	b.n	3000b32a <rxi316_dram_init+0x4b2>
3000b864:	2204      	movs	r2, #4
3000b866:	e6d2      	b.n	3000b60e <rxi316_dram_init+0x796>
3000b868:	686a      	ldr	r2, [r5, #4]
3000b86a:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b86e:	606a      	str	r2, [r5, #4]
3000b870:	f7ff bba8 	b.w	3000afc4 <rxi316_dram_init+0x14c>
3000b874:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b878:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b87c:	e437      	b.n	3000b0ee <rxi316_dram_init+0x276>
3000b87e:	f04f 0802 	mov.w	r8, #2
3000b882:	e596      	b.n	3000b3b2 <rxi316_dram_init+0x53a>
3000b884:	f1a2 0109 	sub.w	r1, r2, #9
3000b888:	2905      	cmp	r1, #5
3000b88a:	d80b      	bhi.n	3000b8a4 <rxi316_dram_init+0xa2c>
3000b88c:	0855      	lsrs	r5, r2, #1
3000b88e:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b892:	026d      	lsls	r5, r5, #9
3000b894:	e42b      	b.n	3000b0ee <rxi316_dram_init+0x276>
3000b896:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b898:	f040 0004 	orr.w	r0, r0, #4
3000b89c:	f1a2 070c 	sub.w	r7, r2, #12
3000b8a0:	013f      	lsls	r7, r7, #4
3000b8a2:	e799      	b.n	3000b7d8 <rxi316_dram_init+0x960>
3000b8a4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b8a8:	2500      	movs	r5, #0
3000b8aa:	e420      	b.n	3000b0ee <rxi316_dram_init+0x276>
3000b8ac:	0bebc200 	.word	0x0bebc200
3000b8b0:	003ff000 	.word	0x003ff000

3000b8b4 <rxi316_ftend_init>:
3000b8b4:	2300      	movs	r3, #0
3000b8b6:	b510      	push	{r4, lr}
3000b8b8:	4c0c      	ldr	r4, [pc, #48]	; (3000b8ec <rxi316_ftend_init+0x38>)
3000b8ba:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b8be:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b8c2:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b8c6:	f7fe fe51 	bl	3000a56c <ddr_init_index>
3000b8ca:	4a09      	ldr	r2, [pc, #36]	; (3000b8f0 <rxi316_ftend_init+0x3c>)
3000b8cc:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b8d0:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b8d4:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b8d8:	68cb      	ldr	r3, [r1, #12]
3000b8da:	689b      	ldr	r3, [r3, #8]
3000b8dc:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b8de:	3301      	adds	r3, #1
3000b8e0:	b2db      	uxtb	r3, r3
3000b8e2:	4313      	orrs	r3, r2
3000b8e4:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b8e8:	bd10      	pop	{r4, pc}
3000b8ea:	bf00      	nop
3000b8ec:	41101000 	.word	0x41101000
3000b8f0:	3000e8bc 	.word	0x3000e8bc

3000b8f4 <rxi316_DynSre_init>:
3000b8f4:	2901      	cmp	r1, #1
3000b8f6:	d013      	beq.n	3000b920 <rxi316_DynSre_init+0x2c>
3000b8f8:	4a12      	ldr	r2, [pc, #72]	; (3000b944 <rxi316_DynSre_init+0x50>)
3000b8fa:	6893      	ldr	r3, [r2, #8]
3000b8fc:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b900:	6093      	str	r3, [r2, #8]
3000b902:	4b10      	ldr	r3, [pc, #64]	; (3000b944 <rxi316_DynSre_init+0x50>)
3000b904:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b908:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b90c:	625a      	str	r2, [r3, #36]	; 0x24
3000b90e:	629a      	str	r2, [r3, #40]	; 0x28
3000b910:	69da      	ldr	r2, [r3, #28]
3000b912:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b916:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b91a:	61da      	str	r2, [r3, #28]
3000b91c:	6019      	str	r1, [r3, #0]
3000b91e:	4770      	bx	lr
3000b920:	4b08      	ldr	r3, [pc, #32]	; (3000b944 <rxi316_DynSre_init+0x50>)
3000b922:	b280      	uxth	r0, r0
3000b924:	4a08      	ldr	r2, [pc, #32]	; (3000b948 <rxi316_DynSre_init+0x54>)
3000b926:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b92a:	400a      	ands	r2, r1
3000b92c:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b930:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b934:	4310      	orrs	r0, r2
3000b936:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b93a:	689a      	ldr	r2, [r3, #8]
3000b93c:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b940:	609a      	str	r2, [r3, #8]
3000b942:	e7de      	b.n	3000b902 <rxi316_DynSre_init+0xe>
3000b944:	41100000 	.word	0x41100000
3000b948:	ffff0000 	.word	0xffff0000

3000b94c <ddr_init>:
3000b94c:	b500      	push	{lr}
3000b94e:	b083      	sub	sp, #12
3000b950:	f7fe fe0c 	bl	3000a56c <ddr_init_index>
3000b954:	4b06      	ldr	r3, [pc, #24]	; (3000b970 <ddr_init+0x24>)
3000b956:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b95a:	9001      	str	r0, [sp, #4]
3000b95c:	f7fe fe30 	bl	3000a5c0 <rxi316_perf_tune>
3000b960:	9801      	ldr	r0, [sp, #4]
3000b962:	f7ff fa89 	bl	3000ae78 <rxi316_dram_init>
3000b966:	b003      	add	sp, #12
3000b968:	f85d eb04 	ldr.w	lr, [sp], #4
3000b96c:	f7ff bfa2 	b.w	3000b8b4 <rxi316_ftend_init>
3000b970:	3000e8bc 	.word	0x3000e8bc

3000b974 <SWR_MEM>:
3000b974:	2801      	cmp	r0, #1
3000b976:	d00e      	beq.n	3000b996 <SWR_MEM+0x22>
3000b978:	4b31      	ldr	r3, [pc, #196]	; (3000ba40 <SWR_MEM+0xcc>)
3000b97a:	2000      	movs	r0, #0
3000b97c:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b980:	f022 0201 	bic.w	r2, r2, #1
3000b984:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b988:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b98c:	f022 0202 	bic.w	r2, r2, #2
3000b990:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b994:	4770      	bx	lr
3000b996:	b510      	push	{r4, lr}
3000b998:	4c29      	ldr	r4, [pc, #164]	; (3000ba40 <SWR_MEM+0xcc>)
3000b99a:	4a2a      	ldr	r2, [pc, #168]	; (3000ba44 <SWR_MEM+0xd0>)
3000b99c:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b9a0:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b9a4:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b9a8:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b9ac:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b9b0:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b9b4:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b9b8:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b9bc:	f023 0303 	bic.w	r3, r3, #3
3000b9c0:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b9c4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b9c8:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b9cc:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b9d0:	4790      	blx	r2
3000b9d2:	2803      	cmp	r0, #3
3000b9d4:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b9d8:	d026      	beq.n	3000ba28 <SWR_MEM+0xb4>
3000b9da:	f043 0303 	orr.w	r3, r3, #3
3000b9de:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b9e2:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b9e6:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b9ea:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b9ee:	4c14      	ldr	r4, [pc, #80]	; (3000ba40 <SWR_MEM+0xcc>)
3000b9f0:	4b15      	ldr	r3, [pc, #84]	; (3000ba48 <SWR_MEM+0xd4>)
3000b9f2:	4798      	blx	r3
3000b9f4:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b9f8:	4a14      	ldr	r2, [pc, #80]	; (3000ba4c <SWR_MEM+0xd8>)
3000b9fa:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b9fe:	f043 0302 	orr.w	r3, r3, #2
3000ba02:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000ba06:	4790      	blx	r2
3000ba08:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ba0c:	4b10      	ldr	r3, [pc, #64]	; (3000ba50 <SWR_MEM+0xdc>)
3000ba0e:	f042 0201 	orr.w	r2, r2, #1
3000ba12:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000ba16:	e001      	b.n	3000ba1c <SWR_MEM+0xa8>
3000ba18:	3b01      	subs	r3, #1
3000ba1a:	d00e      	beq.n	3000ba3a <SWR_MEM+0xc6>
3000ba1c:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ba20:	03d2      	lsls	r2, r2, #15
3000ba22:	d5f9      	bpl.n	3000ba18 <SWR_MEM+0xa4>
3000ba24:	2000      	movs	r0, #0
3000ba26:	bd10      	pop	{r4, pc}
3000ba28:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ba2c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ba30:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000ba34:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ba38:	e7d9      	b.n	3000b9ee <SWR_MEM+0x7a>
3000ba3a:	2001      	movs	r0, #1
3000ba3c:	bd10      	pop	{r4, pc}
3000ba3e:	bf00      	nop
3000ba40:	42008800 	.word	0x42008800
3000ba44:	30007f19 	.word	0x30007f19
3000ba48:	3000bc15 	.word	0x3000bc15
3000ba4c:	00009b2d 	.word	0x00009b2d
3000ba50:	000186a0 	.word	0x000186a0

3000ba54 <SWR_MEM_Manual>:
3000ba54:	4a06      	ldr	r2, [pc, #24]	; (3000ba70 <SWR_MEM_Manual+0x1c>)
3000ba56:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000ba5a:	b120      	cbz	r0, 3000ba66 <SWR_MEM_Manual+0x12>
3000ba5c:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000ba60:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000ba64:	4770      	bx	lr
3000ba66:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000ba6a:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000ba6e:	4770      	bx	lr
3000ba70:	42008800 	.word	0x42008800

3000ba74 <SWR_MEM_Mode_Set>:
3000ba74:	2801      	cmp	r0, #1
3000ba76:	b538      	push	{r3, r4, r5, lr}
3000ba78:	d807      	bhi.n	3000ba8a <SWR_MEM_Mode_Set+0x16>
3000ba7a:	d10a      	bne.n	3000ba92 <SWR_MEM_Mode_Set+0x1e>
3000ba7c:	4c22      	ldr	r4, [pc, #136]	; (3000bb08 <SWR_MEM_Mode_Set+0x94>)
3000ba7e:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000ba82:	035b      	lsls	r3, r3, #13
3000ba84:	d51b      	bpl.n	3000babe <SWR_MEM_Mode_Set+0x4a>
3000ba86:	2000      	movs	r0, #0
3000ba88:	bd38      	pop	{r3, r4, r5, pc}
3000ba8a:	217d      	movs	r1, #125	; 0x7d
3000ba8c:	481f      	ldr	r0, [pc, #124]	; (3000bb0c <SWR_MEM_Mode_Set+0x98>)
3000ba8e:	f000 fbb3 	bl	3000c1f8 <__io_assert_failed_veneer>
3000ba92:	491d      	ldr	r1, [pc, #116]	; (3000bb08 <SWR_MEM_Mode_Set+0x94>)
3000ba94:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000ba98:	0358      	lsls	r0, r3, #13
3000ba9a:	d5f4      	bpl.n	3000ba86 <SWR_MEM_Mode_Set+0x12>
3000ba9c:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000baa0:	f242 7310 	movw	r3, #10000	; 0x2710
3000baa4:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000baa8:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000baac:	e001      	b.n	3000bab2 <SWR_MEM_Mode_Set+0x3e>
3000baae:	3b01      	subs	r3, #1
3000bab0:	d027      	beq.n	3000bb02 <SWR_MEM_Mode_Set+0x8e>
3000bab2:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000bab6:	0352      	lsls	r2, r2, #13
3000bab8:	d4f9      	bmi.n	3000baae <SWR_MEM_Mode_Set+0x3a>
3000baba:	2000      	movs	r0, #0
3000babc:	e7e4      	b.n	3000ba88 <SWR_MEM_Mode_Set+0x14>
3000babe:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000bac2:	2064      	movs	r0, #100	; 0x64
3000bac4:	4d12      	ldr	r5, [pc, #72]	; (3000bb10 <SWR_MEM_Mode_Set+0x9c>)
3000bac6:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000baca:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000bace:	47a8      	blx	r5
3000bad0:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000bad4:	2064      	movs	r0, #100	; 0x64
3000bad6:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000bada:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000bade:	47a8      	blx	r5
3000bae0:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000bae4:	f242 7310 	movw	r3, #10000	; 0x2710
3000bae8:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000baec:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000baf0:	e001      	b.n	3000baf6 <SWR_MEM_Mode_Set+0x82>
3000baf2:	3b01      	subs	r3, #1
3000baf4:	d005      	beq.n	3000bb02 <SWR_MEM_Mode_Set+0x8e>
3000baf6:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000bafa:	0355      	lsls	r5, r2, #13
3000bafc:	d5f9      	bpl.n	3000baf2 <SWR_MEM_Mode_Set+0x7e>
3000bafe:	2000      	movs	r0, #0
3000bb00:	e7c2      	b.n	3000ba88 <SWR_MEM_Mode_Set+0x14>
3000bb02:	2001      	movs	r0, #1
3000bb04:	bd38      	pop	{r3, r4, r5, pc}
3000bb06:	bf00      	nop
3000bb08:	42008800 	.word	0x42008800
3000bb0c:	3000d6dc 	.word	0x3000d6dc
3000bb10:	00009b2d 	.word	0x00009b2d

3000bb14 <SWR_AUDIO>:
3000bb14:	2801      	cmp	r0, #1
3000bb16:	d00e      	beq.n	3000bb36 <SWR_AUDIO+0x22>
3000bb18:	4b1e      	ldr	r3, [pc, #120]	; (3000bb94 <SWR_AUDIO+0x80>)
3000bb1a:	2000      	movs	r0, #0
3000bb1c:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000bb20:	f022 0201 	bic.w	r2, r2, #1
3000bb24:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000bb28:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000bb2c:	f022 0202 	bic.w	r2, r2, #2
3000bb30:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000bb34:	4770      	bx	lr
3000bb36:	b510      	push	{r4, lr}
3000bb38:	4c16      	ldr	r4, [pc, #88]	; (3000bb94 <SWR_AUDIO+0x80>)
3000bb3a:	4b17      	ldr	r3, [pc, #92]	; (3000bb98 <SWR_AUDIO+0x84>)
3000bb3c:	4798      	blx	r3
3000bb3e:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000bb42:	4a16      	ldr	r2, [pc, #88]	; (3000bb9c <SWR_AUDIO+0x88>)
3000bb44:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000bb48:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000bb4c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000bb50:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000bb54:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000bb58:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000bb5c:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000bb60:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000bb64:	f043 0302 	orr.w	r3, r3, #2
3000bb68:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000bb6c:	4790      	blx	r2
3000bb6e:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000bb72:	4b0b      	ldr	r3, [pc, #44]	; (3000bba0 <SWR_AUDIO+0x8c>)
3000bb74:	f042 0201 	orr.w	r2, r2, #1
3000bb78:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000bb7c:	e001      	b.n	3000bb82 <SWR_AUDIO+0x6e>
3000bb7e:	3b01      	subs	r3, #1
3000bb80:	d005      	beq.n	3000bb8e <SWR_AUDIO+0x7a>
3000bb82:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000bb86:	03d2      	lsls	r2, r2, #15
3000bb88:	d5f9      	bpl.n	3000bb7e <SWR_AUDIO+0x6a>
3000bb8a:	2000      	movs	r0, #0
3000bb8c:	bd10      	pop	{r4, pc}
3000bb8e:	2001      	movs	r0, #1
3000bb90:	bd10      	pop	{r4, pc}
3000bb92:	bf00      	nop
3000bb94:	42008800 	.word	0x42008800
3000bb98:	3000bd65 	.word	0x3000bd65
3000bb9c:	00009b2d 	.word	0x00009b2d
3000bba0:	000186a0 	.word	0x000186a0

3000bba4 <SWR_AUDIO_Manual>:
3000bba4:	4a06      	ldr	r2, [pc, #24]	; (3000bbc0 <SWR_AUDIO_Manual+0x1c>)
3000bba6:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000bbaa:	b120      	cbz	r0, 3000bbb6 <SWR_AUDIO_Manual+0x12>
3000bbac:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000bbb0:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000bbb4:	4770      	bx	lr
3000bbb6:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000bbba:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000bbbe:	4770      	bx	lr
3000bbc0:	42008800 	.word	0x42008800

3000bbc4 <SWR_BST_MODE_Set>:
3000bbc4:	4a06      	ldr	r2, [pc, #24]	; (3000bbe0 <SWR_BST_MODE_Set+0x1c>)
3000bbc6:	6813      	ldr	r3, [r2, #0]
3000bbc8:	b118      	cbz	r0, 3000bbd2 <SWR_BST_MODE_Set+0xe>
3000bbca:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000bbce:	6013      	str	r3, [r2, #0]
3000bbd0:	4770      	bx	lr
3000bbd2:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000bbd6:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000bbda:	6013      	str	r3, [r2, #0]
3000bbdc:	4770      	bx	lr
3000bbde:	bf00      	nop
3000bbe0:	42008100 	.word	0x42008100

3000bbe4 <SWR_Calib_DCore>:
3000bbe4:	4b09      	ldr	r3, [pc, #36]	; (3000bc0c <SWR_Calib_DCore+0x28>)
3000bbe6:	f240 3231 	movw	r2, #817	; 0x331
3000bbea:	4909      	ldr	r1, [pc, #36]	; (3000bc10 <SWR_Calib_DCore+0x2c>)
3000bbec:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000bbee:	4001      	ands	r1, r0
3000bbf0:	430a      	orrs	r2, r1
3000bbf2:	651a      	str	r2, [r3, #80]	; 0x50
3000bbf4:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000bbf6:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bbfa:	641a      	str	r2, [r3, #64]	; 0x40
3000bbfc:	685a      	ldr	r2, [r3, #4]
3000bbfe:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000bc02:	f042 0204 	orr.w	r2, r2, #4
3000bc06:	605a      	str	r2, [r3, #4]
3000bc08:	4770      	bx	lr
3000bc0a:	bf00      	nop
3000bc0c:	42008100 	.word	0x42008100
3000bc10:	fffffccc 	.word	0xfffffccc

3000bc14 <SWR_Calib_MEM>:
3000bc14:	4b4e      	ldr	r3, [pc, #312]	; (3000bd50 <SWR_Calib_MEM+0x13c>)
3000bc16:	f240 3231 	movw	r2, #817	; 0x331
3000bc1a:	494e      	ldr	r1, [pc, #312]	; (3000bd54 <SWR_Calib_MEM+0x140>)
3000bc1c:	2000      	movs	r0, #0
3000bc1e:	b510      	push	{r4, lr}
3000bc20:	f8d3 40e0 	ldr.w	r4, [r3, #224]	; 0xe0
3000bc24:	b082      	sub	sp, #8
3000bc26:	4021      	ands	r1, r4
3000bc28:	430a      	orrs	r2, r1
3000bc2a:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000bc2e:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000bc32:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000bc36:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bc3a:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000bc3e:	2307      	movs	r3, #7
3000bc40:	f88d 0007 	strb.w	r0, [sp, #7]
3000bc44:	eb0d 0203 	add.w	r2, sp, r3
3000bc48:	f7fd fbca 	bl	300093e0 <EFUSE_PMAP_READ8>
3000bc4c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bc50:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bc54:	2af0      	cmp	r2, #240	; 0xf0
3000bc56:	d15b      	bne.n	3000bd10 <SWR_Calib_MEM+0xfc>
3000bc58:	f003 030f 	and.w	r3, r3, #15
3000bc5c:	2b0f      	cmp	r3, #15
3000bc5e:	d053      	beq.n	3000bd08 <SWR_Calib_MEM+0xf4>
3000bc60:	2102      	movs	r1, #2
3000bc62:	4c3d      	ldr	r4, [pc, #244]	; (3000bd58 <SWR_Calib_MEM+0x144>)
3000bc64:	2307      	movs	r3, #7
3000bc66:	f10d 0206 	add.w	r2, sp, #6
3000bc6a:	2000      	movs	r0, #0
3000bc6c:	7021      	strb	r1, [r4, #0]
3000bc6e:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000bc72:	f7fd fbb5 	bl	300093e0 <EFUSE_PMAP_READ8>
3000bc76:	7822      	ldrb	r2, [r4, #0]
3000bc78:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000bc7c:	2a03      	cmp	r2, #3
3000bc7e:	d05d      	beq.n	3000bd3c <SWR_Calib_MEM+0x128>
3000bc80:	2a02      	cmp	r2, #2
3000bc82:	bf0c      	ite	eq
3000bc84:	f003 030f 	andeq.w	r3, r3, #15
3000bc88:	2300      	movne	r3, #0
3000bc8a:	4931      	ldr	r1, [pc, #196]	; (3000bd50 <SWR_Calib_MEM+0x13c>)
3000bc8c:	4833      	ldr	r0, [pc, #204]	; (3000bd5c <SWR_Calib_MEM+0x148>)
3000bc8e:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000bc92:	f022 020f 	bic.w	r2, r2, #15
3000bc96:	4313      	orrs	r3, r2
3000bc98:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000bc9c:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000bc9e:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000bca2:	2b07      	cmp	r3, #7
3000bca4:	d03d      	beq.n	3000bd22 <SWR_Calib_MEM+0x10e>
3000bca6:	2307      	movs	r3, #7
3000bca8:	2000      	movs	r0, #0
3000bcaa:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000bcae:	eb0d 0203 	add.w	r2, sp, r3
3000bcb2:	f88d 0007 	strb.w	r0, [sp, #7]
3000bcb6:	f7fd fb93 	bl	300093e0 <EFUSE_PMAP_READ8>
3000bcba:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bcbe:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bcc2:	2af0      	cmp	r2, #240	; 0xf0
3000bcc4:	d126      	bne.n	3000bd14 <SWR_Calib_MEM+0x100>
3000bcc6:	f003 030f 	and.w	r3, r3, #15
3000bcca:	2b0f      	cmp	r3, #15
3000bccc:	d024      	beq.n	3000bd18 <SWR_Calib_MEM+0x104>
3000bcce:	2002      	movs	r0, #2
3000bcd0:	2307      	movs	r3, #7
3000bcd2:	4c23      	ldr	r4, [pc, #140]	; (3000bd60 <SWR_Calib_MEM+0x14c>)
3000bcd4:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000bcd8:	eb0d 0203 	add.w	r2, sp, r3
3000bcdc:	7020      	strb	r0, [r4, #0]
3000bcde:	2000      	movs	r0, #0
3000bce0:	f7fd fb7e 	bl	300093e0 <EFUSE_PMAP_READ8>
3000bce4:	7822      	ldrb	r2, [r4, #0]
3000bce6:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bcea:	2a03      	cmp	r2, #3
3000bcec:	d028      	beq.n	3000bd40 <SWR_Calib_MEM+0x12c>
3000bcee:	2a02      	cmp	r2, #2
3000bcf0:	d029      	beq.n	3000bd46 <SWR_Calib_MEM+0x132>
3000bcf2:	2200      	movs	r2, #0
3000bcf4:	4916      	ldr	r1, [pc, #88]	; (3000bd50 <SWR_Calib_MEM+0x13c>)
3000bcf6:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000bcfa:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000bcfe:	4313      	orrs	r3, r2
3000bd00:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000bd04:	b002      	add	sp, #8
3000bd06:	bd10      	pop	{r4, pc}
3000bd08:	4b13      	ldr	r3, [pc, #76]	; (3000bd58 <SWR_Calib_MEM+0x144>)
3000bd0a:	2201      	movs	r2, #1
3000bd0c:	701a      	strb	r2, [r3, #0]
3000bd0e:	e7ca      	b.n	3000bca6 <SWR_Calib_MEM+0x92>
3000bd10:	2103      	movs	r1, #3
3000bd12:	e7a6      	b.n	3000bc62 <SWR_Calib_MEM+0x4e>
3000bd14:	2003      	movs	r0, #3
3000bd16:	e7db      	b.n	3000bcd0 <SWR_Calib_MEM+0xbc>
3000bd18:	4b11      	ldr	r3, [pc, #68]	; (3000bd60 <SWR_Calib_MEM+0x14c>)
3000bd1a:	2201      	movs	r2, #1
3000bd1c:	701a      	strb	r2, [r3, #0]
3000bd1e:	b002      	add	sp, #8
3000bd20:	bd10      	pop	{r4, pc}
3000bd22:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000bd24:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000bd28:	d1bd      	bne.n	3000bca6 <SWR_Calib_MEM+0x92>
3000bd2a:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000bd2e:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000bd32:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000bd36:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000bd3a:	e7b4      	b.n	3000bca6 <SWR_Calib_MEM+0x92>
3000bd3c:	091b      	lsrs	r3, r3, #4
3000bd3e:	e7a4      	b.n	3000bc8a <SWR_Calib_MEM+0x76>
3000bd40:	091b      	lsrs	r3, r3, #4
3000bd42:	059a      	lsls	r2, r3, #22
3000bd44:	e7d6      	b.n	3000bcf4 <SWR_Calib_MEM+0xe0>
3000bd46:	f003 030f 	and.w	r3, r3, #15
3000bd4a:	059a      	lsls	r2, r3, #22
3000bd4c:	e7d2      	b.n	3000bcf4 <SWR_Calib_MEM+0xe0>
3000bd4e:	bf00      	nop
3000bd50:	42008800 	.word	0x42008800
3000bd54:	fffffccc 	.word	0xfffffccc
3000bd58:	3000ea57 	.word	0x3000ea57
3000bd5c:	42008100 	.word	0x42008100
3000bd60:	3000ea56 	.word	0x3000ea56

3000bd64 <SWR_Calib_AUD>:
3000bd64:	4b44      	ldr	r3, [pc, #272]	; (3000be78 <SWR_Calib_AUD+0x114>)
3000bd66:	f240 3231 	movw	r2, #817	; 0x331
3000bd6a:	4944      	ldr	r1, [pc, #272]	; (3000be7c <SWR_Calib_AUD+0x118>)
3000bd6c:	2000      	movs	r0, #0
3000bd6e:	b510      	push	{r4, lr}
3000bd70:	f8d3 40a0 	ldr.w	r4, [r3, #160]	; 0xa0
3000bd74:	b082      	sub	sp, #8
3000bd76:	4021      	ands	r1, r4
3000bd78:	430a      	orrs	r2, r1
3000bd7a:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000bd7e:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000bd82:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000bd86:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bd8a:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000bd8e:	2307      	movs	r3, #7
3000bd90:	f88d 0007 	strb.w	r0, [sp, #7]
3000bd94:	eb0d 0203 	add.w	r2, sp, r3
3000bd98:	f7fd fb22 	bl	300093e0 <EFUSE_PMAP_READ8>
3000bd9c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bda0:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bda4:	2af0      	cmp	r2, #240	; 0xf0
3000bda6:	d155      	bne.n	3000be54 <SWR_Calib_AUD+0xf0>
3000bda8:	f003 030f 	and.w	r3, r3, #15
3000bdac:	2b0f      	cmp	r3, #15
3000bdae:	d04d      	beq.n	3000be4c <SWR_Calib_AUD+0xe8>
3000bdb0:	2102      	movs	r1, #2
3000bdb2:	4c33      	ldr	r4, [pc, #204]	; (3000be80 <SWR_Calib_AUD+0x11c>)
3000bdb4:	2307      	movs	r3, #7
3000bdb6:	f10d 0206 	add.w	r2, sp, #6
3000bdba:	2000      	movs	r0, #0
3000bdbc:	7021      	strb	r1, [r4, #0]
3000bdbe:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000bdc2:	f7fd fb0d 	bl	300093e0 <EFUSE_PMAP_READ8>
3000bdc6:	7822      	ldrb	r2, [r4, #0]
3000bdc8:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000bdcc:	2a03      	cmp	r2, #3
3000bdce:	d04a      	beq.n	3000be66 <SWR_Calib_AUD+0x102>
3000bdd0:	2a02      	cmp	r2, #2
3000bdd2:	bf0c      	ite	eq
3000bdd4:	f003 030f 	andeq.w	r3, r3, #15
3000bdd8:	2300      	movne	r3, #0
3000bdda:	4927      	ldr	r1, [pc, #156]	; (3000be78 <SWR_Calib_AUD+0x114>)
3000bddc:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000bde0:	f022 020f 	bic.w	r2, r2, #15
3000bde4:	4313      	orrs	r3, r2
3000bde6:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000bdea:	2307      	movs	r3, #7
3000bdec:	2000      	movs	r0, #0
3000bdee:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000bdf2:	eb0d 0203 	add.w	r2, sp, r3
3000bdf6:	f88d 0007 	strb.w	r0, [sp, #7]
3000bdfa:	f7fd faf1 	bl	300093e0 <EFUSE_PMAP_READ8>
3000bdfe:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000be02:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000be06:	2af0      	cmp	r2, #240	; 0xf0
3000be08:	d126      	bne.n	3000be58 <SWR_Calib_AUD+0xf4>
3000be0a:	f003 030f 	and.w	r3, r3, #15
3000be0e:	2b0f      	cmp	r3, #15
3000be10:	d024      	beq.n	3000be5c <SWR_Calib_AUD+0xf8>
3000be12:	2002      	movs	r0, #2
3000be14:	2307      	movs	r3, #7
3000be16:	4c1b      	ldr	r4, [pc, #108]	; (3000be84 <SWR_Calib_AUD+0x120>)
3000be18:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000be1c:	eb0d 0203 	add.w	r2, sp, r3
3000be20:	7020      	strb	r0, [r4, #0]
3000be22:	2000      	movs	r0, #0
3000be24:	f7fd fadc 	bl	300093e0 <EFUSE_PMAP_READ8>
3000be28:	7822      	ldrb	r2, [r4, #0]
3000be2a:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000be2e:	2a03      	cmp	r2, #3
3000be30:	d01b      	beq.n	3000be6a <SWR_Calib_AUD+0x106>
3000be32:	2a02      	cmp	r2, #2
3000be34:	d01c      	beq.n	3000be70 <SWR_Calib_AUD+0x10c>
3000be36:	2200      	movs	r2, #0
3000be38:	490f      	ldr	r1, [pc, #60]	; (3000be78 <SWR_Calib_AUD+0x114>)
3000be3a:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000be3e:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000be42:	4313      	orrs	r3, r2
3000be44:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000be48:	b002      	add	sp, #8
3000be4a:	bd10      	pop	{r4, pc}
3000be4c:	4b0c      	ldr	r3, [pc, #48]	; (3000be80 <SWR_Calib_AUD+0x11c>)
3000be4e:	2201      	movs	r2, #1
3000be50:	701a      	strb	r2, [r3, #0]
3000be52:	e7ca      	b.n	3000bdea <SWR_Calib_AUD+0x86>
3000be54:	2103      	movs	r1, #3
3000be56:	e7ac      	b.n	3000bdb2 <SWR_Calib_AUD+0x4e>
3000be58:	2003      	movs	r0, #3
3000be5a:	e7db      	b.n	3000be14 <SWR_Calib_AUD+0xb0>
3000be5c:	4b09      	ldr	r3, [pc, #36]	; (3000be84 <SWR_Calib_AUD+0x120>)
3000be5e:	2201      	movs	r2, #1
3000be60:	701a      	strb	r2, [r3, #0]
3000be62:	b002      	add	sp, #8
3000be64:	bd10      	pop	{r4, pc}
3000be66:	091b      	lsrs	r3, r3, #4
3000be68:	e7b7      	b.n	3000bdda <SWR_Calib_AUD+0x76>
3000be6a:	091b      	lsrs	r3, r3, #4
3000be6c:	059a      	lsls	r2, r3, #22
3000be6e:	e7e3      	b.n	3000be38 <SWR_Calib_AUD+0xd4>
3000be70:	f003 030f 	and.w	r3, r3, #15
3000be74:	059a      	lsls	r2, r3, #22
3000be76:	e7df      	b.n	3000be38 <SWR_Calib_AUD+0xd4>
3000be78:	42008800 	.word	0x42008800
3000be7c:	fffffccc 	.word	0xfffffccc
3000be80:	3000ea55 	.word	0x3000ea55
3000be84:	3000ea54 	.word	0x3000ea54

3000be88 <__aeabi_uldivmod>:
3000be88:	b953      	cbnz	r3, 3000bea0 <__aeabi_uldivmod+0x18>
3000be8a:	b94a      	cbnz	r2, 3000bea0 <__aeabi_uldivmod+0x18>
3000be8c:	2900      	cmp	r1, #0
3000be8e:	bf08      	it	eq
3000be90:	2800      	cmpeq	r0, #0
3000be92:	bf1c      	itt	ne
3000be94:	f04f 31ff 	movne.w	r1, #4294967295
3000be98:	f04f 30ff 	movne.w	r0, #4294967295
3000be9c:	f000 b960 	b.w	3000c160 <__aeabi_idiv0>
3000bea0:	f1ad 0c08 	sub.w	ip, sp, #8
3000bea4:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000bea8:	f000 f806 	bl	3000beb8 <__udivmoddi4>
3000beac:	f8dd e004 	ldr.w	lr, [sp, #4]
3000beb0:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000beb4:	b004      	add	sp, #16
3000beb6:	4770      	bx	lr

3000beb8 <__udivmoddi4>:
3000beb8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000bebc:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000bebe:	4688      	mov	r8, r1
3000bec0:	468e      	mov	lr, r1
3000bec2:	4615      	mov	r5, r2
3000bec4:	4604      	mov	r4, r0
3000bec6:	4619      	mov	r1, r3
3000bec8:	2b00      	cmp	r3, #0
3000beca:	f040 80c6 	bne.w	3000c05a <__udivmoddi4+0x1a2>
3000bece:	4542      	cmp	r2, r8
3000bed0:	fab2 f782 	clz	r7, r2
3000bed4:	d946      	bls.n	3000bf64 <__udivmoddi4+0xac>
3000bed6:	b14f      	cbz	r7, 3000beec <__udivmoddi4+0x34>
3000bed8:	f1c7 0c20 	rsb	ip, r7, #32
3000bedc:	fa08 f307 	lsl.w	r3, r8, r7
3000bee0:	40bd      	lsls	r5, r7
3000bee2:	40bc      	lsls	r4, r7
3000bee4:	fa20 fc0c 	lsr.w	ip, r0, ip
3000bee8:	ea4c 0e03 	orr.w	lr, ip, r3
3000beec:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000bef0:	fa1f fc85 	uxth.w	ip, r5
3000bef4:	0c22      	lsrs	r2, r4, #16
3000bef6:	fbbe f9f8 	udiv	r9, lr, r8
3000befa:	fb08 e319 	mls	r3, r8, r9, lr
3000befe:	fb09 fa0c 	mul.w	sl, r9, ip
3000bf02:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000bf06:	459a      	cmp	sl, r3
3000bf08:	d928      	bls.n	3000bf5c <__udivmoddi4+0xa4>
3000bf0a:	18eb      	adds	r3, r5, r3
3000bf0c:	f109 30ff 	add.w	r0, r9, #4294967295
3000bf10:	d204      	bcs.n	3000bf1c <__udivmoddi4+0x64>
3000bf12:	459a      	cmp	sl, r3
3000bf14:	d902      	bls.n	3000bf1c <__udivmoddi4+0x64>
3000bf16:	f1a9 0002 	sub.w	r0, r9, #2
3000bf1a:	442b      	add	r3, r5
3000bf1c:	eba3 030a 	sub.w	r3, r3, sl
3000bf20:	b2a4      	uxth	r4, r4
3000bf22:	fbb3 f2f8 	udiv	r2, r3, r8
3000bf26:	fb08 3312 	mls	r3, r8, r2, r3
3000bf2a:	fb02 fc0c 	mul.w	ip, r2, ip
3000bf2e:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bf32:	45a4      	cmp	ip, r4
3000bf34:	d914      	bls.n	3000bf60 <__udivmoddi4+0xa8>
3000bf36:	192c      	adds	r4, r5, r4
3000bf38:	f102 33ff 	add.w	r3, r2, #4294967295
3000bf3c:	d203      	bcs.n	3000bf46 <__udivmoddi4+0x8e>
3000bf3e:	45a4      	cmp	ip, r4
3000bf40:	d901      	bls.n	3000bf46 <__udivmoddi4+0x8e>
3000bf42:	1e93      	subs	r3, r2, #2
3000bf44:	442c      	add	r4, r5
3000bf46:	eba4 040c 	sub.w	r4, r4, ip
3000bf4a:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000bf4e:	b11e      	cbz	r6, 3000bf58 <__udivmoddi4+0xa0>
3000bf50:	40fc      	lsrs	r4, r7
3000bf52:	2300      	movs	r3, #0
3000bf54:	6034      	str	r4, [r6, #0]
3000bf56:	6073      	str	r3, [r6, #4]
3000bf58:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bf5c:	4648      	mov	r0, r9
3000bf5e:	e7dd      	b.n	3000bf1c <__udivmoddi4+0x64>
3000bf60:	4613      	mov	r3, r2
3000bf62:	e7f0      	b.n	3000bf46 <__udivmoddi4+0x8e>
3000bf64:	b902      	cbnz	r2, 3000bf68 <__udivmoddi4+0xb0>
3000bf66:	deff      	udf	#255	; 0xff
3000bf68:	bb8f      	cbnz	r7, 3000bfce <__udivmoddi4+0x116>
3000bf6a:	eba8 0302 	sub.w	r3, r8, r2
3000bf6e:	2101      	movs	r1, #1
3000bf70:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000bf74:	b2aa      	uxth	r2, r5
3000bf76:	0c20      	lsrs	r0, r4, #16
3000bf78:	fbb3 fcfe 	udiv	ip, r3, lr
3000bf7c:	fb0e 331c 	mls	r3, lr, ip, r3
3000bf80:	fb0c f802 	mul.w	r8, ip, r2
3000bf84:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000bf88:	4598      	cmp	r8, r3
3000bf8a:	d962      	bls.n	3000c052 <__udivmoddi4+0x19a>
3000bf8c:	18eb      	adds	r3, r5, r3
3000bf8e:	f10c 30ff 	add.w	r0, ip, #4294967295
3000bf92:	d204      	bcs.n	3000bf9e <__udivmoddi4+0xe6>
3000bf94:	4598      	cmp	r8, r3
3000bf96:	d902      	bls.n	3000bf9e <__udivmoddi4+0xe6>
3000bf98:	f1ac 0002 	sub.w	r0, ip, #2
3000bf9c:	442b      	add	r3, r5
3000bf9e:	eba3 0308 	sub.w	r3, r3, r8
3000bfa2:	b2a4      	uxth	r4, r4
3000bfa4:	fbb3 fcfe 	udiv	ip, r3, lr
3000bfa8:	fb0e 331c 	mls	r3, lr, ip, r3
3000bfac:	fb0c f202 	mul.w	r2, ip, r2
3000bfb0:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bfb4:	42a2      	cmp	r2, r4
3000bfb6:	d94e      	bls.n	3000c056 <__udivmoddi4+0x19e>
3000bfb8:	192c      	adds	r4, r5, r4
3000bfba:	f10c 33ff 	add.w	r3, ip, #4294967295
3000bfbe:	d204      	bcs.n	3000bfca <__udivmoddi4+0x112>
3000bfc0:	42a2      	cmp	r2, r4
3000bfc2:	d902      	bls.n	3000bfca <__udivmoddi4+0x112>
3000bfc4:	f1ac 0302 	sub.w	r3, ip, #2
3000bfc8:	442c      	add	r4, r5
3000bfca:	1aa4      	subs	r4, r4, r2
3000bfcc:	e7bd      	b.n	3000bf4a <__udivmoddi4+0x92>
3000bfce:	f1c7 0c20 	rsb	ip, r7, #32
3000bfd2:	40bd      	lsls	r5, r7
3000bfd4:	fa08 f307 	lsl.w	r3, r8, r7
3000bfd8:	40bc      	lsls	r4, r7
3000bfda:	fa28 f90c 	lsr.w	r9, r8, ip
3000bfde:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000bfe2:	fa20 fc0c 	lsr.w	ip, r0, ip
3000bfe6:	ea4c 0203 	orr.w	r2, ip, r3
3000bfea:	b2ab      	uxth	r3, r5
3000bfec:	fbb9 fcfe 	udiv	ip, r9, lr
3000bff0:	0c11      	lsrs	r1, r2, #16
3000bff2:	fb0e 901c 	mls	r0, lr, ip, r9
3000bff6:	fb0c f803 	mul.w	r8, ip, r3
3000bffa:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000bffe:	4580      	cmp	r8, r0
3000c000:	d923      	bls.n	3000c04a <__udivmoddi4+0x192>
3000c002:	1828      	adds	r0, r5, r0
3000c004:	f10c 31ff 	add.w	r1, ip, #4294967295
3000c008:	d204      	bcs.n	3000c014 <__udivmoddi4+0x15c>
3000c00a:	4580      	cmp	r8, r0
3000c00c:	d902      	bls.n	3000c014 <__udivmoddi4+0x15c>
3000c00e:	f1ac 0102 	sub.w	r1, ip, #2
3000c012:	4428      	add	r0, r5
3000c014:	eba0 0008 	sub.w	r0, r0, r8
3000c018:	b292      	uxth	r2, r2
3000c01a:	fbb0 fcfe 	udiv	ip, r0, lr
3000c01e:	fb0e 001c 	mls	r0, lr, ip, r0
3000c022:	fb0c f803 	mul.w	r8, ip, r3
3000c026:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c02a:	4598      	cmp	r8, r3
3000c02c:	d90f      	bls.n	3000c04e <__udivmoddi4+0x196>
3000c02e:	18eb      	adds	r3, r5, r3
3000c030:	f10c 32ff 	add.w	r2, ip, #4294967295
3000c034:	d204      	bcs.n	3000c040 <__udivmoddi4+0x188>
3000c036:	4598      	cmp	r8, r3
3000c038:	d902      	bls.n	3000c040 <__udivmoddi4+0x188>
3000c03a:	f1ac 0202 	sub.w	r2, ip, #2
3000c03e:	442b      	add	r3, r5
3000c040:	eba3 0308 	sub.w	r3, r3, r8
3000c044:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c048:	e792      	b.n	3000bf70 <__udivmoddi4+0xb8>
3000c04a:	4661      	mov	r1, ip
3000c04c:	e7e2      	b.n	3000c014 <__udivmoddi4+0x15c>
3000c04e:	4662      	mov	r2, ip
3000c050:	e7f6      	b.n	3000c040 <__udivmoddi4+0x188>
3000c052:	4660      	mov	r0, ip
3000c054:	e7a3      	b.n	3000bf9e <__udivmoddi4+0xe6>
3000c056:	4663      	mov	r3, ip
3000c058:	e7b7      	b.n	3000bfca <__udivmoddi4+0x112>
3000c05a:	4543      	cmp	r3, r8
3000c05c:	d905      	bls.n	3000c06a <__udivmoddi4+0x1b2>
3000c05e:	b10e      	cbz	r6, 3000c064 <__udivmoddi4+0x1ac>
3000c060:	e9c6 0800 	strd	r0, r8, [r6]
3000c064:	2100      	movs	r1, #0
3000c066:	4608      	mov	r0, r1
3000c068:	e776      	b.n	3000bf58 <__udivmoddi4+0xa0>
3000c06a:	fab3 fc83 	clz	ip, r3
3000c06e:	f1bc 0f00 	cmp.w	ip, #0
3000c072:	d110      	bne.n	3000c096 <__udivmoddi4+0x1de>
3000c074:	4543      	cmp	r3, r8
3000c076:	d301      	bcc.n	3000c07c <__udivmoddi4+0x1c4>
3000c078:	4282      	cmp	r2, r0
3000c07a:	d80a      	bhi.n	3000c092 <__udivmoddi4+0x1da>
3000c07c:	1a84      	subs	r4, r0, r2
3000c07e:	eb68 0303 	sbc.w	r3, r8, r3
3000c082:	2001      	movs	r0, #1
3000c084:	469e      	mov	lr, r3
3000c086:	2e00      	cmp	r6, #0
3000c088:	d068      	beq.n	3000c15c <__udivmoddi4+0x2a4>
3000c08a:	e9c6 4e00 	strd	r4, lr, [r6]
3000c08e:	2100      	movs	r1, #0
3000c090:	e762      	b.n	3000bf58 <__udivmoddi4+0xa0>
3000c092:	4660      	mov	r0, ip
3000c094:	e7f7      	b.n	3000c086 <__udivmoddi4+0x1ce>
3000c096:	f1cc 0e20 	rsb	lr, ip, #32
3000c09a:	fa03 f30c 	lsl.w	r3, r3, ip
3000c09e:	fa02 f50c 	lsl.w	r5, r2, ip
3000c0a2:	fa00 f70c 	lsl.w	r7, r0, ip
3000c0a6:	fa22 f40e 	lsr.w	r4, r2, lr
3000c0aa:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c0ae:	fa20 f10e 	lsr.w	r1, r0, lr
3000c0b2:	431c      	orrs	r4, r3
3000c0b4:	fa08 f30c 	lsl.w	r3, r8, ip
3000c0b8:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c0bc:	4319      	orrs	r1, r3
3000c0be:	b2a3      	uxth	r3, r4
3000c0c0:	0c0a      	lsrs	r2, r1, #16
3000c0c2:	fbbb f8f9 	udiv	r8, fp, r9
3000c0c6:	fb09 bb18 	mls	fp, r9, r8, fp
3000c0ca:	fb08 fa03 	mul.w	sl, r8, r3
3000c0ce:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c0d2:	4592      	cmp	sl, r2
3000c0d4:	d93e      	bls.n	3000c154 <__udivmoddi4+0x29c>
3000c0d6:	18a2      	adds	r2, r4, r2
3000c0d8:	f108 30ff 	add.w	r0, r8, #4294967295
3000c0dc:	d204      	bcs.n	3000c0e8 <__udivmoddi4+0x230>
3000c0de:	4592      	cmp	sl, r2
3000c0e0:	d902      	bls.n	3000c0e8 <__udivmoddi4+0x230>
3000c0e2:	f1a8 0002 	sub.w	r0, r8, #2
3000c0e6:	4422      	add	r2, r4
3000c0e8:	eba2 020a 	sub.w	r2, r2, sl
3000c0ec:	b289      	uxth	r1, r1
3000c0ee:	fbb2 f8f9 	udiv	r8, r2, r9
3000c0f2:	fb09 2218 	mls	r2, r9, r8, r2
3000c0f6:	fb08 f303 	mul.w	r3, r8, r3
3000c0fa:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c0fe:	4293      	cmp	r3, r2
3000c100:	d92a      	bls.n	3000c158 <__udivmoddi4+0x2a0>
3000c102:	18a2      	adds	r2, r4, r2
3000c104:	f108 31ff 	add.w	r1, r8, #4294967295
3000c108:	d204      	bcs.n	3000c114 <__udivmoddi4+0x25c>
3000c10a:	4293      	cmp	r3, r2
3000c10c:	d902      	bls.n	3000c114 <__udivmoddi4+0x25c>
3000c10e:	f1a8 0102 	sub.w	r1, r8, #2
3000c112:	4422      	add	r2, r4
3000c114:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c118:	1ad2      	subs	r2, r2, r3
3000c11a:	fba0 9805 	umull	r9, r8, r0, r5
3000c11e:	4542      	cmp	r2, r8
3000c120:	464b      	mov	r3, r9
3000c122:	4641      	mov	r1, r8
3000c124:	d302      	bcc.n	3000c12c <__udivmoddi4+0x274>
3000c126:	d106      	bne.n	3000c136 <__udivmoddi4+0x27e>
3000c128:	454f      	cmp	r7, r9
3000c12a:	d204      	bcs.n	3000c136 <__udivmoddi4+0x27e>
3000c12c:	3801      	subs	r0, #1
3000c12e:	ebb9 0305 	subs.w	r3, r9, r5
3000c132:	eb68 0104 	sbc.w	r1, r8, r4
3000c136:	b18e      	cbz	r6, 3000c15c <__udivmoddi4+0x2a4>
3000c138:	1afc      	subs	r4, r7, r3
3000c13a:	eb62 0701 	sbc.w	r7, r2, r1
3000c13e:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c142:	fa24 f30c 	lsr.w	r3, r4, ip
3000c146:	fa27 f70c 	lsr.w	r7, r7, ip
3000c14a:	ea4e 0303 	orr.w	r3, lr, r3
3000c14e:	e9c6 3700 	strd	r3, r7, [r6]
3000c152:	e79c      	b.n	3000c08e <__udivmoddi4+0x1d6>
3000c154:	4640      	mov	r0, r8
3000c156:	e7c7      	b.n	3000c0e8 <__udivmoddi4+0x230>
3000c158:	4641      	mov	r1, r8
3000c15a:	e7db      	b.n	3000c114 <__udivmoddi4+0x25c>
3000c15c:	4631      	mov	r1, r6
3000c15e:	e6fb      	b.n	3000bf58 <__udivmoddi4+0xa0>

3000c160 <__aeabi_idiv0>:
3000c160:	4770      	bx	lr
3000c162:	bf00      	nop
3000c164:	0000      	movs	r0, r0
	...

3000c168 <____wrap_memcpy_veneer>:
3000c168:	b401      	push	{r0}
3000c16a:	4802      	ldr	r0, [pc, #8]	; (3000c174 <____wrap_memcpy_veneer+0xc>)
3000c16c:	4684      	mov	ip, r0
3000c16e:	bc01      	pop	{r0}
3000c170:	4760      	bx	ip
3000c172:	bf00      	nop
3000c174:	00012f05 	.word	0x00012f05

3000c178 <____wrap_memset_veneer>:
3000c178:	b401      	push	{r0}
3000c17a:	4802      	ldr	r0, [pc, #8]	; (3000c184 <____wrap_memset_veneer+0xc>)
3000c17c:	4684      	mov	ip, r0
3000c17e:	bc01      	pop	{r0}
3000c180:	4760      	bx	ip
3000c182:	bf00      	nop
3000c184:	00012efd 	.word	0x00012efd

3000c188 <____wrap_memcmp_veneer>:
3000c188:	b401      	push	{r0}
3000c18a:	4802      	ldr	r0, [pc, #8]	; (3000c194 <____wrap_memcmp_veneer+0xc>)
3000c18c:	4684      	mov	ip, r0
3000c18e:	bc01      	pop	{r0}
3000c190:	4760      	bx	ip
3000c192:	bf00      	nop
3000c194:	00012f0d 	.word	0x00012f0d

3000c198 <____wrap_strlen_veneer>:
3000c198:	b401      	push	{r0}
3000c19a:	4802      	ldr	r0, [pc, #8]	; (3000c1a4 <____wrap_strlen_veneer+0xc>)
3000c19c:	4684      	mov	ip, r0
3000c19e:	bc01      	pop	{r0}
3000c1a0:	4760      	bx	ip
3000c1a2:	bf00      	nop
3000c1a4:	00012f25 	.word	0x00012f25

3000c1a8 <__SBOOT_Validate_ImgHash_veneer>:
3000c1a8:	b401      	push	{r0}
3000c1aa:	4802      	ldr	r0, [pc, #8]	; (3000c1b4 <__SBOOT_Validate_ImgHash_veneer+0xc>)
3000c1ac:	4684      	mov	ip, r0
3000c1ae:	bc01      	pop	{r0}
3000c1b0:	4760      	bx	ip
3000c1b2:	bf00      	nop
3000c1b4:	00003541 	.word	0x00003541

3000c1b8 <__DiagPrintf_veneer>:
3000c1b8:	b401      	push	{r0}
3000c1ba:	4802      	ldr	r0, [pc, #8]	; (3000c1c4 <__DiagPrintf_veneer+0xc>)
3000c1bc:	4684      	mov	ip, r0
3000c1be:	bc01      	pop	{r0}
3000c1c0:	4760      	bx	ip
3000c1c2:	bf00      	nop
3000c1c4:	0000e5f9 	.word	0x0000e5f9

3000c1c8 <__SBOOT_Validate_Signature_veneer>:
3000c1c8:	b401      	push	{r0}
3000c1ca:	4802      	ldr	r0, [pc, #8]	; (3000c1d4 <__SBOOT_Validate_Signature_veneer+0xc>)
3000c1cc:	4684      	mov	ip, r0
3000c1ce:	bc01      	pop	{r0}
3000c1d0:	4760      	bx	ip
3000c1d2:	bf00      	nop
3000c1d4:	00003431 	.word	0x00003431

3000c1d8 <__SBOOT_Validate_Algorithm_veneer>:
3000c1d8:	b401      	push	{r0}
3000c1da:	4802      	ldr	r0, [pc, #8]	; (3000c1e4 <__SBOOT_Validate_Algorithm_veneer+0xc>)
3000c1dc:	4684      	mov	ip, r0
3000c1de:	bc01      	pop	{r0}
3000c1e0:	4760      	bx	ip
3000c1e2:	bf00      	nop
3000c1e4:	000033ed 	.word	0x000033ed

3000c1e8 <__SBOOT_GetMdType_veneer>:
3000c1e8:	b401      	push	{r0}
3000c1ea:	4802      	ldr	r0, [pc, #8]	; (3000c1f4 <__SBOOT_GetMdType_veneer+0xc>)
3000c1ec:	4684      	mov	ip, r0
3000c1ee:	bc01      	pop	{r0}
3000c1f0:	4760      	bx	ip
3000c1f2:	bf00      	nop
3000c1f4:	0000319d 	.word	0x0000319d

3000c1f8 <__io_assert_failed_veneer>:
3000c1f8:	b401      	push	{r0}
3000c1fa:	4802      	ldr	r0, [pc, #8]	; (3000c204 <__io_assert_failed_veneer+0xc>)
3000c1fc:	4684      	mov	ip, r0
3000c1fe:	bc01      	pop	{r0}
3000c200:	4760      	bx	ip
3000c202:	bf00      	nop
3000c204:	0000992d 	.word	0x0000992d

3000c208 <__SBOOT_Validate_PubKey_veneer>:
3000c208:	b401      	push	{r0}
3000c20a:	4802      	ldr	r0, [pc, #8]	; (3000c214 <__SBOOT_Validate_PubKey_veneer+0xc>)
3000c20c:	4684      	mov	ip, r0
3000c20e:	bc01      	pop	{r0}
3000c210:	4760      	bx	ip
3000c212:	bf00      	nop
3000c214:	00003495 	.word	0x00003495
3000c218:	004f4931 	.word	0x004f4931
3000c21c:	00004f32 	.word	0x00004f32
3000c220:	004f4932 	.word	0x004f4932
3000c224:	00004f34 	.word	0x00004f34
3000c228:	004f4934 	.word	0x004f4934
3000c22c:	414c460d 	.word	0x414c460d
3000c230:	435f4853 	.word	0x435f4853
3000c234:	62696c61 	.word	0x62696c61
3000c238:	69746172 	.word	0x69746172
3000c23c:	654e6e6f 	.word	0x654e6e6f
3000c240:	78252077 	.word	0x78252077
3000c244:	2078253a 	.word	0x2078253a
3000c248:	73616870 	.word	0x73616870
3000c24c:	68735f65 	.word	0x68735f65
3000c250:	3a746669 	.word	0x3a746669
3000c254:	000a7825 	.word	0x000a7825
3000c258:	414c460d 	.word	0x414c460d
3000c25c:	435f4853 	.word	0x435f4853
3000c260:	62696c61 	.word	0x62696c61
3000c264:	69746172 	.word	0x69746172
3000c268:	654e6e6f 	.word	0x654e6e6f
3000c26c:	694c2077 	.word	0x694c2077
3000c270:	6544656e 	.word	0x6544656e
3000c274:	3a79616c 	.word	0x3a79616c
3000c278:	000a7825 	.word	0x000a7825
3000c27c:	414c460d 	.word	0x414c460d
3000c280:	435f4853 	.word	0x435f4853
3000c284:	62696c61 	.word	0x62696c61
3000c288:	69746172 	.word	0x69746172
3000c28c:	654e6e6f 	.word	0x654e6e6f
3000c290:	69772077 	.word	0x69772077
3000c294:	776f646e 	.word	0x776f646e
3000c298:	74735f31 	.word	0x74735f31
3000c29c:	3a747261 	.word	0x3a747261
3000c2a0:	77206425 	.word	0x77206425
3000c2a4:	6f646e69 	.word	0x6f646e69
3000c2a8:	735f3177 	.word	0x735f3177
3000c2ac:	3a657a69 	.word	0x3a657a69
3000c2b0:	0a206425 	.word	0x0a206425
3000c2b4:	00000000 	.word	0x00000000
3000c2b8:	414c460d 	.word	0x414c460d
3000c2bc:	435f4853 	.word	0x435f4853
3000c2c0:	62696c61 	.word	0x62696c61
3000c2c4:	69746172 	.word	0x69746172
3000c2c8:	654e6e6f 	.word	0x654e6e6f
3000c2cc:	68702077 	.word	0x68702077
3000c2d0:	5f657361 	.word	0x5f657361
3000c2d4:	66696873 	.word	0x66696873
3000c2d8:	64695f74 	.word	0x64695f74
3000c2dc:	64253a78 	.word	0x64253a78
3000c2e0:	00000a20 	.word	0x00000a20
3000c2e4:	414c460d 	.word	0x414c460d
3000c2e8:	435f4853 	.word	0x435f4853
3000c2ec:	62696c61 	.word	0x62696c61
3000c2f0:	69746172 	.word	0x69746172
3000c2f4:	25206e6f 	.word	0x25206e6f
3000c2f8:	61732078 	.word	0x61732078
3000c2fc:	656c706d 	.word	0x656c706d
3000c300:	6168705f 	.word	0x6168705f
3000c304:	253a6573 	.word	0x253a6573
3000c308:	61722078 	.word	0x61722078
3000c30c:	253a6574 	.word	0x253a6574
3000c310:	00000a78 	.word	0x00000a78
3000c314:	414c460d 	.word	0x414c460d
3000c318:	435f4853 	.word	0x435f4853
3000c31c:	62696c61 	.word	0x62696c61
3000c320:	69746172 	.word	0x69746172
3000c324:	73206e6f 	.word	0x73206e6f
3000c328:	6c706d61 	.word	0x6c706d61
3000c32c:	68705f65 	.word	0x68705f65
3000c330:	3a657361 	.word	0x3a657361
3000c334:	72207825 	.word	0x72207825
3000c338:	3a657461 	.word	0x3a657461
3000c33c:	000a7825 	.word	0x000a7825
3000c340:	444f4d5b 	.word	0x444f4d5b
3000c344:	5f454c55 	.word	0x5f454c55
3000c348:	544f4f42 	.word	0x544f4f42
3000c34c:	56454c2d 	.word	0x56454c2d
3000c350:	495f4c45 	.word	0x495f4c45
3000c354:	5d4f464e 	.word	0x5d4f464e
3000c358:	6c61633a 	.word	0x6c61633a
3000c35c:	61726269 	.word	0x61726269
3000c360:	6e6f6974 	.word	0x6e6f6974
3000c364:	3a6b6f5f 	.word	0x3a6b6f5f
3000c368:	3a64255b 	.word	0x3a64255b
3000c36c:	253a6425 	.word	0x253a6425
3000c370:	0a205d64 	.word	0x0a205d64
3000c374:	00000000 	.word	0x00000000
3000c378:	444f4d5b 	.word	0x444f4d5b
3000c37c:	5f454c55 	.word	0x5f454c55
3000c380:	544f4f42 	.word	0x544f4f42
3000c384:	56454c2d 	.word	0x56454c2d
3000c388:	495f4c45 	.word	0x495f4c45
3000c38c:	5d4f464e 	.word	0x5d4f464e
3000c390:	414c463a 	.word	0x414c463a
3000c394:	43204853 	.word	0x43204853
3000c398:	42494c41 	.word	0x42494c41
3000c39c:	2578305b 	.word	0x2578305b
3000c3a0:	4b4f2078 	.word	0x4b4f2078
3000c3a4:	00000a5d 	.word	0x00000a5d
3000c3a8:	444f4d5b 	.word	0x444f4d5b
3000c3ac:	5f454c55 	.word	0x5f454c55
3000c3b0:	544f4f42 	.word	0x544f4f42
3000c3b4:	56454c2d 	.word	0x56454c2d
3000c3b8:	495f4c45 	.word	0x495f4c45
3000c3bc:	5d4f464e 	.word	0x5d4f464e
3000c3c0:	414c463a 	.word	0x414c463a
3000c3c4:	43204853 	.word	0x43204853
3000c3c8:	42494c41 	.word	0x42494c41
3000c3cc:	2578305b 	.word	0x2578305b
3000c3d0:	41462078 	.word	0x41462078
3000c3d4:	0a5d4c49 	.word	0x0a5d4c49
3000c3d8:	00000000 	.word	0x00000000
3000c3dc:	444f4d5b 	.word	0x444f4d5b
3000c3e0:	5f454c55 	.word	0x5f454c55
3000c3e4:	544f4f42 	.word	0x544f4f42
3000c3e8:	56454c2d 	.word	0x56454c2d
3000c3ec:	495f4c45 	.word	0x495f4c45
3000c3f0:	5d4f464e 	.word	0x5d4f464e
3000c3f4:	616c463a 	.word	0x616c463a
3000c3f8:	52206873 	.word	0x52206873
3000c3fc:	20646165 	.word	0x20646165
3000c400:	000a7325 	.word	0x000a7325
3000c404:	444f4d5b 	.word	0x444f4d5b
3000c408:	5f454c55 	.word	0x5f454c55
3000c40c:	544f4f42 	.word	0x544f4f42
3000c410:	56454c2d 	.word	0x56454c2d
3000c414:	495f4c45 	.word	0x495f4c45
3000c418:	5d4f464e 	.word	0x5d4f464e
3000c41c:	616c463a 	.word	0x616c463a
3000c420:	52206873 	.word	0x52206873
3000c424:	20646165 	.word	0x20646165
3000c428:	202c7325 	.word	0x202c7325
3000c42c:	4c494146 	.word	0x4c494146
3000c430:	0000000a 	.word	0x0000000a
3000c434:	444f4d5b 	.word	0x444f4d5b
3000c438:	5f454c55 	.word	0x5f454c55
3000c43c:	544f4f42 	.word	0x544f4f42
3000c440:	56454c2d 	.word	0x56454c2d
3000c444:	455f4c45 	.word	0x455f4c45
3000c448:	524f5252 	.word	0x524f5252
3000c44c:	6c463a5d 	.word	0x6c463a5d
3000c450:	20687361 	.word	0x20687361
3000c454:	74697753 	.word	0x74697753
3000c458:	52206863 	.word	0x52206863
3000c45c:	20646165 	.word	0x20646165
3000c460:	65646f4d 	.word	0x65646f4d
3000c464:	49414620 	.word	0x49414620
3000c468:	00000a4c 	.word	0x00000a4c
3000c46c:	444f4d5b 	.word	0x444f4d5b
3000c470:	5f454c55 	.word	0x5f454c55
3000c474:	544f4f42 	.word	0x544f4f42
3000c478:	56454c2d 	.word	0x56454c2d
3000c47c:	495f4c45 	.word	0x495f4c45
3000c480:	5d4f464e 	.word	0x5d4f464e
3000c484:	616c463a 	.word	0x616c463a
3000c488:	49206873 	.word	0x49206873
3000c48c:	25203a44 	.word	0x25203a44
3000c490:	78252d78 	.word	0x78252d78
3000c494:	0a78252d 	.word	0x0a78252d
3000c498:	00000000 	.word	0x00000000
3000c49c:	6968540d 	.word	0x6968540d
3000c4a0:	6c662073 	.word	0x6c662073
3000c4a4:	20687361 	.word	0x20687361
3000c4a8:	65707974 	.word	0x65707974
3000c4ac:	20736920 	.word	0x20736920
3000c4b0:	20746f6e 	.word	0x20746f6e
3000c4b4:	70707573 	.word	0x70707573
3000c4b8:	6574726f 	.word	0x6574726f
3000c4bc:	000a2164 	.word	0x000a2164
3000c4c0:	444f4d5b 	.word	0x444f4d5b
3000c4c4:	5f454c55 	.word	0x5f454c55
3000c4c8:	544f4f42 	.word	0x544f4f42
3000c4cc:	56454c2d 	.word	0x56454c2d
3000c4d0:	495f4c45 	.word	0x495f4c45
3000c4d4:	5d4f464e 	.word	0x5d4f464e
3000c4d8:	4e414e3a 	.word	0x4e414e3a
3000c4dc:	44492044 	.word	0x44492044
3000c4e0:	7825203a 	.word	0x7825203a
3000c4e4:	0a78252d 	.word	0x0a78252d
3000c4e8:	00000000 	.word	0x00000000
3000c4ec:	20304d4b 	.word	0x20304d4b
3000c4f0:	20504958 	.word	0x20504958
3000c4f4:	00474d49 	.word	0x00474d49
3000c4f8:	20304d4b 	.word	0x20304d4b
3000c4fc:	4d415253 	.word	0x4d415253
3000c500:	00000000 	.word	0x00000000
3000c504:	20304d4b 	.word	0x20304d4b
3000c508:	4d415244 	.word	0x4d415244
3000c50c:	00000000 	.word	0x00000000
3000c510:	20344d4b 	.word	0x20344d4b
3000c514:	20504958 	.word	0x20504958
3000c518:	00474d49 	.word	0x00474d49
3000c51c:	20344d4b 	.word	0x20344d4b
3000c520:	4d415253 	.word	0x4d415253
3000c524:	00000000 	.word	0x00000000
3000c528:	20344d4b 	.word	0x20344d4b
3000c52c:	4d415244 	.word	0x4d415244
3000c530:	00000000 	.word	0x00000000
3000c534:	42205041 	.word	0x42205041
3000c538:	5320314c 	.word	0x5320314c
3000c53c:	004d4152 	.word	0x004d4152
3000c540:	42205041 	.word	0x42205041
3000c544:	4420314c 	.word	0x4420314c
3000c548:	004d4152 	.word	0x004d4152
3000c54c:	46205041 	.word	0x46205041
3000c550:	00005049 	.word	0x00005049
3000c554:	444f4d5b 	.word	0x444f4d5b
3000c558:	5f454c55 	.word	0x5f454c55
3000c55c:	544f4f42 	.word	0x544f4f42
3000c560:	56454c2d 	.word	0x56454c2d
3000c564:	495f4c45 	.word	0x495f4c45
3000c568:	5d4f464e 	.word	0x5d4f464e
3000c56c:	2073253a 	.word	0x2073253a
3000c570:	61766e49 	.word	0x61766e49
3000c574:	0a64696c 	.word	0x0a64696c
3000c578:	00000000 	.word	0x00000000
3000c57c:	444f4d5b 	.word	0x444f4d5b
3000c580:	5f454c55 	.word	0x5f454c55
3000c584:	544f4f42 	.word	0x544f4f42
3000c588:	56454c2d 	.word	0x56454c2d
3000c58c:	495f4c45 	.word	0x495f4c45
3000c590:	5d4f464e 	.word	0x5d4f464e
3000c594:	5b73253a 	.word	0x5b73253a
3000c598:	78383025 	.word	0x78383025
3000c59c:	5d78253a 	.word	0x5d78253a
3000c5a0:	0000000a 	.word	0x0000000a
3000c5a4:	444f4d5b 	.word	0x444f4d5b
3000c5a8:	5f454c55 	.word	0x5f454c55
3000c5ac:	544f4f42 	.word	0x544f4f42
3000c5b0:	56454c2d 	.word	0x56454c2d
3000c5b4:	495f4c45 	.word	0x495f4c45
3000c5b8:	5d4f464e 	.word	0x5d4f464e
3000c5bc:	5044523a 	.word	0x5044523a
3000c5c0:	0a4e4520 	.word	0x0a4e4520
3000c5c4:	00000000 	.word	0x00000000
3000c5c8:	444f4d5b 	.word	0x444f4d5b
3000c5cc:	5f454c55 	.word	0x5f454c55
3000c5d0:	544f4f42 	.word	0x544f4f42
3000c5d4:	56454c2d 	.word	0x56454c2d
3000c5d8:	455f4c45 	.word	0x455f4c45
3000c5dc:	524f5252 	.word	0x524f5252
3000c5e0:	61463a5d 	.word	0x61463a5d
3000c5e4:	74206c69 	.word	0x74206c69
3000c5e8:	6f6c206f 	.word	0x6f6c206f
3000c5ec:	52206461 	.word	0x52206461
3000c5f0:	69205044 	.word	0x69205044
3000c5f4:	6567616d 	.word	0x6567616d
3000c5f8:	00000a21 	.word	0x00000a21
3000c5fc:	474d490d 	.word	0x474d490d
3000c600:	544f2032 	.word	0x544f2032
3000c604:	4e452046 	.word	0x4e452046
3000c608:	0000000a 	.word	0x0000000a
3000c60c:	46544f0d 	.word	0x46544f0d
3000c610:	646f4d20 	.word	0x646f4d20
3000c614:	72652065 	.word	0x72652065
3000c618:	0d726f72 	.word	0x0d726f72
3000c61c:	0000000a 	.word	0x0000000a
3000c620:	494c4156 	.word	0x494c4156
3000c624:	00000044 	.word	0x00000044
3000c628:	41564e49 	.word	0x41564e49
3000c62c:	0044494c 	.word	0x0044494c
3000c630:	74726563 	.word	0x74726563
3000c634:	63696669 	.word	0x63696669
3000c638:	20657461 	.word	0x20657461
3000c63c:	20434345 	.word	0x20434345
3000c640:	69726576 	.word	0x69726576
3000c644:	66207966 	.word	0x66207966
3000c648:	2c6c6961 	.word	0x2c6c6961
3000c64c:	6c6f7220 	.word	0x6c6f7220
3000c650:	6361626c 	.word	0x6361626c
3000c654:	000a0d6b 	.word	0x000a0d6b
3000c658:	444f4d5b 	.word	0x444f4d5b
3000c65c:	5f454c55 	.word	0x5f454c55
3000c660:	544f4f42 	.word	0x544f4f42
3000c664:	56454c2d 	.word	0x56454c2d
3000c668:	495f4c45 	.word	0x495f4c45
3000c66c:	5d4f464e 	.word	0x5d4f464e
3000c670:	474d493a 	.word	0x474d493a
3000c674:	544f2832 	.word	0x544f2832
3000c678:	29642541 	.word	0x29642541
3000c67c:	2c732520 	.word	0x2c732520
3000c680:	74657220 	.word	0x74657220
3000c684:	6425203a 	.word	0x6425203a
3000c688:	0000000a 	.word	0x0000000a
3000c68c:	32474d49 	.word	0x32474d49
3000c690:	43434520 	.word	0x43434520
3000c694:	72657620 	.word	0x72657620
3000c698:	20796669 	.word	0x20796669
3000c69c:	6c696166 	.word	0x6c696166
3000c6a0:	6f72202c 	.word	0x6f72202c
3000c6a4:	61626c6c 	.word	0x61626c6c
3000c6a8:	0a0d6b63 	.word	0x0a0d6b63
3000c6ac:	00000000 	.word	0x00000000
3000c6b0:	5f41544f 	.word	0x5f41544f
3000c6b4:	45205041 	.word	0x45205041
3000c6b8:	76204343 	.word	0x76204343
3000c6bc:	66697265 	.word	0x66697265
3000c6c0:	61662079 	.word	0x61662079
3000c6c4:	202c6c69 	.word	0x202c6c69
3000c6c8:	6c6c6f72 	.word	0x6c6c6f72
3000c6cc:	6b636162 	.word	0x6b636162
3000c6d0:	00000a0d 	.word	0x00000a0d
3000c6d4:	64616f4c 	.word	0x64616f4c
3000c6d8:	41544f20 	.word	0x41544f20
3000c6dc:	2050415f 	.word	0x2050415f
3000c6e0:	67616d69 	.word	0x67616d69
3000c6e4:	61662065 	.word	0x61662065
3000c6e8:	202c6c69 	.word	0x202c6c69
3000c6ec:	6c6c6f72 	.word	0x6c6c6f72
3000c6f0:	6b636162 	.word	0x6b636162
3000c6f4:	00000a0d 	.word	0x00000a0d
3000c6f8:	444f4d5b 	.word	0x444f4d5b
3000c6fc:	5f454c55 	.word	0x5f454c55
3000c700:	544f4f42 	.word	0x544f4f42
3000c704:	56454c2d 	.word	0x56454c2d
3000c708:	455f4c45 	.word	0x455f4c45
3000c70c:	524f5252 	.word	0x524f5252
3000c710:	61463a5d 	.word	0x61463a5d
3000c714:	74206c69 	.word	0x74206c69
3000c718:	6f6c206f 	.word	0x6f6c206f
3000c71c:	41206461 	.word	0x41206461
3000c720:	6d692050 	.word	0x6d692050
3000c724:	21656761 	.word	0x21656761
3000c728:	0000000a 	.word	0x0000000a
3000c72c:	20344d4b 	.word	0x20344d4b
3000c730:	33474d49 	.word	0x33474d49
3000c734:	00000000 	.word	0x00000000
3000c738:	20344d4b 	.word	0x20344d4b
3000c73c:	0043534e 	.word	0x0043534e
3000c740:	31474d49 	.word	0x31474d49
3000c744:	00000000 	.word	0x00000000
3000c748:	32474d49 	.word	0x32474d49
3000c74c:	00000000 	.word	0x00000000
3000c750:	42205041 	.word	0x42205041
3000c754:	4626314c 	.word	0x4626314c
3000c758:	00005049 	.word	0x00005049
3000c75c:	444f4d5b 	.word	0x444f4d5b
3000c760:	5f454c55 	.word	0x5f454c55
3000c764:	544f4f42 	.word	0x544f4f42
3000c768:	56454c2d 	.word	0x56454c2d
3000c76c:	455f4c45 	.word	0x455f4c45
3000c770:	524f5252 	.word	0x524f5252
3000c774:	68543a5d 	.word	0x68543a5d
3000c778:	20657265 	.word	0x20657265
3000c77c:	206e6163 	.word	0x206e6163
3000c780:	61206562 	.word	0x61206562
3000c784:	6f6d2074 	.word	0x6f6d2074
3000c788:	38207473 	.word	0x38207473
3000c78c:	49535220 	.word	0x49535220
3000c790:	616d2050 	.word	0x616d2050
3000c794:	65206b73 	.word	0x65206b73
3000c798:	6972746e 	.word	0x6972746e
3000c79c:	0a217365 	.word	0x0a217365
3000c7a0:	00000000 	.word	0x00000000
3000c7a4:	444f4d5b 	.word	0x444f4d5b
3000c7a8:	5f454c55 	.word	0x5f454c55
3000c7ac:	544f4f42 	.word	0x544f4f42
3000c7b0:	56454c2d 	.word	0x56454c2d
3000c7b4:	455f4c45 	.word	0x455f4c45
3000c7b8:	524f5252 	.word	0x524f5252
3000c7bc:	53523a5d 	.word	0x53523a5d
3000c7c0:	65205049 	.word	0x65205049
3000c7c4:	7972746e 	.word	0x7972746e
3000c7c8:	64646120 	.word	0x64646120
3000c7cc:	78252072 	.word	0x78252072
3000c7d0:	766e6920 	.word	0x766e6920
3000c7d4:	64696c61 	.word	0x64696c61
3000c7d8:	0000000a 	.word	0x0000000a
3000c7dc:	059e4eb3 	.word	0x059e4eb3
3000c7e0:	4b11ea39 	.word	0x4b11ea39
3000c7e4:	2a1b920a 	.word	0x2a1b920a
3000c7e8:	1d939cee 	.word	0x1d939cee
3000c7ec:	00000000 	.word	0x00000000
3000c7f0:	42205041 	.word	0x42205041
3000c7f4:	0032334c 	.word	0x0032334c
3000c7f8:	2073250d 	.word	0x2073250d
3000c7fc:	49524556 	.word	0x49524556
3000c800:	50205946 	.word	0x50205946
3000c804:	0a535341 	.word	0x0a535341
3000c808:	00000000 	.word	0x00000000
3000c80c:	2073250d 	.word	0x2073250d
3000c810:	49524556 	.word	0x49524556
3000c814:	46205946 	.word	0x46205946
3000c818:	2c4c4941 	.word	0x2c4c4941
3000c81c:	74657220 	.word	0x74657220
3000c820:	25203d20 	.word	0x25203d20
3000c824:	00000a64 	.word	0x00000a64
3000c828:	7265430d 	.word	0x7265430d
3000c82c:	69666974 	.word	0x69666974
3000c830:	65746163 	.word	0x65746163
3000c834:	52455620 	.word	0x52455620
3000c838:	20594649 	.word	0x20594649
3000c83c:	4c494146 	.word	0x4c494146
3000c840:	6572202c 	.word	0x6572202c
3000c844:	203d2074 	.word	0x203d2074
3000c848:	000a6425 	.word	0x000a6425
3000c84c:	444f4d5b 	.word	0x444f4d5b
3000c850:	5f454c55 	.word	0x5f454c55
3000c854:	544f4f42 	.word	0x544f4f42
3000c858:	56454c2d 	.word	0x56454c2d
3000c85c:	495f4c45 	.word	0x495f4c45
3000c860:	5d4f464e 	.word	0x5d4f464e
3000c864:	41544f3a 	.word	0x41544f3a
3000c868:	53552031 	.word	0x53552031
3000c86c:	76202c45 	.word	0x76202c45
3000c870:	69737265 	.word	0x69737265
3000c874:	203a6e6f 	.word	0x203a6e6f
3000c878:	000a6425 	.word	0x000a6425
3000c87c:	444f4d5b 	.word	0x444f4d5b
3000c880:	5f454c55 	.word	0x5f454c55
3000c884:	544f4f42 	.word	0x544f4f42
3000c888:	56454c2d 	.word	0x56454c2d
3000c88c:	495f4c45 	.word	0x495f4c45
3000c890:	5d4f464e 	.word	0x5d4f464e
3000c894:	41544f3a 	.word	0x41544f3a
3000c898:	53552032 	.word	0x53552032
3000c89c:	76202c45 	.word	0x76202c45
3000c8a0:	69737265 	.word	0x69737265
3000c8a4:	203a6e6f 	.word	0x203a6e6f
3000c8a8:	000a6425 	.word	0x000a6425
3000c8ac:	444f4d5b 	.word	0x444f4d5b
3000c8b0:	5f454c55 	.word	0x5f454c55
3000c8b4:	544f4f42 	.word	0x544f4f42
3000c8b8:	56454c2d 	.word	0x56454c2d
3000c8bc:	495f4c45 	.word	0x495f4c45
3000c8c0:	5d4f464e 	.word	0x5d4f464e
3000c8c4:	746f423a 	.word	0x746f423a
3000c8c8:	544f2068 	.word	0x544f2068
3000c8cc:	6d692041 	.word	0x6d692041
3000c8d0:	20656761 	.word	0x20656761
3000c8d4:	69726576 	.word	0x69726576
3000c8d8:	66207966 	.word	0x66207966
3000c8dc:	656c6961 	.word	0x656c6961
3000c8e0:	00000a64 	.word	0x00000a64
3000c8e4:	444f4d5b 	.word	0x444f4d5b
3000c8e8:	5f454c55 	.word	0x5f454c55
3000c8ec:	544f4f42 	.word	0x544f4f42
3000c8f0:	56454c2d 	.word	0x56454c2d
3000c8f4:	495f4c45 	.word	0x495f4c45
3000c8f8:	5d4f464e 	.word	0x5d4f464e
3000c8fc:	696e493a 	.word	0x696e493a
3000c900:	42572074 	.word	0x42572074
3000c904:	00000a0d 	.word	0x00000a0d
3000c908:	444f4d5b 	.word	0x444f4d5b
3000c90c:	5f454c55 	.word	0x5f454c55
3000c910:	544f4f42 	.word	0x544f4f42
3000c914:	56454c2d 	.word	0x56454c2d
3000c918:	495f4c45 	.word	0x495f4c45
3000c91c:	5d4f464e 	.word	0x5d4f464e
3000c920:	4965523a 	.word	0x4965523a
3000c924:	2074696e 	.word	0x2074696e
3000c928:	0d524444 	.word	0x0d524444
3000c92c:	0000000a 	.word	0x0000000a
3000c930:	73616c66 	.word	0x73616c66
3000c934:	69735f68 	.word	0x69735f68
3000c938:	203a657a 	.word	0x203a657a
3000c93c:	000a4d38 	.word	0x000a4d38
3000c940:	73616c66 	.word	0x73616c66
3000c944:	69735f68 	.word	0x69735f68
3000c948:	203a657a 	.word	0x203a657a
3000c94c:	0a4d3631 	.word	0x0a4d3631
3000c950:	00000000 	.word	0x00000000
3000c954:	20315042 	.word	0x20315042
3000c958:	73726576 	.word	0x73726576
3000c95c:	206e6f69 	.word	0x206e6f69
3000c960:	61766e69 	.word	0x61766e69
3000c964:	0a64696c 	.word	0x0a64696c
3000c968:	00000000 	.word	0x00000000
3000c96c:	20315042 	.word	0x20315042
3000c970:	6d726f66 	.word	0x6d726f66
3000c974:	69207461 	.word	0x69207461
3000c978:	6c61766e 	.word	0x6c61766e
3000c97c:	000a6469 	.word	0x000a6469
3000c980:	20315042 	.word	0x20315042
3000c984:	69746361 	.word	0x69746361
3000c988:	69206576 	.word	0x69206576
3000c98c:	7865646e 	.word	0x7865646e
3000c990:	766e6920 	.word	0x766e6920
3000c994:	64696c61 	.word	0x64696c61
3000c998:	0000000a 	.word	0x0000000a
3000c99c:	20315042 	.word	0x20315042
3000c9a0:	61746164 	.word	0x61746164
3000c9a4:	6c617620 	.word	0x6c617620
3000c9a8:	202c6469 	.word	0x202c6469
3000c9ac:	73726576 	.word	0x73726576
3000c9b0:	3a6e6f69 	.word	0x3a6e6f69
3000c9b4:	000a6425 	.word	0x000a6425
3000c9b8:	20325042 	.word	0x20325042
3000c9bc:	6d726f66 	.word	0x6d726f66
3000c9c0:	69207461 	.word	0x69207461
3000c9c4:	6c61766e 	.word	0x6c61766e
3000c9c8:	000a6469 	.word	0x000a6469
3000c9cc:	20315042 	.word	0x20315042
3000c9d0:	33435243 	.word	0x33435243
3000c9d4:	616d2032 	.word	0x616d2032
3000c9d8:	2c686374 	.word	0x2c686374
3000c9dc:	74746120 	.word	0x74746120
3000c9e0:	65686361 	.word	0x65686361
3000c9e4:	52432064 	.word	0x52432064
3000c9e8:	30203a43 	.word	0x30203a43
3000c9ec:	2c782578 	.word	0x2c782578
3000c9f0:	6c616320 	.word	0x6c616320
3000c9f4:	616c7563 	.word	0x616c7563
3000c9f8:	20646574 	.word	0x20646574
3000c9fc:	3a435243 	.word	0x3a435243
3000ca00:	25783020 	.word	0x25783020
3000ca04:	00000a78 	.word	0x00000a78
3000ca08:	20315042 	.word	0x20315042
3000ca0c:	33435243 	.word	0x33435243
3000ca10:	6f6e2032 	.word	0x6f6e2032
3000ca14:	616d2074 	.word	0x616d2074
3000ca18:	2c686374 	.word	0x2c686374
3000ca1c:	74746120 	.word	0x74746120
3000ca20:	65686361 	.word	0x65686361
3000ca24:	52432064 	.word	0x52432064
3000ca28:	30203a43 	.word	0x30203a43
3000ca2c:	2c782578 	.word	0x2c782578
3000ca30:	6c616320 	.word	0x6c616320
3000ca34:	616c7563 	.word	0x616c7563
3000ca38:	20646574 	.word	0x20646574
3000ca3c:	3a435243 	.word	0x3a435243
3000ca40:	25783020 	.word	0x25783020
3000ca44:	00000a78 	.word	0x00000a78
3000ca48:	20325042 	.word	0x20325042
3000ca4c:	33435243 	.word	0x33435243
3000ca50:	616d2032 	.word	0x616d2032
3000ca54:	2c686374 	.word	0x2c686374
3000ca58:	74746120 	.word	0x74746120
3000ca5c:	65686361 	.word	0x65686361
3000ca60:	52432064 	.word	0x52432064
3000ca64:	30203a43 	.word	0x30203a43
3000ca68:	2c782578 	.word	0x2c782578
3000ca6c:	6c616320 	.word	0x6c616320
3000ca70:	616c7563 	.word	0x616c7563
3000ca74:	20646574 	.word	0x20646574
3000ca78:	3a435243 	.word	0x3a435243
3000ca7c:	25783020 	.word	0x25783020
3000ca80:	00000a78 	.word	0x00000a78
3000ca84:	20325042 	.word	0x20325042
3000ca88:	33435243 	.word	0x33435243
3000ca8c:	6f6e2032 	.word	0x6f6e2032
3000ca90:	616d2074 	.word	0x616d2074
3000ca94:	2c686374 	.word	0x2c686374
3000ca98:	74746120 	.word	0x74746120
3000ca9c:	65686361 	.word	0x65686361
3000caa0:	52432064 	.word	0x52432064
3000caa4:	30203a43 	.word	0x30203a43
3000caa8:	2c782578 	.word	0x2c782578
3000caac:	6c616320 	.word	0x6c616320
3000cab0:	616c7563 	.word	0x616c7563
3000cab4:	20646574 	.word	0x20646574
3000cab8:	3a435243 	.word	0x3a435243
3000cabc:	25783020 	.word	0x25783020
3000cac0:	00000a78 	.word	0x00000a78
3000cac4:	20315042 	.word	0x20315042
3000cac8:	3161746f 	.word	0x3161746f
3000cacc:	64646120 	.word	0x64646120
3000cad0:	73736572 	.word	0x73736572
3000cad4:	766e6920 	.word	0x766e6920
3000cad8:	64696c61 	.word	0x64696c61
3000cadc:	0000000a 	.word	0x0000000a
3000cae0:	20315042 	.word	0x20315042
3000cae4:	3261746f 	.word	0x3261746f
3000cae8:	64646120 	.word	0x64646120
3000caec:	73736572 	.word	0x73736572
3000caf0:	766e6920 	.word	0x766e6920
3000caf4:	64696c61 	.word	0x64696c61
3000caf8:	0000000a 	.word	0x0000000a
3000cafc:	68746f42 	.word	0x68746f42
3000cb00:	20504220 	.word	0x20504220
3000cb04:	20435243 	.word	0x20435243
3000cb08:	696c6176 	.word	0x696c6176
3000cb0c:	00000a64 	.word	0x00000a64
3000cb10:	20315042 	.word	0x20315042
3000cb14:	656c6573 	.word	0x656c6573
3000cb18:	64657463 	.word	0x64657463
3000cb1c:	0000000a 	.word	0x0000000a
3000cb20:	20325042 	.word	0x20325042
3000cb24:	656c6573 	.word	0x656c6573
3000cb28:	64657463 	.word	0x64657463
3000cb2c:	0000000a 	.word	0x0000000a
3000cb30:	68746f42 	.word	0x68746f42
3000cb34:	20504220 	.word	0x20504220
3000cb38:	61766e69 	.word	0x61766e69
3000cb3c:	0a64696c 	.word	0x0a64696c
3000cb40:	00000000 	.word	0x00000000
3000cb44:	6f636552 	.word	0x6f636552
3000cb48:	79726576 	.word	0x79726576
3000cb4c:	61747320 	.word	0x61747320
3000cb50:	000a7472 	.word	0x000a7472
3000cb54:	69746341 	.word	0x69746341
3000cb58:	49206576 	.word	0x49206576
3000cb5c:	7865646e 	.word	0x7865646e
3000cb60:	6425203a 	.word	0x6425203a
3000cb64:	0000000a 	.word	0x0000000a
3000cb68:	3141544f 	.word	0x3141544f
3000cb6c:	766e6920 	.word	0x766e6920
3000cb70:	64696c61 	.word	0x64696c61
3000cb74:	0000000a 	.word	0x0000000a
3000cb78:	3241544f 	.word	0x3241544f
3000cb7c:	766e6920 	.word	0x766e6920
3000cb80:	64696c61 	.word	0x64696c61
3000cb84:	0000000a 	.word	0x0000000a
3000cb88:	3141544f 	.word	0x3141544f
3000cb8c:	43524320 	.word	0x43524320
3000cb90:	6d203233 	.word	0x6d203233
3000cb94:	68637461 	.word	0x68637461
3000cb98:	7461202c 	.word	0x7461202c
3000cb9c:	68636174 	.word	0x68636174
3000cba0:	43206465 	.word	0x43206465
3000cba4:	203a4352 	.word	0x203a4352
3000cba8:	78257830 	.word	0x78257830
3000cbac:	6163202c 	.word	0x6163202c
3000cbb0:	6c75636c 	.word	0x6c75636c
3000cbb4:	64657461 	.word	0x64657461
3000cbb8:	43524320 	.word	0x43524320
3000cbbc:	7830203a 	.word	0x7830203a
3000cbc0:	000a7825 	.word	0x000a7825
3000cbc4:	3141544f 	.word	0x3141544f
3000cbc8:	43524320 	.word	0x43524320
3000cbcc:	6e203233 	.word	0x6e203233
3000cbd0:	6d20746f 	.word	0x6d20746f
3000cbd4:	68637461 	.word	0x68637461
3000cbd8:	7461202c 	.word	0x7461202c
3000cbdc:	68636174 	.word	0x68636174
3000cbe0:	43206465 	.word	0x43206465
3000cbe4:	203a4352 	.word	0x203a4352
3000cbe8:	78257830 	.word	0x78257830
3000cbec:	6163202c 	.word	0x6163202c
3000cbf0:	6c75636c 	.word	0x6c75636c
3000cbf4:	64657461 	.word	0x64657461
3000cbf8:	43524320 	.word	0x43524320
3000cbfc:	7830203a 	.word	0x7830203a
3000cc00:	202c7825 	.word	0x202c7825
3000cc04:	73617265 	.word	0x73617265
3000cc08:	544f2065 	.word	0x544f2065
3000cc0c:	000a3141 	.word	0x000a3141
3000cc10:	3241544f 	.word	0x3241544f
3000cc14:	43524320 	.word	0x43524320
3000cc18:	6d203233 	.word	0x6d203233
3000cc1c:	68637461 	.word	0x68637461
3000cc20:	7461202c 	.word	0x7461202c
3000cc24:	68636174 	.word	0x68636174
3000cc28:	43206465 	.word	0x43206465
3000cc2c:	203a4352 	.word	0x203a4352
3000cc30:	78257830 	.word	0x78257830
3000cc34:	6163202c 	.word	0x6163202c
3000cc38:	6c75636c 	.word	0x6c75636c
3000cc3c:	64657461 	.word	0x64657461
3000cc40:	43524320 	.word	0x43524320
3000cc44:	7830203a 	.word	0x7830203a
3000cc48:	000a7825 	.word	0x000a7825
3000cc4c:	3241544f 	.word	0x3241544f
3000cc50:	43524320 	.word	0x43524320
3000cc54:	6e203233 	.word	0x6e203233
3000cc58:	6d20746f 	.word	0x6d20746f
3000cc5c:	68637461 	.word	0x68637461
3000cc60:	7461202c 	.word	0x7461202c
3000cc64:	68636174 	.word	0x68636174
3000cc68:	43206465 	.word	0x43206465
3000cc6c:	203a4352 	.word	0x203a4352
3000cc70:	78257830 	.word	0x78257830
3000cc74:	6163202c 	.word	0x6163202c
3000cc78:	6c75636c 	.word	0x6c75636c
3000cc7c:	64657461 	.word	0x64657461
3000cc80:	43524320 	.word	0x43524320
3000cc84:	7830203a 	.word	0x7830203a
3000cc88:	202c7825 	.word	0x202c7825
3000cc8c:	73617265 	.word	0x73617265
3000cc90:	544f2065 	.word	0x544f2065
3000cc94:	000a3241 	.word	0x000a3241
3000cc98:	69746341 	.word	0x69746341
3000cc9c:	69206576 	.word	0x69206576
3000cca0:	7865646e 	.word	0x7865646e
3000cca4:	41544f20 	.word	0x41544f20
3000cca8:	766e6920 	.word	0x766e6920
3000ccac:	64696c61 	.word	0x64696c61
3000ccb0:	6572202c 	.word	0x6572202c
3000ccb4:	74697277 	.word	0x74697277
3000ccb8:	63612065 	.word	0x63612065
3000ccbc:	65766974 	.word	0x65766974
3000ccc0:	646e6920 	.word	0x646e6920
3000ccc4:	74207865 	.word	0x74207865
3000ccc8:	6425206f 	.word	0x6425206f
3000cccc:	0000000a 	.word	0x0000000a
3000ccd0:	206c6c41 	.word	0x206c6c41
3000ccd4:	2041544f 	.word	0x2041544f
3000ccd8:	72646461 	.word	0x72646461
3000ccdc:	65737365 	.word	0x65737365
3000cce0:	6f6e2073 	.word	0x6f6e2073
3000cce4:	61762074 	.word	0x61762074
3000cce8:	2c64696c 	.word	0x2c64696c
3000ccec:	63657220 	.word	0x63657220
3000ccf0:	7265766f 	.word	0x7265766f
3000ccf4:	61662079 	.word	0x61662079
3000ccf8:	000a6c69 	.word	0x000a6c69
3000ccfc:	20325042 	.word	0x20325042
3000cd00:	3261746f 	.word	0x3261746f
3000cd04:	64646120 	.word	0x64646120
3000cd08:	73736572 	.word	0x73736572
3000cd0c:	766e6920 	.word	0x766e6920
3000cd10:	64696c61 	.word	0x64696c61
3000cd14:	0000000a 	.word	0x0000000a
3000cd18:	20325042 	.word	0x20325042
3000cd1c:	73726576 	.word	0x73726576
3000cd20:	206e6f69 	.word	0x206e6f69
3000cd24:	61766e69 	.word	0x61766e69
3000cd28:	0a64696c 	.word	0x0a64696c
3000cd2c:	00000000 	.word	0x00000000
3000cd30:	20325042 	.word	0x20325042
3000cd34:	69746361 	.word	0x69746361
3000cd38:	69206576 	.word	0x69206576
3000cd3c:	7865646e 	.word	0x7865646e
3000cd40:	766e6920 	.word	0x766e6920
3000cd44:	64696c61 	.word	0x64696c61
3000cd48:	0000000a 	.word	0x0000000a
3000cd4c:	20325042 	.word	0x20325042
3000cd50:	3161746f 	.word	0x3161746f
3000cd54:	64646120 	.word	0x64646120
3000cd58:	73736572 	.word	0x73736572
3000cd5c:	766e6920 	.word	0x766e6920
3000cd60:	64696c61 	.word	0x64696c61
3000cd64:	0000000a 	.word	0x0000000a
3000cd68:	20325042 	.word	0x20325042
3000cd6c:	61746164 	.word	0x61746164
3000cd70:	6c617620 	.word	0x6c617620
3000cd74:	202c6469 	.word	0x202c6469
3000cd78:	73726576 	.word	0x73726576
3000cd7c:	3a6e6f69 	.word	0x3a6e6f69
3000cd80:	000a6425 	.word	0x000a6425
3000cd84:	444f4d5b 	.word	0x444f4d5b
3000cd88:	5f454c55 	.word	0x5f454c55
3000cd8c:	544f4f42 	.word	0x544f4f42
3000cd90:	56454c2d 	.word	0x56454c2d
3000cd94:	495f4c45 	.word	0x495f4c45
3000cd98:	5d4f464e 	.word	0x5d4f464e
3000cd9c:	7369443a 	.word	0x7369443a
3000cda0:	656c6261 	.word	0x656c6261
3000cda4:	304d4b20 	.word	0x304d4b20
3000cda8:	6f6f4220 	.word	0x6f6f4220
3000cdac:	69202074 	.word	0x69202074
3000cdb0:	6554206e 	.word	0x6554206e
3000cdb4:	6d207473 	.word	0x6d207473
3000cdb8:	0d65646f 	.word	0x0d65646f
3000cdbc:	0000000a 	.word	0x0000000a
3000cdc0:	444f4d5b 	.word	0x444f4d5b
3000cdc4:	5f454c55 	.word	0x5f454c55
3000cdc8:	544f4f42 	.word	0x544f4f42
3000cdcc:	56454c2d 	.word	0x56454c2d
3000cdd0:	495f4c45 	.word	0x495f4c45
3000cdd4:	5d4f464e 	.word	0x5d4f464e
3000cdd8:	7079423a 	.word	0x7079423a
3000cddc:	20737361 	.word	0x20737361
3000cde0:	2050544f 	.word	0x2050544f
3000cde4:	6f747561 	.word	0x6f747561
3000cde8:	64616f6c 	.word	0x64616f6c
3000cdec:	00000a0d 	.word	0x00000a0d
3000cdf0:	444f4d5b 	.word	0x444f4d5b
3000cdf4:	5f454c55 	.word	0x5f454c55
3000cdf8:	544f4f42 	.word	0x544f4f42
3000cdfc:	56454c2d 	.word	0x56454c2d
3000ce00:	495f4c45 	.word	0x495f4c45
3000ce04:	5d4f464e 	.word	0x5d4f464e
3000ce08:	20504e3a 	.word	0x20504e3a
3000ce0c:	71657246 	.word	0x71657246
3000ce10:	20642520 	.word	0x20642520
3000ce14:	0a7a484d 	.word	0x0a7a484d
3000ce18:	00000000 	.word	0x00000000
3000ce1c:	444f4d5b 	.word	0x444f4d5b
3000ce20:	5f454c55 	.word	0x5f454c55
3000ce24:	544f4f42 	.word	0x544f4f42
3000ce28:	56454c2d 	.word	0x56454c2d
3000ce2c:	495f4c45 	.word	0x495f4c45
3000ce30:	5d4f464e 	.word	0x5d4f464e
3000ce34:	2050413a 	.word	0x2050413a
3000ce38:	71657246 	.word	0x71657246
3000ce3c:	20642520 	.word	0x20642520
3000ce40:	0a7a484d 	.word	0x0a7a484d
3000ce44:	00000000 	.word	0x00000000
3000ce48:	7361700a 	.word	0x7361700a
3000ce4c:	726f7773 	.word	0x726f7773
3000ce50:	65762064 	.word	0x65762064
3000ce54:	79666972 	.word	0x79666972
3000ce58:	69616620 	.word	0x69616620
3000ce5c:	00000a6c 	.word	0x00000a6c
3000ce60:	7361700a 	.word	0x7361700a
3000ce64:	726f7773 	.word	0x726f7773
3000ce68:	65762064 	.word	0x65762064
3000ce6c:	79666972 	.word	0x79666972
3000ce70:	63757320 	.word	0x63757320
3000ce74:	73736563 	.word	0x73736563
3000ce78:	0000000a 	.word	0x0000000a
3000ce7c:	746f6f62 	.word	0x746f6f62
3000ce80:	64616f6c 	.word	0x64616f6c
3000ce84:	765f7265 	.word	0x765f7265
3000ce88:	69737265 	.word	0x69737265
3000ce8c:	25206e6f 	.word	0x25206e6f
3000ce90:	00000a73 	.word	0x00000a73
3000ce94:	444f4d5b 	.word	0x444f4d5b
3000ce98:	5f454c55 	.word	0x5f454c55
3000ce9c:	544f4f42 	.word	0x544f4f42
3000cea0:	56454c2d 	.word	0x56454c2d
3000cea4:	495f4c45 	.word	0x495f4c45
3000cea8:	5d4f464e 	.word	0x5d4f464e
3000ceac:	474d493a 	.word	0x474d493a
3000ceb0:	4e452031 	.word	0x4e452031
3000ceb4:	20524554 	.word	0x20524554
3000ceb8:	3a50534d 	.word	0x3a50534d
3000cebc:	3830255b 	.word	0x3830255b
3000cec0:	000a5d78 	.word	0x000a5d78
3000cec4:	444f4d5b 	.word	0x444f4d5b
3000cec8:	5f454c55 	.word	0x5f454c55
3000cecc:	544f4f42 	.word	0x544f4f42
3000ced0:	56454c2d 	.word	0x56454c2d
3000ced4:	495f4c45 	.word	0x495f4c45
3000ced8:	5d4f464e 	.word	0x5d4f464e
3000cedc:	474d493a 	.word	0x474d493a
3000cee0:	45532031 	.word	0x45532031
3000cee4:	45525543 	.word	0x45525543
3000cee8:	41545320 	.word	0x41545320
3000ceec:	203a4554 	.word	0x203a4554
3000cef0:	000a6425 	.word	0x000a6425
3000cef4:	444f4d5b 	.word	0x444f4d5b
3000cef8:	5f454c55 	.word	0x5f454c55
3000cefc:	544f4f42 	.word	0x544f4f42
3000cf00:	56454c2d 	.word	0x56454c2d
3000cf04:	495f4c45 	.word	0x495f4c45
3000cf08:	5d4f464e 	.word	0x5d4f464e
3000cf0c:	696e493a 	.word	0x696e493a
3000cf10:	53502074 	.word	0x53502074
3000cf14:	0d4d4152 	.word	0x0d4d4152
3000cf18:	0000000a 	.word	0x0000000a
3000cf1c:	7465730d 	.word	0x7465730d
3000cf20:	6d667020 	.word	0x6d667020
3000cf24:	69616620 	.word	0x69616620
3000cf28:	000a0d6c 	.word	0x000a0d6c
3000cf2c:	444f4d5b 	.word	0x444f4d5b
3000cf30:	5f454c55 	.word	0x5f454c55
3000cf34:	544f4f42 	.word	0x544f4f42
3000cf38:	56454c2d 	.word	0x56454c2d
3000cf3c:	495f4c45 	.word	0x495f4c45
3000cf40:	5d4f464e 	.word	0x5d4f464e
3000cf44:	5252453a 	.word	0x5252453a
3000cf48:	2121524f 	.word	0x2121524f
3000cf4c:	6f685320 	.word	0x6f685320
3000cf50:	20646c75 	.word	0x20646c75
3000cf54:	20746f4e 	.word	0x20746f4e
3000cf58:	62616e65 	.word	0x62616e65
3000cf5c:	4d20656c 	.word	0x4d20656c
3000cf60:	77536d65 	.word	0x77536d65
3000cf64:	6e4f5f72 	.word	0x6e4f5f72
3000cf68:	6920796c 	.word	0x6920796c
3000cf6c:	4444206e 	.word	0x4444206e
3000cf70:	68432052 	.word	0x68432052
3000cf74:	21217069 	.word	0x21217069
3000cf78:	000a0d21 	.word	0x000a0d21
3000cf7c:	444f4d5b 	.word	0x444f4d5b
3000cf80:	5f454c55 	.word	0x5f454c55
3000cf84:	544f4f42 	.word	0x544f4f42
3000cf88:	56454c2d 	.word	0x56454c2d
3000cf8c:	495f4c45 	.word	0x495f4c45
3000cf90:	5d4f464e 	.word	0x5d4f464e
3000cf94:	696e493a 	.word	0x696e493a
3000cf98:	44442074 	.word	0x44442074
3000cf9c:	000a0d52 	.word	0x000a0d52
3000cfa0:	20445753 	.word	0x20445753
3000cfa4:	64207369 	.word	0x64207369
3000cfa8:	62617369 	.word	0x62617369
3000cfac:	2c64656c 	.word	0x2c64656c
3000cfb0:	6f727020 	.word	0x6f727020
3000cfb4:	64656563 	.word	0x64656563
3000cfb8:	206f7420 	.word	0x206f7420
3000cfbc:	61736964 	.word	0x61736964
3000cfc0:	20656c62 	.word	0x20656c62
3000cfc4:	74726155 	.word	0x74726155
3000cfc8:	776f4420 	.word	0x776f4420
3000cfcc:	616f6c6e 	.word	0x616f6c6e
3000cfd0:	2e2e2e64 	.word	0x2e2e2e64
3000cfd4:	0000000a 	.word	0x0000000a
3000cfd8:	74726155 	.word	0x74726155
3000cfdc:	776f4420 	.word	0x776f4420
3000cfe0:	616f6c6e 	.word	0x616f6c6e
3000cfe4:	61682064 	.word	0x61682064
3000cfe8:	65622073 	.word	0x65622073
3000cfec:	64206e65 	.word	0x64206e65
3000cff0:	62617369 	.word	0x62617369
3000cff4:	0a64656c 	.word	0x0a64656c
3000cff8:	00000000 	.word	0x00000000
3000cffc:	55676f4c 	.word	0x55676f4c
3000d000:	20747261 	.word	0x20747261
3000d004:	64756142 	.word	0x64756142
3000d008:	65746172 	.word	0x65746172
3000d00c:	6425203a 	.word	0x6425203a
3000d010:	0000000a 	.word	0x0000000a
3000d014:	4848530a 	.word	0x4848530a
3000d018:	4946544f 	.word	0x4946544f
3000d01c:	6f6e2058 	.word	0x6f6e2058
3000d020:	65722074 	.word	0x65722074
3000d024:	76696563 	.word	0x76696563
3000d028:	000a6465 	.word	0x000a6465
3000d02c:	726f4e0a 	.word	0x726f4e0a
3000d030:	206c616d 	.word	0x206c616d
3000d034:	746f6f62 	.word	0x746f6f62
3000d038:	0000000a 	.word	0x0000000a
3000d03c:	5f6e6962 	.word	0x5f6e6962
3000d040:	313e6f6e 	.word	0x313e6f6e
3000d044:	3d202c30 	.word	0x3d202c30
3000d048:	0a642520 	.word	0x0a642520
3000d04c:	00000000 	.word	0x00000000
3000d050:	6e206164 	.word	0x6e206164
3000d054:	7220746f 	.word	0x7220746f
3000d058:	69656365 	.word	0x69656365
3000d05c:	0a646576 	.word	0x0a646576
3000d060:	00000000 	.word	0x00000000
3000d064:	746f680a 	.word	0x746f680a
3000d068:	20786966 	.word	0x20786966
3000d06c:	656e6f64 	.word	0x656e6f64
3000d070:	202e2e2e 	.word	0x202e2e2e
3000d074:	6f626552 	.word	0x6f626552
3000d078:	6e69746f 	.word	0x6e69746f
3000d07c:	0a2e2e67 	.word	0x0a2e2e67
3000d080:	00000000 	.word	0x00000000
3000d084:	444f4d5b 	.word	0x444f4d5b
3000d088:	5f454c55 	.word	0x5f454c55
3000d08c:	544f4f42 	.word	0x544f4f42
3000d090:	56454c2d 	.word	0x56454c2d
3000d094:	495f4c45 	.word	0x495f4c45
3000d098:	5d4f464e 	.word	0x5d4f464e
3000d09c:	6174533a 	.word	0x6174533a
3000d0a0:	4e207472 	.word	0x4e207472
3000d0a4:	65536e6f 	.word	0x65536e6f
3000d0a8:	65727563 	.word	0x65727563
3000d0ac:	30204020 	.word	0x30204020
3000d0b0:	20782578 	.word	0x20782578
3000d0b4:	0d2e2e2e 	.word	0x0d2e2e2e
3000d0b8:	0000000a 	.word	0x0000000a
3000d0bc:	6c630a0d 	.word	0x6c630a0d
3000d0c0:	2065736f 	.word	0x2065736f
3000d0c4:	646f4d79 	.word	0x646f4d79
3000d0c8:	54206d65 	.word	0x54206d65
3000d0cc:	736e6172 	.word	0x736e6172
3000d0d0:	20726566 	.word	0x20726566
3000d0d4:	0d2e2e2e 	.word	0x0d2e2e2e
3000d0d8:	0000000a 	.word	0x0000000a
3000d0dc:	766e490d 	.word	0x766e490d
3000d0e0:	64696c61 	.word	0x64696c61
3000d0e4:	69684320 	.word	0x69684320
3000d0e8:	666e696e 	.word	0x666e696e
3000d0ec:	4320216f 	.word	0x4320216f
3000d0f0:	6b636568 	.word	0x6b636568
3000d0f4:	50544f20 	.word	0x50544f20
3000d0f8:	0000000a 	.word	0x0000000a
3000d0fc:	656c500d 	.word	0x656c500d
3000d100:	20657361 	.word	0x20657361
3000d104:	676f7270 	.word	0x676f7270
3000d108:	206d6172 	.word	0x206d6172
3000d10c:	70696863 	.word	0x70696863
3000d110:	6f666e69 	.word	0x6f666e69
3000d114:	206e6920 	.word	0x206e6920
3000d118:	2050544f 	.word	0x2050544f
3000d11c:	00000a21 	.word	0x00000a21

3000d120 <Memory_Info>:
3000d120:	03e00007 00060004 010202c0 02010100     ................
3000d130:	02000102 00040302 02040400 01010102     ................
3000d140:	01020221 02240401 03000102 00030103     !.....$.........
3000d150:	01050500 06000002 01020206 01070700     ................
3000d160:	08000002 01020208 02280801 09000102     ..........(.....
3000d170:	00040309 03290901 0a000004 0003010a     ......).........
3000d180:	012a0a01 0a010003 0003012a 020b0b00     ..*.....*.......
3000d190:	0c000102 0102020c ffffffff 0000ffff     ................
3000d1a0:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d1b0:	455f4c45 524f5252 52443a5d 69204d41     EL_ERROR]:DRAM i
3000d1c0:	6f6e2073 61432074 7262696c 6e6f6961     s not Calibraion
3000d1d0:	00000a0d                                ....

3000d1d4 <Flash_AVL>:
3000d1d4:	000000ef 000000ff 00000000 000043fc     .............C..
3000d1e4:	00000000 000000a1 000000ff 00000000     ................
3000d1f4:	0000fffc 00000000 0000000b 000000ff     ................
3000d204:	00000000 000043fc 00000000 0000000e     .....C..........
3000d214:	000000ff 00000000 000043fc 00000000     .........C......
3000d224:	000000c8 000000ff 00000001 000043fc     .............C..
3000d234:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d244:	000200fc 00000000 000000c2 000000ff     ................
3000d254:	00000002 000000fc 00000000 00000068     ............h...
3000d264:	000000ff 00000002 000000fc 00000000     ................
3000d274:	00000051 000000ff 00000002 000000fc     Q...............
3000d284:	00000000 0000001c 000000ff 00000003     ................
3000d294:	000000fc 00000000 00000020 000000ff     ........ .......
3000d2a4:	00000004 000000fc 00000000 00000000     ................
3000d2b4:	000000ff 000000fe ffffffff 30008b3d     ............=..0
3000d2c4:	000000ff ffffffff 000000ff ffffffff     ................
3000d2d4:	00000000                                ....

3000d2d8 <Flash_ReadMode>:
3000d2d8:	                                         ..

3000d2da <Flash_Speed>:
3000d2da:	                                         ..

3000d2dc <NAND_AVL>:
3000d2dc:	000000ef 000000ff 00000000 ffffffff     ................
3000d2ec:	00000000 000000c8 000000ff 00000001     ................
3000d2fc:	ffffffff 00000000 000000e5 000000ff     ................
3000d30c:	00000001 ffffffff 00000000 000000c2     ................
3000d31c:	000000ff 00000002 ffffffff 00000000     ................
3000d32c:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d344:	000000ff 000000fe ffffffff 30008b9d     ...............0
3000d354:	000000ff ffffffff 000000ff ffffffff     ................
3000d364:	00000000 3000c218 3000c21c 3000c220     .......0...0 ..0
3000d374:	3000c224 3000c228                       $..0(..0

3000d37c <__FUNCTION__.0>:
3000d37c:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d38c <__FUNCTION__.1>:
3000d38c:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d39c:	00000000 444f4d5b 5f454c55 544f4f42     ....[MODULE_BOOT
3000d3ac:	56454c2d 455f4c45 524f5252 6c423a5d     -LEVEL_ERROR]:Bl
3000d3bc:	7830206b 69207825 42422073 00000a21     k 0x%x is BB!...
3000d3cc:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d3dc:	495f4c45 5d4f464e 6165523a 61502064     EL_INFO]:Read Pa
3000d3ec:	30206567 20782578 4220666f 6b636f6c     ge 0x%x of Block
3000d3fc:	25783020 614d2078 61462079 202c6c69      0x%x May Fail, 
3000d40c:	74617473 30207375 21782578 0000000a     status 0x%x!....
3000d41c:	444f4d5b 5f454c55 2d50544f 4556454c     [MODULE_OTP-LEVE
3000d42c:	52455f4c 5d524f52 50544f3a 6165525f     L_ERROR]:OTP_Rea
3000d43c:	46203864 206c6961 0a207825 00000000     d8 Fail %x .....
3000d44c:	444f4d5b 5f454c55 2d50544f 4556454c     [MODULE_OTP-LEVE
3000d45c:	52455f4c 5d524f52 524d503a 61655220     L_ERROR]:PMR Rea
3000d46c:	72652064 21726f72 0000000a 444f4d5b     d error!....[MOD
3000d47c:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d48c:	5d524f52 50544f3a 6972575f 20386574     ROR]:OTP_Write8 
3000d49c:	6c696146 20782520 0000000a 444f4d5b     Fail %x ....[MOD
3000d4ac:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d4bc:	5d524f52 50544f3a 6f727020 6d617267     ROR]:OTP program
3000d4cc:	72726520 0a21726f 00000000 444f4d5b      error!.....[MOD
3000d4dc:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d4ec:	5d524f52 524d503a 65522032 65206461     ROR]:PMR2 Read e
3000d4fc:	726f7272 00000a21                       rror!...

3000d504 <__FUNCTION__.1>:
3000d504:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d514:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d524:	495f4c45 5d4f464e 5253503a 43204d41     EL_INFO]:PSRAM C
3000d534:	206c7274 3a4b4c43 20642520 0a207a48     trl CLK: %d Hz .
3000d544:	00000000 6c61430d 6e696d4e 25203d20     .....CalNmin = %
3000d554:	61432078 616d4e6c 203d2078 57207825     x CalNmax = %x W
3000d564:	6f646e69 7a695377 203d2065 70207825     indowSize = %x p
3000d574:	65736168 7825203a 00000a20 444f4d5b     hase: %x ...[MOD
3000d584:	5f454c55 45535049 454c2d43 5f4c4556     ULE_IPSEC-LEVEL_
3000d594:	4f525245 4e3a5d52 6874206f 61207369     ERROR]:No this a
3000d5a4:	5f687475 65707974 29642528 726f6620     uth_type(%d) for
3000d5b4:	41485320 00000a32 7172690d 5d64255b      SHA2....irq[%d]
3000d5c4:	69727020 7469726f 64252079 61687320      priority %d sha
3000d5d4:	3c206c6c 6425203d 0000000a              ll <= %d....

3000d5e0 <__FUNCTION__.0>:
3000d5e0:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000d5f0:	00000000                                ....

3000d5f4 <__FUNCTION__.1>:
3000d5f4:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d604 <__FUNCTION__.2>:
3000d604:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d614:	32616873 696e695f 00000074              sha2_init...

3000d620 <__FUNCTION__.3>:
3000d620:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d630:	6f4d7974 44416564 00000000 5244440d     tyModeAD.....DDR
3000d640:	7a695320 73692065 746f6e20 6e6f6320      Size is not con
3000d650:	75676966 0a646572 00000000 5b203e0d     figured......> [
3000d660:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d670:	63203a5d 5f756c6f 616d6572 69625f70     ]: colu_remap_bi
3000d680:	21207374 7264203d 635f6d61 5f756c6f     ts != dram_colu_
3000d690:	73746962 0a212121 00000000 5b203e0d     bits!!!......> [
3000d6a0:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d6b0:	62203a5d 5f6b6e61 616d6572 69625f70     ]: bank_remap_bi
3000d6c0:	21207374 7264203d 625f6d61 5f6b6e61     ts != dram_bank_
3000d6d0:	73746962 0a212121 00000000              bits!!!.....

3000d6dc <__FUNCTION__.1>:
3000d6dc:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d6ec:	00000000 3000c4ec 3000c4f8 3000c504     .......0...0...0
3000d6fc:	3000c510 3000c51c 3000c528 3000c534     ...0...0(..04..0
3000d70c:	3000c540 3000c54c 3000c534 3000c540     @..0L..04..0@..0
3000d71c:	3000c72c 3000c738 3000c740 3000c748     ,..08..0@..0H..0
3000d72c:	3000c730 3000c750 00000208 0000020c     0..0P..0........
3000d73c:	00000210 00000008 00000214 00000218     ................
3000d74c:	0000021c 0000000c                       ........

3000d754 <__FUNCTION__.0>:
3000d754:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.

3000d764 <km4_bootloader_rev>:
3000d764:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000d774:	625f7265 32303635 325f3763 2f333230     er_b5602c7_2023/
3000d784:	312f3031 37312d39 3a32343a 00003531     10/19-17:42:15..
