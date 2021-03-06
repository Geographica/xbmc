/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <Python.h>
#include <string>
#include <stdint.h>

#include "utils/StdString.h"
#include "interfaces/legacy/Exception.h"
#include "interfaces/legacy/AddonClass.h"
#include "interfaces/legacy/Window.h"

namespace PythonBindings
{
  void PyXBMCGetUnicodeString(std::string& buf, PyObject* pObject, bool coerceToString = false,
                              const char* pos = "unknown", 
                              const char* methodname = "unknown") throw (XBMCAddon::WrongTypeException);

  // This is for casting from child class to base class
  struct TypeConverterBase
  {
    virtual void* convert(void* from) = 0;
  };

  /**
   * Template to allow the instantiation of a particular type conversion
   */
  template<class T, class F> struct TypeConverter : public TypeConverterBase
  {
    inline virtual void* convert(void* from) { return static_cast<T*>((F*)from); }
  };

  struct TypeInfo
  {
    const char* swigType;
    TypeInfo* parentType;
    TypeConverterBase* converter;
  };

  // This will hold the pointer to the api type, whether known or unknown
  struct PyHolder
  { 
    PyObject_HEAD
    int32_t magicNumber;
    const TypeInfo* typeInfo;
    void* pSelf;
  };

#define XBMC_PYTHON_TYPE_MAGIC_NUMBER 0x58626D63

  void PyXBMCInitializeTypeObject(PyTypeObject* type_object, TypeInfo* typeInfo);

  /**
   * This method retrieves the pointer from the PyHolder. The return value should
   * be case to the appropriate type.
   *
   * Since the calls to this are generated there's no NULL pointer checks
   */
  inline void* retrieveApiInstance(PyObject* pythonType, PyTypeObject* typeToCheck, 
                                   const char* methodNameForErrorString, 
                                   const char* typenameForErrorString) throw (XBMCAddon::WrongTypeException)
  {
    if (pythonType == NULL || ((PyHolder*)pythonType)->magicNumber != XBMC_PYTHON_TYPE_MAGIC_NUMBER)
      return NULL;
    if (!PyObject_TypeCheck(pythonType, typeToCheck))
      throw XBMCAddon::WrongTypeException("Incorrect type passed to \"%s\", was expecting a \"%s\".",methodNameForErrorString,typenameForErrorString);
    return ((PyHolder*)pythonType)->pSelf;
  }

  bool isParameterRightType(const char* passedType, const char* expectedType, const char* methodNamespacePrefix, bool tryReverse = true);

  void* doretrieveApiInstance(const PyHolder* pythonType, const TypeInfo* typeInfo, const char* expectedType, 
                              const char* methodNamespacePrefix, const char* methodNameForErrorString) throw (XBMCAddon::WrongTypeException);

  /**
   * This method retrieves the pointer from the PyHolder. The return value should
   * be case to the appropriate type.
   *
   * Since the calls to this are generated there's no NULL pointer checks
   */
  inline void* retrieveApiInstance(const PyObject* pythonType, const char* expectedType, const char* methodNamespacePrefix,
                                   const char* methodNameForErrorString) throw (XBMCAddon::WrongTypeException)
  {
    return (pythonType == NULL) ? NULL :
      doretrieveApiInstance(((PyHolder*)pythonType),((PyHolder*)pythonType)->typeInfo, expectedType, methodNamespacePrefix, methodNameForErrorString);
  }

  /**
   * This method is a helper for the generated API. It's called prior to any API
   * class constructor being returned from the generated code to Python
   */
  void prepareForReturn(XBMCAddon::AddonClass* c);

  /**
   * This method is a helper for the generated API. It's called prior to any API
   * class destructor being dealloc-ed from the generated code from Python
   */
  void cleanForDealloc(XBMCAddon::AddonClass* c);

  /**
   * This method is a helper for the generated API. It's called prior to any API
   * class destructor being dealloc-ed from the generated code from Python
   *
   * There is a Catch-22 in the destruction of a Window. 'dispose' needs to be
   * called on destruction but cannot be called from the destructor.
   * This overrides the default cleanForDealloc to resolve that.
   */
  void cleanForDealloc(XBMCAddon::xbmcgui::Window* c);

  /**
   * This method allows for conversion of the native api Type to the Python type
   *
   * NOTE: swigTypeString must be in the data segment. That is, it should be an explicit string since
   * the const char* is stored in a PyHolder struct and never deleted.
   */
  PyObject* makePythonInstance(void* api, PyTypeObject* typeObj, TypeInfo* typeInfo, bool incrementRefCount);

  class Director
  {
  protected:
    PyObject* self;
  public:
    inline Director() : self(NULL) {}
    inline void setPyObjectForDirector(PyObject* pyargself) { self = pyargself; }
  };

  /**
   * This exception is thrown from Director calls that call into python when the 
   * Python error is 
   */
  class PythonToCppException : public XbmcCommons::UncheckedException
  {
  public:
    /**
     * Assuming a PyErr_Occurred, this will fill the exception message with all
     *  of the appropriate information including the traceback if it can be
     *  obtained. It will also clear the python message.
     */
    PythonToCppException();
  };

  template<class T> struct PythonCompare
  {
    static inline int compare(PyObject* obj1, PyObject* obj2, const char* swigType, const char* methodNamespacePrefix, const char* methodNameForErrorString)
      throw(XBMCAddon::WrongTypeException)
    {
      TRACE;
      try
      {
        T* o1 = (T*)retrieveApiInstance(obj1, swigType, methodNamespacePrefix, methodNameForErrorString);
        T* o2 = (T*)retrieveApiInstance(obj2, swigType, methodNamespacePrefix, methodNameForErrorString);

        return ((*o1) < (*o2) ? -1 : 
                ((*o1) > (*o2) ? 1 : 0));
      }
      catch (const XBMCAddon::WrongTypeException& e)
      {
        CLog::Log(LOGERROR,"EXCEPTION: %s",e.GetMessage());
        PyErr_SetString(PyExc_RuntimeError, e.GetMessage());
      }
      return -1;
    }
  };
}
