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

#ifndef INCLUDED_RGBPointMeshTypes_h_GUID_33CEDC76_9C34_4F8A_935B_652409FE6B30
#define INCLUDED_RGBPointMeshTypes_h_GUID_33CEDC76_9C34_4F8A_935B_652409FE6B30

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <array>
#include <vector>
#include "MonoPointMeshTypes.h"

namespace osvr {
namespace renderkit {

    typedef std::array< //!< One mapping per color red, green, blue
        std::vector<    //!< One mapping per eye
            MonoPointDistortionMeshDescription>,
        3> RGBPointDistortionMeshDescriptions;

} // namespace renderkit
} // namespace osvr
#endif // INCLUDED_RGBPointMeshTypes_h_GUID_33CEDC76_9C34_4F8A_935B_652409FE6B30
