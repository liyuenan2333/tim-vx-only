/****************************************************************************
*   Generated by ACUITY 6.6.0
*   Match timvx 1.1.30
*
*   Neural Network appliction network definition header file
****************************************************************************/
#ifndef _VX_MOBILENET_H
#define _VX_MOBILENET_H

#include "tim/vx/operation.h"
#include "tim/vx/tensor.h"
#include "tim/vx/graph.h"
#include "tim/vx/ops.h"

namespace acuitylite
{

class mobilenet
{
    public:
        using input_0_type = uint8_t;
        using output_0_type = uint8_t;
        static std::vector<std::vector<uint32_t>> input_size_list;
        static std::vector<uint32_t> input_bytes_list;
        static std::vector<std::vector<uint32_t>> output_size_list;
        static std::vector<std::shared_ptr<tim::vx::Tensor>> inputs_tensor;
        static std::vector<std::shared_ptr<tim::vx::Tensor>> outputs_tensor;

        static void construct_graph(std::shared_ptr<tim::vx::Graph> graph, const char *data_file_name);
};

}  // namespace acuitylite

#endif
