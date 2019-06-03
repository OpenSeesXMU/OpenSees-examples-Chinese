clear all
load node.out
plot(node(4:end,1)-3,node(4:end,3))
hold on
plot(node(4:end,1)-3,(node(4:end,9)-node(4:end,3)),'r')
plot(node(4:end,1)-3,(node(4:end,15)-node(4:end,9)),'g')
legend('first interstory drift [m]','second interstory drift [m]','third interstory drift [m]')

