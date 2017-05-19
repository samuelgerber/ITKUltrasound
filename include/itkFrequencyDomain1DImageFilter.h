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
#ifndef itkFrequencyDomain1DImageFilter_h
#define itkFrequencyDomain1DImageFilter_h

#include "itkImageRegionSplitterDirection.h"
#include "itkFrequencyDomain1DFilterFunction.h"

namespace itk
{
/** \class FrequencyDomain1DImageFilter
 * \brief Generates the analytic signal from one direction of an image.
 *
 * Apply a 1D filter along a given direction (SetDirection) to an input image.
 * The input image is expected to be in frequnecy domain.
 * The default filter is an identity function. To implement a partiuclar filter
 * subclass FrequencyDomain1DFilterFunction and override the 
 * FrequencyDomain1DImageFilter::Evaluate function 
 * \ingroup FourierTransform
 * \ingroup Ultrasound
 */
template< typename TInputImage, typename TOutputImage = TInputImage >
class FrequencyDomain1DImageFilter:
  public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef TInputImage                                         InputImageType;
  typedef TOutputImage                                        OutputImageType;
  typedef typename OutputImageType::RegionType                OutputImageRegionType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  typedef FrequencyDomain1DImageFilter                             Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;

  itkTypeMacro( FrequencyDomain1DImageFilter, ImageToImageFilter );
  itkNewMacro( Self );


  itkSetMacro(Direction, unsigned int); 
  itkGetMacro(Direction, unsigned int);

  void SetFilterFunction(FrequencyDomain1DFilterFunction *function)
    {
    m_FilterFunction = function;
    };
 
protected:
  FrequencyDomain1DImageFilter();
  virtual ~FrequencyDomain1DImageFilter() {}

  virtual void PrintSelf(std::ostream& os, Indent indent) const ITK_OVERRIDE;

  // These behave like their analogs in Forward1DFFTImageFilter.
  virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;
  virtual void EnlargeOutputRequestedRegion(DataObject *output) ITK_OVERRIDE;

  virtual void BeforeThreadedGenerateData() ITK_OVERRIDE;
  virtual void ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, ThreadIdType threadId ) ITK_OVERRIDE;
  virtual void AfterThreadedGenerateData() ITK_OVERRIDE;



  /** Override to return a splitter that does not split along the direction we
   * are performing the transform. */
  virtual const ImageRegionSplitterBase* GetImageRegionSplitter() const ITK_OVERRIDE;

private:
  FrequencyDomain1DImageFilter( const Self& ) ITK_DELETE_FUNCTION;
  void operator=( const Self& ) ITK_DELETE_FUNCTION;

  ImageRegionSplitterDirection::Pointer m_ImageRegionSplitter;

  FrequencyDomain1DFilterFunction::Pointer m_FilterFunction;
  unsigned int m_Direction;

};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFrequencyDomain1DImageFilter.hxx"
#endif

#endif // itkFrequencyDomain1DImageFilter_h
