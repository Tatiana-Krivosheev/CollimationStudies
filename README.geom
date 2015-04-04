How to debug geometry

1 - use geantino in your PrimaryGeneratorAction/Source
2 - set /tracking/verbose 1

 For instance, you can run collimator example interactively with:
 
 /gun/particle geantino
 /tracking/verbose 1
 /run/beamOn 1
 
