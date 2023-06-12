#include "nosaydie_camera.h"
#include "headfile.h"
#include "common.h"
#include "nosaydie_camera_bianliang.h"
//����ȫ�ֱ���-------------------------------------------
//  ͼ��
//      ͼ��Ԥ����
  uint8 threshold;                  //��ֵ����ֵ
  uint8 image[MT9V03X_H][MT9V03X_W];        //ʹ�ö�άָ��image������Ϣͼ�񣬱��ں��ڶ�ֵ��ͼ������ͼ���ת��
  uint8 binimage_flag = 0;          //��ֵ����־��Ĭ��û�ж�ֵ��
  struct LEFT_EDGE L_edge[150];     //��߽�ṹ��
  struct RIGHT_EDGE R_edge[150];    //�ұ߽�ṹ��
  struct LEFT_EDGE M_Line[150];     //���߽ṹ�壬����ѡ��2
  struct LEFT_EDGE Last_M_Line[150];//�ϴ����߽ṹ��
  struct LEFT_EDGE MID_LINE[150];   //���߽ṹ�壬����ѡ��1
  struct LEFT_EDGE *Mid_Line;       //���߽ṹ�塾ʵ��ʹ�á�
//      �������ߴ���
  uint8 searchpoint = 1;            //��������������ߵ��־�����ڽṹ��.flag�����ں��ڴ���
  uint8 nihepoint = 2;              //�������������ߵ��־�����ڽṹ��.flag�����ں��ڴ���
  extern uint8 Road_Width[MT9V03X_H];//�����أ����ڶ���
//      ͼ����ʾ
  uint8 l_display_cnt = 0;
  uint8 r_display_cnt = 0;
/*--------------------------------------------------------------------------
* ���������ܡ���ͼ�������ߡ�Ԫ��ʶ��_����������Խ��
* ����    ��������
* ���� �� ֵ������
*--------------------------------------------------------------------------*/
int x1,x2,x3,x4;
void Image_process()
{
    //�������
    //  ȫ�֡��ⲿ����--------------------------
    //      ȫ�ֱ���
    //      �ⲿ����
    //  �����ڲ�����-------------------
    //      һ��ͼ����
     uint8 Bottom2 = Image_H - 40;   //�����ڶ���
     uint8 max_col = Image_W - 5,min_col = 5;         //���/С�����꣬���߽�ֵ
     uint8 L_search_amount = 150, R_search_amount = 150;  //���ұ߽��ѵ�ʱ�������ĵ�
     uint8 jilu_row_l = 0, jilu_col_l = 0, jilu_row_r = 0, jilu_col_r = 0;  //��¼�������Ļ����߽������ֵ
    //ͼ����-----------------------
    //  һ��ͼ����
    //      ��ʼ����ر���
     enable_check_l_r_edge_same = 0;    //ʹ�ܼ�����ұ����Ƿ����غϣ�Ĭ�ϲ���������������ʼ�����ʱ����
     left_findflag = 0;                 //��߽���ڱ�־��1�ҵ���߽磬0û�ҵ���߽�        Ĭ��û���ҵ���߽�
     right_findflag = 0;                //�ұ߽���ڱ�־��1�ҵ��б߽磬0û�ҵ��ұ߽�        Ĭ��û���ҵ��ұ߽�
     L_basic_row_start = Image_H -2;    //��ʼ���ߵ�
     R_basic_row_start = Image_H -2;    //�ҿ�ʼ���ߵ�
    //      ��ȡͼ���ֵ����ֵ��ͼ���ֵ��
     get_deal_image();
     //average_filter();
     //threshold = OSTU_bin(Image_W,Image_H,mt9v03x_image);//��ȡ��̬��ֵ
     threshold = GetOSTUThreshold(image,0,Image_H-1,0,Image_W-1);
     //get_binImage(threshold);//��ȡ��ֵ��ͼ��
     //      ��ʼ������
     enable_balinyu = 1;
     if(enable_balinyu)//���ʹ�ܰ��������߼�һ�㴦��
     {
         //��ر���
         line_lose_center_left = 0;
         line_lose_center_right = 0;
         line_point_count_left = 0;
         line_point_count_right = 0;
         L_edge_count = 0;//��ߵ������0
         R_edge_count = 0;//�ұߵ������0
         int exist_edge_size = 0;   //�ж��Ƿ������/�ұ߽�
         //Ѱ����\���߿�ʼ�㣬���ж��Ƿ���ڵ�ǰ��
         clear_point();
         exist_edge_size = edge_point_ornot(L_basic_row_start,0);
         if(exist_edge_size >= 0){jilu_row_l = L_basic_row_start;jilu_col_l = exist_edge_size;left_findflag = 1;}
         exist_edge_size = edge_point_ornot(R_basic_row_start,(uint8)(1));
         if(exist_edge_size >= 0){jilu_row_r = R_basic_row_start;jilu_col_r = exist_edge_size;right_findflag = 1;}
         //����������
         if(left_findflag)//�����߽����ڲ��ҵ�,��ʼ����
         {
             //��������
             L_edge[0].row = jilu_row_l;
             L_edge[0].col = jilu_col_l;
             L_edge[0].flag = 1;
             uint8 curr_row = jilu_row_l;//��ʼ��������
             uint8 curr_col = jilu_col_l;//��ʼ��������
             dire_left = 0; //��ʼ���ϸ��߽�������
             center_turn_flag = 1;//��ʼ����δ�궨״̬
             //��ʼ���ߣ����ȡ150���㣬���������ѣ���7����λ
             for(int i = 1;i < L_search_amount; i++)    //�������70����
             {
                 ////Խ���˳� ��Խ�����Խ�磨���������������ң�
                 if(curr_row < L_edge_end_row || curr_row>Image_H-1||curr_row+1<L_edge_end_row)  break;
                 if(curr_col > max_col || curr_col < min_col)
                 {
                     if(++L_search_edge_count == 3)//����3���������߽磬�˳�
                         break;
                 }
                 else
                     L_search_edge_count = 0;
                 //���߹���
                 if(dire_left != 2&&black_(image[curr_row-1][curr_col-1])&&white_(image[curr_row-1][curr_col]))   //���Ϻڣ�2���ұ߰�
                 {
                     curr_row = curr_row -1;
                     curr_col = curr_col -1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 7;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=3&&black_(image[curr_row-1][curr_col+1])&&white_(image[curr_row][curr_col+1]))    //���Ϻڣ�3���±߰�
                 {
                     curr_row = curr_row -1;
                     curr_col = curr_col + 1;
                     L_edge_count = L_edge_count + 1;
                     dire_left = 6;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(black_(image[curr_row-1][curr_col])&&white_(image[curr_row-1][curr_col+1]))                  //���Ϻڣ�1���Ұ�
                 {
                     curr_row = curr_row - 1;
                     L_edge_count = L_edge_count + 1;
                     dire_left = 0;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=5&&black_(image[curr_row][curr_col-1])&&white_(image[curr_row-1][curr_col-1]))     //����ڣ�5���ϰ�
                 {
                     curr_col = curr_col - 1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 4;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=4&&black_(image[curr_row][curr_col+1])&&white_(image[curr_row+1][curr_col+1]))  //���Һڣ�4���°�
                 {
                     curr_col = curr_col + 1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 5;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=6&&black_(image[curr_row+1][curr_col-1])&&white_(image[curr_row][curr_col-1]))    //���ºڣ�6���ϰ�
                 {
                     curr_row = curr_row + 1;
                     curr_col = curr_col -1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 3;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=7&&black_(image[curr_row+1][curr_col+1])&&white_(image[curr_row+1][curr_col]))    //���ºڣ�7�����
                 {
                     curr_row = curr_row + 1;
                     curr_col = curr_col + 1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 2;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else
                     break;
             }
         }
         if(right_findflag)//����ұ߽���ڲ��ѵ�
         {
             R_edge[0].row = jilu_row_r;
             R_edge[0].col = jilu_col_r;
             R_edge[0].flag = 1;
             uint8 curr_row = jilu_row_r;
             uint8 curr_col = jilu_col_r;
             dire_right = 0;
             for(int i = 1;i<R_search_amount;i++)
             {
                 //Խ���˳� ��Խ�����Խ�磨���������������ң�
                 if(curr_row < L_edge_end_row ||curr_row>Image_H-1||curr_row+1<L_edge_end_row)  break;
                 if(curr_col > max_col || curr_col < min_col)   //���������������߽磬�˳�
                 {
                     if(++R_search_edge_count == 3)
                         break;
                 }
                 else   R_search_edge_count = 0;
                 //���߹���
                 if(curr_col<Image_W&&dire_right!=3&&black_(image[curr_row-1][curr_col+1])&&white_(image[curr_row-1][curr_col]))    //���Ϻڣ�3�����
                 {
                     curr_row = curr_row - 1;
                     curr_col = curr_col + 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 6;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=2&&black_(image[curr_row-1][curr_col-1])&&white_(image[curr_row][curr_col-1])) //���Ϻڣ�2���°�
                 {
                     curr_row = curr_row-1;
                     curr_col = curr_col-1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 7;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(black_(image[curr_row-1][curr_col])&&white_(image[curr_row-1][curr_col-1]))                  //���Ϻڣ�1�����
                 {
                     curr_row = curr_row - 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 0;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=5&&black_(image[curr_row][curr_col-1])&&white_(image[curr_row+1][curr_col-1]))   //����ڣ�5���°�
                 {
                     curr_col = curr_col-1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 4;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }

                 else if(dire_right!=4&&black_(image[curr_row][curr_col+1])&&white_(image[curr_row-1][curr_col+1]))   //���Һڣ�4���ϰ�
                 {
                     curr_col = curr_col + 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 5;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=6&&black_(image[curr_row+1][curr_col-1])&&white_(image[curr_row+1][curr_col]))   //���ºڣ�6���Ұ�
                 {
                     curr_row = curr_row + 1;
                     curr_col = curr_col - 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 3;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=7&&black_(image[curr_row+1][curr_col+1])&&white_(image[curr_row][curr_col+1]))   //���ºڣ�7���ϰ�
                 {
                     curr_row = curr_row + 1;
                     curr_col = curr_col + 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 2;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else
                     break;
             }
         }
         //�߽�Ԥ����
         // ��ر�����ʼ��
         pre_L_edge_count = 0;
         pre_R_edge_count = 0;
         // ��߽紦��
         if(left_findflag)//�����߽���ڲ��ҵ�
         {
             //ƽ���߽�
             for(int i= 1;i<L_edge_count -1;i++)
             {
                 L_edge[i].row = (L_edge[i].row + L_edge[i+1].row)/2;
                 L_edge[i].col = (L_edge[i].col + L_edge[i+1].col)/2;
             }
             for(int i= L_edge_count-1;i>1;i--)
             {
                 L_edge[i].row = (L_edge[i].row + L_edge[i-1].row)/2;
                 L_edge[i].col = (L_edge[i].col + L_edge[i-1].col)/2;
             }
             edge_process_flag = 0; //�����־
             //�ض�����ˮƽ�ı߽�
             if(L_edge_count >=70)
             {
                 num_cnt = 0;//��¼����ˮƽ��ĸ���
                 L_count = L_edge_count/2;
                 while(L_count<L_edge_count)
                 {
                     if(L_edge[L_count].row == L_edge[L_count+1].row)
                         num_cnt = num_cnt +1;
                     else
                         num_cnt = 0;
                     if(num_cnt > 5)//����5����ˮƽ
                         break;
                     L_count = L_count +1;
                 }
                 L_edge_count = L_count;//�ض���5��ˮƽ�㴦
             }
         }
         //�ұ߽紦��
         if(right_findflag)//�ұ߽�������ҵ�ʱ��ʼ����
         {
             //ƽ���߽�
             for(int i = 1;i<R_edge_count-1;i++)
             {
                 R_edge[i].row = (R_edge[i].row + R_edge[i+1].row)/2;
                 R_edge[i].col = (R_edge[i].col + R_edge[i-1].col)/2;
             }
             for(int i = R_edge_count-1;i>1;i--)
             {
                 R_edge[i].row = (R_edge[i].row + R_edge[i+1].row)/2;
                 R_edge[i].col = (R_edge[i].col + R_edge[i-1].col)/2;
             }
             //�ض�����ˮƽ�ı߽�
             if(R_edge_count>=70)   //�߽繻��ʱ�ſ�ʼ����
             {
                 num_cnt = 0;
                 R_count = R_edge_count/2;
                 while(R_count < R_edge_count)
                 {
                     if(R_edge[R_count].row == R_edge[R_count+1].row)
                         num_cnt = num_cnt + 1;
                     else
                         num_cnt = 0;
                     if(num_cnt > 5)
                         break;
                     R_count = R_count + 1;
                 }
                 R_edge_count = R_count;
             }
         }
         //����쳣�߽�
         // ���1�����ұ��ߺ��ͼ�������Width/2���������ұ��������غϵ����⣬ʹ���غϼ���
         if(fabs(L_edge[0].col-L_edge[L_edge_count].col)>Image_W/2&&fabs(L_edge[0].col-L_edge[L_edge_count].col)>Image_W/2)
             enable_check_l_r_edge_same = 1;
         if(enable_check_l_r_edge_same)
         {
             uint8 i_left = L_edge_count - 1;
             uint8 chongdie_cnt = 0;
             for(int i = 0;i<R_edge_count;i++)
             {
                 if(fabs(R_edge[i].row-L_edge[i_left].row)<5&&fabs(R_edge[i].col-L_edge[i_left].col)<5)
                 {
                     chongdie_cnt = chongdie_cnt + 1;
                     i_left = i_left - 1;
                     if(chongdie_cnt > 3) break;
                     if(i_left<0)   break;
                 }
             }
             if(chongdie_cnt>=2)
             {
                 x2 = fabs(L_edge[0].row-R_edge[0].row);
                 //���1�������-���������ұ߽�������ʼ����ֵ���
                 if(fabs(L_edge[0].row-R_edge[0].row)>=15)
                 {
                     if(L_edge[0].row>R_edge[0].row)        //�����ߵ�ߣ���������߶��ߣ��������
                         left_findflag = 0;
                     else if(R_edge[0].row>L_edge[0].row)   //�����ߵ�ߣ������ұ��߶��ߣ��������
                         right_findflag = 0;
                 }
                 //���2����������/�ջ�ʮ�֡��������������޹յ㣨����·�ڱ����غ��йյ㡢�ջ�ʮ���޹յ㣩
                 else if(fabs(L_edge[0].row-R_edge[0].row)<15)
                 {

                 }
             }
         }
         //�߽��̫�٣�ȥ��
         if(L_edge_count <10)
             left_findflag = 0;
         if(R_edge_count <10)
             right_findflag = 0;
         //�������ʼ������ұ���
         if(left_findflag && right_findflag)
         {
             if(jilu_col_l>jilu_col_r)
             {
                 if(jilu_row_l>jilu_row_r)
                     left_findflag = 0;
                 else if(jilu_row_l<jilu_row_r)
                     right_findflag = 0;
             }
         }
         if(L_edge_count-R_edge_count>30&&right_findflag)  //���ұ߽��������̫����>>��
         {
             right_findflag = 0;
             R_edge_count = 0;
         }
         if(R_edge_count-L_edge_count>30&&left_findflag)   //��>>��
         {
             left_findflag = 0;
             L_edge_count = 0;
         }
         //�����/�ұ߽�̫�����ҵ������ϴ�ɾ������
         if(left_findflag&&right_findflag)
         {
             if(jilu_row_r-jilu_row_l>Image_H/2&&L_edge_count-R_edge_count>10)
             {
                 right_findflag = 0;
                 R_edge_count = 0;
             }
             else if(jilu_row_l-jilu_row_r>Image_H/2&&R_edge_count-L_edge_count>10)
             {
                 left_findflag = 0;
                 L_edge_count = 0;
             }
         }
         if(L_edge[L_edge_count-1].row - L_edge[1].row>-10)  //���߼���ˮƽ ��
             left_findflag = 0;
         if(R_edge[R_edge_count-1].row - R_edge[1].row>-10)  //��
             right_findflag = 0;
         //����ĳһ�߽磬�����������
         if(left_findflag || right_findflag)
             enable_midline = 1;
         else
             enable_midline = 0;
         //���յ�
         L_corner_flag = 0;// ��ʼ������
         L_corner_row = 0;
         L_corner_col = 0;
         L_corner_angle = 0;
         if(enable_L_corner) //���ʹ��������յ�
         {
             if(L_edge_count > 9)
             {
                 for(int i = 0; i<L_edge_count-9;i++)
                 {
                     if(L_edge[i+4].row>5)
                     {
                         if((L_edge[i].col - L_edge[i + 4].col) * (L_edge[i + 8].col - L_edge[i + 4].col) +
                            (L_edge[i].row - L_edge[i + 4].row) * (L_edge[i + 8].row - L_edge[i + 4].row) > 0) //����ȷ��Ϊ��ǻ���ֱ�� ������
                         {
                             L_corner_angle = Get_angle(L_edge[i].row, L_edge[i].col, L_edge[i + 4].row, L_edge[i + 4].col, L_edge[i + 8].row, L_edge[i + 8].col); //��Ƕ�
                             if(L_edge[i+4].col>L_edge[i+8].col)    //ȷ���սǳ�����յ�û�г�������
                             {
                                 L_corner_flag = 1;//�쳣�յ�
                                 L_corner_row = L_edge[i+4].row;
                                 L_corner_col = L_edge[i+4].col;
                                 break;
                             }
                         }
                     }
                 }
             }
         }
         R_corner_flag = 0;//��ʼ������
         R_corner_row = 0;
         R_corner_col = 0;
         R_corner_angle = 0;
         if(enable_R_corner)    //���ʹ�������ҹյ�
         {
             if(R_edge_count > 9)
             {
                 for(int i = 0; i<R_edge_count-9;i++)
                 {
                     if(R_edge[i+4].row>5)
                     {
                         if((R_edge[i].col - R_edge[i + 4].col) * (R_edge[i + 8].col - R_edge[i + 4].col) +
                         (R_edge[i].row - R_edge[i + 4].row) * (R_edge[i + 8].row - R_edge[i + 4].row) > 0) //����ȷ��Ϊ��ǻ���ֱ�� ������
                         {
                             R_corner_angle = Get_angle(R_edge[i].row, R_edge[i].col, R_edge[i + 4].row, R_edge[i + 4].col, R_edge[i + 8].row, R_edge[i + 8].col); //��Ƕ�
                             if(R_edge[i+8].col>R_edge[i+4].col)    //ȷ���սǳ�����յ�û�г�������
                             {
                                 R_corner_flag = 1;//�쳣�յ�
                                 R_corner_row = R_edge[i+4].row;
                                 R_corner_col = R_edge[i+4].col;
                                 break;
                             }
                         }
                     }
                 }
             }
         }
         //�յ㴦��
         //���򣨲����ã�������ɾ��

     }
     //���ʹ�����ߴ�����ʼ��������
     if(enable_midline)
         get_mid();
}
/*--------------------------------------------------------------------------
* ���������ܡ�����ȡ����
* ����    ��������
* ���� �� ֵ������
* ����    ע������
*--------------------------------------------------------------------------*/
int size1 = 0;
void get_mid()
{
    size1 = 1;
    //��ر���
    uint8 mid_cnt = 0;      //����������ϣ����������ϣ������ڼ�¼�µ����ߵ����
    uint8 last_mid_count=0; //���ڼ�¼��һ�����ߵ����߸�������������ƽ��
    uint8 up_cnt=15;        //�������������Ƹ���
    //������һ����������
    for(int i = 0;i<Mid_count;i++)
    {
        Last_M_Line[i].row = Mid_Line[i].row;
        Last_M_Line[i].col = Mid_Line[i].col;
    }
    last_mid_count = Mid_count;
    //���1�����Ҿ��ҵ�
    if(left_findflag&&right_findflag)
    {
        size1 = 2;
        //��ʼ�����߸���
        Mid_count=0;
        //��ֹ���߳���
        L_edge[0].row = L_edge[1].row;
        L_edge[0].col = L_edge[1].col;
        R_edge[0].row = R_edge[1].row;
        R_edge[0].col = R_edge[1].col;
        //����߱��ұ��߳�
        if(L_edge_count>=R_edge_count)
        {
            float k = 1.0 * L_edge_count / R_edge_count;
            for(int i = 0;i<R_edge_count;i++)
            {
                Mid_count = Mid_count+1;
                M_Line[i].row = (uint8)((L_edge[(uint8)(k * i)].row + R_edge[i].row)/2);
                M_Line[i].col = (uint8)((L_edge[(uint8)(k * i)].col + R_edge[i].col)/2);
                M_Line[i].flag = searchpoint;
            }
        }
        //�ұ��߱�����߳�
        else if(L_edge_count<R_edge_count)
        {
            float k = 1.0 * R_edge_count / L_edge_count;
            for(int i = 0;i<L_edge_count;i++)
            {
                Mid_count = Mid_count+1;
                M_Line[i].row = (uint8)((L_edge[i].row + R_edge[(uint8)(k * i)].row)/2);
                M_Line[i].col = (uint8)((L_edge[i].col + R_edge[(uint8)(k * i)].col)/2);
                M_Line[i].flag = searchpoint;
            }
        }
    }
    //���2������ȫ����
    //      �ұ߽綪�ߣ���߽粻����||������������
    else if((left_findflag==1&&right_findflag==0)||(left_findflag&&L_edge_count-R_edge_count>70))
    {
        //��ֹ���߳���
        L_edge[0].row = L_edge[1].row;
        L_edge[0].col = L_edge[1].col;
        //��ʼ�����߸���
        Mid_count=0;
        for(int i=0;i<L_edge_count;i++)
        {
            int16 col = ((2*(int16)L_edge[i].col + (int16)Road_Width[Image_H - L_edge[i].row])/2);
            if (col>Image_W-1)    col = Image_W-1;
            else if(col<0)  col = 0;
            Mid_count = Mid_count + 1;
            M_Line[i].row = L_edge[i].row;
            M_Line[i].col = (uint8)col;
            M_Line[i].flag = searchpoint;
        }
    }
    //      ����߶��ߣ��ұ��߲�����
    else if((left_findflag==0&&right_findflag==1)||(right_findflag&&R_edge_count-L_edge_count>70))
    {
        //��ֹ���߳���
        R_edge[0].row = R_edge[1].row;
        R_edge[0].col = R_edge[1].col;
        //��ʼ�����߸���
        Mid_count=0;
        for(int i=0;i<R_edge_count;i++)
        {
            int16 col = ((2*(int16)R_edge[i].col - (int16)Road_Width[Image_H  - R_edge[i].row])/2);
            if (col>Image_W-1)    col = Image_W-1;
            else if(col<0)  col = 0;
            Mid_count = Mid_count + 1;
            M_Line[i].row = R_edge[i].row;
            M_Line[i].col = (uint8)col;
            M_Line[i].flag = searchpoint;
        }
    }
    //���3������ȫ����
    //���4�����ұ߾����ڣ�������̫��
    if(Mid_count>0)
    {
        uint8 down_cnt = 15;
        //��ͷ��������û�ҵ������������
        if(M_Line[0].row < Image_H -2 && M_Line[0].row > Image_H/4 && Mid_count>2)
        {
            int num = Mid_count/4;
            int sumX = 0,sumY = 0;
            float sumUP= 0,sumDown=0,avrX=0,avrY=0,K,B;
            for(int i = 0;i<num;i++)
            {
                sumX = sumX + M_Line[i].row;
                sumY = sumY + M_Line[i].col;
            }
            avrX = 1.0 * sumX / num;
            avrY = 1.0 * sumY / num;
            for(int i=0;i<num;i++)
            {
                sumUP = sumUP + (M_Line[i].col-avrY)*(M_Line[i].row - avrX);
                sumDown = sumDown + (M_Line[i].row-avrX)*(M_Line[i].row-avrX);
            }
            if(sumDown==0)
            {
                K = 0;
            }
            else
            {
                K = 1.0*sumUP/sumDown;
                B = 1.0*(sumY - K*sumX)/num;
            }
            for(int i = M_Line[0].row;i<Image_H;i++)  //��ʼ�������
            {
                int col = K * i + B;
                if(col > Image_W-1) col = Image_W - 1;
                else if(col < 0)    col = 0;
                MID_LINE[mid_cnt].row = (uint8)i;
                MID_LINE[mid_cnt].col = (uint8)col;
                mid_cnt = mid_cnt + 1;
                if(--down_cnt==0) break;
            }
        }
        //���ߵ����̫�٣������������
        if(Mid_count + mid_cnt < 60 && Mid_count >2)
        {
            int num = Mid_count/4;
            int sumX = 0,sumY = 0;
            float sumUP= 0,sumDown=0,avrX=0,avrY=0,K,B;
            for(int i = Mid_count-num-1;i<Mid_count;i++)
            {
                sumX = sumX + M_Line[i].row;
                sumY = sumY + M_Line[i].col;
            }
            avrX = 1.0 * sumX / num;
            avrY = 1.0 * sumY / num;
            for(int i=Mid_count-num-1;i<Mid_count;i++)
            {
                sumUP = sumUP + (M_Line[i].col-avrY)*(M_Line[i].row - avrX);
                sumDown = sumDown + (M_Line[i].row-avrX)*(M_Line[i].row-avrX);
            }
            if(sumDown==0)
            {
                K = 0;
            }
            else
            {
                K = 1.0*sumUP/sumDown;
                B = 1.0*(sumY - K*sumX)/num;
            }
            for(int i = M_Line[Mid_count-1].row;i>0;i--)  //��ʼ�������
            {
                int col = K * i + B;
                if(col > Image_W-1) col = Image_W - 1;
                else if(col < 0)    col = 0;
                M_Line[Mid_count].row = (uint8)i;
                M_Line[Mid_count].col = (uint8)col;
                Mid_count = Mid_count + 1;
                if(--up_cnt==0) break;
            }
        }
        //�����������ˣ���������ߴ�����
        if(mid_cnt>0)
        {
            for(int i = 0;i<Mid_count;i++)
            {
                MID_LINE[mid_cnt].row = M_Line[i].row;
                MID_LINE[mid_cnt].col = M_Line[i].col;
                mid_cnt = mid_cnt + 1;
            }
        }
    }
    if(mid_cnt > 0)
    {
        Mid_count = mid_cnt;
        Mid_Line = MID_LINE;
    }
    else
        Mid_Line = M_Line;
    //����ƽ��
    float k = 0.8;
    float k2;
    for(int i=0;i<Mid_count-1;i++)
    {
        Mid_Line[i].row = (Mid_Line[i].row + Mid_Line[i+1].row)/2;
        Mid_Line[i].col = (Mid_Line[i].col + Mid_Line[i+1].col)/2;
    }
    for(int i=Mid_count-1;i>1;i--)
    {
        Mid_Line[i].row = (Mid_Line[i].row + Mid_Line[i-1].row)/2;
        Mid_Line[i].col = (Mid_Line[i].col + Mid_Line[i-1].col)/2;
    }
    if(last_mid_count>=Mid_count)
    {
        k2 = 1.0*last_mid_count/Mid_count;
        for(int i=0;i<Mid_count;i++)
        {
            Mid_Line[i].row = (1-k)*Last_M_Line[(uint8)(k2*i)].row + k*Mid_Line[i].row;
            Mid_Line[i].col = (1-k)*Last_M_Line[(uint8)(k2*i)].col + k*Mid_Line[i].col;
        }
    }
    else if(last_mid_count<Mid_count)
    {
        k2 = 1.0*Mid_count/last_mid_count;
        for(int i=0;i<last_mid_count;i++)
        {
            Mid_Line[i].row = (1-k)*Last_M_Line[i].row + k*Mid_Line[(uint8)(k2*i)].row;
            Mid_Line[i].col = (1-k)*Last_M_Line[i].col + k*Mid_Line[(uint8)(k2*i)].col;
        }
    }
}
/*--------------------------------------------------------------------------
* ���������ܡ�����ʼ����߽��ұ߽�ṹ��
* ����    ��������
* ���� �� ֵ������
*--------------------------------------------------------------------------*/
void clear_point()
{
    for(int i = 0;i<L_edge_count;i++)
    {
        L_edge[i].row = 0;
        L_edge[i].col = 0;
        L_edge[i].flag = 0;
    }
    for(int i = 0;i<R_edge_count;i++)
    {
        R_edge[i].row = 0;
        R_edge[i].col = 0;
        R_edge[i].flag = 0;
    }
}
/*--------------------------------------------------------------------------
* ���������ܡ�����յ�ĽǶ�ֵ
* ����    ���������������������
* ���� �� ֵ�����Ƕ�ֵ
* ����    ע������
*--------------------------------------------------------------------------*/
int Get_angle(uint8 ax,uint8 ay,uint8 bx,uint8 by,uint8 cx,uint8 cy)
{
    int8 abx=ax-bx;
    int8 aby=ay-by;
    int8 cbx=cx-bx;
    int8 cby=cy-by;
    int8 ab_muti_cb=abx*cbx+aby*cby;
    int8 dist_ab=sqrt(abx*abx+aby*aby);
    int8 dist_cb=sqrt(cbx*cbx+cby*cby);
    int8 cosvalue=ab_muti_cb/(dist_ab*dist_cb);
    return (int)(acos(cosvalue)*180/3.14);
}
/*--------------------------------------------------------------------------
* ���������ܡ����ж��Ƿ������/�ұ߽��
* ����    ���������߻�������ֵ      ��/�ұ߽�ѡ���־(0:��ߣ�1:�ұ�)
* ���� �� ֵ����-1 �� �߽����ֵ��-1��ʾû���ҵ��߽��
* ����    ע������
*--------------------------------------------------------------------------*/
int edge_point_ornot(uint8 row, uint8 side)
{
    //�����
    if(side == 0)
    {
        uint8 find_edge = 0;
        //�ӿ�ʼ��������������
        for(uint8 rowi =row;rowi>0;rowi--)
        {
            //���ͼ�������Ϊ�����⣬��ʼ��������
            if(black_(image[rowi][3]))
            {
                find_edge = find_edge + 1;
                //��������
                for(int col=2;col<Image_W-5;col++)
                {
                    //������ֺںڰװף����ж�Ϊ�߽��ߣ��˳�ѭ��
                    if(black_(image[rowi][col])&&black_(image[rowi][col+1])&&white_(image[rowi][col+2])&&white_(image[rowi][col+3]))
                    {
                        L_basic_row_start = rowi;   //��ֵ��ʼ�����У���
                        if(rowi<Image_H/2&&col>Image_W/2) return -1;
                        return col+1;               //������ֵ
                    }
                }
            }
            if(find_edge==2) return -1;
        }
    }
    //�ұ���
    if(side == 1)
    {
        uint8 find_edge = 0;
        for(uint8 rowi =row;rowi>0;rowi--)
        {
            if(black_(image[rowi][Image_W-4]))
            {
                find_edge = find_edge + 1;
                for(int col=Image_W-4;col>5;col--)
                {
                    if(white_(image[rowi][col-3])&&white_(image[rowi][col-2])&&black_(image[rowi][col-1])&&black_(image[rowi][col]))
                    {
                        R_basic_row_start = rowi;
                        if(rowi<Image_H/2&&col<Image_W/2) return -1;
                        return col-1;
                    }
                }
            }
            if(find_edge == 2) return -1;
        }
    }
    return -1;
}
/*--------------------------------------------------------------------------
* ���������ܡ����ж��Ƿ��Ǻ�����/�����ص㣬Ϊ���ڲ����ö�ֵ��ͼ����׼��
* ����    �������õ������ֵ
* ���� �� ֵ������
* ����    ע������
*--------------------------------------------------------------------------*/
uint8 black_(uint8 x)    //�жϺ�
{
    if(binimage_flag == 0)  //���û�ж�ֵ������ͨ����ֵ�жϺڰ�
    {
        if(x < threshold)
            return 1;
        else if(x >= threshold)
            return 0;
    }
    else if(binimage_flag ==1)
    {
        if(x==0)
            return 1;
        else if(x==1)
            return 0;
    }
    return 0;
}

uint8 white_(uint8 x)    //�жϰ�
{
    if(binimage_flag==0)
    {
        if(x < threshold)
            return 0;
        else if(x >= threshold)
            return 1;
    }
    else if(binimage_flag==1)
    {
        if(x==1)
            return 1;
        else if(x==0)
            return 0;
    }
    return 0;
}
/*--------------------------------------------------------------------------
* ���������ܡ�����ȡ��ֵ��ͼ��
* ����    ��������
* ���� �� ֵ������
* ����    ע������
*--------------------------------------------------------------------------*/
void get_binImage(uint8 thres)
{
    binimage_flag = 1;  //��ֵ����־��Ϊ1
    for(int row=0;row<Image_H;row++)
    {
        for(int col=0;col<Image_W;col++)
        {
            if(image[row][col]<thres)
            {
                image[row][col] = 0;    //С����ֵ��ֵΪ0����ʾΪ��ɫ
            }
            else
                image[row][col] = 1;    //������ֵ��ֵΪ1����ʾΪ��ɫ
        }
    }
}
/*--------------------------------------------------------------------------
* ���������ܡ��������̬��ֵ
* ����    ��������
* ���� �� ֵ������
*--------------------------------------------------------------------------*/
#if 0
#define GrayScale 256   //frame

typedef unsigned char uchar;
int pixel[256]={0};
uint8 OSTU_bin(uint8 width,uint8 height,uint8 *Image)
    {
        int threshold1=0;
        int32 sum_gray=0;
        int32 sum_pix_num=0;
        int32 pl_pix_num=0;
        int32 p2_pix_mum=0;
        int32 p1_sum_gray=0;
        float m1=0;
        float m2=0;
        float V=0;
        float variance=0;
        int i,j,k=0;

        for(i = 0;i<256;i++)
            pixel[i] = 0;


        for(i = 0; i < height; i++)
        {
            for(j = 0;j < width;j++)
            {
                pixel[(int)Image[i * width + j]]++;
            }
        }

            for(k=0;k<GrayScale;k++)
            {
                sum_gray+=k*pixel[k];
                sum_pix_num+=pixel[k];
            }

            for(k=0;k<GrayScale-1;k++)
            {
                pl_pix_num+=pixel[k];
                p2_pix_mum=sum_pix_num-pl_pix_num;
                p1_sum_gray+=k*pixel[k];
                m1=(float)p1_sum_gray/pl_pix_num;
                m2=(float)(sum_gray-p1_sum_gray)/p2_pix_mum;

                V=pl_pix_num*p2_pix_mum*(m1-m2)*(m1-m2);

                if(V>variance)
                {
                    variance=V;
                    threshold1=k;
                }
            }
            return threshold1;
    }

#endif
/*--------------------------------------------------------------------------
* ���������ܡ��������̬��ֵ
* ����    ��������
* ���� �� ֵ������
*--------------------------------------------------------------------------*/
#if 1
uint8 GetOSTUThreshold(uint8 (*img)[MT9V03X_W], uint16 start_row,uint16 end_row, uint16 start_col, uint16 end_col)
{
  int     threshold1 =0;
  int32 sum_gray   =0;
  int32 sum_pix_num=0;
  int32 pl_pix_num =0;
  int32 p2_pix_mum =0;
  int32 p1_sum_gray=0;
  float m1=0;
  float m2=0;
  float V=0;
  float variance=0;
  int i,j,k=0;
  uint16 MinValue=0,MaxValue=255;
  uint16 DeleGrayClass1 = 30; //�߻Ҷȼ�
  uint16  HistoGram[256]={0};

  for(i = 0; i < 256; i++)
    HistoGram[i] = 0;

  for(i = start_row; i < end_row; i++)
  {
      for(j = start_col; j < end_col; j++)
      {
          HistoGram[(int)img[i][j]]++;
      }
  }

  //�Ż�--ɾ���Ҷȼ�����<x����ĻҶȼ�  ɾ���Ҷȼ��ײ�<x����ĻҶȼ� x==> 10-25֮��
  //for(k=255;k>0;--k) {if(HistoGram[k]<=DeleGrayClass1)  HistoGram[k] = 0; else break;}
  //for(k=0;k<256;++k) {if(HistoGram[k]<=DeleGrayClass2)  HistoGram[k] = 0; else break;}

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; ++MinValue) ;        //��ȡ��С�Ҷȼ�
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0; --MaxValue) ; //��ȡ���Ҷȼ�

  for(k=MinValue;k<=MaxValue;k++)
  {
      sum_gray+=k*HistoGram[k];
      sum_pix_num+=HistoGram[k];
  }

  for(k=MinValue;k<=MaxValue;k++)
  {
      pl_pix_num+=HistoGram[k];
      p2_pix_mum=sum_pix_num-pl_pix_num;
      p1_sum_gray+=k*HistoGram[k];
      m1=(float)p1_sum_gray/pl_pix_num;
      m2=(float)(sum_gray-p1_sum_gray)/p2_pix_mum;

      V=pl_pix_num*p2_pix_mum*(m1-m2)*(m1-m2);

      if(V>variance)
      {
          variance=V;
          threshold1=k;
      }
  }

#if 1
  uint8 t1      = threshold1/2;
  uint8 t2      = threshold1+10;  //���췶Χ
  float rate    = 1.25;          //�����������
  //uint8 top_y   = 15;            //�������� 0-top_y
  uint8 side_x  = 20;            //������� 0-side_x && (COL_1-side_x)-COL


  /* ��ʼ�� */
  for(i=255;i>0;i--)
      HistoGram[i] = i;

  /* �ݶȱ任--�Աȶ����� */
  for(i=t1;i<t2;i++)
  {
      HistoGram[i] = (uint16)(i*rate);
      if(HistoGram[i]>t2) HistoGram[i] = t2;
  }

  //���
  for(i=0;i<MT9V03X_H;i++)
  {
      for(j=0;j<side_x;j++)
      {
          mt9v03x_image[i][j] = HistoGram[mt9v03x_image[i][j]];
      }
  }

  //�Ҳ�
  for(i=0;i<MT9V03X_H;i++)
  {
      for(j=MT9V03X_W - 1 - side_x;j<MT9V03X_W;j++)
      {
          mt9v03x_image[i][j] = HistoGram[mt9v03x_image[i][j]];
      }
  }
#endif
  return threshold1;
}
#endif
/*--------------------------------------------------------------------------
* ���������ܡ�����ֵ�˲�
* ����    ��������
* ���� �� ֵ������
*--------------------------------------------------------------------------*/
int average_value(uint8 *c)
{
    int i;
    int k;
    uint16 t_sum;
    t_sum=0;
    for(i=0;i<=8;i++)
    {
     t_sum=t_sum+c[i];
    }
    k=t_sum/9;
    return k;
}
void average_filter(void)
    {
       uint8 i,j;
         uint8  c[9];
         for(i=1;i<Image_H-1;i++)
        {
           for(j=1;j<Image_W-1;j++)
            {
                  if(i>=8)
                 {
                     c[0]=image[i-1][j-1];
                     c[1]=image[i-1][j];
                     c[2]=image[i-1][j+1];
                     c[3]=image[i][j-1];
                     c[4]=image[i][j];
                     c[5]=image[i][j+1];
                     c[6]=image[i+1][j-1];
                     c[7]=image[i+1][j];
                     c[8]=image[i+1][j+1];
                     image[i][j]=average_value(c);
                 }

            }
        }

    }
void get_deal_image()
{
    for(int row=0;row<Image_H;row++)
        for(int col = 0;col<Image_W;col++)
        {
            image[row][col] = mt9v03x_image[row][col];
        }
}
