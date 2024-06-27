/*****************************************************************************
 * Copyright 2019 Johann 'Myrkraverk' Oskarsson                              *
 * Email: johann@myrkraverk.com                                              *
 * License: WTFPL 2.0                                                        *
 * Version 1.0.1                                                             *
 *                                                                           *
 * A very basic example of using SDL to display a texture.  See the README   *
 * file for build instructions.                                              *
 *****************************************************************************/

/* We include the SDL header file.
 */
#include <SDL.h>

/* And also two standard headers for our command line parser.
 */
#include <stdlib.h>
#include <string.h>

/* The main() function displays a BMP file on the screen and waits for
 * the user to press the X to close the window.
 *
 * There are no error checks for brevity.  If something goes wrong,
 * it's easy to add a check and a call to SDL_GetError() to figure out
 * why.
 */
int main( int argc, char *argv[] ) {

  /* The width, height and framerate of our window.
   */
  int x = 640, y = 480, f = 60;

  /* On a POSIX system we'd use getopt() but on Windows we have to
   * roll our own command line parser.  See the README file for how
   * to use these flags.
   */
  for ( int i = 1; i < argc; i++ ) {
    if        ( strcmp( argv[ i ], "-x" ) == 0 && argc > i + 1 ) {
      x = (int) strtol( argv[ ++i ], NULL, 10 );
    } else if ( strcmp( argv[ i ], "-y" ) == 0 && argc > i + 1 ) {
      y = (int) strtol( argv[ ++i ], NULL, 10 );
    } else if ( strcmp( argv[ i ], "-f" ) == 0 && argc > i + 1 ) {
      f = (int) strtol( argv[ ++i ], NULL, 10 );
    }
  }

  /* We initialize the SDL video and event subsystems.  If we forget
   * or leave ot the SDL_INIT_EVENTS, the SDL_CreateWindow() will
   * initialize it for us.  There is no error checking in our example
   * code, and in a real game we might explicitly check if this fails
   * and try to notify the user somehow.
   */
  SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );

  /* We create a new window to display our message.  Here we pass in *
   * the title text, where it should be on the screen (which is *
   * undefined) and the size as x and y, and finally a flag to say *
   * it's shown.
   */
  SDL_Window *window = SDL_CreateWindow( "Hello World", 
					 SDL_WINDOWPOS_UNDEFINED,
					 SDL_WINDOWPOS_UNDEFINED, 
					 x, y, SDL_WINDOW_SHOWN );

  /* We load the bitmap we're going to display as our "hello world"
   * message.
   */
  SDL_Surface *surface = SDL_LoadBMP( "hello.bmp" );

  /* We create a hardware accelerated renderer.  This is liable to
   * fail on some systems, and again there's no error checking.
   * 
   * We pass in the window we created earlier, and use -1 for the
   * rendering driver to get the first one available.
   *
   * We explicitly ask for hardware redering with
   * SDL_RENDERER_ACCELERATED.
   */
  SDL_Renderer *renderer = 
    SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

  /* We create the texture we want to disaply from the surface we
   * loaded earlier.
   */  
  SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, surface );

  /* We're now done with the surface, so we free the resources.
   */
  SDL_FreeSurface( surface );
  /* And set the pointer to NULL so it won't accidentally be used for
   * something else.
   */
  surface = NULL;

  /* The quit flag.  We run the game loop until this flag becomes
   * true.
   */
  SDL_bool quit = SDL_FALSE;
  /* The event we're going to receive.  Initialised to all zeros.
   */
  SDL_Event e = { 0 };

  /* Here is a very simple game loop, with a soft framerate.  There is
   * a chance an individual frame will take longer, and this happens
   * predictably when the user presses the X button to close the
   * window, and the delay routine can oversleep because of operating
   * system scheduling.
   */
  while ( !quit ) {
    /* We notice our start time of the loop, so we can shave off
     * milliseconds later.
    */
    Uint32 start = SDL_GetTicks();

    /* Our basic event poll system.  We pass through all events in the
     * queue and look for the SDL_QUIT message.  This happens
     * typically when the user presses the upper right corner X on
     * Windows.
     */
    while ( SDL_PollEvent( &e ) )
      switch ( e.type ) {
	/* We got the SDL_QUIT message, so we set the quit flag.
	 */
      case SDL_QUIT: quit = SDL_TRUE; break;
	/* In all other cases we do nothing.
	 */
      default: break;
      }
    
    /* Our rendering part of the game loop.
     *
     * We clear the screan with the default colour (because we don't
     * explicitly set the colour anywhere).
     */
    SDL_RenderClear( renderer );
    /* Then copy the texture we created on to the entire screen.
     *
     * That's what the NULL, NULL means: use the entire texture on the
     * entire screen.
     */
    SDL_RenderCopy( renderer, texture, NULL, NULL );
    /* And finally tell the renderer to display on screen what we've
     * drawn so far.
     */
    SDL_RenderPresent( renderer );

    /* We take a note of the time at the end of our game loop, so we
     * can calculate below how long it took.
     */
    Uint32 end = SDL_GetTicks();

    /* We subtract end - start to get the time spent in our game loop.
     * This is very basic clock usage.  We then calculate how long we
     * should wait in milliseconds given the current framerate f, and
     * subtract again from that our previous results, to get the time
     * we should spend waiting.
     * 
     * If end - start is larger than 1000 / f, this will turn
     * negative; in which case we don't want to wait at all.
     */
    int wait_for = 1000 / f - ( end - start );

    /* If we calculated positive milliseconds to wait_for, we wait.
     */
    if ( wait_for > 0 ) {
      SDL_Delay( wait_for );
    }
  }

  /* Clean up after ourselves before we quit.
   */
  SDL_DestroyTexture( texture );
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
  return 0;
}
