// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "configurationvariable.h"

namespace glsl
{

QByteArray replace_variables(QByteArray src, const ConfigurationVariables& vars)
{
  for (const auto& p : vars)
    src.replace("${" + p.first + "}", p.second);

  return src;
}

} // namespace glsl

