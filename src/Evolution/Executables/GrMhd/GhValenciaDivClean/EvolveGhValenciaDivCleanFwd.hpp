// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <cstddef>

// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

/// \cond
namespace GeneralizedHarmonic {
template <size_t Dim>
struct System;
namespace Solutions {
template <typename GrSolution>
struct WrappedGr;
}  // namespace Solutions
}  // namespace GeneralizedHarmonic

namespace RelativisticEuler::Solutions {
class TovStar;
}  // namespace RelativisticEuler::Solutions

namespace grmhd {
namespace Solutions {
class AlfvenWave;
class KomissarovShock;
class SmoothFlow;
}  // namespace Solutions
namespace AnalyticData {
class BlastWave;
class MagneticFieldLoop;
class MagneticRotor;
class OrszagTangVortex;
}  // namespace AnalyticData
}  // namespace grmhd

struct KerrHorizon;
template <typename InitialData, typename... InterpolationTargetTags>
struct EvolutionMetavars;
/// \endcond
