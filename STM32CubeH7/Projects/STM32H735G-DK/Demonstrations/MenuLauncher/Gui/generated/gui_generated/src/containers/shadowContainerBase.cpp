/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/shadowContainerBase.hpp>
#include "BitmapDatabase.hpp"

shadowContainerBase::shadowContainerBase()
{
    setWidth(160);
    setHeight(40);

    shadowBig.setXY(0, 0);
    shadowBig.setBitmap(touchgfx::Bitmap(BITMAP_SHADOW_ID));

    add(shadowBig);
}

void shadowContainerBase::initialize()
{
	
}
