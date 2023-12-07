/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin V5.28.                           *
*        Compiled Jan 30 2015, 16:40:04                              *
*                                                                    *
*        (c) 1998 - 2015 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: Previous                                              *
* Dimensions:  40 * 42                                               *
* NumColors:   65536 colors + 8 bit alpha channel                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmPreviousButton;

#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static GUI_CONST_STORAGE unsigned char _acPrevious[] = {
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xE2,0xFF,0xDF, 0x93,0xFF,0xFF, 0x3F,0xFF,0xFF, 0x0F,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x10,0xFF,0xFF, 0x3F,0xFF,0xFF, 0x93,0xFF,0xFF, 0xE3,0xFF,0xDF, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xD4,0xFF,0xFF, 0x50,0xFF,0xFF, 0x05,0xF7,0xDF, 0x00,0xD7,0x3D, 0x00,0xA6,0x7C, 0x00,0x9E,0x3B, 0x00,0x9E,0x3B, 0x00,0x9E,0x5B, 0x00,0x9E,0x5C, 0x00,0x9E,0x5C, 0x00,0x9E,0x5C, 0x00,0xA6,0x7C, 0x00,0xA6,0x7C, 0x00,0xA6,0x7C, 
        0x00,0xA6,0x9C, 0x00,0xA6,0x9C, 0x00,0xA6,0x9D, 0x00,0xAE,0xBD, 0x00,0xAE,0xBD, 0x00,0xAE,0xBD, 0x00,0xAE,0xDD, 0x00,0xAE,0xDD, 0x00,0xBE,0xFD, 0x00,0xDF,0x9E, 0x05,0xFF,0xDF, 0x51,0xFF,0xFF, 0xD4,0xFF,0xFF, 0xFF,0x00,0x00,
  0xE0,0xFF,0xDF, 0x4D,0xFF,0xFF, 0x03,0xEF,0x9E, 0x00,0x7D,0xBA, 0x00,0x2C,0x37, 0x00,0x1B,0xF6, 0x00,0x1C,0x17, 0x00,0x24,0x17, 0x00,0x24,0x37, 0x00,0x24,0x57, 0x00,0x2C,0x58, 0x00,0x2C,0x78, 0x00,0x2C,0x98, 0x00,0x34,0x99, 0x00,0x34,0xB9, 
        0x00,0x34,0xD9, 0x00,0x3C,0xF9, 0x00,0x3C,0xFA, 0x00,0x45,0x1A, 0x00,0x45,0x3A, 0x00,0x45,0x5B, 0x00,0x4D,0x5B, 0x00,0x4D,0x7B, 0x00,0x55,0x9B, 0x00,0x65,0xDC, 0x00,0xA6,0xBD, 0x03,0xEF,0xBF, 0x4E,0xFF,0xFF, 0xE1,0xFF,0xDF,
  0x91,0xFF,0xFF, 0x04,0xF7,0xDF, 0x00,0x85,0xBA, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0xA6,0xDD, 0x05,0xFF,0xDF, 0x92,0xFF,0xFF,
  0x3C,0xFF,0xDF, 0x00,0xD7,0x3D, 0x00,0x2C,0x17, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x6D,0xFC, 0x00,0xDF,0x9E, 0x3E,0xFF,0xDF,
  0x0C,0xFF,0xFF, 0x00,0xA6,0x5B, 0x00,0x1B,0xD6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xBF,0x1E, 0x0D,0xFF,0xFF,
  0x01,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x01,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x5D,0x7A, 0x00,0x8E,0x3C, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x45,0x1A, 0x00,0x9E,0x5C, 0x00,0xE7,0x9E, 0x00,0xE7,0x9E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x98, 0x00,0x65,0x7A, 
        0x00,0xBE,0xFD, 0x00,0xC7,0x1D, 0x00,0x9E,0x7C, 0x00,0xD7,0x7E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x3C,0xB9, 0x00,0x96,0x3C, 0x00,0xD7,0x5E, 
        0x00,0x96,0x5C, 0x00,0x45,0x1A, 0x00,0x6D,0xBB, 0x00,0xD7,0x5E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x2C,0x58, 0x00,0x6D,0x7A, 0x00,0xC7,0x1D, 0x00,0xB6,0xDD, 0x00,0x5D,0x7A, 
        0x00,0x34,0xB9, 0x00,0x34,0xD9, 0x00,0x65,0x9B, 0x00,0xD7,0x5E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x96,0x3B, 0x00,0xD7,0x5E, 0x00,0x8E,0x1B, 0x00,0x3C,0xD9, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x65,0x9B, 0x00,0xD7,0x5E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x96,0x3B, 0x00,0xD7,0x5E, 0x00,0x8E,0x1B, 0x00,0x3C,0xD9, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x65,0x9B, 0x00,0xD7,0x5E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x2C,0x58, 0x00,0x6D,0x7A, 0x00,0xC6,0xFD, 0x00,0xB6,0xDD, 0x00,0x5D,0x7A, 
        0x00,0x34,0xB9, 0x00,0x34,0xD9, 0x00,0x65,0x9B, 0x00,0xD7,0x5E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x3C,0xB9, 0x00,0x96,0x3C, 0x00,0xD7,0x5E, 
        0x00,0x9E,0x5C, 0x00,0x45,0x1A, 0x00,0x6D,0xBB, 0x00,0xD7,0x5E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x98, 0x00,0x65,0x7A, 
        0x00,0xBE,0xFD, 0x00,0xC7,0x1D, 0x00,0x9E,0x7C, 0x00,0xD7,0x7E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x45,0x1A, 0x00,0x9E,0x5C, 0x00,0xE7,0x9E, 0x00,0xE7,0x9E, 0x00,0x4D,0x5A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x5D,0x7A, 0x00,0x8E,0x3C, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x00,0xFF,0xFF,
  0x01,0xFF,0xFF, 0x00,0x9E,0x1B, 0x00,0x13,0xB6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xB7,0x1E, 0x01,0xFF,0xFF,
  0x0C,0xFF,0xFF, 0x00,0xA6,0x5B, 0x00,0x1B,0xD6, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x5D,0xDC, 0x00,0xBF,0x1E, 0x0D,0xFF,0xFF,
  0x3C,0xFF,0xDF, 0x00,0xD7,0x3D, 0x00,0x2C,0x37, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0x6D,0xFC, 0x00,0xDF,0x9E, 0x3E,0xFF,0xDF,
  0x91,0xFF,0xFF, 0x05,0xF7,0xDF, 0x00,0x85,0xBA, 0x00,0x0B,0x96, 0x00,0x0B,0xB6, 0x00,0x13,0xD6, 0x00,0x13,0xD6, 0x00,0x1B,0xF7, 0x00,0x1C,0x17, 0x00,0x1C,0x17, 0x00,0x24,0x37, 0x00,0x24,0x58, 0x00,0x24,0x78, 0x00,0x2C,0x78, 0x00,0x2C,0x99, 
        0x00,0x2C,0xB9, 0x00,0x34,0xD9, 0x00,0x34,0xD9, 0x00,0x3C,0xFA, 0x00,0x3D,0x1A, 0x00,0x3D,0x3A, 0x00,0x45,0x3B, 0x00,0x45,0x5B, 0x00,0x45,0x7B, 0x00,0x4D,0x9B, 0x00,0x4D,0x9C, 0x00,0xA6,0xDD, 0x05,0xFF,0xDF, 0x92,0xFF,0xFF,
  0xE1,0xFF,0xDF, 0x4E,0xFF,0xFF, 0x03,0xEF,0x9E, 0x00,0x85,0xBA, 0x00,0x2C,0x37, 0x00,0x1B,0xF6, 0x00,0x1C,0x17, 0x00,0x24,0x17, 0x00,0x24,0x37, 0x00,0x24,0x57, 0x00,0x2C,0x58, 0x00,0x2C,0x78, 0x00,0x2C,0x98, 0x00,0x34,0x99, 0x00,0x34,0xB9, 
        0x00,0x34,0xD9, 0x00,0x3C,0xF9, 0x00,0x3C,0xFA, 0x00,0x45,0x1A, 0x00,0x45,0x3A, 0x00,0x45,0x5B, 0x00,0x4D,0x5B, 0x00,0x4D,0x7B, 0x00,0x55,0x9B, 0x00,0x65,0xDC, 0x00,0xA6,0xBD, 0x03,0xEF,0xBF, 0x4E,0xFF,0xFF, 0xE1,0xFF,0xDF,
  0xFF,0x00,0x00, 0xD4,0xFF,0xFF, 0x51,0xFF,0xFF, 0x05,0xF7,0xDF, 0x00,0xD7,0x3D, 0x00,0xA6,0x7C, 0x00,0x9E,0x3B, 0x00,0x9E,0x3B, 0x00,0x9E,0x5B, 0x00,0x9E,0x5C, 0x00,0x9E,0x5C, 0x00,0x9E,0x5C, 0x00,0xA6,0x7C, 0x00,0xA6,0x7C, 0x00,0xA6,0x7C, 
        0x00,0xA6,0x9C, 0x00,0xA6,0x9C, 0x00,0xA6,0x9C, 0x00,0xAE,0xBD, 0x00,0xAE,0xBD, 0x00,0xAE,0xBD, 0x00,0xAE,0xDD, 0x00,0xAE,0xDD, 0x00,0xBE,0xFD, 0x00,0xDF,0x9E, 0x05,0xFF,0xDF, 0x51,0xFF,0xFF, 0xD5,0xFF,0xFF, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xE3,0xFF,0xDF, 0x94,0xFF,0xFF, 0x41,0xFF,0xFF, 0x10,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0x10,0xFF,0xFF, 0x41,0xFF,0xFF, 0x94,0xFF,0xFF, 0xE3,0xFF,0xDF, 0xFF,0x00,0x00, 0xFF,0x00,0x00
};

GUI_CONST_STORAGE GUI_BITMAP bmPreviousButton = {
  29, // xSize
  30, // ySize
  87, // BytesPerLine
  24, // BitsPerPixel
  (unsigned char *)_acPrevious,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPAM565
};

/*************************** End of file ****************************/
