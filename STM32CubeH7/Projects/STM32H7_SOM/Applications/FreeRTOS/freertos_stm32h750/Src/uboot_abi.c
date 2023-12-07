#include <stdio.h>
#include <string.h>
#include "uboot_abi.h"

struct uboot_abi_s {
	unsigned int magic;
#define UBOOT_ABI_MAGIC		0xB00DB00D
	unsigned int version;
#define UBOOT_ABI_VERSION	0x00000001
	volatile void *gd;
	void (*prepare_abi)(void);
	int (*ubi_part)(char *part_name, const char *vid_header_offset);
	int (*ubifs_mount)(char *vol_name);
	int (*ubifs_umount)(void);
	int (*ubifs_read)(const char *filename, void *buf, loff_t offset, loff_t size, loff_t *actread);
	int (*ubifs_ls)(const char *filename);
	int (*ubifs_write)(const char *filename, void *buf, loff_t offset, loff_t size, loff_t *actwritten);
	int (*ubifs_mkdir)(const char *filename);
	int (*ubifs_rmdir)(const char *filename);
	int (*ubifs_unlink)(const char *filename);
	char * (*getenv)(const char *varname);
	int (*setenv)(const char *varname, const char *value);
	int (*saveenv)(void);
	int (*ubi_volume_write)(char *volume, void *buf, size_t size);
	int (*ubi_volume_read)(char *volume, void *buf, size_t size);
};

struct uboot_abi_s *uboot_abi = (void *)0x00000000; /* Function table is located at the start of ITCM */

int uboot_abi_check(void)
{
	return (uboot_abi->magic != UBOOT_ABI_MAGIC || uboot_abi->version != UBOOT_ABI_VERSION);
}

int uboot_abi_ubi_part(char *part_name, const char *vid_header_offset)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubi_part(part_name, vid_header_offset);
}

int uboot_abi_ubifs_mount(char *vol_name)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubifs_mount(vol_name);
}

void uboot_abi_ubifs_umount(void)
{
	uboot_abi->prepare_abi();
	uboot_abi->ubifs_umount();
}

int uboot_abi_ubifs_read(const char *filename, void *buf, loff_t offset, loff_t size, loff_t *actread)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubifs_read(filename, buf, offset, size, actread);
}

int uboot_abi_ubifs_ls(const char *filename)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubifs_ls(filename);
}

int uboot_abi_ubifs_write(const char *filename, void *buf, loff_t offset, loff_t size, loff_t *actwritten)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubifs_write(filename, buf, offset, size, actwritten);
}

int uboot_abi_ubifs_mkdir(const char *filename)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubifs_mkdir(filename);
}

int uboot_abi_ubifs_rmdir(const char *filename)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubifs_rmdir(filename);
}

int uboot_abi_ubifs_unlink(const char *filename)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubifs_unlink(filename);
}

char * uboot_abi_getenv(const char * varname)
{
	uboot_abi->prepare_abi();
	return uboot_abi->getenv(varname);
}

int uboot_abi_setenv(const char *varname, const char *value)
{
	uboot_abi->prepare_abi();
	return uboot_abi->setenv(varname, value);
}

int uboot_abi_saveenv(void)
{
	uboot_abi->prepare_abi();
	return uboot_abi->saveenv();
}

int ubi_volume_write(char *volume, void *buf, size_t size)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubi_volume_write(volume, buf, size);
}

int ubi_volume_read(char *volume, void *buf, size_t size)
{
	uboot_abi->prepare_abi();
	return uboot_abi->ubi_volume_read(volume, buf, size);
}
