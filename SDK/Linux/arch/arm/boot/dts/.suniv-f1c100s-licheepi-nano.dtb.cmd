cmd_arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb := mkdir -p arch/arm/boot/dts/ ; arm-linux-gnueabi-gcc -E -Wp,-MD,arch/arm/boot/dts/.suniv-f1c100s-licheepi-nano.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.suniv-f1c100s-licheepi-nano.dtb.dts.tmp arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb -b 0 -iarch/arm/boot/dts/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-simple_bus_reg -Wno-unit_address_format -Wno-pci_bridge -Wno-pci_device_bus_num -Wno-pci_device_reg  -d arch/arm/boot/dts/.suniv-f1c100s-licheepi-nano.dtb.d.dtc.tmp arch/arm/boot/dts/.suniv-f1c100s-licheepi-nano.dtb.dts.tmp ; cat arch/arm/boot/dts/.suniv-f1c100s-licheepi-nano.dtb.d.pre.tmp arch/arm/boot/dts/.suniv-f1c100s-licheepi-nano.dtb.d.dtc.tmp > arch/arm/boot/dts/.suniv-f1c100s-licheepi-nano.dtb.d

source_arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb := arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dts

deps_arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb := \
  arch/arm/boot/dts/suniv-f1c100s.dtsi \
  arch/arm/boot/dts/suniv.dtsi \
  scripts/dtc/include-prefixes/dt-bindings/clock/suniv-ccu.h \
  scripts/dtc/include-prefixes/dt-bindings/reset/suniv-ccu.h \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \

arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb: $(deps_arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb)

$(deps_arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb):
