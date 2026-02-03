# Toolfish

A powerful Windows system tray utility for web monitoring, hotkeys, macros, and automation.

## [Download latest version](https://rtsoft.com/ToolfishFullInstaller.exe) (For Windows, signed by RTsoft)

## Screenshots

<p align="center">
  <a href="screenshots/main_screen.png"><img src="screenshots/main_screen.png" width="32%" /></a>
  <a href="screenshots/toolfish_ss2.gif"><img src="screenshots/toolfish_ss2.gif" width="32%" /></a>
  <a href="screenshots/toolfish_ss3.gif"><img src="screenshots/toolfish_ss3.gif" width="32%" /></a>
</p>

## Description

Toolfish is a swiss army knife feature-rich Windows system tray application that handles global hotkeys, macros, monitors websites for changes (percent changed detection), text-to-speech notifications, scheduled tasks, and much more.  

It has kind of an internal scripting system that allows you to specify "triggers" and "actions" to do stuff automatically on your computer.

This was a commercial app I released in 2002, eventually making free in 2015.  As of Feb 2026, it's now open source too!  Anytime I need a little utility to do something, I throw it into this app.  

It can be moved to a new computer very easily simply by copying its folder.

## Triggers & Actions Reference

Toolfish uses a powerful event system where **Triggers** determine *when* something happens, and **Actions** determine *what* happens.

### Triggers

| Trigger | Description |
|---------|-------------|
| **Time** | Run at a specific time with auto-rescheduling options |
| **Time Range** | Only activate within a specified time window |
| **Hotkey** | Trigger on any key combination (global hotkeys) |
| **Inactivity** | Trigger based on keyboard/mouse/gamepad idle time |
| **Day Filter** | Limit events to specific days of the week |
| **Process** | Trigger when a process starts, stops, or has focus |

### Actions

| Action | Description |
|--------|-------------|
| **Say/Show/Email/Macro Message** | Output text via TTS, popup, email, log, or keystrokes |
| **Play Sound** | Play a .wav audio file |
| **Open Website** | Open a URL in your default browser |
| **Web Compare** | Check if a webpage has changed (conditional) |
| **Run/Open File** | Execute programs, batch files, or open any file |
| **Stealth Mode Toggle** | Hide/show the system tray icon |
| **Check Email** | Monitor a POP3 inbox for new messages (conditional) |
| **Open/Hide Toolfish Toggle** | Toggle Toolfish window visibility |
| **Atomic Clock Time Adjust** | Sync system time from an internet time server |
| **Enable/Disable TTS** | Turn text-to-speech on or off |
| **Enable/Disable Smart Mute** | Turn the auto-mute feature on or off |
| **Set System Volume** | Adjust system volume with optional fade |
| **Set Mic Input Volume** | Adjust microphone input level |
| **Set Sound Output Device** | Switch to a specific audio output device |
| **Site Monitor** | Check server uptime and connectivity (conditional) |
| **Send Command To Application** | Control other windows (close, minimize, send keys) |
| **Delay** | Wait a specified time before the next action |
| **Logoff/Shutdown/Reboot** | Power actions for the system |
| **Leet-Type Toggle** | Enable/disable l33t sp34k typing mode |
| **Reset Screen Resolutions** | Fix display issues (NVidia workaround) |

## Other Features

* **Keyboard Spy** - Stealth keyboard logging (use responsibly!)
* **Smart Mute** - Automatically mute your computer after inactivity, with optional volume knob control
* **Fun statistics** - Track how many miles you've moved your mouse, button clicks, and more
* **Built-in hotkeys** - Comes with useful defaults like Ctrl-Shift-Alt-I to paste your IP address

## Old Original documentation is [here](https://rtsoft.com/toolfish/index.php) and [here](https://rtsoft.com/toolfish/help.htm)

## Usage

1. Run `Toolfish.exe`
2. The application will appear in your system tray
3. Use the **Event Wizard** to quickly set up common monitoring tasks
4. Use the **Event Manager** for full control over triggers and actions

### Quick Start Examples

* **Monitor a website** - Use Event Wizard → Web Compare to get notified when a page changes or has a certain keyword
* **Set a reminder** - Create a Time trigger with a TTS (text-to-speech) action
* **Create a hotkey** - Add a Hotkey trigger with a Run action to launch programs
* **Auto-mute** - Enable Smart Mute to silence your PC after inactivity

## Safety

It's a bit scary to run an a system trap app that can see everything you do (especially if you enable elevated access) so a bit about me:

* I am Seth A. Robinson ([Wikipedia](https://en.wikipedia.org/wiki/Robinson_Technologies)), having written over 50+ games and utilities over the last 35 years

* This app doesn't share your personal data at all.  It does hit RTsoft for two functions though:

  * Getting your true IP address - It hits [rtsoft.com/htbin/ip.php](rtsoft.com/htbin/ip.php) to get your 'outside' IP address, for use with the default IP address hotkey macro

  * Checking for updates - It hits [rtsoft.com/toolfish/server.txt](https://rtsoft.com/htbin/ip.php) to see what the latest version is.  Also also sends the current version of toolfish as a parm.

Both of these functions can be disabled in Options menu.

* The version I have here for download has been signed by Robinson Technologies Corporation, my company.  The first time you run it, Windows will popup telling you this - if it doesn't (the file isn't signed) than DON'T run it, it didn't come for me. 

* You can also compile it yourself using Visual Studio 2026 and run that version

## History

**V2.33 Feb 3nd, 2026**
* Added "Set audio output device" action.
* Added "modify volume knob sensitivity" option to the Smart Mute menu.
* Added "Duplicate" to Event Manager dialog.
* "Inactivity" detection improved, now monitors gamepads and detects Windows' "display required' state (watching movies, etc)
* Added option to autostart in admin mode, otherwise hotkeys and inactivity detection won't work with other apps that ARE running in admin mode, like Genshin Impact (I use this to auto mute it when my kid is AFK)
* Using low level hook instead of DLL injection now, should work a bit better I guess
* HTTPS support added for web compares/etc
* Fixed issue with not respecting the system's default browser setting
* Source code released.

## Building from Source

1. Open `Toolfish.sln` in Visual Studio 2026
2. Build the solution (both Release and Debug configurations available)
3. The keyboard hook DLL (`SMDLL`) will be built automatically as part of the solution

## License: BSD-style attribution, see [LICENSE.md](LICENSE.md)

## Credits

* Written by Seth A. Robinson ([seth@rtsoft.com](mailto:seth@rtsoft.com))
* Art by Akiko
* Twitter: [@rtsoft](https://twitter.com/rtsoft)
* Website: [rtsoft.com](https://www.rtsoft.com)
* Blog: [Codedojo](https://www.codedojo.com)

