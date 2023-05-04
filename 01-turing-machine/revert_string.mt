; Code is written for more understanding of actions than for optimal execution

; Program execution can be from the middle of the line

; init start
0 * * l 0
0 _ s r init_end

; init end
init_end * * r init_end
init_end _ e l find_char

; find char
find_char . * l find_char
find_char 0 . r move_0
find_char 1 . r move_1
find_char s _ r clear

; back to end
to_end * * l to_end
to_end e * l find_char

; move 1 to right
move_1 * * r move_1
move_1 _ 1 r to_end

; move 0 to right
move_0 * * r move_0
move_0 _ 0 * to_end

; clear
clear . _ r clear
clear e _ * halt-accept
