#pragma once

#include "Common.hpp"
#include "SceneDescriptor.hpp"

// Scenes from Book e (Ray Tracing the Rest of your life) of the Peter Shirley Raytracing minioooks


namespace Scenes {
    namespace Book3 {
        // There are a few variations on the cornel box we have, so we need to use an enum to differentaite
        enum class CornellBoxConfiguration {
            TwoBoxes = 0,
            TwoBoxesOneMirror,
            OneBoxOneGlassSphere,
            OneMirrorBoxOneGlassSphere
        };

        SceneDescriptor cornell_box(const rreal aspect_ratio, const CornellBoxConfiguration config);
    }
}

