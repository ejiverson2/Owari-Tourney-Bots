# Owari-Tourney-Bots
This project was created for an AI class tournament. Bots, including this one, competed against other teams. This is the undefeated champion bot with a 16-0 record.
### Rules of Owari
Owari is a "pit and seed game" played with two players. Each player (north and south) has six pits on their side. The extra pit to a players right is their "goal pit". On a player's turn, they may select a pit to move. Then, they evenly distribute the seeds one at a time going counter-clockwise into the other pits. If the last seed lands in one of your empty pits, you may "capture" the seeds in the opposing pit (on the other players side) and put them into your goal pit. The game ends when one side can not make a move. At that point, all remaining seeds on their side go into their own goal.

For this particular tournament, turns could be no longer than 30 seconds.

### The AI
The AI (written in C++) uses five different techniques to succeed:
1. Minimax Algorithm
   * Minimax is an algorithm that looks at the possible end states of a game and predict which state two optimal players would get to.
2. Heruistic Function
   * Because the minimax tree would be too deep to calculate in 30 seconds, intermediate states are given a score and used to calculate minimax moves rather than final states.
3. Alpha-beta Pruning
   * Alpha-beta Pruning is a method to greatel reduce the search space of a minimax tree.
4. An Aspiration Window
   * To further reduce the search space, the bot can predict what value the next move will probably have and not go throught the trouble of calculating more of the tree. The aspiration window technique relies on state values that do not change very rapidly.
5. Iterative Deepening
   * A key to victory is getting the most out of your time. The tournament had 30 seconds, so this AI repeats the search for end states, each time at a deeper depth. Every turn from the bot takes the entire 30 seconds and always went deeper in the tree than its competitors.

### The Discord Bot
Before designing the bot, I needed to learn how to play Owari at a high level. So, I created a discord bot so that I could duel other people in the text channels. Eventually, this bot was also used to test the AI against human players.

Bot Chat Command: "play owari"

[a *lot* more could be said about this bot]
