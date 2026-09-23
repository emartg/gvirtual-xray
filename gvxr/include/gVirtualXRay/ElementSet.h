/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef GVXR_ELEMENT_SET_H
#define GVXR_ELEMENT_SET_H


/**
********************************************************************************
*
*   @file       ElementSet.h
*
*   @brief      Class to manage a table of elements in material.
*
*   @version    1.0
*
*   @date       13/10/2014
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <map>
#include <string>

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
class ElementSet;


//******************************************************************************
//  Function declarations
//******************************************************************************

//---------------------------------------------------------------------------
/// operator <<
/*
*   @param anOutputSream:   the output stream
*   @param anElement:   the element to output
*   @return the output stream
*/
//---------------------------------------------------------------------------
std::ostream & operator << (std::ostream& anOutputSream,
        const gVirtualXRay::ElementSet& anElementSet);


//==============================================================================
/**
*   @class  ElementSet
*   @brief  ElementSet is a class to manage a table of elements in material.
*/
//==============================================================================
class ElementSet
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    static ElementSet& getInstance();


    //---------------------------------------------------------------------------
    /// Destructor.
    //---------------------------------------------------------------------------
    ~ElementSet();


    //---------------------------------------------------------------------------
    /// Load a file containing elements.
    /*
    *   @param aFileName:   the name of the file to load
    */
    //---------------------------------------------------------------------------
    void loadFromFile(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Load the table of elements.
    //--------------------------------------------------------------------------
    void loadElementSetFromMemory();


    //---------------------------------------------------------------------------
    /// Add an element to the list.
    /*
    *   @param anElement    the element to add
    */
    //---------------------------------------------------------------------------
    void addElement(const AtomicElement& anElement);


    //---------------------------------------------------------------------------
    /// Get a given element.
    /*
    *   @param anAtomicNumber:  the atomic number (Z) of the element
    *   @return the element
    */
    //---------------------------------------------------------------------------
    AtomicElement& getElement(unsigned short anAtomicNumber);


    //---------------------------------------------------------------------------
    /// Get a given element.
    /*
    *   @param anAtomicNumber:  the atomic number (Z) of the element
    *   @return the element
    */
    //---------------------------------------------------------------------------
    [[nodiscard]] const AtomicElement& getElement(unsigned short anAtomicNumber) const;


    //---------------------------------------------------------------------------
    /// Get a given element.
    /*
    *   @param anAtomicNumber:  the name of the element
    *   @return the element
    */
    //---------------------------------------------------------------------------
    [[nodiscard]] AtomicElement& getElement(const std::string& aName);


    //---------------------------------------------------------------------------
    /// Get a given element.
    /*
    *   @param anAtomicNumber:  the name of the element
    *   @return the element
    */
    //---------------------------------------------------------------------------
    [[nodiscard]] const AtomicElement& getElement(const std::string& aName) const;


    //---------------------------------------------------------------------------
    /// Accessor on the molar mass of the element.
    /*
    *   @param anAtomicNumber:  the atomic number (Z) of the element
    *   @return the molar mass of the element
    */
    //---------------------------------------------------------------------------
    [[nodiscard]] float getMolarMass(unsigned short anAtomicNumber) const;


    //---------------------------------------------------------------------------
    /// Accessor on the density of the element.
    /*
    *   @param anAtomicNumber:  the atomic number (Z) of the element
    *   @return the density of the element
    */
    //---------------------------------------------------------------------------
    [[nodiscard]] float getDensity(unsigned short anAtomicNumber) const;


    //---------------------------------------------------------------------------
    /// Accessor on the number of elements in the set.
    /*
    *   @return the number of elements in the set
    */
    //---------------------------------------------------------------------------
    [[nodiscard]] unsigned int getElementSetSize() const;


    //---------------------------------------------------------------------------
    /// Accessor on the begin const_iterator of m_element_set.
    /*
    *   @return the begin const_iterator of m_element_set
    */
    //---------------------------------------------------------------------------
    std::map<unsigned short, AtomicElement>::const_iterator begin() const;


    //---------------------------------------------------------------------------
    /// Accessor on the end const_iterator of m_element_set.
    /*
    *   @return the end const_iterator of m_element_set
    */
    //---------------------------------------------------------------------------
    std::map<unsigned short, AtomicElement>::const_iterator end() const;


//*******************************************************************************
private:
    //---------------------------------------------------------------------------
    /// Default Constructor
    //---------------------------------------------------------------------------
    ElementSet();


    //--------------------------------------------------------------------------
    /// Copy constructor
    //--------------------------------------------------------------------------
    ElementSet(const ElementSet&);


    //--------------------------------------------------------------------------
    /// Copy operator
    //--------------------------------------------------------------------------
    ElementSet& operator=(const ElementSet&);


    //---------------------------------------------------------------------------
    /// Clear all the memory.
    //---------------------------------------------------------------------------
    void clearMemory();


    //---------------------------------------------------------------------------
    /// Clear the dynamically allocated memory
    //---------------------------------------------------------------------------
    void clearDynamicallyMemory();


    //---------------------------------------------------------------------------
    /// An indexed table containing elements.
    //---------------------------------------------------------------------------
    std::map<unsigned short, AtomicElement> m_element_set;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "ElementSet.inl"


#endif // GVXR_ELEMENT_SET_H
