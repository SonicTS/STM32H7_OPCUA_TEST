#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "console.h"
#include "dma_alloc.h"
#include "uboot_abi.h"
#include "ubifs_test.h"

static int ubi_part(char *part_name, const char *vid_header_offset)
{
	return uboot_abi_ubi_part(part_name, vid_header_offset);
}

static int ubifs_mount(char *vol_name)
{
	return uboot_abi_ubifs_mount(vol_name);
}

static void ubifs_umount(void)
{
	uboot_abi_ubifs_umount();
}

static int ubifs_read(const char *filename, void *buf, loff_t offset,
		loff_t size, loff_t *actread)
{
	return uboot_abi_ubifs_read(filename, buf, offset, size, actread);
}

static int ubifs_ls(const char *filename)
{
	return uboot_abi_ubifs_ls(filename);
}

static int ubifs_write(const char *filename, void *buf, loff_t offset,
		loff_t size, loff_t *actwritten)
{
	return uboot_abi_ubifs_write(filename, buf, offset, size, actwritten);
}

static int ubifs_mkdir(const char *filename)
{
	return uboot_abi_ubifs_mkdir(filename);
}

static int ubifs_rmdir(const char *filename)
{
	return uboot_abi_ubifs_rmdir(filename);
}

static int ubifs_unlink(const char *filename)
{
	return uboot_abi_ubifs_unlink(filename);
}

BaseType_t cmd_ubipart_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	ubi_part((char *)args, NULL);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsmount_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	ubifs_mount((char *)args);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsumount_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	ubifs_umount();

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsls_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	ubifs_ls(args);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsload_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;
	char buf[128];
	void *addr;
	loff_t size = 0, actread;
	char *p;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	p = strchr(args, ' ');
	memcpy(buf, args, p - args);
	buf[p - args] = 0;
	sscanf(p + 1, "%p %llx", &addr, &size);

	ubifs_read(buf, addr, 0, size, &actread);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifswrite_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;
	char buf[128];
	void *addr;
	loff_t size = 0, actwritten;
	char *p;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	p = strchr(args, ' ');
	memcpy(buf, args, p - args);
	buf[p - args] = 0;
	sscanf(p + 1, "%p %llx", &addr, &size);

	if (size == 0) {
		strcpy(wBuf, "Incorrect size specified!");
		return pdFALSE;
	}

	ubifs_write(buf, addr, 0, size, &actwritten);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsmkdir_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	ubifs_mkdir(args);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsrmdir_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	ubifs_rmdir(args);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsunlink_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + sizeof(__func__) - 9;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot UBIFS API is not available!");
		return pdFALSE;
	}

	ubifs_unlink(args);

	*wBuf = 0;

	return pdFALSE;
}

BaseType_t cmd_ubifsstress_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	int idx, ret;

	for(idx = 0; idx < 20; idx++) {
		if ((ret = ubifs_mount("ubi0:rootfs"))) {
			console_printf("can't mount ubifs %i\n", ret);
			break;
		}

		console_printf("ls /\n");
		ubifs_ls("/");

		{
			char filename2[128] = "/test";
			char dirname[128] = "/test_dir";
			char *fileContent = (void *)0xd1100000;
			char *fileContent2 = (void *)0xd1200000;
			loff_t actread, off, actwritten;
			int i, j;

			for (i = 0; i < 10; i++) {
				snprintf(dirname, sizeof(dirname), "/test_dir%02i", i);
				ubifs_mkdir(dirname);

				for (j = 0; j < 10; j++) {
					snprintf(filename2, sizeof(filename2), "%s/file%02i", dirname, j);
					memset(fileContent, 0x0, 1024*1024);
					off = 4096*j;
					memset(&fileContent[0] + off, 'j', 10*(j + 1));
					if(ubifs_write(filename2, (void *)(&fileContent[0] + off), off, 10*(j + 1), &actwritten)) {
						console_printf("write error\n");
					}
				}
			}
			console_printf("ls /\n");
			ubifs_ls("/");
			for (i = 0; i < 10; i++) {
				snprintf(dirname, sizeof(dirname), "/test_dir%02i", i);
				console_printf("%s\n", dirname);
				ubifs_ls(dirname);

				for (j = 0; j < 10; j++) {
					snprintf(filename2, sizeof(filename2), "%s/file%02i", dirname, j);
					memset(fileContent, 0x0, 1024*1024);
					off = 4096*j;
					memset(&fileContent[0] + off, 'j', 10*(j + 1));
					memset(fileContent2, 0x0, 1024*1024);
					if(ubifs_read(filename2, (void *)(&fileContent2[0]), (u32)0, (u32)off + 10*(j+1), &actread)) {
						console_printf("%s read error\n", filename2);
					}
					if (memcmp(fileContent2, fileContent, off + 10*(j + 1))) {
						console_printf("%s content error\n", filename2);
					}
					if (ubifs_unlink(filename2)) {
						console_printf("%s unlink error\n", filename2);
					}
				}
				console_printf("%s\n", dirname);
				ubifs_ls(dirname);
				console_printf("ls /\n");
				ubifs_ls("/");

				if (ubifs_rmdir(dirname)) {
					console_printf("%s rmdir error\n", dirname);
				}
#if 0
				if (ubifs_umount()) {
					console_printf("umount error\n");
					exit(-1);
				}
				if (ubifs_mount(VOLUME_NAME_DEFAULT)) {
					console_printf("mount error\n");
					break;
				}
#endif
				console_printf("ls /\n");
				ubifs_ls("/");
			}

			if(ubifs_write("/iterationCount", (void *)(&idx), 0, 4, &actwritten)) {
				console_printf("ERROR: ubifs_write-iteration:%d, file:iteration\n", idx);
			}

			console_printf("ls /\n");
			ubifs_ls("/");
			ubifs_umount();
		}
	}


	*wBuf = 0;

	return pdFALSE;
}
