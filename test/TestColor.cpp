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
 * @brief  Test color.
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "TestColor.h"

#include <unity.h>
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

/**
 * Test color.
 */
extern void testColor()
{
    Color myColorA;
    Color myColorB  = ColorDef::TOMATO;
    Color myColorC  = myColorB;

    /* Default color is black */
    TEST_ASSERT_EQUAL_UINT32(0u, myColorA);

    /* Does the color assignment works? */
    TEST_ASSERT_EQUAL_UINT8(ColorDef::getRed(ColorDef::TOMATO), myColorB.getRed());
    TEST_ASSERT_EQUAL_UINT8(ColorDef::getGreen(ColorDef::TOMATO), myColorB.getGreen());
    TEST_ASSERT_EQUAL_UINT8(ColorDef::getBlue(ColorDef::TOMATO), myColorB.getBlue());

    /* Does the color assignment via copy constructor works? */
    TEST_ASSERT_EQUAL_UINT8(ColorDef::getRed(ColorDef::TOMATO), myColorC.getRed());
    TEST_ASSERT_EQUAL_UINT8(ColorDef::getGreen(ColorDef::TOMATO), myColorC.getGreen());
    TEST_ASSERT_EQUAL_UINT8(ColorDef::getBlue(ColorDef::TOMATO), myColorC.getBlue());

    /* Check the 5-6-5 RGB format conversion. */
    myColorA.set(ColorDef::WHITE);
    TEST_ASSERT_EQUAL_UINT8(0xffu, myColorA.getRed());
    TEST_ASSERT_EQUAL_UINT8(0xffu, myColorA.getGreen());
    TEST_ASSERT_EQUAL_UINT8(0xffu, myColorA.getBlue());
    TEST_ASSERT_EQUAL_UINT16(0xffffu, myColorA.to565());

    myColorA.set(0x00080408U);
    TEST_ASSERT_EQUAL_UINT8(0x08u, myColorA.getRed());
    TEST_ASSERT_EQUAL_UINT8(0x04u, myColorA.getGreen());
    TEST_ASSERT_EQUAL_UINT8(0x08u, myColorA.getBlue());
    TEST_ASSERT_EQUAL_UINT16(0x0821u, myColorA.to565());

    /* Does the color assignment via assignment operator works? */
    myColorA = myColorB;
    TEST_ASSERT_EQUAL_UINT8(myColorB.getRed(), myColorC.getRed());
    TEST_ASSERT_EQUAL_UINT8(myColorB.getGreen(), myColorC.getGreen());
    TEST_ASSERT_EQUAL_UINT8(myColorB.getBlue(), myColorC.getBlue());

    /* Get/Set single colors */
    myColorA.setRed(0x12U);
    myColorA.setGreen(0x34U);
    myColorA.setBlue(0x56U);
    TEST_ASSERT_EQUAL_UINT8(0x12u, myColorA.getRed());
    TEST_ASSERT_EQUAL_UINT8(0x34u, myColorA.getGreen());
    TEST_ASSERT_EQUAL_UINT8(0x56u, myColorA.getBlue());

    /* Check conversion routines of ColorDef */
    TEST_ASSERT_EQUAL_UINT16(0x0821u, ColorDef::convert888To565(0x00080408U));
    TEST_ASSERT_EQUAL_UINT32(0x00080408u, ColorDef::convert565To888(0x0821U));

    /* Dim color 25% darker */
    myColorA = 0xc8c8c8u;
    myColorA.setIntensity(192);
    TEST_ASSERT_EQUAL_UINT8(0x96u, myColorA.getRed());
    TEST_ASSERT_EQUAL_UINT8(0x96u, myColorA.getGreen());
    TEST_ASSERT_EQUAL_UINT8(0x96u, myColorA.getBlue());

    /* Dim a color by 0%, which means no change.
     * And additional check non-destructive base colors.
     */
    myColorA.setIntensity(255);
    TEST_ASSERT_EQUAL_UINT8(0xc8u, myColorA.getRed());
    TEST_ASSERT_EQUAL_UINT8(0xc8u, myColorA.getGreen());
    TEST_ASSERT_EQUAL_UINT8(0xc8u, myColorA.getBlue());

    return;
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/
