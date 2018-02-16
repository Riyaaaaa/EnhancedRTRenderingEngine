#pragma once

#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include "DXModel.h"

#define TRANSFORM_STRUCT_MEMBER(ign, name, member) (decltype(name::member), member)
#define FUSION_ADAPT_STRUCT_AUTO(name, members) \
  BOOST_FUSION_ADAPT_STRUCT(name, BOOST_PP_SEQ_FOR_EACH(TRANSFORM_STRUCT_MEMBER, name, members))

FUSION_ADAPT_STRUCT_AUTO(
    DXModel::XProperty,
    (type)
    (name)
    (size)
)

FUSION_ADAPT_STRUCT_AUTO(
    DXModel::XTemplate,
    (TemplateName)
    (UID)
    (properties)
)

FUSION_ADAPT_STRUCT_AUTO(
    DXModel::XHeader,
    (magic)
    (version)
    (format)
    (floatSize)
)

FUSION_ADAPT_STRUCT_AUTO(
    Vector2D,
    (x)
    (y)
)

FUSION_ADAPT_STRUCT_AUTO(
    Vector3D,
    (x)
    (y)
    (z)
)

FUSION_ADAPT_STRUCT_AUTO(
    Vector4D,
    (x)
    (y)
    (z)
    (w)
)

FUSION_ADAPT_STRUCT_AUTO(
    DXModel::Material,
    (faceColor)
    (power)
    (specularColor)
    (emissiveColor)
    (textureFileName)
)
