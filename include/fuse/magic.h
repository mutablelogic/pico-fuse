/** @file magic.h
 *  @brief Fuse magic numbers
 *
 *  This file contains the signatures for the magic numbers used
 */
#ifndef FUSE_MAGIC_H
#define FUSE_MAGIC_H

#include <stdint.h>

// Define the magic numbers for the values
#define FUSE_MAGIC_NULL 0x00  ///< NULL value
#define FUSE_MAGIC_APP 0x01   ///< Application
#define FUSE_MAGIC_BLOCK 0x02 ///< Memory block
#define FUSE_MAGIC_U8 0x03    ///< uint8_t
#define FUSE_MAGIC_U16 0x04   ///< uint16_t
#define FUSE_MAGIC_U32 0x05   ///< uint32_t
#define FUSE_MAGIC_U64 0x06   ///< uint64_t
#define FUSE_MAGIC_S8 0x07    ///< int8_t
#define FUSE_MAGIC_S16 0x08   ///< int16_t
#define FUSE_MAGIC_S32 0x09   ///< int32_t
#define FUSE_MAGIC_S64 0x0A   ///< int64_t
#define FUSE_MAGIC_F32 0x0B   ///< float32_t
#define FUSE_MAGIC_F64 0x0C   ///< float64_t
#define FUSE_MAGIC_BOOL 0x0D  ///< bool
#define FUSE_MAGIC_STR 0x0E   ///< string
#define FUSE_MAGIC_LIST 0x0F  ///< list[value]
#define FUSE_MAGIC_MAP 0x10   ///< map[value][value]

// Define the magic numbers for devices
#define FUSE_MAGIC_TIMER 0x20 ///< Timer source, which can fire at a regular interval
#define FUSE_MAGIC_LED 0x21   ///< On-board LED
#define FUSE_MAGIC_WIFI 0x22  ///< Wifi
#define FUSE_MAGIC_MQTT 0x23  ///< MQTT

// Maximum number of magic numbers
#define FUSE_MAGIC_COUNT 0x24 ///< Maximum number of magic numbers

// Define exit codes
#define FUSE_EXIT_SUCCESS 1     ///< Successful completion
#define FUSE_EXIT_MEMORYLEAKS 2 ///< Memory leaks detected in the application

/** @brief Convert a magic number to a string
 *
 *  @param magic The magic number
 *  @returns A zero-terminated string representation of the magic number
 */
const char *fuse_magic_cstr(uint16_t magic);

#endif /* FUSE_MAGIC_H */
