# SpyHunter
A project in C using the SDL library for the subject basics of programming

In this project student is expected to write desktop application of an Atari game called Spy Hunter. The
object of the game is to drive down roads and destroy other (enemy) vehicles, using the weapons that are
available for the player. The project is based on the Atari 2600 version of the game

 Program controls
The key controls should utilized in a specified way:
• arrows: moving the player in given direction;
• esc: quit the program;
• n: start a new game;
• s: save the game state;
• l: load the game state;
• p: pause/continue;
• space: shooting;
• f: finish the game

Mandatory requirements
All elements stated here have to be implemented. Lack of any of the following elements results in gaining 0
points from the project:
(a) The game board has to be presented in a aestethic and ergonomic way. The upper space of the window
should contain the name, surname and index number of the author.
(b) Displaying the elapsed time and score during gameplay. Both values are reset when the new game is
started.
(c) The basic functionality of the game: the movement, the road form (with collisions). The movement is
immediate – responding to the events.
(d) Supporting esc and n controls according to the Section 2.1.
(e) The game should also keep the score according to the mechanics of the game described in the linked
article.
(f) The right bottom corner of the game screen should contain the letters of implemented elements.
(g) The road should have a constant width, the change of width does not have to be implemented.

Optional requirements
The tasks should be implemented according to the article linked in the Section 1 and additional materials:
(g) Saving and restoring game state. Save and load game from file (keys s and l). Saved game
is identified by the time of the save (date, hour, minute and second). When loading, the list of saved
games is displayed and the user choosed the position from the list in a way chosen by the author.
(i) Pausing the game. The player should be able to pause the game and then be able to return back
to the play.
(j) Enemies and other cars. The gameplay should include enemies that the player can chase, that
can attack and overrun the player, but also regular, non-enemy cars that when destroyed halt the score
counter for a while.
(k) Shooting. The player can shoot both enemy and non-enemy cars, this might impact the score in
a way described in the article.
(l) Forcing out of the road. The player can force off the road both enemy and non-enemy cars, this
might impact the score in a way described in the article.
(m) Getting some cars. For a short period of time in the beginning the player should have the unlimited
amount of cars (even if their car is destoyed, the one appears immediately). However, after that period,
the number of cars should be limited and the new ones should be obtained according to the article.
When the number of available cars is zero, the game should be over and a screen that informs about
it should be displayed.
(n) Power-up. Another weapon might appear on the road (This is a small simplification – no Weapons Van described in the article) and when collected (by riding on them) it temporarily (limited ammo)
changes the way the shooting works (for example it allows for longer-range shooting).
