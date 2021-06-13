# Simulation-of-ball-catching-game-using-POSIX-threads
We would like to build a multi-threading application that simulates a catching game that
kids often play at home. We’ll assume we have 4 brothers/sisters that will participate in
that game.
The game is described as follows:
1. Initially, 3 of the kids are assigned as players while the 4th kid is assigned the role of
ball-seeker. The players are usually located at triangle edges while the ball-seeker is
located in the middle of that triangle.
2. The ball is with one of the players and the game consists of throwing the ball from
one player to another and avoid having the ball-seeker catch the ball.
3. The players and the ball-seeker are not all equally tall. Assume all have tallness that
is random in nature but belongs to the range [160 cm . . . 190 cm].
4. A player who has the ball can select randomly another player to throw the ball to.
5. When a player throws a ball without jumping, assume the ball goes to a height that
is equal to that of the player that threw it in addition to a random extra height in
the range [10 cm . . . 250 cm].
6. If a player that has the ball and would like to throw it to another player while
jumping, the ball goes higher by an extra random height in the range [10 cm . . . 50
cm]. Thus, that makes it more difficult for the ball-seeker to catch it. The fact that
a player having the ball jumps or not when throwing the ball is random.
7. While trying to catch the ball being thrown among players, the ball-seeker might
jump to catch the ball. Assume that the fact that the ball-seeker jumps or not while
trying to catch the ball is random. However, if the ball-seeker jumps, he/she gains
an extra margin in the range [10 cm . . . 50 cm].
8. Assume the ball-seeker is able to catch the thrown ball if the height of the thrown
ball is less than or equal to his/her current height.
9. When a player loses the ball, he/she becomes the new ball-seeker while the current
ball-seeker becomes a player.
10. The above-described game continues until anyone of the players has become a ball-
seeker for 5 times.
