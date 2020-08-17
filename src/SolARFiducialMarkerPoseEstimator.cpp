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

#include "SolARFiducialMarkerPoseEstimator.h"
#include "core/Log.h"


namespace xpcf = org::bcom::xpcf;

XPCF_DEFINE_FACTORY_CREATE_INSTANCE(SolAR::MODULES::TOOLS::SolARFiducialMarkerPoseEstimator);


namespace SolAR {
namespace MODULES {
namespace TOOLS {


SolARFiducialMarkerPoseEstimator::SolARFiducialMarkerPoseEstimator():ConfigurableBase(xpcf::toUUID<SolARFiducialMarkerPoseEstimator>())
{
	addInterface<api::solver::pose::IFiducialMarkerPose>(this);
	declareInjectable<api::input::files::IMarker2DSquaredBinary>(m_binaryMarker);
	declareInjectable<api::image::IImageFilter>(m_imageFilterBinary);
	declareInjectable<api::image::IImageConvertor>(m_imageConvertor);
	declareInjectable<api::features::IContoursExtractor>(m_contoursExtractor);
	declareInjectable<api::features::IContoursFilter>(m_contoursFilter);
	declareInjectable<api::image::IPerspectiveController>(m_perspectiveController);
	declareInjectable<api::features::IDescriptorsExtractorSBPattern>(m_patternDescriptorExtractor);
	declareInjectable<api::features::IDescriptorMatcher>(m_patternMatcher, "Matcher-Fiducial");
	declareInjectable<api::features::ISBPatternReIndexer>(m_patternReIndexer);
	declareInjectable<api::geom::IImage2WorldMapper>(m_img2worldMapper);
	declareInjectable<api::solver::pose::I3DTransformFinderFrom2D3D>(m_pnp);
	declareProperty("nbThreshold", m_nbThreshold);
	declareProperty("minThreshold", m_minThreshold);
	declareProperty("maxThreshold", m_maxThreshold);
}

void SolARFiducialMarkerPoseEstimator::setCameraParameters(const CamCalibration & intrinsicParams, const CamDistortion & distortionParams) {
	m_camMatrix = intrinsicParams;
	m_camDistortion = distortionParams;
	m_pnp->setCameraParameters(m_camMatrix, m_camDistortion);
	// components initialisation for marker detection
	m_binaryMarker->loadMarker();
	m_patternDescriptorExtractor->extract(m_binaryMarker->getPattern(), m_markerPatternDescriptor);
	LOG_DEBUG("Marker pattern:\n {}", m_binaryMarker->getPattern().getPatternMatrix());
	int patternSize = m_binaryMarker->getPattern().getSize();
	m_patternDescriptorExtractor->bindTo<xpcf::IConfigurable>()->getProperty("patternSize")->setIntegerValue(patternSize);
	m_patternReIndexer->bindTo<xpcf::IConfigurable>()->getProperty("sbPatternSize")->setIntegerValue(patternSize);
	m_img2worldMapper->bindTo<xpcf::IConfigurable>()->getProperty("digitalWidth")->setIntegerValue(patternSize);
	m_img2worldMapper->bindTo<xpcf::IConfigurable>()->getProperty("digitalHeight")->setIntegerValue(patternSize);
	m_img2worldMapper->bindTo<xpcf::IConfigurable>()->getProperty("worldWidth")->setFloatingValue(m_binaryMarker->getSize().width);
	m_img2worldMapper->bindTo<xpcf::IConfigurable>()->getProperty("worldHeight")->setFloatingValue(m_binaryMarker->getSize().height);
}

FrameworkReturnCode SolARFiducialMarkerPoseEstimator::estimate(const SRef<Image>& image, Transform3Df & pose)
{
	SRef<Image>                     greyImage, binaryImage;
	std::vector<Contour2Df>			contours;
	std::vector<Contour2Df>			filtered_contours;
	std::vector<SRef<Image>>        patches;
	std::vector<Contour2Df>			recognizedContours;
	SRef<DescriptorBuffer>          recognizedPatternsDescriptors;
	std::vector<DescriptorMatch>    patternMatches;
	std::vector<Point2Df>			pattern2DPoints;
	std::vector<Point2Df>			img2DPoints;
	std::vector<Point3Df>			pattern3DPoints;

	// Convert Image from RGB to grey
	m_imageConvertor->convert(image, greyImage, Image::ImageLayout::LAYOUT_GREY);
	for (int num_threshold = 0; num_threshold < m_nbThreshold; num_threshold++)
	{
		// Compute the current Threshold valu for image binarization
		int threshold = m_minThreshold + (m_maxThreshold - m_minThreshold)*((float)num_threshold / (float)(m_nbThreshold - 1));
		// Convert Image from grey to black and white
		m_imageFilterBinary->bindTo<xpcf::IConfigurable>()->getProperty("min")->setIntegerValue(threshold);
		m_imageFilterBinary->bindTo<xpcf::IConfigurable>()->getProperty("max")->setIntegerValue(255);
		// Convert Image from grey to black and white
		m_imageFilterBinary->filter(greyImage, binaryImage);
		// Extract contours from binary image
		m_contoursExtractor->extract(binaryImage, contours);
		// Filter 4 edges contours to find those candidate for marker contours
		m_contoursFilter->filter(contours, filtered_contours);
		// Create one warpped and cropped image by contour
		m_perspectiveController->correct(binaryImage, filtered_contours, patches);
		// test if this last image is really a squared binary marker, and if it is the case, extract its descriptor
		if (m_patternDescriptorExtractor->extract(patches, filtered_contours, recognizedPatternsDescriptors, recognizedContours) != FrameworkReturnCode::_ERROR_)
		{
			// From extracted squared binary pattern, match the one corresponding to the squared binary marker
			if (m_patternMatcher->match(m_markerPatternDescriptor, recognizedPatternsDescriptors, patternMatches) == api::features::IDescriptorMatcher::DESCRIPTORS_MATCHER_OK)
			{
				// Reindex the pattern to create two vector of points, the first one corresponding to marker corner, the second one corresponding to the poitsn of the contour
				m_patternReIndexer->reindex(recognizedContours, patternMatches, pattern2DPoints, img2DPoints);
				// Compute the 3D position of each corner of the marker
				m_img2worldMapper->map(pattern2DPoints, pattern3DPoints);
				// Compute the pose of the camera using a Perspective n Points algorithm using only the 4 corners of the marker
				if (m_pnp->estimate(img2DPoints, pattern3DPoints, pose) == FrameworkReturnCode::_SUCCESS)
				{
					return FrameworkReturnCode::_SUCCESS;
				}
			}
		}
	}
	return FrameworkReturnCode::_ERROR_;
}


}
}
}
