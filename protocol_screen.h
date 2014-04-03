/*
 * protocol_screen.h - per-protocol-screen information
 *
 * Copyright © 2014 Uli Schlachter <psychon@znc.in>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef AWESOME_PROTOCOL_SCREEN_H
#define AWESOME_PROTOCOL_SCREEN_H

#include <common/array.h>

#define SN_API_NOT_YET_FROZEN
#include <libsn/sn.h>

#include <xcb/xcb_icccm.h>
#include <xcb/xcb_cursor.h>

/* Forward declarations */
typedef struct drawin_t drawin_t;
typedef struct tag tag_t;
typedef struct lua_State lua_State;

ARRAY_TYPE(tag_t *, tag)

/** Per-protocol-screen information */
struct protocol_screen_t
{
    /** X11 screen number */
    int screen_number;
    /** xcb-cursor context */
    xcb_cursor_context_t *cursor_ctx;
    /** Window that contains the systray */
    struct
    {
        xcb_window_t window;
        /** Systray window parent */
        drawin_t *parent;
    } systray;
    /** The monitor of startup notifications */
    SnMonitorContext *snmonitor;
    /** The visual, used to draw */
    xcb_visualtype_t *visual;
    /** The screen's default visual */
    xcb_visualtype_t *default_visual;
    /** The screen's information */
    xcb_screen_t *screen;
    /** A graphic context. */
    xcb_gcontext_t gc;
    /** Our default depth */
    uint8_t default_depth;
    /** Our default color map */
    xcb_colormap_t default_cmap;
    /** Tag list */
    tag_array_t tags;
};
typedef struct protocol_screen_t protocol_screen_t;

DO_ARRAY(protocol_screen_t, protocol_screen, DO_NOTHING)

void protocol_screens_init(bool);
protocol_screen_t *protocol_screen_getbyroot(xcb_window_t);

protocol_screen_t *luaA_checkprotocolscreen(lua_State *, int);
void luaA_pushprotocolscreen(lua_State *, protocol_screen_t *);

#endif
// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:textwidth=80
