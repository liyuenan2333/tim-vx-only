/****************************************************************************
 *
 *    Copyright (c) 2020 Vivante Corporation
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a
 *    copy of this software and associated documentation files (the "Software"),
 *    to deal in the Software without restriction, including without limitation
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *    and/or sell copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *    DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#ifndef TIM_LAYOUT_INFER_STACK_LAYOUT_INFERENCE_H_
#define TIM_LAYOUT_INFER_STACK_LAYOUT_INFERENCE_H_

#include "tim/vx/ops/stack.h"
#include "tim/vx/ops/transpose.h"

#include "direct_map_op_impl.h"
#include "permute_vector.h"
#include "ops/op_layout_inference.h"

namespace tim {
namespace transform {
class StackLayoutInfer : public OpLayoutInfer {
 public:
  StackLayoutInfer(
      const std::shared_ptr<vx::Operation> op,
      std::shared_ptr<layout_inference_impl::LayoutInferContext>& context)
      : OpLayoutInfer(op, context) {}
  void OnInputs(
      std::vector<std::shared_ptr<vx::Tensor>>& next_tensors) override {
    auto src_input = op_->impl()->InputsTensor()[0];
    auto input_pv = context_->GetPermuteVector(src_input);

    int32_t axis = op_->impl()->node()->nn_param.stack.axis;
    auto stack = context_->infer_graph_->CreateOperation<vx::ops::Stack>(
        axis, op_->impl()->input_cnt_);
    auto aligninput_pv = AlignPermuteVectorForMutilInputs();

    for (const auto& i_src : op_->impl()->InputsTensor()) {
      (*stack).BindInput(context_->GetMapedTensor(i_src));
    }

    std::vector<uint32_t> v;
    uint32_t dim_num = src_input->GetShape().size();
    if (axis < 0) {
      axis += dim_num;
    }
    for (uint32_t i = 0; i < src_input->GetShape().size(); ++i) {
      if (input_pv->At(i) > (uint32_t)axis) {
        v.push_back(input_pv->At(i) + 1);
      } else if (input_pv->At(i) == (uint32_t)axis) {
        v.push_back(input_pv->At(i));
        v.push_back(input_pv->At(i) + 1);
      } else {
        v.push_back(input_pv->At(i));
      }
    }
    auto out_pv =
        MakeShared(op_->impl()->OutputsTensor()[0]->GetShape().size());
    for (uint32_t i = 0; i < out_pv->Rank(); ++i) {
      out_pv->At(i) = v[i];
    }

    auto out_infer = CreateOutputsTensor(out_pv);
    (*stack).BindOutput(out_infer[0]);
    context_->SetPermuteVector(op_->impl()->OutputsTensor()[0], out_pv);
    // Add out tensor of src_graph into next_tensor
    next_tensors.push_back(op_->impl()->OutputsTensor()[0]);
  }
};

}  // namespace transform
}  // namespace tim

#endif