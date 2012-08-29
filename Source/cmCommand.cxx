/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmCommand.h,v $
  Language:  C++
  Date:      $Date: 2007/07/02 19:43:21 $
  Version:   $Revision: 1.25 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "cmCommand.h"
#include "cmJsUtilities.h"


static QScriptValue cmJsFunction(QScriptContext *context, QScriptEngine *engine)
{
  // build a list file function 
  cmListFileFunction lff;
  cmExecutionStatus status;

  QString functionname = context->callee().data().toString();
  lff.Name = functionname.toStdString();
  
  // TODO use array
  int argCount = context->argumentCount();
  for (int i = 0; i < argCount; i++) 
    { 
	std::string arg = context->argument(i).toString().toStdString();
    lff.Arguments.push_back(cmListFileArgument(arg, false, 0, 0));
    }

  // get the current makefile
  QScriptValue jsvalue = engine->globalObject().property("cmCurrentMakefile");
  cmMakefile *makefile = QScriptBind::valueFromScriptValue<cmMakefile*>(jsvalue);

  // pass it to ExecuteCommand
  if (!makefile->ExecuteCommand(lff, status)) {
	  context->throwError("Error when calling function: " + functionname);
  }

  // What should I do with status?

  return QScriptValue();
}

cmCommand::cmCommand()
{
  this->Makefile = 0; 
  this->Enabled = true;
  this->ExposeToJs = true;
  this->JsFunction = cmJsFunction;
}
