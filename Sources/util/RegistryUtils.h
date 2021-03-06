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

#ifndef REGISTRYUTILS_H
#define REGISTRYUTILS_H

#include "StringUtils.h"


namespace facebook {
   /**
    * class RegistryUtils
    *
    * Does interaction with registry to read, write, delete register keys
    * 
    */

   class RegistryUtils {

   public:
      /**
       * static function that write key into registry with DWORD value
       *
       * @param rootKey a root key (by const)
       * @param subKey a sub key (by const)
       * @param name a name of the key (by const)
       * @param value an value of the key (by const)
       */

      static bool writeDword(const HKEY rootKey, const String subKey, 
         const String name, const DWORD value);

      /**
       * static function that read key from registry with DWORD value
       *
       * @param rootKey a root key (by const)
       * @param subKey a sub key (by const)
       * @param name a name of the key (by const)
       * @param value an value of the key (by ref)
       * @param defaultValue a default value for key (by const)
       */

      static bool readDword(const HKEY rootKey, const String subKey, 
         const String name, DWORD& value, const DWORD defaultValue = 0);

      /**
       * static function that write key into registry with String value
       *
       * @param rootKey a root key (by const)
       * @param subKey a sub key (by const)
       * @param name a name of the key (by const)
       * @param value an value of the key (by const)
       */

      static bool writeString(const HKEY rootKey, const String subKey, 
         const String name, const String value);

      /**
       *  static function that read key from registry with String value
       *
       * @param rootKey a root key (by const)
       * @param subKey a sub key (by const)
       * @param name a name of the key (by const)
       * @param value an value of the key (by ref)
       * @param defaultValue a default value for key (by const)
       */

      static bool readString(const HKEY rootKey, const String subKey, 
         const String name, String& value, const String defaultValue = _T(""));

      /**
       * static function that delete key from registry with String value
       *
       * @param rootKey a root key (by const)
       * @param subKey a sub key (by const)
       *
       */

      static bool deleteKey(const HKEY rootKey, const String subKey);

      /**
       * static function that delete value from registry
       * @param rootKey a root key (by const)
       * @param subKey a sub key (by const)
       * @param value value of subkey
       */
      static bool deleteValue(const HKEY rootKey, const String subKey, 
                              const String value);

      /**
       * static function that gets the possible registry path where we can store settings
       * depending on the calling process rights
       * @param rootKey a root key that will be used
       *
       */
	    static bool getWritableRegistryKey(HKEY &rootKey);
   };

   /**
    * struct RegStrEntry
    * 
    * wrappers registry entry to work with string values
    */
   struct RegStrEntry {
      HKEY _rootKey; // root key
      String _subKey; // sub key
      String _name; // name of the key
      String _value; // value of the key

      RegStrEntry(const HKEY& rootKey, const String& subKey,
         const String& name, const String& value):
      _rootKey(rootKey),
         _subKey(subKey),
         _name(name),
         _value(value) {
      }
      bool write() {
         return RegistryUtils::writeString(_rootKey, _subKey, _name, _value);
      }
      bool removeKey() {
         return RegistryUtils::deleteKey(_rootKey, _subKey);
      }
      bool remove() {
        return RegistryUtils::deleteValue(_rootKey, _subKey, _name);
      }
      bool read() {
         return RegistryUtils::readString(_rootKey, _subKey, _name, _value);
      }

   };

} // !namespace facebook

#endif // REGISTRYUTILS_H
