#include<stdio.h>
#include<SDL.h>

#include<math.h>
//Defining Stuff...
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360
#define FPS_LIMIT 16
#define BALL_MAXIMUM_SPEED 15
#define playerheight 80

struct Cvector
{
    int x ;
    int y;
};
struct Cvector score;
void limit_fps(unsigned int limit)
{
    unsigned int ticks = SDL_GetTicks();
    if(limit < ticks)
        return;
    else if(limit> ticks + FPS_LIMIT)
        SDL_Delay(FPS_LIMIT);
    else
        SDL_Delay(limit - ticks);

}

void ErrorQuit(SDL_Renderer* renderer, SDL_Window* window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int* input(int* flag, SDL_Window* w, SDL_Renderer* r, int* p1, int* p2)
{
    // Declare an Event Type Variable to handle events
    SDL_Event event;
    // You Would want to have only one Event handler:

    while(SDL_PollEvent/*PollEvent Will not Interrupt the program if there is no event*/(&event))
    {
        //block version: SDL_WaitEvent : int <- returns an integer

        //switch is basically a Case Structure
        switch(event.type) //same as Select Case of event.type
        {
        case SDL_KEYUP:
        {
            SDL_FlushEvent(SDL_KEYDOWN);
        }
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.sym) //Case structure to see what key has been pressed
            {
            case SDLK_UP:
            {
                //Action to perform is "B" key is pressed:
                // printf("up key has been pressed! \n");
                *p1 = *p1 - 36;
                //what to do after action is performed
                continue;
            }
            case SDLK_DOWN:
            {
                //printf("Down key has been pressed! \n");
                *p1 = *p1 + 36;
                continue;
            }
            case SDLK_w:
            {
                *p2 = *p2 - 36;
                continue;
            }
            case SDLK_s:
            {
                *p2 = *p2 + 36;
                continue;
            }
            //Case Else:
            default:
            {
                continue;
            }

            }

        }
        case SDL_QUIT :
        {
            printf("Quit");
            *flag = 0;
            break;
        }

        default:
        {
            break;
        }

        }//EndCase
    }//end while pollevent
}

int DisplayTheStuff(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* rectlist[], int n)
{
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0))
    {
        printf("ERROR when changing render colour");
        ErrorQuit(renderer, window);
        return -4;
    }
    SDL_RenderClear(renderer);
    //SDL_RenderPresent(renderer);

    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
    {
        printf("ERROR when changing render colour");
        ErrorQuit(renderer, window);
        return -5;
    }

    for(int i = 0; i <= n-1; i++)
    {
        if (SDL_RenderDrawRect(renderer,rectlist[i]) != 0)
        {
            printf("ERROR when drawing Rect %d ", i+1);
            ErrorQuit(renderer, window);
            return -6 + i;
        }//else{printf("%d rect drawn \n", i);}
    }

    if (SDL_RenderDrawLine(renderer, 240, 0, 240, 360) != 0)
    {
        printf("ERROR when drawing line");
        ErrorQuit(renderer, window);
        return -n;
    }
    SDL_RenderPresent(renderer);
    return 0;
}




int main(int argc, char** argv)
{
    SDL_version nb;
    SDL_VERSION(&nb);
    printf("[Hello, world! Using: SDL %d.%d.%d ] \n", nb.major, nb.minor, nb.patch);
    //VARIABLESS!!
    struct Cvector ballV;
    //int playerheight = 90;
    int loop1Flag = 0;
    int loop2Flag = 0;
    unsigned int frame_limit = 0;
    SDL_Rect ball;
    SDL_Rect player1;
    SDL_Rect player2;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("ERROR during SDL Initialisation");
        return -1;
    }

    SDL_Window* window = NULL;
    window = SDL_CreateWindow("Pong",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              0);
    if (window == NULL)
    {
        printf("ERROR during window Creation");
        return -2;
    }

    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        printf("ERROR during renderer creation");
        SDL_DestroyWindow(window);
        return -3;
    }


    //GAME LOOPES!!
    //Initial Game Stuff

    score.x = 0;
    score.y = 0;
    player1.h = playerheight;
    player1.w = 15;
    player1.x = 0;
    player1.y = (int)(((float)SCREEN_HEIGHT/2.0) - playerheight);
    player2.h = playerheight;
    player2.w = 15;
    player2.x = SCREEN_WIDTH - player2.w;
    player2.y = (int)(((float)SCREEN_HEIGHT/2.0) - playerheight);
    ball.h = 10;
    ball.w = 10;
    ball.x = SCREEN_WIDTH/2;
    ball.y = SCREEN_HEIGHT/2;
    ballV.x = 5;
    ballV.y = 5;

    loop2Flag = 1;
    frame_limit =  SDL_GetTicks() + FPS_LIMIT;
    while(loop2Flag == 1)
    {
        limit_fps(frame_limit);
        //take input
        input(&loop2Flag, window, renderer, &player1.y, &player2.y);
        //game logic
        if (player1.y >= SCREEN_HEIGHT - playerheight )
        {
            player1.y =  SCREEN_HEIGHT - playerheight ;
        }
        if (player1.y <= 0)
        {
            player1.y = 0;
        }
        if (player2.y >= SCREEN_HEIGHT - playerheight)
        {
            player2.y =  SCREEN_HEIGHT - playerheight;
        }
        if (player2.y <= 0)
        {
            player2.y = 0;
        }
        ball.x += ballV.x;
        ball.y += ballV.y;

        if (ball.y <= 0 || ball.y >= 360)
        {
            ballV.y = -1 * ballV.y;
        }

        if(ball.x < player1.w)
        {
            if ((ball.y >player1.y) && (ball.y <(player1.y + playerheight) ))
            {
                printf("Player1 paddle pong \n");
                ballV.x = - ballV.x ;
            }
            else
            {
                score.y ++ ;
                printf("Score: [ %d / %d ] \n", score.x, score.y);
                ballV.x = -1 * ballV.x * 1.1 ;
                if (ballV.x > BALL_MAXIMUM_SPEED )
                {
                    ballV.x = BALL_MAXIMUM_SPEED;
                }
            }
        }
        else
        {
            if((ball.x + ball.w) >  player2.x)
            {
                if ((ball.y >= player2.y) && (ball.y <= (player2.y + playerheight) ))
                {
                    printf("Player2 paddle pong \n");
                    ballV.x = - ballV.x ;
                }
                else
                {
                    score.x ++;
                    printf("Score: [ %d / %d ] \n", score.x, score.y);
                    ballV.x = -1 * ballV.x * 1.1;
                    if (ballV.x > BALL_MAXIMUM_SPEED)
                    {
                        ballV.x = BALL_MAXIMUM_SPEED;
                    }
                }
            }
        }
        //display
        SDL_Rect *rectlist[] = { &ball, &player1, &player2};
        DisplayTheStuff(window, renderer, rectlist, 3);

        //SDL_RenderPresent(renderer);
        frame_limit =  SDL_GetTicks() + FPS_LIMIT;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Goodbye, World! ");
    printf("Score is : %d / %d !", score.x, score.y);
    return(0);
}
