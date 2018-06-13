% Author: Pradeep Singh
% Filename: ImgToText.m
% Date: May 10, 2018

% Description: This program stores the RGB image in a matrix form in Input.txt file. 
% This program also reads Output.txt file and shows the Gray scale image.

clear;
clc;
close all;

I = imread('./image.jpg');

figure();
imshow(I);
title('Original RGB image');

fptr = fopen('./Input.txt','w');
disp(['size = ',num2str(size(I))]);

for i = 1:size(I,1) %For each row   
    for j = 1:size(I,2) %For each column       
        for k = 1:size(I,3) %For each plane (R,G,B planes)    
            fprintf(fptr,'%d ',I(i,j,k));            
            
        end        
    end   
    fprintf(fptr,'\n'); %Next line
end

fclose(fptr);

J=rgb2gray(I);
figure();
imshow(J);    %Show Gray Image
title('Gray Image created using MATLAB');

Mat = uint8(importdata('./Output.txt'));
figure();
imshow(Mat);    %Reconstructed image
title('Gray image created using CUDA');

Error = J-Mat;  %Error between MATLAB generated Gray scale image and ImgConvert.cu program's generated
