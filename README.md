# Resit Assignment 

Please refer to Blackboard for full details on the assignment and its 
deliverables.

Project Layout
------
* app
  * ASGEGame: the game client and OpenGL Window
    * data: used for assets and game deployment
    * resources: used for icons 
    * src: code files
  * ASGEServer: the dedicated server i.e. no window
    * data: used for server assets
    * src: code files
    
* extern
  * important build scripts and other submodules
  * to manually update the submodules
    * `git pull --recurse-submodules`
    * `git submodule update --init --remote --recursive`
  
Static Analysis
------
If you want to resolve issues that GitHub builds have flagged, you can turn
on the live analysis. This will scan each file as it compiles it. Just don't
forget to turn it off when you're finished.
