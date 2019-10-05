#!/usr/bin/env bash
# while IFS="" read -r p || [ -n "$p" ]
# do
	# echo -e "$p" > "../../x64/Release/Moment.exe"
	#"../../x64/Release/Moment.exe" <<<  "$p" 
# done <  "../../mushroom.dat"
"../../x64/Release/Moment.exe" < "../../mushroom.dat"