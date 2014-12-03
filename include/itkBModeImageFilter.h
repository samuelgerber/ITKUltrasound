#ifndef __itkBModeImageFilter_h
#define __itkBModeImageFilter_h

#include "itkAddConstantToImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkRegionFromReferenceImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkLog10ImageFilter.h"

#include "itkAnalyticSignalImageFilter.h"

namespace itk
{
  /** 
   * @class BModeImageFilter
   *
   * @brief Create an ultrasound B-Mode (Brightness-Mode) image from raw
   * "RF" data.  The RF's envelope is calculated from the analytic signal and
   * logarithmic intensity transform is applied.
   *
   * Use SetDirection() to define the axis of propagation. 
   *
   * \sa ResampleRThetaToCartesianImageFilter
   *
   */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT BModeImageFilter :
  public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard "Self" typedef.   */
  typedef BModeImageFilter Self;

  /** The type of input image.   */
  typedef TInputImage InputImageType;

  /** Dimension of the input and output images. */
  itkStaticConstMacro (ImageDimension, unsigned int,
                       TInputImage::ImageDimension);
  
  /** Typedef support for the input image scalar value type. */
  typedef typename InputImageType::PixelType InputPixelType;

  /** The type of output image.   */
  typedef TOutputImage OutputImageType;

  /** Typedef support for the output image scalar value type. */
  typedef typename OutputImageType::PixelType OutputPixelType;

  /** Other convenient typedefs   */
  typedef typename InputImageType::RegionType InputRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  
  /** Standard super class typedef support. */
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;

  /** Smart pointer typedef support.  */
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro( BModeImageFilter, ImageToImageFilter );
  
  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Set the direction in which the envelope is to be calculated. */
  virtual void SetDirection( unsigned int direction )
    {
    this->m_AnalyticFilter->SetDirection( direction );
    this->Modified();
    }

  /** Get the direction in which the envelope is to be calculated. */
  virtual unsigned int GetDirection() const
    {
    return m_AnalyticFilter->GetDirection();
    }

protected:
  BModeImageFilter();
  ~BModeImageFilter() {}

  void PrintSelf( std::ostream& os, Indent indent ) const;

  virtual void GenerateData();

  // These behave like their analogs in FFT1DRealToComplexConjugateImageFilter.
  virtual void GenerateInputRequestedRegion(); 
  virtual void EnlargeOutputRequestedRegion(DataObject *output); 

  /** Component filters. */
  typedef itk::AnalyticSignalImageFilter< OutputPixelType, ImageDimension > AnalyticType;
  typedef itk::ComplexToModulusImageFilter< typename AnalyticType::OutputImageType, OutputImageType > ComplexToModulusType;
  typedef itk::ConstantPadImageFilter< InputImageType, InputImageType > PadType;
  typedef itk::AddConstantToImageFilter< InputImageType, InputPixelType, InputImageType > AddConstantType;
  typedef itk::Log10ImageFilter< InputImageType, OutputImageType > LogType;
  typedef itk::RegionFromReferenceImageFilter< OutputImageType, OutputImageType > ROIType;

private:
  BModeImageFilter( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented

  typename AnalyticType::Pointer m_AnalyticFilter;
  typename ComplexToModulusType::Pointer m_ComplexToModulusFilter;
  typename PadType::Pointer            m_PadFilter;
  typename AddConstantType::Pointer    m_AddConstantFilter;
  typename LogType::Pointer            m_LogFilter;
  typename ROIType::Pointer	       m_ROIFilter;
};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBModeImageFilter.txx"
#endif

#endif // __itkBModeImageFilter_h
