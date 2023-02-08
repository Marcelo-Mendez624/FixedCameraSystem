# Fixed camera system

## This project is a recreation of games like Until down or old Resident evils games

### How to use the system
 
 First you need to go to the Blueprints/Core/Camera folder and searh for a CameraVolume or SplineCamera and a add a BaseCamera in an instance in the game.
 Set Camera Target Actor in CameraVolume when the player overlap the CameraVolume the camera actor is call by an interface and is set the tick enable to true.
 If you use a SplineCamera you also need to set the PathDistance (The distance the character has to travel on his path for tracking to work correctly).
 You also can set some variables in the BaseCamera actor to improve the player experience.
 
 -Oriented to character
  -With this bool you can tell the actor to look at the player all the time
 - Invert Forward Vector 
  - With this bool you invert the forwad vector 
 - Invert Right Vector 
  - With this bool you invert the right vector
 - Convert Right VEctor to Forward Vector

The Character is set to interact with object with the new system of unreal input "Enhanced Input" create a box using the function SearchObjectToInteract. The character also have the capability to sprint you can change the walk speed changing  BaseWalkSpeed and the sprint speed changing MaxWalkSpeed
 

Developed with Unreal Engine 5 by Marcelo Mendez
