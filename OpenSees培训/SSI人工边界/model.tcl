    
#  | 单位: m、ton、sec、kN、kPa                                                          |
wipe
model BasicBuilder -ndm 2 -ndf 3
set framemass1 15.0
set framemass2 30.0
set framemass3  4.0



node  1   7.0   0.0   -mass $framemass2 $framemass2 0.0
node  2   7.0   3.6   -mass $framemass2 $framemass2 0.0
node  3  7.0  -3.6   -mass $framemass3 $framemass3 0.0


# ----------------- 记录结果--------------------------------------

recorder Node -file disp3.out   -time  -node 3  -dof 1 2 disp   
recorder Node -file disp2.out   -time  -node 2  -dof 1 2 disp 
recorder Node -file disp1.out   -time  -node 1  -dof 1 2 disp 

# -------------------定义重力荷载----------------------------------

set upperload1 [expr -$framemass1*10.0]
set upperload2 [expr -$framemass2*10.0]
set download3  [expr -$framemass3*10.0]


#pattern Plain 2 {Series -time {0.0 2.0 100000.0} -values {0.0 1.0 1.0} } {
pattern Plain 2 "Constant"  {
load 1 0.0 $upperload2 0
load 2 0.0 $upperload2 0
load 3 0.0 $download3 0
}



# ---------------------------------定义材料 --------------------------------

# -------------------------------- 上部框架---------------------------------

#----------混凝土保护层---------------

uniaxialMaterial Concrete01  1     -27588.5    -0.002      0.0      -0.008
                                   
# ---------混凝土核心区---------------

uniaxialMaterial Concrete01  2     -34485.6    -0.004     -20691.4  -0.014

#  刚度比 b=Hkin/(E+Hkin)=0.008                                                                              
#  ---------钢筋            材料号    弹模       屈服强度    各向同性强化模量 运动强化模量
uniaxialMaterial Hardening   3      2.0e8       248200.          0.0         1.6129e6 

# ---------------------------------下部基础 ---------------------------------

#----------混凝土保护层---------------

uniaxialMaterial Concrete01  4     -27588.5     -0.002      0.0      -0.008
                                    
# ---------混凝土核心区---------------

uniaxialMaterial Concrete01  5     -34485.6     -0.004    -20691.4   -0.014


# 刚度比 b=Hkin/(E+Hkin)=0.008
#  ---------钢筋            材料号    弹模       屈服强度    各向同性强化模量 运动强化模量
uniaxialMaterial Hardening   6      2.0e8       248200.          0.0         1.6129e6 


# ----------------------------------定义截面 ------------------------------

# ----------------------------------上部框架 ------------------------------

# -----------中柱截面------------------

section Fiber  1 {
   patch quad  2    1   12 -0.2500  0.2000 -0.2500 -0.2000  0.2500 -0.2000  0.2500  0.2000
   patch quad  1    1   14 -0.3000 -0.2000 -0.3000 -0.2500  0.3000 -0.2500  0.3000 -0.2000
   patch quad  1    1   14 -0.3000  0.2500 -0.3000  0.2000  0.3000  0.2000  0.3000  0.2500
   patch quad  1    1    2 -0.3000  0.2000 -0.3000 -0.2000 -0.2500 -0.2000 -0.2500  0.2000
   patch quad  1    1    2  0.2500  0.2000  0.2500 -0.2000  0.3000 -0.2000  0.3000  0.2000
   layer straight  3    3   0.000645 -0.2000   0.2000    -0.2000   -0.2000
   layer straight  3    3   0.000645  0.2000   0.2000     0.2000   -0.2000
}

# ---------- 配筋率:  Ag/As = 6*0.000645/(0.6*0.5) =1.29%
# ---------------------------下部基础 ----------------------------------------

# -----------中柱截面------------------

section Fiber  2 {
   patch quad  5    1   12 -0.2500  0.2000 -0.2500 -0.2000  0.2500 -0.2000  0.2500  0.2000
   patch quad  4    1   14 -0.3000 -0.2000 -0.3000 -0.2500  0.3000 -0.2500  0.3000 -0.2000
   patch quad  4    1   14 -0.3000  0.2500 -0.3000  0.2000  0.3000  0.2000  0.3000  0.2500
   patch quad  4    1    2 -0.3000  0.2000 -0.3000 -0.2000 -0.2500 -0.2000 -0.2500  0.2000
   patch quad  4    1    2  0.2500  0.2000  0.2500 -0.2000  0.3000 -0.2000  0.3000  0.2000
   layer straight  6    3   0.000645 -0.2000   0.2000    -0.2000   -0.2000
   layer straight  6    3   0.000645  0.2000   0.2000     0.2000   -0.2000
}

# -----------------------------------------------------------------------------------------------------
#------------设置高斯点的个数-----------------
set nP 4

geomTransf Linear 1

# ----------------- 定义单元 -----------------------------------------------------

# --------------------------- 上部框架--------------------------------------------

#------------柱单元---------------------------

element dispBeamColumn  1   1   2   $nP   1      1
element dispBeamColumn  2  1   3    $nP   2      1


# -----------------------------记录结果-----------------------------------------------

recorder Element  -ele 1 2  -file Deformation12.out -time section 1 deformations
recorder Element  -ele 1 2  -file Force12.out -time section 1 force

recorder Element -ele 1 -time -file steelstress1.out  section 1 fiber -0.2286 0.2286 stress
recorder Element  -ele 1 -time -file steelstrain1.out section 1 fiber -0.2286 0.2286  strain

recorder Element  -ele 1 -time -file concretestress1.out  section 1 fiber 0.0 0.0 stress
recorder Element  -ele 1 -time -file concretestrain1.out section 1 fiber 0.0 0.0  strain

 

#------------------------------------------------------------------
#|                                                                |
#|           地基土                                               |
#|                                                                |
#------------------------------------------------------------------

set g -19.6
model basic -ndm 2 -ndf 2

# ----- 定义地基土节点 -----------


node 4    2.4      -7.2
node 5    7        -7.2
node 6    10.6     -7.2
node 7    2.4       -3.6
node 8    7         -3.6
node 9    10.6      -3.6
node 10     2.4       0
node 11     7         0
node 12     10.6      0


node 21    2.4      -7.2
node 22    7        -7.2
node 23    10.6     -7.2
node 24    2.4       -3.6
node 26    10.6      -3.6
node 25     2.4       0
node 27     10.6      0


# ------------------------------------------------------------
#|   定义地基土的边界条件                                      |
# ------------------------------------------------------------

fix 21 1 1
fix 22 1 1
fix 23 1 1
fix 24 1 1
fix 26 1 1
fix 25 1 1
fix 27 1 1

#定义人工边界

# lateral: normal K 
uniaxialMaterial	Elastic	1001	3600000
uniaxialMaterial	Elastic	1002	7197751.054
uniaxialMaterial	Elastic	1003	7191016.84
uniaxialMaterial	Elastic	1004	7179835.031


# lateral: normal C
uniaxialMaterial	Viscous	2001	623538	1
uniaxialMaterial	Viscous	2002	1247076	1
uniaxialMaterial	Viscous	2003	1247076	1
uniaxialMaterial	Viscous	2004	1247076	1

# lateral: tangent K
uniaxialMaterial	Elastic	1101	1800000
uniaxialMaterial	Elastic	1102	3598875.527
uniaxialMaterial	Elastic	1103	3595508.42
uniaxialMaterial	Elastic	1104	3589917.516

# lateral: tangent C
uniaxialMaterial	Viscous	2101	360000	1
uniaxialMaterial	Viscous	2102	720000	1
uniaxialMaterial	Viscous	2103	720000	1
uniaxialMaterial	Viscous	2104	720000	1


#bottom :normal K
uniaxialMaterial	Elastic	1201	1800000
uniaxialMaterial	Elastic	1202	3598875.527
uniaxialMaterial	Elastic	1203	3595508.42


#bottom: normal C
uniaxialMaterial	Viscous	2201	360000	1
uniaxialMaterial	Viscous	2202	720000	1
uniaxialMaterial	Viscous	2203	720000	1

#bottom :tangent K
uniaxialMaterial	Elastic	3201	1800000
uniaxialMaterial	Elastic	3202	3598875.527
uniaxialMaterial	Elastic	3203	3595508.42


#bottom: tangent C
uniaxialMaterial	Viscous	4201	360000	1
uniaxialMaterial	Viscous	4202	720000	1
uniaxialMaterial	Viscous	4203	720000	1

#定义零长度单元
element	zeroLength	101	25	10	  -mat	 1001  2001	 1101	2101	-dir	1	1	2	2
element	zeroLength	102	24	7	  -mat	 1002  2002	 1102	2102	-dir	1	1	2	2
element	zeroLength	103	12	27	  -mat	 1003  2003	 1103	2103	-dir	1	1	2	2
element	zeroLength	104	9	26	  -mat	 1004  2004	 1104	2104	-dir	1	1	2	2

element	zeroLength	105	4	21	  -mat	 1201  2201	 3201	4201	-dir	1	1	2	2
element	zeroLength	106	5	22	  -mat	 1202  2202	 3202	4202	-dir	1	1	2	2
element	zeroLength	107	6	23	  -mat	 1203  2203	 3203	4203	-dir	1	1	2	2




#----------------                 材料号 维数 饱合土质量密度  剪切模量   体积模量  屈服强度      最大切应变
nDMaterial MultiYieldSurfaceClay   101   2       2.0         54450	1.6e5     33.           .1          

                                    
# --------弹性混凝土地基-------------------------------- 
                                            
nDMaterial MultiYieldSurfaceClay   100   2       2.0          2e7	1.0e6    21000.  50.0     0 100 0   2
# 混凝土强度摘自'钢筋混凝土设计'  Chu-Kia Wang P16. Ec=22900~29600 Mpa. fc=21~35 Mpa

 


# -----------------------------------------------------
# |        定义地基土单元                              |
# -----------------------------------------------------

# -----第四层土单元         单元号      节点号        土层厚度    平面应变   NDM材料号 表面压力 单元质量密度  体力
element quad  3   4   5   8   7       3.60     "PlaneStrain" 101     0       0.0        0  $g
element quad  4   5   6   9   8   3.60     "PlaneStrain" 101     0       0.0        0  $g
element quad  5   7   8   11   10   3.60     "PlaneStrain" 101     0       0.0        0  $g
element quad  6   8   9   12   11   3.60     "PlaneStrain" 101     0       0.0        0  $g


equalDOF  3 8 1 2
equalDOF  1 11 1 2



 
foreach theNode { 6 5 4 7 8 9 10 11 12} {
    
    recorder Node -file $theNode.out -time -node $theNode -dof 1 2 disp 
        
}

 
recorder Element -ele 3 -time -file stress3.out -time   material 2 stress 
recorder Element -ele 4 -time -file stress4.out -time   material 2 stress 
recorder Element -ele 5 -time -file stress5.out -time   material 2 stress 
recorder Element -ele 6 -time -file stress6.out -time   material 2 stress 




# ------------------------- 静力分析 ----------------------------
constraints Transformation
numberer RCM
test NormDispIncr 1.E-6 25  2
integrator LoadControl 1 1 1 1
algorithm Newton
system BandGeneral

 

analysis Static 
analyze 3
 
puts "soil gravity nonlinear analysis completed ..."

# ------------------------- 动力分析-------------------------------
wipeAnalysis

constraints Transformation
test NormDispIncr 1.E-6 25  2
algorithm Newton
numberer RCM
system BandGeneral
#-------------β=0.55,γ=(0.55+0.5)^2/4=0.275625-----------
integrator Newmark  0.55 0.275625 
 
 analysis Transient

set startT [clock seconds]

# time series
#

timeSeries Path  1 -dt 0.01 -filePath elcentro.txt -factor 1.0
set loadMag 1E5

pattern Plain 111 1 {
load 4      $loadMag     0.0   
load 5      $loadMag     0.0   
load 6      $loadMag     0.0   
load 7      $loadMag     0.0   
load 9      $loadMag     0.0   
load 10      $loadMag     0.0   
load 12      $loadMag     0.0   
}

#pattern UniformExcitation    1     1    -accel "Series -factor 3 -filePath elcentro.txt -dt 0.01"
analyze 2400 0.005

set endT [clock seconds]

puts "完成时间: [expr $endT-$startT] seconds."







