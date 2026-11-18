/* SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup gpu
 *
 * Cheap one-pass FXAA resolve for Android. Workbench keeps AA off
 * (`U.viewport_aa = 0`); this shader is only enabled by the OBL FXAA probe.
 * It runs on the resolved Workbench color, before overlays and the OCIO blit,
 * so UI/gizmos remain crisp and color management is untouched.
 */

#pragma BLENDER_REQUIRE(common_fxaa_lib.glsl)

void main()
{
  vec2 texel_size = 1.0 / vec2(textureSize(colorBuffer, 0));
  vec2 uv = gl_FragCoord.xy * texel_size;

  fragColor = FxaaPixelShader(uv, colorBuffer, texel_size, 0.75, 0.166, 0.0833);
}
