(* ::Package:: *)

k1=.;b1=.;zh=.;xh=.;
Solve[x0 == ki z0+bi && x1 == ki z1+bi, {ki, bi}] // Simplify
Solve[xh == ki zh+bi && xh == k1 zh+b1, {zh, xh}] // Simplify


ki = (x0-x1)/(z0-z1);    bi = (x1 z0-x0 z1)/(z0-z1);
zh = (-b1+bi)/(k1-ki);   xh = (bi k1-b1 ki)/(k1-ki);
Solve[(z1-zh)^2+(x1-xh)^2 == (z2-zh)^2+(x2-xh)^2,{x1}]; // Simplify
