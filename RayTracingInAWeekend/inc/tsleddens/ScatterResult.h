#pragma once
#include "Ray.h"

namespace tsleddens
{
    struct PDF;

    struct ScatterResult
    {
        Color Attenuation;
        std::shared_ptr<PDF> pPdf;
        bool SkipPdf;
        Ray SkipPdfRay;
    };
}
