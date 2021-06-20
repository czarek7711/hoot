---------------------------------------------------------------------------------------------------
Hoot performance support "HootFavoritter" 200510
Kuroha Manufacturing

A hoot front-end that looks like it could be a replacement for C.H.E.A.P.
---------------------------------------------------------------------------------------------------

Initial state is required, please complete the initial settings section if you are using it for the first time.


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

  File]->[Load List] will not load the list from hoot, but will load the file obtained from the hootclient list command.
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
  BS] or [Right mouse click] : Go back one level.
  INS] : Add the currently selected song to the playlist (only when the song name is selected)

  File]->[Exit].
  Exit.

  [View]->[Playlist].
  Open the playlist.

  [Add to Playlist] *Only available when in the title/song title selection hierarchy.
  Register the currently selected song or all songs of the title to the playlist.
  The standard volume when registering, weight when loading the driver (initialization), and weight when switching songs (between songs) will be used.
  The same operation can be performed with the [INS] key.

  Play]->[Play] *Only available in the song name selection hierarchy.
  This will play the currently selected song.
  
  Play]->[Stop] *Only available when in the song title selection hierarchy.
  Stop the currently playing song.

  Play]->[Next] *Only selectable when in the song name selection hierarchy.
  Play the next song currently selected. If it is the bottom of the list, it will return to the top.

  Play]->[Previous Song] *Only available when in the song title selection hierarchy.
  Play the previous song currently selected. If it is the top of the list, it will return to the end.

  Play]->[Random] *Only available when in the song title selection hierarchy.
  Select a song at random.

  Settings]->[Change current volume].
  Change the volume of the currently playing song by entering the total_mix value of the hoot.
  Enter the value of total_mix in hoot as it is. 0x100 will equalize the volume, 0x200 will double it, and so on.
  The input can be either decimal or hexadecimal, and the leading 0 is not necessary for hexadecimal input. (Example: x100)

  Settings]->[Current Volume +8
  Sets the current volume to +8. (Max 0xFFFF).
  I have assigned a shortcut key, so it would be easier to use that.
 
  Settings]->[Current Volume -8
  Sets the current volume to -8. (Minimum 0)
  It is easier to use the shortcut keys that are assigned.
 
  Settings]->[Standard Volume].
  Set the volume for playback and playlist registration. When playing from the main screen, this volume is usually used.
  The value is the same as [Change current volume] above.

  Settings]->[Standard Driver Load Weight].
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

  Settings]->[Standard Fade Time].
  Specify the standard fade time when registering to the playlist.
  The unit is milliseconds. 1000 is 1 second.

  Settings]->[Standard List Playback Time
  Specifies the standard playback time when adding to the playlist.
  The unit is seconds. Normally, you will edit the playlist individually, so this is just a guide.

  Settings]->[Display Font].
  Specify the display font for the list. There is a separate one for the playlist.

  Settings]->[Dark Mode].
  Apply a pseudo-dark mode (Carbon) using VCL Style.
  The result is darker than the current Windows 10 (1809), but it takes almost a second extra to boot up.
  You can change the dark mode on Windows 10 as you wish, regardless of whether the dark mode setting is enabled or disabled.
  It will be enabled after the next start-up.

  Settings]->[Hoot integration]->[Start hoot on startup].
  Settings]->[Hoot]->[Specify hoot].
  Settings]->[Hoot]->[Command line option for hoot
  Settings]->[Hoot Cooperation]->[Quit hoot on exit].
  This will start Hoot when the HootFavoritter starts, and will also terminate Hoot when it exits.
  Specify [specify hoot] and [hoot command line options] first, and then click [start hoot when it starts] or
  If you check the [Terminate hoot when exiting] checkbox, it will be reflected from the next startup.

  Settings]->[Don't use task tray icon].
  The task tray icon will no longer be stored in the task tray when minimized.

  Settings]->[Do not use task tray balloon help
  When the playlist is played in the order of the list when stored in the task tray, the song information is displayed as a balloon when the song is switched.
  This will be disabled.

  Help]->[About].
  Click on the URL and email address to open the version information.

  Help]->[Readme.txt].
  This will open this text, which I assure you is not helpful at all.

  Help]->[See History.txt].
  Opens the HISTORY.TXT file. I'm not sure if this is of any use.


O About the playlist

  This is a list where you can make various settings for the registered songs and actually play them.
  Most of the list's [Edit] menus can also be found by right-clicking on the song name.

  Files that can be registered to the hoot by D&D can also be registered to the list by D&D.

  You can search for a partial match of either the title or the song title using words entered in the text box at the top.
  You can search by [Search]->[Next](F3) or [Previous](SHIFT+F3) or the button next to the text box.

  File]->[Open List].
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

  Edit]->[Song Number].
  Changes the song number. Only items registered as files can be changed.
  Nothing will happen to other items.
  Both decimal and hexadecimal input will be accepted, and the leading 0 is not necessary for hexadecimal input. (Example: x100)
  If the song number is set to 0xFFFF, the playback code will not be issued when loading.
  If the song number is set to 0xFFFF, the playback code will not be issued upon loading. 0xFFFF is a good value for a song that starts playing immediately after D&D to hoot.
  If the volume is set to anything other than 0x100, the change in volume will be visible for a moment at the beginning of the song, but this is a specification.
  If you are concerned about this, please leave the volume at 0x100.

  Edit]->[Volume].
  Change the volume of the selected song by entering the value of total_mix in hoot.
  Enter the value of total_mix in hoot as it is. 0x100 means equal, 0x200 means double, and so on.
  The input can be either decimal or hexadecimal, and the leading 0 is not necessary for hexadecimal input. (Example: x100)

  Edit]->[Driver Load Weight].
  Specify the weight for loading the selected song. Wait this number of seconds before starting playback.
  This is for titles that take a long time to pre-process, such as those with long ADPCM loading or MIDI.
  The unit is milliseconds. 1000 is 1 second.

  Edit]->[Weight when switching songs].
  This is the weight for changing songs in the same driver.

Translated with www.DeepL.com/Translator (free version)