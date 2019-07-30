wipe;
model basic -ndm 2 -ndf 3
node 1 0.0 0.0
node 3 0.0 1.0
node 2 1.0 0.0
node 4 1.0 1.0
nDMaterial BoundingSurfaceSand 1 2 1.90 0.5066 200.0 0.818 0.05 0.5423 0.75 0.0\
1.0 1.9 0.1811 101.3250 1.01537 0.934 0.019 0.7 3.5 0.75 0.4 0.0 0.0
element quadUP 1 1 2 4 3 1.0 1 2.2e6 1 5.09e-8 5.09e-8 0.0 -480.0 0
recorder Element -ele 1 -time -file stress.out material 1 stress
recorder Element -ele 1 -time -file strain.out material 1 strain
fix 1 1 1 0
fix 3 0 0 1
fix 2 1 1 0
fix 4 0 0 1
equalDOF 3 4 1 2
numberer RCM
system ProfileSPD
test NormDispIncr 1.0e-8 50 1
algorithm KrylovNewton
constraints Transformation
integrator Newmark 1.5 1.
analysis VariableTransient
analyze 5 5.0e3 [expr 5.0e3/100] 5.0e3 20;
updateMaterialStage -material 1 -stage 1000
analyze 3 5.0e3 [expr 5.0e3/100] 5.0e3 20;
wipeAnalysis
set P_max 20.0
pattern Plain 1 "Series -time {40000.0 40001.0 40002.0 40003.0
40004.0 40005.0 40006.0 40007.0 40008.0 40009.0 40010.0 40011.
40012. 40013. 40014. 40015. 40016. 40017. 40018. 40019.
40020. 40021. 40022. 40023. 40024. 40025. 40026. 40027.
40028. 40029. 40030. 40031. 40032. 40033. 40034. 40035.
40036. 40037. 40038. 40039. 40040. 40041.} -values {0.0
-1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0
-1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0
-1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0}" {
load 3 $P_max 0.0 0
load 4 $P_max 0.0 0}
constraints Transformation
test NormDispIncr 1.0e-8 50 0
numberer RCM
algorithm Newton
system BandGeneral
rayleigh 0.0 0.0 0.02 0.0
integrator Newmark 0.6 [expr pow(0.6+0.5, 2)/4]
analysis Transient
analyze 4000 0.01







