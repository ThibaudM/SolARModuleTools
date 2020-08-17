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
#include "SolARKeyframesManager.h"
#include "SolARPointCloudManager.h"
#include "SolARCovisibilityGraph.h"
#include "SolARBoostCovisibilityGraph.h"
#include "SolAR3D3DcorrespondencesFinder.h"
#include "SolAR3DTransformEstimationSACFrom3D3D.h"
#include "SolARLoopClosureDetector.h"
#include "SolARLoopCorrector.h"
#include "SolARFiducialMarkerPoseEstimator.h"
#include "SolARSLAMBootstrapper.h"
#include "SolARSLAMTracking.h"
#include "SolARSLAMMapping.h"
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
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARKeyframesManager>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARPointCloudManager>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARCovisibilityGraph>(componentUUID,interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARBoostCovisibilityGraph>(componentUUID,interfaceRef);
    }
	if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
	{
		errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolAR3D3DCorrespondencesFinder>(componentUUID, interfaceRef);
	}
	if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
	{
		errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolAR3DTransformEstimationSACFrom3D3D>(componentUUID, interfaceRef);
	}
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARLoopClosureDetector>(componentUUID, interfaceRef);
    }
    if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
    {
        errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARLoopCorrector>(componentUUID, interfaceRef);
    }
	if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
	{
		errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARFiducialMarkerPoseEstimator>(componentUUID, interfaceRef);
	}
	if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
	{
		errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARSLAMBootstrapper>(componentUUID, interfaceRef);
	}
	if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
	{
		errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARSLAMTracking>(componentUUID, interfaceRef);
	}
	if (errCode != xpcf::XPCFErrorCode::_SUCCESS)
	{
		errCode = xpcf::tryCreateComponent<SolAR::MODULES::TOOLS::SolARSLAMMapping>(componentUUID, interfaceRef);
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
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARKeyframesManager)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARPointCloudManager)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARCovisibilityGraph)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARBoostCovisibilityGraph)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolAR3D3DCorrespondencesFinder)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolAR3DTransformEstimationSACFrom3D3D)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARLoopClosureDetector)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARLoopCorrector)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARFiducialMarkerPoseEstimator)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARSLAMBootstrapper)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARSLAMTracking)
XPCF_ADD_COMPONENT(SolAR::MODULES::TOOLS::SolARSLAMMapping)

XPCF_END_COMPONENTS_DECLARATION

