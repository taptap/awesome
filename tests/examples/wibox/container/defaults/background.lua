--DOC_HIDE_ALL
local wibox     = require("wibox")
local gears     = {shape = require("gears.shape")}
local beautiful = require("beautiful")

return {
    text   = "Before",
    align  = "center",
    valign = "center",
    widget = wibox.widget.textbox,
},
{
    {
        text   = "After",
        align  = "center",
        valign = "center",
        widget = wibox.widget.textbox,
    },
    shape              = gears.shape.circle,
    shape_border_width = 5,
    shape_border_color = "#ff0000",
    bg                 = beautiful.bg_highlight,
    widget             = wibox.container.background
}
