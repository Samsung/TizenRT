// Set up common variables for usage across the libjpeg-turbo modules

cc_defaults {
name: "libjpeg-defaults"
	,
cflags:
	[
		"-O3",
		"-fstrict-aliasing",

		"-Wno-sign-compare",
		"-Wno-unused-parameter",
		"-Werror",
	],
srcs:
	[
		"jcapimin.c",
		"jcapistd.c",
		"jaricom.c",
		"jcarith.c",
		"jccoefct.c",
		"jccolor.c",
		"jcdctmgr.c",
		"jchuff.c",
		"jcinit.c",
		"jcmainct.c",
		"jcmarker.c",
		"jcmaster.c",
		"jcomapi.c",
		"jcparam.c",
		"jcphuff.c",
		"jcprepct.c",
		"jcsample.c",
		"jctrans.c",
		"jdapimin.c",
		"jdapistd.c",
		"jdarith.c",
		"jdatadst.c",
		"jdatasrc.c",
		"jdcoefct.c",
		"jdcolor.c",
		"jddctmgr.c",
		"jdhuff.c",
		"jdinput.c",
		"jdmainct.c",
		"jdmarker.c",
		"jdmaster.c",
		"jdmerge.c",
		"jdphuff.c",
		"jdpostct.c",
		"jdsample.c",
		"jdtrans.c",
		"jerror.c",
		"jfdctflt.c",
		"jfdctfst.c",
		"jfdctint.c",
		"jidctflt.c",
		"jidctfst.c",
		"jidctint.c",
		"jidctred.c",
		"jmemmgr.c",
		"jmemnobs.c",
		"jquant1.c",
		"jquant2.c",
		"jutils.c",
	],

arch:
	{
arm:
		{
			// By default, the build system generates ARM target binaries in
			// thumb mode, where each instruction is 16 bits wide.  Defining
			// this variable as arm forces the build system to generate object
			// files in 32-bit arm mode.  This is the same setting previously
			// used by libjpeg and it provides a small performance benefit.
instruction_set: "arm"
			,
			// ARM v7 NEON
srcs:
			[
				"simd/jsimd_arm_neon.S",
				"simd/jsimd_arm.c",
			],
		},
arm64:
		{
			// ARM v8 64-bit NEON
srcs:
			[
				"simd/jsimd_arm64_neon.S",
				"simd/jsimd_arm64.c",
			],
		},
x86:
		{
			// x86 MMX and SSE2
srcs:
			[
				"simd/jsimd_i386.c",
				"simd/jccolor-mmx.asm",
				"simd/jccolor-sse2.asm",
				"simd/jcgray-mmx.asm",
				"simd/jcgray-sse2.asm",
				"simd/jchuff-sse2.asm",
				"simd/jcsample-mmx.asm",
				"simd/jcsample-sse2.asm",
				"simd/jdcolor-mmx.asm",
				"simd/jdcolor-sse2.asm",
				"simd/jdmerge-mmx.asm",
				"simd/jdmerge-sse2.asm",
				"simd/jdsample-mmx.asm",
				"simd/jdsample-sse2.asm",
				"simd/jfdctflt-3dn.asm",
				"simd/jfdctflt-sse.asm",
				"simd/jfdctfst-mmx.asm",
				"simd/jfdctfst-sse2.asm",
				"simd/jfdctint-mmx.asm",
				"simd/jfdctint-sse2.asm",
				"simd/jidctflt-3dn.asm",
				"simd/jidctflt-sse2.asm",
				"simd/jidctflt-sse.asm",
				"simd/jidctfst-mmx.asm",
				"simd/jidctfst-sse2.asm",
				"simd/jidctint-mmx.asm",
				"simd/jidctint-sse2.asm",
				"simd/jidctred-mmx.asm",
				"simd/jidctred-sse2.asm",
				"simd/jquant-3dn.asm",
				"simd/jquantf-sse2.asm",
				"simd/jquanti-sse2.asm",
				"simd/jquant-mmx.asm",
				"simd/jquant-sse.asm",
				"simd/jsimdcpu.asm",
			],
asflags:
			[
				"-DPIC",
				"-DELF",
			],
local_include_dirs:
			["simd"],
		},
x86_64:
		{
			// x86-64 SSE2
srcs:
			[
				"simd/jsimd_x86_64.c",
				"simd/jccolor-sse2-64.asm",
				"simd/jcgray-sse2-64.asm",
				"simd/jchuff-sse2-64.asm",
				"simd/jcsample-sse2-64.asm",
				"simd/jdcolor-sse2-64.asm",
				"simd/jdmerge-sse2-64.asm",
				"simd/jdsample-sse2-64.asm",
				"simd/jfdctflt-sse-64.asm",
				"simd/jfdctfst-sse2-64.asm",
				"simd/jfdctint-sse2-64.asm",
				"simd/jidctflt-sse2-64.asm",
				"simd/jidctfst-sse2-64.asm",
				"simd/jidctint-sse2-64.asm",
				"simd/jidctred-sse2-64.asm",
				"simd/jquantf-sse2-64.asm",
				"simd/jquanti-sse2-64.asm",
			],
asflags:
			[
				"-D__x86_64__",
				"-DPIC",
				"-DELF",
			],
local_include_dirs:
			["simd"],
		},
mips:
		{
srcs:
			["jsimd_none.c"],
		},
mips64:
		{
srcs:
			["jsimd_none.c"],
		},
	},
}

// Also build as a shared library.
cc_library {
name: "libjpeg"
	,
host_supported:
	true,
vendor_available:
	true,
target:
	{
		// TODO(b/67474260) Turn this back on
darwin:
		{
enabled:
			false,
		},
	},
vndk:
	{
enabled:
		true,
	},
defaults:
	["libjpeg-defaults"],
export_include_dirs:
	["."],
}

// Build static library against the NDK
cc_library_static {
name: "libjpeg_static_ndk"
	,
defaults:
	["libjpeg-defaults"],
export_include_dirs:
	["."],
sdk_version: "17"
	,
}

// Definition for TJBench
cc_binary {
name: "tjbench"
	,

whole_static_libs:
	["libjpeg"],

multilib:
	{
lib32:
		{
stem: "tj32"
			,
		},
lib64:
		{
stem: "tj64"
			,
		},
	},

compile_multilib: "both"
	,

cflags:
	[
		"-DBMP_SUPPORTED",
		"-DPPM_SUPPORTED",
		"-Wno-unused-parameter",
		"-Werror",
	],

srcs:
	[
		"tjbench.c",
		"bmp.c",
		"tjutil.c",
		"rdbmp.c",
		"rdppm.c",
		"wrbmp.c",
		"wrppm.c",
		"turbojpeg.c",
		"transupp.c",
		"jdatadst-tj.c",
		"jdatasrc-tj.c",
	],
}
