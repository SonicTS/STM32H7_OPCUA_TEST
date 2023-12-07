/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/wifiMenuContainerBase.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

wifiMenuContainerBase::wifiMenuContainerBase()
{
    setWidth(800);
    setHeight(480);

    gradientBackgroundBox.setXY(120, 90);
    gradientBackgroundBox.setBitmap(touchgfx::Bitmap(BITMAP_ERROR_BOX_ID));

    wifiListContainer.setPosition(120, 90, 560, 330);
    wifiListContainer.setScrollbarsColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    wifiList.setDirection(touchgfx::SOUTH);
    wifiList.setPosition(0, 0, 330, 60);
    wifiListContainer.add(wifiList);

    add(gradientBackgroundBox);
    add(wifiListContainer);
}

void wifiMenuContainerBase::initialize()
{
	
}
