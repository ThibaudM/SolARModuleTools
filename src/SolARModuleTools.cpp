/**
 * @copyright Copyright (c) 2015 All Right Reserved, B-com http://www.b-com.com/
 *
 * This file is subject to the B<>Com License.
 * All other rights reserved.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 */

#include "xpcf/module/ModuleFactory.h"

#include "SolARHomographyValidation.h"
#include "SolARImage2WorldMapper4Marker2D.h"
#include "SolARSBPatternReIndexer.h"
#include "SolARKeyframeSelector.h"
#include "SolARKeypointsReIndexer.h"
#include "SolAR2DTransform.h"
#include "SolAR3DTransform.h"
#include "SolAR3DTransformEstimationFrom3D3D.h"
#include "SolARBasicMatchesFilter.h"
#include "SolARMapFilter.h"
#include "SolARMapper.h"
#include "SolARBasicSink.h"
#include "SolARBasicSource.h"

#include <iostream>

namespace xpcf=org::bcom::xpcf;

XPCF_DECLARE_MODULE("28b89d39-41bd-451d-b19e-d25a3d7c5797", "SolARModuleTools","SolARModuleTools");

extern "C" XPCF_MODULEHOOKS_API xpcf::XPCFErrorCode XPCF_getComponent(const boost::uuids::uuid& componentUUID,SRef<xpcf::IComponentIntrospect>& interfaceRef)
{
    xpcf::XPCFErrorCode errCode = xpcf::XPCFErrorCode::_FAIL;
    errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARHomographyValidation>(componentUUID,interfaceRef);
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolAR2DTransform>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolAR3DTransform>(componentUUID,interfaceRef);
    }
	if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
	{
		errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolAR3DTransformEstimationFrom3D3D>(componentUUID, interfaceRef);
	}
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARBasicMatchesFilter>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARImage2WorldMapper4Marker2D>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARKeyframeSelector>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARKeypointsReIndexer>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARSBPatternReIndexer>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARMapper>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARMapFilter>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARBasicSink>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARBasicSource>(componentUUID,interfaceRef);
    }
      return errCode;
}


XPCF_BEGIN_COMPONENTS_DECLARATION
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARHomographyValidation)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARBasicMatchesFilter)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARImage2WorldMapper4Marker2D)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARKeypointsReIndexer)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARKeyframeSelector)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARSBPatternReIndexer)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolAR2DTransform)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolAR3DTransform)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolAR3DTransformEstimationFrom3D3D)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARMapper)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARMapFilter)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARBasicSink)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARBasicSource)
XPCF_END_COMPONENTS_DECLARATION

