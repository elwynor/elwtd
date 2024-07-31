<pre>
    ________    _       __   ____  _           __
   /_  __/ /_  (_)___  / /__/ __ \(_)_______  / /
    / / / __ \/ / __ \/ //_/ / / / / ___/ _ \/ / 
   / / / / / / / / / / ,< / /_/ / / /__/  __/_/  
  /_/ /_/ /_/_/_/ /_/_/|_/_____/_/\___/\___(_)   
                                            
                  version 2.0
   Copyright (c) 2004-2024 Elwynor Technologies
</pre>       

## WHAT IS THINKDICE?

 ThinkDice is a simple but fun guessing game where users guess the result of
 a die roll, earning points for correct guesses and losing points for 
 incorrect guesses.  Users compete to get on the top ten list.
 
## GAMEPLAY
 
 ThinkDice is a simple game.  While playing, simply guess the result of a 
 single die's roll (guess 1 through 6).  You guess five times - each time,
 you score ten times the die roll for a correct answer, or lose twice the
 die roll for an incorrect answer.  
 
 Select "T" from the main menu for the top ten list.
 
## INSTALLATION AND CONFIGURATION
 
 Simply unzip the ThinkDice! game archive to your BBS server directory,
 add ThinkDice! to your menu, configure the MSG file to your liking, and start
 the BBS!  It's that easy! 
 
## GAME HISTORY
 
 ThinkDice! was originally written and developed by Rick Hadsall in 1989, and
 based upon his earlier DOS game of the same name. 

 The game was released as shareware for The Major BBS v5.x. It was shelved 
 until being revived along with many other modules in 2004. Then, it was ported 
 to The Major BBS 6.25/Worldgroup 1/Worldgroup 2 DOS format. In 2006, it was 
 ported to Worldgroup 3.2.  In 2023, it was ported to Major BBS V10. Finally,
 in 2024, its old text file top ten was converted to BTrieve. 
 
## LICENSE

 This project is licensed under the AGPL v3. Additional terms apply to 
 contributions and derivative projects. Please see the LICENSE file for 
 more details.

## CONTRIBUTING

 We welcome contributions from the community. By contributing, you agree to the
 terms outlined in the CONTRIBUTING file.

## CREATING A FORK

 If you create an entirely new project based on this work, it must be licensed 
 under the AGPL v3, assign all right, title, and interest, including all 
 copyrights, in and to your fork to Rick Hadsall and Elwynor Technologies, and 
 you must include the additional terms from the LICENSE file in your project's 
 LICENSE file.

## COMPILATION

 This is a Worldgroup 3.2 / Major BBS v10 module. It's compiled using Borland
 C/C++ 5.0 for Worldgroup 3.2. If you have a working Worldgroup 3.2 development
 kit, a simple "make -f ELWTD" should do it! For Major BBS v10, import this
 project folder in the isv/ subtree of Visual Studio 2022, right click the
 project name and choose build! When ready to build for "release", ensure you
 are building for release.
 
 Note: this module will also compile for Worldgroup 2 for DOS using the mkv2p 
 builder. I haven't tested this version on WG2 however - the Btrieve files 
 should work but may be slightly off. When packaging for WG2, you have to also
 include the GALV3.DLL in case the SysOp doesn't already have it.

## PACKING UP

 The DIST folder includes all of the items that should be packaged up in a 
 ELWTD.ZIP. When unzipped in a Worldgroup 3.2 or Major BBS V10 installation 
 folder, it "installs" the module.
