//
// Created by lxzh123 on 2018/4/8.
//


#include "deviceutils.h"
#include "fileutils.h"
#include "common.h"

//const static long DEX_MIN_LEN = 102400L;
const static long DEX_MIN_LEN = 8192L;
static int sdk_int = 0;

void init_sdk_init() {
    if (sdk_int != 0) {
        return;
    }
    char sdk[PROP_VALUE_MAX];
    __system_property_get("ro.build.version.sdk", sdk);
    sdk_int = atoi(sdk);
}

bool is_arm_64() {
#if defined(__aarch64__)
    return true;
#else
    return false;
#endif
}

bool is_x86_64() {
#if defined(__x86_64__)
    return true;
#else
    return false;
#endif
}

bool is_r() {
    return sdk_int == 30;
}

bool is_q() {
    return sdk_int == 29;
}

bool is_pie() {
    return sdk_int == 28;
}

bool is_oreo() {
    return sdk_int == 27 || sdk_int == 26;
}

bool is_nougat() {
    return sdk_int == 25 || sdk_int == 24;
}

bool is_marshmallow() {
    return sdk_int == 23;
}

char *get_open_elf_name() {
    init_sdk_init();
    if (is_q()) {
        return "libdexfile.so";
    } else {
        return "libart.so";
    }
}

//arm64

// other
static HOOK_DEF(void *, arm64_nougat_open_memory, uint8_t *base, size_t size, void *location,
                uint32_t location_checksum,
                void *mem_map, void *oat_dex_file, void *error_msg) {
    LOGI("new_arm64_open_memory base=%p, size=%u, location=%p, location_checksum=%u", base,
         size, location, location_checksum);
    if (size > DEX_MIN_LEN) {
        save_dex_file(base, size);
    }
    return (*old_arm64_nougat_open_memory)(base, size, location, location_checksum, mem_map,
                                           oat_dex_file, error_msg);
}

// o
static HOOK_DEF(void*, arm64_oreo_open_common, uint8_t *base, size_t size, void *location,
                uint32_t location_checksum, void *oat_dex_file,
                bool verify, bool verify_checksum,
                void *error_meessage, void *verify_result) {
    LOGI("new_arm64_oreo_open_common base=%u, size=%u, location=%u, location_checksum=%u", base,
         size, location, location_checksum);
    if (size > DEX_MIN_LEN) {
        save_dex_file(base, size);
    }
    void *result = old_arm64_oreo_open_common(base, size, location, location_checksum,
                                              oat_dex_file, verify, verify_checksum,
                                              error_meessage, verify_result);
    return result;
}

// p & q
static HOOK_DEF(void *, arm64_pq_open_common, uint8_t *base, size_t size, const uint8_t *data_base,
                size_t data_size,
                void *location, uint32_t location_checksum, void *oat_dex_file,
                bool verify, bool verify_checksum,
                void *error_msg, void *container, void *verify_result) {
    LOGI("new_arm64_pq_open_common enter");
    LOGI("new_arm64_pq_open_common base=%p, size=%u, data_base=%p, data_size=%u, location=%u, "
         "location_checksum=%u, oat_dex_file=%u, verify=%u, verify_checksum=%u, DEX_MIN_LEN=%u",
         base, size, data_base, data_size, location, location_checksum, oat_dex_file,
         verify, verify_checksum, DEX_MIN_LEN);
    if (size > DEX_MIN_LEN) {
        save_dex_file(base, size);
    }
    return (*old_arm64_pq_open_common)(base, size, data_base, data_size, location,
                                       location_checksum,
                                       oat_dex_file, verify, verify_checksum,
                                       error_msg, container, verify_result);
}

//arm32 x86 x86_64
/////////////////////
// other
static HOOK_DEF(void *, nougat_open_memory, void *DexFile_thiz, uint8_t *base, size_t size,
                void *location, uint32_t location_checksum,
                void *mem_map, void *oat_dex_file, void *error_msg) {
    LOGI("new_nougat_open_memory DexFile_thiz=%u base=%u, size=%u, location=%u, location_checksum=%u, oat_dex_file=%u",
         (uint32_t) DexFile_thiz, base, size, location, location_checksum, (uint32_t) oat_dex_file);
    if (size > DEX_MIN_LEN) {
        save_dex_file(base, size);
    }
    return (*old_nougat_open_memory)(DexFile_thiz, base, size, location, location_checksum, mem_map,
                                     oat_dex_file, error_msg);
}

// oreo
static HOOK_DEF(void *, oreo_open_common, void *DexFile_thiz, uint8_t *base, size_t size,
                void *location, uint32_t location_checksum, void *oat_dex_file,
                bool verify, bool verify_checksum,
                void *error_meessage, void *verify_result) {
    LOGI("new_oreo_open_common DexFile_thiz=%u base=%u, size=%u, location=%u, location_checksum=%u",
         (uint32_t) DexFile_thiz, base, size, location, location_checksum);
    if (size > DEX_MIN_LEN) {
        save_dex_file(base, size);
    }
    return (*old_oreo_open_common)(DexFile_thiz, base, size, location, location_checksum,
                                  oat_dex_file, verify, verify_checksum, error_meessage,
                                  verify_result);
}

/////////////////////
// p & q
static HOOK_DEF(void *, pq_open_common, void *DexFile_thiz, uint8_t *base, size_t size,
                const uint8_t *data_base, size_t data_size,
                void *location, uint32_t location_checksum,
                void *oat_dex_file, bool verify, bool verify_checksum,
                void *error_msg, void *container, void *verify_result) {
    LOGI("new_pq_open_common enter");
    LOGI("new_pq_open_common DexFile_thiz=%p, base=%p, size=%u, data_base=%p, data_size=%u, location=%u, "
         "location_checksum=%u, oat_dex_file=%u, verify=%u, verify_checksum=%u", DexFile_thiz, base,
         size,
         data_base, data_size, location, location_checksum, oat_dex_file, verify, verify_checksum);
    if (size > DEX_MIN_LEN) {
        save_dex_file(base, size);
    }
    return (*old_pq_open_common)(DexFile_thiz, base, size, data_base, data_size, location,
                                 location_checksum,
                                 oat_dex_file, verify, verify_checksum,
                                 error_msg, container,
                                 verify_result);
}

char *get_open_symbol_name() {
    init_sdk_init();
    LOGI("get_open_symbol_name is_arm_64=%u, is_marshmallow=%u, is_nougat=%u, is_oreo=%u, is_pie=%u, is_q=%u, is_r=%u",
         is_arm_64(), is_marshmallow(), is_nougat(), is_oreo(), is_pie(), is_q(), is_r());
    if (is_arm_64()) { // not test
        if (is_pie() || is_q()) {
            return "_ZN3art13DexFileLoader10OpenCommonEPKhmS2_mRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS0_12VerifyResultE";
        } else if (is_oreo()) {
            return "_ZN3art7DexFile10OpenCommonEPKhmRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_PNS0_12VerifyResultE";
        }
    } else {
        if (is_q()) {
            if (is_x86_64()) {
                // libdexfile.so art::DexFileLoader::OpenCommon x86_64 √
                return "_ZN3art13DexFileLoader10OpenCommonEPKhmS2_mRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS0_12VerifyResultE";
            } else {
                // libdexfile.so art::DexFileLoader::OpenCommon arm √
                return "_ZN3art13DexFileLoader10OpenCommonEPKhjS2_jRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS0_12VerifyResultE";
            }
        } else if (is_pie()) {
            if (is_x86_64()) {
                // libart.so art::ArtDexFileLoader::OpenCommon x86_64 √
                return "_ZN3art16ArtDexFileLoader10OpenCommonEPKhmS2_mRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS_13DexFileLoader12VerifyResultE";
            } else {
                // libart.so art::ArtDexFileLoader::OpenCommon arm √
                return "_ZN3art16ArtDexFileLoader10OpenCommonEPKhjS2_jRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS_13DexFileLoader12VerifyResultE";
            }
        } else if (is_oreo()) {
            // libart.so art::DexFile::OpenCommon
            // hook success, but didn't enter the target new function
            if(is_x86_64()) {
                return "_ZN3art7DexFile10OpenCommonEPKhmRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_PNS0_12VerifyResultE";
            } else {
                return "_ZN3art7DexFile10OpenCommonEPKhjRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_PNS0_12VerifyResultE";
            }
        }
    }
    return "";
}

void **get_old_open_addr() {
    if (is_arm_64()) {
        if (is_oreo()) {
            return reinterpret_cast<void **>(&old_arm64_oreo_open_common);
        } else if (is_pie() || is_q()) {
            return reinterpret_cast<void **>(&old_arm64_pq_open_common);
        } else {
            return reinterpret_cast<void **>(&old_arm64_nougat_open_memory);
        }
    } else {
        if (is_oreo()) {
            return reinterpret_cast<void **>(&old_oreo_open_common);
        } else if (is_pie() || is_q()) {
            return reinterpret_cast<void **>(&old_pq_open_common);
        } else {
            return reinterpret_cast<void **>(&old_nougat_open_memory);
        }
    }
}

void *get_new_open_addr() {
    if (is_arm_64()) {
        if (is_oreo()) {
            return reinterpret_cast<void *>(new_arm64_oreo_open_common);
        } else if (is_pie() || is_q()) {
            return reinterpret_cast<void *>(new_arm64_pq_open_common);
        } else {
            return reinterpret_cast<void *>(new_nougat_open_memory);
        }
    } else {
        if (is_oreo()) {
            return reinterpret_cast<void *>(new_oreo_open_common);
        } else if (is_pie() || is_q()) {
            return reinterpret_cast<void *>(new_pq_open_common);
        } else {
            return reinterpret_cast<void *>(new_nougat_open_memory);
        }
    }
}
