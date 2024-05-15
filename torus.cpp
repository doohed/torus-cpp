#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>

const int WIDTH = 80;
const int HEIGHT = 24;
const char BACKGROUND_ASCII = ' ';
const float TORUS_RADIUS1 = 1.0f;
const float TORUS_RADIUS2 = 2.0f;
const float DISTANCE = 5.0f;

int main() {
    std::vector<std::vector<char>> buffer(HEIGHT, std::vector<char>(WIDTH, BACKGROUND_ASCII));
    std::vector<std::vector<float>> zbuffer(HEIGHT, std::vector<float>(WIDTH, -1.0f));

    float theta_spacing = 0.07f;
    float phi_spacing = 0.02f;

    float a = 0.0f;
    float b = 0.0f;

    while (true) {
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                buffer[i][j] = BACKGROUND_ASCII;
                zbuffer[i][j] = -1.0f;
            }
        }

        for (float theta = 0.0f; theta < 2.0f * M_PI; theta += theta_spacing) {
            for (float phi = 0.0f; phi < 2.0f * M_PI; phi += phi_spacing) {
                float cos_a = cos(a);
                float sin_a = sin(a);
                float cos_b = cos(b);
                float sin_b = sin(b);
                float cos_theta = cos(theta);
                float sin_theta = sin(theta);
                float cos_phi = cos(phi);
                float sin_phi = sin(phi);

                float circle_x = TORUS_RADIUS2 + TORUS_RADIUS1 * cos_theta;
                float circle_y = TORUS_RADIUS1 * sin_theta;

                float x = circle_x * (cos_b * cos_phi + sin_a * sin_b * sin_phi) - circle_y * cos_a * sin_b;
                float y = circle_x * (sin_b * cos_phi - sin_a * cos_b * sin_phi) + circle_y * cos_a * cos_b;
                float z = DISTANCE + cos_a * circle_x * sin_phi + circle_y * sin_a;
                float ooz = 1.0f / z;

                int xp = static_cast<int>(WIDTH / 2.0f + (WIDTH / 4.0f * ooz * x));
                int yp = static_cast<int>(HEIGHT / 2.0f - (HEIGHT / 2.0f * ooz * y));

                float luminance = (cos_phi * cos_theta * sin_b - cos_a * cos_theta * sin_phi - sin_a * sin_theta + cos_b * (cos_a * sin_theta - cos_theta * sin_a * sin_phi));
                int luminance_index = static_cast<int>(8.0f * luminance);

                if (luminance_index >= 0 && luminance_index < 12) {
                    if (ooz > zbuffer[yp][xp]) {
                        zbuffer[yp][xp] = ooz;
                        buffer[yp][xp] = ".,-~:;=!*#$@"[luminance_index];
                    }
                }
            }
        }

        // Clear the screen and print the buffer
        std::cout << "\x1B[2J\x1B[H";
        for (const auto& row : buffer) {
            for (char c : row) {
                std::cout << c;
            }
            std::cout << '\n';
        }

        a += 0.04f;
        b += 0.02f;

        std::this_thread::sleep_for(std::chrono::milliseconds(35));
    }

    return 0;
}

