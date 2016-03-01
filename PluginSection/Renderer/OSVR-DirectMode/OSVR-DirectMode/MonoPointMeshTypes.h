/** @file
    @brief Header

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2015 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDED_MonoPointMeshTypes_h_GUID_33CEDC76_9C34_4F8A_935B_652409FE6B30
#define INCLUDED_MonoPointMeshTypes_h_GUID_33CEDC76_9C34_4F8A_935B_652409FE6B30

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <array>
#include <vector>

namespace osvr {
namespace renderkit {

    /// Holds a list of mappings from physical-display normalized
    /// coordinates to canonical-display normalized coordinates.
    /// There are an arbitrary number of points describing the mapping.
    /// Each point has a from and to vector.
    typedef std::vector<          //!< Vector of mappings, one per point
        std::array<               //!< 2-array of from, to coordinates
            std::array<double, 2> //!< 2-array of unit coordinates (x,y)
            ,
            2> > MonoPointDistortionMeshDescription;

    typedef std::vector< //!< One mapping per eye
        MonoPointDistortionMeshDescription> MonoPointDistortionMeshDescriptions;

} // namespace renderkit
} // namespace osvr
#endif // INCLUDED_MonoPointMeshTypes_h_GUID_33CEDC76_9C34_4F8A_935B_652409FE6B30
