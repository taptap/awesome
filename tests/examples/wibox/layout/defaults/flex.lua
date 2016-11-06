local generic_widget = ... --DOC_HIDE
local wibox     = require("wibox") --DOC_HIDE

return --DOC_HIDE
wibox.widget {
    generic_widget( "first"  ),
    generic_widget( "second" ),
    generic_widget( "third"  ),
    layout  = wibox.layout.flex.horizontal
}
