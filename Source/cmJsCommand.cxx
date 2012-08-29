/*=========================================================================

  Copyright (c) 2009 Peter Kümmel.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cmJsCommand.h"

#include "cmake.h"

#include <QScriptEngine>
#include <QStringList>


bool cmJsCommand
::InitialPass(std::vector<std::string> const& args, cmExecutionStatus &)
{
  // assume there is only one string
  if(args.size() < 1)
    {
    SetError("JavaScript command called with incorrect number of arguments");
    return false;
    }

  int error = 0;
  if (args.size() == 2 && args[0] == "FILE") 
    {
    std::string file = Makefile->GetCurrentDirectory();
    file += "/";
    file += args[1];
    error = Makefile->RunJsFile(file);
    }
  else
    {
    // join args
    std::string str;
    for(std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
      {
      str += *it;
      }

    QScriptEngine* eng = Makefile->jsEngine();
    eng->evaluate(QString::fromStdString(str));
    if (eng->hasUncaughtException()) 
      {
      QString msg = "Error when processing JavaScript code\n";
      msg += eng->uncaughtExceptionBacktrace().join("\n");
      SetError(qPrintable(msg));
      }
    }

  if (error != 0) 
    {
    SetError("Error when processing JavaScript code");
    return false;
    }

  return true;
}

