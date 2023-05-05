

#include <time.h>

#include "src/game.h"


const char *app_name = "Flappy melancholy";
const int app_screen_width = 190;
const int app_screen_height = 60;


/******************************************************************************/

/**
 * main function
 * @return -
 */
int main(void)
{
    DWORD next_game_tick;
    int sleep_time, state;

    HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hnd == INVALID_HANDLE_VALUE) {
        printf("Window handle is invalid!\n");
        return EXIT_FAILURE;
    }
    SetConsoleTitle(app_name);

    CONSOLE_SCREEN_BUFFER_INFOEX info = {0};
    info.cbSize = sizeof(info);
    if (GetConsoleScreenBufferInfoEx(hnd, &info) == 0) {
        printf("GetConsoleScreenBufferInfoEx failed\n");
        return EXIT_FAILURE;
    }

    // screen max size
    COORD maxScreen = GetLargestConsoleWindowSize(hnd);
    if ((maxScreen.X < app_screen_width) || (maxScreen.Y < app_screen_height)) {
        printf("Screen is too small!\n");
        printf("Max supported: width(%i) height(%i)\n", maxScreen.X, maxScreen.Y);
        printf("Needed: width(%i) height(%i)\n", app_screen_width, app_screen_height);
        return EXIT_FAILURE;
    }
    console_set_size(hnd, app_screen_width, app_screen_height);
    console_set_canvasSize(hnd, app_screen_width-1, app_screen_height-1);

    /* buffer setup */
    memset(&gGAMESTATE, 0, sizeof(gGAMESTATE));
    if (0 != bufferConsole_new(&gGAMESTATE.buffer, hnd, app_screen_width, app_screen_height)) {
        printf("Can't init the console\n");
        return EXIT_FAILURE;
    }

    /* Init */
    gGAMESTATE.sprites = aSPRITES;
    gGAMESTATE.sprites_max = SPRITES_MAX;
    gGAMESTATE.blend[0] = rgba32_css("Thistle");
    gGAMESTATE.blend[1] = rgba32_css("White");
    gGAMESTATE.blend[2] = rgba32_css("White");
    gGAMESTATE.blend[3] = rgba32_css("Yellow");
    gGAMESTATE.blend[4] = rgba32_css("White");
    gGAMESTATE.blend[5] = rgba32_css("White");
    gGAMESTATE.blend[6] = rgba32_css("White");
    gGAMESTATE.blend[7] = rgba32_css("White");
    gGAMESTATE.blend[8] = rgba32_css("White");
    gGAMESTATE.blend[9] = rgba32_css("Violet");

    console_cls(gGAMESTATE.buffer.hnd);
    memset(aSPRITES, 0, gGAMESTATE.sprites_max);

    /* timing stuff */
    #define FRAMES_PER_SECOND  60
    #define SKIP_TICKS        (1000 / FRAMES_PER_SECOND)
    next_game_tick = GetTickCount();
    sleep_time = 0;

    //PlaySound("data/audio/title.wav", NULL, SND_ASYNC);


    /* init title screen */
    game_open_title(&gGAMESTATE,1);

    /* title loop */
    for (state = 1; state > 0; ) {
        state = game_update_title(&gGAMESTATE,1);
        bufferConsole_update(&gGAMESTATE.buffer);
        bufferConsole_swap(&gGAMESTATE.buffer);
        if (state < 0) goto end;
        next_game_tick += SKIP_TICKS;
        sleep_time = next_game_tick - GetTickCount();
        if (sleep_time >= 0) Sleep(sleep_time);
    }

    /* close title screen */
    for (state = 1; state > 0; ) {
        state = game_close_title(&gGAMESTATE,1);
        bufferConsole_update(&gGAMESTATE.buffer);
        bufferConsole_swap(&gGAMESTATE.buffer);
        if (state < 0) goto end;
        next_game_tick += SKIP_TICKS;
        sleep_time = next_game_tick - GetTickCount();
        if (sleep_time >= 0) Sleep(sleep_time);
    }

      /* main loop */
    while(1) {
        /* init game loop */
        for (state = 1; state > 0; ) {
            state = game_open_loop(&gGAMESTATE,1);
            bufferConsole_update(&gGAMESTATE.buffer);
            bufferConsole_swap(&gGAMESTATE.buffer);

            next_game_tick += SKIP_TICKS;
            sleep_time = next_game_tick - GetTickCount();
            if (sleep_time >= 0) Sleep(sleep_time);
        }


        /* game loop */
        for (state = 1; state > 0; ) {
            state = game_update_loop(&gGAMESTATE, 1);
            bufferConsole_update(&gGAMESTATE.buffer);
            bufferConsole_swap(&gGAMESTATE.buffer);
            if (state < 0) goto end;

            next_game_tick += SKIP_TICKS;
            sleep_time = next_game_tick - GetTickCount();
            if (sleep_time >= 0) Sleep(sleep_time);
        }


        //PlaySound("data/hit.wav", NULL, SND_ASYNC);
        aSPRITES[1] = sprite_new(aIMAGES+RETRY_LEAVE_ID,
                                        Vec2(30,20), Vec2(96,16), 0);
        aSPRITES[1].xframe_count = 4;
        aSPRITES[1].xframe_speed = 0.3;

        /* gameover loop */
        while (state >= 0) {
            state = game_update_gameover(&gGAMESTATE,1);
            bufferConsole_update(&gGAMESTATE.buffer);
            bufferConsole_swap(&gGAMESTATE.buffer);
            if (state < 0) goto end;
            if (state == 0) break;
            next_game_tick += SKIP_TICKS;
            sleep_time = next_game_tick - GetTickCount();
            if (sleep_time >= 0) Sleep(sleep_time);
        }

    }


    /* good end */
    end:

    /* delete all images */
    img_delete(aIMAGES+TITLE_ID);
    img_delete(aIMAGES+BG1_ID);
    img_delete(aIMAGES+BG2_ID);
    img_delete(aIMAGES+BG3_ID);
    img_delete(aIMAGES+PRESS_SPACE_ID);
    img_delete(aIMAGES+RETRY_LEAVE_ID);
    img_delete(aIMAGES+BIRD_ID);
    img_delete(aIMAGES+FONT_ID);
    img_delete(aIMAGES+SAW_BACK_ID);
    img_delete(aIMAGES+SAW_FRONT_TOP_ID);
    img_delete(aIMAGES+SAW_FRONT_MIDDLE_ID);
    img_delete(aIMAGES+SAW_FRONT_BOTTOM_ID);

    console_cls(gGAMESTATE.buffer.hnd);
    bufferConsole_delete(&gGAMESTATE.buffer);
    return EXIT_SUCCESS;
}


