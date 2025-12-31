#include <windows.h>
#include <d2d1.h>
#include <cmath>
#include <random>
#include <vector>
#include "Globals.hh"
#include "Utils.hh"
#include <iostream>
#define GetKey(key) GetAsyncKeyState(key) & 0x8000

bool isSpaceReleased;
bool isGrounded;
D2D1_POINT_2F velocity{};
D2D1_POINT_2F movement{};
D2D1_POINT_2F broadPhaseBoxPos{};
int direction{0};
float gravity;
float lerpSpeed{10.0f};
float collisionTime{1.0f};
float normalx{0.0f};
float normaly{0.0f};
float cameraShakeTimeX{0.0f};
float cameraShakeTimeY{0.0f};

void gameStart()
{
    g_player = new Player();

    HANDLE hFile{CreateFile(L"level.lvl", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)};
    char buffer[100]{};
    DWORD bytesRead{};

    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"Error trying to open level.lvl file!", L"Error", MB_ICONERROR | MB_OK);
        PostQuitMessage(1);
    }

    BOOL result{ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL)};

    CloseHandle(hFile);
    if (result)
    {
        char *charRead{buffer};
        char value[16]{};
        int i{0};
        int valueIndex{0};
        float values[4];
        while (*charRead != '\0')
        {
            if (*charRead == '\n')
            {
                g_obstacles.push_back({{values[0], values[1]}, {values[2], values[3]}});
                for (int i{0}; i < 16; value[i++] = 0)
                    ;
                i = 0;
                valueIndex = 0;
                charRead++;
                continue;
            }

            if (*charRead == ',')
            {
                values[valueIndex] = std::atof(value);

                i++;
                valueIndex++;
                for (int i{0}; i < 16; value[i++] = 0)
                    ;

                if (valueIndex >= 4)
                    valueIndex = 0;
            }
            else
                value[i] = *charRead;

            charRead++;
        }
    }
    else
    {
        MessageBox(NULL, L"Error trying to read level.lvl file!", L"Error", MB_ICONERROR | MB_OK);
        PostQuitMessage(1);
    }
}

void gameMain()
{
    if (GetKey('A'))
    {
        movement.x = Utils::Lerp(movement.x, -130.0f, lerpSpeed * g_deltaT);
        direction = -1;
    }
    else if (GetKey('D'))
    {
        movement.x = Utils::Lerp(movement.x, 130.0f, lerpSpeed * g_deltaT);
        direction = 1;
    }
    else
    {
        movement.x = Utils::Lerp(movement.x, 0.0f, lerpSpeed * g_deltaT);
        direction = 0;
    }

    movement.y = gravity;
    velocity = movement;
    velocity.x *= g_deltaT;
    velocity.y *= g_deltaT;
    gravity += 9.81f * 40 * g_deltaT;

    const D2D1_POINT_2F &playerPos{g_player->GetPosition()};
    broadPhaseBoxPos = playerPos;
    broadPhaseBoxPos.x -= 30;
    broadPhaseBoxPos.y -= 30;

    collisionTime = 1.0f;
    normalx = 0.0f;
    normaly = 0.0f;

    for (int i{0}; i < g_obstacles.size(); i++)
    {
        const D2D1_POINT_2F &obstaclePos{g_obstacles[i].GetPosition()};
        const D2D1_POINT_2F &obstacleSize{g_obstacles[i].GetSize()};

        if (!(broadPhaseBoxPos.x + 80 < obstaclePos.x || broadPhaseBoxPos.x > obstaclePos.x + obstacleSize.x ||
              broadPhaseBoxPos.y + 80 < obstaclePos.y || broadPhaseBoxPos.y > obstaclePos.y + obstacleSize.y))
        {
            const float result{Utils::SweptAABB(playerPos, obstaclePos, obstacleSize, velocity, normalx, normaly)};
            if (result < collisionTime)
                collisionTime = result;
        }
    }
    g_player->AddPosition(movement.x * collisionTime * g_deltaT, movement.y * collisionTime * g_deltaT);

    if (normalx != 0.0f)
        movement.x = 0.0f;
    if (normaly != 0.0f)
    {
        gravity = 1.0f;
        if (normaly == -1.0f)
            isGrounded = true;
    }
    else
        isGrounded = false;

    const float remainingTime{1.0f - collisionTime};
    const float dotprod{(velocity.x * normaly + velocity.y * normalx) * remainingTime};

    if (!(normalx != 0 && normaly != 0))
        g_player->AddPosition(dotprod * normaly, dotprod * normalx);

    lerpSpeed = isGrounded ? 10.0f : 5.0f;

    if (GetKey(VK_SPACE))
    {
        if (isSpaceReleased)
        {
            if (isGrounded)
                gravity = -150.0f;
            else if (normalx != 0)
            {
                gravity -= 150.0f;
                if (gravity < -150.0f)
                    gravity = -150.0f;
                movement.x = -200 * direction;
            }
        }

        isSpaceReleased = false;
    }
    else
        isSpaceReleased = true;

    if (playerPos.y >= 720.0f)
    {
        g_player->SetPosition(0.0f, 0.0f);
        gravity = 0.0f;
        cameraShakeTimeX = static_cast<float>(std::rand() % 16 + 4);
        cameraShakeTimeY = static_cast<float>(std::rand() % 16 + 4);
    }

    g_cameraPos.x = std::sinf(cameraShakeTimeX) * 8.0f;
    g_cameraPos.y = std::cosf(cameraShakeTimeY) * 8.0f;

    cameraShakeTimeX = Utils::Lerp(cameraShakeTimeX, 0, 10.0f * g_deltaT);
    cameraShakeTimeY = Utils::Lerp(cameraShakeTimeY, 0, 10.0f * g_deltaT);
}