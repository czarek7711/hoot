---------------------------------------------------------------------------------------------------
Hoot performance support "HootFavoritter" 210704
Kuroha Manufacturing

A hoot front-end that looks like it could be a replacement for C.H.E.A.P.
---------------------------------------------------------------------------------------------------

You will need an initial state, please complete the initial settings section if you are using it for the first time.


O Features

This is a tool to make a list of songs and play them by the time.
  You can also set the volume, initialization time, etc. for each song, even for titles with long preloads such as ADPCM.
  You can also fade out when switching songs.
  The main screen is a black cat SPC.

The main screen looks like a combination of the playlist of Kuroneko SPC and the key controls of hoot.
  The playlist is almost the same as the playlist of VHST.


O What you need/OS to run

Operating System
  Windows 2000/XP/Vista/7/8/8.1/10.

Sound Hardware Emulator - Sound Hardware Emulator
  The 20130602 version or later is required. It will not work with earlier versions.


O Initial Settings

  Please start hoot first. (The program will exit with an error if hoot is not running.)
  Once hoot has started, run HootFavoritterRegist and locate hotserv.dll.
  Normally, it will be in the same location as the main hoot file.

  Next, select [File]->[Import list from hoot] to import the list.
  Import the list.

  After the registration is complete, exit and start HootFavoritter.

  [File]->[Load List] will not load the list from hoot, but will load the file obtained from the hootclient list command.
  If you have already created a list (which I don't think you do), you can use that one.

  If you have already created a list, you can use that.


O Usage

  Please start hoot first.
  You can start it even if it is not running, but you cannot play it.

  The main screen is almost the same as the playlist of hoot.
  The shortcut keys that are often used are as follows.

  The first time you use it, it will take a little time to load the DB.

  The stop button can be used at any time during the performance.
  The play button is available only in the hierarchy of song title selection.

  [Double left mouse click] or [ENTER]: Go forward one level, or start playback when in the song selection level.
  [BS] or [Right mouse click] : Go back one level.
  [INS] : Add the currently selected song to the playlist (only when the song name is selected)

  [File]->[Exit].
  Exit.

  [View]->[Playlist].
  Open the playlist.

  [Add to Playlist] *Only available when in the title/song title selection hierarchy.
  Register the currently selected song or all songs of the title to the playlist.
  The standard volume when registering, weight when loading the driver (initialization), and weight when switching songs (between songs) will be used.
  The same operation can be performed with the [INS] key.

  [Search]->[Next].
  Searches the list for words entered in the text box at the top, using partial matches.
  If you search to the bottom of the list, it will search from the top of the list.

  [Search]->[Previous].
  Does the reverse of the above. When you reach the top of the list, it will search from the bottom of the list.

  [Play]->[Play] *Only available when in the song selection hierarchy.
  Play the currently selected song.
  
  [Play]->[Stop] *Only available when in the song title selection hierarchy.
  Stop the currently playing song.

  [Play]->[Next] *Only selectable when in the song name selection hierarchy.
  Play the next song currently selected. If it is the bottom of the list, it will return to the top.

  [Play]->[Previous Song] *Only available when in the song title selection hierarchy.
  Play the previous song currently selected. If it is the top of the list, it will return to the end.

  [Play]->[Random] *Only available when in the song title selection hierarchy.
  Select a song at random.

  [Settings]->[Change current volume].
  Change the volume of the currently playing song by entering the total_mix value of the hoot.
  Enter the value of total_mix in hoot as it is. 0x100 will equalize the volume, 0x200 will double it, and so on.
  The input can be either decimal or hexadecimal, and the leading 0 is not necessary for hexadecimal input. (Example: x100)

  [Settings]->[Current Volume +8]
  Sets the current volume to +8. (Max 0xFFFF).
  I have assigned a shortcut key, so it would be easier to use that.
 
  [Settings]->[Current Volume -8]
  Sets the current volume to -8. (Minimum 0)
  It is easier to use the shortcut keys that are assigned.
 
  [Settings]->[Standard Volume].
  Set the volume for playback and playlist registration. When playing from the main screen, this volume is usually used.
  The value is the same as [Change current volume] above.

  [Settings]->[Standard Driver Load Weight].
  When playing a title, the driver will be loaded before starting playback, but if the title takes a long time to load, the playback instructions may be ignored or the performance may become strange.
  However, if the title takes a long time to load, the playback instructions may be ignored or the performance may become erratic, so this weight is used as a countermeasure.
  Playback will start after waiting the specified number of seconds after loading.
  This is for titles that require a long time for preprocessing, such as those with long ADPCM loading or MIDI.
  The unit is milliseconds. 1000 is 1 second. Normally, the standard value (500) should be fine. Normally, you should leave it at this value.
  If you have any problems with playback from the playlist, you may want to modify the playlist separately.
  Taito's F2, F3, JC, etc. seem to lose their sound when they are set below 2000.

  [Settings]->[Weight when switching standard songs].
  This is the weight when only the song changes with the same driver, but this weight is also used when the song stops.
  The unit is milliseconds, and 1000 is 1 second. Usually, the standard value (50) is fine.

  [Settings]->[Standard Fade Time].
  Specify the standard fade time when registering to the playlist.
  The unit is milliseconds. 1000 is 1 second.

  [Settings]->[Standard List Playback Time]
  Specifies the standard playback time when adding to the playlist.
  The unit is seconds. Normally, you will edit the playlist individually, so this is just a guide.

  [Settings]->[Display Font].
  Specify the display font for the list. There is a separate one for the playlist.

  [Settings]->[Dark Mode].
  Apply a pseudo-dark mode (Carbon) using VCL Style.
  The result is darker than the current Windows 10 (1809), but it takes almost a second extra to boot up.
  You can change the dark mode on Windows 10 as you wish, regardless of whether the dark mode setting is enabled or disabled.
  It will be enabled after the next start-up.

  [Settings]->[Hoot integration]->[Start hoot on startup].
  [Settings]->[Hoot]->[Specify hoot].
  [Settings]->[Hoot]->[Command line option for hoot]
  [Settings]->[Hoot Cooperation]->[Terminate hoot on exit].
  This will start Hoot when the HootFavoritter starts, and will also terminate Hoot when it exits.
  Specify [specify hoot] and [hoot command line options] first, and then click [start hoot when it starts] or
  If you check the [Terminate hoot when exiting] checkbox, it will be reflected from the next startup.

  [Settings]->[Don't use task tray icon].
  The task tray icon will no longer be stored in the task tray when minimized.

  [Settings]->[Do not use task tray balloon help]
  When the playlist is played in the order of the list when stored in the task tray, the song information is displayed as a balloon when the song is switched.
  This will be disabled.

  [Help]->[About].
  Click on the URL and email address to open the version information.

  [Help]->[Readme.txt].
  This will open this text, which I assure you is not helpful at all.

  [Help]->[See History.txt].
  Opens the HISTORY.TXT file. I'm not sure if this is of any use.

O About playlists

  A playlist is a list of registered songs that you can configure and actually play.
  Most of the [Edit] menus in the list can also be found by right-clicking on the song name.

  Files that can be registered to the hoot by D&D can also be registered to the list by D&D.

  You can search for a partial match of either the title or the song title using words entered in the text box at the top.
  You can search by [Search]->[Next](F3) or [Previous](SHIFT+F3) or the button next to the text box.

  [File]->[Open List].
  Open the playlist you have saved.
  Note that the original list will be cleared.

  [File]->[Open File].
  Add files to the list that can be registered to hoot with D&D.
  Note that the file format is not checked.

  [File]->[Save As].
  Save the list.

  [Edit]->[Song Title].
  Change the song title. Only items registered as files can be changed.
  You can change the song title only for the item registered as a file, nothing else will happen.

  [Edit]->[Song Number].
  Changes the song number. Only items registered as files can be changed.
  Nothing will happen to other items.
  Both decimal and hexadecimal input will be accepted, and the leading 0 is not necessary for hexadecimal input. (Example: x100)
  If the song number is set to 0xFFFF, the playback code will not be issued when loading.
  If the song number is set to 0xFFFF, the playback code will not be issued upon loading. 0xFFFF is a good value for a song that starts playing immediately after D&D to hoot.
  If the volume is set to anything other than 0x100, the change in volume will be visible for a moment at the beginning of the song, but this is a specification.
  If you are concerned about this, please leave the volume at 0x100.

  [Edit]->[Volume].
  Change the volume of the selected song by entering the value of total_mix in hoot.
  Enter the value of total_mix in hoot as it is. 0x100 means equal, 0x200 means double, and so on.
  The input can be either decimal or hexadecimal, and the leading 0 is not necessary for hexadecimal input. (Example: x100)

  [Edit]->[Driver Load Weight].
  Specify the weight for loading the selected song. Wait this number of seconds before starting playback.
  This is for titles that take a long time to pre-process, such as those with long ADPCM loading or MIDI.
  The unit is milliseconds. 1000 is 1 second.

  [Edit]->[Weight when switching songs].
  This is the weight for changing songs with the same driver.
  The unit is milliseconds, and 1000 is 1 second.

  [Edit]->[Playback Time].
  Specifies the playback time when playing back songs in list order.
  The unit is seconds.

  [Edit]->[Fade Time].
  Specify the time to fade out.
  The unit is milliseconds. 1000 is 1 second.
  0 means no fade-out.

  [Edit]->[Move Up] / [Move Down]
  Move the selected item up or down.

  [Edit]->[Go to Top] / [Go to Bottom].
  Move the selected item to the top or bottom of the list.

  [Edit]->[Delete]
  Delete the selected item.

  [Edit]->[Clear].
  Empty the list.

  [Edit]->[Shuffle].
  Randomly sort the contents of the list. Note that it cannot be undone.

  [Edit]->[Update].
  Retrieve the song title from the DB. Nothing will happen to the items registered as files.

  [Edit]->[Select All]
  Select all.

  [Edit]->[Toggle Selection].
  Reverses the current selection.

  [Search]->[Next].
  Search the list with partial matches of words entered in the text box at the top. The target is the title and song title.
  If you search to the bottom of the list, it will search from the top of the list.

  [Search]->[Previous].
  It works in the opposite way as above. When you reach the top of the list, it will search from the bottom of the list.

  [Play]->[Play].
  Play the currently selected song.
  
  [Play]->[Stop].
  Stop the currently playing song.

  [Play]->[Next].
  Play the next currently selected song. If it is the bottom of the list, it will return to the top.

  [Play]->[Previous].
  Play the previous song currently selected. If it is at the top of the list, it will return to the end.

  [Play]->[Random].
  Select a song at random.

  [Action]->[Play in order of list]->[Enable].
  When a playlist is displayed on the screen and one of the files is selected, the next file on the list will be played as soon as the currently playing file is finished.
  If the playlist is on the screen and any one of the files is selected, when the currently playing file is finished, it will automatically move to the next file in the list. (If the list is open, it is OK to minimize it.)
  Keep in mind that it will pick the next file in the list that was selected immediately after the end of the current file. When you reach the end, it will return to the beginning.

  [Action]->[Play in list order]->[Play in same order].
  If [Play in list order]->[Enable] is checked above, the next file will be the next song selected in the list.
 
  [Behavior]->[Playback in order of list]->[Random].
  If [Play in list order]->[Enable] is checked above, the next file will be randomized.

  [Action]->[Play in list order]->[Single track repeat].
  When [Play in list order]->[Enable] is checked above, the currently selected song will be repeated.

  [Behavior]->[Do not register multiple copies of the same song].
  Duplicate songs will not be registered in the playlist.

  [Action]->[Follow the main list when playing].
  When playing from the list, the main list will be automatically selected up to the title and song name to be played.
  For db reasons, it will take some time to load the first time it is started.

  [Settings]->[Display Font].
  Specify the display font for the list.


O About the contents of the playlist

  The data is saved in UTF-16LE. The content is tab-delimited text, so it can be edited in Excel or other programs as long as the content is not corrupted.
  The items are listed in the following order.

  Data type : [1] Registration from DB [2] Registration from file)
  Driver Type
  Driver Name
  Title
  Song Title
  Song number : Hexadecimal
  Volume : Hexadecimal
  Initialization (weight when driver is loaded)
  Between songs (weight when switching songs)
  Time (Play time)
  Fade (Fade time)
  Full path of the file name when registering from a file


O About uninstallation

  We don't use the registry, so just delete the entire folder and you're done.
  If the DB goes wrong, delete HootFavoritter.hfdb and try the initial settings again. The contents of the playlist will not be lost.


O Current Specifications and Decisions

  The following are specifications related to C++Builder XE5, but I will try to fix them as much as possible when I find a workaround.

  In rare cases, both the taskbar and the task icon may appear.
  The playlist is fixed to the top of the main window.


O People, tools, etc. that have helped me

Mersenne Twister (mt19937ar.c)
  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
  All rights reserved.
  Copyright (C) 2005, Mutsuo Saito
  All rights reserved.
  http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/mt.html


O License, etc.

  We do not warrant any damages incurred as a result of the use of this software.
  Please use it at your own risk.

  You may use this software for any purpose, commercial or non-commercial.
  However, in the case of commercial use, please contact me for an after-the-fact report.


If you have any requests or complaints, please contact me here (I will try to fulfill your requests as much as possible)

------------------------------------------------------
Kuroha Manufacturing
https://kurohane.net/
kurohane@kurohane.net


Translated with www.DeepL.com/Translator (free version)


