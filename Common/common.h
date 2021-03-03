#pragma once

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "global.h"

namespace common
{
	int is_exist(const char* path);
	int is_auth(const char* file, int mode);
	int create_dir(const char* dir);
	int check_date(char* _date);

	int file_open(FILE** pf, const char* level, const char* path);
	int file_write(pclient client, const char* time, const char* level, const char* data);
	int db_write(pclient client, int i_level, const char* time, const char* level, const char* data);

	int connect_in_or_out(pclient client, int flag);


};

#endif // _COMMON_H_

