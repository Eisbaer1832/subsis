# subsis 🚅



## Variables
- train1_current    = how far did the train already traveled
- train1_length     = how far does the train have to drive
- train1_left       = how much distance does the train have left, based on its current position
- train1_speed      = maximum speed
- train1_aproximet  = how long it will take to arrive at the destination

### int for grid type 🐸
- 0 = nothing
- 1 = rail_horizontal
- 2 = rail_vertical
- 3 = curve_down_right
- 4 = curve_down_left
- 5 = curve_up_right
- 6 = curve_up_left
- 7 = crossing
- 8 = station_horizontal
- 9 = station_vertical
- 10 = signal_error
- 11 = rail_transparent
- 12 = train_station_end
- 13 = weiche_horz_rechts_oben
- 14 = weiche_horz_rechts_unten
- 15 = weiche_horz_links_oben
- 16 = weiche_horz_links_unten
- 17 = weiche_vert_rechts_oben
- 18 = weiche_vert_rechts_unten
- 19 = weiche_vert_links_oben
- 20 = weiche_vert_links_unten
- 21 = signal_red
- 22 = signal_green
- 23 = end_north
- 24 = end_east
- 25 = end_south
- 26 = end_west
- 28 = tree
- 29 = flowers
- 30 = rocks
- 31 = grass
- 32 = lake
- 33 = (Bahnübergang)

### Signalstates
- 0 = no signal
- 1 = red
- 2 = green
- 3 = error