/*
 * common.h
 * Misc functions used in idevicerestore
 *
 * Copyright (c) 2012-2019 Nikias Bassen. All Rights Reserved.
 * Copyright (c) 2012 Martin Szulecki. All Rights Reserved.
 * Copyright (c) 2010 Joshua Hill. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef IDEVICERESTORE_COMMON_H
#define IDEVICERESTORE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define PACKAGE_VERSION "1.0"

#include <unistd.h>

#include <plist/plist.h>
#include <libirecovery.h>

#include "idevicerestore.h"
#include "thread.h"

#define MODE_UNKNOWN        -1
#define MODE_WTF             0
#define MODE_DFU             1
#define MODE_RECOVERY        2
#define MODE_RESTORE         3
#define MODE_NORMAL          4

#define FLAG_QUIT            1

#define CPFM_FLAG_SECURITY_MODE 1 << 0
#define CPFM_FLAG_PRODUCTION_MODE 1 << 1

#define IBOOT_FLAG_IMAGE4_AWARE  1 << 2
#define IBOOT_FLAG_EFFECTIVE_SECURITY_MODE 1 << 3
#define IBOOT_FLAG_EFFECTIVE_PRODUCTION_MODE 1 << 4

#define USER_AGENT_STRING "InetURL/1.0"

struct dfu_client_t;
struct normal_client_t;
struct restore_client_t;
struct recovery_client_t;

struct idevicerestore_mode_t {
	int index;
	const char* string;
};

struct idevicerestore_entry_t {
	char* name;
	char* path;
	char* filename;
	char* blob_data;
	uint32_t blob_size;
	struct idevicerestore_entry* next;
	struct idevicerestore_entry* prev;
};

struct idevicerestore_client_t;

extern struct idevicerestore_mode_t idevicerestore_modes[];

extern int idevicerestore_debug;

void info(const char* format, ...);
void error(const char* format, ...);
void debug(const char* format, ...);

void debug_plist(plist_t plist);
void print_progress_bar(double progress);
int read_file(const char* filename, void** data, size_t* size);
int write_file(const char* filename, const void* data, size_t size);

char *generate_guid(void);

#ifdef WIN32
#include <windows.h>
#include <unistd.h>
#define __mkdir(path, mode) mkdir(path)
#define FMT_qu "%I64u"
#define FMT_016llx "%016I64x"
#ifndef sleep
#define sleep(x) Sleep(x*1000)
#endif
#define __usleep(x) Sleep(x/1000)
#else
#include <sys/stat.h>
#define __mkdir(path, mode) mkdir(path, mode)
#define FMT_qu "%qu"
#define FMT_016llx "%016llx"
#define __usleep(x) usleep(x)
#endif

int mkdir_with_parents(const char *dir, int mode);

char *get_temp_filename(const char *prefix);

void idevicerestore_progress(struct idevicerestore_client_t* client, int step, double progress);

#ifndef HAVE_STRSEP
char* strsep(char** strp, const char* delim);
#endif

#ifndef HAVE_REALPATH
char* realpath(const char *filename, char *resolved_name);
#endif

void get_user_input(char *buf, int maxlen, int secure);

uint8_t _plist_dict_get_bool(plist_t dict, const char *key);
uint64_t _plist_dict_get_uint(plist_t dict, const char *key);

#ifdef __cplusplus
}
#endif

#endif
