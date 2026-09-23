/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*   @file       ElementSet.cxx
*
*   @brief      Class to manage a table of elements in material.
*
*   @version    1.0
*
*   @date       13/10/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <fstream>
#include <algorithm>

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "element.csv.h"


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;



ElementSet& ElementSet::getInstance()
{
    static ElementSet m_instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.

    return (m_instance);
}


//---------------------------------------------------------
void ElementSet::loadFromFile(const std::string& aFileName)
//---------------------------------------------------------
{
	// Clear the memory
	clearMemory();

	// Open the file
	std::ifstream input_file(aFileName.c_str());

	// The file cannot be opened
	if (!input_file.is_open())
	{
		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("ERROR: cannot read the element table file using ") + aFileName);
	}

	// Skip the first line
	char first_line_[1024];
	if (input_file.getline(first_line_, 1024).fail())
	{
		// Close the input file
		input_file.close();

		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("Fail reading the first line of ") + aFileName);
	}

	// Read all the elements in the file,
	// and add them to the table
	while (!input_file.eof())
	{
		// Read the current element
		AtomicElement element;
		input_file >> element;

		// Add the element to the table
		// (no element has a null atomic number)
		if (element.getAtomicNumber() > 0)
		{
			m_element_set.insert(std::pair<unsigned short, AtomicElement>(element.getAtomicNumber(), element));
		}
	}

	// Close the input file
	input_file.close();
}


//-----------------------------------------
void ElementSet::loadElementSetFromMemory()
//-----------------------------------------
{
    // Clear the data
    clearMemory();

    char* p_dest(0);
    int z_lib_return_code = inflate(g_element_csv, sizeof(g_element_csv), &p_dest);

    // It does not exist
    if (!z_lib_return_code)
    {
        delete [] p_dest;
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No element was decoded.");
    }

    // Open the file
    std::stringstream element_stream;
    element_stream << (char*)p_dest;
    delete [] p_dest;

    // It does not exist
    if (!element_stream.str().size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No element was provided.");
    }

    // Ignore the first line
    char first_line[1024];
    element_stream.getline(first_line, 1024);

    // Process every record
    AtomicElement temp;
    while (!element_stream.eof())
    {
        // Load the record
        element_stream >> temp;

        // The record is valid
        if (!element_stream.eof())
        {
            // Store the record
            addElement(temp);
        }
    }
}


//---------------------------------------------------------
void ElementSet::addElement(const AtomicElement& anElement)
//---------------------------------------------------------
{
    m_element_set.insert(std::pair<unsigned short, AtomicElement>(anElement.getAtomicNumber(), anElement));
}


//------------------------------------------------------------------
AtomicElement& ElementSet::getElement(unsigned short anAtomicNumber)
//------------------------------------------------------------------
{
    std::map<unsigned short, AtomicElement>::iterator element_ite(m_element_set.find(anAtomicNumber));

    // Element found
    if (element_ite != m_element_set.end())
    {
        return (element_ite->second);
    }
    // Element not found in the list
    else
    {
        std::stringstream error_message;
        error_message << "Element (atomic number: " << anAtomicNumber << ") not found.";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
}


//------------------------------------------------------------------------------
const AtomicElement& ElementSet::getElement(unsigned short anAtomicNumber) const
//------------------------------------------------------------------------------
{
    std::map<unsigned short, AtomicElement>::const_iterator element_ite(m_element_set.find(anAtomicNumber));

    // Element found
    if (element_ite != m_element_set.end())
    {
        return (element_ite->second);
    }
    // Element not found in the list
    else
    {
        std::stringstream error_message;
        error_message << "Element (atomic number: " << anAtomicNumber << ") not found.";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
}


//-------------------------------------------------------------
AtomicElement& ElementSet::getElement(const std::string& aName)
//-------------------------------------------------------------
{
	// Convert the name to upper case
	std::string name_in_upper_case = aName;
	std::transform(name_in_upper_case.begin(), name_in_upper_case.end(),name_in_upper_case.begin(), ::toupper);

    for (std::map<unsigned short, AtomicElement>::iterator element_ite(m_element_set.begin());
    		element_ite != m_element_set.end();
    		++element_ite)
    {
		// Get the symbol and name from the iterator and convert them to upper case
		std::string symbol =  element_ite->second.getSymbol();
		std::string name   =  element_ite->second.getName();
		std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
		std::transform(  name.begin(),   name.end(),   name.begin(), ::toupper);

    	if (symbol == name_in_upper_case || name == name_in_upper_case) return (element_ite->second);
    }

    std::stringstream error_message;
    error_message << "Element (name: " << aName << ") not found.";
    throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
}


//-------------------------------------------------------------------------
const AtomicElement& ElementSet::getElement(const std::string& aName) const
//-------------------------------------------------------------------------
{
	// Convert the name to upper case
	std::string name_in_upper_case = aName;
	std::transform(name_in_upper_case.begin(), name_in_upper_case.end(),name_in_upper_case.begin(), ::toupper);

    for (std::map<unsigned short, AtomicElement>::const_iterator element_ite(m_element_set.begin());
    		element_ite != m_element_set.end();
    		++element_ite)
    {
		// Get the symbol and name from the iterator and convert them to upper case
		std::string symbol =  element_ite->second.getSymbol();
		std::string name   =  element_ite->second.getName();
		std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
		std::transform(  name.begin(),   name.end(),   name.begin(), ::toupper);

    	if (symbol == name_in_upper_case || name == name_in_upper_case) return (element_ite->second);
    }

    std::stringstream error_message;
    error_message << "Element (name: " << aName << ") not found.";
    throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
}


//-----------------------------------------------------------------
float ElementSet::getMolarMass(unsigned short anAtomicNumber) const
//-----------------------------------------------------------------
{
    return (getElement(anAtomicNumber).getMolarMass());
}


//---------------------------------------------------------------
float ElementSet::getDensity(unsigned short anAtomicNumber) const
//---------------------------------------------------------------
{
    return (getElement(anAtomicNumber).getDensity());
}


//-------------------------------------------------------------------------------
std::map<unsigned short, AtomicElement>::const_iterator ElementSet::begin() const
//-------------------------------------------------------------------------------
{
    return (m_element_set.begin());
}


//-----------------------------------------------------------------------------
std::map<unsigned short, AtomicElement>::const_iterator ElementSet::end() const
//-----------------------------------------------------------------------------
{
    return (m_element_set.end());
}


//---------------------------------------------------------
std::ostream & operator << (std::ostream& anOutputSream,
                            const ElementSet& anElementSet)
//---------------------------------------------------------
{
    for (std::map<unsigned short, AtomicElement>::const_iterator ite(anElementSet.begin());
        ite != anElementSet.end();
        ++ite)
    {
        anOutputSream << ite->second << std::endl;
    }

    return (anOutputSream);
}
