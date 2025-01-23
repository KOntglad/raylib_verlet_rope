/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include "stdlib.h"
#include "raylib.h"
#include "math.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir

typedef struct Box_collision
{
	Vector2 position;
	Vector2 scale;
	int struct_index;

} Box_collision;

typedef struct Point
{
	// Box_collision point_collider;
	Vector2 current_vec;
	Vector2 old_vec;
	bool pinned;

} Point;

typedef struct Stick
{
	Point *Start;
	Point *End;
	float distance;

} Stick;

typedef struct Box
{

	Vector2 position;
	Vector2 scale;

} Box;

Point points[] =
	{
		{{320, 210}, {320, 210}, true},
		{{320, 230}, {320, 230}, false},
		{{320, 250}, {320, 250}, false},

		{{320, 270}, {320, 270}, false},
		{{320, 290}, {300, 290}, false},
		{{320, 310}, {320, 310}, false},

		{{320, 330}, {320, 330}, false},
		{{320, 350}, {320, 350}, false},
		{{320, 370}, {300, 370}, false},

		{{320, 390}, {320, 390}, false},
		{{320, 410}, {350, 410}, false},
};

float point_scale = 10;

Stick *sticks;

int stick_length = 11;

Box_collision* box_colliders;
int box_length = 2;
int box_which_point[] = { 0,10 };

float distance(Point *first, Point *second);
void throwRope(Point *throw, float throw_force, int which);
Vector2 TrigonometricFuncForPoints(Vector2 first, Vector2 second);

void prepareColliders(Box_collision* colliders, int which[],int length);
void collisionStatus(Box_collision *main_colliders, int collider_length);
void changeRopeLength(bool isWayUp, Stick *sticks, int stick_length, float rope_distance_change);
void prepareSticksVerlet(Stick *input_stick, int stick_index, Point *point);
void updatePoints(Point *main_points, int main_points_length, int screen_width, int screen_height);
void updateSticks(Stick *main_sticks, int main_stick_length);
void constrainPoints(Point *main_points, int main_points_length, int screen_width, int screen_height);
void renderPoints(Point *main_points, int main_points_length);
void renderBoxes(Box *main_boxes, int box_length);

float bounce = 0.1;
float gravity = 2;
float friction = 0.999;


int main()
{

	sticks = (Stick *)malloc(stick_length * sizeof(Stick));
	box_colliders = (Box_collision*)malloc(box_length * sizeof(Box_collision));


	double previousTime = GetTime(); // Previous time measure
	double currentTime = 0.0;		 // Current time measure
	double updateDrawTime = 0.0;	 // Update + Draw tim
	double waitTime = 0.0;
	float deltaTime = 0.0f;

	float timeCounter = 0.0f; // Accumulative time counter (seconds)
	float position = 0.0f;	  // Circle position
	bool pause = false;		  // Pause control flag

	int targetFPS = 60;

	int throwForce = 200;

	int width = 640;
	int height = 480;

	prepareSticksVerlet(sticks, 0, points);
	prepareSticksVerlet(sticks, 1, points);
	prepareSticksVerlet(sticks, 2, points);
	prepareSticksVerlet(sticks, 3, points);
	prepareSticksVerlet(sticks, 4, points);
	prepareSticksVerlet(sticks, 5, points);
	prepareSticksVerlet(sticks, 6, points);
	prepareSticksVerlet(sticks, 7, points);
	prepareSticksVerlet(sticks, 8, points);
	prepareSticksVerlet(sticks, 9, points);
	prepareSticksVerlet(sticks, 10, points);

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(width, height, "Verlet Interpolartion");

	SearchAndSetResourceDir("resources");

	// Texture wabbit = LoadTexture("wabbit_alpha.png");

	while (!WindowShouldClose())
	{

		updatePoints(points, 11, width, height);

		constrainPoints(points, 11, width, height);

		for (int i = 0; i < 120; i++)
		{

			updateSticks(sticks, 10);
		}

		PollInputEvents();

		if (IsKeyPressed(KEY_SPACE))
			pause = !pause;

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			throwRope(points, throwForce, 10);
		}
		if (IsKeyPressed(KEY_UP))
			changeRopeLength(true, sticks, 10, 2);
		else if (IsKeyPressed(KEY_DOWN))
			changeRopeLength(false, sticks, 10, 2);
		else if (IsKeyPressed(KEY_LEFT))
			points[10].old_vec.x = points[10].current_vec.x + 10;
		else if (IsKeyPressed(KEY_RIGHT))
			points[10].old_vec.x = points[10].current_vec.x - 10;

		if (IsKeyPressed(KEY_A))
		{
			points[0].current_vec.x -= 10;
			points[0].old_vec = points[0].current_vec;
		}
		else if (IsKeyPressed(KEY_D))
		{
			points[0].current_vec.x += 10;
			points[0].old_vec = points[0].current_vec;
		}
		else if (IsKeyPressed(KEY_W))
		{
			points[0].current_vec.y -= 10;
			points[0].old_vec = points[0].current_vec;
		}
		else if (IsKeyPressed(KEY_S))
		{
			points[0].current_vec.y += 10;
			points[0].old_vec = points[0].current_vec;
		}

		if (IsKeyPressed(KEY_R))
			targetFPS += 6;
		else if (IsKeyPressed(KEY_E))
			targetFPS -= 6;

		if (IsKeyPressed(KEY_Z))
			throwForce -= 5;
		else if (IsKeyPressed(KEY_X))
			throwForce += 5;

		if (targetFPS < 0)
			targetFPS = 0;

		if (!pause)
		{
			timeCounter += deltaTime;
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("Verlet Interpolartion", 400, 20, 20, BLUE);
		DrawText(TextFormat("Target FPS: %d", targetFPS), 400, 50, 5, BLUE);
		DrawText(TextFormat("Throw Force: %d", throwForce), 400, 70, 5, BROWN);
		for (size_t i = 0; i < 11; i++)
		{
			DrawText(TextFormat("points x: %f", points[i].current_vec.x), 35, 10 + (i * 30), 10, RED);
			DrawText(TextFormat("points y: %f", points[i].current_vec.y), 35, 20 + (i * 30), 10, RED);
			DrawText(TextFormat("point pinned: %f", points[i].pinned), 35, 30 + (i * 30), 10, RED);
		}

		// DrawTexture(wabbit, 400, 200, WHITE);

		renderPoints(points, 11);
		// renderBoxes(Boexs, box_length);

		EndDrawing();

		currentTime = GetTime();
		updateDrawTime = currentTime - previousTime;

		if (targetFPS > 0) // We want a fixed frame rate
		{
			waitTime = (1.0f / (float)targetFPS) - updateDrawTime;
			if (waitTime > 0.0)
			{
				WaitTime((float)waitTime);
				currentTime = GetTime();
				deltaTime = (float)(currentTime - previousTime);
			}
		}
		else
			deltaTime = (float)updateDrawTime;

		previousTime = currentTime;
	}

	// UnloadTexture(wabbit);

	CloseWindow();
	return 0;
}

void prepareSticksVerlet(Stick *input_stick, int stick_index, Point *point)
{

	Stick *now_stick = input_stick + stick_index;
	Point *start_point = point + stick_index;
	Point *end_point = point + (stick_index + 1);

	if (end_point == NULL)
		return;

	else
	{
		now_stick->Start = start_point;
		now_stick->End = end_point;
		now_stick->distance = distance(now_stick->Start, now_stick->End);
	}
}

void updatePoints(Point *main_points, int main_points_length, int screen_width, int screen_height)
{
	int i = 0;
	int dif_x = 0;
	int dif_y = 0;

	for (i = 0; i < main_points_length; i++)
	{
		Point *ex_point = main_points + i;
		if (ex_point->pinned == false)
		{
			dif_x = (ex_point->current_vec.x - ex_point->old_vec.x) * friction;
			dif_y = (ex_point->current_vec.y - ex_point->old_vec.y) * friction;

			ex_point->old_vec.x = ex_point->current_vec.x;
			ex_point->old_vec.y = ex_point->current_vec.y;

			ex_point->current_vec.x += dif_x;
			ex_point->current_vec.y += dif_y;
			ex_point->current_vec.y += gravity;
		}
	}
}

void renderPoints(Point *main_points, int main_points_length)
{
	int i = 0;
	for (i = 0; i < main_points_length; i++)
	{
		Point *ex_point = main_points + i;
		DrawCircleV(ex_point->current_vec, 10, RED);
	}
}

void constrainPoints(Point *main_points, int main_points_length, int screen_width, int screen_height)
{
	int i = 0;
	int dif_x, dif_y;
	for (i = 0; i < main_points_length; i++)
	{
		Point *ex_point = main_points + i;
		if (ex_point->pinned == false)
		{

			dif_x = (ex_point->current_vec.x - ex_point->old_vec.x) * friction;
			dif_y = (ex_point->current_vec.y - ex_point->old_vec.y) * friction;

			if (ex_point->current_vec.x > screen_width)
			{
				ex_point->current_vec.x = screen_width;
				ex_point->old_vec.x = ex_point->current_vec.x + dif_x * bounce;
			}
			else if (ex_point->current_vec.x < 0)
			{
				ex_point->current_vec.x = 0;
				ex_point->old_vec.x = ex_point->current_vec.x + dif_x * bounce;
			}
			if (ex_point->current_vec.y > screen_height)
			{
				ex_point->current_vec.y = screen_height;
				ex_point->old_vec.y = ex_point->current_vec.y + dif_y * bounce;
			}
			else if (ex_point->current_vec.y < 0)
			{
				ex_point->current_vec.y = 0;
				ex_point->old_vec.y = ex_point->current_vec.y + dif_y * bounce;
			}
		}
	}
}

void updateSticks(Stick *sticks, int sticks_length)
{
	int i;
	for (i = 0; i < sticks_length; i++)
	{
		Stick *now_stick = sticks + i;
		float dif_x = now_stick->End->current_vec.x - now_stick->Start->current_vec.x;
		float dif_y = now_stick->End->current_vec.y - now_stick->Start->current_vec.y;
		float distance = sqrtf(dif_x * dif_x + dif_y * dif_y);
		float difference = now_stick->distance - distance;
		float percent = difference / distance / 2;
		float offsetX = dif_x * percent;
		float offsetY = dif_y * percent;

		if (now_stick->Start->pinned == false)
		{
			now_stick->Start->current_vec.x -= offsetX;
			now_stick->Start->current_vec.y -= offsetY;
		}

		if (now_stick->End->pinned == false)
		{
			now_stick->End->current_vec.x += offsetX;
			now_stick->End->current_vec.y += offsetY;
		}
	}
}

float distance(Point *first, Point *second)
{
	float dx = second->current_vec.x - first->current_vec.x;
	float dy = second->current_vec.y - first->current_vec.y;
	return sqrtf(dx * dx + dy * dy);
}

Vector2 TrigonometricFuncForPoints(Vector2 first, Vector2 second)
{
	float _x = first.x - second.x;
	float _y = first.y - second.y;

	float _hip_sq = (_x * _x) + (_y * _y);

	float cos = _x / sqrtf(_hip_sq);
	float sin = _y / sqrtf(_hip_sq);

	Vector2 returnvector = {cos, sin};

	return returnvector;

	// Instantiate(refrance_game_object_circle,object_points[0].point.transform.position - new Vector3(_cos,_sin,1f),refrance_game_object_circle.transform.rotation);
}

void throwRope(Point *throw, float throw_force, int which)
{
	Point *throw_point = throw +which;
	Vector2 vec_mouse = GetMousePosition();
	Vector2 vec_cos_sin = TrigonometricFuncForPoints(throw_point->current_vec, vec_mouse);
	Vector2 throw_vec = {(vec_cos_sin.x * throw_force) + throw_point->old_vec.x, (vec_cos_sin.y * throw_force) + throw_point->old_vec.y};
	throw_point->old_vec = throw_vec;
}

void changeRopeLength(bool isWayUp, Stick *sticks, int stick_length, float rope_distance_change)
{
	int i;

	for (i = 0; i < stick_length; i++)
	{
		Stick *now_stick = sticks + i;

		if (now_stick != NULL)
		{

			Vector2 trigonometric_vec = TrigonometricFuncForPoints(now_stick->Start->current_vec, now_stick->End->current_vec);

			if (isWayUp)
			{
				now_stick->Start->current_vec.x -= trigonometric_vec.x * rope_distance_change;
				now_stick->Start->current_vec.y -= trigonometric_vec.y * rope_distance_change;
			}

			else
			{
				now_stick->Start->current_vec.x += trigonometric_vec.x * rope_distance_change;
				now_stick->Start->current_vec.y += trigonometric_vec.y * rope_distance_change;
			}

			now_stick->distance = distance(now_stick->Start, now_stick->End);

			now_stick->Start->current_vec.x = now_stick->Start->old_vec.x;
			now_stick->Start->current_vec.y = now_stick->Start->old_vec.y;
		}

		else
		{
			return;
		}
	}
}

void renderBoxes(Box *main_boxes, int box_length)
{

	int i;

	for (i = 0; i < box_length; i++)
	{
		Box *now_box = main_boxes + i;
		DrawRectangle(now_box->position.x, now_box->position.y, now_box->scale.x, now_box->scale.y, RED);
	}
}

void prepareColliders(Box_collision* colliders, int which[],int length)
{
	int i = 0;
	for(int i = 0;i < length;i++)
	{
		Box_collision* i_collider = colliders + i;
		i_collider->struct_index = which[i];
	}


}

void colliderUpdate(Box_collision* colliders, Vector2 vectors, int which)
{
	Box_collision* update_collider = colliders + which;
	update_collider->position = vectors;

}


/*void collisionStatus(Box_collision *main_colliders, int collider_length)
{
	int i, j;
	for (i = 0; i < collider_length; i++)
	{
		Box_collision *i_collider = main_colliders + i;
		for (j = 0; i < collider_length; i++)
		{
			Box_collision *j_collider = main_colliders + i;
			if ()
		}
	}
}*/