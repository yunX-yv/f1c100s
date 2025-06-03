#!/bin/sh
UBOOT_FILE=./u-boot/u-boot-sunxi-with-spl.bin
DTB_FILE=./Linux/arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb
KERNEL_FILE=./Linux/arch/arm/boot/zImage
#ROOTFS_FILE=./buildroot-2021.02.4/output/images/rootfs.tar
ROOTFS_FILE=./root/rootfs.tar
MOD_FILE=./modules/lib/modules

echo "\e[31;47m Generate img \e[0m"
dd if=/dev/zero of=flashimg.bin bs=1M count=32 &&\

echo "\e[31;47m Burning uboot \e[0m"
echo "$UBOOT_FILE (size: $(stat -c %s $UBOOT_FILE | awk '{print $1/1024/1024 " MB"}'))"
dd if=$UBOOT_FILE of=flashimg.bin bs=1K conv=notrunc &&\

echo "\e[31;47m Burning dtb \e[0m"
echo "$DTB_FILE (size: $(stat -c %s $DTB_FILE | awk '{print $1/1024 " KB"}'))"
dd if=$DTB_FILE of=flashimg.bin bs=1K seek=1024 conv=notrunc &&\

echo "\e[31;47m Burning kernel \e[0m"
echo "$KERNEL_FILE (size: $(stat -c %s $KERNEL_FILE | awk '{print $1/1024/1024 " MB"}'))"
dd if=$KERNEL_FILE of=flashimg.bin bs=1K seek=1088 conv=notrunc &&\


mkdir rootfs
#tar -xvf $ROOTFS_FILE -C ./rootfs &&\
echo "tar -xvf $ROOTFS_FILE -C ./rootfs"
pv $ROOTFS_FILE | tar -xf - -C ./rootfs &&\
cp -r $MOD_FILE rootfs/lib/modules/ &&\

echo "export PS1='[\u@\h: \w\a\]$'" >> rootfs/etc/profile
echo "PermitRootLogin yes" >> rootfs/etc/ssh/sshd_config
cp runOnBoot rootfs/etc/init.d/runOnBoot
chmod +x rootfs/etc/init.d/runOnBoot
ln -s /etc/init.d/runOnBoot rootfs/etc/init.d/S99runOnBoot

echo "\e[31;47m Burning rootfs \e[0m"
#为根文件系统制作jffs2镜像包
#--pad参数指定 jffs2大小
#由此计算得到 0x1000000(16M)-0x10000(64K)-0x100000(1M)-0x400000(4M)=0xAF0000
mkfs.jffs2 -s 0x100 -e 0x10000 --pad=0x3AF0000 -d rootfs/ -o jffs2.img 
dd if=jffs2.img of=flashimg.bin bs=1K seek=5184 conv=notrunc  status=progress

echo "Generate img to $(pwd)/flashimg.bin. (size: $(stat -c %s flashimg.bin | awk '{print $1/1024/1024 " MB"}'))"


#titlo_size = stat -c %s "$file"
echo "\e[31;47m Backup rootfs \e[0m"
cd  rootfs
tar -cf ../rootfs.tar *
cd ..
cp rootfs.tar output/
echo "Optput rootfs to $(pwd)/output/rootfs.tar. (size: $(stat -c %s ./output/rootfs.tar | awk '{print $1/1024/1024 " MB"}'))"

rm -rf rootfs &&\
rm jffs2.img
