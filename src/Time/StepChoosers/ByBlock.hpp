// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <cstddef>
#include <pup.h>
#include <pup_stl.h>  // IWYU pragma: keep
#include <utility>
#include <vector>

#include "Options/Options.hpp"
#include "Parallel/CharmPupable.hpp"
#include "Time/StepChoosers/StepChooser.hpp"  // IWYU pragma: keep
#include "Utilities/ErrorHandling/Error.hpp"
#include "Utilities/TMPL.hpp"

/// \cond
template <size_t VolumeDim>
class Element;
namespace Parallel {
template <typename Metavariables>
class GlobalCache;
}  // namespace Parallel
namespace domain {
namespace Tags {
template <size_t VolumeDim>
struct Element;
}  // namespace Tags
}  // namespace domain
/// \endcond

namespace StepChoosers {
/// Suggests specified step sizes in each block
template <typename StepChooserUse, size_t Dim>
class ByBlock : public StepChooser<StepChooserUse> {
 public:
  /// \cond
  ByBlock() = default;
  explicit ByBlock(CkMigrateMessage* /*unused*/) {}
  using PUP::able::register_constructor;
  WRAPPED_PUPable_decl_template(ByBlock);  // NOLINT
  /// \endcond

  struct Sizes {
    using type = std::vector<double>;
    static constexpr Options::String help{
        "Step sizes, indexed by block number"};
  };

  static constexpr Options::String help{
      "Suggests specified step sizes in each block"};
  using options = tmpl::list<Sizes>;

  explicit ByBlock(std::vector<double> sizes) : sizes_(std::move(sizes)) {}

  using argument_tags = tmpl::list<domain::Tags::Element<Dim>>;
  using return_tags = tmpl::list<>;

  template <typename Metavariables>
  std::pair<double, bool> operator()(
      const Element<Dim>& element, const double /*last_step_magnitude*/,
      const Parallel::GlobalCache<Metavariables>& /*cache*/) const {
    const size_t block = element.id().block_id();
    if (block >= sizes_.size()) {
      ERROR("Step size not specified for block " << block);
    }
    return std::make_pair(sizes_[block], true);
  }

  // NOLINTNEXTLINE(google-runtime-references)
  void pup(PUP::er& p) override { p | sizes_; }

 private:
  std::vector<double> sizes_;
};

/// \cond
template <typename StepChooserUse, size_t Dim>
PUP::able::PUP_ID ByBlock<StepChooserUse, Dim>::my_PUP_ID = 0;  // NOLINT
/// \endcond
}  // namespace StepChoosers
