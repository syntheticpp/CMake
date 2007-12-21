/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmMakefileTargetGenerator_h
#define cmMakefileTargetGenerator_h

#include "cmLocalUnixMakefileGenerator3.h"

class cmCustomCommand;
class cmDependInformation;
class cmDepends;
class cmGeneratedFileStream;
class cmGlobalUnixMakefileGenerator3;
class cmLocalUnixMakefileGenerator3;
class cmMakefile;
class cmTarget;
class cmSourceFile;

/** \class cmMakefileTargetGenerator
 * \brief Support Routines for writing makefiles
 *
 */
class cmMakefileTargetGenerator
{
public:
  // constructor to set the ivars
  cmMakefileTargetGenerator();
  virtual ~cmMakefileTargetGenerator() {};

  // construct using this factory call
  static cmMakefileTargetGenerator *New(cmLocalUnixMakefileGenerator3 *lg,
                                        cmStdString tgtName,
                                        cmTarget *tgt);

  /* the main entry point for this class. Writes the Makefiles associated
     with this target */
  virtual void WriteRuleFiles() = 0;

  /* the main entry point for this class. Writes the Makefiles associated
     with this target */
  virtual void WriteProgressVariables(unsigned long total, 
                                      unsigned long &current);
  
  /* return the number of actions that have progress reporting on them */
  virtual unsigned long GetNumberOfProgressActions() {
    return this->NumberOfProgressActions;}

  const char *GetTargetName() { return this->TargetName.c_str(); }
  cmTarget* GetTarget() { return this->Target;}
protected:

  // create the file and directory etc
  void CreateRuleFile();

  // outputs the rules for object files and custom commands used by
  // this target
  void WriteTargetBuildRules();

  // write some common code at the top of build.make
  void WriteCommonCodeRules();
  void WriteTargetLanguageFlags();

  // write the provide require rules for this target
  void WriteTargetRequiresRules();

  // write the clean rules for this target
  void WriteTargetCleanRules();

  // write the depend rules for this target
  void WriteTargetDependRules();

  // write the rules for an object
  void WriteObjectRuleFiles(cmSourceFile& source);

  // write the build rule for an object
  void WriteObjectBuildFile(std::string &obj,
                            const char *lang,
                            cmSourceFile& source,
                            std::vector<std::string>& depends);

  // write the depend.make file for an object
  void WriteObjectDependRules(cmSourceFile& source,
                              std::vector<std::string>& depends);

  // write the build rule for a custom command
  void GenerateCustomRuleFile(const cmCustomCommand& cc);

  // write a rule to drive building of more than one output from
  // another rule
  void GenerateExtraOutput(const char* out, const char* in,
                           bool symbolic = false);

  // write out the variable that lists the objects for this target
  void WriteObjectsVariable(std::string& variableName,
                            std::string& variableNameExternal);
  void WriteObjectsString(std::string& buildObjs);

  // write the driver rule to build target outputs
  void WriteTargetDriverRule(const char* main_output, bool relink);

  void DriveCustomCommands(std::vector<std::string>& depends);

  // Return the a string with -F flags on apple
  std::string GetFrameworkFlags();

  // append intertarget dependencies
  void AppendTargetDepends(std::vector<std::string>& depends);

  /** In order to support parallel builds for custom commands with
      multiple outputs the outputs are given a serial order, and only
      the first output actually has the build rule.  Other outputs
      just depend on the first one.  The check-build-system step must
      remove a dependee if the depender is missing to make sure both
      are regenerated properly.  This method is used by the local
      makefile generators to register such pairs.  */
  void AddMultipleOutputPair(const char* depender, const char* dependee);

  virtual void CloseFileStreams();
  void RemoveForbiddenFlags(const char* flagVar, const char* linkLang,
                            std::string& linkFlags);
  cmStdString TargetName;
  cmTarget *Target;
  cmLocalUnixMakefileGenerator3 *LocalGenerator;
  cmGlobalUnixMakefileGenerator3 *GlobalGenerator;
  cmMakefile *Makefile;

  enum CustomCommandDriveType { OnBuild, OnDepends, OnUtility };
  CustomCommandDriveType CustomCommandDriver;

  // the full path to the build file
  std::string BuildFileName;
  std::string BuildFileNameFull;

  // the full path to the progress file
  std::string ProgressFileName;
  std::string ProgressFileNameFull;
  unsigned long NumberOfProgressActions;

  // the path to the directory the build file is in
  std::string TargetBuildDirectory;
  std::string TargetBuildDirectoryFull;

  // the stream for the build file
  cmGeneratedFileStream *BuildFileStream;

  // the stream for the flag file
  std::string FlagFileNameFull;
  cmGeneratedFileStream *FlagFileStream;

  // the stream for the info file
  std::string InfoFileNameFull;
  cmGeneratedFileStream *InfoFileStream;

  // files to clean
  std::vector<std::string> CleanFiles;

  // objects used by this target
  std::vector<std::string> Objects;
  std::vector<std::string> ExternalObjects;
  std::set<std::string> ExtraContent;

  // Set of object file names that will be built in this directory.
  std::set<cmStdString> ObjectFiles;

  typedef std::map<cmStdString, cmStdString> MultipleOutputPairsType;
  MultipleOutputPairsType MultipleOutputPairs;

  //==================================================================
  // Convenience routines that do nothing more than forward to
  // implementaitons
  std::string Convert(const char* source,
                      cmLocalGenerator::RelativeRoot relative,
                      cmLocalGenerator::OutputFormat output =
                      cmLocalGenerator::UNCHANGED,
                      bool optional = false)
  {
    return this->LocalGenerator->Convert(source, relative, output, optional);
  }

};

#endif
