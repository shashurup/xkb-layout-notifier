Initially xkb-layout-notifier is created to be used with wmii windows manager to populate status bar with current xkb layout code. wmiirc code would look like:

xkb-layout-notifier "echo -n label %s | wmiir write /rbar/layout"
