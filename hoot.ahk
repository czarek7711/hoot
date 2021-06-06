process, exist, hoot shuffle.exe 	 ; prevents script from running unless hoot shuffle is running
if !errorlevel
	ExitApp
process, exist, hoot.exe 	 	 ; closes hoot shuffle if hoot was closed
if !errorlevel
	Process, Close, hoot shuffle.exe
ControlSend,, {Esc 2}, ahk_exe hoot.exe  ; this stuff is for resetting back to the beginning when we use the manual shuffle button
Sleep 100
ControlSend,, {Up}, ahk_exe hoot.exe
Sleep 100
ControlSend,, {Enter}, ahk_exe hoot.exe
Sleep 100
PlayRandomSong()

PlayRandomSong()
{
	Loop
	{
		process, exist, hoot shuffle.exe 			   ; closes the script if hoot shuffle gets closed
		if !errorlevel
			ExitApp
		Random, timesToPD, 0, 30 				   ; how many times we page down on the games directory
		Random, timesToArrowKey, 0, 12 				   ; how many times we press the arrow key after paging down
		ControlSend,, {PgDn %timesToPD%}, ahk_exe hoot.exe
		ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe
		ControlSend,, {Enter}, ahk_exe hoot.exe 	 	   ; enter this randomly chosen game directory
	
		Random, downOrUp, 0, 1 ; down = 0 up = 1
		if (downOrUp = 0)
		{
			Random, timesToArrowKey, 0, 5
			ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe
			ControlSend,, {Enter}, ahk_exe hoot.exe 			; play the selected track
			Random, continueArrowKey, 0, 1 					; reason for continueArrowKey is to get better variety when shuffling tracks, previously it was choosing the first and last tracks too often
			if (continueArrowKey = 1)
			{
				Random, timesToArrowKey, 0, 10
				ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe
				ControlSend,, {Enter}, ahk_exe hoot.exe
				Random, continueArrowKey, 0, 1
				if (continueArrowKey = 1)
				{
					Random, timesToArrowKey, 0, 20
					ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe
					ControlSend,, {Enter}, ahk_exe hoot.exe
					Random, continueArrowKey, 0, 1
					if (continueArrowKey = 1)
					{
						Random, timesToArrowKey, 0, 30
						ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe
						ControlSend,, {Enter}, ahk_exe hoot.exe
						Random, continueArrowKey, 0, 1
						if (continueArrowKey = 1)
						{
							Random, timesToArrowKey, 0, 40
							ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe
							ControlSend,, {Enter}, ahk_exe hoot.exe
						}
					}
				}
			}
	
		}
		else
		{
			Random, timesToArrowKey, 0, 5
			ControlSend,, {End}, ahk_exe hoot.exe ; go to the bottom before going up
			ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe
			ControlSend,, {Enter}, ahk_exe hoot.exe ; play the selected track
			Random, continueArrowKey, 0, 1
			if (continueArrowKey = 1)
			{
				Random, timesToArrowKey, 0, 10
				ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe
				ControlSend,, {Enter}, ahk_exe hoot.exe
				Random, continueArrowKey, 0, 1
				if (continueArrowKey = 1)
				{
					Random, timesToArrowKey, 0, 20
					ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe
					ControlSend,, {Enter}, ahk_exe hoot.exe
					Random, continueArrowKey, 0, 1
					if (continueArrowKey = 1)
					{
						Random, timesToArrowKey, 0, 30
						ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe
						ControlSend,, {Enter}, ahk_exe hoot.exe
						Random, continueArrowKey, 0, 1
						if (continueArrowKey = 1)
						{
							Random, timesToArrowKey, 0, 40
							ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe
							ControlSend,, {Enter}, ahk_exe hoot.exe
						}
					}
				}
			}
		}
	
		Sleep 1000 				; need to wait a second after playing otherwise it will stop the track
		ControlSend,, {Esc}, ahk_exe hoot.exe 	; get back to the game directory
		ControlSend,, {Home}, ahk_exe hoot.exe 	; go to the top of the game directory
		Sleep 180000 				; let the track play for 3 minutes (180 seconds)
	}
}

ForceRandomSong()
{
	Reload
}

`::ForceRandomSong() ; rebinds ` (~ key) to call the ForceRandomSong function - manual shuffle button