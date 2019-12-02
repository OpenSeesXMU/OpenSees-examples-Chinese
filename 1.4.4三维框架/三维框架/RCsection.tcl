# 定义一个程序用来生成钢筋混凝土截面,截面沿四周均匀分布一排钢筋,如下图
# 
#                       y
#                       /\
#                       |
#                       |    
#          4  ---------------------  3
#             |\                 /|
#             | \---------------/ |
#             | |               | |
#             | |               | |
# z /---------| |               | |  h
#   \         | |               | |
#             | |               | |
#             | /---------------\ |
#             |/                 \|
#          1  ---------------------  2
#                       b
#
# 形式变元
#    id -      程序生成的截面序列号
#    h -       截面全高
#    b -       截面全宽
#    cover -   保护层厚度
#    coreID -  混凝土核心区分块材料号
#    coverID - 混凝土保护层分块材料号
#    steelID - 钢筋材料号
#    numBars - 截面任何一边的钢筋根数
#    barArea - 钢筋正截面面积 
#    nfCoreY - 核心区在Y方向划分的纤维单元数
#    nfCoreZ - 核心区在Z方向划分的纤维单元数
#    nfCoverY -保护层在Y方向划分的纤维单元数
#    nfCoverZ -保护层在Z方向划分的纤维单元数
#
# 注:
#    混凝土保护层的厚度是常数;
#    在截面任何一边上的钢筋数是相同的;
#    所有钢筋的型号是相同的;
#    保护层在短方向的纤维单元个数设置为1;
# 
proc RCsection {id h b cover coreID coverID steelID numBars barArea nfCoreY nfCoreZ nfCoverY nfCoverZ} {

   # --------Y轴正方向的截面Z轴到保护层外边线的距离------------
   
   set coverY [expr $h/2.0]

   # --------Z轴正方向的截面Y轴到保护层外边线的距离-------------
   
   set coverZ [expr $b/2.0]

   # --------Y轴负方向的截面Z轴到保护层外边线的距离-------------
  
   set ncoverY [expr -$coverY]

   # --------Z轴负方向的截面Y轴到保护层外边线的距离-------------
   
   set ncoverZ [expr -$coverZ]

   # --------相应轴线到混凝土核心区外边线的距离-----------------
   
   set coreY  [expr $coverY-$cover]
   set coreZ  [expr $coverZ-$cover]
   set ncoreY [expr -$coreY]
   set ncoreZ [expr -$coreZ]

   # --------定义纤维截面---------------------------------------------------------------------------------
   section fiberSec $id {

	#----------核心区混凝土--------------------------------------------
        
        #--四边形单元 划分单元截面号 Z方向划分个数 Y方向划分个数   1点X、Y方向坐标    2点X、Y方向坐标    3点X、Y方向坐标    4点X、Y方向坐标 
	patch quadr     $coreID      $nfCoreZ     $nfCoreY     $ncoreY $coreZ    $ncoreY $ncoreZ    $coreY $ncoreZ    $coreY $coreZ
      
	#----------保护层混凝土--------------------------------------------
	        
        #--四边形单元 划分单元截面号 Z方向划分个数 Y方向划分个数   1点X、Y方向坐标    2点X、Y方向坐标    3点X、Y方向坐标    4点X、Y方向坐标 
	patch quadr     $coverID        1         $nfCoverY    $ncoverY $coverZ  $ncoreY  $coreZ    $coreY  $coreZ    $coverY $coverZ
	patch quadr     $coverID        1         $nfCoverY    $ncoreY  $ncoreZ  $ncoverY $ncoverZ  $coverY $ncoverZ  $coreY  $ncoreZ
	patch quadr     $coverID    $nfCoverZ         1        $ncoverY $coverZ  $ncoverY $ncoverZ  $ncoreY $ncoreZ   $ncoreY $coreZ
	patch quadr     $coverID    $nfCoverZ         1        $coreY   $coreZ   $coreY   $ncoreZ   $coverY $ncoverZ  $coverY $coverZ
 
	#----------平行于Z方向钢筋------------------------------------------
       
        # ---直排单元 划分单元材料号 钢筋根数       钢筋面积       起点X方向坐标       起点Y方向坐标      终点X方向坐标       终点Y方向坐标 
	layer straight  $steelID    $numBars      $barArea        $ncoreY             $coreZ             $ncoreY           $ncoreZ
	layer straight  $steelID    $numBars      $barArea        $coreY              $coreZ             $coreY            $ncoreZ

	# ---------计算Y方向的剩余空间---------------------------------------
	
	set spacingY [expr ($coreY-$ncoreY)/($numBars-1)]

	#----------防止重复计算钢筋根数--------------------------------------
	
	set numBars [expr $numBars-2]

	#----------平行于Y方向钢筋-------------------------------------------
	
	layer straight  $steelID    $numBars     $barArea  [expr $coreY-$spacingY]   $coreZ    [expr $ncoreY+$spacingY]    $coreZ
	layer straight  $steelID    $numBars     $barArea  [expr $coreY-$spacingY]   $ncoreZ   [expr $ncoreY+$spacingY]    $ncoreZ
  
   }

}