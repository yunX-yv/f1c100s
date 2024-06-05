	.arch armv5te
	.eabi_attribute 20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute 21, 1	@ Tag_ABI_FP_exceptions
	.eabi_attribute 23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute 24, 1	@ Tag_ABI_align8_needed
	.eabi_attribute 25, 1	@ Tag_ABI_align8_preserved
	.eabi_attribute 26, 2	@ Tag_ABI_enum_size
	.eabi_attribute 30, 2	@ Tag_ABI_optimization_goals
	.eabi_attribute 34, 0	@ Tag_CPU_unaligned_access
	.eabi_attribute 18, 2	@ Tag_ABI_PCS_wchar_t
	.file	"asm-offsets.c"
@ GNU C89 (Linaro GCC 7.2-2017.11) version 7.2.1 20171011 (arm-linux-gnueabi)
@	compiled by GNU C version 4.8.4, GMP version 6.1.2, MPFR version 3.1.5, MPC version 1.0.3, isl version none
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed:  -nostdinc -I ./arch/arm/include
@ -I ./arch/arm/include/generated -I ./include -I ./arch/arm/include/uapi
@ -I ./arch/arm/include/generated/uapi -I ./include/uapi
@ -I ./include/generated/uapi -imultilib . -imultiarch arm-linux-gnueabi
@ -iprefix /home/xiao/f1c100s/tools/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin/../lib/gcc/arm-linux-gnueabi/7.2.1/
@ -isysroot /home/xiao/f1c100s/tools/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin/../arm-linux-gnueabi/libc
@ -D __KERNEL__ -D __LINUX_ARM_ARCH__=5 -U arm -D CC_HAVE_ASM_GOTO
@ -D KBUILD_BASENAME="asm_offsets" -D KBUILD_MODNAME="asm_offsets"
@ -isystem /home/xiao/f1c100s/tools/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin/../lib/gcc/arm-linux-gnueabi/7.2.1/include
@ -include ./include/linux/kconfig.h -MD arch/arm/kernel/.asm-offsets.s.d
@ arch/arm/kernel/asm-offsets.c -mlittle-endian -mabi=aapcs-linux
@ -mno-thumb-interwork -mfpu=vfp -marm -march=armv5te -mtune=arm9tdmi
@ -mfloat-abi=soft -mtls-dialect=gnu
@ -auxbase-strip arch/arm/kernel/asm-offsets.s -O2 -Wall -Wundef
@ -Wstrict-prototypes -Wno-trigraphs -Werror=implicit-function-declaration
@ -Wno-format-security -Wno-frame-address -Wformat-truncation=0
@ -Wformat-overflow=0 -Wno-int-in-bool-context -Wframe-larger-than=1024
@ -Wno-unused-but-set-variable -Wunused-const-variable=0
@ -Wdeclaration-after-statement -Wno-pointer-sign -Werror=implicit-int
@ -Werror=strict-prototypes -Werror=date-time
@ -Werror=incompatible-pointer-types -Werror=designated-init -std=gnu90
@ -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE
@ -fno-dwarf2-cfi-asm -fno-ipa-sra -funwind-tables
@ -fno-delete-null-pointer-checks -fno-stack-protector -fomit-frame-pointer
@ -fno-var-tracking-assignments -fno-strict-overflow -fstack-check=no
@ -fconserve-stack -fverbose-asm --param allow-store-data-races=0
@ options enabled:  -faggressive-loop-optimizations -falign-jumps
@ -falign-labels -falign-loops -fauto-inc-dec -fbranch-count-reg
@ -fcaller-saves -fchkp-check-incomplete-type -fchkp-check-read
@ -fchkp-check-write -fchkp-instrument-calls -fchkp-narrow-bounds
@ -fchkp-optimize -fchkp-store-bounds -fchkp-use-static-bounds
@ -fchkp-use-static-const-bounds -fchkp-use-wrappers -fcode-hoisting
@ -fcombine-stack-adjustments -fcompare-elim -fcprop-registers
@ -fcrossjumping -fcse-follow-jumps -fdefer-pop -fdevirtualize
@ -fdevirtualize-speculatively -fearly-inlining
@ -feliminate-unused-debug-types -fexpensive-optimizations
@ -fforward-propagate -ffp-int-builtin-inexact -ffunction-cse -fgcse
@ -fgcse-lm -fgnu-runtime -fgnu-unique -fguess-branch-probability
@ -fhoist-adjacent-loads -fident -fif-conversion -fif-conversion2
@ -findirect-inlining -finline -finline-atomics
@ -finline-functions-called-once -finline-small-functions -fipa-bit-cp
@ -fipa-cp -fipa-icf -fipa-icf-functions -fipa-icf-variables -fipa-profile
@ -fipa-pure-const -fipa-ra -fipa-reference -fipa-vrp -fira-hoist-pressure
@ -fira-share-save-slots -fira-share-spill-slots
@ -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
@ -fleading-underscore -flifetime-dse -flra-remat -flto-odr-type-merging
@ -fmath-errno -fmerge-constants -fmerge-debug-strings
@ -fmove-loop-invariants -fomit-frame-pointer -foptimize-sibling-calls
@ -foptimize-strlen -fpartial-inlining -fpeephole -fpeephole2 -fplt
@ -fprefetch-loop-arrays -freg-struct-return -freorder-blocks
@ -freorder-functions -frerun-cse-after-loop
@ -fsched-critical-path-heuristic -fsched-dep-count-heuristic
@ -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
@ -fsched-pressure -fsched-rank-heuristic -fsched-spec
@ -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-insns
@ -fschedule-insns2 -fsection-anchors -fsemantic-interposition
@ -fshow-column -fshrink-wrap -fshrink-wrap-separate -fsigned-zeros
@ -fsplit-ivs-in-unroller -fsplit-wide-types -fssa-backprop -fssa-phiopt
@ -fstdarg-opt -fstore-merging -fstrict-volatile-bitfields -fsync-libcalls
@ -fthread-jumps -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
@ -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
@ -ftree-copy-prop -ftree-cselim -ftree-dce -ftree-dominator-opts
@ -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-if-convert
@ -ftree-loop-im -ftree-loop-ivcanon -ftree-loop-optimize
@ -ftree-parallelize-loops= -ftree-phiprop -ftree-pre -ftree-pta
@ -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slsr -ftree-sra
@ -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vrp
@ -funit-at-a-time -funwind-tables -fverbose-asm -fzero-initialized-in-bss
@ -marm -mglibc -mlittle-endian -mpic-data-is-text-relative -msched-prolog
@ -mvectorize-with-neon-quad

	.syntax divided
	.macro	it, cond
	.endm
	.macro	itt, cond
	.endm
	.macro	ite, cond
	.endm
	.macro	ittt, cond
	.endm
	.macro	itte, cond
	.endm
	.macro	itet, cond
	.endm
	.macro	itee, cond
	.endm
	.macro	itttt, cond
	.endm
	.macro	ittte, cond
	.endm
	.macro	ittet, cond
	.endm
	.macro	ittee, cond
	.endm
	.macro	itett, cond
	.endm
	.macro	itete, cond
	.endm
	.macro	iteet, cond
	.endm
	.macro	iteee, cond
	.endm

	.arm
	.syntax unified
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu softvfp
	.type	main, %function
main:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
@ arch/arm/kernel/asm-offsets.c:63:   DEFINE(TSK_ACTIVE_MM,		offsetof(struct task_struct, active_mm));
	.syntax divided
@ 63 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TSK_ACTIVE_MM #360 offsetof(struct task_struct, active_mm)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:67:   BLANK();
@ 67 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:68:   DEFINE(TI_FLAGS,		offsetof(struct thread_info, flags));
@ 68 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_FLAGS #0 offsetof(struct thread_info, flags)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:69:   DEFINE(TI_PREEMPT,		offsetof(struct thread_info, preempt_count));
@ 69 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_PREEMPT #4 offsetof(struct thread_info, preempt_count)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:70:   DEFINE(TI_ADDR_LIMIT,		offsetof(struct thread_info, addr_limit));
@ 70 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_ADDR_LIMIT #8 offsetof(struct thread_info, addr_limit)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:71:   DEFINE(TI_TASK,		offsetof(struct thread_info, task));
@ 71 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_TASK #12 offsetof(struct thread_info, task)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:72:   DEFINE(TI_CPU,		offsetof(struct thread_info, cpu));
@ 72 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_CPU #16 offsetof(struct thread_info, cpu)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:73:   DEFINE(TI_CPU_DOMAIN,		offsetof(struct thread_info, cpu_domain));
@ 73 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_CPU_DOMAIN #20 offsetof(struct thread_info, cpu_domain)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:74:   DEFINE(TI_CPU_SAVE,		offsetof(struct thread_info, cpu_context));
@ 74 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_CPU_SAVE #24 offsetof(struct thread_info, cpu_context)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:75:   DEFINE(TI_USED_CP,		offsetof(struct thread_info, used_cp));
@ 75 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_USED_CP #76 offsetof(struct thread_info, used_cp)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:76:   DEFINE(TI_TP_VALUE,		offsetof(struct thread_info, tp_value));
@ 76 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_TP_VALUE #92 offsetof(struct thread_info, tp_value)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:77:   DEFINE(TI_FPSTATE,		offsetof(struct thread_info, fpstate));
@ 77 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->TI_FPSTATE #104 offsetof(struct thread_info, fpstate)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:93:   BLANK();
@ 93 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:94:   DEFINE(S_R0,			offsetof(struct pt_regs, ARM_r0));
@ 94 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R0 #0 offsetof(struct pt_regs, ARM_r0)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:95:   DEFINE(S_R1,			offsetof(struct pt_regs, ARM_r1));
@ 95 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R1 #4 offsetof(struct pt_regs, ARM_r1)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:96:   DEFINE(S_R2,			offsetof(struct pt_regs, ARM_r2));
@ 96 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R2 #8 offsetof(struct pt_regs, ARM_r2)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:97:   DEFINE(S_R3,			offsetof(struct pt_regs, ARM_r3));
@ 97 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R3 #12 offsetof(struct pt_regs, ARM_r3)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:98:   DEFINE(S_R4,			offsetof(struct pt_regs, ARM_r4));
@ 98 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R4 #16 offsetof(struct pt_regs, ARM_r4)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:99:   DEFINE(S_R5,			offsetof(struct pt_regs, ARM_r5));
@ 99 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R5 #20 offsetof(struct pt_regs, ARM_r5)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:100:   DEFINE(S_R6,			offsetof(struct pt_regs, ARM_r6));
@ 100 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R6 #24 offsetof(struct pt_regs, ARM_r6)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:101:   DEFINE(S_R7,			offsetof(struct pt_regs, ARM_r7));
@ 101 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R7 #28 offsetof(struct pt_regs, ARM_r7)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:102:   DEFINE(S_R8,			offsetof(struct pt_regs, ARM_r8));
@ 102 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R8 #32 offsetof(struct pt_regs, ARM_r8)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:103:   DEFINE(S_R9,			offsetof(struct pt_regs, ARM_r9));
@ 103 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R9 #36 offsetof(struct pt_regs, ARM_r9)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:104:   DEFINE(S_R10,			offsetof(struct pt_regs, ARM_r10));
@ 104 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_R10 #40 offsetof(struct pt_regs, ARM_r10)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:105:   DEFINE(S_FP,			offsetof(struct pt_regs, ARM_fp));
@ 105 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_FP #44 offsetof(struct pt_regs, ARM_fp)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:106:   DEFINE(S_IP,			offsetof(struct pt_regs, ARM_ip));
@ 106 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_IP #48 offsetof(struct pt_regs, ARM_ip)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:107:   DEFINE(S_SP,			offsetof(struct pt_regs, ARM_sp));
@ 107 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_SP #52 offsetof(struct pt_regs, ARM_sp)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:108:   DEFINE(S_LR,			offsetof(struct pt_regs, ARM_lr));
@ 108 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_LR #56 offsetof(struct pt_regs, ARM_lr)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:109:   DEFINE(S_PC,			offsetof(struct pt_regs, ARM_pc));
@ 109 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_PC #60 offsetof(struct pt_regs, ARM_pc)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:110:   DEFINE(S_PSR,			offsetof(struct pt_regs, ARM_cpsr));
@ 110 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_PSR #64 offsetof(struct pt_regs, ARM_cpsr)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:111:   DEFINE(S_OLD_R0,		offsetof(struct pt_regs, ARM_ORIG_r0));
@ 111 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->S_OLD_R0 #68 offsetof(struct pt_regs, ARM_ORIG_r0)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:112:   DEFINE(PT_REGS_SIZE,		sizeof(struct pt_regs));
@ 112 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->PT_REGS_SIZE #72 sizeof(struct pt_regs)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:113:   DEFINE(SVC_DACR,		offsetof(struct svc_pt_regs, dacr));
@ 113 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SVC_DACR #72 offsetof(struct svc_pt_regs, dacr)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:114:   DEFINE(SVC_ADDR_LIMIT,	offsetof(struct svc_pt_regs, addr_limit));
@ 114 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SVC_ADDR_LIMIT #76 offsetof(struct svc_pt_regs, addr_limit)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:115:   DEFINE(SVC_REGS_SIZE,		sizeof(struct svc_pt_regs));
@ 115 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SVC_REGS_SIZE #80 sizeof(struct svc_pt_regs)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:116:   BLANK();
@ 116 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:117:   DEFINE(SIGFRAME_RC3_OFFSET,	offsetof(struct sigframe, retcode[3]));
@ 117 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SIGFRAME_RC3_OFFSET #756 offsetof(struct sigframe, retcode[3])"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:118:   DEFINE(RT_SIGFRAME_RC3_OFFSET, offsetof(struct rt_sigframe, sig.retcode[3]));
@ 118 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->RT_SIGFRAME_RC3_OFFSET #884 offsetof(struct rt_sigframe, sig.retcode[3])"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:119:   BLANK();
@ 119 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:135:   DEFINE(VMA_VM_MM,		offsetof(struct vm_area_struct, vm_mm));
@ 135 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->VMA_VM_MM #32 offsetof(struct vm_area_struct, vm_mm)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:136:   DEFINE(VMA_VM_FLAGS,		offsetof(struct vm_area_struct, vm_flags));
@ 136 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->VMA_VM_FLAGS #40 offsetof(struct vm_area_struct, vm_flags)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:137:   BLANK();
@ 137 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:138:   DEFINE(VM_EXEC,	       	VM_EXEC);
@ 138 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->VM_EXEC #4 VM_EXEC"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:139:   BLANK();
@ 139 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:140:   DEFINE(PAGE_SZ,	       	PAGE_SIZE);
@ 140 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->PAGE_SZ #4096 PAGE_SIZE"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:141:   BLANK();
@ 141 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:142:   DEFINE(SYS_ERROR0,		0x9f0000);
@ 142 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SYS_ERROR0 #10420224 0x9f0000"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:143:   BLANK();
@ 143 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:144:   DEFINE(SIZEOF_MACHINE_DESC,	sizeof(struct machine_desc));
@ 144 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SIZEOF_MACHINE_DESC #104 sizeof(struct machine_desc)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:145:   DEFINE(MACHINFO_TYPE,		offsetof(struct machine_desc, nr));
@ 145 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->MACHINFO_TYPE #0 offsetof(struct machine_desc, nr)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:146:   DEFINE(MACHINFO_NAME,		offsetof(struct machine_desc, name));
@ 146 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->MACHINFO_NAME #4 offsetof(struct machine_desc, name)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:147:   BLANK();
@ 147 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:148:   DEFINE(PROC_INFO_SZ,		sizeof(struct proc_info_list));
@ 148 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->PROC_INFO_SZ #52 sizeof(struct proc_info_list)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:149:   DEFINE(PROCINFO_INITFUNC,	offsetof(struct proc_info_list, __cpu_flush));
@ 149 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->PROCINFO_INITFUNC #16 offsetof(struct proc_info_list, __cpu_flush)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:150:   DEFINE(PROCINFO_MM_MMUFLAGS,	offsetof(struct proc_info_list, __cpu_mm_mmu_flags));
@ 150 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->PROCINFO_MM_MMUFLAGS #8 offsetof(struct proc_info_list, __cpu_mm_mmu_flags)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:151:   DEFINE(PROCINFO_IO_MMUFLAGS,	offsetof(struct proc_info_list, __cpu_io_mmu_flags));
@ 151 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->PROCINFO_IO_MMUFLAGS #12 offsetof(struct proc_info_list, __cpu_io_mmu_flags)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:152:   BLANK();
@ 152 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:168:   DEFINE(SLEEP_SAVE_SP_SZ,	sizeof(struct sleep_save_sp));
@ 168 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SLEEP_SAVE_SP_SZ #8 sizeof(struct sleep_save_sp)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:169:   DEFINE(SLEEP_SAVE_SP_PHYS,	offsetof(struct sleep_save_sp, save_ptr_stash_phys));
@ 169 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SLEEP_SAVE_SP_PHYS #4 offsetof(struct sleep_save_sp, save_ptr_stash_phys)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:170:   DEFINE(SLEEP_SAVE_SP_VIRT,	offsetof(struct sleep_save_sp, save_ptr_stash));
@ 170 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->SLEEP_SAVE_SP_VIRT #0 offsetof(struct sleep_save_sp, save_ptr_stash)"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:172:   BLANK();
@ 172 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:173:   DEFINE(DMA_BIDIRECTIONAL,	DMA_BIDIRECTIONAL);
@ 173 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->DMA_BIDIRECTIONAL #0 DMA_BIDIRECTIONAL"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:174:   DEFINE(DMA_TO_DEVICE,		DMA_TO_DEVICE);
@ 174 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->DMA_TO_DEVICE #1 DMA_TO_DEVICE"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:175:   DEFINE(DMA_FROM_DEVICE,	DMA_FROM_DEVICE);
@ 175 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->DMA_FROM_DEVICE #2 DMA_FROM_DEVICE"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:176:   BLANK();
@ 176 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:177:   DEFINE(CACHE_WRITEBACK_ORDER, __CACHE_WRITEBACK_ORDER);
@ 177 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->CACHE_WRITEBACK_ORDER #6 __CACHE_WRITEBACK_ORDER"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:178:   DEFINE(CACHE_WRITEBACK_GRANULE, __CACHE_WRITEBACK_GRANULE);
@ 178 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->CACHE_WRITEBACK_GRANULE #64 __CACHE_WRITEBACK_GRANULE"	@
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:179:   BLANK();
@ 179 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:187:   BLANK();
@ 187 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:191:   BLANK();
@ 191 "arch/arm/kernel/asm-offsets.c" 1
	
.ascii "->"
@ 0 "" 2
@ arch/arm/kernel/asm-offsets.c:202: }
	.arm
	.syntax unified
	mov	r0, #0	@,
	bx	lr	@
	.fnend
	.size	main, .-main
	.ident	"GCC: (Linaro GCC 7.2-2017.11) 7.2.1 20171011"
	.section	.note.GNU-stack,"",%progbits
