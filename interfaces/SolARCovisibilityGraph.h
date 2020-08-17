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

#ifndef SOLARCOVISIBILITYGRAPH_H
#define SOLARCOVISIBILITYGRAPH_H

#include "api/storage/ICovisibilityGraph.h"
#include "xpcf/component/ComponentBase.h"
#include "SolARToolsAPI.h"
#include <fstream>
#include <core/SerializationDefinitions.h>

namespace SolAR {
using namespace datastructure;
namespace MODULES {
namespace TOOLS {
/**
 * @class SolARVisibilityStorageBiMap
 * @brief A storage component to store with persistence the visibility between keypoints and 3D points, and respectively, based on a bimap from boost.
 */
class SOLAR_TOOLS_EXPORT_API SolARCovisibilityGraph : public org::bcom::xpcf::ComponentBase,
        public api::storage::ICovisibilityGraph {
public:

    SolARCovisibilityGraph();
    ~SolARCovisibilityGraph() = default;

	/// @brief This method allow to increase edge between 2 nodes
	/// @param[in] id of 1st node
	/// @param[in] id of 2nd node
	/// @param[in] weight to increase
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode increaseEdge(uint32_t node1_id, uint32_t node2_id, float weight) override;

	/// @brief This method allow to decrease edge between 2 nodes
	/// @param[in] id of 1st node
	/// @param[in] id of 2nd node
	/// @param[in] weight to decrease
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode decreaseEdge(uint32_t node1_id, uint32_t node2_id, float weight) override;

	/// @brief This method allow to remove an edge between 2 nodes
	/// @param[in] id of 1st node
	/// @param[in] id of 2nd node
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode removeEdge(uint32_t node1_id, uint32_t node2_id) override;

	/// @brief This method allow to get edge between 2 nodes
	/// @param[in] id of 1st node
	/// @param[in] id of 2nd node
	/// @param[out] weight of the edge
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode getEdge(uint32_t node1_id, uint32_t node2_id, float &weight) override;

	/// @brief This method allow to verify that exist an edge between 2 nodes
	/// @param[in] id of 1st node
	/// @param[in] id of 2nd node
	/// @return true if exist, else false
	bool isEdge(uint32_t node1_id, uint32_t node2_id) override;

	/// @brief This method allow to get all nodes of the graph
	/// @param[out] ids of all nodes
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode getAllNodes(std::set<uint32_t> &nodes_id) override;

	/// @brief This method allow to suppress a node of the graph
	/// @param[in] id of the node to suppress
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode suppressNode(uint32_t node_id) override;

	/// @brief This method allow to get neighbors of a node in the graph
	/// @param[in] id of the node to get neighbors
	/// @param[in] min value between this node and a neighbor to accept
	/// @param[out] a vector of neighbors sorted to greater weighted edge.
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode getNeighbors(uint32_t node_id, float minWeight, std::vector<uint32_t> &neighbors) override;

	/// @brief This method allow to get minimal spanning tree of the graph
	/// @param[out] edges_weights: the minimal spanning tree graph including edges with weights
	/// @param[out] minTotalWeights: cost of the minimal spanning tree graph
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode minimalSpanningTree(std::vector<std::tuple<uint32_t, uint32_t, float>> &edges_weights, float &minTotalWeights) override;

	/// @brief This method allow to get maximal spanning tree of the graph
	/// @param[out] edges_weights: the maximal spanning tree graph including edges with weights
	/// @param[out] maxTotalWeights: cost of the maximal spanning tree graph
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode maximalSpanningTree(std::vector<std::tuple<uint32_t, uint32_t, float>> &edges_weights, float &maxTotalWeights) override;

	/// @brief This method allow to get the shortest (by number of vertices) path between 2 nodes
	/// @param[in] id of 1st node
	/// @param[in] id of 2nd node
	/// @param[out] the shortest path
	/// @return FrameworkReturnCode::_SUCCESS_ if the addition succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode getShortestPath(uint32_t node1_id, uint32_t node2_id, std::vector<uint32_t> &path) override;

	/// @brief This method allow to display all vertices and weighted edges of the covisibility graph
	FrameworkReturnCode display() override;

	/// @brief This method allows to save the graph to the external file
	/// @param[out] the file name
	/// @return FrameworkReturnCode::_SUCCESS_ if the suppression succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode saveToFile(std::string file) override;

	/// @brief This method allows to load the graph from the external file
	/// @param[in] the file name
	/// @return FrameworkReturnCode::_SUCCESS_ if the suppression succeed, else FrameworkReturnCode::_ERROR.
	FrameworkReturnCode loadFromFile(std::string file) override;
    
	void unloadComponent () override final;

 private:
	 std::set<uint32_t>						m_nodes;
	 std::map<uint32_t, std::set<uint32_t>> m_edges;
	 std::map<uint64_t, float>				m_weights;
};

}
}
}

#endif // SOLARCOVISIBILITYGRAPH_H
