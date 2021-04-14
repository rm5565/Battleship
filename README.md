# Battleship
### Battleship Strategy Simlulator

This code is meant as an intermediate instructional sample of C programming.  It started as a simple Battleship human vs. computer game, with the computer simply guessing every move.   This evolved into a slightly smarter approach, and from there another two
rounds of improvements to the computer's strategy.

### What is Battleship
Battleship is a two player game where each player tries to sink their opponent's ships.  Each player has a private 10x10 matrix on which
his 5 ships are deployed.  Ships do not move after they are deployed.  The play takes place in rounds.  For every round each player 
tries to guess locations where his opponent has placed his ships by announcing coordinates.  The other player then tells if it hit a 
ship or not.   The defending player does not tell what ship was hit until it has sunk.  Ships sink will all the cells containing it 
have been targetted.  The attacking player keeps track of his attacks on a second matrix.  This allows them to track where they have 
hits/misses and cells that have not yet been called.

### This Project
The project has a couple of central concepts
1) The game is actually 2 games in one.  Player 1 is playing his own game trying to
guess the opponent ship's locations.  Player 1 can decide his own strategy and all his
actions are independent of the opposing player's actions during the game.
2) When considering ways to implement automated strategies, it became clear that having
a potential target queue was a common need

![Command Prompt - battleship exe 2021-04-14 13-21-30](https://user-images.githubusercontent.com/15236281/114780393-321fd180-9d2c-11eb-8bec-e4d63c689036.gif)
