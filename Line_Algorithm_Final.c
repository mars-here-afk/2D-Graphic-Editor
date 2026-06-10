#include <stdio.h>
#include <stdlib.h>

#define ROWS 30
#define COLS 30
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef enum {
    LINE,
    RECTANGLE,
    TRIANGLE,
    CIRCLE
}ObjectType;

typedef struct {
    ObjectType type;

    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
}Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

//Functions:

void displayCanvas(){
    printf ("\n");
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            printf ("%c",canvas[i][j]);
        };
        printf ("\n");
    }

}

void clearCanvas(){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            canvas[i][j] = '_';
        }
        
    }
    
}

void putPixel (int x, int y, char ch){
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS){
        canvas[y][x] = ch;
    }
}

//Shapes:

void drawLineCanvas (int x1, int y1, int x2, int y2){
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1){
        putPixel(x1, y1, '*');
        if (x1 == x2 && y1 == y2) break;
        int err2 = err * 2;
        if (err2 > -dy){
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx){
            err += dx;
            y1 += sy;
        }
    }
}

//Rectangle:

void drawRectangleCanvas (int x1, int y1, int x2, int y2){
    drawLineCanvas(x1, y1, x2, y1);
    drawLineCanvas(x2, y1, x2, y2);
    drawLineCanvas(x2, y2, x1, y2);
    drawLineCanvas(x1, y2, x1, y1);
}

//Triangle:

void drawTriangleCanvas (int x1, int y1, int x2, int y2, int x3, int y3){
    drawLineCanvas(x1, y1, x2, y2);
    drawLineCanvas(x2, y2, x3, y3);
    drawLineCanvas(x3, y3, x1, y1);
}

//Circle - Midpoint:

void plotCirclePoints (int xc, int yc, int x, int y, char ch){
    putPixel(xc + x, yc + y, ch);
    putPixel(xc - x, yc + y, ch);
    putPixel(xc + x, yc - y, ch);
    putPixel(xc - x, yc - y, ch);

    putPixel(xc + y, yc + x, ch);
    putPixel(xc - y, yc + x, ch);
    putPixel(xc + y, yc - x, ch);
    putPixel(xc - y, yc - x, ch);
}

//Circle:

void drawCircleCanvas (int xc, int yc, int radius, char ch){
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    plotCirclePoints(xc, yc, x, y, ch);

    while (x < y){
        if (d < 0){
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        plotCirclePoints(xc, yc, x, y, ch);
    }
}

//Draw Object:

void drawObject(Object obj){
    switch (obj.type){
        case LINE:
            drawLineCanvas(obj.x1, obj.y1, obj.x2, obj.y2);
            break;
        
        case RECTANGLE:
            drawRectangleCanvas(obj.x1, obj.y1, obj.x2, obj.y2);
            break;

        case TRIANGLE:
            drawTriangleCanvas(obj.x1, obj.y1, obj.x2, obj.y2, obj.x3, obj.y3);
            break;

        case CIRCLE:
            drawCircleCanvas(obj.x1, obj.y1, obj.radius, '*');
            break;
    }
}

//Rebuild Canvas:

void rebuildCanvas(){
    clearCanvas();
    for (int i = 0; i < objectCount; i++){
        drawObject(objects[i]);
    }
}

//Add Object:

void addObject(){
    if (objectCount >= MAX_OBJECTS){
        printf ("Maximum object limit reached.\n");
        return;
    }

    Object Obj;
    int typeChoice;

    printf ("Select Shape:\n");
    printf ("1. Line\n");
    printf ("2. Rectangle\n");
    printf ("3. Triangle\n");
    printf ("4. Circle\n");
    printf ("Choice: ");
    scanf ("%d", &typeChoice);

    switch (typeChoice){
        case 1:
            Obj.type = LINE;
            printf ("Enter (x1, y1) & (x2, y2): ");
            scanf ("%d %d %d %d", &Obj.x1, &Obj.y1, &Obj.x2, &Obj.y2);
            break;

        case 2:
            Obj.type = RECTANGLE;
            printf ("Enter (x1, y1) & (x2, y2): ");
            scanf ("%d %d %d %d", &Obj.x1, &Obj.y1, &Obj.x2, &Obj.y2);
            break;

        case 3:
            Obj.type = TRIANGLE;
            printf ("Enter (x1, y1), (x2, y2) & (x3, y3): ");
            scanf ("%d %d %d %d %d %d", &Obj.x1, &Obj.y1, &Obj.x2, &Obj.y2, &Obj.x3, &Obj.y3);
            break;

        case 4:
            Obj.type = CIRCLE;
            printf ("Enter center x, y & radius: ");
            scanf ("%d %d %d", &Obj.x1, &Obj.y1, &Obj.radius);
            break;

        default:
            printf ("!Error: Invalid choice!\n");
            return;
    }

    objects[objectCount++] = Obj;
    rebuildCanvas();
}

//Delete Object:

void deleteObject(){
    int id;

    printf ("Object ID to delete (0-%d): ", objectCount - 1);
    scanf ("%d", &id);

    if (id < 0 || id >= objectCount){
        printf ("Invalid Object ID.\n");
        return;
    }

    for (int i = id; i < objectCount - 1; i++){
        objects[i] = objects[i + 1];
    }

    objectCount--;
    rebuildCanvas();
}

//Modify Object:

void modifyObject(){
    int id;

    printf ("Object ID to modify (0-%d): ", objectCount - 1);
    scanf ("%d", &id);

    if (id < 0 || id >= objectCount){
        printf ("Invalid Object ID.\n");
        return;
    }

    Object *obj = &objects[id];

    switch (obj->type){
        case LINE:
            printf ("Enter new (x1, y1) & (x2, y2): ");
            scanf ("%d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2);
            break;

        case RECTANGLE:
            printf ("Enter new (x1, y1) & (x2, y2): ");
            scanf ("%d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2);
            break;

        case TRIANGLE:
            printf ("Enter new (x1, y1), (x2, y2) & (x3, y3): ");
            scanf ("%d %d %d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2, &obj->x3, &obj->y3);
            break;

        case CIRCLE:
            printf ("Enter new center x, y & radius: ");
            scanf ("%d %d %d", &obj->x1, &obj->y1, &obj->radius);
            break;
    }

    rebuildCanvas();
}

//Main Function:

int main(){

    int choice;

    clearCanvas();

    while(1){
        printf (">>>>>>> 2D Graphic Editor <<<<<<<\n");
        printf ("1. Draw an Object\n");
        printf ("2. Delete an Object\n");
        printf ("3. Modify an Object\n");
        printf ("4. Display the Canvas\n");
        printf ("5. Exit 2D Graphic Designer\n");

        printf ("Choice: ");
        scanf ("%d",&choice);

        switch (choice){
            case 1: 
                addObject();
                break;
            
            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;
            
            case 4:
                displayCanvas();
                break;

            case 5:
                return 0;

            default: printf ("!Error: Invalid Choice!\n");
                break;
            }

        }

    return 0;
}