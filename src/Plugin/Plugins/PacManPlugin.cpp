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
 * @brief  An happy pac-man plug in 
 * @author Andrea Campanella <emuboy85@gmail.com>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "PacManPlugin.h"

#include <YAColor.h>

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


int PACMAN[8][8] = {
                    {0, 0, 0, 1, 1, 1, 0, 0},
                    {0, 0, 1, 0, 0, 0, 1, 0},
                    {0, 1, 0, 0, 0, 1, 0, 0},
                    {0, 1, 0, 0, 1, 0, 0, 0},
                    {0, 1, 0, 0, 0, 1, 0, 0},
                    {0, 0, 1, 0, 0, 0, 1, 0},
                    {0, 0, 0, 1, 1, 1, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                };
int PACMAN_2[8][8] = {
                    {0, 0, 0, 1, 1, 1, 0, 0},
                    {0, 0, 1, 0, 0, 0, 1, 0},
                    {0, 1, 0, 0, 0, 0, 0, 1},
                    {0, 1, 0, 0, 0, 1, 1, 1},
                    {0, 1, 0, 0, 0, 0, 0, 1},
                    {0, 0, 1, 0, 0, 0, 1, 0},
                    {0, 0, 0, 1, 1, 1, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                };

int BLUEGHOST[8][8] = {
                    {0, 0, 0, 1, 1, 0, 0, 0},
                    {0, 1, 1, 1, 1, 1, 1, 0},
                    {1, 1, 2, 1, 1, 2, 1, 1},
                    {1, 1, 1, 1, 1, 1, 1, 1},
                    {1, 1, 2, 1, 1, 2, 1, 1},
                    {1, 2, 1, 2, 2, 1, 2, 1},
                    {1, 1, 1, 1, 1, 1, 1, 1},
                    {1, 0, 1, 0, 0, 1, 0, 1},
                };
/******************************************************************************
 * Public Methods
 *****************************************************************************/

void PacManPlugin::update(YAGfx& gfx)
{
    const uint32_t  PERIOD      = 50U;
    bool            showPage    = false;
    static int CreatePlayField = 1;

    if (false == m_timer.isTimerRunning())
    {
        m_timer.start(PERIOD);
        showPage = true;
    }
    else if (true == m_timer.isTimeout())
    {
        m_page++;
        
        if (m_page == 3)
        {
            m_page = 0;
        }
        

        showPage = true;
        m_timer.restart();
    }
    else
    {
        ;
    }



    if (true == showPage)
    {
       // int16_t min = (gfx.getWidth() > gfx.getHeight()) ? gfx.getHeight() : gfx.getWidth();

        /* Clear display */
        //gfx.fillScreen(ColorDef::BLACK);

        static int offset = 0;
        switch(m_page)
        {
        case 0U:

                /*remove trailing of the old frame*/
                for (uint8_t i = 0; i < 8; i++)
                {
                    gfx.drawPixel(offset -1,  i, ColorDef::BLACK);
                }

                for (uint16_t x = 0 ; x < 8; x++)
                {
                    for (uint16_t y = 0  + offset ; y < 8 + offset; y++)
                    {
                        if (PACMAN[x][y - offset] == 1)
                        {
                            gfx.drawPixel(y, x, ColorDef::YELLOW);
                        }
                        else{
                            gfx.drawPixel(y, x, ColorDef::BLACK);
                        }
                    }                    
                }
            break;
        case 1U:
                for (uint16_t x = 0 ; x < 8; x++)
                {
                    for (uint16_t y = 0  + offset ; y < 8 + offset; y++)
                    {
                        if (PACMAN_2[x][y - offset] == 1)
                        {
                            gfx.drawPixel(y, x, ColorDef::YELLOW);
                        }
                        else{
                            gfx.drawPixel(y, x, ColorDef::BLACK);
                        }                        
                    }                    
                }
            break;

        case 2U:
            if (CreatePlayField == 1)
            {
                for (uint16_t y = 0 ; y < 96; y++)
                {                   
                    if (y % 8 == 0) 
                    {
                        if (y == 48)
                        {
                            for (uint16_t figure_x = 0 ; figure_x < 8; figure_x++)
                            {
                                for (uint16_t figure_y = 0 ; figure_y < 8; figure_y++)
                                {
                                    if (BLUEGHOST[figure_x][figure_y] == 1)
                                    {
                                        gfx.drawPixel(figure_y+y-3, figure_x, ColorDef::BLUE);
                                    }
                                    else if (BLUEGHOST[figure_x][figure_y] == 2)
                                    {
                                        gfx.drawPixel(figure_y+y-3, figure_x, ColorDef::WHITE);
                                    }
                                    
                                    else{
                                        gfx.drawPixel(figure_y+y-3, figure_x, ColorDef::BLACK);
                                    }                        
                                }                    
                            }
                        }
                        else
                        {
                            gfx.drawPixel(y, 3, ColorDef::WHITE);
                            gfx.drawPixel(y+1, 3, ColorDef::WHITE);
                            gfx.drawPixel(y, 4, ColorDef::WHITE);
                            gfx.drawPixel(y+1, 4, ColorDef::WHITE);
                        }
                    }
                }   
                CreatePlayField = 0;        
            }    
            break;
        default:
            break;
        }
        if (offset > 96)
        {
            offset = 0;
            CreatePlayField = 1;
        }
        else
        {
            offset++;
        }
        

    }

    return;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
