#  ------------------------------------------------------------------------------------
#  | 厦门大学       古泉、卢佳盛、林陪晖                                                  |
#  | 2011年3月                                                                          |
#  |                                                                                    |
#  | 算例6、   钢筋混凝土柱的相关曲线研究                                                 |
#  | 单位: m、sec、kN、kPa                                                               |
#  |                                                                                    |
#  |                                                                                    |
#  ------------------------------------------------------------------------------------
# 
#                           _____
#                           |   |       /_
#                           |   |       \ \ M
#                           |   |          \
#                           |   |          |                   
#         --------------- 1 |   | 2<-------|----    P
#                           |   |          |
#                           |   |          /
#                           |   |         /
#                           -----
#

set P 0.0;                                                                                                    # 设置初始轴向力
set incrP 20;
set Flag 1;

set fileID [open "mp.out"  "w"]

set criterion -0.003


# ----- 设置钢筋参数 -----------------------------
	set fy 4.14e5;                                                                                         # 屈服强度
	set E  2.0e8;                                                                                         # 杨氏模量

# ----- 定义柱截面  ------------------------------
	set colWidth 0.4064
	set colDepth 0.4064
	set cover    0.0635
	
# ----- 定义钢筋截面 ------------------------------
	set d [expr $colDepth-$cover]; 
        set As   6.45e-4;                                                                                     # 钢筋面积
        set nbars 4;                                                                                          # 每排钢筋数量

# -----估算屈服曲率 ------------------------------
	set epsy [expr $fy/$E];                                                                               # 钢筋屈服时的应变
	set Ky [expr 2.10*$epsy/$d]
	puts "Estimated yield curvature: $Ky"
	set mu 15
	set maxK  [expr $Ky*$mu]


# ------轴力加载---------------------------------------------------------------------------------------

while { $Flag > 0  && $P <= 10000}  {
	
	# ------------------ 读取model.tcl文件 --------------------
	source model.tcl

	# ------------------获取轴向应变和曲率 ---------------------
	set e1 [ nodeDisp 2  1]
	set e3 [ nodeDisp 2  3]

	set Flag  1;

	if { $e1 <  $criterion }  { 
          # ----------------检查轴向压力是否太大---------------------
          puts " 轴向压力太大..."
	  
	  set Flag 0  ;                                                                                         # 结束循环

	} else {
	  # -----------------定义弯矩 ---------------------------------
		 pattern Plain 2 "Linear" {
			load 2 0.0 0.0 1.0
	         }
     
		 set numIncr 1000; 
		 set dK [expr $maxK/$numIncr]
		 puts "dK: $dK"
		
		 # --------截面分析:采用位移控制加载,作用在节点2的第3个自由度-----------------------------
		
		 # ----------------------------节点号 自由度号 位移增量 <因子 最小位移增量 最大位移增量> 
                 integrator DisplacementControl  2       3      $dK      1      $dK         $dK;
		 
		 set maxStrain [expr $e1-$e3*$colDepth/2.0];                                                     #计算最大压应变 
		   
		 set step 0  

		 # -------增加节点2的转角直到达到最大应变标准-------------------------------------------           
		 while  { $maxStrain>$criterion } {
		  	set ok [analyze 1]
			if { $ok<0} { exit; }
			set step [expr $step+1] 
		  
			set e1 [nodeDisp 2  1]
			set e3 [nodeDisp 2  3]
			set maxStrain [expr $e1-$e3*$colDepth/2.0]
		    }

	    set moment [getTime];                                                                               # 时间乘以加载形式2的系数等于施加的弯矩
            

	    puts "step: $step,  maxStrain: $maxStrain, Moment: $moment"

	    #-------------把结果记录到文件中-------------------------------------------------------------
            puts -nonewline $fileID "\n";       
	    set data " $moment       $P"
	    puts -nonewline $fileID $data ;  
		 
	}   ;                                                                                                      

	set P [expr $P + $incrP]; # update P
}  ; 

close $fileID
puts "弯矩-曲率分析完成..."


 

 
 
 
 


