

#ifndef UTILS_MD5_H_
#define UTILS_MD5_H_

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
    uint32_t total[2];          /*!< number of bytes processed  */
    uint32_t state[4];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
} iot_md5_context;


 /**
 * @brief 初始化MD5上下文
 *
 * @param ctx   MD5上下文指针
 */
void utils_md5_init(iot_md5_context *ctx);

/**
 * @brief 清空MD5上下文
 *
 * @param ctx   MD5上下文指针
 */
void utils_md5_free(iot_md5_context *ctx);

/**
 * @brief 拷贝MD5上下文
 *
 * @param dst   目标MD5上下文
 * @param src   源MD5上下文
 */
void utils_md5_clone(iot_md5_context *dst,
                     const iot_md5_context *src);

/**
 * @brief 设置MD5上下文
 *
 * @param ctx   MD5上下文指针
 */
void utils_md5_starts(iot_md5_context *ctx);

/**
 * @brief MD5过程缓冲区
 *
 * @param ctx MD5上下文指针
 * @param input    输入数据
 * @param ilen     输入数据长度
 */
void utils_md5_update(iot_md5_context *ctx, const unsigned char *input, size_t ilen);

/**
 * @brief          MD5数据
 *
 * @param ctx      MD5上下文指针
 * @param output   MD5校验和结果
 */
void utils_md5_finish(iot_md5_context *ctx, unsigned char output[16]);

/* 内部使用 */
void utils_md5_process(iot_md5_context *ctx, const unsigned char data[64]);

/**
 * @brief          Output = MD5( input buffer )
 *
 * @param input    输入数据
 * @param ilen     输入数据长度
 * @param output   MD5校验和结果
 */
void utils_md5(const unsigned char *input, size_t ilen, unsigned char output[16]);


int8_t utils_hb2hex(uint8_t hb);


#endif

