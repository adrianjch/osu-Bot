This is an osu bot that is able to automatically play most of the beatmaps of the game. It can still be improved a bit, but it is already able to **SS** (100% accuracy) a big chunk of the beatmaps of the game. Currently the user has to correctly play the first object of the beatmap, for the bot to be able to determine when did the song start.

---

### DISCLAIMER
I do not endorse the use of cheats in video games. This osu bot was done for coding purposes only. I only used the bot while playing osu offline (not logged into any account).

---

### TODO LIST
- Check why it does not correctly play the maps sometimes after restarting
- Add support for stacked circles
- Add support for all mods
- Fix bezier curves speeds
- Create a way so the user does not need to click the first note of the beatmap in order to start the bot
- Remove hardcoded stuff (pathfile for Stream Companion, screen coordinates, etc)

---

### DEPENDENCIES
[Stream Companion](https://github.com/Piotrekol/StreamCompanion/releases/latest): Used to detect which osu beatmap are we playing. It allows you to save a file with the path of the .osu beatmap that is being played, which is the file that the bot reads to understand the whole beatmap.
