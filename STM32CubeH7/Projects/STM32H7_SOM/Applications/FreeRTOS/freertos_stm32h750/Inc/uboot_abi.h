#ifndef __UBOOT_ABI_H
#define __UBOOT_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t u32;
typedef uint64_t loff_t;

int uboot_abi_check(void);
int uboot_abi_ubi_part(char *part_name, const char *vid_header_offset);
int uboot_abi_ubifs_mount(char *vol_name);
void uboot_abi_ubifs_umount(void);
int uboot_abi_ubifs_read(const char *filename, void *buf, loff_t offset, loff_t size, loff_t *actread);
int uboot_abi_ubifs_ls(const char *filename);
int uboot_abi_ubifs_write(const char *filename, void *buf, loff_t offset, loff_t size, loff_t *actwritten);
int uboot_abi_ubifs_mkdir(const char *filename);
int uboot_abi_ubifs_rmdir(const char *filename);
int uboot_abi_ubifs_unlink(const char *filename);
char * uboot_abi_getenv(const char * varname);
int uboot_abi_setenv(const char *varname, const char *value);
int uboot_abi_saveenv(void);
int ubi_volume_write(char *volume, void *buf, size_t size);
int ubi_volume_read(char *volume, void *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __UBOOT_ABI_H */
