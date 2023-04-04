#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 800;
const float DOT_VEL=0.5;
float mVelX=0,mVelY=0,mPosX=10,mPosY=10;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void handleEvent( SDL_Event& e );

void move1();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gMatrix = NULL;
SDL_Texture* gDot = NULL;
SDL_Rect RDot;
SDL_Rect RMatrix = {50, 50, 800, 800};
void RDot1(){
RDot={mPosX,mPosY,10,10};}
bool init()
{
	//Initialization flag
	bool check = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		check = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			check = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				check = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					check = false;
				}
			}
		}
	}

	return check;
}

bool loadMedia()
{
	//Loading check flag
	bool check = true;

	//Load PNG texture
	gMatrix = loadTexture( "image/matrix3.png" );
	if( gMatrix == NULL )
	{
		printf( "Failed to load texture image!\n" );
		check = false;
	}

	gDot = loadTexture( "image/Dot.png" );
	if( gDot == NULL )
	{
		printf( "Failed to load texture image!\n" );
		check = false;
	}

	return check;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gMatrix );
	gMatrix = NULL;

	SDL_DestroyTexture( gDot );
	gDot = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}
void handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
 //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void move1()
{

  //  std::cout << mPosX<<' '<<mVelX<< ' '<<mPosY<<' '<<mVelY<<std::endl;
// Lấy kích thước của texture
int myTextureWidth, myTextureHeight;
SDL_QueryTexture(gMatrix, NULL, NULL, &myTextureWidth, &myTextureHeight);
mPosX += mVelX;
mPosY += mVelY;
float x,y;
if(mVelX>0)
{
    x=mPosX+10;
    y=mPosY+5;
}
if(mVelX<0)
{
    x=mPosX-1;
    y=mPosY+5;
}
if(mVelY>0)
{
    x=mPosX+5;
    y=mPosY+10;
}
if(mVelY<0)
{
    x=mPosX+5;
    y=mPosY-1;
}

//std::cout<<x<<' '<<y<<std::endl;

// Tạo hình chữ nhật với kích thước của texture
SDL_Rect rect = {x, y, 1, 1};

// Lưu trữ giá trị pixel của texture tại vị trí (x, y) vào biến pixel
Uint32 pixel;
SDL_RenderReadPixels(gRenderer, &rect, SDL_PIXELFORMAT_RGBA8888, &pixel, myTextureWidth * sizeof(Uint32));

// Lấy giá trị r, g, b và a của pixel
Uint8 r, g, b, a;
SDL_GetRGBA(pixel, SDL_GetWindowSurface(gWindow)->format, &r, &g, &b, &a);

// In ra giá trị màu của pixel
std::cout << "Red: " << (int)r << ", Green: " << (int)g << ", Blue: " << (int)b << std::endl;
//Move the dot left or right

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + 10 > SCREEN_WIDTH ) || ((int)r==0 && (int)g==0   ) )
    {
        //Move back
        mPosX -= mVelX;
    }
        //Move the dot up or down


    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + 10 > SCREEN_HEIGHT ) || ((int)r==0 && (int)g==0   ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}
int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					handleEvent(e);
				}
				move1();
                RDot1();

				//Clear screen
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gMatrix, NULL, &RMatrix );
				SDL_RenderCopy( gRenderer, gDot, NULL, &RDot );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
