/* SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup blenlib
 *
 * Android runtime configuration probes, read once per process from system
 * properties. This makes behaviour A/B testable on the device without a
 * rebuild: set a property, restart the app.
 */

#pragma once

#include <chrono>
#include <stdlib.h>

#ifdef __ANDROID__
#  include <sys/system_properties.h>
#endif

namespace blender::perf {

/** Read a boolean Android system property. Off when the property is absent. */
inline bool debug_prop_on(const char *name)
{
#ifdef __ANDROID__
  char value[PROP_VALUE_MAX] = {};
  if (__system_property_get(name, value) > 0 && value[0] != '\0') {
    return atoi(value) != 0;
  }
#else
  (void)name;
#endif
  return false;
}

/** Viewport color target: RGBA8 when `debug.blender.viewport_color` is set,
 * RGBA16F by default. */
inline bool viewport_color_rgba8()
{
  return debug_prop_on("debug.blender.viewport_color");
}

/** Viewport depth target: D16 when `debug.blender.viewport_depth` is 1,
 * D32S8 by default. Returns 0 (default) or 1. */
inline int viewport_depth_choice()
{
#ifdef __ANDROID__
  char value[PROP_VALUE_MAX] = {};
  if (__system_property_get("debug.blender.viewport_depth", value) > 0 && value[0] != '\0') {
    return atoi(value);
  }
#endif
  return 0;
}

/** Whether the legacy C on-screen keyboard is enabled. On by default off
 * Android, off when `debug.blender.vk_legacy` is not set. */
inline bool vk_legacy_enabled()
{
#ifdef __ANDROID__
  return debug_prop_on("debug.blender.vk_legacy");
#else
  return true;
#endif
}

using TimePoint = std::chrono::steady_clock::time_point;

/** Rolling-average accumulator; one static instance per `PERF_ZONE` call site. */
class PerfProbe {
public:
  PerfProbe() = default;

  void observe(double seconds)
  {
    m_sum += seconds;
    m_count++;
  }

  double average() const
  {
    return m_count > 0 ? m_sum / double(m_count) : 0.0;
  }

  int count() const { return m_count; }

private:
  double m_sum = 0.0;
  int m_count = 0;
};

}  // namespace blender::perf
