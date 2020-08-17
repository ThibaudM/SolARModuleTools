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

#ifndef SOLAR3DTRANSFORMESTIMATIONSACFROM3D3D_H
#define SOLAR3DTRANSFORMESTIMATIONSACFROM3D3D_H
#include <vector>
#include <string>
#include "api/solver/pose/I3DTransformSACFinderFrom3D3D.h"
#include "api/geom/I3DTransform.h"
#include "datastructure/Image.h"
#include "SolARToolsAPI.h"
#include "xpcf/component/ConfigurableBase.h"

namespace SolAR {
using namespace datastructure;
using namespace api::geom;
namespace MODULES {
namespace TOOLS {

/**
* @class SolAR3DTransformEstimationSACFrom3D3D
* @brief <B>Finds the 3D transform of 3D-3D points correspondences with a SAmple Consensus.</B>
* <TT>UUID: 3b7a1117-8b59-46b1-8e0c-6e76a8377ab4</TT>
*
*/

class SOLAR_TOOLS_EXPORT_API SolAR3DTransformEstimationSACFrom3D3D : public org::bcom::xpcf::ConfigurableBase,
    public api::solver::pose::I3DTransformSACFinderFrom3D3D
{
public:
    ///@brief SolAR3DTransformEstimationFrom3D3D constructor;
	SolAR3DTransformEstimationSACFrom3D3D();
    ///@brief SolAR3DTransformEstimationFrom3D3D destructor;
    ~SolAR3DTransformEstimationSACFrom3D3D() = default;

	/// @brief Estimates camera pose from a set of 3D-3D point correspondences.
	/// @param[in] firstPoints3D: first set of 3D points.
	/// @param[in] secondPoints3D: second set of 3D points.
	/// @param[out] pose: 3D transformation maps the first set of 3D points to the second one.
	/// @param[out] inliers: indices of inlier correspondences.
	FrameworkReturnCode estimate(const std::vector<Point3Df> & firstPoints3D,
								const std::vector<Point3Df> & secondPoints3D,
								Transform3Df & pose,
								std::vector<int> &inliers) override;

    void unloadComponent () override final;

private:
	/// @brief Number of iterations
	int m_iterationsCount = 500;

	/// @brief Inlier threshold value used by the RANSAC procedure. The parameter value is the maximum allowed distance between the observed and computed point projections to consider it an inlier.
	float m_reprojError = 4.0;

	/// @brief The probability that the algorithm produces a useful result.
	float m_confidence = 0.99f;

	/// @brief The minimum of number of inliers to valid a good pose estimation
	int m_NbInliersToValidPose = 10;

	/// @brief Transform 3D
	SRef<I3DTransform> m_transform3D;
};

}
}
}

#endif // SOLAR3DTRANSFORMESTIMATIONSACFROM3D3D_H
