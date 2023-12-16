## Call AI (or anything external) In Gimp using bash plugin
This plugin is literally just a bash terminal and the ability to run the bash command with a checkbox going on and off. If you have AI's installed (like REMBG, GFPGAN, ect... that means you can run the
through Gimp in this GEGL plugin then apply more GEGL syntax after ward. 

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/cf82c7b2-4ff1-42ea-bff5-6ba1b248ce21)



## Quick guide to plugin:

0. You need the AI/whatever app installed on your machine and check to make sure said AI/whatever app works through bash.

1. No matter what the bash string is GEGL is only programmed to export the current  layer as "/tmp/in.png"
and call the final result "/tmp/out.png"

2. Box 1 is for bash inputs such as 

3. The checkbox is to run the bash command; after it is ran it should be disabled quickly. Gimp will freeze and unfreeze if done correctly.

4. Box 2 is for optional GEGL syntax after a bash command

5. The previous /tmp/out.png image will load (if it exist there) and will only go away if the checkbox is checked/unchecked (to update it) or if the file is manually deleted.


## Location to put binaries (they don't go in the normal plugins folder)

** Linux** 

 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
THIS PLUGIN DOES NOT WORK ON WINDOWS OR FLATPAK LINUX AS THAT CAN'T CALL BASH

Then restart Gimp and go to GEGL operation and look for "Bash Plugin"


## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).

BEAVER RECOMMENDS YOU USE A MODERN VERSION OF GEGL. NO GUARANTEE DATED VERSIONS OF GIMP WILL WORK WITH THIS PLUGIN 

### Windows

This plugin cannot work on Windows because it calls native Linux bash. Windows Sub System for Linux version may come soon but as of now it is not avalible. 



