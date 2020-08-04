//
// Created by lxzh123 on 2018/3/23.
//
#include "util/common.h"
#include "native.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include "fake_dlfcn.h"

#ifdef __aarch64__
#include "A64Inlinehook/And64InlineHook.hpp"
#else
#include "Substrate/SubstrateHook.h"
#endif


#define JNIREG_CLASS "com/lxzh123/dumpdex/Native"//指定要注册的类

static inline void
hook_function(void *addr, void *new_func, void **old_func) {
#ifdef __aarch64__
    LOGD("hook_function with A64HookFunction");
    A64HookFunction(addr, new_func, old_func);
#else
    LOGD("hook_function with MSHookFunction");
    MSHookFunction(addr, new_func, old_func);
#endif
}

static bool (*old_IsMagicValid)(uint32_t magic) = NULL;
static bool new_IsMagicValid(uint32_t magic) {
    LOGD("new_IsMagicValid magic:%d", magic);
    return 0;
}


void printPlatform() {
#if defined(__LP64__)
    LOGD("printPlatform __LP64__");
#endif
#if defined(__aarch64__)
    LOGD("printPlatform __aarch64__");
#endif
#if defined(__x86_64__)
    LOGD("printPlatform __x86_64__");
#endif
#if defined(__x86_64)
    LOGD("printPlatform __x86_64");
#endif
#if defined(__arm__)
    LOGD("printPlatform __arm__");
#endif
#if defined(__i386__)
    LOGD("printPlatform __i386__");
#endif
}

void printMaps() {
    FILE *maps;
    char buff[256];
    int k, fd = -1, found = 0;
    maps = fopen("/proc/self/maps", "r");
    if (!maps) LOGE("failed to open maps");
    while (!found && fgets(buff, sizeof(buff), maps)) {
        LOGE("%s", buff);
    }
    fclose(maps);
}


JNIEXPORT void JNICALL dump(JNIEnv *env, jclass obj, jstring packageName) {
    LOGD("hook start");
    static bool is_hook = false;
    char *p = (char *) env->GetStringUTFChars(packageName, 0);
    LOGD("packageName %s", p);
    if (is_hook) {
        LOGD("hooked ignore");
        return;
    }
    init_package_name(p);
    env->ReleaseStringChars(packageName, (const jchar *) p);
    LOGD("hook start 1");
    char *elf_name = get_open_elf_name();
    void *handle = dlopen_ex(elf_name, RTLD_NOW);
    if (handle == NULL) {
        LOGD("Error: unable to find the SO : %s", elf_name);
        return;
    }
    LOGD("dlopen %s success", elf_name);
//    hookMagicValid(handle);
//    hookTest();
///*
//    void *open_common_addr = ndk_dlsym(handle, get_open_function_flag());
    char *symbol_name = get_open_symbol_name();
    void *open_common_addr = dlsym_ex(handle, symbol_name);
    if (open_common_addr == NULL) {
        LOGD("Error: unable to find the Symbol:%s", symbol_name);
        return;
    }
    LOGD("loaded so finished, start hook");
    void *new_func_addr = get_new_open_addr();
    void **old_func_addr = get_old_open_addr();
    LOGD("hook new_func_addr=%p, old_func_addr=%p", new_func_addr, old_func_addr);
    hook_function(open_common_addr, new_func_addr, old_func_addr);

//    hook_function(open_common_addr, (void *)new_q_open_common, (void **)&old_q_open_common);

//    void *open_with_magic_addr = dlsym_ex(handle, "tDexFileLoader13OpenWithMagicEjiRKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEbbPS7_PNS1_6vectorINS1_10unique_ptrIKNS_7DexFileENS1_14default_deleteISE_EEEENS5_ISH_EEEE");
//    hook_function(open_common_addr, (void *)new_q_open_with_magic, (void **)&old_q_open_with_magic);
//    printMaps();

    LOGD("hook init complete");
    is_hook = true;
}

JNIEXPORT void JNICALL printMap(JNIEnv *env, jclass obj) {
    (void)env;
    (void)obj;
    printMaps();
}

static JNINativeMethod gMethods[] = {
        {"dump", "(Ljava/lang/String;)V", (void *) dump},
//        {"printMap", "()V", (void *) printMap}
};

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass javaClass = env->FindClass(JNIREG_CLASS);
    if (javaClass == NULL) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(javaClass, gMethods, NELEM(gMethods)) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}