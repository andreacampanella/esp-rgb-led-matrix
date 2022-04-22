
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
 * @brief  Countdown plugin
 * @author Yann Le Glaz <yann_le@web.de>
 *
 * @addtogroup plugin
 *
 * @{
 */

#ifndef __COUNTDOWNPLUGIN__
#define __COUNTDOWNPLUGIN__

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Plugin.hpp"
#include "time.h"

#include <WidgetGroup.h>
#include <BitmapWidget.h>
#include <stdint.h>
#include <TextWidget.h>
#include <SimpleTimer.hpp>
#include <Mutex.hpp>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * Shows the remaining days until a configured target date.
 *
 * At the first installation a json document is generated to the /configuration/UUID.json
 * in the filesystem, where the target date has to be configured.
 *
 */
class CountdownPlugin : public Plugin
{
public:

    /** Date data. */
    struct DateDMY
    {
        uint8_t day;    /**< Day of month. */
        uint8_t month;  /**< Month of year. */
        uint16_t year;  /**< Year. */

        /** Initialize structure. */
        DateDMY() :
            day(0U),
            month(0U),
            year(0U)
        {
        }

        /** Destroy structure */
        ~DateDMY()
        {
        }

        /**
         * Initialize structure by assignment.
         *
         * @param[in] desc  Target day description, which to assign.
         */
        DateDMY(const DateDMY& desc) :
            day(desc.day),
            month(desc.month),
            year(desc.year)
        {
        }

        /**
         * Assign structure.
         *
         * @param[in] desc  Target day description, which to assign.
         */
        DateDMY& operator=(const DateDMY& desc)
        {
            if (this != &desc)
            {
                day     = desc.day;
                month   = desc.month;
                year    = desc.year;
            }

            return *this;
        }
    };

    /** The target date description. */
    struct TargetDayDescription
    {
        String plural;      /**< The description in plural form e.g. Days. */
        String singular;    /**< The description in singular form e.g. day */

        /** Initialize structure. */
        TargetDayDescription() :
            plural(),
            singular()
        {
        }

        /** Destroy structure. */
        ~TargetDayDescription()
        {
        }

        /**
         * Initialize structure by assignment.
         *
         * @param[in] desc  Target day description, which to assign.
         */
        TargetDayDescription(const TargetDayDescription& desc) :
            plural(desc.plural),
            singular(desc.singular)
        {
        }

        /**
         * Assign structure.
         *
         * @param[in] desc  Target day description, which to assign.
         */
        TargetDayDescription& operator=(const TargetDayDescription& desc)
        {
            if (this != &desc)
            {
                plural      = desc.plural;
                singular    = desc.singular;
            }

            return *this;
        }
    };

    /**
     * Constructs the plugin.
     *
     * @param[in] name  Plugin name
     * @param[in] uid   Unique id
     */
    CountdownPlugin(const String& name, uint16_t uid) :
        Plugin(name, uid),
        m_textCanvas(),
        m_iconCanvas(),
        m_bitmapWidget(),
        m_textWidget("\\calign?"),
        m_currentDate(),
        m_targetDate(),
        m_targetDateInformation(),
        m_remainingDays(""),
        m_mutex(),
        m_cfgReloadTimer()
    {
        /* Example data, used to generate the very first configuration file. */
        m_targetDate.day                    = 29;
        m_targetDate.month                  = 8;
        m_targetDate.year                   = 2019;
        m_targetDateInformation.plural      = "DAYS";
        m_targetDateInformation.singular    = "DAY";

        /* Move the text widget one line lower for better look. */
        m_textWidget.move(0, 1);

        (void)m_mutex.create();
    }

    /**
     * Destroys the plugin.
     */
    ~CountdownPlugin()
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
        return new CountdownPlugin(name, uid);
    }

    /**
     * Get plugin topics, which can be get/set via different communication
     * interfaces like REST, websocket, MQTT, etc.
     * 
     * Example:
     * {
     *     "topics": [
     *         "/text"
     *     ]
     * }
     * 
     * @param[out] topics   Topis in JSON format
     */
    void getTopics(JsonArray& topics) const final;

    /**
     * Get a topic data.
     * Note, currently only JSON format is supported.
     * 
     * @param[in]   topic   The topic which data shall be retrieved.
     * @param[out]  value   The topic value in JSON format.
     * 
     * @return If successful it will return true otherwise false.
     */
    bool getTopic(const String& topic, JsonObject& value) const final;

    /**
     * Set a topic data.
     * Note, currently only JSON format is supported.
     * 
     * @param[in]   topic   The topic which data shall be retrieved.
     * @param[in]   value   The topic value in JSON format.
     * 
     * @return If successful it will return true otherwise false.
     */
    bool setTopic(const String& topic, const JsonObject& value) final;

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
     * Update the display.
     * The scheduler will call this method periodically.
     *
     * @param[in] gfx   Display graphics interface
     */
    void update(YAGfx& gfx) final;

    /**
     * Get current target date for countdown.
     *
     * @return Target date
     */
    DateDMY getTargetDate() const;

    /**
     * Set target date for countdown.
     *
     * @param[in] targetDate    Target date
     */
    void setTargetDate(const DateDMY& targetDate);

    /**
     * Get the language depended strings for the unit.
     *
     * @return Target day unit descriptions
     */
    TargetDayDescription getTargetDayDescription() const;

    /**
     * Set language depended strings for the unit.
     *
     * @param[in] targetDayDescription  Unit in plural and singular form, e.g. "days/day".
     */
    void setTargetDayDescription(const TargetDayDescription& targetDayDescription);

private:

    /**
     * Icon width in pixels.
     */
    static const int16_t    ICON_WIDTH     = 8;

    /**
     * Icon height in pixels.
     */
    static const int16_t    ICON_HEIGHT    = 8;

    /**
     * Image path within the filesystem.
     */
    static const char*      IMAGE_PATH;

    /**
     * Plugin topic, used for parameter exchange.
     */
    static const char*      TOPIC;

   /**
    * Offset to translate the month of the tm struct (time.h)
    * to a human readable value, since months since January are used (0-11).
    */
    static const int16_t    TM_OFFSET_MONTH = 1;

   /**
    * Offset to translate the year of the tm struct (time.h)
    * to a human readable value, since years since 1900 are used.
    */
    static const int16_t    TM_OFFSET_YEAR  = 1900;

    /**
     * The configuration in the persistent memory shall be cyclic loaded.
     * This mechanism ensure that manual changes in the file are considered.
     * This is the reload period in ms.
     */
    static const uint32_t   CFG_RELOAD_PERIOD   = 30000U;

    WidgetGroup             m_textCanvas;               /**< Canvas used for the text widget. */
    WidgetGroup             m_iconCanvas;               /**< Canvas used for the bitmap widget. */
    BitmapWidget            m_bitmapWidget;             /**< Bitmap widget, used to show the icon. */
    TextWidget              m_textWidget;               /**< Text widget, used for showing the text. */
    DateDMY                 m_currentDate;              /**< Date structure to hold the current date. */
    DateDMY                 m_targetDate;               /**< Date structure to hold the target date from the configuration data. */
    TargetDayDescription    m_targetDateInformation;    /**< String used for configured additional target date information. */
    String                  m_remainingDays;            /**< String used for displaying the remaining days untril the target date. */
    mutable MutexRecursive  m_mutex;                    /**< Mutex to protect against concurrent access. */
    SimpleTimer             m_cfgReloadTimer;           /**< Timer is used to cyclic reload the configuration from persistent memory. */

    /**
     * Saves current configuration to JSON file.
     */
    bool saveConfiguration() const;

    /**
     * Load configuration from JSON file.
     */
    bool loadConfiguration();

    /**
     * Calculates the difference between m_targetTime and m_currentTime in days.
     */
    void calculateDifferenceInDays(void);

    /**
     * Counts the number of leap years.
     *
     * @param[in] date  Date
     *
     * @return Number of leap years.
     */
    uint16_t countLeapYears(const DateDMY& date) const;

    /**
     * Convert a given date in days starting from year 0.
     *
     * @param[in] date  Date
     *
     * @return Number of days since 0.
     */
    uint32_t dateToDays(const DateDMY& date) const;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif  /* __COUNTDOWNPLUGIN__ */

/** @} */
