//#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "unistd.h" // unistd.h 為Linux 內建函式庫 不支援windows需自建標頭檔


float A, B, C;
float ooz;//Z軸參數
int xp, yp;//斜面演算
int idx;

float cubewidth = 20;//立方體原始大小設定
int width = 160, height = 44;
float zBuffer[160 * 44]; //(3D z軸)立體深度背景
char buffer[160 * 44];  //(2D x,y軸)平面背景
int backgroundASCIICode = ' ';
int distanceFromCam = 100; //立體距離

float incrementSpeed = 0.6; // 旋轉的幅度大小
float x, y, z;
float K1 = 40; //立方體面大小倍數


float calculateX(int i, int j, int k) { //橫向旋轉 演算法
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
            j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {  //縱向旋轉 演算法
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) +
        k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);

}

float calculateZ(int i, int j, int k) {  //斜向旋轉 演算法
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);

}

void surface(float cubex, float cubey, float cubez, int ch) {
    x = calculateX(cubex, cubey, cubez);
    y = calculateY(cubex, cubey, cubez);
    z = calculateZ(cubex, cubey, cubez) + distanceFromCam;

    ooz = 1 / z; 


    xp = (int)(width / 2 + K1 * ooz * x * 2);//斜X軸建置演算
    yp = (int)(height / 2 + K1 * ooz * y); //斜Y軸

    idx = xp + yp * width; //斜面建置
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz; 
            buffer[idx] = ch;
        }
    }
}
int main() {
    //system("CLS"); //windows
    printf("\xb[2j");  //linux 用語
    while (1) {
        memset(buffer, backgroundASCIICode, width * height); //背景清空
        memset(zBuffer, 0, width * height * 4); //空間深度參數重製
        for (float x = -cubewidth; x < cubewidth; x += incrementSpeed) {
            for (float y = -cubewidth; y < cubewidth; y += incrementSpeed) {
                surface(x, y, -cubewidth, '#');
                surface(cubewidth, y, x, '$');
                surface(-cubewidth, y, -x, '~');
                surface(-x, y, cubewidth, '.');
                surface(x, -cubewidth, -y,';');
                surface(x,cubewidth, y,'-');
            }

        }
        //system("CLS"); //windows
        printf("\x1b[H"); // linux
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10); //輸出圖形
        }
        A += 0.005; //旋轉方向角度更動
        B += 0.005;
    }


    return 0;
}
