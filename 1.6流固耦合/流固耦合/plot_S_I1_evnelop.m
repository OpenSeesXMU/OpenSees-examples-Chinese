clear all
close all
clc



I1_envelop = -2.0684e6:2.0e5:10.0e7;
       alpha = 2.6614e7;
       theta = 0.11;
fe = alpha+theta*I1_envelop; 

% subplot(2,2,1)



for number=1:8
figure (number) 
str1='stress'
str2=mat2str(number)
str3= '.out'
str=strcat(str1,str2,str3)
stress = load(str);
stress = stress(:,2:end);  % compression is positive
I1 = stress(:,1)+stress(:,2)+stress(:,3);
hold on
s=stress;
normS = zeros( length(I1),1);
   

for i = 1:length(I1) 
    for j = 1:3
          s(i,j) = stress(i,j) - I1(i)*1/3; 
          normS(i) =normS(i)+ s(i,j)^2;
    end
    
    for j =4:6
        normS(i) =normS(i)+ 2*s(i,j)^2;
    end
        normS(i) = normS(i)^0.5;

end

plot(I1, normS,'k-o')
hold on
plot(I1_envelop, fe, 'r-','lineWidth',2)
plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
xlabel('I_1 [MPa]', 'FontSize',18)
ylabel('||s|| [MPa]', 'FontSize',18)
legendstr1='Gauss point #'
legendstr2=mat2str(number)
legendstr=strcat(legendstr1,legendstr2)

legend(legendstr,2);
axis([-2e7 8e7 0  4e7]);
ax=gca;
set(ax, 'FontSize',18)
box on

end

% 
% figure (2)
% stress = load('stress2.out');
% stress = stress(:,2:end);  % compression is positive
% I1 = stress(:,1)+stress(:,2)+stress(:,3);
% hold on
% s=stress;
% normS = zeros( length(I1),1);
%    
% 
% for i = 1:length(I1) 
%     for j = 1:3
%           s(i,j) = stress(i,j) - I1(i)*1/3; 
%           normS(i) =normS(i)+ s(i,j)^2;
%     end
%     
%     for j =4:6
%         normS(i) =normS(i)+ 2*s(i,j)^2;
%     end
%         normS(i) = normS(i)^0.5;
% 
% end
% 
% plot(I1, normS,'k-o')
% hold on
% plot(I1_envelop, fe, 'r-','lineWidth',2)
% plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
% xlabel('I_1 [MPa]')
% ylabel('||s|| [MPa]')
% legend('高斯点 #2')
% box on
% 
% figure (3) 
% stress = load('stress3.out');
% stress = stress(:,2:end);  % compression is positive
% I1 = stress(:,1)+stress(:,2)+stress(:,3);
% hold on
% s=stress;
% normS = zeros( length(I1),1);
%    
% 
% for i = 1:length(I1) 
%     for j = 1:3
%           s(i,j) = stress(i,j) - I1(i)*1/3; 
%           normS(i) =normS(i)+ s(i,j)^2;
%     end
%     
%     for j =4:6
%         normS(i) =normS(i)+ 2*s(i,j)^2;
%     end
%         normS(i) = normS(i)^0.5;
% 
% end
% 
% plot(I1, normS,'k-o')
% hold on
% plot(I1_envelop, fe, 'r-','lineWidth',2)
% plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
% xlabel('I_1 [MPa]')
% ylabel('||s|| [MPa]')
% legend('高斯点 #3')
% box on
% 
% figure (4) 
% stress = load('stress4.out');
% stress = stress(:,2:end);  % compression is positive
% I1 = stress(:,1)+stress(:,2)+stress(:,3);
% hold on
% s=stress;
% normS = zeros( length(I1),1);
%    
% 
% for i = 1:length(I1) 
%     for j = 1:3
%           s(i,j) = stress(i,j) - I1(i)*1/3; 
%           normS(i) =normS(i)+ s(i,j)^2;
%     end
%     
%     for j =4:6
%         normS(i) =normS(i)+ 2*s(i,j)^2;
%     end
%         normS(i) = normS(i)^0.5;
% 
% end
% 
% plot(I1, normS,'k-o')
% hold on
% plot(I1_envelop, fe, 'r-','lineWidth',2)
% plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
% xlabel('I_1 [MPa]')
% ylabel('||s|| [MPa]')
% legend('高斯点 #4')
% box on
% 
% figure (5) 
% stress = load('stress4.out');
% stress = stress(:,2:end);  % compression is positive
% I1 = stress(:,1)+stress(:,2)+stress(:,3);
% hold on
% s=stress;
% normS = zeros( length(I1),1);
%    
% 
% for i = 1:length(I1) 
%     for j = 1:3
%           s(i,j) = stress(i,j) - I1(i)*1/3; 
%           normS(i) =normS(i)+ s(i,j)^2;
%     end
%     
%     for j =4:6
%         normS(i) =normS(i)+ 2*s(i,j)^2;
%     end
%         normS(i) = normS(i)^0.5;
% 
% end
% 
% plot(I1, normS,'k-o')
% hold on
% plot(I1_envelop, fe, 'r-','lineWidth',2)
% plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
% xlabel('I_1 [MPa]')
% ylabel('||s|| [MPa]')
% legend('高斯点 #5')
% box on
% 
% figure (6) 
% stress = load('stress4.out');
% stress = stress(:,2:end);  % compression is positive
% I1 = stress(:,1)+stress(:,2)+stress(:,3);
% hold on
% s=stress;
% normS = zeros( length(I1),1);
%    
% 
% for i = 1:length(I1) 
%     for j = 1:3
%           s(i,j) = stress(i,j) - I1(i)*1/3; 
%           normS(i) =normS(i)+ s(i,j)^2;
%     end
%     
%     for j =4:6
%         normS(i) =normS(i)+ 2*s(i,j)^2;
%     end
%         normS(i) = normS(i)^0.5;
% 
% end
% 
% plot(I1, normS,'k-o')
% hold on
% plot(I1_envelop, fe, 'r-','lineWidth',2)
% plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
% xlabel('I_1 [MPa]')
% ylabel('||s|| [MPa]')
% legend('高斯点 #6')
% box on
% 
% figure (7) 
% stress = load('stress4.out');
% stress = stress(:,2:end);  % compression is positive
% I1 = stress(:,1)+stress(:,2)+stress(:,3);
% hold on
% s=stress;
% normS = zeros( length(I1),1);
%    
% 
% for i = 1:length(I1) 
%     for j = 1:3
%           s(i,j) = stress(i,j) - I1(i)*1/3; 
%           normS(i) =normS(i)+ s(i,j)^2;
%     end
%     
%     for j =4:6
%         normS(i) =normS(i)+ 2*s(i,j)^2;
%     end
%         normS(i) = normS(i)^0.5;
% 
% end
% 
% plot(I1, normS,'k-o')
% hold on
% plot(I1_envelop, fe, 'r-','lineWidth',2)
% plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
% xlabel('I_1 [MPa]')
% ylabel('||s|| [MPa]')
% legend('高斯点 #7')
% box on
% 
% 
% figure (8) 
% stress = load('stress4.out');
% stress = stress(:,2:end);  % compression is positive
% I1 = stress(:,1)+stress(:,2)+stress(:,3);
% hold on
% s=stress;
% normS = zeros( length(I1),1);
%    
% 
% for i = 1:length(I1) 
%     for j = 1:3
%           s(i,j) = stress(i,j) - I1(i)*1/3; 
%           normS(i) =normS(i)+ s(i,j)^2;
%     end
%     
%     for j =4:6
%         normS(i) =normS(i)+ 2*s(i,j)^2;
%     end
%         normS(i) = normS(i)^0.5;
% 
% end
% 
% plot(I1, normS,'k-o')
% hold on
% plot(I1_envelop, fe, 'r-','lineWidth',2)
% plot([-2.0684e6 -2.06386e6], [0 2.64e7], 'r-','lineWidth',2)
% xlabel('I_1 [MPa]')
% ylabel('||s|| [MPa]')
% legend('高斯点 #8')
% box on
