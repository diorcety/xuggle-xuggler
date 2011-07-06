/*
 * Copyright (C) 2006 Smartjog S.A.S, Baptiste Coudurier <baptiste.coudurier@gmail.com>
 * Copyright (C) 2011 Smartjog S.A.S, Clément Bœsch      <clement.boesch@smartjog.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * version 2 of the License.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/**
 * @file
 * Timecode helpers
 */

#include <stdio.h>
#include "timecode.h"
#include "libavutil/log.h"

int ff_init_smtpe_timecode(void *avcl, struct ff_timecode *tc)
{
    int hh, mm, ss, ff, fps;
    char c;

    if (sscanf(tc->str, "%d:%d:%d%c%d", &hh, &mm, &ss, &c, &ff) != 5) {
        av_log(avcl, AV_LOG_ERROR, "unable to parse timecode, "
                                   "syntax: hh:mm:ss[:;.]ff\n");
        return -1;
    }

    fps       = (tc->rate.num + tc->rate.den/2) / tc->rate.den;
    tc->start = (hh*3600 + mm*60 + ss) * fps + ff;
    tc->drop  = c != ':'; // drop if ';', '.', ...

    if (tc->drop) { /* adjust frame number */
        int tmins = 60*hh + mm;
        if (tc->rate.den != 1001 || fps != 30) {
            av_log(avcl, AV_LOG_ERROR, "error: drop frame is only allowed with"
                                       "30000/1001 FPS");
            return -2;
        }
        tc->start -= 2 * (tmins - tmins/10);
    }
    return 0;
}