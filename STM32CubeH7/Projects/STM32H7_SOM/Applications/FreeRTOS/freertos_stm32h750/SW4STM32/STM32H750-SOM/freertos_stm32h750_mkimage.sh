BNAME=$1
ENTRYP=`readelf -h "$BNAME".elf | grep -e "Entry point address" | sed -e 's/ Entry point address: //' | awk '{$1=$1};1'`
LADDR=`readelf -s "$BNAME".elf | grep g_pfnVectors | awk '{print $2}'`

cat << EOF > "$BNAME".its
/dts-v1/;

/ {
	description = "FreeRTOS Boot Image for STM32H7-SOM";
	#address-cells = <1>;

	images {
		kernel {
			description = "FreeRTOS Image";
			data = /incbin/("$BNAME.bin");
			type = "kernel";
			arch = "arm";
			os = "linux";
			compression = "none";
			load = <0x$LADDR>;
			entry = <$ENTRYP>;
			hash-1 {
			       algo = "sha1";
			};
		};
	};

	configurations {
		default = "config-1";
		config-1 {
			description = "FreeRTOS Boot Image";
			kernel = "kernel";
			signature {
				algo = "sha1,rsa2048";
				key-name-hint = "dev";
				sign-images = "kernel";
			};
		};
	};
};
EOF
if [ ! -e ~/keys/dev.key ]; then
    echo "$0: No keys to sign the image, exiting"
    exit -1
fi
mkimage -f "$BNAME".its "$BNAME".img -k ~/keys
exit $?