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

#include "BrowserUtils.h"

#include "ddeml.h"
#include <ShlGuid.h>
#include <Wininet.h>
#include "windows.h"


#pragma warning(disable : 4146)
#pragma warning(disable : 4278) 

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "ComWrappers.h"
#include "ScopeGuard.h"

namespace facebook{namespace BrowserUtils{

void IEPopup(const String& url, 
                           const int width, 
                           const int height) {
  // browser instance
  IWebBrowser2Ptr browser;

  //try to instantiate IE instance
  CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, 
    IID_IWebBrowser2, (void**)&browser);

  if (browser == NULL) {
    return;
  }

  //hide menu bar
  const HRESULT putMenuBarRes = browser->put_MenuBar(VARIANT_FALSE);
  if (FAILED(putMenuBarRes)) {
    _com_raise_error(putMenuBarRes);
  }

  //hide toolbars
  const HRESULT putToolBarRes = browser->put_ToolBar(VARIANT_FALSE);
  if (FAILED(putToolBarRes)) {
    _com_raise_error(putToolBarRes);
  }

  //make not resizable
  const HRESULT putResizableRes = browser->put_Resizable(VARIANT_FALSE);
  if (FAILED(putResizableRes)) {
    _com_raise_error(putResizableRes);
  }

  //hide status bar
  const HRESULT putStatusRes =  browser->put_StatusBar(VARIANT_TRUE);
  if (FAILED(putStatusRes)) {
    _com_raise_error(putStatusRes);
  }

  //hide address bar
  const HRESULT putAddressBarRes = browser->put_AddressBar(VARIANT_FALSE);
  if (FAILED(putAddressBarRes)) {
    _com_raise_error(putAddressBarRes);
  }

  //specify width and height
  const HRESULT putWidthRes = browser->put_Width(width);
  if (FAILED(putWidthRes)) {
    _com_raise_error(putWidthRes);
  }

  const HRESULT putHeigthRes = browser->put_Height(height);
  if (FAILED(putHeigthRes)) {
   _com_raise_error(putHeigthRes);
  }
  //show browser
  const HRESULT putVisibleRes = browser->put_Visible(VARIANT_TRUE);
  if (FAILED(putVisibleRes)) {
   _com_raise_error(putVisibleRes);
  }
  //put it on the front
  HWND handle;
  const HRESULT getHandleRes = browser->get_HWND((SHANDLE_PTR*)&handle);
  if (SUCCEEDED(getHandleRes)) {
    ::SetForegroundWindow(handle);
  }
  navigate(browser, url);
  
}

void navigate(IWebBrowser2Ptr& browser, const String& url) {
  _variant_t empty;
  _variant_t targetUrl = toBSTR(url);

  const HRESULT navigateRes = browser->Navigate2(&targetUrl, 
    &empty, &empty, &empty, &empty);

  if (FAILED(navigateRes)) {
    _com_raise_error(navigateRes);
  }
}

HRESULT getWebBrowser(IUnknown* sitePtr, IWebBrowser2Ptr& pBrwowser) {
  if (!sitePtr) {
    return E_FAIL;
  }

  IUnknownPtr site(sitePtr);

  IServiceProviderPtr serviceProvider;
  HRESULT hr = site.QueryInterface(IID_IServiceProvider, &serviceProvider);
  if (FAILED(hr)) {
    return hr;
  }
  
  hr = serviceProvider->QueryService(SID_SWebBrowserApp, &pBrwowser);
  return hr;
}

// dummy dde callback function.
HDDEDATA CALLBACK DdeCallbackFunction(UINT uType,
                                      UINT uFmt,
                                      HCONV hconv,
                                      HDDEDATA hsz1,
                                      HDDEDATA hsz2,
                                      HDDEDATA hdata,
                                      HDDEDATA dwData1,
                                      HDDEDATA dwData2
                                      ) {
  UNREFERENCED_PARAMETER(uType);
  UNREFERENCED_PARAMETER(uFmt);
  UNREFERENCED_PARAMETER(hconv);
  UNREFERENCED_PARAMETER(hsz1);
  UNREFERENCED_PARAMETER(hsz2);
  UNREFERENCED_PARAMETER(hdata);
  UNREFERENCED_PARAMETER(dwData1);
  UNREFERENCED_PARAMETER(dwData2);
  return (HDDEDATA) NULL;
}

void openUrlNewWindow(const String& url) {
  // perform a DDE command that will open new tab in IE (or another default action)
  // the same as defayut system handling
  DWORD instance = 0;
  UINT res = DdeInitialize(&instance, 
    (PFNCALLBACK)(&DdeCallbackFunction), APPCMD_CLIENTONLY, 0);
  if (res != DMLERR_NO_ERROR) {
    return;
  }
  HSZ applicationName = DdeCreateStringHandle(instance, 
    _T("IExplore"), CP_WINUNICODE);
  HSZ topic = DdeCreateStringHandle(instance, 
    _T("WWW_OpenURLNewWindow"), CP_WINUNICODE);
  //open connection to the ie instance
  HCONV conversation = DdeConnect(instance, applicationName, topic, NULL);
  if (conversation == 0) {
    return;
  }
  String message = _T("\"") + url + _T("\",,-1,0,,,,");
  // send the needed command
  DdeClientTransaction((LPBYTE)message.c_str(), 
    (message.size() + 1) * sizeof(Char), conversation, 0, 0, 
    XTYP_EXECUTE, 60000, NULL); 
  // free all the allocated resources
  DdeDisconnect(conversation);
  DdeFreeStringHandle(instance, topic);
  DdeFreeStringHandle(instance, applicationName);
  DdeUninitialize(instance);
}
} } // !namespace facebook::BrowserUtils
