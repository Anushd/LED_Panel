# LED_Panel

This program generates video files to played by a custom-built display consisting of blue, green, ultraviolet pixels for 
application in insect research. It accepts an input file "rotate-horizon-5sec.values" which contains an 80 by 80 array of values
representing pixels having 3 bytes each. The letter "S" is for ultraviolet, "G" is for green, and "B" is both ultraviolet and
blue. The video which is generated has a duration of 5 seconds and involves the rotation of the input array about the center.
An .img video file is the output, which is read by the LED Panel. 
