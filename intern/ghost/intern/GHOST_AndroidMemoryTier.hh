/* SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup GHOST
 *
 * Android-specific runtime configuration probes, read once per process from
 * system properties. This makes behaviour A/B testable on the device without
 * a rebuild: set a property, restart the app.
 */

#pragma once

#ifdef __ANDROID__
#  include <sys/system_properties.h>
#endif

/** Whether a one-finger drag reports as a pan instead of a left button drag.
 *
 * Off by default (upstream behaviour): the drag is a left button drag and the
 * keymap decides what a drag means. On, a drag past the slop reports as
 * GHOST_kTrackpadEventScroll, which the window manager turns into MOUSEPAN --
 * and the keymaps already disagree about what that means by region, so one
 * gesture covers everything without anything having to know which region it
 * landed in.
 *
 * `setprop debug.blender.gestures 1` turns it on; the value is read once per
 * process, so restart the app after changing it.
 */
inline bool GHOST_android_one_finger_pan()
{
#ifdef __ANDROID__
  static const bool one_finger_pan = []() {
    char value[PROP_VALUE_MAX] = {};
    if (__system_property_get("debug.blender.gestures", value) > 0 && value[0] != '\0') {
      return atoi(value) != 0;
    }
    return false;
  }();
  return one_finger_pan;
#else
  return false;
#endif
}
