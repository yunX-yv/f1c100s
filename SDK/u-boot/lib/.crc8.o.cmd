cmd_lib/crc8.o := arm-linux-gnueabi-gcc -Wp,-MD,lib/.crc8.o.d -nostdinc -isystem /home/xiao/f1c100s/tools/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin/../lib/gcc/arm-linux-gnueabi/7.2.1/include -Iinclude    -I./arch/arm/thumb1/include -I./arch/arm/include -include ./include/linux/kconfig.h -D__KERNEL__ -D__UBOOT__ -Wall -Wstrict-prototypes -Wno-format-security -fno-builtin -ffreestanding -fshort-wchar -Os -fno-stack-protector -fno-delete-null-pointer-checks -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -D__ARM__ -Wa,-mimplicit-it=always -mthumb -mthumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -pipe -march=armv5te -D__LINUX_ARM_ARCH__=5 -I./arch/arm/mach-sunxi/include    -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(crc8)"  -D"KBUILD_MODNAME=KBUILD_STR(crc8)" -c -o lib/crc8.o lib/crc8.c

source_lib/crc8.o := lib/crc8.c

deps_lib/crc8.o := \
  include/linux/crc8.h \

lib/crc8.o: $(deps_lib/crc8.o)

$(deps_lib/crc8.o):
