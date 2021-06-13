/*
RAGHAD QADAH 1170142
LEENA BANI ODEH 1172351
SHAYMAA ZAID 1170540
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void play(int *);
void playersIntialize(void);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


#define MaxSeekerCount 5



typedef struct playerStruct
{
    int height;
    int count;
} playerStruct;


playerStruct player1;
playerStruct player2;
playerStruct player3;
playerStruct player4;

//4th kid is assigned the role of ball-seeker
int ballSeeker=4;
//player who throwing the ball 
int ballPitcher;
//player who throw the ball to him
int ballCatcher;


int PitcherTurn=1;
int SeekerHeight;
int ballHeight;

pthread_t p1;
pthread_t p2;
pthread_t p3;
pthread_t p4;

int count[4]= {0,0,0,1};

int main(void)
{
    int      a=1;
    int      b=2;
    int      c=3;
    int      d=4;


    //function to assign highet to each player
    playersIntialize();



    srand(time(0));
    
    printf("\n");
    fflush(stdout);
    printf("********************START**********************\n");
    fflush(stdout);

    // select the first Pitcher Randomly
    ballPitcher=rand()%3 +1;
    
    //Set the text to the color red
    printf("\033[0;31m");
    printf("start the game with player %d to be the ballPitcher\n",ballPitcher);
    fflush(stdout);
    
    //Resets the text to default color
    printf("\033[0;0m");
    printf("\n");
    fflush(stdout);



    //create Four threads for each player
    pthread_create(&p1,NULL, (void *) play, (void*)&a);
    pthread_create(&p2,NULL, (void *) play, (void*)&b);
    pthread_create(&p3,NULL, (void *) play, (void*)&c);
    pthread_create(&p4,NULL, (void *) play, (void*)&d);



    //waits for the thread specified by thread to termainate
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);


    return 0;
}





// function to simulate ball catching game
void play(int* id)
{

    pthread_t thread;
    thread=pthread_self();
    int k;


    //game continues until anyone of the players has become a ball-seeker for 5 times.
    while(count[0]<MaxSeekerCount && count[1]<MaxSeekerCount && count[2]<MaxSeekerCount  && count[3]<MaxSeekerCount )
    {


        sleep(2);


        //first thread (player 1)
        if (pthread_equal(thread,p1))
        {

            //player 1  has the ball to throw it to another player (ballPitcher)
            if(ballPitcher==1)
            {

                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==0)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }
                printf("\033[0;36m");//blue

                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;



                /*the ball goes to a height that is equal to that of the player that throw it in addition to a random extra height in
                the range [10 cm . . . 250 cm]
                */
                int additionHeight=0;
                additionHeight=(rand()%(250-10+1))+10;
                ballHeight=player1.height + additionHeight;

                printf("player1 throw the ball with ball height :%d CM\n",ballHeight);
                fflush(stdout);


                /*
                if throw it to another player while jumping, the ball goes higher by an extra random height in therange [10 cm . . . 50cm]
                */
                if(jumpOrNot==1) //jump=1
                {

                    int extraHeight=(rand()%(50-10+1))+10;
                    ballHeight= ballHeight + extraHeight;
                    printf("player1 jumping with :%d CM\n",ballHeight);
                    fflush(stdout);
                }

                /*
                A player who has the ball(ballPitcher) can select randomly another player to throw the ball to (ballCatcher).
                */
                int r=rand()%3 +1;
                while(ballSeeker==r || ballPitcher==r )
                {

                    r=rand()%4+1;
                }
                ballCatcher=r;

                printf("throw to player %d \n",ballCatcher);
                fflush(stdout);
                printf("\n");
                fflush(stdout);

                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=0;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);


            }


            //player 1  try to catch the ball (ballSeeker)
            if(ballSeeker==1)
            {

                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==1)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }


                //Set the text to the color white
                printf("\033[0;37m");


                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;


                //if the ball-seeker jumps, he/she gains an extra margin in the range [10 cm . . . 50 cm]
                int extraHeight=0;
                //jump
                if(jumpOrNot==1)
                {
                    extraHeight=(rand()%(50-10+1))+10;

                }
                SeekerHeight=player1.height+extraHeight;


                printf("palyer %d -->the ball seeker  try to catch with :%d hight CM\n",ballSeeker,SeekerHeight);
                fflush(stdout);


                //ball-seeker is able to catch the thrown ball if theheight of the thrownball is less than or equal to his/her current height

                //if ballSeeker catch the ball
                if(SeekerHeight>=ballHeight)
                {

                    printf("palyer %d --> the ball seeker catch the ball \n",ballSeeker);
                    fflush(stdout);

                    //When a player loses the ball, he/she becomes the new ball-seeker while the current ball-seeker becomes a player.
                    ballSeeker=ballPitcher;
                    count[ballSeeker-1]=count[ballSeeker-1]+1;

                    //Set the text to the color orange
                    printf("\033[0;33m");
                    printf("palyer%d become new seeker %d times\n",ballSeeker, count[ballSeeker-1]);
                    fflush(stdout);

                    //Resets the text to default color
                    printf("\033[0m");

                    printf("\n");
                    fflush(stdout);


                    //select new Pitcher randomly
                    int rr=rand()%3 +1;
                    while(ballSeeker==rr)
                    {
                        rr=rand()%4+1;
                    }
                    ballPitcher=rr;



                }
                // ballSeeker not catched
                else
                {
                    //the previous catcher become new Pitcher
                    ballPitcher=ballCatcher;
                    printf("palyer %d --> the ball seeker cant catch the ball \n",ballSeeker);

                    fflush(stdout);
                    printf("player %d catch the ball",ballCatcher);
                    fflush(stdout);
                    printf("\n");
                    fflush(stdout);


                }


                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=1;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);

            }

        }




        //second thread (player 2)
        else if (pthread_equal(p2, thread))
        {
            //player 2  has the ball to throw it to another player (ballPitcher)
            if(ballPitcher==2)
            {
                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==0)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }


                //Set the text to the color blue
                printf("\033[0;36m");

                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;

                /*
                the ball goes to a height that is equal to that of the player that throw it in addition to a random extra height in
                the range [10 cm . . . 250 cm]
                */
                int additionHeight=0;
                additionHeight=(rand()%(250-10+1))+10;
                ballHeight=player2.height + additionHeight;

                printf("player2 throw ball with ball hight %d CM\n",ballHeight);
                fflush(stdout);


                /*
                if throw it to another player while jumping, the ball goes higher by an extra random height in therange [10 cm . . . 50cm]
                 */
                if(jumpOrNot==1) //jump=1
                {


                    int extraHeight=(rand()%(50-10+1))+10;
                    ballHeight= ballHeight + extraHeight;
                    printf("player2 jumping with  :%d \n",ballHeight);
                    fflush(stdout);


                }

                /*
                A player who has the ball(ballPitcher) select randomly another player to throw the ball to (ballCatcher).
                */
                int r=rand()%4 +1;

                while(ballSeeker==r || ballPitcher==r )
                {

                    r=rand()%4+1;
                }

                ballCatcher=r;

                printf("throw to player %d \n",ballCatcher);
                fflush(stdout);
                printf("\n");
                fflush(stdout);


                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=0;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);


            }


            //player 2 try to catch the ball (ballSeeker)

            if(ballSeeker==2)
            {
                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==1)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }

                //Set the text to the color white
                printf("\033[0;37m");


                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;


                //if the ball-seeker jumps, he/she gains an extra margin in the range [10 cm . . . 50 cm]
                int extraHeight=0;

                //jump
                if(jumpOrNot==1)
                {


                    extraHeight=(rand()%(50-10+1))+10;

                }

                SeekerHeight=player2.height+extraHeight;


                printf("palyer %d -->the ball seeker  try to catch with :%d hight CM\n",ballSeeker,SeekerHeight);
                fflush(stdout);



                //ball-seeker is able to catch the thrown ball if theheight of the thrownball is less than or equal to his/her current height

                //if ballSeeker catch the ball
                if(SeekerHeight>=ballHeight)
                {

                    printf("palyer %d -->the ball seeker catch the ball \n", ballSeeker);
                    fflush(stdout);
                    //When a player loses the ball, he/she becomes the new ball-seeker while the current ball-seeker becomes a player.
                    ballSeeker=ballPitcher;
                    count[ballSeeker-1]=count[ballSeeker-1]+1;

                    //Set the text to the color orange
                    printf("\033[0;33m");
                    printf("palyer%d become new seeker %d times\n",ballSeeker, count[ballSeeker-1]);
                    fflush(stdout);

                    //Resets the text to default color
                    printf("\033[0m");
                    printf("\n");
                    fflush(stdout);


                    //select new Pitcher randomly
                    int rr=rand()%4 +1;

                    while(ballSeeker==rr)
                    {
                        rr=rand()%4+1;
                    }
                    ballPitcher=rr;




                }
                // ballSeeker not catch the ball
                else
                {
                    //the previous catcher become new Pitcher
                    ballPitcher=ballCatcher;
                    printf("palyer %d -->the ball seeker cant catch the ball \n", ballSeeker);
                    fflush(stdout);
                    printf("player %d catch the ball\n",ballCatcher);
                    fflush(stdout);
                    printf("\n");
                    fflush(stdout);



                }


                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=1;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);

            }

        }



        //Third thread (player 3)
        else  if (pthread_equal(p3, thread))
        {

            //player 3  has the ball to throw it to another player (ballPitcher)
            if(ballPitcher==3)
            {
                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==0)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }

                //Set the text to the color to blue
                printf("\033[0;36m");

                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;

                /*the ball goes to a height that is equal to that of the player that throw it in addition to a random extra height in
                the range [10 cm . . . 250 cm]
                */

                int additionHeight=0;
                additionHeight=(rand()%(250-10+1))+10;

                ballHeight=player3.height + additionHeight;

                printf("player3 throw ball with ball hight %d CM\n",ballHeight);
                fflush(stdout);


                /*
                if throw it to another player while jumping, the ball goes higher by an extra random height in therange [10 cm . . . 50cm]
                */
                if(jumpOrNot==1) //jump=1
                {


                    int extraHeight=(rand()%(50-10+1))+10;
                    ballHeight= ballHeight + extraHeight;
                    printf("player3 jumping with :%d CM\n",ballHeight);
                    fflush(stdout);


                }

                /*
                A player who has the ball(ballPitcher) can select randomly another player to throw the ball to (ballCatcher).
                */
                int r=rand()%3 +1;

                while(ballSeeker==r || ballPitcher==r )
                {

                    r=rand()%4+1;


                }
                ballCatcher=r;

                printf("throw to player %d \n",ballCatcher);
                fflush(stdout);
                printf("\n");
                fflush(stdout);



                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=0;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);

            }


            //player 1  try to catch the ball (ballSeeker)
            if(ballSeeker==3)
            {
                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==1)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }


                //Set the text to the color white
                printf("\033[0;37m");

                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;
                int extraHeight=0;

                //if the ball-seeker jumps, he/she gains an extra margin in the range [10 cm . . . 50 cm]
                if(jumpOrNot==1) //jump
                {
                    extraHeight=(rand()%(50-10+1))+10;


                }
                SeekerHeight=player3.height+extraHeight;

                printf("palyer %d -->the ball seeker  try to catch with :%d hight CM\n",ballSeeker,SeekerHeight);
                fflush(stdout);



                //ball-seeker is able to catch the thrown ball if theheight of the thrownball is less than or equal to his/her current height

                //if ballSeeker catch the ball
                if(SeekerHeight>=ballHeight)
                {




                    printf("player %d --> the ball seeker catch the ball \n",ballSeeker);
                    fflush(stdout);

                    //When a player loses the ball, he/she becomes the new ball-seeker while the current ball-seeker becomes a player.
                    ballSeeker=ballPitcher;
                    count[ballSeeker-1]=count[ballSeeker-1]+1;

                    //Set the text to the color orange
                    printf("\033[0;33m");
                    printf("palyer%d become new seeker %d times\n",ballSeeker, count[ballSeeker-1]);
                    fflush(stdout);

                    //Resets the text to default color
                    printf("\033[0m");
                    printf("\n");
                    fflush(stdout);


                    //select new Pitcher randomly
                    int rr=rand()%4 +1;


                    while(ballSeeker==rr)
                    {
                        rr=rand()%4+1;
                    }
                    ballPitcher=rr;

                    // ballSeeker not catched
                }
                else
                {

                    ballPitcher=ballCatcher;
                    printf("player %d --> the ball seeker cant catch the ball \n",ballSeeker);
                    fflush(stdout);
                    printf("player %d catch the ball\n",ballCatcher);
                    fflush(stdout);
                    printf("\n");
                    fflush(stdout);



                }
                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=1;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);


            }


        }


        //fourth thread (player 4)
        else if (pthread_equal(p4, thread))
        {



            //player 1  has the ball to throw it to another player (ballPitcher)
            if(ballPitcher==4)
            {
                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==0)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }

                //Set the text to the color blue
                printf("\033[0;36m");

                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;


                /*
                the ball goes to a height that is equal to that of the player that throw it in addition to a random extra height in
                the range [10 cm . . . 250 cm]
                */
                int additionHeight=0;
                additionHeight=(rand()%(250-10+1))+10;
                ballHeight=player4.height + additionHeight;

                printf("player4 throw ball with ball height %d CM\n",ballHeight);
                fflush(stdout);

                /*
                if throw it to another player while jumping, the ball goes higher by an extra random height in therange [10 cm . . . 50cm]
                */

                //jump=1
                if(jumpOrNot==1)
                {

                    int extraHeight=(rand()%(50-10+1))+10;
                    ballHeight= ballHeight + extraHeight;
                    printf("player4 jumping with :%d CM\n",ballHeight);
                    fflush(stdout);

                }

                /*
                A player who has the ball(ballPitcher) can select randomly another player to throw the ball to (ballCatcher).
                */

                int r=rand()%4 +1;
                while(ballSeeker==r || ballPitcher==r )
                {

                    r=rand()%4+1;
                }

                ballCatcher=r;

                printf("throw to player %d \n",ballCatcher);
                fflush(stdout);
                printf("\n");
                fflush(stdout);


                //ballPitcher=r;
                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=0;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);

            }


            //player 4  try to catch the ball (ballSeeker)
            if(ballSeeker==4)
            {
                //Block access by other threads.
                pthread_mutex_lock(&mutex);

                while (PitcherTurn==1)
                {
                    //Waiting on condition variable "cond"
                    pthread_cond_wait(&cond, &mutex);

                }

                //Set the text to the color white
                printf("\033[0;37m");

                //1=jump ,0=notjumped
                int jumpOrNot=rand()%2;

                //if the ball-seeker jumps, he/she gains an extra margin in the range [10 cm . . . 50 cm]
                int extraHeight=0;

                //jump=1
                if(jumpOrNot==1)
                {
                    extraHeight=(rand()%(50-10+1))+10;
                }

                SeekerHeight=player4.height+extraHeight;


                printf("palyer %d -->the ball seeker  try to catch with :%d hight CM\n",ballSeeker,SeekerHeight);
                fflush(stdout);


                //ball-seeker is able to catch the thrown ball if theheight of the thrownball is less than or equal to his/her current height

                //if ballSeeker catch the ball
                if(SeekerHeight>=ballHeight)  //catch the ball
                {




                    printf("palyer %d -->the ball seeker catch the ball \n",ballSeeker);
                    fflush(stdout);
                    //When a player loses the ball, he/she becomes the new ball-seeker while the current ball-seeker becomes a player.
                    ballSeeker=ballPitcher;
                    count[ballSeeker-1]=count[ballSeeker-1]+1;

                    //Set the text to the color orange
                    printf("\033[0;33m");
                    printf("palyer%d become new seeker %d times\n",ballSeeker, count[ballSeeker-1]);
                    fflush(stdout);

                    //Resets the text to default color
                    printf("\033[0m");
                    printf("\n");
                    fflush(stdout);


                    //select new Pitcher randomly
                    int rr=rand()%4 +1;
                    while(ballSeeker==rr)
                    {
                        rr=rand()%4+1;
                    }
                    ballPitcher=rr;



                }
                // ballSeeker not catched
                else
                {
                    //the previous catcher become new Pitcher
                    ballPitcher=ballCatcher;
                    printf("palyer %d -->the ball seeker cant catch the ball \n",ballSeeker);
                    fflush(stdout);
                    printf("player %d catch the ball\n",ballCatcher);
                    fflush(stdout);
                    printf("\n");
                    fflush(stdout);

                }


                for ( k = 0; k < 1000000; k++ );
                PitcherTurn=1;

                //unblock threads blocked on a condition variable "cond"
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);


            }

        }

    }
    
    printf("\n");
    fflush(stdout);
    printf("********************END**********************\n");
    fflush(stdout);

    exit(0);
}






//function to assign highet to each player
void playersIntialize(void)
{
    srand(time(0));
    //Intialize tall to each player that is random in nature but belongs to the range [160 cm . . . 190 cm].
    player1.height=(rand()%(190-160+1))+160;
    player2.height=(rand()%(190-160+1))+160;
    player3.height=(rand()%(190-160+1))+160;
    player4.height=(rand()%(190-160+1))+160;

    player1.count=0;
    player2.count=0;
    player3.count=0;
    player4.count=0;

}
