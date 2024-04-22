# GD_LEAP
This is extension enables Leap Motion tracking inside Godot 4.3+ by using the LeapC library with GDExtension.

Much of this work is taken directly from Rodolphe's repo (with permission), which can be found here: https://forge.lunai.re/rodolphe/godot-ultraleap-plugin

I strongly encourage you to check out Rodolphe's work if you are interested in learning more about how this works.


There are 2 main demo scenes: pose_mixamo, and pose_vrm. pose_mixamo uses the mannequin mixamo model, and demos the beginning of the XRBodyTracker implementation. pose_vrm is the same idea as pose_mixamo, just with no UpperChest bone (different bone layout).

you can switch between the different cameras in the scene using the 1, 2, 3, 4, 5, and 6 keys.

right now, the demos aren't utilizing the XRBodyTracker system. this means that different skeletons need different setups bc of the different bone indices. i plan to integrate with the XRBodyTracker system soon to eliminate the need for different setups depending on skeleton type.

the skeletons used in both demos are retargeted to the Godot humanoid bone profile. this is important bc the rest poses of the bones must be taken into account when posing, and godot humanoid profile provides a consistent convention in this way.


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

# USE INSTRUCTIONS
- add UltraleapDevice node to scene
- add UltraleapHandTracking node to scene
- assign UltraleapDevice as the tracker for the UltraleapHandTracking node in the inspector
- attach a script to UltraleapDeviceNode to access the tracking data

data is passed from the extension to the node in frames. a frame can be accessed with get_interpolated_frame(frame_time), for example. a frame has members for right_hand and left_hand. each hand contains data for the arm, palm/wrist, and fingers.

the data exposed to the nodes closesly follows the way it is presented by LeapC. information on the API can be found here: https://docs.ultraleap.com/api-reference/tracking-api/leapc-guide.html

a full write up of the functions is coming soon. ctrl-clicking the class names in the demo scripts will show the functions available to call, though there aren't descriptions yet.

# CREDITS
This software is released under the MIT License. See LICENSE file for information on permissions.

This software contains source code provided by Ultraleap. See LICENSE.ULTRALEAP.md and THIRD-PARTY.LICENSE.ULTRALEAP.md files for information on permissions.

This software contains assets from Mixamo. See https://community.adobe.com/t5/mixamo-discussions/mixamo-faq-licensing-royalties-ownership-eula-and-tos/td-p/13234775 for information on permissions.
Also see https://www.adobe.com/legal/terms.html for general terms of use of adobe products.

This software would not have been possible without the foundation work done by Rodolphe, and all the help he offered on Discord. Once again, his repo can be found here: https://forge.lunai.re/rodolphe/godot-ultraleap-plugin

Additionally, work done by Bastiaan on the previous leap motion addon helped inspire and inform my goals. You can find this repo here: https://github.com/BastiaanOlij/gdleapmotion-asset

Created By: Ryan Powell, 2024.
