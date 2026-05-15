#include "primlib.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define VERTICES 5
#define BASE_RADIUS 200
#define INCREASE_SPEED 0.04
#define INCREASE_SIZE 110
#define ROTATION 0.02
#define ANGLE (2.0 * M_PI / VERTICES)
#define LINE_COLOUR CYAN
#define DELAY 10

float update_angle(float current_angle, float increment)
{
	float new_angle = current_angle + increment;
	if (new_angle >= 2.0 * M_PI) {
		new_angle -= 2.0 * M_PI;
	}
	return new_angle;
}

void draw_polygon(int center_x, int center_y, int radius, float rotation)
{
	for (int i = 1; i <= VERTICES; i++) {
		float prev_angle = ANGLE * (i - 1) + rotation;
		float curr_angle = ANGLE * i + rotation;

		int prev_x = center_x + radius * cos(prev_angle);
		int prev_y = center_y + radius * sin(prev_angle);
		int curr_x = center_x + radius * cos(curr_angle);
		int curr_y = center_y + radius * sin(curr_angle);

		gfx_line(prev_x, prev_y, curr_x, curr_y, LINE_COLOUR);
	}
}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	float scale_phase = 0.0;
	float current_rotation = 0.0;

	do {
		int center_x = gfx_screenWidth() / 2;
		int center_y = gfx_screenHeight() / 2;
		int current_radius = BASE_RADIUS + sin(scale_phase) * INCREASE_SIZE;

		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
					   BLACK);

		draw_polygon(center_x, center_y, current_radius, current_rotation);

		gfx_updateScreen();

		scale_phase = update_angle(scale_phase, INCREASE_SPEED);
		current_rotation = update_angle(current_rotation, ROTATION);

		SDL_Delay(DELAY);
	} while (!gfx_isKeyDown(SDLK_ESCAPE));

	return 0;
}
