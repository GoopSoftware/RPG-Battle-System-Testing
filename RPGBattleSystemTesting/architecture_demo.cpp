//#include "raylib.h"
//#include <cmath>
//
//// Draw a labeled box with centered text
//void DrawLabeledBox(Rectangle rect, const char* label, int fontSize = 20) {
//    DrawRectangleLinesEx(rect, 2, BLACK);
//
//    int textWidth = MeasureText(label, fontSize);
//    float textX = rect.x + (rect.width - textWidth) / 2.0f;
//    float textY = rect.y + (rect.height - fontSize) / 2.0f;
//
//    DrawText(label, (int)textX, (int)textY, fontSize, BLACK);
//}
//
//// Draw simple glow pulse behind a box
//void DrawGlow(Rectangle rect, Color color, float intensity) {
//    Rectangle glow = {
//        rect.x - 6, rect.y - 6,
//        rect.width + 12, rect.height + 12
//    };
//
//    Color c = color;
//    c.a = (unsigned char)(80 * intensity);
//    DrawRectangleRounded(glow, 0.15f, 8, c);
//}
//
//int main() {
//    const int screenWidth = 1600;
//    const int screenHeight = 900;
//
//    InitWindow(screenWidth, screenHeight, "RPG Engine Architecture");
//    SetTargetFPS(60);
//
//    while (!WindowShouldClose()) {
//        float t = GetTime();
//        float pulse = 0.5f + 0.5f * sinf(t * 2.0f);
//
//        BeginDrawing();
//        ClearBackground(RAYWHITE);
//
//        // ===== TITLE =====
//        const char* title = "C++ Raylib RPG Engine - Architecture Overview";
//        int titleSize = 32;
//        int titleWidth = MeasureText(title, titleSize);
//        DrawText(title, (screenWidth - titleWidth) / 2, 20, titleSize, BLACK);
//
//        // ===== ENGINE (TOP) =====
//        Rectangle engineBox = { screenWidth / 2.0f - 220, 90, 440, 70 };
//        DrawGlow(engineBox, Color{ 135, 206, 250, 255 }, pulse);
//        DrawLabeledBox(engineBox, "Engine (Game Loop)", 24);
//
//        // ===== GAME STATE MANAGER =====
//        Rectangle gsmBox = { screenWidth / 2.0f - 200, 190, 400, 60 };
//        DrawGlow(gsmBox, Color{ 100, 149, 237, 255 }, 0.3f + 0.7f * pulse);
//        DrawLabeledBox(gsmBox, "GameStateManager", 22);
//
//        // ===== ECS TITLE =====
//        DrawText("ECS", 120, 260, 26, BLACK);
//
//        // ===== COMPONENTS BOX =====
//        Rectangle componentsOuter = { 60, 300, 420, 360 };
//        DrawRectangleRoundedLines(componentsOuter, 0.1f, 12, 2, BLACK);
//        DrawText("Components", (int)componentsOuter.x + 10, (int)componentsOuter.y - 28, 22, BLACK);
//
//        const char* compList[] = {
//            "PositionComponent",
//            "SpriteComponent",
//            "HealthComponent",
//            "CombatStatsComponent",
//            "NameComponent"
//        };
//
//        for (int i = 0; i < 5; i++) {
//            Rectangle box = {
//                componentsOuter.x + 20,
//                componentsOuter.y + 40 + i * 60,
//                380, 40
//            };
//            DrawLabeledBox(box, compList[i], 18);
//        }
//
//        // ===== SYSTEMS BOX =====
//        Rectangle systemsOuter = { 520, 300, 420, 360 };
//        DrawRectangleRoundedLines(systemsOuter, 0.1f, 12, 2, BLACK);
//        DrawText("Systems", (int)systemsOuter.x + 10, (int)systemsOuter.y - 28, 22, BLACK);
//
//        const char* sysList[] = {
//            "RenderSystem",
//            "OverworldSystem",
//            "AnimationSystem",
//            "BattleSystem",
//            "DebugSystem"
//        };
//
//        for (int i = 0; i < 5; i++) {
//            Rectangle box = {
//                systemsOuter.x + 20,
//                systemsOuter.y + 40 + i * 60,
//                380, 40
//            };
//            DrawGlow(box, Color{ 173, 216, 230, 255 }, pulse);
//            DrawLabeledBox(box, sysList[i], 18);
//        }
//
//        // ===== OVERWORLD BOX =====
//        Rectangle overworldOuter = { 1000, 300, 300, 220 };
//        DrawRectangleRoundedLines(overworldOuter, 0.1f, 12, 2, BLACK);
//        DrawText("Overworld", (int)overworldOuter.x + 10, (int)overworldOuter.y - 28, 22, BLACK);
//
//        const char* overworldItems[] = {
//            "OverworldMap",
//            "OverworldMapLoader",
//            "Encounter"
//        };
//
//        for (int i = 0; i < 3; i++) {
//            Rectangle box = {
//                overworldOuter.x + 20,
//                overworldOuter.y + 40 + i * 55,
//                260, 40
//            };
//            DrawLabeledBox(box, overworldItems[i], 18);
//        }
//
//        // ===== MANAGERS BOX =====
//        Rectangle managersOuter = { 1000, 550, 300, 160 };
//        DrawRectangleRoundedLines(managersOuter, 0.1f, 12, 2, BLACK);
//        DrawText("Managers", (int)managersOuter.x + 10, (int)managersOuter.y - 28, 22, BLACK);
//
//        Rectangle texBox = { managersOuter.x + 20, managersOuter.y + 40, 260, 40 };
//        Rectangle tagsBox = { managersOuter.x + 20, managersOuter.y + 100, 260, 40 };
//
//        DrawLabeledBox(texBox, "TextureManager", 18);
//        DrawLabeledBox(tagsBox, "Tags", 18);
//
//        // ===== ARROWS =====
//
//        // Engine ? GameStateManager
//        DrawLineEx(
//            { engineBox.x + engineBox.width / 2, engineBox.y + engineBox.height },
//            { gsmBox.x + gsmBox.width / 2, gsmBox.y },
//            3, BLACK
//        );
//
//        //// GameStateManager ? ECS (systems)
//        //DrawLineBezier(
//        //    { gsmBox.x - 10, gsmBox.y + gsmBox.height / 2 },
//        //    { systemsOuter.x + systemsOuter.width, systemsOuter.y },
//        //    3, BLACK
//        //);
//
//        //// GameStateManager ? Overworld
//        //DrawLineBezier(
//        //    { gsmBox.x + gsmBox.width + 10, gsmBox.y + gsmBox.height / 2 },
//        //    { overworldOuter.x, overworldOuter.y },
//        //    3, BLACK
//        //);
//
//        EndDrawing();
//    }
//
//    CloseWindow();
//    return 0;
//}
