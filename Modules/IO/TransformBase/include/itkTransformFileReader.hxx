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
#ifndef __itkTransformFileReader_hxx
#define __itkTransformFileReader_hxx

#include "itkTransformFileReader.h"
#include "itkTransformIOFactory.h"
#include "itkCompositeTransformIOHelper.h"

namespace itk
{
/** Constructor */
template<class ScalarType>
TransformFileReaderTemplate<ScalarType>
::TransformFileReaderTemplate() :
  m_FileName("") /* to be removed soon. See .h */
{
}

/** Destructor */
template<class ScalarType>
TransformFileReaderTemplate<ScalarType>
::~TransformFileReaderTemplate()
{
}

template<class ScalarType>
void TransformFileReaderTemplate<ScalarType>
::Update()
{
  if ( m_FileName == "" )
    {
    itkExceptionMacro ("No file name given");
    }
  typename TransformIOBaseTemplate<ScalarType>::Pointer transformIO =
      TransformIOFactoryTemplate<ScalarType>::CreateTransformIO( m_FileName.c_str(), /*TransformIOFactoryTemplate<ScalarType>::*/ ReadMode );
  if ( transformIO.IsNull() )
    {
    itkExceptionMacro("Can't Create IO object for file "
                      << m_FileName);
    }

  transformIO->SetFileName(m_FileName);
  transformIO->Read();

  typename TransformIOBaseTemplate<ScalarType>::TransformListType &ioTransformList =
  transformIO->GetTransformList();

    // In the case where the first transform in the list is a
    // CompositeTransform, add all the transforms to that first
    // transform. and return a single composite item on the
    // m_TransformList
  const std::string firstTransformName = ioTransformList.front()->GetNameOfClass();
  if(firstTransformName.find("CompositeTransform") != std::string::npos)
    {
    typename TransformListType::const_iterator tit = ioTransformList.begin();
    typename TransformType::Pointer composite = (*tit).GetPointer();
      //
      // CompositeTransformIOHelperTemplate knows how to assign to the composite
      // transform's internal list
    CompositeTransformIOHelperTemplate<ScalarType> helper;
    helper.SetTransformList(composite.GetPointer(),ioTransformList);

    this->m_TransformList.push_back( composite.GetPointer() );
    }
  else  //Just return the entire list of elements
    {
    for ( typename TransformListType::iterator it =
         ioTransformList.begin();
         it != ioTransformList.end(); ++it )
      {
      this->m_TransformList.push_back( (*it).GetPointer() );
      }
    }
}

template<class ScalarType>
void TransformFileReaderTemplate<ScalarType>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << m_FileName << std::endl;
}

} // namespace itk

#endif
