# GD_LEAP
This is extension enables Leap Motion tracking inside Godot 4.3+ by using the LeapC library with GDExtension.

Much of this work is taken directly from Rodolphe's repo (with permission), which can be found here: https://forge.lunai.re/rodolphe/godot-ultraleap-plugin

I strongly encourage you to check out Rodolphe's work if you are interested in learning how this works more specifically.

# NOTE
- idk what i'm doing really but i'm trying my best. if things are sloppy, forgive me.
- this was made and tested on Windows. it can be made to work for Mac, but currently it is not setup for it.
- there is no android building compatibility, no image support, and only is setup for a desktop tracking position. if you are interested in trying to use this with a HMD, I encourage you to check out Rodolphe's repo. I removed some of this functionality for the sake of simplicity.
- I am still not very satisfied with the skeleton posing function in the demo, but that's no reason to hold up releasing the extension :D
  
# BUILD INSTRUCTIONS
- download and install Gemini hand tracking software from Ultraleap: https://leap2.ultraleap.com/gemini-downloads/
- clone this repo
- open repo in vscode
- build with scons (scons will add files to the src path from the SDK installation location. if the SDK is not installed in the default location you will need to modify the Sconstruct file to point to the correct location)

# RELEASE INSTALL INSTRUCTIONS
- N/A (for now)

# CREDITS
This software is released under the MIT License. See LICENSE file for information on permissions.

This software contains source code provided by Ultraleap. See LICENSE.ULTRALEAP.txt file for information on permissions.

This software would not have been possible without the foundation work done by Rodolphe, and all the help he offered on Discord. Once again, his repo can be found here: https://forge.lunai.re/rodolphe/godot-ultraleap-plugin

Additionally, work done by Bastiaan on the previous leap motion addon helped inspire and inform my goals. You can find this repo here: https://github.com/BastiaanOlij/gdleapmotion-asset

Created By: Ryan Powell, 2024.
