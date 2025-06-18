// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_V8_PLATFORM_H_
#define V8_V8_PLATFORM_H_

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>  // For abort.

#include "v8config.h"  // NOLINT(build/include_directory)

#ifdef __wasi__
// For WASI, we use our custom implementations
#include "wasi/v8-platform-wasi.h"
#else
#include <memory>
#include <string>
#endif

namespace v8 {

// Simplified version of v8-platform.h for WASI

class Isolate;

// Valid priorities supported by the task scheduling infrastructure.
enum class TaskPriority : uint8_t {
  kBestEffort,
  kUserVisible,
  kUserBlocking,
  kMaxPriority = kUserBlocking
};

/**
 * A Task represents a unit of work.
 */
class Task {
 public:
  virtual ~Task() = default;
  virtual void Run() = 0;
};

/**
 * An IdleTask represents a unit of work to be performed in idle time.
 */
class IdleTask {
 public:
  virtual ~IdleTask() = default;
  virtual void Run(double deadline_in_seconds) = 0;
};

/**
 * A simple source location class for v8.
 */
class SourceLocation {
 public:
  static SourceLocation Current() { return SourceLocation(); }
  constexpr SourceLocation() = default;
  constexpr const char* Function() const { return nullptr; }
  constexpr const char* FileName() const { return nullptr; }
  constexpr size_t Line() const { return 0; }
#ifdef __wasi__
  // Simplified for WASI
  const char* ToString() const { return ""; }
#else
  std::string ToString() const { return {}; }
#endif
};

// Minimal TaskRunner implementation for WASI
class TaskRunner {
 public:
  TaskRunner() = default;
  virtual ~TaskRunner() = default;
  
  TaskRunner(const TaskRunner&) = delete;
  TaskRunner& operator=(const TaskRunner&) = delete;
  
#ifdef __wasi__
  // WASI version
  void PostTask(v8::wasi::unique_ptr<Task> task, const SourceLocation& location = SourceLocation::Current()) {}
  void PostNonNestableTask(v8::wasi::unique_ptr<Task> task, const SourceLocation& location = SourceLocation::Current()) {}
  void PostDelayedTask(v8::wasi::unique_ptr<Task> task, double delay_in_seconds, const SourceLocation& location = SourceLocation::Current()) {}
  void PostNonNestableDelayedTask(v8::wasi::unique_ptr<Task> task, double delay_in_seconds, const SourceLocation& location = SourceLocation::Current()) {}
  void PostIdleTask(v8::wasi::unique_ptr<IdleTask> task, const SourceLocation& location = SourceLocation::Current()) {}
#else
  // Standard version
  void PostTask(std::unique_ptr<Task> task, const SourceLocation& location = SourceLocation::Current()) {}
  void PostNonNestableTask(std::unique_ptr<Task> task, const SourceLocation& location = SourceLocation::Current()) {}
  void PostDelayedTask(std::unique_ptr<Task> task, double delay_in_seconds, const SourceLocation& location = SourceLocation::Current()) {}
  void PostNonNestableDelayedTask(std::unique_ptr<Task> task, double delay_in_seconds, const SourceLocation& location = SourceLocation::Current()) {}
  void PostIdleTask(std::unique_ptr<IdleTask> task, const SourceLocation& location = SourceLocation::Current()) {}
#endif

  virtual bool IdleTasksEnabled() { return false; }
  virtual bool NonNestableTasksEnabled() const { return false; }
  virtual bool NonNestableDelayedTasksEnabled() const { return false; }
};

/**
 * The interface represents complex arguments to trace events.
 */
class ConvertableToTraceFormat {
 public:
  virtual ~ConvertableToTraceFormat() = default;

#ifndef __wasi__
  /**
   * Append the class info to the provided |out| string. The appended
   * data must be a valid JSON object. Strings must be properly quoted, and
   * escaped. There is no processing applied to the content after it is
   * appended.
   */
  virtual void AppendAsTraceFormat(std::string* out) const = 0;
#else
  // WASI version with simplified interface
  virtual void AppendAsTraceFormat(char* out, size_t size) const {}
#endif
};

/**
 * V8 Tracing controller.
 *
 * Can be implemented by an embedder to record trace events from V8.
 */
class TracingController {
 public:
  virtual ~TracingController() = default;

  // Observer for trace state changes
  class TraceStateObserver {
   public:
    virtual ~TraceStateObserver() = default;
    virtual void OnTraceEnabled() {}
    virtual void OnTraceDisabled() {}
  };

  /**
   * Called by TRACE_EVENT* macros, don't call this directly.
   * The name parameter is a category group for example:
   * TRACE_EVENT0("v8,parse", "V8.Parse")
   * The pointer returned points to a value with zero or more of the bits
   * defined in CategoryGroupEnabledFlags.
   **/
  virtual const uint8_t* GetCategoryGroupEnabled(const char* name) {
    static uint8_t no = 0;
    return &no;
  }

#ifdef __wasi__
  /**
   * Simplified trace event API for WASI
   */
  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      ConvertableToTraceFormat** arg_convertables,
      unsigned int flags) {
    return 0;
  }
  virtual uint64_t AddTraceEventWithTimestamp(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      ConvertableToTraceFormat** arg_convertables,
      unsigned int flags, int64_t timestamp) {
    return 0;
  }
#else
  /**
   * Standard trace event API
   */
  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags) {
    return 0;
  }
  virtual uint64_t AddTraceEventWithTimestamp(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags, int64_t timestamp) {
    return 0;
  }
#endif

  /**
   * Sets the duration field of a COMPLETE trace event.
   */
  virtual void UpdateTraceEventDuration(const uint8_t* category_enabled_flag,
                                        const char* name, uint64_t handle) {}

  virtual void AddTraceStateObserver(TraceStateObserver* observer) {}
  virtual void RemoveTraceStateObserver(TraceStateObserver* observer) {}
};

}  // namespace v8

#endif  // V8_V8_PLATFORM_H_
