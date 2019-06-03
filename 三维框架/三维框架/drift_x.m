clear all
load node.out
plot(node(4:end,1)-3,node(4:end,2))
hold on
plot(node(4:end,1)-3,(node(4:end,8)-node(4:end,2)),'r')
plot(node(4:end,1)-3,(node(4:end,14)-node(4:end,8)),'g')
legend('first interstory drift [m]','second interstory drift [m]','third interstory drift [m]')

