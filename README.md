Pac-man

Known issues:

(1) Ghosts can exit the stage by travelling 
through one of the horizontal tunnels in the middle of the map which breaks their AI and
prevents  them from coming back in the stage

INFO: Classic behavior in old school version is to teleport the entity passing 
the tunnels from  one side of the map to the other. 

HELP: I've marked the tiles for teleportation with "z" in solution direction/data/map.txt 
which is used to generate the tilemap used by the ghost AI behavior and player avatar movement
controls.

AI has limited behavior when ghosts are constrained to their traditional spawn tile locations.
Some edge case is required to assist them with navigating out of restrictive spaces so they can
can spawn in their old school locations and be able to navigate out on startup and map reset.
