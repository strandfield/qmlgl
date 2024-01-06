// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#if (defined(WIN32) || defined(_WIN32)) && !defined(LIBQMLGL_STATIC_LINKING)

#if defined(LIBQMLGL_BUILD_LIBRARY)
#define QMLGL_API __declspec(dllexport)
#else
#define QMLGL_API __declspec(dllimport)
#endif

#else

#define QMLGL_API

#endif
