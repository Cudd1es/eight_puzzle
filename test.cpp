  #include<iostream>
  #include<cstdio>
  #include<cstdlib>
  #include<cstring>
  #include<queue>
  #define COL 3
  #define MAXSTEP 70
  using namespace std;


  void output(int [][COL]);/*输出函数*/
  void input(int [][COL]);/*输出函数*/
  int Astar(int [][COL], int [][COL], int, int path[]);/*核心函数，起始，终止，深度，方向*/
  bool eq(int input_map[][COL], int std_map[][COL]);/*判断起始与终止是否相同*/
  bool change(int input_map[][COL], const int i, const int j, const int step);/*判断当前状态是否可以进行相应移动,并进行状态转变*/
  int value(const int input_map[][COL], const int std_map[][COL]);/*估价函数 比起简单地判断【不符合位置的数字个数只和】，【每个点回到原位所需的步数(曼哈顿距离)总和】启发性更强，更智能一些*/
  void output_tow(int input_map[][COL], int std_map[][COL]);/*输出函数*/
  bool possible(int input_map[][COL], int std_map[][COL]);/*可行性判断 原理：八数码无论怎么移动逆序的奇偶性不变，判断输入和目标结点的逆序奇偶性是否相同即可知道是否有解*/
  int h[9] = {40320, 5040, 720, 120, 24, 6, 2, 1, 1};/*hash函数用到的数据 8-0的阶乘 至于为什么是这些数据我也不太懂，网上说这样的数据更科学*/
  bool ha[400000];
  struct Node{
      int path[MAXSTEP]; /*路径信息*/
      int expend;/*权重*/
      int depth;/*深度*/
      int x[COL][COL];/*状态信息*/
  };
  struct cmp{
      bool operator() (const Node A,const Node B){
          return A.expend>B.expend;
      }
  };
  int pa[MAXSTEP];
  priority_queue<Node,vector<Node>,cmp> q;/*优先队列*/
  Node make(int input_map[][COL], int depth, int v, int path[], int step);/*转换函数*/
  int main()
  {
      int input_map[COL][COL];
      int std_map[COL][COL] = {1, 2, 3, 8, 0, 4, 7, 6, 5};
      int k = 0, c;
      memset(ha, 0, sizeof(ha));
      memset(pa, -1, sizeof(pa));
      printf("Please enter the data, use '0' to replace the blank\n");
      input(input_map);
      printf("Your test case is\n");
      output(input_map);
      printf("Press any key to rock on\n");
      fflush(stdin);
      getchar();
      if(!possible(input_map, std_map)){
          cout<<"Your case has no standard output, which means the result is impossible to be found out." << endl;
          system("pause");
          return 0;
      }
      int d = Astar(input_map, std_map, 0, pa);
      cout << "Current State" << '\t' << "Target State" << endl;
      cout << endl;

      while((c = pa[++k]) != -1){
          int i, j;
          cout << "Step" << k << ":" << endl;
          for(i = 0; i < 3; i++)
              for(j = 0; j < 3; j++)
              if(input_map[i][j] == 0)  goto o;
          o:
              change(input_map, i, j, pa[k]);
              output_tow(input_map, std_map);
          cout << endl;
      }

      cout << "It took " << d << "step(s) to get to the target." << endl;
      cout << "Calculation completed. Have a nice day, bro." << endl;
      cout << endl;
      system("pause");
      return 0;
  }//main()

  void output(int a[][COL])
  {
      int i, j;
      for(i = 0; i < COL; i++){
      for(j = 0; j < COL; j++)
      printf("%d ", a[i][j]);
          putchar('\n');
   }
 }//void output


  void output_tow(int input_map[][COL], int std_map[][COL])
  {
      int i, j;
      for(i = 0; i < COL; i++){
          for(j = 0; j < COL; j++)  printf("%d ", input_map[i][j]);
              cout << '\t' << '\t';
          for(j = 0; j < COL; j++)  printf("%d ", std_map[i][j]);
          putchar('\n');
      }
  }//void output_tow

  void input(int a[][COL])
  {
      int i, j, c;
  s:
      int g[9];
      memset(g, 0, sizeof(g));
      for(i = 0; i < COL; i++)
          for(j = 0; j < COL; j++){
              scanf("%d", &a[i][j]);
              c = a[i][j];
              if(g[c] || c < 0 || c > 8){
                  cout << "Invalid input. Please input again if you want, or just exit." << endl;
                  goto s;
              }
              g[c]++;
          }
  }
  int Astar(int input_map[][COL], int std_map[][COL], int depth, int path[])
  {
      if(eq(input_map, std_map)){
          memcpy(pa, path, sizeof(pa));
          return depth;
      }


      int i, j;
      int *a = input_map[0];
      int b[9];
      int m = 0;
      for(i = 0; i < 9; i++)  b[i] = a[i];
      for(i = 0; i < 9; i++){
          for(j = 0; j < i; j++)
            if(b[i] > a[j]) b[i]--;
          m += h[i] * b[i];
      }
      ha[m] = 1;
      for(i = 0; i < 3; i++)
          for(j = 0; j < 3; j++)
          if(input_map[i][j] == 0)
              goto ok;
  ok: for(int step = 0; step < 4; step++){                  //step为0, 1, 2, 3 分别代表上下左右
          if(change(input_map, i, j, step)){
              int v = value(input_map, std_map) + depth + 1;
              Node n;
              n = make(input_map, depth + 1, v, path, step);
              q.push(n);
              change(input_map, i, j, step);
          }
      }

      Node p = q.top();
      int flag = 0;
      while(!flag){
          a = p.x[0];
          m = 0;
          for(i = 0; i < 9; i++)
              b[i] = a[i];
          for(i = 0; i < 9; i++){
              for(j = 0; j < i; j++)
                  if(b[i] > a[j]) b[i]--;
              m += h[i] * b[i];
          }
          if(!ha[m]){
              q.pop();
              break;
          }
          q.pop();
          p = q.top();
      }
      return Astar(p.x, std_map, p.depth, p.path);
  }//int Astar()

  bool eq(int input_map[][COL], int std_map[][COL])/*判断是否相同*/
  {
      for(int i = 0; i < 3; i++)
          for(int j = 0; j < 3; j++){
              if(input_map[i][j] != std_map[i][j])
                  return 0;
          }
      return 1;
  }//bool eq()

  bool change(int input_map[][COL], const int i, const int j, const int step)/*判断当前状态是否可以进行相应移动*/
  {
      if((i == 0 && step == 0) || (i == 2 && step == 1) || (j == 0 && step == 2) || (j == 2 && step == 3))
          return 0;
      int a = input_map[i][j];
      switch(step){
      case 0:
          input_map[i][j] = input_map[i-1][j];
          input_map[i-1][j] = a;
          break;
      case 1:
          input_map[i][j] = input_map[i+1][j];
          input_map[i+1][j]=a;
          break;
      case 2:
          input_map[i][j] = input_map[i][j-1];
          input_map[i][j-1] = a;
          break;
      case 3:
          input_map[i][j] = input_map[i][j+1];
          input_map[i][j+1] = a;
          break;
      default:
          cout << "WRONG!" << endl;
          break;
      }
      return 1;
  }//bool change()

  int value(const int input_map[][COL], const int std_map[][COL])/*估价函数*/
  {
      int i, j, m, n;
      int v = 0;
      for(i = 0; i < 3; i++)
          for(j = 0; j < 3; j++){
             for(m = 0; m < 3; m++)
                  for(n = 0; n < 3; n++)
                      if(input_map[i][j] == std_map[m][n])
                          goto p;
             p:
             if(input_map[i][j] != 0)
                  v += (abs(i - m) + abs(j - n));
          }
      return v;
  }
  Node make(int input_map[][COL], int depth, int v, int path[], int step)/*转换函数*/
  {
      Node p;
      for(int i = 0; i < 3; i++)
      for(int j = 0; j < 3; j++){
          p.x[i][j] = input_map[i][j];
      }
      p.depth = depth;
      p.expend = v;
      memcpy(p.path, path, sizeof(int)*MAXSTEP);
      p.path[depth] = step;
      return p;
  }
  bool possible(int input_map[][COL], int std_map[][COL])/*可行性判断*/
  {
      int m = 0, n = 0;
      int i, j, k, l;
      int a[COL * COL], b[COL * COL];
      for(i = 0; i < COL; i++)
          for(j = 0; j < COL; j++){
              a[i * COL + j] = input_map[i][j];
              b[i * COL + j] = std_map[i][j];
          }
      for(k = 0; k < COL * COL; k++)
          for(l = k + 1; l < COL * COL; l++){
              if(a[l] < a[k] && a[l] != 0)
                  m++;
              if(b[l] < b[k] && b[l] != 0)
                  n++;
          }
      return (n % 2) == (m % 2);
  }
