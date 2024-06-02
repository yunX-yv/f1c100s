
CURRENT_DIR=$(cd $(dirname $0); pwd)

mkdir -p ${CURRENT_DIR}/image/

cp ${CURRENT_DIR}/arch/arm/boot/zImage ${CURRENT_DIR}/image/

cp ${CURRENT_DIR}/arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb ${CURRENT_DIR}/image/

echo "all kernel and DTB are copied to ${CURRENT_DIR}/image/"

