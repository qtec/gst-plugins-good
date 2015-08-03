/* GStreamer
 * Copyright (C) <2015> Dimitrios Katsaros <patcherwork@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * SECTION:gstaudiometa
 * @short_description: Buffer metadata for audio downmix matrix handling
 *
 * #GstAudioDownmixMeta defines an audio downmix matrix to be send along with
 * audio buffers. These functions in this module help to create and attach the
 * meta as well as extracting it.
 */

#include <string.h>
#include "gstv4l2timecodemeta.h"

GType
gst_v4l2_timecode_meta_api_get_type (void)
{
  static volatile GType type;
  static const gchar *tags[] = { NULL };

  if (g_once_init_enter (&type)) {
    GType _type = gst_meta_api_type_register ("GstV4l2TimecodeMetaAPI", tags);
    g_once_init_leave (&type, _type);
  }
  return type;
}

static gboolean
gst_v4l2_timecode_meta_transform (GstBuffer * transbuf, GstMeta * meta,
    GstBuffer * buffer, GQuark type, gpointer data)
{
  GstV4l2TimecodeMeta *m = (GstV4l2TimecodeMeta *) meta;

  gst_buffer_add_gst_v4l2_timecode_meta (transbuf, &(m->timecode));

  return TRUE;
}

const GstMetaInfo *
gst_v4l2_timecode_meta_get_info (void)
{
  static const GstMetaInfo *meta_info = NULL;

  if (g_once_init_enter (&meta_info)) {
    const GstMetaInfo *mi = gst_meta_register (GST_V4L2_TIMECODE_META_API_TYPE,
        GST_V4L2_TIMECODE_META_IMPL_NAME,
        sizeof (GstV4l2TimecodeMeta),
        NULL,
        NULL,
        gst_v4l2_timecode_meta_transform);
    g_once_init_leave (&meta_info, mi);
  }
  return meta_info;
}

GstV4l2TimecodeMeta *
gst_buffer_add_gst_v4l2_timecode_meta (GstBuffer * buffer,
    struct v4l2_timecode * timecode)
{
  GstV4l2TimecodeMeta *meta;

  g_return_val_if_fail (GST_IS_BUFFER (buffer), NULL);

  meta =
      (GstV4l2TimecodeMeta *) gst_buffer_add_meta (buffer,
      GST_V4L2_TIMECODE_META_INFO, NULL);

  memcpy (&meta->timecode, timecode, sizeof (GstV4l2TimecodeMeta));
  return meta;
}
