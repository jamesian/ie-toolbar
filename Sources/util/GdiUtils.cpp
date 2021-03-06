/**
* Facebook Internet Explorer Toolbar Software License 
* Copyright (c) 2009 Facebook, Inc. 
*
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (which, together with any graphical images included with such
* software, are collectively referred to below as the "Software") to (a) use,
* reproduce, display, distribute, execute, and transmit the Software, (b)
* prepare derivative works of the Software (excluding any graphical images
* included with the Software, which may not be modified or altered), and (c)
* permit third-parties to whom the Software is furnished to do so, all
* subject to the following:
*
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.  
*
* Facebook, Inc. retains ownership of the Software and all associated
* intellectual property rights.  All rights not expressly granted in this
* license are reserved by Facebook, Inc.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

#include "StdAfx.h"

#include "Error.h"
#include "GdiUtils.h"
#include "PtrUtils.h"


namespace facebook {

RgnPtr createRectRegion(const CRect& rect) {
   RgnPtr region(new CRgn());

   const BOOL createResult = region->CreateRectRgnIndirect(rect);
   if (FALSE == createResult) {
      throw Error("Failed to create region\n");
   }

   return region;
}

BitmapPtr loadBitmap(const UINT resourceId) {
   BitmapPtr bitmap(new CBitmap());

   const BOOL loadResult = bitmap->LoadBitmap(resourceId);
   if (FALSE == loadResult) {
      throw Error("Failed to load bitmap\n");
   }

   return bitmap;
}

CSize getBitmapSize(CBitmap& bitmap) {
   BITMAP bitmapData = {0};
   bitmap.GetBitmap(&bitmapData);

   return CSize(bitmapData.bmWidth, bitmapData.bmHeight);
}

SIZE getMessageLength(const String& message, const CFont& font) {
  SIZE textSize;
  HDC context = ::GetDC(0);
  SelectObject(context, font);

  GetTextExtentPoint32(context, message.c_str(), message.length(), &textSize);

  return textSize;
}

void setFont(CDC* memDC, int fontSize, LPCTSTR fontFamily, CFont& font) {
  font.CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, 
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, fontFamily); 
  memDC->SelectObject(font); 
}

}