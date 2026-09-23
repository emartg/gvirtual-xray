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
*   @file       MaterialSet.cxx
*
*   @brief      Class to manage a table of materials.
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
#include <fstream>

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_MATERIAL_SET_H
#include "gVirtualXRay/MaterialSet.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "correlation_CT_number_and_tissue_material.csv.h"


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//-------------------------------------
MaterialSet& MaterialSet::getInstance()
//-------------------------------------
{
    static MaterialSet m_instance; // Guaranteed to be destroyed.
                                   // Instantiated on first use.

    return (m_instance);
}


//----------------------------------------------------------
void MaterialSet::loadFromFile(const std::string& aFileName)
//----------------------------------------------------------
{
	//	Clear all the memory memory
	clearMemory();

	// Open the file
	std::ifstream input_file(aFileName.c_str());

	// The file cannot be opened
	if (!input_file.is_open())
	{
		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("ERROR: cannot read the material table file using ") + aFileName);
	}

	// Skip the first line
	char first_line_[1024];
	if (input_file.getline(first_line_, 1024).fail())
	{
		// Close the file
		input_file.close();

		// Throw an error
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("ERROR: fail reading the first line of ") + aFileName);
	}

	// Read all the materials in the file,
	// and add them to the table
	while (!input_file.eof())
	{
		// Read the current material
		TissueMaterial tissue_material;
		input_file >> tissue_material;

		// Add the material to the table
		if (!(tissue_material.getMinHUValue() == 0 && tissue_material.getMaxHUValue() == 0))
		{
			m_material_set.insert(std::pair<std::pair<short, short>, TissueMaterial>(std::pair<short, short>(tissue_material.getMinHUValue(), tissue_material.getMaxHUValue()), tissue_material));
		}
	}
}


//-------------------------------------------
void MaterialSet::loadMaterialSetFromMemory()
//-------------------------------------------
{
    // Clear the data
    clearMemory();

    char* p_dest(0);
    int z_lib_return_code = inflate(g_correlation_CT_number_and_tissue_material_csv, sizeof(g_correlation_CT_number_and_tissue_material_csv), &p_dest);

    // It does not exist
    if (!z_lib_return_code)
    {
        delete [] p_dest;
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No material was decoded.");
    }

    // Open the file
    std::stringstream material_stream;
    material_stream << (char*)p_dest;
    delete [] p_dest;

    // It does not exist
    if (!material_stream.str().size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No material was provided.");
    }

    // Ignore the first line
    char first_line[1024];
    material_stream.getline(first_line, 1024);

    // Process every record
    TissueMaterial temp;
    while (!material_stream.eof())
    {
        // Load the record
        material_stream >> temp;

        // The record is valid
        if (!material_stream.eof())
        {
            // Store the record
            addMaterial(temp);
        }
    }
}


//------------------------------------------------------------
void MaterialSet::addMaterial(const TissueMaterial& aMaterial)
//------------------------------------------------------------
{
    m_material_set.insert(std::pair<std::pair<short, short>, TissueMaterial>(std::pair<short, short>(aMaterial.getMinHUValue(), aMaterial.getMaxHUValue()), aMaterial));
}


//------------------------------------------------------------------
const TissueMaterial& MaterialSet::getMaterial(short aHUValue) const
//------------------------------------------------------------------
{
	// The material table is empty
	if (m_material_set.size() == 0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "ERROR: the material table is empty.");
	}

	// This iterator is used in case of the HU is higher than the last record in the table
	std::map<std::pair<short, short>, TissueMaterial>::const_iterator last_ite(m_material_set.begin());

	// Find the record that fits aHUValue
	for (std::map<std::pair<short, short>, TissueMaterial>::const_iterator ite(m_material_set.begin());
			ite != m_material_set.end();
			++ite)
	{
		if (aHUValue <= (ite->first).second)
		{
			return ite->second;
		}

		last_ite = ite;
	}

	// No record has been found, use the last record in the table
	return (last_ite->second);
}
