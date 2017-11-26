(* ::Package:: *)

(*math for XYUV transformation*)
Rx[\[Alpha]_]:={{1,0,0},{0,Cos[\[Alpha]],-Sin[\[Alpha]]},{0,Sin[\[Alpha]],Cos[\[Alpha]]}};
Ry[\[Alpha]_]:={{Cos[\[Alpha]],0,Sin[\[Alpha]]},{0,1,0},{-Sin[\[Alpha]],0,Cos[\[Alpha]]}};
Rz[\[Alpha]_]:={{Cos[\[Alpha]],-Sin[\[Alpha]],0},{Sin[\[Alpha]],Cos[\[Alpha]],0},{0,0,1}};
Ru[\[Phi],\[Theta]]=Ry[\[Phi]].Rz[\[Theta]]; (*(Rz[-\[Theta]].Ry[-\[Phi]])^-1*)
Rz[-\[Theta]].Ry[-\[Phi]] // MatrixForm
(*Inverse[Ru[\[Phi],\[Theta]]] // FullSimplify // MatrixForm*)
lu={xu,yu,zu}; (*the line projection in Oxu-yu-zu coordinate*)
lxy=Ru[\[Phi],\[Theta]].lu;
Eliminate[x==lxy[[1]] && y==lxy[[2]] && z==lxy[[3]]&&xu==ku zu+bu,{xu,yu,zu}] // FullSimplify
Collect[x Cos[\[Theta]] Cos[\[Phi]]+y Sin[\[Theta]] - (bu+ku z Cos[\[Phi]]+(ku x+z Cos[\[Theta]]) Sin[\[Phi]]),{x,y,z}]
(*function of projecting plane in the Oxyz coordinate*)
ff[\[Theta]_,\[Phi]_,ku_,bu_]:=x(Cos[\[Phi]] Cos[\[Theta]]-ku Sin[\[Phi]])+y Sin[\[Theta]]-z (ku Cos[\[Phi]] + Sin[\[Phi]]Cos[\[Theta]])-bu;
(*solve the intersection line of the U-V projecting planes*)
Solve[Eliminate[{ff[-Pi/6,\[Phi],ku,bu]==0,ff[Pi/6,\[Phi],kv,bv]==0},y],x] // FullSimplify
Solve[Eliminate[{ff[-Pi/6,\[Phi],ku,bu]==0,ff[Pi/6,\[Phi],kv,bv]==0},x],y] // FullSimplify


(*the above solved function of lx: x=k1t z+b1t and ly: y=k2t z+b2t*)
k1t=((ku+kv) Cos[\[Phi]]+Sqrt[3] Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);
b1t=(bu+bv)/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);
k2t=(Sqrt[3] (-ku+kv))/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);
b2t=(Sqrt[3] (-bu+bv) Cos[\[Phi]]+(-2 bv ku+2 bu kv) Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);


(*express {ku, kv, bu, bv} in {k1t, k2t, b1t, b2t}*)
Solve[k1T==((ku+kv) Cos[\[Phi]]+Sqrt[3] Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]])&&
b1T==(bu+bv)/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]])&&
k2T==(Sqrt[3] (-ku+kv))/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]])&&
b2T==(Sqrt[3] (-bu+bv) Cos[\[Phi]]+(-2 bv ku+2 bu kv) Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]),{ku,kv,bu,bv}] // FullSimplify


Rx[\[Theta]_]:={{1,0,0},{0,Cos[\[Theta]],-Sin[\[Theta]]},{0,Sin[\[Theta]],Cos[\[Theta]]}};
Ry[\[Theta]_]:={{Cos[\[Theta]],0,Sin[\[Theta]]},{0,1,0},{-Sin[\[Theta]],0,Cos[\[Theta]]}}; (*note that z-x, not x-z, so the matrix is odd*)
Rz[\[Theta]_]:={{Cos[\[Theta]],-Sin[\[Theta]],0},{Sin[\[Theta]],Cos[\[Theta]],0},{0,0,1}};
R[\[Phi]_,\[Theta]_,\[Psi]_]:=Rz[\[Psi]].Rx[\[Theta]].Ry[\[Phi]]; (*\[Phi] goes first*)
R[\[Phi],\[Theta],\[Psi]] // MatrixForm
R[\[Phi],0,0] // MatrixForm;
R0[\[Phi]_,\[Theta]_,\[Psi]_]:=Ry[\[Psi]].Rz[\[Theta]].Ry[\[Phi]]; (*\[Phi] goes first*)
R0[\[Phi],\[Theta],\[Psi]] // MatrixForm;
r={353.,375,-85.9};
R[0.398282,0,0].r


(*the hit point of two skew lines (two points in each line that has the shortest distance) - 1*)
f[t1_,t2_]:=((m1 t1+X1)-(m2 t2+X2))^2+((n1 t1+Y1)-(n2 t2+Y2))^2+((p1 t1+Z1)-(p2 t2+Z2))^2;
Collect[D[f[t1,t2],t1],{t1,t2}]
Collect[D[f[t1,t2],t2],{t1,t2}]
Solve[(2 m1^2+2 n1^2+2 p1^2) t1+(-2 m1 m2-2 n1 n2-2 p1 p2) t2+2 m1 X1-2 m1 X2+2 n1 Y1-2 n1 Y2+2 p1 Z1-2 p1 Z2==0 && 
 (-2 m1 m2-2 n1 n2-2 p1 p2) t1+(2 m2^2+2 n2^2+2 p2^2) t2-2 m2 X1+2 m2 X2-2 n2 Y1+2 n2 Y2-2 p2 Z1+2 p2 Z2==0, {t1, t2}] // FullSimplify


(*the hit point of two skew lines (two points in each line that has the shortest distance) - 2*)
tt1=-(-4 (m2^2+n2^2+p2^2) (m1 (X1-X2)+n1 (Y1-Y2)+p1 (Z1-Z2))+4 (m1 m2+n1 n2+p1 p2) (m2 (X1-X2)+n2 (Y1-Y2)+p2 (Z1-Z2)))/
(4 (m1 m2+n1 n2+p1 p2)^2-4 (m1^2+n1^2+p1^2) (m2^2+n2^2+p2^2));
tt2=(-m1 (n1 n2+p1 p2) (X1-X2)+(n2 p1-n1 p2) (p1 Y1-p1 Y2-n1 Z1+n1 Z2)+m1^2 (n2 Y1-n2 Y2+p2 Z1-p2 Z2)+m2 (n1^2 (X1-X2)+m1 n1 (-Y1+Y2)+p1 (p1 X1-p1 X2-m1 Z1+m1 Z2)))/
(m2^2 (n1^2+p1^2)+(n2 p1-n1 p2)^2-2 m1 m2 (n1 n2+p1 p2)+m1^2 (n2^2+p2^2));
P1=m1 tt1+X1 // FullSimplify
P2=n1 tt1+Y1 // FullSimplify
P3=p1 tt1+Z1 // FullSimplify
Q1=m2 tt2+X2 // FullSimplify
Q2=n2 tt2+Y2 // FullSimplify
Q3=p2 tt2+Z2 // FullSimplify
dd=Sqrt[(P1-Q1)^2+(P2-Q2)^2+(P3-Q3)^2]



