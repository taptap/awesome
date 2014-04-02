/*
 * protocol_screen.c - per-protocol-screen information
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

#include "protocol_screen.h"
#include "globalconf.h"

#include <xcb/xcb_aux.h>

void
protocol_screens_init(bool no_argb)
{
    int screen, screen_count = xcb_get_setup(globalconf.connection)->roots_len;
    xcb_window_t tmp_win = xcb_generate_id(globalconf.connection);

    for(screen = 0; screen < screen_count; screen++)
    {
        protocol_screen_t scr;
        p_clear(&scr, 1);

        scr.screen_number = screen;

        /* Setup all the visual-related things */
        scr.screen = xcb_aux_get_screen(globalconf.connection, screen);
        scr.default_visual = draw_default_visual(scr.screen);
        if(!no_argb)
            scr.visual = draw_argb_visual(scr.screen);
        if(!scr.visual)
            scr.visual = scr.default_visual;
        scr.default_depth = draw_visual_depth(scr.screen, scr.visual->visual_id);
        scr.default_cmap = scr.screen->default_colormap;
        if(scr.default_depth != scr.screen->root_depth)
        {
            // We need our own color map if we aren't using the default depth
            scr.default_cmap = xcb_generate_id(globalconf.connection);
            xcb_create_colormap(globalconf.connection, XCB_COLORMAP_ALLOC_NONE,
                    scr.default_cmap, scr.screen->root,
                    scr.visual->visual_id);
        }

        /* The default GC is just a newly created associated with a window with
         * depth globalconf.default_depth */
        scr.gc = xcb_generate_id(globalconf.connection);
        xcb_create_window(globalconf.connection, scr.default_depth,
                          tmp_win, scr.screen->root,
                          -1, -1, 1, 1, 0,
                          XCB_COPY_FROM_PARENT, scr.visual->visual_id,
                          XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_COLORMAP,
                          (const uint32_t [])
                          {
                              scr.screen->black_pixel,
                              scr.screen->black_pixel,
                              scr.default_cmap
                          });
        xcb_create_gc(globalconf.connection, scr.gc, tmp_win, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND,
                      (const uint32_t[]) { scr.screen->black_pixel, scr.screen->white_pixel });
        xcb_destroy_window(globalconf.connection, tmp_win);

        /* Initialize xcb-cursor */
        if (xcb_cursor_context_new(globalconf.connection, scr.screen, &scr.cursor_ctx) < 0)
            fatal("Failed to initialize xcb-cursor");

        protocol_screen_array_push(&globalconf.protocol_screens, scr);
    }

    assert(globalconf.default_screen < globalconf.protocol_screens.len);
    globalconf.protocol_screen = &globalconf.protocol_screens.tab[globalconf.default_screen];
}

protocol_screen_t *
protocol_screen_getbyroot(xcb_window_t root)
{
    foreach(screen, globalconf.protocol_screens)
        if (screen->screen->root == root)
            return screen;
    return NULL;
}

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:textwidth=80
