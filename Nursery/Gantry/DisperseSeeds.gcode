G28 X Y             ; Home only the X and Y axes
G90                 ; Set absolute positioning
M83                 ; Set extruder to absolute positioning

G4 S3               ; Dwell for 3 seconds. 'G4 P500' will dwell for 500ms

G0 X10 Y10 F3000    ; Go to x=10, Y=10, set feedrate to 3000mm/s 
G1 E1               ; Extrude 1mm.
G4 S2               ; Dwell for 2 seconds
G0 X40.48 Y10       ; Move 1.2in (30.48mm) in the x direction.
G1 E1               ; Extrude 1mm.