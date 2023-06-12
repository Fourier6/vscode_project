/*
 * nosaydie_imgshow.c
 *
 *  Created on: 2021年11月13日
 *      Author: Administrator
 */
#include "nosaydie_imgshow.h"
#include "headfile.h"
#include "common.h"
//显示边界、拐点及相关信息
//  全局
extern uint8 Image_H;
extern uint8 Image_W;
//  边线
extern struct LEFT_EDGE L_edge[150];
extern struct RIGHT_EDGE R_edge[150];
extern uint8 L_edge_count;
extern uint8 R_edge_count;
extern uint8 left_findflag;
extern uint8 right_findflag;
//  拐点
extern uint8 L_corner_row;
extern uint8 L_corner_col;
extern uint8 L_corner_flag;
extern uint8 R_corner_row;
extern uint8 R_corner_col;
extern uint8 R_corner_flag;
//  中线
extern struct LEFT_EDGE *Mid_Line;
extern uint8 Mid_count;
//  控制
extern int16 speedl;
extern int16 speedr;
extern struct PID Left_ctr;
extern struct PID Right_ctr;
extern int size1;
extern int x1,x2,x3,x4;
extern float testx;
void draw_line()
{
        //调试用，用于屏幕显示相关参数
        lcd_showuint8(0,1,left_findflag);
        lcd_showuint8(0,2,right_findflag);
        lcd_showint16(0,3,x1);
        lcd_showfloat(0,4,testx,3,3);
//        lcd_showint16(0,4,x4);
//        lcd_showint16(0,1,L_edge_count);
//        lcd_showint16(0,2,R_edge_count);
        //lcd_showint8(0,2,size2);
        //lcd_showint16(0,3,(int16)Left_ctr.currspeed);
        //lcd_showint16(0,4,(int16)Right_ctr.currspeed);
        if(left_findflag)  //如果左边界线存在，则画出左边界线
        {
            for(int i = 0;i<L_edge_count;i++)
            {
                lcd_drawpoint(L_edge[i].col,L_edge[i].row,BLUE);
            }
        }
        if(right_findflag)  //如果右边界线存在，则画出右边界线
        {
            for(int i = 0;i<R_edge_count;i++)
            {
                lcd_drawpoint(R_edge[i].col,R_edge[i].row,RED);
            }
        }
        if(Mid_count>0)
        {
            for(int i=0;i<Mid_count;i++)
                lcd_drawpoint(Mid_Line[i].col,Mid_Line[i].row,BLACK);
        }
        if(L_corner_flag)   //画出左拐点
        {
            if(L_corner_col-2>0 && L_corner_row-2>0&& L_corner_row+2<Image_W && L_corner_col+2<Image_W)
            {
                lcd_drawpoint(L_corner_col-2,L_corner_row-2,RED);
                lcd_drawpoint(L_corner_col-1,L_corner_row-2,RED);
                lcd_drawpoint(L_corner_col,L_corner_row-2,RED);
                lcd_drawpoint(L_corner_col+1,L_corner_row-2,RED);
                lcd_drawpoint(L_corner_col+2,L_corner_row-2,RED);
                lcd_drawpoint(L_corner_col+2,L_corner_row+2,RED);
                lcd_drawpoint(L_corner_col+1,L_corner_row+2,RED);
                lcd_drawpoint(L_corner_col,L_corner_row+2,RED);
                lcd_drawpoint(L_corner_col-1,L_corner_row+2,RED);
                lcd_drawpoint(L_corner_col-2,L_corner_row+2,RED);
                lcd_drawpoint(L_corner_col-2,L_corner_row+1,RED);
                lcd_drawpoint(L_corner_col-2,L_corner_row-1,RED);
                lcd_drawpoint(L_corner_col-2,L_corner_row,RED);
                lcd_drawpoint(L_corner_col+2,L_corner_row-1,RED);
                lcd_drawpoint(L_corner_col+2,L_corner_row,RED);
                lcd_drawpoint(L_corner_col+2,L_corner_row+1,RED);
            }
        }
        if(R_corner_flag)   //画出右拐点
        {
            if(R_corner_col-2>0 && R_corner_row-2>0 && R_corner_row+2<Image_W && R_corner_col+2<Image_W)
            {
                lcd_drawpoint(R_corner_col-2,R_corner_row-2,BLUE);
                lcd_drawpoint(R_corner_col-1,R_corner_row-2,BLUE);
                lcd_drawpoint(R_corner_col,R_corner_row-2,BLUE);
                lcd_drawpoint(R_corner_col+1,R_corner_row-2,BLUE);
                lcd_drawpoint(R_corner_col+2,R_corner_row-2,BLUE);
                lcd_drawpoint(R_corner_col+2,R_corner_row+2,BLUE);
                lcd_drawpoint(R_corner_col+1,R_corner_row+2,BLUE);
                lcd_drawpoint(R_corner_col,R_corner_row+2,BLUE);
                lcd_drawpoint(R_corner_col-1,R_corner_row+2,BLUE);
                lcd_drawpoint(R_corner_col-2,R_corner_row+2,BLUE);
                lcd_drawpoint(R_corner_col-2,R_corner_row+1,BLUE);
                lcd_drawpoint(R_corner_col-2,R_corner_row-1,BLUE);
                lcd_drawpoint(R_corner_col-2,R_corner_row,BLUE);
                lcd_drawpoint(R_corner_col+2,R_corner_row-1,BLUE);
                lcd_drawpoint(R_corner_col+2,R_corner_row,BLUE);
                lcd_drawpoint(R_corner_col+2,R_corner_row+1,BLUE);
            }
        }

}


