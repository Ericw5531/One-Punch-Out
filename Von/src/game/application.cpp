#include "game/application.h"

Application::Application() :
    m_appState(APP_INITIALZING),
	m_countedFrames(0)
{
    printf("Application Initializing... \n");
    if(SDL_Init(SDL_INIT_VIDEO) < 0) printf("SDL could not initialize!\n");
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		g_window = SDL_CreateWindow("Von", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if(g_window == NULL) printf("Window could not be created!\n");
		else
		{
			m_context = SDL_GL_CreateContext(g_window);
			if (m_context == NULL) printf("OpenGL Context could not be created!\n");
			else
			{
				int flags = IMG_INIT_PNG;
				if(!(IMG_Init(flags) & flags)) printf("SDL_image could not initialize!\n");
				if (TTF_Init() == -1) printf("SDL_ttf could not initialize!\n");
			}
			//Initialize SDL_mixer
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) printf("SDL_mixer could not initialize! %s\n", Mix_GetError());

			//TODO: load this volume from a sav file
			Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (g_bgmVolume / 100.f)));
		}
	}
    SDL_ShowCursor(SDL_DISABLE);

	///Initialize OpenGL
	//Initialize GLEW
	if (glewInit() != GLEW_OK) printf("Error initializing GLEW! %s\n", glewGetErrorString(glewInit()));

	if (!GLEW_VERSION_2_1) printf("OpenGL 2.1 not supported!\n");

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//Initiailize Matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.f, 1.f, -1.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glGetError() != GL_NO_ERROR) printf("Error initializing OpenGL! %s\n", gluErrorString(glGetError()));

	srand(static_cast <unsigned> (time(0)));

	changeFontSize(64);

	g_gameState = new Menu(true);

	changeFontSize(16);
}

Application::~Application()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(g_window);

	delete g_gameState;

	SDL_Quit();
    IMG_Quit();
	TTF_Quit();
	Mix_Quit();
}

void Application::setAppState(AppState appState)
{
    m_appState = appState;
}

void Application::run()
{
	m_appState = APP_RUNNING;
    printf("Application State: Running \n");
	while (m_appState != APP_EXITING)
    {
		deltaTime = m_deltaTimer.getTicks() / 1000.f;
        //Cap deltaTime as it may get too large from a freeze/lag spike
        if(deltaTime >= 0.20f) deltaTime = 0.20f;
		m_deltaTimer.start();

        loop();
    }

    printf("Application State: Exiting \n");
}

void Application::loop()
{
	static Uint32 lastTime;
	if (SDL_GetTicks() - lastTime >= 1000)
	{
		m_fpsTime.str("");
		m_fpsTime << "FPS: " << m_countedFrames;
		m_fpsTex.loadFromText(m_fpsTime.str().c_str(), color(255, 255, 255, 255));
		lastTime = SDL_GetTicks();
		m_countedFrames = 0;
	}
    ///----------------
    /// Game Update
    ///----------------
	while (SDL_PollEvent(&m_event))
	{
		if (m_event.type == SDL_KEYUP)
		{
			if (m_event.key.keysym.sym == SDLK_LSHIFT) g_showCollisionBox = !g_showCollisionBox;
		}

		g_gameState->handleEvents(&m_event);
	}

    g_gameState->update(deltaTime);

    ///----------------
    /// Game Rendering
    ///----------------
	glClear(GL_COLOR_BUFFER_BIT);

    g_gameState->render();

	glPopMatrix();
		glLoadIdentity();
	glPushMatrix();
	m_fpsTex.render(0, 0);

	SDL_GL_SwapWindow(g_window);

	m_countedFrames++;
}
