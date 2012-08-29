/*=========================================================================

  Copyright (c) 2009 Peter Kümmel
  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "cmJsUtilities.h"



void QScriptBind::registerGlobalFunction(QScriptEngine* engine, const QString& name, QScriptEngine::FunctionSignature func)
{
  QScriptValue jsfunc = engine->newFunction(func);
  jsfunc.setData(QScriptValue(engine, name));
  engine->globalObject().setProperty(name, jsfunc);
}


void QScriptBind::exportDefinitionToJs(QScriptEngine* engine, const char* name)
{
  // OBSOLETE by using QScriptClass
  return;

  if (engine->globalObject().property(name).isValid())
    {
    // qDebug() << "// nothing to do";
    return;
    }

  QString getter = QString(".__defineGetter__(\"%1\", function() { return cmake.GetDefinition(\"%1\"); } ); ").arg(QString::fromAscii(name));
  QString setter = QString(".__defineSetter__(\"%1\", function(value) { cmake.AddDefinition(\"%1\", value); } ); ").arg(QString::fromAscii(name));

  engine->evaluate(
    "this" + getter + "this" + setter +
    // also add a Object which only contains cmake variables
    "AA_CMake" + getter + "AA_CMake" + setter
    );
}
