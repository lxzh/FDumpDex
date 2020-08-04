//
// Created by lxzh123 on 2018/4/8.
//
#include <sys/system_properties.h>
#include <stdlib.h>

#ifndef DUMPDEX_DEVICEUTIL_H
#define DUMPDEX_DEVICEUTIL_H

//#if defined(__aarch64__)
#define HOOK_DEF(ret, func, ...) \
  ret (*old_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)
//#else
//#define HOOK_DEF(ret, func, thiz, ...) \
//  ret (*orig_##func)(thiz, __VA_ARGS__); \
//  ret new_##func(thiz, __VA_ARGS__)
//#endif

char *get_open_elf_name();

char *get_open_symbol_name();

void **get_old_open_addr();

void *get_new_open_addr();

bool is_arm_64();

#endif //DUMPDEX_DEVICEUTIL_H