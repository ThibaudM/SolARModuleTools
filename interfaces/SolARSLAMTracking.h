/**
 * @copyright Copyright (c) 2017 B-com http://www.b-com.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SOLARSLAMTRACKING_H
#define SOLARSLAMTRACKING_H
#include "api/slam/ITracking.h"
#include "datastructure/Image.h"
#include "api/storage/IKeyframesManager.h"
#include "api/solver/map/IMapper.h"
#include "api/features/IDescriptorMatcher.h"
#include "api/features/IMatchesFilter.h"
#include "api/display/I2DOverlay.h"
#include "api/solver/pose/I2D3DCorrespondencesFinder.h"
#include "api/solver/pose/I3DTransformFinderFrom2D3D.h"
#include "api/solver/pose/I3DTransformSACFinderFrom2D3D.h"
#include "api/geom/IProject.h"
#include "api/reloc/IKeyframeRetriever.h"
#include "SolARToolsAPI.h"
#include "xpcf/component/ConfigurableBase.h"

namespace SolAR {
using namespace datastructure;
namespace MODULES {
namespace TOOLS {

/**
* @class SolARSLAMTracking
* @brief <B> SLAM tracking task.</B>
* <TT>UUID: c45da19d-9637-48b6-ab52-33d3f0af6f72</TT>
*
*/

class SOLAR_TOOLS_EXPORT_API SolARSLAMTracking : public org::bcom::xpcf::ConfigurableBase,
	public api::slam::ITracking
{
public:
	///@brief SolARSLAMTracking constructor;
	SolARSLAMTracking();

	///@brief SolARSLAMTracking destructor;
	~SolARSLAMTracking() = default;

	/// @brief this method is used to set intrinsic parameters and distorsion of the camera
	/// @param[in] Camera calibration matrix parameters.
	/// @param[in] Camera distorsion parameters.
	void setCameraParameters(const CamCalibration & intrinsicParams, const CamDistortion & distorsionParams) override;

	/// @brief this method is used to update reference keyframe to track
	/// @param[in] refKeyframe: the new reference keyframe.
	void updateReferenceKeyframe(const SRef<Keyframe> & refKeyframe) override;

	/// @brief this method is used to process tracking
	/// @param[in] frame: the input frame.
	/// @param[out] displayImage: the image to display.
	/// @return FrameworkReturnCode::_SUCCESS if tracking succeed, else FrameworkReturnCode::_ERROR_
	FrameworkReturnCode process(const SRef<Frame> & frame, SRef<Image> &displayImage) override;

	void unloadComponent() override final;

private:
	void updateLocalMap();

private:
	SRef<Keyframe>										m_referenceKeyframe;
	SRef<Frame>											m_frameToTrack;
	Transform3Df										m_lastPose;
	std::vector<SRef<CloudPoint>>						m_localMap;
	bool												m_isLostTrack = false;
	float												m_minWeightNeighbor = 10.f;
	int													m_displayTrackedPoints = 1;
	bool												m_isUpdateReferenceKeyframe = false;
	std::mutex											m_refKeyframeMutex;
	CamCalibration										m_camMatrix;
	CamDistortion										m_camDistortion;
	SRef<api::solver::map::IMapper>						m_mapper;
	SRef<api::features::IDescriptorMatcher>				m_matcher;
	SRef<api::features::IMatchesFilter>					m_matchesFilter;
	SRef<api::display::I2DOverlay>						m_overlay2D;
	SRef<api::solver::pose::I2D3DCorrespondencesFinder>	m_corr2D3DFinder;
	SRef<api::solver::pose::I3DTransformSACFinderFrom2D3D>	m_pnpRansac;
	SRef<api::solver::pose::I3DTransformFinderFrom2D3D>	m_pnp;
	SRef<api::geom::IProject>							m_projector;
	SRef<api::reloc::IKeyframeRetriever>				m_keyframeRetriever;
	SRef<api::storage::IKeyframesManager>				m_keyframesManager;
};

}
}
}

#endif // SOLARSLAMTRACKING_H
