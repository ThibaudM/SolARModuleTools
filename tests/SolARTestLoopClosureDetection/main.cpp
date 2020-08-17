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

#include "xpcf/xpcf.h"
#include "api/solver/map/IMapper.h"
#include "api/reloc/IKeyframeRetriever.h"
#include "api/storage/ICovisibilityGraph.h"
#include "api/storage/IKeyframesManager.h"
#include "api/storage/IPointCloudManager.h"
#include "api/loop/ILoopClosureDetector.h"
#include "api/geom/I3DTransform.h"
#include "api/display/I3DPointsViewer.h"
#include "core/Log.h"
#include <boost/log/core.hpp>

using namespace SolAR;
using namespace SolAR::datastructure;
using namespace SolAR::api;

namespace xpcf  = org::bcom::xpcf;

int main(int argc,char** argv)
{
#if NDEBUG
    boost::log::core::get()->set_logging_enabled(false);
#endif

    LOG_ADD_LOG_TO_CONSOLE();
	/* instantiate component manager*/
	/* this is needed in dynamic mode */
	SRef<xpcf::IComponentManager> xpcfComponentManager = xpcf::getComponentManagerInstance();

	if (xpcfComponentManager->load("SolARTestLoopClosureDetection_config.xml") != org::bcom::xpcf::_SUCCESS)
	{
		LOG_ERROR("Failed to load the configuration file SolARTestLoopClosureDetection_config.xml")
			return -1;
	}

	// declare and create components
	LOG_INFO("Start creating components");

	// storage components
	auto pointCloudManager = xpcfComponentManager->resolve<IPointCloudManager>();
	auto keyframesManager = xpcfComponentManager->resolve<IKeyframesManager>();
	auto covisibilityGraph = xpcfComponentManager->resolve<ICovisibilityGraph>();
	auto keyframeRetriever = xpcfComponentManager->resolve<IKeyframeRetriever>();
	auto mapper = xpcfComponentManager->resolve<solver::map::IMapper>();
	auto loopDetector = xpcfComponentManager->resolve<loop::ILoopClosureDetector>();
	auto transform3D = xpcfComponentManager->resolve<geom::I3DTransform>();
	auto viewer3DPoints = xpcfComponentManager->resolve<display::I3DPointsViewer>();

	// Load map from file
	if (mapper->loadFromFile() == FrameworkReturnCode::_SUCCESS) {
		LOG_INFO("Load map done!");
	}
	else {
		LOG_INFO("Cannot load map");
		return 0;
	}

	LOG_INFO("Number of keyframes: {}", keyframesManager->getNbKeyframes());
	LOG_INFO("Number of cloud points: {}", pointCloudManager->getNbPoints());

	// get all keyframes and point cloud to display
	std::vector<Transform3Df> keyframePoses;
	std::vector<SRef<Keyframe>> allKeyframes;
	keyframesManager->getAllKeyframes(allKeyframes);	
	for (auto const &it : allKeyframes)
		keyframePoses.push_back(it->getPose());
	std::vector<SRef<CloudPoint>> pointCloud;
	pointCloudManager->getAllPoints(pointCloud);

	// test loop closure with the last keyframe
	SRef<Keyframe> lastKeyframe = allKeyframes[allKeyframes.size() - 1];
	uint32_t lastKeyframeId = lastKeyframe->getId();
	LOG_INFO("Last keyframe id: {}", lastKeyframeId);
	SRef<Keyframe> detectedLoopKeyframe;
	Transform3Df sim3Transform;
	std::vector<std::pair<uint32_t, uint32_t>> duplicatedPointsIndices;
	if (loopDetector->detect(lastKeyframe, detectedLoopKeyframe, sim3Transform, duplicatedPointsIndices) == FrameworkReturnCode::_SUCCESS) {
		// try to transform local point cloud of current keyframe to the detected loop keyframe
		std::vector<SRef<CloudPoint>> localPointCloud, localPointCloudTrans;
		std::vector<Point3Df> localPoint3D, localPoint3DTrans;
		mapper->getLocalPointCloud(lastKeyframe, 10, localPointCloud);
		for (auto it : localPointCloud)
			localPoint3D.push_back(Point3Df(it->getX(), it->getY(), it->getZ()));
		transform3D->transform(localPoint3D, sim3Transform, localPoint3DTrans);
		for (auto it : localPoint3DTrans)
			localPointCloudTrans.push_back(xpcf::utils::make_shared<CloudPoint>(it.getX(), it.getY(), it.getZ()));

		// detected loop keyframe
		LOG_INFO("Detected loop keyframe id: {}", detectedLoopKeyframe->getId());
		LOG_INFO("Transform 3D from last keyframe and best detected loop keyframe: \n{}", sim3Transform.matrix());
		Eigen::Matrix3f scale, rot;
		sim3Transform.computeScalingRotation(&scale, &rot);
		std::cout << "Scale : " << scale << std::endl;
		std::cout << "Rot : " << rot << std::endl;
		sim3Transform.linear() = rot;
		sim3Transform.translation() = sim3Transform.translation() / scale(0, 0);
		LOG_INFO("Transform 3D from last keyframe and best detected loop keyframe: \n{}", sim3Transform.matrix());
		// display point cloud
		while (viewer3DPoints->display(localPointCloudTrans, lastKeyframe->getPose(), { detectedLoopKeyframe->getPose() }, {}, pointCloud, keyframePoses) == FrameworkReturnCode::_SUCCESS);
	}
	else
		LOG_INFO("Cannot detect a loop closure from last keyframe");
	

    return 0;
}