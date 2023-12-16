A special build of my GEGL Layer Effects engine that uses this plugin internally on its 9th tab "Run a bash command on Linux" can be downloaded here and the same rules apply. This plugin works as both a stand alone and a tool inside a special build of GEGL Effects.

https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/blob/main/AI_BASH_GEGL_EFFECTS_Linux_only_binaries_code.zip


## Call AI (or anything external) In Gimp using bash plugin
This plugin is literally just a bash terminal and the ability to run the bash command with a checkbox going on and off. If you have AI's installed (like REMBG, GFPGAN, ect... that means you can run the
through Gimp in this GEGL plugin then apply more GEGL syntax after ward. 

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/cf82c7b2-4ff1-42ea-bff5-6ba1b248ce21)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/4d585743-106f-423a-b611-23a7c2b0784c)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/a370214f-d9d4-4db5-b622-dd93fc48c10d)


I shouldn't have to say this but it looks like Windows XP because of a Linux theme. Moving on...


## Quick guide to plugin:

0. You need the AI/whatever app installed on your machine and check to make sure said AI/whatever app works through bash.

1. The first command is always the bash string followed by optional GEGL syntax. The only way GEGL commands can come first is if users call the systems gegl outside of Gimp and that technically still is bash.

2. No matter what the bash string is GEGL is only programmed to export the current  layer as "/tmp/in.png"
and call the final result "/tmp/out.png" so use those two lines in your bash string for the input and final output.

3. Box 1 is for bash inputs such as **rembg /tmp/in.png -o /tmp/out.png**

4. The checkbox is to run the bash command; after it is ran it should be disabled quickly. Gimp will freeze and unfreeze if done correctly. If it isn't unchecked quickly it will take dozens of times longer to load.

5. Box 2 is for optional GEGL syntax after a bash command. You don't have to put anything here if you don't write GEGL syntax.

6. The previous /tmp/out.png image will load (if it exist there) and will only go away if the checkbox is checked/unchecked (to update it) or if the file is manually deleted. Just get use to the previous image loading on start up and know it will go away.


## Location to put binaries (they don't go in the normal plugins folder)

**Linux** 

 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
THIS PLUGIN DOES NOT WORK ON WINDOWS OR FLATPAK LINUX AS THAT CAN'T CALL BASH

Then restart Gimp and go to GEGL operation drop down list and look for "Bash Plugin" or "GEGL Effects Continual" and go to the 9th tab named "Run a bash command on Linux"


## Compiling and Installing

Click the build all button to compile everything but you can also read here how to compile individual files.

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

## Previews of Special GEGL Effects integration via

https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/blob/main/AI_BASH_GEGL_EFFECTS_Linux_only_binaries_code.zip

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/ba62c04d-4b2c-4066-9400-af8acadd82ca)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/0659875c-25c7-4c18-86b4-59632ee7d977)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/e2f59074-2926-42c2-8fde-a022cc1ccdc0)



## More previews of this based Plugin (in stand alone mode)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/caa43b3a-7c82-443c-b62f-5279e621c625)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/b35a8ebc-0b7f-44ff-bc31-9b4047b0e889)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/fa7cf60d-e8e1-4e48-bf75-eeaeaee67643)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/561db502-6a7e-43fa-9545-d794b904360d)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/0335019d-02f9-4175-a917-2ff95a0398a7)

![image](https://github.com/LinuxBeaver/AI_in_Gimp_by_GEGL_calling_bash/assets/78667207/da2b84a9-5ecf-4800-9f54-ad32f7b172ed)






