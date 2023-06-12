#include "nosaydie_camera.h"
#include "headfile.h"
#include "common.h"
#include "nosaydie_camera_bianliang.h"
//声明全局变量-------------------------------------------
//  图像
//      图像预处理
  uint8 threshold;                  //二值化阈值
  uint8 image[MT9V03X_H][MT9V03X_W];        //使用二维指针image代替信息图像，便于后期二值化图像和深度图像的转换
  uint8 binimage_flag = 0;          //二值化标志，默认没有二值化
  struct LEFT_EDGE L_edge[150];     //左边界结构体
  struct RIGHT_EDGE R_edge[150];    //右边界结构体
  struct LEFT_EDGE M_Line[150];     //中线结构体，备用选择2
  struct LEFT_EDGE Last_M_Line[150];//上次中线结构体
  struct LEFT_EDGE MID_LINE[150];   //中线结构体，备用选择1
  struct LEFT_EDGE *Mid_Line;       //中线结构体【实际使用】
//      用于中线处理
  uint8 searchpoint = 1;            //正常求出来的中线点标志，用于结构体.flag，便于后期处理
  uint8 nihepoint = 2;              //拟合求出来的中线点标志，用于结构体.flag，便于后期处理
  extern uint8 Road_Width[MT9V03X_H];//质量矩，用于丢线
//      图像显示
  uint8 l_display_cnt = 0;
  uint8 r_display_cnt = 0;
/*--------------------------------------------------------------------------
* 【函数功能】：图像处理（中线、元素识别）_八邻域容易越界
* 【参    数】：无
* 【返 回 值】：无
*--------------------------------------------------------------------------*/
int x1,x2,x3,x4;
void Image_process()
{
    //变量相关
    //  全局、外部变量--------------------------
    //      全局变量
    //      外部变量
    //  函数内部变量-------------------
    //      一般图像处理
     uint8 Bottom2 = Image_H - 40;   //倒数第二行
     uint8 max_col = Image_W - 5,min_col = 5;         //最大/小列坐标，即边界值
     uint8 L_search_amount = 150, R_search_amount = 150;  //左右边界搜点时最多允许的点
     uint8 jilu_row_l = 0, jilu_col_l = 0, jilu_row_r = 0, jilu_col_r = 0;  //记录搜索到的基础边界点行列值
    //图像处理-----------------------
    //  一般图像处理
    //      初始化相关变量
     enable_check_l_r_edge_same = 0;    //使能检查做右边线是否爬重合，默认不开启，当爬线起始点过高时开启
     left_findflag = 0;                 //左边界存在标志，1找到左边界，0没找到左边界        默认没有找到左边界
     right_findflag = 0;                //右边界存在标志，1找到有边界，0没找到右边界        默认没有找到右边界
     L_basic_row_start = Image_H -2;    //左开始搜线点
     R_basic_row_start = Image_H -2;    //右开始搜线点
    //      获取图像二值化阈值和图像二值化
     get_deal_image();
     //average_filter();
     //threshold = OSTU_bin(Image_W,Image_H,mt9v03x_image);//获取动态阈值
     threshold = GetOSTUThreshold(image,0,Image_H-1,0,Image_W-1);
     //get_binImage(threshold);//获取二值化图像
     //      开始爬边线
     enable_balinyu = 1;
     if(enable_balinyu)//如果使能八邻域爬线即一般处理
     {
         //相关变量
         line_lose_center_left = 0;
         line_lose_center_right = 0;
         line_point_count_left = 0;
         line_point_count_right = 0;
         L_edge_count = 0;//左边点个数清0
         R_edge_count = 0;//右边点个数清0
         int exist_edge_size = 0;   //判断是否存在左/右边界
         //寻找左\右线开始点，并判断是否存在当前边
         clear_point();
         exist_edge_size = edge_point_ornot(L_basic_row_start,0);
         if(exist_edge_size >= 0){jilu_row_l = L_basic_row_start;jilu_col_l = exist_edge_size;left_findflag = 1;}
         exist_edge_size = edge_point_ornot(R_basic_row_start,(uint8)(1));
         if(exist_edge_size >= 0){jilu_row_r = R_basic_row_start;jilu_col_r = exist_edge_size;right_findflag = 1;}
         //八邻域搜线
         if(left_findflag)//如果左边界点存在并找到,则开始爬线
         {
             //变量声明
             L_edge[0].row = jilu_row_l;
             L_edge[0].col = jilu_col_l;
             L_edge[0].flag = 1;
             uint8 curr_row = jilu_row_l;//初始化行坐标
             uint8 curr_col = jilu_col_l;//初始化列坐标
             dire_left = 0; //初始化上个边界点的来向
             center_turn_flag = 1;//初始化在未标定状态
             //开始搜线，最多取150个点，不会往下搜，共7个方位
             for(int i = 1;i < L_search_amount; i++)    //最多搜索70个点
             {
                 ////越界退出 行越界和列越界（向上向下向左向右）
                 if(curr_row < L_edge_end_row || curr_row>Image_H-1||curr_row+1<L_edge_end_row)  break;
                 if(curr_col > max_col || curr_col < min_col)
                 {
                     if(++L_search_edge_count == 3)//连续3次搜索到边界，退出
                         break;
                 }
                 else
                     L_search_edge_count = 0;
                 //搜线过程
                 if(dire_left != 2&&black_(image[curr_row-1][curr_col-1])&&white_(image[curr_row-1][curr_col]))   //左上黑，2，右边白
                 {
                     curr_row = curr_row -1;
                     curr_col = curr_col -1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 7;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=3&&black_(image[curr_row-1][curr_col+1])&&white_(image[curr_row][curr_col+1]))    //右上黑，3，下边白
                 {
                     curr_row = curr_row -1;
                     curr_col = curr_col + 1;
                     L_edge_count = L_edge_count + 1;
                     dire_left = 6;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(black_(image[curr_row-1][curr_col])&&white_(image[curr_row-1][curr_col+1]))                  //正上黑，1，右白
                 {
                     curr_row = curr_row - 1;
                     L_edge_count = L_edge_count + 1;
                     dire_left = 0;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=5&&black_(image[curr_row][curr_col-1])&&white_(image[curr_row-1][curr_col-1]))     //正左黑，5，上白
                 {
                     curr_col = curr_col - 1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 4;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=4&&black_(image[curr_row][curr_col+1])&&white_(image[curr_row+1][curr_col+1]))  //正右黑，4，下白
                 {
                     curr_col = curr_col + 1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 5;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=6&&black_(image[curr_row+1][curr_col-1])&&white_(image[curr_row][curr_col-1]))    //左下黑，6，上白
                 {
                     curr_row = curr_row + 1;
                     curr_col = curr_col -1;
                     L_edge_count = L_edge_count +1;
                     dire_left = 3;
                     L_edge[i].row = curr_row;
                     L_edge[i].col = curr_col;
                     L_edge[i].flag = 1;
                 }
                 else if(dire_left!=7&&black_(image[curr_row+1][curr_col+1])&&white_(image[curr_row+1][curr_col]))    //右下黑，7，左白
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
         if(right_findflag)//如果右边界存在并搜到
         {
             R_edge[0].row = jilu_row_r;
             R_edge[0].col = jilu_col_r;
             R_edge[0].flag = 1;
             uint8 curr_row = jilu_row_r;
             uint8 curr_col = jilu_col_r;
             dire_right = 0;
             for(int i = 1;i<R_search_amount;i++)
             {
                 //越界退出 行越界和列越界（向上向下向左向右）
                 if(curr_row < L_edge_end_row ||curr_row>Image_H-1||curr_row+1<L_edge_end_row)  break;
                 if(curr_col > max_col || curr_col < min_col)   //连续三次搜索到边界，退出
                 {
                     if(++R_search_edge_count == 3)
                         break;
                 }
                 else   R_search_edge_count = 0;
                 //爬线过程
                 if(curr_col<Image_W&&dire_right!=3&&black_(image[curr_row-1][curr_col+1])&&white_(image[curr_row-1][curr_col]))    //右上黑，3，左白
                 {
                     curr_row = curr_row - 1;
                     curr_col = curr_col + 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 6;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=2&&black_(image[curr_row-1][curr_col-1])&&white_(image[curr_row][curr_col-1])) //左上黑，2，下白
                 {
                     curr_row = curr_row-1;
                     curr_col = curr_col-1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 7;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(black_(image[curr_row-1][curr_col])&&white_(image[curr_row-1][curr_col-1]))                  //正上黑，1，左白
                 {
                     curr_row = curr_row - 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 0;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=5&&black_(image[curr_row][curr_col-1])&&white_(image[curr_row+1][curr_col-1]))   //正左黑，5，下白
                 {
                     curr_col = curr_col-1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 4;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }

                 else if(dire_right!=4&&black_(image[curr_row][curr_col+1])&&white_(image[curr_row-1][curr_col+1]))   //正右黑，4，上白
                 {
                     curr_col = curr_col + 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 5;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=6&&black_(image[curr_row+1][curr_col-1])&&white_(image[curr_row+1][curr_col]))   //左下黑，6，右白
                 {
                     curr_row = curr_row + 1;
                     curr_col = curr_col - 1;
                     R_edge_count = R_edge_count + 1;
                     dire_right = 3;
                     R_edge[i].row = curr_row;
                     R_edge[i].col = curr_col;
                     R_edge[i].flag = 1;
                 }
                 else if(dire_right!=7&&black_(image[curr_row+1][curr_col+1])&&white_(image[curr_row][curr_col+1]))   //右下黑，7，上白
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
         //边界预处理
         // 相关变量初始化
         pre_L_edge_count = 0;
         pre_R_edge_count = 0;
         // 左边界处理
         if(left_findflag)//如果左边界存在并找到
         {
             //平滑边界
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
             edge_process_flag = 0; //清零标志
             //截断连续水平的边界
             if(L_edge_count >=70)
             {
                 num_cnt = 0;//记录连续水平点的个数
                 L_count = L_edge_count/2;
                 while(L_count<L_edge_count)
                 {
                     if(L_edge[L_count].row == L_edge[L_count+1].row)
                         num_cnt = num_cnt +1;
                     else
                         num_cnt = 0;
                     if(num_cnt > 5)//连续5个点水平
                         break;
                     L_count = L_count +1;
                 }
                 L_edge_count = L_count;//截断在5个水平点处
             }
         }
         //右边界处理
         if(right_findflag)//右边界存在且找到时开始处理
         {
             //平滑边界
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
             //截断连续水平的边界
             if(R_edge_count>=70)   //边界够长时才开始处理
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
         //检测异常边界
         // 情况1：左、右边线横跨图像均超过Width/2，考虑做右边线爬线重合的问题，使能重合检索
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
                 //情况1：大弯道-特征：左右边界搜线起始点行值差大
                 if(fabs(L_edge[0].row-R_edge[0].row)>=15)
                 {
                     if(L_edge[0].row>R_edge[0].row)        //左搜线点高，考虑左边线丢线，左向弯道
                         left_findflag = 0;
                     else if(R_edge[0].row>L_edge[0].row)   //右搜线点高，考虑右边线丢线，右向弯道
                         right_findflag = 0;
                 }
                 //情况2：考虑三叉/闭环十字—特征：考虑有无拐点（三叉路口变现重合有拐点、闭环十字无拐点）
                 else if(fabs(L_edge[0].row-R_edge[0].row)<15)
                 {

                 }
             }
         }
         //边界点太少，去掉
         if(L_edge_count <10)
             left_findflag = 0;
         if(R_edge_count <10)
             right_findflag = 0;
         //左边线起始点大于右边线
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
         if(L_edge_count-R_edge_count>30&&right_findflag)  //左右边界数量差距太大，左>>右
         {
             right_findflag = 0;
             R_edge_count = 0;
         }
         if(R_edge_count-L_edge_count>30&&left_findflag)   //右>>左
         {
             left_findflag = 0;
             L_edge_count = 0;
         }
         //如果右/右边界太往上且点数差距较大，删除边线
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
         if(L_edge[L_edge_count-1].row - L_edge[1].row>-10)  //边线几乎水平 左
             left_findflag = 0;
         if(R_edge[R_edge_count-1].row - R_edge[1].row>-10)  //右
             right_findflag = 0;
         //存在某一边界，开启中线拟合
         if(left_findflag || right_findflag)
             enable_midline = 1;
         else
             enable_midline = 0;
         //检测拐点
         L_corner_flag = 0;// 初始化变量
         L_corner_row = 0;
         L_corner_col = 0;
         L_corner_angle = 0;
         if(enable_L_corner) //如果使能搜索左拐点
         {
             if(L_edge_count > 9)
             {
                 for(int i = 0; i<L_edge_count-9;i++)
                 {
                     if(L_edge[i+4].row>5)
                     {
                         if((L_edge[i].col - L_edge[i + 4].col) * (L_edge[i + 8].col - L_edge[i + 4].col) +
                            (L_edge[i].row - L_edge[i + 4].row) * (L_edge[i + 8].row - L_edge[i + 4].row) > 0) //初步确认为锐角或者直角 向量法
                         {
                             L_corner_angle = Get_angle(L_edge[i].row, L_edge[i].col, L_edge[i + 4].row, L_edge[i + 4].col, L_edge[i + 8].row, L_edge[i + 8].col); //求角度
                             if(L_edge[i+4].col>L_edge[i+8].col)    //确定拐角朝向，左拐点没有朝向做的
                             {
                                 L_corner_flag = 1;//异常拐点
                                 L_corner_row = L_edge[i+4].row;
                                 L_corner_col = L_edge[i+4].col;
                                 break;
                             }
                         }
                     }
                 }
             }
         }
         R_corner_flag = 0;//初始化变量
         R_corner_row = 0;
         R_corner_col = 0;
         R_corner_angle = 0;
         if(enable_R_corner)    //如果使能搜索右拐点
         {
             if(R_edge_count > 9)
             {
                 for(int i = 0; i<R_edge_count-9;i++)
                 {
                     if(R_edge[i+4].row>5)
                     {
                         if((R_edge[i].col - R_edge[i + 4].col) * (R_edge[i + 8].col - R_edge[i + 4].col) +
                         (R_edge[i].row - R_edge[i + 4].row) * (R_edge[i + 8].row - R_edge[i + 4].row) > 0) //初步确认为锐角或者直角 向量法
                         {
                             R_corner_angle = Get_angle(R_edge[i].row, R_edge[i].col, R_edge[i + 4].row, R_edge[i + 4].col, R_edge[i + 8].row, R_edge[i + 8].col); //求角度
                             if(R_edge[i+8].col>R_edge[i+4].col)    //确定拐角朝向，左拐点没有朝向做的
                             {
                                 R_corner_flag = 1;//异常拐点
                                 R_corner_row = R_edge[i+4].row;
                                 R_corner_col = R_edge[i+4].col;
                                 break;
                             }
                         }
                     }
                 }
             }
         }
         //拐点处理
         //方向（测试用）测试完删除

     }
     //如果使能中线处理，则开始处理中线
     if(enable_midline)
         get_mid();
}
/*--------------------------------------------------------------------------
* 【函数功能】：获取中线
* 【参    数】：无
* 【返 回 值】：无
* 【备    注】：无
*--------------------------------------------------------------------------*/
int size1 = 0;
void get_mid()
{
    size1 = 1;
    //相关变量
    uint8 mid_cnt = 0;      //用于向下拟合，如果开启拟合，则用于记录新的中线点个数
    uint8 last_mid_count=0; //用于记录上一场中线的中线个数，用于中线平缓
    uint8 up_cnt=15;        //向上生长的限制个数
    //保存上一场中线数据
    for(int i = 0;i<Mid_count;i++)
    {
        Last_M_Line[i].row = Mid_Line[i].row;
        Last_M_Line[i].col = Mid_Line[i].col;
    }
    last_mid_count = Mid_count;
    //情况1：左右均找到
    if(left_findflag&&right_findflag)
    {
        size1 = 2;
        //初始化中线个数
        Mid_count=0;
        //防止中线出错
        L_edge[0].row = L_edge[1].row;
        L_edge[0].col = L_edge[1].col;
        R_edge[0].row = R_edge[1].row;
        R_edge[0].col = R_edge[1].col;
        //左边线比右边线长
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
        //右边线比左边线长
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
    //情况2：单边全丢线
    //      右边界丢线，左边界不丢线||边线数量差距大
    else if((left_findflag==1&&right_findflag==0)||(left_findflag&&L_edge_count-R_edge_count>70))
    {
        //防止中线出错
        L_edge[0].row = L_edge[1].row;
        L_edge[0].col = L_edge[1].col;
        //初始化中线个数
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
    //      左边线丢线，右边线不丢线
    else if((left_findflag==0&&right_findflag==1)||(right_findflag&&R_edge_count-L_edge_count>70))
    {
        //防止中线出错
        R_edge[0].row = R_edge[1].row;
        R_edge[0].col = R_edge[1].col;
        //初始化中线个数
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
    //情况3：两边全丢线
    //情况4：左右边均存在，但中线太少
    if(Mid_count>0)
    {
        uint8 down_cnt = 15;
        //车头附近赛道没找到，则向下拟合
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
            for(int i = M_Line[0].row;i<Image_H;i++)  //开始向下拟合
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
        //中线点个数太少，开启向上拟合
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
            for(int i = M_Line[Mid_count-1].row;i>0;i--)  //开始向上拟合
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
        //如果向下拟合了，则搬运中线代码至
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
    //中线平缓
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
* 【函数功能】：初始化左边界右边界结构体
* 【参    数】：无
* 【返 回 值】：无
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
* 【函数功能】：求拐点的角度值
* 【参    数】：三个点的行列坐标
* 【返 回 值】：角度值
* 【备    注】：无
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
* 【函数功能】：判断是否存在左/右边界点
* 【参    数】：搜线基本行数值      左/右边界选择标志(0:左边，1:右边)
* 【返 回 值】：-1 或 边界点列值，-1表示没有找到边界点
* 【备    注】：无
*--------------------------------------------------------------------------*/
int edge_point_ornot(uint8 row, uint8 side)
{
    //左边线
    if(side == 0)
    {
        uint8 find_edge = 0;
        //从开始搜线行向上生长
        for(uint8 rowi =row;rowi>0;rowi--)
        {
            //如果图像最左侧为赛道外，则开始横向生长
            if(black_(image[rowi][3]))
            {
                find_edge = find_edge + 1;
                //横向生长
                for(int col=2;col<Image_W-5;col++)
                {
                    //如果出现黑黑白白，则判断为边界线，退出循环
                    if(black_(image[rowi][col])&&black_(image[rowi][col+1])&&white_(image[rowi][col+2])&&white_(image[rowi][col+3]))
                    {
                        L_basic_row_start = rowi;   //赋值开始搜线行（左）
                        if(rowi<Image_H/2&&col>Image_W/2) return -1;
                        return col+1;               //返回列值
                    }
                }
            }
            if(find_edge==2) return -1;
        }
    }
    //右边线
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
* 【函数功能】：判断是否是黑像素/白像素点，为后期不适用二值化图像做准备
* 【参    数】：该点的像素值
* 【返 回 值】：无
* 【备    注】：无
*--------------------------------------------------------------------------*/
uint8 black_(uint8 x)    //判断黑
{
    if(binimage_flag == 0)  //如果没有二值化，则通过阈值判断黑白
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

uint8 white_(uint8 x)    //判断白
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
* 【函数功能】：获取二值化图像
* 【参    数】：无
* 【返 回 值】：无
* 【备    注】：无
*--------------------------------------------------------------------------*/
void get_binImage(uint8 thres)
{
    binimage_flag = 1;  //二值化标志置为1
    for(int row=0;row<Image_H;row++)
    {
        for(int col=0;col<Image_W;col++)
        {
            if(image[row][col]<thres)
            {
                image[row][col] = 0;    //小于阈值赋值为0，显示为黑色
            }
            else
                image[row][col] = 1;    //大于阈值赋值为1，显示为白色
        }
    }
}
/*--------------------------------------------------------------------------
* 【函数功能】：大津法求动态阈值
* 【参    数】：无
* 【返 回 值】：无
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
* 【函数功能】：大津法求动态阈值
* 【参    数】：无
* 【返 回 值】：无
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
  uint16 DeleGrayClass1 = 30; //高灰度级
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

  //优化--删除灰度级顶部<x个点的灰度级  删除灰度级底部<x个点的灰度级 x==> 10-25之间
  //for(k=255;k>0;--k) {if(HistoGram[k]<=DeleGrayClass1)  HistoGram[k] = 0; else break;}
  //for(k=0;k<256;++k) {if(HistoGram[k]<=DeleGrayClass2)  HistoGram[k] = 0; else break;}

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; ++MinValue) ;        //获取最小灰度级
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0; --MaxValue) ; //获取最大灰度级

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
  uint8 t2      = threshold1+10;  //拉伸范围
  float rate    = 1.25;          //拉伸比例！！
  //uint8 top_y   = 15;            //顶部行域 0-top_y
  uint8 side_x  = 20;            //侧边列域 0-side_x && (COL_1-side_x)-COL


  /* 初始化 */
  for(i=255;i>0;i--)
      HistoGram[i] = i;

  /* 梯度变换--对比度拉伸 */
  for(i=t1;i<t2;i++)
  {
      HistoGram[i] = (uint16)(i*rate);
      if(HistoGram[i]>t2) HistoGram[i] = t2;
  }

  //左侧
  for(i=0;i<MT9V03X_H;i++)
  {
      for(j=0;j<side_x;j++)
      {
          mt9v03x_image[i][j] = HistoGram[mt9v03x_image[i][j]];
      }
  }

  //右侧
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
* 【函数功能】：均值滤波
* 【参    数】：无
* 【返 回 值】：无
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
