/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkFFTWComplexToComplex1DFFTImageFilter_h
#define itkFFTWComplexToComplex1DFFTImageFilter_h

#include "itkComplexToComplex1DFFTImageFilter.h"
#include "itkFFTWCommonExtended.h"

#include <vector>


namespace itk
{

/** \class FFTWComplexToComplex1DFFTImageFilter
 * \brief only do FFT along one dimension using FFTW as a backend.
 *
 * \ingroup Ultrasound
 */
template< typename TInputImage, typename TOutputImage >
class FFTWComplexToComplex1DFFTImageFilter :
    public ComplexToComplex1DFFTImageFilter< TInputImage, TOutputImage >
{
public:
  typedef FFTWComplexToComplex1DFFTImageFilter                             Self;
  typedef ComplexToComplex1DFFTImageFilter< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer< Self >                                          Pointer;
  typedef SmartPointer< const Self >                                    ConstPointer;

  /** Standard class typedefs.*/
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /**
   * the proxy type is a wrapper for the fftw API
   * since the proxy is only defined over double and float,
   * trying to use any other pixel type is inoperative, as
   * is trying to use double if only the float FFTW1D version is
   * configured in, or float if only double is configured.
   */
  typedef typename fftw::ComplexToComplexProxy< typename TInputImage::PixelType::value_type > FFTW1DProxyType;
  typedef typename std::vector< typename FFTW1DProxyType::PlanType >     PlanArrayType;
  typedef typename std::vector< typename FFTW1DProxyType::ComplexType* > PlanBufferPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( FFTWComplexToComplex1DFFTImageFilter, ComplexToComplex1DFFTImageFilter );


protected:
  FFTWComplexToComplex1DFFTImageFilter(): m_PlanComputed( false ),
    m_LastImageSize( 0 )
  {}
  virtual ~FFTWComplexToComplex1DFFTImageFilter()
  {
  if ( m_PlanComputed )
    {
    this->DestroyPlans();
    }
  }

  virtual void BeforeThreadedGenerateData() ITK_OVERRIDE;
  virtual void ThreadedGenerateData( const OutputImageRegionType&, ThreadIdType threadID ) ITK_OVERRIDE;

private:
  FFTWComplexToComplex1DFFTImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Destroy FFTW Plans and associated buffers. */
  void DestroyPlans();

  bool                  m_PlanComputed;
  PlanArrayType         m_PlanArray;
  unsigned int          m_LastImageSize;
  PlanBufferPointerType m_InputBufferArray;
  PlanBufferPointerType m_OutputBufferArray;
};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTWComplexToComplex1DFFTImageFilter.hxx"
#endif

#endif //itkFFTWComplexToComplex1DFFTImageFilter_h