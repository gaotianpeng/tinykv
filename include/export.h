// Copyright (c) 2017 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_TINYDB_INCLUDE_EXPORT_H_
#define STORAGE_TINYDB_INCLUDE_EXPORT_H_

#if !defined(TINYKV_EXPORT)

#if defined(LEVELDB_SHARED_LIBRARY)
#if defined(_WIN32)

#if defined(LEVELDB_COMPILE_LIBRARY)
#define TINYKV_EXPORT __declspec(dllexport)
#else
#define TINYKV_EXPORT __declspec(dllimport)
#endif  // defined(LEVELDB_COMPILE_LIBRARY)

#else  // defined(_WIN32)
#if defined(LEVELDB_COMPILE_LIBRARY)
#define TINYKV_EXPORT __attribute__((visibility("default")))
#else
#define TINYKV_EXPORT
#endif
#endif  // defined(_WIN32)

#else  // defined(LEVELDB_SHARED_LIBRARY)
#define TINYKV_EXPORT
#endif

#endif  // !defined(TINYKV_EXPORT)

#endif  // STORAGE_TINYDB_INCLUDE_EXPORT_H_
