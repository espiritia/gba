The following branches maintained by contributors can be a good starting point for your hack, or can be merged into an existing codebase. Please visit each page, wiki, post, etc. to see information on how to give credit!


## Table of Contents
- [How to apply a feature branch](#how-to-apply-a-feature-branch)
- [Remove Help System](#remove-help-system)
- [Port RTC from Emerald](#port-rtc-from-emerald)

## How to apply a feature branch

1. Add the repository as a remote on git: `git remote add <name of your choice> <repository url>`

    The URL you use is the same one you use when cloning the repository (so it should have `.git` at the end).

2. Pull the specific branch you want: `git pull <remote name you gave earlier> <branch_name>`

## [Remove Help System](https://github.com/geajack/pokefirered/tree/remove-help-system)

Removes the help system from the game. This is the feature where pressing L or R at any point will go to a blue screen with help information. With these changes, pressing L and R simply do nothing. Note that this does not remove the intro tutorial that plays before Prof. Oak's speech when a new game is started.

The main advantage of this patch is that the help system uses up **16kB of EWRAM**, one of the largest single memory hogs in the game. Since the vanilla game runs pretty close to its EWRAM limit to begin with, it's fairly easy to run out when adding new features.

## [Port RTC from Emerald](https://www.pokecommunity.com/threads/simple-modifications-directory.416647/page-15#post-10391167)

This is Sotomura's port of the RTC from Emerald. It makes Pokerus be catchable and spread, and fixes Eevee's time-based evolutions, but otherwise no time-based events occur. It includes the wallclock specials and the RTC reset screen.