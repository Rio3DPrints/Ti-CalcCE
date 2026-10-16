#include <ti/getcsc.h>
#include <ti/screen.h>
#include <graphx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Custom 16-bit RGB565 Color Definitions
#define COLOR_DARK_GREEN  0x0320  // Top status bar
#define COLOR_LIGHT_GREEN 0x2464  // Main background body
#define COLOR_PALE_GREEN  0x36C8  // Faint placeholder outlines
#define COLOR_WHITE       0xFFFF  // Crisp white text
#define COLOR_ACCENT      0x5FE0  // Bright highlight green for selection
#define COLOR_DARK_BG     0x1102  // Inside input boxes

// Menu Items
#define NUM_ITEMS 5
const char* math_topics[NUM_ITEMS] = {
    "1. Simple Solver",
    "2. Fractions & Decimals",
    "3. Proportions & Ratios",
    "4. Linear Equations (y=mx+b)",
    "5. Geometry & Area"
};

void draw_status_bar(const char* title, int battery_pct) {
    // 1. Draw Dark Green Top Bar (22px tall)
    gfx_SetColor(COLOR_DARK_GREEN);
    gfx_FillRectangle(0, 0, 320, 22);
    
    // 2. Draw App Title Text
    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_PrintStringXY(title, 6, 6);
    
    // 3. Draw Battery Outline & Fill on the right side
    int bx = 285, by = 6, bw = 26, bh = 10;
    gfx_SetColor(COLOR_WHITE);
    gfx_Rectangle(bx, by, bw, bh);
    gfx_FillRectangle(bx + bw, by + 3, 2, 4); // Battery tip
    
    int fill_w = (bw - 4) * battery_pct / 100;
    gfx_FillRectangle(bx + 2, by + 2, fill_w, bh - 4);
}

void draw_math_menu(int selected_index) {
    // Fill background with Light Green
    gfx_SetColor(COLOR_LIGHT_GREEN);
    gfx_FillRectangle(0, 22, 320, 218);
    
    // Subtitle
    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_PrintStringXY("Select a Tool (Press [ENTER] to open):", 20, 28);
    
    // Draw 5 Menu Options inside clean outline boxes
    int start_y = 50;
    int box_w = 280;
    int box_h = 30;
    int spacing = 36;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        int y = start_y + i * spacing;
        
        if (i == selected_index) {
            gfx_SetColor(COLOR_ACCENT); // Brighter highlight box
            gfx_Rectangle(20, y, box_w, box_h);
            gfx_Rectangle(21, y + 1, box_w - 2, box_h - 2); // Thicker border
        } else {
            gfx_SetColor(COLOR_PALE_GREEN); // Faint outline box
            gfx_Rectangle(20, y, box_w, box_h);
        }
        
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY(math_topics[i], 32, y + 9);
    }
}

// --- Sub-Screen: Simple Solver ---
void run_simple_solver(void) {
    double num1 = 0, num2 = 0, result = 0;
    char buf1[16] = "0";
    char buf2[16] = "0";
    int active_field = 0; // 0 for num1, 1 for num2
    
    while(1) {
        uint8_t key = os_GetCSC();
        if (key == sk_Clear || key == sk_Del) break;
        
        // Switch fields with Up/Down
        if (key == sk_Down || key == sk_Up) {
            active_field = 1 - active_field;
        }
        
        // Simple mock number input handler for demonstration
        if (key >= sk_0 && key <= sk_9) {
            char digit = '0' + (key - sk_0);
            if (active_field == 0 && strlen(buf1) < 10) {
                if (strcmp(buf1, "0") == 0) buf1[0] = '\0';
                int len = strlen(buf1);
                buf1[len] = digit;
                buf1[len+1] = '\0';
            } else if (active_field == 1 && strlen(buf2) < 10) {
                if (strcmp(buf2, "0") == 0) buf2[0] = '\0';
                int len = strlen(buf2);
                buf2[len] = digit;
                buf2[len+1] = '\0';
            }
        }
        
        // Calculate on [Enter]
        if (key == sk_Enter) {
            num1 = atof(buf1);
            num2 = atof(buf2);
            result = num1 + num2; // Example: Addition solver
        }

        // Render Simple Solver Screen
        gfx_FillScreen(COLOR_LIGHT_GREEN);
        draw_status_bar("Simple Solver", 90);
        
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY("Enter two numbers to add:", 20, 35);
        
        // Field 1 Box
        gfx_SetColor(active_field == 0 ? COLOR_ACCENT : COLOR_PALE_GREEN);
        gfx_Rectangle(20, 65, 280, 35);
        gfx_PrintStringXY("Number 1: ", 30, 77);
        gfx_PrintStringXY(buf1, 120, 77);
        
        // Field 2 Box
        gfx_SetColor(active_field == 1 ? COLOR_ACCENT : COLOR_PALE_GREEN);
        gfx_Rectangle(20, 110, 280, 35);
        gfx_PrintStringXY("Number 2: ", 30, 122);
        gfx_PrintStringXY(buf2, 120, 122);
        
        // Results Area
        gfx_SetColor(COLOR_DARK_BG);
        gfx_FillRectangle(20, 160, 280, 50);
        gfx_SetColor(COLOR_PALE_GREEN);
        gfx_Rectangle(20, 160, 280, 50);
        
        char res_str[32];
        sprintf(res_str, "Sum Result = %.2f", result);
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY(res_str, 35, 178);
        gfx_PrintStringXY("([ENTER]=Calculate, [CLEAR]=Back)", 30, 220);
        
        gfx_SwapDraw();
    }
}

// --- Generic Sub-Screen for other tools ---
void run_placeholder_tool(const char* tool_title) {
    while(1) {
        uint8_t key = os_GetCSC();
        if (key == sk_Clear) break;
        
        gfx_FillScreen(COLOR_LIGHT_GREEN);
        draw_status_bar(tool_title, 90);
        
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY("Tool under construction...", 40, 70);
        gfx_PrintStringXY("Press [CLEAR] to return to menu.", 40, 100);
        
        gfx_SwapDraw();
    }
}

int main(void) {
    int current_selection = 0;
    
    gfx_Begin();
    
    while (1) {
        uint8_t key = os_GetCSC();
        if (key == sk_Clear) break;
        
        if (key == sk_Down && current_selection < NUM_ITEMS - 1) {
            current_selection++;
        }
        if (key == sk_Up && current_selection > 0) {
            current_selection--;
        }
        
        // Launch selected tool on [Enter]
        if (key == sk_Enter) {
            if (current_selection == 0) {
                run_simple_solver();
            } else if (current_selection == 1) {
                run_placeholder_tool("Fractions & Decimals");
            } else if (current_selection == 2) {
                run_placeholder_tool("Proportions & Ratios");
            } else if (current_selection == 3) {
                run_placeholder_tool("Linear Equations");
            } else if (current_selection == 4) {
                run_placeholder_tool("Geometry & Area");
            }
        }
        
        // Render Main Menu Screen
        gfx_FillScreen(COLOR_LIGHT_GREEN);
        draw_math_menu(current_selection);
        draw_status_bar("7th/8th Math Solver", 90);
        
        gfx_SwapDraw();
    }
    
    gfx_End();
    return 0;
}
