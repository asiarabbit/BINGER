(* ::Package:: *)

koi=-1/ki; boi=x1-ki z1;
ko1=-1/k1; bo1=x2-k1 z2;
Solve[xo == koi zo+boi && xo == ko1 zo+bo1, {zo, xo}] // FullSimplify
