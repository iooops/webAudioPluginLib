/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "../Application/jucer_Headers.h"
#include "jucer_ResourceFile.h"

static const char* resourceFileIdentifierString = "JUCER_BINARY_RESOURCE";

//==============================================================================
ResourceFile::ResourceFile (Project& p)
    : project (p),
      className ("BinaryData")
{
    addResourcesFromProjectItem (project.getMainGroup());
}

ResourceFile::~ResourceFile()
{
}

//==============================================================================
void ResourceFile::addResourcesFromProjectItem (const Project::Item& projectItem)
{
    if (projectItem.isGroup())
    {
        for (int i = 0; i < projectItem.getNumChildren(); ++i)
            addResourcesFromProjectItem (projectItem.getChild(i));
    }
    else
    {
        if (projectItem.shouldBeAddedToBinaryResources())
            addFile (projectItem.getFile());
    }
}

//==============================================================================
void ResourceFile::setClassName (const String& name)
{
    className = name;
}

void ResourceFile::addFile (const File& file)
{
    files.add (file);

    const String variableNameRoot (CodeHelpers::makeBinaryDataIdentifierName (file));
    String variableName (variableNameRoot);

    int suffix = 2;
    while (variableNames.contains (variableName))
        variableName = variableNameRoot + String (suffix++);

    variableNames.add (variableName);
}

String ResourceFile::getDataVariableFor (const File& file) const
{
    jassert (files.indexOf (file) >= 0);
    return variableNames [files.indexOf (file)];
}

String ResourceFile::getSizeVariableFor (const File& file) const
{
    jassert (files.indexOf (file) >= 0);
    return variableNames [files.indexOf (file)] + "Size";
}

int64 ResourceFile::getTotalDataSize() const
{
    int64 total = 0;

    for (int i = 0; i < files.size(); ++i)
        total += files.getReference(i).getSize();

    return total;
}

static String getComment()
{
    String comment;
    comment << newLine << newLine
            << "   This is an auto-generated file: Any edits you make may be overwritten!" << newLine
            << newLine
            << "*/" << newLine
            << newLine;

    return comment;
}

Result ResourceFile::writeHeader (MemoryOutputStream& header)
{
    header << "/* ========================================================================================="
           << getComment()
           << "#pragma once" << newLine
           << newLine
           << "namespace " << className << newLine
           << "{" << newLine;

    bool containsAnyImages = false;

    for (int i = 0; i < files.size(); ++i)
    {
        const File& file = files.getReference(i);

        if (! file.existsAsFile())
            return Result::fail ("Can't open resource file: " + file.getFullPathName());

        const int64 dataSize = file.getSize();

        const String variableName (variableNames[i]);

        FileInputStream fileStream (file);

        if (fileStream.openedOk())
        {
            containsAnyImages = containsAnyImages
                                 || (ImageFileFormat::findImageFormatForStream (fileStream) != nullptr);

            header << "    extern const char*   " << variableName << ";" << newLine;
            header << "    const int            " << variableName << "Size = " << (int) dataSize << ";" << newLine << newLine;
        }
    }

    header << "    // Points to the start of a list of resource names." << newLine
           << "    extern const char* namedResourceList[];" << newLine
           << newLine
           << "    // Number of elements in the namedResourceList array." << newLine
           << "    const int namedResourceListSize = " << files.size() <<  ";" << newLine
           << newLine
           << "    // If you provide the name of one of the binary resource variables above, this function will" << newLine
           << "    // return the corresponding data and its size (or a null pointer if the name isn't found)." << newLine
           << "    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();" << newLine
           << "}" << newLine;

    return Result::ok();
}

Result ResourceFile::writeCpp (MemoryOutputStream& cpp, const File& headerFile, int& i, const int maxFileSize)
{
    const bool isFirstFile = (i == 0);

    cpp << "/* ==================================== " << resourceFileIdentifierString << " ===================================="
        << getComment()
        << "namespace " << className << newLine
        << "{" << newLine;

    bool containsAnyImages = false;

    while (i < files.size())
    {
        const File& file = files.getReference(i);
        const String variableName (variableNames[i]);

        FileInputStream fileStream (file);

        if (fileStream.openedOk())
        {
            containsAnyImages = containsAnyImages
                                 || (ImageFileFormat::findImageFormatForStream (fileStream) != nullptr);

            const String tempVariable ("temp_binary_data_" + String (i));

            cpp  << newLine << "//================== " << file.getFileName() << " ==================" << newLine
                << "static const unsigned char " << tempVariable << "[] =" << newLine;

            {
                MemoryBlock data;
                fileStream.readIntoMemoryBlock (data);
                CodeHelpers::writeDataAsCppLiteral (data, cpp, true, true);
            }

            cpp << newLine << newLine
                << "const char* " << variableName << " = (const char*) " << tempVariable << ";" << newLine;
        }

        ++i;

        if (cpp.getPosition() > maxFileSize)
            break;
    }

    if (isFirstFile)
    {
        if (i < files.size())
        {
            cpp << newLine
                << "}" << newLine
                << newLine
                << "#include \"" << headerFile.getFileName() << "\"" << newLine
                << newLine
                << "namespace " << className << newLine
                << "{";
        }

        cpp << newLine
            << newLine
            << "const char* getNamedResource (const char*, int&) throw();" << newLine
            << "const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()" << newLine
            << "{" << newLine;

        StringArray returnCodes;
        for (int j = 0; j < files.size(); ++j)
        {
            const File& file = files.getReference(j);
            const int64 dataSize = file.getSize();
            returnCodes.add ("numBytes = " + String (dataSize) + "; return " + variableNames[j] + ";");
        }

        CodeHelpers::createStringMatcher (cpp, "resourceNameUTF8", variableNames, returnCodes, 4);

        cpp << "    numBytes = 0;" << newLine
            << "    return 0;" << newLine
            << "}" << newLine
            << newLine
            << "const char* namedResourceList[] =" << newLine
            << "{" << newLine;

        for (int j = 0; j < files.size(); ++j)
            cpp << "    " << variableNames[j].quoted() << (j < files.size() - 1 ? "," : "") << newLine;

        cpp << "};" << newLine;
    }

    cpp << newLine
        << "}" << newLine;

    return Result::ok();
}

Result ResourceFile::write (Array<File>& filesCreated, const int maxFileSize)
{
    const File headerFile (project.getBinaryDataHeaderFile());

    {
        MemoryOutputStream mo;
        Result r (writeHeader (mo));

        if (r.failed())
            return r;

        if (! FileHelpers::overwriteFileWithNewDataIfDifferent (headerFile, mo))
            return Result::fail ("Can't write to file: " + headerFile.getFullPathName());

        filesCreated.add (headerFile);
    }

    int i = 0;
    int fileIndex = 0;

    for (;;)
    {
        File cpp (project.getBinaryDataCppFile (fileIndex));

        MemoryOutputStream mo;

        Result r (writeCpp (mo, headerFile, i, maxFileSize));

        if (r.failed())
            return r;

        if (! FileHelpers::overwriteFileWithNewDataIfDifferent (cpp, mo))
            return Result::fail ("Can't write to file: " + cpp.getFullPathName());

        filesCreated.add (cpp);
        ++fileIndex;

        if (i >= files.size())
            break;
    }

    return Result::ok();
}
