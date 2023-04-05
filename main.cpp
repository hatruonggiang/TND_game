#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

//Screen dimension constants
const int screen_width = 900;
const int screen_height = 800;
const float dot_vel=10;
float mVelX=0,mVelY=0,mPosX=800,mPosY=750;
SDL_Rect RDot;
SDL_Rect RMatrix ;
SDL_Rect RMatrix2;
int matrix_width=1,matrix_height=1;
int dot_width=1,dot_height=1;
int kich_co_anh_in_ra_man_hinh_x=300,kich_co_anh_in_ra_man_hinh_y=300;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void su_kien_ban_phim( SDL_Event& e );

void dich_chuyen();

void kich_co_anh();

void tao_do_anh();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gMatrix = NULL;
SDL_Texture* gDot = NULL;


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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN );
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
void su_kien_ban_phim( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= dot_vel; break;
            case SDLK_DOWN: mVelY += dot_vel; break;
            case SDLK_LEFT: mVelX -= dot_vel; break;
            case SDLK_RIGHT: mVelX += dot_vel; break;
        }
    }
 //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += dot_vel; break;
            case SDLK_DOWN: mVelY -= dot_vel; break;
            case SDLK_LEFT: mVelX += dot_vel; break;
            case SDLK_RIGHT: mVelX -= dot_vel; break;
        }
    }
}

void dich_chuyen()
{
// Lấy kích thước của texture
int myTextureWidth, myTextureHeight;

mPosX += mVelX;
mPosY += mVelY;
float x,y;
if(mVelX>0)
{
    x=mPosX+dot_width;
    y=mPosY+dot_height/2;
}
if(mVelX<0)
{
    x=mPosX-1;
    y=mPosY+dot_height/2;
}
if(mVelY>0)
{
    x=mPosX+dot_width/2;
    y=mPosY+dot_height;
}
if(mVelY<0)
{
    x=mPosX+dot_width/2;
    y=mPosY-1;
}
// lấy pixel
SDL_Rect rect = {x, y, 1, 1};

// Lưu trữ giá trị pixel của texture tại vị trí (x, y) vào biến pixel
Uint32 pixel;
SDL_RenderReadPixels(gRenderer, &rect, SDL_PIXELFORMAT_RGBA8888, &pixel, myTextureWidth * sizeof(Uint32));

// Lấy giá trị r, g, b và a của pixel
Uint8 r, g, b, a;
SDL_GetRGBA(pixel, SDL_GetWindowSurface(gWindow)->format, &r, &g, &b, &a);


//dich chuyenn dot

    //cac dieu kien chan
    if( ( mPosX < 340 ) || ( mPosX -108  > screen_width )  )
    {
        mPosX -= mVelX;
    }
    if( ( mPosY < 280 ) || ( mPosY - 145 > screen_height )  )
    {
        mPosY -= mVelY;
    }
    std::cout<<mPosX<<' ' <<mPosY<<std::endl;
}

void kich_co()
{ // lấy kich cỡ của matrix
    SDL_Texture* kich_co_matrix = IMG_LoadTexture(gRenderer, "image/matrix3.png");
     if (kich_co_matrix == NULL)
    {
    std::cout<<"loi khong lay doc link anh"<<std::endl;
    }

    SDL_QueryTexture(kich_co_matrix, NULL, NULL, &matrix_width, &matrix_height);
  //  std::cout<<matrix_width<<' '<<matrix_height<<std::endl;


    // lấy kích cỡ của dot
     SDL_Texture* kich_co_dot = IMG_LoadTexture(gRenderer, "image/Dot.png");
     if (kich_co_dot == NULL)
    {
    std::cout<<"lỗi không lấy được link của ảnh"<<std::endl;
    }

    SDL_QueryTexture(kich_co_dot, NULL, NULL, &dot_width, &dot_height);
   // std::cout<<dot_width<<' '<<dot_height<<std::endl;

}


void toa_do_anh()
{
RDot={450,400,dot_width,dot_width};
//RMatrix={mPosX,mPosY,matrix_width,matrix_height};
}
void vi_tri_can_show()
{
    RMatrix2={mPosX,mPosY,kich_co_anh_in_ra_man_hinh_x,kich_co_anh_in_ra_man_hinh_y};
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
					su_kien_ban_phim(e);
				}
				dich_chuyen();
                toa_do_anh();
                vi_tri_can_show();
                kich_co();
				//Clear screen
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gMatrix,&RMatrix2, NULL );
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
