/**
 * This file was generated from "original/strawberry.bmp" using...
 *
 *	file2c -dcs original/strawberry.bmp romfs/strawberry.h
 *
 */
static const char strawberry[] = {
	0x42, 0x4D, 0xCA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8A, 0x00, 0x00, 0x00, 0x6C, 0x00,
	0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x02, 0x00,
	0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x80, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x02, 0x11, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x01, 0x31, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x11, 0x11, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,
	0x00, 0x01, 0x31, 0x31, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x01, 0x11, 0x11,
	0x13, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x11, 0x31, 0x31, 0x11, 0x11, 0x00, 0x00,
	0x00, 0x00, 0x02, 0x00, 0x06, 0x11, 0x00, 0x06, 0x31, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,
	0x00, 0x13, 0x13, 0x12, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x01, 0x11, 0x22,
	0x23, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x22, 0x20, 0x22, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x01,
};

#ifdef ROMFS_DIRENTRY_HEAD
	static const ROMFS_DIRENTRY strawberry_dir = { 0, 0, ROMFS_DIRENTRY_HEAD, "strawberry.bmp", 270, strawberry };
	#undef ROMFS_DIRENTRY_HEAD
	#define ROMFS_DIRENTRY_HEAD &strawberry_dir
#endif
