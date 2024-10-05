#include <picofuse/picofuse.h>
#include "bme280.h"


#define READ_BIT                                  0x80
#define WRITE_MASK                                0x7F
#define MODE_MASK                                 0x03
#define MEASURING_BIT                             0x08
#define UPDATING_BIT                              0x01

/* Register Addresses */
#define BME280_CHIP_ID_ADDR                       0xD0
#define BME280_RESET_ADDR                         0xE0
#define BME280_TEMP_PRESS_CALIB_DATA_ADDR         0x88
#define BME280_HUMIDITY_CALIB_DATA_ADDR           0xE1
#define BME280_PWR_CTRL_ADDR                      0xF4
#define BME280_CTRL_HUM_ADDR                      0xF2
#define BME280_STATUS_ADDR                        0xF3
#define BME280_CTRL_MEAS_ADDR                     0xF4
#define BME280_CONFIG_ADDR                        0xF5
#define BME280_DATA_ADDR                          0xF7

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Read a BME280 register
 */
static void bme280_read_register(fuse_t* self, fuse_spi_t* spi, uint8_t reg, uint8_t *buf, size_t len) {
    assert(self);
    assert(spi);
    assert(buf);
    assert(len);

    buf[0] = reg | READ_BIT;
    assert(fuse_spi_xfr(self, spi, buf, sizeof(uint8_t), len, true));
}

/** @brief Write a BME280 register with an 8-bit value
 */
static void bme280_write_register(fuse_t* self, fuse_spi_t* spi, uint8_t reg, uint8_t value) {
    assert(self);
    assert(spi);
    
    uint8_t buf[2];
    buf[0] = reg & WRITE_MASK;
    buf[1] = value;
    assert(fuse_spi_xfr(self, spi, buf, sizeof(uint8_t) * 2, 0, true));
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Read the BME280 chip id
 */
uint8_t bme280_read_chip_id(fuse_t* self, fuse_bme280_t* bme280) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);

    uint8_t id;
    bme280_read_register(self, bme280->spi, BME280_CHIP_ID_ADDR, &id, sizeof(uint8_t));
    return id;
}

/** @brief Read the BME280 device status
 */
void bme280_read_status(fuse_t* self, fuse_bme280_t* bme280, bool* measuring, bool* updating) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);

    uint8_t status;
    bme280_read_register(self, bme280->spi, BME280_STATUS_ADDR, &status, sizeof(uint8_t));

    if(measuring) {
        *measuring = (status & MEASURING_BIT) ? true : false;
    }
    if(updating) {
        *updating = (status & UPDATING_BIT) ? true : false;
    }
}

/** @brief Reset the BME280
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 */
void bme280_reset(fuse_t* self, fuse_bme280_t* bme280) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);

    // Soft reset
    bme280_write_register(self, bme280->spi, BME280_RESET_ADDR, 0xB6);

    // Wait for 2ms for the reset to complete
    bool updating;
    uint8_t n = 3;
    do {
        sleep_ms(2);
        bme280_read_status(self, bme280, NULL, &updating);
    } while(n-- && updating);
}

/** @brief Convert oversampling value
 * 
 */
inline uint8_t bme280_convert_osrs_to(uint8_t osrs) {
    switch (osrs) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 4:
            return 3;
        case 8:
            return 4;
        case 16:
            return 5;
        default:
            return 0;
    }
}

/** @brief Set oversampling values
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param osrs_t Temperature oversampling value (0, 1, 2, 4, 8 or 16)
 * @param osrs_p Pressure oversampling value (0, 1, 2, 4, 8 or 16)
 * @param osrs_h Humidity oversampling value (0, 1, 2, 4, 8 or 16)
 */
void bme280_write_osrs(fuse_t* self, fuse_bme280_t* bme280, uint8_t osrs_t, uint8_t osrs_p, uint8_t osrs_h) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);
    assert(osrs_t == 0 || osrs_t == 1 || osrs_t == 2 || osrs_t == 4 || osrs_t == 8 || osrs_t == 16);
    assert(osrs_p == 0 || osrs_p == 1 || osrs_p == 2 || osrs_p == 4 || osrs_p == 8 || osrs_p == 16);
    assert(osrs_h == 0 || osrs_h == 1 || osrs_h == 2 || osrs_h == 4 || osrs_h == 8 || osrs_h == 16);

    // Get the mode
    uint8_t mode = bme280_read_mode(self, bme280);
    assert(mode == 0 || mode == 1 || mode == 2);

    // Write the humidity oversampling value
    bme280_write_register(self, bme280->spi, BME280_CTRL_HUM_ADDR, bme280_convert_osrs_to(osrs_h));

    // Write the temperature and pressure oversampling values, and apply the humidity oversampling value
    bme280_write_register(self, bme280->spi, BME280_CTRL_MEAS_ADDR, (bme280_convert_osrs_to(osrs_t) << 5) | (bme280_convert_osrs_to(osrs_p) << 2) | (mode & MODE_MASK));
}

/** @brief Read measurement mode
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @return The measurement mode (0 for sleep, 1 for forced, 2 for normal)
 */
uint8_t bme280_read_mode(fuse_t* self, fuse_bme280_t* bme280) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);

    uint8_t mode;
    bme280_read_register(self, bme280->spi, BME280_CTRL_MEAS_ADDR, &mode, sizeof(uint8_t));
    return mode & MODE_MASK;
}

/** @brief Write measurement mode
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param mode  The measurement mode (0 for sleep, 1 for forced, 2 for normal)
 */
void bme280_write_mode(fuse_t* self, fuse_bme280_t* bme280, uint8_t mode) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);
    assert(mode == 0 || mode == 1 || mode == 2);

    uint8_t meas;
    bme280_read_register(self, bme280->spi, BME280_CTRL_MEAS_ADDR, &meas, sizeof(uint8_t));
    bme280_write_register(self, bme280->spi, BME280_CTRL_MEAS_ADDR, mode | (meas & ~MODE_MASK));
}

/** @brief Read the BME280 compensation parameters
 */
void bme280_read_compensation_parameters(fuse_t* self, fuse_bme280_t* bme280) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);

    uint8_t buffer[26];
    bme280_read_register(self, bme280->spi, BME280_TEMP_PRESS_CALIB_DATA_ADDR, buffer, sizeof(uint8_t) * 26);

    // Set temperature compensation parameters
    bme280->T1 = buffer[0] | (buffer[1] << 8);
    bme280->T2 = buffer[2] | (buffer[3] << 8);
    bme280->T3 = buffer[4] | (buffer[5] << 8);

    // Set pressure compensation parameters
    bme280->P1 = buffer[6] | (buffer[7] << 8);
    bme280->P2 = buffer[8] | (buffer[9] << 8);
    bme280->P3 = buffer[10] | (buffer[11] << 8);
    bme280->P4 = buffer[12] | (buffer[13] << 8);
    bme280->P5 = buffer[14] | (buffer[15] << 8);
    bme280->P6 = buffer[16] | (buffer[17] << 8);
    bme280->P7 = buffer[18] | (buffer[19] << 8);
    bme280->P8 = buffer[20] | (buffer[21] << 8);
    bme280->P9 = buffer[22] | (buffer[23] << 8);
    bme280->H1 = buffer[25]; // 0xA1
    
    // Read humidity compensation parameters
    bme280_read_register(self, bme280->spi, BME280_HUMIDITY_CALIB_DATA_ADDR, buffer, sizeof(uint8_t) * 8);

    // Set humidity compensation parameters
    bme280->H2 = buffer[0] | (buffer[1] << 8); // 0xE1 | 0xE2
    bme280->H3 = (int8_t)buffer[2]; // 0xE3
    bme280->H4 = buffer[3] << 4 | (buffer[4] & 0xf); // 0xE4 | 0xE5[3:0]
    bme280->H5 = (buffer[4] >> 4) | (buffer[5] << 4); // 0xE5[7:4] | 0xE6
    bme280->H6 = (int8_t)buffer[6]; // 0xE7    
}

/** @brief Read the BME280 registers as raw values
 * 
 * Read raw values, and must be converted to the correct units using the compensation parameters
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param temperature Pointer to the temperature value, or NULL
 * @param pressure Pointer to the pressure value, or NULL
 * @param humidity Pointer to the humidity value, or NULL
 */
void bme280_read_raw(fuse_t* self, fuse_bme280_t* bme280, int32_t *temperature,int32_t *pressure,int32_t *humidity) {
    assert(self);
    assert(bme280);
    assert(bme280->spi);

    // If the mode is sleep, then set to forced
    uint8_t mode = bme280_read_mode(self, bme280);
    if (mode == 0) {
        bme280_write_mode(self, bme280, 1);
    }

    uint8_t buffer[8];
    bme280_read_register(self, bme280->spi, BME280_DATA_ADDR, buffer, sizeof(uint8_t) * 8);

    // Set the values
    if (temperature) {
        *temperature = ((uint32_t) buffer[3] << 12) | ((uint32_t) buffer[4] << 4) | (buffer[5] >> 4);
    }
    if (pressure) {
        *pressure = ((uint32_t) buffer[0] << 12) | ((uint32_t) buffer[1] << 4) | (buffer[2] >> 4);
    }
    if(humidity) {
        *humidity = ((uint32_t) buffer[6] << 8) | buffer[7];
    }
}

/** @brief Compensate temperature value
 *
 * Provided with a raw temperature value, and this function will return the compensated temperature value
 * and optionally the intermediate temperature coefficient for other calculations. The output temperature
 * is in degrees Celsius multiplied by 100
 * 
 * @param bme280 The BME280 instance
 * @param temperature The raw temperature value
 * @param fine A pointer to the intermediate temperature coefficient, or NULL
 * @return The compensated temperature value
 */
int32_t bme280_compensate_temperature(fuse_bme280_t* bme280, int32_t temperature, int32_t* fine) {
    assert(bme280);

    int32_t var1 = ((((temperature >> 3) - ((int32_t) bme280->T1 << 1))) * ((int32_t) bme280->T2)) >> 11;
    int32_t var2 = (((((temperature >> 4) - ((int32_t) bme280->T1)) * ((temperature >> 4) - ((int32_t) bme280->T1))) >> 12) * ((int32_t) bme280->T3)) >> 14;
    int32_t t_fine = var1 + var2;
    if (fine) {
        *fine = t_fine;
    }
    return (t_fine * 5 + 128) >> 8;
}

/** @brief Compensate pressure value
 *
 * Provided with a raw pressure value and intermediate temperature coefficient,  this function will 
 * return the compensated pressure value. The output pressure is in Pascals
 * 
 * @param bme280 The BME280 instance
 * @param pressure The raw pressure value
 * @param fine The intermediate temperature coefficient
 * @return The compensated pressure value
 */
uint32_t bme280_compensate_pressure(fuse_bme280_t* bme280, int32_t pressure, int32_t fine) {
    assert(bme280);

    int32_t var1 = (((int32_t)fine) >> 1) - (int32_t) 64000;
    int32_t var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) bme280->P6);
    var2 = var2 + ((var1 * ((int32_t) bme280->P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t) bme280->P4) << 16);
    var1 = (((bme280->P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t) bme280->P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t) bme280->P1)) >> 15);
    if (var1 == 0)
        return 0;

    uint32_t p = (((uint32_t) (((int32_t) 1048576) - pressure) - (var2 >> 12))) * 3125;
    if (p < 0x80000000) {
        p = (p << 1) / ((uint32_t) var1);
    } else {
        p = (p / (uint32_t) var1) * 2;
    }

    var1 = (((int32_t) bme280->P9) * ((int32_t) (((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t) (p >> 2)) * ((int32_t) bme280->P8)) >> 13;
    return  (uint32_t) ((int32_t) p + ((var1 + var2 + bme280->P7) >> 4));
}

/** @brief Compensate humidity value
 *
 * Provided with a raw humidity value,  this function will return the compensated humidity value. The output humidity 
 * should be divided by 1024 to get the value in %RH
 * 
 * @param bme280 The BME280 instance
 * @param humidity The raw humidity value
 * @param fine The intermediate temperature coefficient
 * @return The compensated humidity value
 */
uint32_t bme280_compensate_humidity(fuse_bme280_t* bme280, int32_t humidity, int32_t fine) {
    assert(bme280);

    int32_t v_x1_u32r = (fine - ((int32_t) 76800));
    v_x1_u32r = (((((humidity << 14) - (((int32_t) bme280->H4) << 20) - (((int32_t) bme280->H5) * v_x1_u32r)) +
                   ((int32_t) 16384)) >> 15) * (((((((v_x1_u32r * ((int32_t) bme280->H6)) >> 10) * (((v_x1_u32r *
                                                                                                  ((int32_t) bme280->H3))
            >> 11) + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) *
                                                 ((int32_t) bme280->H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) bme280->H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    return (uint32_t) (v_x1_u32r >> 12);
}
