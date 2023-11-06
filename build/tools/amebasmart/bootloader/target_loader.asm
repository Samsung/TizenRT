
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
30003272:	f008 ffd9 	bl	3000c228 <__DiagPrintf_veneer>
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
300032ea:	f008 ff9d 	bl	3000c228 <__DiagPrintf_veneer>
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
30003316:	f008 ff87 	bl	3000c228 <__DiagPrintf_veneer>
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
30003342:	f008 ff71 	bl	3000c228 <__DiagPrintf_veneer>
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
3000335c:	f008 ff64 	bl	3000c228 <__DiagPrintf_veneer>
30003360:	e7be      	b.n	300032e0 <FLASH_CalibrationNew+0x168>
30003362:	480e      	ldr	r0, [pc, #56]	; (3000339c <FLASH_CalibrationNew+0x224>)
30003364:	f008 ff60 	bl	3000c228 <__DiagPrintf_veneer>
30003368:	e795      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
3000336a:	bf00      	nop
3000336c:	3000a541 	.word	0x3000a541
30003370:	2001c01c 	.word	0x2001c01c
30003374:	00009c99 	.word	0x00009c99
30003378:	e000ed00 	.word	0xe000ed00
3000337c:	2001c00c 	.word	0x2001c00c
30003380:	00054060 	.word	0x00054060
30003384:	000129bd 	.word	0x000129bd
30003388:	3000c298 	.word	0x3000c298
3000338c:	3000c26c 	.word	0x3000c26c
30003390:	0000b7e9 	.word	0x0000b7e9
30003394:	ffff0000 	.word	0xffff0000
30003398:	3000c2bc 	.word	0x3000c2bc
3000339c:	3000c2f8 	.word	0x3000c2f8
300033a0:	3000a351 	.word	0x3000a351

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
300034c6:	f008 feaf 	bl	3000c228 <__DiagPrintf_veneer>
300034ca:	e7db      	b.n	30003484 <FLASH_Calibration+0xe0>
300034cc:	4652      	mov	r2, sl
300034ce:	9b02      	ldr	r3, [sp, #8]
300034d0:	480b      	ldr	r0, [pc, #44]	; (30003500 <FLASH_Calibration+0x15c>)
300034d2:	9105      	str	r1, [sp, #20]
300034d4:	f008 fea8 	bl	3000c228 <__DiagPrintf_veneer>
300034d8:	9905      	ldr	r1, [sp, #20]
300034da:	e7b7      	b.n	3000344c <FLASH_Calibration+0xa8>
300034dc:	9600      	str	r6, [sp, #0]
300034de:	e7d1      	b.n	30003484 <FLASH_Calibration+0xe0>
300034e0:	3000a541 	.word	0x3000a541
300034e4:	2001c01c 	.word	0x2001c01c
300034e8:	00009c99 	.word	0x00009c99
300034ec:	e000ed00 	.word	0xe000ed00
300034f0:	3000a351 	.word	0x3000a351
300034f4:	0000b7e9 	.word	0x0000b7e9
300034f8:	2001c00c 	.word	0x2001c00c
300034fc:	3000c354 	.word	0x3000c354
30003500:	3000c324 	.word	0x3000c324
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
3000359c:	f008 fe44 	bl	3000c228 <__DiagPrintf_veneer>
300035a0:	e7ec      	b.n	3000357c <_flash_calibration_highspeed+0x74>
300035a2:	bf00      	nop
300035a4:	2001c01c 	.word	0x2001c01c
300035a8:	2001c00c 	.word	0x2001c00c
300035ac:	3000c380 	.word	0x3000c380

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
300035e4:	f008 fe20 	bl	3000c228 <__DiagPrintf_veneer>
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
3000362a:	f008 fdfd 	bl	3000c228 <__DiagPrintf_veneer>
3000362e:	2001      	movs	r0, #1
30003630:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003632:	bf00      	nop
30003634:	2001c01c 	.word	0x2001c01c
30003638:	0000b569 	.word	0x0000b569
3000363c:	2001c00c 	.word	0x2001c00c
30003640:	3000c3e8 	.word	0x3000c3e8
30003644:	3000a541 	.word	0x3000a541
30003648:	0000c045 	.word	0x0000c045
3000364c:	0000a53d 	.word	0x0000a53d
30003650:	3000c3b8 	.word	0x3000c3b8

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
3000375a:	f008 fd65 	bl	3000c228 <__DiagPrintf_veneer>
3000375e:	e7cd      	b.n	300036fc <flash_rx_mode_switch+0xa8>
30003760:	469c      	mov	ip, r3
30003762:	f1bc 0f05 	cmp.w	ip, #5
30003766:	d10f      	bne.n	30003788 <flash_rx_mode_switch+0x134>
30003768:	4b12      	ldr	r3, [pc, #72]	; (300037b4 <flash_rx_mode_switch+0x160>)
3000376a:	6818      	ldr	r0, [r3, #0]
3000376c:	f010 0002 	ands.w	r0, r0, #2
30003770:	d00b      	beq.n	3000378a <flash_rx_mode_switch+0x136>
30003772:	4815      	ldr	r0, [pc, #84]	; (300037c8 <flash_rx_mode_switch+0x174>)
30003774:	f008 fd58 	bl	3000c228 <__DiagPrintf_veneer>
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
3000379c:	f008 fd44 	bl	3000c228 <__DiagPrintf_veneer>
300037a0:	e7f2      	b.n	30003788 <flash_rx_mode_switch+0x134>
300037a2:	bf00      	nop
300037a4:	3000d40c 	.word	0x3000d40c
300037a8:	2001c01c 	.word	0x2001c01c
300037ac:	e000ed00 	.word	0xe000ed00
300037b0:	00054060 	.word	0x00054060
300037b4:	2001c00c 	.word	0x2001c00c
300037b8:	0000c045 	.word	0x0000c045
300037bc:	00009db5 	.word	0x00009db5
300037c0:	0000a149 	.word	0x0000a149
300037c4:	3000c444 	.word	0x3000c444
300037c8:	3000c474 	.word	0x3000c474
300037cc:	3000c41c 	.word	0x3000c41c
300037d0:	3000a541 	.word	0x3000a541
300037d4:	3000a351 	.word	0x3000a351
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
300038a4:	f008 fcb0 	bl	3000c208 <__io_assert_failed_veneer>
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
30003920:	f008 fc72 	bl	3000c208 <__io_assert_failed_veneer>
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
30003a4a:	f008 fbed 	bl	3000c228 <__DiagPrintf_veneer>
30003a4e:	e6ff      	b.n	30003850 <flash_highspeed_setup+0x70>
30003a50:	f89d 200d 	ldrb.w	r2, [sp, #13]
30003a54:	f89d 100c 	ldrb.w	r1, [sp, #12]
30003a58:	481c      	ldr	r0, [pc, #112]	; (30003acc <flash_highspeed_setup+0x2ec>)
30003a5a:	f008 fbe5 	bl	3000c228 <__DiagPrintf_veneer>
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
30003a8c:	3000d37c 	.word	0x3000d37c
30003a90:	3000d37e 	.word	0x3000d37e
30003a94:	2001c01c 	.word	0x2001c01c
30003a98:	2001c00c 	.word	0x2001c00c
30003a9c:	3000d278 	.word	0x3000d278
30003aa0:	3000d380 	.word	0x3000d380
30003aa4:	3000d430 	.word	0x3000d430
30003aa8:	0000b8c1 	.word	0x0000b8c1
30003aac:	3000d420 	.word	0x3000d420
30003ab0:	0000bd0d 	.word	0x0000bd0d
30003ab4:	0000c0f9 	.word	0x0000c0f9
30003ab8:	0000c1d9 	.word	0x0000c1d9
30003abc:	3000eb10 	.word	0x3000eb10
30003ac0:	000129bd 	.word	0x000129bd
30003ac4:	0000a149 	.word	0x0000a149
30003ac8:	3000c4ac 	.word	0x3000c4ac
30003acc:	3000c500 	.word	0x3000c500
30003ad0:	0000be71 	.word	0x0000be71
30003ad4:	0000bee9 	.word	0x0000bee9
30003ad8:	0000bfcd 	.word	0x0000bfcd
30003adc:	3000a541 	.word	0x3000a541
30003ae0:	00009db5 	.word	0x00009db5
30003ae4:	3000eb14 	.word	0x3000eb14
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
30003b6a:	f008 fb4d 	bl	3000c208 <__io_assert_failed_veneer>
30003b6e:	f8d9 3000 	ldr.w	r3, [r9]
30003b72:	691b      	ldr	r3, [r3, #16]
30003b74:	e6e1      	b.n	3000393a <flash_highspeed_setup+0x15a>
30003b76:	f240 2149 	movw	r1, #585	; 0x249
30003b7a:	4812      	ldr	r0, [pc, #72]	; (30003bc4 <flash_highspeed_setup+0x3e4>)
30003b7c:	f008 fb44 	bl	3000c208 <__io_assert_failed_veneer>
30003b80:	f8d9 3000 	ldr.w	r3, [r9]
30003b84:	691b      	ldr	r3, [r3, #16]
30003b86:	e70f      	b.n	300039a8 <flash_highspeed_setup+0x1c8>
30003b88:	480f      	ldr	r0, [pc, #60]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b8a:	f008 fb4d 	bl	3000c228 <__DiagPrintf_veneer>
30003b8e:	e686      	b.n	3000389e <flash_highspeed_setup+0xbe>
30003b90:	480d      	ldr	r0, [pc, #52]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b92:	f008 fb49 	bl	3000c228 <__DiagPrintf_veneer>
30003b96:	e6c0      	b.n	3000391a <flash_highspeed_setup+0x13a>
30003b98:	4b0c      	ldr	r3, [pc, #48]	; (30003bcc <flash_highspeed_setup+0x3ec>)
30003b9a:	4798      	blx	r3
30003b9c:	e705      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b9e:	bf00      	nop
30003ba0:	2001c01c 	.word	0x2001c01c
30003ba4:	0000bf59 	.word	0x0000bf59
30003ba8:	0000a3c5 	.word	0x0000a3c5
30003bac:	0000a441 	.word	0x0000a441
30003bb0:	3000eb10 	.word	0x3000eb10
30003bb4:	0000a345 	.word	0x0000a345
30003bb8:	0000b991 	.word	0x0000b991
30003bbc:	0000bd0d 	.word	0x0000bd0d
30003bc0:	3000d420 	.word	0x3000d420
30003bc4:	3000d430 	.word	0x3000d430
30003bc8:	3000c4dc 	.word	0x3000c4dc
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
30003bf2:	f002 f87b 	bl	30005cec <BOOT_ImgCopy>
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
30003c4a:	f002 f84f 	bl	30005cec <BOOT_ImgCopy>
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
30003ca2:	f008 fac1 	bl	3000c228 <__DiagPrintf_veneer>
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
30003d0e:	f008 fa8b 	bl	3000c228 <__DiagPrintf_veneer>
30003d12:	2000      	movs	r0, #0
30003d14:	e7cc      	b.n	30003cb0 <BOOT_LoadSubImage+0xe0>
30003d16:	bf00      	nop
30003d18:	31313738 	.word	0x31313738
30003d1c:	f7ffffe0 	.word	0xf7ffffe0
30003d20:	2001c00c 	.word	0x2001c00c
30003d24:	3000e314 	.word	0x3000e314
30003d28:	e000ed00 	.word	0xe000ed00
30003d2c:	3000c5bc 	.word	0x3000c5bc
30003d30:	3000c594 	.word	0x3000c594
30003d34:	35393138 	.word	0x35393138

30003d38 <BOOT_LoadAPSubImage>:
30003d38:	4bb1      	ldr	r3, [pc, #708]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003d3a:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30003d3e:	460c      	mov	r4, r1
30003d40:	4606      	mov	r6, r0
30003d42:	b08a      	sub	sp, #40	; 0x28
30003d44:	4615      	mov	r5, r2
30003d46:	2220      	movs	r2, #32
30003d48:	e893 0003 	ldmia.w	r3, {r0, r1}
30003d4c:	0c0b      	lsrs	r3, r1, #16
30003d4e:	9000      	str	r0, [sp, #0]
30003d50:	f8ad 1004 	strh.w	r1, [sp, #4]
30003d54:	a802      	add	r0, sp, #8
30003d56:	4621      	mov	r1, r4
30003d58:	f88d 3006 	strb.w	r3, [sp, #6]
30003d5c:	f001 ffc6 	bl	30005cec <BOOT_ImgCopy>
30003d60:	4ba8      	ldr	r3, [pc, #672]	; (30004004 <BOOT_LoadAPSubImage+0x2cc>)
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
30003d80:	b00a      	add	sp, #40	; 0x28
30003d82:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003d86:	4ba0      	ldr	r3, [pc, #640]	; (30004008 <BOOT_LoadAPSubImage+0x2d0>)
30003d88:	6898      	ldr	r0, [r3, #8]
30003d8a:	f010 0002 	ands.w	r0, r0, #2
30003d8e:	d0f7      	beq.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d90:	4669      	mov	r1, sp
30003d92:	489e      	ldr	r0, [pc, #632]	; (3000400c <BOOT_LoadAPSubImage+0x2d4>)
30003d94:	f008 fa48 	bl	3000c228 <__DiagPrintf_veneer>
30003d98:	2000      	movs	r0, #0
30003d9a:	e7f1      	b.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d9c:	9a05      	ldr	r2, [sp, #20]
30003d9e:	4b9c      	ldr	r3, [pc, #624]	; (30004010 <BOOT_LoadAPSubImage+0x2d8>)
30003da0:	6ca1      	ldr	r1, [r4, #72]	; 0x48
30003da2:	f1a2 0520 	sub.w	r5, r2, #32
30003da6:	4413      	add	r3, r2
30003da8:	f101 08d0 	add.w	r8, r1, #208	; 0xd0
30003dac:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003db0:	d239      	bcs.n	30003e26 <BOOT_LoadAPSubImage+0xee>
30003db2:	4b98      	ldr	r3, [pc, #608]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003db4:	4445      	add	r5, r8
30003db6:	f8d4 9070 	ldr.w	r9, [r4, #112]	; 0x70
30003dba:	4798      	blx	r3
30003dbc:	4f92      	ldr	r7, [pc, #584]	; (30004008 <BOOT_LoadAPSubImage+0x2d0>)
30003dbe:	2801      	cmp	r0, #1
30003dc0:	44a0      	add	r8, r4
30003dc2:	68bb      	ldr	r3, [r7, #8]
30003dc4:	d01f      	beq.n	30003e06 <BOOT_LoadAPSubImage+0xce>
30003dc6:	079a      	lsls	r2, r3, #30
30003dc8:	f100 8083 	bmi.w	30003ed2 <BOOT_LoadAPSubImage+0x19a>
30003dcc:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003dd0:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003dd4:	f080 8081 	bcs.w	30003eda <BOOT_LoadAPSubImage+0x1a2>
30003dd8:	444d      	add	r5, r9
30003dda:	eb08 0109 	add.w	r1, r8, r9
30003dde:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003de2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003de6:	d249      	bcs.n	30003e7c <BOOT_LoadAPSubImage+0x144>
30003de8:	b14e      	cbz	r6, 30003dfe <BOOT_LoadAPSubImage+0xc6>
30003dea:	e9dd 3204 	ldrd	r3, r2, [sp, #16]
30003dee:	3a20      	subs	r2, #32
30003df0:	3320      	adds	r3, #32
30003df2:	e9c6 2300 	strd	r2, r3, [r6]
30003df6:	68b9      	ldr	r1, [r7, #8]
30003df8:	0789      	lsls	r1, r1, #30
30003dfa:	f100 80b5 	bmi.w	30003f68 <BOOT_LoadAPSubImage+0x230>
30003dfe:	2001      	movs	r0, #1
30003e00:	b00a      	add	sp, #40	; 0x28
30003e02:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003e06:	0798      	lsls	r0, r3, #30
30003e08:	f100 80b3 	bmi.w	30003f72 <BOOT_LoadAPSubImage+0x23a>
30003e0c:	464a      	mov	r2, r9
30003e0e:	4629      	mov	r1, r5
30003e10:	4640      	mov	r0, r8
30003e12:	f000 fc4d 	bl	300046b0 <BOOT_LoadBL32RDPImg>
30003e16:	2800      	cmp	r0, #0
30003e18:	d1de      	bne.n	30003dd8 <BOOT_LoadAPSubImage+0xa0>
30003e1a:	683b      	ldr	r3, [r7, #0]
30003e1c:	0799      	lsls	r1, r3, #30
30003e1e:	f100 80cb 	bmi.w	30003fb8 <BOOT_LoadAPSubImage+0x280>
30003e22:	20ff      	movs	r0, #255	; 0xff
30003e24:	e7ac      	b.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003e26:	4f7c      	ldr	r7, [pc, #496]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003e28:	4642      	mov	r2, r8
30003e2a:	4621      	mov	r1, r4
30003e2c:	4628      	mov	r0, r5
30003e2e:	f001 ff5d 	bl	30005cec <BOOT_ImgCopy>
30003e32:	697b      	ldr	r3, [r7, #20]
30003e34:	03db      	lsls	r3, r3, #15
30003e36:	d5bc      	bpl.n	30003db2 <BOOT_LoadAPSubImage+0x7a>
30003e38:	ea05 0308 	and.w	r3, r5, r8
30003e3c:	3301      	adds	r3, #1
30003e3e:	d077      	beq.n	30003f30 <BOOT_LoadAPSubImage+0x1f8>
30003e40:	f015 031f 	ands.w	r3, r5, #31
30003e44:	f000 8099 	beq.w	30003f7a <BOOT_LoadAPSubImage+0x242>
30003e48:	4443      	add	r3, r8
30003e4a:	f025 021f 	bic.w	r2, r5, #31
30003e4e:	f3bf 8f4f 	dsb	sy
30003e52:	2b00      	cmp	r3, #0
30003e54:	dd0d      	ble.n	30003e72 <BOOT_LoadAPSubImage+0x13a>
30003e56:	1e59      	subs	r1, r3, #1
30003e58:	f102 0320 	add.w	r3, r2, #32
30003e5c:	486e      	ldr	r0, [pc, #440]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003e5e:	f021 011f 	bic.w	r1, r1, #31
30003e62:	4419      	add	r1, r3
30003e64:	428b      	cmp	r3, r1
30003e66:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003e6a:	461a      	mov	r2, r3
30003e6c:	f103 0320 	add.w	r3, r3, #32
30003e70:	d1f8      	bne.n	30003e64 <BOOT_LoadAPSubImage+0x12c>
30003e72:	f3bf 8f4f 	dsb	sy
30003e76:	f3bf 8f6f 	isb	sy
30003e7a:	e79a      	b.n	30003db2 <BOOT_LoadAPSubImage+0x7a>
30003e7c:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30003e80:	4628      	mov	r0, r5
30003e82:	4622      	mov	r2, r4
30003e84:	f001 ff32 	bl	30005cec <BOOT_ImgCopy>
30003e88:	4963      	ldr	r1, [pc, #396]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003e8a:	694b      	ldr	r3, [r1, #20]
30003e8c:	03d8      	lsls	r0, r3, #15
30003e8e:	d5ab      	bpl.n	30003de8 <BOOT_LoadAPSubImage+0xb0>
30003e90:	ea05 0304 	and.w	r3, r5, r4
30003e94:	3301      	adds	r3, #1
30003e96:	d073      	beq.n	30003f80 <BOOT_LoadAPSubImage+0x248>
30003e98:	f015 031f 	ands.w	r3, r5, #31
30003e9c:	d002      	beq.n	30003ea4 <BOOT_LoadAPSubImage+0x16c>
30003e9e:	f025 051f 	bic.w	r5, r5, #31
30003ea2:	441c      	add	r4, r3
30003ea4:	f3bf 8f4f 	dsb	sy
30003ea8:	2c00      	cmp	r4, #0
30003eaa:	dd0d      	ble.n	30003ec8 <BOOT_LoadAPSubImage+0x190>
30003eac:	1e62      	subs	r2, r4, #1
30003eae:	f105 0320 	add.w	r3, r5, #32
30003eb2:	4959      	ldr	r1, [pc, #356]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003eb4:	f022 021f 	bic.w	r2, r2, #31
30003eb8:	441a      	add	r2, r3
30003eba:	4293      	cmp	r3, r2
30003ebc:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30003ec0:	461d      	mov	r5, r3
30003ec2:	f103 0320 	add.w	r3, r3, #32
30003ec6:	d1f8      	bne.n	30003eba <BOOT_LoadAPSubImage+0x182>
30003ec8:	f3bf 8f4f 	dsb	sy
30003ecc:	f3bf 8f6f 	isb	sy
30003ed0:	e78a      	b.n	30003de8 <BOOT_LoadAPSubImage+0xb0>
30003ed2:	4852      	ldr	r0, [pc, #328]	; (3000401c <BOOT_LoadAPSubImage+0x2e4>)
30003ed4:	f008 f9a8 	bl	3000c228 <__DiagPrintf_veneer>
30003ed8:	e778      	b.n	30003dcc <BOOT_LoadAPSubImage+0x94>
30003eda:	4641      	mov	r1, r8
30003edc:	464a      	mov	r2, r9
30003ede:	4628      	mov	r0, r5
30003ee0:	f001 ff04 	bl	30005cec <BOOT_ImgCopy>
30003ee4:	494c      	ldr	r1, [pc, #304]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003ee6:	694b      	ldr	r3, [r1, #20]
30003ee8:	03db      	lsls	r3, r3, #15
30003eea:	f57f af75 	bpl.w	30003dd8 <BOOT_LoadAPSubImage+0xa0>
30003eee:	ea05 0309 	and.w	r3, r5, r9
30003ef2:	3301      	adds	r3, #1
30003ef4:	d064      	beq.n	30003fc0 <BOOT_LoadAPSubImage+0x288>
30003ef6:	f015 031f 	ands.w	r3, r5, #31
30003efa:	d07d      	beq.n	30003ff8 <BOOT_LoadAPSubImage+0x2c0>
30003efc:	444b      	add	r3, r9
30003efe:	f025 021f 	bic.w	r2, r5, #31
30003f02:	f3bf 8f4f 	dsb	sy
30003f06:	2b00      	cmp	r3, #0
30003f08:	dd0d      	ble.n	30003f26 <BOOT_LoadAPSubImage+0x1ee>
30003f0a:	1e59      	subs	r1, r3, #1
30003f0c:	f102 0320 	add.w	r3, r2, #32
30003f10:	4841      	ldr	r0, [pc, #260]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003f12:	f021 011f 	bic.w	r1, r1, #31
30003f16:	4419      	add	r1, r3
30003f18:	428b      	cmp	r3, r1
30003f1a:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003f1e:	461a      	mov	r2, r3
30003f20:	f103 0320 	add.w	r3, r3, #32
30003f24:	d1f8      	bne.n	30003f18 <BOOT_LoadAPSubImage+0x1e0>
30003f26:	f3bf 8f4f 	dsb	sy
30003f2a:	f3bf 8f6f 	isb	sy
30003f2e:	e753      	b.n	30003dd8 <BOOT_LoadAPSubImage+0xa0>
30003f30:	2300      	movs	r3, #0
30003f32:	f8c7 3084 	str.w	r3, [r7, #132]	; 0x84
30003f36:	f3bf 8f4f 	dsb	sy
30003f3a:	f8d7 0080 	ldr.w	r0, [r7, #128]	; 0x80
30003f3e:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003f42:	f3c0 324e 	ubfx	r2, r0, #13, #15
30003f46:	f3c0 00c9 	ubfx	r0, r0, #3, #10
30003f4a:	0152      	lsls	r2, r2, #5
30003f4c:	ea02 0c0e 	and.w	ip, r2, lr
30003f50:	4603      	mov	r3, r0
30003f52:	ea4c 7183 	orr.w	r1, ip, r3, lsl #30
30003f56:	3b01      	subs	r3, #1
30003f58:	f8c7 1274 	str.w	r1, [r7, #628]	; 0x274
30003f5c:	d2f9      	bcs.n	30003f52 <BOOT_LoadAPSubImage+0x21a>
30003f5e:	3a20      	subs	r2, #32
30003f60:	f112 0f20 	cmn.w	r2, #32
30003f64:	d1f2      	bne.n	30003f4c <BOOT_LoadAPSubImage+0x214>
30003f66:	e784      	b.n	30003e72 <BOOT_LoadAPSubImage+0x13a>
30003f68:	4669      	mov	r1, sp
30003f6a:	482d      	ldr	r0, [pc, #180]	; (30004020 <BOOT_LoadAPSubImage+0x2e8>)
30003f6c:	f008 f95c 	bl	3000c228 <__DiagPrintf_veneer>
30003f70:	e745      	b.n	30003dfe <BOOT_LoadAPSubImage+0xc6>
30003f72:	482c      	ldr	r0, [pc, #176]	; (30004024 <BOOT_LoadAPSubImage+0x2ec>)
30003f74:	f008 f958 	bl	3000c228 <__DiagPrintf_veneer>
30003f78:	e748      	b.n	30003e0c <BOOT_LoadAPSubImage+0xd4>
30003f7a:	4643      	mov	r3, r8
30003f7c:	462a      	mov	r2, r5
30003f7e:	e766      	b.n	30003e4e <BOOT_LoadAPSubImage+0x116>
30003f80:	2300      	movs	r3, #0
30003f82:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003f86:	f3bf 8f4f 	dsb	sy
30003f8a:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30003f8e:	f643 75e0 	movw	r5, #16352	; 0x3fe0
30003f92:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30003f96:	f3c3 334e 	ubfx	r3, r3, #13, #15
30003f9a:	015c      	lsls	r4, r3, #5
30003f9c:	ea04 0005 	and.w	r0, r4, r5
30003fa0:	4663      	mov	r3, ip
30003fa2:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30003fa6:	3b01      	subs	r3, #1
30003fa8:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30003fac:	d2f9      	bcs.n	30003fa2 <BOOT_LoadAPSubImage+0x26a>
30003fae:	3c20      	subs	r4, #32
30003fb0:	f114 0f20 	cmn.w	r4, #32
30003fb4:	d1f2      	bne.n	30003f9c <BOOT_LoadAPSubImage+0x264>
30003fb6:	e787      	b.n	30003ec8 <BOOT_LoadAPSubImage+0x190>
30003fb8:	481b      	ldr	r0, [pc, #108]	; (30004028 <BOOT_LoadAPSubImage+0x2f0>)
30003fba:	f008 f935 	bl	3000c228 <__DiagPrintf_veneer>
30003fbe:	e730      	b.n	30003e22 <BOOT_LoadAPSubImage+0xea>
30003fc0:	2300      	movs	r3, #0
30003fc2:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003fc6:	f3bf 8f4f 	dsb	sy
30003fca:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30003fce:	f643 7ae0 	movw	sl, #16352	; 0x3fe0
30003fd2:	f3c3 324e 	ubfx	r2, r3, #13, #15
30003fd6:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30003fda:	0152      	lsls	r2, r2, #5
30003fdc:	ea02 0c0a 	and.w	ip, r2, sl
30003fe0:	4673      	mov	r3, lr
30003fe2:	ea4c 7083 	orr.w	r0, ip, r3, lsl #30
30003fe6:	3b01      	subs	r3, #1
30003fe8:	f8c1 0274 	str.w	r0, [r1, #628]	; 0x274
30003fec:	d2f9      	bcs.n	30003fe2 <BOOT_LoadAPSubImage+0x2aa>
30003fee:	3a20      	subs	r2, #32
30003ff0:	f112 0f20 	cmn.w	r2, #32
30003ff4:	d1f2      	bne.n	30003fdc <BOOT_LoadAPSubImage+0x2a4>
30003ff6:	e796      	b.n	30003f26 <BOOT_LoadAPSubImage+0x1ee>
30003ff8:	464b      	mov	r3, r9
30003ffa:	462a      	mov	r2, r5
30003ffc:	e781      	b.n	30003f02 <BOOT_LoadAPSubImage+0x1ca>
30003ffe:	bf00      	nop
30004000:	3000c58c 	.word	0x3000c58c
30004004:	35393138 	.word	0x35393138
30004008:	2001c00c 	.word	0x2001c00c
3000400c:	3000c594 	.word	0x3000c594
30004010:	f7ffffe0 	.word	0xf7ffffe0
30004014:	0000c109 	.word	0x0000c109
30004018:	e000ed00 	.word	0xe000ed00
3000401c:	3000c608 	.word	0x3000c608
30004020:	3000c5bc 	.word	0x3000c5bc
30004024:	3000c5e4 	.word	0x3000c5e4
30004028:	3000c62c 	.word	0x3000c62c

3000402c <BOOT_RSIPIvSet>:
3000402c:	b530      	push	{r4, r5, lr}
3000402e:	2300      	movs	r3, #0
30004030:	b083      	sub	sp, #12
30004032:	4a09      	ldr	r2, [pc, #36]	; (30004058 <BOOT_RSIPIvSet+0x2c>)
30004034:	4605      	mov	r5, r0
30004036:	460c      	mov	r4, r1
30004038:	9300      	str	r3, [sp, #0]
3000403a:	9301      	str	r3, [sp, #4]
3000403c:	4790      	blx	r2
3000403e:	b148      	cbz	r0, 30004054 <BOOT_RSIPIvSet+0x28>
30004040:	f105 0120 	add.w	r1, r5, #32
30004044:	4b05      	ldr	r3, [pc, #20]	; (3000405c <BOOT_RSIPIvSet+0x30>)
30004046:	2208      	movs	r2, #8
30004048:	4668      	mov	r0, sp
3000404a:	4798      	blx	r3
3000404c:	4669      	mov	r1, sp
3000404e:	b2e0      	uxtb	r0, r4
30004050:	4b03      	ldr	r3, [pc, #12]	; (30004060 <BOOT_RSIPIvSet+0x34>)
30004052:	4798      	blx	r3
30004054:	b003      	add	sp, #12
30004056:	bd30      	pop	{r4, r5, pc}
30004058:	0000c129 	.word	0x0000c129
3000405c:	00012a1d 	.word	0x00012a1d
30004060:	3000a239 	.word	0x3000a239

30004064 <BOOT_OTFCheck>:
30004064:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004068:	4d1d      	ldr	r5, [pc, #116]	; (300040e0 <BOOT_OTFCheck+0x7c>)
3000406a:	b082      	sub	sp, #8
3000406c:	4604      	mov	r4, r0
3000406e:	4688      	mov	r8, r1
30004070:	4617      	mov	r7, r2
30004072:	461e      	mov	r6, r3
30004074:	47a8      	blx	r5
30004076:	b1b0      	cbz	r0, 300040a6 <BOOT_OTFCheck+0x42>
30004078:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
3000407c:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004080:	d211      	bcs.n	300040a6 <BOOT_OTFCheck+0x42>
30004082:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30004086:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000408a:	d20c      	bcs.n	300040a6 <BOOT_OTFCheck+0x42>
3000408c:	4d15      	ldr	r5, [pc, #84]	; (300040e4 <BOOT_OTFCheck+0x80>)
3000408e:	682b      	ldr	r3, [r5, #0]
30004090:	039a      	lsls	r2, r3, #14
30004092:	d420      	bmi.n	300040d6 <BOOT_OTFCheck+0x72>
30004094:	4b14      	ldr	r3, [pc, #80]	; (300040e8 <BOOT_OTFCheck+0x84>)
30004096:	4798      	blx	r3
30004098:	4603      	mov	r3, r0
3000409a:	b138      	cbz	r0, 300040ac <BOOT_OTFCheck+0x48>
3000409c:	2801      	cmp	r0, #1
3000409e:	d006      	beq.n	300040ae <BOOT_OTFCheck+0x4a>
300040a0:	682b      	ldr	r3, [r5, #0]
300040a2:	039b      	lsls	r3, r3, #14
300040a4:	d411      	bmi.n	300040ca <BOOT_OTFCheck+0x66>
300040a6:	b002      	add	sp, #8
300040a8:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300040ac:	2302      	movs	r3, #2
300040ae:	4630      	mov	r0, r6
300040b0:	4621      	mov	r1, r4
300040b2:	9301      	str	r3, [sp, #4]
300040b4:	4642      	mov	r2, r8
300040b6:	2301      	movs	r3, #1
300040b8:	4c0c      	ldr	r4, [pc, #48]	; (300040ec <BOOT_OTFCheck+0x88>)
300040ba:	9700      	str	r7, [sp, #0]
300040bc:	47a0      	blx	r4
300040be:	2001      	movs	r0, #1
300040c0:	4b0b      	ldr	r3, [pc, #44]	; (300040f0 <BOOT_OTFCheck+0x8c>)
300040c2:	b002      	add	sp, #8
300040c4:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040c8:	4718      	bx	r3
300040ca:	480a      	ldr	r0, [pc, #40]	; (300040f4 <BOOT_OTFCheck+0x90>)
300040cc:	b002      	add	sp, #8
300040ce:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040d2:	f008 b8a9 	b.w	3000c228 <__DiagPrintf_veneer>
300040d6:	4808      	ldr	r0, [pc, #32]	; (300040f8 <BOOT_OTFCheck+0x94>)
300040d8:	f008 f8a6 	bl	3000c228 <__DiagPrintf_veneer>
300040dc:	e7da      	b.n	30004094 <BOOT_OTFCheck+0x30>
300040de:	bf00      	nop
300040e0:	0000c129 	.word	0x0000c129
300040e4:	2001c00c 	.word	0x2001c00c
300040e8:	0000c245 	.word	0x0000c245
300040ec:	3000a26d 	.word	0x3000a26d
300040f0:	3000a255 	.word	0x3000a255
300040f4:	3000c670 	.word	0x3000c670
300040f8:	3000c660 	.word	0x3000c660

300040fc <BOOT_OTA_LoadIMG2>:
300040fc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004100:	4b6e      	ldr	r3, [pc, #440]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c0>)
30004102:	b08d      	sub	sp, #52	; 0x34
30004104:	4a6e      	ldr	r2, [pc, #440]	; (300042c0 <BOOT_OTA_LoadIMG2+0x1c4>)
30004106:	f103 040c 	add.w	r4, r3, #12
3000410a:	9003      	str	r0, [sp, #12]
3000410c:	f8d2 8000 	ldr.w	r8, [r2]
30004110:	ae06      	add	r6, sp, #24
30004112:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30004116:	ab09      	add	r3, sp, #36	; 0x24
30004118:	e886 0007 	stmia.w	r6, {r0, r1, r2}
3000411c:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30004120:	e883 0007 	stmia.w	r3, {r0, r1, r2}
30004124:	4641      	mov	r1, r8
30004126:	4867      	ldr	r0, [pc, #412]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1c8>)
30004128:	f44f 72b2 	mov.w	r2, #356	; 0x164
3000412c:	f001 fdde 	bl	30005cec <BOOT_ImgCopy>
30004130:	4641      	mov	r1, r8
30004132:	4864      	ldr	r0, [pc, #400]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1c8>)
30004134:	f000 fda6 	bl	30004c84 <BOOT_CertificateCheck>
30004138:	2800      	cmp	r0, #0
3000413a:	f040 80b7 	bne.w	300042ac <BOOT_OTA_LoadIMG2+0x1b0>
3000413e:	4604      	mov	r4, r0
30004140:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004144:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30004148:	485f      	ldr	r0, [pc, #380]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1cc>)
3000414a:	f001 fdcf 	bl	30005cec <BOOT_ImgCopy>
3000414e:	4b5f      	ldr	r3, [pc, #380]	; (300042cc <BOOT_OTA_LoadIMG2+0x1d0>)
30004150:	e9cd 4404 	strd	r4, r4, [sp, #16]
30004154:	4798      	blx	r3
30004156:	2800      	cmp	r0, #0
30004158:	f040 809e 	bne.w	30004298 <BOOT_OTA_LoadIMG2+0x19c>
3000415c:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30004160:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30004310 <BOOT_OTA_LoadIMG2+0x214>
30004164:	4a5a      	ldr	r2, [pc, #360]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1d4>)
30004166:	2000      	movs	r0, #0
30004168:	4445      	add	r5, r8
3000416a:	4659      	mov	r1, fp
3000416c:	4f59      	ldr	r7, [pc, #356]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1d8>)
3000416e:	f04f 0801 	mov.w	r8, #1
30004172:	462b      	mov	r3, r5
30004174:	f8df 919c 	ldr.w	r9, [pc, #412]	; 30004314 <BOOT_OTA_LoadIMG2+0x218>
30004178:	47b8      	blx	r7
3000417a:	2101      	movs	r1, #1
3000417c:	2000      	movs	r0, #0
3000417e:	4b56      	ldr	r3, [pc, #344]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1dc>)
30004180:	4798      	blx	r3
30004182:	2301      	movs	r3, #1
30004184:	4952      	ldr	r1, [pc, #328]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1d4>)
30004186:	4658      	mov	r0, fp
30004188:	461a      	mov	r2, r3
3000418a:	f7ff ff6b 	bl	30004064 <BOOT_OTFCheck>
3000418e:	47c8      	blx	r9
30004190:	2800      	cmp	r0, #0
30004192:	bf0c      	ite	eq
30004194:	46aa      	moveq	sl, r5
30004196:	46da      	movne	sl, fp
30004198:	4633      	mov	r3, r6
3000419a:	2203      	movs	r2, #3
3000419c:	4651      	mov	r1, sl
3000419e:	484f      	ldr	r0, [pc, #316]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e0>)
300041a0:	f8cd 8000 	str.w	r8, [sp]
300041a4:	f7ff fd14 	bl	30003bd0 <BOOT_LoadSubImage>
300041a8:	2800      	cmp	r0, #0
300041aa:	d070      	beq.n	3000428e <BOOT_OTA_LoadIMG2+0x192>
300041ac:	4e4b      	ldr	r6, [pc, #300]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e0>)
300041ae:	47c8      	blx	r9
300041b0:	2800      	cmp	r0, #0
300041b2:	bf08      	it	eq
300041b4:	46ab      	moveq	fp, r5
300041b6:	4f47      	ldr	r7, [pc, #284]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1d8>)
300041b8:	68f2      	ldr	r2, [r6, #12]
300041ba:	4640      	mov	r0, r8
300041bc:	6873      	ldr	r3, [r6, #4]
300041be:	f8c6 b000 	str.w	fp, [r6]
300041c2:	4413      	add	r3, r2
300041c4:	6972      	ldr	r2, [r6, #20]
300041c6:	f8df b150 	ldr.w	fp, [pc, #336]	; 30004318 <BOOT_OTA_LoadIMG2+0x21c>
300041ca:	441a      	add	r2, r3
300041cc:	4b44      	ldr	r3, [pc, #272]	; (300042e0 <BOOT_OTA_LoadIMG2+0x1e4>)
300041ce:	eb0a 0102 	add.w	r1, sl, r2
300041d2:	61b1      	str	r1, [r6, #24]
300041d4:	1e51      	subs	r1, r2, #1
300041d6:	400b      	ands	r3, r1
300041d8:	4659      	mov	r1, fp
300041da:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
300041de:	441d      	add	r5, r3
300041e0:	1a9b      	subs	r3, r3, r2
300041e2:	4a40      	ldr	r2, [pc, #256]	; (300042e4 <BOOT_OTA_LoadIMG2+0x1e8>)
300041e4:	61f3      	str	r3, [r6, #28]
300041e6:	462b      	mov	r3, r5
300041e8:	47b8      	blx	r7
300041ea:	4641      	mov	r1, r8
300041ec:	4640      	mov	r0, r8
300041ee:	4b3a      	ldr	r3, [pc, #232]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1dc>)
300041f0:	4798      	blx	r3
300041f2:	4642      	mov	r2, r8
300041f4:	2302      	movs	r3, #2
300041f6:	493b      	ldr	r1, [pc, #236]	; (300042e4 <BOOT_OTA_LoadIMG2+0x1e8>)
300041f8:	4658      	mov	r0, fp
300041fa:	f7ff ff33 	bl	30004064 <BOOT_OTFCheck>
300041fe:	47c8      	blx	r9
30004200:	2800      	cmp	r0, #0
30004202:	bf0c      	ite	eq
30004204:	462f      	moveq	r7, r5
30004206:	465f      	movne	r7, fp
30004208:	ab09      	add	r3, sp, #36	; 0x24
3000420a:	2203      	movs	r2, #3
3000420c:	f106 0020 	add.w	r0, r6, #32
30004210:	4639      	mov	r1, r7
30004212:	f8cd 8000 	str.w	r8, [sp]
30004216:	f7ff fcdb 	bl	30003bd0 <BOOT_LoadSubImage>
3000421a:	2800      	cmp	r0, #0
3000421c:	d037      	beq.n	3000428e <BOOT_OTA_LoadIMG2+0x192>
3000421e:	47c8      	blx	r9
30004220:	6af2      	ldr	r2, [r6, #44]	; 0x2c
30004222:	6a73      	ldr	r3, [r6, #36]	; 0x24
30004224:	2800      	cmp	r0, #0
30004226:	bf08      	it	eq
30004228:	46ab      	moveq	fp, r5
3000422a:	4827      	ldr	r0, [pc, #156]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1cc>)
3000422c:	4413      	add	r3, r2
3000422e:	6b72      	ldr	r2, [r6, #52]	; 0x34
30004230:	f8c6 b020 	str.w	fp, [r6, #32]
30004234:	4413      	add	r3, r2
30004236:	4a2a      	ldr	r2, [pc, #168]	; (300042e0 <BOOT_OTA_LoadIMG2+0x1e4>)
30004238:	1e59      	subs	r1, r3, #1
3000423a:	441f      	add	r7, r3
3000423c:	400a      	ands	r2, r1
3000423e:	2100      	movs	r1, #0
30004240:	63b7      	str	r7, [r6, #56]	; 0x38
30004242:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30004246:	9100      	str	r1, [sp, #0]
30004248:	1ad1      	subs	r1, r2, r3
3000424a:	2900      	cmp	r1, #0
3000424c:	63f1      	str	r1, [r6, #60]	; 0x3c
3000424e:	4631      	mov	r1, r6
30004250:	bf18      	it	ne
30004252:	4613      	movne	r3, r2
30004254:	4a24      	ldr	r2, [pc, #144]	; (300042e8 <BOOT_OTA_LoadIMG2+0x1ec>)
30004256:	441d      	add	r5, r3
30004258:	4b1a      	ldr	r3, [pc, #104]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1c8>)
3000425a:	6015      	str	r5, [r2, #0]
3000425c:	bf0c      	ite	eq
3000425e:	2207      	moveq	r2, #7
30004260:	2208      	movne	r2, #8
30004262:	f000 fbfb 	bl	30004a5c <BOOT_SignatureCheck>
30004266:	4b21      	ldr	r3, [pc, #132]	; (300042ec <BOOT_OTA_LoadIMG2+0x1f0>)
30004268:	4605      	mov	r5, r0
3000426a:	689b      	ldr	r3, [r3, #8]
3000426c:	079b      	lsls	r3, r3, #30
3000426e:	d50a      	bpl.n	30004286 <BOOT_OTA_LoadIMG2+0x18a>
30004270:	4b1f      	ldr	r3, [pc, #124]	; (300042f0 <BOOT_OTA_LoadIMG2+0x1f4>)
30004272:	4a20      	ldr	r2, [pc, #128]	; (300042f4 <BOOT_OTA_LoadIMG2+0x1f8>)
30004274:	9903      	ldr	r1, [sp, #12]
30004276:	2800      	cmp	r0, #0
30004278:	bf18      	it	ne
3000427a:	461a      	movne	r2, r3
3000427c:	481e      	ldr	r0, [pc, #120]	; (300042f8 <BOOT_OTA_LoadIMG2+0x1fc>)
3000427e:	3101      	adds	r1, #1
30004280:	462b      	mov	r3, r5
30004282:	f007 ffd1 	bl	3000c228 <__DiagPrintf_veneer>
30004286:	b11d      	cbz	r5, 30004290 <BOOT_OTA_LoadIMG2+0x194>
30004288:	481c      	ldr	r0, [pc, #112]	; (300042fc <BOOT_OTA_LoadIMG2+0x200>)
3000428a:	f007 ffcd 	bl	3000c228 <__DiagPrintf_veneer>
3000428e:	24ff      	movs	r4, #255	; 0xff
30004290:	4620      	mov	r0, r4
30004292:	b00d      	add	sp, #52	; 0x34
30004294:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004298:	2208      	movs	r2, #8
3000429a:	4919      	ldr	r1, [pc, #100]	; (30004300 <BOOT_OTA_LoadIMG2+0x204>)
3000429c:	4b19      	ldr	r3, [pc, #100]	; (30004304 <BOOT_OTA_LoadIMG2+0x208>)
3000429e:	a804      	add	r0, sp, #16
300042a0:	4798      	blx	r3
300042a2:	a904      	add	r1, sp, #16
300042a4:	2001      	movs	r0, #1
300042a6:	4b18      	ldr	r3, [pc, #96]	; (30004308 <BOOT_OTA_LoadIMG2+0x20c>)
300042a8:	4798      	blx	r3
300042aa:	e757      	b.n	3000415c <BOOT_OTA_LoadIMG2+0x60>
300042ac:	24ff      	movs	r4, #255	; 0xff
300042ae:	4817      	ldr	r0, [pc, #92]	; (3000430c <BOOT_OTA_LoadIMG2+0x210>)
300042b0:	f007 ffba 	bl	3000c228 <__DiagPrintf_veneer>
300042b4:	4620      	mov	r0, r4
300042b6:	b00d      	add	sp, #52	; 0x34
300042b8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300042bc:	3000d75c 	.word	0x3000d75c
300042c0:	3000eea0 	.word	0x3000eea0
300042c4:	3000eb44 	.word	0x3000eb44
300042c8:	3000eca8 	.word	0x3000eca8
300042cc:	0000c129 	.word	0x0000c129
300042d0:	0cffffe0 	.word	0x0cffffe0
300042d4:	3000a2e9 	.word	0x3000a2e9
300042d8:	3000a329 	.word	0x3000a329
300042dc:	3000ee50 	.word	0x3000ee50
300042e0:	fffff000 	.word	0xfffff000
300042e4:	0effffe0 	.word	0x0effffe0
300042e8:	3000e314 	.word	0x3000e314
300042ec:	2001c00c 	.word	0x2001c00c
300042f0:	3000c68c 	.word	0x3000c68c
300042f4:	3000c684 	.word	0x3000c684
300042f8:	3000c6bc 	.word	0x3000c6bc
300042fc:	3000c6f0 	.word	0x3000c6f0
30004300:	3000ecc8 	.word	0x3000ecc8
30004304:	00012a1d 	.word	0x00012a1d
30004308:	3000a239 	.word	0x3000a239
3000430c:	3000c694 	.word	0x3000c694
30004310:	0c000000 	.word	0x0c000000
30004314:	3000a541 	.word	0x3000a541
30004318:	0e000000 	.word	0x0e000000

3000431c <BOOT_OTA_Region_Init>:
3000431c:	4b24      	ldr	r3, [pc, #144]	; (300043b0 <BOOT_OTA_Region_Init+0x94>)
3000431e:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004322:	4798      	blx	r3
30004324:	4b23      	ldr	r3, [pc, #140]	; (300043b4 <BOOT_OTA_Region_Init+0x98>)
30004326:	4a24      	ldr	r2, [pc, #144]	; (300043b8 <BOOT_OTA_Region_Init+0x9c>)
30004328:	4924      	ldr	r1, [pc, #144]	; (300043bc <BOOT_OTA_Region_Init+0xa0>)
3000432a:	2800      	cmp	r0, #0
3000432c:	bf08      	it	eq
3000432e:	461a      	moveq	r2, r3
30004330:	6813      	ldr	r3, [r2, #0]
30004332:	2bff      	cmp	r3, #255	; 0xff
30004334:	e9d1 0400 	ldrd	r0, r4, [r1]
30004338:	d029      	beq.n	3000438e <BOOT_OTA_Region_Init+0x72>
3000433a:	2500      	movs	r5, #0
3000433c:	e9d1 e804 	ldrd	lr, r8, [r1, #16]
30004340:	46ac      	mov	ip, r5
30004342:	462f      	mov	r7, r5
30004344:	462e      	mov	r6, r5
30004346:	e00c      	b.n	30004362 <BOOT_OTA_Region_Init+0x46>
30004348:	2b03      	cmp	r3, #3
3000434a:	d028      	beq.n	3000439e <BOOT_OTA_Region_Init+0x82>
3000434c:	2b04      	cmp	r3, #4
3000434e:	d029      	beq.n	300043a4 <BOOT_OTA_Region_Init+0x88>
30004350:	2b05      	cmp	r3, #5
30004352:	d102      	bne.n	3000435a <BOOT_OTA_Region_Init+0x3e>
30004354:	f8d2 8004 	ldr.w	r8, [r2, #4]
30004358:	2501      	movs	r5, #1
3000435a:	f852 3f0c 	ldr.w	r3, [r2, #12]!
3000435e:	2bff      	cmp	r3, #255	; 0xff
30004360:	d007      	beq.n	30004372 <BOOT_OTA_Region_Init+0x56>
30004362:	2b02      	cmp	r3, #2
30004364:	d1f0      	bne.n	30004348 <BOOT_OTA_Region_Init+0x2c>
30004366:	6850      	ldr	r0, [r2, #4]
30004368:	2601      	movs	r6, #1
3000436a:	f852 3f0c 	ldr.w	r3, [r2, #12]!
3000436e:	2bff      	cmp	r3, #255	; 0xff
30004370:	d1f7      	bne.n	30004362 <BOOT_OTA_Region_Init+0x46>
30004372:	b106      	cbz	r6, 30004376 <BOOT_OTA_Region_Init+0x5a>
30004374:	6008      	str	r0, [r1, #0]
30004376:	b107      	cbz	r7, 3000437a <BOOT_OTA_Region_Init+0x5e>
30004378:	604c      	str	r4, [r1, #4]
3000437a:	f1bc 0f00 	cmp.w	ip, #0
3000437e:	d001      	beq.n	30004384 <BOOT_OTA_Region_Init+0x68>
30004380:	f8c1 e010 	str.w	lr, [r1, #16]
30004384:	e9d1 0400 	ldrd	r0, r4, [r1]
30004388:	b10d      	cbz	r5, 3000438e <BOOT_OTA_Region_Init+0x72>
3000438a:	f8c1 8014 	str.w	r8, [r1, #20]
3000438e:	f500 5080 	add.w	r0, r0, #4096	; 0x1000
30004392:	f504 5480 	add.w	r4, r4, #4096	; 0x1000
30004396:	e9c1 0402 	strd	r0, r4, [r1, #8]
3000439a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000439e:	6854      	ldr	r4, [r2, #4]
300043a0:	2701      	movs	r7, #1
300043a2:	e7da      	b.n	3000435a <BOOT_OTA_Region_Init+0x3e>
300043a4:	f8d2 e004 	ldr.w	lr, [r2, #4]
300043a8:	f04f 0c01 	mov.w	ip, #1
300043ac:	e7d5      	b.n	3000435a <BOOT_OTA_Region_Init+0x3e>
300043ae:	bf00      	nop
300043b0:	3000a541 	.word	0x3000a541
300043b4:	3000e43c 	.word	0x3000e43c
300043b8:	3000e4b4 	.word	0x3000e4b4
300043bc:	3000ee38 	.word	0x3000ee38

300043c0 <BOOT_OTA_IMG2>:
300043c0:	f7ff be9c 	b.w	300040fc <BOOT_OTA_LoadIMG2>

300043c4 <BOOT_OTA_AP>:
300043c4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300043c8:	4c31      	ldr	r4, [pc, #196]	; (30004490 <BOOT_OTA_AP+0xcc>)
300043ca:	b0ed      	sub	sp, #436	; 0x1b4
300043cc:	4b31      	ldr	r3, [pc, #196]	; (30004494 <BOOT_OTA_AP+0xd0>)
300043ce:	ad05      	add	r5, sp, #20
300043d0:	af03      	add	r7, sp, #12
300043d2:	681b      	ldr	r3, [r3, #0]
300043d4:	4e30      	ldr	r6, [pc, #192]	; (30004498 <BOOT_OTA_AP+0xd4>)
300043d6:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
300043da:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
300043de:	340c      	adds	r4, #12
300043e0:	e885 0007 	stmia.w	r5, {r0, r1, r2}
300043e4:	e894 0003 	ldmia.w	r4, {r0, r1}
300043e8:	f44f 72c8 	mov.w	r2, #400	; 0x190
300043ec:	e887 0003 	stmia.w	r7, {r0, r1}
300043f0:	4619      	mov	r1, r3
300043f2:	a808      	add	r0, sp, #32
300043f4:	f001 fc7a 	bl	30005cec <BOOT_ImgCopy>
300043f8:	2201      	movs	r2, #1
300043fa:	462b      	mov	r3, r5
300043fc:	4641      	mov	r1, r8
300043fe:	9200      	str	r2, [sp, #0]
30004400:	4630      	mov	r0, r6
30004402:	2203      	movs	r2, #3
30004404:	f7ff fbe4 	bl	30003bd0 <BOOT_LoadSubImage>
30004408:	2801      	cmp	r0, #1
3000440a:	d122      	bne.n	30004452 <BOOT_OTA_AP+0x8e>
3000440c:	f04f 0902 	mov.w	r9, #2
30004410:	4604      	mov	r4, r0
30004412:	4b22      	ldr	r3, [pc, #136]	; (3000449c <BOOT_OTA_AP+0xd8>)
30004414:	a808      	add	r0, sp, #32
30004416:	2203      	movs	r2, #3
30004418:	4631      	mov	r1, r6
3000441a:	f8cd 9000 	str.w	r9, [sp]
3000441e:	f000 fb1d 	bl	30004a5c <BOOT_SignatureCheck>
30004422:	4605      	mov	r5, r0
30004424:	bb30      	cbnz	r0, 30004474 <BOOT_OTA_AP+0xb0>
30004426:	464a      	mov	r2, r9
30004428:	463b      	mov	r3, r7
3000442a:	4641      	mov	r1, r8
3000442c:	4630      	mov	r0, r6
3000442e:	9400      	str	r4, [sp, #0]
30004430:	f7ff fbce 	bl	30003bd0 <BOOT_LoadSubImage>
30004434:	6871      	ldr	r1, [r6, #4]
30004436:	68f3      	ldr	r3, [r6, #12]
30004438:	4622      	mov	r2, r4
3000443a:	f106 0010 	add.w	r0, r6, #16
3000443e:	4419      	add	r1, r3
30004440:	4441      	add	r1, r8
30004442:	f7ff fc79 	bl	30003d38 <BOOT_LoadAPSubImage>
30004446:	2801      	cmp	r0, #1
30004448:	d10b      	bne.n	30004462 <BOOT_OTA_AP+0x9e>
3000444a:	4628      	mov	r0, r5
3000444c:	b06d      	add	sp, #436	; 0x1b4
3000444e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004452:	25ff      	movs	r5, #255	; 0xff
30004454:	4812      	ldr	r0, [pc, #72]	; (300044a0 <BOOT_OTA_AP+0xdc>)
30004456:	f007 fee7 	bl	3000c228 <__DiagPrintf_veneer>
3000445a:	4628      	mov	r0, r5
3000445c:	b06d      	add	sp, #436	; 0x1b4
3000445e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004462:	4b10      	ldr	r3, [pc, #64]	; (300044a4 <BOOT_OTA_AP+0xe0>)
30004464:	681b      	ldr	r3, [r3, #0]
30004466:	079b      	lsls	r3, r3, #30
30004468:	d40c      	bmi.n	30004484 <BOOT_OTA_AP+0xc0>
3000446a:	25ff      	movs	r5, #255	; 0xff
3000446c:	4628      	mov	r0, r5
3000446e:	b06d      	add	sp, #436	; 0x1b4
30004470:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004474:	25ff      	movs	r5, #255	; 0xff
30004476:	480c      	ldr	r0, [pc, #48]	; (300044a8 <BOOT_OTA_AP+0xe4>)
30004478:	f007 fed6 	bl	3000c228 <__DiagPrintf_veneer>
3000447c:	4628      	mov	r0, r5
3000447e:	b06d      	add	sp, #436	; 0x1b4
30004480:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004484:	4809      	ldr	r0, [pc, #36]	; (300044ac <BOOT_OTA_AP+0xe8>)
30004486:	25ff      	movs	r5, #255	; 0xff
30004488:	f007 fece 	bl	3000c228 <__DiagPrintf_veneer>
3000448c:	e7dd      	b.n	3000444a <BOOT_OTA_AP+0x86>
3000448e:	bf00      	nop
30004490:	3000d774 	.word	0x3000d774
30004494:	3000e314 	.word	0x3000e314
30004498:	3000ee50 	.word	0x3000ee50
3000449c:	3000eb44 	.word	0x3000eb44
300044a0:	3000c738 	.word	0x3000c738
300044a4:	2001c00c 	.word	0x2001c00c
300044a8:	3000c714 	.word	0x3000c714
300044ac:	3000c75c 	.word	0x3000c75c

300044b0 <BOOT_OTFMaskConfig>:
300044b0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300044b4:	4c1c      	ldr	r4, [pc, #112]	; (30004528 <BOOT_OTFMaskConfig+0x78>)
300044b6:	b082      	sub	sp, #8
300044b8:	e9d4 1200 	ldrd	r1, r2, [r4]
300044bc:	f8d4 c010 	ldr.w	ip, [r4, #16]
300044c0:	1c4d      	adds	r5, r1, #1
300044c2:	68a0      	ldr	r0, [r4, #8]
300044c4:	d027      	beq.n	30004516 <BOOT_OTFMaskConfig+0x66>
300044c6:	2500      	movs	r5, #0
300044c8:	4e18      	ldr	r6, [pc, #96]	; (3000452c <BOOT_OTFMaskConfig+0x7c>)
300044ca:	f8df 806c 	ldr.w	r8, [pc, #108]	; 30004538 <BOOT_OTFMaskConfig+0x88>
300044ce:	4f18      	ldr	r7, [pc, #96]	; (30004530 <BOOT_OTFMaskConfig+0x80>)
300044d0:	e00d      	b.n	300044ee <BOOT_OTFMaskConfig+0x3e>
300044d2:	6833      	ldr	r3, [r6, #0]
300044d4:	079a      	lsls	r2, r3, #30
300044d6:	d416      	bmi.n	30004506 <BOOT_OTFMaskConfig+0x56>
300044d8:	6961      	ldr	r1, [r4, #20]
300044da:	3501      	adds	r5, #1
300044dc:	69a2      	ldr	r2, [r4, #24]
300044de:	3414      	adds	r4, #20
300044e0:	1c4b      	adds	r3, r1, #1
300044e2:	f8d4 c010 	ldr.w	ip, [r4, #16]
300044e6:	68a0      	ldr	r0, [r4, #8]
300044e8:	d015      	beq.n	30004516 <BOOT_OTFMaskConfig+0x66>
300044ea:	2d08      	cmp	r5, #8
300044ec:	d00f      	beq.n	3000450e <BOOT_OTFMaskConfig+0x5e>
300044ee:	f101 4378 	add.w	r3, r1, #4160749568	; 0xf8000000
300044f2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300044f6:	d2ec      	bcs.n	300044d2 <BOOT_OTFMaskConfig+0x22>
300044f8:	9001      	str	r0, [sp, #4]
300044fa:	2301      	movs	r3, #1
300044fc:	4628      	mov	r0, r5
300044fe:	f8cd c000 	str.w	ip, [sp]
30004502:	47b8      	blx	r7
30004504:	e7e8      	b.n	300044d8 <BOOT_OTFMaskConfig+0x28>
30004506:	4640      	mov	r0, r8
30004508:	f007 fe8e 	bl	3000c228 <__DiagPrintf_veneer>
3000450c:	e7e4      	b.n	300044d8 <BOOT_OTFMaskConfig+0x28>
3000450e:	4b07      	ldr	r3, [pc, #28]	; (3000452c <BOOT_OTFMaskConfig+0x7c>)
30004510:	681b      	ldr	r3, [r3, #0]
30004512:	0799      	lsls	r1, r3, #30
30004514:	d402      	bmi.n	3000451c <BOOT_OTFMaskConfig+0x6c>
30004516:	b002      	add	sp, #8
30004518:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000451c:	4805      	ldr	r0, [pc, #20]	; (30004534 <BOOT_OTFMaskConfig+0x84>)
3000451e:	b002      	add	sp, #8
30004520:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30004524:	f007 be80 	b.w	3000c228 <__DiagPrintf_veneer>
30004528:	3000d820 	.word	0x3000d820
3000452c:	2001c00c 	.word	0x2001c00c
30004530:	3000a26d 	.word	0x3000a26d
30004534:	3000c7c0 	.word	0x3000c7c0
30004538:	3000c808 	.word	0x3000c808

3000453c <BOOT_LoadRDPImg>:
3000453c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004540:	4d50      	ldr	r5, [pc, #320]	; (30004684 <BOOT_LoadRDPImg+0x148>)
30004542:	b0d7      	sub	sp, #348	; 0x15c
30004544:	4613      	mov	r3, r2
30004546:	468b      	mov	fp, r1
30004548:	4604      	mov	r4, r0
3000454a:	aa04      	add	r2, sp, #16
3000454c:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30004550:	e895 0003 	ldmia.w	r5, {r0, r1}
30004554:	2500      	movs	r5, #0
30004556:	e882 0003 	stmia.w	r2, {r0, r1}
3000455a:	4619      	mov	r1, r3
3000455c:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004560:	4620      	mov	r0, r4
30004562:	f001 fbc3 	bl	30005cec <BOOT_ImgCopy>
30004566:	ab04      	add	r3, sp, #16
30004568:	2202      	movs	r2, #2
3000456a:	4641      	mov	r1, r8
3000456c:	4658      	mov	r0, fp
3000456e:	9500      	str	r5, [sp, #0]
30004570:	f7ff fb2e 	bl	30003bd0 <BOOT_LoadSubImage>
30004574:	2801      	cmp	r0, #1
30004576:	d070      	beq.n	3000465a <BOOT_LoadRDPImg+0x11e>
30004578:	4b43      	ldr	r3, [pc, #268]	; (30004688 <BOOT_LoadRDPImg+0x14c>)
3000457a:	4628      	mov	r0, r5
3000457c:	4798      	blx	r3
3000457e:	2101      	movs	r1, #1
30004580:	4b42      	ldr	r3, [pc, #264]	; (3000468c <BOOT_LoadRDPImg+0x150>)
30004582:	4628      	mov	r0, r5
30004584:	4798      	blx	r3
30004586:	af06      	add	r7, sp, #24
30004588:	2120      	movs	r1, #32
3000458a:	4b41      	ldr	r3, [pc, #260]	; (30004690 <BOOT_LoadRDPImg+0x154>)
3000458c:	a80e      	add	r0, sp, #56	; 0x38
3000458e:	f8df 9118 	ldr.w	r9, [pc, #280]	; 300046a8 <BOOT_LoadRDPImg+0x16c>
30004592:	4798      	blx	r3
30004594:	f8df a114 	ldr.w	sl, [pc, #276]	; 300046ac <BOOT_LoadRDPImg+0x170>
30004598:	f104 0120 	add.w	r1, r4, #32
3000459c:	2210      	movs	r2, #16
3000459e:	a80a      	add	r0, sp, #40	; 0x28
300045a0:	47d0      	blx	sl
300045a2:	9503      	str	r5, [sp, #12]
300045a4:	4641      	mov	r1, r8
300045a6:	a816      	add	r0, sp, #88	; 0x58
300045a8:	2210      	movs	r2, #16
300045aa:	f001 fb9f 	bl	30005cec <BOOT_ImgCopy>
300045ae:	2310      	movs	r3, #16
300045b0:	aa0a      	add	r2, sp, #40	; 0x28
300045b2:	a816      	add	r0, sp, #88	; 0x58
300045b4:	4619      	mov	r1, r3
300045b6:	9700      	str	r7, [sp, #0]
300045b8:	47c8      	blx	r9
300045ba:	683b      	ldr	r3, [r7, #0]
300045bc:	4a35      	ldr	r2, [pc, #212]	; (30004694 <BOOT_LoadRDPImg+0x158>)
300045be:	4293      	cmp	r3, r2
300045c0:	4b35      	ldr	r3, [pc, #212]	; (30004698 <BOOT_LoadRDPImg+0x15c>)
300045c2:	d14e      	bne.n	30004662 <BOOT_LoadRDPImg+0x126>
300045c4:	687a      	ldr	r2, [r7, #4]
300045c6:	4935      	ldr	r1, [pc, #212]	; (3000469c <BOOT_LoadRDPImg+0x160>)
300045c8:	689b      	ldr	r3, [r3, #8]
300045ca:	428a      	cmp	r2, r1
300045cc:	f003 0302 	and.w	r3, r3, #2
300045d0:	d155      	bne.n	3000467e <BOOT_LoadRDPImg+0x142>
300045d2:	68fe      	ldr	r6, [r7, #12]
300045d4:	68bd      	ldr	r5, [r7, #8]
300045d6:	3e20      	subs	r6, #32
300045d8:	3520      	adds	r5, #32
300045da:	2b00      	cmp	r3, #0
300045dc:	d133      	bne.n	30004646 <BOOT_LoadRDPImg+0x10a>
300045de:	f1bb 0f00 	cmp.w	fp, #0
300045e2:	d006      	beq.n	300045f2 <BOOT_LoadRDPImg+0xb6>
300045e4:	9a03      	ldr	r2, [sp, #12]
300045e6:	f10b 0304 	add.w	r3, fp, #4
300045ea:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
300045ee:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
300045f2:	b1dd      	cbz	r5, 3000462c <BOOT_LoadRDPImg+0xf0>
300045f4:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
300045f8:	462c      	mov	r4, r5
300045fa:	4641      	mov	r1, r8
300045fc:	a816      	add	r0, sp, #88	; 0x58
300045fe:	bf28      	it	cs
30004600:	f44f 7480 	movcs.w	r4, #256	; 0x100
30004604:	4622      	mov	r2, r4
30004606:	44a0      	add	r8, r4
30004608:	f001 fb70 	bl	30005cec <BOOT_ImgCopy>
3000460c:	aa0a      	add	r2, sp, #40	; 0x28
3000460e:	4621      	mov	r1, r4
30004610:	2310      	movs	r3, #16
30004612:	9600      	str	r6, [sp, #0]
30004614:	a816      	add	r0, sp, #88	; 0x58
30004616:	47c8      	blx	r9
30004618:	ab16      	add	r3, sp, #88	; 0x58
3000461a:	f1a4 0110 	sub.w	r1, r4, #16
3000461e:	2210      	movs	r2, #16
30004620:	a80a      	add	r0, sp, #40	; 0x28
30004622:	4426      	add	r6, r4
30004624:	4419      	add	r1, r3
30004626:	47d0      	blx	sl
30004628:	1b2d      	subs	r5, r5, r4
3000462a:	d1e3      	bne.n	300045f4 <BOOT_LoadRDPImg+0xb8>
3000462c:	9b03      	ldr	r3, [sp, #12]
3000462e:	b13b      	cbz	r3, 30004640 <BOOT_LoadRDPImg+0x104>
30004630:	2100      	movs	r1, #0
30004632:	4b16      	ldr	r3, [pc, #88]	; (3000468c <BOOT_LoadRDPImg+0x150>)
30004634:	4608      	mov	r0, r1
30004636:	4798      	blx	r3
30004638:	2002      	movs	r0, #2
3000463a:	b057      	add	sp, #348	; 0x15c
3000463c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004640:	2301      	movs	r3, #1
30004642:	9303      	str	r3, [sp, #12]
30004644:	e7ae      	b.n	300045a4 <BOOT_LoadRDPImg+0x68>
30004646:	a804      	add	r0, sp, #16
30004648:	9903      	ldr	r1, [sp, #12]
3000464a:	462b      	mov	r3, r5
3000464c:	4632      	mov	r2, r6
3000464e:	f850 1021 	ldr.w	r1, [r0, r1, lsl #2]
30004652:	4813      	ldr	r0, [pc, #76]	; (300046a0 <BOOT_LoadRDPImg+0x164>)
30004654:	f007 fde8 	bl	3000c228 <__DiagPrintf_veneer>
30004658:	e7c1      	b.n	300045de <BOOT_LoadRDPImg+0xa2>
3000465a:	2002      	movs	r0, #2
3000465c:	b057      	add	sp, #348	; 0x15c
3000465e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004662:	6898      	ldr	r0, [r3, #8]
30004664:	f000 0002 	and.w	r0, r0, #2
30004668:	2800      	cmp	r0, #0
3000466a:	d0e6      	beq.n	3000463a <BOOT_LoadRDPImg+0xfe>
3000466c:	9b03      	ldr	r3, [sp, #12]
3000466e:	aa04      	add	r2, sp, #16
30004670:	480c      	ldr	r0, [pc, #48]	; (300046a4 <BOOT_LoadRDPImg+0x168>)
30004672:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30004676:	f007 fdd7 	bl	3000c228 <__DiagPrintf_veneer>
3000467a:	2000      	movs	r0, #0
3000467c:	e7dd      	b.n	3000463a <BOOT_LoadRDPImg+0xfe>
3000467e:	4618      	mov	r0, r3
30004680:	e7f2      	b.n	30004668 <BOOT_LoadRDPImg+0x12c>
30004682:	bf00      	nop
30004684:	3000d788 	.word	0x3000d788
30004688:	00002429 	.word	0x00002429
3000468c:	00002479 	.word	0x00002479
30004690:	00004641 	.word	0x00004641
30004694:	35393138 	.word	0x35393138
30004698:	2001c00c 	.word	0x2001c00c
3000469c:	31313738 	.word	0x31313738
300046a0:	3000c5bc 	.word	0x3000c5bc
300046a4:	3000c594 	.word	0x3000c594
300046a8:	000046f5 	.word	0x000046f5
300046ac:	00012a1d 	.word	0x00012a1d

300046b0 <BOOT_LoadBL32RDPImg>:
300046b0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300046b4:	4b35      	ldr	r3, [pc, #212]	; (3000478c <BOOT_LoadBL32RDPImg+0xdc>)
300046b6:	b0d1      	sub	sp, #324	; 0x144
300046b8:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 300047a8 <BOOT_LoadBL32RDPImg+0xf8>
300046bc:	4607      	mov	r7, r0
300046be:	f10d 0810 	add.w	r8, sp, #16
300046c2:	460e      	mov	r6, r1
300046c4:	4615      	mov	r5, r2
300046c6:	ac02      	add	r4, sp, #8
300046c8:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 300047ac <BOOT_LoadBL32RDPImg+0xfc>
300046cc:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 300047b0 <BOOT_LoadBL32RDPImg+0x100>
300046d0:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300046d2:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
300046d6:	e89c 0003 	ldmia.w	ip, {r0, r1}
300046da:	e884 0003 	stmia.w	r4, {r0, r1}
300046de:	2000      	movs	r0, #0
300046e0:	47c8      	blx	r9
300046e2:	2101      	movs	r1, #1
300046e4:	2000      	movs	r0, #0
300046e6:	47d0      	blx	sl
300046e8:	2120      	movs	r1, #32
300046ea:	4b29      	ldr	r3, [pc, #164]	; (30004790 <BOOT_LoadBL32RDPImg+0xe0>)
300046ec:	eb0d 0001 	add.w	r0, sp, r1
300046f0:	4798      	blx	r3
300046f2:	4b28      	ldr	r3, [pc, #160]	; (30004794 <BOOT_LoadBL32RDPImg+0xe4>)
300046f4:	689b      	ldr	r3, [r3, #8]
300046f6:	079b      	lsls	r3, r3, #30
300046f8:	d436      	bmi.n	30004768 <BOOT_LoadBL32RDPImg+0xb8>
300046fa:	b1fd      	cbz	r5, 3000473c <BOOT_LoadBL32RDPImg+0x8c>
300046fc:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 300047b4 <BOOT_LoadBL32RDPImg+0x104>
30004700:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 300047b8 <BOOT_LoadBL32RDPImg+0x108>
30004704:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30004708:	462c      	mov	r4, r5
3000470a:	4639      	mov	r1, r7
3000470c:	a810      	add	r0, sp, #64	; 0x40
3000470e:	bf28      	it	cs
30004710:	f44f 7480 	movcs.w	r4, #256	; 0x100
30004714:	4622      	mov	r2, r4
30004716:	4427      	add	r7, r4
30004718:	f001 fae8 	bl	30005cec <BOOT_ImgCopy>
3000471c:	4642      	mov	r2, r8
3000471e:	4621      	mov	r1, r4
30004720:	2310      	movs	r3, #16
30004722:	9600      	str	r6, [sp, #0]
30004724:	a810      	add	r0, sp, #64	; 0x40
30004726:	47d8      	blx	fp
30004728:	ab10      	add	r3, sp, #64	; 0x40
3000472a:	f1a4 0110 	sub.w	r1, r4, #16
3000472e:	2210      	movs	r2, #16
30004730:	4640      	mov	r0, r8
30004732:	4426      	add	r6, r4
30004734:	4419      	add	r1, r3
30004736:	47c8      	blx	r9
30004738:	1b2d      	subs	r5, r5, r4
3000473a:	d1e3      	bne.n	30004704 <BOOT_LoadBL32RDPImg+0x54>
3000473c:	f856 2c20 	ldr.w	r2, [r6, #-32]
30004740:	3e20      	subs	r6, #32
30004742:	4b15      	ldr	r3, [pc, #84]	; (30004798 <BOOT_LoadBL32RDPImg+0xe8>)
30004744:	429a      	cmp	r2, r3
30004746:	d108      	bne.n	3000475a <BOOT_LoadBL32RDPImg+0xaa>
30004748:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
3000474c:	6872      	ldr	r2, [r6, #4]
3000474e:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30004752:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30004756:	429a      	cmp	r2, r3
30004758:	d00d      	beq.n	30004776 <BOOT_LoadBL32RDPImg+0xc6>
3000475a:	4810      	ldr	r0, [pc, #64]	; (3000479c <BOOT_LoadBL32RDPImg+0xec>)
3000475c:	f007 fd64 	bl	3000c228 <__DiagPrintf_veneer>
30004760:	2000      	movs	r0, #0
30004762:	b051      	add	sp, #324	; 0x144
30004764:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004768:	4621      	mov	r1, r4
3000476a:	462b      	mov	r3, r5
3000476c:	4632      	mov	r2, r6
3000476e:	480c      	ldr	r0, [pc, #48]	; (300047a0 <BOOT_LoadBL32RDPImg+0xf0>)
30004770:	f007 fd5a 	bl	3000c228 <__DiagPrintf_veneer>
30004774:	e7c1      	b.n	300046fa <BOOT_LoadBL32RDPImg+0x4a>
30004776:	480b      	ldr	r0, [pc, #44]	; (300047a4 <BOOT_LoadBL32RDPImg+0xf4>)
30004778:	f007 fd56 	bl	3000c228 <__DiagPrintf_veneer>
3000477c:	2100      	movs	r1, #0
3000477e:	4608      	mov	r0, r1
30004780:	47d0      	blx	sl
30004782:	2001      	movs	r0, #1
30004784:	b051      	add	sp, #324	; 0x144
30004786:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000478a:	bf00      	nop
3000478c:	3000c85c 	.word	0x3000c85c
30004790:	00004641 	.word	0x00004641
30004794:	2001c00c 	.word	0x2001c00c
30004798:	35393138 	.word	0x35393138
3000479c:	3000c840 	.word	0x3000c840
300047a0:	3000c5bc 	.word	0x3000c5bc
300047a4:	3000c84c 	.word	0x3000c84c
300047a8:	3000c870 	.word	0x3000c870
300047ac:	00002429 	.word	0x00002429
300047b0:	00002479 	.word	0x00002479
300047b4:	000046f5 	.word	0x000046f5
300047b8:	00012a1d 	.word	0x00012a1d

300047bc <_BOOT_Validate_ImgHash>:
300047bc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300047c0:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
300047c4:	4605      	mov	r5, r0
300047c6:	4690      	mov	r8, r2
300047c8:	22e0      	movs	r2, #224	; 0xe0
300047ca:	9103      	str	r1, [sp, #12]
300047cc:	2100      	movs	r1, #0
300047ce:	a818      	add	r0, sp, #96	; 0x60
300047d0:	461c      	mov	r4, r3
300047d2:	f88d 101f 	strb.w	r1, [sp, #31]
300047d6:	f007 fd07 	bl	3000c1e8 <____wrap_memset_veneer>
300047da:	4628      	mov	r0, r5
300047dc:	f10d 021f 	add.w	r2, sp, #31
300047e0:	f10d 011e 	add.w	r1, sp, #30
300047e4:	f007 fce8 	bl	3000c1b8 <__SBOOT_GetMdType_veneer>
300047e8:	9001      	str	r0, [sp, #4]
300047ea:	2800      	cmp	r0, #0
300047ec:	d166      	bne.n	300048bc <_BOOT_Validate_ImgHash+0x100>
300047ee:	f89d 301f 	ldrb.w	r3, [sp, #31]
300047f2:	2b00      	cmp	r3, #0
300047f4:	d067      	beq.n	300048c6 <_BOOT_Validate_ImgHash+0x10a>
300047f6:	2101      	movs	r1, #1
300047f8:	4605      	mov	r5, r0
300047fa:	4b80      	ldr	r3, [pc, #512]	; (300049fc <_BOOT_Validate_ImgHash+0x240>)
300047fc:	4608      	mov	r0, r1
300047fe:	4798      	blx	r3
30004800:	4629      	mov	r1, r5
30004802:	ab18      	add	r3, sp, #96	; 0x60
30004804:	2220      	movs	r2, #32
30004806:	f89d 001e 	ldrb.w	r0, [sp, #30]
3000480a:	4d7d      	ldr	r5, [pc, #500]	; (30004a00 <_BOOT_Validate_ImgHash+0x244>)
3000480c:	47a8      	blx	r5
3000480e:	2800      	cmp	r0, #0
30004810:	f040 808b 	bne.w	3000492a <_BOOT_Validate_ImgHash+0x16e>
30004814:	b32c      	cbz	r4, 30004862 <_BOOT_Validate_ImgHash+0xa6>
30004816:	aad0      	add	r2, sp, #832	; 0x340
30004818:	1e63      	subs	r3, r4, #1
3000481a:	ae50      	add	r6, sp, #320	; 0x140
3000481c:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 30004a18 <_BOOT_Validate_ImgHash+0x25c>
30004820:	f012 0218 	ands.w	r2, r2, #24
30004824:	b2db      	uxtb	r3, r3
30004826:	f026 091f 	bic.w	r9, r6, #31
3000482a:	4d76      	ldr	r5, [pc, #472]	; (30004a04 <_BOOT_Validate_ImgHash+0x248>)
3000482c:	9204      	str	r2, [sp, #16]
3000482e:	f108 0208 	add.w	r2, r8, #8
30004832:	bf08      	it	eq
30004834:	46b1      	moveq	r9, r6
30004836:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
3000483a:	9302      	str	r3, [sp, #8]
3000483c:	e9d8 b700 	ldrd	fp, r7, [r8]
30004840:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30004844:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004848:	f0c0 80a7 	bcc.w	3000499a <_BOOT_Validate_ImgHash+0x1de>
3000484c:	4639      	mov	r1, r7
3000484e:	4658      	mov	r0, fp
30004850:	aa18      	add	r2, sp, #96	; 0x60
30004852:	47d0      	blx	sl
30004854:	2800      	cmp	r0, #0
30004856:	d168      	bne.n	3000492a <_BOOT_Validate_ImgHash+0x16e>
30004858:	f108 0808 	add.w	r8, r8, #8
3000485c:	9b02      	ldr	r3, [sp, #8]
3000485e:	4543      	cmp	r3, r8
30004860:	d1ec      	bne.n	3000483c <_BOOT_Validate_ImgHash+0x80>
30004862:	ac08      	add	r4, sp, #32
30004864:	a918      	add	r1, sp, #96	; 0x60
30004866:	4b68      	ldr	r3, [pc, #416]	; (30004a08 <_BOOT_Validate_ImgHash+0x24c>)
30004868:	4620      	mov	r0, r4
3000486a:	4798      	blx	r3
3000486c:	4601      	mov	r1, r0
3000486e:	2800      	cmp	r0, #0
30004870:	d15b      	bne.n	3000492a <_BOOT_Validate_ImgHash+0x16e>
30004872:	2001      	movs	r0, #1
30004874:	4b61      	ldr	r3, [pc, #388]	; (300049fc <_BOOT_Validate_ImgHash+0x240>)
30004876:	4798      	blx	r3
30004878:	f89d 501e 	ldrb.w	r5, [sp, #30]
3000487c:	4b63      	ldr	r3, [pc, #396]	; (30004a0c <_BOOT_Validate_ImgHash+0x250>)
3000487e:	4798      	blx	r3
30004880:	fbb0 f3f5 	udiv	r3, r0, r5
30004884:	fb03 0315 	mls	r3, r3, r5, r0
30004888:	b2db      	uxtb	r3, r3
3000488a:	b1bd      	cbz	r5, 300048bc <_BOOT_Validate_ImgHash+0x100>
3000488c:	2000      	movs	r0, #0
3000488e:	9f03      	ldr	r7, [sp, #12]
30004890:	4606      	mov	r6, r0
30004892:	1c5a      	adds	r2, r3, #1
30004894:	5cf9      	ldrb	r1, [r7, r3]
30004896:	5ce3      	ldrb	r3, [r4, r3]
30004898:	3001      	adds	r0, #1
3000489a:	b2d2      	uxtb	r2, r2
3000489c:	404b      	eors	r3, r1
3000489e:	431e      	orrs	r6, r3
300048a0:	fbb2 f3f5 	udiv	r3, r2, r5
300048a4:	fb05 2213 	mls	r2, r5, r3, r2
300048a8:	b2d3      	uxtb	r3, r2
300048aa:	b2c2      	uxtb	r2, r0
300048ac:	4295      	cmp	r5, r2
300048ae:	d8f0      	bhi.n	30004892 <_BOOT_Validate_ImgHash+0xd6>
300048b0:	2e00      	cmp	r6, #0
300048b2:	9b01      	ldr	r3, [sp, #4]
300048b4:	bf18      	it	ne
300048b6:	f06f 0306 	mvnne.w	r3, #6
300048ba:	9301      	str	r3, [sp, #4]
300048bc:	9801      	ldr	r0, [sp, #4]
300048be:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300048c2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300048c6:	a918      	add	r1, sp, #96	; 0x60
300048c8:	f89d 001e 	ldrb.w	r0, [sp, #30]
300048cc:	4b50      	ldr	r3, [pc, #320]	; (30004a10 <_BOOT_Validate_ImgHash+0x254>)
300048ce:	4798      	blx	r3
300048d0:	bb58      	cbnz	r0, 3000492a <_BOOT_Validate_ImgHash+0x16e>
300048d2:	b31c      	cbz	r4, 3000491c <_BOOT_Validate_ImgHash+0x160>
300048d4:	aad0      	add	r2, sp, #832	; 0x340
300048d6:	1e63      	subs	r3, r4, #1
300048d8:	ae50      	add	r6, sp, #320	; 0x140
300048da:	f8df a140 	ldr.w	sl, [pc, #320]	; 30004a1c <_BOOT_Validate_ImgHash+0x260>
300048de:	f012 0218 	ands.w	r2, r2, #24
300048e2:	b2db      	uxtb	r3, r3
300048e4:	f026 091f 	bic.w	r9, r6, #31
300048e8:	4d46      	ldr	r5, [pc, #280]	; (30004a04 <_BOOT_Validate_ImgHash+0x248>)
300048ea:	9204      	str	r2, [sp, #16]
300048ec:	f108 0208 	add.w	r2, r8, #8
300048f0:	bf08      	it	eq
300048f2:	46b1      	moveq	r9, r6
300048f4:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300048f8:	9302      	str	r3, [sp, #8]
300048fa:	e9d8 b700 	ldrd	fp, r7, [r8]
300048fe:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30004902:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004906:	d318      	bcc.n	3000493a <_BOOT_Validate_ImgHash+0x17e>
30004908:	4639      	mov	r1, r7
3000490a:	4658      	mov	r0, fp
3000490c:	aa18      	add	r2, sp, #96	; 0x60
3000490e:	47d0      	blx	sl
30004910:	b958      	cbnz	r0, 3000492a <_BOOT_Validate_ImgHash+0x16e>
30004912:	f108 0808 	add.w	r8, r8, #8
30004916:	9b02      	ldr	r3, [sp, #8]
30004918:	4543      	cmp	r3, r8
3000491a:	d1ee      	bne.n	300048fa <_BOOT_Validate_ImgHash+0x13e>
3000491c:	ac08      	add	r4, sp, #32
3000491e:	a918      	add	r1, sp, #96	; 0x60
30004920:	4b3c      	ldr	r3, [pc, #240]	; (30004a14 <_BOOT_Validate_ImgHash+0x258>)
30004922:	4620      	mov	r0, r4
30004924:	4798      	blx	r3
30004926:	2800      	cmp	r0, #0
30004928:	d0a6      	beq.n	30004878 <_BOOT_Validate_ImgHash+0xbc>
3000492a:	f06f 030a 	mvn.w	r3, #10
3000492e:	9301      	str	r3, [sp, #4]
30004930:	9801      	ldr	r0, [sp, #4]
30004932:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30004936:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000493a:	2f00      	cmp	r7, #0
3000493c:	d0e9      	beq.n	30004912 <_BOOT_Validate_ImgHash+0x156>
3000493e:	9c04      	ldr	r4, [sp, #16]
30004940:	f8cd 8014 	str.w	r8, [sp, #20]
30004944:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30004948:	46b8      	mov	r8, r7
3000494a:	4659      	mov	r1, fp
3000494c:	4630      	mov	r0, r6
3000494e:	bf28      	it	cs
30004950:	f44f 7800 	movcs.w	r8, #512	; 0x200
30004954:	4642      	mov	r2, r8
30004956:	f001 f9c9 	bl	30005cec <BOOT_ImgCopy>
3000495a:	696a      	ldr	r2, [r5, #20]
3000495c:	03d3      	lsls	r3, r2, #15
3000495e:	d50f      	bpl.n	30004980 <_BOOT_Validate_ImgHash+0x1c4>
30004960:	eb08 0004 	add.w	r0, r8, r4
30004964:	f3bf 8f4f 	dsb	sy
30004968:	4448      	add	r0, r9
3000496a:	464a      	mov	r2, r9
3000496c:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30004970:	3220      	adds	r2, #32
30004972:	1a83      	subs	r3, r0, r2
30004974:	2b00      	cmp	r3, #0
30004976:	dcf9      	bgt.n	3000496c <_BOOT_Validate_ImgHash+0x1b0>
30004978:	f3bf 8f4f 	dsb	sy
3000497c:	f3bf 8f6f 	isb	sy
30004980:	aa18      	add	r2, sp, #96	; 0x60
30004982:	4641      	mov	r1, r8
30004984:	4630      	mov	r0, r6
30004986:	47d0      	blx	sl
30004988:	2800      	cmp	r0, #0
3000498a:	d1ce      	bne.n	3000492a <_BOOT_Validate_ImgHash+0x16e>
3000498c:	ebb7 0708 	subs.w	r7, r7, r8
30004990:	44c3      	add	fp, r8
30004992:	d1d7      	bne.n	30004944 <_BOOT_Validate_ImgHash+0x188>
30004994:	f8dd 8014 	ldr.w	r8, [sp, #20]
30004998:	e7bb      	b.n	30004912 <_BOOT_Validate_ImgHash+0x156>
3000499a:	2f00      	cmp	r7, #0
3000499c:	f43f af5c 	beq.w	30004858 <_BOOT_Validate_ImgHash+0x9c>
300049a0:	9c04      	ldr	r4, [sp, #16]
300049a2:	f8cd 8014 	str.w	r8, [sp, #20]
300049a6:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
300049aa:	46b8      	mov	r8, r7
300049ac:	4659      	mov	r1, fp
300049ae:	4630      	mov	r0, r6
300049b0:	bf28      	it	cs
300049b2:	f44f 7800 	movcs.w	r8, #512	; 0x200
300049b6:	4642      	mov	r2, r8
300049b8:	f001 f998 	bl	30005cec <BOOT_ImgCopy>
300049bc:	696a      	ldr	r2, [r5, #20]
300049be:	03d2      	lsls	r2, r2, #15
300049c0:	d50f      	bpl.n	300049e2 <_BOOT_Validate_ImgHash+0x226>
300049c2:	eb08 0004 	add.w	r0, r8, r4
300049c6:	f3bf 8f4f 	dsb	sy
300049ca:	4448      	add	r0, r9
300049cc:	464a      	mov	r2, r9
300049ce:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
300049d2:	3220      	adds	r2, #32
300049d4:	1a83      	subs	r3, r0, r2
300049d6:	2b00      	cmp	r3, #0
300049d8:	dcf9      	bgt.n	300049ce <_BOOT_Validate_ImgHash+0x212>
300049da:	f3bf 8f4f 	dsb	sy
300049de:	f3bf 8f6f 	isb	sy
300049e2:	aa18      	add	r2, sp, #96	; 0x60
300049e4:	4641      	mov	r1, r8
300049e6:	4630      	mov	r0, r6
300049e8:	47d0      	blx	sl
300049ea:	2800      	cmp	r0, #0
300049ec:	d19d      	bne.n	3000492a <_BOOT_Validate_ImgHash+0x16e>
300049ee:	ebb7 0708 	subs.w	r7, r7, r8
300049f2:	44c3      	add	fp, r8
300049f4:	d1d7      	bne.n	300049a6 <_BOOT_Validate_ImgHash+0x1ea>
300049f6:	f8dd 8014 	ldr.w	r8, [sp, #20]
300049fa:	e72d      	b.n	30004858 <_BOOT_Validate_ImgHash+0x9c>
300049fc:	000024dd 	.word	0x000024dd
30004a00:	30009eb1 	.word	0x30009eb1
30004a04:	e000ed00 	.word	0xe000ed00
30004a08:	00005bb9 	.word	0x00005bb9
30004a0c:	0000ea4d 	.word	0x0000ea4d
30004a10:	00004ffd 	.word	0x00004ffd
30004a14:	000053f1 	.word	0x000053f1
30004a18:	00005b4d 	.word	0x00005b4d
30004a1c:	00005385 	.word	0x00005385

30004a20 <BOOT_Validate_PubKey>:
30004a20:	f007 bc0a 	b.w	3000c238 <__SBOOT_Validate_PubKey_veneer>

30004a24 <BOOT_Validate_Signature>:
30004a24:	f007 bbd8 	b.w	3000c1d8 <__SBOOT_Validate_Signature_veneer>

30004a28 <BOOT_Validate_ImgHash>:
30004a28:	b530      	push	{r4, r5, lr}
30004a2a:	b085      	sub	sp, #20
30004a2c:	4d0a      	ldr	r5, [pc, #40]	; (30004a58 <BOOT_Validate_ImgHash+0x30>)
30004a2e:	4604      	mov	r4, r0
30004a30:	9301      	str	r3, [sp, #4]
30004a32:	e9cd 2102 	strd	r2, r1, [sp, #8]
30004a36:	47a8      	blx	r5
30004a38:	9903      	ldr	r1, [sp, #12]
30004a3a:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30004a3e:	b128      	cbz	r0, 30004a4c <BOOT_Validate_ImgHash+0x24>
30004a40:	4620      	mov	r0, r4
30004a42:	b005      	add	sp, #20
30004a44:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004a48:	f007 bbe6 	b.w	3000c218 <__SBOOT_Validate_ImgHash_veneer>
30004a4c:	4620      	mov	r0, r4
30004a4e:	b005      	add	sp, #20
30004a50:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004a54:	f7ff beb2 	b.w	300047bc <_BOOT_Validate_ImgHash>
30004a58:	3000a541 	.word	0x3000a541

30004a5c <BOOT_SignatureCheck>:
30004a5c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30004a60:	461e      	mov	r6, r3
30004a62:	b08b      	sub	sp, #44	; 0x2c
30004a64:	4b7e      	ldr	r3, [pc, #504]	; (30004c60 <BOOT_SignatureCheck+0x204>)
30004a66:	4680      	mov	r8, r0
30004a68:	4689      	mov	r9, r1
30004a6a:	4617      	mov	r7, r2
30004a6c:	f10d 0c28 	add.w	ip, sp, #40	; 0x28
30004a70:	4c7c      	ldr	r4, [pc, #496]	; (30004c64 <BOOT_SignatureCheck+0x208>)
30004a72:	9d12      	ldr	r5, [sp, #72]	; 0x48
30004a74:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30004a76:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30004a7a:	47a0      	blx	r4
30004a7c:	b918      	cbnz	r0, 30004a86 <BOOT_SignatureCheck+0x2a>
30004a7e:	2000      	movs	r0, #0
30004a80:	b00b      	add	sp, #44	; 0x2c
30004a82:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004a86:	4b78      	ldr	r3, [pc, #480]	; (30004c68 <BOOT_SignatureCheck+0x20c>)
30004a88:	2000      	movs	r0, #0
30004a8a:	4798      	blx	r3
30004a8c:	f898 2012 	ldrb.w	r2, [r8, #18]
30004a90:	f898 3013 	ldrb.w	r3, [r8, #19]
30004a94:	f10d 0117 	add.w	r1, sp, #23
30004a98:	f10d 0016 	add.w	r0, sp, #22
30004a9c:	f007 fb94 	bl	3000c1c8 <__SBOOT_Validate_Algorithm_veneer>
30004aa0:	4602      	mov	r2, r0
30004aa2:	b9e8      	cbnz	r0, 30004ae0 <BOOT_SignatureCheck+0x84>
30004aa4:	69f0      	ldr	r0, [r6, #28]
30004aa6:	b918      	cbnz	r0, 30004ab0 <BOOT_SignatureCheck+0x54>
30004aa8:	e0d7      	b.n	30004c5a <BOOT_SignatureCheck+0x1fe>
30004aaa:	b2ca      	uxtb	r2, r1
30004aac:	4282      	cmp	r2, r0
30004aae:	d208      	bcs.n	30004ac2 <BOOT_SignatureCheck+0x66>
30004ab0:	eb02 04c2 	add.w	r4, r2, r2, lsl #3
30004ab4:	1c51      	adds	r1, r2, #1
30004ab6:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30004aba:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30004abe:	42ab      	cmp	r3, r5
30004ac0:	d1f3      	bne.n	30004aaa <BOOT_SignatureCheck+0x4e>
30004ac2:	2124      	movs	r1, #36	; 0x24
30004ac4:	23b4      	movs	r3, #180	; 0xb4
30004ac6:	fb11 3202 	smlabb	r2, r1, r2, r3
30004aca:	f108 0470 	add.w	r4, r8, #112	; 0x70
30004ace:	4432      	add	r2, r6
30004ad0:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004ad4:	4621      	mov	r1, r4
30004ad6:	f007 fbaf 	bl	3000c238 <__SBOOT_Validate_PubKey_veneer>
30004ada:	4602      	mov	r2, r0
30004adc:	2800      	cmp	r0, #0
30004ade:	d072      	beq.n	30004bc6 <BOOT_SignatureCheck+0x16a>
30004ae0:	4b62      	ldr	r3, [pc, #392]	; (30004c6c <BOOT_SignatureCheck+0x210>)
30004ae2:	681b      	ldr	r3, [r3, #0]
30004ae4:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004ae8:	2b00      	cmp	r3, #0
30004aea:	f040 80a4 	bne.w	30004c36 <BOOT_SignatureCheck+0x1da>
30004aee:	2f00      	cmp	r7, #0
30004af0:	d045      	beq.n	30004b7e <BOOT_SignatureCheck+0x122>
30004af2:	464d      	mov	r5, r9
30004af4:	f107 39ff 	add.w	r9, r7, #4294967295
30004af8:	f04f 0800 	mov.w	r8, #0
30004afc:	4f5c      	ldr	r7, [pc, #368]	; (30004c70 <BOOT_SignatureCheck+0x214>)
30004afe:	fa5f f389 	uxtb.w	r3, r9
30004b02:	f105 0908 	add.w	r9, r5, #8
30004b06:	1d2e      	adds	r6, r5, #4
30004b08:	4c5a      	ldr	r4, [pc, #360]	; (30004c74 <BOOT_SignatureCheck+0x218>)
30004b0a:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30004b0e:	e003      	b.n	30004b18 <BOOT_SignatureCheck+0xbc>
30004b10:	3508      	adds	r5, #8
30004b12:	3608      	adds	r6, #8
30004b14:	45a9      	cmp	r9, r5
30004b16:	d032      	beq.n	30004b7e <BOOT_SignatureCheck+0x122>
30004b18:	f856 0c04 	ldr.w	r0, [r6, #-4]
30004b1c:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30004b20:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004b24:	d3f4      	bcc.n	30004b10 <BOOT_SignatureCheck+0xb4>
30004b26:	2100      	movs	r1, #0
30004b28:	686a      	ldr	r2, [r5, #4]
30004b2a:	47b8      	blx	r7
30004b2c:	6963      	ldr	r3, [r4, #20]
30004b2e:	03d9      	lsls	r1, r3, #15
30004b30:	d5ee      	bpl.n	30004b10 <BOOT_SignatureCheck+0xb4>
30004b32:	f856 2c04 	ldr.w	r2, [r6, #-4]
30004b36:	686b      	ldr	r3, [r5, #4]
30004b38:	ea02 0103 	and.w	r1, r2, r3
30004b3c:	3101      	adds	r1, #1
30004b3e:	d022      	beq.n	30004b86 <BOOT_SignatureCheck+0x12a>
30004b40:	f012 011f 	ands.w	r1, r2, #31
30004b44:	d002      	beq.n	30004b4c <BOOT_SignatureCheck+0xf0>
30004b46:	440b      	add	r3, r1
30004b48:	f022 021f 	bic.w	r2, r2, #31
30004b4c:	f3bf 8f4f 	dsb	sy
30004b50:	2b00      	cmp	r3, #0
30004b52:	dd0c      	ble.n	30004b6e <BOOT_SignatureCheck+0x112>
30004b54:	1e59      	subs	r1, r3, #1
30004b56:	f102 0320 	add.w	r3, r2, #32
30004b5a:	f021 011f 	bic.w	r1, r1, #31
30004b5e:	4419      	add	r1, r3
30004b60:	428b      	cmp	r3, r1
30004b62:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30004b66:	461a      	mov	r2, r3
30004b68:	f103 0320 	add.w	r3, r3, #32
30004b6c:	d1f8      	bne.n	30004b60 <BOOT_SignatureCheck+0x104>
30004b6e:	f3bf 8f4f 	dsb	sy
30004b72:	f3bf 8f6f 	isb	sy
30004b76:	3508      	adds	r5, #8
30004b78:	3608      	adds	r6, #8
30004b7a:	45a9      	cmp	r9, r5
30004b7c:	d1cc      	bne.n	30004b18 <BOOT_SignatureCheck+0xbc>
30004b7e:	20ff      	movs	r0, #255	; 0xff
30004b80:	b00b      	add	sp, #44	; 0x2c
30004b82:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004b86:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30004b8a:	f3bf 8f4f 	dsb	sy
30004b8e:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30004b92:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30004b96:	f3c3 304e 	ubfx	r0, r3, #13, #15
30004b9a:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30004b9e:	0140      	lsls	r0, r0, #5
30004ba0:	ea00 010e 	and.w	r1, r0, lr
30004ba4:	4663      	mov	r3, ip
30004ba6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004baa:	3b01      	subs	r3, #1
30004bac:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30004bb0:	1c5a      	adds	r2, r3, #1
30004bb2:	d1f8      	bne.n	30004ba6 <BOOT_SignatureCheck+0x14a>
30004bb4:	3820      	subs	r0, #32
30004bb6:	f110 0f20 	cmn.w	r0, #32
30004bba:	d1f1      	bne.n	30004ba0 <BOOT_SignatureCheck+0x144>
30004bbc:	f3bf 8f4f 	dsb	sy
30004bc0:	f3bf 8f6f 	isb	sy
30004bc4:	e7d7      	b.n	30004b76 <BOOT_SignatureCheck+0x11a>
30004bc6:	f508 7380 	add.w	r3, r8, #256	; 0x100
30004bca:	4622      	mov	r2, r4
30004bcc:	f89d 1017 	ldrb.w	r1, [sp, #23]
30004bd0:	9301      	str	r3, [sp, #4]
30004bd2:	f44f 7380 	mov.w	r3, #256	; 0x100
30004bd6:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004bda:	9300      	str	r3, [sp, #0]
30004bdc:	4643      	mov	r3, r8
30004bde:	f007 fafb 	bl	3000c1d8 <__SBOOT_Validate_Signature_veneer>
30004be2:	4602      	mov	r2, r0
30004be4:	2800      	cmp	r0, #0
30004be6:	f47f af7b 	bne.w	30004ae0 <BOOT_SignatureCheck+0x84>
30004bea:	f108 0530 	add.w	r5, r8, #48	; 0x30
30004bee:	4b22      	ldr	r3, [pc, #136]	; (30004c78 <BOOT_SignatureCheck+0x21c>)
30004bf0:	f89d 4017 	ldrb.w	r4, [sp, #23]
30004bf4:	4798      	blx	r3
30004bf6:	4629      	mov	r1, r5
30004bf8:	bb40      	cbnz	r0, 30004c4c <BOOT_SignatureCheck+0x1f0>
30004bfa:	464a      	mov	r2, r9
30004bfc:	4620      	mov	r0, r4
30004bfe:	463b      	mov	r3, r7
30004c00:	f7ff fddc 	bl	300047bc <_BOOT_Validate_ImgHash>
30004c04:	4602      	mov	r2, r0
30004c06:	4b19      	ldr	r3, [pc, #100]	; (30004c6c <BOOT_SignatureCheck+0x210>)
30004c08:	681b      	ldr	r3, [r3, #0]
30004c0a:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004c0e:	2a00      	cmp	r2, #0
30004c10:	f47f af6a 	bne.w	30004ae8 <BOOT_SignatureCheck+0x8c>
30004c14:	2b00      	cmp	r3, #0
30004c16:	f43f af32 	beq.w	30004a7e <BOOT_SignatureCheck+0x22>
30004c1a:	f898 3011 	ldrb.w	r3, [r8, #17]
30004c1e:	9203      	str	r2, [sp, #12]
30004c20:	aa0a      	add	r2, sp, #40	; 0x28
30004c22:	4816      	ldr	r0, [pc, #88]	; (30004c7c <BOOT_SignatureCheck+0x220>)
30004c24:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30004c28:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004c2c:	f007 fafc 	bl	3000c228 <__DiagPrintf_veneer>
30004c30:	9a03      	ldr	r2, [sp, #12]
30004c32:	4610      	mov	r0, r2
30004c34:	e724      	b.n	30004a80 <BOOT_SignatureCheck+0x24>
30004c36:	a90a      	add	r1, sp, #40	; 0x28
30004c38:	f898 3011 	ldrb.w	r3, [r8, #17]
30004c3c:	4810      	ldr	r0, [pc, #64]	; (30004c80 <BOOT_SignatureCheck+0x224>)
30004c3e:	eb01 0383 	add.w	r3, r1, r3, lsl #2
30004c42:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004c46:	f007 faef 	bl	3000c228 <__DiagPrintf_veneer>
30004c4a:	e750      	b.n	30004aee <BOOT_SignatureCheck+0x92>
30004c4c:	464a      	mov	r2, r9
30004c4e:	4620      	mov	r0, r4
30004c50:	463b      	mov	r3, r7
30004c52:	f007 fae1 	bl	3000c218 <__SBOOT_Validate_ImgHash_veneer>
30004c56:	4602      	mov	r2, r0
30004c58:	e7d5      	b.n	30004c06 <BOOT_SignatureCheck+0x1aa>
30004c5a:	22b4      	movs	r2, #180	; 0xb4
30004c5c:	e735      	b.n	30004aca <BOOT_SignatureCheck+0x6e>
30004c5e:	bf00      	nop
30004c60:	3000d790 	.word	0x3000d790
30004c64:	0000c149 	.word	0x0000c149
30004c68:	00002451 	.word	0x00002451
30004c6c:	2001c00c 	.word	0x2001c00c
30004c70:	00012be5 	.word	0x00012be5
30004c74:	e000ed00 	.word	0xe000ed00
30004c78:	3000a541 	.word	0x3000a541
30004c7c:	3000c878 	.word	0x3000c878
30004c80:	3000c88c 	.word	0x3000c88c

30004c84 <BOOT_CertificateCheck>:
30004c84:	b530      	push	{r4, r5, lr}
30004c86:	4b2b      	ldr	r3, [pc, #172]	; (30004d34 <BOOT_CertificateCheck+0xb0>)
30004c88:	b0b1      	sub	sp, #196	; 0xc4
30004c8a:	4604      	mov	r4, r0
30004c8c:	460d      	mov	r5, r1
30004c8e:	4798      	blx	r3
30004c90:	b910      	cbnz	r0, 30004c98 <BOOT_CertificateCheck+0x14>
30004c92:	2000      	movs	r0, #0
30004c94:	b031      	add	sp, #196	; 0xc4
30004c96:	bd30      	pop	{r4, r5, pc}
30004c98:	69a1      	ldr	r1, [r4, #24]
30004c9a:	2290      	movs	r2, #144	; 0x90
30004c9c:	a80c      	add	r0, sp, #48	; 0x30
30004c9e:	4429      	add	r1, r5
30004ca0:	f001 f824 	bl	30005cec <BOOT_ImgCopy>
30004ca4:	4b24      	ldr	r3, [pc, #144]	; (30004d38 <BOOT_CertificateCheck+0xb4>)
30004ca6:	4798      	blx	r3
30004ca8:	4924      	ldr	r1, [pc, #144]	; (30004d3c <BOOT_CertificateCheck+0xb8>)
30004caa:	f000 031f 	and.w	r3, r0, #31
30004cae:	2220      	movs	r2, #32
30004cb0:	6808      	ldr	r0, [r1, #0]
30004cb2:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30004cb6:	a930      	add	r1, sp, #192	; 0xc0
30004cb8:	3a01      	subs	r2, #1
30004cba:	f813 c000 	ldrb.w	ip, [r3, r0]
30004cbe:	4419      	add	r1, r3
30004cc0:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30004cc4:	f103 0301 	add.w	r3, r3, #1
30004cc8:	f801 ccb0 	strb.w	ip, [r1, #-176]
30004ccc:	f003 031f 	and.w	r3, r3, #31
30004cd0:	d1f1      	bne.n	30004cb6 <BOOT_CertificateCheck+0x32>
30004cd2:	4610      	mov	r0, r2
30004cd4:	4b1a      	ldr	r3, [pc, #104]	; (30004d40 <BOOT_CertificateCheck+0xbc>)
30004cd6:	4798      	blx	r3
30004cd8:	7ce3      	ldrb	r3, [r4, #19]
30004cda:	7ca2      	ldrb	r2, [r4, #18]
30004cdc:	f10d 010f 	add.w	r1, sp, #15
30004ce0:	f10d 000e 	add.w	r0, sp, #14
30004ce4:	f007 fa70 	bl	3000c1c8 <__SBOOT_Validate_Algorithm_veneer>
30004ce8:	b940      	cbnz	r0, 30004cfc <BOOT_CertificateCheck+0x78>
30004cea:	f104 0520 	add.w	r5, r4, #32
30004cee:	aa04      	add	r2, sp, #16
30004cf0:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004cf4:	4629      	mov	r1, r5
30004cf6:	f007 fa9f 	bl	3000c238 <__SBOOT_Validate_PubKey_veneer>
30004cfa:	b130      	cbz	r0, 30004d0a <BOOT_CertificateCheck+0x86>
30004cfc:	4b11      	ldr	r3, [pc, #68]	; (30004d44 <BOOT_CertificateCheck+0xc0>)
30004cfe:	681b      	ldr	r3, [r3, #0]
30004d00:	039b      	lsls	r3, r3, #14
30004d02:	d411      	bmi.n	30004d28 <BOOT_CertificateCheck+0xa4>
30004d04:	20ff      	movs	r0, #255	; 0xff
30004d06:	b031      	add	sp, #196	; 0xc4
30004d08:	bd30      	pop	{r4, r5, pc}
30004d0a:	69a0      	ldr	r0, [r4, #24]
30004d0c:	4623      	mov	r3, r4
30004d0e:	ac0c      	add	r4, sp, #48	; 0x30
30004d10:	462a      	mov	r2, r5
30004d12:	f89d 100f 	ldrb.w	r1, [sp, #15]
30004d16:	e9cd 0400 	strd	r0, r4, [sp]
30004d1a:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004d1e:	f007 fa5b 	bl	3000c1d8 <__SBOOT_Validate_Signature_veneer>
30004d22:	2800      	cmp	r0, #0
30004d24:	d0b5      	beq.n	30004c92 <BOOT_CertificateCheck+0xe>
30004d26:	e7e9      	b.n	30004cfc <BOOT_CertificateCheck+0x78>
30004d28:	4601      	mov	r1, r0
30004d2a:	4807      	ldr	r0, [pc, #28]	; (30004d48 <BOOT_CertificateCheck+0xc4>)
30004d2c:	f007 fa7c 	bl	3000c228 <__DiagPrintf_veneer>
30004d30:	20ff      	movs	r0, #255	; 0xff
30004d32:	e7af      	b.n	30004c94 <BOOT_CertificateCheck+0x10>
30004d34:	0000c149 	.word	0x0000c149
30004d38:	0000ea4d 	.word	0x0000ea4d
30004d3c:	3000d810 	.word	0x3000d810
30004d40:	00002451 	.word	0x00002451
30004d44:	2001c00c 	.word	0x2001c00c
30004d48:	3000c8a8 	.word	0x3000c8a8

30004d4c <BOOT_RAM_TZCfg>:
30004d4c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004d50:	b085      	sub	sp, #20
30004d52:	4e56      	ldr	r6, [pc, #344]	; (30004eac <BOOT_RAM_TZCfg+0x160>)
30004d54:	4956      	ldr	r1, [pc, #344]	; (30004eb0 <BOOT_RAM_TZCfg+0x164>)
30004d56:	2501      	movs	r5, #1
30004d58:	f10d 0e08 	add.w	lr, sp, #8
30004d5c:	4a55      	ldr	r2, [pc, #340]	; (30004eb4 <BOOT_RAM_TZCfg+0x168>)
30004d5e:	4b56      	ldr	r3, [pc, #344]	; (30004eb8 <BOOT_RAM_TZCfg+0x16c>)
30004d60:	46ec      	mov	ip, sp
30004d62:	4677      	mov	r7, lr
30004d64:	f8df 816c 	ldr.w	r8, [pc, #364]	; 30004ed4 <BOOT_RAM_TZCfg+0x188>
30004d68:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
30004d6c:	f8df 9168 	ldr.w	r9, [pc, #360]	; 30004ed8 <BOOT_RAM_TZCfg+0x18c>
30004d70:	9202      	str	r2, [sp, #8]
30004d72:	9303      	str	r3, [sp, #12]
30004d74:	e9cd 6100 	strd	r6, r1, [sp]
30004d78:	f85c 2b04 	ldr.w	r2, [ip], #4
30004d7c:	2300      	movs	r3, #0
30004d7e:	f85e 1b04 	ldr.w	r1, [lr], #4
30004d82:	680c      	ldr	r4, [r1, #0]
30004d84:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30004d88:	fa05 f003 	lsl.w	r0, r5, r3
30004d8c:	f1b4 3fff 	cmp.w	r4, #4294967295
30004d90:	d00c      	beq.n	30004dac <BOOT_RAM_TZCfg+0x60>
30004d92:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30004d96:	3301      	adds	r3, #1
30004d98:	684c      	ldr	r4, [r1, #4]
30004d9a:	310c      	adds	r1, #12
30004d9c:	2b08      	cmp	r3, #8
30004d9e:	f8cb 4004 	str.w	r4, [fp, #4]
30004da2:	6c14      	ldr	r4, [r2, #64]	; 0x40
30004da4:	ea40 0004 	orr.w	r0, r0, r4
30004da8:	6410      	str	r0, [r2, #64]	; 0x40
30004daa:	d1ea      	bne.n	30004d82 <BOOT_RAM_TZCfg+0x36>
30004dac:	42b2      	cmp	r2, r6
30004dae:	d06e      	beq.n	30004e8e <BOOT_RAM_TZCfg+0x142>
30004db0:	6455      	str	r5, [r2, #68]	; 0x44
30004db2:	45bc      	cmp	ip, r7
30004db4:	d1e0      	bne.n	30004d78 <BOOT_RAM_TZCfg+0x2c>
30004db6:	4b41      	ldr	r3, [pc, #260]	; (30004ebc <BOOT_RAM_TZCfg+0x170>)
30004db8:	f46f 3400 	mvn.w	r4, #131072	; 0x20000
30004dbc:	f04f 30ff 	mov.w	r0, #4294967295
30004dc0:	4a3f      	ldr	r2, [pc, #252]	; (30004ec0 <BOOT_RAM_TZCfg+0x174>)
30004dc2:	601c      	str	r4, [r3, #0]
30004dc4:	2100      	movs	r1, #0
30004dc6:	4c3f      	ldr	r4, [pc, #252]	; (30004ec4 <BOOT_RAM_TZCfg+0x178>)
30004dc8:	6058      	str	r0, [r3, #4]
30004dca:	6813      	ldr	r3, [r2, #0]
30004dcc:	3301      	adds	r3, #1
30004dce:	d019      	beq.n	30004e04 <BOOT_RAM_TZCfg+0xb8>
30004dd0:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
30004dd4:	320c      	adds	r2, #12
30004dd6:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30004dda:	3101      	adds	r1, #1
30004ddc:	f023 031f 	bic.w	r3, r3, #31
30004de0:	2908      	cmp	r1, #8
30004de2:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
30004de6:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30004dea:	ea4f 0343 	mov.w	r3, r3, lsl #1
30004dee:	f020 001f 	bic.w	r0, r0, #31
30004df2:	f003 0302 	and.w	r3, r3, #2
30004df6:	ea43 0300 	orr.w	r3, r3, r0
30004dfa:	f043 0301 	orr.w	r3, r3, #1
30004dfe:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
30004e02:	d1e2      	bne.n	30004dca <BOOT_RAM_TZCfg+0x7e>
30004e04:	482f      	ldr	r0, [pc, #188]	; (30004ec4 <BOOT_RAM_TZCfg+0x178>)
30004e06:	2201      	movs	r2, #1
30004e08:	4b2f      	ldr	r3, [pc, #188]	; (30004ec8 <BOOT_RAM_TZCfg+0x17c>)
30004e0a:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30004e0e:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004e12:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30004e16:	d11e      	bne.n	30004e56 <BOOT_RAM_TZCfg+0x10a>
30004e18:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004e1c:	f3bf 8f4f 	dsb	sy
30004e20:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004e24:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004e28:	f3c5 344e 	ubfx	r4, r5, #13, #15
30004e2c:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30004e30:	0164      	lsls	r4, r4, #5
30004e32:	ea04 0106 	and.w	r1, r4, r6
30004e36:	462b      	mov	r3, r5
30004e38:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004e3c:	3b01      	subs	r3, #1
30004e3e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004e42:	1c5a      	adds	r2, r3, #1
30004e44:	d1f8      	bne.n	30004e38 <BOOT_RAM_TZCfg+0xec>
30004e46:	3c20      	subs	r4, #32
30004e48:	f114 0f20 	cmn.w	r4, #32
30004e4c:	d1f1      	bne.n	30004e32 <BOOT_RAM_TZCfg+0xe6>
30004e4e:	f3bf 8f4f 	dsb	sy
30004e52:	f3bf 8f6f 	isb	sy
30004e56:	4b1b      	ldr	r3, [pc, #108]	; (30004ec4 <BOOT_RAM_TZCfg+0x178>)
30004e58:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30004e5c:	491b      	ldr	r1, [pc, #108]	; (30004ecc <BOOT_RAM_TZCfg+0x180>)
30004e5e:	691c      	ldr	r4, [r3, #16]
30004e60:	4a1b      	ldr	r2, [pc, #108]	; (30004ed0 <BOOT_RAM_TZCfg+0x184>)
30004e62:	f044 0408 	orr.w	r4, r4, #8
30004e66:	611c      	str	r4, [r3, #16]
30004e68:	f04f 34ff 	mov.w	r4, #4294967295
30004e6c:	68d8      	ldr	r0, [r3, #12]
30004e6e:	4028      	ands	r0, r5
30004e70:	4301      	orrs	r1, r0
30004e72:	60d9      	str	r1, [r3, #12]
30004e74:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30004e78:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30004e7c:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30004e80:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30004e84:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30004e88:	b005      	add	sp, #20
30004e8a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004e8e:	f898 3000 	ldrb.w	r3, [r8]
30004e92:	2b00      	cmp	r3, #0
30004e94:	d18d      	bne.n	30004db2 <BOOT_RAM_TZCfg+0x66>
30004e96:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
30004e9a:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
30004e9e:	6c33      	ldr	r3, [r6, #64]	; 0x40
30004ea0:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30004ea4:	6433      	str	r3, [r6, #64]	; 0x40
30004ea6:	6475      	str	r5, [r6, #68]	; 0x44
30004ea8:	e783      	b.n	30004db2 <BOOT_RAM_TZCfg+0x66>
30004eaa:	bf00      	nop
30004eac:	51001a00 	.word	0x51001a00
30004eb0:	51001200 	.word	0x51001200
30004eb4:	3000dcdc 	.word	0x3000dcdc
30004eb8:	3000dc7c 	.word	0x3000dc7c
30004ebc:	51001b00 	.word	0x51001b00
30004ec0:	3000dd3c 	.word	0x3000dd3c
30004ec4:	e000ed00 	.word	0xe000ed00
30004ec8:	42008000 	.word	0x42008000
30004ecc:	05fa6000 	.word	0x05fa6000
30004ed0:	e000e100 	.word	0xe000e100
30004ed4:	3000d80a 	.word	0x3000d80a
30004ed8:	005fffff 	.word	0x005fffff

30004edc <AsciiToHex>:
30004edc:	f1a0 0341 	sub.w	r3, r0, #65	; 0x41
30004ee0:	2b05      	cmp	r3, #5
30004ee2:	d802      	bhi.n	30004eea <AsciiToHex+0xe>
30004ee4:	3837      	subs	r0, #55	; 0x37
30004ee6:	b2c0      	uxtb	r0, r0
30004ee8:	4770      	bx	lr
30004eea:	f1a0 0361 	sub.w	r3, r0, #97	; 0x61
30004eee:	2b05      	cmp	r3, #5
30004ef0:	d802      	bhi.n	30004ef8 <AsciiToHex+0x1c>
30004ef2:	3857      	subs	r0, #87	; 0x57
30004ef4:	b2c0      	uxtb	r0, r0
30004ef6:	4770      	bx	lr
30004ef8:	3830      	subs	r0, #48	; 0x30
30004efa:	b2c0      	uxtb	r0, r0
30004efc:	280a      	cmp	r0, #10
30004efe:	bf28      	it	cs
30004f00:	20ff      	movcs	r0, #255	; 0xff
30004f02:	4770      	bx	lr

30004f04 <hotfix_get_addr_size>:
30004f04:	2800      	cmp	r0, #0
30004f06:	d054      	beq.n	30004fb2 <hotfix_get_addr_size+0xae>
30004f08:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004f0c:	f04f 0a00 	mov.w	sl, #0
30004f10:	b083      	sub	sp, #12
30004f12:	f1a3 0b04 	sub.w	fp, r3, #4
30004f16:	1c4b      	adds	r3, r1, #1
30004f18:	4688      	mov	r8, r1
30004f1a:	4681      	mov	r9, r0
30004f1c:	9300      	str	r3, [sp, #0]
30004f1e:	4617      	mov	r7, r2
30004f20:	1e53      	subs	r3, r2, #1
30004f22:	f101 0409 	add.w	r4, r1, #9
30004f26:	1dd5      	adds	r5, r2, #7
30004f28:	4656      	mov	r6, sl
30004f2a:	9301      	str	r3, [sp, #4]
30004f2c:	f8c8 6004 	str.w	r6, [r8, #4]
30004f30:	210b      	movs	r1, #11
30004f32:	f8c8 6000 	str.w	r6, [r8]
30004f36:	4640      	mov	r0, r8
30004f38:	f8c8 6007 	str.w	r6, [r8, #7]
30004f3c:	603e      	str	r6, [r7, #0]
30004f3e:	607e      	str	r6, [r7, #4]
30004f40:	f002 fefe 	bl	30007d40 <ymodem_uart_getdata>
30004f44:	e9dd 1000 	ldrd	r1, r0, [sp]
30004f48:	e004      	b.n	30004f54 <hotfix_get_addr_size+0x50>
30004f4a:	b2d3      	uxtb	r3, r2
30004f4c:	428c      	cmp	r4, r1
30004f4e:	f800 3f01 	strb.w	r3, [r0, #1]!
30004f52:	d01a      	beq.n	30004f8a <hotfix_get_addr_size+0x86>
30004f54:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30004f58:	f1a2 0e41 	sub.w	lr, r2, #65	; 0x41
30004f5c:	f1a2 0337 	sub.w	r3, r2, #55	; 0x37
30004f60:	f1a2 0c61 	sub.w	ip, r2, #97	; 0x61
30004f64:	f1be 0f05 	cmp.w	lr, #5
30004f68:	b2db      	uxtb	r3, r3
30004f6a:	d9ef      	bls.n	30004f4c <hotfix_get_addr_size+0x48>
30004f6c:	f1a2 0330 	sub.w	r3, r2, #48	; 0x30
30004f70:	f1bc 0f05 	cmp.w	ip, #5
30004f74:	f1a2 0257 	sub.w	r2, r2, #87	; 0x57
30004f78:	b2db      	uxtb	r3, r3
30004f7a:	d9e6      	bls.n	30004f4a <hotfix_get_addr_size+0x46>
30004f7c:	2b0a      	cmp	r3, #10
30004f7e:	bf28      	it	cs
30004f80:	23ff      	movcs	r3, #255	; 0xff
30004f82:	428c      	cmp	r4, r1
30004f84:	f800 3f01 	strb.w	r3, [r0, #1]!
30004f88:	d1e4      	bne.n	30004f54 <hotfix_get_addr_size+0x50>
30004f8a:	4639      	mov	r1, r7
30004f8c:	2300      	movs	r3, #0
30004f8e:	f811 2b01 	ldrb.w	r2, [r1], #1
30004f92:	4313      	orrs	r3, r2
30004f94:	42a9      	cmp	r1, r5
30004f96:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004f9a:	d1f8      	bne.n	30004f8e <hotfix_get_addr_size+0x8a>
30004f9c:	79fa      	ldrb	r2, [r7, #7]
30004f9e:	f10a 0a01 	add.w	sl, sl, #1
30004fa2:	4313      	orrs	r3, r2
30004fa4:	45ca      	cmp	sl, r9
30004fa6:	f84b 3f04 	str.w	r3, [fp, #4]!
30004faa:	d1bf      	bne.n	30004f2c <hotfix_get_addr_size+0x28>
30004fac:	b003      	add	sp, #12
30004fae:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004fb2:	4770      	bx	lr

30004fb4 <ascii_to_integer>:
30004fb4:	2900      	cmp	r1, #0
30004fb6:	4684      	mov	ip, r0
30004fb8:	dd12      	ble.n	30004fe0 <ascii_to_integer+0x2c>
30004fba:	3901      	subs	r1, #1
30004fbc:	1e42      	subs	r2, r0, #1
30004fbe:	2000      	movs	r0, #0
30004fc0:	448c      	add	ip, r1
30004fc2:	e001      	b.n	30004fc8 <ascii_to_integer+0x14>
30004fc4:	4562      	cmp	r2, ip
30004fc6:	d00d      	beq.n	30004fe4 <ascii_to_integer+0x30>
30004fc8:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30004fcc:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30004fd0:	f1a3 0130 	sub.w	r1, r3, #48	; 0x30
30004fd4:	eb03 0040 	add.w	r0, r3, r0, lsl #1
30004fd8:	2909      	cmp	r1, #9
30004fda:	f1a0 0030 	sub.w	r0, r0, #48	; 0x30
30004fde:	d9f1      	bls.n	30004fc4 <ascii_to_integer+0x10>
30004fe0:	2000      	movs	r0, #0
30004fe2:	4770      	bx	lr
30004fe4:	4770      	bx	lr
30004fe6:	bf00      	nop

30004fe8 <eight_4bit_to_int>:
30004fe8:	2300      	movs	r3, #0
30004fea:	f100 0c07 	add.w	ip, r0, #7
30004fee:	4602      	mov	r2, r0
30004ff0:	f812 1b01 	ldrb.w	r1, [r2], #1
30004ff4:	430b      	orrs	r3, r1
30004ff6:	4562      	cmp	r2, ip
30004ff8:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004ffc:	d1f8      	bne.n	30004ff0 <eight_4bit_to_int+0x8>
30004ffe:	79c0      	ldrb	r0, [r0, #7]
30005000:	4318      	orrs	r0, r3
30005002:	4770      	bx	lr

30005004 <FLASH_Erase_With_Lock>:
30005004:	4b11      	ldr	r3, [pc, #68]	; (3000504c <FLASH_Erase_With_Lock+0x48>)
30005006:	b510      	push	{r4, lr}
30005008:	460c      	mov	r4, r1
3000500a:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
3000500e:	4798      	blx	r3
30005010:	4b0f      	ldr	r3, [pc, #60]	; (30005050 <FLASH_Erase_With_Lock+0x4c>)
30005012:	695b      	ldr	r3, [r3, #20]
30005014:	03db      	lsls	r3, r3, #15
30005016:	d512      	bpl.n	3000503e <FLASH_Erase_With_Lock+0x3a>
30005018:	f014 021f 	ands.w	r2, r4, #31
3000501c:	d110      	bne.n	30005040 <FLASH_Erase_With_Lock+0x3c>
3000501e:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30005022:	f3bf 8f4f 	dsb	sy
30005026:	480a      	ldr	r0, [pc, #40]	; (30005050 <FLASH_Erase_With_Lock+0x4c>)
30005028:	4422      	add	r2, r4
3000502a:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
3000502e:	3420      	adds	r4, #32
30005030:	1b13      	subs	r3, r2, r4
30005032:	2b00      	cmp	r3, #0
30005034:	dcf9      	bgt.n	3000502a <FLASH_Erase_With_Lock+0x26>
30005036:	f3bf 8f4f 	dsb	sy
3000503a:	f3bf 8f6f 	isb	sy
3000503e:	bd10      	pop	{r4, pc}
30005040:	f024 041f 	bic.w	r4, r4, #31
30005044:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30005048:	e7eb      	b.n	30005022 <FLASH_Erase_With_Lock+0x1e>
3000504a:	bf00      	nop
3000504c:	0000a16d 	.word	0x0000a16d
30005050:	e000ed00 	.word	0xe000ed00

30005054 <FLASH_TxData_With_Lock>:
30005054:	b538      	push	{r3, r4, r5, lr}
30005056:	4b13      	ldr	r3, [pc, #76]	; (300050a4 <FLASH_TxData_With_Lock+0x50>)
30005058:	4604      	mov	r4, r0
3000505a:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
3000505e:	460d      	mov	r5, r1
30005060:	4798      	blx	r3
30005062:	4b11      	ldr	r3, [pc, #68]	; (300050a8 <FLASH_TxData_With_Lock+0x54>)
30005064:	695b      	ldr	r3, [r3, #20]
30005066:	03db      	lsls	r3, r3, #15
30005068:	d517      	bpl.n	3000509a <FLASH_TxData_With_Lock+0x46>
3000506a:	f014 031f 	ands.w	r3, r4, #31
3000506e:	d115      	bne.n	3000509c <FLASH_TxData_With_Lock+0x48>
30005070:	f3bf 8f4f 	dsb	sy
30005074:	b16d      	cbz	r5, 30005092 <FLASH_TxData_With_Lock+0x3e>
30005076:	1e6a      	subs	r2, r5, #1
30005078:	f104 0320 	add.w	r3, r4, #32
3000507c:	490a      	ldr	r1, [pc, #40]	; (300050a8 <FLASH_TxData_With_Lock+0x54>)
3000507e:	f022 021f 	bic.w	r2, r2, #31
30005082:	441a      	add	r2, r3
30005084:	4293      	cmp	r3, r2
30005086:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
3000508a:	461c      	mov	r4, r3
3000508c:	f103 0320 	add.w	r3, r3, #32
30005090:	d1f8      	bne.n	30005084 <FLASH_TxData_With_Lock+0x30>
30005092:	f3bf 8f4f 	dsb	sy
30005096:	f3bf 8f6f 	isb	sy
3000509a:	bd38      	pop	{r3, r4, r5, pc}
3000509c:	f024 041f 	bic.w	r4, r4, #31
300050a0:	441d      	add	r5, r3
300050a2:	e7e5      	b.n	30005070 <FLASH_TxData_With_Lock+0x1c>
300050a4:	0000a04d 	.word	0x0000a04d
300050a8:	e000ed00 	.word	0xe000ed00

300050ac <flash_read_id>:
300050ac:	b538      	push	{r3, r4, r5, lr}
300050ae:	4d04      	ldr	r5, [pc, #16]	; (300050c0 <flash_read_id+0x14>)
300050b0:	460c      	mov	r4, r1
300050b2:	4602      	mov	r2, r0
300050b4:	4b03      	ldr	r3, [pc, #12]	; (300050c4 <flash_read_id+0x18>)
300050b6:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
300050ba:	4798      	blx	r3
300050bc:	4620      	mov	r0, r4
300050be:	bd38      	pop	{r3, r4, r5, pc}
300050c0:	2001c01c 	.word	0x2001c01c
300050c4:	00009db5 	.word	0x00009db5

300050c8 <mcc_ReverseBit>:
300050c8:	0843      	lsrs	r3, r0, #1
300050ca:	08c1      	lsrs	r1, r0, #3
300050cc:	f003 0308 	and.w	r3, r3, #8
300050d0:	0942      	lsrs	r2, r0, #5
300050d2:	f001 0104 	and.w	r1, r1, #4
300050d6:	ea4f 1c40 	mov.w	ip, r0, lsl #5
300050da:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
300050de:	f002 0202 	and.w	r2, r2, #2
300050e2:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
300050e6:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
300050ea:	430b      	orrs	r3, r1
300050ec:	00c1      	lsls	r1, r0, #3
300050ee:	4313      	orrs	r3, r2
300050f0:	f001 0120 	and.w	r1, r1, #32
300050f4:	0042      	lsls	r2, r0, #1
300050f6:	ea43 000c 	orr.w	r0, r3, ip
300050fa:	f002 0310 	and.w	r3, r2, #16
300050fe:	4308      	orrs	r0, r1
30005100:	4318      	orrs	r0, r3
30005102:	b2c0      	uxtb	r0, r0
30005104:	4770      	bx	lr
30005106:	bf00      	nop

30005108 <mcc_init_crc32>:
30005108:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
3000510a:	2600      	movs	r6, #0
3000510c:	4f16      	ldr	r7, [pc, #88]	; (30005168 <mcc_init_crc32+0x60>)
3000510e:	4605      	mov	r5, r0
30005110:	b2f0      	uxtb	r0, r6
30005112:	f7ff ffd9 	bl	300050c8 <mcc_ReverseBit>
30005116:	2208      	movs	r2, #8
30005118:	0604      	lsls	r4, r0, #24
3000511a:	2c00      	cmp	r4, #0
3000511c:	ea4f 0444 	mov.w	r4, r4, lsl #1
30005120:	bfb8      	it	lt
30005122:	407c      	eorlt	r4, r7
30005124:	3a01      	subs	r2, #1
30005126:	d1f8      	bne.n	3000511a <mcc_init_crc32+0x12>
30005128:	0e20      	lsrs	r0, r4, #24
3000512a:	3504      	adds	r5, #4
3000512c:	f7ff ffcc 	bl	300050c8 <mcc_ReverseBit>
30005130:	4603      	mov	r3, r0
30005132:	f3c4 4007 	ubfx	r0, r4, #16, #8
30005136:	3601      	adds	r6, #1
30005138:	f805 3c04 	strb.w	r3, [r5, #-4]
3000513c:	f7ff ffc4 	bl	300050c8 <mcc_ReverseBit>
30005140:	4603      	mov	r3, r0
30005142:	f3c4 2007 	ubfx	r0, r4, #8, #8
30005146:	f805 3c03 	strb.w	r3, [r5, #-3]
3000514a:	f7ff ffbd 	bl	300050c8 <mcc_ReverseBit>
3000514e:	4603      	mov	r3, r0
30005150:	b2e0      	uxtb	r0, r4
30005152:	f805 3c02 	strb.w	r3, [r5, #-2]
30005156:	f7ff ffb7 	bl	300050c8 <mcc_ReverseBit>
3000515a:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
3000515e:	f805 0c01 	strb.w	r0, [r5, #-1]
30005162:	d1d5      	bne.n	30005110 <mcc_init_crc32+0x8>
30005164:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30005166:	bf00      	nop
30005168:	04c11db7 	.word	0x04c11db7

3000516c <mcc_crc32>:
3000516c:	b179      	cbz	r1, 3000518e <mcc_crc32+0x22>
3000516e:	eb00 0c01 	add.w	ip, r0, r1
30005172:	f04f 31ff 	mov.w	r1, #4294967295
30005176:	f810 3b01 	ldrb.w	r3, [r0], #1
3000517a:	404b      	eors	r3, r1
3000517c:	4560      	cmp	r0, ip
3000517e:	b2db      	uxtb	r3, r3
30005180:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30005184:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30005188:	d1f5      	bne.n	30005176 <mcc_crc32+0xa>
3000518a:	43c8      	mvns	r0, r1
3000518c:	4770      	bx	lr
3000518e:	4608      	mov	r0, r1
30005190:	4770      	bx	lr
30005192:	bf00      	nop

30005194 <recovery_check>:
30005194:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30005198:	4283      	cmp	r3, r0
3000519a:	d322      	bcc.n	300051e2 <recovery_check+0x4e>
3000519c:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
300051a0:	d91f      	bls.n	300051e2 <recovery_check+0x4e>
300051a2:	b510      	push	{r4, lr}
300051a4:	f8d0 400a 	ldr.w	r4, [r0, #10]
300051a8:	f104 030c 	add.w	r3, r4, #12
300051ac:	4299      	cmp	r1, r3
300051ae:	d316      	bcc.n	300051de <recovery_check+0x4a>
300051b0:	f100 0c04 	add.w	ip, r0, #4
300051b4:	b183      	cbz	r3, 300051d8 <recovery_check+0x44>
300051b6:	f100 0e10 	add.w	lr, r0, #16
300051ba:	f04f 31ff 	mov.w	r1, #4294967295
300051be:	44a6      	add	lr, r4
300051c0:	4c0b      	ldr	r4, [pc, #44]	; (300051f0 <recovery_check+0x5c>)
300051c2:	f81c 3b01 	ldrb.w	r3, [ip], #1
300051c6:	404b      	eors	r3, r1
300051c8:	45f4      	cmp	ip, lr
300051ca:	b2db      	uxtb	r3, r3
300051cc:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051d0:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
300051d4:	d1f5      	bne.n	300051c2 <recovery_check+0x2e>
300051d6:	43cb      	mvns	r3, r1
300051d8:	6801      	ldr	r1, [r0, #0]
300051da:	4299      	cmp	r1, r3
300051dc:	d003      	beq.n	300051e6 <recovery_check+0x52>
300051de:	2000      	movs	r0, #0
300051e0:	bd10      	pop	{r4, pc}
300051e2:	2000      	movs	r0, #0
300051e4:	4770      	bx	lr
300051e6:	f8d0 3006 	ldr.w	r3, [r0, #6]
300051ea:	2001      	movs	r0, #1
300051ec:	6013      	str	r3, [r2, #0]
300051ee:	bd10      	pop	{r4, pc}
300051f0:	3000dd9c 	.word	0x3000dd9c

300051f4 <rewrite_bp>:
300051f4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300051f8:	b0a3      	sub	sp, #140	; 0x8c
300051fa:	4616      	mov	r6, r2
300051fc:	4607      	mov	r7, r0
300051fe:	460d      	mov	r5, r1
30005200:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
30005204:	4699      	mov	r9, r3
30005206:	f10d 0808 	add.w	r8, sp, #8
3000520a:	2280      	movs	r2, #128	; 0x80
3000520c:	2c00      	cmp	r4, #0
3000520e:	f000 80b2 	beq.w	30005376 <rewrite_bp+0x182>
30005212:	4619      	mov	r1, r3
30005214:	4640      	mov	r0, r8
30005216:	f006 ffef 	bl	3000c1f8 <____wrap_memcpy_veneer>
3000521a:	2301      	movs	r3, #1
3000521c:	f109 0404 	add.w	r4, r9, #4
30005220:	4649      	mov	r1, r9
30005222:	2002      	movs	r0, #2
30005224:	f88d 7014 	strb.w	r7, [sp, #20]
30005228:	e9cd 3303 	strd	r3, r3, [sp, #12]
3000522c:	f7ff feea 	bl	30005004 <FLASH_Erase_With_Lock>
30005230:	4620      	mov	r0, r4
30005232:	217c      	movs	r1, #124	; 0x7c
30005234:	aa03      	add	r2, sp, #12
30005236:	f7ff ff0d 	bl	30005054 <FLASH_TxData_With_Lock>
3000523a:	4620      	mov	r0, r4
3000523c:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
30005240:	f04f 33ff 	mov.w	r3, #4294967295
30005244:	4c5c      	ldr	r4, [pc, #368]	; (300053b8 <rewrite_bp+0x1c4>)
30005246:	f810 cb01 	ldrb.w	ip, [r0], #1
3000524a:	ea8c 0c03 	eor.w	ip, ip, r3
3000524e:	4288      	cmp	r0, r1
30005250:	fa5f fc8c 	uxtb.w	ip, ip
30005254:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
30005258:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
3000525c:	d1f3      	bne.n	30005246 <rewrite_bp+0x52>
3000525e:	43db      	mvns	r3, r3
30005260:	4649      	mov	r1, r9
30005262:	2002      	movs	r0, #2
30005264:	9302      	str	r3, [sp, #8]
30005266:	f7ff fecd 	bl	30005004 <FLASH_Erase_With_Lock>
3000526a:	4642      	mov	r2, r8
3000526c:	4648      	mov	r0, r9
3000526e:	2180      	movs	r1, #128	; 0x80
30005270:	f7ff fef0 	bl	30005054 <FLASH_TxData_With_Lock>
30005274:	4b51      	ldr	r3, [pc, #324]	; (300053bc <rewrite_bp+0x1c8>)
30005276:	aa01      	add	r2, sp, #4
30005278:	2104      	movs	r1, #4
3000527a:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
3000527e:	4b50      	ldr	r3, [pc, #320]	; (300053c0 <rewrite_bp+0x1cc>)
30005280:	4798      	blx	r3
30005282:	f89d 3006 	ldrb.w	r3, [sp, #6]
30005286:	f04f 0c01 	mov.w	ip, #1
3000528a:	f8d5 e00a 	ldr.w	lr, [r5, #10]
3000528e:	682a      	ldr	r2, [r5, #0]
30005290:	fa0c fc03 	lsl.w	ip, ip, r3
30005294:	f10e 0e0c 	add.w	lr, lr, #12
30005298:	f8d6 300a 	ldr.w	r3, [r6, #10]
3000529c:	45e6      	cmp	lr, ip
3000529e:	6831      	ldr	r1, [r6, #0]
300052a0:	f103 030c 	add.w	r3, r3, #12
300052a4:	bf28      	it	cs
300052a6:	46e6      	movcs	lr, ip
300052a8:	459c      	cmp	ip, r3
300052aa:	bf28      	it	cs
300052ac:	469c      	movcs	ip, r3
300052ae:	bb9f      	cbnz	r7, 30005318 <rewrite_bp+0x124>
300052b0:	1d28      	adds	r0, r5, #4
300052b2:	f1be 0f00 	cmp.w	lr, #0
300052b6:	d00e      	beq.n	300052d6 <rewrite_bp+0xe2>
300052b8:	4486      	add	lr, r0
300052ba:	f04f 37ff 	mov.w	r7, #4294967295
300052be:	f810 3b01 	ldrb.w	r3, [r0], #1
300052c2:	407b      	eors	r3, r7
300052c4:	4570      	cmp	r0, lr
300052c6:	b2db      	uxtb	r3, r3
300052c8:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300052cc:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300052d0:	d1f5      	bne.n	300052be <rewrite_bp+0xca>
300052d2:	ea6f 0e07 	mvn.w	lr, r7
300052d6:	4572      	cmp	r2, lr
300052d8:	d052      	beq.n	30005380 <rewrite_bp+0x18c>
300052da:	1d32      	adds	r2, r6, #4
300052dc:	f1bc 0f00 	cmp.w	ip, #0
300052e0:	d00e      	beq.n	30005300 <rewrite_bp+0x10c>
300052e2:	4494      	add	ip, r2
300052e4:	f04f 37ff 	mov.w	r7, #4294967295
300052e8:	f812 3b01 	ldrb.w	r3, [r2], #1
300052ec:	407b      	eors	r3, r7
300052ee:	4562      	cmp	r2, ip
300052f0:	b2db      	uxtb	r3, r3
300052f2:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300052f6:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300052fa:	d1f5      	bne.n	300052e8 <rewrite_bp+0xf4>
300052fc:	ea6f 0c07 	mvn.w	ip, r7
30005300:	4561      	cmp	r1, ip
30005302:	d133      	bne.n	3000536c <rewrite_bp+0x178>
30005304:	4b2f      	ldr	r3, [pc, #188]	; (300053c4 <rewrite_bp+0x1d0>)
30005306:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
3000530a:	689b      	ldr	r3, [r3, #8]
3000530c:	0799      	lsls	r1, r3, #30
3000530e:	d44c      	bmi.n	300053aa <rewrite_bp+0x1b6>
30005310:	2002      	movs	r0, #2
30005312:	b023      	add	sp, #140	; 0x8c
30005314:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30005318:	1d30      	adds	r0, r6, #4
3000531a:	f1bc 0f00 	cmp.w	ip, #0
3000531e:	d00e      	beq.n	3000533e <rewrite_bp+0x14a>
30005320:	4484      	add	ip, r0
30005322:	f04f 37ff 	mov.w	r7, #4294967295
30005326:	f810 3b01 	ldrb.w	r3, [r0], #1
3000532a:	407b      	eors	r3, r7
3000532c:	4560      	cmp	r0, ip
3000532e:	b2db      	uxtb	r3, r3
30005330:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005334:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30005338:	d1f5      	bne.n	30005326 <rewrite_bp+0x132>
3000533a:	ea6f 0c07 	mvn.w	ip, r7
3000533e:	4561      	cmp	r1, ip
30005340:	d0e0      	beq.n	30005304 <rewrite_bp+0x110>
30005342:	1d29      	adds	r1, r5, #4
30005344:	f1be 0f00 	cmp.w	lr, #0
30005348:	d00e      	beq.n	30005368 <rewrite_bp+0x174>
3000534a:	448e      	add	lr, r1
3000534c:	f04f 37ff 	mov.w	r7, #4294967295
30005350:	f811 3b01 	ldrb.w	r3, [r1], #1
30005354:	407b      	eors	r3, r7
30005356:	4571      	cmp	r1, lr
30005358:	b2db      	uxtb	r3, r3
3000535a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000535e:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30005362:	d1f5      	bne.n	30005350 <rewrite_bp+0x15c>
30005364:	ea6f 0e07 	mvn.w	lr, r7
30005368:	4572      	cmp	r2, lr
3000536a:	d009      	beq.n	30005380 <rewrite_bp+0x18c>
3000536c:	4b15      	ldr	r3, [pc, #84]	; (300053c4 <rewrite_bp+0x1d0>)
3000536e:	689b      	ldr	r3, [r3, #8]
30005370:	079b      	lsls	r3, r3, #30
30005372:	d40f      	bmi.n	30005394 <rewrite_bp+0x1a0>
30005374:	e7fe      	b.n	30005374 <rewrite_bp+0x180>
30005376:	992a      	ldr	r1, [sp, #168]	; 0xa8
30005378:	4640      	mov	r0, r8
3000537a:	f006 ff3d 	bl	3000c1f8 <____wrap_memcpy_veneer>
3000537e:	e74c      	b.n	3000521a <rewrite_bp+0x26>
30005380:	4b10      	ldr	r3, [pc, #64]	; (300053c4 <rewrite_bp+0x1d0>)
30005382:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
30005386:	689b      	ldr	r3, [r3, #8]
30005388:	079a      	lsls	r2, r3, #30
3000538a:	d407      	bmi.n	3000539c <rewrite_bp+0x1a8>
3000538c:	2001      	movs	r0, #1
3000538e:	b023      	add	sp, #140	; 0x8c
30005390:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30005394:	480c      	ldr	r0, [pc, #48]	; (300053c8 <rewrite_bp+0x1d4>)
30005396:	f006 ff47 	bl	3000c228 <__DiagPrintf_veneer>
3000539a:	e7eb      	b.n	30005374 <rewrite_bp+0x180>
3000539c:	f8d5 1006 	ldr.w	r1, [r5, #6]
300053a0:	480a      	ldr	r0, [pc, #40]	; (300053cc <rewrite_bp+0x1d8>)
300053a2:	f006 ff41 	bl	3000c228 <__DiagPrintf_veneer>
300053a6:	2001      	movs	r0, #1
300053a8:	e7b3      	b.n	30005312 <rewrite_bp+0x11e>
300053aa:	f8d6 1006 	ldr.w	r1, [r6, #6]
300053ae:	4808      	ldr	r0, [pc, #32]	; (300053d0 <rewrite_bp+0x1dc>)
300053b0:	f006 ff3a 	bl	3000c228 <__DiagPrintf_veneer>
300053b4:	e7ac      	b.n	30005310 <rewrite_bp+0x11c>
300053b6:	bf00      	nop
300053b8:	3000dd9c 	.word	0x3000dd9c
300053bc:	2001c01c 	.word	0x2001c01c
300053c0:	00009db5 	.word	0x00009db5
300053c4:	2001c00c 	.word	0x2001c00c
300053c8:	3000c92c 	.word	0x3000c92c
300053cc:	3000c8cc 	.word	0x3000c8cc
300053d0:	3000c8fc 	.word	0x3000c8fc

300053d4 <BOOT_SectionInit>:
300053d4:	4800      	ldr	r0, [pc, #0]	; (300053d8 <BOOT_SectionInit+0x4>)
300053d6:	4770      	bx	lr
300053d8:	60000020 	.word	0x60000020

300053dc <BOOT_NsStart>:
300053dc:	b570      	push	{r4, r5, r6, lr}
300053de:	4c23      	ldr	r4, [pc, #140]	; (3000546c <BOOT_NsStart+0x90>)
300053e0:	4605      	mov	r5, r0
300053e2:	f384 8808 	msr	MSP, r4
300053e6:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300053ea:	2100      	movs	r1, #0
300053ec:	4620      	mov	r0, r4
300053ee:	4e20      	ldr	r6, [pc, #128]	; (30005470 <BOOT_NsStart+0x94>)
300053f0:	47b0      	blx	r6
300053f2:	4a20      	ldr	r2, [pc, #128]	; (30005474 <BOOT_NsStart+0x98>)
300053f4:	6953      	ldr	r3, [r2, #20]
300053f6:	03d9      	lsls	r1, r3, #15
300053f8:	d50b      	bpl.n	30005412 <BOOT_NsStart+0x36>
300053fa:	f3bf 8f4f 	dsb	sy
300053fe:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
30005402:	3420      	adds	r4, #32
30005404:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
30005408:	d1f9      	bne.n	300053fe <BOOT_NsStart+0x22>
3000540a:	f3bf 8f4f 	dsb	sy
3000540e:	f3bf 8f6f 	isb	sy
30005412:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
30005416:	f383 8808 	msr	MSP, r3
3000541a:	f44f 5280 	mov.w	r2, #4096	; 0x1000
3000541e:	2100      	movs	r1, #0
30005420:	4815      	ldr	r0, [pc, #84]	; (30005478 <BOOT_NsStart+0x9c>)
30005422:	47b0      	blx	r6
30005424:	4a13      	ldr	r2, [pc, #76]	; (30005474 <BOOT_NsStart+0x98>)
30005426:	6953      	ldr	r3, [r2, #20]
30005428:	03db      	lsls	r3, r3, #15
3000542a:	d50c      	bpl.n	30005446 <BOOT_NsStart+0x6a>
3000542c:	f3bf 8f4f 	dsb	sy
30005430:	4b11      	ldr	r3, [pc, #68]	; (30005478 <BOOT_NsStart+0x9c>)
30005432:	490e      	ldr	r1, [pc, #56]	; (3000546c <BOOT_NsStart+0x90>)
30005434:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30005438:	3320      	adds	r3, #32
3000543a:	428b      	cmp	r3, r1
3000543c:	d1fa      	bne.n	30005434 <BOOT_NsStart+0x58>
3000543e:	f3bf 8f4f 	dsb	sy
30005442:	f3bf 8f6f 	isb	sy
30005446:	f025 0501 	bic.w	r5, r5, #1
3000544a:	086d      	lsrs	r5, r5, #1
3000544c:	006d      	lsls	r5, r5, #1
3000544e:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005452:	ed2d 8b10 	vpush	{d8-d15}
30005456:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
3000545a:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
3000545e:	47ac      	blxns	r5
30005460:	ecbd 8b10 	vpop	{d8-d15}
30005464:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005468:	bd70      	pop	{r4, r5, r6, pc}
3000546a:	bf00      	nop
3000546c:	30002000 	.word	0x30002000
30005470:	00012be5 	.word	0x00012be5
30005474:	e000ed00 	.word	0xe000ed00
30005478:	30001000 	.word	0x30001000

3000547c <BOOT_RccConfig>:
3000547c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30005480:	4c2f      	ldr	r4, [pc, #188]	; (30005540 <BOOT_RccConfig+0xc4>)
30005482:	b090      	sub	sp, #64	; 0x40
30005484:	4f2f      	ldr	r7, [pc, #188]	; (30005544 <BOOT_RccConfig+0xc8>)
30005486:	466d      	mov	r5, sp
30005488:	f10d 0c20 	add.w	ip, sp, #32
3000548c:	683e      	ldr	r6, [r7, #0]
3000548e:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
30005492:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
30005496:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
3000549a:	f104 0310 	add.w	r3, r4, #16
3000549e:	2400      	movs	r4, #0
300054a0:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300054a2:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
300054a6:	1c72      	adds	r2, r6, #1
300054a8:	e9cd 4404 	strd	r4, r4, [sp, #16]
300054ac:	e9cd 4406 	strd	r4, r4, [sp, #24]
300054b0:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
300054b4:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
300054b8:	d03f      	beq.n	3000553a <BOOT_RccConfig+0xbe>
300054ba:	463a      	mov	r2, r7
300054bc:	6891      	ldr	r1, [r2, #8]
300054be:	0fb3      	lsrs	r3, r6, #30
300054c0:	a810      	add	r0, sp, #64	; 0x40
300054c2:	2901      	cmp	r1, #1
300054c4:	eb00 0383 	add.w	r3, r0, r3, lsl #2
300054c8:	d10e      	bne.n	300054e8 <BOOT_RccConfig+0x6c>
300054ca:	f853 0c30 	ldr.w	r0, [r3, #-48]
300054ce:	6854      	ldr	r4, [r2, #4]
300054d0:	4330      	orrs	r0, r6
300054d2:	ae10      	add	r6, sp, #64	; 0x40
300054d4:	0fa1      	lsrs	r1, r4, #30
300054d6:	f843 0c30 	str.w	r0, [r3, #-48]
300054da:	eb06 0181 	add.w	r1, r6, r1, lsl #2
300054de:	f851 3c10 	ldr.w	r3, [r1, #-16]
300054e2:	4323      	orrs	r3, r4
300054e4:	f841 3c10 	str.w	r3, [r1, #-16]
300054e8:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300054ec:	1c73      	adds	r3, r6, #1
300054ee:	d1e5      	bne.n	300054bc <BOOT_RccConfig+0x40>
300054f0:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300054f2:	9c04      	ldr	r4, [sp, #16]
300054f4:	af0d      	add	r7, sp, #52	; 0x34
300054f6:	ae05      	add	r6, sp, #20
300054f8:	4a13      	ldr	r2, [pc, #76]	; (30005548 <BOOT_RccConfig+0xcc>)
300054fa:	f85c 0b04 	ldr.w	r0, [ip], #4
300054fe:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
30005502:	f855 1b04 	ldr.w	r1, [r5], #4
30005506:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
3000550a:	f850 8002 	ldr.w	r8, [r0, r2]
3000550e:	45e6      	cmp	lr, ip
30005510:	ea43 0308 	orr.w	r3, r3, r8
30005514:	5083      	str	r3, [r0, r2]
30005516:	588b      	ldr	r3, [r1, r2]
30005518:	ea44 0403 	orr.w	r4, r4, r3
3000551c:	508c      	str	r4, [r1, r2]
3000551e:	d004      	beq.n	3000552a <BOOT_RccConfig+0xae>
30005520:	f857 3b04 	ldr.w	r3, [r7], #4
30005524:	f856 4b04 	ldr.w	r4, [r6], #4
30005528:	e7e7      	b.n	300054fa <BOOT_RccConfig+0x7e>
3000552a:	200a      	movs	r0, #10
3000552c:	4b07      	ldr	r3, [pc, #28]	; (3000554c <BOOT_RccConfig+0xd0>)
3000552e:	4798      	blx	r3
30005530:	4b07      	ldr	r3, [pc, #28]	; (30005550 <BOOT_RccConfig+0xd4>)
30005532:	b010      	add	sp, #64	; 0x40
30005534:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30005538:	4718      	bx	r3
3000553a:	4623      	mov	r3, r4
3000553c:	e7da      	b.n	300054f4 <BOOT_RccConfig+0x78>
3000553e:	bf00      	nop
30005540:	3000d7a0 	.word	0x3000d7a0
30005544:	3000d8f4 	.word	0x3000d8f4
30005548:	42008000 	.word	0x42008000
3000554c:	00009b2d 	.word	0x00009b2d
30005550:	30008049 	.word	0x30008049

30005554 <BOOT_CACHEWRR_Set>:
30005554:	4b14      	ldr	r3, [pc, #80]	; (300055a8 <BOOT_CACHEWRR_Set+0x54>)
30005556:	b430      	push	{r4, r5}
30005558:	681a      	ldr	r2, [r3, #0]
3000555a:	2a01      	cmp	r2, #1
3000555c:	d10f      	bne.n	3000557e <BOOT_CACHEWRR_Set+0x2a>
3000555e:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
30005562:	685d      	ldr	r5, [r3, #4]
30005564:	689c      	ldr	r4, [r3, #8]
30005566:	6882      	ldr	r2, [r0, #8]
30005568:	4910      	ldr	r1, [pc, #64]	; (300055ac <BOOT_CACHEWRR_Set+0x58>)
3000556a:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
3000556e:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
30005572:	6082      	str	r2, [r0, #8]
30005574:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30005578:	68da      	ldr	r2, [r3, #12]
3000557a:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
3000557e:	691a      	ldr	r2, [r3, #16]
30005580:	2a01      	cmp	r2, #1
30005582:	d10f      	bne.n	300055a4 <BOOT_CACHEWRR_Set+0x50>
30005584:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30005588:	695d      	ldr	r5, [r3, #20]
3000558a:	4a08      	ldr	r2, [pc, #32]	; (300055ac <BOOT_CACHEWRR_Set+0x58>)
3000558c:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30005590:	688b      	ldr	r3, [r1, #8]
30005592:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
30005596:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
3000559a:	608b      	str	r3, [r1, #8]
3000559c:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
300055a0:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
300055a4:	bc30      	pop	{r4, r5}
300055a6:	4770      	bx	lr
300055a8:	3000e404 	.word	0x3000e404
300055ac:	e0042000 	.word	0xe0042000

300055b0 <BOOT_TCMSet>:
300055b0:	2300      	movs	r3, #0
300055b2:	b5f0      	push	{r4, r5, r6, r7, lr}
300055b4:	4db3      	ldr	r5, [pc, #716]	; (30005884 <BOOT_TCMSet+0x2d4>)
300055b6:	461c      	mov	r4, r3
300055b8:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
300055bc:	3307      	adds	r3, #7
300055be:	4282      	cmp	r2, r0
300055c0:	d03c      	beq.n	3000563c <BOOT_TCMSet+0x8c>
300055c2:	3401      	adds	r4, #1
300055c4:	2c07      	cmp	r4, #7
300055c6:	d1f7      	bne.n	300055b8 <BOOT_TCMSet+0x8>
300055c8:	f3bf 8f4f 	dsb	sy
300055cc:	f3bf 8f6f 	isb	sy
300055d0:	48ad      	ldr	r0, [pc, #692]	; (30005888 <BOOT_TCMSet+0x2d8>)
300055d2:	2200      	movs	r2, #0
300055d4:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
300055d8:	f3bf 8f4f 	dsb	sy
300055dc:	f3bf 8f6f 	isb	sy
300055e0:	6943      	ldr	r3, [r0, #20]
300055e2:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
300055e6:	6143      	str	r3, [r0, #20]
300055e8:	f3bf 8f4f 	dsb	sy
300055ec:	f3bf 8f6f 	isb	sy
300055f0:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300055f4:	f3bf 8f4f 	dsb	sy
300055f8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300055fc:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005600:	f3c5 344e 	ubfx	r4, r5, #13, #15
30005604:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30005608:	0164      	lsls	r4, r4, #5
3000560a:	ea04 0106 	and.w	r1, r4, r6
3000560e:	462b      	mov	r3, r5
30005610:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30005614:	3b01      	subs	r3, #1
30005616:	1c5f      	adds	r7, r3, #1
30005618:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
3000561c:	d1f8      	bne.n	30005610 <BOOT_TCMSet+0x60>
3000561e:	3c20      	subs	r4, #32
30005620:	f114 0f20 	cmn.w	r4, #32
30005624:	d1f1      	bne.n	3000560a <BOOT_TCMSet+0x5a>
30005626:	f3bf 8f4f 	dsb	sy
3000562a:	6943      	ldr	r3, [r0, #20]
3000562c:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30005630:	6143      	str	r3, [r0, #20]
30005632:	f3bf 8f4f 	dsb	sy
30005636:	f3bf 8f6f 	isb	sy
3000563a:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000563c:	2c00      	cmp	r4, #0
3000563e:	d0c3      	beq.n	300055c8 <BOOT_TCMSet+0x18>
30005640:	f3bf 8f4f 	dsb	sy
30005644:	f3bf 8f6f 	isb	sy
30005648:	4890      	ldr	r0, [pc, #576]	; (3000588c <BOOT_TCMSet+0x2dc>)
3000564a:	2300      	movs	r3, #0
3000564c:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30005650:	f3bf 8f4f 	dsb	sy
30005654:	f3bf 8f6f 	isb	sy
30005658:	6942      	ldr	r2, [r0, #20]
3000565a:	03d2      	lsls	r2, r2, #15
3000565c:	d520      	bpl.n	300056a0 <BOOT_TCMSet+0xf0>
3000565e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30005662:	f3bf 8f4f 	dsb	sy
30005666:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
3000566a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000566e:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30005672:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005676:	ea4f 1c43 	mov.w	ip, r3, lsl #5
3000567a:	ea0c 0106 	and.w	r1, ip, r6
3000567e:	4673      	mov	r3, lr
30005680:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30005684:	3b01      	subs	r3, #1
30005686:	1c5f      	adds	r7, r3, #1
30005688:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000568c:	d1f8      	bne.n	30005680 <BOOT_TCMSet+0xd0>
3000568e:	f1ac 0c20 	sub.w	ip, ip, #32
30005692:	f11c 0f20 	cmn.w	ip, #32
30005696:	d1f0      	bne.n	3000567a <BOOT_TCMSet+0xca>
30005698:	f3bf 8f4f 	dsb	sy
3000569c:	f3bf 8f6f 	isb	sy
300056a0:	f3bf 8f4f 	dsb	sy
300056a4:	f3bf 8f6f 	isb	sy
300056a8:	4878      	ldr	r0, [pc, #480]	; (3000588c <BOOT_TCMSet+0x2dc>)
300056aa:	2200      	movs	r2, #0
300056ac:	6943      	ldr	r3, [r0, #20]
300056ae:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
300056b2:	6143      	str	r3, [r0, #20]
300056b4:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
300056b8:	f3bf 8f4f 	dsb	sy
300056bc:	f3bf 8f6f 	isb	sy
300056c0:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300056c4:	f3bf 8f4f 	dsb	sy
300056c8:	6943      	ldr	r3, [r0, #20]
300056ca:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300056ce:	6143      	str	r3, [r0, #20]
300056d0:	f3bf 8f4f 	dsb	sy
300056d4:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
300056d8:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300056dc:	f3c3 06c9 	ubfx	r6, r3, #3, #10
300056e0:	f3c3 334e 	ubfx	r3, r3, #13, #15
300056e4:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300056e8:	ea0c 010e 	and.w	r1, ip, lr
300056ec:	4633      	mov	r3, r6
300056ee:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300056f2:	3b01      	subs	r3, #1
300056f4:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300056f8:	1c5a      	adds	r2, r3, #1
300056fa:	d1f8      	bne.n	300056ee <BOOT_TCMSet+0x13e>
300056fc:	f1ac 0c20 	sub.w	ip, ip, #32
30005700:	f11c 0f20 	cmn.w	ip, #32
30005704:	d1f0      	bne.n	300056e8 <BOOT_TCMSet+0x138>
30005706:	f3bf 8f4f 	dsb	sy
3000570a:	f3bf 8f6f 	isb	sy
3000570e:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005712:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
30005716:	00e3      	lsls	r3, r4, #3
30005718:	6b41      	ldr	r1, [r0, #52]	; 0x34
3000571a:	eb05 0282 	add.w	r2, r5, r2, lsl #2
3000571e:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
30005722:	6341      	str	r1, [r0, #52]	; 0x34
30005724:	6911      	ldr	r1, [r2, #16]
30005726:	2903      	cmp	r1, #3
30005728:	d00f      	beq.n	3000574a <BOOT_TCMSet+0x19a>
3000572a:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
3000572e:	4858      	ldr	r0, [pc, #352]	; (30005890 <BOOT_TCMSet+0x2e0>)
30005730:	f04f 0c00 	mov.w	ip, #0
30005734:	4f57      	ldr	r7, [pc, #348]	; (30005894 <BOOT_TCMSet+0x2e4>)
30005736:	68b2      	ldr	r2, [r6, #8]
30005738:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
3000573c:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
30005740:	60b2      	str	r2, [r6, #8]
30005742:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
30005746:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
3000574a:	1b1a      	subs	r2, r3, r4
3000574c:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005750:	68d1      	ldr	r1, [r2, #12]
30005752:	2903      	cmp	r1, #3
30005754:	d00f      	beq.n	30005776 <BOOT_TCMSet+0x1c6>
30005756:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
3000575a:	484d      	ldr	r0, [pc, #308]	; (30005890 <BOOT_TCMSet+0x2e0>)
3000575c:	f04f 0c00 	mov.w	ip, #0
30005760:	4f4c      	ldr	r7, [pc, #304]	; (30005894 <BOOT_TCMSet+0x2e4>)
30005762:	68b2      	ldr	r2, [r6, #8]
30005764:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30005768:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
3000576c:	60b2      	str	r2, [r6, #8]
3000576e:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
30005772:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
30005776:	1b1a      	subs	r2, r3, r4
30005778:	4945      	ldr	r1, [pc, #276]	; (30005890 <BOOT_TCMSet+0x2e0>)
3000577a:	eb05 0282 	add.w	r2, r5, r2, lsl #2
3000577e:	6950      	ldr	r0, [r2, #20]
30005780:	6608      	str	r0, [r1, #96]	; 0x60
30005782:	6990      	ldr	r0, [r2, #24]
30005784:	6648      	str	r0, [r1, #100]	; 0x64
30005786:	6852      	ldr	r2, [r2, #4]
30005788:	2a01      	cmp	r2, #1
3000578a:	d04b      	beq.n	30005824 <BOOT_TCMSet+0x274>
3000578c:	1b1b      	subs	r3, r3, r4
3000578e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30005792:	68ab      	ldr	r3, [r5, #8]
30005794:	2b01      	cmp	r3, #1
30005796:	f47f af50 	bne.w	3000563a <BOOT_TCMSet+0x8a>
3000579a:	483b      	ldr	r0, [pc, #236]	; (30005888 <BOOT_TCMSet+0x2d8>)
3000579c:	2300      	movs	r3, #0
3000579e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300057a2:	f3bf 8f4f 	dsb	sy
300057a6:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300057aa:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300057ae:	f3c5 344e 	ubfx	r4, r5, #13, #15
300057b2:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300057b6:	0164      	lsls	r4, r4, #5
300057b8:	ea04 0106 	and.w	r1, r4, r6
300057bc:	462b      	mov	r3, r5
300057be:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300057c2:	3b01      	subs	r3, #1
300057c4:	1c5f      	adds	r7, r3, #1
300057c6:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300057ca:	d1f8      	bne.n	300057be <BOOT_TCMSet+0x20e>
300057cc:	3c20      	subs	r4, #32
300057ce:	f114 0f20 	cmn.w	r4, #32
300057d2:	d1f1      	bne.n	300057b8 <BOOT_TCMSet+0x208>
300057d4:	f3bf 8f4f 	dsb	sy
300057d8:	6943      	ldr	r3, [r0, #20]
300057da:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300057de:	6143      	str	r3, [r0, #20]
300057e0:	f3bf 8f4f 	dsb	sy
300057e4:	f3bf 8f6f 	isb	sy
300057e8:	4828      	ldr	r0, [pc, #160]	; (3000588c <BOOT_TCMSet+0x2dc>)
300057ea:	2300      	movs	r3, #0
300057ec:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300057f0:	f3bf 8f4f 	dsb	sy
300057f4:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300057f8:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300057fc:	f3c5 344e 	ubfx	r4, r5, #13, #15
30005800:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30005804:	0164      	lsls	r4, r4, #5
30005806:	ea04 0106 	and.w	r1, r4, r6
3000580a:	462b      	mov	r3, r5
3000580c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30005810:	3b01      	subs	r3, #1
30005812:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30005816:	1c5a      	adds	r2, r3, #1
30005818:	d1f8      	bne.n	3000580c <BOOT_TCMSet+0x25c>
3000581a:	3c20      	subs	r4, #32
3000581c:	f114 0f20 	cmn.w	r4, #32
30005820:	d1f1      	bne.n	30005806 <BOOT_TCMSet+0x256>
30005822:	e700      	b.n	30005626 <BOOT_TCMSet+0x76>
30005824:	f3bf 8f4f 	dsb	sy
30005828:	f3bf 8f6f 	isb	sy
3000582c:	4a16      	ldr	r2, [pc, #88]	; (30005888 <BOOT_TCMSet+0x2d8>)
3000582e:	2000      	movs	r0, #0
30005830:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30005834:	f3bf 8f4f 	dsb	sy
30005838:	f3bf 8f6f 	isb	sy
3000583c:	6951      	ldr	r1, [r2, #20]
3000583e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30005842:	6151      	str	r1, [r2, #20]
30005844:	f3bf 8f4f 	dsb	sy
30005848:	f3bf 8f6f 	isb	sy
3000584c:	f3bf 8f4f 	dsb	sy
30005850:	f3bf 8f6f 	isb	sy
30005854:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30005858:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
3000585c:	f3bf 8f4f 	dsb	sy
30005860:	f3bf 8f6f 	isb	sy
30005864:	6951      	ldr	r1, [r2, #20]
30005866:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
3000586a:	6151      	str	r1, [r2, #20]
3000586c:	f3bf 8f4f 	dsb	sy
30005870:	f3bf 8f6f 	isb	sy
30005874:	1b1b      	subs	r3, r3, r4
30005876:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000587a:	68ab      	ldr	r3, [r5, #8]
3000587c:	2b01      	cmp	r3, #1
3000587e:	f47f aedc 	bne.w	3000563a <BOOT_TCMSet+0x8a>
30005882:	e78a      	b.n	3000579a <BOOT_TCMSet+0x1ea>
30005884:	3000e340 	.word	0x3000e340
30005888:	e002ed00 	.word	0xe002ed00
3000588c:	e000ed00 	.word	0xe000ed00
30005890:	e0042000 	.word	0xe0042000
30005894:	ffff0000 	.word	0xffff0000

30005898 <BOOT_GRstConfig>:
30005898:	4b13      	ldr	r3, [pc, #76]	; (300058e8 <BOOT_GRstConfig+0x50>)
3000589a:	b570      	push	{r4, r5, r6, lr}
3000589c:	681a      	ldr	r2, [r3, #0]
3000589e:	1c50      	adds	r0, r2, #1
300058a0:	d01e      	beq.n	300058e0 <BOOT_GRstConfig+0x48>
300058a2:	2000      	movs	r0, #0
300058a4:	6859      	ldr	r1, [r3, #4]
300058a6:	2901      	cmp	r1, #1
300058a8:	bf08      	it	eq
300058aa:	4310      	orreq	r0, r2
300058ac:	f853 2f08 	ldr.w	r2, [r3, #8]!
300058b0:	1c51      	adds	r1, r2, #1
300058b2:	d1f7      	bne.n	300058a4 <BOOT_GRstConfig+0xc>
300058b4:	b284      	uxth	r4, r0
300058b6:	43e3      	mvns	r3, r4
300058b8:	b29e      	uxth	r6, r3
300058ba:	4d0c      	ldr	r5, [pc, #48]	; (300058ec <BOOT_GRstConfig+0x54>)
300058bc:	2028      	movs	r0, #40	; 0x28
300058be:	4a0c      	ldr	r2, [pc, #48]	; (300058f0 <BOOT_GRstConfig+0x58>)
300058c0:	882b      	ldrh	r3, [r5, #0]
300058c2:	b29b      	uxth	r3, r3
300058c4:	4323      	orrs	r3, r4
300058c6:	802b      	strh	r3, [r5, #0]
300058c8:	4790      	blx	r2
300058ca:	4a0a      	ldr	r2, [pc, #40]	; (300058f4 <BOOT_GRstConfig+0x5c>)
300058cc:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300058d0:	4033      	ands	r3, r6
300058d2:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300058d6:	8868      	ldrh	r0, [r5, #2]
300058d8:	b280      	uxth	r0, r0
300058da:	4320      	orrs	r0, r4
300058dc:	8068      	strh	r0, [r5, #2]
300058de:	bd70      	pop	{r4, r5, r6, pc}
300058e0:	f64f 76ff 	movw	r6, #65535	; 0xffff
300058e4:	2400      	movs	r4, #0
300058e6:	e7e8      	b.n	300058ba <BOOT_GRstConfig+0x22>
300058e8:	3000d8d4 	.word	0x3000d8d4
300058ec:	42008200 	.word	0x42008200
300058f0:	00009b2d 	.word	0x00009b2d
300058f4:	42008000 	.word	0x42008000

300058f8 <BOOT_DDR_LCDC_HPR>:
300058f8:	4a04      	ldr	r2, [pc, #16]	; (3000590c <BOOT_DDR_LCDC_HPR+0x14>)
300058fa:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300058fe:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005902:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005906:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
3000590a:	4770      	bx	lr
3000590c:	42008000 	.word	0x42008000

30005910 <BOOT_DDR_Init>:
30005910:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005914:	b530      	push	{r4, r5, lr}
30005916:	4d15      	ldr	r5, [pc, #84]	; (3000596c <BOOT_DDR_Init+0x5c>)
30005918:	b099      	sub	sp, #100	; 0x64
3000591a:	6b1c      	ldr	r4, [r3, #48]	; 0x30
3000591c:	47a8      	blx	r5
3000591e:	2803      	cmp	r0, #3
30005920:	d11e      	bne.n	30005960 <BOOT_DDR_Init+0x50>
30005922:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
30005926:	4a12      	ldr	r2, [pc, #72]	; (30005970 <BOOT_DDR_Init+0x60>)
30005928:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000592c:	4911      	ldr	r1, [pc, #68]	; (30005974 <BOOT_DDR_Init+0x64>)
3000592e:	a801      	add	r0, sp, #4
30005930:	631c      	str	r4, [r3, #48]	; 0x30
30005932:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
30005936:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
3000593a:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
3000593e:	4788      	blx	r1
30005940:	a801      	add	r0, sp, #4
30005942:	4b0d      	ldr	r3, [pc, #52]	; (30005978 <BOOT_DDR_Init+0x68>)
30005944:	4798      	blx	r3
30005946:	4b0d      	ldr	r3, [pc, #52]	; (3000597c <BOOT_DDR_Init+0x6c>)
30005948:	4798      	blx	r3
3000594a:	4b0d      	ldr	r3, [pc, #52]	; (30005980 <BOOT_DDR_Init+0x70>)
3000594c:	4798      	blx	r3
3000594e:	4b0d      	ldr	r3, [pc, #52]	; (30005984 <BOOT_DDR_Init+0x74>)
30005950:	4798      	blx	r3
30005952:	2101      	movs	r1, #1
30005954:	f240 10ff 	movw	r0, #511	; 0x1ff
30005958:	4b0b      	ldr	r3, [pc, #44]	; (30005988 <BOOT_DDR_Init+0x78>)
3000595a:	4798      	blx	r3
3000595c:	b019      	add	sp, #100	; 0x64
3000595e:	bd30      	pop	{r4, r5, pc}
30005960:	47a8      	blx	r5
30005962:	2802      	cmp	r0, #2
30005964:	d0dd      	beq.n	30005922 <BOOT_DDR_Init+0x12>
30005966:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
3000596a:	e7dc      	b.n	30005926 <BOOT_DDR_Init+0x16>
3000596c:	0000c0f9 	.word	0x0000c0f9
30005970:	42008000 	.word	0x42008000
30005974:	300080c1 	.word	0x300080c1
30005978:	30008a99 	.word	0x30008a99
3000597c:	3000b98d 	.word	0x3000b98d
30005980:	30008b69 	.word	0x30008b69
30005984:	30008b6d 	.word	0x30008b6d
30005988:	3000b935 	.word	0x3000b935

3000598c <BOOT_PSRAM_Init>:
3000598c:	4a1f      	ldr	r2, [pc, #124]	; (30005a0c <BOOT_PSRAM_Init+0x80>)
3000598e:	4920      	ldr	r1, [pc, #128]	; (30005a10 <BOOT_PSRAM_Init+0x84>)
30005990:	b500      	push	{lr}
30005992:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30005996:	b089      	sub	sp, #36	; 0x24
30005998:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000599c:	a801      	add	r0, sp, #4
3000599e:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
300059a2:	4788      	blx	r1
300059a4:	a801      	add	r0, sp, #4
300059a6:	4b1b      	ldr	r3, [pc, #108]	; (30005a14 <BOOT_PSRAM_Init+0x88>)
300059a8:	4798      	blx	r3
300059aa:	4b1b      	ldr	r3, [pc, #108]	; (30005a18 <BOOT_PSRAM_Init+0x8c>)
300059ac:	4798      	blx	r3
300059ae:	4b1b      	ldr	r3, [pc, #108]	; (30005a1c <BOOT_PSRAM_Init+0x90>)
300059b0:	4798      	blx	r3
300059b2:	2801      	cmp	r0, #1
300059b4:	d026      	beq.n	30005a04 <BOOT_PSRAM_Init+0x78>
300059b6:	4b1a      	ldr	r3, [pc, #104]	; (30005a20 <BOOT_PSRAM_Init+0x94>)
300059b8:	689b      	ldr	r3, [r3, #8]
300059ba:	079a      	lsls	r2, r3, #30
300059bc:	d41e      	bmi.n	300059fc <BOOT_PSRAM_Init+0x70>
300059be:	4b19      	ldr	r3, [pc, #100]	; (30005a24 <BOOT_PSRAM_Init+0x98>)
300059c0:	4798      	blx	r3
300059c2:	4b12      	ldr	r3, [pc, #72]	; (30005a0c <BOOT_PSRAM_Init+0x80>)
300059c4:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
300059c8:	009b      	lsls	r3, r3, #2
300059ca:	d510      	bpl.n	300059ee <BOOT_PSRAM_Init+0x62>
300059cc:	4a16      	ldr	r2, [pc, #88]	; (30005a28 <BOOT_PSRAM_Init+0x9c>)
300059ce:	4b17      	ldr	r3, [pc, #92]	; (30005a2c <BOOT_PSRAM_Init+0xa0>)
300059d0:	6812      	ldr	r2, [r2, #0]
300059d2:	605a      	str	r2, [r3, #4]
300059d4:	4a16      	ldr	r2, [pc, #88]	; (30005a30 <BOOT_PSRAM_Init+0xa4>)
300059d6:	f04f 0c20 	mov.w	ip, #32
300059da:	2101      	movs	r1, #1
300059dc:	4b15      	ldr	r3, [pc, #84]	; (30005a34 <BOOT_PSRAM_Init+0xa8>)
300059de:	6952      	ldr	r2, [r2, #20]
300059e0:	4608      	mov	r0, r1
300059e2:	fbbc f2f2 	udiv	r2, ip, r2
300059e6:	4798      	blx	r3
300059e8:	b009      	add	sp, #36	; 0x24
300059ea:	f85d fb04 	ldr.w	pc, [sp], #4
300059ee:	4b12      	ldr	r3, [pc, #72]	; (30005a38 <BOOT_PSRAM_Init+0xac>)
300059f0:	4798      	blx	r3
300059f2:	4a0e      	ldr	r2, [pc, #56]	; (30005a2c <BOOT_PSRAM_Init+0xa0>)
300059f4:	4b0c      	ldr	r3, [pc, #48]	; (30005a28 <BOOT_PSRAM_Init+0x9c>)
300059f6:	6852      	ldr	r2, [r2, #4]
300059f8:	601a      	str	r2, [r3, #0]
300059fa:	e7eb      	b.n	300059d4 <BOOT_PSRAM_Init+0x48>
300059fc:	480f      	ldr	r0, [pc, #60]	; (30005a3c <BOOT_PSRAM_Init+0xb0>)
300059fe:	f006 fc13 	bl	3000c228 <__DiagPrintf_veneer>
30005a02:	e7dc      	b.n	300059be <BOOT_PSRAM_Init+0x32>
30005a04:	4b0e      	ldr	r3, [pc, #56]	; (30005a40 <BOOT_PSRAM_Init+0xb4>)
30005a06:	4798      	blx	r3
30005a08:	e7db      	b.n	300059c2 <BOOT_PSRAM_Init+0x36>
30005a0a:	bf00      	nop
30005a0c:	42008000 	.word	0x42008000
30005a10:	300096bd 	.word	0x300096bd
30005a14:	300096dd 	.word	0x300096dd
30005a18:	30009745 	.word	0x30009745
30005a1c:	30007ffd 	.word	0x30007ffd
30005a20:	2001c00c 	.word	0x2001c00c
30005a24:	300099e9 	.word	0x300099e9
30005a28:	3000ef10 	.word	0x3000ef10
30005a2c:	41002000 	.word	0x41002000
30005a30:	3000eb20 	.word	0x3000eb20
30005a34:	30009cad 	.word	0x30009cad
30005a38:	30009a25 	.word	0x30009a25
30005a3c:	3000c964 	.word	0x3000c964
30005a40:	30009911 	.word	0x30009911

30005a44 <BOOT_WakeFromPG>:
30005a44:	b538      	push	{r3, r4, r5, lr}
30005a46:	4c4b      	ldr	r4, [pc, #300]	; (30005b74 <BOOT_WakeFromPG+0x130>)
30005a48:	f7ff f980 	bl	30004d4c <BOOT_RAM_TZCfg>
30005a4c:	69a3      	ldr	r3, [r4, #24]
30005a4e:	b10b      	cbz	r3, 30005a54 <BOOT_WakeFromPG+0x10>
30005a50:	4a49      	ldr	r2, [pc, #292]	; (30005b78 <BOOT_WakeFromPG+0x134>)
30005a52:	6093      	str	r3, [r2, #8]
30005a54:	4b49      	ldr	r3, [pc, #292]	; (30005b7c <BOOT_WakeFromPG+0x138>)
30005a56:	f06f 0c1a 	mvn.w	ip, #26
30005a5a:	4849      	ldr	r0, [pc, #292]	; (30005b80 <BOOT_WakeFromPG+0x13c>)
30005a5c:	f103 0e50 	add.w	lr, r3, #80	; 0x50
30005a60:	ebac 0c04 	sub.w	ip, ip, r4
30005a64:	eb0c 0203 	add.w	r2, ip, r3
30005a68:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005a6c:	4402      	add	r2, r0
30005a6e:	4573      	cmp	r3, lr
30005a70:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005a74:	d1f6      	bne.n	30005a64 <BOOT_WakeFromPG+0x20>
30005a76:	6822      	ldr	r2, [r4, #0]
30005a78:	4b3f      	ldr	r3, [pc, #252]	; (30005b78 <BOOT_WakeFromPG+0x134>)
30005a7a:	f422 7280 	bic.w	r2, r2, #256	; 0x100
30005a7e:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
30005a82:	6002      	str	r2, [r0, #0]
30005a84:	6045      	str	r5, [r0, #4]
30005a86:	6081      	str	r1, [r0, #8]
30005a88:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005a8a:	4a3e      	ldr	r2, [pc, #248]	; (30005b84 <BOOT_WakeFromPG+0x140>)
30005a8c:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005a90:	4c3d      	ldr	r4, [pc, #244]	; (30005b88 <BOOT_WakeFromPG+0x144>)
30005a92:	6259      	str	r1, [r3, #36]	; 0x24
30005a94:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005a96:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005a9a:	6251      	str	r1, [r2, #36]	; 0x24
30005a9c:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005aa0:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005aa4:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005aa8:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005aac:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005ab0:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005ab4:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005ab8:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005abc:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005ac0:	f7ff fd48 	bl	30005554 <BOOT_CACHEWRR_Set>
30005ac4:	4b31      	ldr	r3, [pc, #196]	; (30005b8c <BOOT_WakeFromPG+0x148>)
30005ac6:	6818      	ldr	r0, [r3, #0]
30005ac8:	f7ff fd72 	bl	300055b0 <BOOT_TCMSet>
30005acc:	2201      	movs	r2, #1
30005ace:	4930      	ldr	r1, [pc, #192]	; (30005b90 <BOOT_WakeFromPG+0x14c>)
30005ad0:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30005ad4:	47a0      	blx	r4
30005ad6:	4b2f      	ldr	r3, [pc, #188]	; (30005b94 <BOOT_WakeFromPG+0x150>)
30005ad8:	4798      	blx	r3
30005ada:	4b2f      	ldr	r3, [pc, #188]	; (30005b98 <BOOT_WakeFromPG+0x154>)
30005adc:	4798      	blx	r3
30005ade:	2801      	cmp	r0, #1
30005ae0:	d130      	bne.n	30005b44 <BOOT_WakeFromPG+0x100>
30005ae2:	4b2e      	ldr	r3, [pc, #184]	; (30005b9c <BOOT_WakeFromPG+0x158>)
30005ae4:	689b      	ldr	r3, [r3, #8]
30005ae6:	079b      	lsls	r3, r3, #30
30005ae8:	d43f      	bmi.n	30005b6a <BOOT_WakeFromPG+0x126>
30005aea:	2201      	movs	r2, #1
30005aec:	492c      	ldr	r1, [pc, #176]	; (30005ba0 <BOOT_WakeFromPG+0x15c>)
30005aee:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005af2:	47a0      	blx	r4
30005af4:	2201      	movs	r2, #1
30005af6:	492b      	ldr	r1, [pc, #172]	; (30005ba4 <BOOT_WakeFromPG+0x160>)
30005af8:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005afc:	47a0      	blx	r4
30005afe:	f7ff ff07 	bl	30005910 <BOOT_DDR_Init>
30005b02:	4a29      	ldr	r2, [pc, #164]	; (30005ba8 <BOOT_WakeFromPG+0x164>)
30005b04:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30005b08:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005b0c:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005b10:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30005b14:	4925      	ldr	r1, [pc, #148]	; (30005bac <BOOT_WakeFromPG+0x168>)
30005b16:	4a26      	ldr	r2, [pc, #152]	; (30005bb0 <BOOT_WakeFromPG+0x16c>)
30005b18:	f891 3067 	ldrb.w	r3, [r1, #103]	; 0x67
30005b1c:	4c19      	ldr	r4, [pc, #100]	; (30005b84 <BOOT_WakeFromPG+0x140>)
30005b1e:	f043 0301 	orr.w	r3, r3, #1
30005b22:	4824      	ldr	r0, [pc, #144]	; (30005bb4 <BOOT_WakeFromPG+0x170>)
30005b24:	f881 3067 	strb.w	r3, [r1, #103]	; 0x67
30005b28:	6893      	ldr	r3, [r2, #8]
30005b2a:	6852      	ldr	r2, [r2, #4]
30005b2c:	605a      	str	r2, [r3, #4]
30005b2e:	60a3      	str	r3, [r4, #8]
30005b30:	f380 8888 	msr	MSP_NS, r0
30005b34:	4a20      	ldr	r2, [pc, #128]	; (30005bb8 <BOOT_WakeFromPG+0x174>)
30005b36:	f382 8809 	msr	PSP, r2
30005b3a:	6858      	ldr	r0, [r3, #4]
30005b3c:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
30005b40:	f7ff bc4c 	b.w	300053dc <BOOT_NsStart>
30005b44:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005b48:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005b4c:	2201      	movs	r2, #1
30005b4e:	47a0      	blx	r4
30005b50:	4a15      	ldr	r2, [pc, #84]	; (30005ba8 <BOOT_WakeFromPG+0x164>)
30005b52:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005b56:	f043 0302 	orr.w	r3, r3, #2
30005b5a:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005b5e:	f7ff ff15 	bl	3000598c <BOOT_PSRAM_Init>
30005b62:	2096      	movs	r0, #150	; 0x96
30005b64:	4b15      	ldr	r3, [pc, #84]	; (30005bbc <BOOT_WakeFromPG+0x178>)
30005b66:	4798      	blx	r3
30005b68:	e7d4      	b.n	30005b14 <BOOT_WakeFromPG+0xd0>
30005b6a:	4815      	ldr	r0, [pc, #84]	; (30005bc0 <BOOT_WakeFromPG+0x17c>)
30005b6c:	f006 fb5c 	bl	3000c228 <__DiagPrintf_veneer>
30005b70:	e7bb      	b.n	30005aea <BOOT_WakeFromPG+0xa6>
30005b72:	bf00      	nop
30005b74:	3000eea4 	.word	0x3000eea4
30005b78:	e000ed00 	.word	0xe000ed00
30005b7c:	3000eebf 	.word	0x3000eebf
30005b80:	e000e100 	.word	0xe000e100
30005b84:	e002ed00 	.word	0xe002ed00
30005b88:	0000b479 	.word	0x0000b479
30005b8c:	3000d80c 	.word	0x3000d80c
30005b90:	40001000 	.word	0x40001000
30005b94:	0000d835 	.word	0x0000d835
30005b98:	30007f25 	.word	0x30007f25
30005b9c:	2001c00c 	.word	0x2001c00c
30005ba0:	40080000 	.word	0x40080000
30005ba4:	40040000 	.word	0x40040000
30005ba8:	42008000 	.word	0x42008000
30005bac:	42008200 	.word	0x42008200
30005bb0:	60000020 	.word	0x60000020
30005bb4:	2001bffc 	.word	0x2001bffc
30005bb8:	2001b7fc 	.word	0x2001b7fc
30005bbc:	00009b2d 	.word	0x00009b2d
30005bc0:	3000c988 	.word	0x3000c988

30005bc4 <BOOT_SCBConfig_HP>:
30005bc4:	4b0e      	ldr	r3, [pc, #56]	; (30005c00 <BOOT_SCBConfig_HP+0x3c>)
30005bc6:	4a0f      	ldr	r2, [pc, #60]	; (30005c04 <BOOT_SCBConfig_HP+0x40>)
30005bc8:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005bca:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005bce:	6259      	str	r1, [r3, #36]	; 0x24
30005bd0:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005bd2:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005bd6:	6251      	str	r1, [r2, #36]	; 0x24
30005bd8:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005bdc:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005be0:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005be4:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005be8:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005bec:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005bf0:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005bf4:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005bf8:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005bfc:	4770      	bx	lr
30005bfe:	bf00      	nop
30005c00:	e000ed00 	.word	0xe000ed00
30005c04:	e002ed00 	.word	0xe002ed00

30005c08 <BOOT_SCBVTORBackup_HP>:
30005c08:	4a02      	ldr	r2, [pc, #8]	; (30005c14 <BOOT_SCBVTORBackup_HP+0xc>)
30005c0a:	4b03      	ldr	r3, [pc, #12]	; (30005c18 <BOOT_SCBVTORBackup_HP+0x10>)
30005c0c:	6892      	ldr	r2, [r2, #8]
30005c0e:	619a      	str	r2, [r3, #24]
30005c10:	4770      	bx	lr
30005c12:	bf00      	nop
30005c14:	e000ed00 	.word	0xe000ed00
30005c18:	3000eea4 	.word	0x3000eea4

30005c1c <BOOT_SCBVTORReFill_HP>:
30005c1c:	4a02      	ldr	r2, [pc, #8]	; (30005c28 <BOOT_SCBVTORReFill_HP+0xc>)
30005c1e:	4b03      	ldr	r3, [pc, #12]	; (30005c2c <BOOT_SCBVTORReFill_HP+0x10>)
30005c20:	6992      	ldr	r2, [r2, #24]
30005c22:	609a      	str	r2, [r3, #8]
30005c24:	4770      	bx	lr
30005c26:	bf00      	nop
30005c28:	3000eea4 	.word	0x3000eea4
30005c2c:	e000ed00 	.word	0xe000ed00

30005c30 <BOOT_NVICBackup_HP>:
30005c30:	491a      	ldr	r1, [pc, #104]	; (30005c9c <BOOT_NVICBackup_HP+0x6c>)
30005c32:	f06f 001a 	mvn.w	r0, #26
30005c36:	b410      	push	{r4}
30005c38:	4c19      	ldr	r4, [pc, #100]	; (30005ca0 <BOOT_NVICBackup_HP+0x70>)
30005c3a:	680a      	ldr	r2, [r1, #0]
30005c3c:	f104 031b 	add.w	r3, r4, #27
30005c40:	1b00      	subs	r0, r0, r4
30005c42:	6022      	str	r2, [r4, #0]
30005c44:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
30005c48:	684a      	ldr	r2, [r1, #4]
30005c4a:	6062      	str	r2, [r4, #4]
30005c4c:	688a      	ldr	r2, [r1, #8]
30005c4e:	60a2      	str	r2, [r4, #8]
30005c50:	18c2      	adds	r2, r0, r3
30005c52:	440a      	add	r2, r1
30005c54:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
30005c58:	f803 2f01 	strb.w	r2, [r3, #1]!
30005c5c:	4563      	cmp	r3, ip
30005c5e:	d1f7      	bne.n	30005c50 <BOOT_NVICBackup_HP+0x20>
30005c60:	4b10      	ldr	r3, [pc, #64]	; (30005ca4 <BOOT_NVICBackup_HP+0x74>)
30005c62:	695b      	ldr	r3, [r3, #20]
30005c64:	03db      	lsls	r3, r3, #15
30005c66:	d512      	bpl.n	30005c8e <BOOT_NVICBackup_HP+0x5e>
30005c68:	f014 011f 	ands.w	r1, r4, #31
30005c6c:	4b0c      	ldr	r3, [pc, #48]	; (30005ca0 <BOOT_NVICBackup_HP+0x70>)
30005c6e:	d111      	bne.n	30005c94 <BOOT_NVICBackup_HP+0x64>
30005c70:	216c      	movs	r1, #108	; 0x6c
30005c72:	f3bf 8f4f 	dsb	sy
30005c76:	480b      	ldr	r0, [pc, #44]	; (30005ca4 <BOOT_NVICBackup_HP+0x74>)
30005c78:	4419      	add	r1, r3
30005c7a:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005c7e:	3320      	adds	r3, #32
30005c80:	1aca      	subs	r2, r1, r3
30005c82:	2a00      	cmp	r2, #0
30005c84:	dcf9      	bgt.n	30005c7a <BOOT_NVICBackup_HP+0x4a>
30005c86:	f3bf 8f4f 	dsb	sy
30005c8a:	f3bf 8f6f 	isb	sy
30005c8e:	f85d 4b04 	ldr.w	r4, [sp], #4
30005c92:	4770      	bx	lr
30005c94:	f024 031f 	bic.w	r3, r4, #31
30005c98:	316c      	adds	r1, #108	; 0x6c
30005c9a:	e7ea      	b.n	30005c72 <BOOT_NVICBackup_HP+0x42>
30005c9c:	e000e100 	.word	0xe000e100
30005ca0:	3000eea4 	.word	0x3000eea4
30005ca4:	e000ed00 	.word	0xe000ed00

30005ca8 <BOOT_NVICReFill_HP>:
30005ca8:	b510      	push	{r4, lr}
30005caa:	f06f 0c1a 	mvn.w	ip, #26
30005cae:	4c0d      	ldr	r4, [pc, #52]	; (30005ce4 <BOOT_NVICReFill_HP+0x3c>)
30005cb0:	480d      	ldr	r0, [pc, #52]	; (30005ce8 <BOOT_NVICReFill_HP+0x40>)
30005cb2:	f104 031b 	add.w	r3, r4, #27
30005cb6:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30005cba:	ebac 0c04 	sub.w	ip, ip, r4
30005cbe:	eb0c 0203 	add.w	r2, ip, r3
30005cc2:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005cc6:	4402      	add	r2, r0
30005cc8:	4573      	cmp	r3, lr
30005cca:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005cce:	d1f6      	bne.n	30005cbe <BOOT_NVICReFill_HP+0x16>
30005cd0:	e9d4 3100 	ldrd	r3, r1, [r4]
30005cd4:	68a2      	ldr	r2, [r4, #8]
30005cd6:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005cda:	6003      	str	r3, [r0, #0]
30005cdc:	6041      	str	r1, [r0, #4]
30005cde:	6082      	str	r2, [r0, #8]
30005ce0:	bd10      	pop	{r4, pc}
30005ce2:	bf00      	nop
30005ce4:	3000eea4 	.word	0x3000eea4
30005ce8:	e000e100 	.word	0xe000e100

30005cec <BOOT_ImgCopy>:
30005cec:	b510      	push	{r4, lr}
30005cee:	b082      	sub	sp, #8
30005cf0:	4b09      	ldr	r3, [pc, #36]	; (30005d18 <BOOT_ImgCopy+0x2c>)
30005cf2:	4604      	mov	r4, r0
30005cf4:	e9cd 2100 	strd	r2, r1, [sp]
30005cf8:	4798      	blx	r3
30005cfa:	e9dd 2100 	ldrd	r2, r1, [sp]
30005cfe:	b128      	cbz	r0, 30005d0c <BOOT_ImgCopy+0x20>
30005d00:	4620      	mov	r0, r4
30005d02:	4b06      	ldr	r3, [pc, #24]	; (30005d1c <BOOT_ImgCopy+0x30>)
30005d04:	b002      	add	sp, #8
30005d06:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005d0a:	4718      	bx	r3
30005d0c:	4620      	mov	r0, r4
30005d0e:	b002      	add	sp, #8
30005d10:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005d14:	f003 b90c 	b.w	30008f30 <NandImgCopy>
30005d18:	3000a541 	.word	0x3000a541
30005d1c:	00012a1d 	.word	0x00012a1d

30005d20 <BOOT_LoadImages>:
30005d20:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005d24:	4b56      	ldr	r3, [pc, #344]	; (30005e80 <BOOT_LoadImages+0x160>)
30005d26:	b0b7      	sub	sp, #220	; 0xdc
30005d28:	2700      	movs	r7, #0
30005d2a:	2104      	movs	r1, #4
30005d2c:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005d30:	aa11      	add	r2, sp, #68	; 0x44
30005d32:	4b54      	ldr	r3, [pc, #336]	; (30005e84 <BOOT_LoadImages+0x164>)
30005d34:	2601      	movs	r6, #1
30005d36:	9703      	str	r7, [sp, #12]
30005d38:	9705      	str	r7, [sp, #20]
30005d3a:	4798      	blx	r3
30005d3c:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
30005d40:	409e      	lsls	r6, r3
30005d42:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
30005d46:	f000 8236 	beq.w	300061b6 <BOOT_LoadImages+0x496>
30005d4a:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
30005d4e:	f000 8373 	beq.w	30006438 <BOOT_LoadImages+0x718>
30005d52:	4c4d      	ldr	r4, [pc, #308]	; (30005e88 <BOOT_LoadImages+0x168>)
30005d54:	f04f 0800 	mov.w	r8, #0
30005d58:	4d4c      	ldr	r5, [pc, #304]	; (30005e8c <BOOT_LoadImages+0x16c>)
30005d5a:	eb06 0904 	add.w	r9, r6, r4
30005d5e:	484c      	ldr	r0, [pc, #304]	; (30005e90 <BOOT_LoadImages+0x170>)
30005d60:	4435      	add	r5, r6
30005d62:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
30005d66:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
30005d6a:	f7ff f9cd 	bl	30005108 <mcc_init_crc32>
30005d6e:	f8d9 3004 	ldr.w	r3, [r9, #4]
30005d72:	f8d5 a008 	ldr.w	sl, [r5, #8]
30005d76:	f899 200c 	ldrb.w	r2, [r9, #12]
30005d7a:	9308      	str	r3, [sp, #32]
30005d7c:	3b01      	subs	r3, #1
30005d7e:	9206      	str	r2, [sp, #24]
30005d80:	3303      	adds	r3, #3
30005d82:	686a      	ldr	r2, [r5, #4]
30005d84:	f895 b00c 	ldrb.w	fp, [r5, #12]
30005d88:	9204      	str	r2, [sp, #16]
30005d8a:	f200 81b8 	bhi.w	300060fe <BOOT_LoadImages+0x3de>
30005d8e:	f8d9 2008 	ldr.w	r2, [r9, #8]
30005d92:	9b04      	ldr	r3, [sp, #16]
30005d94:	3a01      	subs	r2, #1
30005d96:	3b01      	subs	r3, #1
30005d98:	3203      	adds	r2, #3
30005d9a:	d868      	bhi.n	30005e6e <BOOT_LoadImages+0x14e>
30005d9c:	9a06      	ldr	r2, [sp, #24]
30005d9e:	9307      	str	r3, [sp, #28]
30005da0:	2a01      	cmp	r2, #1
30005da2:	f240 8303 	bls.w	300063ac <BOOT_LoadImages+0x68c>
30005da6:	483b      	ldr	r0, [pc, #236]	; (30005e94 <BOOT_LoadImages+0x174>)
30005da8:	f006 fa3e 	bl	3000c228 <__DiagPrintf_veneer>
30005dac:	9b07      	ldr	r3, [sp, #28]
30005dae:	3303      	adds	r3, #3
30005db0:	d864      	bhi.n	30005e7c <BOOT_LoadImages+0x15c>
30005db2:	f10a 3aff 	add.w	sl, sl, #4294967295
30005db6:	f11a 0f03 	cmn.w	sl, #3
30005dba:	f200 81a6 	bhi.w	3000610a <BOOT_LoadImages+0x3ea>
30005dbe:	f1bb 0f01 	cmp.w	fp, #1
30005dc2:	f200 8345 	bhi.w	30006450 <BOOT_LoadImages+0x730>
30005dc6:	9904      	ldr	r1, [sp, #16]
30005dc8:	4833      	ldr	r0, [pc, #204]	; (30005e98 <BOOT_LoadImages+0x178>)
30005dca:	f006 fa2d 	bl	3000c228 <__DiagPrintf_veneer>
30005dce:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005dd2:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005dd6:	4c2e      	ldr	r4, [pc, #184]	; (30005e90 <BOOT_LoadImages+0x170>)
30005dd8:	9307      	str	r3, [sp, #28]
30005dda:	2300      	movs	r3, #0
30005ddc:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005de0:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005de4:	9309      	str	r3, [sp, #36]	; 0x24
30005de6:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30005eb0 <BOOT_LoadImages+0x190>
30005dea:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30005dee:	6829      	ldr	r1, [r5, #0]
30005df0:	f04f 30ff 	mov.w	r0, #4294967295
30005df4:	44b4      	add	ip, r6
30005df6:	f81c 2b01 	ldrb.w	r2, [ip], #1
30005dfa:	4042      	eors	r2, r0
30005dfc:	459c      	cmp	ip, r3
30005dfe:	b2d2      	uxtb	r2, r2
30005e00:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005e04:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30005e08:	d1f5      	bne.n	30005df6 <BOOT_LoadImages+0xd6>
30005e0a:	43c2      	mvns	r2, r0
30005e0c:	930f      	str	r3, [sp, #60]	; 0x3c
30005e0e:	4291      	cmp	r1, r2
30005e10:	f000 82ff 	beq.w	30006412 <BOOT_LoadImages+0x6f2>
30005e14:	4821      	ldr	r0, [pc, #132]	; (30005e9c <BOOT_LoadImages+0x17c>)
30005e16:	f006 fa07 	bl	3000c228 <__DiagPrintf_veneer>
30005e1a:	2200      	movs	r2, #0
30005e1c:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005e1e:	9909      	ldr	r1, [sp, #36]	; 0x24
30005e20:	2900      	cmp	r1, #0
30005e22:	d04e      	beq.n	30005ec2 <BOOT_LoadImages+0x1a2>
30005e24:	f899 100c 	ldrb.w	r1, [r9, #12]
30005e28:	4553      	cmp	r3, sl
30005e2a:	9106      	str	r1, [sp, #24]
30005e2c:	d342      	bcc.n	30005eb4 <BOOT_LoadImages+0x194>
30005e2e:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30005e32:	45e2      	cmp	sl, ip
30005e34:	d93e      	bls.n	30005eb4 <BOOT_LoadImages+0x194>
30005e36:	9807      	ldr	r0, [sp, #28]
30005e38:	4560      	cmp	r0, ip
30005e3a:	f240 81fd 	bls.w	30006238 <BOOT_LoadImages+0x518>
30005e3e:	4283      	cmp	r3, r0
30005e40:	f0c0 81fa 	bcc.w	30006238 <BOOT_LoadImages+0x518>
30005e44:	2a00      	cmp	r2, #0
30005e46:	f000 8207 	beq.w	30006258 <BOOT_LoadImages+0x538>
30005e4a:	459b      	cmp	fp, r3
30005e4c:	d801      	bhi.n	30005e52 <BOOT_LoadImages+0x132>
30005e4e:	45e3      	cmp	fp, ip
30005e50:	d845      	bhi.n	30005ede <BOOT_LoadImages+0x1be>
30005e52:	4813      	ldr	r0, [pc, #76]	; (30005ea0 <BOOT_LoadImages+0x180>)
30005e54:	f006 f9e8 	bl	3000c228 <__DiagPrintf_veneer>
30005e58:	9f06      	ldr	r7, [sp, #24]
30005e5a:	4812      	ldr	r0, [pc, #72]	; (30005ea4 <BOOT_LoadImages+0x184>)
30005e5c:	464d      	mov	r5, r9
30005e5e:	f006 f9e3 	bl	3000c228 <__DiagPrintf_veneer>
30005e62:	9b07      	ldr	r3, [sp, #28]
30005e64:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30005e68:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30005e6c:	e049      	b.n	30005f02 <BOOT_LoadImages+0x1e2>
30005e6e:	480e      	ldr	r0, [pc, #56]	; (30005ea8 <BOOT_LoadImages+0x188>)
30005e70:	9307      	str	r3, [sp, #28]
30005e72:	f006 f9d9 	bl	3000c228 <__DiagPrintf_veneer>
30005e76:	9b07      	ldr	r3, [sp, #28]
30005e78:	3303      	adds	r3, #3
30005e7a:	d99a      	bls.n	30005db2 <BOOT_LoadImages+0x92>
30005e7c:	480b      	ldr	r0, [pc, #44]	; (30005eac <BOOT_LoadImages+0x18c>)
30005e7e:	e145      	b.n	3000610c <BOOT_LoadImages+0x3ec>
30005e80:	2001c01c 	.word	0x2001c01c
30005e84:	00009db5 	.word	0x00009db5
30005e88:	07ffe000 	.word	0x07ffe000
30005e8c:	07fff000 	.word	0x07fff000
30005e90:	3000dd9c 	.word	0x3000dd9c
30005e94:	3000ca00 	.word	0x3000ca00
30005e98:	3000cde8 	.word	0x3000cde8
30005e9c:	3000cb04 	.word	0x3000cb04
30005ea0:	3000cdcc 	.word	0x3000cdcc
30005ea4:	3000cb90 	.word	0x3000cb90
30005ea8:	3000c9ec 	.word	0x3000c9ec
30005eac:	3000cd98 	.word	0x3000cd98
30005eb0:	07fff004 	.word	0x07fff004
30005eb4:	48c2      	ldr	r0, [pc, #776]	; (300061c0 <BOOT_LoadImages+0x4a0>)
30005eb6:	930f      	str	r3, [sp, #60]	; 0x3c
30005eb8:	9209      	str	r2, [sp, #36]	; 0x24
30005eba:	f006 f9b5 	bl	3000c228 <__DiagPrintf_veneer>
30005ebe:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005ec0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005ec2:	2a00      	cmp	r2, #0
30005ec4:	f000 812d 	beq.w	30006122 <BOOT_LoadImages+0x402>
30005ec8:	455b      	cmp	r3, fp
30005eca:	f0c0 82c3 	bcc.w	30006454 <BOOT_LoadImages+0x734>
30005ece:	f1bb 6f00 	cmp.w	fp, #134217728	; 0x8000000
30005ed2:	bf2c      	ite	cs
30005ed4:	2200      	movcs	r2, #0
30005ed6:	2201      	movcc	r2, #1
30005ed8:	2a00      	cmp	r2, #0
30005eda:	f040 82bb 	bne.w	30006454 <BOOT_LoadImages+0x734>
30005ede:	4598      	cmp	r8, r3
30005ee0:	f200 81b2 	bhi.w	30006248 <BOOT_LoadImages+0x528>
30005ee4:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30005ee8:	f0c0 81ae 	bcc.w	30006248 <BOOT_LoadImages+0x528>
30005eec:	7b2f      	ldrb	r7, [r5, #12]
30005eee:	2a00      	cmp	r2, #0
30005ef0:	f040 81d8 	bne.w	300062a4 <BOOT_LoadImages+0x584>
30005ef4:	48b3      	ldr	r0, [pc, #716]	; (300061c4 <BOOT_LoadImages+0x4a4>)
30005ef6:	f006 f997 	bl	3000c228 <__DiagPrintf_veneer>
30005efa:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30005efe:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30005f02:	4639      	mov	r1, r7
30005f04:	48b0      	ldr	r0, [pc, #704]	; (300061c8 <BOOT_LoadImages+0x4a8>)
30005f06:	f006 f98f 	bl	3000c228 <__DiagPrintf_veneer>
30005f0a:	4bb0      	ldr	r3, [pc, #704]	; (300061cc <BOOT_LoadImages+0x4ac>)
30005f0c:	4798      	blx	r3
30005f0e:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005f12:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30005f16:	f8d1 300a 	ldr.w	r3, [r1, #10]
30005f1a:	6809      	ldr	r1, [r1, #0]
30005f1c:	330c      	adds	r3, #12
30005f1e:	910b      	str	r1, [sp, #44]	; 0x2c
30005f20:	42b3      	cmp	r3, r6
30005f22:	bf28      	it	cs
30005f24:	4633      	movcs	r3, r6
30005f26:	930d      	str	r3, [sp, #52]	; 0x34
30005f28:	f8d2 300a 	ldr.w	r3, [r2, #10]
30005f2c:	6812      	ldr	r2, [r2, #0]
30005f2e:	330c      	adds	r3, #12
30005f30:	920a      	str	r2, [sp, #40]	; 0x28
30005f32:	429e      	cmp	r6, r3
30005f34:	bf28      	it	cs
30005f36:	461e      	movcs	r6, r3
30005f38:	960c      	str	r6, [sp, #48]	; 0x30
30005f3a:	2800      	cmp	r0, #0
30005f3c:	f040 818e 	bne.w	3000625c <BOOT_LoadImages+0x53c>
30005f40:	4ba3      	ldr	r3, [pc, #652]	; (300061d0 <BOOT_LoadImages+0x4b0>)
30005f42:	4606      	mov	r6, r0
30005f44:	f8df b2b4 	ldr.w	fp, [pc, #692]	; 300061fc <BOOT_LoadImages+0x4dc>
30005f48:	4680      	mov	r8, r0
30005f4a:	9304      	str	r3, [sp, #16]
30005f4c:	4681      	mov	r9, r0
30005f4e:	4ba1      	ldr	r3, [pc, #644]	; (300061d4 <BOOT_LoadImages+0x4b4>)
30005f50:	9003      	str	r0, [sp, #12]
30005f52:	9306      	str	r3, [sp, #24]
30005f54:	2f00      	cmp	r7, #0
30005f56:	f040 81c1 	bne.w	300062dc <BOOT_LoadImages+0x5bc>
30005f5a:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30005f5e:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30005f60:	f10c 0104 	add.w	r1, ip, #4
30005f64:	2b00      	cmp	r3, #0
30005f66:	f000 83d8 	beq.w	3000671a <BOOT_LoadImages+0x9fa>
30005f6a:	1d18      	adds	r0, r3, #4
30005f6c:	f04f 32ff 	mov.w	r2, #4294967295
30005f70:	4460      	add	r0, ip
30005f72:	f811 3b01 	ldrb.w	r3, [r1], #1
30005f76:	4053      	eors	r3, r2
30005f78:	4281      	cmp	r1, r0
30005f7a:	b2db      	uxtb	r3, r3
30005f7c:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005f80:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005f84:	d1f5      	bne.n	30005f72 <BOOT_LoadImages+0x252>
30005f86:	43d2      	mvns	r2, r2
30005f88:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30005f8a:	4293      	cmp	r3, r2
30005f8c:	f000 839e 	beq.w	300066cc <BOOT_LoadImages+0x9ac>
30005f90:	f8dc 100a 	ldr.w	r1, [ip, #10]
30005f94:	f088 0301 	eor.w	r3, r8, #1
30005f98:	3101      	adds	r1, #1
30005f9a:	f003 0301 	and.w	r3, r3, #1
30005f9e:	ea46 0603 	orr.w	r6, r6, r3
30005fa2:	f000 8327 	beq.w	300065f4 <BOOT_LoadImages+0x8d4>
30005fa6:	990b      	ldr	r1, [sp, #44]	; 0x2c
30005fa8:	488b      	ldr	r0, [pc, #556]	; (300061d8 <BOOT_LoadImages+0x4b8>)
30005faa:	f006 f93d 	bl	3000c228 <__DiagPrintf_veneer>
30005fae:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005fb2:	2002      	movs	r0, #2
30005fb4:	f7ff f826 	bl	30005004 <FLASH_Erase_With_Lock>
30005fb8:	2e00      	cmp	r6, #0
30005fba:	f040 824f 	bne.w	3000645c <BOOT_LoadImages+0x73c>
30005fbe:	9605      	str	r6, [sp, #20]
30005fc0:	f089 0601 	eor.w	r6, r9, #1
30005fc4:	f04f 0801 	mov.w	r8, #1
30005fc8:	f006 0601 	and.w	r6, r6, #1
30005fcc:	9b03      	ldr	r3, [sp, #12]
30005fce:	2b01      	cmp	r3, #1
30005fd0:	f000 8236 	beq.w	30006440 <BOOT_LoadImages+0x720>
30005fd4:	9b03      	ldr	r3, [sp, #12]
30005fd6:	2b02      	cmp	r3, #2
30005fd8:	f000 8406 	beq.w	300067e8 <BOOT_LoadImages+0xac8>
30005fdc:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30005fde:	4648      	mov	r0, r9
30005fe0:	f8cb 3000 	str.w	r3, [fp]
30005fe4:	9b04      	ldr	r3, [sp, #16]
30005fe6:	4798      	blx	r3
30005fe8:	1e03      	subs	r3, r0, #0
30005fea:	bf18      	it	ne
30005fec:	2301      	movne	r3, #1
30005fee:	2800      	cmp	r0, #0
30005ff0:	f000 814f 	beq.w	30006292 <BOOT_LoadImages+0x572>
30005ff4:	2e00      	cmp	r6, #0
30005ff6:	f000 814c 	beq.w	30006292 <BOOT_LoadImages+0x572>
30005ffa:	fab7 f787 	clz	r7, r7
30005ffe:	9b05      	ldr	r3, [sp, #20]
30006000:	097f      	lsrs	r7, r7, #5
30006002:	2b00      	cmp	r3, #0
30006004:	f000 83f7 	beq.w	300067f6 <BOOT_LoadImages+0xad6>
30006008:	4699      	mov	r9, r3
3000600a:	4c74      	ldr	r4, [pc, #464]	; (300061dc <BOOT_LoadImages+0x4bc>)
3000600c:	2f00      	cmp	r7, #0
3000600e:	f040 8131 	bne.w	30006274 <BOOT_LoadImages+0x554>
30006012:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006016:	f8d3 200a 	ldr.w	r2, [r3, #10]
3000601a:	3201      	adds	r2, #1
3000601c:	f000 8237 	beq.w	3000648e <BOOT_LoadImages+0x76e>
30006020:	4a6f      	ldr	r2, [pc, #444]	; (300061e0 <BOOT_LoadImages+0x4c0>)
30006022:	6892      	ldr	r2, [r2, #8]
30006024:	0791      	lsls	r1, r2, #30
30006026:	f100 82ed 	bmi.w	30006604 <BOOT_LoadImages+0x8e4>
3000602a:	2301      	movs	r3, #1
3000602c:	9303      	str	r3, [sp, #12]
3000602e:	f04f 0801 	mov.w	r8, #1
30006032:	4646      	mov	r6, r8
30006034:	f8cd 8014 	str.w	r8, [sp, #20]
30006038:	f1b9 0f00 	cmp.w	r9, #0
3000603c:	d0c6      	beq.n	30005fcc <BOOT_LoadImages+0x2ac>
3000603e:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006042:	2280      	movs	r2, #128	; 0x80
30006044:	21ff      	movs	r1, #255	; 0xff
30006046:	46c8      	mov	r8, r9
30006048:	4650      	mov	r0, sl
3000604a:	f006 f8cd 	bl	3000c1e8 <____wrap_memset_veneer>
3000604e:	2280      	movs	r2, #128	; 0x80
30006050:	4629      	mov	r1, r5
30006052:	4650      	mov	r0, sl
30006054:	f006 f8d0 	bl	3000c1f8 <____wrap_memcpy_veneer>
30006058:	2f01      	cmp	r7, #1
3000605a:	f000 830c 	beq.w	30006676 <BOOT_LoadImages+0x956>
3000605e:	2100      	movs	r1, #0
30006060:	f8cd 9014 	str.w	r9, [sp, #20]
30006064:	f04f 0901 	mov.w	r9, #1
30006068:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
3000606c:	1d2e      	adds	r6, r5, #4
3000606e:	485d      	ldr	r0, [pc, #372]	; (300061e4 <BOOT_LoadImages+0x4c4>)
30006070:	f006 f8da 	bl	3000c228 <__DiagPrintf_veneer>
30006074:	4629      	mov	r1, r5
30006076:	2002      	movs	r0, #2
30006078:	f7fe ffc4 	bl	30005004 <FLASH_Erase_With_Lock>
3000607c:	aa17      	add	r2, sp, #92	; 0x5c
3000607e:	217c      	movs	r1, #124	; 0x7c
30006080:	4630      	mov	r0, r6
30006082:	f7fe ffe7 	bl	30005054 <FLASH_TxData_With_Lock>
30006086:	4630      	mov	r0, r6
30006088:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
3000608c:	f04f 32ff 	mov.w	r2, #4294967295
30006090:	f810 3b01 	ldrb.w	r3, [r0], #1
30006094:	4053      	eors	r3, r2
30006096:	4288      	cmp	r0, r1
30006098:	b2db      	uxtb	r3, r3
3000609a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000609e:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300060a2:	d1f5      	bne.n	30006090 <BOOT_LoadImages+0x370>
300060a4:	43d2      	mvns	r2, r2
300060a6:	4629      	mov	r1, r5
300060a8:	2002      	movs	r0, #2
300060aa:	9216      	str	r2, [sp, #88]	; 0x58
300060ac:	f7fe ffaa 	bl	30005004 <FLASH_Erase_With_Lock>
300060b0:	4652      	mov	r2, sl
300060b2:	2180      	movs	r1, #128	; 0x80
300060b4:	4628      	mov	r0, r5
300060b6:	f7fe ffcd 	bl	30005054 <FLASH_TxData_With_Lock>
300060ba:	9b05      	ldr	r3, [sp, #20]
300060bc:	2b00      	cmp	r3, #0
300060be:	f000 813d 	beq.w	3000633c <BOOT_LoadImages+0x61c>
300060c2:	f1b8 0f00 	cmp.w	r8, #0
300060c6:	f040 83b1 	bne.w	3000682c <BOOT_LoadImages+0xb0c>
300060ca:	2f01      	cmp	r7, #1
300060cc:	f000 827c 	beq.w	300065c8 <BOOT_LoadImages+0x8a8>
300060d0:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300060d4:	f8d3 200a 	ldr.w	r2, [r3, #10]
300060d8:	3201      	adds	r2, #1
300060da:	f000 8287 	beq.w	300065ec <BOOT_LoadImages+0x8cc>
300060de:	4a40      	ldr	r2, [pc, #256]	; (300061e0 <BOOT_LoadImages+0x4c0>)
300060e0:	6892      	ldr	r2, [r2, #8]
300060e2:	0792      	lsls	r2, r2, #30
300060e4:	f100 82ff 	bmi.w	300066e6 <BOOT_LoadImages+0x9c6>
300060e8:	f089 0601 	eor.w	r6, r9, #1
300060ec:	f04f 0801 	mov.w	r8, #1
300060f0:	f006 0601 	and.w	r6, r6, #1
300060f4:	3310      	adds	r3, #16
300060f6:	930e      	str	r3, [sp, #56]	; 0x38
300060f8:	2302      	movs	r3, #2
300060fa:	9303      	str	r3, [sp, #12]
300060fc:	e76e      	b.n	30005fdc <BOOT_LoadImages+0x2bc>
300060fe:	483a      	ldr	r0, [pc, #232]	; (300061e8 <BOOT_LoadImages+0x4c8>)
30006100:	f006 f892 	bl	3000c228 <__DiagPrintf_veneer>
30006104:	9b04      	ldr	r3, [sp, #16]
30006106:	3b01      	subs	r3, #1
30006108:	e651      	b.n	30005dae <BOOT_LoadImages+0x8e>
3000610a:	4838      	ldr	r0, [pc, #224]	; (300061ec <BOOT_LoadImages+0x4cc>)
3000610c:	f006 f88c 	bl	3000c228 <__DiagPrintf_veneer>
30006110:	f8d9 3011 	ldr.w	r3, [r9, #17]
30006114:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006118:	9307      	str	r3, [sp, #28]
3000611a:	f8d5 b00d 	ldr.w	fp, [r5, #13]
3000611e:	f8d5 8011 	ldr.w	r8, [r5, #17]
30006122:	4833      	ldr	r0, [pc, #204]	; (300061f0 <BOOT_LoadImages+0x4d0>)
30006124:	f006 f880 	bl	3000c228 <__DiagPrintf_veneer>
30006128:	4832      	ldr	r0, [pc, #200]	; (300061f4 <BOOT_LoadImages+0x4d4>)
3000612a:	f006 f87d 	bl	3000c228 <__DiagPrintf_veneer>
3000612e:	4631      	mov	r1, r6
30006130:	aa12      	add	r2, sp, #72	; 0x48
30006132:	4650      	mov	r0, sl
30006134:	f7ff f82e 	bl	30005194 <recovery_check>
30006138:	4631      	mov	r1, r6
3000613a:	4604      	mov	r4, r0
3000613c:	aa13      	add	r2, sp, #76	; 0x4c
3000613e:	9807      	ldr	r0, [sp, #28]
30006140:	f7ff f828 	bl	30005194 <recovery_check>
30006144:	4631      	mov	r1, r6
30006146:	aa14      	add	r2, sp, #80	; 0x50
30006148:	9004      	str	r0, [sp, #16]
3000614a:	4658      	mov	r0, fp
3000614c:	f7ff f822 	bl	30005194 <recovery_check>
30006150:	4631      	mov	r1, r6
30006152:	aa15      	add	r2, sp, #84	; 0x54
30006154:	4606      	mov	r6, r0
30006156:	4640      	mov	r0, r8
30006158:	f7ff f81c 	bl	30005194 <recovery_check>
3000615c:	9b04      	ldr	r3, [sp, #16]
3000615e:	4306      	orrs	r6, r0
30006160:	431c      	orrs	r4, r3
30006162:	d04d      	beq.n	30006200 <BOOT_LoadImages+0x4e0>
30006164:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30006168:	2e00      	cmp	r6, #0
3000616a:	f000 81eb 	beq.w	30006544 <BOOT_LoadImages+0x824>
3000616e:	4293      	cmp	r3, r2
30006170:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
30006174:	f0c0 8258 	bcc.w	30006628 <BOOT_LoadImages+0x908>
30006178:	428b      	cmp	r3, r1
3000617a:	d302      	bcc.n	30006182 <BOOT_LoadImages+0x462>
3000617c:	4283      	cmp	r3, r0
3000617e:	f080 8324 	bcs.w	300067ca <BOOT_LoadImages+0xaaa>
30006182:	4293      	cmp	r3, r2
30006184:	f000 8250 	beq.w	30006628 <BOOT_LoadImages+0x908>
30006188:	4293      	cmp	r3, r2
3000618a:	bf38      	it	cc
3000618c:	4613      	movcc	r3, r2
3000618e:	428b      	cmp	r3, r1
30006190:	f240 830c 	bls.w	300067ac <BOOT_LoadImages+0xa8c>
30006194:	428b      	cmp	r3, r1
30006196:	bf38      	it	cc
30006198:	460b      	movcc	r3, r1
3000619a:	4283      	cmp	r3, r0
3000619c:	f240 831d 	bls.w	300067da <BOOT_LoadImages+0xaba>
300061a0:	4b0b      	ldr	r3, [pc, #44]	; (300061d0 <BOOT_LoadImages+0x4b0>)
300061a2:	f04f 0800 	mov.w	r8, #0
300061a6:	2601      	movs	r6, #1
300061a8:	f8df b050 	ldr.w	fp, [pc, #80]	; 300061fc <BOOT_LoadImages+0x4dc>
300061ac:	9304      	str	r3, [sp, #16]
300061ae:	46c1      	mov	r9, r8
300061b0:	4b08      	ldr	r3, [pc, #32]	; (300061d4 <BOOT_LoadImages+0x4b4>)
300061b2:	9306      	str	r3, [sp, #24]
300061b4:	e712      	b.n	30005fdc <BOOT_LoadImages+0x2bc>
300061b6:	4810      	ldr	r0, [pc, #64]	; (300061f8 <BOOT_LoadImages+0x4d8>)
300061b8:	f006 f836 	bl	3000c228 <__DiagPrintf_veneer>
300061bc:	e5c9      	b.n	30005d52 <BOOT_LoadImages+0x32>
300061be:	bf00      	nop
300061c0:	3000cb44 	.word	0x3000cb44
300061c4:	3000cba0 	.word	0x3000cba0
300061c8:	3000cbd4 	.word	0x3000cbd4
300061cc:	0000c149 	.word	0x0000c149
300061d0:	300043c1 	.word	0x300043c1
300061d4:	3000d80a 	.word	0x3000d80a
300061d8:	3000cc44 	.word	0x3000cc44
300061dc:	3000dd9c 	.word	0x3000dd9c
300061e0:	2001c00c 	.word	0x2001c00c
300061e4:	3000cd18 	.word	0x3000cd18
300061e8:	3000c9d4 	.word	0x3000c9d4
300061ec:	3000ca38 	.word	0x3000ca38
300061f0:	3000cbb0 	.word	0x3000cbb0
300061f4:	3000cbc4 	.word	0x3000cbc4
300061f8:	3000c9b0 	.word	0x3000c9b0
300061fc:	3000eea0 	.word	0x3000eea0
30006200:	2e00      	cmp	r6, #0
30006202:	f000 8286 	beq.w	30006712 <BOOT_LoadImages+0x9f2>
30006206:	4659      	mov	r1, fp
30006208:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
3000620c:	e9cd 5400 	strd	r5, r4, [sp]
30006210:	429a      	cmp	r2, r3
30006212:	464b      	mov	r3, r9
30006214:	4642      	mov	r2, r8
30006216:	f080 8205 	bcs.w	30006624 <BOOT_LoadImages+0x904>
3000621a:	2001      	movs	r0, #1
3000621c:	f7fe ffea 	bl	300051f4 <rewrite_bp>
30006220:	46a0      	mov	r8, r4
30006222:	b2c3      	uxtb	r3, r0
30006224:	2601      	movs	r6, #1
30006226:	46a1      	mov	r9, r4
30006228:	f8df b314 	ldr.w	fp, [pc, #788]	; 30006540 <BOOT_LoadImages+0x820>
3000622c:	9303      	str	r3, [sp, #12]
3000622e:	4bae      	ldr	r3, [pc, #696]	; (300064e8 <BOOT_LoadImages+0x7c8>)
30006230:	9304      	str	r3, [sp, #16]
30006232:	4bae      	ldr	r3, [pc, #696]	; (300064ec <BOOT_LoadImages+0x7cc>)
30006234:	9306      	str	r3, [sp, #24]
30006236:	e6c9      	b.n	30005fcc <BOOT_LoadImages+0x2ac>
30006238:	48ad      	ldr	r0, [pc, #692]	; (300064f0 <BOOT_LoadImages+0x7d0>)
3000623a:	930f      	str	r3, [sp, #60]	; 0x3c
3000623c:	9209      	str	r2, [sp, #36]	; 0x24
3000623e:	f005 fff3 	bl	3000c228 <__DiagPrintf_veneer>
30006242:	9a09      	ldr	r2, [sp, #36]	; 0x24
30006244:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30006246:	e63c      	b.n	30005ec2 <BOOT_LoadImages+0x1a2>
30006248:	48aa      	ldr	r0, [pc, #680]	; (300064f4 <BOOT_LoadImages+0x7d4>)
3000624a:	9204      	str	r2, [sp, #16]
3000624c:	f005 ffec 	bl	3000c228 <__DiagPrintf_veneer>
30006250:	9a04      	ldr	r2, [sp, #16]
30006252:	2a00      	cmp	r2, #0
30006254:	f43f af65 	beq.w	30006122 <BOOT_LoadImages+0x402>
30006258:	9f06      	ldr	r7, [sp, #24]
3000625a:	e5fe      	b.n	30005e5a <BOOT_LoadImages+0x13a>
3000625c:	4aa2      	ldr	r2, [pc, #648]	; (300064e8 <BOOT_LoadImages+0x7c8>)
3000625e:	2300      	movs	r3, #0
30006260:	f8df b2dc 	ldr.w	fp, [pc, #732]	; 30006540 <BOOT_LoadImages+0x820>
30006264:	9204      	str	r2, [sp, #16]
30006266:	4699      	mov	r9, r3
30006268:	4aa0      	ldr	r2, [pc, #640]	; (300064ec <BOOT_LoadImages+0x7cc>)
3000626a:	9303      	str	r3, [sp, #12]
3000626c:	9206      	str	r2, [sp, #24]
3000626e:	2f00      	cmp	r7, #0
30006270:	f43f aecf 	beq.w	30006012 <BOOT_LoadImages+0x2f2>
30006274:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006278:	f8d3 200a 	ldr.w	r2, [r3, #10]
3000627c:	3201      	adds	r2, #1
3000627e:	f000 8201 	beq.w	30006684 <BOOT_LoadImages+0x964>
30006282:	4a9d      	ldr	r2, [pc, #628]	; (300064f8 <BOOT_LoadImages+0x7d8>)
30006284:	6892      	ldr	r2, [r2, #8]
30006286:	0792      	lsls	r2, r2, #30
30006288:	f100 81c4 	bmi.w	30006614 <BOOT_LoadImages+0x8f4>
3000628c:	2302      	movs	r3, #2
3000628e:	9303      	str	r3, [sp, #12]
30006290:	e6cd      	b.n	3000602e <BOOT_LoadImages+0x30e>
30006292:	f009 0401 	and.w	r4, r9, #1
30006296:	b1c3      	cbz	r3, 300062ca <BOOT_LoadImages+0x5aa>
30006298:	b1bc      	cbz	r4, 300062ca <BOOT_LoadImages+0x5aa>
3000629a:	4c98      	ldr	r4, [pc, #608]	; (300064fc <BOOT_LoadImages+0x7dc>)
3000629c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300062a0:	47a0      	blx	r4
300062a2:	e7fb      	b.n	3000629c <BOOT_LoadImages+0x57c>
300062a4:	4896      	ldr	r0, [pc, #600]	; (30006500 <BOOT_LoadImages+0x7e0>)
300062a6:	f005 ffbf 	bl	3000c228 <__DiagPrintf_veneer>
300062aa:	9b08      	ldr	r3, [sp, #32]
300062ac:	9a04      	ldr	r2, [sp, #16]
300062ae:	4293      	cmp	r3, r2
300062b0:	f4ff ae20 	bcc.w	30005ef4 <BOOT_LoadImages+0x1d4>
300062b4:	4893      	ldr	r0, [pc, #588]	; (30006504 <BOOT_LoadImages+0x7e4>)
300062b6:	464d      	mov	r5, r9
300062b8:	f005 ffb6 	bl	3000c228 <__DiagPrintf_veneer>
300062bc:	9b07      	ldr	r3, [sp, #28]
300062be:	9f06      	ldr	r7, [sp, #24]
300062c0:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
300062c4:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
300062c8:	e61b      	b.n	30005f02 <BOOT_LoadImages+0x1e2>
300062ca:	9b06      	ldr	r3, [sp, #24]
300062cc:	781b      	ldrb	r3, [r3, #0]
300062ce:	2b00      	cmp	r3, #0
300062d0:	f040 80f9 	bne.w	300064c6 <BOOT_LoadImages+0x7a6>
300062d4:	2001      	movs	r0, #1
300062d6:	b037      	add	sp, #220	; 0xdc
300062d8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300062dc:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
300062e0:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300062e2:	f10c 0104 	add.w	r1, ip, #4
300062e6:	2b00      	cmp	r3, #0
300062e8:	f000 8219 	beq.w	3000671e <BOOT_LoadImages+0x9fe>
300062ec:	1d18      	adds	r0, r3, #4
300062ee:	f04f 32ff 	mov.w	r2, #4294967295
300062f2:	4460      	add	r0, ip
300062f4:	f811 3b01 	ldrb.w	r3, [r1], #1
300062f8:	4053      	eors	r3, r2
300062fa:	4281      	cmp	r1, r0
300062fc:	b2db      	uxtb	r3, r3
300062fe:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006302:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006306:	d1f5      	bne.n	300062f4 <BOOT_LoadImages+0x5d4>
30006308:	43d2      	mvns	r2, r2
3000630a:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000630c:	4293      	cmp	r3, r2
3000630e:	f000 81cf 	beq.w	300066b0 <BOOT_LoadImages+0x990>
30006312:	f8dc 100a 	ldr.w	r1, [ip, #10]
30006316:	f088 0301 	eor.w	r3, r8, #1
3000631a:	3101      	adds	r1, #1
3000631c:	f003 0301 	and.w	r3, r3, #1
30006320:	ea46 0603 	orr.w	r6, r6, r3
30006324:	f000 816a 	beq.w	300065fc <BOOT_LoadImages+0x8dc>
30006328:	990a      	ldr	r1, [sp, #40]	; 0x28
3000632a:	4877      	ldr	r0, [pc, #476]	; (30006508 <BOOT_LoadImages+0x7e8>)
3000632c:	f005 ff7c 	bl	3000c228 <__DiagPrintf_veneer>
30006330:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30006334:	2002      	movs	r0, #2
30006336:	f7fe fe65 	bl	30005004 <FLASH_Erase_With_Lock>
3000633a:	e63d      	b.n	30005fb8 <BOOT_LoadImages+0x298>
3000633c:	f1b8 0f00 	cmp.w	r8, #0
30006340:	f040 826e 	bne.w	30006820 <BOOT_LoadImages+0xb00>
30006344:	2f01      	cmp	r7, #1
30006346:	f000 8117 	beq.w	30006578 <BOOT_LoadImages+0x858>
3000634a:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
3000634e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30006350:	1d31      	adds	r1, r6, #4
30006352:	2b00      	cmp	r3, #0
30006354:	f000 81f2 	beq.w	3000673c <BOOT_LoadImages+0xa1c>
30006358:	1d18      	adds	r0, r3, #4
3000635a:	f04f 32ff 	mov.w	r2, #4294967295
3000635e:	4430      	add	r0, r6
30006360:	f811 3b01 	ldrb.w	r3, [r1], #1
30006364:	4053      	eors	r3, r2
30006366:	4281      	cmp	r1, r0
30006368:	b2db      	uxtb	r3, r3
3000636a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000636e:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006372:	d1f5      	bne.n	30006360 <BOOT_LoadImages+0x640>
30006374:	43d2      	mvns	r2, r2
30006376:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30006378:	4293      	cmp	r3, r2
3000637a:	f000 81bc 	beq.w	300066f6 <BOOT_LoadImages+0x9d6>
3000637e:	f8d6 300a 	ldr.w	r3, [r6, #10]
30006382:	3301      	adds	r3, #1
30006384:	f000 8132 	beq.w	300065ec <BOOT_LoadImages+0x8cc>
30006388:	990a      	ldr	r1, [sp, #40]	; 0x28
3000638a:	485f      	ldr	r0, [pc, #380]	; (30006508 <BOOT_LoadImages+0x7e8>)
3000638c:	f005 ff4c 	bl	3000c228 <__DiagPrintf_veneer>
30006390:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30006394:	2002      	movs	r0, #2
30006396:	f7fe fe35 	bl	30005004 <FLASH_Erase_With_Lock>
3000639a:	4b57      	ldr	r3, [pc, #348]	; (300064f8 <BOOT_LoadImages+0x7d8>)
3000639c:	689b      	ldr	r3, [r3, #8]
3000639e:	079b      	lsls	r3, r3, #30
300063a0:	d400      	bmi.n	300063a4 <BOOT_LoadImages+0x684>
300063a2:	e7fe      	b.n	300063a2 <BOOT_LoadImages+0x682>
300063a4:	4859      	ldr	r0, [pc, #356]	; (3000650c <BOOT_LoadImages+0x7ec>)
300063a6:	f005 ff3f 	bl	3000c228 <__DiagPrintf_veneer>
300063aa:	e7fa      	b.n	300063a2 <BOOT_LoadImages+0x682>
300063ac:	9908      	ldr	r1, [sp, #32]
300063ae:	4858      	ldr	r0, [pc, #352]	; (30006510 <BOOT_LoadImages+0x7f0>)
300063b0:	f005 ff3a 	bl	3000c228 <__DiagPrintf_veneer>
300063b4:	9b07      	ldr	r3, [sp, #28]
300063b6:	3303      	adds	r3, #3
300063b8:	f240 81b3 	bls.w	30006722 <BOOT_LoadImages+0xa02>
300063bc:	4855      	ldr	r0, [pc, #340]	; (30006514 <BOOT_LoadImages+0x7f4>)
300063be:	f005 ff33 	bl	3000c228 <__DiagPrintf_veneer>
300063c2:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
300063c6:	f8d9 3011 	ldr.w	r3, [r9, #17]
300063ca:	f8d9 a00d 	ldr.w	sl, [r9, #13]
300063ce:	9307      	str	r3, [sp, #28]
300063d0:	f8d5 b00d 	ldr.w	fp, [r5, #13]
300063d4:	f8d5 8011 	ldr.w	r8, [r5, #17]
300063d8:	484f      	ldr	r0, [pc, #316]	; (30006518 <BOOT_LoadImages+0x7f8>)
300063da:	f04f 32ff 	mov.w	r2, #4294967295
300063de:	5931      	ldr	r1, [r6, r4]
300063e0:	4430      	add	r0, r6
300063e2:	4c4e      	ldr	r4, [pc, #312]	; (3000651c <BOOT_LoadImages+0x7fc>)
300063e4:	f810 3b01 	ldrb.w	r3, [r0], #1
300063e8:	4053      	eors	r3, r2
300063ea:	4285      	cmp	r5, r0
300063ec:	b2db      	uxtb	r3, r3
300063ee:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300063f2:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300063f6:	d1f5      	bne.n	300063e4 <BOOT_LoadImages+0x6c4>
300063f8:	43d2      	mvns	r2, r2
300063fa:	4291      	cmp	r1, r2
300063fc:	d010      	beq.n	30006420 <BOOT_LoadImages+0x700>
300063fe:	4848      	ldr	r0, [pc, #288]	; (30006520 <BOOT_LoadImages+0x800>)
30006400:	f005 ff12 	bl	3000c228 <__DiagPrintf_veneer>
30006404:	9b09      	ldr	r3, [sp, #36]	; 0x24
30006406:	2b00      	cmp	r3, #0
30006408:	f43f ae8b 	beq.w	30006122 <BOOT_LoadImages+0x402>
3000640c:	2300      	movs	r3, #0
3000640e:	9309      	str	r3, [sp, #36]	; 0x24
30006410:	e4e9      	b.n	30005de6 <BOOT_LoadImages+0xc6>
30006412:	460a      	mov	r2, r1
30006414:	4843      	ldr	r0, [pc, #268]	; (30006524 <BOOT_LoadImages+0x804>)
30006416:	f005 ff07 	bl	3000c228 <__DiagPrintf_veneer>
3000641a:	2201      	movs	r2, #1
3000641c:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000641e:	e4fe      	b.n	30005e1e <BOOT_LoadImages+0xfe>
30006420:	460a      	mov	r2, r1
30006422:	4841      	ldr	r0, [pc, #260]	; (30006528 <BOOT_LoadImages+0x808>)
30006424:	f005 ff00 	bl	3000c228 <__DiagPrintf_veneer>
30006428:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000642a:	2b00      	cmp	r3, #0
3000642c:	f47f acdb 	bne.w	30005de6 <BOOT_LoadImages+0xc6>
30006430:	461a      	mov	r2, r3
30006432:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30006436:	e4f5      	b.n	30005e24 <BOOT_LoadImages+0x104>
30006438:	483c      	ldr	r0, [pc, #240]	; (3000652c <BOOT_LoadImages+0x80c>)
3000643a:	f005 fef5 	bl	3000c228 <__DiagPrintf_veneer>
3000643e:	e488      	b.n	30005d52 <BOOT_LoadImages+0x32>
30006440:	4c36      	ldr	r4, [pc, #216]	; (3000651c <BOOT_LoadImages+0x7fc>)
30006442:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006446:	3310      	adds	r3, #16
30006448:	930e      	str	r3, [sp, #56]	; 0x38
3000644a:	2301      	movs	r3, #1
3000644c:	9303      	str	r3, [sp, #12]
3000644e:	e5c5      	b.n	30005fdc <BOOT_LoadImages+0x2bc>
30006450:	4837      	ldr	r0, [pc, #220]	; (30006530 <BOOT_LoadImages+0x810>)
30006452:	e65b      	b.n	3000610c <BOOT_LoadImages+0x3ec>
30006454:	4837      	ldr	r0, [pc, #220]	; (30006534 <BOOT_LoadImages+0x814>)
30006456:	f005 fee7 	bl	3000c228 <__DiagPrintf_veneer>
3000645a:	e662      	b.n	30006122 <BOOT_LoadImages+0x402>
3000645c:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006460:	2280      	movs	r2, #128	; 0x80
30006462:	21ff      	movs	r1, #255	; 0xff
30006464:	4650      	mov	r0, sl
30006466:	f005 febf 	bl	3000c1e8 <____wrap_memset_veneer>
3000646a:	2280      	movs	r2, #128	; 0x80
3000646c:	4629      	mov	r1, r5
3000646e:	4650      	mov	r0, sl
30006470:	f005 fec2 	bl	3000c1f8 <____wrap_memcpy_veneer>
30006474:	f1b9 0f00 	cmp.w	r9, #0
30006478:	f040 81ba 	bne.w	300067f0 <BOOT_LoadImages+0xad0>
3000647c:	b1e7      	cbz	r7, 300064b8 <BOOT_LoadImages+0x798>
3000647e:	f8cd 9014 	str.w	r9, [sp, #20]
30006482:	f04f 0900 	mov.w	r9, #0
30006486:	4649      	mov	r1, r9
30006488:	f88d 9064 	strb.w	r9, [sp, #100]	; 0x64
3000648c:	e5ee      	b.n	3000606c <BOOT_LoadImages+0x34c>
3000648e:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006492:	4829      	ldr	r0, [pc, #164]	; (30006538 <BOOT_LoadImages+0x818>)
30006494:	f005 fec8 	bl	3000c228 <__DiagPrintf_veneer>
30006498:	2280      	movs	r2, #128	; 0x80
3000649a:	21ff      	movs	r1, #255	; 0xff
3000649c:	4650      	mov	r0, sl
3000649e:	f005 fea3 	bl	3000c1e8 <____wrap_memset_veneer>
300064a2:	2280      	movs	r2, #128	; 0x80
300064a4:	4629      	mov	r1, r5
300064a6:	4650      	mov	r0, sl
300064a8:	46b8      	mov	r8, r7
300064aa:	f005 fea5 	bl	3000c1f8 <____wrap_memcpy_veneer>
300064ae:	f1b9 0f00 	cmp.w	r9, #0
300064b2:	f47f add4 	bne.w	3000605e <BOOT_LoadImages+0x33e>
300064b6:	2701      	movs	r7, #1
300064b8:	2101      	movs	r1, #1
300064ba:	9705      	str	r7, [sp, #20]
300064bc:	2700      	movs	r7, #0
300064be:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300064c2:	46b9      	mov	r9, r7
300064c4:	e5d2      	b.n	3000606c <BOOT_LoadImages+0x34c>
300064c6:	4b1d      	ldr	r3, [pc, #116]	; (3000653c <BOOT_LoadImages+0x81c>)
300064c8:	4648      	mov	r0, r9
300064ca:	4798      	blx	r3
300064cc:	1e03      	subs	r3, r0, #0
300064ce:	bf18      	it	ne
300064d0:	2301      	movne	r3, #1
300064d2:	b110      	cbz	r0, 300064da <BOOT_LoadImages+0x7ba>
300064d4:	2e00      	cmp	r6, #0
300064d6:	f47f ad90 	bne.w	30005ffa <BOOT_LoadImages+0x2da>
300064da:	2c00      	cmp	r4, #0
300064dc:	f43f aefa 	beq.w	300062d4 <BOOT_LoadImages+0x5b4>
300064e0:	2b00      	cmp	r3, #0
300064e2:	f47f aeda 	bne.w	3000629a <BOOT_LoadImages+0x57a>
300064e6:	e6f5      	b.n	300062d4 <BOOT_LoadImages+0x5b4>
300064e8:	300043c1 	.word	0x300043c1
300064ec:	3000d80a 	.word	0x3000d80a
300064f0:	3000cb60 	.word	0x3000cb60
300064f4:	3000cd7c 	.word	0x3000cd7c
300064f8:	2001c00c 	.word	0x2001c00c
300064fc:	00009be5 	.word	0x00009be5
30006500:	3000cb7c 	.word	0x3000cb7c
30006504:	3000cb90 	.word	0x3000cb90
30006508:	3000cccc 	.word	0x3000cccc
3000650c:	3000c92c 	.word	0x3000c92c
30006510:	3000ca1c 	.word	0x3000ca1c
30006514:	3000cd98 	.word	0x3000cd98
30006518:	07ffe004 	.word	0x07ffe004
3000651c:	3000dd9c 	.word	0x3000dd9c
30006520:	3000ca88 	.word	0x3000ca88
30006524:	3000cac8 	.word	0x3000cac8
30006528:	3000ca4c 	.word	0x3000ca4c
3000652c:	3000c9c0 	.word	0x3000c9c0
30006530:	3000cdb0 	.word	0x3000cdb0
30006534:	3000cdcc 	.word	0x3000cdcc
30006538:	3000cbe8 	.word	0x3000cbe8
3000653c:	300043c5 	.word	0x300043c5
30006540:	3000eea0 	.word	0x3000eea0
30006544:	4293      	cmp	r3, r2
30006546:	f04f 0401 	mov.w	r4, #1
3000654a:	464b      	mov	r3, r9
3000654c:	9a07      	ldr	r2, [sp, #28]
3000654e:	bf28      	it	cs
30006550:	4630      	movcs	r0, r6
30006552:	4651      	mov	r1, sl
30006554:	bf38      	it	cc
30006556:	4620      	movcc	r0, r4
30006558:	9500      	str	r5, [sp, #0]
3000655a:	9401      	str	r4, [sp, #4]
3000655c:	46b0      	mov	r8, r6
3000655e:	f7fe fe49 	bl	300051f4 <rewrite_bp>
30006562:	4626      	mov	r6, r4
30006564:	b2c3      	uxtb	r3, r0
30006566:	f8df b310 	ldr.w	fp, [pc, #784]	; 30006878 <BOOT_LoadImages+0xb58>
3000656a:	46c1      	mov	r9, r8
3000656c:	9303      	str	r3, [sp, #12]
3000656e:	4bb3      	ldr	r3, [pc, #716]	; (3000683c <BOOT_LoadImages+0xb1c>)
30006570:	9304      	str	r3, [sp, #16]
30006572:	4bb3      	ldr	r3, [pc, #716]	; (30006840 <BOOT_LoadImages+0xb20>)
30006574:	9306      	str	r3, [sp, #24]
30006576:	e529      	b.n	30005fcc <BOOT_LoadImages+0x2ac>
30006578:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
3000657c:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000657e:	1d31      	adds	r1, r6, #4
30006580:	2b00      	cmp	r3, #0
30006582:	f000 811e 	beq.w	300067c2 <BOOT_LoadImages+0xaa2>
30006586:	1d18      	adds	r0, r3, #4
30006588:	f04f 32ff 	mov.w	r2, #4294967295
3000658c:	4430      	add	r0, r6
3000658e:	f811 3b01 	ldrb.w	r3, [r1], #1
30006592:	4053      	eors	r3, r2
30006594:	4281      	cmp	r1, r0
30006596:	b2db      	uxtb	r3, r3
30006598:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000659c:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300065a0:	d1f5      	bne.n	3000658e <BOOT_LoadImages+0x86e>
300065a2:	43d2      	mvns	r2, r2
300065a4:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300065a6:	4293      	cmp	r3, r2
300065a8:	f000 80e1 	beq.w	3000676e <BOOT_LoadImages+0xa4e>
300065ac:	f8d6 300a 	ldr.w	r3, [r6, #10]
300065b0:	3301      	adds	r3, #1
300065b2:	d054      	beq.n	3000665e <BOOT_LoadImages+0x93e>
300065b4:	990b      	ldr	r1, [sp, #44]	; 0x2c
300065b6:	48a3      	ldr	r0, [pc, #652]	; (30006844 <BOOT_LoadImages+0xb24>)
300065b8:	f005 fe36 	bl	3000c228 <__DiagPrintf_veneer>
300065bc:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
300065c0:	2002      	movs	r0, #2
300065c2:	f7fe fd1f 	bl	30005004 <FLASH_Erase_With_Lock>
300065c6:	e6e8      	b.n	3000639a <BOOT_LoadImages+0x67a>
300065c8:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300065cc:	f8d3 200a 	ldr.w	r2, [r3, #10]
300065d0:	3201      	adds	r2, #1
300065d2:	d044      	beq.n	3000665e <BOOT_LoadImages+0x93e>
300065d4:	4a9c      	ldr	r2, [pc, #624]	; (30006848 <BOOT_LoadImages+0xb28>)
300065d6:	6892      	ldr	r2, [r2, #8]
300065d8:	0791      	lsls	r1, r2, #30
300065da:	d444      	bmi.n	30006666 <BOOT_LoadImages+0x946>
300065dc:	f089 0601 	eor.w	r6, r9, #1
300065e0:	9705      	str	r7, [sp, #20]
300065e2:	2701      	movs	r7, #1
300065e4:	f006 0601 	and.w	r6, r6, #1
300065e8:	46b8      	mov	r8, r7
300065ea:	e72c      	b.n	30006446 <BOOT_LoadImages+0x726>
300065ec:	4897      	ldr	r0, [pc, #604]	; (3000684c <BOOT_LoadImages+0xb2c>)
300065ee:	f005 fe1b 	bl	3000c228 <__DiagPrintf_veneer>
300065f2:	e6d2      	b.n	3000639a <BOOT_LoadImages+0x67a>
300065f4:	4896      	ldr	r0, [pc, #600]	; (30006850 <BOOT_LoadImages+0xb30>)
300065f6:	f005 fe17 	bl	3000c228 <__DiagPrintf_veneer>
300065fa:	e4dd      	b.n	30005fb8 <BOOT_LoadImages+0x298>
300065fc:	4893      	ldr	r0, [pc, #588]	; (3000684c <BOOT_LoadImages+0xb2c>)
300065fe:	f005 fe13 	bl	3000c228 <__DiagPrintf_veneer>
30006602:	e4d9      	b.n	30005fb8 <BOOT_LoadImages+0x298>
30006604:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006608:	4892      	ldr	r0, [pc, #584]	; (30006854 <BOOT_LoadImages+0xb34>)
3000660a:	f005 fe0d 	bl	3000c228 <__DiagPrintf_veneer>
3000660e:	2301      	movs	r3, #1
30006610:	9303      	str	r3, [sp, #12]
30006612:	e50c      	b.n	3000602e <BOOT_LoadImages+0x30e>
30006614:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006618:	488f      	ldr	r0, [pc, #572]	; (30006858 <BOOT_LoadImages+0xb38>)
3000661a:	f005 fe05 	bl	3000c228 <__DiagPrintf_veneer>
3000661e:	2302      	movs	r3, #2
30006620:	9303      	str	r3, [sp, #12]
30006622:	e504      	b.n	3000602e <BOOT_LoadImages+0x30e>
30006624:	4620      	mov	r0, r4
30006626:	e5f9      	b.n	3000621c <BOOT_LoadImages+0x4fc>
30006628:	428a      	cmp	r2, r1
3000662a:	f4ff adad 	bcc.w	30006188 <BOOT_LoadImages+0x468>
3000662e:	4282      	cmp	r2, r0
30006630:	f4ff adaa 	bcc.w	30006188 <BOOT_LoadImages+0x468>
30006634:	2601      	movs	r6, #1
30006636:	464b      	mov	r3, r9
30006638:	9a07      	ldr	r2, [sp, #28]
3000663a:	4651      	mov	r1, sl
3000663c:	4630      	mov	r0, r6
3000663e:	9500      	str	r5, [sp, #0]
30006640:	9601      	str	r6, [sp, #4]
30006642:	f7fe fdd7 	bl	300051f4 <rewrite_bp>
30006646:	f04f 0800 	mov.w	r8, #0
3000664a:	b2c3      	uxtb	r3, r0
3000664c:	f8df b228 	ldr.w	fp, [pc, #552]	; 30006878 <BOOT_LoadImages+0xb58>
30006650:	46c1      	mov	r9, r8
30006652:	9303      	str	r3, [sp, #12]
30006654:	4b79      	ldr	r3, [pc, #484]	; (3000683c <BOOT_LoadImages+0xb1c>)
30006656:	9304      	str	r3, [sp, #16]
30006658:	4b79      	ldr	r3, [pc, #484]	; (30006840 <BOOT_LoadImages+0xb20>)
3000665a:	9306      	str	r3, [sp, #24]
3000665c:	e4b6      	b.n	30005fcc <BOOT_LoadImages+0x2ac>
3000665e:	487c      	ldr	r0, [pc, #496]	; (30006850 <BOOT_LoadImages+0xb30>)
30006660:	f005 fde2 	bl	3000c228 <__DiagPrintf_veneer>
30006664:	e699      	b.n	3000639a <BOOT_LoadImages+0x67a>
30006666:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000666a:	487a      	ldr	r0, [pc, #488]	; (30006854 <BOOT_LoadImages+0xb34>)
3000666c:	f005 fddc 	bl	3000c228 <__DiagPrintf_veneer>
30006670:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006674:	e7b2      	b.n	300065dc <BOOT_LoadImages+0x8bc>
30006676:	f8cd 9014 	str.w	r9, [sp, #20]
3000667a:	4639      	mov	r1, r7
3000667c:	46b9      	mov	r9, r7
3000667e:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
30006682:	e4f3      	b.n	3000606c <BOOT_LoadImages+0x34c>
30006684:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006688:	4870      	ldr	r0, [pc, #448]	; (3000684c <BOOT_LoadImages+0xb2c>)
3000668a:	f005 fdcd 	bl	3000c228 <__DiagPrintf_veneer>
3000668e:	2280      	movs	r2, #128	; 0x80
30006690:	21ff      	movs	r1, #255	; 0xff
30006692:	4650      	mov	r0, sl
30006694:	f005 fda8 	bl	3000c1e8 <____wrap_memset_veneer>
30006698:	2280      	movs	r2, #128	; 0x80
3000669a:	4629      	mov	r1, r5
3000669c:	4650      	mov	r0, sl
3000669e:	f005 fdab 	bl	3000c1f8 <____wrap_memcpy_veneer>
300066a2:	f1b9 0f00 	cmp.w	r9, #0
300066a6:	d146      	bne.n	30006736 <BOOT_LoadImages+0xa16>
300066a8:	46c8      	mov	r8, r9
300066aa:	f04f 0901 	mov.w	r9, #1
300066ae:	e6e6      	b.n	3000647e <BOOT_LoadImages+0x75e>
300066b0:	461a      	mov	r2, r3
300066b2:	4619      	mov	r1, r3
300066b4:	4869      	ldr	r0, [pc, #420]	; (3000685c <BOOT_LoadImages+0xb3c>)
300066b6:	f005 fdb7 	bl	3000c228 <__DiagPrintf_veneer>
300066ba:	4b63      	ldr	r3, [pc, #396]	; (30006848 <BOOT_LoadImages+0xb28>)
300066bc:	689b      	ldr	r3, [r3, #8]
300066be:	0798      	lsls	r0, r3, #30
300066c0:	d449      	bmi.n	30006756 <BOOT_LoadImages+0xa36>
300066c2:	2302      	movs	r3, #2
300066c4:	f04f 0801 	mov.w	r8, #1
300066c8:	9303      	str	r3, [sp, #12]
300066ca:	e475      	b.n	30005fb8 <BOOT_LoadImages+0x298>
300066cc:	461a      	mov	r2, r3
300066ce:	4619      	mov	r1, r3
300066d0:	4863      	ldr	r0, [pc, #396]	; (30006860 <BOOT_LoadImages+0xb40>)
300066d2:	f005 fda9 	bl	3000c228 <__DiagPrintf_veneer>
300066d6:	4b5c      	ldr	r3, [pc, #368]	; (30006848 <BOOT_LoadImages+0xb28>)
300066d8:	689b      	ldr	r3, [r3, #8]
300066da:	079b      	lsls	r3, r3, #30
300066dc:	d430      	bmi.n	30006740 <BOOT_LoadImages+0xa20>
300066de:	2301      	movs	r3, #1
300066e0:	4698      	mov	r8, r3
300066e2:	9303      	str	r3, [sp, #12]
300066e4:	e468      	b.n	30005fb8 <BOOT_LoadImages+0x298>
300066e6:	f8d3 1006 	ldr.w	r1, [r3, #6]
300066ea:	485b      	ldr	r0, [pc, #364]	; (30006858 <BOOT_LoadImages+0xb38>)
300066ec:	f005 fd9c 	bl	3000c228 <__DiagPrintf_veneer>
300066f0:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300066f4:	e4f8      	b.n	300060e8 <BOOT_LoadImages+0x3c8>
300066f6:	461a      	mov	r2, r3
300066f8:	4619      	mov	r1, r3
300066fa:	4858      	ldr	r0, [pc, #352]	; (3000685c <BOOT_LoadImages+0xb3c>)
300066fc:	f005 fd94 	bl	3000c228 <__DiagPrintf_veneer>
30006700:	4b51      	ldr	r3, [pc, #324]	; (30006848 <BOOT_LoadImages+0xb28>)
30006702:	689a      	ldr	r2, [r3, #8]
30006704:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006708:	f012 0202 	ands.w	r2, r2, #2
3000670c:	d145      	bne.n	3000679a <BOOT_LoadImages+0xa7a>
3000670e:	9205      	str	r2, [sp, #20]
30006710:	e4ea      	b.n	300060e8 <BOOT_LoadImages+0x3c8>
30006712:	4854      	ldr	r0, [pc, #336]	; (30006864 <BOOT_LoadImages+0xb44>)
30006714:	f005 fd88 	bl	3000c228 <__DiagPrintf_veneer>
30006718:	e7fe      	b.n	30006718 <BOOT_LoadImages+0x9f8>
3000671a:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000671c:	e434      	b.n	30005f88 <BOOT_LoadImages+0x268>
3000671e:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30006720:	e5f3      	b.n	3000630a <BOOT_LoadImages+0x5ea>
30006722:	f10a 3aff 	add.w	sl, sl, #4294967295
30006726:	f11a 0f03 	cmn.w	sl, #3
3000672a:	d84c      	bhi.n	300067c6 <BOOT_LoadImages+0xaa6>
3000672c:	f1bb 0f01 	cmp.w	fp, #1
30006730:	d966      	bls.n	30006800 <BOOT_LoadImages+0xae0>
30006732:	484d      	ldr	r0, [pc, #308]	; (30006868 <BOOT_LoadImages+0xb48>)
30006734:	e643      	b.n	300063be <BOOT_LoadImages+0x69e>
30006736:	f04f 0800 	mov.w	r8, #0
3000673a:	e48d      	b.n	30006058 <BOOT_LoadImages+0x338>
3000673c:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000673e:	e61a      	b.n	30006376 <BOOT_LoadImages+0x656>
30006740:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006744:	4843      	ldr	r0, [pc, #268]	; (30006854 <BOOT_LoadImages+0xb34>)
30006746:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000674a:	f005 fd6d 	bl	3000c228 <__DiagPrintf_veneer>
3000674e:	2301      	movs	r3, #1
30006750:	4698      	mov	r8, r3
30006752:	9303      	str	r3, [sp, #12]
30006754:	e430      	b.n	30005fb8 <BOOT_LoadImages+0x298>
30006756:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000675a:	f04f 0801 	mov.w	r8, #1
3000675e:	483e      	ldr	r0, [pc, #248]	; (30006858 <BOOT_LoadImages+0xb38>)
30006760:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006764:	f005 fd60 	bl	3000c228 <__DiagPrintf_veneer>
30006768:	2302      	movs	r3, #2
3000676a:	9303      	str	r3, [sp, #12]
3000676c:	e424      	b.n	30005fb8 <BOOT_LoadImages+0x298>
3000676e:	461a      	mov	r2, r3
30006770:	4619      	mov	r1, r3
30006772:	483b      	ldr	r0, [pc, #236]	; (30006860 <BOOT_LoadImages+0xb40>)
30006774:	f005 fd58 	bl	3000c228 <__DiagPrintf_veneer>
30006778:	4b33      	ldr	r3, [pc, #204]	; (30006848 <BOOT_LoadImages+0xb28>)
3000677a:	689f      	ldr	r7, [r3, #8]
3000677c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006780:	f017 0702 	ands.w	r7, r7, #2
30006784:	f43f af2a 	beq.w	300065dc <BOOT_LoadImages+0x8bc>
30006788:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000678c:	2700      	movs	r7, #0
3000678e:	4831      	ldr	r0, [pc, #196]	; (30006854 <BOOT_LoadImages+0xb34>)
30006790:	f005 fd4a 	bl	3000c228 <__DiagPrintf_veneer>
30006794:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006798:	e720      	b.n	300065dc <BOOT_LoadImages+0x8bc>
3000679a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000679e:	482e      	ldr	r0, [pc, #184]	; (30006858 <BOOT_LoadImages+0xb38>)
300067a0:	f005 fd42 	bl	3000c228 <__DiagPrintf_veneer>
300067a4:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300067a8:	2200      	movs	r2, #0
300067aa:	e7b0      	b.n	3000670e <BOOT_LoadImages+0x9ee>
300067ac:	4281      	cmp	r1, r0
300067ae:	f4ff acf1 	bcc.w	30006194 <BOOT_LoadImages+0x474>
300067b2:	2400      	movs	r4, #0
300067b4:	464b      	mov	r3, r9
300067b6:	4642      	mov	r2, r8
300067b8:	4659      	mov	r1, fp
300067ba:	4620      	mov	r0, r4
300067bc:	9500      	str	r5, [sp, #0]
300067be:	9401      	str	r4, [sp, #4]
300067c0:	e52c      	b.n	3000621c <BOOT_LoadImages+0x4fc>
300067c2:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300067c4:	e6ee      	b.n	300065a4 <BOOT_LoadImages+0x884>
300067c6:	4829      	ldr	r0, [pc, #164]	; (3000686c <BOOT_LoadImages+0xb4c>)
300067c8:	e5f9      	b.n	300063be <BOOT_LoadImages+0x69e>
300067ca:	2601      	movs	r6, #1
300067cc:	464b      	mov	r3, r9
300067ce:	9a07      	ldr	r2, [sp, #28]
300067d0:	4651      	mov	r1, sl
300067d2:	2000      	movs	r0, #0
300067d4:	9500      	str	r5, [sp, #0]
300067d6:	9601      	str	r6, [sp, #4]
300067d8:	e733      	b.n	30006642 <BOOT_LoadImages+0x922>
300067da:	2400      	movs	r4, #0
300067dc:	464b      	mov	r3, r9
300067de:	4642      	mov	r2, r8
300067e0:	4659      	mov	r1, fp
300067e2:	9500      	str	r5, [sp, #0]
300067e4:	9401      	str	r4, [sp, #4]
300067e6:	e518      	b.n	3000621a <BOOT_LoadImages+0x4fa>
300067e8:	4c21      	ldr	r4, [pc, #132]	; (30006870 <BOOT_LoadImages+0xb50>)
300067ea:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300067ee:	e481      	b.n	300060f4 <BOOT_LoadImages+0x3d4>
300067f0:	f04f 0900 	mov.w	r9, #0
300067f4:	e430      	b.n	30006058 <BOOT_LoadImages+0x338>
300067f6:	2601      	movs	r6, #1
300067f8:	4c1d      	ldr	r4, [pc, #116]	; (30006870 <BOOT_LoadImages+0xb50>)
300067fa:	46b1      	mov	r9, r6
300067fc:	f7ff bbaa 	b.w	30005f54 <BOOT_LoadImages+0x234>
30006800:	9904      	ldr	r1, [sp, #16]
30006802:	481c      	ldr	r0, [pc, #112]	; (30006874 <BOOT_LoadImages+0xb54>)
30006804:	f005 fd10 	bl	3000c228 <__DiagPrintf_veneer>
30006808:	f8d9 3011 	ldr.w	r3, [r9, #17]
3000680c:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006810:	9307      	str	r3, [sp, #28]
30006812:	2301      	movs	r3, #1
30006814:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30006818:	f8d5 8011 	ldr.w	r8, [r5, #17]
3000681c:	9309      	str	r3, [sp, #36]	; 0x24
3000681e:	e5db      	b.n	300063d8 <BOOT_LoadImages+0x6b8>
30006820:	f089 0601 	eor.w	r6, r9, #1
30006824:	f006 0601 	and.w	r6, r6, #1
30006828:	f7ff bbd0 	b.w	30005fcc <BOOT_LoadImages+0x2ac>
3000682c:	f089 0601 	eor.w	r6, r9, #1
30006830:	f8cd 8014 	str.w	r8, [sp, #20]
30006834:	f006 0601 	and.w	r6, r6, #1
30006838:	f7ff bbc8 	b.w	30005fcc <BOOT_LoadImages+0x2ac>
3000683c:	300043c1 	.word	0x300043c1
30006840:	3000d80a 	.word	0x3000d80a
30006844:	3000cc44 	.word	0x3000cc44
30006848:	2001c00c 	.word	0x2001c00c
3000684c:	3000cbf8 	.word	0x3000cbf8
30006850:	3000cbe8 	.word	0x3000cbe8
30006854:	3000c8cc 	.word	0x3000c8cc
30006858:	3000c8fc 	.word	0x3000c8fc
3000685c:	3000cc90 	.word	0x3000cc90
30006860:	3000cc08 	.word	0x3000cc08
30006864:	3000cd50 	.word	0x3000cd50
30006868:	3000cdb0 	.word	0x3000cdb0
3000686c:	3000ca38 	.word	0x3000ca38
30006870:	3000dd9c 	.word	0x3000dd9c
30006874:	3000cde8 	.word	0x3000cde8
30006878:	3000eea0 	.word	0x3000eea0

3000687c <BOOT_ReasonSet>:
3000687c:	4a03      	ldr	r2, [pc, #12]	; (3000688c <BOOT_ReasonSet+0x10>)
3000687e:	4904      	ldr	r1, [pc, #16]	; (30006890 <BOOT_ReasonSet+0x14>)
30006880:	8c13      	ldrh	r3, [r2, #32]
30006882:	b29b      	uxth	r3, r3
30006884:	8413      	strh	r3, [r2, #32]
30006886:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
3000688a:	4770      	bx	lr
3000688c:	42008000 	.word	0x42008000
30006890:	42008200 	.word	0x42008200

30006894 <BOOT_Enable_KM0>:
30006894:	b510      	push	{r4, lr}
30006896:	4c13      	ldr	r4, [pc, #76]	; (300068e4 <BOOT_Enable_KM0+0x50>)
30006898:	47a0      	blx	r4
3000689a:	b110      	cbz	r0, 300068a2 <BOOT_Enable_KM0+0xe>
3000689c:	4b12      	ldr	r3, [pc, #72]	; (300068e8 <BOOT_Enable_KM0+0x54>)
3000689e:	4798      	blx	r3
300068a0:	b1b0      	cbz	r0, 300068d0 <BOOT_Enable_KM0+0x3c>
300068a2:	47a0      	blx	r4
300068a4:	b940      	cbnz	r0, 300068b8 <BOOT_Enable_KM0+0x24>
300068a6:	4a11      	ldr	r2, [pc, #68]	; (300068ec <BOOT_Enable_KM0+0x58>)
300068a8:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300068ac:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300068b0:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300068b4:	bf40      	sev
300068b6:	bd10      	pop	{r4, pc}
300068b8:	4b0b      	ldr	r3, [pc, #44]	; (300068e8 <BOOT_Enable_KM0+0x54>)
300068ba:	4798      	blx	r3
300068bc:	2800      	cmp	r0, #0
300068be:	d0f2      	beq.n	300068a6 <BOOT_Enable_KM0+0x12>
300068c0:	4b0b      	ldr	r3, [pc, #44]	; (300068f0 <BOOT_Enable_KM0+0x5c>)
300068c2:	689b      	ldr	r3, [r3, #8]
300068c4:	079b      	lsls	r3, r3, #30
300068c6:	d5ee      	bpl.n	300068a6 <BOOT_Enable_KM0+0x12>
300068c8:	480a      	ldr	r0, [pc, #40]	; (300068f4 <BOOT_Enable_KM0+0x60>)
300068ca:	f005 fcad 	bl	3000c228 <__DiagPrintf_veneer>
300068ce:	e7ea      	b.n	300068a6 <BOOT_Enable_KM0+0x12>
300068d0:	4b07      	ldr	r3, [pc, #28]	; (300068f0 <BOOT_Enable_KM0+0x5c>)
300068d2:	689b      	ldr	r3, [r3, #8]
300068d4:	079a      	lsls	r2, r3, #30
300068d6:	d5ee      	bpl.n	300068b6 <BOOT_Enable_KM0+0x22>
300068d8:	4807      	ldr	r0, [pc, #28]	; (300068f8 <BOOT_Enable_KM0+0x64>)
300068da:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300068de:	f005 bca3 	b.w	3000c228 <__DiagPrintf_veneer>
300068e2:	bf00      	nop
300068e4:	0000c09d 	.word	0x0000c09d
300068e8:	0000c0ad 	.word	0x0000c0ad
300068ec:	42008000 	.word	0x42008000
300068f0:	2001c00c 	.word	0x2001c00c
300068f4:	3000ce40 	.word	0x3000ce40
300068f8:	3000ce04 	.word	0x3000ce04

300068fc <BOOT_AP_Clk_Get>:
300068fc:	4a06      	ldr	r2, [pc, #24]	; (30006918 <BOOT_AP_Clk_Get+0x1c>)
300068fe:	6913      	ldr	r3, [r2, #16]
30006900:	2b00      	cmp	r3, #0
30006902:	db03      	blt.n	3000690c <BOOT_AP_Clk_Get+0x10>
30006904:	6810      	ldr	r0, [r2, #0]
30006906:	fbb0 f0f3 	udiv	r0, r0, r3
3000690a:	4770      	bx	lr
3000690c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30006910:	6850      	ldr	r0, [r2, #4]
30006912:	fbb0 f0f3 	udiv	r0, r0, r3
30006916:	4770      	bx	lr
30006918:	3000e424 	.word	0x3000e424

3000691c <BOOT_SOC_ClkSet>:
3000691c:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30006920:	4f5a      	ldr	r7, [pc, #360]	; (30006a8c <BOOT_SOC_ClkSet+0x170>)
30006922:	693d      	ldr	r5, [r7, #16]
30006924:	683e      	ldr	r6, [r7, #0]
30006926:	2d00      	cmp	r5, #0
30006928:	db73      	blt.n	30006a12 <BOOT_SOC_ClkSet+0xf6>
3000692a:	fbb6 f5f5 	udiv	r5, r6, r5
3000692e:	4b58      	ldr	r3, [pc, #352]	; (30006a90 <BOOT_SOC_ClkSet+0x174>)
30006930:	f8d7 a008 	ldr.w	sl, [r7, #8]
30006934:	fba3 1306 	umull	r1, r3, r3, r6
30006938:	68fa      	ldr	r2, [r7, #12]
3000693a:	f1ba 0f01 	cmp.w	sl, #1
3000693e:	ea4f 19d3 	mov.w	r9, r3, lsr #7
30006942:	ea4f 1853 	mov.w	r8, r3, lsr #5
30006946:	ea4f 1393 	mov.w	r3, r3, lsr #6
3000694a:	f109 39ff 	add.w	r9, r9, #4294967295
3000694e:	f108 38ff 	add.w	r8, r8, #4294967295
30006952:	f103 34ff 	add.w	r4, r3, #4294967295
30006956:	fbb6 f6f2 	udiv	r6, r6, r2
3000695a:	d071      	beq.n	30006a40 <BOOT_SOC_ClkSet+0x124>
3000695c:	4b4d      	ldr	r3, [pc, #308]	; (30006a94 <BOOT_SOC_ClkSet+0x178>)
3000695e:	2200      	movs	r2, #0
30006960:	2efa      	cmp	r6, #250	; 0xfa
30006962:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30006966:	d865      	bhi.n	30006a34 <BOOT_SOC_ClkSet+0x118>
30006968:	f5b5 7f66 	cmp.w	r5, #920	; 0x398
3000696c:	d85c      	bhi.n	30006a28 <BOOT_SOC_ClkSet+0x10c>
3000696e:	2001      	movs	r0, #1
30006970:	ea4f 2909 	mov.w	r9, r9, lsl #8
30006974:	f001 fb72 	bl	3000805c <CLK_SWITCH_XTAL>
30006978:	683b      	ldr	r3, [r7, #0]
3000697a:	4847      	ldr	r0, [pc, #284]	; (30006a98 <BOOT_SOC_ClkSet+0x17c>)
3000697c:	ea4f 3808 	mov.w	r8, r8, lsl #12
30006980:	f409 69e0 	and.w	r9, r9, #1792	; 0x700
30006984:	0524      	lsls	r4, r4, #20
30006986:	fb03 f000 	mul.w	r0, r3, r0
3000698a:	4b44      	ldr	r3, [pc, #272]	; (30006a9c <BOOT_SOC_ClkSet+0x180>)
3000698c:	4798      	blx	r3
3000698e:	4944      	ldr	r1, [pc, #272]	; (30006aa0 <BOOT_SOC_ClkSet+0x184>)
30006990:	68fb      	ldr	r3, [r7, #12]
30006992:	fa1f f888 	uxth.w	r8, r8
30006996:	f8d1 0228 	ldr.w	r0, [r1, #552]	; 0x228
3000699a:	f404 04e0 	and.w	r4, r4, #7340032	; 0x700000
3000699e:	3b01      	subs	r3, #1
300069a0:	4a40      	ldr	r2, [pc, #256]	; (30006aa4 <BOOT_SOC_ClkSet+0x188>)
300069a2:	f003 0307 	and.w	r3, r3, #7
300069a6:	4002      	ands	r2, r0
300069a8:	2000      	movs	r0, #0
300069aa:	4313      	orrs	r3, r2
300069ac:	ea49 0303 	orr.w	r3, r9, r3
300069b0:	ea48 0303 	orr.w	r3, r8, r3
300069b4:	4323      	orrs	r3, r4
300069b6:	4c3c      	ldr	r4, [pc, #240]	; (30006aa8 <BOOT_SOC_ClkSet+0x18c>)
300069b8:	f8c1 3228 	str.w	r3, [r1, #552]	; 0x228
300069bc:	f001 fb4e 	bl	3000805c <CLK_SWITCH_XTAL>
300069c0:	68a3      	ldr	r3, [r4, #8]
300069c2:	079a      	lsls	r2, r3, #30
300069c4:	d42b      	bmi.n	30006a1e <BOOT_SOC_ClkSet+0x102>
300069c6:	4b39      	ldr	r3, [pc, #228]	; (30006aac <BOOT_SOC_ClkSet+0x190>)
300069c8:	781b      	ldrb	r3, [r3, #0]
300069ca:	b303      	cbz	r3, 30006a0e <BOOT_SOC_ClkSet+0xf2>
300069cc:	693b      	ldr	r3, [r7, #16]
300069ce:	2b00      	cmp	r3, #0
300069d0:	db48      	blt.n	30006a64 <BOOT_SOC_ClkSet+0x148>
300069d2:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300069d6:	6913      	ldr	r3, [r2, #16]
300069d8:	f043 030c 	orr.w	r3, r3, #12
300069dc:	6113      	str	r3, [r2, #16]
300069de:	2200      	movs	r2, #0
300069e0:	4b2c      	ldr	r3, [pc, #176]	; (30006a94 <BOOT_SOC_ClkSet+0x178>)
300069e2:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
300069e6:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
300069ea:	693b      	ldr	r3, [r7, #16]
300069ec:	68a0      	ldr	r0, [r4, #8]
300069ee:	690a      	ldr	r2, [r1, #16]
300069f0:	3b01      	subs	r3, #1
300069f2:	f022 0203 	bic.w	r2, r2, #3
300069f6:	f003 0303 	and.w	r3, r3, #3
300069fa:	4313      	orrs	r3, r2
300069fc:	610b      	str	r3, [r1, #16]
300069fe:	0783      	lsls	r3, r0, #30
30006a00:	d505      	bpl.n	30006a0e <BOOT_SOC_ClkSet+0xf2>
30006a02:	4629      	mov	r1, r5
30006a04:	482a      	ldr	r0, [pc, #168]	; (30006ab0 <BOOT_SOC_ClkSet+0x194>)
30006a06:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30006a0a:	f005 bc0d 	b.w	3000c228 <__DiagPrintf_veneer>
30006a0e:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30006a12:	f025 4500 	bic.w	r5, r5, #2147483648	; 0x80000000
30006a16:	687b      	ldr	r3, [r7, #4]
30006a18:	fbb3 f5f5 	udiv	r5, r3, r5
30006a1c:	e787      	b.n	3000692e <BOOT_SOC_ClkSet+0x12>
30006a1e:	4631      	mov	r1, r6
30006a20:	4824      	ldr	r0, [pc, #144]	; (30006ab4 <BOOT_SOC_ClkSet+0x198>)
30006a22:	f005 fc01 	bl	3000c228 <__DiagPrintf_veneer>
30006a26:	e7ce      	b.n	300069c6 <BOOT_SOC_ClkSet+0xaa>
30006a28:	f240 6166 	movw	r1, #1638	; 0x666
30006a2c:	4822      	ldr	r0, [pc, #136]	; (30006ab8 <BOOT_SOC_ClkSet+0x19c>)
30006a2e:	f005 fbeb 	bl	3000c208 <__io_assert_failed_veneer>
30006a32:	e79c      	b.n	3000696e <BOOT_SOC_ClkSet+0x52>
30006a34:	f240 6165 	movw	r1, #1637	; 0x665
30006a38:	481f      	ldr	r0, [pc, #124]	; (30006ab8 <BOOT_SOC_ClkSet+0x19c>)
30006a3a:	f005 fbe5 	bl	3000c208 <__io_assert_failed_veneer>
30006a3e:	e793      	b.n	30006968 <BOOT_SOC_ClkSet+0x4c>
30006a40:	4b1e      	ldr	r3, [pc, #120]	; (30006abc <BOOT_SOC_ClkSet+0x1a0>)
30006a42:	4650      	mov	r0, sl
30006a44:	4798      	blx	r3
30006a46:	4b13      	ldr	r3, [pc, #76]	; (30006a94 <BOOT_SOC_ClkSet+0x178>)
30006a48:	f5b6 7fa7 	cmp.w	r6, #334	; 0x14e
30006a4c:	f883 a10c 	strb.w	sl, [r3, #268]	; 0x10c
30006a50:	d216      	bcs.n	30006a80 <BOOT_SOC_ClkSet+0x164>
30006a52:	f5b5 6f96 	cmp.w	r5, #1200	; 0x4b0
30006a56:	d98a      	bls.n	3000696e <BOOT_SOC_ClkSet+0x52>
30006a58:	f240 6162 	movw	r1, #1634	; 0x662
30006a5c:	4816      	ldr	r0, [pc, #88]	; (30006ab8 <BOOT_SOC_ClkSet+0x19c>)
30006a5e:	f005 fbd3 	bl	3000c208 <__io_assert_failed_veneer>
30006a62:	e784      	b.n	3000696e <BOOT_SOC_ClkSet+0x52>
30006a64:	687b      	ldr	r3, [r7, #4]
30006a66:	480c      	ldr	r0, [pc, #48]	; (30006a98 <BOOT_SOC_ClkSet+0x17c>)
30006a68:	fb03 f000 	mul.w	r0, r3, r0
30006a6c:	4b14      	ldr	r3, [pc, #80]	; (30006ac0 <BOOT_SOC_ClkSet+0x1a4>)
30006a6e:	4798      	blx	r3
30006a70:	4b14      	ldr	r3, [pc, #80]	; (30006ac4 <BOOT_SOC_ClkSet+0x1a8>)
30006a72:	2001      	movs	r0, #1
30006a74:	4798      	blx	r3
30006a76:	4b07      	ldr	r3, [pc, #28]	; (30006a94 <BOOT_SOC_ClkSet+0x178>)
30006a78:	2201      	movs	r2, #1
30006a7a:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30006a7e:	e7b2      	b.n	300069e6 <BOOT_SOC_ClkSet+0xca>
30006a80:	f240 6161 	movw	r1, #1633	; 0x661
30006a84:	480c      	ldr	r0, [pc, #48]	; (30006ab8 <BOOT_SOC_ClkSet+0x19c>)
30006a86:	f005 fbbf 	bl	3000c208 <__io_assert_failed_veneer>
30006a8a:	e7e2      	b.n	30006a52 <BOOT_SOC_ClkSet+0x136>
30006a8c:	3000e424 	.word	0x3000e424
30006a90:	51eb851f 	.word	0x51eb851f
30006a94:	23020000 	.word	0x23020000
30006a98:	000f4240 	.word	0x000f4240
30006a9c:	30009459 	.word	0x30009459
30006aa0:	42008000 	.word	0x42008000
30006aa4:	ff8f08f8 	.word	0xff8f08f8
30006aa8:	2001c00c 	.word	0x2001c00c
30006aac:	3000d80a 	.word	0x3000d80a
30006ab0:	3000ce9c 	.word	0x3000ce9c
30006ab4:	3000ce70 	.word	0x3000ce70
30006ab8:	3000d7c0 	.word	0x3000d7c0
30006abc:	3000bc05 	.word	0x3000bc05
30006ac0:	300094d9 	.word	0x300094d9
30006ac4:	30009519 	.word	0x30009519

30006ac8 <BOOT_Disable_AP>:
30006ac8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006acc:	4906      	ldr	r1, [pc, #24]	; (30006ae8 <BOOT_Disable_AP+0x20>)
30006ace:	685a      	ldr	r2, [r3, #4]
30006ad0:	f042 0202 	orr.w	r2, r2, #2
30006ad4:	605a      	str	r2, [r3, #4]
30006ad6:	685a      	ldr	r2, [r3, #4]
30006ad8:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006adc:	605a      	str	r2, [r3, #4]
30006ade:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006ae0:	f043 0303 	orr.w	r3, r3, #3
30006ae4:	678b      	str	r3, [r1, #120]	; 0x78
30006ae6:	4770      	bx	lr
30006ae8:	41000200 	.word	0x41000200

30006aec <BOOT_Enable_AP>:
30006aec:	b538      	push	{r3, r4, r5, lr}
30006aee:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30006af2:	2201      	movs	r2, #1
30006af4:	2104      	movs	r1, #4
30006af6:	4b2b      	ldr	r3, [pc, #172]	; (30006ba4 <BOOT_Enable_AP+0xb8>)
30006af8:	482b      	ldr	r0, [pc, #172]	; (30006ba8 <BOOT_Enable_AP+0xbc>)
30006afa:	4798      	blx	r3
30006afc:	6863      	ldr	r3, [r4, #4]
30006afe:	4d2b      	ldr	r5, [pc, #172]	; (30006bac <BOOT_Enable_AP+0xc0>)
30006b00:	2032      	movs	r0, #50	; 0x32
30006b02:	f043 0302 	orr.w	r3, r3, #2
30006b06:	6063      	str	r3, [r4, #4]
30006b08:	6863      	ldr	r3, [r4, #4]
30006b0a:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30006b0e:	6063      	str	r3, [r4, #4]
30006b10:	6823      	ldr	r3, [r4, #0]
30006b12:	f043 0302 	orr.w	r3, r3, #2
30006b16:	6023      	str	r3, [r4, #0]
30006b18:	47a8      	blx	r5
30006b1a:	6823      	ldr	r3, [r4, #0]
30006b1c:	2032      	movs	r0, #50	; 0x32
30006b1e:	f043 0304 	orr.w	r3, r3, #4
30006b22:	6023      	str	r3, [r4, #0]
30006b24:	6823      	ldr	r3, [r4, #0]
30006b26:	f043 0301 	orr.w	r3, r3, #1
30006b2a:	6023      	str	r3, [r4, #0]
30006b2c:	6823      	ldr	r3, [r4, #0]
30006b2e:	f043 0310 	orr.w	r3, r3, #16
30006b32:	6023      	str	r3, [r4, #0]
30006b34:	47a8      	blx	r5
30006b36:	6823      	ldr	r3, [r4, #0]
30006b38:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30006b3c:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30006b40:	6023      	str	r3, [r4, #0]
30006b42:	47a8      	blx	r5
30006b44:	6863      	ldr	r3, [r4, #4]
30006b46:	4a1a      	ldr	r2, [pc, #104]	; (30006bb0 <BOOT_Enable_AP+0xc4>)
30006b48:	f023 0310 	bic.w	r3, r3, #16
30006b4c:	6063      	str	r3, [r4, #4]
30006b4e:	6863      	ldr	r3, [r4, #4]
30006b50:	f023 0302 	bic.w	r3, r3, #2
30006b54:	6063      	str	r3, [r4, #4]
30006b56:	4790      	blx	r2
30006b58:	b178      	cbz	r0, 30006b7a <BOOT_Enable_AP+0x8e>
30006b5a:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006b5e:	4815      	ldr	r0, [pc, #84]	; (30006bb4 <BOOT_Enable_AP+0xc8>)
30006b60:	4a15      	ldr	r2, [pc, #84]	; (30006bb8 <BOOT_Enable_AP+0xcc>)
30006b62:	68d9      	ldr	r1, [r3, #12]
30006b64:	f041 0101 	orr.w	r1, r1, #1
30006b68:	60d9      	str	r1, [r3, #12]
30006b6a:	6899      	ldr	r1, [r3, #8]
30006b6c:	f041 0101 	orr.w	r1, r1, #1
30006b70:	6099      	str	r1, [r3, #8]
30006b72:	6843      	ldr	r3, [r0, #4]
30006b74:	431a      	orrs	r2, r3
30006b76:	6042      	str	r2, [r0, #4]
30006b78:	bd38      	pop	{r3, r4, r5, pc}
30006b7a:	4604      	mov	r4, r0
30006b7c:	4d0f      	ldr	r5, [pc, #60]	; (30006bbc <BOOT_Enable_AP+0xd0>)
30006b7e:	4601      	mov	r1, r0
30006b80:	2041      	movs	r0, #65	; 0x41
30006b82:	47a8      	blx	r5
30006b84:	4621      	mov	r1, r4
30006b86:	2042      	movs	r0, #66	; 0x42
30006b88:	47a8      	blx	r5
30006b8a:	4621      	mov	r1, r4
30006b8c:	2043      	movs	r0, #67	; 0x43
30006b8e:	47a8      	blx	r5
30006b90:	4621      	mov	r1, r4
30006b92:	2044      	movs	r0, #68	; 0x44
30006b94:	47a8      	blx	r5
30006b96:	4621      	mov	r1, r4
30006b98:	2045      	movs	r0, #69	; 0x45
30006b9a:	47a8      	blx	r5
30006b9c:	4621      	mov	r1, r4
30006b9e:	2046      	movs	r0, #70	; 0x46
30006ba0:	47a8      	blx	r5
30006ba2:	e7da      	b.n	30006b5a <BOOT_Enable_AP+0x6e>
30006ba4:	0000aab9 	.word	0x0000aab9
30006ba8:	4200c000 	.word	0x4200c000
30006bac:	00009b2d 	.word	0x00009b2d
30006bb0:	3000a589 	.word	0x3000a589
30006bb4:	41000200 	.word	0x41000200
30006bb8:	01001111 	.word	0x01001111
30006bbc:	0000b20d 	.word	0x0000b20d

30006bc0 <BOOT_RAM_KeyDeriveFunc>:
30006bc0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006bc4:	ed2d 8b04 	vpush	{d8-d9}
30006bc8:	b0cb      	sub	sp, #300	; 0x12c
30006bca:	4c6a      	ldr	r4, [pc, #424]	; (30006d74 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30006bcc:	4680      	mov	r8, r0
30006bce:	4689      	mov	r9, r1
30006bd0:	4869      	ldr	r0, [pc, #420]	; (30006d78 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30006bd2:	ee08 2a90 	vmov	s17, r2
30006bd6:	4969      	ldr	r1, [pc, #420]	; (30006d7c <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006bd8:	2201      	movs	r2, #1
30006bda:	ee09 3a10 	vmov	s18, r3
30006bde:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30006be2:	47a0      	blx	r4
30006be4:	2201      	movs	r2, #1
30006be6:	4966      	ldr	r1, [pc, #408]	; (30006d80 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30006be8:	ae02      	add	r6, sp, #8
30006bea:	4864      	ldr	r0, [pc, #400]	; (30006d7c <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006bec:	47a0      	blx	r4
30006bee:	4b65      	ldr	r3, [pc, #404]	; (30006d84 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30006bf0:	2000      	movs	r0, #0
30006bf2:	4798      	blx	r3
30006bf4:	22e0      	movs	r2, #224	; 0xe0
30006bf6:	2100      	movs	r1, #0
30006bf8:	a812      	add	r0, sp, #72	; 0x48
30006bfa:	f005 faf5 	bl	3000c1e8 <____wrap_memset_veneer>
30006bfe:	2300      	movs	r3, #0
30006c00:	f8ad 3004 	strh.w	r3, [sp, #4]
30006c04:	f88d 3006 	strb.w	r3, [sp, #6]
30006c08:	2301      	movs	r3, #1
30006c0a:	f88d 3007 	strb.w	r3, [sp, #7]
30006c0e:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c10:	2b00      	cmp	r3, #0
30006c12:	f000 80ab 	beq.w	30006d6c <BOOT_RAM_KeyDeriveFunc+0x1ac>
30006c16:	ab0a      	add	r3, sp, #40	; 0x28
30006c18:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30006d98 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30006c1c:	4f5a      	ldr	r7, [pc, #360]	; (30006d88 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30006c1e:	ee08 3a10 	vmov	s16, r3
30006c22:	ab12      	add	r3, sp, #72	; 0x48
30006c24:	464a      	mov	r2, r9
30006c26:	4641      	mov	r1, r8
30006c28:	2020      	movs	r0, #32
30006c2a:	47d8      	blx	fp
30006c2c:	aa12      	add	r2, sp, #72	; 0x48
30006c2e:	ee19 1a10 	vmov	r1, s18
30006c32:	ee18 0a90 	vmov	r0, s17
30006c36:	47b8      	blx	r7
30006c38:	ee18 5a10 	vmov	r5, s16
30006c3c:	aa12      	add	r2, sp, #72	; 0x48
30006c3e:	2104      	movs	r1, #4
30006c40:	a801      	add	r0, sp, #4
30006c42:	4634      	mov	r4, r6
30006c44:	47b8      	blx	r7
30006c46:	a912      	add	r1, sp, #72	; 0x48
30006c48:	4b50      	ldr	r3, [pc, #320]	; (30006d8c <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c4a:	ee18 0a10 	vmov	r0, s16
30006c4e:	4798      	blx	r3
30006c50:	f1ba 0f01 	cmp.w	sl, #1
30006c54:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30006c56:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30006c58:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30006c5c:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006c60:	d91f      	bls.n	30006ca2 <BOOT_RAM_KeyDeriveFunc+0xe2>
30006c62:	2501      	movs	r5, #1
30006c64:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30006c68:	ab12      	add	r3, sp, #72	; 0x48
30006c6a:	4641      	mov	r1, r8
30006c6c:	464a      	mov	r2, r9
30006c6e:	2020      	movs	r0, #32
30006c70:	47d8      	blx	fp
30006c72:	aa12      	add	r2, sp, #72	; 0x48
30006c74:	2120      	movs	r1, #32
30006c76:	4630      	mov	r0, r6
30006c78:	47b8      	blx	r7
30006c7a:	a912      	add	r1, sp, #72	; 0x48
30006c7c:	4b43      	ldr	r3, [pc, #268]	; (30006d8c <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c7e:	4630      	mov	r0, r6
30006c80:	4798      	blx	r3
30006c82:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30006c86:	f10d 0107 	add.w	r1, sp, #7
30006c8a:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30006c8e:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30006c92:	42a3      	cmp	r3, r4
30006c94:	ea82 0200 	eor.w	r2, r2, r0
30006c98:	701a      	strb	r2, [r3, #0]
30006c9a:	d1f6      	bne.n	30006c8a <BOOT_RAM_KeyDeriveFunc+0xca>
30006c9c:	3501      	adds	r5, #1
30006c9e:	45aa      	cmp	sl, r5
30006ca0:	d1e2      	bne.n	30006c68 <BOOT_RAM_KeyDeriveFunc+0xa8>
30006ca2:	9a59      	ldr	r2, [sp, #356]	; 0x164
30006ca4:	ee18 1a10 	vmov	r1, s16
30006ca8:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006caa:	2a20      	cmp	r2, #32
30006cac:	985a      	ldr	r0, [sp, #360]	; 0x168
30006cae:	bf28      	it	cs
30006cb0:	2220      	movcs	r2, #32
30006cb2:	1a9b      	subs	r3, r3, r2
30006cb4:	9359      	str	r3, [sp, #356]	; 0x164
30006cb6:	4603      	mov	r3, r0
30006cb8:	4413      	add	r3, r2
30006cba:	935a      	str	r3, [sp, #360]	; 0x168
30006cbc:	f005 fa9c 	bl	3000c1f8 <____wrap_memcpy_veneer>
30006cc0:	f10d 0207 	add.w	r2, sp, #7
30006cc4:	2104      	movs	r1, #4
30006cc6:	7813      	ldrb	r3, [r2, #0]
30006cc8:	3901      	subs	r1, #1
30006cca:	3301      	adds	r3, #1
30006ccc:	b2db      	uxtb	r3, r3
30006cce:	f802 3901 	strb.w	r3, [r2], #-1
30006cd2:	b90b      	cbnz	r3, 30006cd8 <BOOT_RAM_KeyDeriveFunc+0x118>
30006cd4:	2900      	cmp	r1, #0
30006cd6:	d1f6      	bne.n	30006cc6 <BOOT_RAM_KeyDeriveFunc+0x106>
30006cd8:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006cda:	2b00      	cmp	r3, #0
30006cdc:	d1a1      	bne.n	30006c22 <BOOT_RAM_KeyDeriveFunc+0x62>
30006cde:	2220      	movs	r2, #32
30006ce0:	2100      	movs	r1, #0
30006ce2:	4c2b      	ldr	r4, [pc, #172]	; (30006d90 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30006ce4:	4630      	mov	r0, r6
30006ce6:	47a0      	blx	r4
30006ce8:	2220      	movs	r2, #32
30006cea:	2100      	movs	r1, #0
30006cec:	ee18 0a10 	vmov	r0, s16
30006cf0:	47a0      	blx	r4
30006cf2:	4b28      	ldr	r3, [pc, #160]	; (30006d94 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cf4:	695b      	ldr	r3, [r3, #20]
30006cf6:	03da      	lsls	r2, r3, #15
30006cf8:	d512      	bpl.n	30006d20 <BOOT_RAM_KeyDeriveFunc+0x160>
30006cfa:	f016 021f 	ands.w	r2, r6, #31
30006cfe:	4633      	mov	r3, r6
30006d00:	d130      	bne.n	30006d64 <BOOT_RAM_KeyDeriveFunc+0x1a4>
30006d02:	2220      	movs	r2, #32
30006d04:	f3bf 8f4f 	dsb	sy
30006d08:	4822      	ldr	r0, [pc, #136]	; (30006d94 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006d0a:	441a      	add	r2, r3
30006d0c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006d10:	3320      	adds	r3, #32
30006d12:	1ad1      	subs	r1, r2, r3
30006d14:	2900      	cmp	r1, #0
30006d16:	dcf9      	bgt.n	30006d0c <BOOT_RAM_KeyDeriveFunc+0x14c>
30006d18:	f3bf 8f4f 	dsb	sy
30006d1c:	f3bf 8f6f 	isb	sy
30006d20:	4b1c      	ldr	r3, [pc, #112]	; (30006d94 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006d22:	695b      	ldr	r3, [r3, #20]
30006d24:	03db      	lsls	r3, r3, #15
30006d26:	d513      	bpl.n	30006d50 <BOOT_RAM_KeyDeriveFunc+0x190>
30006d28:	ee18 3a10 	vmov	r3, s16
30006d2c:	f013 021f 	ands.w	r2, r3, #31
30006d30:	d114      	bne.n	30006d5c <BOOT_RAM_KeyDeriveFunc+0x19c>
30006d32:	2220      	movs	r2, #32
30006d34:	f3bf 8f4f 	dsb	sy
30006d38:	4816      	ldr	r0, [pc, #88]	; (30006d94 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006d3a:	441a      	add	r2, r3
30006d3c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006d40:	3320      	adds	r3, #32
30006d42:	1ad1      	subs	r1, r2, r3
30006d44:	2900      	cmp	r1, #0
30006d46:	dcf9      	bgt.n	30006d3c <BOOT_RAM_KeyDeriveFunc+0x17c>
30006d48:	f3bf 8f4f 	dsb	sy
30006d4c:	f3bf 8f6f 	isb	sy
30006d50:	2000      	movs	r0, #0
30006d52:	b04b      	add	sp, #300	; 0x12c
30006d54:	ecbd 8b04 	vpop	{d8-d9}
30006d58:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006d5c:	f023 031f 	bic.w	r3, r3, #31
30006d60:	3220      	adds	r2, #32
30006d62:	e7e7      	b.n	30006d34 <BOOT_RAM_KeyDeriveFunc+0x174>
30006d64:	f026 031f 	bic.w	r3, r6, #31
30006d68:	3220      	adds	r2, #32
30006d6a:	e7cb      	b.n	30006d04 <BOOT_RAM_KeyDeriveFunc+0x144>
30006d6c:	ab0a      	add	r3, sp, #40	; 0x28
30006d6e:	ee08 3a10 	vmov	s16, r3
30006d72:	e7b4      	b.n	30006cde <BOOT_RAM_KeyDeriveFunc+0x11e>
30006d74:	0000b479 	.word	0x0000b479
30006d78:	40000008 	.word	0x40000008
30006d7c:	40000004 	.word	0x40000004
30006d80:	40000002 	.word	0x40000002
30006d84:	00002451 	.word	0x00002451
30006d88:	00005b4d 	.word	0x00005b4d
30006d8c:	00005bb9 	.word	0x00005bb9
30006d90:	00012be5 	.word	0x00012be5
30006d94:	e000ed00 	.word	0xe000ed00
30006d98:	30009eb1 	.word	0x30009eb1

30006d9c <password_hash_check>:
30006d9c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006da0:	b08f      	sub	sp, #60	; 0x3c
30006da2:	4690      	mov	r8, r2
30006da4:	461d      	mov	r5, r3
30006da6:	4606      	mov	r6, r0
30006da8:	af04      	add	r7, sp, #16
30006daa:	3201      	adds	r2, #1
30006dac:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006dae:	6079      	str	r1, [r7, #4]
30006db0:	2100      	movs	r1, #0
30006db2:	f103 0008 	add.w	r0, r3, #8
30006db6:	f108 0308 	add.w	r3, r8, #8
30006dba:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
30006dbe:	f023 0307 	bic.w	r3, r3, #7
30006dc2:	f020 0007 	bic.w	r0, r0, #7
30006dc6:	ebad 0d03 	sub.w	sp, sp, r3
30006dca:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006dcc:	f10d 0910 	add.w	r9, sp, #16
30006dd0:	3301      	adds	r3, #1
30006dd2:	ebad 0d00 	sub.w	sp, sp, r0
30006dd6:	603b      	str	r3, [r7, #0]
30006dd8:	4648      	mov	r0, r9
30006dda:	4b3f      	ldr	r3, [pc, #252]	; (30006ed8 <password_hash_check+0x13c>)
30006ddc:	f10d 0a10 	add.w	sl, sp, #16
30006de0:	4798      	blx	r3
30006de2:	683b      	ldr	r3, [r7, #0]
30006de4:	2100      	movs	r1, #0
30006de6:	4650      	mov	r0, sl
30006de8:	461a      	mov	r2, r3
30006dea:	4b3b      	ldr	r3, [pc, #236]	; (30006ed8 <password_hash_check+0x13c>)
30006dec:	4798      	blx	r3
30006dee:	f1b8 0001 	subs.w	r0, r8, #1
30006df2:	d01d      	beq.n	30006e30 <password_hash_check+0x94>
30006df4:	2200      	movs	r2, #0
30006df6:	f109 33ff 	add.w	r3, r9, #4294967295
30006dfa:	4611      	mov	r1, r2
30006dfc:	e00b      	b.n	30006e16 <password_hash_check+0x7a>
30006dfe:	f108 38ff 	add.w	r8, r8, #4294967295
30006e02:	2100      	movs	r1, #0
30006e04:	1c9c      	adds	r4, r3, #2
30006e06:	4282      	cmp	r2, r0
30006e08:	7059      	strb	r1, [r3, #1]
30006e0a:	f103 0301 	add.w	r3, r3, #1
30006e0e:	4611      	mov	r1, r2
30006e10:	eba4 0409 	sub.w	r4, r4, r9
30006e14:	d20b      	bcs.n	30006e2e <password_hash_check+0x92>
30006e16:	1874      	adds	r4, r6, r1
30006e18:	f102 0c01 	add.w	ip, r2, #1
30006e1c:	5c71      	ldrb	r1, [r6, r1]
30006e1e:	3202      	adds	r2, #2
30006e20:	7864      	ldrb	r4, [r4, #1]
30006e22:	2c30      	cmp	r4, #48	; 0x30
30006e24:	d101      	bne.n	30006e2a <password_hash_check+0x8e>
30006e26:	295c      	cmp	r1, #92	; 0x5c
30006e28:	d0e9      	beq.n	30006dfe <password_hash_check+0x62>
30006e2a:	4662      	mov	r2, ip
30006e2c:	e7ea      	b.n	30006e04 <password_hash_check+0x68>
30006e2e:	4406      	add	r6, r0
30006e30:	7833      	ldrb	r3, [r6, #0]
30006e32:	f809 3004 	strb.w	r3, [r9, r4]
30006e36:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006e38:	1e5c      	subs	r4, r3, #1
30006e3a:	d04a      	beq.n	30006ed2 <password_hash_check+0x136>
30006e3c:	2200      	movs	r2, #0
30006e3e:	f10a 33ff 	add.w	r3, sl, #4294967295
30006e42:	4611      	mov	r1, r2
30006e44:	e00b      	b.n	30006e5e <password_hash_check+0xc2>
30006e46:	f10b 3bff 	add.w	fp, fp, #4294967295
30006e4a:	2100      	movs	r1, #0
30006e4c:	1c98      	adds	r0, r3, #2
30006e4e:	42a2      	cmp	r2, r4
30006e50:	7059      	strb	r1, [r3, #1]
30006e52:	f103 0301 	add.w	r3, r3, #1
30006e56:	4611      	mov	r1, r2
30006e58:	eba0 000a 	sub.w	r0, r0, sl
30006e5c:	d20a      	bcs.n	30006e74 <password_hash_check+0xd8>
30006e5e:	1868      	adds	r0, r5, r1
30006e60:	1c56      	adds	r6, r2, #1
30006e62:	5c69      	ldrb	r1, [r5, r1]
30006e64:	3202      	adds	r2, #2
30006e66:	7840      	ldrb	r0, [r0, #1]
30006e68:	2830      	cmp	r0, #48	; 0x30
30006e6a:	d101      	bne.n	30006e70 <password_hash_check+0xd4>
30006e6c:	295c      	cmp	r1, #92	; 0x5c
30006e6e:	d0ea      	beq.n	30006e46 <password_hash_check+0xaa>
30006e70:	4632      	mov	r2, r6
30006e72:	e7eb      	b.n	30006e4c <password_hash_check+0xb0>
30006e74:	4425      	add	r5, r4
30006e76:	782b      	ldrb	r3, [r5, #0]
30006e78:	f107 0408 	add.w	r4, r7, #8
30006e7c:	2520      	movs	r5, #32
30006e7e:	4641      	mov	r1, r8
30006e80:	f80a 3000 	strb.w	r3, [sl, r0]
30006e84:	4652      	mov	r2, sl
30006e86:	687b      	ldr	r3, [r7, #4]
30006e88:	4648      	mov	r0, r9
30006e8a:	9402      	str	r4, [sp, #8]
30006e8c:	9300      	str	r3, [sp, #0]
30006e8e:	465b      	mov	r3, fp
30006e90:	9501      	str	r5, [sp, #4]
30006e92:	f7ff fe95 	bl	30006bc0 <BOOT_RAM_KeyDeriveFunc>
30006e96:	4620      	mov	r0, r4
30006e98:	462a      	mov	r2, r5
30006e9a:	4910      	ldr	r1, [pc, #64]	; (30006edc <password_hash_check+0x140>)
30006e9c:	f005 f9d4 	bl	3000c248 <____wrap_memcmp_veneer>
30006ea0:	4604      	mov	r4, r0
30006ea2:	b968      	cbnz	r0, 30006ec0 <password_hash_check+0x124>
30006ea4:	480e      	ldr	r0, [pc, #56]	; (30006ee0 <password_hash_check+0x144>)
30006ea6:	f005 f9bf 	bl	3000c228 <__DiagPrintf_veneer>
30006eaa:	462a      	mov	r2, r5
30006eac:	4621      	mov	r1, r4
30006eae:	f107 0008 	add.w	r0, r7, #8
30006eb2:	4b09      	ldr	r3, [pc, #36]	; (30006ed8 <password_hash_check+0x13c>)
30006eb4:	4798      	blx	r3
30006eb6:	4620      	mov	r0, r4
30006eb8:	372c      	adds	r7, #44	; 0x2c
30006eba:	46bd      	mov	sp, r7
30006ebc:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ec0:	2401      	movs	r4, #1
30006ec2:	4808      	ldr	r0, [pc, #32]	; (30006ee4 <password_hash_check+0x148>)
30006ec4:	f005 f9b0 	bl	3000c228 <__DiagPrintf_veneer>
30006ec8:	372c      	adds	r7, #44	; 0x2c
30006eca:	4620      	mov	r0, r4
30006ecc:	46bd      	mov	sp, r7
30006ece:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ed2:	4620      	mov	r0, r4
30006ed4:	e7cf      	b.n	30006e76 <password_hash_check+0xda>
30006ed6:	bf00      	nop
30006ed8:	00012be5 	.word	0x00012be5
30006edc:	3000e1a4 	.word	0x3000e1a4
30006ee0:	3000cee0 	.word	0x3000cee0
30006ee4:	3000cec8 	.word	0x3000cec8

30006ee8 <BOOT_Image1>:
30006ee8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006eec:	4bb8      	ldr	r3, [pc, #736]	; (300071d0 <BOOT_Image1+0x2e8>)
30006eee:	b08b      	sub	sp, #44	; 0x2c
30006ef0:	4798      	blx	r3
30006ef2:	2800      	cmp	r0, #0
30006ef4:	f040 830c 	bne.w	30007510 <BOOT_Image1+0x628>
30006ef8:	48b6      	ldr	r0, [pc, #728]	; (300071d4 <BOOT_Image1+0x2ec>)
30006efa:	f005 f995 	bl	3000c228 <__DiagPrintf_veneer>
30006efe:	49b6      	ldr	r1, [pc, #728]	; (300071d8 <BOOT_Image1+0x2f0>)
30006f00:	48b6      	ldr	r0, [pc, #728]	; (300071dc <BOOT_Image1+0x2f4>)
30006f02:	f005 f991 	bl	3000c228 <__DiagPrintf_veneer>
30006f06:	48b6      	ldr	r0, [pc, #728]	; (300071e0 <BOOT_Image1+0x2f8>)
30006f08:	4ab6      	ldr	r2, [pc, #728]	; (300071e4 <BOOT_Image1+0x2fc>)
30006f0a:	2100      	movs	r1, #0
30006f0c:	4bb6      	ldr	r3, [pc, #728]	; (300071e8 <BOOT_Image1+0x300>)
30006f0e:	1a12      	subs	r2, r2, r0
30006f10:	4798      	blx	r3
30006f12:	4ab6      	ldr	r2, [pc, #728]	; (300071ec <BOOT_Image1+0x304>)
30006f14:	49b6      	ldr	r1, [pc, #728]	; (300071f0 <BOOT_Image1+0x308>)
30006f16:	8c13      	ldrh	r3, [r2, #32]
30006f18:	b29b      	uxth	r3, r3
30006f1a:	8413      	strh	r3, [r2, #32]
30006f1c:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006f20:	f002 feea 	bl	30009cf8 <BOOT_Reason>
30006f24:	2800      	cmp	r0, #0
30006f26:	f000 8331 	beq.w	3000758c <BOOT_Image1+0x6a4>
30006f2a:	4eb2      	ldr	r6, [pc, #712]	; (300071f4 <BOOT_Image1+0x30c>)
30006f2c:	4bb2      	ldr	r3, [pc, #712]	; (300071f8 <BOOT_Image1+0x310>)
30006f2e:	4798      	blx	r3
30006f30:	7834      	ldrb	r4, [r6, #0]
30006f32:	2c00      	cmp	r4, #0
30006f34:	f000 82f0 	beq.w	30007518 <BOOT_Image1+0x630>
30006f38:	2000      	movs	r0, #0
30006f3a:	4bb0      	ldr	r3, [pc, #704]	; (300071fc <BOOT_Image1+0x314>)
30006f3c:	4798      	blx	r3
30006f3e:	4bb0      	ldr	r3, [pc, #704]	; (30007200 <BOOT_Image1+0x318>)
30006f40:	2001      	movs	r0, #1
30006f42:	4798      	blx	r3
30006f44:	4caf      	ldr	r4, [pc, #700]	; (30007204 <BOOT_Image1+0x31c>)
30006f46:	f7ff fce9 	bl	3000691c <BOOT_SOC_ClkSet>
30006f4a:	f7fe fca5 	bl	30005898 <BOOT_GRstConfig>
30006f4e:	47a0      	blx	r4
30006f50:	2802      	cmp	r0, #2
30006f52:	f000 82eb 	beq.w	3000752c <BOOT_Image1+0x644>
30006f56:	4bac      	ldr	r3, [pc, #688]	; (30007208 <BOOT_Image1+0x320>)
30006f58:	2501      	movs	r5, #1
30006f5a:	49ac      	ldr	r1, [pc, #688]	; (3000720c <BOOT_Image1+0x324>)
30006f5c:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30006f60:	f883 510e 	strb.w	r5, [r3, #270]	; 0x10e
30006f64:	462a      	mov	r2, r5
30006f66:	4faa      	ldr	r7, [pc, #680]	; (30007210 <BOOT_Image1+0x328>)
30006f68:	47b8      	blx	r7
30006f6a:	462a      	mov	r2, r5
30006f6c:	49a9      	ldr	r1, [pc, #676]	; (30007214 <BOOT_Image1+0x32c>)
30006f6e:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30006f72:	47b8      	blx	r7
30006f74:	4a9d      	ldr	r2, [pc, #628]	; (300071ec <BOOT_Image1+0x304>)
30006f76:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30006f7a:	f043 0303 	orr.w	r3, r3, #3
30006f7e:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30006f82:	4ba5      	ldr	r3, [pc, #660]	; (30007218 <BOOT_Image1+0x330>)
30006f84:	4798      	blx	r3
30006f86:	b930      	cbnz	r0, 30006f96 <BOOT_Image1+0xae>
30006f88:	4a98      	ldr	r2, [pc, #608]	; (300071ec <BOOT_Image1+0x304>)
30006f8a:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30006f8e:	f023 0303 	bic.w	r3, r3, #3
30006f92:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30006f96:	4ba1      	ldr	r3, [pc, #644]	; (3000721c <BOOT_Image1+0x334>)
30006f98:	781b      	ldrb	r3, [r3, #0]
30006f9a:	2b00      	cmp	r3, #0
30006f9c:	f040 8346 	bne.w	3000762c <BOOT_Image1+0x744>
30006fa0:	4d9f      	ldr	r5, [pc, #636]	; (30007220 <BOOT_Image1+0x338>)
30006fa2:	2102      	movs	r1, #2
30006fa4:	2201      	movs	r2, #1
30006fa6:	489f      	ldr	r0, [pc, #636]	; (30007224 <BOOT_Image1+0x33c>)
30006fa8:	47a8      	blx	r5
30006faa:	4d9f      	ldr	r5, [pc, #636]	; (30007228 <BOOT_Image1+0x340>)
30006fac:	68ab      	ldr	r3, [r5, #8]
30006fae:	0799      	lsls	r1, r3, #30
30006fb0:	f100 8336 	bmi.w	30007620 <BOOT_Image1+0x738>
30006fb4:	499d      	ldr	r1, [pc, #628]	; (3000722c <BOOT_Image1+0x344>)
30006fb6:	e841 f100 	tt	r1, r1
30006fba:	68ab      	ldr	r3, [r5, #8]
30006fbc:	f3c1 5180 	ubfx	r1, r1, #22, #1
30006fc0:	079a      	lsls	r2, r3, #30
30006fc2:	f100 82ea 	bmi.w	3000759a <BOOT_Image1+0x6b2>
30006fc6:	4b9a      	ldr	r3, [pc, #616]	; (30007230 <BOOT_Image1+0x348>)
30006fc8:	4f9a      	ldr	r7, [pc, #616]	; (30007234 <BOOT_Image1+0x34c>)
30006fca:	681b      	ldr	r3, [r3, #0]
30006fcc:	4798      	blx	r3
30006fce:	f7fe fa55 	bl	3000547c <BOOT_RccConfig>
30006fd2:	f7fe fabf 	bl	30005554 <BOOT_CACHEWRR_Set>
30006fd6:	4b98      	ldr	r3, [pc, #608]	; (30007238 <BOOT_Image1+0x350>)
30006fd8:	6818      	ldr	r0, [r3, #0]
30006fda:	f7fe fae9 	bl	300055b0 <BOOT_TCMSet>
30006fde:	4b97      	ldr	r3, [pc, #604]	; (3000723c <BOOT_Image1+0x354>)
30006fe0:	695b      	ldr	r3, [r3, #20]
30006fe2:	03db      	lsls	r3, r3, #15
30006fe4:	d513      	bpl.n	3000700e <BOOT_Image1+0x126>
30006fe6:	f017 011f 	ands.w	r1, r7, #31
30006fea:	f040 8262 	bne.w	300074b2 <BOOT_Image1+0x5ca>
30006fee:	463b      	mov	r3, r7
30006ff0:	2174      	movs	r1, #116	; 0x74
30006ff2:	f3bf 8f4f 	dsb	sy
30006ff6:	4891      	ldr	r0, [pc, #580]	; (3000723c <BOOT_Image1+0x354>)
30006ff8:	4419      	add	r1, r3
30006ffa:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30006ffe:	3320      	adds	r3, #32
30007000:	1aca      	subs	r2, r1, r3
30007002:	2a00      	cmp	r2, #0
30007004:	dcf9      	bgt.n	30006ffa <BOOT_Image1+0x112>
30007006:	f3bf 8f4f 	dsb	sy
3000700a:	f3bf 8f6f 	isb	sy
3000700e:	4b77      	ldr	r3, [pc, #476]	; (300071ec <BOOT_Image1+0x304>)
30007010:	2200      	movs	r2, #0
30007012:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30007016:	4b7c      	ldr	r3, [pc, #496]	; (30007208 <BOOT_Image1+0x320>)
30007018:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000701c:	f7fc fbe0 	bl	300037e0 <flash_highspeed_setup>
30007020:	f001 fe26 	bl	30008c70 <flash_layout_init>
30007024:	47a0      	blx	r4
30007026:	2802      	cmp	r0, #2
30007028:	f000 8292 	beq.w	30007550 <BOOT_Image1+0x668>
3000702c:	7832      	ldrb	r2, [r6, #0]
3000702e:	68ab      	ldr	r3, [r5, #8]
30007030:	b162      	cbz	r2, 3000704c <BOOT_Image1+0x164>
30007032:	4c83      	ldr	r4, [pc, #524]	; (30007240 <BOOT_Image1+0x358>)
30007034:	4e83      	ldr	r6, [pc, #524]	; (30007244 <BOOT_Image1+0x35c>)
30007036:	f013 0f02 	tst.w	r3, #2
3000703a:	4630      	mov	r0, r6
3000703c:	d001      	beq.n	30007042 <BOOT_Image1+0x15a>
3000703e:	f005 f8f3 	bl	3000c228 <__DiagPrintf_veneer>
30007042:	f241 3088 	movw	r0, #5000	; 0x1388
30007046:	47a0      	blx	r4
30007048:	68ab      	ldr	r3, [r5, #8]
3000704a:	e7f4      	b.n	30007036 <BOOT_Image1+0x14e>
3000704c:	079a      	lsls	r2, r3, #30
3000704e:	f100 8374 	bmi.w	3000773a <BOOT_Image1+0x852>
30007052:	f7fe fc5d 	bl	30005910 <BOOT_DDR_Init>
30007056:	4a65      	ldr	r2, [pc, #404]	; (300071ec <BOOT_Image1+0x304>)
30007058:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
3000705c:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30007060:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30007064:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30007068:	2400      	movs	r4, #0
3000706a:	4960      	ldr	r1, [pc, #384]	; (300071ec <BOOT_Image1+0x304>)
3000706c:	2307      	movs	r3, #7
3000706e:	f10d 021d 	add.w	r2, sp, #29
30007072:	f8c1 42d0 	str.w	r4, [r1, #720]	; 0x2d0
30007076:	4620      	mov	r0, r4
30007078:	f8c1 42d4 	str.w	r4, [r1, #724]	; 0x2d4
3000707c:	f8c1 42d8 	str.w	r4, [r1, #728]	; 0x2d8
30007080:	f8c1 40d0 	str.w	r4, [r1, #208]	; 0xd0
30007084:	f44f 7159 	mov.w	r1, #868	; 0x364
30007088:	f002 f9ce 	bl	30009428 <EFUSERead8>
3000708c:	f10d 011f 	add.w	r1, sp, #31
30007090:	f44f 705a 	mov.w	r0, #872	; 0x368
30007094:	f88d 401e 	strb.w	r4, [sp, #30]
30007098:	f88d 401f 	strb.w	r4, [sp, #31]
3000709c:	f002 f8e4 	bl	30009268 <OTP_Read8>
300070a0:	f10d 011e 	add.w	r1, sp, #30
300070a4:	f240 3069 	movw	r0, #873	; 0x369
300070a8:	f002 f8de 	bl	30009268 <OTP_Read8>
300070ac:	f89d 301d 	ldrb.w	r3, [sp, #29]
300070b0:	f013 0f1e 	tst.w	r3, #30
300070b4:	d10a      	bne.n	300070cc <BOOT_Image1+0x1e4>
300070b6:	f89d 201e 	ldrb.w	r2, [sp, #30]
300070ba:	f89d 301f 	ldrb.w	r3, [sp, #31]
300070be:	f002 0208 	and.w	r2, r2, #8
300070c2:	f003 0301 	and.w	r3, r3, #1
300070c6:	4313      	orrs	r3, r2
300070c8:	f040 81f7 	bne.w	300074ba <BOOT_Image1+0x5d2>
300070cc:	4b5e      	ldr	r3, [pc, #376]	; (30007248 <BOOT_Image1+0x360>)
300070ce:	781b      	ldrb	r3, [r3, #0]
300070d0:	2b00      	cmp	r3, #0
300070d2:	f040 8213 	bne.w	300074fc <BOOT_Image1+0x614>
300070d6:	682b      	ldr	r3, [r5, #0]
300070d8:	f043 0302 	orr.w	r3, r3, #2
300070dc:	602b      	str	r3, [r5, #0]
300070de:	4b5b      	ldr	r3, [pc, #364]	; (3000724c <BOOT_Image1+0x364>)
300070e0:	4798      	blx	r3
300070e2:	2801      	cmp	r0, #1
300070e4:	f04f 0100 	mov.w	r1, #0
300070e8:	f04f 0002 	mov.w	r0, #2
300070ec:	bf0c      	ite	eq
300070ee:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
300070f2:	4a57      	ldrne	r2, [pc, #348]	; (30007250 <BOOT_Image1+0x368>)
300070f4:	f000 fdbc 	bl	30007c70 <ymodem_uart_port_init>
300070f8:	4b54      	ldr	r3, [pc, #336]	; (3000724c <BOOT_Image1+0x364>)
300070fa:	4798      	blx	r3
300070fc:	2800      	cmp	r0, #0
300070fe:	f000 8240 	beq.w	30007582 <BOOT_Image1+0x69a>
30007102:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30007106:	4853      	ldr	r0, [pc, #332]	; (30007254 <BOOT_Image1+0x36c>)
30007108:	f005 f88e 	bl	3000c228 <__DiagPrintf_veneer>
3000710c:	4852      	ldr	r0, [pc, #328]	; (30007258 <BOOT_Image1+0x370>)
3000710e:	2664      	movs	r6, #100	; 0x64
30007110:	f005 f84a 	bl	3000c1a8 <____wrap_strlen_veneer>
30007114:	4601      	mov	r1, r0
30007116:	4850      	ldr	r0, [pc, #320]	; (30007258 <BOOT_Image1+0x370>)
30007118:	4c49      	ldr	r4, [pc, #292]	; (30007240 <BOOT_Image1+0x358>)
3000711a:	f000 fe03 	bl	30007d24 <ymodem_uart_putdata>
3000711e:	e002      	b.n	30007126 <BOOT_Image1+0x23e>
30007120:	3e01      	subs	r6, #1
30007122:	f000 830e 	beq.w	30007742 <BOOT_Image1+0x85a>
30007126:	2001      	movs	r0, #1
30007128:	47a0      	blx	r4
3000712a:	f000 fde1 	bl	30007cf0 <ymodem_uart_readable>
3000712e:	2800      	cmp	r0, #0
30007130:	d0f6      	beq.n	30007120 <BOOT_Image1+0x238>
30007132:	f000 fddd 	bl	30007cf0 <ymodem_uart_readable>
30007136:	2800      	cmp	r0, #0
30007138:	f000 8313 	beq.w	30007762 <BOOT_Image1+0x87a>
3000713c:	2301      	movs	r3, #1
3000713e:	9303      	str	r3, [sp, #12]
30007140:	2001      	movs	r0, #1
30007142:	47a0      	blx	r4
30007144:	2108      	movs	r1, #8
30007146:	4845      	ldr	r0, [pc, #276]	; (3000725c <BOOT_Image1+0x374>)
30007148:	f000 fdfa 	bl	30007d40 <ymodem_uart_getdata>
3000714c:	4943      	ldr	r1, [pc, #268]	; (3000725c <BOOT_Image1+0x374>)
3000714e:	2208      	movs	r2, #8
30007150:	4b43      	ldr	r3, [pc, #268]	; (30007260 <BOOT_Image1+0x378>)
30007152:	f101 0080 	add.w	r0, r1, #128	; 0x80
30007156:	4798      	blx	r3
30007158:	9004      	str	r0, [sp, #16]
3000715a:	2800      	cmp	r0, #0
3000715c:	f040 8376 	bne.w	3000784c <BOOT_Image1+0x964>
30007160:	f000 fdc6 	bl	30007cf0 <ymodem_uart_readable>
30007164:	2800      	cmp	r0, #0
30007166:	d0fb      	beq.n	30007160 <BOOT_Image1+0x278>
30007168:	2101      	movs	r1, #1
3000716a:	483e      	ldr	r0, [pc, #248]	; (30007264 <BOOT_Image1+0x37c>)
3000716c:	f000 fde8 	bl	30007d40 <ymodem_uart_getdata>
30007170:	f000 fdbe 	bl	30007cf0 <ymodem_uart_readable>
30007174:	2800      	cmp	r0, #0
30007176:	d0fb      	beq.n	30007170 <BOOT_Image1+0x288>
30007178:	4e3b      	ldr	r6, [pc, #236]	; (30007268 <BOOT_Image1+0x380>)
3000717a:	2101      	movs	r1, #1
3000717c:	f206 4051 	addw	r0, r6, #1105	; 0x451
30007180:	f000 fdde 	bl	30007d40 <ymodem_uart_getdata>
30007184:	f896 2451 	ldrb.w	r2, [r6, #1105]	; 0x451
30007188:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
3000718c:	429a      	cmp	r2, r3
3000718e:	f000 8368 	beq.w	30007862 <BOOT_Image1+0x97a>
30007192:	f206 4952 	addw	r9, r6, #1106	; 0x452
30007196:	2700      	movs	r7, #0
30007198:	46b8      	mov	r8, r7
3000719a:	3701      	adds	r7, #1
3000719c:	f000 fda8 	bl	30007cf0 <ymodem_uart_readable>
300071a0:	2800      	cmp	r0, #0
300071a2:	d0fb      	beq.n	3000719c <BOOT_Image1+0x2b4>
300071a4:	4648      	mov	r0, r9
300071a6:	2101      	movs	r1, #1
300071a8:	f000 fdca 	bl	30007d40 <ymodem_uart_getdata>
300071ac:	f899 2000 	ldrb.w	r2, [r9]
300071b0:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
300071b4:	f108 080b 	add.w	r8, r8, #11
300071b8:	f109 0901 	add.w	r9, r9, #1
300071bc:	46bb      	mov	fp, r7
300071be:	429a      	cmp	r2, r3
300071c0:	d1ea      	bne.n	30007198 <BOOT_Image1+0x2b0>
300071c2:	2f00      	cmp	r7, #0
300071c4:	dd61      	ble.n	3000728a <BOOT_Image1+0x3a2>
300071c6:	f506 618a 	add.w	r1, r6, #1104	; 0x450
300071ca:	2300      	movs	r3, #0
300071cc:	19c8      	adds	r0, r1, r7
300071ce:	e04f      	b.n	30007270 <BOOT_Image1+0x388>
300071d0:	0000c149 	.word	0x0000c149
300071d4:	3000cefc 	.word	0x3000cefc
300071d8:	3000d7d0 	.word	0x3000d7d0
300071dc:	3000cf20 	.word	0x3000cf20
300071e0:	3000eae0 	.word	0x3000eae0
300071e4:	3000ef14 	.word	0x3000ef14
300071e8:	00012be5 	.word	0x00012be5
300071ec:	42008000 	.word	0x42008000
300071f0:	42008200 	.word	0x42008200
300071f4:	3000d809 	.word	0x3000d809
300071f8:	3000bc25 	.word	0x3000bc25
300071fc:	3000ba95 	.word	0x3000ba95
30007200:	3000b9b5 	.word	0x3000b9b5
30007204:	30007f25 	.word	0x30007f25
30007208:	23020000 	.word	0x23020000
3000720c:	40080000 	.word	0x40080000
30007210:	0000b479 	.word	0x0000b479
30007214:	40040000 	.word	0x40040000
30007218:	3000a589 	.word	0x3000a589
3000721c:	3000d808 	.word	0x3000d808
30007220:	0000aab9 	.word	0x0000aab9
30007224:	4200c000 	.word	0x4200c000
30007228:	2001c00c 	.word	0x2001c00c
3000722c:	30006ee9 	.word	0x30006ee9
30007230:	00033000 	.word	0x00033000
30007234:	2001c01c 	.word	0x2001c01c
30007238:	3000d80c 	.word	0x3000d80c
3000723c:	e000ed00 	.word	0xe000ed00
30007240:	00009be5 	.word	0x00009be5
30007244:	3000cfd0 	.word	0x3000cfd0
30007248:	3000d81d 	.word	0x3000d81d
3000724c:	0000c169 	.word	0x0000c169
30007250:	0016e360 	.word	0x0016e360
30007254:	3000d0a0 	.word	0x3000d0a0
30007258:	3000e1c4 	.word	0x3000e1c4
3000725c:	3000e1e4 	.word	0x3000e1e4
30007260:	000129bd 	.word	0x000129bd
30007264:	3000e1ec 	.word	0x3000e1ec
30007268:	3000dd9c 	.word	0x3000dd9c
3000726c:	4288      	cmp	r0, r1
3000726e:	d00d      	beq.n	3000728c <BOOT_Image1+0x3a4>
30007270:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30007274:	eb03 0383 	add.w	r3, r3, r3, lsl #2
30007278:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
3000727c:	eb02 0343 	add.w	r3, r2, r3, lsl #1
30007280:	f1bc 0f09 	cmp.w	ip, #9
30007284:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
30007288:	d9f0      	bls.n	3000726c <BOOT_Image1+0x384>
3000728a:	2300      	movs	r3, #0
3000728c:	f8c6 34d4 	str.w	r3, [r6, #1236]	; 0x4d4
30007290:	f000 fd2e 	bl	30007cf0 <ymodem_uart_readable>
30007294:	2800      	cmp	r0, #0
30007296:	d0fb      	beq.n	30007290 <BOOT_Image1+0x3a8>
30007298:	49c2      	ldr	r1, [pc, #776]	; (300075a4 <BOOT_Image1+0x6bc>)
3000729a:	460b      	mov	r3, r1
3000729c:	2101      	movs	r1, #1
3000729e:	eb03 0008 	add.w	r0, r3, r8
300072a2:	9302      	str	r3, [sp, #8]
300072a4:	f000 fd4c 	bl	30007d40 <ymodem_uart_getdata>
300072a8:	9902      	ldr	r1, [sp, #8]
300072aa:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
300072ae:	f811 2008 	ldrb.w	r2, [r1, r8]
300072b2:	429a      	cmp	r2, r3
300072b4:	f000 82dd 	beq.w	30007872 <BOOT_Image1+0x98a>
300072b8:	f10b 080b 	add.w	r8, fp, #11
300072bc:	f04f 0900 	mov.w	r9, #0
300072c0:	4488      	add	r8, r1
300072c2:	f109 0901 	add.w	r9, r9, #1
300072c6:	f000 fd13 	bl	30007cf0 <ymodem_uart_readable>
300072ca:	2800      	cmp	r0, #0
300072cc:	d0fb      	beq.n	300072c6 <BOOT_Image1+0x3de>
300072ce:	4640      	mov	r0, r8
300072d0:	2101      	movs	r1, #1
300072d2:	f000 fd35 	bl	30007d40 <ymodem_uart_getdata>
300072d6:	f898 2000 	ldrb.w	r2, [r8]
300072da:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
300072de:	f108 0801 	add.w	r8, r8, #1
300072e2:	eb09 0a0b 	add.w	sl, r9, fp
300072e6:	429a      	cmp	r2, r3
300072e8:	d1eb      	bne.n	300072c2 <BOOT_Image1+0x3da>
300072ea:	f000 fd01 	bl	30007cf0 <ymodem_uart_readable>
300072ee:	2800      	cmp	r0, #0
300072f0:	d0fb      	beq.n	300072ea <BOOT_Image1+0x402>
300072f2:	9b02      	ldr	r3, [sp, #8]
300072f4:	f10a 080b 	add.w	r8, sl, #11
300072f8:	2101      	movs	r1, #1
300072fa:	eb03 0008 	add.w	r0, r3, r8
300072fe:	f000 fd1f 	bl	30007d40 <ymodem_uart_getdata>
30007302:	9902      	ldr	r1, [sp, #8]
30007304:	f896 24d1 	ldrb.w	r2, [r6, #1233]	; 0x4d1
30007308:	f811 3008 	ldrb.w	r3, [r1, r8]
3000730c:	429a      	cmp	r2, r3
3000730e:	f000 82ae 	beq.w	3000786e <BOOT_Image1+0x986>
30007312:	f10a 080c 	add.w	r8, sl, #12
30007316:	46d3      	mov	fp, sl
30007318:	f8dd a010 	ldr.w	sl, [sp, #16]
3000731c:	4488      	add	r8, r1
3000731e:	9405      	str	r4, [sp, #20]
30007320:	f10a 0a01 	add.w	sl, sl, #1
30007324:	f000 fce4 	bl	30007cf0 <ymodem_uart_readable>
30007328:	2800      	cmp	r0, #0
3000732a:	d0fb      	beq.n	30007324 <BOOT_Image1+0x43c>
3000732c:	4640      	mov	r0, r8
3000732e:	2101      	movs	r1, #1
30007330:	f000 fd06 	bl	30007d40 <ymodem_uart_getdata>
30007334:	f898 2000 	ldrb.w	r2, [r8]
30007338:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
3000733c:	f108 0801 	add.w	r8, r8, #1
30007340:	eb0a 010b 	add.w	r1, sl, fp
30007344:	429a      	cmp	r2, r3
30007346:	d1eb      	bne.n	30007320 <BOOT_Image1+0x438>
30007348:	468b      	mov	fp, r1
3000734a:	9c05      	ldr	r4, [sp, #20]
3000734c:	f8cd a010 	str.w	sl, [sp, #16]
30007350:	9b02      	ldr	r3, [sp, #8]
30007352:	f10b 080c 	add.w	r8, fp, #12
30007356:	2101      	movs	r1, #1
30007358:	eb03 0008 	add.w	r0, r3, r8
3000735c:	f000 fcf0 	bl	30007d40 <ymodem_uart_getdata>
30007360:	9b02      	ldr	r3, [sp, #8]
30007362:	f813 1008 	ldrb.w	r1, [r3, r8]
30007366:	290a      	cmp	r1, #10
30007368:	f8c6 14d8 	str.w	r1, [r6, #1240]	; 0x4d8
3000736c:	f200 826a 	bhi.w	30007844 <BOOT_Image1+0x95c>
30007370:	9b02      	ldr	r3, [sp, #8]
30007372:	f10b 000d 	add.w	r0, fp, #13
30007376:	2101      	movs	r1, #1
30007378:	f10b 080e 	add.w	r8, fp, #14
3000737c:	4418      	add	r0, r3
3000737e:	f000 fcdf 	bl	30007d40 <ymodem_uart_getdata>
30007382:	9b02      	ldr	r3, [sp, #8]
30007384:	2102      	movs	r1, #2
30007386:	4498      	add	r8, r3
30007388:	4640      	mov	r0, r8
3000738a:	f000 fcd9 	bl	30007d40 <ymodem_uart_getdata>
3000738e:	4641      	mov	r1, r8
30007390:	2202      	movs	r2, #2
30007392:	4885      	ldr	r0, [pc, #532]	; (300075a8 <BOOT_Image1+0x6c0>)
30007394:	4b85      	ldr	r3, [pc, #532]	; (300075ac <BOOT_Image1+0x6c4>)
30007396:	4798      	blx	r3
30007398:	2800      	cmp	r0, #0
3000739a:	f040 825e 	bne.w	3000785a <BOOT_Image1+0x972>
3000739e:	9b02      	ldr	r3, [sp, #8]
300073a0:	f10b 0010 	add.w	r0, fp, #16
300073a4:	2101      	movs	r1, #1
300073a6:	f10b 0811 	add.w	r8, fp, #17
300073aa:	4418      	add	r0, r3
300073ac:	f000 fcc8 	bl	30007d40 <ymodem_uart_getdata>
300073b0:	4b7f      	ldr	r3, [pc, #508]	; (300075b0 <BOOT_Image1+0x6c8>)
300073b2:	f8d6 04d8 	ldr.w	r0, [r6, #1240]	; 0x4d8
300073b6:	f103 0228 	add.w	r2, r3, #40	; 0x28
300073ba:	f103 0130 	add.w	r1, r3, #48	; 0x30
300073be:	f7fd fda1 	bl	30004f04 <hotfix_get_addr_size>
300073c2:	9b02      	ldr	r3, [sp, #8]
300073c4:	2102      	movs	r1, #2
300073c6:	4498      	add	r8, r3
300073c8:	4640      	mov	r0, r8
300073ca:	f000 fcb9 	bl	30007d40 <ymodem_uart_getdata>
300073ce:	4641      	mov	r1, r8
300073d0:	2202      	movs	r2, #2
300073d2:	4878      	ldr	r0, [pc, #480]	; (300075b4 <BOOT_Image1+0x6cc>)
300073d4:	4b75      	ldr	r3, [pc, #468]	; (300075ac <BOOT_Image1+0x6c4>)
300073d6:	4798      	blx	r3
300073d8:	2800      	cmp	r0, #0
300073da:	f000 8148 	beq.w	3000766e <BOOT_Image1+0x786>
300073de:	9b03      	ldr	r3, [sp, #12]
300073e0:	2b00      	cmp	r3, #0
300073e2:	f000 818f 	beq.w	30007704 <BOOT_Image1+0x81c>
300073e6:	f7fe fc9b 	bl	30005d20 <BOOT_LoadImages>
300073ea:	2800      	cmp	r0, #0
300073ec:	f000 8138 	beq.w	30007660 <BOOT_Image1+0x778>
300073f0:	f7ff fa50 	bl	30006894 <BOOT_Enable_KM0>
300073f4:	2008      	movs	r0, #8
300073f6:	4b70      	ldr	r3, [pc, #448]	; (300075b8 <BOOT_Image1+0x6d0>)
300073f8:	4798      	blx	r3
300073fa:	4b70      	ldr	r3, [pc, #448]	; (300075bc <BOOT_Image1+0x6d4>)
300073fc:	2201      	movs	r2, #1
300073fe:	f8c3 2398 	str.w	r2, [r3, #920]	; 0x398
30007402:	f7fd fca3 	bl	30004d4c <BOOT_RAM_TZCfg>
30007406:	4b6e      	ldr	r3, [pc, #440]	; (300075c0 <BOOT_Image1+0x6d8>)
30007408:	781b      	ldrb	r3, [r3, #0]
3000740a:	2b01      	cmp	r3, #1
3000740c:	f000 8212 	beq.w	30007834 <BOOT_Image1+0x94c>
30007410:	4b6c      	ldr	r3, [pc, #432]	; (300075c4 <BOOT_Image1+0x6dc>)
30007412:	781b      	ldrb	r3, [r3, #0]
30007414:	2b00      	cmp	r3, #0
30007416:	f000 817c 	beq.w	30007712 <BOOT_Image1+0x82a>
3000741a:	f7ff fb67 	bl	30006aec <BOOT_Enable_AP>
3000741e:	4a6a      	ldr	r2, [pc, #424]	; (300075c8 <BOOT_Image1+0x6e0>)
30007420:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
30007424:	f043 0302 	orr.w	r3, r3, #2
30007428:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
3000742c:	4b64      	ldr	r3, [pc, #400]	; (300075c0 <BOOT_Image1+0x6d8>)
3000742e:	781b      	ldrb	r3, [r3, #0]
30007430:	2b01      	cmp	r3, #1
30007432:	f000 81f7 	beq.w	30007824 <BOOT_Image1+0x93c>
30007436:	4964      	ldr	r1, [pc, #400]	; (300075c8 <BOOT_Image1+0x6e0>)
30007438:	4a64      	ldr	r2, [pc, #400]	; (300075cc <BOOT_Image1+0x6e4>)
3000743a:	f891 3067 	ldrb.w	r3, [r1, #103]	; 0x67
3000743e:	6894      	ldr	r4, [r2, #8]
30007440:	f043 0301 	orr.w	r3, r3, #1
30007444:	f881 3067 	strb.w	r3, [r1, #103]	; 0x67
30007448:	4b61      	ldr	r3, [pc, #388]	; (300075d0 <BOOT_Image1+0x6e8>)
3000744a:	6811      	ldr	r1, [r2, #0]
3000744c:	4a61      	ldr	r2, [pc, #388]	; (300075d4 <BOOT_Image1+0x6ec>)
3000744e:	6061      	str	r1, [r4, #4]
30007450:	6094      	str	r4, [r2, #8]
30007452:	6a59      	ldr	r1, [r3, #36]	; 0x24
30007454:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30007458:	6259      	str	r1, [r3, #36]	; 0x24
3000745a:	6a51      	ldr	r1, [r2, #36]	; 0x24
3000745c:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30007460:	6251      	str	r1, [r2, #36]	; 0x24
30007462:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30007466:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000746a:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
3000746e:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30007472:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30007476:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
3000747a:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
3000747e:	4956      	ldr	r1, [pc, #344]	; (300075d8 <BOOT_Image1+0x6f0>)
30007480:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30007484:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30007488:	689b      	ldr	r3, [r3, #8]
3000748a:	618b      	str	r3, [r1, #24]
3000748c:	f7fe fbd0 	bl	30005c30 <BOOT_NVICBackup_HP>
30007490:	4b52      	ldr	r3, [pc, #328]	; (300075dc <BOOT_Image1+0x6f4>)
30007492:	f383 8888 	msr	MSP_NS, r3
30007496:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
3000749a:	f383 8809 	msr	PSP, r3
3000749e:	68ab      	ldr	r3, [r5, #8]
300074a0:	079b      	lsls	r3, r3, #30
300074a2:	f100 8154 	bmi.w	3000774e <BOOT_Image1+0x866>
300074a6:	6860      	ldr	r0, [r4, #4]
300074a8:	f7fd ff98 	bl	300053dc <BOOT_NsStart>
300074ac:	b00b      	add	sp, #44	; 0x2c
300074ae:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300074b2:	3174      	adds	r1, #116	; 0x74
300074b4:	f027 031f 	bic.w	r3, r7, #31
300074b8:	e59b      	b.n	30006ff2 <BOOT_Image1+0x10a>
300074ba:	4849      	ldr	r0, [pc, #292]	; (300075e0 <BOOT_Image1+0x6f8>)
300074bc:	f004 feb4 	bl	3000c228 <__DiagPrintf_veneer>
300074c0:	f89d 101f 	ldrb.w	r1, [sp, #31]
300074c4:	f89d 301e 	ldrb.w	r3, [sp, #30]
300074c8:	f44f 705a 	mov.w	r0, #872	; 0x368
300074cc:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
300074d0:	f023 0308 	bic.w	r3, r3, #8
300074d4:	f88d 101f 	strb.w	r1, [sp, #31]
300074d8:	f88d 301e 	strb.w	r3, [sp, #30]
300074dc:	f001 fece 	bl	3000927c <OTP_Write8>
300074e0:	f89d 101e 	ldrb.w	r1, [sp, #30]
300074e4:	f240 3069 	movw	r0, #873	; 0x369
300074e8:	f001 fec8 	bl	3000927c <OTP_Write8>
300074ec:	483d      	ldr	r0, [pc, #244]	; (300075e4 <BOOT_Image1+0x6fc>)
300074ee:	f004 fe9b 	bl	3000c228 <__DiagPrintf_veneer>
300074f2:	4b33      	ldr	r3, [pc, #204]	; (300075c0 <BOOT_Image1+0x6d8>)
300074f4:	781b      	ldrb	r3, [r3, #0]
300074f6:	2b00      	cmp	r3, #0
300074f8:	f43f aded 	beq.w	300070d6 <BOOT_Image1+0x1ee>
300074fc:	4b3a      	ldr	r3, [pc, #232]	; (300075e8 <BOOT_Image1+0x700>)
300074fe:	4798      	blx	r3
30007500:	2800      	cmp	r0, #0
30007502:	f47f ade8 	bne.w	300070d6 <BOOT_Image1+0x1ee>
30007506:	68ab      	ldr	r3, [r5, #8]
30007508:	f043 0302 	orr.w	r3, r3, #2
3000750c:	60ab      	str	r3, [r5, #8]
3000750e:	e5e2      	b.n	300070d6 <BOOT_Image1+0x1ee>
30007510:	4836      	ldr	r0, [pc, #216]	; (300075ec <BOOT_Image1+0x704>)
30007512:	f004 fe89 	bl	3000c228 <__DiagPrintf_veneer>
30007516:	e4f2      	b.n	30006efe <BOOT_Image1+0x16>
30007518:	4b35      	ldr	r3, [pc, #212]	; (300075f0 <BOOT_Image1+0x708>)
3000751a:	2001      	movs	r0, #1
3000751c:	4798      	blx	r3
3000751e:	4620      	mov	r0, r4
30007520:	4b34      	ldr	r3, [pc, #208]	; (300075f4 <BOOT_Image1+0x70c>)
30007522:	4798      	blx	r3
30007524:	4b34      	ldr	r3, [pc, #208]	; (300075f8 <BOOT_Image1+0x710>)
30007526:	2001      	movs	r0, #1
30007528:	4798      	blx	r3
3000752a:	e50b      	b.n	30006f44 <BOOT_Image1+0x5c>
3000752c:	4b33      	ldr	r3, [pc, #204]	; (300075fc <BOOT_Image1+0x714>)
3000752e:	2201      	movs	r2, #1
30007530:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30007534:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30007538:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
3000753c:	4b30      	ldr	r3, [pc, #192]	; (30007600 <BOOT_Image1+0x718>)
3000753e:	4798      	blx	r3
30007540:	4a30      	ldr	r2, [pc, #192]	; (30007604 <BOOT_Image1+0x71c>)
30007542:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30007546:	f043 0302 	orr.w	r3, r3, #2
3000754a:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
3000754e:	e518      	b.n	30006f82 <BOOT_Image1+0x9a>
30007550:	68ab      	ldr	r3, [r5, #8]
30007552:	0798      	lsls	r0, r3, #30
30007554:	f100 80ed 	bmi.w	30007732 <BOOT_Image1+0x84a>
30007558:	4b2b      	ldr	r3, [pc, #172]	; (30007608 <BOOT_Image1+0x720>)
3000755a:	4798      	blx	r3
3000755c:	f7fe fa16 	bl	3000598c <BOOT_PSRAM_Init>
30007560:	7830      	ldrb	r0, [r6, #0]
30007562:	2800      	cmp	r0, #0
30007564:	f47f ad80 	bne.w	30007068 <BOOT_Image1+0x180>
30007568:	4b28      	ldr	r3, [pc, #160]	; (3000760c <BOOT_Image1+0x724>)
3000756a:	4798      	blx	r3
3000756c:	2800      	cmp	r0, #0
3000756e:	f43f ad7b 	beq.w	30007068 <BOOT_Image1+0x180>
30007572:	682b      	ldr	r3, [r5, #0]
30007574:	0399      	lsls	r1, r3, #14
30007576:	f57f ad77 	bpl.w	30007068 <BOOT_Image1+0x180>
3000757a:	4825      	ldr	r0, [pc, #148]	; (30007610 <BOOT_Image1+0x728>)
3000757c:	f004 fe54 	bl	3000c228 <__DiagPrintf_veneer>
30007580:	e572      	b.n	30007068 <BOOT_Image1+0x180>
30007582:	4924      	ldr	r1, [pc, #144]	; (30007614 <BOOT_Image1+0x72c>)
30007584:	4824      	ldr	r0, [pc, #144]	; (30007618 <BOOT_Image1+0x730>)
30007586:	f004 fe4f 	bl	3000c228 <__DiagPrintf_veneer>
3000758a:	e5bf      	b.n	3000710c <BOOT_Image1+0x224>
3000758c:	4601      	mov	r1, r0
3000758e:	f44f 7201 	mov.w	r2, #516	; 0x204
30007592:	481a      	ldr	r0, [pc, #104]	; (300075fc <BOOT_Image1+0x714>)
30007594:	f004 fe28 	bl	3000c1e8 <____wrap_memset_veneer>
30007598:	e4c7      	b.n	30006f2a <BOOT_Image1+0x42>
3000759a:	4820      	ldr	r0, [pc, #128]	; (3000761c <BOOT_Image1+0x734>)
3000759c:	f004 fe44 	bl	3000c228 <__DiagPrintf_veneer>
300075a0:	e511      	b.n	30006fc6 <BOOT_Image1+0xde>
300075a2:	bf00      	nop
300075a4:	3000e1e4 	.word	0x3000e1e4
300075a8:	3000e278 	.word	0x3000e278
300075ac:	000129bd 	.word	0x000129bd
300075b0:	3000e27c 	.word	0x3000e27c
300075b4:	3000e2b8 	.word	0x3000e2b8
300075b8:	0000e1d9 	.word	0x0000e1d9
300075bc:	400e0000 	.word	0x400e0000
300075c0:	3000d81d 	.word	0x3000d81d
300075c4:	3000d80a 	.word	0x3000d80a
300075c8:	42008200 	.word	0x42008200
300075cc:	60000020 	.word	0x60000020
300075d0:	e000ed00 	.word	0xe000ed00
300075d4:	e002ed00 	.word	0xe002ed00
300075d8:	3000eea4 	.word	0x3000eea4
300075dc:	2001bffc 	.word	0x2001bffc
300075e0:	3000d044 	.word	0x3000d044
300075e4:	3000d07c 	.word	0x3000d07c
300075e8:	0000c179 	.word	0x0000c179
300075ec:	3000cf0c 	.word	0x3000cf0c
300075f0:	3000b9b5 	.word	0x3000b9b5
300075f4:	3000bbe5 	.word	0x3000bbe5
300075f8:	3000bb55 	.word	0x3000bb55
300075fc:	23020000 	.word	0x23020000
30007600:	0000b479 	.word	0x0000b479
30007604:	42008000 	.word	0x42008000
30007608:	30009579 	.word	0x30009579
3000760c:	3000bab5 	.word	0x3000bab5
30007610:	3000cfc0 	.word	0x3000cfc0
30007614:	0016e360 	.word	0x0016e360
30007618:	3000d0a0 	.word	0x3000d0a0
3000761c:	3000cf68 	.word	0x3000cf68
30007620:	f3ef 8108 	mrs	r1, MSP
30007624:	4898      	ldr	r0, [pc, #608]	; (30007888 <BOOT_Image1+0x9a0>)
30007626:	f004 fdff 	bl	3000c228 <__DiagPrintf_veneer>
3000762a:	e4c3      	b.n	30006fb4 <BOOT_Image1+0xcc>
3000762c:	4f97      	ldr	r7, [pc, #604]	; (3000788c <BOOT_Image1+0x9a4>)
3000762e:	47b8      	blx	r7
30007630:	4605      	mov	r5, r0
30007632:	47b8      	blx	r7
30007634:	2d02      	cmp	r5, #2
30007636:	f43f acb3 	beq.w	30006fa0 <BOOT_Image1+0xb8>
3000763a:	2803      	cmp	r0, #3
3000763c:	f43f acb0 	beq.w	30006fa0 <BOOT_Image1+0xb8>
30007640:	4b93      	ldr	r3, [pc, #588]	; (30007890 <BOOT_Image1+0x9a8>)
30007642:	4798      	blx	r3
30007644:	4d93      	ldr	r5, [pc, #588]	; (30007894 <BOOT_Image1+0x9ac>)
30007646:	2200      	movs	r2, #0
30007648:	2101      	movs	r1, #1
3000764a:	4893      	ldr	r0, [pc, #588]	; (30007898 <BOOT_Image1+0x9b0>)
3000764c:	47a8      	blx	r5
3000764e:	2101      	movs	r1, #1
30007650:	4891      	ldr	r0, [pc, #580]	; (30007898 <BOOT_Image1+0x9b0>)
30007652:	4b92      	ldr	r3, [pc, #584]	; (3000789c <BOOT_Image1+0x9b4>)
30007654:	4798      	blx	r3
30007656:	2201      	movs	r2, #1
30007658:	488f      	ldr	r0, [pc, #572]	; (30007898 <BOOT_Image1+0x9b0>)
3000765a:	4611      	mov	r1, r2
3000765c:	47a8      	blx	r5
3000765e:	e4a0      	b.n	30006fa2 <BOOT_Image1+0xba>
30007660:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007664:	47a0      	blx	r4
30007666:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000766a:	47a0      	blx	r4
3000766c:	e7f8      	b.n	30007660 <BOOT_Image1+0x778>
3000766e:	9b02      	ldr	r3, [sp, #8]
30007670:	f10b 0013 	add.w	r0, fp, #19
30007674:	2101      	movs	r1, #1
30007676:	f10b 0814 	add.w	r8, fp, #20
3000767a:	4418      	add	r0, r3
3000767c:	f000 fb60 	bl	30007d40 <ymodem_uart_getdata>
30007680:	4b87      	ldr	r3, [pc, #540]	; (300078a0 <BOOT_Image1+0x9b8>)
30007682:	f8d6 04d8 	ldr.w	r0, [r6, #1240]	; 0x4d8
30007686:	f1a3 0218 	sub.w	r2, r3, #24
3000768a:	f1a3 0110 	sub.w	r1, r3, #16
3000768e:	f7fd fc39 	bl	30004f04 <hotfix_get_addr_size>
30007692:	9b02      	ldr	r3, [sp, #8]
30007694:	2102      	movs	r1, #2
30007696:	4498      	add	r8, r3
30007698:	4640      	mov	r0, r8
3000769a:	f000 fb51 	bl	30007d40 <ymodem_uart_getdata>
3000769e:	4641      	mov	r1, r8
300076a0:	2202      	movs	r2, #2
300076a2:	4880      	ldr	r0, [pc, #512]	; (300078a4 <BOOT_Image1+0x9bc>)
300076a4:	4b80      	ldr	r3, [pc, #512]	; (300078a8 <BOOT_Image1+0x9c0>)
300076a6:	4798      	blx	r3
300076a8:	2800      	cmp	r0, #0
300076aa:	f47f ae98 	bne.w	300073de <BOOT_Image1+0x4f6>
300076ae:	9b02      	ldr	r3, [sp, #8]
300076b0:	f10b 0016 	add.w	r0, fp, #22
300076b4:	2101      	movs	r1, #1
300076b6:	f10b 0817 	add.w	r8, fp, #23
300076ba:	4418      	add	r0, r3
300076bc:	f000 fb40 	bl	30007d40 <ymodem_uart_getdata>
300076c0:	4b7a      	ldr	r3, [pc, #488]	; (300078ac <BOOT_Image1+0x9c4>)
300076c2:	f8d6 04d8 	ldr.w	r0, [r6, #1240]	; 0x4d8
300076c6:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
300076ca:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
300076ce:	f7fd fc19 	bl	30004f04 <hotfix_get_addr_size>
300076d2:	9a02      	ldr	r2, [sp, #8]
300076d4:	2102      	movs	r1, #2
300076d6:	4490      	add	r8, r2
300076d8:	4640      	mov	r0, r8
300076da:	f000 fb31 	bl	30007d40 <ymodem_uart_getdata>
300076de:	4641      	mov	r1, r8
300076e0:	2202      	movs	r2, #2
300076e2:	4873      	ldr	r0, [pc, #460]	; (300078b0 <BOOT_Image1+0x9c8>)
300076e4:	4b70      	ldr	r3, [pc, #448]	; (300078a8 <BOOT_Image1+0x9c0>)
300076e6:	4798      	blx	r3
300076e8:	2800      	cmp	r0, #0
300076ea:	f47f ae78 	bne.w	300073de <BOOT_Image1+0x4f6>
300076ee:	9b02      	ldr	r3, [sp, #8]
300076f0:	f10b 0019 	add.w	r0, fp, #25
300076f4:	2101      	movs	r1, #1
300076f6:	4418      	add	r0, r3
300076f8:	f000 fb22 	bl	30007d40 <ymodem_uart_getdata>
300076fc:	9b03      	ldr	r3, [sp, #12]
300076fe:	2b00      	cmp	r3, #0
30007700:	f040 80bb 	bne.w	3000787a <BOOT_Image1+0x992>
30007704:	2002      	movs	r0, #2
30007706:	f000 facb 	bl	30007ca0 <ymodem_uart_port_deinit>
3000770a:	486a      	ldr	r0, [pc, #424]	; (300078b4 <BOOT_Image1+0x9cc>)
3000770c:	f004 fd8c 	bl	3000c228 <__DiagPrintf_veneer>
30007710:	e669      	b.n	300073e6 <BOOT_Image1+0x4fe>
30007712:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30007716:	4968      	ldr	r1, [pc, #416]	; (300078b8 <BOOT_Image1+0x9d0>)
30007718:	685a      	ldr	r2, [r3, #4]
3000771a:	f042 0202 	orr.w	r2, r2, #2
3000771e:	605a      	str	r2, [r3, #4]
30007720:	685a      	ldr	r2, [r3, #4]
30007722:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30007726:	605a      	str	r2, [r3, #4]
30007728:	6f8b      	ldr	r3, [r1, #120]	; 0x78
3000772a:	f043 0303 	orr.w	r3, r3, #3
3000772e:	678b      	str	r3, [r1, #120]	; 0x78
30007730:	e67c      	b.n	3000742c <BOOT_Image1+0x544>
30007732:	4862      	ldr	r0, [pc, #392]	; (300078bc <BOOT_Image1+0x9d4>)
30007734:	f004 fd78 	bl	3000c228 <__DiagPrintf_veneer>
30007738:	e70e      	b.n	30007558 <BOOT_Image1+0x670>
3000773a:	4861      	ldr	r0, [pc, #388]	; (300078c0 <BOOT_Image1+0x9d8>)
3000773c:	f004 fd74 	bl	3000c228 <__DiagPrintf_veneer>
30007740:	e487      	b.n	30007052 <BOOT_Image1+0x16a>
30007742:	f000 fad5 	bl	30007cf0 <ymodem_uart_readable>
30007746:	2800      	cmp	r0, #0
30007748:	d0dc      	beq.n	30007704 <BOOT_Image1+0x81c>
3000774a:	9603      	str	r6, [sp, #12]
3000774c:	e4f8      	b.n	30007140 <BOOT_Image1+0x258>
3000774e:	6861      	ldr	r1, [r4, #4]
30007750:	485c      	ldr	r0, [pc, #368]	; (300078c4 <BOOT_Image1+0x9dc>)
30007752:	f004 fd69 	bl	3000c228 <__DiagPrintf_veneer>
30007756:	6860      	ldr	r0, [r4, #4]
30007758:	f7fd fe40 	bl	300053dc <BOOT_NsStart>
3000775c:	b00b      	add	sp, #44	; 0x2c
3000775e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007762:	4e59      	ldr	r6, [pc, #356]	; (300078c8 <BOOT_Image1+0x9e0>)
30007764:	4680      	mov	r8, r0
30007766:	f04f 0a0b 	mov.w	sl, #11
3000776a:	270a      	movs	r7, #10
3000776c:	f506 6389 	add.w	r3, r6, #1096	; 0x448
30007770:	4681      	mov	r9, r0
30007772:	9302      	str	r3, [sp, #8]
30007774:	2004      	movs	r0, #4
30007776:	f000 fa93 	bl	30007ca0 <ymodem_uart_port_deinit>
3000777a:	9a02      	ldr	r2, [sp, #8]
3000777c:	f8cd 8000 	str.w	r8, [sp]
30007780:	4613      	mov	r3, r2
30007782:	19d0      	adds	r0, r2, r7
30007784:	f8d6 14d4 	ldr.w	r1, [r6, #1236]	; 0x4d4
30007788:	464a      	mov	r2, r9
3000778a:	4453      	add	r3, sl
3000778c:	f7ff fb06 	bl	30006d9c <password_hash_check>
30007790:	4680      	mov	r8, r0
30007792:	2800      	cmp	r0, #0
30007794:	d1b6      	bne.n	30007704 <BOOT_Image1+0x81c>
30007796:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000779a:	47a0      	blx	r4
3000779c:	484b      	ldr	r0, [pc, #300]	; (300078cc <BOOT_Image1+0x9e4>)
3000779e:	f004 fd03 	bl	3000c1a8 <____wrap_strlen_veneer>
300077a2:	4601      	mov	r1, r0
300077a4:	4849      	ldr	r0, [pc, #292]	; (300078cc <BOOT_Image1+0x9e4>)
300077a6:	f000 fabd 	bl	30007d24 <ymodem_uart_putdata>
300077aa:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077ae:	47a0      	blx	r4
300077b0:	f8d6 24d8 	ldr.w	r2, [r6, #1240]	; 0x4d8
300077b4:	4946      	ldr	r1, [pc, #280]	; (300078d0 <BOOT_Image1+0x9e8>)
300077b6:	2002      	movs	r0, #2
300077b8:	f000 fb54 	bl	30007e64 <ymodem_img_download>
300077bc:	2032      	movs	r0, #50	; 0x32
300077be:	47a0      	blx	r4
300077c0:	f8d6 34d8 	ldr.w	r3, [r6, #1240]	; 0x4d8
300077c4:	b1e3      	cbz	r3, 30007800 <BOOT_Image1+0x918>
300077c6:	f8df 90e4 	ldr.w	r9, [pc, #228]	; 300078ac <BOOT_Image1+0x9c4>
300077ca:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
300077ce:	f859 7b04 	ldr.w	r7, [r9], #4
300077d2:	0b3f      	lsrs	r7, r7, #12
300077d4:	d00c      	beq.n	300077f0 <BOOT_Image1+0x908>
300077d6:	2500      	movs	r5, #0
300077d8:	f8da 1000 	ldr.w	r1, [sl]
300077dc:	2002      	movs	r0, #2
300077de:	eb01 3105 	add.w	r1, r1, r5, lsl #12
300077e2:	3501      	adds	r5, #1
300077e4:	f7fd fc0e 	bl	30005004 <FLASH_Erase_With_Lock>
300077e8:	2032      	movs	r0, #50	; 0x32
300077ea:	47a0      	blx	r4
300077ec:	42af      	cmp	r7, r5
300077ee:	d1f3      	bne.n	300077d8 <BOOT_Image1+0x8f0>
300077f0:	f108 0801 	add.w	r8, r8, #1
300077f4:	f8d6 34d8 	ldr.w	r3, [r6, #1240]	; 0x4d8
300077f8:	f10a 0a04 	add.w	sl, sl, #4
300077fc:	4543      	cmp	r3, r8
300077fe:	d8e6      	bhi.n	300077ce <BOOT_Image1+0x8e6>
30007800:	4834      	ldr	r0, [pc, #208]	; (300078d4 <BOOT_Image1+0x9ec>)
30007802:	f004 fd11 	bl	3000c228 <__DiagPrintf_veneer>
30007806:	4b34      	ldr	r3, [pc, #208]	; (300078d8 <BOOT_Image1+0x9f0>)
30007808:	a808      	add	r0, sp, #32
3000780a:	4798      	blx	r3
3000780c:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
30007810:	4832      	ldr	r0, [pc, #200]	; (300078dc <BOOT_Image1+0x9f4>)
30007812:	a908      	add	r1, sp, #32
30007814:	4b32      	ldr	r3, [pc, #200]	; (300078e0 <BOOT_Image1+0x9f8>)
30007816:	f8ad 2022 	strh.w	r2, [sp, #34]	; 0x22
3000781a:	4798      	blx	r3
3000781c:	482f      	ldr	r0, [pc, #188]	; (300078dc <BOOT_Image1+0x9f4>)
3000781e:	4b31      	ldr	r3, [pc, #196]	; (300078e4 <BOOT_Image1+0x9fc>)
30007820:	4798      	blx	r3
30007822:	e7fe      	b.n	30007822 <BOOT_Image1+0x93a>
30007824:	4b30      	ldr	r3, [pc, #192]	; (300078e8 <BOOT_Image1+0xa00>)
30007826:	4798      	blx	r3
30007828:	2801      	cmp	r0, #1
3000782a:	bf0c      	ite	eq
3000782c:	20e6      	moveq	r0, #230	; 0xe6
3000782e:	2019      	movne	r0, #25
30007830:	47a0      	blx	r4
30007832:	e600      	b.n	30007436 <BOOT_Image1+0x54e>
30007834:	4b2c      	ldr	r3, [pc, #176]	; (300078e8 <BOOT_Image1+0xa00>)
30007836:	4798      	blx	r3
30007838:	2801      	cmp	r0, #1
3000783a:	bf0c      	ite	eq
3000783c:	2019      	moveq	r0, #25
3000783e:	2004      	movne	r0, #4
30007840:	47a0      	blx	r4
30007842:	e5e5      	b.n	30007410 <BOOT_Image1+0x528>
30007844:	4829      	ldr	r0, [pc, #164]	; (300078ec <BOOT_Image1+0xa04>)
30007846:	f004 fcef 	bl	3000c228 <__DiagPrintf_veneer>
3000784a:	e5c8      	b.n	300073de <BOOT_Image1+0x4f6>
3000784c:	4828      	ldr	r0, [pc, #160]	; (300078f0 <BOOT_Image1+0xa08>)
3000784e:	f004 fceb 	bl	3000c228 <__DiagPrintf_veneer>
30007852:	4818      	ldr	r0, [pc, #96]	; (300078b4 <BOOT_Image1+0x9cc>)
30007854:	f004 fce8 	bl	3000c228 <__DiagPrintf_veneer>
30007858:	e5c1      	b.n	300073de <BOOT_Image1+0x4f6>
3000785a:	4826      	ldr	r0, [pc, #152]	; (300078f4 <BOOT_Image1+0xa0c>)
3000785c:	f004 fce4 	bl	3000c228 <__DiagPrintf_veneer>
30007860:	e5bd      	b.n	300073de <BOOT_Image1+0x4f6>
30007862:	2700      	movs	r7, #0
30007864:	f04f 080a 	mov.w	r8, #10
30007868:	463b      	mov	r3, r7
3000786a:	46bb      	mov	fp, r7
3000786c:	e50e      	b.n	3000728c <BOOT_Image1+0x3a4>
3000786e:	46d3      	mov	fp, sl
30007870:	e56e      	b.n	30007350 <BOOT_Image1+0x468>
30007872:	46da      	mov	sl, fp
30007874:	f04f 0900 	mov.w	r9, #0
30007878:	e537      	b.n	300072ea <BOOT_Image1+0x402>
3000787a:	f107 0a0b 	add.w	sl, r7, #11
3000787e:	f8dd 8010 	ldr.w	r8, [sp, #16]
30007882:	370a      	adds	r7, #10
30007884:	44ca      	add	sl, r9
30007886:	e775      	b.n	30007774 <BOOT_Image1+0x88c>
30007888:	3000cf38 	.word	0x3000cf38
3000788c:	0000c0f9 	.word	0x0000c0f9
30007890:	0000aea1 	.word	0x0000aea1
30007894:	0000aab9 	.word	0x0000aab9
30007898:	4200c000 	.word	0x4200c000
3000789c:	0000aaa5 	.word	0x0000aaa5
300078a0:	3000e2bc 	.word	0x3000e2bc
300078a4:	3000e2e4 	.word	0x3000e2e4
300078a8:	000129bd 	.word	0x000129bd
300078ac:	3000e2e8 	.word	0x3000e2e8
300078b0:	3000e310 	.word	0x3000e310
300078b4:	3000d0d0 	.word	0x3000d0d0
300078b8:	41000200 	.word	0x41000200
300078bc:	3000cf98 	.word	0x3000cf98
300078c0:	3000d020 	.word	0x3000d020
300078c4:	3000d128 	.word	0x3000d128
300078c8:	3000dd9c 	.word	0x3000dd9c
300078cc:	3000eac8 	.word	0x3000eac8
300078d0:	3000e27c 	.word	0x3000e27c
300078d4:	3000d108 	.word	0x3000d108
300078d8:	0000d3f9 	.word	0x0000d3f9
300078dc:	41000440 	.word	0x41000440
300078e0:	0000d409 	.word	0x0000d409
300078e4:	0000d489 	.word	0x0000d489
300078e8:	0000c169 	.word	0x0000c169
300078ec:	3000d0e0 	.word	0x3000d0e0
300078f0:	3000d0b8 	.word	0x3000d0b8
300078f4:	3000d0f4 	.word	0x3000d0f4

300078f8 <ymodem_inquiry>:
300078f8:	2806      	cmp	r0, #6
300078fa:	b510      	push	{r4, lr}
300078fc:	4604      	mov	r4, r0
300078fe:	d003      	beq.n	30007908 <ymodem_inquiry+0x10>
30007900:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007904:	4b03      	ldr	r3, [pc, #12]	; (30007914 <ymodem_inquiry+0x1c>)
30007906:	4798      	blx	r3
30007908:	4620      	mov	r0, r4
3000790a:	f000 fa07 	bl	30007d1c <ymodem_uart_putc>
3000790e:	2001      	movs	r0, #1
30007910:	bd10      	pop	{r4, pc}
30007912:	bf00      	nop
30007914:	00009b2d 	.word	0x00009b2d

30007918 <ymodem_get_first>:
30007918:	b530      	push	{r4, r5, lr}
3000791a:	b083      	sub	sp, #12
3000791c:	2300      	movs	r3, #0
3000791e:	4604      	mov	r4, r0
30007920:	4936      	ldr	r1, [pc, #216]	; (300079fc <ymodem_get_first+0xe4>)
30007922:	f10d 0006 	add.w	r0, sp, #6
30007926:	f88d 3006 	strb.w	r3, [sp, #6]
3000792a:	f000 fa19 	bl	30007d60 <ymodem_uart_getc_to>
3000792e:	b9d0      	cbnz	r0, 30007966 <ymodem_get_first+0x4e>
30007930:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007934:	3b01      	subs	r3, #1
30007936:	2b17      	cmp	r3, #23
30007938:	d80d      	bhi.n	30007956 <ymodem_get_first+0x3e>
3000793a:	e8df f003 	tbb	[pc, r3]
3000793e:	231e      	.short	0x231e
30007940:	0c0c290c 	.word	0x0c0c290c
30007944:	0c0c0c0c 	.word	0x0c0c0c0c
30007948:	0c0c0c0c 	.word	0x0c0c0c0c
3000794c:	0c0c0c0c 	.word	0x0c0c0c0c
30007950:	0c0c0c0c 	.word	0x0c0c0c0c
30007954:	190c      	.short	0x190c
30007956:	4d29      	ldr	r5, [pc, #164]	; (300079fc <ymodem_get_first+0xe4>)
30007958:	4629      	mov	r1, r5
3000795a:	f10d 0007 	add.w	r0, sp, #7
3000795e:	f000 f9ff 	bl	30007d60 <ymodem_uart_getc_to>
30007962:	2800      	cmp	r0, #0
30007964:	d0f8      	beq.n	30007958 <ymodem_get_first+0x40>
30007966:	2301      	movs	r3, #1
30007968:	2008      	movs	r0, #8
3000796a:	60a3      	str	r3, [r4, #8]
3000796c:	b003      	add	sp, #12
3000796e:	bd30      	pop	{r4, r5, pc}
30007970:	2301      	movs	r3, #1
30007972:	2002      	movs	r0, #2
30007974:	60a3      	str	r3, [r4, #8]
30007976:	b003      	add	sp, #12
30007978:	bd30      	pop	{r4, r5, pc}
3000797a:	2380      	movs	r3, #128	; 0x80
3000797c:	2001      	movs	r0, #1
3000797e:	80a3      	strh	r3, [r4, #4]
30007980:	b003      	add	sp, #12
30007982:	bd30      	pop	{r4, r5, pc}
30007984:	f44f 6380 	mov.w	r3, #1024	; 0x400
30007988:	2001      	movs	r0, #1
3000798a:	80a3      	strh	r3, [r4, #4]
3000798c:	b003      	add	sp, #12
3000798e:	bd30      	pop	{r4, r5, pc}
30007990:	2006      	movs	r0, #6
30007992:	4d1b      	ldr	r5, [pc, #108]	; (30007a00 <ymodem_get_first+0xe8>)
30007994:	f000 f9c2 	bl	30007d1c <ymodem_uart_putc>
30007998:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000799c:	47a8      	blx	r5
3000799e:	2043      	movs	r0, #67	; 0x43
300079a0:	f000 f9bc 	bl	30007d1c <ymodem_uart_putc>
300079a4:	2006      	movs	r0, #6
300079a6:	f000 f9b9 	bl	30007d1c <ymodem_uart_putc>
300079aa:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079ae:	47a8      	blx	r5
300079b0:	2043      	movs	r0, #67	; 0x43
300079b2:	f000 f9b3 	bl	30007d1c <ymodem_uart_putc>
300079b6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079ba:	47a8      	blx	r5
300079bc:	2018      	movs	r0, #24
300079be:	f000 f9ad 	bl	30007d1c <ymodem_uart_putc>
300079c2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079c6:	47a8      	blx	r5
300079c8:	2018      	movs	r0, #24
300079ca:	f000 f9a7 	bl	30007d1c <ymodem_uart_putc>
300079ce:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079d2:	47a8      	blx	r5
300079d4:	2018      	movs	r0, #24
300079d6:	f000 f9a1 	bl	30007d1c <ymodem_uart_putc>
300079da:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079de:	47a8      	blx	r5
300079e0:	2018      	movs	r0, #24
300079e2:	f000 f99b 	bl	30007d1c <ymodem_uart_putc>
300079e6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079ea:	47a8      	blx	r5
300079ec:	2018      	movs	r0, #24
300079ee:	f000 f995 	bl	30007d1c <ymodem_uart_putc>
300079f2:	2301      	movs	r3, #1
300079f4:	2005      	movs	r0, #5
300079f6:	60a3      	str	r3, [r4, #8]
300079f8:	b003      	add	sp, #12
300079fa:	bd30      	pop	{r4, r5, pc}
300079fc:	000caa30 	.word	0x000caa30
30007a00:	00009b2d 	.word	0x00009b2d

30007a04 <ymodem_get_others>:
30007a04:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007a08:	b083      	sub	sp, #12
30007a0a:	2300      	movs	r3, #0
30007a0c:	4605      	mov	r5, r0
30007a0e:	4952      	ldr	r1, [pc, #328]	; (30007b58 <ymodem_get_others+0x154>)
30007a10:	a801      	add	r0, sp, #4
30007a12:	f88d 3006 	strb.w	r3, [sp, #6]
30007a16:	f88d 3007 	strb.w	r3, [sp, #7]
30007a1a:	f000 f9a1 	bl	30007d60 <ymodem_uart_getc_to>
30007a1e:	bb38      	cbnz	r0, 30007a70 <ymodem_get_others+0x6c>
30007a20:	494d      	ldr	r1, [pc, #308]	; (30007b58 <ymodem_get_others+0x154>)
30007a22:	f10d 0005 	add.w	r0, sp, #5
30007a26:	f000 f99b 	bl	30007d60 <ymodem_uart_getc_to>
30007a2a:	4607      	mov	r7, r0
30007a2c:	bb00      	cbnz	r0, 30007a70 <ymodem_get_others+0x6c>
30007a2e:	f89d 2004 	ldrb.w	r2, [sp, #4]
30007a32:	f89d 6005 	ldrb.w	r6, [sp, #5]
30007a36:	43d3      	mvns	r3, r2
30007a38:	b2db      	uxtb	r3, r3
30007a3a:	429e      	cmp	r6, r3
30007a3c:	bf0c      	ite	eq
30007a3e:	2601      	moveq	r6, #1
30007a40:	2604      	movne	r6, #4
30007a42:	682b      	ldr	r3, [r5, #0]
30007a44:	429a      	cmp	r2, r3
30007a46:	d21a      	bcs.n	30007a7e <ymodem_get_others+0x7a>
30007a48:	2701      	movs	r7, #1
30007a4a:	88ab      	ldrh	r3, [r5, #4]
30007a4c:	b1d3      	cbz	r3, 30007a84 <ymodem_get_others+0x80>
30007a4e:	2400      	movs	r4, #0
30007a50:	f8df 9108 	ldr.w	r9, [pc, #264]	; 30007b5c <ymodem_get_others+0x158>
30007a54:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007b58 <ymodem_get_others+0x154>
30007a58:	e002      	b.n	30007a60 <ymodem_get_others+0x5c>
30007a5a:	88ab      	ldrh	r3, [r5, #4]
30007a5c:	42a3      	cmp	r3, r4
30007a5e:	d911      	bls.n	30007a84 <ymodem_get_others+0x80>
30007a60:	eb04 0009 	add.w	r0, r4, r9
30007a64:	4641      	mov	r1, r8
30007a66:	3401      	adds	r4, #1
30007a68:	f000 f97a 	bl	30007d60 <ymodem_uart_getc_to>
30007a6c:	2800      	cmp	r0, #0
30007a6e:	d0f4      	beq.n	30007a5a <ymodem_get_others+0x56>
30007a70:	2301      	movs	r3, #1
30007a72:	2608      	movs	r6, #8
30007a74:	60ab      	str	r3, [r5, #8]
30007a76:	4630      	mov	r0, r6
30007a78:	b003      	add	sp, #12
30007a7a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007a7e:	d0e4      	beq.n	30007a4a <ymodem_get_others+0x46>
30007a80:	2604      	movs	r6, #4
30007a82:	e7e2      	b.n	30007a4a <ymodem_get_others+0x46>
30007a84:	4934      	ldr	r1, [pc, #208]	; (30007b58 <ymodem_get_others+0x154>)
30007a86:	f10d 0006 	add.w	r0, sp, #6
30007a8a:	f000 f969 	bl	30007d60 <ymodem_uart_getc_to>
30007a8e:	2800      	cmp	r0, #0
30007a90:	d1ee      	bne.n	30007a70 <ymodem_get_others+0x6c>
30007a92:	4931      	ldr	r1, [pc, #196]	; (30007b58 <ymodem_get_others+0x154>)
30007a94:	f10d 0007 	add.w	r0, sp, #7
30007a98:	f000 f962 	bl	30007d60 <ymodem_uart_getc_to>
30007a9c:	2800      	cmp	r0, #0
30007a9e:	d1e7      	bne.n	30007a70 <ymodem_get_others+0x6c>
30007aa0:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007aa4:	f89d e007 	ldrb.w	lr, [sp, #7]
30007aa8:	88aa      	ldrh	r2, [r5, #4]
30007aaa:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
30007aae:	b36a      	cbz	r2, 30007b0c <ymodem_get_others+0x108>
30007ab0:	4603      	mov	r3, r0
30007ab2:	482a      	ldr	r0, [pc, #168]	; (30007b5c <ymodem_get_others+0x158>)
30007ab4:	f241 0c21 	movw	ip, #4129	; 0x1021
30007ab8:	eb02 0800 	add.w	r8, r2, r0
30007abc:	7801      	ldrb	r1, [r0, #0]
30007abe:	3001      	adds	r0, #1
30007ac0:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
30007ac4:	2108      	movs	r1, #8
30007ac6:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
30007aca:	005b      	lsls	r3, r3, #1
30007acc:	b10c      	cbz	r4, 30007ad2 <ymodem_get_others+0xce>
30007ace:	ea83 030c 	eor.w	r3, r3, ip
30007ad2:	3901      	subs	r1, #1
30007ad4:	d1f7      	bne.n	30007ac6 <ymodem_get_others+0xc2>
30007ad6:	4580      	cmp	r8, r0
30007ad8:	d1f0      	bne.n	30007abc <ymodem_get_others+0xb8>
30007ada:	b29b      	uxth	r3, r3
30007adc:	459e      	cmp	lr, r3
30007ade:	d00a      	beq.n	30007af6 <ymodem_get_others+0xf2>
30007ae0:	4b1f      	ldr	r3, [pc, #124]	; (30007b60 <ymodem_get_others+0x15c>)
30007ae2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007ae6:	4798      	blx	r3
30007ae8:	2015      	movs	r0, #21
30007aea:	f000 f917 	bl	30007d1c <ymodem_uart_putc>
30007aee:	2301      	movs	r3, #1
30007af0:	2604      	movs	r6, #4
30007af2:	60ab      	str	r3, [r5, #8]
30007af4:	e7bf      	b.n	30007a76 <ymodem_get_others+0x72>
30007af6:	b917      	cbnz	r7, 30007afe <ymodem_get_others+0xfa>
30007af8:	4c1a      	ldr	r4, [pc, #104]	; (30007b64 <ymodem_get_others+0x160>)
30007afa:	6823      	ldr	r3, [r4, #0]
30007afc:	b9b3      	cbnz	r3, 30007b2c <ymodem_get_others+0x128>
30007afe:	f000 f947 	bl	30007d90 <ymodem_uart_clean_rx>
30007b02:	2e01      	cmp	r6, #1
30007b04:	d004      	beq.n	30007b10 <ymodem_get_others+0x10c>
30007b06:	2e04      	cmp	r6, #4
30007b08:	d1b5      	bne.n	30007a76 <ymodem_get_others+0x72>
30007b0a:	e7e9      	b.n	30007ae0 <ymodem_get_others+0xdc>
30007b0c:	4613      	mov	r3, r2
30007b0e:	e7e5      	b.n	30007adc <ymodem_get_others+0xd8>
30007b10:	4c14      	ldr	r4, [pc, #80]	; (30007b64 <ymodem_get_others+0x160>)
30007b12:	2006      	movs	r0, #6
30007b14:	60ae      	str	r6, [r5, #8]
30007b16:	f000 f901 	bl	30007d1c <ymodem_uart_putc>
30007b1a:	6823      	ldr	r3, [r4, #0]
30007b1c:	b193      	cbz	r3, 30007b44 <ymodem_get_others+0x140>
30007b1e:	2f00      	cmp	r7, #0
30007b20:	f103 0301 	add.w	r3, r3, #1
30007b24:	bf18      	it	ne
30007b26:	2604      	movne	r6, #4
30007b28:	6023      	str	r3, [r4, #0]
30007b2a:	e7a4      	b.n	30007a76 <ymodem_get_others+0x72>
30007b2c:	68eb      	ldr	r3, [r5, #12]
30007b2e:	6861      	ldr	r1, [r4, #4]
30007b30:	3301      	adds	r3, #1
30007b32:	480a      	ldr	r0, [pc, #40]	; (30007b5c <ymodem_get_others+0x158>)
30007b34:	60eb      	str	r3, [r5, #12]
30007b36:	f000 f94f 	bl	30007dd8 <UARTIMG_Write>
30007b3a:	6862      	ldr	r2, [r4, #4]
30007b3c:	88ab      	ldrh	r3, [r5, #4]
30007b3e:	4413      	add	r3, r2
30007b40:	6063      	str	r3, [r4, #4]
30007b42:	e7dc      	b.n	30007afe <ymodem_get_others+0xfa>
30007b44:	4b06      	ldr	r3, [pc, #24]	; (30007b60 <ymodem_get_others+0x15c>)
30007b46:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007b4a:	4798      	blx	r3
30007b4c:	2043      	movs	r0, #67	; 0x43
30007b4e:	f000 f8e5 	bl	30007d1c <ymodem_uart_putc>
30007b52:	6823      	ldr	r3, [r4, #0]
30007b54:	e7e3      	b.n	30007b1e <ymodem_get_others+0x11a>
30007b56:	bf00      	nop
30007b58:	000caa30 	.word	0x000caa30
30007b5c:	20016ffc 	.word	0x20016ffc
30007b60:	00009b2d 	.word	0x00009b2d
30007b64:	3000e320 	.word	0x3000e320

30007b68 <ymodem_rx_frame>:
30007b68:	2300      	movs	r3, #0
30007b6a:	2201      	movs	r2, #1
30007b6c:	b510      	push	{r4, lr}
30007b6e:	4604      	mov	r4, r0
30007b70:	6083      	str	r3, [r0, #8]
30007b72:	8082      	strh	r2, [r0, #4]
30007b74:	f7ff fed0 	bl	30007918 <ymodem_get_first>
30007b78:	68a3      	ldr	r3, [r4, #8]
30007b7a:	b92b      	cbnz	r3, 30007b88 <ymodem_rx_frame+0x20>
30007b7c:	4620      	mov	r0, r4
30007b7e:	f7ff ff41 	bl	30007a04 <ymodem_get_others>
30007b82:	68a3      	ldr	r3, [r4, #8]
30007b84:	2b00      	cmp	r3, #0
30007b86:	d0f9      	beq.n	30007b7c <ymodem_rx_frame+0x14>
30007b88:	2801      	cmp	r0, #1
30007b8a:	d000      	beq.n	30007b8e <ymodem_rx_frame+0x26>
30007b8c:	bd10      	pop	{r4, pc}
30007b8e:	6823      	ldr	r3, [r4, #0]
30007b90:	3301      	adds	r3, #1
30007b92:	b2db      	uxtb	r3, r3
30007b94:	6023      	str	r3, [r4, #0]
30007b96:	bd10      	pop	{r4, pc}

30007b98 <ymodem_img_rxbuffer>:
30007b98:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007b9c:	2300      	movs	r3, #0
30007b9e:	4c28      	ldr	r4, [pc, #160]	; (30007c40 <ymodem_img_rxbuffer+0xa8>)
30007ba0:	4e28      	ldr	r6, [pc, #160]	; (30007c44 <ymodem_img_rxbuffer+0xac>)
30007ba2:	460d      	mov	r5, r1
30007ba4:	60a3      	str	r3, [r4, #8]
30007ba6:	6163      	str	r3, [r4, #20]
30007ba8:	f000 f8f2 	bl	30007d90 <ymodem_uart_clean_rx>
30007bac:	2043      	movs	r0, #67	; 0x43
30007bae:	f000 f8b5 	bl	30007d1c <ymodem_uart_putc>
30007bb2:	f24c 3050 	movw	r0, #50000	; 0xc350
30007bb6:	47b0      	blx	r6
30007bb8:	f000 f89a 	bl	30007cf0 <ymodem_uart_readable>
30007bbc:	2800      	cmp	r0, #0
30007bbe:	d0f5      	beq.n	30007bac <ymodem_img_rxbuffer+0x14>
30007bc0:	2700      	movs	r7, #0
30007bc2:	f04f 0801 	mov.w	r8, #1
30007bc6:	e9c4 7500 	strd	r7, r5, [r4]
30007bca:	4d1f      	ldr	r5, [pc, #124]	; (30007c48 <ymodem_img_rxbuffer+0xb0>)
30007bcc:	4628      	mov	r0, r5
30007bce:	f8a4 800c 	strh.w	r8, [r4, #12]
30007bd2:	6127      	str	r7, [r4, #16]
30007bd4:	f7ff fea0 	bl	30007918 <ymodem_get_first>
30007bd8:	6923      	ldr	r3, [r4, #16]
30007bda:	b92b      	cbnz	r3, 30007be8 <ymodem_img_rxbuffer+0x50>
30007bdc:	4628      	mov	r0, r5
30007bde:	f7ff ff11 	bl	30007a04 <ymodem_get_others>
30007be2:	6923      	ldr	r3, [r4, #16]
30007be4:	2b00      	cmp	r3, #0
30007be6:	d0f9      	beq.n	30007bdc <ymodem_img_rxbuffer+0x44>
30007be8:	2801      	cmp	r0, #1
30007bea:	d024      	beq.n	30007c36 <ymodem_img_rxbuffer+0x9e>
30007bec:	3804      	subs	r0, #4
30007bee:	2804      	cmp	r0, #4
30007bf0:	d80c      	bhi.n	30007c0c <ymodem_img_rxbuffer+0x74>
30007bf2:	a301      	add	r3, pc, #4	; (adr r3, 30007bf8 <ymodem_img_rxbuffer+0x60>)
30007bf4:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007bf8:	30007bcd 	.word	0x30007bcd
30007bfc:	30007c19 	.word	0x30007c19
30007c00:	30007c0d 	.word	0x30007c0d
30007c04:	30007c0d 	.word	0x30007c0d
30007c08:	30007c25 	.word	0x30007c25
30007c0c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007c10:	47b0      	blx	r6
30007c12:	2018      	movs	r0, #24
30007c14:	f000 f882 	bl	30007d1c <ymodem_uart_putc>
30007c18:	200a      	movs	r0, #10
30007c1a:	f000 f8c9 	bl	30007db0 <ymodem_uart_waitbusy>
30007c1e:	2001      	movs	r0, #1
30007c20:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007c24:	f000 f8b4 	bl	30007d90 <ymodem_uart_clean_rx>
30007c28:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007c2c:	47b0      	blx	r6
30007c2e:	2015      	movs	r0, #21
30007c30:	f000 f874 	bl	30007d1c <ymodem_uart_putc>
30007c34:	e7ca      	b.n	30007bcc <ymodem_img_rxbuffer+0x34>
30007c36:	68a3      	ldr	r3, [r4, #8]
30007c38:	3301      	adds	r3, #1
30007c3a:	b2db      	uxtb	r3, r3
30007c3c:	60a3      	str	r3, [r4, #8]
30007c3e:	e7c5      	b.n	30007bcc <ymodem_img_rxbuffer+0x34>
30007c40:	3000e320 	.word	0x3000e320
30007c44:	00009b2d 	.word	0x00009b2d
30007c48:	3000e328 	.word	0x3000e328

30007c4c <ymodem_log_open>:
30007c4c:	b510      	push	{r4, lr}
30007c4e:	2303      	movs	r3, #3
30007c50:	4c04      	ldr	r4, [pc, #16]	; (30007c64 <ymodem_log_open+0x18>)
30007c52:	2200      	movs	r2, #0
30007c54:	2108      	movs	r1, #8
30007c56:	4804      	ldr	r0, [pc, #16]	; (30007c68 <ymodem_log_open+0x1c>)
30007c58:	47a0      	blx	r4
30007c5a:	2008      	movs	r0, #8
30007c5c:	4b03      	ldr	r3, [pc, #12]	; (30007c6c <ymodem_log_open+0x20>)
30007c5e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007c62:	4718      	bx	r3
30007c64:	3000a4c9 	.word	0x3000a4c9
30007c68:	0000ed39 	.word	0x0000ed39
30007c6c:	0000e1bd 	.word	0x0000e1bd

30007c70 <ymodem_uart_port_init>:
30007c70:	b500      	push	{lr}
30007c72:	b083      	sub	sp, #12
30007c74:	2008      	movs	r0, #8
30007c76:	4b06      	ldr	r3, [pc, #24]	; (30007c90 <ymodem_uart_port_init+0x20>)
30007c78:	9201      	str	r2, [sp, #4]
30007c7a:	4798      	blx	r3
30007c7c:	4b05      	ldr	r3, [pc, #20]	; (30007c94 <ymodem_uart_port_init+0x24>)
30007c7e:	2008      	movs	r0, #8
30007c80:	4798      	blx	r3
30007c82:	9901      	ldr	r1, [sp, #4]
30007c84:	4804      	ldr	r0, [pc, #16]	; (30007c98 <ymodem_uart_port_init+0x28>)
30007c86:	4b05      	ldr	r3, [pc, #20]	; (30007c9c <ymodem_uart_port_init+0x2c>)
30007c88:	b003      	add	sp, #12
30007c8a:	f85d eb04 	ldr.w	lr, [sp], #4
30007c8e:	4718      	bx	r3
30007c90:	0000e1d9 	.word	0x0000e1d9
30007c94:	0000e2fd 	.word	0x0000e2fd
30007c98:	4200c000 	.word	0x4200c000
30007c9c:	0000a9e5 	.word	0x0000a9e5

30007ca0 <ymodem_uart_port_deinit>:
30007ca0:	2804      	cmp	r0, #4
30007ca2:	d004      	beq.n	30007cae <ymodem_uart_port_deinit+0xe>
30007ca4:	4b0d      	ldr	r3, [pc, #52]	; (30007cdc <ymodem_uart_port_deinit+0x3c>)
30007ca6:	681b      	ldr	r3, [r3, #0]
30007ca8:	039b      	lsls	r3, r3, #14
30007caa:	d40f      	bmi.n	30007ccc <ymodem_uart_port_deinit+0x2c>
30007cac:	4770      	bx	lr
30007cae:	2200      	movs	r2, #0
30007cb0:	2303      	movs	r3, #3
30007cb2:	2108      	movs	r1, #8
30007cb4:	480a      	ldr	r0, [pc, #40]	; (30007ce0 <ymodem_uart_port_deinit+0x40>)
30007cb6:	b510      	push	{r4, lr}
30007cb8:	4c0a      	ldr	r4, [pc, #40]	; (30007ce4 <ymodem_uart_port_deinit+0x44>)
30007cba:	47a0      	blx	r4
30007cbc:	4b0a      	ldr	r3, [pc, #40]	; (30007ce8 <ymodem_uart_port_deinit+0x48>)
30007cbe:	2008      	movs	r0, #8
30007cc0:	4798      	blx	r3
30007cc2:	4b06      	ldr	r3, [pc, #24]	; (30007cdc <ymodem_uart_port_deinit+0x3c>)
30007cc4:	681b      	ldr	r3, [r3, #0]
30007cc6:	039a      	lsls	r2, r3, #14
30007cc8:	d403      	bmi.n	30007cd2 <ymodem_uart_port_deinit+0x32>
30007cca:	bd10      	pop	{r4, pc}
30007ccc:	4807      	ldr	r0, [pc, #28]	; (30007cec <ymodem_uart_port_deinit+0x4c>)
30007cce:	f004 baab 	b.w	3000c228 <__DiagPrintf_veneer>
30007cd2:	4806      	ldr	r0, [pc, #24]	; (30007cec <ymodem_uart_port_deinit+0x4c>)
30007cd4:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007cd8:	f004 baa6 	b.w	3000c228 <__DiagPrintf_veneer>
30007cdc:	2001c00c 	.word	0x2001c00c
30007ce0:	0000ed39 	.word	0x0000ed39
30007ce4:	3000a4c9 	.word	0x3000a4c9
30007ce8:	0000e1bd 	.word	0x0000e1bd
30007cec:	3000d160 	.word	0x3000d160

30007cf0 <ymodem_uart_readable>:
30007cf0:	4b00      	ldr	r3, [pc, #0]	; (30007cf4 <ymodem_uart_readable+0x4>)
30007cf2:	4718      	bx	r3
30007cf4:	0000ac39 	.word	0x0000ac39

30007cf8 <ymodem_uart_writable>:
30007cf8:	4a03      	ldr	r2, [pc, #12]	; (30007d08 <ymodem_uart_writable+0x10>)
30007cfa:	b508      	push	{r3, lr}
30007cfc:	4b03      	ldr	r3, [pc, #12]	; (30007d0c <ymodem_uart_writable+0x14>)
30007cfe:	6810      	ldr	r0, [r2, #0]
30007d00:	4798      	blx	r3
30007d02:	b2c0      	uxtb	r0, r0
30007d04:	bd08      	pop	{r3, pc}
30007d06:	bf00      	nop
30007d08:	3000e338 	.word	0x3000e338
30007d0c:	0000d1a9 	.word	0x0000d1a9

30007d10 <ymodem_uart_getc>:
30007d10:	2000      	movs	r0, #0
30007d12:	4b01      	ldr	r3, [pc, #4]	; (30007d18 <ymodem_uart_getc+0x8>)
30007d14:	4718      	bx	r3
30007d16:	bf00      	nop
30007d18:	0000ac1d 	.word	0x0000ac1d

30007d1c <ymodem_uart_putc>:
30007d1c:	4b00      	ldr	r3, [pc, #0]	; (30007d20 <ymodem_uart_putc+0x4>)
30007d1e:	4718      	bx	r3
30007d20:	0000abb5 	.word	0x0000abb5

30007d24 <ymodem_uart_putdata>:
30007d24:	b149      	cbz	r1, 30007d3a <ymodem_uart_putdata+0x16>
30007d26:	b570      	push	{r4, r5, r6, lr}
30007d28:	4604      	mov	r4, r0
30007d2a:	1845      	adds	r5, r0, r1
30007d2c:	4e03      	ldr	r6, [pc, #12]	; (30007d3c <ymodem_uart_putdata+0x18>)
30007d2e:	f814 0b01 	ldrb.w	r0, [r4], #1
30007d32:	47b0      	blx	r6
30007d34:	42ac      	cmp	r4, r5
30007d36:	d1fa      	bne.n	30007d2e <ymodem_uart_putdata+0xa>
30007d38:	bd70      	pop	{r4, r5, r6, pc}
30007d3a:	4770      	bx	lr
30007d3c:	0000abb5 	.word	0x0000abb5

30007d40 <ymodem_uart_getdata>:
30007d40:	b151      	cbz	r1, 30007d58 <ymodem_uart_getdata+0x18>
30007d42:	b570      	push	{r4, r5, r6, lr}
30007d44:	4604      	mov	r4, r0
30007d46:	1845      	adds	r5, r0, r1
30007d48:	4e04      	ldr	r6, [pc, #16]	; (30007d5c <ymodem_uart_getdata+0x1c>)
30007d4a:	2001      	movs	r0, #1
30007d4c:	47b0      	blx	r6
30007d4e:	f804 0b01 	strb.w	r0, [r4], #1
30007d52:	42ac      	cmp	r4, r5
30007d54:	d1f9      	bne.n	30007d4a <ymodem_uart_getdata+0xa>
30007d56:	bd70      	pop	{r4, r5, r6, pc}
30007d58:	4770      	bx	lr
30007d5a:	bf00      	nop
30007d5c:	0000ac1d 	.word	0x0000ac1d

30007d60 <ymodem_uart_getc_to>:
30007d60:	b570      	push	{r4, r5, r6, lr}
30007d62:	4d09      	ldr	r5, [pc, #36]	; (30007d88 <ymodem_uart_getc_to+0x28>)
30007d64:	4606      	mov	r6, r0
30007d66:	460c      	mov	r4, r1
30007d68:	e001      	b.n	30007d6e <ymodem_uart_getc_to+0xe>
30007d6a:	3c01      	subs	r4, #1
30007d6c:	d009      	beq.n	30007d82 <ymodem_uart_getc_to+0x22>
30007d6e:	47a8      	blx	r5
30007d70:	2800      	cmp	r0, #0
30007d72:	d0fa      	beq.n	30007d6a <ymodem_uart_getc_to+0xa>
30007d74:	4b05      	ldr	r3, [pc, #20]	; (30007d8c <ymodem_uart_getc_to+0x2c>)
30007d76:	2000      	movs	r0, #0
30007d78:	4798      	blx	r3
30007d7a:	4603      	mov	r3, r0
30007d7c:	2000      	movs	r0, #0
30007d7e:	7033      	strb	r3, [r6, #0]
30007d80:	bd70      	pop	{r4, r5, r6, pc}
30007d82:	2001      	movs	r0, #1
30007d84:	bd70      	pop	{r4, r5, r6, pc}
30007d86:	bf00      	nop
30007d88:	0000ac39 	.word	0x0000ac39
30007d8c:	0000ac1d 	.word	0x0000ac1d

30007d90 <ymodem_uart_clean_rx>:
30007d90:	b538      	push	{r3, r4, r5, lr}
30007d92:	4c05      	ldr	r4, [pc, #20]	; (30007da8 <ymodem_uart_clean_rx+0x18>)
30007d94:	4d05      	ldr	r5, [pc, #20]	; (30007dac <ymodem_uart_clean_rx+0x1c>)
30007d96:	e000      	b.n	30007d9a <ymodem_uart_clean_rx+0xa>
30007d98:	47a8      	blx	r5
30007d9a:	47a0      	blx	r4
30007d9c:	4603      	mov	r3, r0
30007d9e:	2000      	movs	r0, #0
30007da0:	2b00      	cmp	r3, #0
30007da2:	d1f9      	bne.n	30007d98 <ymodem_uart_clean_rx+0x8>
30007da4:	bd38      	pop	{r3, r4, r5, pc}
30007da6:	bf00      	nop
30007da8:	0000ac39 	.word	0x0000ac39
30007dac:	0000ac1d 	.word	0x0000ac1d

30007db0 <ymodem_uart_waitbusy>:
30007db0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007db2:	2400      	movs	r4, #0
30007db4:	4e06      	ldr	r6, [pc, #24]	; (30007dd0 <ymodem_uart_waitbusy+0x20>)
30007db6:	4f07      	ldr	r7, [pc, #28]	; (30007dd4 <ymodem_uart_waitbusy+0x24>)
30007db8:	4605      	mov	r5, r0
30007dba:	e002      	b.n	30007dc2 <ymodem_uart_waitbusy+0x12>
30007dbc:	47b8      	blx	r7
30007dbe:	42ac      	cmp	r4, r5
30007dc0:	d804      	bhi.n	30007dcc <ymodem_uart_waitbusy+0x1c>
30007dc2:	6973      	ldr	r3, [r6, #20]
30007dc4:	3401      	adds	r4, #1
30007dc6:	2064      	movs	r0, #100	; 0x64
30007dc8:	03db      	lsls	r3, r3, #15
30007dca:	d5f7      	bpl.n	30007dbc <ymodem_uart_waitbusy+0xc>
30007dcc:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007dce:	bf00      	nop
30007dd0:	4200c000 	.word	0x4200c000
30007dd4:	00009b2d 	.word	0x00009b2d

30007dd8 <UARTIMG_Write>:
30007dd8:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007ddc:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007de0:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007de4:	d21d      	bcs.n	30007e22 <UARTIMG_Write+0x4a>
30007de6:	3a01      	subs	r2, #1
30007de8:	4f19      	ldr	r7, [pc, #100]	; (30007e50 <UARTIMG_Write+0x78>)
30007dea:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007e60 <UARTIMG_Write+0x88>
30007dee:	4688      	mov	r8, r1
30007df0:	1916      	adds	r6, r2, r4
30007df2:	4605      	mov	r5, r0
30007df4:	f8d9 3000 	ldr.w	r3, [r9]
30007df8:	403e      	ands	r6, r7
30007dfa:	4027      	ands	r7, r4
30007dfc:	42bb      	cmp	r3, r7
30007dfe:	d91b      	bls.n	30007e38 <UARTIMG_Write+0x60>
30007e00:	42b3      	cmp	r3, r6
30007e02:	d910      	bls.n	30007e26 <UARTIMG_Write+0x4e>
30007e04:	4e13      	ldr	r6, [pc, #76]	; (30007e54 <UARTIMG_Write+0x7c>)
30007e06:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007e0a:	4f13      	ldr	r7, [pc, #76]	; (30007e58 <UARTIMG_Write+0x80>)
30007e0c:	4446      	add	r6, r8
30007e0e:	eba3 0808 	sub.w	r8, r3, r8
30007e12:	eb08 0204 	add.w	r2, r8, r4
30007e16:	4620      	mov	r0, r4
30007e18:	3408      	adds	r4, #8
30007e1a:	2108      	movs	r1, #8
30007e1c:	47b8      	blx	r7
30007e1e:	42b4      	cmp	r4, r6
30007e20:	d1f7      	bne.n	30007e12 <UARTIMG_Write+0x3a>
30007e22:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007e26:	4631      	mov	r1, r6
30007e28:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007e2c:	2002      	movs	r0, #2
30007e2e:	4b0b      	ldr	r3, [pc, #44]	; (30007e5c <UARTIMG_Write+0x84>)
30007e30:	4798      	blx	r3
30007e32:	f8c9 6000 	str.w	r6, [r9]
30007e36:	e7e5      	b.n	30007e04 <UARTIMG_Write+0x2c>
30007e38:	4b08      	ldr	r3, [pc, #32]	; (30007e5c <UARTIMG_Write+0x84>)
30007e3a:	4639      	mov	r1, r7
30007e3c:	2002      	movs	r0, #2
30007e3e:	4798      	blx	r3
30007e40:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007e44:	42b3      	cmp	r3, r6
30007e46:	f8c9 3000 	str.w	r3, [r9]
30007e4a:	d8db      	bhi.n	30007e04 <UARTIMG_Write+0x2c>
30007e4c:	e7eb      	b.n	30007e26 <UARTIMG_Write+0x4e>
30007e4e:	bf00      	nop
30007e50:	fffff000 	.word	0xfffff000
30007e54:	f8000400 	.word	0xf8000400
30007e58:	0000a04d 	.word	0x0000a04d
30007e5c:	0000a16d 	.word	0x0000a16d
30007e60:	3000e33c 	.word	0x3000e33c

30007e64 <ymodem_img_download>:
30007e64:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007e68:	4616      	mov	r6, r2
30007e6a:	460d      	mov	r5, r1
30007e6c:	4a0a      	ldr	r2, [pc, #40]	; (30007e98 <ymodem_img_download+0x34>)
30007e6e:	2100      	movs	r1, #0
30007e70:	4680      	mov	r8, r0
30007e72:	f7ff fefd 	bl	30007c70 <ymodem_uart_port_init>
30007e76:	b156      	cbz	r6, 30007e8e <ymodem_img_download+0x2a>
30007e78:	3d04      	subs	r5, #4
30007e7a:	2400      	movs	r4, #0
30007e7c:	4f06      	ldr	r7, [pc, #24]	; (30007e98 <ymodem_img_download+0x34>)
30007e7e:	3401      	adds	r4, #1
30007e80:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007e84:	4638      	mov	r0, r7
30007e86:	f7ff fe87 	bl	30007b98 <ymodem_img_rxbuffer>
30007e8a:	42b4      	cmp	r4, r6
30007e8c:	d1f7      	bne.n	30007e7e <ymodem_img_download+0x1a>
30007e8e:	4640      	mov	r0, r8
30007e90:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007e94:	f7ff bf04 	b.w	30007ca0 <ymodem_uart_port_deinit>
30007e98:	0016e360 	.word	0x0016e360

30007e9c <ChipInfo_Invalid>:
30007e9c:	4d07      	ldr	r5, [pc, #28]	; (30007ebc <ChipInfo_Invalid+0x20>)
30007e9e:	4c08      	ldr	r4, [pc, #32]	; (30007ec0 <ChipInfo_Invalid+0x24>)
30007ea0:	4e08      	ldr	r6, [pc, #32]	; (30007ec4 <ChipInfo_Invalid+0x28>)
30007ea2:	b508      	push	{r3, lr}
30007ea4:	e002      	b.n	30007eac <ChipInfo_Invalid+0x10>
30007ea6:	f242 7010 	movw	r0, #10000	; 0x2710
30007eaa:	47a0      	blx	r4
30007eac:	682b      	ldr	r3, [r5, #0]
30007eae:	039b      	lsls	r3, r3, #14
30007eb0:	d5f9      	bpl.n	30007ea6 <ChipInfo_Invalid+0xa>
30007eb2:	4630      	mov	r0, r6
30007eb4:	f004 f9b8 	bl	3000c228 <__DiagPrintf_veneer>
30007eb8:	e7f5      	b.n	30007ea6 <ChipInfo_Invalid+0xa>
30007eba:	bf00      	nop
30007ebc:	2001c00c 	.word	0x2001c00c
30007ec0:	00009be5 	.word	0x00009be5
30007ec4:	3000d180 	.word	0x3000d180

30007ec8 <ChipInfo_Get>:
30007ec8:	b570      	push	{r4, r5, r6, lr}
30007eca:	4c11      	ldr	r4, [pc, #68]	; (30007f10 <ChipInfo_Get+0x48>)
30007ecc:	7820      	ldrb	r0, [r4, #0]
30007ece:	28ff      	cmp	r0, #255	; 0xff
30007ed0:	d104      	bne.n	30007edc <ChipInfo_Get+0x14>
30007ed2:	4d10      	ldr	r5, [pc, #64]	; (30007f14 <ChipInfo_Get+0x4c>)
30007ed4:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007ed8:	b108      	cbz	r0, 30007ede <ChipInfo_Get+0x16>
30007eda:	7020      	strb	r0, [r4, #0]
30007edc:	bd70      	pop	{r4, r5, r6, pc}
30007ede:	4621      	mov	r1, r4
30007ee0:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007ee4:	f001 f9c0 	bl	30009268 <OTP_Read8>
30007ee8:	7820      	ldrb	r0, [r4, #0]
30007eea:	28ff      	cmp	r0, #255	; 0xff
30007eec:	d002      	beq.n	30007ef4 <ChipInfo_Get+0x2c>
30007eee:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007ef2:	bd70      	pop	{r4, r5, r6, pc}
30007ef4:	4d08      	ldr	r5, [pc, #32]	; (30007f18 <ChipInfo_Get+0x50>)
30007ef6:	4c09      	ldr	r4, [pc, #36]	; (30007f1c <ChipInfo_Get+0x54>)
30007ef8:	4e09      	ldr	r6, [pc, #36]	; (30007f20 <ChipInfo_Get+0x58>)
30007efa:	e002      	b.n	30007f02 <ChipInfo_Get+0x3a>
30007efc:	f242 7010 	movw	r0, #10000	; 0x2710
30007f00:	47a0      	blx	r4
30007f02:	682b      	ldr	r3, [r5, #0]
30007f04:	039b      	lsls	r3, r3, #14
30007f06:	d5f9      	bpl.n	30007efc <ChipInfo_Get+0x34>
30007f08:	4630      	mov	r0, r6
30007f0a:	f004 f98d 	bl	3000c228 <__DiagPrintf_veneer>
30007f0e:	e7f5      	b.n	30007efc <ChipInfo_Get+0x34>
30007f10:	3000e438 	.word	0x3000e438
30007f14:	23020000 	.word	0x23020000
30007f18:	2001c00c 	.word	0x2001c00c
30007f1c:	00009be5 	.word	0x00009be5
30007f20:	3000d1a0 	.word	0x3000d1a0

30007f24 <ChipInfo_MemoryType>:
30007f24:	b510      	push	{r4, lr}
30007f26:	4c11      	ldr	r4, [pc, #68]	; (30007f6c <ChipInfo_MemoryType+0x48>)
30007f28:	47a0      	blx	r4
30007f2a:	2802      	cmp	r0, #2
30007f2c:	d019      	beq.n	30007f62 <ChipInfo_MemoryType+0x3e>
30007f2e:	47a0      	blx	r4
30007f30:	2803      	cmp	r0, #3
30007f32:	d016      	beq.n	30007f62 <ChipInfo_MemoryType+0x3e>
30007f34:	4c0e      	ldr	r4, [pc, #56]	; (30007f70 <ChipInfo_MemoryType+0x4c>)
30007f36:	4b0f      	ldr	r3, [pc, #60]	; (30007f74 <ChipInfo_MemoryType+0x50>)
30007f38:	4798      	blx	r3
30007f3a:	2100      	movs	r1, #0
30007f3c:	23e0      	movs	r3, #224	; 0xe0
30007f3e:	4622      	mov	r2, r4
30007f40:	e004      	b.n	30007f4c <ChipInfo_MemoryType+0x28>
30007f42:	7a13      	ldrb	r3, [r2, #8]
30007f44:	3101      	adds	r1, #1
30007f46:	3206      	adds	r2, #6
30007f48:	2bff      	cmp	r3, #255	; 0xff
30007f4a:	d00c      	beq.n	30007f66 <ChipInfo_MemoryType+0x42>
30007f4c:	4298      	cmp	r0, r3
30007f4e:	d1f8      	bne.n	30007f42 <ChipInfo_MemoryType+0x1e>
30007f50:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007f54:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007f58:	78cb      	ldrb	r3, [r1, #3]
30007f5a:	2b02      	cmp	r3, #2
30007f5c:	d001      	beq.n	30007f62 <ChipInfo_MemoryType+0x3e>
30007f5e:	2001      	movs	r0, #1
30007f60:	bd10      	pop	{r4, pc}
30007f62:	2002      	movs	r0, #2
30007f64:	bd10      	pop	{r4, pc}
30007f66:	f7ff ff99 	bl	30007e9c <ChipInfo_Invalid>
30007f6a:	bf00      	nop
30007f6c:	0000c0f9 	.word	0x0000c0f9
30007f70:	3000d1c4 	.word	0x3000d1c4
30007f74:	30007ec9 	.word	0x30007ec9

30007f78 <ChipInfo_DDRType>:
30007f78:	b510      	push	{r4, lr}
30007f7a:	4b0f      	ldr	r3, [pc, #60]	; (30007fb8 <ChipInfo_DDRType+0x40>)
30007f7c:	4798      	blx	r3
30007f7e:	4b0f      	ldr	r3, [pc, #60]	; (30007fbc <ChipInfo_DDRType+0x44>)
30007f80:	4604      	mov	r4, r0
30007f82:	4798      	blx	r3
30007f84:	2801      	cmp	r0, #1
30007f86:	d112      	bne.n	30007fae <ChipInfo_DDRType+0x36>
30007f88:	480d      	ldr	r0, [pc, #52]	; (30007fc0 <ChipInfo_DDRType+0x48>)
30007f8a:	2100      	movs	r1, #0
30007f8c:	22e0      	movs	r2, #224	; 0xe0
30007f8e:	4603      	mov	r3, r0
30007f90:	e003      	b.n	30007f9a <ChipInfo_DDRType+0x22>
30007f92:	789a      	ldrb	r2, [r3, #2]
30007f94:	3101      	adds	r1, #1
30007f96:	2aff      	cmp	r2, #255	; 0xff
30007f98:	d00b      	beq.n	30007fb2 <ChipInfo_DDRType+0x3a>
30007f9a:	4294      	cmp	r4, r2
30007f9c:	f103 0306 	add.w	r3, r3, #6
30007fa0:	d1f7      	bne.n	30007f92 <ChipInfo_DDRType+0x1a>
30007fa2:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007fa6:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007faa:	78c8      	ldrb	r0, [r1, #3]
30007fac:	bd10      	pop	{r4, pc}
30007fae:	20ff      	movs	r0, #255	; 0xff
30007fb0:	bd10      	pop	{r4, pc}
30007fb2:	2000      	movs	r0, #0
30007fb4:	bd10      	pop	{r4, pc}
30007fb6:	bf00      	nop
30007fb8:	30007ec9 	.word	0x30007ec9
30007fbc:	30007f25 	.word	0x30007f25
30007fc0:	3000d1c4 	.word	0x3000d1c4

30007fc4 <ChipInfo_MemorySize>:
30007fc4:	b510      	push	{r4, lr}
30007fc6:	4c0b      	ldr	r4, [pc, #44]	; (30007ff4 <ChipInfo_MemorySize+0x30>)
30007fc8:	4b0b      	ldr	r3, [pc, #44]	; (30007ff8 <ChipInfo_MemorySize+0x34>)
30007fca:	4798      	blx	r3
30007fcc:	2100      	movs	r1, #0
30007fce:	23e0      	movs	r3, #224	; 0xe0
30007fd0:	4622      	mov	r2, r4
30007fd2:	e004      	b.n	30007fde <ChipInfo_MemorySize+0x1a>
30007fd4:	7a13      	ldrb	r3, [r2, #8]
30007fd6:	3101      	adds	r1, #1
30007fd8:	3206      	adds	r2, #6
30007fda:	2bff      	cmp	r3, #255	; 0xff
30007fdc:	d007      	beq.n	30007fee <ChipInfo_MemorySize+0x2a>
30007fde:	4298      	cmp	r0, r3
30007fe0:	d1f8      	bne.n	30007fd4 <ChipInfo_MemorySize+0x10>
30007fe2:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007fe6:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007fea:	7908      	ldrb	r0, [r1, #4]
30007fec:	bd10      	pop	{r4, pc}
30007fee:	f7ff ff55 	bl	30007e9c <ChipInfo_Invalid>
30007ff2:	bf00      	nop
30007ff4:	3000d1c4 	.word	0x3000d1c4
30007ff8:	30007ec9 	.word	0x30007ec9

30007ffc <ChipInfo_MemoryVendor>:
30007ffc:	b510      	push	{r4, lr}
30007ffe:	4c0f      	ldr	r4, [pc, #60]	; (3000803c <ChipInfo_MemoryVendor+0x40>)
30008000:	47a0      	blx	r4
30008002:	2803      	cmp	r0, #3
30008004:	d016      	beq.n	30008034 <ChipInfo_MemoryVendor+0x38>
30008006:	47a0      	blx	r4
30008008:	2802      	cmp	r0, #2
3000800a:	d012      	beq.n	30008032 <ChipInfo_MemoryVendor+0x36>
3000800c:	4c0c      	ldr	r4, [pc, #48]	; (30008040 <ChipInfo_MemoryVendor+0x44>)
3000800e:	4b0d      	ldr	r3, [pc, #52]	; (30008044 <ChipInfo_MemoryVendor+0x48>)
30008010:	4798      	blx	r3
30008012:	2100      	movs	r1, #0
30008014:	23e0      	movs	r3, #224	; 0xe0
30008016:	4622      	mov	r2, r4
30008018:	e004      	b.n	30008024 <ChipInfo_MemoryVendor+0x28>
3000801a:	7a13      	ldrb	r3, [r2, #8]
3000801c:	3101      	adds	r1, #1
3000801e:	3206      	adds	r2, #6
30008020:	2bff      	cmp	r3, #255	; 0xff
30008022:	d009      	beq.n	30008038 <ChipInfo_MemoryVendor+0x3c>
30008024:	4298      	cmp	r0, r3
30008026:	d1f8      	bne.n	3000801a <ChipInfo_MemoryVendor+0x1e>
30008028:	eb01 0141 	add.w	r1, r1, r1, lsl #1
3000802c:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30008030:	7948      	ldrb	r0, [r1, #5]
30008032:	bd10      	pop	{r4, pc}
30008034:	2001      	movs	r0, #1
30008036:	bd10      	pop	{r4, pc}
30008038:	f7ff ff30 	bl	30007e9c <ChipInfo_Invalid>
3000803c:	0000c0f9 	.word	0x0000c0f9
30008040:	3000d1c4 	.word	0x3000d1c4
30008044:	30007ec9 	.word	0x30007ec9

30008048 <SDM32K_Enable>:
30008048:	4a02      	ldr	r2, [pc, #8]	; (30008054 <SDM32K_Enable+0xc>)
3000804a:	4b03      	ldr	r3, [pc, #12]	; (30008058 <SDM32K_Enable+0x10>)
3000804c:	6811      	ldr	r1, [r2, #0]
3000804e:	430b      	orrs	r3, r1
30008050:	6013      	str	r3, [r2, #0]
30008052:	4770      	bx	lr
30008054:	42008e00 	.word	0x42008e00
30008058:	c0060000 	.word	0xc0060000

3000805c <CLK_SWITCH_XTAL>:
3000805c:	b508      	push	{r3, lr}
3000805e:	2801      	cmp	r0, #1
30008060:	4b0a      	ldr	r3, [pc, #40]	; (3000808c <CLK_SWITCH_XTAL+0x30>)
30008062:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30008066:	d00d      	beq.n	30008084 <CLK_SWITCH_XTAL+0x28>
30008068:	4b09      	ldr	r3, [pc, #36]	; (30008090 <CLK_SWITCH_XTAL+0x34>)
3000806a:	4313      	orrs	r3, r2
3000806c:	4907      	ldr	r1, [pc, #28]	; (3000808c <CLK_SWITCH_XTAL+0x30>)
3000806e:	4a09      	ldr	r2, [pc, #36]	; (30008094 <CLK_SWITCH_XTAL+0x38>)
30008070:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30008074:	4790      	blx	r2
30008076:	4a08      	ldr	r2, [pc, #32]	; (30008098 <CLK_SWITCH_XTAL+0x3c>)
30008078:	4b08      	ldr	r3, [pc, #32]	; (3000809c <CLK_SWITCH_XTAL+0x40>)
3000807a:	fba2 2000 	umull	r2, r0, r2, r0
3000807e:	0c80      	lsrs	r0, r0, #18
30008080:	6018      	str	r0, [r3, #0]
30008082:	bd08      	pop	{r3, pc}
30008084:	4b06      	ldr	r3, [pc, #24]	; (300080a0 <CLK_SWITCH_XTAL+0x44>)
30008086:	4013      	ands	r3, r2
30008088:	e7f0      	b.n	3000806c <CLK_SWITCH_XTAL+0x10>
3000808a:	bf00      	nop
3000808c:	42008000 	.word	0x42008000
30008090:	80000500 	.word	0x80000500
30008094:	3000a199 	.word	0x3000a199
30008098:	431bde83 	.word	0x431bde83
3000809c:	2001c700 	.word	0x2001c700
300080a0:	7ffffaff 	.word	0x7ffffaff

300080a4 <DDR_PHY_ChipInfo_ddrtype>:
300080a4:	b510      	push	{r4, lr}
300080a6:	4c04      	ldr	r4, [pc, #16]	; (300080b8 <DDR_PHY_ChipInfo_ddrtype+0x14>)
300080a8:	7820      	ldrb	r0, [r4, #0]
300080aa:	b100      	cbz	r0, 300080ae <DDR_PHY_ChipInfo_ddrtype+0xa>
300080ac:	bd10      	pop	{r4, pc}
300080ae:	4b03      	ldr	r3, [pc, #12]	; (300080bc <DDR_PHY_ChipInfo_ddrtype+0x18>)
300080b0:	4798      	blx	r3
300080b2:	7020      	strb	r0, [r4, #0]
300080b4:	bd10      	pop	{r4, pc}
300080b6:	bf00      	nop
300080b8:	3000eae1 	.word	0x3000eae1
300080bc:	30007f79 	.word	0x30007f79

300080c0 <DDR_PHY_StructInit>:
300080c0:	f240 2315 	movw	r3, #533	; 0x215
300080c4:	6583      	str	r3, [r0, #88]	; 0x58
300080c6:	4770      	bx	lr

300080c8 <DDR_PHY_DLL_CLK_DIV>:
300080c8:	b570      	push	{r4, r5, r6, lr}
300080ca:	4d96      	ldr	r5, [pc, #600]	; (30008324 <DDR_PHY_DLL_CLK_DIV+0x25c>)
300080cc:	4b96      	ldr	r3, [pc, #600]	; (30008328 <DDR_PHY_DLL_CLK_DIV+0x260>)
300080ce:	fba5 4200 	umull	r4, r2, r5, r0
300080d2:	4996      	ldr	r1, [pc, #600]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
300080d4:	4403      	add	r3, r0
300080d6:	4c96      	ldr	r4, [pc, #600]	; (30008330 <DDR_PHY_DLL_CLK_DIV+0x268>)
300080d8:	0952      	lsrs	r2, r2, #5
300080da:	6a4e      	ldr	r6, [r1, #36]	; 0x24
300080dc:	4895      	ldr	r0, [pc, #596]	; (30008334 <DDR_PHY_DLL_CLK_DIV+0x26c>)
300080de:	3a03      	subs	r2, #3
300080e0:	4034      	ands	r4, r6
300080e2:	eb02 0c82 	add.w	ip, r2, r2, lsl #2
300080e6:	eba3 03cc 	sub.w	r3, r3, ip, lsl #3
300080ea:	02db      	lsls	r3, r3, #11
300080ec:	fba5 5303 	umull	r5, r3, r5, r3
300080f0:	2501      	movs	r5, #1
300080f2:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
300080f6:	f3c3 134a 	ubfx	r3, r3, #5, #11
300080fa:	4320      	orrs	r0, r4
300080fc:	0214      	lsls	r4, r2, #8
300080fe:	b2d2      	uxtb	r2, r2
30008100:	6248      	str	r0, [r1, #36]	; 0x24
30008102:	b2a4      	uxth	r4, r4
30008104:	6a88      	ldr	r0, [r1, #40]	; 0x28
30008106:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
3000810a:	4304      	orrs	r4, r0
3000810c:	488a      	ldr	r0, [pc, #552]	; (30008338 <DDR_PHY_DLL_CLK_DIV+0x270>)
3000810e:	628c      	str	r4, [r1, #40]	; 0x28
30008110:	6a4c      	ldr	r4, [r1, #36]	; 0x24
30008112:	4020      	ands	r0, r4
30008114:	4c89      	ldr	r4, [pc, #548]	; (3000833c <DDR_PHY_DLL_CLK_DIV+0x274>)
30008116:	4303      	orrs	r3, r0
30008118:	624b      	str	r3, [r1, #36]	; 0x24
3000811a:	6a8b      	ldr	r3, [r1, #40]	; 0x28
3000811c:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008120:	431a      	orrs	r2, r3
30008122:	628a      	str	r2, [r1, #40]	; 0x28
30008124:	678d      	str	r5, [r1, #120]	; 0x78
30008126:	7820      	ldrb	r0, [r4, #0]
30008128:	2800      	cmp	r0, #0
3000812a:	f000 8083 	beq.w	30008234 <DDR_PHY_DLL_CLK_DIV+0x16c>
3000812e:	28e0      	cmp	r0, #224	; 0xe0
30008130:	d15a      	bne.n	300081e8 <DDR_PHY_DLL_CLK_DIV+0x120>
30008132:	4b7e      	ldr	r3, [pc, #504]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
30008134:	2108      	movs	r1, #8
30008136:	4c82      	ldr	r4, [pc, #520]	; (30008340 <DDR_PHY_DLL_CLK_DIV+0x278>)
30008138:	4a82      	ldr	r2, [pc, #520]	; (30008344 <DDR_PHY_DLL_CLK_DIV+0x27c>)
3000813a:	4883      	ldr	r0, [pc, #524]	; (30008348 <DDR_PHY_DLL_CLK_DIV+0x280>)
3000813c:	611c      	str	r4, [r3, #16]
3000813e:	615a      	str	r2, [r3, #20]
30008140:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008144:	2022      	movs	r0, #34	; 0x22
30008146:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
3000814a:	f04f 3122 	mov.w	r1, #572662306	; 0x22222222
3000814e:	689c      	ldr	r4, [r3, #8]
30008150:	4a7e      	ldr	r2, [pc, #504]	; (3000834c <DDR_PHY_DLL_CLK_DIV+0x284>)
30008152:	4022      	ands	r2, r4
30008154:	f042 0201 	orr.w	r2, r2, #1
30008158:	609a      	str	r2, [r3, #8]
3000815a:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
3000815e:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30008162:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
30008166:	f8c3 1080 	str.w	r1, [r3, #128]	; 0x80
3000816a:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
3000816e:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008172:	f022 0277 	bic.w	r2, r2, #119	; 0x77
30008176:	4c76      	ldr	r4, [pc, #472]	; (30008350 <DDR_PHY_DLL_CLK_DIV+0x288>)
30008178:	f042 0212 	orr.w	r2, r2, #18
3000817c:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008180:	f8c3 0088 	str.w	r0, [r3, #136]	; 0x88
30008184:	f8c3 1094 	str.w	r1, [r3, #148]	; 0x94
30008188:	7820      	ldrb	r0, [r4, #0]
3000818a:	4a68      	ldr	r2, [pc, #416]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
3000818c:	4971      	ldr	r1, [pc, #452]	; (30008354 <DDR_PHY_DLL_CLK_DIV+0x28c>)
3000818e:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008192:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
30008196:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000819a:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
3000819e:	009b      	lsls	r3, r3, #2
300081a0:	d519      	bpl.n	300081d6 <DDR_PHY_DLL_CLK_DIV+0x10e>
300081a2:	2800      	cmp	r0, #0
300081a4:	d04e      	beq.n	30008244 <DDR_PHY_DLL_CLK_DIV+0x17c>
300081a6:	2801      	cmp	r0, #1
300081a8:	4b60      	ldr	r3, [pc, #384]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
300081aa:	bf0c      	ite	eq
300081ac:	4a6a      	ldreq	r2, [pc, #424]	; (30008358 <DDR_PHY_DLL_CLK_DIV+0x290>)
300081ae:	4a6b      	ldrne	r2, [pc, #428]	; (3000835c <DDR_PHY_DLL_CLK_DIV+0x294>)
300081b0:	601a      	str	r2, [r3, #0]
300081b2:	4b5e      	ldr	r3, [pc, #376]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
300081b4:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300081b8:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
300081bc:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
300081c0:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300081c4:	699a      	ldr	r2, [r3, #24]
300081c6:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
300081ca:	619a      	str	r2, [r3, #24]
300081cc:	699a      	ldr	r2, [r3, #24]
300081ce:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
300081d2:	619a      	str	r2, [r3, #24]
300081d4:	bd70      	pop	{r4, r5, r6, pc}
300081d6:	2800      	cmp	r0, #0
300081d8:	d030      	beq.n	3000823c <DDR_PHY_DLL_CLK_DIV+0x174>
300081da:	2801      	cmp	r0, #1
300081dc:	4b53      	ldr	r3, [pc, #332]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
300081de:	bf0c      	ite	eq
300081e0:	4a5f      	ldreq	r2, [pc, #380]	; (30008360 <DDR_PHY_DLL_CLK_DIV+0x298>)
300081e2:	4a60      	ldrne	r2, [pc, #384]	; (30008364 <DDR_PHY_DLL_CLK_DIV+0x29c>)
300081e4:	601a      	str	r2, [r3, #0]
300081e6:	e7e4      	b.n	300081b2 <DDR_PHY_DLL_CLK_DIV+0xea>
300081e8:	2800      	cmp	r0, #0
300081ea:	d062      	beq.n	300082b2 <DDR_PHY_DLL_CLK_DIV+0x1ea>
300081ec:	2802      	cmp	r0, #2
300081ee:	d12d      	bne.n	3000824c <DDR_PHY_DLL_CLK_DIV+0x184>
300081f0:	4b4e      	ldr	r3, [pc, #312]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
300081f2:	220e      	movs	r2, #14
300081f4:	495c      	ldr	r1, [pc, #368]	; (30008368 <DDR_PHY_DLL_CLK_DIV+0x2a0>)
300081f6:	485d      	ldr	r0, [pc, #372]	; (3000836c <DDR_PHY_DLL_CLK_DIV+0x2a4>)
300081f8:	4c5d      	ldr	r4, [pc, #372]	; (30008370 <DDR_PHY_DLL_CLK_DIV+0x2a8>)
300081fa:	611c      	str	r4, [r3, #16]
300081fc:	6159      	str	r1, [r3, #20]
300081fe:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008202:	4952      	ldr	r1, [pc, #328]	; (3000834c <DDR_PHY_DLL_CLK_DIV+0x284>)
30008204:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
30008208:	f240 32c1 	movw	r2, #961	; 0x3c1
3000820c:	6898      	ldr	r0, [r3, #8]
3000820e:	4001      	ands	r1, r0
30008210:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
30008214:	2022      	movs	r0, #34	; 0x22
30008216:	430a      	orrs	r2, r1
30008218:	609a      	str	r2, [r3, #8]
3000821a:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
3000821e:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
30008222:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30008226:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
3000822a:	f8c3 4080 	str.w	r4, [r3, #128]	; 0x80
3000822e:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008232:	e79e      	b.n	30008172 <DDR_PHY_DLL_CLK_DIV+0xaa>
30008234:	4b4f      	ldr	r3, [pc, #316]	; (30008374 <DDR_PHY_DLL_CLK_DIV+0x2ac>)
30008236:	4798      	blx	r3
30008238:	7020      	strb	r0, [r4, #0]
3000823a:	e778      	b.n	3000812e <DDR_PHY_DLL_CLK_DIV+0x66>
3000823c:	4b4e      	ldr	r3, [pc, #312]	; (30008378 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
3000823e:	4798      	blx	r3
30008240:	7020      	strb	r0, [r4, #0]
30008242:	e7ca      	b.n	300081da <DDR_PHY_DLL_CLK_DIV+0x112>
30008244:	4b4c      	ldr	r3, [pc, #304]	; (30008378 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
30008246:	4798      	blx	r3
30008248:	7020      	strb	r0, [r4, #0]
3000824a:	e7ac      	b.n	300081a6 <DDR_PHY_DLL_CLK_DIV+0xde>
3000824c:	2805      	cmp	r0, #5
3000824e:	d03b      	beq.n	300082c8 <DDR_PHY_DLL_CLK_DIV+0x200>
30008250:	2803      	cmp	r0, #3
30008252:	f040 80e6 	bne.w	30008422 <DDR_PHY_DLL_CLK_DIV+0x35a>
30008256:	4a35      	ldr	r2, [pc, #212]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
30008258:	230e      	movs	r3, #14
3000825a:	4c48      	ldr	r4, [pc, #288]	; (3000837c <DDR_PHY_DLL_CLK_DIV+0x2b4>)
3000825c:	4948      	ldr	r1, [pc, #288]	; (30008380 <DDR_PHY_DLL_CLK_DIV+0x2b8>)
3000825e:	4849      	ldr	r0, [pc, #292]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x2bc>)
30008260:	6114      	str	r4, [r2, #16]
30008262:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
30008266:	6151      	str	r1, [r2, #20]
30008268:	f8c2 0098 	str.w	r0, [r2, #152]	; 0x98
3000826c:	4937      	ldr	r1, [pc, #220]	; (3000834c <DDR_PHY_DLL_CLK_DIV+0x284>)
3000826e:	f8c2 30a0 	str.w	r3, [r2, #160]	; 0xa0
30008272:	f240 33c1 	movw	r3, #961	; 0x3c1
30008276:	6890      	ldr	r0, [r2, #8]
30008278:	4001      	ands	r1, r0
3000827a:	2033      	movs	r0, #51	; 0x33
3000827c:	430b      	orrs	r3, r1
3000827e:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
30008282:	6093      	str	r3, [r2, #8]
30008284:	f8d2 309c 	ldr.w	r3, [r2, #156]	; 0x9c
30008288:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
3000828c:	f8c2 309c 	str.w	r3, [r2, #156]	; 0x9c
30008290:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
30008294:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008298:	4c2d      	ldr	r4, [pc, #180]	; (30008350 <DDR_PHY_DLL_CLK_DIV+0x288>)
3000829a:	f023 0377 	bic.w	r3, r3, #119	; 0x77
3000829e:	f043 0312 	orr.w	r3, r3, #18
300082a2:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300082a6:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
300082aa:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
300082ae:	7820      	ldrb	r0, [r4, #0]
300082b0:	e76b      	b.n	3000818a <DDR_PHY_DLL_CLK_DIV+0xc2>
300082b2:	4d30      	ldr	r5, [pc, #192]	; (30008374 <DDR_PHY_DLL_CLK_DIV+0x2ac>)
300082b4:	47a8      	blx	r5
300082b6:	2802      	cmp	r0, #2
300082b8:	7020      	strb	r0, [r4, #0]
300082ba:	d099      	beq.n	300081f0 <DDR_PHY_DLL_CLK_DIV+0x128>
300082bc:	2800      	cmp	r0, #0
300082be:	d1c5      	bne.n	3000824c <DDR_PHY_DLL_CLK_DIV+0x184>
300082c0:	47a8      	blx	r5
300082c2:	2805      	cmp	r0, #5
300082c4:	7020      	strb	r0, [r4, #0]
300082c6:	d163      	bne.n	30008390 <DDR_PHY_DLL_CLK_DIV+0x2c8>
300082c8:	4918      	ldr	r1, [pc, #96]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x264>)
300082ca:	230e      	movs	r3, #14
300082cc:	4c2e      	ldr	r4, [pc, #184]	; (30008388 <DDR_PHY_DLL_CLK_DIV+0x2c0>)
300082ce:	4a2f      	ldr	r2, [pc, #188]	; (3000838c <DDR_PHY_DLL_CLK_DIV+0x2c4>)
300082d0:	4826      	ldr	r0, [pc, #152]	; (3000836c <DDR_PHY_DLL_CLK_DIV+0x2a4>)
300082d2:	610c      	str	r4, [r1, #16]
300082d4:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
300082d8:	614a      	str	r2, [r1, #20]
300082da:	f8c1 0098 	str.w	r0, [r1, #152]	; 0x98
300082de:	4a1b      	ldr	r2, [pc, #108]	; (3000834c <DDR_PHY_DLL_CLK_DIV+0x284>)
300082e0:	f8c1 30a0 	str.w	r3, [r1, #160]	; 0xa0
300082e4:	f240 33c1 	movw	r3, #961	; 0x3c1
300082e8:	6888      	ldr	r0, [r1, #8]
300082ea:	4002      	ands	r2, r0
300082ec:	2033      	movs	r0, #51	; 0x33
300082ee:	4313      	orrs	r3, r2
300082f0:	f04f 5250 	mov.w	r2, #872415232	; 0x34000000
300082f4:	608b      	str	r3, [r1, #8]
300082f6:	f8d1 309c 	ldr.w	r3, [r1, #156]	; 0x9c
300082fa:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
300082fe:	f8c1 309c 	str.w	r3, [r1, #156]	; 0x9c
30008302:	f8c1 4080 	str.w	r4, [r1, #128]	; 0x80
30008306:	f8d1 3084 	ldr.w	r3, [r1, #132]	; 0x84
3000830a:	4c11      	ldr	r4, [pc, #68]	; (30008350 <DDR_PHY_DLL_CLK_DIV+0x288>)
3000830c:	f023 0377 	bic.w	r3, r3, #119	; 0x77
30008310:	f043 0312 	orr.w	r3, r3, #18
30008314:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30008318:	f8c1 0088 	str.w	r0, [r1, #136]	; 0x88
3000831c:	f8c1 2094 	str.w	r2, [r1, #148]	; 0x94
30008320:	7820      	ldrb	r0, [r4, #0]
30008322:	e732      	b.n	3000818a <DDR_PHY_DLL_CLK_DIV+0xc2>
30008324:	cccccccd 	.word	0xcccccccd
30008328:	001fff88 	.word	0x001fff88
3000832c:	41011000 	.word	0x41011000
30008330:	f800ffff 	.word	0xf800ffff
30008334:	07ff0000 	.word	0x07ff0000
30008338:	fffff800 	.word	0xfffff800
3000833c:	3000eae0 	.word	0x3000eae0
30008340:	07060012 	.word	0x07060012
30008344:	000f0707 	.word	0x000f0707
30008348:	080f0f0f 	.word	0x080f0f0f
3000834c:	ffffe000 	.word	0xffffe000
30008350:	3000eae1 	.word	0x3000eae1
30008354:	42008000 	.word	0x42008000
30008358:	4c000903 	.word	0x4c000903
3000835c:	4c000821 	.word	0x4c000821
30008360:	40000103 	.word	0x40000103
30008364:	40000021 	.word	0x40000021
30008368:	00130b0b 	.word	0x00130b0b
3000836c:	0e131313 	.word	0x0e131313
30008370:	0b0b0016 	.word	0x0b0b0016
30008374:	30007ec9 	.word	0x30007ec9
30008378:	30007f79 	.word	0x30007f79
3000837c:	0e0e0018 	.word	0x0e0e0018
30008380:	00170e0e 	.word	0x00170e0e
30008384:	0e171717 	.word	0x0e171717
30008388:	0a0a0018 	.word	0x0a0a0018
3000838c:	00130a0a 	.word	0x00130a0a
30008390:	2800      	cmp	r0, #0
30008392:	f47f af5d 	bne.w	30008250 <DDR_PHY_DLL_CLK_DIV+0x188>
30008396:	47a8      	blx	r5
30008398:	2803      	cmp	r0, #3
3000839a:	7020      	strb	r0, [r4, #0]
3000839c:	f43f af5b 	beq.w	30008256 <DDR_PHY_DLL_CLK_DIV+0x18e>
300083a0:	2800      	cmp	r0, #0
300083a2:	d13e      	bne.n	30008422 <DDR_PHY_DLL_CLK_DIV+0x35a>
300083a4:	47a8      	blx	r5
300083a6:	280a      	cmp	r0, #10
300083a8:	7020      	strb	r0, [r4, #0]
300083aa:	d03c      	beq.n	30008426 <DDR_PHY_DLL_CLK_DIV+0x35e>
300083ac:	b908      	cbnz	r0, 300083b2 <DDR_PHY_DLL_CLK_DIV+0x2ea>
300083ae:	47a8      	blx	r5
300083b0:	7020      	strb	r0, [r4, #0]
300083b2:	2809      	cmp	r0, #9
300083b4:	d04c      	beq.n	30008450 <DDR_PHY_DLL_CLK_DIV+0x388>
300083b6:	4b32      	ldr	r3, [pc, #200]	; (30008480 <DDR_PHY_DLL_CLK_DIV+0x3b8>)
300083b8:	681b      	ldr	r3, [r3, #0]
300083ba:	079a      	lsls	r2, r3, #30
300083bc:	d45c      	bmi.n	30008478 <DDR_PHY_DLL_CLK_DIV+0x3b0>
300083be:	4b31      	ldr	r3, [pc, #196]	; (30008484 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300083c0:	2100      	movs	r1, #0
300083c2:	f44f 2400 	mov.w	r4, #524288	; 0x80000
300083c6:	4830      	ldr	r0, [pc, #192]	; (30008488 <DDR_PHY_DLL_CLK_DIV+0x3c0>)
300083c8:	6119      	str	r1, [r3, #16]
300083ca:	4a30      	ldr	r2, [pc, #192]	; (3000848c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
300083cc:	615c      	str	r4, [r3, #20]
300083ce:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
300083d2:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
300083d6:	6899      	ldr	r1, [r3, #8]
300083d8:	4c2d      	ldr	r4, [pc, #180]	; (30008490 <DDR_PHY_DLL_CLK_DIV+0x3c8>)
300083da:	400a      	ands	r2, r1
300083dc:	609a      	str	r2, [r3, #8]
300083de:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300083e2:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300083e6:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300083ea:	7820      	ldrb	r0, [r4, #0]
300083ec:	2800      	cmp	r0, #0
300083ee:	d03f      	beq.n	30008470 <DDR_PHY_DLL_CLK_DIV+0x3a8>
300083f0:	2801      	cmp	r0, #1
300083f2:	4b24      	ldr	r3, [pc, #144]	; (30008484 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300083f4:	f04f 0100 	mov.w	r1, #0
300083f8:	bf0c      	ite	eq
300083fa:	2233      	moveq	r2, #51	; 0x33
300083fc:	2222      	movne	r2, #34	; 0x22
300083fe:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
30008402:	f04f 3333 	mov.w	r3, #858993459	; 0x33333333
30008406:	4a1f      	ldr	r2, [pc, #124]	; (30008484 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008408:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000840c:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008410:	f023 0377 	bic.w	r3, r3, #119	; 0x77
30008414:	f043 0322 	orr.w	r3, r3, #34	; 0x22
30008418:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000841c:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
30008420:	e6b3      	b.n	3000818a <DDR_PHY_DLL_CLK_DIV+0xc2>
30008422:	280a      	cmp	r0, #10
30008424:	d1c5      	bne.n	300083b2 <DDR_PHY_DLL_CLK_DIV+0x2ea>
30008426:	4b17      	ldr	r3, [pc, #92]	; (30008484 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008428:	220e      	movs	r2, #14
3000842a:	4c1a      	ldr	r4, [pc, #104]	; (30008494 <DDR_PHY_DLL_CLK_DIV+0x3cc>)
3000842c:	491a      	ldr	r1, [pc, #104]	; (30008498 <DDR_PHY_DLL_CLK_DIV+0x3d0>)
3000842e:	481b      	ldr	r0, [pc, #108]	; (3000849c <DDR_PHY_DLL_CLK_DIV+0x3d4>)
30008430:	611c      	str	r4, [r3, #16]
30008432:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
30008436:	6159      	str	r1, [r3, #20]
30008438:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
3000843c:	4913      	ldr	r1, [pc, #76]	; (3000848c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
3000843e:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
30008442:	f240 32c1 	movw	r2, #961	; 0x3c1
30008446:	6898      	ldr	r0, [r3, #8]
30008448:	4001      	ands	r1, r0
3000844a:	2033      	movs	r0, #51	; 0x33
3000844c:	430a      	orrs	r2, r1
3000844e:	e6e3      	b.n	30008218 <DDR_PHY_DLL_CLK_DIV+0x150>
30008450:	4b0c      	ldr	r3, [pc, #48]	; (30008484 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008452:	2212      	movs	r2, #18
30008454:	4912      	ldr	r1, [pc, #72]	; (300084a0 <DDR_PHY_DLL_CLK_DIV+0x3d8>)
30008456:	4813      	ldr	r0, [pc, #76]	; (300084a4 <DDR_PHY_DLL_CLK_DIV+0x3dc>)
30008458:	4c13      	ldr	r4, [pc, #76]	; (300084a8 <DDR_PHY_DLL_CLK_DIV+0x3e0>)
3000845a:	611c      	str	r4, [r3, #16]
3000845c:	6159      	str	r1, [r3, #20]
3000845e:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008462:	490a      	ldr	r1, [pc, #40]	; (3000848c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
30008464:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
30008468:	f640 72c1 	movw	r2, #4033	; 0xfc1
3000846c:	6898      	ldr	r0, [r3, #8]
3000846e:	e6ce      	b.n	3000820e <DDR_PHY_DLL_CLK_DIV+0x146>
30008470:	4b0e      	ldr	r3, [pc, #56]	; (300084ac <DDR_PHY_DLL_CLK_DIV+0x3e4>)
30008472:	4798      	blx	r3
30008474:	7020      	strb	r0, [r4, #0]
30008476:	e7bb      	b.n	300083f0 <DDR_PHY_DLL_CLK_DIV+0x328>
30008478:	480d      	ldr	r0, [pc, #52]	; (300084b0 <DDR_PHY_DLL_CLK_DIV+0x3e8>)
3000847a:	f003 fed5 	bl	3000c228 <__DiagPrintf_veneer>
3000847e:	e79e      	b.n	300083be <DDR_PHY_DLL_CLK_DIV+0x2f6>
30008480:	2001c00c 	.word	0x2001c00c
30008484:	41011000 	.word	0x41011000
30008488:	00080808 	.word	0x00080808
3000848c:	ffffe000 	.word	0xffffe000
30008490:	3000eae1 	.word	0x3000eae1
30008494:	0f0f0016 	.word	0x0f0f0016
30008498:	00180f0f 	.word	0x00180f0f
3000849c:	0e181818 	.word	0x0e181818
300084a0:	00170e0e 	.word	0x00170e0e
300084a4:	12171717 	.word	0x12171717
300084a8:	0e0e001a 	.word	0x0e0e001a
300084ac:	30007f79 	.word	0x30007f79
300084b0:	3000d244 	.word	0x3000d244

300084b4 <DDR_PHY_CRT_Init>:
300084b4:	492f      	ldr	r1, [pc, #188]	; (30008574 <DDR_PHY_CRT_Init+0xc0>)
300084b6:	2005      	movs	r0, #5
300084b8:	4a2f      	ldr	r2, [pc, #188]	; (30008578 <DDR_PHY_CRT_Init+0xc4>)
300084ba:	b510      	push	{r4, lr}
300084bc:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
300084c0:	4c2e      	ldr	r4, [pc, #184]	; (3000857c <DDR_PHY_CRT_Init+0xc8>)
300084c2:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300084c6:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
300084ca:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300084ce:	f043 0301 	orr.w	r3, r3, #1
300084d2:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300084d6:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300084da:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300084de:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300084e2:	68e3      	ldr	r3, [r4, #12]
300084e4:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
300084e8:	60e3      	str	r3, [r4, #12]
300084ea:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300084ee:	f043 0302 	orr.w	r3, r3, #2
300084f2:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300084f6:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300084fa:	f023 0308 	bic.w	r3, r3, #8
300084fe:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008502:	4790      	blx	r2
30008504:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008508:	f641 73ff 	movw	r3, #8191	; 0x1fff
3000850c:	491c      	ldr	r1, [pc, #112]	; (30008580 <DDR_PHY_CRT_Init+0xcc>)
3000850e:	f042 0204 	orr.w	r2, r2, #4
30008512:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
30008516:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
3000851a:	f042 0208 	orr.w	r2, r2, #8
3000851e:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
30008522:	6aa2      	ldr	r2, [r4, #40]	; 0x28
30008524:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
30008528:	62a2      	str	r2, [r4, #40]	; 0x28
3000852a:	6aa2      	ldr	r2, [r4, #40]	; 0x28
3000852c:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
30008530:	62a2      	str	r2, [r4, #40]	; 0x28
30008532:	69e2      	ldr	r2, [r4, #28]
30008534:	f042 0208 	orr.w	r2, r2, #8
30008538:	61e2      	str	r2, [r4, #28]
3000853a:	68a2      	ldr	r2, [r4, #8]
3000853c:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008540:	60a2      	str	r2, [r4, #8]
30008542:	6862      	ldr	r2, [r4, #4]
30008544:	4313      	orrs	r3, r2
30008546:	6063      	str	r3, [r4, #4]
30008548:	68a3      	ldr	r3, [r4, #8]
3000854a:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
3000854e:	60a3      	str	r3, [r4, #8]
30008550:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
30008554:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008558:	f023 0303 	bic.w	r3, r3, #3
3000855c:	f043 0302 	orr.w	r3, r3, #2
30008560:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008564:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008568:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000856c:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008570:	bd10      	pop	{r4, pc}
30008572:	bf00      	nop
30008574:	42008000 	.word	0x42008000
30008578:	00009b2d 	.word	0x00009b2d
3000857c:	41011000 	.word	0x41011000
30008580:	90161f00 	.word	0x90161f00

30008584 <DDR_PHY_R240_ZQ_CAL>:
30008584:	b538      	push	{r3, r4, r5, lr}
30008586:	4c84      	ldr	r4, [pc, #528]	; (30008798 <DDR_PHY_R240_ZQ_CAL+0x214>)
30008588:	7823      	ldrb	r3, [r4, #0]
3000858a:	2b00      	cmp	r3, #0
3000858c:	d05e      	beq.n	3000864c <DDR_PHY_R240_ZQ_CAL+0xc8>
3000858e:	4c83      	ldr	r4, [pc, #524]	; (3000879c <DDR_PHY_R240_ZQ_CAL+0x218>)
30008590:	2b03      	cmp	r3, #3
30008592:	7820      	ldrb	r0, [r4, #0]
30008594:	d136      	bne.n	30008604 <DDR_PHY_R240_ZQ_CAL+0x80>
30008596:	2800      	cmp	r0, #0
30008598:	d155      	bne.n	30008646 <DDR_PHY_R240_ZQ_CAL+0xc2>
3000859a:	4d81      	ldr	r5, [pc, #516]	; (300087a0 <DDR_PHY_R240_ZQ_CAL+0x21c>)
3000859c:	47a8      	blx	r5
3000859e:	28e0      	cmp	r0, #224	; 0xe0
300085a0:	7020      	strb	r0, [r4, #0]
300085a2:	f040 808f 	bne.w	300086c4 <DDR_PHY_R240_ZQ_CAL+0x140>
300085a6:	4b7f      	ldr	r3, [pc, #508]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
300085a8:	2100      	movs	r1, #0
300085aa:	4c7f      	ldr	r4, [pc, #508]	; (300087a8 <DDR_PHY_R240_ZQ_CAL+0x224>)
300085ac:	487f      	ldr	r0, [pc, #508]	; (300087ac <DDR_PHY_R240_ZQ_CAL+0x228>)
300085ae:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
300085b2:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
300085b6:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
300085ba:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300085be:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
300085c2:	f504 2432 	add.w	r4, r4, #729088	; 0xb2000
300085c6:	4a7a      	ldr	r2, [pc, #488]	; (300087b0 <DDR_PHY_R240_ZQ_CAL+0x22c>)
300085c8:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
300085cc:	f204 241a 	addw	r4, r4, #538	; 0x21a
300085d0:	f200 201b 	addw	r0, r0, #539	; 0x21b
300085d4:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
300085d8:	f8c3 4190 	str.w	r4, [r3, #400]	; 0x190
300085dc:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300085e0:	f8c3 0198 	str.w	r0, [r3, #408]	; 0x198
300085e4:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300085e8:	4b6e      	ldr	r3, [pc, #440]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
300085ea:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300085ee:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
300085f2:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300085f6:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300085fa:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
300085fe:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008602:	bd38      	pop	{r3, r4, r5, pc}
30008604:	2800      	cmp	r0, #0
30008606:	d040      	beq.n	3000868a <DDR_PHY_R240_ZQ_CAL+0x106>
30008608:	2805      	cmp	r0, #5
3000860a:	d124      	bne.n	30008656 <DDR_PHY_R240_ZQ_CAL+0xd2>
3000860c:	4a65      	ldr	r2, [pc, #404]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000860e:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
30008612:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
30008616:	f043 0306 	orr.w	r3, r3, #6
3000861a:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
3000861e:	4b61      	ldr	r3, [pc, #388]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
30008620:	2100      	movs	r1, #0
30008622:	4c64      	ldr	r4, [pc, #400]	; (300087b4 <DDR_PHY_R240_ZQ_CAL+0x230>)
30008624:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008628:	4863      	ldr	r0, [pc, #396]	; (300087b8 <DDR_PHY_R240_ZQ_CAL+0x234>)
3000862a:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
3000862e:	f104 54e0 	add.w	r4, r4, #469762048	; 0x1c000000
30008632:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
30008636:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
3000863a:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
3000863e:	485f      	ldr	r0, [pc, #380]	; (300087bc <DDR_PHY_R240_ZQ_CAL+0x238>)
30008640:	f504 6422 	add.w	r4, r4, #2592	; 0xa20
30008644:	e7c6      	b.n	300085d4 <DDR_PHY_R240_ZQ_CAL+0x50>
30008646:	28e0      	cmp	r0, #224	; 0xe0
30008648:	d0ad      	beq.n	300085a6 <DDR_PHY_R240_ZQ_CAL+0x22>
3000864a:	e03e      	b.n	300086ca <DDR_PHY_R240_ZQ_CAL+0x146>
3000864c:	4b5c      	ldr	r3, [pc, #368]	; (300087c0 <DDR_PHY_R240_ZQ_CAL+0x23c>)
3000864e:	4798      	blx	r3
30008650:	4603      	mov	r3, r0
30008652:	7020      	strb	r0, [r4, #0]
30008654:	e79b      	b.n	3000858e <DDR_PHY_R240_ZQ_CAL+0xa>
30008656:	2803      	cmp	r0, #3
30008658:	d125      	bne.n	300086a6 <DDR_PHY_R240_ZQ_CAL+0x122>
3000865a:	4a52      	ldr	r2, [pc, #328]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000865c:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
30008660:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
30008664:	f043 030c 	orr.w	r3, r3, #12
30008668:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
3000866c:	4b4d      	ldr	r3, [pc, #308]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000866e:	2100      	movs	r1, #0
30008670:	4c54      	ldr	r4, [pc, #336]	; (300087c4 <DDR_PHY_R240_ZQ_CAL+0x240>)
30008672:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008676:	4854      	ldr	r0, [pc, #336]	; (300087c8 <DDR_PHY_R240_ZQ_CAL+0x244>)
30008678:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
3000867c:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
30008680:	4c52      	ldr	r4, [pc, #328]	; (300087cc <DDR_PHY_R240_ZQ_CAL+0x248>)
30008682:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008686:	4852      	ldr	r0, [pc, #328]	; (300087d0 <DDR_PHY_R240_ZQ_CAL+0x24c>)
30008688:	e7a4      	b.n	300085d4 <DDR_PHY_R240_ZQ_CAL+0x50>
3000868a:	4d45      	ldr	r5, [pc, #276]	; (300087a0 <DDR_PHY_R240_ZQ_CAL+0x21c>)
3000868c:	47a8      	blx	r5
3000868e:	2805      	cmp	r0, #5
30008690:	7020      	strb	r0, [r4, #0]
30008692:	d0bb      	beq.n	3000860c <DDR_PHY_R240_ZQ_CAL+0x88>
30008694:	2800      	cmp	r0, #0
30008696:	d1de      	bne.n	30008656 <DDR_PHY_R240_ZQ_CAL+0xd2>
30008698:	47a8      	blx	r5
3000869a:	2803      	cmp	r0, #3
3000869c:	7020      	strb	r0, [r4, #0]
3000869e:	d0dc      	beq.n	3000865a <DDR_PHY_R240_ZQ_CAL+0xd6>
300086a0:	b908      	cbnz	r0, 300086a6 <DDR_PHY_R240_ZQ_CAL+0x122>
300086a2:	47a8      	blx	r5
300086a4:	7020      	strb	r0, [r4, #0]
300086a6:	4a3f      	ldr	r2, [pc, #252]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
300086a8:	280a      	cmp	r0, #10
300086aa:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
300086ae:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
300086b2:	d056      	beq.n	30008762 <DDR_PHY_R240_ZQ_CAL+0x1de>
300086b4:	f043 031e 	orr.w	r3, r3, #30
300086b8:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
300086bc:	2800      	cmp	r0, #0
300086be:	f43f af6c 	beq.w	3000859a <DDR_PHY_R240_ZQ_CAL+0x16>
300086c2:	e7c0      	b.n	30008646 <DDR_PHY_R240_ZQ_CAL+0xc2>
300086c4:	b908      	cbnz	r0, 300086ca <DDR_PHY_R240_ZQ_CAL+0x146>
300086c6:	47a8      	blx	r5
300086c8:	7020      	strb	r0, [r4, #0]
300086ca:	2802      	cmp	r0, #2
300086cc:	f43f af6b 	beq.w	300085a6 <DDR_PHY_R240_ZQ_CAL+0x22>
300086d0:	b910      	cbnz	r0, 300086d8 <DDR_PHY_R240_ZQ_CAL+0x154>
300086d2:	4b33      	ldr	r3, [pc, #204]	; (300087a0 <DDR_PHY_R240_ZQ_CAL+0x21c>)
300086d4:	4798      	blx	r3
300086d6:	7020      	strb	r0, [r4, #0]
300086d8:	2805      	cmp	r0, #5
300086da:	d0a0      	beq.n	3000861e <DDR_PHY_R240_ZQ_CAL+0x9a>
300086dc:	bb20      	cbnz	r0, 30008728 <DDR_PHY_R240_ZQ_CAL+0x1a4>
300086de:	4d30      	ldr	r5, [pc, #192]	; (300087a0 <DDR_PHY_R240_ZQ_CAL+0x21c>)
300086e0:	47a8      	blx	r5
300086e2:	2803      	cmp	r0, #3
300086e4:	7020      	strb	r0, [r4, #0]
300086e6:	d0c1      	beq.n	3000866c <DDR_PHY_R240_ZQ_CAL+0xe8>
300086e8:	bb00      	cbnz	r0, 3000872c <DDR_PHY_R240_ZQ_CAL+0x1a8>
300086ea:	47a8      	blx	r5
300086ec:	280a      	cmp	r0, #10
300086ee:	7020      	strb	r0, [r4, #0]
300086f0:	d01e      	beq.n	30008730 <DDR_PHY_R240_ZQ_CAL+0x1ac>
300086f2:	b908      	cbnz	r0, 300086f8 <DDR_PHY_R240_ZQ_CAL+0x174>
300086f4:	47a8      	blx	r5
300086f6:	7020      	strb	r0, [r4, #0]
300086f8:	2809      	cmp	r0, #9
300086fa:	4b2a      	ldr	r3, [pc, #168]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
300086fc:	f04f 0100 	mov.w	r1, #0
30008700:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008704:	d032      	beq.n	3000876c <DDR_PHY_R240_ZQ_CAL+0x1e8>
30008706:	f8c3 1180 	str.w	r1, [r3, #384]	; 0x180
3000870a:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000870e:	f8c3 1188 	str.w	r1, [r3, #392]	; 0x188
30008712:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
30008716:	f8c3 2190 	str.w	r2, [r3, #400]	; 0x190
3000871a:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
3000871e:	f8c3 2198 	str.w	r2, [r3, #408]	; 0x198
30008722:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30008726:	e75f      	b.n	300085e8 <DDR_PHY_R240_ZQ_CAL+0x64>
30008728:	2803      	cmp	r0, #3
3000872a:	d09f      	beq.n	3000866c <DDR_PHY_R240_ZQ_CAL+0xe8>
3000872c:	280a      	cmp	r0, #10
3000872e:	d1e3      	bne.n	300086f8 <DDR_PHY_R240_ZQ_CAL+0x174>
30008730:	4b1c      	ldr	r3, [pc, #112]	; (300087a4 <DDR_PHY_R240_ZQ_CAL+0x220>)
30008732:	2100      	movs	r1, #0
30008734:	4c27      	ldr	r4, [pc, #156]	; (300087d4 <DDR_PHY_R240_ZQ_CAL+0x250>)
30008736:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
3000873a:	4827      	ldr	r0, [pc, #156]	; (300087d8 <DDR_PHY_R240_ZQ_CAL+0x254>)
3000873c:	f8c3 41a0 	str.w	r4, [r3, #416]	; 0x1a0
30008740:	f8c3 11a4 	str.w	r1, [r3, #420]	; 0x1a4
30008744:	4c22      	ldr	r4, [pc, #136]	; (300087d0 <DDR_PHY_R240_ZQ_CAL+0x24c>)
30008746:	f8c3 01a8 	str.w	r0, [r3, #424]	; 0x1a8
3000874a:	4824      	ldr	r0, [pc, #144]	; (300087dc <DDR_PHY_R240_ZQ_CAL+0x258>)
3000874c:	f8c3 11ac 	str.w	r1, [r3, #428]	; 0x1ac
30008750:	f8c3 41b0 	str.w	r4, [r3, #432]	; 0x1b0
30008754:	f8c3 21b4 	str.w	r2, [r3, #436]	; 0x1b4
30008758:	f8c3 01b8 	str.w	r0, [r3, #440]	; 0x1b8
3000875c:	f8c3 21bc 	str.w	r2, [r3, #444]	; 0x1bc
30008760:	e742      	b.n	300085e8 <DDR_PHY_R240_ZQ_CAL+0x64>
30008762:	f043 0308 	orr.w	r3, r3, #8
30008766:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
3000876a:	e7e1      	b.n	30008730 <DDR_PHY_R240_ZQ_CAL+0x1ac>
3000876c:	4c0e      	ldr	r4, [pc, #56]	; (300087a8 <DDR_PHY_R240_ZQ_CAL+0x224>)
3000876e:	480f      	ldr	r0, [pc, #60]	; (300087ac <DDR_PHY_R240_ZQ_CAL+0x228>)
30008770:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
30008774:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
30008778:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000877c:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008780:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
30008784:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
30008788:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
3000878c:	f504 6402 	add.w	r4, r4, #2080	; 0x820
30008790:	f200 201b 	addw	r0, r0, #539	; 0x21b
30008794:	e71e      	b.n	300085d4 <DDR_PHY_R240_ZQ_CAL+0x50>
30008796:	bf00      	nop
30008798:	3000eae1 	.word	0x3000eae1
3000879c:	3000eae0 	.word	0x3000eae0
300087a0:	30007ec9 	.word	0x30007ec9
300087a4:	41011000 	.word	0x41011000
300087a8:	0e141414 	.word	0x0e141414
300087ac:	140e0e0e 	.word	0x140e0e0e
300087b0:	00262626 	.word	0x00262626
300087b4:	0a0e0c0c 	.word	0x0a0e0c0c
300087b8:	0c0b0a0a 	.word	0x0c0b0a0a
300087bc:	26162620 	.word	0x26162620
300087c0:	30007f79 	.word	0x30007f79
300087c4:	0a0c0b0b 	.word	0x0a0c0b0b
300087c8:	0b0b0a0a 	.word	0x0b0b0a0a
300087cc:	26172721 	.word	0x26172721
300087d0:	2615231e 	.word	0x2615231e
300087d4:	090b0a0a 	.word	0x090b0a0a
300087d8:	0a0a0909 	.word	0x0a0a0909
300087dc:	2613201b 	.word	0x2613201b

300087e0 <DDR_PHY_ZQ_SET_SEL>:
300087e0:	b410      	push	{r4}
300087e2:	4b25      	ldr	r3, [pc, #148]	; (30008878 <DDR_PHY_ZQ_SET_SEL+0x98>)
300087e4:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300087e8:	4a24      	ldr	r2, [pc, #144]	; (3000887c <DDR_PHY_ZQ_SET_SEL+0x9c>)
300087ea:	4c25      	ldr	r4, [pc, #148]	; (30008880 <DDR_PHY_ZQ_SET_SEL+0xa0>)
300087ec:	4925      	ldr	r1, [pc, #148]	; (30008884 <DDR_PHY_ZQ_SET_SEL+0xa4>)
300087ee:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
300087f2:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
300087f6:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
300087fa:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
300087fe:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
30008802:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
30008806:	f64f 74ff 	movw	r4, #65535	; 0xffff
3000880a:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
3000880e:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
30008812:	2200      	movs	r2, #0
30008814:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30008818:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
3000881c:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30008820:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
30008824:	f04f 31ff 	mov.w	r1, #4294967295
30008828:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
3000882c:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30008830:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
30008834:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30008838:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
3000883c:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30008840:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
30008844:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30008848:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
3000884c:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30008850:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
30008854:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
30008858:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
3000885c:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30008860:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
30008864:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
30008868:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
3000886c:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30008870:	f85d 4b04 	ldr.w	r4, [sp], #4
30008874:	4770      	bx	lr
30008876:	bf00      	nop
30008878:	41011000 	.word	0x41011000
3000887c:	ffff1111 	.word	0xffff1111
30008880:	ffff3333 	.word	0xffff3333
30008884:	ffff0000 	.word	0xffff0000

30008888 <DDR_PHY_DELAY_TAP_SET>:
30008888:	b510      	push	{r4, lr}
3000888a:	4b15      	ldr	r3, [pc, #84]	; (300088e0 <DDR_PHY_DELAY_TAP_SET+0x58>)
3000888c:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
30008890:	4914      	ldr	r1, [pc, #80]	; (300088e4 <DDR_PHY_DELAY_TAP_SET+0x5c>)
30008892:	4c15      	ldr	r4, [pc, #84]	; (300088e8 <DDR_PHY_DELAY_TAP_SET+0x60>)
30008894:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
30008898:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
3000889c:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300088a0:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
300088a4:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
300088a8:	7820      	ldrb	r0, [r4, #0]
300088aa:	b1a0      	cbz	r0, 300088d6 <DDR_PHY_DELAY_TAP_SET+0x4e>
300088ac:	2801      	cmp	r0, #1
300088ae:	4b0c      	ldr	r3, [pc, #48]	; (300088e0 <DDR_PHY_DELAY_TAP_SET+0x58>)
300088b0:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
300088b4:	bf0c      	ite	eq
300088b6:	4a0d      	ldreq	r2, [pc, #52]	; (300088ec <DDR_PHY_DELAY_TAP_SET+0x64>)
300088b8:	4a0d      	ldrne	r2, [pc, #52]	; (300088f0 <DDR_PHY_DELAY_TAP_SET+0x68>)
300088ba:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
300088be:	f640 0288 	movw	r2, #2184	; 0x888
300088c2:	4b07      	ldr	r3, [pc, #28]	; (300088e0 <DDR_PHY_DELAY_TAP_SET+0x58>)
300088c4:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
300088c8:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
300088cc:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
300088d0:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
300088d4:	bd10      	pop	{r4, pc}
300088d6:	4b07      	ldr	r3, [pc, #28]	; (300088f4 <DDR_PHY_DELAY_TAP_SET+0x6c>)
300088d8:	4798      	blx	r3
300088da:	7020      	strb	r0, [r4, #0]
300088dc:	e7e6      	b.n	300088ac <DDR_PHY_DELAY_TAP_SET+0x24>
300088de:	bf00      	nop
300088e0:	41011000 	.word	0x41011000
300088e4:	20888888 	.word	0x20888888
300088e8:	3000eae1 	.word	0x3000eae1
300088ec:	7c033333 	.word	0x7c033333
300088f0:	7c011111 	.word	0x7c011111
300088f4:	30007f79 	.word	0x30007f79

300088f8 <DDR_PHY_READ_CTRL>:
300088f8:	b538      	push	{r3, r4, r5, lr}
300088fa:	4c2a      	ldr	r4, [pc, #168]	; (300089a4 <DDR_PHY_READ_CTRL+0xac>)
300088fc:	7820      	ldrb	r0, [r4, #0]
300088fe:	b330      	cbz	r0, 3000894e <DDR_PHY_READ_CTRL+0x56>
30008900:	28e0      	cmp	r0, #224	; 0xe0
30008902:	d114      	bne.n	3000892e <DDR_PHY_READ_CTRL+0x36>
30008904:	4b28      	ldr	r3, [pc, #160]	; (300089a8 <DDR_PHY_READ_CTRL+0xb0>)
30008906:	2202      	movs	r2, #2
30008908:	2103      	movs	r1, #3
3000890a:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
3000890e:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
30008912:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
30008916:	4b24      	ldr	r3, [pc, #144]	; (300089a8 <DDR_PHY_READ_CTRL+0xb0>)
30008918:	2108      	movs	r1, #8
3000891a:	22fe      	movs	r2, #254	; 0xfe
3000891c:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30008920:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
30008924:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30008928:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
3000892c:	bd38      	pop	{r3, r4, r5, pc}
3000892e:	2802      	cmp	r0, #2
30008930:	d0e8      	beq.n	30008904 <DDR_PHY_READ_CTRL+0xc>
30008932:	2805      	cmp	r0, #5
30008934:	d001      	beq.n	3000893a <DDR_PHY_READ_CTRL+0x42>
30008936:	2803      	cmp	r0, #3
30008938:	d131      	bne.n	3000899e <DDR_PHY_READ_CTRL+0xa6>
3000893a:	4b1b      	ldr	r3, [pc, #108]	; (300089a8 <DDR_PHY_READ_CTRL+0xb0>)
3000893c:	2202      	movs	r2, #2
3000893e:	2104      	movs	r1, #4
30008940:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
30008944:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
30008948:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
3000894c:	e7e3      	b.n	30008916 <DDR_PHY_READ_CTRL+0x1e>
3000894e:	4d17      	ldr	r5, [pc, #92]	; (300089ac <DDR_PHY_READ_CTRL+0xb4>)
30008950:	47a8      	blx	r5
30008952:	28e0      	cmp	r0, #224	; 0xe0
30008954:	7020      	strb	r0, [r4, #0]
30008956:	d0d5      	beq.n	30008904 <DDR_PHY_READ_CTRL+0xc>
30008958:	2800      	cmp	r0, #0
3000895a:	d1e8      	bne.n	3000892e <DDR_PHY_READ_CTRL+0x36>
3000895c:	47a8      	blx	r5
3000895e:	2802      	cmp	r0, #2
30008960:	7020      	strb	r0, [r4, #0]
30008962:	d0cf      	beq.n	30008904 <DDR_PHY_READ_CTRL+0xc>
30008964:	2800      	cmp	r0, #0
30008966:	d1e4      	bne.n	30008932 <DDR_PHY_READ_CTRL+0x3a>
30008968:	47a8      	blx	r5
3000896a:	2805      	cmp	r0, #5
3000896c:	7020      	strb	r0, [r4, #0]
3000896e:	d0e4      	beq.n	3000893a <DDR_PHY_READ_CTRL+0x42>
30008970:	2800      	cmp	r0, #0
30008972:	d1e0      	bne.n	30008936 <DDR_PHY_READ_CTRL+0x3e>
30008974:	47a8      	blx	r5
30008976:	2803      	cmp	r0, #3
30008978:	7020      	strb	r0, [r4, #0]
3000897a:	d0de      	beq.n	3000893a <DDR_PHY_READ_CTRL+0x42>
3000897c:	b978      	cbnz	r0, 3000899e <DDR_PHY_READ_CTRL+0xa6>
3000897e:	47a8      	blx	r5
30008980:	280a      	cmp	r0, #10
30008982:	7020      	strb	r0, [r4, #0]
30008984:	d0d9      	beq.n	3000893a <DDR_PHY_READ_CTRL+0x42>
30008986:	b908      	cbnz	r0, 3000898c <DDR_PHY_READ_CTRL+0x94>
30008988:	47a8      	blx	r5
3000898a:	7020      	strb	r0, [r4, #0]
3000898c:	4b06      	ldr	r3, [pc, #24]	; (300089a8 <DDR_PHY_READ_CTRL+0xb0>)
3000898e:	2203      	movs	r2, #3
30008990:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
30008994:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
30008998:	f8c3 2170 	str.w	r2, [r3, #368]	; 0x170
3000899c:	e7bb      	b.n	30008916 <DDR_PHY_READ_CTRL+0x1e>
3000899e:	280a      	cmp	r0, #10
300089a0:	d0cb      	beq.n	3000893a <DDR_PHY_READ_CTRL+0x42>
300089a2:	e7f3      	b.n	3000898c <DDR_PHY_READ_CTRL+0x94>
300089a4:	3000eae0 	.word	0x3000eae0
300089a8:	41011000 	.word	0x41011000
300089ac:	30007ec9 	.word	0x30007ec9

300089b0 <DDR_PHY_READ_LEVELING>:
300089b0:	b538      	push	{r3, r4, r5, lr}
300089b2:	4c36      	ldr	r4, [pc, #216]	; (30008a8c <DDR_PHY_READ_LEVELING+0xdc>)
300089b4:	7820      	ldrb	r0, [r4, #0]
300089b6:	b348      	cbz	r0, 30008a0c <DDR_PHY_READ_LEVELING+0x5c>
300089b8:	28e0      	cmp	r0, #224	; 0xe0
300089ba:	d11e      	bne.n	300089fa <DDR_PHY_READ_LEVELING+0x4a>
300089bc:	4b34      	ldr	r3, [pc, #208]	; (30008a90 <DDR_PHY_READ_LEVELING+0xe0>)
300089be:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
300089c2:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300089c6:	220f      	movs	r2, #15
300089c8:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
300089cc:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
300089d0:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
300089d4:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
300089d8:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
300089dc:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
300089e0:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
300089e4:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
300089e8:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
300089ec:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
300089f0:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
300089f4:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
300089f8:	bd38      	pop	{r3, r4, r5, pc}
300089fa:	2802      	cmp	r0, #2
300089fc:	d0de      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
300089fe:	2805      	cmp	r0, #5
30008a00:	d0dc      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a02:	2803      	cmp	r0, #3
30008a04:	d0da      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a06:	280a      	cmp	r0, #10
30008a08:	d0d8      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a0a:	e01f      	b.n	30008a4c <DDR_PHY_READ_LEVELING+0x9c>
30008a0c:	4d21      	ldr	r5, [pc, #132]	; (30008a94 <DDR_PHY_READ_LEVELING+0xe4>)
30008a0e:	47a8      	blx	r5
30008a10:	28e0      	cmp	r0, #224	; 0xe0
30008a12:	7020      	strb	r0, [r4, #0]
30008a14:	d0d2      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a16:	2800      	cmp	r0, #0
30008a18:	d1ef      	bne.n	300089fa <DDR_PHY_READ_LEVELING+0x4a>
30008a1a:	47a8      	blx	r5
30008a1c:	2802      	cmp	r0, #2
30008a1e:	7020      	strb	r0, [r4, #0]
30008a20:	d0cc      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a22:	2800      	cmp	r0, #0
30008a24:	d1eb      	bne.n	300089fe <DDR_PHY_READ_LEVELING+0x4e>
30008a26:	47a8      	blx	r5
30008a28:	2805      	cmp	r0, #5
30008a2a:	7020      	strb	r0, [r4, #0]
30008a2c:	d0c6      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a2e:	2800      	cmp	r0, #0
30008a30:	d1e7      	bne.n	30008a02 <DDR_PHY_READ_LEVELING+0x52>
30008a32:	47a8      	blx	r5
30008a34:	2803      	cmp	r0, #3
30008a36:	7020      	strb	r0, [r4, #0]
30008a38:	d0c0      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a3a:	2800      	cmp	r0, #0
30008a3c:	d1e3      	bne.n	30008a06 <DDR_PHY_READ_LEVELING+0x56>
30008a3e:	47a8      	blx	r5
30008a40:	280a      	cmp	r0, #10
30008a42:	7020      	strb	r0, [r4, #0]
30008a44:	d0ba      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a46:	b908      	cbnz	r0, 30008a4c <DDR_PHY_READ_LEVELING+0x9c>
30008a48:	47a8      	blx	r5
30008a4a:	7020      	strb	r0, [r4, #0]
30008a4c:	2809      	cmp	r0, #9
30008a4e:	d0b5      	beq.n	300089bc <DDR_PHY_READ_LEVELING+0xc>
30008a50:	4b0f      	ldr	r3, [pc, #60]	; (30008a90 <DDR_PHY_READ_LEVELING+0xe0>)
30008a52:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
30008a56:	2110      	movs	r1, #16
30008a58:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30008a5c:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30008a60:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30008a64:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30008a68:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30008a6c:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30008a70:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30008a74:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30008a78:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30008a7c:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30008a80:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30008a84:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30008a88:	bd38      	pop	{r3, r4, r5, pc}
30008a8a:	bf00      	nop
30008a8c:	3000eae0 	.word	0x3000eae0
30008a90:	41011000 	.word	0x41011000
30008a94:	30007ec9 	.word	0x30007ec9

30008a98 <DDR_PHY_Init>:
30008a98:	b510      	push	{r4, lr}
30008a9a:	6d80      	ldr	r0, [r0, #88]	; 0x58
30008a9c:	4c2f      	ldr	r4, [pc, #188]	; (30008b5c <DDR_PHY_Init+0xc4>)
30008a9e:	f7ff fb13 	bl	300080c8 <DDR_PHY_DLL_CLK_DIV>
30008aa2:	f7ff fd07 	bl	300084b4 <DDR_PHY_CRT_Init>
30008aa6:	f7ff fd6d 	bl	30008584 <DDR_PHY_R240_ZQ_CAL>
30008aaa:	f7ff fe99 	bl	300087e0 <DDR_PHY_ZQ_SET_SEL>
30008aae:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30008ab2:	2300      	movs	r3, #0
30008ab4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30008ab8:	4829      	ldr	r0, [pc, #164]	; (30008b60 <DDR_PHY_Init+0xc8>)
30008aba:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30008abe:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
30008ac2:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
30008ac6:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
30008aca:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30008ace:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
30008ad2:	f7ff fed9 	bl	30008888 <DDR_PHY_DELAY_TAP_SET>
30008ad6:	f7ff ff0f 	bl	300088f8 <DDR_PHY_READ_CTRL>
30008ada:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30008ade:	4a21      	ldr	r2, [pc, #132]	; (30008b64 <DDR_PHY_Init+0xcc>)
30008ae0:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
30008ae4:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008ae8:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30008aec:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008af0:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30008af4:	f7ff ff5c 	bl	300089b0 <DDR_PHY_READ_LEVELING>
30008af8:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30008afc:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008b00:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30008b04:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008b08:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30008b0c:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30008b10:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30008b14:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30008b18:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30008b1c:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30008b20:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30008b24:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008b28:	f043 0303 	orr.w	r3, r3, #3
30008b2c:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008b30:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008b34:	f043 030c 	orr.w	r3, r3, #12
30008b38:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008b3c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008b40:	f023 0303 	bic.w	r3, r3, #3
30008b44:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008b48:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008b4c:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
30008b50:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30008b54:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008b58:	bd10      	pop	{r4, pc}
30008b5a:	bf00      	nop
30008b5c:	41011000 	.word	0x41011000
30008b60:	00045500 	.word	0x00045500
30008b64:	11117777 	.word	0x11117777

30008b68 <DDR_PHY_CAL>:
30008b68:	4770      	bx	lr
30008b6a:	bf00      	nop

30008b6c <DDR_PHY_AutoGating>:
30008b6c:	4a0a      	ldr	r2, [pc, #40]	; (30008b98 <DDR_PHY_AutoGating+0x2c>)
30008b6e:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30008b72:	6893      	ldr	r3, [r2, #8]
30008b74:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30008b78:	6093      	str	r3, [r2, #8]
30008b7a:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30008b7e:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30008b82:	f043 0314 	orr.w	r3, r3, #20
30008b86:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008b8a:	f8d1 31e0 	ldr.w	r3, [r1, #480]	; 0x1e0
30008b8e:	f043 0303 	orr.w	r3, r3, #3
30008b92:	f8c1 31e0 	str.w	r3, [r1, #480]	; 0x1e0
30008b96:	4770      	bx	lr
30008b98:	41011000 	.word	0x41011000

30008b9c <flash_init_userdef>:
30008b9c:	4b14      	ldr	r3, [pc, #80]	; (30008bf0 <flash_init_userdef+0x54>)
30008b9e:	2201      	movs	r2, #1
30008ba0:	2140      	movs	r1, #64	; 0x40
30008ba2:	f44f 6000 	mov.w	r0, #2048	; 0x800
30008ba6:	b430      	push	{r4, r5}
30008ba8:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008bac:	2502      	movs	r5, #2
30008bae:	2400      	movs	r4, #0
30008bb0:	f640 0104 	movw	r1, #2052	; 0x804
30008bb4:	601a      	str	r2, [r3, #0]
30008bb6:	22bb      	movs	r2, #187	; 0xbb
30008bb8:	83d8      	strh	r0, [r3, #30]
30008bba:	8419      	strh	r1, [r3, #32]
30008bbc:	203b      	movs	r0, #59	; 0x3b
30008bbe:	21eb      	movs	r1, #235	; 0xeb
30008bc0:	629a      	str	r2, [r3, #40]	; 0x28
30008bc2:	4a0c      	ldr	r2, [pc, #48]	; (30008bf4 <flash_init_userdef+0x58>)
30008bc4:	6258      	str	r0, [r3, #36]	; 0x24
30008bc6:	6319      	str	r1, [r3, #48]	; 0x30
30008bc8:	f64a 3120 	movw	r1, #43808	; 0xab20
30008bcc:	655a      	str	r2, [r3, #84]	; 0x54
30008bce:	22b9      	movs	r2, #185	; 0xb9
30008bd0:	4809      	ldr	r0, [pc, #36]	; (30008bf8 <flash_init_userdef+0x5c>)
30008bd2:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30008bd6:	6598      	str	r0, [r3, #88]	; 0x58
30008bd8:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008bdc:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008be0:	2406      	movs	r4, #6
30008be2:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30008be6:	246b      	movs	r4, #107	; 0x6b
30008be8:	62dc      	str	r4, [r3, #44]	; 0x2c
30008bea:	bc30      	pop	{r4, r5}
30008bec:	4770      	bx	lr
30008bee:	bf00      	nop
30008bf0:	2001c01c 	.word	0x2001c01c
30008bf4:	00059f06 	.word	0x00059f06
30008bf8:	d8600001 	.word	0xd8600001

30008bfc <nand_init_userdef>:
30008bfc:	4b18      	ldr	r3, [pc, #96]	; (30008c60 <nand_init_userdef+0x64>)
30008bfe:	2001      	movs	r0, #1
30008c00:	2200      	movs	r2, #0
30008c02:	2102      	movs	r1, #2
30008c04:	b410      	push	{r4}
30008c06:	6018      	str	r0, [r3, #0]
30008c08:	f640 0408 	movw	r4, #2056	; 0x808
30008c0c:	6159      	str	r1, [r3, #20]
30008c0e:	83dc      	strh	r4, [r3, #30]
30008c10:	f44f 6400 	mov.w	r4, #2048	; 0x800
30008c14:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30008c18:	841c      	strh	r4, [r3, #32]
30008c1a:	246b      	movs	r4, #107	; 0x6b
30008c1c:	619a      	str	r2, [r3, #24]
30008c1e:	62dc      	str	r4, [r3, #44]	; 0x2c
30008c20:	4c10      	ldr	r4, [pc, #64]	; (30008c64 <nand_init_userdef+0x68>)
30008c22:	775a      	strb	r2, [r3, #29]
30008c24:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30008c28:	629a      	str	r2, [r3, #40]	; 0x28
30008c2a:	631a      	str	r2, [r3, #48]	; 0x30
30008c2c:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30008c30:	65da      	str	r2, [r3, #92]	; 0x5c
30008c32:	e9c3 0003 	strd	r0, r0, [r3, #12]
30008c36:	203b      	movs	r0, #59	; 0x3b
30008c38:	6258      	str	r0, [r3, #36]	; 0x24
30008c3a:	200b      	movs	r0, #11
30008c3c:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30008c40:	6698      	str	r0, [r3, #104]	; 0x68
30008c42:	2008      	movs	r0, #8
30008c44:	4908      	ldr	r1, [pc, #32]	; (30008c68 <nand_init_userdef+0x6c>)
30008c46:	64d8      	str	r0, [r3, #76]	; 0x4c
30008c48:	f241 0013 	movw	r0, #4115	; 0x1013
30008c4c:	6599      	str	r1, [r3, #88]	; 0x58
30008c4e:	4907      	ldr	r1, [pc, #28]	; (30008c6c <nand_init_userdef+0x70>)
30008c50:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30008c54:	6719      	str	r1, [r3, #112]	; 0x70
30008c56:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30008c5a:	f85d 4b04 	ldr.w	r4, [sp], #4
30008c5e:	4770      	bx	lr
30008c60:	2001c01c 	.word	0x2001c01c
30008c64:	000f9f06 	.word	0x000f9f06
30008c68:	d800001f 	.word	0xd800001f
30008c6c:	10300804 	.word	0x10300804

30008c70 <flash_layout_init>:
30008c70:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008c74:	4b65      	ldr	r3, [pc, #404]	; (30008e0c <flash_layout_init+0x19c>)
30008c76:	b08f      	sub	sp, #60	; 0x3c
30008c78:	4798      	blx	r3
30008c7a:	4b65      	ldr	r3, [pc, #404]	; (30008e10 <flash_layout_init+0x1a0>)
30008c7c:	4a65      	ldr	r2, [pc, #404]	; (30008e14 <flash_layout_init+0x1a4>)
30008c7e:	2800      	cmp	r0, #0
30008c80:	bf08      	it	eq
30008c82:	4613      	moveq	r3, r2
30008c84:	681f      	ldr	r7, [r3, #0]
30008c86:	2fff      	cmp	r7, #255	; 0xff
30008c88:	d05a      	beq.n	30008d40 <flash_layout_init+0xd0>
30008c8a:	f8df b19c 	ldr.w	fp, [pc, #412]	; 30008e28 <flash_layout_init+0x1b8>
30008c8e:	f04f 0900 	mov.w	r9, #0
30008c92:	4a61      	ldr	r2, [pc, #388]	; (30008e18 <flash_layout_init+0x1a8>)
30008c94:	f8db 1000 	ldr.w	r1, [fp]
30008c98:	46c8      	mov	r8, r9
30008c9a:	f8df e190 	ldr.w	lr, [pc, #400]	; 30008e2c <flash_layout_init+0x1bc>
30008c9e:	464e      	mov	r6, r9
30008ca0:	910d      	str	r1, [sp, #52]	; 0x34
30008ca2:	464c      	mov	r4, r9
30008ca4:	495d      	ldr	r1, [pc, #372]	; (30008e1c <flash_layout_init+0x1ac>)
30008ca6:	6812      	ldr	r2, [r2, #0]
30008ca8:	6809      	ldr	r1, [r1, #0]
30008caa:	f8df c184 	ldr.w	ip, [pc, #388]	; 30008e30 <flash_layout_init+0x1c0>
30008cae:	9109      	str	r1, [sp, #36]	; 0x24
30008cb0:	495b      	ldr	r1, [pc, #364]	; (30008e20 <flash_layout_init+0x1b0>)
30008cb2:	920c      	str	r2, [sp, #48]	; 0x30
30008cb4:	6809      	ldr	r1, [r1, #0]
30008cb6:	4a5b      	ldr	r2, [pc, #364]	; (30008e24 <flash_layout_init+0x1b4>)
30008cb8:	910b      	str	r1, [sp, #44]	; 0x2c
30008cba:	f8de 1000 	ldr.w	r1, [lr]
30008cbe:	f8cd 9028 	str.w	r9, [sp, #40]	; 0x28
30008cc2:	9107      	str	r1, [sp, #28]
30008cc4:	f89c 1000 	ldrb.w	r1, [ip]
30008cc8:	f8cd 9018 	str.w	r9, [sp, #24]
30008ccc:	9108      	str	r1, [sp, #32]
30008cce:	6951      	ldr	r1, [r2, #20]
30008cd0:	f8cd 9010 	str.w	r9, [sp, #16]
30008cd4:	9105      	str	r1, [sp, #20]
30008cd6:	6911      	ldr	r1, [r2, #16]
30008cd8:	f8cd 9008 	str.w	r9, [sp, #8]
30008cdc:	9103      	str	r1, [sp, #12]
30008cde:	68d1      	ldr	r1, [r2, #12]
30008ce0:	9101      	str	r1, [sp, #4]
30008ce2:	6891      	ldr	r1, [r2, #8]
30008ce4:	e9d2 0500 	ldrd	r0, r5, [r2]
30008ce8:	9100      	str	r1, [sp, #0]
30008cea:	4649      	mov	r1, r9
30008cec:	2f00      	cmp	r7, #0
30008cee:	d12a      	bne.n	30008d46 <flash_layout_init+0xd6>
30008cf0:	6858      	ldr	r0, [r3, #4]
30008cf2:	2101      	movs	r1, #1
30008cf4:	f853 7f0c 	ldr.w	r7, [r3, #12]!
30008cf8:	2fff      	cmp	r7, #255	; 0xff
30008cfa:	d1f7      	bne.n	30008cec <flash_layout_init+0x7c>
30008cfc:	b101      	cbz	r1, 30008d00 <flash_layout_init+0x90>
30008cfe:	6010      	str	r0, [r2, #0]
30008d00:	b104      	cbz	r4, 30008d04 <flash_layout_init+0x94>
30008d02:	6055      	str	r5, [r2, #4]
30008d04:	b10e      	cbz	r6, 30008d0a <flash_layout_init+0x9a>
30008d06:	9b00      	ldr	r3, [sp, #0]
30008d08:	6093      	str	r3, [r2, #8]
30008d0a:	f1b8 0f00 	cmp.w	r8, #0
30008d0e:	d001      	beq.n	30008d14 <flash_layout_init+0xa4>
30008d10:	9b01      	ldr	r3, [sp, #4]
30008d12:	60d3      	str	r3, [r2, #12]
30008d14:	9b02      	ldr	r3, [sp, #8]
30008d16:	b10b      	cbz	r3, 30008d1c <flash_layout_init+0xac>
30008d18:	9b03      	ldr	r3, [sp, #12]
30008d1a:	6113      	str	r3, [r2, #16]
30008d1c:	9b04      	ldr	r3, [sp, #16]
30008d1e:	2b00      	cmp	r3, #0
30008d20:	d169      	bne.n	30008df6 <flash_layout_init+0x186>
30008d22:	9b06      	ldr	r3, [sp, #24]
30008d24:	2b00      	cmp	r3, #0
30008d26:	d15f      	bne.n	30008de8 <flash_layout_init+0x178>
30008d28:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008d2a:	2b00      	cmp	r3, #0
30008d2c:	d166      	bne.n	30008dfc <flash_layout_init+0x18c>
30008d2e:	f1b9 0f00 	cmp.w	r9, #0
30008d32:	d005      	beq.n	30008d40 <flash_layout_init+0xd0>
30008d34:	4b38      	ldr	r3, [pc, #224]	; (30008e18 <flash_layout_init+0x1a8>)
30008d36:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30008d38:	601a      	str	r2, [r3, #0]
30008d3a:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30008d3c:	f8cb 3000 	str.w	r3, [fp]
30008d40:	b00f      	add	sp, #60	; 0x3c
30008d42:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008d46:	2f01      	cmp	r7, #1
30008d48:	d01a      	beq.n	30008d80 <flash_layout_init+0x110>
30008d4a:	2f02      	cmp	r7, #2
30008d4c:	d01b      	beq.n	30008d86 <flash_layout_init+0x116>
30008d4e:	2f03      	cmp	r7, #3
30008d50:	d01d      	beq.n	30008d8e <flash_layout_init+0x11e>
30008d52:	2f04      	cmp	r7, #4
30008d54:	d020      	beq.n	30008d98 <flash_layout_init+0x128>
30008d56:	2f05      	cmp	r7, #5
30008d58:	d023      	beq.n	30008da2 <flash_layout_init+0x132>
30008d5a:	2f06      	cmp	r7, #6
30008d5c:	d026      	beq.n	30008dac <flash_layout_init+0x13c>
30008d5e:	2f07      	cmp	r7, #7
30008d60:	d035      	beq.n	30008dce <flash_layout_init+0x15e>
30008d62:	2f08      	cmp	r7, #8
30008d64:	d1c6      	bne.n	30008cf4 <flash_layout_init+0x84>
30008d66:	689f      	ldr	r7, [r3, #8]
30008d68:	f8d3 9004 	ldr.w	r9, [r3, #4]
30008d6c:	3701      	adds	r7, #1
30008d6e:	eba7 0709 	sub.w	r7, r7, r9
30008d72:	970d      	str	r7, [sp, #52]	; 0x34
30008d74:	f109 4778 	add.w	r7, r9, #4160749568	; 0xf8000000
30008d78:	f04f 0901 	mov.w	r9, #1
30008d7c:	970c      	str	r7, [sp, #48]	; 0x30
30008d7e:	e7b9      	b.n	30008cf4 <flash_layout_init+0x84>
30008d80:	463c      	mov	r4, r7
30008d82:	685d      	ldr	r5, [r3, #4]
30008d84:	e7b6      	b.n	30008cf4 <flash_layout_init+0x84>
30008d86:	685e      	ldr	r6, [r3, #4]
30008d88:	9600      	str	r6, [sp, #0]
30008d8a:	2601      	movs	r6, #1
30008d8c:	e7b2      	b.n	30008cf4 <flash_layout_init+0x84>
30008d8e:	685f      	ldr	r7, [r3, #4]
30008d90:	f04f 0801 	mov.w	r8, #1
30008d94:	9701      	str	r7, [sp, #4]
30008d96:	e7ad      	b.n	30008cf4 <flash_layout_init+0x84>
30008d98:	685f      	ldr	r7, [r3, #4]
30008d9a:	9703      	str	r7, [sp, #12]
30008d9c:	2701      	movs	r7, #1
30008d9e:	9702      	str	r7, [sp, #8]
30008da0:	e7a8      	b.n	30008cf4 <flash_layout_init+0x84>
30008da2:	685f      	ldr	r7, [r3, #4]
30008da4:	9705      	str	r7, [sp, #20]
30008da6:	2701      	movs	r7, #1
30008da8:	9704      	str	r7, [sp, #16]
30008daa:	e7a3      	b.n	30008cf4 <flash_layout_init+0x84>
30008dac:	689f      	ldr	r7, [r3, #8]
30008dae:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008db2:	3701      	adds	r7, #1
30008db4:	eba7 070a 	sub.w	r7, r7, sl
30008db8:	9706      	str	r7, [sp, #24]
30008dba:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008dbe:	9707      	str	r7, [sp, #28]
30008dc0:	9f06      	ldr	r7, [sp, #24]
30008dc2:	f3c7 3707 	ubfx	r7, r7, #12, #8
30008dc6:	9708      	str	r7, [sp, #32]
30008dc8:	2701      	movs	r7, #1
30008dca:	9706      	str	r7, [sp, #24]
30008dcc:	e792      	b.n	30008cf4 <flash_layout_init+0x84>
30008dce:	689f      	ldr	r7, [r3, #8]
30008dd0:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008dd4:	3701      	adds	r7, #1
30008dd6:	eba7 070a 	sub.w	r7, r7, sl
30008dda:	970b      	str	r7, [sp, #44]	; 0x2c
30008ddc:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008de0:	9709      	str	r7, [sp, #36]	; 0x24
30008de2:	2701      	movs	r7, #1
30008de4:	970a      	str	r7, [sp, #40]	; 0x28
30008de6:	e785      	b.n	30008cf4 <flash_layout_init+0x84>
30008de8:	9b07      	ldr	r3, [sp, #28]
30008dea:	f8ce 3000 	str.w	r3, [lr]
30008dee:	9b08      	ldr	r3, [sp, #32]
30008df0:	f88c 3000 	strb.w	r3, [ip]
30008df4:	e798      	b.n	30008d28 <flash_layout_init+0xb8>
30008df6:	9b05      	ldr	r3, [sp, #20]
30008df8:	6153      	str	r3, [r2, #20]
30008dfa:	e792      	b.n	30008d22 <flash_layout_init+0xb2>
30008dfc:	4b07      	ldr	r3, [pc, #28]	; (30008e1c <flash_layout_init+0x1ac>)
30008dfe:	9a09      	ldr	r2, [sp, #36]	; 0x24
30008e00:	601a      	str	r2, [r3, #0]
30008e02:	4b07      	ldr	r3, [pc, #28]	; (30008e20 <flash_layout_init+0x1b0>)
30008e04:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30008e06:	601a      	str	r2, [r3, #0]
30008e08:	e791      	b.n	30008d2e <flash_layout_init+0xbe>
30008e0a:	bf00      	nop
30008e0c:	3000a541 	.word	0x3000a541
30008e10:	3000e4b4 	.word	0x3000e4b4
30008e14:	3000e43c 	.word	0x3000e43c
30008e18:	3000eb04 	.word	0x3000eb04
30008e1c:	3000eafc 	.word	0x3000eafc
30008e20:	3000eb00 	.word	0x3000eb00
30008e24:	3000eae4 	.word	0x3000eae4
30008e28:	3000eb08 	.word	0x3000eb08
30008e2c:	3000eb0c 	.word	0x3000eb0c
30008e30:	3000e514 	.word	0x3000e514

30008e34 <NAND_CHECK_IS_BAD_BLOCK>:
30008e34:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008e38:	4e25      	ldr	r6, [pc, #148]	; (30008ed0 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008e3a:	0907      	lsrs	r7, r0, #4
30008e3c:	0045      	lsls	r5, r0, #1
30008e3e:	b083      	sub	sp, #12
30008e40:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008e44:	f005 051e 	and.w	r5, r5, #30
30008e48:	40eb      	lsrs	r3, r5
30008e4a:	f003 0303 	and.w	r3, r3, #3
30008e4e:	2b02      	cmp	r3, #2
30008e50:	d022      	beq.n	30008e98 <NAND_CHECK_IS_BAD_BLOCK+0x64>
30008e52:	2b01      	cmp	r3, #1
30008e54:	d024      	beq.n	30008ea0 <NAND_CHECK_IS_BAD_BLOCK+0x6c>
30008e56:	4b1f      	ldr	r3, [pc, #124]	; (30008ed4 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30008e58:	f04f 0801 	mov.w	r8, #1
30008e5c:	2204      	movs	r2, #4
30008e5e:	4604      	mov	r4, r0
30008e60:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008e64:	0180      	lsls	r0, r0, #6
30008e66:	ab01      	add	r3, sp, #4
30008e68:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30008ee0 <NAND_CHECK_IS_BAD_BLOCK+0xac>
30008e6c:	fa08 f101 	lsl.w	r1, r8, r1
30008e70:	47c8      	blx	r9
30008e72:	f89d 3004 	ldrb.w	r3, [sp, #4]
30008e76:	f89d 2005 	ldrb.w	r2, [sp, #5]
30008e7a:	4013      	ands	r3, r2
30008e7c:	2bff      	cmp	r3, #255	; 0xff
30008e7e:	d013      	beq.n	30008ea8 <NAND_CHECK_IS_BAD_BLOCK+0x74>
30008e80:	2302      	movs	r3, #2
30008e82:	fa03 f505 	lsl.w	r5, r3, r5
30008e86:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008e8a:	432b      	orrs	r3, r5
30008e8c:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008e90:	4b11      	ldr	r3, [pc, #68]	; (30008ed8 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30008e92:	681b      	ldr	r3, [r3, #0]
30008e94:	079b      	lsls	r3, r3, #30
30008e96:	d412      	bmi.n	30008ebe <NAND_CHECK_IS_BAD_BLOCK+0x8a>
30008e98:	2001      	movs	r0, #1
30008e9a:	b003      	add	sp, #12
30008e9c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008ea0:	2000      	movs	r0, #0
30008ea2:	b003      	add	sp, #12
30008ea4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008ea8:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008eac:	fa08 f005 	lsl.w	r0, r8, r5
30008eb0:	4303      	orrs	r3, r0
30008eb2:	2000      	movs	r0, #0
30008eb4:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008eb8:	b003      	add	sp, #12
30008eba:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008ebe:	4621      	mov	r1, r4
30008ec0:	4806      	ldr	r0, [pc, #24]	; (30008edc <NAND_CHECK_IS_BAD_BLOCK+0xa8>)
30008ec2:	f003 f9b1 	bl	3000c228 <__DiagPrintf_veneer>
30008ec6:	4640      	mov	r0, r8
30008ec8:	b003      	add	sp, #12
30008eca:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008ece:	bf00      	nop
30008ed0:	3000eb18 	.word	0x3000eb18
30008ed4:	2001c01c 	.word	0x2001c01c
30008ed8:	2001c00c 	.word	0x2001c00c
30008edc:	3000d444 	.word	0x3000d444
30008ee0:	3000a499 	.word	0x3000a499

30008ee4 <Nand_Get_NandAddr>:
30008ee4:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008ee8:	4f10      	ldr	r7, [pc, #64]	; (30008f2c <Nand_Get_NandAddr+0x48>)
30008eea:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008eee:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30008ef2:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30008ef6:	40e8      	lsrs	r0, r5
30008ef8:	fa26 f505 	lsr.w	r5, r6, r5
30008efc:	09ad      	lsrs	r5, r5, #6
30008efe:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30008f02:	d90f      	bls.n	30008f24 <Nand_Get_NandAddr+0x40>
30008f04:	0984      	lsrs	r4, r0, #6
30008f06:	f04f 0840 	mov.w	r8, #64	; 0x40
30008f0a:	4620      	mov	r0, r4
30008f0c:	3401      	adds	r4, #1
30008f0e:	f7ff ff91 	bl	30008e34 <NAND_CHECK_IS_BAD_BLOCK>
30008f12:	b128      	cbz	r0, 30008f20 <Nand_Get_NandAddr+0x3c>
30008f14:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008f18:	3501      	adds	r5, #1
30008f1a:	fa08 f303 	lsl.w	r3, r8, r3
30008f1e:	441e      	add	r6, r3
30008f20:	42a5      	cmp	r5, r4
30008f22:	d8f2      	bhi.n	30008f0a <Nand_Get_NandAddr+0x26>
30008f24:	4630      	mov	r0, r6
30008f26:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008f2a:	bf00      	nop
30008f2c:	2001c01c 	.word	0x2001c01c

30008f30 <NandImgCopy>:
30008f30:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008f34:	4b56      	ldr	r3, [pc, #344]	; (30009090 <NandImgCopy+0x160>)
30008f36:	b085      	sub	sp, #20
30008f38:	4617      	mov	r7, r2
30008f3a:	9003      	str	r0, [sp, #12]
30008f3c:	6898      	ldr	r0, [r3, #8]
30008f3e:	4281      	cmp	r1, r0
30008f40:	f0c0 8089 	bcc.w	30009056 <NandImgCopy+0x126>
30008f44:	691b      	ldr	r3, [r3, #16]
30008f46:	4299      	cmp	r1, r3
30008f48:	d37b      	bcc.n	30009042 <NandImgCopy+0x112>
30008f4a:	f8df 9154 	ldr.w	r9, [pc, #340]	; 300090a0 <NandImgCopy+0x170>
30008f4e:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30008f52:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008f56:	fa25 f406 	lsr.w	r4, r5, r6
30008f5a:	f107 38ff 	add.w	r8, r7, #4294967295
30008f5e:	f1c4 0301 	rsb	r3, r4, #1
30008f62:	44a8      	add	r8, r5
30008f64:	fa28 f806 	lsr.w	r8, r8, r6
30008f68:	eb18 0803 	adds.w	r8, r8, r3
30008f6c:	d05e      	beq.n	3000902c <NandImgCopy+0xfc>
30008f6e:	f108 38ff 	add.w	r8, r8, #4294967295
30008f72:	463b      	mov	r3, r7
30008f74:	f04f 0a00 	mov.w	sl, #0
30008f78:	f04f 0b40 	mov.w	fp, #64	; 0x40
30008f7c:	4647      	mov	r7, r8
30008f7e:	4698      	mov	r8, r3
30008f80:	f8cd a008 	str.w	sl, [sp, #8]
30008f84:	e00a      	b.n	30008f9c <NandImgCopy+0x6c>
30008f86:	1ac9      	subs	r1, r1, r3
30008f88:	4b42      	ldr	r3, [pc, #264]	; (30009094 <NandImgCopy+0x164>)
30008f8a:	689b      	ldr	r3, [r3, #8]
30008f8c:	4488      	add	r8, r1
30008f8e:	079a      	lsls	r2, r3, #30
30008f90:	d44f      	bmi.n	30009032 <NandImgCopy+0x102>
30008f92:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008f96:	3f01      	subs	r7, #1
30008f98:	fa25 f406 	lsr.w	r4, r5, r6
30008f9c:	9b02      	ldr	r3, [sp, #8]
30008f9e:	4632      	mov	r2, r6
30008fa0:	09a6      	lsrs	r6, r4, #6
30008fa2:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008fa6:	d922      	bls.n	30008fee <NandImgCopy+0xbe>
30008fa8:	f1c2 0320 	rsb	r3, r2, #32
30008fac:	f04f 31ff 	mov.w	r1, #4294967295
30008fb0:	40d9      	lsrs	r1, r3
30008fb2:	9b03      	ldr	r3, [sp, #12]
30008fb4:	4029      	ands	r1, r5
30008fb6:	4453      	add	r3, sl
30008fb8:	b31f      	cbz	r7, 30009002 <NandImgCopy+0xd2>
30008fba:	2601      	movs	r6, #1
30008fbc:	4620      	mov	r0, r4
30008fbe:	9101      	str	r1, [sp, #4]
30008fc0:	fa06 f202 	lsl.w	r2, r6, r2
30008fc4:	4e34      	ldr	r6, [pc, #208]	; (30009098 <NandImgCopy+0x168>)
30008fc6:	9200      	str	r2, [sp, #0]
30008fc8:	1a52      	subs	r2, r2, r1
30008fca:	4492      	add	sl, r2
30008fcc:	4415      	add	r5, r2
30008fce:	47b0      	blx	r6
30008fd0:	f010 0f70 	tst.w	r0, #112	; 0x70
30008fd4:	9b00      	ldr	r3, [sp, #0]
30008fd6:	9901      	ldr	r1, [sp, #4]
30008fd8:	d1d5      	bne.n	30008f86 <NandImgCopy+0x56>
30008fda:	eba8 0803 	sub.w	r8, r8, r3
30008fde:	4488      	add	r8, r1
30008fe0:	e7d7      	b.n	30008f92 <NandImgCopy+0x62>
30008fe2:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008fe6:	3440      	adds	r4, #64	; 0x40
30008fe8:	fa0b f303 	lsl.w	r3, fp, r3
30008fec:	441d      	add	r5, r3
30008fee:	4630      	mov	r0, r6
30008ff0:	3601      	adds	r6, #1
30008ff2:	f7ff ff1f 	bl	30008e34 <NAND_CHECK_IS_BAD_BLOCK>
30008ff6:	2800      	cmp	r0, #0
30008ff8:	d1f3      	bne.n	30008fe2 <NandImgCopy+0xb2>
30008ffa:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008ffe:	9602      	str	r6, [sp, #8]
30009000:	e7d2      	b.n	30008fa8 <NandImgCopy+0x78>
30009002:	4642      	mov	r2, r8
30009004:	4620      	mov	r0, r4
30009006:	4d24      	ldr	r5, [pc, #144]	; (30009098 <NandImgCopy+0x168>)
30009008:	47a8      	blx	r5
3000900a:	f010 0f70 	tst.w	r0, #112	; 0x70
3000900e:	d00d      	beq.n	3000902c <NandImgCopy+0xfc>
30009010:	4b20      	ldr	r3, [pc, #128]	; (30009094 <NandImgCopy+0x164>)
30009012:	689b      	ldr	r3, [r3, #8]
30009014:	079b      	lsls	r3, r3, #30
30009016:	d509      	bpl.n	3000902c <NandImgCopy+0xfc>
30009018:	4603      	mov	r3, r0
3000901a:	09a2      	lsrs	r2, r4, #6
3000901c:	f004 013f 	and.w	r1, r4, #63	; 0x3f
30009020:	481e      	ldr	r0, [pc, #120]	; (3000909c <NandImgCopy+0x16c>)
30009022:	b005      	add	sp, #20
30009024:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009028:	f003 b8fe 	b.w	3000c228 <__DiagPrintf_veneer>
3000902c:	b005      	add	sp, #20
3000902e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009032:	4603      	mov	r3, r0
30009034:	09a2      	lsrs	r2, r4, #6
30009036:	f004 013f 	and.w	r1, r4, #63	; 0x3f
3000903a:	4818      	ldr	r0, [pc, #96]	; (3000909c <NandImgCopy+0x16c>)
3000903c:	f003 f8f4 	bl	3000c228 <__DiagPrintf_veneer>
30009040:	e7a7      	b.n	30008f92 <NandImgCopy+0x62>
30009042:	f8df 905c 	ldr.w	r9, [pc, #92]	; 300090a0 <NandImgCopy+0x170>
30009046:	f7ff ff4d 	bl	30008ee4 <Nand_Get_NandAddr>
3000904a:	4605      	mov	r5, r0
3000904c:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30009050:	fa20 f406 	lsr.w	r4, r0, r6
30009054:	e781      	b.n	30008f5a <NandImgCopy+0x2a>
30009056:	f8df 9048 	ldr.w	r9, [pc, #72]	; 300090a0 <NandImgCopy+0x170>
3000905a:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
3000905e:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30009062:	fa25 f406 	lsr.w	r4, r5, r6
30009066:	ea5f 1894 	movs.w	r8, r4, lsr #6
3000906a:	f43f af76 	beq.w	30008f5a <NandImgCopy+0x2a>
3000906e:	2400      	movs	r4, #0
30009070:	2640      	movs	r6, #64	; 0x40
30009072:	4620      	mov	r0, r4
30009074:	3401      	adds	r4, #1
30009076:	f7ff fedd 	bl	30008e34 <NAND_CHECK_IS_BAD_BLOCK>
3000907a:	b130      	cbz	r0, 3000908a <NandImgCopy+0x15a>
3000907c:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30009080:	f108 0801 	add.w	r8, r8, #1
30009084:	fa06 f303 	lsl.w	r3, r6, r3
30009088:	441d      	add	r5, r3
3000908a:	4544      	cmp	r4, r8
3000908c:	d3f1      	bcc.n	30009072 <NandImgCopy+0x142>
3000908e:	e760      	b.n	30008f52 <NandImgCopy+0x22>
30009090:	3000ee38 	.word	0x3000ee38
30009094:	2001c00c 	.word	0x2001c00c
30009098:	3000a499 	.word	0x3000a499
3000909c:	3000d470 	.word	0x3000d470
300090a0:	2001c01c 	.word	0x2001c01c

300090a4 <OTP_Read8.part.0>:
300090a4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300090a8:	4b2f      	ldr	r3, [pc, #188]	; (30009168 <OTP_Read8.part.0+0xc4>)
300090aa:	4680      	mov	r8, r0
300090ac:	e843 f300 	tt	r3, r3
300090b0:	f3c3 5380 	ubfx	r3, r3, #22, #1
300090b4:	4a2d      	ldr	r2, [pc, #180]	; (3000916c <OTP_Read8.part.0+0xc8>)
300090b6:	4e2e      	ldr	r6, [pc, #184]	; (30009170 <OTP_Read8.part.0+0xcc>)
300090b8:	4689      	mov	r9, r1
300090ba:	2b00      	cmp	r3, #0
300090bc:	bf0e      	itee	eq
300090be:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300090c2:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300090c6:	4616      	movne	r6, r2
300090c8:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300090ca:	05d8      	lsls	r0, r3, #23
300090cc:	d505      	bpl.n	300090da <OTP_Read8.part.0+0x36>
300090ce:	4d29      	ldr	r5, [pc, #164]	; (30009174 <OTP_Read8.part.0+0xd0>)
300090d0:	2064      	movs	r0, #100	; 0x64
300090d2:	47a8      	blx	r5
300090d4:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300090d6:	05d9      	lsls	r1, r3, #23
300090d8:	d4fa      	bmi.n	300090d0 <OTP_Read8.part.0+0x2c>
300090da:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300090dc:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300090e0:	643b      	str	r3, [r7, #64]	; 0x40
300090e2:	4b25      	ldr	r3, [pc, #148]	; (30009178 <OTP_Read8.part.0+0xd4>)
300090e4:	681a      	ldr	r2, [r3, #0]
300090e6:	07d2      	lsls	r2, r2, #31
300090e8:	d403      	bmi.n	300090f2 <OTP_Read8.part.0+0x4e>
300090ea:	681a      	ldr	r2, [r3, #0]
300090ec:	f042 0201 	orr.w	r2, r2, #1
300090f0:	601a      	str	r2, [r3, #0]
300090f2:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
300090f6:	ea4f 2308 	mov.w	r3, r8, lsl #8
300090fa:	694a      	ldr	r2, [r1, #20]
300090fc:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30009100:	614a      	str	r2, [r1, #20]
30009102:	6033      	str	r3, [r6, #0]
30009104:	6833      	ldr	r3, [r6, #0]
30009106:	2b00      	cmp	r3, #0
30009108:	db10      	blt.n	3000912c <OTP_Read8.part.0+0x88>
3000910a:	2400      	movs	r4, #0
3000910c:	4d19      	ldr	r5, [pc, #100]	; (30009174 <OTP_Read8.part.0+0xd0>)
3000910e:	f644 6a20 	movw	sl, #20000	; 0x4e20
30009112:	e001      	b.n	30009118 <OTP_Read8.part.0+0x74>
30009114:	4554      	cmp	r4, sl
30009116:	d019      	beq.n	3000914c <OTP_Read8.part.0+0xa8>
30009118:	2005      	movs	r0, #5
3000911a:	3401      	adds	r4, #1
3000911c:	47a8      	blx	r5
3000911e:	6833      	ldr	r3, [r6, #0]
30009120:	2b00      	cmp	r3, #0
30009122:	daf7      	bge.n	30009114 <OTP_Read8.part.0+0x70>
30009124:	f644 6320 	movw	r3, #20000	; 0x4e20
30009128:	429c      	cmp	r4, r3
3000912a:	d00f      	beq.n	3000914c <OTP_Read8.part.0+0xa8>
3000912c:	6833      	ldr	r3, [r6, #0]
3000912e:	2001      	movs	r0, #1
30009130:	f889 3000 	strb.w	r3, [r9]
30009134:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30009138:	6953      	ldr	r3, [r2, #20]
3000913a:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
3000913e:	6153      	str	r3, [r2, #20]
30009140:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009142:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30009146:	643b      	str	r3, [r7, #64]	; 0x40
30009148:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
3000914c:	22ff      	movs	r2, #255	; 0xff
3000914e:	4b0b      	ldr	r3, [pc, #44]	; (3000917c <OTP_Read8.part.0+0xd8>)
30009150:	f889 2000 	strb.w	r2, [r9]
30009154:	6818      	ldr	r0, [r3, #0]
30009156:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
3000915a:	d0eb      	beq.n	30009134 <OTP_Read8.part.0+0x90>
3000915c:	4641      	mov	r1, r8
3000915e:	4808      	ldr	r0, [pc, #32]	; (30009180 <OTP_Read8.part.0+0xdc>)
30009160:	f003 f862 	bl	3000c228 <__DiagPrintf_veneer>
30009164:	2000      	movs	r0, #0
30009166:	e7e5      	b.n	30009134 <OTP_Read8.part.0+0x90>
30009168:	0000e5f9 	.word	0x0000e5f9
3000916c:	5200000c 	.word	0x5200000c
30009170:	42000008 	.word	0x42000008
30009174:	00009b2d 	.word	0x00009b2d
30009178:	42008000 	.word	0x42008000
3000917c:	2001c00c 	.word	0x2001c00c
30009180:	3000d4c0 	.word	0x3000d4c0

30009184 <OTP_ProgramMarginRead8.part.0>:
30009184:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30009188:	4b30      	ldr	r3, [pc, #192]	; (3000924c <OTP_ProgramMarginRead8.part.0+0xc8>)
3000918a:	4680      	mov	r8, r0
3000918c:	e843 f300 	tt	r3, r3
30009190:	f3c3 5380 	ubfx	r3, r3, #22, #1
30009194:	4a2e      	ldr	r2, [pc, #184]	; (30009250 <OTP_ProgramMarginRead8.part.0+0xcc>)
30009196:	4e2f      	ldr	r6, [pc, #188]	; (30009254 <OTP_ProgramMarginRead8.part.0+0xd0>)
30009198:	4689      	mov	r9, r1
3000919a:	2b00      	cmp	r3, #0
3000919c:	bf0e      	itee	eq
3000919e:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300091a2:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300091a6:	4616      	movne	r6, r2
300091a8:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300091aa:	05d8      	lsls	r0, r3, #23
300091ac:	d505      	bpl.n	300091ba <OTP_ProgramMarginRead8.part.0+0x36>
300091ae:	4d2a      	ldr	r5, [pc, #168]	; (30009258 <OTP_ProgramMarginRead8.part.0+0xd4>)
300091b0:	2064      	movs	r0, #100	; 0x64
300091b2:	47a8      	blx	r5
300091b4:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300091b6:	05d9      	lsls	r1, r3, #23
300091b8:	d4fa      	bmi.n	300091b0 <OTP_ProgramMarginRead8.part.0+0x2c>
300091ba:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300091bc:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300091c0:	643b      	str	r3, [r7, #64]	; 0x40
300091c2:	4b26      	ldr	r3, [pc, #152]	; (3000925c <OTP_ProgramMarginRead8.part.0+0xd8>)
300091c4:	681a      	ldr	r2, [r3, #0]
300091c6:	07d2      	lsls	r2, r2, #31
300091c8:	d403      	bmi.n	300091d2 <OTP_ProgramMarginRead8.part.0+0x4e>
300091ca:	681a      	ldr	r2, [r3, #0]
300091cc:	f042 0201 	orr.w	r2, r2, #1
300091d0:	601a      	str	r2, [r3, #0]
300091d2:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
300091d6:	ea4f 2308 	mov.w	r3, r8, lsl #8
300091da:	694a      	ldr	r2, [r1, #20]
300091dc:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
300091e0:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
300091e4:	614a      	str	r2, [r1, #20]
300091e6:	6033      	str	r3, [r6, #0]
300091e8:	6833      	ldr	r3, [r6, #0]
300091ea:	2b00      	cmp	r3, #0
300091ec:	db10      	blt.n	30009210 <OTP_ProgramMarginRead8.part.0+0x8c>
300091ee:	2400      	movs	r4, #0
300091f0:	4d19      	ldr	r5, [pc, #100]	; (30009258 <OTP_ProgramMarginRead8.part.0+0xd4>)
300091f2:	f644 6a20 	movw	sl, #20000	; 0x4e20
300091f6:	e001      	b.n	300091fc <OTP_ProgramMarginRead8.part.0+0x78>
300091f8:	4554      	cmp	r4, sl
300091fa:	d019      	beq.n	30009230 <OTP_ProgramMarginRead8.part.0+0xac>
300091fc:	2005      	movs	r0, #5
300091fe:	3401      	adds	r4, #1
30009200:	47a8      	blx	r5
30009202:	6833      	ldr	r3, [r6, #0]
30009204:	2b00      	cmp	r3, #0
30009206:	daf7      	bge.n	300091f8 <OTP_ProgramMarginRead8.part.0+0x74>
30009208:	f644 6320 	movw	r3, #20000	; 0x4e20
3000920c:	429c      	cmp	r4, r3
3000920e:	d00f      	beq.n	30009230 <OTP_ProgramMarginRead8.part.0+0xac>
30009210:	6833      	ldr	r3, [r6, #0]
30009212:	2001      	movs	r0, #1
30009214:	f889 3000 	strb.w	r3, [r9]
30009218:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
3000921c:	6953      	ldr	r3, [r2, #20]
3000921e:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30009222:	6153      	str	r3, [r2, #20]
30009224:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009226:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000922a:	643b      	str	r3, [r7, #64]	; 0x40
3000922c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30009230:	22ff      	movs	r2, #255	; 0xff
30009232:	4b0b      	ldr	r3, [pc, #44]	; (30009260 <OTP_ProgramMarginRead8.part.0+0xdc>)
30009234:	f889 2000 	strb.w	r2, [r9]
30009238:	6818      	ldr	r0, [r3, #0]
3000923a:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
3000923e:	d0eb      	beq.n	30009218 <OTP_ProgramMarginRead8.part.0+0x94>
30009240:	4641      	mov	r1, r8
30009242:	4808      	ldr	r0, [pc, #32]	; (30009264 <OTP_ProgramMarginRead8.part.0+0xe0>)
30009244:	f002 fff0 	bl	3000c228 <__DiagPrintf_veneer>
30009248:	2000      	movs	r0, #0
3000924a:	e7e5      	b.n	30009218 <OTP_ProgramMarginRead8.part.0+0x94>
3000924c:	0000e5f9 	.word	0x0000e5f9
30009250:	5200000c 	.word	0x5200000c
30009254:	42000008 	.word	0x42000008
30009258:	00009b2d 	.word	0x00009b2d
3000925c:	42008000 	.word	0x42008000
30009260:	2001c00c 	.word	0x2001c00c
30009264:	3000d4c0 	.word	0x3000d4c0

30009268 <OTP_Read8>:
30009268:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
3000926c:	d201      	bcs.n	30009272 <OTP_Read8+0xa>
3000926e:	f7ff bf19 	b.w	300090a4 <OTP_Read8.part.0>
30009272:	22ff      	movs	r2, #255	; 0xff
30009274:	2000      	movs	r0, #0
30009276:	700a      	strb	r2, [r1, #0]
30009278:	4770      	bx	lr
3000927a:	bf00      	nop

3000927c <OTP_Write8>:
3000927c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009280:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009284:	b085      	sub	sp, #20
30009286:	d277      	bcs.n	30009378 <OTP_Write8+0xfc>
30009288:	4689      	mov	r9, r1
3000928a:	f10d 010f 	add.w	r1, sp, #15
3000928e:	4680      	mov	r8, r0
30009290:	f7ff ff78 	bl	30009184 <OTP_ProgramMarginRead8.part.0>
30009294:	2800      	cmp	r0, #0
30009296:	d072      	beq.n	3000937e <OTP_Write8+0x102>
30009298:	ea4f 2308 	mov.w	r3, r8, lsl #8
3000929c:	2202      	movs	r2, #2
3000929e:	46ca      	mov	sl, r9
300092a0:	9301      	str	r3, [sp, #4]
300092a2:	f89d 300f 	ldrb.w	r3, [sp, #15]
300092a6:	9200      	str	r2, [sp, #0]
300092a8:	ea6a 0a03 	orn	sl, sl, r3
300092ac:	fa5f fa8a 	uxtb.w	sl, sl
300092b0:	f1ba 0fff 	cmp.w	sl, #255	; 0xff
300092b4:	d04f      	beq.n	30009356 <OTP_Write8+0xda>
300092b6:	4b52      	ldr	r3, [pc, #328]	; (30009400 <OTP_Write8+0x184>)
300092b8:	e843 f300 	tt	r3, r3
300092bc:	f3c3 5380 	ubfx	r3, r3, #22, #1
300092c0:	4a50      	ldr	r2, [pc, #320]	; (30009404 <OTP_Write8+0x188>)
300092c2:	2b00      	cmp	r3, #0
300092c4:	4b50      	ldr	r3, [pc, #320]	; (30009408 <OTP_Write8+0x18c>)
300092c6:	bf0b      	itete	eq
300092c8:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300092cc:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300092d0:	461d      	moveq	r5, r3
300092d2:	4615      	movne	r5, r2
300092d4:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300092d6:	05dc      	lsls	r4, r3, #23
300092d8:	d505      	bpl.n	300092e6 <OTP_Write8+0x6a>
300092da:	4c4c      	ldr	r4, [pc, #304]	; (3000940c <OTP_Write8+0x190>)
300092dc:	2064      	movs	r0, #100	; 0x64
300092de:	47a0      	blx	r4
300092e0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300092e2:	05d8      	lsls	r0, r3, #23
300092e4:	d4fa      	bmi.n	300092dc <OTP_Write8+0x60>
300092e6:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300092e8:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300092ec:	643b      	str	r3, [r7, #64]	; 0x40
300092ee:	4b48      	ldr	r3, [pc, #288]	; (30009410 <OTP_Write8+0x194>)
300092f0:	681a      	ldr	r2, [r3, #0]
300092f2:	07d1      	lsls	r1, r2, #31
300092f4:	d403      	bmi.n	300092fe <OTP_Write8+0x82>
300092f6:	681a      	ldr	r2, [r3, #0]
300092f8:	f042 0201 	orr.w	r2, r2, #1
300092fc:	601a      	str	r2, [r3, #0]
300092fe:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30009302:	9b01      	ldr	r3, [sp, #4]
30009304:	694a      	ldr	r2, [r1, #20]
30009306:	ea4a 0303 	orr.w	r3, sl, r3
3000930a:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
3000930e:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
30009312:	614a      	str	r2, [r1, #20]
30009314:	602b      	str	r3, [r5, #0]
30009316:	682b      	ldr	r3, [r5, #0]
30009318:	2b00      	cmp	r3, #0
3000931a:	da12      	bge.n	30009342 <OTP_Write8+0xc6>
3000931c:	f04f 0b00 	mov.w	fp, #0
30009320:	4c3a      	ldr	r4, [pc, #232]	; (3000940c <OTP_Write8+0x190>)
30009322:	f644 6620 	movw	r6, #20000	; 0x4e20
30009326:	e001      	b.n	3000932c <OTP_Write8+0xb0>
30009328:	45b3      	cmp	fp, r6
3000932a:	d037      	beq.n	3000939c <OTP_Write8+0x120>
3000932c:	2005      	movs	r0, #5
3000932e:	f10b 0b01 	add.w	fp, fp, #1
30009332:	47a0      	blx	r4
30009334:	682b      	ldr	r3, [r5, #0]
30009336:	2b00      	cmp	r3, #0
30009338:	dbf6      	blt.n	30009328 <OTP_Write8+0xac>
3000933a:	f644 6220 	movw	r2, #20000	; 0x4e20
3000933e:	4593      	cmp	fp, r2
30009340:	d02c      	beq.n	3000939c <OTP_Write8+0x120>
30009342:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30009346:	6953      	ldr	r3, [r2, #20]
30009348:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
3000934c:	6153      	str	r3, [r2, #20]
3000934e:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009350:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30009354:	643b      	str	r3, [r7, #64]	; 0x40
30009356:	f10d 010f 	add.w	r1, sp, #15
3000935a:	4640      	mov	r0, r8
3000935c:	f7ff ff12 	bl	30009184 <OTP_ProgramMarginRead8.part.0>
30009360:	2800      	cmp	r0, #0
30009362:	d040      	beq.n	300093e6 <OTP_Write8+0x16a>
30009364:	f89d 300f 	ldrb.w	r3, [sp, #15]
30009368:	454b      	cmp	r3, r9
3000936a:	d046      	beq.n	300093fa <OTP_Write8+0x17e>
3000936c:	9a00      	ldr	r2, [sp, #0]
3000936e:	2a01      	cmp	r2, #1
30009370:	d009      	beq.n	30009386 <OTP_Write8+0x10a>
30009372:	2201      	movs	r2, #1
30009374:	9200      	str	r2, [sp, #0]
30009376:	e797      	b.n	300092a8 <OTP_Write8+0x2c>
30009378:	23ff      	movs	r3, #255	; 0xff
3000937a:	f88d 300f 	strb.w	r3, [sp, #15]
3000937e:	4b25      	ldr	r3, [pc, #148]	; (30009414 <OTP_Write8+0x198>)
30009380:	681b      	ldr	r3, [r3, #0]
30009382:	041d      	lsls	r5, r3, #16
30009384:	d403      	bmi.n	3000938e <OTP_Write8+0x112>
30009386:	2000      	movs	r0, #0
30009388:	b005      	add	sp, #20
3000938a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000938e:	4822      	ldr	r0, [pc, #136]	; (30009418 <OTP_Write8+0x19c>)
30009390:	f002 ff4a 	bl	3000c228 <__DiagPrintf_veneer>
30009394:	2000      	movs	r0, #0
30009396:	b005      	add	sp, #20
30009398:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000939c:	4c1d      	ldr	r4, [pc, #116]	; (30009414 <OTP_Write8+0x198>)
3000939e:	6820      	ldr	r0, [r4, #0]
300093a0:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
300093a4:	d10a      	bne.n	300093bc <OTP_Write8+0x140>
300093a6:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300093aa:	6953      	ldr	r3, [r2, #20]
300093ac:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300093b0:	6153      	str	r3, [r2, #20]
300093b2:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300093b4:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300093b8:	643b      	str	r3, [r7, #64]	; 0x40
300093ba:	e7e5      	b.n	30009388 <OTP_Write8+0x10c>
300093bc:	4641      	mov	r1, r8
300093be:	4817      	ldr	r0, [pc, #92]	; (3000941c <OTP_Write8+0x1a0>)
300093c0:	f002 ff32 	bl	3000c228 <__DiagPrintf_veneer>
300093c4:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300093c8:	6953      	ldr	r3, [r2, #20]
300093ca:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300093ce:	6153      	str	r3, [r2, #20]
300093d0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300093d2:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300093d6:	643b      	str	r3, [r7, #64]	; 0x40
300093d8:	6823      	ldr	r3, [r4, #0]
300093da:	041a      	lsls	r2, r3, #16
300093dc:	d5d3      	bpl.n	30009386 <OTP_Write8+0x10a>
300093de:	4810      	ldr	r0, [pc, #64]	; (30009420 <OTP_Write8+0x1a4>)
300093e0:	f002 ff22 	bl	3000c228 <__DiagPrintf_veneer>
300093e4:	e7cf      	b.n	30009386 <OTP_Write8+0x10a>
300093e6:	4b0b      	ldr	r3, [pc, #44]	; (30009414 <OTP_Write8+0x198>)
300093e8:	4604      	mov	r4, r0
300093ea:	681b      	ldr	r3, [r3, #0]
300093ec:	041b      	lsls	r3, r3, #16
300093ee:	d5ca      	bpl.n	30009386 <OTP_Write8+0x10a>
300093f0:	480c      	ldr	r0, [pc, #48]	; (30009424 <OTP_Write8+0x1a8>)
300093f2:	f002 ff19 	bl	3000c228 <__DiagPrintf_veneer>
300093f6:	4620      	mov	r0, r4
300093f8:	e7c6      	b.n	30009388 <OTP_Write8+0x10c>
300093fa:	2001      	movs	r0, #1
300093fc:	e7c4      	b.n	30009388 <OTP_Write8+0x10c>
300093fe:	bf00      	nop
30009400:	0000e5f9 	.word	0x0000e5f9
30009404:	5200000c 	.word	0x5200000c
30009408:	42000008 	.word	0x42000008
3000940c:	00009b2d 	.word	0x00009b2d
30009410:	42008000 	.word	0x42008000
30009414:	2001c00c 	.word	0x2001c00c
30009418:	3000d4f0 	.word	0x3000d4f0
3000941c:	3000d51c 	.word	0x3000d51c
30009420:	3000d54c 	.word	0x3000d54c
30009424:	3000d57c 	.word	0x3000d57c

30009428 <EFUSERead8>:
30009428:	4608      	mov	r0, r1
3000942a:	4611      	mov	r1, r2
3000942c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009430:	d201      	bcs.n	30009436 <EFUSERead8+0xe>
30009432:	f7ff be37 	b.w	300090a4 <OTP_Read8.part.0>
30009436:	23ff      	movs	r3, #255	; 0xff
30009438:	2000      	movs	r0, #0
3000943a:	7013      	strb	r3, [r2, #0]
3000943c:	4770      	bx	lr
3000943e:	bf00      	nop

30009440 <EFUSE_PMAP_READ8>:
30009440:	4608      	mov	r0, r1
30009442:	4611      	mov	r1, r2
30009444:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009448:	d201      	bcs.n	3000944e <EFUSE_PMAP_READ8+0xe>
3000944a:	f7ff be2b 	b.w	300090a4 <OTP_Read8.part.0>
3000944e:	23ff      	movs	r3, #255	; 0xff
30009450:	2000      	movs	r0, #0
30009452:	7013      	strb	r3, [r2, #0]
30009454:	4770      	bx	lr
30009456:	bf00      	nop

30009458 <PLL_NP_ClkSet>:
30009458:	b570      	push	{r4, r5, r6, lr}
3000945a:	4604      	mov	r4, r0
3000945c:	4b1b      	ldr	r3, [pc, #108]	; (300094cc <PLL_NP_ClkSet+0x74>)
3000945e:	4798      	blx	r3
30009460:	4d1b      	ldr	r5, [pc, #108]	; (300094d0 <PLL_NP_ClkSet+0x78>)
30009462:	08c2      	lsrs	r2, r0, #3
30009464:	fbb4 f3f0 	udiv	r3, r4, r0
30009468:	fb00 4c13 	mls	ip, r0, r3, r4
3000946c:	1e9c      	subs	r4, r3, #2
3000946e:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009470:	0164      	lsls	r4, r4, #5
30009472:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30009476:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
3000947a:	431c      	orrs	r4, r3
3000947c:	fbbc f3f2 	udiv	r3, ip, r2
30009480:	646c      	str	r4, [r5, #68]	; 0x44
30009482:	041c      	lsls	r4, r3, #16
30009484:	fb02 c013 	mls	r0, r2, r3, ip
30009488:	2300      	movs	r3, #0
3000948a:	6cee      	ldr	r6, [r5, #76]	; 0x4c
3000948c:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30009490:	4619      	mov	r1, r3
30009492:	fa1f fc86 	uxth.w	ip, r6
30009496:	4e0f      	ldr	r6, [pc, #60]	; (300094d4 <PLL_NP_ClkSet+0x7c>)
30009498:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
3000949c:	ea44 040c 	orr.w	r4, r4, ip
300094a0:	f002 fd12 	bl	3000bec8 <__aeabi_uldivmod>
300094a4:	4603      	mov	r3, r0
300094a6:	2001      	movs	r0, #1
300094a8:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
300094ac:	64ec      	str	r4, [r5, #76]	; 0x4c
300094ae:	47b0      	blx	r6
300094b0:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300094b2:	203c      	movs	r0, #60	; 0x3c
300094b4:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
300094b8:	646b      	str	r3, [r5, #68]	; 0x44
300094ba:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300094bc:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300094c0:	646b      	str	r3, [r5, #68]	; 0x44
300094c2:	4633      	mov	r3, r6
300094c4:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
300094c8:	4718      	bx	r3
300094ca:	bf00      	nop
300094cc:	000099f5 	.word	0x000099f5
300094d0:	42008800 	.word	0x42008800
300094d4:	00009b2d 	.word	0x00009b2d

300094d8 <PLL_AP_ClkSet>:
300094d8:	b510      	push	{r4, lr}
300094da:	4604      	mov	r4, r0
300094dc:	4b0b      	ldr	r3, [pc, #44]	; (3000950c <PLL_AP_ClkSet+0x34>)
300094de:	4798      	blx	r3
300094e0:	fbb4 f0f0 	udiv	r0, r4, r0
300094e4:	f1a0 0319 	sub.w	r3, r0, #25
300094e8:	1e84      	subs	r4, r0, #2
300094ea:	2b0f      	cmp	r3, #15
300094ec:	d807      	bhi.n	300094fe <PLL_AP_ClkSet+0x26>
300094ee:	4b08      	ldr	r3, [pc, #32]	; (30009510 <PLL_AP_ClkSet+0x38>)
300094f0:	b2e0      	uxtb	r0, r4
300094f2:	695c      	ldr	r4, [r3, #20]
300094f4:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
300094f8:	4320      	orrs	r0, r4
300094fa:	6158      	str	r0, [r3, #20]
300094fc:	bd10      	pop	{r4, pc}
300094fe:	f240 1175 	movw	r1, #373	; 0x175
30009502:	4804      	ldr	r0, [pc, #16]	; (30009514 <PLL_AP_ClkSet+0x3c>)
30009504:	f002 fe80 	bl	3000c208 <__io_assert_failed_veneer>
30009508:	e7f1      	b.n	300094ee <PLL_AP_ClkSet+0x16>
3000950a:	bf00      	nop
3000950c:	000099f5 	.word	0x000099f5
30009510:	41000300 	.word	0x41000300
30009514:	3000d5a8 	.word	0x3000d5a8

30009518 <PLL_AP>:
30009518:	2801      	cmp	r0, #1
3000951a:	b538      	push	{r3, r4, r5, lr}
3000951c:	d009      	beq.n	30009532 <PLL_AP+0x1a>
3000951e:	4b13      	ldr	r3, [pc, #76]	; (3000956c <PLL_AP+0x54>)
30009520:	681a      	ldr	r2, [r3, #0]
30009522:	f022 0209 	bic.w	r2, r2, #9
30009526:	601a      	str	r2, [r3, #0]
30009528:	681a      	ldr	r2, [r3, #0]
3000952a:	f022 0206 	bic.w	r2, r2, #6
3000952e:	601a      	str	r2, [r3, #0]
30009530:	bd38      	pop	{r3, r4, r5, pc}
30009532:	4b0f      	ldr	r3, [pc, #60]	; (30009570 <PLL_AP+0x58>)
30009534:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30009536:	f012 0f05 	tst.w	r2, #5
3000953a:	d00f      	beq.n	3000955c <PLL_AP+0x44>
3000953c:	4d0d      	ldr	r5, [pc, #52]	; (30009574 <PLL_AP+0x5c>)
3000953e:	4c0b      	ldr	r4, [pc, #44]	; (3000956c <PLL_AP+0x54>)
30009540:	2001      	movs	r0, #1
30009542:	6823      	ldr	r3, [r4, #0]
30009544:	f043 0306 	orr.w	r3, r3, #6
30009548:	6023      	str	r3, [r4, #0]
3000954a:	47a8      	blx	r5
3000954c:	6823      	ldr	r3, [r4, #0]
3000954e:	f043 0309 	orr.w	r3, r3, #9
30009552:	6023      	str	r3, [r4, #0]
30009554:	6863      	ldr	r3, [r4, #4]
30009556:	2b00      	cmp	r3, #0
30009558:	dafc      	bge.n	30009554 <PLL_AP+0x3c>
3000955a:	bd38      	pop	{r3, r4, r5, pc}
3000955c:	6f9a      	ldr	r2, [r3, #120]	; 0x78
3000955e:	20a0      	movs	r0, #160	; 0xa0
30009560:	4d04      	ldr	r5, [pc, #16]	; (30009574 <PLL_AP+0x5c>)
30009562:	f042 0207 	orr.w	r2, r2, #7
30009566:	679a      	str	r2, [r3, #120]	; 0x78
30009568:	47a8      	blx	r5
3000956a:	e7e8      	b.n	3000953e <PLL_AP+0x26>
3000956c:	41000300 	.word	0x41000300
30009570:	42008800 	.word	0x42008800
30009574:	00009b2d 	.word	0x00009b2d

30009578 <PSRAM_INFO_Update>:
30009578:	4942      	ldr	r1, [pc, #264]	; (30009684 <PSRAM_INFO_Update+0x10c>)
3000957a:	4a43      	ldr	r2, [pc, #268]	; (30009688 <PSRAM_INFO_Update+0x110>)
3000957c:	b538      	push	{r3, r4, r5, lr}
3000957e:	f8d1 1228 	ldr.w	r1, [r1, #552]	; 0x228
30009582:	6815      	ldr	r5, [r2, #0]
30009584:	4b41      	ldr	r3, [pc, #260]	; (3000968c <PSRAM_INFO_Update+0x114>)
30009586:	f3c1 2102 	ubfx	r1, r1, #8, #3
3000958a:	4c41      	ldr	r4, [pc, #260]	; (30009690 <PSRAM_INFO_Update+0x118>)
3000958c:	3101      	adds	r1, #1
3000958e:	fb03 f505 	mul.w	r5, r3, r5
30009592:	4b40      	ldr	r3, [pc, #256]	; (30009694 <PSRAM_INFO_Update+0x11c>)
30009594:	fbb5 f5f1 	udiv	r5, r5, r1
30009598:	086a      	lsrs	r2, r5, #1
3000959a:	60e2      	str	r2, [r4, #12]
3000959c:	4798      	blx	r3
3000959e:	4b3e      	ldr	r3, [pc, #248]	; (30009698 <PSRAM_INFO_Update+0x120>)
300095a0:	6020      	str	r0, [r4, #0]
300095a2:	4798      	blx	r3
300095a4:	4b3d      	ldr	r3, [pc, #244]	; (3000969c <PSRAM_INFO_Update+0x124>)
300095a6:	220a      	movs	r2, #10
300095a8:	60a0      	str	r0, [r4, #8]
300095aa:	6122      	str	r2, [r4, #16]
300095ac:	2220      	movs	r2, #32
300095ae:	fbb3 f3f5 	udiv	r3, r3, r5
300095b2:	6163      	str	r3, [r4, #20]
300095b4:	fbb2 f3f3 	udiv	r3, r2, r3
300095b8:	6822      	ldr	r2, [r4, #0]
300095ba:	6223      	str	r3, [r4, #32]
300095bc:	2a01      	cmp	r2, #1
300095be:	68e3      	ldr	r3, [r4, #12]
300095c0:	4a37      	ldr	r2, [pc, #220]	; (300096a0 <PSRAM_INFO_Update+0x128>)
300095c2:	d014      	beq.n	300095ee <PSRAM_INFO_Update+0x76>
300095c4:	4293      	cmp	r3, r2
300095c6:	d808      	bhi.n	300095da <PSRAM_INFO_Update+0x62>
300095c8:	2204      	movs	r2, #4
300095ca:	2308      	movs	r3, #8
300095cc:	e9c4 2306 	strd	r2, r3, [r4, #24]
300095d0:	4b34      	ldr	r3, [pc, #208]	; (300096a4 <PSRAM_INFO_Update+0x12c>)
300095d2:	689b      	ldr	r3, [r3, #8]
300095d4:	079b      	lsls	r3, r3, #30
300095d6:	d414      	bmi.n	30009602 <PSRAM_INFO_Update+0x8a>
300095d8:	bd38      	pop	{r3, r4, r5, pc}
300095da:	4a33      	ldr	r2, [pc, #204]	; (300096a8 <PSRAM_INFO_Update+0x130>)
300095dc:	4293      	cmp	r3, r2
300095de:	d91b      	bls.n	30009618 <PSRAM_INFO_Update+0xa0>
300095e0:	4a32      	ldr	r2, [pc, #200]	; (300096ac <PSRAM_INFO_Update+0x134>)
300095e2:	4293      	cmp	r3, r2
300095e4:	d828      	bhi.n	30009638 <PSRAM_INFO_Update+0xc0>
300095e6:	2306      	movs	r3, #6
300095e8:	e9c4 3306 	strd	r3, r3, [r4, #24]
300095ec:	e7f0      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
300095ee:	4293      	cmp	r3, r2
300095f0:	d90d      	bls.n	3000960e <PSRAM_INFO_Update+0x96>
300095f2:	4a2d      	ldr	r2, [pc, #180]	; (300096a8 <PSRAM_INFO_Update+0x130>)
300095f4:	4293      	cmp	r3, r2
300095f6:	d814      	bhi.n	30009622 <PSRAM_INFO_Update+0xaa>
300095f8:	2205      	movs	r2, #5
300095fa:	230f      	movs	r3, #15
300095fc:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009600:	e7e6      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
30009602:	4629      	mov	r1, r5
30009604:	482a      	ldr	r0, [pc, #168]	; (300096b0 <PSRAM_INFO_Update+0x138>)
30009606:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
3000960a:	f002 be0d 	b.w	3000c228 <__DiagPrintf_veneer>
3000960e:	2204      	movs	r2, #4
30009610:	230f      	movs	r3, #15
30009612:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009616:	e7db      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
30009618:	2205      	movs	r2, #5
3000961a:	2308      	movs	r3, #8
3000961c:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009620:	e7d6      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
30009622:	4a22      	ldr	r2, [pc, #136]	; (300096ac <PSRAM_INFO_Update+0x134>)
30009624:	4293      	cmp	r3, r2
30009626:	d91b      	bls.n	30009660 <PSRAM_INFO_Update+0xe8>
30009628:	4a22      	ldr	r2, [pc, #136]	; (300096b4 <PSRAM_INFO_Update+0x13c>)
3000962a:	4293      	cmp	r3, r2
3000962c:	d822      	bhi.n	30009674 <PSRAM_INFO_Update+0xfc>
3000962e:	2207      	movs	r2, #7
30009630:	2314      	movs	r3, #20
30009632:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009636:	e7cb      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
30009638:	4a1e      	ldr	r2, [pc, #120]	; (300096b4 <PSRAM_INFO_Update+0x13c>)
3000963a:	4293      	cmp	r3, r2
3000963c:	d915      	bls.n	3000966a <PSRAM_INFO_Update+0xf2>
3000963e:	4a1e      	ldr	r2, [pc, #120]	; (300096b8 <PSRAM_INFO_Update+0x140>)
30009640:	4293      	cmp	r3, r2
30009642:	bf8c      	ite	hi
30009644:	2300      	movhi	r3, #0
30009646:	2301      	movls	r3, #1
30009648:	2804      	cmp	r0, #4
3000964a:	d100      	bne.n	3000964e <PSRAM_INFO_Update+0xd6>
3000964c:	b96b      	cbnz	r3, 3000966a <PSRAM_INFO_Update+0xf2>
3000964e:	2803      	cmp	r0, #3
30009650:	d1be      	bne.n	300095d0 <PSRAM_INFO_Update+0x58>
30009652:	2b00      	cmp	r3, #0
30009654:	d0bc      	beq.n	300095d0 <PSRAM_INFO_Update+0x58>
30009656:	220a      	movs	r2, #10
30009658:	2306      	movs	r3, #6
3000965a:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000965e:	e7b7      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
30009660:	2206      	movs	r2, #6
30009662:	2312      	movs	r3, #18
30009664:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009668:	e7b2      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
3000966a:	2207      	movs	r2, #7
3000966c:	2306      	movs	r3, #6
3000966e:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009672:	e7ad      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
30009674:	4a10      	ldr	r2, [pc, #64]	; (300096b8 <PSRAM_INFO_Update+0x140>)
30009676:	4293      	cmp	r3, r2
30009678:	d8aa      	bhi.n	300095d0 <PSRAM_INFO_Update+0x58>
3000967a:	2208      	movs	r2, #8
3000967c:	231c      	movs	r3, #28
3000967e:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009682:	e7a5      	b.n	300095d0 <PSRAM_INFO_Update+0x58>
30009684:	42008000 	.word	0x42008000
30009688:	3000e424 	.word	0x3000e424
3000968c:	000f4240 	.word	0x000f4240
30009690:	3000eb20 	.word	0x3000eb20
30009694:	30007ffd 	.word	0x30007ffd
30009698:	30007fc5 	.word	0x30007fc5
3000969c:	3b9aca00 	.word	0x3b9aca00
300096a0:	0632ea00 	.word	0x0632ea00
300096a4:	2001c00c 	.word	0x2001c00c
300096a8:	07ed6b40 	.word	0x07ed6b40
300096ac:	09e4f580 	.word	0x09e4f580
300096b0:	3000d5b8 	.word	0x3000d5b8
300096b4:	0bebc200 	.word	0x0bebc200
300096b8:	0ee6b280 	.word	0x0ee6b280

300096bc <PSRAM_PHY_StructInit>:
300096bc:	2203      	movs	r2, #3
300096be:	2301      	movs	r3, #1
300096c0:	210a      	movs	r1, #10
300096c2:	b410      	push	{r4}
300096c4:	e9c0 2202 	strd	r2, r2, [r0, #8]
300096c8:	2402      	movs	r4, #2
300096ca:	2207      	movs	r2, #7
300096cc:	e9c0 3400 	strd	r3, r4, [r0]
300096d0:	e9c0 1304 	strd	r1, r3, [r0, #16]
300096d4:	f85d 4b04 	ldr.w	r4, [sp], #4
300096d8:	6182      	str	r2, [r0, #24]
300096da:	4770      	bx	lr

300096dc <PSRAM_PHY_Init>:
300096dc:	b538      	push	{r3, r4, r5, lr}
300096de:	4604      	mov	r4, r0
300096e0:	6903      	ldr	r3, [r0, #16]
300096e2:	4d14      	ldr	r5, [pc, #80]	; (30009734 <PSRAM_PHY_Init+0x58>)
300096e4:	f003 031f 	and.w	r3, r3, #31
300096e8:	4913      	ldr	r1, [pc, #76]	; (30009738 <PSRAM_PHY_Init+0x5c>)
300096ea:	e9d0 0200 	ldrd	r0, r2, [r0]
300096ee:	0612      	lsls	r2, r2, #24
300096f0:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
300096f4:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
300096f8:	4313      	orrs	r3, r2
300096fa:	68a2      	ldr	r2, [r4, #8]
300096fc:	0412      	lsls	r2, r2, #16
300096fe:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009702:	4313      	orrs	r3, r2
30009704:	68e2      	ldr	r2, [r4, #12]
30009706:	0212      	lsls	r2, r2, #8
30009708:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000970c:	4313      	orrs	r3, r2
3000970e:	606b      	str	r3, [r5, #4]
30009710:	4788      	blx	r1
30009712:	2801      	cmp	r0, #1
30009714:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30009718:	bf08      	it	eq
3000971a:	4a08      	ldreq	r2, [pc, #32]	; (3000973c <PSRAM_PHY_Init+0x60>)
3000971c:	ea4f 1303 	mov.w	r3, r3, lsl #4
30009720:	f001 010f 	and.w	r1, r1, #15
30009724:	bf18      	it	ne
30009726:	4a06      	ldrne	r2, [pc, #24]	; (30009740 <PSRAM_PHY_Init+0x64>)
30009728:	b2db      	uxtb	r3, r3
3000972a:	430b      	orrs	r3, r1
3000972c:	431a      	orrs	r2, r3
3000972e:	61aa      	str	r2, [r5, #24]
30009730:	bd38      	pop	{r3, r4, r5, pc}
30009732:	bf00      	nop
30009734:	41002000 	.word	0x41002000
30009738:	30007ffd 	.word	0x30007ffd
3000973c:	80000e00 	.word	0x80000e00
30009740:	80000f00 	.word	0x80000f00

30009744 <PSRAM_CTRL_Init>:
30009744:	b470      	push	{r4, r5, r6}
30009746:	4b4c      	ldr	r3, [pc, #304]	; (30009878 <PSRAM_CTRL_Init+0x134>)
30009748:	2400      	movs	r4, #0
3000974a:	4a4c      	ldr	r2, [pc, #304]	; (3000987c <PSRAM_CTRL_Init+0x138>)
3000974c:	f24d 0101 	movw	r1, #53249	; 0xd001
30009750:	609c      	str	r4, [r3, #8]
30009752:	2001      	movs	r0, #1
30009754:	601a      	str	r2, [r3, #0]
30009756:	4a4a      	ldr	r2, [pc, #296]	; (30009880 <PSRAM_CTRL_Init+0x13c>)
30009758:	6158      	str	r0, [r3, #20]
3000975a:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
3000975e:	6811      	ldr	r1, [r2, #0]
30009760:	2902      	cmp	r1, #2
30009762:	d04a      	beq.n	300097fa <PSRAM_CTRL_Init+0xb6>
30009764:	6954      	ldr	r4, [r2, #20]
30009766:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
3000976a:	69d0      	ldr	r0, [r2, #28]
3000976c:	6915      	ldr	r5, [r2, #16]
3000976e:	fbb0 f0f4 	udiv	r0, r0, r4
30009772:	fbb1 f1f4 	udiv	r1, r1, r4
30009776:	6994      	ldr	r4, [r2, #24]
30009778:	f005 020f 	and.w	r2, r5, #15
3000977c:	f241 5551 	movw	r5, #5457	; 0x1551
30009780:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30009784:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30009788:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
3000978c:	094a      	lsrs	r2, r1, #5
3000978e:	0181      	lsls	r1, r0, #6
30009790:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30009794:	4d3b      	ldr	r5, [pc, #236]	; (30009884 <PSRAM_CTRL_Init+0x140>)
30009796:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
3000979a:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
3000979e:	00a2      	lsls	r2, r4, #2
300097a0:	4301      	orrs	r1, r0
300097a2:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
300097a6:	3a01      	subs	r2, #1
300097a8:	430d      	orrs	r5, r1
300097aa:	f242 0120 	movw	r1, #8224	; 0x2020
300097ae:	f3c2 020b 	ubfx	r2, r2, #0, #12
300097b2:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
300097b6:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
300097ba:	f44f 2180 	mov.w	r1, #262144	; 0x40000
300097be:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
300097c2:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
300097c6:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300097ca:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
300097ce:	4420      	add	r0, r4
300097d0:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
300097d4:	f640 71fe 	movw	r1, #4094	; 0xffe
300097d8:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
300097dc:	ea01 0140 	and.w	r1, r1, r0, lsl #1
300097e0:	432a      	orrs	r2, r5
300097e2:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300097e6:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300097ea:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
300097ee:	4a22      	ldr	r2, [pc, #136]	; (30009878 <PSRAM_CTRL_Init+0x134>)
300097f0:	6a93      	ldr	r3, [r2, #40]	; 0x28
300097f2:	061b      	lsls	r3, r3, #24
300097f4:	d5fc      	bpl.n	300097f0 <PSRAM_CTRL_Init+0xac>
300097f6:	bc70      	pop	{r4, r5, r6}
300097f8:	4770      	bx	lr
300097fa:	6955      	ldr	r5, [r2, #20]
300097fc:	207d      	movs	r0, #125	; 0x7d
300097fe:	69d1      	ldr	r1, [r2, #28]
30009800:	26a0      	movs	r6, #160	; 0xa0
30009802:	6914      	ldr	r4, [r2, #16]
30009804:	6992      	ldr	r2, [r2, #24]
30009806:	f004 040f 	and.w	r4, r4, #15
3000980a:	0092      	lsls	r2, r2, #2
3000980c:	fbb1 f1f5 	udiv	r1, r1, r5
30009810:	fbb0 f5f5 	udiv	r5, r0, r5
30009814:	f245 5051 	movw	r0, #21841	; 0x5551
30009818:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
3000981c:	f001 003f 	and.w	r0, r1, #63	; 0x3f
30009820:	0189      	lsls	r1, r1, #6
30009822:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
30009826:	f641 4570 	movw	r5, #7280	; 0x1c70
3000982a:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
3000982e:	4325      	orrs	r5, r4
30009830:	2420      	movs	r4, #32
30009832:	4308      	orrs	r0, r1
30009834:	1e51      	subs	r1, r2, #1
30009836:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
3000983a:	3a02      	subs	r2, #2
3000983c:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
30009840:	f3c1 010b 	ubfx	r1, r1, #0, #12
30009844:	f3c2 020b 	ubfx	r2, r2, #0, #12
30009848:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
3000984c:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
30009850:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30009854:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30009858:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
3000985c:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30009860:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30009864:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30009868:	4301      	orrs	r1, r0
3000986a:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
3000986e:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30009872:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30009876:	e7ba      	b.n	300097ee <PSRAM_CTRL_Init+0xaa>
30009878:	41012000 	.word	0x41012000
3000987c:	00bfe000 	.word	0x00bfe000
30009880:	3000eb20 	.word	0x3000eb20
30009884:	00107000 	.word	0x00107000

30009888 <PSRAM_REG_Write>:
30009888:	b430      	push	{r4, r5}
3000988a:	4c20      	ldr	r4, [pc, #128]	; (3000990c <PSRAM_REG_Write+0x84>)
3000988c:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000988e:	f013 0301 	ands.w	r3, r3, #1
30009892:	d1fb      	bne.n	3000988c <PSRAM_REG_Write+0x4>
30009894:	6825      	ldr	r5, [r4, #0]
30009896:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
3000989a:	6025      	str	r5, [r4, #0]
3000989c:	60a3      	str	r3, [r4, #8]
3000989e:	6825      	ldr	r5, [r4, #0]
300098a0:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300098a4:	6025      	str	r5, [r4, #0]
300098a6:	6063      	str	r3, [r4, #4]
300098a8:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
300098ac:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
300098b0:	23c0      	movs	r3, #192	; 0xc0
300098b2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098b6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098ba:	0e03      	lsrs	r3, r0, #24
300098bc:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098c0:	f3c0 4307 	ubfx	r3, r0, #16, #8
300098c4:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098c8:	f3c0 2307 	ubfx	r3, r0, #8, #8
300098cc:	b2c0      	uxtb	r0, r0
300098ce:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098d2:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300098d6:	b141      	cbz	r1, 300098ea <PSRAM_REG_Write+0x62>
300098d8:	3a01      	subs	r2, #1
300098da:	480c      	ldr	r0, [pc, #48]	; (3000990c <PSRAM_REG_Write+0x84>)
300098dc:	4411      	add	r1, r2
300098de:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300098e2:	428a      	cmp	r2, r1
300098e4:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300098e8:	d1f9      	bne.n	300098de <PSRAM_REG_Write+0x56>
300098ea:	4a08      	ldr	r2, [pc, #32]	; (3000990c <PSRAM_REG_Write+0x84>)
300098ec:	2301      	movs	r3, #1
300098ee:	6093      	str	r3, [r2, #8]
300098f0:	6893      	ldr	r3, [r2, #8]
300098f2:	07d9      	lsls	r1, r3, #31
300098f4:	d4fc      	bmi.n	300098f0 <PSRAM_REG_Write+0x68>
300098f6:	4a05      	ldr	r2, [pc, #20]	; (3000990c <PSRAM_REG_Write+0x84>)
300098f8:	6a93      	ldr	r3, [r2, #40]	; 0x28
300098fa:	07db      	lsls	r3, r3, #31
300098fc:	d4fc      	bmi.n	300098f8 <PSRAM_REG_Write+0x70>
300098fe:	6813      	ldr	r3, [r2, #0]
30009900:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009904:	6013      	str	r3, [r2, #0]
30009906:	bc30      	pop	{r4, r5}
30009908:	4770      	bx	lr
3000990a:	bf00      	nop
3000990c:	41012000 	.word	0x41012000

30009910 <PSRAM_APM_DEVIC_Init>:
30009910:	4b12      	ldr	r3, [pc, #72]	; (3000995c <PSRAM_APM_DEVIC_Init+0x4c>)
30009912:	2102      	movs	r1, #2
30009914:	2000      	movs	r0, #0
30009916:	b530      	push	{r4, r5, lr}
30009918:	4c11      	ldr	r4, [pc, #68]	; (30009960 <PSRAM_APM_DEVIC_Init+0x50>)
3000991a:	b083      	sub	sp, #12
3000991c:	699d      	ldr	r5, [r3, #24]
3000991e:	466a      	mov	r2, sp
30009920:	eb04 0c05 	add.w	ip, r4, r5
30009924:	1eeb      	subs	r3, r5, #3
30009926:	4c0f      	ldr	r4, [pc, #60]	; (30009964 <PSRAM_APM_DEVIC_Init+0x54>)
30009928:	408b      	lsls	r3, r1
3000992a:	f81c cc03 	ldrb.w	ip, [ip, #-3]
3000992e:	f003 031c 	and.w	r3, r3, #28
30009932:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30009936:	f043 0320 	orr.w	r3, r3, #32
3000993a:	fa5f fc8c 	uxtb.w	ip, ip
3000993e:	f88d 3000 	strb.w	r3, [sp]
30009942:	f88d c004 	strb.w	ip, [sp, #4]
30009946:	f88d 3001 	strb.w	r3, [sp, #1]
3000994a:	f88d c005 	strb.w	ip, [sp, #5]
3000994e:	47a0      	blx	r4
30009950:	aa01      	add	r2, sp, #4
30009952:	2102      	movs	r1, #2
30009954:	2004      	movs	r0, #4
30009956:	47a0      	blx	r4
30009958:	b003      	add	sp, #12
3000995a:	bd30      	pop	{r4, r5, pc}
3000995c:	3000eb20 	.word	0x3000eb20
30009960:	3000e518 	.word	0x3000e518
30009964:	30009889 	.word	0x30009889

30009968 <PSRAM_WB_REG_Write>:
30009968:	b430      	push	{r4, r5}
3000996a:	4c1e      	ldr	r4, [pc, #120]	; (300099e4 <PSRAM_WB_REG_Write+0x7c>)
3000996c:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000996e:	f013 0301 	ands.w	r3, r3, #1
30009972:	d1fb      	bne.n	3000996c <PSRAM_WB_REG_Write+0x4>
30009974:	6825      	ldr	r5, [r4, #0]
30009976:	b2c0      	uxtb	r0, r0
30009978:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
3000997c:	6025      	str	r5, [r4, #0]
3000997e:	60a3      	str	r3, [r4, #8]
30009980:	6825      	ldr	r5, [r4, #0]
30009982:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009986:	6025      	str	r5, [r4, #0]
30009988:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
3000998c:	6063      	str	r3, [r4, #4]
3000998e:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
30009992:	2560      	movs	r5, #96	; 0x60
30009994:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30009998:	2501      	movs	r5, #1
3000999a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000999e:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300099a2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300099a6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300099aa:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300099ae:	b141      	cbz	r1, 300099c2 <PSRAM_WB_REG_Write+0x5a>
300099b0:	3a01      	subs	r2, #1
300099b2:	480c      	ldr	r0, [pc, #48]	; (300099e4 <PSRAM_WB_REG_Write+0x7c>)
300099b4:	4411      	add	r1, r2
300099b6:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300099ba:	428a      	cmp	r2, r1
300099bc:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300099c0:	d1f9      	bne.n	300099b6 <PSRAM_WB_REG_Write+0x4e>
300099c2:	4a08      	ldr	r2, [pc, #32]	; (300099e4 <PSRAM_WB_REG_Write+0x7c>)
300099c4:	2301      	movs	r3, #1
300099c6:	6093      	str	r3, [r2, #8]
300099c8:	6893      	ldr	r3, [r2, #8]
300099ca:	07d9      	lsls	r1, r3, #31
300099cc:	d4fc      	bmi.n	300099c8 <PSRAM_WB_REG_Write+0x60>
300099ce:	4a05      	ldr	r2, [pc, #20]	; (300099e4 <PSRAM_WB_REG_Write+0x7c>)
300099d0:	6a93      	ldr	r3, [r2, #40]	; 0x28
300099d2:	07db      	lsls	r3, r3, #31
300099d4:	d4fc      	bmi.n	300099d0 <PSRAM_WB_REG_Write+0x68>
300099d6:	6813      	ldr	r3, [r2, #0]
300099d8:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300099dc:	6013      	str	r3, [r2, #0]
300099de:	bc30      	pop	{r4, r5}
300099e0:	4770      	bx	lr
300099e2:	bf00      	nop
300099e4:	41012000 	.word	0x41012000

300099e8 <PSRAM_WB_DEVIC_Init>:
300099e8:	490b      	ldr	r1, [pc, #44]	; (30009a18 <PSRAM_WB_DEVIC_Init+0x30>)
300099ea:	4b0c      	ldr	r3, [pc, #48]	; (30009a1c <PSRAM_WB_DEVIC_Init+0x34>)
300099ec:	b510      	push	{r4, lr}
300099ee:	6988      	ldr	r0, [r1, #24]
300099f0:	b082      	sub	sp, #8
300099f2:	248f      	movs	r4, #143	; 0x8f
300099f4:	2102      	movs	r1, #2
300099f6:	4403      	add	r3, r0
300099f8:	aa01      	add	r2, sp, #4
300099fa:	f88d 4004 	strb.w	r4, [sp, #4]
300099fe:	2000      	movs	r0, #0
30009a00:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30009a04:	4c06      	ldr	r4, [pc, #24]	; (30009a20 <PSRAM_WB_DEVIC_Init+0x38>)
30009a06:	011b      	lsls	r3, r3, #4
30009a08:	f043 030c 	orr.w	r3, r3, #12
30009a0c:	f88d 3005 	strb.w	r3, [sp, #5]
30009a10:	47a0      	blx	r4
30009a12:	b002      	add	sp, #8
30009a14:	bd10      	pop	{r4, pc}
30009a16:	bf00      	nop
30009a18:	3000eb20 	.word	0x3000eb20
30009a1c:	3000e538 	.word	0x3000e538
30009a20:	30009969 	.word	0x30009969

30009a24 <PSRAM_calibration>:
30009a24:	4a97      	ldr	r2, [pc, #604]	; (30009c84 <PSRAM_calibration+0x260>)
30009a26:	2300      	movs	r3, #0
30009a28:	4997      	ldr	r1, [pc, #604]	; (30009c88 <PSRAM_calibration+0x264>)
30009a2a:	4898      	ldr	r0, [pc, #608]	; (30009c8c <PSRAM_calibration+0x268>)
30009a2c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009a30:	6852      	ldr	r2, [r2, #4]
30009a32:	b08f      	sub	sp, #60	; 0x3c
30009a34:	e9cd 2307 	strd	r2, r3, [sp, #28]
30009a38:	400a      	ands	r2, r1
30009a3a:	930d      	str	r3, [sp, #52]	; 0x34
30009a3c:	e9cd 3309 	strd	r3, r3, [sp, #36]	; 0x24
30009a40:	e9cd 330b 	strd	r3, r3, [sp, #44]	; 0x2c
30009a44:	9206      	str	r2, [sp, #24]
30009a46:	6942      	ldr	r2, [r0, #20]
30009a48:	03d1      	lsls	r1, r2, #15
30009a4a:	d51e      	bpl.n	30009a8a <PSRAM_calibration+0x66>
30009a4c:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009a50:	f3bf 8f4f 	dsb	sy
30009a54:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009a58:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009a5c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009a60:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009a64:	0164      	lsls	r4, r4, #5
30009a66:	ea04 0106 	and.w	r1, r4, r6
30009a6a:	462b      	mov	r3, r5
30009a6c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009a70:	3b01      	subs	r3, #1
30009a72:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009a76:	1c5a      	adds	r2, r3, #1
30009a78:	d1f8      	bne.n	30009a6c <PSRAM_calibration+0x48>
30009a7a:	3c20      	subs	r4, #32
30009a7c:	f114 0f20 	cmn.w	r4, #32
30009a80:	d1f1      	bne.n	30009a66 <PSRAM_calibration+0x42>
30009a82:	f3bf 8f4f 	dsb	sy
30009a86:	f3bf 8f6f 	isb	sy
30009a8a:	4880      	ldr	r0, [pc, #512]	; (30009c8c <PSRAM_calibration+0x268>)
30009a8c:	2300      	movs	r3, #0
30009a8e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009a92:	f3bf 8f4f 	dsb	sy
30009a96:	6943      	ldr	r3, [r0, #20]
30009a98:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30009a9c:	6143      	str	r3, [r0, #20]
30009a9e:	f3bf 8f4f 	dsb	sy
30009aa2:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009aa6:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009aaa:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009aae:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009ab2:	0164      	lsls	r4, r4, #5
30009ab4:	ea04 0106 	and.w	r1, r4, r6
30009ab8:	462b      	mov	r3, r5
30009aba:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009abe:	3b01      	subs	r3, #1
30009ac0:	1c5f      	adds	r7, r3, #1
30009ac2:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009ac6:	d1f8      	bne.n	30009aba <PSRAM_calibration+0x96>
30009ac8:	3c20      	subs	r4, #32
30009aca:	f114 0f20 	cmn.w	r4, #32
30009ace:	d1f1      	bne.n	30009ab4 <PSRAM_calibration+0x90>
30009ad0:	f3bf 8f4f 	dsb	sy
30009ad4:	f3bf 8f6f 	isb	sy
30009ad8:	2204      	movs	r2, #4
30009ada:	461e      	mov	r6, r3
30009adc:	9302      	str	r3, [sp, #8]
30009ade:	2700      	movs	r7, #0
30009ae0:	9205      	str	r2, [sp, #20]
30009ae2:	46b3      	mov	fp, r6
30009ae4:	4a67      	ldr	r2, [pc, #412]	; (30009c84 <PSRAM_calibration+0x260>)
30009ae6:	4c6a      	ldr	r4, [pc, #424]	; (30009c90 <PSRAM_calibration+0x26c>)
30009ae8:	6813      	ldr	r3, [r2, #0]
30009aea:	f8df 91b8 	ldr.w	r9, [pc, #440]	; 30009ca4 <PSRAM_calibration+0x280>
30009aee:	f023 0301 	bic.w	r3, r3, #1
30009af2:	f8df a1b4 	ldr.w	sl, [pc, #436]	; 30009ca8 <PSRAM_calibration+0x284>
30009af6:	6013      	str	r3, [r2, #0]
30009af8:	2301      	movs	r3, #1
30009afa:	9301      	str	r3, [sp, #4]
30009afc:	9b06      	ldr	r3, [sp, #24]
30009afe:	9a01      	ldr	r2, [sp, #4]
30009b00:	9700      	str	r7, [sp, #0]
30009b02:	2700      	movs	r7, #0
30009b04:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30009b08:	463d      	mov	r5, r7
30009b0a:	e9cd 6b03 	strd	r6, fp, [sp, #12]
30009b0e:	f04f 3bff 	mov.w	fp, #4294967295
30009b12:	465e      	mov	r6, fp
30009b14:	e010      	b.n	30009b38 <PSRAM_calibration+0x114>
30009b16:	f1b6 3fff 	cmp.w	r6, #4294967295
30009b1a:	bf08      	it	eq
30009b1c:	462e      	moveq	r6, r5
30009b1e:	2d1f      	cmp	r5, #31
30009b20:	eb06 0b07 	add.w	fp, r6, r7
30009b24:	f107 0701 	add.w	r7, r7, #1
30009b28:	d102      	bne.n	30009b30 <PSRAM_calibration+0x10c>
30009b2a:	9b00      	ldr	r3, [sp, #0]
30009b2c:	429f      	cmp	r7, r3
30009b2e:	dc46      	bgt.n	30009bbe <PSRAM_calibration+0x19a>
30009b30:	3501      	adds	r5, #1
30009b32:	2d20      	cmp	r5, #32
30009b34:	f000 80a1 	beq.w	30009c7a <PSRAM_calibration+0x256>
30009b38:	ea48 0205 	orr.w	r2, r8, r5
30009b3c:	4b51      	ldr	r3, [pc, #324]	; (30009c84 <PSRAM_calibration+0x260>)
30009b3e:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
30009b42:	4954      	ldr	r1, [pc, #336]	; (30009c94 <PSRAM_calibration+0x270>)
30009b44:	605a      	str	r2, [r3, #4]
30009b46:	6822      	ldr	r2, [r4, #0]
30009b48:	4b53      	ldr	r3, [pc, #332]	; (30009c98 <PSRAM_calibration+0x274>)
30009b4a:	f8ce 2000 	str.w	r2, [lr]
30009b4e:	6860      	ldr	r0, [r4, #4]
30009b50:	4a52      	ldr	r2, [pc, #328]	; (30009c9c <PSRAM_calibration+0x278>)
30009b52:	f8c9 0000 	str.w	r0, [r9]
30009b56:	68a0      	ldr	r0, [r4, #8]
30009b58:	f8ca 0000 	str.w	r0, [sl]
30009b5c:	68e0      	ldr	r0, [r4, #12]
30009b5e:	6018      	str	r0, [r3, #0]
30009b60:	6920      	ldr	r0, [r4, #16]
30009b62:	6008      	str	r0, [r1, #0]
30009b64:	6960      	ldr	r0, [r4, #20]
30009b66:	6010      	str	r0, [r2, #0]
30009b68:	f8de 0000 	ldr.w	r0, [lr]
30009b6c:	9008      	str	r0, [sp, #32]
30009b6e:	f8d9 0000 	ldr.w	r0, [r9]
30009b72:	9009      	str	r0, [sp, #36]	; 0x24
30009b74:	f8da 0000 	ldr.w	r0, [sl]
30009b78:	900a      	str	r0, [sp, #40]	; 0x28
30009b7a:	6818      	ldr	r0, [r3, #0]
30009b7c:	4b48      	ldr	r3, [pc, #288]	; (30009ca0 <PSRAM_calibration+0x27c>)
30009b7e:	900b      	str	r0, [sp, #44]	; 0x2c
30009b80:	a808      	add	r0, sp, #32
30009b82:	6809      	ldr	r1, [r1, #0]
30009b84:	910c      	str	r1, [sp, #48]	; 0x30
30009b86:	6812      	ldr	r2, [r2, #0]
30009b88:	4941      	ldr	r1, [pc, #260]	; (30009c90 <PSRAM_calibration+0x26c>)
30009b8a:	920d      	str	r2, [sp, #52]	; 0x34
30009b8c:	2218      	movs	r2, #24
30009b8e:	4798      	blx	r3
30009b90:	2800      	cmp	r0, #0
30009b92:	d0c0      	beq.n	30009b16 <PSRAM_calibration+0xf2>
30009b94:	1c70      	adds	r0, r6, #1
30009b96:	d0cb      	beq.n	30009b30 <PSRAM_calibration+0x10c>
30009b98:	9b00      	ldr	r3, [sp, #0]
30009b9a:	42bb      	cmp	r3, r7
30009b9c:	da0a      	bge.n	30009bb4 <PSRAM_calibration+0x190>
30009b9e:	9b01      	ldr	r3, [sp, #4]
30009ba0:	f8cd b010 	str.w	fp, [sp, #16]
30009ba4:	f04f 3bff 	mov.w	fp, #4294967295
30009ba8:	9700      	str	r7, [sp, #0]
30009baa:	2700      	movs	r7, #0
30009bac:	9603      	str	r6, [sp, #12]
30009bae:	465e      	mov	r6, fp
30009bb0:	9302      	str	r3, [sp, #8]
30009bb2:	e7bd      	b.n	30009b30 <PSRAM_calibration+0x10c>
30009bb4:	f04f 3bff 	mov.w	fp, #4294967295
30009bb8:	2700      	movs	r7, #0
30009bba:	465e      	mov	r6, fp
30009bbc:	e7b8      	b.n	30009b30 <PSRAM_calibration+0x10c>
30009bbe:	9b01      	ldr	r3, [sp, #4]
30009bc0:	9302      	str	r3, [sp, #8]
30009bc2:	9b01      	ldr	r3, [sp, #4]
30009bc4:	005b      	lsls	r3, r3, #1
30009bc6:	9301      	str	r3, [sp, #4]
30009bc8:	9b05      	ldr	r3, [sp, #20]
30009bca:	3b01      	subs	r3, #1
30009bcc:	9305      	str	r3, [sp, #20]
30009bce:	d195      	bne.n	30009afc <PSRAM_calibration+0xd8>
30009bd0:	482e      	ldr	r0, [pc, #184]	; (30009c8c <PSRAM_calibration+0x268>)
30009bd2:	6943      	ldr	r3, [r0, #20]
30009bd4:	03d9      	lsls	r1, r3, #15
30009bd6:	d425      	bmi.n	30009c24 <PSRAM_calibration+0x200>
30009bd8:	9b05      	ldr	r3, [sp, #20]
30009bda:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009bde:	f3bf 8f4f 	dsb	sy
30009be2:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009be6:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30009bea:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009bee:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009bf2:	0164      	lsls	r4, r4, #5
30009bf4:	ea04 010c 	and.w	r1, r4, ip
30009bf8:	462b      	mov	r3, r5
30009bfa:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009bfe:	3b01      	subs	r3, #1
30009c00:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30009c04:	1c5a      	adds	r2, r3, #1
30009c06:	d1f8      	bne.n	30009bfa <PSRAM_calibration+0x1d6>
30009c08:	3c20      	subs	r4, #32
30009c0a:	f114 0f20 	cmn.w	r4, #32
30009c0e:	d1f1      	bne.n	30009bf4 <PSRAM_calibration+0x1d0>
30009c10:	f3bf 8f4f 	dsb	sy
30009c14:	6943      	ldr	r3, [r0, #20]
30009c16:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30009c1a:	6143      	str	r3, [r0, #20]
30009c1c:	f3bf 8f4f 	dsb	sy
30009c20:	f3bf 8f6f 	isb	sy
30009c24:	2f08      	cmp	r7, #8
30009c26:	dd24      	ble.n	30009c72 <PSRAM_calibration+0x24e>
30009c28:	ebbb 0306 	subs.w	r3, fp, r6
30009c2c:	9802      	ldr	r0, [sp, #8]
30009c2e:	bf48      	it	mi
30009c30:	3301      	addmi	r3, #1
30009c32:	eb1b 0606 	adds.w	r6, fp, r6
30009c36:	ea4f 0363 	mov.w	r3, r3, asr #1
30009c3a:	bf48      	it	mi
30009c3c:	3601      	addmi	r6, #1
30009c3e:	1e9a      	subs	r2, r3, #2
30009c40:	9b07      	ldr	r3, [sp, #28]
30009c42:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009c46:	0414      	lsls	r4, r2, #16
30009c48:	f3c6 0344 	ubfx	r3, r6, #1, #5
30009c4c:	0212      	lsls	r2, r2, #8
30009c4e:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009c52:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009c56:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009c5a:	2001      	movs	r0, #1
30009c5c:	430b      	orrs	r3, r1
30009c5e:	4909      	ldr	r1, [pc, #36]	; (30009c84 <PSRAM_calibration+0x260>)
30009c60:	4323      	orrs	r3, r4
30009c62:	4313      	orrs	r3, r2
30009c64:	604b      	str	r3, [r1, #4]
30009c66:	680b      	ldr	r3, [r1, #0]
30009c68:	4303      	orrs	r3, r0
30009c6a:	600b      	str	r3, [r1, #0]
30009c6c:	b00f      	add	sp, #60	; 0x3c
30009c6e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009c72:	2000      	movs	r0, #0
30009c74:	b00f      	add	sp, #60	; 0x3c
30009c76:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009c7a:	9f00      	ldr	r7, [sp, #0]
30009c7c:	e9dd 6b03 	ldrd	r6, fp, [sp, #12]
30009c80:	e79f      	b.n	30009bc2 <PSRAM_calibration+0x19e>
30009c82:	bf00      	nop
30009c84:	41002000 	.word	0x41002000
30009c88:	0fffffe0 	.word	0x0fffffe0
30009c8c:	e000ed00 	.word	0xe000ed00
30009c90:	3000e520 	.word	0x3000e520
30009c94:	60200000 	.word	0x60200000
30009c98:	60150000 	.word	0x60150000
30009c9c:	60250000 	.word	0x60250000
30009ca0:	000129bd 	.word	0x000129bd
30009ca4:	60050000 	.word	0x60050000
30009ca8:	60100000 	.word	0x60100000

30009cac <PSRAM_AutoGating>:
30009cac:	b1c0      	cbz	r0, 30009ce0 <PSRAM_AutoGating+0x34>
30009cae:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30009cb2:	4810      	ldr	r0, [pc, #64]	; (30009cf4 <PSRAM_AutoGating+0x48>)
30009cb4:	f3c1 0108 	ubfx	r1, r1, #0, #9
30009cb8:	0252      	lsls	r2, r2, #9
30009cba:	b410      	push	{r4}
30009cbc:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
30009cc0:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
30009cc4:	4020      	ands	r0, r4
30009cc6:	4301      	orrs	r1, r0
30009cc8:	430a      	orrs	r2, r1
30009cca:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30009cce:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
30009cd2:	f042 0201 	orr.w	r2, r2, #1
30009cd6:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30009cda:	f85d 4b04 	ldr.w	r4, [sp], #4
30009cde:	4770      	bx	lr
30009ce0:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30009ce4:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30009ce8:	f023 0301 	bic.w	r3, r3, #1
30009cec:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30009cf0:	4770      	bx	lr
30009cf2:	bf00      	nop
30009cf4:	ffffc000 	.word	0xffffc000

30009cf8 <BOOT_Reason>:
30009cf8:	4b02      	ldr	r3, [pc, #8]	; (30009d04 <BOOT_Reason+0xc>)
30009cfa:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009cfe:	b280      	uxth	r0, r0
30009d00:	4770      	bx	lr
30009d02:	bf00      	nop
30009d04:	42008200 	.word	0x42008200

30009d08 <CRYPTO_SetSecurityModeAD>:
30009d08:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009d0c:	460d      	mov	r5, r1
30009d0e:	4616      	mov	r6, r2
30009d10:	4699      	mov	r9, r3
30009d12:	4604      	mov	r4, r0
30009d14:	e9dd 8709 	ldrd	r8, r7, [sp, #36]	; 0x24
30009d18:	2800      	cmp	r0, #0
30009d1a:	f000 80a6 	beq.w	30009e6a <CRYPTO_SetSecurityModeAD+0x162>
30009d1e:	7823      	ldrb	r3, [r4, #0]
30009d20:	2b01      	cmp	r3, #1
30009d22:	d003      	beq.n	30009d2c <CRYPTO_SetSecurityModeAD+0x24>
30009d24:	2124      	movs	r1, #36	; 0x24
30009d26:	485d      	ldr	r0, [pc, #372]	; (30009e9c <CRYPTO_SetSecurityModeAD+0x194>)
30009d28:	f002 fa6e 	bl	3000c208 <__io_assert_failed_veneer>
30009d2c:	2300      	movs	r3, #0
30009d2e:	1c69      	adds	r1, r5, #1
30009d30:	6065      	str	r5, [r4, #4]
30009d32:	8123      	strh	r3, [r4, #8]
30009d34:	d008      	beq.n	30009d48 <CRYPTO_SetSecurityModeAD+0x40>
30009d36:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009d3a:	2b20      	cmp	r3, #32
30009d3c:	d078      	beq.n	30009e30 <CRYPTO_SetSecurityModeAD+0x128>
30009d3e:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30009d42:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30009d46:	7265      	strb	r5, [r4, #9]
30009d48:	1c72      	adds	r2, r6, #1
30009d4a:	60e6      	str	r6, [r4, #12]
30009d4c:	d07f      	beq.n	30009e4e <CRYPTO_SetSecurityModeAD+0x146>
30009d4e:	f006 0303 	and.w	r3, r6, #3
30009d52:	f3c6 0280 	ubfx	r2, r6, #2, #1
30009d56:	7422      	strb	r2, [r4, #16]
30009d58:	f1a3 0202 	sub.w	r2, r3, #2
30009d5c:	2b01      	cmp	r3, #1
30009d5e:	fab2 f282 	clz	r2, r2
30009d62:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009d66:	7462      	strb	r2, [r4, #17]
30009d68:	fab3 f283 	clz	r2, r3
30009d6c:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009d70:	74a2      	strb	r2, [r4, #18]
30009d72:	bf0c      	ite	eq
30009d74:	2201      	moveq	r2, #1
30009d76:	2200      	movne	r2, #0
30009d78:	74e2      	strb	r2, [r4, #19]
30009d7a:	d13e      	bne.n	30009dfa <CRYPTO_SetSecurityModeAD+0xf2>
30009d7c:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30009d80:	2b40      	cmp	r3, #64	; 0x40
30009d82:	d07e      	beq.n	30009e82 <CRYPTO_SetSecurityModeAD+0x17a>
30009d84:	d869      	bhi.n	30009e5a <CRYPTO_SetSecurityModeAD+0x152>
30009d86:	2b10      	cmp	r3, #16
30009d88:	d06c      	beq.n	30009e64 <CRYPTO_SetSecurityModeAD+0x15c>
30009d8a:	2b20      	cmp	r3, #32
30009d8c:	d172      	bne.n	30009e74 <CRYPTO_SetSecurityModeAD+0x16c>
30009d8e:	7523      	strb	r3, [r4, #20]
30009d90:	62a3      	str	r3, [r4, #40]	; 0x28
30009d92:	9b08      	ldr	r3, [sp, #32]
30009d94:	6227      	str	r7, [r4, #32]
30009d96:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009d9a:	2f00      	cmp	r7, #0
30009d9c:	d038      	beq.n	30009e10 <CRYPTO_SetSecurityModeAD+0x108>
30009d9e:	7d23      	ldrb	r3, [r4, #20]
30009da0:	f104 0064 	add.w	r0, r4, #100	; 0x64
30009da4:	2b30      	cmp	r3, #48	; 0x30
30009da6:	65e0      	str	r0, [r4, #92]	; 0x5c
30009da8:	d045      	beq.n	30009e36 <CRYPTO_SetSecurityModeAD+0x12e>
30009daa:	2b40      	cmp	r3, #64	; 0x40
30009dac:	d043      	beq.n	30009e36 <CRYPTO_SetSecurityModeAD+0x12e>
30009dae:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30009db2:	2240      	movs	r2, #64	; 0x40
30009db4:	2136      	movs	r1, #54	; 0x36
30009db6:	4d3a      	ldr	r5, [pc, #232]	; (30009ea0 <CRYPTO_SetSecurityModeAD+0x198>)
30009db8:	6623      	str	r3, [r4, #96]	; 0x60
30009dba:	47a8      	blx	r5
30009dbc:	2240      	movs	r2, #64	; 0x40
30009dbe:	215c      	movs	r1, #92	; 0x5c
30009dc0:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009dc2:	47a8      	blx	r5
30009dc4:	4b37      	ldr	r3, [pc, #220]	; (30009ea4 <CRYPTO_SetSecurityModeAD+0x19c>)
30009dc6:	681d      	ldr	r5, [r3, #0]
30009dc8:	bb25      	cbnz	r5, 30009e14 <CRYPTO_SetSecurityModeAD+0x10c>
30009dca:	f1b8 0f00 	cmp.w	r8, #0
30009dce:	d060      	beq.n	30009e92 <CRYPTO_SetSecurityModeAD+0x18a>
30009dd0:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30009dd4:	e001      	b.n	30009dda <CRYPTO_SetSecurityModeAD+0xd2>
30009dd6:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30009dda:	6de2      	ldr	r2, [r4, #92]	; 0x5c
30009ddc:	f818 3005 	ldrb.w	r3, [r8, r5]
30009de0:	5d51      	ldrb	r1, [r2, r5]
30009de2:	404b      	eors	r3, r1
30009de4:	5553      	strb	r3, [r2, r5]
30009de6:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009de8:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009dea:	5d51      	ldrb	r1, [r2, r5]
30009dec:	5d5b      	ldrb	r3, [r3, r5]
30009dee:	404b      	eors	r3, r1
30009df0:	5553      	strb	r3, [r2, r5]
30009df2:	3501      	adds	r5, #1
30009df4:	42af      	cmp	r7, r5
30009df6:	d1ee      	bne.n	30009dd6 <CRYPTO_SetSecurityModeAD+0xce>
30009df8:	e00c      	b.n	30009e14 <CRYPTO_SetSecurityModeAD+0x10c>
30009dfa:	2200      	movs	r2, #0
30009dfc:	7522      	strb	r2, [r4, #20]
30009dfe:	b993      	cbnz	r3, 30009e26 <CRYPTO_SetSecurityModeAD+0x11e>
30009e00:	2314      	movs	r3, #20
30009e02:	6227      	str	r7, [r4, #32]
30009e04:	62a3      	str	r3, [r4, #40]	; 0x28
30009e06:	9b08      	ldr	r3, [sp, #32]
30009e08:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009e0c:	2f00      	cmp	r7, #0
30009e0e:	d1c6      	bne.n	30009d9e <CRYPTO_SetSecurityModeAD+0x96>
30009e10:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009e14:	3601      	adds	r6, #1
30009e16:	d003      	beq.n	30009e20 <CRYPTO_SetSecurityModeAD+0x118>
30009e18:	2201      	movs	r2, #1
30009e1a:	2300      	movs	r3, #0
30009e1c:	85a2      	strh	r2, [r4, #44]	; 0x2c
30009e1e:	6323      	str	r3, [r4, #48]	; 0x30
30009e20:	2000      	movs	r0, #0
30009e22:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009e26:	2b02      	cmp	r3, #2
30009e28:	d114      	bne.n	30009e54 <CRYPTO_SetSecurityModeAD+0x14c>
30009e2a:	2310      	movs	r3, #16
30009e2c:	62a3      	str	r3, [r4, #40]	; 0x28
30009e2e:	e7b0      	b.n	30009d92 <CRYPTO_SetSecurityModeAD+0x8a>
30009e30:	2301      	movs	r3, #1
30009e32:	7223      	strb	r3, [r4, #8]
30009e34:	e783      	b.n	30009d3e <CRYPTO_SetSecurityModeAD+0x36>
30009e36:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30009e3a:	2280      	movs	r2, #128	; 0x80
30009e3c:	2136      	movs	r1, #54	; 0x36
30009e3e:	4d18      	ldr	r5, [pc, #96]	; (30009ea0 <CRYPTO_SetSecurityModeAD+0x198>)
30009e40:	6623      	str	r3, [r4, #96]	; 0x60
30009e42:	47a8      	blx	r5
30009e44:	2280      	movs	r2, #128	; 0x80
30009e46:	215c      	movs	r1, #92	; 0x5c
30009e48:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009e4a:	47a8      	blx	r5
30009e4c:	e7ba      	b.n	30009dc4 <CRYPTO_SetSecurityModeAD+0xbc>
30009e4e:	2300      	movs	r3, #0
30009e50:	6123      	str	r3, [r4, #16]
30009e52:	7523      	strb	r3, [r4, #20]
30009e54:	2300      	movs	r3, #0
30009e56:	62a3      	str	r3, [r4, #40]	; 0x28
30009e58:	e79b      	b.n	30009d92 <CRYPTO_SetSecurityModeAD+0x8a>
30009e5a:	2b80      	cmp	r3, #128	; 0x80
30009e5c:	d10a      	bne.n	30009e74 <CRYPTO_SetSecurityModeAD+0x16c>
30009e5e:	2340      	movs	r3, #64	; 0x40
30009e60:	7523      	strb	r3, [r4, #20]
30009e62:	e795      	b.n	30009d90 <CRYPTO_SetSecurityModeAD+0x88>
30009e64:	231c      	movs	r3, #28
30009e66:	7523      	strb	r3, [r4, #20]
30009e68:	e7f5      	b.n	30009e56 <CRYPTO_SetSecurityModeAD+0x14e>
30009e6a:	2123      	movs	r1, #35	; 0x23
30009e6c:	480b      	ldr	r0, [pc, #44]	; (30009e9c <CRYPTO_SetSecurityModeAD+0x194>)
30009e6e:	f002 f9cb 	bl	3000c208 <__io_assert_failed_veneer>
30009e72:	e754      	b.n	30009d1e <CRYPTO_SetSecurityModeAD+0x16>
30009e74:	4b0c      	ldr	r3, [pc, #48]	; (30009ea8 <CRYPTO_SetSecurityModeAD+0x1a0>)
30009e76:	681b      	ldr	r3, [r3, #0]
30009e78:	049b      	lsls	r3, r3, #18
30009e7a:	d405      	bmi.n	30009e88 <CRYPTO_SetSecurityModeAD+0x180>
30009e7c:	2300      	movs	r3, #0
30009e7e:	7523      	strb	r3, [r4, #20]
30009e80:	e7e9      	b.n	30009e56 <CRYPTO_SetSecurityModeAD+0x14e>
30009e82:	2330      	movs	r3, #48	; 0x30
30009e84:	7523      	strb	r3, [r4, #20]
30009e86:	e7e6      	b.n	30009e56 <CRYPTO_SetSecurityModeAD+0x14e>
30009e88:	4631      	mov	r1, r6
30009e8a:	4808      	ldr	r0, [pc, #32]	; (30009eac <CRYPTO_SetSecurityModeAD+0x1a4>)
30009e8c:	f002 f9cc 	bl	3000c228 <__DiagPrintf_veneer>
30009e90:	e7f4      	b.n	30009e7c <CRYPTO_SetSecurityModeAD+0x174>
30009e92:	217f      	movs	r1, #127	; 0x7f
30009e94:	4801      	ldr	r0, [pc, #4]	; (30009e9c <CRYPTO_SetSecurityModeAD+0x194>)
30009e96:	f002 f9b7 	bl	3000c208 <__io_assert_failed_veneer>
30009e9a:	e799      	b.n	30009dd0 <CRYPTO_SetSecurityModeAD+0xc8>
30009e9c:	3000d68c 	.word	0x3000d68c
30009ea0:	00012be5 	.word	0x00012be5
30009ea4:	2001c200 	.word	0x2001c200
30009ea8:	2001c00c 	.word	0x2001c00c
30009eac:	3000d5ec 	.word	0x3000d5ec

30009eb0 <rtl_crypto_hmac_sha2_init>:
30009eb0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009eb4:	4c97      	ldr	r4, [pc, #604]	; (3000a114 <rtl_crypto_hmac_sha2_init+0x264>)
30009eb6:	4617      	mov	r7, r2
30009eb8:	e844 f400 	tt	r4, r4
30009ebc:	4a96      	ldr	r2, [pc, #600]	; (3000a118 <rtl_crypto_hmac_sha2_init+0x268>)
30009ebe:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
30009ec2:	461c      	mov	r4, r3
30009ec4:	4b95      	ldr	r3, [pc, #596]	; (3000a11c <rtl_crypto_hmac_sha2_init+0x26c>)
30009ec6:	bf18      	it	ne
30009ec8:	4613      	movne	r3, r2
30009eca:	b085      	sub	sp, #20
30009ecc:	4605      	mov	r5, r0
30009ece:	460e      	mov	r6, r1
30009ed0:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30009ed2:	2b00      	cmp	r3, #0
30009ed4:	db05      	blt.n	30009ee2 <rtl_crypto_hmac_sha2_init+0x32>
30009ed6:	2900      	cmp	r1, #0
30009ed8:	f000 8118 	beq.w	3000a10c <rtl_crypto_hmac_sha2_init+0x25c>
30009edc:	078b      	lsls	r3, r1, #30
30009ede:	f040 8112 	bne.w	3000a106 <rtl_crypto_hmac_sha2_init+0x256>
30009ee2:	2d20      	cmp	r5, #32
30009ee4:	bf18      	it	ne
30009ee6:	2d1c      	cmpne	r5, #28
30009ee8:	bf0c      	ite	eq
30009eea:	f04f 0901 	moveq.w	r9, #1
30009eee:	f04f 0900 	movne.w	r9, #0
30009ef2:	d04d      	beq.n	30009f90 <rtl_crypto_hmac_sha2_init+0xe0>
30009ef4:	2d40      	cmp	r5, #64	; 0x40
30009ef6:	bf18      	it	ne
30009ef8:	2d30      	cmpne	r5, #48	; 0x30
30009efa:	d102      	bne.n	30009f02 <rtl_crypto_hmac_sha2_init+0x52>
30009efc:	2f80      	cmp	r7, #128	; 0x80
30009efe:	f200 80f9 	bhi.w	3000a0f4 <rtl_crypto_hmac_sha2_init+0x244>
30009f02:	f8df 8238 	ldr.w	r8, [pc, #568]	; 3000a13c <rtl_crypto_hmac_sha2_init+0x28c>
30009f06:	f898 3000 	ldrb.w	r3, [r8]
30009f0a:	2b01      	cmp	r3, #1
30009f0c:	d003      	beq.n	30009f16 <rtl_crypto_hmac_sha2_init+0x66>
30009f0e:	21b3      	movs	r1, #179	; 0xb3
30009f10:	4883      	ldr	r0, [pc, #524]	; (3000a120 <rtl_crypto_hmac_sha2_init+0x270>)
30009f12:	f002 f979 	bl	3000c208 <__io_assert_failed_veneer>
30009f16:	2d30      	cmp	r5, #48	; 0x30
30009f18:	f000 809b 	beq.w	3000a052 <rtl_crypto_hmac_sha2_init+0x1a2>
30009f1c:	d83f      	bhi.n	30009f9e <rtl_crypto_hmac_sha2_init+0xee>
30009f1e:	2d1c      	cmp	r5, #28
30009f20:	f000 80cb 	beq.w	3000a0ba <rtl_crypto_hmac_sha2_init+0x20a>
30009f24:	f1b9 0f00 	cmp.w	r9, #0
30009f28:	d078      	beq.n	3000a01c <rtl_crypto_hmac_sha2_init+0x16c>
30009f2a:	2c00      	cmp	r4, #0
30009f2c:	f000 808f 	beq.w	3000a04e <rtl_crypto_hmac_sha2_init+0x19e>
30009f30:	4b7c      	ldr	r3, [pc, #496]	; (3000a124 <rtl_crypto_hmac_sha2_init+0x274>)
30009f32:	f8df e210 	ldr.w	lr, [pc, #528]	; 3000a144 <rtl_crypto_hmac_sha2_init+0x294>
30009f36:	f8df 921c 	ldr.w	r9, [pc, #540]	; 3000a154 <rtl_crypto_hmac_sha2_init+0x2a4>
30009f3a:	4a7b      	ldr	r2, [pc, #492]	; (3000a128 <rtl_crypto_hmac_sha2_init+0x278>)
30009f3c:	e9c4 e302 	strd	lr, r3, [r4, #8]
30009f40:	4b7a      	ldr	r3, [pc, #488]	; (3000a12c <rtl_crypto_hmac_sha2_init+0x27c>)
30009f42:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009f46:	4b7a      	ldr	r3, [pc, #488]	; (3000a130 <rtl_crypto_hmac_sha2_init+0x280>)
30009f48:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009f4c:	4b79      	ldr	r3, [pc, #484]	; (3000a134 <rtl_crypto_hmac_sha2_init+0x284>)
30009f4e:	2225      	movs	r2, #37	; 0x25
30009f50:	6223      	str	r3, [r4, #32]
30009f52:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009f56:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009f5a:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30009f5e:	6263      	str	r3, [r4, #36]	; 0x24
30009f60:	2300      	movs	r3, #0
30009f62:	f04f 31ff 	mov.w	r1, #4294967295
30009f66:	4d74      	ldr	r5, [pc, #464]	; (3000a138 <rtl_crypto_hmac_sha2_init+0x288>)
30009f68:	9300      	str	r3, [sp, #0]
30009f6a:	9702      	str	r7, [sp, #8]
30009f6c:	9601      	str	r6, [sp, #4]
30009f6e:	4873      	ldr	r0, [pc, #460]	; (3000a13c <rtl_crypto_hmac_sha2_init+0x28c>)
30009f70:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009f74:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009f78:	47a8      	blx	r5
30009f7a:	4605      	mov	r5, r0
30009f7c:	4b70      	ldr	r3, [pc, #448]	; (3000a140 <rtl_crypto_hmac_sha2_init+0x290>)
30009f7e:	4620      	mov	r0, r4
30009f80:	4798      	blx	r3
30009f82:	2301      	movs	r3, #1
30009f84:	4628      	mov	r0, r5
30009f86:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30009f8a:	b005      	add	sp, #20
30009f8c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009f90:	2f40      	cmp	r7, #64	; 0x40
30009f92:	d9b6      	bls.n	30009f02 <rtl_crypto_hmac_sha2_init+0x52>
30009f94:	21ad      	movs	r1, #173	; 0xad
30009f96:	4862      	ldr	r0, [pc, #392]	; (3000a120 <rtl_crypto_hmac_sha2_init+0x270>)
30009f98:	f002 f936 	bl	3000c208 <__io_assert_failed_veneer>
30009f9c:	e7b1      	b.n	30009f02 <rtl_crypto_hmac_sha2_init+0x52>
30009f9e:	2d40      	cmp	r5, #64	; 0x40
30009fa0:	d13c      	bne.n	3000a01c <rtl_crypto_hmac_sha2_init+0x16c>
30009fa2:	2c00      	cmp	r4, #0
30009fa4:	d040      	beq.n	3000a028 <rtl_crypto_hmac_sha2_init+0x178>
30009fa6:	4b67      	ldr	r3, [pc, #412]	; (3000a144 <rtl_crypto_hmac_sha2_init+0x294>)
30009fa8:	4a67      	ldr	r2, [pc, #412]	; (3000a148 <rtl_crypto_hmac_sha2_init+0x298>)
30009faa:	4968      	ldr	r1, [pc, #416]	; (3000a14c <rtl_crypto_hmac_sha2_init+0x29c>)
30009fac:	4d68      	ldr	r5, [pc, #416]	; (3000a150 <rtl_crypto_hmac_sha2_init+0x2a0>)
30009fae:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 3000a194 <rtl_crypto_hmac_sha2_init+0x2e4>
30009fb2:	e9c4 3202 	strd	r3, r2, [r4, #8]
30009fb6:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30009fba:	2285      	movs	r2, #133	; 0x85
30009fbc:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30009fc0:	33d4      	adds	r3, #212	; 0xd4
30009fc2:	6163      	str	r3, [r4, #20]
30009fc4:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30009fc8:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30009fcc:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30009fd0:	6123      	str	r3, [r4, #16]
30009fd2:	4b60      	ldr	r3, [pc, #384]	; (3000a154 <rtl_crypto_hmac_sha2_init+0x2a4>)
30009fd4:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009fd8:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30009fdc:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30009fe0:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009fe4:	6263      	str	r3, [r4, #36]	; 0x24
30009fe6:	4b51      	ldr	r3, [pc, #324]	; (3000a12c <rtl_crypto_hmac_sha2_init+0x27c>)
30009fe8:	6223      	str	r3, [r4, #32]
30009fea:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30009fee:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30009ff2:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009ff6:	62e3      	str	r3, [r4, #44]	; 0x2c
30009ff8:	4b4b      	ldr	r3, [pc, #300]	; (3000a128 <rtl_crypto_hmac_sha2_init+0x278>)
30009ffa:	62a3      	str	r3, [r4, #40]	; 0x28
30009ffc:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
3000a000:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
3000a004:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
3000a008:	6363      	str	r3, [r4, #52]	; 0x34
3000a00a:	4b49      	ldr	r3, [pc, #292]	; (3000a130 <rtl_crypto_hmac_sha2_init+0x280>)
3000a00c:	6323      	str	r3, [r4, #48]	; 0x30
3000a00e:	4b49      	ldr	r3, [pc, #292]	; (3000a134 <rtl_crypto_hmac_sha2_init+0x284>)
3000a010:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
3000a014:	4b50      	ldr	r3, [pc, #320]	; (3000a158 <rtl_crypto_hmac_sha2_init+0x2a8>)
3000a016:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
3000a01a:	e7a1      	b.n	30009f60 <rtl_crypto_hmac_sha2_init+0xb0>
3000a01c:	f04f 35ff 	mov.w	r5, #4294967295
3000a020:	4628      	mov	r0, r5
3000a022:	b005      	add	sp, #20
3000a024:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000a028:	2285      	movs	r2, #133	; 0x85
3000a02a:	2300      	movs	r3, #0
3000a02c:	f04f 31ff 	mov.w	r1, #4294967295
3000a030:	4c41      	ldr	r4, [pc, #260]	; (3000a138 <rtl_crypto_hmac_sha2_init+0x288>)
3000a032:	9702      	str	r7, [sp, #8]
3000a034:	4841      	ldr	r0, [pc, #260]	; (3000a13c <rtl_crypto_hmac_sha2_init+0x28c>)
3000a036:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
3000a03a:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
3000a03e:	e9cd 3600 	strd	r3, r6, [sp]
3000a042:	47a0      	blx	r4
3000a044:	4605      	mov	r5, r0
3000a046:	4628      	mov	r0, r5
3000a048:	b005      	add	sp, #20
3000a04a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000a04e:	2225      	movs	r2, #37	; 0x25
3000a050:	e7eb      	b.n	3000a02a <rtl_crypto_hmac_sha2_init+0x17a>
3000a052:	2c00      	cmp	r4, #0
3000a054:	d053      	beq.n	3000a0fe <rtl_crypto_hmac_sha2_init+0x24e>
3000a056:	4b41      	ldr	r3, [pc, #260]	; (3000a15c <rtl_crypto_hmac_sha2_init+0x2ac>)
3000a058:	f8df e130 	ldr.w	lr, [pc, #304]	; 3000a18c <rtl_crypto_hmac_sha2_init+0x2dc>
3000a05c:	4a40      	ldr	r2, [pc, #256]	; (3000a160 <rtl_crypto_hmac_sha2_init+0x2b0>)
3000a05e:	4941      	ldr	r1, [pc, #260]	; (3000a164 <rtl_crypto_hmac_sha2_init+0x2b4>)
3000a060:	4d41      	ldr	r5, [pc, #260]	; (3000a168 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000a062:	f8df c12c 	ldr.w	ip, [pc, #300]	; 3000a190 <rtl_crypto_hmac_sha2_init+0x2e0>
3000a066:	e9c4 3e02 	strd	r3, lr, [r4, #8]
3000a06a:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
3000a06e:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
3000a072:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
3000a076:	6163      	str	r3, [r4, #20]
3000a078:	4b3c      	ldr	r3, [pc, #240]	; (3000a16c <rtl_crypto_hmac_sha2_init+0x2bc>)
3000a07a:	6123      	str	r3, [r4, #16]
3000a07c:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
3000a080:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
3000a084:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
3000a088:	61e3      	str	r3, [r4, #28]
3000a08a:	4b39      	ldr	r3, [pc, #228]	; (3000a170 <rtl_crypto_hmac_sha2_init+0x2c0>)
3000a08c:	61a3      	str	r3, [r4, #24]
3000a08e:	4b39      	ldr	r3, [pc, #228]	; (3000a174 <rtl_crypto_hmac_sha2_init+0x2c4>)
3000a090:	e9c4 3208 	strd	r3, r2, [r4, #32]
3000a094:	4b38      	ldr	r3, [pc, #224]	; (3000a178 <rtl_crypto_hmac_sha2_init+0x2c8>)
3000a096:	2245      	movs	r2, #69	; 0x45
3000a098:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
3000a09c:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
3000a0a0:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
3000a0a4:	33aa      	adds	r3, #170	; 0xaa
3000a0a6:	6363      	str	r3, [r4, #52]	; 0x34
3000a0a8:	4b34      	ldr	r3, [pc, #208]	; (3000a17c <rtl_crypto_hmac_sha2_init+0x2cc>)
3000a0aa:	6323      	str	r3, [r4, #48]	; 0x30
3000a0ac:	4b34      	ldr	r3, [pc, #208]	; (3000a180 <rtl_crypto_hmac_sha2_init+0x2d0>)
3000a0ae:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
3000a0b2:	4b34      	ldr	r3, [pc, #208]	; (3000a184 <rtl_crypto_hmac_sha2_init+0x2d4>)
3000a0b4:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
3000a0b8:	e752      	b.n	30009f60 <rtl_crypto_hmac_sha2_init+0xb0>
3000a0ba:	b314      	cbz	r4, 3000a102 <rtl_crypto_hmac_sha2_init+0x252>
3000a0bc:	4b32      	ldr	r3, [pc, #200]	; (3000a188 <rtl_crypto_hmac_sha2_init+0x2d8>)
3000a0be:	2215      	movs	r2, #21
3000a0c0:	4932      	ldr	r1, [pc, #200]	; (3000a18c <rtl_crypto_hmac_sha2_init+0x2dc>)
3000a0c2:	4827      	ldr	r0, [pc, #156]	; (3000a160 <rtl_crypto_hmac_sha2_init+0x2b0>)
3000a0c4:	4d28      	ldr	r5, [pc, #160]	; (3000a168 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000a0c6:	e9c4 1302 	strd	r1, r3, [r4, #8]
3000a0ca:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
3000a0ce:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
3000a0d2:	f503 6301 	add.w	r3, r3, #2064	; 0x810
3000a0d6:	6123      	str	r3, [r4, #16]
3000a0d8:	4b22      	ldr	r3, [pc, #136]	; (3000a164 <rtl_crypto_hmac_sha2_init+0x2b4>)
3000a0da:	e9c4 0305 	strd	r0, r3, [r4, #20]
3000a0de:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
3000a0e2:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
3000a0e6:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
3000a0ea:	61e3      	str	r3, [r4, #28]
3000a0ec:	4b28      	ldr	r3, [pc, #160]	; (3000a190 <rtl_crypto_hmac_sha2_init+0x2e0>)
3000a0ee:	e9c4 5308 	strd	r5, r3, [r4, #32]
3000a0f2:	e735      	b.n	30009f60 <rtl_crypto_hmac_sha2_init+0xb0>
3000a0f4:	21b0      	movs	r1, #176	; 0xb0
3000a0f6:	480a      	ldr	r0, [pc, #40]	; (3000a120 <rtl_crypto_hmac_sha2_init+0x270>)
3000a0f8:	f002 f886 	bl	3000c208 <__io_assert_failed_veneer>
3000a0fc:	e701      	b.n	30009f02 <rtl_crypto_hmac_sha2_init+0x52>
3000a0fe:	2245      	movs	r2, #69	; 0x45
3000a100:	e793      	b.n	3000a02a <rtl_crypto_hmac_sha2_init+0x17a>
3000a102:	2215      	movs	r2, #21
3000a104:	e791      	b.n	3000a02a <rtl_crypto_hmac_sha2_init+0x17a>
3000a106:	f06f 0505 	mvn.w	r5, #5
3000a10a:	e789      	b.n	3000a020 <rtl_crypto_hmac_sha2_init+0x170>
3000a10c:	f06f 0503 	mvn.w	r5, #3
3000a110:	e786      	b.n	3000a020 <rtl_crypto_hmac_sha2_init+0x170>
3000a112:	bf00      	nop
3000a114:	0000e5f9 	.word	0x0000e5f9
3000a118:	500c8000 	.word	0x500c8000
3000a11c:	400c8000 	.word	0x400c8000
3000a120:	3000d670 	.word	0x3000d670
3000a124:	bb67ae85 	.word	0xbb67ae85
3000a128:	510e527f 	.word	0x510e527f
3000a12c:	a54ff53a 	.word	0xa54ff53a
3000a130:	9b05688c 	.word	0x9b05688c
3000a134:	1f83d9ab 	.word	0x1f83d9ab
3000a138:	30009d09 	.word	0x30009d09
3000a13c:	2001c460 	.word	0x2001c460
3000a140:	00004c99 	.word	0x00004c99
3000a144:	6a09e667 	.word	0x6a09e667
3000a148:	f3bcc908 	.word	0xf3bcc908
3000a14c:	fe94f82b 	.word	0xfe94f82b
3000a150:	fb41bd6b 	.word	0xfb41bd6b
3000a154:	3c6ef372 	.word	0x3c6ef372
3000a158:	5be0cd19 	.word	0x5be0cd19
3000a15c:	cbbb9d5d 	.word	0xcbbb9d5d
3000a160:	f70e5939 	.word	0xf70e5939
3000a164:	ffc00b31 	.word	0xffc00b31
3000a168:	64f98fa7 	.word	0x64f98fa7
3000a16c:	629a292a 	.word	0x629a292a
3000a170:	9159015a 	.word	0x9159015a
3000a174:	152fecd8 	.word	0x152fecd8
3000a178:	67332667 	.word	0x67332667
3000a17c:	8eb44a87 	.word	0x8eb44a87
3000a180:	db0c2e0d 	.word	0xdb0c2e0d
3000a184:	47b5481d 	.word	0x47b5481d
3000a188:	367cd507 	.word	0x367cd507
3000a18c:	c1059ed8 	.word	0xc1059ed8
3000a190:	befa4fa4 	.word	0xbefa4fa4
3000a194:	137e2179 	.word	0x137e2179

3000a198 <CPU_ClkGet>:
3000a198:	b570      	push	{r4, r5, r6, lr}
3000a19a:	4d1e      	ldr	r5, [pc, #120]	; (3000a214 <CPU_ClkGet+0x7c>)
3000a19c:	4b1e      	ldr	r3, [pc, #120]	; (3000a218 <CPU_ClkGet+0x80>)
3000a19e:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
3000a1a2:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
3000a1a6:	f3c4 1481 	ubfx	r4, r4, #6, #2
3000a1aa:	4798      	blx	r3
3000a1ac:	b92c      	cbnz	r4, 3000a1ba <CPU_ClkGet+0x22>
3000a1ae:	2801      	cmp	r0, #1
3000a1b0:	d007      	beq.n	3000a1c2 <CPU_ClkGet+0x2a>
3000a1b2:	07b2      	lsls	r2, r6, #30
3000a1b4:	d407      	bmi.n	3000a1c6 <CPU_ClkGet+0x2e>
3000a1b6:	4819      	ldr	r0, [pc, #100]	; (3000a21c <CPU_ClkGet+0x84>)
3000a1b8:	bd70      	pop	{r4, r5, r6, pc}
3000a1ba:	2c01      	cmp	r4, #1
3000a1bc:	d007      	beq.n	3000a1ce <CPU_ClkGet+0x36>
3000a1be:	2000      	movs	r0, #0
3000a1c0:	bd70      	pop	{r4, r5, r6, pc}
3000a1c2:	4817      	ldr	r0, [pc, #92]	; (3000a220 <CPU_ClkGet+0x88>)
3000a1c4:	bd70      	pop	{r4, r5, r6, pc}
3000a1c6:	4b17      	ldr	r3, [pc, #92]	; (3000a224 <CPU_ClkGet+0x8c>)
3000a1c8:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000a1cc:	4718      	bx	r3
3000a1ce:	2801      	cmp	r0, #1
3000a1d0:	d011      	beq.n	3000a1f6 <CPU_ClkGet+0x5e>
3000a1d2:	05f3      	lsls	r3, r6, #23
3000a1d4:	d51c      	bpl.n	3000a210 <CPU_ClkGet+0x78>
3000a1d6:	4914      	ldr	r1, [pc, #80]	; (3000a228 <CPU_ClkGet+0x90>)
3000a1d8:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
3000a1dc:	6c48      	ldr	r0, [r1, #68]	; 0x44
3000a1de:	4a13      	ldr	r2, [pc, #76]	; (3000a22c <CPU_ClkGet+0x94>)
3000a1e0:	f003 0307 	and.w	r3, r3, #7
3000a1e4:	f3c0 1045 	ubfx	r0, r0, #5, #6
3000a1e8:	3301      	adds	r3, #1
3000a1ea:	3002      	adds	r0, #2
3000a1ec:	fb02 f000 	mul.w	r0, r2, r0
3000a1f0:	fbb0 f0f3 	udiv	r0, r0, r3
3000a1f4:	bd70      	pop	{r4, r5, r6, pc}
3000a1f6:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
3000a1fa:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
3000a1fe:	f3c3 4007 	ubfx	r0, r3, #16, #8
3000a202:	d003      	beq.n	3000a20c <CPU_ClkGet+0x74>
3000a204:	4b0a      	ldr	r3, [pc, #40]	; (3000a230 <CPU_ClkGet+0x98>)
3000a206:	fb03 f000 	mul.w	r0, r3, r0
3000a20a:	bd70      	pop	{r4, r5, r6, pc}
3000a20c:	4809      	ldr	r0, [pc, #36]	; (3000a234 <CPU_ClkGet+0x9c>)
3000a20e:	bd70      	pop	{r4, r5, r6, pc}
3000a210:	4806      	ldr	r0, [pc, #24]	; (3000a22c <CPU_ClkGet+0x94>)
3000a212:	bd70      	pop	{r4, r5, r6, pc}
3000a214:	42008000 	.word	0x42008000
3000a218:	0000c0f9 	.word	0x0000c0f9
3000a21c:	003d0900 	.word	0x003d0900
3000a220:	01312d00 	.word	0x01312d00
3000a224:	000099f5 	.word	0x000099f5
3000a228:	42008800 	.word	0x42008800
3000a22c:	02625a00 	.word	0x02625a00
3000a230:	000f4240 	.word	0x000f4240
3000a234:	02faf080 	.word	0x02faf080

3000a238 <RSIP_IV_Set>:
3000a238:	4b05      	ldr	r3, [pc, #20]	; (3000a250 <RSIP_IV_Set+0x18>)
3000a23a:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
3000a23e:	680b      	ldr	r3, [r1, #0]
3000a240:	ba1b      	rev	r3, r3
3000a242:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000a246:	684b      	ldr	r3, [r1, #4]
3000a248:	ba1b      	rev	r3, r3
3000a24a:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
3000a24e:	4770      	bx	lr
3000a250:	42008c00 	.word	0x42008c00

3000a254 <RSIP_OTF_Cmd>:
3000a254:	4a04      	ldr	r2, [pc, #16]	; (3000a268 <RSIP_OTF_Cmd+0x14>)
3000a256:	2801      	cmp	r0, #1
3000a258:	6953      	ldr	r3, [r2, #20]
3000a25a:	bf0c      	ite	eq
3000a25c:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
3000a260:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
3000a264:	6153      	str	r3, [r2, #20]
3000a266:	4770      	bx	lr
3000a268:	42008c00 	.word	0x42008c00

3000a26c <RSIP_OTF_Enable>:
3000a26c:	b570      	push	{r4, r5, r6, lr}
3000a26e:	4604      	mov	r4, r0
3000a270:	b1f3      	cbz	r3, 3000a2b0 <RSIP_OTF_Enable+0x44>
3000a272:	9b04      	ldr	r3, [sp, #16]
3000a274:	460d      	mov	r5, r1
3000a276:	4616      	mov	r6, r2
3000a278:	2b07      	cmp	r3, #7
3000a27a:	d827      	bhi.n	3000a2cc <RSIP_OTF_Enable+0x60>
3000a27c:	9b05      	ldr	r3, [sp, #20]
3000a27e:	3b01      	subs	r3, #1
3000a280:	2b01      	cmp	r3, #1
3000a282:	d81d      	bhi.n	3000a2c0 <RSIP_OTF_Enable+0x54>
3000a284:	9a04      	ldr	r2, [sp, #16]
3000a286:	9b05      	ldr	r3, [sp, #20]
3000a288:	f002 0007 	and.w	r0, r2, #7
3000a28c:	005b      	lsls	r3, r3, #1
3000a28e:	4305      	orrs	r5, r0
3000a290:	4813      	ldr	r0, [pc, #76]	; (3000a2e0 <RSIP_OTF_Enable+0x74>)
3000a292:	f003 0306 	and.w	r3, r3, #6
3000a296:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000a29a:	ea43 0206 	orr.w	r2, r3, r6
3000a29e:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000a2a0:	f042 0201 	orr.w	r2, r2, #1
3000a2a4:	f023 0301 	bic.w	r3, r3, #1
3000a2a8:	6463      	str	r3, [r4, #68]	; 0x44
3000a2aa:	6425      	str	r5, [r4, #64]	; 0x40
3000a2ac:	6462      	str	r2, [r4, #68]	; 0x44
3000a2ae:	bd70      	pop	{r4, r5, r6, pc}
3000a2b0:	480b      	ldr	r0, [pc, #44]	; (3000a2e0 <RSIP_OTF_Enable+0x74>)
3000a2b2:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000a2b6:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000a2b8:	f023 0301 	bic.w	r3, r3, #1
3000a2bc:	6463      	str	r3, [r4, #68]	; 0x44
3000a2be:	bd70      	pop	{r4, r5, r6, pc}
3000a2c0:	f44f 71f3 	mov.w	r1, #486	; 0x1e6
3000a2c4:	4807      	ldr	r0, [pc, #28]	; (3000a2e4 <RSIP_OTF_Enable+0x78>)
3000a2c6:	f001 ff9f 	bl	3000c208 <__io_assert_failed_veneer>
3000a2ca:	e7db      	b.n	3000a284 <RSIP_OTF_Enable+0x18>
3000a2cc:	f240 11e5 	movw	r1, #485	; 0x1e5
3000a2d0:	4804      	ldr	r0, [pc, #16]	; (3000a2e4 <RSIP_OTF_Enable+0x78>)
3000a2d2:	f001 ff99 	bl	3000c208 <__io_assert_failed_veneer>
3000a2d6:	9b05      	ldr	r3, [sp, #20]
3000a2d8:	3b01      	subs	r3, #1
3000a2da:	2b01      	cmp	r3, #1
3000a2dc:	d9d2      	bls.n	3000a284 <RSIP_OTF_Enable+0x18>
3000a2de:	e7ef      	b.n	3000a2c0 <RSIP_OTF_Enable+0x54>
3000a2e0:	42008c00 	.word	0x42008c00
3000a2e4:	3000d660 	.word	0x3000d660

3000a2e8 <RSIP_MMU_Config>:
3000a2e8:	eb00 0040 	add.w	r0, r0, r0, lsl #1
3000a2ec:	311f      	adds	r1, #31
3000a2ee:	321f      	adds	r2, #31
3000a2f0:	331f      	adds	r3, #31
3000a2f2:	0080      	lsls	r0, r0, #2
3000a2f4:	0949      	lsrs	r1, r1, #5
3000a2f6:	0952      	lsrs	r2, r2, #5
3000a2f8:	095b      	lsrs	r3, r3, #5
3000a2fa:	0249      	lsls	r1, r1, #9
3000a2fc:	0252      	lsls	r2, r2, #9
3000a2fe:	025b      	lsls	r3, r3, #9
3000a300:	b410      	push	{r4}
3000a302:	4c08      	ldr	r4, [pc, #32]	; (3000a324 <RSIP_MMU_Config+0x3c>)
3000a304:	4404      	add	r4, r0
3000a306:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
3000a30a:	f020 0001 	bic.w	r0, r0, #1
3000a30e:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
3000a312:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
3000a316:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
3000a31a:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
3000a31e:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a322:	4770      	bx	lr
3000a324:	42008c00 	.word	0x42008c00

3000a328 <RSIP_MMU_Cmd>:
3000a328:	eb00 0040 	add.w	r0, r0, r0, lsl #1
3000a32c:	4b07      	ldr	r3, [pc, #28]	; (3000a34c <RSIP_MMU_Cmd+0x24>)
3000a32e:	0080      	lsls	r0, r0, #2
3000a330:	4403      	add	r3, r0
3000a332:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
3000a336:	b121      	cbz	r1, 3000a342 <RSIP_MMU_Cmd+0x1a>
3000a338:	f042 0201 	orr.w	r2, r2, #1
3000a33c:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a340:	4770      	bx	lr
3000a342:	f022 0201 	bic.w	r2, r2, #1
3000a346:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a34a:	4770      	bx	lr
3000a34c:	42008c00 	.word	0x42008c00

3000a350 <RSIP_MMU_Cache_Clean>:
3000a350:	4a02      	ldr	r2, [pc, #8]	; (3000a35c <RSIP_MMU_Cache_Clean+0xc>)
3000a352:	6993      	ldr	r3, [r2, #24]
3000a354:	f043 0304 	orr.w	r3, r3, #4
3000a358:	6193      	str	r3, [r2, #24]
3000a35a:	4770      	bx	lr
3000a35c:	42008c00 	.word	0x42008c00

3000a360 <NAND_RxData>:
3000a360:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
3000a364:	460c      	mov	r4, r1
3000a366:	4616      	mov	r6, r2
3000a368:	461f      	mov	r7, r3
3000a36a:	2800      	cmp	r0, #0
3000a36c:	f000 808a 	beq.w	3000a484 <NAND_RxData+0x124>
3000a370:	4605      	mov	r5, r0
3000a372:	f8df 911c 	ldr.w	r9, [pc, #284]	; 3000a490 <NAND_RxData+0x130>
3000a376:	2000      	movs	r0, #0
3000a378:	f8df 8118 	ldr.w	r8, [pc, #280]	; 3000a494 <NAND_RxData+0x134>
3000a37c:	47c0      	blx	r8
3000a37e:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
3000a382:	2000      	movs	r0, #0
3000a384:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
3000a388:	6819      	ldr	r1, [r3, #0]
3000a38a:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
3000a38e:	6019      	str	r1, [r3, #0]
3000a390:	6819      	ldr	r1, [r3, #0]
3000a392:	f441 7140 	orr.w	r1, r1, #768	; 0x300
3000a396:	6019      	str	r1, [r3, #0]
3000a398:	605a      	str	r2, [r3, #4]
3000a39a:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
3000a39e:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
3000a3a2:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
3000a3a6:	ea4f 420c 	mov.w	r2, ip, lsl #16
3000a3aa:	f1bc 0f03 	cmp.w	ip, #3
3000a3ae:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
3000a3b2:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a3b6:	ea42 0201 	orr.w	r2, r2, r1
3000a3ba:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
3000a3be:	f04f 0201 	mov.w	r2, #1
3000a3c2:	611a      	str	r2, [r3, #16]
3000a3c4:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
3000a3c8:	bf08      	it	eq
3000a3ca:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
3000a3ce:	f3c4 2307 	ubfx	r3, r4, #8, #8
3000a3d2:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000a3d6:	b2e4      	uxtb	r4, r4
3000a3d8:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000a3dc:	2301      	movs	r3, #1
3000a3de:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
3000a3e2:	f017 0403 	ands.w	r4, r7, #3
3000a3e6:	6083      	str	r3, [r0, #8]
3000a3e8:	d137      	bne.n	3000a45a <NAND_RxData+0xfa>
3000a3ea:	463b      	mov	r3, r7
3000a3ec:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
3000a3f0:	42b4      	cmp	r4, r6
3000a3f2:	d216      	bcs.n	3000a422 <NAND_RxData+0xc2>
3000a3f4:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
3000a3f8:	f012 0f3c 	tst.w	r2, #60	; 0x3c
3000a3fc:	f3c2 0583 	ubfx	r5, r2, #2, #4
3000a400:	d00c      	beq.n	3000a41c <NAND_RxData+0xbc>
3000a402:	00ad      	lsls	r5, r5, #2
3000a404:	1f19      	subs	r1, r3, #4
3000a406:	f1a5 0c04 	sub.w	ip, r5, #4
3000a40a:	449c      	add	ip, r3
3000a40c:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
3000a410:	f841 0f04 	str.w	r0, [r1, #4]!
3000a414:	458c      	cmp	ip, r1
3000a416:	d1f9      	bne.n	3000a40c <NAND_RxData+0xac>
3000a418:	442b      	add	r3, r5
3000a41a:	442c      	add	r4, r5
3000a41c:	1b32      	subs	r2, r6, r4
3000a41e:	2a03      	cmp	r2, #3
3000a420:	d8e6      	bhi.n	3000a3f0 <NAND_RxData+0x90>
3000a422:	1e63      	subs	r3, r4, #1
3000a424:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a428:	441f      	add	r7, r3
3000a42a:	e002      	b.n	3000a432 <NAND_RxData+0xd2>
3000a42c:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000a42e:	071b      	lsls	r3, r3, #28
3000a430:	d40d      	bmi.n	3000a44e <NAND_RxData+0xee>
3000a432:	42b4      	cmp	r4, r6
3000a434:	d3fa      	bcc.n	3000a42c <NAND_RxData+0xcc>
3000a436:	2004      	movs	r0, #4
3000a438:	47c0      	blx	r8
3000a43a:	2000      	movs	r0, #0
3000a43c:	47c0      	blx	r8
3000a43e:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a442:	6813      	ldr	r3, [r2, #0]
3000a444:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000a448:	6013      	str	r3, [r2, #0]
3000a44a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
3000a44e:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
3000a452:	3401      	adds	r4, #1
3000a454:	f807 3f01 	strb.w	r3, [r7, #1]!
3000a458:	e7eb      	b.n	3000a432 <NAND_RxData+0xd2>
3000a45a:	f1c4 0404 	rsb	r4, r4, #4
3000a45e:	2200      	movs	r2, #0
3000a460:	4639      	mov	r1, r7
3000a462:	e00a      	b.n	3000a47a <NAND_RxData+0x11a>
3000a464:	6a85      	ldr	r5, [r0, #40]	; 0x28
3000a466:	072d      	lsls	r5, r5, #28
3000a468:	d505      	bpl.n	3000a476 <NAND_RxData+0x116>
3000a46a:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
3000a46e:	3201      	adds	r2, #1
3000a470:	700b      	strb	r3, [r1, #0]
3000a472:	18b9      	adds	r1, r7, r2
3000a474:	460b      	mov	r3, r1
3000a476:	4294      	cmp	r4, r2
3000a478:	d0b8      	beq.n	3000a3ec <NAND_RxData+0x8c>
3000a47a:	42b2      	cmp	r2, r6
3000a47c:	460b      	mov	r3, r1
3000a47e:	d3f1      	bcc.n	3000a464 <NAND_RxData+0x104>
3000a480:	4614      	mov	r4, r2
3000a482:	e7ce      	b.n	3000a422 <NAND_RxData+0xc2>
3000a484:	f8df 9008 	ldr.w	r9, [pc, #8]	; 3000a490 <NAND_RxData+0x130>
3000a488:	f899 5008 	ldrb.w	r5, [r9, #8]
3000a48c:	e773      	b.n	3000a376 <NAND_RxData+0x16>
3000a48e:	bf00      	nop
3000a490:	2001c01c 	.word	0x2001c01c
3000a494:	0000b9c1 	.word	0x0000b9c1

3000a498 <NAND_Page_Read>:
3000a498:	b530      	push	{r4, r5, lr}
3000a49a:	b085      	sub	sp, #20
3000a49c:	4c07      	ldr	r4, [pc, #28]	; (3000a4bc <NAND_Page_Read+0x24>)
3000a49e:	4d08      	ldr	r5, [pc, #32]	; (3000a4c0 <NAND_Page_Read+0x28>)
3000a4a0:	9301      	str	r3, [sp, #4]
3000a4a2:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000a4a6:	47a0      	blx	r4
3000a4a8:	4604      	mov	r4, r0
3000a4aa:	9903      	ldr	r1, [sp, #12]
3000a4ac:	7a28      	ldrb	r0, [r5, #8]
3000a4ae:	4d05      	ldr	r5, [pc, #20]	; (3000a4c4 <NAND_Page_Read+0x2c>)
3000a4b0:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000a4b4:	47a8      	blx	r5
3000a4b6:	4620      	mov	r0, r4
3000a4b8:	b005      	add	sp, #20
3000a4ba:	bd30      	pop	{r4, r5, pc}
3000a4bc:	0000bd85 	.word	0x0000bd85
3000a4c0:	2001c01c 	.word	0x2001c01c
3000a4c4:	3000a361 	.word	0x3000a361

3000a4c8 <irq_register>:
3000a4c8:	b538      	push	{r3, r4, r5, lr}
3000a4ca:	f040 0001 	orr.w	r0, r0, #1
3000a4ce:	461c      	mov	r4, r3
3000a4d0:	4b14      	ldr	r3, [pc, #80]	; (3000a524 <irq_register+0x5c>)
3000a4d2:	460d      	mov	r5, r1
3000a4d4:	2c07      	cmp	r4, #7
3000a4d6:	f843 0021 	str.w	r0, [r3, r1, lsl #2]
3000a4da:	4b13      	ldr	r3, [pc, #76]	; (3000a528 <irq_register+0x60>)
3000a4dc:	f843 2021 	str.w	r2, [r3, r1, lsl #2]
3000a4e0:	d908      	bls.n	3000a4f4 <irq_register+0x2c>
3000a4e2:	4b12      	ldr	r3, [pc, #72]	; (3000a52c <irq_register+0x64>)
3000a4e4:	681b      	ldr	r3, [r3, #0]
3000a4e6:	039b      	lsls	r3, r3, #14
3000a4e8:	d416      	bmi.n	3000a518 <irq_register+0x50>
3000a4ea:	f44f 7144 	mov.w	r1, #784	; 0x310
3000a4ee:	4810      	ldr	r0, [pc, #64]	; (3000a530 <irq_register+0x68>)
3000a4f0:	f001 fe8a 	bl	3000c208 <__io_assert_failed_veneer>
3000a4f4:	2c07      	cmp	r4, #7
3000a4f6:	bf28      	it	cs
3000a4f8:	2407      	movcs	r4, #7
3000a4fa:	2d00      	cmp	r5, #0
3000a4fc:	ea4f 1444 	mov.w	r4, r4, lsl #5
3000a500:	b2e4      	uxtb	r4, r4
3000a502:	db03      	blt.n	3000a50c <irq_register+0x44>
3000a504:	4b0b      	ldr	r3, [pc, #44]	; (3000a534 <irq_register+0x6c>)
3000a506:	2001      	movs	r0, #1
3000a508:	555c      	strb	r4, [r3, r5]
3000a50a:	bd38      	pop	{r3, r4, r5, pc}
3000a50c:	f005 050f 	and.w	r5, r5, #15
3000a510:	4b09      	ldr	r3, [pc, #36]	; (3000a538 <irq_register+0x70>)
3000a512:	2001      	movs	r0, #1
3000a514:	555c      	strb	r4, [r3, r5]
3000a516:	bd38      	pop	{r3, r4, r5, pc}
3000a518:	2307      	movs	r3, #7
3000a51a:	4622      	mov	r2, r4
3000a51c:	4807      	ldr	r0, [pc, #28]	; (3000a53c <irq_register+0x74>)
3000a51e:	f001 fe83 	bl	3000c228 <__DiagPrintf_veneer>
3000a522:	e7e2      	b.n	3000a4ea <irq_register+0x22>
3000a524:	30000184 	.word	0x30000184
3000a528:	300002c4 	.word	0x300002c4
3000a52c:	2001c00c 	.word	0x2001c00c
3000a530:	3000d64c 	.word	0x3000d64c
3000a534:	e000e400 	.word	0xe000e400
3000a538:	e000ed14 	.word	0xe000ed14
3000a53c:	3000d628 	.word	0x3000d628

3000a540 <SYSCFG_BootFromNor>:
3000a540:	4b0e      	ldr	r3, [pc, #56]	; (3000a57c <SYSCFG_BootFromNor+0x3c>)
3000a542:	b510      	push	{r4, lr}
3000a544:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
3000a548:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
3000a54c:	0392      	lsls	r2, r2, #14
3000a54e:	d50a      	bpl.n	3000a566 <SYSCFG_BootFromNor+0x26>
3000a550:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
3000a554:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
3000a558:	d105      	bne.n	3000a566 <SYSCFG_BootFromNor+0x26>
3000a55a:	4b09      	ldr	r3, [pc, #36]	; (3000a580 <SYSCFG_BootFromNor+0x40>)
3000a55c:	4798      	blx	r3
3000a55e:	3800      	subs	r0, #0
3000a560:	bf18      	it	ne
3000a562:	2001      	movne	r0, #1
3000a564:	bd10      	pop	{r4, pc}
3000a566:	4c07      	ldr	r4, [pc, #28]	; (3000a584 <SYSCFG_BootFromNor+0x44>)
3000a568:	47a0      	blx	r4
3000a56a:	2802      	cmp	r0, #2
3000a56c:	d004      	beq.n	3000a578 <SYSCFG_BootFromNor+0x38>
3000a56e:	47a0      	blx	r4
3000a570:	2801      	cmp	r0, #1
3000a572:	d1f2      	bne.n	3000a55a <SYSCFG_BootFromNor+0x1a>
3000a574:	2000      	movs	r0, #0
3000a576:	bd10      	pop	{r4, pc}
3000a578:	2001      	movs	r0, #1
3000a57a:	bd10      	pop	{r4, pc}
3000a57c:	42008000 	.word	0x42008000
3000a580:	0000c0bd 	.word	0x0000c0bd
3000a584:	0000c1a9 	.word	0x0000c1a9

3000a588 <SYSCFG_RLVersion>:
3000a588:	4b07      	ldr	r3, [pc, #28]	; (3000a5a8 <SYSCFG_RLVersion+0x20>)
3000a58a:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000a58e:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000a592:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
3000a596:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
3000a59a:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000a59e:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000a5a2:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000a5a6:	4770      	bx	lr
3000a5a8:	42008000 	.word	0x42008000

3000a5ac <ddr_init_index>:
3000a5ac:	b570      	push	{r4, r5, r6, lr}
3000a5ae:	4b0f      	ldr	r3, [pc, #60]	; (3000a5ec <ddr_init_index+0x40>)
3000a5b0:	4798      	blx	r3
3000a5b2:	4604      	mov	r4, r0
3000a5b4:	4b0e      	ldr	r3, [pc, #56]	; (3000a5f0 <ddr_init_index+0x44>)
3000a5b6:	4798      	blx	r3
3000a5b8:	2c03      	cmp	r4, #3
3000a5ba:	d015      	beq.n	3000a5e8 <ddr_init_index+0x3c>
3000a5bc:	2c01      	cmp	r4, #1
3000a5be:	d00d      	beq.n	3000a5dc <ddr_init_index+0x30>
3000a5c0:	4d0c      	ldr	r5, [pc, #48]	; (3000a5f4 <ddr_init_index+0x48>)
3000a5c2:	4c0d      	ldr	r4, [pc, #52]	; (3000a5f8 <ddr_init_index+0x4c>)
3000a5c4:	4e0d      	ldr	r6, [pc, #52]	; (3000a5fc <ddr_init_index+0x50>)
3000a5c6:	e002      	b.n	3000a5ce <ddr_init_index+0x22>
3000a5c8:	f242 7010 	movw	r0, #10000	; 0x2710
3000a5cc:	47a0      	blx	r4
3000a5ce:	682b      	ldr	r3, [r5, #0]
3000a5d0:	039b      	lsls	r3, r3, #14
3000a5d2:	d5f9      	bpl.n	3000a5c8 <ddr_init_index+0x1c>
3000a5d4:	4630      	mov	r0, r6
3000a5d6:	f001 fe27 	bl	3000c228 <__DiagPrintf_veneer>
3000a5da:	e7f5      	b.n	3000a5c8 <ddr_init_index+0x1c>
3000a5dc:	2802      	cmp	r0, #2
3000a5de:	d003      	beq.n	3000a5e8 <ddr_init_index+0x3c>
3000a5e0:	2803      	cmp	r0, #3
3000a5e2:	d1ed      	bne.n	3000a5c0 <ddr_init_index+0x14>
3000a5e4:	2000      	movs	r0, #0
3000a5e6:	bd70      	pop	{r4, r5, r6, pc}
3000a5e8:	4620      	mov	r0, r4
3000a5ea:	bd70      	pop	{r4, r5, r6, pc}
3000a5ec:	300080a5 	.word	0x300080a5
3000a5f0:	30007fc5 	.word	0x30007fc5
3000a5f4:	2001c00c 	.word	0x2001c00c
3000a5f8:	00009be5 	.word	0x00009be5
3000a5fc:	3000d6a8 	.word	0x3000d6a8

3000a600 <rxi316_perf_tune>:
3000a600:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a604:	f8d0 e000 	ldr.w	lr, [r0]
3000a608:	b0bf      	sub	sp, #252	; 0xfc
3000a60a:	4606      	mov	r6, r0
3000a60c:	f89e 5006 	ldrb.w	r5, [lr, #6]
3000a610:	2d00      	cmp	r5, #0
3000a612:	f000 833e 	beq.w	3000ac92 <rxi316_perf_tune+0x692>
3000a616:	2d01      	cmp	r5, #1
3000a618:	d003      	beq.n	3000a622 <rxi316_perf_tune+0x22>
3000a61a:	2d03      	cmp	r5, #3
3000a61c:	bf14      	ite	ne
3000a61e:	2502      	movne	r5, #2
3000a620:	2500      	moveq	r5, #0
3000a622:	68f3      	ldr	r3, [r6, #12]
3000a624:	2420      	movs	r4, #32
3000a626:	2100      	movs	r1, #0
3000a628:	685b      	ldr	r3, [r3, #4]
3000a62a:	4627      	mov	r7, r4
3000a62c:	46a0      	mov	r8, r4
3000a62e:	46a1      	mov	r9, r4
3000a630:	785a      	ldrb	r2, [r3, #1]
3000a632:	46a2      	mov	sl, r4
3000a634:	7818      	ldrb	r0, [r3, #0]
3000a636:	46a3      	mov	fp, r4
3000a638:	921f      	str	r2, [sp, #124]	; 0x7c
3000a63a:	468c      	mov	ip, r1
3000a63c:	789a      	ldrb	r2, [r3, #2]
3000a63e:	9220      	str	r2, [sp, #128]	; 0x80
3000a640:	78da      	ldrb	r2, [r3, #3]
3000a642:	9221      	str	r2, [sp, #132]	; 0x84
3000a644:	791a      	ldrb	r2, [r3, #4]
3000a646:	9222      	str	r2, [sp, #136]	; 0x88
3000a648:	795a      	ldrb	r2, [r3, #5]
3000a64a:	9223      	str	r2, [sp, #140]	; 0x8c
3000a64c:	799a      	ldrb	r2, [r3, #6]
3000a64e:	9224      	str	r2, [sp, #144]	; 0x90
3000a650:	79da      	ldrb	r2, [r3, #7]
3000a652:	9225      	str	r2, [sp, #148]	; 0x94
3000a654:	7a1a      	ldrb	r2, [r3, #8]
3000a656:	9226      	str	r2, [sp, #152]	; 0x98
3000a658:	7a5a      	ldrb	r2, [r3, #9]
3000a65a:	9227      	str	r2, [sp, #156]	; 0x9c
3000a65c:	7a9a      	ldrb	r2, [r3, #10]
3000a65e:	9228      	str	r2, [sp, #160]	; 0xa0
3000a660:	7ada      	ldrb	r2, [r3, #11]
3000a662:	9229      	str	r2, [sp, #164]	; 0xa4
3000a664:	7b1a      	ldrb	r2, [r3, #12]
3000a666:	922a      	str	r2, [sp, #168]	; 0xa8
3000a668:	7b5a      	ldrb	r2, [r3, #13]
3000a66a:	922b      	str	r2, [sp, #172]	; 0xac
3000a66c:	7b9a      	ldrb	r2, [r3, #14]
3000a66e:	922c      	str	r2, [sp, #176]	; 0xb0
3000a670:	7bda      	ldrb	r2, [r3, #15]
3000a672:	922d      	str	r2, [sp, #180]	; 0xb4
3000a674:	7c1a      	ldrb	r2, [r3, #16]
3000a676:	941a      	str	r4, [sp, #104]	; 0x68
3000a678:	922e      	str	r2, [sp, #184]	; 0xb8
3000a67a:	7c5a      	ldrb	r2, [r3, #17]
3000a67c:	922f      	str	r2, [sp, #188]	; 0xbc
3000a67e:	7c9a      	ldrb	r2, [r3, #18]
3000a680:	9230      	str	r2, [sp, #192]	; 0xc0
3000a682:	7cda      	ldrb	r2, [r3, #19]
3000a684:	9231      	str	r2, [sp, #196]	; 0xc4
3000a686:	7d1a      	ldrb	r2, [r3, #20]
3000a688:	9232      	str	r2, [sp, #200]	; 0xc8
3000a68a:	7d5a      	ldrb	r2, [r3, #21]
3000a68c:	9233      	str	r2, [sp, #204]	; 0xcc
3000a68e:	7d9a      	ldrb	r2, [r3, #22]
3000a690:	9234      	str	r2, [sp, #208]	; 0xd0
3000a692:	7dda      	ldrb	r2, [r3, #23]
3000a694:	9235      	str	r2, [sp, #212]	; 0xd4
3000a696:	7e1a      	ldrb	r2, [r3, #24]
3000a698:	9236      	str	r2, [sp, #216]	; 0xd8
3000a69a:	7e5a      	ldrb	r2, [r3, #25]
3000a69c:	9237      	str	r2, [sp, #220]	; 0xdc
3000a69e:	7e9a      	ldrb	r2, [r3, #26]
3000a6a0:	9238      	str	r2, [sp, #224]	; 0xe0
3000a6a2:	7eda      	ldrb	r2, [r3, #27]
3000a6a4:	9239      	str	r2, [sp, #228]	; 0xe4
3000a6a6:	7f1a      	ldrb	r2, [r3, #28]
3000a6a8:	923a      	str	r2, [sp, #232]	; 0xe8
3000a6aa:	7f5a      	ldrb	r2, [r3, #29]
3000a6ac:	923b      	str	r2, [sp, #236]	; 0xec
3000a6ae:	7f9a      	ldrb	r2, [r3, #30]
3000a6b0:	7fdb      	ldrb	r3, [r3, #31]
3000a6b2:	923c      	str	r2, [sp, #240]	; 0xf0
3000a6b4:	460a      	mov	r2, r1
3000a6b6:	933d      	str	r3, [sp, #244]	; 0xf4
3000a6b8:	4623      	mov	r3, r4
3000a6ba:	9406      	str	r4, [sp, #24]
3000a6bc:	9419      	str	r4, [sp, #100]	; 0x64
3000a6be:	9418      	str	r4, [sp, #96]	; 0x60
3000a6c0:	9407      	str	r4, [sp, #28]
3000a6c2:	951b      	str	r5, [sp, #108]	; 0x6c
3000a6c4:	e9cd 4402 	strd	r4, r4, [sp, #8]
3000a6c8:	e9cd 4400 	strd	r4, r4, [sp]
3000a6cc:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000a6d0:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
3000a6d4:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
3000a6d8:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
3000a6dc:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
3000a6e0:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000a6e4:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000a6e8:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
3000a6ec:	e9cd 4408 	strd	r4, r4, [sp, #32]
3000a6f0:	b950      	cbnz	r0, 3000a708 <rxi316_perf_tune+0x108>
3000a6f2:	f10c 0c01 	add.w	ip, ip, #1
3000a6f6:	4613      	mov	r3, r2
3000a6f8:	3201      	adds	r2, #1
3000a6fa:	2a20      	cmp	r2, #32
3000a6fc:	d06f      	beq.n	3000a7de <rxi316_perf_tune+0x1de>
3000a6fe:	a81e      	add	r0, sp, #120	; 0x78
3000a700:	f850 0022 	ldr.w	r0, [r0, r2, lsl #2]
3000a704:	2800      	cmp	r0, #0
3000a706:	d0f4      	beq.n	3000a6f2 <rxi316_perf_tune+0xf2>
3000a708:	2801      	cmp	r0, #1
3000a70a:	d062      	beq.n	3000a7d2 <rxi316_perf_tune+0x1d2>
3000a70c:	2802      	cmp	r0, #2
3000a70e:	f000 8083 	beq.w	3000a818 <rxi316_perf_tune+0x218>
3000a712:	2803      	cmp	r0, #3
3000a714:	f000 8084 	beq.w	3000a820 <rxi316_perf_tune+0x220>
3000a718:	2804      	cmp	r0, #4
3000a71a:	f000 8085 	beq.w	3000a828 <rxi316_perf_tune+0x228>
3000a71e:	2805      	cmp	r0, #5
3000a720:	f000 8086 	beq.w	3000a830 <rxi316_perf_tune+0x230>
3000a724:	2806      	cmp	r0, #6
3000a726:	f000 8087 	beq.w	3000a838 <rxi316_perf_tune+0x238>
3000a72a:	2807      	cmp	r0, #7
3000a72c:	f000 82b3 	beq.w	3000ac96 <rxi316_perf_tune+0x696>
3000a730:	2808      	cmp	r0, #8
3000a732:	f000 8375 	beq.w	3000ae20 <rxi316_perf_tune+0x820>
3000a736:	2809      	cmp	r0, #9
3000a738:	f000 8376 	beq.w	3000ae28 <rxi316_perf_tune+0x828>
3000a73c:	280a      	cmp	r0, #10
3000a73e:	f000 837c 	beq.w	3000ae3a <rxi316_perf_tune+0x83a>
3000a742:	280b      	cmp	r0, #11
3000a744:	f000 8376 	beq.w	3000ae34 <rxi316_perf_tune+0x834>
3000a748:	280c      	cmp	r0, #12
3000a74a:	f000 8383 	beq.w	3000ae54 <rxi316_perf_tune+0x854>
3000a74e:	280d      	cmp	r0, #13
3000a750:	f000 8386 	beq.w	3000ae60 <rxi316_perf_tune+0x860>
3000a754:	280e      	cmp	r0, #14
3000a756:	f000 8380 	beq.w	3000ae5a <rxi316_perf_tune+0x85a>
3000a75a:	280f      	cmp	r0, #15
3000a75c:	f000 8383 	beq.w	3000ae66 <rxi316_perf_tune+0x866>
3000a760:	2810      	cmp	r0, #16
3000a762:	f000 8383 	beq.w	3000ae6c <rxi316_perf_tune+0x86c>
3000a766:	2811      	cmp	r0, #17
3000a768:	f000 8382 	beq.w	3000ae70 <rxi316_perf_tune+0x870>
3000a76c:	2812      	cmp	r0, #18
3000a76e:	f000 8381 	beq.w	3000ae74 <rxi316_perf_tune+0x874>
3000a772:	2813      	cmp	r0, #19
3000a774:	f000 8382 	beq.w	3000ae7c <rxi316_perf_tune+0x87c>
3000a778:	2814      	cmp	r0, #20
3000a77a:	f000 837d 	beq.w	3000ae78 <rxi316_perf_tune+0x878>
3000a77e:	2815      	cmp	r0, #21
3000a780:	f000 8384 	beq.w	3000ae8c <rxi316_perf_tune+0x88c>
3000a784:	2816      	cmp	r0, #22
3000a786:	f000 837f 	beq.w	3000ae88 <rxi316_perf_tune+0x888>
3000a78a:	2817      	cmp	r0, #23
3000a78c:	f000 838e 	beq.w	3000aeac <rxi316_perf_tune+0x8ac>
3000a790:	2818      	cmp	r0, #24
3000a792:	f000 8389 	beq.w	3000aea8 <rxi316_perf_tune+0x8a8>
3000a796:	2819      	cmp	r0, #25
3000a798:	f000 8384 	beq.w	3000aea4 <rxi316_perf_tune+0x8a4>
3000a79c:	281a      	cmp	r0, #26
3000a79e:	f000 837f 	beq.w	3000aea0 <rxi316_perf_tune+0x8a0>
3000a7a2:	281b      	cmp	r0, #27
3000a7a4:	f000 837a 	beq.w	3000ae9c <rxi316_perf_tune+0x89c>
3000a7a8:	281c      	cmp	r0, #28
3000a7aa:	f000 8375 	beq.w	3000ae98 <rxi316_perf_tune+0x898>
3000a7ae:	281d      	cmp	r0, #29
3000a7b0:	f000 8370 	beq.w	3000ae94 <rxi316_perf_tune+0x894>
3000a7b4:	281e      	cmp	r0, #30
3000a7b6:	f000 836b 	beq.w	3000ae90 <rxi316_perf_tune+0x890>
3000a7ba:	281f      	cmp	r0, #31
3000a7bc:	f000 8362 	beq.w	3000ae84 <rxi316_perf_tune+0x884>
3000a7c0:	2820      	cmp	r0, #32
3000a7c2:	f000 835d 	beq.w	3000ae80 <rxi316_perf_tune+0x880>
3000a7c6:	9d06      	ldr	r5, [sp, #24]
3000a7c8:	2821      	cmp	r0, #33	; 0x21
3000a7ca:	bf08      	it	eq
3000a7cc:	4615      	moveq	r5, r2
3000a7ce:	9506      	str	r5, [sp, #24]
3000a7d0:	e792      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000a7d2:	4693      	mov	fp, r2
3000a7d4:	3201      	adds	r2, #1
3000a7d6:	f10c 0c01 	add.w	ip, ip, #1
3000a7da:	2a20      	cmp	r2, #32
3000a7dc:	d18f      	bne.n	3000a6fe <rxi316_perf_tune+0xfe>
3000a7de:	f89e 2002 	ldrb.w	r2, [lr, #2]
3000a7e2:	9d1b      	ldr	r5, [sp, #108]	; 0x6c
3000a7e4:	f8de 0008 	ldr.w	r0, [lr, #8]
3000a7e8:	b372      	cbz	r2, 3000a848 <rxi316_perf_tune+0x248>
3000a7ea:	2a01      	cmp	r2, #1
3000a7ec:	f000 8320 	beq.w	3000ae30 <rxi316_perf_tune+0x830>
3000a7f0:	2a03      	cmp	r2, #3
3000a7f2:	f1a0 0002 	sub.w	r0, r0, #2
3000a7f6:	bf0c      	ite	eq
3000a7f8:	2204      	moveq	r2, #4
3000a7fa:	2203      	movne	r2, #3
3000a7fc:	4560      	cmp	r0, ip
3000a7fe:	d127      	bne.n	3000a850 <rxi316_perf_tune+0x250>
3000a800:	4291      	cmp	r1, r2
3000a802:	d02a      	beq.n	3000a85a <rxi316_perf_tune+0x25a>
3000a804:	4a0e      	ldr	r2, [pc, #56]	; (3000a840 <rxi316_perf_tune+0x240>)
3000a806:	6812      	ldr	r2, [r2, #0]
3000a808:	0392      	lsls	r2, r2, #14
3000a80a:	d526      	bpl.n	3000a85a <rxi316_perf_tune+0x25a>
3000a80c:	480d      	ldr	r0, [pc, #52]	; (3000a844 <rxi316_perf_tune+0x244>)
3000a80e:	931b      	str	r3, [sp, #108]	; 0x6c
3000a810:	f001 fd0a 	bl	3000c228 <__DiagPrintf_veneer>
3000a814:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a816:	e020      	b.n	3000a85a <rxi316_perf_tune+0x25a>
3000a818:	f10c 0c01 	add.w	ip, ip, #1
3000a81c:	4692      	mov	sl, r2
3000a81e:	e76b      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000a820:	f10c 0c01 	add.w	ip, ip, #1
3000a824:	4691      	mov	r9, r2
3000a826:	e767      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000a828:	f10c 0c01 	add.w	ip, ip, #1
3000a82c:	4690      	mov	r8, r2
3000a82e:	e763      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000a830:	f10c 0c01 	add.w	ip, ip, #1
3000a834:	4617      	mov	r7, r2
3000a836:	e75f      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000a838:	f10c 0c01 	add.w	ip, ip, #1
3000a83c:	9200      	str	r2, [sp, #0]
3000a83e:	e75b      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000a840:	2001c00c 	.word	0x2001c00c
3000a844:	3000d708 	.word	0x3000d708
3000a848:	2201      	movs	r2, #1
3000a84a:	3802      	subs	r0, #2
3000a84c:	4560      	cmp	r0, ip
3000a84e:	d0d7      	beq.n	3000a800 <rxi316_perf_tune+0x200>
3000a850:	48c4      	ldr	r0, [pc, #784]	; (3000ab64 <rxi316_perf_tune+0x564>)
3000a852:	6800      	ldr	r0, [r0, #0]
3000a854:	0380      	lsls	r0, r0, #14
3000a856:	f100 82f3 	bmi.w	3000ae40 <rxi316_perf_tune+0x840>
3000a85a:	9a07      	ldr	r2, [sp, #28]
3000a85c:	2a20      	cmp	r2, #32
3000a85e:	f000 82dc 	beq.w	3000ae1a <rxi316_perf_tune+0x81a>
3000a862:	1b52      	subs	r2, r2, r5
3000a864:	3a05      	subs	r2, #5
3000a866:	f002 020f 	and.w	r2, r2, #15
3000a86a:	9207      	str	r2, [sp, #28]
3000a86c:	9a08      	ldr	r2, [sp, #32]
3000a86e:	2a20      	cmp	r2, #32
3000a870:	f000 82d1 	beq.w	3000ae16 <rxi316_perf_tune+0x816>
3000a874:	1b52      	subs	r2, r2, r5
3000a876:	3a06      	subs	r2, #6
3000a878:	0112      	lsls	r2, r2, #4
3000a87a:	b2d2      	uxtb	r2, r2
3000a87c:	9909      	ldr	r1, [sp, #36]	; 0x24
3000a87e:	2920      	cmp	r1, #32
3000a880:	f000 82c5 	beq.w	3000ae0e <rxi316_perf_tune+0x80e>
3000a884:	1b49      	subs	r1, r1, r5
3000a886:	3907      	subs	r1, #7
3000a888:	0209      	lsls	r1, r1, #8
3000a88a:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a88e:	9108      	str	r1, [sp, #32]
3000a890:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a892:	2920      	cmp	r1, #32
3000a894:	f000 821a 	beq.w	3000accc <rxi316_perf_tune+0x6cc>
3000a898:	1b49      	subs	r1, r1, r5
3000a89a:	3908      	subs	r1, #8
3000a89c:	0309      	lsls	r1, r1, #12
3000a89e:	b289      	uxth	r1, r1
3000a8a0:	9109      	str	r1, [sp, #36]	; 0x24
3000a8a2:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a8a4:	2920      	cmp	r1, #32
3000a8a6:	f000 8218 	beq.w	3000acda <rxi316_perf_tune+0x6da>
3000a8aa:	1b49      	subs	r1, r1, r5
3000a8ac:	3909      	subs	r1, #9
3000a8ae:	0409      	lsls	r1, r1, #16
3000a8b0:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a8b4:	910a      	str	r1, [sp, #40]	; 0x28
3000a8b6:	990c      	ldr	r1, [sp, #48]	; 0x30
3000a8b8:	2920      	cmp	r1, #32
3000a8ba:	f000 8215 	beq.w	3000ace8 <rxi316_perf_tune+0x6e8>
3000a8be:	1b49      	subs	r1, r1, r5
3000a8c0:	390a      	subs	r1, #10
3000a8c2:	0509      	lsls	r1, r1, #20
3000a8c4:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a8c8:	910b      	str	r1, [sp, #44]	; 0x2c
3000a8ca:	990d      	ldr	r1, [sp, #52]	; 0x34
3000a8cc:	2920      	cmp	r1, #32
3000a8ce:	f000 8212 	beq.w	3000acf6 <rxi316_perf_tune+0x6f6>
3000a8d2:	1b49      	subs	r1, r1, r5
3000a8d4:	390b      	subs	r1, #11
3000a8d6:	0609      	lsls	r1, r1, #24
3000a8d8:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a8dc:	910c      	str	r1, [sp, #48]	; 0x30
3000a8de:	990e      	ldr	r1, [sp, #56]	; 0x38
3000a8e0:	2920      	cmp	r1, #32
3000a8e2:	f000 820f 	beq.w	3000ad04 <rxi316_perf_tune+0x704>
3000a8e6:	1b49      	subs	r1, r1, r5
3000a8e8:	390c      	subs	r1, #12
3000a8ea:	0709      	lsls	r1, r1, #28
3000a8ec:	910d      	str	r1, [sp, #52]	; 0x34
3000a8ee:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000a8f0:	2920      	cmp	r1, #32
3000a8f2:	f000 820e 	beq.w	3000ad12 <rxi316_perf_tune+0x712>
3000a8f6:	1b49      	subs	r1, r1, r5
3000a8f8:	390d      	subs	r1, #13
3000a8fa:	f001 010f 	and.w	r1, r1, #15
3000a8fe:	910e      	str	r1, [sp, #56]	; 0x38
3000a900:	9910      	ldr	r1, [sp, #64]	; 0x40
3000a902:	2920      	cmp	r1, #32
3000a904:	f000 820b 	beq.w	3000ad1e <rxi316_perf_tune+0x71e>
3000a908:	1b49      	subs	r1, r1, r5
3000a90a:	390e      	subs	r1, #14
3000a90c:	0109      	lsls	r1, r1, #4
3000a90e:	b2c9      	uxtb	r1, r1
3000a910:	910f      	str	r1, [sp, #60]	; 0x3c
3000a912:	9911      	ldr	r1, [sp, #68]	; 0x44
3000a914:	2920      	cmp	r1, #32
3000a916:	f000 8208 	beq.w	3000ad2a <rxi316_perf_tune+0x72a>
3000a91a:	1b49      	subs	r1, r1, r5
3000a91c:	390f      	subs	r1, #15
3000a91e:	0209      	lsls	r1, r1, #8
3000a920:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a924:	9110      	str	r1, [sp, #64]	; 0x40
3000a926:	9912      	ldr	r1, [sp, #72]	; 0x48
3000a928:	2920      	cmp	r1, #32
3000a92a:	f000 8205 	beq.w	3000ad38 <rxi316_perf_tune+0x738>
3000a92e:	1b49      	subs	r1, r1, r5
3000a930:	0309      	lsls	r1, r1, #12
3000a932:	f401 4170 	and.w	r1, r1, #61440	; 0xf000
3000a936:	9111      	str	r1, [sp, #68]	; 0x44
3000a938:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000a93a:	2920      	cmp	r1, #32
3000a93c:	f000 8203 	beq.w	3000ad46 <rxi316_perf_tune+0x746>
3000a940:	1b49      	subs	r1, r1, r5
3000a942:	3911      	subs	r1, #17
3000a944:	0409      	lsls	r1, r1, #16
3000a946:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a94a:	9112      	str	r1, [sp, #72]	; 0x48
3000a94c:	9914      	ldr	r1, [sp, #80]	; 0x50
3000a94e:	2920      	cmp	r1, #32
3000a950:	f000 8200 	beq.w	3000ad54 <rxi316_perf_tune+0x754>
3000a954:	1b49      	subs	r1, r1, r5
3000a956:	3912      	subs	r1, #18
3000a958:	0509      	lsls	r1, r1, #20
3000a95a:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a95e:	9113      	str	r1, [sp, #76]	; 0x4c
3000a960:	9915      	ldr	r1, [sp, #84]	; 0x54
3000a962:	2920      	cmp	r1, #32
3000a964:	f000 81fd 	beq.w	3000ad62 <rxi316_perf_tune+0x762>
3000a968:	1b49      	subs	r1, r1, r5
3000a96a:	3913      	subs	r1, #19
3000a96c:	0609      	lsls	r1, r1, #24
3000a96e:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a972:	911d      	str	r1, [sp, #116]	; 0x74
3000a974:	9916      	ldr	r1, [sp, #88]	; 0x58
3000a976:	2920      	cmp	r1, #32
3000a978:	f000 81fa 	beq.w	3000ad70 <rxi316_perf_tune+0x770>
3000a97c:	1b49      	subs	r1, r1, r5
3000a97e:	3914      	subs	r1, #20
3000a980:	0709      	lsls	r1, r1, #28
3000a982:	911c      	str	r1, [sp, #112]	; 0x70
3000a984:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000a986:	2920      	cmp	r1, #32
3000a988:	f000 81f9 	beq.w	3000ad7e <rxi316_perf_tune+0x77e>
3000a98c:	1b49      	subs	r1, r1, r5
3000a98e:	3915      	subs	r1, #21
3000a990:	f001 010f 	and.w	r1, r1, #15
3000a994:	911b      	str	r1, [sp, #108]	; 0x6c
3000a996:	9904      	ldr	r1, [sp, #16]
3000a998:	2920      	cmp	r1, #32
3000a99a:	f000 81f6 	beq.w	3000ad8a <rxi316_perf_tune+0x78a>
3000a99e:	1b49      	subs	r1, r1, r5
3000a9a0:	3902      	subs	r1, #2
3000a9a2:	f001 011f 	and.w	r1, r1, #31
3000a9a6:	9117      	str	r1, [sp, #92]	; 0x5c
3000a9a8:	9905      	ldr	r1, [sp, #20]
3000a9aa:	2920      	cmp	r1, #32
3000a9ac:	f000 81f3 	beq.w	3000ad96 <rxi316_perf_tune+0x796>
3000a9b0:	1b49      	subs	r1, r1, r5
3000a9b2:	3902      	subs	r1, #2
3000a9b4:	0149      	lsls	r1, r1, #5
3000a9b6:	f401 7178 	and.w	r1, r1, #992	; 0x3e0
3000a9ba:	9116      	str	r1, [sp, #88]	; 0x58
3000a9bc:	9918      	ldr	r1, [sp, #96]	; 0x60
3000a9be:	2920      	cmp	r1, #32
3000a9c0:	f000 81f0 	beq.w	3000ada4 <rxi316_perf_tune+0x7a4>
3000a9c4:	1b49      	subs	r1, r1, r5
3000a9c6:	2b20      	cmp	r3, #32
3000a9c8:	f1a1 0102 	sub.w	r1, r1, #2
3000a9cc:	ea4f 2181 	mov.w	r1, r1, lsl #10
3000a9d0:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000a9d4:	9115      	str	r1, [sp, #84]	; 0x54
3000a9d6:	f000 81eb 	beq.w	3000adb0 <rxi316_perf_tune+0x7b0>
3000a9da:	1b59      	subs	r1, r3, r5
3000a9dc:	f1bb 0f20 	cmp.w	fp, #32
3000a9e0:	f1a1 0102 	sub.w	r1, r1, #2
3000a9e4:	f001 030f 	and.w	r3, r1, #15
3000a9e8:	9314      	str	r3, [sp, #80]	; 0x50
3000a9ea:	f000 81e7 	beq.w	3000adbc <rxi316_perf_tune+0x7bc>
3000a9ee:	ebab 0105 	sub.w	r1, fp, r5
3000a9f2:	f1ba 0f20 	cmp.w	sl, #32
3000a9f6:	f1a1 0103 	sub.w	r1, r1, #3
3000a9fa:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a9fe:	b2c9      	uxtb	r1, r1
3000aa00:	f000 81e2 	beq.w	3000adc8 <rxi316_perf_tune+0x7c8>
3000aa04:	ebaa 0305 	sub.w	r3, sl, r5
3000aa08:	f1b9 0f20 	cmp.w	r9, #32
3000aa0c:	f1a3 0304 	sub.w	r3, r3, #4
3000aa10:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000aa14:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000aa18:	9305      	str	r3, [sp, #20]
3000aa1a:	f000 81dc 	beq.w	3000add6 <rxi316_perf_tune+0x7d6>
3000aa1e:	eba9 0305 	sub.w	r3, r9, r5
3000aa22:	f1b8 0f20 	cmp.w	r8, #32
3000aa26:	f1a3 0305 	sub.w	r3, r3, #5
3000aa2a:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000aa2e:	b29b      	uxth	r3, r3
3000aa30:	9304      	str	r3, [sp, #16]
3000aa32:	f000 81d7 	beq.w	3000ade4 <rxi316_perf_tune+0x7e4>
3000aa36:	eba8 0c05 	sub.w	ip, r8, r5
3000aa3a:	2f20      	cmp	r7, #32
3000aa3c:	f1ac 0c06 	sub.w	ip, ip, #6
3000aa40:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000aa44:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000aa48:	f000 81d1 	beq.w	3000adee <rxi316_perf_tune+0x7ee>
3000aa4c:	eba7 0e05 	sub.w	lr, r7, r5
3000aa50:	9b00      	ldr	r3, [sp, #0]
3000aa52:	f1ae 0e07 	sub.w	lr, lr, #7
3000aa56:	2b20      	cmp	r3, #32
3000aa58:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000aa5c:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000aa60:	f000 81cb 	beq.w	3000adfa <rxi316_perf_tune+0x7fa>
3000aa64:	1b5f      	subs	r7, r3, r5
3000aa66:	9b01      	ldr	r3, [sp, #4]
3000aa68:	3f08      	subs	r7, #8
3000aa6a:	2b20      	cmp	r3, #32
3000aa6c:	ea4f 6707 	mov.w	r7, r7, lsl #24
3000aa70:	f007 6770 	and.w	r7, r7, #251658240	; 0xf000000
3000aa74:	f000 81c7 	beq.w	3000ae06 <rxi316_perf_tune+0x806>
3000aa78:	1b5b      	subs	r3, r3, r5
3000aa7a:	3b09      	subs	r3, #9
3000aa7c:	071b      	lsls	r3, r3, #28
3000aa7e:	9301      	str	r3, [sp, #4]
3000aa80:	9b02      	ldr	r3, [sp, #8]
3000aa82:	2b20      	cmp	r3, #32
3000aa84:	f000 811f 	beq.w	3000acc6 <rxi316_perf_tune+0x6c6>
3000aa88:	eba3 0805 	sub.w	r8, r3, r5
3000aa8c:	f1a8 080a 	sub.w	r8, r8, #10
3000aa90:	f008 080f 	and.w	r8, r8, #15
3000aa94:	9b03      	ldr	r3, [sp, #12]
3000aa96:	2b20      	cmp	r3, #32
3000aa98:	f000 8113 	beq.w	3000acc2 <rxi316_perf_tune+0x6c2>
3000aa9c:	1b58      	subs	r0, r3, r5
3000aa9e:	380b      	subs	r0, #11
3000aaa0:	0100      	lsls	r0, r0, #4
3000aaa2:	b2c0      	uxtb	r0, r0
3000aaa4:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000aaa6:	2b20      	cmp	r3, #32
3000aaa8:	f000 8108 	beq.w	3000acbc <rxi316_perf_tune+0x6bc>
3000aaac:	eba3 0a05 	sub.w	sl, r3, r5
3000aab0:	f1aa 0a02 	sub.w	sl, sl, #2
3000aab4:	f00a 0a1f 	and.w	sl, sl, #31
3000aab8:	9b06      	ldr	r3, [sp, #24]
3000aaba:	2b20      	cmp	r3, #32
3000aabc:	f000 80ef 	beq.w	3000ac9e <rxi316_perf_tune+0x69e>
3000aac0:	eba3 0905 	sub.w	r9, r3, r5
3000aac4:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000aac6:	f1a9 0902 	sub.w	r9, r9, #2
3000aaca:	2b20      	cmp	r3, #32
3000aacc:	f009 091f 	and.w	r9, r9, #31
3000aad0:	f000 80eb 	beq.w	3000acaa <rxi316_perf_tune+0x6aa>
3000aad4:	eba3 0b05 	sub.w	fp, r3, r5
3000aad8:	2c20      	cmp	r4, #32
3000aada:	f1ab 0b02 	sub.w	fp, fp, #2
3000aade:	f00b 0b1f 	and.w	fp, fp, #31
3000aae2:	f000 80e7 	beq.w	3000acb4 <rxi316_perf_tune+0x6b4>
3000aae6:	1b63      	subs	r3, r4, r5
3000aae8:	3b02      	subs	r3, #2
3000aaea:	015b      	lsls	r3, r3, #5
3000aaec:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000aaf0:	9300      	str	r3, [sp, #0]
3000aaf2:	9c07      	ldr	r4, [sp, #28]
3000aaf4:	ea40 0008 	orr.w	r0, r0, r8
3000aaf8:	4314      	orrs	r4, r2
3000aafa:	4622      	mov	r2, r4
3000aafc:	e9dd 450e 	ldrd	r4, r5, [sp, #56]	; 0x38
3000ab00:	432c      	orrs	r4, r5
3000ab02:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000ab04:	4329      	orrs	r1, r5
3000ab06:	e9dd 5316 	ldrd	r5, r3, [sp, #88]	; 0x58
3000ab0a:	431d      	orrs	r5, r3
3000ab0c:	9b08      	ldr	r3, [sp, #32]
3000ab0e:	431a      	orrs	r2, r3
3000ab10:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000ab12:	431c      	orrs	r4, r3
3000ab14:	9b05      	ldr	r3, [sp, #20]
3000ab16:	4319      	orrs	r1, r3
3000ab18:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000ab1a:	431d      	orrs	r5, r3
3000ab1c:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000ab1e:	431a      	orrs	r2, r3
3000ab20:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000ab22:	431c      	orrs	r4, r3
3000ab24:	9b04      	ldr	r3, [sp, #16]
3000ab26:	4319      	orrs	r1, r3
3000ab28:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000ab2a:	ea41 0c0c 	orr.w	ip, r1, ip
3000ab2e:	431a      	orrs	r2, r3
3000ab30:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000ab32:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000ab34:	ea4c 0c0e 	orr.w	ip, ip, lr
3000ab38:	430a      	orrs	r2, r1
3000ab3a:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000ab3c:	431c      	orrs	r4, r3
3000ab3e:	ea4c 0c07 	orr.w	ip, ip, r7
3000ab42:	4f09      	ldr	r7, [pc, #36]	; (3000ab68 <rxi316_perf_tune+0x568>)
3000ab44:	430c      	orrs	r4, r1
3000ab46:	990c      	ldr	r1, [sp, #48]	; 0x30
3000ab48:	9b00      	ldr	r3, [sp, #0]
3000ab4a:	430a      	orrs	r2, r1
3000ab4c:	991d      	ldr	r1, [sp, #116]	; 0x74
3000ab4e:	ea43 030b 	orr.w	r3, r3, fp
3000ab52:	430c      	orrs	r4, r1
3000ab54:	990d      	ldr	r1, [sp, #52]	; 0x34
3000ab56:	430a      	orrs	r2, r1
3000ab58:	991c      	ldr	r1, [sp, #112]	; 0x70
3000ab5a:	4321      	orrs	r1, r4
3000ab5c:	9c01      	ldr	r4, [sp, #4]
3000ab5e:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000ab62:	e003      	b.n	3000ab6c <rxi316_perf_tune+0x56c>
3000ab64:	2001c00c 	.word	0x2001c00c
3000ab68:	41100000 	.word	0x41100000
3000ab6c:	f8c7 1404 	str.w	r1, [r7, #1028]	; 0x404
3000ab70:	ea4c 0404 	orr.w	r4, ip, r4
3000ab74:	9a1b      	ldr	r2, [sp, #108]	; 0x6c
3000ab76:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000ab7a:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000ab7e:	f8c7 4410 	str.w	r4, [r7, #1040]	; 0x410
3000ab82:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000ab86:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000ab8a:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000ab8e:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000ab92:	68f3      	ldr	r3, [r6, #12]
3000ab94:	689a      	ldr	r2, [r3, #8]
3000ab96:	e9d2 3200 	ldrd	r3, r2, [r2]
3000ab9a:	f003 0301 	and.w	r3, r3, #1
3000ab9e:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000aba2:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000aba6:	68f3      	ldr	r3, [r6, #12]
3000aba8:	689a      	ldr	r2, [r3, #8]
3000abaa:	7a13      	ldrb	r3, [r2, #8]
3000abac:	6951      	ldr	r1, [r2, #20]
3000abae:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000abb2:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000abb6:	0209      	lsls	r1, r1, #8
3000abb8:	0412      	lsls	r2, r2, #16
3000abba:	b289      	uxth	r1, r1
3000abbc:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000abc0:	430b      	orrs	r3, r1
3000abc2:	4313      	orrs	r3, r2
3000abc4:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000abc8:	68f3      	ldr	r3, [r6, #12]
3000abca:	6899      	ldr	r1, [r3, #8]
3000abcc:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000abd0:	7e09      	ldrb	r1, [r1, #24]
3000abd2:	021b      	lsls	r3, r3, #8
3000abd4:	0412      	lsls	r2, r2, #16
3000abd6:	b29b      	uxth	r3, r3
3000abd8:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000abdc:	4313      	orrs	r3, r2
3000abde:	430b      	orrs	r3, r1
3000abe0:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000abe4:	68f3      	ldr	r3, [r6, #12]
3000abe6:	689a      	ldr	r2, [r3, #8]
3000abe8:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000abec:	041b      	lsls	r3, r3, #16
3000abee:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000abf2:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000abf6:	4313      	orrs	r3, r2
3000abf8:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000abfc:	68f3      	ldr	r3, [r6, #12]
3000abfe:	6899      	ldr	r1, [r3, #8]
3000ac00:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000ac02:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000ac04:	005b      	lsls	r3, r3, #1
3000ac06:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000ac08:	0412      	lsls	r2, r2, #16
3000ac0a:	f003 0302 	and.w	r3, r3, #2
3000ac0e:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000ac12:	4313      	orrs	r3, r2
3000ac14:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000ac16:	f000 0101 	and.w	r1, r0, #1
3000ac1a:	0212      	lsls	r2, r2, #8
3000ac1c:	430b      	orrs	r3, r1
3000ac1e:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000ac22:	4313      	orrs	r3, r2
3000ac24:	4aa2      	ldr	r2, [pc, #648]	; (3000aeb0 <rxi316_perf_tune+0x8b0>)
3000ac26:	431a      	orrs	r2, r3
3000ac28:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000ac2c:	68f3      	ldr	r3, [r6, #12]
3000ac2e:	689b      	ldr	r3, [r3, #8]
3000ac30:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000ac32:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000ac36:	68f3      	ldr	r3, [r6, #12]
3000ac38:	689b      	ldr	r3, [r3, #8]
3000ac3a:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000ac3c:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000ac40:	68f3      	ldr	r3, [r6, #12]
3000ac42:	689b      	ldr	r3, [r3, #8]
3000ac44:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000ac46:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000ac4a:	68f3      	ldr	r3, [r6, #12]
3000ac4c:	689b      	ldr	r3, [r3, #8]
3000ac4e:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000ac50:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000ac54:	68f3      	ldr	r3, [r6, #12]
3000ac56:	689b      	ldr	r3, [r3, #8]
3000ac58:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000ac5a:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000ac5e:	68f3      	ldr	r3, [r6, #12]
3000ac60:	689b      	ldr	r3, [r3, #8]
3000ac62:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000ac64:	f003 030f 	and.w	r3, r3, #15
3000ac68:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000ac6c:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000ac70:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000ac74:	f043 0304 	orr.w	r3, r3, #4
3000ac78:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000ac7c:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000ac80:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000ac84:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000ac88:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000ac8c:	b03f      	add	sp, #252	; 0xfc
3000ac8e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000ac92:	2502      	movs	r5, #2
3000ac94:	e4c5      	b.n	3000a622 <rxi316_perf_tune+0x22>
3000ac96:	f10c 0c01 	add.w	ip, ip, #1
3000ac9a:	9201      	str	r2, [sp, #4]
3000ac9c:	e52c      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ac9e:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000aca0:	f04f 091f 	mov.w	r9, #31
3000aca4:	2b20      	cmp	r3, #32
3000aca6:	f47f af15 	bne.w	3000aad4 <rxi316_perf_tune+0x4d4>
3000acaa:	2c20      	cmp	r4, #32
3000acac:	f04f 0b1f 	mov.w	fp, #31
3000acb0:	f47f af19 	bne.w	3000aae6 <rxi316_perf_tune+0x4e6>
3000acb4:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000acb8:	9300      	str	r3, [sp, #0]
3000acba:	e71a      	b.n	3000aaf2 <rxi316_perf_tune+0x4f2>
3000acbc:	f04f 0a1f 	mov.w	sl, #31
3000acc0:	e6fa      	b.n	3000aab8 <rxi316_perf_tune+0x4b8>
3000acc2:	20f0      	movs	r0, #240	; 0xf0
3000acc4:	e6ee      	b.n	3000aaa4 <rxi316_perf_tune+0x4a4>
3000acc6:	f04f 080f 	mov.w	r8, #15
3000acca:	e6e3      	b.n	3000aa94 <rxi316_perf_tune+0x494>
3000accc:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000acd0:	9109      	str	r1, [sp, #36]	; 0x24
3000acd2:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000acd4:	2920      	cmp	r1, #32
3000acd6:	f47f ade8 	bne.w	3000a8aa <rxi316_perf_tune+0x2aa>
3000acda:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000acde:	910a      	str	r1, [sp, #40]	; 0x28
3000ace0:	990c      	ldr	r1, [sp, #48]	; 0x30
3000ace2:	2920      	cmp	r1, #32
3000ace4:	f47f adeb 	bne.w	3000a8be <rxi316_perf_tune+0x2be>
3000ace8:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000acec:	910b      	str	r1, [sp, #44]	; 0x2c
3000acee:	990d      	ldr	r1, [sp, #52]	; 0x34
3000acf0:	2920      	cmp	r1, #32
3000acf2:	f47f adee 	bne.w	3000a8d2 <rxi316_perf_tune+0x2d2>
3000acf6:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000acfa:	910c      	str	r1, [sp, #48]	; 0x30
3000acfc:	990e      	ldr	r1, [sp, #56]	; 0x38
3000acfe:	2920      	cmp	r1, #32
3000ad00:	f47f adf1 	bne.w	3000a8e6 <rxi316_perf_tune+0x2e6>
3000ad04:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000ad08:	910d      	str	r1, [sp, #52]	; 0x34
3000ad0a:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000ad0c:	2920      	cmp	r1, #32
3000ad0e:	f47f adf2 	bne.w	3000a8f6 <rxi316_perf_tune+0x2f6>
3000ad12:	210f      	movs	r1, #15
3000ad14:	910e      	str	r1, [sp, #56]	; 0x38
3000ad16:	9910      	ldr	r1, [sp, #64]	; 0x40
3000ad18:	2920      	cmp	r1, #32
3000ad1a:	f47f adf5 	bne.w	3000a908 <rxi316_perf_tune+0x308>
3000ad1e:	21f0      	movs	r1, #240	; 0xf0
3000ad20:	910f      	str	r1, [sp, #60]	; 0x3c
3000ad22:	9911      	ldr	r1, [sp, #68]	; 0x44
3000ad24:	2920      	cmp	r1, #32
3000ad26:	f47f adf8 	bne.w	3000a91a <rxi316_perf_tune+0x31a>
3000ad2a:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000ad2e:	9110      	str	r1, [sp, #64]	; 0x40
3000ad30:	9912      	ldr	r1, [sp, #72]	; 0x48
3000ad32:	2920      	cmp	r1, #32
3000ad34:	f47f adfb 	bne.w	3000a92e <rxi316_perf_tune+0x32e>
3000ad38:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000ad3c:	9111      	str	r1, [sp, #68]	; 0x44
3000ad3e:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000ad40:	2920      	cmp	r1, #32
3000ad42:	f47f adfd 	bne.w	3000a940 <rxi316_perf_tune+0x340>
3000ad46:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000ad4a:	9112      	str	r1, [sp, #72]	; 0x48
3000ad4c:	9914      	ldr	r1, [sp, #80]	; 0x50
3000ad4e:	2920      	cmp	r1, #32
3000ad50:	f47f ae00 	bne.w	3000a954 <rxi316_perf_tune+0x354>
3000ad54:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000ad58:	9113      	str	r1, [sp, #76]	; 0x4c
3000ad5a:	9915      	ldr	r1, [sp, #84]	; 0x54
3000ad5c:	2920      	cmp	r1, #32
3000ad5e:	f47f ae03 	bne.w	3000a968 <rxi316_perf_tune+0x368>
3000ad62:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000ad66:	911d      	str	r1, [sp, #116]	; 0x74
3000ad68:	9916      	ldr	r1, [sp, #88]	; 0x58
3000ad6a:	2920      	cmp	r1, #32
3000ad6c:	f47f ae06 	bne.w	3000a97c <rxi316_perf_tune+0x37c>
3000ad70:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000ad74:	911c      	str	r1, [sp, #112]	; 0x70
3000ad76:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000ad78:	2920      	cmp	r1, #32
3000ad7a:	f47f ae07 	bne.w	3000a98c <rxi316_perf_tune+0x38c>
3000ad7e:	210f      	movs	r1, #15
3000ad80:	911b      	str	r1, [sp, #108]	; 0x6c
3000ad82:	9904      	ldr	r1, [sp, #16]
3000ad84:	2920      	cmp	r1, #32
3000ad86:	f47f ae0a 	bne.w	3000a99e <rxi316_perf_tune+0x39e>
3000ad8a:	211f      	movs	r1, #31
3000ad8c:	9117      	str	r1, [sp, #92]	; 0x5c
3000ad8e:	9905      	ldr	r1, [sp, #20]
3000ad90:	2920      	cmp	r1, #32
3000ad92:	f47f ae0d 	bne.w	3000a9b0 <rxi316_perf_tune+0x3b0>
3000ad96:	f44f 7178 	mov.w	r1, #992	; 0x3e0
3000ad9a:	9116      	str	r1, [sp, #88]	; 0x58
3000ad9c:	9918      	ldr	r1, [sp, #96]	; 0x60
3000ad9e:	2920      	cmp	r1, #32
3000ada0:	f47f ae10 	bne.w	3000a9c4 <rxi316_perf_tune+0x3c4>
3000ada4:	f44f 41f8 	mov.w	r1, #31744	; 0x7c00
3000ada8:	2b20      	cmp	r3, #32
3000adaa:	9115      	str	r1, [sp, #84]	; 0x54
3000adac:	f47f ae15 	bne.w	3000a9da <rxi316_perf_tune+0x3da>
3000adb0:	230f      	movs	r3, #15
3000adb2:	f1bb 0f20 	cmp.w	fp, #32
3000adb6:	9314      	str	r3, [sp, #80]	; 0x50
3000adb8:	f47f ae19 	bne.w	3000a9ee <rxi316_perf_tune+0x3ee>
3000adbc:	f1ba 0f20 	cmp.w	sl, #32
3000adc0:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000adc4:	f47f ae1e 	bne.w	3000aa04 <rxi316_perf_tune+0x404>
3000adc8:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000adcc:	f1b9 0f20 	cmp.w	r9, #32
3000add0:	9305      	str	r3, [sp, #20]
3000add2:	f47f ae24 	bne.w	3000aa1e <rxi316_perf_tune+0x41e>
3000add6:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000adda:	f1b8 0f20 	cmp.w	r8, #32
3000adde:	9304      	str	r3, [sp, #16]
3000ade0:	f47f ae29 	bne.w	3000aa36 <rxi316_perf_tune+0x436>
3000ade4:	2f20      	cmp	r7, #32
3000ade6:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000adea:	f47f ae2f 	bne.w	3000aa4c <rxi316_perf_tune+0x44c>
3000adee:	9b00      	ldr	r3, [sp, #0]
3000adf0:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000adf4:	2b20      	cmp	r3, #32
3000adf6:	f47f ae35 	bne.w	3000aa64 <rxi316_perf_tune+0x464>
3000adfa:	9b01      	ldr	r3, [sp, #4]
3000adfc:	f04f 6770 	mov.w	r7, #251658240	; 0xf000000
3000ae00:	2b20      	cmp	r3, #32
3000ae02:	f47f ae39 	bne.w	3000aa78 <rxi316_perf_tune+0x478>
3000ae06:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000ae0a:	9301      	str	r3, [sp, #4]
3000ae0c:	e638      	b.n	3000aa80 <rxi316_perf_tune+0x480>
3000ae0e:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000ae12:	9108      	str	r1, [sp, #32]
3000ae14:	e53c      	b.n	3000a890 <rxi316_perf_tune+0x290>
3000ae16:	22f0      	movs	r2, #240	; 0xf0
3000ae18:	e530      	b.n	3000a87c <rxi316_perf_tune+0x27c>
3000ae1a:	220f      	movs	r2, #15
3000ae1c:	9207      	str	r2, [sp, #28]
3000ae1e:	e525      	b.n	3000a86c <rxi316_perf_tune+0x26c>
3000ae20:	f10c 0c01 	add.w	ip, ip, #1
3000ae24:	9202      	str	r2, [sp, #8]
3000ae26:	e467      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae28:	f10c 0c01 	add.w	ip, ip, #1
3000ae2c:	9203      	str	r2, [sp, #12]
3000ae2e:	e463      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae30:	2202      	movs	r2, #2
3000ae32:	e50a      	b.n	3000a84a <rxi316_perf_tune+0x24a>
3000ae34:	3101      	adds	r1, #1
3000ae36:	9205      	str	r2, [sp, #20]
3000ae38:	e45e      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae3a:	3101      	adds	r1, #1
3000ae3c:	9204      	str	r2, [sp, #16]
3000ae3e:	e45b      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae40:	481c      	ldr	r0, [pc, #112]	; (3000aeb4 <rxi316_perf_tune+0x8b4>)
3000ae42:	911b      	str	r1, [sp, #108]	; 0x6c
3000ae44:	e9cd 231c 	strd	r2, r3, [sp, #112]	; 0x70
3000ae48:	f001 f9ee 	bl	3000c228 <__DiagPrintf_veneer>
3000ae4c:	991b      	ldr	r1, [sp, #108]	; 0x6c
3000ae4e:	e9dd 231c 	ldrd	r2, r3, [sp, #112]	; 0x70
3000ae52:	e4d5      	b.n	3000a800 <rxi316_perf_tune+0x200>
3000ae54:	3101      	adds	r1, #1
3000ae56:	9218      	str	r2, [sp, #96]	; 0x60
3000ae58:	e44e      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae5a:	3101      	adds	r1, #1
3000ae5c:	4614      	mov	r4, r2
3000ae5e:	e44b      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae60:	3101      	adds	r1, #1
3000ae62:	921a      	str	r2, [sp, #104]	; 0x68
3000ae64:	e448      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae66:	3101      	adds	r1, #1
3000ae68:	9219      	str	r2, [sp, #100]	; 0x64
3000ae6a:	e445      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae6c:	9207      	str	r2, [sp, #28]
3000ae6e:	e443      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae70:	9208      	str	r2, [sp, #32]
3000ae72:	e441      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae74:	9209      	str	r2, [sp, #36]	; 0x24
3000ae76:	e43f      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae78:	920b      	str	r2, [sp, #44]	; 0x2c
3000ae7a:	e43d      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae7c:	920a      	str	r2, [sp, #40]	; 0x28
3000ae7e:	e43b      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae80:	9217      	str	r2, [sp, #92]	; 0x5c
3000ae82:	e439      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae84:	9216      	str	r2, [sp, #88]	; 0x58
3000ae86:	e437      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae88:	920d      	str	r2, [sp, #52]	; 0x34
3000ae8a:	e435      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae8c:	920c      	str	r2, [sp, #48]	; 0x30
3000ae8e:	e433      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae90:	9215      	str	r2, [sp, #84]	; 0x54
3000ae92:	e431      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae94:	9214      	str	r2, [sp, #80]	; 0x50
3000ae96:	e42f      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae98:	9213      	str	r2, [sp, #76]	; 0x4c
3000ae9a:	e42d      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000ae9c:	9212      	str	r2, [sp, #72]	; 0x48
3000ae9e:	e42b      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000aea0:	9211      	str	r2, [sp, #68]	; 0x44
3000aea2:	e429      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000aea4:	9210      	str	r2, [sp, #64]	; 0x40
3000aea6:	e427      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000aea8:	920f      	str	r2, [sp, #60]	; 0x3c
3000aeaa:	e425      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000aeac:	920e      	str	r2, [sp, #56]	; 0x38
3000aeae:	e423      	b.n	3000a6f8 <rxi316_perf_tune+0xf8>
3000aeb0:	40000008 	.word	0x40000008
3000aeb4:	3000d6c8 	.word	0x3000d6c8

3000aeb8 <rxi316_dram_init>:
3000aeb8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000aebc:	4604      	mov	r4, r0
3000aebe:	b08f      	sub	sp, #60	; 0x3c
3000aec0:	4b10      	ldr	r3, [pc, #64]	; (3000af04 <rxi316_dram_init+0x4c>)
3000aec2:	4798      	blx	r3
3000aec4:	f894 b014 	ldrb.w	fp, [r4, #20]
3000aec8:	2101      	movs	r1, #1
3000aeca:	900c      	str	r0, [sp, #48]	; 0x30
3000aecc:	fa01 f30b 	lsl.w	r3, r1, fp
3000aed0:	9302      	str	r3, [sp, #8]
3000aed2:	6823      	ldr	r3, [r4, #0]
3000aed4:	78da      	ldrb	r2, [r3, #3]
3000aed6:	6923      	ldr	r3, [r4, #16]
3000aed8:	fa03 f30b 	lsl.w	r3, r3, fp
3000aedc:	b1a2      	cbz	r2, 3000af08 <rxi316_dram_init+0x50>
3000aede:	428a      	cmp	r2, r1
3000aee0:	f000 847c 	beq.w	3000b7dc <rxi316_dram_init+0x924>
3000aee4:	2a02      	cmp	r2, #2
3000aee6:	bf0b      	itete	eq
3000aee8:	2206      	moveq	r2, #6
3000aeea:	220a      	movne	r2, #10
3000aeec:	2540      	moveq	r5, #64	; 0x40
3000aeee:	2580      	movne	r5, #128	; 0x80
3000aef0:	9207      	str	r2, [sp, #28]
3000aef2:	bf0c      	ite	eq
3000aef4:	2204      	moveq	r2, #4
3000aef6:	2208      	movne	r2, #8
3000aef8:	9208      	str	r2, [sp, #32]
3000aefa:	bf0c      	ite	eq
3000aefc:	2208      	moveq	r2, #8
3000aefe:	2210      	movne	r2, #16
3000af00:	9201      	str	r2, [sp, #4]
3000af02:	e007      	b.n	3000af14 <rxi316_dram_init+0x5c>
3000af04:	300080a5 	.word	0x300080a5
3000af08:	2203      	movs	r2, #3
3000af0a:	25c0      	movs	r5, #192	; 0xc0
3000af0c:	9108      	str	r1, [sp, #32]
3000af0e:	9207      	str	r2, [sp, #28]
3000af10:	2202      	movs	r2, #2
3000af12:	9201      	str	r2, [sp, #4]
3000af14:	6860      	ldr	r0, [r4, #4]
3000af16:	f04f 31ff 	mov.w	r1, #4294967295
3000af1a:	4ac2      	ldr	r2, [pc, #776]	; (3000b224 <rxi316_dram_init+0x36c>)
3000af1c:	6806      	ldr	r6, [r0, #0]
3000af1e:	960d      	str	r6, [sp, #52]	; 0x34
3000af20:	6846      	ldr	r6, [r0, #4]
3000af22:	6880      	ldr	r0, [r0, #8]
3000af24:	9609      	str	r6, [sp, #36]	; 0x24
3000af26:	9003      	str	r0, [sp, #12]
3000af28:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000af2c:	6820      	ldr	r0, [r4, #0]
3000af2e:	7981      	ldrb	r1, [r0, #6]
3000af30:	6882      	ldr	r2, [r0, #8]
3000af32:	b129      	cbz	r1, 3000af40 <rxi316_dram_init+0x88>
3000af34:	2901      	cmp	r1, #1
3000af36:	f000 8449 	beq.w	3000b7cc <rxi316_dram_init+0x914>
3000af3a:	2903      	cmp	r1, #3
3000af3c:	f000 8456 	beq.w	3000b7ec <rxi316_dram_init+0x934>
3000af40:	3a06      	subs	r2, #6
3000af42:	7881      	ldrb	r1, [r0, #2]
3000af44:	f002 020f 	and.w	r2, r2, #15
3000af48:	7940      	ldrb	r0, [r0, #5]
3000af4a:	0109      	lsls	r1, r1, #4
3000af4c:	0280      	lsls	r0, r0, #10
3000af4e:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000af52:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000af56:	4301      	orrs	r1, r0
3000af58:	430d      	orrs	r5, r1
3000af5a:	432a      	orrs	r2, r5
3000af5c:	4db1      	ldr	r5, [pc, #708]	; (3000b224 <rxi316_dram_init+0x36c>)
3000af5e:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000af62:	68e2      	ldr	r2, [r4, #12]
3000af64:	6812      	ldr	r2, [r2, #0]
3000af66:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000af6a:	f002 0207 	and.w	r2, r2, #7
3000af6e:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000af72:	68e2      	ldr	r2, [r4, #12]
3000af74:	6811      	ldr	r1, [r2, #0]
3000af76:	6822      	ldr	r2, [r4, #0]
3000af78:	6848      	ldr	r0, [r1, #4]
3000af7a:	f892 9000 	ldrb.w	r9, [r2]
3000af7e:	688a      	ldr	r2, [r1, #8]
3000af80:	9004      	str	r0, [sp, #16]
3000af82:	0300      	lsls	r0, r0, #12
3000af84:	0412      	lsls	r2, r2, #16
3000af86:	f009 070f 	and.w	r7, r9, #15
3000af8a:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000af8e:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000af92:	4302      	orrs	r2, r0
3000af94:	68c8      	ldr	r0, [r1, #12]
3000af96:	0440      	lsls	r0, r0, #17
3000af98:	433a      	orrs	r2, r7
3000af9a:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000af9e:	6908      	ldr	r0, [r1, #16]
3000afa0:	0480      	lsls	r0, r0, #18
3000afa2:	4332      	orrs	r2, r6
3000afa4:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000afa8:	4310      	orrs	r0, r2
3000afaa:	694a      	ldr	r2, [r1, #20]
3000afac:	04d2      	lsls	r2, r2, #19
3000afae:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000afb2:	4310      	orrs	r0, r2
3000afb4:	698a      	ldr	r2, [r1, #24]
3000afb6:	0552      	lsls	r2, r2, #21
3000afb8:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000afbc:	4310      	orrs	r0, r2
3000afbe:	69ca      	ldr	r2, [r1, #28]
3000afc0:	0592      	lsls	r2, r2, #22
3000afc2:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000afc6:	4310      	orrs	r0, r2
3000afc8:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000afca:	0612      	lsls	r2, r2, #24
3000afcc:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000afd0:	4302      	orrs	r2, r0
3000afd2:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000afd6:	06c0      	lsls	r0, r0, #27
3000afd8:	0789      	lsls	r1, r1, #30
3000afda:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000afde:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000afe2:	4310      	orrs	r0, r2
3000afe4:	9a02      	ldr	r2, [sp, #8]
3000afe6:	0212      	lsls	r2, r2, #8
3000afe8:	4308      	orrs	r0, r1
3000afea:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000afee:	4310      	orrs	r0, r2
3000aff0:	6068      	str	r0, [r5, #4]
3000aff2:	6822      	ldr	r2, [r4, #0]
3000aff4:	7992      	ldrb	r2, [r2, #6]
3000aff6:	2a00      	cmp	r2, #0
3000aff8:	f040 8317 	bne.w	3000b62a <rxi316_dram_init+0x772>
3000affc:	686a      	ldr	r2, [r5, #4]
3000affe:	f042 0210 	orr.w	r2, r2, #16
3000b002:	606a      	str	r2, [r5, #4]
3000b004:	68e2      	ldr	r2, [r4, #12]
3000b006:	9802      	ldr	r0, [sp, #8]
3000b008:	6812      	ldr	r2, [r2, #0]
3000b00a:	f100 3aff 	add.w	sl, r0, #4294967295
3000b00e:	6861      	ldr	r1, [r4, #4]
3000b010:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000b012:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000b016:	9005      	str	r0, [sp, #20]
3000b018:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000b01a:	fa2e f80b 	lsr.w	r8, lr, fp
3000b01e:	6949      	ldr	r1, [r1, #20]
3000b020:	ea1e 0f0a 	tst.w	lr, sl
3000b024:	9006      	str	r0, [sp, #24]
3000b026:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000b028:	fa21 fe0b 	lsr.w	lr, r1, fp
3000b02c:	bf18      	it	ne
3000b02e:	f108 0801 	addne.w	r8, r8, #1
3000b032:	ea11 0f0a 	tst.w	r1, sl
3000b036:	9906      	ldr	r1, [sp, #24]
3000b038:	950a      	str	r5, [sp, #40]	; 0x28
3000b03a:	bf18      	it	ne
3000b03c:	f10e 0e01 	addne.w	lr, lr, #1
3000b040:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000b042:	0049      	lsls	r1, r1, #1
3000b044:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000b048:	950b      	str	r5, [sp, #44]	; 0x2c
3000b04a:	f001 0102 	and.w	r1, r1, #2
3000b04e:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000b052:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000b056:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000b05a:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000b05e:	0080      	lsls	r0, r0, #2
3000b060:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000b064:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000b068:	9006      	str	r0, [sp, #24]
3000b06a:	013f      	lsls	r7, r7, #4
3000b06c:	9805      	ldr	r0, [sp, #20]
3000b06e:	f00c 0c08 	and.w	ip, ip, #8
3000b072:	0176      	lsls	r6, r6, #5
3000b074:	f007 0710 	and.w	r7, r7, #16
3000b078:	f000 0001 	and.w	r0, r0, #1
3000b07c:	f006 0620 	and.w	r6, r6, #32
3000b080:	4605      	mov	r5, r0
3000b082:	9806      	ldr	r0, [sp, #24]
3000b084:	f000 0004 	and.w	r0, r0, #4
3000b088:	4301      	orrs	r1, r0
3000b08a:	4329      	orrs	r1, r5
3000b08c:	ea41 0c0c 	orr.w	ip, r1, ip
3000b090:	990a      	ldr	r1, [sp, #40]	; 0x28
3000b092:	ea4c 0707 	orr.w	r7, ip, r7
3000b096:	018d      	lsls	r5, r1, #6
3000b098:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000b09a:	4337      	orrs	r7, r6
3000b09c:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000b0a0:	0208      	lsls	r0, r1, #8
3000b0a2:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000b0a4:	432f      	orrs	r7, r5
3000b0a6:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000b0aa:	06c9      	lsls	r1, r1, #27
3000b0ac:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000b0b0:	4307      	orrs	r7, r0
3000b0b2:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000b0b6:	0736      	lsls	r6, r6, #28
3000b0b8:	430f      	orrs	r7, r1
3000b0ba:	0752      	lsls	r2, r2, #29
3000b0bc:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000b0c0:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000b0c4:	433e      	orrs	r6, r7
3000b0c6:	4316      	orrs	r6, r2
3000b0c8:	4a56      	ldr	r2, [pc, #344]	; (3000b224 <rxi316_dram_init+0x36c>)
3000b0ca:	ea46 0608 	orr.w	r6, r6, r8
3000b0ce:	ea46 060e 	orr.w	r6, r6, lr
3000b0d2:	6096      	str	r6, [r2, #8]
3000b0d4:	6862      	ldr	r2, [r4, #4]
3000b0d6:	6990      	ldr	r0, [r2, #24]
3000b0d8:	9a01      	ldr	r2, [sp, #4]
3000b0da:	2a10      	cmp	r2, #16
3000b0dc:	6822      	ldr	r2, [r4, #0]
3000b0de:	7811      	ldrb	r1, [r2, #0]
3000b0e0:	7916      	ldrb	r6, [r2, #4]
3000b0e2:	f000 833d 	beq.w	3000b760 <rxi316_dram_init+0x8a8>
3000b0e6:	f1a1 0502 	sub.w	r5, r1, #2
3000b0ea:	f1a1 0e09 	sub.w	lr, r1, #9
3000b0ee:	9f01      	ldr	r7, [sp, #4]
3000b0f0:	fab5 f585 	clz	r5, r5
3000b0f4:	fabe fe8e 	clz	lr, lr
3000b0f8:	2f08      	cmp	r7, #8
3000b0fa:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b0fe:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000b102:	ea45 020e 	orr.w	r2, r5, lr
3000b106:	f000 829e 	beq.w	3000b646 <rxi316_dram_init+0x78e>
3000b10a:	9f01      	ldr	r7, [sp, #4]
3000b10c:	2f04      	cmp	r7, #4
3000b10e:	f000 83a5 	beq.w	3000b85c <rxi316_dram_init+0x9a4>
3000b112:	9f01      	ldr	r7, [sp, #4]
3000b114:	2f02      	cmp	r7, #2
3000b116:	bf08      	it	eq
3000b118:	4310      	orreq	r0, r2
3000b11a:	2a00      	cmp	r2, #0
3000b11c:	f040 8298 	bne.w	3000b650 <rxi316_dram_init+0x798>
3000b120:	2903      	cmp	r1, #3
3000b122:	f000 8368 	beq.w	3000b7f6 <rxi316_dram_init+0x93e>
3000b126:	2500      	movs	r5, #0
3000b128:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b12c:	462f      	mov	r7, r5
3000b12e:	433d      	orrs	r5, r7
3000b130:	4f3c      	ldr	r7, [pc, #240]	; (3000b224 <rxi316_dram_init+0x36c>)
3000b132:	430d      	orrs	r5, r1
3000b134:	4305      	orrs	r5, r0
3000b136:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000b13a:	637e      	str	r6, [r7, #52]	; 0x34
3000b13c:	6862      	ldr	r2, [r4, #4]
3000b13e:	9903      	ldr	r1, [sp, #12]
3000b140:	69d2      	ldr	r2, [r2, #28]
3000b142:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000b146:	63ba      	str	r2, [r7, #56]	; 0x38
3000b148:	6822      	ldr	r2, [r4, #0]
3000b14a:	7810      	ldrb	r0, [r2, #0]
3000b14c:	6862      	ldr	r2, [r4, #4]
3000b14e:	2803      	cmp	r0, #3
3000b150:	6a11      	ldr	r1, [r2, #32]
3000b152:	f000 82f8 	beq.w	3000b746 <rxi316_dram_init+0x88e>
3000b156:	2809      	cmp	r0, #9
3000b158:	f000 830d 	beq.w	3000b776 <rxi316_dram_init+0x8be>
3000b15c:	4831      	ldr	r0, [pc, #196]	; (3000b224 <rxi316_dram_init+0x36c>)
3000b15e:	63c1      	str	r1, [r0, #60]	; 0x3c
3000b160:	6862      	ldr	r2, [r4, #4]
3000b162:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000b164:	6402      	str	r2, [r0, #64]	; 0x40
3000b166:	6865      	ldr	r5, [r4, #4]
3000b168:	68e8      	ldr	r0, [r5, #12]
3000b16a:	682a      	ldr	r2, [r5, #0]
3000b16c:	6869      	ldr	r1, [r5, #4]
3000b16e:	4402      	add	r2, r0
3000b170:	68ae      	ldr	r6, [r5, #8]
3000b172:	4401      	add	r1, r0
3000b174:	ea1a 0f02 	tst.w	sl, r2
3000b178:	f000 82b8 	beq.w	3000b6ec <rxi316_dram_init+0x834>
3000b17c:	fa22 f20b 	lsr.w	r2, r2, fp
3000b180:	1c50      	adds	r0, r2, #1
3000b182:	ea1a 0f01 	tst.w	sl, r1
3000b186:	fa21 f50b 	lsr.w	r5, r1, fp
3000b18a:	6822      	ldr	r2, [r4, #0]
3000b18c:	fa26 f10b 	lsr.w	r1, r6, fp
3000b190:	bf18      	it	ne
3000b192:	3501      	addne	r5, #1
3000b194:	ea1a 0f06 	tst.w	sl, r6
3000b198:	7816      	ldrb	r6, [r2, #0]
3000b19a:	bf18      	it	ne
3000b19c:	3101      	addne	r1, #1
3000b19e:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000b1a2:	1eb2      	subs	r2, r6, #2
3000b1a4:	2a01      	cmp	r2, #1
3000b1a6:	f240 82a4 	bls.w	3000b6f2 <rxi316_dram_init+0x83a>
3000b1aa:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000b1ae:	fbb7 f2f3 	udiv	r2, r7, r3
3000b1b2:	fb03 7712 	mls	r7, r3, r2, r7
3000b1b6:	b107      	cbz	r7, 3000b1ba <rxi316_dram_init+0x302>
3000b1b8:	3201      	adds	r2, #1
3000b1ba:	2e09      	cmp	r6, #9
3000b1bc:	f040 82b3 	bne.w	3000b726 <rxi316_dram_init+0x86e>
3000b1c0:	9e08      	ldr	r6, [sp, #32]
3000b1c2:	fa26 f60b 	lsr.w	r6, r6, fp
3000b1c6:	016d      	lsls	r5, r5, #5
3000b1c8:	f000 001f 	and.w	r0, r0, #31
3000b1cc:	0289      	lsls	r1, r1, #10
3000b1ce:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000b1d2:	0512      	lsls	r2, r2, #20
3000b1d4:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000b1d8:	4305      	orrs	r5, r0
3000b1da:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000b1de:	430d      	orrs	r5, r1
3000b1e0:	4910      	ldr	r1, [pc, #64]	; (3000b224 <rxi316_dram_init+0x36c>)
3000b1e2:	4315      	orrs	r5, r2
3000b1e4:	0672      	lsls	r2, r6, #25
3000b1e6:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000b1ea:	432a      	orrs	r2, r5
3000b1ec:	630a      	str	r2, [r1, #48]	; 0x30
3000b1ee:	68a0      	ldr	r0, [r4, #8]
3000b1f0:	6802      	ldr	r2, [r0, #0]
3000b1f2:	fbb2 f1f3 	udiv	r1, r2, r3
3000b1f6:	fb03 2211 	mls	r2, r3, r1, r2
3000b1fa:	2a00      	cmp	r2, #0
3000b1fc:	f000 8274 	beq.w	3000b6e8 <rxi316_dram_init+0x830>
3000b200:	1c4a      	adds	r2, r1, #1
3000b202:	9205      	str	r2, [sp, #20]
3000b204:	6842      	ldr	r2, [r0, #4]
3000b206:	fbb2 f0f3 	udiv	r0, r2, r3
3000b20a:	fb03 2210 	mls	r2, r3, r0, r2
3000b20e:	2a00      	cmp	r2, #0
3000b210:	f000 8268 	beq.w	3000b6e4 <rxi316_dram_init+0x82c>
3000b214:	38c7      	subs	r0, #199	; 0xc7
3000b216:	68e2      	ldr	r2, [r4, #12]
3000b218:	6811      	ldr	r1, [r2, #0]
3000b21a:	4a03      	ldr	r2, [pc, #12]	; (3000b228 <rxi316_dram_init+0x370>)
3000b21c:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000b220:	e004      	b.n	3000b22c <rxi316_dram_init+0x374>
3000b222:	bf00      	nop
3000b224:	41100000 	.word	0x41100000
3000b228:	00ffff00 	.word	0x00ffff00
3000b22c:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000b230:	900a      	str	r0, [sp, #40]	; 0x28
3000b232:	0712      	lsls	r2, r2, #28
3000b234:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000b238:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000b23c:	430a      	orrs	r2, r1
3000b23e:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000b242:	430a      	orrs	r2, r1
3000b244:	49c3      	ldr	r1, [pc, #780]	; (3000b554 <rxi316_dram_init+0x69c>)
3000b246:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000b24a:	610a      	str	r2, [r1, #16]
3000b24c:	68a2      	ldr	r2, [r4, #8]
3000b24e:	6911      	ldr	r1, [r2, #16]
3000b250:	fbb1 fcf3 	udiv	ip, r1, r3
3000b254:	fb03 111c 	mls	r1, r3, ip, r1
3000b258:	2900      	cmp	r1, #0
3000b25a:	f000 8240 	beq.w	3000b6de <rxi316_dram_init+0x826>
3000b25e:	f10c 0101 	add.w	r1, ip, #1
3000b262:	9106      	str	r1, [sp, #24]
3000b264:	6991      	ldr	r1, [r2, #24]
3000b266:	fbb1 f0f3 	udiv	r0, r1, r3
3000b26a:	fb03 1110 	mls	r1, r3, r0, r1
3000b26e:	b101      	cbz	r1, 3000b272 <rxi316_dram_init+0x3ba>
3000b270:	3001      	adds	r0, #1
3000b272:	68d1      	ldr	r1, [r2, #12]
3000b274:	fbb1 f5f3 	udiv	r5, r1, r3
3000b278:	fb03 1115 	mls	r1, r3, r5, r1
3000b27c:	9503      	str	r5, [sp, #12]
3000b27e:	b109      	cbz	r1, 3000b284 <rxi316_dram_init+0x3cc>
3000b280:	3501      	adds	r5, #1
3000b282:	9503      	str	r5, [sp, #12]
3000b284:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000b286:	fbb5 f1f3 	udiv	r1, r5, r3
3000b28a:	fb03 5511 	mls	r5, r3, r1, r5
3000b28e:	b105      	cbz	r5, 3000b292 <rxi316_dram_init+0x3da>
3000b290:	3101      	adds	r1, #1
3000b292:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000b294:	0349      	lsls	r1, r1, #13
3000b296:	68e2      	ldr	r2, [r4, #12]
3000b298:	fa25 f60b 	lsr.w	r6, r5, fp
3000b29c:	ea15 0f0a 	tst.w	r5, sl
3000b2a0:	6812      	ldr	r2, [r2, #0]
3000b2a2:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000b2a6:	bf18      	it	ne
3000b2a8:	3601      	addne	r6, #1
3000b2aa:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000b2ac:	05d2      	lsls	r2, r2, #23
3000b2ae:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000b2b2:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000b2b6:	9a06      	ldr	r2, [sp, #24]
3000b2b8:	0112      	lsls	r2, r2, #4
3000b2ba:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000b2be:	4332      	orrs	r2, r6
3000b2c0:	0246      	lsls	r6, r0, #9
3000b2c2:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000b2c6:	4316      	orrs	r6, r2
3000b2c8:	9a03      	ldr	r2, [sp, #12]
3000b2ca:	f002 020f 	and.w	r2, r2, #15
3000b2ce:	4316      	orrs	r6, r2
3000b2d0:	4aa0      	ldr	r2, [pc, #640]	; (3000b554 <rxi316_dram_init+0x69c>)
3000b2d2:	4331      	orrs	r1, r6
3000b2d4:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000b2d8:	6151      	str	r1, [r2, #20]
3000b2da:	68a7      	ldr	r7, [r4, #8]
3000b2dc:	697a      	ldr	r2, [r7, #20]
3000b2de:	fbb2 fef3 	udiv	lr, r2, r3
3000b2e2:	fb03 221e 	mls	r2, r3, lr, r2
3000b2e6:	b10a      	cbz	r2, 3000b2ec <rxi316_dram_init+0x434>
3000b2e8:	f10e 0e01 	add.w	lr, lr, #1
3000b2ec:	6939      	ldr	r1, [r7, #16]
3000b2ee:	fbb1 f2f3 	udiv	r2, r1, r3
3000b2f2:	fb03 1112 	mls	r1, r3, r2, r1
3000b2f6:	b101      	cbz	r1, 3000b2fa <rxi316_dram_init+0x442>
3000b2f8:	3201      	adds	r2, #1
3000b2fa:	68fd      	ldr	r5, [r7, #12]
3000b2fc:	fbb5 f1f3 	udiv	r1, r5, r3
3000b300:	fb03 5511 	mls	r5, r3, r1, r5
3000b304:	b105      	cbz	r5, 3000b308 <rxi316_dram_init+0x450>
3000b306:	3101      	adds	r1, #1
3000b308:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000b30a:	fbb5 f6f3 	udiv	r6, r5, r3
3000b30e:	fb03 5516 	mls	r5, r3, r6, r5
3000b312:	b105      	cbz	r5, 3000b316 <rxi316_dram_init+0x45e>
3000b314:	3601      	adds	r6, #1
3000b316:	68bd      	ldr	r5, [r7, #8]
3000b318:	4411      	add	r1, r2
3000b31a:	fbb5 f2f3 	udiv	r2, r5, r3
3000b31e:	42b1      	cmp	r1, r6
3000b320:	fb03 5512 	mls	r5, r3, r2, r5
3000b324:	bf38      	it	cc
3000b326:	4631      	movcc	r1, r6
3000b328:	b905      	cbnz	r5, 3000b32c <rxi316_dram_init+0x474>
3000b32a:	3a01      	subs	r2, #1
3000b32c:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000b32e:	ea15 0f0a 	tst.w	r5, sl
3000b332:	fa25 fc0b 	lsr.w	ip, r5, fp
3000b336:	69fd      	ldr	r5, [r7, #28]
3000b338:	bf18      	it	ne
3000b33a:	f10c 0c01 	addne.w	ip, ip, #1
3000b33e:	fa25 f60b 	lsr.w	r6, r5, fp
3000b342:	ea15 0f0a 	tst.w	r5, sl
3000b346:	bf18      	it	ne
3000b348:	3601      	addne	r6, #1
3000b34a:	f1b9 0f09 	cmp.w	r9, #9
3000b34e:	f000 81f5 	beq.w	3000b73c <rxi316_dram_init+0x884>
3000b352:	f1b9 0f02 	cmp.w	r9, #2
3000b356:	f000 8233 	beq.w	3000b7c0 <rxi316_dram_init+0x908>
3000b35a:	f1b9 0f03 	cmp.w	r9, #3
3000b35e:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b360:	f000 8298 	beq.w	3000b894 <rxi316_dram_init+0x9dc>
3000b364:	46a8      	mov	r8, r5
3000b366:	9d01      	ldr	r5, [sp, #4]
3000b368:	44a8      	add	r8, r5
3000b36a:	fa28 f50b 	lsr.w	r5, r8, fp
3000b36e:	ea18 0f0a 	tst.w	r8, sl
3000b372:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000b376:	bf18      	it	ne
3000b378:	3501      	addne	r5, #1
3000b37a:	fbb8 f7f3 	udiv	r7, r8, r3
3000b37e:	fb03 8817 	mls	r8, r3, r7, r8
3000b382:	f1b8 0f00 	cmp.w	r8, #0
3000b386:	d000      	beq.n	3000b38a <rxi316_dram_init+0x4d2>
3000b388:	3701      	adds	r7, #1
3000b38a:	0109      	lsls	r1, r1, #4
3000b38c:	f00e 0e0f 	and.w	lr, lr, #15
3000b390:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000b394:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000b398:	063f      	lsls	r7, r7, #24
3000b39a:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000b39e:	ea41 0e0e 	orr.w	lr, r1, lr
3000b3a2:	0291      	lsls	r1, r2, #10
3000b3a4:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000b3a8:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000b3ac:	ea4e 0e01 	orr.w	lr, lr, r1
3000b3b0:	0471      	lsls	r1, r6, #17
3000b3b2:	ea4e 0c0c 	orr.w	ip, lr, ip
3000b3b6:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000b3ba:	ea4c 0c01 	orr.w	ip, ip, r1
3000b3be:	0529      	lsls	r1, r5, #20
3000b3c0:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000b3c4:	ea4c 0c01 	orr.w	ip, ip, r1
3000b3c8:	4962      	ldr	r1, [pc, #392]	; (3000b554 <rxi316_dram_init+0x69c>)
3000b3ca:	ea4c 0707 	orr.w	r7, ip, r7
3000b3ce:	618f      	str	r7, [r1, #24]
3000b3d0:	6821      	ldr	r1, [r4, #0]
3000b3d2:	780f      	ldrb	r7, [r1, #0]
3000b3d4:	2f02      	cmp	r7, #2
3000b3d6:	f000 8152 	beq.w	3000b67e <rxi316_dram_init+0x7c6>
3000b3da:	2f09      	cmp	r7, #9
3000b3dc:	f000 814f 	beq.w	3000b67e <rxi316_dram_init+0x7c6>
3000b3e0:	68e1      	ldr	r1, [r4, #12]
3000b3e2:	2f03      	cmp	r7, #3
3000b3e4:	6809      	ldr	r1, [r1, #0]
3000b3e6:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000b3e8:	f001 080f 	and.w	r8, r1, #15
3000b3ec:	68a1      	ldr	r1, [r4, #8]
3000b3ee:	6a09      	ldr	r1, [r1, #32]
3000b3f0:	d005      	beq.n	3000b3fe <rxi316_dram_init+0x546>
3000b3f2:	ea0a 0701 	and.w	r7, sl, r1
3000b3f6:	fa21 f10b 	lsr.w	r1, r1, fp
3000b3fa:	b107      	cbz	r7, 3000b3fe <rxi316_dram_init+0x546>
3000b3fc:	3101      	adds	r1, #1
3000b3fe:	f1b9 0f02 	cmp.w	r9, #2
3000b402:	f000 8154 	beq.w	3000b6ae <rxi316_dram_init+0x7f6>
3000b406:	f1b9 0f03 	cmp.w	r9, #3
3000b40a:	f000 81b6 	beq.w	3000b77a <rxi316_dram_init+0x8c2>
3000b40e:	f1b9 0f09 	cmp.w	r9, #9
3000b412:	f000 822f 	beq.w	3000b874 <rxi316_dram_init+0x9bc>
3000b416:	f04f 0e00 	mov.w	lr, #0
3000b41a:	46f1      	mov	r9, lr
3000b41c:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b420:	0109      	lsls	r1, r1, #4
3000b422:	9f06      	ldr	r7, [sp, #24]
3000b424:	b2c9      	uxtb	r1, r1
3000b426:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000b42a:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b42c:	ea41 0108 	orr.w	r1, r1, r8
3000b430:	4339      	orrs	r1, r7
3000b432:	4f48      	ldr	r7, [pc, #288]	; (3000b554 <rxi316_dram_init+0x69c>)
3000b434:	ea41 0109 	orr.w	r1, r1, r9
3000b438:	ea41 010e 	orr.w	r1, r1, lr
3000b43c:	61f9      	str	r1, [r7, #28]
3000b43e:	68a7      	ldr	r7, [r4, #8]
3000b440:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000b442:	fbb4 f1f3 	udiv	r1, r4, r3
3000b446:	fb03 4311 	mls	r3, r3, r1, r4
3000b44a:	b103      	cbz	r3, 3000b44e <rxi316_dram_init+0x596>
3000b44c:	3101      	adds	r1, #1
3000b44e:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000b450:	094b      	lsrs	r3, r1, #5
3000b452:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000b456:	0912      	lsrs	r2, r2, #4
3000b458:	ea14 0f0a 	tst.w	r4, sl
3000b45c:	fa24 f80b 	lsr.w	r8, r4, fp
3000b460:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000b462:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000b466:	bf18      	it	ne
3000b468:	f108 0801 	addne.w	r8, r8, #1
3000b46c:	9306      	str	r3, [sp, #24]
3000b46e:	ea14 0f0a 	tst.w	r4, sl
3000b472:	fa24 fe0b 	lsr.w	lr, r4, fp
3000b476:	9b01      	ldr	r3, [sp, #4]
3000b478:	fa29 f40b 	lsr.w	r4, r9, fp
3000b47c:	bf18      	it	ne
3000b47e:	f10e 0e01 	addne.w	lr, lr, #1
3000b482:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000b484:	ea19 0f0a 	tst.w	r9, sl
3000b488:	fa23 f90b 	lsr.w	r9, r3, fp
3000b48c:	9905      	ldr	r1, [sp, #20]
3000b48e:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000b492:	bf18      	it	ne
3000b494:	3401      	addne	r4, #1
3000b496:	45b9      	cmp	r9, r7
3000b498:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000b49c:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000b4a0:	f240 80eb 	bls.w	3000b67a <rxi316_dram_init+0x7c2>
3000b4a4:	eba9 0707 	sub.w	r7, r9, r7
3000b4a8:	fa27 f40b 	lsr.w	r4, r7, fp
3000b4ac:	ea17 070a 	ands.w	r7, r7, sl
3000b4b0:	d001      	beq.n	3000b4b6 <rxi316_dram_init+0x5fe>
3000b4b2:	3401      	adds	r4, #1
3000b4b4:	2700      	movs	r7, #0
3000b4b6:	9b04      	ldr	r3, [sp, #16]
3000b4b8:	b10b      	cbz	r3, 3000b4be <rxi316_dram_init+0x606>
3000b4ba:	073b      	lsls	r3, r7, #28
3000b4bc:	9304      	str	r3, [sp, #16]
3000b4be:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000b4c2:	9b06      	ldr	r3, [sp, #24]
3000b4c4:	0289      	lsls	r1, r1, #10
3000b4c6:	02c0      	lsls	r0, r0, #11
3000b4c8:	f00c 0c04 	and.w	ip, ip, #4
3000b4cc:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000b4d0:	01d2      	lsls	r2, r2, #7
3000b4d2:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000b4d6:	0236      	lsls	r6, r6, #8
3000b4d8:	ea4c 0c01 	orr.w	ip, ip, r1
3000b4dc:	b2d2      	uxtb	r2, r2
3000b4de:	026d      	lsls	r5, r5, #9
3000b4e0:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000b4e4:	ea4c 0c00 	orr.w	ip, ip, r0
3000b4e8:	00df      	lsls	r7, r3, #3
3000b4ea:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000b4ee:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000b4f2:	ea4c 0202 	orr.w	r2, ip, r2
3000b4f6:	f007 0708 	and.w	r7, r7, #8
3000b4fa:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000b4fe:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000b502:	4332      	orrs	r2, r6
3000b504:	0524      	lsls	r4, r4, #20
3000b506:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000b50a:	4912      	ldr	r1, [pc, #72]	; (3000b554 <rxi316_dram_init+0x69c>)
3000b50c:	ea42 0305 	orr.w	r3, r2, r5
3000b510:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000b514:	9a04      	ldr	r2, [sp, #16]
3000b516:	433b      	orrs	r3, r7
3000b518:	ea43 0308 	orr.w	r3, r3, r8
3000b51c:	ea43 030e 	orr.w	r3, r3, lr
3000b520:	4323      	orrs	r3, r4
3000b522:	4313      	orrs	r3, r2
3000b524:	9a03      	ldr	r2, [sp, #12]
3000b526:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000b52a:	620b      	str	r3, [r1, #32]
3000b52c:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000b530:	9b02      	ldr	r3, [sp, #8]
3000b532:	2b01      	cmp	r3, #1
3000b534:	d002      	beq.n	3000b53c <rxi316_dram_init+0x684>
3000b536:	9b08      	ldr	r3, [sp, #32]
3000b538:	3303      	adds	r3, #3
3000b53a:	9307      	str	r3, [sp, #28]
3000b53c:	4b05      	ldr	r3, [pc, #20]	; (3000b554 <rxi316_dram_init+0x69c>)
3000b53e:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000b542:	9a07      	ldr	r2, [sp, #28]
3000b544:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000b548:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000b54c:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000b550:	e002      	b.n	3000b558 <rxi316_dram_init+0x6a0>
3000b552:	bf00      	nop
3000b554:	41100000 	.word	0x41100000
3000b558:	60da      	str	r2, [r3, #12]
3000b55a:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b55e:	430a      	orrs	r2, r1
3000b560:	49b3      	ldr	r1, [pc, #716]	; (3000b830 <rxi316_dram_init+0x978>)
3000b562:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b566:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000b56a:	0096      	lsls	r6, r2, #2
3000b56c:	d57f      	bpl.n	3000b66e <rxi316_dram_init+0x7b6>
3000b56e:	691a      	ldr	r2, [r3, #16]
3000b570:	4cb0      	ldr	r4, [pc, #704]	; (3000b834 <rxi316_dram_init+0x97c>)
3000b572:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000b576:	611a      	str	r2, [r3, #16]
3000b578:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000b57c:	601a      	str	r2, [r3, #0]
3000b57e:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000b582:	f043 0303 	orr.w	r3, r3, #3
3000b586:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000b58a:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000b58e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000b590:	2b01      	cmp	r3, #1
3000b592:	4ba9      	ldr	r3, [pc, #676]	; (3000b838 <rxi316_dram_init+0x980>)
3000b594:	bf0c      	ite	eq
3000b596:	4aa9      	ldreq	r2, [pc, #676]	; (3000b83c <rxi316_dram_init+0x984>)
3000b598:	4aa9      	ldrne	r2, [pc, #676]	; (3000b840 <rxi316_dram_init+0x988>)
3000b59a:	601a      	str	r2, [r3, #0]
3000b59c:	2203      	movs	r2, #3
3000b59e:	68e0      	ldr	r0, [r4, #12]
3000b5a0:	4ba8      	ldr	r3, [pc, #672]	; (3000b844 <rxi316_dram_init+0x98c>)
3000b5a2:	4003      	ands	r3, r0
3000b5a4:	60e3      	str	r3, [r4, #12]
3000b5a6:	2308      	movs	r3, #8
3000b5a8:	62a1      	str	r1, [r4, #40]	; 0x28
3000b5aa:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b5ac:	6023      	str	r3, [r4, #0]
3000b5ae:	6823      	ldr	r3, [r4, #0]
3000b5b0:	071d      	lsls	r5, r3, #28
3000b5b2:	d5fc      	bpl.n	3000b5ae <rxi316_dram_init+0x6f6>
3000b5b4:	4ba4      	ldr	r3, [pc, #656]	; (3000b848 <rxi316_dram_init+0x990>)
3000b5b6:	2001      	movs	r0, #1
3000b5b8:	4798      	blx	r3
3000b5ba:	68e2      	ldr	r2, [r4, #12]
3000b5bc:	4ba1      	ldr	r3, [pc, #644]	; (3000b844 <rxi316_dram_init+0x98c>)
3000b5be:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000b5c2:	4013      	ands	r3, r2
3000b5c4:	2203      	movs	r2, #3
3000b5c6:	60e3      	str	r3, [r4, #12]
3000b5c8:	2308      	movs	r3, #8
3000b5ca:	62a1      	str	r1, [r4, #40]	; 0x28
3000b5cc:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b5ce:	4a99      	ldr	r2, [pc, #612]	; (3000b834 <rxi316_dram_init+0x97c>)
3000b5d0:	6023      	str	r3, [r4, #0]
3000b5d2:	6813      	ldr	r3, [r2, #0]
3000b5d4:	071c      	lsls	r4, r3, #28
3000b5d6:	d5fc      	bpl.n	3000b5d2 <rxi316_dram_init+0x71a>
3000b5d8:	68d0      	ldr	r0, [r2, #12]
3000b5da:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000b5de:	4b99      	ldr	r3, [pc, #612]	; (3000b844 <rxi316_dram_init+0x98c>)
3000b5e0:	4003      	ands	r3, r0
3000b5e2:	2008      	movs	r0, #8
3000b5e4:	60d3      	str	r3, [r2, #12]
3000b5e6:	6291      	str	r1, [r2, #40]	; 0x28
3000b5e8:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000b5ea:	4992      	ldr	r1, [pc, #584]	; (3000b834 <rxi316_dram_init+0x97c>)
3000b5ec:	f023 0302 	bic.w	r3, r3, #2
3000b5f0:	62d3      	str	r3, [r2, #44]	; 0x2c
3000b5f2:	6010      	str	r0, [r2, #0]
3000b5f4:	680b      	ldr	r3, [r1, #0]
3000b5f6:	0718      	lsls	r0, r3, #28
3000b5f8:	d5fc      	bpl.n	3000b5f4 <rxi316_dram_init+0x73c>
3000b5fa:	4a8e      	ldr	r2, [pc, #568]	; (3000b834 <rxi316_dram_init+0x97c>)
3000b5fc:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000b600:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b604:	60d3      	str	r3, [r2, #12]
3000b606:	980a      	ldr	r0, [sp, #40]	; 0x28
3000b608:	6913      	ldr	r3, [r2, #16]
3000b60a:	4318      	orrs	r0, r3
3000b60c:	6110      	str	r0, [r2, #16]
3000b60e:	6913      	ldr	r3, [r2, #16]
3000b610:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000b614:	6113      	str	r3, [r2, #16]
3000b616:	6011      	str	r1, [r2, #0]
3000b618:	68d3      	ldr	r3, [r2, #12]
3000b61a:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000b61e:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b622:	d1f9      	bne.n	3000b618 <rxi316_dram_init+0x760>
3000b624:	b00f      	add	sp, #60	; 0x3c
3000b626:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b62a:	2a01      	cmp	r2, #1
3000b62c:	f000 80d1 	beq.w	3000b7d2 <rxi316_dram_init+0x91a>
3000b630:	2a02      	cmp	r2, #2
3000b632:	f000 8139 	beq.w	3000b8a8 <rxi316_dram_init+0x9f0>
3000b636:	2a03      	cmp	r2, #3
3000b638:	f47f ace4 	bne.w	3000b004 <rxi316_dram_init+0x14c>
3000b63c:	686a      	ldr	r2, [r5, #4]
3000b63e:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b642:	606a      	str	r2, [r5, #4]
3000b644:	e4de      	b.n	3000b004 <rxi316_dram_init+0x14c>
3000b646:	2a00      	cmp	r2, #0
3000b648:	f43f ad6a 	beq.w	3000b120 <rxi316_dram_init+0x268>
3000b64c:	2203      	movs	r2, #3
3000b64e:	4310      	orrs	r0, r2
3000b650:	68a2      	ldr	r2, [r4, #8]
3000b652:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b654:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b656:	013f      	lsls	r7, r7, #4
3000b658:	2d00      	cmp	r5, #0
3000b65a:	f000 80d7 	beq.w	3000b80c <rxi316_dram_init+0x954>
3000b65e:	1e91      	subs	r1, r2, #2
3000b660:	2906      	cmp	r1, #6
3000b662:	f200 80b0 	bhi.w	3000b7c6 <rxi316_dram_init+0x90e>
3000b666:	1e55      	subs	r5, r2, #1
3000b668:	2100      	movs	r1, #0
3000b66a:	026d      	lsls	r5, r5, #9
3000b66c:	e55f      	b.n	3000b12e <rxi316_dram_init+0x276>
3000b66e:	2201      	movs	r2, #1
3000b670:	601a      	str	r2, [r3, #0]
3000b672:	681a      	ldr	r2, [r3, #0]
3000b674:	07d2      	lsls	r2, r2, #31
3000b676:	d5fc      	bpl.n	3000b672 <rxi316_dram_init+0x7ba>
3000b678:	e7bf      	b.n	3000b5fa <rxi316_dram_init+0x742>
3000b67a:	2701      	movs	r7, #1
3000b67c:	e71b      	b.n	3000b4b6 <rxi316_dram_init+0x5fe>
3000b67e:	68a1      	ldr	r1, [r4, #8]
3000b680:	2f03      	cmp	r7, #3
3000b682:	6a09      	ldr	r1, [r1, #32]
3000b684:	f000 80bf 	beq.w	3000b806 <rxi316_dram_init+0x94e>
3000b688:	2f02      	cmp	r7, #2
3000b68a:	f040 8118 	bne.w	3000b8be <rxi316_dram_init+0xa06>
3000b68e:	fbb1 fcf3 	udiv	ip, r1, r3
3000b692:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b696:	4661      	mov	r1, ip
3000b698:	f1be 0f00 	cmp.w	lr, #0
3000b69c:	f000 80a9 	beq.w	3000b7f2 <rxi316_dram_init+0x93a>
3000b6a0:	f1b9 0f02 	cmp.w	r9, #2
3000b6a4:	f101 0101 	add.w	r1, r1, #1
3000b6a8:	46b8      	mov	r8, r7
3000b6aa:	f47f aeac 	bne.w	3000b406 <rxi316_dram_init+0x54e>
3000b6ae:	4f67      	ldr	r7, [pc, #412]	; (3000b84c <rxi316_dram_init+0x994>)
3000b6b0:	f04f 0e00 	mov.w	lr, #0
3000b6b4:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b850 <rxi316_dram_init+0x998>
3000b6b8:	fbb7 f7f3 	udiv	r7, r7, r3
3000b6bc:	fbb9 f9f3 	udiv	r9, r9, r3
3000b6c0:	09ff      	lsrs	r7, r7, #7
3000b6c2:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b6c6:	3701      	adds	r7, #1
3000b6c8:	f109 0c01 	add.w	ip, r9, #1
3000b6cc:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b854 <rxi316_dram_init+0x99c>
3000b6d0:	023f      	lsls	r7, r7, #8
3000b6d2:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b6d6:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b6da:	9709      	str	r7, [sp, #36]	; 0x24
3000b6dc:	e6a0      	b.n	3000b420 <rxi316_dram_init+0x568>
3000b6de:	f8cd c018 	str.w	ip, [sp, #24]
3000b6e2:	e5bf      	b.n	3000b264 <rxi316_dram_init+0x3ac>
3000b6e4:	38c8      	subs	r0, #200	; 0xc8
3000b6e6:	e596      	b.n	3000b216 <rxi316_dram_init+0x35e>
3000b6e8:	9105      	str	r1, [sp, #20]
3000b6ea:	e58b      	b.n	3000b204 <rxi316_dram_init+0x34c>
3000b6ec:	fa22 f00b 	lsr.w	r0, r2, fp
3000b6f0:	e547      	b.n	3000b182 <rxi316_dram_init+0x2ca>
3000b6f2:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000b6f6:	ea1a 0f07 	tst.w	sl, r7
3000b6fa:	fa27 f20b 	lsr.w	r2, r7, fp
3000b6fe:	d000      	beq.n	3000b702 <rxi316_dram_init+0x84a>
3000b700:	3201      	adds	r2, #1
3000b702:	2e02      	cmp	r6, #2
3000b704:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b708:	d10f      	bne.n	3000b72a <rxi316_dram_init+0x872>
3000b70a:	9e08      	ldr	r6, [sp, #32]
3000b70c:	3e02      	subs	r6, #2
3000b70e:	fa26 f60b 	lsr.w	r6, r6, fp
3000b712:	4437      	add	r7, r6
3000b714:	fbb7 f6f3 	udiv	r6, r7, r3
3000b718:	fb03 7716 	mls	r7, r3, r6, r7
3000b71c:	2f00      	cmp	r7, #0
3000b71e:	f43f ad52 	beq.w	3000b1c6 <rxi316_dram_init+0x30e>
3000b722:	3601      	adds	r6, #1
3000b724:	e54f      	b.n	3000b1c6 <rxi316_dram_init+0x30e>
3000b726:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b72a:	fbb7 f6f3 	udiv	r6, r7, r3
3000b72e:	fb03 7716 	mls	r7, r3, r6, r7
3000b732:	2f00      	cmp	r7, #0
3000b734:	f43f ad47 	beq.w	3000b1c6 <rxi316_dram_init+0x30e>
3000b738:	3601      	adds	r6, #1
3000b73a:	e544      	b.n	3000b1c6 <rxi316_dram_init+0x30e>
3000b73c:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b73e:	46a8      	mov	r8, r5
3000b740:	9d08      	ldr	r5, [sp, #32]
3000b742:	44a8      	add	r8, r5
3000b744:	e611      	b.n	3000b36a <rxi316_dram_init+0x4b2>
3000b746:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b748:	1f50      	subs	r0, r2, #5
3000b74a:	00c0      	lsls	r0, r0, #3
3000b74c:	68e2      	ldr	r2, [r4, #12]
3000b74e:	6812      	ldr	r2, [r2, #0]
3000b750:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b752:	fab2 f282 	clz	r2, r2
3000b756:	0952      	lsrs	r2, r2, #5
3000b758:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b75c:	4301      	orrs	r1, r0
3000b75e:	e4fd      	b.n	3000b15c <rxi316_dram_init+0x2a4>
3000b760:	f1a1 0502 	sub.w	r5, r1, #2
3000b764:	2909      	cmp	r1, #9
3000b766:	fab5 f585 	clz	r5, r5
3000b76a:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b76e:	f000 8099 	beq.w	3000b8a4 <rxi316_dram_init+0x9ec>
3000b772:	462a      	mov	r2, r5
3000b774:	e4d1      	b.n	3000b11a <rxi316_dram_init+0x262>
3000b776:	2000      	movs	r0, #0
3000b778:	e7e8      	b.n	3000b74c <rxi316_dram_init+0x894>
3000b77a:	f242 7710 	movw	r7, #10000	; 0x2710
3000b77e:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b858 <rxi316_dram_init+0x9a0>
3000b782:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b850 <rxi316_dram_init+0x998>
3000b786:	fbb7 fcf3 	udiv	ip, r7, r3
3000b78a:	9f05      	ldr	r7, [sp, #20]
3000b78c:	4467      	add	r7, ip
3000b78e:	fbb9 f9f3 	udiv	r9, r9, r3
3000b792:	fbbe fef3 	udiv	lr, lr, r3
3000b796:	09ff      	lsrs	r7, r7, #7
3000b798:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b79c:	3701      	adds	r7, #1
3000b79e:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b7a2:	f109 0c01 	add.w	ip, r9, #1
3000b7a6:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b854 <rxi316_dram_init+0x99c>
3000b7aa:	023f      	lsls	r7, r7, #8
3000b7ac:	f10e 0e01 	add.w	lr, lr, #1
3000b7b0:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b7b4:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b7b8:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b7bc:	9709      	str	r7, [sp, #36]	; 0x24
3000b7be:	e62f      	b.n	3000b420 <rxi316_dram_init+0x568>
3000b7c0:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b7c4:	e5d1      	b.n	3000b36a <rxi316_dram_init+0x4b2>
3000b7c6:	2100      	movs	r1, #0
3000b7c8:	460d      	mov	r5, r1
3000b7ca:	e4b0      	b.n	3000b12e <rxi316_dram_init+0x276>
3000b7cc:	3a07      	subs	r2, #7
3000b7ce:	f7ff bbb8 	b.w	3000af42 <rxi316_dram_init+0x8a>
3000b7d2:	686a      	ldr	r2, [r5, #4]
3000b7d4:	f042 0220 	orr.w	r2, r2, #32
3000b7d8:	606a      	str	r2, [r5, #4]
3000b7da:	e413      	b.n	3000b004 <rxi316_dram_init+0x14c>
3000b7dc:	2204      	movs	r2, #4
3000b7de:	2102      	movs	r1, #2
3000b7e0:	2500      	movs	r5, #0
3000b7e2:	9201      	str	r2, [sp, #4]
3000b7e4:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b7e8:	f7ff bb94 	b.w	3000af14 <rxi316_dram_init+0x5c>
3000b7ec:	3a08      	subs	r2, #8
3000b7ee:	f7ff bba8 	b.w	3000af42 <rxi316_dram_init+0x8a>
3000b7f2:	46b8      	mov	r8, r7
3000b7f4:	e603      	b.n	3000b3fe <rxi316_dram_init+0x546>
3000b7f6:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b7f8:	2a04      	cmp	r2, #4
3000b7fa:	d90c      	bls.n	3000b816 <rxi316_dram_init+0x95e>
3000b7fc:	2a0b      	cmp	r2, #11
3000b7fe:	d86a      	bhi.n	3000b8d6 <rxi316_dram_init+0xa1e>
3000b800:	1f17      	subs	r7, r2, #4
3000b802:	013f      	lsls	r7, r7, #4
3000b804:	e008      	b.n	3000b818 <rxi316_dram_init+0x960>
3000b806:	f04f 0802 	mov.w	r8, #2
3000b80a:	e5f8      	b.n	3000b3fe <rxi316_dram_init+0x546>
3000b80c:	2903      	cmp	r1, #3
3000b80e:	d005      	beq.n	3000b81c <rxi316_dram_init+0x964>
3000b810:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b814:	e48b      	b.n	3000b12e <rxi316_dram_init+0x276>
3000b816:	2710      	movs	r7, #16
3000b818:	68a2      	ldr	r2, [r4, #8]
3000b81a:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b81c:	2a04      	cmp	r2, #4
3000b81e:	d949      	bls.n	3000b8b4 <rxi316_dram_init+0x9fc>
3000b820:	1f51      	subs	r1, r2, #5
3000b822:	2903      	cmp	r1, #3
3000b824:	d84e      	bhi.n	3000b8c4 <rxi316_dram_init+0xa0c>
3000b826:	1f15      	subs	r5, r2, #4
3000b828:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b82c:	026d      	lsls	r5, r5, #9
3000b82e:	e47e      	b.n	3000b12e <rxi316_dram_init+0x276>
3000b830:	42008000 	.word	0x42008000
3000b834:	41100000 	.word	0x41100000
3000b838:	41011000 	.word	0x41011000
3000b83c:	44000103 	.word	0x44000103
3000b840:	44000021 	.word	0x44000021
3000b844:	fff8fffc 	.word	0xfff8fffc
3000b848:	00009b2d 	.word	0x00009b2d
3000b84c:	00061a80 	.word	0x00061a80
3000b850:	0bebc200 	.word	0x0bebc200
3000b854:	003ff000 	.word	0x003ff000
3000b858:	1dcd6500 	.word	0x1dcd6500
3000b85c:	f1a1 0c02 	sub.w	ip, r1, #2
3000b860:	f1bc 0f01 	cmp.w	ip, #1
3000b864:	d903      	bls.n	3000b86e <rxi316_dram_init+0x9b6>
3000b866:	f1be 0f00 	cmp.w	lr, #0
3000b86a:	f43f ac56 	beq.w	3000b11a <rxi316_dram_init+0x262>
3000b86e:	f040 0002 	orr.w	r0, r0, #2
3000b872:	e452      	b.n	3000b11a <rxi316_dram_init+0x262>
3000b874:	4f1d      	ldr	r7, [pc, #116]	; (3000b8ec <rxi316_dram_init+0xa34>)
3000b876:	f04f 0e00 	mov.w	lr, #0
3000b87a:	fbb7 f9f3 	udiv	r9, r7, r3
3000b87e:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b882:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b886:	f109 0701 	add.w	r7, r9, #1
3000b88a:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b8f0 <rxi316_dram_init+0xa38>
3000b88e:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b892:	e5c5      	b.n	3000b420 <rxi316_dram_init+0x568>
3000b894:	f105 0802 	add.w	r8, r5, #2
3000b898:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b89a:	eba8 0805 	sub.w	r8, r8, r5
3000b89e:	9d08      	ldr	r5, [sp, #32]
3000b8a0:	44a8      	add	r8, r5
3000b8a2:	e562      	b.n	3000b36a <rxi316_dram_init+0x4b2>
3000b8a4:	2204      	movs	r2, #4
3000b8a6:	e6d2      	b.n	3000b64e <rxi316_dram_init+0x796>
3000b8a8:	686a      	ldr	r2, [r5, #4]
3000b8aa:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b8ae:	606a      	str	r2, [r5, #4]
3000b8b0:	f7ff bba8 	b.w	3000b004 <rxi316_dram_init+0x14c>
3000b8b4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b8b8:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b8bc:	e437      	b.n	3000b12e <rxi316_dram_init+0x276>
3000b8be:	f04f 0802 	mov.w	r8, #2
3000b8c2:	e596      	b.n	3000b3f2 <rxi316_dram_init+0x53a>
3000b8c4:	f1a2 0109 	sub.w	r1, r2, #9
3000b8c8:	2905      	cmp	r1, #5
3000b8ca:	d80b      	bhi.n	3000b8e4 <rxi316_dram_init+0xa2c>
3000b8cc:	0855      	lsrs	r5, r2, #1
3000b8ce:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b8d2:	026d      	lsls	r5, r5, #9
3000b8d4:	e42b      	b.n	3000b12e <rxi316_dram_init+0x276>
3000b8d6:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b8d8:	f040 0004 	orr.w	r0, r0, #4
3000b8dc:	f1a2 070c 	sub.w	r7, r2, #12
3000b8e0:	013f      	lsls	r7, r7, #4
3000b8e2:	e799      	b.n	3000b818 <rxi316_dram_init+0x960>
3000b8e4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b8e8:	2500      	movs	r5, #0
3000b8ea:	e420      	b.n	3000b12e <rxi316_dram_init+0x276>
3000b8ec:	0bebc200 	.word	0x0bebc200
3000b8f0:	003ff000 	.word	0x003ff000

3000b8f4 <rxi316_ftend_init>:
3000b8f4:	2300      	movs	r3, #0
3000b8f6:	b510      	push	{r4, lr}
3000b8f8:	4c0c      	ldr	r4, [pc, #48]	; (3000b92c <rxi316_ftend_init+0x38>)
3000b8fa:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b8fe:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b902:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b906:	f7fe fe51 	bl	3000a5ac <ddr_init_index>
3000b90a:	4a09      	ldr	r2, [pc, #36]	; (3000b930 <rxi316_ftend_init+0x3c>)
3000b90c:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b910:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b914:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b918:	68cb      	ldr	r3, [r1, #12]
3000b91a:	689b      	ldr	r3, [r3, #8]
3000b91c:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b91e:	3301      	adds	r3, #1
3000b920:	b2db      	uxtb	r3, r3
3000b922:	4313      	orrs	r3, r2
3000b924:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b928:	bd10      	pop	{r4, pc}
3000b92a:	bf00      	nop
3000b92c:	41101000 	.word	0x41101000
3000b930:	3000e92c 	.word	0x3000e92c

3000b934 <rxi316_DynSre_init>:
3000b934:	2901      	cmp	r1, #1
3000b936:	d013      	beq.n	3000b960 <rxi316_DynSre_init+0x2c>
3000b938:	4a12      	ldr	r2, [pc, #72]	; (3000b984 <rxi316_DynSre_init+0x50>)
3000b93a:	6893      	ldr	r3, [r2, #8]
3000b93c:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b940:	6093      	str	r3, [r2, #8]
3000b942:	4b10      	ldr	r3, [pc, #64]	; (3000b984 <rxi316_DynSre_init+0x50>)
3000b944:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b948:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b94c:	625a      	str	r2, [r3, #36]	; 0x24
3000b94e:	629a      	str	r2, [r3, #40]	; 0x28
3000b950:	69da      	ldr	r2, [r3, #28]
3000b952:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b956:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b95a:	61da      	str	r2, [r3, #28]
3000b95c:	6019      	str	r1, [r3, #0]
3000b95e:	4770      	bx	lr
3000b960:	4b08      	ldr	r3, [pc, #32]	; (3000b984 <rxi316_DynSre_init+0x50>)
3000b962:	b280      	uxth	r0, r0
3000b964:	4a08      	ldr	r2, [pc, #32]	; (3000b988 <rxi316_DynSre_init+0x54>)
3000b966:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b96a:	400a      	ands	r2, r1
3000b96c:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b970:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b974:	4310      	orrs	r0, r2
3000b976:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b97a:	689a      	ldr	r2, [r3, #8]
3000b97c:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b980:	609a      	str	r2, [r3, #8]
3000b982:	e7de      	b.n	3000b942 <rxi316_DynSre_init+0xe>
3000b984:	41100000 	.word	0x41100000
3000b988:	ffff0000 	.word	0xffff0000

3000b98c <ddr_init>:
3000b98c:	b500      	push	{lr}
3000b98e:	b083      	sub	sp, #12
3000b990:	f7fe fe0c 	bl	3000a5ac <ddr_init_index>
3000b994:	4b06      	ldr	r3, [pc, #24]	; (3000b9b0 <ddr_init+0x24>)
3000b996:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b99a:	9001      	str	r0, [sp, #4]
3000b99c:	f7fe fe30 	bl	3000a600 <rxi316_perf_tune>
3000b9a0:	9801      	ldr	r0, [sp, #4]
3000b9a2:	f7ff fa89 	bl	3000aeb8 <rxi316_dram_init>
3000b9a6:	b003      	add	sp, #12
3000b9a8:	f85d eb04 	ldr.w	lr, [sp], #4
3000b9ac:	f7ff bfa2 	b.w	3000b8f4 <rxi316_ftend_init>
3000b9b0:	3000e92c 	.word	0x3000e92c

3000b9b4 <SWR_MEM>:
3000b9b4:	2801      	cmp	r0, #1
3000b9b6:	d00e      	beq.n	3000b9d6 <SWR_MEM+0x22>
3000b9b8:	4b31      	ldr	r3, [pc, #196]	; (3000ba80 <SWR_MEM+0xcc>)
3000b9ba:	2000      	movs	r0, #0
3000b9bc:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b9c0:	f022 0201 	bic.w	r2, r2, #1
3000b9c4:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b9c8:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b9cc:	f022 0202 	bic.w	r2, r2, #2
3000b9d0:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b9d4:	4770      	bx	lr
3000b9d6:	b510      	push	{r4, lr}
3000b9d8:	4c29      	ldr	r4, [pc, #164]	; (3000ba80 <SWR_MEM+0xcc>)
3000b9da:	4a2a      	ldr	r2, [pc, #168]	; (3000ba84 <SWR_MEM+0xd0>)
3000b9dc:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b9e0:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b9e4:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b9e8:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b9ec:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b9f0:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b9f4:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b9f8:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b9fc:	f023 0303 	bic.w	r3, r3, #3
3000ba00:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ba04:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ba08:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ba0c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ba10:	4790      	blx	r2
3000ba12:	2803      	cmp	r0, #3
3000ba14:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000ba18:	d026      	beq.n	3000ba68 <SWR_MEM+0xb4>
3000ba1a:	f043 0303 	orr.w	r3, r3, #3
3000ba1e:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ba22:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ba26:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000ba2a:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ba2e:	4c14      	ldr	r4, [pc, #80]	; (3000ba80 <SWR_MEM+0xcc>)
3000ba30:	4b15      	ldr	r3, [pc, #84]	; (3000ba88 <SWR_MEM+0xd4>)
3000ba32:	4798      	blx	r3
3000ba34:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000ba38:	4a14      	ldr	r2, [pc, #80]	; (3000ba8c <SWR_MEM+0xd8>)
3000ba3a:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000ba3e:	f043 0302 	orr.w	r3, r3, #2
3000ba42:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000ba46:	4790      	blx	r2
3000ba48:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ba4c:	4b10      	ldr	r3, [pc, #64]	; (3000ba90 <SWR_MEM+0xdc>)
3000ba4e:	f042 0201 	orr.w	r2, r2, #1
3000ba52:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000ba56:	e001      	b.n	3000ba5c <SWR_MEM+0xa8>
3000ba58:	3b01      	subs	r3, #1
3000ba5a:	d00e      	beq.n	3000ba7a <SWR_MEM+0xc6>
3000ba5c:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ba60:	03d2      	lsls	r2, r2, #15
3000ba62:	d5f9      	bpl.n	3000ba58 <SWR_MEM+0xa4>
3000ba64:	2000      	movs	r0, #0
3000ba66:	bd10      	pop	{r4, pc}
3000ba68:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000ba6c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ba70:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000ba74:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ba78:	e7d9      	b.n	3000ba2e <SWR_MEM+0x7a>
3000ba7a:	2001      	movs	r0, #1
3000ba7c:	bd10      	pop	{r4, pc}
3000ba7e:	bf00      	nop
3000ba80:	42008800 	.word	0x42008800
3000ba84:	30007f79 	.word	0x30007f79
3000ba88:	3000bc55 	.word	0x3000bc55
3000ba8c:	00009b2d 	.word	0x00009b2d
3000ba90:	000186a0 	.word	0x000186a0

3000ba94 <SWR_MEM_Manual>:
3000ba94:	4a06      	ldr	r2, [pc, #24]	; (3000bab0 <SWR_MEM_Manual+0x1c>)
3000ba96:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000ba9a:	b120      	cbz	r0, 3000baa6 <SWR_MEM_Manual+0x12>
3000ba9c:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000baa0:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000baa4:	4770      	bx	lr
3000baa6:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000baaa:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000baae:	4770      	bx	lr
3000bab0:	42008800 	.word	0x42008800

3000bab4 <SWR_MEM_Mode_Set>:
3000bab4:	2801      	cmp	r0, #1
3000bab6:	b538      	push	{r3, r4, r5, lr}
3000bab8:	d807      	bhi.n	3000baca <SWR_MEM_Mode_Set+0x16>
3000baba:	d10a      	bne.n	3000bad2 <SWR_MEM_Mode_Set+0x1e>
3000babc:	4c22      	ldr	r4, [pc, #136]	; (3000bb48 <SWR_MEM_Mode_Set+0x94>)
3000babe:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000bac2:	035b      	lsls	r3, r3, #13
3000bac4:	d51b      	bpl.n	3000bafe <SWR_MEM_Mode_Set+0x4a>
3000bac6:	2000      	movs	r0, #0
3000bac8:	bd38      	pop	{r3, r4, r5, pc}
3000baca:	217d      	movs	r1, #125	; 0x7d
3000bacc:	481f      	ldr	r0, [pc, #124]	; (3000bb4c <SWR_MEM_Mode_Set+0x98>)
3000bace:	f000 fb9b 	bl	3000c208 <__io_assert_failed_veneer>
3000bad2:	491d      	ldr	r1, [pc, #116]	; (3000bb48 <SWR_MEM_Mode_Set+0x94>)
3000bad4:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000bad8:	0358      	lsls	r0, r3, #13
3000bada:	d5f4      	bpl.n	3000bac6 <SWR_MEM_Mode_Set+0x12>
3000badc:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000bae0:	f242 7310 	movw	r3, #10000	; 0x2710
3000bae4:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000bae8:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000baec:	e001      	b.n	3000baf2 <SWR_MEM_Mode_Set+0x3e>
3000baee:	3b01      	subs	r3, #1
3000baf0:	d027      	beq.n	3000bb42 <SWR_MEM_Mode_Set+0x8e>
3000baf2:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000baf6:	0352      	lsls	r2, r2, #13
3000baf8:	d4f9      	bmi.n	3000baee <SWR_MEM_Mode_Set+0x3a>
3000bafa:	2000      	movs	r0, #0
3000bafc:	e7e4      	b.n	3000bac8 <SWR_MEM_Mode_Set+0x14>
3000bafe:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000bb02:	2064      	movs	r0, #100	; 0x64
3000bb04:	4d12      	ldr	r5, [pc, #72]	; (3000bb50 <SWR_MEM_Mode_Set+0x9c>)
3000bb06:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000bb0a:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000bb0e:	47a8      	blx	r5
3000bb10:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000bb14:	2064      	movs	r0, #100	; 0x64
3000bb16:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000bb1a:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000bb1e:	47a8      	blx	r5
3000bb20:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000bb24:	f242 7310 	movw	r3, #10000	; 0x2710
3000bb28:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000bb2c:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000bb30:	e001      	b.n	3000bb36 <SWR_MEM_Mode_Set+0x82>
3000bb32:	3b01      	subs	r3, #1
3000bb34:	d005      	beq.n	3000bb42 <SWR_MEM_Mode_Set+0x8e>
3000bb36:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000bb3a:	0355      	lsls	r5, r2, #13
3000bb3c:	d5f9      	bpl.n	3000bb32 <SWR_MEM_Mode_Set+0x7e>
3000bb3e:	2000      	movs	r0, #0
3000bb40:	e7c2      	b.n	3000bac8 <SWR_MEM_Mode_Set+0x14>
3000bb42:	2001      	movs	r0, #1
3000bb44:	bd38      	pop	{r3, r4, r5, pc}
3000bb46:	bf00      	nop
3000bb48:	42008800 	.word	0x42008800
3000bb4c:	3000d748 	.word	0x3000d748
3000bb50:	00009b2d 	.word	0x00009b2d

3000bb54 <SWR_AUDIO>:
3000bb54:	2801      	cmp	r0, #1
3000bb56:	d00e      	beq.n	3000bb76 <SWR_AUDIO+0x22>
3000bb58:	4b1e      	ldr	r3, [pc, #120]	; (3000bbd4 <SWR_AUDIO+0x80>)
3000bb5a:	2000      	movs	r0, #0
3000bb5c:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000bb60:	f022 0201 	bic.w	r2, r2, #1
3000bb64:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000bb68:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000bb6c:	f022 0202 	bic.w	r2, r2, #2
3000bb70:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000bb74:	4770      	bx	lr
3000bb76:	b510      	push	{r4, lr}
3000bb78:	4c16      	ldr	r4, [pc, #88]	; (3000bbd4 <SWR_AUDIO+0x80>)
3000bb7a:	4b17      	ldr	r3, [pc, #92]	; (3000bbd8 <SWR_AUDIO+0x84>)
3000bb7c:	4798      	blx	r3
3000bb7e:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000bb82:	4a16      	ldr	r2, [pc, #88]	; (3000bbdc <SWR_AUDIO+0x88>)
3000bb84:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000bb88:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000bb8c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000bb90:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000bb94:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000bb98:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000bb9c:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000bba0:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000bba4:	f043 0302 	orr.w	r3, r3, #2
3000bba8:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000bbac:	4790      	blx	r2
3000bbae:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000bbb2:	4b0b      	ldr	r3, [pc, #44]	; (3000bbe0 <SWR_AUDIO+0x8c>)
3000bbb4:	f042 0201 	orr.w	r2, r2, #1
3000bbb8:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000bbbc:	e001      	b.n	3000bbc2 <SWR_AUDIO+0x6e>
3000bbbe:	3b01      	subs	r3, #1
3000bbc0:	d005      	beq.n	3000bbce <SWR_AUDIO+0x7a>
3000bbc2:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000bbc6:	03d2      	lsls	r2, r2, #15
3000bbc8:	d5f9      	bpl.n	3000bbbe <SWR_AUDIO+0x6a>
3000bbca:	2000      	movs	r0, #0
3000bbcc:	bd10      	pop	{r4, pc}
3000bbce:	2001      	movs	r0, #1
3000bbd0:	bd10      	pop	{r4, pc}
3000bbd2:	bf00      	nop
3000bbd4:	42008800 	.word	0x42008800
3000bbd8:	3000bda5 	.word	0x3000bda5
3000bbdc:	00009b2d 	.word	0x00009b2d
3000bbe0:	000186a0 	.word	0x000186a0

3000bbe4 <SWR_AUDIO_Manual>:
3000bbe4:	4a06      	ldr	r2, [pc, #24]	; (3000bc00 <SWR_AUDIO_Manual+0x1c>)
3000bbe6:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000bbea:	b120      	cbz	r0, 3000bbf6 <SWR_AUDIO_Manual+0x12>
3000bbec:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000bbf0:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000bbf4:	4770      	bx	lr
3000bbf6:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000bbfa:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000bbfe:	4770      	bx	lr
3000bc00:	42008800 	.word	0x42008800

3000bc04 <SWR_BST_MODE_Set>:
3000bc04:	4a06      	ldr	r2, [pc, #24]	; (3000bc20 <SWR_BST_MODE_Set+0x1c>)
3000bc06:	6813      	ldr	r3, [r2, #0]
3000bc08:	b118      	cbz	r0, 3000bc12 <SWR_BST_MODE_Set+0xe>
3000bc0a:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000bc0e:	6013      	str	r3, [r2, #0]
3000bc10:	4770      	bx	lr
3000bc12:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000bc16:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000bc1a:	6013      	str	r3, [r2, #0]
3000bc1c:	4770      	bx	lr
3000bc1e:	bf00      	nop
3000bc20:	42008100 	.word	0x42008100

3000bc24 <SWR_Calib_DCore>:
3000bc24:	4b09      	ldr	r3, [pc, #36]	; (3000bc4c <SWR_Calib_DCore+0x28>)
3000bc26:	f240 3231 	movw	r2, #817	; 0x331
3000bc2a:	4909      	ldr	r1, [pc, #36]	; (3000bc50 <SWR_Calib_DCore+0x2c>)
3000bc2c:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000bc2e:	4001      	ands	r1, r0
3000bc30:	430a      	orrs	r2, r1
3000bc32:	651a      	str	r2, [r3, #80]	; 0x50
3000bc34:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000bc36:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bc3a:	641a      	str	r2, [r3, #64]	; 0x40
3000bc3c:	685a      	ldr	r2, [r3, #4]
3000bc3e:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000bc42:	f042 0204 	orr.w	r2, r2, #4
3000bc46:	605a      	str	r2, [r3, #4]
3000bc48:	4770      	bx	lr
3000bc4a:	bf00      	nop
3000bc4c:	42008100 	.word	0x42008100
3000bc50:	fffffccc 	.word	0xfffffccc

3000bc54 <SWR_Calib_MEM>:
3000bc54:	4b4e      	ldr	r3, [pc, #312]	; (3000bd90 <SWR_Calib_MEM+0x13c>)
3000bc56:	f240 3231 	movw	r2, #817	; 0x331
3000bc5a:	494e      	ldr	r1, [pc, #312]	; (3000bd94 <SWR_Calib_MEM+0x140>)
3000bc5c:	2000      	movs	r0, #0
3000bc5e:	b510      	push	{r4, lr}
3000bc60:	f8d3 40e0 	ldr.w	r4, [r3, #224]	; 0xe0
3000bc64:	b082      	sub	sp, #8
3000bc66:	4021      	ands	r1, r4
3000bc68:	430a      	orrs	r2, r1
3000bc6a:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000bc6e:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000bc72:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000bc76:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bc7a:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000bc7e:	2307      	movs	r3, #7
3000bc80:	f88d 0007 	strb.w	r0, [sp, #7]
3000bc84:	eb0d 0203 	add.w	r2, sp, r3
3000bc88:	f7fd fbda 	bl	30009440 <EFUSE_PMAP_READ8>
3000bc8c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bc90:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bc94:	2af0      	cmp	r2, #240	; 0xf0
3000bc96:	d15b      	bne.n	3000bd50 <SWR_Calib_MEM+0xfc>
3000bc98:	f003 030f 	and.w	r3, r3, #15
3000bc9c:	2b0f      	cmp	r3, #15
3000bc9e:	d053      	beq.n	3000bd48 <SWR_Calib_MEM+0xf4>
3000bca0:	2102      	movs	r1, #2
3000bca2:	4c3d      	ldr	r4, [pc, #244]	; (3000bd98 <SWR_Calib_MEM+0x144>)
3000bca4:	2307      	movs	r3, #7
3000bca6:	f10d 0206 	add.w	r2, sp, #6
3000bcaa:	2000      	movs	r0, #0
3000bcac:	7021      	strb	r1, [r4, #0]
3000bcae:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000bcb2:	f7fd fbc5 	bl	30009440 <EFUSE_PMAP_READ8>
3000bcb6:	7822      	ldrb	r2, [r4, #0]
3000bcb8:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000bcbc:	2a03      	cmp	r2, #3
3000bcbe:	d05d      	beq.n	3000bd7c <SWR_Calib_MEM+0x128>
3000bcc0:	2a02      	cmp	r2, #2
3000bcc2:	bf0c      	ite	eq
3000bcc4:	f003 030f 	andeq.w	r3, r3, #15
3000bcc8:	2300      	movne	r3, #0
3000bcca:	4931      	ldr	r1, [pc, #196]	; (3000bd90 <SWR_Calib_MEM+0x13c>)
3000bccc:	4833      	ldr	r0, [pc, #204]	; (3000bd9c <SWR_Calib_MEM+0x148>)
3000bcce:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000bcd2:	f022 020f 	bic.w	r2, r2, #15
3000bcd6:	4313      	orrs	r3, r2
3000bcd8:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000bcdc:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000bcde:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000bce2:	2b07      	cmp	r3, #7
3000bce4:	d03d      	beq.n	3000bd62 <SWR_Calib_MEM+0x10e>
3000bce6:	2307      	movs	r3, #7
3000bce8:	2000      	movs	r0, #0
3000bcea:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000bcee:	eb0d 0203 	add.w	r2, sp, r3
3000bcf2:	f88d 0007 	strb.w	r0, [sp, #7]
3000bcf6:	f7fd fba3 	bl	30009440 <EFUSE_PMAP_READ8>
3000bcfa:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bcfe:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bd02:	2af0      	cmp	r2, #240	; 0xf0
3000bd04:	d126      	bne.n	3000bd54 <SWR_Calib_MEM+0x100>
3000bd06:	f003 030f 	and.w	r3, r3, #15
3000bd0a:	2b0f      	cmp	r3, #15
3000bd0c:	d024      	beq.n	3000bd58 <SWR_Calib_MEM+0x104>
3000bd0e:	2002      	movs	r0, #2
3000bd10:	2307      	movs	r3, #7
3000bd12:	4c23      	ldr	r4, [pc, #140]	; (3000bda0 <SWR_Calib_MEM+0x14c>)
3000bd14:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000bd18:	eb0d 0203 	add.w	r2, sp, r3
3000bd1c:	7020      	strb	r0, [r4, #0]
3000bd1e:	2000      	movs	r0, #0
3000bd20:	f7fd fb8e 	bl	30009440 <EFUSE_PMAP_READ8>
3000bd24:	7822      	ldrb	r2, [r4, #0]
3000bd26:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bd2a:	2a03      	cmp	r2, #3
3000bd2c:	d028      	beq.n	3000bd80 <SWR_Calib_MEM+0x12c>
3000bd2e:	2a02      	cmp	r2, #2
3000bd30:	d029      	beq.n	3000bd86 <SWR_Calib_MEM+0x132>
3000bd32:	2200      	movs	r2, #0
3000bd34:	4916      	ldr	r1, [pc, #88]	; (3000bd90 <SWR_Calib_MEM+0x13c>)
3000bd36:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000bd3a:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000bd3e:	4313      	orrs	r3, r2
3000bd40:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000bd44:	b002      	add	sp, #8
3000bd46:	bd10      	pop	{r4, pc}
3000bd48:	4b13      	ldr	r3, [pc, #76]	; (3000bd98 <SWR_Calib_MEM+0x144>)
3000bd4a:	2201      	movs	r2, #1
3000bd4c:	701a      	strb	r2, [r3, #0]
3000bd4e:	e7ca      	b.n	3000bce6 <SWR_Calib_MEM+0x92>
3000bd50:	2103      	movs	r1, #3
3000bd52:	e7a6      	b.n	3000bca2 <SWR_Calib_MEM+0x4e>
3000bd54:	2003      	movs	r0, #3
3000bd56:	e7db      	b.n	3000bd10 <SWR_Calib_MEM+0xbc>
3000bd58:	4b11      	ldr	r3, [pc, #68]	; (3000bda0 <SWR_Calib_MEM+0x14c>)
3000bd5a:	2201      	movs	r2, #1
3000bd5c:	701a      	strb	r2, [r3, #0]
3000bd5e:	b002      	add	sp, #8
3000bd60:	bd10      	pop	{r4, pc}
3000bd62:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000bd64:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000bd68:	d1bd      	bne.n	3000bce6 <SWR_Calib_MEM+0x92>
3000bd6a:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000bd6e:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000bd72:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000bd76:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000bd7a:	e7b4      	b.n	3000bce6 <SWR_Calib_MEM+0x92>
3000bd7c:	091b      	lsrs	r3, r3, #4
3000bd7e:	e7a4      	b.n	3000bcca <SWR_Calib_MEM+0x76>
3000bd80:	091b      	lsrs	r3, r3, #4
3000bd82:	059a      	lsls	r2, r3, #22
3000bd84:	e7d6      	b.n	3000bd34 <SWR_Calib_MEM+0xe0>
3000bd86:	f003 030f 	and.w	r3, r3, #15
3000bd8a:	059a      	lsls	r2, r3, #22
3000bd8c:	e7d2      	b.n	3000bd34 <SWR_Calib_MEM+0xe0>
3000bd8e:	bf00      	nop
3000bd90:	42008800 	.word	0x42008800
3000bd94:	fffffccc 	.word	0xfffffccc
3000bd98:	3000eac7 	.word	0x3000eac7
3000bd9c:	42008100 	.word	0x42008100
3000bda0:	3000eac6 	.word	0x3000eac6

3000bda4 <SWR_Calib_AUD>:
3000bda4:	4b44      	ldr	r3, [pc, #272]	; (3000beb8 <SWR_Calib_AUD+0x114>)
3000bda6:	f240 3231 	movw	r2, #817	; 0x331
3000bdaa:	4944      	ldr	r1, [pc, #272]	; (3000bebc <SWR_Calib_AUD+0x118>)
3000bdac:	2000      	movs	r0, #0
3000bdae:	b510      	push	{r4, lr}
3000bdb0:	f8d3 40a0 	ldr.w	r4, [r3, #160]	; 0xa0
3000bdb4:	b082      	sub	sp, #8
3000bdb6:	4021      	ands	r1, r4
3000bdb8:	430a      	orrs	r2, r1
3000bdba:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000bdbe:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000bdc2:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000bdc6:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bdca:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000bdce:	2307      	movs	r3, #7
3000bdd0:	f88d 0007 	strb.w	r0, [sp, #7]
3000bdd4:	eb0d 0203 	add.w	r2, sp, r3
3000bdd8:	f7fd fb32 	bl	30009440 <EFUSE_PMAP_READ8>
3000bddc:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bde0:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bde4:	2af0      	cmp	r2, #240	; 0xf0
3000bde6:	d155      	bne.n	3000be94 <SWR_Calib_AUD+0xf0>
3000bde8:	f003 030f 	and.w	r3, r3, #15
3000bdec:	2b0f      	cmp	r3, #15
3000bdee:	d04d      	beq.n	3000be8c <SWR_Calib_AUD+0xe8>
3000bdf0:	2102      	movs	r1, #2
3000bdf2:	4c33      	ldr	r4, [pc, #204]	; (3000bec0 <SWR_Calib_AUD+0x11c>)
3000bdf4:	2307      	movs	r3, #7
3000bdf6:	f10d 0206 	add.w	r2, sp, #6
3000bdfa:	2000      	movs	r0, #0
3000bdfc:	7021      	strb	r1, [r4, #0]
3000bdfe:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000be02:	f7fd fb1d 	bl	30009440 <EFUSE_PMAP_READ8>
3000be06:	7822      	ldrb	r2, [r4, #0]
3000be08:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000be0c:	2a03      	cmp	r2, #3
3000be0e:	d04a      	beq.n	3000bea6 <SWR_Calib_AUD+0x102>
3000be10:	2a02      	cmp	r2, #2
3000be12:	bf0c      	ite	eq
3000be14:	f003 030f 	andeq.w	r3, r3, #15
3000be18:	2300      	movne	r3, #0
3000be1a:	4927      	ldr	r1, [pc, #156]	; (3000beb8 <SWR_Calib_AUD+0x114>)
3000be1c:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000be20:	f022 020f 	bic.w	r2, r2, #15
3000be24:	4313      	orrs	r3, r2
3000be26:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000be2a:	2307      	movs	r3, #7
3000be2c:	2000      	movs	r0, #0
3000be2e:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000be32:	eb0d 0203 	add.w	r2, sp, r3
3000be36:	f88d 0007 	strb.w	r0, [sp, #7]
3000be3a:	f7fd fb01 	bl	30009440 <EFUSE_PMAP_READ8>
3000be3e:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000be42:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000be46:	2af0      	cmp	r2, #240	; 0xf0
3000be48:	d126      	bne.n	3000be98 <SWR_Calib_AUD+0xf4>
3000be4a:	f003 030f 	and.w	r3, r3, #15
3000be4e:	2b0f      	cmp	r3, #15
3000be50:	d024      	beq.n	3000be9c <SWR_Calib_AUD+0xf8>
3000be52:	2002      	movs	r0, #2
3000be54:	2307      	movs	r3, #7
3000be56:	4c1b      	ldr	r4, [pc, #108]	; (3000bec4 <SWR_Calib_AUD+0x120>)
3000be58:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000be5c:	eb0d 0203 	add.w	r2, sp, r3
3000be60:	7020      	strb	r0, [r4, #0]
3000be62:	2000      	movs	r0, #0
3000be64:	f7fd faec 	bl	30009440 <EFUSE_PMAP_READ8>
3000be68:	7822      	ldrb	r2, [r4, #0]
3000be6a:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000be6e:	2a03      	cmp	r2, #3
3000be70:	d01b      	beq.n	3000beaa <SWR_Calib_AUD+0x106>
3000be72:	2a02      	cmp	r2, #2
3000be74:	d01c      	beq.n	3000beb0 <SWR_Calib_AUD+0x10c>
3000be76:	2200      	movs	r2, #0
3000be78:	490f      	ldr	r1, [pc, #60]	; (3000beb8 <SWR_Calib_AUD+0x114>)
3000be7a:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000be7e:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000be82:	4313      	orrs	r3, r2
3000be84:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000be88:	b002      	add	sp, #8
3000be8a:	bd10      	pop	{r4, pc}
3000be8c:	4b0c      	ldr	r3, [pc, #48]	; (3000bec0 <SWR_Calib_AUD+0x11c>)
3000be8e:	2201      	movs	r2, #1
3000be90:	701a      	strb	r2, [r3, #0]
3000be92:	e7ca      	b.n	3000be2a <SWR_Calib_AUD+0x86>
3000be94:	2103      	movs	r1, #3
3000be96:	e7ac      	b.n	3000bdf2 <SWR_Calib_AUD+0x4e>
3000be98:	2003      	movs	r0, #3
3000be9a:	e7db      	b.n	3000be54 <SWR_Calib_AUD+0xb0>
3000be9c:	4b09      	ldr	r3, [pc, #36]	; (3000bec4 <SWR_Calib_AUD+0x120>)
3000be9e:	2201      	movs	r2, #1
3000bea0:	701a      	strb	r2, [r3, #0]
3000bea2:	b002      	add	sp, #8
3000bea4:	bd10      	pop	{r4, pc}
3000bea6:	091b      	lsrs	r3, r3, #4
3000bea8:	e7b7      	b.n	3000be1a <SWR_Calib_AUD+0x76>
3000beaa:	091b      	lsrs	r3, r3, #4
3000beac:	059a      	lsls	r2, r3, #22
3000beae:	e7e3      	b.n	3000be78 <SWR_Calib_AUD+0xd4>
3000beb0:	f003 030f 	and.w	r3, r3, #15
3000beb4:	059a      	lsls	r2, r3, #22
3000beb6:	e7df      	b.n	3000be78 <SWR_Calib_AUD+0xd4>
3000beb8:	42008800 	.word	0x42008800
3000bebc:	fffffccc 	.word	0xfffffccc
3000bec0:	3000eac5 	.word	0x3000eac5
3000bec4:	3000eac4 	.word	0x3000eac4

3000bec8 <__aeabi_uldivmod>:
3000bec8:	b953      	cbnz	r3, 3000bee0 <__aeabi_uldivmod+0x18>
3000beca:	b94a      	cbnz	r2, 3000bee0 <__aeabi_uldivmod+0x18>
3000becc:	2900      	cmp	r1, #0
3000bece:	bf08      	it	eq
3000bed0:	2800      	cmpeq	r0, #0
3000bed2:	bf1c      	itt	ne
3000bed4:	f04f 31ff 	movne.w	r1, #4294967295
3000bed8:	f04f 30ff 	movne.w	r0, #4294967295
3000bedc:	f000 b960 	b.w	3000c1a0 <__aeabi_idiv0>
3000bee0:	f1ad 0c08 	sub.w	ip, sp, #8
3000bee4:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000bee8:	f000 f806 	bl	3000bef8 <__udivmoddi4>
3000beec:	f8dd e004 	ldr.w	lr, [sp, #4]
3000bef0:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000bef4:	b004      	add	sp, #16
3000bef6:	4770      	bx	lr

3000bef8 <__udivmoddi4>:
3000bef8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000befc:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000befe:	4688      	mov	r8, r1
3000bf00:	468e      	mov	lr, r1
3000bf02:	4615      	mov	r5, r2
3000bf04:	4604      	mov	r4, r0
3000bf06:	4619      	mov	r1, r3
3000bf08:	2b00      	cmp	r3, #0
3000bf0a:	f040 80c6 	bne.w	3000c09a <__udivmoddi4+0x1a2>
3000bf0e:	4542      	cmp	r2, r8
3000bf10:	fab2 f782 	clz	r7, r2
3000bf14:	d946      	bls.n	3000bfa4 <__udivmoddi4+0xac>
3000bf16:	b14f      	cbz	r7, 3000bf2c <__udivmoddi4+0x34>
3000bf18:	f1c7 0c20 	rsb	ip, r7, #32
3000bf1c:	fa08 f307 	lsl.w	r3, r8, r7
3000bf20:	40bd      	lsls	r5, r7
3000bf22:	40bc      	lsls	r4, r7
3000bf24:	fa20 fc0c 	lsr.w	ip, r0, ip
3000bf28:	ea4c 0e03 	orr.w	lr, ip, r3
3000bf2c:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000bf30:	fa1f fc85 	uxth.w	ip, r5
3000bf34:	0c22      	lsrs	r2, r4, #16
3000bf36:	fbbe f9f8 	udiv	r9, lr, r8
3000bf3a:	fb08 e319 	mls	r3, r8, r9, lr
3000bf3e:	fb09 fa0c 	mul.w	sl, r9, ip
3000bf42:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000bf46:	459a      	cmp	sl, r3
3000bf48:	d928      	bls.n	3000bf9c <__udivmoddi4+0xa4>
3000bf4a:	18eb      	adds	r3, r5, r3
3000bf4c:	f109 30ff 	add.w	r0, r9, #4294967295
3000bf50:	d204      	bcs.n	3000bf5c <__udivmoddi4+0x64>
3000bf52:	459a      	cmp	sl, r3
3000bf54:	d902      	bls.n	3000bf5c <__udivmoddi4+0x64>
3000bf56:	f1a9 0002 	sub.w	r0, r9, #2
3000bf5a:	442b      	add	r3, r5
3000bf5c:	eba3 030a 	sub.w	r3, r3, sl
3000bf60:	b2a4      	uxth	r4, r4
3000bf62:	fbb3 f2f8 	udiv	r2, r3, r8
3000bf66:	fb08 3312 	mls	r3, r8, r2, r3
3000bf6a:	fb02 fc0c 	mul.w	ip, r2, ip
3000bf6e:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bf72:	45a4      	cmp	ip, r4
3000bf74:	d914      	bls.n	3000bfa0 <__udivmoddi4+0xa8>
3000bf76:	192c      	adds	r4, r5, r4
3000bf78:	f102 33ff 	add.w	r3, r2, #4294967295
3000bf7c:	d203      	bcs.n	3000bf86 <__udivmoddi4+0x8e>
3000bf7e:	45a4      	cmp	ip, r4
3000bf80:	d901      	bls.n	3000bf86 <__udivmoddi4+0x8e>
3000bf82:	1e93      	subs	r3, r2, #2
3000bf84:	442c      	add	r4, r5
3000bf86:	eba4 040c 	sub.w	r4, r4, ip
3000bf8a:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000bf8e:	b11e      	cbz	r6, 3000bf98 <__udivmoddi4+0xa0>
3000bf90:	40fc      	lsrs	r4, r7
3000bf92:	2300      	movs	r3, #0
3000bf94:	6034      	str	r4, [r6, #0]
3000bf96:	6073      	str	r3, [r6, #4]
3000bf98:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bf9c:	4648      	mov	r0, r9
3000bf9e:	e7dd      	b.n	3000bf5c <__udivmoddi4+0x64>
3000bfa0:	4613      	mov	r3, r2
3000bfa2:	e7f0      	b.n	3000bf86 <__udivmoddi4+0x8e>
3000bfa4:	b902      	cbnz	r2, 3000bfa8 <__udivmoddi4+0xb0>
3000bfa6:	deff      	udf	#255	; 0xff
3000bfa8:	bb8f      	cbnz	r7, 3000c00e <__udivmoddi4+0x116>
3000bfaa:	eba8 0302 	sub.w	r3, r8, r2
3000bfae:	2101      	movs	r1, #1
3000bfb0:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000bfb4:	b2aa      	uxth	r2, r5
3000bfb6:	0c20      	lsrs	r0, r4, #16
3000bfb8:	fbb3 fcfe 	udiv	ip, r3, lr
3000bfbc:	fb0e 331c 	mls	r3, lr, ip, r3
3000bfc0:	fb0c f802 	mul.w	r8, ip, r2
3000bfc4:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000bfc8:	4598      	cmp	r8, r3
3000bfca:	d962      	bls.n	3000c092 <__udivmoddi4+0x19a>
3000bfcc:	18eb      	adds	r3, r5, r3
3000bfce:	f10c 30ff 	add.w	r0, ip, #4294967295
3000bfd2:	d204      	bcs.n	3000bfde <__udivmoddi4+0xe6>
3000bfd4:	4598      	cmp	r8, r3
3000bfd6:	d902      	bls.n	3000bfde <__udivmoddi4+0xe6>
3000bfd8:	f1ac 0002 	sub.w	r0, ip, #2
3000bfdc:	442b      	add	r3, r5
3000bfde:	eba3 0308 	sub.w	r3, r3, r8
3000bfe2:	b2a4      	uxth	r4, r4
3000bfe4:	fbb3 fcfe 	udiv	ip, r3, lr
3000bfe8:	fb0e 331c 	mls	r3, lr, ip, r3
3000bfec:	fb0c f202 	mul.w	r2, ip, r2
3000bff0:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bff4:	42a2      	cmp	r2, r4
3000bff6:	d94e      	bls.n	3000c096 <__udivmoddi4+0x19e>
3000bff8:	192c      	adds	r4, r5, r4
3000bffa:	f10c 33ff 	add.w	r3, ip, #4294967295
3000bffe:	d204      	bcs.n	3000c00a <__udivmoddi4+0x112>
3000c000:	42a2      	cmp	r2, r4
3000c002:	d902      	bls.n	3000c00a <__udivmoddi4+0x112>
3000c004:	f1ac 0302 	sub.w	r3, ip, #2
3000c008:	442c      	add	r4, r5
3000c00a:	1aa4      	subs	r4, r4, r2
3000c00c:	e7bd      	b.n	3000bf8a <__udivmoddi4+0x92>
3000c00e:	f1c7 0c20 	rsb	ip, r7, #32
3000c012:	40bd      	lsls	r5, r7
3000c014:	fa08 f307 	lsl.w	r3, r8, r7
3000c018:	40bc      	lsls	r4, r7
3000c01a:	fa28 f90c 	lsr.w	r9, r8, ip
3000c01e:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c022:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c026:	ea4c 0203 	orr.w	r2, ip, r3
3000c02a:	b2ab      	uxth	r3, r5
3000c02c:	fbb9 fcfe 	udiv	ip, r9, lr
3000c030:	0c11      	lsrs	r1, r2, #16
3000c032:	fb0e 901c 	mls	r0, lr, ip, r9
3000c036:	fb0c f803 	mul.w	r8, ip, r3
3000c03a:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c03e:	4580      	cmp	r8, r0
3000c040:	d923      	bls.n	3000c08a <__udivmoddi4+0x192>
3000c042:	1828      	adds	r0, r5, r0
3000c044:	f10c 31ff 	add.w	r1, ip, #4294967295
3000c048:	d204      	bcs.n	3000c054 <__udivmoddi4+0x15c>
3000c04a:	4580      	cmp	r8, r0
3000c04c:	d902      	bls.n	3000c054 <__udivmoddi4+0x15c>
3000c04e:	f1ac 0102 	sub.w	r1, ip, #2
3000c052:	4428      	add	r0, r5
3000c054:	eba0 0008 	sub.w	r0, r0, r8
3000c058:	b292      	uxth	r2, r2
3000c05a:	fbb0 fcfe 	udiv	ip, r0, lr
3000c05e:	fb0e 001c 	mls	r0, lr, ip, r0
3000c062:	fb0c f803 	mul.w	r8, ip, r3
3000c066:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c06a:	4598      	cmp	r8, r3
3000c06c:	d90f      	bls.n	3000c08e <__udivmoddi4+0x196>
3000c06e:	18eb      	adds	r3, r5, r3
3000c070:	f10c 32ff 	add.w	r2, ip, #4294967295
3000c074:	d204      	bcs.n	3000c080 <__udivmoddi4+0x188>
3000c076:	4598      	cmp	r8, r3
3000c078:	d902      	bls.n	3000c080 <__udivmoddi4+0x188>
3000c07a:	f1ac 0202 	sub.w	r2, ip, #2
3000c07e:	442b      	add	r3, r5
3000c080:	eba3 0308 	sub.w	r3, r3, r8
3000c084:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c088:	e792      	b.n	3000bfb0 <__udivmoddi4+0xb8>
3000c08a:	4661      	mov	r1, ip
3000c08c:	e7e2      	b.n	3000c054 <__udivmoddi4+0x15c>
3000c08e:	4662      	mov	r2, ip
3000c090:	e7f6      	b.n	3000c080 <__udivmoddi4+0x188>
3000c092:	4660      	mov	r0, ip
3000c094:	e7a3      	b.n	3000bfde <__udivmoddi4+0xe6>
3000c096:	4663      	mov	r3, ip
3000c098:	e7b7      	b.n	3000c00a <__udivmoddi4+0x112>
3000c09a:	4543      	cmp	r3, r8
3000c09c:	d905      	bls.n	3000c0aa <__udivmoddi4+0x1b2>
3000c09e:	b10e      	cbz	r6, 3000c0a4 <__udivmoddi4+0x1ac>
3000c0a0:	e9c6 0800 	strd	r0, r8, [r6]
3000c0a4:	2100      	movs	r1, #0
3000c0a6:	4608      	mov	r0, r1
3000c0a8:	e776      	b.n	3000bf98 <__udivmoddi4+0xa0>
3000c0aa:	fab3 fc83 	clz	ip, r3
3000c0ae:	f1bc 0f00 	cmp.w	ip, #0
3000c0b2:	d110      	bne.n	3000c0d6 <__udivmoddi4+0x1de>
3000c0b4:	4543      	cmp	r3, r8
3000c0b6:	d301      	bcc.n	3000c0bc <__udivmoddi4+0x1c4>
3000c0b8:	4282      	cmp	r2, r0
3000c0ba:	d80a      	bhi.n	3000c0d2 <__udivmoddi4+0x1da>
3000c0bc:	1a84      	subs	r4, r0, r2
3000c0be:	eb68 0303 	sbc.w	r3, r8, r3
3000c0c2:	2001      	movs	r0, #1
3000c0c4:	469e      	mov	lr, r3
3000c0c6:	2e00      	cmp	r6, #0
3000c0c8:	d068      	beq.n	3000c19c <__udivmoddi4+0x2a4>
3000c0ca:	e9c6 4e00 	strd	r4, lr, [r6]
3000c0ce:	2100      	movs	r1, #0
3000c0d0:	e762      	b.n	3000bf98 <__udivmoddi4+0xa0>
3000c0d2:	4660      	mov	r0, ip
3000c0d4:	e7f7      	b.n	3000c0c6 <__udivmoddi4+0x1ce>
3000c0d6:	f1cc 0e20 	rsb	lr, ip, #32
3000c0da:	fa03 f30c 	lsl.w	r3, r3, ip
3000c0de:	fa02 f50c 	lsl.w	r5, r2, ip
3000c0e2:	fa00 f70c 	lsl.w	r7, r0, ip
3000c0e6:	fa22 f40e 	lsr.w	r4, r2, lr
3000c0ea:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c0ee:	fa20 f10e 	lsr.w	r1, r0, lr
3000c0f2:	431c      	orrs	r4, r3
3000c0f4:	fa08 f30c 	lsl.w	r3, r8, ip
3000c0f8:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c0fc:	4319      	orrs	r1, r3
3000c0fe:	b2a3      	uxth	r3, r4
3000c100:	0c0a      	lsrs	r2, r1, #16
3000c102:	fbbb f8f9 	udiv	r8, fp, r9
3000c106:	fb09 bb18 	mls	fp, r9, r8, fp
3000c10a:	fb08 fa03 	mul.w	sl, r8, r3
3000c10e:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c112:	4592      	cmp	sl, r2
3000c114:	d93e      	bls.n	3000c194 <__udivmoddi4+0x29c>
3000c116:	18a2      	adds	r2, r4, r2
3000c118:	f108 30ff 	add.w	r0, r8, #4294967295
3000c11c:	d204      	bcs.n	3000c128 <__udivmoddi4+0x230>
3000c11e:	4592      	cmp	sl, r2
3000c120:	d902      	bls.n	3000c128 <__udivmoddi4+0x230>
3000c122:	f1a8 0002 	sub.w	r0, r8, #2
3000c126:	4422      	add	r2, r4
3000c128:	eba2 020a 	sub.w	r2, r2, sl
3000c12c:	b289      	uxth	r1, r1
3000c12e:	fbb2 f8f9 	udiv	r8, r2, r9
3000c132:	fb09 2218 	mls	r2, r9, r8, r2
3000c136:	fb08 f303 	mul.w	r3, r8, r3
3000c13a:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c13e:	4293      	cmp	r3, r2
3000c140:	d92a      	bls.n	3000c198 <__udivmoddi4+0x2a0>
3000c142:	18a2      	adds	r2, r4, r2
3000c144:	f108 31ff 	add.w	r1, r8, #4294967295
3000c148:	d204      	bcs.n	3000c154 <__udivmoddi4+0x25c>
3000c14a:	4293      	cmp	r3, r2
3000c14c:	d902      	bls.n	3000c154 <__udivmoddi4+0x25c>
3000c14e:	f1a8 0102 	sub.w	r1, r8, #2
3000c152:	4422      	add	r2, r4
3000c154:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c158:	1ad2      	subs	r2, r2, r3
3000c15a:	fba0 9805 	umull	r9, r8, r0, r5
3000c15e:	4542      	cmp	r2, r8
3000c160:	464b      	mov	r3, r9
3000c162:	4641      	mov	r1, r8
3000c164:	d302      	bcc.n	3000c16c <__udivmoddi4+0x274>
3000c166:	d106      	bne.n	3000c176 <__udivmoddi4+0x27e>
3000c168:	454f      	cmp	r7, r9
3000c16a:	d204      	bcs.n	3000c176 <__udivmoddi4+0x27e>
3000c16c:	3801      	subs	r0, #1
3000c16e:	ebb9 0305 	subs.w	r3, r9, r5
3000c172:	eb68 0104 	sbc.w	r1, r8, r4
3000c176:	b18e      	cbz	r6, 3000c19c <__udivmoddi4+0x2a4>
3000c178:	1afc      	subs	r4, r7, r3
3000c17a:	eb62 0701 	sbc.w	r7, r2, r1
3000c17e:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c182:	fa24 f30c 	lsr.w	r3, r4, ip
3000c186:	fa27 f70c 	lsr.w	r7, r7, ip
3000c18a:	ea4e 0303 	orr.w	r3, lr, r3
3000c18e:	e9c6 3700 	strd	r3, r7, [r6]
3000c192:	e79c      	b.n	3000c0ce <__udivmoddi4+0x1d6>
3000c194:	4640      	mov	r0, r8
3000c196:	e7c7      	b.n	3000c128 <__udivmoddi4+0x230>
3000c198:	4641      	mov	r1, r8
3000c19a:	e7db      	b.n	3000c154 <__udivmoddi4+0x25c>
3000c19c:	4631      	mov	r1, r6
3000c19e:	e6fb      	b.n	3000bf98 <__udivmoddi4+0xa0>

3000c1a0 <__aeabi_idiv0>:
3000c1a0:	4770      	bx	lr
3000c1a2:	bf00      	nop
3000c1a4:	0000      	movs	r0, r0
	...

3000c1a8 <____wrap_strlen_veneer>:
3000c1a8:	b401      	push	{r0}
3000c1aa:	4802      	ldr	r0, [pc, #8]	; (3000c1b4 <____wrap_strlen_veneer+0xc>)
3000c1ac:	4684      	mov	ip, r0
3000c1ae:	bc01      	pop	{r0}
3000c1b0:	4760      	bx	ip
3000c1b2:	bf00      	nop
3000c1b4:	00012f25 	.word	0x00012f25

3000c1b8 <__SBOOT_GetMdType_veneer>:
3000c1b8:	b401      	push	{r0}
3000c1ba:	4802      	ldr	r0, [pc, #8]	; (3000c1c4 <__SBOOT_GetMdType_veneer+0xc>)
3000c1bc:	4684      	mov	ip, r0
3000c1be:	bc01      	pop	{r0}
3000c1c0:	4760      	bx	ip
3000c1c2:	bf00      	nop
3000c1c4:	0000319d 	.word	0x0000319d

3000c1c8 <__SBOOT_Validate_Algorithm_veneer>:
3000c1c8:	b401      	push	{r0}
3000c1ca:	4802      	ldr	r0, [pc, #8]	; (3000c1d4 <__SBOOT_Validate_Algorithm_veneer+0xc>)
3000c1cc:	4684      	mov	ip, r0
3000c1ce:	bc01      	pop	{r0}
3000c1d0:	4760      	bx	ip
3000c1d2:	bf00      	nop
3000c1d4:	000033ed 	.word	0x000033ed

3000c1d8 <__SBOOT_Validate_Signature_veneer>:
3000c1d8:	b401      	push	{r0}
3000c1da:	4802      	ldr	r0, [pc, #8]	; (3000c1e4 <__SBOOT_Validate_Signature_veneer+0xc>)
3000c1dc:	4684      	mov	ip, r0
3000c1de:	bc01      	pop	{r0}
3000c1e0:	4760      	bx	ip
3000c1e2:	bf00      	nop
3000c1e4:	00003431 	.word	0x00003431

3000c1e8 <____wrap_memset_veneer>:
3000c1e8:	b401      	push	{r0}
3000c1ea:	4802      	ldr	r0, [pc, #8]	; (3000c1f4 <____wrap_memset_veneer+0xc>)
3000c1ec:	4684      	mov	ip, r0
3000c1ee:	bc01      	pop	{r0}
3000c1f0:	4760      	bx	ip
3000c1f2:	bf00      	nop
3000c1f4:	00012efd 	.word	0x00012efd

3000c1f8 <____wrap_memcpy_veneer>:
3000c1f8:	b401      	push	{r0}
3000c1fa:	4802      	ldr	r0, [pc, #8]	; (3000c204 <____wrap_memcpy_veneer+0xc>)
3000c1fc:	4684      	mov	ip, r0
3000c1fe:	bc01      	pop	{r0}
3000c200:	4760      	bx	ip
3000c202:	bf00      	nop
3000c204:	00012f05 	.word	0x00012f05

3000c208 <__io_assert_failed_veneer>:
3000c208:	b401      	push	{r0}
3000c20a:	4802      	ldr	r0, [pc, #8]	; (3000c214 <__io_assert_failed_veneer+0xc>)
3000c20c:	4684      	mov	ip, r0
3000c20e:	bc01      	pop	{r0}
3000c210:	4760      	bx	ip
3000c212:	bf00      	nop
3000c214:	0000992d 	.word	0x0000992d

3000c218 <__SBOOT_Validate_ImgHash_veneer>:
3000c218:	b401      	push	{r0}
3000c21a:	4802      	ldr	r0, [pc, #8]	; (3000c224 <__SBOOT_Validate_ImgHash_veneer+0xc>)
3000c21c:	4684      	mov	ip, r0
3000c21e:	bc01      	pop	{r0}
3000c220:	4760      	bx	ip
3000c222:	bf00      	nop
3000c224:	00003541 	.word	0x00003541

3000c228 <__DiagPrintf_veneer>:
3000c228:	b401      	push	{r0}
3000c22a:	4802      	ldr	r0, [pc, #8]	; (3000c234 <__DiagPrintf_veneer+0xc>)
3000c22c:	4684      	mov	ip, r0
3000c22e:	bc01      	pop	{r0}
3000c230:	4760      	bx	ip
3000c232:	bf00      	nop
3000c234:	0000e5f9 	.word	0x0000e5f9

3000c238 <__SBOOT_Validate_PubKey_veneer>:
3000c238:	b401      	push	{r0}
3000c23a:	4802      	ldr	r0, [pc, #8]	; (3000c244 <__SBOOT_Validate_PubKey_veneer+0xc>)
3000c23c:	4684      	mov	ip, r0
3000c23e:	bc01      	pop	{r0}
3000c240:	4760      	bx	ip
3000c242:	bf00      	nop
3000c244:	00003495 	.word	0x00003495

3000c248 <____wrap_memcmp_veneer>:
3000c248:	b401      	push	{r0}
3000c24a:	4802      	ldr	r0, [pc, #8]	; (3000c254 <____wrap_memcmp_veneer+0xc>)
3000c24c:	4684      	mov	ip, r0
3000c24e:	bc01      	pop	{r0}
3000c250:	4760      	bx	ip
3000c252:	bf00      	nop
3000c254:	00012f0d 	.word	0x00012f0d
3000c258:	004f4931 	.word	0x004f4931
3000c25c:	00004f32 	.word	0x00004f32
3000c260:	004f4932 	.word	0x004f4932
3000c264:	00004f34 	.word	0x00004f34
3000c268:	004f4934 	.word	0x004f4934
3000c26c:	414c460d 	.word	0x414c460d
3000c270:	435f4853 	.word	0x435f4853
3000c274:	62696c61 	.word	0x62696c61
3000c278:	69746172 	.word	0x69746172
3000c27c:	654e6e6f 	.word	0x654e6e6f
3000c280:	78252077 	.word	0x78252077
3000c284:	2078253a 	.word	0x2078253a
3000c288:	73616870 	.word	0x73616870
3000c28c:	68735f65 	.word	0x68735f65
3000c290:	3a746669 	.word	0x3a746669
3000c294:	000a7825 	.word	0x000a7825
3000c298:	414c460d 	.word	0x414c460d
3000c29c:	435f4853 	.word	0x435f4853
3000c2a0:	62696c61 	.word	0x62696c61
3000c2a4:	69746172 	.word	0x69746172
3000c2a8:	654e6e6f 	.word	0x654e6e6f
3000c2ac:	694c2077 	.word	0x694c2077
3000c2b0:	6544656e 	.word	0x6544656e
3000c2b4:	3a79616c 	.word	0x3a79616c
3000c2b8:	000a7825 	.word	0x000a7825
3000c2bc:	414c460d 	.word	0x414c460d
3000c2c0:	435f4853 	.word	0x435f4853
3000c2c4:	62696c61 	.word	0x62696c61
3000c2c8:	69746172 	.word	0x69746172
3000c2cc:	654e6e6f 	.word	0x654e6e6f
3000c2d0:	69772077 	.word	0x69772077
3000c2d4:	776f646e 	.word	0x776f646e
3000c2d8:	74735f31 	.word	0x74735f31
3000c2dc:	3a747261 	.word	0x3a747261
3000c2e0:	77206425 	.word	0x77206425
3000c2e4:	6f646e69 	.word	0x6f646e69
3000c2e8:	735f3177 	.word	0x735f3177
3000c2ec:	3a657a69 	.word	0x3a657a69
3000c2f0:	0a206425 	.word	0x0a206425
3000c2f4:	00000000 	.word	0x00000000
3000c2f8:	414c460d 	.word	0x414c460d
3000c2fc:	435f4853 	.word	0x435f4853
3000c300:	62696c61 	.word	0x62696c61
3000c304:	69746172 	.word	0x69746172
3000c308:	654e6e6f 	.word	0x654e6e6f
3000c30c:	68702077 	.word	0x68702077
3000c310:	5f657361 	.word	0x5f657361
3000c314:	66696873 	.word	0x66696873
3000c318:	64695f74 	.word	0x64695f74
3000c31c:	64253a78 	.word	0x64253a78
3000c320:	00000a20 	.word	0x00000a20
3000c324:	414c460d 	.word	0x414c460d
3000c328:	435f4853 	.word	0x435f4853
3000c32c:	62696c61 	.word	0x62696c61
3000c330:	69746172 	.word	0x69746172
3000c334:	25206e6f 	.word	0x25206e6f
3000c338:	61732078 	.word	0x61732078
3000c33c:	656c706d 	.word	0x656c706d
3000c340:	6168705f 	.word	0x6168705f
3000c344:	253a6573 	.word	0x253a6573
3000c348:	61722078 	.word	0x61722078
3000c34c:	253a6574 	.word	0x253a6574
3000c350:	00000a78 	.word	0x00000a78
3000c354:	414c460d 	.word	0x414c460d
3000c358:	435f4853 	.word	0x435f4853
3000c35c:	62696c61 	.word	0x62696c61
3000c360:	69746172 	.word	0x69746172
3000c364:	73206e6f 	.word	0x73206e6f
3000c368:	6c706d61 	.word	0x6c706d61
3000c36c:	68705f65 	.word	0x68705f65
3000c370:	3a657361 	.word	0x3a657361
3000c374:	72207825 	.word	0x72207825
3000c378:	3a657461 	.word	0x3a657461
3000c37c:	000a7825 	.word	0x000a7825
3000c380:	444f4d5b 	.word	0x444f4d5b
3000c384:	5f454c55 	.word	0x5f454c55
3000c388:	544f4f42 	.word	0x544f4f42
3000c38c:	56454c2d 	.word	0x56454c2d
3000c390:	495f4c45 	.word	0x495f4c45
3000c394:	5d4f464e 	.word	0x5d4f464e
3000c398:	6c61633a 	.word	0x6c61633a
3000c39c:	61726269 	.word	0x61726269
3000c3a0:	6e6f6974 	.word	0x6e6f6974
3000c3a4:	3a6b6f5f 	.word	0x3a6b6f5f
3000c3a8:	3a64255b 	.word	0x3a64255b
3000c3ac:	253a6425 	.word	0x253a6425
3000c3b0:	0a205d64 	.word	0x0a205d64
3000c3b4:	00000000 	.word	0x00000000
3000c3b8:	444f4d5b 	.word	0x444f4d5b
3000c3bc:	5f454c55 	.word	0x5f454c55
3000c3c0:	544f4f42 	.word	0x544f4f42
3000c3c4:	56454c2d 	.word	0x56454c2d
3000c3c8:	495f4c45 	.word	0x495f4c45
3000c3cc:	5d4f464e 	.word	0x5d4f464e
3000c3d0:	414c463a 	.word	0x414c463a
3000c3d4:	43204853 	.word	0x43204853
3000c3d8:	42494c41 	.word	0x42494c41
3000c3dc:	2578305b 	.word	0x2578305b
3000c3e0:	4b4f2078 	.word	0x4b4f2078
3000c3e4:	00000a5d 	.word	0x00000a5d
3000c3e8:	444f4d5b 	.word	0x444f4d5b
3000c3ec:	5f454c55 	.word	0x5f454c55
3000c3f0:	544f4f42 	.word	0x544f4f42
3000c3f4:	56454c2d 	.word	0x56454c2d
3000c3f8:	495f4c45 	.word	0x495f4c45
3000c3fc:	5d4f464e 	.word	0x5d4f464e
3000c400:	414c463a 	.word	0x414c463a
3000c404:	43204853 	.word	0x43204853
3000c408:	42494c41 	.word	0x42494c41
3000c40c:	2578305b 	.word	0x2578305b
3000c410:	41462078 	.word	0x41462078
3000c414:	0a5d4c49 	.word	0x0a5d4c49
3000c418:	00000000 	.word	0x00000000
3000c41c:	444f4d5b 	.word	0x444f4d5b
3000c420:	5f454c55 	.word	0x5f454c55
3000c424:	544f4f42 	.word	0x544f4f42
3000c428:	56454c2d 	.word	0x56454c2d
3000c42c:	495f4c45 	.word	0x495f4c45
3000c430:	5d4f464e 	.word	0x5d4f464e
3000c434:	616c463a 	.word	0x616c463a
3000c438:	52206873 	.word	0x52206873
3000c43c:	20646165 	.word	0x20646165
3000c440:	000a7325 	.word	0x000a7325
3000c444:	444f4d5b 	.word	0x444f4d5b
3000c448:	5f454c55 	.word	0x5f454c55
3000c44c:	544f4f42 	.word	0x544f4f42
3000c450:	56454c2d 	.word	0x56454c2d
3000c454:	495f4c45 	.word	0x495f4c45
3000c458:	5d4f464e 	.word	0x5d4f464e
3000c45c:	616c463a 	.word	0x616c463a
3000c460:	52206873 	.word	0x52206873
3000c464:	20646165 	.word	0x20646165
3000c468:	202c7325 	.word	0x202c7325
3000c46c:	4c494146 	.word	0x4c494146
3000c470:	0000000a 	.word	0x0000000a
3000c474:	444f4d5b 	.word	0x444f4d5b
3000c478:	5f454c55 	.word	0x5f454c55
3000c47c:	544f4f42 	.word	0x544f4f42
3000c480:	56454c2d 	.word	0x56454c2d
3000c484:	455f4c45 	.word	0x455f4c45
3000c488:	524f5252 	.word	0x524f5252
3000c48c:	6c463a5d 	.word	0x6c463a5d
3000c490:	20687361 	.word	0x20687361
3000c494:	74697753 	.word	0x74697753
3000c498:	52206863 	.word	0x52206863
3000c49c:	20646165 	.word	0x20646165
3000c4a0:	65646f4d 	.word	0x65646f4d
3000c4a4:	49414620 	.word	0x49414620
3000c4a8:	00000a4c 	.word	0x00000a4c
3000c4ac:	444f4d5b 	.word	0x444f4d5b
3000c4b0:	5f454c55 	.word	0x5f454c55
3000c4b4:	544f4f42 	.word	0x544f4f42
3000c4b8:	56454c2d 	.word	0x56454c2d
3000c4bc:	495f4c45 	.word	0x495f4c45
3000c4c0:	5d4f464e 	.word	0x5d4f464e
3000c4c4:	616c463a 	.word	0x616c463a
3000c4c8:	49206873 	.word	0x49206873
3000c4cc:	25203a44 	.word	0x25203a44
3000c4d0:	78252d78 	.word	0x78252d78
3000c4d4:	0a78252d 	.word	0x0a78252d
3000c4d8:	00000000 	.word	0x00000000
3000c4dc:	6968540d 	.word	0x6968540d
3000c4e0:	6c662073 	.word	0x6c662073
3000c4e4:	20687361 	.word	0x20687361
3000c4e8:	65707974 	.word	0x65707974
3000c4ec:	20736920 	.word	0x20736920
3000c4f0:	20746f6e 	.word	0x20746f6e
3000c4f4:	70707573 	.word	0x70707573
3000c4f8:	6574726f 	.word	0x6574726f
3000c4fc:	000a2164 	.word	0x000a2164
3000c500:	444f4d5b 	.word	0x444f4d5b
3000c504:	5f454c55 	.word	0x5f454c55
3000c508:	544f4f42 	.word	0x544f4f42
3000c50c:	56454c2d 	.word	0x56454c2d
3000c510:	495f4c45 	.word	0x495f4c45
3000c514:	5d4f464e 	.word	0x5d4f464e
3000c518:	4e414e3a 	.word	0x4e414e3a
3000c51c:	44492044 	.word	0x44492044
3000c520:	7825203a 	.word	0x7825203a
3000c524:	0a78252d 	.word	0x0a78252d
3000c528:	00000000 	.word	0x00000000
3000c52c:	20304d4b 	.word	0x20304d4b
3000c530:	20504958 	.word	0x20504958
3000c534:	00474d49 	.word	0x00474d49
3000c538:	20304d4b 	.word	0x20304d4b
3000c53c:	4d415253 	.word	0x4d415253
3000c540:	00000000 	.word	0x00000000
3000c544:	20304d4b 	.word	0x20304d4b
3000c548:	4d415244 	.word	0x4d415244
3000c54c:	00000000 	.word	0x00000000
3000c550:	20344d4b 	.word	0x20344d4b
3000c554:	20504958 	.word	0x20504958
3000c558:	00474d49 	.word	0x00474d49
3000c55c:	20344d4b 	.word	0x20344d4b
3000c560:	4d415253 	.word	0x4d415253
3000c564:	00000000 	.word	0x00000000
3000c568:	20344d4b 	.word	0x20344d4b
3000c56c:	4d415244 	.word	0x4d415244
3000c570:	00000000 	.word	0x00000000
3000c574:	42205041 	.word	0x42205041
3000c578:	5320314c 	.word	0x5320314c
3000c57c:	004d4152 	.word	0x004d4152
3000c580:	42205041 	.word	0x42205041
3000c584:	4420314c 	.word	0x4420314c
3000c588:	004d4152 	.word	0x004d4152
3000c58c:	46205041 	.word	0x46205041
3000c590:	00005049 	.word	0x00005049
3000c594:	444f4d5b 	.word	0x444f4d5b
3000c598:	5f454c55 	.word	0x5f454c55
3000c59c:	544f4f42 	.word	0x544f4f42
3000c5a0:	56454c2d 	.word	0x56454c2d
3000c5a4:	495f4c45 	.word	0x495f4c45
3000c5a8:	5d4f464e 	.word	0x5d4f464e
3000c5ac:	2073253a 	.word	0x2073253a
3000c5b0:	61766e49 	.word	0x61766e49
3000c5b4:	0a64696c 	.word	0x0a64696c
3000c5b8:	00000000 	.word	0x00000000
3000c5bc:	444f4d5b 	.word	0x444f4d5b
3000c5c0:	5f454c55 	.word	0x5f454c55
3000c5c4:	544f4f42 	.word	0x544f4f42
3000c5c8:	56454c2d 	.word	0x56454c2d
3000c5cc:	495f4c45 	.word	0x495f4c45
3000c5d0:	5d4f464e 	.word	0x5d4f464e
3000c5d4:	5b73253a 	.word	0x5b73253a
3000c5d8:	78383025 	.word	0x78383025
3000c5dc:	5d78253a 	.word	0x5d78253a
3000c5e0:	0000000a 	.word	0x0000000a
3000c5e4:	444f4d5b 	.word	0x444f4d5b
3000c5e8:	5f454c55 	.word	0x5f454c55
3000c5ec:	544f4f42 	.word	0x544f4f42
3000c5f0:	56454c2d 	.word	0x56454c2d
3000c5f4:	495f4c45 	.word	0x495f4c45
3000c5f8:	5d4f464e 	.word	0x5d4f464e
3000c5fc:	5044523a 	.word	0x5044523a
3000c600:	0a4e4520 	.word	0x0a4e4520
3000c604:	00000000 	.word	0x00000000
3000c608:	444f4d5b 	.word	0x444f4d5b
3000c60c:	5f454c55 	.word	0x5f454c55
3000c610:	544f4f42 	.word	0x544f4f42
3000c614:	56454c2d 	.word	0x56454c2d
3000c618:	495f4c45 	.word	0x495f4c45
3000c61c:	5d4f464e 	.word	0x5d4f464e
3000c620:	5044523a 	.word	0x5044523a
3000c624:	46464f20 	.word	0x46464f20
3000c628:	0000000a 	.word	0x0000000a
3000c62c:	444f4d5b 	.word	0x444f4d5b
3000c630:	5f454c55 	.word	0x5f454c55
3000c634:	544f4f42 	.word	0x544f4f42
3000c638:	56454c2d 	.word	0x56454c2d
3000c63c:	455f4c45 	.word	0x455f4c45
3000c640:	524f5252 	.word	0x524f5252
3000c644:	61463a5d 	.word	0x61463a5d
3000c648:	74206c69 	.word	0x74206c69
3000c64c:	6f6c206f 	.word	0x6f6c206f
3000c650:	52206461 	.word	0x52206461
3000c654:	69205044 	.word	0x69205044
3000c658:	6567616d 	.word	0x6567616d
3000c65c:	00000a21 	.word	0x00000a21
3000c660:	474d490d 	.word	0x474d490d
3000c664:	544f2032 	.word	0x544f2032
3000c668:	4e452046 	.word	0x4e452046
3000c66c:	0000000a 	.word	0x0000000a
3000c670:	46544f0d 	.word	0x46544f0d
3000c674:	646f4d20 	.word	0x646f4d20
3000c678:	72652065 	.word	0x72652065
3000c67c:	0d726f72 	.word	0x0d726f72
3000c680:	0000000a 	.word	0x0000000a
3000c684:	494c4156 	.word	0x494c4156
3000c688:	00000044 	.word	0x00000044
3000c68c:	41564e49 	.word	0x41564e49
3000c690:	0044494c 	.word	0x0044494c
3000c694:	74726563 	.word	0x74726563
3000c698:	63696669 	.word	0x63696669
3000c69c:	20657461 	.word	0x20657461
3000c6a0:	20434345 	.word	0x20434345
3000c6a4:	69726576 	.word	0x69726576
3000c6a8:	66207966 	.word	0x66207966
3000c6ac:	2c6c6961 	.word	0x2c6c6961
3000c6b0:	6c6f7220 	.word	0x6c6f7220
3000c6b4:	6361626c 	.word	0x6361626c
3000c6b8:	000a0d6b 	.word	0x000a0d6b
3000c6bc:	444f4d5b 	.word	0x444f4d5b
3000c6c0:	5f454c55 	.word	0x5f454c55
3000c6c4:	544f4f42 	.word	0x544f4f42
3000c6c8:	56454c2d 	.word	0x56454c2d
3000c6cc:	495f4c45 	.word	0x495f4c45
3000c6d0:	5d4f464e 	.word	0x5d4f464e
3000c6d4:	474d493a 	.word	0x474d493a
3000c6d8:	544f2832 	.word	0x544f2832
3000c6dc:	29642541 	.word	0x29642541
3000c6e0:	2c732520 	.word	0x2c732520
3000c6e4:	74657220 	.word	0x74657220
3000c6e8:	6425203a 	.word	0x6425203a
3000c6ec:	0000000a 	.word	0x0000000a
3000c6f0:	32474d49 	.word	0x32474d49
3000c6f4:	43434520 	.word	0x43434520
3000c6f8:	72657620 	.word	0x72657620
3000c6fc:	20796669 	.word	0x20796669
3000c700:	6c696166 	.word	0x6c696166
3000c704:	6f72202c 	.word	0x6f72202c
3000c708:	61626c6c 	.word	0x61626c6c
3000c70c:	0a0d6b63 	.word	0x0a0d6b63
3000c710:	00000000 	.word	0x00000000
3000c714:	5f41544f 	.word	0x5f41544f
3000c718:	45205041 	.word	0x45205041
3000c71c:	76204343 	.word	0x76204343
3000c720:	66697265 	.word	0x66697265
3000c724:	61662079 	.word	0x61662079
3000c728:	202c6c69 	.word	0x202c6c69
3000c72c:	6c6c6f72 	.word	0x6c6c6f72
3000c730:	6b636162 	.word	0x6b636162
3000c734:	00000a0d 	.word	0x00000a0d
3000c738:	64616f4c 	.word	0x64616f4c
3000c73c:	41544f20 	.word	0x41544f20
3000c740:	2050415f 	.word	0x2050415f
3000c744:	67616d69 	.word	0x67616d69
3000c748:	61662065 	.word	0x61662065
3000c74c:	202c6c69 	.word	0x202c6c69
3000c750:	6c6c6f72 	.word	0x6c6c6f72
3000c754:	6b636162 	.word	0x6b636162
3000c758:	00000a0d 	.word	0x00000a0d
3000c75c:	444f4d5b 	.word	0x444f4d5b
3000c760:	5f454c55 	.word	0x5f454c55
3000c764:	544f4f42 	.word	0x544f4f42
3000c768:	56454c2d 	.word	0x56454c2d
3000c76c:	455f4c45 	.word	0x455f4c45
3000c770:	524f5252 	.word	0x524f5252
3000c774:	61463a5d 	.word	0x61463a5d
3000c778:	74206c69 	.word	0x74206c69
3000c77c:	6f6c206f 	.word	0x6f6c206f
3000c780:	41206461 	.word	0x41206461
3000c784:	6d692050 	.word	0x6d692050
3000c788:	21656761 	.word	0x21656761
3000c78c:	0000000a 	.word	0x0000000a
3000c790:	20344d4b 	.word	0x20344d4b
3000c794:	33474d49 	.word	0x33474d49
3000c798:	00000000 	.word	0x00000000
3000c79c:	20344d4b 	.word	0x20344d4b
3000c7a0:	0043534e 	.word	0x0043534e
3000c7a4:	31474d49 	.word	0x31474d49
3000c7a8:	00000000 	.word	0x00000000
3000c7ac:	32474d49 	.word	0x32474d49
3000c7b0:	00000000 	.word	0x00000000
3000c7b4:	42205041 	.word	0x42205041
3000c7b8:	4626314c 	.word	0x4626314c
3000c7bc:	00005049 	.word	0x00005049
3000c7c0:	444f4d5b 	.word	0x444f4d5b
3000c7c4:	5f454c55 	.word	0x5f454c55
3000c7c8:	544f4f42 	.word	0x544f4f42
3000c7cc:	56454c2d 	.word	0x56454c2d
3000c7d0:	455f4c45 	.word	0x455f4c45
3000c7d4:	524f5252 	.word	0x524f5252
3000c7d8:	68543a5d 	.word	0x68543a5d
3000c7dc:	20657265 	.word	0x20657265
3000c7e0:	206e6163 	.word	0x206e6163
3000c7e4:	61206562 	.word	0x61206562
3000c7e8:	6f6d2074 	.word	0x6f6d2074
3000c7ec:	38207473 	.word	0x38207473
3000c7f0:	49535220 	.word	0x49535220
3000c7f4:	616d2050 	.word	0x616d2050
3000c7f8:	65206b73 	.word	0x65206b73
3000c7fc:	6972746e 	.word	0x6972746e
3000c800:	0a217365 	.word	0x0a217365
3000c804:	00000000 	.word	0x00000000
3000c808:	444f4d5b 	.word	0x444f4d5b
3000c80c:	5f454c55 	.word	0x5f454c55
3000c810:	544f4f42 	.word	0x544f4f42
3000c814:	56454c2d 	.word	0x56454c2d
3000c818:	455f4c45 	.word	0x455f4c45
3000c81c:	524f5252 	.word	0x524f5252
3000c820:	53523a5d 	.word	0x53523a5d
3000c824:	65205049 	.word	0x65205049
3000c828:	7972746e 	.word	0x7972746e
3000c82c:	64646120 	.word	0x64646120
3000c830:	78252072 	.word	0x78252072
3000c834:	766e6920 	.word	0x766e6920
3000c838:	64696c61 	.word	0x64696c61
3000c83c:	0000000a 	.word	0x0000000a
3000c840:	20504452 	.word	0x20504452
3000c844:	4c494146 	.word	0x4c494146
3000c848:	0000000a 	.word	0x0000000a
3000c84c:	20504452 	.word	0x20504452
3000c850:	43435553 	.word	0x43435553
3000c854:	0a535345 	.word	0x0a535345
3000c858:	00000000 	.word	0x00000000
3000c85c:	059e4eb3 	.word	0x059e4eb3
3000c860:	4b11ea39 	.word	0x4b11ea39
3000c864:	2a1b920a 	.word	0x2a1b920a
3000c868:	29939cee 	.word	0x29939cee
3000c86c:	00000000 	.word	0x00000000
3000c870:	42205041 	.word	0x42205041
3000c874:	0032334c 	.word	0x0032334c
3000c878:	2073250d 	.word	0x2073250d
3000c87c:	49524556 	.word	0x49524556
3000c880:	50205946 	.word	0x50205946
3000c884:	0a535341 	.word	0x0a535341
3000c888:	00000000 	.word	0x00000000
3000c88c:	2073250d 	.word	0x2073250d
3000c890:	49524556 	.word	0x49524556
3000c894:	46205946 	.word	0x46205946
3000c898:	2c4c4941 	.word	0x2c4c4941
3000c89c:	74657220 	.word	0x74657220
3000c8a0:	25203d20 	.word	0x25203d20
3000c8a4:	00000a64 	.word	0x00000a64
3000c8a8:	7265430d 	.word	0x7265430d
3000c8ac:	69666974 	.word	0x69666974
3000c8b0:	65746163 	.word	0x65746163
3000c8b4:	52455620 	.word	0x52455620
3000c8b8:	20594649 	.word	0x20594649
3000c8bc:	4c494146 	.word	0x4c494146
3000c8c0:	6572202c 	.word	0x6572202c
3000c8c4:	203d2074 	.word	0x203d2074
3000c8c8:	000a6425 	.word	0x000a6425
3000c8cc:	444f4d5b 	.word	0x444f4d5b
3000c8d0:	5f454c55 	.word	0x5f454c55
3000c8d4:	544f4f42 	.word	0x544f4f42
3000c8d8:	56454c2d 	.word	0x56454c2d
3000c8dc:	495f4c45 	.word	0x495f4c45
3000c8e0:	5d4f464e 	.word	0x5d4f464e
3000c8e4:	41544f3a 	.word	0x41544f3a
3000c8e8:	53552031 	.word	0x53552031
3000c8ec:	76202c45 	.word	0x76202c45
3000c8f0:	69737265 	.word	0x69737265
3000c8f4:	203a6e6f 	.word	0x203a6e6f
3000c8f8:	000a6425 	.word	0x000a6425
3000c8fc:	444f4d5b 	.word	0x444f4d5b
3000c900:	5f454c55 	.word	0x5f454c55
3000c904:	544f4f42 	.word	0x544f4f42
3000c908:	56454c2d 	.word	0x56454c2d
3000c90c:	495f4c45 	.word	0x495f4c45
3000c910:	5d4f464e 	.word	0x5d4f464e
3000c914:	41544f3a 	.word	0x41544f3a
3000c918:	53552032 	.word	0x53552032
3000c91c:	76202c45 	.word	0x76202c45
3000c920:	69737265 	.word	0x69737265
3000c924:	203a6e6f 	.word	0x203a6e6f
3000c928:	000a6425 	.word	0x000a6425
3000c92c:	444f4d5b 	.word	0x444f4d5b
3000c930:	5f454c55 	.word	0x5f454c55
3000c934:	544f4f42 	.word	0x544f4f42
3000c938:	56454c2d 	.word	0x56454c2d
3000c93c:	495f4c45 	.word	0x495f4c45
3000c940:	5d4f464e 	.word	0x5d4f464e
3000c944:	746f423a 	.word	0x746f423a
3000c948:	544f2068 	.word	0x544f2068
3000c94c:	6d692041 	.word	0x6d692041
3000c950:	20656761 	.word	0x20656761
3000c954:	69726576 	.word	0x69726576
3000c958:	66207966 	.word	0x66207966
3000c95c:	656c6961 	.word	0x656c6961
3000c960:	00000a64 	.word	0x00000a64
3000c964:	444f4d5b 	.word	0x444f4d5b
3000c968:	5f454c55 	.word	0x5f454c55
3000c96c:	544f4f42 	.word	0x544f4f42
3000c970:	56454c2d 	.word	0x56454c2d
3000c974:	495f4c45 	.word	0x495f4c45
3000c978:	5d4f464e 	.word	0x5d4f464e
3000c97c:	696e493a 	.word	0x696e493a
3000c980:	42572074 	.word	0x42572074
3000c984:	00000a0d 	.word	0x00000a0d
3000c988:	444f4d5b 	.word	0x444f4d5b
3000c98c:	5f454c55 	.word	0x5f454c55
3000c990:	544f4f42 	.word	0x544f4f42
3000c994:	56454c2d 	.word	0x56454c2d
3000c998:	495f4c45 	.word	0x495f4c45
3000c99c:	5d4f464e 	.word	0x5d4f464e
3000c9a0:	4965523a 	.word	0x4965523a
3000c9a4:	2074696e 	.word	0x2074696e
3000c9a8:	0d524444 	.word	0x0d524444
3000c9ac:	0000000a 	.word	0x0000000a
3000c9b0:	73616c66 	.word	0x73616c66
3000c9b4:	69735f68 	.word	0x69735f68
3000c9b8:	203a657a 	.word	0x203a657a
3000c9bc:	000a4d38 	.word	0x000a4d38
3000c9c0:	73616c66 	.word	0x73616c66
3000c9c4:	69735f68 	.word	0x69735f68
3000c9c8:	203a657a 	.word	0x203a657a
3000c9cc:	0a4d3631 	.word	0x0a4d3631
3000c9d0:	00000000 	.word	0x00000000
3000c9d4:	20315042 	.word	0x20315042
3000c9d8:	73726576 	.word	0x73726576
3000c9dc:	206e6f69 	.word	0x206e6f69
3000c9e0:	61766e69 	.word	0x61766e69
3000c9e4:	0a64696c 	.word	0x0a64696c
3000c9e8:	00000000 	.word	0x00000000
3000c9ec:	20315042 	.word	0x20315042
3000c9f0:	6d726f66 	.word	0x6d726f66
3000c9f4:	69207461 	.word	0x69207461
3000c9f8:	6c61766e 	.word	0x6c61766e
3000c9fc:	000a6469 	.word	0x000a6469
3000ca00:	20315042 	.word	0x20315042
3000ca04:	69746361 	.word	0x69746361
3000ca08:	69206576 	.word	0x69206576
3000ca0c:	7865646e 	.word	0x7865646e
3000ca10:	766e6920 	.word	0x766e6920
3000ca14:	64696c61 	.word	0x64696c61
3000ca18:	0000000a 	.word	0x0000000a
3000ca1c:	20315042 	.word	0x20315042
3000ca20:	61746164 	.word	0x61746164
3000ca24:	6c617620 	.word	0x6c617620
3000ca28:	202c6469 	.word	0x202c6469
3000ca2c:	73726576 	.word	0x73726576
3000ca30:	3a6e6f69 	.word	0x3a6e6f69
3000ca34:	000a6425 	.word	0x000a6425
3000ca38:	20325042 	.word	0x20325042
3000ca3c:	6d726f66 	.word	0x6d726f66
3000ca40:	69207461 	.word	0x69207461
3000ca44:	6c61766e 	.word	0x6c61766e
3000ca48:	000a6469 	.word	0x000a6469
3000ca4c:	20315042 	.word	0x20315042
3000ca50:	33435243 	.word	0x33435243
3000ca54:	616d2032 	.word	0x616d2032
3000ca58:	2c686374 	.word	0x2c686374
3000ca5c:	74746120 	.word	0x74746120
3000ca60:	65686361 	.word	0x65686361
3000ca64:	52432064 	.word	0x52432064
3000ca68:	30203a43 	.word	0x30203a43
3000ca6c:	2c782578 	.word	0x2c782578
3000ca70:	6c616320 	.word	0x6c616320
3000ca74:	616c7563 	.word	0x616c7563
3000ca78:	20646574 	.word	0x20646574
3000ca7c:	3a435243 	.word	0x3a435243
3000ca80:	25783020 	.word	0x25783020
3000ca84:	00000a78 	.word	0x00000a78
3000ca88:	20315042 	.word	0x20315042
3000ca8c:	33435243 	.word	0x33435243
3000ca90:	6f6e2032 	.word	0x6f6e2032
3000ca94:	616d2074 	.word	0x616d2074
3000ca98:	2c686374 	.word	0x2c686374
3000ca9c:	74746120 	.word	0x74746120
3000caa0:	65686361 	.word	0x65686361
3000caa4:	52432064 	.word	0x52432064
3000caa8:	30203a43 	.word	0x30203a43
3000caac:	2c782578 	.word	0x2c782578
3000cab0:	6c616320 	.word	0x6c616320
3000cab4:	616c7563 	.word	0x616c7563
3000cab8:	20646574 	.word	0x20646574
3000cabc:	3a435243 	.word	0x3a435243
3000cac0:	25783020 	.word	0x25783020
3000cac4:	00000a78 	.word	0x00000a78
3000cac8:	20325042 	.word	0x20325042
3000cacc:	33435243 	.word	0x33435243
3000cad0:	616d2032 	.word	0x616d2032
3000cad4:	2c686374 	.word	0x2c686374
3000cad8:	74746120 	.word	0x74746120
3000cadc:	65686361 	.word	0x65686361
3000cae0:	52432064 	.word	0x52432064
3000cae4:	30203a43 	.word	0x30203a43
3000cae8:	2c782578 	.word	0x2c782578
3000caec:	6c616320 	.word	0x6c616320
3000caf0:	616c7563 	.word	0x616c7563
3000caf4:	20646574 	.word	0x20646574
3000caf8:	3a435243 	.word	0x3a435243
3000cafc:	25783020 	.word	0x25783020
3000cb00:	00000a78 	.word	0x00000a78
3000cb04:	20325042 	.word	0x20325042
3000cb08:	33435243 	.word	0x33435243
3000cb0c:	6f6e2032 	.word	0x6f6e2032
3000cb10:	616d2074 	.word	0x616d2074
3000cb14:	2c686374 	.word	0x2c686374
3000cb18:	74746120 	.word	0x74746120
3000cb1c:	65686361 	.word	0x65686361
3000cb20:	52432064 	.word	0x52432064
3000cb24:	30203a43 	.word	0x30203a43
3000cb28:	2c782578 	.word	0x2c782578
3000cb2c:	6c616320 	.word	0x6c616320
3000cb30:	616c7563 	.word	0x616c7563
3000cb34:	20646574 	.word	0x20646574
3000cb38:	3a435243 	.word	0x3a435243
3000cb3c:	25783020 	.word	0x25783020
3000cb40:	00000a78 	.word	0x00000a78
3000cb44:	20315042 	.word	0x20315042
3000cb48:	3161746f 	.word	0x3161746f
3000cb4c:	64646120 	.word	0x64646120
3000cb50:	73736572 	.word	0x73736572
3000cb54:	766e6920 	.word	0x766e6920
3000cb58:	64696c61 	.word	0x64696c61
3000cb5c:	0000000a 	.word	0x0000000a
3000cb60:	20315042 	.word	0x20315042
3000cb64:	3261746f 	.word	0x3261746f
3000cb68:	64646120 	.word	0x64646120
3000cb6c:	73736572 	.word	0x73736572
3000cb70:	766e6920 	.word	0x766e6920
3000cb74:	64696c61 	.word	0x64696c61
3000cb78:	0000000a 	.word	0x0000000a
3000cb7c:	68746f42 	.word	0x68746f42
3000cb80:	20504220 	.word	0x20504220
3000cb84:	20435243 	.word	0x20435243
3000cb88:	696c6176 	.word	0x696c6176
3000cb8c:	00000a64 	.word	0x00000a64
3000cb90:	20315042 	.word	0x20315042
3000cb94:	656c6573 	.word	0x656c6573
3000cb98:	64657463 	.word	0x64657463
3000cb9c:	0000000a 	.word	0x0000000a
3000cba0:	20325042 	.word	0x20325042
3000cba4:	656c6573 	.word	0x656c6573
3000cba8:	64657463 	.word	0x64657463
3000cbac:	0000000a 	.word	0x0000000a
3000cbb0:	68746f42 	.word	0x68746f42
3000cbb4:	20504220 	.word	0x20504220
3000cbb8:	61766e69 	.word	0x61766e69
3000cbbc:	0a64696c 	.word	0x0a64696c
3000cbc0:	00000000 	.word	0x00000000
3000cbc4:	6f636552 	.word	0x6f636552
3000cbc8:	79726576 	.word	0x79726576
3000cbcc:	61747320 	.word	0x61747320
3000cbd0:	000a7472 	.word	0x000a7472
3000cbd4:	69746341 	.word	0x69746341
3000cbd8:	49206576 	.word	0x49206576
3000cbdc:	7865646e 	.word	0x7865646e
3000cbe0:	6425203a 	.word	0x6425203a
3000cbe4:	0000000a 	.word	0x0000000a
3000cbe8:	3141544f 	.word	0x3141544f
3000cbec:	766e6920 	.word	0x766e6920
3000cbf0:	64696c61 	.word	0x64696c61
3000cbf4:	0000000a 	.word	0x0000000a
3000cbf8:	3241544f 	.word	0x3241544f
3000cbfc:	766e6920 	.word	0x766e6920
3000cc00:	64696c61 	.word	0x64696c61
3000cc04:	0000000a 	.word	0x0000000a
3000cc08:	3141544f 	.word	0x3141544f
3000cc0c:	43524320 	.word	0x43524320
3000cc10:	6d203233 	.word	0x6d203233
3000cc14:	68637461 	.word	0x68637461
3000cc18:	7461202c 	.word	0x7461202c
3000cc1c:	68636174 	.word	0x68636174
3000cc20:	43206465 	.word	0x43206465
3000cc24:	203a4352 	.word	0x203a4352
3000cc28:	78257830 	.word	0x78257830
3000cc2c:	6163202c 	.word	0x6163202c
3000cc30:	6c75636c 	.word	0x6c75636c
3000cc34:	64657461 	.word	0x64657461
3000cc38:	43524320 	.word	0x43524320
3000cc3c:	7830203a 	.word	0x7830203a
3000cc40:	000a7825 	.word	0x000a7825
3000cc44:	3141544f 	.word	0x3141544f
3000cc48:	43524320 	.word	0x43524320
3000cc4c:	6e203233 	.word	0x6e203233
3000cc50:	6d20746f 	.word	0x6d20746f
3000cc54:	68637461 	.word	0x68637461
3000cc58:	7461202c 	.word	0x7461202c
3000cc5c:	68636174 	.word	0x68636174
3000cc60:	43206465 	.word	0x43206465
3000cc64:	203a4352 	.word	0x203a4352
3000cc68:	78257830 	.word	0x78257830
3000cc6c:	6163202c 	.word	0x6163202c
3000cc70:	6c75636c 	.word	0x6c75636c
3000cc74:	64657461 	.word	0x64657461
3000cc78:	43524320 	.word	0x43524320
3000cc7c:	7830203a 	.word	0x7830203a
3000cc80:	202c7825 	.word	0x202c7825
3000cc84:	73617265 	.word	0x73617265
3000cc88:	544f2065 	.word	0x544f2065
3000cc8c:	000a3141 	.word	0x000a3141
3000cc90:	3241544f 	.word	0x3241544f
3000cc94:	43524320 	.word	0x43524320
3000cc98:	6d203233 	.word	0x6d203233
3000cc9c:	68637461 	.word	0x68637461
3000cca0:	7461202c 	.word	0x7461202c
3000cca4:	68636174 	.word	0x68636174
3000cca8:	43206465 	.word	0x43206465
3000ccac:	203a4352 	.word	0x203a4352
3000ccb0:	78257830 	.word	0x78257830
3000ccb4:	6163202c 	.word	0x6163202c
3000ccb8:	6c75636c 	.word	0x6c75636c
3000ccbc:	64657461 	.word	0x64657461
3000ccc0:	43524320 	.word	0x43524320
3000ccc4:	7830203a 	.word	0x7830203a
3000ccc8:	000a7825 	.word	0x000a7825
3000cccc:	3241544f 	.word	0x3241544f
3000ccd0:	43524320 	.word	0x43524320
3000ccd4:	6e203233 	.word	0x6e203233
3000ccd8:	6d20746f 	.word	0x6d20746f
3000ccdc:	68637461 	.word	0x68637461
3000cce0:	7461202c 	.word	0x7461202c
3000cce4:	68636174 	.word	0x68636174
3000cce8:	43206465 	.word	0x43206465
3000ccec:	203a4352 	.word	0x203a4352
3000ccf0:	78257830 	.word	0x78257830
3000ccf4:	6163202c 	.word	0x6163202c
3000ccf8:	6c75636c 	.word	0x6c75636c
3000ccfc:	64657461 	.word	0x64657461
3000cd00:	43524320 	.word	0x43524320
3000cd04:	7830203a 	.word	0x7830203a
3000cd08:	202c7825 	.word	0x202c7825
3000cd0c:	73617265 	.word	0x73617265
3000cd10:	544f2065 	.word	0x544f2065
3000cd14:	000a3241 	.word	0x000a3241
3000cd18:	69746341 	.word	0x69746341
3000cd1c:	69206576 	.word	0x69206576
3000cd20:	7865646e 	.word	0x7865646e
3000cd24:	41544f20 	.word	0x41544f20
3000cd28:	766e6920 	.word	0x766e6920
3000cd2c:	64696c61 	.word	0x64696c61
3000cd30:	6572202c 	.word	0x6572202c
3000cd34:	74697277 	.word	0x74697277
3000cd38:	63612065 	.word	0x63612065
3000cd3c:	65766974 	.word	0x65766974
3000cd40:	646e6920 	.word	0x646e6920
3000cd44:	74207865 	.word	0x74207865
3000cd48:	6425206f 	.word	0x6425206f
3000cd4c:	0000000a 	.word	0x0000000a
3000cd50:	206c6c41 	.word	0x206c6c41
3000cd54:	2041544f 	.word	0x2041544f
3000cd58:	72646461 	.word	0x72646461
3000cd5c:	65737365 	.word	0x65737365
3000cd60:	6f6e2073 	.word	0x6f6e2073
3000cd64:	61762074 	.word	0x61762074
3000cd68:	2c64696c 	.word	0x2c64696c
3000cd6c:	63657220 	.word	0x63657220
3000cd70:	7265766f 	.word	0x7265766f
3000cd74:	61662079 	.word	0x61662079
3000cd78:	000a6c69 	.word	0x000a6c69
3000cd7c:	20325042 	.word	0x20325042
3000cd80:	3261746f 	.word	0x3261746f
3000cd84:	64646120 	.word	0x64646120
3000cd88:	73736572 	.word	0x73736572
3000cd8c:	766e6920 	.word	0x766e6920
3000cd90:	64696c61 	.word	0x64696c61
3000cd94:	0000000a 	.word	0x0000000a
3000cd98:	20325042 	.word	0x20325042
3000cd9c:	73726576 	.word	0x73726576
3000cda0:	206e6f69 	.word	0x206e6f69
3000cda4:	61766e69 	.word	0x61766e69
3000cda8:	0a64696c 	.word	0x0a64696c
3000cdac:	00000000 	.word	0x00000000
3000cdb0:	20325042 	.word	0x20325042
3000cdb4:	69746361 	.word	0x69746361
3000cdb8:	69206576 	.word	0x69206576
3000cdbc:	7865646e 	.word	0x7865646e
3000cdc0:	766e6920 	.word	0x766e6920
3000cdc4:	64696c61 	.word	0x64696c61
3000cdc8:	0000000a 	.word	0x0000000a
3000cdcc:	20325042 	.word	0x20325042
3000cdd0:	3161746f 	.word	0x3161746f
3000cdd4:	64646120 	.word	0x64646120
3000cdd8:	73736572 	.word	0x73736572
3000cddc:	766e6920 	.word	0x766e6920
3000cde0:	64696c61 	.word	0x64696c61
3000cde4:	0000000a 	.word	0x0000000a
3000cde8:	20325042 	.word	0x20325042
3000cdec:	61746164 	.word	0x61746164
3000cdf0:	6c617620 	.word	0x6c617620
3000cdf4:	202c6469 	.word	0x202c6469
3000cdf8:	73726576 	.word	0x73726576
3000cdfc:	3a6e6f69 	.word	0x3a6e6f69
3000ce00:	000a6425 	.word	0x000a6425
3000ce04:	444f4d5b 	.word	0x444f4d5b
3000ce08:	5f454c55 	.word	0x5f454c55
3000ce0c:	544f4f42 	.word	0x544f4f42
3000ce10:	56454c2d 	.word	0x56454c2d
3000ce14:	495f4c45 	.word	0x495f4c45
3000ce18:	5d4f464e 	.word	0x5d4f464e
3000ce1c:	7369443a 	.word	0x7369443a
3000ce20:	656c6261 	.word	0x656c6261
3000ce24:	304d4b20 	.word	0x304d4b20
3000ce28:	6f6f4220 	.word	0x6f6f4220
3000ce2c:	69202074 	.word	0x69202074
3000ce30:	6554206e 	.word	0x6554206e
3000ce34:	6d207473 	.word	0x6d207473
3000ce38:	0d65646f 	.word	0x0d65646f
3000ce3c:	0000000a 	.word	0x0000000a
3000ce40:	444f4d5b 	.word	0x444f4d5b
3000ce44:	5f454c55 	.word	0x5f454c55
3000ce48:	544f4f42 	.word	0x544f4f42
3000ce4c:	56454c2d 	.word	0x56454c2d
3000ce50:	495f4c45 	.word	0x495f4c45
3000ce54:	5d4f464e 	.word	0x5d4f464e
3000ce58:	7079423a 	.word	0x7079423a
3000ce5c:	20737361 	.word	0x20737361
3000ce60:	2050544f 	.word	0x2050544f
3000ce64:	6f747561 	.word	0x6f747561
3000ce68:	64616f6c 	.word	0x64616f6c
3000ce6c:	00000a0d 	.word	0x00000a0d
3000ce70:	444f4d5b 	.word	0x444f4d5b
3000ce74:	5f454c55 	.word	0x5f454c55
3000ce78:	544f4f42 	.word	0x544f4f42
3000ce7c:	56454c2d 	.word	0x56454c2d
3000ce80:	495f4c45 	.word	0x495f4c45
3000ce84:	5d4f464e 	.word	0x5d4f464e
3000ce88:	20504e3a 	.word	0x20504e3a
3000ce8c:	71657246 	.word	0x71657246
3000ce90:	20642520 	.word	0x20642520
3000ce94:	0a7a484d 	.word	0x0a7a484d
3000ce98:	00000000 	.word	0x00000000
3000ce9c:	444f4d5b 	.word	0x444f4d5b
3000cea0:	5f454c55 	.word	0x5f454c55
3000cea4:	544f4f42 	.word	0x544f4f42
3000cea8:	56454c2d 	.word	0x56454c2d
3000ceac:	495f4c45 	.word	0x495f4c45
3000ceb0:	5d4f464e 	.word	0x5d4f464e
3000ceb4:	2050413a 	.word	0x2050413a
3000ceb8:	71657246 	.word	0x71657246
3000cebc:	20642520 	.word	0x20642520
3000cec0:	0a7a484d 	.word	0x0a7a484d
3000cec4:	00000000 	.word	0x00000000
3000cec8:	7361700a 	.word	0x7361700a
3000cecc:	726f7773 	.word	0x726f7773
3000ced0:	65762064 	.word	0x65762064
3000ced4:	79666972 	.word	0x79666972
3000ced8:	69616620 	.word	0x69616620
3000cedc:	00000a6c 	.word	0x00000a6c
3000cee0:	7361700a 	.word	0x7361700a
3000cee4:	726f7773 	.word	0x726f7773
3000cee8:	65762064 	.word	0x65762064
3000ceec:	79666972 	.word	0x79666972
3000cef0:	63757320 	.word	0x63757320
3000cef4:	73736563 	.word	0x73736563
3000cef8:	0000000a 	.word	0x0000000a
3000cefc:	31474d49 	.word	0x31474d49
3000cf00:	4f425320 	.word	0x4f425320
3000cf04:	4f20544f 	.word	0x4f20544f
3000cf08:	000a4646 	.word	0x000a4646
3000cf0c:	31474d49 	.word	0x31474d49
3000cf10:	4f425320 	.word	0x4f425320
3000cf14:	5320544f 	.word	0x5320544f
3000cf18:	45434355 	.word	0x45434355
3000cf1c:	000a5353 	.word	0x000a5353
3000cf20:	746f6f62 	.word	0x746f6f62
3000cf24:	64616f6c 	.word	0x64616f6c
3000cf28:	765f7265 	.word	0x765f7265
3000cf2c:	69737265 	.word	0x69737265
3000cf30:	25206e6f 	.word	0x25206e6f
3000cf34:	00000a73 	.word	0x00000a73
3000cf38:	444f4d5b 	.word	0x444f4d5b
3000cf3c:	5f454c55 	.word	0x5f454c55
3000cf40:	544f4f42 	.word	0x544f4f42
3000cf44:	56454c2d 	.word	0x56454c2d
3000cf48:	495f4c45 	.word	0x495f4c45
3000cf4c:	5d4f464e 	.word	0x5d4f464e
3000cf50:	474d493a 	.word	0x474d493a
3000cf54:	4e452031 	.word	0x4e452031
3000cf58:	20524554 	.word	0x20524554
3000cf5c:	3a50534d 	.word	0x3a50534d
3000cf60:	3830255b 	.word	0x3830255b
3000cf64:	000a5d78 	.word	0x000a5d78
3000cf68:	444f4d5b 	.word	0x444f4d5b
3000cf6c:	5f454c55 	.word	0x5f454c55
3000cf70:	544f4f42 	.word	0x544f4f42
3000cf74:	56454c2d 	.word	0x56454c2d
3000cf78:	495f4c45 	.word	0x495f4c45
3000cf7c:	5d4f464e 	.word	0x5d4f464e
3000cf80:	474d493a 	.word	0x474d493a
3000cf84:	45532031 	.word	0x45532031
3000cf88:	45525543 	.word	0x45525543
3000cf8c:	41545320 	.word	0x41545320
3000cf90:	203a4554 	.word	0x203a4554
3000cf94:	000a6425 	.word	0x000a6425
3000cf98:	444f4d5b 	.word	0x444f4d5b
3000cf9c:	5f454c55 	.word	0x5f454c55
3000cfa0:	544f4f42 	.word	0x544f4f42
3000cfa4:	56454c2d 	.word	0x56454c2d
3000cfa8:	495f4c45 	.word	0x495f4c45
3000cfac:	5d4f464e 	.word	0x5d4f464e
3000cfb0:	696e493a 	.word	0x696e493a
3000cfb4:	53502074 	.word	0x53502074
3000cfb8:	0d4d4152 	.word	0x0d4d4152
3000cfbc:	0000000a 	.word	0x0000000a
3000cfc0:	7465730d 	.word	0x7465730d
3000cfc4:	6d667020 	.word	0x6d667020
3000cfc8:	69616620 	.word	0x69616620
3000cfcc:	000a0d6c 	.word	0x000a0d6c
3000cfd0:	444f4d5b 	.word	0x444f4d5b
3000cfd4:	5f454c55 	.word	0x5f454c55
3000cfd8:	544f4f42 	.word	0x544f4f42
3000cfdc:	56454c2d 	.word	0x56454c2d
3000cfe0:	495f4c45 	.word	0x495f4c45
3000cfe4:	5d4f464e 	.word	0x5d4f464e
3000cfe8:	5252453a 	.word	0x5252453a
3000cfec:	2121524f 	.word	0x2121524f
3000cff0:	6f685320 	.word	0x6f685320
3000cff4:	20646c75 	.word	0x20646c75
3000cff8:	20746f4e 	.word	0x20746f4e
3000cffc:	62616e65 	.word	0x62616e65
3000d000:	4d20656c 	.word	0x4d20656c
3000d004:	77536d65 	.word	0x77536d65
3000d008:	6e4f5f72 	.word	0x6e4f5f72
3000d00c:	6920796c 	.word	0x6920796c
3000d010:	4444206e 	.word	0x4444206e
3000d014:	68432052 	.word	0x68432052
3000d018:	21217069 	.word	0x21217069
3000d01c:	000a0d21 	.word	0x000a0d21
3000d020:	444f4d5b 	.word	0x444f4d5b
3000d024:	5f454c55 	.word	0x5f454c55
3000d028:	544f4f42 	.word	0x544f4f42
3000d02c:	56454c2d 	.word	0x56454c2d
3000d030:	495f4c45 	.word	0x495f4c45
3000d034:	5d4f464e 	.word	0x5d4f464e
3000d038:	696e493a 	.word	0x696e493a
3000d03c:	44442074 	.word	0x44442074
3000d040:	000a0d52 	.word	0x000a0d52
3000d044:	20445753 	.word	0x20445753
3000d048:	64207369 	.word	0x64207369
3000d04c:	62617369 	.word	0x62617369
3000d050:	2c64656c 	.word	0x2c64656c
3000d054:	6f727020 	.word	0x6f727020
3000d058:	64656563 	.word	0x64656563
3000d05c:	206f7420 	.word	0x206f7420
3000d060:	61736964 	.word	0x61736964
3000d064:	20656c62 	.word	0x20656c62
3000d068:	74726155 	.word	0x74726155
3000d06c:	776f4420 	.word	0x776f4420
3000d070:	616f6c6e 	.word	0x616f6c6e
3000d074:	2e2e2e64 	.word	0x2e2e2e64
3000d078:	0000000a 	.word	0x0000000a
3000d07c:	74726155 	.word	0x74726155
3000d080:	776f4420 	.word	0x776f4420
3000d084:	616f6c6e 	.word	0x616f6c6e
3000d088:	61682064 	.word	0x61682064
3000d08c:	65622073 	.word	0x65622073
3000d090:	64206e65 	.word	0x64206e65
3000d094:	62617369 	.word	0x62617369
3000d098:	0a64656c 	.word	0x0a64656c
3000d09c:	00000000 	.word	0x00000000
3000d0a0:	55676f4c 	.word	0x55676f4c
3000d0a4:	20747261 	.word	0x20747261
3000d0a8:	64756142 	.word	0x64756142
3000d0ac:	65746172 	.word	0x65746172
3000d0b0:	6425203a 	.word	0x6425203a
3000d0b4:	0000000a 	.word	0x0000000a
3000d0b8:	4848530a 	.word	0x4848530a
3000d0bc:	4946544f 	.word	0x4946544f
3000d0c0:	6f6e2058 	.word	0x6f6e2058
3000d0c4:	65722074 	.word	0x65722074
3000d0c8:	76696563 	.word	0x76696563
3000d0cc:	000a6465 	.word	0x000a6465
3000d0d0:	726f4e0a 	.word	0x726f4e0a
3000d0d4:	206c616d 	.word	0x206c616d
3000d0d8:	746f6f62 	.word	0x746f6f62
3000d0dc:	0000000a 	.word	0x0000000a
3000d0e0:	5f6e6962 	.word	0x5f6e6962
3000d0e4:	313e6f6e 	.word	0x313e6f6e
3000d0e8:	3d202c30 	.word	0x3d202c30
3000d0ec:	0a642520 	.word	0x0a642520
3000d0f0:	00000000 	.word	0x00000000
3000d0f4:	6e206164 	.word	0x6e206164
3000d0f8:	7220746f 	.word	0x7220746f
3000d0fc:	69656365 	.word	0x69656365
3000d100:	0a646576 	.word	0x0a646576
3000d104:	00000000 	.word	0x00000000
3000d108:	746f680a 	.word	0x746f680a
3000d10c:	20786966 	.word	0x20786966
3000d110:	656e6f64 	.word	0x656e6f64
3000d114:	202e2e2e 	.word	0x202e2e2e
3000d118:	6f626552 	.word	0x6f626552
3000d11c:	6e69746f 	.word	0x6e69746f
3000d120:	0a2e2e67 	.word	0x0a2e2e67
3000d124:	00000000 	.word	0x00000000
3000d128:	444f4d5b 	.word	0x444f4d5b
3000d12c:	5f454c55 	.word	0x5f454c55
3000d130:	544f4f42 	.word	0x544f4f42
3000d134:	56454c2d 	.word	0x56454c2d
3000d138:	495f4c45 	.word	0x495f4c45
3000d13c:	5d4f464e 	.word	0x5d4f464e
3000d140:	6174533a 	.word	0x6174533a
3000d144:	4e207472 	.word	0x4e207472
3000d148:	65536e6f 	.word	0x65536e6f
3000d14c:	65727563 	.word	0x65727563
3000d150:	30204020 	.word	0x30204020
3000d154:	20782578 	.word	0x20782578
3000d158:	0d2e2e2e 	.word	0x0d2e2e2e
3000d15c:	0000000a 	.word	0x0000000a
3000d160:	6c630a0d 	.word	0x6c630a0d
3000d164:	2065736f 	.word	0x2065736f
3000d168:	646f4d79 	.word	0x646f4d79
3000d16c:	54206d65 	.word	0x54206d65
3000d170:	736e6172 	.word	0x736e6172
3000d174:	20726566 	.word	0x20726566
3000d178:	0d2e2e2e 	.word	0x0d2e2e2e
3000d17c:	0000000a 	.word	0x0000000a
3000d180:	766e490d 	.word	0x766e490d
3000d184:	64696c61 	.word	0x64696c61
3000d188:	69684320 	.word	0x69684320
3000d18c:	666e696e 	.word	0x666e696e
3000d190:	4320216f 	.word	0x4320216f
3000d194:	6b636568 	.word	0x6b636568
3000d198:	50544f20 	.word	0x50544f20
3000d19c:	0000000a 	.word	0x0000000a
3000d1a0:	656c500d 	.word	0x656c500d
3000d1a4:	20657361 	.word	0x20657361
3000d1a8:	676f7270 	.word	0x676f7270
3000d1ac:	206d6172 	.word	0x206d6172
3000d1b0:	70696863 	.word	0x70696863
3000d1b4:	6f666e69 	.word	0x6f666e69
3000d1b8:	206e6920 	.word	0x206e6920
3000d1bc:	2050544f 	.word	0x2050544f
3000d1c0:	00000a21 	.word	0x00000a21

3000d1c4 <Memory_Info>:
3000d1c4:	03e00007 00060004 010202c0 02010100     ................
3000d1d4:	02000102 00040302 02040400 01010102     ................
3000d1e4:	01020221 02240401 03000102 00030103     !.....$.........
3000d1f4:	01050500 06000002 01020206 01070700     ................
3000d204:	08000002 01020208 02280801 09000102     ..........(.....
3000d214:	00040309 03290901 0a000004 0003010a     ......).........
3000d224:	012a0a01 0a010003 0003012a 020b0b00     ..*.....*.......
3000d234:	0c000102 0102020c ffffffff 0000ffff     ................
3000d244:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d254:	455f4c45 524f5252 52443a5d 69204d41     EL_ERROR]:DRAM i
3000d264:	6f6e2073 61432074 7262696c 6e6f6961     s not Calibraion
3000d274:	00000a0d                                ....

3000d278 <Flash_AVL>:
3000d278:	000000ef 000000ff 00000000 000043fc     .............C..
3000d288:	00000000 000000a1 000000ff 00000000     ................
3000d298:	0000fffc 00000000 0000000b 000000ff     ................
3000d2a8:	00000000 000043fc 00000000 0000000e     .....C..........
3000d2b8:	000000ff 00000000 000043fc 00000000     .........C......
3000d2c8:	000000c8 000000ff 00000001 000043fc     .............C..
3000d2d8:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d2e8:	000200fc 00000000 000000c2 000000ff     ................
3000d2f8:	00000002 000000fc 00000000 00000068     ............h...
3000d308:	000000ff 00000002 000000fc 00000000     ................
3000d318:	00000051 000000ff 00000002 000000fc     Q...............
3000d328:	00000000 0000001c 000000ff 00000003     ................
3000d338:	000000fc 00000000 00000020 000000ff     ........ .......
3000d348:	00000004 000000fc 00000000 00000000     ................
3000d358:	000000ff 000000fe ffffffff 30008b9d     ...............0
3000d368:	000000ff ffffffff 000000ff ffffffff     ................
3000d378:	00000000                                ....

3000d37c <Flash_ReadMode>:
3000d37c:	                                         ..

3000d37e <Flash_Speed>:
3000d37e:	                                         ..

3000d380 <NAND_AVL>:
3000d380:	000000ef 000000ff 00000000 ffffffff     ................
3000d390:	00000000 000000c8 000000ff 00000001     ................
3000d3a0:	ffffffff 00000000 000000e5 000000ff     ................
3000d3b0:	00000001 ffffffff 00000000 000000c2     ................
3000d3c0:	000000ff 00000002 ffffffff 00000000     ................
3000d3d0:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d3e8:	000000ff 000000fe ffffffff 30008bfd     ...............0
3000d3f8:	000000ff ffffffff 000000ff ffffffff     ................
3000d408:	00000000 3000c258 3000c25c 3000c260     ....X..0\..0`..0
3000d418:	3000c264 3000c268                       d..0h..0

3000d420 <__FUNCTION__.0>:
3000d420:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d430 <__FUNCTION__.1>:
3000d430:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d440:	00000000 444f4d5b 5f454c55 544f4f42     ....[MODULE_BOOT
3000d450:	56454c2d 455f4c45 524f5252 6c423a5d     -LEVEL_ERROR]:Bl
3000d460:	7830206b 69207825 42422073 00000a21     k 0x%x is BB!...
3000d470:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d480:	495f4c45 5d4f464e 6165523a 61502064     EL_INFO]:Read Pa
3000d490:	30206567 20782578 4220666f 6b636f6c     ge 0x%x of Block
3000d4a0:	25783020 614d2078 61462079 202c6c69      0x%x May Fail, 
3000d4b0:	74617473 30207375 21782578 0000000a     status 0x%x!....
3000d4c0:	444f4d5b 5f454c55 2d50544f 4556454c     [MODULE_OTP-LEVE
3000d4d0:	52455f4c 5d524f52 50544f3a 6165525f     L_ERROR]:OTP_Rea
3000d4e0:	46203864 206c6961 0a207825 00000000     d8 Fail %x .....
3000d4f0:	444f4d5b 5f454c55 2d50544f 4556454c     [MODULE_OTP-LEVE
3000d500:	52455f4c 5d524f52 524d503a 61655220     L_ERROR]:PMR Rea
3000d510:	72652064 21726f72 0000000a 444f4d5b     d error!....[MOD
3000d520:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d530:	5d524f52 50544f3a 6972575f 20386574     ROR]:OTP_Write8 
3000d540:	6c696146 20782520 0000000a 444f4d5b     Fail %x ....[MOD
3000d550:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d560:	5d524f52 50544f3a 6f727020 6d617267     ROR]:OTP program
3000d570:	72726520 0a21726f 00000000 444f4d5b      error!.....[MOD
3000d580:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d590:	5d524f52 524d503a 65522032 65206461     ROR]:PMR2 Read e
3000d5a0:	726f7272 00000a21                       rror!...

3000d5a8 <__FUNCTION__.1>:
3000d5a8:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d5b8:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d5c8:	495f4c45 5d4f464e 5253503a 43204d41     EL_INFO]:PSRAM C
3000d5d8:	206c7274 3a4b4c43 20642520 0a207a48     trl CLK: %d Hz .
3000d5e8:	00000000 444f4d5b 5f454c55 45535049     ....[MODULE_IPSE
3000d5f8:	454c2d43 5f4c4556 4f525245 4e3a5d52     C-LEVEL_ERROR]:N
3000d608:	6874206f 61207369 5f687475 65707974     o this auth_type
3000d618:	29642528 726f6620 41485320 00000a32     (%d) for SHA2...
3000d628:	7172690d 5d64255b 69727020 7469726f     .irq[%d] priorit
3000d638:	64252079 61687320 3c206c6c 6425203d     y %d shall <= %d
3000d648:	0000000a                                ....

3000d64c <__FUNCTION__.0>:
3000d64c:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000d65c:	00000000                                ....

3000d660 <__FUNCTION__.1>:
3000d660:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d670 <__FUNCTION__.2>:
3000d670:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d680:	32616873 696e695f 00000074              sha2_init...

3000d68c <__FUNCTION__.3>:
3000d68c:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d69c:	6f4d7974 44416564 00000000 5244440d     tyModeAD.....DDR
3000d6ac:	7a695320 73692065 746f6e20 6e6f6320      Size is not con
3000d6bc:	75676966 0a646572 00000000 5b203e0d     figured......> [
3000d6cc:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d6dc:	63203a5d 5f756c6f 616d6572 69625f70     ]: colu_remap_bi
3000d6ec:	21207374 7264203d 635f6d61 5f756c6f     ts != dram_colu_
3000d6fc:	73746962 0a212121 00000000 5b203e0d     bits!!!......> [
3000d70c:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d71c:	62203a5d 5f6b6e61 616d6572 69625f70     ]: bank_remap_bi
3000d72c:	21207374 7264203d 625f6d61 5f6b6e61     ts != dram_bank_
3000d73c:	73746962 0a212121 00000000              bits!!!.....

3000d748 <__FUNCTION__.1>:
3000d748:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d758:	00000000 3000c52c 3000c538 3000c544     ....,..08..0D..0
3000d768:	3000c550 3000c55c 3000c568 3000c574     P..0\..0h..0t..0
3000d778:	3000c580 3000c58c 3000c574 3000c580     ...0...0t..0...0
3000d788:	3000c790 3000c79c 3000c7a4 3000c7ac     ...0...0...0...0
3000d798:	3000c794 3000c7b4 00000208 0000020c     ...0...0........
3000d7a8:	00000210 00000008 00000214 00000218     ................
3000d7b8:	0000021c 0000000c                       ........

3000d7c0 <__FUNCTION__.0>:
3000d7c0:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.

3000d7d0 <km4_bootloader_rev>:
3000d7d0:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000d7e0:	305f7265 63666239 325f3431 2f333230     er_09bfc14_2023/
3000d7f0:	302f3131 35312d36 3a37333a 00003333     11/06-15:37:33..
