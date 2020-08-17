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

#ifndef SOLARBOOSTCOVISIBILITYGRAPH_H
#define SOLARBOOSTCOVISIBILITYGRAPH_H

#include "api/storage/ICovisibilityGraph.h"
#include "xpcf/component/ComponentBase.h"
#include "SolARToolsAPI.h"
#include <fstream>
#include <core/SerializationDefinitions.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/graphml.hpp>
#include <mutex>

namespace SolAR {
using namespace datastructure;
namespace MODULES {
namespace TOOLS {

using namespace boost;


/**
 * @class SolARBoostCovisibilityGraph
 * @brief A storage component to store with persistence the visibility between keypoints and 3D points, and respectively, based on a bimap from boost.
 */
class SOLAR_TOOLS_EXPORT_API SolARBoostCovisibilityGraph : public org::bcom::xpcf::ComponentBase,
        public api::storage::ICovisibilityGraph {
public:

    SolARBoostCovisibilityGraph();
    ~SolARBoostCovisibilityGraph() = default;

    /// @brief This method allow to increase edge between 2 nodes
    /// @param[in] id of 1st node
    /// @param[in] id of 2nd node
    /// @param[in] weight to increase
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode increaseEdge(uint32_t node1_id, uint32_t node2_id, float weight) override;

    /// @brief This method allow to decrease edge between 2 nodes
    /// @param[in] id of 1st node
    /// @param[in] id of 2nd node
    /// @param[in] weight to decrease
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode decreaseEdge(uint32_t node1_id, uint32_t node2_id, float weight) override;

    /// @brief This method allow to remove an edge between 2 nodes
    /// @param[in] id of 1st node
    /// @param[in] id of 2nd node
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode removeEdge(uint32_t node1_id, uint32_t node2_id) override;

    /// @brief This method allow to get edge between 2 nodes
    /// @param[in] id of 1st node
    /// @param[in] id of 2nd node
    /// @param[out] weight of the edge
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode getEdge(uint32_t node1_id, uint32_t node2_id, float &weight) override;

    /// @brief This method allow to verify that exist an edge between 2 nodes
    /// @param[in] id of 1st node
    /// @param[in] id of 2nd node
    /// @return true if exist, else false
    bool isEdge(uint32_t node1_id, uint32_t node2_id) override;

    /// @brief This method allow to get all nodes of the graph
    /// @param[out] ids of all nodes
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode getAllNodes(std::set<uint32_t> &nodes_id) override;

    /// @brief This method allow to suppress a node of the graph
    /// @param[in] id of the node to suppress
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode suppressNode(uint32_t node_id) override;

    /// @brief This method allow to get neighbors of a node in the graph
    /// @param[in] id of the node to get neighbors
    /// @param[in] min value between this node and a neighbor to accept
    /// @param[out] a vector of neighbors sorted to greater weighted edge.
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode getNeighbors(uint32_t node_id, float minWeight, std::vector<uint32_t> &neighbors) override;

    /// @brief This method allow to get minimal spanning tree of the graph
    /// @param[out] edges_weights: the minimal spanning tree graph including edges with weights
    /// @param[out] minTotalWeights: cost of the minimal spanning tree graph
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode minimalSpanningTree(std::vector<std::tuple<uint32_t, uint32_t, float>> &edges_weights, float &minTotalWeights) override;

    /// @brief This method allow to get maximal spanning tree of the graph
    /// @param[out] edges_weights: the maximal spanning tree graph including edges with weights
    /// @param[out] maxTotalWeights: cost of the maximal spanning tree graph
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode maximalSpanningTree(std::vector<std::tuple<uint32_t, uint32_t, float>> &edges_weights, float &maxTotalWeights) override;

    /// @brief This method allow to get the shortest (by number of vertices) path between 2 nodes
    /// @param[in] id of 1st node
    /// @param[in] id of 2nd node
    /// @param[out] the shortest path
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode getShortestPath(uint32_t node1_id, uint32_t node2_id, std::vector<uint32_t> &path) override;

    /// @brief This method allow to display all vertices and weighted edges of the covisibility graph
    FrameworkReturnCode display() override;

    /// @brief This method allows to save the graph to the external file
    /// @param[out] the file name
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode saveToFile(std::string file) override;

    /// @brief This method allows to load the graph from the external file
    /// @param[in] the file name
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode loadFromFile(std::string file) override;
    
    // new
    /// @brief This method clears the covisibility graph (deletes all nodes and edges).
    FrameworkReturnCode clear() ;

    /// @brief This method tests if node_id exists in the covisibility graph
    /// @param[in] id of the node to test
    /// @return true if the node is present in the graph.
    bool isNode(uint32_t node_id);

    /// @brief This method allows to suppress a node of the graph
    /// @param[in] id of the node to suppress
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode addNode(uint32_t node_id) ;

    /// @brief This method adds an edge to the covisibility graph
    /// @param[in] id of the node to add
    /// @return FrameworkReturnCode::_SUCCESS_ if the execution succeed, else FrameworkReturnCode::_ERROR.
    FrameworkReturnCode addEdge(uint32_t node_id_1, uint32_t node_id_2, float weight) ;

	void unloadComponent () override final;

 private:
    std::mutex m_mutex;

    // Defines properties structure attached to each vertices of the covisibility graph
    struct VertexProperties
    {
       uint32_t frame_id;
       float    distance;
       int      name;
       VertexProperties() : frame_id(0) {}
       VertexProperties(uint32_t id) : frame_id(id) {}
    };

    // Defines properties structure attached to each edges of the covisibility graph
    struct EdgeProperties
    {
       float weight ;
       float unit  =  1.0;
       EdgeProperties() : weight(0.0) {}
       EdgeProperties(float w) : weight(w) {}
    };

    // Internal boost graph representation
    typedef adjacency_list <
            listS,
            listS,                      // an optimize version should use lists as internal representations to avoid allocation lag. In this version of boost library, a bug avoid a robust use of labeled_graph with listS, listS
            undirectedS,                     // covisibility is an undirected graph weight(a,b) = weight(b,a)
            VertexProperties,
            EdgeProperties // use bundle properties for futur evolutions
    > CoGraph;

    // Defines internal types
    typedef graph_traits<CoGraph>::vertex_descriptor vertex_t;
    typedef graph_traits<CoGraph>::edge_descriptor   edge_t;
    typedef graph_traits<CoGraph>::vertex_iterator   vertex_iterator_t;
    typedef graph_traits<CoGraph>::edge_iterator     edge_iterator_t;
    typedef graph_traits<CoGraph>::in_edge_iterator  in_edge_iterator_t;
    typedef std::unordered_map<uint32_t, vertex_t>   CoMap;
    typedef std::map<vertex_t, vertex_t>             PredecessorMap;
    typedef std::map<vertex_t, int>                  IndexMap; // this map should be defined in [0, #V[
    typedef std::map<vertex_t, float>                DistanceMap;
    typedef std::map<edge_t,   float>                WeightMap;
    typedef std::pair<CoGraph::edge_descriptor, bool> edge_info_t;
    CoMap   m_map;    // private map < frame_id, vertex_t>
    CoGraph m_graph;  // private graph using boost::adjacency_list

};

}
}
}

#endif // SOLARBOOSTCOVISIBILITYGRAPH_H
