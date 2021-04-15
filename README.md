# Battleship Strategy Simlulator

This code is meant as a beginner instructional sample of C programming.  It started as a simple Battleship human vs. computer game, with the computer simply guessing every move.   This evolved into a slightly smarter approach, and from there another two rounds of improvements to the computer's strategy.

It demonstrates use of arrays, structures, pointers, header file and modules, macros, as well as using terminal escape sequences for color and location.

### What is Battleship
Battleship is a two player game where each player tries to sink their opponent's ships.  Each player has a private 10x10 matrix on which
his 5 ships are deployed.  Ships do not move after they are deployed.  The play takes place in rounds.  For every round each player 
tries to guess locations where his opponent has placed his ships by announcing coordinates.  The other player then tells if it hit a 
ship or not.   The defending player does not tell what ship was hit until it has sunk.  Ships sink will all the cells containing it 
have been targetted.  The attacking player keeps track of his attacks on a second matrix.  This allows them to track where they have 
hits/misses and cells that have not yet been called.
![Full-screen-animated-run_Trim](https://user-images.githubusercontent.com/15236281/114796337-8edbb600-9d45-11eb-8fcd-f912261dfc7c.gif)
### This Project
The project has a couple of central concepts
1) The game is actually 2 games in one.  Player 1 is playing his own game trying to
guess the opponent ship's locations.  Player 1 can decide his own strategy and all his
actions are independent of the opposing player's actions during the game.
2) When considering ways to implement automated strategies, it became clear that having
a target queue was a common need.  Strategy in a simple game like Battleship ends up 
being a list of next targets to attack.  You code up the strategy, then when it has 
an idea of what to do, it puts those coordinates into the target queue.   Each player has
their own target queue.

### The Operational Modes
1) The fully animated computer vs. computer display mode shows Player 1 on top and Player 2 below.  Both players show three
matrices.  The leftmost is the private view of the defending player.  It's shown with the attacking player's screen because
the attacker's strategy is all about figuring out what is in there.  The middle matrix shows what the attacking player KNOWS of the
defender's positions.  The rightmost matrix then takes the known information and adds green letters to areas the strategy 
wants to attack next.  This mode can run an entire game in about a second, plus or minus depending on your system.

![Strategy-Mode_Trim](https://user-images.githubusercontent.com/15236281/114796351-93a06a00-9d45-11eb-9a84-fd5dd55d957b.gif)
2) The Strategy Test Mode is a mode where all the fancy output is turned off and we just crank on the strategies until done.  
When I first implemented it, I seriously thought I had messed up.  It ran about 25,000 trials in about a second!  I added a litle
more output to verify it was progressing, and yes it was executing this extremely fast.  I attribute this to a) the code is small, 
b) the data set is also small, and c) there is no significant I/O.  This means that the program is probably running entirely out of L1 or L2
caches, which is basically greased lighting.  I have setup the test mode to run 1,000,000 games each time, about 30 seconds.  This gives
very solid win/loss percentages relative to other strategies.

3) Logging mode.  This is handy with used with the animated mode.  Using the Pause between turns flag, you can watch where a strategy does something
unexpected and then have a log of the events that got you there.  There is debug code for re-creating the initial board layout and inserting
coordinates into the target queue, thus replaying areas of interest.  Enabling this is a matter of changing the code and re-running it.


