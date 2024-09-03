/** @file magic.h
 *  @brief Fuse magic numbers
 *
 *  This file contains the signatures for the magic numbers used
 */
#ifndef FUSE_MAGIC_H
#define FUSE_MAGIC_H

// Define the magic numbers for the value types
#define FUSE_MAGIC_NULL 0x00  ///< NULL value
#define FUSE_MAGIC_APP 0x01   ///< Application
#define FUSE_MAGIC_MUTEX 0x02 ///< Mutex
#define FUSE_MAGIC_EVENT 0x03 ///< Mutex
#define FUSE_MAGIC_DATA 0x04  ///< Memory block
#define FUSE_MAGIC_U8 0x05    ///< uint8_t
#define FUSE_MAGIC_U16 0x06   ///< uint16_t
#define FUSE_MAGIC_U32 0x07   ///< uint32_t
#define FUSE_MAGIC_U64 0x08   ///< uint64_t
#define FUSE_MAGIC_S8 0x09    ///< int8_t
#define FUSE_MAGIC_S16 0x0A   ///< int16_t
#define FUSE_MAGIC_S32 0x0B   ///< int32_t
#define FUSE_MAGIC_S64 0x0C   ///< int64_t
#define FUSE_MAGIC_F32 0x0D   ///< float
#define FUSE_MAGIC_F64 0x0E   ///< double
#define FUSE_MAGIC_BOOL 0x0F  ///< bool
#define FUSE_MAGIC_CSTR 0x10  ///< constant string with null terminator
#define FUSE_MAGIC_LIST 0x11  ///< list[value]
#define FUSE_MAGIC_MAP 0x12   ///< map[value][value]
#define FUSE_MAGIC_TIMER 0x13 ///< Timer source, which can fire at a regular interval
#define FUSE_MAGIC_GPIO 0x14  ///< GPIO pins
#define FUSE_MAGIC_LED 0x15   ///< On-board LED
#define FUSE_MAGIC_ADC 0x16   ///< On-board ADC
#define FUSE_MAGIC_PWM 0x17   ///< On-board PWM
#define FUSE_MAGIC_I2C 0x18   ///< On-board I2C
#define FUSE_MAGIC_SPI 0x19   ///< On-board SPI
#define FUSE_MAGIC_WIFI 0x1A  ///< Pico W Wifi
#define FUSE_MAGIC_MQTT 0x1B  ///< Pico W MQTT

// Maximum number of magic numbers
#define FUSE_MAGIC_COUNT 0x1C ///< Maximum number of magic numbers

// Define exit codes
#define FUSE_EXIT_SUCCESS 1     ///< Successful completion
#define FUSE_EXIT_MEMORYLEAKS 2 ///< Memory leaks detected in the application

#endif /* FUSE_MAGIC_H */
