wipe; 
model basic -ndm 2 -ndf 3

node 1 0.0 0.0
node 2 0.0 0.0

fix 1 1 1 1
fix 2 0 1 0


# ----------定义截面材料  -----------------------------------------------------------------

# ----------核心区混凝土    材料号   抗压强度   峰值应变    屈曲强度    屈曲时的应变 
uniaxialMaterial Concrete01  1      -3.45e4    -0.004     -2.07e4      -0.014

# ----------保护层混凝土    材料号   抗压强度   峰值应变    屈曲强度    屈曲时的应变 
uniaxialMaterial Concrete01  2      -3.45e4    -0.002      0.0         -0.006

#  ---------钢筋            材料号  屈服强度      弹模      刚度比
uniaxialMaterial Steel01     3       $fy          $E       0.01



#  -------------- 定义非线性柱钢筋混凝土截面--------------------------------------------------------------
#
#                           y
#                         /\
#                         |     
#              ---------------------- 2     ---   ---
#              |   o     o     o    |        |    _|_ cover
#              |                    |        |     |
#              |                    |        |     |
#     x <---   |          +         |        H     |
#              |                    |        |     |
#              |                    |        |    _|_
#              |   o     o     o    |        |     |   cover
#            1 ----------------------       ---   ---
#              |-------- B ---------|
#
#  



set y1 [expr $colDepth/2.0]
set z1 [expr $colWidth/2.0]

section Fiber 1 {

    # ---------划分纤维单元--------------------------------------------
   
    #----------核心区混凝土--------------------------------------------
    patch rect          1         10            1          [expr $cover-$y1] [expr $cover-$z1] [expr $y1-$cover] [expr $z1-$cover]

    # ---------保护层混凝土--------------------------------------------
    patch rect          2         10            1          [expr -$y1]       [expr $z1-$cover]       $y1                $z1
    patch rect          2         10            1          [expr -$y1]       [expr -$z1]             $y1         [expr $cover-$z1]
    patch rect          2          2            1          [expr -$y1]       [expr $cover-$z1] [expr $cover-$y1] [expr $z1-$cover]
    patch rect          2          2            1          [expr $y1-$cover] [expr $cover-$z1]       $y1         [expr $z1-$cover]
 
    #----------钢筋----------------------------------------------------
    layer straight      3       $nbars         $As         [expr $y1-$cover] [expr $z1-$cover] [expr $y1-$cover] [expr $cover-$z1]
    layer straight      3       $nbars         $As         [expr $cover-$y1] [expr $z1-$cover] [expr $cover-$y1] [expr $cover-$z1]
}    


# ---------- 定义非线性纤维单元------------------------------------------------------------------------------

# ------------------------- 单元号 节点号    节点号  截面号             X轴定位向量     Y轴定位向量
   element zeroLengthSection  1       1       2      1     -orient   1    0     0     0   1    0

# ---------- 设置轴向力  -----------------------------------------------------------------------------------

set axialLoad -$P

puts "axialLoad: $axialLoad"


# ---------- 轴向力加载-------------------------------
pattern Plain 1 "Constant" {
	load 2 $axialLoad 0.0 0.0
}

# ---------- 定义分析参数 -----------------------------------------------------------------------------------
integrator LoadControl 0.
system BandGeneral
test NormUnbalance 1.0e-9 10
numberer Plain
constraints Plain
algorithm Newton
analysis Static

#----------- 轴向力分析运行一次------------------------
set ok [analyze 1]
if { $ok <0 } { puts "Non-converged solution"; exit }


loadConst -time 0.0  	


 
