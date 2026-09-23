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


#ifndef GVXR_PHOTON_CROSS_SECTION_H
#define GVXR_PHOTON_CROSS_SECTION_H


/**
 ********************************************************************************
 *
 *   @file       PhotonCrossSection.h
 *
 *   @brief      Class to manage photon cross sections of elements, compounds and mixtures.
 *
 *   @version    1.0
 *
 *   @date       27/09/2018
 *
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
#ifndef GVXR_MIXTURE_H
#include "gVirtualXRay/Mixture.h"
#endif


//******************************************************************************
//  defines
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

class PhotonCrossSection;

std::ostream& operator<<(std::ostream& anOuputStream,
  const PhotonCrossSection& anInstance);


    //==============================================================================
    /**
     *   @class  PhotonCrossSection
     *   @brief  PhotonCrossSection is a class to manage photon cross sections of
     *           elements, compounds and mixtures.
     */
    //==============================================================================
    class PhotonCrossSection
    //------------------------------------------------------------------------------
    {
    //******************************************************************************
    public:
        PhotonCrossSection();
        explicit PhotonCrossSection(unsigned short Z);
        PhotonCrossSection(const Mixture& aMixture);
        PhotonCrossSection(const std::map<int, double>& aWeightSet);
        PhotonCrossSection(const std::map<std::string, double>& aWeightSet);

        void reset();

        void useHU(short HU);
        void useHounsfieldUnit(short HU);
        void useHounsfieldValue(short HU);

        void useElement(unsigned short Z);
        void useElement(const std::string& aName);

        void useMixture(const Mixture& aMixture);
        void useMixture(const std::map<int, double>& aMixture);
        void useMixture(const std::map<std::string, double>& aMixture);
        void useMixture(const std::string& aName);

        void useCompound(const std::string& aName);

        void useMassAttenuationCoefficient(double aCoefficient);
        void useLinearAttenuationCoefficient(double aCoefficient);

        void setDensity(double aDensity);

        [[nodiscard]] double getEffectiveZNumber() const;
        [[nodiscard]] double getEffectiveAtomicNumber() const;
        [[nodiscard]] double getEffectiveMolarMass() const;
        [[nodiscard]] double getMassAttenuationCoefficient(double anEnergy) const;
        [[nodiscard]] double getLinearAttenuationCoefficient(double anEnergy) const;
        [[nodiscard]] double getMu(double anEnergy) const;
        [[nodiscard]] double getDensity() const;
        [[nodiscard]] double getMolarMass() const;
        [[nodiscard]] const std::string& getLabel() const;

        [[nodiscard]] bool operator==(const PhotonCrossSection& aTestData) const;
        [[nodiscard]] bool operator!=(const PhotonCrossSection& aTestData) const;

        [[nodiscard]] static std::map<int, double> getMixtureWeightSet(const std::string& aName);
        [[nodiscard]] static std::map<int, double> getCompoundWeightSet(const std::string& aName);
        [[nodiscard]] static double getCompoundMolarMass(const std::string& aName);


        [[nodiscard]] bool useHU() const;
        [[nodiscard]] bool useHounsfieldUnit() const;
        [[nodiscard]] bool useHounsfieldValue() const;
        [[nodiscard]] bool useElement() const;
        [[nodiscard]] bool useMixture() const;
        [[nodiscard]] bool useMassAttenuationCoefficient() const;
        [[nodiscard]] bool useLinearAttenuationCoefficient() const;


        //------------------------------------------------------------------------------
        /// Accessor on the compound description of the polygon mesh.
        /**
         *  @return if the sample is made of a compound, then return the description of the compound (e.g. H2O for water), otherwise return an empty string
         */
         //------------------------------------------------------------------------------
        [[nodiscard]] std::string getCompound() const;


        //------------------------------------------------------------------------------
        /// Accessor on the element Z number set of the mixture of the polygon mesh.
        /**
         *  @return if the sample is made of a mixture, then return the element Z number set of the mixture, otherwise return an empty vector
         */
         //------------------------------------------------------------------------------
        [[nodiscard]] std::vector<int> getMixtureElementSet() const;


        //------------------------------------------------------------------------------
        /// Accessor on the element weight set of the mixture of the polygon mesh.
        /**
         *  @return if the sample is made of a mixture, then return the element weight set of the mixture, otherwise return an empty vector
         */
         //------------------------------------------------------------------------------
        [[nodiscard]] std::vector<double> getMixtureWeightSet() const;


        friend std::ostream& operator<<(std::ostream& anOuputStream,
                                        const PhotonCrossSection& anInstance);


    //*****************************************************************************
    private:
        static void processElement(std::string& aSymbol,
                std::string& aWeight,
                std::map<int, double>& aWeightSet);


        bool  m_use_HU;
        bool  m_use_element;
        bool  m_use_mixture;
        bool  m_use_mass_attenuation_coefficient;
        bool  m_use_linear_attenuation_coefficient;

        short m_HU;
        unsigned short m_Z_number;
        double m_mass_attenuation_coefficient;
        double m_linear_attenuation_coefficient;
        Mixture m_mixture;
        double m_density;
        std::string m_label;
    };


} // namespace gVirtualXRay


//******************************************************************************
#include "PhotonCrossSection.inl"


#endif // GVXR_PHOTON_CROSS_SECTION_H
