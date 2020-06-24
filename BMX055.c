#include "air-brakes.h"
#include "bma2x2.h"
#include "bmm150.h"

typedef struct bmm150_dev BMX055_MAG;
BMX055_MAG magnetometer;
bool magnetometerInitialised = false;

typedef struct bma2x2_accel_data_temp BMX055_XYZ_TEMP;
bool accelerometerInitialised = false;

bool fileMode = false;
bool fileInitalised = false;

DATA_POINT *heights;
int numberOfHeights = 0;

clock_t t1, t2;
bool first = true;
DATA_POINT previousData;

void checkError(int result, char *message)
{
    if (result != BMM150_OK)
    {
        printf("Error in setup process: %s\n", message);
        exit(-1);
    }
}

int getFakeCurrentHeightQuadratic(int time, int maxTime, int maxHeight)
{
    // Quadratic: roots of 0 and maxTime, height of h
    return -(4 * time * (time - maxTime) * maxHeight) / (pow(maxTime, 2.0));
}

const char *getfield(char *line, int num)
{
    const char *tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

int initialiseFile(char *fileName, int timeColumn, int altColumn, int speedColumn, int accelerationColumn)
{
    fileMode = true;

    FILE *stream = fopen(fileName, "r");
    char line[3000];
    heights = malloc(0);
    float apogeeTime = 0.0;
    float previousHeight = 0.0;
    bool foundApogee = false;
    while (fgets(line, 3000, stream))
    {
        if (startsWith("#", line)) // Comment line in the CSV
        {
            continue;
        }
        numberOfHeights++;

        char *tmp = strdup(line);
        float height = atof(getfield(tmp, altColumn));
        free(tmp);

        tmp = strdup(line);
        float time = atof(getfield(tmp, timeColumn));
        free(tmp);

        tmp = strdup(line);
        float speed = atof(getfield(tmp, speedColumn));
        free(tmp);

        tmp = strdup(line);
        float acceleration = atof(getfield(tmp, accelerationColumn));
        free(tmp);

        // printf("At time %f, altitude: %f\n", time, height);
        heights = realloc(heights, numberOfHeights * sizeof(DATA_POINT));
        heights[numberOfHeights - 1].time = time;
        heights[numberOfHeights - 1].height = height;
        heights[numberOfHeights - 1].vertical_speed = speed;
        heights[numberOfHeights - 1].vertical_acceleration = acceleration;

        if (!foundApogee && numberOfHeights > 2)
        {
            if (previousHeight > height && height > 5.0)
            {
                apogeeTime = time;
                foundApogee = true;
            }
        }
        previousHeight = height;
    }

    fileInitalised = true;
    return (int)apogeeTime;
}

DATA_POINT *getDataFromFile(float time)
{
    if (!fileInitalised)
    {
        return NULL;
    }
    float previous = 0.0;
    DATA_POINT *correct = NULL;
    for (size_t i = 0; i < numberOfHeights; i++)
    {
        if (time < heights[i].time)
        {
            return correct;
        }
        previous = heights[i].height;
        correct = &heights[i];
    }
    return correct;
}

/* MAGNETOMETER */
BMX055_MAG default_initialise_magnetometer(void *user_spi_read, void *user_spi_write, void *user_delay_ms)
{
    BMX055_MAG dev;
    int8_t rslt = BMM150_OK;

    /* Sensor interface over SPI with native chip select line */
    dev.dev_id = 0;
    dev.intf = BMM150_SPI_INTF;
    dev.read = (bmm150_com_fptr_t)&user_spi_read;
    dev.write = (bmm150_com_fptr_t)&user_spi_write;
    dev.delay_ms = (bmm150_delay_fptr_t)&user_delay_ms;

    rslt = bmm150_init(&dev);
    return dev;
}

int8_t set_sensor_settings(BMX055_MAG *dev)
{
    int8_t rslt;

    /* Setting the power mode as normal */
    dev->settings.pwr_mode = BMM150_NORMAL_MODE;
    rslt = bmm150_set_op_mode(dev);

    /* Setting the preset mode as Low power mode 
	i.e. data rate = 10Hz XY-rep = 1 Z-rep = 2*/
    dev->settings.preset_mode = BMM150_PRESETMODE_LOWPOWER;
    rslt = bmm150_set_presetmode(dev);

    return rslt;
}

int8_t perform_self_tests(BMX055_MAG *dev)
{
    int8_t rslt;

    /* Perform Normal Self test */
    rslt = bmm150_perform_self_test(BMM150_NORMAL_SELF_TEST, dev);
    printf("\nNORMAL SELF TEST RESULT :  %d\n", rslt);

    /* Validate normal self test result */
    if (rslt == BMM150_OK)
    {
        printf("Normal Self test passed\n");
    }
    else
    {
        printf("Normal Self test failed\n");
    }
    /* Perform Advanced Self test */
    rslt |= bmm150_perform_self_test(BMM150_ADVANCED_SELF_TEST, dev);
    printf("ADVANCED SELF TEST RESULT : %d\n", rslt);

    /* Validate Advanced self test result */
    if (rslt == BMM150_OK)
    {
        printf("Advanced Self test passed\n");
    }
    else
    {
        printf("Advanced Self test failed\n");
    }

    return rslt;
}

int8_t drdy_interrupt_configure(BMX055_MAG *dev)
{
    int8_t rslt;
    uint16_t desired_settings;

    /* Set the macros to enable DRDY pin */
    desired_settings = BMM150_DRDY_PIN_EN_SEL | BMM150_DRDY_POLARITY_SEL;
    /* Set the drdy_pin_en to enable the drdy interrupt  */
    dev->settings.int_settings.drdy_pin_en = BMM150_INT_ENABLE;
    /* Set the polarity as active high on the DRDY pin */
    dev->settings.int_settings.drdy_polarity = BMM150_ACTIVE_HIGH_POLARITY;

    /* Set the configurations in the sensor */
    rslt = bmm150_set_sensor_settings(desired_settings, dev);

    return rslt;
}

int8_t check_interrupt_get_data(BMX055_MAG *dev)
{
    int8_t rslt;

    /* Read the interrupt status */
    rslt = bmm150_get_interrupt_status(dev);
    if (rslt == BMM150_OK)
    {
        if (dev->int_status & BMM150_DATA_READY_INT)
        {
            /* Interrupt asserted - Read mag data */
            rslt = bmm150_read_mag_data(dev);
            // printf("\n MAG DATA ");
            // printf("\n MAG X : %d MAG Y : %d MAG Z : %d ", dev->data.x, dev->data.y, dev->data.z);
        }
        else
        {
            /*Interrupt not asserted */
            // printf("\n Data is not ready yet");
        }
    }

    return rslt;
}

void setUpMagnetometer()
{
    // Function pointers:                         READ, WRITE, DELAY
    magnetometer = default_initialise_magnetometer((void *)NULL, (void *)NULL, (void *)NULL);

    checkError(set_sensor_settings(&magnetometer), "Sensor settings");
    checkError(perform_self_tests(&magnetometer), "Self test");
    checkError(drdy_interrupt_configure(&magnetometer), "Configure interrupt");
    magnetometerInitialised = true;
}

/* ACCELEROMETER */

// Declarations
s8 BMA2x2_SPI_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
s8 BMA2x2_SPI_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
s8 SPI_routine(void);
void BMA2x2_delay_msec(u32 msec);
struct bma2x2_t bma2x2;

// V_BMA2x2RESOLUTION_u8R used for selecting the accelerometer resolution: 12, 14 or 10 bit
extern u8 V_BMA2x2RESOLUTION_u8R;

BMX055_XYZ_TEMP bma2x2_data_readout()
{
    // s16 accel_x_s16, accel_y_s16, accel_z_s16 = BMA2x2_INIT_VALUE;
    // struct bma2x2_accel_data data_xyz;
    BMX055_XYZ_TEMP data_XYZT;

    u8 bandwidth_value_u8 = BMA2x2_INIT_VALUE;
    u8 bandwidth = BMA2x2_INIT_VALUE;
    s32 result = ERROR;

    bandwidth_value_u8 = 0x08; /* set bandwidth of 7.81Hz*/
    result += bma2x2_set_bw(bandwidth_value_u8);
    result += bma2x2_get_bw(&bandwidth); // This API used to read back the written value of bandwidth

    // Reading the data - individually
    // result += bma2x2_read_accel_x(&accel_x_s16);
    // result += bma2x2_read_accel_y(&accel_y_s16);
    // result += bma2x2_read_accel_z(&accel_z_s16);
    // result += bma2x2_read_accel_xyz(&data_xyz); // Reading X, Y and Z

    // Read X, Y, Z & T data
    result += bma2x2_read_accel_xyzt(&data_XYZT);

    // Set the power mode as DEEPSUSPEND to conserve power
    // result += bma2x2_set_power_mode(BMA2x2_MODE_DEEP_SUSPEND);
    checkError(result, "Reading of accelerometer");
    return data_XYZT;
}

s8 SPI_routine(void)
{
    bma2x2.bus_write = BMA2x2_SPI_bus_write;
    bma2x2.bus_read = BMA2x2_SPI_bus_read;
    bma2x2.delay_msec = BMA2x2_delay_msec;

    return BMA2x2_INIT_VALUE;
}

// SPI buffer length
#define SPI_BUFFER_LEN 5
#define BMA2x2_BUS_READ_WRITE_ARRAY_INDEX 1
#define BMA2x2_SPI_BUS_WRITE_CONTROL_BYTE 0x7F
#define BMA2x2_SPI_BUS_READ_CONTROL_BYTE 0x80

s8 BMA2x2_SPI_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 bytesCount)
{
    s32 iError = BMA2x2_INIT_VALUE;
    u8 array[SPI_BUFFER_LEN] = {0xFF};
    u8 stringpos;
    /*	For the SPI mode only 7 bits of register addresses are used.
	The MSB of register address is declared the bit what functionality it is
	read/write (read as 1/write as 0)*/
    array[BMA2x2_INIT_VALUE] = reg_addr | BMA2x2_SPI_BUS_READ_CONTROL_BYTE;
    /*read routine is initiated register address is mask with 0x80*/
    for (stringpos = BMA2x2_INIT_VALUE; stringpos < bytesCount; stringpos++)
    {
        *(reg_data + stringpos) = array[stringpos + BMA2x2_BUS_READ_WRITE_ARRAY_INDEX];
    }
    return (s8)iError;
}

s8 BMA2x2_SPI_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 bytesCount)
{
    s32 iError = BMA2x2_INIT_VALUE;
    u8 array[SPI_BUFFER_LEN * 2];
    u8 stringpos = BMA2x2_INIT_VALUE;

    for (stringpos = BMA2x2_INIT_VALUE; stringpos < bytesCount; stringpos++)
    {
        array[stringpos * 2] = (reg_addr++) & BMA2x2_SPI_BUS_WRITE_CONTROL_BYTE;
        array[stringpos * 2 + BMA2x2_BUS_READ_WRITE_ARRAY_INDEX] = *(reg_data + stringpos);
    }
    return (s8)iError;
}

void BMA2x2_delay_msec(u32 msec)
{
    return;
}

void setupAccelerometer()
{
    SPI_routine();
    checkError(bma2x2_init(&bma2x2), "Accelerometer initialisation");
    checkError(bma2x2_set_power_mode(BMA2x2_MODE_NORMAL), "Accelerometer power mode set procedure");
    accelerometerInitialised = true;
}

/* GENERAL */
void initialiseBMX055()
{
    setUpMagnetometer();
    setupAccelerometer();
}

DATA_POINT getCurrentBMX055Data()
{
    DATA_POINT currentData;
    if (first)
    {
        currentData.time = 0;
        currentData.height = 0;
        currentData.vertical_speed = 0;
        currentData.vertical_acceleration = 0;
        currentData.temperature = 0;

        previousData = currentData;

        t1 = clock();
        first = false;

        return currentData;
    }

    if (!magnetometerInitialised)
    {
        setUpMagnetometer();
    }

    if (!accelerometerInitialised)
    {
        setupAccelerometer();
    }

    BMX055_XYZ_TEMP dataXYZT = bma2x2_data_readout();

    checkError(check_interrupt_get_data(&magnetometer), "Check interrupt and get data");

    t2 = clock();
    float timeDifference = ((float)(t2 - t1) / CLOCKS_PER_SEC) * 1000;
    t1 = t2;

    currentData.time = 0;
    currentData.temperature = dataXYZT.temp;
    currentData.vertical_acceleration = (float)dataXYZT.z;
    currentData.vertical_speed = previousData.vertical_speed + (currentData.vertical_acceleration * timeDifference);
    currentData.height = previousData.height + (((previousData.vertical_speed + currentData.vertical_speed) / 2) * timeDifference);
    previousData = currentData;
    return currentData;
}
