/* MIT License
 *
 * Copyright (c) 2019 - 2022 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  Sensor data provider
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Sensors.h"

#include <Util.h>
#include <SensorLdrGl5528.h>
#include <SensorSht3X.h>
#include <SensorDhtX.h>

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** The LDR GL5528 is used for automatic display brightness control. */
static SensorLdrGl5528          gLdrGl5528;

/** The SHT3x sensor in autodetect mode (for two-wire sensors only). */
static SensorSht3X              gSht3x(SHTSensor::AUTO_DETECT);

/** The DHT11 sensor. */
static SensorDhtX               gDht11(SensorDhtX::MODEL_DHT11);

/** A list with all registered sensors. */
static ISensor*                 gSensors[] =
{
    &gLdrGl5528,
    &gSht3x,
    &gDht11
};

/** The concrete sensor data provider implementation. */
static SensorDataProviderImpl   gSensorDataProviderImpl(gSensors, UTIL_ARRAY_NUM(gSensors));

/******************************************************************************
 * Public Methods
 *****************************************************************************/

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

extern SensorDataProviderImpl* Sensors::getSensorDataProviderImpl()
{
    return &gSensorDataProviderImpl;
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/
