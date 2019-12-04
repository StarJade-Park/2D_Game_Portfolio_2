#include "GVector.h"

const GVector4D GVector4D::Zero = GVector4D(0.0f, 0.0f, 0.0f, 0.0f);
const GVector4D GVector4D::One = GVector4D(1.0f, 1.0f, 1.0f, 1.0f);

const GVector4D GVector4D::Red = GVector4D(1.0f, 0.0f, 0.0f, 1.0f);
const GVector4D GVector4D::Green = GVector4D(0.0f, 1.0f, 0.0f, 1.0f);
const GVector4D GVector4D::Blue = GVector4D(0.0f, 0.0f, 1.0f, 1.0f);
const GVector4D GVector4D::White = GVector4D(1.0f, 1.0f, 1.0f, 1.0f); 
const GVector4D GVector4D::Black = GVector4D(0.0f, 0.0f, 0.0f, 1.0f);
const GVector4D GVector4D::Magenta = GVector4D(1.0f, 0.0f, 1.0f, 1.0f);

const GVector4D GVector4D::vUp = GVector4D(0.0f, 1.0f, 0.0f, 1.0f);
const GVector4D GVector4D::vLeft = GVector4D(-1.0f, 0.0f, 0.0f, 1.0f);
const GVector4D GVector4D::vRight = GVector4D(1.0f, 0.0f, 0.0f, 1.0f);
const GVector4D GVector4D::vDown = GVector4D(0.0f, -1.0f, 0.0f, 1.0f);

const GVector4D GVector4D::vBack = GVector4D(0.0f, 0.0f, 1.0f, 1.0f);