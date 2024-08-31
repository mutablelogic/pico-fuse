/** @file magic.h
 *  @brief Fuse magic numbers
 *
 *  This file contains the signatures for the magic numbers used
 */
#ifndef FUSE_MAGIC_H
#define FUSE_MAGIC_H

// Define the magic numbers for the values
#define FUSE_MAGIC_NULL  0x00  ///< NULL value
#define FUSE_MAGIC_APP   0x01  ///< Application
#define FUSE_MAGIC_MUTEX 0x02  ///< Mutex
#define FUSE_MAGIC_DATA  0x03  ///< Memory block
#define FUSE_MAGIC_U8    0x04  ///< uint8_t
#define FUSE_MAGIC_U16   0x05  ///< uint16_t
#define FUSE_MAGIC_U32   0x06  ///< uint32_t
#define FUSE_MAGIC_U64   0x07  ///< uint64_t
#define FUSE_MAGIC_S8    0x08  ///< int8_t
#define FUSE_MAGIC_S16   0x09  ///< int16_t
#define FUSE_MAGIC_S32   0x0A  ///< int32_t
#define FUSE_MAGIC_S64   0x0B  ///< int64_t
#define FUSE_MAGIC_F32   0x0C  ///< float
#define FUSE_MAGIC_F64   0x0D  ///< double
#define FUSE_MAGIC_BOOL  0x0E  ///< bool
#define FUSE_MAGIC_CSTR  0x0F  ///< constant string with null terminator
#define FUSE_MAGIC_LIST  0x10  ///< list[value]
#define FUSE_MAGIC_MAP   0x11  ///< map[value][value]
#define FUSE_MAGIC_TIMER 0x13  ///< Timer source, which can fire at a regular interval
#define FUSE_MAGIC_LED   0x14  ///< On-board LED
#define FUSE_MAGIC_WIFI  0x15  ///< Wifi
#define FUSE_MAGIC_MQTT  0x16  ///< MQTT

// Maximum number of magic numbers
#define FUSE_MAGIC_COUNT 0x17 ///< Maximum number of magic numbers

// Define exit codes
#define FUSE_EXIT_SUCCESS     1 ///< Successful completion
#define FUSE_EXIT_MEMORYLEAKS 2 ///< Memory leaks detected in the application

#endif /* FUSE_MAGIC_H */
