//
// Created by lxzh123 on 2018/4/8.
//


#include "fileutils.h"
#include "common.h"
#include <sys/stat.h>
#include <errno.h>

static char pname[256];

static void makedir(char *path) {
    if (access(path, F_OK) != 0) {
        if (mkdir(path, 0755) == -1) {
            LOGE("[-] mkdir %s error:%s", path, strerror(errno));
            return;
        }
    }
}

//生成dex文件
static void get_file_name(char *name, int len, int dexlen) {
    memset(name, 0, len);
    char path[256];
//    sprintf(path, "/sdcard/Android/data/%s/", pname);
//    makedir(path);
//    sprintf(path, "/sdcard/Android/data/%s/dex/", pname);
//    makedir(path);
//    sprintf(name, "/sdcard/Android/data/%s/dex/source-%u.dex", pname, dexlen);
    sprintf(name, "/sdcard/dumpDex/%s/dex/source-%u.dex", pname, dexlen);
//    sprintf(name, "/data/data/com.lxzh123.inlinehooktest/files/source-%u.dex", dexlen);
}

//保存dex文件
void save_dex_file(u_int8_t *data, size_t length) {
    char dname[1024];
    get_file_name(dname, sizeof(dname), length);
    void *p = fopen(dname, "r");
    if (p != NULL) {
        LOGI("%s file exist", dname);
        return;
    }
    LOGI("dump dex file name is : %s", dname);
    LOGI("start dump");
    int dex = open(dname, O_CREAT | O_WRONLY, 0644);
    if (dex < 0) {
        LOGE("open or create file error");
        return;
    }
    int ret = (int) write(dex, data, length);
    if (ret < 0) {
        LOGE("write file error");
    } else {
        LOGI("dump dex file success %s", dname);
    }
    close(dex);
}


void init_package_name(char *package_name) {
    strcpy(pname, package_name);
}