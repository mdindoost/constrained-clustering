#ifndef CONSTRAINED_H
#define CONSTRAINED_H
// mincut_custom.h needs to be included first because of string override
#include "mincut_custom.h"

#include <cmath>
#include <chrono>
#include <condition_variable>
#include <thread>

#include <libleidenalg/GraphHelper.h>
#include <libleidenalg/Optimiser.h>
#include <libleidenalg/CPMVertexPartition.h>
#include <libleidenalg/ModularityVertexPartition.h>

enum Log {info, debug, error = -1};

class ConstrainedClustering {
    public:
        ConstrainedClustering(std::string edgelist, std::string algorithm, double resolution, int num_processors, std::string output_file, std::string log_file, int log_level) : edgelist(edgelist), algorithm(algorithm), resolution(resolution), num_processors(num_processors), output_file(output_file), log_file(log_file), log_level(log_level) {
            if(this->log_level > 0) {
                this->start_time = std::chrono::steady_clock::now();
                this->log_file_handle.open(this->log_file);
            }
        };
        virtual int main() = 0;
        int write_to_log_file(std::string message, Log message_type);
        void RemoveInterClusterEdges(igraph_t* graph, const std::map<int,int>& node_id_to_cluster_id_map);
        void write_partition_map(std::map<int,int>& final_partition);
        void start_workers(std::vector<std::map<int,int>>& results, igraph_t* graph);
        void write_cluster_queue(std::queue<std::vector<int>>& to_be_clustered_clusters);
        void cluster_queue_to_map(std::queue<std::vector<int>>& to_be_clustered_clusters, std::map<int, int>* node_id_to_cluster_id_map);
        virtual ~ConstrainedClustering() {
            if(this->log_level > 0) {
                this->log_file_handle.close();
            }
        }
    protected:
        std::string edgelist;
        std::string algorithm;
        double resolution;
        int num_processors;
        std::string output_file;
        std::string log_file;
        std::chrono::steady_clock::time_point start_time;
        std::ofstream log_file_handle;
        int log_level;
};

class MinCutGlobalClusterRepeat : public ConstrainedClustering {
    public:
        MinCutGlobalClusterRepeat(std::string edgelist, std::string algorithm, double resolution, int num_processors, std::string output_file, std::string log_file, int log_level) : ConstrainedClustering(edgelist, algorithm, resolution, num_processors, output_file, log_file, log_level) {
        };
        int main();
};

#endif