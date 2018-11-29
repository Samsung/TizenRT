/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 * Copyright © 2005-2014 Rich Felker, et al.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/
/****************************************************************************
 * origin: FreeBSD /usr/src/lib/msun/src/s_exp2.c
 *
 * Copyright (c) 2005 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 ***************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/compiler.h>

#include "libm.h"

#ifdef CONFIG_HAVE_DOUBLE

#define TBLSIZE 256

/************************************************************************
 * Private Data
 ************************************************************************/

static const double redux = 0x1 .8 p52 / TBLSIZE;
static const double P1 = 0x1 .62e42f efa39efp - 1;
static const double P2 = 0x1.ebfbdff82c575p - 3;
static const double P3 = 0x1.c6b08d704a0a6p - 5;
static const double P4 = 0x1 .3 b2ab88f70400p - 7;
static const double P5 = 0x1 .5 d88003875c74p - 10;

static const double tbl[TBLSIZE * 2] = {
	/* exp2(z +eps)         eps     */
	0x1 .6 a09e667f3d5dp - 1, 0x1 .9880 p - 44,
	0x1 .6 b052fa751744p - 1, 0x1 .8000 p - 50,
	0x1 .6 c012750bd9fep - 1, -0x1 .8780 p - 45,
	0x1 .6 cfdcddd476bfp - 1, 0x1.ec00p - 46,
	0x1 .6 dfb23c651a29p - 1, -0x1 .8000 p - 50,
	0x1 .6ef9298593 ae3p - 1, -0x1.c000p - 52,
	0x1 .6f f7df9519386p - 1, -0x1.fd80p - 45,
	0x1 .70f 7466f 42 da3p - 1, -0x1.c880p - 45,
	0x1 .71f 75e8 ec5fc3p - 1, 0x1 .3 c00p - 46,
	0x1 .72f 8286ea cf05p - 1, -0x1 .8300 p - 44,
	0x1 .73f 9 a48a58152p - 1, -0x1 .0 c00p - 47,
	0x1 .74f bd35d7ccfcp - 1, 0x1.f880p - 45,
	0x1 .75f eb564267f1p - 1, 0x1 .3e00 p - 47,
	0x1 .77024 b1ab6d48p - 1, -0x1 .7 d00p - 45,
	0x1 .780694f de5d38p - 1, -0x1.d000p - 50,
	0x1 .790 b938ac1d00p - 1, 0x1 .3000 p - 49,
	0x1 .7 a11473eb0178p - 1, -0x1.d000p - 49,
	0x1 .7 b17b0976d060p - 1, 0x1 .0400 p - 45,
	0x1 .7 c1ed0130c133p - 1, 0x1 .0000 p - 53,
	0x1 .7 d26a62ff8636p - 1, -0x1 .6900 p - 45,
	0x1 .7e2f 336 cf4e3bp - 1, -0x1 .2e00 p - 47,
	0x1 .7f 3878491 c3e8p - 1, -0x1 .4580 p - 45,
	0x1 .80427543e1 b4ep - 1, 0x1 .3000 p - 44,
	0x1 .814 d2add1071ap - 1, 0x1.f000p - 47,
	0x1 .82589994 ccd7ep - 1, -0x1 .1 c00p - 45,
	0x1 .8364 c1eb942d0p - 1, 0x1 .9 d00p - 45,
	0x1 .8471 a4623cab5p - 1, 0x1 .7100 p - 43,
	0x1 .857f 4179f 5 bbcp - 1, 0x1 .2600 p - 45,
	0x1 .868 d99b4491afp - 1, -0x1 .2 c40p - 44,
	0x1 .879 cad931a395p - 1, -0x1 .3000 p - 45,
	0x1 .88 ac7d98a65b8p - 1, -0x1.a800p - 45,
	0x1 .89 bd0a4785800p - 1, -0x1.d000p - 49,
	0x1 .8 ace5422aa223p - 1, 0x1 .3280 p - 44,
	0x1 .8 be05bad619fap - 1, 0x1 .2 b40p - 43,
	0x1 .8 cf3216b54383p - 1, -0x1.ed00p - 45,
	0x1 .8e06 a5e08664cp - 1, -0x1 .0500 p - 45,
	0x1 .8f 1 ae99157807p - 1, 0x1 .8280 p - 45,
	0x1 .902f ed0282c0ep - 1, -0x1.cb00p - 46,
	0x1 .9145 b0b91ff96p - 1, -0x1 .5e00 p - 47,
	0x1 .925 c353aa2ff9p - 1, 0x1 .5400 p - 48,
	0x1 .93737 b0cdc64ap - 1, 0x1 .7200 p - 46,
	0x1 .948 b82b5f98aep - 1, -0x1 .9000 p - 47,
	0x1 .95 a44cbc852cbp - 1, 0x1 .5680 p - 45,
	0x1 .96 bdd9a766f21p - 1, -0x1 .6 d00p - 44,
	0x1 .97 d829fde4e2ap - 1, -0x1 .1000 p - 47,
	0x1 .98f 33e47 a23a3p - 1, 0x1.d000p - 45,
	0x1 .9 a0f170ca0604p - 1, -0x1 .8 a40p - 44,
	0x1 .9 b2bb4d53ff89p - 1, 0x1 .55 c0p - 44,
	0x1 .9 c49182a3f15bp - 1, 0x1 .6 b80p - 45,
	0x1 .9 d674194bb8c5p - 1, -0x1.c000p - 49,
	0x1 .9e86319 e3238ep - 1, 0x1 .7 d00p - 46,
	0x1 .9f a5e8d07f302p - 1, 0x1 .6400 p - 46,
	0x1.a0c667b5de54dp - 1, -0x1 .5000 p - 48,
	0x1.a1e7aed8eb8f6p - 1, 0x1 .9e00 p - 47,
	0x1.a309bec4a2e27p - 1, 0x1.ad80p - 45,
	0x1.a42c980460a5dp - 1, -0x1.af00p - 46,
	0x1.a5503b23e259bp - 1, 0x1.b600p - 47,
	0x1.a674a8af46213p - 1, 0x1 .8880 p - 44,
	0x1.a799e1330b3a7p - 1, 0x1 .1200 p - 46,
	0x1.a8bfe53c12e8dp - 1, 0x1 .6 c00p - 47,
	0x1.a9e6b5579fcd2p - 1, -0x1 .9 b80p - 45,
	0x1.ab0e521356fb8p - 1, 0x1.b700p - 45,
	0x1.ac36bbfd3f381p - 1, 0x1 .9000 p - 50,
	0x1.ad5ff3a3c2780p - 1, 0x1 .4000 p - 49,
	0x1.ae89f995ad2a3p - 1, -0x1.c900p - 45,
	0x1.afb4ce622f367p - 1, 0x1 .6500 p - 46,
	0x1.b0e07298db790p - 1, 0x1.fd40p - 45,
	0x1.b20ce6c9a89a9p - 1, 0x1 .2700 p - 46,
	0x1.b33a2b84f1a4bp - 1, 0x1.d470p - 43,
	0x1.b468415b747e7p - 1, -0x1 .8380 p - 44,
	0x1.b59728de5593ap - 1, 0x1 .8000 p - 54,
	0x1.b6c6e29f1c56ap - 1, 0x1.ad00p - 47,
	0x1.b7f76f2fb5e50p - 1, 0x1.e800p - 50,
	0x1.b928cf22749b2p - 1, -0x1 .4 c00p - 47,
	0x1.ba5b030a10603p - 1, -0x1.d700p - 47,
	0x1.bb8e0b79a6f66p - 1, 0x1.d900p - 47,
	0x1.bcc1e904bc1ffp - 1, 0x1 .2 a00p - 47,
	0x1.bdf69c3f3a16fp - 1, -0x1.f780p - 46,
	0x1.bf2c25bd71db8p - 1, -0x1 .0 a00p - 46,
	0x1.c06286141b2e9p - 1, -0x1 .1400 p - 46,
	0x1.c199bdd8552e0p - 1, 0x1.be00p - 47,
	0x1.c2d1cd9fa64eep - 1, -0x1 .9400 p - 47,
	0x1.c40ab5fffd02fp - 1, -0x1.ed00p - 47,
	0x1.c544778fafd15p - 1, 0x1 .9660 p - 44,
	0x1.c67f12e57d0cbp - 1, -0x1.a100p - 46,
	0x1.c7ba88988c1b6p - 1, -0x1 .8458 p - 42,
	0x1.c8f6d9406e733p - 1, -0x1.a480p - 46,
	0x1.ca3405751c4dfp - 1, 0x1.b000p - 51,
	0x1.cb720dcef9094p - 1, 0x1 .1400 p - 47,
	0x1.ccb0f2e6d1689p - 1, 0x1 .0200 p - 48,
	0x1.cdf0b555dc412p - 1, 0x1 .3600 p - 48,
	0x1.cf3155b5bab3bp - 1, -0x1 .6900 p - 47,
	0x1.d072d4a0789bcp - 1, 0x1 .9 a00p - 47,
	0x1.d1b532b08c8fap - 1, -0x1 .5e00 p - 46,
	0x1.d2f87080d8a85p - 1, 0x1.d280p - 46,
	0x1.d43c8eacaa203p - 1, 0x1 .1 a00p - 47,
	0x1.d5818dcfba491p - 1, 0x1.f000p - 50,
	0x1.d6c76e862e6a1p - 1, -0x1 .3 a00p - 47,
	0x1.d80e316c9834ep - 1, -0x1.cd80p - 47,
	0x1.d955d71ff6090p - 1, 0x1 .4 c00p - 48,
	0x1.da9e603db32aep - 1, 0x1.f900p - 48,
	0x1.dbe7cd63a8325p - 1, 0x1 .9800 p - 49,
	0x1.dd321f301b445p - 1, -0x1 .5200 p - 48,
	0x1.de7d5641c05bfp - 1, -0x1.d700p - 46,
	0x1.dfc97337b9aecp - 1, -0x1 .6140 p - 46,
	0x1.e11676b197d5ep - 1, 0x1.b480p - 47,
	0x1.e264614f5a3e7p - 1, 0x1 .0 ce0p - 43,
	0x1.e3b333b16ee5cp - 1, 0x1.c680p - 47,
	0x1.e502ee78b3fb4p - 1, -0x1 .9300 p - 47,
	0x1.e653924676d68p - 1, -0x1 .5000 p - 49,
	0x1.e7a51fbc74c44p - 1, -0x1 .7f 80 p - 47,
	0x1.e8f7977cdb726p - 1, -0x1 .3700 p - 48,
	0x1.ea4afa2a490e8p - 1, 0x1 .5 d00p - 49,
	0x1.eb9f4867ccae4p - 1, 0x1 .61 a0p - 46,
	0x1.ecf482d8e680dp - 1, 0x1 .5500 p - 48,
	0x1.ee4aaa2188514p - 1, 0x1 .6400 p - 51,
	0x1.efa1bee615a13p - 1, -0x1.e800p - 49,
	0x1.f0f9c1cb64106p - 1, -0x1.a880p - 48,
	0x1.f252b376bb963p - 1, -0x1.c900p - 45,
	0x1.f3ac948dd7275p - 1, 0x1.a000p - 53,
	0x1.f50765b6e4524p - 1, -0x1 .4f 00 p - 48,
	0x1.f6632798844fdp - 1, 0x1.a800p - 51,
	0x1.f7bfdad9cbe38p - 1, 0x1.abc0p - 48,
	0x1.f91d802243c82p - 1, -0x1 .4600 p - 50,
	0x1.fa7c1819e908ep - 1, -0x1.b0c0p - 47,
	0x1.fbdba3692d511p - 1, -0x1 .0e00 p - 51,
	0x1.fd3c22b8f7194p - 1, -0x1 .0 de8p - 46,
	0x1.fe9d96b2a23eep - 1, 0x1.e430p - 49,
	0x1 .0000000000000 p + 0, 0x0 .0000 p + 0,
	0x1 .00 b1afa5abcbep + 0, -0x1 .3400 p - 52,
	0x1 .0163 da9fb3303p + 0, -0x1 .2170 p - 46,
	0x1 .02168143 b0282p + 0, 0x1.a400p - 52,
	0x1 .02 c9a3e77806cp + 0, 0x1.f980p - 49,
	0x1 .037 d42e11bbcap + 0, -0x1 .7400 p - 51,
	0x1 .04315e86 e7f89p + 0, 0x1 .8300 p - 50,
	0x1 .04e5f 72f 65467 p + 0, -0x1.a3f0p - 46,
	0x1 .059 b0d315855ap + 0, -0x1 .2840 p - 47,
	0x1 .0650 a0e3c1f95p + 0, 0x1 .1600 p - 48,
	0x1 .0706 b29ddf71ap + 0, 0x1 .5240 p - 46,
	0x1 .07 bd42b72a82dp + 0, -0x1 .9 a00p - 49,
	0x1 .0874518759 bd0p + 0, 0x1 .6400 p - 49,
	0x1 .092 bdf66607c8p + 0, -0x1 .0780 p - 47,
	0x1 .09e3 ecac6f383p + 0, -0x1 .8000 p - 54,
	0x1 .0 a9c79b1f3930p + 0, 0x1.fa00p - 48,
	0x1 .0 b5586cf988fcp + 0, -0x1.ac80p - 48,
	0x1 .0 c0f145e46c8ap + 0, 0x1 .9 c00p - 50,
	0x1 .0 cc922b724816p + 0, 0x1 .5200 p - 47,
	0x1 .0 d83b23395dd8p + 0, -0x1.ad00p - 48,
	0x1 .0e3 ec32d3d1f3p + 0, 0x1.bac0p - 46,
	0x1 .0ef a55fdfa9a6p + 0, -0x1 .4e80 p - 47,
	0x1 .0f b66affed2f0p + 0, -0x1.d300p - 47,
	0x1 .1073028 d7234bp + 0, 0x1 .1500 p - 48,
	0x1 .11301 d0125b5bp + 0, 0x1.c000p - 49,
	0x1 .11ed bab5e2af9p + 0, 0x1 .6 bc0p - 46,
	0x1 .12 abdc06c31d5p + 0, 0x1 .8400 p - 49,
	0x1 .136 a814f2047dp + 0, -0x1.ed00p - 47,
	0x1 .1429 aaea92de9p + 0, 0x1 .8e00 p - 49,
	0x1 .14e95934f 3138 p + 0, 0x1.b400p - 49,
	0x1 .15 a98c8a58e71p + 0, 0x1 .5300 p - 47,
	0x1 .166 a45471c3dfp + 0, 0x1 .3380 p - 47,
	0x1 .172 b83c7d5211p + 0, 0x1 .8 d40p - 45,
	0x1 .17ed48695 bb9fp + 0, -0x1 .5 d00p - 47,
	0x1 .18 af9388c8d93p + 0, -0x1.c880p - 46,
	0x1 .1972658375 d66p + 0, 0x1 .1f 00 p - 46,
	0x1 .1 a35beb6fcba7p + 0, 0x1 .0480 p - 46,
	0x1 .1 af99f81387e3p + 0, -0x1 .7390 p - 43,
	0x1 .1 bbe084045d54p + 0, 0x1 .4e40 p - 45,
	0x1 .1 c82f95281c43p + 0, -0x1.a200p - 47,
	0x1 .1 d4873168b9b2p + 0, 0x1 .3800 p - 49,
	0x1 .1e0 e75eb44031p + 0, 0x1.ac00p - 49,
	0x1 .1ed5022f cd938p + 0, 0x1 .1900 p - 47,
	0x1 .1f 9 c18438cdf7p + 0, -0x1.b780p - 46,
	0x1 .2063 b88628d8fp + 0, 0x1.d940p - 45,
	0x1 .212 be3578a81ep + 0, 0x1 .8000 p - 50,
	0x1 .21f 49917 ddd41p + 0, 0x1.b340p - 45,
	0x1 .22 bdda2791323p + 0, 0x1 .9f 80 p - 46,
	0x1 .2387 a6e7561e7p + 0, -0x1 .9 c80p - 46,
	0x1 .2451f fb821427p + 0, 0x1 .2300 p - 47,
	0x1 .251 ce4fb2a602p + 0, -0x1 .3480 p - 46,
	0x1 .25e85711 eceb0p + 0, 0x1 .2700 p - 46,
	0x1 .26 b4565e27d16p + 0, 0x1 .1 d00p - 46,
	0x1 .2780e341 de00fp + 0, 0x1 .1ee0 p - 44,
	0x1 .284 dfe1f5633ep + 0, -0x1 .4 c00p - 46,
	0x1 .291 ba7591bb30p + 0, -0x1 .3 d80p - 46,
	0x1 .29e9 df51fdf09p + 0, 0x1 .8 b00p - 47,
	0x1 .2 ab8a66d10e9bp + 0, -0x1 .27 c0p - 45,
	0x1 .2 b87fd0dada3ap + 0, 0x1.a340p - 45,
	0x1 .2 c57e39771af9p + 0, -0x1 .0800 p - 46,
	0x1 .2 d285a6e402d9p + 0, -0x1.ed00p - 47,
	0x1 .2 df961f641579p + 0, -0x1 .4200 p - 48,
	0x1 .2ec afa93e2ecfp + 0, -0x1 .4980 p - 45,
	0x1 .2f 9 d24abd8822p + 0, -0x1 .6300 p - 46,
	0x1 .306f e0a31b625p + 0, -0x1 .2360 p - 44,
	0x1 .31432ed eea50bp + 0, -0x1 .0 df8p - 40,
	0x1 .32170f c4cd7b8p + 0, -0x1 .2480 p - 45,
	0x1 .32eb83 ba8e9a2p + 0, -0x1 .5980 p - 45,
	0x1 .33 c08b2641766p + 0, 0x1.ed00p - 46,
	0x1 .3496266e3f a27p + 0, -0x1.c000p - 50,
	0x1 .356 c55f929f0fp + 0, -0x1 .0 d80p - 44,
	0x1 .36431 a2de88b9p + 0, 0x1 .2 c80p - 45,
	0x1 .371 a7373aaa39p + 0, 0x1 .0600 p - 45,
	0x1 .37f 26231e74f ep + 0, -0x1 .6600 p - 46,
	0x1 .38 cae6d05d838p + 0, -0x1.ae00p - 47,
	0x1 .39 a401b713ec3p + 0, -0x1 .4720 p - 43,
	0x1 .3 a7db34e5a020p + 0, 0x1 .8200 p - 47,
	0x1 .3 b57fbfec6e95p + 0, 0x1.e800p - 44,
	0x1 .3 c32dc313a8f2p + 0, 0x1.f800p - 49,
	0x1 .3 d0e544ede122p + 0, -0x1 .7 a00p - 46,
	0x1 .3 dea64c1234bbp + 0, 0x1 .6300 p - 45,
	0x1 .3ec70 df1c4eccp + 0, -0x1 .8 a60p - 43,
	0x1 .3f a4504ac7e8cp + 0, -0x1.cdc0p - 44,
	0x1 .40822 c367a0bbp + 0, 0x1 .5 b80p - 45,
	0x1 .4160 a21f72e95p + 0, 0x1.ec00p - 46,
	0x1 .423f b27094646p + 0, -0x1 .3600 p - 46,
	0x1 .431f 5 d950a920p + 0, 0x1 .3980 p - 45,
	0x1 .43f fa3f84b9ebp + 0, 0x1.a000p - 48,
	0x1 .44e0860618919 p + 0, -0x1 .6 c00p - 48,
	0x1 .45 c2042a7d201p + 0, -0x1.bc00p - 47,
	0x1 .46 a41ed1d0016p + 0, -0x1 .2800 p - 46,
	0x1 .4786 d668b3326p + 0, 0x1 .0e00 p - 44,
	0x1 .486 a2b5c13c00p + 0, -0x1.d400p - 45,
	0x1 .494e1 e192af04p + 0, 0x1.c200p - 47,
	0x1 .4 a32af0d7d372p + 0, -0x1.e500p - 46,
	0x1 .4 b17dea6db801p + 0, 0x1 .7800 p - 47,
	0x1 .4 bfdad53629e1p + 0, -0x1 .3800 p - 46,
	0x1 .4 ce41b817c132p + 0, 0x1 .0800 p - 47,
	0x1 .4 dcb299fddddbp + 0, 0x1.c700p - 45,
	0x1 .4eb2 d81d8ab96p + 0, -0x1.ce00p - 46,
	0x1 .4f 9 b2769d2d02p + 0, 0x1 .9200 p - 46,
	0x1 .508417f 4531 c1p + 0, -0x1 .8 c00p - 47,
	0x1 .516 daa2cf662ap + 0, -0x1.a000p - 48,
	0x1 .5257 de83f51eap + 0, 0x1.a080p - 43,
	0x1 .5342 b569d4edap + 0, -0x1 .6 d80p - 45,
	0x1 .542e2f 4f 6 ac1ap + 0, -0x1 .2440 p - 44,
	0x1 .551 a4ca5d94dbp + 0, 0x1 .83 c0p - 43,
	0x1 .56070 dde9116bp + 0, 0x1 .4 b00p - 45,
	0x1 .56f 4736 b529dep + 0, 0x1 .15 a0p - 43,
	0x1 .57e27 dbe2c40ep + 0, -0x1 .9e00 p - 45,
	0x1 .58 d12d497c76fp + 0, -0x1 .3080 p - 45,
	0x1 .59 c0827ff0b4cp + 0, 0x1.dec0p - 43,
	0x1 .5 ab07dd485427p + 0, -0x1 .4000 p - 51,
	0x1 .5 ba11fba87af4p + 0, 0x1 .0080 p - 44,
	0x1 .5 c9268a59460bp + 0, -0x1 .6 c80p - 45,
	0x1 .5 d84590998e3fp + 0, 0x1 .69 a0p - 43,
	0x1 .5e76f 15 ad20e1p + 0, -0x1.b400p - 46,
	0x1 .5f 6 a320dcebcap + 0, 0x1 .7700 p - 46,
	0x1 .605e1 b976dcb8p + 0, 0x1 .6f 80 p - 45,
	0x1 .6152 ae6cdf715p + 0, 0x1 .1000 p - 47,
	0x1 .6247eb03 a5531p + 0, -0x1 .5 d00p - 46,
	0x1 .633 dd1d1929b5p + 0, -0x1 .2 d00p - 46,
	0x1 .6434634 ccc313p + 0, -0x1.a800p - 49,
	0x1 .652 b9febc8efap + 0, -0x1 .8600 p - 45,
	0x1 .6623882553397 p + 0, 0x1 .1f e0p - 40,
	0x1 .671 c1c708328ep + 0, -0x1 .7200 p - 44,
	0x1 .68155 d44ca97ep + 0, 0x1 .6800 p - 49,
	0x1 .690f 4 b19e9471p + 0, -0x1 .9780 p - 45,
};

/************************************************************************
 * Public Functions
 ************************************************************************/

/*
 * exp2(x): compute the base 2 exponential of x
 *
 * Accuracy: Peak error < 0.503 ulp for normalized results.
 *
 * Method: (accurate tables)
 *
 *   Reduce x:
 *     x = k + y, for integer k and |y| <= 1/2.
 *     Thus we have exp2(x) = 2**k * exp2(y).
 *
 *   Reduce y:
 *     y = i/TBLSIZE + z - eps[i] for integer i near y * TBLSIZE.
 *     Thus we have exp2(y) = exp2(i/TBLSIZE) * exp2(z - eps[i]),
 *     with |z - eps[i]| <= 2**-9 + 2**-39 for the table used.
 *
 *   We compute exp2(i/TBLSIZE) via table lookup and exp2(z - eps[i]) via
 *   a degree-5 minimax polynomial with maximum error under 1.3 * 2**-61.
 *   The values in exp2t[] and eps[] are chosen such that
 *   exp2t[i] = exp2(i/TBLSIZE + eps[i]), and eps[i] is a small offset such
 *   that exp2t[i] is accurate to 2**-64.
 *
 *   Note that the range of i is +-TBLSIZE/2, so we actually index the tables
 *   by i0 = i + TBLSIZE/2.  For cache efficiency, exp2t[] and eps[] are
 *   virtual tables, interleaved in the real table tbl[].
 *
 *   This method is due to Gal, with many details due to Gal and Bachelis:
 *
 *      Gal, S. and Bachelis, B.  An Accurate Elementary Mathematical Library
 *      for the IEEE Floating Point Standard.  TOMS 17(1), 26-46 (1991).
 */
double exp2(double x)
{
	double r;
	double t;
	double z;
	uint32_t ix;
	uint32_t i0;
	union {
		double f;
		uint64_t i;
	} u = {
		x
	};
	union {
		uint32_t u;
		int32_t i;
	} k;

	/* Filter out exceptional cases. */
	ix = u.i >> 32 & 0x7fffffff;
	if (ix >= 0x408ff000) {		/* |x| >= 1022 or nan */
		if (ix >= 0x40900000 && u.i >> 63 == 0) {	/* x >= 1024 or nan */
			/* overflow */
			x *= 0x1 p1023;
			return x;
		}
		if (ix >= 0x7ff00000) {	/* -inf or -nan */
			return -1 / x;
		}
		if (u.i >> 63) {		/* x <= -1022 */
			/* underflow */
			if (x <= -1075 || x - 0x1 p52 + 0x1 p52 != x) {
				FORCE_EVAL((float)(-0x1 p - 149 / x));
			}
			if (x <= -1075) {
				return 0;
			}
		}
	} else if (ix < 0x3c900000) {	/* |x| < 0x1p-54 */
		return 1.0 + x;
	}

	/* Reduce x, computing z, i0, and k. */
	u.f = x + redux;
	i0 = u.i;
	i0 += TBLSIZE / 2;
	k.u = i0 / TBLSIZE * TBLSIZE;
	k.i /= TBLSIZE;
	i0 %= TBLSIZE;
	u.f -= redux;
	z = x - u.f;

	/* Compute r = exp2(y) = exp2t[i0] * p(z - eps[i]). */
	t = tbl[2 * i0];			/* exp2t[i0] */
	z -= tbl[2 * i0 + 1];		/* eps[i0]   */
	r = t + t * z * (P1 + z * (P2 + z * (P3 + z * (P4 + z * P5))));

	return scalbn(r, k.i);
}
#endif
