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
 * @brief  Reads temperature and humidity from a sensor and displays it
 * @author Flavio Curti <fcu-github@no-way.org>
 * @addtogroup plugin
 *
 * @{
 */

#ifndef __TEMPHUMIDPLUGIN_H__
#define __TEMPHUMIDPLUGIN_H__

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include "Plugin.hpp"

#include <SimpleTimer.hpp>
#include <WidgetGroup.h>
#include <BitmapWidget.h>
#include <TextWidget.h>
#include <ISensorChannel.hpp>
#include <Mutex.hpp>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * TempHumid plugin.
 *
 * This plugins displays the temperature and humidity read from a sensor.
 */
class TempHumidPlugin : public Plugin
{
public:

    /**
     * Constructs the plugin.
     *
     * @param[in] name  Plugin name
     * @param[in] uid   Unique id
     */
    TempHumidPlugin(const String& name, uint16_t uid) :
        Plugin(name, uid),
        m_textCanvas(),
        m_iconCanvas(),
        m_bitmapWidget(),
        m_textWidget("\\calign?"),
        m_page(TEMPERATURE),
        m_pageTime(10000U),
        m_timer(),
        m_mutex(),
        m_humid(0.0F),
        m_temp(0.0F),
        m_sensorUpdateTimer(),
        m_slotInterf(nullptr),
        m_temperatureSensorCh(nullptr),
        m_humiditySensorCh(nullptr)
    {
        /* Move the text widget one line lower for better look. */
        m_textWidget.move(0, 1);

        (void)m_mutex.create();
    }

    /**
     * Enumeration to choose the page to be displayed.
     */    
    enum PageDisplay
    {
        TEMPERATURE = 0,   /**< Display Temperature in degree Celsius. */
        HUMIDITY,          /**< Display humidity in %. */
        PAGE_MAX           /**< Number of pages, use for simple switch (by using a PAGE_MAX as modulo operator) */ 
    };

    /**
     * Destroys the plugin.
     */
    ~TempHumidPlugin()
    {
        m_mutex.destroy();
    }

    /**
     * Plugin creation method, used to register on the plugin manager.
     *
     * @param[in] name  Plugin name
     * @param[in] uid   Unique id
     *
     * @return If successful, it will return the pointer to the plugin instance, otherwise nullptr.
     */
    static IPluginMaintenance* create(const String& name, uint16_t uid)
    {
        return new TempHumidPlugin(name, uid);
    }

    /**
     * Set the slot interface, which the plugin can used to request information
     * from the slot, it is plugged in.
     *
     * @param[in] slotInterf    Slot interface
     */
    void setSlot(const ISlotPlugin* slotInterf) final;

    /**
     * Start the plugin. This is called only once during plugin lifetime.
     * It can be used as deferred initialization (after the constructor)
     * and provides the canvas size.
     * 
     * Overwrite it if your plugin needs to know that it was installed.
     * 
     * @param[in] width     Display width in pixel
     * @param[in] height    Display height in pixel
     */
    void start(uint16_t width, uint16_t height) final;

   /**
     * Stop the plugin. This is called only once during plugin lifetime.
     * It can be used as a first clean-up, before the plugin will be destroyed.
     * 
     * Overwrite it if your plugin needs to know that it will be uninstalled.
     */
    void stop() final;

    /**
     * Process the plugin.
     * Overwrite it if your plugin has cyclic stuff to do without being in a
     * active slot.
     */
    void process(void) final;    

    /**
     * This method will be called in case the plugin is set active, which means
     * it will be shown on the display in the next step.
     *
     * @param[in] gfx   Display graphics interface
     */
    void active(YAGfx& gfx) final;

    /**
     * This method will be called in case the plugin is set inactive, which means
     * it won't be shown on the display anymore.
     */
    void inactive() final;
    
    /**
     * Update the display.
     * The scheduler will call this method periodically.
     *
     * @param[in] gfx   Display graphics interface
     */
    void update(YAGfx& gfx) final;

private:

    /**
     * Icon width in pixels.
     */
    static const int16_t        ICON_WIDTH          = 8;

    /**
     * Icon height in pixels.
     */
    static const int16_t        ICON_HEIGHT         = 8;

    /**
     * Path to the temperature icon
     */    
    static const char*          IMAGE_PATH_TEMP_ICON;

    /**
     * Path to the humidity icon
     */
    static const char*          IMAGE_PATH_HUMID_ICON;

    /**
     * Read sensor only every N milliseconds (currently 90 seconds)
     */
    static const uint32_t       SENSOR_UPDATE_PERIOD = (90U * 1000U);

    WidgetGroup                 m_textCanvas;               /**< Canvas used for the text widget. */
    WidgetGroup                 m_iconCanvas;               /**< Canvas used for the bitmap widget. */
    BitmapWidget                m_bitmapWidget;             /**< Bitmap widget, used to show the icon. */
    TextWidget                  m_textWidget;               /**< Text widget, used for showing the text. */
    uint8_t                     m_page;                     /**< Number of page, which to show. */
    unsigned long               m_pageTime;                 /**< How long to show page (1/4 slot-time or 10s default). */    
    SimpleTimer                 m_timer;                    /**< Timer for changing page. */
    MutexRecursive              m_mutex;                    /**< Mutex to protect against concurrent access. */
    float                       m_humid;                    /**< Last sensor humidity value */
    float                       m_temp;                     /**< Last sensor temperature value */
    SimpleTimer                 m_sensorUpdateTimer;        /**< Time used for cyclic sensor reading. */
    const ISlotPlugin*          m_slotInterf;               /**< Slot interface */
    ISensorChannel*             m_temperatureSensorCh;      /**< Temperature sensor channel */
    ISensorChannel*             m_humiditySensorCh;         /**< Humidity sensor channel */
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif  /* __TEMPHUMIDPLUGIN_H__ */

/** @} */