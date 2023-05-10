# QMK Libraries

While working on customizing my keymap I have isolated some parts of code so they could be reused by others. Check out the table below for a list of features you can start using yourself:

Feature|Description
--:|:--
[RGB Layers](/qmk-lib/rgb-layers/)|Allows you to easily define color codes per key per layer, Ergodox EZ configurator style!
[OSM](/qmk-lib/osm/)|My own implementation of one shot modifiers, swapping out the timeout timer with a reset button and using real modifiers that the operating system picks up on as soon as the modifier key is pressed.
[Zones](/qmk-lib/zones/)|Splitting up a layer into multiple others and adding some custom code on top can allow you to repeat keypresses on one layer, while still having access to the base layer when you need it.<br><br>Unfortunately this feature only has docs with code examples and doesn't come with a simplified library that you can download. However, I believe this mechanic is worth sharing, so have a look and see if it's something that you would like to implement as well. 
[Utility Functions](/qmk-lib/utility-functions/)|A few miscellaneous functions, currently only consisting of improved (base) layer detection functions.